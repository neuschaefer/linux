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
 * $RCSfile: venc_drvif.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file venc_drvif.h
 *  Public interface of video encoder
 */


#ifndef VENC_DRVIF_H
#define VENC_DRVIF_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "venc_if.h"

/*lint -save -e961 */
#include "x_common.h"
#include "x_typedef.h"
#include "x_timer.h"
#include "x_assert.h"
#include "x_os.h"
#include "x_bim.h"
#include "x_clksrc.h"
#include "x_hal_5381.h"
#include "x_hal_926.h"


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//#define CC_EMULATION

#ifdef CC_EMULATION
#define VENC_EMU
#endif
#ifdef VENC_EMU
//#define USB_SUPPORT
//#define VENC_CYCLE
//#define VENC_DRAM_PROFILE
//#define VENC_MT8580_PATTERN

#ifndef VENC_DRAM_PROFILE
#if defined(CC_MT5396) || defined(CC_MT5398) // MT5368 doesn't have channel B
#define DRAM_CHANNEL_AB_TEST
#endif
#endif

#if defined(VENC_CYCLE) || defined(VENC_DRAM_PROFILE)
#define VENC_LIGHT_LOG
#endif

#ifdef USB_SUPPORT
//#define SINGLE_FILE_SUPPORT
#endif

#endif

//#define VENC_DUMP_BS // tmp for dump
//#define VENC_FRAME_CTRL_BY_DRV

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define VENC_CMDQ_SIZE       (5)
#define VENC_CMDQ_PRIORITY   (255)

#define VENC_FRMQ_SIZE       (3 + VENC_CMDQ_SIZE)
#define VENC_FRMQ_PRIORITY   VENC_CMDQ_PRIORITY

#define VENC_STACK_SIZE      (2048)
#define VENC_THREAD_PRIORITY (100)

#define VENC_MAX_FRAME_BUFFER_NUM (8)

#define QVGA_WIDTH (320)
#define QVGA_HEIGHT (240)

#define VENC_DEV_USED (0xa4)

#define VENC_RESIZE_BUF_N (2)
#define VENC_RESIZE_BUF_SZ (1920*1088 * 3 / 2)
#define VENC_RESIZE_BUF_TOTAL (VENC_RESIZE_BUF_SZ * VENC_RESIZE_BUF_N)


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define VENC_ALIGN_MASK(VAL, MSK) (((VAL) + (MSK)) & (~(MSK)))

#define VENC_OBJECT_TO_HANDLE(object) ((VENC_HANDLE)(object))
#define VENC_HANDLE_TO_OBJECT(h,o)    ((o*)(h))


#define VENC_CHK_OK_BREAK(ret) if ((ret) != VENC_OK) break;
#define VENC_SUPPORT_ASYNC_MODE(m) ( VENC_SMODE_ASYNC == (m) || VENC_SMODE_DUAL == (m))

#define _VEncInitSyncCmdFlag(obj)    x_sema_lock(obj->hEncSyncSema, X_SEMA_OPTION_NOWAIT)
#define _VEncWaitSyncCmdDone(obj)    VERIFY(x_sema_lock(obj->hEncSyncSema, X_SEMA_OPTION_WAIT) == OSR_OK)
#define _VEncNotifySyncCmdDone(obj)  VERIFY(x_sema_unlock(obj->hEncSyncSema) == OSR_OK)


#define VENC_LINK_FOR_EACH_OBJECT(o) for (o=_ptVEncObjHead->next; o; o = o->next)
#define VENC_FOR_EACH_HANDLE(h) for (h=VENC_OBJECT_TO_HANDLE(_ptVEncObjHead->next); h!= VENC_NULL_HANDLE; h = VENC_OBJECT_TO_HANDLE(VENC_HANDLE_TO_OBJECT(h,VENC_OBJECT_T)->next))

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef enum
{
    VENC_CTRL_INT_GET_BS_INFO = VENC_CTRL_MAX + 1,
    VENC_CTRL_INT_GET_STATE,
    VENC_CTRL_INT_GET_NAME,
    VENC_CTRL_INT_GET_TYPE,
    VENC_CTRL_INT_MAX
}VENC_CTRL_INT_T;


typedef enum
{
    VENC_CMD_START_ASYNC = 0x01,  // start
    VENC_CMD_STOP_ASYNC  = 0x02,  // stop
    VENC_CMD_EXIT_ASYNC  = 0x03,  // thread exit

    VENC_CMD_START_SYNC  = 0x11,  // start
    VENC_CMD_STOP_SYNC   = 0x12,  // stop
    VENC_CMD_EXIT_SYNC   = 0x13,  // thread exit

    VENC_CMD_UNKNOWN     = 0xFF
} VENC_CMD_T;



typedef enum
{
    VENC_STATE_STOP,        // stop
    VENC_STATE_RUN,         // run
    VENC_STATE_UNKNOWN
} VENC_STATE_T;


typedef enum
{
    VENC_ENC_STATE_IDLE,
    VENC_ENC_STATE_ENC,
    VENC_ENC_STATE_MAX
} VENC_ENC_STATE_T;


typedef enum
{
    VENC_FRMQ_TYPE_FRM,
    VENC_FRMQ_TYPE_DUMMY,
    VENC_FRMQ_TYPE_CMD,
    VENC_FRMQ_TYPE_RT_PARM,
    VENC_FRMQ_TYPE_RT_CTRL,
    VENC_FRMQ_TYPE_MAX
} VENC_FRMQ_TYPE_T;


typedef struct
{
    UINT32 data;
}VENC_PIC_PRAM_T;

typedef struct
{
    VENC_FRMQ_TYPE_T eType;
    union{
        struct {
            VENC_IN_PIC_T    tIn;
            VENC_OUT_PIC_T   tOut;
            VENC_PIC_PRAM_T  tPram;
        }frm;
        VENC_RT_PARM_T tRTParm;
        VENC_RT_CTRL_T tRTCtrl;
    }u;
}VENC_FRMQ_DATA_T;


typedef struct _VENC_OBJECT_T
{
    //link list
    struct _VENC_OBJECT_T *next;

    UINT8   fgUsed;
    UINT8   Reserved[3];

    //variable
    UINT32  u4Type;
    UINT8   u1aName[7];
    UINT8   u1Framerate;
    UINT16  u2Width;
    UINT16  u2Height;
    UINT32  u4Bitrate;
    VENC_HANDLE hParam;

    //resize source
    UINT32 u4SrcPitch;
    UINT32 u4SrcWidth;
    UINT32 u4SrcHeight;
    VOID *pvIn;
    VOID *pvOut;

    // ctrl
    VENC_PIX_FMT_T   ePixFmt;
    UINT16           u2Pitch;
    VENC_SMODE_T     eSMode;

    VENC_STATE_T     eState;
    VENC_ENC_STATE_T eEncState;
    VENC_PROFILE_T eProfile;

#ifdef VENC_DRAM_PROFILE
    // Dram info
    UINT32                u4DramTotalCycle;
    UINT32                u4FrmCnt;
#endif

    HANDLE_T    hEncThread;
    VOID        (*pfnEncMainLoop)(VOID *);
    HANDLE_T    hEncCmdQ;
    HANDLE_T    hEncSyncSema;
    HANDLE_T    hEncFrmQ;
    VENC_HANDLE hCtrl;


    //output callback
    VOID *pvCallbackData;
    VOID (*OutputCallback)(VOID *, VENC_IN_PIC_T *, VENC_OUT_PIC_T *, UINT32);

    //working buffer
    UINT32 u4BuffAddr;
    UINT32 u4BuffSize;
    UINT32 u4UsedSize;


    //function
    UINT32      (*Close)          (struct _VENC_OBJECT_T *);
    VENC_HANDLE (*CreateParam)    (VENC_PROFILE_T);
    UINT32      (*SetParam)       (struct _VENC_OBJECT_T *, UINT32, VOID *);
    UINT32      (*GetParam)       (struct _VENC_OBJECT_T *, UINT32, VOID *);
    VENC_HANDLE (*CreateCtrl)     (VENC_PROFILE_T);
    UINT32      (*SetCtrl)        (struct _VENC_OBJECT_T *, UINT32, VOID *);
    UINT32      (*GetCtrl)        (struct _VENC_OBJECT_T *, UINT32, VOID *);
    UINT32      (*Init)           (struct _VENC_OBJECT_T *);
    UINT32      (*Run)            (struct _VENC_OBJECT_T *);
    UINT32      (*Stop)           (struct _VENC_OBJECT_T *);
    UINT32      (*EncFrameSync)   (struct _VENC_OBJECT_T *, VENC_IN_PIC_T *, VENC_OUT_PIC_T *, VOID*);
    UINT32      (*EncFrameAsync)  (struct _VENC_OBJECT_T *, VENC_IN_PIC_T *, VENC_OUT_PIC_T *, VOID*);
    UINT32      (*SetRtParamSync) (struct _VENC_OBJECT_T *, VENC_RT_PARM_T *);
    UINT32      (*SetRtParamAsync)(struct _VENC_OBJECT_T *, VENC_RT_PARM_T *);
    UINT32      (*SetRtCtrlSync)  (struct _VENC_OBJECT_T *, VENC_RT_CTRL_T *);
    UINT32      (*SetRtCtrlAsync) (struct _VENC_OBJECT_T *, VENC_RT_CTRL_T *);

}VENC_OBJECT_T;


typedef struct
{
    UINT32 u4YAddr;
    UINT32 u4UVAddr;
}VENC_FRM_BUFF_T;


typedef struct _VENC_OBJECT_HEAD_T
{
    //link list
    struct _VENC_OBJECT_T *next;
}VENC_OBJECT_HEAD_T;

extern VENC_OBJECT_HEAD_T *_ptVEncObjHead;

typedef struct
{
    UINT32 u4Addr;
    UINT32 u4Size;
    UINT32 u4Wp;
}VENC_CTRL_BS_INFO;

typedef struct
{
    VENC_STATE_T     eState;
    VENC_ENC_STATE_T eEncState;
}VENC_CTRL_STATE;

//-----------------------------------------------------------------------------
// Prototype of Inter-file functions
//-----------------------------------------------------------------------------
UINT32 _VENC_Open(VENC_HANDLE *phEnc, UINT32 u4Type, VENC_PROFILE_T eProfile);
UINT32 _VENC_Close(VENC_HANDLE hEnc);
UINT32 _VENC_Init(VENC_HANDLE hEnc);
UINT32 _VENC_Run(VENC_HANDLE hEnc);
UINT32 _VENC_Stop(VENC_HANDLE hEnc);
UINT32 _VENC_SetParam(VENC_HANDLE hEnc, UINT32 u4Type, VOID *pvArg);
UINT32 _VENC_GetParam(VENC_HANDLE hEnc, UINT32 u4Type, VOID *pvArg);
UINT32 _VENC_SetCtrl(VENC_HANDLE hEnc, UINT32 u4Type, VOID *pvArg);
UINT32 _VENC_GetCtrl(VENC_HANDLE hEnc, UINT32 u4Type, VOID *pvArg);
UINT32 _VENC_EncFrameSync(VENC_HANDLE hEnc, VENC_IN_PIC_T *ptIn, VENC_OUT_PIC_T *ptOut);
UINT32 _VENC_EncFrameAsync(VENC_HANDLE hEnc, VENC_IN_PIC_T *ptIn, VENC_OUT_PIC_T *ptOut);
UINT32 _VENC_SetRtParamSync(VENC_HANDLE hEnc, VENC_RT_PARM_T *ptParm);
UINT32 _VENC_SetRtParamAsync(VENC_HANDLE hEnc, VENC_RT_PARM_T *ptParm);
UINT32 _VENC_SetRtCtrlSync(VENC_HANDLE hEnc, VENC_RT_CTRL_T *ptParm);
UINT32 _VENC_SetRtCtrlAsync(VENC_HANDLE hEnc, VENC_RT_CTRL_T *ptParm);

UINT32 _VENC_ReceiveCmd(VENC_OBJECT_T *pObj, VOID *pvData);
VOID   _VENC_ProcessCmd(VENC_OBJECT_T *pObj, UINT32 u4Cmd, BOOL *pfgRun);
VOID   _VENC_SendFrm(VENC_OBJECT_T *pObj, VENC_FRMQ_DATA_T *prFrm);
#ifdef VENC_DRAM_PROFILE
VOID _VENC_DramAccessByteStart(VENC_OBJECT_T *this);
VOID _VENC_DramAccessByteEnd(VENC_OBJECT_T *this);
#endif


//-----------------------------------------------------------------------------
// Prototype  of public functions
//-----------------------------------------------------------------------------

typedef struct _VENC_BITS_BUFFER
{
    UINT32  i_data;
    UINT8   bit_left;
    UINT8   *p_data;
} VENC_BITS_BUFFER;


#define BITS_BUFFER rBitsBuff
#define PBITS_BUFFER prBitsBuff
#define PUT_BIT_START(buff) VENC_BITS_BUFFER BITS_BUFFER,*PBITS_BUFFER;PBITS_BUFFER=&BITS_BUFFER;BITS_BUFFER.i_data=0;if(!buff){ASSERT(buff);return VENC_INV_ARG;}inti_bits_buffer(PBITS_BUFFER,buff);
#define PUT_BIT_END(size) bits_flush(PBITS_BUFFER);size=BITS_BUFFER.i_data
#define PUT_BIT(val, n)    put_bits(PBITS_BUFFER, n, val)
#define ALIGN_BIT()    bits_align(PBITS_BUFFER)
#define PUT_BIT_UE(val)    put_bits(PBITS_BUFFER, bits_ue_size(val), val+1)
#define PUT_BIT_SE(val)    put_bits(PBITS_BUFFER, bits_se_size(val), bits_se_to_ue(val))

/*
put bit function
max 32 bit
*/
extern void put_bits(VENC_BITS_BUFFER *p_buffer, UINT8 count, UINT32 bits);
extern void inti_bits_buffer(VENC_BITS_BUFFER *p_buffer, UINT8 *p_addr);
extern void bits_align(VENC_BITS_BUFFER *p_buffer );
extern void bits_flush(VENC_BITS_BUFFER *p_buffer );
//exp-golomb code from spec 9.1
extern UINT8 bits_ue_size(UINT32 value);
extern UINT8 bits_se_size(INT32 value);
extern UINT32 bits_se_to_ue(INT32 value);

#endif // VENC_DRVIF_H


