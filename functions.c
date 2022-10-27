#include "functions.h"

int get_string_length(char* buffer)
{
    int i = 0;
    while(buffer[i] != '\0')
    {
        i++;
    }
    return i;
}

bool is_number(char c)
{
    if (c >= '0' && c <= '9')
    {
        return true;
    }
    return false;
}