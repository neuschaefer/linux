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
 * $RCSfile: papi_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file papi_if.c
 *  PAPI ioctl interface of MT538Xp.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_linux.h"
#include "x_typedef.h"
#include "x_assert.h"
#include <linux/papi_ioctl.h>
#include "x_drv_cli.h"
#include "panel.h"
#include "x_timeprofile.h"

#include "dmx_if.h"
#include "aud_if.h"
#include "mpv_if.h"
#include "vdp_if.h"
#include "pmx_if.h"


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
extern BOOL _TSDMX_Init(void);
extern BOOL _papi_setup_IsPanelSelected(void);

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

void PAPI_Init(void)
{
    static UINT32 u4PapiInit = 0;
    
    if (u4PapiInit == 0)
    {
#ifdef MEASURE_DRV_INIT_TIME
        int id[20];
        id[0] = TIME_RegisterEntry("PAPI_Init");
        TIME_StartMeasure(id[0]);
#endif /* MEASURE_DRV_INIT_TIME */

        u4PapiInit = 1;

        // Should implement panel select customization
        // LoadPanelIndex();
        /*
        if(!_papi_setup_IsPanelSelected())
        {
            SelectPanel(0);
        }
        */

        /* temporarily light-up the LC32_WUF_SBN1 here ! */
        MEASURE_TIME(id[1], "SelectPanel", SelectPanel(100));
        MEASURE_TIME(id[2], "GPIO_Init", GPIO_Init());
        MEASURE_TIME(id[3], "PDWNC_Init", PDWNC_Init());
        MEASURE_TIME(id[4], "FBM_Init", FBM_Init());
        MEASURE_TIME(id[5], "_TSDMX_Init", VERIFY(_TSDMX_Init()));
        MEASURE_TIME(id[6], "AUD_Init", AUD_Init());
        MEASURE_TIME(id[7], "AUD_OutPadEnable", AUD_OutPadEnable(AUD_DEC_MAIN, TRUE));
        MEASURE_TIME(id[8], "AUD_OutPadEnable", AUD_OutPadEnable(AUD_DEC_AUX, TRUE));
        MEASURE_TIME(id[9], "VDEC_Init", VDEC_Init());
        MEASURE_TIME(id[10], "VDP_Init", VDP_Init());
        MEASURE_TIME(id[11], "VdoMLInit", VdoMLInit());
        MEASURE_TIME(id[12], "SIF_Init", SIF_Init());
        MEASURE_TIME(id[13], "EEPROM_Init", EEPROM_Init());
        
        // Change Scaler to Pixel Based
        MEASURE_TIME(id[14], "VDP_SetScalerPixelBased", VDP_SetScalerPixelBased(1));

        MEASURE_TIME(id[15], "PMX_Init", PMX_Init());
        //PMX_SetEnable(0, 1);

        /* temporarily light-up the LC32_WUF_SBN1 here ! */
        MEASURE_TIME(id[16], "vApiPanelPowerSequence", vApiPanelPowerSequence(TRUE));

        MEASURE_TIME(id[17], "RTC_Init", RTC_Init());
        
#ifdef MEASURE_DRV_INIT_TIME
        TIME_StopMeasure(id[0]);
#endif /* MEASURE_DRV_INIT_TIME */
    }
}

static int papi_open(struct inode * inode, struct file * filp)
{
    PAPI_Init();

    return 0;
}

static int papi_ioctl(struct inode *inode, struct file *file, unsigned int cmd,
              unsigned long arg)
{
    int retval = 0;

	switch (_IOC_TYPE(cmd))
	{		
	case PAPI_IOCTYPE_FE:
		retval = papi_ioctl_fe(inode, file, cmd, arg);
		break;

	case PAPI_IOCTYPE_AF:
		retval = papi_ioctl_af(inode, file, cmd, arg);
		break;
/*
	case PAPI_IOCTYPE_CONN:
		retval = papi_ioctl_conn(inode, file, cmd, arg);
		break;
*/
	case PAPI_IOCTYPE_GEN:
		retval = papi_ioctl_gen(inode, file, cmd, arg);
		break;

	case PAPI_IOCTYPE_INFRA:
		retval = papi_ioctl_infra(inode, file, cmd, arg);
		break;

	case PAPI_IOCTYPE_INTERAL:
    	        retval = papi_ioctl_internal(inode, file, cmd, arg);
    	        break;

	case PAPI_IOCTYPE_MIXER:
		retval = papi_ioctl_mixer(inode, file, cmd, arg);
		break;

	case PAPI_IOCTYPE_SETUP:
		retval = papi_ioctl_setup(inode, file, cmd, arg);
		break;

	case PAPI_IOCTYPE_SRC:
		retval = papi_ioctl_src(inode, file, cmd, arg);
		break;

    	case PAPI_IOCTYPE_VF:
        	retval = papi_ioctl_vf(inode, file, cmd, arg);
        	break;

	case PAPI_IOCTYPE_PAPIONLY:
		retval = papi_ioctl_papionly(inode, file, cmd, arg);
		break;

	case PAPI_IOCTYPE_MTK:
		retval = papi_ioctl_mtk(inode, file, cmd, arg);
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

struct file_operations papi_fops = {
	.open		= papi_open,
	.ioctl		= papi_ioctl,
};


