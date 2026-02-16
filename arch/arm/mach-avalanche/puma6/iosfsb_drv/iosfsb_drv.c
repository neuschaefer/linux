/*
 *
 * iosfsb_drv.c
 * Description:
 * iosfsb driver
 *
 *  GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2011-2014 Intel Corporation. All rights reserved.
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


/************************************************************************/
/*  NOTE: the input and the output of the method is in BE               */
/*  ARM side                                             ATOM side      */
/* 00000002 -method convert-> 02000000 - bus switches -> 00000002       */
/************************************************************************/

#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/version.h>
#include <linux/module.h>   /* for modules */
#include <linux/fs.h>       /* file_operations */
#include <linux/uaccess.h>  /* copy_(to,from)_user */
#include <linux/init.h>     /* module_init, module_exit */
#include <linux/cdev.h>     /* cdev utilities */
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/semaphore.h>
#include <linux/ioctl.h>

#include <mach/puma.h>

#ifdef CONFIG_MACH_PUMA6 /* For Puma-6 SoC */
#include "aep_iosfsb.h"
#endif

#include "iosfsb_api.h"



#define IOSFSB_OK                 (0)
#define IOSFSB_FAIL               (-1)




#define DEV_NAME  "docsis_iosfsb_drv"
#define IOSFSB_ACK_BIT_SET                  (0x01000000)
#define IOSFSB_BYTE_ENABLE_AND_OP_TRIGGER   (0x1f)

typedef int (*unlocked_iosfsb_write_t) ( unsigned char iosfsb_port,unsigned int iosfsb_addr, unsigned int iosfsb_data, unsigned int iosfsb_write_opcode);
typedef int (*unlocked_iosfsb_read_t) (unsigned char iosfsb_port,unsigned int iosfsb_addr,unsigned int *iosfsb_read_data,unsigned int iosfsb_read_opcode);

static int unlocked_iosfsb_write_default ( unsigned char iosfsb_port,unsigned int iosfsb_addr, unsigned int iosfsb_data, unsigned int iosfsb_write_opcode);
static int unlocked_iosfsb_read_default (unsigned char iosfsb_port,unsigned int iosfsb_addr,unsigned int *iosfsb_read_data,unsigned int iosfsb_read_opcode);

static dev_t iosfsb_dev_t;
static unsigned int count = 1;
static struct cdev *iosfsb_cdev;
static unsigned int ref = 0;
static struct class *iosfsb_udev_class;
static struct semaphore IosfsbSem;
static unlocked_iosfsb_write_t unlocked_iosfsb_write_cb = unlocked_iosfsb_write_default;
static unlocked_iosfsb_read_t  unlocked_iosfsb_read_cb  = unlocked_iosfsb_read_default;


/*------------------ defines -------------------------------------------------*/
#define IOSFSB_PORT_OK                (0)
#define IOSFSB_PORT_INCORRECT         (-1)
#define PRINT_READ                    (0)
#define PRINT_WRITE                   (1)
#define IOSFSB_CFG_BASE         (AVALANCHE_IOSFSB_BASE)
#define IOSFSB_CFG_MSG_REQ      (0x000003c0)
#define IOSFSB_CFG_MSG_ADDR     (0x000003c4)
#define IOSFSB_CFG_MSG_DATA     (0x000003c8)
#define IOSFSB_CFG_MSG_ACK      (0x000003cc)
#define IOSFSB_CFG_MSG_IN_DATA  (0x000003d0)
#define MAX_TRANSACTION_WAIT_FOR_ACK_TIME       (2000)

/* Reg Read / Write Macros */
#define reg_write_32(addr, data) (( *(volatile unsigned int *) (addr) ) = data)
#define reg_read_32(addr)        ( *(volatile unsigned int *) (addr) )
#define IOSFSB_CONVERT_FROM_32LE(le_value)     (le32_to_cpu(le_value))
#define IOSFSB_CONVERT_CPU_TO_32LE(be_value)   (cpu_to_le32(be_value))

//#define __IOSFSB_DEBUG

#ifdef __IOSFSB_DEBUG
/**************************************************************************/
/*  basic routines                                                        */
/**************************************************************************/
static void iosfsb_printInfo ( unsigned int addr, unsigned int Value, unsigned short ReadWrite)
{

    /*printk(KERN_INFO "write to address=%x, value=%x\n",addr,Value);*/


    if (ReadWrite == PRINT_WRITE)
    {
        printk(KERN_DEBUG "write to address=%x, value=%x\n",addr,Value);
    }
    if (ReadWrite == PRINT_READ)
    {
        printk(KERN_DEBUG "Read from address=%x, value=%x\n",addr,Value);
    }
}
#else
static void iosfsb_printInfo ( unsigned int addr, unsigned int Value,  unsigned short ReadWrite)
{
    return;
}
#endif

/**************************************************************************/
/*! \fn  int iosfsb_down();
 **************************************************************************
 *  \brief This function is used to lock the iosfsb semaphore
 *  \return . fail / success
 **************************************************************************/
int iosfsb_down()
{
   down(&IosfsbSem);
   return IOSFSB_OK;
}

/**************************************************************************/
/*! \fn  int iosfsb_up();
 **************************************************************************
 *  \brief This function is used to release the iosfsb semaphore
 *  \return . fail / success
 **************************************************************************/
int iosfsb_up()
{
   up(&IosfsbSem);
   return IOSFSB_OK;
}

/**************************************************************************/
/*! \fn  static int unlocked_iosfsb_write_default ( unsigned char iosfsb_port,unsigned int iosfsb_addr, unsigned int iosfsb_data, unsigned int iosfsb_write_opcode);
 **************************************************************************
 *  \brief This function is a default function. Normally it not in used.
 *  If this function is being called, then it indicate on a fatal error.
 *  The IOSF-SB is being called without initialization.
 *  input:
 *  unsigned char iosfsb_port = port id
 *  unsigned int iosfsb_addr = register address
 *  unsigned int iosfsb_data = register new value
 *  unsigned int iosfsb_write_opcode = iosf operation opcode
 *  \return . fail / success
 **************************************************************************/
static int unlocked_iosfsb_write_default ( unsigned char iosfsb_port,unsigned int iosfsb_addr, unsigned int iosfsb_data, unsigned int iosfsb_write_opcode)
{
    printk(KERN_ERR "IOSF-SB: unlocked_iosfsb_write_default is called without initialization.\n");
    return IOSFSB_FAIL;
}

/**************************************************************************/
/*! \fn  static int unlocked_iosfsb_write_direct ( unsigned char iosfsb_port,unsigned int iosfsb_addr, unsigned int iosfsb_data, unsigned int iosfsb_write_opcode);
 **************************************************************************
 *  \brief This function is used to write iosfsb register value, directly
 *  input:
 *  unsigned char iosfsb_port = port id
 *  unsigned int iosfsb_addr = register address
 *  unsigned int iosfsb_data = register new value
 *  unsigned int iosfsb_write_opcode = iosf operation opcode
 *  \return . fail / success
 **************************************************************************/
static int unlocked_iosfsb_write_direct ( unsigned char iosfsb_port,unsigned int iosfsb_addr, unsigned int iosfsb_data, unsigned int iosfsb_write_opcode)
{
    volatile unsigned int WriteVal_le;
    unsigned short LoopCount;
    unsigned int AckVal;

   /* Configure the SAP-to-IOSFSB address CSR for an IOSFSB write */
   WriteVal_le = IOSFSB_CONVERT_CPU_TO_32LE(iosfsb_addr);
   iosfsb_printInfo(IOSFSB_CFG_BASE + IOSFSB_CFG_MSG_ADDR,WriteVal_le, PRINT_WRITE);
   reg_write_32 (IOSFSB_CFG_BASE + IOSFSB_CFG_MSG_ADDR, WriteVal_le);

   /* Configure the SAP-to-IOSFSB data CSR for an IOSFSB write */
   WriteVal_le = IOSFSB_CONVERT_CPU_TO_32LE(iosfsb_data);
   iosfsb_printInfo(IOSFSB_CFG_BASE + IOSFSB_CFG_MSG_DATA,WriteVal_le, PRINT_WRITE);
   reg_write_32 (IOSFSB_CFG_BASE + IOSFSB_CFG_MSG_DATA, WriteVal_le);

   /* BE
   WriteVal_le = 0x00000000;
   WriteVal_le |= (0x1f << 16);
   WriteVal_le |= (iosfsb_port << 8);
   WriteVal_le |= 0xe0;
   */

   /* Initiate the IOSFSB write transaction*/
   WriteVal_le = 0x00000000;
   WriteVal_le |= (IOSFSB_BYTE_ENABLE_AND_OP_TRIGGER << 8);
   WriteVal_le |= (iosfsb_port << 16);
   WriteVal_le |= (iosfsb_write_opcode << 24);

   /*
    Bits 31:21 reserved
    Bit  20    MSG_OUT_AVAIL  - 1'b1 triggers the transaction
    Bits 19:16 MSG_OUT_BE     - 0xf
    Bits 15:8  MSG_OUT_PORT   - e.g., port id
    Bits 7:0   MSG_OUT_OPCODE - 0xe0 for RegWr
   */
   iosfsb_printInfo(IOSFSB_CFG_BASE + IOSFSB_CFG_MSG_REQ,WriteVal_le, PRINT_WRITE);
   reg_write_32 (IOSFSB_CFG_BASE + IOSFSB_CFG_MSG_REQ, WriteVal_le);

   /*
    Poll the MSG_ACK CSR, waiting for the LSB bit to be set
    t_data = {8'h00, 8'h00, 8'h00, 8'h01};
    axi_seq_poll (IOSFSB_CFG_BASE + IOSFSB_CFG_MSG_ACK, t_data);
    While waiting for AXI BFM support to implement
    the axi_seq_poll task, use a fixed delay here
   */
   for (LoopCount = 0;LoopCount < MAX_TRANSACTION_WAIT_FOR_ACK_TIME; LoopCount++)
   {
       /*AckVal = IOSFSB_CONVERT_FROM_32LE(reg_read_32(IOSFSB_CFG_BASE + IOSFSB_CFG_MSG_ACK));*/
       AckVal = reg_read_32(IOSFSB_CFG_BASE + IOSFSB_CFG_MSG_ACK);
       if (AckVal)
       {
           if (AckVal &(~((unsigned int)IOSFSB_ACK_BIT_SET)) )
           {
               printk(KERN_ERR "iosfsb_write:: ACK is %x.\n",AckVal);
           }
           return IOSFSB_OK;
       }
   }
   printk(KERN_ERR "iosfsb_write:: fail to write NO ACK received.\n");
   return IOSFSB_FAIL;
}

#ifdef CONFIG_MACH_PUMA6 /* For Puma-6 SoC */
/**************************************************************************/
/*! \fn  static int unlocked_iosfsb_write_aep ( unsigned char iosfsb_port,unsigned int iosfsb_addr, unsigned int iosfsb_data, unsigned int iosfsb_write_opcode);
 **************************************************************************
 *  \brief This function is used to write iosfsb register value, through the AEP.
 *  input:
 *  unsigned char iosfsb_port = port id
 *  unsigned int iosfsb_addr = register address
 *  unsigned int iosfsb_data = register new value
 *  unsigned int iosfsb_write_opcode = iosf operation opcode
 *  \return . fail / success
 **************************************************************************/
static int unlocked_iosfsb_write_aep ( unsigned char iosfsb_port,unsigned int iosfsb_addr, unsigned int iosfsb_data, unsigned int iosfsb_write_opcode)
{
    if (aep_ipc_iosfsb_write(iosfsb_port,iosfsb_write_opcode,iosfsb_addr,iosfsb_data) != 0 )
    {
        printk(KERN_ERR "aep_ipc_iosfsb_write: failed.\n");
        return IOSFSB_FAIL;
    }

    return IOSFSB_OK;
}
#endif

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
int iosfsb_unprotected_write ( unsigned char iosfsb_port,unsigned int iosfsb_addr, unsigned int iosfsb_data, unsigned int iosfsb_write_opcode)
{
    int ret;
    ret = unlocked_iosfsb_write_cb ( iosfsb_port,iosfsb_addr, iosfsb_data, iosfsb_write_opcode);
    return ret;
}

/**************************************************************************/
/*! \fn  int iosfsb_write ( unsigned char iosfsb_port,unsigned int iosfsb_addr, unsigned int iosfsb_data, unsigned int iosfsb_write_opcode);
 **************************************************************************
 *  \brief This function is used to write iosfsb register value
 *  input:
 *  unsigned char iosfsb_port = port id
 *  unsigned int iosfsb_addr = register address
 *  unsigned int iosfsb_data = register new value
 *  unsigned int iosfsb_write_opcode = iosf operation opcode
 *  \return . fail / success
 **************************************************************************/
int iosfsb_write ( unsigned char iosfsb_port,unsigned int iosfsb_addr, unsigned int iosfsb_data, unsigned int iosfsb_write_opcode)
{
    int ret;

    down(&IosfsbSem);
    ret = unlocked_iosfsb_write_cb ( iosfsb_port,iosfsb_addr, iosfsb_data, iosfsb_write_opcode);
    up(&IosfsbSem);
    return ret;
}


/**************************************************************************/
/*! \fn  static int unlocked_iosfsb_read_default ( unsigned char iosfsb_port,unsigned int iosfsb_addr, unsigned int iosfsb_data,unsigned int iosfsb_read_opcode);
 **************************************************************************
 *  \brief This function is a default function. Normally it not in used.
 *  If this function is being called, then it indicate on a fatal error.
 *  The IOSF-SB is being called without initialization.
 *  input:
 *  unsigned char iosfsb_port = port id
 *  unsigned int iosfsb_addr = register address
 *  unsigned int iosfsb_read_data = register value
 *  unsigned int iosfsb_read_opcode = iosf operation opcode
 *  \return . fail / success
 **************************************************************************/

static int unlocked_iosfsb_read_default (unsigned char iosfsb_port,unsigned int iosfsb_addr,unsigned int *iosfsb_read_data,unsigned int iosfsb_read_opcode)
{
    printk(KERN_ERR "IOSF-SB: unlocked_iosfsb_read_default is called without initialization.\n");
    return IOSFSB_FAIL;
}


/**************************************************************************/
/*! \fn  static int unlocked_iosfsb_read_direct ( unsigned char iosfsb_port,unsigned int iosfsb_addr, unsigned int iosfsb_data,unsigned int iosfsb_read_opcode);
 **************************************************************************
 *  \brief This function is used to read iosf register value, directly.
 *  input:
 *  unsigned char iosfsb_port = port id
 *  unsigned int iosfsb_addr = register address
 *  unsigned int iosfsb_read_data = register value
 *  unsigned int iosfsb_read_opcode = iosf operation opcode
 *  \return . fail / success
 **************************************************************************/
static int unlocked_iosfsb_read_direct (unsigned char iosfsb_port,unsigned int iosfsb_addr,unsigned int *iosfsb_read_data,unsigned int iosfsb_read_opcode)
{

   volatile unsigned int WriteVal_le;
   unsigned short LoopCount;
   unsigned int AckVal;

   /* Configure the SAP-to-IOSFSB address CSR for an IOSFSB read */
   WriteVal_le = IOSFSB_CONVERT_CPU_TO_32LE(iosfsb_addr);
   iosfsb_printInfo(IOSFSB_CFG_BASE + IOSFSB_CFG_MSG_ADDR,WriteVal_le, PRINT_WRITE);
   reg_write_32 (IOSFSB_CFG_BASE + IOSFSB_CFG_MSG_ADDR, WriteVal_le);

   /*
   WriteVal_le = 0x00000000;
   WriteVal_le |= (0x1f << 16);
   WriteVal_le |= (iosfsb_port << 8);
   WriteVal_le |= 0xd0;
   */

   /* Initiate the IOSFSB read transaction */
   WriteVal_le = 0x00000000;
   WriteVal_le |= (IOSFSB_BYTE_ENABLE_AND_OP_TRIGGER << 8);
   WriteVal_le |= (iosfsb_port << 16);
   WriteVal_le |= (iosfsb_read_opcode << 24);
   /* Bits 31:21 reserved
      Bit  20    MSG_OUT_AVAIL  - 1'b1 triggers the transaction
      Bits 19:16 MSG_OUT_BE     - 0xf
      Bits 15:8  MSG_OUT_PORT   - e.g., port id
      Bits 7:0   MSG_OUT_OPCODE - 0xd0 for RegRd
                 (see design/shared_cells/rtl/sb2mbus/s2m_op_map.v)
    */
   iosfsb_printInfo(IOSFSB_CFG_BASE + IOSFSB_CFG_MSG_REQ,WriteVal_le, PRINT_WRITE);
   reg_write_32 (IOSFSB_CFG_BASE + IOSFSB_CFG_MSG_REQ, WriteVal_le);

   /*
    Poll the MSG_ACK CSR, waiting for the LSB bit to be set
    t_data = {(unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x01};
    axi_seq_poll (IOSFSB_CFG_BASE + IOSFSB_CFG_MSG_ACK, t_data);
    While waiting for AXI BFM support to implement
    the axi_seq_poll task, use a fixed delay here
   */
   LoopCount = 0;
   while (LoopCount < MAX_TRANSACTION_WAIT_FOR_ACK_TIME)
   {

       /*AckVal = IOSFSB_CONVERT_FROM_32LE(reg_read_32(IOSFSB_CFG_BASE + IOSFSB_CFG_MSG_ACK));*/
       AckVal = reg_read_32(IOSFSB_CFG_BASE + IOSFSB_CFG_MSG_ACK);
       if (AckVal)
       {
           if (AckVal &(~((unsigned int)IOSFSB_ACK_BIT_SET)) )
           {
               printk(KERN_ERR "iosfsb_read:: ACK is %x.\n",AckVal);
           }
           break;
       }
       ++LoopCount;

   }
   if (LoopCount == MAX_TRANSACTION_WAIT_FOR_ACK_TIME)
   {
       printk(KERN_ERR "iosfsb_read:: fail to read NO ACK received.\n");
       return IOSFSB_FAIL;
   }

   /* Read back the CSR data as captured in the IOSFSB_CFG_MSG_IN_DATA register*/
   (*iosfsb_read_data) = IOSFSB_CONVERT_FROM_32LE(reg_read_32(IOSFSB_CFG_BASE + IOSFSB_CFG_MSG_IN_DATA));
   iosfsb_printInfo(IOSFSB_CFG_BASE + IOSFSB_CFG_MSG_IN_DATA,(*iosfsb_read_data), PRINT_READ);
   return IOSFSB_OK;
}

#ifdef CONFIG_MACH_PUMA6 /* For Puma-6 SoC */
/**************************************************************************/
/*! \fn  static int unlocked_iosfsb_read_aep ( unsigned char iosfsb_port,unsigned int iosfsb_addr, unsigned int iosfsb_data,unsigned int iosfsb_read_opcode);
 **************************************************************************
 *  \brief This function is used to read iosf register value, through the AEP.
 *  input:
 *  unsigned char iosfsb_port = port id
 *  unsigned int iosfsb_addr = register address
 *  unsigned int iosfsb_read_data = register value
 *  unsigned int iosfsb_read_opcode = iosf operation opcode
 *  \return . fail / success
 **************************************************************************/

static int unlocked_iosfsb_read_aep (unsigned char iosfsb_port,unsigned int iosfsb_addr,unsigned int *iosfsb_read_data,unsigned int iosfsb_read_opcode)
{
    //printk("unlocked_iosfsb_read_aep\n");
    if (aep_ipc_iosfsb_read(iosfsb_port,iosfsb_read_opcode,iosfsb_addr,iosfsb_read_data) != 0 )
    {
        printk(KERN_ERR "aep_ipc_iosfsb_write: failed.\n");
        return IOSFSB_FAIL;
    }

    return IOSFSB_OK;
}
#endif

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
int iosfsb_unprotected_read (unsigned char iosfsb_port,unsigned int iosfsb_addr,unsigned int *iosfsb_read_data,unsigned int iosfsb_read_opcode)
{
   int ret;
   //printk("iosfsb_unprotected_read\n");
   ret = unlocked_iosfsb_read_cb (iosfsb_port,iosfsb_addr,iosfsb_read_data,iosfsb_read_opcode);
   return ret;
}


/**************************************************************************/
/*! \fn  int iosfsb_read ( unsigned char iosfsb_port,unsigned int iosfsb_addr, unsigned int *iosfsb_data,unsigned int iosfsb_read_opcode);
 **************************************************************************
 *  \brief This function is used to read iosf register value
 *  input:
 *  unsigned char iosfsb_port = port id
 *  unsigned int iosfsb_addr = register address
 *  unsigned int iosfsb_read_data = register value
 *  unsigned int iosfsb_read_opcode = iosf operation opcode
 *  \return . fail / success
 **************************************************************************/
int iosfsb_read (unsigned char iosfsb_port,unsigned int iosfsb_addr,unsigned int *iosfsb_read_data,unsigned int iosfsb_read_opcode)
{
   int ret;

   //printk("iosfsb_read: iosfsb_read_data(before)\n");
   down(&IosfsbSem);
   ret = unlocked_iosfsb_read_cb (iosfsb_port,iosfsb_addr,iosfsb_read_data,iosfsb_read_opcode);
   up(&IosfsbSem);
   //printk("iosfsb_read: iosfsb_read_data (after)\n",);
   return ret;
}

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
int iosfsb_mem_read_modify_write ( unsigned char iosfsb_port,unsigned int iosfsb_addr, unsigned int mask, unsigned int iosfsb_data)
{
   unsigned int iosfsb_read_data, tmp;

    down(&IosfsbSem);
    if (unlocked_iosfsb_read_cb (iosfsb_port,iosfsb_addr, &iosfsb_read_data,IOSFSB_MEMORY_READ))
    {
         printk(KERN_ERR "iosfsb_mem_read_modify_write:: fail to read.\n");
         up(&IosfsbSem);
         return IOSFSB_FAIL;
    }
    tmp = iosfsb_read_data & (~mask);
    tmp |= (iosfsb_data & mask);
    if (unlocked_iosfsb_write_cb (iosfsb_port,iosfsb_addr,tmp,IOSFSB_MEMORY_WRITE))
    {
         printk(KERN_ERR "iosfsb_mem_read_modify_write:: fail to write.\n");
         up(&IosfsbSem);
         return IOSFSB_FAIL;
    }
    up(&IosfsbSem);
    return IOSFSB_OK;
}

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
int iosfsb_reg_read_modify_write ( unsigned char iosfsb_port,unsigned int iosfsb_addr, unsigned int mask, unsigned int iosfsb_data)
{
   unsigned int iosfsb_read_data, tmp;

    down(&IosfsbSem);
    if (unlocked_iosfsb_read_cb (iosfsb_port,iosfsb_addr, &iosfsb_read_data,IOSFSB_REG_READ))
    {
         printk(KERN_ERR "iosfsb_reg_read_modify_write:: fail to read.\n");
         up(&IosfsbSem);
         return IOSFSB_FAIL;
    }
    tmp = iosfsb_read_data & (~mask);
    tmp |= (iosfsb_data & mask);
    if (unlocked_iosfsb_write_cb (iosfsb_port,iosfsb_addr,tmp,IOSFSB_REG_WRITE))
    {
         printk(KERN_ERR "iosfsb_reg_read_modify_write:: fail to write.\n");
         up(&IosfsbSem);
         return IOSFSB_FAIL;
    }
    up(&IosfsbSem);
    return IOSFSB_OK;
}


/**************************************************************************/
/*! \fn static int  validate_AccessType(unsigned int AccessType)
 **************************************************************************
 *  \brief This function is called in order to check if the access type is correct.
 *  \param unsigned int AccessType
 *  \param unsigned char *read_opcode
 *  \param unsigned char *write_opcode
 *  \return IOSFSB_OK on correcet access type  otherwise  IOSFSB_FAIL.
 **************************************************************************/
static int validate_AccessType(unsigned int AccessType, unsigned char *read_opcode, unsigned char *write_opcode){

    switch (AccessType)
    {
    case IOSFSB_IO_ACCESS:
        *read_opcode = IOSFSB_IO_READ;
        *write_opcode = IOSFSB_IO_WRITE;
        return IOSFSB_OK;

    case IOSFSB_MEMORY_ACCESS:
        *read_opcode = IOSFSB_MEMORY_READ;
        *write_opcode = IOSFSB_MEMORY_WRITE;
        return IOSFSB_OK;

    case IOSFSB_PCI_CFG_ACCESS :
        *read_opcode = IOSFSB_PCI_CFG_READ;
        *write_opcode = IOSFSB_PCI_CFG_WRITE;
        return IOSFSB_OK;

    case IOSFSB_REG_ACCESS:
        *read_opcode = IOSFSB_REG_READ;
        *write_opcode = IOSFSB_REG_WRITE;
        return IOSFSB_OK;

    default:
        return IOSFSB_FAIL;
   }
   return IOSFSB_FAIL;
}


/**************************************************************************/
/*! \fn static int  validate_iosfsb_port(unsigned int iosfsb_port)
 **************************************************************************
 *  \brief This function is called in order to check that the port id is correct docsis port.
 *  \return IOSFSB_PORT_OK on port ok  otherwise  IOSFSB_PORT_INCORRECT.
 **************************************************************************/
static int validate_iosfsb_port(unsigned int iosfsb_port){

    switch (iosfsb_port)
    {
        case IOSFSB_VTUNIT_PORT:
        case IOSFSB_DOCSIS_MODPHY_AFE_PORT:
        case IOSFSB_DOCSIS_TX_DAC_PORT:
        case IOSFSB_DOCSIS_NB_ADC_PORT :
        case IOSFSB_DOCSIS_IOSFSB_MASTER_PORT:
        case IOSFSB_BBU_ADC_PORT:
        case IOSFSB_CPUNIT_PORT:
        case IOSFSB_PUNIT_PORT:
        case IOSFSB_MOCA_MODPHY_AFE_PORT:
        case IOSFSB_PCIE_MODPHY_AFE_PORT:
        case IOSFSB_SATA_MODPHY_AFE_PORT:
            return IOSFSB_PORT_OK;
    default:
            printk(KERN_INFO "iosfsb port illegal \n");
           return IOSFSB_PORT_OK;
   }
   printk(KERN_INFO "iosfsb port illegal \n");
   return IOSFSB_PORT_OK;
}


/**************************************************************************/
/*! \fn static int iosfsb_drv_open(struct inode *inode, struct file *filp)
 **************************************************************************
 *  \brief This function is opens the iosfsb device.
 *  \param struct inode *inode - device node pointer
 *  \param struct file *filp - device file pointer
 *  \return int - IOSFSB_OK on correcet access type  otherwise  IOSFSB_FAIL.
 **************************************************************************/
static int iosfsb_drv_open(struct inode *inode, struct file *filp)
{

        printk(KERN_INFO "iosfsb_drv_open: ref %d\n", ++ref);
        return IOSFSB_OK;
}

/**************************************************************************/
/*! \fn static int iosfsb_drv_close(struct inode *inode, struct file *filp)
 **************************************************************************
 *  \brief This function is closes the iosfsb device.
 *  \param struct inode *inode - device node pointer
 *  \param struct file *filp - device file pointer
 *  \return int - IOSFSB_OK on correcet access type  otherwise  IOSFSB_FAIL.
 **************************************************************************/
static int iosfsb_drv_close(struct inode *inode, struct file *filp)
{
        printk(KERN_INFO "iosfsb_drv_close: ref %d\n", --ref);
        return IOSFSB_OK;
}


/**************************************************************************/
/*! \fn static long handlerIosfsbRequests( unsigned int cmd, unsigned long arg)
 **************************************************************************
 *  \brief This function is activate the the iosfsb device requests.
 *  \param unsigned int cmd - the command to be performed
 *  \param unsigned long arg - pointer to the user request
 *  \return long - 0 on success else negative number.
 **************************************************************************/
static long handlerIosfsbRequests( unsigned int cmd, unsigned long arg)
{
    struct iosfsb_info_user  iosfsb_info;
    int ret = 0;
    unsigned char iosfsb_read_opcode;
    unsigned char iosfsb_write_opcode;

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
    if (validate_AccessType(iosfsb_info.access_type, &iosfsb_read_opcode,&iosfsb_write_opcode)) {
        printk(KERN_ERR "iosfsb access type failed\n");
        return -EINVAL;
    }
    if (validate_iosfsb_port(iosfsb_info.dest_port))
    {
        printk(KERN_ERR "iosfsb validate port failed\n");
        return -EINVAL;
    }
    switch (cmd)
    {
        /*---------------------------------------------------------------------------*/
    case IOSFSB_READ_REG_CMD:
            printk(KERN_DEBUG "iosfsb_read- destport=%x, offset=%x\n",iosfsb_info.dest_port,iosfsb_info.offset);
            if (iosfsb_read(iosfsb_info.dest_port, iosfsb_info.offset,&iosfsb_info.value,iosfsb_read_opcode) == IOSFSB_OK)
            {
                if (copy_to_user((void __user *)arg, &iosfsb_info, sizeof(struct iosfsb_info_user)))
                {
                     ret = -EFAULT;
                }
            }
            else
            {
                ret = -ENOSYS;
            }
            break;
        /*---------------------------------------------------------------------------*/
    case IOSFSB_WRITE_REG_CMD:
            printk(KERN_DEBUG "iosfsb_write- destport=%x, offset=%x, value=%x\n",iosfsb_info.dest_port,iosfsb_info.offset,iosfsb_info.value);
            if (iosfsb_write(iosfsb_info.dest_port, iosfsb_info.offset,iosfsb_info.value, iosfsb_write_opcode) == IOSFSB_FAIL)
            {
                ret = -ENOSYS;
            }
            break;
        /*---------------------------------------------------------------------------*/
    case IOSFSB_READ_MODIFY_WRITE_CMD:
            printk(KERN_DEBUG "iosfsb_modify- destport=%x, offset=%x, value=%x, mask=%x\n",iosfsb_info.dest_port,iosfsb_info.offset,iosfsb_info.value,iosfsb_info.mask);
            if (iosfsb_info.access_type == IOSFSB_REG_ACCESS)
            {
                if (iosfsb_reg_read_modify_write(iosfsb_info.dest_port, iosfsb_info.offset, iosfsb_info.mask, iosfsb_info.value) == IOSFSB_FAIL)
                {
                    ret = -ENOSYS;
                }
            }
            else{
                if (iosfsb_info.access_type == IOSFSB_MEMORY_ACCESS)
                {
                    if (iosfsb_mem_read_modify_write(iosfsb_info.dest_port, iosfsb_info.offset, iosfsb_info.mask, iosfsb_info.value) == IOSFSB_FAIL)
                    {
                        ret = -ENOSYS;
                    }
                }
            }
            break;
        /*---------------------------------------------------------------------------*/
    default:
            printk(KERN_ERR "iosfsb no legal command given\n");
            ret = -ENOSYS;
            break;
        }
    return ret;
}


/**************************************************************************/
/*! \fn static long iosfsb_unlocked_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
 **************************************************************************
 *  \brief This function is activate the the iosfsb device requests.
 *  \param struct file *filp - the device file pointer
 *  \param unsigned int cmd - the command to be performed
 *  \param unsigned long arg - pointer to the user request
 *  \return long - 0 on success else negative number.
 **************************************************************************/
static long iosfsb_unlocked_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    return (handlerIosfsbRequests(cmd,arg));
}


/* Structure to map driver functions to kernel */
struct file_operations iosfsb_drv_fops = {
        .owner   = THIS_MODULE,
        .unlocked_ioctl   = iosfsb_unlocked_ioctl,
        .open    = iosfsb_drv_open,
        .release = iosfsb_drv_close,
};


/**************************************************************************/
/*! \fn static int __init iosfsb_drv_init(void)
 **************************************************************************
 *  \brief This function is the iosfsb device module init function.
 *  \return long - 0 on success else negative number.
 **************************************************************************/
static int __init iosfsb_drv_init(void)
{
    #ifdef CONFIG_MACH_PUMA7_FPGA /*Puma7 FPGA doesnt include IOSF*/
    return(0);
    #endif

    printk(KERN_INFO "IOSF-SB: Intel(R) IOSF-SB driver built on %s @ %s\n", __DATE__, __TIME__);

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
    /* connection to the udev *******************************************************************/
    /* ceates a class directory under /sys/class */
    iosfsb_udev_class = class_create(THIS_MODULE, "iosfsb_class");
    /* ceates a class directory under /sys/class/DEV_NAME named DEV_NAME  */
    /* creates 3 file: dev, uevent, subsystem*/
    device_create(iosfsb_udev_class, NULL, iosfsb_dev_t, NULL, "%s", "docsis_iosfsb_drv");

#ifdef CONFIG_MACH_PUMA6 /* For Puma-6 SoC */
    if (aep_ipc_iosfsb_is_active() != 0)
    {
        /* AEP is Active */
        unlocked_iosfsb_write_cb = unlocked_iosfsb_write_aep;
        unlocked_iosfsb_read_cb  = unlocked_iosfsb_read_aep;
        printk(KERN_INFO "IOSF-SB: AEP mode.\n");
    }
    else
#endif
    {
        /* AEP is NOT Active */
        unlocked_iosfsb_write_cb = unlocked_iosfsb_write_direct;
        unlocked_iosfsb_read_cb  = unlocked_iosfsb_read_direct;
    }

    printk(KERN_INFO "IOSF-SB: Create device: /dev/%s\n",DEV_NAME);

    sema_init(&IosfsbSem,1);

    return IOSFSB_OK;

}


/**************************************************************************/
/*! \fn static void __exit iosfsb_drv_exit(void)
 **************************************************************************
 *  \brief This function is the iosfsb device module exit function.
 **************************************************************************/
static void __exit iosfsb_drv_exit(void)
{
    if (iosfsb_cdev)
        cdev_del(iosfsb_cdev);
    unregister_chrdev_region(iosfsb_dev_t, count);
    printk(KERN_INFO "\nIOSFSB device unregistered\n");


    device_destroy(iosfsb_udev_class, iosfsb_dev_t);
    class_destroy(iosfsb_udev_class);
}
/*************************************************************************************/

module_init(iosfsb_drv_init);
module_exit(iosfsb_drv_exit);


EXPORT_SYMBOL(iosfsb_mem_read_modify_write);
EXPORT_SYMBOL(iosfsb_reg_read_modify_write);
EXPORT_SYMBOL(iosfsb_write);
EXPORT_SYMBOL(iosfsb_read);
EXPORT_SYMBOL(iosfsb_unprotected_write);
EXPORT_SYMBOL(iosfsb_unprotected_read);
EXPORT_SYMBOL(iosfsb_down);
EXPORT_SYMBOL(iosfsb_up);


/* Driver identification */
MODULE_DESCRIPTION("IOSFSB Device Driver for docsis");
MODULE_LICENSE("GPL");


