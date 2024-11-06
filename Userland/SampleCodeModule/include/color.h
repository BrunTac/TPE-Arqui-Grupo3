#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>

typedef uint32_t Color;

enum{
    WHITE = 0xFFFFFF,
    BLACK = 0x000000,
    RED = 0xFF3B3F,
    BLUE = 0x003566,
    GREEN = 0x97C77B,
    DARK_GREEN = 0x42763B,
    PINE_GREEN = 0x1B3B2F,
    ICE_GREEN = 0xE0F8E0,
    GRAY = 0x2E3B42,
    YELLOW = 0xFFC300,
    MUSTARD = 0xA66A00,
    LIGHT_MUSTARD = 0xD89B39,
    BURGUNDY = 0x4A1E1B,
    SOFT_RED = 0x6B3A36,
    GRAY_BLUE = 0x1A2B4C,
    LIGHT_GRAY_BLUE = 0x2F456A
};

#endif