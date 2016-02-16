#include "uuid.h"
#include <stdlib.h>
#include <stdio.h>

char* uuid_gen()
{
    unsigned char b[16];
    int i;

    for (i = 0; i < 16; i++)
    {
        b[i] = rand() & 0xff;
    }

    char* s = malloc(40);
    sprintf(s, "{%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
            b[0], b[1], b[2], b[3],
            b[4], b[5],
            b[6], b[7],
            b[8], b[9],
            b[10], b[11], b[12], b[13], b[14], b[15]);

    return s;
}

