/*****************************************************************************
*  Copyright 2006 - 2011 Broadcom Corporation.  All rights reserved.
*
*  Unless you and Broadcom execute a separate written software license
*  agreement governing use of this software, this software is licensed to you
*  under the terms of the GNU General Public License version 2, available at
*  http://www.gnu.org/copyleft/gpl.html (the "GPL").
*
*  Notwithstanding the above, under no circumstances may you combine this
*  software in any way with any other Broadcom software provided under a
*  license other than the GPL, without Broadcom's express prior written
*  consent.
*
*****************************************************************************/

/**
 * @file    chal_nand_uasm.h
 * @brief   CHAL NAND microcode assembly language definition.
 * @note
 **/

#ifndef __CHAL_NAND_UASM_H__
#define __CHAL_NAND_UASM_H__

/* microcode instructions */
#define instr_WC	(0x00)	/* put command 		   */
#define instr_WA	(0x04)	/* put address 		   */
#define instr_WD	(0x08)	/* put data 		      */
#define instr_RD	(0x0C)	/* sample/read data 	   */
#define instr_RS	(0x10)	/* read stsus           */
#define instr_WT	(0x20)	/* wait event/timeout   */

#define instr_ADD	 (0x40)
#define instr_SUB  (0x60)
#define instr_AND  (0x80)
#define instr_OR   (0xA0)
#define instr_SH   (0xC0)
#define instr_MV   (0xD0)
#define instr_BR   (0xE0)

#define instr_END  (0x1C)


/* instruction with 10 bits for operands */
#define mk_instr_10(cmd,op)	(0x10000 | ((cmd & 0xFC) << 8)|((op) & 0x3FF))
/* instruction with 13 bits for operands */
#define mk_instr_13(cmd,op)	(0x10000 | ((cmd & 0xE0) << 8)|((op) & 0x1FFF))

/* registers */
#define uR0	(0x0)
#define uR1	(0x1)
#define uR2	(0x2)
#define uR3	(0x3)
#define uR4	(0x4)
#define uR5	(0x5)
#define uR6	(0x6)
#define uR7	(0x7)
#define uR8	(0x8)
#define uR9	(0x9)
#define uRA	(0xA)
#define uRB	(0xB)
#define uRC	(0xC)
#define uRD	(0xD)
#define uRE	(0xE)
#define uRF	(0xF)

/* events */
#define uEVT_RB                  0
#define uEVT_DMA_START           1
#define uEVT_WR_DONE             2
#define uEVT_RD_DONE             3
#define uEVT_DMA_DONE            4
#define uEVT_TOUT                7

/* micro assembly instruction definition */
#define uA_END			            mk_instr_10(instr_END,0x3FF)

#define uA_WCI(opcode)		      mk_instr_10(instr_WC,(opcode & 0xFF))
#define uA_WAI(cycles)		      mk_instr_10(instr_WA,(cycles & 0xFF))
#define uA_WDI(bytes)	         mk_instr_10(instr_WD,(bytes & 0x1FF))
#define uA_RDI(bytes)	         mk_instr_10(instr_RD,(bytes & 0x1FF))

#define uA_WC(reg)		         mk_instr_10(instr_WC, (0x200 | (reg & 0xF)))
#define uA_WA(reg)		         mk_instr_10(instr_WA, (0x200 | (reg & 0xF)))
#define uA_WD(reg)		         mk_instr_10(instr_WD, (0x200 | (reg & 0xF)))
#define uA_RD(reg)		         mk_instr_10(instr_RD, (0x200 | (reg & 0xF)))

#define uA_RS			            mk_instr_10(instr_RS,0x1)

#define uA_ADDI(reg,val)         mk_instr_13(instr_ADD, ((reg & 0xF) << 9) | (val & 0xFF))
#define uA_SUBI(reg,val)         mk_instr_13(instr_SUB, ((reg & 0xF) << 9) | (val & 0xFF))
#define uA_ANDI(reg,val)         mk_instr_13(instr_AND, ((reg & 0xF) << 9) | (val & 0xFF))
#define uA_ORI(reg,val)          mk_instr_13(instr_OR, ((reg & 0xF) << 9) | (val & 0xFF))

#define uA_ADD(reg1,reg2)        mk_instr_13(instr_ADD, ((reg1 & 0xF) << 9) | 0x100 | ((reg2 & 0xF) << 4))
#define uA_SUB(reg1,reg2)        mk_instr_13(instr_SUB, ((reg1 & 0xF) << 9) | 0x100 | ((reg2 & 0xF) << 4))
#define uA_AND(reg1,reg2)        mk_instr_13(instr_AND, ((reg1 & 0xF) << 9) | 0x100 | ((reg2 & 0xF) << 4))
#define uA_OR(reg1,reg2)         mk_instr_13(instr_OR, ((reg1 & 0xF) << 9) | 0x100 | ((reg2 & 0xF) << 4))

#define uA_LSH(reg,offset)       mk_instr_10(instr_SH, ((reg & 0xF) << 5) | (offset & 0xF))
#define uA_RSH(reg,offset)       mk_instr_10(instr_SH, ((reg & 0xF) << 5) | 0x10 | (offset & 0xF))
#define uA_MV(reg1,reg2)         mk_instr_10(instr_MV, ((reg1 & 0xF) << 5) | (reg2 & 0xF))

#define uA_BRA(offset)           mk_instr_13(instr_BR, ((offset<0) ? (0x100 | (-(offset) & 0xFF)) : (offset & 0xFF))) 
#define uA_BRZ(offset)           mk_instr_13(instr_BR, ((0x1 << 10) | ((offset<0) ? (0x100 | (-(offset) & 0xFF)) : (offset & 0xFF)))) 
#define uA_BRC(offset)           mk_instr_13(instr_BR, ((0x2 << 10) | ((offset<0) ? (0x100 | (-(offset) & 0xFF)) : (offset & 0xFF))))

#define uA_WTS(evt,ticks)        mk_instr_13(instr_WT, ((evt & 0x7) << 10) | (ticks & 0x1FF))
#define uA_WTL(evt,ticks)        mk_instr_13(instr_WT, ((evt & 0x7) << 10) | 0x200 | (ticks & 0x1FF))

#endif

