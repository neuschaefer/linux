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
 * $Author: kai.wang $
 * $Date: 2012/06/28 $
 * $RCSfile: dsp_data.h,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file dsp_data.h
 *  Brief of file dsp_data.h.
 *  Details of file dsp_data.h (optional).
 */

#ifdef AUD_EXTERN_FILE
#error "Please don't include this header file in release C file"
#endif

#ifndef DSP_DATA_H
#define DSP_DATA_H

#include "dsp_common.h"

/**********************************************************************/
/***************        DRAM layout of ADSP            ****************/
/**********************************************************************/
#define DSP_ROM_SZ              (0x1000*2)  // 8K for mt5365
#define DSP_POS_RAM_SZ          (0x1800*2)  // 12K
#define DSP_CTR_RAM_SZ          (0x0800*2)  // 4k
#define DSP_CMM_RAM_SZ          (0x3800*2)  // 28k
#define DSP_DEC4_RAM_SZ         (0x0800*2)  // 4K
#define DSP_DEC1_RAM_SZ         (0x3000*2)  // 24K
#define DSP_DEC3_RAM_SZ         (0x2000*2)  // 16K
#if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
#define DSP_DEC2_RAM_SZ         (0x14000*2)  // 32K
#else
#define DSP_DEC2_RAM_SZ         (0x4000*2)  // 32K
#endif

#define DSP_ROM_PG_OFST          (0)
#define DSP_POS_PG_OFST          (DSP_ROM_PG_OFST + (DSP_ROM_SZ/2))
#define DSP_CTR_PG_OFST         (DSP_POS_PG_OFST + (DSP_POS_RAM_SZ/2))
#define DSP_CMM_PG_OFST         (DSP_CTR_PG_OFST + (DSP_CTR_RAM_SZ/2))
#define DSP_DEC4_PG_OFST        (DSP_CMM_PG_OFST + (DSP_CMM_RAM_SZ/2))
#define DSP_DEC1_PG_OFST        (DSP_DEC4_PG_OFST + (DSP_DEC4_RAM_SZ/2))
#define DSP_DEC3_PG_OFST        (DSP_DEC1_PG_OFST + (DSP_DEC1_RAM_SZ/2))
#define DSP_DEC2_PG_OFST        (DSP_DEC3_PG_OFST + (DSP_DEC3_RAM_SZ/2))

// Data Size
// For 2ch output, max size of DEC1 is 145K bytes (MPEG) (no AAC and WMA)
// 0x18000*4: for PCM delay
#define DSP_DAT_DEC1_SZ         (0x12700*4+0x18000*4) // 195 Kbytes, E-AC-3 decoder with converter
#define DSP_DAT_DEC2_SZ         (0x12700*4)         // 195 Kbytes, E-AC-3 decoder with converter
#define DSP_DAT_DEC3_SZ         (0x12700*4)       // 144 Kbytes, by gallen, for decoder 3
#define DSP_DAT_CMM_NORMAL_SZ   (0x8000*4)          // Normal common DRAM
#if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
#if defined(CC_AUD_DSP_CHINA_MODEL) || defined(CC_AUD_DSP_US_MODEL)
#define DSP_DAT_CMM_SZ          (0x42800*4)
#else
#define DSP_DAT_CMM_SZ          (0x62900*4)
#endif
#else
#define DSP_DAT_CMM_SZ          (0x49600*4)
#endif
#define DSP_DAT_DEC4_SZ         (0x200*4)         // 2 Kbytes, for decoder 4

#define DSP_CODE_SZ             (DSP_ROM_SZ + DSP_POS_RAM_SZ + DSP_CTR_RAM_SZ + DSP_CMM_RAM_SZ + DSP_DEC4_RAM_SZ + DSP_DEC1_RAM_SZ + DSP_DEC3_RAM_SZ + DSP_DEC2_RAM_SZ )
#define DSP_DATA_SZ             (DSP_DAT_CMM_NORMAL_SZ + DSP_DAT_CMM_SZ + DSP_DAT_DEC1_SZ+DSP_DAT_DEC2_SZ+DSP_DAT_DEC3_SZ + DSP_DAT_DEC4_SZ)

//ADSP RISC shared information start address
#define DSP_SHARED_INFO_SA      (0) // offset
#define DSP_SHARED_INF0_SZ      (64*32) // 2K

//Instruction cache start address
#define DSP_AREA_IC_0_SA        (DSP_SHARED_INFO_SA+DSP_SHARED_INF0_SZ) //ROM
#define DSP_AREA_IC_1_SA        (DSP_AREA_IC_0_SA + DSP_ROM_SZ)         //POS
#define DSP_AREA_IC_2_SA        (DSP_AREA_IC_1_SA + DSP_POS_RAM_SZ)     //CRITICAL
#define DSP_AREA_IC_3_SA        (DSP_AREA_IC_2_SA + DSP_CTR_RAM_SZ)     //Common
#define DSP_AREA_IC_4_SA        (DSP_AREA_IC_3_SA + DSP_CMM_RAM_SZ)     //DEC4
#define DSP_AREA_IC_5_SA        (DSP_AREA_IC_4_SA + DSP_DEC4_RAM_SZ)    //DEC1
#define DSP_AREA_IC_6_SA        (DSP_AREA_IC_5_SA + DSP_DEC1_RAM_SZ)    //DEC3
#define DSP_AREA_IC_7_SA        (DSP_AREA_IC_6_SA + DSP_DEC3_RAM_SZ)    //DEC2

//Data area start address
#define DSP_DATA_AREA           (DSP_AREA_IC_0_SA + DSP_CODE_SZ)
#define DSP_AREA_CMPT_0_SA      (DSP_DATA_AREA)
#define DSP_AREA_CMPT_1_SA      (DSP_AREA_CMPT_0_SA + DSP_DAT_DEC1_SZ)
#define DSP_AREA_CMPT_2_SA      (DSP_AREA_CMPT_1_SA + DSP_DAT_DEC2_SZ)
#define DSP_AREA_DRAM_3_SA      (DSP_AREA_CMPT_2_SA + DSP_DAT_DEC3_SZ)
#define DSP_AREA_CMPT_3_SA      (DSP_AREA_DRAM_3_SA + DSP_DAT_CMM_NORMAL_SZ)
#define DSP_AREA_CMPT_4_SA      (DSP_AREA_CMPT_3_SA + DSP_DAT_CMM_SZ)
#define DSP_AREA_CMPT_5_SA      (DSP_AREA_CMPT_4_SA + DSP_DAT_DEC4_SZ)

//Audio FIFO start address
#define DSP_AFIFO_SA            (DSP_DATA_AREA+DSP_DATA_SZ)
#define DSP_AFIFO_SZ            (288*1024) // 84 K,  (84K --> 290 K for MM request)(288 to be multiple of 3)
#define SPDIFIN_FIFO_SA         (DSP_AFIFO_SA)
#define SPDIFIN_FIFO_SZ         (DSP_AFIFO_SZ*3/4)
#define SPDIFIN_AFIFO_SA        (SPDIFIN_FIFO_SA+SPDIFIN_FIFO_SZ)
#define SPDIFIN_AFIFO_SZ        (DSP_AFIFO_SZ/4)

//Audio FIFO start address for secondary decoder
#define DSP_AFIFO_SA_DEC2       (DSP_AFIFO_SA+DSP_AFIFO_SZ)

#ifdef __MODEL_slt__
// Use audio fifo 2 as SLT line in buffer
#define DSP_AFIFO_SZ_DEC2       (12*80*1024)    // 960KB
#else
#ifdef CC_AUD_KARAOKE_SUPPORT
// The max usage of AFIFO is for DDP. Because it must support very high bitrate up to 3024kbps on DTV.
#define DSP_AFIFO_SZ_DEC2       (288*1024)      // for MM LPCM
#else
#define DSP_AFIFO_SZ_DEC2       (135*1024)
#endif
#endif
#define SPDIFIN_FIFO_SA_DEC2    (DSP_AFIFO_SA_DEC2)
#define SPDIFIN_FIFO_SZ_DEC2    (DSP_AFIFO_SZ_DEC2*3/4)
#define SPDIFIN_AFIFO_SA_DEC2   (SPDIFIN_FIFO_SA_DEC2+SPDIFIN_FIFO_SZ_DEC2)
#define SPDIFIN_AFIFO_SZ_DEC2   (DSP_AFIFO_SZ_DEC2/4)

//Audio FIFO start address for third decoder
#define DSP_AFIFO_SA_DEC3       (DSP_AFIFO_SA_DEC2+DSP_AFIFO_SZ_DEC2)
#ifdef CC_AUD_SKYPE_SUPPORT
#define DSP_AFIFO_SZ_DEC3       (190*1024)
#else
#define DSP_AFIFO_SZ_DEC3       (39*1024)
#endif

//Audio FIFO start address for 4th decoder
#define DSP_AFIFO_SA_DEC4       (DSP_AFIFO_SA_DEC3+DSP_AFIFO_SZ_DEC3)
#if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
#if defined(CC_AUD_DSP_CHINA_MODEL) || defined(CC_AUD_DSP_US_MODEL)
#define DSP_AFIFO_SZ_DEC4       (0*1024)
#else
#define DSP_AFIFO_SZ_DEC4       (16*1024)
#endif
#else
#define DSP_AFIFO_SZ_DEC4       (0*1024)
#endif


//DSP putbits buffer (for RISC config)
#define DSP_PUTBITS_SA          (DSP_AFIFO_SA_DEC4+DSP_AFIFO_SZ_DEC4)
#ifndef DSP_TEST_PUTBITS
#define DSP_PUTBITS_SZ          (0) // 0K
#else
#define DSP_PUTBITS_SZ          (0) // 0K
#endif

#define DSP_AFIFO_HDMI_PARSER_SA    (DSP_PUTBITS_SA+DSP_PUTBITS_SZ)
#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
#define DSP_AFIFO_HDMI_PARSER_SZ    (384*1024)
#else
#define DSP_AFIFO_HDMI_PARSER_SZ    (0*1024)
#endif

//Mix sound input buffer
#define DSP_MIXSNDBUF_SA        (DSP_AFIFO_HDMI_PARSER_SA+DSP_AFIFO_HDMI_PARSER_SZ)            // for Mixsound
#ifdef CC_AUD_MIXSOUND_SUPPORT
#define DSP_MIXSNDBUF_SZ        (64*1024)   //for UI button sound
#define DSP_MIXSNDBUF_SZ2       (72*1024)   //for mixsnd clip (8+1(alsa))
#define DSP_MIXSNDBUF_TMP_SZ2   (8*1024)    //for mixsnd clip SRC buffer
#define DSP_MIXSNDBUF_SZ3       (64*1024)   //for ALSA share memory
#else
#define DSP_MIXSNDBUF_SZ        (0*1024)
#define DSP_MIXSNDBUF_SZ2       (0*1024)
#define DSP_MIXSNDBUF_TMP_SZ2   (0*1024)
#define DSP_MIXSNDBUF_SZ3       (0*1024)
#endif
#define DSP_MIXSNDBUF_ALL_SZ    (DSP_MIXSNDBUF_SZ+DSP_MIXSNDBUF_SZ2+DSP_MIXSNDBUF_TMP_SZ2+DSP_MIXSNDBUF_SZ3)

// The Size and Start Address are define in UINT32 address
// If SA = X, Real SA in Byte is 4X, from first byte of header
// If SZ = Y, Real SZ in Byte is 4Y

// ======================================================================
// Header File Format
// Item                     Item Size: 4 Byte
/*
MT5365                      0,1
Comm\CommRamCode.pra        2,3
Comm\ROMCode.pra            4,5
ROM\DROM_V650000.data       6,7
ROM\PROM_V650000.data       8,9
Ain\AinIntRam.pra           10,11
Ain\AinIntRam_Miner.pra     12,13
Critical\critical.pra       14,15
Dec1\NTSCDec1.pra           16,17
Dec2\NTSCDec2.pra           18,19
Tbl\ntscdec.bin             20,21
Dec1\DetectorDec1.pra       22,23
Dec2\DetectorDec2.pra       24,25
Tbl\Detector.data           26,27
Dec1\A2Dec1.pra             28,29
Dec2\A2Dec2.pra             30,31
Tbl\A2.data                 32,33
Dec1\PALDec1.pra            34,35
Dec2\PALDec2.pra            36,37
Tbl\PAL.data                38,39
Dec1\fmfmdec1.pra           40,41
Dec2\fmfmdec2.pra           42,43
Tbl\fmfmdec.data            44,45
Dec1\MinerDec1.pra          46,47
Dec2\MinerDec2.pra          48,49
Tbl\MinerTbl.data           50,51
Dec1\fmrdodec1.pra          52,53
Dec1\fmrdodetdec1.pra       54,55
Tbl\fmrdo.bin               56,57
Tbl\atv_cos_tab.data        58,59
Dec1\pcmdec1.pra            60,61
Dec2\pcmdec2.pra            62,63
Dec3\pcmdec3.pra            64,65
Dec1\mp2dec_mc.pra          66,67
Dec2\mp2dec2.pra            68,69
Dec3\mp2dec3.pra            70,71
Tbl\mp2tbl.bin              72,73
Dec1\mp3dec1.pra            74,75
Tbl\mp3tbl_5371.bin         76,77
Dec1\ac3dec1.pra            78,79
Dec2\ac3dec2.pra            80,81
Dec3\ac3dec3.pra            82,83
Tbl\ac3_tbl.bin             84,85
Dec1\aacdec1.pra            86,87
Dec2\aacdec2.pra            88,89
Dec3\aacdec3.pra            90,91
Tbl\aac_dram.bin            92,93
Tbl\aac_dram.data           94,95
Dec1\ddco.pra               96,97
Tbl\ddco_tbl.bin            98,99
Dec1\wmadec1.pra            100,101
Tbl\wma_cmpt.bin            102,103
Tbl\wma_norm.bin            104,105
Dec1\lpcmdec1.pra           106,107
Tbl\lpcm_tbl.bin            108,109
Dec1\pink_noise.pra         110,111
Dec1\sbcdec1.pra            112,113
Tbl\sbctbl.bin              114,115
Dec2\mp3enc_dec2.pra        116,117
Tbl\mp3enc_tbl.data         118,119
Dec1\cookdec1.pra           120,121
Tbl\cooktbl.bin             122,123
Post\VectPost.pra           124,125
Post\pl2.pra                126,127
Tbl\pl2_tbl.bin             128,129
Post\vsurr_mtk2.pra         130,131
Post\BBE\bbe_viv4.pra       132,133
Post\SRS.pra                134,135
Tbl\srs_tbl.bin             136,137
Dec4\sbcenc4.pra            138,139
Tbl\sbcenc_tbl.bin          140, 141
Post\adv.pra                142, 143
Tbl\adv_tbl.bin             144, 145
Dec1\G711dec1.pra           146, 147
Dec1\G711enc1.pra           148, 149
ROM\DROM_V65_comm.data      150, 151
ROM\DROM_V65_mts.data       152, 153
ROM\DROM_V65_ac3.data       154, 155
ROM\DROM_V65_mpeg.data      156, 157
ROM\DROM_V65_aac.data       158, 159
Dec1\dtsdec1.pra            160, 161
Tbl\dts_huff_tbl.bin        162, 163
Tbl\dts_adpcm_vq_tbl.bin    164, 165
Tbl\dts_rom_tbl.bin         166, 167
Dec2\mp3dec2.pra            168, 169
Dec2\lpcmdec2.pra           170, 171
Dec1\dra_dec1.bin           172, 173
Tbl\dra_tbl.bin             174, 175
Dec2\dra_dec2.pra           176, 177

*/
// ======================================================================
// Define Item Index
//MT5365                                0,1
#define DSPF_IMAGE_SA                   0
#define DSPF_IMAGE_SZ                   1
//Comm\CommRamCode.pra                  2,3
#define DSPF_COMM_CODE_SA               2
#define DSPF_COMM_CODE_SZ               3
//Comm\ROMCode.pra                      4,5
#define DSPF_ROM_CODE_SA                4
#define DSPF_ROM_CODE_SZ                5
//ROM\DROM_V700000.data                 6,7
#define DSPF_DROM_SA                    6
#define DSPF_DROM_SZ                    7
//ROM\PROM_V700000.data                 8,9
#define DSPF_PROM_SA                    8
#define DSPF_PROM_SZ                    9
//Ain\AinIntRam.pra                     10,11
#define DSPF_AIN_INTRAM_SA              10
#define DSPF_AIN_INTRAM_SZ              11
//Ain\AinIntRam_Miner.pra               12,13
#define DSPF_AIN_INTRAM_MINER_SA        12
#define DSPF_AIN_INTRAM_MINER_SZ        13
//Critical\critical.pra    		        14, 15
#define DSPF_CRITICAL_CODE_SA           14
#define DSPF_CRITICAL_CODE_SZ           15
//Dec1\NTSCDec1.pra                     16,17
#define DSPF_NTSC_DEC1_SA               16
#define DSPF_NTSC_DEC1_SZ               17
//Dec2\NTSCDec2.pra                     18,19
#define DSPF_NTSC_DEC2_SA               18
#define DSPF_NTSC_DEC2_SZ               19
//Tbl\ntscdec.bin                       20,21
#define DSPF_NTSCTBL_SA                 20
#define DSPF_NTSCTBL_SZ                 21
//Dec1\DetectorDec1.pra                 22,23
#define DSPF_DETECTOR_DEC1_SA           22
#define DSPF_DETECTOR_DEC1_SZ           23
//Dec2\DetectorDec2.pra                 24,25
#define DSPF_DETECTOR_DEC2_SA           24
#define DSPF_DETECTOR_DEC2_SZ           25
//Tbl\Detector.data                     26,27
#define DSPF_DETECTORTBL_SA             26
#define DSPF_DETECTORTBL_SZ             27
//Dec1\A2Dec1.pra                       28,29
#define DSPF_A2_DEC1_SA                 28
#define DSPF_A2_DEC1_SZ                 29
//Dec2\A2Dec2.pra                       30,31
#define DSPF_A2_DEC2_SA                 30
#define DSPF_A2_DEC2_SZ                 31
//Tbl\A2.data                           32,33
#define DSPF_A2TBL_SA                   32
#define DSPF_A2TBL_SZ                   33
//Dec1\PALDec1.pra                      34,35
#define DSPF_PAL_DEC1_SA                34
#define DSPF_PAL_DEC1_SZ                35
//Dec2\PALDec2.pra                      36,37
#define DSPF_PAL_DEC2_SA                36
#define DSPF_PAL_DEC2_SZ                37
//Tbl\PAL.data                          38,39
#define DSPF_PALTBL_SA                  38
#define DSPF_PALTBL_SZ                  39
//Dec1\fmfmdec1.pra                     40,41
#define DSPF_FMFM_DEC1_SA               40
#define DSPF_FMFM_DEC1_SZ               41
//Dec2\fmfmdec2.pra                     42,43
#define DSPF_FMFM_DEC2_SA               42
#define DSPF_FMFM_DEC2_SZ               43
//Tbl\fmfmdec.data                      44,45
#define DSPF_FMFMTBL_SA                 44
#define DSPF_FMFMTBL_SZ                 45
//Dec1\MinerDec1.pra                    46,47
#define DSPF_MINER_DEC1_SA              46
#define DSPF_MINER_DEC1_SZ              47
//Dec2\MinerDec2.pra                    48,49
#define DSPF_MINER_DEC2_SA              48
#define DSPF_MINER_DEC2_SZ              49
//Tbl\MinerTbl.data                     50,51
#define DSPF_MINERTBL_SA                50
#define DSPF_MINERTBL_SZ                51
//Dec1\fmrdodec1.pra                    52,53
#define DSPF_FMRDODEC1_SA               52
#define DSPF_FMRDODEC1_SZ               53
//Dec1\fmrdodetdec1.pra                 54,55
#define DSPF_FMRDODETDEC1_SA            54
#define DSPF_FMRDODETDEC1_SZ            55
//Tbl\FMRadio.data                      56,57
#define DSPF_FMRDOTBL_SA                56
#define DSPF_FMRDOTBL_SZ                57
//Tbl\atv_cos_tab.data                  58,59
#define DSPF_ATV_COSTBL_SA              58
#define DSPF_ATV_COSTBL_SZ              59

//Dec1\pcmdec1.pra                      60,61
#define DSPF_PCM_DEC1_SA                60
#define DSPF_PCM_DEC1_SZ                61
//Dec2\pcmdec2.pra                      62,63
#define DSPF_PCM_DEC2_SA                62
#define DSPF_PCM_DEC2_SZ                63
//Dec3\pcmdec3.pra                      64,65
#define DSPF_PCM_DEC3_SA                64
#define DSPF_PCM_DEC3_SZ                65
//Dec1\mp2dec1.pra                      66,67
#define DSPF_MP2_DEC1_SA                66
#define DSPF_MP2_DEC1_SZ                67
//Dec2\mp2dec2.pra                      68,69
#define DSPF_MP2_DEC2_SA                68
#define DSPF_MP2_DEC2_SZ                69
//Dec3\mp2dec3.pra                      70,71
#define DSPF_MP2_DEC3_SA                70
#define DSPF_MP2_DEC3_SZ                71
//Tbl\mp2tbl.bin                        72,73
#define DSPF_MP2_TABLE_SA               72
#define DSPF_MP2_TABLE_SZ               73
//Dec1\mp3dec1.pra                      74,75
#define DSPF_MP3_DEC1_SA                74
#define DSPF_MP3_DEC1_SZ                75
//Tbl\mp3tbl_5371.bin                   76,77
#define DSPF_MP3_TABLE_SA               76
#define DSPF_MP3_TABLE_SZ               77
//Dec1\ac3dec1.pra                      78,79
#define DSPF_AC3_DEC1_SA                78
#define DSPF_AC3_DEC1_SZ                79
//Dec2\ac3dec2.pra                      80,81
#define DSPF_AC3_DEC2_SA                80
#define DSPF_AC3_DEC2_SZ                81
//Dec3\ac3dec3.pra                      82,83
#define DSPF_AC3_DEC3_SA                82
#define DSPF_AC3_DEC3_SZ                83
//Tbl\ac3_tbl.bin                       84,85
#define DSPF_AC3_TABLE_SA               84
#define DSPF_AC3_TABLE_SZ               85
//Dec1\aacdec1.pra                      86,87
#define DSPF_AAC_DEC1_SA                86
#define DSPF_AAC_DEC1_SZ                87
//Dec2\aacdec2.pra                      88,89
#define DSPF_AAC_DEC2_SA                88
#define DSPF_AAC_DEC2_SZ                89
//Dec3\aacdec3.pra                      90,91
#define DSPF_AAC_DEC3_SA                90
#define DSPF_AAC_DEC3_SZ                91
//Tbl\aac_dram.bin    		            92,93
#define DSPF_AAC_CMPT_TABLE_SA          92
#define DSPF_AAC_CMPT_TABLE_SZ          93
//Tbl\aac_dram.data                     94,95
#define DSPF_AAC_TABLE_SA               94
#define DSPF_AAC_TABLE_SZ               95
//Dec1\ddco.pra                         96,97
#define DSPF_DDCO_SA                    96
#define DSPF_DDCO_SZ                    97
//Tbl\ddco_tbl.bin                      98,99
#define DSPF_DDCO_TBL_SA                98
#define DSPF_DDCO_TBL_SZ                99
//Dec1\wmadec1.pra                      100,101
#define DSPF_WMA_DEC1_SA                100
#define DSPF_WMA_DEC1_SZ                101
//Tbl\wma_cmpt.bin                      102,103
#define DSPF_WMA_C_TABLE_SA             102
#define DSPF_WMA_C_TABLE_SZ             103
//Tbl\wma_norm.bin                      104,105
#define DSPF_WMA_N_TABLE_SA             104
#define DSPF_WMA_N_TABLE_SZ             105
//Dec1\lpcmdec1.pra                     106,107
#define DSPF_LPCM_DEC1_SA               106
#define DSPF_LPCM_DEC1_SZ               107
//Tbl\lpcm_tbl.bin                      108.109
#define DSPF_LPCM_TBL_SA                108
#define DSPF_LPCM_TBL_SZ                109
//Dec1\pink_noise.pra                   110,111
#define DSPF_PNOISE_SA                  110
#define DSPF_PNOISE_SZ                  111
//Dec1\sbcdec1.data                     112,113
#define DSPF_SBCDEC1_SA                 112
#define DSPF_SBCDEC1_SZ                 113
//Tbl\sbctbl.bin                        114,115
#define DSPF_SBC_TABLE_SA               114
#define DSPF_SBC_TABLE_SZ               115
//Dec2\mp3enc_dec2.pra                  116,117
#define DSPF_MP3ENC_DEC2_SA	            116		///[Joel]20090311
#define DSPF_MP3ENC_DEC2_SZ	            117
//Tbl\mp3enc_tbl.bin                    118,119
#define DSPF_MP3ENC_TABLE_SA	        118		///[Joel]20090311
#define DSPF_MP3ENC_TABLE_SZ	        119
//Dec1\cookdec1.pra                     120,121
#define DSPF_COOK_DEC1_SA               120
#define DSPF_COOK_DEC1_SZ               121
//Tbl\cooktbl.bin                       122,123
#define DSPF_COOKTBL_SA                 122
#define DSPF_COOKTBL_SZ                 123

//Post\postvect.pra                     124,125
#define DSPF_POST_VECT_SA               124
#define DSPF_POST_VECT_SZ               125
//Post\pl2.pra                          126,127
#define DSPF_PL2_POST_SA                126
#define DSPF_PL2_POST_SZ                127
//Tbl\pl2_tbl.bin                       128,129
#define DSPF_PL2_TABLE_SA               128
#define DSPF_PL2_TABLE_SZ               129
//Post\vsurr.pra                        130,131
#define DSPF_VSURR_POST_SA              130
#define DSPF_VSURR_POST_SZ              131
//Post\bbe_viv4.pra                     132,133
#define DSPF_BBE_VIVA4_SA               132
#define DSPF_BBE_VIVA4_SZ               133
//Post\SRS\srs_viq.pra                          134,135
#define DSPF_VIQ_SA                     134
#define DSPF_VIQ_SZ                     135
//Tbl\srs_viq_tbl.bin                       136,137
#define DSPF_VIQ_TBL_SA                 136
#define DSPF_VIQ_TBL_SZ                 137
//Dec4\sbcenc4.pra                      138,139
#define DSPF_SBCENC4_SA                 138
#define DSPF_SBCENC4_SZ                 139
//tbl\sbcenc_tbl.bin                    140,141
#define DSPF_SBCENC_TABLE_SA	        140
#define DSPF_SBCENC_TABLE_SZ	        141
//#ifdef CC_AUD_DSP_SUPPORT_AUDYSSEY
//Post\adv.pra                          142,143
#define DSPF_ADV_SA                     142
#define DSPF_ADV_SZ                     143
//Tbl\adv_tbl.bin                       144,145
#define DSPF_ADV_TBL_SA                 144
#define DSPF_ADV_TBL_SZ                 145
//#endif
//Dec1\G711dec1.pra                     146,147
#define DSPF_G711DEC1_SA                146
#define DSPF_G711DEC1_SZ                147
//Dec1\G711enc1.pra                     148,149
#define DSPF_G711ENC1_SA                148
#define DSPF_G711ENC1_SZ                149
//ROM\DROM_V65_comm.data                150, 151
#define DSPF_DROM_COMM_SA               150
#define DSPF_DROM_COMM_SZ               151
//ROM\DROM_V65_mts.data                 152, 153
#define DSPF_DROM_MTS_SA                152
#define DSPF_DROM_MTS_SZ                153
//ROM\DROM_V65_ac3.data                 154, 155
#define DSPF_DROM_AC3_SA                154
#define DSPF_DROM_AC3_SZ                155
//ROM\DROM_V65_mpeg.data                156, 157
#define DSPF_DROM_MPEG_SA               156
#define DSPF_DROM_MPEG_SZ               157
//ROM\DROM_V65_aac.data                 158, 159
#define DSPF_DROM_AAC_SA                158
#define DSPF_DROM_AAC_SZ                159
//Dec1\dtsdec1.pra                      160, 161
#define DSPF_DTS_DEC1_SA                160
#define DSPF_DTS_DEC1_SZ                161
//Dec2\dtsdec2.pra                      162, 163
#define DSPF_DTS_DEC2_SA                162
#define DSPF_DTS_DEC2_SZ                163
//+tbl\dts_huff_tbl.bin                 164, 165
#define DSPF_DTS_HUFF_TABLE_SA          164
#define DSPF_DTS_HUFF_TABLE_SZ          165
//+tbl\dts_adpcm_vq_tbl.bin              166, 167
#define DSPF_DTS_ADPCM_VQ_TABLE_SA      166
#define DSPF_DTS_ADPCM_VQ_TABLE_SZ      167
//+tbl\dts_rom_tbl.bin                  168, 169
#define DSPF_DTS_ROM_TABLE_SA           168
#define DSPF_DTS_ROM_TABLE_SZ           169
//Tbl\dts_BC_tbl.bin      170, 171
#define DSPF_DTS_BC_SA                     170
#define DSPF_DTS_BC_SZ                     171


//Dec2\mp3dec2.pra                     172, 173
#define DSPF_MP3_DEC2_SA                172
#define DSPF_MP3_DEC2_SZ                173
//Dec2\lpcmdec2.pra                      174, 175
#define DSPF_LPCM_DEC2_SA               174
#define DSPF_LPCM_DEC2_SZ               175
//Dec1\dra_dec1.bin                     176, 177
#define DSPF_DRA_DEC1_SA                176
#define DSPF_DRA_DEC1_SZ                177
//Tbl\dra_tbl.bin                        178, 179
#define DSPF_DRA_TABLE_SA               178
#define DSPF_DRA_TABLE_SZ               179
//Dec2\dra_dec2.pra                      180, 181
#define DSPF_DRA_DEC2_SA                180
#define DSPF_DRA_DEC2_SZ                181
//Dec3\mp2dec3vct.pra                  182, 183
#define DSPF_VCT_MP2DEC3_SA             182
#define DSPF_VCT_MP2DEC3_SZ             183
//Dec3\aacdec3vct.pra                   184, 185
#define DSPF_VCT_AACDEC3_SA             184
#define DSPF_VCT_AACDEC3_SZ             185
//Dec3\ac3dec3vct.pra                    186,187
#define DSPF_VCT_AC3DEC3_SA             186
#define DSPF_VCT_AC3DEC3_SZ             187
//Dec3\pcmdec3vct.pra                   188,189
#define DSPF_VCT_PCMDEC3_SA             188
#define DSPF_VCT_PCMDEC3_SZ             189
//Dec3\mp3encdec3.pra                   186,187     ///MP3ENC_SUPPORT
//Tbl\mp3enc_tbl.bin                    190,191
#define DSPF_MP3ENC_DEC3_SA             190
#define DSPF_MP3ENC_DEC3_SZ             191
//Dec3\mp3encdec3vct.pra         192,193
#define DSPF_VCT_MP3ENC_DEC3_SA         192
#define DSPF_VCT_MP3ENC_DEC3_SZ         193         ///MP3ENC_SUPPORT
//Dec2\g729dec2.pra                    194,195
#define DSPF_G729DEC2_SA                194
#define DSPF_G729DEC2_SZ                195
//Tbl\g729dec_dram.data                        196,197
#define DSPF_G729DEC_TABLE_SA               196
#define DSPF_G729DEC_TABLE_SZ               197
//Dec1\g729dec1.pra                     198,199
#define DSPF_G729DEC1_SA                198
#define DSPF_G729DEC1_SZ                199
//Post\cdnotch.pra                      200,201
#define DSPF_CDNOTCH_SA					200
#define DSPF_CDNOTCH_SZ					201
//Post\vbass.pra                        202,203
#define DSPF_VBASS_SA					202
#define DSPF_VBASS_SZ					203
//Post\SRS\SRS_TSHDM.pra                204,205
#define DSPF_SRS_TSHD_SA                204
#define DSPF_SRS_TSHD_SZ                205
//Dec3\dtsenc.pra
#define DSPF_DTSENC_SA					206
#define DSPF_DTSENC_SZ					207
//Tbl\ddco_tb3.bin                      208,209
#define DSPF_DDCO_TB3_SA                208
#define DSPF_DDCO_TB3_SZ                209
//Post\spkheight.pra                    210,211
#define DSPF_SPKHEIGHT_SA               210
#define DSPF_SPKHEIGHT_SZ               211
//Tbl\ddco_tb4.bin                      212,213
#define DSPF_DDCO_TB4_SA                212
#define DSPF_DDCO_TB4_SZ                213
//Dec3\ddpconv.pra                      214,215
#define DSPF_DDPCONV_SA                 214
#define DSPF_DDPCONV_SZ                 215
//Dec4\pcmdec4.pra                      216,217
#define DSPF_PCM_DEC4_SA                216
#define DSPF_PCM_DEC4_SZ                217
//Dec1\vorbisdec.pra
#define DSPF_VORBIS_DEC1_SA             218 ///CC_VORBIS_SUPPORT
#define DSPF_VORBIS_DEC1_SZ             219
//Tbl\vorbis_cmpt.bin
#define DSPF_VORBIS_CTBL_SA             220 ///CC_VORBIS_SUPPORT
#define DSPF_VORBIS_CTBL_SZ             221
//Tbl\vorbis_norm.bin
#define DSPF_VORBIS_NTBL_SA             222 ///CC_VORBIS_SUPPORT
#define DSPF_VORBIS_NTBL_SZ             223
//Dec2\vorbisdec2.pra
#define DSPF_VORBIS_DEC2_SA             224 ///CC_VORBIS_SUPPORT
#define DSPF_VORBIS_DEC2_SZ             225
//Tbl\pts_info.data
#define DSPF_PTSINFO_SA             226
#define DSPF_PTSINFO_SZ             227
//Dec3\aacdec2ext.pra
#define DSPF_AAC_DEC2EXT_SA             228
#define DSPF_AAC_DEC2EXT_SZ             229
//Dec1\wmaprodec1.pra
#define DSPF_WMAPRO_DEC1_SA                230
#define DSPF_WMAPRO_DEC1_SZ                231
//Tbl\wmapro_cmpt.bin
#define DSPF_WMAPRO_C_TABLE_SA             232
#define DSPF_WMAPRO_C_TABLE_SZ             233
//Tbl\wmapro_norm.bin
#define DSPF_WMAPRO_N_TABLE_SA             234
#define DSPF_WMAPRO_N_TABLE_SZ             235
//Tbl\ddco_crctab.bin, for test mode only
#define DSPF_DDCO_CRC_TABLE_SA             236
#define DSPF_DDCO_CRC_TABLE_SZ             237
//Post\SRS\srs_wowhd.pra
#define DSPF_SRS_WOWHD_SA                238
#define DSPF_SRS_WOWHD_SZ                239
//Post\keyshift.pra
#define DSPF_KEYSHIFT_SA                240
#define DSPF_KEYSHIFT_SZ                241
//Dec1\amrdec1.pra
#define DSPF_AMRDEC1_SA                 242
#define DSPF_AMRDEC1_SZ                 243
//Tbl\amr_tbl.bin
#define DSPF_AMR_TBL_SA                 244
#define DSPF_AMR_TBL_SZ                 245
//Tbl\amr_mode_tbl.bin
#define DSPF_AMR_MODE_TBL_SA            246
#define DSPF_AMR_MODE_TBL_SZ            247
//Dec1\awbdec1.pra
#define DSPF_AWBDEC1_SA                 248
#define DSPF_AWBDEC1_SZ                 249
//Tbl\awb_tbl.bin
#define DSPF_AWB_TBL_SA                 250
#define DSPF_AWB_TBL_SZ                 251
//Tbl\awb_mode_tbl.bin
#define DSPF_AWB_MODE_TBL_SA            252
#define DSPF_AWB_MODE_TBL_SZ            253
#endif  // DSP_DATA_H

