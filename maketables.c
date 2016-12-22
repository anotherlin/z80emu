/* maketables.c
 * Instructions and flags tables generator.
 *
 * Copyright (c) 2012, 2016 Lin Ke-Fong
 * 
 * This code is free, do whatever you want with it.
 */

#include <stdio.h>
#include <stdlib.h>
#include "z80emu.h"

/* Encoding for indirect or indexed 8-bit memory operands. */

#define INDIRECT_HL     0x06

static void     make_instruction_table (void);
static void     make_cb_instruction_table (void);
static void     make_ed_instruction_table (void);

static void     make_szyx_flags_table (void);
static void     make_szyxp_flags_table (void);

int main (void) 
{
        printf("/* Generated file, see maketables.c. */\n\n");

        make_instruction_table();
        putchar('\n');
        make_cb_instruction_table();
        putchar('\n');
        make_ed_instruction_table();
        putchar('\n');

        make_szyx_flags_table();
        putchar('\n');
        make_szyxp_flags_table();

        return EXIT_SUCCESS;
}

/* Make single opcodes instruction table. */

static void make_instruction_table (void)
{
        int             i, j, k;
        char            *s, *t;
        static char     *accumulator_operations[8] = {

                                "ADD",
                                "ADC",
                                "SUB",
                                "SBC",
                                "AND",
                                "XOR",
                                "OR",
                                "CP",

                        };

        printf("static const unsigned char INSTRUCTION_TABLE[256] = {\n\n");
        for (k = 0; k < (1 << 6); k++) {

                putchar('\t');
                i = k >> 3;
                j = k & 0x07;
                switch (j) {

                        /* Various opcodes. */

                        case 0x00: {

                                static char     *strings[8] = {

                                                        "NOP",
                                                        "EX_AF_AF_PRIME",
                                                        "DJNZ_E",
                                                        "JR_E",
                                                        "JR_DD_E",
                                                        "JR_DD_E",
                                                        "JR_DD_E",
                                                        "JR_DD_E",

                                                };

                                s = strings[i];
                                break;

                        }

                        /* LD_RR_NN and ADD_HL_RR. */

                        case 0x01: {

                                s = i & 1 ? "ADD_HL_RR" : "LD_RR_NN";
                                break;                                  

                        }

                        /* Indirect loading. */

                        case 0x02: { 
                        
                                static char     *strings[8] = {

                                                        "LD_INDIRECT_BC_A",
                                                        "LD_A_INDIRECT_BC",
                                                        "LD_INDIRECT_DE_A",
                                                        "LD_A_INDIRECT_DE",
                                                        "LD_INDIRECT_NN_HL",
                                                        "LD_HL_INDIRECT_NN",
                                                        "LD_INDIRECT_NN_A",
                                                        "LD_A_INDIRECT_NN",

                                                };

                                s = strings[i];
                                break;

                        }

                        /* 16-bit INC and DEC. */

                        case 0x03: {

                                s = i & 1 ? "DEC_RR" : "INC_RR";
                                break;                                  

                        }

                        /* 8-bit INC and DEC. */

                        case 0x04: 
                        case 0x05: {

                                if (j == 0x04)

                                        printf("INC_");

                                else 

                                        printf("DEC_");

                                if (i == INDIRECT_HL)

                                        s = "INDIRECT_HL";

                                else

                                        s = "R";

                                break;

                        }

                        /* Immediate 8-bit loading. */

                        case 0x06: {

                                if (i == INDIRECT_HL)

                                        s = "LD_INDIRECT_HL_N";

                                else    

                                        s = "LD_R_N";
                                
                                break;

                        }

                        /* RLCA, RRCA, RLA, RRA, DAA, CPL, SCF, and CCF. */

                        case 0x07: 
                        default: {

                                static char     *strings[8] = {

                                                        "RLCA",
                                                        "RRCA",
                                                        "RLA",
                                                        "RRA",
                                                        "DAA",
                                                        "CPL",
                                                        "SCF",
                                                        "CCF",

                                                };

                                s = strings[i];
                                break;

                        }
                        
                }
                printf("%s,\n", s);
                if (j == 0x07)

                        putchar('\n');

        }

        /* 8-bit loading and HALT. */

        for (k = 0; k < (1 << 6); k++) {

                i = k >> 3;
                j = k & 0x07;
                if (i == j) {

                        if (i == INDIRECT_HL)

                                s = "HALT";

                        else

                                s = "NOP";

                        printf("\t%s,\n", s);

                } else {

                        if (i == INDIRECT_HL)

                                s = "LD_INDIRECT_HL_";

                        else

                                s = "LD_R_";

                        if (j == INDIRECT_HL)

                                t = "INDIRECT_HL";

                        else

                                t = "R";

                        printf("\t%s%s,\n", s, t);

                }
                if (j == 0x07)

                        putchar('\n');

        }

        /* Operation on accumulator and register/memory. */

        for (i = 0; i < (1 << 3); i++) {

                s = accumulator_operations[i];
                for (j = 0; j < (1 << 3); j++) {

                        printf("\t%s_", s);
                        if (j == INDIRECT_HL)

                                t = "INDIRECT_HL";

                        else

                                t = "R";

                        printf("%s,\n", t);

                }
                putchar('\n');

        }

        for (k = 0; k < (1 << 6); k++) {

                putchar('\t');
                i = k >> 3;
                j = k & 0x07;
                switch (j) {

                        /* RET CC. */
                                        
                        case 0x00:      s = "RET_CC"; break;

                        /* POP_SS, RET, EXX, JP_HL, and LD_SP_HL. */

                        case 0x01: {

                                if (i & 1) {

                                        static char     *strings[4] = {

                                                                "RET",
                                                                "EXX",
                                                                "JP_HL",
                                                                "LD_SP_HL",

                                                        };

                                        s = strings[i >> 1];

                                } else 

                                        s = "POP_SS";

                                break;

                        }

                        /* JP CC, NN. */

                        case 0x02:      s = "JP_CC_NN"; break;

                        /* Various opcodes. */

                        case 0x03: {

                                static char     *strings[8] = {

                                                        "JP_NN",
                                                        "CB_PREFIX",
                                                        "OUT_N_A",
                                                        "IN_A_N",
                                                        "EX_INDIRECT_SP_HL",
                                                        "EX_DE_HL",
                                                        "DI",
                                                        "EI",

                                                };

                                s = strings[i];
                                break;

                        }

                        /* CALL_CC_NN. */

                        case 0x04:      s = "CALL_CC_NN"; break;

                        /* PUSH_SS, CALL_NN, and prefixes. */

                        case 0x05: {

                                if (i & 1) {

                                        static char     *strings[4] = {

                                                                "CALL_NN",
                                                                "DD_PREFIX",
                                                                "ED_PREFIX",
                                                                "FD_PREFIX",

                                                        };

                                        s = strings[i >> 1];

                                } else 

                                        s = "PUSH_SS";

                                break;

                        }

                        /* Operation on accumulator and immediate. */

                        case 0x06: {

                                printf("%s", accumulator_operations[i]);
                                s = "_N";
                                break;

                        }

                        /* RST_P. */

                        case 0x07:      
                        default:        s = "RST_P"; break;

                }
                printf("%s,\n", s);
                if (j == 0x07)

                        putchar('\n');

        }
        printf("};\n");
}

/* Make 0xcb prefixed opcodes instruction table. */

static void make_cb_instruction_table (void)
{
        int     i;
        char    *s;

        printf("static const unsigned char CB_INSTRUCTION_TABLE[256] = {\n\n");

        /* Rotation/shift operations. */

        for (i = 0; i < (1 << 6); i++) {

                static char     *rotation_shift_operations[8] = {

                                        "RLC",
                                        "RRC",
                                        "RL",
                                        "RR",
                                        "SLA",
                                        "SRA",
                                        "SLL",
                                        "SRL",

                                };

                printf("\t%s_", rotation_shift_operations[i >> 3]);
                if ((i & 0x07) == INDIRECT_HL)

                        s = "INDIRECT_HL";

                else

                        s = "R";

                printf("%s,\n", s);
                if ((i & 0x07) == 0x07)

                        putchar('\n');

        }

        /* BIT_B_R and BIT_B_INDIRECT_HL. */

        for (i = 0; i < (1 << 6); i++) {

                if ((i & 0x07) == INDIRECT_HL)

                        s = "BIT_B_INDIRECT_HL";

                else

                        s = "BIT_B_R";

                printf("\t%s,\n", s);
                if ((i & 0x07) == 0x07)

                        putchar('\n');

        }

        /* RES_B_R and RES_B_INDIRECT_HL. */

        for (i = 0; i < (1 << 6); i++) {

                if ((i & 0x07) == INDIRECT_HL)

                        s = "RES_B_INDIRECT_HL";

                else

                        s = "RES_B_R";

                printf("\t%s,\n", s);
                if ((i & 0x07) == 0x07)

                        putchar('\n');

        }

        /* SET_B_R and SET_B_INDIRECT_HL. */

        for (i = 0; i < (1 << 6); i++) {

                if ((i & 0x07) == INDIRECT_HL)

                        s = "SET_B_INDIRECT_HL";

                else

                        s = "SET_B_R";

                printf("\t%s,\n", s);
                if ((i & 0x07) == 0x07)

                        putchar('\n');

        }

        printf("};\n");
}

/* Make 0xed prefixed opcodes instruction table. */

static void make_ed_instruction_table (void)
{
        int     i, j, k;
        char    *s, *t;

        printf("static const unsigned char ED_INSTRUCTION_TABLE[256] = {\n\n");

        /* Undefined opcodes are catched and will execute as NOPs. */

        for (i = 0; i < (1 << 6); i++) {

                printf("\tED_UNDEFINED,\n");
                if ((i & 0x07) == 0x07)

                        putchar('\n');

        }

        /* 0xed prefixed opcodes. */

        for (k = 0; k < (1 << 6); k++) {

                i = k >> 3;
                j = k & 0x07;
                switch (j) {

                        case 0x00: 
                        case 0x01: { 

                                s = j ? "OUT_C_R" : "IN_R_C";
                                t = "";
                                break;

                        }

                        /* SBC_HL_RR and ADC_HL_RR. */

                        case 0x02: {

                                s = i & 1 ? "ADC" : "SBC";
                                t = "_HL_RR";
                                break;

                        }

                        /* LD_INDIRECT_NN_RR and LD_RR_INDIRECT_NN. */

                        case 0x03: { 

                                s = "LD_";
                                t = i & 1 
                                        ? "RR_INDIRECT_NN"
                                        : "INDIRECT_NN_RR";
                                break;

                        }

                        /* NEG and IM_N. */

                        case 0x04: 
                        case 0x06: { 

                                s = j == 0x04 ? "NEG" : "IM_N";
                                t = "";
                                break;

                        }

                        /* RETN and RETI. */

                        case 0x05: {

                                s = "RETI_RETN";
                                t = "";
                                break;                                    

                        }

                        /* Various opcodes. */

                        case 0x07: 
                        default: {

                                static char     *strings[8] = {

                                                        "LD_I_A_LD_R_A",
                                                        "LD_I_A_LD_R_A",
                                                        "LD_A_I_LD_A_R",
                                                        "LD_A_I_LD_A_R",
                                                        "RLD_RRD",
                                                        "RLD_RRD",
                                                        "ED_UNDEFINED",
                                                        "ED_UNDEFINED",

                                                };                                      
                                s = strings[i];
                                t = "";
                                break;

                        }

                }
                printf("\t%s%s,\n", s, t);
                if ((j & 0x07) == 0x07)

                        putchar('\n');

        }

        /* Block instructions opcodes. */

        for (k = 0; k < (1 << 6); k++) {

                static char     *strings[4][4] = {

                                        { 

                                                "LDI_LDD", 
                                                "CPI_CPD", 
                                                "INI_IND", 
                                                "OUTI_OUTD", 

                                        },

                                        { 

                                                "LDI_LDD", 
                                                "CPI_CPD", 
                                                "INI_IND", 
                                                "OUTI_OUTD", 

                                        },

                                        { 

                                                "LDIR_LDDR", 
                                                "CPIR_CPDR", 
                                                "INIR_INDR", 
                                                "OTIR_OTDR", 

                                        },

                                        { 

                                                "LDIR_LDDR", 
                                                "CPIR_CPDR", 
                                                "INIR_INDR", 
                                                "OTIR_OTDR", 

                                        },

                                };
                i = k >> 3;
                j = k & 0x07;
                if (i < 4 || j > 3)

                        s = "ED_UNDEFINED";

                else

                        s = strings[i - 4][j];

                printf("\t%s, \n", s);
                if ((j & 0x07) == 0x07)

                        putchar('\n');

        }

        for (i = 0; i < (1 << 6); i++) {

                printf("\tED_UNDEFINED,\n");
                if ((i & 0x07) == 0x07)

                        putchar('\n');

        }

        printf("};\n");
}

/* Make S, Z, Y, and X flags table. */

static void make_szyx_flags_table (void)
{
        int     i;

        printf("static const unsigned char SZYX_FLAGS_TABLE[256] = {\n");
        for (i = 0; i < 256; i++) {

                int     r;

                r = i & (Z80_S_FLAG | Z80_Y_FLAG | Z80_X_FLAG);
                r |= !i ? Z80_Z_FLAG : 0;

                if (!(i & 7))

                        printf("\n\t0x%02x, ", r);

                else

                        printf("0x%02x, ", r);


        }
        printf("\n\n};\n");
}

/* Make S, Z, Y, X, and P flags table. */

static void make_szyxp_flags_table (void)
{
        int     i;

        printf("static const unsigned char SZYXP_FLAGS_TABLE[256] = {\n");
        for (i = 0; i < 256; i++) {

                int     j, p, r;

                j = i;
                p = !0;
                while (j) {

                        if (j & 1)

                                p = !p;

                        j >>= 1;

                }
                r = i & (Z80_S_FLAG | Z80_Y_FLAG | Z80_X_FLAG);
                r |= !i ? Z80_Z_FLAG : 0;
                r |= p ? Z80_PV_FLAG : 0;

                if (!(i & 7))

                        printf("\n\t0x%02x, ", r);

                else

                        printf("0x%02x, ", r);

        }
        printf("\n\n};\n");
}
