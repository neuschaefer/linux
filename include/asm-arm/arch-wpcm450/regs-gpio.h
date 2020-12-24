/* linux/include/asm/hardware/wpcm450/regs-gpio.h
 *
 * Copyright (c) 2003,2004 Simtec Electronics <linux@simtec.co.uk>
 *		           http://www.simtec.co.uk/products/SWLINUX/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * WPCM450 GPIO register definitions
 *
 *  Changelog:
 *    19-06-2003     BJD     Created file
 *    23-06-2003     BJD     Updated GSTATUS registers
 *    12-03-2004     BJD     Updated include protection
 *    20-07-2004     BJD     Added GPIO pin numbers, added Port A definitions
 *    04-10-2004     BJD     Fixed number of bugs, added EXT IRQ filter defs
 *    17-10-2004     BJD     Added GSTATUS1 register definitions
 *    18-11-2004     BJD     Fixed definitions of GPE3, GPE4, GPE5 and GPE6
 *    18-11-2004     BJD     Added S3C2440 AC97 controls
 *    10-Mar-2005    LCVR    Changed WPCM450_VA to WPCM450_VA
 *    28-Mar-2005    LCVR    Fixed definition of GPB10
 *    26-Oct-2005    BJD     Added generic configuration types
 *    27-Nov-2005    LCVR    Added definitions to S3C2400 registers
 *    15-Jan-2006    LCVR    Written WPCM450_GPIO_BASE() macro
*/


#ifndef __ASM_ARCH_REGS_GPIO_H
#define __ASM_ARCH_REGS_GPIO_H "$Id$"

#define WPCM450_GPIONO(bank,offset) ((bank) + (offset))

#define WPCM450_GPIO_BANKA   (32*0)
#define WPCM450_GPIO_BANKB   (32*1)
#define WPCM450_GPIO_BANKC   (32*2)
#define WPCM450_GPIO_BANKD   (32*3)
#define WPCM450_GPIO_BANKE   (32*4)
#define WPCM450_GPIO_BANKF   (32*5)
#define WPCM450_GPIO_BANKG   (32*6)
#define WPCM450_GPIO_BANKH   (32*7)

#define WPCM4XX_GPIO_BASE(x)  WPCM450_GPIO_BASE(x)
#define WPCM4XX_MISCCR        WPCM450_MISCCR



/* S3C2400 doesn't have a 1:1 mapping to WPCM450 gpio base pins */

#define S3C2400_BANKNUM(pin)     (((pin) & ~31) / 32)
#define S3C2400_BASEA2B(pin)     ((((pin) & ~31) >> 2))
#define S3C2400_BASEC2H(pin)     ((S3C2400_BANKNUM(pin) * 10) + \
                                 (2 * (S3C2400_BANKNUM(pin)-2)))

#define S3C2400_GPIO_BASE(pin)   (pin < WPCM450_GPIO_BANKC ? \
                                 S3C2400_BASEA2B(pin)+WPCM450_VA_GPIO : \
                                 S3C2400_BASEC2H(pin)+WPCM450_VA_GPIO)


#define WPCM450_GPIO_BASE(pin)   ((((pin) & ~31) >> 1) + WPCM450_VA_GPIO)
#define WPCM450_GPIO_OFFSET(pin) ((pin) & 31)

/* general configuration options */

#define WPCM450_GPIO_LEAVE   (0xFFFFFFFF)
#define WPCM450_GPIO_INPUT   (0xFFFFFFF0)
#define WPCM450_GPIO_OUTPUT  (0xFFFFFFF1)
#define WPCM450_GPIO_IRQ     (0xFFFFFFF2)	/* not available for all */
#define WPCM450_GPIO_SFN2    (0xFFFFFFF2)	/* not available on A */
#define WPCM450_GPIO_SFN3    (0xFFFFFFF3)	/* not available on A */

/* configure GPIO ports A..G */

#define WPCM450_GPIOREG(x) ((x) + WPCM450_VA_GPIO)

/* port A - WPCM450: 22bits, zero in bit X makes pin X output
 *          S3C2400: 18bits, zero in bit X makes pin X output
 * 1 makes port special function, this is default
*/
#define WPCM450_GPACON	   WPCM450_GPIOREG(0x00)
#define WPCM450_GPADAT	   WPCM450_GPIOREG(0x04)

#define S3C2400_GPACON	   WPCM450_GPIOREG(0x00)
#define S3C2400_GPADAT	   WPCM450_GPIOREG(0x04)

#define WPCM450_GPA0         WPCM450_GPIONO(WPCM450_GPIO_BANKA, 0)
#define WPCM450_GPA0_OUT     (0<<0)
#define WPCM450_GPA0_ADDR0   (1<<0)

#define WPCM450_GPA1         WPCM450_GPIONO(WPCM450_GPIO_BANKA, 1)
#define WPCM450_GPA1_OUT     (0<<1)
#define WPCM450_GPA1_ADDR16  (1<<1)

#define WPCM450_GPA2         WPCM450_GPIONO(WPCM450_GPIO_BANKA, 2)
#define WPCM450_GPA2_OUT     (0<<2)
#define WPCM450_GPA2_ADDR17  (1<<2)

#define WPCM450_GPA3         WPCM450_GPIONO(WPCM450_GPIO_BANKA, 3)
#define WPCM450_GPA3_OUT     (0<<3)
#define WPCM450_GPA3_ADDR18  (1<<3)

#define WPCM450_GPA4         WPCM450_GPIONO(WPCM450_GPIO_BANKA, 4)
#define WPCM450_GPA4_OUT     (0<<4)
#define WPCM450_GPA4_ADDR19  (1<<4)

#define WPCM450_GPA5         WPCM450_GPIONO(WPCM450_GPIO_BANKA, 5)
#define WPCM450_GPA5_OUT     (0<<5)
#define WPCM450_GPA5_ADDR20  (1<<5)

#define WPCM450_GPA6         WPCM450_GPIONO(WPCM450_GPIO_BANKA, 6)
#define WPCM450_GPA6_OUT     (0<<6)
#define WPCM450_GPA6_ADDR21  (1<<6)

#define WPCM450_GPA7         WPCM450_GPIONO(WPCM450_GPIO_BANKA, 7)
#define WPCM450_GPA7_OUT     (0<<7)
#define WPCM450_GPA7_ADDR22  (1<<7)

#define WPCM450_GPA8         WPCM450_GPIONO(WPCM450_GPIO_BANKA, 8)
#define WPCM450_GPA8_OUT     (0<<8)
#define WPCM450_GPA8_ADDR23  (1<<8)

#define WPCM450_GPA9         WPCM450_GPIONO(WPCM450_GPIO_BANKA, 9)
#define WPCM450_GPA9_OUT     (0<<9)
#define WPCM450_GPA9_ADDR24  (1<<9)

#define WPCM450_GPA10        WPCM450_GPIONO(WPCM450_GPIO_BANKA, 10)
#define WPCM450_GPA10_OUT    (0<<10)
#define WPCM450_GPA10_ADDR25 (1<<10)
#define S3C2400_GPA10_SCKE   (1<<10)

#define WPCM450_GPA11        WPCM450_GPIONO(WPCM450_GPIO_BANKA, 11)
#define WPCM450_GPA11_OUT    (0<<11)
#define WPCM450_GPA11_ADDR26 (1<<11)
#define S3C2400_GPA11_nCAS0  (1<<11)

#define WPCM450_GPA12        WPCM450_GPIONO(WPCM450_GPIO_BANKA, 12)
#define WPCM450_GPA12_OUT    (0<<12)
#define WPCM450_GPA12_nGCS1  (1<<12)
#define S3C2400_GPA12_nCAS1  (1<<12)

#define WPCM450_GPA13        WPCM450_GPIONO(WPCM450_GPIO_BANKA, 13)
#define WPCM450_GPA13_OUT    (0<<13)
#define WPCM450_GPA13_nGCS2  (1<<13)
#define S3C2400_GPA13_nGCS1  (1<<13)

#define WPCM450_GPA14        WPCM450_GPIONO(WPCM450_GPIO_BANKA, 14)
#define WPCM450_GPA14_OUT    (0<<14)
#define WPCM450_GPA14_nGCS3  (1<<14)
#define S3C2400_GPA14_nGCS2  (1<<14)

#define WPCM450_GPA15        WPCM450_GPIONO(WPCM450_GPIO_BANKA, 15)
#define WPCM450_GPA15_OUT    (0<<15)
#define WPCM450_GPA15_nGCS4  (1<<15)
#define S3C2400_GPA15_nGCS3  (1<<15)

#define WPCM450_GPA16        WPCM450_GPIONO(WPCM450_GPIO_BANKA, 16)
#define WPCM450_GPA16_OUT    (0<<16)
#define WPCM450_GPA16_nGCS5  (1<<16)
#define S3C2400_GPA16_nGCS4  (1<<16)

#define WPCM450_GPA17        WPCM450_GPIONO(WPCM450_GPIO_BANKA, 17)
#define WPCM450_GPA17_OUT    (0<<17)
#define WPCM450_GPA17_CLE    (1<<17)
#define S3C2400_GPA17_nGCS5  (1<<17)

#define WPCM450_GPA18        WPCM450_GPIONO(WPCM450_GPIO_BANKA, 18)
#define WPCM450_GPA18_OUT    (0<<18)
#define WPCM450_GPA18_ALE    (1<<18)

#define WPCM450_GPA19        WPCM450_GPIONO(WPCM450_GPIO_BANKA, 19)
#define WPCM450_GPA19_OUT    (0<<19)
#define WPCM450_GPA19_nFWE   (1<<19)

#define WPCM450_GPA20        WPCM450_GPIONO(WPCM450_GPIO_BANKA, 20)
#define WPCM450_GPA20_OUT    (0<<20)
#define WPCM450_GPA20_nFRE   (1<<20)

#define WPCM450_GPA21        WPCM450_GPIONO(WPCM450_GPIO_BANKA, 21)
#define WPCM450_GPA21_OUT    (0<<21)
#define WPCM450_GPA21_nRSTOUT (1<<21)

#define WPCM450_GPA22        WPCM450_GPIONO(WPCM450_GPIO_BANKA, 22)
#define WPCM450_GPA22_OUT    (0<<22)
#define WPCM450_GPA22_nFCE   (1<<22)

/* 0x08 and 0x0c are reserved on WPCM450 */

/* WPCM450:
 * GPB is 10 IO pins, each configured by 2 bits each in GPBCON.
 *   00 = input, 01 = output, 10=special function, 11=reserved

 * S3C2400:
 * GPB is 16 IO pins, each configured by 2 bits each in GPBCON.
 *   00 = input, 01 = output, 10=data, 11=special function

 * bit 0,1 = pin 0, 2,3= pin 1...
 *
 * CPBUP = pull up resistor control, 1=disabled, 0=enabled
*/

#define WPCM450_GPBCON	   WPCM450_GPIOREG(0x10)
#define WPCM450_GPBDAT	   WPCM450_GPIOREG(0x14)
#define WPCM450_GPBUP	   WPCM450_GPIOREG(0x18)

#define S3C2400_GPBCON	   WPCM450_GPIOREG(0x08)
#define S3C2400_GPBDAT	   WPCM450_GPIOREG(0x0C)
#define S3C2400_GPBUP	   WPCM450_GPIOREG(0x10)

/* no i/o pin in port b can have value 3! */

#define WPCM450_GPB0         WPCM450_GPIONO(WPCM450_GPIO_BANKB, 0)
#define WPCM450_GPB0_INP     (0x00 << 0)
#define WPCM450_GPB0_OUTP    (0x01 << 0)
#define WPCM450_GPB0_TOUT0   (0x02 << 0)
#define S3C2400_GPB0_DATA16  (0x02 << 0)

#define WPCM450_GPB1         WPCM450_GPIONO(WPCM450_GPIO_BANKB, 1)
#define WPCM450_GPB1_INP     (0x00 << 2)
#define WPCM450_GPB1_OUTP    (0x01 << 2)
#define WPCM450_GPB1_TOUT1   (0x02 << 2)
#define S3C2400_GPB1_DATA17  (0x02 << 2)

#define WPCM450_GPB2         WPCM450_GPIONO(WPCM450_GPIO_BANKB, 2)
#define WPCM450_GPB2_INP     (0x00 << 4)
#define WPCM450_GPB2_OUTP    (0x01 << 4)
#define WPCM450_GPB2_TOUT2   (0x02 << 4)
#define S3C2400_GPB2_DATA18  (0x02 << 4)
#define S3C2400_GPB2_TCLK1   (0x03 << 4)

#define WPCM450_GPB3         WPCM450_GPIONO(WPCM450_GPIO_BANKB, 3)
#define WPCM450_GPB3_INP     (0x00 << 6)
#define WPCM450_GPB3_OUTP    (0x01 << 6)
#define WPCM450_GPB3_TOUT3   (0x02 << 6)
#define S3C2400_GPB3_DATA19  (0x02 << 6)
#define S3C2400_GPB3_TXD1    (0x03 << 6)

#define WPCM450_GPB4         WPCM450_GPIONO(WPCM450_GPIO_BANKB, 4)
#define WPCM450_GPB4_INP     (0x00 << 8)
#define WPCM450_GPB4_OUTP    (0x01 << 8)
#define WPCM450_GPB4_TCLK0   (0x02 << 8)
#define S3C2400_GPB4_DATA20  (0x02 << 8)
#define WPCM450_GPB4_MASK    (0x03 << 8)
#define S3C2400_GPB4_RXD1    (0x03 << 8)
#define S3C2400_GPB4_MASK    (0x03 << 8)

#define WPCM450_GPB5         WPCM450_GPIONO(WPCM450_GPIO_BANKB, 5)
#define WPCM450_GPB5_INP     (0x00 << 10)
#define WPCM450_GPB5_OUTP    (0x01 << 10)
#define WPCM450_GPB5_nXBACK  (0x02 << 10)
#define S3C2400_GPB5_DATA21  (0x02 << 10)
#define S3C2400_GPB5_nCTS1   (0x03 << 10)

#define WPCM450_GPB6         WPCM450_GPIONO(WPCM450_GPIO_BANKB, 6)
#define WPCM450_GPB6_INP     (0x00 << 12)
#define WPCM450_GPB6_OUTP    (0x01 << 12)
#define WPCM450_GPB6_nXBREQ  (0x02 << 12)
#define S3C2400_GPB6_DATA22  (0x02 << 12)
#define S3C2400_GPB6_nRTS1   (0x03 << 12)

#define WPCM450_GPB7         WPCM450_GPIONO(WPCM450_GPIO_BANKB, 7)
#define WPCM450_GPB7_INP     (0x00 << 14)
#define WPCM450_GPB7_OUTP    (0x01 << 14)
#define WPCM450_GPB7_nXDACK1 (0x02 << 14)
#define S3C2400_GPB7_DATA23  (0x02 << 14)

#define WPCM450_GPB8         WPCM450_GPIONO(WPCM450_GPIO_BANKB, 8)
#define WPCM450_GPB8_INP     (0x00 << 16)
#define WPCM450_GPB8_OUTP    (0x01 << 16)
#define WPCM450_GPB8_nXDREQ1 (0x02 << 16)
#define S3C2400_GPB8_DATA24  (0x02 << 16)

#define WPCM450_GPB9         WPCM450_GPIONO(WPCM450_GPIO_BANKB, 9)
#define WPCM450_GPB9_INP     (0x00 << 18)
#define WPCM450_GPB9_OUTP    (0x01 << 18)
#define WPCM450_GPB9_nXDACK0 (0x02 << 18)
#define S3C2400_GPB9_DATA25  (0x02 << 18)
#define S3C2400_GPB9_I2SSDI  (0x03 << 18)

#define WPCM450_GPB10        WPCM450_GPIONO(WPCM450_GPIO_BANKB, 10)
#define WPCM450_GPB10_INP    (0x00 << 20)
#define WPCM450_GPB10_OUTP   (0x01 << 20)
#define WPCM450_GPB10_nXDRE0 (0x02 << 20)
#define S3C2400_GPB10_DATA26 (0x02 << 20)
#define S3C2400_GPB10_nSS    (0x03 << 20)

#define S3C2400_GPB11        WPCM450_GPIONO(WPCM450_GPIO_BANKB, 11)
#define S3C2400_GPB11_INP    (0x00 << 22)
#define S3C2400_GPB11_OUTP   (0x01 << 22)
#define S3C2400_GPB11_DATA27 (0x02 << 22)

#define S3C2400_GPB12        WPCM450_GPIONO(WPCM450_GPIO_BANKB, 12)
#define S3C2400_GPB12_INP    (0x00 << 24)
#define S3C2400_GPB12_OUTP   (0x01 << 24)
#define S3C2400_GPB12_DATA28 (0x02 << 24)

#define S3C2400_GPB13        WPCM450_GPIONO(WPCM450_GPIO_BANKB, 13)
#define S3C2400_GPB13_INP    (0x00 << 26)
#define S3C2400_GPB13_OUTP   (0x01 << 26)
#define S3C2400_GPB13_DATA29 (0x02 << 26)

#define S3C2400_GPB14        WPCM450_GPIONO(WPCM450_GPIO_BANKB, 14)
#define S3C2400_GPB14_INP    (0x00 << 28)
#define S3C2400_GPB14_OUTP   (0x01 << 28)
#define S3C2400_GPB14_DATA30 (0x02 << 28)

#define S3C2400_GPB15        WPCM450_GPIONO(WPCM450_GPIO_BANKB, 15)
#define S3C2400_GPB15_INP    (0x00 << 30)
#define S3C2400_GPB15_OUTP   (0x01 << 30)
#define S3C2400_GPB15_DATA31 (0x02 << 30)

#define WPCM450_GPB_PUPDIS(x)  (1<<(x))

/* Port C consits of 16 GPIO/Special function
 *
 * almost identical setup to port b, but the special functions are mostly
 * to do with the video system's sync/etc.
*/

#define WPCM450_GPCCON	   WPCM450_GPIOREG(0x20)
#define WPCM450_GPCDAT	   WPCM450_GPIOREG(0x24)
#define WPCM450_GPCUP	   WPCM450_GPIOREG(0x28)

#define S3C2400_GPCCON	   WPCM450_GPIOREG(0x14)
#define S3C2400_GPCDAT	   WPCM450_GPIOREG(0x18)
#define S3C2400_GPCUP	   WPCM450_GPIOREG(0x1C)

#define WPCM450_GPC0            WPCM450_GPIONO(WPCM450_GPIO_BANKC, 0)
#define WPCM450_GPC0_INP	(0x00 << 0)
#define WPCM450_GPC0_OUTP	(0x01 << 0)
#define WPCM450_GPC0_LEND	(0x02 << 0)
#define S3C2400_GPC0_VD0 	(0x02 << 0)

#define WPCM450_GPC1            WPCM450_GPIONO(WPCM450_GPIO_BANKC, 1)
#define WPCM450_GPC1_INP	(0x00 << 2)
#define WPCM450_GPC1_OUTP	(0x01 << 2)
#define WPCM450_GPC1_VCLK	(0x02 << 2)
#define S3C2400_GPC1_VD1 	(0x02 << 2)

#define WPCM450_GPC2            WPCM450_GPIONO(WPCM450_GPIO_BANKC, 2)
#define WPCM450_GPC2_INP	(0x00 << 4)
#define WPCM450_GPC2_OUTP	(0x01 << 4)
#define WPCM450_GPC2_VLINE	(0x02 << 4)
#define S3C2400_GPC2_VD2  	(0x02 << 4)

#define WPCM450_GPC3            WPCM450_GPIONO(WPCM450_GPIO_BANKC, 3)
#define WPCM450_GPC3_INP	(0x00 << 6)
#define WPCM450_GPC3_OUTP	(0x01 << 6)
#define WPCM450_GPC3_VFRAME	(0x02 << 6)
#define S3C2400_GPC3_VD3   	(0x02 << 6)

#define WPCM450_GPC4            WPCM450_GPIONO(WPCM450_GPIO_BANKC, 4)
#define WPCM450_GPC4_INP	(0x00 << 8)
#define WPCM450_GPC4_OUTP	(0x01 << 8)
#define WPCM450_GPC4_VM		(0x02 << 8)
#define S3C2400_GPC4_VD4	(0x02 << 8)

#define WPCM450_GPC5            WPCM450_GPIONO(WPCM450_GPIO_BANKC, 5)
#define WPCM450_GPC5_INP	(0x00 << 10)
#define WPCM450_GPC5_OUTP	(0x01 << 10)
#define WPCM450_GPC5_LCDVF0	(0x02 << 10)
#define S3C2400_GPC5_VD5   	(0x02 << 10)

#define WPCM450_GPC6            WPCM450_GPIONO(WPCM450_GPIO_BANKC, 6)
#define WPCM450_GPC6_INP	(0x00 << 12)
#define WPCM450_GPC6_OUTP	(0x01 << 12)
#define WPCM450_GPC6_LCDVF1	(0x02 << 12)
#define S3C2400_GPC6_VD6   	(0x02 << 12)

#define WPCM450_GPC7            WPCM450_GPIONO(WPCM450_GPIO_BANKC, 7)
#define WPCM450_GPC7_INP	(0x00 << 14)
#define WPCM450_GPC7_OUTP	(0x01 << 14)
#define WPCM450_GPC7_LCDVF2	(0x02 << 14)
#define S3C2400_GPC7_VD7   	(0x02 << 14)

#define WPCM450_GPC8            WPCM450_GPIONO(WPCM450_GPIO_BANKC, 8)
#define WPCM450_GPC8_INP	(0x00 << 16)
#define WPCM450_GPC8_OUTP	(0x01 << 16)
#define WPCM450_GPC8_VD0	(0x02 << 16)
#define S3C2400_GPC8_VD8	(0x02 << 16)

#define WPCM450_GPC9            WPCM450_GPIONO(WPCM450_GPIO_BANKC, 9)
#define WPCM450_GPC9_INP	(0x00 << 18)
#define WPCM450_GPC9_OUTP	(0x01 << 18)
#define WPCM450_GPC9_VD1	(0x02 << 18)
#define S3C2400_GPC9_VD9	(0x02 << 18)

#define WPCM450_GPC10           WPCM450_GPIONO(WPCM450_GPIO_BANKC, 10)
#define WPCM450_GPC10_INP	(0x00 << 20)
#define WPCM450_GPC10_OUTP	(0x01 << 20)
#define WPCM450_GPC10_VD2	(0x02 << 20)
#define S3C2400_GPC10_VD10	(0x02 << 20)

#define WPCM450_GPC11           WPCM450_GPIONO(WPCM450_GPIO_BANKC, 11)
#define WPCM450_GPC11_INP	(0x00 << 22)
#define WPCM450_GPC11_OUTP	(0x01 << 22)
#define WPCM450_GPC11_VD3	(0x02 << 22)
#define S3C2400_GPC11_VD11	(0x02 << 22)

#define WPCM450_GPC12           WPCM450_GPIONO(WPCM450_GPIO_BANKC, 12)
#define WPCM450_GPC12_INP	(0x00 << 24)
#define WPCM450_GPC12_OUTP	(0x01 << 24)
#define WPCM450_GPC12_VD4	(0x02 << 24)
#define S3C2400_GPC12_VD12	(0x02 << 24)

#define WPCM450_GPC13           WPCM450_GPIONO(WPCM450_GPIO_BANKC, 13)
#define WPCM450_GPC13_INP	(0x00 << 26)
#define WPCM450_GPC13_OUTP	(0x01 << 26)
#define WPCM450_GPC13_VD5	(0x02 << 26)
#define S3C2400_GPC13_VD13	(0x02 << 26)

#define WPCM450_GPC14           WPCM450_GPIONO(WPCM450_GPIO_BANKC, 14)
#define WPCM450_GPC14_INP	(0x00 << 28)
#define WPCM450_GPC14_OUTP	(0x01 << 28)
#define WPCM450_GPC14_VD6	(0x02 << 28)
#define S3C2400_GPC14_VD14	(0x02 << 28)

#define WPCM450_GPC15           WPCM450_GPIONO(WPCM450_GPIO_BANKC, 15)
#define WPCM450_GPC15_INP	(0x00 << 30)
#define WPCM450_GPC15_OUTP	(0x01 << 30)
#define WPCM450_GPC15_VD7	(0x02 << 30)
#define S3C2400_GPC15_VD15	(0x02 << 30)

#define WPCM450_GPC_PUPDIS(x)  (1<<(x))

/*
 * WPCM450: Port D consists of 16 GPIO/Special function
 *
 * almost identical setup to port b, but the special functions are mostly
 * to do with the video system's data.
 *
 * S3C2400: Port D consists of 11 GPIO/Special function
 *
 * almost identical setup to port c
*/

#define WPCM450_GPDCON	   WPCM450_GPIOREG(0x30)
#define WPCM450_GPDDAT	   WPCM450_GPIOREG(0x34)
#define WPCM450_GPDUP	   WPCM450_GPIOREG(0x38)

#define S3C2400_GPDCON	   WPCM450_GPIOREG(0x20)
#define S3C2400_GPDDAT	   WPCM450_GPIOREG(0x24)
#define S3C2400_GPDUP	   WPCM450_GPIOREG(0x28)

#define WPCM450_GPD0            WPCM450_GPIONO(WPCM450_GPIO_BANKD, 0)
#define WPCM450_GPD0_INP	(0x00 << 0)
#define WPCM450_GPD0_OUTP	(0x01 << 0)
#define WPCM450_GPD0_VD8	(0x02 << 0)
#define S3C2400_GPD0_VFRAME	(0x02 << 0)

#define WPCM450_GPD1            WPCM450_GPIONO(WPCM450_GPIO_BANKD, 1)
#define WPCM450_GPD1_INP	(0x00 << 2)
#define WPCM450_GPD1_OUTP	(0x01 << 2)
#define WPCM450_GPD1_VD9	(0x02 << 2)
#define S3C2400_GPD1_VM		(0x02 << 2)

#define WPCM450_GPD2            WPCM450_GPIONO(WPCM450_GPIO_BANKD, 2)
#define WPCM450_GPD2_INP	(0x00 << 4)
#define WPCM450_GPD2_OUTP	(0x01 << 4)
#define WPCM450_GPD2_VD10	(0x02 << 4)
#define S3C2400_GPD2_VLINE	(0x02 << 4)

#define WPCM450_GPD3            WPCM450_GPIONO(WPCM450_GPIO_BANKD, 3)
#define WPCM450_GPD3_INP	(0x00 << 6)
#define WPCM450_GPD3_OUTP	(0x01 << 6)
#define WPCM450_GPD3_VD11	(0x02 << 6)
#define S3C2400_GPD3_VCLK	(0x02 << 6)

#define WPCM450_GPD4            WPCM450_GPIONO(WPCM450_GPIO_BANKD, 4)
#define WPCM450_GPD4_INP	(0x00 << 8)
#define WPCM450_GPD4_OUTP	(0x01 << 8)
#define WPCM450_GPD4_VD12	(0x02 << 8)
#define S3C2400_GPD4_LEND	(0x02 << 8)

#define WPCM450_GPD5            WPCM450_GPIONO(WPCM450_GPIO_BANKD, 5)
#define WPCM450_GPD5_INP	(0x00 << 10)
#define WPCM450_GPD5_OUTP	(0x01 << 10)
#define WPCM450_GPD5_VD13	(0x02 << 10)
#define S3C2400_GPD5_TOUT0	(0x02 << 10)

#define WPCM450_GPD6            WPCM450_GPIONO(WPCM450_GPIO_BANKD, 6)
#define WPCM450_GPD6_INP	(0x00 << 12)
#define WPCM450_GPD6_OUTP	(0x01 << 12)
#define WPCM450_GPD6_VD14	(0x02 << 12)
#define S3C2400_GPD6_TOUT1	(0x02 << 12)

#define WPCM450_GPD7            WPCM450_GPIONO(WPCM450_GPIO_BANKD, 7)
#define WPCM450_GPD7_INP	(0x00 << 14)
#define WPCM450_GPD7_OUTP	(0x01 << 14)
#define WPCM450_GPD7_VD15	(0x02 << 14)
#define S3C2400_GPD7_TOUT2	(0x02 << 14)

#define WPCM450_GPD8            WPCM450_GPIONO(WPCM450_GPIO_BANKD, 8)
#define WPCM450_GPD8_INP	(0x00 << 16)
#define WPCM450_GPD8_OUTP	(0x01 << 16)
#define WPCM450_GPD8_VD16	(0x02 << 16)
#define S3C2400_GPD8_TOUT3	(0x02 << 16)

#define WPCM450_GPD9            WPCM450_GPIONO(WPCM450_GPIO_BANKD, 9)
#define WPCM450_GPD9_INP	(0x00 << 18)
#define WPCM450_GPD9_OUTP	(0x01 << 18)
#define WPCM450_GPD9_VD17	(0x02 << 18)
#define S3C2400_GPD9_TCLK0	(0x02 << 18)
#define WPCM450_GPD9_MASK       (0x03 << 18)

#define WPCM450_GPD10           WPCM450_GPIONO(WPCM450_GPIO_BANKD, 10)
#define WPCM450_GPD10_INP	(0x00 << 20)
#define WPCM450_GPD10_OUTP	(0x01 << 20)
#define WPCM450_GPD10_VD18	(0x02 << 20)
#define S3C2400_GPD10_nWAIT	(0x02 << 20)

#define WPCM450_GPD11           WPCM450_GPIONO(WPCM450_GPIO_BANKD, 11)
#define WPCM450_GPD11_INP	(0x00 << 22)
#define WPCM450_GPD11_OUTP	(0x01 << 22)
#define WPCM450_GPD11_VD19	(0x02 << 22)

#define WPCM450_GPD12           WPCM450_GPIONO(WPCM450_GPIO_BANKD, 12)
#define WPCM450_GPD12_INP	(0x00 << 24)
#define WPCM450_GPD12_OUTP	(0x01 << 24)
#define WPCM450_GPD12_VD20	(0x02 << 24)

#define WPCM450_GPD13           WPCM450_GPIONO(WPCM450_GPIO_BANKD, 13)
#define WPCM450_GPD13_INP	(0x00 << 26)
#define WPCM450_GPD13_OUTP	(0x01 << 26)
#define WPCM450_GPD13_VD21	(0x02 << 26)

#define WPCM450_GPD14           WPCM450_GPIONO(WPCM450_GPIO_BANKD, 14)
#define WPCM450_GPD14_INP	(0x00 << 28)
#define WPCM450_GPD14_OUTP	(0x01 << 28)
#define WPCM450_GPD14_VD22	(0x02 << 28)

#define WPCM450_GPD15           WPCM450_GPIONO(WPCM450_GPIO_BANKD, 15)
#define WPCM450_GPD15_INP	(0x00 << 30)
#define WPCM450_GPD15_OUTP	(0x01 << 30)
#define WPCM450_GPD15_VD23	(0x02 << 30)

#define WPCM450_GPD_PUPDIS(x)  (1<<(x))

/* WPCM450:
 * Port E consists of 16 GPIO/Special function
 *
 * again, the same as port B, but dealing with I2S, SDI, and
 * more miscellaneous functions
 *
 * S3C2400:
 * Port E consists of 12 GPIO/Special function
 *
 * GPIO / interrupt inputs
*/

#define WPCM450_GPECON	   WPCM450_GPIOREG(0x40)
#define WPCM450_GPEDAT	   WPCM450_GPIOREG(0x44)
#define WPCM450_GPEUP	   WPCM450_GPIOREG(0x48)

#define S3C2400_GPECON	   WPCM450_GPIOREG(0x2C)
#define S3C2400_GPEDAT	   WPCM450_GPIOREG(0x30)
#define S3C2400_GPEUP	   WPCM450_GPIOREG(0x34)

#define WPCM450_GPE0           WPCM450_GPIONO(WPCM450_GPIO_BANKE, 0)
#define WPCM450_GPE0_INP       (0x00 << 0)
#define WPCM450_GPE0_OUTP      (0x01 << 0)
#define WPCM450_GPE0_I2SLRCK   (0x02 << 0)
#define S3C2400_GPE0_EINT0     (0x02 << 0)
#define WPCM450_GPE0_MASK      (0x03 << 0)

#define WPCM450_GPE1           WPCM450_GPIONO(WPCM450_GPIO_BANKE, 1)
#define WPCM450_GPE1_INP       (0x00 << 2)
#define WPCM450_GPE1_OUTP      (0x01 << 2)
#define WPCM450_GPE1_I2SSCLK   (0x02 << 2)
#define S3C2400_GPE1_EINT1     (0x02 << 2)
#define S3C2400_GPE1_nSS       (0x03 << 2)
#define WPCM450_GPE1_MASK      (0x03 << 2)

#define WPCM450_GPE2           WPCM450_GPIONO(WPCM450_GPIO_BANKE, 2)
#define WPCM450_GPE2_INP       (0x00 << 4)
#define WPCM450_GPE2_OUTP      (0x01 << 4)
#define WPCM450_GPE2_CDCLK     (0x02 << 4)
#define S3C2400_GPE2_EINT2     (0x02 << 4)
#define S3C2400_GPE2_I2SSDI    (0x03 << 4)

#define WPCM450_GPE3           WPCM450_GPIONO(WPCM450_GPIO_BANKE, 3)
#define WPCM450_GPE3_INP       (0x00 << 6)
#define WPCM450_GPE3_OUTP      (0x01 << 6)
#define WPCM450_GPE3_I2SSDI    (0x02 << 6)
#define S3C2400_GPE3_EINT3     (0x02 << 6)
#define S3C2400_GPE3_nCTS1     (0x03 << 6)
#define WPCM450_GPE3_nSS0      (0x03 << 6)
#define WPCM450_GPE3_MASK      (0x03 << 6)

#define WPCM450_GPE4           WPCM450_GPIONO(WPCM450_GPIO_BANKE, 4)
#define WPCM450_GPE4_INP       (0x00 << 8)
#define WPCM450_GPE4_OUTP      (0x01 << 8)
#define WPCM450_GPE4_I2SSDO    (0x02 << 8)
#define S3C2400_GPE4_EINT4     (0x02 << 8)
#define S3C2400_GPE4_nRTS1     (0x03 << 8)
#define WPCM450_GPE4_I2SSDI    (0x03 << 8)
#define WPCM450_GPE4_MASK      (0x03 << 8)

#define WPCM450_GPE5           WPCM450_GPIONO(WPCM450_GPIO_BANKE, 5)
#define WPCM450_GPE5_INP       (0x00 << 10)
#define WPCM450_GPE5_OUTP      (0x01 << 10)
#define WPCM450_GPE5_SDCLK     (0x02 << 10)
#define S3C2400_GPE5_EINT5     (0x02 << 10)
#define S3C2400_GPE5_TCLK1     (0x03 << 10)

#define WPCM450_GPE6           WPCM450_GPIONO(WPCM450_GPIO_BANKE, 6)
#define WPCM450_GPE6_INP       (0x00 << 12)
#define WPCM450_GPE6_OUTP      (0x01 << 12)
#define WPCM450_GPE6_SDCMD     (0x02 << 12)
#define S3C2400_GPE6_EINT6     (0x02 << 12)

#define WPCM450_GPE7           WPCM450_GPIONO(WPCM450_GPIO_BANKE, 7)
#define WPCM450_GPE7_INP       (0x00 << 14)
#define WPCM450_GPE7_OUTP      (0x01 << 14)
#define WPCM450_GPE7_SDDAT0    (0x02 << 14)
#define S3C2400_GPE7_EINT7     (0x02 << 14)

#define WPCM450_GPE8           WPCM450_GPIONO(WPCM450_GPIO_BANKE, 8)
#define WPCM450_GPE8_INP       (0x00 << 16)
#define WPCM450_GPE8_OUTP      (0x01 << 16)
#define WPCM450_GPE8_SDDAT1    (0x02 << 16)
#define S3C2400_GPE8_nXDACK0   (0x02 << 16)

#define WPCM450_GPE9           WPCM450_GPIONO(WPCM450_GPIO_BANKE, 9)
#define WPCM450_GPE9_INP       (0x00 << 18)
#define WPCM450_GPE9_OUTP      (0x01 << 18)
#define WPCM450_GPE9_SDDAT2    (0x02 << 18)
#define S3C2400_GPE9_nXDACK1   (0x02 << 18)
#define S3C2400_GPE9_nXBACK    (0x03 << 18)

#define WPCM450_GPE10          WPCM450_GPIONO(WPCM450_GPIO_BANKE, 10)
#define WPCM450_GPE10_INP      (0x00 << 20)
#define WPCM450_GPE10_OUTP     (0x01 << 20)
#define WPCM450_GPE10_SDDAT3   (0x02 << 20)
#define S3C2400_GPE10_nXDREQ0  (0x02 << 20)

#define WPCM450_GPE11          WPCM450_GPIONO(WPCM450_GPIO_BANKE, 11)
#define WPCM450_GPE11_INP      (0x00 << 22)
#define WPCM450_GPE11_OUTP     (0x01 << 22)
#define WPCM450_GPE11_SPIMISO0 (0x02 << 22)
#define S3C2400_GPE11_nXDREQ1  (0x02 << 22)
#define S3C2400_GPE11_nXBREQ   (0x03 << 22)

#define WPCM450_GPE12          WPCM450_GPIONO(WPCM450_GPIO_BANKE, 12)
#define WPCM450_GPE12_INP      (0x00 << 24)
#define WPCM450_GPE12_OUTP     (0x01 << 24)
#define WPCM450_GPE12_SPIMOSI0 (0x02 << 24)

#define WPCM450_GPE13          WPCM450_GPIONO(WPCM450_GPIO_BANKE, 13)
#define WPCM450_GPE13_INP      (0x00 << 26)
#define WPCM450_GPE13_OUTP     (0x01 << 26)
#define WPCM450_GPE13_SPICLK0  (0x02 << 26)

#define WPCM450_GPE14          WPCM450_GPIONO(WPCM450_GPIO_BANKE, 14)
#define WPCM450_GPE14_INP      (0x00 << 28)
#define WPCM450_GPE14_OUTP     (0x01 << 28)
#define WPCM450_GPE14_IICSCL   (0x02 << 28)
#define WPCM450_GPE14_MASK     (0x03 << 28)

#define WPCM450_GPE15          WPCM450_GPIONO(WPCM450_GPIO_BANKE, 15)
#define WPCM450_GPE15_INP      (0x00 << 30)
#define WPCM450_GPE15_OUTP     (0x01 << 30)
#define WPCM450_GPE15_IICSDA   (0x02 << 30)
#define WPCM450_GPE15_MASK     (0x03 << 30)

#define S3C2440_GPE0_ACSYNC    (0x03 << 0)
#define S3C2440_GPE1_ACBITCLK  (0x03 << 2)
#define S3C2440_GPE2_ACRESET   (0x03 << 4)
#define S3C2440_GPE3_ACIN      (0x03 << 6)
#define S3C2440_GPE4_ACOUT     (0x03 << 8)

#define WPCM450_GPE_PUPDIS(x)  (1<<(x))

/* WPCM450:
 * Port F consists of 8 GPIO/Special function
 *
 * GPIO / interrupt inputs
 *
 * GPFCON has 2 bits for each of the input pins on port F
 *   00 = 0 input, 1 output, 2 interrupt (EINT0..7), 3 undefined
 *
 * pull up works like all other ports.
 *
 * S3C2400:
 * Port F consists of 7 GPIO/Special function
 *
 * GPIO/serial/misc pins
*/

#define WPCM450_GPFCON	   WPCM450_GPIOREG(0x50)
#define WPCM450_GPFDAT	   WPCM450_GPIOREG(0x54)
#define WPCM450_GPFUP	   WPCM450_GPIOREG(0x58)

#define S3C2400_GPFCON	   WPCM450_GPIOREG(0x38)
#define S3C2400_GPFDAT	   WPCM450_GPIOREG(0x3C)
#define S3C2400_GPFUP	   WPCM450_GPIOREG(0x40)

#define WPCM450_GPF0        WPCM450_GPIONO(WPCM450_GPIO_BANKF, 0)
#define WPCM450_GPF0_INP    (0x00 << 0)
#define WPCM450_GPF0_OUTP   (0x01 << 0)
#define WPCM450_GPF0_EINT0  (0x02 << 0)
#define S3C2400_GPF0_RXD0   (0x02 << 0)

#define WPCM450_GPF1        WPCM450_GPIONO(WPCM450_GPIO_BANKF, 1)
#define WPCM450_GPF1_INP    (0x00 << 2)
#define WPCM450_GPF1_OUTP   (0x01 << 2)
#define WPCM450_GPF1_EINT1  (0x02 << 2)
#define S3C2400_GPF1_RXD1   (0x02 << 2)
#define S3C2400_GPF1_IICSDA (0x03 << 2)

#define WPCM450_GPF2        WPCM450_GPIONO(WPCM450_GPIO_BANKF, 2)
#define WPCM450_GPF2_INP    (0x00 << 4)
#define WPCM450_GPF2_OUTP   (0x01 << 4)
#define WPCM450_GPF2_EINT2  (0x02 << 4)
#define S3C2400_GPF2_TXD0   (0x02 << 4)

#define WPCM450_GPF3        WPCM450_GPIONO(WPCM450_GPIO_BANKF, 3)
#define WPCM450_GPF3_INP    (0x00 << 6)
#define WPCM450_GPF3_OUTP   (0x01 << 6)
#define WPCM450_GPF3_EINT3  (0x02 << 6)
#define S3C2400_GPF3_TXD1   (0x02 << 6)
#define S3C2400_GPF3_IICSCL (0x03 << 6)

#define WPCM450_GPF4        WPCM450_GPIONO(WPCM450_GPIO_BANKF, 4)
#define WPCM450_GPF4_INP    (0x00 << 8)
#define WPCM450_GPF4_OUTP   (0x01 << 8)
#define WPCM450_GPF4_EINT4  (0x02 << 8)
#define S3C2400_GPF4_nRTS0  (0x02 << 8)
#define S3C2400_GPF4_nXBACK (0x03 << 8)

#define WPCM450_GPF5        WPCM450_GPIONO(WPCM450_GPIO_BANKF, 5)
#define WPCM450_GPF5_INP    (0x00 << 10)
#define WPCM450_GPF5_OUTP   (0x01 << 10)
#define WPCM450_GPF5_EINT5  (0x02 << 10)
#define S3C2400_GPF5_nCTS0  (0x02 << 10)
#define S3C2400_GPF5_nXBREQ (0x03 << 10)

#define WPCM450_GPF6        WPCM450_GPIONO(WPCM450_GPIO_BANKF, 6)
#define WPCM450_GPF6_INP    (0x00 << 12)
#define WPCM450_GPF6_OUTP   (0x01 << 12)
#define WPCM450_GPF6_EINT6  (0x02 << 12)
#define S3C2400_GPF6_CLKOUT (0x02 << 12)

#define WPCM450_GPF7        WPCM450_GPIONO(WPCM450_GPIO_BANKF, 7)
#define WPCM450_GPF7_INP    (0x00 << 14)
#define WPCM450_GPF7_OUTP   (0x01 << 14)
#define WPCM450_GPF7_EINT7  (0x02 << 14)

#define WPCM450_GPF_PUPDIS(x)  (1<<(x))

/* WPCM450:
 * Port G consists of 8 GPIO/IRQ/Special function
 *
 * GPGCON has 2 bits for each of the input pins on port F
 *   00 = 0 input, 1 output, 2 interrupt (EINT0..7), 3 special func
 *
 * pull up works like all other ports.
 *
 * S3C2400:
 * Port G consists of 10 GPIO/Special function
*/

#define WPCM450_GPGCON	   WPCM450_GPIOREG(0x60)
#define WPCM450_GPGDAT	   WPCM450_GPIOREG(0x64)
#define WPCM450_GPGUP	   WPCM450_GPIOREG(0x68)

#define S3C2400_GPGCON	   WPCM450_GPIOREG(0x44)
#define S3C2400_GPGDAT	   WPCM450_GPIOREG(0x48)
#define S3C2400_GPGUP	   WPCM450_GPIOREG(0x4C)

#define WPCM450_GPG0          WPCM450_GPIONO(WPCM450_GPIO_BANKG, 0)
#define WPCM450_GPG0_INP      (0x00 << 0)
#define WPCM450_GPG0_OUTP     (0x01 << 0)
#define WPCM450_GPG0_EINT8    (0x02 << 0)
#define S3C2400_GPG0_I2SLRCK  (0x02 << 0)

#define WPCM450_GPG1          WPCM450_GPIONO(WPCM450_GPIO_BANKG, 1)
#define WPCM450_GPG1_INP      (0x00 << 2)
#define WPCM450_GPG1_OUTP     (0x01 << 2)
#define WPCM450_GPG1_EINT9    (0x02 << 2)
#define S3C2400_GPG1_I2SSCLK  (0x02 << 2)

#define WPCM450_GPG2          WPCM450_GPIONO(WPCM450_GPIO_BANKG, 2)
#define WPCM450_GPG2_INP      (0x00 << 4)
#define WPCM450_GPG2_OUTP     (0x01 << 4)
#define WPCM450_GPG2_EINT10   (0x02 << 4)
#define S3C2400_GPG2_CDCLK    (0x02 << 4)

#define WPCM450_GPG3          WPCM450_GPIONO(WPCM450_GPIO_BANKG, 3)
#define WPCM450_GPG3_INP      (0x00 << 6)
#define WPCM450_GPG3_OUTP     (0x01 << 6)
#define WPCM450_GPG3_EINT11   (0x02 << 6)
#define S3C2400_GPG3_I2SSDO   (0x02 << 6)
#define S3C2400_GPG3_I2SSDI   (0x03 << 6)

#define WPCM450_GPG4          WPCM450_GPIONO(WPCM450_GPIO_BANKG, 4)
#define WPCM450_GPG4_INP      (0x00 << 8)
#define WPCM450_GPG4_OUTP     (0x01 << 8)
#define WPCM450_GPG4_EINT12   (0x02 << 8)
#define S3C2400_GPG4_MMCCLK   (0x02 << 8)
#define S3C2400_GPG4_I2SSDI   (0x03 << 8)
#define WPCM450_GPG4_LCDPWREN (0x03 << 8)

#define WPCM450_GPG5          WPCM450_GPIONO(WPCM450_GPIO_BANKG, 5)
#define WPCM450_GPG5_INP      (0x00 << 10)
#define WPCM450_GPG5_OUTP     (0x01 << 10)
#define WPCM450_GPG5_EINT13   (0x02 << 10)
#define S3C2400_GPG5_MMCCMD   (0x02 << 10)
#define S3C2400_GPG5_IICSDA   (0x03 << 10)
#define WPCM450_GPG5_SPIMISO1 (0x03 << 10)

#define WPCM450_GPG6          WPCM450_GPIONO(WPCM450_GPIO_BANKG, 6)
#define WPCM450_GPG6_INP      (0x00 << 12)
#define WPCM450_GPG6_OUTP     (0x01 << 12)
#define WPCM450_GPG6_EINT14   (0x02 << 12)
#define S3C2400_GPG6_MMCDAT   (0x02 << 12)
#define S3C2400_GPG6_IICSCL   (0x03 << 12)
#define WPCM450_GPG6_SPIMOSI1 (0x03 << 12)

#define WPCM450_GPG7          WPCM450_GPIONO(WPCM450_GPIO_BANKG, 7)
#define WPCM450_GPG7_INP      (0x00 << 14)
#define WPCM450_GPG7_OUTP     (0x01 << 14)
#define WPCM450_GPG7_EINT15   (0x02 << 14)
#define WPCM450_GPG7_SPICLK1  (0x03 << 14)
#define S3C2400_GPG7_SPIMISO  (0x02 << 14)
#define S3C2400_GPG7_IICSDA   (0x03 << 14)

#define WPCM450_GPG8          WPCM450_GPIONO(WPCM450_GPIO_BANKG, 8)
#define WPCM450_GPG8_INP      (0x00 << 16)
#define WPCM450_GPG8_OUTP     (0x01 << 16)
#define WPCM450_GPG8_EINT16   (0x02 << 16)
#define S3C2400_GPG8_SPIMOSI  (0x02 << 16)
#define S3C2400_GPG8_IICSCL   (0x03 << 16)

#define WPCM450_GPG9          WPCM450_GPIONO(WPCM450_GPIO_BANKG, 9)
#define WPCM450_GPG9_INP      (0x00 << 18)
#define WPCM450_GPG9_OUTP     (0x01 << 18)
#define WPCM450_GPG9_EINT17   (0x02 << 18)
#define S3C2400_GPG9_SPICLK   (0x02 << 18)
#define S3C2400_GPG9_MMCCLK   (0x03 << 18)

#define WPCM450_GPG10         WPCM450_GPIONO(WPCM450_GPIO_BANKG, 10)
#define WPCM450_GPG10_INP     (0x00 << 20)
#define WPCM450_GPG10_OUTP    (0x01 << 20)
#define WPCM450_GPG10_EINT18  (0x02 << 20)

#define WPCM450_GPG11         WPCM450_GPIONO(WPCM450_GPIO_BANKG, 11)
#define WPCM450_GPG11_INP     (0x00 << 22)
#define WPCM450_GPG11_OUTP    (0x01 << 22)
#define WPCM450_GPG11_EINT19  (0x02 << 22)
#define WPCM450_GPG11_TCLK1   (0x03 << 22)

#define WPCM450_GPG12         WPCM450_GPIONO(WPCM450_GPIO_BANKG, 12)
#define WPCM450_GPG12_INP     (0x00 << 24)
#define WPCM450_GPG12_OUTP    (0x01 << 24)
#define WPCM450_GPG12_EINT20  (0x02 << 24)
#define WPCM450_GPG12_XMON    (0x03 << 24)

#define WPCM450_GPG13         WPCM450_GPIONO(WPCM450_GPIO_BANKG, 13)
#define WPCM450_GPG13_INP     (0x00 << 26)
#define WPCM450_GPG13_OUTP    (0x01 << 26)
#define WPCM450_GPG13_EINT21  (0x02 << 26)
#define WPCM450_GPG13_nXPON   (0x03 << 26)

#define WPCM450_GPG14         WPCM450_GPIONO(WPCM450_GPIO_BANKG, 14)
#define WPCM450_GPG14_INP     (0x00 << 28)
#define WPCM450_GPG14_OUTP    (0x01 << 28)
#define WPCM450_GPG14_EINT22  (0x02 << 28)
#define WPCM450_GPG14_YMON    (0x03 << 28)

#define WPCM450_GPG15         WPCM450_GPIONO(WPCM450_GPIO_BANKG, 15)
#define WPCM450_GPG15_INP     (0x00 << 30)
#define WPCM450_GPG15_OUTP    (0x01 << 30)
#define WPCM450_GPG15_EINT23  (0x02 << 30)
#define WPCM450_GPG15_nYPON   (0x03 << 30)


#define WPCM450_GPG_PUPDIS(x)  (1<<(x))

/* Port H consists of11 GPIO/serial/Misc pins
 *
 * GPGCON has 2 bits for each of the input pins on port F
 *   00 = 0 input, 1 output, 2 interrupt (EINT0..7), 3 special func
 *
 * pull up works like all other ports.
*/

#define WPCM450_GPHCON	   WPCM450_GPIOREG(0x70)
#define WPCM450_GPHDAT	   WPCM450_GPIOREG(0x74)
#define WPCM450_GPHUP	   WPCM450_GPIOREG(0x78)

#define WPCM450_GPH0        WPCM450_GPIONO(WPCM450_GPIO_BANKH, 0)
#define WPCM450_GPH0_INP    (0x00 << 0)
#define WPCM450_GPH0_OUTP   (0x01 << 0)
#define WPCM450_GPH0_nCTS0  (0x02 << 0)

#define WPCM450_GPH1        WPCM450_GPIONO(WPCM450_GPIO_BANKH, 1)
#define WPCM450_GPH1_INP    (0x00 << 2)
#define WPCM450_GPH1_OUTP   (0x01 << 2)
#define WPCM450_GPH1_nRTS0  (0x02 << 2)

#define WPCM450_GPH2        WPCM450_GPIONO(WPCM450_GPIO_BANKH, 2)
#define WPCM450_GPH2_INP    (0x00 << 4)
#define WPCM450_GPH2_OUTP   (0x01 << 4)
#define WPCM450_GPH2_TXD0   (0x02 << 4)

#define WPCM450_GPH3        WPCM450_GPIONO(WPCM450_GPIO_BANKH, 3)
#define WPCM450_GPH3_INP    (0x00 << 6)
#define WPCM450_GPH3_OUTP   (0x01 << 6)
#define WPCM450_GPH3_RXD0   (0x02 << 6)

#define WPCM450_GPH4        WPCM450_GPIONO(WPCM450_GPIO_BANKH, 4)
#define WPCM450_GPH4_INP    (0x00 << 8)
#define WPCM450_GPH4_OUTP   (0x01 << 8)
#define WPCM450_GPH4_TXD1   (0x02 << 8)

#define WPCM450_GPH5        WPCM450_GPIONO(WPCM450_GPIO_BANKH, 5)
#define WPCM450_GPH5_INP    (0x00 << 10)
#define WPCM450_GPH5_OUTP   (0x01 << 10)
#define WPCM450_GPH5_RXD1   (0x02 << 10)

#define WPCM450_GPH6        WPCM450_GPIONO(WPCM450_GPIO_BANKH, 6)
#define WPCM450_GPH6_INP    (0x00 << 12)
#define WPCM450_GPH6_OUTP   (0x01 << 12)
#define WPCM450_GPH6_TXD2   (0x02 << 12)
#define WPCM450_GPH6_nRTS1  (0x03 << 12)

#define WPCM450_GPH7        WPCM450_GPIONO(WPCM450_GPIO_BANKH, 7)
#define WPCM450_GPH7_INP    (0x00 << 14)
#define WPCM450_GPH7_OUTP   (0x01 << 14)
#define WPCM450_GPH7_RXD2   (0x02 << 14)
#define WPCM450_GPH7_nCTS1  (0x03 << 14)

#define WPCM450_GPH8        WPCM450_GPIONO(WPCM450_GPIO_BANKH, 8)
#define WPCM450_GPH8_INP    (0x00 << 16)
#define WPCM450_GPH8_OUTP   (0x01 << 16)
#define WPCM450_GPH8_UCLK   (0x02 << 16)

#define WPCM450_GPH9          WPCM450_GPIONO(WPCM450_GPIO_BANKH, 9)
#define WPCM450_GPH9_INP      (0x00 << 18)
#define WPCM450_GPH9_OUTP     (0x01 << 18)
#define WPCM450_GPH9_CLKOUT0  (0x02 << 18)

#define WPCM450_GPH10         WPCM450_GPIONO(WPCM450_GPIO_BANKH, 10)
#define WPCM450_GPH10_INP     (0x00 << 20)
#define WPCM450_GPH10_OUTP    (0x01 << 20)
#define WPCM450_GPH10_CLKOUT1 (0x02 << 20)

/* miscellaneous control */
#define S3C2400_MISCCR	   WPCM450_GPIOREG(0x54)
#define WPCM450_MISCCR	   WPCM450_GPIOREG(0x80)
#define WPCM450_DCLKCON	   WPCM450_GPIOREG(0x84)

/* see clock.h for dclk definitions */

/* pullup control on databus */
#define WPCM450_MISCCR_SPUCR_HEN    (0<<0)
#define WPCM450_MISCCR_SPUCR_HDIS   (1<<0)
#define WPCM450_MISCCR_SPUCR_LEN    (0<<1)
#define WPCM450_MISCCR_SPUCR_LDIS   (1<<1)

#define S3C2400_MISCCR_SPUCR_LEN    (0<<0)
#define S3C2400_MISCCR_SPUCR_LDIS   (1<<0)
#define S3C2400_MISCCR_SPUCR_HEN    (0<<1)
#define S3C2400_MISCCR_SPUCR_HDIS   (1<<1)

#define S3C2400_MISCCR_HZ_STOPEN    (0<<2)
#define S3C2400_MISCCR_HZ_STOPPREV  (1<<2)

#define WPCM450_MISCCR_USBDEV	    (0<<3)
#define WPCM450_MISCCR_USBHOST	    (1<<3)

#define WPCM450_MISCCR_CLK0_MPLL    (0<<4)
#define WPCM450_MISCCR_CLK0_UPLL    (1<<4)
#define WPCM450_MISCCR_CLK0_FCLK    (2<<4)
#define WPCM450_MISCCR_CLK0_HCLK    (3<<4)
#define WPCM450_MISCCR_CLK0_PCLK    (4<<4)
#define WPCM450_MISCCR_CLK0_DCLK0   (5<<4)
#define WPCM450_MISCCR_CLK0_MASK    (7<<4)

#define WPCM450_MISCCR_CLK1_MPLL    (0<<8)
#define WPCM450_MISCCR_CLK1_UPLL    (1<<8)
#define WPCM450_MISCCR_CLK1_FCLK    (2<<8)
#define WPCM450_MISCCR_CLK1_HCLK    (3<<8)
#define WPCM450_MISCCR_CLK1_PCLK    (4<<8)
#define WPCM450_MISCCR_CLK1_DCLK1   (5<<8)
#define WPCM450_MISCCR_CLK1_MASK    (7<<8)

#define WPCM450_MISCCR_USBSUSPND0   (1<<12)
#define WPCM450_MISCCR_USBSUSPND1   (1<<13)

#define WPCM450_MISCCR_nRSTCON	    (1<<16)

#define WPCM450_MISCCR_nEN_SCLK0    (1<<17)
#define WPCM450_MISCCR_nEN_SCLK1    (1<<18)
#define WPCM450_MISCCR_nEN_SCLKE    (1<<19)
#define WPCM450_MISCCR_SDSLEEP	    (7<<17)

/* external interrupt control... */
/* WPCM450_EXTINT0 -> irq sense control for EINT0..EINT7
 * WPCM450_EXTINT1 -> irq sense control for EINT8..EINT15
 * WPCM450_EXTINT2 -> irq sense control for EINT16..EINT23
 *
 * note WPCM450_EXTINT2 has filtering options for EINT16..EINT23
 *
 * Samsung datasheet p9-25
*/
#define S3C2400_EXTINT0    WPCM450_GPIOREG(0x58)
#define WPCM450_EXTINT0	   WPCM450_GPIOREG(0x88)
#define WPCM450_EXTINT1	   WPCM450_GPIOREG(0x8C)
#define WPCM450_EXTINT2	   WPCM450_GPIOREG(0x90)

/* values for WPCM450_EXTINT0/1/2 */
#define WPCM450_EXTINT_LOWLEV	 (0x00)
#define WPCM450_EXTINT_HILEV	 (0x01)
#define WPCM450_EXTINT_FALLEDGE	 (0x02)
#define WPCM450_EXTINT_RISEEDGE	 (0x04)
#define WPCM450_EXTINT_BOTHEDGE	 (0x06)

/* interrupt filtering conrrol for EINT16..EINT23 */
#define WPCM450_EINFLT0	   WPCM450_GPIOREG(0x94)
#define WPCM450_EINFLT1	   WPCM450_GPIOREG(0x98)
#define WPCM450_EINFLT2	   WPCM450_GPIOREG(0x9C)
#define WPCM450_EINFLT3	   WPCM450_GPIOREG(0xA0)

/* values for interrupt filtering */
#define WPCM450_EINTFLT_PCLK		(0x00)
#define WPCM450_EINTFLT_EXTCLK		(1<<7)
#define WPCM450_EINTFLT_WIDTHMSK(x)	((x) & 0x3f)

/* removed EINTxxxx defs from here, not meant for this */

/* GSTATUS have miscellaneous information in them
 *
 */

#define WPCM450_GSTATUS0   WPCM450_GPIOREG(0x0AC)
#define WPCM450_GSTATUS1   WPCM450_GPIOREG(0x0B0)
#define WPCM450_GSTATUS2   WPCM450_GPIOREG(0x0B4)
#define WPCM450_GSTATUS3   WPCM450_GPIOREG(0x0B8)
#define WPCM450_GSTATUS4   WPCM450_GPIOREG(0x0BC)

#define WPCM450_GSTATUS0_nWAIT	   (1<<3)
#define WPCM450_GSTATUS0_NCON	   (1<<2)
#define WPCM450_GSTATUS0_RnB	   (1<<1)
#define WPCM450_GSTATUS0_nBATTFLT  (1<<0)

#define WPCM450_GSTATUS1_IDMASK	   (0xffff0000)
#define WPCM450_GSTATUS1_2410	   (0x32410000)
#define WPCM450_GSTATUS1_2440	   (0x32440000)

#define WPCM450_GSTATUS2_WTRESET   (1<<2)
#define WPCM450_GSTATUS2_OFFRESET  (1<<1)
#define WPCM450_GSTATUS2_PONRESET  (1<<0)

/* open drain control register */
#define S3C2400_OPENCR     WPCM450_GPIOREG(0x50)

#define S3C2400_OPENCR_OPC_RXD1DIS  (0<<0)
#define S3C2400_OPENCR_OPC_RXD1EN   (1<<0)
#define S3C2400_OPENCR_OPC_TXD1DIS  (0<<1)
#define S3C2400_OPENCR_OPC_TXD1EN   (1<<1)
#define S3C2400_OPENCR_OPC_CMDDIS   (0<<2)
#define S3C2400_OPENCR_OPC_CMDEN    (1<<2)
#define S3C2400_OPENCR_OPC_DATDIS   (0<<3)
#define S3C2400_OPENCR_OPC_DATEN    (1<<3)
#define S3C2400_OPENCR_OPC_MISODIS  (0<<4)
#define S3C2400_OPENCR_OPC_MISOEN   (1<<4)
#define S3C2400_OPENCR_OPC_MOSIDIS  (0<<5)
#define S3C2400_OPENCR_OPC_MOSIEN   (1<<5)

#endif	/* __ASM_ARCH_REGS_GPIO_H */

