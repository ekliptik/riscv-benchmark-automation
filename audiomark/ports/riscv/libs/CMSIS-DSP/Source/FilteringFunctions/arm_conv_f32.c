/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_conv_f32.c
 * Description:  Convolution of floating-point sequences
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

#include "dsp/filtering_functions.h"

/**
  @ingroup groupFilters
 */

/**
  @defgroup Conv Convolution

  Convolution is a mathematical operation that operates on two finite length vectors to generate a finite length output vector.
  Convolution is similar to correlation and is frequently used in filtering and data analysis.
  The CMSIS DSP library contains functions for convolving Q7, Q15, Q31, and floating-point data types.
  The library also provides fast versions of the Q15 and Q31 functions.

 @par            Algorithm
                   Let <code>a[n]</code> and <code>b[n]</code> be sequences of length <code>srcALen</code> and
                   <code>srcBLen</code> samples respectively. Then the convolution
                   \f[
                      c[n] = a[n] * b[n]
                   \f]
  @par
                   is defined as
                   \f[
                   c[n] = \sum_{k=0}^{srcALen} a[k] b[n-k]
                   \f]
  @par
                   Note that <code>c[n]</code> is of length <code>srcALen + srcBLen - 1</code> and is defined over the interval <code>n=0, 1, 2, ..., srcALen + srcBLen - 2</code>.
                   <code>pSrcA</code> points to the first input vector of length <code>srcALen</code> and
                   <code>pSrcB</code> points to the second input vector of length <code>srcBLen</code>.
                   The output result is written to <code>pDst</code> and the calling function must allocate <code>srcALen+srcBLen-1</code> words for the result.
  @par
                   Conceptually, when two signals <code>a[n]</code> and <code>b[n]</code> are convolved,
                   the signal <code>b[n]</code> slides over <code>a[n]</code>.
                   For each offset \c n, the overlapping portions of a[n] and b[n] are multiplied and summed together.
  @par
                   Note that convolution is a commutative operation:
                   \f[
                      a[n] * b[n] = b[n] * a[n].
                   \f]
  @par
                   This means that switching the A and B arguments to the convolution functions has no effect.

  @par           Fixed-Point Behavior
                   Convolution requires summing up a large number of intermediate products.
                   As such, the Q7, Q15, and Q31 functions run a risk of overflow and saturation.
                   Refer to the function specific documentation below for further details of the particular algorithm used.

  @par           Fast Versions
                   Fast versions are supported for Q31 and Q15. Cycles for Fast versions are less compared to Q31 and Q15 of conv and the design requires
                   the input signals should be scaled down to avoid intermediate overflows.

  @par           Opt Versions
                   Opt versions are supported for Q15 and Q7. Design uses internal scratch buffer for getting good optimisation.
                   These versions are optimised in cycles and consumes more memory (Scratch memory) compared to Q15 and Q7 versions
  
  @par           Long versions:
                   For convolution of long vectors, those functions are
                   no more adapted and will be very slow.
                   An implementation based upon FFTs should be used.

 */

/**
  @addtogroup Conv
  @{
 */

/**
  @brief         Convolution of floating-point sequences.
  @param[in]     pSrcA      points to the first input sequence
  @param[in]     srcALen    length of the first input sequence
  @param[in]     pSrcB      points to the second input sequence
  @param[in]     srcBLen    length of the second input sequence
  @param[out]    pDst       points to the location where the output result is written.  Length srcALen+srcBLen-1.
  @return        none
 */
void arm_conv_f32(
  const float32_t * pSrcA,
        uint32_t srcALen,
  const float32_t * pSrcB,
        uint32_t srcBLen,
        float32_t * pDst)
{

/* alternate version for CM0_FAMILY */

  const float32_t *pIn1 = pSrcA;                       /* InputA pointer */
  const float32_t *pIn2 = pSrcB;                       /* InputB pointer */
        float32_t sum;                                 /* Accumulator */
        uint32_t i, j;                                 /* Loop counters */

  /* Loop to calculate convolution for output length number of times */
  for (i = 0U; i < (srcALen + srcBLen - 1U); i++)
  {
    /* Initialize sum with zero to carry out MAC operations */
    sum = 0.0f;

    /* Loop to perform MAC operations according to convolution equation */
    for (j = 0U; j <= i; j++)
    {
      /* Check the array limitations */
      if (((i - j) < srcBLen) && (j < srcALen))
      {
        /* z[i] += x[i-j] * y[j] */
        sum += ( pIn1[j] * pIn2[i - j]);
      }
    }

    /* Store the output in the destination buffer */
    pDst[i] = sum;
  }


}

/**
  @} end of Conv group
 */
