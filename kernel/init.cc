#include "init.h"

#include "debug.h"
#include "config.h"
#include "uart.h"
#include "smp.h"
#include "atomic.h"
#include "heap.h"

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

static constexpr uint64_t HEAP_SIZE = 5 * 1024 * 1024;

static inline void *align_up(void *p, uint64_t align)
{
    auto v = (uint64_t)p;
    v = (v + align - 1) & ~(align - 1);
    return (void *)v;
}

static Atomic<uint32_t> howManyAreHere{0};

extern "C" char _end;

// class Dummy
// {
//     uint32_t a;
//     uint32_t b;

// public:
//     Dummy() : a(5), b(6)
//     {
//     }

//     void get()
//     {
//         Debug::printf("GET EXECUTED, a: %d, b: %d\n", a, b);
//     }
// };

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

        void *heap_start = align_up(&_end, 16);

        // uint64_t sp;
        // asm volatile("mov %0, sp" : "=r"(sp));
        // Debug::printf("| SP = %p\n", (void *)sp);
        // Debug::printf("| _end = %p\n", &_end);
        // Debug::printf("| heap_start = %p\n", heap_start);
        // Debug::printf("| stacks base = %p\n", &stacks);

        heapInit(heap_start, HEAP_SIZE);

        // Dummy *d = new Dummy();
        // Debug::printf("| new Dummy -> %p\n", d);

        Debug::init(new Uart);
        Debug::printf("| switched to new UART\n");

        // launch other cores
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
