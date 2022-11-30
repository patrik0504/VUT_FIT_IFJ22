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
            ok = declaredCheck(binaryTree, globalFunctions, comesFromFunction, functionPtr, *l);
            if (!ok)
            {
                error(l->row, "Proměnná ve volání WRITE není definovaná", SEM_UNDEFINED_VAR_ERROR);
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
        } 
        if (ok)
        {
            codeGenWrite(*l);
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
            ok = declaredCheck(binaryTree, globalFunctions, comesFromFunction, functionPtr, *l);    
            if (!ok)
            {
                error(l->row, "Promenna ve volani WRITE neni definovana", SEM_UNDEFINED_VAR_ERROR);
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
        } 
        if (ok)
        {
            codeGenWrite(*l);
            result = writeString2(l, binaryTree, globalFunctions, comesFromFunction, functionPtr);
        }
    }else if (l->type == RBRACKET)
    {
        result = 1;
    }
    return result;
}

bool declaredCheck(p_node binaryTree, p_node globalFunctions, bool comesFromFunction, p_node functionPtr, Lexeme l)
{
    if (comesFromFunction)
    {
        if(((functionPtr->data->params != NULL) && (tree_search(functionPtr->data->params, l.extra_data.string) == NULL)) || (functionPtr->data->params == NULL))
        {
                    if (((functionPtr->data->elements != NULL) && (tree_search(functionPtr->data->elements, l.extra_data.string) == NULL)) || (functionPtr->data->elements == NULL))
                    {
                        return false;
                    }
        }
        return true;
    } else
    {
        if(((globalFunctions->data->params != NULL) && (tree_search(globalFunctions->data->params, l.extra_data.string) == NULL)) || (globalFunctions->data->params == NULL))
        {
                    if (((globalFunctions->data->elements != NULL) && (tree_search(globalFunctions->data->elements, l.extra_data.string) == NULL)) || (globalFunctions->data->elements == NULL))
                    {
                        return false;
                    }
        }
        return true;
    }
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
    buffer[3] = asciivalue%10 + 48;
    asciivalue = (asciivalue - asciivalue%10)/10;
    buffer[2] = asciivalue%10 + 48;
    asciivalue = (asciivalue - asciivalue%10)/10;
    buffer[1] = asciivalue%10 + 48;
    //92 == '\'
    buffer[0] = 92;
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
                l->extra_data.string = realloc(l->extra_data.string, stringlength+3);
                shiftRight(&l->extra_data.string[i], 3, stringlength-i+3);
                replaceEscapeSequenceByNumber(&l->extra_data.string[i], c);
                stringlength +=3;
                break;

        }
        i++;
    }
}

int builtInReads(Lexeme *l, p_node binaryTree, p_node globalFunctions, bool comesFromFunction, p_node functionPtr)
{
    int result = 0;
    *l = get_token(binaryTree);
    if (l->type == LBRACKET)
    {
        *l = get_token(binaryTree);
        if (l->type == RBRACKET)
        {
            codeGenReads();
            result = 1;
        }
    }
    if (get_token(binaryTree).type != SEMICOLON)
    {
        result = 0;
    }
    return result;
}

int builtInReadi(Lexeme *l, p_node binaryTree, p_node globalFunctions, bool comesFromFunction, p_node functionPtr)
{
    int result = 0;
    *l = get_token(binaryTree);
    if (l->type == LBRACKET)
    {
        *l = get_token(binaryTree);
        if (l->type == RBRACKET)
        {
            codeGenReadi();
            result = 1;
        }
    }
    if (get_token(binaryTree).type != SEMICOLON)
    {
        result = 0;
    }
    return result;
}

int builtInReadf(Lexeme *l, p_node binaryTree, p_node globalFunctions, bool comesFromFunction, p_node functionPtr)
{
    int result = 0;
    *l = get_token(binaryTree);
    if (l->type == LBRACKET)
    {
        *l = get_token(binaryTree);
        if (l->type == RBRACKET)
        {
            codeGenReadf();
            result = 1;
        }
    }
    if (get_token(binaryTree).type != SEMICOLON)
    {
        result = 0;
    }
    return result;
}

int builtInOrd(Lexeme *l, p_node binaryTree, p_node globalFunctions, bool comesFromFunction, p_node functionPtr)
{
    int result = 0;
    bool ok = true;
    *l = get_token(binaryTree);
    if (l->type == LBRACKET)
    {
        *l = get_token(binaryTree);
        if (l->type == VARIABLE_ID)
        {
            ok = declaredCheck(binaryTree, globalFunctions, comesFromFunction, functionPtr, *l);
            if (!ok)
            {
                error(l->row, "Proměnná ve volání ORD není definovaná", SEM_UNDEFINED_VAR_ERROR);
                return 0;
            }
        }else if (l->type == STRING_LITERAL)
        {
            evaluateEscapeSequencies(l);
        }else
        {
            error(l->row, "Špatný typ parametru alebo bez parametrov pri voláni ORD", SYNTAX_ERROR);
            return 0;
        }
        *l = get_token(binaryTree);
        if (l->type == RBRACKET)
        {
            codeGenOrd();
            result = 1;
        }
    }
    if (get_token(binaryTree).type != SEMICOLON)
    {
        result = 0;
    }
    return result;
}

int builtInChr(Lexeme *l, p_node binaryTree, p_node globalFunctions, bool comesFromFunction, p_node functionPtr)
{
    int result = 0;
    bool ok = true;
    *l = get_token(binaryTree);
    if (l->type == LBRACKET)
    {
        *l = get_token(binaryTree);
        if (l->type == VARIABLE_ID)
        {
            ok = declaredCheck(binaryTree, globalFunctions, comesFromFunction, functionPtr, *l);
            if (!ok)
            {
                error(l->row, "Proměnná ve volání CHR není definovaná", SEM_UNDEFINED_VAR_ERROR);
                return 0;
            }
        }else if (l->type == NUMBER)
        {
            ok = true;
        }else
        {
            error(l->row, "Špatný typ parametru alebo bez parametrov pri voláni ORD", SYNTAX_ERROR);
            return 0;
        }
        *l = get_token(binaryTree);
        if (l->type == RBRACKET)
        {
            //Vytvorit funckciu codeGenChr
            //codeGenChr();
            result = 1;
        }
    }
    if (get_token(binaryTree).type != SEMICOLON)
    {
        result = 0;
    }
    return result;
}

///////////////////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!///////////////////////////
///////////////////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!///////////////////////////
//Floatval, Intval, Strval neberú momentálne BOOL ako parameter 
///////////////////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!///////////////////////////
///////////////////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!///////////////////////////

int builtInFloatval(Lexeme *l, p_node binaryTree, p_node globalFunctions, bool comesFromFunction, p_node functionPtr)
{
    int result = 0;
    bool ok = true;
    *l = get_token(binaryTree);
    if (l->type == LBRACKET)
    {
        *l = get_token(binaryTree);
        if (l->type == VARIABLE_ID)
        {
            ok = declaredCheck(binaryTree, globalFunctions, comesFromFunction, functionPtr, *l);
            if (!ok)
            {
                error(l->row, "Proměnná ve volání FLOATVAL není definovaná", SEM_UNDEFINED_VAR_ERROR);
                return 0;
            }
        }else if (l->type == NUMBER || l->type == FLOAT || l->type == STRING_LITERAL)
        {
            ok = true;
        } else
        {
            error(l->row, "Špatný typ parametru alebo bez parametrov pri voláni FLOATVAL", SYNTAX_ERROR);
            return 0;
        }
        *l = get_token(binaryTree);
        if (l->type == RBRACKET)
        {
            //Vytvorit funckciu codeGenFloatval
            //codeGenFloatval();
            result = 1;
        }
    }
    if (get_token(binaryTree).type != SEMICOLON)
    {
        result = 0;
    }
    return result;
}

int builtInIntval(Lexeme *l, p_node binaryTree, p_node globalFunctions, bool comesFromFunction, p_node functionPtr)
{
    int result = 0;
    bool ok = true;
    *l = get_token(binaryTree);
    if (l->type == LBRACKET)
    {
        *l = get_token(binaryTree);
        if (l->type == VARIABLE_ID)
        {
            ok = declaredCheck(binaryTree, globalFunctions, comesFromFunction, functionPtr, *l);
            if (!ok)
            {
                error(l->row, "Proměnná ve volání INTVAL není definovaná", SEM_UNDEFINED_VAR_ERROR);
                return 0;
            }
        }else if (l->type == NUMBER || l->type == FLOAT || l->type == STRING_LITERAL)
        {
            ok = true;
        } else
        {
            error(l->row, "Špatný typ parametru alebo bez parametrov pri voláni INTVAL", SYNTAX_ERROR);
            return 0;
        }
        *l = get_token(binaryTree);
        if (l->type == RBRACKET)
        {
            //Vytvorit funckciu codeGenIntval
            //codeGenIntval();
            result = 1;
        }
    }
    if (get_token(binaryTree).type != SEMICOLON)
    {
        result = 0;
    }
    return result;
}

int builtInStrval(Lexeme *l, p_node binaryTree, p_node globalFunctions, bool comesFromFunction, p_node functionPtr)
{
    int result = 0;
    bool ok = true;
    *l = get_token(binaryTree);
    if (l->type == LBRACKET)
    {
        *l = get_token(binaryTree);
        if (l->type == VARIABLE_ID)
        {
            ok = declaredCheck(binaryTree, globalFunctions, comesFromFunction, functionPtr, *l);
            if (!ok)
            {
                error(l->row, "Proměnná ve volání STRVAL není definovaná", SEM_UNDEFINED_VAR_ERROR);
                return 0;
            }
        }else if (l->type == NUMBER || l->type == FLOAT || l->type == STRING_LITERAL)
        {
            ok = true;
        } else
        {
            error(l->row, "Špatný typ parametru alebo bez parametrov pri voláni STRVAL", SYNTAX_ERROR);
            return 0;
        }
        *l = get_token(binaryTree);
        if (l->type == RBRACKET)
        {
            //Vytvorit funckciu codeGenStrval
            //codeGenStrval();
            result = 1;
        }
    }
    if (get_token(binaryTree).type != SEMICOLON)
    {
        result = 0;
    }
    return result;
}

int builtInSubstring(Lexeme *l, p_node binaryTree, p_node globalFunctions, bool comesFromFunction, p_node functionPtr)
{
    int result = 0;
    bool ok = true;
    *l = get_token(binaryTree);
    if (l->type == LBRACKET)
    {
        *l = get_token(binaryTree);
        if (l->type == VARIABLE_ID)
        {
            ok = declaredCheck(binaryTree, globalFunctions, comesFromFunction, functionPtr, *l);
            if (!ok)
            {
                error(l->row, "Proměnná ve volání SUBSTR není definovaná", SEM_UNDEFINED_VAR_ERROR);
                return 0;
            }
        }else if (l->type == STRING_LITERAL)
        {
            ok = true;
        } else
        {
            error(l->row, "Špatný typ parametru alebo bez parametrov pri voláni SUBSTR", SYNTAX_ERROR);
            return 0;
        }
        *l = get_token(binaryTree);
        if (l->type == COMMA)
        {
            *l = get_token(binaryTree);
            if (l->type == VARIABLE_ID)
            {
                ok = declaredCheck(binaryTree, globalFunctions, comesFromFunction, functionPtr, *l);
                if (!ok)
                {
                    error(l->row, "Proměnná ve volání SUBSTR není definovaná", SEM_UNDEFINED_VAR_ERROR);
                    return 0;
                }
            }else if (l->type == NUMBER)
            {
                ok = true;
            } else
            {
                error(l->row, "Špatný typ parametru alebo bez parametrov pri voláni SUBSTR", SYNTAX_ERROR);
                return 0;
            }
            *l = get_token(binaryTree);
            if (l->type == COMMA)
            {
                *l = get_token(binaryTree);
                if (l->type == VARIABLE_ID)
                {
                    ok = declaredCheck(binaryTree, globalFunctions, comesFromFunction, functionPtr, *l);
                    if (!ok)
                    {
                        error(l->row, "Proměnná ve volání SUBSTR není definovaná", SEM_UNDEFINED_VAR_ERROR);
                        return 0;
                    }
                }else if (l->type == NUMBER)
                {
                    ok = true;
                } else
                {
                    error(l->row, "Špatný typ parametru alebo bez parametrov pri voláni SUBSTR", SYNTAX_ERROR);
                    return 0;
                }
                *l = get_token(binaryTree);
                if (l->type == RBRACKET)
                {
                    //Vytvorit funckciu codeGenSubstring
                    //codeGenSubstring();
                    result = 1;
                }
            }
            else
            {
                error(l->row, "Chýba čiarka", SYNTAX_ERROR);
                return 0;
            }
        }
        else
        {
            error(l->row, "Chýba čiarka", SYNTAX_ERROR);
            return 0;
        }
    }
    if (get_token(binaryTree).type != SEMICOLON)
    {
        result = 0;
    }
    return result;
}

int builtInStrlen(Lexeme *l, p_node binaryTree, p_node globalFunctions, bool comesFromFunction, p_node functionPtr)
{
    int result = 0;
    bool ok = true;
    *l = get_token(binaryTree);
    if (l->type == LBRACKET)
    {
        *l = get_token(binaryTree);
        if (l->type == VARIABLE_ID)
        {
            ok = declaredCheck(binaryTree, globalFunctions, comesFromFunction, functionPtr, *l);
            if (!ok)
            {
                error(l->row, "Proměnná ve volání STRLEN není definovaná", SEM_UNDEFINED_VAR_ERROR);
                return 0;
            }
        }else if (l->type == STRING_LITERAL)
        {
            ok = true;
        } else
        {
            error(l->row, "Špatný typ parametru alebo bez parametrov pri voláni STRLEN", SYNTAX_ERROR);
            return 0;
        }
        *l = get_token(binaryTree);
        if (l->type == RBRACKET)
        {
            //Vytvorit funckciu codeGenStrlen
            //codeGenStrlen();
            result = 1;
        }
    }
    if (get_token(binaryTree).type != SEMICOLON)
    {
        result = 0;
    }
    return result;
}
