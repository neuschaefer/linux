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
 * $RCSfile: pdwnc_srvad.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pdwnc_srvad.c
 *  pdwnc_srvad.c The PDWNC (Power DoWN Controller) interface
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#ifdef CC_UP8032_ATV
#include "general_mt82.h"
#include "x_pinmux_mt82.h"
#include "x_pdwnc_mt82.h"
#include "hw_pdwnc.h"
#include "x_bim_mt82.h"
#include "x_assert.h"
#include "x_ckgen_mt82.h"
#include "x_timer.h"
#include "x_gpio_mt82.h"
#include "drvcust_if.h"
#include "pdwnc_config.h"
#include "pdwnc_debug.h"
#include "pdwnc_entry.h"
#include "pdwnc_gpio.h"
#include "pdwnc_drvif.h"
#include "sif_if.h"
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
#include "pdwnc_entry.h"
#include "pdwnc_gpio.h"
#include "pdwnc_t8032.h"
#include "pdwnc_drvif.h"
#include "sif_if.h"
#include "eeprom_if.h"
#endif
#include "ir_if.h"
#include "rtc_if.h"

//#undef PDWNC_WRITE32
//#undef PDWNC_READ32

//-----------------------------------------------------------------------------
// Configuration
//-----------------------------------------------------------------------------
#ifdef CC_UP8032_ATV
#define HAL_Delay_us(val)	vUtDelay2us(val/2)
#endif


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define CSYNC_CONT_THU_DEFAULT   (0x01a4 / 9)
#define CSYNC_CONT_THL_DEFAULT   (0x00ff / 9)
#define CSYNC_CONT_THH_DEFAULT	(0x19c /9)


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static BOOL _fgServoADWakeupSet = FALSE;
#ifndef CC_MTK_LOADER
static const INT8 _ai1PdwncServoGpio2Int[TOTAL_SERVOADC_NUM] =
{
    -1, -1, 20, 21, 22, 23
};
#endif

static BOOL _fgServoADCInit = 0;
#ifndef CC_MTK_LOADER
#if defined(CC_MT5365) || defined(CC_MT5395) 
#else
// PDWNC module sub-interrupt routine list.
static PFN_GPIO_CALLBACK _afnPDWNCServoGpioCallback[TOTAL_SERVOADC_NUMBER];
//static PFN_GPIO_CALLBACK _afnPDWNCServoGpioCallback[TOTAL_SERVOADC_NUM];
static const INT8 _ai1PdwncInt2ServoGpio[MAX_PDWNC_INT_ID] =
{//check 2804c[7:0] and [24:31], translate them to bit offset within 28080(offset of sysytem gpio number), only 15 gpio can issue interrupt
    -1, -1, -1, -1, -1, -1, -1, -1,
   -1, -1, -1, -1, -1, -1, -1, -1,
   -1, -1, -1, -1, 2, 3, 4, 5,
   -1, -1, -1, -1, -1, -1, -1, -1
};
#endif
#endif 
//-----------------------------------------------------------------------------
// Prototypes 
//-----------------------------------------------------------------------------
#ifdef  CC_UP8032_ATV
void __PDWNC_SetupServoADC(UINT32 *pu4PowerDownEn);
#else
void _PDWNC_SetupServoADC(UINT32 *pu4PowerDownEn);
#endif
//INT32 PDWNC_ServoGpioEnable(INT32 i4Gpio, INT32 *pfgSet);
//INT32 PDWNC_ServoGpioOutput(INT32 i4Gpio, INT32 *pfgSet);
//INT32 PDWNC_ServoGpioInput(INT32 i4Gpio);
//INT32 PDWNC_InitServoGpio(void);
#ifndef CC_MTK_LOADER
INT32 PDWNC_ServoGpioReg(INT32 i4Gpio, GPIO_TYPE eType, PFN_GPIO_CALLBACK pfnCallback);
INT32 PDWNC_ServoGpioIntrq(INT32 i4Gpio, INT32 *pfgSet);
INT32 PDWNC_ServoGpioQuery(INT32 i4Gpio, INT32 *pi4Intr, INT32 *pi4Fall, INT32 *pi4Rise);
#endif
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
#ifndef CC_MTK_LOADER
#ifndef CC_ENABLE_HW_SERVO_ADC  
static HANDLE_T _hServoADMutex = NULL;
#endif
#endif /* CC_MTK_LOADER */
#ifndef CC_MTK_LOADER
//-----------------------------------------------------------------------------
/** _PdwncGpioIsr() is the interrupt routine for GPIO pins of PDWNC module.
 *  @param u2Gpio is the gpio number in PDWNC module to trigger interrupt.
 */
//-----------------------------------------------------------------------------
#if defined(CC_MT5365) || defined(CC_MT5395) 
#else
static void _PdwncServoGpioIsr(UINT16 u2ServoGpio)
{
    BOOL fgStatus;

    if ((u2ServoGpio >= PDWNC_INTNO_SRVAD2 && u2ServoGpio <= PDWNC_INTNO_SRVAD5) &&
        (_afnPDWNCServoGpioCallback[_ai1PdwncInt2ServoGpio[u2ServoGpio]]!=NULL))
    {
        fgStatus = PDWNC_ServoGpioInput((INT32)_ai1PdwncInt2ServoGpio[u2ServoGpio]);
        _afnPDWNCServoGpioCallback[_ai1PdwncInt2ServoGpio[u2ServoGpio]]((INT32)(_ai1PdwncInt2ServoGpio[u2ServoGpio] + SERVO(0)), fgStatus);
    }

    // Still need to clear this non-hook interrupt.
    vIO32Write4B(PDWNC_INTCLR,  _PINT(u2ServoGpio));
}
#endif    
#endif
//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
void _PDWNC_ReadServoADCWakeupStatus(UINT32* pu4Val)
{
     VERIFY(pu4Val != NULL);
    *pu4Val = (_PDWNC_ReadWakeupStatus() & (0x3F << 20));

}

#ifdef  CC_UP8032_ATV
void __PDWNC_SetupServoADC(UINT32 *pu4PowerDownEn) 
#else
void _PDWNC_SetupServoADC(UINT32 *pu4PowerDownEn)
#endif
{
    INT32 i, i4LastCh = -1;
    UINT32 u4Val, u4DkBit;
    UINT32 u4Max, u4Min;
    SGL_SERVO_T   *prSingleServoKeyList;
    
    ASSERT(pu4PowerDownEn != NULL);

    if (!PDWNC_SetupFpWakeup(NULL))
    {
        Printf("FP wakeup disable\n");
        return;
    }

#ifdef CC_NO_ACD_STANDBY_SETTING
    // if ServoADC is enabled, skip this setting, because this is set.
    {
#ifndef CC_MTK_LOADER
        CRIT_STATE_T rCrit;
#else /* CC_MTK_LOADER */
        //UINT32 u4State;
#endif /* CC_MTK_LOADER */
       
        // To indicates servo AD wakeup has been set
#ifndef CC_MTK_LOADER
        rCrit = x_crit_start();
#endif /* CC_MTK_LOADER */
        _fgServoADWakeupSet = TRUE;
#ifndef CC_MTK_LOADER
        x_crit_end(rCrit);         
#endif /* CC_MTK_LOADER */
        
        Printf("Power down Enable:0x%08x\n", *pu4PowerDownEn);
        return;
    }
#endif

    u4DkBit = DRVCUST_InitGet(eKeyPadWakeupBit);
#ifdef  CC_UP8032_ATV
    i = DRVCUST_GpioQuery(eSingleServoKeyList, (UINT32 *)(void *)&prSingleServoKeyList);
#else
    i = DRVCUST_InitQuery(eSingleServoKeyList, (UINT32 *)(void *)&prSingleServoKeyList);
#endif
    if (i < 0)
    {
        goto ServoExit;
    }

    ASSERT(pu4PowerDownEn != NULL);
    if (!((*pu4PowerDownEn) & WAK_IRRX))
    {
        u4DkBit = DKBIT_POWER;
    }

    // disable ServoADC working.
    vIO32WriteFldAlign(PDWNC_SRVCFG1, 0xC0, FLD_SRVCH_EN);
    // Clear ServoADC value.
	vIO32WriteFldAlign(PDWNC_SRVCLR, 0x1, FLD_ADCLR);
    // Clear ServoADC register
    for (i=0; i<MAX_SERVOADC_NUM; i++)
    {
			vIO32WriteFldAlign(PDWNC_WAKTOP0 + (12 * i), 0x0, FLD_CH0TOP);
			vIO32WriteFldAlign(PDWNC_WAKBTM0 + (12 * i), 0x0, FLD_CH0BTM);
			vIO32WriteFldAlign(PDWNC_CHCFG0 + (12 * i), 0x0, FLD_CH0WKC);
    }

    // Do the Servo wakeup initialization
    i = 0;
    u4Max = 0;    
    u4Min = 63;    
    while ((prSingleServoKeyList) &&
            (prSingleServoKeyList[i].u4Key != 0) &&
            (prSingleServoKeyList[i].u4Key != DKBIT_NONE))
    {
        if (prSingleServoKeyList[i].u4Key & u4DkBit)
        {
            if ((prSingleServoKeyList[i].u4Top > 63) ||
                (prSingleServoKeyList[i].u4Servo >= (ADIN0 + TOTAL_SERVOADC_NUMBER)) ||
                (prSingleServoKeyList[i].u4Btm > 63) ||
                (prSingleServoKeyList[i].u4Btm >= prSingleServoKeyList[i].u4Top))
            {
                LOG(1, "Error Power button setting\n");
                i++;
                continue;
            }
            // In case of old style servo ADC numbering
            if(prSingleServoKeyList[i].u4Servo >= ADIN0)
            {
               prSingleServoKeyList[i].u4Servo -= ADIN0; 
            }

            // Make sure enable PAD
            u4Val = PDWNC_ReadServoADCChannelValue(prSingleServoKeyList[i].u4Servo);

		//Get the range
            if(prSingleServoKeyList[i].u4Servo != i4LastCh)
            {//reset init value while setting another channel...
                u4Max = 0;    
                u4Min = 63;    
            }
            else        
            {
                u4Max = IO32ReadFldAlign(PDWNC_WAKTOP0 + (12 * prSingleServoKeyList[i].u4Servo), FLD_CH0TOP);
                u4Min = IO32ReadFldAlign(PDWNC_WAKBTM0 + (12 * prSingleServoKeyList[i].u4Servo), FLD_CH0BTM);
            }
            i4LastCh = prSingleServoKeyList[i].u4Servo;                    
            // Set Servo ADC value            
            u4Val = CHTOP_VAL(((u4Max < prSingleServoKeyList[i].u4Top) ? (prSingleServoKeyList[i].u4Top) : u4Max));
            vIO32WriteFldAlign(PDWNC_WAKTOP0 + (12 * prSingleServoKeyList[i].u4Servo), u4Val, FLD_CH0TOP);
              
            u4Val = CHBTM_VAL(((u4Min > prSingleServoKeyList[i].u4Btm) ? (prSingleServoKeyList[i].u4Btm) : u4Min));  
            vIO32WriteFldAlign(PDWNC_WAKBTM0 + (12 * prSingleServoKeyList[i].u4Servo), u4Val, FLD_CH0BTM);
            
            
            if(u4Val)
                u4Val = CH_WAK_MID_EN & CH_MASK;
            else
                u4Val = (CH_WAK_MID_EN | CH_WAK_BTM_EN) & CH_MASK;
						vIO32WriteFldAlign(PDWNC_CHCFG0 + (12 * prSingleServoKeyList[i].u4Servo), u4Val, FLD_CH0WKC);
            
            u4Val = 0;

            if(i < 6)
	            vIO32WriteFldAlign(PDWNC_ADOUT0 + (prSingleServoKeyList[i].u4Servo * 4), u4Val, FLD_ADOUT0);
            else
            		vIO32WriteFldAlign(PDWNC_ADOUT0 + 0x24 + (prSingleServoKeyList[i].u4Servo * 4), u4Val, FLD_ADOUT0);
            *pu4PowerDownEn |= PDWNC_SRVAD_WAKEN(PDWNC_WAK_SRVADX(prSingleServoKeyList[i].u4Servo));
        }
        i++;
    }

ServoExit:
/*david: always enable srvadc engine for ch6/7 (by the way, all of power consumption measurement will includes keypad)  	
    if (!(*pu4PowerDownEn & PDWNC_WAK_ALLSRVAD))    	
    {
        Printf("Disable ServoADC\n");
      // No longer need to turn off ServoADC first.
        UNUSED(u4Val);
#if defined(CC_MT5365) || defined(CC_MT5395) 
        vIO32WriteFldAlign(PDWNC_ADCCFG1, 0x0, FLD_RG_SRV_PWD_B);
        vIO32WriteFldAlign(PDWNC_ADCCFG1, 0x1, FLD_RG_SRV_INBUFF_PD);
#else      
        vIO32WriteFldAlign(PDWNC_ADCCFG1, 0x0, FLD_SRV_PWD_B);
        vIO32WriteFldAlign(PDWNC_ADCCFG1, 0x1, FLD_IN_BUFF_PD);
#endif        
        vIO32WriteFldAlign(PDWNC_CLKPDN, 0x1, FLD_SRVPD);
    }
    else
*/    
    {    
#ifndef CC_MTK_LOADER
        CRIT_STATE_T rCrit;
#else /* CC_MTK_LOADER */
        //UINT32 u4State;
#endif /* CC_MTK_LOADER */
        
        // Enable servo ADC wakeup
        u4Val = *pu4PowerDownEn;        
       vIO32Write4B(PDWNC_WAKEN, u4Val );

        // Enable H/W
        vIO32WriteFldAlign(PDWNC_SRVCFG0, 0x1, FLD_HWEN);

        // To indicates servo AD wakeup has been set
#ifndef CC_MTK_LOADER
        rCrit = x_crit_start();
#endif /* CC_MTK_LOADER */
        _fgServoADWakeupSet = TRUE;
#ifndef CC_MTK_LOADER
        x_crit_end(rCrit);        
#endif /* CC_MTK_LOADER */
    }
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/** PDWNC_InitSrvad() Read the ServoADC Channel Value
 *  @param u4Channel the channel number
 *  @retval the channel adc value.
 */
//-----------------------------------------------------------------------------

INT32 PDWNC_InitServoGpio(void)
{
#if defined(CC_MT5365) || defined(CC_MT5395) 
    return 0;
#else
    #ifndef CC_MTK_LOADER
        INT32 i;
        INT32 i4Val;
        void (* pfnOldIsr)(UINT16);
        for (i=0; i<TOTAL_SERVOADC_NUM;i++)
        {
            _afnPDWNCServoGpioCallback[i] = NULL;
        }	
        // Hook PDWNC Gpio interrupt routine.
        for (i=0; i < TOTAL_SERVOADC_NUM; i++)
        {
            if(_ai1PdwncServoGpio2Int[ i] >= 0)
            {
                i4Val = PDWNC_RegIsr((UINT16)_ai1PdwncServoGpio2Int[i], _PdwncServoGpioIsr, &pfnOldIsr);
                if (i4Val != PDWNC_OK)
                {
                    return -1;
                }
            }
        }
        return 0;
    #else    
        return 0;
    #endif   
#endif

}

void PDWNC_ResumeServoADC(void)
{
    //INT32 i4Val;
    // Enable servo ADC H/W mode
	vIO32Write4B(PDWNC_CLKPDN, 0x0);
#if defined(CC_MT5365) || defined(CC_MT5395) 
        vIO32WriteFldAlign(PDWNC_ADCCFG1, 0x1, FLD_RG_SRV_PWD_B);
        vIO32WriteFldAlign(PDWNC_ADCCFG1, 0x0, FLD_RG_SRV_INBUFF_PD);
#else    
	vIO32WriteFldAlign(PDWNC_ADCCFG1, 0x1, FLD_SRV_PWD_B);
	vIO32WriteFldAlign(PDWNC_ADCCFG1, 0x0, FLD_IN_BUFF_PD);
#endif    
	vIO32WriteFldAlign(PDWNC_SRVCFG0, 0x0, FLD_SWEN);
	vIO32WriteFldAlign(PDWNC_SRVCFG0, 0x1, FLD_HWEN);
	//UNUSED(PDWNC_InitServoGpio());
}

void PDWNC_InitServoADC(void)
{
    //INT32 i4Val;
    // Enable servo ADC H/W mode
    if(_fgServoADCInit == 0)
    {        
    	vIO32Write4B(PDWNC_CLKPDN, 0x0);
    #if defined(CC_MT5365) || defined(CC_MT5395) 
            vIO32WriteFldAlign(PDWNC_ADCCFG1, 0x1, FLD_RG_SRV_PWD_B);
            vIO32WriteFldAlign(PDWNC_ADCCFG1, 0x0, FLD_RG_SRV_INBUFF_PD);
    #else    
    	vIO32WriteFldAlign(PDWNC_ADCCFG1, 0x1, FLD_SRV_PWD_B);
    	vIO32WriteFldAlign(PDWNC_ADCCFG1, 0x0, FLD_IN_BUFF_PD);
    #endif    
    	vIO32WriteFldAlign(PDWNC_SRVCFG0, 0x0, FLD_SWEN);
    	vIO32WriteFldAlign(PDWNC_SRVCFG0, 0x1, FLD_HWEN);
    #ifndef CC_MTK_LOADER    
        VERIFY(x_sema_create(&_hServoADMutex, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) == OSR_OK);    
    #endif    
    	UNUSED(PDWNC_InitServoGpio());
    	_fgServoADCInit = 1;
    }
}

//-----------------------------------------------------------------------------
/** PDWNC_ReadServoADCChannelValue() Read the ServoADC Channel Value
 *  @param u4Channel the channel number
 *  @retval the channel adc value.
 */
//-----------------------------------------------------------------------------
UINT32 PDWNC_ReadServoADCChannelValue(UINT32 u4Channel)
{
	UINT32 u4Val, u4Ret, i;

    // support 2 styles of servo adc numbering: 0 ~ 4 and 400 ~ 404
    if(u4Channel >= ADIN0)
    {
        u4Channel -= ADIN0;
    }
    // Maximum is 8 channel.
    VERIFY(u4Channel < TOTAL_SERVOADC_NUM);
    PDWNC_InitServoADC();
	
	i = 0;
	
#ifndef CC_MTK_LOADER
   VERIFY(x_sema_lock(_hServoADMutex, X_SEMA_OPTION_WAIT) == OSR_OK);
#endif

#if defined(CC_MT5387) || defined(CC_MT5363)
	u4Val = IO32ReadFld(PDWNC_SRVCFG1, FLD_SRVCH_EN);
	if ((u4Val & PDWNC_SRVCH_EN_CH(u4Channel)) == 0)
	{
	        u4Val |= PDWNC_SRVCH_EN_CH(u4Channel);
	        vIO32Write4B(PDWNC_SRVCFG1, u4Val);        
	        // Wait until servo AD sampling done.   
	        HAL_Delay_us(10);              
	}
    u4Ret = 0;
	if(u4Channel < 6)
		vIO32WriteFldAlign(PDWNC_ADOUT0 + (u4Channel * 4), u4Ret, FLD_ADOUT0);
	else
		vIO32WriteFldAlign(PDWNC_ADOUT0 + 0x24 + (u4Channel * 4), u4Ret, FLD_ADOUT0);

    HAL_Delay_us(85);     

    if(u4Channel < 6)    
			u4Ret = IO32ReadFldAlign(PDWNC_ADOUT0 + (u4Channel * 4), FLD_ADOUT0);
    else
    	u4Ret = IO32ReadFldAlign(PDWNC_ADOUT0 + 0x24 +  (u4Channel * 4), FLD_ADOUT0);
#else
    #if 0
    u4Val = IO32ReadFld(PDWNC_SRVCFG1, FLD_SRVCH_EN); //store in temp. variable
    vIO32Write4B(PDWNC_SRVCFG1, PDWNC_SRVCH_EN_CH(u4Channel));//vIO32WriteFldAlign(PDWNC_SRVCFG1, FLD_SRVCH_EN, PDWNC_SRVCH_EN_CH(u4Channel)); //only enable requested channel
    vIO32WriteFldAlign(PDWNC_SRVTOTEN, FLD_DATA_RDST, 0);// clear ready bit
    u4Ret = 0;
    do
    {
        HAL_Delay_us(5);
        if(u4Ret++ == 20)
        {
            Printf("Warning!! Read ADC channel %d fail...\n", u4Channel);
            break;
        }   
    }while(IO32ReadFld(PDWNC_SRVTOTEN, FLD_DATA_RDST) == 0);
    #else
    u4Val = IO32ReadFld(PDWNC_SRVCFG1, FLD_SRVCH_EN);
	if((u4Val & PDWNC_SRVCH_EN_CH(u4Channel)) == 0)
		{
	    	u4Val |= PDWNC_SRVCH_EN_CH(u4Channel);
	    	vIO32Write4B(PDWNC_SRVCFG1, u4Val);
			
			if(u4Channel < 6)
				{
					vIO32WriteFldAlign(PDWNC_ADOUT0 + (u4Channel * 4), 0, FLD_ADOUT0);
				}
			else
				{
					vIO32WriteFldAlign(PDWNC_ADOUT0 + 0x24 + (u4Channel * 4), 0, FLD_ADOUT0);
				}
			//wait for servoadc update
			HAL_Delay_us(50);
			while(((IO32ReadFldAlign(PDWNC_SRVCST, FLD_SRVCST)) != u4Channel) && (i<3000))
				{
				i++;	//use i for timeout
				}
			if(i >= 3000)
				{
					Printf("Wait Servoadc Channel%d update timeout!!!", u4Channel);
				}
		}
    #endif
    
    if(u4Channel < 6)    
    {
	    u4Ret = IO32ReadFldAlign(PDWNC_ADOUT0 + (u4Channel * 4), FLD_ADOUT0);
    }
    else
    {
    	u4Ret = IO32ReadFldAlign(PDWNC_ADOUT0 + 0x24 +  (u4Channel * 4), FLD_ADOUT0);    
    }
    
    #if 0    
    u4Val |= PDWNC_SRVCH_EN_CH(u4Channel);
    vIO32Write4B(PDWNC_SRVCFG1, u4Val);
    #endif
#endif         	
    if(_fgServoADWakeupSet)
    {
			u4Val = 0;
			if(u4Channel < 6)    
				vIO32WriteFldAlign(PDWNC_ADOUT0 + (u4Channel * 4), u4Val, FLD_ADOUT0);
			else
				vIO32WriteFldAlign(PDWNC_ADOUT0 + 0x24 + (u4Channel * 4), u4Val, FLD_ADOUT0);
    }
#ifndef CC_MTK_LOADER
    VERIFY(x_sema_unlock(_hServoADMutex) == OSR_OK);
#endif
    return (u4Ret & 0xff);
}

#if defined(CC_MT5365) || defined(CC_MT5395) 
//-----------------------------------------------------------------------------
/** PDWNC_SRVAD_Read : ABIST version of PDWNC_ReadServoADCChannelValue()
 * function name 'PDWNC_SRVAD_Read' is named by ABIST integrator
 *  @param u4Channel the channel number
 *  @retval the channel adc value.
 */
//-----------------------------------------------------------------------------
UINT32 PDWNC_SRVAD_Read(UINT32 u4Channel)
{
	UINT32 u4Val;
#if 1
	UINT32 u4OldSrvRate0, u4OldSrvRate1;
	u4OldSrvRate0 = IO32ReadFld(PDWNC_SRVCFG0, FLD_SMP_RATE0);
	u4OldSrvRate1 = IO32ReadFld(PDWNC_SRVCFG1, FLD_SMP_RATE1);
#endif
	vIO32WriteFldAlign(PDWNC_SRVRAT0, 0x0, FLD_SMP_RATE0);
	vIO32WriteFldAlign(PDWNC_SRVRAT1, 0x0, FLD_SMP_RATE1);
	vIO32WriteFldAlign(PDWNC_SRVCFG0, 0x1, FLD_ABIST_MODE);
	u4Val = PDWNC_ReadServoADCChannelValue(u4Channel);
#if 1 //restore
	vIO32WriteFldAlign(PDWNC_SRVRAT0, u4OldSrvRate0, FLD_SMP_RATE0);
	vIO32WriteFldAlign(PDWNC_SRVRAT1, u4OldSrvRate1, FLD_SMP_RATE1);
	vIO32WriteFldAlign(PDWNC_SRVCFG0, 0x0, FLD_ABIST_MODE);	
#endif
	return u4Val;
}
#endif


INT32 PDWNC_ServoGpioRangeCheck(INT32 i4Gpio)
{
     
#if defined(CC_MT5365) || defined(CC_MT5395) 
        if((i4Gpio >= (ADC2GPIO_CH_ID_MIN + SERVO_0_ALIAS)) && (i4Gpio <= (ADC2GPIO_CH_ID_MAX + SERVO_0_ALIAS)))//only srvad2~5 can be configured as gpio
        {
            return 1;
        }
        else
        {   
            return 0;
        }   
#else
        if((i4Gpio >= (ADC2GPIO_CH_ID_MIN+SERVO(0))) && (i4Gpio <= (ADC2GPIO_CH_ID_MAX+SERVO(0))))//only srvad2~5 can be configured as gpio
        {
            return 1;
        }
         else
         {   
             return 0;
         }   
#endif
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
INT32 PDWNC_ServoGpioEnable(INT32 i4Gpio, INT32 *pfgSet)
{
#if defined(CC_MT5365) || defined(CC_MT5395) 
        return (*pfgSet);
#else
    
    UINT32 u4Val;
#ifndef CC_MTK_LOADER
        CRIT_STATE_T rCrit;
#else /* CC_MTK_LOADER */
        //UINT32 u4State;
#endif 
//    if ((i4Gpio < 0) || (i4Gpio >= TOTAL_SERVOADC_NUM))
	if ((i4Gpio < ADC2GPIO_CH_ID_MIN) || (i4Gpio >  ADC2GPIO_CH_ID_MAX))
    {
        return -1;
    }
#ifndef CC_MTK_LOADER
        rCrit = x_crit_start();
#endif /* CC_MTK_LOADER */

    // Servo ADC used as GPIO (5362: 28394, 5387:28398)
//    u4Val = IO32ReadFldAlign(PDWNC_ADPADG, FLD_ADPADE);
    u4Val = IO32ReadFldAlign(PDWNC_ADPADE, FLD_ADPADE);//david: 20090803
    if (pfgSet == NULL)
    {
#ifndef CC_MTK_LOADER
        x_crit_end(rCrit);         
#endif /* CC_MTK_LOADER */

        //return ((u4Val & SRVIO_GP(i4Gpio)) ? 1 : 0);
        return ((u4Val & PDWNC_SRVIO_GP(i4Gpio)) ? 1 : 0);
    }
    u4Val = IO32ReadFldAlign(PDWNC_ADPADG, FLD_ADPADG);
    u4Val = u4Val | SRVIO_GP(i4Gpio);
    vIO32WriteFldAlign(PDWNC_ADPADG, u4Val, FLD_ADPADG);

    // Servo ADC used as GPO 
    u4Val = IO32ReadFldAlign(PDWNC_ADPADE, FLD_ADPADE);
    u4Val = (*pfgSet) ?
            (u4Val | PDANC_PAD_SERVOADC_EN(i4Gpio)) :
            (u4Val & ~(PDANC_PAD_SERVOADC_EN(i4Gpio)));
    vIO32WriteFldAlign(PDWNC_ADPADE, u4Val, FLD_ADPADE);
#ifndef CC_MTK_LOADER
        x_crit_end(rCrit);         
#endif /* CC_MTK_LOADER */
    return (*pfgSet);
#endif
}

//-----------------------------------------------------------------------------
/** PDWNC_ServoGpioOutput() The GPIO output value setting functions. It will check
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
INT32 PDWNC_ServoGpioOutput(INT32 i4Gpio, INT32 *pfgSet)
{
#if defined(CC_MT5365) || defined(CC_MT5395) 
        return (*pfgSet);
#else

    UINT32 u4Val;
    INT32 i4Idx;
#ifndef CC_MTK_LOADER
        CRIT_STATE_T rCrit;
#else /* CC_MTK_LOADER */
        //UINT32 u4State;
#endif /* CC_MTK_LOADER */

 //    if ((i4Gpio < 0) || (i4Gpio >= TOTAL_SERVOADC_NUM))
	if ((i4Gpio < 2) || (i4Gpio >  ADC2GPIO_CH_ID_MAX))
    {
        return -1;
    }

#ifndef CC_MTK_LOADER
        rCrit = x_crit_start();
#endif /* CC_MTK_LOADER */

    u4Val = IO32ReadFldAlign(PDWNC_ADPADO, FLD_ADPADO);
    if (pfgSet == NULL)
    {
#ifndef CC_MTK_LOADER
        x_crit_end(rCrit);         
#endif /* CC_MTK_LOADER */
        return ((u4Val & PDWNC_PAD_SRVIO_OUT(i4Gpio)) ? 1 : 0);
    }
    u4Val = (*pfgSet) ?
            (u4Val | PDWNC_PAD_SRVIO_OUT(i4Gpio)) :
            (u4Val & ~ PDWNC_PAD_SRVIO_OUT(i4Gpio));
            
    // Write pad output  
    vIO32WriteFldAlign(PDWNC_ADPADO, u4Val, FLD_ADPADO);
#ifndef CC_MTK_LOADER
        x_crit_end(rCrit);         
#endif /* CC_MTK_LOADER */

    // Set to output mode.
    i4Idx = 1;
    VERIFY(i4Idx==PDWNC_ServoGpioEnable(i4Gpio, &i4Idx));
    return (*pfgSet);
#endif    
}

//-----------------------------------------------------------------------------
/** PDWNC_ServoGpioInput()  The GPIO input reading functions. It will check the 
 *  i4Gpio and read related register bit to return.
 *  @param i4Gpio the gpio number to read.
 *  @retval 0 or 1.  (GPIO input value.)
 */
//-----------------------------------------------------------------------------
INT32 PDWNC_ServoGpioInput(INT32 i4Gpio)
{
#if defined(CC_MT5365) || defined(CC_MT5395) 
        return -1;
#else
    
    UINT32 u4Val;

 //    if ((i4Gpio < 0) || (i4Gpio >= TOTAL_SERVOADC_NUM))
	if ((i4Gpio < 2) || (i4Gpio >  ADC2GPIO_CH_ID_MAX))
    {
        return -1;
    }

    // Servo ADC used as GPIO
    u4Val = u4IO32Read4B(PDWNC_ADPADG);
    u4Val |= PDWNC_SRVIO_GP(i4Gpio);
    vIO32Write4B(PDWNC_ADPADG, u4Val);

    // Servo ADC used as GPI
    u4Val = u4IO32Read4B(PDWNC_ADPADE);
    u4Val &= ~(PDWNC_SRVIO_GP_O(i4Gpio));
    vIO32Write4B(PDWNC_ADPADE, u4Val);

    // Read pad input  
    u4Val = IO32ReadFldAlign(PDWNC_ADPADI, FLD_ADPADI);

    return ((u4Val & PDWNC_PAD_SRVIO_IN(i4Gpio)) ? 1U : 0);
#endif
}


void PDWNC_SetupServoADC(UINT32 *pu4PowerDownEn)
{
#ifdef  CC_UP8032_ATV
    __PDWNC_SetupServoADC(pu4PowerDownEn);
#else
    _PDWNC_SetupServoADC(pu4PowerDownEn);
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
INT32 PDWNC_ServoGpioIntrq(INT32 i4Gpio, INT32 *pfgSet)
{
    UINT32 u4Val;
    INT32 i4Int;
    
     if((i4Gpio < ADC2GPIO_CH_ID_MIN) || (i4Gpio > ADC2GPIO_CH_ID_MAX))//only srvad2~5 can be configured as gpio
    {
        return -1;
    }

    if((i4Int = _ai1PdwncServoGpio2Int[i4Gpio]) < 0)
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
INT32 PDWNC_ServoGpioQuery(INT32 i4Gpio, INT32 *pi4Intr, INT32 *pi4Fall, INT32 *pi4Rise)
{
	INT32 i4Int;

        UINT32 u4RegIntCfg;
    if ((i4Gpio < ADC2GPIO_CH_ID_MIN) || (i4Gpio >  ADC2GPIO_CH_ID_MAX))
    {
        return -1;
    }
	i4Int = _ai1PdwncServoGpio2Int[i4Gpio];
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
    
    u4RegIntCfg = PDWNC_ADINTCFG;
    
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
INT32 PDWNC_ServoGpioReg(INT32 i4Gpio, GPIO_TYPE eType, PFN_GPIO_CALLBACK pfnCallback)
{
#if defined(CC_MT5365) || defined(CC_MT5395) 
        return -1;
#else
    
    UINT32 u4Val;
    UINT32 u4RegIntCfg;
    INT32 i4Int;
    if ((i4Gpio < ADC2GPIO_CH_ID_MIN) || (i4Gpio >  ADC2GPIO_CH_ID_MAX))
    {
        return -1;
    }

    if((i4Int = _ai1PdwncServoGpio2Int[i4Gpio]) < 0)    
    {
        return -1;        
    }
    
    if(eType == GPIO_TYPE_NONE)
    {
        u4Val = u4IO32Read4B(PDWNC_ARM_INTEN);
        u4Val &= ~(1U << i4Int);       // Disable interrupt.
        vIO32Write4B(PDWNC_ARM_INTEN, u4Val);
        return 0;
    }
    else if(eType <= GPIO_TYPE_INTR_LEVEL_HIGH)
    {
        u4Val = u4IO32Read4B(PDWNC_ARM_INTEN);
        u4Val |= (1U << i4Int);        // Enable interrupt.
        vIO32Write4B(PDWNC_ARM_INTEN, u4Val);
    }
    u4Val = IO32ReadFldAlign(PDWNC_ADPADG, FLD_ADPADE) | SRVIO_GP(i4Gpio);
    vIO32WriteFldAlign(PDWNC_ADPADG, u4Val, FLD_ADPADG);
    // Interrupt trigger type configuration of first GPIO interrupt group
    u4RegIntCfg = PDWNC_ADINTCFG;
     i4Int -= PINTNO_SRVAD0;        
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
        u4Val = u4IO32Read4B(u4RegIntCfg);
        u4Val &= ~(1U << (i4Int+16));  // Disable double edge trigger.
        u4Val &= ~(1U << (i4Int+8));   // Set Edge trigger.
        u4Val |= (1U << i4Int);        // Set Rising Edge interrupt.
        vIO32Write4B(u4RegIntCfg, u4Val);
        break;
    case GPIO_TYPE_INTR_BOTH:
        u4Val = u4IO32Read4B(u4RegIntCfg);
        u4Val |= (1U << (i4Int+16));   // Enable double edge trigger.
        vIO32Write4B(u4RegIntCfg, u4Val);
        break;
    case GPIO_TYPE_INTR_LEVEL_LOW:
        u4Val = u4IO32Read4B(u4RegIntCfg);
        u4Val &= ~(1U << (i4Int+16));  // Disable double edge trigger.
        u4Val |= (1U << (i4Int+8));    // Set Level trigger.
        u4Val &= ~(1U << i4Int);       // Always level low active.
        vIO32Write4B(u4RegIntCfg, u4Val);
        break;
    case GPIO_TYPE_INTR_LEVEL_HIGH:
        u4Val = u4IO32Read4B(u4RegIntCfg);
        u4Val &= ~(1U << (i4Int+16));  // Disable double edge trigger.
        u4Val |= (1U << (i4Int+8));    // Set Level trigger.
        u4Val |= (1U << i4Int);       // Always level high active.
        vIO32Write4B(u4RegIntCfg, u4Val);
        break;
        
    default:
        return -1;
    }   
    
    if (pfnCallback)
    {
        _afnPDWNCServoGpioCallback[_ai1PdwncInt2ServoGpio[i4Int + PINTNO_SRVAD0]] = pfnCallback;
    }
    return 0;
#endif    
}
#else
INT32 PDWNC_ServoGpioIntrq(INT32 i4Gpio, INT32 *pfgSet)
{
	
	return -1;
}
INT32 PDWNC_ServoGpioQuery(INT32 i4Gpio, INT32 *pi4Intr, INT32 *pi4Fall, INT32 *pi4Rise)
{
	return -1;
}
INT32 PDWNC_ServoGpioReg(INT32 i4Gpio, GPIO_TYPE eType, PFN_GPIO_CALLBACK pfnCallback)
{
	return -1;
}
#endif /* CC_MTK_LOADER */
