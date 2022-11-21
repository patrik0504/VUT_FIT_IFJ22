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

    push(stack, SYM_STACK_TAG);
    
    l = get_token(symtable);

    if (l.type == SEMICOLON && context == RETURN)
    {
        stack_destroy(stack);
        return 1;
    }

    printf("Pravý rozbor: ");

    //Check ukončení
    while (!should_end(context, &l, stack))
    {
        //printf("While stack: ");
        //stack_print(stack);
        if(l.type == LBRACKET)
        {
            stack->lpar_count += 1;
        }
        if(l.type == RBRACKET)
        {
            stack->lpar_count -= 1;
        }
        if(check_operation(symtable, stack, &l, context) == 0)
        {
            printf("\n");
            stack_destroy(stack);
            return 0;
        }
    }    

    if(context == ASSIGNMENT || context == RETURN)
    {
        l.type = SEMICOLON;
    }

    while (stack->top != 1)
    {
        if(check_operation(symtable, stack, &l, context) == 0)
        {
            printf("\n");
            stack_destroy(stack);
            return 0;
        }
    }
    printf("\n");
    stack_destroy(stack);
    return 1;
}

int check_operation (p_node symtable, p_stack stack, Lexeme *l, context context){
    symbol_type input_symbol = lex_type_to_psa(l);
    int precedence = precedence_lookup(non_terminal_check(stack), input_symbol);
    
    symbol_type op3 = -1;
    symbol_type op2 = -1;
    symbol_type op1 = -1;

    
    switch (precedence)
    {
        // REDUKCE ( > )
    case 2:
        op3 = pop(stack);
        if (peek(stack) != SYM_HANDLE_TAG && (stack->top > 2))
        {
            op2 = pop(stack);
            op1 = pop(stack);
        }
        
        if (peek(stack) != SYM_HANDLE_TAG)
        {
            fprintf(stderr, "Chyba syntaxe 1!\n");
            return 0;
        }
        reduction_rule result = check_rule(op1, op2, op3, stack);
        if (result != RR_None)
        {
            pop(stack); // Pop handle
            push(stack, SYM_NONTERMINAL);
            printf("%d ", result);
            return 1;
        }
        else
        {
            fprintf(stderr, "Chyba syntaxe 2!\n");
            return 0;
        }
        break;

        // ( = )
    case 1:
        push(stack, input_symbol);

        printf("%d ", RR_PAR);
        *l = get_token(symtable);
        return 1;
        break;
        
        // SHIFT ( < )
    case 0:
        push_after_terminal(stack, SYM_HANDLE_TAG);
        push(stack, input_symbol);
        *l = get_token(symtable);
        return 1;
        break;

            
    default:
        fprintf(stderr, "Chyba syntaxe! (%d)\n", precedence);
        // debug 
        // printf("Stack: %s Input: %s\n",symbol_type_err[non_terminal_check(stack)], symbol_type_err[input_symbol]);
        return 0;
    }
}

reduction_rule check_rule(symbol_type op1, symbol_type op2, symbol_type op3, p_stack stack)
{
    // Pravidla s jedním operandem
    if (op1 == -1 && op2 == -1)
    {
        // <term>  -> i
        if (op3 == SYM_ID)
        {
            // TODO: semantická akce
            return RR_ID;
        }
    }

    // Pravidlo pro E <- (E)
    if (op3 == SYM_RPAR && op2 == SYM_NONTERMINAL && op1 == SYM_LPAR)
    {
        return RR_PAR;
    }
    
    //Pravidla se dvěma operandy
    else if (op1 != -1 && op2 != -1 && op3 != -1)
    {
        //Pokud není op1 a op3 neterminál -> neexistuje takové pravidlo               PŘ: (EE+ nejde)
        if (op1 != SYM_NONTERMINAL || op3 != SYM_NONTERMINAL)
        {
            fprintf(stderr, "Neexistující pravidlo pro redukci!\n");
            return RR_None;
        }

        //Switch, který rozhodne o pravidlu redukce na základě operandu v op2
        switch (op2)
        {
        case SYM_MUL:
            return RR_MUL;
        case SYM_DIV:
            return RR_DIV;
        case SYM_PLUS:
            return RR_PLUS;
        case SYM_MINUS:
            return RR_MINUS;
        case SYM_CONCAT:
            return RR_CONCAT;
        case SYM_LESSER:
            return RR_LESSER;
        case SYM_GREATER:
            return RR_GREATER;
        case SYM_LESOREQ:
            return RR_LESOREQ;
        case SYM_GREOREQ:
            return RR_GREOREQ;
        case SYM_EQ:
            return RR_EQ;
        case SYM_NOTEQ:
            return RR_NOTEQ;

        //Pokud se nenajde vhodné pravidlo, neexistuje.
        default:
            fprintf(stderr, "Neexistující pravidlo pro redukci!\n");
            return RR_None;
        }        
    }
    else
    {
        //Neexistuje odpovídající pravidlo
        fprintf(stderr, "Neexistující pravidlo pro redukci!\n");
        return RR_None;
    }

}


int should_end(context context, Lexeme *lexeme, p_stack stack)
{
    //Pro assignment hledáme ;
    if(context == ASSIGNMENT || context == RETURN)
    {
        if(lexeme->type == SEMICOLON)
        {
            return 1;
        }
    }
    //Pro call control hledáme )
    else if (context == CALL_CONTROL)
    {   
        if(stack->lpar_count == 0)
        {
            // Podmínka v hlavním while cyklu přestane platit dříve, než se závorka dostane na stack
            // Ošetřujeme ji proto manuálně zde
            return 1;
        }
    }
    
    // V případě, že se neukončí dříve, vypne se  na EOF
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