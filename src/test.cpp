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

#include <iostream>

int main( void )
{
  Joystick myJoy;

  cout << "Number of available devices: " << myJoy.QueryNumberDevices() << '\n';
  
  vector<string> devs = myJoy.QueryDeviceNames();
  for( size_t ii=0; ii<devs.size(); ii++ )
  {
    cout << "Device " << ii << ": " << devs.at(ii) << '\n';
  }
  
  if( myJoy.Initialise( -1 ) ) cout << "Unexpected true initialisation for -1 device.\n";
  
  if( myJoy.QueryNumberDevices() == 0 )
  {
    cout << "Zero devices detected. Trying to initialise anyway (for kicks).\n";
    if( myJoy.Initialise( 0 ) ) cout << "Unexpected true initialisation with 0 devices.\n";
    if( myJoy.Initialise( 1 ) ) cout << "Unexpected true initialisation with the 1 device.\n";
  }
  else
  {
    if( myJoy.Initialise( 0 ) )
    {
      cout << "Successfully initialised device 0.\n";
      vector<int> capabilities = myJoy.QueryIO();
      cout << "Joystick reports " << capabilities[0] << " axes, " << capabilities[1] << " buttons, " << capabilities[2] << " POV, " << capabilities[3] << " inputs.\n";
      vector<bool> buttons;
      for( size_t ii=0; ii<100; ii++ )
      {
        usleep( 100000 );
        buttons = myJoy.PollButtons();
        fprintf(stdout,"\r");
        for( size_t jj=0; jj<buttons.size(); jj++ )
        {
          fprintf(stdout,"%i ", (uint8_t)buttons[jj]);
        }
        fflush( stdout );
      }
      fprintf(stdout,"\n");
    }
    else
    {
      cout << "Failed to initialise device 0.\n";
    }
  }
  
  return 0;
}