/*
 * arch/ppc/platforms/83xx/tqm834x.h
 *
 * TQ Components TQM834x common board definitions
 *
 * Copyright 2005 DENX Software Engineering
 * Derived from mpc834x_sys.h
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#ifndef __MACH_TQM834X_H__
#define __MACH_TQM834X_H__

#include <linux/init.h>
#include <linux/seq_file.h>
#include <syslib/ppc83xx_setup.h>
#include <asm/ppcboot.h>

#define VIRT_IMMRBAR		((uint)0xfe000000)

#define PIRQA	MPC83xx_IRQ_EXT2
#define PIRQB	MPC83xx_IRQ_EXT3
#define PIRQC	MPC83xx_IRQ_EXT6
#define PIRQD	MPC83xx_IRQ_EXT5

#define MPC83xx_PCI1_LOWER_IO	0x00000000
#define MPC83xx_PCI1_UPPER_IO	0x00ffffff
#define MPC83xx_PCI1_LOWER_MEM	0xc0000000
#define MPC83xx_PCI1_UPPER_MEM	0xdfffffff
#define MPC83xx_PCI1_IO_BASE	0xe2000000
#define MPC83xx_PCI1_MEM_OFFSET	0x00000000
#define MPC83xx_PCI1_IO_SIZE	0x01000000

#endif                /* __MACH_TQM834X_H__ */
