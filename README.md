# Systems Programming
## Class Project

---Shell Project---
A mini project to make it easier to test the other projects,
and make the OS seem a bit more practical. The shell is organized
such that it is very simple to make new commands. The process is
    1. Add yourcommand.c to the shell directory, and add appropriate
    Makefile additions. This file must include the method 
        "void yourcommand(int argc, char **argv) {}"
    2. Add the prototype  for this method in shell.h
    3. Add a new value for the command_tab in shell.c that follows the
    pattern of the other commands there, and increment the command_count
    by one.

---IDE Project---
Primary Developer: Owen Royall-Kahin
The purpose of this project was to develop a disk driver to communicate with
any of the drives that were connected to the DSL machines, and then write a mini
filesystem that would allow data to be written to the disk, both as a test and
hopefully good enough for the other projects to use, should they have the use 
for persistent storage.

Filesystem (sub-project):
This project was designed to test the hard drive functionality, and potentially
create something useful for general OS use. Combined with the shell and disk 
module, it supports many access commands.

Commands supported:
	touch, cat, ls, rm, append, df, stat
	
Implementing ofs in other applications:
To implement persistant file storage, include ofs.h in your application. Then,
you can use fopen, fread and fwrite as you would expect. However, to create a 
new file, you must use fcreat (which also returns the file_t).
	
Direct sector addressing is possible using the debug command "test".
	test 0 : Reads from sector 0
	test 0 hello : Writes "hello" to sector 0
	
You can use this to trash the filesystem ("test 0 foo"), so that the next time 
the system boots, mount will "reformat" the drive.


