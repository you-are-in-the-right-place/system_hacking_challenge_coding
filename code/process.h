#pragma once

#include "cache.h"

#pragma pack(push, 1)

struct __reg{
    char r[4];
    char pc;
};

#pragma pack(pop)

typedef struct context{
    struct __reg reg;
    char core;
    struct __L1cache* cache;
    char memory[0x100];
} ctx;

void run_process(ctx* ctx, l2* l2cache, char* program, char* argv[], int argc);
