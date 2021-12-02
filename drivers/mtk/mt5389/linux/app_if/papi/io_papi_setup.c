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
 * $RCSfile: io_papi_setup.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file io_papi_setup.c
 *  PAPI kernel driver implementation: setup
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
	
#include "papi.h"

#include "dmx_if.h"
#include "aud_if.h"
#include "mpv_if.h"
#include "vdp_if.h"
#include "pmx_if.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define MAX_PANEL 5

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
/// Common error code
#define TM_FAIL                     -0x800

/// Copy data from user space to kernel space
#define COPY_FROM_USER(dst, src, size) \
    if (!access_ok(VERIFY_READ, (void __user *)(src), (size))) \
    { \
        return TM_FAIL; \
    } \
    if (copy_from_user((dst), (void __user *)(src), (size))) \
    { \
        return TM_FAIL; \
    }

/// Copy data from kernel space to user space
#define COPY_TO_USER(dst, src, size) \
    if (!access_ok(VERIFY_WRITE, (void __user *)(dst), (size))) \
    { \
        return TM_FAIL; \
    } \
    if (copy_to_user((void __user *)(dst), (src), (size))) \
    { \
        return TM_FAIL; \
    }
    

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static int _i4PowerMode = papi_setup_pow_PowerOff;

static BOOL _fgPanelSelected = FALSE;

static UINT8 _au1PanelMap[MAX_PANEL] =
{
    PANEL_DEFAULT,  //papi_setup_cfg_PanelDVI
    PANEL_AUO_315_XW2_V6, //papi_setup_cfg_PanelT315XW02                 
    PANEL_AUO_37_HW1,  //papi_setup_cfg_PanelT370HW02                
    PANEL_AUO_40_XW1_V0, //papi_setup_cfg_PanelT420HW01               
    PANEL_LG_42_WU2 //papi_setup_cfg_PanelLC420WU2
};

//-----------------------------------------------------------------------------
// External function prototypes
//-----------------------------------------------------------------------------
extern void SelectPanel(UINT32 u4Index);
BOOL _papi_setup_IsPanelSelected(void);

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static INT32 _ioctl_SetPanel(unsigned long u4Arg)
{
    if(u4Arg < MAX_PANEL)
    {
        SelectPanel(_au1PanelMap[u4Arg]);
    }
    else
    {
        SelectPanel(_au1PanelMap[0]);
    }
    _fgPanelSelected = TRUE;
    return 0;
}

static INT32 _ioctl_SetTvPower(unsigned long u4Arg)
{
    static UINT32 u4PapiInit = 0;
    int i4PowerMode;
    INT32 i4Ret = 0;
    PAPI_SETUP_CB_T rCb;
#ifdef MEASURE_DRV_INIT_TIME
    int id;
    id = TIME_RegisterEntry("_ioctl_SetTvPower");
    TIME_StartMeasure(id);
#endif /* MEASURE_DRV_INIT_TIME */
    
    COPY_FROM_USER(&i4PowerMode, (void*)u4Arg, sizeof(int));

#if 0
    if (u4PapiInit == 0)
    {
        u4PapiInit = 1;

        // Should implement panel select customization
        // LoadPanelIndex();
        /*
        if(!_papi_setup_IsPanelSelected())
        {
            SelectPanel(0);
        }
        */

        GPIO_Init();
        PDWNC_Init();
        
        FBM_Init();
        
        VERIFY(_TSDMX_Init());

        AUD_Init();
        AUD_OutPadEnable(AUD_DEC_MAIN, TRUE);
        AUD_OutPadEnable(AUD_DEC_AUX, TRUE);

        MPV_Init();

        VDP_Init();

        VdoMLInit();

        // Change Scaler to Pixel Based
        VDP_SetScalerPixelBased(1);

        PMX_Init();
        //PMX_SetEnable(0, 1);
    }
#endif
    if(_i4PowerMode == i4PowerMode)
    {
        // Issue async notification
        rCb.u4Type = SETUP_POW_CB_TVPOWERNOTCHANGED;        
        UNUSED(_CB_PutEvent(CB_PAPI_SETUP_TRIGGER, sizeof(PAPI_SETUP_CB_T), &rCb));         
    }
    else if(_i4PowerMode == papi_setup_pow_PowerOff && (i4PowerMode == papi_setup_pow_PowerOn || i4PowerMode == papi_setup_pow_PowerMax))
    {
        _i4PowerMode = i4PowerMode;

        // Bring back from papi_setup_pow_PowerOff to papi_setup_pow_PowerOn mode, extra initialization
        
        // Issue async notification
        rCb.u4Type = SETUP_POW_CB_TVPOWERCHANGED;        
        UNUSED(_CB_PutEvent(CB_PAPI_SETUP_TRIGGER, sizeof(PAPI_SETUP_CB_T), &rCb));        
    }
    else if(_i4PowerMode == papi_setup_pow_PowerOn && (i4PowerMode == papi_setup_pow_PowerOff))
    {    
        _i4PowerMode = i4PowerMode;
        
        // Issue async notification
        rCb.u4Type = SETUP_POW_CB_TVPOWERCHANGED;        
        UNUSED(_CB_PutEvent(CB_PAPI_SETUP_TRIGGER, sizeof(PAPI_SETUP_CB_T), &rCb));      

        // Let driver start power down sequence
        _io_papi_EnterPowerDown();
    }
    else
    {
        i4Ret = -TM_ERR_NOT_SUPPORTED;

        // Issue async notification
        rCb.u4Type = SETUP_POW_CB_TVPOWERNOTCHANGED;        
        UNUSED(_CB_PutEvent(CB_PAPI_SETUP_TRIGGER, sizeof(PAPI_SETUP_CB_T), &rCb));        
    }

#ifdef MEASURE_DRV_INIT_TIME
    TIME_StopMeasure(id);
#endif /* MEASURE_DRV_INIT_TIME */
    
    return i4Ret; 
}


static INT32 _ioctl_GetTvPower(unsigned long u4Arg)
{    
    COPY_TO_USER((void*)u4Arg, &_i4PowerMode, sizeof(int));

    return 0; 
}


static INT32 _ioctl_Restart(unsigned long u4Arg)
{        
    int i4RestartAppl = (int)u4Arg;

    if(i4RestartAppl == papi_setup_pow_ApplicationTvCold)
    {
        PDWNC_Reboot();
    }
    else
    {
        return -1;
    }
    return 0; 
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

int papi_ioctl_setup(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg)
{
    int i4Ret = 0;

    switch (cmd)
    {
    case PAPI_IO_SETUP_POW_SETTVPOWER:    	
        i4Ret = _ioctl_SetTvPower(arg);
        break;    
    case PAPI_IO_SETUP_POW_GETTVPOWER:    	
        i4Ret = _ioctl_GetTvPower(arg);
        break;          
    case PAPI_IO_SETUP_POW_RESTART:    	
        i4Ret = _ioctl_Restart(arg);
        break;          
    case PAPI_IO_SETUP_CFG_SETPANEL:    	
        i4Ret = _ioctl_SetPanel(arg);
        break;

    default:
        printk("papi_ioctl_setup: unknown IO command %u\n", cmd);
        i4Ret = -EFAULT;
    }

    return i4Ret;
}

BOOL _papi_setup_IsPanelSelected(void)
{
    return _fgPanelSelected;
}

