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
    printf("DEFVAR LF@returnvar\n");
    printf("READ LF@returnvar string\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    /*****************FUNCTION READI******************/
    printf("LABEL READI\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@returnvar\n");
    printf("READ LF@returnvar int\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    /*****************FUNCTION READF******************/
    printf("LABEL READF\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@returnvar\n");
    printf("READ LF@returnvar float\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    /*****************FUNCTION STRLEN******************/
    printf("LABEL strlen\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@returnvar\n");
    printf("STRLEN LF@returnvar LF@param1\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    /*****************FUNCTION SUBSTRING******************/
    printf("LABEL substring\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@returnvar\n");                        //string returnvar;
    printf("MOVE LF@returnvar string@\n");
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
    printf("CONCAT LF@returnvar LF@returnvar LF@char\n");
    printf("ADD LF@indexcount LF@indexcount int@1\n");
    printf("JUMP whilesubstring\n");

    printf("LABEL errorsubstring\n");
    printf("MOVE LF@returnvar nil@nil\n");

    printf("LABEL endsubstring\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    /*****************FUNCTION ORD******************/
    printf("LABEL ord\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@returnvar\n");
    printf("JUMPIFEQ errorord LF@param1 string@\n");
    printf("STRI2INT LF@returnvar LF@param1 int@0\n");
    printf("JUMP returnord\n");
    printf("LABEL errorord\n");
    printf("MOVE LF@returnvar int@0\n");
    printf("JUMP returnord\n");
    printf("LABEL returnord\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    /*****************FUNCTION FLOATVAL******************/
    printf("LABEL floatval\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@returnvar\n");
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
    printf("MOVE LF@returnvar LF@param1\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    /*****************FUNCTION INTVAL******************/
    printf("LABEL intval\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@returnvar\n");
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
    printf("MOVE LF@returnvar LF@param1\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    /*****************FUNCTION STRVAL******************/
    printf("LABEL strval\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@returnvar\n");
    printf("DEFVAR LF@typ\n");
    printf("TYPE LF@typ LF@param1\n");
    printf("JUMPIFEQ strvalend LF@typ string@string\n");
    printf("JUMPIFEQ strvalnil LF@typ string@nil\n");

    printf("LABEL strvalnil\n");
    printf("MOVE LF@param1 string@\n");
    printf("LABEL strvalend\n");
    printf("MOVE LF@returnvar LF@param1\n");
    printf("POPFRAME\n");
    printf("RETURN\n");



    /*****************FUNCTION CHR******************/
    printf("LABEL chr\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@returnvar\n");
    printf("INT2CHAR LF@returnvar LF@param1\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("\n#HLAVNI TELO\n");
    printf("LABEL MAIN\n");
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
        printf("MOVE LF@$%s TF@returnvar\n",destination);
    } else
    {
        printf("MOVE GF@$%s TF@returnvar\n",destination);
    }
}

void defineNewVar(char *varName, bool comesFromFunction)
{
    if (!comesFromFunction)
    {
        printf("DEFVAR GF@$%s\n", varName);
    } else
    {
        printf("DEFVAR LF@$%s\n", varName);
    }
}

void declareFunction(char *functionName)
{
    printf("\n#FUNCTION %s\n", functionName);
    printf("JUMP %sEND\n", functionName);       //potreba preskocit deklaraci funkce pri vykonavaci hlavniho tela programu
    printf("LABEL %s\n", functionName);
    printf("PUSHFRAME\n");
}

void declareParams(int number, char *varName)
{
    printf("DEFVAR LF@$%s\n", varName);
    printf("MOVE LF@$%s LF@param%d\n", varName, number);
}

void codeGenReturn()
{
    printf("RETURN\n");
}

void codeGenFunctionEnd(char *functionName)
{
    printf("RETURN\n");
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
        printf("MOVE LF@$%s LF@$*%d\n", target, source_var_count);
    }
    else
    {
        printf("MOVE GF@$%s GF@$*%d\n", target, source_var_count);
    }
    
}

void generate_operation(int expr_var_count, Lexeme* sym1, Lexeme* sym2, operation operation, bool comesFromFunction)
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
        if(sym1->type == NUMBER && sym2->type == NUMBER)
        {
            operation_print_symbols(expr_var_count, sym1, sym2, "IDIV", comesFromFunction);
        }
        else if ((sym1->type == DECIMAL_NUMBER && (sym2->type == DECIMAL_NUMBER || sym2->type == EXPONENT_NUMBER || sym2->type == EXPR || sym2->type == VARIABLE_ID)) ||
                (sym2->type == DECIMAL_NUMBER && (sym1->type == DECIMAL_NUMBER || sym1->type == EXPONENT_NUMBER|| sym1->type == EXPR || sym1->type == VARIABLE_ID)))
        {
            operation_print_symbols(expr_var_count, sym1, sym2, "DIV", comesFromFunction);
        }
        else if ((sym1->type == EXPR && sym2->type == NUMBER) || (sym1->type == NUMBER && sym2->type == EXPR) ){
            operation_print_symbols(expr_var_count, sym1, sym2, "IDIV", comesFromFunction);
        }
        else
        {
            // Volání disasteru (Proměnný :D)
            div_decider(sym1, sym2, comesFromFunction, expr_var_count);
            printf("LABEL idiv%d\n", expr_var_count);
            operation_print_symbols(expr_var_count, sym1, sym2, "IDIV", comesFromFunction);
            printf("JUMP divEnd%d\n", expr_var_count);
            printf("LABEL div%d\n", expr_var_count);
            operation_print_symbols(-1, sym1, sym2, "DIV", comesFromFunction);
            printf("LABEL divEnd%d\n", expr_var_count);
        }

        
        break;
        
    case RR_CONCAT: //  .
        operation_print_symbols(expr_var_count, sym1, sym2, "CONCAT", comesFromFunction);
        break;
    // case RR_LESSER: //  <
        // operation_print_symbols(expr_var_count, sym1, sym2, "LT", comesFromFunction);
        // break;
    // case RR_LESOREQ: //  <=
        // operation_print_symbols(expr_var_count, sym1, sym2, "LT", comesFromFunction); // EQ !!!!!!!!
        // break;
    // case RR_GREATER: //  >
        // operation_print_symbols(expr_var_count, sym1, sym2, "GT", comesFromFunction);
        // break;
    // case RR_GREOREQ: //  >=
        // operation_print_symbols(expr_var_count, sym1, sym2, "GT", comesFromFunction); // EQ !!!!!!!!
        // break;
    // case RR_EQ: //  ===
        // operation_print_symbols(expr_var_count, sym1, sym2, "EQ", comesFromFunction);
        // break;
    // case RR_NOTEQ: //  !==
        // operation_print_symbols(expr_var_count, sym1, sym2, "EQ", comesFromFunction);  // NOT EQ !!!!!!!!!!!!
        // break;

    default:
        break;
    }
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

    // Print operace a výstupní proměnné
    printf("%s %s$*%d ", operation, scope, expr_var_count);

    // Print prvního symbolu
    print_single_symbol(sym1, scope);

    // Print druhého symbolu
    print_single_symbol(sym2, scope);
    printf("\n");
}

void print_single_symbol(Lexeme* lexeme, char* scope)
{
    switch (lexeme->type)
    {
    case EXPR: // Enum symbolizující expression (ve value je uloženo číslo compiler proměnné)
        printf("%s$*%d ", scope, lexeme->extra_data.value);
        break;
    case VARIABLE_ID:
        printf("%s$%s ", scope, lexeme->extra_data.string);
        break;
    case NUMBER:
        printf("int@%d ", lexeme->extra_data.value);
        break;
    case DECIMAL_NUMBER:
        printf("float@%a ", lexeme->extra_data.decimal);
        break;
    case EXPONENT_NUMBER:
        printf("float@%a ", lexeme->extra_data.exponent);
        break;
    case STRING_LITERAL:
        evaluateEscapeSequencies(lexeme);
        printf("string@%s ", lexeme->extra_data.string);
        break;
    default:
        break;
    }
}

void div_decider(Lexeme* sym1, Lexeme* sym2, bool comesFromFunction, int expr_var_count)
{
    // DISASTER

    char* scope = "GF@";
    if (comesFromFunction) scope = "LF@";

    printf("DEFVAR %s*type%d_1\n", scope, expr_var_count);
    printf("DEFVAR %s*type%d_2\n", scope, expr_var_count);

    if(sym1->type == VARIABLE_ID)
    {
        printf("TYPE %s*type%d_1 %s$%s\n", scope, expr_var_count, scope, sym1->extra_data.string);
    }
    else if(sym1->type == EXPR)
    {
        printf("TYPE %s*type%d_1 %s$*%d\n", scope, expr_var_count, scope, sym1->extra_data.value);
    }
    else
    {
        printf("MOVE %s*type%d_1 string@none\n", scope, expr_var_count);
    }

    if(sym2->type == VARIABLE_ID)
    {
        printf("TYPE %s*type%d_2 %s$%s\n", scope, expr_var_count, scope, sym2->extra_data.string);
    }
    else if(sym2->type == EXPR)
    {
        printf("TYPE %s*type%d_2 %s$*%d\n", scope, expr_var_count, scope, sym2->extra_data.value);
    }
    else
    {
        printf("MOVE %s*type%d_2 string@none\n", scope, expr_var_count);
    }

    // je 1 int?
    printf("DEFVAR %s*decide%d_1\n", scope, expr_var_count);
    printf("EQ %s*decide%d_1 %s*type%d_1 string@int\n", scope, expr_var_count, scope, expr_var_count);

    // je 2 int?
    printf("DEFVAR %s*decide%d_2\n", scope, expr_var_count);
    printf("EQ %s*decide%d_2 %s*type%d_2 string@int\n", scope, expr_var_count, scope, expr_var_count);

    // je oboje int?
    printf("DEFVAR %s*decide%d_int\n", scope, expr_var_count);
    printf("AND %s*decide%d_int %s*decide%d_1 %s*decide%d_2\n", scope, expr_var_count, scope, expr_var_count, scope, expr_var_count);
    printf("JUMPIFEQ idiv%d %s*decide%d_int bool@true\n", expr_var_count, scope, expr_var_count);
    printf("NOT %s*decide%d_1 %s*decide%d_1\n", scope, expr_var_count, scope, expr_var_count);
    printf("NOT %s*decide%d_2 %s*decide%d_2\n", scope, expr_var_count, scope, expr_var_count);
    printf("JUMPIFEQ div%d %s*decide%d_int bool@true\n", expr_var_count, scope, expr_var_count);
    printf("JUMP divEnd%d\n", expr_var_count);
    // TODO: chyba

}