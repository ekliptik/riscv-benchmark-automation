/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_fir_lattice_f32.c
 * Description:  Processing function for floating-point FIR Lattice filter
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
  @defgroup FIR_Lattice Finite Impulse Response (FIR) Lattice Filters

  @deprecated Those functions are no more tested nor maintained and will be removed in
              a future version.
              
  This set of functions implements Finite Impulse Response (FIR) lattice filters
  for Q15, Q31 and floating-point data types.  Lattice filters are used in a
  variety of adaptive filter applications. The filter structure is feedforward and
  the net impulse response is finite length.
  The functions operate on blocks
  of input and output data and each call to the function processes
  <code>blockSize</code> samples through the filter.  <code>pSrc</code> and
  <code>pDst</code> point to input and output arrays containing <code>blockSize</code> values.

  @par           Algorithm
                   \image html FIRLattice.gif "Finite Impulse Response Lattice filter"
                   The following difference equation is implemented:
  @par
  <pre>
      f0[n] = g0[n] = x[n]
      fm[n] = fm-1[n] + km * gm-1[n-1] for m = 1, 2, ...M
      gm[n] = km * fm-1[n] + gm-1[n-1] for m = 1, 2, ...M
      y[n] = fM[n]
  </pre>
  @par
                   <code>pCoeffs</code> points to tha array of reflection coefficients of size <code>numStages</code>.
                   Reflection Coefficients are stored in the following order.
  @par
  <pre>
      {k1, k2, ..., kM}
  </pre>
                   where M is number of stages
  @par
                   <code>pState</code> points to a state array of size <code>numStages</code>.
                   The state variables (g values) hold previous inputs and are stored in the following order.
  <pre>
    {g0[n], g1[n], g2[n] ...gM-1[n]}
  </pre>
                   The state variables are updated after each block of data is processed; the coefficients are untouched.

  @par           Instance Structure
                   The coefficients and state variables for a filter are stored together in an instance data structure.
                   A separate instance structure must be defined for each filter.
                   Coefficient arrays may be shared among several instances while state variable arrays cannot be shared.
                   There are separate instance structure declarations for each of the 3 supported data types.

  @par           Initialization Functions
                   There is also an associated initialization function for each data type.
                   The initialization function performs the following operations:
                   - Sets the values of the internal structure fields.
                   - Zeros out the values in the state buffer.
                   To do this manually without calling the init function, assign the follow subfields of the instance structure:
                   numStages, pCoeffs, pState. Also set all of the values in pState to zero.
  @par
                   Use of the initialization function is optional.
                   However, if the initialization function is used, then the instance structure cannot be placed into a const data section.
                   To place an instance structure into a const data section, the instance structure must be manually initialized.
                   Set the values in the state buffer to zeros and then manually initialize the instance structure as follows:
  <pre>
      arm_fir_lattice_instance_f32 S = {numStages, pState, pCoeffs};
      arm_fir_lattice_instance_q31 S = {numStages, pState, pCoeffs};
      arm_fir_lattice_instance_q15 S = {numStages, pState, pCoeffs};
  </pre>
  @par
                   where <code>numStages</code> is the number of stages in the filter;
                   <code>pState</code> is the address of the state buffer;
                   <code>pCoeffs</code> is the address of the coefficient buffer.

  @par           Fixed-Point Behavior
                   Care must be taken when using the fixed-point versions of the FIR Lattice filter functions.
                   In particular, the overflow and saturation behavior of the accumulator used in each function must be considered.
                   Refer to the function specific documentation below for usage guidelines.
 */

/**
  @addtogroup FIR_Lattice
  @{
 */

/**
  @brief         Processing function for the floating-point FIR lattice filter.
  @param[in]     S          points to an instance of the floating-point FIR lattice structure
  @param[in]     pSrc       points to the block of input data
  @param[out]    pDst       points to the block of output data
  @param[in]     blockSize  number of samples to process
  @return        none
 */

void arm_fir_lattice_f32(
  const arm_fir_lattice_instance_f32 * S,
  const float32_t * pSrc,
        float32_t * pDst,
        uint32_t blockSize)
{
        float32_t *pState = S->pState;                 /* State pointer */
  const float32_t *pCoeffs = S->pCoeffs;               /* Coefficient pointer */
        float32_t *px;                                 /* Temporary state pointer */
  const float32_t *pk;                                 /* Temporary coefficient pointer */
        uint32_t numStages = S->numStages;             /* Number of stages in the filter */
        uint32_t blkCnt, stageCnt;                     /* Loop counters */
        float32_t fcurr0, fnext0, gnext0, gcurr0;      /* Temporary variables */


  gcurr0 = 0.0f;


  /* Initialize blkCnt with number of samples */
  blkCnt = blockSize;


  while (blkCnt > 0U)
  {
    /* f0(n) = x(n) */
    fcurr0 = *pSrc++;

    /* Initialize state pointer */
    px = pState;

    /* Initialize coeff pointer */
    pk = pCoeffs;

    /* read g2(n) from state buffer */
    gcurr0 = *px;

    /* for sample 1 processing */
    /* f1(n) = f0(n) +  K1 * g0(n-1) */
    fnext0 = (gcurr0 * (*pk)) + fcurr0;

    /* g1(n) = f0(n) * K1  +  g0(n-1) */
    gnext0 = (fcurr0 * (*pk++)) + gcurr0;

    /* save g1(n) in state buffer */
    *px++ = fcurr0;

    /* f1(n) is saved in fcurr0 for next stage processing */
    fcurr0 = fnext0;

    stageCnt = (numStages - 1U);

    /* stage loop */
    while (stageCnt > 0U)
    {
      /* read g2(n) from state buffer */
      gcurr0 = *px;

      /* save g1(n) in state buffer */
      *px++ = gnext0;

      /* Sample processing for K2, K3.... */
      /* f2(n) = f1(n) +  K2 * g1(n-1) */
      fnext0 = (gcurr0 * (*pk)) + fcurr0;

      /* g2(n) = f1(n) * K2  +  g1(n-1) */
      gnext0 = (fcurr0 * (*pk++)) + gcurr0;

      /* f1(n) is saved in fcurr0 for next stage processing */
      fcurr0 = fnext0;

      stageCnt--;
    }

    /* y(n) = fN(n) */
    *pDst++ = fcurr0;

    blkCnt--;
  }

}

/**
  @} end of FIR_Lattice group
 */
