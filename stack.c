#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
    stack->array = (int *)malloc(stack->size * sizeof(int));
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

void push(p_stack stack, int data)
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

void pop(p_stack stack, int data)
{
    if (checkUnderflow(stack))
    {
        stack->top = stack->top - 1;
    }
    else
    {
        return;
    }
}