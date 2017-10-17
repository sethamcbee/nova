# When changing version here, update version in grub.cfg accordingly.
VERSION=nova

# Whether or not the build will be setup for debugging.
DEBUG=true

BIN=$(VERSION).bin
ISO=$(BIN:.bin=.iso)
SYM=$(BIN:.bin=.sym)

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
LIBRARY_PATH=\

# terminal emulator to execute new windows in
TERMINAL=xterm

# directory containing gcc
GCC_DIR=/home/sethamcbee/opt/gcc-7.2.0/bin

# compiler settings
OPTIMIZE=-O2
AS=$(GCC_DIR)/as
ASFLAGS=\
	$(INCLUDE_SCRIPT)
CC=$(GCC_DIR)/gcc
CCFLAGS=\
	$(INCLUDE_SCRIPT) \
	$(OPTIMIZE) \
	-std=gnu11 \
	-ffreestanding \
	-Wall \
	-Wextra
CPP=$(GCC_DIR)/g++
CPPFLAGS=\
	$(INCLUDE_SCRIPT) \
	$(OPTIMIZE) \
	-std=gnu++11 \
	-ffreestanding \
	-Wall \
	-Wextra \
	-fno-exceptions \
	-fno-rtti
LD=$(CC)
LDFLAGS=\
	$(OPTIMIZE) \
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

# adds debug dependencies if debugging
ifeq ($(DEBUG), true)
TEST_DEPENDENCIES+=\
	CCFLAGS+=DDEBUG \
	CPPFLAGS+=DDEBUG \
	ASFLAGS+=DDEBUG \
	$(DEBUG_DIR)/$(BIN) \
	$(DEBUG_DIR)/$(SYM) \
	$(DEBUG_DIR)/$(ISO_DIR)/$(ISO)
else
CCFLAGS+=-DNDEBUG
CPPFLAGS+=-DNDEBUG
ASFLAGS+=DNDEBUG
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
QEMU_CPU=1

#QEMU flags
QEMU_FLAGS=\
	-cdrom $(ISO_DIR)/$(ISO) \
	-boot d \
	-display sdl \
	-m $(QEMU_MEMORY) \
	-smp $(QEMU_CPU) \
	-vga std \
	-soundhw pcspk,sb16

# build objects, binary, and disk image
.PHONY: all
all: obj bin iso

# delete contents of build folder
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)/*

# target to build all objects
.PHONY: obj
obj: $(OBJ)

# build output binary
$(BIN_DIR)/$(BIN): $(OBJ)
	mkdir -p $(BIN_DIR)
	$(LD) -T $(LD_SCRIPT) -o $(BIN_DIR)/$(BIN) $(LDFLAGS) $(OBJ)

# target to build output binary
.PHONY: bin
bin: $(OBJ) $(BIN_DIR)/$(BIN)

# target to produce output iso
.PHONY: iso
iso: $(OBJ) $(BIN_DIR)/$(BIN) $(ISO_DIR)/$(ISO)

# build output iso
$(ISO_DIR)/$(ISO): $(BIN_DIR)/$(BIN)
	mkdir -p $(GRUB_DIR)/boot/grub
	mkdir -p $(ISO_DIR)
	cp $(SRC_DIR)/grub.cfg $(GRUB_DIR)/boot/grub/
	cp $(BIN_DIR)/$(BIN) $(GRUB_DIR)/boot/$(BIN)
	$(GRUB) $(GRUB_FLAGS) -o $(ISO_DIR)/$(ISO) $(GRUB_DIR)

.PHONY: qemu
qemu: $(ISO_DIR)/$(ISO)
	$(QEMU) $(QEMU_FLAGS)

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
