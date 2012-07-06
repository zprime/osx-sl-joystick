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

#ifndef _JOYSTICK_HPP_
#define _JOYSTICK_HPP_

#include <string>
#include <vector>
#include <IOKit/hid/IOHIDManager.h>
#include <IOKit/hid/IOHIDDevice.h>
#include "button.hpp"
#include "axes.hpp"
#include "pov.hpp"

using namespace std;

/**
 * \brief Enumerated indices for the QueryIO vector.
 */
enum JoystickIOIndex {
  kJoystick_Axes = 0,
  kJoystick_Buttons,
  kJoystick_POVs,
  kJoystick_Outputs
};

class JoyDev
{
  public:
    string productKey;
    int32_t locationKey;
};

class Joystick
{
public:
  /**
   * \brief Joystick constructor
   */
  Joystick();

  /**
   * \brief Joystick destructor
   */
  ~Joystick();

  /**
   * \brief Initialise the Joystick
   * 
   * \param[in] joyLocation LocationKey of the selected Joystick. These can be obtained
   *                        from the QueryAvailableDevices function.
   *
   * \return true if successful, false if unsuccessful (such as the joystick doesn't exist)
   */
  bool Initialise( int32_t joyLocation );
  
  /**
   * \brief Query joystick for IO capabilities
   *
   * \return An vector containing the number of axes, buttons, pov, outputs
   */
  vector<int> QueryIO( void );
   
  /**
   * \brief Poll the joystick axes
   *
   * \output vector of normalised axes doubles.
   */
  vector<double> PollAxes( void );

  /**
   * \brief Poll the joystick buttons
   *
   * \output vector of button boolean values.
   */
  vector<bool> PollButtons( void );
    
  /**
   * \brief Poll the joystick POV hats
   *
   * \output vector of POV doubles. The value corresponds to the angle (degrees) and
   *         -1 corresponds to nothing pressed.
   */
  vector<double> PollPOV( void );

  /**
   * \brief Push values to the joystick inputs (such as force feedback)
   */
  void PushInputs( vector<double> normInputs );

  /**
   * \brief Query for the available device names
   *
   * \output vector JoyDev devices (which contain Product names and location values).
   */
  vector<JoyDev> QueryAvailableDevices( );
  
  /**
   * \brief Query for the number of available devices
   *
   * \output number of available devices
   */
  unsigned int QueryNumberDevices( );
  
private:
  IOHIDManagerRef myManager;
  IOHIDDeviceRef myDevice;
  CFArrayRef myElements;
  size_t numButtons, numAxes, numInputs;
  vector<Button> myButtons;
  vector<Axes> myAxes;
  vector<POV> myPOV;
  
  /**
   * \brief Initialise the IOHID manager
   *
   * \output true if successful, false if unsuccessful.
   */
  bool InitialiseJoyManager( );
  
  /**
   * \brief Returns the LocationKey of the input device.
   *
   * \param[in] dev Device to extract the LocationKey from.
   * \return LocationKey, or 0 if there was an error.
   */
  int32_t GetLocationKey( IOHIDDeviceRef dev );

  /**
   * \brief Returns the ProductKey for the input device.
   * \param[in] dev Device to extract the ProductKey from.
   * \return A string object with the ProductKey in it. The string may be empty if the
   *         there was an error (or the device doesn't have a product key).
   */
  string GetProductKey( IOHIDDeviceRef dev );

};

#endif