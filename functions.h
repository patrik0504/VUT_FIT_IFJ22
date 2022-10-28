#ifndef FUNCTIONS_FILE
#define FUNCTIONS_FILE

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>

int get_string_length(char* buffer);

bool is_number(char c);

int StrOctToDec(char* oct);

int StrHexToDec(char* hex);

#endif