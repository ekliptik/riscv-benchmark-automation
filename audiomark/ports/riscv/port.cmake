# Developer libraries
include_directories(${PORT_DIR})
include_directories(${PORT_DIR}/libs/CMSIS-DSP/Include)
include_directories(${PORT_DIR}/libs/CMSIS-DSP/PrivateInclude)
include_directories(${PORT_DIR}/libs/CMSIS-NN/Include)

# Linker script
link_directories(${PORT_DIR})
set(LINKER_SCRIPT link.ld)

# Switch SpeeX to SmallFFT mode, whatever that means
add_definitions(-DUSE_SMALLFT)
# Lie to get the right timing interface
add_definitions(-D__PERF_COUNTER__)
add_definitions(-DSPIKE)


set(PORT_SOURCE
    ${PORT_DIR}/th_api.c

    ${PORT_DIR}/stub.c
    ${PORT_DIR}/util.c
    ${PORT_DIR}/crt0.S
    ${PORT_DIR}/boardsupport.c

    ${PORT_DIR}/libs/CMSIS-DSP/Source/BasicMathFunctions/BasicMathFunctions.c
    ${PORT_DIR}/libs/CMSIS-DSP/Source/CommonTables/CommonTables.c
    ${PORT_DIR}/libs/CMSIS-DSP/Source/ComplexMathFunctions/ComplexMathFunctions.c
    ${PORT_DIR}/libs/CMSIS-DSP/Source/FastMathFunctions/FastMathFunctions.c
    ${PORT_DIR}/libs/CMSIS-DSP/Source/MatrixFunctions/MatrixFunctions.c	
    ${PORT_DIR}/libs/CMSIS-DSP/Source/StatisticsFunctions/StatisticsFunctions.c
    ${PORT_DIR}/libs/CMSIS-DSP/Source/SupportFunctions/arm_float_to_q15.c
    ${PORT_DIR}/libs/CMSIS-DSP/Source/SupportFunctions/arm_q15_to_float.c
    ${PORT_DIR}/libs/CMSIS-DSP/Source/SupportFunctions/arm_copy_f32.c
    ${PORT_DIR}/libs/CMSIS-DSP/Source/TransformFunctions/TransformFunctions.c

    ${PORT_DIR}/libs/CMSIS-NN/Source/PoolingFunctions/arm_avgpool_s8.c
    ${PORT_DIR}/libs/CMSIS-NN/Source/ConvolutionFunctions/arm_convolve_wrapper_s8.c
    ${PORT_DIR}/libs/CMSIS-NN/Source/ConvolutionFunctions/arm_convolve_1_x_n_s8.c
    ${PORT_DIR}/libs/CMSIS-NN/Source/ConvolutionFunctions/arm_convolve_1x1_s8.c
    ${PORT_DIR}/libs/CMSIS-NN/Source/ConvolutionFunctions/arm_convolve_1x1_s8_fast.c
    ${PORT_DIR}/libs/CMSIS-NN/Source/ConvolutionFunctions/arm_convolve_s8.c
    ${PORT_DIR}/libs/CMSIS-NN/Source/ConvolutionFunctions/arm_depthwise_conv_wrapper_s8.c
    ${PORT_DIR}/libs/CMSIS-NN/Source/ConvolutionFunctions/arm_depthwise_conv_3x3_s8.c
    ${PORT_DIR}/libs/CMSIS-NN/Source/ConvolutionFunctions/arm_depthwise_conv_s8.c
    ${PORT_DIR}/libs/CMSIS-NN/Source/ConvolutionFunctions/arm_depthwise_conv_s8_opt.c
    ${PORT_DIR}/libs/CMSIS-NN/Source/ConvolutionFunctions/arm_nn_mat_mult_kernel_s8_s16.c
    ${PORT_DIR}/libs/CMSIS-NN/Source/ConvolutionFunctions/arm_nn_mat_mult_s8.c
    ${PORT_DIR}/libs/CMSIS-NN/Source/FullyConnectedFunctions/arm_fully_connected_s8.c
    ${PORT_DIR}/libs/CMSIS-NN/Source/SoftmaxFunctions/arm_softmax_s8.c
    ${PORT_DIR}/libs/CMSIS-NN/Source/SoftmaxFunctions/arm_nn_softmax_common_s8.c
    ${PORT_DIR}/libs/CMSIS-NN/Source/NNSupportFunctions/arm_nn_mat_mult_nt_t_s8.c
    ${PORT_DIR}/libs/CMSIS-NN/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_s8.c
    ${PORT_DIR}/libs/CMSIS-NN/Source/NNSupportFunctions/arm_q7_to_q15_with_offset.c
    ${PORT_DIR}/libs/CMSIS-NN/Source/NNSupportFunctions/arm_nn_mat_mul_core_1x_s8.c
    ${PORT_DIR}/libs/CMSIS-NN/Source/NNSupportFunctions/arm_nn_mat_mul_core_4x_s8.c
    ${PORT_DIR}/libs/CMSIS-NN/Source/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_s8.c
    ${PORT_DIR}/libs/CMSIS-NN/Source/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_padded_s8.c
)