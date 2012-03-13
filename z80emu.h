/* z80emu.h
 * Main header of z80emu. Modify it to suit your needs.
 *
 * Copyright (c) 2012 Lin Ke-Fong
 *
 * This program is free, do whatever you want with it.
 */

#ifndef __Z80_INCLUDED__

#ifdef __cplusplus
extern "C" {
#endif

/* Define or comment the following macros to control the features implemented
 * in the emulator. 
 */

/* Define this macro if the host processor is big endian. */

/* #define Z80_BIG_ENDIAN */

/* Emulation can be speed up a little bit by emulating only the documented
 * flags.
 */     

/* #define Z80_DOCUMENTED_FLAGS_ONLY */

/* The emulator cannot be stopped between prefixed opcodes. This can be a 
 * problem if there is a long sequence of 0xdd and/or 0xfd prefixes. But if
 * Z80_PREFIX_FAILSAFE is defined, it will always be able to stop after at 
 * least number_cycles cycles are executed. This is safer but a little bit 
 * slower for 0xdd and 0xfd prefixes handling. Most program won't need this
 * feature.
 */

/* #define Z80_PREFIX_FAILSAFE */

/* HALT, DI, EI, RETI, and RETN instructions can be catched. When such an
 * instruction is catched, the emulator is stopped and the PC register points
 * at the opcode to be executed next. The catched instruction can be retrieved
 * from the flags of Z80_STATE's status member. Keep in mind that no interrupt
 * can be accepted at the instruction right after a DI or EI on an actual Z80 
 * processor.
 */

/*      
#define Z80_CATCH_HALT
#define Z80_CATCH_DI
#define Z80_CATCH_EI
#define Z80_CATCH_RETI
#define Z80_CATCH_RETN
*/

/* Undefined 0xed prefixed opcodes may be catched, otherwise they are treated
 * like NOPs. When one is catched, Z80_STATUS_FLAG_ED_UNDEFINED is set in 
 * Z80_STATE's status member and the PC register points at the 0xed prefix 
 * before the undefined opcode. The elapsed cycles are not counted.
 */

/* #define Z80_CATCH_ED_UNDEFINED */
        
/* By defining this macro, the emulator will always fetch the displacement or 
 * address of a conditionnal jump or call instruction, even if the condition 
 * is false and the fetch can be avoided.
 */

/* #define Z80_FALSE_CONDITION_FETCH */

/* It may be possible to overwrite the opcode of the currently executing LDIR, 
 * LDDR, INIR, or OTDR instruction. Define this macro, if you need to handle 
 * those pathological cases.
 */

/* #define Z80_HANDLE_SELF_MODIFYING_CODE */

/* Flags for Z80_STATE's status member. If the emulation is interrupted, status
 * can indicate why. You may add additionnal flags for your own use as needed.
 */

#define Z80_STATUS_FLAG_HALT            (1 << 0)
#define Z80_STATUS_FLAG_DI              (1 << 1)
#define Z80_STATUS_FLAG_EI              (1 << 2)
#define Z80_STATUS_FLAG_RETI            (1 << 3)
#define Z80_STATUS_FLAG_RETN            (1 << 4)
#define Z80_STATUS_FLAG_ED_UNDEFINED    (1 << 5)
 
/* The main registers are stored inside Z80_STATE as an union of arrays named 
 * registers. They are referenced using indexes. Words are stored in the 
 * endianness of the host processor. The alternate set of word registers AF', 
 * BC', DE', and HL' is stored in the alternates member of Z80_STATE, as an 
 * array using the same ordering.
 */

#ifdef Z80_BIG_ENDIAN

#       define Z80_B            0
#       define Z80_C            1
#       define Z80_D            2
#       define Z80_E            3
#       define Z80_H            4
#       define Z80_L            5
#       define Z80_A            6
#       define Z80_F            7

#       define Z80_IXH          8
#       define Z80_IXL          9
#       define Z80_IYH          10
#       define Z80_IYL          11

#else

#       define Z80_B            1
#       define Z80_C            0
#       define Z80_D            3
#       define Z80_E            2
#       define Z80_H            5
#       define Z80_L            4
#       define Z80_A            7
#       define Z80_F            6

#       define Z80_IXH          9
#       define Z80_IXL          8
#       define Z80_IYH          11
#       define Z80_IYL          10
                                
#endif

#define Z80_BC                  0
#define Z80_DE                  1
#define Z80_HL                  2
#define Z80_AF                  3

#define Z80_IX                  4
#define Z80_IY                  5 
#define Z80_SP                  6

/* Z80's flags. */

#define Z80_S_FLAG_SHIFT        7       
#define Z80_Z_FLAG_SHIFT        6
#define Z80_Y_FLAG_SHIFT        5
#define Z80_H_FLAG_SHIFT        4
#define Z80_X_FLAG_SHIFT        3
#define Z80_PV_FLAG_SHIFT       2
#define Z80_N_FLAG_SHIFT        1
#define Z80_C_FLAG_SHIFT        0

#define Z80_S_FLAG              (1 << Z80_S_FLAG_SHIFT)
#define Z80_Z_FLAG              (1 << Z80_Z_FLAG_SHIFT)
#define Z80_Y_FLAG              (1 << Z80_Y_FLAG_SHIFT)
#define Z80_H_FLAG              (1 << Z80_H_FLAG_SHIFT)
#define Z80_X_FLAG              (1 << Z80_X_FLAG_SHIFT)
#define Z80_PV_FLAG             (1 << Z80_PV_FLAG_SHIFT)
#define Z80_N_FLAG              (1 << Z80_N_FLAG_SHIFT)
#define Z80_C_FLAG              (1 << Z80_C_FLAG_SHIFT)

#define Z80_P_FLAG_SHIFT        Z80_PV_FLAG_SHIFT
#define Z80_V_FLAG_SHIFT        Z80_PV_FLAG_SHIFT
#define Z80_P_FLAG              Z80_PV_FLAG
#define Z80_V_FLAG              Z80_PV_FLAG

/* Z80's three interrupt modes. */

enum {

        Z80_INTERRUPT_MODE_0,
        Z80_INTERRUPT_MODE_1,
        Z80_INTERRUPT_MODE_2

};

/* Z80 processor's state, you may add additionnal members as necessary. */

typedef struct {

        int             status;
                                
        union {
                                
                unsigned char   byte[14];
                unsigned short  word[7];

        } registers;

        unsigned short  alternates[4];

        int             i, r, pc, iff1, iff2, im;

} Z80_STATE;

/* Write the following macros for memory access and input/output on the Z80. 
 *
 * Z80_FETCH_BYTE() and Z80_FETCH_WORD() are used by the emulator to read the
 * code (opcode, constants, displacement, etc). The upper 16-bit of the address
 * parameters is undefined and must be reset to zero before actually reading 
 * memory (use & 0xffff). The value x read, must be an unsigned 8-bit or 16-bit
 * value in the endianness of the host processor.
 *
 * Z80_READ_BYTE(), Z80_WRITE_BYTE(), Z80_READ_WORD(), and Z80_WRITE_WORD()
 * are used for general memory access. They obey the same rule as the code 
 * reading macros. The upper bits of the value x to write may be non-zero.
 * 
 * Z80_INPUT_BYTE() and Z80_OUTPUT_BYTE() are for input and output. The upper
 * bits of the port number to read or write are always zero. The input byte x 
 * must be an unsigned 8-bit value. The value x to write is an unsigned 8-bit 
 * with its upper bits zeroes.
 *
 * All macros have access to the following variables:
 *
 *      state           Pointer to the current Z80_STATE. Member registers is
 *                      most likely not up to date because the current 
 *                      executing instruction is working on it.
 *
 *      pc              Current PC register (upper bits are undefined). It
 *                      points on the displacement or constant to read for
 *                      Z80_FETCH_BYTE() and Z80_FETCH_WORD(), or on the next
 *                      instruction otherwise.
 *              
 *      number_cycles   Number of cycles to emulate. Set number_cycles to zero
 *                      if you want to quit emulation after completion of the
 *                      current instruction.
 *
 *      elapsed_cycles  Number of cycles emulated, you may add wait states
 *                      for slow memory accesses as needed.
 *
 *      instruction     Type of the currently executing instruction, see
 *                      instructions.h for a list.
 *
 *      registers       Current register decoding table, it points on 
 *                      dd_register_table for 0xdd prefixed instructions, 
 *                      fd_register_table for 0xfd ones, or register_table
 *                      otherwise.
 *
 *      opcode          Opcode of the currently executing instruction, check
 *                      register_table for 0xdd or 0xfd prefixes.
 */

/* Here are macros for the zextest example. Read/write memory macros have been 
 * written for a linear 64k RAM. Input/output port macros are used to simulate 
 * system calls.
 */

#include "zextest.h"

#define Z80_FETCH_BYTE(address, x)                                      \
{                                                                       \
        (x) = memory[(address) & 0xffff];                               \
}

#define Z80_FETCH_WORD(address, x)                                      \
{                                                                       \
        (x) = memory[(address) & 0xffff]                                \
                | (memory[((address) + 1) & 0xffff] << 8);              \
}

#define Z80_READ_BYTE(address, x)                                       \
{                                                                       \
        (x) = memory[(address) & 0xffff];                               \
}

#define Z80_WRITE_BYTE(address, x)                                      \
{                                                                       \
        memory[(address) & 0xffff] = (x);                               \
}

#define Z80_READ_WORD(address, x)                                       \
{                                                                       \
        (x) = memory[(address) & 0xffff]                                \
                | (memory[((address) + 1) & 0xffff] << 8);              \
}

#define Z80_WRITE_WORD(address, x)                                      \
{                                                                       \
        memory[(address) & 0xffff] = x;                                 \
        memory[((address) + 1) & 0xffff] = x >> 8;                      \
}

#define Z80_INPUT_BYTE(port, x)                                         \
{                                                                       \
        (x) = 0;                /* Make compiler happy. */              \
        SystemCall(state);                                              \
}

#define Z80_OUTPUT_BYTE(port, x)                                        \
{                                                                       \
        number_cycles = 0;                                              \
        state->status |= FLAG_STOP_EMULATION;                           \
}

/* See comments in z80emu.c for a description of each functions. */

extern void     Z80Reset (Z80_STATE *state);

extern int      Z80Interrupt (Z80_STATE *state, int data_on_bus);
extern int      Z80NonMaskableInterrupt (Z80_STATE *state);
 
extern int      Z80Emulate (Z80_STATE *state, int number_cycles);

#ifdef __cplusplus
}
#endif

#define __Z80_INCLUDED__

#endif
