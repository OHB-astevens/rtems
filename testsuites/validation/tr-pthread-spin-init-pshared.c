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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <pthread.h>

#include "tr-pthread-spin-init-pshared.h"

#include <rtems/test.h>

/**
 * @defgroup CPthreadReqSpinInitPshared spec:/c/pthread/req/spin-init-pshared
 *
 * @ingroup TestsuitesValidationNoClock1
 *
 * @{
 */

typedef struct {
  uint8_t Skip : 1;
  uint8_t Pre_Pshared_NA : 1;
  uint8_t Post_Status : 1;
} CPthreadReqSpinInitPshared_Entry;

/**
 * @brief Test context for spec:/c/pthread/req/spin-init-pshared test case.
 */
typedef struct {
  /**
   * @brief This member specifies the `pshared` parameter value.
   */
  int pshared;

  /**
   * @brief This member contains the return value of the pthread_spin_init()
   *   call.
   */
  int status;

  /**
   * @brief This member contains the spin lock object.
   */
  pthread_spinlock_t lock;

  struct {
    /**
     * @brief This member defines the pre-condition states for the next action.
     */
    size_t pcs[ 1 ];

    /**
     * @brief If this member is true, then the test action loop is executed.
     */
    bool in_action_loop;

    /**
     * @brief This member contains the next transition map index.
     */
    size_t index;

    /**
     * @brief This member contains the current transition map entry.
     */
    CPthreadReqSpinInitPshared_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} CPthreadReqSpinInitPshared_Context;

static CPthreadReqSpinInitPshared_Context CPthreadReqSpinInitPshared_Instance;

static const char *const CPthreadReqSpinInitPshared_PreDesc_Pshared[] =
  { "Private", "Shared", "Invalid", "NA" };

static const char *const *const CPthreadReqSpinInitPshared_PreDesc[] = {
  CPthreadReqSpinInitPshared_PreDesc_Pshared,
  NULL
};

#define PSHARED_INVALID 123

static void CPthreadReqSpinInitPshared_Pre_Pshared_Prepare(
  CPthreadReqSpinInitPshared_Context    *ctx,
  CPthreadReqSpinInitPshared_Pre_Pshared state
)
{
  switch ( state ) {
    case CPthreadReqSpinInitPshared_Pre_Pshared_Private: {
      /*
       * While the `pshared` parameter is PTHREAD_PROCESS_PRIVATE.
       */
      ctx->pshared = PTHREAD_PROCESS_PRIVATE;
      break;
    }

    case CPthreadReqSpinInitPshared_Pre_Pshared_Shared: {
      /*
       * While the `pshared` parameter is PTHREAD_PROCESS_SHARED.
       */
      ctx->pshared = PTHREAD_PROCESS_SHARED;
      break;
    }

    case CPthreadReqSpinInitPshared_Pre_Pshared_Invalid: {
      /*
       * While the `pshared` parameter is not a valid process-shared attribute
       * value.
       */
      ctx->pshared = PSHARED_INVALID;
      break;
    }

    case CPthreadReqSpinInitPshared_Pre_Pshared_NA:
      break;
  }
}

static void CPthreadReqSpinInitPshared_Post_Status_Check(
  CPthreadReqSpinInitPshared_Context    *ctx,
  CPthreadReqSpinInitPshared_Post_Status state
)
{
  switch ( state ) {
    case CPthreadReqSpinInitPshared_Post_Status_Ok: {
      /*
       * The return value of pthread_spin_init() shall be zero.
       */
      T_eq_int( ctx->status, 0 );
      break;
    }

    case CPthreadReqSpinInitPshared_Post_Status_NA:
      break;
  }
}

static void CPthreadReqSpinInitPshared_Action(
  CPthreadReqSpinInitPshared_Context *ctx
)
{
  ctx->status = pthread_spin_init( &ctx->lock, ctx->pshared );
}

static void CPthreadReqSpinInitPshared_Cleanup(
  CPthreadReqSpinInitPshared_Context *ctx
)
{
  T_eq_int( pthread_spin_destroy( &ctx->lock ), 0 );
}

/* clang-format off */

static const CPthreadReqSpinInitPshared_Entry
CPthreadReqSpinInitPshared_Entries[] = {
  { 0, 0, CPthreadReqSpinInitPshared_Post_Status_Ok }
};

static const uint8_t
CPthreadReqSpinInitPshared_Map[] = {
  0, 0, 0
};

/* clang-format on */

static size_t CPthreadReqSpinInitPshared_Scope(
  void  *arg,
  char  *buf,
  size_t n
)
{
  CPthreadReqSpinInitPshared_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope(
      CPthreadReqSpinInitPshared_PreDesc,
      buf,
      n,
      ctx->Map.pcs
    );
  }

  return 0;
}

static T_fixture CPthreadReqSpinInitPshared_Fixture = {
  .setup = NULL,
  .stop = NULL,
  .teardown = NULL,
  .scope = CPthreadReqSpinInitPshared_Scope,
  .initial_context = &CPthreadReqSpinInitPshared_Instance
};

static inline CPthreadReqSpinInitPshared_Entry
CPthreadReqSpinInitPshared_PopEntry( CPthreadReqSpinInitPshared_Context *ctx )
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return CPthreadReqSpinInitPshared_Entries
    [ CPthreadReqSpinInitPshared_Map[ index ] ];
}

static void CPthreadReqSpinInitPshared_TestVariant(
  CPthreadReqSpinInitPshared_Context *ctx
)
{
  CPthreadReqSpinInitPshared_Pre_Pshared_Prepare( ctx, ctx->Map.pcs[ 0 ] );
  CPthreadReqSpinInitPshared_Action( ctx );
  CPthreadReqSpinInitPshared_Post_Status_Check(
    ctx,
    ctx->Map.entry.Post_Status
  );
}

static T_fixture_node CPthreadReqSpinInitPshared_Node;

static T_remark CPthreadReqSpinInitPshared_Remark = {
  .next = NULL,
  .remark = "CPthreadReqSpinInitPshared"
};

void CPthreadReqSpinInitPshared_Run( void )
{
  CPthreadReqSpinInitPshared_Context *ctx;

  ctx = T_push_fixture(
    &CPthreadReqSpinInitPshared_Node,
    &CPthreadReqSpinInitPshared_Fixture
  );
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pcs[ 0 ] = CPthreadReqSpinInitPshared_Pre_Pshared_Private;
    ctx->Map.pcs[ 0 ] < CPthreadReqSpinInitPshared_Pre_Pshared_NA;
    ++ctx->Map.pcs[ 0 ]
  ) {
    ctx->Map.entry = CPthreadReqSpinInitPshared_PopEntry( ctx );
    CPthreadReqSpinInitPshared_TestVariant( ctx );
    CPthreadReqSpinInitPshared_Cleanup( ctx );
  }

  T_add_remark( &CPthreadReqSpinInitPshared_Remark );
  T_pop_fixture();
}

/** @} */
