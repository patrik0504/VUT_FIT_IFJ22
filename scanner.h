#ifndef SCANNER_FILE
#define SCANNER_FILE

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <malloc.h>
#include "functions.h"

#define ARRAYSIZE 5
#define ESCAPEHEXA 3
#define ESCAPEOCTA 3
#define ESCAPE 1
#define ERRORRETURN -1

typedef enum{
    Start,
    LexEOF,
    Semicolon,
    Colon,
    String,
    StringEnd,
    Number,
    Decimal,
    Exponential,
    Exponent,
    Identifier,
    Var,
    LoadVar,
    LBracket,
    RBracket,
    LBracketSKudrlinkou,
    RBracketSKudrlinkou,
    Equal,
    Equal2,
    Equal3,
    Plus,
    Minus,
    Multiply,
    Divide,
    DivideOrComment,
    Konkatenace,
    Less,
    LessEqual,
    Greater,
    GreaterEqual,
    Not,
    Not2,
    NotEqual,
    LineComment,
    Error
}AutomatState;

typedef struct {
    enum {
        NULLLEX,
        LEXEOF,
        SEMICOLON,
        COLON,
        STRING_LITERAL,
        NUMBER,
        DECIMAL_NUMBER,
        EXPONENT_NUMBER,
        FUNCTION_ID,
        VARIABLE_ID,
        LBRACKET,
        RBRACKET,
        LBRACKET_S_KUDRLINKOU,
        RBRACKET_S_KUDRLINKOU,
        EQUAL,
        EQUAL3,
        PLUS,
        MINUS,
        MULTIPLY,
        DIVIDE,
        KONKATENACE,
        LESS,
        LESSEQUAL,
        GREATER,
        GREATEREQUAL,
        NOTEQUAL,
        IF,
        WHILE
    } type;
    union {
        char* string;
        int value;
        float exponent;
        float decimal;
        //int symtab_index;
    } extra_data;
}Lexeme;

int scanner();
void shiftLeft(char* buffer, int shift, int stringlenght);
char * str_lexeme(Lexeme in);
Lexeme scan_lexeme();
Lexeme generateLexeme(AutomatState state, char* pole, int stringlength);
AutomatState transition(AutomatState currentState, char c);


#endif