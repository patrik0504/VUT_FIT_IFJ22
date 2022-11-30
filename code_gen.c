#include "code_gen.h"

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
    /*****************FUNCTION ORD******************/
    printf("LABEL ord\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@returnvar\n");
    printf("DEFVAR LF@temp\n");
    printf("GETCHAR LF@temp LF@param1 int@0\n");
    printf("STRI2INT LF@returnvar LF@temp int@0\n");
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
    printf("JUMPIFEQ error LF@check bool@true\n");
    printf("LT LF@check LF@param3 int@0\n");                // j<0
    printf("JUMPIFEQ error LF@check bool@true\n");
    printf("GT LF@check LF@param2 LF@param3\n");            // i>j
    printf("JUMPIFEQ error LF@check bool@true\n");
    printf("DEFVAR LF@length\n");
    printf("STRLEN LF@length LF@param1\n");
    printf("LT LF@check LF@param2 LF@length\n");            // i>=strlen($s)
    printf("JUMPIFNEQ error LF@check bool@true\n");
    printf("GT LF@check LF@param3 LF@length\n");            // j>strlen($s)
    printf("JUMPIFEQ error LF@check bool@true\n");

    printf("LABEL whilesubstring\n");
    printf("EQ LF@greaterthen LF@indexcount LF@param3\n");
    printf("JUMPIFEQ end LF@greaterthen bool@true\n");
    printf("GETCHAR LF@char LF@param1 LF@indexcount\n");
    printf("CONCAT LF@returnvar LF@returnvar LF@char\n");
    printf("ADD LF@indexcount LF@indexcount int@1\n");
    printf("JUMP whilesubstring\n");

    printf("LABEL error\n");
    printf("MOVE LF@returnvar nil@nil\n");
    printf("WRITE string@chyba\n");

    printf("LABEL end\n");
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

void codeGenOrd()
{
    printf("CREATEFRAME\n");
    printf("CALL ORD\n");

    //DEBUG PRINT
    printf("WRITE TF@returnvar\n");
}