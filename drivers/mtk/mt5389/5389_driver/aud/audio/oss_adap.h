/********************************************************************************************
 *     LEGAL DISCLAIMER 
 *
 *     (Header of MediaTek Software/Firmware Release or Documentation)
 *
 *     BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES 
 *     THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED 
 *     FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS 
 *     ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED, 
 *     INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR 
 *     A PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY 
 *     WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, 
 *     INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK 
 *     ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *     NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION 
 *     OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
 *     
 *     BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE LIABILITY WITH 
 *     RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, 
 *     TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE 
 *     FEES OR SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
 *     
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS 
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.  
 ************************************************************************************************/
/*****************************************************************************
*
* Filename:
* ---------
*   $Workfile:$
*
* Project:
* --------
* MT8530
*
* Description:
* ------------
* The prototypes of API for ipod USB Client driver
*
* Author:
* -------
* Hui Zhang
*
* Last changed:
* -------------
* $Author: kai.wang $
*
* $Modtime: $
*
* $Revision: #2 $
****************************************************************************/

#ifndef _PCM_ADAP_H_
#define _PCM_ADAP_H_


#ifdef CC_AUD_SUPPORT_OSS_ADAPTION_LAYER

#define ADRV_OSS_ADAP_CMD_Q         "ADRV_OSS_ADAP_CMD_Q"
#define AUD_OSS_ADAP_THREAD         "AudOssAdapThread"

typedef enum
{
    AUD_OSS_ADAP_ST_IDLE = 0,
	AUD_OSS_ADAP_ST_ENABLED,
	AUD_OSS_ADAP_ST_WORKING,
}AUD_OSS_ADAP_ST_TYPE_T;

typedef enum
{
    AUD_OSS_ADAP_CONNECT = 0,
    AUD_OSS_ADAP_START,
    AUD_OSS_ADAP_TRANSFER,
    AUD_OSS_ADAP_STOP,
    AUD_OSS_ADAP_DISCONNECT,

	AUD_OSS_ADAP_INVALID
}AUD_OSS_ADAP_CMD_T;

typedef enum
{
	DEVICE_OUT_USB_HEADPHONE = 0,
	DEVICE_OUT_BLUETOOTH_EARPHONE,
	DEVICE_OUT_MAX
}AUD_OSS_ADAP_DEV_T;

typedef enum
{
	AUD_OSS_ADAP_MIXSOUND = 0,
	AUD_OSS_ADAP_UPLOAD,
	AUD_OSS_ADAP_OTHER
}AUD_OSS_ADAP_SRC_TYPE_T;


typedef struct
{
	INT32 (*open)(void);
	INT32 (*close)(void);
	INT32 (*write)(UINT32 u4Addr, UINT32 u4Size);
	INT32 (*read)(void);
	INT32 (*create)(UINT32 samplerate, UINT32 channel, UINT32 format);
	INT32 (*free)(void);
}AUD_OSS_ADAP_FUNC_T;

typedef struct 
{
	AUD_OSS_ADAP_DEV_T devId;
	AUD_OSS_ADAP_ST_TYPE_T status;
	AUD_OSS_ADAP_SRC_TYPE_T srcType;
	AUD_OSS_ADAP_FUNC_T *ops;
}AUD_OSS_ADAP_PROPERTY_T;

typedef struct
{
	AUD_OSS_ADAP_DEV_T devId;
	AUD_OSS_ADAP_CMD_T cmd;
	UINT32 dataAddr;
	UINT32 dataSize;
}AUD_OSS_ADAP_MSG_T;

INT32 i4Usb_Open(void);
INT32 i4Usb_Close(void);
INT32 i4Usb_Write(UINT32 u4Addr, UINT32 u4Size);
INT32 i4Usb_Read(void);
INT32 i4Usb_Create(UINT32 samplerate, UINT32 channel, UINT32 format);
INT32 i4Usb_Free(void);

void _AudOssAdap_Init(void);
INT32 _AudOssAdap_SendMsg(AUD_OSS_ADAP_MSG_T tMsg);
void _AudOssAdap_GetProperty(AUD_OSS_ADAP_PROPERTY_T * property);
AUD_OSS_ADAP_ST_TYPE_T _AudOssAdap_GetStatus(void);
AUD_OSS_ADAP_DEV_T _AudOssAdap_GetDevId(void);
INT32 _AudOssAdap_Enable(AUD_OSS_ADAP_DEV_T devId);
INT32 _AudOssAdap_Disable(AUD_OSS_ADAP_DEV_T devId);
void _AudOssAdap_SetSourceType(AUD_OSS_ADAP_SRC_TYPE_T srcType);
AUD_OSS_ADAP_SRC_TYPE_T _AudOssAdap_GetSourceType(void);


#endif


#endif //_PCM_OSS_H_
