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
  
  return 0;
}