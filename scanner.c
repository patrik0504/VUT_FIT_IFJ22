#include "scanner.h"

AutomatState transition(AutomatState currentState, char c)
{
    switch(currentState)
    {
        case Error:
            fprintf(stderr, "Should have generated token by now.");
            return Error;
        case Start:
            if(c == EOF) return LexEOF;
            else if (c == ';') return Semicolon;
            else if (c == ':') return Colon;
            else if (c == '"') return String;
            else if (c >= '0' && c <= '9') return Number;
            else if (isalpha(c) || c == '_') return Identifier;
            else if (c == '$') return Var;
            else if (c == '(') return LBracket;
            else if (c == ')') return RBracket;
            else if (c == '{') return LBracketSKudrlinkou;
            else if (c == '}') return RBracketSKudrlinkou;
            else if (c == '=') return Equal;
            else if (c == '+') return Plus;
            else if (c == '-') return Minus;
            else if (c == '*') return Multiply;
            else if (c == '/') return Divide;
            else if (c == '.') return Konkatenace;
            else if (c == '<') return Less;
            else if (c == '>') return Greater;
            else if (c == '!') return Not;
            else return Start;
        case Number:
            if (c >= '0' && c <= '9') return Number;
            else if (c == '.') return Decimal;
            else return Error;
        case Decimal:
            if ( c >= '0' && c <= '9') return Decimal;
            else if (c == 'e' || c == 'E') return Exponential;
            else return Error;
        case Exponential:
            if (c == '+' || c == '-' || (c >= '0' && c <= '9')) return Exponent;
            else return Error;
        case Exponent:
            if ( c >= '0' && c <= '9') return Exponent;
            else return Error;
        case Identifier:
            if (isalnum(c)) return Identifier;
            else return Error;
        case Var:
            if (isalpha(c) || c == '_') return LoadVar;
            else return Error;
        case LoadVar:
            if (isalnum(c) || c == '_') return LoadVar;
            else return Error;
        case Equal:
            if (c == '=') return Equal2;
            else return Error;
        case Equal2:
            if (c == '=') return Equal3;
            else return Error;
        case Not:
            if ( c == '=') return Not2;
            else return Error;
        case Not2:
            if (c == '=') return NotEqual;
            else return Error;
        case Greater:
            if (c == '=') return GreaterEqual;
            else return Error;
        case Less:
            if (c == '=') return LessEqual;
            else return Error;
        case LexEOF:
        case Semicolon:
        case Colon:
        case StringEnd:
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

int get_string_length(char* buffer)
{
    int i = 0;
    while(buffer[i] != '\0')
    {
        i++;
    }
    return i;
}

Lexeme generateLexeme(AutomatState state, char* pole)
{
    Lexeme final_lexeme;
    int length;
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
            break;
        case Number:
            final_lexeme.type = NUMBER;
            break;
        case Decimal:
            final_lexeme.type = DECIMAL_NUMBER;
            break;
        case Exponent:
            final_lexeme.type = EXPONENT_NUMBER;
            break;
        case Identifier:
            final_lexeme.type = FUNCTION_ID;
            length = get_string_length(pole);
            final_lexeme.extra_data.string = malloc(length*sizeof(char));
            strcpy(final_lexeme.extra_data.string, pole);
            break;
        case LoadVar:
            final_lexeme.type = VARIABLE_ID;
            length = get_string_length(pole);
            final_lexeme.extra_data.string = malloc(length*sizeof(char));
            strcpy(final_lexeme.extra_data.string, pole);
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
    char pole[100];
    char* current_index;
    current_index = pole;
    int c;
    while(true)
    {
        c = getchar();
        /*if (c == ' ')
            printf("nacteny znak je mezera\n");
        else
        printf("nacteny znak je %c\n", c);*/
        if ( c == EOF)
        {
            if (current_state == Start)
            {
                return (Lexeme){.type = LEXEOF};
            }
            *(current_index++) = '\0';
            return generateLexeme(current_state, pole);
        }
        //printf("current state je %d\n", current_state);
        next_state = transition(current_state, (char)c);
        //printf("next state je %d\n", next_state);
        if (next_state == Error)
        {
            ungetc((char)c, stdin);
            *(current_index++) = '\0';
            current_index = pole;
            //printf("pole je %s\n", pole);
            return generateLexeme(current_state, pole);
        }
        *(current_index++) = c;
        current_state = next_state;

    }
}

char * str_lexeme(Lexeme in)
{
    switch(in.type)
    {
        case LEXEOF: return "EOF";
        case SEMICOLON: return ";";
        case COLON: return ":";
        case STRING_LITERAL: return in.extra_data.string;
        case NUMBER: return in.extra_data.string;
        case DECIMAL_NUMBER: return in.extra_data.string;
        case EXPONENT_NUMBER:
        case FUNCTION_ID:
        case VARIABLE_ID:
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
    Lexeme l = {.type = PLUS};
    while(l.type != LEXEOF)
    {
        l = scan_lexeme();
        printf("lexem je %s\n", str_lexeme(l));
    }
    return 0;
}