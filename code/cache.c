#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cache.h"

void load_l1cache(l1* l1cache, unsigned int addr, char data);

void flushL1(l1* cache){
    int i;
    for(i=0; i<0x40; i++){
        cache->line[i].tag = '\xFF';
    }
}

void flushL2(l2* cache){
    int i;
    for(i=0; i<0x80; i++){
        cache->line[i].core = '\x00';
        cache->line[i].tag = '\xFF';
    }
}

struct __L2cache* getL2cache(){
    struct __L2cache* cache = (struct __L2cache*)malloc(sizeof(struct __L2cache));
    flushL2(cache);
    return cache;
}

struct __L1cache* getL1cache(){
    struct __L1cache* cache = (struct __L1cache*)malloc(sizeof(struct __L1cache));
    flushL1(cache);
    return cache;
}

int shared(unsigned int addr){
    return addr >= 0x20 && addr < 0x80;
}

int read_cache(l1* l1cache, l2* l2cache, char core, unsigned int addr, char* out){

    char tag, idx;
    // read l1cache
    tag = (addr & 0xC0) >> 6;
    idx = addr & 0x3F;
    if (l1cache->line[idx].tag == tag){ // cache hit
        *out = l1cache->line[idx].data;
        return CACHE_HIT_L1;
    }
    
    // read l2cache
    tag = (addr & 0x80) >> 7;
    idx = addr & 0x7F;
    if ((l2cache->line[idx].core & core) && (l2cache->line[idx].tag == tag)){   // cache hit
        *out = l2cache->line[idx].data;
        load_l1cache(l1cache, addr, *out);
        return CACHE_HIT_L2;
    }

    return CACHE_MISS;
}

void load_l1cache(l1* l1cache, unsigned int addr, char data){

    char tag, idx;

    tag = (addr & 0xC0) >> 6;
    idx = addr & 0x3F;
    l1cache->line[idx].tag = tag;
    l1cache->line[idx].data = data;
}

void load_cache(l1* l1cache, l2* l2cache, char core, unsigned int addr, char data){
    char tag, idx;

    // load l2cache
    tag = (addr & 0x80) >> 7;
    idx = addr & 0x7F;
    l2cache->line[idx].tag = tag;
    if(shared(addr))
        l2cache->line[idx].core = 0x01 | 0x02;
    else
        l2cache->line[idx].core = core;
    l2cache->line[idx].data = data;
    
    // load l1cache
    load_l1cache(l1cache, addr, data);
}
