/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup CPthreadReqSpinInitPshared
 */

/*
 * Copyright (C) 2026 OHB System AG
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _TR_PTHREAD_SPIN_INIT_PSHARED_H
#define _TR_PTHREAD_SPIN_INIT_PSHARED_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CPthreadReqSpinInitPshared
 *
 * @{
 */

typedef enum {
  CPthreadReqSpinInitPshared_Pre_Pshared_Private,
  CPthreadReqSpinInitPshared_Pre_Pshared_Shared,
  CPthreadReqSpinInitPshared_Pre_Pshared_Invalid,
  CPthreadReqSpinInitPshared_Pre_Pshared_NA
} CPthreadReqSpinInitPshared_Pre_Pshared;

typedef enum {
  CPthreadReqSpinInitPshared_Post_Status_Ok,
  CPthreadReqSpinInitPshared_Post_Status_NA
} CPthreadReqSpinInitPshared_Post_Status;

/**
 * @brief Runs the parameterized test case.
 */
void CPthreadReqSpinInitPshared_Run( void );

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _TR_PTHREAD_SPIN_INIT_PSHARED_H */
