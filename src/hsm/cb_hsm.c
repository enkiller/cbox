/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-06-03     tyx          first implementation
 */

#include "cb_hsm.h"

// Built-in base events that control state transitions of the state machine
const cb_hsm_event_t cb_hsm_event_null = { CB_HSM_TOPIC_NULL };
const cb_hsm_event_t cb_hsm_event_enter = { CB_HSM_TOPIC_ENTER };
const cb_hsm_event_t cb_hsm_event_exit = { CB_HSM_TOPIC_EXIT };
const cb_hsm_event_t cb_hsm_event_init = { CB_HSM_TOPIC_INIT };

// The top level state, the root node of the state
cb_hsm_ret_t cb_hsm_state_top(cb_hsm_actor_t* me, const cb_hsm_event_t* const e)
{
    (void)(me);
    (void)(e);
    return CB_HSM_RET_IGNORED;
}

// Two path bifurcation nodes are probed
cb_hsm_state_t cb_hsm_bifurcation_node(cb_hsm_state_t *p1, int d1, cb_hsm_state_t *p2, int d2)
{
    cb_hsm_state_t res = cb_null;

    /*
     * Match the height of the trees on both sides
     */
    if (d1 > d2)
    {
        p1 += d1 - d2;
        d1 -= d1 - d2;
    }
    else if (d2 > d1)
    {
        p2 += d2 - d1;
        d2 -= d2 - d1;
    }
    if (d1 > 0 && d2 > 0 && p1[0] == p2[0])
    {
        /* 
         * In special cases, the child nodes are the same
         */
        res = p1[0];
    }
    else
    {
        /*
         * Loop to find the parent node
         */
        for (int i = 0; i < d1; i++)
        {
            if (p1[i] == p2[i])
            {
                res = p1[i];
                break;
            }
        }
    }
    return res;
}

// probe the path between top state and substate
int cb_hsm_detect_path(cb_hsm_actor_t *me, cb_hsm_state_t substate,
    cb_hsm_state_t *path, unsigned int max_depth)
{
    unsigned int depth = 0;
    int res = -1;
    // Back up the current state, which needs to be restored before the function returns
    cb_hsm_state_t parent = me->state;

    // Temporarily change state to substate state, probing parent from bottom to top
    me->state = substate;
    // First push the substate state
    if (max_depth > 0)
    {
        path[0] = me->state;
    }
    /*
     * Loop through the parent state until the target parent
     * state is found or the maximum depth is reached
    */
    while (depth < CB_HSM_STATE_LEVEL_MAX && me->state != parent)
    {
        /*
         * When the state machine receives a null state,
         * it must return its parent state.
         * The parent state of a state must be fixed.
         * It is not allowed to return different parent
         * states based on the internal state
        */
        me->state(me, &cb_hsm_event_null);
        depth += 1;
        if (depth < max_depth)
        {
            path[depth] = me->state;
        }
    }
    if (me->state == parent)
    {
        res = depth;
    }
    /*
     * The state will be changed during the detect.
     * After the detect is over,
     * the state must be restored to the original state
     */
    me->state = parent;
    return res;
}

// probe the ancestors of both nodes
static cb_hsm_state_t cb_hsm_detect_ancestor(cb_hsm_actor_t *me, cb_hsm_state_t original, cb_hsm_state_t target)
{
    cb_hsm_state_t path1[CB_HSM_STATE_LEVEL_MAX];
    cb_hsm_state_t path2[CB_HSM_STATE_LEVEL_MAX];
    int h1, h2;
    cb_hsm_state_t ancestor, bk = me->state;

    me->state = cb_hsm_state_top;
    /*
     * Calculates the height from the root node and then
     * starts at the same height until the common parent state is found
     */
    h1 = cb_hsm_detect_path(me, original, path1, CB_HSM_STATE_LEVEL_MAX);
    h2 = cb_hsm_detect_path(me, target, path2, CB_HSM_STATE_LEVEL_MAX);
    ancestor = cb_hsm_bifurcation_node(path1, h1, path2, h2);
    me->state = bk;
    return ancestor;
}

// Enter the target substate from the current state. The target state must be a child state
void cb_hsm_enter(cb_hsm_actor_t *me, cb_hsm_state_t substate)
{
    cb_hsm_state_t path[CB_HSM_STATE_LEVEL_MAX];
    cb_hsm_state_t top;
    cb_hsm_ret_t ret;

    do {
        /*
         * gets the path from the top state to the target state
         * path stores the path from the current state to the goal child state
         * Mapping relation: path[0]:path[depth] --> substate:top
        */
        int depth = cb_hsm_detect_path(me, substate, path, CB_HSM_STATE_LEVEL_MAX);
        // Enter the status of each layer in sequence
        while (--depth >= 0)
        {
            me->state = path[depth];
            me->state(me, &cb_hsm_event_enter);
        }
        top = me->state;
        ret = me->state(me, &cb_hsm_event_init);
        substate = me->state;
        me->state = top;
    } while (ret == CB_HSM_RET_TRAN);
}

// Exit child state to parent state
static void cb_hsm_exit(cb_hsm_actor_t *me, cb_hsm_state_t top)
{
    cb_hsm_state_t path[CB_HSM_STATE_LEVEL_MAX];
    int depth, i;
    cb_hsm_state_t substate = me->state;

    me->state = top;
    depth = cb_hsm_detect_path(me, substate, path, CB_HSM_STATE_LEVEL_MAX);
    // Enter the status of each layer in sequence
    for (i = 0; i < depth; i++)
    {
        me->state = path[i];
        me->state(me, &cb_hsm_event_exit);
    }
    // Set target state
    me->state = path[i];
}

static void cb_hsm_transition(cb_hsm_actor_t* me, cb_hsm_state_t target)
{
    cb_hsm_state_t ancestor;

    /*
     * Check whether it is a self transition,
     * self transition needs to exit the current state before entering
     */
    if (me->state == target)
    {
        me->state(me, &cb_hsm_event_null);
        ancestor = me->state;
        me->state = target;
    }
    else
    {
        /*
         * Search for the common parent node,
         * transition state needs to exit to
         * the parent node before entering the target state
         */
        ancestor = cb_hsm_detect_ancestor(me, me->state, target);
    }
    /*
     * Exits the original state to the common ancestor node
     */
    cb_hsm_exit(me, ancestor);
    /*
     * From ancestor state to target state
     */
    cb_hsm_enter(me, target);
}

void cb_hsm_start(cb_hsm_actor_t *me, cb_hsm_state_t state_init)
{
    cb_hsm_state_t top, substate;

    top = me->state;
    // do init
    state_init(me, &cb_hsm_event_null);
    substate = me->state;
    me->state = top;
    // enter target state
    cb_hsm_enter(me, substate);
}

cb_hsm_ret_t cb_hsm_tran(cb_hsm_actor_t *me, cb_hsm_state_t state)
{
    me->state = state;
    return CB_HSM_RET_TRAN;
}

cb_hsm_ret_t cb_hsm_super(cb_hsm_actor_t *me, cb_hsm_state_t state)
{
    me->state = state;
    return CB_HSM_RET_SUPER;
}

cb_hsm_ret_t cb_hsm_handle(cb_hsm_actor_t* me)
{
    (void)(me);
    return CB_HSM_RET_HANDLED;
}

void cb_hsm_event_handle(cb_hsm_actor_t *me, const cb_hsm_event_t * const e)
{
    cb_hsm_ret_t ret = CB_HSM_RET_SUPER;
    cb_hsm_state_t current, original = me->state;

    /*
     * Passes the event to the state machine for
     * processing until the event is processed.
     */
    while (ret == CB_HSM_RET_SUPER)
    {
        current = me->state;
        ret = me->state(me, e);
    }
    /*
     * Check whether a state transition is required.
     */
    if (ret == CB_HSM_RET_TRAN)
    {
        cb_hsm_state_t target = me->state;
        me->state = original;
        cb_hsm_exit(me, current);
        cb_hsm_transition(me, target);
    }
    else
    {
        me->state = original;
    }
}
