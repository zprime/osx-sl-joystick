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

/**
 * \brief Initialise the Axes object.
 *
 * \param[in] device Device reference.
 * \param[in] element Element reference. The element must be of type
 *  kIOHIDElementTypeInput_Axis or kIOHIDElementTypeInput_Misc otherwise the behaviour
 *  is undefined.
 */
Axes::Axes( IOHIDDeviceRef device, IOHIDElementRef element )
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
 * \brief Axes destructor.
 */
Axes::~Axes(){}

/**
 * \brief Read the state of the axes as a normalised double.
 *
 * \return Normalised state of the axis. -1 corresponds to LogicalMinimum and +1
 *   corresponds to LogicalMaximum.
 * \exception const char* exception thrown if the value cannot be read.
 */
double Axes::ReadState( void )
{
  // Try opening the value
  IOHIDValueRef myValue;
  IOReturn mySuccess = IOHIDDeviceGetValue( myDevice, myElement, &myValue );
  if( mySuccess == kIOReturnSuccess )
  {
    // If successful, read the value.
    double value;
    value = double( IOHIDValueGetIntegerValue( myValue ) );
    // If it is relative, accumulate the value
    if( isRelative )
    {
      value += lastVal;
      lastVal = value;
    }
    // Normalise and return the result
    return 2*value/(logmax-logmin) - 1;
  }
  throw "Error reading axes";
}