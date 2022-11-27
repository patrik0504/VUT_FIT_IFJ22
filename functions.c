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

    if (oct > 255 || oct <= 0)
    {
        return -1;
    }
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

int StrHexToDec(char* hex){
    int dec = 0;
    int len = 2;
    int base = 1;
    for (int i = len - 1; i >= 0; i--)
    {
        if (hex[i] >= '0' && hex[i] <= '9')
        {
            dec += (hex[i] - 48) * base;
            base = base * 16;
        }
        else if (hex[i] >= 'A' && hex[i] <= 'F')
        {
            dec += (hex[i] - 55) * base;
            base = base * 16;
        }
        else if (hex[i] >= 'a' && hex[i] <= 'f')
        {
            dec += (hex[i] - 87) * base;
            base = base * 16;
        }
        else
        {
            return -1;
        }
    }
    if(dec == 0)
    {
        return -1;
    }
    return dec;
}