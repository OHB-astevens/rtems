/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup CPthreadValSpin
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

#include "tr-isr-lock-acquire.h"
#include "tr-isr-lock-destroy.h"
#include "tr-isr-lock-initialize.h"
#include "tr-isr-lock-release.h"
#include "tr-pthread-spin-init-pshared.h"
#include "tx-isr-lock.h"
#include "tx-support.h"

#include <rtems/test.h>

/**
 * @defgroup CPthreadValSpin spec:/c/pthread/val/spin
 *
 * @ingroup TestsuitesValidationNoClock1
 *
 * @brief Tests the spin lock functions.
 *
 * This test case performs the following actions:
 *
 * - Validate the initialization of a spin lock.
 *
 *   - Validate pthread_spin_init().
 *
 *   - Validate the `pshared` parameter of pthread_spin_init().
 *
 * - Validate the functions which acquire and release a spin lock.
 *
 *   - Validate pthread_spin_lock().
 *
 *   - Validate pthread_spin_unlock().
 *
 * - Validate the function which acquires a spin lock as pthread_spin_lock()
 *   does.
 *
 *   - Validate pthread_spin_trylock().
 *
 * - Validate the function which destroys a spin lock.
 *
 *   - Validate pthread_spin_destroy().
 *
 * - Initialize a spin lock with pthread_spin_init(). Call each spin lock
 *   function once.
 *
 *   - Acquire the spin lock with pthread_spin_lock() and release it with
 *     pthread_spin_unlock(). Check that pthread_spin_lock() returned zero.
 *
 *   - Check that pthread_spin_unlock() returned zero.
 *
 *   - Acquire the spin lock with pthread_spin_trylock() and release it with
 *     pthread_spin_unlock(). Check that both functions returned zero.
 *
 *   - Destroy the spin lock. Check that pthread_spin_destroy() returned zero.
 *
 * @{
 */

/**
 * @brief Test context for spec:/c/pthread/val/spin test case.
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
} CPthreadValSpin_Context;

static CPthreadValSpin_Context CPthreadValSpin_Instance;

static T_fixture CPthreadValSpin_Fixture = {
  .setup = NULL,
  .stop = NULL,
  .teardown = NULL,
  .scope = NULL,
  .initial_context = &CPthreadValSpin_Instance
};

/**
 * @brief Validate the initialization of a spin lock.
 */
static void CPthreadValSpin_Action_0( CPthreadValSpin_Context *ctx )
{
  ISRLockSetPOSIX( &ctx->lock_ctx, &ctx->lock, false );

  /*
   * Validate pthread_spin_init().
   */
  ScoreIsrReqLockInitialize_Run( &ctx->lock_ctx );

  /*
   * Validate the `pshared` parameter of pthread_spin_init().
   */
  CPthreadReqSpinInitPshared_Run();
}

/**
 * @brief Validate the functions which acquire and release a spin lock.
 */
static void CPthreadValSpin_Action_1( CPthreadValSpin_Context *ctx )
{
  ISRLockSetPOSIX( &ctx->lock_ctx, &ctx->lock, false );

  /*
   * Validate pthread_spin_lock().
   */
  ScoreIsrReqLockAcquire_Run( &ctx->lock_ctx );

  /*
   * Validate pthread_spin_unlock().
   */
  ScoreIsrReqLockRelease_Run( &ctx->lock_ctx );
}

/**
 * @brief Validate the function which acquires a spin lock as
 *   pthread_spin_lock() does.
 */
static void CPthreadValSpin_Action_2( CPthreadValSpin_Context *ctx )
{
  ISRLockSetPOSIX( &ctx->lock_ctx, &ctx->lock, true );

  /*
   * Validate pthread_spin_trylock().
   */
  ScoreIsrReqLockAcquire_Run( &ctx->lock_ctx );
}

/**
 * @brief Validate the function which destroys a spin lock.
 */
static void CPthreadValSpin_Action_3( CPthreadValSpin_Context *ctx )
{
  ISRLockSetPOSIX( &ctx->lock_ctx, &ctx->lock, false );

  /*
   * Validate pthread_spin_destroy().
   */
  ScoreIsrReqLockDestroy_Run( &ctx->lock_ctx );
}

/**
 * @brief Initialize a spin lock with pthread_spin_init(). Call each spin lock
 *   function once.
 */
static void CPthreadValSpin_Action_4( CPthreadValSpin_Context *ctx )
{
  int acquire_status;
  int release_status;

  T_eq_int( pthread_spin_init( &ctx->lock, PTHREAD_PROCESS_PRIVATE ), 0 );

  /*
   * Acquire the spin lock with pthread_spin_lock() and release it with
   * pthread_spin_unlock(). Check that pthread_spin_lock() returned zero.
   */
  acquire_status = pthread_spin_lock( &ctx->lock );
  release_status = pthread_spin_unlock( &ctx->lock );
  T_eq_int( acquire_status, 0 );

  /*
   * Check that pthread_spin_unlock() returned zero.
   */
  T_eq_int( release_status, 0 );

  /*
   * Acquire the spin lock with pthread_spin_trylock() and release it with
   * pthread_spin_unlock(). Check that both functions returned zero.
   */
  acquire_status = pthread_spin_trylock( &ctx->lock );
  release_status = pthread_spin_unlock( &ctx->lock );
  T_eq_int( acquire_status, 0 );
  T_eq_int( release_status, 0 );

  /*
   * Destroy the spin lock. Check that pthread_spin_destroy() returned zero.
   */
  T_eq_int( pthread_spin_destroy( &ctx->lock ), 0 );
}

/**
 * @fn void T_case_body_CPthreadValSpin( void )
 */
T_TEST_CASE_FIXTURE( CPthreadValSpin, &CPthreadValSpin_Fixture )
{
  CPthreadValSpin_Context *ctx;

  ctx = T_fixture_context();

  CPthreadValSpin_Action_0( ctx );
  CPthreadValSpin_Action_1( ctx );
  CPthreadValSpin_Action_2( ctx );
  CPthreadValSpin_Action_3( ctx );
  CPthreadValSpin_Action_4( ctx );
}

/** @} */
