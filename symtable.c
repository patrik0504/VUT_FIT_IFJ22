#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "symtable.h"

p_node node_init (p_data data, char* key) 
{
    p_node temp = (p_node)malloc(sizeof(struct node));
    if (temp == NULL) 
    {
        return NULL;
    }

    temp->data = data;
    temp->key = key;
    temp->left = NULL;
    temp->right = NULL;

    return temp;
}

void insert_node (p_node root, p_node node) 
{
    if (root == NULL) 
    { // Kořen stromu musí být inicializovaný
        fprintf(stderr, "Chyba při vkládání do stromu: Kořen stromu není validní!\n");
        return;
    }

    // Oba prvky p_node musí mít svůj klíč. Při inicializaci přes node_init() je tohle zaručeno.
    if (root->key == NULL || node->key == NULL) 
    { 
        fprintf(stderr, "\
        Chyba při vkládání do stromu: Jeden z listů nemá nastavený klíč!\n\
        Použij pro vytváření listů výhradně funcki node_init.\n");
        return;
    }

    // Je-li node->key menší než klíč kořenu, pokračujeme doleva
    if (strcmp(root->key, node->key) > 0) 
    {
        if (root->left == NULL) 
        { // Pokud kořen nemá levého potomka, zařadíme node sem
            root->left = node;
            return;
        }
        insert_node(root->left, node); // Rekurzivní volání pro levého potomka
    }
    // Je-li node->key větší než klíč kořenu, pokračujeme doprava
    else if (strcmp(root->key, node->key) < 0) 
    {
        if (root->right == NULL) 
        { // Pokud kořen nemá pravého potomka, zařadíme node sem
            root->right = node;
            return;
        }
        insert_node(root->right, node); // Rekurzivní volání pro pravého potomka
    }
}

p_node tree_search (p_node root, char* key) 
{
    // Pokud je strom prázdný, vrátíme null
    // Pokud je hledaný node přímo kořen, vrátíme kořen
    if (root == NULL || strcmp(root->key, key) == 0)
    {
        return root;
    }

    if ((strcmp(root->key, key)) < 0)
    {
        return tree_search(root->right, key); // Rekurzivní volání pro pravého potomka
    }

    return tree_search(root->left, key);  // Rekurzivní volání pro levého potomka
}


void tree_destroy (p_node root) 
{
    if (root != NULL) 
    {
        tree_destroy(root->left);  // Rekurzivní volání pro levého potomka
        tree_destroy(root->right); // Rekurzivní volání pro pravého potomka
        free(root->data);
        free(root);
    }
}

void debug_tree(p_node root) 
{
    if (root != NULL) 
    {
        debug_tree(root->left);
        printf("%s ", root->key);
        //vymazat
        printf("%d ", root->data->func_type);
        debug_tree(root->right);
    }
}