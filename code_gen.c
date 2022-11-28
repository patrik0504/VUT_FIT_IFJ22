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

    printf("LABEL MAIN\n");
}


void codeGenReads(Lexeme l)
{
    printf("CREATEFRAME\n");
    printf("CALL READS\n");

    //DEBUG PRINT
    printf("WRITE TF@returnvar\n");
}

void codeGenReadi(Lexeme l)
{
    printf("CREATEFRAME\n");
    printf("CALL READI\n");

    //DEBUG PRINT
    printf("WRITE TF@returnvar\n");
}

void codeGenReadf(Lexeme l)
{
    printf("CREATEFRAME\n");
    printf("CALL READF\n");

    //DEBUG PRINT
    printf("WRITE TF@returnvar\n");
}