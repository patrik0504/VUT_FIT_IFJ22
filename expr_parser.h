#ifndef EXPR_FILE
#define EXPR_FILE

#include "scanner.h"
#include "stack.h"
#include "symtable.h"
#include "error.h"
#include "code_gen.h"

#define PSA_STACK_SIZE 256
#define LEX_STACK_SIZE 256

typedef enum {
    ASSIGNMENT,
    IF,
    WHILE,
    RETURN
} context;

typedef enum {
    RR_None,       // 0: Pravidlo neexistuje
    RR_MUL,        // 1: <term>  -> <term> * <term>
    RR_DIV,        // 2: <term>  -> <term> / <term>
    RR_PLUS,       // 3: <term>  -> <term> + <term>
    RR_MINUS,      // 4: <term>  -> <term> - <term>
    RR_CONCAT,     // 5: <term>  -> <term> . <term>
    RR_LESSER,     // 6: <term>  -> <term> < <term>
    RR_GREATER,    // 7: <term>  -> <term> > <term>
    RR_LESOREQ,    // 8: <term>  -> <term> <= <term>
    RR_GREOREQ,    // 9: <term>  -> <term> >= <term>
    RR_EQ,         // 10: <term>  -> <term> === <term>
    RR_NOTEQ,      // 11: <term>  -> <term> !== <term>
    RR_PAR,        // 12: <term>  -> (<term>)
    RR_ID,         // 13: <term>  -> i
} reduction_rule;

/**
 * Funkce pro předání řízení syntaktické analýzy.
 * Slouží k vyhodnocení výrazů pomocí precedenční syntaktické analýzy.
 * @param context Kontext, ve kterém je PSA zavolána (přiřazení, volání funkce/vyhodnocení podmínky)
 * @param jump_label Číslo ifu/whilu pro generování instrukce skoku
 * @param symtable Globální tabulka symbolů
 * @param target Lexém cílové proměnné pro přiřazení (může být NULL)
 * @param varable_name Názov premmenej pri vaiable
 * @param globalFunctions obalovací seznam globálních funkcí
 * @param comesFromFunction TRUE, pokud je PSA zavolána z funkce, FALSE jinak
 * @param functionPtr ukazatel na funkci, ve které je PSA zavolána (může být NULL)
 * @return 1 (true) pokud nedošlo k chybě, jinak 0
*/
int expr(context context, int jump_label, p_node symtable, Lexeme *target, char * variable_name, 
    p_node globalFunctions, bool comesFromFunction, p_node functionPtr);

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

/** Funkce hledající vhodné pravidlo pro redukci
 *  @param op1 1. operand pravidla 
 *  @param op2 2. operand pravidla
 *  @param op3  3. operand pravidla
 *  @param stack stack nad kterým je funkce prováděna
 *  @param comesFromFunction Bool hodnota určující globální / lokální rámec
 *  @param context Kontext volání PSA
 *  @param jump_label Číslo ifu/whilu pro generování instrukce skoku
 *  @return Číslo pravidla pro redukci
*/
reduction_rule check_rule(symbol_type op1, symbol_type op2, symbol_type op3, p_stack stack, p_lex_stack lex_stack, 
    bool comesFromFunction, context context, int jump_label, p_node functionPtr, p_node globalFunctions);

/** Funkce hledající další operaci dle tabulky
    @param symtable  Tabulka symbolů
    @param stack     Předávaný stack
    @param l         Předávaný lexém    
    @param context   Předávaný kontext (jestli jde o přiřazení nebo rozhodování např. v ifu)
    @param jump_label Číslo ifu/whilu pro generování instrukce skoku
    @param comesFromFunction Bool hodnota určující globální / lokální rámec
    @param functionPtr Proměnné v lokálním rámci
    @param globalFunctions Proměnné v globálním rámci
    @return (true = 1 / false) dle úspěšnosti
*/
int check_operation (p_node symtable, p_stack stack, p_lex_stack lex_stack, Lexeme *l, context context, int jump_label,
bool comesFromFunction, p_node functionPtr, p_node globalFunctions);

#endif