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
#include "drvcust_if.h"
#include "x_printf.h"
#include "x_ckgen.h"
#include "x_assert.h"

LINT_EXT_HEADER_END

//lint --e{717} do ... while (0);

extern void CHIP_Delay1_us(UINT32 u4Micros);

#if (CC_CALIB_VER == 3)


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

// CC_CALIB_DEBUG : currently implemented for CC_CALIB_VER==1 mode;
/*
   CC_CALIB_DEBUG==1 : detail dump, not for general use, just for debug.
   CC_CALIB_DEBUG==4 : detail DQS window test result, simplest per bit info
   CC_CALIB_DEBUG==5 : detail DQS window test result, DQS rising v.s. falling
   CC_CALIB_DEBUG==6 : detail DQS window test result, sample 0 v.s. 1 error
   CC_CALIB_DEBUG==7 : detail DQS window test result, type 5 + type 6
*/
#define CC_CALIB_DEBUG              0
//#define CC_CALIB_DEBUG              7

// if GFX DMA mode fail or not stable, increase wait loop to 0x1000.
#define CC_GFXDMA_TEST              1


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

// for IsDQSFail() return value, each nibble contains 1, 2, or 4 items.
// power of # of items
#define PW_ITEM_NUM     ((CC_CALIB_DEBUG & 1) + ((CC_CALIB_DEBUG & 2) >> 1))
#define PW_ITEM_BITS    (2 - PW_ITEM_NUM)

#if CC_CALIB_DEBUG == 0
	#define CALIB_STEP                  5
#else
	#define CALIB_STEP                  1//It must be 1 at debug mode(CC_CALIB_DEBUG != 0)
#endif
const UINT32 u4DqsIen[5] = {DDR_DQSIEN, DDR_DQSIEN + 10, DDR_DQSIEN + 20,
                            DDR_DQSIEN - 10, DDR_DQSIEN - 20};


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

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

#define INITIAL_CALI_ITER_OFFSET 17

//note: not for 16-bit ... .
static UINT32 _DdrIsDQSFail(UINT32 u4DQSth, UINT32 u4Bin)
{
    UINT32 u4Addr = CALIB_START, u4Mask, u4Val, ib = INITIAL_CALI_ITER_OFFSET;
    INT32 iter, c, iter_mod_3 = 0;
#if CC_CALIB_DEBUG>=4 && CC_CALIB_DEBUG<=7    
    UINT32 errval, errcode=0, flag, u4Data;
    UINT8 kk;
	#if (DEFAULT_DRAM_TYPE==DDR_III_x1) || (DEFAULT_DRAM_TYPE==DDR_III_x2)
	    #define BYTE_PER_CHIP   4
	#else
	    #define BYTE_PER_CHIP   2
	#endif
	
	#if DEFAULT_DRAM_TYPE==DDR_II_x2
	    #define CHIP_NUM        2
	#else
	    #define CHIP_NUM        1
	#endif
	#define EDGE_ADDRESS_ALIGN (BYTE_PER_CHIP * CHIP_NUM)
#endif
    _DDR_SetDQS(u4DQSth, u4Bin);

    // wait dram stable
#ifdef CC_5392B_PRELOADER
    CHIP_Delay1_us(10);
#else
    WAIT_DRAM_STABLE();
#endif


#if CC_ONLY_SUPPORT_16BIT
    u4Mask = (u4DQSth >= 8) ?
        (0xffffffffU) : (0x00ff00ffU << (8 * (u4DQSth & 3)));
#else
    u4Mask = 0xffU << (8 * (u4DQSth & 3));
#endif //CC_ONLY_SUPPORT_16BIT

    for (c = 0; c < 32; c++)
    {
        //1 c is 4Byte*241, 8 c is nearly (<) 8K Bytes
#if CC_GFXDMA_TEST
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
            *((volatile UINT32*)(0x2000D224)) = 0x6;    // Gfx pll = 162mhz

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
            /*
            iter = 0x400;
            do
            {
            } while (--iter);
            // i should be 0 now.
            */
    	    iter = 0;
            do
            {
                if ((iter++) & 0xFFFF0000)
                {
                    LOG(1, "Gfx dma error!\n");
                    break;
                }
            }
        	while (((*((volatile UINT32*)(0x20004004))) & 1)==0);
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
        #if CC_CALIB_DEBUG>=4 && CC_CALIB_DEBUG<=7

			    			errval = (MEM_READ32(u4Addr) ^ u4Val) & u4Mask;
                if (errval)
                {
                	u4Data = u4Val & u4Mask;
                	u4Data >>= (8 * (u4DQSth & 3));
                	errval >>= (8 * (u4DQSth & 3));
			
                	for(kk = 0; kk < 8; kk++)
                	{
                		if( (errval & 0x01) == 1 )
                    {
#if CC_CALIB_DEBUG==7                    	
                    	flag = 0;
                    	if((u4Addr + u4DQSth) & EDGE_ADDRESS_ALIGN)
                    		flag += 1;
                			if((u4Data >> kk) & 1)
                				flag += 2;
                			errcode |= (1 << flag) << (4 * kk);	                				
#elif CC_CALIB_DEBUG==6 
                    	flag = 2;
                			if((u4Data >> kk) & 1)
                				flag <<= 2;
                			errcode |= flag << (4 * kk);	                					
#elif CC_CALIB_DEBUG==5 
                    	flag = 1;
                    	if((u4Addr + u4DQSth) & EDGE_ADDRESS_ALIGN)
                    		flag <<= 2;
											errcode |= flag << (4 * kk);
#elif CC_CALIB_DEBUG==4 
                    	flag = 1;
											errcode |= flag << (4 * kk);
#endif
                		}
                		errval >>= 1;
                	}

                }
                
        #else
            	
                if ((MEM_READ32(u4Addr) ^ u4Val) & u4Mask)
                {
                    return TRUE;
                }
				#endif                
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
#if CC_CALIB_DEBUG>=4 && CC_CALIB_DEBUG<=7    
	if(errcode == 0)
    return FALSE;
  else
  	return errcode;  
#else
	return FALSE;
#endif  	
}

#if CC_CALIB_DEBUG>=4 && CC_CALIB_DEBUG<=7
static void _DdrFullScanManualDqsWindow(UINT32 u4ChipNum)
{
    UINT32 i, u4Val;
    INT32 i4Min, ib;

    UNUSED(u4Val);

    for (i = 0; i < (2 * u4ChipNum); i++)
    {
        LOG(1, "Scanning Byte %d DQS window ...\n", i);
        #if CC_CALIB_DEBUG==5
        LOG(1, "(test DQS edge)");
        for (ib=8; ib--; ) LOG(1, " FR");
        LOG(1, "\n");
        #elif CC_CALIB_DEBUG==6
        LOG(1, "(expected data)");
        for (ib=8; ib--; ) LOG(1, " 10");
        LOG(1, "\n");
        #elif CC_CALIB_DEBUG==7
        LOG(1, "(expected data)");
        for (ib=8; ib--; ) LOG(1, " 1100");
        LOG(1, "\n");
        LOG(1, "(test DQS edge)");
        for (ib=8; ib--; ) LOG(1, " FRFR");
        LOG(1, "\n");
        #endif

        for (i4Min = 10; i4Min >0; i4Min--)
        {
            LOG(1, "DQS delay = %3d", -i4Min);
            _DDR_SetDQ(i, i4Min);
    #if CC_ONLY_SUPPORT_16BIT
            _DDR_SetDQ(i ^ 2, i4Min);
    #endif
            u4Val = _DdrIsDQSFail(i, 0);

            for (ib=32; ib>0; )
            {
        #if (1 << PW_ITEM_BITS)==4
                ib-=4;
                LOG(1, " ");
                LOG(1, "%X", (u4Val>>ib)&15);
        #elif (1 << PW_ITEM_BITS)==2
                ib-=2;
                if ((ib&3)==2) LOG(1, " ");
                LOG(1, "%X", (u4Val>>ib)&3);
        #elif (1 << PW_ITEM_BITS)==1
                ib-=1;
                if ((ib&3)==3) LOG(1, " ");
                LOG(1, "%X", (u4Val>>ib)&1);
        #endif
            }
            LOG(1, "\n");
        }

        //now i4Min == 0
        _DDR_SetDQ(i, i4Min);
    #if CC_ONLY_SUPPORT_16BIT
        _DDR_SetDQ(i ^ 2, i4Min);
    #endif

        for (i4Min = 0; i4Min <64; i4Min++)
        {
            LOG(1, "DQS delay = %3d", i4Min);
            u4Val = _DdrIsDQSFail(i, i4Min);
            for (ib=32; ib>0; ) {
        #if (1 << PW_ITEM_BITS)==4
                ib-=4;
                LOG(1, " ");
                LOG(1, "%X", (u4Val>>ib)&15);
        #elif (1 << PW_ITEM_BITS)==2
                ib-=2;
                if ((ib&3)==2) LOG(1, " ");
                LOG(1, "%X", (u4Val>>ib)&3);
        #elif (1 << PW_ITEM_BITS)==1
                ib-=1;
                if ((ib&3)==3) LOG(1, " ");
                LOG(1, "%X", (u4Val>>ib)&1);
        #endif
                //if ((ib&3)==3) LOG(1, " ");
                //LOG(1, "%X", (u4Val>>ib)&1);
            }
            LOG(1, "\n");
        }
        LOG(1, "\n");
    }

}
#endif //CC_CALIB_DEBUG>=4 && CC_CALIB_DEBUG<=7

static UINT32 _DdrCalibrateDqs(INT32 byterange[])
{
#if (DEFAULT_DRAM_TYPE==DDR_III_x1) || (DEFAULT_DRAM_TYPE==DDR_III_x2)
    #define BYTE_PER_CHIP   4
#else
    #define BYTE_PER_CHIP   2
#endif

#if DEFAULT_DRAM_TYPE==DDR_II_x2
    #define CHIP_NUM        2
#else
    #define CHIP_NUM        1
#endif

    UINT32 i, u4DQS = 0, u4Val;
    UINT32 au4DQS[BYTE_PER_CHIP * CHIP_NUM] = {0,};
    INT32 i4Min = 0, i4Max = 0, i4Avg = 0, i4MinL, i4MaxL;

#if CC_CALIB_DEBUG>=4 && CC_CALIB_DEBUG<=7
    INT8 logmem_lb[8 << PW_ITEM_NUM];
    INT8 logmem_ub[8 << PW_ITEM_NUM];
    INT32 i4MinMin, i4MaxMax;
    INT32 i4AlreadyFailedItem;
#endif //CC_CALIB_DEBUG>=4 && CC_CALIB_DEBUG<=7

#if CC_RECORD_DQS
    UINT32 u4Rec = 0;
#endif

    // Set DQS input delay to manual mode
    u4Val = DRAM_READ32(0x28);
    DRAM_WRITE32(0x28, u4Val | (1U << 28));
#ifdef CC_5392B_PRELOADER
    CHIP_Delay1_us(10);
#else
    WAIT_DRAM_STABLE();
#endif

//#if CC_CALIB_DEBUG>=4 && CC_CALIB_DEBUG<=7
//  _DdrFullScanManualDqsWindow(CHIP_NUM);
//#endif //CC_CALIB_DEBUG>=4 && CC_CALIB_DEBUG<=7

    for (i = 0; i < (BYTE_PER_CHIP * CHIP_NUM); i++)
    {
        {
#if CC_MANUAL_DQS_MODE
            i4Avg = MANUAL_DQS;
            i4Avg = (INT8)((i4Avg >> (i * 8)) & 0xFF);
#else //CC_MANUAL_DQS_MODE

    #if CC_CALIB_DEBUG>=4 && CC_CALIB_DEBUG<=7
            for (i4Avg = 0; i4Avg < NUM_ELE(logmem_lb); i4Avg++)
            {
                logmem_lb[i4Avg] = -MAX_DQ_DELAY;
                logmem_ub[i4Avg] = -128;
            }
            i4AlreadyFailedItem = 0;
    #endif //CC_CALIB_DEBUG>=4 && CC_CALIB_DEBUG<=7

            // Using DQ to align DQ&DQS first
            
            for (i4Min = 0; i4Min < MAX_DQ_DELAY; i4Min += CALIB_STEP)
            {
                //LOG(1, "Align DQ, try i4Min = %d\n", i4Min);
                _DDR_SetDQ(i, i4Min);

    #if CC_ONLY_SUPPORT_16BIT
                _DDR_SetDQ(i ^ 2, i4Min);
    #endif

    #if CC_CALIB_DEBUG>=4 && CC_CALIB_DEBUG<=7
    
                u4Val = _DdrIsDQSFail(i, 0);
                
                for (i4Avg = 0; i4Avg < NUM_ELE(logmem_lb); i4Avg++)
                {
                    if (logmem_lb[i4Avg] == -MAX_DQ_DELAY &&
                        ((u4Val >> (i4Avg << PW_ITEM_BITS) ) &
                         ((1 << (1 << PW_ITEM_BITS)) -1)))
                    {
                        logmem_lb[i4Avg] = i4Min;
                        i4AlreadyFailedItem |= (1<<i4Avg);
                    }
                }
        #if (1 << PW_ITEM_BITS)==4
								if (i4AlreadyFailedItem == 0xff)
        #elif (1 << PW_ITEM_BITS)==2
								if (i4AlreadyFailedItem == 0xffff)
        #elif (1 << PW_ITEM_BITS)==1
								if (i4AlreadyFailedItem == 0xffffffff)                	
        #endif
    #else //CC_CALIB_DEBUG>=4 && CC_CALIB_DEBUG<=7
                if (_DdrIsDQSFail(i, 0))
    #endif //CC_CALIB_DEBUG>=4 && CC_CALIB_DEBUG<=7
                {
#if CALIB_STEP > 1                	
                    if (i4Min == 0)
                    {
                        break;
                    }
                    i4MinL = i4Min - CALIB_STEP;
                    for (; i4Min > i4MinL; i4Min--)
                    {
//                        LOG(1, "Align DQ, try i4Min = %d\n", i4Min);
                        _DDR_SetDQ(i, i4Min);
                        if (!_DdrIsDQSFail(i, 0))
                        {
                            break;
                        }
                    }
                    if(i4Min == 0)
										{
											goto HIGH_BOUND_STAGE;
										} 
#endif              
										     
                    break;
                }
            }

            //LOG(1, "Align DQ, i4Min = %d\n", i4Min);

            _DDR_SetDQ(i, 0);

    #if CC_ONLY_SUPPORT_16BIT
            _DDR_SetDQ(i ^ 2, 0);
    #endif

    #if CC_CALIB_DEBUG>=4 && CC_CALIB_DEBUG<=7
            i4MinMin = -(i4Min - 1);
            //change logmem_lb, to log OK DQ_delay_value, a minus number.
            //search max DQ_delay_value
            i4Min = -MAX_DQ_DELAY;
            for (i4Avg = 0; i4Avg < NUM_ELE(logmem_lb); i4Avg++)
            {
                logmem_lb[i4Avg] = -(logmem_lb[i4Avg] - 1);
                
                if (logmem_lb[i4Avg] > i4Min)
                {
                    i4Min = logmem_lb[i4Avg];
                }
            }
    #else //CC_CALIB_DEBUG>=4 && CC_CALIB_DEBUG<=7
            //may check first whether i4Min==MAX_DQ_DELAY or not, then..
            	i4Min = -(i4Min - 1);
    #endif //CC_CALIB_DEBUG>=4 && CC_CALIB_DEBUG<=7

            //for iMin/iMinMin, 1 means any possible positive DQS delay...
            if (i4Min >= 1) //iMinMin may be also 1, or not.
            {
                //LOG(1,"expected i4Min=1, now i4Min=%d\n.", i4Min);
                for (i4Min = 1; i4Min < MAX_DQS_DELAY; i4Min += CALIB_STEP)
                {
    #if CC_CALIB_DEBUG>=4 && CC_CALIB_DEBUG<=7
                    u4Val=_DdrIsDQSFail(i, i4Min);
                    //LOG(1,"B%dDQS=%3d: %08x ", i, i4Min, u4Val);
                    for (i4Avg = 0; i4Avg < NUM_ELE(logmem_lb); i4Avg++)
                    {
                        if ((u4Val >> (i4Avg << PW_ITEM_BITS)) &
                            ((1 << (1 << PW_ITEM_BITS)) - 1))
                        {
                            logmem_lb[i4Avg] = i4Min + 1;
                        }
                    }

                    //if all fail, set i4MinMin=i4Min+1
        #if (1 << PW_ITEM_BITS)==4
										if (u4Val == 0xff)                    
                    {
                        i4MinMin = i4Min + 1;
                    }
        #elif (1 << PW_ITEM_BITS)==2
										if (u4Val == 0xffff)
                    {
                        i4MinMin = i4Min + 1;
                    }
        #elif (1 << PW_ITEM_BITS)==1
                    if (u4Val == 0xffffffff)
                    {
                        i4MinMin = i4Min + 1;
                    }
        #endif //(1 << PW_ITEM_BITS)
                    if (u4Val == 0) //none nibbles failed, then...
    #else //CC_CALIB_DEBUG>=4 && CC_CALIB_DEBUG<=7
                    //LOG(1, "Try i4Min = %d\n", i4Min);
                    if (!_DdrIsDQSFail(i, i4Min))
    #endif //CC_CALIB_DEBUG>=4 && CC_CALIB_DEBUG<=7
                    {
#if CALIB_STEP > 1                    	
                        i4MinL = i4Min - CALIB_STEP;
                        for (; i4Min > i4MinL; i4Min--)
                        {
                        
                        	if(i4Min == 0)
                        	{	
                        		break;//(1)
                        	}
                            //LOG(1, "Try i4Min = %d\n", i4Min);
                            if (_DdrIsDQSFail(i, i4Min))
                            {
                                break;//(2)
                            }
                        }
                      	i4Min++;//if i4Min == 0 here, it must be fail
#endif                        
                        break;
                    }
                }
            }

            //LOG(1, "Found i4Min, i4Min = %d\n", i4Min);

            // Cannot find ok DQS, return failed.
            if (i4Min >= MAX_DQS_DELAY)
            {
                return ((i << 8) | 1);
            }
HIGH_BOUND_STAGE:
            // Find maximal DQS value
            i4Max = i4Min + DDR_CALIB_JUMP;
            i4Max = i4Max < 0 ? 0 : i4Max;
    #if CC_CALIB_DEBUG>=4 && CC_CALIB_DEBUG<=7
            i4AlreadyFailedItem = 0;
		#endif            
            for (; i4Max < MAX_DQS_DELAY; i4Max += CALIB_STEP)
            {
    #if CC_CALIB_DEBUG>=4 && CC_CALIB_DEBUG<=7
                u4Val = _DdrIsDQSFail(i, i4Max);
                //if(i == 1) LOG(1,"B%dDQS=%3d: %08x ", i, i4Max, u4Val);
                
                for (i4Avg = 0; i4Avg < NUM_ELE(logmem_ub); i4Avg++)
                {
                    if ((logmem_ub[i4Avg] == -128) &&
                        ((u4Val >> (i4Avg << PW_ITEM_BITS)) &
                         ((1 << (1 << PW_ITEM_BITS)) - 1)))
                    {
                        logmem_ub[i4Avg] = i4Max;
                        i4AlreadyFailedItem |= (1<<i4Avg);
                    }
                }

        #if (1 << PW_ITEM_BITS)==4
        				if (i4AlreadyFailedItem == 0xff) 
        #elif (1 << PW_ITEM_BITS)==2
        				if (i4AlreadyFailedItem == 0xffff)         
        #elif (1 << PW_ITEM_BITS)==1
                if (i4AlreadyFailedItem == 0xffffffff) 
        #endif //(1 << PW_ITEM_BITS)
    #else //CC_CALIB_DEBUG>=4 && CC_CALIB_DEBUG<=7
                if (_DdrIsDQSFail(i, i4Max))
    #endif //CC_CALIB_DEBUG>=4 && CC_CALIB_DEBUG<=7
                {
#if CALIB_STEP > 1                	
                    i4MaxL = i4Max - CALIB_STEP;
                    for(; i4Max > i4MaxL; i4Max --)
                    {
                        if (!_DdrIsDQSFail(i, i4Max))
                        {
                            break;
                        }
                    }
#endif                    
                    break;
                }
            }

            //?????? range check of i4Max ?????????
            //??????? is it required to program DQS delay back to zero now ?????
    #if CC_CALIB_DEBUG>=4 && CC_CALIB_DEBUG<=7
            i4MaxMax = i4Max - 1;
            //change logmem_ub, to log OK DQS_delay_value.
            //search min DQS_delay_value
            i4Max = 127;
            for (i4Avg = 0; i4Avg < NUM_ELE(logmem_ub); i4Avg++)
            {
                if (logmem_ub[i4Avg] == -128)
                {
                    logmem_ub[i4Avg]=127;
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
            for (u4Val = 0; u4Val < NUM_ELE(logmem_lb); u4Val++)
            {
                LOG(1,
        #if CC_CALIB_DEBUG==4
                    "bit%2d (%2d ~ %2d) ", (i << 3) | u4Val,
        #elif CC_CALIB_DEBUG==5
                    "bit%2d %s (%2d ~ %2d) ", (i << 3) | (u4Val >> 1),
                    (u4Val & 1) ? "F" : "R",
        #elif CC_CALIB_DEBUG==6
                    "bit%2d %d (%2d ~ %2d) ", (i << 3) | (u4Val >> 1),
                    u4Val & 1,
        #elif CC_CALIB_DEBUG==7
                    "bit%2d %s %d (%2d ~ %2d) ", (i << 3) | (u4Val >> 2),
                    (u4Val & 1) ? "F" : "R", (u4Val & 2) >> 1,
        #endif //CC_CALIB_DEBUG
                    logmem_lb[u4Val], logmem_ub[u4Val]);
								if (logmem_lb[u4Val] > -10)
									LOG(1," ");
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
                LOG(1,"\n");
            }
            LOG(1,"    %s%s            ", (CC_CALIB_DEBUG & 1) ? "  " : "",
                (CC_CALIB_DEBUG & 2) ? "  " : "");
						//if (i4MinMin <= -10)
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
            LOG(1,"\niMinMin=%d, iMin=%d, iMax=%d, iMaxMax=%d\n",
                i4MinMin, i4Min, i4Max, i4MaxMax);
    #else //CC_CALIB_DEBUG>=4 && CC_CALIB_DEBUG<=7
    	#if CALIB_STEP == 1
            i4Max--;
      #endif      
    #endif //CC_CALIB_DEBUG>=4 && CC_CALIB_DEBUG<=7

            i4Avg = (i4Min + i4Max) / 2;
            //ASSERT((i4Max - i4Min) >= 10);

#endif //CC_MANUAL_DQS_MODE

            u4DQS = DRAM_READ32(0x3DC + (4 * (i >> 2)));
            u4DQS = (u4DQS >> (8 * (i & 3))) & 0x7f; 
        }

        if (i4Avg >= 0)
        {
            _DDR_SetDQ(i, 0);
            _DDR_SetDQS(i, (UINT32)i4Avg);
        }
        else
        {
            _DDR_SetDQ(i, -i4Avg);
            _DDR_SetDQS(i, 0);
        }
#if CC_ONLY_SUPPORT_16BIT
        if (i4Avg >= 0)
        {
            _DDR_SetDQ(i ^ 2, 0);
            _DDR_SetDQS(i ^ 2, (UINT32)i4Avg);
        }
        else
        {
            _DDR_SetDQ(i ^ 2, -i4Avg);
            _DDR_SetDQS(i ^ 2, 0);
        }
#endif
        //??????? is it required to program DQS delay back to zero now ?????

        au4DQS[i] = ((UINT32)i4Avg - u4DQS) & 0xff;

        LINT_SUPPRESS_NEXT_EXPRESSION(774);
#if CC_MANUAL_DQS_MODE
        LOG(1, "Byte %d : %d\n", i, i4Avg);
#else
        LOG(1, "Byte %d : %d ~ %d, Size %d, Set %d\n", i, i4Min, i4Max,
            (i4Max - i4Min + 1), i4Avg);
            
	#if (DEFAULT_DRAM_TYPE==DDR_III_x2)     
		byterange[i*2] = i4Min;
		byterange[i*2+1] = i4Max;
	#endif
	
#endif //CC_MANUAL_DQS_MODE

#if CC_RECORD_DQS
        u4Rec = (((UINT32)i4Min) << 24) | (((UINT32)i4Max) << 16) |
                (((UINT32)i4Avg) << 8);
        HAL_WRITE32(RECORD_DQS_REG + (i * 4), u4Rec);
#endif //CC_RECORD_DQS

#if defined(CC_5391_PRELOADER) && (defined(CC_NAND_BOOT) || defined(CC_ROM_BOOT))
    #if CC_MANUAL_DQS_MODE
        
	CHIP_DisplayString("Byte ");
	CHIP_DisplayInteger(i);
	CHIP_DisplayString(" : DQS(");
	CHIP_DisplayInteger(i4Avg);
	CHIP_DisplayString(")\n");
	
    #else //CC_MANUAL_DQS_MODE
        print_dqs(i, i4Min, i4Max, i4Avg);
    #endif //CC_MANUAL_DQS_MODE
#endif //CC_5391_PRELOADER

    }
#if 0
    // auto mode, DQS counter
    u4DQS = DRAM_READ32(0x3FC);
    u4DQS = u4DQS & 0x7f; // counter (Gray code) 7bits
    LINT_SUPPRESS_NEXT_EXPRESSION(774);
    LOG(5, "Final DQS Count is 0x%08x\n", u4DQS);
#endif
    // Use the value of 0x18 and 0x1c to substrate u4Val and write to 0x20 and
    // 0x24.

#if 1		//20080526
	
    for (i = 0; i < (BYTE_PER_CHIP * CHIP_NUM); i++) //
    {
        u4Val = DRAM_READ32((0x20 + (4 * (i >> 2))));
        u4Val &= ~(0xffU << (8 * (i & 3)));
        u4Val |= (au4DQS[i] << (8 * (i & 3)));
        DRAM_WRITE32((0x20 + (4 * (i >> 2))), u4Val);
    }
#else

    for (i = 0; i < (BYTE_PER_CHIP * CHIP_NUM); i++)
//    for (i = 0; i < 4; i++)
    {
//		au4DQS[i] = au4DQS[i] ; //+ 1; //dtv low-temperature
		if((au4DQS[i]&0x80)== 0)
		{
			u4Val = DRAM_READ32((0x20 + (4 * (i >> 2))));
        	u4Val &= ~(0xffU << (8 * (i & 3)));
        	u4Val |= (au4DQS[i] << (8 * (i & 3)));
        	DRAM_WRITE32((0x20 + (4 * (i >> 2))), u4Val);
//        	Printf("+%d: %x\n", i, au4DQS[i]);
		}
		else
		{
			u4Val = DRAM_READ32((0x90 + (4 * (i >> 2))));
        	u4Val &= ~(0x7fU << (8 * (i & 3)));        	
        	u4Val |= (BIN_2_GREY((-au4DQS[i])&0x7f) << (8 * (i & 3)));  
        	DRAM_WRITE32((0x90 + (4 * (i >> 2))), u4Val);
//        	Printf("-%d: %x, 0x90=%x\n", i, au4DQS[i], u4Val);        	
		}		
    }

#endif



#if CC_USE_AUTO_MODE
    // Back to auto mode
    u4Val = DRAM_READ32(0x28);
    u4Val &= ~(1U << 28); // clear bit 28
    DRAM_WRITE32(0x28, u4Val);
#endif //CC_USE_AUTO_MODE

    return 0;
}

#if (DEFAULT_DRAM_TYPE==DDR_II_x1) || (DEFAULT_DRAM_TYPE==DDR_II_x2)
static void _DdrSetDqsIen(UINT32 u4Val)
{
    u4Val = BIN_2_GREY(u4Val & 0xFF) ;
    u4Val = (u4Val << 24) | (u4Val << 16) | (u4Val << 8) | u4Val;
    DRAM_WRITE32(0x94, DDR_DQSIEN_DMCKSEL | u4Val); // Need to do early
}
#endif

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** DDR_CalibrateDqs() to calculate and set the DQS offset.
 *  @param u4ChipNum the chip number of dram, only can be 1, 2, and 4.
 *  @retval 0 successfully calibrate, otherwise failure
 *            [15:8] - failed offset, [7:0] - error code.
 */
//-----------------------------------------------------------------------------
UINT32 DDR_CalibrateDqs(void)
{

#if (DEFAULT_DRAM_TYPE==DDR_III_x1)
	INT32 byterange_rk0[8];
	UINT32 u4Ret = 1;
	
	
	//DQS gating window scan moved to chip_entry1.   DDR_SetDramController();
	u4Ret = _DdrCalibrateDqs(byterange_rk0);
	return u4Ret;
	
#endif

#if (DEFAULT_DRAM_TYPE==DDR_III_x2)
	INT32 byterange_rk0[8];
	UINT32 u4Ret = 1;
	//DQS gating window scan moved to chip_entry1.   DDR_SetDramController();
	u4Ret = _DdrCalibrateDqs(byterange_rk0);

	#if (CC_MANUAL_DQS_MODE==0)
	INT32 byterange_rk1[8], au4DQS;
	UINT32 i, u4DQS, i4Avg, u4Val;
	UINT32 reg04, reg08, reg110, reg3dc;

	reg04 = DRAM_READ32(0x04);
	reg08 = DRAM_READ32(0x08);
	reg110 = DRAM_READ32(0x110);

	DRAM_WRITE32(0x08, reg08&0xFFFFFF00);
	DRAM_WRITE32(0x110, reg110^0x08);
	DRAM_WRITE32(0x04, 0x00010000);
	CHIP_Delay1_us(10);// WAIT_DRAM_STABLE(); will not worked in gcc -O2

	DRAM_WRITE32(0x08, reg08);
	DRAM_WRITE32(0x04, reg04);

	u4Ret = u4Ret | _DdrCalibrateDqs(byterange_rk1);

	//restore
	DRAM_WRITE32(0x08, reg08&0xFFFFFF00);
	DRAM_WRITE32(0x110, (reg110&0x0FFFFFFF)|0xc0000000 );
	DRAM_WRITE32(0x04, 0x00010000);
	CHIP_Delay1_us(10);// WAIT_DRAM_STABLE(); will not worked in gcc -O2

	DRAM_WRITE32(0x08, reg08);
	DRAM_WRITE32(0x04, reg04);

	reg3dc = DRAM_READ32(0x3DC);
    LOG(1, "0x3dc = %x\n", reg3dc);

	for(i=0; i<4; i++)
	{
#if 1
        u4DQS = (reg3dc >> (8 * (i & 3))) & 0x7f; 

		i4Avg = (byterange_rk0[2*i] + byterange_rk0[2*i+1])/2;
        au4DQS = ((UINT32)i4Avg - u4DQS) & 0xff;
		u4Val = DRAM_READ32(0x20);
       	u4Val &= ~(0xffU << (8 * (i & 3)));
       	u4Val |= (au4DQS << (8 * (i & 3)));
       	DRAM_WRITE32(0x20, u4Val);

		i4Avg = (byterange_rk1[2*i] + byterange_rk1[2*i+1])/2;
        au4DQS = ((UINT32)i4Avg - u4DQS) & 0xff;
		u4Val = DRAM_READ32(0x24);
       	u4Val &= ~(0xffU << (8 * (i & 3)));
       	u4Val |= (au4DQS << (8 * (i & 3)));
       	DRAM_WRITE32(0x24, u4Val);



#else
		byterange_rk0[2*i] = (byterange_rk0[2*i] > byterange_rk1[2*i]) ? byterange_rk0[2*i] : byterange_rk1[2*i];
		byterange_rk0[2*i+1] = (byterange_rk0[2*i+1] < byterange_rk1[2*i+1]) ? byterange_rk0[2*i+1] : byterange_rk1[2*i+1];
		i4Avg = (byterange_rk0[2*i] + byterange_rk0[2*i+1])/2;
	
        LOG(1, "Byte %d : %d ~ %d, Size %d, Set %d\n", i, byterange_rk0[2*i], byterange_rk0[2*i+1],
            (byterange_rk0[2*i+1] - byterange_rk0[2*i] + 1), i4Avg);

		u4DQS = DRAM_READ32(0x3DC);
        u4DQS = (u4DQS >> (8 * (i & 3))) & 0x7f; 

        au4DQS = ((UINT32)i4Avg - u4DQS) & 0xff;

		if((au4DQS&0x80)== 0)
		{
			u4Val = DRAM_READ32((0x20 + (4 * (i >> 2))));
        	u4Val &= ~(0xffU << (8 * (i & 3)));
        	u4Val |= (au4DQS << (8 * (i & 3)));
        	DRAM_WRITE32((0x20 + (4 * (i >> 2))), u4Val);
		}
		else
		{
			u4Val = DRAM_READ32((0x90 + (4 * (i >> 2))));
        	u4Val &= ~(0x7fU << (8 * (i & 3)));        	
        	u4Val |= (BIN_2_GREY((-au4DQS)&0x7f) << (8 * (i & 3)));  
        	DRAM_WRITE32((0x90 + (4 * (i >> 2))), u4Val);
		}		
#endif

		
	}


	#endif
	
	return u4Ret;
	
#endif


#if (DEFAULT_DRAM_TYPE==DDR_II_x1 || DEFAULT_DRAM_TYPE==DDR_II_x2)
	INT32 byterange_rk0[8];
	UINT32 u4Ret = 1;
	
	
	//DQS gating window scan moved to chip_entry1.   DDR_SetDramController();
	u4Ret = _DdrCalibrateDqs(byterange_rk0);
	return u4Ret;
#endif



#if 0//(DEFAULT_DRAM_TYPE==DDR_II_x1 || DEFAULT_DRAM_TYPE==DDR_II_x2)
	INT32 byterange_rk0[8];
	UINT32 i = 0, u4Ret = 1;
	UINT32 reg, reg1;
	
	for (i = 0; i < 5; i++)
	{
		_DdrSetDqsIen(u4DqsIen[i]);
		u4Ret = _DdrCalibrateDqs(byterange_rk0);
		if (u4Ret == 0)
		{
			break;
		}
		//Reset gating window delay for next value
		DRAM_WRITE32(0xF0, 0x11FFFFFF);
		DRAM_WRITE32(0xF4, 0x83FFFFFF);
		DRAM_WRITE32(0xF0, 0x01FFFFFF);
		DRAM_WRITE32(0xF4, 0x81FFFFFF);
/*
		//INIT DDR
		reg = DRAM_READ32(0x08);
		reg1 = DRAM_READ32(0x04);
		DRAM_WRITE32(0x08, reg&0xFFFFFF00);
		
		DRAM_WRITE32(0x04, 0x00010000);
		WAIT_DRAM_STABLE();

		DRAM_WRITE32(0x04, reg1);
		DRAM_WRITE32(0x08, reg);
*/
	}
	return u4Ret;

#endif	//DEFAULT_DRAM_TYPE==DDR_III_x1
}


#endif //CC_CALIB_VER


