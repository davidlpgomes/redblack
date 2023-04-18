#include <stdio.h>
#include <stdlib.h>

#include "tree.h"

int main() {
    tree_t* tree = tree_create();

    tree_node_t* p1 = tree_insert(tree, 0, NULL);
    print_tree(tree);

    tree_node_t* p2 = tree_insert(tree, 10, NULL);
    print_tree(tree);

    tree_node_t* p3 = tree_insert(tree, 5, NULL);
    print_tree(tree);
    
    tree_insert(tree, 8, NULL);
    print_tree(tree);

    tree_insert(tree, 2, NULL);
    print_tree(tree);

    tree_delete(tree, p3);
    print_tree(tree);

    tree_delete(tree, p1);
    print_tree(tree);

    tree_delete(tree, p2);
    print_tree(tree);

    tree_free(tree);

    return 0;
}
