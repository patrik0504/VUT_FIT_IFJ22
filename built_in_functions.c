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
            case 92:
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
    return result;
}

