#include "scanner.h"


p_node init_binary_treeKW()
{
    p_node root = node_init(NULL, "if");
    p_node node2 = node_init(NULL, "while");
    insert_node(root, node2);
    p_node node3 = node_init(NULL, "else");
    insert_node(root, node3);
    p_node node4 = node_init(NULL, "return");
    insert_node(root, node4);
    p_node node5 = node_init(NULL, "int");
    insert_node(root, node5);
    p_node node6 = node_init(NULL, "float");
    insert_node(root, node6);
    p_node node7 = node_init(NULL, "string");
    insert_node(root, node7);
    p_node node8 = node_init(NULL, "void");
    insert_node(root, node8);
    p_node node9 = node_init(NULL, "null");
    insert_node(root, node9);
    return root;
}

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
                case '?':
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
                case ',':
                    return Comma;
                case '.':
                    return Konkatenace;
                case '<':
                    return Less;
                case '>':
                    return Greater;
                case '!':
                    return Not;
                case ' ':
                case '\n':
                case '\t':
                    return Start;
                default:
                    return ScanError;
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
            else if (c == '>') return FileEndSign;
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
            if (c == '?') return String;
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
                return Error;
            else if (c == '*')
                return BlockCommentPotentialEnd;
            else return BlockComment;
        case DivideOrComment:
            if (c == '/')
                return LineComment;
            else if(c == '*')
                return BlockComment;
            else return Divide; 
        case Comma:
        case Prolog:
        case FileEndSign:
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
            //printf("c %c\n", buffer[i+1]);
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
        case Comma:
            final_lexeme.type = COMMA;
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
        case Prolog:
            final_lexeme.type = PROLOG;
            break;
        case FileEndSign:
            final_lexeme.type = FILE_END_SIGN;
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
        case ScanError:
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
        if ((c != ' ' || current_state == String || current_state == LineComment || current_state == BlockComment) && c != EOF)
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
            if (current_state == LineComment)
            {
                free(buffer);
                return (Lexeme){.type = LEXEOF};
            }
            *(current_index++) = '\0';
            stringlength++;
            return generateLexeme(current_state, buffer, stringlength);
            stringlength = 0;
        }
        if (current_state == String && (!(strcmp(buffer, "<?php\n")) || !(strcmp(buffer, "<?php\t")) || !(strcmp(buffer, "<?php "))))
        {
            next_state = Prolog;
            ungetc(c, stdin);
        }else if (current_state == String && stringlength == 7)
        {
            if(!strncmp(buffer, "<?phpx", 5))
            {
                free(buffer);
                return (Lexeme){.type = SCANERROR};
            }
        }
        else
        {
            next_state = transition(current_state, (char)c);
        }
        if (next_state == Error)
        {
            if (c != ' ')
            {
                stringlength--;
            }
            if (current_state != BlockCommentPotentialEnd)
            ungetc((char)c, stdin);
            
            *(current_index++) = '\0';
            stringlength++;
            current_index = buffer;
            if (current_state == LineComment || current_state == BlockCommentPotentialEnd)
            {
                free(buffer);
                Lexeme l;
                l.type = NULLLEX;
                return l;
            }
            Lexeme l = generateLexeme(current_state, buffer, stringlength);
            free(buffer);
            return l;
        } else if (next_state == ScanError)
        {
            free(buffer);
            Lexeme l;
            l.type = SCANERROR;
            return l;
        }
        *(current_index++) = c;
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
        default: return "CHYBA";
    }
}

void check_forKW(p_node root, Lexeme *l)
{
    p_node found = tree_search(root, l->extra_data.string);

    if (found != NULL)
    {
        if (strcmp(found->key, "while") == 0)
        {
            l->type = KW_WHILE;
        } else if (strcmp(found->key, "if") == 0)
        {
            l->type = KW_IF;
        }
        else if (strcmp(found->key, "else") == 0)
        {
            l->type = KW_ELSE;
        }
        else if (strcmp(found->key, "null") == 0)
        {
            l->type = KW_NULL;
        }
        else if (strcmp(found->key, "return") == 0)
        {
            l->type = KW_RETURN;
        }
        else if (strcmp(found->key, "void") == 0)
        {
            l->type = KW_VOID;
        }
        else if (strcmp(found->key, "int") == 0)
        {
            l->type = KW_INT;
        }
        else if (strcmp(found->key, "float") == 0)
        {
            l->type = KW_FLOAT;
        }
        else if (strcmp(found->key, "string") == 0)
        {
            l->type = KW_STRING;
        }
    }
}


Lexeme get_token(p_node binaryTree)
{
    //p_node binaryTree = init_binary_treeKW();
    Lexeme l = {.type = NULLLEX};

    while(l.type == NULLLEX){
        l = scan_lexeme();
    }
    if (l.type == FUNCTION_ID)
    {
        check_forKW(binaryTree, &l);
    }
    return l;

    /* Debug vypsani vsech lexemu 
    while(l.type != LEXEOF)
    {
        l = scan_lexeme();

        if (l.type == FUNCTION_ID)
        {
            check_forKW(binaryTree, &l);
        }

        if (l.type == NUMBER)
        {
            printf("lexem je %d\n", l.extra_data.value);
        } else if (l.type == EXPONENT_NUMBER)
        {
            printf("lexem je  %f\n", l.extra_data.exponent);
        } else if (l.type == DECIMAL_NUMBER)
        {
            printf("lexem je %f\n", l.extra_data.decimal);
        } else if (l.type == SCANERROR)
        {
            printf("Unexpected character\n");
            return 1;
        }else if (l.type == COMMA)
        {
            printf("lexem je carka\n");
        } else if (l.type == PROLOG)
        {
            printf("lexem je <?php\n");
        }else if (l.type == FILE_END_SIGN)
        {
            printf("lexem je ?>\n");
        }else if (l.type == KW_WHILE)
        {
            printf("keyword je WHILE\n");
        }else if (l.type == KW_IF)
        {
            printf("keyword je IF\n");
        }
        else if (l.type == KW_ELSE)
        {
            printf("keyword je ELSE\n");
        }
        else if (l.type == KW_NULL)
        {
            printf("keyword je NULL\n");
        }
        else if (l.type == KW_RETURN)
        {
            printf("keyword je RETURN\n");
        }
        else if (l.type == KW_VOID)
        {
            printf("keyword je VOID\n");
        }
        else if (l.type == KW_INT)
        {
            printf("keyword je INT\n");
        }
        else if (l.type == KW_FLOAT)
        {
            printf("keyword je FLOAT\n");
        }
        else if (l.type == KW_STRING)
        {
            printf("keyword je STRING\n");
        }
        else
        {
            printf("lexem je %s\n", str_lexeme(l));
            //printf("typ lexemu je %d\n", l.type);
        }
        
    }
    tree_destroy(binaryTree);
    return l;
    
    */

}   