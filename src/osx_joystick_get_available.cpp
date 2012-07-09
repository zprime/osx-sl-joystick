#include "mex.h"
#include "matrix.h"
#include "osx_joystick.hpp"
#include <vector>
#include <string>

#define UNUSED(x) (void)(x)

/* The gateway function */
void mexFunction( int nlhs, mxArray *plhs[],
                     int nrhs, const mxArray *prhs[] )
{
  // Sanity check the inputs.
  if( nrhs != 0 ) mexWarnMsgIdAndTxt( "JoystickGetAvailable:NonEmptyInput",
                             "Ignoring input to JoystickGetAvailable.\n" );
  if( nlhs > 1 ) mexErrMsgIdAndTxt( "JoystickGetAvailable:TooManyOutputs",
                   "Too many output arguments to JoystickGetAvailable.\n");
  UNUSED( prhs );
  
  // Open the joystick, and get the names of all available devices
  Joystick myJoy;
  mwSize numJoys[2];
  numJoys[0] = (unsigned int)myJoy.QueryNumberDevices();
  numJoys[1] = 2;
  vector<JoyDev> AvailJoyDevs = myJoy.QueryAvailableDevices();
  
  // Create the cell output array
  plhs[0] = mxCreateCellArray( 2, numJoys );
  
  mwIndex cellID, subs[2];
  const mwSize keySize = 1;
  
  // Loop through the joystick names, and add them to the cell output
  for( mwSize ii=0; ii<AvailJoyDevs.size(); ii++ )
  {
    mxArray *str = mxCreateString( AvailJoyDevs[ii].productKey.c_str() );
    mxArray *id = mxCreateNumericArray( 1, &keySize, mxINT32_CLASS, mxREAL );
    int32_T *idPtr = (int32_T *)mxGetData( id );
    *idPtr = AvailJoyDevs[ii].locationKey;
    subs[0] = ii;
    subs[1] = 0;
    cellID = mxCalcSingleSubscript( plhs[0], 2, subs );
    mxSetCell( plhs[0], cellID, str );
    subs[1] = 1;
    cellID = mxCalcSingleSubscript( plhs[0], 2, subs );
    mxSetCell( plhs[0], cellID, id );
  }
}