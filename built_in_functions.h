#ifndef BUILT_IN_FUNCTIONS_FILE
#define BUILT_IN_FUNCTIONS_FILE


#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <malloc.h>
#include "parser.h"

 
int writeString(Lexeme *l, p_node binaryTree, p_node globalFunctions, bool comesFromFunction, p_node functionPtr);
int writeString2(Lexeme *l, p_node binaryTree, p_node globalFunctions, bool comesFromFunction, p_node functionPtr);
bool declaredCheck(p_node binaryTree, p_node globalFunctions, bool comesFromFunction, p_node functionPtr, Lexeme l);
void shiftRight(char* buffer, int shift, int stringlength);
void evaluateEscapeSequencies(Lexeme *l);
void replaceEscapeSequenceByNumber(char *buffer, char c);
int builtInReads(Lexeme *l, p_node binaryTree, p_node globalFunctions, bool comesFromFunction, p_node functionPtr);
int builtInReadi(Lexeme *l, p_node binaryTree, p_node globalFunctions, bool comesFromFunction, p_node functionPtr);
int builtInReadf(Lexeme *l, p_node binaryTree, p_node globalFunctions, bool comesFromFunction, p_node functionPtr);
int builtInOrd(Lexeme *l, p_node binaryTree, p_node globalFunctions, bool comesFromFunction, p_node functionPtr);
int builtInChr(Lexeme *l, p_node binaryTree, p_node globalFunctions, bool comesFromFunction, p_node functionPtr);
int builtInFloatval(Lexeme *l, p_node binaryTree, p_node globalFunctions, bool comesFromFunction, p_node functionPtr);
int builtInIntval(Lexeme *l, p_node binaryTree, p_node globalFunctions, bool comesFromFunction, p_node functionPtr);
int builtInStrval(Lexeme *l, p_node binaryTree, p_node globalFunctions, bool comesFromFunction, p_node functionPtr);
int builtInStrlen(Lexeme *l, p_node binaryTree, p_node globalFunctions, bool comesFromFunction, p_node functionPtr);
int builtInSubstring(Lexeme *l, p_node binaryTree, p_node globalFunctions, bool comesFromFunction, p_node functionPtr);

#endif