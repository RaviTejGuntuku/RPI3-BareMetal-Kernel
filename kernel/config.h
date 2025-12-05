// config.h
#pragma once

#include <stdint.h>

#define MAX_PROCS 16 // Pi 3B: 4 cores, but 16 = default max

struct Config
{
    uint32_t memSize;    // optional for now
    uint32_t totalProcs; // how many cores we expect
};

extern Config kConfig;
void configInit(Config *cfg);
