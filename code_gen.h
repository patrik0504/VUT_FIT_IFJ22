#ifndef CODE_GEN_FILE
#define CODE_GEN_FILE

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <malloc.h>
#include "functions.h"
#include "symtable.h"
#include "scanner.h"
#include "error.h"
#include "built_in_functions.h"

// !! MUSÍ ZŮSTAT 1:1 S ENUMEM REDUCTION_RULE !!
typedef enum {
    CG_None,       // 0: Pravidlo neexistuje
    CG_MUL,        // 1: <term>  -> <term> * <term>
    CG_DIV,        // 2: <term>  -> <term> / <term>
    CG_PLUS,       // 3: <term>  -> <term> + <term>
    CG_MINUS,      // 4: <term>  -> <term> - <term>
    CG_CONCAT,     // 5: <term>  -> <term> . <term>
    CG_LESSER,     // 6: <term>  -> <term> < <term>
    CG_GREATER,    // 7: <term>  -> <term> > <term>
    CG_LESOREQ,    // 8: <term>  -> <term> <= <term>
    CG_GREOREQ,    // 9: <term>  -> <term> >= <term>
    CG_EQ,         // 10: <term>  -> <term> === <term>
    CG_NOTEQ,      // 11: <term>  -> <term> !== <term>
    CG_PAR,        // 12: <term>  -> (<term>)
    CG_ID,         // 13: <term>  -> i
} operation;


// !! MUSÍ ZŮSTAT 1:1 S ENUMEM CONTEXT !!
typedef enum {
    CG_ASSIGNMENT,
    CG_IF,
    CG_WHILE,
    CG_RETURN
} gen_context;


/**
 * Funkce na vytisknutí prologu v IFJcode22
 */
void printProlog();

/**
 * Funkce generující kód pro funkci write
 * Funkce vypisuje proměnné nebo konstanty
 * @param l Lexém, který se má vypsat
 * @param comesFromFunction True pokud byl write volán ve funkci, false pokud v hlavním těle programu
 */
void codeGenWrite(Lexeme l, bool comesFromFunction);

/**
 * Funkce na výpis kódu vestavěných funkcí kromě write
 * Funkce vypisuje kód pro funkce reads, readi, readf, strlen, substring, ord, floatval, intval, strval, chr
 * Vypisuje se vždy na začátku generování kódu
 */
void printBuiltInFunctions();

/**
 * Funkce na generování kódu pro zavolání funkce reads
 */
void codeGenReads();

/**
 * Funkce na generování kódu pro zavolání funkce readi
 */
void codeGenReadi();

/**
 * Funkce na generování kódu pro zavolání funkce readf
 */
void codeGenReadf();

/**
 * Funkce na generování kódu pro vytvoření rámce před voláním funkce
 */
void createFrame();

/**
 * Funkce generující kód přesun parametrů při volání funkce do dočasného rámce
 * @param number Číslo parametru
 * @param l Ukazatel na lexém, který je parametrem
 * @param comesFromFunction True pokud byl write volán ve funkci, false pokud v hlavním těle programu
 */
void generateParam(int number, Lexeme *l, bool comesFromFunction);

/**
 * Funkce generující příkaz pro zavolání funkce
 * @param functionName Název funkce
 */
void callFunction(char *functionName);

/**
 * Funkce generující kód pro přesun návratové hodnoty z dočasného rámce do lokálního/globálního
 * @param destination Název proměnné v lokálním/globálním rámci
 * @param comesFromFunction True pokud byl write volán ve funkci, false pokud v hlavním těle programu
 */
void returnVariable(char *destination, bool comesFromFunction);


/**
 * Funkce generující kód pro začátek deklarace funkce
 * @param functionName Název funkce
 */
void declareFunction(char *functionName);

/**
 * Funkcia generújuca kód pre parameter v deklarácii funkcie
 * @param number číslo parametru
 * @param varName názov parametru
 */
void declareParams(int number, char *varName);

/**
 * Funkcia pre generovanie kódu return
 */
void codeGenReturn();

/**
 * Funkce pre generování kódu pro deklaraci návratové proměnné
 */
void codeGenReturnVar();

/**
 * Funkce pro deklaraci všech proměnných funkce
 * @param functionName Název funkce
 * @param globalFunction Binární strom obsahující data celého programu (built-in funkce, deklarované funkce, všechny proměnné apod)
 * @param comesFromFunction True pokud byl write volán ve funkci, false pokud v hlavním těle programu
 */
void codeGenDeclareVars(char *func_name, p_node globalFunctions, bool comesFromFunction);

void declare_variables(p_node root, bool comesFromFunctions);

/**
 * Funckia pre generovanie začiatku ifu
 * 
 * @param c číslo ifu
 */
void codeGenIfStart(int c);

/**
 * Funckia pre generovanie konca ifu
 * 
 * @param c číslo ifu
 */
void codeGenIfEnd(int c);

/**
 * Funckia pre generovanie else
 * 
 * @param c číslo ifu
 */
void codeGenIfElse(int c);

/**
 * Funkcia pre generovanie koncu funkcie
 * 
 * @param functionName názov funkcie
 * @param globalFunction binárny strom obsahujúci dáta celého programu (built-in funkcie, deklarované funkcie, všetky premenne atď)
 */
void codeGenFunctionEnd(char *functionName, p_node globalFunctions);

/**
 * Funkcia pre generovania kódu pre začiatok while cyklu
 * 
 * @param c číslo whilu
 */
void codeGenWhileStart(int c);

/**
 * Funkce generující kód pro while
 * 
 * @param c číslo whilu
 */
void codeGenWhileEnd(int c);

/** Funkce pro generaci operací řešených v PSA
 *  @param expr_var_count Count pro generaci unikátních ID pro dočasné proměnné
 *  @param sym1 První operand operace
 *  @param sym2 Druhý operand operace 
 *  @param operation Číslo redukčního pravidla - operace mezi dvěma lexemy
 *  @param comesFromFunction Bool hodnota určující globální / lokální rámec
 *  @param context Kontext volání PSA
 *  @param jump_label Číslo ifu/whilu pro generování instrukce skoku
 *  @param functionPtr Strom funkce
 *  @param globalFunctions Globální strom
*/

void generate_operation(int expr_var_count, Lexeme* sym1, Lexeme* sym2, operation operation, 
    bool comesFromFunction, gen_context context, int jump_label, p_node functionPtr, p_node globalFunctions);

void float_conversion(Lexeme *sym, char* scope, int expr_var_count, int helper_var_count, bool comesFromFunction);

void type_checked_operation(int expr_var_count, Lexeme* sym1, Lexeme* sym2, char* operation, bool comesFromFunction,
    p_node functionPtr, p_node globalFunctions);

/** Funkce pro výpis instrukcí pro operace řešené v PSA
 *  @param expr_var_count Count pro generaci unikátních ID pro dočasné proměnné
 *  @param sym1 První operand operace
 *  @param sym2 Druhý operand operace 
 *  @param operation Číslo redukčního pravidla - operace mezi dvěma lexemy
 *  @param comesFromFunction Bool hodnota určující globální / lokální rámec
 *  @param functionPtr Strom funkce
 *  @param globalFunctions Globální strom
*/
void operation_print_symbols(int expr_var_count, Lexeme* sym1, Lexeme* sym2, char* operation, bool comesFromFunction,
    p_node functionPtr, p_node globalFunctions, bool float_conversion);

void define_comp_var(int expr_var_count, bool comesFromFunction, p_node functionPtr, p_node globalFunctions);

void define_comp_var_with_helper(int expr_var_count, bool comesFromFunction, p_node functionPtr, p_node globalFunctions);

void define_vars_for_typecheck(int expr_var_count, bool comesFromFunction, p_node functionPtr, p_node globalFunctions);

/** Funkce pro výpis instrukcí pro operace >=, <=, apod.
 *  @param expr_var_count Count pro generaci unikátních ID pro dočasné proměnné
 *  @param sym1 První operand operace
 *  @param sym2 Druhý operand operace 
 *  @param operation Číslo redukčního pravidla - operace mezi dvěma lexemy
 *  @param comesFromFunction Bool hodnota určující globální / lokální rámec
 *  @param functionPtr Strom funkce
 *  @param globalFunctions Globální strom
*/
void mixed_jump_print_symbols(int expr_var_count, Lexeme* sym1, Lexeme* sym2, char* operation, 
    bool comesFromFunction, p_node functionPtr, p_node globalFunctions);

/**
 * Funkce kontrolující kompatibilitu datových typů.
 * @param sym1 Lexém prvního symbolu
 * @param sym2 Lexém druhého symbolu
 * @return 2 pokud je třeba udělat kontrolu proměnných, 1 pokud jsou symboly kompatibilní, 
 *         0 pokud je možná float konverze, -1 při chybě
*/
int type_check(Lexeme *sym1, Lexeme *sym2);

/** Pomocná funkce pro výpis jednotlivých symbolů u operací
 *  @param lexeme Lexém pro korespondující symbol
 *  @param scope  Buď "LF@" nebo "GF@" v závislosti na kontextu
*/
void print_single_symbol(Lexeme* lexeme, char* scope, bool is_helper, int helper_var_count);

/** Funkce pro uložení výsledku ze source do target
 *  @param target Název proměnné kam ukládáme
 *  @param source_var_count Počítadlo dočasných proměnných - Dá nám poslední proměnnou (poslední zredukovaný expression), ze které uložíme výsledek do targetu.
 *  @param comesFromFunction Bool hodnota určující globální / lokální rámec
*/
void expr_move(char* target, int source_var_count, bool comesFromFunction);

/** Pomocná funkce pro generování skoku pro if/while
 *  @param context Kontext generování
 *  @param jump_label Číslo ifu/whilu
 *  @param comesFromFunction Bool hodnota určující globální / lokální rámec
 *  @param expr_var_count Count pro generaci unikátních ID pro dočasné proměnné
 *  @param skip_on String nastavený na "true" pro operaci !==, jinak "false"
*/
void print_expr_jump(gen_context context, int jump_label, int expr_var_count, bool comesFromFunction, char* skip_on);

#endif