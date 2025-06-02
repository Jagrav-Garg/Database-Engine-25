// btree.h
#ifndef BTREE_H
#define BTREE_H

#include <stdbool.h>

#define MAX_KEYS 2
#define MIN_KEYS 1
#define MAX_CHILDREN (MAX_KEYS+1)

typedef struct BTreeNode {
    int keys[MAX_KEYS];
    char* values[MAX_KEYS];
    struct BTreeNode* children[MAX_CHILDREN];
    int num_keys;
    bool is_leaf;
} BTreeNode;

BTreeNode* create_node(bool is_leaf);
void insert(BTreeNode** root, int key, const char* value);
char* search(BTreeNode* root, int key);
void delete_key(BTreeNode** root, int key);
void level_order_traversal(BTreeNode* root);
void free_btree(BTreeNode* root);
void print_tree(BTreeNode* root, int level);

#endif // BTREE_H