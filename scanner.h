/**
 * ***********************IFJ PROJEKT 2022********************************
 * @file scanner.h
 * @author  Veronika Nevařilová (xnevar00@stud.fit.vutbr.cz)
 *          Patrik Michlian     (xmichl12@stud.fit.vutbr.cz)
 * @brief Lexikální analýza
 * @date 2022-12-06
 * 
 * @copyright Copyright (c) 2022
*/


#ifndef SCANNER_FILE
#define SCANNER_FILE

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <malloc.h>
#include "functions.h"
#include "symtable.h"
#include "error.h"

#define ARRAYSIZE 5
// Délka escape sekvence pro Hexa formát
#define ESCAPEHEXA 3
// Délka escape sekvence pro Octa formát
#define ESCAPEOCTA 3
// Délka escape sekvence pro klasický formát
#define ESCAPE 1
#define ERRORRETURN -1

/** Výčtový typ pro stavy konečného automatu*/
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
    Comma,
    Konkatenace,
    Less,
    LessEqual,
    Greater,
    GreaterEqual,
    Not,
    Not2,
    NotEqual,
    LineComment,
    BlockComment,
    BlockCommentPotentialEnd,
    BlockCommentEnd,
    Prolog,
    FileEndSign,
    Error,
    ScanError
}AutomatState;

//struktura pro lexém obsahující typ lexému ve formě výčtového typu a jeho data
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
        COMMA,
        KONKATENACE,
        LESS,
        LESSEQUAL,
        GREATER,
        GREATEREQUAL,
        NOTEQUAL,
        
        KW_IF,
        KW_WHILE,
        KW_ELSE,
        KW_NULL,
        KW_RETURN,
        KW_VOID,
        KW_INT,
        KW_FLOAT,
        KW_STRING,
        KW_FUNCTION,
        KW_OPTIONALINT,
        KW_OPTIONALFLOAT,
        KW_OPTIONALSTRING,
        
        PROLOG,
        FILE_END_SIGN,
        SCANERROR,
        EXPR,
    } type;
    union {
        char* string;
        int value;
        float exponent;
        double decimal;
        int row_number;
    } extra_data;
    int row;
    bool negative_num;
}Lexeme;

/**
 * @brief Funkce pro získání dalšího lexému ze vstupu
 * Získává lexémy ze vstupu a vrací je v tvaru struktury Lexeme
 * @return Struktura Lexeme obsahující typ lexému a jeho data 
 */
Lexeme get_token(p_node binaryTree);
/**
 * @brief Funkce na inicializaci binárního stromu pro klíčové slova
 * @return Vrací ukazatel na kořen binárního stromu
 */
p_node init_binary_treeKW();
/**
 * @brief Pomocná funkce na posun stringu o shift
 * Tato funkce slouží na posun stringu doleva o délku shift
 * @param buffer Vstupní string
 * @param shift O kolik sa má posunout string
 * @param stringlenght Délka stringu
 */
void shiftLeft(char* buffer, int shift, int stringlenght);
/**
 * @brief Pomocná funkce převodu typu lexemu na string
 * Funkce slouží na převod typu lexému na string
 * @param in Struktura Lexeme
 * @return char* String obsahující název lexému
 */
char * str_lexeme(Lexeme in);
/**
 * @brief Funkce na skenování vstupu
 * Funkce skenuje lexémy ze vstupu a vrací je ve tvaru struktury Lexeme
 * @return Lexeme Vrací načtený lexém
 */
Lexeme scan_lexeme(int *row, bool epilog, bool prolog);
/**
 * @brief Funkce na generování lexému
 * Funkce generuje lexém pomocí konečného automatu
 * @param state Momentální stav konečného automatu
 * @param pole Vstupní string
 * @param stringlength Délka vstupního stringu
 * @return Lexeme Struktura Lexeme obsahující typ lexému a jeho data
 */
Lexeme generateLexeme(AutomatState state, char* pole, int stringlength, int row);
/**
 * @brief Funkce na změnu stavu konečného automatu
 * Funkce dostává na vstup aktuální stav konečného automatu a znak ze vstupu a pomocí něho rozhodne o násl. stavu
 * @param currentState Aktuální stav konečného automatu
 * @param c Vstupní znak
 * @return AutomatState další stav konečného automatu
 */
AutomatState transition(AutomatState currentState, char c);
/**
 * @brief Pomocná funkce na detekci klíčového slova
 * Pokud je na vstupu lexému kľíčové slovo, tak se změní jeho typ na klíčové slovo
 * @param root Ukazatel na kořen binárního stromu klíčových slov
 * @param l Ukazatel na vstupní lexem
 */
void check_forKW(p_node root, Lexeme* l);
/**
 * @brief Pomocná funkce na detekci escape sekvence
 * Funkce hledá v stringu escape sekvenci a pokud ji najde, tak ji nahradí
 * @param buffer Vstupní string
 * @param stringlength Délka vstupního stringu
 * @return int Délka stringu po odstranění escape sekvence
 */
int transferEscapeSequences(char* buffer, int stringlength);

#endif