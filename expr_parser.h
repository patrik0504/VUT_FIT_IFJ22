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

/**
 * Funkce převádějící lexém na typ precedenční syntaktické analýzy.
 * @param lexeme Vstupní lexém
 * @return Odpovídající typ PSA pro typ lexému, případně -1 a chybu je-li na vstupu nekompatibilní typ.
*/
symbol_type lex_type_to_psa(Lexeme *lexeme);

/** Funkce pro vyhledání v precedenční tabulce (viz. dokumentace).
 *  @param stack_symbol Symbol aktuálně na vrcholu zásobníku
 *  @param input Aktuální symbol na vstupu
 *  @return  2: '>'
 *  @return  1: '='
 *  @return  0: '<'
 *  @return -1: chybový stav
*/
int precedence_lookup(symbol_type stack_symbol, symbol_type input);

#endif