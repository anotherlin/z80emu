z80emu
Version 1.1.3

Copyright (c) 2012-2017 Lin Ke-Fong
 
z80emu is a free portable Z80 processor emulator. Its performance is comparable
to other Z80 emulators. It emulates all undocumented features as described in 
"The Undocumented Z80 Documented Version 0.91" and passes both the zexdoc and 
zexall Z80 instruction exerciser tests. Code is pure ANSI C.
 
The Makefile will compile a sample program to run zexdoc.com and zexall.com.
Only needed CP/M BIOS functions are emulated. See zextest.c for details.
 
All structures and functions are documented in the code, please read the header
files. To use z80emu in your programs, modify z80config.h to configure the 
emulator, and in z80user.h, write the memory read/write and port input/output 
macros to interface with your system to emulate. The macros for zextest.c are 
simple examples. You shouldn't have to modify z80emu.c or any other files.
 
z80emu works a little bit differently from other emulators. Instead of a switch
case for all opcodes, it converts them to "generic" instructions and then do
the switch case. Registers and operands are then decoded during execution. All 
of this is done using several tables (see maketables.c). The code is more 
compact, which should make it fit inside L1 instruction cache.
 
You may find further information regarding the Z80 on http://www.z80.info. This
emulator is heavily based on the information found on this website. And I would
like to thank all its contributors. In particular, Sean Young for "The 
Undocumented Z80 Documented" and Christian Dinu for "Decoding Z80 Opcodes". The
zexdoc and zexall programs have been written by Frank D. Cringles.
 
I would like to thank Hugo Clement, Guy Hermann, Julien Montmartin, and Pierre
Rousseau for review and testing.
 
Sebastien Katz, Pierre Rousseau, and Thibaut Mattern made me start this project
as part of a Sega Master System console emulator. That was long ago, back at
university. I hope I will complete a full emulator someday.
 
Feel free to send bug reports, comments, and suggestions to:
 
anotherlin@gmail.com
 
Revision history:

1.1.3 (18th September 2017)

Another gross mistake fixed in the interrupt handling code: In mode 2, the 
interrupt vector wasn't dereferenced! Thank you rootednode for finding and
reporting this issue.

1.1.2 (20th June 2017)

The implementation of the "IM" Interrupt Mode selection instruction was 
completely wrong. The zextests don't check it and it seems that it was never
encountered in the few systems emulated so far. This bug has gone unnoticed 
until now. Thank you Chris Despoinidis for reporting that bug.

1.1.1 (28th February 2017)

I made the wrong assumption that char is signed by default. It is in fact 
implementation (compiler) dependent. So explicit (signed char) casts are now
used instead. Otherwise, with unsigned char compilers, the zextests will have
a few failing CRC checks. In fact, relative jumps will also fail to emulate 
correctly. Thank you SmallRoomLabs for finding and reporting this issue, and 
Mark Allender for verifying it.
 
1.1.0 "first-app" (22th December 2016)

Ten years already! But hopefully, work on the Sega Master System emulator has 
restarted. For the first application of z80emu, redesign its user API, the
original being really bad. Add 2 header files for the user to customize instead
of having to modify z80emu.h, update the documentation: make it clear what 
variables each macro can expect, compute the register decoding tables only once
at reset, and add a (void *) context for interfacing with the system to 
emulate. All these changes should make the API better organized and easier to 
use. 
 
1.0.2 (12th February 2014)
 
Conditional relative jump (JR_DD_E) instructions such as "jr NZ, loop_label"
were not decoded correctly, just a silly bug in a macro to extract bits. The 
zexdoc and zexall exercisers have excellent coverage of ALU instructions, but
they both don't feature a single "JR" instruction! Hence this (very obvious)
bug has gone unnoticed. Thank you Marcelo Dantas.
 
1.0.1 (14th November 2012)
 
Original implementation of zextest used int64_t. In retrospect, this was a poor 
choice. Using double to count cycles is ANSI and has more than enough precision 
to do so. Thank you Chris Pressey.
 
1.0.0 (13th March 2012)
 
Initial commit on github. Actually, the code has been almost entirely written 
during the summer of 2006.
