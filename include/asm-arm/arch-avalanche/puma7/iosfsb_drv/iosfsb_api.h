/*
 *
 * iosfsb_api.h
 * Description:
 * iosfsb common type declarations
 *
 *  GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2011 Intel Corporation. All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of version 2 of the GNU General Public License as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *  The full GNU General Public License is included in this distribution
 *  in the file called LICENSE.GPL.
 *
 *  Contact Information:
 *    Intel Corporation
 *    2200 Mission College Blvd.
 *    Santa Clara, CA  97052
 *
 * The file contains the main data structure and API definitions for Linux Hardware Mutex driver
 * Intel CE processor supports 4 masters and 12 mutexes avalible
 *
 */


/*------------------------------------------------------------------------------
* File Name: iosfsb_api.h
*------------------------------------------------------------------------------
*/
//! \file
#ifndef  IOSFSB_API_H
#define  IOSFSB_API_H


#define IOSFSB_INTERFACE_DRIVER_DEV_NAME "/dev/docsis_iosfsb_drv"

/* IOSF ports*/
#define IOSFSB_AUNIT_PORT          0x00
#define IOSFSB_BUNIT_PORT          0x03
#define IOSFSB_CCK_PORT            0x14
#define IOSFSB_CCU_PORT	           0xA9
#define IOSFSB_CUNIT_PORT          0x08
#define IOSFSB_DDRPHY_PORT         0x0C
#define IOSFSB_DFXAGG_PORT         0xB7
#define IOSFSB_DFXJTAGXACT_PORT    0xBD
#define IOSFSB_DFXLAKEMORE_PORT    0x17
#define IOSFSB_DFXMASTER_PORT      0x15
#define IOSFSB_DFXPNDRFU_PORT      0x05
#define IOSFSB_DFXREGION0_PORT     0x16
#define IOSFSB_DFXREGION0FP_PORT   0xB8
#define IOSFSB_DFXREGION1_PORT     0xAA
#define IOSFSB_DFXREGION1FP_PORT   0xB9
#define IOSFSB_DFXREGION2_PORT     0xB3
#define IOSFSB_DFXREGION2FP_PORT   0xBA
#define IOSFSB_DFXREGION3_PORT     0xB4
#define IOSFSB_DFXREGION3FP_PORT   0xBB
#define IOSFSB_DFXVISA_PORT        0x18
#define IOSFSB_DOCSISPHY3_PORT     0x1A
#define IOSFSB_DRNG_PORT           0x0F
#define IOSFSB_DUNIT_PORT          0x01
#define IOSFSB_FUSECONT_PORT       0x40
#define IOSFSB_GBEPHY1_PORT        0x1B
#define IOSFSB_GBEPHY2_PORT        0x1D
#define IOSFSB_GPIONE_PORT         0x09
#define IOSFSB_GPIONW_PORT         0x13
#define IOSFSB_GPIOSE_PORT         0x48
#define IOSFSB_GPIOSW_PORT         0xB2
#define IOSFSB_GPIOV_PORT          0xB6
#define IOSFSB_GPIOW_PORT          0xA8
#define IOSFSB_ILB_PORT            0x53
#define IOSFSB_LPIOEP_PORT         0xA0
#define IOSFSB_LPIO1_PORT          0xAB
#define IOSFSB_LPIO2_PORT          0xAC
#define IOSFSB_MOCAPHY_PORT        0x10
#define IOSFSB_MSPOD_PORT          0xA5
#define IOSFSB_NETIPEP_PORT        0x20
#define IOSFSB_OTG_PORT            0x59
#define IOSFSB_PCIe_PORT           0xA4
#define IOSFSB_PCIEPHY_PORT        0xA6
#define IOSFSB_PCUEP_PORT          0x54
#define IOSFSB_PMC_PORT            0x52
#define IOSFSB_PSF0_PORT           0x45
#define IOSFSB_PSF1_PORT           0x46
#define IOSFSB_PSF2_PORT           0x47
#define IOSFSB_PSFSATA_PORT        0xA2
#define IOSFSB_PUNIT_PORT          0x04
#define IOSFSB_REUT_PORT           0x0D
#define IOSFSB_SATA_PORT           0xA1
#define IOSFSB_SATAPHY_PORT        0xA3
#define IOSFSB_SATPHY2_PORT        0x12
#define IOSFSB_SCCEMMC1_PORT       0x5B
#define IOSFSB_SCCEMMC2_PORT       0x5C
#define IOSFSB_SCCEP_PORT          0x63
#define IOSFSB_SCCSDCARD_PORT      0x5D
#define IOSFSB_SECEP_PORT          0x44
#define IOSFSB_SMBUS_PORT          0x55
#define IOSFSB_SPI_PORT            0x51
#define IOSFSB_SSICPHY_PORT        0xB0
#define IOSFSB_STCEP_PORT          0x90
#define IOSFSB_STCSEC_PORT         0x91
#define IOSFSB_STCTSD1_PORT        0x92
#define IOSFSB_STCTSD2_PORT        0x93
#define IOSFSB_SVID_PORT           0x0A
#define IOSFSB_TSPREFILTER_PORT    0xB1
#define IOSFSB_TUNIT_PORT          0x02
#define IOSFSB_UART_PORT           0x50
#define IOSFSB_USB2SHIP_PORT       0x43
#define IOSFSB_USH_PORT            0x5A
#define IOSFSB_USHCPHY_PORT        0x61
#define IOSFSB_NETIPEP1_PORT       0x20
#define IOSFSB_NETIPEP2_PORT       0x20
#define IOSFSB_NETIPEP3_PORT       0x20
#define IOSFSB_NETIPEP4_PORT       0x20
#define IOSFSB_NETIPEP5_PORT       0x20
#define IOSFSB_NETIPEP6_PORT       0x20

/* kernel api commands*/

#define IOSFSB_OP_MEMREAD  (0x0)
#define IOSFSB_OP_MEMWRITE (0x1)
#define IOSFSB_OP_IOREAD   (0x2)
#define IOSFSB_OP_IOWRITE  (0x3)
#define IOSFSB_OP_CFGREAD  (0x4)
#define IOSFSB_OP_CFGWRITE (0x5)
#define IOSFSB_OP_CRREAD   (0x6)
#define IOSFSB_OP_CRWRITE  (0x7)


/* user ioctl access_type values */
#define IOSFSB_IO_ACCESS            (0x01)
#define IOSFSB_MEMORY_ACCESS        (0x02)
#define IOSFSB_PCI_CFG_ACCESS       (0x03)
#define IOSFSB_REG_ACCESS           (0x04)


struct iosfsb_info_user
{
    unsigned int  dest_port;
    unsigned short address0;
    unsigned int   address1;
    unsigned int  mask;
    unsigned int  value;
    unsigned int  access_type;
};

/********************************************************************************************************/
/* IOCTL commands:

   If you are adding new ioctl's to the kernel, you should use the _IO
   macros defined in <linux/ioctl.h> _IO macros are used to create ioctl numbers:

    _IO(type, nr)         - an ioctl with no parameter.
   _IOW(type, nr, size)  - an ioctl with write parameters (copy_from_user), kernel would actually read data from user space
   _IOR(type, nr, size)  - an ioctl with read parameters (copy_to_user), kernel would actually write data to user space
   _IOWR(type, nr, size) - an ioctl with both write and read parameters

   'Write' and 'read' are from the user's point of view, just like the
    system calls 'write' and 'read'.  For example, a SET_FOO ioctl would
    be _IOW, although the kernel would actually read data from user space;
    a GET_FOO ioctl would be _IOR, although the kernel would actually write
    data to user space.

    The first argument to _IO, _IOW, _IOR, or _IOWR is an identifying letter
    or number from the SoC_ModuleIds_e enum located in this file.

    The second argument to _IO, _IOW, _IOR, or _IOWR is a sequence number
    to distinguish ioctls from each other.

   The third argument to _IOW, _IOR, or _IOWR is the type of the data going
   into the kernel or coming out of the kernel (e.g.  'int' or 'struct foo').

   NOTE!  Do NOT use sizeof(arg) as the third argument as this results in
   your ioctl thinking it passes an argument of type size_t.

*/

#define SOC_IOSFSB_MODULE_ID                  (0x01)
#define IOSFSB_READ_REG_CMD                      _IOR(SOC_IOSFSB_MODULE_ID, 1, unsigned long)
#define IOSFSB_WRITE_REG_CMD                     _IOW(SOC_IOSFSB_MODULE_ID, 2, unsigned long)
#define IOSFSB_READ_MODIFY_WRITE_CMD             _IOWR(SOC_IOSFSB_MODULE_ID, 3, unsigned long)



#ifdef __KERNEL__
/**************************************************************************
 * int iosfsb_write (unsigned char destID, unsigned int writeOpcode, 
 *                             unsigned short addr0, unsigned int data, unsigned int addr1) 
 **************************************************************************
 *  DESCRIPTION: This function is used to write iosfsb register value     *
 *  INPUT: destID = Destination ID (port ID)                              *
 *         opcode  = Write operation code                                 *
 *         addr0   = Remote address of the register (include offset)      *
 *         data    = Data to write                         *
 *         addr1   = Used only when the address is bigger than 16 bits    *
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int iosfsb_write (unsigned char destID, unsigned int writeOpcode, 
                              unsigned short addr0, unsigned int data, unsigned int addr1);
/**************************************************************************
 * int iosfsb_read (unsigned char destID, unsigned int readOpcode, 
 *                   unsigned short addr0, unsigned int * data, unsigned int addr1) 
 **************************************************************************
 *  DESCRIPTION: This function is used to write iosfsb register value     *
 *  INPUT: destID = Destination ID (port ID)                              *
 *         opcode  = Read operation code                                  *
 *         addr0   = Remote address of the register (include offset)      *
 *         data    = Pointer to buffer to hold the read data              *
 *         addr1   = Used only when the address is bigger than 16 bits    *
 *                                                                        *
 *  OUTPUT:  Data to read                                                 *
 **************************************************************************/
int iosfsb_read (unsigned char destID, unsigned int readOpcode, 
                              unsigned short addr0, unsigned int * data, unsigned int addr1);
/**************************************************************************
 * int iosfsb_mem_read_modify_write (unsigned char destID, unsigned short addr0, 
 *                                   unsigned int data, unsigned int addr1, unsigned int mask) 
 **************************************************************************
 *  DESCRIPTION: This function is used to read modifies write iosfsb      *
 *               memory register value                                    *
 *  INPUT: destID = Destination ID (port ID)                              *
 *         addr0   = Remote address of the memory                         *
 *                   register (include offset)                            *
 *         data    = Data to write                                        *
 *         addr1   = Used only when the address is bigger than 16 bits    *
 *         mask    = bit mask for the relevant bits                       *
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int iosfsb_memReadModifyWrite (unsigned char destID, unsigned short addr0, 
                                  unsigned int data, unsigned int addr1, unsigned int mask);
/**************************************************************************
 * int iosfsb_regReadModifyWrite (unsigned char destID, unsigned short addr0, 
 *                                unsigned int data, unsigned int addr1, unsigned int mask) 
 **************************************************************************
 *  DESCRIPTION: This function is used to read modifies write iosfsb      *
 *               register value                                           *
 *  INPUT: destID = Destination ID (port ID)                              *
 *         addr0   = Remote address of the register (include offset)      *
 *         data    = Data to write                                        *
 *         addr1   = Used only when the address is bigger than 16 bits    *
 *         mask    = bit mask for the relevant bits                       *
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int iosfsb_regReadModifyWrite (unsigned char destID, unsigned short addr0, 
                                  unsigned int data, unsigned int addr1, unsigned int mask);

#endif /* __KERNEL__ */
#endif /* IOSFSB_API_H */

