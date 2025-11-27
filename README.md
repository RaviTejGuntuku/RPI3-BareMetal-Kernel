# Raspberry Pi 3B (AArch64) Bare-Metal Kernel

This project builds a 64-bit bare-metal kernel for the Raspberry Pi 3B and the QEMU `virt` board. It uses a minimal AArch64 boot stub, a linker script, and a freestanding C/C++ runtime to bring up the system. The long-term goal is to evolve this into a portable kernel with modern features.

## Goals

- Pre-emptive multithreading (timer-driven context switches)
- Virtual memory (MMU setup, EL1 page tables, demand mapping)
- User processes + syscalls (EL0 execution, trap/exception handling)
- Basic filesystem (simple block device + VFS layer)
- Bluetooth support (driver + userland APIs)

## Repository Layout

- `kernel/start.S`: AArch64 boot code (EL1), stack setup, BSS clear, calls `kernelInit`
- `kernel/script-aarch64.ld`: Linker script targeting QEMU `virt` at 0x40000000
- `kernel/kernel.cc`: UART routines and C++ kernel entry
- `kernel/machine.S`/`machine.h`: Low-level machine helpers
- `kernel/Makefile`: Builds `kernel8.elf` and `kernel8.bin`
- `Makefile`: Top-level driver, builds and runs under QEMU

## Build

From the repository root:

```
make CROSS=aarch64-none-elf
```

If your toolchain uses `aarch64-elf`:

```
make CROSS=aarch64-elf
```

## Run (QEMU)

```
make run CROSS=aarch64-none-elf
```

This starts QEMU with the `virt` machine and prints a greeting over the emulated PL011 UART to your terminal.

## Porting to Raspberry Pi 3B

- Replace QEMU `virt` MMIO base addresses with RPi3 PL011 base (at 0x3F201000 on RPi3, 0xFE201000 on RPi4).
- Adjust linker load address (RPi3 bootloader loads at 0x00080000).
- Provide minimal boot in EL2/EL1 depending on firmware state; configure MMU and caches for RPi.
- Set up the GIC and timer for preemption.

## Roadmap

1. Exceptions/interrupts: vector table, timer interrupts, context switch
2. Memory: page tables, allocator, kernel/user address spaces
3. Syscalls: trap from EL0, process lifecycle
4. Filesystem: block I/O, simple FS, VFS
5. Bluetooth: driver and user-mode API
6. RPi3 hardware bring-up: UART, mailbox, timers, GIC

## License

Educational use. Adapt freely for coursework and experimentation.
