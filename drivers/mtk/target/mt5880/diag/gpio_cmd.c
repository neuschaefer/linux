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
 * Description:
 *         The CLI commands for gpio driver
 *
 *---------------------------------------------------------------------------*/

#include "x_serial.h"
#include "x_printf.h"
#include "x_os.h"
#include "x_stl_lib.h"
#include "x_assert.h"
#include "x_pinmux.h"
#include "drvcust_if.h"
#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif
#define DEFINE_IS_LOG	CLI_IsLog
#include "x_debug.h"
#include "x_timer.h"
#include "x_pdwnc.h"
#include "u_dbg_drv.h"

EXTERN INT32 GPIO_VotageLevel(INT32 i4GpioNum);
//===========================================================================
// Configuration
#define FPD_TEST_ENABLE             0

//---------------------------------------------------------------------

static INT32 _GPIOCLI_Init(INT32 i4Argc, const CHAR** aszArgv)
{
    GPIO_Init();
    return 0;
}

static INT32 _GPIOCLI_Stop(INT32 i4Argc, const CHAR** aszArgv)
{
    GPIO_Stop();
    return 0;
}

static INT32 _GPIOCLI_Reset(INT32 i4Argc, const CHAR** aszArgv)
{
    GPIO_Stop();
    GPIO_Init();
	return 0;
}

static INT32 _GPIOCLI_List(INT32 i4Argc, const CHAR** aszArgv)
{
    INT32 i, i4Intr, i4Fall, i4Rise;

    for (i=0; i<TOTAL_GPIO_NUM; i++) {
        GPIO_Query(i, &i4Intr, &i4Fall, &i4Rise);
        if (i4Intr || i4Fall || i4Rise) {
            Printf("GPIO%d(%c,%c,%c)\n", i, (i4Intr ? 'I' : 'X'),
                (i4Fall ? 'F' : 'X'), (i4Rise ? 'R' : 'X'));
        }
    }
	return 0;
}

static void _GPIO_Callback(INT32 i4Gpio, BOOL fgStatus)
{
    Printf("GPIO:%d fgStatus:%d\n", i4Gpio, (INT32)fgStatus);
}

static INT32 _GPIOCLI_Enable(INT32 i4Argc, const CHAR** aszArgv)
{
    INT32 i, i4Gpio, i4Intr, i4Rise, i4Fall, i4Low, i4High;

    if (i4Argc < 2) {
        Printf("%s [-i -r -f -L -H] [gpio num]\n", aszArgv[0]);
        return 1;
    }

    i4Gpio = i4Intr = i4Rise = i4Fall = i4High = i4Low = 0;
    for (i=1; i<i4Argc; i++) {
        switch(aszArgv[i][0]) {
        case '-':
            if (aszArgv[i][1]=='i') {
                i4Intr = 1;
            } else if (aszArgv[i][1]=='r') {
                i4Rise = 1;
            } else if (aszArgv[i][1]=='f') {
                i4Fall = 1;
            }else if (aszArgv[i][1]=='L') {
                i4Low = 1;
            }else if (aszArgv[i][1]=='H') {
                i4High = 1;
            }
            break;
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            i4Gpio = StrToInt(aszArgv[i]);
            break;
        default:
            Printf("%s [-i -r -f -L -H] [gpio num]\n", aszArgv[0]);
            return 1;
        }
    }
    if (!i4Intr && !i4Rise && !i4Fall && !i4High && !i4Low) {
        Printf("%s [-i -r -f -L -H] [gpio num]\n", aszArgv[0]);
        return 1;
    }

    BSP_PinGpioSel(i4Gpio);
    GPIO_Reg(i4Gpio, GPIO_TYPE_NONE, NULL);
    Printf("GPIO %d - ", i4Gpio);
    if (i4Intr) {
        GPIO_Reg(i4Gpio, GPIO_TYPE_INTR_BOTH, _GPIO_Callback);
        Printf(" INTR ");
    }
    if (i4Fall) {
        GPIO_Reg(i4Gpio, GPIO_TYPE_INTR_FALL, _GPIO_Callback);
        Printf(" INTRFALL ");
    }
    if (i4Rise) {
        GPIO_Reg(i4Gpio, GPIO_TYPE_INTR_RISE, _GPIO_Callback);
        Printf(" INTRRISE ");
    }
    if (i4Low) {
        GPIO_Reg(i4Gpio, GPIO_TYPE_INTR_LEVEL_LOW, _GPIO_Callback);
        Printf(" INTRLOW");
    }
    if (i4High) {
        GPIO_Reg(i4Gpio, GPIO_TYPE_INTR_LEVEL_HIGH, _GPIO_Callback);
        Printf(" INTRHIGH ");
    }
    
    Printf("\n");
    return 0;
}

static INT32 _GPIOCLI_Disable(INT32 i4Argc, const CHAR** aszArgv)
{
    INT32 i4Gpio;

    if (i4Argc < 2) {
        Printf("%s [gpio num]\n", aszArgv[0]);
        return 1;
    }

    i4Gpio = StrToInt(aszArgv[1]);
    GPIO_Reg(i4Gpio, GPIO_TYPE_NONE, NULL);
    Printf("Disable all gpio function in gpio %d\n", i4Gpio);
    return 0;
}

static INT32 _GPIOCLI_Output(INT32 i4Argc, const CHAR ** aszArgv)
{
    INT32 i4Gpio, i4Val, i4Set;

    if (i4Argc < 2) {
        Printf("%s [gpio num] [[0|1]]\n", aszArgv[0]);
        return 1;
    }
    i4Gpio = StrToInt(aszArgv[1]);

    if (i4Argc == 2) { /* Query only. */
        //BSP_PinGpioSel(i4Gpio);
        i4Set = GPIO_Output(i4Gpio, NULL);
        Printf("GPIO %d output mode is set as %d\n", i4Gpio, i4Set);
        return 0;
    }
    i4Set = StrToInt(aszArgv[2]);

    i4Val = 0;
    GPIO_Enable(i4Gpio, &i4Val); /* Change to input mode first. */    
    //BSP_PinGpioSel(i4Gpio);    
    GPIO_Output(i4Gpio, &i4Set);
    i4Val = 1;
    GPIO_Enable(i4Gpio, &i4Val); /* Change to output mode. */
    Printf("GPIO %d output mode set as %d\n", i4Gpio, i4Set);
    return 0;
}

static INT32 _GPIOCLI_Output_Verify(INT32 i4Argc, const CHAR ** aszArgv)
{
    INT32 i4Gpio_start,i4Gpio_end, i4Val, i;

    if (i4Argc > 3) {
        Printf("%s gpio.out_verify [gpio_start] [gpio_end]", aszArgv[0]);
        return 1;
    }
    else if(i4Argc ==3)
    {
        i4Gpio_start = StrToInt(aszArgv[1]);
        i4Gpio_end = StrToInt(aszArgv[2]);
    }
    else 
    {
        i4Gpio_start = 0;
        i4Gpio_end = NORMAL_GPIO_NUM;
    }
    for(i =i4Gpio_start; i <= i4Gpio_end;i4Gpio_start++)
        GPIO_Enable(i4Gpio_start, 0); /* Change to input mode first. */ 
    while(1)
    {
        for(i =i4Gpio_start; i <= i4Gpio_end;i4Gpio_start++)
        {				   
            //BSP_PinGpioSel(i4Gpio);   
            i4Val = 0;
            GPIO_Output(i4Gpio_start, &i4Val);
            i4Val = 1;
            GPIO_Enable(i4Gpio_start, &i4Val); /* Change to output mode. */
            GPIO_Output(i4Gpio_start, &i4Val);				
        }
        x_thread_delay(1000);
        for(i =i4Gpio_start; i <= i4Gpio_end;i4Gpio_start++)
        {
            i4Val = 0;
            //BSP_PinGpioSel(i4Gpio);    
            GPIO_Output(i4Gpio_start, &i4Val);
            i4Val = 1;
            GPIO_Enable(i4Gpio_start, &i4Val); /* Change to output mode. */			
            GPIO_Output(i4Gpio_start, &i4Val);
            x_thread_delay(1000);
        }
        x_thread_delay(1000);
    }
//     return 0;
}

static INT32 _GPIOCLI_Input(INT32 i4Argc, const CHAR ** aszArgv)
{
    INT32 i4Gpio, i4Val;

    if (i4Argc < 2) {
        Printf("%s [gpio num]\n", aszArgv[0]);
        return 1;
    }

    i4Gpio = StrToInt(aszArgv[1]);
    i4Val = 0;
    GPIO_Enable(i4Gpio, &i4Val); /* Change to input mode first. */
    i4Val = GPIO_Input(i4Gpio);
    Printf("Read GPIO %d value is %d\n", i4Gpio, i4Val);
    return 0;
}
static INT32 _GPIOCLI_VoltageLvl(INT32 i4Argc, const CHAR ** aszArgv)
{
    INT32 i4Gpio, i4Val;
    INT32 i,j;
    if (i4Argc < 2) 
    {//Printf ALL
        Printf("%s [gpio num]\t(list specific gpio level)\n", aszArgv[0]);
        Printf("%s \t\t(list all)\n", aszArgv[0]);

        Printf("\r   ");
        for(j=0; j <10;j++)
            Printf("%2d", j);
        Printf("\n");
        for(i = 0; i<13 ;i++)
        {
            Printf("%02d ", i);
            for(j=0; j <10;j++)
            {
                i4Val = GPIO_VotageLevel( i*10+j );
                Printf(" %s", (i4Val)?"H":"L" );
            }
            Printf("\n");
        }
        return 0;
    }

    //Print specific GPIO 
    i4Gpio = StrToInt(aszArgv[1]);
    i4Val = 0;

    i4Val = GPIO_VotageLevel( i4Gpio );

    Printf("Read GPIO %d value is %d\n", i4Gpio, i4Val);
    return 0;
}

#if FPD_TEST_ENABLE
extern INT32 MW_FpdSet7Seg(UINT8 u1Digit);
#endif /* FPD_TEST_ENABLE */

static INT32 _GPIOCLI_Seg7(INT32 i4Argc, const CHAR ** aszArgv)
{
#if FPD_TEST_ENABLE
    INT32 i4Gpio, i4Disable, i4Enable;

    if (i4Argc > 1) {
        MW_FpdSet7Seg(aszArgv[1][0]);
        return 0;
    }

    i4Disable = 1;
    i4Enable = 0;
    UNUSED(i4Enable);
    for (i4Gpio = 115; i4Gpio < 128; i4Gpio++) {
        BSP_PinGpioSel(i4Gpio);
        GPIO_Output(i4Gpio, &i4Disable);
    }
#endif /* FPD_TEST_ENABLE */
    return 0;
}

#if FPD_TEST_ENABLE
extern INT32 MW_FpdSetBuf(UINT32 u4Offset, UINT8 *pu1Digit);
extern INT32 FPD_DevInit(void);
#endif /* FPD_TEST_ENABLE */

static INT32 _GPIOCLI_Time(INT32 i4Argc, const CHAR ** aszArgv)
{
#if FPD_TEST_ENABLE
    INT32 i;
    UINT8 szBuf[10];

    FPD_DevInit();
    if ((i4Argc >= 2) && aszArgv[1][0]=='-' && aszArgv[1][1]=='l') {
        for (i=0; i<9999; i++) {
            x_sprintf((CHAR *)szBuf, "%04d", i);
            MW_FpdSetBuf(0, szBuf);
            x_thread_delay(50);
        }
        return 0;
    }

    if (i4Argc >= 2) {
        MW_FpdSetBuf(0, (UINT8 *)aszArgv[1]);
        return 0;
    }

    x_sprintf((CHAR *)szBuf, "%04d", 1234);
    MW_FpdSetBuf(0, szBuf);
#endif /* FPD_TEST_ENABLE */
    return 0;
}

#define KEY_ESCAPE                      0x1b
static INT32 _i4Idx;
static UCHAR _aucArray[256];

static INT32 _GPIOCLI_Test(INT32 i4Argc, const CHAR ** aszArgv)
{
    INT32 i;
    UCHAR uc;

    if ((i4Argc == 1) || ((i4Argc > 1) && (aszArgv[1][0]!='-'))) {
        for (i=0; i<_i4Idx; i++) {
            if (i%16==15) {
                Printf("0x%02x\n", _aucArray[i]);
            } else {
                Printf("0x%02x ", _aucArray[i]);
            }
        }
        return 0;
    }

    switch(aszArgv[1][1]) {
    case 'i': case 'I':
        _i4Idx = 0;
        do {
            // SerInByte(&uc);
            uc = SerPollGetChar();
            if (uc != KEY_ESCAPE) {
                _aucArray[_i4Idx] = uc;
               if( _i4Idx < 127) _i4Idx++;
                _i4Idx &= 0xff;
            }
        } while (uc != KEY_ESCAPE);
        break;
    case '0':
    default:
        _i4Idx = 0;
        for (i=0; i<128; i++) { _aucArray[i] = 0; }
        break;
    }
    return 0;
}

#ifndef __KERNEL__
#ifndef CC_MINI_DRIVER
extern void BSP_DoCoreDump(void* prCtx);
#endif /* CC_MINI_DRIVER */
extern void BSP_ShowReg(void);

static INT32 _GPIOCLI_CoreDump(INT32 i4Argc, const CHAR ** aszArgv)
{
#ifndef CC_MINI_DRIVER
    BSP_DoCoreDump(NULL);
#endif /* CC_MINI_DRIVER */
    return 0;
}

static INT32 _GPIOCLI_ShowReg(INT32 i4Argc, const CHAR ** aszArgv)
{
    BSP_ShowReg();
    return 0;
}
#endif /* __KERNEL__ code not in Linux */

static INT32 _GPIOCLI_Assert(INT32 i4Argc, const CHAR ** aszArgv)
{
    ASSERT(0);
    return 0;
}

typedef void (* PFN_VOID)(void);
static UINT32 u4Data0xffff = 0xffffffff;

static INT32 _GPIOCLI_Abort(INT32 i4Argc, const CHAR ** aszArgv)
{
    PFN_VOID pfnAbort;

    if (i4Argc != 2) {
        Printf("gpio.abort [p|d|u]\n");
        Printf("\tPrefetch abort|Data abort|Undefined Instruction\n");
        return 1;
    }

    switch (aszArgv[1][0]) {
    case 'p': case 'P':
        pfnAbort = (PFN_VOID)0xabcdef12;
        pfnAbort();
        break;
    case 'd': case 'D':
        u4Data0xffff = *((UINT32 *) 0xabcdef12);
        break;
    case 'u': case 'U':
        pfnAbort = (PFN_VOID) & u4Data0xffff;
        pfnAbort();
        break;
    default:
        Printf("gpio.abort [p|d|u]\n");
        Printf("\tPrefetch abort|Data abort|Undefined Instruction\n");
        break;
    }
    return 0;
}

static INT32 _GPIOCLI_Servo(INT32 i4Argc, const CHAR ** aszArgv)
{
    UINT32 u4Val;

    if (i4Argc < 2)
    {
        Printf("%s [0-4] to read servo adc channel value.\n", aszArgv[0]);
        return -1;
    }

    u4Val = StrToInt(aszArgv[1]);
    Printf("Channel %d value is %d\n", u4Val, PDWNC_ReadServoADCChannelValue(u4Val));
    return 0;
}
#if 0
static void _GPIOCLI_ServoIsr(UINT16 u2Vector)
{
    UINT32 u4Val, u4Top, u4Btm, u4ServoNo;

    u4ServoNo = (u2Vector - PINTNO_SRVAD0);
    u4Val = PDWNC_ReadServoADCChannelValue(u4ServoNo);
    LOG(5, "Channel %d get %d - 0x%08x->", u4ServoNo, u4Val, 
        PDWNC_READ32(CHCFG0_REG + (4*u4ServoNo)));
    if (u4Val < 30)
    {
        // PDWNC_SetServoADCCfg(u4ServoNo, 176, 25, CH_INT_TOP_EN | CH_INT_MID_EN);
        u4Val = CHTOP_VAL(176) | CHBTM_VAL(25) | (CH_WAK_MID_EN & CH_MASK);
        PDWNC_WRITE32(CHCFG0_REG + (4*(u4ServoNo)), u4Val);
    }
    else
    {
        u4Top = u4Val + 15;
        u4Btm = u4Val - 15;
        // PDWNC_SetServoADCCfg(u4ServoNo, u4Top, u4Btm, CH_INT_TOP_EN | CH_INT_BTM_EN);
        u4Val = CHTOP_VAL(u4Top) | CHBTM_VAL(u4Btm) | (CH_WAK_MID_EN & CH_MASK);
        PDWNC_WRITE32(CHCFG0_REG + (4*(u4ServoNo)), u4Val);
    }
    LOG(5, "0x%08x\n", PDWNC_READ32(CHCFG0_REG + (4*u4ServoNo)));
    PDWNC_ClearInt((UINT32)u2Vector);
}

static INT32 _GPIOCLI_SrvIntr(INT32 i4Argc, const CHAR ** aszArgv)
{
    UINT32 u4Val;
    INT32 i4Ret;
    x_os_isr_fct pfnOld;

    if (i4Argc < 2)
    {
        Printf("%s [0-4] to enable interrupt of servo adc.\n", aszArgv[0]);
        return -1;
    }

    u4Val = StrToInt(aszArgv[1]);
    if (u4Val >= TOTAL_SERVOADC_NUM)
    {
        Printf("%s [0-4] to enable interrupt of servo adc.\n", aszArgv[0]);
        return 1;
    }

    // PDWNC_SetServoADCCfg(u4Val, 176, 25, CH_INT_TOP_EN | CH_INT_MID_EN);
    PDWNC_WRITE32(CHCFG0_REG + (4*(u4Val)), CHTOP_VAL(176) | CHBTM_VAL(25) | (CH_WAK_MID_EN & CH_MASK));
    i4Ret = PDWNC_RegIsr(PINTNO_SRVAD0+u4Val, _GPIOCLI_ServoIsr, &pfnOld);
    Printf("Hook Isr on ServoADC Channel %d return %d\n", u4Val, i4Ret);
    return 0;
}


static INT32 _GPIOCLI_Dimming(INT32 i4Argc, const CHAR ** aszArgv)
{
    UINT32 u4Hz, u4Duty;

    if (i4Argc < 3)
    {
        Printf("Usage: %s [hz] [duty]\n", aszArgv[0]);
        return -1;
    }

    u4Hz = StrToInt(aszArgv[1]);
    u4Duty = StrToInt(aszArgv[2]);

    if ((u4Hz < 4) || (u4Hz > 400) || (u4Duty > 100))
    {
        Printf("Usage: %s [hz] [duty]\n", aszArgv[0]);
        Printf("\thz: 4~400, duty:0~100\n");
        return -1;
    }

    GPIO_SetDimming(u4Hz, u4Duty);
    return 0;
}

static INT32 _GPIOCLI_DimOff(INT32 i4Argc, const CHAR ** aszArgv)
{
    BSP_PinSet(GPIO_OPWM1, 0);
    return 0;
}
#endif

#define MAX_CAL_NS 10

static INT32 _SltTestPll(CAL_SRC_T eSrc, UINT32 u4Target, UINT32 u4Bound)
{
#if (defined CC_MT5365) || (defined CC_MT5395) || defined(CC_MT5396)
    // MT5365_TODO
    return 0;

#else // !CC_MT5365

    INT32 i4Fail = 0;
    UINT32 u4CalNs = 0, u4Clock;

    switch (eSrc)
    {
    case CAL_SRC_DMPLL:
        Printf("test DMPLL: ");
        break;
    #if !defined(CC_MT8223)
    case CAL_SRC_TVDPLL:
        Printf("test TVDPLL: ");
        break;
    case CAL_SRC_DTDPLL:
        Printf("test DTDPLL: ");
        break;
    #endif
    case CAL_SRC_SYSPLL:
        Printf("test SYSPLL: ");
        break;
    case CAL_SRC_APLL1:
        Printf("test APLL1: ");
        break;
    case CAL_SRC_APLL2:
        Printf("test APLL2: ");
        break;
    default:
        return -1;
    }

    do
    {
        if (eSrc == CAL_SRC_APLL1 || eSrc == CAL_SRC_APLL2)
        {
            VERIFY(BSP_Calibrate(eSrc, u4Target));
            ++u4CalNs;
        }
        else
        {
            u4CalNs = MAX_CAL_NS;
        }

        x_thread_delay(5);
        u4Clock = BSP_GetPLLClock(eSrc);

        if ((u4Clock > (u4Target + u4Bound)) ||
            (u4Clock < (u4Target - u4Bound)))
        {
            Printf("FAIL: u4CalNs=%d, u4Target=%d, u4Clock=%d)\n",
                   u4CalNs, u4Target, u4Clock);
            i4Fail++;
        }
    } while (u4CalNs < MAX_CAL_NS);

    if (i4Fail == 0)
    {
        Printf("PASS (target:%d, clock:%d) PASS\n", u4Target, u4Clock);
    }

    return i4Fail;

#endif // CC_MT5365
}

static INT32 _GPIOCLI_PllTest(INT32 i4Argc, const CHAR ** aszArgv)
{
    #if (!defined(CC_MT8223) && !defined(CC_MT5365) && !defined(CC_MT5395) && !defined(CC_MT5396))
    // MT5365_TODO
    _SltTestPll(CAL_SRC_TVDPLL, 540000000, 1000000);
    _SltTestPll(CAL_SRC_DTDPLL, 600000000, 1000000);
    #endif
    #if defined(CC_MT5395) || defined(CC_MT5396)
    _SltTestPll(SRC_SYSPLL_CLK, 459000000, 1000000);
    #else
    _SltTestPll(CAL_SRC_SYSPLL, 324000000, 1000000);
    #endif
    _SltTestPll(CAL_SRC_APLL1, CLK_APLL294MHZ, 1000000);
    _SltTestPll(CAL_SRC_APLL2, CLK_APLL270MHZ, 1000000);
    return 0;
}

static void _GpioCLI_WatchdogThread(void *pvArgv)
{
    UINT32 u4Seconds;
    HAL_TIME_T rPrevTime, rTime, rDelta;

    u4Seconds = *((UINT32 *)pvArgv);
    Printf("u4Seconds:%d\n", u4Seconds);

    HAL_GetTime(&rPrevTime);

    do {
        HAL_GetTime(&rTime);
        HAL_GetDeltaTime(&rDelta, &rPrevTime, &rTime);
    } while (rDelta.u4Seconds < u4Seconds);

    Printf("Finish %d seconds\n", u4Seconds);
}

typedef void (*PFN_TESTFUNC)(void);
const UINT32 _au4Swi[] = { 0xef000045, 0xef000046 };

static INT32 _GPIOCLI_TestWatchdog(INT32 i4Argc, const CHAR ** aszArgv)
{
    PFN_TESTFUNC pfnTest;
    UINT32 u4Priority = 0;
	UINT32 u4Seconds;
    INT32 i4Ret;
    HANDLE_T hWatchdogThread;

    if (i4Argc < 2)
    {
        Printf("%s s|m|n|t [priority] [seconds]\n", aszArgv[0]);
        return 0;
    }
    else if (aszArgv[1][0]=='s')
    {
        pfnTest = (PFN_TESTFUNC)&_au4Swi;
        pfnTest();
        return 0;
    }
    else if (aszArgv[1][0]=='n')
    {
        pfnTest = (PFN_TESTFUNC)0x30000000;//david for klocwork warning (PFN_TESTFUNC)0;
        pfnTest();
    }
    else if (aszArgv[1][0]=='m')
    {
        x_dbg_abort(__LINE__, 0x12345678);
    }
    else if (aszArgv[1][0]=='t')
    {
        if (i4Argc >= 4)
        {
            u4Priority = StrToInt(aszArgv[2]);
            u4Seconds = StrToInt(aszArgv[3]);
        }
        if ((i4Argc < 4) || (u4Priority == 0) || (u4Seconds == 0))
        {
            Printf("%s s|m|n|t [priority] [seconds]\n", aszArgv[0]);
            return 0;
        }

        u4Priority = (u4Priority > 250) ? 250 : u4Priority;
        u4Seconds = (u4Seconds > 10) ? 10 : u4Seconds;

        i4Ret = x_thread_create(&hWatchdogThread, "wtest", 4096, u4Priority,
                    _GpioCLI_WatchdogThread, 4, &u4Seconds);
        if (i4Ret != OSR_OK)
        {
            Printf("Create thread to test watchdog failed.\n");
            return 1;
        }
    }
    
    return 0;
}

static INT32 _GPIOCLI_PinSel(INT32 i4Argc, const CHAR ** aszArgv)
{
    INT32 i4GpioNum, i4FuncNo;
    UINT32 u4PinSel;

    if (i4Argc < 2)
    {
        Printf("%s [gpio num] [[func no.]]\n", aszArgv[0]);
        return 1;
    }

    i4GpioNum = StrToInt(aszArgv[1]);
    if (i4Argc < 3)
    {
        i4FuncNo = BSP_PinGpioGet(i4GpioNum, &u4PinSel);
        Printf("PinSel:0x%08x GpioNum:%d Current function is %d\n", u4PinSel, i4GpioNum, i4FuncNo);
        return 0;
    }
    i4FuncNo = StrToInt(aszArgv[2]);
    BSP_PinGpioGet(i4GpioNum, &u4PinSel);
    BSP_PinSet(u4PinSel, i4FuncNo);
    Printf("PinSel:0x%08x GpioNum:%d function set %d\n", u4PinSel, i4GpioNum, i4FuncNo);
    i4FuncNo = BSP_PinGet(u4PinSel);
    Printf("PinSel:0x%08x GpioNum:%d Current function is %d\n", u4PinSel, i4GpioNum, i4FuncNo);
    Printf("\tLoRegIdx:%d LoMask:%d LoBits:%d\n", LO_MUX_REG(u4PinSel), LO_MASK(u4PinSel), LO_BIT_OFFSET(u4PinSel));
    Printf("\tHiRegIdx:%d HiMask:%d HiBits:%d\n", HI_MUX_REG(u4PinSel), HI_MASK(u4PinSel), HI_BIT_OFFSET(u4PinSel));
    Printf("\tMaxFunc:%d\n", MAX_FUNC(u4PinSel));
    return 0;
}

//===========================================================================

static CLI_EXEC_T arGpioCmdTbl[] =
{
    DECLARE_CMD(_GPIOCLI_Init, init, init, "gpio.init"),
    DECLARE_CMD(_GPIOCLI_Stop, stop, stop, "gpio.stop"),
    DECLARE_CMD(_GPIOCLI_Reset, reset, reset, "gpio.reset"),
    DECLARE_CMD(_GPIOCLI_List, list, ls, "gpio.ls"),
    DECLARE_CMD(_GPIOCLI_Enable, enable, en, "gpio.en [-i/-r/-f/-L/-H][gpio num]"),
    DECLARE_CMD(_GPIOCLI_Disable, disable, dis, "gpio.dis [gpio num]"),
    DECLARE_CMD(_GPIOCLI_PinSel, pinsel, ps, "gpio.ps [sel num] [[func no.]]"),
    DECLARE_G_CMD(_GPIOCLI_Output, output, out, "gpio.out [gpio num] [[0|1]]"),
    DECLARE_G_CMD(_GPIOCLI_Output_Verify, output_verify, out_v, "gpio.out_verify [gpio_start] [gpio_end]"),
    
    DECLARE_G_CMD(_GPIOCLI_Input, input, in, "gpio.in [gpio num]"),
    DECLARE_G_CMD(_GPIOCLI_VoltageLvl, vl , vl, "gpio.vl [gpio num]"),
    DECLARE_CMD(_GPIOCLI_Seg7, seg7, seg7, "gpio.seg7"),
    DECLARE_CMD(_GPIOCLI_Time, time, time, "gpio.time"),
    DECLARE_CMD(_GPIOCLI_Test, test, test, "gpio.test"),
#ifndef __KERNEL__
    DECLARE_CMD(_GPIOCLI_CoreDump, coredump, co, "gpio.co or gpio.coredump"),
    DECLARE_CMD(_GPIOCLI_ShowReg, showreg, sr, "gpio.sr or gpio.showreg"),
#endif /* __KERNEL__ code not in Linux */
    DECLARE_CMD(_GPIOCLI_Assert, assert, assert, "gpio.assert"),
    DECLARE_CMD(_GPIOCLI_Abort, abort, abort, "gpio.abort"),
    DECLARE_G_CMD(_GPIOCLI_Servo, servo, servo, "gpio.servo [0-4]"),
//    DECLARE_CMD(_GPIOCLI_SrvIntr, srvintr, srvintr, "gpio.srvintr [0-4]"),
//    DECLARE_G_CMD(_GPIOCLI_Dimming, dimming, dim, "gpio.dim [hz] [duty]"),
//    DECLARE_G_CMD(_GPIOCLI_DimOff, dimoff, dimoff, "gpio.dimoff"),
    DECLARE_CMD(_GPIOCLI_PllTest, plltest, plt, "gpio.plt - to test PLL"),
    DECLARE_CMD(_GPIOCLI_TestWatchdog, testwatchdog, twg, "gpio.twg - to test watchdog"),
    DECLARE_END_ITEM(),
};

CLI_MAIN_COMMAND_ITEM(Gpio)
{
	"gpio",
	NULL,
	NULL,
	arGpioCmdTbl,
	"Gpio interface",
	CLI_GUEST
};

#if 0
#ifdef __MW_CLI_DEF__
CLI_EXEC_T* GetGpioMwCmdTbl(void)
#else
CLI_EXEC_T* GetGpioCmdTbl(void)
#endif
{
	return &_rGpioCmdTbl;
}
#endif

