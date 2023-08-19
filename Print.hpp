#include "external/stb_sprintf.h"

#include <iostream>

static char PRINT_BUF[127];

#define print(fmt, ...) \
    stbsp_sprintf(PRINT_BUF, fmt, __VA_ARGS__); \
    std::cout << PRINT_BUF;

#define println(fmt, ...) \
    stbsp_sprintf(PRINT_BUF, fmt, __VA_ARGS__); \
    std::cout << PRINT_BUF << '\n';
