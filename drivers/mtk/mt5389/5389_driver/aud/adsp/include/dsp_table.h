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
 * $Date: 2012/10/12 $
 * $RCSfile: dsp_table.h,v $
 * $Revision: #5 $
 *
 *---------------------------------------------------------------------------*/

/** @file dsp_table.h
 *  Brief of file dsp_table.h.
 *  Details of file dsp_table.h (optional).
 */

#ifndef DSP_TABLE_H
#define DSP_TABLE_H

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
#define SFREQ_52K                   (0x10)
#define SFREQ_62K                   (0x11)
#define SFREQ_64K                   (0x12)
#define SFREQ_56K                   (0x14)

#define SAMPLE_32K                  (0)
#define SAMPLE_44K                  (1)
#define SAMPLE_48K                  (2)
#define SAMPLE_96K                  (3)
#define SAMPLE_192K                 (4)

#define DSP_BASSM_SZ                (10)
#define DSP_PL2_SZ                  (32)

#define DSP_EQ_BAND_TABLE_SIZE  (((14*2*2)+1)*5)

// HPF Table relative define
#define HPF_FC_STR     50
#define HPF_FC_END     300
#define HPF_FC_STEP    10
#define HPF_FREQ_NUM   (((HPF_FC_END - HPF_FC_STR)/HPF_FC_STEP) + 1)

#define HPF_TABLE_SET_SZ (5)
#ifdef CC_AUD_FIR_SUPPORT
#define DSP_FIR_SZ                  (256*2)//*2 LR SUPPORT
#endif



// *********************************************************************
// Tables
// *********************************************************************
extern const UINT32 BASSM_TABLE_50[10];
extern const UINT32 BASSM_TABLE_60[10];
extern const UINT32 BASSM_TABLE_70[10];
extern const UINT32 BASSM_TABLE_80[10];
extern const UINT32 BASSM_TABLE_90[10];
extern const UINT32 BASSM_TABLE_100[10];
extern const UINT32 BASSM_TABLE_110[10];
extern const UINT32 BASSM_TABLE_120[10];
extern const UINT32 BASSM_TABLE_130[10];
extern const UINT32 BASSM_TABLE_140[10];
extern const UINT32 BASSM_TABLE_150[10];
extern const UINT32 BASSM_TABLE_160[10];
extern const UINT32 BASSM_TABLE_170[10];
extern const UINT32 BASSM_TABLE_180[10];
extern const UINT32 BASSM_TABLE_190[10];
extern const UINT32 BASSM_TABLE_100[10];
extern const UINT32 BASSM_TABLE_200[10];
extern const UINT32 BASSM_TABLE_300[10];


#ifdef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
extern const UINT32 MTK_COMB_R_TABLE[70];
extern const UINT32 MTK_COMB_L_TABLE[70];
extern const UINT32 MTK_M2S_ALLPASS_TABLE[20];
#endif

#ifdef CC_AUD_PCM_LINE_IN_LPF
extern const UINT32 PCM_LINE_IN_LPF_TABLE[20];
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
#ifdef CC_AUD_PEQ_SUPPORT
extern const UINT32 COS_TABLE[8192];
extern const UINT32 MTK_TAN_TABLE[12501];
#endif
#ifdef CC_AUD_VBASS_SUPPORT
extern const UINT32 MTK_VBASS_R_TABLE[70];
extern const UINT32 MTK_VBASS_L_TABLE[70];
extern const UINT32 MTK_VBASS_CUTOFF_TABLE[6][5];
extern const UINT32 MTK_VBASS_HIGHPASS_TABLE[20];
#endif

#ifdef CC_AUD_FIR_SUPPORT
extern const UINT32 FIR_TABLE_1[512];
extern const UINT32 FIR_TABLE_2[512];
extern const UINT32 FIR_TABLE_3[512];
#endif

extern const UINT32 DELAY_FACTOR[5];

extern const UINT32 MTK_VSURR_TABLE[20];//20070831,kevin
extern const UINT32 MTK_VSURR_CROS_TABLE[15];
extern const UINT32 MTK_VSURR_BASS_TABLE[5];
extern const UINT32 MTK_VSURR_1STOP_TABLE[5];
extern const UINT32 MTK_VSURR_2STOP_TABLE[5];

#ifdef CC_AUD_BBE_SUPPORT
extern const UINT32 BBEV4_LEVEL_DB[6];
extern const UINT32 BBEV4_3D_GAIN[101];
extern const UINT32 BBEV4_DB_8[31];
extern const UINT32 BBEV4_BBE_F_TABLE[1];
extern const UINT32 BBEV4_IIR_COEF_TABLE[5];
#endif

extern const UINT32 SRSWOW_TBL[3][110];
extern const UINT32 SRSTSXT2CH_TBL[3][143];
extern const UINT32 _au4CookSampPerFrame[30];
extern const UINT32 SRSTSHD_M_TBL[3][188+90];//sunman_tshd
extern UINT32 SRSTSHD_CUSTOM_SPEAKER_TBL[25];//bofeng_custom size


extern const UINT32 MIXSOUND_COEFF[3*5];

#ifdef DSP_SUPPORT_NPTV
extern const UINT32 VOLUME_FINE_TUNE[41];
#endif

#ifdef CC_AUD_HPF_SUPPORT
extern const UINT32 HPF_TBL[HPF_FREQ_NUM][HPF_TABLE_SET_SZ];
extern const UINT32 ORDER_6_FILTER_TABLE[HPF_FREQ_NUM][HPF_TABLE_SET_SZ];
#endif

extern const UINT32 _au4OptlFuncTableSize[AUD_OPL_NUM][3];

#ifdef CC_AUD_MIXSOUND_SUPPORT
extern const UINT32 _au4MixSndData0[24];
extern const UINT32 _au4MixSndData0_Stereo[48];
extern const UINT32 _au4MixSndData0_Stereo_LOnROff[64];
#endif

#if 0 //Daniel, 20100202, Move to customization part
#ifdef CC_AUD_DSP_SUPPORT_AUDYSSEY
extern const UINT32 AEQ_5BAND_TABLE[1][(1+1+1+1+1)*5];
#endif
#endif

#ifdef CC_AUD_DSP_SUPPORT_AUDYSSEY//ABX_SUPPORT
extern const UINT32 ABX_COEF_TABLE[2][100]; //DANIEL_20091013
extern const UINT32 ABX_GAIN_TABLE[25]; //DANIEL_20091013
#endif

#ifdef CC_AUD_DSP_SUPPORT_CDNOTCH
extern const UINT32 CDNOTCH_ALPHA_HALFH_TABLE[76];
#endif

extern const UINT32 AVC_PARA_TAB[38];

#ifdef CC_AUD_SUPPORT_MS10
extern const UINT32 xmxchsclgainstolineartab[16];
extern const UINT32 xmxpgmscltolineartab[64];
extern const UINT32 xmxgaininterptab[245];
extern const UINT32 xmxpanmean2chlineargaintab[42];
#endif

#ifdef KARAOKE_SUPPORT
extern const UINT32 KEYSHIFT_TABLE[132];
#endif

#ifdef DSP_SUPPORT_SRS_SSHD
extern const INT32 srs_peq[3][80];
extern const INT32 srs_hpf[3][10];
extern const INT32 srs_lmt[243];
extern const INT32 srs_lmt_lzTbl[256];

#endif
#ifdef DSP_SUPPORT_SRS_PURESOUND
extern const INT32 srs_lmt[243];
extern const INT32 srs_lmt_lzTbl[256];
//extern const INT32 srs_aeq[104+50+15];
extern const INT32 srs_aeq[77+50+15];

#endif


//zy_dts
#if 0  //Disable DTS for code shrink
extern const UINT32 DTSENC_TABLE[8640];
extern const UINT32 dts_twiddle_tab[32];
extern const UINT32 dts_rms7bit_tab[125];
extern const UINT32 dts_qmfCoeff_tab[512];
extern const UINT32 dts_tab[669];
#endif

#ifdef DSP_TEST_CORDIC                          // CORDIC test
extern const UINT32 u4CordicTest[];
#endif

#endif // _H_DSP_TABLE

