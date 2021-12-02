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
 * $RCSfile: vdec_mpeg4dec.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_mpeg4dec.c
 *  This file contains implementation of mpeg4 decoder.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
#include "vdec_mpeg4hw.h"
#include "vdec_mpeg4util.h"
#include "vdec_mpeg4dec.h"
#include "vdec_mpeg4api.h"
LINT_EXT_HEADER_BEGIN
#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "x_common.h"
#include "x_typedef.h"
#include "x_util.h"
LINT_EXT_HEADER_END

//-----------------------------------------------------------------------------
// Static Variables
//-----------------------------------------------------------------------------
static HANDLE_T _hMpeg4VdecFinSema = NULL;
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static void _VDEC_DecM4VProc(VDEC_MPEG4_DEC_PARAM_INFO_T* prDecPrm
								, UINT32 u4MBx, INT32* pi4RetValue);
/*static void _VDEC_DecH263Proc(VDEC_MPEG4_DEC_PARAM_INFO_T* prDecPrm
								, INT32* pi4RetValue);*/

static void _SetMpeg4McDecBuf(VDEC_MPEG4_DEC_PARAM_INFO_T* prDecPrm);
#ifdef CC_MPEG4_DEBLOCKING
static void _SetMpeg4McDbkDecBuf(VDEC_MPEG4_DEC_PARAM_INFO_T* prDecPrm);
#endif
static void _SetMpeg4DecFld(VDEC_MPEG4_DEC_PARAM_INFO_T* prDecPrm);
static void _VLDSetPicSize(VDEC_MPEG4_DEC_PARAM_INFO_T* prDecPrm
								, INT32* pi4RetValue);
static void _VLDSetMBRowPrm(VDEC_MPEG4_DEC_PARAM_INFO_T *prDecPrm
								, INT32* pi4RetValue);
/*static int Mpeg4T32HostExec(unsigned int uTargetFlag, char *szFormat, ...);
static void _CopyRefBuf(UINT32 u4ForwardAddY, UINT32 u4ForwardAddC, 
						   UINT32 u4AddrY, UINT32 u4AddrC, UINT32 u4YSize, UINT32 u4CSize);*/

static void _Mpeg4VldIsr(UINT16 u2Vector);


// **************************************************************************
// Function : void _VDEC_InitMpeg4VDecHW(DWRD dwRDPtrAddr);
// Description : Init Video Decoder
// Parameter : None
// Return      : TRUE: init OK.
//                   FALSE: init failed
// **************************************************************************
void _VDEC_InitMpeg4VDecHW(UINT32 u4RDPtrAddr, 
                                                            UINT32 u4BsFifoStart, 
								    UINT32 u4BsFifoEnd, 
								    BOOL fgSetAddress, 
								    VDEC_HDLR hVDec)
{
#ifdef EN_IRQ
	if (!_hMpeg4VdecFinSema)
	{
		VERIFY(x_sema_create(&_hMpeg4VdecFinSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
	}
#endif 

	_VDEC_VLDMpeg4HWReset();

	if (fgSetAddress)
	{
		_VDEC_ResetMpeg4VLD1(u4RDPtrAddr, u4BsFifoStart, u4BsFifoEnd, hVDec);
	}
  
}

// *********************************************************************
// Function    : void vSetVLDNum(BYTE bNum)
// Description : Set VLD related Num
// Parameter   : BYTE bNum
// Return      : None
// *********************************************************************
void _VDEC_SetMpeg4VLDNum(UINT8 u1Num, UINT32 u4BsFifoS, UINT32 u4BsFifoE)
{  
	UNUSED(u1Num);
	UNUSED(u4BsFifoS);
	UNUSED(u4BsFifoE);	
}


// **************************************************************************
// Function : void _VDEC_DecProc(void);
// Description : Init Video Decoder
// Parameter : None
// Return      : 
// **************************************************************************
void _VMpeg4DEC_DecProc(VDEC_HDLR hVdec, INT32* pi4RetValue)
{
    UINT32	u4MBx;
    UINT32	u4RegVal1 = 0, u4Temp = 0;
    UINT32	u4Mp4Hdr;

    VDEC_MPEG4_DEC_PARAM_INFO_T *prDecPrm = (VDEC_MPEG4_DEC_PARAM_INFO_T*)hVdec;

    VDEC_M4V_DEC_PARAM_INFO_T *prM4vDecPrm = &(prDecPrm->rM4vParams);
    VDEC_M4V_VOL_INFO_T *prVolPrm = &(prM4vDecPrm->rVol);
    VDEC_M4V_PIC_INFO_T *prVopPrm = &(prM4vDecPrm->rVop);
    VDEC_M4V_DIRECT_MODE_INFO_T *prDirMdPrm = &(prVopPrm->rDirectMd);
    VDEC_M4V_GMC_INFO_T *prGmcPrm = &(prVopPrm->rGmcParam);

    ASSERT(prDecPrm != NULL);
    LOG(7, "Enter _VMpeg4DEC_DecProc");

    MC_WRITE32(RW_MC_HREFP, 0);

    if (prDecPrm->u1FbgId == FBM_FBG_ID_UNKNOWN ||
        prDecPrm->u1FbId == FBM_FBG_ID_UNKNOWN)
    {
        LOG(3, "FBM is not ready\n");
        *pi4RetValue = E_VDEC_FAIL;
        return;
    }
        

    _SetMpeg4McDecBuf(prDecPrm);
#ifdef CC_IS_MPEG4_POST_MT5363
#ifdef CC_MPEG4_DEBLOCKING
    if (prDecPrm->u1VideoType == VIDEO_TYPE_FLV)
    {
        _SetMpeg4McDbkDecBuf(prDecPrm);
        _SetMpeg4DbkPrmProc(prDecPrm);
    }
#endif
#endif

    /*if (prDecPrm->fgVopNotCoded == TRUE)
    {
    	LOG(7, "This vop is not coded\n");
    	return;
    }*/

    VLD_WRITE32(RW_VLD_MP4_FLG, 1);

    u4MBx = (UINT32)((prDecPrm->u2Width + 15) / 16);

    u4RegVal1 = ((UINT32)prVopPrm->fgTopFldFirst << 26) +
              ((UINT32)prDecPrm->u1VopCodingType << 21) +
              ((UINT32)prVopPrm->u1ForwardFCode << 12) +
              ((UINT32)prVopPrm->u1BackwardFCode << 4);

    VLD_WRITE32(RW_VLD_PARA, u4RegVal1);

    _VLDSetPicSize(prDecPrm, pi4RetValue);
    _VLDSetMBRowPrm(prDecPrm, pi4RetValue);

    _VDEC_DecM4VProc(prDecPrm, u4MBx, pi4RetValue);

    _SetMpeg4DecFld(prDecPrm);

    MC_WRITE32(RW_MC_ADDRSWAP, prDecPrm->u4AddrMode);
#ifdef CC_IS_MPEG4_POST_MT5363
    MC_WRITE32(RW_MC_NBM_ADDRSWAP, prDecPrm->u4AddrMode);
    //u4Temp = MC_READ32(0x71C);
    //u4Temp &= (0x88FFFFFF);
    //MC_WRITE32(0x71C, u4Temp);
    u4Temp = MC_READ32(0x5E4);
    u4Temp |= (0x1000);
    MC_WRITE32(0x5E4, u4Temp);
#endif

    VLD_WRITE32(RW_VLD_SCALE, H_SCALE_1_1);
    VLD_WRITE32(RW_VLD_DIGMBYOFF, 0);

    u4Mp4Hdr = (prVolPrm->fgResyncMarkerDisable << 26) +
             (prVolPrm->fgQuarterSample << 25) +
             (prVolPrm->u1QuantType << 24) +
             (prVolPrm->fgInterlaced << 22) +
             (prVolPrm->fgDataPartitioned << 21) +
             (prVolPrm->fgReversibleVlc << 20) +
             (prVolPrm->fgShortHeader << 19) +
             (prVolPrm->u4TimeIncrementResolution);

#ifdef CC_IS_MPEG4_POST_MT5363
    if (prDecPrm->u1VideoType == VIDEO_TYPE_FLV)
    {
        u4Mp4Hdr += (1 << 23);
    }
#endif

    VLD_WRITE32(RW_VLD_MP4_HDR, u4Mp4Hdr);
    VLD_WRITE32(RW_VLD_M4SH, prVopPrm->u1SourceFormat);

    // HHKuo's
    VLD_WRITE32(RW_VLD_DIRE_MD, prDirMdPrm->u4Trb | (prDirMdPrm->u4Trd << 16));
    //  vWriteVLD(RW_VLD_BMB1, (dwAbsDramANc(BMB1_SA) >> 2) | ((dMBx * 4) << 22));
    //  vWriteVLD(RW_VLD_BMB2, (dwAbsDramANc(MVEC_SA) >> 2) | ((dMBx * 4) << 22));
    //  vWriteVLD(RW_VLD_BCODE_SA, (dwAbsDramANc(BCODE_SA) >> 2) | ((dMBx * 4) << 22));
    if(prVolPrm->fgInterlaced)
    {
    	VLD_WRITE32(RW_VLD_DIRE_MD_IL, prDirMdPrm->u4Trbi | (prDirMdPrm->u4Trdi << 16));
    }
    else
    {
        VLD_WRITE32(RW_VLD_DIRE_MD_IL, 0);
    }

    // CCJu's
    MC_WRITE32(RW_MC_WARP_POINT_NUM, prVopPrm->u1EffectiveWarpingPoints);
    MC_WRITE32(RW_MC_GMC_Y_MV, (UINT32)((prGmcPrm->i4GmcYMvX << 16) | (prGmcPrm->i4GmcYMvY & 0xffff)));
    MC_WRITE32(RW_MC_GMC_C_MV, (UINT32)((prGmcPrm->i4GmcCMvX << 16) | (prGmcPrm->i4GmcCMvY & 0xffff)));

    // DivX3 only registers...
    // Maybe it's not necessary but clear them anyway
    VLD_WRITE32(RW_VLD_DX3, 0);

    u4RegVal1 = 0;
    if (prDecPrm->u1VideoType == VIDEO_TYPE_ISO_MPEG4 || 
         	prDecPrm->u1VideoType == VIDEO_TYPE_DIVX_MPEG4 ||
    	 prDecPrm->u1VideoType == VIDEO_TYPE_H263 ||
    	 prDecPrm->u1VideoType == VIDEO_TYPE_FLV)
    {
    	u4RegVal1 = (UINT32)(1 << 6);

    	if (prDecPrm->u1VideoType == VIDEO_TYPE_H263 ||
    	    prDecPrm->u1VideoType == VIDEO_TYPE_FLV)
    	{
    		u4RegVal1 |= 1 << 7;
    	}
    }
    else if (prDecPrm->u1VideoType == VIDEO_TYPE_DX311)
    {
        u4RegVal1 = (UINT32)(1 << 5);
    }

	if (prDecPrm->u1VopCodingType == VOP_CODING_TYPE_I)
      	{
      		u4RegVal1 |= (1 << 0);
	}
    	else if (prDecPrm->u1VopCodingType == VOP_CODING_TYPE_P)
      	{
      		u4RegVal1 |= (1 << 1);
    	}
    	else if (prDecPrm->u1VopCodingType == VOP_CODING_TYPE_B)
     	{
     		u4RegVal1 |= (1 << 2);
    	}
    	else if (prDecPrm->u1VopCodingType == VOP_CODING_TYPE_S)
      	{
      		u4RegVal1 |= (1 << 3);
    	}

	VLD_WRITE32(RW_VLD_VOP_TYPE, u4RegVal1);

	VLD_WRITE32(RW_VLD_VDOUFM, VLD_VDOUFM + VLD_ERR_MONITOR);

	u4RegVal1 = 0;
	u4RegVal1 |= VLD_PDHW;
	if(prDecPrm->u1VopCodingType == VOP_CODING_TYPE_B)
	{
		// check Partial Decode
		if(prDecPrm->u4DecXOff != 0)
		{
			u4Temp = ((prDecPrm->u4DecXOff >> 4) << 8) +
			      ((prDecPrm->u4DecXOff + prDecPrm->u2Width) >> 4);
			if(((prDecPrm->u4DecXOff + prDecPrm->u2Width) & 0x0F) != 0)
			{
				u4Temp += 1;
			}
		}
		else
		{
			u4Temp = (prDecPrm->u2Width >> 4);
			if((prDecPrm->u2Width & 0x0F) != 0)
			{
				u4Temp += 1;
			}
		}
		u4RegVal1 |= u4Temp;
	}
	else
	{
		// I, P shall decode whole picture
		u4RegVal1 |= (prDecPrm->u2Width >> 4);
	}

       u4Temp = 0;
	if (prDecPrm->u1VopCodingType == VOP_CODING_TYPE_B)
	{
		if (prDecPrm->u1BackwardRefVopType != VOP_CODING_TYPE_P)
		{
			u4Temp = 1;
		}
	}

       u4Temp |= (1<<8);
       VLD_WRITE32(RW_VLD_NON_P, u4Temp);

	if (prVolPrm->u1QuantType)
	{
		UINT32 u4ReloadQ = 0;
		
		if (prVolPrm->fgLoadIntraQuantMat)
		{
			u4ReloadQ |= VLD_RELOAD_INTRA;
		}
		if (prVolPrm->fgLoadNonIntraQuantMat)
		{
			u4ReloadQ |= VLD_RELOAD_NONINTRA;
		}

		VLD_WRITE32(WO_VLD_FDEC, u4ReloadQ);
	}

	// Turn Off Racing Mode
	u4RegVal1 |= (0x1 << 27);
	
	// Process Decode
	VLD_WRITE32(RW_VLD_PROC, u4RegVal1);

#ifdef OutputDecSettings
	u4Temp = VLD_READ32(0xF0);
	LOG(0, "Decoder's Input Window: 0x%8x\n", u4Temp);

	u4Temp = MC_READ32(RW_MC_QPEL_TYPE);
	LOG(0, "MC_QPEL_TYPE (%d) = 0x%x\n", (RW_MC_QPEL_TYPE>>2), u4Temp);
	
	u4Temp = MC_READ32(RW_MC_QUARTER_SAMPLE);
	LOG(0, "MC_QUARTER_SAMPLE (%d) = 0x%x\n", (RW_MC_QUARTER_SAMPLE>>2), u4Temp);

	u4Temp = MC_READ32(RW_MC_ROUNDING_CTRL);
	LOG(0, "ROUNDING_CTRL (%d) = 0x%x\n", (RW_MC_ROUNDING_CTRL>>2), u4Temp);

	u4Temp = MC_READ32(RW_MC_UMV_PIC_WIDTH);
	LOG(0, "MC_UMV_PIC_WIDTH (%d) = 0x%x\n", (RW_MC_UMV_PIC_WIDTH>>2), u4Temp);

	u4Temp = MC_READ32(RW_MC_UMV_PIC_HEIGHT);
	LOG(0, "MC_UMV_PIC_HEIGHT (%d) = 0x%x\n", (RW_MC_UMV_PIC_HEIGHT>>2), u4Temp);

	u4Temp = MC_READ32(RW_MC_WARP_POINT_NUM);
	LOG(0, "MC_WARP_POINT_NUM (%d) = 0x%x\n", (RW_MC_WARP_POINT_NUM>>2), u4Temp);

	u4Temp = MC_READ32(RW_MC_GMC_Y_MV);
	LOG(0, "MC_GMC_Y_MV (%d) = 0x%x\n", (RW_MC_GMC_Y_MV>>2), u4Temp);

	u4Temp = MC_READ32(RW_MC_GMC_C_MV);
	LOG(0, "MC_GMC_C_MV (%d) = 0x%x\n",  (RW_MC_GMC_C_MV>>2), u4Temp);

	u4Temp = MC_READ32(RW_MC_CBCR_MV_TYPE);
	LOG(0, "MC_CBCR_MV_TYPE (%d) = 0x%x\n", (RW_MC_CBCR_MV_TYPE>>2), u4Temp);

	u4Temp = VLD_READ32(RW_VLD_PARA);
	LOG(0, "VLD_PARA (%d) = 0x%x\n", (RW_VLD_PARA>>2), u4Temp);

	u4Temp = VLD_READ32(RW_VLD_PROC);
	LOG(0, "VLD_PROC (%d) = 0x%x\n", (RW_VLD_PROC>>2), u4Temp);

	u4Temp = VLD_READ32(RW_VLD_PICSZ);
	LOG(0, "VLD_PICSZ (%d) = 0x%x\n", (RW_VLD_PICSZ>>2), u4Temp);

	u4Temp = VLD_READ32(RW_VLD_MBROWPRM);
	LOG(0, "VLD_MBROWPRM (%d) = 0x%x\n", (RW_VLD_MBROWPRM>>2), u4Temp);

	u4Temp = VLD_READ32(RW_VLD_MP4_HDR);
	LOG(0, "VLD_MP4_HDR (%d) = 0x%x\n", (RW_VLD_MP4_HDR>>2), u4Temp);

	u4Temp = VLD_READ32(RW_VLD_MP4_FLG);
	LOG(0, "VLD_MP4_FLG (%d) = 0x%x\n", (RW_VLD_MP4_FLG>>2), u4Temp);

	u4Temp = VLD_READ32(RW_VLD_QUANT);
	LOG(0, "VLD_QUANT (%d) = 0x%x\n", (RW_VLD_QUANT>>2), u4Temp);

	u4Temp = VLD_READ32(RW_VLD_DCACPITCH);
	LOG(0, "VLD_DCACPITCH (%d) = 0x%x\n", (RW_VLD_DCACPITCH>>2), u4Temp);

	u4Temp = VLD_READ32(RW_VLD_DIRE_MD);
	LOG(0, "VLD_DIRE_MD (%d) = 0x%x\n", (RW_VLD_DIRE_MD>>2), u4Temp);

	u4Temp = VLD_READ32(RW_VLD_DIRE_MD_IL);
	LOG(0, "VLD_DIRE_MD_IL (%d) = 0x%x\n", (RW_VLD_DIRE_MD_IL>>2), u4Temp);

#endif

    HalFlushInvalidateDCache();

#ifdef IC_MPEG4_VERIFICATION
    u4Temp = MC_READ32(0x714);
    LOG(0, "0x%8x (Register %ld) =  0x%8x before decoding\n", 0x714, (0x714>>2), u4Temp);
#endif

    // Trigger Decoder
    VLD_WRITE32(RW_VLD_DECSTART, 1);
    VLD_WRITE32(RW_VLD_DECSTART, 0);

    LOG(7, "Enter _VMpeg4DEC_DecProc Succeed. Trigger decoder");
}

static void _VDEC_DecM4VProc(VDEC_MPEG4_DEC_PARAM_INFO_T* prDecPrm 
								, UINT32 u4MBx, INT32* pi4RetValue)
{
	UINT32 u4DcacPitch, u4NonPBit, u4Temp;

	VDEC_M4V_DEC_PARAM_INFO_T *prM4vDecPrm;
	VDEC_M4V_VOL_INFO_T *prVolPrm;

	ASSERT(pi4RetValue != NULL);
	ASSERT(prDecPrm != NULL);

	LOG(7, "Enter _VDEC_DecM4VProc");

	prM4vDecPrm = &prDecPrm->rM4vParams;
	prVolPrm = &prM4vDecPrm->rVol;

	if ((prDecPrm->u1VideoType == VIDEO_TYPE_DIVX_MPEG4) &&
		(prVolPrm->u4DivxCodecVersion < 500) &&
		(prVolPrm->u4DivxCodecVersion != 0) &&
		(prVolPrm->u4DivxCodecVersion != 311))
	{
		prDecPrm->u4UmvPicW = prDecPrm->u2Width;
		prDecPrm->u4UmvPicH = prDecPrm->u2Height;
	}
	else
	{
		prDecPrm->u4UmvPicW = (UINT32)(((prDecPrm->u2Width + 15) / 16) * 16);
		prDecPrm->u4UmvPicH = (UINT32)(((prDecPrm->u2Height + 15) / 16) * 16);
	}
	
	if(((prDecPrm->u1VideoType == VIDEO_TYPE_DIVX_MPEG4) && 
		(prVolPrm->u4DivxCodecVersion > 0) && 
		(prVolPrm->u4DivxCodecVersion < 503)) || 
		(prDecPrm->u1VideoType == VIDEO_TYPE_DX311))
	{
		prDecPrm->u4QPelType = TYPE_14496;
	}
	else
	{
		prDecPrm->u4QPelType = TYPE_MOMUSYS;
	}

	prDecPrm->u4CMvType = QPEL_4MV_CMV_DIVX503;  // Follow DivX source code
	if(prDecPrm->u1VopCodingType != VOP_CODING_TYPE_B)
	{
		if((prDecPrm->u1VideoType != VIDEO_TYPE_DX311) &&
			((prVolPrm->u4DivxCodecVersion == 0) || 
			(prVolPrm->u4DivxCodecVersion >= 503)))
		{
			prDecPrm->u4CMvType |= QPEL_FRAME_CMV_DIVX503;
		}
		else
		{
			prDecPrm->u4CMvType |= QPEL_FRAME_CMV_DIVXOLD;
		}
		prDecPrm->u4CMvType |= QPEL_FIELD_CMV_DIVX503;
	}
	else
	{
		prDecPrm->u4CMvType |= QPEL_FRAME_CMV_DIVXOLD;
		prDecPrm->u4CMvType |= QPEL_FIELD_CMV_14496;
	}

	
  	_VDEC_VLDBarl2Vdec();
  	VLD_WRITE32(RW_VLD_QUANT, (prDecPrm->rM4vParams.rVop.u1IntraDcVlcThr << 8) 
  		+ prDecPrm->u1VopQuant);

	  // HHKuo's
	  VLD_WRITE32(RW_VLD_DCACSA, (prDecPrm->u4DCACBufAddr >> 2));
	  u4DcacPitch = (u4MBx * 4) |  // MBx * 4
	                              ((u4MBx * 4 * 3) << 12) |  // MBx * 4 * 3
	                              (8 << 24) |  // bit per pixel
	                              (0 << 28) |  // only write 1 in DivX 3.11 1st picture
	                              (prVolPrm->fgQuarterSample << 29) |
	//#ifdef WRITE_FULL_DCAC_DATA
	//             (1 << 30) |   // write full dcac data to DRAM
	//#else
	                              (0 << 30) |   // only write 1 line data to DRAM
	//#endif
               			  (prVolPrm->fgDataPartitioned<< 31);
	// Jack's test codes
	//u4DcacPitch = 0x821c0b4;
	VLD_WRITE32(RW_VLD_DCACPITCH, u4DcacPitch);
	VLD_WRITE32(RW_VLD_MVSA, ((prDecPrm->u4MVECBufAddr >> 2) & 0x3FFFFF) | ((u4MBx * 4) << 22));
	VLD_WRITE32(RW_VLD_BMB1, ((prDecPrm->u4BMB1BufAddr >> 2) & 0x3FFFFF) | ((u4MBx * 4) << 22));
	VLD_WRITE32(RW_VLD_BMB2, ((prDecPrm->u4BMB2BufAddr >> 2) & 0x3FFFFF) | ((u4MBx * 4) << 22));

       u4Temp = VLD_READ32(RW_VLD_BCODE_SA);
	u4Temp &= 0x0F000000;
	VLD_WRITE32(RW_VLD_BCODE_SA, ((prDecPrm->u4BCODEBufAddr << 2) & 0xF0000000) |
                                                                ((prDecPrm->u4BCODEBufAddr >> 2) & 0xFFFFFF) | u4Temp);

	VLD_WRITE32(0x328,  /*((prDecPrm->u4BCODEBufAddr << 4) & 0xF0000000) | */
		((prDecPrm->u4BMB2BufAddr >> 8) & 0x3F0000) | 
		((prDecPrm->u4BMB1BufAddr >> 16) & 0x3F00) | 
		((prDecPrm->u4MVECBufAddr >> 24) & 0x3F));	
    
	if (prDecPrm->u1VopCodingType == 0) // for 8520
	  u4NonPBit = (UINT32)(1 << 18);
	else if ((prDecPrm->u1VopCodingType == 1) || (prDecPrm->u1VopCodingType== 2))
	  u4NonPBit = (UINT32)(0 << 18);
	else
	  u4NonPBit = (UINT32)(VLD_READ32(RW_VLD_DCMVSEL) & (1 << 18));
      
    u4DcacPitch = (0 << 0)  |  //turn on reset direction_busy
           (1 << 1)  |  //turn on idctbusy + VWRDRAMDCACST[0]
           (0 << 6)  |  //dcacbusy wait for wdle (required for 8105 DDR)
           (0 << 7)  |  //dcacbusy4is wait for wdle (required for 8105 DDR)
           (0 << 16) |  //mv_dram reset by 2D reset
           (0 << 17) |  //turn on mv_modb1
           u4NonPBit  |  //none_P_bitstream mode
           (0 << 19) |  //turn on sram
           (0 << 20) |  //mvbusy wait for wdle (required for 8105 DDR)
           (0 << 21);   //turn 3t period without mem_pause;
           
    	if(1) // for data-partitioned, but can turn on in general case
    	{
      		u4DcacPitch |= (1 << 4) |  //turn on dcacbusy + VWRDRAMDCACST[2]
              			  (1 << 5);   //turn on dcacbusy4is + VWRDRAMDCACST[2]
   	 }
    	u4DcacPitch |= (((UINT32)1) << 31); // for 8520
	// Jack's test code
    	//u4DcacPitch = 0x903008f2;
    	//u4DcacPitch = 0x40830;
    
        if (prDecPrm->u1VopCodingType == VOP_CODING_TYPE_B)
        {
            if (prDecPrm->u1BackwardRefVopType == VOP_CODING_TYPE_I)
            {
                u4DcacPitch = 0x903408f2;
            }
            else
            {
                u4DcacPitch = 0x903008f2;
            }
        }
        else
        {
            u4DcacPitch = 0x903008f2;
        }

    	VLD_WRITE32 (RW_VLD_DCMVSEL, u4DcacPitch);

    	u4DcacPitch = (0 << 0) | //vmbbusy is busy until data write2DRAM, not Write2WRFIFO.
           (0 << 1) | //vmbbusy wait for wdle (required for 8105 DDR)
           (0 << 16)| //stop vld_state by DCACFULL
           (0 << 17)| //trun off reset dramctrl when mc_abort
           (1 << 18)| //stop vld_state by VDramWrVFull
           (0 << 20); //check wr3fifo full when Wdle (required for 8105 DDR)
    	u4DcacPitch |= (1 << 23); // for 8520
	// Jack's test code
	u4DcacPitch = 0x910002;
    	VLD_WRITE32(RW_VLD_DRAMMBSEL, u4DcacPitch);

	MC_WRITE32(RW_MC_QPEL_TYPE, prDecPrm->u4QPelType);
	MC_WRITE32(RW_MC_QUARTER_SAMPLE, prVolPrm->fgQuarterSample);
  	MC_WRITE32(RW_MC_ROUNDING_CTRL, prDecPrm->u1VopRoundingType);

  	MC_WRITE32(RW_MC_UMV_PIC_WIDTH, prDecPrm->u4UmvPicW);
  	MC_WRITE32(RW_MC_UMV_PIC_HEIGHT, prDecPrm->u4UmvPicH);

  	MC_WRITE32(RW_MC_CBCR_MV_TYPE, prDecPrm->u4CMvType);

  	// Modified by C.K. Hu 20040617. Porting from 1389_5039
  	/*u4DcacPitch = MC_READ32(RW_MC_BREF);
  	u4DcacPitch |= 0x10;
  	MC_WRITE32(RW_MC_BREF, u4DcacPitch);*/

  	*pi4RetValue = 0;

	LOG(7, "Enter _VDEC_DecM4VProc Succeed");
}

void _VDEC_DecDx311Proc(VDEC_HDLR hVdec, INT32* pi4RetValue)
{
    VDEC_MPEG4_DEC_PARAM_INFO_T *prDecPrm = (VDEC_MPEG4_DEC_PARAM_INFO_T*)hVdec;
    VDEC_DX311_PIC_INFO_T *prDx3DecPrm = &(prDecPrm->rDx3Params);

    UINT32 u4Dx3Hdr, u4RegVal1;

    if (prDecPrm->u1FbgId == FBM_FBG_ID_UNKNOWN ||
        prDecPrm->u1FbId == FBM_FBG_ID_UNKNOWN)
    {
        LOG(3, "FBM is not ready\n");
        *pi4RetValue = E_VDEC_FAIL;
        return;
    }

    _SetMpeg4McDecBuf(prDecPrm);

    /*if (prDecPrm->fgVopNotCoded == TRUE)
    {
    	LOG(7, "This vop is not coded\n");
    	return;
    }*/

    VLD_WRITE32(RW_VLD_MP4_FLG, 0x101);
    VLD_WRITE32(RW_VLD_PARA, (prDecPrm->u1VopCodingType << 21));

    _VLDSetPicSize(prDecPrm, pi4RetValue);
    _VLDSetMBRowPrm(prDecPrm, pi4RetValue);
    _VDEC_DecM4VProc(prDecPrm, (UINT32)((prDecPrm->u2Width + 15) / 16), pi4RetValue);

    MC_WRITE32(RW_MC_ADDRSWAP, prDecPrm->u4AddrMode);
#ifdef CC_IS_MPEG4_POST_MT5363
    MC_WRITE32(RW_MC_NBM_ADDRSWAP, prDecPrm->u4AddrMode);
#endif

    u4Dx3Hdr = (prDx3DecPrm->u1AltIAcChromDctIdx) |
            (prDx3DecPrm->u1AltIAcChromDct << 1) |
            (prDx3DecPrm->u1AltIAcLumDctIdx << 2) |
            (prDx3DecPrm->u1AltIAcLumDct << 3) |
            (prDx3DecPrm->u1AltIDcDct << 4) |
            (prDx3DecPrm->u1AltPAcDctIdx << 8) |
            (prDx3DecPrm->u1AltPAcDct << 9) |
            (prDx3DecPrm->u1AltPDcDct << 10) |
            (prDx3DecPrm->u1AltMv << 11) |
            (prDx3DecPrm->u1HasSkip << 16) |
            (prDx3DecPrm->u1FrameMode << 20);
    VLD_WRITE32(RW_VLD_DX3, u4Dx3Hdr);

    // Non-DivX3 (MPEG4-only) registers...
    // Maybe it's not necessary but clear them anyway
    // GSLin's
    VLD_WRITE32(RW_VLD_MP4_HDR, 0x0);
    VLD_WRITE32(RW_VLD_M4SH, 0);
    // HHKuo's
    VLD_WRITE32(RW_VLD_DIRE_MD, 0);
    //  vWriteVLD(RW_VLD_BMB1, 0);
    //  vWriteVLD(RW_VLD_BMB2, 0);
    //  vWriteVLD(RW_VLD_BCODE_SA, 0);
    VLD_WRITE32(RW_VLD_DIRE_MD_IL, 0);
    // CCJu's
    MC_WRITE32(RW_MC_WARP_POINT_NUM, 0);
    MC_WRITE32(RW_MC_GMC_Y_MV, 0);
    MC_WRITE32(RW_MC_GMC_C_MV, 0);

    u4RegVal1 = (UINT32)(1 << 5);
    u4RegVal1 |= (UINT32)(1 << 6);
    if (prDecPrm->u1VopCodingType == VOP_CODING_TYPE_I)
      	{
      		u4RegVal1 |= (1 << 0);
    }
    else if (prDecPrm->u1VopCodingType == VOP_CODING_TYPE_P)
    {
    	u4RegVal1 |= (1 << 1);
    }

    VLD_WRITE32(RW_VLD_VOP_TYPE, u4RegVal1);

    VLD_WRITE32(RW_VLD_VDOUFM, VLD_VDOUFM + VLD_ERR_MONITOR);

    u4RegVal1 = 0;
    u4RegVal1 |= VLD_PDHW;
    u4RegVal1 |= VLD_RTERR;

    // I, P shall decode whole picture
    u4RegVal1 |= (prDecPrm->u2Width >> 4);

    // Turn Off Racing Mode
    u4RegVal1 |= (0x1 << 27);

    // Process Decode
    VLD_WRITE32(RW_VLD_PROC, u4RegVal1);

#ifdef OutputDecSettings
        UINT32 u4Temp;
    u4Temp = VLD_READ32(0xF0);
    LOG(0, "Decoder's Input Window: 0x%8x\n", u4Temp);

    u4Temp = MC_READ32(RW_MC_QPEL_TYPE);
    LOG(0, "MC_QPEL_TYPE (%d) = 0x%x\n", (RW_MC_QPEL_TYPE>>2), u4Temp);

    u4Temp = MC_READ32(RW_MC_QUARTER_SAMPLE);
    LOG(0, "MC_QUARTER_SAMPLE (%d) = 0x%x\n", (RW_MC_QUARTER_SAMPLE>>2), u4Temp);

    u4Temp = MC_READ32(RW_MC_ROUNDING_CTRL);
    LOG(0, "ROUNDING_CTRL (%d) = 0x%x\n", (RW_MC_ROUNDING_CTRL>>2), u4Temp);

    u4Temp = MC_READ32(RW_MC_UMV_PIC_WIDTH);
    LOG(0, "MC_UMV_PIC_WIDTH (%d) = 0x%x\n", (RW_MC_UMV_PIC_WIDTH>>2), u4Temp);

    u4Temp = MC_READ32(RW_MC_UMV_PIC_HEIGHT);
    LOG(0, "MC_UMV_PIC_HEIGHT (%d) = 0x%x\n", (RW_MC_UMV_PIC_HEIGHT>>2), u4Temp);

    u4Temp = MC_READ32(RW_MC_WARP_POINT_NUM);
    LOG(0, "MC_WARP_POINT_NUM (%d) = 0x%x\n", (RW_MC_WARP_POINT_NUM>>2), u4Temp);

    u4Temp = MC_READ32(RW_MC_GMC_Y_MV);
    LOG(0, "MC_GMC_Y_MV (%d) = 0x%x\n", (RW_MC_GMC_Y_MV>>2), u4Temp);

    u4Temp = MC_READ32(RW_MC_GMC_C_MV);
    LOG(0, "MC_GMC_C_MV (%d) = 0x%x\n",  (RW_MC_GMC_C_MV>>2), u4Temp);

    u4Temp = MC_READ32(RW_MC_CBCR_MV_TYPE);
    LOG(0, "MC_CBCR_MV_TYPE (%d) = 0x%x\n", (RW_MC_CBCR_MV_TYPE>>2), u4Temp);

    u4Temp = VLD_READ32(RW_VLD_PARA);
    LOG(0, "VLD_PARA (%d) = 0x%x\n", (RW_VLD_PARA>>2), u4Temp);

    u4Temp = VLD_READ32(RW_VLD_PROC);
    LOG(0, "VLD_PROC (%d) = 0x%x\n", (RW_VLD_PROC>>2), u4Temp);

    u4Temp = VLD_READ32(RW_VLD_PICSZ);
    LOG(0, "VLD_PICSZ (%d) = 0x%x\n", (RW_VLD_PICSZ>>2), u4Temp);

    u4Temp = VLD_READ32(RW_VLD_MBROWPRM);
    LOG(0, "VLD_MBROWPRM (%d) = 0x%x\n", (RW_VLD_MBROWPRM>>2), u4Temp);

    u4Temp = VLD_READ32(RW_VLD_MP4_HDR);
    LOG(0, "VLD_MP4_HDR (%d) = 0x%x\n", (RW_VLD_MP4_HDR>>2), u4Temp);

    u4Temp = VLD_READ32(RW_VLD_MP4_FLG);
    LOG(0, "VLD_MP4_FLG (%d) = 0x%x\n", (RW_VLD_MP4_FLG>>2), u4Temp);

    u4Temp = VLD_READ32(RW_VLD_QUANT);
    LOG(0, "VLD_QUANT (%d) = 0x%x\n", (RW_VLD_QUANT>>2), u4Temp);

    u4Temp = VLD_READ32(RW_VLD_DCACPITCH);
    LOG(0, "VLD_DCACPITCH (%d) = 0x%x\n", (RW_VLD_DCACPITCH>>2), u4Temp);

    u4Temp = VLD_READ32(RW_VLD_DIRE_MD);
    LOG(0, "VLD_DIRE_MD (%d) = 0x%x\n", (RW_VLD_DIRE_MD>>2), u4Temp);

    u4Temp = VLD_READ32(RW_VLD_DIRE_MD_IL);
    LOG(0, "VLD_DIRE_MD_IL (%d) = 0x%x\n", (RW_VLD_DIRE_MD_IL>>2), u4Temp);
#endif
    HalFlushInvalidateDCache();

#ifdef IC_MPEG4_VERIFICATION
    UINT32 u4Temp;
    u4Temp = MC_READ32(0x714);
    LOG(0, "0x%8x (Register %ld) =  0x%8x before decoding\n", 0x714, (0x714>>2), u4Temp);
#endif

    // Trigger Decoder
    VLD_WRITE32(RW_VLD_DECSTART, 1);
    VLD_WRITE32(RW_VLD_DECSTART, 0);
}


// **************************************************************************
// Function : void _VDEC_WaitDecDone(VDEC_MPEG4_DEC_PARAM_INFO_T rDecPrm);
// Description : WaitDecDone
// Parameter : rDecPrm
// Return      : BOOL, if decode successfully
// **************************************************************************
BOOL _VDEC_WaitMpeg4DecDone(VDEC_MPEG4_DEC_PARAM_INFO_T* prDecPrm)
{
	BOOL fgRet = TRUE;
	UINT32 u4MbX, u4MbY;
	UINT32 u4Prd, u4Idx;
	UINT32* pu4Ptr;

	LOG(7, "Enter _VDEC_WaitMpeg4DecDone");

	//if (prDecPrm->fgVopNotCoded == TRUE)
	//{
		/*if (prDecPrm->u1VopCodingType != VOP_CODING_TYPE_B)
		{
			FBM_SetFrameBufferStatus(prDecPrm->u1FbgId, prDecPrm->u1FbId,
				FBM_FB_STATUS_READY);
		}*/

		//return TRUE;
	//}
	
#ifdef EN_IRQ

	LOG(7, "Wait Decoder's Interrupt\n");

	//wait decode done
	if (x_sema_lock_timeout(_hMpeg4VdecFinSema, MP4_WAIT_DEC_TIME) != OSR_OK)
	{
	    LOG(3, " %ld frame decoding Error\n", prDecPrm->u4FrameCount);
	    prDecPrm->fgDecError = TRUE;
	    fgRet = FALSE;
#ifdef MPEG4_EMULATION
		UINT32 u4Cnt = 0x300;

		LOG(0, "Decoder Error Timeout!!!\n");

		LOG(0, "//WaitFetchOK Rptr 0xB0=0x%x, 0xFC=0x%x, 0xEC=0x%x, 0xE8=0x%x\n, WPtr 0xF8=0x%x\n", 
              VLD_READ32(RW_VLD_RPTR), VLD_READ32(RO_VLD_VRPTR),
              VLD_READ32(RO_VLD_VBAR), VLD_READ32(RO_VLD_FETCHOK), VLD_READ32(RO_VLD_VWPTR));

		LOG(0, "Frame Count = %ld\n", prDecPrm->u4FrameCount);

		LOG(0, "MBX =0x%x, MBY =0x%x\n", MC_READ32(RO_MC_MBX), MC_READ32(RO_MC_MBX));

		u4Temp = VLD_READ32(0xF0);
		LOG(0, "Decoder Error Timeout!! Decoder's Input Window: 0x%8x\n", u4Temp);

		u4Temp = VLD_READ32(RW_VLD_MP4_HDR);
		LOG(0, "RW_VLD_MP4_HDR (Register %ld) =  0x%8x\n", (RW_VLD_MP4_HDR>>2), u4Temp);

		u4Temp = VLD_READ32(RW_VLD_MP4_FLG);
		LOG(0, "RW_VLD_MP4_FLG (Register %ld) =  0x%8x\n", (RW_VLD_MP4_FLG >> 2), u4Temp);

		u4Temp = VLD_READ32(RW_VLD_LDSR);
		LOG(0, "RW_VLD_LDSR (Register %ld) =  0x%8x\n", (RW_VLD_LDSR >> 2), u4Temp);

		u4Temp = VLD_READ32(RW_VLD_LDSH);
		LOG(0, "RW_VLD_LDSH (Register %ld) =  0x%8x\n", (RW_VLD_LDSH >> 2), u4Temp);

		u4Temp = VLD_READ32(RW_VLD_QUANT);
		LOG(0, "RW_VLD_QUANT (Register %ld) =  0x%8x\n", (RW_VLD_QUANT >> 2), u4Temp);

			
		u4Temp = VLD_READ32(RW_VLD_M4SH);
		LOG(0, "RW_VLD_M4SH (Register %ld) =  0x%8x\n", (RW_VLD_M4SH>> 2), u4Temp);

		u4Temp = VLD_READ32(RW_VLD_DX3);
		LOG(0, "RW_VLD_DX3 (Register %ld) =  0x%8x\n", (RW_VLD_DX3>>2), u4Temp);

		u4Temp = VLD_READ32(RO_VLD_MP4DECBSY);
		LOG(0, "RO_VLD_MP4DECBSY (Register %ld) =  0x%8x\n", (RO_VLD_MP4DECBSY>>2), u4Temp);

		u4Temp = VLD_READ32(RW_VLD_DECSTART);
		LOG(0, "RW_VLD_DECSTART (Register %ld) =  0x%8x\n", (RW_VLD_DECSTART>>2), u4Temp);

		u4Temp = VLD_READ32(RW_VLD_NON_P);
		LOG(0, "RW_VLD_NON_P (Register %ld) =  0x%8x\n", (RW_VLD_NON_P>>2), u4Temp);

		u4Temp = VLD_READ32(0x1EC);
		LOG(0, "0x1EC (Register %ld) =  0x%8x\n", (0x1EC>>2), u4Temp);

		u4Temp = VLD_READ32(0x1F0);
		LOG(0, "0x1F0 (Register %ld) =  0x%8x\n", (0x1F0>>2), u4Temp);

		u4Temp = VLD_READ32(RO_VLD_MP4DECERR);
		LOG(0, "RO_VLD_MP4DECERR (Register %ld) =  0x%8x\n", (RO_VLD_MP4DECERR>>2), u4Temp);

		u4Temp = VLD_READ32(0x1F8);
		LOG(0, "0x1F8 (Register %ld) =  0x%8x\n", (0x1F8>>2), u4Temp);

		u4Temp = VLD_READ32(0x1FC);
		LOG(0, "0x1FC (Register %ld) =  0x%8x\n", (0x1FC>>2), u4Temp);
		
		u4Temp = VLD_READ32(0x200);
		LOG(0, "0x200 (Register %ld) =  0x%8x\n", (0x200>>2), u4Temp);

		u4Temp = VLD_READ32(RW_VLD_DCACSA);
		LOG(0, "RW_VLD_DCACSA (Register %ld) =  0x%8x\n", (RW_VLD_DCACSA>>2), u4Temp);

		u4Temp = VLD_READ32(RW_VLD_DCACPITCH);
		LOG(0, "RW_VLD_DCACPITCH (Register %ld) =  0x%8x\n", (RW_VLD_DCACPITCH >> 2), u4Temp);

		u4Temp = VLD_READ32(RW_VLD_MVSA);
		LOG(0, "RW_VLD_MVSA (Register %ld) =  0x%8x\n", (RW_VLD_MVSA >> 2), u4Temp);

		u4Temp = VLD_READ32(RW_VLD_DIRE_MD);
		LOG(0, "RW_VLD_DIRE_MD (Register %ld) =  0x%8x\n", (RW_VLD_DIRE_MD >> 2), u4Temp);

		u4Temp = VLD_READ32(RW_VLD_BMB1);
		LOG(0, "RW_VLD_BMB1 (Register %ld) =  0x%8x\n", (RW_VLD_BMB1 >> 2), u4Temp);

			
		u4Temp = VLD_READ32(RW_VLD_BMB2);
		LOG(0, "RW_VLD_BMB2 (Register %ld) =  0x%8x\n", (RW_VLD_BMB2>> 2), u4Temp);

		u4Temp = VLD_READ32(RW_VLD_BCODE_SA);
		LOG(0, "RW_VLD_BCODE_SA (Register %ld) =  0x%8x\n", (RW_VLD_BCODE_SA>>2), u4Temp);

		u4Temp = VLD_READ32(RW_VLD_DIRE_MD_IL);
		LOG(0, "RW_VLD_DIRE_MD_IL (Register %ld) =  0x%8x\n", (RW_VLD_DIRE_MD_IL>>2), u4Temp);

		u4Temp = VLD_READ32(RW_VLD_DCMVSEL);
		LOG(0, "RW_VLD_DCMVSEL (Register %ld) =  0x%8x\n", (RW_VLD_DCMVSEL>>2), u4Temp);

		u4Temp = VLD_READ32(RW_VLD_DRAMMBSEL);
		LOG(0, "RW_VLD_DRAMMBSEL (Register %ld) =  0x%8x\n", (RW_VLD_DRAMMBSEL>>2), u4Temp);

		for (u4Cnt = 0x324; u4Cnt <= 0x3E8; u4Cnt += 4)
		{
			u4Temp = VLD_READ32(u4Cnt);
			LOG(0, "0x%8x (Register %ld) =  0x%8x\n", u4Cnt, (u4Cnt>>2), u4Temp);
		}
#endif
	}

    LOG(9, "Wait Decoder's Interrupt %ld Frame is done\n", prDecPrm->u4FrameCount);

    if (prDecPrm->fgDecError == TRUE)
    {
        FBM_SetFrameBufferStatus(prDecPrm->u1FbgId, prDecPrm->u1FbId,
        FBM_FB_STATUS_EMPTY);
    }
    else
    {
#ifdef IC_MPEG4_VERIFICATION
        UINT32 u4Temp = 0;

        u4Temp = MC_READ32(0x770);
        LOG(0, "0x%8x (Register %ld) =  0x%8x\n", 0x770, (0x770>>2), u4Temp);
        u4Temp = MC_READ32(0x774);
        LOG(0, "0x%8x (Register %ld) =  0x%8x\n", 0x774, (0x774>>2), u4Temp);
        u4Temp = MC_READ32(0x778);
        LOG(0, "0x%8x (Register %ld) =  0x%8x\n", 0x778, (0x778>>2), u4Temp);
        u4Temp = MC_READ32(0x644);
        LOG(0, "0x%8x (Register %ld) =  0x%8x\n", 0x644, (0x644>>2), u4Temp);
        u4Temp = MC_READ32(0x828);
        LOG(0, "0x%8x (Register %ld) =  0x%8x\n", 0x828, (0x828>>2), u4Temp);
#endif

#ifdef CC_IS_MPEG4_POST_MT5363
#ifdef __MODEL_slt__
        if (prDecPrm->u4FrameCount  == 8)
        {
            UINT32 u4TempY, u4TempC;

            u4TempY = MC_READ32(0x5F4);
            u4TempC = MC_READ32(0x5F8);

            if ((u4TempY != 0x008c00ed) ||
                (u4TempC != 0x0003000e))
            {   
                Printf("MMNG!!\n");
            }
            else
            {
                Printf("MMOK!!\n");
            }
        }
#endif
#endif
        if (prDecPrm->u1VopCodingType != VOP_CODING_TYPE_B)
        {
        	FBM_SetFrameBufferStatus(prDecPrm->u1FbgId, prDecPrm->u1FbId,
        		FBM_FB_STATUS_READY);

#ifdef CC_MPEG4_DEBLOCKING
        	if (prDecPrm->u1VideoType == VIDEO_TYPE_FLV)
        	{
        	    FBM_SetFrameBufferStatus(prDecPrm->u1FbgId, 
        	        prDecPrm->u1DbkFbId, 
        	        FBM_FB_STATUS_READY);                    
        	}
#endif

        	if (prDecPrm->u1VopCodingType == VOP_CODING_TYPE_P)
        	{
        		u4MbX = (UINT32)((prDecPrm->u2Width + 15) / 16);
        		u4MbY = (UINT32)((prDecPrm->u2Height + 15) / 16);
        		u4Prd = u4MbX * u4MbY;
        		if((u4Prd % 32) == 0)
        		{
        			u4Idx = u4Prd / 32;
        		  	pu4Ptr = (UINT32*)(VIRTUAL(prDecPrm->u4BCODEBufAddr));
        		  	pu4Ptr[u4Idx - 1] = pu4Ptr[u4Idx];
        		}
        	}
        	else if (prDecPrm->u1VopCodingType == VOP_CODING_TYPE_S)
        	{
        		pu4Ptr = (UINT32*)(VIRTUAL(prDecPrm->u4BCODEBufAddr));
        		for(u4Idx = 0; u4Idx < 64; u4Idx++)
        		{
        		  	pu4Ptr[u4Idx] = 0;
        		}
        	}
        }
        else
        {
            BOOL fgNotDisplay;
            FBM_PIC_HDR_T * prFbmPicHdr;
            VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(0);

            prFbmPicHdr = FBM_GetFrameBufferPicHdr(prVdecEsInfo->ucFbgId, prDecPrm->u1FbId);
            if (!prFbmPicHdr)
            {
                //Printf("_VDEC_WaitMpeg4DecDone 1 prFbmPicHdr NULL");
                LOG(6, "_VDEC_WaitMpeg4DecDone 1 prFbmPicHdr NULL");
                return FALSE;
            }

            if (prDecPrm->fgEnCalPts)
            {
                prFbmPicHdr->u4PTS = prDecPrm->u4DispPts;
                LOG(7, "Enable PTS calculation Cur PTS = 0x%08x\n", prFbmPicHdr->u4PTS);
                if ((prDecPrm->u8PtsScale != 0) &&
                    (prDecPrm->u4Rate != 0)&&(!prDecPrm->rPesInfo.fgNoVdecCalPts))
                {
                    UINT64 u8Numerator = 0, u8Denominator = 0;

                    u8Numerator = prDecPrm->u8PtsScale + prDecPrm->u8Remainder;
                    u8Denominator = (UINT64)prDecPrm->u4Rate;
                    prDecPrm->u4DispPts += 
                        u8Div6432(u8Numerator, u8Denominator, 
                        &prDecPrm->u8Remainder)* 
                        (prDecPrm->rPesInfo.u2SkipVidCount + 1+  prDecPrm->u4VopNotCoded);

                    prDecPrm->u4VopNotCoded = 0;
                }
            }
            else
            {
                if (prDecPrm->rPesInfo.fgDtsValid)
                {
                    prFbmPicHdr->u4PTS = prDecPrm->rPesInfo.u4PTS;
                    LOG(7, "Use container's PTS = 0x%08x\n", prFbmPicHdr->u4PTS);
                }
                else
                {
                    VDEC_M4V_VOL_INFO_T *prVolPrm;
                    VDEC_M4V_PIC_INFO_T *prVopPrm;
                    UINT64 u8Numerator = 0, u8Denominator = 0;

                    prVolPrm = &prDecPrm->rM4vParams.rVol;
                    prVopPrm = &prDecPrm->rM4vParams.rVop;

                    if (prVopPrm->u2VopTimeIncrement >= prVopPrm->u2VopPrevTimeIncrement)
                    {
                        u8Numerator = 
                            (prVopPrm->u2VopTimeIncrement - prVopPrm->u2VopPrevTimeIncrement) * MPEG4_TIME_SCALE;
                    }
                    else
                    {
                        u8Numerator =  
                            (prVolPrm->u4TimeIncrementResolution - prVopPrm->u2VopPrevTimeIncrement + prVopPrm->u2VopTimeIncrement) * MPEG4_TIME_SCALE;
                    }

                    u8Numerator += prDecPrm->u8Remainder;
                    u8Denominator = (UINT64)prVolPrm->u4TimeIncrementResolution;

                    prFbmPicHdr->u4PTS = prDecPrm->u4DispPts;

                  prDecPrm->u4DispPts += 
                        u8Div6432(u8Numerator, u8Denominator, 
                        &prDecPrm->u8Remainder) * (prDecPrm->rPesInfo.u2SkipVidCount + 1);
                    
                    LOG(7, "Use ES PTS = 0x%08x\n", prFbmPicHdr->u4PTS);
                }
            }

            prFbmPicHdr->u2DecodingOrder = prVdecEsInfo->u2DecodingOrder;
            prFbmPicHdr->u4TotlaTimeOffset = prVdecEsInfo->u4TotlaTimeOffset;
            
            prFbmPicHdr->u8Offset = prDecPrm->rPesInfo.u8Offset;
            prFbmPicHdr->u8OffsetDisp = prVdecEsInfo->u8OffsetLast;
            prFbmPicHdr->u8OffsetI = prDecPrm->rPesInfo.u8OffsetI;
            prFbmPicHdr->fgTopFldFirst = prDecPrm->rM4vParams.rVop.fgTopFldFirst;
            prFbmPicHdr->ucTrueZeroPTS = FBM_USE_ZERO_PTS_IF_ZERO;
            prFbmPicHdr->i4TemporalRef = prDecPrm->i4TemporalRef;

            prDecPrm->i4TemporalRef++;
            
#if 0
            prFbmPicHdr->u4PicWidth = prDecPrm->u2Width;
            prFbmPicHdr->u4PicHeight = prDecPrm->u2Height;
            prFbmPicHdr->u4PicWidthPitch = prDecPrm->u4LineSize;
#endif

            fgNotDisplay = VDEC_IsNotDisplay(0, prFbmPicHdr->u4PTS,
                prFbmPicHdr->i4TemporalRef, prFbmPicHdr->u8Offset);
            if (fgNotDisplay)
            {
                /*FBM_SetFrameBufferPicFlag(prVdecEsInfo->ucFbgId, 
                    prDecPrm->u1FbId, 
                    FBM_MM_NOT_DISPLAY_FLAG);
                
                LOG(6, "_VDEC_WaitMpeg4DecDone 1 fgNotDisplay ucFbId %d\n", prDecPrm->u1FbId);*/
                UCHAR ucFbStatus;
                ucFbStatus = FBM_GetFrameBufferStatus(prDecPrm->u1FbgId, prDecPrm->u1FbId);
                if ((ucFbStatus != FBM_FB_STATUS_DISPLAYQ) ||
                    (ucFbStatus != FBM_FB_STATUS_LOCK))
                {
                    FBM_SetFrameBufferStatus(prDecPrm->u1FbgId, prDecPrm->u1FbId,
                        FBM_FB_STATUS_EMPTY);
                }
                else
                {
                    LOG(6, "Frame buffer status = %x\n", ucFbStatus);
                }
            }
            else
            {
                LOG(6, "B frame prFbmPicHdr->u4PTS = 0x%08x\n", prFbmPicHdr->u4PTS);
                LOG(6, "prDecPrm->u1FbId = 0x%08x\n", prDecPrm->u1FbId);
                //Printf("B frame prFbmPicHdr->u4PTS = 0x%08x\n", prFbmPicHdr->u4PTS);
                //Printf("prDecPrm->u1FbId = 0x%08x\n", prDecPrm->u1FbId);

                LOG(5, "B Pic Temporal Ref = %d\n", prFbmPicHdr->i4TemporalRef);
                UCHAR ucFbStatus;
                ucFbStatus = FBM_GetFrameBufferStatus(prDecPrm->u1FbgId, prDecPrm->u1FbId);
                if ((ucFbStatus == FBM_FB_STATUS_DECODE) || (ucFbStatus == FBM_FB_STATUS_FIELD_READY) ||
			       (ucFbStatus == FBM_FB_STATUS_READY))
                {
                    FBM_SetFrameBufferStatus(prDecPrm->u1FbgId, prDecPrm->u1FbId,
                        FBM_FB_STATUS_DISPLAYQ);
                    prVdecEsInfo->u4DisplayQPicCnt++;            
                }
            }

            VDEC_CheckStopStatus(0, prFbmPicHdr->u4PTS, prFbmPicHdr->i4TemporalRef);
        }
    }

#else

	/*while(u4CntTimeChk < DEC_RETRY_NUM) //polling retry times
	{
		u4Cnt++;
		u4CntTimeChk++;
		if((u4Cnt & 0x3f)== 0x3f)	//delay
		{
			if(1 == (VLD_READ32(0xBC))) // Picture Finish Parameter Register
			{
				LOG(7, "Wait for _VDEC_WaitMpeg4DecDone Succeed");
				fgRet = TRUE;
				break;
			}
			else
			{

			
			if ((MC_READ32(RO_MC_MBX) == (prDecPrm->u2Width >> 4) -1)
				&& (MC_READ32(RO_MC_MBY) == (prDecPrm->u2Height >> 4) -1))
			{
				fgRet = TRUE;
				break;
			}
		}
				
			u4Cnt = 0;
		}
	}*/
	

	while(u4CntTimeChk < DEC_RETRY_NUM) //polling retry times
	{
		u4Cnt++;
		u4CntTimeChk++;
		if((u4Cnt & 0x3f)== 0x3f)	//delay
		{
			if (VLD_READ32(RO_VLD_DEC_OK) & 0x1)
        		{
        			if (prDecPrm->u1VopCodingType != VOP_CODING_TYPE_B)
				{
					FBM_SetFrameBufferStatus(prDecPrm->u1FbgId, prDecPrm->u1FbId,
						FBM_FB_STATUS_READY);
				}
				else
				{
					FBM_SetFrameBufferStatus(prDecPrm->u1FbgId, prDecPrm->u1FbId,
						FBM_FB_STATUS_DISPLAYQ);
				}

				fgRet = TRUE;
				break;
			}
		}
	}
	
	if (!fgRet)
	{
		UINT32 u4ChkSum = 0;
		LOG(0, "===================================");
		LOG(0, "Decoding Error");
		LOG(0, "===================================");
		fgRet = TRUE;
		//u4ChkSum = VLD_READ32(0xEB);
	}
#endif

	//_VDEC_Mpeg4DumpCheckSum();


	/*if (!fgRet)
	{
		LOG(3, "State 0x%x, MC State 0x%x\n",
            	VLD_READ32(RO_VLD_FSBI), MC_READ32(RO_MC_STATUS));
        	LOG(3, " MBX: 0x%x, MBY: 0x%x CBP(0x6c):0x%x\n",
            	MC_READ32(RO_MC_MBX), MC_READ32(RO_MC_MBY), 
            	MC_READ32(RO_MC_CBP));
        	LOG(3, "0xbc: 0x%x, 0xb0: 0x%x, 0xf8: 0x%x, 0xfc: 0x%x, \n", VLD_READ32(0xbc), 
            	VLD_READ32(RW_VLD_RPTR), VLD_READ32(RO_VLD_VWPTR), _MPV_VldRead32(RO_VLD_VRPTR));
	}*/

	return fgRet;
}

// **************************************************************************
// Function : void _VDEC_OutputDispPic(void);
// Description : Output 1 frm buff in DPB when DPB full
// Parameter : None
// Return      : 
// **************************************************************************

void _VDEC_Mpeg4OutputDispPic(VDEC_MPEG4_DEC_PARAM_INFO_T* prDecPrm, INT32* pi4RetValue)
{
    static UINT32 u4Count = 0;
    UINT32 u4AddrY, u4AddrC;

    FBM_GetFrameBufferAddr(prDecPrm->u1FbgId, prDecPrm->u1FbId, &u4AddrY, &u4AddrC);

    u4Count++;
}

static void _SetMpeg4McDecBuf(VDEC_MPEG4_DEC_PARAM_INFO_T* prDecPrm)
{
	UINT8 	u1FbgId;
    	UINT8 	u1FRefFbId;
    	UINT8 	u1BRefFbId;
    	UINT8 	u1DecFbId;

    	UINT32 	u4AddrY;
    	UINT32 	u4AddrC;
	UINT32	u4ForwardAddrY;    	
	UINT32	u4ForwardAddrC;
	UINT32	u4BackwardAddrY;    	
	UINT32	u4BackwardAddrC;
	
     	u1FbgId = prDecPrm->u1FbgId;
    	ASSERT(u1FbgId != FBM_FBG_ID_UNKNOWN);        

	FBM_GetRefFrameBuffer(u1FbgId, &u1FRefFbId, &u1BRefFbId);
	u1DecFbId = prDecPrm->u1FbId;
       ASSERT(u1DecFbId != FBM_FB_ID_UNKNOWN);

       switch (prDecPrm->u1VopCodingType)
       {
       	case VOP_CODING_TYPE_I:
       		FBM_GetFrameBufferAddr(u1FbgId, u1FRefFbId, &u4AddrY, &u4AddrC);           
            		_VDEC_Mpeg4SetFrmBufPtr(MP4_REF_BUF_0, u4AddrY, u4AddrC);
            
            		FBM_GetFrameBufferAddr(u1FbgId, u1DecFbId, &u4AddrY, &u4AddrC);
            		ASSERT((u4AddrY!=0)&&(u4AddrC !=0));                        
			LOG(7, "VOP_CODING_TYPE_I _SetMpeg4McDecBuf u1DecFbId %d u1BRefFbId %d u1FRefFbId %d\n", u1DecFbId, u1BRefFbId, u1FRefFbId );
            		_VDEC_Mpeg4SetFrmBufPtr(MP4_B_BUF_0, u4AddrY, u4AddrC);   
            		break;
      		case VOP_CODING_TYPE_P:
      		case VOP_CODING_TYPE_S:
			//if (prDecPrm->fgVopNotCoded == TRUE)
			//{
				/*_CopyRefBuf(u4ForwardAddrY, u4ForwardAddrC, u4AddrY, u4AddrC
					, prDecPrm->u4DramWidth * prDecPrm->u4DramHeight
					, (prDecPrm->u4DramWidth * prDecPrm->u4DramHeight) >> 1);*/
			//	return;
			//}
			LOG(7, "VOP_CODING_TYPE_P _SetMpeg4McDecBuf u1DecFbId %d u1BRefFbId %d u1FRefFbId %d\n", u1DecFbId, u1BRefFbId, u1FRefFbId );
            		FBM_GetFrameBufferAddr(u1FbgId, u1FRefFbId, &u4ForwardAddrY, &u4ForwardAddrC);            
            		_VDEC_Mpeg4SetFrmBufPtr(MP4_REF_BUF_0, u4ForwardAddrY, u4ForwardAddrC);
			LOG(7, "_SetMpeg4McDecBuf u1FRefFbId u4AddrY 0x%x, u4AddrC 0x%x\n", u4ForwardAddrY, u4ForwardAddrC);

           		FBM_GetFrameBufferAddr(u1FbgId, u1BRefFbId, &u4BackwardAddrY, &u4BackwardAddrC);
            		_VDEC_Mpeg4SetFrmBufPtr(MP4_REF_BUF_1, u4BackwardAddrY, u4BackwardAddrC);
        
            		FBM_GetFrameBufferAddr(u1FbgId, u1DecFbId, &u4AddrY, &u4AddrC);
            		LOG(7, "_SetMpeg4McDecBuf u1DecFbId u4AddrY 0x%x, u4AddrC 0x%x\n", u4AddrY, u4AddrC);
            		ASSERT((u4AddrY!=0)&&(u4AddrC !=0));                        
			_VDEC_Mpeg4SetFrmBufPtr(MP4_B_BUF_0, u4AddrY, u4AddrC);
			LOG(7, "VOP_CODING_TYPE_P _SetMpeg4McDecBuf u1DecFbId %d u1BRefFbId %d u1FRefFbId %d\n", u1DecFbId, u1BRefFbId, u1FRefFbId );
			break;
		case VOP_CODING_TYPE_B:
			LOG(7, "VOP_CODING_TYPE_B _SetMpeg4McDecBuf u1DecFbId %d u1BRefFbId %d u1FRefFbId %d\n", u1DecFbId, u1BRefFbId, u1FRefFbId );
			FBM_GetFrameBufferAddr(u1FbgId, u1FRefFbId, &u4AddrY, &u4AddrC);
            		_VDEC_Mpeg4SetFrmBufPtr(MP4_REF_BUF_0, u4AddrY, u4AddrC);
			LOG(7, "_SetMpeg4McDecBuf u1FRefFbId u4AddrY 0x%x, u4AddrC 0x%x\n", u4AddrY, u4AddrC);
			
            		FBM_GetFrameBufferAddr(u1FbgId, u1BRefFbId, &u4AddrY, &u4AddrC);
            		_VDEC_Mpeg4SetFrmBufPtr(MP4_REF_BUF_1, u4AddrY, u4AddrC);
			LOG(7, "_SetMpeg4McDecBuf u1BRefFbId u4AddrY 0x%x, u4AddrC 0x%x\n", u4AddrY, u4AddrC);
			
            		FBM_GetFrameBufferAddr(u1FbgId, u1DecFbId, &u4AddrY, &u4AddrC);           
            		ASSERT((u4AddrY!=0)&&(u4AddrC !=0));             
            		_VDEC_Mpeg4SetFrmBufPtr(MP4_B_BUF_0, u4AddrY, u4AddrC);  
            		LOG(7, "_SetMpeg4McDecBuf u1DecFbId u4AddrY 0x%x, u4AddrC 0x%x\n", u4AddrY, u4AddrC);
            		break;
            	default:
            		ASSERT(0);
            		LOG(0, "_SetMpeg4McDecBuf Unknown VOP Coding Type");
            		u4AddrY = 0;
            		u4AddrC = 0;
            		break;
       }
	
	prDecPrm->ucDecYPtr = (UCHAR*)u4AddrY;
	prDecPrm->ucDecCbCrPtr = (UCHAR*)u4AddrC;
}

#ifdef CC_MPEG4_DEBLOCKING
static void _SetMpeg4McDbkDecBuf(VDEC_MPEG4_DEC_PARAM_INFO_T* prDecPrm)
{
    UINT32 u4Temp;
    UINT32 u4AddrY;
    UINT32 u4AddrC;

    FBM_GetFrameBufferAddr(prDecPrm->u1FbgId, prDecPrm->u1DbkFbId, &u4AddrY, &u4AddrC);
    ASSERT((u4AddrY!=0)&&(u4AddrC !=0));                    
    _VDEC_Mpeg4SetFrmBufPtr(MP4_MC_DBK_BUF, u4AddrY, u4AddrC);

    prDecPrm->ucDecYPtr = (UCHAR*)u4AddrY;
    prDecPrm->ucDecCbCrPtr = (UCHAR*)u4AddrC;

    u4Temp = MC_READ32(RW_MC_BY1);
    u4Temp *= 2;
    MC_WRITE32(0x100, u4Temp);

    u4Temp = MC_READ32(RW_MC_BC1);
    u4Temp *= 2;
    MC_WRITE32(0x104, u4Temp);

}
#endif

static void _SetMpeg4DecFld(VDEC_MPEG4_DEC_PARAM_INFO_T* prDecPrm)
{
    UCHAR ucDecFld;
    UCHAR uc2ndFld;

    VDEC_M4V_PIC_INFO_T 	*prVopPrm = &prDecPrm->rM4vParams.rVop;

    
    ASSERT(prDecPrm != NULL);
    
    if (prDecPrm->u1PicStructure == MP4_FRM_PIC)
    {
        ucDecFld = MP4_MC_TOP_FLD;
        uc2ndFld = MP4_MC_2ND_FLD_OFF;
    }  
    else
    {
        if (prDecPrm->u1VopCodingType == VOP_CODING_TYPE_I ||
        	prDecPrm->u1VopCodingType == VOP_CODING_TYPE_P)
        {    
            if ((prDecPrm->u4DecFlag & DEC_FLG_MP4_2ND_FLD_PIC) > 0) 
            {
                ucDecFld = ((prVopPrm->fgTopFldFirst) ? 
                                  (UCHAR)(MP4_MC_BOTTOM_FLD) : (UCHAR)(MP4_MC_TOP_FLD));
                uc2ndFld = (UCHAR)MP4_MC_2ND_FLD_ON;
            }
            else
            {
                ucDecFld = ((prVopPrm->fgTopFldFirst)  ?
                                  (UCHAR)(MP4_MC_TOP_FLD) : (UCHAR)(MP4_MC_BOTTOM_FLD));  
                uc2ndFld = (UCHAR)MP4_MC_2ND_FLD_OFF;
            }
        }  
        else  // B Picture
        {
            ucDecFld = ((prVopPrm->fgTopFldFirst) ?
                              (UCHAR)(MP4_MC_TOP_FLD) : (UCHAR)(MP4_MC_BOTTOM_FLD));    
            uc2ndFld = (UCHAR)MP4_MC_2ND_FLD_OFF;
        }  
    }    
  
    _VDEC_Mpeg4McSetDecFld(ucDecFld, uc2ndFld); 

}

// *********************************************************************
// Function: void _VLDSetPicSize(VDEC_MPEG4_DEC_PARAM_INFO_T *prDecPrm, INT32* pi4RetValue)
// Description : Set RW_VLD_PICSZ register
// Parameter : ptDecPrm: pointer to decoder relate variables
//             fgB21FrmPic: B_1/2 mode and frame pic
// Return    : None
// *********************************************************************
void _VLDSetPicSize(VDEC_MPEG4_DEC_PARAM_INFO_T *prDecPrm, INT32* pi4RetValue)
{
	UINT32	u4MaxMblMod;
	UINT32 	u4WidthMb;
	UINT32  u4RegVal2;

	//u4MaxMblMod = 0x3f;
	u4MaxMblMod = (UINT32)((prDecPrm->u2Height + 15) >> 4);

	u4WidthMb = (UINT32)((prDecPrm->u2Width + 15) >> 4);
	/*u4RegVal2 = ((prDecPrm->u2Height + 15) << 16) +
	          ((u4MaxMblMod & 0x3F) << 8) + u4WidthMb;*/
	u4RegVal2 = ((prDecPrm->u2Height + 15) << 16) +
	          (u4MaxMblMod << 8) + u4WidthMb;

	VLD_WRITE32(RW_VLD_PICSZ, u4RegVal2);

	/*if (prDecPrm->u4AddrMode == 2)
	{
		VLD_WRITE32(RW_VLD_MCPICSZ, (prDecPrm->u4DramWidth + 63) >> 6);
	}
	else*/
	{
		//VLD_WRITE32(RW_VLD_MCPICSZ, (prDecPrm->u2Width + 15) >> 4);
		VLD_WRITE32(RW_VLD_MCPICSZ, (prDecPrm->u4LineSize+ 15) >> 4);
	}
}

// *********************************************************************
// Function :void vVLDSetMBRowPrm(TMpvDecPrm *ptDecPrm, BOOL fgBPic, BOOL fgFrmPic)
// Description : Set RW_VLD_MBROWPRM register
// This register specifies the different macroblock row parameters that will be used during 
// video decoding by hardware
// Parameter : ptDecPrm: pointer to decoder relate variables
//             fgBPic: B-picture
//             fgFrmPic: frame picture
// Return    : None
// *********************************************************************
void _VLDSetMBRowPrm(VDEC_MPEG4_DEC_PARAM_INFO_T *prDecPrm, INT32* pi4RetValue)
{
	UINT32	u4RegVal3;
	UINT32  	u4StartX, u4StartY, u4EndY;

       u4StartY = 0;
	//Partial decode parameters calculation
	u4StartX = prDecPrm->u4DecXOff >> 4;
	if((prDecPrm->u4DecXOff & 0x0f) != 0)
	{
		u4StartX += 1;
	}

	u4StartX = prDecPrm->u4DecYOff >> 4;
	if(prDecPrm->u4DecYOff == 0)
	{
		u4StartY = 0;
	}
	else if((u4StartY & 0x01) != 0)
	{
		u4StartY += 1;
	}

	u4EndY = (prDecPrm->u4DecYOff + prDecPrm->u2Height - 1) >> 4;
	if((prDecPrm->u2Height == prDecPrm->u2Height) &&
	   (prDecPrm->u4DecYOff == 0))
	{
		u4EndY = 127;
	}
	else if((u4EndY & 0x01)==0)
	{
		u4EndY -= 1;
	}

	if(prDecPrm->u1VopCodingType == VOP_CODING_TYPE_B)
	{
		u4RegVal3 = (u4StartY << 24) + (u4EndY << 16) + (prDecPrm->u4BufStart << 8);
	}
	else
	{
		u4RegVal3 = (u4EndY << 16) + (prDecPrm->u4BufStart << 8);
	}

	VLD_WRITE32(RW_VLD_MBROWPRM, u4RegVal3);
}

/*static int Mpeg4T32HostExec(unsigned int uTargetFlag, char *szFormat, ...)
{
    char szBuffer[160];
    VA_LIST args;
    VA_START(args, szFormat);

    x_vsprintf(szBuffer, szFormat, args);

    VA_END(args);
    return 0;
}

static void _CopyRefBuf(UINT32 u4ForwardAddY, UINT32 u4ForwardAddC, 
			UINT32 u4AddrY, UINT32 u4AddrC, UINT32 u4YSize, UINT32 u4CSize)
{
	x_memcpy((void*)u4AddrY, (void*)u4ForwardAddY, u4YSize);
	x_memcpy((void*)u4AddrC, (void*)u4ForwardAddC, u4CSize);
}*/

void Mpeg4IsrInit(void)
{
        x_os_isr_fct pfnOldIsr;

        // reg ISR
        if (x_reg_isr(VLD_MPEG4_VECTOR, _Mpeg4VldIsr, &pfnOldIsr) != OSR_OK)
        {
            ASSERT(0);
        }
}

static void _Mpeg4VldIsr(UINT16 u2Vector)
{
    if (u2Vector == VLD_MPEG4_VECTOR)
    {
        if (VLD_READ32(RO_VLD_DEC_OK) & 0x1)
        {
		//LOG(7, "Decoder Finished");
		//LOG(0,  "Decoder Finished and interrupt\n");

		VLD_WRITE32(RW_VLD_DECSTART, (UINT32)VLD_CLR);

		//unlock semaphore
		VERIFY(x_sema_unlock(_hMpeg4VdecFinSema) == OSR_OK);
        }
    }

}



