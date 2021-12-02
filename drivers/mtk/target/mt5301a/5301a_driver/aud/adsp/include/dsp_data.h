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
 * $RCSfile: dsp_data.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dsp_data.h
 *  Brief of file dsp_data.h.
 *  Details of file dsp_data.h (optional).
 */

#ifndef DSP_DATA_H
#define DSP_DATA_H

#include "dsp_common.h"

/***************   DRAM LAYOUT OF ADSP (916k Max)  ****************/
/**                          SHARE INFO       2k                                                  **/
/**                          CODE             129k                                                  **/
/**                          DATA             627k                                                   **/
/**                          AFIFO             150k                                                  **/
/**                          TOTAL            908k                                                   **/
/****************************************************************/
#ifndef CC_MT5391_AUD_SUPPORT 
    #define DSP_ROM_SZ      (0x0800*2)  // 4K for mt536x
#else
    #define DSP_ROM_SZ      (0x0E00*2)  // 5K for mt539x
#endif	 	
    #define DSP_POS_RAM_SZ  (0x2000*2)  // 16K
    #define DSP_CMM_RAM_SZ  (0x4000*2)  //32k
    #define DSP_DEC2_RAM_SZ (0x3A00*2)  // 29K
    #define DSP_MIX_RAM_SZ  (0x2000*2)  // 16K
    #define DSP_DEC1_RAM_SZ (0x4000*2)  // 32K
    #define DSP_DEC3_RAM_SZ (0x0)       //(0x1A00*2)  // 13K

     //CODE size = 4k+16K+32K+29k+16k+32k =  129K  
/*
 * Data Size
 */
#ifndef SIF_ADC 
// For 2ch output, max size of DEC1 is 145K bytes (MPEG) (no AAC and WMA)
    //sunman_0x3d000
    #define DSP_DAT_DEC1_SZ  (0x12700*4) // (121kbytes) //0x7900 to 0x12700 by xiaoyi 100319
#else  
    #define DSP_DAT_DEC1_SZ  (0x2FF900) //3144000 bytes 
#endif
		
    #define DSP_DAT_DEC2_SZ  (0x08000*4) // 128 Kbytes,   ( 478kbytes for jpeg  )
    #define DSP_DAT_MIX_SZ    (0x2000*4) // 32K for mixsound
    #define DSP_DAT_CMM_SZ   (0x15a00*4) // 346K for com
		
    #define DSP_CODE_SZ      (DSP_ROM_SZ + DSP_POS_RAM_SZ + DSP_CMM_RAM_SZ + DSP_DEC2_RAM_SZ + DSP_MIX_RAM_SZ + DSP_DEC1_RAM_SZ+ DSP_DEC3_RAM_SZ)
    #define DSP_DATA_SZ      (DSP_DAT_CMM_SZ + DSP_DAT_DEC1_SZ+DSP_DAT_DEC2_SZ+DSP_DAT_MIX_SZ)

    //DATA size= 121 + 128 + 32 + 346 =627k

/*
 * ADSP RISC shared information start address
 */
#define DSP_SHARED_INFO_SA  (0) // offset
#define DSP_SHARED_INF0_SZ  (64*32) // 2K
/*
 * Instruction cache start address
 */
 //#ifndef CC_MT5391_AUD_SUPPORT
#if 1 //added by ling , wait for Nilson
#define DSP_AREA_IC_0_SA    (DSP_SHARED_INFO_SA+DSP_SHARED_INF0_SZ)
//#ifndef CC_MT5391_AUD_3_DECODER
#if 0
    // gallen 0708, 
#define DSP_AREA_IC_1_SA    (DSP_AREA_IC_0_SA + DSP_ROM_SZ)
#else
#define DSP_AREA_IC_1_SA    (DSP_AREA_IC_0_SA + DSP_ROM_SZ + DSP_DEC3_RAM_SZ)
// note that (rom.prg + dec3.pra) is put together at SBLK0 in MT5391/5360
#endif
#define DSP_AREA_IC_2_SA    (DSP_AREA_IC_1_SA + DSP_POS_RAM_SZ )
#define DSP_AREA_IC_3_SA    (DSP_AREA_IC_2_SA + DSP_CMM_RAM_SZ )
#define DSP_AREA_IC_4_SA    (DSP_AREA_IC_3_SA + DSP_DEC2_RAM_SZ )
#define DSP_AREA_IC_5_SA    (DSP_AREA_IC_4_SA + DSP_MIX_RAM_SZ )
#else
#define DSP_AREA_IC_0_SA    (DSP_SHARED_INFO_SA+DSP_SHARED_INF0_SZ)
#define DSP_AREA_IC_1_SA    (DSP_AREA_IC_0_SA + DSP_ROM_SZ)
#define DSP_AREA_IC_2_SA    (DSP_AREA_IC_1_SA + DSP_DEC3_RAM_SZ )
#define DSP_AREA_IC_3_SA    (DSP_AREA_IC_2_SA + DSP_POS_RAM_SZ )
#define DSP_AREA_IC_4_SA    (DSP_AREA_IC_3_SA + DSP_CMM_RAM_SZ )
#define DSP_AREA_IC_5_SA    (DSP_AREA_IC_4_SA + DSP_DEC2_RAM_SZ )
#define DSP_AREA_IC_6_SA    (DSP_AREA_IC_5_SA + DSP_MIX_RAM_SZ )
#endif

/*
 * Data area start address
 */
#define DSP_DATA_AREA       (DSP_AREA_IC_0_SA + DSP_CODE_SZ)
#define DSP_AREA_CMPT_0_SA  (DSP_DATA_AREA)                             //dec1  
#define DSP_AREA_CMPT_1_SA  (DSP_AREA_CMPT_0_SA + DSP_DAT_DEC1_SZ)      //dec2 
#define DSP_AREA_CMPT_2_SA  (DSP_AREA_CMPT_1_SA + DSP_DAT_DEC2_SZ)      //mixsound   
#define DSP_AREA_CMPT_3_SA  (DSP_AREA_CMPT_2_SA + DSP_DAT_MIX_SZ)       //common

/*
 * Audio FIFO start address
 */
#define DSP_AFIFO_SA        (DSP_DATA_AREA+DSP_DATA_SZ)
#define DSP_AFIFO_SZ        (288*1024) // 84 K -> 60k   (84K --> 290 K for MM request)(288 to be multiple of 3)

#define SPDIFIN_FIFO_SA     (DSP_AFIFO_SA)
#define SPDIFIN_FIFO_SZ     (DSP_AFIFO_SZ*3/4)
#define SPDIFIN_AFIFO_SA    (SPDIFIN_FIFO_SA+SPDIFIN_FIFO_SZ)
#define SPDIFIN_AFIFO_SZ    (DSP_AFIFO_SZ/4)
/*
 * Audio FIFO start address for secondary decoder
 */
#define DSP_AFIFO_SA_DEC2     (DSP_AFIFO_SA+DSP_AFIFO_SZ)

#ifdef __MODEL_slt__
// Use audio fifo 2 as SLT line in buffer
#define DSP_AFIFO_SZ_DEC2     (0) //(12*80*1024)    // 960KB
#else
//    #ifdef CC_AUD_DVBT_SUPPORT  //MT5387Merge ??
    #define DSP_AFIFO_SZ_DEC2     (0*1024)      //Audio Description
//    #else
//    #define DSP_AFIFO_SZ_DEC2     (0*1024)      
//    #endif 
#endif
#define SPDIFIN_FIFO_SA_DEC2  (DSP_AFIFO_SA_DEC2)
#define SPDIFIN_FIFO_SZ_DEC2  (DSP_AFIFO_SZ_DEC2*3/4)
#define SPDIFIN_AFIFO_SA_DEC2 (SPDIFIN_FIFO_SA_DEC2+SPDIFIN_FIFO_SZ_DEC2)
#define SPDIFIN_AFIFO_SZ_DEC2 (DSP_AFIFO_SZ_DEC2/4)
/*
 * DSP putbits buffer (for RISC config)
 */
#define DSP_PUTBITS_SA        (DSP_AFIFO_SA_DEC2+DSP_AFIFO_SZ_DEC2)
#ifndef DSP_TEST_PUTBITS
#define DSP_PUTBITS_SZ        (0) // 0K
#else
#define DSP_PUTBITS_SZ        (0) // 0K
#endif
/*
 * Mix sound input buffer
 */
//#define DSP_MIXSNDBUF_OFFSET  (0x100*4)                                  // reserved for mixsound in mainroutine
#define DSP_MIXSNDBUF_OFFSET  (0)                                  // reserved for mixsound in mainroutine
#define DSP_MIXSNDBUF_SA      (DSP_AREA_CMPT_2_SA+DSP_MIXSNDBUF_OFFSET)  // Mixsound input buff start addr
//#define DSP_MIXSNDBUF_SZ      (0x400000-DSP_MIXSNDBUF_OFFSET)            // 4M Byte (cmpt limit) - reserved size
#define DSP_MIXSNDBUF_SZ      (0x9000*4) // 144 Kbytes, by gallen, for decoder 3

// The Size and Start Address are define in UINT32 address
// If SA = X, Real SA in Byte is 4X, from first byte of header
// If SZ = Y, Real SZ in Byte is 4Y

// ======================================================================
// Header File Format
// Item                  Item Size: 4 Byte
/*
MT5351                   0,1
Comm\CommRamCode.pra     2,3
MixSnd\CommMixSnd.pra    4,5
Dec2\CommDec2.pra        6,7
Post\pl2.pra             8,9
Post\vsurr.pra           10,11
Tbl\pl2_tbl.bin          12,13
Tbl\1389_mp2.data        14,15
Tbl\1389_mp3.data        16,17
Tbl\ac3_tbl.bin          18,19
Tbl\wma_cmpt.bin         20,21
Tbl\wma_norm.bin         22,23
Tbl\aac_dram.data        24,25
Dec1\mp2dec1.pra         26,27
Dec1\mp3dec1.pra         28,29
Dec1\ac3dec1.pra         30,31
Dec1\pcmdec1.pra         32,33
Dec1\wmadec1.pra         34,35
Dec1\aacdec1.pra         36,37
Dec1\vorbisdec1.pra      38,39
Dec1\pink_noise.pra      40,41
Dec2\ac3dec2.pra         42,43
Dec2\mp2dec2.pra         44,45
Dec2\aacdec2.pra         46,47
Dec2\pcmdec2.pra         48,49
Tbl\vorbis_cmpt.bin      50,51
Tbl\vorbis_norm.bin      52,53
Comm\ROMCode.pra         54,55
Post\postvect.pra        56,57

Ain\AinIntRam.pra        58,59
Tbl\ntscdec.data         60,61
Tbl\A2.data              62,63
Tbl\PAL.data             64,65
Tbl\fmfmdec.data         66,67
Tbl\Detector.data        68,69
Tbl\MinerTbl.data        70,71
Dec1\NTSCDec1.pra        72,73
Dec1\A2Dec1.pra          74,75
Dec1\PALDec1.pra         76,77
Dec1\fmfmdec1.pra        78,79
Dec1\DetectorDec1.pra    80,81
Dec1\MinerDec1.pra       82,83
Dec1\NTSCDec2.pra        84,85
Dec1\A2Dec1.pra          86,87
Dec1\PALDec2.pra         88,89
Dec1\fmfmdec2.pra        90,91
Dec1\DetectorDec2.pra    92,93
Dec1\MinerDec2.pra       94,95
ROM\DROM_V700000.data    96,97
Tbl\srs_wow_tbl.bin      98,99
Post\SRS_WOW.pra         100,101
Tbl\srs_tsxt_tbl.bin     102,103
Post\SRS_TSXT.pra        104,105
ROM\PROM_V700000.data    106,107
Ain\AinIntRam_Miner.pra    108,109
Post\bbe_viv4.pra    110,111
Dec3\pcmdec3.pra   112,113
Dec3\mp2dec3.pra   114,115
Dec1\lpcmdec1.pra        116,117  
Tbl\lpcm_tbl.bin        118.119
Dec2\aacdec2x.pra        120.121
Post\ddco.pra        122.123
Tbl\ddco_tbl.bin        124.125
Ain\mp2dec3vct.pra         126,127
Ain\ac3dec2ext0.pra        128.129
Tbl\dts_downmix_tab.data    130, 131
Ain\aacdec2ext0.pra             132, 133
Ain\aacdec2ext1.pra             134, 135
Tbl\aac_dram.bin                  136, 137
Critical\critical.pra                138, 139
Dec2\tmpain.pra                   140, 141
Comm\ac3dec2ext.pra          142, 143
Ain\ac3dec2ext0.pra             144, 145
Comm\aacdec2ext.pra          146, 147
Post\ddco_2.pra                    148, 149
Dec3\aacdec3.pra                 150, 151
Ain\aacdec3vct.pra               152, 153
Dec3\ac3dec3.pra                 154, 155
Ain\ac3dec3vct.pra               156, 157
Dec1\fmrdodec1.pra             158, 159
Dec1\fmrdodetdec1.pra         160, 161
Tbl\fmrdo.data                      162, 163
*/
// ======================================================================
// Define Item Index
//MT5351                   0,1
#define DSPF_IMAGE_SA               0
#define DSPF_IMAGE_SZ               1
//Comm\CommRamCode.pra     2,3
#define DSPF_COMM_CODE_SA           2
#define DSPF_COMM_CODE_SZ           3
//MixSnd\CommMixSnd.pra    4,5
#define DSPF_MIX_SOUND_SA           4
#define DSPF_MIX_SOUND_SZ           5
//Dec2\CommDec2.pra        6,7
#define DSPF_COMM_DEC2_SA           6
#define DSPF_COMM_DEC2_SZ           7
//Post\pl2.pra             8,9
#define DSPF_PL2_POST_SA            8
#define DSPF_PL2_POST_SZ            9
//Post\vsurr.pra           10,11
#define DSPF_VSURR_POST_SA          10
#define DSPF_VSURR_POST_SZ          11
//Tbl\pl2_tbl.bin          12,13
#define DSPF_PL2_TABLE_SA           12
#define DSPF_PL2_TABLE_SZ           13
//Tbl\1389_mp2.data        14,15
#define DSPF_MP2_TABLE_SA           14
#define DSPF_MP2_TABLE_SZ           15
//Tbl\1389_mp3.data        16,17
#define DSPF_MP3_TABLE_SA           16
#define DSPF_MP3_TABLE_SZ           17
//Tbl\ac3_tbl.bin          18,19
#define DSPF_AC3_TABLE_SA           18
#define DSPF_AC3_TABLE_SZ           19
//Tbl\wma_cmpt.bin         20,21
#define DSPF_WMA_C_TABLE_SA         20
#define DSPF_WMA_C_TABLE_SZ         21
//Tbl\wma_norm.bin         22,23
#define DSPF_WMA_N_TABLE_SA         22
#define DSPF_WMA_N_TABLE_SZ         23
//Tbl\aac_dram.data        24,25
#define DSPF_AAC_TABLE_SA           24
#define DSPF_AAC_TABLE_SZ           25
//Dec1\mp2dec1.pra         26,27
#define DSPF_MP2_DEC1_SA            26
#define DSPF_MP2_DEC1_SZ            27
//Dec1\mp3dec1.pra         28,29
#define DSPF_MP3_DEC1_SA            28
#define DSPF_MP3_DEC1_SZ            29
//Dec1\ac3dec1.pra         30,31
#define DSPF_AC3_DEC1_SA            30
#define DSPF_AC3_DEC1_SZ            31
//Dec1\pcmdec1.pra         32,33
#define DSPF_PCM_DEC1_SA            32
#define DSPF_PCM_DEC1_SZ            33
//Dec1\wmadec1.pra         34,35
#define DSPF_WMA_DEC1_SA            34
#define DSPF_WMA_DEC1_SZ            35
//Dec1\aacdec1.pra         36,37
#define DSPF_AAC_DEC1_SA            36
#define DSPF_AAC_DEC1_SZ            37
//Dec1\vorbisdec1.pra      38,39
#define DSPF_VORBIS_DEC1_SA         38
#define DSPF_VORBIS_DEC1_SZ         39
//Dec1\pink_noise.pra      40,41
#define DSPF_PNOISE_SA              40
#define DSPF_PNOISE_SZ              41
//Dec2\ac3dec2.pra         42,43
#define DSPF_AC3_DEC2_SA            42
#define DSPF_AC3_DEC2_SZ            43
//Dec2\mp2dec2.pra         44,45
#define DSPF_MP2_DEC2_SA            44
#define DSPF_MP2_DEC2_SZ            45
//Dec2\aacdec2.pra         46,47
#define DSPF_AAC_DEC2_SA            46
#define DSPF_AAC_DEC2_SZ            47
//Dec2\pcmdec2.pra         48,49
#define DSPF_PCM_DEC2_SA            48
#define DSPF_PCM_DEC2_SZ            49
//Tbl\vorbis_cmpt.bin      50,51
#define DSPF_VORBIS_C_TABLE_SA      50
#define DSPF_VORBIS_C_TABLE_SZ      51
//Tbl\vorbis_norm.bin      52,53
#define DSPF_VORBIS_N_TABLE_SA      52
#define DSPF_VORBIS_N_TABLE_SZ      53
//Comm\ROMCode.pra         54,55
#define DSPF_ROM_CODE_SA            54
#define DSPF_ROM_CODE_SZ            55
//Post\postvect.pra        56,57
#define DSPF_POST_VECT_SA           56
#define DSPF_POST_VECT_SZ           57
//Ain\AinIntRam.pra        58,59
#define DSPF_AIN_INTRAM_SA          58
#define DSPF_AIN_INTRAM_SZ          59
//Tbl\ntscdec.data         60,61
#define DSPF_NTSCTBL_SA             60
#define DSPF_NTSCTBL_SZ             61
//Tbl\A2.data              62,63
#define DSPF_A2TBL_SA               62
#define DSPF_A2TBL_SZ               63
//Tbl\PAL.data             64,65
#define DSPF_PALTBL_SA              64
#define DSPF_PALTBL_SZ              65
//Tbl\fmfmdec.data         66,67
#define DSPF_FMFMTBL_SA             66
#define DSPF_FMFMTBL_SZ             67
//Tbl\Detector.data        68,69
#define DSPF_DETECTORTBL_SA         68
#define DSPF_DETECTORTBL_SZ         69
//Tbl\MinerTbl.data        70,71
#define DSPF_MINERTBL_SA            70
#define DSPF_MINERTBL_SZ            71
//Dec1\NTSCDec1.pra        72,73
#define DSPF_NTSC_DEC1_SA           72
#define DSPF_NTSC_DEC1_SZ           73
//Dec1\A2Dec1.pra          74,75
#define DSPF_A2_DEC1_SA             74
#define DSPF_A2_DEC1_SZ             75
//Dec1\PALDec1.pra         76,77
#define DSPF_PAL_DEC1_SA            76
#define DSPF_PAL_DEC1_SZ            77
//Dec1\fmfmdec1.pra        78,79
#define DSPF_FMFM_DEC1_SA           78
#define DSPF_FMFM_DEC1_SZ           79
//Dec1\DetectorDec1.pra    80,81
#define DSPF_DETECTOR_DEC1_SA       80
#define DSPF_DETECTOR_DEC1_SZ       81
//Dec1\MinerDec1.pra       82,83
#define DSPF_MINER_DEC1_SA          82
#define DSPF_MINER_DEC1_SZ          83
//Dec2\NTSCDec2.pra        84,85
#define DSPF_NTSC_DEC2_SA           84
#define DSPF_NTSC_DEC2_SZ           85
//Dec2\A2Dec2.pra          86,87
#define DSPF_A2_DEC2_SA             86
#define DSPF_A2_DEC2_SZ             87
//Dec2\PALDec2.pra         88,89
#define DSPF_PAL_DEC2_SA            88
#define DSPF_PAL_DEC2_SZ            89
//Dec2\fmfmdec2.pra        90,91
#define DSPF_FMFM_DEC2_SA           90
#define DSPF_FMFM_DEC2_SZ           91
//Dec2\DetectorDec2.pra    92,93
#define DSPF_DETECTOR_DEC2_SA       92
#define DSPF_DETECTOR_DEC2_SZ       93
//Dec2\MinerDec2.pra       94,95
#define DSPF_MINER_DEC2_SA          94
#define DSPF_MINER_DEC2_SZ          95
//ROM\DROM_V700000.data <-- Rice: for FPGA only
#define DSPF_DROM_SA                96
#define DSPF_DROM_SZ                97
//Tbl\srs_wow_tbl.bin
#define DSPF_SRS_WOW_TBL_SA         98          
#define DSPF_SRS_WOW_TBL_SZ         99
//Post\SRS_WOW.pra
#define DSPF_SRS_WOW_SA             100
#define DSPF_SRS_WOW_SZ             101
//Post\SRS_TSHD.pra
#define DSPF_SRS_TSHD_SA            100
#define DSPF_SRS_TSHD_SZ            101

//Tbl\srs_tsxt_tbl.bin
#define DSPF_SRS_TSXT_TBL_SA        102
#define DSPF_SRS_TSXT_TBL_SZ        103
//Post\SRS_TSXT.pra
#define DSPF_SRS_TSXT_SA            104
#define DSPF_SRS_TSXT_SZ            105
//ROM\PROM_V700000.data
#define DSPF_PROM_SA                106
#define DSPF_PROM_SZ                107

//Ain\AinIntRam_Miner.pra
#define DSPF_AIN_INTRAM_MINER_SA          108
#define DSPF_AIN_INTRAM_MINER_SZ          109

//Post\bbe_viv4.pra
#define DSPF_BBE_VIVA4_SA           110
#define DSPF_BBE_VIVA4_SZ           111

#if 1//def CC_MT5391_AUD_SUPPORT
//Dec3\pcmdec3.pra         112,113
#define DSPF_PCM_DEC3_SA            112
#define DSPF_PCM_DEC3_SZ            113

//Dec3\mp2dec3.pra         114,115
#define DSPF_MP2_DEC3_SA            114
#define DSPF_MP2_DEC3_SZ            115

//Dec1\lpcmdec1.pra        116,117  
#define DSPF_LPCM_DEC1_SA           116
#define DSPF_LPCM_DEC1_SZ           117

//Tbl\lpcm_tbl.bin        118.119
#define DSPF_LPCM_TBL_SA           118
#define DSPF_LPCM_TBL_SZ           119

//Dec3\aacdec2x.pra        120.121
#define DSPF_AAC_DEC2X_SA           120
#define DSPF_AAC_DEC2X_SZ           121

//Post\ddco.pra        122.123
#define DSPF_DDCO_SA           122
#define DSPF_DDCO_SZ           123

//Tbl\ddco_tbl.bin        124.125
#define DSPF_DDCO_TBL_SA           124
#define DSPF_DDCO_TBL_SZ           125

//Ain\mp2dec3vct.pra         126,127
#define DSPF_VCT_MP2DEC3_SA	126
#define DSPF_VCT_MP2DEC3_SZ	127

//Ain\ac3dec2ext1.pra        128.129
#define DSPF_AC3_DEC2EXT1_SA	128
#define DSPF_AC3_DEC2EXT1_SZ	129

//+Tbl\dts_downmix_tab.data    130, 131
#define DSPF_DOWNMIX_TBL_SA         130         
#define DSPF_DOWNMIX_TBL_SZ         131

//Ain\aacdec2ext0.pra    	132, 133
#define DSPF_AAC_DEC2EXT0_SA         132         
#define DSPF_AAC_DEC2EXT0_SZ         133

//Ain\aacdec2ext1.pra    	134, 135
#define DSPF_AAC_DEC2EXT1_SA         134         
#define DSPF_AAC_DEC2EXT1_SZ         135

//Tbl\aac_dram.bin    		136, 137
#define DSPF_AAC_CMPT_TABLE_SA       136
#define DSPF_AAC_CMPT_TABLE_SZ       137

//Critical\critical.pra    		138, 139
#define DSPF_CRITICAL_CODE_SA       138
#define DSPF_CRITICAL_CODE_SZ       139

#ifdef CC_MT5360B_AUD_FIX_PROM
//Dec2\TmpAin.pra    		140, 414
#define DSPF_TEMP_AIN_CODE_SA       140
#define DSPF_TEMP_AIN_CODE_SZ       141
#endif

//Comm\ac3dec2ext.pra    		142, 143
#define DSPF_AC3_DEC2EXT_SA        142
#define DSPF_AC3_DEC2EXT_SZ        143

//Ain\ac3dec2ext0.pra    		144, 145
#define DSPF_AC3_DEC2EXT0_SA        144
#define DSPF_AC3_DEC2EXT0_SZ        145

//Comm\aacdec2ext.pra    		146, 147
#define DSPF_AAC_DEC2EXT_SA        146
#define DSPF_AAC_DEC2EXT_SZ        147

//Post\ddco.pra        148,149
#define DSPF_DDCO_SA_2           148
#define DSPF_DDCO_SZ_2           149

//Dec3\aacdec3.pra         150,151
#define DSPF_AAC_DEC3_SA            150
#define DSPF_AAC_DEC3_SZ            151

//Ain\aacdec3vct.pra         152,153
#define DSPF_VCT_AACDEC3_SA	        152
#define DSPF_VCT_AACDEC3_SZ	        153

//Dec3\ac3dec3.pra         154,155
#define DSPF_AC3_DEC3_SA            154
#define DSPF_AC3_DEC3_SZ            155

//Ain\ac3dec3vct.pra         156,157
#define DSPF_VCT_AC3DEC3_SA	        156
#define DSPF_VCT_AC3DEC3_SZ	        157

//Dec1\fmrdodec1.pra       158, 159
#define DSPF_FMRDODEC1_SA                158
#define DSPF_FMRDODEC1_SZ                159

//Dec1\fmrdodetdec1.pra  160, 161
#define DSPF_FMRDODETDEC1_SA                160
#define DSPF_FMRDODETDEC1_SZ                161

//Tbl\FMRadio.data             162,163
#define DSPF_FMRDOTBL_SA                       162
#define DSPF_FMRDOTBL_SZ                       163

//Dec1\sbcdec1.data             164,165
#define DSPF_SBCDEC1_SA                       164
#define DSPF_SBCDEC1_SZ                       165

//Tbl\sbctbl.bin                    166,167
#define DSPF_SBC_TABLE_SA                     166
#define DSPF_SBC_TABLE_SZ                     167

//Dec2\jpegdec2.pra                 168,169
#define DSPF_JPEG_DEC2_SA               168     ///[Joel] 61*2
#define DSPF_JPEG_DEC2_SZ               169     ///ADSP_JPEG_DEC2

//Tbl\jpeg_tbl.bin                    170,171
#define DSPF_JPEG_TBL_SA                170     ///[Joel] 62*2
#define DSPF_JPEG_TBL_SZ                171

//Tbl\atv_cos_tab.data          172, 173
#define DSPF_ATV_COSTBL_SA                   172 
#define DSPF_ATV_COSTBL_SZ                 173

#endif //CC_MT5391_AUD_SUPPORT


//DSPF_NICAM_DEC1_SA
//DSPF_TTXACC_DEC1_SA

#endif  // DSP_DATA_H

