/* z80config.h
 * Define or comment out macros in this file to configure the emulator. 
 *
 * Copyright (c) 2016 Lin Ke-Fong
 *
 * This program is free, do whatever you want with it.
 */

#ifndef __Z80CONFIG_INCLUDED__
#define __Z80CONFIG_INCLUDED__

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
 * like NOP instructions. When one is catched, Z80_STATUS_FLAG_ED_UNDEFINED is 
 * set in Z80_STATE's status member and the PC register points at the 0xed 
 * prefix before the undefined opcode. The elapsed cycles are not counted.
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

#endif
