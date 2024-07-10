#pragma once
#include <bx/bx.h>

#define DEBUG() \
    bx::debugPrintf("%s - %d", __FILE__, __LINE__)

#define SHDR_SOURCE(x) #x

#define FROM_PX_TO_NORM(value, resolution) ((2.0f * value / resolution) - 1.0f)

#define MTX_BASE    \
{                   \
    1, 0, 0, 0,     \
    0, 1, 0, 0,     \
    0, 0, 1, 0,     \
    0, 0, 0, 1      \
}