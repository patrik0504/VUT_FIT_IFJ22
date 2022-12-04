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

void generate_operation(int expr_var_count, Lexeme* sym1, Lexeme* sym2, operation operation, bool comesFromFunction, gen_context context, int jump_label)
{

    //Switch pro generaci jednotlivých operací
    switch (operation)
    {
    case RR_PLUS: //  +
        operation_print_symbols(expr_var_count, sym1, sym2, "ADD", comesFromFunction);
        break;
        
    case RR_MINUS: //  -
        operation_print_symbols(expr_var_count, sym1, sym2, "SUB", comesFromFunction);
        break;

    case RR_MUL: //  *
        operation_print_symbols(expr_var_count, sym1, sym2, "MUL", comesFromFunction);
        break;
        
    case RR_DIV: //  /
        print_div(expr_var_count, sym1, sym2, comesFromFunction);
        break;
        
    case RR_CONCAT: //  .
        operation_print_symbols(expr_var_count, sym1, sym2, "CONCAT", comesFromFunction);
        break;
    case RR_LESSER: //  <
        operation_print_symbols(expr_var_count, sym1, sym2, "LT", comesFromFunction);
        print_expr_jump(context, jump_label, expr_var_count, comesFromFunction, "false");
        break;
    case RR_LESOREQ: //  <=
        mixed_jump_print_symbols(expr_var_count, sym1, sym2, "LT", comesFromFunction);
        print_expr_jump(context, jump_label, expr_var_count, comesFromFunction, "false");
        break;
    case RR_GREATER: //  >
        operation_print_symbols(expr_var_count, sym1, sym2, "GT", comesFromFunction);
        print_expr_jump(context, jump_label, expr_var_count, comesFromFunction, "false");
        break;
    case RR_GREOREQ: //  >=
        mixed_jump_print_symbols(expr_var_count, sym1, sym2, "GT", comesFromFunction);
        print_expr_jump(context, jump_label, expr_var_count, comesFromFunction, "false");
        break;
    case RR_EQ: //  ===
        operation_print_symbols(expr_var_count, sym1, sym2, "EQ", comesFromFunction);
        print_expr_jump(context, jump_label, expr_var_count, comesFromFunction, "false");
        break;
    case RR_NOTEQ: //  !==
        operation_print_symbols(expr_var_count, sym1, sym2, "EQ", comesFromFunction);
        print_expr_jump(context, jump_label, expr_var_count, comesFromFunction, "true");
        break;

    default:
        break;
    }
}

void print_div(int expr_var_count, Lexeme* sym1, Lexeme* sym2, bool comesFromFunction)
{
    char* scope = "GF@";
    if(comesFromFunction)
    {
        scope = "LF@";
    }

    printf("DEFVAR %s$*%d\n", scope, expr_var_count);
    printf("DEFVAR %s$*%d_fl1\n", scope, expr_var_count);
    printf("DEFVAR %s$*%d_fl2\n", scope, expr_var_count);

    createFrame();
    generateParam(1, sym1, comesFromFunction);
    callFunction("floatval");
    printf("MOVE %s$*%d_fl1 TF@**returnvar\n", scope, expr_var_count);

    createFrame();
    generateParam(1, sym2, comesFromFunction);
    callFunction("floatval");
    printf("MOVE %s$*%d_fl2 TF@**returnvar\n", scope, expr_var_count);

    printf("DIV %s$*%d %s$*%d_fl1 %s$*%d_fl2\n", scope, expr_var_count, scope, expr_var_count, scope, expr_var_count);
}

void operation_print_symbols(int expr_var_count, Lexeme* sym1, Lexeme* sym2, char* operation, bool comesFromFunction){
    char* scope;
    if (expr_var_count != -1)
    {
        if (comesFromFunction)
        {
            // Nastavení kontextu
            scope = "LF@";
            // Definování překladačové proměnné pro uložení dočasného výsledku
            printf("DEFVAR LF@$*%d\n", expr_var_count);
        }
        else
        {
            scope = "GF@";
            printf("DEFVAR GF@$*%d\n", expr_var_count);
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

void mixed_jump_print_symbols(int expr_var_count, Lexeme* sym1, Lexeme* sym2, char* operation, bool comesFromFunction)
{
    char* scope;
    if (expr_var_count != -1)
    {
        if (comesFromFunction)
        {
            // Nastavení kontextu
            scope = "LF@";
            // Definování překladačové proměnné pro uložení dočasného výsledku
            printf("DEFVAR LF@$*%d\n", expr_var_count);
            printf("DEFVAR LF@$*%d_1\n", expr_var_count);
            printf("DEFVAR LF@$*%d_2\n", expr_var_count);
        }
        else
        {
            scope = "GF@";
            printf("DEFVAR GF@$*%d\n", expr_var_count);
            printf("DEFVAR GF@$*%d_1\n", expr_var_count);
            printf("DEFVAR GF@$*%d_2\n", expr_var_count);
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