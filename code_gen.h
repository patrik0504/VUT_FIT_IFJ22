/**
 * ***********************IFJ PROJEKT 2022********************************
 * @file code_gen.h
 * @author  Veronika Nevařilová (xnevar00@stud.fit.vutbr.cz)
 *          Patrik Michlian     (xmichl12@stud.fit.vutbr.cz)
 *          Matěj Toul          (xtoulm00@stud.fit.vutbr.cz)
 *          Lukáš Etzler        (xetzle00@stud.fit.vutbr.cz)
 * @brief Generování kódu jazyka IFJcode22
 * @date 2022-12-06
 * 
 * @copyright Copyright (c) 2022
*/

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
 * Funkce generující kód pro kontrolu správného typu návratové hodnoty
 * @param type Typ návratové hodnoty funkce
 * @param comesFromFunction True pokud byl write volán ve funkci, false pokud v hlavním těle programu
 * @param functionName Název funkce, ve které byl nalezen return
 * @param number Pořadí returnu ve funkci, potřeba odlišovat kvůli opakové definici návěští
 */
void checkReturnType(int type, bool comesFromFunction, char* functionName, int number);

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
 * Funkce generující kód pro parametr v deklaraci funkce
 * @param number Číslo parametru
 * @param varName Název parametru
 * @param type Datový typ parametru
 * @param functionName Název funkce, ve které probíhá deklarace parametrů
 */
void declareParams(int number, char *varName, int type, char*functionName);

/**
 * Funkce pro generování kódu pro skok na ukončení programu z hlavního těla programu
 */
void codeGenReturnMain();

/**
 * Funkce pro generování kódu return
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
 * Funkce pro generování začátku ifu
 * 
 * @param c Pořadí ifu
 */
void codeGenIfStart(int c);

/**
 * Funkce pro generování konce ifu
 * 
 * @param c Pořadí ifu
 */
void codeGenIfEnd(int c);

/**
 * Funkce pro generování začátku else
 * 
 * @param c Pořadí ifu
 */
void codeGenIfElse(int c);

/**
 * Funkce pro generování konce funkce
 * 
 * @param functionName Název funkce
 * @param globalFunction Binární strom obsahující data celého programu (built-in funkce, deklarované funkce, všechny proměnné atd)
 */
void codeGenFunctionEnd(char *functionName, p_node globalFunctions);

/**
 * Funkce pro generování kódu pro začátek while cyklu
 * 
 * @param c Pořadí whilu
 */
void codeGenWhileStart(int c);

/**
 * Funkce generující kód pro konec while cyklu
 * 
 * @param c Pořadí whilu
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

/** Funkce vyhodnocující pravdivostní hodnotu podmínky, která neobsahuje relační operátory
 *  @param last Poslední lexém z PSA, tedy vstup, podle kterého se podmínka vyhodnocuje
 *  @param comesFromFunction Bool hodnota určující globální / lokální rámec
 *  @param expr_var_count Count pro generaci unikátních ID pro dočasné proměnné
*/
void arithmetic_if_while(Lexeme* last, bool comesFromFunction, int expr_var_count);

/** Funkce pro float konverzi
 *  @param sym Konvertovaný symbol
 *  @param scope  Buď "LF@" nebo "GF@" v závislosti na local/global framu
 *  @param expr_var_count Count pro generaci unikátních ID pro dočasné proměnné
 *  @param helper_var_count Counter pomocných proměnných
 *  @param comesFromFunction Bool hodnota určující globální / lokální rámec
*/
void float_conversion(Lexeme *sym, char* scope, int expr_var_count, int helper_var_count, bool comesFromFunction);

/** Funkce pro naplnění typů pro sym1, sym2
 *  @param sym1 První operand operace
 *  @param sym2 Druhý operand operace 
 *  @param scope  Buď "LF@" nebo "GF@" v závislosti na local/global framu
 *  @param expr_var_count Count pro generaci unikátních ID pro dočasné proměnné
*/
void fill_in_type_vars(Lexeme* sym1, Lexeme* sym2, char* scope, int expr_var_count);

/** Funkce pro generaci konkatenace
 *  @param expr_var_count Count pro generaci unikátních ID pro dočasné proměnné
 *  @param sym1 První operand operace
 *  @param sym2 Druhý operand operace 
 *  @param comesFromFunction Bool hodnota určující globální / lokální rámec
 *  @param functionPtr Strom funkce
 *  @param globalFunctions Globální strom
*/
void generate_concat(int expr_var_count, Lexeme* sym1, Lexeme* sym2, bool comesFromFunction,
    p_node functionPtr, p_node globalFunctions);

/** Funkce pro generace operace se zkontrolovanými typy
 *  @param expr_var_count Count pro generaci unikátních ID pro dočasné proměnné
 *  @param sym1 První operand operace
 *  @param sym2 Druhý operand operace
 *  @param operation  Číslo redukčního pravidla - operace mezi dvěma lexemy
 *  @param comesFromFunction  Bool hodnota určující globální / lokální rámec
 *  @param functionPtr  Strom funkce
 *  @param globalFunctions  Globální strom
*/
void type_checked_operation(int expr_var_count, Lexeme* sym1, Lexeme* sym2, char* operation, bool comesFromFunction,
    p_node functionPtr, p_node globalFunctions);

/** Funkce pro generaci relační operace
 *  @param expr_var_count Count pro generaci unikátních ID pro dočasné proměnné
 *  @param sym1 První operand operace
 *  @param sym2 Druhý operand operace
 *  @param operation  Číslo redukčního pravidla - operace mezi dvěma lexemy
 *  @param comesFromFunction  Bool hodnota určující globální / lokální rámec
 *  @param functionPtr  Strom funkce
 *  @param globalFunctions  Globální strom
*/
void relation_operation(int expr_var_count, Lexeme* sym1, Lexeme* sym2, char* operation, bool comesFromFunction,
    p_node functionPtr, p_node globalFunctions);

/** Funkce pro výpis instrukcí pro operace řešené v PSA
 *  @param expr_var_count Count pro generaci unikátních ID pro dočasné proměnné
 *  @param sym1 První operand operace
 *  @param sym2 Druhý operand operace 
 *  @param operation Číslo redukčního pravidla - operace mezi dvěma lexemy
 *  @param comesFromFunction Bool hodnota určující globální / lokální rámec
 *  @param functionPtr Strom funkce
 *  @param globalFunctions Globální strom
 *  @param float_conversion Flag značící potřebu float konverze
*/
void operation_print_symbols(int expr_var_count, Lexeme* sym1, Lexeme* sym2, char* operation, bool comesFromFunction,
    p_node functionPtr, p_node globalFunctions, bool float_conversion);

/** Pomocná funkce pro definici compiler proměnné
 *  @param expr_var_count Count pro generaci unikátních ID pro dočasné proměnné
 *  @param comesFromFunction Bool hodnota určující globální / lokální rámec
 *  @param functionPtr Strom funkce
 *  @param globalFunctions Globální strom
*/
void define_comp_var(int expr_var_count, bool comesFromFunction, p_node functionPtr, p_node globalFunctions);

/** Funkce pro generaci compiler proměnné a dvou (až tří) pomocných
 *  @param expr_var_count Count pro generaci unikátních ID pro dočasné proměnné
 *  @param comesFromFunction  Bool hodnota určující globální / lokální rámec
 *  @param functionPtr  Strom funkce
 *  @param globalFunctions  Globální strom
*/
void define_comp_var_with_helper(int expr_var_count, bool comesFromFunction, p_node functionPtr, p_node globalFunctions);

/** Funkce pro nadefinování proměných pro type_check
 *  @param expr_var_count Count pro generaci unikátních ID pro dočasné proměnné
 *  @param comesFromFunction  Bool hodnota určující globální / lokální rámec
 *  @param functionPtr  Strom funkce
 *  @param globalFunctions  Globální strom
*/
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
 *  @param arithmetic Flag značící skok u aritmetické podmínky
*/
void print_expr_jump(gen_context context, int jump_label, int expr_var_count, bool comesFromFunction, bool arithemtic);

#endif