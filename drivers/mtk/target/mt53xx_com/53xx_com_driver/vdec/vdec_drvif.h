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
 * $RCSfile: vdec_drvif.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_drvif.h
 *  This header file declares exported APIs of MPV.
 */

#ifndef VDEC_DRVIF_H
#define VDEC_DRVIF_H


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "vdec_if.h"
#include "vdp_if.h"
//#include "mpv_drvif.h"
#ifdef ENABLE_MULTIMEDIA
#include "swdmx_drvif.h"
#endif
#include "mpv_if.h"  // for PFN_DEC_NFY_FCT... etc.....  will change it.
LINT_EXT_HEADER_BEGIN
#include "drv_dbase.h"
#include "x_os.h"
LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

// in time-shift, usb device is not fast enough to send 2x data to us.
// use I frame mode to send data.
#ifdef ENABLE_MULTIMEDIA
#define VDEC_TIME_SHIFT_2X_I_MODE
#endif

//#define CC_EMULATION  // for emulation flow only

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

// VLD ID
#ifndef VLD0
#define VLD0   0  // in the case, other don't expect we have more than one VLD hw
#endif

//ES ID
#ifndef ES0
#define ES0     0  // in the case, other don't expect we could decode have more than on es
#endif

#ifdef VDEC_WAIT_DISP
    #define VDEC_WAIT_DISP_TIME           150         // 100 ms
//    #define MPV_WAIT_DISP_TIME           100         // 100 ms
    #define VDEC_AVSYNC_WAIT_DISP_TIME    1000         // 1000 ms
#else
    #define VDEC_WAIT_DISP_TIME           0           // Don't wait display, push mode
    #define VDEC_AVSYNC_WAIT_DISP_TIME    0            
#endif

//Maximum VLD number
#define VDEC_MAX_VLD            1

// Maximum Elementary Stream number
#define VDEC_MAX_ES              1

// Picture Coding Type
#define I_TYPE            1
#define P_TYPE            2
#define B_TYPE            3
#define UNKNOWN_TYPE 255

//Skip Mode
#define IPBMode 0x0
#define IPMode 0x1
#define IMode 0x2

// config section
//After dectecting all Intra slice in No I bitstream, wait 1 P to display.
#define MPV_WAIT_P                       1

#define MPV_FIFO_CTRL_INIT               3000           // 3000 ms
#ifdef __MODEL_slt__
#define H264_FIFO_CTRL_INIT             3000         // 200 ms, 18000/90000
#else
#define H264_FIFO_CTRL_INIT             18000         // 200 ms, 18000/90000
#endif

// ESM Q Underflow Number
#define MPV_UDF_PES_NS                   8

// Maximum PES Info number in ESM queue
#define MPV_MAX_PES_NS_PER_QUEUE         768
#define H264_MAX_PES_NS_PER_QUEUE        768

#define VDEC_MSG_Q_SIZE                    1024//255//80 // es queue

#define VDEC_AVG_PIC_COUNT               200
#define VDEC_AVG_PIC_SIZE               (20*1024)

#define VDEC_BIG_B_LEVEL1               (33000)
#define VDEC_BIG_B_LEVEL2               (35000)
#define VDEC_BIG_B_LEVEL3               (37000)
#ifdef CC_MT5391
#define VDEC_BIG_B_BASE_COUNT           (30)
#else //CC_MT5360
#define VDEC_BIG_B_BASE_COUNT           (2)
#endif

// Frame Rate
#define VDEC_FRAME_RATE_1000_UNKNOWN                                       (60000)
#define VDEC_FRAME_RATE_1000_23_976                                        (23976)
#define VDEC_FRAME_RATE_1000_24                                            (24000)
#define VDEC_FRAME_RATE_1000_25                                            (25000)
#define VDEC_FRAME_RATE_1000_29_97                                         (29970)
#define VDEC_FRAME_RATE_1000_30                                            (30000)
#define VDEC_FRAME_RATE_1000_50                                            (50000)
#define VDEC_FRAME_RATE_1000_59_94                                         (59940)
#define VDEC_FRAME_RATE_1000_60                                            (60000)

#define MAX_USERDATA_SIZE          2048
#define MPV_MAX_ES 1

#define CODEC_DETECT_THRD    15

//Max RPR SIZE
#define RV_MAX_NUM_RPR_SIZES 8  

//Vdec Alignment
#define VDEC_ALIGN_MASK(value, mask)     ((value + mask) & (~mask))
//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

typedef void* VDEC_HDLR;

/** the notify conditions.
*/
typedef enum
{
    VDEC_EVT_DEC_FINISH,
    VDEC_EVT_DISP_READY,        
    VDEC_EVT_DEC_ERROR,
    VDEC_EVT_DEC_BS_END
}   VDEC_EVENT_T;

/** vdec command.
*/
typedef enum
{
    VDEC_CMD_PLAY,
    VDEC_CMD_PLAY_I_FRAME,
    VDEC_CMD_PLAY_VIDEO_CLIP,        
    VDEC_CMD_STOP,
    VDEC_CMD_AVSYNC,
    VDEC_CMD_RR,
    VDEC_CMD_DEBLOCK,
    VDEC_CMD_PAUSE,
    VDEC_CMD_CAPTURE,
    VDEC_CMD_FLUSH,
    VDEC_CMD_FLUSH_DPB,
    //VDEC_CMD_SYNC_STC,
    VDEC_CMD_MAX
}   VDEC_CMD_T;


/** decoding state.
*/
typedef enum
{
    VDEC_ST_STOP,
    	
    VDEC_ST_PLAY,
    VDEC_ST_PLAY_I_FRAME,
    
    VDEC_ST_HEADER_PARSE,
    VDEC_ST_DEC_TRIGGER,
    VDEC_ST_WAIT_DEC_FINISH,
    VDEC_ST_DROP,
    
    VDEC_ST_IDLE,
    VDEC_ST_AVSYNC,
    VDEC_ST_RR,
    VDEC_ST_DEBLOCK,
    VDEC_ST_MAX
}   ENUM_VDEC_STATE_T;


/** play state.
*/
typedef enum
{
    VDEC_DEC_HEADER_PARSE,
    VDEC_DEC_TRIGGER,
    VDEC_DEC_WAIT_DEC_FINISH,
    VDEC_DEC_DROP
}   ENUM_VDEC_DEC_STATE_T;

typedef enum
{
    E_VDEC_OK,
    E_VDEC_DISP_NOT_READY,
    E_VDEC_PARSE_NOT_READY,
    E_VDEC_FAIL,
    E_VDEC_BS_END,
    E_VDEC_NOT_SUPPORT, //for emulation    
    E_VDEC_SKIPFRAME,
    E_VDEC_NOT_CODED_FRAME,
    E_VDEC_OK_RES_CHG,
    E_VDEC_RETRIGGER,
    E_VDEC_MAX
} ENUM_VDEC_AVC_RET;


/** the frame rate of video source.
*/
typedef enum
{
    VDEC_SRC_FRAME_RATE_UNKNOWN = 0,
    VDEC_SRC_FRAME_RATE_23_976, /* 24000/1001 (23.976...) */
    VDEC_SRC_FRAME_RATE_24,
    VDEC_SRC_FRAME_RATE_25,
    VDEC_SRC_FRAME_RATE_29_97, /* 30000/1001 (29.97...) */
    VDEC_SRC_FRAME_RATE_30,
    VDEC_SRC_FRAME_RATE_50,
    VDEC_SRC_FRAME_RATE_59_94, /* 60000/1001 (59.94...) */
    VDEC_SRC_FRAME_RATE_60,
    VDEC_SRC_FRAME_RATE_120,
    VDEC_SRC_FRAME_RATE_1,
    VDEC_SRC_FRAME_RATE_5,
    VDEC_SRC_FRAME_RATE_8,
    VDEC_SRC_FRAME_RATE_10,
    VDEC_SRC_FRAME_RATE_12,
    VDEC_SRC_FRAME_RATE_15,
    VDEC_SRC_FRAME_RATE_2,
    VDEC_SRC_FRAME_RATE_6,
    VDEC_SRC_FRAME_RATE_48,
    VDEC_SRC_FRAME_RATE_70,
    VDEC_SRC_FRAME_RATE_VARIABLE
} ENUM_VDEC_FRAME_RATE_T;


/* from MW VID_DEC_SET_TYPE_CODEC settings *******************************************/
typedef enum
{
    VDEC_CODEC_UNKNOWN = 0,
    VDEC_CODEC_MPEG_1,
    VDEC_CODEC_MPEG_2,
    VDEC_CODEC_MPEG_4,
    VDEC_CODEC_DIVX_311,
    VDEC_CODEC_DIVX_4,
    VDEC_CODEC_DIVX_5,
    VDEC_CODEC_XVID,
    VDEC_CODEC_WMV1,
    VDEC_CODEC_WMV2,
    VDEC_CODEC_WMV3,
    VDEC_CODEC_WVC1,
    VDEC_CODEC_H264,
    VDEC_CODEC_H263,
    VDEC_CODEC_MJPEG,
    VDEC_CODEC_RV8,
    VDEC_CODEC_RV9
}   ENUM_VDEC_CODEC_T;


//ES Info in ESMQ
typedef struct 
{
    BOOL fgSeqHdr;
    BOOL fgGop;
    BOOL fgDtsValid;      //DTS is valid
    BOOL fgNoVdecCalPts;
    UCHAR ucMpvId;
    UCHAR ucEsId;
    UCHAR ucPicType;
    UINT32 u4FifoStart;
    UINT32 u4FifoEnd;
    UINT32 u4VldReadPtr;
    UINT32 u4PTS;
    UINT32 u4DTS;
    UINT32 u4VideoType;
    BOOL fgEos;    
    BOOL fgSeqEnd;
    BOOL fgNonBuffer;
    UINT8 u1SliceCount;
    UINT32 u4RenderPts;
    UINT32 u4TotalTimeOffset;
    UINT32 u4TickNum;
    UINT64 u8Offset;
    UINT64 u8OffsetI;
    UINT16 u2DecodingOrder;
    UINT32 u4DmxFrameType;
    UINT16 u2SkipVidCount;
    #ifdef CC_DMX_PES_EXT_BUFFER
    UINT32 u4PesExtBufStart;
    UINT32 u4PesExtBufEnd;
    UINT32 u4PesExtBufCurrentRp;
    UINT32 u4PesExtBufCurrentWp;
    #endif
    //BOOL fgSkipFrame;  // only enable this for WMV emulation!
} VDEC_PES_INFO_T;


//for emulation
typedef struct
{  
    void* pvYAddr;
    void* pvCAddr;
    void* pvYDbkAddr;
    void* pvCDbkAddr;
    void* pvMVAddr;
    UINT32 u4YSize;
    UINT32 u4CSize;
    UINT32 u4W;
    UINT32 u4H;
    UINT32 u4Ycrc[4];
	UINT32 u4Ccrc[4];
    UINT32 u4ChromaFormatIdc;
    UINT8 u1PicStruct;
    //for wmv	
    UINT32 u4CurrentPicNo;	
	UINT8  u1FrameCodingMode;	
	UINT32 i4CurrentTemporalField;	
	UINT32 u4W_Cmp;	
	UINT32 u4H_Cmp;
}VDEC_DISP_PIC_INFO_T;


typedef struct
{
    UINT32  u4DecErrNs;
    UCHAR u1EsId;
}VDEC_DECERR_CB_T;


typedef struct
{
    UCHAR u1PicType;
    UCHAR u1EsId;
}VDEC_PICTYPE_CB_T;


typedef struct
{
    UINT32 u4Status;
    UCHAR u1EsId;
}VDEC_STATUS_CB_T;

//status mask
#define VDEC_DATA_ERROR  (0x1 << 0)
#define VDEC_PIC_DECODING_ERROR  (0x1 << 1)
#define VDEC_BUF_OVERFLOW  (0x1 << 2)
#define VDEC_BUF_UNDERFLOW  (0x1 << 3)
#define VDEC_FRM_DROP  (0x1 << 4)
#define VDEC_FRM_REPEAT  (0x1 << 5)

//Flags for op code
#define VDEC_OP_DISABLE_MPEG2    (0x1 << 0)
#define VDEC_OP_DISABLE_H264     (0x1 << 1)
#define VDEC_OP_DISABLE_MPEG4    (0x1 << 2)
#define VDEC_OP_DISABLE_VC1      (0x1 << 3)
#define VDEC_OP_DISABLE_MJPEG    (0x1 << 4)
#define VDEC_OP_DISABLE_RV       (0x1 << 5)

//Flags for decoding
#define DEC_FLG_2ND_FLD_PIC   (0x1 << 0)
#define DEC_FLG_WAIT_NEXT_I   (0x1 << 1)
#define DEC_FLAG_RACING_FULLB  (0x1 << 2)
#define DEC_FLAG_RACING_HALFB  (0x1 << 3)
#define DEC_FLAG_HALF_REF          (0x1 << 4)
#define DEC_FLAG_REDUCE_REF          (0x1 << 4)
#define DEC_FLAG_DBK                   (0x1 << 5)
#define DEC_FLG_WAIT_AVC_RAP           (0x1 << 6)

#define SET_DECFLAG(prVdecEsInfo,arg)      (prVdecEsInfo->u4DecFlag |= (arg))
#define CLR_DECFLAG(prVdecEsInfo,arg)      (prVdecEsInfo->u4DecFlag &= (~(arg)))
#define TOGGLE_DECFLAG(prVdecEsInfo,arg)   (prVdecEsInfo->u4DecFlag ^= (arg))
#define IS_DECFLAG_SET(prVdecEsInfo,arg)   ((prVdecEsInfo->u4DecFlag & (arg)) > 0)
/**
*/
typedef void (*PFN_VDEC_SW_RST)(
    UCHAR ucMpvId,
    UCHAR ucEsId
);

/**
*/
typedef INT32 (*PFN_VDEC_DECODE)(
    UCHAR ucMpvId,
    UCHAR ucEsId
);

/**
*/
typedef INT32 (*PFN_VDEC_HEADER_PARSE)(
    UCHAR ucMpvId,
    UCHAR ucEsId
);

/**
*/
typedef INT32 (*PFN_VDEC_DEC_START)(
    UCHAR ucMpvId,
    UCHAR ucEsId
);

/**
*/
typedef INT32 (*PFN_VDEC_DEC_FINISH)(
    UCHAR ucMpvId,
    UCHAR ucEsId
);

/**
*/
typedef void (*PFN_VDEC_DEC_DROP)(
    UCHAR ucEsId
);


/**
*/
typedef void (*PFN_VDEC_STOP)(
    UCHAR ucEsId
);

/**
*/
typedef void (*PFN_VDEC_GET_PES_STRUCT)(
    UCHAR ucEsId,
    void **ppvPesInfo
);

/** for emulation
*/
typedef void (*PFN_VDEC_GET_DISP_PIC)(
    UCHAR ucEsId,
    void* pvInfo
);

typedef void (*PFN_VDEC_SET_PARAM)(
    UCHAR ucEsId,
    UINT32 u4Type, UINT32 u4Param1, UINT32 u4Param2, UINT32 u4Param3
);

typedef void (*PFN_VDEC_GET_PARAM)(
    UCHAR ucEsId,
    UINT32 u4Type, UINT32* u4Param1, UINT32* u4Param2, UINT32* u4Param3
);

typedef void (*PFN_VDEC_UPD_RPTR)(
    UCHAR ucEsId
);    

typedef void (*PFN_VDEC_SET_WPTR)(
    UCHAR ucEsId,
    UINT32 u4WPtr
);

/*typedef void (*PFN_VDEC_EVENT_NOTIFY)(
    UINT32 u4Arg1, 
    UINT32 u4Arg2, 
    UINT32 u4Arg3, 
    UINT32 u4Arg4
);*/

/**
*/
typedef BOOL (*PFN_VDEC_ES_FLW_CTRL)(
    UCHAR ucEsId,
    UCHAR* ucMpvId,
    INT8 i1EsBufStatus,
    UCHAR ucSkipMode
);

typedef BOOL (*PFN_VDEC_WAIT_RANDOM_AP)(
    const void* prPesInfo
);

typedef UCHAR (*PFN_VDEC_HANDLE_OVERFLW)(
    UCHAR ucMpvId, 
    UCHAR ucEsmQId,
    HANDLE_T hMutex,
    HANDLE_T hMsgQueue,
    const void* pvPesInfo
);


typedef void (*PFN_VDEC_DECERR_CB)
(
	VDEC_DECERR_CB_T* prDecErr
);

typedef void (*PFN_VDEC_PIC_TYPE_CB)
(
	VDEC_PICTYPE_CB_T* prPicType
);

typedef void (*PFN_VDEC_STATUS_CB)
(
	VDEC_STATUS_CB_T* prStatus
);

typedef void (*PFN_VDEC_RENDER_PTS_CB)
(
	UINT32 u4Pts
);


typedef void (*PFN_VDEC_TRICK_PTS_CB)
(
	UINT32 u4Pts,
    BOOL fgAgain
);

typedef void (*PFN_VDEC_START_PTS)(
    UCHAR ucEsId,
    BOOL fgEnCalPTS,
    UINT32 u4Pts,
    UINT32 u4PtsDiff
);

typedef struct _MPV_EXT_INFO_T
{
    UCHAR           aucUserDataBuf[MAX_USERDATA_SIZE];        
    BOOL            fgUserData[MPV_MAX_ES];
    BOOL            fgPesUserData[MPV_MAX_ES];
    BOOL            fgAfd[MPV_MAX_ES];    
    //BOOL            fgSdUserDataFilter[MPV_MAX_ES];    
    //UINT32          u4FilterMask[MPV_MAX_ES];
    UINT32          u4UserDataRptr;
    UINT32          u4UserDataWptr;    
    PFN_MPV_AVDEC_CB   pfAVDecCb[MPV_MAX_ES];
    PFN_MPV_USERDATA_CB   pfUserDataCb;
       
} MPV_EXT_INFO_T;
//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

EXTERN void _VDEC_SendCmd(UCHAR ucEsId, UINT32 u4Cmd);

/** Send Es Info.
*@param .
 */
EXTERN UCHAR VDEC_SendEs(const VDEC_PES_INFO_T* pvPesInfo);

EXTERN void VDEC_SetWptr(UINT8 u1VdecId, UINT32 u4Wptr);

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

typedef struct
{
    UINT16                     u2_width;      /* horizontal_size_value */
    UINT16                     u2_height;     /* vertical_size_value */
    ENUM_VDEC_FRAME_RATE_T   e_frame_rate;   /* Frame Rate */
    VDEC_ASPECT_RATIO_T e_src_asp;      /* Display Aspect Ratio (DAR) */
    UINT32                      u4SequenceHeader;
    UINT16                      u2SequenceHeaderSize;
} VDEC_SEQUENCE_DATA_T;

/**
 *    Input compressed image and output destination information.
 */ 
typedef struct {
    // one each esid
    BOOL fgFlushEsmQ;    
    BOOL fgFlushFB;
            
    // comes from PesInfo
    BOOL fgDtsValid;    
    BOOL fgSeqHdr;
    BOOL fgChgThrsd;
    BOOL fgSetDmxIntThrsd;
    BOOL fgCalDmxIntThrsd;    
    BOOL fgCanDisp;        
    //BOOL afgIntraSlice[0xAE];    
    BOOL fgAllIntraSlice;
    BOOL fgBisOk;
    BOOL fgSetStartPts;
    BOOL fgRefNoDbk;
    BOOL fgIFrameDec;  // for mpeg2 MHEG5 only
    BOOL fgKeepPes;
    BOOL fgMMPlayback;
    BOOL fgMMSeqInfoValid;
    BOOL fgMMTPTSeqInfoValid;    
    BOOL fgTricking;
    BOOL fgPlayOneFrm;
    BOOL fgRenderFromFirstPic; // trick/seek used.
    BOOL fgRenderPicCnt;   // digest used.
    BOOL fgRenderFromPos;  // trick 2x up -> 1x.
    BOOL fgRenderPts;
    BOOL fgRenderBackStep;
    BOOL fgRenderFromPts;
    BOOL fgRenderLastFrame;
    BOOL fgSendedEos;
    BOOL fgBigBForceEnable;
    BOOL fgOnlyGotSeqHeader;
    BOOL fgDetectIntra;
    BOOL fgIsTPT;
    //mpeg2
    BOOL fgLowDelay;
    BOOL fgContrained;
    BOOL fgMPEG2; // mpeg1 or mpeg2
    BOOL fgSeqDispExt;
    //~mpeg2
#ifdef CC_MT5392B
    BOOL fgOffMjc;
#endif
    
    UCHAR ucDropBNs;
    UCHAR ucStarted;
    
    UCHAR ucPicType;    
    UCHAR ucFbgId;
    UCHAR ucFbId;
    UCHAR ucSyncMode;    
    UCHAR ucStcId;    
    UCHAR ucDbkRefId;
    UCHAR ucDbkFbId;
    UCHAR ucSequenceHdrFound;
    UCHAR ucVideoFmt;
    //mpeg2
    UCHAR ucProfileId;
    //~mpeg2
    
    UINT8 u1PVRWaitSps;
    UINT16 u2Count;
    UINT16 u2UdfNs;
    UINT16 u2DecodingOrder;
    //UINT16 u2TempRef;
    UINT16 u2RenderDecodingOrder;
    UINT16 u2Disp2EmptyCnt;

    UINT16 u2LineSize;                              // horizontal line size, due to block color mode
    UINT16 u2HSize;                                 // horizontal size value to dispaly
    UINT16 u2VSize;                                 // vertical size value to display
    UINT16 u2DHS;                                 // display_horizontal_size in Sequence display extension 
    UINT16 u2DVS;                                 // display_vertical_size in Sequence display extension 
    UINT16 u2OrgHSize;                              // original horizontal size value in Sequence Header
    UINT16 u2OrgVSize;                              // original vertical size value in Sequence Header
    
    INT32 i4RenderTempRef;
    
    UINT32 u4EsId;
    UINT32 u4VldId;
    UINT32 u4FifoStart;
    UINT32 u4FifoEnd;
    UINT32 u4VldReadPtr;
    UINT32 u4PTS;
    UINT32 u4PTSLast;
    UINT32 u4DTS;
    UINT32 u4LastVldRptr;
    UINT32 u4DmxIntThrsd;
    UINT32 u4TotalPicSize;
    UINT32 u4AvgPicSize;
    UINT32 u4PicCnt;
    #if 0
    UINT32 u4PicCntJump;
    #endif
    UINT32 u4DecFlag;
    UINT32 u4WaitP;
    UINT32 u4DecReadPtr;
    UINT32 u4WaitDispTime;
    UINT32 u4MaxEsCnt;
    UINT32 u4FailCnt;
    UINT32 u4FbgFailCnt;
    UINT32 u4NsAtTimeout;
    UINT32 au4DecTime[5][9]; //[3]P,B,I,SP,SI, [4]maxMs, minMs, totalMs, FrmNo, skipFrm, SwTime, DecCycle, MaxDecCycle, DramCycle    
    UINT32 u4FrameRate;

    UINT32 u4MMSpeed;
    UINT32 u4RenderPts;
    UINT32 u4RenderPicCnt;
    UINT32 u4DisplayQPicCnt;
    UINT32 u4FirstDispPTS;
    UINT32 u4RenderFromPts;
    
    UINT32 u4TotalTime;
    UINT32 u4TotlaTimeOffset;
    UINT32 u4TickNum;
    
    UINT32 u4BigBLevel1;
    UINT32 u4BigBLevel2;
    UINT32 u4BigBLevel3;
    UINT32 u4BigBBaseCnt;
    //mpeg2
    UINT32 u4VbvSizeExt;
    //~mpeg2
   
    UINT64 u8RenderFromPos; 
    UINT64 u8Offset;    
    UINT64 u8OffsetLast;    
    UINT64 u8OffsetI;
    UINT64 u8FileSize;
    
    VDEC_SEQUENCE_DATA_T rMMSeqInfo;    
    VDEC_SEQUENCE_DATA_T rMMTPTSeqInfo;
    
    // Should be name to VDEC instead of MPV
    // change it when 264 use it?
    MPV_AVDEC_STATUS_T rAVDecStatus;
    VDEC_DEC_DECODE_STATUS_T eNotifyStatus;

    PARAM_MPV_T *prMpvCounter;// PARAM_MPV_T
    
#ifdef MPV_SEMI_HOST
    UINT32 u4CompareNs;
#endif
#ifdef MPV_REPEAT_TEST
    BOOL fgParseOnRepeat;
#endif

#ifdef __MODEL_slt__
    UCHAR fgCountPicNs;
    UCHAR fgCountPicFinish;    
    UINT32 u4StartPts; 
    UINT32 u4EndPts;
    UINT32 u4PicNs;
#endif
    //for video digest thumbnail
    UINT16 u2CompId;
    VDP_THUMBNAIL_INFO_T rCapDstInfo;
    VDP_CAPTURE_INTO_T rCapSrcInfo;  
    BOOL bIsOnePass;

    //mm start PTS
    BOOL fgEnCalPTS;
    UINT32 u4StartPTS;
    UINT32 u4VFifoDataSz;
    UINT32 u4VFifoPrevWptr;
    VDEC_PTS_INFO_T rPTSInfo;
    
    // auto pause after "PlayUntilPTS" (for Adobe)
    BOOL fgAutoPause;

    //Container Type
#ifdef ENABLE_MULTIMEDIA
    ENUM_SWDMX_FMT_T eContainerType;
    SWDMX_SOURCE_TYPE_T eMMSrcType;
 #ifndef CC_VDEC_RV_DISABLE
	//rv RPR info
	BOOL   fgRV9;
	UINT32 u4SzOfPicSz;
	UINT32 au4RPRSizes[2 * RV_MAX_NUM_RPR_SIZES];
#endif	
   
#endif
    BOOL fgFmtDectFinish;
    UINT8 arRcvPesCnt[VDEC_FMT_MAX];
} VDEC_ES_INFO_T;


/**
 *    Input compressed image and output destination information.
 */ 
typedef struct {
    HANDLE_T hCmdQ;
    HANDLE_T hMsgQ;
    HANDLE_T hUnderFlowSema;  // begin, es underflow lock
    HANDLE_T hMutex;                   // PES receivelock (between threads)
    HANDLE_T hFlushMutex;         // lock for fgFlushEsmQ variable
    HANDLE_T hFlushSema;         //  lock for cli??
    HANDLE_T hCcSema;
    HANDLE_T hDataTimer;        // timer to keep watching input data
    HANDLE_T hCodecChkTimer;        // timer to check if codec type error    

    BOOL fgThreadActive;
    BOOL fgThreadDestroy;
    BOOL fgDmxPartialPes;
    BOOL fgTimerStarted;
    BOOL fgCodecTypeChecking;
    BOOL fgVdecMidEnable;
    BOOL fgMidDBKDisable;
    UINT8 u1AvSyncTestMode;
    UINT8 u1FbmMidMode;
    UINT32 u4AvSyncCnt;
	#ifdef CRCFLOWTEST
	UINT32 u4Ycrc[4];
	UINT32 u4Ccrc[4];
	#endif
    
    void*           pvDecNfyTag;    
    PFN_VDEC_DEC_NFY_FCT pfDecNfyFct;
    
    UCHAR            ucMode; //stc sync mode
    INT16            i2DBKAdjust; // -29 ~ + 29
    UINT16           u2DBKLevel;  // 0~100
    
    UINT32           u4DBKValue;  //for MPEG deblocking, P1~P4
} VDEC_ES_INFO_KEEP_T;

typedef struct {
    BOOL fgPowerOn;
    HANDLE_T hVldSemaphore;
} VDEC_VLD_INFO_T;

typedef struct {
    VDEC_ES_INFO_T arVdecEsInfo[VDEC_MAX_ES];
    VDEC_ES_INFO_KEEP_T arVdecEsInfoKeep[VDEC_MAX_ES]; // we don't reset this variable
    VDEC_VLD_INFO_T arVdecVldInfo[VDEC_MAX_VLD];
    
    // VDEC
    BOOL fgVDecInitiated;
    
    UINT32 u4OpCode;
    
    // setting?
    BOOL fgSave;
    BOOL fgCompare;
    BOOL fgWaitIFrame;

    UINT32 u4CurStopId;
    UINT32 u4PreviousStopId;
    
    ENUM_VDEC_STATE_T eCurState;
    ENUM_VDEC_STATE_T ePreviousState; // to prevent stop twice
    ENUM_VDEC_DEC_STATE_T eDecStatus;
    ENUM_VDEC_FMT_T eCurFMT;
    
    //UINT32 u4DataSize;
    VDEC_PES_INFO_T rPesInfo;
    
    PFN_VDEC_EVENT_NOTIFY pfErrNotify;
    PFN_VDEC_SW_RST pfnVdecSWRst;
    PFN_VDEC_DECODE pfnVdecDecode;
    PFN_VDEC_HEADER_PARSE pfnVdecHdrParse;
    PFN_VDEC_DEC_START pfnVdecDecStart;
    PFN_VDEC_DEC_FINISH pfnVdecDecFin;
    PFN_VDEC_DEC_DROP pfnVdecDecDrop;
    PFN_VDEC_STOP pfnVdecStop;
    PFN_VDEC_GET_PES_STRUCT pfnVdecGetPesStruct;
    PFN_VDEC_GET_DISP_PIC pfnVdecGetDispPic;
    PFN_VDEC_SET_PARAM pfnVdecSetParam;
    PFN_VDEC_GET_PARAM pfnVdecGetParam;    
    PFN_VDEC_UPD_RPTR pfnVdecUpdRptr;
    PFN_VDEC_SET_WPTR pfnVdecSetWptr;
    PFN_VDEC_ES_FLW_CTRL pfnVdecEsFlwCtrl;
    PFN_VDEC_WAIT_RANDOM_AP pfnVdecWaitRandomAP;
    PFN_VDEC_HANDLE_OVERFLW pfnVdecHandleOverFlw;
    PFN_VDEC_START_PTS pfnVdecStartPts;

    UINT32 u4ErrDuration;
    PFN_VDEC_DECERR_CB pfnDecErrCb;    
    PFN_VDEC_PIC_TYPE_CB pfPicTypeCb;
    PFN_VDEC_STATUS_CB pfnStatusCb;
    PFN_VDEC_RENDER_PTS_CB pfnRenderPtsCb;
    PFN_VDEC_TRICK_PTS_CB pfnTrickPtsCb;
} VDEC_INFO_T;


EXTERN VDEC_ES_INFO_T* _VDEC_GetEsInfo(UCHAR ucEsId);   
EXTERN VDEC_ES_INFO_KEEP_T* _VDEC_GetEsInfoKeep(UCHAR ucEsId);               
EXTERN VDEC_INFO_T* _VDEC_GetInfo(void);
EXTERN VDEC_PES_INFO_T* _VDEC_GetPesInfo(void);
EXTERN void _VDEC_LockCcSema(UCHAR ucEsId);
EXTERN void _VDEC_UnlockCcSema(UCHAR ucEsId);
EXTERN void _VDEC_CreateEsmSema(UCHAR ucEsId);
EXTERN void _VDEC_DeleteEsmSema(UCHAR ucEsId);
EXTERN void _VDEC_InitInfo(void);
EXTERN void _VDEC_ClearInfo(void);
EXTERN void _VDEC_ClearMpvCc(UCHAR ucEsId);
EXTERN void* _VDEC_GetMpvCc(UCHAR ucEsId);
EXTERN void _VDEC_FlushEs(UCHAR ucEsId);
EXTERN void _VDEC_ClearEsInfo(UCHAR ucEsId);

EXTERN void _VDEC_LockFlushSema(UCHAR ucEsId);
EXTERN void _VDEC_UnlockFlushSema(UCHAR ucEsId);
EXTERN void _VDEC_LockFlushMutex(UCHAR ucEsId);
EXTERN void _VDEC_UnlockFlushMutex(UCHAR ucEsId);

// vdec_flw.c
EXTERN void _VdecCheckStatus(UCHAR ucEsId, BOOL fgDecodeOK);

EXTERN void VDEC_WaitSyncPoint(UCHAR ucEsId, BOOL fgTricking);
#if 0
EXTERN UINT32 VDEC_GetDecPicCount(UCHAR ucEsId);
#endif
EXTERN UINT32 VDEC_GetAvgPicSize(UCHAR ucEsId);

EXTERN void VDEC_GetDecStatus(UCHAR ucEsId, BOOL* pfgLock
    , BOOL* pfgTopFldFirst, BOOL* pfgRepFirstFld, BOOL* pfgProgressiveFrm
    , UCHAR* pucPicType, UCHAR* pucPicStruture, UCHAR* pucTemporalRef
    , UINT32* pu4ReceiveNs, UINT32* pu4DecOkNs);

EXTERN BOOL VDEC_RegDecErrCb(PFN_VDEC_DECERR_CB pfDecErrCb, UINT32 u4ErrDuration);
EXTERN BOOL VDEC_RegPicTypeCb(PFN_VDEC_PIC_TYPE_CB pfnPicTypeCb);
EXTERN BOOL VDEC_RegStatusCb(PFN_VDEC_STATUS_CB pfnStatusCb);

EXTERN BOOL VDEC_PlayOneFrm(UCHAR ucEsId, ENUM_VDEC_FMT_T eFmt);
EXTERN BOOL VDEC_RegRenderPtsCb(PFN_VDEC_RENDER_PTS_CB pfnRenderPtsCb);
EXTERN BOOL VDEC_RegTrickPtsCb(PFN_VDEC_TRICK_PTS_CB pfnTrickPtsCb);
EXTERN BOOL VDEC_SetRenderPts(UCHAR ucEsId, UINT32 u4Pts);
EXTERN BOOL VDEC_SetTotalTime(UCHAR ucEsId, UINT32 u4TotalTime);
EXTERN BOOL VDEC_SetFileSize(UCHAR ucEsId, UINT64 u8FileSize);
EXTERN BOOL VDEC_SetLastTime(UCHAR ucEsId, UINT32 u4LastTime);
EXTERN BOOL  VDEC_SetRenderBackStep(UCHAR ucEsId, BOOL fgRenderBackStep, UINT64 u8Pos,
    INT32 i4TempRef, UINT16 u2DecodingOrder);
EXTERN BOOL VDEC_SetRenderFromPos(UCHAR ucEsId, BOOL fgRenderFromPos,
    UINT64 u8Pos, INT32 i4TempRef, UINT16 u2DecodingOrder);
EXTERN BOOL VDEC_SetRenderFromPts(UCHAR ucEsId, UINT32 u4Pts);
EXTERN BOOL VDEC_SetRenderFromPtsAutoPause(UCHAR ucEsId, UINT32 u4Pts, BOOL fgAutoPause);
EXTERN BOOL VDEC_SetRenderFromFirstPic(UCHAR ucEsId, BOOL fgRenderFromFirstPic,
    UINT64 u8LastRenderPos);
EXTERN BOOL VDEC_SetRenderPicCount(UCHAR ucEsId, BOOL fgRenderPicCnt, UINT32 u4Count);
EXTERN BOOL VDEC_SetGetSeqHdrOnlyBegin(UCHAR ucEsId,
    ENUM_VDEC_FMT_T eFmt, BOOL *pfgPowerOff);
EXTERN BOOL VDEC_SetGetSeqHdrOnlyEnd(UCHAR ucEsId, BOOL fgPowerOff);
EXTERN BOOL VDEC_SetSuperFreeze(UCHAR ucEsId, BOOL fgSuperFreeze);
EXTERN BOOL VDEC_CheckStopStatus(UCHAR ucEsId, UINT32 u4PTS, INT32 i4TempRef);
EXTERN BOOL VDEC_IsNotDisplay(UCHAR ucEsId, UINT32 u4PTS, INT32 i4TempRef, UINT64 u8Offset);
EXTERN BOOL VDEC_ReleaseDispQ(UCHAR ucEsId);
EXTERN BOOL VDEC_PowerOn(UCHAR ucVldId);
EXTERN BOOL VDEC_PowerOff(UCHAR ucVldId);
EXTERN void VDEC_StartPTS(UCHAR ucEsId, BOOL fgEnCalPTS, UINT32 u4Pts, VDEC_PTS_INFO_T* prPTSInfo);
EXTERN BOOL VDEC_PrsSeqHdr(ENUM_VDEC_FMT_T eFmt,
    VDEC_PES_INFO_T* prPesInfo, VDEC_SEQUENCE_DATA_T* prSeqInfo);

EXTERN BOOL VDEC_SetMid(UCHAR ucEsId, BOOL fgVdecMidEnable);
EXTERN BOOL _VDEC_MidSetting(UCHAR ucEsId, VDEC_DISP_PIC_INFO_T* prDispInfo);
EXTERN void VDEC_FbmMidTest(UCHAR ucEsId, UINT8 u1Mode, BOOL fgDBKDisable);
EXTERN void VDEC_AVSyncTest(UCHAR ucEsId, UINT8 u1Mode);
EXTERN void VDEC_GenPTS(UCHAR ucFbgId, UCHAR ucFbId);
EXTERN void VDEC_FbgChgHdlr(UCHAR ucEsId, UCHAR ucFbgId, UINT32 u4FBStart, UINT32 u4FBEnd);
EXTERN void VDEC_FbgReleaseHdlr(void);
EXTERN INT32 _VDEC_DecCfg(UCHAR ucEsId);

#endif //VDEC_DRVIF_H

