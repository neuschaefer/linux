/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: io_sdsif.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------*/

/** @file io_sdnvram.c
 *  SDAL ioctl interface of MT538X nvram driver.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_linux.h"
#include <linux/sdal_ioctl.h>
#include <linux/slab.h>

#include "sif_if.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------
#define Printf printk
#define STATIC 
#define STATIC  static
//#define lousy
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define I2C_BUSY_SCL  0x01 // SCL is low
#define I2C_BUSY_SDA  0x02 // SDA is low
//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define	SIF_DRIVER_BUF_SIZE	(256)

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
DECLARE_MUTEX(_hSIF_Sem);

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static INT32 K_SIF_ReadMultipleSubAddr(UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddrNum, UINT32 u4WordAddr, 
								UINT8 *pu1Buf, UINT16 u2ByteCnt)
{
	UINT8 * pu1KernelBuf = NULL;
	INT32 i4Ret;

	//check demanding buffer size is not too large
	if ( u2ByteCnt > SIF_DRIVER_BUF_SIZE )
	{
		//return -1;
		u2ByteCnt = SIF_DRIVER_BUF_SIZE;
	}
	
	pu1KernelBuf = kmalloc( u2ByteCnt , GFP_KERNEL );

	if ( !pu1KernelBuf )
	{
		return -1;
	}

	//----Memory Allocated (remember to free allocated buffer before exit)------//
	
	i4Ret = SIF_ReadMultipleSubAddr( u2ClkDiv , u1DevAddr , u1WordAddrNum , u4WordAddr , pu1KernelBuf , u2ByteCnt ) ;

	if ( i4Ret <= 0 )
	{
		//read fail
	}else if ( copy_to_user( (void __user *) pu1Buf , (const void *) pu1KernelBuf , i4Ret ) )
	{
		i4Ret = -EFAULT;
	}
	
	if( pu1KernelBuf )
	{
		kfree( pu1KernelBuf );
	}
	return 	i4Ret ; 
}

static INT32 K_SIF_WriteMultipleSubAddr(UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddrNum , UINT32 u4WordAddr, 
								UINT8 *pu1Buf, UINT16 u2ByteCnt)
{
	UINT8 * pu1KernelBuf = NULL;
	INT32 i4Ret;

	//check demanding buffer size is not too large
	if ( u2ByteCnt > SIF_DRIVER_BUF_SIZE )
	{
		u2ByteCnt = SIF_DRIVER_BUF_SIZE;
	}

	pu1KernelBuf = kmalloc( SIF_DRIVER_BUF_SIZE , GFP_KERNEL );
	
	if ( !pu1KernelBuf )
	{
		return -1;
	}

	//----Memory Allocated (remember to free allocated buffer before exit)------//
	
	if ( copy_from_user( (void *)pu1KernelBuf , (const void __user *) pu1Buf , u2ByteCnt ) )
	{
		i4Ret = -EFAULT;
	}else{
		i4Ret = SIF_WriteMultipleSubAddr( u2ClkDiv , u1DevAddr , u1WordAddrNum , u4WordAddr , pu1KernelBuf , u2ByteCnt ) ;
	}

	if( pu1KernelBuf )
	{
		kfree( pu1KernelBuf );
	}
	return 	i4Ret ; 
}

STATIC INT32 K_SIFSW_ReadDataNoSubAddr(    UINT32 u4Sda,UINT32 u4Scl,UINT8 bDevAddr, UINT8 bDataCount,
        UINT8 *pbData, UINT32 bOption, UINT16 u2Delay, UINT16 u2SyncTimeout)
{
	UINT8 * pu1KernelBuf = NULL;
	INT32 i4Ret;

	//check demanding buffer size is not too large
	if ( bDataCount > SIF_DRIVER_BUF_SIZE )
	{
		bDataCount = SIF_DRIVER_BUF_SIZE;
	}

	pu1KernelBuf = kmalloc( SIF_DRIVER_BUF_SIZE , GFP_KERNEL );
	
	if ( !pu1KernelBuf )
	{
		return -1;
	}
#ifdef lousy
    printk("K_SIFSW_ReadDataNoSubAddr: u4Sda=%d,u4Scl=%d,bDevAddr=0x%x, bDataCount=%d, pbData[0]=0x%x, bOption=0x%x, u2Delay=%d\n",u4Sda, u4Scl ,bDevAddr,  bDataCount ,pbData[0] ,bOption, u2Delay);    //lousy
#endif
 	//----Memory Allocated (remember to free allocated buffer before exit)------//
   // initial SIF sda, scl pin.
    SIFSW_Init(u4Sda, u4Scl);		
	i4Ret = SIFSW_ReadDataNoSubAddr( bDevAddr , bDataCount , pu1KernelBuf , bOption ,u2Delay ,u2SyncTimeout) ;
    SIFSW_Remove(u4Sda, u4Scl);		
/*
    printk("K_SIFSW_ReadDataNoSubAddr: u4Sda=%d,u4Scl=%d,bDevAddr=0x%x, bDataCount=%d,  pu1KernelBuf[0]=0x%x, bOption=0x%x, i4Ret=%d\n",u4Sda, u4Scl ,bDevAddr,  bDataCount , pu1KernelBuf[0] ,bOption,i4Ret);    //lousy debug
    {   //lousy debug
    	UINT32 i4Index;
    	UINT32 u4Cnt = bDataCount;
    	if (i4Ret>0)
    	{
    	// print sif read data content.
    		for (i4Index = 0 ; i4Index<u4Cnt ; i4Index ++)
    		{
    			if (!(i4Index % 8))
    			{
    				Printf("\n0x%08X :", i4Index);                    
    			}
    			Printf(" 0x%02X,", pu1KernelBuf[i4Index]);
    		}
    			Printf("\n");
    	}
    	else if( i4Ret ==0 )
    	{
    		Printf("driver fail, sif ioctl should never return 0\n");
        }
    }
*/
	if ( i4Ret <= 0 )
	{
		//read fail
	}else if ( copy_to_user( (void __user *) pbData , (const void *) pu1KernelBuf , bDataCount ) )
	{
		i4Ret = -EFAULT;
	}

/*    printk("K_SIFSW_ReadDataNoSubAddr: i4Ret=%d after copy_to_user,pbData[0]=0x%x\n",i4Ret,pbData[0]);    //lousy
    {   //lousy
    	UINT32 i4Index;
    	UINT32 u4Cnt = bDataCount;    	
    	if (i4Ret>0)
    	{
    	// print sif read data content.
    		for (i4Index = 0 ; i4Index<u4Cnt ; i4Index ++)
    		{
    			if (!(i4Index % 8))
    			{
    				Printf("\n0x%08X :", i4Index);                    
    			}
    			Printf(" 0x%02X,", pbData[i4Index]);
    		}
    			Printf("\n");
    	}
    	else if( i4Ret ==0 )
    	{
    		Printf("driver fail, sif ioctl should never return 0\n");
        }
    }
*/
	if( pu1KernelBuf )
	{
		kfree( pu1KernelBuf );
	}
	return 	i4Ret ; 
}

STATIC INT32 K_SIFSW_WriteDataNoSubAddr(UINT32 u4Sda,UINT32 u4Scl,UINT8 bDevAddr, UINT8 bDataCount, 
        UINT8 *pbData, UINT32 bOption, UINT16 u2Delay, UINT16 u2SyncTimeout)
{
	UINT8 * pu1KernelBuf = NULL;
	INT32 i4Ret;

	//check demanding buffer size is not too large
	if ( bDataCount > SIF_DRIVER_BUF_SIZE )
	{
		bDataCount = SIF_DRIVER_BUF_SIZE;
	}

	pu1KernelBuf = kmalloc( SIF_DRIVER_BUF_SIZE , GFP_KERNEL );
	
	if ( !pu1KernelBuf )
	{
		return -1;
	}
#ifdef lousy
    printk("K_SIFSW_WriteDataNoSubAddr: u4Sda=%d,u4Scl=%d,bDevAddr=0x%x, bDataCount=%d, pbData[0]=0x%x, bOption=0x%x, u2Delay=%d\n",u4Sda, u4Scl ,bDevAddr,  bDataCount ,pbData[0] ,bOption, u2Delay);    //lousy
#endif
	//----Memory Allocated (remember to free allocated buffer before exit)------//
	if ( copy_from_user( (void *)pu1KernelBuf , (const void __user *) pbData , bDataCount ) )
	{
		i4Ret = -EFAULT;
	}else{
        // initial SIF sda, scl pin.
        SIFSW_Init(u4Sda, u4Scl);	    
		i4Ret = SIFSW_WriteDataNoSubAddr( bDevAddr , bDataCount , pu1KernelBuf , bOption ,u2Delay ,u2SyncTimeout) ;
        SIFSW_Remove(u4Sda, u4Scl);		
	}
/*
    printk("K_SIFSW_WriteDataNoSubAddr: i4Ret=%d \n", i4Ret);    //lousy 
*/

	if( pu1KernelBuf )
	{
		kfree( pu1KernelBuf );
	}
	return 	i4Ret ; 
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
int  sdal_ioctl_sdsif(struct inode *inode, struct file *file, unsigned int cmd,
                       unsigned long arg)
{
    SDAL_IOCTL_8ARG_T rArg8;
    INT32 i4Ret = 0;
    UINT8 * pu1KBuf = NULL;
    if (down_interruptible(&_hSIF_Sem))
    {
        return -ERESTARTSYS;
    }
    
    // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *) arg, _IOC_SIZE(cmd)))
    {
        printk("sdal_ioctl_sdsif arg access error\n");
        i4Ret = -EFAULT;
        goto _SIF_EXIT;
    }

    // Copy user space argument
    if (copy_from_user(&rArg8, (void __user *) arg, _IOC_SIZE(cmd)))
    {
	printk("sdal_ioctl_sdsif copy_from_user error\n");
        i4Ret = -EFAULT;
        goto _SIF_EXIT;
    }

    pu1KBuf =  kmalloc( SIF_DRIVER_BUF_SIZE , GFP_KERNEL );
    if( !pu1KBuf )
    {
	i4Ret = -ENOMEM ;
	goto _SIF_EXIT;
    }
/*
    if( (cmd==SIF_IO_READ) || cmd==SIF_IO_READ_NO_SUBADDR) ||cmd==SIF_IO_TUNER_READ_NO_SUBADDR) ||cmd==SIF_IO_READ_MULTIPLE_SUBADDR)  )
    {
		printk("SIF IO_WRITE\n");
    }
*/
    switch (cmd)
    {
        case  SIF_IO_READ:
            //i4Ret =SIF_Read(rArg8.au4Arg[0], rArg8.au4Arg[1], rArg8.au4Arg[2], rArg8.au4Arg[3], rArg8.au4Arg[4]);
            i4Ret =K_SIF_ReadMultipleSubAddr(rArg8.au4Arg[0], rArg8.au4Arg[1], 1, rArg8.au4Arg[2], rArg8.au4Arg[3], rArg8.au4Arg[4]);
	     break;

        case  SIF_IO_WRITE:
            //i4Ret =SIF_Write(rArg8.au4Arg[0], rArg8.au4Arg[1], rArg8.au4Arg[2], rArg8.au4Arg[3], rArg8.au4Arg[4]);
            i4Ret =K_SIF_WriteMultipleSubAddr(rArg8.au4Arg[0], rArg8.au4Arg[1], 1, rArg8.au4Arg[2], rArg8.au4Arg[3], rArg8.au4Arg[4]);
	     break;

        case  SIF_IO_READ_NO_SUBADDR:
            //i4Ret =SIF_ReadNoSubAddr(rArg8.au4Arg[0], rArg8.au4Arg[1], rArg8.au4Arg[2], rArg8.au4Arg[3]);
            i4Ret =K_SIF_ReadMultipleSubAddr(rArg8.au4Arg[0], rArg8.au4Arg[1],  0,       0        , rArg8.au4Arg[2], rArg8.au4Arg[3]);
	     break;

        case  SIF_IO_WRITE_NO_SUBADDR:
            //i4Ret =SIF_WriteNoSubAddr(rArg8.au4Arg[0], rArg8.au4Arg[1], rArg8.au4Arg[2], rArg8.au4Arg[3]);
            i4Ret =K_SIF_WriteMultipleSubAddr(rArg8.au4Arg[0], rArg8.au4Arg[1],  0,       0        , rArg8.au4Arg[2], rArg8.au4Arg[3]);
	     break;

        case  SIF_IO_TUNER_READ_NO_SUBADDR:
        {
            UINT16 u2Size=rArg8.au4Arg[3];  UINT8 * pu1UserBuf=rArg8.au4Arg[2];
            if(u2Size>SIF_DRIVER_BUF_SIZE)  
            {
                u2Size=SIF_DRIVER_BUF_SIZE;
            }
            //i4Ret =SIF_TunerReadNoSubAddr(rArg8.au4Arg[0], rArg8.au4Arg[1], rArg8.au4Arg[2], rArg8.au4Arg[3]);
            i4Ret =SIF_TunerReadNoSubAddr(rArg8.au4Arg[0], rArg8.au4Arg[1], pu1KBuf , u2Size);
        	if ( i4Ret <= 0 )
        	{
        		//read fail
        	}else if ( copy_to_user( (void __user *) pu1UserBuf , (const void *) pu1KBuf , u2Size ) )
        	{
        		i4Ret = -EFAULT;
        	}
        }
	     break;

        case  SIF_IO_TUNER_WRITE_NO_SUBADDR:
        {
            UINT16 u2Size=rArg8.au4Arg[3];  UINT8 * pu1UserBuf=rArg8.au4Arg[2];
            if(u2Size>SIF_DRIVER_BUF_SIZE)  
            {
                u2Size=SIF_DRIVER_BUF_SIZE;
            }
        	if ( copy_from_user( (void *)pu1KBuf , (const void __user *) pu1UserBuf , u2Size ) )
        	{
        		i4Ret = -EFAULT;
        	}else
        	{
            	//i4Ret =SIF_TunerWriteNoSubAddr(rArg8.au4Arg[0], rArg8.au4Arg[1], rArg8.au4Arg[2], rArg8.au4Arg[3]);
                i4Ret =SIF_TunerWriteNoSubAddr(rArg8.au4Arg[0], rArg8.au4Arg[1], pu1KBuf , u2Size);
        	}
        }
	     break;

        case  SIF_IO_READ_MULTIPLE_SUBADDR:
            i4Ret =K_SIF_ReadMultipleSubAddr(rArg8.au4Arg[0], rArg8.au4Arg[1], rArg8.au4Arg[2], rArg8.au4Arg[3], rArg8.au4Arg[4], rArg8.au4Arg[5]);
	     break;

        case  SIF_IO_WRITE_MULTIPLE_SUBADDR:
            i4Ret =K_SIF_WriteMultipleSubAddr(rArg8.au4Arg[0], rArg8.au4Arg[1], rArg8.au4Arg[2], rArg8.au4Arg[3], rArg8.au4Arg[4], rArg8.au4Arg[5]);
	     break;

        case  SIF_IO_SW_READ_NOSUBADDR:
            i4Ret =K_SIFSW_ReadDataNoSubAddr(rArg8.au4Arg[0], rArg8.au4Arg[1], rArg8.au4Arg[2], rArg8.au4Arg[3],
                rArg8.au4Arg[4], rArg8.au4Arg[5], rArg8.au4Arg[6],rArg8.au4Arg[7]);
            break;

        case  SIF_IO_SW_WRITE_NOSUBADDR:
            i4Ret =K_SIFSW_WriteDataNoSubAddr(rArg8.au4Arg[0], rArg8.au4Arg[1], rArg8.au4Arg[2], rArg8.au4Arg[3],
                rArg8.au4Arg[4], rArg8.au4Arg[5],rArg8.au4Arg[6],rArg8.au4Arg[7]);
            break;

        case  SIF_IO_CHECKBUSY:
            if( GPIO_VotageLevel(rArg8.au4Arg[0])==0 )
                i4Ret |= I2C_BUSY_SCL;
            if( GPIO_VotageLevel(rArg8.au4Arg[1])==0 )
                i4Ret |= I2C_BUSY_SDA;
            break;
#if 0            
        case SIF_IO_PMX_READ_EDID: //#edid, pbuf, size
        {
            UINT32 u4Size=rArg8.au4Arg[2];
            if(u4Size>SIF_DRIVER_BUF_SIZE)  
            {
                u4Size=SIF_DRIVER_BUF_SIZE;
            }
            i4Ret = Sif_SS_ReadEDIDData(rArg8.au4Arg[0] , pu1KBuf, u4Size);
        	if ( i4Ret <= 0 )
        	{
        		//read fail
        	}else if ( copy_to_user( (void __user *) rArg8.au4Arg[1] , (const void *) pu1KBuf , u4Size ) )
        	{
        		i4Ret = -EFAULT;
        	}
        }
            break;
#endif
#if 0
        case SIF_IO_PMX_WRITE_EDID: //#edid, pbuf, size
        {
            UINT32 u4Size=rArg8.au4Arg[2];
            if(u4Size>SIF_DRIVER_BUF_SIZE)  
            {
                u4Size=SIF_DRIVER_BUF_SIZE;
            }
        	if ( copy_from_user( (void *)pu1KBuf , (const void __user *) rArg8.au4Arg[1] , u4Size ) )
        	{
        		i4Ret = -EFAULT;
        	}else
        	{
                i4Ret = Sif_SS_WriteEDIDData(rArg8.au4Arg[0] , pu1KBuf, u4Size);
        	}
        }
            break;
#endif
#if 0
        case SIF_IO_PMX_REDIRECT: 
            i4Ret = Sif_Pinmux( rArg8.au4Arg[0] );
            break;
#endif
        default:
            i4Ret = -1;
            break;
    }
/*
    if( (cmd==SIF_IO_WRITE) || cmd==SIF_IO_WRITE_NO_SUBADDR) ||cmd==SIF_IO_TUNER_WRITE_NO_SUBADDR) ||cmd==SIF_IO_WRITE_MULTIPLE_SUBADDR)  )
    {
		printk("SIF IO_WRITE\n");
    }
*/    
_SIF_EXIT:

	if( pu1KBuf )
	{
		kfree( pu1KBuf );
    	}

    up(&_hSIF_Sem);
    
    return i4Ret;
}

