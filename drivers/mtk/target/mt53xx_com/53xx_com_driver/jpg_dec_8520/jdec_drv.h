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
#ifndef __JDEC_H
#define __JDEC_H

#include "x_common.h"
#include "drv_img_dec.h"
#include "jdec_com_jfif.h"

#define JdecCopyMemory(destination, source, length) memcpy((destination), (source), (length))
#define JdecZeroMemory(destination, length) memset((destination), 0, (length))

#define JDEC_INVALID64      0xFFFFFFFFFFFFFFFF
#define JDEC_INVALID32      0xFFFFFFFF
#define JDEC_INVALID16      0xFFFF
#define JDEC_INVALID8       0xFF

#define JDEC_INST_NUM               3                   ///< the instance number
#define JDEC_PIC_INFO_CACHE_NUM     10                  ///< the picture info cache number
#define JDEC_EXTRA_COMP_ID          (JDEC_INST_NUM - 1) ///< extra jpeg component id
#ifdef JPEG_AUTO_TEST
#define JDEC_FIFO_SIZE              10*1024*1024    ///< fifo size, 2MB
#else
#define JDEC_FIFO_SIZE              512*1024    ///< fifo size, 2MB
#endif
#define JDEC_CMD_NONE               0x4A504700  ///< JPG0
#define JDEC_CMD_FRAME_START        0x4A504701  ///< JPG1
#define JDEC_CMD_DECODE             0x4A504702  ///< JPG2
#define JDEC_CMD_FRAME_END          0x4A504703  ///< JPG3
#define JDEC_CMD_STOP               0x4A504704  ///< JPG4
#define JDEC_CMD_VPD_TURN_ON        0x4A504705  ///< JPG5
#define JDEC_CMD_VPD_TURN_OFF       0x4A504706  ///< JPG6
#define JDEC_CMD_FILL_BUF           0x4A504707  ///< JPG7
#define JDEC_CMD_MAX                0x4A504708  ///< JPG8

/// Image Resizer State Machine
typedef enum 
{
    JDEC_STATE_NONE,
    JDEC_STATE_START,
    JDEC_STATE_PRS_HDR,
    JDEC_STATE_DECODING,
    JDEC_STATE_IDLE,
    JDEC_STATE_MAX
} JDEC_STATE;

/// Image Resizer Decodeing State Machine
typedef enum 
{
    JDEC_DECODE_STATE_NONE,
    JDEC_DECODE_STATE_WAIT_DATA,
    JDEC_DECODE_STATE_DECODING,
    JDEC_DECODE_STATE_END,
    JDEC_DECODE_STATE_MAX
} JDEC_DECODE_STATE;

/// Image Resizer Decodeing State Machine
typedef enum 
{
    JDEC_PRSHDR_STATE_NONE,
    JDEC_PRSHDR_STATE_WAIT_DATA,
    JDEC_PRSHDR_STATE_PARSING,
    JDEC_PRSHDR_STATE_END,
    JDEC_PRSHDR_STATE_MAX
} JDEC_PRSHDR_STATE;

/// Image Resizer Usage State
typedef enum 
{
    JDEC_USAGE_NONE,
    JDEC_USAGE_NORMAL,
    JDEC_USAGE_EXTRA,
    JDEC_USAGE_MJPG,
    JDEC_USAGE_MAX
} JDEC_USAGE;

/// Image Resizer Cache State
typedef enum 
{
    JDEC_CACHE_INVALID,
    JDEC_CACHE_INUSING,
    JDEC_CACHE_VALID,
    JDEC_CACHE_DEC_FAIL
} JDEC_CACHE_STATE;

typedef struct _JDEC_MESSAGE_T
{
    UINT32      ui4_command;
    UINT16      ui2_id;         /* For driver it is the comp_id. */
    UINT32      ui4_arg1;
} JDEC_MESSAGE_T;

typedef struct _JDEC_PIC_INFO_T
{
    UINT32              u4ImgId;                ///< picture id
    JPEG_JFIF_SOS_T     rSosInfo;
    JDEC_JFIF_DQT_T     rQTblInfo;
    JDEC_JFIF_DHT_T     rHuffTblInfo;
    JDEC_JFIF_SOF_T     rSofInfo;
    UINT32              u4Ofst1stSOS;           ///< offset of 1st SOS
	BOOL                fgSOF;			        ///< indicate that already have an SOF marker
	BOOL				fgDQT;                  ///< indicate that already have an DQT marker

    JDEC_CACHE_STATE    eCacheState;            ///< Cache State
    INT32               i4ReplaceCount;         ///< if 0, can be replaced
} JDEC_PIC_INFO_T;

typedef struct _JDEC_FIFO_INFO_T
{
    UINT32              u4FifoTrnsId;           ///< the fifo id for trns
    void                *pvFifoBuf;             ///< the source fifo buffer
    UINT32              u4WrPoint;              ///< current fifo write point
    UINT32              u4RdPoint;              ///< current fifo read point
    UINT32              u4FifoStart;            ///< fifo start address
    UINT32              u4FifoSize;             ///< fifo size
    UINT32              u4RequiredLen;          ///< require to fill buffer length
    UINT32              u4RealFilledLen;        ///< Real Filled length
    BOOL                fgFillingBuf;           ///< Is fillinf buffer
    IMG_DRV_FILL_BUF_T  rNfyFillBuf;
} JDEC_FIFO_INFO_T;

typedef struct _JDEC_INST_T
{
    UINT16              u2CompId;               ///< the Jdec Component Id
    HANDLE_T            hJdecInstId;            ///< the instance id
    HANDLE_T            hJdecMsgQId;            ///< the message queue id

    JDEC_USAGE          eJdecUsage;             ///< jpeg usage
    BOOL                fgExtraCompOn;          ///< extra component turn 1:on 2:off
    IMG_DRV_NFY_FCT_T   pfCbFunc;               ///< the callback function of MW
    VOID                *pvTag;                 ///< the callback function private data
    JDEC_STATE          eJdecState;             ///< state machine
    JDEC_DECODE_STATE   eJdecDecState;          ///< state machine of decoding
    JDEC_PRSHDR_STATE   eJdecPrsState;          ///< state machine of parsing
    UINT32              u4DecProgress;          ///< decode progress

    UINT32              u4JdecHwId;             ///< the Jpeg HW decode Id
    UINT32              u4ImgId;                ///< picture id
    void                *pvImgBuf;              ///< if MW has put whole source data into buffer
    UINT32              u4ImgSize;              ///< image source size

    JDEC_FIFO_INFO_T    rFifoInfo;              ///< fifo infomation
    void*               pvPicCache;             ///< jpeg picture cache

    UINT32              u4FrameIndex;           ///< must be 0 in Jpeg
    UINT32              u4SrcCropPosX;          ///< source picture crop X position
    UINT32              u4SrcCropPosY;          ///< source picture crop Y position
    UINT32              u4SrcCropWidth;         ///< source picture crop width
    UINT32              u4SrcCropHeight;        ///< source picture crop height

    void                *pvDstBuffer;           ///< destination buffer point
    UINT32              u4DstCropPosX;          ///< destination picture crop X position
    UINT32              u4DstCropPosY;          ///< destination picture crop y position
    UINT32              u4DstCropWidth;         ///< destination picture crop width
    UINT32              u4DstCropHeight;        ///< destination picture crop height
    UINT32              u4DstBufPitch;          ///< destination buffer pitch
    GFX_DRV_COLORMODE_T eDstColorMode;          ///< destination picture color mode
    IMG_DRV_ROTATE_T    eDstRotate;             ///< destination picture rotation degree

    ///< decode mode
    UINT32              u4CurrDecodeMCU;        ///< current decode MCU
    UINT32              u4CurrDecodeMCURow;     ///< current decode MCU ROW
    UINT32              u4JdecErrorType;        ///< error handle type
} JDEC_INST_T;

#endif // __JDEC_H
