#include "font.h"
#include <stdio.h>

unsigned char font_src[] = {
#include "font_8x16.h"
};

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

void font_init()
{
    if (sizeof(font_src) < (FONT_WIDTH*FONT_HEIGHT*256/8))
    {
        fprintf(stderr, "font_init: wrong font size\n");
        return;
    }

    int i, y;
    for (i = 0; i < 256; i++)
    {
        for (y = 0; y < FONT_HEIGHT; y++)
        {
            font[i][y] = flip(font_src[i*FONT_HEIGHT + y]);
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

