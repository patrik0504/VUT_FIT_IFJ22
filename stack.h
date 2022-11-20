#ifndef STACK_FILE
#define STACK_FILE

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "scanner.h"

/** Enum značící typ symbolu pro precedenční syntaktickou analýzu */
typedef enum 
{
    SYM_MUL,
    SYM_DIV,
    SYM_PLUS,
    SYM_MINUS,
    SYM_CONCAT,
    SYM_LESSER,
    SYM_GREATER,
    SYM_LESOREQ,
    SYM_GREOREQ,
    SYM_EQ,
    SYM_NOTEQ,
    SYM_LPAR,
    SYM_RPAR,
    SYM_ID,
    SYM_STACK_TAG,
    SYM_HANDLE_TAG,
    SYM_NONTERMINAL
} symbol_type;

static const char *symbol_type_err[] = {"*","/","+","-",".","<",">","<=",">=","===","!==","(",")","identifikátor","$","HANDLE<", "výraz"};

/** Struktura stacku */
typedef struct stack
{
    int top;                ///< Vrchol stacku
    unsigned size;          ///< Size stacku
    int lpar_count;    ///< Počet levých závorek na stacku (hodnota pro ukončení PSA)
    symbol_type *array;     ///< Stack
} * p_stack;

/**
 * Funkce inicializující stack.
 * @param size Maximální velikost stacku
 * @return Stack s maximání velikostí size
 */
p_stack stack_init(unsigned size);

/**
 * Pomocná funkce pro funkci Push - kontroluje přetečení stacku.
 * @param stack Kontrolovaný stack
 * @return 0 (false) při přetečení, jinak 1
 */
int checkOverflow(p_stack stack);

/**
 * Pomocná funkce pro funkci Pop - kontroluje podtečení stacku.
 * @param stack Kontrolovaný stack
 * @return 0 (false) při podtečení, jinak 1
 */
int checkUnderflow(p_stack stack);

/**
 * Funkce kontrolující plnost stacku.
 * @param stack Kontrolovaný stack
 * @return 1 (true) když je stack plný, jinak 0
*/
int is_full(p_stack stack);

/**
 * Funkce kontrolující prázdnost stacku.
 * @param stack Kontrolovaný stack
 * @return 1 (true) když je stack prázdný, jinak 1
*/
int is_empty(p_stack stack);

/**
 * Funkce pro vložení do stacku. Posouvá top o +1.
 * @param stack Stack do kterého se vkládá
 * @param data Vkládaný symbol
 */
void push(p_stack stack, symbol_type data);

/**
 * Funkce pro odstranění ze stacku. Posouvá top o -1. (Hodnoty prvků zůstávají stejné??)
 * @param stack Stack ze kterého se odstraňuje
 * @return Symbol z vrcholu stacku, -1 při chybě
 */
symbol_type pop(p_stack stack);

/**
 * Funkce pro nahlédnutí na prvek na vrcholu stacku bez jeho odstranění.
 * @param stack Stack jehož prvky sledujeme
 * @return Symbol z vrcholu stacku, -1 při chybě
 */
symbol_type peek(p_stack stack);

/**
 * Funkce pro smazání a dealokaci stacku.
 * @param stack Mazaný stack
 */
void stack_destroy(p_stack stack);

/**
 * Pomocná funkce pro správné posunutí při redukci
 * @param stack Stack nad kterým operaci provádíme
 */
symbol_type non_terminal_check(p_stack stack);

/**
 * Pomocná funkce pro správné vložení při redukci
 * @param stack Stack nad kterým operaci provádíme
 * @param data Vkládané data
 */
void push_after_terminal(p_stack stack, symbol_type data);



/**
 * DEBUG
 * Funkce pro vypsání stacku
 * Vlevo index 0
 * @param stack Stack nad kterým operaci provádíme
 */
void stack_print(p_stack stack);

#endif