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
 * $RCSfile: io_mtk_cust.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file io_mtk_cust.c
 *  PAPI kernel driver implementation: diversity
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include <linux/papi_ioctl.h>
#include "drvcust_if.h"
#include "papi_types.h"


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define TM_FAIL                -0x800

/// Copy data from user space to kernel space
#define COPY_FROM_USER(dst, src, size)                              \
    if (!access_ok(VERIFY_READ, (void __user *)(src), (size)))      \
    {                                                               \
        printk(KERN_ERR "Cannot read from address 0x%08X!\n", (unsigned int)(src)); \
        return TM_FAIL;                                             \
    }                                                               \
    if (copy_from_user((dst), (void __user *)(src), (size)))        \
    {                                                               \
        printk(KERN_ERR "Failed to copy data to kernel space!\n");  \
        return TM_FAIL;                                             \
    }

/// Copy data from kernel space to user space
#define COPY_TO_USER(dst, src, size)                                \
    if (!access_ok(VERIFY_WRITE, (void __user *)(dst), (size)))     \
    {                                                               \
        printk(KERN_ERR "Cannot write to address 0x%08X!\n", (unsigned int)(dst)); \
        return TM_FAIL;                                             \
    }                                                               \
    if (copy_to_user((void __user *)(dst), (src), (size)))          \
    {                                                               \
        printk(KERN_ERR "Failed to copy data to user space!\n");    \
        return TM_FAIL;                                             \
    }


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static MTK_CUST_KEYBOARD_BUTTON_T _arPapiKeyboard[MTK_CUST_KEYBOARD_BUTTON_MAX_COUNT];
static UINT32 _u4PapiNumButtons;
static UINT32 _u4PapiKeyboardServo = SERVO5 -SERVO0;


//-----------------------------------------------------------------------------
// External function prototypes
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static INT32 _ioctl_cust_GetDmxShmProperties(unsigned long u4Arg)
{
    IOCTL_2ARG_T rArg;
    UINT32 u4Size, u4Addr;    
    INT32 i4Ret = 0;
    
    if(DRVCUST_OptQuery(eDmxSharedMemSize, &u4Size))
    {
        return -1;
    }

    u4Addr = DRVCUST_OptGet(eTotalMemSize);
    u4Addr -= DRVCUST_OptGet(eFbmMemSize); 
    u4Addr -= DRVCUST_OptGet(eFBMemSize); 
    u4Addr -= DRVCUST_OptGet(eDirectFBMemSize); 
    u4Addr -= u4Size;

    rArg.au4Arg[0] = u4Addr;
    rArg.au4Arg[1] = u4Size;
    
    COPY_TO_USER((void*)u4Arg, &rArg, sizeof(IOCTL_2ARG_T));
    
    return i4Ret;
}

static INT32 _ioctl_cust_GetNoClearMemoryProperties(unsigned long u4Arg)
{
    IOCTL_2ARG_T rArg;
    INT32 i4Ret;
    UINT32 u4Size, u4Addr;

    if (DRVCUST_OptQuery(eNoClearMemSize, &u4Size))
    {
        return -1;
    }

    u4Addr = DRVCUST_OptGet(eTotalMemSize);
    u4Addr -= u4Size;

    rArg.au4Arg[0] = u4Addr;
    rArg.au4Arg[1] = u4Size;
    
    COPY_TO_USER((void*)u4Arg, &rArg, sizeof(IOCTL_2ARG_T));
    
    return i4Ret;
}

static INT32 _ioctl_cust_InstallKeyboard(unsigned long u4Arg)
{
    UINT32 u4NumKeys = 0;
    IOCTL_2ARG_T rArg;

    COPY_FROM_USER(&rArg, u4Arg, sizeof(IOCTL_2ARG_T));    

    _u4PapiNumButtons = rArg.au4Arg[0];
    
    COPY_FROM_USER((void*)_arPapiKeyboard, (void*)rArg.au4Arg[1], _u4PapiNumButtons * 3 * sizeof(UINT32));    

    return 0;
}

static INT32 _ioctl_cust_SetPanelTiming(unsigned long u4Arg)
{
    IOCTL_2ARG_T rArg;
    MTK_CUST_PANEL_ATTR_T rPanelAttr;
    UINT32 u4ControlWord;
    UINT32 u4Clk_50Hz, u4Clk_60Hz;

    COPY_FROM_USER(&rArg, u4Arg, sizeof(IOCTL_2ARG_T));
    COPY_FROM_USER(&rPanelAttr, rArg.au4Arg[0], rArg.au4Arg[1]);

    /* set the panel attribute */
    PANEL_SetPanelWidth(rPanelAttr.HActive);
    PANEL_SetPanelHeight(rPanelAttr.VActive);
    PANEL_SetHTotal50Hz(rPanelAttr.HTotal_50Hz);
    PANEL_SetHTotal60Hz(rPanelAttr.HTotal_60Hz);
    PANEL_SetHTotalMin(rPanelAttr.HTotal_50Hz*8/10);
    PANEL_SetHTotalMax(rPanelAttr.HTotal_50Hz*12/10);
    PANEL_SetVTotal50Hz(rPanelAttr.VTotal_50Hz);
    PANEL_SetVTotal60Hz(rPanelAttr.VTotal_60Hz);
    PANEL_SetVTotalMin(rPanelAttr.VTotal_50Hz*8/10);
    PANEL_SetVTotalMax(rPanelAttr.VTotal_50Hz*12/10);

    PANEL_SetHPosition60Hz(rPanelAttr.HBlankE_50Hz - rPanelAttr.HBlankS_50Hz);
    PANEL_SetVPosition(rPanelAttr.VBlankE_50Hz - rPanelAttr.VBlankS_50Hz);
    PANEL_SetHSyncWidth(rPanelAttr.HSyncE - rPanelAttr.HSyncS);
    PANEL_SetVSyncWidth(rPanelAttr.VSyncE - rPanelAttr.VSyncS);

    u4Clk_50Hz = rPanelAttr.HTotal_50Hz * rPanelAttr.VTotal_50Hz * 50;
    u4Clk_60Hz = rPanelAttr.HTotal_60Hz * rPanelAttr.VTotal_60Hz * 60;

    if(rPanelAttr.FieldRate >= 100)
    {
        /* 100Hz/ 120Hz panel */
        u4Clk_50Hz <<= 1;
        u4Clk_60Hz <<= 1;
    }

    PANEL_SetPixelClk50Hz(u4Clk_50Hz);
    PANEL_SetPixelClk60Hz(u4Clk_60Hz);
    PANEL_SetPixelClkMin(u4Clk_50Hz/10*8);
    PANEL_SetPixelClkMax(u4Clk_60Hz/10*12);

    /* get control word, only update the bits we care */
    u4ControlWord = PANEL_GetControlWord();
    /* clear the bits we used */
    u4ControlWord &= ~(LVDS_MSB_SW_ON|DUAL_PORT|FOUR_PORT|DISP_24BIT|DISP_30BIT);

    if(u4Clk_60Hz/2 > 90000000)
    {
        u4ControlWord |= FOUR_PORT;
    }
    else if(u4Clk_60Hz > 90000000)
    {
        u4ControlWord |= DUAL_PORT;
    }
    else
    {
        u4ControlWord |= SINGLE_PORT;
    }

    switch(rPanelAttr.LvdsMode)
    {
        case LVDS_MODE_8BIT_JEIDA:
            u4ControlWord |= DISP_24BIT;
        break;
        case LVDS_MODE_8BIT_VESA:
            u4ControlWord |= DISP_24BIT;
            u4ControlWord |= LVDS_MSB_SW_ON;
        break;
        case LVDS_MODE_10BIT_VESA:
            u4ControlWord |= DISP_30BIT;
            u4ControlWord |= LVDS_MSB_SW_ON;
        break;
        case LVDS_MODE_10BIT_JEIDA:
            u4ControlWord |= DISP_30BIT;
        break;
        case LVDS_MODE_10BIT_KOREA:
            u4ControlWord |= DISP_30BIT;
        break;
    }

    /* PCB setting */
    u4ControlWord &= ~(0xff << 9);
    u4ControlWord |= (LVDS_A_FROM_BO|LVDS_B_FORM_BE|LVDS_C_FROM_AE|LVDS_D_FROM_AO);

    PANEL_SetControlWord(u4ControlWord);

    PANEL_SetLvdsOnDelay(rPanelAttr.LvdsOnDelay);
    PANEL_SetBacklightOnDelay(rPanelAttr.BacklightOnDelay);
    PANEL_SetBacklightOffDelay(rPanelAttr.LvdsOffDelay);
    PANEL_SetLvdsOffDelay(rPanelAttr.PanelOffDelay);

    /* not really matters, leave them the safe value */
    PANEL_SetVClkMax(63);
    PANEL_SetVClkMin(47);
    
    return 0;
}



//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
INT32 _MTK_CUST_GetKeyboard(UINT32* pu4Servo, UINT32* pu4NumButtons, MTK_CUST_KEYBOARD_BUTTON_T** pprButtons)
{
    if(pu4NumButtons == NULL || pprButtons == NULL || pu4Servo == NULL)
    {
        return -1;
    }
    *pprButtons = _arPapiKeyboard;
    *pu4NumButtons = _u4PapiNumButtons;
    *pu4Servo = _u4PapiKeyboardServo;
    return 0;
}


int papi_ioctl_mtk(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg)
{
    int i4Ret = 0;

    switch (cmd)
    {
        case PAPI_IO_MTK_CUST_GETDMXSHHMPROPERTIES:        	
            i4Ret = _ioctl_cust_GetDmxShmProperties(arg);        	
        break;

        case PAPI_IO_MTK_CUST_GETNOCLEARMPROPERTIES:        	
            i4Ret = _ioctl_cust_GetNoClearMemoryProperties(arg);
        break;
        case PAPI_IO_MTK_CUST_INSTALLKEYBOARD:        	
            i4Ret = _ioctl_cust_InstallKeyboard(arg);
        break;
	case PAPI_IO_MTK_CUST_SETPANELTIMING:
	    i4Ret = _ioctl_cust_SetPanelTiming(arg);
	break;

    default:
        printk("papi_ioctl_mtk: unknown IO command %u\n", cmd);
        i4Ret = -EFAULT;
    }

    return i4Ret;
}




