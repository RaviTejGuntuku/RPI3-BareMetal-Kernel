# kernel/Makefile (AArch64, C + C++ for Raspberry Pi 3, STRICT)

B       := build
TARGET  := kernel8

CROSS   ?= aarch64-elf
CC      := $(CROSS)-gcc
CXX     := $(CROSS)-g++
LD      := $(CROSS)-ld
OBJCOPY := $(CROSS)-objcopy

UTCS_OPT ?= -O2   # or -O3 if you want

# Common warning flags, very strict
WARN    := -Wall -Werror

CFLAGS   := -std=gnu11 -ffreestanding -nostdlib -nostartfiles \
            $(WARN) $(UTCS_OPT) -g \
            -mcpu=cortex-a53 -march=armv8-a

CXXFLAGS := -std=c++20 -ffreestanding -nostdlib -nostartfiles \
            -fno-exceptions -fno-rtti -fno-threadsafe-statics \
            $(WARN) $(UTCS_OPT) -g \
            -mcpu=cortex-a53 -march=armv8-a

# Sources
SFILES   := start.S machine.S
CFILES   := $(wildcard *.c)
CXXFILES := $(wildcard *.cc)

SOBJS    := $(addprefix $(B)/,$(SFILES:.S=.o))
COBJS    := $(addprefix $(B)/,$(CFILES:.c=.o))
CXXOBJS  := $(addprefix $(B)/,$(CXXFILES:.cc=.o))
OBJS     := $(SOBJS) $(COBJS) $(CXXOBJS)

.PHONY: all clean

all: $(B)/$(TARGET).bin

$(B):
	@mkdir -p $(B)

# Assembly (use GCC driver so warnings apply too)
$(B)/%.o: %.S | $(B)
	@echo "assembling $<"
	$(CC) $(CFLAGS) -x assembler-with-cpp -c -o $@ $<

# C
$(B)/%.o: %.c | $(B)
	@echo "compiling  $<"
	$(CC) $(CFLAGS) -I. -MMD -MP -MF $(B)/$*.d -c -o $@ $<

# C++
$(B)/%.o: %.cc | $(B)
	@echo "compiling  $<"
	$(CXX) $(CXXFLAGS) -I. -MMD -MP -MF $(B)/$*.d -c -o $@ $<

# Link (RPi3: link.ld should set . = 0x80000 and ENTRY(_start))
$(B)/$(TARGET).elf: link.ld $(OBJS)
	@echo "linking    $@"
	$(LD) -T link.ld -o $@ $(OBJS)

# Binary image (what QEMU -kernel and real RPi3 firmware expect)
$(B)/$(TARGET).bin: $(B)/$(TARGET).elf
	@echo "objcopy    $@"
	$(OBJCOPY) -O binary $< $@

clean:
	rm -rf $(B)

-include $(B)/*.d
