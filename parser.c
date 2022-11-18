#include "parser.h"



int parse(){
    int result = 0;
    p_node binaryTree = init_binary_treeKW();
    Lexeme * l = token_init(); 
    p_node globalFunctions = init_binary_treeKW();
    prog(l, binaryTree, globalFunctions);
    tree_destroy(binaryTree);
    token_free(l);
    return result;
}

int check_prolog(Lexeme *l, p_node binaryTree)
{
    Dputs("Vosiel som do prologu\n");
    if(get_token().type != PROLOG)
    {
        Dputs("Nenaslo sa prolog\n"); 
        return 0;
    }
    int result = 0;
    *l = get_token(binaryTree);
    Dprintf("TU SOM %d\n", l->type);
    if(l->type == FUNCTION_ID)
    {
        if(!strcmp(l->extra_data.string, "declare"))
        {
            
            if(get_token(binaryTree).type == LBRACKET){
                *l = get_token(binaryTree);
                if (l->type == FUNCTION_ID)
                {
                    if(!strcmp(l->extra_data.string, "strict_types"))
                    {
                        if(get_token(binaryTree).type == EQUAL)
                        {
                            *l = get_token(binaryTree);
                            if(l->type == NUMBER){
                                if(l->extra_data.value == 1)
                                {
                                    if(get_token(binaryTree).type == RBRACKET)
                                    {
                                        if(get_token(binaryTree).type == SEMICOLON)
                                        {
                                            Dputs("Prolog je v poriadku\n");
                                            result = 1;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }       
        }
    }
    return result;
}

int check_type(Lexeme *l)
{
    int result = -1;
    switch (l->type)
    {
        case KW_INT:
        case KW_FLOAT:
        case KW_STRING:
        case KW_VOID:
            result = l->type;
            break;
        default:
            break;
    }
    return result;
}

int type(Lexeme *l)
{
    int result = 0;
    if(check_type(l) != -1)
    {
        result = 1;
    }
    return result;
}

int decl_param(Lexeme *l, p_node binaryTree, p_node globalFunctions)
{
    *l = get_token(binaryTree);
    int result = 0;
    if(type(l))
    {
       *l = get_token(binaryTree);
        if(l->type == VARIABLE_ID)
        {
            result = decl_param2(l, binaryTree, globalFunctions);
        }
    }
    else if(l->type == RBRACKET)
    {
        result = 1;
    }
    return result;
}

int decl_param2(Lexeme *l, p_node binaryTree, p_node globalFunctions)
{
    *l = get_token(binaryTree);
    int result = 0;
    if(l->type == COMMA)
    {
        *l = get_token(binaryTree);
        if(type(l))
        {
            *l = get_token(binaryTree);
            if(l->type == VARIABLE_ID)
            {
                result = decl_param2(l, binaryTree, globalFunctions);

            }
        }
    }
    else if(l->type == RBRACKET)
    {
        result = 1;
    }
    return result;
}

int statement(Lexeme *l, p_node binaryTree)
{
    int result = 0;
    if (l->type == VARIABLE_ID)
    {
        *l = get_token(binaryTree);
        while(l->type != SEMICOLON){
           *l = get_token(binaryTree);
        }
        //velmi docasne riesenie vymazat
        //ungetc(';', stdin);
        printf("statement prosel v poradku!\n");
        result = 1;
    } else if (l->type == FUNCTION_ID)
    {
        *l = get_token(binaryTree);
        
        if(l->type == LBRACKET)
        {
            *l = get_token(binaryTree);
            result = param(l, binaryTree);
            if(result)
            {
                if(l->type == RBRACKET)
                {
                    Dputs("Nasiel som volanie funkcie v statemente\n");
                    result = 1;
                }
            }
        }
    } else if(l->type == KW_RETURN)
    {
        result = ret_expr(l, binaryTree);
        if(result)
        {
            Dputs("Nasiel som return v statemente\n");
            result = 1;

        }
    }
    return result;
}

int ret_expr(Lexeme *l, p_node binaryTree)
{
    int result = 0;
    *l = get_token(binaryTree);
    if(l->type == SEMICOLON)
    {
        result = 1;
    }
    else{
        //Call expr
        while (l->type != SEMICOLON)
        {
            *l = get_token(binaryTree);
        }
        result = 1;
    }
    return result;
}

int st_list(Lexeme *l, p_node binaryTree)
{
    int result = 0;
    if(l->type == VARIABLE_ID)
    {
        result = statement(l, binaryTree);
        if(result)
        {
            if(l->type == SEMICOLON)
            {
                *l = get_token(binaryTree);
                result = st_list(l, binaryTree);
            }
            else
            {
                result = PARSER_ERROR;
            }
        }
    }
    else if (l->type == FUNCTION_ID)
    {
        result = statement(l, binaryTree);
        if(result)
        {
            *l = get_token(binaryTree);
            if(l->type == SEMICOLON)
            {
                *l = get_token(binaryTree);
                Dputs("Nacital som spravne volanie funkcie\n");
                result = st_list(l, binaryTree);
            }
            else
            {
                result = PARSER_ERROR;
                return result;
            }
        }
    }
    result = 1;
    return result;
}

int param(Lexeme *l, p_node binaryTree)
{
    int result = 0;
    if(l->type == VARIABLE_ID)
    {   
        *l = get_token(binaryTree);
        result = param2(l, binaryTree);
    }
    else if(l->type == RBRACKET)
    {
        result = 1;
    }
    return result;
}

int param2(Lexeme *l, p_node binaryTree)
{
    int result = 0;
    if(l->type == COMMA)
    {
        *l = get_token(binaryTree);
        if(l->type == VARIABLE_ID)
        {
            *l = get_token(binaryTree);
            result = param2(l, binaryTree);
        }
    }
    else if(l->type == RBRACKET)
    {
        result = 1;
    }
    return result;
}

int while_check(Lexeme *l, p_node binaryTree)
{
    
    int result = 0;
    *l = get_token(binaryTree);
    if(l->type == LBRACKET)
    {
        //Call expression
        *l = get_token(binaryTree);
        while (l->type != RBRACKET)
        {
            *l = get_token(binaryTree);
        }
        *l = get_token(binaryTree);
        if(l->type == LBRACKET_S_KUDRLINKOU)
        {
            *l = get_token(binaryTree);
            result = st_list(l, binaryTree);
            if (result)
            {
                if(l->type == RBRACKET_S_KUDRLINKOU){
                    Dputs("While je v poriadku\n");
                    result = 1;
                }
            }
        }
    }
    return result;
}

int control(Lexeme *l, p_node binaryTree)
{
    int result = 0;
    if(l->type == KW_IF)
    {
        result = if_check(l, binaryTree);
    }
    else if(l->type == KW_WHILE)
    {
        result = while_check(l, binaryTree);
    }
    return result;
}

int function_check(Lexeme *l, p_node binaryTree, p_node globalFunctions)
{
    *l = get_token(binaryTree);

    int result = -1;
    if(tree_search(globalFunctions, l->extra_data.string) == NULL)
    {
        p_node node = node_init(NULL, l->extra_data.string);
        insert_node(globalFunctions, node);
        if(get_token(binaryTree).type == LBRACKET)
        {
            int result_args = decl_param(l, binaryTree, globalFunctions);
            if (result_args != 1)
            {
                return result_args;
            }
            
            if(get_token(binaryTree).type == COLON)
            {
                *l = get_token(binaryTree);
                int type = check_type(l);
                if(type != -1)
                {
                    *l = get_token(binaryTree);
                    if(l->type == LBRACKET_S_KUDRLINKOU)
                    {
                        *l = get_token(binaryTree);
                        result = st_list(l, binaryTree);
                        if(result)
                        {
                            if(l->type == RBRACKET_S_KUDRLINKOU)
                            {
                                Dputs("Funkcia je v poriadku\n");
                                result = 1;
                            }
                        }
                    }
                }
            }

        }
    }
    return result;
}

int if_check(Lexeme *l, p_node binaryTree)
{
    int result = 0;
    *l = get_token(binaryTree);
    if (l->type == LBRACKET)
    {
        //Call expression
        *l = get_token(binaryTree);
        while (l->type != RBRACKET)
        {
            *l = get_token(binaryTree);
        }
        *l = get_token(binaryTree);
        if(l->type == LBRACKET_S_KUDRLINKOU)
        {
            *l = get_token(binaryTree);
            result = st_list(l, binaryTree);
            if (result)
            {
                if(l->type == RBRACKET_S_KUDRLINKOU)
                {
                    *l = get_token(binaryTree);
                    if(l->type == KW_ELSE)
                    {
                        *l = get_token(binaryTree);
                        if(l->type == LBRACKET_S_KUDRLINKOU)
                        {
                            *l = get_token(binaryTree);
                            result = st_list(l, binaryTree);
                            if (result)
                            {
                                if(l->type == RBRACKET_S_KUDRLINKOU)
                                {
                                    Dputs("If je v poriadku\n");
                                    result = 1;
                                }
                            }
                        }
                    }
                }
            }
        }
        
    }
    return result;
}

int prog(Lexeme *l, p_node binaryTree, p_node globalFunctions)
{
    return (check_prolog(l, binaryTree) && body(l, binaryTree, globalFunctions));
}

int body(Lexeme *l, p_node binaryTree, p_node globalFunctions){
    int result = 0;
    *l = get_token(binaryTree);
    switch (l->type) {
        case FILE_END_SIGN:
            *l = get_token(binaryTree);
            if(l->type == LEXEOF)
            {
                result = 1;
            }
        case KW_FUNCTION:
            result = function_check(l, binaryTree, globalFunctions);
            if(result == -1)
            {
                //TODO ERROR
                return PARSER_ERROR;
            }
            result = body(l, binaryTree, globalFunctions);
            break;
        case LEXEOF:
            result = 1;
            break;
        case KW_IF:
        case KW_WHILE:
            result = control(l, binaryTree);
            if(result == -1)
            {
                //TODO ERROR
                return PARSER_ERROR;
            }
            result = body(l, binaryTree, globalFunctions);
            break;
        //Chyba tu expression - neviem ako to spravit
        case VARIABLE_ID:
        case FUNCTION_ID:
        case KW_RETURN:
            result = statement(l, binaryTree);
            if(result == -1)
            {
                //TODO ERROR
                return PARSER_ERROR;
            }
            if(l->type == SEMICOLON)
            {
                result = body(l, binaryTree, globalFunctions);
            }
            //Docasne kvoli bugu v statement lebo sa semicolon nema nacitavat a to je problem pri expression
            else{
                *l = get_token(binaryTree);
                if(l->type == SEMICOLON)
                {
                    result = body(l, binaryTree, globalFunctions);
                }
            }
            break;
    }
    return result;
}


Lexeme * token_init()
{
    Lexeme * token = malloc(sizeof(Lexeme));
    token->type = NULLLEX;
    token->extra_data.string = NULL;
    return token;
}

void token_free(Lexeme * token)
{
    if(token != NULL)
    {
        free(token);
    }
}
