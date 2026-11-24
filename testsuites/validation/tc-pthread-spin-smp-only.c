/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup CPthreadValSpinSmpOnly
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <pthread.h>

#include "tr-isr-lock-fifo.h"
#include "tx-isr-lock.h"
#include "tx-support.h"

#include <rtems/test.h>

/**
 * @defgroup CPthreadValSpinSmpOnly spec:/c/pthread/val/spin-smp-only
 *
 * @ingroup TestsuitesValidationSmpOnly0
 *
 * @brief Tests the spin lock functions in SMP configurations.
 *
 * This test case performs the following actions:
 *
 * - Initialize a spin lock with pthread_spin_init().
 *
 *   - Check that no processor owns the spin lock after the initialization.
 *
 *   - Destroy the spin lock.
 *
 * - Validate the order in which processors acquire a spin lock.
 *
 *   - Validate the order where the processors acquire the spin lock with
 *     pthread_spin_lock().
 *
 *   - Validate the order where the processors acquire the spin lock with
 *     pthread_spin_trylock().
 *
 * @{
 */

/**
 * @brief Test context for spec:/c/pthread/val/spin-smp-only test case.
 */
typedef struct {
  /**
   * @brief This member contains the context of the ISR lock test runs.
   */
  ISRLockContext lock_ctx;

  /**
   * @brief This member contains the spin lock under test.
   */
  pthread_spinlock_t lock;
} CPthreadValSpinSmpOnly_Context;

static CPthreadValSpinSmpOnly_Context CPthreadValSpinSmpOnly_Instance;

static T_fixture CPthreadValSpinSmpOnly_Fixture = {
  .setup = NULL,
  .stop = NULL,
  .teardown = NULL,
  .scope = NULL,
  .initial_context = &CPthreadValSpinSmpOnly_Instance
};

/**
 * @brief Initialize a spin lock with pthread_spin_init().
 */
static void CPthreadValSpinSmpOnly_Action_0(
  CPthreadValSpinSmpOnly_Context *ctx
)
{
  ISRLockSetPOSIX( &ctx->lock_ctx, &ctx->lock, false );
  T_eq_int( pthread_spin_init( &ctx->lock, PTHREAD_PROCESS_PRIVATE ), 0 );

  /*
   * Check that no processor owns the spin lock after the initialization.
   */
  T_true( ISRLockIsFree( &ctx->lock_ctx ) );

  /*
   * Destroy the spin lock.
   */
  T_eq_int( pthread_spin_destroy( &ctx->lock ), 0 );
}

/**
 * @brief Validate the order in which processors acquire a spin lock.
 */
static void CPthreadValSpinSmpOnly_Action_1(
  CPthreadValSpinSmpOnly_Context *ctx
)
{
  ISRLockSetPOSIX( &ctx->lock_ctx, &ctx->lock, false );

  /*
   * Validate the order where the processors acquire the spin lock with
   * pthread_spin_lock().
   */
  ScoreIsrReqLockFifo_Run( &ctx->lock_ctx );

  /*
   * Validate the order where the processors acquire the spin lock with
   * pthread_spin_trylock().
   */
  ISRLockSetPOSIX( &ctx->lock_ctx, &ctx->lock, true );
  ScoreIsrReqLockFifo_Run( &ctx->lock_ctx );
}

/**
 * @fn void T_case_body_CPthreadValSpinSmpOnly( void )
 */
T_TEST_CASE_FIXTURE( CPthreadValSpinSmpOnly, &CPthreadValSpinSmpOnly_Fixture )
{
  CPthreadValSpinSmpOnly_Context *ctx;

  ctx = T_fixture_context();

  CPthreadValSpinSmpOnly_Action_0( ctx );
  CPthreadValSpinSmpOnly_Action_1( ctx );
}

/** @} */
