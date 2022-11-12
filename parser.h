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

typedef enum{
    PARSER_ERROR
}ERRORS;

int parse();
int check_prolog(Lexeme l, p_node binaryTree);
int program(Lexeme l, p_node binaryTree, p_node globalFunctions);
int function_check(Lexeme l, p_node binaryTree, p_node globalFunctions);

#endif