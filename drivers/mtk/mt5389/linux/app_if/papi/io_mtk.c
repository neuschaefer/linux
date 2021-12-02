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
 * $RCSfile: io_mtk.c,v $
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
#include "x_pdwnc.h"
#include "x_timer.h"
#include "pdwnc_if.h"
#include "x_assert.h"
#include "x_debug.h"
#include "pdwnc_drvif.h"
#include "io_mtk_t8032fw.c"
#include "x_timeprofile.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define WATCHDOG_MSEC_CNT(x)  (0x0fffffffU - (x * 3000))    
#define WATCHDOG_STARTUP_TIMEOUT (20 * 1000)

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
static UINT32 _u4AliveTimeout = 0;
static UINT32 _u4BoostAliveTimeout = 0;
static HAL_TIME_T _rBoostStartTime;


//-----------------------------------------------------------------------------
// External function prototypes
//-----------------------------------------------------------------------------

extern INT32 _papi_fe_tuner_SetFeDiversity(MTK_CUST_FE_DIV_T* prFeDiv);

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

/* Reserved memory layout
 * +1M: PCL AP use
 * +log buf: printk buffer
 * ~end: fast cold reboot
 */
#define PCL_NO_CLR_MEM_SIZE (1 * 1024 * 1024)

static INT32 _ioctl_cust_GetNoClearMemoryProperties(unsigned long u4Arg)
{
    IOCTL_2ARG_T rArg;
    INT32 i4Ret;
    UINT32 u4Size, u4Addr;

    if (DRVCUST_OptQuery(eReservedMemSize, &u4Size))
    {
        return -1;
    }

    u4Addr = DRVCUST_OptGet(eTotalMemSize);
    u4Addr -= u4Size;

    rArg.au4Arg[0] = u4Addr + PCL_NO_CLR_MEM_SIZE;
    // eNoClearMemSize is total reserved no clear memory size
    // we return only the printk buffer size for AP use
    rArg.au4Arg[1] = (1 << CONFIG_LOG_BUF_SHIFT);
    
    COPY_TO_USER((void*)u4Arg, &rArg, sizeof(IOCTL_2ARG_T));
    
    return i4Ret;
}

static INT32 _ioctl_cust_GetPclNoClearMemoryProperties(unsigned long u4Arg)
{
    IOCTL_2ARG_T rArg;
    INT32 i4Ret;
    UINT32 u4Size, u4Addr;

    if (DRVCUST_OptQuery(eReservedMemSize, &u4Size))
    {
        return -1;
    }

    u4Addr = DRVCUST_OptGet(eTotalMemSize);
    u4Addr -= u4Size;

    rArg.au4Arg[0] = u4Addr;
    rArg.au4Arg[1] = PCL_NO_CLR_MEM_SIZE;
    
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

static INT32 _ioctl_nfr_SetAliveTimeout(unsigned long u4Arg)
{
    UINT32 u4State;
    u4State = HalCriticalStart(); 
    _u4AliveTimeout = u4Arg;    
    PDWNC_WRITE32(REG_RW_WATCHDOG_TMR, WATCHDOG_MSEC_CNT(_u4AliveTimeout));    
    HalCriticalEnd(u4State);    
    
    return 0;
}

static INT32 _ioctl_nfr_FeedAlive(unsigned long u4Arg)
{
    UINT32 u4State;    
    u4State = HalCriticalStart(); 
    PDWNC_WRITE32(REG_RW_WATCHDOG_TMR, WATCHDOG_MSEC_CNT(_u4AliveTimeout));
    HalCriticalEnd(u4State);    
    return 0;
}

static INT32 _ioctl_nfr_BoostAlive(unsigned long u4Arg)
{
    UINT32 u4State;
    u4State = HalCriticalStart(); 
    PDWNC_WRITE32(REG_RW_WATCHDOG_TMR, WATCHDOG_MSEC_CNT(u4Arg));
    HalCriticalEnd(u4State);    
    return 0;
}

static INT32 _ioctl_nfr_ColdReboot(unsigned long u4Arg)
{
    INT32 i4Ret;
    PDWNC_T8032_CMD_T rCmd, rAck; 
    rCmd.u1Cmd = UPCMD_COLDREBOOT;
    i4Ret = PDWNC_T8032Cmd(&rCmd, NULL);
    return i4Ret;
}

extern void activate_fast_cold_reboot(void);
static INT32 _ioctl_nfr_FastColdReboot(unsigned long u4Arg)
{
#ifdef CONFIG_FAST_COLD_REBOOT
    // Call fast cold reboot function
    activate_fast_cold_reboot();

    // Never return, only make gcc happy
    return 0;
#else
    return -1;
#endif // CONFIG_FAST_COLD_REBOOT
}

static INT32 _ioctl_nfr_LoaduPCode(unsigned long u4Arg)
{
    UINT32 u4Size, u4Offset, u4Val;
    if( (u4Size = DRVCUST_InitGet(eT8032uPSize)) != 0)
    {
        int id;
        u4Offset = DRVCUST_InitGet(eT8032uPOffset);
        MEASURE_TIME(id, "PDWNC_InitT8032", PDWNC_InitT8032(u4Offset, u4Size));
        PDWNC_Init();
    }
    return 0;
}

static INT32 _ioctl_nfr_SetStartupTimeout(unsigned long u4Arg)
{
    // Set watchdog timer initial value, 25 seconds.
    PDWNC_WRITE32(REG_RW_WATCHDOG_TMR, WATCHDOG_MSEC_CNT(u4Arg));
    // Enable the watchdog, then watchdog can be disable by outside.
    PDWNC_WRITE32(REG_RW_WATCHDOG_EN, 1);
    return 0;        
}

extern UINT32 CKGEN_GetXtalClock(void);
static INT32 _ioctl_nfr_GetHTimerTicksPerSec(unsigned long u4Arg)
{
    UINT32 u4XtalClock;

    u4XtalClock = CKGEN_GetXtalClock();
    COPY_TO_USER((void*)u4Arg, &u4XtalClock, sizeof(UINT32));

    return 0;
}

static INT32 _ioctl_nfr_GetHTimerCurrentTick(unsigned long u4Arg)
{
    IOCTL_2ARG_T rArg;
    UINT32 u4TickH, u4TickL;

    u4TickH = *((volatile UINT32 *) (0xF0008194));
    u4TickL = *((volatile UINT32 *) (0xF0008074));

    rArg.au4Arg[0] = u4TickH;
    rArg.au4Arg[1] = u4TickL;
    
    COPY_TO_USER((void*)u4Arg, &rArg, sizeof(IOCTL_2ARG_T));
    
    return 0;
}

static INT32 _ioctl_cust_SetFeDiversity(unsigned long u4Arg)
{
    IOCTL_2ARG_T rArg;
    MTK_CUST_FE_DIV_T rFeDiv;

    COPY_FROM_USER(&rArg, u4Arg, sizeof(IOCTL_2ARG_T));
    COPY_FROM_USER(&rFeDiv, rArg.au4Arg[0], rArg.au4Arg[1]);

    /* set fe diversity parameters */
    if (rFeDiv.FeId >= MTK_CUST_FE_NUM) {
        return -EFAULT;
    }

    _papi_fe_tuner_SetFeDiversity(&rFeDiv);
    
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
        case PAPI_IO_MTK_CUST_GETPCLNOCLEARMPROPERTIES:        	
            i4Ret = _ioctl_cust_GetPclNoClearMemoryProperties(arg);
        break;
        case PAPI_IO_MTK_CUST_INSTALLKEYBOARD:        	
            i4Ret = _ioctl_cust_InstallKeyboard(arg);
        break;
        case PAPI_IO_MTK_NFR_SETALIVETIMEOUT:        	
            i4Ret = _ioctl_nfr_SetAliveTimeout(arg);
        break;
        case PAPI_IO_MTK_NFR_FEEDALIVE:        	
            i4Ret = _ioctl_nfr_FeedAlive(arg);
        break;
        case PAPI_IO_MTK_NFR_BOOSTALIVE:        	
            i4Ret = _ioctl_nfr_BoostAlive(arg);
        break;
        case PAPI_IO_MTK_NFR_COLDREBOOT:        	
            i4Ret = _ioctl_nfr_ColdReboot(arg);
        break;        
        case PAPI_IO_MTK_NFR_LOADUPCODE:        	
            i4Ret = _ioctl_nfr_LoaduPCode(arg);
        break;        
        case PAPI_IO_MTK_NFR_SETSTARTUPTIMEOUT:        	
            i4Ret = _ioctl_nfr_SetStartupTimeout(arg);
        break;          
        case PAPI_IO_MTK_NFR_FASTCOLDREBOOT:
            i4Ret = _ioctl_nfr_FastColdReboot(arg);
        break;
        case PAPI_IO_MTK_NFR_GETHTIMERTICKSPERSEC:
            i4Ret = _ioctl_nfr_GetHTimerTicksPerSec(arg);
        break;
        case PAPI_IO_MTK_NFR_GETHTIMERCURRENTTICK:
            i4Ret = _ioctl_nfr_GetHTimerCurrentTick(arg);
        break;
        case PAPI_IO_MTK_CUST_SETFEDIVERSITY:
            i4Ret = _ioctl_cust_SetFeDiversity(arg);
        break;
    default:
        printk("papi_ioctl_mtk: unknown IO command %u\n", cmd);
        i4Ret = -EFAULT;
    }

    return i4Ret;
}




