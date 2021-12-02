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
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: vdec_mpeg4hw.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_mpeg4hw.c
 *  This file contains implementation of exported APIs of mpeg4 decoder.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
#include "vdec_mpeg4hw.h"
#include "vdec_mpeg4util.h"
LINT_EXT_HEADER_BEGIN
#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "x_common.h"
#include "x_typedef.h"
LINT_EXT_HEADER_END

#ifdef CC_IS_MPEG4_POST_MT5363
#define DBK_MIN_H    32

#define DBK_THQP0I     9
#define DBK_THQP0P     8
#define DBK_THQP0B     8
#define DBK_THQP0      12
#define DBK_THQP0I_C  9
#define DBK_THQP0P_C  8
#define DBK_THQP0B_C  8
#define DBK_THQP0_C   12

#define DBK_FTR_RATIO  4
#define DBK_QP1_Y     3
#define DBK_QP2_Y     11
#define DBK_QP3_Y     3
#define DBK_QP1_C     17
#define DBK_QP2_C     4
#define DBK_QP3_C     2
#define DBK_MB_FTR_CNT  96 //16x6
#define DBK_QP_MAX  31
#else
//for DBK
#define DBK_QP0     12
#define DBK_QP1     15
#define DBK_QP2     26
#define DBK_QP3     29
//I
#define DBK_QP0_I     9
#define DBK_QP1_I     12
#define DBK_QP2_I     23
#define DBK_QP3_I     26
//P
#define DBK_QP0_P     8
#define DBK_QP1_P     11
#define DBK_QP2_P     22
#define DBK_QP3_P     25
//B
#define DBK_QP0_B     8
#define DBK_QP1_B     11
#define DBK_QP2_B     22
#define DBK_QP3_B     25
#endif


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static BOOL _IsMpeg4VLDFetchOk(void);

static BOOL _WaitMpeg4VldFetchOk(void);

static UINT32 _VLDSramRp(void);
static UINT32 _VLDSramWp(void);
static UINT32 _VLDDramRp(void);

// *********************************************************************
// Function : BOOL IsVLDFetchOk(void)
// Description : Check if VLD fetch is done
// Parameter : None
// Return    : TRUE: VLD fetch OK, FALSE: not OK
// *********************************************************************
static BOOL _IsMpeg4VLDFetchOk(void)
{
	if((VLD_READ32(RO_VLD_FETCHOK) & VLD_FETCH_OK) == 0)
	{
		return FALSE;
	}
	return TRUE;
}

// *********************************************************************
// Function : BOOL WaitVldFetchOk(void)
// Description :
// Parameter :
// Return    : TRUE: Init Barrel Shifter OK, FALSE: Time Out
// *********************************************************************
static BOOL _WaitMpeg4VldFetchOk(void)
{
	UINT32 u4Cnt;

	if(!_IsMpeg4VLDFetchOk())
	{
		u4Cnt = 0;
		while(!_IsMpeg4VLDFetchOk())
		{
			u4Cnt++;
			if(u4Cnt > 0xFFFFFF)
			{
			    LOG(0, "Fetch is not OK\n");
			    return FALSE;
			}
		}
	}

    u4Cnt = 0;
    while((VLD_READ32(RO_VLD_SRAMCTRL) & AA_FIT_TARGET_D)==0)
    {
        u4Cnt++;
        if(u4Cnt >= 0x100000)
        {
            LOG(1, "//===SRAM !Stable 0xF4=0x%x, Rptr 0xB0=0x%x, 0xFC=0x%x, 0xEC=0x%x, 0xE8=0x%x\n", 
                VLD_READ32(RO_VLD_SRAMCTRL), VLD_READ32(RW_VLD_RPTR), VLD_READ32(RO_VLD_VRPTR),
                VLD_READ32(RO_VLD_VBAR), VLD_READ32(RO_VLD_FETCHOK));
            return FALSE;
        }    
    }
    LOG(6, "//WaitFetchOK Rptr 0xB0=0x%x, 0xFC=0x%x, 0xEC=0x%x, 0xE8=0x%x\n", 
        VLD_READ32(RW_VLD_RPTR), VLD_READ32(RO_VLD_VRPTR),
        VLD_READ32(RO_VLD_VBAR), VLD_READ32(RO_VLD_FETCHOK));

	return TRUE;
}

// *********************************************************************
// Function : DWRD _Mpeg4VLDGetBitS(DWRD dShiftBit)
// Description : Get Bitstream from VLD barrel shifter
// Parameter : dShiftBit: Bits to shift (0-32)
// Return    : barrel shifter
// *********************************************************************
static UINT32 _Mpeg4VLDGetBitS(UINT32 u4ShiftBit)//, BYTE bBarrelNum)
{
	UINT32 u4RegVal;

	u4RegVal = VLD_READ32(RO_VLD_BARL + (u4ShiftBit << 2));

	return u4RegVal;
}

// *********************************************************************
// Function : DWRD VLDSramRp(void)
// Description : Get VLD SRAM Read Ptr
// Parameter : None
// Return    : VLD SRAM Read PTR
// *********************************************************************
static UINT32 _VLDSramRp(void)
{
#ifdef CC_IS_MPEG4_POST_MT5363
    UINT32 u4SramRptr;
    u4SramRptr = VLD_READ32(RO_VLD_VBAR);
    u4SramRptr = (UINT32)((((u4SramRptr & 0xf) * 4) + ((u4SramRptr >> 24) & 0x3)));
    return u4SramRptr;
#else
	return(VLD_READ32(RO_VLD_VBAR) & 0xff);
#endif
}


// *********************************************************************
// Function : DWRD VLDSramWp(void)
// Description : Get VLD SRAM Write Ptr
// Parameter : None
// Return    : VLD SRAM Write PTR
// *********************************************************************
static UINT32 _VLDSramWp(void)
{
#ifdef CC_IS_MPEG4_POST_MT5363
    UINT32 u4SramWptr;
    //u4SramWptr = (_MPV_VldRead32(ucMpvId, RO_VLD_VBAR) >> 16) & 0x3f;
    u4SramWptr = (VLD_READ32(RO_VLD_VBAR));
    u4SramWptr = (UINT32)((((u4SramWptr >> 8) & 0xf) * 4));
    return u4SramWptr;
#else
	return((VLD_READ32(RO_VLD_VBAR) >> 16) & 0xff);
#endif
}  


// *********************************************************************
// Function : DWRD dVLDDramRp(void)
// Description : Get VLD DRAM Read Ptr
// Parameter : None
// Return    : VLD DRAM Read PTR
// *********************************************************************
static UINT32 _VLDDramRp(void)
{
	return VLD_READ32(RO_VLD_VRPTR);// & 0xffffff);
}

// *********************************************************************
// Function : void vVLDBarl2Vdec(void)
// Description : Load sum from barrel shifter to VDec HW
// Parameter : None
// Return    : None
// *********************************************************************
void _VDEC_VLDBarl2Vdec(void)
{
    // Software workaround 
    // Fix random decoding timeout for 240MHz 
    VLD_READ32(0);
       
    VLD_WRITE32(RW_VLD_LDSR, 1);
    VLD_WRITE32(RW_VLD_LDSR, 0);
}

// *********************************************************************
// Function : void vVLDVdec2Barl(void)
// Description : Load sum from VDec HW to barrel shifter
// Parameter : None
// Return    : None
// *********************************************************************
void _VDEC_VLDVdec2Barl(void)
{
	VLD_WRITE32(RW_VLD_LDSH, 1);
	VLD_WRITE32(RW_VLD_LDSH, 0);
}

// *********************************************************************
// Function : DWRD _VDEC_Mpeg4GetBitStream(VDEC_MPEG4_DEC_PARAM_INFO_T* pDecPrm,UINT32 dwShiftBit)
// Description : get 32 bits from shifter
// Parameter : bShiftBit: if 0, just get 32 bits from shifter
//                        others, 1. remove bShiftBit bits
//                                2. get 32 bits from shifter
// Return    : 32 bits value
// *********************************************************************
UINT32 _VDEC_Mpeg4GetBitStream(UINT32* pu4BitCount, UINT32 u4ShiftBit)
{
	UINT32 u4RegVal;

	*pu4BitCount += u4ShiftBit;
	u4RegVal = _Mpeg4VLDGetBitS(u4ShiftBit);
	return u4RegVal;
}

// *********************************************************************
// Function : DWRD _VDEC_Mpeg4VLDShiftBits(void)
// Description : Get Mpeg4 shift bits %64
// Parameter : None
// Return    : VLD Sum
// *********************************************************************
UINT32 _VDEC_Mpeg4VLDShiftBits(void)
{
	return((VLD_READ32(RO_VLD_VRPTR)>> 16) & 0x3F);
}

// *********************************************************************
// Function : DWRD _VDEC_VldMpeg4RPtr(void)
// Description : Get VLD Current Dram pointer
// Parameter : None
// Return    : VLD Sum
// *********************************************************************
#ifdef CC_IS_MPEG4_POST_MT5363
UINT32 _VDEC_VldMpeg4RPtr(UINT32* pu4Bits)
{
    UINT32 u4Cnt = 0;
    UINT32 u4DramRptr;
    UINT32 u4SramRptr, u4SramWptr;
    UINT32 u4SramDataSz;
    //UINT32 u4ByteAddr;
    UINT32 u4ReadPtr;
    UINT32 u4VStart, u4VEnd;
    
    /*while(_WaitMpeg4VldFetchOk() != TRUE)
    {
    }*/

    while((VLD_READ32(RO_VLD_SRAMCTRL) & AA_FIT_TARGET_D)==0)
    {
        u4Cnt++;
        if(u4Cnt >= 0x100000)
        {
            LOG(1, "Rptr-WaitSramStable Fail\n");
            break;
        }        
    }
    
    u4DramRptr = _VLDDramRp();
    u4SramRptr = _VLDSramRp();
    u4SramWptr = _VLDSramWp();
    if (u4SramRptr == u4SramWptr)
    {
        u4SramDataSz = 64;
    }
    else if (u4SramWptr >= u4SramRptr)
    {
        u4SramDataSz = u4SramWptr - u4SramRptr;
    }
    else
    {
        u4SramDataSz = 64 - (u4SramRptr - u4SramWptr);
    }

    u4ReadPtr= (u4DramRptr - ((u4SramDataSz + 3) * 4)) & 0xfffffff0;

    u4VStart = VLD_READ32(RW_VLD_VSTART) << 6;    
    u4VEnd = VLD_READ32(RW_VLD_VEND) << 6;
    
    if (u4ReadPtr < u4VStart)    
    {
        u4ReadPtr = u4ReadPtr + (u4VEnd - u4VStart);
    }
    if (u4ReadPtr > u4VEnd)
    {
        u4ReadPtr = u4VStart + (u4ReadPtr - u4VEnd);
    }

    return (u4ReadPtr);
}
#else
UINT32 _VDEC_VldMpeg4RPtr(UINT32* pu4Bits)
{
    UINT32 u4Cnt = 0;
    UINT32 u4DramRptr;
    UINT32 u4SramRptr, u4SramWptr;
    UINT32 u4SramDataSz;
    UINT32 u4ByteAddr;
    UINT32 u4VStart, u4VEnd;
    
    /*while(_WaitMpeg4VldFetchOk() != TRUE)
    {
    }*/

    while((VLD_READ32(RO_VLD_SRAMCTRL) & AA_FIT_TARGET_D)==0)
    {
        u4Cnt++;
        if(u4Cnt >= 0x100000)
        {
            LOG(1, "Rptr-WaitSramStable Fail\n");
            break;
        }        
    }
    
    u4DramRptr = _VLDDramRp();
    u4SramRptr = _VLDSramRp();
    u4SramWptr = _VLDSramWp();
    if(u4SramWptr > u4SramRptr)
    {
        u4SramDataSz = u4SramWptr - u4SramRptr;
    }
    else
    {
        u4SramDataSz = 64 - (u4SramRptr - u4SramWptr);
    }

    *pu4Bits = VLD_READ32(RO_VLD_SUM) & 0x3F;

    if (VLD_READ32(RW_VLD_MP4_ABS) & 0x1) // 03 replacement enable!
    {
        u4ByteAddr = u4DramRptr - (u4SramDataSz + 4) * 4 + (*pu4Bits / 8);
    }
    else
    {
        u4ByteAddr = u4DramRptr - (u4SramDataSz + 3)* 4 + (*pu4Bits / 8);
    }
    *pu4Bits &= 0x7;

    u4VStart = VLD_READ32(RW_VLD_VSTART) << 6;
    u4VEnd = VLD_READ32(RW_VLD_VEND) << 6;

    if (u4ByteAddr < u4VStart)
    {
        u4ByteAddr = u4ByteAddr + (u4VEnd - u4VStart);
    }
    if(u4ByteAddr > u4VEnd)
    {
        u4ByteAddr = u4VStart + (u4ByteAddr - u4VEnd);
    }

    return u4ByteAddr;
}
#endif

// *********************************************************************
// Function : DWRD _VDEC_UpdateVldMpeg4WPtr(UINT32 u4WPtr)
// Description : Get VLD Current Dram pointer
// Parameter : None
// Return    : VLD Sum
// *********************************************************************
UINT32 _VDEC_UpdateVldMpeg4WPtr(UINT32 u4WPtr)
{
    /*UINT32 u4RegVal;

    u4RegVal = (VLD_READ32(WO_VLD_WPTR) & 0x7)|(u4WPtr << 3);

    VLD_WRITE32(WO_VLD_WPTR, u4RegVal);*/

    UINT32 u4WPtrAlign, u4VStart, u4VEnd;

    u4WPtrAlign = ((u4WPtr + RPTR_ALIGN) & (~((UINT32)RPTR_ALIGN - 1))) + WPTR_OFFSET;

    u4VStart = VLD_READ32(RW_VLD_VSTART)<<6;
    u4VEnd = VLD_READ32(RW_VLD_VEND)<<6;    

    if(u4WPtrAlign < u4VStart)
    {
        u4WPtrAlign = u4VEnd - (u4VStart - u4WPtrAlign);
    }
    if(u4WPtrAlign > u4VEnd)
    {
        u4WPtrAlign = u4VStart + (u4WPtrAlign - u4VEnd);
    }
#if defined (CC_MT5360B) || defined (CC_MT5392B) || defined (CC_IS_MPEG4_POST_MT5363)
    VLD_WRITE32(WO_VLD_WPTR, u4WPtrAlign);    //set wptr infinity
#else
    VLD_WRITE32(WO_VLD_WPTR, (u4WPtrAlign << 3) | 7);    //set wptr infinity
#endif
    return  E_VDEC_OK;
}


// *********************************************************************
// Function : BOOL _VDEC_VLDInitMpeg4BarrelShifter(DWRD dwPtr)
// Description : Init HW Barrel Shifter
// Parameter : dwPtr: Physical DRAM Start Address to fill Barrel Shifter
// Return    : TRUE: Initial Success, Fail: Initial Fail
// *********************************************************************
#ifdef CC_IS_MPEG4_POST_MT5363
BOOL _VDEC_VLDInitMpeg4BarrelShifter(UINT32 u4Ptr, VDEC_HDLR hVdec)
{
       BOOL fgFetchOK = FALSE;
       INT32 i;
	UINT32 u4Temp = 0, u4ShiftPtr, u4Cnt = 0;
	VDEC_MPEG4_DEC_PARAM_INFO_T* prVdec;

	prVdec = (VDEC_MPEG4_DEC_PARAM_INFO_T*)hVdec;

	u4ShiftPtr = u4Ptr;
       //LOG(0, "Read ptr = 0x%08x\n", u4Ptr);
	
       u4Ptr &= ~0xF;        // 16 Byte Alignment

       //u4ShiftPtr -= u4Ptr;

	VLD_WRITE32(RW_VLD_RPTR, u4Ptr);

    	/*VLD_WRITE32(RW_VLD_ASYNC, VLD_READ32(RW_VLD_ASYNC) | 0x101);
    	VLD_WRITE32(RW_VLD_ASYNC, VLD_READ32(RW_VLD_ASYNC) |0x301);
    	VLD_WRITE32(RW_VLD_ASYNC, VLD_READ32(RW_VLD_ASYNC) |0x101);*/
    	VLD_WRITE32(RW_VLD_ASYNC, VLD_READ32(RW_VLD_ASYNC) |0x1040104);
    	
       for (i = 0; i < 5; i++)
       {
           VLD_WRITE32(RW_VLD_RPTR, u4Ptr);
           VLD_WRITE32(RW_VLD_RPTR, u4Ptr);

           _VDEC_UpdateVldMpeg4WPtr(prVdec->u4WPtr);
           //_VDEC_UpdateVldMpeg4WPtr(prVdec->rPesInfo.u4FifoEnd);
           	
	    VLD_WRITE32(RW_VLD_PROC, VLD_INIFET);
	    if(_WaitMpeg4VldFetchOk())
	    {
	        fgFetchOK= TRUE;
	        break;
	    }
       }

       if (!fgFetchOK)
       {
           LOG(3, "Fetch is not ok\n");
           return FALSE;
       }

	VLD_WRITE32(RW_VLD_PROC, VLD_INIBR);

        while(1)
        {
            u4Temp = VLD_READ32(RO_VLD_VBAR);
            if (((u4Temp | 0xFFFFFFF0) == 0xFFFFFFF0) &&
                ((u4Temp & 0x3000000) == 0x3000000))
            {
                //Printf("Initialize Barrel Shifter successfully\n");
                break;
            }

            if(u4Cnt >= 0x1000000)
            {
                LOG(3, "Fail to wait Barrel Shifter\n");
                return FALSE;
            }    
            u4Cnt++;
        }
        
	
        UINT32 u4RealAddr, u4Bits, u4RestData;
        u4RealAddr = _VDEC_VldMpeg4RPtr(&u4Bits);
        if (u4ShiftPtr > u4RealAddr)
	{
		UINT32 i;

            u4RestData = u4ShiftPtr -u4RealAddr;
            for (i = 0; i < u4RestData; i++)
		{
			_VDEC_Mpeg4GetBitStream(&u4Temp, 8);
		}
	}

	return TRUE;
}
#else
BOOL _VDEC_VLDInitMpeg4BarrelShifter(UINT32 u4Ptr, VDEC_HDLR hVdec)
{
       BOOL fgFetchOK = FALSE;
       INT32 i;
	UINT32 u4Temp = 0, u4ShiftPtr;
	VDEC_MPEG4_DEC_PARAM_INFO_T* prVdec;

	prVdec = (VDEC_MPEG4_DEC_PARAM_INFO_T*)hVdec;

	u4ShiftPtr = u4Ptr;
       //LOG(0, "Read ptr = 0x%08x\n", u4Ptr);
	
       u4Ptr &= ~0xF;        // 16 Byte Alignment

       u4ShiftPtr -= u4Ptr;

	VLD_WRITE32(RW_VLD_RPTR, u4Ptr);

    	VLD_WRITE32(RW_VLD_ASYNC, VLD_READ32(RW_VLD_ASYNC) | 0x101);
    	VLD_WRITE32(RW_VLD_ASYNC, VLD_READ32(RW_VLD_ASYNC) |0x301);
    	VLD_WRITE32(RW_VLD_ASYNC, VLD_READ32(RW_VLD_ASYNC) |0x101);
    	
       for (i = 0; i < 5; i++)
       {
           VLD_WRITE32(RW_VLD_RPTR, u4Ptr);
           VLD_WRITE32(RW_VLD_RPTR, u4Ptr);

           _VDEC_UpdateVldMpeg4WPtr(prVdec->u4WPtr);
           	
	    VLD_WRITE32(RW_VLD_PROC, VLD_INIFET);
	    if(_WaitMpeg4VldFetchOk())
	    {
	        fgFetchOK= TRUE;
	        break;
	    }
       }

       if (!fgFetchOK)
       {
           return FALSE;
       }

	VLD_WRITE32(RW_VLD_PROC, VLD_INIBR);

	/*u4RealAddr = _VDEC_VldMpeg4RPtr(&u4Bits);
	if (u4ShiftPtr > u4RealAddr)*/
	{
		UINT32 i;

		//u4RestData = u4ShiftPtr -u4RealAddr;
		for (i = 0; i < u4ShiftPtr; i++)
		{
			_VDEC_Mpeg4GetBitStream(&u4Temp, 8);
		}
	}

	return TRUE;
}
#endif

// *********************************************************************
// Function : _VDEC_VLDMpeg4SetVFifo(DWRD dSa, DWRD dEa)
// Description : Set VLD VFifo Sa, Ea
// Parameter : dSa: Absolute Dram Address of VFifo Start
//             dEa: Absolute Dram Address of VFifo End
// Return    : None
// *********************************************************************
void _VDEC_VLDMpeg4SetVFifo(UINT32 u4Sa, UINT32 u4Ea)
{
	VLD_WRITE32(RW_VLD_VSTART, u4Sa >> 6);
	VLD_WRITE32(RW_VLD_VEND, u4Ea >> 6);
}

// *********************************************************************
// Function : void _VDEC_ResetMpeg4VLD1(DWRD dwRDPtrAddr)
// Description : Reset VLD1
// Parameter : u4RDPtrAddr, u4BsFifo
// Return    : None
// *********************************************************************
void _VDEC_ResetMpeg4VLD1(UINT32 u4RDPtrAddr, 
                                                           UINT32 u4BsFifoStart, 
                                                           UINT32 u4BsFifoEnd, 
                                                           VDEC_HDLR hVDec)
{
	//UINT32 u4Temp;
	// Jack's test code
	//VLD_WRITE32(RW_VLD_RDY_SWTICH, READY_TO_RISC);
  
	MC_WRITE32(RW_MC_FWDP, 0);
	MC_WRITE32(RW_MC_OPBUF, 4);
	
 	// set write ptr
  	VLD_WRITE32(WO_VLD_WPTR, 0xFFFFFFFF);	//set wptr infinity
	// Set for VLD boundary check 0x5000000
	_VDEC_VLDMpeg4SetVFifo(PHYSICAL(u4BsFifoStart), PHYSICAL(u4BsFifoEnd));
	if(!_VDEC_VLDInitMpeg4BarrelShifter(u4RDPtrAddr, hVDec))
	{
		/*UINT32 u4Temp;
		
		LOG(0, "Barrel Shifter Init failed");
		u4Temp = VLD_READ32(RO_VLD_FSBI);
		LOG(0, "RO_VLD_FSBI (%d) = 0x%x\n", (RO_VLD_FSBI>>2), u4Temp);
		u4Temp = VLD_READ32(RO_VLD_ERRFG);
		LOG(0, "RO_VLD_ERRFG (%d) = 0x%x\n", (RO_VLD_ERRFG>>2), u4Temp);
		u4Temp = VLD_READ32(RO_VLD_FETCHOK);
		LOG(0, "RO_VLD_FETCHOK (%d) = 0x%x\n", (RO_VLD_FETCHOK>>2), u4Temp);
		u4Temp = VLD_READ32(RO_VLD_VBAR);
		LOG(0, "RO_VLD_VBAR (%d) = 0x%x\n", (RO_VLD_VBAR>>2), u4Temp);
		u4Temp = VLD_READ32(RO_VLD_SRAMCTRL);
		LOG(0, "RO_VLD_SRAMCTRL (%d) = 0x%x\n", (RO_VLD_SRAMCTRL>>2), u4Temp);
		u4Temp = VLD_READ32(RO_VLD_VWPTR);
		LOG(0, "RO_VLD_VWPTR (%d) = 0x%x\n", (RO_VLD_VWPTR>>2), u4Temp);
		u4Temp = VLD_READ32(RO_VLD_VRPTR);
		LOG(0, "RO_VLD_VRPTR (%d) = 0x%x\n", (RO_VLD_VRPTR>>2), u4Temp);*/
	}
	//_VDEC_VLDMpeg4SetVFifo(u4BsFifoStart, u4BsFifoEnd);

	/*u4Temp = VLD_READ32(0xFC);
	LOG(0, "_VDEC_ResetMpeg4VLD1 Dram RP =  0x%8x\n", u4Temp);

	u4Temp = (VLD_READ32(0xEc) & 0x3f);
	LOG(0, "_VDEC_ResetMpeg4VLD1 Sram RP =  0x%8x\n", u4Temp);

	u4Temp = ((VLD_READ32(0xEC) >> 16) & 0x3f);
	LOG(0, "_VDEC_ResetMpeg4VLD1 Sram WP =  0x%8x\n", u4Temp);

	u4Temp = (VLD_READ32(0xEc) & 0x3f);
	LOG(0, "_VDEC_ResetMpeg4VLD1 Sram RP =  0x%8x\n", u4Temp);

	u4Temp = VLD_READ32(0xE0);
	LOG(0, "_VDEC_ResetMpeg4VLD1 RO_VLD_FSBI (%d) = 0x%x\n", (0xE0>>2), u4Temp);

	u4Temp = VLD_READ32(0xE4);
	LOG(0, "_VDEC_ResetMpeg4VLD1 RO_VLD_ERRFG (%d) = 0x%x\n", (0xE4>>2), u4Temp);

	u4Temp = VLD_READ32(0xE8);
	LOG(0, "_VDEC_ResetMpeg4VLD1 RO_VLD_FETCHOK (%d) = 0x%x\n", (0xE8>>2), u4Temp);

	u4Temp = VLD_READ32(0xEC);
	LOG(0, "_VDEC_ResetMpeg4VLD1 RO_VLD_VBAR (%d) = 0x%x\n", (0xEC>>2), u4Temp);

	u4Temp = VLD_READ32(0xF4);
	LOG(0, "_VDEC_ResetMpeg4VLD1 RO_VLD_SRAMCTRL (%d) = 0x%x\n", (0xF4>>2), u4Temp);

	u4Temp = VLD_READ32(0xF8);
	LOG(0, "_VDEC_ResetMpeg4VLD1 RO_VLD_VWPTR (%d) = 0x%x\n", (0xF8>>2), u4Temp);*/


	/*if(!_VDEC_VLDInitMpeg4BarrelShifter(u4RDPtrAddr))
	{
		LOG(0, "Barrel Shifter Init failed");
	}*/

	//VLD_WRITE32(RW_VLD_ASYNC, 0X01040100);	//set wptr infinity
}


BOOL _VDEC_GetVldRPtr(UINT32* pu4Addr, UINT32* pu4Bits, UINT32 u4FifoStart, UINT32 u4FifoEnd)
{
	UINT32	u4DramRptr;
	UINT32 	u4SramRptr, u4SramWptr;
	UINT32 	u4SramDataSz;

	if(!_WaitMpeg4VldFetchOk())
	{
		return FALSE;
	}

	u4DramRptr = _VLDDramRp();
	u4SramRptr = _VLDSramRp();
	u4SramWptr = _VLDSramWp();

	if(u4SramWptr >= u4SramRptr)
	{
		u4SramDataSz = u4SramWptr - u4SramRptr;
	}
	else
	{
		u4SramDataSz = 64 - (u4SramRptr - u4SramWptr);
	}

	
	*pu4Bits = VLD_READ32(RO_VLD_SUM) & 0x3f;
	*pu4Addr = u4DramRptr - (u4SramDataSz + 4) * 4 + (*pu4Bits / 8);
	
	*pu4Bits &= 0x7;

	return TRUE;

}


VOID _VDEC_VLDMpeg4WaitForSramStable(VOID)
{
    UINT32 u4Cnt = 0;
    
    while((VLD_READ32(RO_VLD_SRAMCTRL) & AA_FIT_TARGET_D)==0)
    {
        u4Cnt++;
        if(u4Cnt >= 0x100000)
        {
            LOG(0, "//===SRAM !Stable Fail 0xF4=0x%x, Rptr 0xB0=0x%x, 0xFC=0x%x, 0xEC=0x%x, 0xE8=0x%x\n", 
                VLD_READ32(RO_VLD_SRAMCTRL), VLD_READ32(RW_VLD_RPTR), VLD_READ32(RO_VLD_VRPTR),
                VLD_READ32(RO_VLD_VBAR), VLD_READ32(RO_VLD_FETCHOK));
            break;
        }    
    }
}


// *********************************************************************
// Function : void _VDEC_VLDMpeg4HWReset(void)
// Description : Reset Video decode HW
// Parameter : None
// Return    : None
// *********************************************************************
void _VDEC_VLDMpeg4HWReset(void)
{
    	UINT32 u4RegVal;

	VLD_WRITE32(WO_VLD_SRST, 1);
	VLD_WRITE32(WO_VLD_SRST, 0);
	u4RegVal = (VLD_READ32(RW_VLD_VDOUFM) & 0xFFFFF7FF) | (0x1 << 11);
       VLD_WRITE32(RW_VLD_VDOUFM, u4RegVal);        

}

// *********************************************************************
// Function : void _VDEC_Mpeg4SetFrmBufPtr(DWRD dBufIdx, DWRD dPtrY, DWRD dPtrC)
// Description : Set Frame Buffer Pointer
// Parameter : u4BufIdx: MP4_REF_BUF_0/1, MPV_B_BUF_0/1
//             u4PtrY: Physical Dram Address for Y
//             u4PtrC: Physical Dram Address for C
// Return    : None
// *********************************************************************
void _VDEC_Mpeg4SetFrmBufPtr(UINT32 u4BufIdx, UINT32 u4PtrY, UINT32 u4PtrC)
{
  switch(u4BufIdx)
  {
    case MP4_REF_BUF_0:
      MC_WRITE32(RW_MC_R1Y, (u4PtrY >> 9)); // div 512
      MC_WRITE32(RW_MC_R1C, (u4PtrC >> 8)); // div 256
      break;
    case MP4_REF_BUF_1:
      MC_WRITE32(RW_MC_R2Y, (u4PtrY >> 9)); // div 512
      MC_WRITE32(RW_MC_R2C, (u4PtrC >> 8)); // div 256
      break;
    case MP4_B_BUF_0:
      MC_WRITE32(RW_MC_BY1,  (u4PtrY >> 9)); // div 512
      MC_WRITE32(RW_MC_BC1,  (u4PtrC >> 8)); // div 256
      break;
    case MP4_B_BUF_1:
      MC_WRITE32(RW_MC_BY2,  (u4PtrY >> 9)); // div 512
      MC_WRITE32(RW_MC_BC2,  (u4PtrC >> 8)); // div 256
      break;
    case MP4_MC_DBK_BUF: // current dbk decode frame (I, P, B)
      MC_WRITE32(RW_MC_DBK_Y,  u4PtrY >> 9);
      MC_WRITE32(RW_MC_DBK_C,  u4PtrC >> 8);
      break;
    default:
    	break;
  }
}


void _VDEC_Mpeg4McSetDecFld(UCHAR ucDecFld, UCHAR uc2ndFldSw)
{

    if(ucDecFld == MP4_MC_TOP_FLD)
    {
        MC_WRITE32(RW_MC_DECFLD, 0);
    }
    else
    {
        MC_WRITE32(RW_MC_DECFLD, 1);
    }
    if(uc2ndFldSw == MP4_MC_2ND_FLD_ON)
    {
        MC_WRITE32(RW_MC_2FLDMD, 1);
    }
    else
    {
        MC_WRITE32(RW_MC_2FLDMD, 0);
    }

}


#ifdef CC_MPEG4_DEBLOCKING
void _SetMpeg4DbkPrmProc(VDEC_MPEG4_DEC_PARAM_INFO_T *prDecPrm)
{
    UCHAR ucFRefFbId;
    UCHAR ucBRefFbId;
    UINT16 u2HSizeMB;
    UINT16 u2HSizeMBtoMC;
    UINT16 u2VSize;
    UINT32 u4DbkPicSz;
    UINT32 u4QP0_Y;
    UINT32 u4QP1_Y;
    UINT32 u4QP2_Y;
    UINT32 u4QP3_Y;    
    UINT32 u4QP0_C;
    UINT32 u4QP1_C;
    UINT32 u4QP2_C;
    UINT32 u4QP3_C;    
    UINT32 u4FtrCnt;
    UINT32 u4FtrThd;
    UINT32 u4OfstCnt;
    UINT32 u4pCnt;    
    UINT32 u4FRefOfstCnt;
    UINT32 u4FRef4pCnt;
    UINT32 u4BRefOfstCnt;
    UINT32 u4BRef4pCnt;
    UINT32 u4RegQpY;
    UINT32 u4RegQpC;
    UINT32 u4DbkRegion;
    UINT32 u4RegVal;
    UINT32 u4DbkReg = 0;
    VDEC_ES_INFO_T *prVdecEsInfo;

    prVdecEsInfo = _VDEC_GetEsInfo(ES0);

    //if (prFbmSeqHdr->u2HSize > DBK_MIN_H)
    {
        FBM_GetRefFrameBuffer(prDecPrm->u1FbgId, &ucFRefFbId, &ucBRefFbId);
        FBM_GetFrameBufferFtrCnt(prDecPrm->u1FbgId, ucFRefFbId, &u4OfstCnt, &u4pCnt);
        u4FRefOfstCnt = u4OfstCnt;
        u4FRef4pCnt = u4pCnt;

        FBM_GetFrameBufferFtrCnt(prDecPrm->u1FbgId, ucBRefFbId, &u4OfstCnt, &u4pCnt);
        u4BRefOfstCnt = u4OfstCnt;
        u4BRef4pCnt = u4pCnt;

        u2HSizeMB = (prDecPrm->u2Width + 15) >> 4;
        u2HSizeMBtoMC = ((UINT16)prDecPrm->u4LineSize + 15) >> 4;        
        u2VSize =  MPEG4_ALIGN_MASK(prDecPrm->u2Height, 0xf);
        
        u4FtrCnt = u2HSizeMB * u2VSize * 6;
        
        if (!prDecPrm->u4P11)
        {
            u4FtrThd = (u4FtrCnt * DBK_FTR_RATIO)/10;
        }
        else
        {
            u4FtrThd = (u4FtrCnt *prDecPrm->u4P12)/10;
            MC_WRITE32(RW_MC_DBK_THRSD, (prDecPrm->u4P11<<16)|0x0202);        
            MC_WRITE32(RW_MC_DBK_ADV_THRSD_Y, 
                (prDecPrm->u4P20<<28)|(prDecPrm->u4P19<<24)|(prDecPrm->u4P18<<20)|
                (prDecPrm->u4P17<<16)|(prDecPrm->u4P16<<12)|(prDecPrm->u4P15<<8)|
                (prDecPrm->u4P14<<4)|prDecPrm->u4P13);                        
            MC_WRITE32(RW_MC_DBK_ADV_THRSD_C, 
                (prDecPrm->u4P20<<28)|(prDecPrm->u4P19<<24)|(prDecPrm->u4P18<<20)|
                (prDecPrm->u4P17<<16)|(prDecPrm->u4P16<<12)|(prDecPrm->u4P15<<8)|
                (prDecPrm->u4P14<<4)|prDecPrm->u4P13);                        
        }

        if (!prDecPrm->rM4vParams.rVol.fgInterlaced)
        {
            u4DbkPicSz =  ((u2HSizeMB << 16) + ((u2VSize >> 4) << 8) + u2HSizeMB);
        }
        else
        {
            u4DbkPicSz =  ((u2HSizeMB << 16) + ((u2VSize >> 5) << 8) + u2HSizeMB);
        }


        u4DbkReg = MC_READ32(RW_MC_DBK_CTRL);

        if (prDecPrm->u1VopCodingType == VOP_CODING_TYPE_B)
        {
            u4DbkReg |= (DBK_EN);
        }
        else
        {
           u4DbkReg |= DBK_EN ;
        }

        if (prDecPrm->u1VopCodingType == VOP_CODING_TYPE_B)
        {
            u4DbkReg |= 0/*DBK_BW_SAVING*/;        
        }

        u4DbkReg |= (DBK_DISABLE_DELAY);
        //u4DbkReg |= (1 << 2);
            

        LOG(7,"dbk reg 0x%x\n", u4DbkReg);        

        MC_WRITE32(RW_MC_DBK_CTRL, u4DbkReg);
        
        MC_WRITE32(RW_MC_DBK_PICSZ, u4DbkPicSz);

        if (prDecPrm->u4P1)
        {
            if (prDecPrm->u1VopCodingType == VOP_CODING_TYPE_I)
            {
                u4QP0_Y = prDecPrm->u4P1;    
                u4QP0_C = prDecPrm->u4P1;    
            }
            else if ((prDecPrm->u1VopCodingType == VOP_CODING_TYPE_P) && 
                ((u4FRefOfstCnt + u4FRef4pCnt) >= u4FtrThd))
            {
                u4QP0_Y = prDecPrm->u4P2;        
                u4QP0_C = prDecPrm->u4P2;            
            }
            else if ((prDecPrm->u1VopCodingType == VOP_CODING_TYPE_B) && 
                ((u4FRefOfstCnt + u4FRef4pCnt + u4BRefOfstCnt + u4BRef4pCnt) >= (u4FtrThd << 1)))
            {
                u4QP0_Y = prDecPrm->u4P3;    
                u4QP0_C = prDecPrm->u4P3;            
            }
            else
            {
                u4QP0_Y = prDecPrm->u4P4;        
                u4QP0_C = prDecPrm->u4P4;        
            }

            u4QP1_Y = u4QP0_Y + prDecPrm->u4P5;
            u4QP2_Y = u4QP1_Y + prDecPrm->u4P6;
            u4QP3_Y = u4QP2_Y + prDecPrm->u4P7;

            u4QP1_C = u4QP0_C + prDecPrm->u4P8;
            u4QP2_C = u4QP1_C + prDecPrm->u4P9;
            u4QP3_C = u4QP2_C + prDecPrm->u4P10;
        }
        else
        {
            if (prDecPrm->u1VopCodingType == VOP_CODING_TYPE_I)
            {
                u4QP0_Y = DBK_THQP0I;    
                u4QP0_C = DBK_THQP0I_C;    
            }
            else if ((prDecPrm->u1VopCodingType == VOP_CODING_TYPE_P) && 
                ((u4FRefOfstCnt + u4FRef4pCnt) >= u4FtrThd))
            {
                u4QP0_Y = DBK_THQP0P;        
                u4QP0_C = DBK_THQP0P_C;            
            }
            else if ((prDecPrm->u1VopCodingType == VOP_CODING_TYPE_B) && 
                ((u4FRefOfstCnt + u4FRef4pCnt + u4BRefOfstCnt + u4BRef4pCnt) >= (u4FtrThd << 1)))
            {
                u4QP0_Y = DBK_THQP0B;    
                u4QP0_C = DBK_THQP0B_C;            
            }
            else
            {
                u4QP0_Y = DBK_THQP0;        
                u4QP0_C = DBK_THQP0_C;        
            }

            u4QP1_Y = u4QP0_Y + DBK_QP1_Y;
            u4QP2_Y = u4QP1_Y + DBK_QP2_Y;
            u4QP3_Y = u4QP2_Y + DBK_QP3_Y;

            u4QP1_C = u4QP0_C + DBK_QP1_C;
            u4QP2_C = u4QP1_C + DBK_QP2_C;
            u4QP3_C = u4QP2_C + DBK_QP3_C;
        }

        if (u4QP2_C > DBK_QP_MAX)
        {
            u4QP2_C = DBK_QP_MAX;
        }

        if (u4QP3_C > DBK_QP_MAX)
        {
            u4QP3_C = DBK_QP_MAX;
        }

        u4RegQpY = (u4QP3_Y << 24) | (u4QP2_Y << 16) | (u4QP1_Y << 8) | (u4QP0_Y);
        u4RegQpC = (u4QP3_C << 24) | (u4QP2_C << 16) | (u4QP1_C << 8) | (u4QP0_C);

        MC_WRITE32(RW_MC_DBK_QP_Y, u4RegQpY);        
        MC_WRITE32(RW_MC_DBK_QP_C, u4RegQpC);        

        if (((prDecPrm->u4DbkXEnd == 0) || (prDecPrm->u4DbkYEnd == 0)) &&
            ((prDecPrm->u2DbkWidthT == 0) || (prDecPrm->u2DbkHeightT)))
        {
            prDecPrm->u4DbkXStart= 0;
            prDecPrm->u4DbkXEnd = prDecPrm->u2Width >> 4;
            prDecPrm->u4DbkYStart= 0;
            prDecPrm->u4DbkYEnd = prDecPrm->u2Height >> 4;        
        }
        else if ((prDecPrm->u2DbkWidthT != 0) && (prDecPrm->u2DbkHeightT != 0))
        {
            prDecPrm->u4DbkXStart = ((prDecPrm->u2DbkXStartT * prDecPrm->u2Width) / 1000) >> 4;
            prDecPrm->u4DbkYStart = ((prDecPrm->u2DbkYStartT * prDecPrm->u2Height) / 1000) >> 4;
            prDecPrm->u4DbkXEnd = (((prDecPrm->u2DbkXStartT + prDecPrm->u2DbkWidthT) * prDecPrm->u2Width) / 1000) >> 4;
            prDecPrm->u4DbkYEnd = (((prDecPrm->u2DbkYStartT + prDecPrm->u2DbkHeightT) * prDecPrm->u2Height) / 1000) >> 4;
        }

        u4DbkRegion= ((prDecPrm->u4DbkYEnd << 24) |(prDecPrm->u4DbkYStart << 16)|
                     (prDecPrm->u4DbkXEnd << 8)|(prDecPrm->u4DbkXStart));       
                                    
        MC_WRITE32(RW_MC_DBK_INTEREST, u4DbkRegion);        

        if (prDecPrm->fgDbkEngrMode)
        {
            u4RegVal = _MPV_McRead32(RW_MC_DBK_DEBUG);            
            MC_WRITE32(RW_MC_DBK_DEBUG, (u4RegVal|FPF_EN|DEF_EN|OFST_EN));                
        }
        
        MC_WRITE32(WO_MC_DBK_DEC, DBK_START);    
    }
}
#endif
