/**
 * ***********************IFJ PROJEKT 2022********************************
 * @file scanner.c
 * @author  Veronika Nevařilová (xnevar00@stud.fit.vutbr.cz)
 *          Patrik Michlian     (xmichl12@stud.fit.vutbr.cz)
 * @brief Lexikální analýza
 * @date 2022-12-06
 * 
 * @copyright Copyright (c) 2022
*/


#include "scanner.h"


p_node init_binary_treeKW()
{
    // Inicializace binárního stromu
    // Postupné vložení klíčových slov do binárního stromu
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
    p_node node10 = node_init(NULL, "function");
    insert_node(root, node10);
    p_node node11 = node_init(NULL, "?int");
    insert_node(root, node11);
    p_node node12 = node_init(NULL, "?float");
    insert_node(root, node12);
    p_node node13 = node_init(NULL, "?string");
    insert_node(root, node13);
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
                    return Divide;
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
            {
                return Error;
            }
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
        case Divide:
            if (c == '/')
                return LineComment;
            else if(c == '*')
                return BlockComment;
            else return Error;
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
        case Konkatenace:
        case LessEqual:
        case GreaterEqual:
        case NotEqual:
        default:
            return Error;
    }
    return Error;
}

void shiftLeft(char* buffer, int shift, int stringlength) 
{
    for(int j = 0; j+shift <= stringlength; j++)
    {
        buffer[j] = buffer[j+shift];
    }
    return;
}



int transferEscapeSequences(char* buffer, int stringlength)
{
    //Procházíme string a hledáme escape sekvence
    for (int i = 0; i < stringlength; i++)
    {
        //92 == '\'
        if (buffer[i] == 92)
        {
            if ((i+1 < stringlength) && buffer[i+1] == 'n') 
            {
                //Na místo escape sekvence vložíme znak nového řádku
                buffer[i] = '\n';
                shiftLeft(&buffer[i+1], ESCAPE, stringlength - i -1);
                stringlength -= ESCAPE;
                continue;
            } else if((i+1 < stringlength) && buffer[i+1] == 't')
            {
                //Na místo escape sekvence vložime znak tabulátoru
                buffer[i] = '\t';
                shiftLeft(&buffer[i+1], ESCAPE, stringlength - i -1);
                stringlength -= ESCAPE;
                continue;
            }  else if((i+1 < stringlength) && buffer[i+1] == '$')
            {
                //Na místo escape sekvence vložíme znak dolaru
                buffer[i] = '$';
                shiftLeft(&buffer[i+1], ESCAPE, stringlength - i -1);
                stringlength -= ESCAPE;
                continue;
            } else if((i+1 < stringlength) && buffer[i+1] == 92)
            {
                //Na místo escape sekvence vložíme znak lomítka
                buffer[i] = 92;
                shiftLeft(&buffer[i+1], ESCAPE, stringlength - i -1);
                stringlength -= ESCAPE;
                continue;
            } else if((i+1 < stringlength) && buffer[i+1] == 'x')
            {
                if(i+3 > stringlength)
                {
                    continue;
                }
                //Konverze hex čísla na desítkové číslo
                int ascii_value = StrHexToDec(&buffer[i+2]);
                //Kontrola správnosti ASCII kódu
                if(ascii_value == ERRORRETURN)
                {
                    continue;
                }
                //Vložení ASCII kódu na místo escape sekvence
                buffer[i] = ascii_value;
                //Posunutíe ďalších znaků o 3 pozice doleva
                shiftLeft(&buffer[i+1], ESCAPEHEXA, stringlength - i-1);
                stringlength -= ESCAPEHEXA;
            }else if (i+3 < stringlength)
            {
                //Konverze z oktálového čísla na desítkové číslo
                int ascii_value = StrOctToDec(&buffer[i+1]);
                //Kontrola správnosti ASCII kódu               
                if(ascii_value == ERRORRETURN){
                    continue;
                }
                //Vložení ASCII kódu na místo escape sekvence
                buffer[i] = ascii_value;
                //Posunutí dalších znaků o 3 pozice doleva
                shiftLeft(&buffer[i+1], ESCAPEOCTA, stringlength - i-1);
                stringlength -= ESCAPEOCTA;
            }
            
        }
    }
    return stringlength;
}


Lexeme generateLexeme(AutomatState state, char* buffer, int stringlength, int row)
{
    //Vytvoření lexému
    Lexeme final_lexeme;
    final_lexeme.row = row;
    final_lexeme.negative_num = 0;
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
            stringlength-=2;    //Kvůli uvozovkám na začátku a na konci řetězce
            //Alokace paměti pro řetězec
            final_lexeme.extra_data.string =  malloc((stringlength)*sizeof(char));
            strncpy(final_lexeme.extra_data.string, buffer+1, (stringlength-1)*sizeof(char));
            final_lexeme.extra_data.string[stringlength-1] = '\0';
            break;
        case Number:
            final_lexeme.type = NUMBER;
            //Převedení řetězce na číslo a uložení do lexému
            final_lexeme.extra_data.value = atoi(buffer);
            break;
        case Decimal:
            final_lexeme.type = DECIMAL_NUMBER;
            //Převedení stringu na číslo a uložení do lexému
            if(buffer[stringlength-2] == '.')
            {
                error(row, "Desatinné číslo nemôže končiť s .", LEX_ERROR);
                final_lexeme.type = SCANERROR;
            }
            final_lexeme.extra_data.decimal = strtod(buffer, NULL);
            break;
        case Exponent:
            final_lexeme.type = EXPONENT_NUMBER;
            //Převedení řetězce na číslo a uložení do lexému
            final_lexeme.extra_data.exponent = atof(buffer);
            break;
        case Identifier:
            final_lexeme.type = FUNCTION_ID;
            //Alokace paměti pro řetězec
            final_lexeme.extra_data.string = malloc(stringlength*sizeof(char));
            strcpy(final_lexeme.extra_data.string, buffer);
            break;
        case LoadVar:
            final_lexeme.type = VARIABLE_ID;
            //Alokace paměti pro řetězec
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
            if((!(strcmp(buffer, "<?php\n"))))
            {
                final_lexeme.row--;
            }
            final_lexeme.type = PROLOG;
            break;
        case FileEndSign:
            final_lexeme.type = FILE_END_SIGN;
            break;
        case Start:
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
            final_lexeme.type = SCANERROR;
    }
    return final_lexeme;
}

Lexeme scan_lexeme(int *row, bool epilog, bool prolog)
{
    //Inicializace proměnných
    AutomatState current_state = Start;
    AutomatState next_state;
    int current_array_size = ARRAYSIZE;
    char *buffer = malloc(sizeof(char)*ARRAYSIZE);
    int stringlength = 0;
    char* current_index;
    current_index = buffer;
    char c;
    if(epilog)
    {
        c = getchar();
        if(c != EOF)
        {
            error(*row, "Po epilogu nesmie nasledovať žiadny znak.", SYNTAX_ERROR); 
        }
        return (Lexeme){.type = LEXEOF, .row = *row};
    } else if (!prolog)
    {
        c = getchar();
        if (c != '<')
        {
            error(*row, "Pred prologom sa nesmie nachádzať žiadny znak.", SYNTAX_ERROR); 
            return (Lexeme){.type = SCANERROR, .row = *row};
        }
        ungetc(c, stdin);
    }
    while(true)
    {
        c = getchar();
        //Pokud je znak nový řádek, inkrementace počítadla řádků
        if(c == '\n')
        {
            (*row)++;
        }
        if (c == '\r')
        {
            c = getchar();
            if (c == '\n')
            {
                (*row)++;
            }
        }
        //Escapování uvozovek
        if(c == '"')
        {
            // \" -> "
            if(buffer[stringlength-1] == '\\' && buffer[stringlength-2] != '\\')
            {
                buffer[stringlength-1] = c;
                continue;
            }
        } else if (c == '$' && current_state == String) //chyba, dolar nebyl zadán přes escape sekvenci
        {
            if (buffer[stringlength-1] != '\\')
            {
                free(buffer);
                return (Lexeme){.type = SCANERROR, .row = *row};
            }
        }
        //Přidání znaku do bufferu
        if ((c != ' ' || current_state == String || current_state == LineComment || current_state == BlockComment) && c != EOF)
            stringlength++;
        //Realokace paměti, pokud je buffer plný
        if (stringlength == current_array_size)
        {
            current_array_size += ARRAYSIZE;
            buffer = realloc(buffer, current_array_size);
            current_index = buffer + current_array_size - ARRAYSIZE - 1;
        }
        //Konec souboru
        if ( c == EOF)
        {
            if (current_state == Start)
            {
                return (Lexeme){.type = LEXEOF, .row = *row};
            }
            if (current_state == LineComment)
            {
                free(buffer);
                return (Lexeme){.type = LEXEOF, .row = *row};
            }
            *(current_index++) = '\0';
            stringlength++;
            return generateLexeme(current_state, buffer, stringlength, *row);
            stringlength = 0;
        }
        //Načtení prologu
        if (current_state == String && (!(strcmp(buffer, "<?php\n")) || !(strcmp(buffer, "<?php\t")) || !(strcmp(buffer, "<?php ")) || !(strcmp(buffer, "<?php/"))))
        {
            next_state = Prolog;
            if(c == '\n')
            {
                (*row)--;
            }
            if(c == '/')
            {
                ungetc(c, stdin);
            }
            if(c == '*')
            {
                ungetc(c, stdin);
                ungetc('/', stdin);
            }
            else
            {
                ungetc(c, stdin);
            }
        }else if (current_state == String && stringlength == 7)
        {
            if(!strncmp(buffer, "<?phpx", 5))
            {
                free(buffer);
                return (Lexeme){.type = SCANERROR, .row = *row};
            } else
            {
                next_state = transition(current_state, (char)c);
            }
        }
        else
        {
            //Přechod do dalšího stavu
            next_state = transition(current_state, (char)c);
        }
        if (next_state == Error)
        {
            if (c != ' ')
            {
                stringlength--;
            }
            if (current_state != BlockCommentPotentialEnd)
            {
                if(c == '\n')
                {
                    (*row)--;
                }
                ungetc((char)c, stdin);
            }
            
            *(current_index++) = '\0';
            stringlength++;
            current_index = buffer;
            if (current_state == LineComment || current_state == BlockCommentPotentialEnd)
            {
                free(buffer);
                Lexeme l;
                l.type = NULLLEX;
                l.row = *row;
                return l;
            }
            Lexeme l = generateLexeme(current_state, buffer, stringlength, *row);
            free(buffer);
            return l;
        } else if (next_state == ScanError)
        {
            free(buffer);
            Lexeme l;
            l.type = SCANERROR;
            l.row = *row;
            return l;
        }
        *(current_index++) = c;
        current_state = next_state;
        if ((next_state == Start))
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

    //Přehození typu lexému
    if (found != NULL)
    {
        if (strcmp(found->key, "while") == 0)
        {
            l->type = KW_WHILE;
        } else if (strcmp(found->key, "if") == 0)
        {
            l->type = KW_IF;
        } else if (strcmp(found->key, "else") == 0)
        {
            l->type = KW_ELSE;
        } else if (strcmp(found->key, "null") == 0)
        {
            l->type = KW_NULL;
        } else if (strcmp(found->key, "return") == 0)
        {
            l->type = KW_RETURN;
        } else if (strcmp(found->key, "void") == 0)
        {
            l->type = KW_VOID;
        } else if (strcmp(found->key, "int") == 0)
        {
            l->type = KW_INT;
        } else if (strcmp(found->key, "float") == 0)
        {
            l->type = KW_FLOAT;
        } else if (strcmp(found->key, "string") == 0)
        {
            l->type = KW_STRING;
        } else if (strcmp(found->key, "?int") == 0)
        {
            l->type = KW_OPTIONALINT;
        } else if (strcmp(found->key, "?float") == 0)
        {
            l->type = KW_OPTIONALFLOAT;
        } else if (strcmp(found->key, "?string") == 0)
        {
            l->type = KW_OPTIONALSTRING;
        } else if (strcmp(found->key, "function") == 0)
        {
            l->type = KW_FUNCTION;
        }
    }
}


Lexeme get_token(p_node binaryTree)
{
    Lexeme l = {.type = NULLLEX};
    static int row = 1;
    static bool epilog = false;
    static bool prolog = false;

    //Preskočení komentářů
    while(l.type == NULLLEX){
        l = scan_lexeme(&row, epilog, prolog);
    }

    if (l.type == FUNCTION_ID)
    {
        //Pokud je lexém klíčové slovo, změní se jeho typ
        check_forKW(binaryTree, &l);
    } else if(l.type == SCANERROR)
    {
        error(l.row, "Lexikální chyba", LEX_ERROR);
    } else if (l.type == FILE_END_SIGN)
    {
        epilog = true;
    } else if (l.type == PROLOG)
    {
        prolog = true;
    }

    return l;
}   