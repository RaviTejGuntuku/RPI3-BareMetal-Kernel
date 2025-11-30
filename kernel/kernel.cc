#include <stdint.h>
#include "uart.h"
#include "debug.h"

// This is the symbol start.S calls, so it MUST be extern "C"
extern "C" void kernelInit(void)
{

    Uart uart;

    const char *s = "Hello from C++ kernel_main()\n";
    while (*s)
    {
        uart.put(*s++);
    }

    // uart.puts("Hello from C++ kernel_main()\n");

    // Let start.S handle what happens after return
    // (it will drop into the WFE loop)
    return;
}
