#include "uart.h"
#include "gpio.h"
#include <stdint.h>

// REFACTOR FILE WHEN PORTING TO REAL HARDWARE = RPI 3B

// PL011 UART at 0x3F201000 on RPi3
#define UART0_BASE (MMIO_BASE + 0x00201000)
#define UART0_DR ((volatile uint32_t *)(UART0_BASE + 0x00))
#define UART0_FR ((volatile uint32_t *)(UART0_BASE + 0x18))
#define UART0_IBRD ((volatile uint32_t *)(UART0_BASE + 0x24))
#define UART0_FBRD ((volatile uint32_t *)(UART0_BASE + 0x28))
#define UART0_LCRH ((volatile uint32_t *)(UART0_BASE + 0x2C))
#define UART0_CR ((volatile uint32_t *)(UART0_BASE + 0x30))
#define UART0_IMSC ((volatile uint32_t *)(UART0_BASE + 0x38))
#define UART0_ICR ((volatile uint32_t *)(UART0_BASE + 0x44))

// Flags in FR
#define FR_TXFF (1u << 5)
#define FR_RXFE (1u << 4)

// Control bits
#define CR_UARTEN (1u << 0)
#define CR_TXE (1u << 8)
#define CR_RXE (1u << 9)

// Line control
#define LCRH_WLEN_8 (0b11u << 5)
#define LCRH_FEN (1u << 4)

Uart::Uart()
{
    // GPIO 14/15 to ALT0 for PL011 (TXD0/RXD0)
    unsigned int r = *GPFSEL1;
    r &= ~((7 << 12) | (7 << 15)); // clear FSEL14, FSEL15
    r |= (4 << 12) | (4 << 15);    // set ALT0
    *GPFSEL1 = r;

    // Disable pulls during change
    *GPPUD = 0;
    for (unsigned int i = 0; i < 150; ++i)
        asm volatile("nop");
    *GPPUDCLK0 = (1 << 14) | (1 << 15);
    for (unsigned int i = 0; i < 150; ++i)
        asm volatile("nop");
    *GPPUDCLK0 = 0;

    // Disable UART and clear interrupts
    *UART0_CR = 0;
    *UART0_ICR = 0x7FF;

    // Baud ~115200 with 48 MHz UARTCLK: IBRD=26, FBRD=3
    *UART0_IBRD = 26;
    *UART0_FBRD = 3;

    // 8N1, enable FIFOs
    *UART0_LCRH = LCRH_WLEN_8 | LCRH_FEN;

    // Mask all interrupts (polled I/O)
    *UART0_IMSC = 0;

    // Enable UART, TX and RX
    *UART0_CR = CR_UARTEN | CR_TXE | CR_RXE;
}

void Uart::put(char c)
{
    // Convert '\n' -> "\r\n" for terminals
    if (c == '\n')
    {
        while (*UART0_FR & FR_TXFF)
        {
        }
        *UART0_DR = '\r';
    }
    while (*UART0_FR & FR_TXFF)
    {
    }
    *UART0_DR = static_cast<uint32_t>(c);
}

char Uart::get()
{
    // Wait for a character
    while (*UART0_FR & FR_RXFE)
    {
    }
    char r = static_cast<char>(*UART0_DR & 0xFF);
    return (r == '\r') ? '\n' : r;
}
