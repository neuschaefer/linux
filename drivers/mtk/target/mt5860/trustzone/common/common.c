/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2004, MediaTek, Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: common.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file timer.c
 *  timer.c provides timer function from boot time.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_ckgen.h"
#include "x_chipreg.h"
#include "x_bim.h"
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

void SetupGpioAs0()
{
#if 0
#if 0 // ndef CC_INIT_BLOCK
    BIM_WRITE32(REG_RW_TIMER_CTRL, (0));
    BIM_WRITE32(REG_RW_TIMER2_LLMT, 0xffffffff);
    BIM_WRITE32(REG_RW_TIMER2_LOW , 0xffffffff);
    BIM_WRITE32(REG_RW_TIMER2_HLMT, 0xffffffff);
    BIM_WRITE32(REG_RW_TIMER2_HIGH, 0xffffffff);
    BIM_WRITE32(REG_RW_TIMER_CTRL, (TMR2_CNTDWN_EN | TMR2_AUTOLD_EN));
#endif /* CC_INIT_BLOCK */

    //MX3[13:12]
    CKGEN_WRITE32(REG_PINMUX_SEL3, CKGEN_READ32(REG_PINMUX_SEL3) & ~(0x3 << 12));
    CKGEN_WRITE32(REG_GPIO_EN0, CKGEN_READ32(REG_GPIO_EN0) | (0x1 << 23) );
    CKGEN_WRITE32(REG_GPIO_OUT0, CKGEN_READ32(REG_GPIO_OUT0) & ~(0x1 << 23) );

    CKGEN_WRITE32(REG_PINMUX_SEL3, CKGEN_READ32(REG_PINMUX_SEL3) & ~(0x3 << 12));
    CKGEN_WRITE32(REG_GPIO_EN0, CKGEN_READ32(REG_GPIO_EN0) | (0x1 << 24));
    CKGEN_WRITE32(REG_GPIO_OUT0, CKGEN_READ32(REG_GPIO_OUT0) & ~(0x1 << 24));
#endif
}

//GPIO23
void SetTestDone() 
{
#if 0
    //MX3[13:12]
    CKGEN_WRITE32(REG_PINMUX_SEL3, CKGEN_READ32(REG_PINMUX_SEL3) & ~(0x3 << 12));
    CKGEN_WRITE32(REG_GPIO_EN0, CKGEN_READ32(REG_GPIO_EN0) | (0x1 << 23) );
    CKGEN_WRITE32(REG_GPIO_OUT0, CKGEN_READ32(REG_GPIO_OUT0) | (0x1 << 23) );
#endif
}

//GPIO24
void SetTestPass(BOOL isPass) 
{
#if 0
    CKGEN_WRITE32(REG_PINMUX_SEL3, CKGEN_READ32(REG_PINMUX_SEL3) & ~(0x3 << 12));
    CKGEN_WRITE32(REG_GPIO_EN0, CKGEN_READ32(REG_GPIO_EN0) | (0x1 << 24));
    if(isPass)
        CKGEN_WRITE32(REG_GPIO_OUT0, CKGEN_READ32(REG_GPIO_OUT0) | (0x1 << 24));
    else
        CKGEN_WRITE32(REG_GPIO_OUT0, CKGEN_READ32(REG_GPIO_OUT0) & ~(0x1 << 24));
#endif
}

void SetSingleDataOutput(UINT32 u4Data)
{
    BIM_WRITE32(REG_RW_GPRDW0, u4Data);
}

void SetDataOutput(UINT32 u4Sym, UINT32 u4Val)
{
    BIM_WRITE32(REG_RW_GPRDW0, u4Sym);
    BIM_WRITE32(REG_RW_GPRDW0, u4Val);
}

#define REG_D2R_SDA 0x2000509c

void DSP_ClearInt(void)
{
    HAL_WRITE32(REG_D2R_SDA, 0x0);
}

UINT32 u4DspRegR_D2rSda(void)
{
    return (HAL_READ32(REG_D2R_SDA));
}

UINT32 WaitForDSPDone() 
{
    UINT8 bDspRIntAddr;
    UINT32 u4DspRIntData;
    UINT32 u4DspRIntData2;
    UINT32 u4DspRIntType;
    UINT32 u4WaitCnt = 0;

    DSP_ClearInt();

    while(1){
        u4WaitCnt++;
        if ((u4WaitCnt & 0xfffff)==0)
        {
            Printf("DSP wait cnt:0x%08x!\n", u4WaitCnt);
            if ((u4WaitCnt & 0xffffff)==0)
            {
                Printf("DSP wait timeout!!!\n");
                return 1;
            }
        }
        u4DspRIntData = u4DspRegR_D2rSda();
        bDspRIntAddr = (u4DspRIntData & 0xFF);
        u4DspRIntData = (u4DspRIntData >> 8);
        DSP_ClearInt();
        if(bDspRIntAddr==0x10){
            break;
        }        
    }
    return 0;
}

