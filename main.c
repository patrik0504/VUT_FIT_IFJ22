#include <stdio.h>
#include <stdlib.h>

#include "scanner.h"
#include "symtable.h"

void bst_test() {
    //default root
    p_node root = NULL;

    p_data data = (p_data)malloc(sizeof(struct data));
    data->data = 69;

    root = node_init(data, "aaaa");

    p_node node = node_init(data, "a");
    insert_node(root, node);

    p_node node2 = node_init(data, "aaaaa");
    insert_node(root, node2);

    p_node node3 = node_init(data, "dfkgnjsdfn");
    insert_node(root, node3);

    p_node node4 = node_init(data, "aa");
    insert_node(root, node4);

    debug_tree(root);
    printf("\n");
}

int main() {
    bst_test();
}