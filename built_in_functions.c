/**
 * ***********************IFJ PROJEKT 2022********************************
 * @file built_in_functions.c
 * @author Veronika Nevařilová (xnevar00@stud.fit.vutbr.cz)
 * @brief Pomocné funkce pro kontrolu a generování kódu vestavěných funkcí
 * @date 2022-12-06
 * 
 * @copyright Copyright (c) 2022
*/

#include "built_in_functions.h"


int writeString(Lexeme *l, p_node binaryTree, p_node globalFunctions, bool comesFromFunction, p_node functionPtr)
{
    bool ok = true;
    int result = 0;
    *l = get_token(binaryTree);
    if (l->type == LBRACKET)
    {
        *l = get_token(binaryTree);
        if (l->type == VARIABLE_ID)
        {
            int oke = 0;
            if(comesFromFunction)
            {
                oke = check_if_variable_is_defined(functionPtr, l->extra_data.string);
            } else
            {
                oke = check_if_variable_is_defined(globalFunctions, l->extra_data.string);
            }
            if(!oke)
            {
                error(l->row, "Proměnná ve volání WRITE nebyla deklarovaná", SEM_UNDEFINED_VAR_ERROR);
                ok = false;
            }
        }  else if (l->type == MINUS)
        {
            *l = get_token(binaryTree);
            switch (l->type)
            {
                case NUMBER:
                    l->extra_data.value = -(l->extra_data.value);
                    break;
                case DECIMAL_NUMBER:
                    l->extra_data.decimal = -(l->extra_data.decimal);
                    break;
                case EXPONENT_NUMBER:
                    l->extra_data.exponent = -(l->extra_data.exponent);
                    break;
                default:
                    ok = false;
                    error(l->row, "Neočekávaný znak ve volání WRITE", SYNTAX_ERROR);
            }
        }else if (l->type == STRING_LITERAL)
        {
            evaluateEscapeSequencies(l);
        } else if(l->type == NUMBER || l->type == DECIMAL_NUMBER || l->type == EXPONENT_NUMBER)
        {
            ok = true;
        } else if(l->type == RBRACKET)
        {
            *l = get_token(binaryTree);
            if(l->type == SEMICOLON)
            {
                return 1;
            }
            ok = false;
            error(l->row, "Neočekávaný znak ve volání WRITE", SYNTAX_ERROR);
        } else
        {
            ok = false;
            error(l->row, "Neočekávaný znak ve volání WRITE", SYNTAX_ERROR);
        }
        if (ok)
        {
            codeGenWrite(*l, comesFromFunction);
            result = writeString2(l, binaryTree, globalFunctions, comesFromFunction, functionPtr);
            if (get_token(binaryTree).type != SEMICOLON)
            {
                result = 0;
            }
        }
    }
    return result;   
}

int writeString2(Lexeme *l, p_node binaryTree, p_node globalFunctions, bool comesFromFunction, p_node functionPtr)
{
    *l = get_token(binaryTree);
    bool ok = true;
    int result = 0;
    if (l->type == COMMA)
    {
        *l = get_token(binaryTree);
        if (l->type == VARIABLE_ID)
        {
            int oke = 0;
            if(comesFromFunction)
            {
                oke = check_if_variable_is_defined(functionPtr, l->extra_data.string);
            } else
            {
                oke = check_if_variable_is_defined(globalFunctions, l->extra_data.string);
            }
            if(!oke)
            {
                error(l->row, "Proměnná ve volání WRITE nebyla deklarovaná", SEM_UNDEFINED_VAR_ERROR);
                ok = false;
            }
       
        }  else if (l->type == MINUS)
        {
            *l = get_token(binaryTree);
            switch (l->type)
            {
                case NUMBER:
                    l->extra_data.value = -(l->extra_data.value);
                    break;
                case DECIMAL_NUMBER:
                    l->extra_data.decimal = -(l->extra_data.decimal);
                    break;
                case EXPONENT_NUMBER:
                    l->extra_data.exponent = -(l->extra_data.exponent);
                    break;
                default:
                    ok = false;
                    error(l->row, "Neočekávaný znak ve volání WRITE", SYNTAX_ERROR);
            }
        } else if (l->type == STRING_LITERAL)
        {
            evaluateEscapeSequencies(l);
        } else if(l->type == NUMBER || l->type == DECIMAL_NUMBER || l->type == EXPONENT_NUMBER)
        {
            ok = true;
        } else
        {
            ok = false;
            error(l->row, "Neočekávaný znak ve volání WRITE", SYNTAX_ERROR);
        }
        if (ok)
        {
            codeGenWrite(*l, comesFromFunction);
            result = writeString2(l, binaryTree, globalFunctions, comesFromFunction, functionPtr);
        }
    }else if (l->type == RBRACKET)
    {
        result = 1;
    } else 
    {
        error(l->row, "Neočekávaný znak ve volání WRITE", SYNTAX_ERROR);
    }
    return result;
}

void shiftRight(char* buffer, int shift, int stringlength) 
{
    for(int j = stringlength - 1 - shift; j>0; j--)
    {
        buffer[j+shift] = buffer[j];
    }
    return;
}

void replaceEscapeSequenceByNumber(char *buffer, char c)
{
    int asciivalue = (int)c;
    buffer[3] = asciivalue%10 + '0';
    asciivalue = (asciivalue - asciivalue%10)/10;
    buffer[2] = asciivalue%10 + '0';
    asciivalue = (asciivalue - asciivalue%10)/10;
    buffer[1] = asciivalue%10 + '0';
    buffer[0] = '\\';
    return;
}

void evaluateEscapeSequencies(Lexeme *l)
{
    int stringlength = strlen(l->extra_data.string);
    stringlength +=1;   //kvuli znaku '\0', ktery tam funkce strlen nezapocitava
    int i = 0;
    char c;
    while (i < stringlength-1)
    {
        c = l->extra_data.string[i];
        switch(c)
        {
            case '\0'...' ':
            case '#':
            case '\\':
                l->extra_data.string = realloc(l->extra_data.string, stringlength+3);   //potreba nachystat misto pro 3 znaky escape sekvence
                shiftRight(&l->extra_data.string[i], 3, stringlength-i+3);
                replaceEscapeSequenceByNumber(&l->extra_data.string[i], c);
                stringlength +=3;
                break;

        }
        i++;
    }
}
