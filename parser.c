#include "parser.h"



int parse(){
    int result = 0;
    p_node binaryTree = init_binary_treeKW();
    Lexeme * l = token_init(); 
    p_node globalFunctions = init_binary_treeKW();
    prog(l, binaryTree, globalFunctions);
    //debug_tree(globalFunctions);

    //Len pre testovanie
    p_node found = tree_search(globalFunctions, "test2");
    if(found != NULL){
        debug_tree(found->data->params);
        printf("\n");
    }
    else{
        printf("Not found\n");
    }
    //--------------------------------
    tree_destroy(binaryTree);
    //Doplit funkciu na znicenie vnutra stromu globalFunctions. Resp. pozriet ze ci to robi tree_destroy
    tree_destroy(globalFunctions);
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
    char * function_name = l->extra_data.string;
    *l = get_token(binaryTree);
    int result = 0;
    if(type(l))
    {
        int type = l->type;
       *l = get_token(binaryTree);
        if(l->type == VARIABLE_ID)
        {
            p_node node = tree_search(globalFunctions, function_name);
            p_data data = data_init_type(type);
            p_node param = node_init(data, l->extra_data.string);
            node->data->params = param;
            result = decl_param2(l, binaryTree, globalFunctions, node);
        }
    }
    else if(l->type == RBRACKET)
    {
        result = 1;
    }
    return result;
}

int decl_param2(Lexeme *l, p_node binaryTree, p_node globalFunctions, p_node function_node)
{
    *l = get_token(binaryTree);
    int result = 0;
    if(l->type == COMMA)
    {
        *l = get_token(binaryTree);
        if(type(l))
        {
            int type = l->type;
            *l = get_token(binaryTree);
            if(l->type == VARIABLE_ID)
            {
                //p_node node = tree_search(globalFunctions, function_name);
                p_data data = data_init_type(type);
                p_node param = node_init(data, l->extra_data.string);
                insert_node(function_node->data->params, param);
                result = decl_param2(l, binaryTree, globalFunctions, function_node);

            }
        }
    }
    else if(l->type == RBRACKET)
    {
        result = 1;
    }
    return result;
}

int check_if_variable_is_defined(p_node functionPtr, char * variable)
{
    int result = 0;
    if (((functionPtr->data->params != NULL) && (tree_search(functionPtr->data->params, variable) == NULL)) || (functionPtr->data->params == NULL))
    {
        if(((functionPtr->data->elements != NULL) && (tree_search(functionPtr->data->elements, variable) == NULL)) || (functionPtr->data->elements == NULL))
        {
            Dprintf("promenna %s neni ve funkci definovana :(\n", variable);
        } else
        {      
            Dprintf("promenna %s je ve funkci definovana :)\n", variable);
            result = 1;
        }
    }
    else
    {
        Dprintf("promenna %s je ve funkci definovana :)\n", variable);
        result = 1;
    }
    return result;
}

int statement(Lexeme *l, p_node binaryTree, p_node globalFunctions, bool comesFromFunction, p_node functionPtr)
{
    int result = 0;
    if (l->type == VARIABLE_ID)
    {
        *l = get_token(binaryTree);
        while(l->type != SEMICOLON){
           *l = get_token(binaryTree);
        }
        printf("statement prosel v poradku!\n");
        result = 1;
    } else if (l->type == FUNCTION_ID)
    {
        char * function_name = l->extra_data.string;
        p_node node = tree_search(globalFunctions, function_name);
        if(node == NULL)
        {
            Dprintf("Funkcia %s neexistuje\n", function_name);
            return PARSER_ERROR;
        }
        Dprintf("Funkcia %s existuje\n", function_name);
        *l = get_token(binaryTree);
        
        if(l->type == LBRACKET)
        {
            *l = get_token(binaryTree);
            result = param(l, binaryTree, comesFromFunction, functionPtr);
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

int st_list(Lexeme *l, p_node binaryTree, p_node globalFunctions, bool comesFromFunction, p_node functionPtr)
{
    int result = 0;
    if(l->type == VARIABLE_ID)
    {
        
        if(comesFromFunction)
        { 
                if(((functionPtr->data->params != NULL) && (tree_search(functionPtr->data->params, l->extra_data.string) == NULL)) || (functionPtr->data->params == NULL))
                {
                    if (((functionPtr->data->elements != NULL) && (tree_search(functionPtr->data->elements, l->extra_data.string) == NULL)) || (functionPtr->data->elements == NULL))
                    {
                        p_node local_var = node_init(NULL, l->extra_data.string);
                        if (functionPtr->data->elements == NULL)
                        {
                            functionPtr->data->elements = local_var;
                        } else
                        {
                            insert_node(functionPtr->data->elements, local_var);
                        }
                        Dprintf("nasel jsem lokalni promennou %s\n", l->extra_data.string);
                    }
                } else
                {
                    Dprintf("promenna %s je parametrem funkce\n", l->extra_data.string);
                }
        }
        result = statement(l, binaryTree, globalFunctions, comesFromFunction, functionPtr);
        if(result)
        {
            if(l->type == SEMICOLON)
            {
                *l = get_token(binaryTree);
                if (comesFromFunction == 1)
                {
                   result = st_list(l, binaryTree, globalFunctions, 1, functionPtr);
            
                } else
                {
                    result = st_list(l, binaryTree, globalFunctions, 0, NULL);
                }
            }
            else
            {
                result = PARSER_ERROR;
            }
        }
    }
    else if (l->type == FUNCTION_ID)
    {
        result = statement(l, binaryTree, globalFunctions, comesFromFunction, functionPtr);
        if(result)
        {
            *l = get_token(binaryTree);
            if(l->type == SEMICOLON)
            {
                *l = get_token(binaryTree);
                Dputs("Nacital som spravne volanie funkcie\n");
                result = st_list(l, binaryTree, globalFunctions, 0, NULL);
            }
            else
            {
                result = PARSER_ERROR;
                return result;
            }
        }
    }else if(l->type == KW_IF){
        Dputs("Nasiel som if v st_list\n");
        result = control(l, binaryTree, globalFunctions);
        if (result)
        {
            *l = get_token(binaryTree);
            result = st_list(l, binaryTree, globalFunctions, 0, NULL);
        }
    }else if(l->type == KW_WHILE){
        result = control(l, binaryTree, globalFunctions);
        if (result)
        {
            *l = get_token(binaryTree);
            result = st_list(l, binaryTree, globalFunctions, 0, NULL);
        }
    }
    result = 1;
    return result;
}

int param(Lexeme *l, p_node binaryTree, bool comesFromFunction, p_node functionPtr)
{
    int result = 0;
    if(l->type == VARIABLE_ID)
    { 
            if (check_if_variable_is_defined(functionPtr, l->extra_data.string) == 1)
            {
                *l = get_token(binaryTree);
                result = param2(l, binaryTree, comesFromFunction, functionPtr);
            } else
            {
                return PARSER_ERROR;
            }

    }
    else if(l->type == RBRACKET)
    {
        result = 1;
    }
    return result;
}

int param2(Lexeme *l, p_node binaryTree, bool comesFromFunction, p_node functionPtr)
{
    int result = 0;
    if(l->type == COMMA)
    {
        *l = get_token(binaryTree);
        if(l->type == VARIABLE_ID)
        {
            if (check_if_variable_is_defined(functionPtr, l->extra_data.string) == 1)
            {
                *l = get_token(binaryTree);
                result = param2(l, binaryTree, comesFromFunction, functionPtr);
            } else
            {
                return PARSER_ERROR;
            }
        }
    }
    else if(l->type == RBRACKET)
    {
        result = 1;
    }
    return result;
}

int while_check(Lexeme *l, p_node binaryTree, p_node globalFunctions)
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
            result = st_list(l, binaryTree, globalFunctions, 0, NULL);
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

int control(Lexeme *l, p_node binaryTree, p_node globalFunctions)
{
    int result = 0;
    if(l->type == KW_IF)
    {
        result = if_check(l, binaryTree, globalFunctions);
    }
    else if(l->type == KW_WHILE)
    {
        result = while_check(l, binaryTree, globalFunctions);
    }
    return result;
}

int function_check(Lexeme *l, p_node binaryTree, p_node globalFunctions)
{
    *l = get_token(binaryTree);

    int result = -1;
    char * func_name = l->extra_data.string;
    if(tree_search(globalFunctions, l->extra_data.string) == NULL)
    {
        //p_node node = node_init(NULL, l->extra_data.string);
        //insert_node(globalFunctions, node);
        insert_function(globalFunctions, l);
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
                    p_node node = tree_search(globalFunctions, func_name);
                    if(node != NULL)
                    {
                        node->data->func_type = lexeme_type_to_type(l);
                    }
                    *l = get_token(binaryTree);
                    if(l->type == LBRACKET_S_KUDRLINKOU)
                    {
                        *l = get_token(binaryTree);
                        result = st_list(l, binaryTree, globalFunctions, 1, node);
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

int if_check(Lexeme *l, p_node binaryTree, p_node globalFunctions)
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
            result = st_list(l, binaryTree, globalFunctions, 0, NULL);
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
                            result = st_list(l, binaryTree, globalFunctions, 0, NULL);
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
        case LEXEOF:
            result = 1;
            break;
        case KW_IF:
        case KW_WHILE:
            result = control(l, binaryTree, globalFunctions);
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
            result = statement(l, binaryTree, globalFunctions, 0, NULL);
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


//Insert new function to globalFunctions
void insert_function(p_node globalFunctions, Lexeme *l)
{
    p_node node = node_init(data_init(), l->extra_data.string);
    insert_node(globalFunctions, node);
}

//Incializacia data pre globalFunctions
p_data data_init()
{
    p_data data = (p_data)malloc(sizeof(struct data));
    data->declared = false;
    data->defined = false;
    data->func_type = -1;
    data->params = NULL;
    data->elements = NULL;
    return data;
}
p_data data_init_type(int type)
{
    switch (type) {
        case KW_INT:
            type = INT;
            break;
        case KW_FLOAT:
            type = FLOAT;
            break;
        case KW_STRING:
            type = STRING;
            break;
        default:
            type = -1;
            break;
    }
    p_data data = (p_data)malloc(sizeof(struct data));
    data->declared = false;
    data->defined = false;
    data->func_type = type;
    data->params = NULL;
    data->elements = NULL;
    return data;
}
int lexeme_type_to_type(Lexeme *l)
{
    switch (l->type) {
        case KW_INT:
            return INT;
            break;
        case KW_FLOAT:
            return FLOAT;
            break;
        case KW_STRING:
            return STRING;
            break;
        case KW_VOID:
            return VOID;
            break;
        default:
            return -1;
            break;
    }
}

