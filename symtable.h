/**
 * ***********************IFJ PROJEKT 2022********************************
 * @file symtable.h
 * @author  Matěj Toul          (xtoulm00@stud.fit.vutbr.cz)
 *          Lukáš Etzler        (xetzle00@stud.fit.vutbr.cz)
 * @brief Implementace binárního vyhledávacího stromu
 * @date 2022-12-06
 * 
 * @copyright Copyright (c) 2022
*/

#ifndef SYMTABLE_FILE
#define SYMTABLE_FILE

#include <stdbool.h>
#include "functions.h"

typedef enum{
    INT,
    FLOAT,
    STRING,
    OPTIONALINT,
    OPTIONALFLOAT,
    OPTIONALSTRING,
    VOID
}st_type;



/** Struktura listu stromu */
typedef struct node
{
    char *key; ///< ID_listu
    struct data *data; ///< Data v listu
    struct node *left; ///< Následující levý list
    struct node *right; ///< Následující pravý list
} *p_node;

/** Struktura uložených dat v listu */
typedef struct data
{
    int data; ///< Placeholder data
    bool declared; ///< Zda byla proměnná deklarována
    bool defined; ///< Zda byla proměnná definována
    st_type func_type; ///< Datový typ funkce
    bool found_return; ///< Zda byl nalezen return
    int param_count;
    int return_count;
    p_node params;
    p_node elements;
} *p_data;

/** 
 * Funkce inicializující list.
 * @param data Data nového listu
 * @param key  ID nového listu (ID - musí být UNIKÁTNÍ, jinak se ztrácí data)
 */
p_node node_init (p_data data, char* key);

/**
 * Funkce na vložení nového listu do stromu.
 * @param root Kořen stromu, do kterého vkládáme
 * @param node Vkládaný list
 */
void insert_node (p_node root, p_node node);

/** 
 * Funkce na vyhledání listu dle ID.
 * @param root Existující kořen stromu
 * @param key ID hledaného listu
 */
p_node tree_search (p_node root, char* key);

/** 
 * Funkce pro zrušení existujícího stromu.
 * @param root Existující kořen stromu
 */
void tree_destroy (p_node root);

/**
 * Vytiskne na stdout prvky stromu od nejmenšího (nejvíce vlevo) po největší (nejvíce vpravo).
 * Funkce určená primárně pro debugging.
 * @param root Existující kořen stromu
*/
void debug_tree(p_node root);
/**
 * @brief Funkcia na výpočet počtu listov v strome
 * 
 * @param root Koreň stromu
 * @return int Počet listov v strome
 */
int count_tree(p_node root); 

/**
 * @brief Funkcia na průchod stromu funkcí
 * Zjišťuje, zda byly všechny funkce definované i deklarované
 * @param root Koreň stromu
 * @return int Počet listov v strome
 */
int check_func(p_node root, int *error);

#endif