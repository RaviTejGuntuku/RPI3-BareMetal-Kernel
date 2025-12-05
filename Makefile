# Top-level Makefile

CROSS ?= aarch64-elf
QEMU  ?= qemu-system-aarch64

KERNEL_BIN = kernel/build/kernel8.bin

.PHONY: all run clean

all: $(KERNEL_BIN)

$(KERNEL_BIN):
	$(MAKE) -C kernel CROSS=$(CROSS)

run: all
	# Emulate Raspberry Pi 3B board and memory map
	$(QEMU) -M raspi3b \
	        -cpu cortex-a53 \
	        -m 1G \
			-smp 4 \
	        -nographic \
	        -kernel $(KERNEL_BIN)

clean:
	$(MAKE) -C kernel clean
