/**
 * Copyright (C) 2022 EEMBC
 * Copyright (C) 2022 Arm Limited
 * Copyright (C) 2022 Infineon
 *
 * All EEMBC Benchmark Software are products of EEMBC and are provided under the
 * terms of the EEMBC Benchmark License Agreements. The EEMBC Benchmark Software
 * are proprietary intellectual properties of EEMBC and its Members and is
 * protected under all applicable laws, including all applicable copyright laws.
 *
 * If you received this EEMBC Benchmark Software without having a currently
 * effective EEMBC Benchmark License Agreement, you must discontinue use.
 */

#ifndef __EE_ABF_H
#define __EE_ABF_H

#include "ee_types.h"
#include "ee_api.h"
#include "ee_audiomark.h"

#define NFFT    128
#define NFFTD2  (NFFT / 2)
#define LAGSTEP 16
/* fixed direction beamformer from [0.. LAGSTEP-1] <=> [-180deg .. +180deg] */
#define FIXED_DIRECTION 0
#define REAL            1
#define COMPLEX         2
#define LEN_BM_ADF      6

typedef struct abf_f32_fastdata_params_t
{
    ee_f32_t alpha_BM_NLMS;
    ee_f32_t DS_DET_TH;
    ee_f32_t ep_GSC;
} abf_f32_fastdata_params_t;

typedef struct abf_f32_fastdata_mem_t
{
    ee_f32_t states_BM_ADF[NFFT / 2 + 1][LEN_BM_ADF * 2];
    ee_f32_t coefs_BM_ADF[NFFT / 2 + 1][LEN_BM_ADF * 2];
    ee_f32_t Norm_out_BM[NFFT / 2 + 1];
    ee_f32_t lookBF_out[NFFT / 2 + 1];
    ee_f32_t GSC_det_avg;
    uint8_t  adptBF_coefs_update_enable;
} abf_f32_fastdata_mem_t;

/* Fast coefficient structure */
typedef struct abf_f32_fastdata_static_t
{
    ee_f32_t                  old_left[NFFTD2 * COMPLEX];
    ee_f32_t                  old_right[NFFTD2 * COMPLEX];
    ee_f32_t                  ola_new[NFFTD2];
    ee_rfft_f32_t             rS;
    ee_cfft_f32_t             cS;
    abf_f32_fastdata_params_t bf_params;
    abf_f32_fastdata_mem_t    bf_mem;
} abf_f32_fastdata_static_t;

/* "working" / scratch area */
typedef struct abf_f32_fastdata_working_t
{
    ee_f32_t mic[NFFT * COMPLEX + 2];
    ee_f32_t BM[NFFT * COMPLEX + 2];
    ee_f32_t BF[NFFT * COMPLEX + 2];
    ee_f32_t X0[NFFT * COMPLEX + 2];
    ee_f32_t Y0[NFFT * COMPLEX + 2];
    ee_f32_t CY0[NFFTD2 * COMPLEX + 2];
    ee_f32_t XY[NFFTD2 * COMPLEX];
    ee_f32_t PHATNORM[NFFTD2 * COMPLEX];
    ee_f32_t allDerot[LAGSTEP];
    ee_f32_t corr;
    uint32_t icorr;
} abf_f32_fastdata_working_t;

typedef struct abf_f32_instance_t
{
    ee_f32_t                   *wrot;
    ee_f32_t                   *window;
    abf_f32_fastdata_static_t  *st;
    abf_f32_fastdata_working_t *w;
} abf_f32_instance_t;

int32_t ee_abf_f32(int32_t command,
                   void  **pp_instance,
                   void   *p_data,
                   void   *p_parameters);

#endif
