#pragma once

#include <stdint.h>
#include "config.h"
#include "atomic.h"

class SMP
{
public:
    static Atomic<uint32_t> running;
    static const char *names[MAX_PROCS];

    static void init(bool isFirst);
    static uint32_t me();
    static const char *name() { return names[me()]; }

    static void startOthers();
};

template <class T>
class PerCPU
{
private:
    T data[MAX_PROCS];

public:
    inline T &forCPU(int id) { return data[id]; }
    inline T &mine() { return forCPU(SMP::me()); }
};
