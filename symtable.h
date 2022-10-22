#ifndef SYMTABLE_FILE
#define SYMTABLE_FILE

/** Struktura uložených dat v listu */
typedef struct data{
    int data; ///< Placeholder data
} *p_data;

/** Struktura listu stromu */
typedef struct node{
    char *key; ///< ID_listu
    struct data *data; ///< Data v listu
    struct node *left; ///< Následující levý list
    struct node *right; ///< Následující pravý list
} *p_node;

/** 
 * Funkce inicializující list.
 * @param data Data nového listu
 * @param key  ID nového listu
 */
p_node node_init (p_data data, char* key);

/** 
 * Funkce na vložení nového listu do stromu.
 * @param root Kořen stromu, do kterého vkládáme
 * @param node Vkládaný list
 */
void insert_node (p_node root, p_node node);

/** 
 * Funkce na smazání listu.
 * @param root Existující kořen stromu
 * @param key  ID listu, který mažeme
 */
void delete_node (p_node *root, char* key);

/** 
 * Funkce na vyhledání listu dle ID.
 * @param root Existující kořen stromu
 * @param key ID hledaného listu
 */
p_node tree_search (p_node *root, char* key);

/** 
 * Funkce pro zrušení existujícího stromu.
 * @param root Existující kořen stromu
 */
void tree_destroy (p_node *root);

/**
 * Vytiskne na stdout prvky stromu od nejmenšího (nejvíce vlevo) po největší (nejvíce vpravo).
 * Funkce určená primárně pro debugging.
 * @param root Existující kořen stromu
*/
void debug_tree(p_node root);

#endif