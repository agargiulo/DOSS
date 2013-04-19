#
# Makefile to control the compiling, assembling and linking of
# standalone programs in the DSL.  Used for both 4003-406 and
# 4003-506 (with appropriate tweaking).
#
#
# User supplied files
#
# Various directories
SRCPATH = src
BUILDPATH = bin
INCPATH = include
SHELL_PATH = shell
NETWORK_PATH = network
PCI_PATH = pci

U_C_SHELL_SRC = $(wildcard $(SRCPATH)/$(SHELL_PATH)/*.c)
U_C_NETWORK_SRC = $(wildcard $(SRCPATH)/$(NETWORK_PATH)/*.c)
U_C_PCI_SRC = $(wildcard $(SRCPATH)/$(PCI_PATH)/*.c)

# The src files before we append src/ to them
U_C_ORG_SRC = clock.c klibc.c process.c queue.c scheduler.c sio.c \
	stack.c syscall.c system.c ulibc.c user.c string.c

U_C_SRC = $(foreach src, $(U_C_ORG_SRC),$(SRCPATH)/$(src)) \
$(U_C_SHELL_SRC) $(U_C_NETWORK_SRC) $(U_C_PCI_SRC)

U_C_OBJ = $(patsubst $(SRCPATH)/%.c,$(BUILDPATH)/%.o, $(U_C_SRC))

U_S_SRC = $(foreach src, klibs.S ulibs.S,$(SRCPATH)/$(src))
U_S_OBJ = $(patsubst $(SRCPATH)/%.S,$(BUILDPATH)/%.o, $(U_S_SRC))

U_LIBS	=

VPATH = . shell pci 
#
# User compilation/assembly definable options
#
#	ISR_DEBUGGING_CODE	include context restore debugging code
#	CLEAR_BSS_SEGMENT	include code to clear all BSS space
#	SP2_CONFIG		enable SP2-specific startup variations
#	REPORT_MYSTERY_INTS	print a message on interrupt 0x27
#
USER_OPTIONS = -DCLEAR_BSS_SEGMENT -DSP2_CONFIG

#
# YOU SHOULD NOT NEED TO CHANGE ANYTHING BELOW THIS POINT!!!
#
# Compilation/assembly control
#

#
# We only want to include from the current directory and ~wrc/include
#
INCLUDES = -I$(INCPATH)

#
# Compilation/assembly/linking commands and options
#
CPP = cpp
# CPPFLAGS = $(USER_OPTIONS) -nostdinc -I- $(INCLUDES)
CPPFLAGS = $(USER_OPTIONS) -nostdinc $(INCLUDES)

CC = gcc
CFLAGS = -std=c99 -fno-stack-protector -fno-builtin -Wall -Wstrict-prototypes $(CPPFLAGS)

AS = as
ASFLAGS =

LD = ld

#		
# Transformation rules - these ensure that all compilation
# flags that are necessary are specified
#
# Note use of 'cpp' to convert .S files to temporary .s files: this allows
# use of #include/#define/#ifdef statements. However, the line numbers of
# error messages reflect the .s file rather than the original .S file. 
# (If the .s file already exists before a .S file is assembled, then
# the temporary .s file is not deleted.  This is useful for figuring
# out the line numbers of error messages, but take care not to accidentally
# start fixing things by editing the .s file.)
#

.SUFFIXES:	.S .b

.c.s:
	$(CC) $(CFLAGS) -S $*.c

.S.s:
	$(CPP) $(CPPFLAGS) -o $*.s $*.S

.S.o:
	$(CPP) $(CPPFLAGS) -o $*.s $*.S
	$(AS) -o $*.o $*.s -a=$*.lst
	$(RM) $*.s

.s.b:
	$(AS) -o $*.o $*.s -a=$*.lst
	$(LD) -Ttext 0x0 -s --oformat binary -e begtext -o $*.b $*.o

$(SRCPATH)/%.c$(BUILDPATH)/%.o:
	$(CC) $(CFLAGS) -c $*.c -o $*.o

# Binary/source file for system bootstrap code

BOOT_OBJ = $(BUILDPATH)/bootstrap.b
BOOT_SRC = $(SRCPATH)/bootstrap.S

# Assembly language object/source files

S_OBJ = $(foreach obj, startup.o isr_stubs.o,$(BUILDPATH)/$(obj)) $(U_S_OBJ)
S_SRC = $(foreach src, startup.S isr_stubs.S,$(SRCPATH)/$(src)) $(U_S_SRC)

# C object/source files

C_OBJ = $(foreach obj, c_io.o support.o,$(BUILDPATH)/$(obj)) $(U_C_OBJ)
C_SRC = $(foreach src, c_io.c support.c,$(SRCPATH)/$(src)) $(U_C_SRC)

# Collections of files

OBJECTS = $(S_OBJ) $(C_OBJ)

SOURCES = $(BOOT_SRC) $(S_SRC) $(C_SRC)

#
# Targets for remaking bootable image of the program
#
# Default target:  usb.image
#

usb.image: bin/bootstrap.b prog.b BuildImage
	./BuildImage -d usb -o usb.image -b $(BUILDPATH)/bootstrap.b $(BUILDPATH)/prog.b 0x10000

bin/bootstrap.b: $(BOOT_SRC)
	$(CPP) $(CPPFLAGS) -o $*.s $<
	$(AS) -o $*.o $*.s -a=$*.lst
	$(LD) -Ttext 0x0 -s --oformat binary -e begtext -o $*.b $*.o
	$(RM) $(foreach bin, lst o s,$*.$(bin))

test:
	@echo Objects:
	@echo $(OBJECTS)
	@echo
	@echo U_S_OBJ:
	@echo $(U_S_OBJ)
	@echo
	@echo Sources:
	@echo $(SOURCES)

prog.o: $(OBJECTS)
	$(LD) -o $(BUILDPATH)/prog.o -Ttext 0x10000 $(OBJECTS) $(U_LIBS)

prog.b: prog.o
	$(LD) -o $(BUILDPATH)/prog.b -s --oformat binary -Ttext 0x10000 $(BUILDPATH)/prog.o

#
# Targets for copying bootable image onto boot devices
#

usb: usb.image
	dd if=usb.image of=/local/devices/disk

#
# Special rule for creating the modification and offset programs
#
# These are required because we don't want to use the same options
# as for the standalone binaries.
#

BuildImage: $(SRCPATH)/BuildImage.c
	$(CC) -o BuildImage BuildImage.c

Offsets: $(SRCPATH)/Offsets.c $(foreach header, common.h queue.h process.h,$(INCPATH)/$(header))
	$(CC) $(INCLUDES) -o Offsets Offsets.c

#
# Clean out this directory
#

clean:
	$(RM) **/*.nl **/*.lst **/*.b **/*.o *.image *.dis BuildImage Offsets

#
# Create a printable namelist from the prog.o file
#

prog.nl: $(BUILDPATH)/prog.o
	nm -Bng $(BUILDPATH)/prog.o | pr -w80 -3 > prog.nl

#
# Generate a disassembly
#

prog.dis: $(BUILDPATH)/prog.o
	objdump -D $(BUILDPATH)/prog.o > prog.dis

#
#       makedepend is a program which creates dependency lists by
#       looking at the #include lines in the source files
#

depend:
	makedepend -- $(CFLAGS) -- $(SOURCES)
	@#./fix_makedepend

.PHONY: Makefile bootstrap.b bin/klibs.o

# DO NOT DELETE THIS LINE -- make depend depends on it.

bin/bootstrap.o: include/bootstrap.h
bin/startup.o: include/bootstrap.h
bin/isr_stubs.o: include/bootstrap.h
bin/ulibs.o: include/syscall.h include/common.h
bin/c_io.o: include/c_io.h include/startup.h include/support.h
bin/c_io.o: include/x86arch.h
bin/support.o: include/startup.h include/support.h include/c_io.h
bin/support.o: include/x86arch.h include/bootstrap.h
bin/clock.o: include/common.h include/x86arch.h include/startup.h
bin/clock.o: include/clock.h include/process.h include/stack.h
bin/clock.o: include/queue.h include/scheduler.h include/sio.h
bin/clock.o: include/syscall.h
bin/klibc.o: include/common.h include/queue.h include/stack.h
bin/process.o: include/common.h include/process.h include/clock.h
bin/process.o: include/stack.h include/queue.h
bin/queue.o: include/common.h include/queue.h include/process.h
bin/queue.o: include/clock.h include/stack.h include/syscall.h include/sio.h
bin/queue.o: include/scheduler.h
bin/scheduler.o: include/common.h include/scheduler.h include/process.h
bin/scheduler.o: include/clock.h include/stack.h include/queue.h
bin/sio.o: include/common.h include/sio.h include/queue.h include/process.h
bin/sio.o: include/clock.h include/stack.h include/scheduler.h
bin/sio.o: include/system.h include/startup.h include/uart.h
bin/sio.o: include/x86arch.h
bin/stack.o: include/common.h include/stack.h include/queue.h
bin/syscall.o: include/common.h include/syscall.h include/process.h
bin/syscall.o: include/clock.h include/stack.h include/queue.h
bin/syscall.o: include/scheduler.h include/sio.h include/support.h
bin/syscall.o: include/startup.h include/x86arch.h
bin/system.o: include/common.h include/system.h include/process.h
bin/system.o: include/clock.h include/stack.h include/bootstrap.h
bin/system.o: include/syscall.h include/sio.h include/queue.h
bin/system.o: include/scheduler.h include/pci.h include/startup.h
bin/system.o: include/x86arch.h include/user.h include/ulib.h
bin/ulibc.o: include/common.h
bin/user.o: include/common.h include/user.h include/c_io.h include/shell.h
bin/string.o: include/string.h include/common.h
bin/shell/clear.o: include/common.h
bin/shell/echo.o: include/common.h
bin/shell/help.o: include/common.h
bin/shell/ps.o: include/common.h
bin/shell/reboot.o: include/common.h include/string.h include/shell.h
bin/shell/shell.o: include/shell.h include/common.h include/string.h
bin/pci/pci.o: include/common.h include/pci.h include/startup.h
