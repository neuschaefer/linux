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
 * $RCSfile: venc_h264hw.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file venc_h264hw.h
 *  This header file declares HW setting of H.264.
 */


#ifndef VENC_H264HW_H
#define VENC_H264HW_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define VENC_H264_BASE                                   (IO_VIRT + 0x3A000)
#define IO_VENC_H264_READ_BITS(RegName,FieldName)        IO32ReadFldAlign((IO_VIRT+RegName) ,(FieldName))   
#define IO_VENC_H264_READ_REG(RegName)                   u4IO32Read4B((IO_VIRT+RegName)) 
#define IO_VENC_H264_WRITE_BITS(RegName,FieldName,value) vIO32WriteFldAlign((IO_VIRT+RegName) ,(value) ,(FieldName))
#define IO_VENC_H264_WRITE_REG(RegName, value)           vIO32Write4B((IO_VIRT+RegName), (value))

#define VIDEOCODEC_HAL_DEF_DRAM_PAGE_SIZE                (1024)

#if defined(CC_MT5395)
#define VENC_H264_HW_CYCLE (216000000)
#else
#define VENC_H264_HW_CYCLE (240000000)
#endif

// ME configuration
// BME order: DIRECT, 16x16, 16x08, 08x16, 08x08
#define VENC_H264_BME11111 (2400)
#define VENC_H264_BME11101 (2150)
#define VENC_H264_BME11001 (2000)
#define VENC_H264_BME11000 (1850)


// VIDEO_CODEC_INTM
#define BITS_FCS_INTM       ((UINT32)0x1 << 31)
#define BITS_RCU_INTM       ((UINT32)0x1 << 28)
#define BITS_ENC_UPDIP_INTM ((UINT32)0x1 << 27)
#define BITS_ENC_FRM_INTM   ((UINT32)0x1 << 26)
#define BITS_ENC_PPS_INTM   ((UINT32)0x1 << 25)
#define BITS_ENC_SPS_INTM   ((UINT32)0x1 << 24)

// VIDEO_HW_INTERRUPT 
#define BITS_ENC_SPS_INT    ((UINT32)1 << 16)
#define BITS_ENC_PPS_INT    ((UINT32)1 << 17)
#define BITS_ENC_FRM_INT    ((UINT32)1 << 18)
#define BITS_ENC_UPDIP_INT  ((UINT32)1 << 19)
#define BITS_RC_INT         ((UINT32)1 << 20)
#define BITS_MB_BIT_FLAG    ((UINT32)1 << 21)
#define BITS_FRAME_BIT_FLAG ((UINT32)1 << 22)
#define BITS_ENC_CS_INT     ((UINT32)1 << 23)

// VIDEO_SRAM_PD_CONTROL
#define BITS_ECO_ITEM_1_CTRL     ((UINT32)1 << 26)
#define BITS_ECO_ITEM_2_CTRL     ((UINT32)1 << 27)
#define BITS_SRAM_RESET_N        ((UINT32)1 << 31)


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//! \brief VideoCodecHal start options.
typedef enum VideoCodecHalStartOpt
{
    VIDEOCODEC_HAL_STARTOPT_NONE       =0, //!<: NONE
    VIDEOCODEC_HAL_STARTOPT_FIRST_SPS,     //!<: first SPS.
    VIDEOCODEC_HAL_STARTOPT_DEBUG,         //!<: debug mode enable/disable.
    VIDEOCODEC_HAL_STARTOPT_COPY_B,        //!<: copy B frame.
    VIDEOCODEC_HAL_STARTOPT_ENCODE_DIP,    //!<: update DIP index.
    VIDEOCODEC_HAL_STARTOPT_ENCODE_RCULD,  //!<: rate control unit load.
    VIDEOCODEC_HAL_STARTOPT_ENCODE_SPS,    //!<: encode a SPS/VOL.
    VIDEOCODEC_HAL_STARTOPT_ENCODE_PPS,    //!<: encode a PPS.
    VIDEOCODEC_HAL_STARTOPT_ENCODE_FRAME,  //!<: encode a frame.
    VIDEOCODEC_HAL_STARTOPT_ENCODE_FINAL,  //!<: encode final frame.
    VIDEOCODEC_HAL_STARTOPT_DECODE_SPS,    //!<: decode a SPS/VOL.
    VIDEOCODEC_HAL_STARTOPT_DECODE_FRAME,  //!<: decode a frame.
    VIDEOCODEC_HAL_STARTOPT_DECODE_FINAL,  //!<: final decode. Only use to get final frame.
    VIDEOCODEC_HAL_STARTOPT_MAX            //!<: VIDEOCODEC_STARTOPT_MAX
} VideoCodecHalStartOpt_T;


//! \brief VideoCodecHal module status.
typedef enum VideoCodecHalIntType
{
    VIDEOCODEC_HAL_INT_TYPE_NONE,           //!<: NONE
    VIDEOCODEC_HAL_INT_TYPE_ENC_SPS,        //!<: SPS encoding finish interrupt 
    VIDEOCODEC_HAL_INT_TYPE_ENC_PPS,        //!<: PPS encoding finish interrupt 
    VIDEOCODEC_HAL_INT_TYPE_ENC_FRAME,      //!<: Frame encoding finish interrupt 
    VIDEOCODEC_HAL_INT_TYPE_ENC_PARTIAL,    //!<: 
    VIDEOCODEC_HAL_INT_TYPE_ENC_STALL,      //!<: 
    VIDEOCODEC_HAL_INT_TYPE_ENC_EMPTY,      //!<: 
    VIDEOCODEC_HAL_INT_TYPE_ENC_UPDATE_DIP, //!<: Update DIP DRAM addresses interrupt 
    VIDEOCODEC_HAL_INT_TYPE_ENC_RCU,        //!<: RCU loading codes interrupt 
    VIDEOCODEC_HAL_INT_TYPE_ENC_MB_BIT,     //!<: The coded bits of one MB are larger than 3200 bits 
    VIDEOCODEC_HAL_INT_TYPE_ENC_FRAME_BIT,  //!<: The coded bits of one frame are larger than MAX_PIC_BITSTREAM_BYTE_CNT bits 
    VIDEOCODEC_HAL_INT_TYPE_ENC_CS,         //!<: Frame-level coarse search interrupt 
    VIDEOCODEC_HAL_INT_TYPE_ENC_ERROR,      //!<: 
    VIDEOCODEC_HAL_INT_TYPE_DEC_SPS,        //!<: SPS/PPS decoding finish interrupt 
    VIDEOCODEC_HAL_INT_TYPE_DEC_PPS,        //!<: 
    VIDEOCODEC_HAL_INT_TYPE_DEC_FRAME,      //!<: Frame decoding finish interrupt 
    VIDEOCODEC_HAL_INT_TYPE_DEC_PARTIAL,    //!<: 
    VIDEOCODEC_HAL_INT_TYPE_DEC_STALL,      //!<: 
    VIDEOCODEC_HAL_INT_TYPE_DEC_EMPTY,      //!<: 
    VIDEOCODEC_HAL_INT_TYPE_DEC_DISPLAY,    //!<: Display dram addr ready interrupt 
    VIDEOCODEC_HAL_INT_TYPE_DEC_ERROR,      //!<: ERR decoding finish interrupt 
    VIDEOCODEC_HAL_INT_TYPE_MAX             //!<: VideoCodecStatus MAX.
} VideoCodecHalIntType_T;


//-----------------------------------------------------------------------------
// Prototype  of inter-file functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Prototype  of public functions
//-----------------------------------------------------------------------------
VOID _VENC_H264_WaitDramWriteDone(VOID);
VOID _VENC_H264_HalSetStartClear(VideoCodecHalStartOpt_T eParam);
VOID _VENC_H264_HalSetStart(VideoCodecHalStartOpt_T eParam);
VOID _VENC_H264_HalPreStart(VENC_OBJECT_T *this);
VOID _VENC_H264_HalStart(VENC_OBJECT_T *this);
VOID _VENC_H264_HalStop(VENC_OBJECT_T *this);
VOID _VENC_H264_HalRtReset(VENC_OBJECT_T *this);
VOID _VENC_H264_HalSetYUVAddr(UINT32 u4YAddr, UINT32 u4UVAddr, UINT32 u4Idx);
VOID _VENC_H264_HalSetYCoarseAddr(UINT32 u4Addr, UINT32 u4Idx);
UINT32 _VENC_H264_HalGetOutputCurAddr(VOID);
UINT32 _VENC_H264_HalGetOutputCurSize(VOID);
BOOL _VENC_H264_HalEncDone(VOID);
UINT32 _VENC_H264_HalStuffbytes(VOID);
VOID _VEnc_H264_HalSetBrefCoarseAddr(UINT32 u4Coarse);
VOID _VEnc_H264_HalSetFrefCoarseAddr(UINT32 u4Coarse);
VOID _VEnc_H264_HalSetBSWriteAddr(UINT32 u4Addr);
VOID _VEnc_H264_HalInsertKeyframeOnOff(UINT32 u4OnOff);
VOID _VEnc_H264_HalSetBSReadAddr(UINT32 u4Addr);
UINT32 _VEnc_H264_HalGetBSCurrWriteAddr(VOID);
UINT32 _VEnc_H264_HalGetBSReadAddr(VOID);
UINT32 _VENC_H264_HalGetOutputCurAddr(VOID);
UINT32 _VENC_H264_HalGetOutputCurSize(VOID);
VOID _VEnc_H264_HalClearBSWriteEnableFlag(VOID);
UINT32 _VENC_H264_HalGetECCRC(void);;
BOOL _VEnc_H264_HalIsInsertKeyframeOn(VOID);
UINT32 _VENC_H264_u4EncCycleCount(VOID);
#endif // VENC_H264HW_H


