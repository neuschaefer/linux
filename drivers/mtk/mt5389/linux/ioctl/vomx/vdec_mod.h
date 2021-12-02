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
/*
*
* $Author: dtvbm11 $
* $Modtime: $
* $Revision: #1 $
*
*/

// *********************************************************************
// Memo
// *********************************************************************
/*
*/

#ifndef _VDEC_MOD_H_
#define _VDEC_MOD_H_

#include <x_rm.h>

#include "openmaxil_ioctl.h"
//fix89 #include "drv_vdec.h"
#include "drv_common.h"
#include "omx_vdec_vconv.h"

#define VDecOMX_OK                0
#define VDecOMX_ERR            (-1)
#define VDO_OMX_INV_PTS         ((UINT64)(-1))

#define VDO_COMP_MAX  2
#define VDO_INPUT_BUF_MAX  20
#define VDO_OUTPUT_BUF_MAX 20

typedef enum {
    VOMX_STOPPING_NONE,
    VOMX_STOPPING_VCONV,
    //VOMX_STOPPING_VDEC,
    VOMX_STOPPING_VPUSH,
    VOMX_STOPPING_MAX,
} VOMX_STOPPING_STATUS_T;

typedef enum {
    VOMX_FLUSHING_NONE,
    VOMX_FLUSHING_VCONV,
    VOMX_FLUSHING_VPUSH,
    VOMX_FLUSHING_MAX,
} VOMX_FLUSHING_STATUS_T;

typedef struct _VDO_OMX_BUFFER_T VDO_OMX_BUFFER_T;

typedef struct 
	{
    HANDLE_T       h_handle;
    BOOL              fgUsed;
    BOOL              fgEmptying;
    BOOL              fgFilling;
    BOOL              fgStopVConv;
    BOOL              fgStoppingVPush;
    BOOL              fgStoppingVConv;
    BOOL              fgStateTransing;
//    BOOL              fgInPortEnabled;
//    BOOL              fgOutPortEnabled;
//    BOOL              fgInPortTransing;
//    BOOL              fgOutPortTransing;
    BOOL              fgGotEOS;
    BOOL              fgSentEOS;
    UINT64            u8TimeStamp;
    UINT64			  u8TimeStampBase;
    UINT32            pid;
    HANDLE_T          hMainTask;
    HANDLE_T          hCommandQ;
    UINT32            u4EmptySize;
    UINT32            u4EmptyCounter;
    UINT32            u4EmptyWritePos;
    UINT32            u4EmptyReadPos;
    UINT32            u4FillCounter;
    UINT32            u4FillWritePos;
    UINT32            u4FillReadPos;
    OMX_TICKS         nOutputDelay;
    VDO_OMX_BUFFER_T *pEmptyQ[VDO_INPUT_BUF_MAX]; // always left aligned, -> 11OO0000
    VDO_OMX_BUFFER_T *pFillQ[VDO_OUTPUT_BUF_MAX];
    OMX_COLOR_FORMATTYPE eColorFormatIndex0;
    VCodeC     eCodecType;
    OMX_STATETYPE     eState;
    OMX_STATETYPE     eStateTo;
    VOMX_STOPPING_STATUS_T   eStopStatus;
    VOMX_FLUSHING_STATUS_T   eFlushStatus;
    OMX_PTR           pAppData;
    MTVDEC_PFN_OMX_IL_CB  pEventHandler;
    MTVDEC_PFN_OMX_IL_CB  pEmptyBufferDone;
    MTVDEC_PFN_OMX_IL_CB  pFillBufferDone;
    void             *pFifoPntr;  // VFIFO SA for decoder
    UINT32            u4FifoSize; // VFIFO length for decoder
    void             *pFifoWptr;  // VFIFO Wptr for decoder
    BOOL		  fgFlush[2]; // input 0, output 1
    VOID       	*pVdecPush;
    OMX_BUFFERHEADERTYPE rCbHeader; //for jpeg thumbnail callback
} VDO_OMX_HANDLE_T;

struct _VDO_OMX_BUFFER_T {
    VDO_OMX_HANDLE_T    *pOwner;
    HANDLE_T             hBuffer; // handle for below buffer
    void                *pBufSA;
    UINT32               u4BufSize; // althrough buffer size is included in below header, keep this for error detection
    HANDLE_T             hHeader; // handle for below structure
    UINT32               u4FilledLen; // used on empty, keep this althrough duplicated
    UINT32               u4Offset; // used on empty, keep this althrough duplicated
    OMX_BUFFERHEADERTYPE rHeader; // client will read/write this area
    OMX_TICKS            nTimeStamp;
    UINT32               u4Reserved; // for error detection
};

typedef enum {
    VDO_OMX_CMD_SET_STATE,
    VDO_OMX_CMD_EMPTY_BUFFER,
    VDO_OMX_CMD_FILL_BUFFER,
    VDO_OMX_CMD_FLUSH,
    VDO_OMX_CMD_PORT_ENABLE,
    VDO_OMX_CMD_PORT_DISABLE,
    VDO_OMX_CMD_BUFFER_ALLOCATED,
    VDO_OMX_CMD_BUFFER_FREED,
    VDO_OMX_CMD_TERMINATE,
    VDO_OMX_CMD_MAX = 0x7FFFFFFF,
} VDO_OMX_CMD_T;

typedef struct {
    VDO_OMX_CMD_T     eCmd;
    OMX_STATETYPE     eStateTo; // for VDO_OMX_CMD_SET_STATE
    HANDLE_T          hHeader;  // for VDO_OMX_CMD_EMPTY_BUFFER & VDO_OMX_CMD_FILL_BUFFER
    BOOL              fgInput;  // for VDO_OMX_CMD_FLUSH
    BOOL              fgOutput; // for VDO_OMX_CMD_FLUSH
} VDO_OMX_MSG_T;

typedef struct
{
    OMX_COMMAND	rOMXVDECCMD;
//    OMX_COMMAND_SEND rOMXVDECCMDSent;
    OMX_STATETYPE ePrevStateType;
} OMX_CMD_VDEC_T;

typedef struct
{
    UINT32 nPorts;             /**< The number of ports for this component */
    UINT32 nStartPortNumber;   /** first port number for this type of port */
}VDEC_PORT_PARAM_T;

typedef struct
{
    UINT32 nBufferCountActual;    /**< The actual number of buffers allocated on this port */
    UINT32 nBufferCountMin;       /**< The minimum number of buffers this port requires */
    UINT32 nBufferSize;           /**< Size, in bytes, for buffers to be used for this channel */
    UINT32 nFrameWidth;
    UINT32 nFrameHeight;
    UINT32 nStride;
}VDEC_PORT_DEFINITION_T;

typedef enum
{
OMX_EVENT_HANDLER=0,
OMX_EMPTY_BUFF_DONE,
OMX_FILL_BUFF_DONE,	
}OMX_VDEC_CB_COND_T;

typedef struct _OMX_VDEC_CB_DATA
{
    OMX_VDEC_CB_COND_T      e_nfy_cond;
    UINT32              ui4_data_1;
    UINT32              ui4_data_2;
}OMX_VDEC_CB_DATA;

typedef struct
{
OMX_CMD_VDEC_T rOMXVDECCMDALL;
VDEC_PORT_PARAM_T rPortParamType;
VDEC_PORT_DEFINITION_T rPortDefinitionPara;
}VDEC_OMX_INFO_T;



/* data passed with VDEC_NFY_FILL_BUF */
typedef struct
{
    UINT32          u4DataAdd;        
    UINT32          u4DataLen;   
    BOOL            fgFileEnd;
} VDEC_FILL_BUF_T;

extern VDO_OMX_HANDLE_T _prVDOOmxHandle[2]; // currently only support single instance

INT32 i4VDOOmxTaskSendCommand(VDO_OMX_HANDLE_T *pHandle, VDO_OMX_MSG_T *prMsg);
INT32 i4VDOOmxTaskDestroy(VDO_OMX_HANDLE_T *pHandle);
INT32 i4VDOOmxTaskCreate(VDO_OMX_HANDLE_T *pHandle);
void vVDOOmxBufferInit(void);
void vVDOOmxBufferUninit(void);

extern INT32 i4VDOOmxTaskSemInit(void);
extern INT32 i4VDOOmxTaskSemUninit(void);
extern INT32 i4VDOOmxTaskLockSem(void);
extern INT32 i4VDOOmxTaskUnlockSem(void);

//callback use
extern INT32 i4VDOOmxCompNotifyFillDone(VDO_OMX_HANDLE_T *pHandle, VDO_OMX_BUFFER_T *pBufFilled);
extern INT32 i4VDOOmxCompNotifyEvent(VDO_OMX_HANDLE_T *pHandle, PARAMETER_OMX_CB_EVENTHANDLER *pEventInfo);
extern INT32 i4VDOOmxCompNotifyEmptyDone(VDO_OMX_HANDLE_T *pHandle, VDO_OMX_BUFFER_T *pBufEmptied);

// ioctl use
INT32 i4VDOOmxCompInit(void);
INT32 i4VDOOmxCompUninit(void);

INT32 i4VDOOmxCompGetHandle(PARAMETER_OMX_GETHANDLE *prUArg, PARAMETER_OMX_CB *prOmxCb);
INT32 i4VDOOmxCompFreeHandle(PARAMETER_OMX_FREEHANDLE *prUArg);
INT32 i4VDOOmxCompAllocateBuffer(PARAMETER_OMX_ALLOCATEBUFFER *prUArg);
INT32 i4VDOOmxCompFreeBuffer(PARAMETER_OMX_FREEBUFFER *prUArg);
INT32 i4VDOOmxCompEmptyBuffer(PARAMETER_OMX_EMPTYTHISBUFFER *prUArg);
INT32 i4VDOOmxCompFillBuffer(PARAMETER_OMX_FILLTHISBUFFER *prUArg);
INT32 i4VDOOmxCompGetState(PARAMETER_OMX_GETSTATE *prUArg);
INT32 i4VDOOmxCompGetParamVideoInit(PARAMETER_OMX_PORT_PARAM_TYPE *prUArg);
INT32 i4VDOOmxCompParamPortDef(PARAMETER_OMX_PARAM_PORTDEFINITIONTYPE *prUArg);
INT32 i4VDOOmxCompSetState(VDO_OMX_HANDLE_T *pHandle, OMX_STATETYPE omxStateTo);
INT32 i4VDOOmxCompFlushAsync(PARAMETER_OMX_COMMAND_PORT *prUArg);
INT32 i4VDOOmxCompSetStateAsync(PARAMETER_OMX_COMMAND_STATESET *prUArg);
INT32 i4VDOOmxCompPortDisable(VDO_OMX_HANDLE_T *pHandle, BOOL fgInput, BOOL fgOutput);
INT32 i4VDOOmxCompPortDisableAsync(PARAMETER_OMX_COMMAND_PORT *prUArg);
INT32 i4VDOOmxCompPortEnable(VDO_OMX_HANDLE_T *pHandle, BOOL fgInput, BOOL fgOutput);
INT32 i4VDOOmxCompPortEnableAsync(PARAMETER_OMX_COMMAND_PORT *prUArg);
INT32 i4VDOOmxCompParamVideoProfileLevelQuerySupported(PARAMETER_OMX_VIDEO_PARAM_PROFILELEVELTYPE *prUArg);
INT32 i4VDOOmxCompParamVideoAvc(PARAMETER_OMX_VIDEO_PARAM_AVC *prUArg);
INT32 i4VDOOmxCompParamVideoRv(PARAMETER_OMX_VIDEO_PARAM_RV *prUArg);
INT32 i4VDOOmxCompParamVideoWmv(PARAMETER_OMX_VIDEO_PARAM_WMV *prUArg);
INT32 i4VDOOmxCompParamVideoPortFmt(PARAMETER_OMX_VIDEO_PARAM_PORTFORMATTYPE *prUArg);
INT32 i4VDOOmxCompParamVideoSeamlessFmt(PARAMETER_OMX_VIDEO_PARAM_SEAMLESSFORMATTYPE *prUArg);
INT32 i4VDOOmxCompParamTimeScale(PARAMETER_OMX_TIME_CONFIG_SCALETYPE* prUArg);
INT32 i4VDOOmxCompConfigPictureMode(PARAMETER_OMX_IMAGE_MODE* prUArg);
INT32 i4VDOOmxCompConfigClearImageFrame(PARAMETER_OMX_IMAGE_PROCESSING* prUArg);
INT32 i4VDOOmxCompConfigCommonOutputSize(PARAMETER_OMX_FRAMESIZETYPE* prUArg);
INT32 i4VDOOmxCompConfigCommonRotate(PARAMETER_OMX_CONFIG_ROTATIONTYPE* prUArg);
INT32 i4VDOOmxCompConfigCommonScale(PARAMETER_OMX_CONFIG_SCALEFACTORTYPE* prUArg);
INT32 i4VDOOmxCompConfigCommonOutputPosition(PARAMETER_OMX_CONFIG_POINTTYPE* prUArg);
INT32 i4VDOOmxCompParamExpandedSize(PARAMETER_OMX_FRAMESIZETYPE* prUArg);
INT32 i4VDOOmxCompParamOriginalSize(PARAMETER_OMX_FRAMESIZETYPE* prUArg);
INT32 i4VDOOmxCompParamImageFreeBuffer(PARAMETER_OMX_PARAM_IMAGE_FREEBUFFER* prUArg);
INT32 i4VDOOmxCompParamImageUsrDefineData(PARAMETER_OMX_PARAM_IMAGE_USER_DEFINED* prUArg);


INT32 i4VDOOmxAllocInputBuf(VDO_OMX_HANDLE_T *pHandle, UINT32 u4Size, UINT32 u4Port, VDO_OMX_BUFFER_T **ppBufInfo);
INT32 i4VDOOmxFreeInputBuf(OMX_HANDLETYPE h_Handle, HANDLE_T hHeader);
INT32 i4VDOOmxAllocOutputBuf(VDO_OMX_HANDLE_T *pHandle,
    BOOL fgUseBuf, UINT32 u4BufSA,
    UINT32 u4Size, UINT32 u4Port, VDO_OMX_BUFFER_T **ppBufInfo);
INT32 i4VDOOmxFreeOutputBuf(OMX_HANDLETYPE h_Handle, HANDLE_T hHeader);
INT32 i4VDOOmxQueueEmptyBuf(VDO_OMX_HANDLE_T *pHandle, HANDLE_T hHeader);
INT32 i4VDOOmxQueueFillBuf(VDO_OMX_HANDLE_T *pHandle, HANDLE_T hHeader);
INT32 i4VDOOmxCompFlush(VDO_OMX_HANDLE_T *pHandle, BOOL fgInput, BOOL fgOutput);
INT32 i4VDOOmxCompCheckStateTransition(VDO_OMX_HANDLE_T *pHandle);
//INT32 i4VDOOmxDoEmptyBuf(VDO_OMX_HANDLE_T *pHandle);
INT32 i4VDOOmxDoEmptyBuf(VDO_OMX_HANDLE_T *pHandle, UINT32 u4Index);
//INT32 i4VDOOmxDoFillBuf(VDO_OMX_HANDLE_T *pHandle);
INT32 i4VDOOmxDoFillBuf(VDO_OMX_HANDLE_T *pHandle, UINT32 u4Index);

extern BOOL fgVDOOmxAllInputBufferAlloced(VDO_OMX_HANDLE_T *pHandle);
extern BOOL fgVDOOmxAllInputBufferFreed(VDO_OMX_HANDLE_T *pHandle);
extern INT32 i4VDOOmxForceAllBufEmptyDone(VDO_OMX_HANDLE_T *pHandle);
extern BOOL fgVDOOmxAllOutputBufferAlloced(VDO_OMX_HANDLE_T *pHandle);
extern BOOL fgVDOOmxAllOutputBufferFreed(VDO_OMX_HANDLE_T *pHandle);
extern INT32 i4VDOOmxForceAllBufFillDone(VDO_OMX_HANDLE_T *pHandle);
extern BOOL i4VDOOmxPrintStatus(UINT32 u4InstId);
extern BOOL i4VDOOmxSetInfo(UINT32 u4InstId, UINT32 u4Info, UINT32 u4Para1, UINT32 u4Para2, UINT32 u4Para3);

void vVDOOmxFillBufDone(VDO_OMX_HANDLE_T *pHandle);
void vVDOOmxEmptyBufDone(VDO_OMX_HANDLE_T *pHandle);
void vVDOOmxCompPicInfoChange(VDO_OMX_HANDLE_T *pHandle);

//INT32 i4VDOOmxCompImgFrmDone(VDO_OMX_HANDLE_T *pHandle, void* pvData);
BOOL i4VDOOmxCompImgFrmDone(UINT32 u4Tag, void* pvData);

#endif //#ifndef _VDEC_MOD_H_




