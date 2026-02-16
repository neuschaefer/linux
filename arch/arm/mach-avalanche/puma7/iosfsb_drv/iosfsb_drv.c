/*
 *
 * iosfsb_drv.c
 * Description:
 * iosfsb driver
 *
 *  GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2015-2017 Intel Corporation. All rights reserved.
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
 */

/*------------------------------------------------------------------------------
 * File Name: iosfsb_drv.c
 *------------------------------------------------------------------------------
 */

#include <linux/module.h>   /* for modules */
#include <linux/fs.h>       /* file_operations */
#include <linux/uaccess.h>  /* copy_(to,from)_user */
#include <linux/cdev.h>     /* cdev utilities */
#include <linux/device.h>
#include <mach/puma.h>
#include "iosfsb_api.h"
#include <asm-arm/arch-avalanche/puma7/io_ctrl/puma7_io_ctrl.h>

/**************************************************************************/
/*           LOCAL DEFINE DECLERATIONS                                    */
/**************************************************************************/

#define DEV_NAME  "docsis_iosfsb_drv"
#define IOSFSB_OK                 (0)
#define IOSFSB_FAIL               (-1)
#define NEED_INIT                 (0)
#define INIT_DONE                 (1)
#define IOSFSB_CFG_BASE         (AVALANCHE_IOSFSB_BASE)
#define IOSF2OCP_SB_INDIRECT_ADDR_0   (IOSFSB_CFG_BASE + 0x2008)
#define IOSF2OCP_SB_INDIRECT_ADDR_1   (IOSFSB_CFG_BASE + 0x200C)
#define IOSF2OCP_SB_INDIRECT_DATA_0   (IOSFSB_CFG_BASE + 0x2010)
#define IOSF2OCP_SB_INDIRECT_DATA_1   (IOSFSB_CFG_BASE + 0x2014)
#define IOSF2OCP_SB_INDIRECT_DATA_2   (IOSFSB_CFG_BASE + 0x2018)
#define IOSF2OCP_SB_INDIRECT_DATA_3   (IOSFSB_CFG_BASE + 0x201c)
#define IOSF2OCP_SB_INDIRECT_HEADER   (IOSFSB_CFG_BASE + 0x2000)

/* Reg Read / Write Macros */
#define reg_write_32(addr, data) (( *(volatile unsigned int *) (addr) ) = data)
#define reg_read_32(addr)        ( *(volatile unsigned int *) (addr) )

/****************************************** 
* (Define the SB indirect ADDR 0          *
* (3:0)  FBE     - Expanded Header ID     *
* (7:4)  SBE     - Second DW Byte Enable  *
* (15:8) FID     - Function ID            *
* (31:16)ADDRESS - Address0               *
*******************************************/
#define IOSFSB_INDIRECT_ADDR_0_SET(addr0, fid, sbe, fbe) \
        ((addr0) << 16) | (fbe) | ((sbe) << 4) | ((fid) << 8)

/****************************************************************************************
* define SB indirect access header                                                      *
* (7:0)  DEST        - Destination Port ID                                              *
* (15:8) OPCODE      - operation code                                                   *
* (22)   ADDR_LEN    - Address Length: Indicates the length of the address as follows   *
*                                      0 - 16-bit address                               *
*                                      1 - 48-bit address                               *
* (26:24)DATA_LENGTH - Indicates the validity of data fields. Legal values: 0x0 - 0x4   *
* (27)   PC_NPB      - Posted / non-posted indication                                   *
*****************************************************************************************/

#define IOSFSB_INDIRECT_HEADER_SET(destID, opcode)             \
         ((destID ) | ((opcode) << 8) | (((opcode)&1) << 24)| (((opcode)&1) << 27))
 
// Endian Swap :
#define  endian_swap(x)                            \
     ((((x)&0x000000FF)<<24) + (((x)&0x0000FF00)<<8)+  \
      (((x)&0x00FF0000)>>8 ) + (((x)&0xFF000000)>>24)) \

#define IOSFSB_HEADER_BUSY_BIT_OFFSET      0x80000000
#define MAX_TRANSACTION_WAIT_FOR_ACK_TIME  (2000)

/* To open debug/info prints use this macro */     
// #define IOSFSB_DEBUG_OUTPUT_ON
//#define IOSFSB_INFO_OUTPUT_ON
/* Debug */
#ifdef  IOSFSB_DEBUG_OUTPUT_ON
    #define IOSFSB_DEBUG_OUTPUT(fmt, args...) printk("Puma7 IOSFSB Debug (%s): " fmt, __FUNCTION__ , ## args)
#else
    #define IOSFSB_DEBUG_OUTPUT(fmt, args...)
#endif

/* Info */
#ifdef  IOSFSB_INFO_OUTPUT_ON
    #define IOSFSB_INFO_OUTPUT(fmt, args...) printk("Puma7 IOSFSB Info (%s): " fmt, __FUNCTION__ , ## args)
#else
    #define IOSFSB_INFO_OUTPUT(fmt, args...)
#endif

static dev_t iosfsb_dev_t;
static unsigned int count = 1;
static struct cdev *iosfsb_cdev;
static char iosfsbInitStat = NEED_INIT;
static struct class *iosfsb_udev_class;
static struct semaphore IosfsbSem;

/**************************************************************************/
/*            STATIC LOCAL FUNCTIONS DECLERATIONS                         */
/**************************************************************************/
static int iosfsb_down(void);
static int iosfsb_up(void);
static int iosfsb_validateAccessType(unsigned int AccessType, unsigned char *read_opcode, unsigned char *write_opcode);
static unsigned int sendSBMessage(unsigned char destID, unsigned char opcode, 
                                  unsigned short addr0, unsigned int *data, unsigned int addr1);
static long iosfsb_unlocked_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);

/**************************************************************************/
/*              GLOBAL FUNCTIONS                                          */
/**************************************************************************/
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
                              unsigned short addr0, unsigned int data, unsigned int addr1) 
{
    IOSFSB_DEBUG_OUTPUT("Requesting to write to dest-ID: %d, write opcode: %d, address0 = 0x%x, address1 = 0x%x, data = %x\n",
                        destID, writeOpcode, addr0, addr1, data);
    if (iosfsbInitStat != INIT_DONE)
    {
        printk(KERN_ERR "Puma7 IOSFSB: driver was not initialized\n");
        return IOSFSB_FAIL;
    }
    iosfsb_down();
    if (sendSBMessage(destID, writeOpcode, addr0, &data, addr1) != IOSFSB_OK)
    {
        printk(KERN_ERR "Puma7 IOSFSB: fail send SB message\n");
        iosfsb_up();
        return IOSFSB_FAIL;
    }
    iosfsb_up();
    return IOSFSB_OK;
}

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
                              unsigned short addr0, unsigned int * data, unsigned int addr1)
{
    IOSFSB_DEBUG_OUTPUT("requesting to read from dest-ID: %d, read opcode: %d, address0 = 0x%x, address1 = 0x%x\n",
                       destID, readOpcode, addr0, addr1);
    if (iosfsbInitStat != INIT_DONE)
    {
        printk(KERN_ERR "Puma7 IOSFSB: driver was not initialized\n");
        return IOSFSB_FAIL;
    }
    iosfsb_down();
    if (sendSBMessage(destID, readOpcode, addr0, data, addr1) != IOSFSB_OK)
    {
        printk(KERN_ERR "Puma7 IOSFSB: fail send SB message\n");
        iosfsb_up();
        return IOSFSB_FAIL;
    }
    iosfsb_up();
    IOSFSB_DEBUG_OUTPUT("returned data = %x\n",*data);
    return IOSFSB_OK;
}

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
                                  unsigned int data, unsigned int addr1, unsigned int mask) 
{
    unsigned int readData, tmp;

    IOSFSB_DEBUG_OUTPUT("requesting to RMW memory of dest-ID: %d, address0 = 0x%x, address1 = 0x%x, data = %x, mask = %x\n",
                        destID, addr0, addr1, data, mask);
    if (iosfsbInitStat != INIT_DONE)
    {
        printk(KERN_ERR "Puma7 IOSFSB: driver was not initialized\n");
        return IOSFSB_FAIL;
    }
    iosfsb_down();
    if (sendSBMessage(destID, IOSFSB_OP_MEMREAD, addr0, (unsigned int*) &readData, addr1) != IOSFSB_OK)
    {
        printk(KERN_ERR "Puma7 IOSFSB: iosfsb no legal command given\n");
        iosfsb_up();
        return IOSFSB_FAIL;
    }
    tmp = readData & (~mask);
    tmp |= (data & mask);
    if (sendSBMessage(destID, IOSFSB_OP_MEMWRITE, addr0, (unsigned int*) &tmp, addr1) != IOSFSB_OK)
    {
        printk(KERN_ERR "Puma7 IOSFSB: fail send SB message\n");
        iosfsb_up();
        return IOSFSB_FAIL;
    }
    iosfsb_up();
    return IOSFSB_OK;
}

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
                                  unsigned int data, unsigned int addr1, unsigned int mask) 
{
    unsigned int readData = 0;
    unsigned int tmp;
  
  
    IOSFSB_DEBUG_OUTPUT("requesting to RMW register of dest-ID: %d, address0 = 0x%x, address1 = 0x%x, data = %x, mask = %x\n",
                         destID, addr0, addr1, data, mask);
    if (iosfsbInitStat != INIT_DONE)
    {
        printk(KERN_ERR "Puma7 IOSFSB: driver was not initialized\n");
        return IOSFSB_FAIL;
    }
    iosfsb_down();
    if (sendSBMessage(destID, IOSFSB_OP_CRREAD, addr0, &readData, addr1)!= IOSFSB_OK)
    {
        printk(KERN_ERR "Puma7 IOSFSB: fail send SB message\n");
        iosfsb_up();
        return IOSFSB_FAIL;
    }
    tmp = readData & (~mask);
    tmp |= (data & mask);
    if (sendSBMessage(destID, IOSFSB_OP_CRWRITE, addr0, &tmp, addr1)!= IOSFSB_OK)
    {
        printk(KERN_ERR "Puma7 IOSFSB: fail send SB message\n");
        iosfsb_up();
        return IOSFSB_FAIL;
    }
    iosfsb_up();
    return IOSFSB_OK;
}

/**************************************************************************/
/*            STATIC LOCAL FUNCTIONS                                      */
/**************************************************************************/

/**************************************************************************
 * int iosfsb_down(void)                                                  *
 **************************************************************************
 *  DESCRIPTION: This function is used to lock the iosfsb semaphore       *
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
static int iosfsb_down(void)
{
   down(&IosfsbSem);
   return IOSFSB_OK;
}

/**************************************************************************
 *  int iosfsb_up(void)                                                   *
 **************************************************************************
 *  DESCRIPTION: This function is used to release the iosfsb semaphore    *
 *                                                                        *
 *  OUTPUT: fail / success                                                *
 **************************************************************************/
static int iosfsb_up(void)
{
   up(&IosfsbSem);
   return IOSFSB_OK;
}

/**************************************************************************
 * static int iosfsb_validateAccessType(unsigned int AccessType)          *
 **************************************************************************
 *  DESCRIPTION This function is called in order to check if the access   *
 *              type is correct.                                          *
 *  INPUT: unsigned int AccessType                                        *
 *         unsigned char *read_opcode                                     *
 *         unsigned char *write_opcode                                    * 
 *                                                                        * 
 *  OUTPUT: IOSFSB_OK on correct access type  otherwise  IOSFSB_FAIL.     *
 **************************************************************************/
static int iosfsb_validateAccessType(unsigned int AccessType, unsigned char *read_opcode, unsigned char *write_opcode){

    switch (AccessType)
    {
    case IOSFSB_IO_ACCESS:
        *read_opcode = IOSFSB_OP_IOREAD;
        *write_opcode = IOSFSB_OP_IOWRITE;
        break;

    case IOSFSB_MEMORY_ACCESS:
        *read_opcode = IOSFSB_OP_MEMREAD;
        *write_opcode = IOSFSB_OP_MEMWRITE;
        break;;

    case IOSFSB_PCI_CFG_ACCESS :
        *read_opcode = IOSFSB_OP_CFGREAD;
        *write_opcode = IOSFSB_OP_CFGWRITE;
        break;

    case IOSFSB_REG_ACCESS:
        *read_opcode = IOSFSB_OP_CRREAD;
        *write_opcode = IOSFSB_OP_CRWRITE;
        break;

    default:
        printk(KERN_ERR "Puma7 IOSFSB:: received non valid access type %d\n", AccessType);
        return IOSFSB_FAIL;
   }
   IOSFSB_DEBUG_OUTPUT("read opcode is: %d, write opcode is: %d\n",*read_opcode, *write_opcode);
   return IOSFSB_OK;
}

/**************************************************************************
 * unsigned int sendSBMessage(unsigned char destID, unsigned char opcode, 
 *                            unsigned short addr0, unsigned int *data, unsigned int addr1)
 **************************************************************************
 *  DESCRIPTION: This function is to communicate with the                 *
 *               IOSF-SB indirectly                                       *
 *  INPUT: destID  = Destination ID (port ID)                             *
 *         opcode  = Operation code                                       *
 *         addr0   = Remote address of the register (include offset)      *
 *         data    = Pointer to data to write/ read                       *
 *         addr1   = Used only when the address is bigger than 16 bits    *
 *                                                                        *
 *  OUTPUT: Data to read (0 in-cease of write Op)                         *
 **************************************************************************/
static unsigned int sendSBMessage(unsigned char destID, unsigned char opcode, 
                                  unsigned short addr0, unsigned int *data, unsigned int addr1) 
{
    unsigned int address0Data = 0;
    unsigned int loopCount, readVal;
    unsigned int postedIndication = 0;
    unsigned int headerData = 0 ;
    unsigned long flags;
    

#ifdef CONFIG_MACH_PUMA7_FPGA /*Puma7 FPGA doesnt include IOSF*/
    return(IOSFSB_OK);
#endif

    /*See IOSFSB_INDIRECT_ADDR_0_SET definition for farther info 
    FBE = 0xF, SBE = 0, FID =0                                 */
    address0Data = IOSFSB_INDIRECT_ADDR_0_SET(addr0, 0, 0, 0x0F);

    reg_write_32(IOSF2OCP_SB_INDIRECT_ADDR_0,  endian_swap(address0Data));
    reg_write_32(IOSF2OCP_SB_INDIRECT_ADDR_1,  endian_swap(addr1));
    
    /*update data0 register and update posted message is needed only in write opcodes*/
    if(opcode == IOSFSB_OP_CRWRITE || opcode == IOSFSB_OP_CFGWRITE || opcode == IOSFSB_OP_MEMWRITE  || opcode == IOSFSB_OP_IOWRITE) 
    {
        reg_write_32(IOSF2OCP_SB_INDIRECT_DATA_0,  endian_swap(*data));
        /*The ARM11 indirect sideband code MUST use posted writes */
        postedIndication = 1;
    }

    /*See IOSFSB_INDIRECT_HEADER_SET definition for farther info*/
    headerData = IOSFSB_INDIRECT_HEADER_SET(destID, opcode);

    /*disable interrupts */
    local_irq_save(flags);

    reg_write_32(IOSF2OCP_SB_INDIRECT_HEADER,  endian_swap(headerData));

    /* wait for busy bit to be set only in Read operation */
    if (opcode == IOSFSB_OP_CRREAD || opcode == IOSFSB_OP_CFGREAD || opcode == IOSFSB_OP_MEMREAD  || opcode == IOSFSB_OP_IOREAD)
    {
        // Wait for busy bit to be set
        loopCount = 0;
        while (loopCount < MAX_TRANSACTION_WAIT_FOR_ACK_TIME)
        {
            readVal = endian_swap(reg_read_32(IOSF2OCP_SB_INDIRECT_HEADER));
            if ((readVal & IOSFSB_HEADER_BUSY_BIT_OFFSET) != 0)
            {
                break;
            }
            loopCount++;
        }
    }

    /*enable interrupts*/
    local_irq_restore(flags);

    if (loopCount == MAX_TRANSACTION_WAIT_FOR_ACK_TIME)
    {
        printk(KERN_ERR "Puma7 IOSFSB: busy bit fail to set after timeout\n");
        return IOSFSB_FAIL;
    }

    /*delay for write*/
    if(opcode == IOSFSB_OP_CRWRITE || opcode == IOSFSB_OP_CFGWRITE || opcode == IOSFSB_OP_MEMWRITE  || opcode == IOSFSB_OP_IOWRITE) 
    {
        loopCount = 0;
        while (loopCount < MAX_TRANSACTION_WAIT_FOR_ACK_TIME)
        {
            loopCount++;
        }
    }

    // Wait for busy bit to clear
    loopCount = 0;
    while (loopCount < MAX_TRANSACTION_WAIT_FOR_ACK_TIME)
    {
        readVal = endian_swap(reg_read_32(IOSF2OCP_SB_INDIRECT_HEADER));
        if ((readVal & IOSFSB_HEADER_BUSY_BIT_OFFSET) == 0)
        {
            break;
        }
        loopCount++;
    } 

    if (loopCount == MAX_TRANSACTION_WAIT_FOR_ACK_TIME)
    {
        printk(KERN_ERR "Puma7 IOSFSB: busy bit fail to clear after timeout\n");
        return IOSFSB_FAIL;
    }

    /*read data if "read" opcode was received*/
    if(opcode == IOSFSB_OP_CRREAD || opcode == IOSFSB_OP_CFGREAD || opcode == IOSFSB_OP_MEMREAD  || opcode == IOSFSB_OP_IOREAD) 
    {
        *data = endian_swap(reg_read_32(IOSF2OCP_SB_INDIRECT_DATA_0));
    }
    return IOSFSB_OK;
}

/**************************************************************************
 * int iosfsb_drv_open(struct inode *inode, struct file *filp)            *
 **************************************************************************
 *  DESCRIPTION: This function is opens the iosfsb device.                *
 *  INPUT: struct inode *inode - device node pointer                      *
 *         struct file *filp - device file pointer                        *
 *                                                                        *
 *  OUTPUT: IOSFSB_OK on correct access type  otherwise  IOSFSB_FAIL.     *
 **************************************************************************/
static int iosfsb_drv_open(struct inode *inode, struct file *filp)
{

        IOSFSB_INFO_OUTPUT("iosfsb_drv_open\n");
        return IOSFSB_OK;
}

/**************************************************************************
 * int iosfsb_drv_close(struct inode *inode, struct file *filp)           *
 **************************************************************************
 *  DESCRIPTION: This function is closes the iosfsb device.               *
 *  INPUT:  struct inode *inode - device node pointer                     *
 *          struct file *filp - device file pointer                       *
 *  OUTPUT: IOSFSB_OK on correct access type  otherwise  IOSFSB_FAIL.     *
 **************************************************************************/
static int iosfsb_drv_close(struct inode *inode, struct file *filp)
{
        IOSFSB_INFO_OUTPUT("iosfsb_drv_close\n");
        return IOSFSB_OK;
}


/**************************************************************************
 * iosfsb_unlocked_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
 **************************************************************************
 *  DESCRIPTION: This function is activate the the iosfsb device requests.*
 *  INPUT: struct file *filp - the device file pointer                    *
 *         unsigned int cmd - the command to be performed                 *
 *         unsigned long arg - pointer to the user request                *
 *  OUTPUT: long - 0 on success else negative number.                     *
 **************************************************************************/
static long iosfsb_unlocked_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    struct iosfsb_info_user  iosfsb_info;
    int ret = 0;
    unsigned char readOpcode;
    unsigned char writeOpcode;

    /* Check for valid pointer to the parameter list */
    if (0 == arg)
    {
        printk(KERN_ERR "iosfsb arg == 0\n");
        return -EINVAL;
    }
    if (copy_from_user(&iosfsb_info, (void __user *)arg, sizeof(struct iosfsb_info_user)))
    {
        printk(KERN_ERR "iosfsb copy from user failed\n");
        return -EFAULT;
        /* Execute ioctl request */
    }

    IOSFSB_DEBUG_OUTPUT("Received command %d, uset info:\n\t dest port:%d\n\taddress0: 0x%x\n\taddress1: 0x%x\n\tmask: %x\n\tvalue: %x\n\taccess type: %d\n",
                        iosfsb_info.dest_port, iosfsb_info.address0, iosfsb_info.address1, iosfsb_info.mask, iosfsb_info.value, iosfsb_info.access_type);

    if (iosfsb_validateAccessType(iosfsb_info.access_type, &readOpcode,&writeOpcode))
    {
        printk(KERN_ERR "iosfsb access type failed\n");
        return -EINVAL;
    }
    
    switch (cmd)
    {
        case IOSFSB_READ_REG_CMD:
        {
            IOSFSB_DEBUG_OUTPUT("handling IOSFSB_READ_REG_CMD\n");
            IOSFSB_DEBUG_OUTPUT( "Kernel Print: iosfsb_read- destport=%x, address0=%x\n", iosfsb_info.dest_port, iosfsb_info.address0);
            if (iosfsb_read(iosfsb_info.dest_port, readOpcode, 
                                                iosfsb_info.address0 ,&iosfsb_info.value, iosfsb_info.address1) != IOSFSB_OK)
            {
                printk(KERN_ERR "fail send SB message\n");
                ret = -EFAULT;
            }
            if (copy_to_user((void __user *)arg, &iosfsb_info, sizeof(struct iosfsb_info_user)))
            {
                printk(KERN_ERR "fail copy to user\n");
                 ret = -EPERM;
            }
            break;
        }
        case IOSFSB_WRITE_REG_CMD:
        {
            IOSFSB_DEBUG_OUTPUT("handling IOSFSB_WRITE_REG_CMD\n");
            IOSFSB_DEBUG_OUTPUT( "Kernel Print: iosfsb_write- destport=%x, address0=%x, value=%x\n", iosfsb_info.dest_port, iosfsb_info.address0, iosfsb_info.value);
            if (iosfsb_write(iosfsb_info.dest_port, writeOpcode, iosfsb_info.address0 ,iosfsb_info.value , iosfsb_info.address1) != IOSFSB_OK)
            {
                printk(KERN_ERR "fail send SB message\n");
                ret = -EPERM;
            }
            break;
        }
        case IOSFSB_READ_MODIFY_WRITE_CMD:
        {
            IOSFSB_DEBUG_OUTPUT("handling IOSFSB_READ_MODIFY_WRITE_CMD\n");
            IOSFSB_DEBUG_OUTPUT( "Kernel Print: iosfsb_modify- destport=%x, address0=%x, value=%x, mask=%x\n",iosfsb_info.dest_port,iosfsb_info.address0,iosfsb_info.value,iosfsb_info.mask);
            if (iosfsb_info.access_type == IOSFSB_REG_ACCESS)
            {
                if (iosfsb_regReadModifyWrite(iosfsb_info.dest_port, iosfsb_info.address0, iosfsb_info.value,
                                              iosfsb_info.address1, iosfsb_info.mask) != IOSFSB_OK)
                {
                    printk(KERN_ERR "fail send SB message\n");
                    ret = -EPERM;
                }
            }
            else if (iosfsb_info.access_type == IOSFSB_MEMORY_ACCESS)
            {
                if (iosfsb_memReadModifyWrite(iosfsb_info.dest_port, iosfsb_info.address0, iosfsb_info.value,
                                              iosfsb_info.address1, iosfsb_info.mask) != IOSFSB_OK)
                {
                    printk(KERN_ERR "fail send SB message\n");
                    ret = -EPERM;
                }
            }
            break;
        }
        default:
        {  
            printk(KERN_ERR "iosfsb no legal command given\n");
            ret = -ENOSYS;
            break;
        }
    }
    return ret;
}


/* Structure to map driver functions to kernel */
struct file_operations iosfsb_drv_fops = {
        .owner   = THIS_MODULE,
        .unlocked_ioctl   = iosfsb_unlocked_ioctl,
        .open    = iosfsb_drv_open,
        .release = iosfsb_drv_close,
};


/**************************************************************************
 * int __init iosfsb_drv_init(void)                                       *
 **************************************************************************
 *  DESCRIPTION: This function is the iosfsb device module init function. *
 *  OUTPUT:  long - 0 on success else negative number.                    *
 **************************************************************************/
static int __init iosfsb_drv_init(void)
{

    IOSFSB_INFO_OUTPUT("IOSF-SB: Initializing Intel(R) IOSF-SB driver\n");

    if (alloc_chrdev_region(&iosfsb_dev_t, 0, count, DEV_NAME) < 0)
    { /*count indicates how many minors we get*/
        printk(KERN_ERR "\nIOSFSB Failed to register character device region %s\n",DEV_NAME);
        return IOSFSB_FAIL;
    }

    if (!(iosfsb_cdev = cdev_alloc()))
    {
        printk(KERN_ERR "\nIOSFSB Failed in cdev_alloc %s\n",DEV_NAME);
        unregister_chrdev_region(iosfsb_dev_t, count);
        return IOSFSB_FAIL;
    }
    /* Connect the file operations with the cdev */
    cdev_init(iosfsb_cdev, &iosfsb_drv_fops);
    /* Connect the major/minor number to the cdev  - Activates the device*/
    if (cdev_add(iosfsb_cdev, iosfsb_dev_t, count) < 0)
    {
        printk(KERN_ERR "\nIOSFSB Failed in add character device %s\n",DEV_NAME);
        cdev_del(iosfsb_cdev);
        unregister_chrdev_region(iosfsb_dev_t, count);
        return IOSFSB_FAIL;
    }
    /* connection to the udev */
    /* creates a class directory under /sys/class */
    iosfsb_udev_class = class_create(THIS_MODULE, "iosfsb_class");
    /* creates a class directory under /sys/class/DEV_NAME named DEV_NAME  */
    /* creates 3 file: dev, uevent, subsystem*/
    device_create(iosfsb_udev_class, NULL, iosfsb_dev_t, NULL, "%s", "docsis_iosfsb_drv");

    IOSFSB_DEBUG_OUTPUT("IOSF-SB: Create device: /dev/%s\n",DEV_NAME);

    sema_init(&IosfsbSem,1);

    iosfsbInitStat = INIT_DONE;
	
#if defined(CONFIG_MACH_PUMA7) && defined(CONFIG_VOICE_VCODEC_INIT_CONFIG_SSI)
    /* Enabling SSI interface */
    printk(KERN_ERR "\nIOSFSB: configuring SSI interface ..\n");
    IO_CTRL_configSSI();
#endif

    return IOSFSB_OK;

}


/**************************************************************************
 * void __exit iosfsb_drv_exit(void)                                      *
 **************************************************************************
 * DESCRIPTION: This function is the iosfsb device module exit function.  *
 **************************************************************************/
static void __exit iosfsb_drv_exit(void)
{
    if (iosfsb_cdev)
    {
        cdev_del(iosfsb_cdev);
    }
    unregister_chrdev_region(iosfsb_dev_t, count);

    IOSFSB_DEBUG_OUTPUT("\nIOSFSB device unregistered\n");

    device_destroy(iosfsb_udev_class, iosfsb_dev_t);
    class_destroy(iosfsb_udev_class);
}
/*************************************************************************/

module_init(iosfsb_drv_init);
module_exit(iosfsb_drv_exit);

EXPORT_SYMBOL(iosfsb_write);
EXPORT_SYMBOL(iosfsb_read);
EXPORT_SYMBOL(iosfsb_memReadModifyWrite);
EXPORT_SYMBOL(iosfsb_regReadModifyWrite);

/* Driver identification */
MODULE_DESCRIPTION("IOSFSB Device Driver");
MODULE_LICENSE("GPL");




