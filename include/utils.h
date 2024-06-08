#pragma once
#include <bx/bx.h>

#define DEBUG() \
    bx::debugPrintf("%s - %d", __FILE__, __LINE__)

#define SHDR_SOURCE(x) #x