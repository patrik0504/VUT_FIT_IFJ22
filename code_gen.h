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
#include "expr_parser.h"
#include "error.h"

void printProlog();
void codeGenWrite(Lexeme l);
void printBuiltInFunctions();
void codeGenReads();
void codeGenReadi();
void codeGenReadf();
void createFrame();
void generateParam(int number, Lexeme *l, bool comesFromFunction);
void callFunction(char *functionName);
void defineNewVar(char *varName, bool comesFromFunction);
void declareFunction(char *functionName);
void declareParams(int number, char *varName);
void codeGenReturn(bool comesFromFunction, char *functionName);


#endif