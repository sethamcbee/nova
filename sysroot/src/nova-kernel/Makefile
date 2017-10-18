# When changing version here, update version in grub.cfg accordingly.
VERSION=nova

# Whether or not the build will be setup for debugging.
DEBUG=true

# Output file names
BIN=$(BIN_DIR)/$(VERSION).bin
ISO=$(ISO_DIR)/$(VERSION).iso
SYM=$(DEBUG_DIR)/debug.sym

# build root directory
BUILD_DIR=./build
SRC_DIR:=.

# Architecture. Options are: x86_64
ARCH=x86_64

# directories to export objects, binaries, ISOs, etc.
OBJ_DIR=$(BUILD_DIR)/obj
ASM_DIR=$(BUILD_DIR)/asm
BIN_DIR=$(BUILD_DIR)/bin
GRUB_DIR=$(BUILD_DIR)/grub
ISO_DIR=$(BUILD_DIR)/iso
RELEASE_DIR=$(BUILD_DIR)/release
DEBUG_DIR=$(BUILD_DIR)/debug
QEMU_DIR=$(BUILD_DIR)/qemu

# find all source files except architecture-specific files
SRC=$(shell find $(SRC_DIR) -type f -name "*.c" -not -path "./arch/*")
SRC+=$(shell find $(SRC_DIR) -type f -name "*.s" -not -path "./arch/*")

# add appropriate architecture-specific files
ifeq ($(ARCH), x86_64)
	LD_SCRIPT=x86_64.ld
	SRC+=$(shell find $(SRC_DIR)/arch/x86_64 -type f -name "*.c")
	SRC+=$(shell find $(SRC_DIR)/arch/x86_64 -type f -name "*.s")
endif

# objects list made from generic sources list
OBJ=$(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(SRC)))

# assembly list made from generic sources list
ASM=$(addprefix $(ASM_DIR)/, $(addsuffix .s, $(SRC)))

# additional include paths
INCLUDE_PATH=\
$(SRC_DIR)

# script for adding specified include paths
INCLUDE_SCRIPT=$(addprefix -I, $(INCLUDE_PATH))

# additional library paths
LIBRARY_PATH=

# terminal emulator to execute new windows in
TERMINAL=xfce4-terminal

# directory containing gcc
GCC_DIR=/home/sethamcbee/opt/gcc-7.2.0/bin

# config defines
DEFINES=\

# special defines
ifeq ($(ARCH), x86_64)
	DEFINES+=_ARCH_X86_64_
endif

# add prefix to defines
DEFINES:=$(addprefix -D, $(DEFINES))

# compiler settings
OPTIMIZE=-O2
AS=$(GCC_DIR)/as
ASFLAGS=\
	$(INCLUDE_SCRIPT)
CC=$(GCC_DIR)/gcc
CCFLAGS=\
	$(INCLUDE_SCRIPT) \
	$(OPTIMIZE) \
	$(DEFINES) \
	-std=gnu11 \
	-ffreestanding \
	-Wall \
	-Wextra \
	-mno-red-zone
CPP=$(GCC_DIR)/g++
CPPFLAGS=\
	$(INCLUDE_SCRIPT) \
	$(OPTIMIZE) \
	$(DEFINES) \
	-std=gnu++11 \
	-ffreestanding \
	-Wall \
	-Wextra \
	-fno-exceptions \
	-fno-rtti \
	-mno-red-zone
LD=$(CC)
LDFLAGS=\
	$(OPTIMIZE) \
	$(DEFINES) \
	-z max-page-size=4096 \
	-ffreestanding \
	-nostdlib \
	-lgcc

# adds additional compiler flags if debugging
ifeq ($(DEBUG), true)
ASFLAGS+=-gdwarf-2
CCFLAGS+=-gdwarf-2
CPPFLAGS+=-gdwarf-2
endif

# Grub2 settings
GRUB=grub-mkrescue
GRUB_FLAGS=--directory=/usr/lib/grub/i386-pc

# QEMU application settings
QEMU=qemu-system-x86_64

# QEMU disk settings
QEMU_DISK_FORMAT=qcow2
QEMU_DISK=disk.img
QEMU_DISK_SIZE=30M

#QEMU flags settings
QEMU_MEMORY=800M
QEMU_CPU=qemu64
QEMU_SMP=1

#QEMU flags
QEMU_FLAGS=\
	-cdrom $(ISO) \
	-s \
	-S \
	-boot d \
	-display sdl \
	-monitor stdio \
	-m $(QEMU_MEMORY) \
	-cpu $(QEMU_CPU) \
	-smp $(QEMU_SMP) \
	-vga std \
	-soundhw pcspk,sb16

# build objects, binary, and disk image
.PHONY: default
default: iso

# rebuild objects, binary, and disk image
.PHONY: remake
remake: clean iso

# delete contents of build folder
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

# target to build all objects
.PHONY: obj
obj: $(OBJ)

# build output binary
$(BIN): $(OBJ)
	mkdir -p $(BIN_DIR)
	mkdir -p $(DEBUG_DIR)
	$(LD) -T $(LD_SCRIPT) -o $(BIN) $(LDFLAGS) $(OBJ)
	objcopy --only-keep-debug $(BIN) $(SYM)
	objcopy --strip-debug $(BIN)

# target to build output binary
.PHONY: bin
bin: $(BIN)

# target to produce output iso
.PHONY: iso
iso: $(ISO)

# build output iso
$(ISO): $(BIN)
	mkdir -p $(GRUB_DIR)/boot/grub
	mkdir -p $(ISO_DIR)
	cp $(SRC_DIR)/grub.cfg $(GRUB_DIR)/boot/grub/
	cp $(BIN) $(GRUB_DIR)/boot/$(VERSION).bin
	$(GRUB) $(GRUB_FLAGS) -o $(ISO) $(GRUB_DIR)

.PHONY: qemu
qemu: $(ISO)
	$(TERMINAL) -e "$(QEMU) $(QEMU_FLAGS)" &
	gdb --tui --quiet -ex "set confirm off" -ex "target remote localhost:1234" -ex "symbol-file $(SYM)"

# target to export assembled source files
.PHONY: asm
asm: $(ASM)

# assemble assembly files, generic
$(OBJ_DIR)/%.s.o: %.s
	mkdir -p $(@D)
	$(AS) $< -o $@ $(ASFLAGS)

# compile C files, generic
$(OBJ_DIR)/%.c.o: %.c
	mkdir -p $(@D)
	$(CC) -c $< -o $@ $(CCFLAGS)

# compile C++ files, generic
$(OBJ_DIR)/%.cpp.o: %.cpp
	mkdir -p $(@D)
	$(CPP) -c $< -o $@ $(CPPFLAGS)

# copy assembly files, generic
$(ASM_DIR)/%.s.s: %.s
	mkdir -p $(@D)
	cp $< $@

# assemble C files, generic
$(ASM_DIR)/%.c.s: %.c
	mkdir -p $(@D)
	$(CC) -S $< -o $@ $(CCFLAGS)

# assemble C++ files, generic
$(ASM_DIR)/%.cpp.s: %.cpp
	mkdir -p $(@D)
	$(CPP) -S $< -o $@ $(CPPFLAGS)
