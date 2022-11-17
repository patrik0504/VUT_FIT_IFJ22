#include "expr_parser.h"

int expr(context context, p_node symtable)
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