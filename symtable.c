#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "symtable.h"

p_node node_init (p_data data, char* key) {
    p_node temp = (p_node)malloc(sizeof(struct node));
    if (temp == NULL) { // Při chybě alokace vracíme NULL
        return NULL;
    }

    temp->data = data;
    temp->key = key;
    temp->left = NULL;
    temp->right = NULL;

    return temp;
}

void insert_node (p_node root, p_node node) {

    if (strcmp(root->key, node->key) > 0) {
        if (root->left == NULL) {
            root->left = node;
            return;
        }
        insert_node(root->left, node);
    }
    else if (strcmp(root->key, node->key) < 0) {
        if (root->right == NULL) {
            root->right = node;
            return;
        }
        insert_node(root->right, node);
    }
}

void delete_node (p_node *root, char* key) {

}

p_node tree_search (p_node *root, char* key) {

}

void tree_destroy (p_node *root) {
    
}

void debug_tree(p_node root) {
    if (root != NULL) {
        debug_tree(root->left);
        printf("%s ", root->key);
        debug_tree(root->right);
    }
}