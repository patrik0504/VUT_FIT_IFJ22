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
    stack->lpar_count = 1; // Hodnota relevantní při kontrole parametrů funkce a podmínek
    stack->array = (symbol_type *)malloc(stack->size * sizeof(symbol_type));
    return stack;
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

symbol_type non_terminal_check(p_stack stack)
{
    symbol_type symbol = stack->array[stack->top];
    if (symbol == SYM_NONTERMINAL)
    {
        symbol = stack->array[stack->top-1];
    }
    return symbol;
}

void push_after_terminal(p_stack stack, symbol_type data)
{
    int index = stack->top; // index = 1 
    symbol_type symbol = stack->array[index]; // symbol = výraz
    if (symbol == SYM_NONTERMINAL)            // symbol - je nonterminál
    {
        symbol = stack->array[--index];       // symbol = $ , index == 0
    }

    for (int i = stack->top; i > index; i--)  //i = 1 > 0  i -- 
    {
        stack->array[i+i] = stack->array[i]; 
    }
    stack->top += 1;
    stack->array[index+1] = data;
}

symbol_type peek(p_stack stack)
{
    if (checkUnderflow(stack))
    {
        return stack->array[stack->top];
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

void stack_print(p_stack stack){
    int i = 0;
    while (i <= stack->top)
    {
        printf("%s ", symbol_type_err[stack->array[i]]);
        i++;
    }
    printf("\n");
}