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
 * $RCSfile: io_sdimage.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file io_sdimage.c
 *  SDAL SdImage module ioctl interface of MT538X.
 */
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_linux.h"
#include <linux/sdal_ioctl.h>
#include "SdImage.h"
#include "img_lib_if.h"
#include "vdp_if.h"
#include "fbm_drvif.h"
#include "sd_vdopath.h"
#include "pmx_drvif.h"
#include "gfx_if.h"
#include "panel.h"
/********************************************************************
	MACRO CONSTANT DEFINITIONS
********************************************************************/

/********************************************************************
	MACRO FUNCTION DEFINITIONS
********************************************************************/
#define ALIGN16(X) (((X) + 0xF) & (~0xF))	///< for gfx pitch alignment
#define FBM_WORKING_BUFSZ	0x100000

#define IMG_DRV_HW_ERROR		(-1)
#define IMG_DRV_IO_ERROR		(-2)
#define IMG_DRV_NOT_SUPPORT	    (-3)
/********************************************************************
	TYPE DEFINITION
********************************************************************/

/********************************************************************
	VARIABLE
********************************************************************/

static SdVideoID_k _eVideoId = SD_VIDEO0;
static ENUM_IMG_ROTATE_PHASE _eRotateDegree = CLOCKWISE_0;
static IMG_REGION_T _rDecOut;
static SdImage_DecParam_t _rDecParam;
static SdGeometry_t _rsGeom;
static UINT8 _u1DecErr = 0;	// 1: error, 2: not support, 3: stop
static BOOL _bStartDec = FALSE;
static BOOL _bIsRawPic = TRUE;

/********************************************************************
	FUNCTION IMPLEMENTATION
********************************************************************/

static INT32 _InjectVideoPlane(void)
{
	IMG_VP_INFO_T rVpInfo;
	IMG_BUF_PARAM_T rCmd;

	//error handling
	if((_rDecOut.pu1Addr == NULL) || (_rDecOut.u4Pitch == 0)
		||(_rDecOut.u4TotalLine == 0) || (_rDecOut.u4Width == 0) ||
		(_rDecOut.u4Height == 0) || (_rsGeom.w < 2)||
		(_rsGeom.h < 2)) 
	{
		return SD_PARAMETER_ERROR;
	}
	//------ rotate operation, src: decoding image buffer

	rCmd.eDispColorMode = Y_CBCR422_SEP16;
	rCmd.eRotateDegree = _eRotateDegree;
	rCmd.rSrcRegion.pu1Addr = _rDecOut.pu1Addr;
	rCmd.rSrcRegion.u4CbCrOffset = _rDecOut.u4CbCrOffset;
	rCmd.rSrcRegion.u4Depth = _rDecOut.u4Depth;
	rCmd.rSrcRegion.u4Pitch = _rDecOut.u4Pitch;
	rCmd.rSrcRegion.u4TotalLine = _rDecOut.u4TotalLine;
	rCmd.rSrcRegion.u4StartX = _rDecOut.u4StartX;
	rCmd.rSrcRegion.u4StartY = _rDecOut.u4StartY;
	rCmd.rSrcRegion.u4Width = _rDecOut.u4Width;
	rCmd.rSrcRegion.u4Height = _rDecOut.u4Height;

	if(IMG_GetVideoPlaneInfo(&rVpInfo) != E_IMG_OK)
	{
		return SD_ERR_INV;
	}
	//error handling
	if((rVpInfo.u4Width < _rsGeom.x) || (rVpInfo.u4Height < _rsGeom.y) ||
		(rVpInfo.u4Width < (_rsGeom.x + _rsGeom.w)) || 
		(rVpInfo.u4Height < (_rsGeom.y + _rsGeom.h)))
	{
		return SD_PARAMETER_ERROR;
	}
		
	//copy current VP to back VP, still do frame flip without mute
	x_memcpy((void*)VIRTUAL((UINT32)rVpInfo.pu1BackFrameAddr[0]), 
		(void*)VIRTUAL((UINT32)rVpInfo.pu1CurFrameAddr[0]),
		((UINT32)rVpInfo.pu1CurFrameAddr[0] < (UINT32)rVpInfo.pu1BackFrameAddr[0])?
		((UINT32)rVpInfo.pu1BackFrameAddr[0] - (UINT32)rVpInfo.pu1CurFrameAddr[0]):
		((UINT32)rVpInfo.pu1CurFrameAddr[0] - (UINT32)rVpInfo.pu1BackFrameAddr[0]));
	
	rCmd.rDstRegion.pu1Addr = rVpInfo.pu1BackFrameAddr[0];
	rCmd.rDstRegion.u4CbCrOffset = (UINT32)rVpInfo.pu1BackFrameAddr[1] - 
		(UINT32)rVpInfo.pu1BackFrameAddr[0];
		
	rCmd.rDstRegion.u4Depth = 1;
	rCmd.rDstRegion.u4Pitch = ALIGN16(rVpInfo.u4Width);
	rCmd.rDstRegion.u4TotalLine = rVpInfo.u4Height;
	rCmd.rDstRegion.u4StartX = _rsGeom.x;
	rCmd.rDstRegion.u4StartY = _rsGeom.y;
	rCmd.rDstRegion.u4Width = ALIGN16(_rsGeom.w);//rVpInfo.u4Width;
	rCmd.rDstRegion.u4Height = _rsGeom.h;//rVpInfo.u4Height;

	rCmd.bKeepRatio = TRUE;
	
	//use working area as temp buffer
	IMG_Set(rCmd);

	if(IMG_Flip() != E_IMG_OK)
	{
		return SD_NOT_OK;
	}
	return SD_OK;
}


void _IMGDecNotify(
UINT32 u4ImgID,
void *pv_tag,
void *pv_data,
UINT32 u4State)
{
	if(u4State == JPG_HW_NFY_FINISHED)
	{
		_rDecParam.depth = _rDecOut.u4Depth << 3;
		_bStartDec = FALSE;
		//_InjectVideoPlane();
	}
	else if(u4State == JPG_HW_NFY_ERROR)
	{
		_bStartDec = FALSE;	
		if((UINT32)pv_data == IMG_DRV_NOT_SUPPORT)
		{
			_u1DecErr = 2;
		}
		else	//eState==IMG_DRV_HW_ERROR
		{
			_u1DecErr = 1;
		}
	}
}


//----------------------------------------------------------------------------
SdResult _SdVideo_Disconnect(SdVideoID_k eVideoId,UINT8 FromSdImage);

static INT32 _Image_Open(unsigned long arg)
{
	SDAL_IOCTL_2ARG_T rSdalArg;
	SdImage_Attrib_t rAttrib;
	IMG_VP_INFO_T rVpInfo;
	VDP_REGION_T rRegion;
	SdVideo_Settings_t rSettings;
	IMG_CAP_T rCap;
	UCHAR ucSrcFullRegion;
	UINT32 u4PanelW, u4PanelH, u4YSz, u4CSz;

	// Check access of user space
	if (!access_ok(VERIFY_READ, (void __user *)arg,  sizeof(SDAL_IOCTL_2ARG_T) ))
	{
		printk("_Image_Open arg access error\n");
		return SD_ERR_SYS;
	}
	// Copy user space argument
	if(copy_from_user(&rSdalArg, (void __user *)arg, sizeof(SDAL_IOCTL_2ARG_T)))
	{
		printk("_Image_Open copy_from_user error\n");
		return SD_ERR_SYS;
	}
	// Copy user space argument
	if(copy_from_user(&rAttrib, (void __user *)rSdalArg.au4Arg[1], sizeof(SdImage_Attrib_t)))
	{
		printk("_Image_Open copy_from_user error\n");
		return SD_ERR_SYS;
	}
	_u1DecErr = 0;
	_eVideoId = (SdVideoID_k)rSdalArg.au4Arg[0];

	if(sdal_video[_eVideoId].injectvideo==0)
	{
		//======== img library init
		IMG_Lib_Init();
		x_memset((void *)&_rDecParam, 0, sizeof(SdImage_DecParam_t));
		x_memset((void *)&_rDecOut, 0, sizeof(IMG_REGION_T));
		
		//======== connect/set video path
		_SdVideo_Disconnect(_eVideoId, 1);//disconnect first
		Sd_VdoPathInjectVideo(_eVideoId);
		if(sdal_video[_eVideoId].injectvideo == 0)
		{
			return SD_ERR_IN_USE;
		}

		//======== set full input/output region
		x_memset((void*)&rRegion, 0, sizeof(VDP_REGION_T));
		if(VDP_SetSrcRegion((UCHAR)_eVideoId, 1, rRegion) ==0)
		{
			return SD_NOT_OK;
		}

		//PMX_GetResolution(0, (UINT16*)&u4PanelW, (UINT16*)&u4PanelH);
		u4PanelW = PANEL_GetPanelWidth();
                u4PanelH = PANEL_GetPanelHeight();
		rRegion.u4Width = u4PanelW;
		rRegion.u4Height = u4PanelH;
		rRegion.u4X = 0;
		rRegion.u4Y = 0;
		if(VDP_SetOutRegion((UCHAR)_eVideoId, 0, rRegion)==0)
		{
			return SD_NOT_OK;
		}	
		if(IMG_GetVideoPlaneInfo(&rVpInfo) != E_IMG_OK)
		{
			return SD_NOT_OK;
		}
		VDP_GetOutRegion(_eVideoId, &ucSrcFullRegion, &rRegion);
	
		//======== reset input/out region
		Sd_VdoPathVideo_Get(_eVideoId, &rSettings);
		rSettings.sCaptureSize.vStart = 0;
		rSettings.sCaptureSize.hStart = 0;
		rSettings.sCaptureSize.hResolution = rVpInfo.u4Width;
		rSettings.sCaptureSize.vResolution = rVpInfo.u4Height;
		rSettings.sCaptureSize.hTotal = rVpInfo.u4Width;
		rSettings.sCaptureSize.vTotal = rVpInfo.u4Height;

		rSettings.sGeometry.x = 0;
		rSettings.sGeometry.y = 0;
		rSettings.sGeometry.w = rRegion.u4Width;
		rSettings.sGeometry.h = rRegion.u4Height;
		x_memcpy((void *)&_rsGeom, (void *)&rSettings.sGeometry, sizeof(SdGeometry_t));
		Sd_VdoPathVideo_Set(_eVideoId, &rSettings);

		//======== clear video buffer
		u4YSz = (UINT32)rVpInfo.pu1CurFrameAddr[1] - (UINT32)rVpInfo.pu1CurFrameAddr[0];
		u4CSz = ((UINT32)rVpInfo.pu1CurFrameAddr[0] < (UINT32)rVpInfo.pu1BackFrameAddr[0])?
			((UINT32)rVpInfo.pu1BackFrameAddr[0] - (UINT32)rVpInfo.pu1CurFrameAddr[1]):
			((UINT32)rVpInfo.pu1CurFrameAddr[0] - (UINT32)rVpInfo.pu1BackFrameAddr[1]);
		x_memset((void*)VIRTUAL((UINT32)rVpInfo.pu1CurFrameAddr[0]), 0, u4YSz);
		x_memset((void*)VIRTUAL((UINT32)rVpInfo.pu1CurFrameAddr[1]), 0x80, u4CSz);
		x_memset((void*)VIRTUAL((UINT32)rVpInfo.pu1BackFrameAddr[0]), 0, u4YSz);
		x_memset((void*)VIRTUAL((UINT32)rVpInfo.pu1BackFrameAddr[1]), 0x80, u4CSz);
	}
	
	//======== get JPG available frame buffer size
	IMG_Get(&rCap);
	rAttrib.bRenderRotate = TRUE;
	rAttrib.pUserBuffer = (UINT8*)PAGE_ALIGN(rCap.u4RsvAddr);
	rAttrib.decoderSubmitSize = BS_PKT_SIZE;
	rAttrib.userBufferSize = rCap.u4RsvTotalSz/*rCap.u4RsvSizeL*/ & PAGE_MASK; //for mmap function page align
	if(copy_to_user((void __user *)rSdalArg.au4Arg[1], &rAttrib, sizeof(SdImage_Attrib_t)))
	{
		printk("_Image_Open copy_to_user error\n");
		return SD_ERR_SYS;
	}
	GFX_Init();
	//GFXSC_Init();
	
	return SD_OK;
}


static INT32 _Image_Close(unsigned long arg)
{
	//_SdVideo_Disconnect(_eVideoId,1);
	_u1DecErr = 0;
	return SD_OK;
}

static INT32 _Image_DecInit(unsigned long arg)
{
	SDAL_IOCTL_2ARG_T rSdalArg;
	SdImage_DecParam_t rSettings;
	IMG_CAP_T rCap;
	
	// Check access of user space
	if (!access_ok(VERIFY_READ, (void __user *)arg,  sizeof(SDAL_IOCTL_2ARG_T)))
	{
		printk("_Image_DecInit arg access error\n");
		return SD_ERR_SYS;
	}
	// Copy user space argument
	if(copy_from_user(&rSdalArg, (void __user *)arg, sizeof(SDAL_IOCTL_2ARG_T)))
	{
		printk("_Image_DecInit copy_from_user error\n");
		return SD_ERR_SYS;
	}
	
	// Copy user space argument
	if(copy_from_user(&rSettings, (void __user *)rSdalArg.au4Arg[1], sizeof(SdImage_DecParam_t)))
	{
		printk("_Image_DecInit copy_from_user error\n");
		return SD_ERR_SYS;
	}

	//check image fomat
	if(rSettings.eFormat != SD_IMAGE_JPEG)
	{
		return SD_NOT_SUPPORTED;	
	}
	x_memcpy((void *)&_rDecParam, (void *)&rSettings, sizeof(SdImage_DecParam_t));
	IMG_Get(&rCap);
	_rDecParam.pPicBuffer = (UINT8*)PAGE_ALIGN(rCap.u4ScaleBufAddr);
	_rDecParam.picBufSize = rCap.u4MaxScaleBufSize & PAGE_MASK;	
	_eRotateDegree = CLOCKWISE_0;
	_bIsRawPic = FALSE;
	_u1DecErr = 0;
	
	return SD_OK;
}


static INT32 _Image_DecSubmit(unsigned long arg)
{
	SDAL_IOCTL_4ARG_T rSdalArg;
	IMG_PARAM_T rInfo;
	IMG_VP_INFO_T rVpInfo;
	IMG_LIB_FILL_RING_BUF_T rStreamBufInfo;
	IMG_CAP_T rCap;	
	UINT32 u4FillSize, u4Entry1FillSz, u4Entry2FillSz;
	INT32 i4Ret = SD_OK;
	
	//return if decoding error occur
	if(_u1DecErr==1)
	{
		//_u1DecErr = 0;
		return SD_NOT_OK;
	}
	else if(_u1DecErr==2)
	{
		//_u1DecErr = 0;	
		return SD_NOT_SUPPORTED;
	}
		
	// Check access of user space
	if (!access_ok(VERIFY_READ, (void __user *)arg,  sizeof(SDAL_IOCTL_4ARG_T) ))
	{
		printk("_Image_DecSubmit arg access error\n");
		return SD_ERR_SYS;
	}
	// Copy user space argument
	if(copy_from_user(&rSdalArg, (void __user *)arg, sizeof(SDAL_IOCTL_4ARG_T)))
	{
		printk("_Image_DecSubmit copy_from_user error\n");
		return SD_ERR_SYS;
	}
	
	//init
	IMG_Get(&rCap);
	if(_rDecParam.eFormat == SD_IMAGE_JPEG)
	{
		if(IMG_GetDecState()==IMG_RELEASE)
		{
			if(IMG_Init(RING_IN_DOUBLE_OUT, NULL, 0, _IMGDecNotify) != E_IMG_OK)
			{
				return SD_NOT_OK;
			}
			_eRotateDegree = CLOCKWISE_0;	//reset setting
			x_memset((void *)&_rDecOut, 0, sizeof(IMG_REGION_T));
		}

		//fill buffer
		//query available buffer size and info
		u4FillSize = rSdalArg.au4Arg[2];
		x_memset((void *)&rStreamBufInfo, 0, sizeof(IMG_LIB_FILL_RING_BUF_T));
		while((IMG_GetBSBufInfo(&rStreamBufInfo) < u4FillSize) && (_u1DecErr==0));	//wait for enough space 

		if(rStreamBufInfo.rEntry1.bValid)
		{
			if(rStreamBufInfo.rEntry1.u4RequiredLen < u4FillSize)
			{
				u4Entry1FillSz = rStreamBufInfo.rEntry1.u4RequiredLen;
				u4Entry2FillSz = u4FillSize - rStreamBufInfo.rEntry1.u4RequiredLen;
			}
			else
			{
				u4Entry1FillSz = u4FillSize;
				u4Entry2FillSz = 0;
			}
		}
		else
		{
			u4Entry1FillSz = 0;
			u4Entry2FillSz = u4FillSize;
		}
		// Copy user space argument
		if(rStreamBufInfo.rEntry1.pvStartAddr)
		{		
			if(copy_from_user((void *)VIRTUAL((UINT32)rStreamBufInfo.rEntry1.pvStartAddr), 
						(void __user *)rSdalArg.au4Arg[1], 
						u4Entry1FillSz))
			{
				printk("_Image_DecSubmit copy_from_user error\n");
				return SD_ERR_SYS;
			}
		}
		if(rStreamBufInfo.rEntry2.pvStartAddr)
		{
			if(copy_from_user((void *)VIRTUAL((UINT32)rStreamBufInfo.rEntry2.pvStartAddr), 
						(void __user *)(rSdalArg.au4Arg[1] + u4Entry1FillSz), 
						u4Entry2FillSz))
			{
				printk("_Image_DecSubmit copy_from_user error\n");
				return SD_ERR_SYS;
			}
		}
		IMG_SetFillBsBufEnd(u4Entry1FillSz, u4Entry2FillSz, (u4FillSize < BS_PKT_SIZE)?TRUE:FALSE);

	
		//decode
		if((IMG_GetDecState()==IMG_INIT) && (_bStartDec == FALSE))
		{
			if((IMG_GetBSBufInfo(&rStreamBufInfo) < u4FillSize) || (u4FillSize < BS_PKT_SIZE))
			{
				_bStartDec = TRUE;
				if(IMG_GetVideoPlaneInfo(&rVpInfo) != E_IMG_OK)
				{
					return SD_NOT_OK;
				}	
				rInfo.pu1InBuffer = NULL;		//use internal ring buffer
				rInfo.u8BufSize = 0;			//use internal ring buffer
				rInfo.rDstRegion.u4StartX = 0;
				rInfo.rDstRegion.u4StartY = 0;
				rInfo.rDstRegion.u4Width = 0;	//rVpInfo.u4Width;
				rInfo.rDstRegion.u4Height = 0;	//rVpInfo.u4Height;
				rInfo.rDstRegion.u4Pitch = 0;	//rVpInfo.u4Width;

				rInfo.eDispColorMode = Y_CBCR422_SEP16;
				rInfo.eRotateDegree = CLOCKWISE_0;
				rInfo.bKeepRatio = TRUE;
				rInfo.prDecOut = &_rDecOut;

				rInfo.rDstRegion.pu1Addr = NULL;
				rInfo.rDstRegion.u4CbCrOffset = 0;

				if(IMG_Decode(rInfo) != E_IMG_OK)
				{
					return SD_NOT_OK;
				}
			}
		}
	}
	else
	{
		return SD_NOT_SUPPORTED;	//neither JPEG nor RAW type
	}
	
	if(_u1DecErr == 0)
	{
		return SD_OK;
	}
	else
	{
		i4Ret = (_u1DecErr == 2)? SD_NOT_SUPPORTED: SD_NOT_OK;
		//_u1DecErr = 0;
		return i4Ret;
	}
}


static INT32 _Image_DecStop(unsigned long arg)
{
	_bStartDec = FALSE;
	_u1DecErr = 3;

	//printk("[1201]Entering _Image_DecStop\n");
	while(IMG_GetDecState() == IMG_DECODE)
	{
		//printk("[1201]Waiting Underflow/Rel\n");
	}
	IMG_Stop();
	IMG_SetFillBsBufEnd(0, 0, TRUE);
	
	IMG_Release();
	//printk("[1201]Leaving _Image_DecStop\n");
	return SD_OK;
}

static INT32 _Image_Submit(unsigned long arg)
{
	SDAL_IOCTL_2ARG_T rSdalArg;
	SdImage_SubmitParam_t sSettings;
	UINT32 u4RawSz, u4Idx, u4Tmp, u4Y = 0, u4YCnt = 0;
	IMG_CAP_T rCap;
	static UINT32 u4X = 0;
	static UINT8* pu1YAddr;
	static UINT8* pu1CAddr;
	UINT8* pu1CrAddr;	
	static UINT8* pu1CAddrSc;
	static UINT8* pu1CAddrTmp;
	UINT8* pu1RawAddr;
	
	// Check access of user space
	if (!access_ok(VERIFY_READ, (void __user *)arg,  sizeof(SDAL_IOCTL_2ARG_T)))
	{
		printk("_Image_Submit arg access error\n");
		return SD_ERR_SYS;
	}
	// Copy user space argument
	if(copy_from_user(&rSdalArg, (void __user *)arg, sizeof(SDAL_IOCTL_2ARG_T)))
	{
		printk("_Image_Submit copy_from_user error\n");
		return SD_ERR_SYS;
	}
	
	// Copy user space argument
	if(copy_from_user(&sSettings, (void __user *)rSdalArg.au4Arg[1], sizeof(SdImage_SubmitParam_t)))
	{
		printk("_Image_Submit copy_from_user error\n");
		return SD_ERR_SYS;
	}
	x_memcpy((void *)&_rsGeom, (void *)&sSettings.sGeom, sizeof(SdGeometry_t));
	_eRotateDegree = (ENUM_IMG_ROTATE_PHASE)(((sSettings.rotateDegree>=360) ? 359 : sSettings.rotateDegree)/90);

	//raw type, submit and display
	if(_bIsRawPic) 
	{
		if((sSettings.sSize.w == 0) || (sSettings.sSize.h == 0))
		{
			return SD_PARAMETER_ERROR;
		}
		u4RawSz = (sSettings.sSize.w * sSettings.sSize.h)<<1;	//422

		IMG_Get(&rCap);
		if(u4RawSz > rCap.u4MaxScaleBufSize)
		{	//raw image too large
			return SD_ERR_NOMEM;
		}

		if(sSettings.eColorFormat == SD_COLOR_FORMAT_YCbCr444)
		{
			u4RawSz = (u4RawSz >>1) + u4RawSz;
		}
		else if(sSettings.eColorFormat != SD_COLOR_FORMAT_YCbCr422)
		{
			return SD_NOT_SUPPORTED;
		}

		//set raw image parameter
		_rDecOut.pu1Addr = (UINT8*)rCap.u4ScaleBufAddr;
		_rDecOut.u4Depth = 2;
		_rDecOut.u4Pitch = (sSettings.sSize.w + 0x10) & (~0xf); //should be 16 align
		_rDecOut.u4TotalLine = sSettings.sSize.h;
		_rDecOut.u4CbCrOffset = _rDecOut.u4Pitch * _rDecOut.u4TotalLine;
		_rDecOut.u4StartX = 0;
		_rDecOut.u4StartY = 0;
		_rDecOut.u4Width = sSettings.sSize.w;
		_rDecOut.u4Height = sSettings.sSize.h;
				
		pu1YAddr = (UINT8*)VIRTUAL(rCap.u4ScaleBufAddr);
		pu1CAddr = (UINT8*)VIRTUAL(rCap.u4ScaleBufAddr + _rDecOut.u4CbCrOffset);
		pu1CAddrSc = (UINT8*)VIRTUAL(PAGE_ALIGN(rCap.u4RsvAddr) + rCap.u4RsvSizeL - FBM_WORKING_BUFSZ);	//CbCr scaling src addr
		pu1CAddrTmp = pu1CAddrSc;
		u4X = 0;

		//copy raw image
		//x_memcpy((void *)VIRTUAL(rCap.u4WorkingAddr), (void *)VIRTUAL(rCap.u4ScaleBufAddr), sSettings.picBufSize);
		/*if(copy_from_user((void *)VIRTUAL(rCap.u4WorkingAddr), 
			(void __user *)sSettings.pPicBuffer, sSettings.picBufSize))
		{
			printk("_Image_Submit copy_from_user error\n");
			return SD_ERR_SYS;
		}*/
		pu1RawAddr = (UINT8*)VIRTUAL((UINT32)sSettings.pPicBuffer);
		//(UINT8*)VIRTUAL(PAGE_ALIGN(rCap.u4RsvAddr));
		if(sSettings.eColorFormat == SD_COLOR_FORMAT_YCbCr422)
		{
			//transfer "CbYCrY" to "Y, CbCr seperate"
			//pu1CrAddr = pu1CAddr + (_rDecOut.u4Width >> 1);
			for(u4Idx = 0; u4Idx < sSettings.picBufSize; u4Idx +=4)
			{
				u4X += 2;
				*pu1CAddr++ = *pu1RawAddr++;			
				*pu1YAddr++ = *pu1RawAddr++;
				//*pu1CrAddr++ = *pu1RawAddr++;	
				*pu1CAddr++ = *pu1RawAddr++;
				*pu1YAddr++ = *pu1RawAddr++;				
				if((u4X % _rDecOut.u4Width) == 0)
				{
					u4X = 0;
					u4Tmp = _rDecOut.u4Pitch - _rDecOut.u4Width;
					x_memset((void*)pu1YAddr, 0, u4Tmp);					
					//x_memset((void*)pu1CrAddr, 0x80, u4Tmp);
					x_memset((void*)pu1CAddr, 0x80, u4Tmp);
					pu1YAddr += u4Tmp;
					//pu1CAddr = pu1CrAddr + u4Tmp;
					pu1CAddr += u4Tmp;
					//pu1CrAddr = pu1CAddr + (_rDecOut.u4Width >> 1);
				}
			}
		}
		else		//444, transfer "YCbCr" to "Y, CbCr seperate"
		{
			//u4YCnt = (FBM_WORKING_BUFSZ >>1)/_rDecOut.u4Pitch;	//CbCr size
			//pu1CrAddr = pu1CAddrTmp + _rDecOut.u4Width;
			for(u4Idx = 0; u4Idx < sSettings.picBufSize; u4Idx +=3)
			{
				u4X++;
				*pu1YAddr++ = *pu1RawAddr++;			
				//*pu1CAddrTmp++ = *pu1RawAddr++;	//Cb
				//*pu1CrAddr++ = *pu1RawAddr++;	//Cr				
				if (u4X % 2)
				{
				    *pu1CAddr++ = *pu1RawAddr++;        //cb
    				*pu1CAddr++ = *pu1RawAddr++;        //cr
				}
				else
				{
				    pu1RawAddr += 2;
				}
				if((u4X % _rDecOut.u4Width) == 0)
				{
					u4X = 0;
					u4Y++;
					u4Tmp = _rDecOut.u4Pitch - _rDecOut.u4Width;
					x_memset((void*)pu1YAddr, 0, u4Tmp);					
					//x_memset((void*)pu1CrAddr, 0x80, u4Tmp<<1); //Cr
					x_memset((void*)pu1CAddr, 0x80, u4Tmp);         //CbCr
					
					pu1YAddr += u4Tmp;
					//pu1CAddrTmp = pu1CrAddr + (u4Tmp<<1);					
					//pu1CrAddr = pu1CAddrTmp + _rDecOut.u4Width;
					//pu1CAddr = pu1CrAddr + u4Tmp;
					pu1CAddr += u4Tmp;
#if 0					
					if((u4Y==u4YCnt) || (u4Idx == (sSettings.picBufSize - 3)))
					{

						//CbCr scaling
						//CbCr, 2 byte per pixel
						//HalFlushInvalidateDCache();
						GFX_Lock();
						
						//Cb
						GFXSC_SetSrc((UINT8*)PHYSICAL((UINT32)pu1CAddrSc), (UINT32)CM_SC_SINGLE, _rDecOut.u4Pitch<<1);         
						GFXSC_SetDst((UINT8*)PHYSICAL((UINT32)pu1CAddr), (UINT32)CM_SC_SINGLE, _rDecOut.u4Pitch);

						GFXSC_Scale(0, 0, 0, 0,
						 	_rDecOut.u4Width, u4Y,
						 	_rDecOut.u4Width>>1, u4Y);
						GFXSC_Flush();
						GFXSC_Wait();
						//Cr
						GFXSC_SetSrc((UINT8*)PHYSICAL((UINT32)pu1CAddrSc + _rDecOut.u4Width), (UINT32)CM_SC_SINGLE, _rDecOut.u4Pitch<<1);         
						GFXSC_SetDst((UINT8*)PHYSICAL((UINT32)pu1CAddr + (_rDecOut.u4Width >> 1)), (UINT32)CM_SC_SINGLE, _rDecOut.u4Pitch);

						GFXSC_Scale(0, 0, 0, 0,
						 	(_rDecOut.u4Pitch + u4Tmp), u4Y, 
						 	(_rDecOut.u4Pitch + u4Tmp)>>1, u4Y);
						GFXSC_Flush();
						GFXSC_Wait();
						GFX_Unlock();
						
						pu1CAddr += (_rDecOut.u4Pitch * u4Y);
						u4Y = 0;
						pu1CAddrTmp = pu1CAddrSc;
						pu1CrAddr = pu1CAddrTmp + _rDecOut.u4Width;
					}
#endif					
				}
			}
		}
	}
	
	return _InjectVideoPlane();
	
}

static INT32 _Image_FillRect(unsigned long arg)
{
	SDAL_IOCTL_3ARG_T rSdalArg;
	SdGeometry_t sGem;
	IMG_VP_INFO_T rVpInfo;
	UINT32 u4YValue, u4CbValue, u4CrValue;
	
	// Check access of user space
	if (!access_ok(VERIFY_READ, (void __user *)arg,  sizeof(SDAL_IOCTL_3ARG_T)))
	{
		printk("_Image_FillRect arg access error\n");
		return SD_ERR_SYS;
	}
	// Copy user space argument
	if(copy_from_user(&rSdalArg, (void __user *)arg, sizeof(SDAL_IOCTL_3ARG_T)))
	{
		printk("_Image_FillRect copy_from_user error\n");
		return SD_ERR_SYS;
	}
	
	if(copy_from_user(&sGem, (void __user *)rSdalArg.au4Arg[1], sizeof(SdGeometry_t)))
	{
		printk("_Image_FillRect copy_from_user error\n");
		return SD_ERR_SYS;
	}

	//error handling
	if((sGem.w == 0) || (sGem.h == 0)) 
	{
		return SD_PARAMETER_ERROR;
	}

	if(IMG_GetVideoPlaneInfo(&rVpInfo) != E_IMG_OK)
	{
		return SD_NOT_OK;
	}

	//error handling
	if((rVpInfo.u4Width < sGem.x) || (rVpInfo.u4Height < sGem.y) ||
		(rVpInfo.u4Width < (sGem.x + sGem.w)) || 
		(rVpInfo.u4Height < (sGem.y + sGem.h)))
	{
		return SD_PARAMETER_ERROR;
	}
	
	u4YValue = (rSdalArg.au4Arg[2] >> 16) & 0xff;
	u4CbValue = (rSdalArg.au4Arg[2] >> 8) & 0xff;
	u4CrValue = rSdalArg.au4Arg[2] & 0xff;
	
	//fill Y value
	GFX_Lock();
	GFX_SetDst(rVpInfo.pu1CurFrameAddr[0], (UINT32)CM_ARGB4444_DIRECT16, rVpInfo.u4Width);
	GFX_SetColor((u4YValue << 8) + u4YValue);
	GFX_Fill(sGem.x>>1, sGem.y, sGem.w>>1, sGem.h);
	
	//fill CbCr value
	GFX_SetDst(rVpInfo.pu1CurFrameAddr[1], (UINT32)CM_ARGB4444_DIRECT16, rVpInfo.u4Width);
	GFX_SetColor((u4CrValue << 8) + u4CbValue);
	GFX_Fill(sGem.x>>1, sGem.y, sGem.w>>1, sGem.h);

	GFX_Flush();
	GFX_Wait();
	GFX_Unlock();
	
	return SD_OK;
}

static INT32 _Image_GetStatus(unsigned long arg)
{
	SDAL_IOCTL_4ARG_T rSdalArg;
	SdImage_Status_t rState;
	
	// Check access of user space
	if (!access_ok(VERIFY_READ, (void __user *)arg,  sizeof(SDAL_IOCTL_4ARG_T)))
	{
		printk("_Image_GetStatus arg access error\n");
		return SD_ERR_SYS;
	}
	// Copy user space argument
	if(copy_from_user(&rSdalArg, (void __user *)arg, sizeof(SDAL_IOCTL_4ARG_T)))
	{
		printk("_Image_GetStatus copy_from_user error\n");
		return SD_ERR_SYS;
	}

	// Check access of user space
	if (!access_ok(VERIFY_READ, (void __user *)rSdalArg.au4Arg[2], sizeof(SdImage_Status_t)))
	{
		printk("_Image_GetStatus arg access error\n");
		return SD_ERR_SYS;
	}
	// Copy user space argument
	if(copy_from_user(&rState, (void __user *)rSdalArg.au4Arg[2], sizeof(SdImage_Status_t)))
	{
		printk("_Image_GetStatus copy_from_user error\n");
		return SD_ERR_SYS;
	}

	rState.bOnGoing = (IMG_GetDecState()==IMG_RELEASE)?FALSE:TRUE;
	x_memcpy((void *)&rState.sDecParam, (void *)&_rDecParam, sizeof(SdImage_DecParam_t));

	rState.sDecParam.pPicBuffer = _rDecOut.pu1Addr;
	rState.sDecParam.sSize.w = _rDecOut.u4Pitch;
	rState.sDecParam.sSize.h = _rDecOut.u4TotalLine;
	if(copy_to_user((void __user *)rSdalArg.au4Arg[2], &rState, sizeof(SdImage_Status_t)))
	{
		printk("_Image_GetStatus copy_to_user error\n");
		return SD_ERR_SYS;
	}
	//return if decoding error occur
	if(_u1DecErr==1)
	{
		//_u1DecErr = 0;
		return SD_NOT_OK;
	}
	else if(_u1DecErr==2)
	{
		//_u1DecErr = 0;	
		return SD_NOT_SUPPORTED;
	}
	else
	{
		return SD_OK;
	}
}


static INT32 _Image_UseVideoMem(unsigned long arg)
{
	SDAL_IOCTL_2ARG_T rSdalArg;
	SdImage_Attrib_t rAttrib;
	IMG_CAP_T rCap;

	// Check access of user space
	if (!access_ok(VERIFY_READ, (void __user *)arg,  sizeof(SDAL_IOCTL_2ARG_T) ))
	{
		printk("_Image_UseVideoMem arg access error\n");
		return SD_ERR_SYS;
	}
	// Copy user space argument
	if(copy_from_user(&rSdalArg, (void __user *)arg, sizeof(SDAL_IOCTL_2ARG_T)))
	{
		printk("_Image_UseVideoMem copy_from_user error\n");
		return SD_ERR_SYS;
	}
	// Copy user space argument
	if(copy_from_user(&rAttrib, (void __user *)rSdalArg.au4Arg[1], sizeof(SdImage_Attrib_t)))
	{
		printk("_Image_UseVideoMem copy_from_user error\n");
		return SD_ERR_SYS;
	}
	_bIsRawPic = (rSdalArg.au4Arg[0]==1)?TRUE:FALSE;

	//======== get JPG available frame buffer size
	IMG_Get(&rCap);
	rAttrib.bRenderRotate = TRUE;
	rAttrib.pUserBuffer = (UINT8*)PAGE_ALIGN(rCap.u4RsvAddr);
	rAttrib.decoderSubmitSize = BS_PKT_SIZE;
	if(_bIsRawPic)
	{
		rAttrib.userBufferSize = (rCap.u4RsvSizeL - FBM_WORKING_BUFSZ) & PAGE_MASK; //for mmap function page align
	}
	else
	{
		rAttrib.userBufferSize = rCap.u4RsvSizeS & PAGE_MASK; //for mmap function page align	
	}
	if(copy_to_user((void __user *)rSdalArg.au4Arg[1], &rAttrib, sizeof(SdImage_Attrib_t)))
	{
		printk("_Image_UseVideoMem copy_to_user error\n");
		return SD_ERR_SYS;
	}
	
	return SD_OK;
}


static INT32 _Image_Debug(void)
{
	return SD_OK;
}


int sdal_ioctl_sdimage(struct inode *inode, struct file *file, unsigned int cmd,
              unsigned long arg)
{
	int i4Ret = 0;

	switch (cmd) 
	{
		//printk("cmd 0x%x\n", cmd);
		case SDAL_IO_IMAGE_OPEN:
			i4Ret = _Image_Open(arg);
			break;

		case SDAL_IO_IMAGE_CLOSE:
			i4Ret = _Image_Close(arg);
			break;
			
		case SDAL_IO_IMAGE_DEC_INIT:
			i4Ret = _Image_DecInit(arg);
			break;

		case SDAL_IO_IMAGE_DEC_SUBMIT:
			i4Ret = _Image_DecSubmit(arg);
			break;

		case SDAL_IO_IMAGE_DEC_STOP:
			i4Ret = _Image_DecStop(arg);
			break;
			
		case SDAL_IO_IMAGE_SUBMIT:	//submit for display
			i4Ret = _Image_Submit(arg);
			break;

		case SDAL_IO_IMAGE_FILLRECT:
			i4Ret = _Image_FillRect(arg);
			break;

		case SDAL_IO_IMAGE_GETSTATUS:
			i4Ret = _Image_GetStatus(arg);
			break;

		case SDAL_IO_IMAGE_USE_VMEM:
			i4Ret = _Image_UseVideoMem(arg);
			break;
			
		case SDAL_IO_IMAGE_DEBUG:
			i4Ret = _Image_Debug();
			break;	
			
		default:
			i4Ret = -1;
			break;
	}
	return i4Ret;
}





