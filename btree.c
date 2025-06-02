// btree.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_KEYS 2
#define MAX_CHILDREN (MAX_KEYS + 1)

typedef struct BTreeNode {
    int keys[MAX_KEYS];
    char* values[MAX_KEYS];
    struct BTreeNode* children[MAX_CHILDREN];
    int num_keys;
    bool is_leaf;
} BTreeNode;

BTreeNode* create_node(bool is_leaf) {
    BTreeNode* node = (BTreeNode*)malloc(sizeof(BTreeNode));
    if (!node) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    node->is_leaf = is_leaf;
    node->num_keys = 0;
    for(int i=0; i< MAX_CHILDREN; i++) node->children[i] = NULL;
    for(int i=0; i< MAX_KEYS; i++) {
        node->values[i] = NULL;
        node->keys[i] = 0;
    }
    return node;
}

char* search(BTreeNode* root, int key) {
    if (root == NULL) return NULL;
    int i = 0;
    while (i < root->num_keys && key > root->keys[i]) i++;
    if (i < root->num_keys && key == root->keys[i]) return root->values[i];
    if (root->is_leaf) return NULL;
    return search(root->children[i], key);
}

void split_child(BTreeNode* parent, int index) {
    BTreeNode* full_child = parent->children[index];
    BTreeNode* new_child = create_node(full_child->is_leaf);
    int mid = MAX_KEYS / 2;
    new_child->num_keys = MAX_KEYS - mid - 1;
    for (int j = 0; j < new_child->num_keys; j++) {
        new_child->keys[j] = full_child->keys[j + mid + 1];
        new_child->values[j] = full_child->values[j + mid + 1];
        full_child->values[j + mid + 1] = NULL;
    }
    if (!full_child->is_leaf) {
        for (int j = 0; j <= new_child->num_keys; j++) {
            new_child->children[j] = full_child->children[j + mid + 1];
            full_child->children[j + mid + 1] = NULL;
        }
    }
    full_child->num_keys = mid;
    for (int j = parent->num_keys; j >= index + 1; j--) parent->children[j + 1] = parent->children[j];
    parent->children[index + 1] = new_child;
    for (int j = parent->num_keys - 1; j >= index; j--) {
        parent->keys[j + 1] = parent->keys[j];
        parent->values[j + 1] = parent->values[j];
    }
    parent->keys[index] = full_child->keys[mid];
    parent->values[index] = full_child->values[mid];
    full_child->values[mid] = NULL;
    parent->num_keys++;
}

void insert_non_full(BTreeNode* node, int key, const char* value) {
    int i = node->num_keys - 1;
    if (node->is_leaf) {
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            node->values[i + 1] = node->values[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->values[i + 1] = strdup(value);
        node->num_keys++;
    } else {
        while (i >= 0 && key < node->keys[i]) i--;
        i++;
        if (node->children[i] && node->children[i]->num_keys == MAX_KEYS) {
            split_child(node, i);
            if (key > node->keys[i]) i++;
        }
        insert_non_full(node->children[i], key, value);
    }
}

void insert(BTreeNode** root, int key, const char* value) {
    if (*root == NULL) {
        *root = create_node(true);
        (*root)->keys[0] = key;
        (*root)->values[0] = strdup(value);
        (*root)->num_keys = 1;
        return;
    }
    if ((*root)->num_keys == MAX_KEYS) {
        BTreeNode* new_root = create_node(false);
        new_root->children[0] = *root;
        split_child(new_root, 0);
        insert_non_full(new_root, key, value);
        *root = new_root;
    } else {
        insert_non_full(*root, key, value);
    }
}

void level_order_traversal(BTreeNode* root) {
    if (root == NULL) {
        printf("Tree is empty.\n");
        return;
    }
    BTreeNode* queue[1000];
    int levels[1000];
    int front = 0, rear = 0;
    queue[rear] = root;
    levels[rear] = 0;
    rear++;
    int current_level = -1;
    printf("Tree Structure:\n");
    while (front < rear) {
        BTreeNode* node = queue[front];
        int level = levels[front];
        front++;
        if (node->num_keys == 0) continue;
        if (level != current_level) {
            if (current_level != -1) printf("\n");
            printf("Level %d: ", level);
            current_level = level;
        } else {
            printf(" ");
        }
        printf("[[");
        for(int i=0; i< node->num_keys; i++) {
            printf("%d:%s", node->keys[i], node->values[i] ? node->values[i] : "NULL");
            if (i < node->num_keys - 1) printf(", ");
        }
        printf("]]");
        if (!node->is_leaf) {
            for(int i=0; i<= node->num_keys; i++) {
                if (node->children[i] != NULL) {
                    queue[rear] = node->children[i];
                    levels[rear] = level + 1;
                    rear++;
                }
            }
        }
    }
    printf("\n");
}

void free_btree(BTreeNode* root) {
    if (root != NULL) {
        if (!root->is_leaf) {
            for(int i=0; i<= root->num_keys; i++) {
                free_btree(root->children[i]);
            }
        }
        for(int i=0; i< root->num_keys; i++) {
            if (root->values[i]) free(root->values[i]);
        }
        free(root);
    }
}
