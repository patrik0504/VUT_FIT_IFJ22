#ifndef FUNCTIONS_FILE
#define FUNCTIONS_FILE

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>


//odkomentovat pre vypnutie debugovania
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
 * @brief Získanie dĺžky reťazca
 * 
 * @param buffer Vsuvný reťazec
 * @return int Dĺžka reťazca
 */
int get_string_length(char* buffer);

/**
 * @brief Overenie či je znak číslo
 * 
 * @param c Znak
 * @return true Znak je číslo
 * @return false Znak nie je číslo
 */
bool is_number(char c);

/**
 * @brief Prevod reťazca obsahujúceho číslo v oktalovej sústave na desiatkovú
 * 
 * @param oct String obsahujúci číslo v oktalovej sústave
 * @return int ASCII hodnota čísla v desiatkovej sústave
 */
int StrOctToDec(char* oct);

/**
 * @brief Prevod reťazca obsahujúceho číslo v hexadecimálnej sústave na desiatkovú
 * 
 * @param hex String obsahujúci číslo v hexadecimálnej sústave
 * @return int ASCII hodnota čísla v desiatkovej sústave
 */
int StrHexToDec(char* hex);


#endif