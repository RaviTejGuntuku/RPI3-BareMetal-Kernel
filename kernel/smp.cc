#include "smp.h"
#include "debug.h"
#include <stdint.h>

Atomic<uint32_t> SMP::running{0};

const char *SMP::names[MAX_PROCS] = {
    "cpu0", "cpu1", "cpu2", "cpu3"};

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

static inline volatile uint64_t *spin_table_entry_for_cpu(uint32_t cpu)
{
    if (cpu == 0)
        return nullptr;

    const uint64_t base = 0xE0;
    return reinterpret_cast<volatile uint64_t *>(base + 8ull * (cpu - 1u));
}

void SMP::startOthers()
{
    Debug::printf("| SMP::startOthers() using Pi3 spin-table\n");

    uint32_t this_id = me();
    uint64_t entry = reinterpret_cast<uint64_t>(&secondary_entry);

    for (uint32_t cpu = 0; cpu < 5; ++cpu)
    {
        if (cpu == this_id)
            continue;

        volatile uint64_t *slot = spin_table_entry_for_cpu(cpu);
        if (!slot)
            continue;

        Debug::printf("|   starting cpu%u at entry=0x%lx\n", cpu, entry);

        *slot = entry;

        asm volatile("dsb sy" ::: "memory");
        asm volatile("sev" ::: "memory");
    }
}
