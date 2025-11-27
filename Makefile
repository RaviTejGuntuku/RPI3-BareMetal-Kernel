# Top-level Makefile

CROSS ?= aarch64-elf
QEMU  ?= qemu-system-aarch64

KERNEL_BIN = kernel/build/kernel8.bin

.PHONY: all run clean

all: $(KERNEL_BIN)

$(KERNEL_BIN):
	$(MAKE) -C kernel

run: all
	# Run the kernel under QEMU with semihosting enabled
	$(QEMU) -M virt -cpu cortex-a53 -m 128M -nographic \
	        -semihosting \
	        -kernel $(KERNEL_BIN)

clean:
	$(MAKE) -C kernel clean
