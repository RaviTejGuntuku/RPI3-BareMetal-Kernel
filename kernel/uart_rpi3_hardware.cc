// #include "uart.h"
// #include "gpio.h"

// #define AUX_ENABLE ((volatile unsigned int *)(MMIO_BASE + 0x00215004))
// #define AUX_MU_IO ((volatile unsigned int *)(MMIO_BASE + 0x00215040))
// #define AUX_MU_IER ((volatile unsigned int *)(MMIO_BASE + 0x00215044))
// #define AUX_MU_IIR ((volatile unsigned int *)(MMIO_BASE + 0x00215048))
// #define AUX_MU_LCR ((volatile unsigned int *)(MMIO_BASE + 0x0021504C))
// #define AUX_MU_MCR ((volatile unsigned int *)(MMIO_BASE + 0x00215050))
// #define AUX_MU_LSR ((volatile unsigned int *)(MMIO_BASE + 0x00215054))
// #define AUX_MU_CNTL ((volatile unsigned int *)(MMIO_BASE + 0x00215060))
// #define AUX_MU_BAUD ((volatile unsigned int *)(MMIO_BASE + 0x00215068))

// Uart::Uart()
// {
//     unsigned int r;

//     // Enable mini UART
//     *AUX_ENABLE |= 1;

//     *AUX_MU_CNTL = 0; // disable TX/RX during config
//     *AUX_MU_LCR = 3;  // 8-bit mode
//     *AUX_MU_MCR = 0;
//     *AUX_MU_IER = 0;
//     *AUX_MU_IIR = 0xC6; // disable interrupts
//     *AUX_MU_BAUD = 270; // 115200 baud on Pi3

//     // Setup GPIO 14 (TXD1) & 15 (RXD1) to ALT5
//     r = *GPFSEL1;
//     r &= ~((7 << 12) | (7 << 15));
//     r |= (2 << 12) | (2 << 15);
//     *GPFSEL1 = r;

//     // Enable pins
//     *GPPUD = 0;
//     for (r = 0; r < 150; r++)
//         asm volatile("nop");
//     *GPPUDCLK0 = (1 << 14) | (1 << 15);
//     for (r = 0; r < 150; r++)
//         asm volatile("nop");
//     *GPPUDCLK0 = 0;

//     // Enable TX/RX
//     *AUX_MU_CNTL = 3;
// }

// void Uart::put(char c)
// {
//     // Wait for UART to be ready
//     while (!(*AUX_MU_LSR & 0x20))
//         asm volatile("nop");

//     *AUX_MU_IO = c;

//     // Convert '\n' → '\r\n'
//     if (c == '\n')
//         put('\r');
// }

// char Uart::get()
// {
//     // Wait for a character
//     while (!(*AUX_MU_LSR & 0x01))
//         asm volatile("nop");

//     char r = (char)*AUX_MU_IO;
//     return (r == '\r') ? '\n' : r;
// }
