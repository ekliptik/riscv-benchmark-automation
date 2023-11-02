/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_mat_trans_q7.c
 * Description:  Q7 matrix transpose
 *
 * $Date:        23 April 2021
 * $Revision:    V1.9.0
 *
 * Target Processor: Cortex-M and Cortex-A cores
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2010-2021 ARM Limited or its affiliates. All rights reserved.
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

#include "dsp/matrix_functions.h"

/**
  @ingroup groupMatrix
 */

/**
  @addtogroup MatrixTrans
  @{
 */

/**
  @brief         Q7 matrix transpose.
  @param[in]     pSrc      points to input matrix
  @param[out]    pDst      points to output matrix
  @return        execution status
                   - \ref ARM_MATH_SUCCESS       : Operation successful
                   - \ref ARM_MATH_SIZE_MISMATCH : Matrix size check failed
 */
arm_status arm_mat_trans_q7(const arm_matrix_instance_q7 *pSrc, arm_matrix_instance_q7 *pDst)
{
    q7_t *pSrcA = pSrc->pData;         /* input data matrix pointer */
    q7_t *pOut = pDst->pData;          /* output data matrix pointer */
    uint16_t nRows = pSrc->numRows;    /* number of nRows */
    uint16_t nColumns = pSrc->numCols; /* number of nColumns */
    uint16_t col, row = nRows, i = 0U; /* row and column loop counters */
    arm_status status;                 /* status of matrix transpose */



    {
        /* Matrix transpose by exchanging the rows with columns */
        /* row loop     */
        do {
            /* The pointer pOut is set to starting address of the column being processed */
            pOut = pDst->pData + i;

            /* Initialize column loop counter */
            col = nColumns;


            while (col > 0U) {
                /* Read and store the input element in the destination */
                *pOut = *pSrcA++;

                /* Update the pointer pOut to point to the next row of the transposed matrix */
                pOut += nRows;

                /* Decrement the column loop counter */
                col--;
            }

            i++;

            /* Decrement the row loop counter */
            row--;

        } while (row > 0U);

        /* set status as ARM_MATH_SUCCESS */
        status = ARM_MATH_SUCCESS;
    }
    /* Return to application */
    return (status);
}


/**
  @} end of MatrixTrans group
 */
