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
 * $RCSfile: dsp_table.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dsp_table.h
 *  Brief of file dsp_table.h.
 *  Details of file dsp_table.h (optional).
 */

#ifndef DSP_TABLE_H
#define DSP_TABLE_H

//#define SPATIALIZER_CERTIFICATION

// sample frequency defined here
#define SFREQ_16K                   (0x02)
#define SFREQ_32K                   (0x03)
#define SFREQ_8K                    (0x04)
#define SFREQ_11K                   (0x05)
#define SFREQ_12K                   (0x06)
#define SFREQ_22K                   (0x07)
#define SFREQ_44K                   (0x08)
#define SFREQ_88K                   (0x09)
#define SFREQ_176K                  (0x0A)
#define SFREQ_24K                   (0x0C)
#define SFREQ_48K                   (0x0D)
#define SFREQ_96K                   (0x0E)
#define SFREQ_192K                  (0x0F)

#define SAMPLE_32K                  (0)
#define SAMPLE_44K                  (1)
#define SAMPLE_48K                  (2)
#define SAMPLE_96K                  (3)
#define SAMPLE_192K                 (4)


#define DSP_MICEQ_SZ                (9)
#define DSP_BASSM_SZ                (10)
#define DSP_SBASSPIPE_SZ            (5)
#define DSP_SBASSBOOST_SZ           (5)
#define DSP_SBASSCLEAR_SZ           (5)
#define DSP_PROLOGIC_SZ             (28)
#define DSP_VSURR_SZ                (3)
#define DSP_REVERB_SZ               (2)
#define DSP_PL2_SZ                  (32)
#define DSP_N22GAIN_SZ              (7)
#define DSP_N22FREQ_SZ              (16)
#define DSP_N22ANGLE_SZ             (7)
#define DSP_QSOUND_TBLSZ            (37)
#define DSP_TBL_SRS_WOW_FSENTSZ     (110)
#define DSP_TBL_SRS_TSXT_FSENTSZ    (147)
#ifdef SONY_VSS
#define DSP_VSS_SZ                  (61)
extern const UINT32 VSS_TABLE[1][DSP_VSS_SZ];
#endif //SONY_VSS
#define HP_TABLE_OFFSET             (0x4b0)
#define HP_TABLE_COMMON_SIZE        (23)
#define HP_TABLE_FREQ_SIZE          (23)

#define MAX_REVERB_BANK             (79)

#ifdef CC_AUD_EQ_SETA
#define DSP_EQ_BAND_TABLE_SIZE  (((10*2*2)+1)*5)
#else
#define DSP_EQ_BAND_TABLE_SIZE  (((14*2*2)+1)*5)
#endif

#if (0)
#define DSP_EQ_BAND1_SZ             (((10*2*2)+1)*5)
#define DSP_EQ_BAND2_SZ             (((10*2*2)+1)*5)
#define DSP_EQ_BAND3_SZ             (((10*2*2)+1)*5)
#define DSP_EQ_BAND4_SZ             (((10*2*2)+1)*5)
#define DSP_EQ_BAND5_SZ	            (((10*2*2)+1)*5)
#define DSP_EQ_5BAND_SZ             (DSP_EQ_BAND1_SZ + (DSP_EQ_BAND2_SZ*2) + \
	                                (DSP_EQ_BAND3_SZ*2) + (DSP_EQ_BAND4_SZ*2) + DSP_EQ_BAND5_SZ)
#endif

// HPF Table relative define
#define HPF_FC_STR     50
#define HPF_FC_END     300
#define HPF_FC_STEP    10 
#define HPF_FREQ_NUM   (((HPF_FC_END - HPF_FC_STR)/HPF_FC_STEP) + 1)

#define HPF_TABLE_SET_SZ (5)


// *********************************************************************
// Tables
// *********************************************************************
/*
extern const UINT32 BASSM_TABLE[5][10];
*/
extern const UINT32 BASSM_TABLE_70[10];
extern const UINT32 BASSM_TABLE_100[10];
extern const UINT32 BASSM_TABLE_200[10];
extern const UINT32 BASSM_TABLE_300[10];
#ifndef CC_NEW_POST_SUPPORT
extern const UINT32 MTK_TAN_TABLE[512];
#else
extern const UINT32 MTK_TAN_TABLE[8192*2];
#endif
#ifdef CC_NEW_POST_MULTI_VS_SUPPORT
extern const UINT32 MTK_COMB_R_TABLE[70];
extern const UINT32 MTK_COMB_L_TABLE[70];
extern const UINT32 MTK_M2S_ALLPASS_TABLE[20];
#endif

#ifdef CC_AUD_SPEAKER_HEIGHT
extern const UINT32 MTK_SHELF_R_TABLE[15];
extern const UINT32 MTK_SHELF_L_TABLE[15];
extern const UINT32 MTK_ALLPASS1_TABLE[10];
#ifdef CC_AUD_SPEAKER_HEIGHT_MODE
extern const UINT32 MTK_SHELF_2DB_TABLE[5];
extern const UINT32 MTK_SHELF_4DB_TABLE[5];
extern const UINT32 MTK_SHELF_6DB_TABLE[5];
#endif
#endif

extern const UINT32 MTK_SPKCAL_TABLE[40];
extern const UINT32 MTK_VBASS_R_TABLE[70];
extern const UINT32 MTK_VBASS_L_TABLE[70];
extern const UINT32 MTK_VBASS_HIGHPASS_TABLE[20];


#ifdef  BASS_FILTER_CUSTOMIZE
extern const UINT32 BASSM_TABLE_50[10];
extern const UINT32 BASSM_TABLE_60[10];
extern const UINT32 BASSM_TABLE_80[10];
extern const UINT32 BASSM_TABLE_90[10];
extern const UINT32 BASSM_TABLE_110[10];
extern const UINT32 BASSM_TABLE_120[10];
extern const UINT32 BASSM_TABLE_130[10];
extern const UINT32 BASSM_TABLE_140[10];
extern const UINT32 BASSM_TABLE_150[10];
extern const UINT32 BASSM_TABLE_160[10];
extern const UINT32 BASSM_TABLE_170[10];
extern const UINT32 BASSM_TABLE_180[10];
extern const UINT32 BASSM_TABLE_190[10];
#endif //BASS_FILTER_CUSTOMIZE

#if 0
#ifdef DSP_POST_PROCESS_ENABLE

extern const UINT32 DELAY_FACTOR[5];

extern const UINT32 MICEQ_TABLE[5][9];
extern const UINT32 EQ_5BAND_TABLE[5][(41+41*2+41*2+41*2+41)*5];
extern const UINT32 SBASSPIPE_TABLE[5][5];
extern const UINT32 SBASSBOOST_TABLE[5][5];

extern const UINT32 SBASSCLEAR_TABLE[5][5];

extern const UINT32 VSURR_TABLE[5][3];


extern const UINT32 REVERB_TABLE[5][2];
#endif
#endif


/*
const UINT32 BASSM_TABLE[5][10] = {
       {0xFFC1C0B1,0x007E3914,0x0000018F/2,0x0000031D/2,0x0000018F/2,
        0xFFC1C0B1,0x007E3914,0x003F1E19/2,0xFF81C3CF/2,0x003F1E19/2},
       {0xFFC146D2,0x007EB5E3,0x000000D3/2,0x000001A5/2,0x000000D3/2,
        0xFFC146D2,0x007EB5E3,0x003F5BC4/2,0xFF814877/2,0x003F5BC4/2},
       {0xFFC12C83,0x007ED0B5,0x000000B2/2,0x00000164/2,0x000000B2/2,
        0xFFC12C83,0x007ED0B5,0x003F690D/2,0xFF812DE7/2,0x003F690D/2},
       {0xFFC096F3,0x007F685A,0x0000002D/2,0x00000059/2,0x0000002D/2,
        0xFFC096F3,0x007F685A,0x003FB45A/2,0xFF80974D/2,0x003FB45A/2},
       {0xFFC04BA6,0x007FB42D,0x0000000B/2,0x00000016/2,0x0000000B/2,
        0xFFC04BA6,0x007FB42D,0x003FDA22/2,0xFF804BBD/2,0x003FDA22/2}};
*/
extern const UINT32 BASSM_TABLE_100[10];

extern const UINT32 BASSM_TABLE_200[10];

#ifdef DSP_POST_PROCESS_ENABLE
extern const UINT32 DCFilter_COEFF[5];

extern const UINT32 DELAY_FACTOR[5];


extern const UINT32 MICEQ_TABLE[9];

extern const UINT8 EQ_SHIFT_BIT[2];

#ifdef CC_AUD_7BAND_EQUALIZER
extern const UINT32 EQ_7BAND_TABLE[(57+57+57+57+57+57+57)*5];
#elif defined(CC_AUD_EQ_SETA)
extern const UINT32 EQ_5BAND_TABLE[(41+41*2+41*2+41*2+41)*5];
#else
extern const UINT32 EQ_5BAND_TABLE[(57+57*2+57*2+57*2+57)*5];
#endif

#ifdef DSP_EQ2_SBASS_SUPPORT
#ifdef CC_AUD_3_5K_TREBLE
extern const UINT32 EQ2_SBASS_TABLE[(57*2)*5];
#else
extern const UINT32 EQ2_SBASS_TABLE[(29*2)*5];
#endif
#else
extern const UINT32 SBASSPIPE_TABLE[5][5];
extern const UINT32 SBASSBOOST_TABLE[5][5];
extern const UINT32 SBASSCLEAR_TABLE[5][5];
#endif

extern const UINT32 VSURR_TABLE[3];


// Must comment the Spatializer on target
#ifdef EMULATION_FPGA           // only on FPGA
#ifdef SPATIALIZER
extern const UINT32 N22GAIN[7];
extern const UINT32 N22TABLE[5][16 + 7 + 7 + 7];
#endif // SPATIALIZER
#endif // EMULATION_FPGA

extern const UINT32 REVERB_TABLE[2];
#ifdef CC_AUD_OLD_MTK_VSURR
extern const UINT32 MTK_VSURR_TABLE[20];
extern const UINT32 MTK_VSURR_CROS_TABLE[10];
#else
extern const UINT32 MTK_VSURR_TABLE[20];//20070831,kevin
extern const UINT32 MTK_VSURR_CROS_TABLE[15];
extern const UINT32 MTK_VSURR_BASS_TABLE[5];
extern const UINT32 MTK_VSURR_1STOP_TABLE[5];
extern const UINT32 MTK_VSURR_2STOP_TABLE[5];
#endif

extern const UINT32 BBEV4_LEVEL_DB[6];
extern const UINT32 BBEV4_3D_GAIN[101];
extern const UINT32 BBEV4_DB_8[31];
extern const UINT32 BBEV4_BBE_F_TABLE[5];
extern const UINT32 BBEV4_IIR_COEF_TABLE[5];

extern const UINT32 SRSWOW_TBL[110];
extern const UINT32 SRSTSXT2CH_TBL[143];
extern const UINT32 SRSTSHD_M_TBL[188];//sunman_tshd 


#endif // DSP_POST_PROCESS_ENABLE

extern const UINT32 MIXSOUND_COEFF[3 * 5];

#ifdef DSP_SUPPORT_NPTV
extern const UINT32 VOLUME_FINE_TUNE[41]; 
#endif

extern const UINT32 HPF_TBL[HPF_FREQ_NUM][HPF_TABLE_SET_SZ];
extern const UINT32 ORDER_6_FILTER_TABLE[HPF_FREQ_NUM][HPF_TABLE_SET_SZ];

#endif // _H_DSP_TABLE

