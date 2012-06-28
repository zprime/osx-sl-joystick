#include "mex.h"
#include "matrix.h"
#include "osx_joystick.hpp"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

/* The gateway function */
void mexFunction( int nlhs, mxArray *plhs[],
                     int nrhs, const mxArray *prhs[] )
{
  // Sanity check the inputs.
  if( nrhs != 0 ) mexWarnMsgIdAndTxt( "JoystickGetAvailable:NonEmptyInput",
                             "Ignoring input to JoystickGetAvailable.\n" );
  if( nlhs > 1 ) mexErrMsgIdAndTxt( "JoystickGetAvailable:TooManyOutputs",
                   "Too many output arguments to JoystickGetAvailable.\n");
  
  // Open the joystick, and get the names of all available devices
  Joystick myJoy;
  mwSize numJoys = (unsigned int)myJoy.QueryNumberDevices();
  vector<string> JoyNames = myJoy.QueryDeviceNames();
  
  // Create the cell output array
  plhs[0] = mxCreateCellArray( 1, &numJoys );
  
  // Loop through the joystick names, and add them to the cell output
  for( mwSize ii=0; ii<numJoys; ii++ )
  {
    stringstream identifier( stringstream::in|stringstream::out );
    identifier << ii << ": " << JoyNames[ii].c_str();
    mxArray *str = mxCreateString( identifier.str().c_str() );
    mxSetCell( plhs[0], ii, str );
  }
}