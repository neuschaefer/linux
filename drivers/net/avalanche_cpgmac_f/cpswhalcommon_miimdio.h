/*
 *
 * CPSWHALCOMMON_MIIMDIO.h
 * Description:
 * see below
 *
 *
 * Copyright (C) 2008, Texas Instruments, Incorporated
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 */


/*****************************************************************************
**
**  FILE: CPSWHALCOMMON_MIIMDIO.H   User Include for MDIO API Access
**
**  DESCRIPTION:
**      This include file contains definitions for the the MDIO API
**
**  HISTORY:
**      27Mar02 Michael Hanrahan Original (modified from emacmdio.h)
**      04Apr02 Michael Hanrahan Added Interrupt Support
*****************************************************************************/
#ifndef _CPSWHALCOMMON_MIIMDIO_H
#define _CPSWHALCOMMON_MIIMDIO_H

/* Define a type for printf function required for MII module */
typedef int (*OsPrintf)(const char *Format, ...);

/*Version Information */

void cpswHalCommonMiiMdioGetVer(bit32u miiBase, bit32u *ModID,  bit32u *RevMaj,  bit32u *RevMin);

/*Called once at the begining of time                                        */

int  cpswHalCommonMiiMdioGetPhyDevSize(void);  /*Called first to get size of storage needed!*/

int  cpswHalCommonMiiMdioInit(PHY_DEVICE *PhyDev, bit32u miibase, bit32u inst, bit32u PhyMask,
                        bit32u MLinkMask, bit32u MdixMask, bit32u ResetBase, bit32u ResetBit, bit32u MdioBusFreq,
                        bit32u MdioClockFreq,int verbose,int (*osprintf)(const char *Format, ...));



/*Called every 10 mili Seconds, returns TRUE if there has been a mode change */

int cpswHalCommonMiiMdioTic(PHY_DEVICE *PhyDev);

/*Called to set Phy mode                                                     */

void cpswHalCommonMiiMdioSetPhyMode(PHY_DEVICE *PhyDev,bit32u PhyMode);

/*Calls to retreive info after a mode change!                                */

int  cpswHalCommonMiiMdioGetDuplex(PHY_DEVICE *PhyDev);
int  cpswHalCommonMiiMdioGetSpeed(PHY_DEVICE *PhyDev);
int  cpswHalCommonMiiMdioGetPhyNum(PHY_DEVICE *PhyDev);
int  cpswHalCommonMiiMdioGetLinked(PHY_DEVICE *PhyDev);
void cpswHalCommonMiiMdioLinkChange(PHY_DEVICE *PhyDev);
int  cpswHalCommonMiiMdioGetLoopback(PHY_DEVICE *PhyDev);

/*  Shut Down  */

void cpswHalCommonMiiMdioClose(PHY_DEVICE *PhyDev, int Full);

/*
 *
 *   Expert Use Functions (exported)
 *
 */

bit32u _cpswHalCommonMiiMdioUserAccessRead (PHY_DEVICE *PhyDev, bit32u regadr, bit32u phyadr);
void   _cpswHalCommonMiiMdioUserAccessWrite(PHY_DEVICE *PhyDev, bit32u regadr, bit32u phyadr, bit32u data);


/* Phy Mode Values  */
#define NWAY_AUTOMDIX       (1<<16)
#define NWAY_FD1000         (1<<13)
#define NWAY_HD1000         (1<<12)
#define NWAY_NOPHY          (1<<10)
#define NWAY_LPBK           (1<<9)
#define NWAY_FD100          (1<<8)
#define NWAY_HD100          (1<<7)
#define NWAY_FD10           (1<<6)
#define NWAY_HD10           (1<<5)
#define NWAY_AUTO           (1<<0)

/*
 *
 *    Tic() return values
 *
 */

#define _MIIMDIO_MDIXFLIP (1<<28)

#define _AUTOMDIX_DELAY_MIN  80  /* milli-seconds*/
#define _AUTOMDIX_DELAY_MAX 200  /* milli-seconds*/

#endif
