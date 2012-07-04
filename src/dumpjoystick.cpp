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

#include "dumpjoystick.hpp"

/**
 * \brief Constructor of the DumpJoystick object.
 *
 * \param[in] filename File name to dump the joystick info to.
 */
DumpJoystick::DumpJoystick( const char * filename, IOHIDDeviceRef device )
{
  // Open the file and write the header.
  fh = fopen( filename, "w+" );
  if( fh==NULL ) fprintf(stderr,"Failed to open file: %s\n",filename);
  else
  {
    CFTypeRef prodRef = IOHIDDeviceGetProperty( (IOHIDDeviceRef)device, CFSTR(kIOHIDProductKey) );
    if( prodRef == NULL ) fprintf(fh,"ProductKey: NULL\n");
    else
    {
      if( CFGetTypeID(prodRef) == CFStringGetTypeID() )
      {
        char buffer[256];
        CFStringGetCString( (CFStringRef)prodRef, buffer,  sizeof(buffer), kCFStringEncodingUTF8 );
        fprintf(fh,"ProductKey: %s\n",buffer);
      }
    }
    
    CFTypeRef serialRef = IOHIDDeviceGetProperty( device, CFSTR(kIOHIDSerialNumberKey) );
    if( serialRef == NULL ) fprintf(fh,"SerialKey: NULL\n");
    else
    {
      if( CFGetTypeID(serialRef) == CFStringGetTypeID() )
      {
        char buffer[256];
        CFStringGetCString( (CFStringRef)serialRef, buffer, sizeof(buffer), kCFStringEncodingUTF8 );
        fprintf(fh,"SerialKey: %s\n",buffer);
      }
    }
  
    CFTypeRef transRef = IOHIDDeviceGetProperty( device, CFSTR(kIOHIDTransportKey) );
    if( transRef == NULL ) fprintf(fh,"TransportKey: NULL\n");
    else
    {
      if( CFGetTypeID(transRef) == CFStringGetTypeID() )
      {
        char buffer[256];
        CFStringGetCString( (CFStringRef)transRef, buffer, sizeof(buffer), kCFStringEncodingUTF8 );
        fprintf(fh,"TransportKey: %s\n",buffer);
      }
    }
  
    CFTypeRef locRef = IOHIDDeviceGetProperty( device, CFSTR(kIOHIDLocationIDKey) );
    if( locRef == NULL ) fprintf(fh,"LocationKey: NULL\n");
    else
    {
      if( CFGetTypeID(locRef) == CFNumberGetTypeID() )
      { 
        int32_t loc;
        CFNumberGetValue( (CFNumberRef)locRef, kCFNumberSInt32Type, (void *)&loc );
        fprintf(fh,"LocationKey: 0x%x\n", loc );
      }
    }
  
    fprintf(fh, "  ID           TYPE        USAGE    UID LMAX LMIN LEN IV HN HP IA  U UE NL IR IW RID RC RS\n");
  }
}

/**
 * \brief DumpJoystick deconstructor.
 */   
DumpJoystick::~DumpJoystick()
{
  Close();
}

/**
 * \brief Close the DumpJoystick object (and the corresponding file).
 */
void DumpJoystick::Close( void )
{
  if( fh != NULL )
  {
    fclose( fh );
    fh = NULL;
  }
}

/**
 * \brief Dump the characteristics of an element into the file.
 *
 * \param[in] device Device reference.
 * \param[in] element Device element.
 * \param[in] id The element index (for printing in the file).
 */
void DumpJoystick::DumpElement( IOHIDDeviceRef device, IOHIDElementRef element, size_t id )
{
  // Get the characteristics to dump
  IOHIDElementType type = IOHIDElementGetType( element );
  uint32_t usage = IOHIDElementGetUsage( element );
  const char *typeStr = ElemTypeStr( type );
  const char *usageStr = ElemUsageStr( usage );
  int logmax = IOHIDElementGetLogicalMax( element );
  int logmin = IOHIDElementGetLogicalMin( element );
  int iv = IOHIDElementIsVirtual( element );
  int hn = IOHIDElementHasNullState( element );
  int hp = IOHIDElementHasPreferredState( element );
  int ia = IOHIDElementIsArray( element );
  int eu = IOHIDElementGetUnit( element );
  int ue = IOHIDElementGetUnitExponent( element );
  int nl = IOHIDElementIsNonLinear( element );
  int ir = IOHIDElementIsRelative( element );
  int iw = IOHIDElementIsWrapping( element );
  uint32_t rid =  IOHIDElementGetReportID( element );
  uint32_t rc = IOHIDElementGetReportCount( element );
  uint32_t rs = IOHIDElementGetReportSize( element );
    
  // Try dumping the length of the element.
  int len = -1;
  IOHIDValueRef val;
  IOReturn successful;
  if( type == kIOHIDElementTypeInput_Misc || type == kIOHIDElementTypeInput_Button || type == kIOHIDElementTypeInput_Axis || type == kIOHIDElementTypeCollection )
  {
    successful = IOHIDDeviceGetValue( device, element, &val );
    if( successful == kIOReturnSuccess )
    {
      len = IOHIDValueGetLength( val );
    }
  }
  
  // Make sure the file is open
  if( fh == NULL ) fprintf( stderr, "Error: file has not yet been opened.\n");
  else
  {
    // Then dump all of the characteristics.
    fprintf( fh, "%4i %14s %12s (0x%02x) %4i %4i %3i %2i %2i %2i %2i %2i %2i %2i %2i %2i %3i %2i %2i\n", (int)id, typeStr, usageStr, usage, logmax, logmin, len, iv, hn, hp, ia, eu, ue, nl, ir, iw, rid, rc, rs );
  }
}

/**
 * \brief Returns a pointer to a string corresponding to a type (from the input).
 *
 * \param[in] intype Element type ID to lookup.
 * \return Pointer to a type string.
 */
const char * DumpJoystick::ElemTypeStr( IOHIDElementType intype )
{
  // Array of type strings (static, so calling functions can access them).
  static const char *types[] = { "Input_Misc", "Input_Button", "Input_Axis", "Input_ScanCodes",
    "Output", "Feature", "Collection", "Unknown" };
    
  // Switch statement to get the types.
  switch( intype )
  {
    case kIOHIDElementTypeInput_Misc:
      return types[0];
    case kIOHIDElementTypeInput_Button:
      return types[1];
    case kIOHIDElementTypeInput_Axis:
      return types[2];
    case kIOHIDElementTypeInput_ScanCodes:
      return types[3];
    case kIOHIDElementTypeOutput:
      return types[4];
    case kIOHIDElementTypeFeature:
      return types[5];
    case kIOHIDElementTypeCollection:
      return types[6];
    default:
      return types[7];
  }
}

/**
 * \brief Get a usage string for a given usage id.
 *
 * \param[in] inusage Usage number to lookup.
 * \return pointer to a string that corresponds to the usage id.
 */
const char * DumpJoystick::ElemUsageStr( uint32_t inusage )
{
  // Array of usage strings
  static const char *usages[] = { "X", "Y", "Z", "Rx", "Ry", "Rz", "Slider", "Dial", "Wheel",
                            "Hatswitch", "CountedBuffer", "ByteCount", "MotionWakeup",
                            "Start", "Select", "Vx", "Vy", "Vz", "Vbrx", "Vbry", "Vbrz",
                            "Vno", "DPadUp", "DPadDown", "DPadRight", "DPadLeft", "Other"};
  // Switch statement to find usages
  switch( inusage )
  {
    case kHIDUsage_GD_X:	return usages[0];
	case kHIDUsage_GD_Y:	return usages[1];
	case kHIDUsage_GD_Z:	return usages[2];
	case kHIDUsage_GD_Rx:	return usages[3];
	case kHIDUsage_GD_Ry:	return usages[4];
	case kHIDUsage_GD_Rz:	return usages[5];
	case kHIDUsage_GD_Slider:	return usages[6];
	case kHIDUsage_GD_Dial:	return usages[7];
	case kHIDUsage_GD_Wheel:	return usages[8];
	case kHIDUsage_GD_Hatswitch:	return usages[9];
	case kHIDUsage_GD_CountedBuffer:	return usages[10];
	case kHIDUsage_GD_ByteCount:	return usages[11];
	case kHIDUsage_GD_MotionWakeup:	return usages[12];
	case kHIDUsage_GD_Start:	return usages[13];
	case kHIDUsage_GD_Select:	return usages[14];
	case kHIDUsage_GD_Vx:	return usages[15];
	case kHIDUsage_GD_Vy:	return usages[16];
	case kHIDUsage_GD_Vz:	return usages[17];
	case kHIDUsage_GD_Vbrx: return usages[18];
	case kHIDUsage_GD_Vbry: return usages[19];
	case kHIDUsage_GD_Vbrz: return usages[20];
	case kHIDUsage_GD_Vno:	return usages[21];
	case kHIDUsage_GD_DPadUp:	return usages[22];
	case kHIDUsage_GD_DPadDown:	return usages[23];
	case kHIDUsage_GD_DPadRight:	return usages[24];
	case kHIDUsage_GD_DPadLeft:	return usages[25];
	default: return usages[26];
  }
}