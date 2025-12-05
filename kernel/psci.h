#pragma once
#include <stdint.h>

constexpr uint64_t PSCI_CPU_ON = 0xC4000003;

static inline int psci_cpu_on(uint64_t target_mpidr,
                              uint64_t entry,
                              uint64_t context_id)
{
    register uint64_t x0 asm("x0") = PSCI_CPU_ON;
    register uint64_t x1 asm("x1") = target_mpidr;
    register uint64_t x2 asm("x2") = entry;
    register uint64_t x3 asm("x3") = context_id;

    asm volatile("smc #0"
                 : "+r"(x0)
                 : "r"(x1), "r"(x2), "r"(x3)
                 : "x4", "x5", "x6", "x7", "memory");

    return (int)x0;
}
