#pragma once
#include <bx/bx.h>

#define DEBUG() \
    bx::debugPrintf("%s - %d", __FILE__, __LINE__)

#define SHDR_SOURCE(x) #x

#define MTX_BASE    \
{                   \
    1, 0, 0, 0,     \
    0, 1, 0, 0,     \
    0, 0, 1, 0,     \
    0, 0, 0, 1      \
}