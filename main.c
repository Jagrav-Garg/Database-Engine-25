// main.c
#include <stdio.h>
#include "btree.h"

int main() {
    BTreeNode* root = NULL;  

    printf("=== Insertion Operations ===\n\n");

    int keys[] = {10, 5, 20, 2, 3, 12, 17, 30};
    const char *values[] = {"Charmander", "Bulbasaur", "Squirtle", "Pikachu","Snorlax", "Jigglypuff", "Mewtwo", "Dragonite"};

    for(int i=0; i<8; i++){
        insert(&root, keys[i], values[i]);
        printf("After inserting %d:\n", keys[i]);
        level_order_traversal(root);
        printf("\n");
    }


    printf("=== Search Operations ===\n");
    int search_keys[] = {6, 15, 12, 30, 5};
    int num_searches = sizeof(search_keys) / sizeof(search_keys[0]);
    
    for (int i = 0; i < num_searches; i++) {
        char* result = search(root, search_keys[i]);
        if (result)
            printf("Search %d: Found %s\n", search_keys[i], result);
        else
            printf("Search %d: Key not found\n", search_keys[i]);
    }

    printf("\n=== Final B-Tree Structure ===\n");
    level_order_traversal(root);
    printf("\n");

    free_btree(root);
    printf("\nMemory cleaned up successfully.\n");

    return 0;
}
