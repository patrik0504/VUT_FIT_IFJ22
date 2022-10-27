#include <stdio.h>
#include <stdlib.h>

#include "scanner.h"
#include "symtable.h"

void bst_test() 
{

    //TEST-DATA
    p_data data = (p_data)malloc(sizeof(struct data));
    data->data = 69;

    p_data data1 = (p_data)malloc(sizeof(struct data));
    data1->data = 420;

    p_data data2 = (p_data)malloc(sizeof(struct data));
    data2->data = 360;

    p_data data3 = (p_data)malloc(sizeof(struct data));
    data3->data = 666;

    p_data data4 = (p_data)malloc(sizeof(struct data));
    data4->data = 158;
    //TEST-DATA-END

    //default root
    p_node root = NULL;
    root = node_init(data, "a0");

    //TEST-NODE-INIT
    p_node node = node_init(data1, "a");
    insert_node(root, node);

    p_node node2 = node_init(data2, "aa");
    insert_node(root, node2);

    p_node node3 = node_init(data3, "aaa");
    insert_node(root, node3);

    p_node node4 = node_init(data4, "aaaa");
    insert_node(root, node4);
    //TEST-NODE-END

    // TEST-SEARCH
    p_node found = tree_search(root, "aaaa");
    printf("Data found prvku: %d\n", found->data->data);
    printf("Key found prvku: %s\n", found->key);
    // TEST-SEARCH-END

    debug_tree(root);
    printf("\n");
    tree_destroy(root);
}

int main() 
{
    bst_test();
}