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

#include "button.hpp"

/**
 * \brief Button constructor.
 *
 * \param[in] device Device reference.
 * \param[in] element Element reference.
 *
 * Takes an IOHIDDeviceRef and an IOHIDElementRef. The IOHIDElementRef must be of type
 * kIOHIDElementTypeInput_Button otherwise the behaviour is undefined.
 */
Button::Button( IOHIDDeviceRef device, IOHIDElementRef element )
{
  // Copy device and element to local (object) storage
  myDevice = device;
  myElement = element;
}

/**
 * \brief Button destructor.
 */
Button::~Button( void )
{
}

/**
 * \brief Reads the state of the button element.
 *
 * \return Boolean button state.
 * \exception const char* exception thrown if the value cannot be read.
 */
bool Button::ReadState( void )
{
  // Get the value
  IOHIDValueRef myVal;
  IOReturn mySuccess = IOHIDDeviceGetValue( myDevice, myElement, &myVal );
  // If successful, return the state of the button
  if( mySuccess == kIOReturnSuccess )
  {
    return (bool)IOHIDValueGetIntegerValue( myVal );
  }
  // Otherwise, throw an exception
  throw "Error reading button";
}