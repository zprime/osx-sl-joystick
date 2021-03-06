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

#include "outputs.hpp"

/**
 * \brief Initialise the Output object.
 *
 * \param[in] device Device reference.
 * \param[in] element Element reference. The element must be of type
 *  kIOHIDElementTypeOutput otherwise the behaviour is undefined.
 */
Outputs::Outputs( IOHIDDeviceRef device, IOHIDElementRef element )
{
  // Copy the device and element to local (object) storage
  myDevice = device;
  myElement = element;
  // Get the (logical) max and min of the element
  logmax = IOHIDElementGetLogicalMax( myElement );
  logmin = IOHIDElementGetLogicalMin( myElement );
  // Is it relative?
  isRelative = IOHIDElementIsRelative( myElement );
}
    
/**
 * \brief Outputs destructor.
 */
Outputs::~Outputs()
{
}
    
/**
 * \brief Set the value of the output.
 *
 * \param[in] val Normalised value to send to the device.
 * \exception const char* exception thrown if the value cannot be read.
 */
void Outputs::SetValue( double val )
{
  if( val > 1.0 ) val = 1.0;
  if( val < 0.0 ) val = 0.0;
  if( isRelative )
  {
    double temp = val;
    val = (val-lastVal);
    lastVal = temp;
    if( val > 1.0 ) val = 1.0;
    if( val < 0.0 ) val = 0.0;
  }
  int intVal = int( (logmax-logmin)*val + logmin );
  IOHIDValueRef hidVal = IOHIDValueCreateWithIntegerValue( kCFAllocatorDefault, myElement, 
                                                  mach_absolute_time(), intVal );
  IOReturn mySuccess = IOHIDDeviceSetValue( myDevice, myElement, hidVal );
  if( mySuccess != kIOReturnSuccess )
  {
    throw "Unable to set output value.";
  }
}