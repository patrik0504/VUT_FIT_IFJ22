#ifndef PARSER_FILE
#define PARSER_FILE



#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <malloc.h>
#include "functions.h"
#include "symtable.h"
#include "scanner.h"
#include "expr_parser.h"
#include "error.h"
#include "code_gen.h"
#include "built_in_functions.h"

typedef enum{
    PARSER_ERROR
}ERRORS;

/**
 * Funkce pro spuštění scanneru, parsování a následně generování kódu
 */
int parse();

/**
 * Funkce na zkontrolování správného formátu prologu
 * @param l Ukazatel na aktuální token
 * @param binaryTree Binární strom obsahující keywords, potřeba pro scanner při volání get_token
 * @return Vrací 1 pokud kontrola proběhla v pořádku, 0 pokud ne
 */
int check_prolog(Lexeme *l, p_node binaryTree);

/**
 * Funkce na určení typu tokenu
 * @param l Ukazatel na aktuální token
 * @return Vrací typ tokenu jako hodnotu enumu
 */
int check_type(Lexeme *l);

/**
 * Funkce na zkontrolování typu tokenu
 * @param l Ukazatel na aktuální token
 * @return Vrací 1 pokud je typ string, type, void nebo float, 0 pokud cokoli jíného
 */
int type(Lexeme *l);

/**
 * Funkcia kontrolu že či za lexemom - je správny type lexému
 * 
 * @param l ukazatel na aktuálny lexém
 * @param binaryTree Binární strom obsahující keywords, potřeba pro scanner při volání get_token 
 * @return int Vracia 0 ak je správny type lexému, inak vracia 1
 */
int check_minus(Lexeme *l, p_node binaryTree);

/**
 * Funkce pro vrácení operátoru v rámci výrazu na stdin v případě, že výraz nemá proměnnou pro uložení (např. $b+4;)
 * 
 * @param l Lexém, který pokud je operátor, vrátí se na stdin
 * @return Vrací 1 pokud výraz byl operátor, pokud ne tak 0 a funkce hlásí chybu
 */
int returnOperator(Lexeme *l);

/**
 * Kontrola správnosti zápisu prvního parametru v deklaraci funkce
 * @param l Ukazatel na aktuální token
 * @param binaryTree Binární strom obsahující keywords, potřeba pro scanner při volání get_token
 * @param globalFunctions Binární strom obsahující data celého programu (built-in funkce, deklarované funkce, všechny proměnné apod)
 * @return Vrací 1 pokud kontrola proběhla v pořádku, 0 pokud ne
 */
int decl_param(Lexeme *l, p_node binaryTree, p_node globalFunctions);

/**
 * Kontrola správnosti zápisu druhého až posledního parametru v deklaraci funkce
 * @param l Ukazatel na aktuální token
 * @param binaryTree Binární strom obsahující keywords, potřeba pro scanner při volání get_token
 * @param globalFunctions Binární strom obsahující data celého programu (built-in funkce, deklarované funkce, všechny proměnné apod)
 * @param function_node Ukazatel na binární strom funkce
 * @return Vrací 1 pokud kontrola proběhla v pořádku, 0 pokud ne
 */
int decl_param2(Lexeme *l, p_node binaryTree, p_node globalFunctions, p_node function_node, int loadedParamCount);

/**
 * Funckia kontrolu že či je premmená definovaná
 * 
 * @param functionPtr odkaz na strom funkcie alebo globalneho stromu 
 * @param variable meno premenej
 * @return int vracia 1 keď bola nájdená a 0 keď nebola nájdená
 */
int check_if_variable_is_defined(p_node functionPtr, char * variable);

/**
 * Funkce na zkontrolování správného formátu statementu
 * @param l Ukazatel na aktuální token
 * @param binaryTree Binární strom obsahující keywords, potřeba pro scanner při volání get_token
 * @param globalFunctions Binární strom obsahující data celého programu (built-in funkce, deklarované funkce, všechny proměnné apod)
 * @param comesFromFunction True pokud byl write volán ve funkci, false pokud v hlavním těle programu
 * @param functionPtr Ukazatel na binární strom funkce
 * @return Vrací 1 pokud kontrola proběhla v pořádku, 0 pokud ne
 */
int statement(Lexeme *l, p_node binaryTree, p_node globalFunctions, bool comesFromFunction, p_node functionPtr);

/**
 * Kontrola správnosti zápisu returnu
 * @param l Ukazatel na aktuální token
 * @param binaryTree Binární strom obsahující keywords, potřeba pro scanner při volání get_token
 * @param globalFunctions Binární strom obsahující data celého programu (built-in funkce, deklarované funkce, všechny proměnné apod)
 * @param comesFromFunction True pokud byl write volán ve funkci, false pokud v hlavním těle programu
 * @param functionPtr Ukazatel na binární strom funkce
 * @return Vrací 1 pokud je oboje v pořádku, 0 pokud ne
 */
int ret_expr(Lexeme *l, p_node binaryTree, p_node globalFunctions, bool comesFromFunction, p_node functionPtr);

/**
 * Kontrola a zpracování těla funkce, ifu a whilu
 * @param l Ukazatel na aktuální token
 * @param binaryTree Binární strom obsahující keywords, potřeba pro scanner při volání get_token
 * @param globalFunctions Binární strom obsahující data celého programu (built-in funkce, deklarované funkce, všechny proměnné apod)
 * @param comesFromFunction True pokud byl write volán ve funkci, false pokud v hlavním těle programu
 * @param functionPtr Ukazatel na binární strom funkce
 * @return Vrací 1 pokud je oboje v pořádku, 0 pokud ne
 */
int st_list(Lexeme *l, p_node binaryTree, p_node globalFunctions, bool comesFromFunction, p_node functionPtr);

/**
 * Kontrola správnosti zápisu a zpracování prvního parametru při volání funkce
 * @param l Ukazatel na aktuální token
 * @param binaryTree Binární strom obsahující keywords, potřeba pro scanner při volání get_token
 * @param globalFunctions Binární strom obsahující data celého programu (built-in funkce, deklarované funkce, všechny proměnné apod)
 * @param comesFromFunction True pokud byl write volán ve funkci, false pokud v hlavním těle programu
 * @param functionPtr Ukazatel na binární strom funkce
 * @param callFunction
 * @return Vrací 1 pokud je oboje v pořádku, 0 pokud ne
 */
int param(Lexeme *l, p_node binaryTree, bool comesFromFunction, p_node functionPtr, p_node callFunction, p_node globalFunctions);

/**
 * Kontrola správnosti zápisu a zpracování druhého až posledního parametru při volání funkce
 * @param l Ukazatel na aktuální token
 * @param binaryTree Binární strom obsahující keywords, potřeba pro scanner při volání get_token
 * @param globalFunctions Binární strom obsahující data celého programu (built-in funkce, deklarované funkce, všechny proměnné apod)
 * @param comesFromFunction True pokud byl write volán ve funkci, false pokud v hlavním těle programu
 * @param functionPtr Ukazatel na binární strom funkce
 * @param callFunction
 * @param paramCount
 * @return Vrací 1 pokud je oboje v pořádku, 0 pokud ne
 */
int param2(Lexeme *l, p_node binaryTree, bool comesFromFunction, p_node functionPtr, int paramCount, p_node callFunction, p_node globalFunctions);

/**
 * Kontrola správnosti zápisu whilu
 * @param l Ukazatel na aktuální token
 * @param binaryTree Binární strom obsahující keywords, potřeba pro scanner při volání get_token
 * @param globalFunctions Binární strom obsahující data celého programu (built-in funkce, deklarované funkce, všechny proměnné apod)
 * @param comesFromFunction True pokud byl write volán ve funkci, false pokud v hlavním těle programu
 * @param functionPtr Ukazatel na binární strom funkce
 * @return Vrací 1 pokud je oboje v pořádku, 0 pokud ne
 */
int while_check(Lexeme *l, p_node binaryTree, p_node globalFunctions, bool comesFromFunction, p_node functionPtr);

/**
 * Volání kontrol pro if a while
 * @param l Ukazatel na aktuální token
 * @param binaryTree Binární strom obsahující keywords, potřeba pro scanner při volání get_token
 * @param globalFunctions Binární strom obsahující data celého programu (built-in funkce, deklarované funkce, všechny proměnné apod)
 * @param comesFromFunction True pokud byl write volán ve funkci, false pokud v hlavním těle programu
 * @param functionPtr Ukazatel na binární strom funkce
 * @return Vrací 1 pokud je oboje v pořádku, 0 pokud ne
 */
int control(Lexeme *l, p_node binaryTree, p_node globalFunctions, bool comesFromFunction, p_node functionPtr);

/**
 * Funkce na zkontrolování správného formátu deklarace funkce
 * @param l Ukazatel na aktuální token
 * @param binaryTree Binární strom obsahující keywords, potřeba pro scanner při volání get_token
 * @param globalFunctions Binární strom obsahující data celého programu (built-in funkce, deklarované funkce, všechny proměnné apod)
 * @return Vrací 1 pokud kontrola proběhla v pořádku, 0 pokud ne
 */
int function_check(Lexeme *l, p_node binaryTree, p_node globalFunctions);

/**
 * Funkce na zkontrolování správného formátu ifu
 * @param l Ukazatel na aktuální token
 * @param binaryTree Binární strom obsahující keywords, potřeba pro scanner při volání get_token
 * @param globalFunctions Binární strom obsahující data celého programu (built-in funkce, deklarované funkce, všechny proměnné apod)
 * @param comesFromFunction True pokud byl write volán ve funkci, false pokud v hlavním těle programu
 * @param functionPtr Ukazatel na binární strom funkce
 * @return Vrací 1 pokud kontrola proběhla v pořádku, 0 pokud ne
 */
int if_check(Lexeme *l, p_node binaryTree, p_node globalFunctions, bool comesFromFunction, p_node functionPtr);

/**
 * Kontroluje, zda je správně jak prolog, tak tělo programu
 * @param l Ukazatel na aktuální token
 * @param binaryTree Binární strom obsahující keywords, potřeba pro scanner při volání get_token
 * @param globalFunctions Binární strom obsahující data celého programu (built-in funkce, deklarované funkce, všechny proměnné apod)
 * @return Vrací 1 pokud je oboje v pořádku, 0 pokud ne
 */
int prog(Lexeme *l, p_node binaryTree, p_node globalFunctions);

/**
 * Funkce na kontrolu správnosti těla programu
 * @param l Ukazatel na aktuální token
 * @param binaryTree Binární strom obsahující keywords, potřeba pro scanner při volání get_token
 * @param globalFunctions Binární strom obsahující data celého programu (built-in funkce, deklarované funkce, všechny proměnné apod)
 * @return Vrací 1 pokud kontrola proběhla v pořádku, 0 pokud ne
 */
int body(Lexeme *l, p_node binaryTree, p_node globalFunctions);

/**
 * Inicializace ukazatele na token do kterého se budou ukládat hodnoty ze scanneru
 * @return Vrací ukazatel na token
 */
Lexeme * token_init();

/**
 * Kontrola správnosti zápisu prvního parametru v deklaraci funkce
 * @param token Ukazatel na token, který se ruší
 */
void token_free(Lexeme * token);

/**
 * Funkcia na vloženie novej funkcie do stromu
 * 
 * @param globalFunctions ukazovateľ na globálny strom
 * @param l ukazovateľ na aktuálny lexém v ktorom je meno funkcie
 */
void insert_function(p_node globalFunctions, Lexeme *l);

/**
 * Inicializuje data pre binárny strom
 * 
 * @return p_data 
 */
p_data data_init();

/**
 * Funkcia inicializuje data pre binárny strom
 * 
 * @param type Typ dát 
 * @return p_data ukazatel na inicializované dáta
 */
p_data data_init_type(int type);

/**
 * vytváří data pro keywords
 * 
 * @return vrací ukazatel na data keywords
 */
p_data data_init_KW();

/**
 * Funkcia mení KW type na type
 * 
 * @param l Ukazatel na aktuální token
 * @return int Type aktuálního tokenu
 */
int lexeme_type_to_type(Lexeme *l);

/**
 * Funkcia nastavuje parametre pre vstavané funkcie
 * 
 * @param binaryTree Binární strom obsahující keywords, potřeba pro scanner při volání get_token 
 */
void set_params_in_builtin_functions(p_node binaryTree);

/**
 * Funkcia inicialzuje globálny strom pre globálne telo programu
 * 
 * @return p_node Vracia ukazatel na inicializovaný strom
 */
p_node init_global_function();




#endif