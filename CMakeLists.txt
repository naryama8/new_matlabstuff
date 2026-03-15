cmake_minimum_required(VERSION 3.20)
project(MyMexProject C)

# 1. TELL CLION WHERE MATLAB LIVES (Change this to your actual path!)
set(MATLAB_ROOT "C:/Program Files/MATLAB/R2024a") # Use / even on Windows

# 2. ADD THE FOLDERS FOR simstruc.h
include_directories("${MATLAB_ROOT}/simulink/include")
include_directories("${MATLAB_ROOT}/extern/include")

# 3. DEFINE YOUR SOURCE FILE
# Replace 'library.c' with your actual .c filename
add_library(my_mex_file SHARED
        skml2/three_phase.c
        skml2/cross_signal.c
        skml_tugas4/pmsm.c)

# 4. CRITICAL: Tell simstruc.h this is a MEX file
target_compile_definitions(my_mex_file PRIVATE MATLAB_MEX_FILE)
