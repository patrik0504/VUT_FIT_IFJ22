/**
 * ***********************IFJ PROJEKT 2022********************************
 * @file error.h
 * @author  Matěj Toul          (xtoulm00@stud.fit.vutbr.cz)
 *          Lukáš Etzler        (xetzle00@stud.fit.vutbr.cz)
 * @brief Funkce pro správu a výpis chyb
 * @date 2022-12-06
 * 
 * @copyright Copyright (c) 2022
*/

#ifndef ERR_HANDLE_FILE
#define ERR_HANDLE_FILE

#include <stdlib.h>
#include <stdio.h>

typedef enum {
    LEX_ERROR = 1,
    SYNTAX_ERROR = 2,
    SEM_UNDEFINED_FUNC_ERROR = 3,
    SEM_INVALID_CALL_ERROR = 4,
    SEM_UNDEFINED_VAR_ERROR = 5,
    SEM_INVALID_RETURN_ERROR = 6,
    SEM_INVALID_TYPE_ERROR = 7,
    SEM_OTHER_ERROR = 8,
    SEM_INTERNAL_ERROR = 99,
} error_code;

/** Struktura chybového výpisu */
typedef struct error{
    int line;   ///< Řádek, na kterém se chyba vyskytla
    char* message;   ///< Chybová hláška
    error_code error_code;  ///< Kód chyby
} *p_error;

/** Struktura seznamu chyb */
typedef struct error_list{
    p_error data;   ///< Soubor dat chyby
    struct error_list *next;  ///< Ukazatel na další error (další položku seznamu)
} *p_error_list;

/** 
 * Funkce inicializující globální seznam chyb.
 */
void error_list_init();

/** 
 *  Funkce pro přidání chybové hlášky do seznamu chyb
 *  @param line Číslo řádku chyby
 *  @param message Chybová hláška
 *  @param error_code Kód chyby dle enumu error_code
 */
void error(int line, char* message, error_code error_code);

/** 
 *  Funkce pro výpis všech chyb a vrácení chybového kódu první z nich, Na konci funkce uvolní paměť alokovanou pro error_list
 *  @return Vrací error_code první chyby v seznamu
 */
int error_eval();

/**
 * Pomocná funkce pro výpis chyb na stderr.
 * @param error Chyba k vypsání
*/
void error_out(p_error error);

#endif