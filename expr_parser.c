#include "expr_parser.h"

int precedence_lookup(symbol_type stack_symbol, symbol_type input)
{
    /** Precedenční tabulka jako dvojrozměrný array (viz. dokumentace).
     *  @return  2: '>'
     *  @return  1: '='
     *  @return  0: '<'
     *  @return -1: chybový stav
    */
    static const int precedence[15][15] = {
        //  *   /   +   -   .   <   >   <=  >=  === !== (   )   i   $
        {   2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  0,  2,  0,  2 }, //  *
        {   2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  0,  2,  0,  2 }, //  /
        {   0,  0,  2,  2,  2,  2,  2,  2,  2,  2,  2,  0,  2,  0,  2 }, //  +
        {   0,  0,  2,  2,  2,  2,  2,  2,  2,  2,  2,  0,  2,  0,  2 }, //  -
        {   0,  0,  2,  2,  2,  2,  2,  2,  2,  2,  2,  0,  2,  0,  2 }, //  .
        {   0,  0,  0,  0,  0,  2,  2,  2,  2,  2,  2,  0,  2,  0,  2 }, //  <
        {   0,  0,  0,  0,  0,  2,  2,  2,  2,  2,  2,  0,  2,  0,  2 }, //  >
        {   0,  0,  0,  0,  0,  2,  2,  2,  2,  2,  2,  0,  2,  0,  2 }, //  <=
        {   0,  0,  0,  0,  0,  2,  2,  2,  2,  2,  2,  0,  2,  0,  2 }, //  >=
        {   0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  2,  0,  2,  0,  2 }, //  ===
        {   0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  2,  0,  2,  0,  2 }, //  !==
        {   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0, -1 }, //  (
        {   2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, -1,  2, -1,  2 }, //  )
        {   2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, -1,  2, -1,  2 }, //  i
        {   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,  0, -1 }  //  $
    };

    return precedence[stack_symbol][input];
}

int expr(context context, p_node symtable, Lexeme *target)
{
    p_stack stack = stack_init(256);
    Lexeme l = {.type = NULLLEX};

    while (!should_end(context, &l, stack))
    {
        l = get_token(symtable);
        if(l.type == LBRACKET)
        {
            stack->lpar_count += 1;
        }
        if(l.type == RBRACKET)
        {
            stack->lpar_count -= 1;
        }

        symbol_type symbol = lex_type_to_psa(&l);
        if (symbol == -1)
        {
            fprintf(stderr, "Nevalidní znak v rámci výrazu! (%d) \n", l.type);
            stack_destroy(stack);
            return 0;
        }     
        push(stack, lex_type_to_psa(&l));
    }

    while (!is_empty(stack))
    {
        symbol_type symbol = pop(stack);
        printf("%d ", symbol);
    }
    printf("\n");
    stack_destroy(stack);
    return 1;
}

int should_end(context context, Lexeme *lexeme, p_stack stack)
{
    if(context == ASSIGNMENT)
    {
        if(lexeme->type == SEMICOLON)
        {
            if(stack->lpar_count != 1)
            {
                fprintf(stderr, "Ve výrazu chybí znak ')'!\n");
            }
            return 1;
        }
    }
    else if (context == CALL_CONTROL)
    {
        if(stack->lpar_count == 0)
        {
            return 1;
        }
    }
    
    if (lexeme->type == LEXEOF)
    {
        fprintf(stderr, "Chyba ukončení výrazu!\n");
        return 1;
    }
    return 0;
}

symbol_type lex_type_to_psa(Lexeme *lexeme)
{
    switch (lexeme->type)
    {
    case MULTIPLY:
        return SYM_MUL;
    case DIVIDE:
        return SYM_DIV;
    case PLUS:
        return SYM_PLUS;
    case MINUS:
        return SYM_MINUS;
    case KONKATENACE:
        return SYM_CONCAT;
    case LESS:
        return SYM_LESSER;
    case GREATER:
        return SYM_GREATER;
    case LESSEQUAL:
        return SYM_LESOREQ;
    case GREATEREQUAL:
        return SYM_GREOREQ;
    case EQUAL3:
        return SYM_EQ;
    case NOTEQUAL:
        return SYM_NOTEQ;
    case LBRACKET:
        return SYM_LPAR;
    case RBRACKET:
        return SYM_RPAR;
    case FUNCTION_ID:
    case VARIABLE_ID:
    case STRING_LITERAL:
    case NUMBER:
    case DECIMAL_NUMBER:
    case EXPONENT_NUMBER:
        return SYM_ID;
    case SEMICOLON:
        return SYM_STACK_TAG;
    default:
        return -1;
    }
}
