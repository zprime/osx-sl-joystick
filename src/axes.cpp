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

#include "axes.hpp"

Axes::Axes( IOHIDDeviceRef newDev, IOHIDElementRef newElem )
{
  device = newDev;
  element = newElem;
  logmax = IOHIDElementGetLogicalMax( element );
  logmin = IOHIDElementGetLogicalMin( element );
  length = IOHIDElementGetReportCount( element );
  if( length > 1 ) isMultiByte = true;
  else isMultiByte = false;
  if( IOHIDElementIsRelative( element ) ) isRelative = true;
  else isRelative = false;
}

Axes::~Axes(){}

double Axes::ReadState( void )
{
  IOHIDValueRef valref;
  IOReturn successful = IOHIDDeviceGetValue( device, element, &valref );
  if( successful == kIOReturnSuccess )
  {
    double value;
    if( !isMultiByte )
    {
      value = double( IOHIDValueGetIntegerValue( valref ) );
    }
    else
    {
      const uint8_t *ptr = IOHIDValueGetBytePtr( valref );
      switch( length )
      {
        case 2:
          value = double( int(ptr[1])<<8 + int(ptr[0]) );
          break;
        case 3:
          value = double( int(ptr[2])<<16 + int(ptr[1])<<8 + int(ptr[0]) );
          break;
        case 4:
          value = double( int(ptr[3])<<24 + int(ptr[2])<<16 + int(ptr[1])<<8 + int(ptr[0]) );
          break;
        default:
          value = -5;
      }
    }
    if( isRelative )
    {
      value += lastVal;
      lastVal = value;
    }
    return 2*value/(logmax-logmin) - 1;
  }
  return -5;
}