// config.cc
#include "config.h"

Config kConfig;

void configInit(Config *cfg)
{
    // For now, just hardcode. You can later probe memory if you want.
    cfg->memSize = 0;            // you can fill this in later
    cfg->totalProcs = MAX_PROCS; // Pi 3B has 4 cores
}
