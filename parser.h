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
int check_prolog(Lexeme *l, p_node binaryTree);
int program(Lexeme *l, p_node binaryTree, p_node globalFunctions);
int function_check(Lexeme *l, p_node binaryTree, p_node globalFunctions);
int if_check(Lexeme *l, p_node binaryTree);
int prog(Lexeme *l, p_node binaryTree, p_node globalFunctions);
int body(Lexeme *l, p_node binaryTree, p_node globalFunctions);
int decl_param(Lexeme *l, p_node binaryTree, p_node globalFunctions);
int decl_param2(Lexeme *l, p_node binaryTree, p_node globalFunctions);
Lexeme * token_init();
void token_free(Lexeme * token);
int param(Lexeme *l, p_node binaryTree);
int param2(Lexeme *l, p_node binaryTree);
int st_list(Lexeme *l, p_node binaryTree);
int while_check(Lexeme *l, p_node binaryTree);
int ret_expr(Lexeme *l, p_node binaryTree);
int control(Lexeme *l, p_node binaryTree);


#endif