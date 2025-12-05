#include "smp.h"
#include "debug.h"
#include "psci.h"
#include <stdint.h>

Atomic<uint32_t> SMP::running{0};

const char *SMP::names[MAX_PROCS] = {
    "cpu0", "cpu1", "cpu2", "cpu3",
    "cpu4", "cpu5", "cpu6", "cpu7",
    "cpu8", "cpu9", "cpu10", "cpu11",
    "cpu12", "cpu13", "cpu14", "cpu15"};

static inline uint32_t read_mpidr()
{
    uint64_t v;
    asm volatile("mrs %0, mpidr_el1" : "=r"(v));
    return static_cast<uint32_t>(v);
}

uint32_t SMP::me()
{
    uint32_t mpidr = read_mpidr();
    return mpidr & 0x3u;
}

void SMP::init(bool isFirst)
{
    uint32_t id = me();

    if (isFirst)
    {
        Debug::printf("| SMP primary init on core %u (%s)\n", id, name());
        running = 1;
    }
    else
    {
        Debug::printf("| SMP secondary init on core %u (%s)\n", id, name());
        running.fetch_add(1);
    }
}

extern "C" void secondary_entry(); // from start.S

void SMP::startOthers()
{
    Debug::printf("| SMP::startOthers() using PSCI\n");

    uint32_t this_id = me();

    for (uint32_t cpu = 0; cpu < 4; ++cpu)
    {
        if (cpu == this_id)
            continue;

        uint64_t target_mpidr = cpu;
        uint64_t entry = reinterpret_cast<uint64_t>(&secondary_entry);

        int ret = psci_cpu_on(target_mpidr, entry, 0);
        if (ret != 0)
        {
            Debug::printf("|   psci_cpu_on(cpu=%u) failed: %d\n", cpu, ret);
        }
        else
        {
            Debug::printf("|   psci_cpu_on(cpu=%u) ok\n", cpu);
        }
    }
}
