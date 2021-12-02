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
 * $Author: hongjun.chu $
 * $Date: 2012/04/28 $
 * $RCSfile: cb_data.h,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file cb_data.h
 *  Define callback data structure.
 */

#ifndef CB_DATA_H
#define CB_DATA_H

// Notes: swdmx debug util
#ifndef SWDMX_DBG_USB
// #define SWDMX_DBG_USB
#endif // SWDMX_DBG_USB
//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

/// Callback structure of timer.
typedef struct
{
    int i4Cnt;
} TIM_CB_T;

/// Callback structure of MPV.
typedef struct
{
    unsigned int u4EsId;
    unsigned short u2H;
    unsigned short u2W;
    unsigned char ucProgressive;
} MPV_CB_T;

/// Callback struct for NPTV
typedef struct
{
    unsigned int u4SigStatus;
    unsigned short u2H;
    unsigned short u2W;
    unsigned char ucProgressive;
    unsigned char ucVdpId;
} NPTV_CB_T;

/// Callback structure of demux
typedef struct
{
    unsigned int u4SecAddr;
    unsigned int u4SecLen;
    unsigned char u1Pidx;
    unsigned char u1SerialNumber;
} DMX_CB_T;

/// Callback structure of demux
typedef struct
{
	unsigned char type;
	unsigned char u1Pidx;
	unsigned int u4Data0;
	unsigned int u4Data1;
	unsigned int u4Data2;
	unsigned int u4Data3;
} DMX_MTAL_CB_T;

/// Callback structure of demux
typedef struct
{
	unsigned char type;
	unsigned int u4Data0;
	unsigned int u4Data1;
	unsigned int u4Data2;
} TSREC_MTAL_CB_T;

typedef struct
{
	unsigned char result;
	unsigned int u4Data0;
	unsigned int u4Data1;
	unsigned int u4Data2;
	unsigned int u4Data3;
} DREC_MTAL_CB_T;

/// Callback structure of demux
typedef struct
{
    unsigned int u4Type;
    unsigned int u4Key;
} IR_CB_T;

typedef struct
{
    unsigned int  u4Rptr;
    unsigned int u4Pts;
    unsigned short u2TemporalRef;
    unsigned char u1DataType;
    unsigned char u1PicType;
    unsigned char u1PicStruct;
    unsigned char u1RepFirstFld;
    unsigned char u1TopFldFirst;
    unsigned char u1DataLen;
    unsigned char u1EsId;
    unsigned char u1ProgressiveFrm;
} DGUSERDATA_CB_T;


typedef struct
{
    unsigned int  u4DecErrNs;
    unsigned char u1EsId;
} DECERR_CB_T;


typedef struct
{
    unsigned char u1PicType;
    unsigned char u1EsId;
} PICTYPE_CB_T;


typedef struct
{
    unsigned int u4Status;
    unsigned char u1EsId;
} DEC_STATUS_CB_T;

#ifdef SWDMX_DBG_USB
typedef enum
{
    // Cmd for AP part and _CB_PutEvent
    SWDMX_DBG_DO_OPEN_FILE = 1,        // open file CMD
    SWDMX_DBG_DO_CLOSE_FILE,            // 
    SWDMX_DBG_DO_SAVE_DATA,
    SWDMX_DBG_DO_LOAD_DATA,
    SWDMX_DBG_DO_SAVE_CRC,
    SWDMX_DBG_DO_LOAD_CRC,
    SWDMX_DBG_DO_SAVE_BLOCK,
    SWDMX_DBG_DO_DEBUG_EXIT,

    // Cmd for 
    SWDMX_DBG_REQ_OPEN_FILE,        // open file CMD
    SWDMX_DBG_REQ_CLOSE_FILE,      // 
    SWDMX_DBG_REQ_SAVE_DATA,
    SWDMX_DBG_REQ_LOAD_DATA,
    SWDMX_DBG_REQ_SAVE_CRC,
    SWDMX_DBG_REQ_LOAD_CRC,
    SWDMX_DBG_REQ_SAVE_BLOCK,
    SWDMX_DBG_REQ_INIT,
    SWDMX_DBG_REQ_EXIT,
    
    SWDMX_DBG_CMD_NUM
}SWDMX_DBG_CMD_T;

typedef struct
{
    unsigned int u4Src;
    unsigned int u4File;
    unsigned int u4FileOffset;
    unsigned int u4BlockSize;
    unsigned int u4Crc;
}SWDMX_DBG_CRC_ELEMENT_T;

typedef struct
{
    unsigned int u4Src;
    unsigned int u4File;
    void *pvBufStart;
    unsigned int u4BlockSize;
    unsigned char au1FilePath[ 256 ];
}SWDMX_DBG_BUF_INFO_T;

typedef struct
{
    unsigned int u4Src;
    unsigned int u4File;
    unsigned int u4FileAddr;
    unsigned char fgAppend;
    unsigned char au1FilePath[ 256 ];
}SWDMX_DBG_FILE_INFO_T;

typedef struct
{
    SWDMX_DBG_CMD_T eCmd;
    
    union
    {
        SWDMX_DBG_FILE_INFO_T           rFileInfo;
        SWDMX_DBG_CRC_ELEMENT_T      rCrcInfo;
        SWDMX_DBG_BUF_INFO_T            rBufInfo;
    }u;

}SWDMX_DBG_T;
#endif // SWDMX_DBG_USB

typedef struct
{
    unsigned int u4Info;
    unsigned int u4Tag;
} DEC_OMX_CB_T;


/// Callback structure of SDAL AvDec Module
typedef struct
{
    unsigned int u4AVDecId;
    unsigned int u4Mask;
    unsigned int u4Param;

    // SdAVDec_Status_t
    unsigned int u4bTSDLock;
    unsigned int u4bAudioLock;
    unsigned int u4bVideoLock;
    unsigned int u4bUnmuteEnable;
    unsigned int u4eHDMIAudioFormat;

    // SdAVDec_VideoInfo_t
    unsigned int u4hResolution;
    unsigned int u4vResolution;
    unsigned int u4hStart;
    unsigned int u4vStart;
    unsigned int u4hTotal;
    unsigned int u4vTotal;
    unsigned int u4bProgressScan;
    unsigned int u4eRatio;
    unsigned int u4frameRate;
} SDAL_AVDEC_CB_T;

/// Callback structure of SDAL TSData
typedef struct
{
    unsigned int u4Addr;
    unsigned int u4Size;
    unsigned short u2Pid;
    unsigned char u1Pidx;
    unsigned char u1SerialNumber;
    unsigned char u1MonitorHandle;
} SDAL_TSDATA_CB_T;

/// Callback structure of SDAL UserData
typedef struct
{
    unsigned char u1DataType; /* 0:CC, 1:TTX, 2:WSS, 3:VPS */
    unsigned char u1Data0;
    unsigned char u1Data1;
    unsigned char u1FieldID;
    unsigned int u4NumPktAvail;
    unsigned int u4StartAddr;
    unsigned int u4WSS;
    unsigned char au1VPSData[13];
} SDAL_USERDATA_CB_T;

/// Callback structure of SDAL Digital UserData
typedef struct
{
    unsigned int u4Rptr;
    unsigned int u4Pts;
    unsigned short u2TemporalRef;
    unsigned char u1DataType;
    unsigned char u1PicType;
    unsigned char u1PicStruct;
    unsigned char u1RepFirstFld;
    unsigned char u1TopFldFirst;
    unsigned char u1DataLen;
} SDAL_DGUSERDATA_CB_T;

/// Information of a callback function

typedef struct
{
    unsigned char u1SwdmxId;
    unsigned char u1PlaymgrId;
    unsigned int u4SrcType;             ///< Callback source
    unsigned int u4Condition;           ///< Callback condition
    unsigned int u4Param;               ///< Callback paramter
} MM_NOTIFY_INFO_T;

typedef struct _MM_FILE_OPS_T
{
    unsigned long long u8Offset;
    unsigned int u4QueryID;
    unsigned char u1Whence;
} MM_FILE_OPS_T;

typedef struct
{
    MM_NOTIFY_INFO_T rMMNotify;
    MM_FILE_OPS_T rFileInfo;
    unsigned int u4QuerySize;
    unsigned int u4WritePtr;
} MM_CALLBACK_INFO_T;

/// Callback structure of TSDMX
typedef struct
{
    unsigned int u4_trsn_id;		       // transaction id
    unsigned int u4_start_add;           //VOID *pv_start_add;        // starting address
    unsigned int u4_required_len;	// required length
    unsigned char u1_reset_pos;                       //BOOL b_reset_pos; // position-resetting flag
    unsigned int u4_position;		// the position to be resettd
} IMG_REQ_DATA_T;

typedef struct
{
    unsigned int u4Condition;           ///< Callback condition
    unsigned int u4Param;               ///< Callback paramter
    IMG_REQ_DATA_T rReqDataInfo;    
} IMG_CALLBACK_INFO_T;

/// Callback structure of TSDMX
typedef struct
{
    unsigned int u4Tag;
    unsigned int u4Type;
    unsigned int u4Addr;
    unsigned int u4Len;
    unsigned int u4FilterMap;
    unsigned short u2Pid;
    unsigned char u1SerialNumber;
    unsigned char u1FilterId;
    unsigned char u1Pidx;
} TSDMX_CB_T;

/// Callback structure of PAPI_INFRA_CEC
typedef struct
{
    unsigned int   u4Type;
    int i4Size;
    unsigned char  au1Data[16];
} PAPI_INFRA_CEC_CB_T;

/// Callback structure of PAPI_AF
typedef struct
{
    unsigned int u4Type;
    unsigned int u4Org;
    unsigned int u4New;
} PAPI_INFRA_CLK_CB_T;

/// Callback structure of SRC
typedef struct
{
    int i4Type;
    int i4Data1;
    int i4Data2;
} PAPI_SRC_CB_T;

/// Callback structure of PAPI_VF
typedef struct
{
    int i4Type;
    int i4Data1;
    int i4Data2;
} PAPI_VF_CB_T;

/// Callback structure of PAPI_FE_COL
typedef struct
{
    int i4Type;
    int i4Data1;
    int i4Data2;
} PAPI_FE_COL_CB_T;

/// Callback structure of PAPI_FE_VIP
typedef struct
{
    int i4Type;
    int i4Data1;
    int i4Data2;
} PAPI_FE_VIP_CB_T;

/// Callback structure of PAPI_AF
typedef struct
{
    int i4Type;
    int i4Data1;
    int i4Data2;
} PAPI_AF_CB_T;

/// Callback structure of PAPI_AF
typedef struct
{
    unsigned int u4Type;
} PAPI_SETUP_CB_T;

/// Callback structure of PAPI_MIXER
typedef struct
{
    int i4Type;
    unsigned int  u4Data1;
    unsigned int  u4Data2;
    unsigned int  u4Data3;
    unsigned int  u4Data4;
    unsigned int  u4Data5;
    unsigned int  u4Data6;
    unsigned int  u4Data7;
    unsigned int  u4Data8;
    unsigned int  u4Data9;
  //  VMixNewVidLayerProp rVidLayerProps;
    unsigned int  u4Data10;
    unsigned int  u4Data11;
    unsigned int  u4Data12;
    unsigned int  u4Data13;
    unsigned int  u4Data14;
    unsigned int  u4Data15;
    unsigned int  u4Data16;
    unsigned int  u4Data17;
    unsigned int  u4Data18;
    unsigned int  u4Data19;

} PAPI_MIXER_CB_T;

/// Callback structure of PAPI_PAPIONLY
typedef struct
{
    int i4Type;
    int i4Data1;
     unsigned int ai4Data2[8];
} PAPI_PAPIONLY_CB_T;

/// Callback structure of PAPI_TUNER
typedef	enum
{
	PAPI_FE_CALLBACK_CARRIER_PRESENT = 0,
	PAPI_FE_CALLBACK_STATION_PRESENT,
	PAPI_FE_CALLBACK_SEARCH_IN_PROGRESS,
	PAPI_FE_CALLBACK_SEARCH_STATE_CHANGED,
	PAPI_FE_CALLBACK_TVSYSTEM_DETECTED,
	PAPI_FE_CALLBACK_AFC_FREQ_CHANGED,
	PAPI_FE_CALLBACK_AFC_LIMIT_REACHED,
	PAPI_FE_CALLBACK_AFC_MODE_CHANGED
} PAPI_FE_CALLBACK_ID_ENUM_T;

typedef struct
{
	PAPI_FE_CALLBACK_ID_ENUM_T		CbId;
	union
	{
		int				CarrierPresent;
		int				StationPresent;
		int				SearchState;
		int				TvSystem;
	    unsigned long	CurrentFreq;
	} CbIdInfo;
} PAPI_FE_CALLBACK_INFO_T;

/** VBI Information callback Data. */
typedef union
{
    struct
    {
        unsigned int u4TTX_Type;
        unsigned int u4Path;
        unsigned int u4PktsAvail;
        unsigned int u4ReadPtr;
    }TTXInfo;

    struct
    {
        unsigned int u4WSS_Type;
        unsigned int u4Path;
        unsigned int u4WSSData;
        unsigned int u4Reserved;
    }WSSInfo;

    struct
    {
        unsigned int  u4CC_Type;
        unsigned int  u4Path;
        unsigned int  u4Field;
        unsigned char u1CCData0;
        unsigned char u1CCData1;
        unsigned char u1Reserved[2];
    }CCInfo;

    struct
    {
        unsigned char u1VPSData[13];
        unsigned char u1Reserved[3];
    }VPSInfo;

    unsigned int u4AllData[4];
}VBICBDATA;

/// Callback structure of PAPI_FE_VBI
typedef struct
{
    unsigned int u4Type;
    VBICBDATA    CbData;
} PAPI_FE_VBI_CB_T;

/// Callback function ID enumeration.
typedef enum
{
    // Old style callbacks for CLI demo program
    CB_TIM_TRIGGER,
    CB_MPV_TRIGGER,
    CB_NPTV_TRIGGER,
    CB_DMX_TRIGGER,

    CB_SDAL_AVDEC_TRIGGER,      // SDAL AvDec Module
    CB_SDAL_TSDATA_TRIGGER,
    CB_SDAL_USERDATA_TRIGGER,   // SDAL UserData Module
    CB_SDAL_DGUSERDATA_TRIGGER, // SDAL Digital UserData Module
    CB_MTMPLAYER_TRIGGER,     // SDAL Feeder Module
    CB_MTIMAGE_TRIGGER,
    CB_MTIMAGE_TRIGGER_VDO,
    CB_SC_TRIGGER,
    CB_SDAL_SOUNDAUI_TRIGGER,

    CB_TSDMX_TRIGGER,           // TSDMX

    CB_PAPI_AF_TRIGGER,           // PAPI Audio Feature
    CB_PAPI_SRC_TRIGGER,           // PAPI_SRC_SRC
    CB_PAPI_VF_TRIGGER,
    CB_PAPI_TUNER_TRIGGER,		// PAPI Tuner Module
    CB_PAPI_FE_VIP_TRIGGER,
    CB_PAPI_FE_COL_TRIGGER,		// PAPI FE COL Module
    CB_PAPI_FE_SND_TRIGGER,     // PAPI FE Sound Module
    CB_PAPI_MIXER_TRIGGER,
    CB_PAPI_FE_VBI_TRIGGER,     // PAPI FE VBI Module
    CB_PAPI_SETUP_TRIGGER,     // PAPI FE VBI Module
    CB_PAPI_INFRA_CEC_TRIGGER,  	// PAPI INFRA Feature
    CB_PAPI_INFRA_CLK_TRIGGER,  	// PAPI INFRA Feature
    CB_PAPI_PAPIONLY_TRIGGER,  	// PAPIONLY Feature

    CB_MTAL_FE_US_TUNER_TRIGGER,   // MTAL FE US Tuner Module
    CB_MTAL_FE_NTSC_TUNER_TRIGGER,   // MTAL FE US NTSC Tuner Module
    CB_MTAL_FE_EU_TUNER_TRIGGER,   // MTAL FE EU Tuner Module
    CB_MTAL_FE_PAL_TUNER_TRIGGER,   // MTAL FE EU PAL Tuner Module
    CB_MTAL_FE_CH_TUNER_TRIGGER,   // MTAL FE CH Tuner Module
    CB_MTAL_FE_JAPAN_TUNER_TRIGGER,   // MTAL FE Japan Tuner Module 
    CB_MTAL_FE_SBTVD_TUNER_TRIGGER, //MTAL Brazil tuner Module   
	
    CB_MTAL_DMX_TRIGGER,           // MTAL DMX MODULE
    CB_MTAL_VDO_SRCCHG_TRIGGER,
    CB_MTAL_VDO_PTS_TRIGGER,
    CB_MTAL_VDO_AFD_TRIGGER,
    CB_MTAL_VDO_UNMUTE_TRIGGER,
    CB_MTAL_VDO_LIPSYNC_TRIGGER,
    CB_MTAL_VDO_LIPSYNC_OK_TO_NG_TRIGGER,    
    CB_MTAL_VDO_LIPSYNC_NG_TO_OK_TRIGGER,
    CB_MTAL_VDO_OUT_VSYNC_TRIGGER,
    CB_MTAL_VDO_VSS_TRIGGER,
    CB_MTAL_VDO_TDNAVI_CHG,

    CB_MTAL_TVDECEX_SCART_PIN8CHG_TRIGGER,

    CB_MTAL_VDECEX_SIG_CHG,
    CB_MTAL_VDECEX_VGA_AUTO,
    CB_MTAL_VDECEX_AUTO_COLOR,

    CB_MTAL_VDEC_USRDATA_TRIGGER,
    CB_MTAL_VDEC_DECERR_TRIGGER,
    CB_MTAL_VDEC_STATUS_TRIGGER,
    CB_MTAL_VDEC_OMX_EVENT_MESSAGE,
    CB_MTAL_VDEC_OMX_EMPTYBUFFERDONE_MESSAGE,
    CB_MTAL_VDEC_OMX_FILLBUFFERDONE_MESSAGE,
	CB_MTAL_AUDDEC_HP_TRIGGER,

    CB_MTAL_VBI_TRIGGER,            //MTAL VBI TRIGGER

    CB_MTAL_PERIPHERAL_RTC_TRIGGER,     // RTC alert Handler
    CB_MTAL_GPIO_TRIGGER,               // GPIO event Handler

    CB_MTAL_PIC_TYPE_TRIGGER,

    CB_MTAL_AUDDEC_MODE_CHG_TRIGGER,
    CB_MTAL_AUDDEC_MP3_ERROR_TRIGGER,
    CB_MTAL_AUDDEC_MIXSOUND_CTRL_TRIGGER,   
    CB_MTAL_AUDDEC_CLIP_PLAY_DONE_TRIGGER,
		CB_DRV_HDMI_NFY,
		CB_DRV_NFY,
		CB_MTAL_VDECEX_AFD,


    //DANIEL_HACK
    CB_MTAL_ADEC_OMX_EVENT_MESSAGE,
    CB_MTAL_ADEC_OMX_EMPTYBUFFERDONE_MESSAGE,
    CB_MTAL_ADEC_OMX_FILLBUFFERDONE_MESSAGE,
    //DANIEL_HACK

    CB_MTAL_TSREC_TRIGGER,
    CB_MTAL_DREC_TRIGGER,
    CB_MTAL_PVR_THUMBNAIL_TRIGGER,
    
#ifdef SWDMX_DBG_USB
    CB_SWDMX_DBG_TRIGGER,
#endif  // SWDMX_DBG_USB

    CB_MTAL_SMC_TRIGGER,
    
    CB_MTMPLAYER_IMAGE,
    CB_MTAL_TEMINATE_TRIGGER,
#ifdef LINUX_TURNKEY_SOLUTION
    CB_RM_CALLBACK=100,
    CB_RM_SELECTION,
    CB_RM_MC_ADD_DEV,
    CB_RM_MC_DEL_DEV,
    CB_RM_MC_RST_DEV,
    CB_RM_MC_ADD_INST,
    CB_RM_MC_DEL_INST,
    CB_RM_MC_SET,
    CB_DRV_IRRX,
    CB_DRV_AVC_NFY,
    CB_DRV_TV_DEC_NFY,
    CB_DRV_TV_DEC_TTX_NFY,
    CB_DRV_TV_DEC_VPS_DATA_NFY,
    CB_DRV_TV_DEC_EU_WSS_NFY,
    CB_DRV_TV_SRC_AVC_NFY,
    CB_DRV_AUD_NFY,
    CB_DRV_BAGT_NFY,
    CB_DRV_BAGT_MMAP_NFY,           /* MHF Linux - Basara */
    CB_DRV_CEC_NFY,
    CB_DRV_DMX_NFY,
    CB_DRV_DMX_SET_NFY,
    CB_DRV_MPV_SET_NFY,
    CB_DRV_MPV_FRAME_NFY,
    CB_DRV_MPV_DEC_CC_NFY,
    CB_DRV_MPV_REACH_NFY,
    CB_DRV_PCD_NFY,
    CB_DRV_VDP_NFY,
    CB_DRV_VDP_EX_NFY,
    CB_DRV_CC_NFY,
    CB_DRV_VDP_GET_CUR_DISP_FMT,
    CB_DRV_VDP_OVERSCAN,
    CB_DRV_VDP_UPDATE_OVERSCAN,
    CB_DRV_JPG_SET_NFY,             /* MHF Linux - yjg 9x mmp */
/* MHF Linux - Basara for mmp */ 
    CB_DRV_AUD_HP_NFY,              
    CB_DRV_DMX_MM_NFY,                  
    CB_DRV_MWDMX_NFY,
    CB_DRV_VDP_PLA_NFY,
/* MHF Linux - end */
#endif
    CB_FCT_NUM                  // Last one, = number of items
} CB_FCT_ID_T;


#endif  //CB_DATA_H

