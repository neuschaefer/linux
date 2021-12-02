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
 * $Author: muran.xiang $
 * $Date: 2012/08/01 $
 * $RCSfile: io_ice_hdmi.c,v $
 * $Revision: #5 $
 *
 *---------------------------------------------------------------------------*/

/** @file io_ice_hdmi.c
 *  ICE hdmi driver interface.
 */

#include "mttype.h"
#include "ice.h"
#include "ice_hdmi.h"
#include "drv_hdmi.h"
#include "sv_const.h"
#include "drv_dvi.h"
#include "video_timing.h"
#include "cb_data.h"
#include "api_notify.h"

static UINT8 bHDMIKernelLogLevel = 10;
#define HDMI_LOG_LEVEL bHDMIKernelLogLevel
#define HDMI_LOG_PROMPT "[KICE_"
#define HDMI_Printf(lv, fmt...) \
    do \
    {\
        if(((HDMI_LOG_LEVEL) >= (lv))) \
        { \
            printf(HDMI_LOG_PROMPT"HDMI]:%s()     " , __FUNCTION__); \
            printf(fmt); \
        } \
    } while (0)


extern INT32 _CB_PutEvent(CB_FCT_ID_T eFctId, INT32 i4TagSize, void *pvTag);

static MT_RESULT_T _ICE_HDMIDebugLevel(unsigned long arg)
{
	int i4Ret = MTR_OK;
	printf("pre d_l:%d; now d_l:%d\n", bHDMIKernelLogLevel, arg );
    bHDMIKernelLogLevel = arg;
	return i4Ret;
}

static MT_RESULT_T _ICE_HDMIInit(void)
{
    int i4Ret = MTR_OK;

    HDMI_Printf(10, "---------------vHDMIInit-----------------\n");
    vHDMIInit();
    return i4Ret;
}

ICE_HDMIOperationResult _ICE_HDMISetResolution(unsigned long arg)
{
	int i4Ret = MTR_OK;
    ICE_HDMIOutputResolution HDMI_video_output_resolution_mode;
    frost_bool do_not_use_delay_and_mute;
    frost_bool control_for_support;
	MTAL_IOCTL_3ARG_T rArg;

	USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
	COPY_FROM_USER_3ARG(arg,rArg);
	
	HDMI_video_output_resolution_mode = (ICE_HDMIOutputResolution)rArg.ai4Arg[0];
	do_not_use_delay_and_mute = (frost_bool)rArg.ai4Arg[1];
	control_for_support = (frost_bool)rArg.ai4Arg[2];
	HDMI_Printf(2, "%d, %d, %d\n", HDMI_video_output_resolution_mode, do_not_use_delay_and_mute, control_for_support);

	return i4Ret;
}


/**
 * @brief Sets a HDMI video output colour space (RGB, YCbCr).
 *
 * @param HDMI_video_output_colour_space_mode   The new HDMI video output
 *                                              colour space.
 *
 * @return ICE_HDMIOperationSuccess on success, ICE_HDMIOperationFail on
 *         failure, or ICE_HDMIOperationNotSupported if not supported.
 */
ICE_HDMIOperationResult _ICE_HDMISetOutputColourSpace(unsigned long arg)
{
	int i4Ret = MTR_OK;
	ICE_HDMIOutputColourSpace HDMI_video_output_colour_space_mode;
	HDMI_video_output_colour_space_mode = (ICE_HDMIOutputColourSpace)arg;
	HDMI_Printf(2, "HDMI_video_output_colour_space_mode = %d\n", HDMI_video_output_colour_space_mode); 
	return i4Ret;


}

/**
 * @brief Returns current HDMI output colour space (RGB, YCbCr).
 *
 * @return The current HDMI output colour space.
 */
ICE_HDMIOperationResult _ICE_HDMIGetCurrentOutputColourSpace(unsigned long arg)
{
	int i4Ret = MTR_OK;
	
	ICE_HDMIOutputColourSpace * pCurOutputColorSpace = (ICE_HDMIOutputColourSpace *)arg;
	ICE_HDMIOutputColourSpace   rCurOutputColorSpace = ICE_HDMI_YCbCr_Colour_Space;
	
	switch(bHDMIColorFormat(0))
	{
		case SV_COLOR_HDMI_RGB:
			rCurOutputColorSpace = ICE_HDMI_RGB_Colour_Space;
			break;
		case SV_COLOR_HDMI_422_601:
		case SV_COLOR_HDMI_422_709:
		case SV_COLOR_HDMI_444_601:
		case SV_COLOR_HDMI_444_709:
		case SV_COLOR_HDMI_XVYCC_601:
		case SV_COLOR_HDMI_XVYCC_709:
			rCurOutputColorSpace = ICE_HDMI_YCbCr_Colour_Space;
			break;
		default:
			rCurOutputColorSpace = ICE_HDMI_RGB_Colour_Space;
			break;
	}
	
	USR_SPACE_ACCESS_VALIDATE_ARG(pCurOutputColorSpace, ICE_HDMIOutputColourSpace);
	COPY_TO_USER_ARG(pCurOutputColorSpace, rCurOutputColorSpace, ICE_HDMIOutputColourSpace);
	HDMI_Printf(2, "");
	return i4Ret;
}

ICE_HDMIOperationResult _ICE_HDMISetAspectRatio(unsigned long arg)
{
	int i4Ret = MTR_OK;
    ICE_HDMITVAspectRatio HDMI_video_output_aspect_ratio;
    frost_bool do_not_use_delay_and_mute;
	MTAL_IOCTL_2ARG_T rArg;

	USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
	COPY_FROM_USER_2ARG(arg, rArg);
	
	HDMI_video_output_aspect_ratio = (ICE_HDMITVAspectRatio)rArg.ai4Arg[0];
	do_not_use_delay_and_mute = (frost_bool)rArg.ai4Arg[1];
	HDMI_Printf(2, "%d, %d\n", HDMI_video_output_aspect_ratio, do_not_use_delay_and_mute);

	return i4Ret;
}

ICE_HDMIOperationResult _ICE_HDMIGetCurrentHDMIVideoOutputResolution(unsigned long arg)
{
	int i4Ret = MTR_OK;
	ICE_HDMIOutputResolution rHDMI_Output_Resolution = ICE_HDMI_576p_Resolution;
	ICE_HDMIOutputResolution * pHDMI_Output_Resolution = (ICE_HDMIOutputResolution *)arg;

	switch(bDviIsVideoTiming())
	{
		case 1:
			switch(_bDviTiming)
			{
				case MODE_625I_OVERSAMPLE:
				case MODE_625I:
					rHDMI_Output_Resolution = ICE_HDMI_576i_Resolution;
					break;
				case MODE_576P_OVERSAMPLE:
				case MODE_576P:
				case MODE_576P_25:
					rHDMI_Output_Resolution = ICE_HDMI_576p_Resolution;
					break;
				case MODE_720p_50:
				case MODE_720p_60:
				case MODE_720p_24:
				case MODE_720p_25:
				case MODE_720p_30:
					rHDMI_Output_Resolution = ICE_HDMI_720p_Resolution;
					break;
				case MODE_1080i:
					rHDMI_Output_Resolution = ICE_HDMI_1080i_Resolution;
					break;
				default:
					rHDMI_Output_Resolution = ICE_HDMI_576p_Resolution;
					break;
			}
			break;
			
		default:
			rHDMI_Output_Resolution = ICE_HDMI_576p_Resolution;
			break;
	}
	
	USR_SPACE_ACCESS_VALIDATE_ARG(pHDMI_Output_Resolution, ICE_HDMIOutputResolution);
	COPY_TO_USER_ARG(pHDMI_Output_Resolution, rHDMI_Output_Resolution, ICE_HDMIOutputResolution);
	HDMI_Printf(2, "");
	return i4Ret;
}
ICE_HDMIOperationResult _ICE_HDMISetCurrentHDMIVideoOutputResolution(unsigned long arg)
{
	int i4Ret = MTR_OK;
	ICE_HDMIOutputResolution video_output_resolution = (ICE_HDMIOutputResolution)arg;
	HDMI_Printf(2, "video_output_resolution = %d\n", video_output_resolution); 
	return i4Ret;
}

extern UINT8  _bDviModeChged;
ICE_HDMIOperationResult _ICE_HDMIGetCurrentAspectRatio(unsigned long arg)
{
    int i4Ret = MTR_OK;
    ICE_HDMITVAspectRatio rHDMI_Get_Current_Aspect_Ratio = ICE_HDMI_TV_aspect_16_9;
    ICE_HDMITVAspectRatio * pHDMI_Get_Current_Aspect_Ratio = (ICE_HDMITVAspectRatio *)arg;

    if(0 == _bDviModeChged)//mode detect done
    {
        switch(bHDMIAspectRatio())
        {
            case 1:
            rHDMI_Get_Current_Aspect_Ratio = ICE_HDMI_TV_aspect_4_3;
            break;
            case 2:
            rHDMI_Get_Current_Aspect_Ratio = ICE_HDMI_TV_aspect_16_9;
            break;
            default:
            rHDMI_Get_Current_Aspect_Ratio = ICE_HDMI_TV_aspect_auto;
            break;
        }
    }
    else//still mode detecting;
    {
        rHDMI_Get_Current_Aspect_Ratio = ICE_HDMI_TV_aspect_16_9;
    }
      
    USR_SPACE_ACCESS_VALIDATE_ARG(pHDMI_Get_Current_Aspect_Ratio, ICE_HDMITVAspectRatio);
    COPY_TO_USER_ARG(pHDMI_Get_Current_Aspect_Ratio, rHDMI_Get_Current_Aspect_Ratio, ICE_HDMITVAspectRatio);
    HDMI_Printf(2, "");
    return i4Ret;
}

ICE_HDMIOperationResult _ICE_HDMIGetCurrentAFD(unsigned long arg)
{
    int i4Ret = MTR_OK;
    UINT8 rHDMI_Get_Current_AFD = ICE_afd_not_present;
    ICE_HDMITVAspectRatio * pHDMI_Get_Current_AFD = (ICE_HDMITVAspectRatio *)arg;

    if(0 == _bDviModeChged)//mode detect done
    {
        rHDMI_Get_Current_AFD = bHDMIAFD();
    }
    else//still mode detecting;
    {
        rHDMI_Get_Current_AFD = ICE_afd_not_present;
    }
      
    USR_SPACE_ACCESS_VALIDATE_ARG(pHDMI_Get_Current_AFD, UINT8);
    COPY_TO_USER_ARG(pHDMI_Get_Current_AFD, rHDMI_Get_Current_AFD, UINT8);
    HDMI_Printf(2, "");
//    printf("_ICE_HDMIGetCurrentAFD = 0x%x_______________________________________\n", rHDMI_Get_Current_AFD);
    return i4Ret;
}

ICE_HDMIOperationResult _ICE_HDMISetCurrentAspectRatio(unsigned long arg)
{
	int i4Ret = MTR_OK;
	ICE_HDMITVAspectRatio aspect_ratio = (ICE_HDMITVAspectRatio)arg;
	HDMI_Printf(2, "aspect_ratio = %d\n", aspect_ratio); 
	return i4Ret;
}

ICE_HDMIOperationResult _ICE_HDMISetTVAspectRatio(unsigned long arg)
{
	int i4Ret = MTR_OK;
	ICE_HDMITVAspectRatio tv_aspect_ratio = (ICE_HDMITVAspectRatio)arg;
	HDMI_Printf(2, "tv_aspect_ratio = %d\n", tv_aspect_ratio); 
	return i4Ret;
}

ICE_HDMIOperationResult _ICE_HDMIHardwareResetTransmitter(unsigned long arg)
{
	int i4Ret = MTR_OK;
	HDMI_Printf(2, "hdmi hardware reset transmitter\n");
	return i4Ret;

}

ICE_HDMIOperationResult _ICE_HDMIHardwareResetScaler(unsigned long arg)
{
	int i4Ret = MTR_OK;
	HDMI_Printf(2, "hdmi hardware reset scaler\n");
	return i4Ret;

}

ICE_HDMIOperationResult _ICE_HDMIHardwareResetScalerAndTransmitter(unsigned long arg)
{
	int i4Ret = MTR_OK;
	HDMI_Printf(2, "hdmi hardware reset scaler and transmitter\n");
	return i4Ret;
}

ICE_HDMIOperationResult _ICE_HDMIMute(unsigned long arg)
{
	int i4Ret = MTR_OK;
	HDMI_Printf(2, "hdmi mute\n");
	return i4Ret;
}

ICE_HDMIOperationResult _ICE_HDMIUnmute(unsigned long arg)
{
	int i4Ret = MTR_OK;
	HDMI_Printf(2, "hdmi unmute\n");
	return i4Ret;

}

ICE_HDMIOperationResult _ICE_HDMIGetOutputAvailability(unsigned long arg)
{
	int i4Ret = MTR_OK;
	frost_bool rOutput_Availability = frost_true;
	frost_bool * pOutput_Availability = (frost_bool *)arg;
	USR_SPACE_ACCESS_VALIDATE_ARG(pOutput_Availability, frost_bool);
	COPY_TO_USER_ARG(pOutput_Availability, rOutput_Availability, frost_bool);
	HDMI_Printf(2, "");
	return i4Ret;
}

ICE_HDMIOperationResult _ICE_HDMIIsSimplayDefined(unsigned long arg)
{
	int i4Ret = MTR_OK;
	frost_bool rIsSimplayDefined = frost_true;
	frost_bool * pIsSimplayDefined = (frost_bool *)arg;
	USR_SPACE_ACCESS_VALIDATE_ARG(pIsSimplayDefined, frost_bool);
	COPY_TO_USER_ARG(pIsSimplayDefined, rIsSimplayDefined, frost_bool);
	HDMI_Printf(2, "");
	return i4Ret;
}

ICE_HDMIOperationResult _ICE_HDMIHDCPEnableDisable(unsigned long arg)
{
	int i4Ret = MTR_OK;
	frost_bool enable = (frost_bool)arg;
	HDMI_Printf(2, "enable = %d\n", enable); 
	return i4Ret;
}

ICE_HDMIOperationResult _ICE_HDMIHDCPReceiveSRM(unsigned long arg)
{
	int i4Ret = MTR_OK;
	HDMI_Printf(2, "");
	return i4Ret;

}

ICE_HDMIOperationResult _ICE_HDMIAudioDelay(unsigned long arg)
{
	int i4Ret = MTR_OK;
	HDMI_Printf(2, "");
	return i4Ret;
}

typedef struct _HDMI_CB_T_
{
    UINT32 u4Arg1;
    UINT32 u4Arg2;
}HDMI_CB_T;
extern INT32 _CB_PutEvent(CB_FCT_ID_T eFctId, INT32 i4TagSize, void *pvTag);

static void _ICE_HDMI_CB_FUNC(UINT32 bHdmiPort, UINT32 bStatus)
{
    HDMI_CB_T cbArg; 
    
    cbArg.u4Arg1 = bHdmiPort;
    cbArg.u4Arg2 = bStatus;
    _CB_PutEvent(CB_DRV_HDMI_NFY, sizeof(cbArg), (void*)&cbArg);
}
extern INT32 NPTV_SetHdmiNfy(const NPTV_PFN_HDMI_NFY pfnFct);

extern UINT8 bHDMIStatusCanBeNotified;
extern UINT8 bHDMITurnOnAutoDetect;
ICE_HDMIOperationResult _ICE_HDMI_Reg_Cb_func(unsigned long arg)
{
    int i4Ret = MTR_OK;
    int rArg;
    
    COPY_FROM_USER_ARG(arg, rArg, int);
    NPTV_SetHdmiNfy(_ICE_HDMI_CB_FUNC);
    bHDMIStatusCanBeNotified = 1;
    bHDMITurnOnAutoDetect = (UINT8)rArg;
    return i4Ret;
}

ICE_HDMIOperationResult _ice_HDMIPrint(unsigned long arg)
{
    int i4Ret = MTR_OK;
    INT32 param1, param2, param3;
    MTAL_IOCTL_3ARG_T rArg;
    
    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    param1 = (INT32)rArg.ai4Arg[0];
    param2 = (INT32)rArg.ai4Arg[1];
    param3 = (INT32)rArg.ai4Arg[2];
    Printf("0x%x, 0x%x, 0x%x______________________________________________________\n", param1, param2, param3);
    return i4Ret;
}
extern INT32 STORG_SyncNandRead(UINT32 u4PartId, UINT32 u4Offset, VOID* pvBuf, UINT32 u4BufSize);


extern INT32 NORPART_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);

UINT8 bIsHDCPKeyOK = 0;//0:NG;1:OK
UINT32 AddCrc( UINT32 crc, UINT8 b )
{
    UINT32 data;
    UINT32 crc_out;
    UINT32 cs;
    data = b;
    crc_out = crc ^ data;
    cs = (1 + (crc_out>>7) + (crc_out>>16))& 0x0001;
    crc_out = ((crc_out << 1) + cs) & 0x1ffff;
    return crc_out;
}

UINT8 bHDCPKeyCRCCheck(UINT8 * pHdcpKey)
{
    UINT32 i = 0;
    UINT32 CRC_result = 0xA3C5;
    UINT32 bCRC[2];
    for ( i = 0; i < 328; i++)
    {
        if(i < 320)
        {
            if ((i != 6) && (i != 7) && (i != 8))
            {
                CRC_result = AddCrc( CRC_result, *(pHdcpKey+i));
            }
        }
        else
        {
            CRC_result = AddCrc( CRC_result, 0);
        }
    }
    bCRC[0] = *(pHdcpKey+6);
    bCRC[1] = *(pHdcpKey+7);
    printf("bCRC[0] = 0x%x, bCRC[1] = 0x%x, CRC_result = 0x%x\n", bCRC[0], bCRC[1], CRC_result);
    if((bCRC[0] + (bCRC[1]<<8)) == (CRC_result & 0xffff))
    {
        printf("hdcpkey crc ok\n");
        return 1;
    }
    else
    {
        printf("hdcpkey crc fail\n");
        return 0;
    }
}


ICE_HDMIOperationResult _ice_HDMILoadHDCPKey(unsigned long arg)
{
#if 0
    int i4Ret = MTR_OK;
#if 1

    char kernelbuf[320];
    UINT8 bTmp = 0;  
    UINT32 u4MemLen = 320;
    UINT64 u8Offset = 0;
    UINT32 i = 0;
    
    struct file *file = file_open("/ro/hdcpkey.bin", O_RDONLY, 0);

    if (!file)
    {
        return MTR_NOT_OK;
    }

    u4MemLen = file_read(file, (void*)kernelbuf, u4MemLen, &u8Offset);

    file_close(file);
    for(i = 0; i<10; i++)
    Printf("kernelbuf[i] = 0x%x, i = %d, u4MemLen = %d________________\n", kernelbuf[i], i, u4MemLen);
#if 1//hdcp key tool compatible.
    bTmp= kernelbuf[5];
    kernelbuf[5] = kernelbuf[4];
    kernelbuf[4] = kernelbuf[3];
    kernelbuf[3] = kernelbuf[2];
    kernelbuf[2] = kernelbuf[1];
    kernelbuf[1] = kernelbuf[0];
    kernelbuf[0] = bTmp;
#endif
    vHDMIHDCPKeyInit(&kernelbuf[0]);
#endif
    return i4Ret;
#else
    UINT32 u4MemLen = 320;
    UINT32 i = 0;
    UINT64 u8CurOff;
    char hdcpkey[320];
    UINT8 bTmp = 0;  
    int i4Ret = MTR_OK;

    UINT32 u4HdcpPartition = 3;
    UINT32 u4HdcpOffset = 0;

    INT32 ret = 0;
    
    ret = DRVCUST_InitQuery(eHDCPNorPartId, &u4HdcpPartition);
    if(0 != ret)
    {
        Printf("hdcp key error 1\n");
        return MTR_NOT_OK;
    }
 
    ret = DRVCUST_InitQuery(eHDCPNorOffset, &u4HdcpOffset);
    if(0 != ret)
    {
        Printf("hdcp error 2\n");
        return MTR_NOT_OK;
    }
    
    u8CurOff = (UINT64)(u4HdcpOffset);
    u8CurOff |= (((UINT64)(u4HdcpPartition)) << 32);
    Printf("u4HdcpPartition= %d\n", u4HdcpPartition);
    Printf("u4HdcpOffset= %d\n", u4HdcpOffset);
    Printf("_ice_HDMILoadHDCPKey current flag: 0X%x,0X%x,0X%x_________________________\n", u8CurOff, u4HdcpPartition, u4HdcpOffset);
    NORPART_Read(u8CurOff, (UINT32)(&hdcpkey[0]), u4MemLen);
    
    for(i = 0; i<10; i++)
    Printf("hdcpkey[i] = 0x%x, i = %d, u4MemLen = %d++++\n", hdcpkey[i], i, u4MemLen);
#if 1//hdcp key tool compatible.
    bTmp= hdcpkey[5];
    hdcpkey[5] = hdcpkey[4];
    hdcpkey[4] = hdcpkey[3];
    hdcpkey[3] = hdcpkey[2];
    hdcpkey[2] = hdcpkey[1];
    hdcpkey[1] = hdcpkey[0];
    hdcpkey[0] = bTmp;
#endif
    bIsHDCPKeyOK = bHDCPKeyCRCCheck(&hdcpkey[0]);
    vHDMIHDCPKeyInit(&hdcpkey[0]);
    return i4Ret;

#endif
}

MT_RESULT_T _ICE_HDMIIsHDCPKeyOK(unsigned long arg)
{
    MT_RESULT_T i4Ret = MTR_OK;
    UINT8 * pArg = (UINT8 *)arg;

    USR_SPACE_ACCESS_VALIDATE_ARG(pArg, UINT8);
    COPY_TO_USER_ARG(pArg, bIsHDCPKeyOK, UINT8);
    return i4Ret;
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
int ice_ioctl_hdmi(struct inode *inode, struct file *file, unsigned int cmd,
              unsigned long arg)
{
    int i4Ret = MTR_NOT_OK;
    switch (cmd)
    {
    	case ICE_IO_HDMI_DEBUG_LEVEL:
			i4Ret = _ICE_HDMIDebugLevel(arg);
			break;
        case ICE_IO_HDMI_INIT:
            i4Ret = _ICE_HDMIInit();  
            break;
		case ICE_IO_HDMI_SET_RESOLUTION:
			i4Ret = _ICE_HDMISetResolution(arg); 
			break;
		case ICE_IO_HDMI_SET_OUTPUT_COLOUR_SPACE:
			i4Ret = _ICE_HDMISetOutputColourSpace(arg);
			break;
		case ICE_IO_HDMI_GET_CURRENT_OUTPUT_COLOUR_SPACE:
			i4Ret = _ICE_HDMIGetCurrentOutputColourSpace(arg);
			break;
		case ICE_IO_HDMI_SET_ASPECT_RATIO:
			i4Ret = _ICE_HDMISetAspectRatio(arg);
			break;
		case ICE_IO_HDMI_GET_CURRENT_HDMI_VIDEO_OUTPUT_RESOLUTION:
			i4Ret = _ICE_HDMIGetCurrentHDMIVideoOutputResolution(arg);
			break;
		case ICE_IO_HDMI_SET_CURRENT_HDMI_VIDEO_OUTPUT_RESOLUTION:
			i4Ret = _ICE_HDMISetCurrentHDMIVideoOutputResolution(arg);
			break;
		case ICE_IO_HDMI_GET_CURRENT_ASPECT_RATIO:
			i4Ret = _ICE_HDMIGetCurrentAspectRatio(arg);
			break;
		case ICE_IO_HDMI_SET_CURRENT_ASPECT_RATIO:
			i4Ret = _ICE_HDMISetCurrentAspectRatio(arg);
			break;
		case ICE_IO_HDMI_SET_TV_ASPECT_RATIO:
			i4Ret = _ICE_HDMISetTVAspectRatio(arg);
			break;
		case ICE_IO_HDMI_HARDWARE_RESET_TRANSMITTER:
			i4Ret = _ICE_HDMIHardwareResetTransmitter(arg);
			break;
		case ICE_IO_HDMI_HARDWARE_RESET_SCALER:
			i4Ret = _ICE_HDMIHardwareResetScaler(arg);
			break;
		case ICE_IO_HDMI_HARDWARE_RESET_SCALER_AND_TRANSMITTER:
			i4Ret = _ICE_HDMIHardwareResetScalerAndTransmitter(arg);
			break;
		case ICE_IO_HDMI_MUTE:
			i4Ret = _ICE_HDMIMute(arg);
			break;
		case ICE_IO_HDMI_UNMUTE:
			i4Ret = _ICE_HDMIUnmute(arg);
			break;
		case ICE_IO_HDMI_GET_OUTPUT_AVAILABILITY:
			i4Ret = _ICE_HDMIGetOutputAvailability(arg);
			break;
		case ICE_IO_HDMI_IS_SIMPLAY_DEFINED:
			i4Ret = _ICE_HDMIIsSimplayDefined(arg);
			break;
		case ICE_IO_HDMI_HDCP_ENABLE_DISABLE:
			i4Ret = _ICE_HDMIHDCPEnableDisable(arg);
			break;
		case ICE_IO_HDMI_HDCP_RECEIVE_SRM:
			i4Ret = _ICE_HDMIHDCPReceiveSRM(arg);
			break;
		case ICE_IO_HDMI_AUDIO_DELAY:
			i4Ret = _ICE_HDMIAudioDelay(arg);
			break;
            case ICE_IO_HDMI_REG_CB_FUNC:
                    i4Ret = _ICE_HDMI_Reg_Cb_func(arg);
                    break;
            case ICE_IO_HDMI_GET_CURRENT_AFD:
                  i4Ret = _ICE_HDMIGetCurrentAFD(arg);
                    break;
            case ICE_IO_HDMI_PRINT:
                    _ice_HDMIPrint(arg);
                    break;
            case ICE_IO_HDMI_LOAD_HDCP_KEY:
                    i4Ret = _ice_HDMILoadHDCPKey(arg);
                    break;
            case ICE_IO_HDMI_IS_HDCPKEY_OK:
            i4Ret = _ICE_HDMIIsHDCPKeyOK(arg);
            break;
        default:
            break;
    }
    return i4Ret;
}


