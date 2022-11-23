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

#define ARRAYSIZE 5
// Dlžka escape sekvencie pre Hexa formát
#define ESCAPEHEXA 3
// Dlžka escape sekvencie pre Octa formát
#define ESCAPEOCTA 3
// Dlžka escape sekvencie pre klasický formát
#define ESCAPE 1
#define ERRORRETURN -1

/** Enum pre stavy konečného automatu*/
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

/** Štruktúra pre Lexem obsahujúca typ lexému vo forme enum a jeho data*/
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
        
        PROLOG,
        FILE_END_SIGN,
        SCANERROR
    } type;
    union {
        char* string;
        int value;
        float exponent;
        float decimal;
        int row_number;
        //int symtab_index;
    } extra_data;
    int row;
}Lexeme;

/**
 * @brief Funkcia na získanie ďalšieho lexému zo vstupu
 * Získava lexémy zo vstupu a vracia ich v tvare štruktúry Lexeme
 * @return Štuktúra Lexeme obsahujúca typ lexému a jeho data 
 */
Lexeme get_token(p_node binaryTree,int *row);
/**
 * @brief Funkcia na inicializáciu binárneho stromu pre naše kľúčové slová
 * @return Vracia ukazateľ na koreň binárneho stromu
 */
p_node init_binary_treeKW();
/**
 * @brief Pomocná funkcia na posun stringu o shift
 * Táto funkcia slúži na posun stringu doľava o dľžku shift
 * @param buffer Vstupný string
 * @param shift O koľko sa má posunúť string
 * @param stringlenght Dlžká stringu
 */
void shiftLeft(char* buffer, int shift, int stringlenght);
/**
 * @brief Pomocná funkcia prevodu typu lexemu na string
 * Funckia slúži na prevod typu lexému na string
 * @param in Štruktúra Lexeme
 * @return char* String obsahujúci názov lexému
 */
char * str_lexeme(Lexeme in);
/**
 * @brief Funkcia na skenovanie vsupu
 * Funkcia skenuje lexémy zo vstupu a vracia ich v tvare štruktúry Lexeme
 * @return Lexeme Vracia načítaný lexém
 */
Lexeme scan_lexeme();
/**
 * @brief Funkcia na generovania lexemu
 * Funkcia generuje lexém pomocou konečného automatu
 * @param state Momentálny stav konečného automatu
 * @param pole Vstupný string
 * @param stringlength Dľžka vstupného stringu
 * @return Lexeme Štruktúra Lexeme obsahujúca typ lexému a jeho data
 */
Lexeme generateLexeme(AutomatState state, char* pole, int stringlength, int row);
/**
 * @brief Funckia na zmenu stavu konečného automatu
 * Funkcia dostáva na vstup aktuálny stav konečného automatu a znak zo vstupu a pomocou neho rozhodne o násl. stave
 * @param currentState Aktuálny stav konečného automatu
 * @param c Vsutupný znak
 * @return AutomatState Ďalší stav konečného automatu
 */
AutomatState transition(AutomatState currentState, char c);
/**
 * @brief Pomocná funkcia na detekovania kľúčového slova
 * Ak je na vstupne lexému kľúčové slovo, tak sa zmení jeho typ na kľúčové slovo
 * @param root Ukazateľ na koreň binárneho stromu kľúčových slov
 * @param l Ukazateľ na vstupný lexem
 */
void check_forKW(p_node root, Lexeme* l);
/**
 * @brief Pomocná funkcia na detekovanie escape sekvencie
 * Funckia hľadá v stringu escape sekvenciu a ak ju nájde, tak ju nahradí
 * @param buffer Vsutupný string
 * @param stringlength Dľžka vstupného stringu
 * @return int Dlžka stringu po odstránení escape sekvencie
 */
int transferEscapeSequences(char* buffer, int stringlength);

#endif