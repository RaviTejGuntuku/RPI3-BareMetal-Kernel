// loop.h (AArch64 / RPi3)

#pragma once

inline void iAmStuckInALoop(bool use_wfe)
{
    if (use_wfe)
    {
        asm volatile("wfe" ::: "memory");
    }
    else
    {
        asm volatile("yield" ::: "memory");
    }
}
