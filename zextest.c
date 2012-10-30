/* zextest.c
 * Example program using z80emu to run the zexall and zexdoc tests. This will 
 * check if the Z80 is correctly emulated.
 *
 * Copyright (c) 2012 Lin Ke-Fong
 *
 * This program is free, do whatever you want with it.
 */

#include <stdio.h>
#include <stdlib.h>
#include "z80emu.h"

#include <inttypes.h>

#ifdef _MSC_VER

typedef __int64 int64_t;

#else

#include <stdint.h>

#endif

#define Z80_CPU_SPEED           4000000
#define CYCLES_PER_STEP         (Z80_CPU_SPEED / 50)
#define MAXIMUM_STRING_LENGTH   100

unsigned char   memory[1 << 16];

static void     emulate (char *filename);

int main (void)

{
        emulate("testfiles/zexdoc.com");
        emulate("testfiles/zexall.com");        
        return EXIT_SUCCESS;
}

/* Emulate "zexdoc.com" or "zexall.com". */

static void emulate (char *filename)

{
        FILE            *file;
        long            l;
        Z80_STATE       state;
        int64_t         total;

        printf("Testing \"%s\"...\n", filename);

        if ((file = fopen(filename, "rb")) == NULL) {

                fprintf(stderr, "Can't open file!\n");
                exit(EXIT_FAILURE);

        }

        fseek(file, 0, SEEK_END);
        l = ftell(file);

        fseek(file, 0, SEEK_SET);
        fread(memory + 0x100, 1, l, file);

        fclose(file);

        /* Patch memory of the program. Reset at 0x0000 is trapped by an OUT
         * which will stop emulation. CP/M bdos call 5 is trapped by an IN. See
         * Z80_INPUT_BYTE() and Z80_OUTPUT_BYTE() definitions in z80emu.h.
         */

        memory[0] = 0xd3;       /* OUT N, A */
        memory[1] = 0x00;

        memory[5] = 0xdb;       /* IN A, N */
        memory[6] = 0x00;
        memory[7] = 0xc9;       /* RET */

        /* Emulate. */

        Z80Reset(&state);        
        state.pc = 0x100;
        total = 0;
        for ( ; ; ) {

                total += Z80Emulate(&state, CYCLES_PER_STEP);
                if (state.status & FLAG_STOP_EMULATION) 

                        break;

        }
        printf("\n%" PRId64 " cycle(s) emulated.\n" 
                "For a Z80 running at %dMHz, "
                "that would be %d second(s) or %.2f hour(s).\n",
                total,
                Z80_CPU_SPEED, 
                (int) (total / Z80_CPU_SPEED),
                total / ((double) 3600 * Z80_CPU_SPEED));
}

/* Emulate CP/M bdos call 5 functions 2 (output character on screen) and 9
 * (output $-terminated string to screen).
 */

void SystemCall (Z80_STATE *state)
{
        if (state->registers.byte[Z80_C] == 2)

                printf("%c", state->registers.byte[Z80_E]);

        else if (state->registers.byte[Z80_C] == 9) {

                int     i, c;

                for (i = state->registers.word[Z80_DE], c = 0; 
                        memory[i] != '$';
                        i++) {

                        printf("%c", memory[i & 0xffff]);
                        if (c++ > MAXIMUM_STRING_LENGTH) {

                                fprintf(stderr,
                                        "String to print is too long!\n");
                                exit(EXIT_FAILURE);

                        }

                }

        }
}
