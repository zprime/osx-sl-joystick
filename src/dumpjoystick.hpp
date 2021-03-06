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

#ifndef __DUMPJOYSTICK_H__
#define __DUMPJOYSTICK_H__

#include <cstdio>
#include <IOKit/hid/IOHIDDevice.h>
#include <IOKit/hid/IOHIDElement.h>
#include <IOKit/hid/IOHIDValue.h>
#include <IOKit/hid/IOHIDUsageTables.h>


class DumpJoystick
{
  public:
    /**
     * \brief Constructor of the DumpJoystick object.
     *
     * \param[in] filename File name to dump the joystick info to.
     */
    DumpJoystick( const char * filename, IOHIDDeviceRef device );
    
    /**
     * \brief DumpJoystick deconstructor.
     */   
    ~DumpJoystick();
    
    /**
     * \brief Dump the characteristics of an element into the file.
     *
     * \param[in] device Device reference.
     * \param[in] element Device element.
     * \param[in] id The element index (for printing in the file).
     */
    void DumpElement( IOHIDDeviceRef myDevice, IOHIDElementRef element, size_t id );
    
    /**
     * \brief Close the DumpJoystick object (and the corresponding file).
     */
    void Close( void );
    
  private:
    FILE *fh;
    const char * ElemUsageStr( uint32_t inusage );
    const char * ElemTypeStr( IOHIDElementType intype );
};

#endif