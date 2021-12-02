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
* The operations of USB Audio driver
*
* Author:
* -------
*
* Last changed:
* -------------
*
* $Modtime: $
*
* $Revision: #2 $
****************************************************************************/


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#ifdef CC_AUD_SUPPORT_OSS_ADAPTION_LAYER
#include "x_util.h"
#include "x_typedef.h"
#include "x_os.h"
#include "x_bim.h"
#include "x_printf.h"
#include "x_assert.h"
#include "aud_drvif.h"
#include "x_ckgen.h"
#include "x_pinmux.h"
//#include "x_pinmux.h"

#include "aud_debug.h"
#include "psr_drvif.h"
#include "drv_common.h"
//#include "drv_dsp_cfg.h"
#include "drv_adsp.h"
//#include "drv_stc.h"
#include "stc_drvif.h"
//#include "drv_slt.h"
#include "mpv_drvif.h"
#include "drvcust_if.h"
#include "drv_dbase.h"
#include "mute_if.h"

#include "aud_drv.h"
#include "aud_if.h"
#include "spdif_drvif.h"
#include "aud_hw.h"
#include "aud_cfg.h"
#include "dsp_intf.h"
#include "aud_detect.h"
#include "aud_notify.h"
#include "adac_if.h"
#include "aud_pll.h"
#include "aud_dsp_cfg.h"

#include "oss_pcm.h"
#include "oss_adap.h"

INT32 i4Usb_Open(void)
{
    if (0 != i4OssCreateInstance(1, WRITE_ONLY))
    {
        printf("[_USB_Open]Error: Turn ON USB Audio module failed\n");
        return -1;
    }

	printf("[_USB_Open] Turn ON USB Audio module successed\n");
	return 0;
}

INT32 i4Usb_Close(void)
{
	i4OssDestroyInstance();
	return 0;
}

INT32 i4Usb_Create(UINT32 samplerate, UINT32 channel, UINT32 format)
{
	INT32 i4Result = 0;
	// set sample format: 16 bit, little endian
    i4Result = i4OssSetFormat(format);
	if(i4Result != 0)
	{
		printf("[_USB_Set] Error. i4OssSetFormat() failed.\n");
		return i4Result;
	}

    // it is required to set channel before setting sampling rate
    i4Result = i4OssSetChannel(channel);	
	if(i4Result != 0)
	{
		printf("[_USB_Set] Error. i4OssSetChannel() failed.\n");
		return i4Result;
	}
	
    i4Result = i4OssSetSpeed(samplerate);	
	if(i4Result != 0)
	{
		printf("[_USB_Set] Error. i4OssSetSpeed() failed.\n");
		return i4Result;
	}

	return i4Result;
}
INT32 i4Usb_Free(void)
{
	return 0;
}

INT32 i4Usb_Write(UINT32 u4Addr, UINT32 u4Size)
{
	//printf("i4Usb_Write() u4Addr = 0x%x u4Size = %d\n", u4Addr, u4Size);
	
	INT32  ret = 0;
	UINT32 u4Size2 = 0;
	
    while(u4Size2 < u4Size) 
    {
        if ((ret = i4OssWrite((UCHAR*)(u4Addr + u4Size2), u4Size - u4Size2)) == -1) 
        {
            printf("audio write fail");
            return -1;
        }
        u4Size2 += ret;
    }

	//printf(".");
    return u4Size2;
}

INT32 i4Usb_Read(void)
{
	return 0;
}

static HANDLE_T _hAudOssAdapCmdQueue = (HANDLE_T)NULL;
static HANDLE_T _hAudOssAdapThread = (HANDLE_T)NULL;
//AUD_OSS_ADAP_ST_TYPE_T _eAudOssAdapTaskStatus;
AUD_OSS_ADAP_PROPERTY_T g_AudOssAdap_Property;

UINT32 g_u4DevCount = DEVICE_OUT_MAX;
UINT32 g_p4DevIdIndex[] = {DEVICE_OUT_USB_HEADPHONE, DEVICE_OUT_BLUETOOTH_EARPHONE};
UINT32 g_p4FuncIndex[] = {0, 1};

AUD_OSS_ADAP_FUNC_T g_prFuncTbl[] = 
{
	{
		i4Usb_Open, 
		i4Usb_Close,
		i4Usb_Write,
		i4Usb_Read,
		i4Usb_Create,
		i4Usb_Free
	},
	{
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL
	}
	
};

INT32 _AudOssAdap_CheckMsg(AUD_OSS_ADAP_MSG_T tMsg)
{
	INT32 i4Ret = OSR_OK;
	AUD_OSS_ADAP_ST_TYPE_T status;
	AUD_OSS_ADAP_DEV_T devId;

	if((tMsg.devId >= DEVICE_OUT_MAX) || (tMsg.cmd >= AUD_OSS_ADAP_INVALID))
	{
		Printf("[Error] Line: %d tMsg.devId: %d tMsg.cmd: %d\n", __LINE__, tMsg.devId, tMsg.cmd);
		return OSR_INVALID;	
	}

	status = _AudOssAdap_GetStatus();
	devId = _AudOssAdap_GetDevId();

	if((AUD_OSS_ADAP_ST_IDLE != status) && (devId != tMsg.devId))
	{
		Printf("[Error] Line: %d tMsg.devId: %d status: %d\n", __LINE__, tMsg.devId, status);
		return OSR_INVALID;
	}

	switch(status)
	{
		case AUD_OSS_ADAP_ST_IDLE:
			if(tMsg.cmd != AUD_OSS_ADAP_CONNECT)
			{
				i4Ret = OSR_INVALID;
			}
			break;
		case AUD_OSS_ADAP_ST_ENABLED:
			if((tMsg.cmd != AUD_OSS_ADAP_START) && (tMsg.cmd != AUD_OSS_ADAP_DISCONNECT))
			{
				i4Ret = OSR_INVALID;
			}
			break;
		case AUD_OSS_ADAP_ST_WORKING:
			if((tMsg.cmd != AUD_OSS_ADAP_TRANSFER) && (tMsg.cmd != AUD_OSS_ADAP_STOP))
			{
				i4Ret = OSR_INVALID;
			}
			break;
		default:
			Printf("[Error] Line: %d status: %d\n", __LINE__, status);
			i4Ret = OSR_INVALID;
			break;
	}

	return i4Ret;
}

#define OSS_ADAP_MSG_QUEUE_TIME_OUT 200 

INT32 _AudOssAdap_SendMsg(AUD_OSS_ADAP_MSG_T tMsg)
{
	INT32 i4Ret = OSR_OK;
	INT32 i4MsgOverFlowCnt = 0;
	
	i4Ret = _AudOssAdap_CheckMsg(tMsg);
	if(OSR_OK != i4Ret)
	{
		Printf("[Error] Line: %d SendMsg invalid\n", __LINE__);
		return OSR_FAIL;
	}

    do 
    {
        i4Ret = x_msg_q_send(_hAudOssAdapCmdQueue, &tMsg, sizeof(AUD_OSS_ADAP_MSG_T), 1);
        VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_TOO_MANY));
        if (i4Ret == OSR_TOO_MANY)
        {
            x_thread_delay(10);
            i4MsgOverFlowCnt ++;
            if (i4MsgOverFlowCnt >= OSS_ADAP_MSG_QUEUE_TIME_OUT)
            {
                Printf("OSS adaptation queue message queue over flow\n");
                ASSERT(0);
            }
        }
    } while(i4Ret != OSR_OK);

	return i4Ret;
}

INT32 _AudOssAdap_RecvMsg(AUD_OSS_ADAP_MSG_T *ptMsg)
{
	INT32 i4Ret = OSR_OK;
	UINT16 u2MsgIdx;
	AUD_OSS_ADAP_MSG_T tMsg; 
	SIZE_T zMsgSize = sizeof(AUD_OSS_ADAP_MSG_T);
	
	VERIFY(x_msg_q_receive(&u2MsgIdx, &tMsg, &zMsgSize,
        &_hAudOssAdapCmdQueue, 1, X_MSGQ_OPTION_WAIT) == OSR_OK);
	
	i4Ret = _AudOssAdap_CheckMsg(tMsg);

	if(OSR_OK == i4Ret)
	{
		ptMsg->cmd = tMsg.cmd;
		ptMsg->devId = tMsg.devId;
		ptMsg->dataAddr = tMsg.dataAddr;
		ptMsg->dataSize = tMsg.dataSize;
	}
	else
	{
		Printf("[Error] Line: %d RecvMsg invalid\n", __LINE__);
	}

	return i4Ret;
}

void _AudOssAdap_IdleRoutine(void)
{
	AUD_OSS_ADAP_MSG_T tMsg; 
	UINT32 u4Index = 0;

	VERIFY(_AudOssAdap_RecvMsg(&tMsg) == OSR_OK);

	switch(tMsg.cmd)
	{
		case AUD_OSS_ADAP_CONNECT:
			for(u4Index = 0; u4Index < g_u4DevCount; u4Index++)
			{
				if(tMsg.devId == g_p4DevIdIndex[u4Index])
				{
					g_AudOssAdap_Property.ops = &g_prFuncTbl[g_p4FuncIndex[u4Index]];
					break;
				}
			}
			if(g_u4DevCount == u4Index)
			{
				printf("[Error] Line: %d tMsg.devId = %d  tMsg.cmd = %d\n", __LINE__, tMsg.devId, tMsg.cmd);
				break;
			}

			g_AudOssAdap_Property.ops->open();
			
			g_AudOssAdap_Property.devId = tMsg.devId;
    		g_AudOssAdap_Property.status = AUD_OSS_ADAP_ST_ENABLED;

			printf("g_AudOssAdap_Property.i4Status = %d\n", g_AudOssAdap_Property.status);
			break;

		default:
			printf("[Error] Line: %d tMsg.devId = %d  tMsg.cmd = %d\n", __LINE__, tMsg.devId, tMsg.cmd);
			break;
	}
}

void _AudOssAdap_EnabledRoutine(void)
{
	AUD_OSS_ADAP_MSG_T tMsg; 

	VERIFY(_AudOssAdap_RecvMsg(&tMsg) == OSR_OK);

	switch(tMsg.cmd)
	{
		case AUD_OSS_ADAP_START:
			if(g_AudOssAdap_Property.srcType == AUD_OSS_ADAP_UPLOAD)
			{
				g_AudOssAdap_Property.ops->create(48000, 2, 32);
			}
			else
			{
				g_AudOssAdap_Property.ops->create(48000, 2, 16);
			}
			
			g_AudOssAdap_Property.status = AUD_OSS_ADAP_ST_WORKING;
			break;
		case AUD_OSS_ADAP_DISCONNECT:
			g_AudOssAdap_Property.ops->close();
			g_AudOssAdap_Property.ops = NULL;
			g_AudOssAdap_Property.devId = DEVICE_OUT_MAX;
    		g_AudOssAdap_Property.status = AUD_OSS_ADAP_ST_IDLE;
			break;
		default:
			printf("[Error] Line: %d tMsg.devId = %d  tMsg.cmd = %d\n", __LINE__, tMsg.devId, tMsg.cmd);
			break;
	}
}


void _AudOssAdap_WorkingRoutine(void)
{
	AUD_OSS_ADAP_MSG_T tMsg; 

	UINT32 u4tempStart;
	UINT32 u4tempEnd;

	u4tempStart = u4GetAFIFOStart(1);
	u4tempEnd = u4GetAFIFOEnd(1);

	VERIFY(_AudOssAdap_RecvMsg(&tMsg) == OSR_OK);

	switch(tMsg.cmd)
	{
		case AUD_OSS_ADAP_STOP:
			g_AudOssAdap_Property.ops->free();
			g_AudOssAdap_Property.status = AUD_OSS_ADAP_ST_ENABLED;
			break;
		case AUD_OSS_ADAP_TRANSFER:
			//_AUD_UploadLockSem2();
			g_AudOssAdap_Property.ops->write(tMsg.dataAddr, tMsg.dataSize);
			g_AudOssAdap_Property.status = AUD_OSS_ADAP_ST_WORKING;

			#if 0
			if((u4tempPtr + u4tempStart + tMsg.dataSize) > u4tempEnd)
			{
				u4tempFlag = 1;
			}
			
			if(u4tempFlag == 0)
			{	
				x_memcpy((VOID*)(VIRTUAL(u4tempPtr + u4tempStart)), (VOID*)(tMsg.dataAddr), tMsg.dataSize);
				u4tempPtr += tMsg.dataSize;
			}
			#endif

			//_AUD_UploadUnlockSem2();
						
			break;
		default:
			printf("[Error] Line: %d tMsg.devId = %d  tMsg.cmd = %d\n", __LINE__, tMsg.devId, tMsg.cmd);
			break;
	}
}

static void _AudOssAdap_Thread(void* pvArg)
{
	#if 1
	AUD_OSS_ADAP_MSG_T tMsg; 
	UINT32 u4Index = 0;

	UINT32 u4tempStart;
	UINT32 u4tempEnd;

	u4tempStart = u4GetAFIFOStart(1);
	u4tempEnd = u4GetAFIFOEnd(1);

	while(TRUE)
	{
		VERIFY(_AudOssAdap_RecvMsg(&tMsg) == OSR_OK);
		switch(tMsg.cmd)
		{
			case AUD_OSS_ADAP_CONNECT:
				for(u4Index = 0; u4Index < g_u4DevCount; u4Index++)
				{
					if(tMsg.devId == g_p4DevIdIndex[u4Index])
					{
						g_AudOssAdap_Property.ops = &g_prFuncTbl[g_p4FuncIndex[u4Index]];
						break;
					}
				}
				if(g_u4DevCount == u4Index)
				{
					printf("[Error] Line: %d tMsg.devId = %d  tMsg.cmd = %d\n", __LINE__, tMsg.devId, tMsg.cmd);
					break;
				}

				g_AudOssAdap_Property.ops->open();
				
				g_AudOssAdap_Property.devId = tMsg.devId;
	    		g_AudOssAdap_Property.status = AUD_OSS_ADAP_ST_ENABLED;

				printf("g_AudOssAdap_Property.i4Status = %d\n", g_AudOssAdap_Property.status);
			
				break;
			case AUD_OSS_ADAP_START:
				if(g_AudOssAdap_Property.srcType == AUD_OSS_ADAP_UPLOAD)
				{
					g_AudOssAdap_Property.ops->create(48000, 2, 32);
				}
				else
				{
					g_AudOssAdap_Property.ops->create(48000, 2, 16);
				}
				
				g_AudOssAdap_Property.status = AUD_OSS_ADAP_ST_WORKING;
				break;
			case AUD_OSS_ADAP_TRANSFER:
				
				//_AUD_UploadLockSem2();
				g_AudOssAdap_Property.ops->write(tMsg.dataAddr, tMsg.dataSize);
				g_AudOssAdap_Property.status = AUD_OSS_ADAP_ST_WORKING;
				
				#if 0
				if((u4tempPtr + u4tempStart + tMsg.dataSize) > u4tempEnd)
				{
					u4tempFlag = 1;
				}
				
				if(u4tempFlag == 0)
				{	
					x_memcpy((VOID*)(VIRTUAL(u4tempPtr + u4tempStart)), (VOID*)(tMsg.dataAddr), tMsg.dataSize);
					u4tempPtr += tMsg.dataSize;
				}
				#endif

				break;
			case AUD_OSS_ADAP_STOP:
				g_AudOssAdap_Property.ops->free();
				g_AudOssAdap_Property.status = AUD_OSS_ADAP_ST_ENABLED;
				break;
			case AUD_OSS_ADAP_DISCONNECT:
				g_AudOssAdap_Property.ops->close();
				g_AudOssAdap_Property.ops = NULL;
				g_AudOssAdap_Property.devId = DEVICE_OUT_MAX;
	    		g_AudOssAdap_Property.status = AUD_OSS_ADAP_ST_IDLE;
				break;
			default:
				break;
		}
	}
		
	#else
    while (1)
    {
        switch (g_AudOssAdap_Property.status)
        {
            case AUD_OSS_ADAP_ST_IDLE:
                _AudOssAdap_IdleRoutine();
		        break;
			
            case AUD_OSS_ADAP_ST_ENABLED:
                _AudOssAdap_EnabledRoutine();
                break;
			
			case AUD_OSS_ADAP_ST_WORKING:
				_AudOssAdap_WorkingRoutine();
				break;
            default:
                printf("[_AudOssAdapThread] OSS adap audio status wrong, thread is terminated!\n");
                //g_AudOssAdap_Property.status = AUD_OSS_ADAP_ST_IDLE;
                return;
        }
    }
	#endif
}


/******************************************************************************
 *Function    : i4OssWrite()
 *Purpose     : Playback the data streaming
 *Returns      :0 means succeuss
                      :-1 means failed
 *****************************************************************************/

void _AudOssAdap_Init(void)
{
	Printf("_AudOssAdap_Init() 1\n");
	g_AudOssAdap_Property.devId = DEVICE_OUT_MAX;
	g_AudOssAdap_Property.status = AUD_OSS_ADAP_ST_IDLE;
	g_AudOssAdap_Property.srcType = AUD_OSS_ADAP_OTHER;
	g_AudOssAdap_Property.ops = NULL;

	if((HANDLE_T)NULL == _hAudOssAdapCmdQueue)
	{
		VERIFY((x_msg_q_create(&_hAudOssAdapCmdQueue, ADRV_OSS_ADAP_CMD_Q, sizeof(AUD_OSS_ADAP_MSG_T), 32)) == OSR_OK);
	}
	if((HANDLE_T)NULL == _hAudOssAdapThread)
	{
		VERIFY(x_thread_create(&_hAudOssAdapThread, AUD_OSS_ADAP_THREAD, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
	            _AudOssAdap_Thread, 0, NULL) == OSR_OK);
	}
	Printf("_AudOssAdap_Init() _hAudOssAdapCmdQueue = %d  _hAudOssAdapThread = %d\n", _hAudOssAdapCmdQueue, _hAudOssAdapThread);
}

void _AudOssAdap_GetProperty(AUD_OSS_ADAP_PROPERTY_T * property)

{
	property->devId = g_AudOssAdap_Property.devId;
	property->status = g_AudOssAdap_Property.status;
	property->ops = g_AudOssAdap_Property.ops;
}

AUD_OSS_ADAP_ST_TYPE_T _AudOssAdap_GetStatus(void)
{
	return g_AudOssAdap_Property.status;
}
AUD_OSS_ADAP_DEV_T _AudOssAdap_GetDevId(void)
{
	return g_AudOssAdap_Property.devId;
}

INT32 _AudOssAdap_Enable(AUD_OSS_ADAP_DEV_T devId)
{
	INT32 i4Ret = OSR_OK;
	AUD_OSS_ADAP_DEV_T devIdtmp;
	AUD_OSS_ADAP_MSG_T tMsg;
	AUD_OSS_ADAP_ST_TYPE_T status;

	devIdtmp = _AudOssAdap_GetDevId();

	if(devIdtmp == devId)
	{
		Printf("_AudOssAdap_Enable() same. devIdtmp = %d  devId = %d\n", devIdtmp, devId);
		return i4Ret;
	}

	_AudOssAdap_Disable(devIdtmp);
	
	if(DEVICE_OUT_MAX != devId)
	{
		ADAC_SpeakerEnable(FALSE);
		
		tMsg.devId = devId;
		tMsg.cmd = AUD_OSS_ADAP_CONNECT;
		i4Ret = _AudOssAdap_SendMsg(tMsg);
		VERIFY(i4Ret == OSR_OK);

		do{
			x_thread_delay(10);
			status = _AudOssAdap_GetStatus();
		}while(status != AUD_OSS_ADAP_ST_ENABLED);
		
		tMsg.devId = devId;
		tMsg.cmd = AUD_OSS_ADAP_START;
		i4Ret = _AudOssAdap_SendMsg(tMsg);
		VERIFY(i4Ret == OSR_OK);

		do{
			x_thread_delay(10);
			status = _AudOssAdap_GetStatus();
		}while(status != AUD_OSS_ADAP_ST_WORKING);
	}
	else
	{
		ADAC_SpeakerEnable(TRUE);
	}
	
	return i4Ret;
}

INT32 _AudOssAdap_Disable(AUD_OSS_ADAP_DEV_T devId)
{
#if 1
	INT32 i4Ret = OSR_OK;
	AUD_OSS_ADAP_ST_TYPE_T status;

	status = _AudOssAdap_GetStatus();
	if(AUD_OSS_ADAP_ST_WORKING == status)
	{
		g_AudOssAdap_Property.ops->free();
		g_AudOssAdap_Property.ops->close();
	}
	else if(AUD_OSS_ADAP_ST_ENABLED == status)
	{
		g_AudOssAdap_Property.ops->close();
	}

	g_AudOssAdap_Property.status = AUD_OSS_ADAP_ST_IDLE;
	g_AudOssAdap_Property.ops = NULL;
	g_AudOssAdap_Property.devId = DEVICE_OUT_MAX;

#else
	tMsg.devId = devId;
	tMsg.cmd = AUD_OSS_ADAP_STOP;
	i4Ret = _AudOssAdap_SendMsg(tMsg);
	VERIFY(i4Ret == OSR_OK);

	do{
		x_thread_delay(10);
		status = _AudOssAdap_GetStatus();
	}while(status != AUD_OSS_ADAP_ST_ENABLED);
	
	tMsg.devId = devId;
	tMsg.cmd = AUD_OSS_ADAP_DISCONNECT;
	i4Ret = _AudOssAdap_SendMsg(tMsg);
	VERIFY(i4Ret == OSR_OK);

	do{
		x_thread_delay(10);
		status = _AudOssAdap_GetStatus();
	}while(status != AUD_OSS_ADAP_ST_IDLE);
#endif
	return i4Ret;
}

void _AudOssAdap_SetSourceType(AUD_OSS_ADAP_SRC_TYPE_T srcType)
{
	if(g_AudOssAdap_Property.srcType == srcType)
	{
		return;
	}
	
	g_AudOssAdap_Property.srcType = srcType;
}

AUD_OSS_ADAP_SRC_TYPE_T _AudOssAdap_GetSourceType()
{
	return g_AudOssAdap_Property.srcType;
}


#endif

