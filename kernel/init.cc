#include "init.h"

#include "debug.h"
#include "config.h"
#include "uart.h"
#include "smp.h"
#include "atomic.h"

struct Stack
{
    static constexpr int BYTES = 4096;
    uint8_t bytes[BYTES] __attribute__((aligned(16)));
};

PerCPU<Stack> stacks;

static bool smpInitDone = false;

// For now just return top-of-stack for each core
extern "C" uint64_t pickKernelStack(void)
{
    uint32_t id = SMP::me();
    Stack &st = stacks.forCPU(id);
    return reinterpret_cast<uint64_t>(&st.bytes[Stack::BYTES]);
}

static Atomic<uint32_t> howManyAreHere{0};

extern "C" void kernelInit(void)
{
    Uart uart;

    uint32_t id = SMP::me();

    if (!smpInitDone)
    {
        smpInitDone = true;

        Debug::init(&uart);
        Debug::debugAll = false;

        Debug::printf("\n| [core %u] primary boot\n", id);

        configInit(&kConfig);
        Debug::printf("| totalProcs %u\n", kConfig.totalProcs);

        SMP::init(true);

        SMP::startOthers();
    }
    else
    {
        Debug::init(&uart);
        SMP::init(false);
    }

    auto order = howManyAreHere.add_fetch(1);

    Debug::printf("| [core %u] kernelInit reached (order=%u)\n", id, order);

    while (1)
    {
        asm volatile("wfe");
    }
}
