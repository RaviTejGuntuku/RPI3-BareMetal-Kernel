#include <stdint.h>

constexpr uintptr_t UART0_BASE = 0x09000000UL;
constexpr uintptr_t UARTDR = UART0_BASE + 0x00;
constexpr uintptr_t UARTFR = UART0_BASE + 0x18;
constexpr uint32_t UARTFR_TXFF = 1u << 5;

static inline void mmio_write(uintptr_t addr, uint32_t value)
{
    *reinterpret_cast<volatile uint32_t *>(addr) = value;
}

static inline uint32_t mmio_read(uintptr_t addr)
{
    return *reinterpret_cast<volatile uint32_t *>(addr);
}

class Uart
{
public:
    void put(char c)
    {
        while (mmio_read(UARTFR) & UARTFR_TXFF)
        {
            // spin
        }
        mmio_write(UARTDR, static_cast<uint32_t>(c));
    }

    void puts(const char *s)
    {
        while (*s)
        {
            if (*s == '\n')
            {
                put('\r');
            }
            put(*s++);
        }
    }
};

static Uart uart;

// This is the symbol start.S calls, so it MUST be extern "C"
extern "C" void kernelInit(void)
{
    uart.puts("Hello from C++ kernel_main()\n");

    // Let start.S handle what happens after return
    // (it will drop into the WFE loop)
    return;
}
