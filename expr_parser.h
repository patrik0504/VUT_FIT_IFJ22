#ifndef EXPR_FILE
#define EXPR_FILE

#include "scanner.h"
#include "stack.h"
#include "symtable.h"

#define PSA_STACK_SIZE 256

typedef enum {
    ASSIGNMENT,
    CALL_CONTROL
} context;

/**
 * Funkce pro předání řízení syntaktické analýzy.
 * Slouží k vyhodnocení výrazů pomocí precedenční syntaktické analýzy.
 * @param context Kontext, ve kterém je PSA zavolána (přiřazení, volání funkce/vyhodnocení podmínky)
 * @param symtable Globální tabulka symbolů
 * @param target Lexém cílové proměnné pro přiřazení (může být NULL)
 * @return 1 (true) pokud nedošlo k chybě, jinak 0
*/
int expr(context context, p_node symtable, Lexeme *target);

/**
 * Funkce v závislosti na kontextu určí, zda má být PSA ukončena.
 * @param context Kontext, ve kterém je PSA zavolána (přiřazení, volání funkce/vyhodnocení podmínky)
 * @param lexeme Ukazatel na aktuální lexém
 * @param stack Ukazatel na stack PSA
 * @return 1 (true) pokud by PSA měla skončit, jinak 0
*/
int should_end(context context, Lexeme *lexeme, p_stack stack);

#endif