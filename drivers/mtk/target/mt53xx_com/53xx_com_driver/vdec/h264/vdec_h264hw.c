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
 *-----------------------------------------------------------------------------
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: vdec_h264hw.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_h264hw.c
 *  This file contains implementation of exported APIs of VDEC.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
#include "vdec_h264hw.h"
#include "vdec_h264util.h"
LINT_EXT_HEADER_BEGIN
#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "x_common.h"
#include "x_typedef.h"
LINT_EXT_HEADER_END


static BOOL _bVdecLogSimPat = FALSE;
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static UINT8 _u1BarrelShifterNum;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static BOOL _fgIsVLDFetchOk(void);
static BOOL _fgIsVLDFetchOk2(void);

static BOOL _fgWaitVldFetchOk(void);
static BOOL _fgWaitVldFetchOk2(void);
static UINT32 _AVCVLDGetBitS(UINT32 u4ShiftBit);//, BYTE bBarrelNum)

static UINT32 _dVLDSramRp(void);
static UINT32 _dVLDSramWp(void);
static UINT32 _dVLDDramRp(void);
/*static UINT32 _dVLDDramWp(void);
static UINT32 _dVLDSum(void);
*/

// *********************************************************************
// Function : BOOL fgIsVLDFetchOk(void)
// Description : Check if VLD fetch is done
// Parameter : None
// Return    : TRUE: VLD fetch OK, FALSE: not OK
// *********************************************************************
static BOOL _fgIsVLDFetchOk(void)
{
    if((VLD_READ32(RO_VLD_FETCHOK) & VLD_FETCH_OK) == 0)
    {
        return FALSE;
    }
    return TRUE;
}

// *********************************************************************
// Function : BOOL fgIsVLDFetchOk2(void)
// Description : Check if VLD fetch is done
// Parameter : None
// Return    : TRUE: VLD fetch OK, FALSE: not OK
// *********************************************************************
static BOOL _fgIsVLDFetchOk2(void)
{
    if((VLD_READ32(RO_VLD_2ND_FETCHOK) & VLD_FETCH_OK) == 0)
    {
        return FALSE;
    }
    return TRUE;
}


// *********************************************************************
// Function : BOOL fgWaitVldFetchOk(void)
// Description :
// Parameter :
// Return    : TRUE: Init Barrel Shifter OK, FALSE: Time Out
// *********************************************************************
static BOOL _fgWaitVldFetchOk(void)
{
    UINT32 u4Cnt;

    if(!_fgIsVLDFetchOk())
    {
        u4Cnt = 0;
        while(!_fgIsVLDFetchOk())
        {
            u4Cnt++;
            if(u4Cnt >= WAIT_THRD)
            {
                return FALSE;
            }
        }
    }
    #ifdef WAIT_SRAM_STABLE
    u4Cnt = 0;
    while((VLD_READ32(RO_VLD_SRAMCTRL) & AA_FIT_TARGET_D)==0)
    {
        u4Cnt++;
        if(u4Cnt >= WAIT_THRD)
        {
            LOG(2, "//===SRAM !Stable 0xF4=0x%x, Rptr 0xB0=0x%x, 0xFC=0x%x, 0xEC=0x%x, 0xE8=0x%x\n", 
                VLD_READ32(RO_VLD_SRAMCTRL), VLD_READ32(RW_VLD_RPTR), VLD_READ32(RO_VLD_VRPTR),
                VLD_READ32(RO_VLD_VBAR), VLD_READ32(RO_VLD_FETCHOK));
            return FALSE;
        }    
    }
    //LOG(6, "//WaitFetchOK Rptr 0xB0=0x%x, 0xFC=0x%x, 0xEC=0x%x, 0xE8=0x%x\n", 
    //    VLD_READ32(RW_VLD_RPTR), VLD_READ32(RO_VLD_VRPTR),
    //    VLD_READ32(RO_VLD_VBAR), VLD_READ32(RO_VLD_FETCHOK));
    #endif
    
    return TRUE;
}

// *********************************************************************
// Function : BOOL fgWaitVldFetchOk(void)
// Description :
// Parameter :
// Return    : TRUE: Init Barrel Shifter OK, FALSE: Time Out
// *********************************************************************
static BOOL _fgWaitVldFetchOk2(void)
{
    UINT32 u4Cnt;

    if(!_fgIsVLDFetchOk2())
    {
        u4Cnt = 0;
        while(!_fgIsVLDFetchOk2())
        {
            u4Cnt++;
            if(u4Cnt >= WAIT_THRD)
            {
                return FALSE;
            }
        }
    }
    #ifdef WAIT_SRAM_STABLE
    u4Cnt = 0;
    while((VLD_READ32(RO_VLD_2ND_SRAMCTRL) & AA_FIT_TARGET_D)==0)
    {
        u4Cnt++;
        if(u4Cnt >= WAIT_THRD)
        {
            LOG(2, "//===2SRAM !Stable 0x4F4=0x%x, Rptr 0x4B0=0x%x, 0x4FC=0x%x, 0x4EC=0x%x, 0x4E8=0x%x\n", 
                VLD_READ32(RO_VLD_2ND_SRAMCTRL), VLD_READ32(RW_VLD_2ND_RPTR), VLD_READ32(RO_VLD_2ND_VRPTR),
                VLD_READ32(RO_VLD_2ND_VBAR), VLD_READ32(RO_VLD_2ND_FETCHOK));
            return FALSE;
        }    
    }
    //LOG(6, "//2WaitFetchOK Rptr 0x4B0=0x%x, 0x4FC=0x%x, 0x4EC=0x%x, 0x4E8=0x%x\n", 
    //    VLD_READ32(RW_VLD_2ND_RPTR), VLD_READ32(RO_VLD_2ND_VRPTR),
    //    VLD_READ32(RO_VLD_2ND_VBAR), VLD_READ32(RO_VLD_2ND_FETCHOK));
    #endif
    
    return TRUE;
}

// *********************************************************************
// Function : DWRD dAVCVLDGetBitS(DWRD dShiftBit)
// Description : Get Bitstream from VLD barrel shifter
// Parameter : dShiftBit: Bits to shift (0-32)
// Return    : barrel shifter
// *********************************************************************
static UINT32 _AVCVLDGetBitS(UINT32 u4ShiftBit)//, BYTE bBarrelNum)
{
    UINT32 u4RegVal, u4Rptr, u4Wptr;

    u4Rptr = VLD_READ32(RO_VLD_VRPTR);
    u4Wptr = VLD_READ32(RO_VLD_VWPTR);
    
    if(AVAILABLE_SZ(u4Rptr, u4Wptr) <= MIN_LEN)
    {
        LOG(3, "GetBitS Rptr==Wptr\n");
        x_thread_delay(1);
        return 0;
    }
    #ifdef BARREL2_SUPPORT  
    if(_u1BarrelShifterNum == BARREL2)
    {
        u4RegVal = AVC_VLD_READ32(RO_AVLD_2ND_BARL + (u4ShiftBit << 2));
    }
    else        
    #endif
    {
        u4RegVal = AVC_VLD_READ32(RO_AVLD_BARL + (u4ShiftBit << 2));
    }

    return u4RegVal;
}

// *********************************************************************
// Function : DWRD dVLDSramRp(void)
// Description : Get VLD SRAM Read Ptr
// Parameter : None
// Return    : VLD SRAM Read PTR
// *********************************************************************
static UINT32 _dVLDSramRp(void)
{
    #ifdef BARREL2_SUPPORT  
    if(_u1BarrelShifterNum == BARREL2)
    {
        return(VLD_READ32(RO_VLD_2ND_VBAR) & 0x3f);
    }  
    else        
    #endif
    {
        #ifdef IS_POST_MT5363
        UINT32 u4Reg = 0;
        u4Reg = VLD_READ32(RO_VLD_VBAR);
        return (((u4Reg & 0xf) * 4) + ((u4Reg >> 24) & 0x3));   
        
        #else
        return(VLD_READ32(RO_VLD_VBAR) & 0x3f);
        #endif
    }
}


// *********************************************************************
// Function : DWRD dVLDSramWp(void)
// Description : Get VLD SRAM Write Ptr
// Parameter : None
// Return    : VLD SRAM Write PTR
// *********************************************************************
static UINT32 _dVLDSramWp(void)
{
    #ifdef BARREL2_SUPPORT  
    if(_u1BarrelShifterNum == BARREL2)
    {
        return((VLD_READ32(RO_VLD_2ND_VBAR) >> 16) & 0x3f);
    }
    else        
    #endif
    {
        #ifdef IS_POST_MT5363
        UINT32 u4Reg = 0;
        u4Reg = VLD_READ32(RO_VLD_VBAR);
        return (((u4Reg >> 8) & 0xf) * 4);
        
        #else
        return((VLD_READ32(RO_VLD_VBAR) >> 16) & 0x3f);
        #endif
    }
}  


// *********************************************************************
// Function : DWRD dVLDDramRp(void)
// Description : Get VLD DRAM Read Ptr
// Parameter : None
// Return    : VLD DRAM Read PTR
// *********************************************************************
static UINT32 _dVLDDramRp(void)
{
    #ifdef BARREL2_SUPPORT  
    if(_u1BarrelShifterNum == BARREL2)
    {
        return VLD_READ32(RO_VLD_2ND_VRPTR);// & 0xffffff);
    }
    else
    #endif
    {
        return VLD_READ32(RO_VLD_VRPTR);// & 0xffffff);
    }
}


// *********************************************************************
// Function : DWRD dVLDDramWp(void)
// Description : Get VLD DRAM Write Ptr from parser
// Parameter : None
// Return    : VLD DRAM Write PTR
// *********************************************************************
/*static UINT32 _dVLDDramWp(void)
{
  return VLD_READ32(RO_VLD_VWPTR);
}  

// *********************************************************************
// Function : DWRD dVLDSum(void)
// Description : Get VLD Sum for exact read pointer
// Parameter : None
// Return    : VLD Sum
// *********************************************************************
static UINT32 _dVLDSum(void)
{
  return VLD_READ32(RO_VLD_SUM);
}*/



//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

// *********************************************************************
// Function : void vVLDHWReset(void)
// Description : Reset Video decode HW
// Parameter : None
// Return    : None
// *********************************************************************
void _VDEC_VLDHWReset(BOOL bIsInit)
{
    #ifdef WAIT_SRAM_STABLE
    UINT32 u4Cnt = 0;
    //if(!bIsInit)
    {
        while((VLD_READ32(RO_VLD_SRAMCTRL) & AA_FIT_TARGET_D)==0)
        {
            u4Cnt++;
            if(u4Cnt >= WAIT_THRD)
            {
                LOG(2, "WaitSramStable Fail\n");
                break;
            }
        }
    }
    #endif
    //to avoid reset when HW still access DRAM
    MC_WRITE32(RW_MC_PP_DRAM_ACCESS, ACCESS_DISABLE);

    VLD_WRITE32(WO_VLD_SRST, 1);
    VLD_WRITE32(WO_VLD_SRST, 0);
    AVC_VLD_WRITE32(RW_AVLD_TIMEOUT_THD, AVLD_TIMEOUT_THD);
    AVC_VLD_WRITE32(RW_AVLD_MC_BUSY_THD, MC_BUSY_THD);
    MC_WRITE32(RW_MC_PP_DRAM_ACCESS, ACCESS_ENABLE);
    //LOG(6, "//SRST Rptr 0xB0=0x%x, 0xFC=0x%x\n", VLD_READ32(RW_VLD_RPTR), VLD_READ32(RO_VLD_VRPTR));
    UNUSED(bIsInit);
}


// *********************************************************************
// Function : void vVLDHWReset(void)
// Description : Reset Video decode HW
// Parameter : None
// Return    : None
// *********************************************************************
void _VDEC_VLDHWReset2(BOOL bIsInit)
{
    #ifdef WAIT_SRAM_STABLE
    if(!bIsInit)
    {    
        while((VLD_READ32(RO_VLD_2ND_SRAMCTRL) & AA_FIT_TARGET_D)==0){}
    }
    #endif
    
    VLD_WRITE32(WO_VLD_SRST, (UINT32)(1<<8));
    VLD_WRITE32(WO_VLD_SRST, 0);

    //LOG(6, "//2SRST Rptr 0x4B0=0x%x, 0x4FC=0x%x\n", VLD_READ32(RW_VLD_2ND_RPTR), VLD_READ32(RO_VLD_2ND_VRPTR));    
}


// *********************************************************************
// Function : void vResetVLD1(DWRD dwRDPtrAddr)
// Description : Reset VLD1
// Parameter : u4RDPtrAddr, u4BsFifo, u4PredSA
// Return    : None
// *********************************************************************
BOOL _VDEC_ResetVLD1
    (UINT32 u4RDPtrAddr, UINT32 u4WPtrAddr,
    UINT32 u4BsFifoStart, UINT32 u4BsFifoEnd, 
    UINT32 u4PredSA, UINT8 u1ECCMode, UINT32 u4ECType)
{  
    UINT32 u4AvalSz;
    
    VLD_WRITE32(RW_VLD_RDY_SWTICH, (UINT32)READY_TO_RISC);

    if(u1ECCMode > 0)  //frame decoding
    {
    AVC_VLD_WRITE32(RW_AVLD_CTRL, (UINT32)(AVC_DEC_EN | AVC_RDY_WITH_CNT | 
        AVC_RDY_CNT_THD | AVLD_MEM_PAUSE_MOD_EN | 
        AVC_SUM6_APPEND_INV |AVC_NOT_CHK_DATA_VALID | AVC_RBSP_CHK_INV | AVC_READ_FLAG_CHK_INV |
        AVC_DEC_CYCLE_EN | 
        AVC_DEC_ERROR_SWITCH_ON | AVC_DEC_ERROR_ONLY1MC_START |
        AVC_RBSP_SWITCH |AVC_NON_SPEC_SWITCH)
        );
    }
    else                       //header parsing
    {
        AVC_VLD_WRITE32(RW_AVLD_CTRL, (UINT32)(AVC_DEC_EN | AVC_RDY_WITH_CNT | 
            AVC_RDY_CNT_THD | AVLD_MEM_PAUSE_MOD_EN | 
            AVC_SUM6_APPEND_INV |AVC_NOT_CHK_DATA_VALID | AVC_RBSP_CHK_INV | AVC_READ_FLAG_CHK_INV |
            AVC_DEC_CYCLE_EN | 
            AVC_DEC_ERROR_SWITCH_OFF | AVC_DEC_ERROR_ONLY1MC_START |
            AVC_RBSP_SWITCH |AVC_NON_SPEC_SWITCH)
            );
    }
    #ifdef MEM_PAUSE_SUPPORT
    AVC_VLD_WRITE32(RW_AVLD_EMU_PREV, (AVC_VLD_READ32(RW_AVLD_EMU_PREV) & 0xFFFFFFFC) | MEM_PAUSE_EN);
    #else
    AVC_VLD_WRITE32(RW_AVLD_EMU_PREV, (AVC_VLD_READ32(RW_AVLD_EMU_PREV) & 0xFFFFFFFC) | MEM_PAUSE_DISABLE);
    #endif
    
    #ifndef NON_SPEC_CTRL1_EN
    AVC_VLD_WRITE32(RW_AVLD_EMU_PREV, AVC_VLD_READ32(RW_AVLD_EMU_PREV) | NON_SPEC_SHIFT_EN);
    #endif

    #ifdef IS_POST_MT5363
    AVC_VLD_WRITE32(RW_AVLD_EMU_PREV, AVC_VLD_READ32(RW_AVLD_EMU_PREV) | NEIGHBOR_WRITE_DISABLE);
    #endif
    
    //error concealment
    #ifdef ERROR_CCM_EN
    if((u1ECCMode & 0x1) == 0)
    {
        AVC_VLD_WRITE32(RW_AVLD_ERC_CTRL, ERC_DISABLE);
    }
    else
    {
        AVC_VLD_WRITE32(RW_AVLD_ERC_CTRL, ((u1ECCMode & 0xC)<<6) | (u1ECCMode & 0x3));
        AVC_VLD_WRITE32(RW_AVLD_ERC_DED_ERR_TYPE, ((u4ECType & DISABLE_FIND_STARTCODE_ERR) /*& ERR_MASK*/));
    }
    #endif
    AVC_MV_WRITE32(RW_AMV_EC_SETTING, AVOID_HANG/* | SKIP_MODE3*/);
    AVC_VLD_WRITE32(RW_AVLD_ERR_MASK, ERR_MASK);

    
    MC_WRITE32(RW_MC_OPBUF, 6);
    
    // Set for VLD boundary check 0x5000000
    _VDEC_VLDSetVFifo(PHYSICAL(u4BsFifoStart), PHYSICAL(u4BsFifoEnd));
    AVC_VLD_WRITE32(RW_AVLD_PRED_ADDR, PHYSICAL(u4PredSA) | PRED_SRAM_USED);    //buffer for VLD, 32*1024    

    // Reset AVC VLD Sum
    //AVC_VLD_WRITE32(RW_AVLD_RESET_SUM, AVLD_RESET_SUM_ON);
    //AVC_VLD_WRITE32(RW_AVLD_RESET_SUM, AVLD_RESET_SUM_OFF);  

    if(u4WPtrAddr > u4RDPtrAddr)
    {
        u4AvalSz = u4WPtrAddr - u4RDPtrAddr;
    }
    else
    {
        u4AvalSz = (u4BsFifoEnd - u4RDPtrAddr) + (u4WPtrAddr - u4BsFifoStart);
    }
    if(u4AvalSz > ((u4BsFifoEnd - u4BsFifoStart) - (WPTR_OFFSET<<1)))
    {
        //LOG(2, "Fifo Data full\n");
        u4WPtrAddr -= WPTR_OFFSET;
    }
    // set write ptr    
    _VDEC_SetVldWPtr(u4WPtrAddr, u4BsFifoStart, u4BsFifoEnd);
    if(!_VDEC_VLDInitBarrelShifter(u4RDPtrAddr))
    {
        LOG(2, "Barrel Shifter Init failed\n");
        return FALSE;
    }
    return TRUE;
}

// *********************************************************************
// Function : void vResetVLD2(DWRD dwRDPtrAddr)
// Description : Reset VLD2
// Parameter : None
// Return    : None
// *********************************************************************
BOOL _VDEC_ResetVLD2(UINT32 u4RDPtrAddr, UINT32 u4WPtrAddr,
    UINT32 u4BsFifoStart, UINT32 u4BsFifoEnd)
{
    VLD_WRITE32(RW_VLD_2ND_RDY_SWTICH, (UINT32)READY_TO_RISC);
    AVC_VLD_WRITE32(RW_AVLD_2ND_BARL_CTRL, AVLD_2ND_BARL_EN);    
    _VDEC_VLDSetVFifo2(PHYSICAL(u4BsFifoStart), PHYSICAL(u4BsFifoEnd));

    /*u4Temp = 32 - (_VDEC_AVCVLDShiftBits() % 32);
    // at list trailing bit
    if(u4Temp)
    {
        u4Temp = _VDEC_AVCGetBitStream(u4Temp);
    }*/
        // set write ptr
      VLD_WRITE32(RW_VLD_2ND_WPTR, u4WPtrAddr);    //set wptr infinity

    if(!_VDEC_VLDInitBarrelShifter2(u4RDPtrAddr))
    {
        LOG(2, "Barrel Shifter 2 Init failed\n");
        return FALSE;
    }  
    return TRUE;
}


// *********************************************************************
// Function : vVLDSetVFifo(DWRD dSa, DWRD dEa)
// Description : Set VLD VFifo Sa, Ea
// Parameter : dSa: Absolute Dram Address of VFifo Start
//             dEa: Absolute Dram Address of VFifo End
// Return    : None
// *********************************************************************
void _VDEC_VLDSetVFifo(UINT32 u4Sa, UINT32 u4Ea)
{
    VLD_WRITE32(RW_VLD_VSTART, u4Sa >> 6);
    VLD_WRITE32(RW_VLD_VEND, u4Ea >> 6);
}


// *********************************************************************
// Function : vVLDSetVFifo(DWRD dSa, DWRD dEa)
// Description : Set VLD VFifo Sa, Ea
// Parameter : dSa: Absolute Dram Address of VFifo Start
//             dEa: Absolute Dram Address of VFifo End
// Return    : None
// *********************************************************************
void _VDEC_VLDSetVFifo2(UINT32 u4Sa, UINT32 u4Ea)
{
    VLD_WRITE32(RW_VLD_2ND_VSTART, u4Sa >> 6);
    VLD_WRITE32(RW_VLD_2ND_VEND, u4Ea >> 6);
}

// *********************************************************************
// Function : BOOL fgVLDInitBarrelShifter(DWRD dwPtr)
// Description : Init HW Barrel Shifter
// Parameter : dwPtr: Physical DRAM Start Address to fill Barrel Shifter
// Return    : TRUE: Initial Success, Fail: Initial Fail
// *********************************************************************
BOOL _VDEC_VLDInitBarrelShifter(UINT32 u4Ptr)
{
    UINT32 u4RPtr;
    UINT32 u4Cnt = 0;
    
    _u1BarrelShifterNum = BARREL1;

    VLD_WRITE32(RW_VLD_RPTR, u4Ptr);
    if(VLD_READ32(RO_VLD_VRPTR) != (u4Ptr - (u4Ptr % RPTR_ALIGN)))
    {
        LOG(2, "RPTR moved before INIFET\n");
        return FALSE;
    }
    
    // start to fetch data &  Reset AVLD_SUM
    AVC_VLD_WRITE32(RW_AVLD_RESET_SUM, AVLD_RESET_SUM_ON);
    
    VLD_WRITE32(RW_VLD_ASYNC, VLD_READ32(RW_VLD_ASYNC) | 0x101);
    VLD_WRITE32(RW_VLD_ASYNC, VLD_READ32(RW_VLD_ASYNC) |0x301);
    VLD_WRITE32(RW_VLD_ASYNC, VLD_READ32(RW_VLD_ASYNC) |0x101);

    VLD_WRITE32(RW_VLD_PROC, VLD_INIFET);

    if(!_fgWaitVldFetchOk())
    {
        LOG(2, "!HwRptr 0x%x, Rptr 0x%x\n", _VDEC_VldRPtr(), u4Ptr);
        return FALSE;
    }
    VLD_WRITE32(RW_VLD_PROC, VLD_INIBR);
    
    #ifdef MEM_PAUSE_SUPPORT

    #ifdef IS_POST_MT5363
    while((((VLD_READ32(RO_VLD_VBAR) & 0xf) * 4) + 
        ((VLD_READ32(RO_VLD_VBAR) >> 24) & 0x3)) !=5)
    #else
    while((VLD_READ32(RO_VLD_VBAR) & 0x3f) !=5)
    #endif
    {
            u4Cnt++;
            if(u4Cnt >= WAIT_THRD)
            {
                LOG(2, "FetchOK-WaitSramStable Fail\n");
                break;
            }    
    }
    #else

    #ifdef IS_POST_MT5363
    while((((VLD_READ32(RO_VLD_VBAR) & 0xf) * 4) + 
        ((VLD_READ32(RO_VLD_VBAR) >> 24) & 0x3)) !=4)    
    #else
    while((VLD_READ32(RO_VLD_VBAR) & 0x3f) !=4)
    #endif    
    {
            u4Cnt++;
            if(u4Cnt >= WAIT_THRD)
            {
                LOG(2, "FetchOK-WaitSramStable Fail\n");
                break;
            }      
    }
    #endif
    
    AVC_VLD_WRITE32(RW_AVLD_RESET_SUM, AVLD_RESET_SUM_OFF);  

    LOG(9, "//InWin 0x%x\n", VLD_READ32(RO_VLD_BARL));
    while(_VDEC_VldRPtr() < u4Ptr)
    {
        UNUSED(_VDEC_AVCGetBitStream(8));
    }

    if((u4RPtr = _VDEC_VldRPtr()) != u4Ptr)
    {
        LOG(2, "BarrelShifter init u4Ptr 0x%x failed 0x%x\n", u4Ptr, u4RPtr);
    }
    
    UNUSED(u4RPtr);
    return TRUE;
}


// *********************************************************************
// Function : BOOL fgVLDInitBarrelShifter2(DWRD dwPtr)
// Description : Init HW Barrel Shifter
// Parameter : dwPtr: Physical DRAM Start Address to fill Barrel Shifter
// Return    : TRUE: Initial Success, Fail: Initial Fail
// *********************************************************************
BOOL _VDEC_VLDInitBarrelShifter2(UINT32 u4Ptr)
{
    UINT32 u4Remainder;
    
    _u1BarrelShifterNum = BARREL2;

    u4Remainder = u4Ptr % 16; //VLD read ptr 16-byte aligned

    VLD_WRITE32(RW_VLD_2ND_RPTR, u4Ptr);
    if(VLD_READ32(RO_VLD_2ND_VRPTR) != (u4Ptr - u4Remainder))
    {
        LOG(2, "2RPTR moved before INIFET\n");
        //return FALSE;
    }
    // start to fetch data
    
    VLD_WRITE32(RW_VLD_2ND_ASYNC, 0x101);
    VLD_WRITE32(RW_VLD_2ND_ASYNC, 0x301);
    VLD_WRITE32(RW_VLD_2ND_ASYNC, 0x101);

    VLD_WRITE32(RW_VLD_2ND_PROC, VLD_INIFET);
    if(!_fgWaitVldFetchOk2())
    {
        return FALSE;
    }
    VLD_WRITE32(RW_VLD_2ND_PROC, VLD_INIBR);
    while(_VDEC_VldRPtr() < u4Ptr)
    {
        UNUSED(_VDEC_AVCGetBitStream(8));
    }  
    return TRUE;
}

// *********************************************************************
// Function : DWRD dwGetBitStream(DWRD dwShiftBit)
// Description : get 32 bits from shifter
// Parameter : bShiftBit: if 0, just get 32 bits from shifter
//                        others, 1. remove bShiftBit bits
//                                2. get 32 bits from shifter
// Return    : 32 bits value
// *********************************************************************
UINT32 _VDEC_AVCGetBitStream(UINT32 u4ShiftBit)
{
    UINT32 u4RegVal;

    //_dwBitCount += dwShiftBit;
    u4RegVal = _AVCVLDGetBitS(u4ShiftBit);
    return u4RegVal;
}

// *********************************************************************
// Function : DWRD dwGetBitStream(DWRD dwShiftBit)
// Description : get 32 bits from shifter
// Parameter : bShiftBit: if 0, just get 32 bits from shifter
//                        others, 1. get 32 bits from the current position
//                                2. remove bShiftBit bits
// Return    : 32 bits value
// *********************************************************************
UINT32 _VDEC_AVCGetShtBitStream(UINT32 u4ShiftBit)
{
    UINT32 u4RegVal1;

    //_dwBitCount += dwShiftBit;
    u4RegVal1 = _AVCVLDGetBitS(0);
    UNUSED(_AVCVLDGetBitS(u4ShiftBit));

    return(u4RegVal1);
}

// *********************************************************************
// Function : DWRD dwGetRealShtBitStream(DWRD dwShiftBit)
// Description : get dwShiftBit bits from shifter
// Parameter : bShiftBit: if 0, just get 32 bits from shifter
//                        others, 1. get 32 bits from the current position
//                                   2. remove bShiftBit bits
// Return    : 32 bits value
// *********************************************************************
UINT32 _VDEC_AVCGetRealShtBitStream(UINT32 u4ShiftBit)
{
    UINT32 u4RegVal1;

    //_dwBitCount += dwShiftBit;
    u4RegVal1 = _AVCVLDGetBitS(0);
    UNUSED(_AVCVLDGetBitS(u4ShiftBit));
  
    return(u4RegVal1 >> (32 - u4ShiftBit));
}


VOID _VDEC_AVCYC(VDEC_DISP_PIC_INFO_T* prDispInfo)
{   int i,j;  
    VDEC_DISP_PIC_INFO_T* prDispInfoForCrc= prDispInfo;
    for(i=0;i<4;i++)
    {
        prDispInfoForCrc->u4Ycrc[i] = IO_READ32(IO_VIRT + 0x3000,(2+i)*4);
    }
	
    for(j=0;j<4;j++)
    {
        prDispInfoForCrc->u4Ccrc[j] = IO_READ32(IO_VIRT + 0x3000,(6+j)*4);
    }

}

// *********************************************************************
// Function : BOOL fgGetBitStreamFlg(void)
// Description : get 1 bits from shifter
// Parameter : 1. remove bShiftBit bits
//                   2. get 1 bit from shifter
// Return    : 32 bits value
// *********************************************************************
BOOL _VDEC_AVCGetBitStreamFlg(void)
{
    UINT32 u4RegVal;

    u4RegVal = _VDEC_AVCGetShtBitStream(1);
    return((u4RegVal >> 31));
}

// *********************************************************************
// Function    : DWRD dwUeCodeNum(BYTE bBarrelNum)
// Description : Ue
// Parameter   : 
// Return      : 
// *********************************************************************
UINT32 _VDEC_UeCodeNum(void)
{
    UINT32 u4Rptr, u4Wptr;
    
    u4Rptr = VLD_READ32(RO_VLD_VRPTR);
    u4Wptr = VLD_READ32(RO_VLD_VWPTR);
    
    if(AVAILABLE_SZ(u4Rptr, u4Wptr) <= MIN_LEN)
    {
        LOG(3, "Ue Rptr==Wptr\n");
        x_thread_delay(1);
        return 0;
    }
    #ifdef BARREL2_SUPPORT  
    if(_u1BarrelShifterNum == BARREL2)
    {
        return AVC_VLD_READ32(RO_AVLD_2ND_UE);
    }
    else
    #endif
    {
        return AVC_VLD_READ32(RO_AVLD_UE);
    }
}

// *********************************************************************
// Function    : INT iSeCodeNum(BYTE bBarrelNum)
// Description : Ue
// Parameter   : 
// Return      : 
// *********************************************************************
INT32 _VDEC_SeCodeNum(void)
{
    UINT32 u4Rptr, u4Wptr;
    
    u4Rptr = VLD_READ32(RO_VLD_VRPTR);
    u4Wptr = VLD_READ32(RO_VLD_VWPTR);
    
    if(AVAILABLE_SZ(u4Rptr, u4Wptr) <= MIN_LEN)
    {
        LOG(3, "Se Rptr==Wptr\n");
        x_thread_delay(1);
        return 0;
    }
    #ifdef BARREL2_SUPPORT  
    if(_u1BarrelShifterNum == BARREL2)
    {
        return((INT32)AVC_VLD_READ32(RO_AVLD_2ND_SE));
    }  
    else        
    #endif
    {
        return((INT32)AVC_VLD_READ32(RO_AVLD_SE));
    }
}

// *********************************************************************
// Function    : DWRD vTrailingBits()
// Description : Ue
// Parameter   : 
// Return      : 
// *********************************************************************
void _VDEC_TrailingBits(void)
{
    UINT32 u4Temp;

    u4Temp = 8 - (_VDEC_AVCVLDShiftBits() % 8);
    // at list trailing bit
    if(u4Temp < 8)
    {
        u4Temp = _VDEC_AVCGetBitStream(u4Temp);
    }
}

// *********************************************************************
// Function    : DWRD vSetSPSAVLD()
// Description : Set SPS related registers
// Parameter   : 
// Return      : 
// *********************************************************************
void _VDEC_SetSPSAVLD(UINT32 u4SPSInfo, UINT32 u4Width, UINT32 u4Height)
{
    AVC_VLD_WRITE32(RW_AVLD_SPS, u4SPSInfo);
    AVC_VLD_WRITE32(RW_AVLD_PIC_SIZE, (u4Height << 8) | u4Width);  
}

// *********************************************************************
// Function    : DWRD vSetPPSAVLD()
// Description : Set SPS related registers
// Parameter   : 
// Return      : 
// *********************************************************************
void _VDEC_SetPPSAVLD(UINT32 u4PPSInfo1, UINT32 u4PPSInfo2)
{
  
    AVC_VLD_WRITE32(RW_AVLD_PPS_1 , u4PPSInfo1);  
    AVC_VLD_WRITE32(RW_AVLD_PPS_2 , u4PPSInfo2);
}

// *********************************************************************
// Function    : DWRD vSetPPSAVLD1()
// Description : Set SPS related registers
// Parameter   : 
// Return      : 
// *********************************************************************
void _VDEC_SetSHDRAVLD1(UINT32 u4SHDRInfo)
{
    AVC_VLD_WRITE32(RW_AVLD_SHDR_1, u4SHDRInfo);  
  }

// *********************************************************************
// Function    : DWRD vSetPPSAVLD2()
// Description : Set SPS related registers
// Parameter   : 
// Return      : 
// *********************************************************************
void _VDEC_SetSHDRAVLD2(UINT32 u4SHDRInfo)
{
    AVC_VLD_WRITE32(RW_AVLD_SHDR_2, u4SHDRInfo);
}

// *********************************************************************
// Function : void vMCGetMbXY(DWRD *pdX, DWRD *pdY)
// Description : MC Get Last Decoded Macro Block Position
// Parameter : pdX, pdY: Pointer to X, Y
// Return    : X, Y position for MB, store in pdX, pdY
// *********************************************************************
void _VDEC_MCGetMbXY(UINT32 *pdX, UINT32 *pdY)
{
    UINT32 u4Tmp;
    
    ASSERT(pdX!=NULL);
    ASSERT(pdY!=NULL);

    u4Tmp = AVC_VLD_READ32(RO_AVLD_SMKR);
    *pdX = u4Tmp>>24;
    *pdY = (u4Tmp<<8)>>24;
    //*pdX = AVC_MV_READ32(RO_MC_MBX);
    //*pdY = AVC_MV_READ32(RO_MC_MBY);
}


// *********************************************************************
// Function : void vWriteScalingList(DWRD dwIdx, dwData)
// Description : 
// Parameter : 
// Return    : 
// *********************************************************************
void _VDEC_WriteScalingList(UINT32 u4Idx, const CHAR* pcSlicePtr)
{
    UINT32 i;
    UINT32 u4Temp;

    ASSERT(pcSlicePtr!=NULL);
    
    //fprintf(_tRecFileInfo.fpFile,"Scaling List [%d] ======>\n", dwIdx);
    if(u4Idx < 6)
    {
        u4Idx = (u4Idx << 4);
        for(i=0; i<4; i++)
        {
            // add 16 for every list
            u4Temp = (((BYTE)pcSlicePtr[i<<2])<<24) + (((BYTE)pcSlicePtr[(i<<2)+1]) << 16) +(((BYTE)pcSlicePtr[(i<<2)+2]) << 8) + (((BYTE)pcSlicePtr[(i<<2)+3]));
            VLD_WRITE32(RW_VLD_SCL_ADDR, 0x200 + u4Idx + (i << 2));
            VLD_WRITE32(RW_VLD_SCL_DATA, u4Temp);
            VLD_WRITE32(RW_VLD_SCL_ADDR, 0x100 + u4Idx + (i << 2));
            //fprintf(_tRecFileInfo.fpFile,"0x%.8x, ", dReadVLD(RW_VLD_SCL_DATA));
        }
        //fprintf(_tRecFileInfo.fpFile,"\n");      
    }
    else
    {
        u4Idx = (u4Idx == 6)? (u4Idx << 4) : ((u4Idx + 3) << 4); // 6=>16*6   7=>16*6+64(equal to 16*7+48)
        for(i=0; i<16; i++)
        {
            // add 64 for every list
            u4Temp = (((BYTE)pcSlicePtr[i<<2])<<24) + (((BYTE)pcSlicePtr[(i<<2)+1]) << 16) +(((BYTE)pcSlicePtr[(i<<2)+2]) << 8) + (((BYTE)pcSlicePtr[(i<<2)+3]));
            VLD_WRITE32(RW_VLD_SCL_ADDR, 0x200 + u4Idx + (i << 2));
            VLD_WRITE32(RW_VLD_SCL_DATA, u4Temp);
            VLD_WRITE32(RW_VLD_SCL_ADDR, 0x100 + u4Idx + (i << 2));
            //fprintf(_tRecFileInfo.fpFile,"0x%8.x, ", dReadVLD(RW_VLD_SCL_DATA));
        }
        //fprintf(_tRecFileInfo.fpFile,"\n");     
    }
}

// *********************************************************************
// Function : void Reordering(void)
// Description : 
// Parameter : 
// Return    : 
// *********************************************************************
void _VDEC_AVCReordering(void)
{
    UINT32 u4Cnt, u4RegData;
  
    AVC_VLD_WRITE32(RW_AVLD_RPL_REORD, 1);
    u4Cnt = 0;
    while(1)
    {
        if(u4Cnt == 100)
        {
            if((u4RegData = AVC_VLD_READ32(RW_AVLD_RPL_REORD)) > 0)
            {
                if((u4RegData & 0x2) > 0)
                {
                    LOG(2, "Error! _VDEC_AVCReordering timeout!\n");
                }
                break;
            }
            else
            {
                u4Cnt = 0;        
            }
        }
        else
        {
            u4Cnt ++;
        }
    }
}


// *********************************************************************
// Function : void _VDEC_PredWeightTable(void)
// Description : 
// Parameter : 
// Return    : 
// *********************************************************************

void _VDEC_PredWeightTable(void)
{
    UINT32 u4Cnt, u4RegData;
    
    AVC_VLD_WRITE32(RW_AVLD_WEIGHT_PRED_TBL, 1);
    u4Cnt = 0;
    while(1)
    {
        if(u4Cnt == 100)
        {
            if((u4RegData = AVC_VLD_READ32(RW_AVLD_WEIGHT_PRED_TBL)) > 0)
            {
                if((u4RegData & 0x2) > 0)
                {
                    LOG(2, "Error! _VDEC_PredWeightTable timeout!\n");
                }
                break;
            }
            else
            {
                u4Cnt = 0;        
            }
        }
        else
        {
            u4Cnt ++;
        }
    }
}


// *********************************************************************
// Function : DWRD dwAVCVLDShiftBits(void)
// Description : Get AVCVLD shift bits %64
// Parameter : None
// Return    : VLD Sum
// *********************************************************************
UINT32 _VDEC_AVCVLDShiftBits(void)
{
    #ifdef BARREL2_SUPPORT  
    if(_u1BarrelShifterNum == BARREL2)
    {
        return(AVC_VLD_READ32(RO_AVLD_2ND_CTRL) & 0x3F);
    }
    else
    #endif
    {
        return((AVC_VLD_READ32(RW_AVLD_CTRL) >> 16) & 0x3F);
    }
    
}


// *********************************************************************
// Function : DWRD dwVldRPtr(void)
// Description : Get VLD Current Dram pointer
// Parameter : None
// Return    : VLD Sum
// *********************************************************************
UINT32 _VDEC_VldRPtr(void)
{
    UINT32 u4DramRptr;
    UINT32 u4SramRptr, u4SramWptr;
    UINT32 u4SramDataSz;
    UINT32 u4ByteAddr;
    UINT32 u4Bits, u4VStart, u4VEnd;
    UINT32 u4Cnt = 0;

    #ifdef WAIT_SRAM_STABLE
    if(_u1BarrelShifterNum == BARREL2)
    {
        while((VLD_READ32(RO_VLD_2ND_SRAMCTRL) & AA_FIT_TARGET_D)==0)
        {
            u4Cnt++;
            if(u4Cnt >= WAIT_THRD)
            {
                LOG(2, "Rptr-WaitSramStable Fail\n");
                break;
            }        
        }
    }
    else
    {
        while((VLD_READ32(RO_VLD_SRAMCTRL) & AA_FIT_TARGET_D)==0)
        {
            u4Cnt++;
            if(u4Cnt >= WAIT_THRD)
            {
                LOG(2, "Rptr-WaitSramStable Fail\n");
                break;
            }        
        }
    }
    #endif
    
    u4DramRptr = _dVLDDramRp();
    u4SramRptr = _dVLDSramRp();
    u4SramWptr = _dVLDSramWp();
    if(u4SramWptr > u4SramRptr)
    {
        u4SramDataSz = u4SramWptr - u4SramRptr;
    }
    else
    {
        u4SramDataSz = 64 - (u4SramRptr - u4SramWptr);
    }

    u4Bits = _VDEC_AVCVLDShiftBits();

    if(_u1BarrelShifterNum == BARREL2)
    {
        u4ByteAddr = (u4DramRptr - ((u4SramDataSz + 4) * 4)) + (u4Bits / 8);
    }
    else
    {
        #ifdef MEM_PAUSE_SUPPORT  
        u4ByteAddr = (u4DramRptr - ((u4SramDataSz + 5) * 4)) + (u4Bits / 8);
        #else
        u4ByteAddr = (u4DramRptr - ((u4SramDataSz + 4) * 4)) + (u4Bits / 8);
        #endif
    }
    //ringbuffer check
    if(_u1BarrelShifterNum == BARREL2)
    {
        u4VStart = VLD_READ32(RW_VLD_2ND_VSTART)<<6;
        u4VEnd = VLD_READ32(RW_VLD_2ND_VEND)<<6;
    }
    else
    {
        u4VStart = VLD_READ32(RW_VLD_VSTART)<<6;
        u4VEnd = VLD_READ32(RW_VLD_VEND)<<6;    
    }
    if(u4ByteAddr < u4VStart)
    {
        u4ByteAddr = u4VEnd - (u4VStart - u4ByteAddr);
    }
    if(u4ByteAddr > u4VEnd)
    {
        u4ByteAddr = u4VStart + (u4ByteAddr - u4VEnd);
    }
    
    u4Bits &= 0x7;

    return u4ByteAddr;
}


void _VDEC_SetVldWPtr(UINT32 u4WPtr, UINT32 u4VFIFOStart, UINT32 u4VFIFOEnd)
{
    UINT32 u4WPtrAlign, u4VStart, u4VEnd;

    u4WPtrAlign = ((u4WPtr + RPTR_ALIGN) & (~((UINT32)RPTR_ALIGN - 1))) + WPTR_OFFSET;

    /*if(_u1BarrelShifterNum == BARREL2)
    {
        u4VStart = VLD_READ32(RW_VLD_2ND_VSTART)<<6;
        u4VEnd = VLD_READ32(RW_VLD_2ND_VEND)<<6;
    }
    else
    {
        u4VStart = VLD_READ32(RW_VLD_VSTART)<<6;
        u4VEnd = VLD_READ32(RW_VLD_VEND)<<6;    
    }*/
    //avoid to access vld register 47(0xbc) before at powerOff time
    u4VEnd = u4VFIFOEnd;
    u4VStart = u4VFIFOStart;
    
    if(u4WPtrAlign < u4VStart)
    {
        u4WPtrAlign = u4VEnd - (u4VStart - u4WPtrAlign);
    }
    if(u4WPtrAlign > u4VEnd)
    {
        u4WPtrAlign = u4VStart + (u4WPtrAlign - u4VEnd);
    }
    
    VLD_WRITE32(RW_VLD_ASYNC, VLD_READ32(RW_VLD_ASYNC) | WPTR_SETBY_RISC_EN);
    #if defined(CC_MT5360B) || defined(CC_MT5392B) || defined (IS_POST_MT5363)
    VLD_WRITE32(WO_VLD_WPTR, u4WPtrAlign);
    #else
    VLD_WRITE32(WO_VLD_WPTR, (u4WPtrAlign << WPTR_LSH_BITS) | (VLD_EWPL | VLD_RISC_WR_EN));    //set wptr infinity
    #endif
}


// *********************************************************************
// Function    : void vVDecSetPRefPicListReg(void)
// Description : Set B Ref Pic List related registers
// Parameter   : None
// Return      : None
// Note  :JM decode_POC
// *********************************************************************
void _VDEC_SetPRefPicListReg(VDEC_REF_PIC_LIST_T rRefPicList, const VDEC_FBUF_INFO_T* prFBufInfo, 
    UINT32 u4FBufInfo, UINT32 u4ListIdx)
{
    UINT8 u1FBufIdx;
    UINT8 u1Fld;  
    UINT8 u1RefPicIdx;  
    UINT8 u1RegIdx;
    UINT32 u4Temp;
    BOOL fgLRefPic;

    ASSERT(prFBufInfo!=NULL);
    
    u1Fld = (UINT8)(u4FBufInfo & 0xff);  
    u1RefPicIdx = (UINT8)((u4FBufInfo >> 8) & 0xff);  
    u1RegIdx = (UINT8)((u4FBufInfo >> 16) & 0xff);  

    if(u1RefPicIdx >= 32)
    {
        ASSERT(u1RefPicIdx < 32);   //size of au4FBufIdx
        return;
    }
    u1FBufIdx = (UINT8)rRefPicList.au4FBufIdx[u1RefPicIdx];
    fgLRefPic = (u4ListIdx > 3)? TRUE: FALSE;

    if(u1Fld == FRAME)
    {    
        //bRegIdx = bRefPicIdx;
        MC_WRITE32((UINT32)(RW_MC_P_LIST0 + (u1RegIdx <<2)), prFBufInfo[u1FBufIdx].u4YAddr);
        AVC_VLD_WRITE32((UINT32)(RW_AVLD_REORD_P0_RPL + (u1RegIdx<<2)), 
                        (UINT32)((fgLRefPic << 19) + (fgLRefPic? 
                        (prFBufInfo[u1FBufIdx].i4LongTermPicNum & 0x7ffff) : 
                        (prFBufInfo[u1FBufIdx].i4PicNum & 0x7ffff))));        

        u4Temp = (UINT32)((((prFBufInfo[u1FBufIdx].i4TFldPOC <= 
                        prFBufInfo[u1FBufIdx].i4BFldPOC) && 
                        (prFBufInfo[u1FBufIdx].i4TFldPOC != 0x7fffffff))? 1: 0) << 21);    
        u4Temp |= (fgLRefPic << 20);    
        u4Temp |= ((((UINT32)u1FBufIdx)<<1)<<22) + prFBufInfo[u1FBufIdx].u4TFldPara;
        AVC_MV_WRITE32((UINT32)(RW_AMV_P_REF_PARA + (u1RegIdx<<3)), u4Temp);

        #ifdef REF_TRACE
        LOG(2, "PList RegIdx=%d, YAddr=0x%x, (-), FrmNum(%d), LongTermIdx(%d), POC(%d), 0x%x\n", 
        u1RegIdx, PHYSICAL(prFBufInfo[u1FBufIdx].u4YAddr), prFBufInfo[u1FBufIdx].u4FrameNum, 
        prFBufInfo[u1FBufIdx].u4LongTermFrameIdx, prFBufInfo[u1FBufIdx].i4POC, u4Temp);
        #endif        
        u4Temp = (UINT32)((((prFBufInfo[u1FBufIdx].i4TFldPOC <= 
                        prFBufInfo[u1FBufIdx].i4BFldPOC) && 
                        (prFBufInfo[u1FBufIdx].i4TFldPOC != 0x7fffffff))? 1: 0) << 21);    
        u4Temp |= (fgLRefPic << 20);    
        u4Temp |= (UINT32)((((((UINT32)u1FBufIdx)<<1) + 1)<<22) + prFBufInfo[u1FBufIdx].u4BFldPara);
        AVC_MV_WRITE32((UINT32)(RW_AMV_P_REF_PARA + (u1RegIdx<<3) + 4), u4Temp);
    }
    else
    { 
        if(u1Fld == TOP_FIELD)
        {
            MC_WRITE32((UINT32)(RW_MC_P_LIST0 + (u1RegIdx<<2)), prFBufInfo[u1FBufIdx].u4YAddr);            
            AVC_VLD_WRITE32((UINT32)(RW_AVLD_REORD_P0_RPL + (u1RegIdx<<2)), 
                        (UINT32)((fgLRefPic << 19) + (fgLRefPic? 
                        (prFBufInfo[u1FBufIdx].i4TFldLongTermPicNum & 0x7ffff) : 
                        (prFBufInfo[u1FBufIdx].i4TFldPicNum & 0x7ffff))));
      
            //dwTemp = (((_ptFBufInfo[bFBufIdx].iTFldPOC <= _ptFBufInfo[bFBufIdx].iBFldPOC) && (_ptFBufInfo[bFBufIdx].iTFldPOC != 0x7fffffff))? 1: 0) << 21;    
            u4Temp = (fgLRefPic << 20);      
            u4Temp |= ((((UINT32)u1FBufIdx)<<1)<<22) + prFBufInfo[u1FBufIdx].u4TFldPara;
            AVC_MV_WRITE32((UINT32)(RW_AMV_P_REF_PARA + (u1RegIdx<<2)), u4Temp);            
            
            #ifdef REF_TRACE
            LOG(2, "PList RegIdx=%d, YAddr=0x%x, (-), FrmNum(%d), LongTermIdx(%d), POC(%d), 0x%x\n", 
            u1RegIdx, PHYSICAL(prFBufInfo[u1FBufIdx].u4YAddr), prFBufInfo[u1FBufIdx].u4FrameNum, 
            prFBufInfo[u1FBufIdx].u4LongTermFrameIdx, prFBufInfo[u1FBufIdx].i4POC, u4Temp);
            #endif

        }
        else
        {        
            MC_WRITE32((UINT32)(RW_MC_P_LIST0 + (u1RegIdx<<2)), prFBufInfo[u1FBufIdx].u4YAddr);            
            AVC_VLD_WRITE32((UINT32)(RW_AVLD_REORD_P0_RPL + (u1RegIdx<<2)), 
                        (UINT32)((fgLRefPic << 19) + (fgLRefPic? 
                        (prFBufInfo[u1FBufIdx].i4BFldLongTermPicNum & 0x7ffff) : 
                        (prFBufInfo[u1FBufIdx].i4BFldPicNum & 0x7ffff))));
      
            //dwTemp = (((_ptFBufInfo[bFBufIdx].iTFldPOC <= _ptFBufInfo[bFBufIdx].iBFldPOC) && (_ptFBufInfo[bFBufIdx].iTFldPOC != 0x7fffffff))? 1: 0) << 21;    
            u4Temp = (fgLRefPic << 20);      
            u4Temp |= ((((UINT32)u1FBufIdx<<1) + 1)<<22) + prFBufInfo[u1FBufIdx].u4BFldPara;
            AVC_MV_WRITE32((UINT32)(RW_AMV_P_REF_PARA + (u1RegIdx<<2)), u4Temp);
      
            MC_WRITE32(RW_MC_P_LIST0_FLD, 
                (UINT32)(MC_READ32(RW_MC_P_LIST0_FLD) | (0x1 << u1RegIdx)));
            #ifdef REF_TRACE
            LOG(2, "PList RegIdx=%d, YAddr=0x%x, (-), FrmNum(%d), LongTermIdx(%d), POC(%d), 0x%x\n", 
            u1RegIdx, PHYSICAL(prFBufInfo[u1FBufIdx].u4YAddr), prFBufInfo[u1FBufIdx].u4FrameNum, 
            prFBufInfo[u1FBufIdx].u4LongTermFrameIdx, prFBufInfo[u1FBufIdx].i4POC, u4Temp);
            #endif

        }     
    }
}


// *********************************************************************
// Function    : void vVDecSetBRefPicListReg(void)
// Description : Set B Ref Pic List related registers
// Parameter   : None
// Return      : None
// Note  :JM decode_POC
// *********************************************************************
void _VDEC_SetBRefPicListReg(VDEC_REF_PIC_LIST_T rRefPicList, VDEC_REF_PIC_LIST_T rRefPicList1, 
    const VDEC_FBUF_INFO_T* prFBufInfo, UINT32 u4FBufInfo, UINT32 u4ListIdx)
{
    UINT8 u1FBufIdx;
    UINT8 u1Fld;  
    UINT8 u1RefPicIdx;  
    UINT8 u1RegIdx;
    UINT32 u4Temp;
    BOOL fgLRefPic; 

    ASSERT(prFBufInfo!=NULL);

    u1Fld = (UINT8)(u4FBufInfo & 0xff);  
    u1RefPicIdx = (UINT8)((u4FBufInfo >> 8) & 0xff);  
    u1RegIdx = (UINT8)((u4FBufInfo >> 16) & 0xff);  

    if(u1RefPicIdx >= 32)
    {
        ASSERT(u1RefPicIdx < 32);   //size of au4FBufIdx
        return;
    }
    u1FBufIdx = (UINT8)rRefPicList.au4FBufIdx[u1RefPicIdx];
    fgLRefPic = (u4ListIdx > 3)? TRUE: FALSE;

    if(u1Fld == FRAME)
    {
        // B_0
        MC_WRITE32((UINT32)(RW_MC_B_LIST0 + (u1RegIdx<<2)), prFBufInfo[u1FBufIdx].u4YAddr);
        #ifdef REF_TRACE
        LOG(2, "BList0 RegIdx=%d, YAddr=0x%x\n", u1RegIdx, prFBufInfo[u1FBufIdx].u4YAddr);
        #endif
        
        AVC_VLD_WRITE32((UINT32)(RW_AVLD_REORD_B0_RPL + (u1RegIdx<<2)), 
                        (UINT32)((fgLRefPic << 19) + 
                        (fgLRefPic? (prFBufInfo[u1FBufIdx].i4LongTermPicNum & 0x7ffff) : 
                        (prFBufInfo[u1FBufIdx].i4PicNum & 0x7ffff))));
      
        u4Temp = (UINT32)((((prFBufInfo[u1FBufIdx].i4TFldPOC <= prFBufInfo[u1FBufIdx].i4BFldPOC) 
                        && (prFBufInfo[u1FBufIdx].i4TFldPOC != 0x7fffffff))? 1: 0) << 21);
        u4Temp |= (fgLRefPic << 20);
        u4Temp += ((((UINT32)u1FBufIdx)<<1)<<22) + prFBufInfo[u1FBufIdx].u4TFldPara + 
                        (prFBufInfo[u1FBufIdx].i4TFldPOC & 0x3ffff);
        AVC_MV_WRITE32((UINT32)(RW_AMV_B0_REF_PARA + (u1RegIdx<<3)), u4Temp);

        u4Temp = (UINT32)((((prFBufInfo[u1FBufIdx].i4TFldPOC <= prFBufInfo[u1FBufIdx].i4BFldPOC) 
                        && (prFBufInfo[u1FBufIdx].i4TFldPOC != 0x7fffffff))? 1: 0) << 21);
        u4Temp |= (fgLRefPic << 20);
        u4Temp += (((((UINT32)u1FBufIdx)<<1) + 1)<<22) + prFBufInfo[u1FBufIdx].u4BFldPara + 
                        (prFBufInfo[u1FBufIdx].i4BFldPOC & 0x3ffff);
        AVC_MV_WRITE32((UINT32)(RW_AMV_B0_REF_PARA + (u1RegIdx<<3) + 4), u4Temp);

        //B_1
        if(u4ListIdx < 4)
        {
            u4ListIdx += 2;
            u1FBufIdx = (UINT8)rRefPicList1.au4FBufIdx[u1RefPicIdx];   
        }
        else     // long term
        {
            u1FBufIdx = (UINT8)rRefPicList.au4FBufIdx[u1RefPicIdx];
        }
        MC_WRITE32((UINT32)(RW_MC_B_LIST1 + (u1RegIdx<<2)), prFBufInfo[u1FBufIdx].u4YAddr);  
        #ifdef REF_TRACE
        LOG(2, "BList1 RegIdx=%d, YAddr=0x%x\n", u1RegIdx, prFBufInfo[u1FBufIdx].u4YAddr);
        #endif
        
        AVC_VLD_WRITE32((UINT32)(RW_AVLD_REORD_B1_RPL + (u1RegIdx<<2)), 
                        (UINT32)((fgLRefPic << 19) + 
                        (fgLRefPic? (prFBufInfo[u1FBufIdx].i4LongTermPicNum & 0x7ffff) : 
                        (prFBufInfo[u1FBufIdx].i4PicNum & 0x7ffff))));
      
        u4Temp = (UINT32)((((prFBufInfo[u1FBufIdx].i4TFldPOC <= prFBufInfo[u1FBufIdx].i4BFldPOC) 
                        && (prFBufInfo[u1FBufIdx].i4TFldPOC != 0x7fffffff))? 1: 0) << 21);
        u4Temp |= (fgLRefPic << 20);
        u4Temp += ((((UINT32)u1FBufIdx)<<1)<<22) + prFBufInfo[u1FBufIdx].u4TFldPara + 
                        (prFBufInfo[u1FBufIdx].i4TFldPOC & 0x3ffff);
        AVC_MV_WRITE32((UINT32)(RW_AMV_B1_REF_PARA + (u1RegIdx<<3)), u4Temp);

        u4Temp = (UINT32)((((prFBufInfo[u1FBufIdx].i4TFldPOC <= prFBufInfo[u1FBufIdx].i4BFldPOC) 
                        && (prFBufInfo[u1FBufIdx].i4TFldPOC != 0x7fffffff))? 1: 0) << 21);
        u4Temp |= (fgLRefPic << 20);
        u4Temp += (((((UINT32)u1FBufIdx)<<1) + 1)<<22) + prFBufInfo[u1FBufIdx].u4BFldPara + 
                        (prFBufInfo[u1FBufIdx].i4BFldPOC & 0x3ffff);
        AVC_MV_WRITE32((UINT32)(RW_AMV_B1_REF_PARA + (u1RegIdx<<3) + 4), u4Temp);

        u4Temp = PHYSICAL(prFBufInfo[u1FBufIdx].u4MVBufAddr);
        AVC_MV_WRITE32((UINT32)(RW_AMV_MV_BUF_ADDR + (u1RegIdx<<3)), (UINT32)(u4Temp >> 4));
        AVC_MV_WRITE32((UINT32)(RW_AMV_MV_BUF_ADDR + (u1RegIdx<<3) + 4), 
            (UINT32)((u4Temp >> 4) + 4)); 
    }
    else
    {
        // B_0
        if((u4ListIdx == 0) || (u4ListIdx == 1) || (u4ListIdx == 4) || (u4ListIdx == 5))
        {      
            //u4Temp = (((_ptFBufInfo[bFBufIdx].iTFldPOC <= _ptFBufInfo[bFBufIdx].iBFldPOC) && (_ptFBufInfo[bFBufIdx].iTFldPOC != 0x7fffffff))? 1: 0) << 21;
            if(u1Fld == TOP_FIELD)
            {
                MC_WRITE32((UINT32)(RW_MC_B_LIST0 + (u1RegIdx<<2)), prFBufInfo[u1FBufIdx].u4YAddr);
                #ifdef REF_TRACE
                LOG(2, "BList0 RegIdx=%d, YAddr=0x%x\n", u1RegIdx, prFBufInfo[u1FBufIdx].u4YAddr);
                #endif
                
                AVC_VLD_WRITE32((UINT32)(RW_AVLD_REORD_B0_RPL + (u1RegIdx<<2)), 
                        (UINT32)((fgLRefPic << 19) + 
                        (fgLRefPic? (prFBufInfo[u1FBufIdx].i4TFldLongTermPicNum & 0x7ffff) : 
                        (prFBufInfo[u1FBufIdx].i4TFldPicNum & 0x7ffff))));
        
                u4Temp = (UINT32)(((((UINT32)u1FBufIdx)<<1)<<22) + prFBufInfo[u1FBufIdx].u4TFldPara + 
                        (prFBufInfo[u1FBufIdx].i4TFldPOC & 0x3ffff));
                u4Temp |= (fgLRefPic << 20);
                AVC_MV_WRITE32((UINT32)(RW_AMV_B0_REF_PARA + (u1RegIdx<<2)), u4Temp);
            }
            else
            {        
                MC_WRITE32((UINT32)(RW_MC_B_LIST0 + (u1RegIdx<<2)), prFBufInfo[u1FBufIdx].u4YAddr);
                #ifdef REF_TRACE
                LOG(2, "BList0 RegIdx=%d, YAddr=0x%x\n", u1RegIdx, prFBufInfo[u1FBufIdx].u4YAddr);
                #endif
                
                AVC_VLD_WRITE32((UINT32)(RW_AVLD_REORD_B0_RPL + (u1RegIdx<<2)), 
                        (UINT32)((fgLRefPic << 19) + 
                        (fgLRefPic? (prFBufInfo[u1FBufIdx].i4BFldLongTermPicNum & 0x7ffff) : 
                        (prFBufInfo[u1FBufIdx].i4BFldPicNum & 0x7ffff))));
        
                u4Temp = (UINT32)(((((UINT32)u1FBufIdx<<1) + 1)<<22) + prFBufInfo[u1FBufIdx].u4BFldPara + 
                        (prFBufInfo[u1FBufIdx].i4BFldPOC & 0x3ffff));
                u4Temp |= (fgLRefPic << 20);
                AVC_MV_WRITE32((UINT32)(RW_AMV_B0_REF_PARA + (u1RegIdx<<2)), u4Temp);
                MC_WRITE32(RW_MC_B_LIST0_FLD, 
                    (UINT32)(MC_READ32(RW_MC_B_LIST0_FLD) | (0x1 << u1RegIdx)));        
            }
        }
        // B_1    
        if((u4ListIdx == 2) || (u4ListIdx == 3) || (u4ListIdx == 4) || (u4ListIdx == 5))
        {      
            //u4Temp = (((_ptFBufInfo[bFBufIdx].iTFldPOC <= _ptFBufInfo[bFBufIdx].iBFldPOC) && (_ptFBufInfo[bFBufIdx].iTFldPOC != 0x7fffffff))? 1: 0) << 21;
            if(u1Fld == TOP_FIELD)
            {
                MC_WRITE32((UINT32)(RW_MC_B_LIST1 + (u1RegIdx<<2)), prFBufInfo[u1FBufIdx].u4YAddr);
                #ifdef REF_TRACE
                LOG(2, "BList1 RegIdx=%d, YAddr=0x%x\n", u1RegIdx, prFBufInfo[u1FBufIdx].u4YAddr);
                #endif
                AVC_VLD_WRITE32((UINT32)(RW_AVLD_REORD_B1_RPL + (u1RegIdx<<2)), 
                        (UINT32)((fgLRefPic << 19) + 
                        (fgLRefPic? (prFBufInfo[u1FBufIdx].i4TFldLongTermPicNum & 0x7ffff) : 
                        (prFBufInfo[u1FBufIdx].i4TFldPicNum & 0x7ffff))));

                u4Temp = PHYSICAL(prFBufInfo[u1FBufIdx].u4MVBufAddr);
                AVC_MV_WRITE32((UINT32)(RW_AMV_MV_BUF_ADDR + (u1RegIdx<<2)), 
                    (UINT32)(u4Temp >> 4));      
        
                u4Temp = (UINT32)(((((UINT32)u1FBufIdx <<1 )<<22) + prFBufInfo[u1FBufIdx].u4TFldPara) + 
                        (prFBufInfo[u1FBufIdx].i4TFldPOC & 0x3ffff));
                u4Temp |= (fgLRefPic << 20);
                AVC_MV_WRITE32((UINT32)(RW_AMV_B1_REF_PARA + (u1RegIdx<<2)), u4Temp);
            }
            else
            {
                MC_WRITE32((UINT32)(RW_MC_B_LIST1 + (u1RegIdx<<2)), prFBufInfo[u1FBufIdx].u4YAddr);
                #ifdef REF_TRACE
                LOG(2, "BList1 RegIdx=%d, YAddr=0x%x\n", u1RegIdx, prFBufInfo[u1FBufIdx].u4YAddr);
                #endif
                AVC_VLD_WRITE32((UINT32)(RW_AVLD_REORD_B1_RPL + (u1RegIdx<<2)), 
                        (UINT32)((fgLRefPic << 19) + 
                        (fgLRefPic? (prFBufInfo[u1FBufIdx].i4BFldLongTermPicNum & 0x7ffff) : 
                        (prFBufInfo[u1FBufIdx].i4BFldPicNum & 0x7ffff))));
        
                u4Temp = PHYSICAL(prFBufInfo[u1FBufIdx].u4MVBufAddr);
                AVC_MV_WRITE32((UINT32)(RW_AMV_MV_BUF_ADDR + (u1RegIdx<<2)), 
                    (UINT32)((u4Temp >> 4) + 4));
     
                u4Temp = (UINT32)((((((UINT32)u1FBufIdx<<1) + 1) << 22) + prFBufInfo[u1FBufIdx].u4BFldPara) + 
                        (prFBufInfo[u1FBufIdx].i4BFldPOC & 0x3ffff));
                u4Temp |= (fgLRefPic << 20);
                AVC_MV_WRITE32((UINT32)(RW_AMV_B1_REF_PARA + (u1RegIdx<<2)), u4Temp);

                MC_WRITE32(RW_MC_B_LIST1_FLD, 
                    (UINT32)(MC_READ32(RW_MC_B_LIST1_FLD) | (0x1 << u1RegIdx)));        
            }
        }
    }
}


UINT32 _VDEC_IO_Read32(UINT32 u4Base, UINT32 u4Offset)
{
    if(_bVdecLogSimPat)
    {
        switch(u4Base)
        {
            case VLD_BASE:
                LOG(2, "RISCRead(`VDEC_FULL_ADDR_VLD + 4*%d, 32'hx);\n", (u4Offset & 0xFFF)>>2);
                break;
            case MC_BASE:
                LOG(2, "RISCRead(`VDEC_FULL_ADDR_MC + 4*%d, 32'hx);\n", (u4Offset & 0xFFF)>>2);            
                break;
            case AVC_VLD_BASE:
                LOG(2, "RISCRead(`VDEC_FULL_ADDR_AVC_VLD + 4*%d, 32'hx);\n", (u4Offset & 0xFFF)>>2);            
                break;
            case AVC_MV_BASE:
                LOG(2, "RISCRead(`VDEC_FULL_ADDR_AVC_MV + 4*%d, 32'hx);\n", (u4Offset & 0xFFF)>>2);            
                break;
            default:
                break;
        }
        x_thread_delay(1);
    }
    return IO_READ32(u4Base, u4Offset);
}

void _VDEC_IO_Write32(UINT32 u4Base, UINT32 u4Offset, UINT32 u4Value)
{
    if(_bVdecLogSimPat)
    {
        switch(u4Base)
        {
            case VLD_BASE:
                if((u4Offset==RW_VLD_RPTR) || (u4Offset==RW_VLD_2ND_RPTR))
                {
                    LOG(2, "RISCWrite(`VDEC_FULL_ADDR_VLD + 4*%d, 32'h%x);\n", (u4Offset & 0xFFF)>>2, 
                        u4Value - (IO_READ32(u4Base, RW_VLD_VSTART) << 6));
                }
                else
                {
                    LOG(2, "RISCWrite(`VDEC_FULL_ADDR_VLD + 4*%d, 32'h%x);\n", (u4Offset & 0xFFF)>>2, u4Value);
                    if((u4Offset==RW_VLD_PROC) || (u4Offset==RW_VLD_2ND_PROC))
                    {
                        LOG(2, "#200000;\n");
                    }
                }
                break;
            case MC_BASE:
                if(u4Offset==RW_MC_PP_Y_ADDR)
                {
                    LOG(2, "RISCWrite(`VDEC_FULL_ADDR_MC + 4*137, PP_OUT_Y_ADDR);\n");
                }
                else if(u4Offset==RW_MC_PP_C_ADDR)
                {
                    LOG(2, "RISCWrite(`VDEC_FULL_ADDR_MC + 4*138, PP_OUT_C_ADDR);\n");
                }
                else
                {
                    LOG(2, "RISCWrite(`VDEC_FULL_ADDR_MC + 4*%d, 32'h%x);\n", (u4Offset & 0xFFF)>>2, u4Value);            
                }
                break;
            case AVC_VLD_BASE:
                LOG(2, "RISCWrite(`VDEC_FULL_ADDR_AVC_VLD + 4*%d, 32'h%x);\n", (u4Offset & 0xFFF)>>2, u4Value);
                if(u4Offset==RW_AVLD_PROC)
                {
                    LOG(2, "//==============Finish simulation pattern===========\n");
                }
                break;
            case AVC_MV_BASE:
                LOG(2, "RISCWrite(`VDEC_FULL_ADDR_AVC_MV + 4*%d, 32'h%x);\n", (u4Offset & 0xFFF)>>2, u4Value);            
                break;
            default:
                break;
        }
        x_thread_delay(1);
    }
    IO_WRITE32(u4Base, u4Offset, u4Value);
}

void _VDEC_LogSimPat(BOOL bLogEn)
{
    _bVdecLogSimPat = bLogEn;
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------



