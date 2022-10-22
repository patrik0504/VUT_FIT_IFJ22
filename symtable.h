#ifndef SYMTABLE_FILE
#define SYMTABLE_FILE


/** Struktura uložených dat */
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
 * Funkce inicializující strom 
 * @param root Ukazatel na kořen stromu
 */
void tree_init (p_node *root);

/** 
 * Funkce na vložení nového listu do stromu
 * @param root Ukazatel na existující kořen stromu
 * @param key  ID nového listu
 * @param data Data, které se vloží do nového listu
 */
void insert_node (p_node *root, char* key, p_data data);

/** 
 * Funkce na smazání listu 
 * @param root Ukazatel na existující kořen stromu
 * @param key  ID listu, který mažeme
 */
void delete_node (p_node *root, char* key);
/** 
 * Funkce na vyhledání listu dle ID
 * @param root Ukazatel na existující kořen stromu
 * @param key ID hledaného listu
 */
p_node tree_search (p_node *root, char* key);

/** 
 * Funkce pro zrušení existujícího stromu
 * @param root Ukazatel na kořen stromu
 */
void tree_destroy (p_node *root);

#endif