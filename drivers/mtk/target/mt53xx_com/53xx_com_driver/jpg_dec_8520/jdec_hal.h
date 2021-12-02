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
/*! \file hal_vdec_mpeg_if.h
*
*  \par LEGAL DISCLAIMER
*
* (Header of MediaTek Software/Firmware Release or Documentation)
*
* BY OPENING OR USING THIS FILE, USER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND
* AGREES THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
* ARE PROVIDED TO USER ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS
* ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
* IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
* NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH
* RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED
* IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND USER AGREES TO LOOK ONLY TO
* SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL
* ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO USER'S
* SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
* USER HEREBY ACKNOWLEDGES THE CONFIDENTIALITY OF MEDIATEK SOFTWARE AND AGREES
* NOT TO DISCLOSE OR PERMIT DISCLOSURE OF ANY MEDIATEK SOFTWARE TO ANY THIRD
* PARTY OR TO ANY OTHER PERSON, EXCEPT TO DIRECTORS, OFFICERS, EMPLOYEES OF
* USER WHO ARE REQUIRED TO HAVE THE INFORMATION TO CARRY OUT THE PURPOSE OF
* OPENING OR USING THIS FILE.
*
* \par Project
*    MT8520
*
* \par Description
*    Jpeg Decoder HAL interface definition.
*
* \par Author_Name
*    CK Hu
*
* \par Last_Changed
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



#ifndef _HW_JDEC_H_
#define _HW_JDEC_H_

#include "jdec_hal_if.h"
#include "x_typedef.h"
#include "jdec_com_common.h"

#ifndef CC_MT5363
    #ifdef  CC_MT5365
    #define CC_MT5363
    #define JPG_CLK_FROM_GRAPHIC
    #endif
    
    #ifdef  CC_MT5395
    #define CC_MT5363
    #define JPG_CLK_FROM_GRAPHIC
    #define JPG_CLK_FROM_GRAPHIC_5395_PLL
    #endif    
#endif

#define IO_BASE_ADDRESS                     0x20000000L
#define VLD0_REG_OFFSET 0x2000
#define VLD1_REG_OFFSET 0x2000
#define MC0_REG_OFFSET  0xa000
#define MC1_REG_OFFSET  0xa000

#define HW_JDEC_VLD_NUM 2
#define HW_JDEC_MC_NUM 2

//JPEG decoding register address define
#define RO_VLD_BARL      0x00
#define RW_VLD_PROC             0x8C
    #define VLD_SSCBIT              ((unsigned)0x1 << 16)
    #define VLD_SSCBYTE             ((unsigned)0x1 << 17)
    #define VLD_ABRT                ((unsigned)0x1 << 18)
    #define VLD_PSUP                ((unsigned)0x1 << 19)
    #define VLD_INIFET              ((unsigned)0x1 << 20)
    #define VLD_PDHW                ((unsigned)0x1 << 22)
    #define VLD_INIBR               ((unsigned)0x1 << 23)
    #define VLD_PERRCON             ((unsigned)0x1 << 24)
    #define VLD_RTERR               ((unsigned)0x1 << 25)
    #define VLD_EOFR                ((unsigned)0x1 << 26)
    #define VLD_IGBCL               ((unsigned)0x1 << 27)
    #define VLD_DECTOP              ((unsigned)0x1 << 29)
    #define VLD_DECBTM              ((unsigned)0x2 << 29)
    #define VLD_FPRBS               ((unsigned)0x3 << 29)
    #define VLD_B21EN               ((unsigned)0x1 << 31)
#define RW_VLD_TABLIM           0xA0
#define RW_VLD_TABLNIM          0xA4
#define RW_VLD_RPTR             0xB0
#define RW_VLD_VSTART           0xB4
#define RW_VLD_VEND             0xB8
#define RW_VLD_PWRSAVE          0xC4
#define RW_VLD_ASMR             0xD0
#define RO_VLD_FETCHOK          0xE8
    #define VLD_FETCH_OK            ((unsigned)0x1 << 0)
    #define VLD_DRAM_REQ_FIN        ((unsigned)0x1 << 2)
#define RW_VLD_HW_READ_POINT    0xFC
#define RW_VLD_WAITT            0x100
#define WO_VLD_SW_RESET         0x108
#define RW_VLD_RISC_WP          0x110
    #define VLD_SW_RESET_CLEAR       0x0
    #define VLD_SW_RESET_RESET       0x1
#define WO_VLD_JPG_BITS_TBL     0x118
#define RW_VLD_JPG_HUFF_TBL     0x11C
	#define VLD_AC_HUFF_FLAG		0x80000000
#define RW_VLD_JPG_Q_TBL        0x120
#define RW_VLD_JPG_LAHUFF_IDX   0x124
#define RW_VLD_JPG_LAHUFF_DATA  0x128
#define RW_VLD_JPG_PRG_MISC1    0x12C
#define RW_VLD_JPG_BLK_DC_TBL   0x130
#define RW_VLD_JPG_BLK_AC_TBL   0x134
#define RW_VLD_JPG_DC_AC_NEEDED 0x138
#define RW_VLD_JPG_MEMBERSHIP   0x13C
#define RW_VLD_JPG_START        0x140
	#define VLD_DECODE_MCU_CLEAR            0
	#define VLD_DECODE_MCU		            1
#define RW_VLD_JPG_SWITCH       0x144
#define RW_VLD_JPG_FAST_DEC     0x148
	#define VLD_CLEAN_INTERNAL_BUFFER		0
	#define VLD_KEEP_INTERNAL_BUFFER        1
#define RW_VLD_JPG_RESTART      0x14C
	#define VLD_MCU_RESTART			0x1
	#define VLD_RES_DC_PRED			0x101
	#define VLD_RES_MKR_FND			0x10101
#define RW_VLD_JPG_PRG_MODE     0x150
    #define PROG_MODE_LAST_SCAN     (1<<3)
    #define DECODE_MODE_PROG        (1<<8)
    #define JPG_DC_REFINE 1
    #define JPG_AC_FIRST 2
    #define JPG_AC_REFINE 3
    #define JPG_DC_FIRST 4
    #define WR_IDCT_DATA 8
    #define WR_VLD_COEFF 0
#define RW_VLD_JPG_PRG_NWB      0x154
	#define VLD_PROG_BUFF_ALL		0
	#define VLD_PROG_BUFF_NZ_ONLY	1
#define RW_VLD_JPG_AH_AL        0x158
#define RW_VLD_JPG_SS_SE        0x15C
#define RW_VLD_JPG_NZ_HIST      0x160
#define RW_VLD_JPG_MCU_COUNT    0x164
#define RW_VLD_JPG_WR_EOB_RUN   0x168
#define RW_VLD_JPG_RD_ADD0      0x16C
#define RW_VLD_JPG_RD_ADD1      0x170
#define RW_VLD_JPG_RD_ADD2      0x174
#define RW_VLD_JPG_WR_ADD0      0x178
#define RW_VLD_JPG_WR_ADD1      0x17C
#define RW_VLD_JPG_WR_ADD2      0x180
#define RW_VLD_JPG_X_IN_MCU     0x184
#define RW_VLD_JPG_Y_IN_MCU     0x188
#define RW_VLD_JPG_IDCT_WIDTH01 0x18C
#define RW_VLD_JPG_IDCT_WIDTH2_COEFF_WIDTH0   0x190
#define RW_VLD_JPG_COEFF_WIDTH12    0x194
#define RW_VLD_JPG_RMK_AND_FINISH   0x198
	#define VLD_MCU_DECODE_FINISH           1
	#define VLD_RECEIVE_MARKER              (0x1<<16)
#define RW_VLD_JPG_CLR_ST_MRK   0x19C
#define RO_VLD_JPG_RD_EOB_RUN   0x1A0
#define RW_VLD_JPEG_MARK_REG    0x1A8
#define RO_VLD_JPG_BITS_LEFT    0x1AC
#define RO_VLD_JPG_INTERNAL_BUF 0x1B0


#define RW_VLD_STA_JPG_DEC_MB_ROW           0x2A8
    #define JPG_ROW_DEC_FIRST_ROW               0x1
    #define JPG_ROW_DEC_NEXT_ROW                0x100
#define RW_VLD_JPG_MB_ROW_DEC_SWITCH        0x2AC
    #define JPG_ROW_DEC_WR_ADDR                 0x100
    #define JPG_ROW_DEC_WR_BANK1_ADDR           0x200
    #define JPG_ROW_DEC_MCU_LEVEL               0x0
    #define JPG_ROW_DEC_MCU_ROW_LEVEL           0x1
    #define JPG_ROW_DEC_PIC_LEVEL               0x3
#define RW_VLD_JPG_ROW_DEC_COMP0_ADDR       0x2B0
#define RW_VLD_JPG_ROW_DEC_COMP1_ADDR       0x2B4
#define RW_VLD_JPG_ROW_DEC_COMP2_ADDR       0x2B8
#define RW_VLD_JPG_ROW_DEC_COMP0_ADDR_OFST  0x2BC
#define RW_VLD_JPG_ROW_DEC_COMP1_ADDR_OFST  0x2C0
#define RW_VLD_JPG_ROW_DEC_COMP2_ADDR_OFST  0x2C4
#define RW_VLD_JPG_PIC_SIZE                 0x2C8
    #define JPG_ROW_DEC_WIDTH_SHIFT             0x10
    #define JPG_ROW_DEC_HEIGHT_SHIFT            0x0
#define RW_VLD_JPG_ROW_PIC_DEC_FINISH       0x2CC
    #define JPG_ROW_DEC_PIC_FINISH              (0x1 << 8)
    #define JPG_ROW_DEC_MCU_ROW_FINISH          (0x1 << 0)
#define RW_VLD_JPG_RESTART_INTERVAL         0x2D0
    #define JPG_RST_ENABLE                  (0x1 << 16)
#define RW_VLD_JPG_LOAD_INTER_DATA_TRIGGER  0x2D4
#define RW_VLD_JPG_LOAD_INTER_DATA1         0x2D8
#define RW_VLD_JPG_LOAD_INTER_DATA2         0x2DC
#define RW_VLD_JPG_LOAD_INTER_DATA3         0x2E0
#define RW_VLD_JPG_LOAD_INTER_DATA4         0x2E4
#define RW_VLD_JPG_LOAD_INTER_DATA5         0x2E8
#define RW_VLD_JPG_SRAM_CLEAR               0x2EC

#define RW_VLD_JPG_JFILLH                   0x3D4 
    #define JPG_HUFF_DC_AC_SELECTION        12
    #define JPG_HUFF_TABLE_IDENTIFIER       8
    #define JPG_HUFF_TABLE_ADDR             0
#define RW_VLD_JPG_JHVAL                    0x3D8


// MC register define
#define RW_MC_ADDRSWAP                      0x90
#define RW_MC_PS_DRAM_MODE                  0x1C8//read/write Dram address mode

extern void vHwJdecVLDPowerOn(UINT32 u4HwId,BOOL fgPowerOn);
extern void vHwJdecHwReset(UINT32 u4HwId);
extern void vHwJdecSRAMClear(UINT32 u4HwId);
extern void vHwJdecSetVLDWaitTimeoutValue(UINT32 u4HwId,UINT32 u4TimeoutValue);
extern void vHwJdecSetDecodeModeProgressive(UINT32 u4HwId);
extern void vHwJdecSetDecodeModeBaseline(UINT32 u4HwId);
extern void vHwJdecSetAhAl(UINT32 u4HwId,UINT32 Ah,UINT32 Al);
extern void vHwJdecSetSsSe(UINT32 u4HwId,UINT32 Ss,UINT32 Se);
extern void vHwJdecSetDcAc(UINT32 u4HwId,UINT32 Ah,UINT32 Ss);
extern void vHwJdecSetLastScan(UINT32 u4HwId,BOOL fgLastScan);
extern void vHwJdecCleanInterBuf(UINT32 u4HwId,BOOL fgClean);
extern void vHwJdecSetNoneZeroHistoryBuf(UINT32 u4HwId,UINT32 u4NoneZeroHistoryBuf);
extern void vHwJdecSetOutputNoneZeroHistoryBufOnly(UINT32 u4HwId,BOOL fgNzBufOnly);
extern void vHwJdecSetInputBuffer(UINT32 u4HwId,UINT32 u4InBufSa,UINT32 u4InBufSz);
extern INT32 i4HwJdecSetBarrelShifterAddr(UINT32 u4HwId,UINT32 u4Addr,UINT32 u4Bits);
extern void vHwJdecSetVLDBarrelShifterToJpegMode(UINT32 u4HwId,BOOL fgJpegMode);
extern void vHwJdecSetMiscReg(UINT32 u4HwId);
extern INT32 i4HwJdecCalculateOutputBufAddr(UINT32 u4HwId,UINT32 u4CurrMCUNum,
                                                   JDEC_JFIF_SOF_T *prSofInfo,
                                                   JDEC_JFIF_SOS_T *prScanInfo,
                                                   JDEC_HAL_OUTPUT_INFO_T *prOutputInfo,
                                                   UINT32 u4OutCoefYBufSa,
                                                   UINT32 u4OutCoefCbBufSa,
                                                   UINT32 u4OutCoefCrBufSa,
                                                   UINT32 *pu4YBufSa,
                                                   UINT32 *pu4CbBufSa,
                                                   UINT32 *pu4CrBufSa,
                                                   UINT32 *pu4YCoeffBufSa,
                                                   UINT32 *pu4CbCoeffBufSa,
                                                   UINT32 *pu4CrCoeffBufSa, JDEC_HAL_DEC_PROG_T eFlow);
extern void vHwJdecSetOutputBufAddr(UINT32 u4HwId,UINT32 u4Comp0Addr,UINT32 u4Comp1Addr,UINT32 u4Comp2Addr,
                                           JDEC_JFIF_SOS_T *prScanInfo);
extern void vHwJdecSetOutputCoefBufAddr(UINT32 u4HwId,UINT32 u4Comp0Addr,UINT32 u4Comp1Addr,UINT32 u4Comp2Addr,
                                               JDEC_JFIF_SOS_T *prScanInfo);
extern void vHwJdecSetReadWriteOutputBufMode(UINT32 u4HwId);
extern void vHwJdecInitScan(UINT32 u4HwId);
extern void vHwJdecRestart(UINT32 u4HwId);
extern void vHwJdecSetBlockAddrList(UINT32 u4HwId,UINT32 *au4XAddrList,UINT32 *au4YAddrList);
extern void vHwJdecSetCurrMCUNum(UINT32 u4HwId,UINT32 u4CurrMCUNum);
extern void vHwJdecDecodeMCU(UINT32 u4HwId);
extern BOOL fgHwJdecIsDecodeFinish(UINT32 u4HwId);
extern BOOL fgHwJdecIsFoundMarker(UINT32 u4HwId);
extern UINT32 u4HwJdecGetFoundMarker(UINT32 u4HwId);
extern INT32 i4HwJdecSetBlockPostition(UINT32 u4HwId,JDEC_JFIF_SOF_T *prPicInfo,JDEC_JFIF_SOS_T *prScanInfo);
extern INT32 i4HwJdecSetHuffmanTable(UINT32 u4HwId,JDEC_JFIF_DHT_T *prHuffmanTable,
                              UINT8 *pu1HwDcHuffTbl,UINT8 *pu1HwAcHuffTbl);
extern INT32 i4HwJdecSetBlkAndDcAcTblList(UINT32 u4HwId,JDEC_SOF_COMP_T *prSofComp,JDEC_JFIF_SOS_T *prScanInfo);
extern INT32 i4HwJdecSetBlkAndCompMembership(UINT32 u4HwId,JDEC_SOF_COMP_T *prSofComp,JDEC_JFIF_SOS_T *prScanInfo);
extern INT32 i4HwJdecSetQuantTable(UINT32 u4HwId,JDEC_JFIF_DQT_T *prQTblInfo);
extern INT32 i4HwJdecSetCurrUsedQuantTable(UINT32 u4HwId,JDEC_JFIF_SOF_T *prPicInfo,JDEC_JFIF_SOS_T *prScanInfo);
extern INT32 i4HwJdecSetPitchPerRow(UINT32 u4HwId,UINT32 u4Comp0PitchPerRow,UINT32 u4Comp1PitchPerRow,UINT32 u4Comp2PitchPerRow,BOOL fgCoeffPitch, BOOL fgLastScan);
extern INT32 i4HwJdecSetCoefPitchPerRow(UINT32 u4HwId,JDEC_JFIF_SOF_T *prPicInfo);
extern INT32 i4HwJdecSetDcAcNeededList(UINT32 u4HwId,JDEC_SOF_COMP_T *prSofComp,JDEC_JFIF_SOS_T *prScanInfo);
extern INT32 i4HwJdecSetBlockCount(UINT32 u4HwId,JDEC_SOF_COMP_T *prSofComp,JDEC_JFIF_SOS_T *prScanInfo);
extern INT32 i4HwJdecSetEOBRUN(UINT32 u4HwId,UINT32 u4EOBRUN);
extern UINT32 u4HwJdecGetEOBRUN(UINT32 u4HwId);
extern INT32 i4HwJdecProcRestart(UINT32 u4HwId);
extern void vHwJdecSetOutputBank0BufAddr(UINT32 u4HwId,UINT32 u4Comp0Addr,UINT32 u4Comp1Addr,UINT32 u4Comp2Addr);
extern void vHwJdecSetOutputBank1BufAddr(UINT32 u4HwId,UINT32 u4Comp0Addr,UINT32 u4Comp1Addr,UINT32 u4Comp2Addr);
extern void vHwJdecSetMCUWidth(UINT32 u4HwId,UINT32 u4Comp0MCUWidth,UINT32 u4Comp1MCUWidth,UINT32 u4Comp2MCUWidth);
extern void vHwJdecSetPictureSize(UINT32 u4HwId,JDEC_JFIF_SOF_T *prPicInfo);
extern void vHwJdecSetRestartInterval(UINT32 u4HwId,UINT32 u4RestartInterval);
extern void vHwJdecSetDecodeMode(UINT32 u4HwId,JDEC_HAL_DEC_MODE_T eDecodeMode);
extern void vHwJdecDecodePicture(UINT32 u4HwId);
extern BOOL fgHwJdecIsDecodePictureFinish(UINT32 u4HwId);
extern void vHwJdecDecodeMCURow(UINT32 u4HwId,BOOL fgFirstRow);
extern BOOL fgHwJdecIsDecodeMCURowFinish(UINT32 u4HwId);
extern INT32 i4HwJdecGetHwStatus(UINT32 u4HwId,JDEC_HAL_HW_STATUS_T *prHwStatus);
extern INT32 i4HwJdecSetHwStatus(UINT32 u4HwId,JDEC_HAL_HW_STATUS_T *prHwStatus);


#define D_HUFF_LOOKAHEAD_BITS	6	/* # of bits of lookahead */
typedef struct
{
	/* Basic tables: (element [0] of each array is unused) */
	INT32	ai4Maxcode[18];
		/*
			largest code of length k (-1 if none)
			(maxcode[17] is a sentinel to ensure jpeg_huff_decode terminates)
		*/
	INT32	ai4Valoffset[17];
		/*
			huffval[] offset for codes of length k
			valoffset[k] = huffval[] index of 1st symbol of code length k,
			less the smallest code of length k; so given a code of length k,
			thecorresponding symbol is huffval[code + valoffset[k]]
		*/
	INT32	ai4LookNbits[1 << D_HUFF_LOOKAHEAD_BITS];
		/* # bits, 0 if too long */
	UINT8	au1LookSym[1 << D_HUFF_LOOKAHEAD_BITS];
		/* symbol, or unused */
} JDEC_HAL_DERIVED_HUFF_TABLE_T;


#endif
