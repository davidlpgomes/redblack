// Author: David Lucas Pereira Gomes

#ifndef __TREE__
#define __TREE__

typedef struct tree_node_t {
    struct tree_node_t* p; // Parent
    struct tree_node_t* l; // Left
    struct tree_node_t* r; // Right

    char c;                // Color, 0 for black and 1 for red
    int k;                 // Key
    void* v;               // Arbitrary value
} tree_node_t;

typedef struct tree_t {
    tree_node_t* root;
    tree_node_t* nil;
} tree_t;

tree_t* tree_create();

void tree_free(tree_t* tree);

void tree_left_rotate(tree_t* tree, tree_node_t* x);

void tree_right_rotate(tree_t* tree, tree_node_t* y);

tree_node_t* tree_insert(tree_t* tree, int key, void* value);

void tree_insert_fixup(tree_t* tree, tree_node_t* z);

void tree_transplant(tree_t* tree, tree_node_t* u, tree_node_t* v);

void tree_delete(tree_t* tree, tree_node_t* z);

void tree_delete_fixup(tree_t* tree, tree_node_t* x);

tree_node_t* tree_minimum(tree_t* tree, tree_node_t* x);

tree_node_t* tree_maximum(tree_t* tree, tree_node_t* x);

void print_tree(tree_t* tree);

#endif
