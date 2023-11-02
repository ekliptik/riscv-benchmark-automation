/*
 * SPDX-FileCopyrightText: Copyright 2010-2022 Arm Limited and/or its affiliates <open-source-office@arm.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* ----------------------------------------------------------------------
 * Project:      CMSIS NN Library
 * Title:        arm_nn_mat_mult_s8.c
 * Description:  General Matrix-multiplication function
 *
 * $Date:        26 October 2022
 * $Revision:    V.2.0.8
 *
 * Target Processor:  Cortex-M cores
 * -------------------------------------------------------------------- */

#include "arm_nnsupportfunctions.h"

/*
 * s8 General matrix multiplication function with per-channel requantization for upto 4 column batches.
 *
 * Refer header file for details.
 *
 */

int8_t *arm_nn_mat_mult_s8(const int8_t *input_row,
                           const int8_t *input_col,
                           const uint16_t output_ch,
                           const uint16_t col_batches,
                           const int32_t *output_shift,
                           const int32_t *output_mult,
                           const int32_t out_offset,
                           const int32_t col_offset,
                           const int32_t row_offset,
                           const int16_t activation_min,
                           const int16_t activation_max,
                           const uint16_t row_len,
                           const int32_t *const bias,
                           int8_t *out)
{
    (void)input_row;
    (void)input_col;
    (void)output_ch;
    (void)col_batches;
    (void)output_shift;
    (void)output_mult;
    (void)out_offset;
    (void)col_offset;
    (void)row_offset;
    (void)activation_min;
    (void)activation_max;
    (void)row_len;
    (void)bias;
    (void)out;
    return NULL;
}
