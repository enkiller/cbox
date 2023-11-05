/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-10-04     tyx          first implementation
 */

#ifndef CB_RADIXTREE_H_
#define CB_RADIXTREE_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * If runs on a 64-bit system,
 * You are advised to change the value to 16 to reduce the number of search layers
 */
#define CB_RADIX_TREE_MAP_SIZE 4

typedef union cb_radix_tree_node_slot
{
    union cb_radix_tree_node_slot* next_level;
    void* value;
    const void* const_value;
}cb_rtslots_t;

struct cb_radix_tree_root;
typedef struct cb_radix_tree_root cb_rtroot_t;
struct cb_radix_tree_root
{
    unsigned char height;
    unsigned char slot_cnt; // Recommended value 4 or 16
    unsigned char mask_bits;
    unsigned long long max_key;
    void *def_value;                // The default value. Mark unused space
    union cb_radix_tree_node_slot *root_slots;
    cb_rtslots_t *(*alloc_node)(cb_rtroot_t *root, unsigned char slots);
    void (*free_node)(cb_rtroot_t *root, cb_rtslots_t *ptr, unsigned char slots);
};

cb_rtroot_t *cb_radix_tree_init(cb_rtroot_t *root, unsigned char slots);
void cb_radix_tree_deinit(cb_rtroot_t* root);

void *cb_radix_tree_lookup(cb_rtroot_t *root, const void *key);
int cb_radix_tree_insert(cb_rtroot_t *root, const void *key, const void *value);
void *cb_radix_tree_remove(cb_rtroot_t *root, const void *key);
void cb_radix_tree_shrink(cb_rtroot_t* root);

#ifdef __cplusplus
}
#endif


#endif /* CB_RADIXTREE_H_ */
