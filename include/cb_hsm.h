/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-06-03     tyx          first implementation
 */

#ifndef CB_HSM_H_
#define CB_HSM_H_

#include "cb_def.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CB_HSM_STATE_LEVEL_MAX  (16)

typedef enum cb_hsm_ret
{
    CB_HSM_RET_NULL = 0,
    CB_HSM_RET_IGNORED,
    CB_HSM_RET_HANDLED,
    CB_HSM_RET_SUPER,
    CB_HSM_RET_TRAN,
} cb_hsm_ret_t;

typedef unsigned short cb_hsm_topic_t;

#define CB_HSM_TOPIC_NULL     (0x00u)
#define CB_HSM_TOPIC_ENTER    (0x01u)
#define CB_HSM_TOPIC_EXIT     (0x02u)
#define CB_HSM_TOPIC_INIT     (0x03u)
#define CB_HSM_TOPIC_USER     (0x10u)

struct cb_hsm_event;
typedef struct cb_hsm_event cb_hsm_event_t;
struct cb_hsm_event
{
    cb_hsm_topic_t topic;
};

struct cb_hsm_actor;
typedef struct cb_hsm_actor cb_hsm_actor_t;
typedef cb_hsm_ret_t (*cb_hsm_state_t)(cb_hsm_actor_t *me, const cb_hsm_event_t * const e);

struct cb_hsm_actor
{
    volatile cb_hsm_state_t state;
};

#define CB_HSM_START(_me, _init)             cb_hsm_start((cb_hsm_actor_t*)(_me), (cb_hsm_state_t)_init)
#define CB_HSM_TRAN(_me, _target)            cb_hsm_tran((cb_hsm_actor_t*)(_me), (cb_hsm_state_t)_target)
#define CB_HSM_SUPER(_me, _super)            cb_hsm_super((cb_hsm_actor_t*)(_me), (cb_hsm_state_t)_super)
#define CB_HSM_HANDLED(_me)                  cb_hsm_handle((cb_hsm_actor_t*)(_me))

// Event handler. When a state machine receives an event, it processes the event using this function
void cb_hsm_event_handle(cb_hsm_actor_t *me, const cb_hsm_event_t * const e);

cb_hsm_ret_t cb_hsm_state_top(cb_hsm_actor_t* me, const cb_hsm_event_t* const e);
// The hierarchical state machine object is initialized, setting the state to the top-level state.
cb_inline void cb_hsm_init(cb_hsm_actor_t *me)
{
    me->state = cb_hsm_state_top;
}
// Instead of using these functions directly, use the macro CB_HSM_xx
void cb_hsm_start(cb_hsm_actor_t *me, cb_hsm_state_t state_init);
cb_hsm_ret_t cb_hsm_tran(cb_hsm_actor_t *me, cb_hsm_state_t state);
cb_hsm_ret_t cb_hsm_super(cb_hsm_actor_t *me, cb_hsm_state_t state);
cb_hsm_ret_t cb_hsm_handle(cb_hsm_actor_t *me);

#ifdef __cplusplus
}
#endif

#endif
