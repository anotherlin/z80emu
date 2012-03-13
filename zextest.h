/* zextest.h
 * Header for zextest example.
 *
 * Copyright (c) 2012 Lin Ke-Fong
 *
 * This program is free, do whatever you want with it.
 */

#ifndef __ZEXTEST_INCLUDED__

/* Additional Z80_STATE status flag to request emulation termination. */
                                
#define FLAG_STOP_EMULATION     (1 << 31)

extern unsigned char    memory[1 << 16];

extern void     SystemCall (Z80_STATE *state);

#define __ZEXTEST_INCLUDED__

#endif
