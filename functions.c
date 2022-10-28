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

int StrOctToDec(char* buffer)
{
    if (!((buffer[0] >= '0' && buffer[0] <= '3') && (buffer[1] >= '0' && buffer[1] <= '7') && (buffer[2] >= '0' && buffer[2] <= '7')))
    {
        return -1;
    }
    int oct = atoi(buffer);

    int rem, dec=0, mul=1;
    while(oct != 0)
    {
        rem = oct%10;
        dec = dec + (rem*mul);
        mul = mul*8;
        oct = oct/10;
    }
    return dec;
}