#pragma once
#include <stdint.h>

extern "C" void kernelInit(void);
extern "C" uint64_t pickKernelStack(void);
