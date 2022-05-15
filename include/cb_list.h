/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-03-16     tyx          first implementation
 */

#ifndef __CB_LIST_H__
#define __CB_LIST_H__

#include "cb_def.h"

#ifdef __cplusplus
extern "C" {
#endif

struct cb_list_node
{
    struct cb_list_node *next;                          /**< point to next node. */
    struct cb_list_node *prev;                          /**< point to prev node. */
};
typedef struct cb_list_node cb_list_t;                  /**< Type for lists. */

#define cb_container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - (char *)(&((type *)0)->member)))


#define CB_LIST_OBJECT_INIT(object) { &(object), &(object) }

/**
 * @brief initialize a list object
 *
 * @param l the list to be initialized
 */
cb_inline void cb_list_init(cb_list_t *l)
{
    l->next = l->prev = l;
}

cb_inline void cb_list_insert_after(cb_list_t *l, cb_list_t *n)
{
    l->next->prev = n;
    n->next = l->next;

    l->next = n;
    n->prev = l;
}

cb_inline void cb_list_insert_before(cb_list_t *l, cb_list_t *n)
{
    l->prev->next = n;
    n->prev = l->prev;

    l->prev = n;
    n->next = l;
}

cb_inline void cb_list_remove(cb_list_t *n)
{
    n->next->prev = n->prev;
    n->prev->next = n->next;

    n->next = n->prev = n;
}

cb_inline cb_list_t *cb_list_first(cb_list_t *l)
{
    return l->next;
}

cb_inline int cb_list_isempty(const cb_list_t *l)
{
    return l->next == l;
}

cb_inline unsigned int cb_list_len(const cb_list_t *l)
{
    unsigned int len = 0;
    const cb_list_t *p = l;
    while (p->next != l)
    {
        p = p->next;
        len ++;
    }

    return len;
}

/**
 * @brief get the struct for this entry
 * @param node the entry point
 * @param type the type of structure
 * @param member the name of list in structure
 */
#define cb_list_entry(node, type, member) \
    cb_container_of(node, type, member)

/**
 * cb_list_for_each - iterate over a list
 * @pos:    the cb_list_t * to use as a loop cursor.
 * @head:   the head for your list.
 */
#define cb_list_for_each(pos, head) \
    for (cb_list_t *pos = (head)->next, *nn = pos->next; pos != (head); pos = nn, nn = pos->next)

/**
 * cb_list_first_entry - get the first element from a list
 * @ptr:    the list head to take the element from.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the list_struct within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define cb_list_first_entry(ptr, type, member) \
    cb_list_entry((ptr)->next, type, member)

#define CB_SLIST_OBJECT_INIT(object) { CB_NULL }


/**
 * Hash Node list structure
*/
struct cb_hlist_node
{
    struct cb_hlist_node *next;                         /**< point to next node. */
    struct cb_hlist_node **pprev;                       /**< Point to the next node of the previous node */
};
typedef struct cb_hlist_node cb_hlist_t;

/**
 * Hash Head List structure
*/
struct cb_hlist_head
{
    cb_hlist_t *first;                        /**< Point to the first node. */
};
typedef struct cb_hlist_head cb_hhead_t;

#define CB_HASH_HEAD_OBJECT_INIT(object) { cb_null }
#define CB_HASH_NODE_OBJECT_INIT(object) { cb_null, cb_null}

/**
 * @brief initialize a hash head object
 *
 * @param h the hsah head to be initialized
 */
cb_inline void cb_hlist_head_init(struct cb_hlist_head *h)
{
    h->first = cb_null;
}

/**
 * @brief initialize a hash list
 *
 * @param l hash list to be initialized
 */
cb_inline void cb_hlist_init(cb_hlist_t *l)
{
    l->next = cb_null;
    l->pprev = cb_null;
}

/**
 * @brief insert a node after a hash list
 *
 * @param l list to insert it
 * @param n new node to be inserted
 */
cb_inline void cb_hlist_insert_after(cb_hlist_t *l, cb_hlist_t *n)
{
    n->next = l->next;
    l->next = n;

    n->pprev = &l->next;
    if (n->next) n->next->pprev = &n->next;
}

/**
 * @brief insert a node before a hash list
 *
 * @param n new node to be inserted
 * @param l list to insert it
 */
cb_inline void cb_hlist_insert_before(cb_hlist_t *l, cb_hlist_t *n)
{
    n->next = l;
    *(l->pprev) = n;

    n->pprev = l->pprev;
    l->pprev = &n->next;
}

/**
 * @brief insert a node in head
 *
 * @param n new node to be inserted
 * @param h hash head to insert it
 */
cb_inline void cb_hlist_insert_head(struct cb_hlist_head *h, cb_hlist_t *n)
{
    n->next = h->first;
    if (h->first) h->first->pprev = &n->next;

    h->first = n;
    n->pprev = &h->first;
}

/**
 * @brief remove node from hash list.
 * @param n the node to remove from the hash list.
 */
cb_inline void cb_hlist_remove(cb_hlist_t *n)
{
    if (n->pprev)
    {
        *(n->pprev) = n->next;
        if (n->next) n->next->pprev = n->pprev;
    }
    n->next = cb_null;
    n->pprev = cb_null;
}

/**
 * @brief Move the hash list to another head
 *
 * @param des new list head to be moved
 * @param sec old list head to move it
 */
cb_inline void cb_hlist_move(struct cb_hlist_head *des,
    struct cb_hlist_head *src)
{
    des->first = src->first;
    if (des->first) des->first->pprev = &des->first;
    src->first = cb_null;
}

/**
 * @brief tests whether a hash list is empty
 * @param h the list head to test.
 */
cb_inline int cb_hlist_isempty(struct cb_hlist_head *h)
{
    return h->first == cb_null;
}

/**
 * @brief tests Whether the node is in the hash list
 * @param l the list to test.
 */
cb_inline int cb_hlist_unhashed(cb_hlist_t *l)
{
    return l->pprev == cb_null;
}

/**
 * @brief get the hash list length
 * @param h the list head to get.
 */
cb_inline unsigned int cb_hlist_len(const struct cb_hlist_head *h)
{
    unsigned int len = 0;
    cb_hlist_t *l = h->first;
    while (l)
    {
        len ++;
        l = l->next;
    }
    return len;
}

cb_inline cb_hlist_t *cb_hlist_first(const struct cb_hlist_head *h)
{
    return h->first;
}

cb_inline cb_hlist_t *cb_hlist_tail(const struct cb_hlist_head *h)
{
    cb_hlist_t *l = h->first;
    if (l) while (l->next) l = l->next;
    return l;
}

/**
 * @brief get the struct for this hash list node
 * @param node the entry point
 * @param type the type of structure
 * @param member the name of list in structure
 */
#define cb_hlist_entry(node, type, member) \
    cb_container_of(node, type, member)

/**
 * cb_hlist_for_each - iterate over a hash list
 * @pos:    the cb_hlist_t * to use as a loop cursor.
 * @head:   the head for your cb_hlist_head.
 */
#define cb_hlist_for_each(pos, head) \
    for (cb_hlist_t *pos = (head)->first, *nn = pos ? pos->next : cb_null; pos != cb_null; \
        pos = nn, nn = pos ? pos->next : cb_null)

/**
 * cb_hlist_first_entry - get the first element from a hash slist
 * @ptr:    the hlist head to take the element from.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the slist_struct within the struct.
 *
 * Note, that hlist is expected to be not empty.
 */
#define cb_hlist_first_entry(ptr, type, member) \
    cb_hlist_entry((ptr)->first, type, member)

/**@}*/

#ifdef __cplusplus
}
#endif

#endif
