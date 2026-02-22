#define S_FUNCTION_LEVEL 2
#define S_FUNCTION_NAME cross_signal
#include <math.h>
#include "simstruc.h"

#define U(element) (*uPtrs[element]) /*Pointer to Input Port0*/


static void mdlInitializeSizes(SimStruct *S){
    if (!ssSetNumInputPorts(S, 1)) return;
    ssSetInputPortWidth(S, 0, 3);
    ssSetInputPortDirectFeedThrough(S, 0, 1);
    ssSetInputPortOverWritable(S, 0, 1);
    if (!ssSetNumOutputPorts(S, 1)) return;
    ssSetOutputPortWidth(S, 0, 1);
    ssSetNumSampleTimes(S, 1);
    ssSetNeedAbsoluteTime(S, 1);

    ssSetOptions(S, SS_OPTION_EXCEPTION_FREE_CODE); }

static void mdlInitializeSampleTimes(SimStruct *S) {
    ssSetSampleTime(S, 0, CONTINUOUS_SAMPLE_TIME);
    ssSetOffsetTime(S, 0, 0); }

static void mdlOutputs(SimStruct *S, int_T tid) {
    real_T *Y = ssGetOutputPortRealSignal(S,0);
    InputRealPtrsType uPtrs = ssGetInputPortRealSignalPtrs(S,0);
    real_T t = ssGetT(S);
    real_T a, b, c;
    a = U(0);
    b = U(1);
    c = U(2);

    if (fabs(a - b) < 0.001 && a > 0) {
        Y[0] = a;
    } else if (fabs(b - c) < 0.001 && b > 0) {
        Y[0] = b;
    } else if (fabs(c - a) < 0.001 && c > 0) {
        Y[0] = c;
    } else {
        Y[0] = 0;
    }

}


static void mdlTerminate(SimStruct *S)
{ } /*Keep this function empty since no memory is allocated*/

#ifdef MATLAB_MEX_FILE
/* Is this file being compiled as a MEX-file? */
#include "simulink.c" /* MEX-file interface mechanism */
#else
#include "cg_sfun.h" /*Code generation registration function*/
#endif