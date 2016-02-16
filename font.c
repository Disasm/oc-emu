#include "font.h"
#include <stdio.h>

static uint16_t font[256][FONT_HEIGHT];

uint8_t flip(uint8_t v)
{
    uint8_t r = 0;
    int i;
    for (i = 0; i < 8; i++)
    {
        r = r << 1;
        r |= (v & 1);
        v = v >> 1;
    }
    return r;
}

void font_load(const char* filename)
{
    FILE* f = fopen(filename, "rb");
    if (f == 0)
    {
        fprintf(stderr, "Can't open font file\n");
        return;
    }

    uint8_t buffer[4096];
    if (fread(buffer, 1, 4096, f) != 4096)
    {
        fprintf(stderr, "Can't read font file\n");
        fclose(f);
        return;
    }
    fclose(f);

    int i, y;
    for (i = 0; i < 256; i++)
    {
        for (y = 0; y < FONT_HEIGHT; y++)
        {
            font[i][y] = flip(buffer[i*FONT_HEIGHT + y]);
        }
    }
}

void font_get_char(uint16_t* lines, char ch)
{
    int y;
    for (y = 0; y < FONT_HEIGHT; y++)
    {
        lines[y] = font[(uint8_t)ch][y];
    }
}

