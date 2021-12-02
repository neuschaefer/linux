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
/*-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: io_sdci.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file io_sdci.c
 *  SDAL SdCI module i/f of Europe Common Interface 
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_linux.h"
#include <linux/sdal_ioctl.h>
#include "SdCI.h"
#include "pod_if.h"
#include "dmx_if.h"
#include "util.h"

#define SEMA
//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
#ifdef SEMA
DECLARE_MUTEX(_hCI_Sem);
#endif
//-----------------------------------------------------------------------------
// Static Variables
//-----------------------------------------------------------------------------
static SdCI_StreamPath_k mStreamPath  =SD_CI_STREAM_CARD_BYPASS;
static UINT8 * pu1CurCisMap;   

//-----------------------------------------------------------------------------
// MACRO
//-----------------------------------------------------------------------------
#define CI_WARNING(fmt...)\
{\
  printk("***********************************************************\n");\
  printk("***********************************************************\n");\
  printk("***********************************************************\n");\
  printk("*                                                         *\n");\
  printk("*                                                         *\n");\
  printk("\t\t\t");  printk(fmt);  printk("%s: %d: %s\n",__FILE__,__LINE__,__FUNCTION__); \
  printk("*                                                         *\n");\
  printk("*                                                         *\n");\
  printk("***********************************************************\n");\
  printk("***********************************************************\n");\
  printk("***********************************************************\n");\
}

int sdal_ioctl_sdci(struct inode *inode, struct file *file, unsigned int cmd,
              unsigned long arg)
{
        SDAL_IOCTL_4ARG_T rArg12;
        INT32 i4SubRet;
        SdResult SdRet=SD_OK;
        static BOOL fgSdCiInit=FALSE;
#ifdef SEMA
    if (down_interruptible(&_hCI_Sem))
    {
        CI_WARNING("down_interruptible(&_hCI_Sem) fail\n");        
        return -ERESTARTSYS;
    }
#endif
        /*========= country code check =============*/
        if( COUNTRY_EU != GetTargetCountry() )
        {
            CI_WARNING("sdal_ioctl_sdci country code wrong\n");
            SdRet = SD_ERR_SYS;
            goto _CI_EXIT;
        }

	if (_IOC_SIZE(cmd)>sizeof(unsigned long))
	{
	    // Check access of user space
	    if (!access_ok(VERIFY_READ, (void __user *)arg,  _IOC_SIZE(cmd) ))
	    {
            CI_WARNING("sdal_ioctl_sdci arg access error\n");
            SdRet = SD_ERR_SYS;
            goto _CI_EXIT;
	    }
	    // Copy user space argument
	    if(copy_from_user(&rArg12, (void __user *)arg,
		       _IOC_SIZE(cmd)))
	    {
            CI_WARNING("sdal_ioctl_sdci copy_from_user error\n");
            SdRet = SD_ERR_SYS;
            goto _CI_EXIT;
	    }
	}

    switch (cmd) 
    {
        case SDAL_IO_SDCI_INIT:
            if( !fgSdCiInit )
            {
                printk("SDCI_INIT\n");
                fgSdCiInit=TRUE;
                i4SubRet =POD_Init();
                POD_AllocDmaBuf();
                
                pu1CurCisMap=kmalloc(MAX_CIS_MAP_SIZE,GFP_KERNEL);// kmalloc( SIF_DRIVER_BUF_SIZE , GFP_KERNEL );
                if(!pu1CurCisMap)
                {
                    CI_WARNING("pu1CurCisMap alloc memory fail\n");
                }
                
                POD_PcmciaEnableTS(TRUE);            
            }
            SdRet = SD_OK;
            break;
            
        case SDAL_IO_SDCI_RESET:
            POD_ResetCam();
            SdRet = SD_OK;
            break;
        case SDAL_IO_SDCI_GETSTATUS:    //SdMask_t mask, SdCI_Status_t *pState, uint32_t timeout
            i4SubRet = POD_DetectCard();
            if( i4SubRet ==1 )
            {
                ((SdCI_Status_t*)rArg12.au4Arg[1])->eCardState= SD_CI_CARDSTATE_IDLE;
            }
            else
            {
                ((SdCI_Status_t*)rArg12.au4Arg[1])->eCardState= SD_CI_CARDSTATE_NONE;
            }
            SdRet =SD_OK;
            break;
            
        case SDAL_IO_SDCI_GET:      //SdCI_Settings_t * pSettings
        {
            SdCI_Settings_t eSdCI_Setting;
            eSdCI_Setting.ePath = mStreamPath;
            if ( copy_to_user( (void __user *) rArg12.au4Arg[0]  , (const void *) &eSdCI_Setting
                , sizeof(SdCI_Settings_t) )==0)
            {
                SdRet =SD_OK;
            }
            else
            {
                CI_WARNING("copy_to_user fail\n");
                SdRet=SD_ERR_SYS;
            }
        break;
        }

        case SDAL_IO_SDCI_SET:      //SdCI_Settings_t * pSettings
        {
            SdCI_Settings_t eSdCI_Setting;
            if( copy_from_user( (void *)&eSdCI_Setting , (const void __user *) rArg12.au4Arg[0] 
                , sizeof(SdCI_Settings_t) )==0)
            {
                mStreamPath= eSdCI_Setting.ePath;
                
                if( eSdCI_Setting.ePath == SD_CI_STREAM_DISCONNECT )
                {
                    DMX_SetFrontEnd(DMX_FE_DVB_DEMOD);
                }
                else if ( eSdCI_Setting.ePath== SD_CI_STREAM_CARD_BYPASS)
                {
                    DMX_SetFrontEnd(DMX_FE_DVB_DEMOD);
                }
                else if (eSdCI_Setting.ePath== SD_CI_STREAM_CARD_THROUGH)
                {
                    DMX_SetFrontEnd(DMX_FE_EXT_SERIAL);
                }
                SdRet =SD_OK;
            }
            else
            {
                CI_WARNING("copy_to_user fail\n");
                SdRet=SD_ERR_SYS;
            }
        break;
        }

        case SDAL_IO_SDCI_READREG:      //eReg, uint8* data, uint32 length
        {
            SdCI_Register_k eReg = (SdCI_Register_k)rArg12.au4Arg[0];    UINT8* pKerBuff ;
            UINT32 u4length = rArg12.au4Arg[2];
            if( eReg <SD_CI_REGISTER_MAX)
            {
                pKerBuff=kmalloc(u4length,GFP_KERNEL) ; 
                if( pKerBuff  )
                {
                    POD_SetAccessMode(POD_DATA_CHANNEL);
                    POD_PcmciaToPod(TRUE);                
                    POD_ReadReg(POD_SDA, (UINT32)eReg,  pKerBuff, (const UINT16 *)&u4length);
                    if( copy_to_user( (void __user *) rArg12.au4Arg[1], (const void *) pKerBuff , u4length) )
                    {
                        CI_WARNING("SDAL_IO_SDCI_READREG: copy_to_user fail \n");
                    }
                    kfree( pKerBuff );            
                    SdRet =SD_OK;
                }
                else
                {
                    CI_WARNING(" kmalloc fail\n");
                    SdRet =SD_NOT_OK;
                }
            }
            else
            {
                CI_WARNING(" Register Over Range\n");
                SdRet =SD_NOT_OK;
            }
        break;
        }
        
        case SDAL_IO_SDCI_WRITEREG:    //eReg, uint8* data, uint32 length
        {
            SdCI_Register_k eReg = rArg12.au4Arg[0];    UINT8 *pKerBuff ;
            UINT32 u4length = rArg12.au4Arg[2];
            if( eReg <SD_CI_REGISTER_MAX)
            {
                pKerBuff=kmalloc(u4length,GFP_KERNEL);
                if(pKerBuff)
                {
                    if( copy_from_user( (void *)pKerBuff ,  (const void __user *) rArg12.au4Arg[1] , u4length ) )
                    {
                        CI_WARNING("SDAL_IO_SDCI_WRITEREG: copy_to_user fail \n");
                    }
                        
                    POD_SetAccessMode(POD_DATA_CHANNEL);
                    POD_PcmciaToPod(TRUE);                
                    POD_WriteReg(POD_SDA, (UINT32)eReg,  pKerBuff, u4length);
                    kfree( pKerBuff );           
                    SdRet =SD_OK;
                }
                else
                {
                    CI_WARNING(" kmalloc fail\n");
                    SdRet =SD_NOT_OK;
                }
            }
            else
            {
                CI_WARNING(" Register Over Range\n");
                SdRet =SD_NOT_OK;
            }
        break;
        }  
        
        case SDAL_IO_SDCI_READCISMEMORY:
        {
            int i;  UINT32 u4LenToRead = rArg12.au4Arg[1];
            if( u4LenToRead > MAX_CIS_MAP_SIZE )
            {
                CI_WARNING("u4LenToRead (=%u) too big\n", (unsigned int)u4LenToRead);
                u4LenToRead = MAX_CIS_MAP_SIZE;
                CI_WARNING("2)u4LenToRead change to %u\n", (unsigned int)u4LenToRead);
            }

            POD_PcmciaToPod( FALSE );       //change to PCMCIA mode (attribute memory)

            //read all cis (256 bytes)
            for( i = 0 ; i < MAX_CIS_MAP_SIZE ; i ++)
            {
                pu1CurCisMap[i] = POD_ReadCor( i *2 );          //read even bytes 
            }

            if(copy_to_user( (void __user *) rArg12.au4Arg[0], (const void *) pu1CurCisMap , u4LenToRead ))
            {
                CI_WARNING("SDAL_IO_SDCI_READCISMEMORY: copy_to_user fail \n");
            }

            SdRet =SD_OK;
        break;
        }                

        case SDAL_IO_SDCI_WRITEFCRVALUE:        //uint32_t addr32, uint8_t ccr_byte
        {
            UINT32 addr32=rArg12.au4Arg[0];     UINT8 ccr_byte=rArg12.au4Arg[1];
            POD_PcmciaToPod( FALSE );       //change to PCMCIA mode (attribute memory)            
            i4SubRet = POD_WriteCor( addr32, ccr_byte );
            if( i4SubRet ==POD_OK)
            {
                SdRet =SD_OK;
            }
            else
            {
                SdRet=SD_NOT_OK;
            }
        break;
        }
            
        case SDAL_IO_SDCI_DEBUG:
            CI_WARNING("SD_NOT_SUPPORTED\n");
            SdRet =SD_NOT_SUPPORTED;
            break;

        default:
            CI_WARNING("SD_NOT_SUPPORTED\n");
            SdRet =SD_NOT_SUPPORTED;
            break;
    }

_CI_EXIT:
#ifdef SEMA
    up(&_hCI_Sem);
#endif  
 
    return SdRet;
}
