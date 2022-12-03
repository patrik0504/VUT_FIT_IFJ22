#include <stdio.h>
#include <stdlib.h>

#include "scanner.h"
#include "symtable.h"
#include "stack.h"
#include "parser.h"
#include "error.h"

// TODO: oddělat (přijde do parseru)
#include "expr_parser.h"

void bst_test() 
{

    //TEST-DATA
    p_data data = (p_data)malloc(sizeof(struct data));
    data->data = 69;

    p_data data1 = (p_data)malloc(sizeof(struct data));
    data1->data = 420;

    p_data data2 = (p_data)malloc(sizeof(struct data));
    data2->data = 360;

    p_data data3 = (p_data)malloc(sizeof(struct data));
    data3->data = 666;

    p_data data4 = (p_data)malloc(sizeof(struct data));
    data4->data = 158;
    //TEST-DATA-END

    //default root
    p_node root = NULL;
    root = node_init(data, "a0");

    //TEST-NODE-INIT
    p_node node = node_init(data1, "a");
    insert_node(root, node);

    p_node node2 = node_init(data2, "aa");
    insert_node(root, node2);

    p_node node3 = node_init(data3, "aaa");
    insert_node(root, node3);

    p_node node4 = node_init(data4, "aaaa");
    insert_node(root, node4);
    //TEST-NODE-END

    // TEST-SEARCH
    p_node found = tree_search(root, "aaaa");
    printf("Data found prvku: %d\n", found->data->data);
    printf("Key found prvku: %s\n", found->key);
    // TEST-SEARCH-END

    debug_tree(root);
    printf("\n");
    tree_destroy(root);
}

int main() {
    error_list_init();
    
    int some = 0;
    some = parse();

    //error(1, "Token retrn nebylo možné zpracovat! (Možná jste mysleli return?)", LEX_ERROR);
    //error(69, "U výrazu chybí ';' pro ukončení!", SYNTAX_ERROR);
    //error(112, "Volání funkce neodpovídá její deklaraci!", SEM_INVALID_CALL_ERROR);
    //error(360, "Typy operandů výrazu nejsou navzájem kompatibilní!", SEM_INVALID_TYPE_ERROR);


    /*
    p_node binaryTree = init_binary_treeKW();

    Lexeme l = {.type = NULLLEX};

    while(l.type != LEXEOF)
    {
        l = get_token(binaryTree);
        printf("Row of lexeme: %d\n", l.row);
        //printf("TYPE = %d\n", l.type);

        //DEBUGOVACI PRINTY, MUZE SE SMAZAT, JEN PRO PREDSTAVU JAK TO VRACI TOKENY
        if (l.type == NUMBER)
        {
            printf("lexem je %d\n", l.extra_data.value);
        } else if (l.type == EXPONENT_NUMBER)
        {
            printf("lexem je  %f\n", l.extra_data.exponent);
        } else if (l.type == DECIMAL_NUMBER)
        {
            printf("lexem je %f\n", l.extra_data.decimal);
        }else if (l.type == COMMA)
        {
            printf("lexem je carka\n");
        } else if (l.type == PROLOG)
        {
            printf("lexem je <?php\n");
        }else if (l.type == FILE_END_SIGN)
        {
            printf("lexem je ?>\n");
        }else if (l.type == KW_WHILE)
        {
            printf("keyword je WHILE\n");
        }else if (l.type == KW_IF)
        {
            printf("keyword je IF\n");
        }
        else if (l.type == KW_ELSE)
        {
            printf("keyword je ELSE\n");
        }
        else if (l.type == KW_NULL)
        {
            printf("keyword je NULL\n");
        }
        else if (l.type == KW_RETURN)
        {
            printf("keyword je RETURN\n");
        }
        else if (l.type == KW_VOID)
        {
            printf("keyword je VOID\n");
        }
        else if (l.type == KW_INT)
        {
            printf("keyword je INT\n");
        }
        else if (l.type == KW_FLOAT)
        {
            printf("keyword je FLOAT\n");
        }
        else if (l.type == KW_STRING)
        {
            printf("keyword je STRING\n");
        }
        else
        {
            printf("lexem je %s\n", str_lexeme(l));
            //printf("typ lexemu je %d\n", l.type);
        }
        //KONEC DEBUGOVACICH PRINTU 
    }
    
    tree_destroy(binaryTree); */


    
    error_eval();
    
    //exit(error_eval());
}