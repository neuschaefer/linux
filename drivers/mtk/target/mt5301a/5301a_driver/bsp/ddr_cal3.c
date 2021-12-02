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
/** @file ddr_cal3.c
 *  Dram calibration algorithm 3.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "ddr.h"
#include "x_bim.h"
#include "x_dram.h"
#include "x_printf.h"
#include "x_ckgen.h"
#include "x_assert.h"

LINT_EXT_HEADER_END

//lint --e{717} do ... while (0);

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------
//#define CC_CALIB_DEBUG

#ifdef DRAM_DQINPUTPERBITCALI       
#ifndef CC_CALIB_DEBUG
#define CC_CALIB_DEBUG
#endif
#endif

#ifdef DRAM_DEBUG_TOOL
#undef CC_CALIB_DEBUG
#endif

// if GFX DMA mode fail or not stable, increase wait loop to 0x1000.
#define CC_GFXDMA_TEST

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define DQSTARTOFFSET 0

#define MAXDQSINPUTDELAY 70

#define PW_ITEM_NUM    32

#ifndef CC_CALIB_DEBUG
#define CALIB_STEP                  5
#else
#define CALIB_STEP                  1 
#endif

#define INITIAL_CALI_ITER_OFFSET 17

// Offset to avoid non-stable boundary
#define DDR_CALIB_JUMP              5

#define DRAM_HWCALI_ALIGNBITS     4

#define DRAM_HWCALI_BUFFERSIZE     8192

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
#ifndef DRAM_MANUAL_DQS_MODE
#ifdef DRAM_HW_CALIBRATION_V1
static void _DdrHWDQSCalStart(void)
{
    UINT32 u4addr;

    u4addr = (UINT32)(INIT_DRAM_B_BASE + CALIB_START);
    
    //audio pattern; loop=4, post write enabling for test agent 2.
    DRAM_WRITE32(0x218, 0x001F110D);

   #ifdef DRAM_HW_DQINPUTPERBITCALI
    DRAM_WRITE32(0x220, 0x14);
   #else
     DRAM_WRITE32(0x220, 0x04);
   #endif 
    //test agent2 address
    DRAM_WRITE32(0x210, u4addr);
    //test agent2 offset
    DRAM_WRITE32(0x214, DRAM_HWCALI_BUFFERSIZE);
    //enable DQS cal.
    #ifdef DRAM_HW_DQINPUTPERBITCALI
    DRAM_WRITE32(0x220, 0x34);
    #else
    DRAM_WRITE32(0x220, 0x24);
    #endif
}

static void _DdrHWDQSCalDisable(void) 
{ 
    DRAM_WRITE32(0x220, (DRAM_READ32(0x220) & ~0xFF)); 
}

static BOOL _DdrHWIsDQSCalDone(void)
{
    // CALI_DONE_MON: calibration result is updated, SW can disable calibration.
    // From 0 - > 1 : Calibration done.
    // From 1 - > 0 : Calibration is turned off at the internal of DRAMC.
    return ((DRAM_READ32(0x280) & 0x80000000) == 0) ? FALSE : TRUE;
}

static UINT32 _DdrCalibrateDqs(UINT32 u4ChipNum)
{
#ifdef DRAM_HW_DQINPUTPERBITCALI
    UINT32 i4DQMax0[4];
    UINT32 i4DQSMin0[4];
    UINT32 i4DQSMax0[4];
    UINT32 i4DQPerbit0[4];
    INT32 i4DQMax;
    INT32 i4DQSMin;
    INT32 i4DQSMax;
    INT32 i4DQSAvg;
#else
    INT32 i4DQMax;
    INT32 i4DQSMin;
    INT32 i4DQSMax;
    INT32 i4DQSAvg;
#endif
	
    UINT32 i;
    UINT32 u4Val = 0;
    UINT32 u4ManuDQS = 0;    
    UINT32 u4Bytes = u4ChipNum * BYTE_PER_CHIP;

    _DdrHWDQSCalStart();

    u4Val = 0;
    do
    {
        u4Val ++;
        CHIP_Delay_us(10);
    }while ((!_DdrHWIsDQSCalDone()) && (u4Val < 0xFFF));

    _DdrHWDQSCalDisable();

#ifdef DRAM_HW_DQINPUTPERBITCALI
        DRAM_WRITE32(0x220, DRAM_READ32(0x220) | 0<<16);
        i4DQPerbit0[0] = (INT32)DRAM_READ32(0x2E0);
	i4DQPerbit0[1] = (INT32)DRAM_READ32(0x2E4);
	i4DQPerbit0[2] = (INT32)DRAM_READ32(0x2E8);
	i4DQPerbit0[3] = (INT32)DRAM_READ32(0x2EC);
        DRAM_WRITE32(0x220, DRAM_READ32(0x220) | 1<<16);
        i4DQMax0[0] = (INT32)DRAM_READ32(0x2E0);
	i4DQMax0[1] = (INT32)DRAM_READ32(0x2E4);
	i4DQMax0[2] = (INT32)DRAM_READ32(0x2E8);
	i4DQMax0[3] = (INT32)DRAM_READ32(0x2EC);
	DRAM_WRITE32(0x220,( DRAM_READ32(0x220) & (~(3<<16)) )| 2<<16);
        i4DQSMin0[0] = (INT32)DRAM_READ32(0x2E0);
	i4DQSMin0[1] = (INT32)DRAM_READ32(0x2E4);
	i4DQSMin0[2] = (INT32)DRAM_READ32(0x2E8);
	i4DQSMin0[3] = (INT32)DRAM_READ32(0x2EC);
	DRAM_WRITE32(0x220, DRAM_READ32(0x220) | 3<<16);
        i4DQSMax0[0] = (INT32)DRAM_READ32(0x2E0);
	i4DQSMax0[1] = (INT32)DRAM_READ32(0x2E4);
	i4DQSMax0[2] = (INT32)DRAM_READ32(0x2E8);
	i4DQSMax0[3] = (INT32)DRAM_READ32(0x2EC);

        for(i=0; i < u4Bytes*2;i++)
        {
           Printf(" DQ %d : DQMax = %d,DQSMin = %d,DQSMax = %d\n",i*4,  (i4DQMax0[i] & 0xFF),(i4DQSMin0[i] & 0xFF),(i4DQSMax0[i] & 0xFF) );
	   Printf(" DQ %d : DQMax = %d,DQSMin = %d,DQSMax = %d\n",i*4+1,((i4DQMax0[i] >> 8 )& 0xFF),((i4DQSMin0[i] >> 8 ) & 0xFF),((i4DQSMax0[i] >> 8) & 0xFF) );
	   Printf(" DQ %d : DQMax = %d,DQSMin = %d,DQSMax = %d\n",i*4+2,((i4DQMax0[i] >>16)& 0xFF),((i4DQSMin0[i] >>16) & 0xFF),((i4DQSMax0[i] >>16) & 0xFF) );
	   Printf(" DQ %d : DQMax = %d,DQSMin = %d,DQSMax = %d\n",i*4+3,((i4DQMax0[i] >> 24) & 0xFF),((i4DQSMin0[i]>>24) & 0xFF),((i4DQSMax0[i] >>24)& 0xFF) );
        }

       //DQ Delay auto enable,DQS Cal update value
       DRAM_WRITE32(0x220, DRAM_READ32(0x220) |0xC0);

       for(i = 0; i < u4Bytes; i++)
       {
	   Printf("HW Byte %d :  DQ Perbit = %8X %8X\n",i,i4DQPerbit0[i*2+1],i4DQPerbit0[i*2]);
           //Printf("SW Byte %d :  DQ Perbit = %X %X\n",i,DRAM_READ32(0x3B0 + (i+1)*8),DRAM_READ32(0x3B0 + i*8));
           //Printf("HW DQS %d Center:  %d\n" ,i,((DRAM_READ32(0x29C) >> i*8) & 0xFF));
	   //Printf("SW DQS %d Center:  %d\n" ,i,((DRAM_READ32(0x3A0) >> i*8) & 0xFF));
       }
#endif
    // Need to update center value of DQS.
    for (i=0; i<u4Bytes; i++)
    {
        i4DQMax = (INT32)DRAM_READ32(0x290);
        i4DQMax = (i4DQMax >> (8*i)) & 0xFF;
        
        i4DQSMin = (INT32)DRAM_READ32(0x294);
        i4DQSMin = (i4DQSMin >> (8*i)) & 0xFF;
        
        i4DQSMax = (INT32)DRAM_READ32(0x298);
        i4DQSMax = (i4DQSMax >> (8*i)) & 0xFF;            
        
        i4DQSAvg = (INT32)DRAM_READ32(0x29C);
        i4DQSAvg = (i4DQSAvg >> (8*i)) & 0xFF;            
       
        if (i4DQMax != 0)
        {
            i4DQSMin -= i4DQMax;
        }

        if (i4DQSMin <= i4DQSMax)
        {
            // Get graycode of DQS delay.
            u4ManuDQS |= BIN_2_GREY((UINT32)i4DQSAvg) << (8 * i);

            u4Val = (((UINT32)i4DQSMin) << 24) | (((UINT32)i4DQSMax) << 16) |(((UINT32)i4DQSAvg) << 8);
            HAL_WRITE32(RECORD_DQS_REG + (i * 4), u4Val);

            LOG(1, "HW Byte %d : DQS(%2d ~ %2d),Size %d, Middle = %d.\n", 
                i, i4DQSMin, i4DQSMax,(i4DQSMax- i4DQSMin + 1), i4DQSAvg);
        }
        else
        {
            HAL_WRITE32(RECORD_DQS_REG + (i * 4), 0);

            LOG(1, "HW Byte %d : Calibration DQS(%2d ~ %2d) error !\n", 
                i, i4DQSMin, i4DQSMax);
        }
    } 

    // Note: This delay can not be removed.
    // After disable h/w calibration, it will actually being disabled at next refresh cycle.
    // At this critical section, we can not turn on MANUDQSUPD=1.
    // Check if Calibration is turned off at the internal of DRAMC.
    u4Val = 0;
    do
    {
        u4Val ++;
        CHIP_Delay_us(10);
    }while ((_DdrHWIsDQSCalDone()) && (u4Val < 0xFFF));
           
    // Update manual DQS input delay only while MANUDQSUPD=1 and DLLFRZ.   
    DRAM_WRITE32(0x234, (DRAM_READ32(0x234) | 0x00040000)); 
    
    DRAM_WRITE32(0x3A0, u4ManuDQS);

    // On-line manual DQS input delay adjust enabling.
    DRAM_WRITE32(0x234, (DRAM_READ32(0x234) | 0x00020000)); 

    CHIP_Delay_us(10);

    DRAM_WRITE32(0x234, (DRAM_READ32(0x234) & (~0x00060000))); 


    return 0;    
}

#else  /* Risc calibration */

UINT32 _Ddr4BitTo4Byte(UINT32 i)
{
    return (((i & 8) ? 0xFF000000 : 0) |
            ((i & 4) ? 0x00FF0000 : 0) |
            ((i & 2) ? 0x0000FF00 : 0) |
            ((i & 1) ? 0x000000FF : 0) );
}

INT32 _DdrNextIter(INT32 iter)
{
    INT32 new_iter;
    INT32 i = (iter >> 8) & 255;
    INT32 j= iter &255;

    if (i <= j)
    {
        new_iter = (j << 8) | i;
        if (j == (i + 1))
        {
            new_iter = ((i + 1) << 8) | 15;
            if (iter == 0x0E0F)
            {
                new_iter = 0x0F0E;
            }
        }
        if (i == j)
        {
            new_iter = 0x000F; //iter must be 0 now.
        }
    }
    else
    {
        new_iter = (j << 8) | (i - 1);
        if (i == (j + 1))
        {
            new_iter = ((i - 1) << 8) | (j - 1);
            if (j == 0)
            {
                new_iter = 0;
            }
        }
    }

    return new_iter;
}

static UINT32 _DdrCalibrateDqs(UINT32 u4ChipNum)
{
    UINT32 i;
    INT32 i4Min = 0, i4Max = 0, i4Avg = 0;
    UINT32 u4DQInputDelay[8];
    INT32 i4Step = 0;
    INT32 i4MinL, i4MaxL;
    UINT32 u4Rec = 0;
    BOOL fgCalibrationFail = FALSE;

#ifdef CC_CALIB_DEBUG
    UINT32 u4Val;
    INT8 logmem_lb[PW_ITEM_NUM];
    INT8 logmem_ub[PW_ITEM_NUM];
    INT32 i4MinMin = 0, i4MaxMax = 0;
    INT32 i4AlreadyFailedItem;
#ifdef DRAM_DQINPUTPERBITCALI       
    UINT32 j;
    UINT32 k;
    INT8 i1DQLb;
    INT8 i1DQUb;
    INT8 i1DQPerBitOffset;    
    INT8 i1DQPerBitDelay[8];   
#endif
#endif /* #ifdef CC_CALIB_DEBUG */

    // Keep DQ input delay.
    for (i=0; i<8; i++)
    {
        u4DQInputDelay[i] = DRAM_READ32(0x3B0 + 4*i);
    }        

    for (i = 0; i < (BYTE_PER_CHIP * u4ChipNum); i++)
    {
#ifdef CC_CALIB_DEBUG
        for (i4Avg = 0; i4Avg < PW_ITEM_NUM; i4Avg++)
        {
            logmem_lb[i4Avg] = -MAX_DQ_DELAY;
            logmem_ub[i4Avg] = -MAX_DQS_DELAY;
        }
        i4AlreadyFailedItem = 0;
#endif /* #ifdef CC_CALIB_DEBUG */

        i4Step = CALIB_STEP;

        // Using DQ to align DQS first
        for (i4Min = 0; i4Min < MAX_DQ_DELAY; i4Min += i4Step)
        {
            _DDR_SetDQ(i, i4Min, u4DQInputDelay);

#ifdef CC_CALIB_DEBUG

            u4Val = DDR_IsDqsFail(i, 0, DRAM_DQS_INPUT_TYPE, u4ChipNum);
            
            for (i4Avg = 0; i4Avg < PW_ITEM_NUM; i4Avg++)
            {
                if ((logmem_lb[i4Avg] == -MAX_DQ_DELAY) && ((u4Val >> i4Avg) & 0x1))
                {
                    logmem_lb[i4Avg] = i4Min;
                    i4AlreadyFailedItem |= (1<<i4Avg);
                }
            }
            
            if (i4AlreadyFailedItem == 0xffffffff)                	
            
#else /* #ifdef CC_CALIB_DEBUG */

            if (DDR_IsDqsFail(i, 0, DRAM_DQS_INPUT_TYPE, u4ChipNum))

#endif /* #ifdef CC_CALIB_DEBUG */
            {
                if (i4Step > 1)
                {
                    if (i4Min == 0)
                    {
                        break;
                    }
                    
                    i4MinL = i4Min - i4Step;
                    for (; i4Min > i4MinL; i4Min--)
                    {
                        _DDR_SetDQ(i, i4Min, u4DQInputDelay);  
                        
                        if (!DDR_IsDqsFail(i, 0, DRAM_DQS_INPUT_TYPE, u4ChipNum))
                        {
                            break;
                        }
                    }
                    if(i4Min == 0)
                    {
                        goto HIGH_BOUND_STAGE;
                    } 
                }									     
                break;
            }
        }

        _DDR_SetDQ(i, 0, u4DQInputDelay);        

#ifdef CC_CALIB_DEBUG
        i4MinMin = -(i4Min - 1);
        //change logmem_lb, to log OK DQ_delay_value, a minus number.
        //search max DQ_delay_value
        i4Min = -MAX_DQ_DELAY;
        for (i4Avg = 0; i4Avg < PW_ITEM_NUM; i4Avg++)
        {
            logmem_lb[i4Avg] = -(logmem_lb[i4Avg] - 1);
            
            if (logmem_lb[i4Avg] > i4Min)
            {
                i4Min = logmem_lb[i4Avg];
            }
        }
#else /* #ifdef CC_CALIB_DEBUG */

        //may check first whether i4Min==i4DQMax or not, then..
    	i4Min = -(i4Min - 1);

#endif /* #ifdef CC_CALIB_DEBUG */

        //for iMin/iMinMin, 1 means any possible positive DQS delay...
        if (i4Min >= 1) //iMinMin may be also 1, or not.
        {
            for (i4Min = 1; i4Min < MAX_DQS_DELAY; i4Min += i4Step)
            {
#ifdef CC_CALIB_DEBUG
                u4Val=DDR_IsDqsFail(i, i4Min, DRAM_DQS_INPUT_TYPE, u4ChipNum);

                for (i4Avg = 0; i4Avg < PW_ITEM_NUM; i4Avg++)
                {
                    if ((logmem_lb[i4Avg] >= 1) && ((u4Val >> i4Avg) & 0x1))                
                    {
                        logmem_lb[i4Avg] = i4Min;
                    }
                }

                //if all fail, set i4MinMin=i4Min+1
                if (u4Val == 0xffffffff)
                {
                    i4MinMin = i4Min + 1;
                }
    
                if (u4Val == 0) //none nibbles failed, then...

#else /* #ifdef CC_CALIB_DEBUG */

                if (!DDR_IsDqsFail(i, i4Min, DRAM_DQS_INPUT_TYPE, u4ChipNum))

#endif /* #ifdef CC_CALIB_DEBUG */
                {
                    if (i4Step)
                    {
                        i4MinL = i4Min - i4Step;
                        for (; i4Min > i4MinL; i4Min--)
                        {                        
                            if(i4Min == 0)
                            {	
                                break;//(1)
                            }
                            if (DDR_IsDqsFail(i, i4Min, DRAM_DQS_INPUT_TYPE, u4ChipNum))
                            {
                                break;//(2)
                            }
                        }
                        i4Min++;//if i4Min == 0 here, it must be fail
                    }                    
                    break;
                }
            }
        }

        // Cannot find ok DQS, return failed.
        if (i4Min >= MAX_DQS_DELAY)
        {
            _DDR_SetDQS(i, 0);

            fgCalibrationFail = TRUE;

#if (!defined(CC_CALIB_DEBUG)) && (!defined(DRAM_DEBUG_TOOL))
       	    CHIP_DisplayString("Byte ");
	    CHIP_DisplayInteger(i);
	    CHIP_DisplayString(" Cal Fail.\n");
            continue;
#endif /* (!defined(CC_CALIB_DEBUG)) && (!defined(DRAM_DEBUG_TOOL)) */
        }

HIGH_BOUND_STAGE:

        // Find maximal DQS value
#ifdef CC_CALIB_DEBUG
        i4AlreadyFailedItem = 0;
        i4Max = (i4Min >=  MAX_DQS_DELAY) ? 0 : (i4Min + 1);
#else /* #ifdef CC_CALIB_DEBUG */
        i4Max = i4Min + DDR_CALIB_JUMP;
#endif /* #ifdef CC_CALIB_DEBUG */            

        i4Max = i4Max < 0 ? 0 : i4Max;

        for (; i4Max < MAX_DQS_DELAY; i4Max += i4Step)
        {
#ifdef CC_CALIB_DEBUG
            u4Val = DDR_IsDqsFail(i, i4Max, DRAM_DQS_INPUT_TYPE, u4ChipNum);
            
            for (i4Avg = 0; i4Avg < PW_ITEM_NUM; i4Avg++)
            {
                if ((logmem_ub[i4Avg] == -MAX_DQS_DELAY) && ((u4Val >> i4Avg) & 0x1))
                {
                    logmem_ub[i4Avg] = i4Max;
                    i4AlreadyFailedItem |= (1<<i4Avg);
                }
            }

            if (i4AlreadyFailedItem == 0xffffffff)
#else /* #ifdef CC_CALIB_DEBUG */

            if (DDR_IsDqsFail(i, i4Max, DRAM_DQS_INPUT_TYPE, u4ChipNum))
#endif /* #ifdef CC_CALIB_DEBUG */
            {
                if (i4Step)
                {
                    i4MaxL = i4Max - i4Step;
                    for(; i4Max > i4MaxL; i4Max --)
                    {
                        if (!DDR_IsDqsFail(i, i4Max, DRAM_DQS_INPUT_TYPE, u4ChipNum))
                        {
                            break;
                        }
                    }
                }                
                break;
            }
        }

#ifdef CC_CALIB_DEBUG

        i4MaxMax = i4Max - 1;
        //change logmem_ub, to log OK DQS_delay_value.
        //search min DQS_delay_value
        i4Max = MAX_DQS_DELAY;
        for (i4Avg = 0; i4Avg < PW_ITEM_NUM; i4Avg++)
        {
            if (logmem_lb[i4Avg] > logmem_ub[i4Avg])
            {
                logmem_ub[i4Avg] = logmem_lb[i4Avg];
            }
            else
            {
                logmem_ub[i4Avg] = logmem_ub[i4Avg] - 1;
            }
            
            if (logmem_ub[i4Avg] < i4Max)
            {
                i4Max = logmem_ub[i4Avg];
            }
        }

        //dump ....................
        LOG(1,"\n");
        for (u4Val = 0; u4Val < PW_ITEM_NUM; u4Val++)
        {
            LOG(1, "bit%2d %s %d (%2d ~ %2d) ", 
                ((i << 3) | (u4Val >> 2)), 
                ((u4Val & 2) ? "F" : "R"), 
                (u4Val & 1), 
                logmem_lb[u4Val], logmem_ub[u4Val]);

            if (i4Min <= i4Max)
            {
                if (logmem_lb[u4Val] > -10)
                {
                    LOG(1," ");
                }

                for (i4Avg = i4MinMin; i4Avg < logmem_lb[u4Val]; i4Avg++)
                {
                    LOG(1,"x");
                }
                
                for (; i4Avg <= logmem_ub[u4Val]; i4Avg++)
                {
                    LOG(1, "=");
                }
                
                for (; i4Avg <= i4MaxMax; i4Avg++)
                {
                    LOG(1,"x");
                }
            }
                        
            LOG(1,"\n");
        }

        if (i4Min <= i4Max)
        {
            LOG(1,"    %s%s            ", "  ", "  ");
            LOG(1, " ");
                	
            for (i4Avg = i4MinMin; i4Avg <= i4MaxMax; i4Avg++)
            {
                u4Val = (i4Avg + 100) % 10;
                if (u4Val == 0)
                {
                    LOG(1, "|");
                }
                else if (u4Val == 5)
                {
                    LOG(1, ":");
                }
                else
                {
                    LOG(1, ".");
                }
            }
        }
        
        LOG(1,"\niMinMin=%d, iMin=%d, iMax=%d, iMaxMax=%d\n",
            i4MinMin, i4Min, i4Max, i4MaxMax);
        
#else /* #ifdef CC_CALIB_DEBUG */

        if (i4Step == 1)
        {
            i4Max--;
        }

#endif /* #ifdef CC_CALIB_DEBUG */

        if ((i4Min != MAX_DQS_DELAY) && 
            (i4Min <= i4Max))
        {
            i4Avg = (i4Min + i4Max) / 2;
            
            //Base on comparing CLI:memtest.dqs and boot up value, need to offset the DQS center value.
            if ((DDR_I_x1 == TCMGET_DRAMTYPE()) || 
                (DDR_I_x2 == TCMGET_DRAMTYPE()))
            {
                i4Avg = i4Avg-3; //offset DQS 
            }
            else
            {           
                i4Avg = i4Avg-2; //offset DQS 
            }
        }
        else
        {
            i4Avg = 0;        
        }
        
        if (i4Avg >= 0)
        {
#ifdef CC_CALIB_DEBUG

#ifdef DRAM_DQINPUTPERBITCALI    

            // search per bit delay.
            i1DQPerBitOffset = 0;
            for (j=0; j<8; j++)
            {
                i1DQLb = logmem_lb[j*4];
                i1DQUb = logmem_ub[j*4];
                for (k=1; k<4; k++)
                {
                    if (i1DQLb < logmem_lb[k+(j*4)])
                    {
                        i1DQLb = logmem_lb[k+(j*4)];
                    }
                    if (i1DQUb > logmem_ub[k+(j*4)])
                    {
                        i1DQUb= logmem_ub[k+(j*4)];
                    }
                }                
                i1DQPerBitDelay[j] = i4Avg - ((i1DQLb+i1DQUb)/2);
                if (i1DQPerBitOffset > i1DQPerBitDelay[j])
                {
                    i1DQPerBitOffset = i1DQPerBitDelay[j];
                }                
            }

            // Shift i4Avg and i1DQPerBitDelay[] by minimum value.
            for (j=0; j<8; j++)
            {
                i1DQPerBitDelay[j] -= i1DQPerBitOffset;
            }
            i4Avg -= i1DQPerBitOffset;
            
            _DDR_SetDQPerBit(i, i1DQPerBitDelay, u4DQInputDelay);

#else

            _DDR_SetDQ(i, 0, u4DQInputDelay);

#endif /* DRAM_DQINPUTPERBITCALI */

#else  /* #ifdef CC_CALIB_DEBUG */

            _DDR_SetDQ(i, 0, u4DQInputDelay);

#endif /* #ifdef CC_CALIB_DEBUG */

            _DDR_SetDQS(i, (UINT32)i4Avg);
        }
        else
        {
            _DDR_SetDQ(i, -i4Avg, u4DQInputDelay);
            _DDR_SetDQS(i, 0);
        }

        u4Rec = (((UINT32)i4Min) << 24) | (((UINT32)i4Max) << 16) |(((UINT32)i4Avg) << 8);
        HAL_WRITE32(RECORD_DQS_REG + (i * 4), u4Rec);

        if (i4Min <= i4Max)
        {
#ifndef DRAM_DEBUG_TOOL
            LOG(1, "Byte %d : DQS(%2d ~ %2d), Size %d, Set = %d.\n", 
                i, i4Min, i4Max, ((i4Max - i4Min) + 1), i4Avg);
#endif /* #ifndef DRAM_DEBUG_TOOL */
        }
        else
        {
            LOG(1, "Byte %d : Calibration DQS(%2d ~ %2d) error !\n", 
                i, i4Min, i4Max);
        }

#ifdef __MODEL_slt__
        
        /*
        SLT use 12 as threhold to check this IC performance.
        */
        if ((i4Max < i4Min) || (((i4Max - i4Min) + 1) <= 12))
        {
            CHIP_DisplayString("DRAM Byte");
	    CHIP_DisplayInteger(i);
            CHIP_DisplayString(" DQS(");
	    CHIP_DisplayInteger(i4Min);
            CHIP_DisplayString(" ~ ");
	    CHIP_DisplayInteger(i4Max);
            CHIP_DisplayString("), Size=");
	    CHIP_DisplayInteger(((i4Max - i4Min) + 1));
	    CHIP_DisplayString(" <= 12, system halt !!!\n");
            
            // Lock system and wait to check this ic.
            while (1);
        }

#endif /* #ifdef __MODEL_slt__ */
    }

    return (UINT32)fgCalibrationFail;
}

UINT32 DDR_IsDqsFail(UINT32 u4DQSth, UINT32 u4DQSVal, UINT32 u4DQSType, UINT32 u4ChipNum)
{
    UINT32 u4Addr = CALIB_START, u4Mask, u4Val, ib = INITIAL_CALI_ITER_OFFSET;
    INT32 iter, c, iter_mod_3 = 0;
    UINT32 u4MemVal;
        
#ifdef CC_CALIB_DEBUG
    UINT32 errval, errcode=0, flag, u4Data;
    UINT8 kk;
    UINT32 u4EdgeAddrAlign;

    u4EdgeAddrAlign = (BYTE_PER_CHIP * u4ChipNum);
#endif

    if (u4DQSType == DRAM_DQS_INPUT_TYPE)
    {
        _DDR_SetDQS(u4DQSth, u4DQSVal);
    }
#ifdef DRAM_WRITE_DQS_CALIBRATION   
    else if (u4DQSType == DRAM_DQS_OUTPUT_TYPE)
    {    
        _DDR_SetOutDQS(u4DQSth, u4DQSVal);
    }
#endif    

    // wait dram stable
    CHIP_Delay_us(10);

    u4Mask = 0xffU << (8 * (u4DQSth & 3));

    for (c = 0; c < 32; c++)
    {
        // 1 c is 4Byte*241, 8 c is nearly (<) 8K Bytes
#ifdef CC_GFXDMA_TEST
        if ((c & 7) == 0)
        { //start of write or read
            if ((c & 15) == 0)
            {
                u4Addr = CALIB_START; //start of write
            }
            ib = INITIAL_CALI_ITER_OFFSET;
            iter_mod_3 = 0;
        }
        if (c & 8)
        {
            // *((volatile UINT32*)(0x2000D224)) = 0x6;    // Gfx pll = 162mhz
            *((volatile UINT32*)(0x2000D230)) = 0x5;    // Gfx pll = 162mhz

            //read
            // GFX DMA test (for write initialization, it's redundant):
            u4Val = INIT_DRAM_B_BASE + u4Addr;
            *((volatile UINT32*)(0x2000401C)) = u4Val;  // Dst

            u4Val -= 8 * 241 * 4;
            *((volatile UINT32*)(0x20004018)) = u4Val;  // Src
            *((volatile UINT32*)(0x20004028)) = 0x50000000 | (241 * 4);
            *((volatile UINT32*)(0x20004044)) = 0x0;
            *((volatile UINT32*)(0x20004010)) = 0x862;

            // i = 0x100; //DDR2-729-544bytes
            // => maybe vecdor will use external bus 16-bit mode  =>

    	    iter = 0;
            do
            {
                if ((iter++) & 0xFFFF0000)
                {
                    LOG(1, "Gfx dma error!\n");
                    break;
                }
            }while (((*((volatile UINT32*)(0x20004004))) & 1)==0);
        }
#else

        if ((c & 7) == 0)
        {
            //start of write or read
            u4Addr = CALIB_START;
            ib = INITIAL_CALI_ITER_OFFSET;
            iter_mod_3 = 0;
        }

#endif

        iter = 0;
        do
        {
            if (c & 16)
            {
                ib = 32;
            }
            u4Val = _Ddr4BitTo4Byte(iter & 255) ^ (1 << ib);
            if (c & 8)
            {
                u4MemVal = MEM_READ32(u4Addr);
                
#ifdef CC_CALIB_DEBUG
		errval = (u4MemVal ^ u4Val) & u4Mask;
                if (errval)
                {
                    u4Data = u4Val & u4Mask;
                    u4Data >>= (8 * (u4DQSth & 3));
                    errval >>= (8 * (u4DQSth & 3));

                    for(kk = 0; kk < 8; kk++)
                    {
                        if( (errval & 0x01) == 1 )
                        {
                            flag = 0;
                            if((u4Addr + u4DQSth) & u4EdgeAddrAlign) /* TRUE: DQS failing, FALSE: DQS rising */
                                flag += 2;
                            if((u4Data >> kk) & 1) /* TRUE: DQ = 1, FALSE: DQ = 0 */
                                flag += 1;
                            /*
                            errcode format: 
                            DQ bit 0 at [3:0], DQ bit 1 at [7:4], DQ bit 2 at [11:8], DQ bit 3 at [15:12].                            
                            DQ bit 4 at [19:16], DQ bit 5 at [23:20], DQ bit 6 at [27:24], DQ bit 7 at [31:28].                            
                            each DQ bit content => DQS: F, F, R, R
                                                                DQ:   1, 0, 1, 0
                            */
                            errcode |= (1 << flag) << (4 * kk);	                				
                        }
                        errval >>= 1;
                    }
                }

                if (u4ChipNum == 1)
                {
                    errval = (u4MemVal ^ u4Val) & (u4Mask << 16);
                    if (errval)
                    {
                        u4Data = u4Val & (u4Mask << 16);
                        u4Data >>= (8 * ((u4DQSth+2) & 3));
                        errval >>= (8 * ((u4DQSth+2) & 3));

                        for(kk = 0; kk < 8; kk++)
                        {
                            if( (errval & 0x01) == 1 )
                            {
                                flag = 0;
                                if((u4Addr + (u4DQSth+2)) & u4EdgeAddrAlign) /* TRUE: DQS failing, FALSE: DQS rising */
                                    flag += 2;
                                if((u4Data >> kk) & 1) /* TRUE: DQ = 1, FALSE: DQ = 0 */
                                    flag += 1;
                                /*
                                errcode format: 
                                DQ bit 0 at [3:0], DQ bit 1 at [7:4], DQ bit 2 at [11:8], DQ bit 3 at [15:12].                            
                                DQ bit 4 at [19:16], DQ bit 5 at [23:20], DQ bit 6 at [27:24], DQ bit 7 at [31:28].                            
                                each DQ bit content => DQS: F, F, R, R
                                                                    DQ:   1, 0, 1, 0
                                */
                                errcode |= (1 << flag) << (4 * kk);	                				
                            }
                            errval >>= 1;
                        }
                    }
                }                
#else  /* #ifdef CC_CALIB_DEBUG */

                if ((u4MemVal ^ u4Val) & u4Mask)
                {
                    return TRUE;
                }

                if (u4ChipNum == 1)
                {
                    if ((u4MemVal ^ u4Val) & (u4Mask << 16))
                    {
                        return TRUE;
                    }
                }		

#endif /* #ifdef CC_CALIB_DEBUG */

            }
            MEM_WRITE32(u4Addr, u4Val);
            if (iter_mod_3 == 0)
            {
                ib = (ib + 13) & 31;
                iter_mod_3 = 3;
            }
            iter_mod_3--;
            u4Addr += 4;
            iter = _DdrNextIter(iter);
        } while (iter != 0);
    }

#ifdef CC_CALIB_DEBUG

    if(errcode == 0)
    {
        return FALSE;
    }
    else
    {
        return errcode;
    }

#else

    return FALSE;

#endif  	
}

#endif /* #ifdef DRAM_HW_CALIBRATION_V1 */

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** DDR_CalibrateDqs() to calculate and set the DQS offset.
 *  @param void
 *  @retval 0 successfully calibrate, otherwise failure
 */
//-----------------------------------------------------------------------------
UINT32 DDR_CalibrateDqs(void)
{
    UINT32 u4Ret;
    UINT32 u4ChipNum = (IS_FORCE32()) ? 1 : 2;
   
    // Calibrate Input delay.
    u4Ret = _DdrCalibrateDqs(u4ChipNum);

    return u4Ret;
}

#else /* #ifndef DRAM_MANUAL_DQS_MODE */

UINT32 DDR_CalibrateDqs(void)
{
    INT32 i4Avg;
    UINT32 u4ChipNum = (IS_FORCE32()) ? 1 : 2;
    UINT32 i;
    UINT32 u4DQInputDelay[8];
    UINT32 u4Rec;

    // Keep DQ input delay.
    for (i=0; i<8; i++)
    {
        u4DQInputDelay[i] = DRAM_READ32(0x3B0 + 4*i);
    }        

    for (i = 0; i < (BYTE_PER_CHIP * u4ChipNum); i++)
    {
        i4Avg = (INT8)((((UINT32)MANUAL_DQS) >> (i * 8)) & 0xFF);

        LOG(1, "Byte %d : DQS Set = %d.\n", i, i4Avg);

        u4Rec =  (((UINT32)i4Avg) << 8);
        HAL_WRITE32(RECORD_DQS_REG + (i * 4), u4Rec);

        if (i4Avg >= 0)
        {
            _DDR_SetDQS(i, (UINT32)i4Avg);
        }
        else
        {
            _DDR_SetDQ(i, (UINT32)(-i4Avg), u4DQInputDelay);
            _DDR_SetDQS(i, 0);
        }
    }
    return 0;
}
#endif /* #ifndef DRAM_MANUAL_DQS_MODE */

