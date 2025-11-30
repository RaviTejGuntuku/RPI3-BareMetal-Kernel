#include <stdint.h>
#include "uart.h"
#include "debug.h"

// static void uart_puts(Uart &uart, const char *s)
// {
//     while (*s)
//         uart.put(*s++);
// }

extern "C" void kernelInit(void)
{
    Uart uart;
    // uart_puts(uart, "Hello world!\n");

    Debug::init(&uart);
    Debug::debugAll = false;

    Debug::printf("Hello world from debug.printf\n");

    while (1)
        asm volatile("wfe");
}