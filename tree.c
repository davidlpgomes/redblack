// Author: David Lucas Pereira Gomes

#include "tree.h"

#include <stdio.h>
#include <stdlib.h>

void test_alloc(void* p, char* name) {
    if (p) return;

    fprintf(stderr, "Error: could not alloc %s\n", name);
    exit(-1);
}

tree_t* tree_create() {
    tree_t* tree = malloc(sizeof(tree_t));
    test_alloc(tree, "tree");

    tree->nil = malloc(sizeof(tree_node_t));
    test_alloc(tree->nil, "nil");

    tree->nil->p = tree->nil;
    tree->nil->l = tree->nil;
    tree->nil->r = tree->nil;

    tree->nil->c = 0;
    tree->nil->k = 0;
    tree->nil->v = NULL;

    tree->root = tree->nil;

    return tree;
}

void tree_free_nodes(tree_t* tree, tree_node_t* node) {
    if (node == tree->nil)
        return;

    tree_free_nodes(tree, node->l);
    tree_free_nodes(tree, node->r);

    free(node);

    return;
}

void tree_free(tree_t* tree) {
    tree_free_nodes(tree, tree->root);
    
    if (tree->nil)
        free(tree->nil);

    if (tree)
        free(tree);

    return;
}


void tree_left_rotate(tree_t* tree, tree_node_t* x) {
    tree_node_t* y = x->r;
    x->r = y->l;

    if (y->l != tree->nil)
        y->l->p = x;

    y->p = x->p;

    if (x->p == tree->nil)
        tree->root = y;
    else if (x == x->p->l)
        x->p->l = y;
    else
        x->p->r = y;

    y->l = x;
    x->p = y;

    return;
}

void tree_right_rotate(tree_t* tree, tree_node_t* y) {
    tree_node_t* x = y->l;
    y->l = x->r;

    if (x->r != tree->nil)
        x->r->p = y;

    x->p = y->p;

    if (y->p == tree->nil)
        tree->root = x;
    else if (y == y->p->r)
        y->p->r = x;
    else
        y->p->l = x;

    x->r = y;
    y->p = x;

    return;
}

tree_node_t* tree_insert(tree_t* tree, int key, void* value) {
    tree_node_t* x = tree->root;
    tree_node_t* y = tree->nil;

    tree_node_t* z = malloc(sizeof(tree_node_t));
    z->k = key;
    z->v = value;

    while (x != tree->nil) {
        y = x;

        if (z->k < x->k)
            x = x->l;
        else
            x = x->r;
    }

    z->p = y;

    if (y == tree->nil)
        tree->root = z;
    else if (z->k < y->k)
        y->l = z;
    else
        y->r = z;

    z->l = tree->nil;
    z->r = tree->nil;
    z->c = 1;

    tree_insert_fixup(tree, z);

    return z;
}

void tree_insert_fixup(tree_t* tree, tree_node_t* z) {
    tree_node_t* y;

    while (z->p->c) {
        if (z->p == z->p->p->l) {
            y = z->p->p->r;

            if (y->c) {
                z->p->c = 0;
                y->c = 0;
                z->p->p->c = 1;
                z = z->p->p;
            } else {
                if (z == z->p->r) {
                    z = z->p;
                    tree_left_rotate(tree, z);
                }

                z->p->c = 0;
                z->p->p->c = 1;
                tree_right_rotate(tree, z->p->p);
            }
        } else {
            y = z->p->p->l;

            if (y->c) {
                z->p->c = 0;
                y->c = 0;
                z->p->p->c = 1;
                z = z->p->p;
            } else {
                if (z == z->p->l) {
                    z = z->p;
                    tree_right_rotate(tree, z);
                }

                z->p->c = 0;
                z->p->p->c = 1;
                tree_left_rotate(tree, z->p->p);
            }
        }
    }

    tree->root->c = 0;

    return;
}

void tree_transplant(tree_t* tree, tree_node_t* u, tree_node_t* v) {
    if (u->p == tree->nil)
        tree->root = v;
    else if (u == u->p->l)
        u->p->l = v;
    else
        u->p->r = v;

    v->p = u->p;
    
    return;
}

void tree_delete(tree_t* tree, tree_node_t* z) {
    tree_node_t* x;
    tree_node_t* y = z;
    char y_original_color = y->c;

    if (z->l == tree->nil) {
        x = z->r;
        tree_transplant(tree, z, z->r);
    } else if (z->r == tree->nil) {
        x = z->l;
        tree_transplant(tree, z, z->l);
    } else {
        y = tree_minimum(tree, z->r);
        y_original_color = y->c;
        x = y->r;

        if (y != z->r) {
            tree_transplant(tree, y, y->r);
            y->r = z->r;
            y->r->p = y;
        } else
            x->p = y;

        tree_transplant(tree, z, y);
        y->l = z->l;
        y->l->p = y;
        y->c = z->c;
    }

    if (!y_original_color)
        tree_delete_fixup(tree, x);

    free(z);

    return;
}

void tree_delete_fixup(tree_t* tree, tree_node_t* x) {
    tree_node_t* w;

    while (x != tree->root && !x->c) {
        if (x == x->p->l) {
            w = x->p->r;

            if (w->c) {
                w->c = 0;
                x->p->c = 1;

                tree_left_rotate(tree, x->p);
                w = x->p->r;
            }

            if (!w->l->c && !w->r->c) {
                w->c = 1;
                x = x->p;
            } else {
                if (!w->r->c) {
                    w->l->c = 0;
                    w->c = 1;

                    tree_right_rotate(tree, w);
                    w = x->p->r;
                }

                w->c = x->p->c;
                x->p->c = 0;
                w->r->c = 0;
                tree_left_rotate(tree, x->p);
                x = tree->root;
            }
        } else {
            w = x->p->l;

            if (w->c) {
                w->c = 0;
                x->p->c = 1;

                tree_right_rotate(tree, x->p);
                w = x->p->l;
            }

            if (!w->r->c && !w->l->c) {
                w->c = 1;
                x = x->p;
            } else {
                if (!w->l->c) {
                    w->r->c = 0;
                    w->c = 1;

                    tree_left_rotate(tree, w);
                    w = x->p->l;
                }

                w->c = x->p->c;
                x->p->c = 0;
                w->l->c = 0;
                tree_right_rotate(tree, x->p);
                x = tree->root;
            }
        }
    }

    x->c = 0;

    return;
}

tree_node_t* tree_minimum(tree_t* tree, tree_node_t* x) {
    while (x->l != tree->nil)
        x = x->l;

    return x;
}

tree_node_t* tree_maximum(tree_t* tree, tree_node_t* x) {
    while (x->r != tree->nil)
        x = x->r;

    return x;
}

void print_tree_recursive(tree_t* tree, tree_node_t* x) {
    if (x == tree->nil)
        return;

    print_tree_recursive(tree, x->l);
    printf("%d ", x->k);
    print_tree_recursive(tree, x->r);
    
    return;
}

void print_tree(tree_t* tree) {
    if (tree->root == tree->nil)
        return;

    print_tree_recursive(tree, tree->root);
    printf("\n");

    return;
}
