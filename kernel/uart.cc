#include "uart.h"
#include <stdint.h>

constexpr uintptr_t UART0_BASE = 0x09000000UL;
constexpr uintptr_t UARTDR = UART0_BASE + 0x00;
constexpr uintptr_t UARTFR = UART0_BASE + 0x18;
constexpr uint32_t UARTFR_TXFF = 1u << 5;
constexpr uint32_t UARTFR_RXFE = 1u << 4;

static inline void mmio_write(uintptr_t a, uint32_t v) { *reinterpret_cast<volatile uint32_t *>(a) = v; }
static inline uint32_t mmio_read(uintptr_t a) { return *reinterpret_cast<volatile uint32_t *>(a); }

Uart::Uart() { /* QEMU virt PL011 OK without explicit init for TX */ }

void Uart::put(char ch)
{
    // Convert LF to CRLF for terminal visibility
    if (ch == '\n')
    {
        while (mmio_read(UARTFR) & UARTFR_TXFF)
        {
        }
        mmio_write(UARTDR, '\r');
    }
    while (mmio_read(UARTFR) & UARTFR_TXFF)
    {
    }
    mmio_write(UARTDR, static_cast<uint32_t>(ch));
}

char Uart::get()
{
    while (mmio_read(UARTFR) & UARTFR_RXFE)
    {
    }
    return static_cast<char>(mmio_read(UARTDR) & 0xFF);
}