#include "mex.h"
#include "matrix.h"
#include "osx_joystick.hpp"
#include <vector>

#define IS_PARAM_INT32(pVal) ( mxIsNumeric(pVal) && !mxIsLogical(pVal) &&\
    !mxIsEmpty(pVal) && !mxIsSparse(pVal) && !mxIsComplex(pVal) &&\
    mxIsClass(pVal,"int32") && (mxGetNumberOfElements(pVal)==1) )

/**
 * \brief mex gateway function.
 * \param[in] nlhs Number of left-hand side arguments
 * \param[out] plhs Pointers to left-hand side data
 * \param[in] nrhs Number of right-hand side arguments
 * \param[in] prhs Pointers to right-hand side data
 */
void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )
{
  // Sanity check the inputs.
  if( nrhs != 1 ) mexErrMsgIdAndTxt( 
      "osx_joystick_get_capabilities:IncorrectNumberOfParameters",
      "Exactly 1 parameter required.\n" );
  if( !IS_PARAM_INT32(prhs[0]) ) mexErrMsgIdAndTxt(
      "osx_joystick_get_capabilities:LocKeyNotInt32",
      "Joystick LocationKey must be an int32.\n");
  
  // Make sure there are enough outputs
  if( nlhs !=4 ) mexErrMsgIdAndTxt(
      "osx_joystick_get_capabilities:IncorrectNumberOfOutputs",
      "Exactly 4 outputs required.\n");
  
  // Initialise the joystick
  int32_T JoyLoc = mxGetScalar( prhs[0] );
  Joystick myJoy;
  if( !myJoy.Initialise( JoyLoc ) ) mexErrMsgIdAndTxt(
      "osx_joystick_get_capabilities:JoystickNotFound",
      "Selected joystick not found.\n");
      
  // Get the IO capabilities of the Joystick
  vector<int> IO = myJoy.QueryIO();
  for( size_t ii=0; ii<4; ii++ )
  {
    plhs[ii] = mxCreateDoubleScalar( double(IO[ii]) );
  }
}