#ifndef FONT_H
#define FONT_H

#include <stdint.h>

#define FONT_WIDTH 8
#define FONT_HEIGHT 16

void font_load(const char* filename);
void font_get_char(uint16_t* lines, char ch);

#endif /* FONT_H */

