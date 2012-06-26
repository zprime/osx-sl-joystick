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

#define DEBUG
#define ERROR_OUT

#ifdef ERROR_OUT
  #include <cstdio>
  #define ERR_PRINTF(...) fprintf(stderr,__VA_ARGS__)
#else
  #define ERR_PRINTF(...)
#endif

#ifdef DEBUG
  #include <iostream>
  #include <cstdio>
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
 }

/**
 * \brief Joystick destructor
 */
Joystick::~Joystick()
{
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
  
  
  
/*
  // Dictionary to find Joysticks
  CFDictionaryRef myDict =  CFDictionaryCreate( kCFAllocatorDefault, CFSTR(kIOHIDDeviceUsagePairsKey), matching,  2, 
                        &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
                        
  
  if( !myDict )
  {
#ifdef DEBUG
    cout << "Failed to open dictionary\n";
#endif
  CFRelease( myManager );
  myManager = NULL;
  return false;
  }
#ifdef DEBUG
  else
  {
    cout << "Successfully opened the dictionary\n";
  }
#endif
*/  
  // Add key for device type to matching dictionary
//  CFNumberRef myNumref = CFNumberCreate( kCFAllocatorDefault, kCFNumberIntType, &kHIDPage_GenericDesktop );
  
//  CFRelease( myDict );
//  CFRelease( myNumref );                     

  return false;
}
  
/**
 * \brief Query joystick for IO capabilities
 *
 * \input numaxes - pointer to storage to write the number of analogue axes to
 * \input numbuttons - pointer to storage to write the number of buttons to
 * \input numinputs - pointer to storage to write the number of inputs (force feedback
 *        or rumble) to
 *
 * \return true if successful, false if unsuccessful
 */
bool Joystick::QueryIO( unsigned int *numaxes, unsigned int *numbuttons, unsigned int *numinputs )
{
  return false;
}
   
/**
 * \brief Poll the joystick axes and buttons
 *
 * \input axes - pointer to storage to put the axes data into.
 * \input buttons - pointer to storage to put the button data into.
 *
 * \output true if successful, false if unsuccessful
 */
bool Joystick::Poll( unsigned char *axes, unsigned char *buttons )
{
  return false;
}
    
/**
 * \brief Push force-feedback or rumble information to the joystick
 *
 * \input inputs - pointer to a string of values to write to the joystick
 *
 * \output true if successful, false if unsuccessful
 */
bool Joystick::Input( unsigned char *inputs )
{
  return false;
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
    else ERR_PRINTF("Device returned a product key that wasn't a string.");
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
  
  // Otherwise return the number of devices
  return (unsigned int) CFSetGetCount( deviceRefs );
  
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