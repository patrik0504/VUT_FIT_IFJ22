#include "code_gen.h"

int navestiCount = 0;

void printProlog()
{
    printf(".IFJcode22\n");
    return;
}

void codeGenWrite(Lexeme l)
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
            printf("WRITE LF@$%s\n", l.extra_data.string);
            break;
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
    printf("WRITE LF@returnvar\n");
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
    printf("WRITE string@chyba\n");

    printf("LABEL endsubstring\n");
    printf("WRITE LF@returnvar\n");
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
    printf("WRITE LF@returnvar\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    /*****************FUNCTION CHR******************/
    printf("LABEL chr\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@returnvar\n");
    printf("INT2CHAR LF@returnvar LF@param1\n");
    printf("WRITE LF@returnvar\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("\n#HLAVNI TELO\n");
    printf("LABEL MAIN\n");
}


void codeGenReads()
{
    printf("CREATEFRAME\n");
    printf("CALL READS\n");

    //DEBUG PRINT
    printf("WRITE TF@returnvar\n");
}

void codeGenReadi()
{
    printf("CREATEFRAME\n");
    printf("CALL READI\n");

    //DEBUG PRINT
    printf("WRITE TF@returnvar\n");
}

void codeGenReadf()
{
    printf("CREATEFRAME\n");
    printf("CALL READF\n");

    //DEBUG PRINT
    printf("WRITE TF@returnvar\n");
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
            //TODO udelat volani s promennyma, ne jen s konstantama
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

void defineNewVar(char *varName, bool comesFromFunction)
{
    if (!comesFromFunction)
    {
        printf("DEFVAR GF@$%s\n", varName);
        //TODO PRIRADIT HODNOTY
    } else
    {
        printf("DEFVAR LF@$%s\n", varName);
        //TODO PRIRADIT HODNOTY
    }
}

void declareFunction(char *functionName)
{
    printf("\n#FUNCTION %s\n", functionName);
    printf("JUMP %sEND\n", functionName);
    printf("LABEL %s\n", functionName);
    printf("PUSHFRAME\n");
}

void declareParams(int number, char *varName)
{
    printf("DEFVAR LF@$%s\n", varName);
    printf("MOVE LF@$%s LF@param%d\n", varName, number);
}

void codeGenReturn(bool comesFromFunction, char *functionName)
{
    printf("RETURN\n");
    if (comesFromFunction)
    {
        printf("LABEL %sEND\n\n", functionName);
    }
}

void expr_move(char* target, int source_var_count, bool comesFromFunction)
{
    if (comesFromFunction)
    {
        printf("MOVE LF@$%s LF@$expr%d\n", target, source_var_count);
    }
    else
    {
        printf("MOVE GF@$%s GF@$expr%d\n", target, source_var_count);
    }
    
}

void generate_operation(int expr_var_count, Lexeme* sym1, Lexeme* sym2, operation operation, bool comesFromFunction)
{
    
    //Switch pro generaci jednotlivých operací
    switch (operation)
    {
    case RR_PLUS:
        operation_print_symbols(expr_var_count, sym1, sym2, "ADD", comesFromFunction);
        //printf("ADD %s$expr%s %s%s %s%s\n", scope, expr_var_count, var1, sym1->extra_data.string, var2, sym2->extra_data.string);
        break;
        
    case RR_MINUS: //  -
        operation_print_symbols(expr_var_count, sym1, sym2, "SUB", comesFromFunction);
        break;

    // case RR_MUL: //  *
        // operation_print_symbols(expr_var_count, sym1, sym2, "MUL", comesFromFunction);
        // break;
        
    // case RR_DIV: //  /
        // operation_print_symbols(expr_var_count, sym1, sym2, "DIV", comesFromFunction);
        // break;
        
    // case RR_CONCAT: //  .
        // operation_print_symbols(expr_var_count, sym1, sym2, "CONCAT", comesFromFunction);
        // break;
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
    if (comesFromFunction)
    {
        scope = "LF@";
        printf("DEFVAR LF@$expr%d\n", expr_var_count);
    }
    else
    {
        scope = "GF@";
        printf("DEFVAR GF@$expr%d\n", expr_var_count);
    }

    char* var1 = "";
    char* varscope1 = "";
    char* var2 = "";
    char* varscope2 = "";
    char number[12];
    char number2[12];

    // ------- Nastavení scopu a typu u proměnných --------------
    if(sym1->type == VARIABLE_ID) 
    {
        varscope1 = scope;
        var1 = "$";
    }
    else if(sym1->type == NUMBER)
    {
        varscope1 = "int@";
        sprintf(number, "%d ", sym1->extra_data.value);
        var1 = number;
    }

    if(sym2->type == VARIABLE_ID) 
    {
        varscope2 = scope;
        var2 = "$";
    }
    else if(sym2->type == NUMBER)
    {
        varscope2 = "int@";
        sprintf(number2, "%d ", sym2->extra_data.value);
        var2 = number2;
    }
    // ---------------------------------------------------------




    if(sym1->type == EQUAL) 
    {
        varscope1 = scope;
        var1 = "$expr";
    }

    if(sym2->type == EQUAL) 
    {
        varscope2 = scope;
        var2 = "$expr";
    }

    printf("%s %s$expr%d %s%s", operation, scope, expr_var_count, varscope1, var1);
    if(sym1->type == EQUAL)
    {
        printf("%d ", sym1->extra_data.value);
    }
    else if(sym1->type == VARIABLE_ID)
    {
        printf("%s ", sym1->extra_data.string);
    }
    printf("%s%s", varscope2, var2);
    if(sym2->type == EQUAL)
    {
        printf("%d", sym2->extra_data.value);
    }
    else if(sym2->type == VARIABLE_ID)
    {
        printf("%s", sym2->extra_data.string);
    }
    printf("\n");
}