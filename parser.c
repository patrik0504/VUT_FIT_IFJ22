#include "parser.h"

int parse(){
    int result = 0;
    p_node binaryTree = init_binary_treeKW();
    Lexeme l = {.type = NULLLEX};
    p_node globalFunctions = init_binary_treeKW();
    program(l, binaryTree, globalFunctions);
    tree_destroy(binaryTree);
    return result;
}

int check_prolog(Lexeme l, p_node binaryTree)
{
    int result = 0;
    l = get_token(binaryTree);
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
            //TODO: check parameters
            if(get_token(binaryTree).type == RBRACKET)
            {
                if(get_token(binaryTree).type == COLON)
                {
                    int type = check_type(get_token(binaryTree));
                    if(type != -1)
                    {
                        
                        if(get_token(binaryTree).type == LBRACKET_S_KUDRLINKOU)
                        {
                            //call something
                            if(result != PARSER_ERROR)
                            {
                                printf("jsem tu.\n");
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

int program(Lexeme l, p_node binaryTree, p_node globalFunctions){
    
    int result = 0;
    l = get_token(binaryTree);
    printf("program\n");
    switch (l.type) {
        case PROLOG:
            if(check_prolog(l, binaryTree) != 1)
            {
                //TODO ERROR
                return PARSER_ERROR;
            }
            result = program(l, binaryTree, globalFunctions);
            break;
        case KW_FUNCTION:
            result = function_check(l, binaryTree, globalFunctions);
            if(result == -1)
            {
                //TODO ERROR
                return PARSER_ERROR;
            }
            result = program(l, binaryTree, globalFunctions);
            break;
        default:
            result = 0;
            break;
    }
    return result;
}

