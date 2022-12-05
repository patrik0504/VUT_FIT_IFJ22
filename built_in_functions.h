#ifndef BUILT_IN_FUNCTIONS_FILE
#define BUILT_IN_FUNCTIONS_FILE


#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <malloc.h>
#include "parser.h"

 /**
 * Funkce kontrolující a připravujiící první parametr ve volání write a zároveň volá codeGenWrite
 * @param l Ukazatel na token, který se má vypsat
 * @param binaryTree Binární strom obsahující keywords, potřeba pro scanner při volání get_token
 * @param globalFunctions Binární strom obsahující data celého programu (built-in funkce, deklarované funkce, všechny proměnné apod)
 * @param comesFromFunction True pokud byl write volán ve funkci, false pokud v hlavním těle programu
 * @param functionPtr Ukazatel na binární strom funkce
 * @return Vrací 1, pokud kontrola a výpis proběhly v pořádku, 0, pokud ne 
 */
int writeString(Lexeme *l, p_node binaryTree, p_node globalFunctions, bool comesFromFunction, p_node functionPtr);

/**
 * Funkce kontrolující a připravujiící druhý až poslední parametr ve volání write a zároveň volá codeGenWrite
 * @param l Ukazatel na token, který se má vypsat
 * @param binaryTree Binární strom obsahující keywords, potřeba pro scanner při volání get_token
 * @param globalFunctions Binární strom obsahující data celého programu (built-in funkce, deklarované funkce, všechny proměnné apod)
 * @param comesFromFunction True pokud byl write volán ve funkci, false pokud v hlavním těle programu
 * @param functionPtr Ukazatel na binární strom funkce
 * @return Vrací 1, pokud kontrola a výpis proběhly v pořádku, 0, pokud ne 
 */
int writeString2(Lexeme *l, p_node binaryTree, p_node globalFunctions, bool comesFromFunction, p_node functionPtr);

/**
 * Funkce pro posun řetězce doprava
 * @param buffer Řetězec 
 * @param shift Počet indexů, o které se má řetězec posunout
 * @param stringlength Délka řetězce
 */
void shiftRight(char* buffer, int shift, int stringlength);

/**
 * Funkce převádějící znak na escape sekvenci pro IFJcode22
 * @param buffer Řetězec, ve kterém se má převod provést
 * @param c Znak, který se má převést na escape sekvenci
 */
void replaceEscapeSequenceByNumber(char *buffer, char c);

/**
 * Funkce vyhledávající znaky, které mají být převedeny na escape sekvence pro IFJcode22
 * @param l Lexém, ve kterém se má převod provést
 */
void evaluateEscapeSequencies(Lexeme *l);


#endif