// config.h
#pragma once

#include <stdint.h>

#define MAX_PROCS 4 // Pi3b = 4 cores

struct Config
{
    uint32_t memSize;    // optional for now
    uint32_t totalProcs; // how many cores we expect
};

extern Config kConfig;
void configInit(Config *cfg);
