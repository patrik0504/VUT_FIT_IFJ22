#include "parser.h"

int parse(){
    int result = 0;
    p_node binaryTree = init_binary_treeKW();
    Lexeme * l = token_init(); 
    p_node globalFunctions = init_global_function();
    prog(l, binaryTree, globalFunctions);
    int err = 0;
    check_func(globalFunctions, &err);
    if(err == 1){
        error(-1, "Funkcia bola zavolaná, ale nebola deklarovaná", SEM_UNDEFINED_FUNC_ERROR);
        return 0;
    }
    //debug_tree(globalFunctions);
    /*
    //Len pre testovanie
        p_node found = tree_search(globalFunctions, "test2");
        if(found != NULL){
            debug_tree(found->data->params);
            printf("\n");
            Dprintf("Number of leaves: %d\n", count_tree(found->data->params));
            if(found->data->elements != NULL){
                debug_tree(found->data->elements);
                printf("\n");
                Dprintf("Number of leaves: %d\n", count_tree(found->data->elements));
            }
        }
        else{
            Dputs("Not found test2\n");
        }
        //debug tree elements globalono/lokalne
        if(globalFunctions->data->elements != NULL){
            debug_tree(globalFunctions->data->elements);
            printf("\n");
            Dprintf("Number of leaves: %d\n", count_tree(globalFunctions->data->elements));
        }
    //--------------------------------
    */
    tree_destroy(binaryTree);
    //Doplit funkciu na znicenie vnutra stromu globalFunctions. Resp. pozriet ze ci to robi tree_destroy
    tree_destroy(globalFunctions);
    token_free(l);
    return result;
}

int check_prolog(Lexeme *l, p_node binaryTree)
{
    if(get_token(binaryTree).type != PROLOG)
    {
        error(1, "Nebol najdený <?php", SYNTAX_ERROR);
        return 0;
    }
    int result = 0;
    *l = get_token(binaryTree);
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
    if(!result)
    {
        error(1, "Prolog je v zlom formate", SYNTAX_ERROR);
        return 0;
    }
    return result;
}

int check_type(Lexeme *l)
{
    int result = 0;
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
    if(check_type(l) != 0)
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
        p_node func = tree_search(globalFunctions, function_name);
        if (func->data->defined)
        {
            if(func->data->param_count == count_tree(func->data->params))
            {
                result = 1;
            }
        }
        else
        {
            result = 1;
        }
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
        p_node func = function_node;
        if (func->data->defined)
        {
            if(func->data->param_count == count_tree(func->data->params))
            {
                result = 1;
            }
        }
        else
        {
            result = 1;
        }
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
            //Promena nie je vo funkcii definovana
            result = 0;
        } else
        {   
            //Promena je vo funkcii definovana   
            result = 1;
        }
    }
    else
    {
        //Promena je vo funkcii definovana    
        result = 1;
    }
    return result;
}

int statement(Lexeme *l, p_node binaryTree, p_node globalFunctions, bool comesFromFunction, p_node functionPtr)
{
    int result = 0;
    if (l->type == VARIABLE_ID)
    {
        if(!comesFromFunction)
        {
            if (((globalFunctions->data->elements != NULL) && (tree_search(globalFunctions->data->elements, l->extra_data.string) == NULL)) || (globalFunctions->data->elements == NULL))
            {
                p_data data = data_init();
                p_node local_var = node_init(data, l->extra_data.string);
                if (globalFunctions->data->elements == NULL)
                {
                    globalFunctions->data->elements = local_var;
                } else
                {
                    insert_node(globalFunctions->data->elements, local_var);
                }
            }
        }
        *l = get_token(binaryTree);
        result = expr(ASSIGNMENT, binaryTree, l);
        if(result != 1)
        {
            return result;
        }
        //Docasne lebo to nevracia tak ako ma l->type
        l->type = SEMICOLON;
        result = 1;
    } else if (l->type == FUNCTION_ID)
    {
        char * function_name = l->extra_data.string;
        p_node node = tree_search(globalFunctions, function_name);
        if(node == NULL)
        {
            if(comesFromFunction)
            {
                p_data data = data_init();
                node = node_init(data, function_name);
                node->data->defined = true;
                insert_node(globalFunctions, node);
            } else
            {
                error(l->row, "Funkcia nie je definovana", SEM_UNDEFINED_FUNC_ERROR);
                return 0;
            }
        }
        *l = get_token(binaryTree);
        
        if(l->type == LBRACKET)
        {
            *l = get_token(binaryTree);
            result = param(l, binaryTree, comesFromFunction, functionPtr, node, globalFunctions);
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
    result = expr(RETURN, binaryTree, l);
    if(!result)
    {
        return result;
    }
    //Docasne kvoli bugu v expr
    l->type = SEMICOLON;
    result = 1;
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
                        p_data data = data_init();
                        p_node local_var = node_init(data, l->extra_data.string);
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
        else
        {
            if (((globalFunctions->data->elements != NULL) && (tree_search(globalFunctions->data->elements, l->extra_data.string) == NULL)) || (globalFunctions->data->elements == NULL))
            {
                p_data data = data_init();
                p_node local_var = node_init(data, l->extra_data.string);
                if (globalFunctions->data->elements == NULL)
                {
                    globalFunctions->data->elements = local_var;
                } else
                {
                    insert_node(globalFunctions->data->elements, local_var);
                }
                Dprintf("nasel jsem globalni promennou %s\n", l->extra_data.string);
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
                result = st_list(l, binaryTree, globalFunctions, comesFromFunction, functionPtr);
            }
            else
            {
                result = 0;
                return result;
            }
        }
    }else if(l->type == KW_IF){
        Dputs("Nasiel som if v st_list\n");
        result = control(l, binaryTree, globalFunctions, comesFromFunction, functionPtr);
        if (result)
        {
            *l = get_token(binaryTree);
            result = st_list(l, binaryTree, globalFunctions, comesFromFunction, functionPtr);
        }
    }else if(l->type == KW_WHILE){
        result = control(l, binaryTree, globalFunctions, comesFromFunction, functionPtr);
        if (result)
        {
            *l = get_token(binaryTree);
            result = st_list(l, binaryTree, globalFunctions, comesFromFunction, functionPtr);
        }
    }else if(l->type == KW_RETURN)
    {
        result = statement(l, binaryTree, globalFunctions, comesFromFunction, functionPtr);
        if(result)
        {
            Dputs("Nasiel som return v st_list\n");
            *l = get_token(binaryTree);
            result = st_list(l, binaryTree, globalFunctions, comesFromFunction, functionPtr);
        }
    }
    result = 1;
    return result;
}

int param(Lexeme *l, p_node binaryTree, bool comesFromFunction, p_node functionPtr, p_node callFunction, p_node globalFunctions)
{
    int param_count = 0;
    int result = 0;
    
    if(l->type == VARIABLE_ID)
    { 
        if((count_tree(callFunction->data->params) == 0) && (callFunction->data->declared)){
            Dprintf("Funkce %s nema zadne parametry a boli jej hodené\n", callFunction->key);
            error(l->row, "Funkce nema zadne parametry a boli jej hodené", SEM_INVALID_CALL_ERROR);
            return 0;
        } else if (callFunction->data->defined && !callFunction->data->declared )
        {
            Dprintf("pricitam parametr %s\n", l->extra_data.string);
            callFunction->data->param_count++;
        }

        if(comesFromFunction)
        {
            if (check_if_variable_is_defined(functionPtr, l->extra_data.string) == 1)
            {
                param_count++;
                *l = get_token(binaryTree);
                result = param2(l, binaryTree, comesFromFunction, functionPtr, param_count, callFunction, globalFunctions);
            } else
            {
                error(l->row, "Promenna neni definovana", SEM_UNDEFINED_VAR_ERROR);
                return 0;
            }
        }
        else
        {
            if(tree_search(globalFunctions->data->elements, l->extra_data.string) != NULL)
            {
                param_count++;
                *l = get_token(binaryTree);
                result = param2(l, binaryTree, comesFromFunction, functionPtr, param_count, callFunction, globalFunctions);
            }
            else
            {
                Dprintf("Promenna %s neni definovana\n", l->extra_data.string);
                error(l->row, "Promenna neni definovana", SEM_UNDEFINED_VAR_ERROR);
                return 0;
            }
        }
    
    }
    else if(l->type == RBRACKET)
    {
        if(callFunction->data->defined && callFunction->data->declared)
        {
            if(param_count == count_tree(callFunction->data->params))
            {
                result = 1;
            }
            else
            {
                Dprintf("Zlý počet parametrov, očakávalo se %d, ale bylo zadáno %d\n", count_tree(callFunction->data->params), param_count);
                error(l->row, "Zlý počet parametrov", SEM_INVALID_CALL_ERROR);
                result = 0;
            }
        } else
        {
            result = 1;
        }
    }
        
    return result;
}

int param2(Lexeme *l, p_node binaryTree, bool comesFromFunction, p_node functionPtr, int paramCount, p_node callFunction, p_node globalFunctions)
{
    int result = 0;
    if(l->type == COMMA)
    {
        *l = get_token(binaryTree);
        if(l->type == VARIABLE_ID)
        {
            if (callFunction->data->defined && !callFunction->data->declared )
            {
                Dprintf("pricitam parametr %s\n", l->extra_data.string);
                callFunction->data->param_count++;
            }
            if(comesFromFunction)
            {
                if (check_if_variable_is_defined(functionPtr, l->extra_data.string) == 1)
                {
                    paramCount++;
                    *l = get_token(binaryTree);
                    result = param2(l, binaryTree, comesFromFunction, functionPtr, paramCount, callFunction, globalFunctions);
                } else
                {
                    error(l->row, "Promenna neni definovana", SEM_UNDEFINED_VAR_ERROR);
                    return 0;
                }
            }
            else
            {
                if(tree_search(globalFunctions->data->elements, l->extra_data.string) != NULL)
                {
                    paramCount++;
                    *l = get_token(binaryTree);
                    result = param2(l, binaryTree, comesFromFunction, functionPtr, paramCount, callFunction, globalFunctions);
                }
                else
                {
                    error(l->row, "Promenna neni definovana", SEM_UNDEFINED_VAR_ERROR);
                    return 0;
                }
            }
            
        }
    }
    else if(l->type == RBRACKET)
    {
        if(callFunction->data->defined && callFunction->data->declared)
        {
            debug_tree(callFunction->data->params);
            printf("\n");
            if(paramCount == count_tree(callFunction->data->params))
            {
                Dprintf("Nasiel som %d parametrov\n", paramCount);
                result = 1;
            }
            else
            {
                error(l->row, "Zlý počet parametrov", SEM_INVALID_CALL_ERROR);
                result = 0;
            }
        }else
        {
            result = 1;
        }
    }
    return result;
}

int while_check(Lexeme *l, p_node binaryTree, p_node globalFunctions, bool comesFromFunction, p_node functionPtr)
{
    
    int result = 0;
    *l = get_token(binaryTree);
    if(l->type == LBRACKET)
    {
        //Call expression
        result = expr(CALL_CONTROL, binaryTree, l);
        if(!result)
        {
            return result;
        }
        Dprintf("Typ vyrazu v ife je %d\n", l->type);
        *l = get_token(binaryTree);
        if(l->type == LBRACKET_S_KUDRLINKOU)
        {
            *l = get_token(binaryTree);
            result = st_list(l, binaryTree, globalFunctions, comesFromFunction, functionPtr);
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

int control(Lexeme *l, p_node binaryTree, p_node globalFunctions, bool comesFromFunction, p_node functionPtr)
{
    int result = 0;
    if(l->type == KW_IF)
    {
        result = if_check(l, binaryTree, globalFunctions, comesFromFunction, functionPtr);
    }
    else if(l->type == KW_WHILE)
    {
        result = while_check(l, binaryTree, globalFunctions, comesFromFunction, functionPtr);
    }
    return result;
}

int function_check(Lexeme *l, p_node binaryTree, p_node globalFunctions)
{
    *l = get_token(binaryTree);

    int result = 0;
    char * func_name = l->extra_data.string;
    p_node root = tree_search(globalFunctions, l->extra_data.string);
    Dprintf("Funkcia %s\n", l->extra_data.string);
    if(root == NULL)
    {
        insert_function(globalFunctions, l);
        if(get_token(binaryTree).type == LBRACKET)
        {
            int result_args = decl_param(l, binaryTree, globalFunctions);
            if (result_args != 1)
            {
                Dputs("Chyba pri deklaracii parametrov\n");
                return 0;
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
                                node->data->declared = true;
                                node->data->defined = true;
                                Dputs("Funkcia je v poriadku\n");
                                result = 1;
                            }
                        }
                    }
                }
            }
        }
    } else if (root->data->defined && !root->data->declared)
    {
        Dprintf("Funkcia %s je definovana, ale nie deklarovana\n", func_name);
        if(get_token(binaryTree).type == LBRACKET)
        {
            int result_args = decl_param(l, binaryTree, globalFunctions);
            if (result_args != 1)
            {
                //TODO error ale opacne pozor na ze program hadze chybu pri deklaracii, ale je to chyba pri volani lebo bola zavolana s inym poctom argumentom ako v definicii
                Dputs("Chyba pri deklaracii parametrov\n");
                return 0;
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
                                node->data->declared = true;
                                node->data->defined = true;
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

int if_check(Lexeme *l, p_node binaryTree, p_node globalFunctions, bool comesFromFunction, p_node functionPtr)
{
    int result = 0;
    *l = get_token(binaryTree);
    if (l->type == LBRACKET)
    {
        //Call expression
        result = expr(CALL_CONTROL, binaryTree, l);
        if(!result)
        {
            Dputs("Chyba pri vyrazoch v ife\n");
            return 0;
        }
        Dprintf("Typ vyrazu v ife je %d\n", l->type);
        *l = get_token(binaryTree);
        if(l->type == LBRACKET_S_KUDRLINKOU)
        {
            *l = get_token(binaryTree);
            result = st_list(l, binaryTree, globalFunctions, comesFromFunction, functionPtr);
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
                            result = st_list(l, binaryTree, globalFunctions, comesFromFunction, functionPtr);
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
            result = control(l, binaryTree, globalFunctions, 0, NULL);
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
    data->param_count = 0;
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
    data->param_count = 0;
    data->func_type = type;
    data->params = NULL;
    data->elements = NULL;
    return data;
}
p_data data_init_KW()
{
    p_data data = (p_data)malloc(sizeof(struct data));
    data->declared = true;
    data->defined = true;
    data->param_count = 0;
    data->func_type = -1;
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

p_node init_global_function()
{
    p_data data = data_init_KW();
    p_node root = node_init(data, "if");
    p_node node2 = node_init(data, "while");
    insert_node(root, node2);
    p_node node3 = node_init(data, "else");
    insert_node(root, node3);
    p_node node4 = node_init(data, "return");
    insert_node(root, node4);
    p_node node5 = node_init(data, "int");
    insert_node(root, node5);
    p_node node6 = node_init(data, "float");
    insert_node(root, node6);
    p_node node7 = node_init(data, "string");
    insert_node(root, node7);
    p_node node8 = node_init(data, "void");
    insert_node(root, node8);
    p_node node9 = node_init(data, "null");
    insert_node(root, node9);
    p_node node10 = node_init(data, "function");
    insert_node(root, node10);
    return root;
}