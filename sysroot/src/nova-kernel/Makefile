# When changing version here, update version in grub.cfg accordingly.
VERSION=nova

# Whether or not the build will be setup for debugging.
DEBUG=true

# Output file names
BIN=$(BIN_DIR)/$(VERSION).elf
ISO=$(ISO_DIR)/$(VERSION).iso

# build root directory
BUILD_DIR=./build
SRC_DIR:=.

# Architecture. Options are: x86_64, x86
ARCH=x86_64
OPTIMIZE=-O0 -mno-sse

# Whether to enable the __stack_chk_guard.
STACK_GUARD=true

# directories to export objects, binaries, ISOs, etc.
OBJ_DIR=$(BUILD_DIR)/obj
ASM_DIR=$(BUILD_DIR)/asm
BIN_DIR=$(BUILD_DIR)/bin
GRUB_DIR=$(BUILD_DIR)/grub
ISO_DIR=$(BUILD_DIR)/iso
RELEASE_DIR=$(BUILD_DIR)/release
DEBUG_DIR=$(BUILD_DIR)/debug
DOC_DIR=$(BUILD_DIR)/doc
QEMU_DIR=$(BUILD_DIR)/qemu

# find all source files except architecture-specific files
SRC=$(shell find $(SRC_DIR) -type f -name "*.c" -not -path "./arch/*" -not -path "./build/*")
SRC+=$(shell find $(SRC_DIR) -type f -name "*.cc" -not -path "./arch/*" -not -path "./build/*")
SRC+=$(shell find $(SRC_DIR) -type f -name "*.s" -not -path "./arch/*" -not -path "./build/*")

# add appropriate architecture-specific files
ifeq ($(ARCH), x86_64)
	LD_SCRIPT=arch/x86_64/x86_64.ld
	SRC+=$(shell find $(SRC_DIR)/arch/x86_64 -type f -name "*.c")
	SRC+=$(shell find $(SRC_DIR)/arch/x86_64 -type f -name "*.cc")
	SRC+=$(shell find $(SRC_DIR)/arch/x86_64 -type f -name "*.s")
	DOXYGEN_CFG=x86_64.doxygen
endif
ifeq ($(ARCH), x86)
	LD_SCRIPT=arch/x86/x86.ld
	SRC+=$(shell find $(SRC_DIR)/arch/x86 -type f -name "*.c")
	SRC+=$(shell find $(SRC_DIR)/arch/x86 -type f -name "*.cc")
	SRC+=$(shell find $(SRC_DIR)/arch/x86 -type f -name "*.s")
	DOXYGEN_CFG=x86.doxygen
endif

# objects list made from generic sources list
OBJ=$(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(SRC)))

# assembly list made from generic sources list
ASM=$(addprefix $(ASM_DIR)/, $(addsuffix .s, $(SRC)))

# Headers to be included into all other (C) files.
#HEADERS=stack_guard.h

# Script for adding headers.
HEADERS_SCRIPT=$(addprefix -include , $(HEADERS))

# additional include paths
INCLUDE_PATH=\
$(SRC_DIR) \
$(SRC_DIR)/libc \
$(SRC_DIR)/libcpp

# script for adding specified include paths
INCLUDE_SCRIPT=$(addprefix -I, $(INCLUDE_PATH))

# additional library paths
LIBRARY_PATH=

# terminal emulator to execute new windows in
TERMINAL=xfce4-terminal

# directory containing gcc
ifeq ($(ARCH), x86_64)
	GCC_DIR=~/opt/gcc-7.3.0-x86_64-elf/bin
endif
ifeq ($(ARCH), x86)
	GCC_DIR=~/opt/gcc-7.3.0-i686-elf/bin
endif

# config defines
DEFINES=\

# special defines
ifeq ($(ARCH), x86_64)
	DEFINES+=ARCH_X86_64
endif
ifeq ($(ARCH), x86)
	DEFINES+=ARCH_X86
endif
ifeq ($(STACK_GUARD), true)
	DEFINES+=STACK_GUARD
endif
ifeq ($(DEBUG), true)
	DEFINES+=DEBUG
else
	DEFINES+=NDEBUG
endif

# add prefix to defines
DEFINES:=$(addprefix -D, $(DEFINES))

# compiler settings
ifeq ($(ARCH), x86_64)
	GCC_PREFIX=x86_64-elf-
	AS=$(GCC_DIR)/$(GCC_PREFIX)as
	ASFLAGS=\
		$(INCLUDE_SCRIPT)
	CC=$(GCC_DIR)/$(GCC_PREFIX)gcc
	CCFLAGS=\
		$(INCLUDE_SCRIPT) \
		$(HEADERS_SCRIPT) \
		$(OPTIMIZE) \
		$(DEFINES) \
		-std=gnu11 \
		-ffreestanding \
		-Wall \
		-mcmodel=kernel \
		-mno-red-zone
	CPP=$(GCC_DIR)/$(GCC_PREFIX)g++
	CPPFLAGS=\
		$(INCLUDE_SCRIPT) \
		$(HEADERS_SCRIPT) \
		$(OPTIMIZE) \
		$(DEFINES) \
		-std=gnu++17 \
		-ffreestanding \
		-Wall \
		-fno-exceptions \
		-fno-rtti \
		-mcmodel=kernel \
		-mno-red-zone
	LD=$(CC)
	LDFLAGS=\
		$(OPTIMIZE) \
		$(DEFINES) \
		-z max-page-size=4096 \
		-ffreestanding \
		-nostdlib \
		-static \
		-mno-red-zone
endif
ifeq ($(ARCH), x86)
	GCC_PREFIX=i686-elf-
	AS=$(GCC_DIR)/$(GCC_PREFIX)as
	ASFLAGS=\
		$(INCLUDE_SCRIPT)
	CC=$(GCC_DIR)/$(GCC_PREFIX)gcc
	CCFLAGS=\
		$(INCLUDE_SCRIPT) \
		$(HEADERS_SCRIPT) \
		$(OPTIMIZE) \
		$(DEFINES) \
		-std=gnu11 \
		-ffreestanding \
		-Wall
	CPP=$(GCC_DIR)/$(GCC_PREFIX)g++
	CPPFLAGS=\
		$(INCLUDE_SCRIPT) \
		$(HEADERS_SCRIPT) \
		$(OPTIMIZE) \
		$(DEFINES) \
		-std=gnu++17 \
		-ffreestanding \
		-Wall \
		-fno-exceptions \
		-fno-rtti
	LD=$(CC)
	LDFLAGS=\
		$(OPTIMIZE) \
		$(DEFINES) \
		-z max-page-size=4096 \
		-ffreestanding \
		-nostdlib
endif

# GDB settings.
ifeq ($(ARCH), x86_64)
	#GDB=~/opt/gdb-8.1/bin/gdb
	GDB=gdb
else
	#GDB=~/opt/gdb-8.1/bin/gdb -ex "set architecture i386"
	GDB=gdb
endif

# Adds additional compiler flags if debugging.
ifeq ($(DEBUG),true)
	ASFLAGS+= -gdwarf-2
	CCFLAGS+= -gdwarf-2
	CPPFLAGS+= -gdwarf-2
endif

# Add additional flags if the stack_chk_guard is desired.
ifeq ($(STACK_GUARD), true)
	CCFLAGS += -fstack-protector-all
	CPPFLAGS += -fstack-protector-all
endif

# Grub2 settings
GRUB=grub-mkrescue
GRUB_FLAGS=--directory=/usr/lib/grub/i386-pc

# QEMU application settings
ifeq ($(ARCH), x86_64)
	QEMU=qemu-system-x86_64
endif
ifeq ($(ARCH), x86)
	QEMU=qemu-system-i386
endif

# QEMU disk settings
QEMU_DISK_FORMAT=qcow2
QEMU_DISK=disk.img
QEMU_DISK_SIZE=30M

#QEMU flags settings
ifeq ($(ARCH), x86_64)
	QEMU_MEMORY=800M
	QEMU_CPU=qemu64
	QEMU_SMP=1
endif
ifeq ($(ARCH), x86)
	QEMU_MEMORY=800M
	QEMU_CPU=qemu32
	QEMU_SMP=1
endif

#QEMU flags
QEMU_FLAGS=\
	-cdrom $(ISO) \
	-boot d \
	-no-reboot \
	-no-shutdown \
	-m $(QEMU_MEMORY) \
	-cpu $(QEMU_CPU) \
	-smp $(QEMU_SMP) \
	-vga std \
	-soundhw pcspk,sb16 \
	-enable-kvm

# Bochs application settings.
#BOCHS=$(HOME)/opt/bochs/bin/bochs
BOCHS=bochs

# build objects, binary, and disk image
.PHONY: default
default: iso doc

# rebuild objects, binary, and disk image
.PHONY: remake
remake:
	make clean
	make

# delete contents of build folder
.PHONY: clean
clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR) $(ASM_DIR) $(GRUB_DIR) $(ISO_DIR) $(DEBUG_DIR)

# target to build all objects
.PHONY: obj
obj: $(OBJ)

# build output binary
$(BIN): $(OBJ)
	mkdir -p $(BIN_DIR)
	mkdir -p $(DEBUG_DIR)
	$(LD) -T $(LD_SCRIPT) -o $(BIN) $(LDFLAGS) $(OBJ) -lgcc

# target to build output binary
.PHONY: bin
bin: $(BIN)

# target to produce output iso
.PHONY: iso
iso: $(ISO)

# Run CPPCheck on entire codebase.
.PHONY: cppcheck
cppcheck: $(SRC)
	cppcheck -q --enable=all .

# Print LOC stats.
.PHONY: cloc
cloc:
	cloc . --exclude-dir=build

# Generate documentation.
.PHONY: doc
doc: $(DOC_DIR)/html/index.html

# build output iso
$(ISO): $(BIN)
	mkdir -p $(GRUB_DIR)/boot/grub
	mkdir -p $(ISO_DIR)
	cp $(SRC_DIR)/grub/grub.cfg $(GRUB_DIR)/boot/grub/
	cp $(SRC_DIR)/grub/manifest.txt $(GRUB_DIR)/boot/
	cp $(BIN) $(GRUB_DIR)/boot/$(VERSION).bin
	$(GRUB) $(GRUB_FLAGS) -o $(ISO) $(GRUB_DIR)

.PHONY: qemu
qemu: $(ISO)
	$(TERMINAL) -e "$(QEMU) -s -S -display sdl -monitor stdio $(QEMU_FLAGS)" &
	$(GDB) -tui --quiet $(BIN) -ex "set confirm off" -ex "target remote localhost:1234"

.PHONY: curses-qemu
curses-qemu: $(ISO)
	$(QEMU) -curses $(QEMU_FLAGS)

.PHONY: bochs
bochs: $(ISO)
	$(TERMINAL) -e "$(BOCHS)"

# target to export assembled source files
.PHONY: asm
asm: $(ASM)

# Target to generate HTML documentation using doxygen.
$(DOC_DIR)/html/index.html: $(SRC) $(DOXYGEN_CFG)
	mkdir -p $(DOC_DIR)
	doxygen $(DOXYGEN_CFG)

# assemble assembly files, generic
$(OBJ_DIR)/%.s.o: %.s
	mkdir -p $(@D)
	$(AS) $(ASFLAGS) $< -o $@

# compile C files, generic
$(OBJ_DIR)/%.c.o: %.c
	mkdir -p $(@D)
	$(CC) $(CCFLAGS) -c $< -o $@

# compile C++ files, generic
$(OBJ_DIR)/%.cc.o: %.cc
	mkdir -p $(@D)
	$(CPP) $(CPPFLAGS) -c $< -o $@

# copy assembly files, generic
$(ASM_DIR)/%.s.s: $(OBJ_DIR)/%.s.o
	mkdir -p $(@D)
	objdump -drwS $< > $@

# assemble C files, generic
$(ASM_DIR)/%.c.s: $(OBJ_DIR)/%.c.o
	mkdir -p $(@D)
	objdump -drwS $< > $@

# assemble C++ files, generic
$(ASM_DIR)/%.cc.s: $(OBJ_DIR)/%.cpp.o
	mkdir -p $(@D)
	objdump -drwS $< > $@
