#include "scanner.h"

AutomatState transition(AutomatState currentState, char c)
{
    switch(currentState)
    {
        case Error:
            fprintf(stderr, "Should have generated token by now.");
            return Error;
        case Start:
            switch (c)
            {
                case EOF:
                    return LexEOF;
                case ';':
                    return Semicolon;
                case ':':
                    return Colon;
                case '"':
                    return String;
                case '0'...'9':
                    return Number;
                case 'a'...'z':
                case 'A'...'Z':
                case '_':
                    return Identifier;
                case '$':
                    return Var;
                case '(':
                    return LBracket;
                case ')':
                    return RBracket;
                case '{':
                    return LBracketSKudrlinkou;
                case '}':
                    return RBracketSKudrlinkou;
                case '=':
                    return Equal;
                case '+':
                    return Plus;
                case '-':
                    return Minus;
                case '*':
                    return Multiply;
                case '/':
                    return DivideOrComment;
                case '.':
                    return Konkatenace;
                case '<':
                    return Less;
                case '>':
                    return Greater;
                case '!':
                    return Not;
                default:
                    return Start;
            }
        case Number:
            if (is_number(c)) return Number;
            if (c == '.') return Decimal;
            return Error;
        case Decimal:
            if (is_number(c)) return Decimal;
            if (c == 'e' || c == 'E') return Exponential;
            return Error;
        case Exponential:
            if (c == '+' || c == '-' || (is_number(c))) return Exponent;
            return Error;
        case Exponent:
            if (is_number(c)) return Exponent;
            return Error;
        case Identifier:
            if (isalnum(c) || c == '_') return Identifier;
            return Error;
        case Var:
            if (isalpha(c) || c == '_') return LoadVar;
            return Error;
        case LoadVar:
            if (isalnum(c) || c == '_') return LoadVar;
            return Error;
        case Equal:
            if (c == '=') return Equal2;
            return Error;
        case Equal2:
            if (c == '=') return Equal3;
            return Error;
        case Not:
            if ( c == '=') return Not2;
            return Error;
        case Not2:
            if (c == '=') return NotEqual;
            return Error;
        case Greater:
            if (c == '=') return GreaterEqual;
            return Error;
        case Less:
            if (c == '=') return LessEqual;
            return Error;
        case String:
            if (c != '"') return String;
            else return StringEnd;
        case LineComment:
            if (c == '\n')
                return Error;
            else return LineComment;
        case BlockComment:
            if (c == '*')
                return BlockCommentPotentialEnd;
            else return BlockComment;
        case BlockCommentPotentialEnd:
            if (c == '/')
                return BlockCommentEnd;
            else if (c == '*')
                return BlockCommentPotentialEnd;
            else return BlockComment;
        case DivideOrComment:
            if (c == '/')
                return LineComment;
            else if(c == '*')
                return BlockComment;
            else return Divide; 
        case LexEOF:
        case Semicolon:
        case Colon:
        case StringEnd:
        case BlockCommentEnd:
        case LBracket:
        case RBracket:
        case RBracketSKudrlinkou:
        case LBracketSKudrlinkou:
        case Equal3:
        case Plus:
        case Minus:
        case Multiply:
        case Divide:
        case Konkatenace:
        case LessEqual:
        case GreaterEqual:
        case NotEqual:
        default:
            return Error;
    }
    return Error;
}

void shiftLeft(char* buffer, int shift, int stringlength) // "dsdsd\ndsdsd"
{
    for(int j = 1; j+shift <= stringlength; j++)
    {
        buffer[j] = buffer[j+shift];
    }
    return;
}



int transferEscapeSequences(char* buffer, int stringlength)
{
    for (int i = 0; i < stringlength; i++)
    {
        //92 == '\'
        if (buffer[i] == 92)
        {
            if ((i+1 < stringlength) && buffer[i+1] == 'n') //TODO: rest of escape sequences
            {
                buffer[i] = '\n';
                shiftLeft(&buffer[i], ESCAPE, stringlength - i);
                stringlength -= ESCAPE;
                continue;
            } else if((i+1 < stringlength) && buffer[i+1] == 't')
            {
                buffer[i] = '\t';
                shiftLeft(&buffer[i], ESCAPE, stringlength - i);
                stringlength -= ESCAPE;
                continue;
            } else if((i+1 < stringlength) && buffer[i+1] == 92)
            {
                buffer[i] = 92;
                shiftLeft(&buffer[i], ESCAPE, stringlength - i);
                stringlength -= ESCAPE;
                continue;
            } else if((i+1 < stringlength) && buffer[i+1] == 'x')
            {
                if(i+3 > stringlength)
                {
                    continue;
                }
                //convert hex to decimal (and check for valid hex number)
                int ascii_value = StrHexToDec(&buffer[i+2]);
                //check for correct ascii value
                if(ascii_value == ERRORRETURN)
                {
                    continue;
                }
                buffer[i] = ascii_value;
                shiftLeft(&buffer[i], ESCAPEHEXA, stringlength - i);
                stringlength -= ESCAPEHEXA;
            }else if (i+3 < stringlength)
            {
                //convert octal to decimal (and check for valid octal number)
                int ascii_value = StrOctToDec(&buffer[i+1]);
                //check for correct ascii value                 
                if(ascii_value == ERRORRETURN){
                    continue;
                }
                buffer[i] = ascii_value;
                shiftLeft(&buffer[i], ESCAPEOCTA, stringlength - i);
                stringlength -= ESCAPEOCTA;
            }
            
        }
    }
    return stringlength;
}


Lexeme generateLexeme(AutomatState state, char* buffer, int stringlength)
{
    Lexeme final_lexeme;
    switch(state)
    {
        case LexEOF:
            final_lexeme.type = LEXEOF;
            break;
        case Semicolon:
            final_lexeme.type = SEMICOLON;
            break;
        case Colon:
            final_lexeme.type = COLON;
            break;
        case StringEnd:
            final_lexeme.type = STRING_LITERAL;
            stringlength = transferEscapeSequences(buffer, stringlength);
            stringlength-=2;    //kvuli uvozovkam na zacatku a na konci retezce
            final_lexeme.extra_data.string =  malloc((stringlength)*sizeof(char));
            strncpy(final_lexeme.extra_data.string, buffer+1, (stringlength-1)*sizeof(char));
            final_lexeme.extra_data.string[stringlength-1] = '\0';
            break;
        case Number:
            final_lexeme.type = NUMBER;
            final_lexeme.extra_data.value = atoi(buffer);
            break;
        case Decimal:
            final_lexeme.type = DECIMAL_NUMBER;
            final_lexeme.extra_data.decimal = atof(buffer);
            break;
        case Exponent:
            final_lexeme.type = EXPONENT_NUMBER;
            final_lexeme.extra_data.exponent = atof(buffer);
            break;
        case Identifier:
            final_lexeme.type = FUNCTION_ID;
            final_lexeme.extra_data.string = malloc(stringlength*sizeof(char));
            strcpy(final_lexeme.extra_data.string, buffer);
            break;
        case LoadVar:
            final_lexeme.type = VARIABLE_ID;
            final_lexeme.extra_data.string = malloc((stringlength-1)*sizeof(char));
            strcpy(final_lexeme.extra_data.string, buffer + sizeof(char));
            break;
        case LBracket:
            final_lexeme.type = LBRACKET;
            break;
        case RBracket:
            final_lexeme.type = RBRACKET;
            break;
        case LBracketSKudrlinkou:
            final_lexeme.type = LBRACKET_S_KUDRLINKOU;
            break;
        case RBracketSKudrlinkou:
            final_lexeme.type = RBRACKET_S_KUDRLINKOU;
            break;
        case Equal:
            final_lexeme.type = EQUAL;
            break;
        case Equal3:
            final_lexeme.type = EQUAL3;
            break;
        case Plus:
            final_lexeme.type = PLUS;
            break;
        case Minus:
            final_lexeme.type = MINUS;
            break;
        case Multiply:
            final_lexeme.type = MULTIPLY;
            break;
        case Divide:
            final_lexeme.type = DIVIDE;
            break;
        case Konkatenace:
            final_lexeme.type = KONKATENACE;
            break;
        case Less:
            final_lexeme.type = LESS;
            break;
        case LessEqual:
            final_lexeme.type = LESSEQUAL;
            break;
        case Greater:
            final_lexeme.type = GREATER;
            break;
        case GreaterEqual:
            final_lexeme.type = GREATEREQUAL;
            break;
        case NotEqual:
            final_lexeme.type = NOTEQUAL;
            break;
        case Start:
        case DivideOrComment:
        case LineComment:
        case BlockComment:
        case BlockCommentPotentialEnd:
        case BlockCommentEnd:
        case String:
        case Exponential:
        case Var:
        case Equal2:
        case Not:
        case Not2:
        case Error:
            exit(1);
    }
    return final_lexeme;
}

Lexeme scan_lexeme()
{
    AutomatState current_state = Start;
    AutomatState next_state;
    int current_array_size = ARRAYSIZE;
    char *buffer = malloc(sizeof(char)*ARRAYSIZE);
    int stringlength = 0;
    char* current_index;
    current_index = buffer;
    char c;
    while(true)
    {
        c = getchar();
        if(c == '"')
        {
            if(buffer[stringlength-1] == 92)
            {
                buffer[stringlength-1] = c;
                continue;
            }
        }
        if ((c != ' ' || current_state == String) && c != EOF)
            stringlength++;
        if (stringlength == current_array_size)
        {
            current_array_size += ARRAYSIZE;
            buffer = realloc(buffer, current_array_size);
            current_index = buffer + current_array_size - ARRAYSIZE - 1;
        }
        if ( c == EOF)
        {
            if (current_state == Start)
            {
                return (Lexeme){.type = LEXEOF};
            }
            *(current_index++) = '\0';
            stringlength++;
            return generateLexeme(current_state, buffer, stringlength);
            stringlength = 0;
        }
        next_state = transition(current_state, (char)c);
        //printf("jsem ve stavu %d, next state je %d, nacteny znak je %c, ukladam na pozici %d\n", current_state, next_state, c, stringlength-1);
        //printf("current state: %d, next state: %d, c: %c\n", current_state, next_state, c);
        if (next_state == Error)
        {
            if (c != ' ')       //TODO: fix stringlength
            {
                stringlength--;
            }
            ungetc((char)c, stdin);
            
            *(current_index++) = '\0';
            stringlength++;
            current_index = buffer;
            if (current_state == LineComment || current_state == BlockCommentEnd)
            {
                free(buffer);
                Lexeme l;
                l.type = NULLLEX;
                return l;
            }
            Lexeme l = generateLexeme(current_state, buffer, stringlength);
            free(buffer);
            return l;
        }
        *(current_index++) = c;
        //printf("nacteny znak: %c, buffer: %s\n", c, buffer);
        current_state = next_state;
        if ((next_state == Start) /*&& c == ' '*/)
        {
            current_index = buffer;
            stringlength = 0;
        }
    }
    free(buffer);
}

char * str_lexeme(Lexeme in)
{
    switch(in.type)
    {
        case NULLLEX: return "KOMENTAR";
        case LEXEOF: return "EOF";
        case SEMICOLON: return ";";
        case COLON: return ":";
        case STRING_LITERAL: return in.extra_data.string;
        case NUMBER: return in.extra_data.string;
        case DECIMAL_NUMBER: return in.extra_data.string;
        case EXPONENT_NUMBER:
        case FUNCTION_ID: return in.extra_data.string;
        case VARIABLE_ID: return in.extra_data.string;
        case LBRACKET: return "(";
        case RBRACKET: return ")";
        case LBRACKET_S_KUDRLINKOU: return "{";
        case RBRACKET_S_KUDRLINKOU: return "}";
        case EQUAL: return "=";
        case EQUAL3: return "===";
        case PLUS: return "+";
        case MINUS: return "-";
        case MULTIPLY: return "*";
        case DIVIDE: return "/";
        case KONKATENACE: return ".";
        case LESS: return "<";
        case LESSEQUAL: return "<=";
        case GREATER: return ">";
        case GREATEREQUAL: return ">=";
        case NOTEQUAL: return "!=";
        case IF: return "IF";
        case WHILE: return "WHILE";
        default: return "CHYBA";
    }
}



int scanner()
{
    Lexeme l = {.type = NULLLEX};
    while(l.type != LEXEOF)
    {
        l = scan_lexeme();
        if (l.type == NUMBER)
        {
            printf(" %d", l.extra_data.value);
        } else if (l.type == EXPONENT_NUMBER)
        {
            printf(" %f", l.extra_data.exponent);
        } else if (l.type == DECIMAL_NUMBER)
        {
            printf(" %f", l.extra_data.decimal);
        } else
        {
            printf(" %s", str_lexeme(l));
        }
    }
    return 0;
}