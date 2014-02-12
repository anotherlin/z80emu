z80emu
Version 1.0.2

Copyright (c) 2012-2014 Lin Ke-Fong

z80emu is a free portable Z80 processor emulator. Its performance is comparable
to other Z80 emulators. It emulates all undocumented features as described in 
"The Undocumented Z80 Documented Version 0.91" and passes both the zexdoc and 
zexall Z80 instruction exerciser tests. The emulator core (z80emu.c) is pure 
ANSI C.

The makefile will compile a sample program to run zexdoc.com and zexall.com.
Only needed CP/M BIOS functions are emulated. See zextest.c for details.

All structures and functions are documented in the code. To use z80emu in your 
own programs, you just have to modify z80emu.h to customize it to your needs. 
Memory read/write and in/out macros are written for zextest.c. Rewrite them for
the system you want to emulate. You shouldn't need to modify z80emu.c or any 
other files.

z80emu works a little bit differently from other emulators. Instead of a switch
case for all opcodes, it converts them to "generic" instructions and then do
the switch case. Registers and operands are then decoded during execution. All 
of this is done using several tables (see maketables.c). The code is more 
compact, which should make it fit inside L1 instruction cache. 

You may find further information regarding the Z80 on http://www.z80.info. This
emulator is heavily based on the information found on this website. And I would
like to thank all the contributors. In particular, Sean Young for "The 
Undocumented Z80 Documented" and Christian Dinu for "Decoding Z80 Opcodes". The
zexdoc and zexall programs have been written by Frank D. Cringles.

I would like to thank Hugo Clement, Guy Hermann, Julien Montmartin, and Pierre
Rousseau for review and testing.

Sebastien Katz, Pierre Rousseau, and Thibaut Mattern made me start this project
as part of a Sega Master System console emulator. That was long ago, back at
university. I hope I will complete it someday.

Feel free to send bug reports, comments, and suggestions to:

another_lin@yahoo.com

Revision history:

1.0.0 (13 March 2012)

Initial commit on github. Actually, the code has been almost entirely written 
during the summer of 2006.

1.0.1 (14 November 2012)

Original implementation of zextest used int64_t. In retrospect, this was a poor 
choice. Using double to count cycles is ANSI and has more than enough precision 
to do so. Thank you Chris Pressey (cpressey@gmail.com).

1.0.2 (12 February 2014)

Conditional relative jump (JR_DD_E) instructions such as "jr NZ, myLoopLabel"
were not decoded correctly, just a silly bug in a macro to extract bits. The 
zexdoc and zexall exercisers have excellent coverage of ALU instructions, but
they both don't feature a single "JR" instruction! Hence this (very obvious)
bug has gone unnoticed. Thank you Marcelo Dantas (marcelo.f.dantas@gmail.com).
