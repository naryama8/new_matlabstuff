#define S_FUNCTION_LEVEL 2
#define S_FUNCTION_NAME pmsm
#include "simstruc.h"
#include <math.h>

#define U(element) (*uPtrs[element]) /*Pointer to Input Port0*/

static void mdlInitializeSizes(SimStruct *S){
    ssSetNumContStates(S, 4);
    if (!ssSetNumInputPorts(S, 1)) return;
    ssSetInputPortWidth(S, 0, 3);
    ssSetInputPortDirectFeedThrough(S, 0, 1);
    ssSetInputPortOverWritable(S, 0, 1);
    if (!ssSetNumOutputPorts(S, 1)) return;
    ssSetOutputPortWidth(S, 0, 5);
    ssSetNumSampleTimes(S, 1);

    ssSetOptions(S, SS_OPTION_EXCEPTION_FREE_CODE); }

static void mdlInitializeSampleTimes(SimStruct *S) {
    ssSetSampleTime(S, 0, CONTINUOUS_SAMPLE_TIME);
    ssSetOffsetTime(S, 0, 0.0); }

#define MDL_INITIALIZE_CONDITIONS
static void mdlInitializeConditions(SimStruct *S) {

    real_T *X0 = ssGetContStates(S);
    int_T nStates = ssGetNumContStates(S);
    int_T i;

    /* initialize the states to 0.0 */
    for (i=0; i < nStates; i++) {X0[i] = 0.0;} }

static void mdlOutputs(SimStruct *S, int_T tid) {
    real_T *Y = ssGetOutputPortRealSignal(S,0);
    real_T *X = ssGetContStates(S);
    InputRealPtrsType uPtrs = ssGetInputPortRealSignalPtrs(S,0);

    real_T i_sd = X[0];
    real_T i_sq = X[1];
    real_T N = 3;
    real_T L_sd = 9.1e-3;
    real_T L_sq = 18.5e-3;
    real_T flux = 0.249;

    real_T Te = N * (flux + (L_sd - L_sq) * i_sd) * i_sq;

    Y[0] = X[0];
    Y[1] = X[1];
    Y[2] = X[2];
    Y[3] = X[3];
    Y[4] = Te;

}

#define MDL_DERIVATIVES
static void mdlDerivatives(SimStruct *S) {
    real_T *dX = ssGetdX(S);
    real_T *X = ssGetContStates(S);
    InputRealPtrsType uPtrs = ssGetInputPortRealSignalPtrs(S,0);

    real_T va, vb, vc;
    va = U(0);
    vb = U(1);
    vc = U(2);

    real_T i_sd = X[0];
    real_T i_sq = X[1];
    real_T omega_m = X[2];
    real_T theta_m = X[3];

    real_T v_alpha = sqrt(2.0/3.0) * (va + (-0.5) * vb + (-0.5) * vc);
    real_T v_beta = sqrt(2.0/3.0) * ((sqrt(3.0)/2.0) * vb + (-sqrt(3.0)/2.0) * vc);

    real_T N = 3;
    real_T L_sd = 9.1e-3;
    real_T L_sq = 18.5e-3;
    real_T Rs = 1.025;
    real_T flux = 0.249;
    real_T J = 0.003;
    real_T B = 0.0005;
    real_T T_L = 0;

    real_T theta_e = theta_m * N;


    real_T v_sd = cos(theta_e) * v_alpha + sin(theta_e) * v_beta;
    real_T v_sq = -sin(theta_e) * v_alpha + cos(theta_e) * v_beta;

    dX[0] = (1.0/L_sd) * (v_sd - (Rs * i_sd) + N * omega_m * L_sq * i_sq);
    dX[1] = (1.0/L_sq) * (v_sq - (Rs * i_sq) - (N * omega_m * L_sd * i_sd) - (N * omega_m * flux));

    real_T Te = N * (flux + (L_sd - L_sq) * i_sd) * i_sq;

    dX[2] = (Te -T_L - B * omega_m) / J;
    dX[3] = omega_m;

}

static void mdlTerminate(SimStruct *S)
{} /*Keep this function empty since no memory is allocated*/

#ifdef MATLAB_MEX_FILE
/* Is this file being compiled as a MEX-file? */
#include "simulink.c" /* MEX-file interface mechanism */
#else
#include "cg_sfun.h" /*Code generation registration function*/
#endif