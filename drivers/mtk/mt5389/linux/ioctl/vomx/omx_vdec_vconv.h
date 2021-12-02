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
#ifndef _OMX_VDEC_VCONV_H_
#define _OMX_VDEC_VCONV_H_


typedef enum
{
    VCONV_NOTIFY_FILLBUF_DONE,
    VCONV_NOTIFY_STOP_DONE,
    VCONV_NOTIFY_PAUSE_DONE,
    VCONV_NOTIFY_CHANGE_BUF,
    VCONV_NOTIFY_FLUSH_DONE,
    VDEC_NOTIFY_STOP_DONE,
    VDEC_NOTIFY_FLUSH_DONE,
    VDEC_NOTIFY_EMPTY_BUFFER_DONE,
    VDEC_NOTIFY_PIC_INFO_CHANGE, 
} VCONV_VDEC_OMX_NOTIFY_TYPE_T;

typedef enum
{
    VCONV_CMD_FILLFBMBUF
} VCONV_CMD_TYPE_T;

typedef struct
{
    UINT32  u4FbmBufAddr;
} VCONV_FILL_FBM_BUF_PARAM_T;

typedef struct
{
    UINT32*   pu4VfifoBufAddr;
    UINT32    u4Size;
    BOOL 	     fgGotEOS;
} VDEC_INSERT_VFIFO_BUF_PARAM_T;


typedef enum
{
    VCONV_COLOR_B2R2,
    VCONV_COLOR_ARBG8888,
    VCONV_COLOR_YUV420,
    VCONV_COLOR_YUV420SemiPlanar,
    VCONV_COLOR_MAX
} VCONV_COLOR_T;


typedef struct
{
    VCONV_COLOR_T eVConvFormat; 
    UINT32 u4PortNum;               //[input param] port number

    UINT32  u4FrameWidth;           //[output param] Parse from vdec driver, default set as 1920
    UINT32  u4FrameHeight;          //[output param] Parse from vdec driver, default set as 1080
    UINT32  u4FrameRate;
} VCONV_SET_PARAM_T;

typedef struct
{
    UINT32  u4PortNum;              //[input param] port number

    UINT32  u4PortCount;            //[output param] The number of ports for this component, default set value as 2 */
    UINT32  u4StartPortNumber;      //[output param] First port number for this type of port, default set value as 0
    UINT32  u4BufferCountActual;    //[output param] The actual number of buffers allocated on this port, default set as 5
    UINT32  u4BufferCountMin;       //[output param] The minimum number of buffers this port requires, default set as 5
    UINT32  u4BufferSize;           //[output param] Size, in bytes, for buffers to be used for this channel, alignment of nFrameWidth * nFrameHeight * n
    UINT32  u4FrameWidth;           //[output param] Parse from vdec driver, default set as 1920
    UINT32  u4FrameHeight;          //[output param] Parse from vdec driver, default set as 1080
    UINT32  u4Stride;               //[output param] The pitch of line, alignment of nFrameWidth, default set as 1920
    VCONV_COLOR_T eVConvFormat;     //[output param] Color space information
} VCONV_GET_PARAM_T;

typedef struct
{
    UINT32  u4BufAddr;
    UINT32  u4BufSize;
    UINT32  u4PTS;
    UINT32  u4FillSize;
    BOOL    fgEos;
} VCONV_VDEC_OMX_NFY_PARAM;

typedef struct{
    UINT32 u4PicWidth;
    UINT32 u4PicHeight;
} VCONV_VDEC_PIC_INFO_T;

typedef void (*vOmxVdecVConvCbFunc)(void *pvCbPrivData, HANDLE_T h_Handle, VCONV_VDEC_OMX_NOTIFY_TYPE_T eNotifyType, VOID* pvNfyParam);


/**************************** VConv Interface ******************************/
extern INT32 _i4VConv_DrvInit(VOID);

extern INT32 _i4VConv_Init(VOID);

extern INT32 _i4VConv_SetNotifyFct(HANDLE_T h_Handle, vOmxVdecVConvCbFunc pvCbFunc, void *pvCbPrivData);

extern INT32 _i4VConv_GetHandle (HANDLE_T* ph_Handle);

extern INT32 _i4VConv_FreeHandle (HANDLE_T h_Handle);

extern INT32 _i4VConv_AllocFbmBuf (HANDLE_T h_Handle,
    BOOL fgUseBuf, UINT32 u4BufSize, UINT32 u4BufCount, UINT32* pu4BufAddr);

extern INT32 _i4VConv_FreeFbmBuf (HANDLE_T h_Handle, UINT32 u4BufAddr);

extern INT32 _i4VConv_SendCmd (HANDLE_T h_Handle, VCONV_CMD_TYPE_T eCmd, VOID* pvParam);

extern INT32 _i4VConv_Stop (HANDLE_T h_Handle);

extern INT32 _i4VConv_Finish (HANDLE_T h_Handle);

extern INT32 _i4VConv_Pause (HANDLE_T h_Handle);

extern INT32 _i4VConv_Resume (HANDLE_T h_Handle);

extern INT32 _i4VConv_SetParam (HANDLE_T h_Handle, VCONV_SET_PARAM_T* prParam);

extern INT32 _i4VConv_GetParam (HANDLE_T h_Handle, VCONV_GET_PARAM_T* prParam);

extern INT32 _i4VConv_SetVPush (HANDLE_T h_Handle, VOID* prVPush);

extern INT32 _i4VConv_Flush (HANDLE_T h_Handle);

extern INT32 _i4VConv_PrintStatus(HANDLE_T h_Handle);

/**************************** Vdec OMX Interface ******************************/

extern INT32 _i4Vdec_OMX_Init (VOID);

extern INT32 _i4Vdec_OMX_Uninit (HANDLE_T h_Handle);

extern INT32 _i4Vdec_SetNotifyFct(HANDLE_T h_Handle, vOmxVdecVConvCbFunc pvCbFunc, void *pvCbPrivData);

extern INT32 _i4Vdec_OMX_AllocVFifo (HANDLE_T h_Handle, VOID **ppvFifo, UINT32 u4Size, VCodeC eCodeCType);

extern INT32 _i4Vdec_OMX_FreeVFifo (HANDLE_T h_Handle);

extern INT32 _i4Vdec_OMX_Play (HANDLE_T h_Handle);

extern INT32 _i4Vdec_OMX_Stop (HANDLE_T h_Handle);

extern INT32 _i4Vdec_OMX_Finish (HANDLE_T h_Handle);

extern INT32 _i4Vdec_OMX_InsPic (HANDLE_T h_Handle, VDEC_INSERT_VFIFO_BUF_PARAM_T* prVfifoBufParam);

#endif //#ifndef _OMX_VDEC_VCONV_H_

