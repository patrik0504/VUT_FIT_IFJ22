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
//#include "expr_parser.h"
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

void printProlog();
void codeGenWrite(Lexeme l, bool comesFromFunction);
void printBuiltInFunctions();
void codeGenReads();
void codeGenReadi();
void codeGenReadf();
void createFrame();
void generateParam(int number, Lexeme *l, bool comesFromFunction);
void callFunction(char *functionName);
void returnVariable(char *destination, bool comesFromFunction);
void defineNewVar(char *varName, bool comesFromFunction);
void declareFunction(char *functionName);
void declareParams(int number, char *varName);
void codeGenReturn(bool comesFromFunction, char *functionName);
void codeGenIfStart(int c);
void codeGenIfEnd(int c);
void codeGenIfElse(int c);
void codeGenFunctionEnd(char *functionName);
void codeGenWhileStart(int c);
void codeGenWhileEnd(int c);

/** Funkce pro generaci operací řešených v PSA
 *  @param expr_var_count Count pro generaci unikátních ID pro dočasné proměnné
 *  @param sym1 První operand operace
 *  @param sym2 Druhý operand operace 
 *  @param operation Číslo redukčního pravidla - operace mezi dvěma lexemy
 *  @param comesFromFunction Bool hodnota určující globální / lokální rámec
 *  @param context Kontext volání PSA
 *  @param jump_label Číslo ifu/whilu pro generování instrukce skoku
*/
void generate_operation(int expr_var_count, Lexeme* sym1, Lexeme* sym2, operation operation, 
    bool comesFromFunction, gen_context context, int jump_label);

/** Funkce pro výpis instrukcí pro operace řešené v PSA
 *  @param expr_var_count Count pro generaci unikátních ID pro dočasné proměnné
 *  @param sym1 První operand operace
 *  @param sym2 Druhý operand operace 
 *  @param operation Číslo redukčního pravidla - operace mezi dvěma lexemy
 *  @param comesFromFunction Bool hodnota určující globální / lokální rámec
*/
void operation_print_symbols(int expr_var_count, Lexeme* sym1, Lexeme* sym2, char* operation, bool comesFromFunction);

/** Funkce pro výpis instrukcí pro operace >=, <=, apod.
 *  @param expr_var_count Count pro generaci unikátních ID pro dočasné proměnné
 *  @param sym1 První operand operace
 *  @param sym2 Druhý operand operace 
 *  @param operation Číslo redukčního pravidla - operace mezi dvěma lexemy
 *  @param comesFromFunction Bool hodnota určující globální / lokální rámec
*/
void mixed_jump_print_symbols(int expr_var_count, Lexeme* sym1, Lexeme* sym2, char* operation, bool comesFromFunction);

/** Pomocná funkce pro výpis jednotlivých symbolů u operací
 *  @param lexeme Lexém pro korespondující symbol
 *  @param scope  Buď "LF@" nebo "GF@" v závislosti na kontextu
*/
void print_single_symbol(Lexeme* lexeme, char* scope);

/** Funkce pro uložení výsledku ze source do target
 *  @param target Název proměnné kam ukládáme
 *  @param source_var_count Počítadlo dočasných proměnných - Dá nám poslední proměnnou (poslední zredukovaný expression), ze které uložíme výsledek do targetu.
 *  @param comesFromFunction Bool hodnota určující globální / lokální rámec
*/
void expr_move(char* target, int source_var_count, bool comesFromFunction);

/** Pomocná funkce pro generování dělení
 *  @param sym1 Rozhodující symbol
 *  @param sym2 Rozhodující symbol
 *  @param comesFromFunction Bool hodnota určující globální / lokální rámec
 *  @param expr_var_count Count pro generaci unikátních ID pro dočasné proměnné
*/
void div_decider(Lexeme* sym1, Lexeme* sym2, bool comesFromFunction, int expr_var_count);

/** Pomocná funkce pro generování skoku pro if/while
 *  @param context Kontext generování
 *  @param jump_label Číslo ifu/whilu
 *  @param comesFromFunction Bool hodnota určující globální / lokální rámec
 *  @param expr_var_count Count pro generaci unikátních ID pro dočasné proměnné
 *  @param skip_on String nastavený na "true" pro operaci !==, jinak "false"
*/
void print_expr_jump(gen_context context, int jump_label, int expr_var_count, bool comesFromFunction, char* skip_on);

#endif