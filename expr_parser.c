#include "expr_parser.h"

int negative_par = 0;
static int expr_var_counter = 0;

int precedence_lookup(symbol_type stack_symbol, symbol_type input)
{
    // Zabraňujeme přístupu do tabulky mimo její rozsah
    if (stack_symbol >= 16 || input >= 16 || stack_symbol < 0 || input < 0)
    {
        return -1;
    }
    // printf("Stack: %d Input: %d\n", stack_symbol, input);
    /** Precedenční tabulka jako dvojrozměrný array (viz. dokumentace).
     *  @return  2: '>'
     *  @return  1: '='
     *  @return  0: '<'
     *  @return -1: chybový stav
    */
    static const int precedence[16][16] = {
        //  *   /   +   -   .   <   >   <=  >=  === !==  (   )   i   $  NULL
        {   2,  2,  2,  2,  2,  2,  2,  2,  2,   2,  2,  0,  2,  0,  2,  0   }, //  *
        {   2,  2,  2,  2,  2,  2,  2,  2,  2,   2,  2,  0,  2,  0,  2,  0   }, //  /
        {   0,  0,  2,  2,  2,  2,  2,  2,  2,   2,  2,  0,  2,  0,  2,  0   }, //  +
        {   0,  0,  2,  2,  2,  2,  2,  2,  2,   2,  2,  0,  2,  0,  2,  0   }, //  -
        {   0,  0,  2,  2,  2,  2,  2,  2,  2,   2,  2,  0,  2,  0,  2,  0   }, //  .
        {   0,  0,  0,  0,  0,  2,  2,  2,  2,   2,  2,  0,  2,  0,  2,  0   }, //  <
        {   0,  0,  0,  0,  0,  2,  2,  2,  2,   2,  2,  0,  2,  0,  2,  0   }, //  >
        {   0,  0,  0,  0,  0,  2,  2,  2,  2,   2,  2,  0,  2,  0,  2,  0   }, //  <=
        {   0,  0,  0,  0,  0,  2,  2,  2,  2,   2,  2,  0,  2,  0,  2,  0   }, //  >=
        {   0,  0,  0,  0,  0,  0,  0,  0,  0,   2,  2,  0,  2,  0,  2,  0   }, //  ===
        {   0,  0,  0,  0,  0,  0,  0,  0,  0,   2,  2,  0,  2,  0,  2,  0   }, //  !==
        {   0,  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  1,  0, -1,  0   }, //  (
        {   2,  2,  2,  2,  2,  2,  2,  2,  2,   2,  2, -1,  2, -1,  2, -1   }, //  )
        {   2,  2,  2,  2,  2,  2,  2,  2,  2,   2,  2, -1,  2, -1,  2, -1   }, //  i
        {   0,  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0, -1,  0, -1,  0   }, //  $
        {   2,  2,  2,  2,  2,  2,  2,  2,  2,   2,  2, -1,  2, -1,  2, -1   }  //  NULL


    };

    return precedence[stack_symbol][input];
}

int expr(context context, int jump_label, p_node symtable, Lexeme *target, char * variable_name, 
    p_node globalFunctions, bool comesFromFunction, p_node functionPtr)
{
    p_stack stack = stack_init(PSA_STACK_SIZE);
    p_lex_stack lex_stack = lex_stack_init(LEX_STACK_SIZE);
    Lexeme l = {.type = NULLLEX};

    push(stack, SYM_STACK_TAG);
    
    l = get_token(symtable);

    if(l.type == FUNCTION_ID)
    {
        
        int res_func = statement(&l, symtable, globalFunctions, comesFromFunction, functionPtr);
        if(!res_func)
        {
            return 0;
        }
        else
        {
            returnVariable(variable_name, comesFromFunction);
            return 1;
        }
    }

    if (l.type == SEMICOLON && context == RETURN)
    {
        stack_destroy(stack);
        return 1;
    }
    if (context == RETURN)
    {
        context = ASSIGNMENT;
        variable_name = "**returnvar";
    }

    //printf("Pravý rozbor: ");

    //Check ukončení
    while (!should_end(context, &l, stack))
    {
        ////printf("While stack: ");
        //stack_print(stack);
        if(l.type == LBRACKET)
        {
            stack->lpar_count += 1;
        }
        if(l.type == RBRACKET)
        {
            stack->lpar_count -= 1;
        }
        if(check_operation(symtable, stack, lex_stack, &l, context, jump_label, comesFromFunction, functionPtr, globalFunctions) == 0)
        {
            //printf("\n");
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
        if(check_operation(symtable, stack, lex_stack, &l, context, jump_label, comesFromFunction, functionPtr, globalFunctions) == 0)
        {
            //printf("\n");
            stack_destroy(stack);
            return 0;
        }
    }

    if (context == ASSIGNMENT || context == RETURN)
    {
        char* scope = "GF@";
        if(comesFromFunction)
        {
            scope = "LF@";
        }
        // stack_print(stack);
        //Řešení typu u assigmentu
        Lexeme * last = lexStack_pop(lex_stack);
        // printf("last: %d\n", last->type);
        char* identifier = "";
        if (strcmp(variable_name, "**returnvar") != 0)
        {
            identifier = "$";
        }
        switch (last->type)
        {
        case VARIABLE_ID:
            printf("MOVE %s%s%s %s$%s\n", scope, identifier, variable_name, scope, last->extra_data.string);
            break;
        case EXPR:
            expr_move(variable_name, last->extra_data.value, comesFromFunction);
            break;
        case NUMBER:
            printf("MOVE %s%s%s int@%d\n", scope, identifier, variable_name, last->extra_data.value);
            break;
        case STRING_LITERAL:
            printf("MOVE %s%s%s string@%s\n", scope, identifier, variable_name, last->extra_data.string);
            break;
        case DECIMAL_NUMBER:
        case EXPONENT_NUMBER:
            printf("MOVE %s%s%s float@%a\n", scope, identifier, variable_name, last->extra_data.decimal);
            break;
        
        default:
            break;
        }
    }

    //printf("\n");
    stack_destroy(stack);
    lexStack_stack_destroy(lex_stack);
    return 1;
}

int check_operation (p_node symtable, p_stack stack, p_lex_stack lex_stack, Lexeme *l, context context, int jump_label,
bool comesFromFunction, p_node functionPtr, p_node globalFunctions)
{
    if (l->type == VARIABLE_ID || l->type == STRING_LITERAL || l->type == NUMBER ||
        l->type == DECIMAL_NUMBER || l->type == EXPONENT_NUMBER || l->type == KW_NULL)
    {
        if(l->type == VARIABLE_ID)
        {
            int check_defined = 0;
            if(comesFromFunction)
            {
                check_defined = check_if_variable_is_defined(functionPtr, l->extra_data.string);
            }
            else
            {
                check_defined = check_if_variable_is_defined(globalFunctions, l->extra_data.string);
            }
            if(!check_defined)
            {
                error(l->row, "Premmená vo výraze nie je definovaná", SEM_UNDEFINED_VAR_ERROR);
            }
        }
        lexStack_push(lex_stack, l);
        //printf("%d\n", lexStack_pop(lex_stack)->type);
    }
    
    symbol_type input_symbol = lex_type_to_psa(l);

    //Řešení záporných hodnot
    if (input_symbol == SYM_MINUS)
    {
        if (peek(stack) != SYM_ID && peek(stack) != SYM_NONTERMINAL)
        {
            *l = get_token(symtable);

            //Mínus před závorkou   2*-(3+6)
            if (l->type == LBRACKET)
            {
                negative_par = 1;
            }
            //Mínus před hodnotou   2+-3
            else 
            {
                l->negative_num = true;
            }
            
            return 1;
        }
        
    }

    int precedence = precedence_lookup(non_terminal_check(stack), input_symbol);
    
    symbol_type op3 = -1;
    symbol_type op2 = -1;
    symbol_type op1 = -1;

    // 1 + (-2)

    //VALID:     2-(-2),
    //NOT VALID: 2--2, 

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
            error(l->row, "Ve výrazu chybí některý z operandů!", SYNTAX_ERROR);
            return 0;
        }
        reduction_rule result = check_rule(op1, op2, op3, stack, lex_stack, comesFromFunction, context, jump_label, functionPtr, globalFunctions);
        if (result != 0)
        {
            pop(stack); // Pop handle
            push(stack, SYM_NONTERMINAL);
            //printf("%d ", result);
            return 1;
        }
        else
        {
            error(l->row, "Ve výrazu byla nalezena neznámá operace!", SYNTAX_ERROR);
            return 0;
        }
        break;

        // ( = )
    case 1:
        push(stack, input_symbol);

        //printf("%d ", RR_PAR);
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
            // Zabraňujeme přístupu do tabulky mimo její rozsah
        if (input_symbol >= 18 || input_symbol < 0)
        {
            error(l->row, "Výraz nebyl správně ukončen, nebo se ve výrazu nachází neznámý symbol!", SYNTAX_ERROR);
            return 0;
        }

        error(l->row, "Chyba v posloupnosti symbolů v rámci výrazu!", SYNTAX_ERROR);
        // debug 
        // printf("Stack: %s Input: %s\n",symbol_type_err[non_terminal_check(stack)], symbol_type_err[input_symbol]);
        return 0;
    }
}

reduction_rule check_rule(symbol_type op1, symbol_type op2, symbol_type op3, p_stack stack, p_lex_stack lex_stack, 
    bool comesFromFunction, context context, int jump_label, p_node functionPtr, p_node globalFunctions)
{
    // Pravidla s jedním operandem
    if (op1 == -1 && op2 == -1)
    {
        // <term>  -> i
        if (op3 == SYM_ID || op3 == SYM_NULL)
        {
            // TODO: semantická akce
            return RR_ID;

        }
    }

    // Pravidlo pro E <- (E)
    if (op3 == SYM_RPAR && op2 == SYM_NONTERMINAL && op1 == SYM_LPAR)
    {
        if(negative_par == 1){
                //printf("NEG_PAR ");
                lexStack_peek(lex_stack)->negative_num = true;
                negative_par = 0;
        }

        return RR_PAR;
    }
    
    //Pravidla se dvěma operandy
    else if (op1 != -1 && op2 != -1 && op3 != -1)
    {
        //2+(-3)
        //Pokud není op1 a op3 neterminál -> neexistuje takové pravidlo               PŘ: (EE+ nejde)
        if (op1 != SYM_NONTERMINAL || op3 != SYM_NONTERMINAL)
        {
            return 0;
        }
        //printf("%d\n", lexStack_pop(lex_stack)->type);
        //printf("%d\n", lexStack_pop(lex_stack)->type);

        Lexeme *sym2 = lexStack_pop(lex_stack);
        Lexeme *sym1 = lexStack_pop(lex_stack);

        //Switch, který rozhodne o pravidlu redukce na základě operandu v op2
        switch (op2)
        {
        case SYM_MUL:
            if (type_check(sym1, sym2) == 1)
            {
                generate_operation(expr_var_counter, sym1, sym2, RR_MUL, comesFromFunction, context, jump_label, functionPtr, globalFunctions);
            }
            break;
        case SYM_DIV:
            generate_operation(expr_var_counter, sym1, sym2, RR_DIV, comesFromFunction, context, jump_label, functionPtr, globalFunctions);
            break;
        case SYM_PLUS:
            if (type_check(sym1, sym2) == 1)
            {
                generate_operation(expr_var_counter, sym1, sym2, RR_PLUS, comesFromFunction, context, jump_label, functionPtr, globalFunctions);
            }
            break;
        case SYM_MINUS:
            if (type_check(sym1, sym2) == 1)
            {
                generate_operation(expr_var_counter, sym1, sym2, RR_MINUS, comesFromFunction, context, jump_label, functionPtr, globalFunctions);
            }
            break;
        case SYM_CONCAT:
            //TODO: Zkontrolovat kompatibilitu datových typů u proměnných
            if (sym1->type == STRING_LITERAL && sym2->type == STRING_LITERAL)
            {
                generate_operation(expr_var_counter, sym1, sym2, RR_CONCAT, comesFromFunction, context, jump_label, functionPtr, globalFunctions);
            }
            else
            {
                error(sym1->row, "Operace konkatenace je možná pouze pro datové typy string!", SEM_INVALID_TYPE_ERROR);
            }
            break;
        case SYM_LESSER:
            if (context == ASSIGNMENT || context == RETURN)
            {
                error(sym1->row, 
                "Logické operátory nemohou být použity v rámci přiřazení do proměnné!", SEM_INVALID_TYPE_ERROR);
                break;
            }
            generate_operation(expr_var_counter, sym1, sym2, RR_LESSER, comesFromFunction, context, jump_label, functionPtr, globalFunctions);
            break;
        case SYM_GREATER:
            if (context == ASSIGNMENT || context == RETURN)
            {
                error(sym1->row, 
                "Logické operátory nemohou být použity v rámci přiřazení do proměnné!", SEM_INVALID_TYPE_ERROR);
                break;
            }
            generate_operation(expr_var_counter, sym1, sym2, RR_GREATER, comesFromFunction, context, jump_label, functionPtr, globalFunctions);
            break;
        case SYM_LESOREQ:
            if (context == ASSIGNMENT || context == RETURN)
            {
                error(sym1->row, 
                "Logické operátory nemohou být použity v rámci přiřazení do proměnné!", SEM_INVALID_TYPE_ERROR);
                break;
            }
            generate_operation(expr_var_counter, sym1, sym2, RR_LESOREQ, comesFromFunction, context, jump_label, functionPtr, globalFunctions);
            break;
        case SYM_GREOREQ:
            if (context == ASSIGNMENT || context == RETURN)
            {
                error(sym1->row, 
                "Logické operátory nemohou být použity v rámci přiřazení do proměnné!", SEM_INVALID_TYPE_ERROR);
                break;
            }
            generate_operation(expr_var_counter, sym1, sym2, RR_GREOREQ, comesFromFunction, context, jump_label, functionPtr, globalFunctions);
            break;
        case SYM_EQ:
            if (context == ASSIGNMENT || context == RETURN)
            {
                error(sym1->row, 
                "Logické operátory nemohou být použity v rámci přiřazení do proměnné!", SEM_INVALID_TYPE_ERROR);
                break;
            }
            generate_operation(expr_var_counter, sym1, sym2, RR_EQ, comesFromFunction, context, jump_label, functionPtr, globalFunctions);
            break;
        case SYM_NOTEQ:
            if (context == ASSIGNMENT || context == RETURN)
            {
                error(sym1->row, 
                "Logické operátory nemohou být použity v rámci přiřazení do proměnné!", SEM_INVALID_TYPE_ERROR);
                break;
            }
            generate_operation(expr_var_counter, sym1, sym2, RR_NOTEQ, comesFromFunction, context, jump_label, functionPtr, globalFunctions);
            break;

        //Pokud se nenajde vhodné pravidlo, neexistuje.
        default:
            return 0;
        }

        Lexeme expr_lex = {.type = EXPR};

        expr_lex.extra_data.value = expr_var_counter;
        expr_var_counter++;
        lexStack_push(lex_stack, &expr_lex);
        return 1;
    }
    else
    {
        //Neexistuje odpovídající pravidlo
        return 0;
    }

}

int type_check(Lexeme *sym1, Lexeme *sym2)
{
    if (sym1->type == NUMBER && sym2->type == NUMBER)
    {
        return 1;
    }
    else if (sym1->type == DECIMAL_NUMBER && (sym2->type == DECIMAL_NUMBER || sym2->type == EXPONENT_NUMBER))
    {
        return 1;
    }
    else if (sym1->type == EXPONENT_NUMBER && (sym2->type == DECIMAL_NUMBER || sym2->type == EXPONENT_NUMBER))
    {
        return 1;
    }
    // ODTUD DOČASNÝ KÓD
    else if (sym1->type == VARIABLE_ID || sym2->type == VARIABLE_ID)
    {
        //TODO: Zkontrolovat kompatibilitu datových typů u proměnných
        return 1;
    }
    else if (sym1->type == EXPR || sym2->type == EXPR)
    {
        //TODO: Vkládání redukovaných expressions buď jako VARIABLE_ID včetně typu do stromu
        //      nebo přímo do kódu jako konstanty.
        return 1;
    }
    else if (sym1->type == KW_NULL || sym2->type == KW_NULL)
    {
        return 1;
    }
    // POTUD DOČASNÝ KÓD
    else 
    {
        error(sym1->row, "Ve výrazu se nachází nekompatibilní datové typy!", SEM_INVALID_TYPE_ERROR);
        return 0;
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
    //Pro if a while hledáme )
    else if (context == IF || context == WHILE)
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
        error(lexeme->row, "Nějaký výraz nebyl správně ukončen - syntaktická analýza byla ukončena.", SYNTAX_ERROR);
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
    case KW_NULL:
        return SYM_NULL;
    case SEMICOLON:
        return SYM_STACK_TAG;
    default:
        return -1;
    }
}
