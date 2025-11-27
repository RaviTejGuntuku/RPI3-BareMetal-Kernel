# kernel/Makefile (AArch64, C + C++)

B       := build
TARGET  := kernel8

CROSS   ?= aarch64-elf
CC      := $(CROSS)-gcc
CXX     := $(CROSS)-g++
LD      := $(CROSS)-ld
OBJCOPY := $(CROSS)-objcopy

UTCS_OPT ?= -O2   # or -O3 if you want

CFLAGS   := -std=gnu11 -ffreestanding -nostdlib -nostartfiles \
            -Wall -Wextra $(UTCS_OPT) -g \
            -mcpu=cortex-a53 -march=armv8-a

CXXFLAGS := -std=c++20 -ffreestanding -nostdlib -nostartfiles \
            -fno-exceptions -fno-rtti -fno-threadsafe-statics \
            -Wall -Wextra $(UTCS_OPT) -g \
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

# Assembly
$(B)/%.o: %.S | $(B)
	@echo "assembling $<"
	$(CC) $(CFLAGS) -c -o $@ $<

# C
$(B)/%.o: %.c | $(B)
	@echo "compiling  $<"
	$(CC) $(CFLAGS) -I. -MMD -MP -MF $(B)/$*.d -c -o $@ $<

# C++
$(B)/%.o: %.cc | $(B)
	@echo "compiling  $<"
	$(CXX) $(CXXFLAGS) -I. -MMD -MP -MF $(B)/$*.d -c -o $@ $<

# Link
$(B)/$(TARGET).elf: script-aarch64.ld $(OBJS)
	@echo "linking    $@"
	$(LD) -T script-aarch64.ld -o $@ $(OBJS)

# Binary image
$(B)/$(TARGET).bin: $(B)/$(TARGET).elf
	@echo "objcopy    $@"
	$(OBJCOPY) -O binary $< $@

clean:
	rm -rf $(B)

-include $(B)/*.d
