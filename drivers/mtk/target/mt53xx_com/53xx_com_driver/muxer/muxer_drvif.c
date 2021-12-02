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
 * $RCSfile: muxer_drvif.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file muxer_drvif.c
 *  Software program stream demux driver - public interface
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "muxer_if.h"
#include "muxer_drvif.h"
#include "muxer_mpgts.h"
#include "muxer_cmpb.h"
#include "muxer_raw.h"

#ifdef CC_ATV_PVR_SUPPORT
extern void _AUD_SetPsrStc1(UINT32 u4Val);
#endif

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
static MUXER_INFO_T _arMuxerInfo[MUXER_MAX_INST_NS];

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _MUXER_SendCmd
 *  The upper layer uses this API to send commands to Muxer main loop 
 *
 *  @retval VOID
 */
//-----------------------------------------------------------------------------
static BOOL _MUXER_SendCmd(UCHAR ucMxId, ENUM_MUXER_STATE_T eMuxerState)
{
    INT32 i4Ret;
    UINT32 u4PlayCmd = eMuxerState;
    SIZE_T zMsgSize = sizeof(UINT32);

    i4Ret = x_msg_q_send(_arMuxerInfo[ucMxId].hMuxerCmdQ, &u4PlayCmd, zMsgSize, 255);
    while (i4Ret == OSR_TOO_MANY)
    {
        x_thread_delay(10);
        LOG(0, "Muxer Cmd Queue, OSR_TOO_MANY\n");
        i4Ret = x_msg_q_send(_arMuxerInfo[ucMxId].hMuxerCmdQ, &u4PlayCmd, zMsgSize, 255);
    }

    VERIFY(i4Ret == OSR_OK);

    return TRUE;

}


//-----------------------------------------------------------------------------
/** _MUXER_ReceiveCmd
 *  Muxer main loop uses this API to get commands from the upper layer
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
static BOOL _MUXER_ReceiveCmd(UCHAR ucMxId, VOID *pvData, UINT32 u4CurState)
{
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    INT32 i4Ret;

    zMsgSize = sizeof(UINT32);

    if (u4CurState != eMUXER_STATE_RUN)
    {
        i4Ret = x_msg_q_receive(&u2MsgQIdx, pvData, &zMsgSize,
                                &(_arMuxerInfo[ucMxId].hMuxerCmdQ), 1, X_MSGQ_OPTION_WAIT);
    }
    else
    {
        i4Ret = x_msg_q_receive(&u2MsgQIdx, pvData, &zMsgSize,
                                &(_arMuxerInfo[ucMxId].hMuxerCmdQ), 1, X_MSGQ_OPTION_NOWAIT);
    }

    ASSERT(zMsgSize == sizeof(UINT32));

    if (i4Ret != OSR_OK)
    {
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _SWMUX_MainLoop
 *  Software muxer main loop
 *
 *  @retval VOID
 */
//-----------------------------------------------------------------------------
static VOID _MUXER_MainLoop(VOID *pvArg)
{
    UCHAR ucMxId;
    UINT32 u4MuxCmd;
    
    if (pvArg)
    {
        ucMxId = *(UCHAR *)pvArg;
        if (ucMxId >= MUXER_MAX_INST_NS)
        {
            ASSERT(0);
            return;
        }
    }
    else
    {
        ASSERT(0);
        return;
    }

    while (1)
    {
        if (_MUXER_ReceiveCmd(ucMxId, &u4MuxCmd, _arMuxerInfo[ucMxId].eMuxerStatus))
        {
            // Function needs to be implemented.
            _arMuxerInfo[ucMxId].eMuxerStatus = (ENUM_MUXER_STATE_T)u4MuxCmd;
        }

        if (_arMuxerInfo[ucMxId].eMuxerStatus == eMUXER_STATE_RUN)
        {
            if (!_arMuxerInfo[ucMxId].hFileHandle)
            {
#if 0
                INT32 i4Ret;
                i4Ret = x_fm_open(FM_ROOT_HANDLE, 
                    "//mnt//usb_0//evaluation.ts", 
                    FM_WRITE_ONLY/*FM_READ_WRITE*/, 
                    0777, 
                    FALSE, 
                    &_arMuxerInfo[ucMxId].hFileHandle);
                if (i4Ret != FMR_OK)
                {
                    LOG(0, "Fail to open file Err = %d\n", i4Ret);
                }
#endif
            }
            _arMuxerInfo[ucMxId].pfnMuxHdr(ucMxId);
        }
        else if (_arMuxerInfo[ucMxId].eMuxerStatus == eMUXER_STATE_STOP)
        {
#if 0
            x_fm_close(_arMuxerInfo[ucMxId].hFileHandle);
#endif
        }

    }
}


#if 0
//-----------------------------------------------------------------------------
/** _MUXER_TestLoop
 *  Muxer main loop
 *
 *  @retval VOID
 */
//-----------------------------------------------------------------------------
static VOID _MUXER_TestLoop(VOID *pvArg)
{

    UNUSED(pvArg);

    while (1)
    {
    	
    }
}
#endif


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** SWDMX_Init
 *  Initialize software program stream demux driver
 *
 *  @retval VOID
 */
//-----------------------------------------------------------------------------
VOID _MUXER_Init(VOID)
{
    int i;
    char szName[] = { "        " };
    
    for (i = 0; i < MUXER_MAX_INST_NS; i++)
    {
        if (!_arMuxerInfo[i].fgMuxerInited)
        {
            LOG(5, "Muxer Init for instance #%d\n", i);

            // create muxer command queue
            x_memcpy((VOID *)szName, "MX_CMDQx", 9);
            szName[7] = '0' + i;
            VERIFY(x_msg_q_create(&_arMuxerInfo[i].hMuxerCmdQ, szName, sizeof(UINT32),
                                  MUXER_CMD_Q_SIZE) == OSR_OK);

            // create muxer video data queue
            x_memcpy((VOID *)szName, "MX_VIDQx", 9);
            szName[7] = '0' + i;
            VERIFY(x_msg_q_create(&_arMuxerInfo[i].hMuxerVidDataQ, szName, sizeof(VENC_PES_INFO_T),
                                  MUXER_VID_PES_Q_SIZE) == OSR_OK);

            // create muxer audio data queue
            x_memcpy((VOID *)szName, "MX_AUDQx", 9);
            szName[7] = '0' + i;
            VERIFY(x_msg_q_create(&_arMuxerInfo[i].hMuxerAudDataQ, szName, sizeof(AENC_PES_INFO_T),
                                  MUXER_AUD_PES_Q_SIZE) == OSR_OK);

            // create muxer VBI data queue
            x_memcpy((VOID *)szName, "MX_VBIQx", 9);
            szName[7] = '0' + i;
            VERIFY(x_msg_q_create(&_arMuxerInfo[i].hMuxerVBIDataQ, szName, sizeof(VBIENC_PES_INFO_T),
                                  MUXER_VBI_PES_Q_SIZE) == OSR_OK);

            // create muxer data queue
            x_memcpy((VOID *)szName, "MX_DATQx", 9);
            szName[7] = '0' + i;
            VERIFY(x_msg_q_create(&_arMuxerInfo[i].hMuxerDataQ, szName, sizeof(MUXER_PES_INFO_T),
                                  MUXER_DATA_Q_SIZE) == OSR_OK);

            VERIFY(x_sema_create(&_arMuxerInfo[i].hMuxerSema, X_SEMA_TYPE_MUTEX,
                                 X_SEMA_STATE_UNLOCK) == OSR_OK);

            // create muxer main loop thread for command receiving
            x_memcpy((VOID *)szName, "MX_THDx", 8);
            szName[6] = '0' + i;
            VERIFY(x_thread_create(&_arMuxerInfo[i].hMuxerMainThread,
                                   szName, 4096, 100,
                                   (x_os_thread_main_fct)_MUXER_MainLoop, sizeof(UCHAR), (VOID *)&i) == OSR_OK);
                               
#if 0
            // create muxer main loop thread for command receiving
            VERIFY(x_thread_create(&_arMuxerInfo[i].hMuxerTestThread,
                                   "MUXER_TestThr", 4096, 255,
                                   (x_os_thread_main_fct)_MUXER_TestLoop, sizeof(UCHAR), (VOID *)&i) == OSR_OK);
#endif

            _arMuxerInfo[i].fgMuxerInited = TRUE;
            _arMuxerInfo[i].fgOccupied = FALSE;
            _arMuxerInfo[i].pfnMuxHdr = _MUXER_HdrMuxMpgTs;

            x_memset((VOID *)&_arMuxerInfo[i].rFmtInfo, 0, sizeof(_arMuxerInfo[i].rFmtInfo));
            
#ifdef CC_MUXER_EVALUATION
            _arMuxerInfo[i].pucVidSrcBuf = (UCHAR*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(MUXER_TS_PACKET_SIZE, 16));
            _arMuxerInfo[i].pucVidDestBuf = (UCHAR*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(MUXER_VID_DEST_SIZE, 16));

            _arMuxerInfo[i].pucAudSrcBuf = (UCHAR*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(MUXER_TS_PACKET_SIZE, 16));
            _arMuxerInfo[i].pucAudDestBuf = (UCHAR*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(MUXER_AUD_DEST_SIZE, 16));

            _arMuxerInfo[i].rFmtInfo.rMuxerMpgInfo.u4VidFps = 30000;
            _arMuxerInfo[i].rFmtInfo.rMuxerMpgInfo.u4AudSamplingRate = 48000;
#endif
        }
        
#ifdef CC_ATV_PVR_SUPPORT
        _AUD_SetPsrStc1(0);  // tmp solution
#endif
    }
}


//-----------------------------------------------------------------------------
/** _MUXER_GetInst
 *  Get a Muxer instance
 *
 *  @retval MX_RET
 */
//-----------------------------------------------------------------------------
MX_RET _MUXER_GetInst(UCHAR *pucMxId)
{
    int i;
    
    for (i = 0; i < MUXER_MAX_INST_NS; i++)
    {
        if (!_arMuxerInfo[i].fgOccupied)
        {
            _arMuxerInfo[i].fgOccupied = TRUE;
            *pucMxId = i;
            return MX_RET_OK;
        }
    }
    
    return MX_RET_NO_AVAIL_INST;
}


//-----------------------------------------------------------------------------
/** _MUXER_ReleaseInst
 *  Release a Muxer instance
 *
 *  @retval MX_RET
 */
//-----------------------------------------------------------------------------
MX_RET _MUXER_ReleaseInst(UCHAR ucMxId)
{
    if (_arMuxerInfo[ucMxId].fgOccupied)
    {
        _arMuxerInfo[ucMxId].fgOccupied = FALSE;
        return MX_RET_OK;
    }
    
    return MX_RET_INST_NOT_OCCUPIED;
}


//-----------------------------------------------------------------------------
/** _MUXER_QueryInst
 *  Query a Muxer instance
 *
 *  @retval MUXER_INFO_T *
 */
//-----------------------------------------------------------------------------
MUXER_INFO_T *_MUXER_QueryInst(UCHAR ucMxId)
{
    return &(_arMuxerInfo[ucMxId]);
}


//-----------------------------------------------------------------------------
/** SWDMX_Play
 *  Start to run software program stream demux
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _MUXER_Run(UCHAR ucMxId)
{
    if (ucMxId >= MUXER_MAX_INST_NS)
    {
        ASSERT(0);
        return FALSE;
    }

    if (_arMuxerInfo[ucMxId].fgMuxerInited)
    {
        return _MUXER_SendCmd(ucMxId, eMUXER_STATE_RUN);
    }
    
    return FALSE;
}


//-----------------------------------------------------------------------------
/** SWDMX_Pause
 *  Pause software demux
 *
 *  @retval VOID
 */
//-----------------------------------------------------------------------------
BOOL _MUXER_Pause(UCHAR ucMxId)
{
    if (ucMxId >= MUXER_MAX_INST_NS)
    {
        ASSERT(0);
        return FALSE;
    }

    if (_arMuxerInfo[ucMxId].fgMuxerInited)
    {
        return _MUXER_SendCmd(ucMxId, eMUXER_STATE_PAUSE);
    }
    
    return FALSE;
}


//-----------------------------------------------------------------------------
/** SWDMX_Stop
 *  Stop software program stream demux
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _MUXER_Stop(UCHAR ucMxId)
{
    if (ucMxId >= MUXER_MAX_INST_NS)
    {
        ASSERT(0);
        return FALSE;
    }

    if (_arMuxerInfo[ucMxId].fgMuxerInited)
    {
        return _MUXER_SendCmd(ucMxId, eMUXER_STATE_STOP);
    }
    
    return FALSE;
}


//-----------------------------------------------------------------------------
/** SWDMX_Flush
 *  Flush A/V data
 *
 *  @retval VOID
 */
//-----------------------------------------------------------------------------
BOOL _MUXER_Flush(UCHAR ucMxId)
{
    if (ucMxId >= MUXER_MAX_INST_NS)
    {
        ASSERT(0);
        return FALSE;
    }
    
    if (_arMuxerInfo[ucMxId].fgMuxerInited)
    {
        return _MUXER_SendCmd(ucMxId, eMUXER_STATE_FLUSH);
    }
    
    return FALSE;
}


//-----------------------------------------------------------------------------
/** _MUXER_SendVideoPes
 *  Send Video Pes to Muxer
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _MUXER_SendVideoPes(UCHAR ucMxId, VENC_PES_INFO_T *prVEncPes)
{
    INT32 i4Ret;

    i4Ret = x_msg_q_send(_arMuxerInfo[ucMxId].hMuxerVidDataQ, (void *)prVEncPes,
        sizeof(VENC_PES_INFO_T), 255);
    
    while (i4Ret == OSR_TOO_MANY)
    {
        x_thread_delay(10);
        //LOG(0, "Muxer Video Data Q, OSR_TOO_MANY\n");
        Printf("Muxer Video Data Q#%d, OSR_TOO_MANY\n", ucMxId);
        i4Ret = x_msg_q_send(_arMuxerInfo[ucMxId].hMuxerVidDataQ, (void *)prVEncPes,
            sizeof(VENC_PES_INFO_T), 255);
    }

    return FALSE;
}


//-----------------------------------------------------------------------------
/** _MUXER_ReceiveVideoPes
 *  Send Video Pes to Muxer
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _MUXER_ReceiveVideoPes(UCHAR ucMxId)
{
    INT32 i4Ret;
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;

    zMsgSize = sizeof(VENC_PES_INFO_T);

    i4Ret = x_msg_q_receive(&u2MsgQIdx, &_arMuxerInfo[ucMxId].rVEncPesInfo, &zMsgSize,
        &(_arMuxerInfo[ucMxId].hMuxerVidDataQ), 1, X_MSGQ_OPTION_WAIT);

    if (i4Ret != OSR_OK)
    {
        //LOG(3, "Fail to receive Vid Pes Error code =%d\n", i4Ret);
        Printf("Muxer #%d: Fail to receive Aud Pes Error code =%d\n", ucMxId, i4Ret);
        ASSERT(0);
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _MUXER_QueryVideoPes
 *  Query the number of Audio Pes Count
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _MUXER_QueryVideoPesCnt(UCHAR ucMxId)
{
    UINT16 u2QueueSize;
    INT32 i4Ret;

    i4Ret = x_msg_q_num_msgs(_arMuxerInfo[ucMxId].hMuxerVidDataQ, &u2QueueSize);
    
    if (i4Ret == OSR_OK)
    {
        LOG(0, "Video Q size = %d\n", u2QueueSize);
        //Printf("Video Q size = %d\n", u2QueueSize);
    }
    
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _MUXER_QueryPes
 *  Query the number of Audio Pes Count
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _MUXER_QueryPesCnt(UCHAR ucMxId)
{
    UINT16 u2QueueSize;
    INT32 i4Ret;

    i4Ret = x_msg_q_num_msgs(_arMuxerInfo[ucMxId].hMuxerDataQ, &u2QueueSize);
    
    if (i4Ret == OSR_OK)
    {
        LOG(0, "Muxer Q size = %d\n", u2QueueSize);
    }
    
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _MUXER_SendAudioPes
 *  Send Audio Pes to Muxer
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _MUXER_SendAudioPes(UCHAR ucMxId, AENC_PES_INFO_T *prAEncPes)
{
    INT32 i4Ret;

    i4Ret = x_msg_q_send(_arMuxerInfo[ucMxId].hMuxerAudDataQ, (void *)prAEncPes,
        sizeof(AENC_PES_INFO_T), 255);
    
    while (i4Ret == OSR_TOO_MANY)
    {
        x_thread_delay(10);
        //LOG(0, "Muxer Audio Data Q, OSR_TOO_MANY\n");
        Printf("Muxer Audio Data Q#%d, OSR_TOO_MANY\n", ucMxId);
        i4Ret = x_msg_q_send(_arMuxerInfo[ucMxId].hMuxerAudDataQ, (void *)prAEncPes,
            sizeof(AENC_PES_INFO_T), 255);
    }

    return FALSE;
}


//-----------------------------------------------------------------------------
/** _MUXER_ReceiveAudioPes
 *  Send Audio Pes to Muxer
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _MUXER_ReceiveAudioPes(UCHAR ucMxId)
{
    INT32 i4Ret;
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;

    zMsgSize = sizeof(AENC_PES_INFO_T);
    
    i4Ret = x_msg_q_receive(&u2MsgQIdx, &_arMuxerInfo[ucMxId].rAEncPesInfo, &zMsgSize,
        &(_arMuxerInfo[ucMxId].hMuxerAudDataQ), 1, X_MSGQ_OPTION_WAIT);

    if (i4Ret != OSR_OK)
    {
        //LOG(3, "Fail to receive Aud Pes Error code =%d\n", i4Ret);
        Printf("Muxer #%d: Fail to receive Aud Pes Error code =%d\n", ucMxId, i4Ret);
        ASSERT(0);
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _MUXER_QueryAudioPes
 *  Query the number of Audio Pes Count
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _MUXER_QueryAudioPesCnt(UCHAR ucMxId)
{
    UINT16 u2QueueSize;
    INT32 i4Ret;

    i4Ret = x_msg_q_num_msgs(_arMuxerInfo[ucMxId].hMuxerAudDataQ, &u2QueueSize);
    
    if (i4Ret == OSR_OK)
    {
        LOG(0, "Muxer #%d: Audio Q size = %d\n", ucMxId, u2QueueSize);
        //Printf("Audio Q size = %d\n", u2QueueSize);
    }
    
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _MUXER_SendVBIPes
 *  Send VBI Pes to Muxer
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _MUXER_SendVBIPes(UCHAR ucMxId, VBIENC_PES_INFO_T *prVBIEncPes)
{
    INT32 i4Ret;

    i4Ret = x_msg_q_send(_arMuxerInfo[ucMxId].hMuxerVBIDataQ, (void *)prVBIEncPes,
        sizeof(VBIENC_PES_INFO_T), 255);
    
    while (i4Ret == OSR_TOO_MANY)
    {
        x_thread_delay(10);
        //LOG(0, "Muxer VBI Data Q, OSR_TOO_MANY\n");
        Printf("Muxer VBI Data Q#%d, OSR_TOO_MANY\n", ucMxId);
        i4Ret = x_msg_q_send(_arMuxerInfo[ucMxId].hMuxerVBIDataQ, (void *)prVBIEncPes,
            sizeof(VBIENC_PES_INFO_T), 255);
    }

    return FALSE;
}


//-----------------------------------------------------------------------------
/** _MUXER_ReceiveVBIPes
 *  Receive VBI Pes from Muxer
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _MUXER_ReceiveVBIPes(UCHAR ucMxId)
{
    INT32 i4Ret;
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;

    zMsgSize = sizeof(VBIENC_PES_INFO_T);
    
    i4Ret = x_msg_q_receive(&u2MsgQIdx, &_arMuxerInfo[ucMxId].rVBIEncPesInfo, &zMsgSize,
        &(_arMuxerInfo[ucMxId].hMuxerVBIDataQ), 1, X_MSGQ_OPTION_WAIT);

    if (i4Ret != OSR_OK)
    {
        //LOG(3, "Fail to receive VBI Pes Error code =%d\n", i4Ret);
        Printf("Muxer #%d: Fail to receive VBI Pes Error code =%d\n", ucMxId, i4Ret);
        ASSERT(0);
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _MUXER_SendPes
 *  Send Video Pes to Muxer
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _MUXER_SendPes(UCHAR ucMxId, MUXER_PES_INFO_T *prPes)
{
    INT32 i4Ret;

    VERIFY(x_sema_lock(_arMuxerInfo[ucMxId].hMuxerSema, X_SEMA_OPTION_WAIT) == OSR_OK);

    i4Ret = x_msg_q_send(_arMuxerInfo[ucMxId].hMuxerDataQ, (void *)prPes,
        sizeof(MUXER_PES_INFO_T), 255);
    
    while (i4Ret == OSR_TOO_MANY)
    {
        x_thread_delay(10);
        LOG(0, "Muxer Video Data Q#%d, OSR_TOO_MANY\n", ucMxId);
        i4Ret = x_msg_q_send(_arMuxerInfo[ucMxId].hMuxerDataQ, (void *)prPes,
            sizeof(MUXER_PES_INFO_T), 255);
    }

    VERIFY(x_sema_unlock(_arMuxerInfo[ucMxId].hMuxerSema) == OSR_OK);

    return FALSE;
}


//-----------------------------------------------------------------------------
/** _MUXER_ReceivePes
 *  Send Video Pes to Muxer
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _MUXER_ReceivePes(UCHAR ucMxId)
{
    INT32 i4Ret;
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;

    zMsgSize = sizeof(MUXER_PES_INFO_T);

    i4Ret = x_msg_q_receive(&u2MsgQIdx, &_arMuxerInfo[ucMxId].rMuxerPesInfo, &zMsgSize,
        &(_arMuxerInfo[ucMxId].hMuxerDataQ), 1, X_MSGQ_OPTION_WAIT);

    if (i4Ret != OSR_OK)
    {
        LOG(3, "Muxer #%d: Fail to receive Vid Pes Error code =%d\n", ucMxId, i4Ret);
        ASSERT(0);
        return FALSE;
    }

    return TRUE;
}



//-----------------------------------------------------------------------------
/** _MUXER_SetMuxFmt
 *  Set Muxer Format
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _MUXER_SetMuxFmt(UCHAR ucMxId, ENUM_MUXER_FMT_T eMuxFmt)
{
    switch(eMuxFmt)
    {
    case MUXER_FMT_MPEG2_TS:
        _arMuxerInfo[ucMxId].pfnMuxHdr = _MUXER_HdrMuxMpgTs;
        break;
    case MUXER_FMT_CMPB:
        _arMuxerInfo[ucMxId].pfnMuxHdr = _MUXER_HdrMuxCmpb;
        break;
    case MUXER_FMT_RAW:
        _arMuxerInfo[ucMxId].pfnMuxHdr = _MUXER_HdrMuxRaw;
        break;
    default:
        return FALSE;
    }
    
    _arMuxerInfo[ucMxId].eCurFMT = eMuxFmt;
    
    return TRUE;
}



UINT32 _u4MxCliBufSz = 10485760 * 4;

volatile UCHAR *_pucTmpFrmBuf = NULL;
volatile UCHAR *_pucAudResidueBuf = NULL;
volatile UCHAR *_pucMxBuf = NULL;
volatile UCHAR *_pucTtxBuf = NULL;
UINT32 _u4MxBufWp = 0;
UINT32 _u4TtxBufWp = 0;
UINT32 _u4MxBufFrmCnt = 0;
UINT32 _u4BitPos = 0;

static VOID PUT_HALF_BYTE(UINT8 u1HalfByte)
{
    UINT8 u1OriginalByte, u1NewByte;
    
    u1OriginalByte = *(_pucMxBuf + _u4MxBufWp);
    if (_u4BitPos / 4)
    {
        u1NewByte = u1OriginalByte & 0xF0;
        u1NewByte |= (u1HalfByte & 0x0F);
        *(_pucMxBuf + _u4MxBufWp) = u1NewByte;
        _u4BitPos = 0;
        _u4MxBufWp++;
    }
    else
    {
        u1NewByte = u1OriginalByte & 0x0F;
        u1NewByte |= (u1HalfByte << 4);
        *(_pucMxBuf + _u4MxBufWp) = u1NewByte;
        _u4BitPos = 4;
    }
}

VOID vMxPutBits(UINT32 u4Pattern, UINT32 u4BitLen)
{
    UINT8 u1HalfByte;
    UINT32 u4Mask;
    UINT32 i;
    
    ASSERT(u4BitLen <= 32);
    ASSERT(u4BitLen % 4 == 0);
    
    u1HalfByte = 0;
    u4Mask = 0xF0000000 >> (32 - u4BitLen);
    for (i = 0; i < u4BitLen / 4; i++)
    {
        u1HalfByte = (u4Pattern & u4Mask) >> (u4BitLen - 4);
        u4Pattern <<= 4;
        PUT_HALF_BYTE(u1HalfByte);
    }
}


VOID _MUXER_CLI_TEST_Init(UINT32 u4Fmt)
{
    VENC_PES_INFO_T rVEncPes;
    UCHAR ucMxId;

    if (_pucTmpFrmBuf == NULL)
    {
        _pucTmpFrmBuf = (UCHAR*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(MUXER_TMP_BUF_SZ, 16));
        ASSERT(_pucTmpFrmBuf != NULL);
    }
#ifdef CC_ATV_PVR_SUPPORT
    if (_pucAudResidueBuf == NULL)
    {
        _pucAudResidueBuf = (UCHAR*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(1024, 16));
        ASSERT(_pucAudResidueBuf != NULL);
    }
#endif
    if (_pucMxBuf == NULL)
    {
        _pucMxBuf = (UCHAR*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(MX_CLI_BUF_SZ, 16));
        ASSERT(_pucMxBuf != NULL);
    }
#ifdef CC_ATV_PVR_SUPPORT
    if (_pucTtxBuf == NULL)
    {
        _pucTtxBuf = (UCHAR*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(10485760, 16));
        ASSERT(_pucTtxBuf != NULL);
    }
#endif
    _u4MxBufWp = 0;
    _u4TtxBufWp = 0;
    _u4MxBufFrmCnt = 0;
    _u4BitPos = 0;
    
    MUXER_Init();
    VERIFY(MX_RET_OK == MUXER_GetInst(&ucMxId));
    
    MUXER_SetMuxFmt(ucMxId, (ENUM_MUXER_FMT_T)u4Fmt/*MUXER_FMT_MPEG2_TS*/);
    MUXER_Run(ucMxId);
    
    if ((ENUM_MUXER_FMT_T)u4Fmt == MUXER_FMT_MPEG2_TS)
    {
        rVEncPes.u4FifoStart = 0;
        rVEncPes.u4FifoEnd = 0;
        rVEncPes.u4StartAddr = 0;
        rVEncPes.u4FrameSize = 0;
        rVEncPes.u4VideoType = 1;  // PSI
        rVEncPes.u4Pts = 0;
        rVEncPes.u2SkipFrameCount = 0;
        MUXER_SendVideoPes(ucMxId, &rVEncPes);
    }
}
