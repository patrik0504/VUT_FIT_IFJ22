#include "parser.h"

int parse(){
    int result = 0;
    p_node binaryTree = init_binary_treeKW();
    Lexeme l = {.type = NULLLEX};

    return result;
}

int program(Lexeme l, p_node binaryTree){
    int result = 0;
    l = get_token(binaryTree);
    switch (l.type) {
        case PROLOG:
            result = check_prolog(l, binaryTree);
        default:
            result = 0;
            break;
    }
    return result;
}

int check_prolog(Lexeme l, p_node binaryTree)
{
    int result = 0;
    l = get_token(binaryTree);
    if(l.type == FUNCTION_ID)
    {
        
    }
}
