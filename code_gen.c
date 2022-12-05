#include "code_gen.h"


void printProlog()
{
    printf(".IFJcode22\n");
}

void codeGenWrite(Lexeme l, bool comesFromFunction)
{
    switch(l.type)
    {
        case STRING_LITERAL:
            printf("WRITE string@%s\n", l.extra_data.string);
            break;
        case DECIMAL_NUMBER:
            printf("WRITE float@%a\n", l.extra_data.decimal);
            break;
        case NUMBER:
            printf("WRITE int@%d\n", l.extra_data.value);
            break;
        case EXPONENT_NUMBER:
            printf("WRITE float@%a\n", l.extra_data.exponent);
            break;
        case VARIABLE_ID:
        if(comesFromFunction)
        {
            printf("WRITE LF@$%s\n", l.extra_data.string);
            break;
        }else
        {
            printf("WRITE GF@$%s\n", l.extra_data.string);
            break;
        }
            
    }
}

void printBuiltInFunctions()
{
    printf("JUMP MAIN\n");
    /***************FUNCTION READS***************/
    printf("LABEL READS\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@**returnvar\n");
    printf("READ LF@**returnvar string\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    /*****************FUNCTION READI******************/
    printf("LABEL READI\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@**returnvar\n");
    printf("READ LF@**returnvar int\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    /*****************FUNCTION READF******************/
    printf("LABEL READF\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@**returnvar\n");
    printf("READ LF@**returnvar float\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    /*****************FUNCTION STRLEN******************/
    printf("LABEL strlen\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@**returnvar\n");
    printf("STRLEN LF@**returnvar LF@param1\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    /*****************FUNCTION SUBSTRING******************/
    printf("LABEL substring\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@**returnvar\n");                        //string returnvar;
    printf("MOVE LF@**returnvar string@\n");
    printf("DEFVAR LF@indexcount\n");                       //int indexcount;
    printf("MOVE LF@indexcount LF@param2\n");
    printf("DEFVAR LF@greaterthen\n");                      //bool greaterthen;
    printf("DEFVAR LF@char\n");
    printf("DEFVAR LF@check\n");
    printf("LT LF@check LF@param2 int@0\n");                // i<0
    printf("JUMPIFEQ errorsubstring LF@check bool@true\n");
    printf("LT LF@check LF@param3 int@0\n");                // j<0
    printf("JUMPIFEQ errorsubstring LF@check bool@true\n");
    printf("GT LF@check LF@param2 LF@param3\n");            // i>j
    printf("JUMPIFEQ errorsubstring LF@check bool@true\n");
    printf("DEFVAR LF@length\n");
    printf("STRLEN LF@length LF@param1\n");
    printf("LT LF@check LF@param2 LF@length\n");            // i>=strlen($s)
    printf("JUMPIFNEQ errorsubstring LF@check bool@true\n");
    printf("GT LF@check LF@param3 LF@length\n");            // j>strlen($s)
    printf("JUMPIFEQ errorsubstring LF@check bool@true\n");

    printf("LABEL whilesubstring\n");
    printf("EQ LF@greaterthen LF@indexcount LF@param3\n");
    printf("JUMPIFEQ endsubstring LF@greaterthen bool@true\n");
    printf("GETCHAR LF@char LF@param1 LF@indexcount\n");
    printf("CONCAT LF@**returnvar LF@**returnvar LF@char\n");
    printf("ADD LF@indexcount LF@indexcount int@1\n");
    printf("JUMP whilesubstring\n");

    printf("LABEL errorsubstring\n");
    printf("MOVE LF@**returnvar nil@nil\n");

    printf("LABEL endsubstring\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    /*****************FUNCTION ORD******************/
    printf("LABEL ord\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@**returnvar\n");
    printf("JUMPIFEQ errorord LF@param1 string@\n");
    printf("STRI2INT LF@**returnvar LF@param1 int@0\n");
    printf("JUMP returnord\n");
    printf("LABEL errorord\n");
    printf("MOVE LF@**returnvar int@0\n");
    printf("JUMP returnord\n");
    printf("LABEL returnord\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    /*****************FUNCTION FLOATVAL******************/
    printf("LABEL floatval\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@**returnvar\n");
    printf("DEFVAR LF@typ\n");
    printf("TYPE LF@typ LF@param1\n");
    printf("JUMPIFEQ floatvalint LF@typ string@int\n");
    printf("JUMPIFEQ floatvalend LF@typ string@float\n");
    printf("JUMPIFEQ floatvalnil LF@typ string@nil\n");

    printf("LABEL floatvalnil\n");
    printf("MOVE LF@param1 float@%a\n", 0.0);
    printf("JUMP floatvalend\n");

    printf("LABEL floatvalint\n");
    printf("INT2FLOAT LF@param1 LF@param1\n");

    printf("LABEL floatvalend\n");
    printf("MOVE LF@**returnvar LF@param1\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    /*****************FUNCTION INTVAL******************/
    printf("LABEL intval\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@**returnvar\n");
    printf("DEFVAR LF@typ\n");
    printf("TYPE LF@typ LF@param1\n");
    printf("JUMPIFEQ intvalfloat LF@typ string@float\n");
    printf("JUMPIFEQ intvalend LF@typ string@int\n");
    printf("JUMPIFEQ intvalnil LF@typ string@nil\n");

    printf("LABEL intvalnil\n");
    printf("MOVE LF@param1 int@0\n");
    printf("JUMP intvalend\n");

    printf("LABEL intvalfloat\n");
    printf("FLOAT2INT LF@param1 LF@param1\n");

    printf("LABEL intvalend\n");
    printf("MOVE LF@**returnvar LF@param1\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    /*****************FUNCTION STRVAL******************/
    printf("LABEL strval\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@**returnvar\n");
    printf("DEFVAR LF@typ\n");
    printf("TYPE LF@typ LF@param1\n");
    printf("JUMPIFEQ strvalend LF@typ string@string\n");
    printf("JUMPIFEQ strvalnil LF@typ string@nil\n");

    printf("LABEL strvalnil\n");
    printf("MOVE LF@param1 string@\n");
    printf("LABEL strvalend\n");
    printf("MOVE LF@**returnvar LF@param1\n");
    printf("POPFRAME\n");
    printf("RETURN\n");



    /*****************FUNCTION CHR******************/
    printf("LABEL chr\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@**returnvar\n");
    printf("INT2CHAR LF@**returnvar LF@param1\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("\n#HLAVNI TELO\n");
    printf("LABEL MAIN\n");
    printf("JUMP MAINVARDECLARE\n");
    printf("LABEL MAINAFTERVARDECLARE\n");
}


void codeGenReads()
{
    printf("CREATEFRAME\n");
    printf("CALL READS\n");
}

void codeGenReadi()
{
    printf("CREATEFRAME\n");
    printf("CALL READI\n");
}

void codeGenReadf()
{
    printf("CREATEFRAME\n");
    printf("CALL READF\n");
}

void createFrame()
{
    printf("CREATEFRAME\n");
}

void generateParam(int number, Lexeme *l, bool comesFromFunction)
{

    printf("DEFVAR TF@param%d\n", number);
    switch (l->type)
    {
        case NUMBER:
            printf("MOVE TF@param%d int@%d\n", number, l->extra_data.value);
            break;
        case STRING_LITERAL:
            evaluateEscapeSequencies(l);
            printf("MOVE TF@param%d string@%s\n", number, l->extra_data.string);
            break;
        case DECIMAL_NUMBER:
            printf("MOVE TF@param%d float@%a\n", number, l->extra_data.decimal);
            break;
        case EXPONENT_NUMBER:
            printf("MOVE TF@param%d float@%a\n", number, l->extra_data.exponent);
            break;
        case VARIABLE_ID:
            if (comesFromFunction)
            {
                printf("MOVE TF@param%d LF@$%s\n", number, l->extra_data.string);
            } else
            {
                printf("MOVE TF@param%d GF@$%s\n", number, l->extra_data.string);
            }
    }
}

void callFunction(char *functionName)
{
    printf("CALL %s\n", functionName);
}

void returnVariable(char *destination, bool comesFromFunction)
{
    if(comesFromFunction)
    {
        printf("MOVE LF@$%s TF@**returnvar\n",destination);
    } else
    {
        printf("MOVE GF@$%s TF@**returnvar\n",destination);
    }
}

void declareFunction(char *functionName)
{
    printf("\n#FUNCTION %s\n", functionName);
    printf("JUMP %sEND\n", functionName);       //potreba preskocit deklaraci funkce pri vykonavaci hlavniho tela programu
    printf("LABEL %s\n", functionName);
    printf("PUSHFRAME\n");
    printf("JUMP %sVARDECLARE\n", functionName);
    printf("LABEL %sAFTERVARDECLARE\n", functionName);
}

void declareParams(int number, char *varName)
{
    //printf("DEFVAR LF@$%s\n", varName);
    printf("MOVE LF@$%s LF@param%d\n", varName, number);
}

void codeGenReturn()
{
    printf("POPFRAME\n");
    printf("RETURN\n");
}

void codeGenReturnVar()
{
    printf("DEFVAR LF@**returnvar\n");
}

void codeGenDeclareVars(char *func_name, p_node globalFunctions, bool comesFromFunction)
{
    p_node functionPtr;

    if (!comesFromFunction)
    {
        printf("JUMP MAINEND\n");
        printf("LABEL MAINVARDECLARE\n");
        functionPtr = globalFunctions;
    } else
    {
        printf("LABEL %sVARDECLARE\n", func_name);
        functionPtr = tree_search(globalFunctions, func_name);
    }
    declare_variables(functionPtr->data->elements, comesFromFunction);
    declare_variables(functionPtr->data->params, comesFromFunction);
    if (!comesFromFunction)
    {
        printf("JUMP MAINAFTERVARDECLARE\n");
        printf("LABEL MAINEND\n");
    } else
    {
        printf("JUMP %sAFTERVARDECLARE\n", func_name);
    }
}

void declare_variables(p_node root, bool comesFromFunctions) 
{
    if (root != NULL) 
    {
        declare_variables(root->left, comesFromFunctions);
        if(comesFromFunctions)
        {
            printf("DEFVAR LF@$%s\n", root->key);
        } else
        {
            printf("DEFVAR GF@$%s\n", root->key);
        }
        declare_variables(root->right, comesFromFunctions);
    } 
}

void codeGenFunctionEnd(char *functionName, p_node globalFunctions)
{
    printf("RETURN\n");
    codeGenDeclareVars(functionName, globalFunctions, true);
    printf("LABEL %sEND\n\n", functionName);

}

void codeGenIfStart(int c)
{
    printf("LABEL IFSTART%d\n", c);
}
void codeGenIfElse(int c)
{
    printf("JUMP IFEND%d\n", c);
    printf("LABEL IFELSE%d\n", c);
}
void codeGenIfEnd(int c)
{
    printf("LABEL IFEND%d\n", c);
}

void codeGenWhileStart(int c)
{
    printf("LABEL WHILESTART%d\n", c);
}
void codeGenWhileEnd(int c)
{
    printf("JUMP WHILESTART%d\n", c);
    printf("LABEL WHILEEND%d\n", c);
}

void expr_move(char* target, int source_var_count, bool comesFromFunction)
{
    if (comesFromFunction)
    {
        if (strcmp(target, "**returnvar") == 0)
        {
            printf("MOVE LF@%s LF@$*%d\n", target, source_var_count);
        } else
        {
            printf("MOVE LF@$%s LF@$*%d\n", target, source_var_count);
        }
    }
    else
    {
        if (strcmp(target, "**returnvar") == 0)
        {
            printf("MOVE GF@%s GF@$*%d\n", target, source_var_count);
        } else
        {
            printf("MOVE GF@$%s GF@$*%d\n", target, source_var_count);
        }
    }
    
}

void generate_operation(int expr_var_count, Lexeme* sym1, Lexeme* sym2, operation operation, 
bool comesFromFunction, gen_context context, int jump_label, p_node functionPtr, p_node globalFunctions)
{

    //Switch pro generaci jednotlivých operací
    switch (operation)
    {
    case RR_PLUS: //  +
        if (type_check(sym1, sym2) == 0) return;
        operation_print_symbols(expr_var_count, sym1, sym2, "ADD", comesFromFunction, functionPtr, globalFunctions);
        break;
        
    case RR_MINUS: //  -
        if (type_check(sym1, sym2) == 0) return;
        operation_print_symbols(expr_var_count, sym1, sym2, "SUB", comesFromFunction, functionPtr, globalFunctions);
        break;

    case RR_MUL: //  *
        if (type_check(sym1, sym2) == 0) return;
        operation_print_symbols(expr_var_count, sym1, sym2, "MUL", comesFromFunction, functionPtr, globalFunctions);
        break;
        
    case RR_DIV: //  /
        print_div(expr_var_count, sym1, sym2, comesFromFunction, functionPtr, globalFunctions);
        break;
        
    case RR_CONCAT: //  .
        operation_print_symbols(expr_var_count, sym1, sym2, "CONCAT", comesFromFunction, functionPtr, globalFunctions);
        break;
    case RR_LESSER: //  <
        operation_print_symbols(expr_var_count, sym1, sym2, "LT", comesFromFunction, functionPtr, globalFunctions);
        print_expr_jump(context, jump_label, expr_var_count, comesFromFunction, "false");
        break;
    case RR_LESOREQ: //  <=
        mixed_jump_print_symbols(expr_var_count, sym1, sym2, "LT", comesFromFunction, functionPtr, globalFunctions);
        print_expr_jump(context, jump_label, expr_var_count, comesFromFunction, "false");
        break;
    case RR_GREATER: //  >
        operation_print_symbols(expr_var_count, sym1, sym2, "GT", comesFromFunction, functionPtr, globalFunctions);
        print_expr_jump(context, jump_label, expr_var_count, comesFromFunction, "false");
        break;
    case RR_GREOREQ: //  >=
        mixed_jump_print_symbols(expr_var_count, sym1, sym2, "GT", comesFromFunction, functionPtr, globalFunctions);
        print_expr_jump(context, jump_label, expr_var_count, comesFromFunction, "false");
        break;
    case RR_EQ: //  ===
        operation_print_symbols(expr_var_count, sym1, sym2, "EQ", comesFromFunction, functionPtr, globalFunctions);
        print_expr_jump(context, jump_label, expr_var_count, comesFromFunction, "false");
        break;
    case RR_NOTEQ: //  !==
        operation_print_symbols(expr_var_count, sym1, sym2, "EQ", comesFromFunction, functionPtr, globalFunctions);
        print_expr_jump(context, jump_label, expr_var_count, comesFromFunction, "true");
        break;

    default:
        break;
    }
}

void print_div(int expr_var_count, Lexeme* sym1, Lexeme* sym2, bool comesFromFunction, p_node functionPtr, p_node globalFunctions)
{
    define_comp_var_with_helper(expr_var_count, comesFromFunction, functionPtr, globalFunctions);

    char* scope = "GF@";
    if(comesFromFunction)
    {
        scope = "LF@";
    }

    createFrame();
    generateParam(1, sym1, comesFromFunction);
    callFunction("floatval");
    printf("MOVE %s$*%d_1 TF@**returnvar\n", scope, expr_var_count);

    createFrame();
    generateParam(1, sym2, comesFromFunction);
    callFunction("floatval");
    printf("MOVE %s$*%d_2 TF@**returnvar\n", scope, expr_var_count);

    printf("DIV %s$*%d %s$*%d_1 %s$*%d_2\n", scope, expr_var_count, scope, expr_var_count, scope, expr_var_count);
}

void operation_print_symbols(int expr_var_count, Lexeme* sym1, Lexeme* sym2, char* operation, bool comesFromFunction,
    p_node functionPtr, p_node globalFunctions){

    define_comp_var(expr_var_count, comesFromFunction, functionPtr, globalFunctions);

    char* scope;
    if (expr_var_count != -1)
    {
        if (comesFromFunction)
        {
            // Nastavení kontextu
            scope = "LF@";
        }
        else
        {
            scope = "GF@";
        }
    }

    //Řešení - před závorkou
    //TODO: Type konverze pro konstantu (int/float)
    if(sym1->type == EXPR && sym1->negative_num == true)
    {
        printf("SUB %s$*%d int@0 %s$*%d\n", scope, sym1->extra_data.value, scope, sym1->extra_data.value);
        sym1->negative_num = false;
    }
    if(sym2->type == EXPR && sym2->negative_num == true)
    {
        printf("SUB %s$*%d int@0 %s$*%d\n", scope, sym2->extra_data.value, scope, sym2->extra_data.value);
        sym2->negative_num = false;
    }

    // Print operace a výstupní proměnné
    printf("%s %s$*%d ", operation, scope, expr_var_count);

    // Print prvního symbolu
    print_single_symbol(sym1, scope);

    // Print druhého symbolu
    print_single_symbol(sym2, scope);
    printf("\n");
}

void define_comp_var(int expr_var_count, bool comesFromFunction, p_node functionPtr, p_node globalFunctions)
{
    p_data data = data_init();
    char* var_name = malloc(sizeof(char)*20);
    snprintf(var_name, 20, "*%d", expr_var_count);
    p_node comp_var = node_init(data, var_name);

    char* scope;
    if (expr_var_count != -1)
    {
        if (comesFromFunction)
        {
            // Definování překladačové proměnné pro uložení dočasného výsledku
            if (functionPtr->data->elements == NULL)
            {
                functionPtr->data->elements = comp_var;
            }
            else 
            {
                insert_node(functionPtr->data->elements, comp_var);
            }
        }
        else
        {
            if (globalFunctions->data->elements == NULL)
            {
                globalFunctions->data->elements = comp_var;
            }
            else 
            {
                insert_node(globalFunctions->data->elements, comp_var);
            }
        }
    }
}

void define_comp_var_with_helper(int expr_var_count, bool comesFromFunction, p_node functionPtr, p_node globalFunctions)
{
    p_data data = data_init();
    p_data data_fl1 = data_init();
    p_data data_fl2 = data_init();

    char* var_name = malloc(sizeof(char)*20);
    sprintf(var_name, "*%d", expr_var_count);
    p_node comp_var = node_init(data, var_name);
    char* fl1_var_name = malloc(sizeof(char)*20);
    sprintf(fl1_var_name, "*%d_1", expr_var_count);
    p_node fl_var_1 = node_init(data_fl1, fl1_var_name);
    char* fl2_var_name = malloc(sizeof(char)*20);
    sprintf(fl2_var_name, "*%d_2", expr_var_count);
    p_node fl_var_2 = node_init(data_fl2, fl2_var_name);

    if(comesFromFunction)
    {
        if (functionPtr->data->elements == NULL)
        {
            functionPtr->data->elements = comp_var;
            insert_node(functionPtr->data->elements, fl_var_1);
            insert_node(functionPtr->data->elements, fl_var_2);
        }
        else 
        {
            insert_node(functionPtr->data->elements, comp_var);
            insert_node(functionPtr->data->elements, fl_var_1);
            insert_node(functionPtr->data->elements, fl_var_2);
        }
    }
    else 
    {
        if (globalFunctions->data->elements == NULL)
        {
            globalFunctions->data->elements = comp_var;
            insert_node(globalFunctions->data->elements, fl_var_1);
            insert_node(globalFunctions->data->elements, fl_var_2);
        }
        else 
        {
            insert_node(globalFunctions->data->elements, comp_var);
            insert_node(globalFunctions->data->elements, fl_var_1);
            insert_node(globalFunctions->data->elements, fl_var_2);
        }
    }
}

void mixed_jump_print_symbols(int expr_var_count, Lexeme* sym1, Lexeme* sym2, char* operation, 
    bool comesFromFunction, p_node functionPtr, p_node globalFunctions)
{
    char* scope = "GF@";
    if (expr_var_count != -1)
    {
        define_comp_var_with_helper(expr_var_count, comesFromFunction, functionPtr, globalFunctions);
        if(comesFromFunction)
        {
            scope = "LF@";
        }
    }

    // Print relační operace
    printf("%s %s$*%d_1 ", operation, scope, expr_var_count);

    // Print prvního symbolu
    print_single_symbol(sym1, scope);

    // Print druhého symbolu
    print_single_symbol(sym2, scope);
    printf("\n");

    // Print operace rovnosti
    printf("EQ %s$*%d_2 ", scope, expr_var_count);
    print_single_symbol(sym1, scope);
    print_single_symbol(sym2, scope);
    printf("\n");

    // Print OR jednotlivých výsledků do dočasné výstupní proměnné
    printf("OR %s$*%d %s$*%d_1 %s$*%d_2", scope, expr_var_count, scope, expr_var_count, scope, expr_var_count);
    printf("\n");
}

int type_check(Lexeme *sym1, Lexeme *sym2)
{
    if (sym1->type == NUMBER && sym2->type == NUMBER)
    {
        return 1;
    }
    else if (sym1->type == DECIMAL_NUMBER && (sym2->type == DECIMAL_NUMBER || sym2->type == EXPONENT_NUMBER))
    {
        return 1;
    }
    else if (sym1->type == EXPONENT_NUMBER && (sym2->type == DECIMAL_NUMBER || sym2->type == EXPONENT_NUMBER))
    {
        return 1;
    }
    else if ((sym1->type == DECIMAL_NUMBER || sym1->type == EXPONENT_NUMBER) && 
        !(sym2->type == DECIMAL_NUMBER || sym2->type == EXPONENT_NUMBER))
    {
        
    }
    // ODTUD DOČASNÝ KÓD
    else if (sym1->type == VARIABLE_ID || sym2->type == VARIABLE_ID)
    {
        //TODO: Zkontrolovat kompatibilitu datových typů u proměnných
        return 1;
    }
    else if (sym1->type == EXPR || sym2->type == EXPR)
    {
        //TODO: Vkládání redukovaných expressions buď jako VARIABLE_ID včetně typu do stromu
        //      nebo přímo do kódu jako konstanty.
        return 1;
    }
    else if (sym1->type == KW_NULL || sym2->type == KW_NULL)
    {
        return 1;
    }
    // POTUD DOČASNÝ KÓD
    else 
    {
        error(sym1->row, "Ve výrazu se nachází nekompatibilní datové typy!", SEM_INVALID_TYPE_ERROR);
        return 0;
    }
}

void print_single_symbol(Lexeme* lexeme, char* scope)
{
    char* minus = "";
    if(lexeme->negative_num == true)
    {
        minus = "-";
    }

    switch (lexeme->type)
    {
    case EXPR: // Enum symbolizující expression (ve value je uloženo číslo compiler proměnné)
        printf("%s$*%d ", scope, lexeme->extra_data.value);
        break;
    case VARIABLE_ID:
        printf("%s$%s ", scope, lexeme->extra_data.string);
        break;
    case NUMBER:
        printf("int@%s%d ", minus, lexeme->extra_data.value);
        // printf("negative flag: %d\n", lexeme->negative_num);
        break;
    case DECIMAL_NUMBER:
        printf("float@%s%a ", minus, lexeme->extra_data.decimal);
        break;
    case EXPONENT_NUMBER:
        printf("float@%s%a ", minus, lexeme->extra_data.exponent);
        break;
    case STRING_LITERAL:
        evaluateEscapeSequencies(lexeme);
        printf("string@%s ", lexeme->extra_data.string);
        break;
    case KW_NULL:
        printf("nil@nil ");
        break;
    default:
        break;
    }
}

void print_expr_jump(gen_context context, int jump_label, int expr_var_count, bool comesFromFunction, char* skip_on)
{
    char* scope = "GF@";
    if(comesFromFunction)
    {
        scope = "LF@";
    }
    if (context == CG_IF)
    {
        printf("JUMPIFEQ IFELSE%d %s$*%d bool@%s\n", jump_label, scope, expr_var_count, skip_on);
    }
    else if (context == CG_WHILE)
    {
        printf("JUMPIFEQ WHILEEND%d %s$*%d bool@%s\n", jump_label, scope, expr_var_count, skip_on);
    }
}