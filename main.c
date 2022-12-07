/**
 * ***********************IFJ PROJEKT 2022********************************
 * @file main.c
 * @author  Veronika Nevařilová (xnevar00@stud.fit.vutbr.cz)
 *          Patrik Michlian     (xmichl12@stud.fit.vutbr.cz)
 *          Matěj Toul          (xtoulm00@stud.fit.vutbr.cz)
 *          Lukáš Etzler        (xetzle00@stud.fit.vutbr.cz)
 * @brief Hlavní tělo projektu
 * @date 2022-12-06
 * 
 * @copyright Copyright (c) 2022
*/

#include <stdio.h>
#include <stdlib.h>

#include "scanner.h"
#include "symtable.h"
#include "stack.h"
#include "parser.h"
#include "error.h"

// TODO: oddělat (přijde do parseru)
#include "expr_parser.h"

int main() {
    error_list_init();
    
    int some = 0;
    some = parse();
    exit(error_eval());
}