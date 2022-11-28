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

#endif