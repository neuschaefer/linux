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
 * $RCSfile: sdal_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file sdal_if.c
 *  SDAL ioctl interface of MT538X.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_linux.h"
#include "x_typedef.h"
#include <linux/sdal_ioctl.h>
#include "x_drv_cli.h"
#include "x_assert.h"
#include "x_ckgen.h"
#include "panel.h"

#include "dmx_if.h"
#include "aud_if.h"
#include "mpv_if.h"
#include "vdp_if.h"
#include "pmx_if.h"
#include "nptv_if.h"
#ifndef CC_MT5360
#include "swdmx_if.h"
#include "playmgr_if.h"
#endif // CC_MT5360

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

/// IOCTL debug flag
//#define IOCTL_DEBUG


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Import functions
//-----------------------------------------------------------------------------

extern BOOL VdoMLInit(void);

extern void AUD_OutPadEnable(UINT8 u1DecId, BOOL fgEnable);


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

void SDAL_Init(unsigned long u4Country)
{
    static BOOL fgSdalInitFinish = FALSE, fgSdalInitStart = FALSE;
    BOOL fgWait = FALSE;
    CRIT_STATE_T rCritState;

    rCritState = x_crit_start();
    if (fgSdalInitFinish)
    {
        x_crit_end(rCritState);
        return;
    }

    if (!fgSdalInitStart) {
        fgSdalInitStart = 1;
    }
    else
    {
        fgWait = TRUE;
    }
    x_crit_end(rCritState);

    if (!fgWait)
    {
        // Set country
        if(u4Country == SD_BDCONFIG_TARGETCOUNTRY_USA ||
            u4Country == SD_BDCONFIG_TARGETCOUNTRY_KOR)
        {
            SetTargetCountry(COUNTRY_US);
#ifndef CC_MT5360            
            #ifdef CC_VCXO_ON
            BSP_CkgenInitModel(COUNTRY_US, 1);// TV_MODEL_US in x_chgen.h
            #else /* CC_VCXO_ON */
            BSP_CkgenInitModel(COUNTRY_US, 0);// TV_MODEL_US in x_chgen.h
            #endif /* CC_VCXO_ON */
#endif // CC_MT5360            
        }
        else if(u4Country == SD_BDCONFIG_TARGETCOUNTRY_EU)
        {
            SetTargetCountry(COUNTRY_EU);
#ifndef CC_MT5360            
            #ifdef CC_VCXO_ON
            BSP_CkgenInitModel(COUNTRY_EU, 1);// TV_MODEL_EU in x_chgen.h
            #else /* CC_VCXO_ON */
            BSP_CkgenInitModel(COUNTRY_EU, 0);// TV_MODEL_EU in x_chgen.h
            #endif /* CC_VCXO_ON */
#endif // CC_MT5360            
        }
        else
        {
            // keep original
        }
        
        LoadPanelIndex();

        FBM_Init();

        DMX_Init();

        AUD_Init();
        AUD_OutPadEnable(AUD_DEC_MAIN, TRUE);
        AUD_OutPadEnable(AUD_DEC_AUX, TRUE);

        VDEC_Init();
#ifndef CC_MT5360
        SWDMX_Init();

        PLAYMGR_Init();
#endif // CC_MT5360
        VDP_Init();

        VdoMLInit();

        // Change Scaler to Pixel Based
        VDP_SetScalerPixelBased(1);

        PMX_Init();
        vApiPanelPowerSequence(1);

        fgSdalInitFinish = TRUE;
    }
    else
    {
        /* Wait until SDAL_Init() finish, which is invoked by other thread. */
        while (!fgSdalInitFinish)
        {
            static UINT32 u4Count=0;
            x_thread_delay(10);
            u4Count++;
            ASSERT(u4Count < 1000);
        }
    }
}

static int sdal_open(struct inode * inode, struct file * filp)
{
    // move to SDAL_IO_SDDRV_INIT()
    //SDAL_Init();

    return 0;
}

static int sdal_ioctl(struct inode *inode, struct file *file, unsigned int cmd,
              unsigned long arg)
{
    int retval = 0;

	switch (_IOC_TYPE(cmd))
	{
	case SDAL_IOCTYPE_DRV:
		retval = sdal_ioctl_sddrv(inode, file, cmd, arg);
		break;
		
	case SDAL_IOCTYPE_SDAUXOUT:
		retval = sdal_ioctl_sdauxout(inode, file, cmd, arg);
		break;

	case SDAL_IOCTYPE_SDDISPLAY:
		retval = sdal_ioctl_sddisplay(inode, file, cmd, arg);
		break;

	case SDAL_IOCTYPE_SDEXTIN:
		retval = sdal_ioctl_sdextin(inode, file, cmd, arg);
		break;

	case SDAL_IOCTYPE_SDNVRAM:
		retval = sdal_ioctl_sdnvram(inode, file, cmd, arg);
		break;

	case SDAL_IOCTYPE_SDTSDATA:
		retval = sdal_ioctl_sdtsdata(inode, file, cmd, arg);
		break;

    	case SDAL_IOCTYPE_SDTUNER:
		retval = sdal_ioctl_sdtuner(inode, file, cmd, arg);
        	break;

	case SDAL_IOCTYPE_SDVIDEO:
		retval = sdal_ioctl_sdvideo(inode, file, cmd, arg);
		break;

	case SDAL_IOCTYPE_SDUSERDATA:
		retval = sdal_ioctl_sduserdata(inode, file, cmd, arg);
		break;

	case SDAL_IOCTYPE_SDAVDEC:
		retval = sdal_ioctl_sdavdec(inode, file, cmd, arg);
		break;

    case SDAL_IOCTYPE_SDSOUND:
        retval = sdal_ioctl_sdsound(inode, file, cmd, arg);
        break;
#ifndef CC_MT5360
    case SDAL_IOCTYPE_SDFEEDER:
        retval = sdal_ioctl_sdfeeder(inode, file, cmd, arg);
        break;
#endif // CC_MT5360
    case SDAL_IOCTYPE_SDIMAGE:
        retval = sdal_ioctl_sdimage(inode, file, cmd, arg);
        break;

    case SDAL_IOCTYPE_SIF:
        retval = sdal_ioctl_sdsif(inode, file, cmd, arg);
        break;

    case SDAL_IOCTYPE_SDMISC:
        retval = sdal_ioctl_sdmisc(inode, file, cmd, arg);
        break;

    case SDAL_IOCTYPE_SDDEBUG:
        retval = sdal_ioctl_sddebug(inode, file, cmd, arg);
        break;
        
    case SDAL_IOCTYPE_SDSCART:
    	retval = sdal_ioctl_sdscart(inode, file, cmd, arg);
    	break;

    case SDAL_IOCTYPE_SDCI:
    	retval = sdal_ioctl_sdci(inode, file, cmd, arg);
    	break;

    	
	default:
        break;
	}

#ifdef IOCTL_DEBUG
    printk("IOCTL cmd: 0x%08x, arg: 0x%08x, ret: %d\n", cmd, arg, retval);
#endif

	return retval;
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

struct file_operations sdal_fops = {
	.open		= sdal_open,
	.ioctl		= sdal_ioctl,
};


