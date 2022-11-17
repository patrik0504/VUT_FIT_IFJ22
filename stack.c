#include "stack.h"

p_stack stack_init(unsigned size)
{
    p_stack stack = (p_stack)malloc(sizeof(struct stack));
    if (stack == NULL)
    {
        return NULL;
    }

    stack->size = size;
    stack->top = -1;
    stack->array = (symbol_type *)malloc(stack->size * sizeof(symbol_type));
    return stack;
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
    default:
        fprintf(stderr, "[PSA] Nevalidní znak v rámci výrazu! (%d) \n", lexeme->type);
        return -1;
    }
}

int checkOverflow(p_stack stack)
{
    if (stack->top == stack->size - 1)
    {
        fprintf(stderr, "\
        Chyba při vkládání do stacku: Bylo zabráněno přetečení\n");
        return 0;
    }
    return 1;
}

int checkUnderflow(p_stack stack)
{
    if (stack->top == -1)
    {
        fprintf(stderr, "\
        Chyba při mazání ze stacku: Bylo zabráněno podtečení\n");
        return 0;
    }
    return 1;
}

int is_full(p_stack stack)
{
    if (stack->top == stack->size - 1)
    {
        return 1;
    }
    return 0;
}

int is_empty(p_stack stack)
{
    if (stack->top == -1)
    {
        return 1;
    }
    return 0;
}

void push(p_stack stack, symbol_type data)
{
    if (checkOverflow(stack))
    {
        stack->top = stack->top + 1;
        stack->array[stack->top] = data;
    }
    else
    {
        return;
    }
}

symbol_type pop(p_stack stack)
{
    if (checkUnderflow(stack))
    {
        symbol_type symbol = stack->array[stack->top];
        stack->top = stack->top - 1;
        return symbol;
    }
    else
    {
        return -1;
    }
}

void stack_destroy(p_stack stack)
{
    if (stack != NULL)
    {
        if (stack->array != NULL)
        {
            free(stack->array);
        }
        free(stack);
    }
}