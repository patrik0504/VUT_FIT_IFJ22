#include "parser.h"



int parse(){
    int result = 0;
    p_node binaryTree = init_binary_treeKW();
    Lexeme l = {.type = NULLLEX};
    p_node globalFunctions = init_binary_treeKW();
    prog(l, binaryTree, globalFunctions);
    tree_destroy(binaryTree);
    return result;
}

int check_prolog(Lexeme l, p_node binaryTree)
{
    Dputs("Vosiel som do prologu\n");
    if(get_token().type != PROLOG)
    {
        Dputs("Nenaslo sa prolog\n"); 
        return 0;
    }
    int result = 0;
    l = get_token(binaryTree);
    Dprintf("TU SOM %d\n", l.type);
    if(l.type == FUNCTION_ID)
    {
        if(!strcmp(l.extra_data.string, "declare"))
        {
            
            if(get_token(binaryTree).type == LBRACKET){
                l = get_token(binaryTree);
                if (l.type == FUNCTION_ID)
                {
                    if(!strcmp(l.extra_data.string, "strict_types"))
                    {
                        if(get_token(binaryTree).type == EQUAL)
                        {
                            l = get_token(binaryTree);
                            if(l.type == NUMBER){
                                if(l.extra_data.value == 1)
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

int check_type(Lexeme l)
{
    int result = -1;
    switch (l.type)
    {
        case KW_INT:
        case KW_FLOAT:
        case KW_STRING:
        case KW_VOID:
            result = l.type;
            break;
        default:
            break;
    }
    return result;
}

int type(Lexeme l)
{
    int result = 0;
    if(check_type(l) != -1)
    {
        result = 1;
    }
    return result;
}

int decl_param(Lexeme l, p_node binaryTree, p_node globalFunctions)
{
    l = get_token(binaryTree);
    Dprintf("Vosiel som do function_args %d\n", l.type);
    int result = 0;
    if(type(l))
    {
       l = get_token(binaryTree);
        if(l.type == VARIABLE_ID)
        {
            result = decl_param2(l, binaryTree, globalFunctions);
        }
    }
    else if(l.type == RBRACKET)
    {
        result = 1;
    }
    return result;
}

int decl_param2(Lexeme l, p_node binaryTree, p_node globalFunctions)
{
    l = get_token(binaryTree);
    Dprintf("Vosiel som do function_args %d\n", l.type);
    int result = 0;
    if(l.type == COMMA)
    {
        l = get_token(binaryTree);
        if(type(l))
        {
            l = get_token(binaryTree);
            if(l.type == VARIABLE_ID)
            {
                result = decl_param2(l, binaryTree, globalFunctions);

            }
        }
    }
    else if(l.type == RBRACKET)
    {
        result = 1;
    }
    return result;
}

int function_check(Lexeme l, p_node binaryTree, p_node globalFunctions)
{
    l = get_token(binaryTree);

    int result = -1;
    if(tree_search(globalFunctions, l.extra_data.string) == NULL)
    {
        p_node node = node_init(NULL, l.extra_data.string);
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
                int type = check_type(get_token(binaryTree));
                if(type != -1)
                {
                    
                    if(get_token(binaryTree).type == LBRACKET_S_KUDRLINKOU)
                    {
                        //Docasne na preskocenie vnutra funkcie
                        while (l.type != RBRACKET_S_KUDRLINKOU)
                        {
                            l = get_token(binaryTree);
                        }
                        if(result != PARSER_ERROR)
                        {
                            Dputs("Funkcia je v poriadku\n");
                            result = 1;
                        }
                    }
                }
            }

        }
    }
    return result;
}

int if_check(Lexeme l, p_node binaryTree)
{
    int result = -1;
    l = get_token(binaryTree);
    if (l.type == LBRACKET)
    {
        if (get_token(binaryTree).type == VARIABLE_ID)
        {
            l = get_token(binaryTree);
            if (l.type == EQUAL3 || l.type == GREATER || l.type == GREATEREQUAL || l.type == LESS || l.type == LESSEQUAL )
            {
                if(get_token(binaryTree).type == NUMBER)
                {
                    if (get_token(binaryTree).type == RBRACKET)
                    {
                        if(get_token(binaryTree).type == LBRACKET_S_KUDRLINKOU)
                        {
                            result = 1;
                            printf("mam if!\n");
                        }
                    }
                }
            }
        }
    }
    return result;
}

int prog(Lexeme l, p_node binaryTree, p_node globalFunctions)
{
    return (check_prolog(l, binaryTree) && body(l, binaryTree, globalFunctions));
}

int body(Lexeme l, p_node binaryTree, p_node globalFunctions){
    int result = 0;
    l = get_token(binaryTree);
    switch (l.type) {
        case PROLOG:
            
            result = body(l, binaryTree, globalFunctions);
            break;
        case KW_FUNCTION:
            result = function_check(l, binaryTree, globalFunctions);
            if(result == -1)
            {
                //TODO ERROR
                return PARSER_ERROR;
            }
            result = body(l, binaryTree, globalFunctions);
            break;
        case KW_IF:
            result = if_check(l, binaryTree);
            if (result == -1)
            {
                return PARSER_ERROR;
            }
            result = body(l, binaryTree, globalFunctions);
            break;
        default:
            result = 0;
            break;
    }
    return result;
}

