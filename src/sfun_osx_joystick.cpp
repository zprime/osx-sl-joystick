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

#define S_FUNCTION_LEVEL 2
#define S_FUNCTION_NAME  sfun_osx_joystick

#include "simstruc.h"
#include "osx_joystick.hpp"

// Parameter indicies
#define NUM_PARAMS 6
#define P_JOYID 0
#define P_TS 1
#define P_LA 2
#define P_LB 3
#define P_LP 4
#define P_LO 5

#define UNUSED(x) (void)(x)

#define IS_PARAM_DOUBLE(pVal) ( mxIsNumeric(pVal) && !mxIsLogical(pVal) &&\
    !mxIsEmpty(pVal) && !mxIsSparse(pVal) && !mxIsComplex(pVal) && mxIsDouble(pVal) &&\
     (mxGetNumberOfElements(pVal)==1) )
     
#define IS_PARAM_INT32(pVal) ( mxIsNumeric(pVal) && !mxIsLogical(pVal) &&\
    !mxIsEmpty(pVal) && !mxIsSparse(pVal) && !mxIsComplex(pVal) &&\
    mxIsClass(pVal,"int32") && (mxGetNumberOfElements(pVal)==1) )

/*==================== S-function methods ====================*/

#define MDL_CHECK_PARAMETERS
#if defined(MDL_CHECK_PARAMETERS) && defined(MATLAB_MEX_FILE)

/**
 * \brief Check the s-function parameters for a real joystick
 * \param[in] S Simulink structure.
 */
void mdlCheckParameters_REALJoy( SimStruct *S )
{
  int32_T locKey = int32_T( mxGetScalar( ssGetSFcnParam( S, P_JOYID ) ) );
  Joystick myJoy;
  if( !myJoy.Initialise( locKey ) )
  {
    ssSetErrorStatus( S, "sfun-osx-joystick::mdlCheckParameters Selected Joystick is not available.");
    return;
  }
}

/**
 * \brief Check the s-function parameters for a dummy (NULL) joystick
 * \param[in] S Simulink structure.
 */
void mdlCheckParameters_NULLJoy( SimStruct *S )
{
  if( !IS_PARAM_DOUBLE( ssGetSFcnParam( S, P_LA ) ) )
  {
    ssSetErrorStatus( S, "sfun_osx_joystick::mdlCheckParameters lA (any axes?) must be a scalar double.");
    return;
  }
  if( !IS_PARAM_DOUBLE( ssGetSFcnParam( S, P_LB ) ) )
  {
    ssSetErrorStatus( S, "sfun_osx_joystick::mdlCheckParameters lB (any buttons?) must be a scalar double.");
    return;
  }
  if( !IS_PARAM_DOUBLE( ssGetSFcnParam( S, P_LP ) ) )
  {
    ssSetErrorStatus( S, "sfun_osx_joystick::mdlCheckParameters lP (any POVs?) must be a scalar double.");
    return;
  }
}

/**
 * \brief Check s-function parameters
 * \param[in] S Simulink structure.
 */
static void mdlCheckParameters( SimStruct *S )
{
  // Make sure we have the correct number of parameters
  int_T numParams = ssGetSFcnParamsCount(S);
  if( numParams != NUM_PARAMS )
  {
    static char msg[256];
    sprintf( msg, "sfun-osx-joystick::mdlCheckParameters Exactly %i parameters required.", NUM_PARAMS );
    ssSetErrorStatus( S, msg );
    return;
  }
  
  // Make sure that the Joystick LocationKey is an int32
  if ( !IS_PARAM_INT32( ssGetSFcnParam( S, P_JOYID ) ) )
  {
    ssSetErrorStatus(S, "sfun-osx-joystick::mdlCheckParameters Joystick ID must be an int32.");
    return;
  }
  
  // Make sure the appropriate device is available.
  int32_t locKey = int32_t( mxGetScalar( ssGetSFcnParam( S, P_JOYID ) ) );
  if( locKey == 0 ) mdlCheckParameters_NULLJoy( S );
  else mdlCheckParameters_REALJoy( S );
  
  // Make sure the sampling time is a scalar double.
  if ( !IS_PARAM_DOUBLE( ssGetSFcnParam( S, P_TS ) ) )
  {
    ssSetErrorStatus( S, "sfun-osx-joystick::mdlCheckParameters Sample time must be a double scalar." );
    return;
  }
  // Make sure the sampling time is positive
  real_T Ts = mxGetScalar( ssGetSFcnParam( S, P_TS ) );
  if( (Ts < 0.0) & (Ts != -1.0)  )
  {
    ssSetErrorStatus( S, "sfun-osx-joystick::mdlCheckParameters Sample time must be positive, or -1 for inherited." );
    return;
  }
  // Check the enable Outputs parameter
  if( !IS_PARAM_DOUBLE( ssGetSFcnParam( S, P_LO ) ) )
  {
    ssSetErrorStatus( S, "sfun_osx_joystick::mdlCheckParameters lO (any outputs?) must be a scalar double.");
    return;
  }
}
#endif

/**
 * \brief Provide size information to Simulink about the real Joystick.
 * \param[in,out] S Simulink structure.
 */
void mdlInitializeSizes_REALJoy( SimStruct *S )
{
  // Initialise the joystick, so we can retrieve its IO capabilities
  Joystick myJoy;
  int32_t locKey = int32_t( mxGetScalar( ssGetSFcnParam( S, P_JOYID ) ) );
  if( !myJoy.Initialise( locKey ) )
  {
    ssSetErrorStatus( S, "sfun-osx-joystick::mdlInitializeSizes Selected Joystick does not exist or couldn't be initialised." );
    return;
  }
  // Retrieve IO capability information from the joystick.
  vector<int> JoyIO = myJoy.QueryIO();
  
  // Set the number of input ports to the number of Outputs to the Joystick
  bool result;
  if( JoyIO[ kJoystick_Outputs ] > 0 )
  {
    int lO = int( mxGetScalar( ssGetSFcnParam( S, P_LO ) ) );
    if( lO )
    {
      result = ssSetNumInputPorts( S, 1 );
      ssSetInputPortWidth( S, 0, JoyIO[ kJoystick_Outputs ] );
      ssSetInputPortDataType( S, 0, SS_DOUBLE );
      ssSetInputPortDirectFeedThrough( S, 0, 1 );
    }
    else result = ssSetNumInputPorts( S, 0 );
  }
  else result = ssSetNumInputPorts( S, 0 );
  if( !result )
  {
    ssSetErrorStatus( S, "Unable to set the number of input ports." );
    return;
  }
  
  // Count the number of output types
  int numOutputs = 0;
  if( JoyIO[ kJoystick_Axes ] > 0 ) numOutputs++;
  if( JoyIO[ kJoystick_Buttons ] > 0 ) numOutputs++;
  if( JoyIO[ kJoystick_POVs ] > 0 ) numOutputs++;
  
  // Set the number of output ports
  if( !ssSetNumOutputPorts( S, numOutputs ) )
  {
    static char msg[256];
    if( numOutputs > 65535 ) numOutputs = -1;
    sprintf( msg, "Unable to set %i output ports.", numOutputs );
    ssSetErrorStatus( S, msg );
  }
  
  // Now set the output port widths and data types
  int jj = 0;
  if( JoyIO[ kJoystick_Axes ] > 0 )
  {
    ssSetOutputPortWidth( S, jj, JoyIO[ kJoystick_Axes ] );
    ssSetOutputPortDataType( S, jj, SS_DOUBLE );
    jj++;
  }
  if( JoyIO[ kJoystick_Buttons ] > 0 )
  {
    ssSetOutputPortWidth( S, jj, JoyIO[ kJoystick_Buttons ] );
    ssSetOutputPortDataType( S, jj, SS_BOOLEAN );
    jj++;
  }
  if( JoyIO[ kJoystick_POVs ] > 0 )
  {
    ssSetOutputPortWidth( S, jj, JoyIO[ kJoystick_POVs ] );
    ssSetOutputPortDataType( S, jj, SS_DOUBLE );
    jj++;
  }
}

/**
 * \brief Provide size information to Simulink about the dummy (NULL) Joystick.
 * \param[in,out] S Simulink structure.
 */
void mdlInitializeSizes_NULLJoy( SimStruct *S )
{
  // Get 'logical' values as to whether to enable inputs and outputs
  int lA, lB, lP, lO;
  lA = int( mxGetScalar( ssGetSFcnParam( S, P_LA ) ) );
  lB = int( mxGetScalar( ssGetSFcnParam( S, P_LB ) ) );
  lP = int( mxGetScalar( ssGetSFcnParam( S, P_LP ) ) );
  lO = int( mxGetScalar( ssGetSFcnParam( S, P_LO ) ) );
  
  // Set the number of input ports and the input data size
  bool result;
  if( lO )
  {
    result = ssSetNumInputPorts( S, 1 );
    ssSetInputPortWidth( S, 0, DYNAMICALLY_SIZED );
    ssSetInputPortDataType( S, 0, SS_DOUBLE );
  }
  else result = ssSetNumInputPorts( S, 0 );
  if( !result )
  {
    ssSetErrorStatus( S, "sfun-osx-joystick::mdlInitializeSizes_NULLJoy Failed to set the number of input ports.\n" );
    return;
  }
  
  // Set the number of output ports
  int numOutputs = 0;
  if( lA ) numOutputs++;
  if( lB ) numOutputs++;
  if( lP ) numOutputs++;
  
  result = ssSetNumOutputPorts( S, numOutputs );
  if( !result )
  {
    ssSetErrorStatus( S, "sfun-osx-joystick::mdlInitializeSizes Failed to set the number of output ports.\n");
    return;
  }
  
  // Set the output port sizes and data types
  int output = 0;
  if( lA )
  {
    ssSetOutputPortWidth( S, output, DYNAMICALLY_SIZED );
    ssSetOutputPortDataType( S, output, SS_DOUBLE );
    output++;
  }
  if( lB )
  {
    ssSetOutputPortWidth( S, output, DYNAMICALLY_SIZED );
    ssSetOutputPortDataType( S, output, SS_BOOLEAN );
    output++;
  }
  if( lP )
  {
    ssSetOutputPortWidth( S, output, DYNAMICALLY_SIZED );
    ssSetOutputPortDataType( S, output, SS_DOUBLE );
    output++;
  }
}

#define MDL_SET_INPUT_PORT_WIDTH
/**
 * \brief Dynamically set the input port width.
 */
static void mdlSetInputPortWidth( SimStruct *S, int_T port, int_T inputPortWidth )
{
  ssSetInputPortWidth( S, port, inputPortWidth );
}

#define MDL_SET_OUTPUT_PORT_WIDTH
/**
 * \brief Dynamically set the output port width.
 */
static void mdlSetOutputPortWidth( SimStruct *S, int_T port, int_T outputPortWidth )
{
  ssSetOutputPortWidth(S,port,outputPortWidth);
}

# define MDL_SET_DEFAULT_PORT_DIMENSION_INFO
/**
 * \brief Set the default port sizes to be 1.
 */
static void mdlSetDefaultPortDimensionInfo( SimStruct *S )
{
  // Get 'logical' values as to whether to enable inputs and outputs
  int output = 0;
  if( mxGetScalar( ssGetSFcnParam( S, P_LA ) ) > 0 )
  {
    if( ssGetOutputPortWidth( S, output ) == DYNAMICALLY_SIZED ) ssSetOutputPortWidth( S, output, 1 );
    output++;
  }
  if( mxGetScalar( ssGetSFcnParam( S, P_LB ) ) > 0 )
  {
    if( ssGetOutputPortWidth( S, output ) == DYNAMICALLY_SIZED ) ssSetOutputPortWidth( S, output, 1 );
    output++;
  }
  if( mxGetScalar( ssGetSFcnParam( S, P_LP ) ) > 0 )
  {
    if( ssGetOutputPortWidth( S, output ) == DYNAMICALLY_SIZED ) ssSetOutputPortWidth( S, output, 1 );
    output++;
  }
  if( mxGetScalar( ssGetSFcnParam( S, P_LO ) ) > 0 )
  {
    if( ssGetInputPortWidth( S, 0 ) == DYNAMICALLY_SIZED ) ssSetInputPortWidth( S, 0, 1 );
  }
}

/**
 * \brief Provide size information to Simulink.
 *
 * \param[in,out] S Simulink structure.
 */
static void mdlInitializeSizes(SimStruct *S)
{
  // Number of expected parameters, and check them.
  ssSetNumSFcnParams( S, NUM_PARAMS );
  mdlCheckParameters(S);
  if (ssGetErrorStatus(S) != NULL) return;
  
  // No tunable parameters.
  ssSetSFcnParamTunable( S, P_JOYID, SS_PRM_NOT_TUNABLE );
  ssSetSFcnParamTunable( S, P_TS, SS_PRM_NOT_TUNABLE );

  // No internal dynamic states
  ssSetNumContStates(S, 0);
  ssSetNumDiscStates(S, 0);
  
  // Number of sample times (1)
  ssSetNumSampleTimes(S, 1);
  // No Real work vector
  ssSetNumRWork(S, 0);
  // No integer work vector
  ssSetNumIWork(S, 0);
  // 2 pointers in the work vector (to store the Joystick object and Joystick IO)
  ssSetNumPWork(S, 2);
  // No Modes
  ssSetNumModes(S, 0);
  // No zero crossings
  ssSetNumNonsampledZCs(S, 0);
  // Allow automatic state saving and restoring
  ssSetSimStateCompliance( S, USE_DEFAULT_SIM_STATE );
  // No options set.
  ssSetOptions(S, 0);

  int32_t locKey = int32_t( mxGetScalar( ssGetSFcnParam( S, P_JOYID ) ) );
  if( locKey == 0 ) mdlInitializeSizes_NULLJoy( S );
  else mdlInitializeSizes_REALJoy( S );
}



/**
 * \brief Tell simulink about the block sample times.
 *
 * \param[in,out] S Simulink structure.
 */
static void mdlInitializeSampleTimes(SimStruct *S)
{
  ssSetSampleTime( S, 0, mxGetScalar( ssGetSFcnParam( S, P_TS ) ) );
  ssSetOffsetTime( S, 0, 0.0 );
  // Allow the block to inherit sample times
  ssSetModelReferenceSampleTimeDefaultInheritance(S);
}

/**
 * \brief Start the dummy (NULL) joystick block.
 */
void mdlStart_NULLJoy( SimStruct *S )
{
  ssGetPWork(S)[0] = NULL;
  ssGetPWork(S)[1] = NULL;
}

/**
 * \brief Start the real joystick block
 */
void mdlStart_REALJoy( SimStruct *S )
{
  // Create a new Joystick object, and get the pointer to it.
  Joystick *myJoy = new Joystick;
  // Get the device ID and open it.
  int devId = mxGetScalar( ssGetSFcnParam( S, P_JOYID ) );
  if( !myJoy->Initialise( devId ) )
  {
    if( devId > 65535 ) devId = -1;
    static char msg[256];
    sprintf( msg, "sfun-osx-joystick::mdlStart Device %i does not exist.\n", devId );
    ssSetErrorStatus( S, msg );
    delete myJoy;
    return;
  }
  vector<int> *JoyIO = new vector<int>( myJoy->QueryIO() );
//  static vector<int> JoyIO = myJoy->QueryIO();
  // Double check that the device hasn't changed between the call to mdlInitializeSizes
  // and mdlStart
  bool error = false;
  if( (*JoyIO)[ kJoystick_Outputs ] > 0 )
  {
    int lO = int( mxGetScalar( ssGetSFcnParam( S, P_LO ) ) );
    if( lO )
    {
      if( ssGetNumInputPorts(S) != 1 )
      {
        ssSetErrorStatus( S, "sfun-osx-joystick::mdlStart Input number of ports size error." );
        delete myJoy;
        delete JoyIO;
        return;
      }
      if( ssGetInputPortWidth( S, 0 ) != (*JoyIO)[ kJoystick_Outputs ] )
      {
        static char msg[256];
        sprintf(msg,"sfun-osx-joystick::mdlStart Input port width error. Port is set to %i, but joystick has %i.", ssGetInputPortWidth( S, 0 ), (*JoyIO)[ kJoystick_Outputs] );
        ssSetErrorStatus( S, msg );
        delete myJoy;
        delete JoyIO;
        return;
      }
    }
    else (*JoyIO)[ kJoystick_Outputs ] = 0;
  }
  int jj=0;
  if( (*JoyIO)[ kJoystick_Axes ] > 0 )
  {
    if( ssGetOutputPortWidth( S, jj ) != (*JoyIO)[ kJoystick_Axes ] ) error = true;
    jj++;
  }
  if( (*JoyIO)[ kJoystick_Buttons ] > 0 )
  {
    if( ssGetOutputPortWidth( S, jj ) != (*JoyIO)[ kJoystick_Buttons ] ) error = true;
    jj++;
  }
  if( (*JoyIO)[ kJoystick_POVs ] > 0 )
  {
    if( ssGetOutputPortWidth( S, jj ) != (*JoyIO)[ kJoystick_POVs ] ) error = true;
    jj++;
  }
  if( jj != ssGetNumOutputPorts(S) ) error = true;
  
  if( error )
  {
    ssSetErrorStatus( S, "sfun-osx-joystick::mdlStart Number of block input outputs or port widths has changed between mdlInitializeSizes and mdlStart." );
    delete myJoy;
    delete JoyIO;
    return;
  }
  
  // Store Joystick object and the joystick IO capabilities
  ssGetPWork(S)[0] = (void *) myJoy;
  ssGetPWork(S)[1] = (vector<int> *) JoyIO;
}

#define MDL_START
/**
 * \brief At the start of model execution, initialise the Joystick.
 *
 * \param[in,out] S Simulink structure.
 */
static void mdlStart( SimStruct *S )
{
  int devId = mxGetScalar( ssGetSFcnParam( S, P_JOYID ) );
  if( devId==0 ) mdlStart_NULLJoy( S );
  else mdlStart_REALJoy( S );
}


/**
 * \brief Outputs (and inputs) to the dummy (NULL) block.
 */
void mdlOutputs_NULLJoy( SimStruct *S, int_T tid )
{
  UNUSED( S );
  UNUSED( tid );
}

/**
 * \brief Outputs from (and output to) the real joystick
 */
void mdlOutputs_REALJoy( SimStruct *S, int_T tid )
{
  UNUSED( tid );
  Joystick *myJoy = (Joystick *) ssGetPWork(S)[0];
  vector<int> *JoyIO = (vector<int> *) ssGetPWork(S)[1];
  
  // Exception could be thrown in the case of a read error.
  try
  {
    // Poll the Joystick axes
    int jj = 0;
    if( (*JoyIO)[ kJoystick_Axes ] > 0 )
    {
      real_T *pr = ssGetOutputPortRealSignal( S, jj );
      vector<double> axes = myJoy->PollAxes();
      if( (int)axes.size() != ssGetOutputPortWidth( S, jj ) )
      {
        ssSetErrorStatus( S, "osx-sl-joystick::mdlOutputs Axes port width badness." );
        return;
      }
      copy( axes.begin(), axes.end(), pr );
      jj++;
    }
  
    // Poll the buttons
    if( (*JoyIO)[ kJoystick_Buttons ] > 0 )
    {
      boolean_T *pb = (boolean_T *)ssGetOutputPortSignal( S, jj );
      vector<bool> buttons = myJoy->PollButtons();
      if( (int)buttons.size() != ssGetOutputPortWidth( S, jj ) )
      {
        ssSetErrorStatus( S, "osx-sl-joystick::mdlOutputs Button port width badness." );
        return;
      }
      copy( buttons.begin(), buttons.end(), pb );
      jj++;
    }
  
    // Poll the POVs
    if( (*JoyIO)[ kJoystick_POVs ] > 0 )
    {
      real_T *pr = ssGetOutputPortRealSignal( S, jj );
      vector<double> POVs = myJoy->PollPOV();
      if( (int)POVs.size() != ssGetOutputPortWidth( S, jj ) )
      {
        ssSetErrorStatus( S, "osx-sl-joystick::mdlOutputs POV port width badness.");
        return;
      }
      copy( POVs.begin(), POVs.end(), pr );
      jj++;
    }
  
    // Push the input signals to the Joystick
    if( (*JoyIO)[ kJoystick_Outputs ] > 0 )
    {
      const real_T *pr = ssGetInputPortRealSignal( S, 0 );
      vector<double> outputs( (*JoyIO)[ kJoystick_Outputs ], 0 );
      if( (int)outputs.size() != ssGetInputPortWidth( S, 0 ) )
      {
        ssSetErrorStatus( S, "osx-sl-joystick::mdlOutputs Joystick Output (block input) port width badness." );
        return;
      }
      copy( pr, pr+outputs.size(), outputs.begin() );
      myJoy->PushInputs( outputs );
    }
  }
  catch(const char *message)
  {
    ssSetErrorStatus( S, "Joystick read error. This is most likely due to a joystick being removed during simulation." );
  }
  return;
}

/**
 * \brief Read the Joystick states and output them.
 */
static void mdlOutputs(SimStruct *S, int_T tid)
{
  int JoyLocKey = mxGetScalar( ssGetSFcnParam( S, P_JOYID ) );
  if( JoyLocKey == 0 ) mdlOutputs_NULLJoy( S, tid );
  else mdlOutputs_REALJoy( S, tid );
}                                                

/**
 * \brief At model termination, delete the Joystick object in memory.
 *
 * \input S Simulink structure.
 */
static void mdlTerminate(SimStruct *S)
{
  int JoyLocKey = mxGetScalar( ssGetSFcnParam( S, P_JOYID ) );
  if( JoyLocKey != 0 )
  {
    // Retrieve the Joystick object.
    Joystick *myJoy =  (Joystick *) ssGetPWork(S)[0];
    vector<int> *JoyIO = (vector<int> *) ssGetPWork(S)[1];
    // If there are some Joystick outputs, set them to 0.
    if( (*JoyIO)[ kJoystick_Outputs ] > 0 )
    {
      vector<double> outputs( (*JoyIO)[ kJoystick_Outputs ], 0 );
      try
      {
        myJoy->PushInputs( outputs );
      }
      catch(const char* message)
      {
      }
    }
    delete myJoy;
    delete JoyIO;
  }
}

// Required s-function trailer
#ifdef  MATLAB_MEX_FILE    // Is this file being compiled as a MEX-file?
#include "simulink.c"      // MEX-file interface mechanism
#else
#include "cg_sfun.h"       // Code generation registration function
#endif

