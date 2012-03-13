CC = gcc
CFLAGS = -Wall -ansi -pedantic -O2 -fomit-frame-pointer

all: zextest

tables.h: maketables.c
	$(CC) -Wall $< -o maketables
	./maketables > $@

z80emu.o: z80emu.c z80emu.h instructions.h macros.h tables.h
	$(CC) $(CFLAGS) -c $<

zextest.o: zextest.c z80emu.h
	$(CC) -Wall -c $<

OBJECT_FILES = zextest.o z80emu.o

zextest: $(OBJECT_FILES)
	$(CC) $(OBJECT_FILES) -o $@
