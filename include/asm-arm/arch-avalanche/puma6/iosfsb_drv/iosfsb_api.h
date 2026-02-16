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
#define IOSFSB_VTUNIT_PORT                  0x00
#define IOSFSB_PUNIT_PORT                   0x04
#define IOSFSB_CPUNIT_PORT                  0x0A
#define IOSFSB_DOCSIS_MODPHY_AFE_PORT       0x5A
#define IOSFSB_DOCSIS_TX_DAC_PORT           0x5B
#define IOSFSB_DOCSIS_NB_ADC_PORT           0x5C
#define IOSFSB_DOCSIS_IOSFSB_MASTER_PORT    0x5D
#define IOSFSB_BBU_ADC_PORT                 0x5E
#define IOSFSB_MOCA_MODPHY_AFE_PORT         0x8E
#define IOSFSB_PCIE_MODPHY_AFE_PORT         0x11
#define IOSFSB_SATA_MODPHY_AFE_PORT         0x59

/* kernel api commands*/
#define IOSFSB_REG_READ         (0xD0)
#define IOSFSB_REG_WRITE        (0xE0)
#define IOSFSB_PCI_CFG_READ     (0xF0)
#define IOSFSB_PCI_CFG_WRITE    (0xF1)
#define IOSFSB_MEMORY_READ      (0xF8)
#define IOSFSB_MEMORY_WRITE     (0xF9)
#define IOSFSB_IO_READ          (0xFA)
#define IOSFSB_IO_WRITE         (0xFB)

/* user ioctl access_type values */
#define IOSFSB_IO_ACCESS            (0x01)
#define IOSFSB_MEMORY_ACCESS        (0x02)
#define IOSFSB_PCI_CFG_ACCESS       (0x03)
#define IOSFSB_REG_ACCESS           (0x04)


struct iosfsb_info_user
{
    unsigned int  dest_port;
    unsigned int  offset;
    unsigned int  mask;
    unsigned int  value;
    unsigned int  access_type;
};


/*   VTUNIT registers       */
#define IOSFSB_VTUNIT_SAPm_ROUTING_CTRL_REGISTER          0x3E
#define IOSFSB_VTUNIT_SAPm_ROUTING_BUNIT_SNOOPED_VAL      0x0
#define IOSFSB_VTUNIT_SAPm_ROUTING_BUNIT_NON_SNOOPED_VAL  0x10
#define IOSFSB_VTUNIT_SAPm_ROUTING_SAPMEM_NON_SNOOPED_VAL 0x20

/*   CPUNIT registers       */
#define IOSFSB_CPUNIT_MSIP_CTRL_REGISTER                  (0xC0)




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
/**************************************************************************/
/*! \fn  int iosfsb_mem_read_modify_write ( unsigned char iosfsb_port,unsigned int iosfsb_addr, unsigned int mask, unsigned int iosfsb_data)
 **************************************************************************
 *  \brief This function is used to read modifies write iosfsb memory register value
 *  input:
 *  unsigned char iosfsb_port = port id
 *  unsigned int iosfsb_addr = register address
 *  unsigned int mask = bit mask for the relevant bits
 *  unsigned int iosfsb_data = register value
 *  \return . fail / success
 **************************************************************************/
int iosfsb_mem_read_modify_write ( unsigned char iosfsb_port,unsigned int iosfsb_addr, unsigned int mask, unsigned int iosfsb_data);

/**************************************************************************/
/*! \fn  int iosfsb_reg_read_modify_write ( unsigned char iosfsb_port,unsigned int iosfsb_addr, unsigned int mask, unsigned int iosfsb_data)
 **************************************************************************
 *  \brief This function is used to read modifies and write iosfsb register value
 *  input:
 *  unsigned char iosfsb_port = port id
 *  unsigned int iosfsb_addr = register address
 *  unsigned int mask = bit mask for the relevant bits
 *  unsigned int iosfsb_data = register value
 *  \return . fail / success
 **************************************************************************/
int iosfsb_reg_read_modify_write ( unsigned char iosfsb_port,unsigned int iosfsb_addr, unsigned int mask, unsigned int iosfsb_data);

/**************************************************************************/
/*! \fn  int iosfsb_write ( unsigned char iosfsb_port,unsigned int iosfsb_addr, unsigned int iosfsb_data,unsigned int iosfsb_write_opcode)
 **************************************************************************
 *  \brief This function is used to write iosfsb register value
 *  input:
 *  unsigned char iosfsb_port = port id
 *  unsigned int iosfsb_addr = register address
 *  unsigned int iosfsb_data = register new value
 *  unsigned int iosfsb_write_opcode = iosf operation opcode
 *  \return . fail / success
 **************************************************************************/
int iosfsb_write ( unsigned char iosfsb_port,unsigned int iosfsb_addr, unsigned int iosfsb_data,unsigned int iosfsb_write_opcode);

/**************************************************************************/
/*! \fn  int iosfsb_read (unsigned char iosfsb_port,unsigned int iosfsb_addr,unsigned int *iosfsb_read_data,unsigned int iosfsb_read_opcode)
 **************************************************************************
 *  \brief This function is used to read iosf register value
 *  input:
 *  unsigned char iosfsb_port = port id
 *  unsigned int iosfsb_addr = register address
 *  unsigned int iosfsb_read_data = register value
 *  unsigned int iosfsb_read_opcode = iosf operation opcode
 *  \return . fail / success
 **************************************************************************/
int iosfsb_read (unsigned char iosfsb_port,unsigned int iosfsb_addr,unsigned int *iosfsb_read_data,unsigned int iosfsb_read_opcode);

/**************************************************************************/
/*! \fn  int iosfsb_unprotected_write ( unsigned char iosfsb_port,unsigned int iosfsb_addr, unsigned int iosfsb_data, unsigned int iosfsb_write_opcode);
 **************************************************************************
 *  \brief This function is used to write iosfsb register value - 
 *         iosfsb_down() must_be called before calling this function and iosfsb_up() must_be called after calling this function.  
 *  input:
 *  unsigned char iosfsb_port = port id
 *  unsigned int iosfsb_addr = register address
 *  unsigned int iosfsb_data = register new value
 *  unsigned int iosfsb_write_opcode = iosf operation opcode
 *  \return . fail / success
 **************************************************************************/
int iosfsb_unprotected_write ( unsigned char iosfsb_port,unsigned int iosfsb_addr, unsigned int iosfsb_data, unsigned int iosfsb_write_opcode);

/**************************************************************************/
/*! \fn  int iosfsb_unprotected_read ( unsigned char iosfsb_port,unsigned int iosfsb_addr, unsigned int *iosfsb_data,unsigned int iosfsb_read_opcode);
 **************************************************************************
 *  \brief This function is used to read iosf register value - 
 *         iosfsb_down() must_be called before calling this function and iosfsb_up() must_be called after calling this function. 
 *  input:
 *  unsigned char iosfsb_port = port id
 *  unsigned int iosfsb_addr = register address
 *  unsigned int iosfsb_read_data = register value
 *  unsigned int iosfsb_read_opcode = iosf operation opcode
 *  \return . fail / success
 **************************************************************************/
int iosfsb_unprotected_read (unsigned char iosfsb_port,unsigned int iosfsb_addr,unsigned int *iosfsb_read_data,unsigned int iosfsb_read_opcode);
/**************************************************************************/
/*! \fn  int iosfsb_down();
 **************************************************************************
 *  \brief This function is used to lock the iosfsb semaphore
 *  \return . fail / success
 **************************************************************************/
int iosfsb_down(void);

/**************************************************************************/
/*! \fn  int iosfsb_up();
 **************************************************************************
 *  \brief This function is used to release the iosfsb semaphore
 *  \return . fail / success
 **************************************************************************/
int iosfsb_up(void);

#endif /* __KERNEL__ */
#endif /* IOSFSB_API_H */

