// psci.h
#pragma once

#include <stdint.h>
#include "debug.h"

static constexpr uint32_t PSCI_CPU_ON_FNID = 0xC4000003u;

extern "C" uint64_t psci_cpu_on_asm(uint64_t fnid,
                                    uint64_t target_cpu,
                                    uint64_t entry,
                                    uint64_t context);

int psci_cpu_on(uint64_t target_cpu,
                uint64_t entry,
                uint64_t context)
{
    Debug::printf("| psci_cpu_on(): fnid=0x%x target=0x%x entry=0x%x ctx=0x%x\n",
                  (uint32_t)PSCI_CPU_ON_FNID,
                  (uint32_t)target_cpu,
                  (uint32_t)entry,
                  (uint32_t)context);

    uint64_t el;
    asm volatile("mrs %0, CurrentEL" : "=r"(el));

    Debug::printf("Curr exception level: %d\n", (uint32_t)((el >> 2) & 0x3));

    uint64_t ret64 = psci_cpu_on_asm(PSCI_CPU_ON_FNID, target_cpu, entry, context);
    int ret = (int)(uint32_t)ret64;

    Debug::printf("| psci_cpu_on(): ret=0x%x (%d)\n",
                  (uint32_t)ret64,
                  ret);

    return ret;
}
