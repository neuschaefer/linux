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
 * $Author: kai.wang $
 * $Date: 2012/07/23 $
 * $RCSfile: pdwnc_gpio.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file pdwnc_gpio.c
 *  pdwnc_gpio.c The PDWNC (Power DoWN Controller) GPIO functions
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#ifdef CC_UP8032_ATV
#include "general_mt82.h"
#include "x_pinmux_mt82.h"
#include "x_pdwnc_mt82.h"
#include "hw_pdwnc.h"
#include "x_assert.h"
#include "x_ckgen_mt82.h"
#include "x_gpio_mt82.h"
#include "pdwnc_config.h" //XuYing - 090421
#include "pdwnc_drvif.h"
#else
#include "x_pinmux.h"
#include "x_pdwnc.h"
#include "x_bim.h"
#include "x_assert.h"
#include "x_os.h"
#include "x_ckgen.h"
#include "x_timer.h"
#include "x_hal_arm.h"
#include "x_gpio.h"
#include "drvcust_if.h"
#include "pdwnc_config.h"
#include "pdwnc_debug.h"
#include "pdwnc_hw.h"
#include "pdwnc_t8032.h"
#include "pdwnc_srvad.h"
#include "pdwnc_if.h"
#include "sif_if.h"
#endif
#ifndef CC_MTK_LOADER
#ifdef __KERNEL__
#include "cb_data.h"
#include "mtperipheral.h"
#endif
#endif
#undef PDWNC_WRITE32
#undef PDWNC_READ32

//-----------------------------------------------------------------------------
// Configuration
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#if defined(CC_MT5368)
#define PDWNC_GPIO_INT0_BASE 0
#define PDWNC_GPIO_INT_GROUP_0_NUM 8

#define PDWNC_GPIO_INT1_BASE 16
#define PDWNC_GPIO_INT_GROUP_1_NUM 4

#define PDWNC_GPIO_INT2_BASE 25
#define PDWNC_GPIO_INT_GROUP_2_NUM 7

#elif defined(CC_MT5396)
#define PDWNC_GPIO_INT0_BASE 0
#define PDWNC_GPIO_INT_GROUP_0_NUM 8

#define PDWNC_GPIO_INT1_BASE 16
#define PDWNC_GPIO_INT_GROUP_1_NUM 4

#define PDWNC_GPIO_INT2_BASE 26
#define PDWNC_GPIO_INT_GROUP_2_NUM 6

#elif defined(CC_MT5389)
#define PDWNC_GPIO_INT0_BASE 0
#define PDWNC_GPIO_INT_GROUP_0_NUM 8
#endif

//#define PDWNC_GPIO_INT_GROUP_2_BEGIN 8 //david : in 280B0, EXIT2[0] ==> hdmi_hpd1 is the 8rd bit of 28080
#define PDWNC_SRVAD_GPIO_BEGIN 33
#define PDWNC_SRVAD_INT_BEGIN 28
//-----------------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
#ifndef CC_MTK_LOADER
// PDWNC module sub-interrupt routine list.
static PFN_GPIO_CALLBACK _afnPDWNCGpioCallback[TOTAL_PDWNC_GPIO_NUM];
#if defined(CC_MT5396)
static const INT8 _ai1PdwncInt2Gpio[MAX_PDWNC_INT_ID] =
{//check 2804c[7:0] [18] and [24:31], translate them to bit offset within 280A8(offset of sysytem gpio number), only 17 gpio can issue interrupt
    0, 1, 2, 3, 4, 5, 6, 7,
   -1, -1, -1, -1, -1, -1, -1, -1,
   8, 9, 10, 11, -1, -1, -1, -1,
   -1, -1, 13, 15, 33, 34, 35,36   
};
#elif defined(CC_MT5389)
static const INT8 _ai1PdwncInt2Gpio[MAX_PDWNC_INT_ID] =
{//check 2804c[7:0] [18] and [24:31], translate them to bit offset within 280A8(offset of sysytem gpio number), only 17 gpio can issue interrupt
    0, 1, 2, 3, 4, 5, 17, 18,
   -1, -1, -1, -1, -1, -1, -1, -1,
   -1, -1, -1, -1, -1, -1, -1, -1,
   -1, -1, -1
};
#else	//defined(CC_MT5368)
static const INT8 _ai1PdwncInt2Gpio[MAX_PDWNC_INT_ID] =
{//check 2804c[7:0] [18] and [24:31], translate them to bit offset within 280A8(offset of sysytem gpio number), only 17 gpio can issue interrupt
    0, 1, 2, 3, 4, 5, 6, 7,
   -1, -1, -1, -1, -1, -1, -1, -1,
   8, 9, 10, 11, -1, -1, -1, -1,
   -1, 13, 14, 15, 33, 34, 35,36
};
#endif
#endif

#if defined(CC_MT5396)
static const INT8 _ai1PdwncGpio2Int[TOTAL_PDWNC_GPIO_NUM] =
{
	0, 1, 2, 3, 4, 5, 6, 7,
   16, 17, 18, 19, -1, 26, -1, 27,
   -1, -1, -1, -1, -1, -1, -1, -1,
   -1, -1, -1, -1, -1, -1, -1, -1,
   -1, 28, 29, 30, 31, -1, -1, -1,
   -1
};
#elif defined(CC_MT5389)
static const INT8 _ai1PdwncGpio2Int[TOTAL_PDWNC_GPIO_NUM] =
{
    0, 1, 2, 3, 4, 5, -1, -1,
   -1, -1, -1, -1, -1, -1, -1, -1, 
   -1, 6, 7
};
#else	//defined(CC_MT5368)
static const INT8 _ai1PdwncGpio2Int[TOTAL_PDWNC_GPIO_NUM] =
{
    0, 1, 2, 3, 4, 5, 6, 7,
   16, 17, 18, 19, -1, 25, 26, 27,
   -1, -1, -1, -1, -1, -1, -1, -1, 
   -1, -1, -1, -1, -1, -1, -1, -1,
   -1, 28, 29, 30, 31
};
#endif

#if defined(CC_MT5396)
static const INT8 _ai1PdwncGpio2Pad[TOTAL_PDWNC_GPIO_NUM] =
{//There are some reserved bit field in 28080 of mt536x, so the 'offset of system gpio number' needs to be translated to the correct bit field of 28080
    0, 1, 2, 3, 4, 5, 6, 7,
    8, 9, 10, 11, 12, 13,14, 15, 
    16, 17, 18, 19, 20, 21, 22, 23,
    24, 25, 26, 27, 28, 29, 30, 31,
    32, 33, 34, 35, 36, 37, 38, 39,
    40
};
#elif defined(CC_MT5389)
static const INT8 _ai1PdwncGpio2Pad[TOTAL_PDWNC_GPIO_NUM] =
{//There are some reserved bit field in 28080 of mt536x, so the 'offset of system gpio number' needs to be translated to the correct bit field of 28080
    0, 1, 2, 3, 4, 5, 6, 7,
    8, 9, 10, 11, 12, 13,14, 15, 
    16, 17, 18   
};
#else	//defined(CC_MT5368)
static const INT8 _ai1PdwncGpio2Pad[TOTAL_PDWNC_GPIO_NUM] =
{//There are some reserved bit field in 28080 of mt536x, so the 'offset of system gpio number' needs to be translated to the correct bit field of 28080
    0, 1, 2, 3, 4, 5, 6, 7,
    8, 9, 10, 11, 12, 13,14, 15, 
    16, 17, 18, 19, 20, 21, 22, 23,
    24, 25, 26, 27, 28, 29, 30, 31,
    32, 33, 34, 35, 36
};
#endif
#ifdef __KERNEL__
static UINT32 _au4PdwncGpioCallBackArg[TOTAL_PDWNC_GPIO_NUM];
static UINT8 _au4PdwncCallBackFromLinuxAP[TOTAL_PDWNC_GPIO_NUM];
#endif
//-----------------------------------------------------------------------------
// Prototypes 
//-----------------------------------------------------------------------------
//INT32 PDWNC_GpioEnable(INT32 i4Gpio, INT32 *pfgSet);
//INT32 PDWNC_GpioOutput(INT32 i4Gpio, INT32 *pfgSet);
//INT32 PDWNC_GpioInput(INT32 i4Gpio);
#ifndef CC_MTK_LOADER
INT32 PDWNC_GpioIntrq(INT32 i4Gpio, INT32 *pfgSet);
INT32 PDWNC_GpioQuery(INT32 i4Gpio, INT32 *pi4Intr, INT32 *pi4Fall, INT32 *pi4Rise);
INT32 PDWNC_GpioReg(INT32 i4Gpio, GPIO_TYPE eType, PFN_GPIO_CALLBACK pfnCallback);
INT32 PDWNC_T8032GpioReg(INT32 i4Gpio, GPIO_TYPE eType, PFN_GPIO_CALLBACK pfnCallback);
#endif


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
#ifndef CC_MTK_LOADER

#ifdef __KERNEL__  

extern INT32 _CB_PutEvent(CB_FCT_ID_T eFctId, INT32 i4TagSize, void *pvTag);

#endif

//-----------------------------------------------------------------------------
/** _PdwncGpioIsr() is the interrupt routine for GPIO pins of PDWNC module.
 *  @param u2IntIndex is the gpio number in PDWNC module to trigger interrupt.
 */
//-----------------------------------------------------------------------------
static void _PdwncGpioIsr(UINT16 u2IntIndex)
{
    BOOL fgStatus;
    UNUSED(_ai1PdwncInt2Gpio);
	#if defined(CC_MT5389)	
    if (((u2IntIndex < PDWNC_GPIO_INT_GROUP_0_NUM)) && (_afnPDWNCGpioCallback[_ai1PdwncInt2Gpio[u2IntIndex]]!=NULL))
	#else
    if (
        (
        (u2IntIndex < PDWNC_GPIO_INT_GROUP_0_NUM) \
        || ((u2IntIndex >= PDWNC_GPIO_INT1_BASE) && (u2IntIndex < PDWNC_GPIO_INT1_BASE + PDWNC_GPIO_INT_GROUP_1_NUM)) \
        || ((u2IntIndex >= PDWNC_GPIO_INT2_BASE) && (u2IntIndex < PDWNC_GPIO_INT2_BASE + PDWNC_GPIO_INT_GROUP_2_NUM))
        )
        &&
        (_afnPDWNCGpioCallback[_ai1PdwncInt2Gpio[u2IntIndex]]!=NULL))
    #endif
    {
        fgStatus = PDWNC_GpioInput((INT32)_ai1PdwncInt2Gpio[u2IntIndex]);
//#if defined(CC_MT5365) || defined(CC_MT5395)
			if((u2IntIndex >= PDWNC_SRVAD_INT_BEGIN) && (u2IntIndex <= PDWNC_SRVAD_INT_BEGIN + (ADC2GPIO_CH_ID_MAX - ADC2GPIO_CH_ID_MIN)))
			#ifdef __KERNEL__                    
            if(!_au4PdwncCallBackFromLinuxAP[_ai1PdwncInt2Gpio[u2IntIndex]])
            {
                _afnPDWNCGpioCallback[_ai1PdwncInt2Gpio[u2IntIndex]]((INT32)((u2IntIndex - PDWNC_SRVAD_INT_BEGIN+(ADC2GPIO_CH_ID_MAX - ADC2GPIO_CH_ID_MIN)) ), fgStatus);
            }
            else
            {
                MTGPIO_CB_T rGpioCb;
                rGpioCb.i4Gpio = ((u2IntIndex - PDWNC_SRVAD_INT_BEGIN) + ADIN(ADC2GPIO_CH_ID_MIN));
                rGpioCb.fgStatus = fgStatus;
                rGpioCb.u4Pfn =(UINT32) _afnPDWNCGpioCallback[_ai1PdwncInt2Gpio[u2IntIndex]];
                rGpioCb.u4Arg = _au4PdwncGpioCallBackArg[_ai1PdwncInt2Gpio[u2IntIndex]];
               // printf( "_PdwncGpioIsr: gpio_%d, fgStatus: %d\n", rGpioCb.i4Gpio, rGpioCb.fgStatus);
                _CB_PutEvent(CB_MTAL_GPIO_TRIGGER, sizeof(MTGPIO_CB_T), &rGpioCb);
            }
            #else	
			
			_afnPDWNCGpioCallback[_ai1PdwncInt2Gpio[u2IntIndex]]((INT32)((u2IntIndex - PDWNC_SRVAD_INT_BEGIN+(ADC2GPIO_CH_ID_MAX - ADC2GPIO_CH_ID_MIN)) ), fgStatus);
			#endif
		else
			
		#ifdef __KERNEL__					 
		if(!_au4PdwncCallBackFromLinuxAP[_ai1PdwncInt2Gpio[u2IntIndex]])
		{
			_afnPDWNCGpioCallback[_ai1PdwncInt2Gpio[u2IntIndex]]((INT32)(_ai1PdwncInt2Gpio[u2IntIndex] + OPCTRL(0)), fgStatus);
		}
		else
		{
			MTGPIO_CB_T rGpioCb;
			rGpioCb.i4Gpio = (INT32)(_ai1PdwncInt2Gpio[u2IntIndex] + OPCTRL(0));
			rGpioCb.fgStatus = fgStatus;
			rGpioCb.u4Pfn = (UINT32)_afnPDWNCGpioCallback[_ai1PdwncInt2Gpio[u2IntIndex]];
			rGpioCb.u4Arg = _au4PdwncGpioCallBackArg[_ai1PdwncInt2Gpio[u2IntIndex]];
			//printf("_PdwncGpioIsr: gpio_%d, fgStatus: %d\n", rGpioCb.i4Gpio, rGpioCb.fgStatus);
			_CB_PutEvent(CB_MTAL_GPIO_TRIGGER, sizeof(MTGPIO_CB_T), &rGpioCb);
		}
		#else
		_afnPDWNCGpioCallback[_ai1PdwncInt2Gpio[u2IntIndex]]((INT32)(_ai1PdwncInt2Gpio[u2IntIndex] + OPCTRL(0)), fgStatus); 
		#endif	
		//#else//MT5368
		//		  _afnPDWNCGpioCallback[_ai1PdwncInt2Gpio[u2IntIndex]]((INT32)(
	//	_ai1PdwncInt2Gpio[u2IntIndex] + OPCTRL(0)), fgStatus);
		//#endif

    }

    // Still need to clear this non-hook interrupt.
    vIO32Write4B(PDWNC_INTCLR,  _PINT(u2IntIndex));
}
#endif

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _PDWNC_InitGpio(). Initialize the interrupt routine for GPIO irq.
 *  @param void
 */
//-----------------------------------------------------------------------------
#if 1//defined(CC_MT5365) || defined(CC_MT5395) //change after 5365/95/...
#if 0
static UINT32 u4PDWNC_GPIO_INTEN_BACKUP;
static UINT32 u4PDWNC_GPIO_EXINTCFG_BACKUP;
static UINT32 u4PDWNC_GPIO_EXTIO2INTCFG_BACKUP;
void PDWNC_GPIO_Suspend(void)
{
        // Backup interupt register.
    u4PDWNC_GPIO_INTEN_BACKUP = u4IO32Read4B(PDWNC_ARM_INTEN);
    u4PDWNC_GPIO_EXINTCFG_BACKUP = u4IO32Read4B(PDWNC_EXINTCFG);
    u4PDWNC_GPIO_EXTIO2INTCFG_BACKUP = u4IO32Read4B(PDWNC_EXTIO2INTCFG);
}

void PDWNC_GPIO_Resume(void)
{
        // Initialize interupt register.
    vIO32Write4B(PDWNC_INTCLR, REG_RW_PDWNC_INTEN_GPIO_MASK); //david: just clean gpio int.        
    vIO32Write4B(PDWNC_ARM_INTEN, u4PDWNC_GPIO_INTEN_BACKUP & (~(REG_RW_PDWNC_INTEN_GPIO_MASK)) );
    vIO32Write4B(PDWNC_EXINTCFG, u4PDWNC_GPIO_EXINTCFG_BACKUP);
    vIO32Write4B(PDWNC_EXTIO2INTCFG, u4PDWNC_GPIO_EXTIO2INTCFG_BACKUP);
    //vIO32Write4B(PDWNC_INTCLR, REG_RW_PDWNC_INTEN_GPIO_MASK); //david: just clean gpio int.
}
#else
void PDWNC_GPIO_Suspend(void)
{

}

void PDWNC_GPIO_Resume(void)
{
    vIO32Write4B(PDWNC_ARM_INTEN, u4IO32Read4B(PDWNC_ARM_INTEN) & (~(REG_RW_PDWNC_INTEN_GPIO_MASK)) );

    vIO32Write4B(PDWNC_EXINT2ED, 0);
    vIO32Write4B(PDWNC_EXINTLEV, 0);
    vIO32Write4B(PDWNC_EXINTPOL, 0);
    vIO32Write4B(PDWNC_INTCLR, REG_RW_PDWNC_INTEN_GPIO_MASK); //david: just clean gpio int.
}
#endif
#else
void PDWNC_GPIO_Suspend(void)
{

}
void PDWNC_GPIO_Resume(void)
{
}
#endif
#ifdef __KERNEL__
INT32 PDWNC_GpioSetIntExtParam(INT32 i4Gpio, UINT32 u4Arg)
{
    _au4PdwncCallBackFromLinuxAP[i4Gpio] = 1;
    if(_ai1PdwncGpio2Int[i4Gpio] != -1)
    {
        _au4PdwncGpioCallBackArg[i4Gpio] = u4Arg;
        return 0;
    }
    else
    {
        return -1;        
    }
}

INT32 PDWNC_GpioGetIntExtParam(INT32 i4Gpio, UINT32 *pu4Arg)
{
    if(_ai1PdwncGpio2Int[i4Gpio] != -1)
    {
        *pu4Arg = _au4PdwncGpioCallBackArg[i4Gpio];
        return 0;
    }
    else
    {
        return -1;        
    }
}
#endif

#ifndef CC_MTK_LOADER
//-----------------------------------------------------------------------------
/** _PDWNC_InitGpio(). Initialize the interrupt routine for GPIO irq.
 *  @param void
 */
//-----------------------------------------------------------------------------
INT32 _PDWNC_InitGpio(void)
{
    UINT32 i;
    INT32 i4Val;
    void (* pfnOldIsr)(UINT16);
    // Clear callback function array.
    for (i=0; i<TOTAL_PDWNC_GPIO_NUM;i++)
    {
        _afnPDWNCGpioCallback[i] = NULL;
#ifdef __KERNEL__        
        _au4PdwncGpioCallBackArg[i] = 0;
        _au4PdwncCallBackFromLinuxAP[i] = 0;
#endif        
    }
    // Initialize interupt register.
    vIO32Write4B(PDWNC_ARM_INTEN, u4IO32Read4B(PDWNC_ARM_INTEN) & (~(REG_RW_PDWNC_INTEN_GPIO_MASK)) );
    vIO32Write4B(PDWNC_EXINT2ED, 0);
    vIO32Write4B(PDWNC_EXINTLEV, 0);
    vIO32Write4B(PDWNC_EXINTPOL, 0);    
    vIO32Write4B(PDWNC_INTCLR, REG_RW_PDWNC_INTEN_GPIO_MASK); //david: just clean gpio int.

    // Hook PDWNC Gpio interrupt routine.
    for (i=0; i < TOTAL_PDWNC_GPIO_NUM; i++)
    {
        if(_ai1PdwncGpio2Int[PINTNO_OPCTRL0 + i] >= 0)
        {
            i4Val = PDWNC_RegIsr((UINT16)_ai1PdwncGpio2Int[PINTNO_OPCTRL0 + i], _PdwncGpioIsr, &pfnOldIsr);
            if (i4Val != PDWNC_OK)
            {
                return -1;
            }
        }
    }
    return 0;
}
#endif


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

INT32 PDWNC_GpioRangeCheck(INT32 i4Gpio)
{
		if ((i4Gpio >= OPCTRL(0)) && (i4Gpio <= (TOTAL_PDWNC_GPIO_NUM + OPCTRL(0))))
		{
			return 1;
		}
		else
		{	
			return 0;
		}	
}
//-----------------------------------------------------------------------------
/** PDWNC_GpioEnable() The GPIO input/output mode setting functions. It will
 *  check the i4Gpio and set to related register bit as 0 or 1.  In this 
 *  function, 0 is input mode and 1 is output mode.
 *  @param i4Gpio the gpio number to set or read.
 *  @param pfgSet A integer pointer.  If it's NULL, this function will return
 *  the current mode of gpio number (0 or 1).  If it's not NULL, it must
 *  reference to a integer.  If the integer is 0, this function will set the
 *  mode of the gpio number as input mode, otherwise set as output mode.
 *  @retval If pfgSet is NULL, it return 0 or 1 (0 is input mode, 1 is output
 *          mode.)  Otherwise, return (*pfgSet).
 */
//-----------------------------------------------------------------------------
INT32 PDWNC_GpioEnable(INT32 i4Gpio, INT32 *pfgSet)
{
    UINT32 u4Val;
    INT32 i4Idx;
#ifndef CC_MTK_LOADER
        CRIT_STATE_T rCrit;
#else /* CC_MTK_LOADER */
        //UINT32 u4State;
#endif
    if ((i4Gpio < 0) || (i4Gpio >= TOTAL_PDWNC_GPIO_NUM))
    {
        return -1;
    }

    // Translate GPIO number to pad setting register offset
    i4Idx = (INT32)_ai1PdwncGpio2Pad[i4Gpio];
    
#ifndef CC_UBOOT
    if(T8032_ALIVE() && (pfgSet != NULL))
    {        
        PDWNC_T8032_CMD_T rCmd;
        PDWNC_T8032_RESPONSE_T rResponse;
        rCmd.u1Cmd = PDWNC_CMD_ARMT8032_SET_GPIO_ENABLE;

        // Supply pad setting register offset directly to uP        
        if((u4IO32Read4B(PDWNC_GPIOEN0) & (1U << i4Idx)))
       {
            u4Val = 1;
        }
        else
        {
            u4Val = 0;
        }
         if(u4Val != *pfgSet)
        {
            rCmd.u1SubCmd = i4Idx;
            rCmd.au1Data[0] = (UINT8)*pfgSet;        
            VERIFY(PDWNC_T8032GpioCmd(&rCmd, &rResponse) == 0);
            return (INT32)(rResponse.au1Data[0]);         
         }    
         else
         {
            return u4Val;
         }
    }
#endif    

#ifndef CC_MTK_LOADER
        rCrit = x_crit_start();
#endif /* CC_MTK_LOADER */

#if defined(CC_MT5389)
	u4Val = u4IO32Read4B(PDWNC_GPIOEN0);
#else
    if (i4Gpio <= 31)
    {
        u4Val = u4IO32Read4B(PDWNC_GPIOEN0);
    }
    else
    {
        u4Val = u4IO32Read4B(PDWNC_GPIOEN1);
    }
#endif

    if (pfgSet == NULL)
    {
#ifndef CC_MTK_LOADER
        x_crit_end(rCrit);         
#endif /* CC_MTK_LOADER */

		i4Idx &= 0x1f;
        return ((u4Val & (1U << i4Idx)) ? 1 : 0);
    }
  	i4Idx &= 0x1f;
    u4Val = (*pfgSet) ?
                (u4Val | (1U << i4Idx)) :
                (u4Val & ~(1U << i4Idx));
				
#if defined(CC_MT5389)
	vIO32Write4B(PDWNC_GPIOEN0, u4Val);
#else
    if (i4Gpio <= 31)
    {
        vIO32Write4B(PDWNC_GPIOEN0, u4Val);
    }
    else
    {
        vIO32Write4B(PDWNC_GPIOEN1, u4Val);
    }
#endif

#ifndef CC_MTK_LOADER
        x_crit_end(rCrit);         
#endif /* CC_MTK_LOADER */ 
    return (*pfgSet);
}

//-----------------------------------------------------------------------------
/** PDWNC_GpioOutput() The GPIO output value setting functions. It will check
 *  the i4Gpio and set to related register bit as 0 or 1.
 *  @param i4Gpio the gpio number to set or read.
 *  @param pfgSet A integer pointer.  If it's NULL, this function will return
 *          the bit of gpio number (0 or 1).  If it's not NULL, it must 
 *          reference to a integer.  If the integer is 0, this function 
 *          will set the bit of the gpio number as 0, otherwise set as 1.
 *  @retval If pfgSet is NULL, it return 0 or 1 (the bit value of the gpio
 *          number of output mode.  Otherwise, return (*pfgSet).
 */
//-----------------------------------------------------------------------------
INT32 PDWNC_GpioOutput(INT32 i4Gpio, INT32 *pfgSet)
{
    UINT32 u4Val,u4Val1;
    INT32 i4Idx,i4Val;
#ifndef CC_MTK_LOADER
        CRIT_STATE_T rCrit;
#else /* CC_MTK_LOADER */
        //UINT32 u4State;
#endif 
    if ((i4Gpio < 0) || (i4Gpio >= TOTAL_PDWNC_GPIO_NUM))
    {
        return -1;
    }

    // Translate GPIO number to pad setting register offset
    i4Idx = (INT32)_ai1PdwncGpio2Pad[i4Gpio];	
	
#ifndef CC_UBOOT
    if(T8032_ALIVE() && (pfgSet != NULL))
    {        
         if((u4IO32Read4B(PDWNC_GPIOOUT0) & (1U << i4Idx)))
        {
            u4Val = 1;
        }
        else
        {
            u4Val = 0;
        }

        if((u4Val != *pfgSet) || ((u4IO32Read4B(PDWNC_GPIOEN0) & (1U << i4Idx)) == 0))
        {
            // Supply pad setting register offset directly to uP                
            PDWNC_T8032_CMD_T rCmd;
            PDWNC_T8032_RESPONSE_T rResponse;
            rCmd.u1Cmd = PDWNC_CMD_ARMT8032_SET_GPIO_OUTPUT;
            rCmd.u1SubCmd = i4Idx;
            rCmd.au1Data[0] = (UINT8)*pfgSet;        
            VERIFY(PDWNC_T8032GpioCmd(&rCmd, &rResponse) == 0);
            BSP_PinGpioSel(i4Gpio +  OPCTRL(0));                
            return (INT32)(rResponse.au1Data[0]);  
        }
        else
        {
            BSP_PinGpioSel(i4Gpio +  OPCTRL(0));            
            return u4Val;
        }

    }
#endif
#ifndef CC_MTK_LOADER
        rCrit = x_crit_start();
#endif /* CC_MTK_LOADER */

	BSP_PinGpioSel(i4Gpio +  OPCTRL(0));
	
	i4Idx &= 0x1f;
	
    if (pfgSet == NULL)	//NULL: for query gpio status, must be GPIO output , but not change pin level
    {
#ifndef CC_MTK_LOADER
        x_crit_end(rCrit);         
#endif /* CC_MTK_LOADER */
		if(PDWNC_GpioEnable(i4Gpio, NULL) == 0)//old is input state, change to be output
		{
			//get pin level		
		#if defined(CC_MT5389)
			u4Val1 = IO32ReadFldAlign(PDWNC_GPIOIN0, FLD_PD_GPIO_IN0);		
		#else
		    if (i4Gpio <= 31)
		    {
		        u4Val1 = IO32ReadFldAlign(PDWNC_GPIOIN0, FLD_PD_GPIO_IN0);		
		    }
		    else
		    {
		        u4Val1 = IO32ReadFldAlign(PDWNC_GPIOIN1, FLD_PD_GPIO_IN1);
		    }
		#endif
			//get current out register setting
		#if defined(CC_MT5389)
			u4Val = u4IO32Read4B(PDWNC_GPIOOUT0);
		#else
			if (i4Gpio <= 31)
			{
				u4Val = u4IO32Read4B(PDWNC_GPIOOUT0);
			}
			else
			{
				u4Val = u4IO32Read4B(PDWNC_GPIOOUT1);
			}
		#endif			
			u4Val = (u4Val1 & (1U << i4Idx)) ?
						(u4Val | (1U << i4Idx)) :
						(u4Val & ~(1U << i4Idx));
		#if defined(CC_MT5389)
			vIO32Write4B(PDWNC_GPIOOUT0, u4Val);
		#else
			if (i4Gpio <= 31)
			{
				vIO32Write4B(PDWNC_GPIOOUT0, u4Val);
			}
			else
			{
				vIO32Write4B(PDWNC_GPIOOUT1, u4Val);
			}
		#endif
		
			// Set the output mode.
			i4Val = 1;
			VERIFY(1 == PDWNC_GpioEnable(i4Gpio, &i4Val));

		}
		
	//get out value
	#if defined(CC_MT5389)
		u4Val = u4IO32Read4B(PDWNC_GPIOOUT0);
	#else
		if (i4Gpio <= 31)
		{
			u4Val = u4IO32Read4B(PDWNC_GPIOOUT0);
		}
		else
		{
			u4Val = u4IO32Read4B(PDWNC_GPIOOUT1);
		}
	#endif

        return ((u4Val & (1U << i4Idx)) ? 1 : 0);
    }

//get out value
#if defined(CC_MT5389)
	u4Val = u4IO32Read4B(PDWNC_GPIOOUT0);
#else
	if (i4Gpio <= 31)
	{
		u4Val = u4IO32Read4B(PDWNC_GPIOOUT0);
	}
	else
	{
		u4Val = u4IO32Read4B(PDWNC_GPIOOUT1);
	}
#endif

    u4Val = (*pfgSet) ?
                (u4Val | (1U << i4Idx)) :
                (u4Val & ~(1U << i4Idx));
				
#if defined(CC_MT5389)
	vIO32Write4B(PDWNC_GPIOOUT0, u4Val);
#else
    if (i4Gpio <= 31)
    {
        vIO32Write4B(PDWNC_GPIOOUT0, u4Val);
    }
    else
    {
        vIO32Write4B(PDWNC_GPIOOUT1, u4Val);
    }
#endif

#ifndef CC_MTK_LOADER
        x_crit_end(rCrit);         
#endif /* CC_MTK_LOADER */

    // Set the output mode.
    i4Val = 1;
    VERIFY(1 == PDWNC_GpioEnable(i4Gpio, &i4Val));

    // _PdwncSetGpioPin(i4Gpio);
 
    return (*pfgSet);
}

//-----------------------------------------------------------------------------
/** PDWNC_GpioInput()  The GPIO input reading functions. It will check the 
 *  i4Gpio and read related register bit to return.
 *  @param i4Gpio the gpio number to read.
 *  @retval 0 or 1.  (GPIO input value.)
 */
//-----------------------------------------------------------------------------
INT32 PDWNC_GpioInput(INT32 i4Gpio)
{
    UINT32 u4Val;
    INT32 i4Idx = 0;

    if ((i4Gpio < 0) || (i4Gpio >= TOTAL_PDWNC_GPIO_NUM))
    {
        return -1;
    }

    // Translate GPIO number to pad setting register offset
    i4Idx = (INT32)_ai1PdwncGpio2Pad[i4Gpio];
    
//    _PdwncSetGpioPin(i4Gpio);
    BSP_PinGpioSel(i4Gpio +  OPCTRL(0));

#if defined(CC_MT5389)
	u4Val = IO32ReadFldAlign(PDWNC_GPIOIN0, FLD_PD_GPIO_IN0);		
	return ((u4Val & (1U << i4Idx)) ? 1U : 0);
#else
    if (i4Gpio <= 31)
    {
        u4Val = IO32ReadFldAlign(PDWNC_GPIOIN0, FLD_PD_GPIO_IN0);		
		return ((u4Val & (1U << i4Idx)) ? 1U : 0);	
    }
    else
    {
        u4Val = IO32ReadFldAlign(PDWNC_GPIOIN1, FLD_PD_GPIO_IN1);
		i4Idx &= 0x1f;
		return ((u4Val & (1U << i4Idx)) ? 1U : 0);	
    }
#endif
}


#ifndef CC_MTK_LOADER
//-----------------------------------------------------------------------------
/** PDWNC_GpioIntrq() The GPIO interrupt enable setting functions. It will
 *  check the i4Gpio and set to related register bit as 0 or 1.  In this 
 *  function, 0 is interrupt disable mode and 1 is interrupt enable mode.
 *  @param i4Gpio the gpio number to set or read.
 *  @param pfgSet A integer pointer.  If it's NULL, this function will return
 *  the current setting of gpio number (0 or 1).  If it's not NULL, it must
 *  reference to a integer.  If the integer is 0, this function will set the
 *  mode of the gpio number as interrupt disable mode, otherwise set as
 *  interrupt enable mode.
 *  @retval If pfgSet is NULL, it return 0 or 1 (0 is interrupt disable mode,
 *          1 is interrupt enable mode.)  Otherwise, return (*pfgSet).
 */
//-----------------------------------------------------------------------------
INT32 PDWNC_GpioIntrq(INT32 i4Gpio, INT32 *pfgSet)
{
    UINT32 u4Val;
    INT32 i4Int;
    UNUSED(_ai1PdwncGpio2Int);
    if ((i4Gpio < 0) || (i4Gpio >= TOTAL_PDWNC_GPIO_NUM))
    {
        return -1;
    }

    if((i4Int = _ai1PdwncGpio2Int[i4Gpio]) < 0)
    {
        return -1;    
    }

    u4Val = u4IO32Read4B(PDWNC_ARM_INTEN);
    if (pfgSet == NULL)
    {
        return ((u4Val & (1U << i4Int)) ? 1 : 0);
    }
    u4Val = (*pfgSet) ?
            (u4Val | OPCTRL_INTEN(i4Int)) :
            (u4Val & ~ OPCTRL_INTEN(i4Int));
    vIO32Write4B(PDWNC_ARM_INTEN, u4Val);

    return (*pfgSet);
}

//-----------------------------------------------------------------------------
/* PDWNC_GpioQuery() to query PDWNC Gpio settings.
 *  @param i4Gpio should be between 0~7.
 *  @param pi4Intr to get interrupt enable/disable status.
 *  @param pi4Fall to get falling callback enable/disable status.
 *  @param pi4Rise to get rising callback enable/disable status.
 *  @retval return 0 if successful, otherwise return -1.
 */
//-----------------------------------------------------------------------------
INT32 PDWNC_GpioQuery(INT32 i4Gpio, INT32 *pi4Intr, INT32 *pi4Fall, INT32 *pi4Rise)
{
#if 0//lili mark
    INT32 i4Int;
	INT32 i4Offset = 0;
	    UINT32 u4RegIntCfg;
    if ((i4Gpio < 0) || (i4Gpio >= TOTAL_PDWNC_GPIO_NUM))
    {
        return -1;
    }
	i4Int = _ai1PdwncGpio2Int[i4Gpio];
    if (pi4Intr)
    {
        *pi4Intr = (u4IO32Read4B(PDWNC_ARM_INTEN) & (1U << i4Int)) ? 1 : 0;
    }

    if ((u4IO32Read4B(PDWNC_ARM_INTEN) & (1U << i4Gpio))==0)
    {
        if (pi4Fall)
        {
            *pi4Fall = 0;
        }
        if (pi4Rise)
        {
            *pi4Rise = 0;
        }
        return 0;
    }
    
    if(i4Gpio < PDWNC_GPIO_INT_GROUP_2_BEGIN)
    {
        u4RegIntCfg = PDWNC_EXINTCFG;
    }
    // Interrupt trigger type configuration of remaining GPIO
    else 
#if defined(CC_MT5365) || defined(CC_MT5395)         
    if(i4Gpio < PDWNC_SRVAD_GPIO_BEGIN)
#endif        
    {
        u4RegIntCfg = PDWNC_EXTIO2INTCFG;    
        i4Offset = _ai1PdwncGpio2Int[PDWNC_GPIO_INT_GROUP_2_BEGIN];    
        
    }
#if defined(CC_MT5365) || defined(CC_MT5395) 
    else     
    {
        u4RegIntCfg = PDWNC_ADINTCFG;    
        i4Offset = _ai1PdwncGpio2Int[PDWNC_SRVAD_GPIO_BEGIN];            
        i4Offset -= ADC2GPIO_CH_ID_MIN;
    }
#endif    
    i4Int -= i4Offset;
    
    if (pi4Fall)
    {
			*pi4Fall = (u4IO32Read4B(u4RegIntCfg ) & (1U << (i4Int+16))) ? 1 : 
                    ((u4IO32Read4B(u4RegIntCfg )  & (1U << i4Int)) ? 0 : 1);
    }
    if (pi4Rise)
    {
        *pi4Rise = (u4IO32Read4B(u4RegIntCfg )  & (1U << (i4Int+16))) ? 1 : 
                    ((u4IO32Read4B(u4RegIntCfg )  & (1U << i4Int)) ? 1 : 0);
    }
    #endif

    return 0;
}

//-----------------------------------------------------------------------------
/* PDWNC_GpioReg() to setup the PDWNC Gpio interrupt callback function, type,
 *      and state.
 *  @param i4Gpio should be between 0~7.
 *  @param eType should be one of enum GPIO_TYPE.
 *  @param pfnCallback the callback function.
 *  @retval 0 successful, -1 failed.
 */
//-----------------------------------------------------------------------------

INT32 PDWNC_T8032GpioReg(INT32 i4Gpio, GPIO_TYPE eType, PFN_GPIO_CALLBACK pfnCallback)
{

    UINT32 u4Val;
    //UINT32 u4RegIntCfg;
    INT32 i4Int;
//	UINT32 u4Gpio;
    //INT32 i4Offset = 0;

	if(i4Gpio >= 200)
		i4Gpio -= 200;
    if ((i4Gpio < 0) || (i4Gpio >= TOTAL_PDWNC_GPIO_NUM))
    {
        return -1;
    }

    if((i4Int = _ai1PdwncGpio2Int[i4Gpio]) < 0)    
    {
        return -1;        
    }
    #if 0
    if((eType == GPIO_TYPE_INTR_RISE) || (eType == GPIO_TYPE_INTR_FALL) || (eType == GPIO_TYPE_INTR_BOTH) || (eType == GPIO_TYPE_INTR_LEVEL_HIGH) || (eType == GPIO_TYPE_INTR_LEVEL_LOW))
    {    
		BSP_PinGpioSel(i4Gpio + OPCTRL(0));//pinmux 
		
        u4Val = u4IO32Read4B(PDWNC_T8032_INTEN);//PDWNC_T8032_INTEN
        u4Val |= (1U << i4Int);        // Enable interrupt.
        vIO32Write4B(PDWNC_T8032_INTEN, u4Val);
    }
    else
    {
        u4Val = u4IO32Read4B(PDWNC_T8032_INTEN);//PDWNC_T8032_INTEN
        u4Val &= ~(1U << i4Int);       // Disable interrupt.
        vIO32Write4B(PDWNC_T8032_INTEN, u4Val);
        return 0;
    }
	#endif
     /* Set the register and callback function. */
//    //i4Int = (i4Int>=24)? (i4Int-16):(i4Int);
//    u4Gpio = _ai1PdwncInt2Gpio[i4Int];
    switch(eType)

    {
        case GPIO_TYPE_INTR_FALL:
            u4Val = u4IO32Read4B(PDWNC_EXINT2ED);
            u4Val &= ~(1U << i4Int); // Disable double edge trigger.
            vIO32Write4B(PDWNC_EXINT2ED, u4Val);
            u4Val = u4IO32Read4B(PDWNC_EXINTLEV);
            u4Val &= ~(1U << i4Int);  // Set Edge trigger.
            vIO32Write4B(PDWNC_EXINTLEV, u4Val);
            u4Val = u4IO32Read4B(PDWNC_EXINTPOL);
            u4Val &= ~(1U << i4Int);       // Set Falling Edge interrupt.
            vIO32Write4B(PDWNC_EXINTPOL, u4Val);
            break;
            
        case GPIO_TYPE_INTR_RISE:            
            u4Val = u4IO32Read4B(PDWNC_EXINT2ED);
            u4Val &= ~(1U << i4Int); // Disable double edge trigger.
            vIO32Write4B(PDWNC_EXINT2ED, u4Val);
            u4Val = u4IO32Read4B(PDWNC_EXINTLEV);
            u4Val &= ~(1U << i4Int);  // Set Edge trigger.
            vIO32Write4B(PDWNC_EXINTLEV, u4Val);
            u4Val = u4IO32Read4B(PDWNC_EXINTPOL);
            u4Val |= (1U << i4Int);       // Set rasing Edge interrupt.
            vIO32Write4B(PDWNC_EXINTPOL, u4Val);
            break;               
            
        case GPIO_TYPE_INTR_BOTH:            
            u4Val = u4IO32Read4B(PDWNC_EXINT2ED);
            u4Val |= (1U << i4Int); // enable double edge trigger.
            vIO32Write4B(PDWNC_EXINT2ED, u4Val);
            u4Val = u4IO32Read4B(PDWNC_EXINTLEV);
            u4Val &= ~(1U << i4Int);  // Set Edge trigger.
            vIO32Write4B(PDWNC_EXINTLEV, u4Val);
            break;
            
        case GPIO_TYPE_INTR_LEVEL_LOW:            
            u4Val = u4IO32Read4B(PDWNC_EXINT2ED);
            u4Val &= ~(1U << i4Int); // Disable double edge trigger.
            vIO32Write4B(PDWNC_EXINT2ED, u4Val);
            u4Val = u4IO32Read4B(PDWNC_EXINTLEV);
            u4Val |= (1U << i4Int);  // Set Level trigger.
            vIO32Write4B(PDWNC_EXINTLEV, u4Val);
            u4Val = u4IO32Read4B(PDWNC_EXINTPOL);
            u4Val &= ~(1U << i4Int);       // Set level low interrupt.
            vIO32Write4B(PDWNC_EXINTPOL, u4Val);
            break;
            
        case GPIO_TYPE_INTR_LEVEL_HIGH:            
            u4Val = u4IO32Read4B(PDWNC_EXINT2ED);
            u4Val &= ~(1U << i4Int); // Disable double edge trigger.
            vIO32Write4B(PDWNC_EXINT2ED, u4Val);
            u4Val = u4IO32Read4B(PDWNC_EXINTLEV);
            u4Val |= (1U << i4Int);  // Set level trigger.
            vIO32Write4B(PDWNC_EXINTLEV, u4Val);
            u4Val = u4IO32Read4B(PDWNC_EXINTPOL);
            u4Val |= (1U << i4Int);       // Set level high interrupt.
            vIO32Write4B(PDWNC_EXINTPOL, u4Val);
            break;
            
        default:
			
			u4Val = u4IO32Read4B(PDWNC_T8032_INTEN);//PDWNC_T8032_INTEN
			u4Val &= ~(1U << i4Int);	   // Disable interrupt.
			vIO32Write4B(PDWNC_T8032_INTEN, u4Val);
            return 0;
        }
	
	BSP_PinGpioSel(i4Gpio + OPCTRL(0));//pinmux 
	
	u4Val = u4IO32Read4B(PDWNC_T8032_INTEN);//PDWNC_T8032_INTEN
	u4Val |= (1U << i4Int); 	   // Enable interrupt.
	vIO32Write4B(PDWNC_T8032_INTEN, u4Val);

    if (pfnCallback)
    {
        _afnPDWNCGpioCallback[i4Gpio] = pfnCallback;
    }

//    printf("Interrupt seting is:0x%8x,0x%8x,0x%8x\n",u4IO32Read4B(PDWNC_EXINT2ED),u4IO32Read4B(PDWNC_EXINTLEV),u4IO32Read4B(PDWNC_EXINTPOL));
    return 0;
}


INT32 PDWNC_GpioReg(INT32 i4Gpio, GPIO_TYPE eType, PFN_GPIO_CALLBACK pfnCallback)
{
    UINT32 u4Val;
    INT32 i4Int;
    if ((i4Gpio < 0) || (i4Gpio >= TOTAL_PDWNC_GPIO_NUM))
    {
        return -1;
    }
	
#ifndef CC_FAST_INIT
    if((eType != GPIO_TYPE_NONE) && (pfnCallback == NULL))
    {
        return -1;        
    }
#endif

    if((i4Int = _ai1PdwncGpio2Int[i4Gpio]) < 0)    
    {
        return -1;        
    }
    #if 0
    if((eType == GPIO_TYPE_INTR_RISE) || (eType == GPIO_TYPE_INTR_FALL) || (eType == GPIO_TYPE_INTR_BOTH) || (eType == GPIO_TYPE_INTR_LEVEL_HIGH) || (eType == GPIO_TYPE_INTR_LEVEL_LOW))
    {
		BSP_PinGpioSel(i4Gpio + OPCTRL(0));//pinmux 
		
        u4Val = u4IO32Read4B(PDWNC_ARM_INTEN);
        u4Val |= (1U << i4Int);        // Enable interrupt.
        vIO32Write4B(PDWNC_ARM_INTEN, u4Val);
    }
    else
    {
        u4Val = u4IO32Read4B(PDWNC_ARM_INTEN);
        u4Val &= ~(1U << i4Int);       // Disable interrupt.
        vIO32Write4B(PDWNC_ARM_INTEN, u4Val);
        return 0;
    }
	#endif
     /* Set the register and callback function. */
    //i4Int = (i4Int>=24)? (i4Int-16):(i4Int);
    //u4Gpio = _ai1PdwncInt2Gpio[i4Int];
    switch(eType)
#if 1
    {
        case GPIO_TYPE_INTR_FALL:
            u4Val = u4IO32Read4B(PDWNC_EXINT2ED);
            u4Val &= ~(1U << i4Int); // Disable double edge trigger.
            vIO32Write4B(PDWNC_EXINT2ED, u4Val);
            u4Val = u4IO32Read4B(PDWNC_EXINTLEV);
            u4Val &= ~(1U << i4Int);  // Set Edge trigger.
            vIO32Write4B(PDWNC_EXINTLEV, u4Val);
            u4Val = u4IO32Read4B(PDWNC_EXINTPOL);
            u4Val &= ~(1U << i4Int);       // Set Falling Edge interrupt.
            vIO32Write4B(PDWNC_EXINTPOL, u4Val);
            break;
            
        case GPIO_TYPE_INTR_RISE:            
            u4Val = u4IO32Read4B(PDWNC_EXINT2ED);
            u4Val &= ~(1U << i4Int); // Disable double edge trigger.
            vIO32Write4B(PDWNC_EXINT2ED, u4Val);
            u4Val = u4IO32Read4B(PDWNC_EXINTLEV);
            u4Val &= ~(1U << i4Int);  // Set Edge trigger.
            vIO32Write4B(PDWNC_EXINTLEV, u4Val);
            u4Val = u4IO32Read4B(PDWNC_EXINTPOL);
            u4Val |= (1U << i4Int);       // Set rasing Edge interrupt.
            vIO32Write4B(PDWNC_EXINTPOL, u4Val);
            break;               
            
        case GPIO_TYPE_INTR_BOTH:            
            u4Val = u4IO32Read4B(PDWNC_EXINT2ED);
            u4Val |= (1U << i4Int); // enable double edge trigger.
            vIO32Write4B(PDWNC_EXINT2ED, u4Val);
            u4Val = u4IO32Read4B(PDWNC_EXINTLEV);
            u4Val &= ~(1U << i4Int);  // Set Edge trigger.
            vIO32Write4B(PDWNC_EXINTLEV, u4Val);
            break;
            
        case GPIO_TYPE_INTR_LEVEL_LOW:            
            u4Val = u4IO32Read4B(PDWNC_EXINT2ED);
            u4Val &= ~(1U << i4Int); // Disable double edge trigger.
            vIO32Write4B(PDWNC_EXINT2ED, u4Val);
            u4Val = u4IO32Read4B(PDWNC_EXINTLEV);
            u4Val |= (1U << i4Int);  // Set Level trigger.
            vIO32Write4B(PDWNC_EXINTLEV, u4Val);
            u4Val = u4IO32Read4B(PDWNC_EXINTPOL);
            u4Val &= ~(1U << i4Int);       // Set level low interrupt.
            vIO32Write4B(PDWNC_EXINTPOL, u4Val);
            break;
            
        case GPIO_TYPE_INTR_LEVEL_HIGH:            
            u4Val = u4IO32Read4B(PDWNC_EXINT2ED);
            u4Val &= ~(1U << i4Int); // Disable double edge trigger.
            vIO32Write4B(PDWNC_EXINT2ED, u4Val);
            u4Val = u4IO32Read4B(PDWNC_EXINTLEV);
            u4Val |= (1U << i4Int);  // Set level trigger.
            vIO32Write4B(PDWNC_EXINTLEV, u4Val);
            u4Val = u4IO32Read4B(PDWNC_EXINTPOL);
            u4Val |= (1U << i4Int);       // Set level high interrupt.
            vIO32Write4B(PDWNC_EXINTPOL, u4Val);
            break;
            
        default:
			u4Val = u4IO32Read4B(PDWNC_ARM_INTEN);
			u4Val &= ~(1U << i4Int);	   // Disable interrupt.
			vIO32Write4B(PDWNC_ARM_INTEN, u4Val);
			return 0;
        }

		BSP_PinGpioSel(i4Gpio + OPCTRL(0));//pinmux 

		u4Val = u4IO32Read4B(PDWNC_ARM_INTEN);
		u4Val |= (1U << i4Int); 	   // Enable interrupt.
		vIO32Write4B(PDWNC_ARM_INTEN, u4Val);

#else
    // Interrupt trigger type configuration of first GPIO interrupt group
    if(i4Gpio < PDWNC_GPIO_INT_GROUP_2_BEGIN)
    {
        u4RegIntCfg = PDWNC_EXINTCFG;
    }
    // Interrupt trigger type configuration of remaining GPIO
    else
#if defined(CC_MT5365) || defined(CC_MT5395)         
    if(i4Gpio < PDWNC_SRVAD_GPIO_BEGIN)
#endif                
    {
        u4RegIntCfg = PDWNC_EXTIO2INTCFG;    
        i4Offset = _ai1PdwncGpio2Int[PDWNC_GPIO_INT_GROUP_2_BEGIN];    
    }
#if defined(CC_MT5365) || defined(CC_MT5395) 
    else
    {
        u4RegIntCfg = PDWNC_ADINTCFG;    
        i4Offset = _ai1PdwncGpio2Int[PDWNC_SRVAD_GPIO_BEGIN];
        i4Offset -= ADC2GPIO_CH_ID_MIN;
    }
#endif    
#if 0
    if(eType == GPIO_TYPE_NONE)
    {
        u4Val = u4IO32Read4B(PDWNC_ARM_INTEN);
        u4Val &= ~(1U << i4Int);       // Disable interrupt.
        vIO32Write4B(PDWNC_ARM_INTEN, u4Val);
        return 0;
    }
    else  if(eType <= GPIO_TYPE_INTR_LEVEL_HIGH)
    {
        u4Val = u4IO32Read4B(PDWNC_ARM_INTEN);
        u4Val |= (1U << i4Int);        // Enable interrupt.
        vIO32Write4B(PDWNC_ARM_INTEN, u4Val);
    }
#else
    if((eType == GPIO_TYPE_INTR_RISE) || (eType == GPIO_TYPE_INTR_FALL) || (eType == GPIO_TYPE_INTR_BOTH) || (eType == GPIO_TYPE_INTR_LEVEL_HIGH) || (eType == GPIO_TYPE_INTR_LEVEL_LOW))
    {
#if defined(CC_MT5365) || defined(CC_MT5395)         
		BSP_PinGpioSel(i4Gpio + OPCTRL(0));
#endif    

        u4Val = u4IO32Read4B(PDWNC_ARM_INTEN);
        u4Val |= (1U << i4Int);        // Enable interrupt.
        vIO32Write4B(PDWNC_ARM_INTEN, u4Val);
    }
    else
    {
        u4Val = u4IO32Read4B(PDWNC_ARM_INTEN);
        u4Val &= ~(1U << i4Int);       // Disable interrupt.
        vIO32Write4B(PDWNC_ARM_INTEN, u4Val);
        return 0;
    }

#endif
    i4Int -= i4Offset;
    switch(eType)
    {
    case GPIO_TYPE_INTR_FALL:
        u4Val = u4IO32Read4B(u4RegIntCfg);
        u4Val &= ~(1U << (i4Int+16));  // Disable double edge trigger.
        u4Val &= ~(1U << (i4Int+8));   // Set Edge trigger.
        u4Val &= ~(1U << i4Int);       // Set Falling Edge interrupt.
        vIO32Write4B(u4RegIntCfg, u4Val);
        break;
    case GPIO_TYPE_INTR_RISE:
/*
        u4Val = u4IO32Read4B(PDWNC_ARM_INTEN);
        u4Val |= (1U << i4Int);        // Enable interrupt.
        vIO32Write4B(PDWNC_ARM_INTEN, u4Val);
*/
        u4Val = u4IO32Read4B(u4RegIntCfg);
        u4Val &= ~(1U << (i4Int+16));  // Disable double edge trigger.
        u4Val &= ~(1U << (i4Int+8));   // Set Edge trigger.
        u4Val |= (1U << i4Int);        // Set Rising Edge interrupt.
        vIO32Write4B(u4RegIntCfg, u4Val);
        break;
    case GPIO_TYPE_INTR_BOTH:
/*        
        u4Val = u4IO32Read4B(PDWNC_ARM_INTEN);
        u4Val |= (1U << i4Int);        // Enable interrupt.
        vIO32Write4B(PDWNC_ARM_INTEN, u4Val);
*/        
        u4Val = u4IO32Read4B(u4RegIntCfg);
        u4Val |= (1U << (i4Int+16));   // Enable double edge trigger.
        u4Val &= ~(1U << (i4Int+8));   // Set Edge trigger.        
        vIO32Write4B(u4RegIntCfg, u4Val);
        break;
    case GPIO_TYPE_INTR_LEVEL_LOW:
		/*        
        u4Val = u4IO32Read4B(PDWNC_ARM_INTEN);
        u4Val |= (1U << i4Int);        // Enable interrupt.
        vIO32Write4B(PDWNC_ARM_INTEN, u4Val);
*/        
        u4Val = u4IO32Read4B(u4RegIntCfg);
        u4Val &= ~(1U << (i4Int+16));  // Disable double edge trigger.
        u4Val |= (1U << (i4Int+8));    // Set Level trigger.
        u4Val &= ~(1U << i4Int);       // Always level low active.
        vIO32Write4B(u4RegIntCfg, u4Val);
        break;
    case GPIO_TYPE_INTR_LEVEL_HIGH:
/*        
        u4Val = u4IO32Read4B(PDWNC_ARM_INTEN);
        u4Val |= (1U << i4Int);        // Enable interrupt.
        vIO32Write4B(PDWNC_ARM_INTEN, u4Val);
*/        
        u4Val = u4IO32Read4B(u4RegIntCfg);
        u4Val &= ~(1U << (i4Int+16));  // Disable double edge trigger.
        u4Val |= (1U << (i4Int+8));    // Set Level trigger.
        u4Val |= (1U << i4Int);       // Always level high active.
        vIO32Write4B(u4RegIntCfg, u4Val);
        break;
    default:
        return -1;
    }   
    #endif
    if (pfnCallback)
    {
        _afnPDWNCGpioCallback[i4Gpio] = pfnCallback;
        
        //_afnPDWNCGpioCallback[_ai1PdwncInt2Gpio[i4Int + i4Offset]] = pfnCallback;
    }
//    printf("Interrupt seting is:0x%8x,0x%8x,0x%8x\n",u4IO32Read4B(PDWNC_EXINT2ED),u4IO32Read4B(PDWNC_EXINTLEV),u4IO32Read4B(PDWNC_EXINTPOL));
    
    return 0;
}

INT32 PDWNC_GpioResumeINTEN(INT32 i4Gpio, GPIO_TYPE eType)
{
//note: At resume stage, pass NULL will not overwrite _afnPDWNCGpioCallback[] which setup at initial stage    
    return PDWNC_GpioReg(i4Gpio, eType, NULL);
}
#else

INT32 PDWNC_GpioIntrq(INT32 i4Gpio, INT32 *pfgSet)
{
	
	return -1;
}
INT32 PDWNC_GpioQuery(INT32 i4Gpio, INT32 *pi4Intr, INT32 *pi4Fall, INT32 *pi4Rise)
{
	return -1;
}
INT32 PDWNC_GpioReg(INT32 i4Gpio, GPIO_TYPE eType, PFN_GPIO_CALLBACK pfnCallback)
{
	return -1;
}
INT32 PDWNC_T8032GpioReg(INT32 i4Gpio, GPIO_TYPE eType, PFN_GPIO_CALLBACK pfnCallback)
{
	return -1;
}
INT32 PDWNC_GpioResumeINTEN(INT32 i4Gpio, GPIO_TYPE eType)
{
	return -1;
}

#endif /* CC_MTK_LOADER */

//-----------------------------------------------------------------------------
/** _PdwncSetupGPIO() to setup PDWNC module GPIO wakeup settings
 *  @param pu4PowerDownEn is the power down wakeup enable register value.
 */
//-----------------------------------------------------------------------------
#ifdef CC_UP8032_ATV
void __PDWNC_SetupGPIO(UINT32 *pu4PowerDownEn)
#else
void _PDWNC_SetupGPIO(UINT32 *pu4PowerDownEn)
#endif
{
    INT32 i, i4Val;
    UINT32 u4Val, u4Gpio, u4DkBit;
    SGL_GPIO_T   *prSingleGpioKeyList = NULL;
    GREED_KEY_T  *prGreedGpioKeyList = NULL;

    ASSERT(pu4PowerDownEn != NULL);

    if (!PDWNC_SetupFpWakeup(NULL))
    {
        LOG(1,"FP wakeup disable\n");
        return;
    }

    if (DRVCUST_InitGet(eAudioAmpEnableGpioNum) != 0xffffffff)
    {
        // turn off audio amp
        GPIO_SetOut(DRVCUST_InitGet(eAudioAmpEnableGpioNum), !DRVCUST_InitGet(eAudioAmpGpioPolarity));
    }
    if (DRVCUST_InitGet(eAudioMuteGpioNum) != 0xffffffff)
    {
        // set audio mute
        GPIO_SetOut(DRVCUST_InitGet(eAudioMuteGpioNum), DRVCUST_InitGet(eAudioMuteGpioPolarity));
    }

    u4DkBit = DRVCUST_InitGet(eKeyPadWakeupBit);
#ifdef CC_UP8032_ATV
    i = DRVCUST_GpioQuery(eSingleGpioKeyList, (UINT32 *)(void *)&prSingleGpioKeyList);
#else
    i = DRVCUST_InitQuery(eSingleGpioKeyList, (UINT32 *)(void *)&prSingleGpioKeyList);
#endif
    if (i < 0)
    {
        return;
    }
    if (!((*pu4PowerDownEn) & WAK_IRRX))
    {
        u4DkBit = DKBIT_POWER;
    }

    // Do the GPIO wakeup initialization
    i = 0;
    while ((prSingleGpioKeyList) &&
            (prSingleGpioKeyList[i].u4Key != 0) &&
            (prSingleGpioKeyList[i].u4Key != DKBIT_NONE))
    {
        if (prSingleGpioKeyList[i].u4Key & u4DkBit)
        {
            u4Gpio = prSingleGpioKeyList[i].u4Gpio - OPCTRL(0);
            if (u4Gpio > TOTAL_PDWNC_DEDICATE_GPI_NUM || _ai1PdwncGpio2Int[u4Gpio] < 0)
            {
                LOG(1, "Error Power button setting\n");
                i++;
                continue;
            }
            u4Val = u4IO32Read4B(PDWNC_PDIO);
            if (prSingleGpioKeyList[i].u4Val)
            {
                u4Val |= (GPIOX_POL(u4Gpio));
            }
            else
            {
                u4Val &= ~(GPIOX_POL(u4Gpio));
            }
            vIO32Write4B(PDWNC_PDIO, u4Val);
            *pu4PowerDownEn |= GPIO_WAKEN(WAK_GPIOX(u4Gpio));
        }
        i++;
    }

#ifdef CC_UP8032_ATV
    i = DRVCUST_GpioQuery(eGreedGpioKeyList, (UINT32 *)(void *)&prGreedGpioKeyList);
#else
    i = DRVCUST_InitQuery(eGreedGpioKeyList, (UINT32 *)(void *)&prGreedGpioKeyList);
#endif
    if (i < 0)
    {
        return;
    }

    // Do the GPIO wakeup initialization
    i = 0;
    while ((prGreedGpioKeyList) &&
            (prGreedGpioKeyList[i].u4Key != 0) &&
            (prGreedGpioKeyList[i].u4Key != DKBIT_NONE))
    {
        if (prGreedGpioKeyList[i].u4Key & u4DkBit)
        {
            u4Gpio = prGreedGpioKeyList[i].u4GpioOut;
            i4Val = (INT32)prGreedGpioKeyList[i].u4Val;
            UNUSED(GPIO_Output((INT32)u4Gpio, &i4Val));
            u4Gpio = prGreedGpioKeyList[i].u4GpioIn - OPCTRL(0);
            if (u4Gpio > TOTAL_PDWNC_DEDICATE_GPI_NUM) //|| _ai1PdwncGpio2Int[u4Gpio] < 0)
            {
                LOG(1, "Error Power button setting\n");
                i++;
                continue;
            }
            u4Val = u4IO32Read4B(PDWNC_PDIO);
            if (prGreedGpioKeyList[i].u4Val)
            {
                u4Val |= (GPIOX_POL(u4Gpio));
            }
            else
            {
                u4Val &= ~(GPIOX_POL(u4Gpio));
            }
            vIO32Write4B(PDWNC_PDIO, u4Val);
            *pu4PowerDownEn |= GPIO_WAKEN(WAK_GPIOX(u4Gpio));
        }
        i++;
    }

    // Do the special GPIO wakeup initialization
#ifdef CC_UP8032_ATV
    i = DRVCUST_GpioQuery(eSpecialWakeupGpio, (UINT32 *)&u4Gpio);
#else
    i = DRVCUST_InitQuery(eSpecialWakeupGpio, (UINT32 *)&u4Gpio);
#endif
    if (i < 0)
    {
        return;
    }
#ifdef CC_UP8032_ATV
    i = DRVCUST_GpioQuery(eSpecialWakeupOnVal, (UINT32 *)&u4Val);
#else
    i = DRVCUST_InitQuery(eSpecialWakeupOnVal, (UINT32 *)&u4Val);
#endif
    if (i < 0)
    {
        return;
    }
    // Set to input mode.
    i4Val = 0;
    UNUSED(GPIO_Enable((INT32)u4Gpio, &i4Val));
    // Set the wakeup polarity.
    i4Val = (INT32)u4Val;
    u4Gpio = u4Gpio - OPCTRL(0);

    if(u4Gpio < TOTAL_PDWNC_DEDICATE_GPI_NUM)
    {                
        u4Val =  u4IO32Read4B(PDWNC_PDIO);
        if (i4Val)
        {
            u4Val |= (GPIOX_POL(u4Gpio));
        }
        else
        {
            u4Val &= ~(GPIOX_POL(u4Gpio));
        }
        vIO32Write4B(PDWNC_PDIO, u4Val);
    }
    
    *pu4PowerDownEn |= GPIO_WAKEN(WAK_GPIOX(u4Gpio));
}


//-----------------------------------------------------------------------------
/** _PDWNC_RegLedBlinker() Register/Unregister an GPIO as LED blinker
 *  @param i4Gpio the gpio idx to be used as LED blinker.
 *  @param fgEnable enable/disable the gpio as LED blinker. 
 *  @param u1RoundPeriod blinking dark/light cycle time (unit: 0.5 sec).  
 *  @param u1LightPeriod blinking dark time in a cycle time (unit: 0.5 sec).   
 *  @retval blinker gpio idx if success; -1 if fail
 */
//-----------------------------------------------------------------------------
INT32 _PDWNC_RegLedBlinker(INT32 i4Gpio, BOOL fgEnable, UINT8 u1RoundPeriod, UINT8 u1LightPeriod)
{
	  UINT32 u4Val;
    INT32 i4Val = 1;
    if(i4Gpio > OPCTRL(5) - OPCTRL(0) )
    {
        return -1;
    }

    if(fgEnable)
    {
        UNUSED(PDWNC_GpioEnable(i4Gpio, &i4Val));
        u4Val =  u4IO32Read4B(PDWNC_PDMISC) | (1U << i4Gpio);
        if(u1RoundPeriod == 4 && u1LightPeriod == 2)
        {
            u4Val |= (0x1 << 8);
        }
        else
        {
            u4Val &= ~(0x1 << 8);            
        }
    }
    else
    {
        u4Val = u4IO32Read4B(PDWNC_PDMISC) & ~(1U << i4Gpio);
    }
      
    vIO32Write4B(PDWNC_PDMISC, u4Val);

		return i4Gpio;
}


void PDWNC_SetupGPIO(UINT32 *pu4PowerDownEn)
{
#ifdef CC_UP8032_ATV
    __PDWNC_SetupGPIO(pu4PowerDownEn);
#else
    _PDWNC_SetupGPIO(pu4PowerDownEn);
#endif
}

