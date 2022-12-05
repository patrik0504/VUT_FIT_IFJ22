#include "parser.h"

int parse(){
    int result = 0;
    //Incializácia stromu pre lexer s KW
    p_node binaryTree = init_binary_treeKW();
    Lexeme * l = token_init(); 
    p_node globalFunctions = init_global_function();
    
    //Výpis do code genu
    printProlog();
    printBuiltInFunctions();
    //Kontrola hlavného tela programu
    result = prog(l, binaryTree, globalFunctions);
    codeGenDeclareVars("main", globalFunctions, false);
    int err = 0;
    //Kontrola že či boli všetky funkcie definované
    check_func(globalFunctions, &err);
    if(err == 1){
        error(-1, "Funkcia bola zavolaná, ale nebola deklarovaná", SEM_UNDEFINED_FUNC_ERROR);
        return 0;
    }
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
        case KW_OPTIONALINT:
        case KW_OPTIONALFLOAT:
        case KW_OPTIONALSTRING:
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

int check_minus(Lexeme *l, p_node binaryTree)
{
    int result = 1;
    if(l->type == MINUS)
    {
        *l = get_token(binaryTree);
        switch (l->type)
        {
            case NUMBER:
                l->extra_data.value = -l->extra_data.value;
                break;
            case EXPONENT_NUMBER:
                l->extra_data.exponent = -l->extra_data.exponent;
                break;
            case DECIMAL_NUMBER:
                l->extra_data.decimal = -l->extra_data.decimal;
                break;
            default:
                error(l->row, "Neočekávaný token za '-'", SYNTAX_ERROR);
                result = 0;
        }
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
            //Vloženie nového parametra do stromu
            p_node param = node_init(data, l->extra_data.string);
            node->data->params = param;
            if (!node->data->defined && !node->data->declared)
            {
                node->data->param_count++;
            }
            declareParams(node->data->param_count, l->extra_data.string);   //generování kódu pro deklaraci parametrů
            result = decl_param2(l, binaryTree, globalFunctions, node);
        } else
        {
            error(l->row, "Spatna deklarace parametru funkce", SYNTAX_ERROR);
        }
    }
    else if(l->type == RBRACKET)
    {
        p_node func = tree_search(globalFunctions, function_name);
        //Ak bola funkcia definovaná, tak sa kontroluje počet parametrov pri deklarácii
        if (func->data->defined)
        {
            if(func->data->param_count == count_tree(func->data->params))
            {
                result = 1;
            }
        } else
        {
            result = 1;
        }
    } else
    {
        error(l->row, "Chybny nebo spatny typ parametru v deklaraci funkce", SYNTAX_ERROR);
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
                if(tree_search(function_node->data->params, l->extra_data.string) != NULL)
                {
                    error(l->row, "Premenná bola deklarovaná viackrát", SEM_OTHER_ERROR);
                    return 0;
                }
                //Vloženie nového parametra do stromu
                p_node param = node_init(data, l->extra_data.string);
                insert_node(function_node->data->params, param);
                if (!function_node->data->defined && !function_node->data->declared)
                {
                    function_node->data->param_count++;
                }
                declareParams(function_node->data->param_count, l->extra_data.string);
                result = decl_param2(l, binaryTree, globalFunctions, function_node);

            }
        }
    } else if(l->type == RBRACKET)
    {
        p_node func = function_node;
        if (func->data->defined)
        {
            if(func->data->param_count == count_tree(func->data->params))
            {
                result = 1;
            }
        } else
        {
            result = 1;
        }
    } else
    {
        error(l->row, "Chybny nebo spatny typ parametru v deklaraci funkce", SYNTAX_ERROR);
    }
    return result;
}

int check_if_variable_is_defined(p_node functionPtr, char * variable)
{
    int result = 0;
    
    //Kontrola že či je variable ako parameter funkcie
    if (((functionPtr->data->params != NULL) && (tree_search(functionPtr->data->params, variable) == NULL)) || (functionPtr->data->params == NULL))
    {
        //Kontrola že či je variable ako lokálna premmená
        if(((functionPtr->data->elements != NULL) && (tree_search(functionPtr->data->elements, variable) == NULL)) || (functionPtr->data->elements == NULL))
        {
            //Promena nie je vo funkcii definovana
            result = 0;
        } else
        {   
            //Promena je vo funkcii definovana   
            result = 1;
        }
    } else
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
        char * variable = l->extra_data.string;
        if(!comesFromFunction)
        {
            //Kontrola že či je už variable deklarovaná
            if (((globalFunctions->data->elements != NULL) && (tree_search(globalFunctions->data->elements, l->extra_data.string) == NULL)) || (globalFunctions->data->elements == NULL))
            {
                //Premmená nie je deklarovaná
                //Vytvorenie novej premmenj
                p_data data = data_init();
                p_node local_var = node_init(data, l->extra_data.string);
                //Kontrola že či je strom premmených inicializovaný
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
        result = expr(ASSIGNMENT, -1, binaryTree, l, variable, globalFunctions, comesFromFunction, functionPtr);
        if(result != 1)
        {
            return result;
        }
        result = 1;
    } else if (l->type == FUNCTION_ID)
    {
        char * function_name = l->extra_data.string;
        p_node node = tree_search(globalFunctions, function_name);
        if(node == NULL)
        {
            //funkce nebyla deklarovaná ani předtím zavolaná
            if(comesFromFunction)
            {
                p_data data = data_init();
                node = node_init(data, function_name);
                insert_node(globalFunctions, node);
            }
            else
            {
                error(l->row, "Funkcia nie je definovana", SEM_UNDEFINED_FUNC_ERROR);
                return 0;
            }
        } else if (strcmp(node->key, "write") == 0)
        {
            result = writeString(l, binaryTree, globalFunctions, comesFromFunction, functionPtr);
            return result;
        }
        *l = get_token(binaryTree);
        
        if(l->type == LBRACKET)
        {
            *l = get_token(binaryTree);
            int result_par = param(l, binaryTree, comesFromFunction, functionPtr, node, globalFunctions);
            if(result_par)
            {
                if(l->type == RBRACKET)
                {
                    *l = get_token(binaryTree);
                    if(l->type == SEMICOLON)
                    {
                        Dputs("Nasiel som volanie funkcie v statemente\n");
                        callFunction(function_name);
                        result = 1;
                    }
                    else
                    {
                        error(l->row, "Chýba bodkočiarka.", SYNTAX_ERROR);
                        return 0;
                    }
                }
                else
                {
                    error(l->row, "Chýba zatvorka.", SYNTAX_ERROR);
                    return 0;
                }
            }
        }
    } else if(l->type == KW_RETURN)
    {
        result = ret_expr(l, binaryTree, globalFunctions, comesFromFunction, functionPtr);
        if(result)
        {
            Dputs("Nasiel som return v statemente\n");
            result = 1;
        }
        if(functionPtr == NULL)
        {
            //DOCASNE RIESNIE ZMENIT KED SA TO BUDE RIESIT
            codeGenReturn(comesFromFunction, "main");
        }
        else
        {
            codeGenReturn(comesFromFunction, functionPtr->key);
        }
    }
    return result;
}

int ret_expr(Lexeme *l, p_node binaryTree, p_node globalFunctions, bool comesFromFunction, p_node functionPtr)
{
    int result = 0;
    result = expr(RETURN, -1, binaryTree, l, NULL, globalFunctions, comesFromFunction, functionPtr);
    if(!result)
    {
        return result;
    }
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
        } else
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
            *l = get_token(binaryTree);
            if (comesFromFunction == 1)
            {
                result = st_list(l, binaryTree, globalFunctions, 1, functionPtr);
        
            } else
            {
                result = st_list(l, binaryTree, globalFunctions, 0, NULL);
            }
        }
    } else if (l->type == FUNCTION_ID)
    {
        result = statement(l, binaryTree, globalFunctions, comesFromFunction, functionPtr);
        if(result)
        {
            *l = get_token(binaryTree);
            Dputs("Nacital som spravne volanie funkcie\n");
            result = st_list(l, binaryTree, globalFunctions, comesFromFunction, functionPtr);
        }
    } else if(l->type == KW_IF){
        Dputs("Nasiel som if v st_list\n");
        result = control(l, binaryTree, globalFunctions, comesFromFunction, functionPtr);
        if (result)
        {
            *l = get_token(binaryTree);
            result = st_list(l, binaryTree, globalFunctions, comesFromFunction, functionPtr);
        }
    } else if(l->type == KW_WHILE){
        result = control(l, binaryTree, globalFunctions, comesFromFunction, functionPtr);
        if (result)
        {
            *l = get_token(binaryTree);
            result = st_list(l, binaryTree, globalFunctions, comesFromFunction, functionPtr);
        }
    } else if(l->type == KW_RETURN)
    {
        result = statement(l, binaryTree, globalFunctions, comesFromFunction, functionPtr);
        if(result)
        {
            Dputs("Nasiel som return v st_list\n");
            functionPtr->data->found_return = true;
            *l = get_token(binaryTree);
            result = st_list(l, binaryTree, globalFunctions, comesFromFunction, functionPtr);
        }
    } else
    {
        result = 1;
    }
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
        } else if (!callFunction->data->defined && !callFunction->data->declared )
        {
            Dprintf("pricitam parametr %s\n", l->extra_data.string);
            callFunction->data->param_count++;
        }

        if(comesFromFunction)
        {
            if (check_if_variable_is_defined(functionPtr, l->extra_data.string) == 1)
            {
                param_count++;
                createFrame();
                generateParam(param_count, l, comesFromFunction);
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
                createFrame();
                generateParam(param_count, l, comesFromFunction);
                *l = get_token(binaryTree);
                result = param2(l, binaryTree, comesFromFunction, functionPtr, param_count, callFunction, globalFunctions);
            } else
            {
                Dprintf("Promenna %s neni definovana\n", l->extra_data.string);
                error(l->row, "Promenna neni definovana", SEM_UNDEFINED_VAR_ERROR);
                return 0;
            }
        }
    
    } else if (l->type == NUMBER || l->type == STRING_LITERAL || l->type == DECIMAL_NUMBER || l->type == EXPONENT_NUMBER || l->type == MINUS)
    {
        if(l->type == MINUS)
        {
            if (!check_minus(l, binaryTree))
            {
                return 0;
            }
        }
        if((count_tree(callFunction->data->params) == 0) && (callFunction->data->declared)){
            Dprintf("Funkce %s nema zadne parametry a boli jej hodené\n", callFunction->key);
            error(l->row, "Funkce nema zadne parametry a boli jej hodené", SEM_INVALID_CALL_ERROR);
            return 0;
        } else if (!callFunction->data->defined && !callFunction->data->declared )
        {
            Dprintf("pricitam parametr %s\n", l->extra_data.string);
            callFunction->data->param_count++;
        }

        param_count++;
        createFrame();
        generateParam(param_count, l, comesFromFunction);
        *l = get_token(binaryTree);
        result = param2(l, binaryTree, comesFromFunction, functionPtr, param_count, callFunction, globalFunctions);
    }
    else if(l->type == RBRACKET)
    {
        if(callFunction->data->defined && callFunction->data->declared)
        {
            if(param_count == count_tree(callFunction->data->params))
            {
                createFrame();
                result = 1;
            } else
            {
                Dprintf("Zlý počet parametrov, očakávalo se %d, ale bylo zadáno %d\n", count_tree(callFunction->data->params), param_count);
                error(l->row, "Zlý počet parametrov", SEM_INVALID_CALL_ERROR);
                result = 0;
            }
        } else if (callFunction->data->defined && !callFunction->data->declared)
        {
            if(param_count == callFunction->data->param_count)
            {
                createFrame();
                result = 1;
            } else
            {
                Dprintf("Zlý počet parametrov, očakávalo se %d, ale bylo zadáno %d\n", count_tree(callFunction->data->params), param_count);
                error(l->row, "Zlý počet parametrov", SEM_INVALID_CALL_ERROR);
                result = 0;
            }
        } else
        {
            createFrame();
            callFunction->data->defined = true;
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
            if (!callFunction->data->defined && !callFunction->data->declared )
            {
                Dprintf("pricitam parametr %s\n", l->extra_data.string);
                callFunction->data->param_count++;
            }
            if(comesFromFunction)
            {
                if (check_if_variable_is_defined(functionPtr, l->extra_data.string) == 1)
                {
                    paramCount++;
                    generateParam(paramCount, l, comesFromFunction);
                    *l = get_token(binaryTree);
                    result = param2(l, binaryTree, comesFromFunction, functionPtr, paramCount, callFunction, globalFunctions);
                } else
                {
                    error(l->row, "Promenna neni definovana", SEM_UNDEFINED_VAR_ERROR);
                    return 0;
                }
            } else
            {
                if(tree_search(globalFunctions->data->elements, l->extra_data.string) != NULL)
                {
                    paramCount++;
                    generateParam(paramCount, l, comesFromFunction);
                    *l = get_token(binaryTree);
                    result = param2(l, binaryTree, comesFromFunction, functionPtr, paramCount, callFunction, globalFunctions);
                } else
                {
                    error(l->row, "Promenna neni definovana", SEM_UNDEFINED_VAR_ERROR);
                    return 0;
                }
            }
            
        } else if (l->type == NUMBER || l->type == STRING_LITERAL || l->type == DECIMAL_NUMBER || l->type == EXPONENT_NUMBER || l->type == MINUS)
        {
            if(l->type == MINUS)
            {
                if (!check_minus(l, binaryTree))
                {
                    return 0;
                }
            }
            if (!callFunction->data->defined && !callFunction->data->declared )
            {
                Dprintf("pricitam parametr %s\n", l->extra_data.string);
                callFunction->data->param_count++;
            }

            paramCount++;

            generateParam(paramCount, l, comesFromFunction);
            *l = get_token(binaryTree);
            result = param2(l, binaryTree, comesFromFunction, functionPtr, paramCount, callFunction, globalFunctions);
        }
    } else if(l->type == RBRACKET)
    {
        if(callFunction->data->defined && callFunction->data->declared)
        {
            if(paramCount == count_tree(callFunction->data->params))
            {
                Dprintf("Nasiel som %d parametrov\n", paramCount);
                result = 1;
            } else
            {
                error(l->row, "Zlý počet parametrov", SEM_INVALID_CALL_ERROR);
                result = 0;
            }
        } else if (callFunction->data->defined && !callFunction->data->declared)
        {
            if(paramCount == callFunction->data->param_count)
            {
                result = 1;
            } else
            {
                Dprintf("Zlý počet parametrov, očakávalo se %d, ale bylo zadáno %d\n", count_tree(callFunction->data->params), param_count);
                error(l->row, "Zlý počet parametrov", SEM_INVALID_CALL_ERROR);
                result = 0;
            }
        } else
        {
            callFunction->data->defined = true;
            result = 1;
        }
    }
    return result;
}

int while_check(Lexeme *l, p_node binaryTree, p_node globalFunctions, bool comesFromFunction, p_node functionPtr)
{
    
    int result = 0;
    static int while_count = 0;
    while_count++;
    int while_id = while_count;
    *l = get_token(binaryTree);
    if(l->type == LBRACKET)
    {
        codeGenWhileStart(while_id);
        result = expr(WHILE, while_id, binaryTree, l, NULL, globalFunctions, comesFromFunction, functionPtr);
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
                    codeGenWhileEnd(while_id);
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
        declareFunction(l->extra_data.string);
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
                if(type != 0)
                {
                    if(type != KW_VOID)
                    {
                        codeGenReturnVar();
                    }
                    p_node node = tree_search(globalFunctions, func_name);
                    if(node != NULL)
                    {
                        node->data->func_type = lexeme_type_to_type(l);
                    }
                    *l = get_token(binaryTree);
                    if(l->type == LBRACKET_S_KUDRLINKOU)
                    {
                        *l = get_token(binaryTree);
                        int result_st = st_list(l, binaryTree, globalFunctions, 1, node);
                        if(result_st)
                        {
                            if(l->type == RBRACKET_S_KUDRLINKOU)
                            {
                                if(node->data->func_type != VOID && node->data->found_return == false)
                                {
                                    error(l->row, "Funkce neobsahuje return, i když není void", SEM_INVALID_CALL_ERROR);
                                    return 0;
                                }
                                codeGenFunctionEnd(func_name, globalFunctions);
                                //codeGenDeclareVars(func_name, globalFunctions, true);
                                node->data->declared = true;
                                node->data->defined = true;
                                Dputs("Funkcia je v poriadku\n");
                                result = 1;
                            }
                        }
                    }
                }
                else
                {
                    error(l->row, "Nespravny navratový typ funkcie", SYNTAX_ERROR);
                    return 0;
                }
            }
        }
    }else if (root->data->defined && !root->data->declared)
    {
        declareFunction(func_name);
        Dprintf("Funkcia %s je definovana, ale nie deklarovana\n", func_name);
        if(get_token(binaryTree).type == LBRACKET)
        {
            int result_args = decl_param(l, binaryTree, globalFunctions);
            if (result_args != 1)
            {
                //TODO error ale opacne pozor na ze program hadze chybu pri deklaracii, ale je to chyba pri volani lebo bola zavolana s inym poctom argumentom ako v definicii
                Dputs("Chyba pri deklaracii parametrov\n");
                error(l->row, "Zlý počet parametrov", SEM_INVALID_CALL_ERROR);
                return 0;
            }
            
            if(get_token(binaryTree).type == COLON)
            {
                *l = get_token(binaryTree);
                int type = check_type(l);
                if(type != 0)
                {
                    if(type != KW_VOID)
                    {
                        codeGenReturnVar();
                    }
                    p_node node = tree_search(globalFunctions, func_name);
                    if(node != NULL)
                    {
                        node->data->func_type = lexeme_type_to_type(l);
                    }
                    *l = get_token(binaryTree);
                    if(l->type == LBRACKET_S_KUDRLINKOU)
                    {
                        *l = get_token(binaryTree);
                        int result_st = st_list(l, binaryTree, globalFunctions, 1, node);
                        if(result_st)
                        {
                            if(l->type == RBRACKET_S_KUDRLINKOU)
                            {
                                if(node->data->func_type != VOID && node->data->found_return == false)
                                {
                                    error(l->row, "Funkce neobsahuje return, i když není void", SEM_INVALID_CALL_ERROR);
                                    return 0;
                                }
                                node->data->declared = true;
                                node->data->defined = true;
                                Dputs("Funkcia je v poriadku\n");
                                codeGenFunctionEnd(func_name, globalFunctions);
                                result = 1;
                            }
                        }
                    }
                }
            }
        }
    } else if (root->data->defined && root->data->declared)
    {
        if(tree_search(binaryTree, func_name) == NULL)
        {
            error(l->row, "Pokus o redefinici funkce", SEM_UNDEFINED_FUNC_ERROR);
        }
        else
        {
            error(l->row, "Názov funkcie je key word", SYNTAX_ERROR);
        }
        
    }
    if(!result)
    {
        error(l->row, "Chyba pri deklaracii funkcie", SYNTAX_ERROR);
    }
    return result;
}

int if_check(Lexeme *l, p_node binaryTree, p_node globalFunctions, bool comesFromFunction, p_node functionPtr)
{
    int result = 0;
    *l = get_token(binaryTree);
    static int if_counter = 0;
    if_counter++;
    const int if_label = if_counter;
    codeGenIfStart(if_label);
    if (l->type == LBRACKET)
    {
        //Call expression
        int result_expr = expr(IF, if_label, binaryTree, l, NULL, globalFunctions, comesFromFunction, functionPtr);
        if(!result_expr)
        {
            Dputs("Chyba pri vyrazoch v ife\n");
            return 0;
        }
        Dprintf("Typ vyrazu v ife je %d\n", l->type);
        *l = get_token(binaryTree);
        if(l->type == LBRACKET_S_KUDRLINKOU)
        {
            *l = get_token(binaryTree);
            int result_st = st_list(l, binaryTree, globalFunctions, comesFromFunction, functionPtr);
            if (result_st)
            {
                if(l->type == RBRACKET_S_KUDRLINKOU)
                {
                    *l = get_token(binaryTree);
                    if(l->type == KW_ELSE)
                    {
                        codeGenIfElse(if_label);
                        *l = get_token(binaryTree);
                        if(l->type == LBRACKET_S_KUDRLINKOU)
                        {
                            *l = get_token(binaryTree);
                            result_st = st_list(l, binaryTree, globalFunctions, comesFromFunction, functionPtr);
                            if (result_st)
                            {
                                if(l->type == RBRACKET_S_KUDRLINKOU)
                                {
                                    codeGenIfEnd(if_label);
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
    if(!result)
    {
        error(l->row, "Chyba pri definicii ifu\n", SYNTAX_ERROR);
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
            if(result == 0)
            {
                return 0;
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
                return 0;
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
                return 0;
            }
            result = body(l, binaryTree, globalFunctions);
            break;
        default:
            error(l->row, "Neočekávaný token", SYNTAX_ERROR);
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

void insert_function(p_node globalFunctions, Lexeme *l)
{
    p_node node = node_init(data_init(), l->extra_data.string);
    insert_node(globalFunctions, node);
}


p_data data_init()
{
    p_data data = (p_data)malloc(sizeof(struct data));
    data->declared = false;
    data->defined = false;
    data->param_count = 0;
    data->func_type = -1;
    data->found_return = false;
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
        case KW_OPTIONALINT:
            type = OPTIONALINT;
            break;
        case KW_OPTIONALFLOAT:
            type = OPTIONALFLOAT;
            break;
        case KW_OPTIONALSTRING:
            type = OPTIONALSTRING;
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
    data->found_return = false;
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
    data->found_return = true;
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
        case KW_OPTIONALINT:
            return OPTIONALINT;
            break;
        case KW_OPTIONALFLOAT:
            return OPTIONALFLOAT;
            break;
        case KW_OPTIONALSTRING:
            return OPTIONALSTRING;
            break;
        case KW_VOID:
            return VOID;
            break;
        default:
            return -1;
            break;
    }
}

void set_params_in_builtin_functions(p_node binaryTree)
{

    /*************FUNCTION STRLEN**********************/
    p_node function = tree_search(binaryTree, "strlen");
    p_data data = data_init_type(KW_STRING);
    p_node param1 = node_init(data, "$s");
    function->data->params = param1;
    function->data->param_count = 1;

    /*************FUNCTION SUBSTRING*******************/
    function = tree_search(binaryTree, "substring");
    data = data_init_type(KW_STRING);
     param1 = node_init(data, "$c");
    function->data->params = param1;

    p_node param2 = node_init(data, "$i");
    insert_node(function->data->params, param2);

    p_node param3 = node_init(data, "$j");
    insert_node(function->data->params, param3);
    function->data->param_count = 3;

    /*************FUNCTION ORD*************************/
    function = tree_search(binaryTree, "ord");
    data = data_init_type(KW_STRING);
    param1 = node_init(data, "$s");
    function->data->params = param1;
    function->data->param_count = 1;

    /*************FUNCTION CHR****************************/
    function = tree_search(binaryTree, "chr");
    data = data_init_type(KW_INT);
    param1 = node_init(data, "$i");
    function->data->params = param1;
    function->data->param_count = 1;

    /*************FUNCTION FLOATVAL********************/
    function = tree_search(binaryTree, "floatval");
    data = data_init_type(KW_FLOAT);
    param1 = node_init(data, "$s");
    function->data->params = param1;
    function->data->param_count = 1;

    /*************FUNCTION INTVAL**********************/
    function = tree_search(binaryTree, "intval");
    data = data_init_type(KW_INT);
    param1 = node_init(data, "$s");
    function->data->params = param1;
    function->data->param_count = 1;

    /*************FUNCTION STRVAL**********************/
    function = tree_search(binaryTree, "strval");
    data = data_init_type(KW_STRING);
    param1 = node_init(data, "$s");
    function->data->params = param1;
    function->data->param_count = 1;

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
    p_node node11 = node_init(data, "write");
    insert_node(root, node11);
    p_data datareads = data_init_KW();
    datareads->func_type = OPTIONALSTRING;
    p_node node12 = node_init(datareads, "reads");
    insert_node(root, node12);
    p_data datareadi = data_init_KW();
    datareadi->func_type = OPTIONALINT;
    p_node node13 = node_init(datareadi, "readi");
    insert_node(root, node13);
    p_data datareadf = data_init_KW();
    datareadf->func_type = OPTIONALFLOAT;
    p_node node14 = node_init(datareadf, "readf");

    //TODO Doplnit datove typy pro funkce
    insert_node(root, node14);
    p_data datastrlen = data_init_KW();
    p_node node15 = node_init(datastrlen, "strlen");
    insert_node(root, node15);
    p_data datasubstring = data_init_KW();
    p_node node16 = node_init(datasubstring, "substring");
    insert_node(root, node16);
    p_data dataord = data_init_KW();
    p_node node17 = node_init(dataord, "ord");
    insert_node(root, node17);
    p_data datachr = data_init_KW();
    p_node node18 = node_init(datachr, "chr");
    insert_node(root, node18);
    p_data datafloatval = data_init_KW();
    p_node node19 = node_init(datafloatval, "floatval");
    insert_node(root, node19);
    p_data dataintval = data_init_KW();
    p_node node20 = node_init(dataintval, "intval");
    insert_node(root, node20);
    p_data datastrval = data_init_KW();
    p_node node21 = node_init(datastrval, "strval");
    insert_node(root, node21);

    set_params_in_builtin_functions(root);
    return root;
}