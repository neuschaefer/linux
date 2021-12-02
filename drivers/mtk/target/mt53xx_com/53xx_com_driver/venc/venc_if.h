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
 * $RCSfile: venc_if.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file venc_if.h
 *  Public interface of software program stream demux driver
 */


#ifndef VENC_IF_H
#define VENC_IF_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
//#include "drv_common.h"
#include "x_common.h"
#include "x_typedef.h"
//#include "x_timer.h"
//#include "x_assert.h"
//#include "x_os.h"
//#include "x_bim.h"
//#include "x_clksrc.h"
//#include "x_hal_5381.h"
//#include "x_hal_926.h"
#ifdef CC_ATV_PVR_SUPPORT
#include "drv_vbi.h"
#endif

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//return value
#define VENC_OK                     (0)
#define VENC_FAIL                   (1)
#define VENC_NO_MEMORY              (2)
#define VENC_DEV_BUSY               (3)
#define VENC_INV_HANDLE             (4)
#define VENC_INV_ARG                (5)
#define VENC_INTERNAL_ERR           (6)
#define VENC_NOT_IMPL               (7)
#define VENC_WAIT_YUV               (8)

#define VENC_COMM    0x000
#define VENC_H264    0x100


#define VENC_PRAM_FLAG_RES       (1<<0)
#define VENC_PRAM_FLAG_FRAMERATE (1<<1)
#define VENC_PRAM_FLAG_BITRATE   (1<<2)

#define VENC_CTRL_FLAG_KEYFRAME  (1<<0)


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef VOID * VENC_HANDLE;
#define VENC_NULL_HANDLE  ((VOID*)0 )


typedef enum
{
    VENC_PROFILE_VPHONE = 0,    //VIDEO PHONE, ex. skype
    VENC_PROFILE_PVR,           //ATV PVR
    VENC_PROFILE_MS,            //Multiscreen interaction
    VENC_PROFILE_EMU,           //FOR EMULATION OR IC VERIFICATION
    VENC_PROFILE_SLT,           // for SLT
    VENC_PROFILE_MAX
} VENC_PROFILE_T;


typedef enum
{
    VENC_PIX_FMT_PSCAN = 0,     // DI format
    VENC_PIX_FMT_NV12,          //420 with two planes, the Y plane is first. ex.emulation
    VENC_PIX_FMT_B16X32,        //BLOCK MODE 16 X 32
    VENC_PIX_FMT_B64X32,        //BLOCK MODE 64 X 32
    VENC_PIX_FMT_MAX
} VENC_PIX_FMT_T;

typedef struct
{
    UINT32 u4YAddr;
    UINT32 u4CAddr;
    UINT32 u4Pts;
    BOOL fgRzSrc;
#ifdef CC_ATV_PVR_SUPPORT
    VBI_PVR_NOTIFY_DATA rCC_Data;
    VBI_PVR_NOTIFY_DATA rAFD_Data;
#endif
} VENC_IN_PIC_T;

typedef struct
{
    UINT32 u4YAddr;
    UINT32 u4UVAddr;
} VENC_FRM_T;

typedef struct
{
    //~~ must have the same layout with H264_EMU_OUT_PIC_T
    //data type
    UINT32 u4Type;    //[out]
    UINT32 u4Pts;
    //data
    UINT32 u4Addr;
    UINT32 u4Size;
    UINT32 u4RStart; // ring start
    UINT32 u4REnd;   // ring end
} VENC_OUT_PIC_T;


typedef enum
{
    VENC_PRAM_FRAMERATE    = VENC_COMM + 1,
    VENC_PRAM_BITRATE      ,//= VENC_COMM + 2,
    VENC_PRAM_RESOLUTION   ,//= VENC_COMM + 3,
    VENC_PRAM_MAX
} VENC_PRAM_T;


typedef enum
{
    VENC_CTRL_SYNC_MODE,
    VENC_CTRL_PIX_FMT,
    VENC_CTRL_BUFF,
    VENC_CTRL_OUT_CB,
    VENC_CTRL_RZ,
    VENC_CTRL_MAX
} VENC_CTRL_T;


typedef enum
{
    VENC_SMODE_SYNC,    // only sync api is work.
    VENC_SMODE_ASYNC,   //
    VENC_SMODE_DUAL,    //both sync api and async api  are work.
    VENC_SMODE_MAX
} VENC_SMODE_T;


typedef struct
{
    UINT16 u2Width;
    UINT16 u2Height;
} VENC_PRAM_RES_T;

typedef struct
{
    VENC_PIX_FMT_T ePixFmt;
    UINT16 u2Pitch;
} VENC_CTRL_PIXFMT_T;


typedef struct
{
    UINT32 u4Addr;
    UINT32 u4Size;
} VENC_CTRL_BUFF_T;

typedef struct
{
    VOID* pvData;
    VOID  (*cb)(VOID *, VENC_IN_PIC_T *, VENC_OUT_PIC_T *, UINT32);
} VENC_CTRL_OUTCB_T;


typedef struct
{
    UINT32 u4SrcPitch;
    UINT32 u4SrcWidth;
    UINT32 u4SrcHeight;
    VOID *pvIn;
    VOID *pvOut;
} VENC_CTRL_RZ_T; // resize


typedef struct
{
    UINT32 u4Flag;
    UINT16 u2Width;
    UINT16 u2Height;
    UINT16 u2Pitch;
    UINT32 u4Bitrate;
    UINT8  u1Framerate;
} VENC_RT_PARM_T;


typedef struct
{
    UINT32 u4Flag;
} VENC_RT_CTRL_T;


//H264 PART
typedef enum
{
    VENC_H264_PIC_TYPE_SPS = 0x01,
    VENC_H264_PIC_TYPE_PPS = 0x02,
    VENC_H264_PIC_TYPE_SEI = 0x04,
    VENC_H264_PIC_TYPE_IDR = 0x10,
    VENC_H264_PIC_TYPE_I   = 0x20,
    VENC_H264_PIC_TYPE_P   = 0x30,
    VENC_H264_PIC_TYPE_B   = 0x40,
    VENC_H264_PIC_TYPE_MAX
} VENC_H264_PIC_TYPE;


typedef enum
{
    VENC_H264_PRAM_PROFILE_LEVEL  = VENC_H264 + 1,
    VENC_H264_PRAM_2              = VENC_H264 + 2,
    VENC_H264_PRAM_MAX
} VENC_H264_PRAM_T;


typedef enum
{
    VENC_H264_CTRL_1    = VENC_H264 + 1,
    VENC_H264_CTRL_2    = VENC_H264 + 2,
    VENC_H264_CTRL_3    = VENC_H264 + 3,
    VENC_H264_CTRL_MAX
} VENC_H264_CTRL_T;

typedef struct
{
    UINT16 u2Profile;
    UINT16 u2Level;
} VENC_H264_PRAM_PL_T;



#define VENC_RT_PARM_FLAG_RES        (1L<<0)
#define VENC_RT_PARM_FLAG_BITRATE    (1L<<1)
#define VENC_RT_PARM_FLAG_FRAMERATE  (1L<<2)


#define VENC_RT_CTRL_FLAG_INSERT_KEYFRAME   (1L<<0)


//-----------------------------------------------------------------------------
// Prototype  of public functions
//-----------------------------------------------------------------------------

VENC_HANDLE VENC_Open(UINT32 u4Type, VENC_PROFILE_T eProfile);
UINT32 VENC_Close(VENC_HANDLE hEnc);
UINT32 VENC_Init(VENC_HANDLE hEnc);
UINT32 VENC_Run(VENC_HANDLE hEnc);
UINT32 VENC_Stop(VENC_HANDLE hEnc);
UINT32 VENC_SetParam(VENC_HANDLE hEnc, UINT32 u4Type, VOID *pvArg);
UINT32 VENC_GetParam(VENC_HANDLE hEnc, UINT32 u4Type, VOID *pvArg);
UINT32 VENC_SetCtrl(VENC_HANDLE hEnc, UINT32 u4Type, VOID *pvArg);
UINT32 VENC_GetCtrl(VENC_HANDLE hEnc, UINT32 u4Type, VOID *pvArg);
UINT32 VENC_EncFrameSync(VENC_HANDLE hEnc, VENC_IN_PIC_T *ptIn, VENC_OUT_PIC_T *ptOut);
UINT32 VENC_EncFrameAsync(VENC_HANDLE hEnc, VENC_IN_PIC_T *ptIn, VENC_OUT_PIC_T *ptOut);
UINT32 VENC_SetRtParamSync(VENC_HANDLE hEnc, VENC_RT_PARM_T *ptParm);
UINT32 VENC_SetRtParamAsync(VENC_HANDLE hEnc, VENC_RT_PARM_T *ptParm);
UINT32 VENC_SetRtCtrlSync(VENC_HANDLE hEnc, VENC_RT_CTRL_T *ptCtrl);
UINT32 VENC_SetRtCtrlAsync(VENC_HANDLE hEnc, VENC_RT_CTRL_T *ptCtrl);
BOOL VENC_CkgenOnOff(BOOL fgEnable);

//-----------------------------------------------------------------------------
// usage
//-----------------------------------------------------------------------------
/*
//######example 1:
//##skype: IPPPPPP.... mode
// USE SYNC MODE

VENC_HANDLE hVEnc;
UINT32 u4Value;

hVEnc = VENC_Open(VENC_H264, VENC_PROFILE_VPHONE);

if (VENC_NULL_HANDLE == hVEnc)
{
    Printf("Open venc fail!\n");
    return;
}

//30 frame per sec.
u4Value = 30;
if (VENC_OK != VENC_SetParam(hVEnc, VENC_PRAM_FRAMERATE, (VOID*)&u4Framerate))
{
    Printf("Set parameter error!\n");
    return;
}

//200k
u4Value = 204800;
if (VENC_OK != VENC_SetParam(hVEnc, VENC_PRAM_BITRATE, (VOID*)&u4Value))
{
    Printf("Set parameter error!\n");
    return;
}

//352*288
VENC_PRAM_RES_T res;
res.u2Width = 352;
res.u2Height= 288;
if (VENC_OK != VENC_SetParam(hVEnc, VENC_PRAM_RESOLUTION, (VOID*)&res))
{
    Printf("Set parameter error!\n");
    return;
}

//PIX FORMATE
VENC_CTRL_PIXFMT_T pix;
pix.ePixFmt  = VENC_PIX_FMT_NV12;
pix.u2Pitch   = 352;
if (VENC_OK != VENC_SetCtrl(hVEnc, VENC_CTRL_PIX_FMT, (VOID*)&pix))
{
    Printf("Set ctrl error!\n");
    return;
}

//SYNC MODE
u4Val = (UINT32)VENC_SMODE_SYNC;
if (VENC_OK != VENC_SetCtrl(hVEnc, VENC_CTRL_SYNC_MODE, (VOID*)&u4Val))
{
    Printf("Set ctrl error!\n");
    return;
}


//Working Buffer
VENC_CTRL_BUFF_T buff;
buff.u4Addr  = 0x00001234;
buff.u4Size   = 0x00005678;
if (VENC_OK != VENC_SetCtrl(hVEnc, VENC_CTRL_BUFF, (VOID*)&buff))
{
    Printf("Set ctrl error!\n");
    return;
}


//Init
if (VENC_OK != VENC_Init(hVEnc))
{
    Printf("Init fail!\n");
    return;
}


//Run
if (VENC_OK != VENC_Run(hVEnc))
{
    Printf("Run fail!\n");
    return;
}

//encoder frames
while (doing)
{
    UINT32 u4Ret;
    VENC_IN_PIC_T rIn;
    VENC_OUT_PIC_T rOut;

    get_frame_from_camera;

    rIn.u4YAddr  = Y_data;
    rIn.u4CAddr = C_data;

    if (use extra buffer)
    {
        rOut.u4Addr = 0x23456789; //use
        rOut.u4Size  = 0x20000;      //use
        rOut.u4RStart = 0               // line buffer
        rOut.u4REnd  = 0                // line buffer
    }
    else
    {
        rOut.u4Addr = 0; //use
        rOut.u4Size  = 0;      //use
    }

    if (VENC_OK == (u4Ret= VENC_EncFrameSync(hVEnc, &rIn, &rOut)))
    {
         process encode from rOut.u4Addr;
     }
    else
    {
        Printf("Encode frame fail, errCode:%d\n", u4Ret);
    }

}

//Stop
if (VENC_OK != VENC_Stop(hVEnc))
{
    Printf("Stop fail!\n");
    return;
}

//Close
if (VENC_OK != VENC_Close(hVEnc))
{
    Printf("Close fail!\n");
    return;
}



//######example 2:
//##skype: IPPPPPP.... mode
// USE ASYNC MODE

VENC_HANDLE hVEnc;
UINT32 u4Value;

hVEnc = VENC_Open(VENC_H264, VENC_PROFILE_VPHONE);

if (VENC_NULL_HANDLE == hVEnc)
{
    Printf("Open venc fail!\n");
    return;
}

//30 frame per sec.
u4Value = 30;
if (VENC_OK != VENC_SetParam(hVEnc, VENC_PRAM_FRAMERATE, (VOID*)&u4Framerate))
{
    Printf("Set parameter error!\n");
    return;
}

//200k
u4Value = 204800;
if (VENC_OK != VENC_SetParam(hVEnc, VENC_PRAM_BITRATE, (VOID*)&u4Value))
{
    Printf("Set parameter error!\n");
    return;
}

//352*288
VENC_PRAM_RES_T res;
res.u2Width = 352;
res.u2Height= 288;
if (VENC_OK != VENC_SetParam(hVEnc, VENC_PRAM_RESOLUTION, (VOID*)&res))
{
    Printf("Set parameter error!\n");
    return;
}

//PIX FORMATE
VENC_CTRL_PIXFMT_T pix;
pix.ePixFmt  = VENC_PIX_FMT_NV12;
pix.u2Pitch   = 352;
if (VENC_OK != VENC_SetCtrl(hVEnc, VENC_CTRL_PIX_FMT, (VOID*)&pix))
{
    Printf("Set ctrl error!\n");
    return;
}

//SYNC MODE
u4Val = (UINT32)VENC_SMODE_ASYNC;
if (VENC_OK != VENC_SetCtrl(hVEnc, VENC_CTRL_SYNC_MODE, (VOID*)&u4Val))
{
    Printf("Set ctrl error!\n");
    return;
}


//Working Buffer
VENC_CTRL_BUFF_T buff;
buff.u4Addr  = 0x00001234;
buff.u4Size   = 0x00005678;
if (VENC_OK != VENC_SetCtrl(hVEnc, VENC_CTRL_BUFF, (VOID*)&buff))
{
    Printf("Set ctrl error!\n");
    return;
}


//Output Callback
HOST_T  rHost;
rHost.host = HOST;
rHost.hVEnc = hVEnc;

VOID demo_cb(VOID *pvArg, VENC_OUT_PIC_T * ptOut)
{
    HOST_T * pHost = (HOST_T)pvArg;

    process encode from ptOut->u4Addr;
}

VENC_CTRL_OUTCB_T rCb;
rCb.pvData = (VOID *)&rHost;
rCb.cb        = demo_cb;

if (VENC_OK != VENC_SetCtrl(hVEnc, VENC_CTRL_OUT_CB, (VOID*)&rCb))
{
    Printf("Set ctrl error!\n");
    return;
}


//Init
if (VENC_OK != VENC_Init(hVEnc))
{
    Printf("Init fail!\n");
    return;
}


//Run
if (VENC_OK != VENC_Run(hVEnc))
{
    Printf("Run fail!\n");
    return;
}

//encoder frames
while (doing)
{
    UINT32 u4Ret;
    VENC_IN_PIC_T rIn;
    VENC_OUT_PIC_T rOut;

    get_frame_from_camera;

    rIn.u4YAddr  = Y_data;
    rIn.u4CAddr = C_data;

    if (use extra buffer)
    {
        rOut.u4Addr = 0x23456789; //use
        rOut.u4Size  = 0x20000;      //use
        rOut.u4RStart = 0               // line buffer
        rOut.u4REnd  = 0                // line buffer
    }
    else
    {
        rOut.u4Addr = 0; //use
        rOut.u4Size  = 0;      //use
    }

    if (VENC_OK == (u4Ret= VENC_EncFrameAsync(hVEnc, &rIn, &rOut)))
    {
        ;
    }
    else
    {
        Printf("Encode frame fail, errCode:%d\n", u4Ret);
    }

}



//Stop
if (VENC_OK != VENC_Stop(hVEnc))
{
    Printf("Stop fail!\n");
    return;
}

//Close
if (VENC_OK != VENC_Close(hVEnc))
{
    Printf("Close fail!\n");
    return;
}



//######example 3:
//##prv: BBPBBP... mode
// only support async mode, when need to generate B type Frame
// please ref example 2

hVEnc = VENC_Open(VENC_H264, VENC_PROFILE_PVR);

if (VENC_NULL_HANDLE == hVEnc)
{
    Printf("Open venc error!\n");
    return;
}

...


*/

#endif // VENC_IF_H


