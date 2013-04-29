#
# Makefile to control the compiling, assembling and linking of
# standalone programs in the DSL.  Used for both 4003-406 and
# 4003-506 (with appropriate tweaking).
#

#
# User supplied files
#
U_C_SRC = clock.c klibc.c process.c queue.c scheduler.c sio.c \
	stack.c syscall.c system.c ulibc.c user.c string.c shell/shell.c \
	shell/ps.c shell/clear.c shell/help.c shell/reboot.c shell/echo.c \
	shell/halt.c shell/lspci.c shell/test.c shell/nettest.c pci/pci.c \
	disk.c network/8255x.c network/net_handler.c

U_C_OBJ = clock.o klibc.o process.o queue.o scheduler.o sio.o \
	stack.o syscall.o system.o ulibc.o user.o string.o shell/shell.o\
	shell/ps.o shell/clear.o shell/help.o shell/reboot.o shell/echo.o\
	shell/halt.o shell/lspci.o shell/test.o shell/nettest.o pci/pci.o \
	disk.o network/8255x.o network/net_handler.o

U_S_SRC = klibs.S ulibs.S
U_S_OBJ = klibs.o ulibs.o

U_LIBS	=

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
INCLUDES = -I./include

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

.c.o:
	$(CC) $(CFLAGS) -c $*.c -o $*.o

# Binary/source file for system bootstrap code

BOOT_OBJ = bootstrap.b
BOOT_SRC = bootstrap.S

# Assembly language object/source files

S_OBJ = startup.o isr_stubs.o $(U_S_OBJ)
S_SRC =	startup.S isr_stubs.S $(U_S_SRC)

# C object/source files

C_OBJ =	c_io.o support.o $(U_C_OBJ)
C_SRC =	c_io.c support.c $(U_C_SRC)

# Header files

H_SRC = include/bootstrap.h include/c_io.h include/startup.h include/support.h $(U_H_SRC)

# Collections of files

OBJECTS = $(S_OBJ) $(C_OBJ)

SOURCES = $(BOOT_SRC) $(S_SRC) $(C_SRC)

#
# Targets for remaking bootable image of the program
#
# Default target:  usb.image
#

usb.image: bootstrap.b prog.b BuildImage
	./BuildImage -d usb -o usb.image -b bootstrap.b prog.b 0x10000

prog.o:	$(OBJECTS)
	$(LD) -o prog.o -Ttext 0x10000 $(OBJECTS) $(U_LIBS)

prog.b:	prog.o
	$(LD) -o prog.b -s --oformat binary -Ttext 0x10000 prog.o

#
# Targets for copying bootable image onto boot devices
#

floppy:	floppy.image
	dd if=floppy.image of=/dev/fd0

usb:	usb.image
	dd if=usb.image of=/local/devices/disk

#
# Special rule for creating the modification and offset programs
#
# These are required because we don't want to use the same options
# as for the standalone binaries.
#

BuildImage:	BuildImage.c
	$(CC) -o BuildImage BuildImage.c

Offsets:	Offsets.c include/common.h include/queue.h include/process.h
	$(CC) $(INCLUDES) -o Offsets Offsets.c

#
# Clean out this directory
#

clean:
	$(RM) *.nl *.lst *.b *.o **/*.o *.image *.dis BuildImage Offsets

#
# Create a printable namelist from the prog.o file
#

prog.nl: prog.o
	nm -Bng prog.o | pr -w80 -3 > prog.nl

#
# Generate a disassembly
#

prog.dis: prog.o
	objdump -D prog.o > prog.dis

#
#       makedepend is a program which creates dependency lists by
#       looking at the #include lines in the source files
#

depend:
	makedepend -- $(CFLAGS) -- $(SOURCES)

# DO NOT DELETE THIS LINE -- make depend depends on it.

bootstrap.o: ./include/bootstrap.h
startup.o: ./include/bootstrap.h
isr_stubs.o: ./include/bootstrap.h
ulibs.o: ./include/syscall.h ./include/common.h
c_io.o: ./include/c_io.h ./include/startup.h ./include/support.h
c_io.o: ./include/x86arch.h
support.o: ./include/startup.h ./include/support.h ./include/c_io.h
support.o: ./include/x86arch.h ./include/bootstrap.h
clock.o: ./include/common.h ./include/x86arch.h ./include/startup.h
clock.o: ./include/clock.h ./include/process.h ./include/stack.h
clock.o: ./include/queue.h ./include/scheduler.h ./include/sio.h
clock.o: ./include/syscall.h
klibc.o: ./include/common.h ./include/queue.h ./include/stack.h
process.o: ./include/common.h ./include/process.h ./include/clock.h
process.o: ./include/stack.h ./include/queue.h
queue.o: ./include/common.h ./include/queue.h ./include/process.h
queue.o: ./include/clock.h ./include/stack.h ./include/syscall.h
queue.o: ./include/sio.h ./include/scheduler.h
scheduler.o: ./include/common.h ./include/scheduler.h ./include/process.h
scheduler.o: ./include/clock.h ./include/stack.h ./include/queue.h
sio.o: ./include/common.h ./include/sio.h ./include/queue.h
sio.o: ./include/process.h ./include/clock.h ./include/stack.h
sio.o: ./include/scheduler.h ./include/system.h ./include/startup.h
sio.o: ./include/uart.h ./include/x86arch.h
stack.o: ./include/common.h ./include/stack.h ./include/queue.h
syscall.o: ./include/common.h ./include/syscall.h ./include/process.h
syscall.o: ./include/clock.h ./include/stack.h ./include/queue.h
syscall.o: ./include/scheduler.h ./include/sio.h ./include/support.h
syscall.o: ./include/startup.h ./include/x86arch.h
system.o: ./include/common.h ./include/system.h ./include/process.h
system.o: ./include/clock.h ./include/stack.h ./include/bootstrap.h
system.o: ./include/syscall.h ./include/sio.h ./include/queue.h
system.o: ./include/scheduler.h ./include/pci.h ./include/8255x.h
system.o: ./include/startup.h ./include/x86arch.h ./include/user.h
system.o: ./include/ulib.h
ulibc.o: ./include/common.h
user.o: ./include/common.h ./include/user.h ./include/c_io.h
user.o: ./include/shell.h
string.o: ./include/string.h ./include/common.h
shell/shell.o: ./include/shell.h ./include/common.h ./include/string.h
shell/ps.o: ./include/common.h
shell/clear.o: ./include/common.h
shell/help.o: ./include/common.h
shell/reboot.o: ./include/common.h ./include/string.h ./include/shell.h
shell/echo.o: ./include/common.h
shell/halt.o: ./include/common.h ./include/string.h ./include/shell.h
shell/lspci.o: ./include/common.h ./include/string.h ./include/pci.h
shell/test.o: ./include/disk.h ./include/common.h
shell/nettest.o: ./include/common.h ./include/startup.h ./include/8255x.h
pci/pci.o: ./include/common.h ./include/pci.h ./include/startup.h
disk.o: ./include/common.h ./include/disk.h ./include/pci.h
network/8255x.o: ./include/common.h ./include/startup.h ./include/pci.h
network/8255x.o: ./include/8255x.h ./include/net_handler.h
network/net_handler.o: ./include/common.h ./include/startup.h ./include/pci.h
network/net_handler.o: ./include/8255x.h ./include/net_handler.h
