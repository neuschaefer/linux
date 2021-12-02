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
 * $Author: xiongfeng.wen $
 * $Date: 2012/12/20 $
 * $RCSfile: mtal_if.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file mtal_if.c
 *  MTAL ioctl interface.
 */

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

/// IOCTL debug flag
//#define IOCTL_DEBUG

#ifdef CC_MT5391
// Workaround before PLL related settings being migrated to U-Boot
#define MT8297_SYSPLL_KERNEL_INIT
#endif

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "mtal.h"
#include <linux/version.h>
#include <linux/mtal_ioctl.h>
#include <linux/mm.h>
#include <asm/pgtable.h>

#if 0
#include "panel.h"
#include "dmx_if.h"
#include "aud_if.h"
#include "mpv_if.h"
#include "vdp_if.h"
#include "pmx_if.h"
#endif

#include "mtal_ioctl.h"

#include "x_drv_cli.h"
#include "x_assert.h"
#include "x_linux.h"
#include "x_typedef.h"
#include "x_hal_5381.h"
#include "fbm_drvif.h"

#include "sif_if.h"
#include "eeprom_if.h"

#ifdef MT8297_SYSPLL_KERNEL_INIT
#include "x_ckgen.h"
#endif
#include "drvcust_if.h"

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,36)
#define USE_UNLOCK_IOCTL
#endif

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
EXTERN UINT32 BSP_GetFbmMemAddr(VOID);


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef struct 
{
    UINT32 u4PhyAddr;
    UINT32 u4VirtAddr;    
    UINT32 u4Size;        
} MTAL_BUFFER_T;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define KERNEL_DRAM_SIZE (num_physpages << PAGE_SHIFT)

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
unsigned long u4MtalLogMask = 0;
UINT8          u1IfATVOnlyStatus = 0;

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static MTAL_BUFFER_T _rMtalBuf =
{
    0,
    0,
    0
};

//-----------------------------------------------------------------------------
// Import functions
//-----------------------------------------------------------------------------

extern BOOL VdoMLInit(void);
extern void AUD_OutPadEnable(UINT8 u1DecId, BOOL fgEnable);
extern BOOL _MTPMX_IsPanelSelected(void);
extern void LoadPanelIndex(void);

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
#ifdef MT8297_SYSPLL_KERNEL_INIT
static void _CkgenInit(void)
{
    // turn on main clock for PSPLL PSDDS/NCO clock.
    CKGEN_WRITE32(REG_VPCLK_STOP, CKGEN_READ32(REG_VPCLK_STOP) & ~(CK_MAIN_CLK_STOP));

#if 1 // #ifdef CC_5391_LOADER
    // Modify SIFCFG first.
    SIF_MT8297_DirectWrite8Msk(0x00, 0x01, 0xff);       // 8297 only use 0x50 as device id.

    // Enable SYSPLL1/SYSPLL2 in 8297
    if (XTAL_STRAP_MODE == SET_XTAL_27MHZ)
    {
        // Setup SYSPLL1 540MHz
#ifdef CC_VCXO_ON
        SIF_MT8297_DirectWrite8Msk(0xB4, 0x04, 0x0c);   // SYSPLL1 from VCXO 27MHz
#else /* CC_VCXO_ON */
        SIF_MT8297_DirectWrite8Msk(0xB4, 0x00, 0x0c);   // SYSPLL1 from Xtal 27MHz
#endif /* CC_VCXO_ON */
        SIF_MT8297_DirectWrite8Msk(0x10, 0x33, 0xff);   // SYSPLL1 charge pump and enable
        SIF_MT8297_DirectWrite8Msk(0x11, 0x24, 0xff);   // SYSPLL1 constant gm and band select
        SIF_MT8297_DirectWrite8Msk(0x12, 0x12, 0xff);   // 27MHz*(18+2)
#ifndef CC_DTDPLL_432MHZ        // ATSC/US
        // Setup SYSPLL2 as 600MHz.
        SIF_MT8297_DirectWrite8Msk(0xB4, 0x01, 0x03);   // SYSPLL2 input from SYSPLL1/9 60MHz
        SIF_MT8297_DirectWrite8Msk(0xA8, 0xC4, 0xff);   // SYSPLL2 enable
        SIF_MT8297_DirectWrite8Msk(0xA9, 0x34, 0xff);   // SYSPLL2 charge pump and band select
        SIF_MT8297_DirectWrite8Msk(0xAA, 0x20, 0xff);   // FBDIV 60MHz*(8+2) and turn on postdiv power down
#else /* CC_DTDPLL_432MHZ */    // DVBT/EU
        // Setup SYSPLL2 as 864MHz.
        SIF_MT8297_DirectWrite8Msk(0xB4, 0x00, 0x03);   // SYSPLL2 input from Xtal 27MHz
        SIF_MT8297_DirectWrite8Msk(0xA8, 0xC4, 0xff);   // SYSPLL2 enable
        SIF_MT8297_DirectWrite8Msk(0xA9, 0x34, 0xff);   // SYSPLL2 charge pump and band select
        SIF_MT8297_DirectWrite8Msk(0xAA, 0x78, 0xff);   // FBDIV 27MHz*(30+2) and turn on postdiv power down
#endif /* CC_DTDPLL_432MHZ */
    }
    else if (XTAL_STRAP_MODE == SET_XTAL_60MHZ)
    {
        // Setup SYSPLL1 540MHz
#ifdef CC_VCXO_ON
        SIF_MT8297_DirectWrite8Msk(0xB4, 0x04, 0x0c);   // SYSPLL1 from VCXO 27MHz
        SIF_MT8297_DirectWrite8Msk(0x10, 0x33, 0xff);   // SYSPLL1 charge pump and enable
        SIF_MT8297_DirectWrite8Msk(0x11, 0x24, 0xff);   // SYSPLL1 constant gm and band select
        SIF_MT8297_DirectWrite8Msk(0x12, 0x12, 0xff);   // 27MHz*(18+2)
#else /* CC_VCXO_ON */
        SIF_MT8297_DirectWrite8Msk(0xB4, 0x00, 0x0c);   // SYSPLL1 from Xtal 60MHz
        SIF_MT8297_DirectWrite8Msk(0x10, 0x33, 0xff);   // SYSPLL1 charge pump and enable
        SIF_MT8297_DirectWrite8Msk(0x11, 0x24, 0xff);   // SYSPLL1 constant gm and band select
        SIF_MT8297_DirectWrite8Msk(0x12, 0x07, 0xff);   // 60MHz*(7+2)
#endif /* CC_VCXO_ON */
        // Setup SYSPLL2 as 600MHz.
        SIF_MT8297_DirectWrite8Msk(0xB4, 0x00, 0x03);   // SYSPLL2 input from Xtal 60MHz
        SIF_MT8297_DirectWrite8Msk(0xA8, 0xC4, 0xff);   // SYSPLL2 enable
        SIF_MT8297_DirectWrite8Msk(0xA9, 0x34, 0xff);   // SYSPLL2 charge pump and band select
        SIF_MT8297_DirectWrite8Msk(0xAA, 0x20, 0xff);   // FBDIV 60MHz*(8+2) and turn on postdiv power down
    }
    else if (XTAL_STRAP_MODE == SET_XTAL_48MHZ)
    {
        // Setup SYSPLL2 as 864MHz.
        SIF_MT8297_DirectWrite8Msk(0xB4, 0x00, 0x03);   // SYSPLL2 input from Xtal 48MHz
        SIF_MT8297_DirectWrite8Msk(0xA8, 0xC4, 0xff);   // SYSPLL2 enable
        SIF_MT8297_DirectWrite8Msk(0xA9, 0x34, 0xff);   // SYSPLL2 charge pump and band select
        SIF_MT8297_DirectWrite8Msk(0xAA, 0x40, 0xff);   // FBDIV 48MHz*(16+2) and turn on postdiv power down
        // Setup SYSPLL1 540MHz
#ifdef CC_VCXO_ON
        SIF_MT8297_DirectWrite8Msk(0xB4, 0x04, 0x0c);   // SYSPLL1 from VCXO 27MHz
        SIF_MT8297_DirectWrite8Msk(0x10, 0x33, 0xff);   // SYSPLL1 charge pump and enable
        SIF_MT8297_DirectWrite8Msk(0x11, 0x24, 0xff);   // SYSPLL1 constant gm and band select
        SIF_MT8297_DirectWrite8Msk(0x12, 0x12, 0xff);   // 27MHz*(18+2)
#else /* CC_VCXO_ON */
        SIF_MT8297_DirectWrite8Msk(0xB4, 0x08, 0x0c);   // SYSPLL1 from SYSPLL2/16=54MHz
        SIF_MT8297_DirectWrite8Msk(0x10, 0x33, 0xff);   // SYSPLL1 charge pump and enable
        SIF_MT8297_DirectWrite8Msk(0x11, 0x24, 0xff);   // SYSPLL1 constant gm and band select
        SIF_MT8297_DirectWrite8Msk(0x12, 0x08, 0xff);   // 54MHz*(8+2)
#endif /* CC_VCXO_ON */
    }
    else if (XTAL_STRAP_MODE == SET_XTAL_54MHZ)
    {
        // Setup SYSPLL1 540MHz
#ifdef CC_VCXO_ON
        SIF_MT8297_DirectWrite8Msk(0xB4, 0x04, 0x0c);   // SYSPLL1 from VCXO 27MHz
#else /* CC_VCXO_ON */
        SIF_MT8297_DirectWrite8Msk(0xB4, 0x00, 0x0c);   // SYSPLL1 from Xtal 27MHz
#endif /* CC_VCXO_ON */
        SIF_MT8297_DirectWrite8Msk(0x10, 0x33, 0xff);   // SYSPLL1 charge pump and enable
        SIF_MT8297_DirectWrite8Msk(0x11, 0x24, 0xff);   // SYSPLL1 constant gm and band select
#ifdef CC_VCXO_ON
        SIF_MT8297_DirectWrite8Msk(0x12, 0x12, 0xff);   // 27MHz*(18+2)
#else /* CC_VCXO_ON */
        SIF_MT8297_DirectWrite8Msk(0x12, 0x08, 0xff);   // 54MHz*(8+2)
#endif /* CC_VCXO_ON */

#ifndef CC_DTDPLL_432MHZ        // ATSC/US
        // Setup SYSPLL2 as 600MHz.
        SIF_MT8297_DirectWrite8Msk(0xB9, 0x1C, 0xFC);   // SYSPLL1/9 (0x1C >> 2)=7 => 540/(7+2)=60MHz
        SIF_MT8297_DirectWrite8Msk(0xB4, 0x01, 0x03);   // SYSPLL2 input from SYSPLL1/9 60MHz
        SIF_MT8297_DirectWrite8Msk(0xA8, 0xC4, 0xff);   // SYSPLL2 enable
        SIF_MT8297_DirectWrite8Msk(0xA9, 0x34, 0xff);   // SYSPLL2 charge pump and band select
        SIF_MT8297_DirectWrite8Msk(0xAA, 0x20, 0xff);   // FBDIV 60MHz*(8+2) and turn on postdiv power down
#else /* CC_DTDPLL_432MHZ */    // DVBT/EU
        // Setup SYSPLL2 as 864MHz.
        SIF_MT8297_DirectWrite8Msk(0xB4, 0x00, 0x03);   // SYSPLL2 input from Xtal 54MHz
        SIF_MT8297_DirectWrite8Msk(0xA8, 0xC4, 0xff);   // SYSPLL2 enable
        SIF_MT8297_DirectWrite8Msk(0xA9, 0x34, 0xff);   // SYSPLL2 charge pump and band select
        SIF_MT8297_DirectWrite8Msk(0xAA, 0x38, 0xff);   // FBDIV 27MHz*(14+2) and turn on postdiv power down
#endif /* CC_DTDPLL_432MHZ */
    }
    else
    {
        while (1) { }
    }
    SIF_MT8297_DirectWrite8Msk(0xB4, 0x20, 0x20);   // Turn on digit postdiv power down
    SIF_MT8297_DirectWrite8Msk(0xB7, 0x00, 0x07);   // Turn on digit postdiv power down
#endif /* CC_5391_LOADER */
}
#endif

MT_RESULT_T MTAL_Init(void)
{
    static BOOL fgMtalInitFinish = FALSE, fgMtalInitStart = FALSE;
    BOOL fgWait = FALSE;
    CRIT_STATE_T rCritState;

    rCritState = x_crit_start();
    if (fgMtalInitFinish)
    {
        x_crit_end(rCritState);
        return MTR_OK;
    }

    if (!fgMtalInitStart) {
        fgMtalInitStart = 1;
    }
    else
    {
        fgWait = TRUE;
    }
    x_crit_end(rCritState);

#ifdef MT8297_SYSPLL_KERNEL_INIT
    _CkgenInit();
#endif

    if (!fgWait)
    {
        // Should implement panel select customization
        // LoadPanelIndex();
        if(!_MTPMX_IsPanelSelected())
        {
       	 LoadPanelIndex();
        }
        	
        FBM_Init();
#if 0

        DMX_Init();

        AUD_Init();
        AUD_OutPadEnable(AUD_DEC_MAIN, TRUE);
        AUD_OutPadEnable(AUD_DEC_AUX, TRUE);
        MPV_Init();
        //SelectPanel(0);
#endif
       //VDP_Init();
		SIF_Init();
        EEPROM_Init();
        //VdoMLInit();
#if 0
        // Change Scaler to Pixel Based
        VDP_SetScalerPixelBased(1);

        PMX_Init();
        PMX_SetEnable(0, 1);
#endif
        fgMtalInitFinish = TRUE;
    }
    else
    {
        /* Wait until MTAL_Init() finish, which is invoked by other thread. */
        while (!fgMtalInitFinish)
        {
            static UINT32 u4Count=0;
            x_thread_delay(10);
            u4Count++;
            ASSERT(u4Count < 1000);
        }
    }
    return MTR_OK;
}

static int mtal_open(struct inode * inode, struct file * filp)
{
    MTAL_Init();

    return 0;
}

#ifndef USE_UNLOCK_IOCTL
static int mtal_ioctl(struct inode *inode, struct file *file, unsigned int cmd,
              unsigned long arg)
#else
static long mtal_ioctl(struct file *file, unsigned int cmd,
              unsigned long arg)
#endif
{
#ifdef USE_UNLOCK_IOCTL
    struct inode *inode = file->f_dentry->d_inode;
#endif

    int retval = 0;
    switch (_IOC_TYPE(cmd))
    {
    case MTAL_IOCTYPE_MTADAC:
        retval = mtal_ioctl_mtadac(inode, file, cmd, arg);
        break;

    case MTAL_IOCTYPE_MTAUDDEC:
        retval = mtal_ioctl_mtauddec(inode, file, cmd, arg);
        break;

    case MTAL_IOCTYPE_MTDMX:
        retval = mtal_ioctl_mtdmx(inode, file, cmd, arg);
        break;
#ifdef CC_FEDTD
    case MTAL_IOCTYPE_MTFE_ATSC:
        retval = mtal_ioctl_mtfe_atsc(inode, file, cmd, arg);
        break;
    case MTAL_IOCTYPE_MTFE_NTSC:
        retval = mtal_ioctl_mtfe_ntsc(inode, file, cmd, arg);
        break;
#endif

#ifdef CC_FEISDB
    case MTAL_IOCTYPE_MTFE_ISDBT:
        retval = mtal_ioctl_mtfe_isdbt(inode, file, cmd, arg);
        break;
    case MTAL_IOCTYPE_MTFE_ISDBS:
        retval = mtal_ioctl_mtfe_isdbs(inode, file, cmd, arg);
        break;
    case MTAL_IOCTYPE_MTFE_NTSC:
        retval = mtal_ioctl_mtfe_ntsc(inode, file, cmd, arg);
        break;
#endif

#ifdef CC_FEDVBT
    case MTAL_IOCTYPE_MTFE_DVBT:
        retval = mtal_ioctl_mtfe_dvbt(inode, file, cmd, arg);
            break;
    case MTAL_IOCTYPE_MTFE_PAL:
        retval = mtal_ioctl_mtfe_pal(inode, file, cmd, arg);
        break;
#endif

#ifdef CC_FEDVBC
    case MTAL_IOCTYPE_MTFE_DVBC:
        retval = mtal_ioctl_mtfe_dvbc(inode, file, cmd, arg);
        break;
	#ifndef CC_FEDVBT
    case MTAL_IOCTYPE_MTFE_PAL:
        retval = mtal_ioctl_mtfe_pal(inode, file, cmd, arg);
        break;
	#endif
#endif
        
#ifdef CC_FEDTMB      
   case MTAL_IOCTYPE_MTFE_PAL:
        retval = mtal_ioctl_mtfe_pal(inode, file, cmd, arg);
        break;
	 case MTAL_IOCTYPE_MTFE_DTMB:
		    retval = mtal_ioctl_mtfe_dtmb(inode, file, cmd, arg);
		    break;
		    
#endif

#ifdef CC_FESBTVD
   case MTAL_IOCTYPE_MTFE_SBTVD:
        retval = mtal_ioctl_mtfe_sbtvd(inode, file, cmd, arg);
        break;
    case MTAL_IOCTYPE_MTFE_NTSC:
        retval = mtal_ioctl_mtfe_ntsc(inode, file, cmd, arg);
        break;

#endif
#if   defined(ENABLE_SIF_BYPASS) && !defined(CC_FEDVBT) && !defined(CC_FEDVBC)//common ATD
	case MTAL_IOCTYPE_MTFE_PAL:
        retval = mtal_ioctl_mtfe_pal(inode, file, cmd, arg);
        break;
#endif	
    case MTAL_IOCTYPE_MTGFX:
        retval = mtal_ioctl_mtgfx(inode, file, cmd, arg);
        break;

    case MTAL_IOCTYPE_MTMPLAYER:
        retval = mtal_ioctl_mtmplayer(inode, file, cmd, arg);
        break;

    case MTAL_IOCTYPE_MTIMAGE:
        retval = mtal_ioctl_mtimage(inode, file, cmd, arg);
        break;

    case MTAL_IOCTYPE_MTOSD:
        retval = mtal_ioctl_mtosd(inode, file, cmd, arg);
        break;

    case MTAL_IOCTYPE_MTPERIPHERAL:
        retval = mtal_ioctl_mtperipheral(inode, file, cmd, arg);
        break;

    case MTAL_IOCTYPE_MTPMX:
        retval = mtal_ioctl_mtpmx(inode, file, cmd, arg);
        break;

    case MTAL_IOCTYPE_MTSCART:
        retval = mtal_ioctl_mtscart(inode, file, cmd, arg);
        break;

    case MTAL_IOCTYPE_MTVDEC:
        retval = mtal_ioctl_mtvdec(inode, file, cmd, arg);
        break;
        
    case MTAL_IOCTYPE_MTVDECEX:
        retval = mtal_ioctl_mtvdecex(inode, file, cmd, arg);
        break;

    case MTAL_IOCTYPE_MTVDO:
        retval = mtal_ioctl_mtvdo(inode, file, cmd, arg);
        break;

    case MTAL_IOCTYPE_MTVFE:
        retval = mtal_ioctl_mtvfe(inode, file, cmd, arg);
        break;

#ifndef CC_DISCARD_SUPPORT_PCMCIA
    case MTAL_IOCTYPE_MTCI:
        retval = mtal_ioctl_mtci(inode, file, cmd, arg);
        break;
#endif

    case MTAL_IOCTYPE_MTVDECEX_VBI:
        retval = mtal_ioctl_mtvbiex(inode, file, cmd, arg);
        break;

    case MTAL_IOCTYPE_MTTVE:
        retval = mtal_ioctl_mttve(inode, file, cmd, arg);
        break;

    case MTAL_IOCTYPE_MTIOMMU:
        retval = mtal_ioctl_mtmmu(inode, file, cmd, arg);
        break;
    case MTAL_IOCTYPE_MTSTC:
        retval = mtal_ioctl_mtstc(inode, file, cmd, arg);
        break;
    case MTAL_IOCTYPE_MTB2R:        
        retval = mtal_ioctl_mtb2r(inode, file, cmd, arg);
        break;
    case MTAL_IOCTYPE_MTPVR:
        retval = mtal_ioctl_mtpvr(inode, file, cmd, arg);
        break;
    case MTAL_IOCTYPE_MTVSS:
        retval = mtal_ioctl_mtvss(inode, file, cmd, arg);
        break;        

    default:
        break;
    }

#ifdef IOCTL_DEBUG
    printk("IOCTL cmd: 0x%08x, arg: 0x%08x, ret: %d\n", cmd, arg, retval);
#endif

    return retval;
}


static int mtal_mmap(struct file *file, struct vm_area_struct *vma)
{
    unsigned long off, start =0x0; //Maybe need to update later.
    unsigned long len, Vbioffset;
    FBM_POOL_T *pVBI_Pool_Info;
    

    pVBI_Pool_Info = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_VBI);
    Vbioffset = pVBI_Pool_Info->u4Addr % 0x1000 ;

    // Ensure page alignment
    VERIFY(PAGE_ALIGN(DRVCUST_OptGet(eFBMemSize) & ~PAGE_MASK) == 0);      
    VERIFY(PAGE_ALIGN(DRVCUST_OptGet(eDirectFBMemSize) & ~PAGE_MASK) == 0);   
    
    _rMtalBuf.u4PhyAddr = BSP_GetFbmMemAddr() - (DRVCUST_OptGet(eFBMemSize) + DRVCUST_OptGet(eDirectFBMemSize));
    _rMtalBuf.u4Size = DRVCUST_OptGet(eFBMemSize);

    off = vma->vm_pgoff << PAGE_SHIFT;

    start = _rMtalBuf.u4PhyAddr;
    len = PAGE_ALIGN(start & ~PAGE_MASK) + _rMtalBuf.u4Size;
    start &= PAGE_MASK;
    

    if (off != (pVBI_Pool_Info->u4Addr - Vbioffset)  )
    {
        off += start;
    }

    vma->vm_pgoff = off >> PAGE_SHIFT;

    /* This is an IO map - tell maydump to skip this VMA */
    vma->vm_flags |= VM_IO;

    /*
     * Don't alter the page protection flags; we want to keep the area
     * cached for better performance.  This does mean that we may miss
     * some updates to the screen occasionally, but process switches
     * should cause the caches and buffers to be flushed often enough.
     */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,18)     
    if(io_remap_pfn_range(vma, vma->vm_start, off >> PAGE_SHIFT,
                vma->vm_end - vma->vm_start,
                vma->vm_page_prot))         
#else
    if(io_remap_page_range(vma, vma->vm_start, off,
                vma->vm_end - vma->vm_start,
                vma->vm_page_prot))         
#endif                                       
    {
    	return -EAGAIN;
    }

    return 0;
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

struct file_operations mtal_fops = {
    .open       = mtal_open,
#ifndef USE_UNLOCK_IOCTL
    .ioctl      = mtal_ioctl,
#else
    .unlocked_ioctl = mtal_ioctl,
#endif
    .mmap       = mtal_mmap,
};


