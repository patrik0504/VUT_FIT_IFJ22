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
//#define NDEBUG

#ifdef NDEBUG
#define Dprintf(FORMAT, ...) ((void)0)
#define Dputs(MSG) ((void)0)
#else
#define Dprintf(FORMAT, ...) \
    fprintf(stderr, "%s() in %s, line %i: " FORMAT "\n", \
        __func__, __FILE__, __LINE__, __VA_ARGS__)
#define Dputs(MSG) Dprintf("%s", MSG)
#endif

int get_string_length(char* buffer);

bool is_number(char c);

int StrOctToDec(char* oct);

int StrHexToDec(char* hex);

#endif