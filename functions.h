/**
 * ***********************IFJ PROJEKT 2022********************************
 * @file functions.h
 * @author  Veronika Nevařilová (xnevar00@stud.fit.vutbr.cz)
 *          Patrik Michlian     (xmichl12@stud.fit.vutbr.cz)
 * @brief Pomocné funkce pro lexikální analýzu
 * @date 2022-12-06
 * 
 * @copyright Copyright (c) 2022
*/

#ifndef FUNCTIONS_FILE
#define FUNCTIONS_FILE

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>


//odkomentovat pro vypnutí debug módu
#define NDEBUG

#ifdef NDEBUG
#define Dprintf(FORMAT, ...) ((void)0)
#define Dputs(MSG) ((void)0)
#else
#define Dprintf(FORMAT, ...) \
    fprintf(stderr, "%s() in %s, line %i: " FORMAT "\n", \
        __func__, __FILE__, __LINE__, __VA_ARGS__)
#define Dputs(MSG) Dprintf("%s", MSG)
#endif

/**
 * @brief Získání délky řetězce
 * 
 * @param buffer Vstupní řetězec
 * @return int délka řetězce
 */
int get_string_length(char* buffer);

/**
 * @brief Ověření, zda je znak číslo
 * 
 * @param c Znak
 * @return true Znak je číslo
 * @return false Znak není číslo
 */
bool is_number(char c);

/**
 * @brief Převod řetězce obsahujícího číslo v oktálové soustavě na desítkovou
 * 
 * @param oct String obsahující číslo v oktálové soustavě
 * @return int ASCII hodnota čísla v desítkové soustavě
 */
int StrOctToDec(char* oct);

/**
 * @brief Převod řetězce obsahujícího číslo v hexadecimální soustavě na desítkovou
 * 
 * @param hex String obsahující číslo v hexadecimální soustavě
 * @return int ASCII hodnota čísla v desítkové soustavě
 */
int StrHexToDec(char* hex);


#endif