#include "atomic.h"
#include "debug.h"

// Simple pause: on AArch64, WFE or YIELD is the closest equivalent
extern "C" void pause()
{
    if (Debug::shutdown_called)
    {
        Debug::panic("shutdown called\n");
    }

    asm volatile("wfe" ::: "memory");
}
