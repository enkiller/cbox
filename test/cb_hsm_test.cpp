/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-06-03     tyx          first implementation
 */

#include <stdio.h>
#include <string.h>
#include <gtest/gtest.h>
#include "cb_hsm.h"

typedef struct sm_test_actor
{
    cb_hsm_actor_t super;
    int foo;
    char buff[256];
    char *ptr;
    char* end;
}sm_actor;

enum QHsmTstSignals {
    A_SIG = CB_HSM_TOPIC_USER,
    B_SIG,
    C_SIG,
    D_SIG,
    E_SIG,
    F_SIG,
    G_SIG,
    H_SIG,
    I_SIG,
    TERMINATE_SIG,
    MAX_SIG
};

static sm_actor test_actor;

cb_hsm_ret_t QHsmTst_initial(sm_actor *me, const cb_hsm_event_t * const e);
cb_hsm_ret_t QHsmTst_s2(sm_actor *me, const cb_hsm_event_t * const e);
cb_hsm_ret_t QHsmTst_s11(sm_actor *me, const cb_hsm_event_t * const e);
cb_hsm_ret_t QHsmTst_s211(sm_actor *me, const cb_hsm_event_t * const e);

const char* QHsmTst_ctor(void) {
    sm_actor *me = &test_actor;
    cb_hsm_init(&me->super);
    CB_HSM_START(&me->super, &QHsmTst_initial);
    return test_actor.buff;
}

cb_hsm_ret_t QHsmTst_initial(sm_actor *me, const cb_hsm_event_t * const e)
{
    (void)e;                      /* suppress "e not used" compiler warning */
    test_actor.ptr += snprintf(test_actor.ptr, test_actor.end - test_actor.ptr, "top-INIT;");
    me->foo = 0;                      /* initialize extended state variable */
    return CB_HSM_TRAN(me, &QHsmTst_s2);
}

cb_hsm_ret_t QHsmTst_s(sm_actor *me, const cb_hsm_event_t * const e) {
    switch (e->topic) {
        case CB_HSM_TOPIC_ENTER: {
            test_actor.ptr += snprintf(test_actor.ptr, test_actor.end - test_actor.ptr, "s-ENTRY;");
            return CB_HSM_HANDLED(me);
        }
        case CB_HSM_TOPIC_EXIT: {
            test_actor.ptr += snprintf(test_actor.ptr, test_actor.end - test_actor.ptr, "s-EXIT;");
            return CB_HSM_HANDLED(me);
        }
        case CB_HSM_TOPIC_INIT: {
            test_actor.ptr += snprintf(test_actor.ptr, test_actor.end - test_actor.ptr, "s-INIT;");
            return CB_HSM_TRAN(me, &QHsmTst_s11);
        }
        case E_SIG: {
            test_actor.ptr += snprintf(test_actor.ptr, test_actor.end - test_actor.ptr, "s-E;");
            return CB_HSM_TRAN(me, &QHsmTst_s11);
        }
        case I_SIG: {                   /* internal transition with a guard */
            if (me->foo) {
                test_actor.ptr += snprintf(test_actor.ptr, test_actor.end - test_actor.ptr, "s-I;");
                me->foo = 0;
                return CB_HSM_HANDLED(me);
            }
            break;
        }
        case TERMINATE_SIG: {
            test_actor.ptr += snprintf(test_actor.ptr, test_actor.end - test_actor.ptr, "exit\n\n;");
            return CB_HSM_HANDLED(me);
        }
    }
    return CB_HSM_SUPER(me, &cb_hsm_state_top);
}

cb_hsm_ret_t QHsmTst_s1(sm_actor *me, const cb_hsm_event_t * const e) {
    switch (e->topic) {
        case CB_HSM_TOPIC_ENTER: {
            test_actor.ptr += snprintf(test_actor.ptr, test_actor.end - test_actor.ptr, "s1-ENTRY;");
            return CB_HSM_HANDLED(me);
        }
        case CB_HSM_TOPIC_EXIT: {
            test_actor.ptr += snprintf(test_actor.ptr, test_actor.end - test_actor.ptr, "s1-EXIT;");
            return CB_HSM_HANDLED(me);
        }
        case CB_HSM_TOPIC_INIT: {
            test_actor.ptr += snprintf(test_actor.ptr, test_actor.end - test_actor.ptr, "s1-INIT;");
            return CB_HSM_TRAN(me, &QHsmTst_s11);
        }
        case A_SIG: {
            test_actor.ptr += snprintf(test_actor.ptr, test_actor.end - test_actor.ptr, "s1-A;");
            return CB_HSM_TRAN(me, &QHsmTst_s1);
        }
        case B_SIG: {
            test_actor.ptr += snprintf(test_actor.ptr, test_actor.end - test_actor.ptr, "s1-B;");
            return CB_HSM_TRAN(me, &QHsmTst_s11);
        }
        case C_SIG: {
            test_actor.ptr += snprintf(test_actor.ptr, test_actor.end - test_actor.ptr, "s1-C;");
            return CB_HSM_TRAN(me, &QHsmTst_s2);
        }
        case D_SIG: {                            /* transition with a gurad */
            if (!me->foo) {
                test_actor.ptr += snprintf(test_actor.ptr, test_actor.end - test_actor.ptr, "s1-D;");
                me->foo = 1;
                return CB_HSM_TRAN(me, &QHsmTst_s);
            }
            break;
        }
        case F_SIG: {
            test_actor.ptr += snprintf(test_actor.ptr, test_actor.end - test_actor.ptr, "s1-F;");
            return CB_HSM_TRAN(me, &QHsmTst_s211);
        }
        case I_SIG: {                                /* internal transition */
            test_actor.ptr += snprintf(test_actor.ptr, test_actor.end - test_actor.ptr, "s1-I;");
            return CB_HSM_HANDLED(me);
        }
    }
    return CB_HSM_SUPER(me, &QHsmTst_s);
}

/*..........................................................................*/
cb_hsm_ret_t QHsmTst_s11(sm_actor *me, const cb_hsm_event_t * const e) {
    switch (e->topic) {
        case CB_HSM_TOPIC_ENTER: {
            test_actor.ptr += snprintf(test_actor.ptr, test_actor.end - test_actor.ptr, "s11-ENTRY;");
            return CB_HSM_HANDLED(me);
        }
        case CB_HSM_TOPIC_EXIT: {
            test_actor.ptr += snprintf(test_actor.ptr, test_actor.end - test_actor.ptr, "s11-EXIT;");
            return CB_HSM_HANDLED(me);
        }
        case D_SIG: {                            /* transition with a gurad */
            if (me->foo) {
                test_actor.ptr += snprintf(test_actor.ptr, test_actor.end - test_actor.ptr, "s11-D;");
                me->foo = 0;
                return CB_HSM_TRAN(me, &QHsmTst_s1);
            }
            break;
        }
        case G_SIG: {
            test_actor.ptr += snprintf(test_actor.ptr, test_actor.end - test_actor.ptr, "s11-G;");
            return CB_HSM_TRAN(me, &QHsmTst_s211);
        }
        case H_SIG: {
            test_actor.ptr += snprintf(test_actor.ptr, test_actor.end - test_actor.ptr, "s11-H;");
            return CB_HSM_TRAN(me, &QHsmTst_s);
        }
    }
    return CB_HSM_SUPER(me, &QHsmTst_s1);
}
/*..........................................................................*/
cb_hsm_ret_t QHsmTst_s2(sm_actor *me, const cb_hsm_event_t * const e) {
    switch (e->topic) {
        case CB_HSM_TOPIC_ENTER: {
            test_actor.ptr += snprintf(test_actor.ptr, test_actor.end - test_actor.ptr, "s2-ENTRY;");
            return CB_HSM_HANDLED(me);
        }
        case CB_HSM_TOPIC_EXIT: {
            test_actor.ptr += snprintf(test_actor.ptr, test_actor.end - test_actor.ptr, "s2-EXIT;");
            return CB_HSM_HANDLED(me);
        }
        case CB_HSM_TOPIC_INIT: {
            test_actor.ptr += snprintf(test_actor.ptr, test_actor.end - test_actor.ptr, "s2-INIT;");
            return CB_HSM_TRAN(me, &QHsmTst_s211);
        }
        case C_SIG: {
            test_actor.ptr += snprintf(test_actor.ptr, test_actor.end - test_actor.ptr, "s2-C;");
            return CB_HSM_TRAN(me, &QHsmTst_s1);
        }
        case F_SIG: {
            test_actor.ptr += snprintf(test_actor.ptr, test_actor.end - test_actor.ptr, "s2-F;");
            return CB_HSM_TRAN(me, &QHsmTst_s11);
        }
        case I_SIG: {                   /* internal transition with a guard */
            if (!me->foo) {
                test_actor.ptr += snprintf(test_actor.ptr, test_actor.end - test_actor.ptr, "s2-I;");
                me->foo = 1;
                return CB_HSM_HANDLED(me);
            }
            break;
        }
    }
    return CB_HSM_SUPER(me, &QHsmTst_s);
}
/*..........................................................................*/
cb_hsm_ret_t QHsmTst_s21(sm_actor *me, const cb_hsm_event_t * const e) {
    switch (e->topic) {
        case CB_HSM_TOPIC_ENTER: {
            test_actor.ptr += snprintf(test_actor.ptr, test_actor.end - test_actor.ptr, "s21-ENTRY;");
            return CB_HSM_HANDLED(me);
        }
        case CB_HSM_TOPIC_EXIT: {
            test_actor.ptr += snprintf(test_actor.ptr, test_actor.end - test_actor.ptr, "s21-EXIT;");
            return CB_HSM_HANDLED(me);
        }
        case CB_HSM_TOPIC_INIT: {
            test_actor.ptr += snprintf(test_actor.ptr, test_actor.end - test_actor.ptr, "s21-INIT;");
            return CB_HSM_TRAN(me, &QHsmTst_s211);
        }
        case A_SIG: {
            test_actor.ptr += snprintf(test_actor.ptr, test_actor.end - test_actor.ptr, "s21-A;");
            return CB_HSM_TRAN(me, &QHsmTst_s21);
        }
        case B_SIG: {
            test_actor.ptr += snprintf(test_actor.ptr, test_actor.end - test_actor.ptr, "s21-B;");
            return CB_HSM_TRAN(me, &QHsmTst_s211);
        }
        case G_SIG: {
            test_actor.ptr += snprintf(test_actor.ptr, test_actor.end - test_actor.ptr, "s21-G;");
            return CB_HSM_TRAN(me, &QHsmTst_s1);
        }
    }
    return CB_HSM_SUPER(me, &QHsmTst_s2);
}
/*..........................................................................*/
cb_hsm_ret_t QHsmTst_s211(sm_actor *me, const cb_hsm_event_t * const e) {
    switch (e->topic) {
        case CB_HSM_TOPIC_ENTER: {
            test_actor.ptr += snprintf(test_actor.ptr, test_actor.end - test_actor.ptr, "s211-ENTRY;");
            return CB_HSM_HANDLED(me);
        }
        case CB_HSM_TOPIC_EXIT: {
            test_actor.ptr += snprintf(test_actor.ptr, test_actor.end - test_actor.ptr, "s211-EXIT;");
            return CB_HSM_HANDLED(me);
        }
        case D_SIG: {
            test_actor.ptr += snprintf(test_actor.ptr, test_actor.end - test_actor.ptr, "s211-D;");
            return CB_HSM_TRAN(me, &QHsmTst_s21);
        }
        case H_SIG: {
            test_actor.ptr += snprintf(test_actor.ptr, test_actor.end - test_actor.ptr, "s211-H;");
            return CB_HSM_TRAN(me, &QHsmTst_s);
        }
    }
    return CB_HSM_SUPER(me, &QHsmTst_s21);
}

static void result_clean(void)
{
    memset(test_actor.buff, 0, sizeof(test_actor.buff));
    test_actor.ptr = test_actor.buff;
    test_actor.end = &test_actor.buff[sizeof(test_actor.buff) / sizeof(test_actor.buff[0])];
}

static const char *dispatch(enum QHsmTstSignals sig) {
    cb_hsm_event_t e;
    result_clean();
    e.topic = sig & 0xFFFF;
    test_actor.ptr += snprintf(test_actor.ptr, test_actor.end - test_actor.ptr, "%c:", 'A' + sig - A_SIG);
    cb_hsm_event_handle(&test_actor.super, &e);                       /* dispatch the event */
    return test_actor.buff;
}

TEST(testCase, cb_hsm_test01)
{
    result_clean();
    EXPECT_STREQ(QHsmTst_ctor(), "top-INIT;s-ENTRY;s2-ENTRY;s2-INIT;s21-ENTRY;s211-ENTRY;");
    EXPECT_STREQ(dispatch(G_SIG), "G:s21-G;s211-EXIT;s21-EXIT;s2-EXIT;s1-ENTRY;s1-INIT;s11-ENTRY;");
    EXPECT_STREQ(dispatch(I_SIG), "I:s1-I;");
    EXPECT_STREQ(dispatch(A_SIG), "A:s1-A;s11-EXIT;s1-EXIT;s1-ENTRY;s1-INIT;s11-ENTRY;");
    EXPECT_STREQ(dispatch(D_SIG), "D:s1-D;s11-EXIT;s1-EXIT;s-INIT;s1-ENTRY;s11-ENTRY;");
    EXPECT_STREQ(dispatch(D_SIG), "D:s11-D;s11-EXIT;s1-INIT;s11-ENTRY;");
    EXPECT_STREQ(dispatch(C_SIG), "C:s1-C;s11-EXIT;s1-EXIT;s2-ENTRY;s2-INIT;s21-ENTRY;s211-ENTRY;");
    EXPECT_STREQ(dispatch(E_SIG), "E:s-E;s211-EXIT;s21-EXIT;s2-EXIT;s1-ENTRY;s11-ENTRY;");
    EXPECT_STREQ(dispatch(E_SIG), "E:s-E;s11-EXIT;s1-EXIT;s1-ENTRY;s11-ENTRY;");
    EXPECT_STREQ(dispatch(G_SIG), "G:s11-G;s11-EXIT;s1-EXIT;s2-ENTRY;s21-ENTRY;s211-ENTRY;");
    EXPECT_STREQ(dispatch(I_SIG), "I:s2-I;");
    EXPECT_STREQ(dispatch(I_SIG), "I:s-I;");
}
