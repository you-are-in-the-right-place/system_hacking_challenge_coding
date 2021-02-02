#pragma once

#pragma pack(push, 1)

#define CACHE_MISS 0
#define CACHE_HIT_L1 1
#define CACHE_HIT_L2 2

struct __L1line{
    char tag;       // 0xFF if the line is not used
    char data;
};

struct __L2line{
    char core;      // which core's memory? set the bit (01/10/11<-shared library)
    char tag;       // 0xFF if the line is not used
    char data;
};

typedef struct __L1cache{
    struct __L1line line[0x40];
} l1;

typedef struct __L2cache{
    struct __L2line line[0x80];
} l2;

#pragma pack(pop)

struct __L2cache* getL2cache();
struct __L1cache* getL1cache();
int read_cache(l1* l1cache, l2* l2cache, char core, unsigned int addr, char* out);
void load_cache(l1* l1cache, l2* l2cache, char core, unsigned int addr, char data);
void flushL1(l1* cache);
void flushL2(l2* cache);
