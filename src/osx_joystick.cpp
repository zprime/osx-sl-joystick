/*
Copyright (c) 2012, Zebb Prime and The University of Adelaide
All rights reserved.
 
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the organization nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ZEBB PRIME OR THE UNIVERSITY OF ADELAIDE BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "osx_joystick.hpp"

#define UNUSED(x) (void)(x)

#ifdef ERROR_OUT
  #include <cstdio>
  #define ERR_PRINTF(...) fprintf(stderr,__VA_ARGS__)
#else
  #define ERR_PRINTF(...)
#endif

#ifdef DEBUG
  #include <iostream>
  #include <cstdio>
  #include "dumpjoystick.hpp"
  #define DBG_PRINTF(...) fprintf(stdout,__VA_ARGS__)
#else
  #define DBG_PRINTF(...)
#endif

/**
 * \brief Joystick constructor
 */
 Joystick::Joystick()
 {
   myManager = NULL;
   myElements = NULL;
   myDevice = NULL;
 }

/**
 * \brief Joystick destructor
 */
Joystick::~Joystick()
{
  if( myElements != NULL )
  {
    CFRelease( myElements );
    myElements = NULL;
  }
  if( myDevice != NULL )
  {
//    CFRelease( myDevice );  // Causes segfault 11?
    myDevice = NULL;
  }
  if( myManager != NULL )
  {
    IOHIDManagerClose(myManager, kIOHIDOptionsTypeNone);  // Ignore output.
    CFRelease(myManager);
    myManager = NULL;
  }
}
 
 /**
  * \brief Initialise the Joystick
  * 
  * \input joyid - Joystick ID to open
  *
  * \return true if successful, false if unsuccessful (such as the joystick doesn't exist)
  */
bool Joystick::Initialise( size_t joyid )
{
  if( !InitialiseJoyManager() )
  {
    ERR_PRINTF("Failed to initialise the IO HID Manager.\n");
    return false;
  }
  else DBG_PRINTF("Joystick::Initialise - Successfully opened the IO HID Manager.\n");
  
  // Clear the output, buttons and axes storage
  if( !myButtons.empty() )
  {
    myButtons.erase( myButtons.begin(), myButtons.end() );
  }
  
  // Open the device references
  CFSetRef deviceRefs = IOHIDManagerCopyDevices(myManager);
  
  // If the device references are empty (NULL), it means there are no devices
  if( deviceRefs == NULL ) return false;
  
  // Number of available devices
  CFIndex numDevices = CFSetGetCount( deviceRefs );
  
  // Check to make sure the asked for device exists
  if( joyid >= (size_t)numDevices )
  {
    DBG_PRINTF("Joystick::Initialise - Asked for device %i, but only %i devices are available.\n",
                                                            (int)joyid, (int)numDevices );
    CFRelease( deviceRefs );
    return false;
  }
  
  // Move the device references into a vector
  vector<const void *> devices (numDevices, 0);
  CFSetGetValues( deviceRefs, &devices.front() );
  CFRelease( deviceRefs );
  
  // Isolate the selected device
  myDevice = (IOHIDDeviceRef)devices[ joyid ];
  
  myElements = IOHIDDeviceCopyMatchingElements( myDevice, NULL, kIOHIDOptionsTypeNone );
  // Test to make sure elements are valid                                                                
  if( myElements == NULL )
  {
    ERR_PRINTF("Selected device has no elements. Weird.\n");
    return false;
  }
  
  // Number of elements
  CFIndex numElements = CFArrayGetCount( myElements );
  if( numElements == 0 )
  {
    ERR_PRINTF("Selected device has no elements. Weird.\n");
    CFRelease( myElements );
    myElements = NULL;
    return false;
  }

#ifdef DEBUG
  vector<string> devnames = QueryDeviceNames();
  DumpJoystick dj( devnames[ joyid ].c_str() );
#endif
  
  // Loop through elements
  // NOTE: IOHIDElementGetName seems to always return NULL (for the gamepads I've tried)
  // NOTE: IOHIDDeviceGetValue must be called to retrieve the new values from the joystick.
  for( size_t ii=0; ii<(size_t)numElements; ii++ )
  {
    IOHIDElementRef element = (IOHIDElementRef) CFArrayGetValueAtIndex( myElements, ii );
    IOHIDElementType type = IOHIDElementGetType(element);
    uint32_t usage = IOHIDElementGetUsage( element );

#ifdef DEBUG
  dj.DumpElement( myDevice, element, ii );
#endif
    
    switch( type )
    {
      case kIOHIDElementTypeInput_Misc:
      case kIOHIDElementTypeInput_Axis:
        if( IOHIDElementGetReportCount( element ) < 2 )
        {
          if( usage == kHIDUsage_GD_Hatswitch )
          {
            DBG_PRINTF("HatSwitch at %i\n",(int)ii);
            myPOV.push_back( POV( myDevice, element ) );
            continue;
          }
#ifdef DEBUG
          if( type == kIOHIDElementTypeInput_Misc ) DBG_PRINTF("Misc at %i: usage 0x%X\n",(int)ii,usage);
          else DBG_PRINTF("Axis at %i: usage 0x%X\n",(int)ii,usage);
#endif
          myAxes.push_back( Axes( myDevice, element ) );
        }
        break;
        
      case kIOHIDElementTypeInput_Button:
        DBG_PRINTF("Button at %i\n", (int)ii );
        myButtons.push_back( Button( myDevice, element ) );
        break;
        
      case kIOHIDElementTypeOutput:
        break;
      default: break;
    }
  }

#ifdef DEBUG
  dj.Close();
#endif

  return true;
}
  
/**
 * \brief Query joystick for IO capabilities
 *
 * \return An vector containing the number of axes, buttons, pov, outputs. If the joystick
 *         has not been initialised yet, the result will be all -1.
 */
vector<int> Joystick::QueryIO( void )
{
  vector<int> result(4,-1);
  if( myElements != NULL )
  {
    result[0] = myAxes.size();
    result[1] = myButtons.size();
    result[2] = myPOV.size();
    result[3] = 0;
  }
  return result;
}
   
/**
 * \brief Poll the joystick axes
 *
 * \output vector of normalised axes doubles.
 */
vector<double> Joystick::PollAxes( void )
{
  vector<double> axes( myAxes.size(), 0.0 );
  for( size_t ii=0; ii<myAxes.size(); ii++ )
  {
    axes[ ii ] = myAxes[ ii ].ReadState();
  }
  return axes;
}

/**
 * \brief Poll the joystick buttons
 *
 * \output vector of button boolean values.
 */
vector<bool> Joystick::PollButtons( void )
{
  vector<bool> buttons( myButtons.size(), FALSE );
  for( size_t ii=0; ii<myButtons.size(); ii++ )
  {
    buttons[ ii ] = myButtons[ ii ].ReadState();
  }
  return buttons;
}
    
/**
 * \brief Poll the joystick POV hats
 *
 * \output vector of POV doubles. The value corresponds to the angle (degrees) and
 *         -1 corresponds to nothing pressed.
 */
vector<double> Joystick::PollPOV( void )
{
  vector<double> POVs( myPOV.size(), 65536.0 );
  for( size_t ii=0; ii<myPOV.size(); ii++ )
  {
    POVs[ ii ] = myPOV[ ii ].ReadState();
  }
  return POVs;
}

/**
 * \brief Push values to the joystick inputs (such as force feedback)
 */
void Joystick::PushInputs( vector<double> normInputs )
{
  UNUSED( normInputs );
#ifdef ERROR_OUT
  static bool onceOff = true;
  if( onceOff )
  {
    onceOff = false;
    ERR_PRINTF("Joystick::PushInputs has not been implemented.");
  }
#endif
}

/**
 * \brief Query for the available device names
 *
 * \output vector of device name strings. If no devices are available, the vector is
 *         empty.
 */
vector<string> Joystick::QueryDeviceNames( void )
{
  vector<string> result;
  
  // Open the IO HID manager (sub-function tests whether it is already open already)
  if( !InitialiseJoyManager() )
  {
    ERR_PRINTF("Joystick::QueryDeviceNames - Failed to initialise the IO HID manager.");
    return result;
  }
  
  // Open the device references
  CFSetRef deviceRefs = IOHIDManagerCopyDevices(myManager);
  
  // If the device references are empty (NULL), it means there are no devices
  if( deviceRefs == NULL ) return result;
  
  // Number of available devices
  CFIndex numDevices = CFSetGetCount( deviceRefs );
  
  // Move the device references into a vector
  vector<const void *> devices (numDevices, 0);
  CFSetGetValues( deviceRefs, &devices.front() );
  CFRelease( deviceRefs );
  
  // Loop through the devices, and add them to the output vector
  for( CFIndex ii=0; ii<numDevices; ii++ )
  {
    // Extract the Product Key string
    CFTypeRef devProdRef = IOHIDDeviceGetProperty( (IOHIDDeviceRef)devices[ii],
                                                                CFSTR(kIOHIDProductKey) );
    if( devProdRef == NULL )
    {
      ERR_PRINTF("Device returned a NULL product key.");
    }
    else if( CFGetTypeID(devProdRef) == CFStringGetTypeID() )
    {
      char buffer[256];
      CFStringGetCString( (CFStringRef)devProdRef, buffer,  sizeof(buffer),
                                                                  kCFStringEncodingUTF8 );
      string devStr(buffer);
      result.push_back( devStr );
    }
    else
    {
      ERR_PRINTF("Device returned a product key that wasn't a string.");
    }
  }
  return result;
}
  
/**
 * \brief Query for the number of available devices
 *
 * \output number of available devices
 */
unsigned int Joystick::QueryNumberDevices( void )
{
  // Open the IO HID manager (sub-function tests whether it is already open already)
  if( !InitialiseJoyManager() )
  {
    ERR_PRINTF("Joystick::QueryDeviceNames - Failed to initialise the IO HID manager.");
    return 0;
  }
  
  // Copy the devices
  CFSetRef deviceRefs = IOHIDManagerCopyDevices(myManager);
  
  // If none (NULL), it means there are no devices
  if( deviceRefs == NULL ) return 0;
  
  // Copy the number of devices and release the set
  unsigned int devCount = CFSetGetCount( deviceRefs );
  CFRelease( deviceRefs );
  
  // Otherwise return the number of devices
  return devCount;
  
}

/**
 * \brief Initialise the IOHID manager
 *
 * \output true if successful, false if unsuccessful.
 */
bool Joystick::InitialiseJoyManager( void )
{
  // If the manager is already open
  if( myManager != NULL ) return true;

  // Create IO HID manager
  myManager = IOHIDManagerCreate( kCFAllocatorDefault, kIOHIDManagerOptionNone );
  if( myManager == NULL ){
    ERR_PRINTF("Failed to create the IO HID Manager.\n");
    return false;
  }
  
  // Open IO HID manager
  IOReturn openResult =  IOHIDManagerOpen( myManager, kIOHIDManagerOptionNone );
  
  // Check the result of opening the IO HID manager
  if( openResult != kIOReturnSuccess )
  {
    ERR_PRINTF("Failed to open the IO HID Manager.\n");
    CFRelease( myManager );
    myManager = NULL;
    return false;
  }

  // Magic numbers telling the system to access joysticks (and associated)
  uint32_t page = kHIDPage_GenericDesktop;
  const uint32_t usages[] = { kHIDUsage_GD_Joystick,
                              kHIDUsage_GD_GamePad,
                              kHIDUsage_GD_MultiAxisController };
  
  CFMutableArrayRef myMatching = CFArrayCreateMutable( kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks );
  if( myMatching == NULL )
  {
    ERR_PRINTF("Failed to create CFMutableArrayRef.\n");
    return false;
  }
  
  // Loop through and add the usages.
  for( size_t ii=0; ii<(sizeof(usages)/sizeof(uint32_t)); ii++ )
  {
    CFMutableDictionaryRef matchingDict = CFDictionaryCreateMutable( kCFAllocatorDefault,
                    1, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks );
    if( matchingDict == NULL )
    {
      ERR_PRINTF("Failed to create CFMutableDictionaryRef.\n");
      return false;
    }
    
    // Add the page reference
    CFNumberRef pageRef = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &page);
    if( pageRef == NULL )
    {
      ERR_PRINTF("Failed to create page CFNumberCreate.\n");
      return false;
    }
    CFDictionarySetValue( matchingDict, CFSTR(kIOHIDDeviceUsagePageKey), pageRef );
    CFRelease( pageRef );
    
    // Add the usage reference
    CFNumberRef usageRef = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &usages[ii] );
    if( usageRef == NULL )
    {
      ERR_PRINTF("Failed to create the usage CFNumberRef.\n");
      return false;
    }
    CFDictionarySetValue( matchingDict, CFSTR(kIOHIDDeviceUsageKey), usageRef );
    CFRelease( usageRef );
    
    // Append the page-usage pairs to the CFArray
    CFArrayAppendValue( myMatching, matchingDict );
    CFRelease( matchingDict );
  }

  IOHIDManagerSetDeviceMatchingMultiple( myManager, myMatching );
  CFRelease( myMatching );

  return true;
}