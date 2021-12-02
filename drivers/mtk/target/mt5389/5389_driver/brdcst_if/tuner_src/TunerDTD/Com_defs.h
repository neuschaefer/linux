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
/***********************************************************************/
/*                                                                     */
/*    FILE DESCRIPTION                           Com_defs.h            */
/*                                                                     */
/*                                                                     */
/*    ROUTINES                                                         */
/*                                                                     */
/*    NOTES                                                            */
/*                                                                     */
/*    MAINTAINER                                                       */
/*      James Wei      MediaTek Inc.   2005.04                         */
/*                                                                     */
/***********************************************************************/

#ifndef COM_DEFS_H
#define COM_DEFS_H

/***********************************************************************/
/*      Defines                                                        */
/***********************************************************************/
#define fcADD_MSG_TIME          0
#define fcMT5111_ACQ            0

/***********************************************************************/
#define cCMD_REG_NUM            8
#define cCMD_ID_POS             (cCMD_REG_NUM - 1)
//#define cPARA_NUM_POS           0
#define cFW_RUN_TAG             0x66
#define cFW_WDT_TAG             0x99
#define cFW_INITED_TAG          0x33
#define cFW_RESET_TAG           0x00

#define cADV_FW_TAG             0x55
#define cMSG_TAG                0xDD
#define cSIG_LVL_BOUND          150

/***********************************************************************/
/*``[                                                                  */
/***********************************************************************/
/*                                                                     */
/*  Format:                                                            */
/*                                                                     */
/*  #define cCMD_ID_XXX         0xYY    ==> Cmd_ID / Rsp_ID definition */
/*  enum                                                               */
/*  {                                                                  */
/*      cOFST_XXX1,                     ==> Parameter 0                */
/*      cOFST_XXX2,                     ==> Parameter 1                */
/*      cOFST_XXX_SIZE                  ==> Parameter Numbers          */
/*  };                                                                 */
/*                                                                     */
/***********************************************************************/

/***********************************************************************/
/*      Acquisition Start / Stop                                       */
/***********************************************************************/
#define cCMD_ID_DEMOD_START     0x10        // Start Inband Acquisition
enum
{
    cOFST_DEMOD_START_RESET,                // 1: Reset before every Re-Acq
    cOFST_DEMOD_START_SIZE
};
#define cCMD_ID_DEMOD_STOP      0x11        // Stop Inband Acquisition
#define cCMD_ID_DEMOD_FDC_START 0x12        // Start OOB FDC Acquisition
#define cCMD_ID_DEMOD_FDC_STOP  0x13        // Stop OOB FDC Acquisition
#define cCMD_ID_DEMOD_RDC_START 0x14        // Start OOB RDC Initialization
enum
{
    cOFST_DEMOD_RDC_START_RESET,
    cOFST_DEMOD_RDC_MODE,                   // 0: Mode A
                                            // 1: Mode B, Grade A
                                            // 2: Mode B, Grade B
                                            // 3: Mode B, Grade C
    cOFST_DEMOD_RDC_RF_FREQ_EN,             // 1: Set Freq, 0: not set freq
    cOFST_DEMOD_RDC_RF_FREQ_H,              // High byte of RF Frequency in KHz
    cOFST_DEMOD_RDC_RF_FREQ_L,              // Low byte of RF Frequency in KHz
    cOFST_DEMOD_RDC_START_SIZE
};

#define cCMD_ID_DEMOD_RDC_STOP  0x15        // Reset OOB RDC
#define cCMD_ID_TUNER_I2C_ON    0x16        // Enable I2C control for Tuner
#define cCMD_ID_TUNER_I2C_OFF   0x17        // Disable I2C control for Tuner
#define cCMD_ID_TUNER_I2C_STAT  0x18        // Query status of I2C control for Tuner
enum
{
    cOFST_TUNER_I2C_STAT,                   // 0: Tuner I2C control disabled
                                            // 1: Tuner I2C control enabled
    cOFST_TUNER_I2C_STAT_SIZE
};

#define cCMD_ID_DRIVER_START  0x19
enum
{
    cOFST_DRIVER_PWR_UP_PIN,                 // 1: Do power up, 0: Do NOT power up
    cOFST_DRIVER_START_RESET,                // 1: Reset before every Re-Acq
    cOFST_DRIVER_DEMOD_MOD,                  // Modulation type
    cOFST_DRIVER_AUTO_QAM,                   // 1: Enable AutoQam function, 0: Disable function
    cOFST_DRIVER_QSPCINV,
    cOFST_DRIVER_QADSAMP,                    // 1: 60MHz, 0: 50MHz (for MT538x QAM)
    cOFST_DRIVER_RESERVED2,
    cOFST_DRIVER_START_SIZE
};

/***********************************************************************/
/*      Get Demod Status                                               */
/***********************************************************************/
#define cCMD_ID_DEMOD_GET_ACQ   0x20        // Get Acquisition Status
enum
{
    cOFST_DEMOD_ACQ_STATUS,                 // Ref. DEMOD_ACQ_STATUS Bit definition
    cOFST_DEMOD_ACQ_STATE,                  // Reserved
    cOFST_DEMOD_ACQ_START,                  // 1: Inband Acquisition Started
    cOFST_DEMOD_REACQ_CNT,                  // Inband Re-Acquisition count
//    cOFST_DEMOD_ACQ_TIME_L,
    cOFST_DEMOD_ACQ_TIME_H,                 // Reserved
    cOFST_DEMOD_SIG_LEVEL,                  // Rx signal strength: 0 - 100
    cOFST_DEMOD_SIG_CNT,                    // bit[7:4]: Confidence of no inband-signal existing
                                            // bit[3:0]: Confidence of inband-signal existing
    cOFST_DEMOD_ACQ_SIZE
};

#define cCMD_ID_DEMOD_GET_INFO1 0x21
enum
{
    cOFST_DEMOD_INFO_AGC_RF0,               // RF-AGC Gain
    cOFST_DEMOD_INFO_AGC_RF1,
    cOFST_DEMOD_INFO_AGC_IF0,               // IF-AGC Gain
    cOFST_DEMOD_INFO_AGC_IF1,
    //cOFST_DEMOD_INFO_CR0,                   // Carrier frequency offset
    //cOFST_DEMOD_INFO_CR1,    
    //cOFST_DEMOD_INFO_CR2,
    cOFST_DEMOD_INFO1_SIZE
};
#define cCMD_ID_DEMOD_GET_INFO2 0x22
enum
{
    cOFST_DEMOD_INFO_SNR,                   // SNR reported by EQ
    cOFST_DEMOD_INFO_FS_SNR,                // Reserved
    cOFST_DEMOD_INFO_PER0,                  // TS packet error monitor duration
    cOFST_DEMOD_PER_CNT,                    // TS accumulated packet count
    cOFST_DEMOD_INFO_PER1,                  // TS accumulated error packet count
    cOFST_DEMOD_INFO_PER2,
    cOFST_DEMOD_TSIF_RST,                   // Reserved
    cOFST_DEMOD_INFO2_SIZE
};

#define cCMD_ID_DEMOD_GET_INFO3 0x23
enum
{
    cOFST_DEMOD_INFO_SER0,                  // EQ symbol error rate
//    cOFST_DEMOD_INFO_SER1,
    cOFST_DEMOD_INFO_BER0,                  // TCM bit error rate
    cOFST_DEMOD_INFO_BER1,
    cOFST_DEMOD_INFO_BER2,
    cOFST_DEMOD_INFO_POW0,                  // Reserved
    cOFST_DEMOD_INFO_POW1,
    cOFST_DEMOD_INFO_SPECTRUM,              // 1: Inverted Spectrum polarity
    cOFST_DEMOD_INFO3_SIZE
};

#define cCMD_ID_OOB_GET_ACQ     0x24
enum
{
    cOFST_DEMOD_ACQ_STATUS_FDC,             // Ref. DEMOD_ACQ_STATUS_FDC Bit definition
    cOFST_DEMOD_ACQ_STATE_FDC,              // Reserved
    cOFST_DEMOD_ACQ_START_FDC,              // 1: FDC Acquisition Started
    cOFST_DEMOD_REACQ_CNT_FDC,              // FDC Re-Acquisition count
    cOFST_DEMOD_ACQ_STATE_RDC,              // Reserved
    cOFST_DEMOD_ACQ_START_RDC,              // 1: RDC Initialized
    cOFST_DEMOD_OOB_ACQ_SIZE
};

#define cCMD_ID_DEMOD_GET_INFO4 0x25
enum
{
    cOFST_DEMOD_INFO_TR0,                   // Timing offset
    cOFST_DEMOD_INFO_TR1,
    cOFST_DEMOD_INFO_ACQ_TIME_H,            // Acquisition time to lock channel (ms)
    cOFST_DEMOD_INFO_ACQ_TIME_L,
    cOFST_DEMOD_INFO_LEAKAGE,               // EQ Leakage
    cOFST_DEMOD_INFO_EQ_STEP,               // EQ Step size
    cOFST_DEMOD_INFO_TV,                    // Time-varying indicator
    cOFST_DEMOD_INFO4_SIZE
};

#define cCMD_ID_DEMOD_GET_FDC_INFO 0x26
enum
{
    cOFST_DEMOD_INFO_FDC_OSYS_MD,           // 0: Mode A
                                            // 1: Mode B, Grade A
                                            // 2: Mode B, Grade B
    cOFST_DEMOD_INFO_FDC_OPRA_CTL,          // FDC AGC gain
    cOFST_DEMOD_INFO_FDC_CR0,               // FDC Carrier frequency offset
    cOFST_DEMOD_INFO_FDC_CR1,
    cOFST_DEMOD_INFO_FDC_TR0,               // FDC Timing offset
    cOFST_DEMOD_INFO_FDC_TR1,
    cOFST_DEMOD_FDC_INFO_SIZE
};

#define cCMD_ID_DEMOD_GET_OLBT_INFO 0x27    // Reserved
enum
{
    cOFST_DEMOD_INFO_OLBT_ACCERR1,
    cOFST_DEMOD_INFO_OLBT_ACCERR2,
    cOFST_DEMOD_INFO_OLBT_ACCERR3,
    cOFST_DEMOD_OLBT_INFO_SIZE
};

#define cCMD_ID_DEMOD_GET_FDC_INFO2 0x28    // Reserved
enum
{
    cOFST_DEMOD_INFO_OEQ_CNST_RE,
    cOFST_DEMOD_INFO_OEQ_CNST_IM,
    cOFST_DEMOD_FDC_INFO2_SIZE
};

#define cCMD_ID_DEMOD_GET_FDC_INFO3 0x29    // Reserved
enum
{
    cOFST_DEMOD_INFO_OEQ_RE_WT_1,
    cOFST_DEMOD_INFO_OEQ_RE_WT_2,
    cOFST_DEMOD_INFO_OEQ_IM_WT_1,
    cOFST_DEMOD_INFO_OEQ_IM_WT_2,
    cOFST_DEMOD_INFO_OEQ_TAP_POS,
    cOFST_DEMOD_FDC_INFO3_SIZE
};

#define cCMD_ID_DEMOD_GET_RDC_ACQ 0x2A
enum
{
    cOFST_DEMOD_RDC_ACQ_MOD,                // 0: Mode A
                                            // 1: Mode B, Grade A
                                            // 2: Mode B, Grade B
                                            // 3: Mode B, Grade C
    cOFST_DEMOD_RDC_ACQ_START,              // 1: RDC Initialized
    cOFST_DEMOD_RDC_ACQ_SIZE
};

#define cCMD_ID_DEMOD_GET_FDC_INFO4 0x2B
enum
{
    cOFST_DEMOD_INFO_OEQ_ERR_AVE_0,         // OEQ Error Power
    cOFST_DEMOD_INFO_OEQ_ERR_AVE_1,
    cOFST_DEMOD_FDC_INFO4_SIZE
};

#define cCMD_ID_DEMOD_GET_INFO5 0x2C
enum
{
    cOFST_DEMOD_INFO_SNR_1,                 // SNR reported by EQ
    cOFST_DEMOD_INFO_SNR_2,                 // SNR reported by EQ
    cOFST_DEMOD_INFO_FS_SNR_1,              // FS_SNR
    cOFST_DEMOD_INFO_FS_SNR_2,              // FS_SNR
    cOFST_DEMOD_INFO5_SIZE
};

#define cCMD_ID_DEMOD_GET_REACQ   0x2D      // Get Re-Acquisition Status
enum
{
    cOFST_DEMOD_STATUS_IND,                 // ucStatusInd: Report Re-acq cause
    cOFST_DEMOD_STATUS_IND_EXTEND, 
    cOFST_DEMOD_REACQ_SIZE
};


#define cCMD_ID_DEMOD_GET_REACQ_LOG   0x2E      // Get Re-Acquisition Log
enum
{
    cOFST_DEMOD_REACQ_UCLOG_1,
    cOFST_DEMOD_REACQ_UCLOG_2,
    cOFST_DEMOD_REACQ_UCLOG_3,    
    
    cOFST_DEMOD_REACQ_U2LOG_1_H,
    cOFST_DEMOD_REACQ_U2LOG_1_L,
    
    cOFST_DEMOD_REACQ_U2LOG_2_H,
    cOFST_DEMOD_REACQ_U2LOG_2_L,

    cOFST_DEMOD_REACQ_LOG_SIZE
};                         

#define cCMD_ID_DEMOD_GET_INFO6   0x2F      // Get PER
enum
{
    cOFST_DEMOD_INFO6_RV1,                   // Reserved
    cOFST_DEMOD_INFO6_RV2,                   // Reserved
    cOFST_DEMOD_INFO6_PER0,                  // TS packet error monitor duration
    cOFST_DEMOD_INFO6_PER_CNT,               // TS accumulated packet count
    cOFST_DEMOD_INFO6_PER1,                  // TS accumulated error packet count
    cOFST_DEMOD_INFO6_PER2,
    cOFST_DEMOD_INFO6_RV3,                   // Reserved
    cOFST_DEMOD_INFO6_SIZE
};

/***********************************************************************/
#define cCMD_ID_DEMOD_GET_CFO_INIT 0x79
enum
{
	cOFST_DEMOD_CFO_INIT_CR0,         //EC_code_dB815: Add the Host-commands for init value of carrier frequency offset for SAWLess DTD QAM mode
	cOFST_DEMOD_CFO_INIT_CR1,
       cOFST_DEMOD_CFO_INIT_CR2,
       cOFST_DEMOD_INFO_CR0,                 // Carrier frequency offset
       cOFST_DEMOD_INFO_CR1,
       cOFST_DEMOD_INFO_CR2,
	cOFST_DEMOD_CFO_INIT_SIZE
};

#define cCMD_ID_DEMOD_GET_INFO7       0x80
enum
{
    cOFST_DEMOD_INFO7_SLD_PRA_STATUS_MONITOR,
    cOFST_DEMOD_INFO7_SLD_PRA_RFAGC_CTRL_WORD_H,
    cOFST_DEMOD_INFO7_SLD_PRA_RFAGC_CTRL_WORD_L,
    cOFST_DEMOD_INFO7_SLD_PRA_IFAGC_CTRL_WORD_H,
    cOFST_DEMOD_INFO7_SLD_PRA_IFAGC_CTRL_WORD_L,
    cOFST_DEMOD_INFO7_SLD_PRA_PGA_GAIN_INDEX,
    cOFST_DEMOD_INFO7_SIZE
};

#define cCMD_ID_DEMOD_GET_INFO8       0x81
enum
{
    cOFST_DEMOD_INFO8_SLD_PRA_MAG_REF_OUT_H,
    cOFST_DEMOD_INFO8_SLD_PRA_MAG_REF_OUT_L,
    cOFST_DEMOD_INFO8_LEGACY_PRAD_Cur_Level_sf_H,
    cOFST_DEMOD_INFO8_LEGACY_PRAD_Cur_Level_sf_L,
    cOFST_DEMOD_INFO8_PRA_W_ADC0,
    cOFST_DEMOD_INFO8_PRA_W_ADC1,
    cOFST_DEMOD_INFO8_SIZE
};



#define cCMD_ID_DEMOD_GET_INFO9       0x82
enum
{
    cOFST_DEMOD_INFO9_REG_VCR_Rpdev_0,
    cOFST_DEMOD_INFO9_REG_VCR_Rpdev_1,
    cOFST_DEMOD_INFO9_REG_VCR_Cpdev_0,
    cOFST_DEMOD_INFO9_REG_VCR_Cpdev_1,
    cOFST_DEMOD_INFO9_REG_VCR_Pilot,
    cOFST_DEMOD_INFO9_IIR_BW,    
    cOFST_DEMOD_INFO9_VPR_INFO,        
    
    cOFST_DEMOD_INFO9_SIZE
};

#define cCMD_ID_DEMOD_GET_INFOA       0x83
enum
{
    cOFST_DEMOD_INFOA_REG_GS_FW_Cfg_1,
    cOFST_DEMOD_INFOA_REG_GS_FW_Cfg_2,    	
    cOFST_DEMOD_INFOA_REG_VCR_Dlp_Ind,
    cOFST_DEMOD_INFOA_REG_VCR_Dlp_Cnt,

    cOFST_DEMOD_INFOA_SIZE
};

#define cCMD_ID_SET_TUNER_PARA0 0x84
enum
{
    cOFST_SET_TUNER_PARA0_SLP,
    cOFST_SET_TUNER_PARA0_INT,
    cOFST_SET_TUNER_PARA0_MAX,
    cOFST_SET_TUNER_PARA0_MIN,
    cOFST_SET_TUNER_PARA0_SIZE
};

#define cCMD_ID_SET_TUNER_PARA1 0x85
enum
{
    cOFST_SET_TUNER_PARA1_RFTuner,
    cOFST_SET_TUNER_PARA1_TarLV_1,
    cOFST_SET_TUNER_PARA1_TarLV_2,
    cOFST_SET_TUNER_PARA1_TarLV_3,
    cOFST_SET_TUNER_PARA1_PGAIdx_Legacy,
    cOFST_SET_TUNER_PARA1_SIZE
};

/*      Set Demod Configuration                                        */
/***********************************************************************/
#define cCMD_ID_DEMOD_SET_REG   0x30        // Write up to 4 demod registers with continuous address
enum
{
    cOFST_REG_NUM,                          // Register numbers
    cOFST_REG1_ADDR_L,                      // Low byte of 1st register address
    cOFST_REG1_ADDR_H,                      // High byte of 1st register address
    cOFST_REG1_DATA,                        // 1st register data
    cOFST_REG1_SIZE,
    cOFST_REG2_DATA = cOFST_REG1_SIZE,      // 2nd register data
    cOFST_REG3_DATA,                        // 3rd register data
    cOFST_REG4_DATA,                        // 4th register data
    cOFST_REG4_SIZE,
//    cOFST_REG2_ADDR_L = cOFST_REG1_SIZE,
//    cOFST_REG2_ADDR_H,
//    cOFST_REG2_DATA,
//    cOFST_REG2_SIZE
    cOFST_REG_ALL_SIZE = cOFST_REG4_SIZE
};

#define cCMD_ID_DEMOD_SET_MOD   0x34        // Set Inband modulation type
enum
{
    cOFST_DEMOD_MOD,                        // Ref. DEMOD_SET_MOD / DEMOD_GET_MOD definition
    cOFST_DEMOD_MOD_DBG1,                   // Reserved
    cOFST_DEMOD_MOD_DBG2,
    cOFST_DEMOD_MOD_DBG3,
    cOFST_DEMOD_MOD_SIZE
};
#define cCMD_ID_FDC_SET_MOD     0x35        // Set OOB FDC mode
                                            // 0: Mode A
                                            // 1: Mode B, Grade A
                                            // 2: Mode B, Grade B
#define cCMD_ID_TS_SET_FMT      0x36        // Set TS output format
enum
{
    cOFST_DEMOD_TS_FMT_ALL,                 // Ref. TS_SET_FMT / TS_GET_FMT Bit definition
    cOFST_DEMOD_TS_FMT_SIZE
};
#define cCMD_ID_DEMOD_SET_VAR   0x37        // Set firmware configuration, ref. DEMOD_SET_REG
#define cCMD_ID_FDC_SET_SYNTH   0x38        // Set FDC synthesizer frequency (for silicon tuner)
enum
{
    cOFST_SET_SYNTH_SIZE = 4
};
#define cCMD_ID_FDC_SET_DIFF    0x39        // Set FDC differential mode (spectrum inverse)
#define cCMD_ID_RDC_SET_PGA     0x3A
enum
{
    cOFST_RDC_SET_PGA_SCL,
    cOFST_RDC_SET_PGA_SDA,
    cOFST_RDC_SET_PGA_AS,
    cOFST_RDC_SET_PGA_SIZE
};
#define cCMD_ID_TS_SET_DRIVING  0x3B
enum
{
    cOFST_DEMOD_TS_DRIVING,
    cOFST_DEMOD_TS_DRIVING_SIZE
};

#define cCMD_ID_DEMOD_SET_SAMPLING  0x3C   // Set Inband sampling rate 
enum
{
    cOFST_DEMOD_SAMPLING_RATE,             // 1: 60MHz, 0: 25MHz
    cOFST_DEMOD_SAMPLING_FIXED,            // 1: non-fix(sampling would change), 0: fixed
    cCMD_ID_DEMOD_SAMPLING_SIZE
};

#define cCMD_ID_DEMOD_SET_TEST_MODE  0x3D   // Set Test mode
enum
{
    cOFST_DEMOD_TEST_MODE,
    cOFST_DEMOD_ANALOG_LOOKBACK,
    cCMD_ID_DEMOD_TEST_MODE_SIZE
};
#define cCMD_ID_SET_REG_VAR     0x3E

#define cCMD_ID_DEMOD_SET_MANUAL_AGC  0x3F   // Set Manual AGC
enum
{
    cCMD_ID_DEMOD_MAGC_EN,
    cCMD_ID_DEMOD_MAGC_RF_CTRL_0,
    cCMD_ID_DEMOD_MAGC_RF_CTRL_1,
    cCMD_ID_DEMOD_MAGC_IF_CTRL_0,
    cCMD_ID_DEMOD_MAGC_IF_CTRL_1,
    cCMD_ID_DEMOD_MAGC_SIZE
};


/***********************************************************************/
/*      Get Demod Configuration                                        */
/***********************************************************************/
#define cCMD_ID_GET_VER         0x40
enum
{
    cOFST_VER_YEAR_MONTH,
    cOFST_VER_DATE,
    cOFST_VER_HOUR,
    cOFST_VER_MINUTE,
    cOFST_VER_TYPE,
    cOFST_VER_ID,
    cOFST_VER_SIZE
};
#define cCMD_ID_DEMOD_GET_REG   0x41
#define cCMD_ID_FDC_GET_REG     0x42
#define cCMD_ID_RDC_GET_REG     0x43
#define cCMD_ID_DEMOD_GET_MOD   0x44
#define cCMD_ID_FDC_GET_MOD     0x45
#define cCMD_ID_TS_GET_FMT      0x46
#define cCMD_ID_DEMOD_GET_VAR   0x47
#define cCMD_ID_FDC_GET_DIFF    0x48
#define cCMD_ID_TS_GET_DRIVING  0x49
#define cCMD_ID_GET_REG_VAR     0x4A

/* For TS_SET_FMT / TS_GET_FMT Bit definition */
enum
{
    cBIT_TS_FMT_VALID_LOW,
    cBIT_TS_FMT_ERR_LOW,
    cBIT_TS_FMT_SYNC_LOW,
    cBIT_TS_FMT_PARALLEL,
    cBIT_TS_FMT_CLK_CONT,
    cBIT_TS_FMT_DATA_FALL,
    cBIT_TS_FMT_OUTPUT_EN
};

#define cCMD_ID_GET_LABEL         0x4B
enum
{
    cOFST_LABEL_BRANCH_TYPE,
    cOFST_LABEL_BRANCH_ID_1,
    cOFST_LABEL_BRANCH_ID_2,
    cOFST_LABEL_ID_1,
    cOFST_LABEL_ID_2,
    cOFST_LABEL_SIZE
};

/***********************************************************************/
/*      Demod Reset                                                    */
/***********************************************************************/
#define cCMD_ID_UP_RESET        0x60
#define cCMD_ID_GET_VAR         0x61
enum
{
//    cOFST_GET_VAR_IDX,
    cOFST_GET_VAR_CURR,
    cOFST_GET_VAR_STOP,
    cOFST_GET_VAR_SYND,
    cOFST_GET_VAR_CFG,
    cOFST_GET_VAR_SPARSE,
    cOFST_GET_VAR_LEAK,
    cOFST_GET_VAR_SPEED,
    cOFST_GET_VAR_SIZE
};
#define cCMD_ID_UP_PWR_DN               0x62
#define cCMD_ID_UP_PWR_UP               0x63
#define cCMD_ID_UP_WDT_RESET            0x64
#define cCMD_ID_RESET_INBAND            0x65
#define cCMD_ID_RESET_OOB               0x66
#define cCMD_ID_SET_IFAGC_HIGH_Z        0x67
enum
{
    cOFST_SET_IFAGC_EN,                 // 0: Normal, 1: High-Z mode
    cCMD_ID_SET_IFAGC_HIGH_Z_SIZE
};

/***********************************************************************/
/*      Smart Antenna Control                                          */
/***********************************************************************/
#define cCMD_ID_ANTIF_WRITE     0x70
enum
{
    cOFST_ANTIF_WRITE_CODE0,
    cOFST_ANTIF_WRITE_CODE1,
    cOFST_ANTIF_WRITE_SIZE
};
#define cCMD_ID_ANTIF_READ      0x71
#define cCMD_ID_ANTIF_SCAN      0x72
#define cCMD_ID_ANTIF_STATUS    0x73
enum
{
    cOFST_ANTIF_READY,
    cOFST_ANTIF_CODE0,
    cOFST_ANTIF_CODE1,
    cOFST_ANTIF_STATUS_SIZE
};

#define cCMD_ID_ANT_DETECT      0x74
enum
{
    cOFST_ANT_DETECT_PIN,
    cOFST_ANT_DETECT_SIZE
};

#define cCMD_ID_GPIO_OUT        0x75
enum
{
    cOFST_GPIO_OUT_ADDR,
    cOFST_GPIO_OUT_VALUE,
    cOFST_GPIO_OUT_SIZE
};
#define cCMD_ID_GPIO_IN         0x76

#define cCMD_ID_ANT_SET_DET_EN  0x77
enum
{
    cOFST_ANT_DET_EN,
    cOFST_ANT_DET_EN_SIZE
};


/***********************************************************************/
/*``]                                                                  */
/***********************************************************************/
/*      Log / Debug Function                                           */
/***********************************************************************/
#define cCMD_ID_DEMOD_CFG       0x31
enum
{
    cOFST_CFG_TYPE,
    cOFST_CFG_ENABLE,
    cOFST_CFG_NUM,
//    cOFST_CFG_ADDR_H,
    cOFST_CFG_SIZE
};
#define cCMD_ID_DEMOD_CFG_PARA  0x32
enum
{
    cOFST_CFG_PARA_TYPE,
    cOFST_CFG_IDX,
    cOFST_CFG_ADDR1_L,
    cOFST_CFG_ADDR1_H,
    cOFST_CFG_VALUE1,
    cOFST_CFG_PARA1_SIZE
//    cOFST_CFG_ADDR2 = cOFST_CFG_PARA1_SIZE,
//    cOFST_CFG_VALUE2,
//    cOFST_CFG_PARA2_SIZE
};

#define cCMD_ID_DEMOD_TEST      0x33
enum
{
    cOFST_TEST_TYPE,
    cOFST_TEST_ENABLE,
    cOFST_TEST_SIZE
};

/***********************************************************************/
#define cCMD_ID_LOG_GET_ADDR    0x50
enum
{
    cOFST_LOG_ADDR_L,
    cOFST_LOG_ADDR_H,
    cOFST_LOG_ADDR_SIZE,
    cOFST_LOG_DATA_L = cOFST_LOG_ADDR_SIZE,
    cOFST_LOG_DATA_H,
    cOFST_LOG_DATA_SIZE
};

#define cCMD_ID_LOG_GET_SIZE    0x51
#define cCMD_ID_LOG_SET_SIZE    0x52
enum
{
    cOFST_LOG_OVERFLOW,
    cOFST_LOG_SET_SIZE
};

#define cCMD_ID_LOG_SET_TYPE    0x53
enum
{
    cOFST_LOG_TYPE,
    cOFST_LOG_SET_TYPE_SIZE
};

#define cCMD_ID_MSG_GET_SIZE    0x54
enum
{
    cOFST_MSG_ADDR,
    cOFST_MSG_ADDR_SIZE
};
#define cCMD_ID_MSG_GET_DATA    0x55
#define cCMD_ID_ADCAP_START     0x56
#define cCMD_ID_ADCAP_STOP      0x57
#define cCMD_ID_OADCAP_START    0x58
#define cCMD_ID_OADCAP_STOP     0x59
#define cCMD_ID_TEST_SEQ        0x5A

/***********************************************************************/
/* For DEMOD_SET_MOD / DEMOD_GET_MOD definition */
enum
{
    MT5112_MOD_TYPE_BEGIN = 0,
    MT5112_MOD_ATSC_8VSB = MT5112_MOD_TYPE_BEGIN,
//    MT5112_MOD_RESERVED,
    MT5112_MOD_J83B_64QAM,
    MT5112_MOD_J83B_256QAM,
    MT5112_MOD_TYPE_END
};

typedef enum
{
    MT5112_TUNER_TYPE_BEGIN = 0,
    MT5112_TUNER_PHILIPS_TD1336O = MT5112_TUNER_TYPE_BEGIN,   // add new Inband tuner type after this line
    MT5112_TUNER_SAMSUNG_DTVS203,
    MT5112_TUNER_SAMSUNG_DTNM3000AIA,
    MT5112_TUNER_MAXLINEAR_MXL5003,
    MT5112_TUNER_PHILIPS_FQD1236,
    MT5112_TUNER_FORWARD_TAEH6,
    MT5112_TUNER_PHILIPS_FQD1236E,
    MT5112_TUNER_THOMSON_DTT76500,
    MT5112_TUNER_SAMSUNG_DTVS203EH,
//    MT5112_TUNER_THOMSON_DTT76800,
    MT5112_TUNER_THOMSON_DTT76830,
//    MT5112_TUNER_LG_TDVTH001F,
    MT5112_TUNER_LG_TDVTH151F,
    MT5112_TUNER_ALPS_TDEU2,
    MT5112_TUNER_ALPS_TDQU2,
    MT5112_TUNER_ALPS_TDQU4X524A,
	MT5112_TUNER_ALPS_TDQU8XT01A,
    MT5112_TUNER_ALPS_TDQU8K01A,
    MT5112_TUNER_MICROTUNE_MT2131,
    MT5112_TUNER_PHILIPS_TD1336O_LA7795,
    MT5112_TUNER_PHILIPS_TD1336O_MT1233,
    MT5112_TUNER_SAMSUNG_DTVS227CH,
    MT5112_TUNER_NXP_TD1636,
    MT5112_TUNER_PANASONIC_ENG36A44GF,
    MT5112_TUNER_PANASONIC_ET55HT,
    MT5112_TUNER_COMTECH_DAVB20046,
    MT5112_TUNER_COMTECH_SN761677A,
    MT5112_TUNER_SHARP_VH1B1UF3010,
    MT5112_TUNER_PHILIPS_FCV1236D,
    MT5112_TUNER_COMTECH_ATSC20046,
    MT5112_TUNER_DEV_TYPE_BEGIN = 0x40,          // add new Inband tuner type above this line
    MT5112_TUNER_TUNER_TYPE_END,                  
    MT5112_TUNER_TYPE_NULL = -1
} MT5112_TUNER_ATSC_T;

typedef enum
{
    MT5112_TUNER_TYPE_OOB_BEGIN = 0,
    MT5112_TUNER_ISG_ISG10065 = MT5112_TUNER_TYPE_OOB_BEGIN,  // add new OOB tuner type after this line
    MT5112_TUNER_SANYO_LA7784,
    MT5112_TUNER_SAMSUNG_DTQS300IV241O,
    MT5112_TUNER_TYPE_OOB_END              // add new OOB tuner type above this line
} MT5112_TUNER_OOB_T;

/***********************************************************************/
#define cLOG_TYPE_SRAM          0           // Only for dump memory, not log
#define cLOG_TYPE_NONE          0
#define cLOG_TYPE_TIMER         1
#define cLOG_TYPE_REG_RW        2
#define cLOG_TYPE_QCR           3
#define cLOG_TYPE_ACQ_STATE     4
#define cLOG_TYPE_MONI_REG      5
#define cLOG_TYPE_MONI_FW       6
#define cLOG_TYPE_ADV_ACQ       7
#define cLOG_TYPE_CHIR          8
#define cLOG_TYPE_VTR_PROBE     9
#define cLOG_TYPE_AD_BB         10
#define cLOG_TYPE_MISC          11
#define cLOG_TYPE_EQ_CONST      12
#define cLOG_TYPE_REG_RD        13
#define cLOG_TYPE_REG_WR        14
#define cLOG_TYPE_REG_MONI      15
#define cLOG_TYPE_BZC_PROBE     16
#define cLOG_TYPE_CODE_UTIL     17
#define cLOG_TYPE_VSB_CFG       18

#define cTEST_TYPE_NO_TIMEOUT   0
#define cTEST_TYPE_NO_EQ_WRITE  1
#define cTEST_TYPE_EQ_NO_DD     2
#define cTEST_TYPE_EQ_FIX_STEP  3
#define cTEST_TYPE_EQ_FSM       4
#define cTEST_TYPE_NO_REACQ     5
#define cTEST_TYPE_STEP_ACQ     6

#define cCFG_TYPE_IMMEDIATE     0
#define cCFG_TYPE_MONITOR       1
#define cCFG_TYPE_AGC           2
#define cCFG_TYPE_EQ_TRN        3
//#define cCFG_TYPE_IMMEDIATE_VAR 3
#define cCFG_TYPE_EQ_TRK        4
#define cCFG_TYPE_MONITOR_FW    5
#define cCFG_TYPE_ACQ_CFG       6

//#define cCFG_NUM_MAX            16
#define cCFG_NUM_MAX            32
#define cCFG_NUM_IMMEDIATE      cCFG_NUM_MAX
#define cCFG_NUM_MONITOR        cCFG_NUM_MAX
#define cCFG_NUM_AGC            cCFG_NUM_MAX
#define cCFG_NUM_EQ             cCFG_NUM_MAX
#define cCFG_NUM_MONITOR_FW     cCFG_NUM_MAX
#define cCFG_NUM_ACQ_CFG        cCFG_NUM_MAX

#define cERR_TYPE_ACQ_TIME      0

#define cFDC_DIFF_NORM_SET      0x12
#define cFDC_DIFF_INV_SET       0x13

enum
{
    cOFST_MSG_TAG,
    cOFST_MSG_LEN,
    cOFST_MSG_TYPE,
#if fcADD_MSG_TIME
    cOFST_MSG_TIME_L,
    cOFST_MSG_TIME_H,
#endif
    cOFST_MSG_SIZE
};

enum
{
    eMSG_TIMER,
    eMSG_TIME_CHECK,
    eMSG_SPC_INV,
    eMSG_ACI,
    eMSG_REG_RST,
    eMSG_TRY,
    eMSG_TRY_PER,
    eMSG_BRZC,
    eMSG_SIZE
};

#define cMSG_STR0               "Timeout"
#define cMSG_STR1               "TimeCheck"
#define cMSG_STR2               "SpecInv"
#define cMSG_STR3               "ACI"
#define cMSG_STR4               "RegRst"
#define cMSG_STR5               "Try"
#define cMSG_STR6               "TryPer"
#define cMSG_STR7               "BrzC"
#define cMSG_STR8               ""
#define cMSG_STR9               ""

enum
{
    eVAR_START = 0,
    eVAR_PRA_STATE = eVAR_START,            // 0x00
    eVAR_PRA_CFG,                           // 0x01
    eVAR_PRA_CTL0,                          // 0x02
    eVAR_PRA_CTL1,                          // 0x03
    eVAR_PRA_CTL2,                          // 0x04
    eVAR_PRA_CTL3,                          // 0x05
    eVAR_PRA_SLOPE,                         // 0x06
    eVAR_PRA_RF_BIAS,                       // 0x07
    eVAR_PRA_IF_BIAS,                       // 0x08
    eVAR_PRA_IF_MIN_0,                      // 0x09
    eVAR_PRA_IF_MIN_1,                      // 0x0A
    eVAR_PRA_IF_MAX_0,                      // 0x0B
    eVAR_PRA_IF_MAX_1,                      // 0x0C
    eVAR_PRA_RF_MIN_0,                      // 0x0D
    eVAR_PRA_RF_MIN_1,                      // 0x0E
    eVAR_PRA_RF_MAX_0,                      // 0x0F
    eVAR_PRA_RF_MAX_1,                      // 0x10
    eVAR_PRA_LOOP_AD0,                      // 0x11
    eVAR_PRA_LOOP_AD1,                      // 0x12
    eVAR_UCMAGC_EN,                         // 0x13
    eVAR_UCMAGC_IF_CTRL_0,                  // 0x14
    eVAR_UCMAGC_IF_CTRL_1,                  // 0x15
    eVAR_U2CURACQSTATECNT_H,                // 0x16
    eVAR_U2CURACQSTATECNT_L,                // 0x17
    eVAR_UCTUNERTYPE,                       // 0x18
    eVAR_FGTUNERAGCMAP,                     // 0x19
    eVAR_U2VAGCTIMEOUT_H,                   // 0x1A
    eVAR_U2VAGCTIMEOUT_L,                   // 0x1B
    eVAR_U2QAGCTIMEOUT_H,                   // 0x1C
    eVAR_U2QAGCTIMEOUT_L,                   // 0x1D
    eVAR_U2SIGOPOFST_H,                     // 0x1E
    eVAR_U2SIGOPOFST_L,                     // 0x1F
    eVAR_UCCURACQSTATE,                     // 0x20
    eVAR_FGENMONITOR,                       // 0x21
    eVAR_FGENREACQ,                         // 0x22
    eVAR_FGSTEPNEXTACQ,                     // 0x23
    eVAR_UCNOSIGCNT,                        // 0x24
    eVAR_FGAGCFAIL,                         // 0x25
    eVAR_UCSTATUSIND,                       // 0x26
    eVAR_UCRSTIND,                          // 0x27
    eVAR_UCSTATE1IND,                       // 0x28
    eVAR_UCSTATE2IND,                       // 0x29
    eVAR_FGISYSSPECINV,                     // 0x2A
    eVAR_FGDEFISYSSPECVSB,                  // 0x2B
    eVAR_FGDEFISYSSPECQAM,                  // 0x2C
    eVAR_UCBRZCCFG,                         // 0x2D
    eVAR_UCADSAMPCFG,                       // 0x2E
    eVAR_UCDEMTRIALCNT,                     // 0x2F
    eVAR_UCTS_RSTCNT,                       // 0x30
    eVAR_UCTS_RSTCNT2,                      // 0x31
    eVAR_UCANTIFCODE0,                      // 0x32
    eVAR_UCANTIFCODE1,                      // 0x33
    eVAR_UCCMDCHKSUMERR,                    // 0x34
    eVAR_UCCMDCHKSUMID,                     // 0x35
    eVAR_UCTESTSEQERR,                      // 0x36
    eVAR_FGANTDETENABLE,                    // 0x37
    eVAR_UCANTDETSTATUS,                    // 0x38
    eVAR_FGMT5112_QSYMBOLRATE,              // 0x39
    eVAR_UCQAMADSAMPCFG,                    // 0x3A     
    eVAR_QTR_TFO_1,                         // 0x3B
    eVAR_QTR_TFO_2,                         // 0x3C
    eVAR_QTR_TFO_3,                         // 0x3D
    eVAR_PNHISTORY,                         // 0x3E
    eVAR_PRESYND,                           // 0x3F
    eVAR_UCACCBK,                           // 0x40
    eVAR_BN_COUNT,                          // 0x41
    eVAR_U2ACCERRPKT_H,                     // 0x42
    eVAR_U2ACCERRPKT_L,                     // 0x43
    eVAR_UCACCPKTCNT,                       // 0x44
    eVAR_UCERRPKT_0,                        // 0x45
    eVAR_UCERRPKT_1,                        // 0x46
    eVAR_UCQCR_STTS,                        // 0x47
    eVAR_VCR_CMETRIC_PHDEV_H,               // 0x48 
    eVAR_VCR_CMETRIC_PHDEV_L,               // 0x49
    eVAR_VCR_RMETRIC_PHDEV_H,               // 0x4A
    eVAR_VCR_RMETRIC_PHDEV_L,               // 0x4B
    eVAR_VCR_CMETRIC_PHDEV_AVG_H,           // 0x4C
    eVAR_VCR_CMETRIC_PHDEV_AVG_L,           // 0x4D
    eVAR_VCR_RMETRIC_PHDEV_AVG_H,           // 0x4E
    eVAR_VCR_RMETRIC_PHDEV_AVG_L,           // 0x4F
    eVAR_PAT_CONFIG_IND,                    // 0x50 //EC_code_6728
    eVAR_UCSETPN,                           // 0x51
    eVAR_VCR_PILOT_LEVEL,                   // 0x52
    eVAR_UCVSBDETECTCFG,                    // 0x53
    eVAR_UCVSBDETECTIND,                    // 0x54
    eVAR_UCVSBDETECTEN,                     // 0x55
    eVAR_U2VCR_RMETRIC_THRD_H,              // 0x56
    eVAR_U2VCR_RMETRIC_THRD_L,              // 0x57
    eVAR_UCRMETRICCNTLIMIT_NORMAL,          // 0x58 //EC_code_7718
    eVAR_UCSNRCNTLIMIT,                     // 0x59
    eVAR_UCRMETRICCNT,                      // 0x5A
    eVAR_MEASURE_CNT3,                      // 0x5B
    eVAR_UCRMETRICCNTLIMIT_SPECIAL,         // 0x5C //EC_code_7718   
	eVAR_UCRMETRICCNT2,                     // 0x5D
    eVAR_FGFREEZECONFIG,                    // 0x5E
    eVAR_UCTVPNIND,                         // 0x5F
    eVAR_MEASURE_CNT2_H,                    // 0x60
    eVAR_MEASURE_CNT2_L,                    // 0x61
    eVAR_UCSYNDSET0,                        // 0x62
    eVAR_UCINDSET0,                         // 0x63
    eVAR_UCSYNDSET1,                        // 0x64
    eVAR_UCINDSET1,                         // 0x65
    eVAR_UCINDSET2,                         // 0x66
    eVAR_UCINDSET3,                         // 0x67
    eVAR_UCVTRPOWMAINPATH,                  // 0x68
    eVAR_VCR_VTR_CPO,                       // 0x69
    eVAR_VTR_CPO_MEAS_REC_OLD,              // 0x6A
    eVAR_VTR_CPO_MEAS_DIFF,                 // 0x6B
    eVAR_VTR_CPO_MEAS_DIFF_AVG_H,           // 0x6C
    eVAR_VTR_CPO_MEAS_DIFF_AVG_L,           // 0x6D
    eVAR_VTR_EQ_TPO,                        // 0x6E
    eVAR_VTR_TPO_MEAS_AVG,                  // 0x6F
    eVAR_VTR_TPO_MEAS_REC,                  // 0x70
    eVAR_VTR_TPO_MEAS_DIFF,                 // 0x71
    eVAR_SS_SNR,                            // 0x72
    eVAR_UCSERGLOBAL,                       // 0x73
    eVAR_U2SNRAVG_H,                        // 0x74
    eVAR_U2SNRAVG_L,                        // 0x75
    eVAR_UCSNRAVGNORM,                      // 0x76
    eVAR_SNR_MEAS_SHORT_REC,                // 0x77
    eVAR_SNR_MEAS_SHORT_DIFF,               // 0x78
    eVAR_SNR_MEAS_LONG_REC,                 // 0x79
    eVAR_SNR_MEAS_LONG_DIFF,                // 0x7A
    eVAR_DFE_POWER,                         // 0x7B
    eVAR_PAT_PH,                            // 0x7C
    eVAR_PAT_PH_MEAS_AVG,                   // 0x7D
    eVAR_RANGE_PRE_3DB_H,                   // 0x7E
    eVAR_RANGE_PRE_3DB_L,                   // 0x7F
    eVAR_RANGE_POST_3DB_H,                  // 0x80
    eVAR_RANGE_POST_3DB_L,                  // 0x81
    eVAR_FGSETSPARSON,                      // 0x82
    eVAR_UCSETLEAKSTS,                      // 0x83
    eVAR_UCSETCOEFSTS,                      // 0x84
    eVAR_ULELENGTH,                         // 0x85
    eVAR_UMAINTAPPOSITION,                  // 0x86
    eVAR_UDFELENGTH,                        // 0x87
    eVAR_ISTEPSIZEMAX,                      // 0x88
    eVAR_UPRACFACTOR,                       // 0x89
    eVAR_VCR_CFO_0,                         // 0x8A
    eVAR_VCR_CFO_1,                         // 0x8B
    eVAR_VPS_PILOTIND0,                     // 0x8C
    eVAR_VPS_PILOTIND1,                     // 0x8D
    eVAR_U2REFCNT_H,                        // 0x8E
    eVAR_U2REFCNT_L,                        // 0x8F
    eVAR_UCCURSPEED,                        // 0x90
    eVAR_PTR_CURR,                          // 0x91
    eVAR_PTR_STOP,                          // 0x92
    eVAR_SYNDROME_FOR_REACQ,                // 0x93
    eVAR_UCCURCFGID,                        // 0x94
    eVAR_VCR_FN_STATE,   	                // 0x95 //EC_code_D7913: For VCR FN State Indicator  
	eVAR_VCR_CFO_DEV_U,						// 0x96
	eVAR_VCR_CFO_DEV_L, 					// 0x97
	eVAR_MONITOR_CFO_DEV_2,  				// 0x98
	eVAR_MONITOR_CFO_DEV_1,					// 0x99
    eVAR_MONITOR_CFO_DEV_0,                 // 0x9A
    eVAR_UCVCRSTATE1_TO,                    // 0x9B
    eVAR_UCVCRSTATE2_TO,                    // 0x9C
    eVAR_UCVCRSTATE3_TO,                    // 0x9D
    eVAR_UCVCR_FREEZE_CFO_DEV_H,            // 0x9E
    eVAR_UCVCR_FREEZE_CFO_DEV_L,            // 0x9F
    eVAR_UCAD_TARGET_LEVEL_0,               // 0xA0
    eVAR_UCAD_TARGET_LEVEL_1,               // 0xA1
    eVAR_FGMANUAL_AD_TL,                    // 0xA2
    eVAR_UCMAGC_RF_CTRL_0,                  // 0xA3
    eVAR_UCMAGC_RF_CTRL_1,                  // 0xA4
    eVAR_A5,					            // 0xA5
    eVAR_UCVCRSTATE,                        // 0xA6
    eVAR_UCQCRFAILCNT,                      // 0xA7
    eVAR_U2ACQTIME_H,                       // 0xA8
    eVAR_U2ACQTIME_L,                       // 0xA9
    eVAR_QTR_TFO_DIFF_H,                    // 0xAA
    eVAR_QTR_TFO_DIFF_L,                    // 0xAB
    eVAR_FW_SFR,		                    // 0xAC
    eVAR_UCQTR_TFO_SET_1,                   // 0xAD
    eVAR_UCQTR_TFO_SET_2,                   // 0xAE
    eVAR_UCQTR_TFO_SET_3,                   // 0xAF
    eVAR_UC82P_FNCFG,                       // 0xB0
    eVAR_UCVCR_REACQCNT,                    // 0xB1     
    eVAR_UCINIT_VCR_FSCR,                   // 0xB2
    eVAR_UCINIT_IIR0_LEAKY,                 // 0xB3
    eVAR_UCVCR_SWR,                         // 0xB4
    eVAR_UCVPS_STATE_TO,                    // 0xB5
    eVAR_UCFNDETECT_EN,                     // 0xB6
    eVAR_UCFORCE82P_FNCFG,                  // 0xB7
    eVAR_UCFORCE_FNSTATE,                   // 0xB8
    eVAR_UCVPS_PILOT_LEVEL,                 // 0xB9
    eVAR_UCVSBDETECTINDEXTEND,              // 0xBA
    eVAR_UCVCR_PILOT_THRES, 		   		// 0xBB
    eVAR_U2VCR_CFO_DEV_H,                   // 0xBC
    eVAR_U2VCR_CFO_DEV_L,	                // 0xBD
    eVAR_MONITOR_MAX_CFO_DEV_H,             // 0xBE
    eVAR_MONITOR_MAX_CFO_DEV_L,             // 0xBF
    eVAR_UCVCR_CFO_MONCNT,                  // 0xC0
    eVAR_FW_CFG_POOL,                       // 0xC1
    eVAR_UCFORCEVSBCFG,			            // 0xC2
    eVAR_UCVCR_FSCR_DefVal,                 // 0xC3
    eVAR_UC91VSBDETECTEN,                   // 0xC4
    eVAR_UCVCRPNIND,                        // 0xC5
    eVAR_UCVFD_FREEZE_CFO_DEV_H,            // 0xC6
    eVAR_UCVFD_FREEZE_CFO_DEV_L,            // 0xC7
    eVAR_VFD_CFO_DEV_U,                     // 0xC8
    eVAR_VFD_CFO_DEV_L,                     // 0xC9
    eVAR_VCR_PN_CMETRIC_H,                  // 0xCA
    eVAR_UCMONPNIND,                        // 0xCB
    eVAR_MON_PN_CMETRIC_H,                  // 0xCC
    eVAR_UCSGP_SGCFG,                       // 0xCD
    eVAR_UCSGP_ECHOPOS,                     // 0xCE
    eVAR_UCSGP_ECHORANGE,                   // 0xCF
    eVAR_VCR_PN_STATE,                      // 0xD0 //Merge_code_d7B17: For MT5391 PN state indicator
    eVAR_UCPREV_PN_STATE,                   // 0xD1
    eVAR_UCPREV_FN_STATE,                   // 0xD2
    eVAR_UCDLY_N,                           // 0xD3
    eVAR_UCDLY_K,                           // 0xD4
    eVAR_UCDLY_M,                           // 0xD5
    eVAR_I2VALUE_TFO_BASE_H,                // 0xD6
    eVAR_I2VALUE_TFO_BASE_L,                // 0xD7
    eVAR_UCAUTOQAM_INITMOD,         		// 0xD8
    eVAR_D9,                                // 0xD9  
    eVAR_UCVCR_CFO_TH_H,                    // 0xDA  //Neo: FN/CFO detection modification (2009-06-10)
    eVAR_UCVCR_CFO_TH_L,                    // 0xDB
    eVAR_MONITOR_MAX_VCR_CFO_H,             // 0xDC
    eVAR_MONITOR_MAX_VCR_CFO_L,             // 0xDD
    eVAR_MONITOR_Min_VCR_CFO_H,             // 0xDE
    eVAR_MONITOR_Min_VCR_CFO_L,             // 0xDF
    eVAR_E0,                                // 0xE0
	eVAR_UCALT_IF_MODE,                     // 0xE1 //Ivy 2008/3/14
    eVAR_HSF_ALTIFFRQ_H,                    // 0xE2 //Ivy 2008/3/14
    eVAR_HSF_ALTIFFRQ_L,                    // 0xE3 //Ivy 2008/3/14
    eVAR_LSF_ALTIFFRQ_H,                    // 0xE4 //Ivy 2008/3/14
    eVAR_LSF_ALTIFFRQ_L,                    // 0xE5 //Ivy 2008/3/14    
	eVAR_UCGS_DEBUG,                        // 0xE6	//Ivy
    eVAR_UCGS_DEBUG2,                       // 0xE7
    eVAR_UCGS_DEBUG3,                       // 0xE8
    eVAR_UCGSSWITCH,                        // 0xE9
    eVAR_UCSNRTHRD,                         // 0xEA
    eVAR_UCGSEN_CNT,                        // 0xEB
    eVAR_UCGSPRESET,                        // 0xEC
    eVAR_UCPATTEST,                         // 0xED
    eVAR_UCGSONLY_CNT,                      // 0xEE
    eVAR_UCWAITDOPINDCNT,                   // 0xEF
    eVAR_UCMONIDOPINDCNT,                   // 0xF0
    eVAR_UCGSCFG,                           // 0xF1
    eVAR_UCGSCFG2,              			// 0xF2
    eVAR_F3,                                // 0xF3
    eVAR_UCVCR_CFO_DEV_TH_H,                // 0xF4
    eVAR_UCVCR_CFO_DEV_TH_L,                // 0xF5
    eVAR_F6,                                // 0xF6
    eVAR_UCQAMFUNCEN,                     	// 0xF7
    eVAR_UCQNIR_QCNDD_SS,     				// 0xF8  //MT5393 Ivy,                             
    eVAR_F9,                                // 0xF9
    eVAR_VPN_CmetricSel,                                // 0xFA
    eVAR_HPN_TH,                                // 0xFB
    eVAR_LPN_TH,                                // 0xFC
    eVAR_QCR_DFG,     //eVAR_FD,                                // 0xFD
    eVAR_FE,                                // 0xFE
    eVAR_Cmetric_Expr_Rest,    //eVAR_FF,                                // 0xFF
    eVAR_100,                               // 0x100
    eVAR_VPN_HTH,                               // 0x101
    eVAR_VPN_LTH,                               // 0x102
    eVAR_103_SLD_DftPGA_Ind,                               // 0x103, for legacy fixed PGA gain index default setting
    eVAR_104_fgRFTuner,                               // 0x104
    eVAR_105,                               // 0x105
    eVAR_106_U2PreACQTIME_H,                               // 0x106
    eVAR_107_U2PreACQTIME_L,                               // 0x107
    eVAR_UCVCR_PS_CFG_DefVal,                               // 0x108, /* EC_code_dB825: set as default value of VCR pilot searcher configuration 0x145 */
    eVAR_109_QARS_Ctrl,                               // 0x109
    eVAR_10A_FN_Force,                               // 0x10A
    eVAR_10B_GQ,                               // 0x10B
    eVAR_10C_GQ_LeaveMrk,                               // 0x10C
    eVAR_10D_GQ_DecUnit,                               // 0x10D
    eVAR_10E_GQ_SMR,                               // 0x10E
    eVAR_10F_ConfCnt,                               // 0x10F
    eVAR_110,                               // 0x110
    eVAR_111_VCR_LPN_TH,                               // 0x111
    eVAR_112_VCR_HPN_TH,                               // 0x112
    eVAR_113,                               // 0x113
    eVAR_114,                               // 0x114  nee to check...
    eVAR_115,                               // 0x115
    eVAR_116,                               // 0x116
    eVAR_117,                               // 0x117
    eVAR_118,                               // 0x118
    eVAR_119,                               // 0x119
    eVAR_11A,                               // 0x11A
    eVAR_11B,                               // 0x11B
    eVAR_11C,                               // 0x11C
    eVAR_11D,                               // 0x11D
    eVAR_11E,                               // 0x11E
    eVAR_11F,                               // 0x11F
    eVAR_120,                               // 0x120
    eVAR_121,                               // 0x121
    eVAR_122,                               // 0x122
    eVAR_UCSTATUSLOG3,                      // 0x123
    eVAR_UCSTATUSLOG2,                      // 0x124
    eVAR_UCSTATUSLOG1,                      // 0x125
    eVAR_126,                               // 0x126
    eVAR_127,                               // 0x127
    eVAR_128,                               // 0x128
    eVAR_129,                               // 0x129
    eVAR_U2STATUSLOG2_H,                    // 0x12A
    eVAR_U2STATUSLOG2_L,                    // 0x12B
    eVAR_U2STATUSLOG1_H,                    // 0x12C
    eVAR_U2STATUSLOG1_L,                    // 0x12D
    eVAR_UCSTATUSIND_EXTEND,                // 0x12E
    eVAR_12F,                               // 0x12F
    eVAR_SIZE,
    eVAR_END = eVAR_SIZE - 1,
// ATTENTION: Please DO NOT modify the following variable.
// This is a pointer to indicate the initializing address of data memory.
    eVAR_DATAMEM = 0x1FE
};

#define cVAR_NAME00             "PRA_State"
#define cVAR_NAME01             "PRA_Cfg"
#define cVAR_NAME02             "PRA_Ctl0"
#define cVAR_NAME03             "PRA_Ctl1"
#define cVAR_NAME04             "PRA_Ctl2"
#define cVAR_NAME05             "PRA_Ctl3"
#define cVAR_NAME06             "PRA_Slope"
#define cVAR_NAME07             "PRA_RF_Bias"
#define cVAR_NAME08             "PRA_IF_Bias"
#define cVAR_NAME09             "PRA_IF_Min_0"
#define cVAR_NAME0A             "PRA_IF_Min_1"
#define cVAR_NAME0B             "PRA_IF_Max_0"
#define cVAR_NAME0C             "PRA_IF_Max_1"
#define cVAR_NAME0D             "PRA_RF_Min_0"
#define cVAR_NAME0E             "PRA_RF_Min_1"
#define cVAR_NAME0F             "PRA_RF_Max_0"
#define cVAR_NAME10             "PRA_RF_Max_1"
#define cVAR_NAME11             "PRA_Loop_AD0"
#define cVAR_NAME12             "PRA_Loop_AD1"
#define cVAR_NAME13             "ucMAGC_En"
#define cVAR_NAME14             "ucMAGC_IF_Ctrl_0"
#define cVAR_NAME15             "ucMAGC_IF_Ctrl_1"
#define cVAR_NAME16             "u2CurAcqStateCnt_H"
#define cVAR_NAME17             "u2CurAcqStateCnt_L"
#define cVAR_NAME18             "ucTunerType"
#define cVAR_NAME19             "fgTunerAgcMap"
#define cVAR_NAME1A             "u2VAgcTimeOut_H"
#define cVAR_NAME1B             "u2VAgcTimeOut_L"
#define cVAR_NAME1C             "u2QAgcTimeOut_H"
#define cVAR_NAME1D             "u2QAgcTimeOut_L"
#define cVAR_NAME1E             "u2SigOpOfst_H"
#define cVAR_NAME1F             "u2SigOpOfst_L"
#define cVAR_NAME20             "ucCurAcqState"
#define cVAR_NAME21             "fgEnMonitor"
#define cVAR_NAME22             "fgEnReAcq"
#define cVAR_NAME23             "fgStepNextAcq"
#define cVAR_NAME24             "ucNoSigCnt"
#define cVAR_NAME25             "fgAgcFail"
#define cVAR_NAME26             "ucStatusInd"
#define cVAR_NAME27             "ucRstInd"
#define cVAR_NAME28             "ucState1Ind"
#define cVAR_NAME29             "ucState2Ind"
#define cVAR_NAME2A             "fgIsysSpecInv"
#define cVAR_NAME2B             "fgDefIsysSpecVSB"
#define cVAR_NAME2C             "fgDefIsysSpecQAM"
#define cVAR_NAME2D             "ucBrzCCfg"
#define cVAR_NAME2E             "ucAdSampCfg"
#define cVAR_NAME2F             "ucDemTrialCnt"
#define cVAR_NAME30             "ucTS_RstCnt"
#define cVAR_NAME31             "ucTS_RstCnt2"
#define cVAR_NAME32             "ucAntIfCode0"
#define cVAR_NAME33             "ucAntIfCode1"
#define cVAR_NAME34             "ucCmdChkSumErr"
#define cVAR_NAME35             "ucCmdChkSumId"
#define cVAR_NAME36             "ucTestSeqErr"
#define cVAR_NAME37             "fgAntDetEnable"
#define cVAR_NAME38             "ucAntDetStatus"
#define cVAR_NAME39             "fgMT5112_QSymbolRate"
#define cVAR_NAME3A             "ucqamADsamcfg"
#define cVAR_NAME3B             "ucQTR_TFO_1"
#define cVAR_NAME3C             "ucQTR_TFO_2"
#define cVAR_NAME3D             "ucQTR_TFO_3"
#define cVAR_NAME3E             "pnHistory" // MT5381_QCR: pnLevel max for each distinct sate
#define cVAR_NAME3F             "PreSynd"   // MT5381_QCR: QCR state
#define cVAR_NAME40             "ucAccBK"
#define cVAR_NAME41             "BN_count"
#define cVAR_NAME42             "u2AccErrPkt_H"
#define cVAR_NAME43             "u2AccErrPkt_L"
#define cVAR_NAME44             "ucAccPktCnt"
#define cVAR_NAME45             "ucErrPkt_0"
#define cVAR_NAME46             "ucErrPkt_1"
#define cVAR_NAME47             "ucQCR_STTS"
#define cVAR_NAME48             "vcr_cmetric_phdev_H"
#define cVAR_NAME49             "vcr_cmetric_phdev_L"
#define cVAR_NAME4A             "vcr_rmetric_phdev_H"
#define cVAR_NAME4B             "vcr_rmetric_phdev_L"
#define cVAR_NAME4C             "vcr_cmetric_phdev_avg_H"
#define cVAR_NAME4D             "vcr_cmetric_phdev_avg_L"
#define cVAR_NAME4E             "vcr_rmetric_phdev_avg_H"
#define cVAR_NAME4F             "vcr_rmetric_phdev_avg_L"
#define cVAR_NAME50             "pat_config_ind"
#define cVAR_NAME51             "ucSetPN"
#define cVAR_NAME52             "vcr_pilot_level"
#define cVAR_NAME53             "ucVsbDetectCfg"
#define cVAR_NAME54             "ucVsbDetectInd"
#define cVAR_NAME55             "ucVsbDetectEn"
#define cVAR_NAME56             "u2VcrRmetricThrd_H"
#define cVAR_NAME57             "u2VcrRmetricThrd_L"
#define cVAR_NAME58             "ucRmetricCntLimit_normal"//EC_code_7718
#define cVAR_NAME59             "ucSnrCntLimit"
#define cVAR_NAME5A             "ucRmetricCnt"
#define cVAR_NAME5B             "measure_cnt3"
#define cVAR_NAME5C             "ucSnrCntLimit_special"//EC_code_7718
#define cVAR_NAME5D             "ucRmetricCnt2"
#define cVAR_NAME5E             "fgFreezeConfig"
#define cVAR_NAME5F             "ucTvPnInd"
#define cVAR_NAME60             "measure_cnt2_H"
#define cVAR_NAME61             "measure_cnt2_L"
#define cVAR_NAME62             "ucSyndSet0"
#define cVAR_NAME63             "ucIndSet0"
#define cVAR_NAME64             "ucSyndSet1"
#define cVAR_NAME65             "ucIndSet1"
#define cVAR_NAME66             "ucIndSet2"
#define cVAR_NAME67             "ucIndSet3"
#define cVAR_NAME68             "ucVtrPowMainpath"
#define cVAR_NAME69             "vcr_vtr_cpo"
#define cVAR_NAME6A             "vtr_cpo_meas_rec_old"
#define cVAR_NAME6B             "vtr_cpo_meas_diff"
#define cVAR_NAME6C             "vtr_cpo_meas_diff_avg_H"
#define cVAR_NAME6D             "vtr_cpo_meas_diff_avg_L"
#define cVAR_NAME6E             "vtr_eq_tpo"
#define cVAR_NAME6F             "vtr_tpo_meas_avg"
#define cVAR_NAME70             "vtr_tpo_meas_rec"
#define cVAR_NAME71             "vtr_tpo_meas_diff"
#define cVAR_NAME72             "ss_snr"
#define cVAR_NAME73             "ucSerGlobal"
#define cVAR_NAME74             "u2SnrAvg_H"
#define cVAR_NAME75             "u2SnrAvg_L"
#define cVAR_NAME76             "ucSnrAvgNorm"
#define cVAR_NAME77             "snr_meas_short_rec"
#define cVAR_NAME78             "snr_meas_short_diff"
#define cVAR_NAME79             "snr_meas_long_rec"
#define cVAR_NAME7A             "snr_meas_long_diff"
#define cVAR_NAME7B             "dfe_power"
#define cVAR_NAME7C             "pat_ph"
#define cVAR_NAME7D             "pat_ph_meas_avg"
#define cVAR_NAME7E             "range_pre_3db_H"
#define cVAR_NAME7F             "range_pre_3db_L"
#define cVAR_NAME80             "range_post_3db_H"
#define cVAR_NAME81             "range_post_3db_L"
#define cVAR_NAME82             "fgSetSparsOn"
#define cVAR_NAME83             "ucSetLeakSts"
#define cVAR_NAME84             "ucSetCoefSts"
#define cVAR_NAME85             "uLeLength"
#define cVAR_NAME86             "uMainTapPosition"
#define cVAR_NAME87             "uDfeLength"
#define cVAR_NAME88             "iStepSizeMax"
#define cVAR_NAME89             "uPracFactor"
#define cVAR_NAME8A             "vcr_cfo_0"
#define cVAR_NAME8B             "vcr_cfo_1"
#define cVAR_NAME8C             "vps_pilotInd0"
#define cVAR_NAME8D             "vps_pilotInd1"
#define cVAR_NAME8E             "u2RefCnt_H"
#define cVAR_NAME8F             "u2RefCnt_L"
#define cVAR_NAME90             "ucCurSpeed"
#define cVAR_NAME91             "ptr_curr"
#define cVAR_NAME92             "ptr_stop"
#define cVAR_NAME93             "syndrome_for_reacq"
#define cVAR_NAME94             "ucCurCfgId"
#define cVAR_NAME95             "vcr_fn_state"
#define cVAR_NAME96             "vcr_cfo_dev_U"
#define cVAR_NAME97             "vcr_cfo_dev_L"
#define cVAR_NAME98             "monitor_cfo_dev_2"
#define cVAR_NAME99             "monitor_cfo_dev_1"
#define cVAR_NAME9A             "monitor_cfo_dev_0"
#define cVAR_NAME9B             "vcr_state1_TO"
#define cVAR_NAME9C             "vcr_state2_TO"
#define cVAR_NAME9D             "vcr_state3_TO"
#define cVAR_NAME9E             "ucvcr_freeze_cfo_dev_H"
#define cVAR_NAME9F             "ucvcr_freeze_cfo_dev_L"
#define cVAR_NAMEA0             "ucAD_Target_Level_0"
#define cVAR_NAMEA1             "ucAD_Target_Level_1"
#define cVAR_NAMEA2             "fgManual_AD_TL"
#define cVAR_NAMEA3             "ucMAGC_RF_Ctrl_0"
#define cVAR_NAMEA4             "ucMAGC_RF_Ctrl_1"
#define cVAR_NAMEA5             "Reserved"
#define cVAR_NAMEA6             "vcr_state"
#define cVAR_NAMEA7             "Ucqcrfailcnt" 
#define cVAR_NAMEA8             "u2AcqTime_H"
#define cVAR_NAMEA9             "u2AcqTime_L"
#define cVAR_NAMEAA             "u2QTR_TFO_DIFF_H"
#define cVAR_NAMEAB             "u2QTR_TFO_DIFF_L"
#define cVAR_NAMEAC             "FW_SFR"
#define cVAR_NAMEAD             "ucQTR_TFO_SET_1"
#define cVAR_NAMEAE             "ucQTR_TFO_SET_2"
#define cVAR_NAMEAF             "ucQTR_TFO_SET_3"
#define cVAR_NAMEB0             "uc82p_FNCfg"
#define cVAR_NAMEB1             "ucVCR_ReAcqCnt"
#define cVAR_NAMEB2             "ucInit_VCR_FSCR"
#define cVAR_NAMEB3             "ucInit_IIR0_Leaky"
#define cVAR_NAMEB4             "ucVCR_SWR"
#define cVAR_NAMEB5             "ucVPS_State_TO"
#define cVAR_NAMEB6             "ucFNDetect_En"
#define cVAR_NAMEB7             "ucForce82p_FNCfg"
#define cVAR_NAMEB8             "ucForce_FNState"
#define cVAR_NAMEB9             "ucVPS_Pilot_Level"
#define cVAR_NAMEBA             "ucVsbDetectIndExtend"
#define cVAR_NAMEBB             "ucVCR_Pilot_Thres"
#define cVAR_NAMEBC             "Reserved"
#define cVAR_NAMEBD             "Reserved"
#define cVAR_NAMEBE             "monitor_max_cfo_dev_H"
#define cVAR_NAMEBF             "monitor_max_cfo_dev_L"
#define cVAR_NAMEC0             "ucVcr_CFO_Moncnt"
#define cVAR_NAMEC1             "FW_CFG_POOL"
#define cVAR_NAMEC2             "ucForceVsbcfg"
#define cVAR_NAMEC3             "ucVcrFscrDefVal"
#define cVAR_NAMEC4             "uc91VsbDetectEn"
#define cVAR_NAMEC5             "ucVcrPnInd"
#define cVAR_NAMEC6             "ucvfd_freeze_cfo_dev_H"
#define cVAR_NAMEC7             "ucvfd_freeze_cfo_dev_L"
#define cVAR_NAMEC8             "vfd_cfo_dev_U"
#define cVAR_NAMEC9             "vfd_cfo_dev_L"
#define cVAR_NAMECA             "vcr_pn_cmetric_H"
#define cVAR_NAMECB             "ucMonPnInd"
#define cVAR_NAMECC             "mon_pn_cmetric_H"
#define cVAR_NAMECD             "ucSGP_SGcfg"
#define cVAR_NAMECE             "ucSGP_EchoPos"
#define cVAR_NAMECF             "ucSGP_EchoRange"
#define cVAR_NAMED0             "vcr_pn_state"
#define cVAR_NAMED1             "ucPrevPN_state"
#define cVAR_NAMED2             "ucPrevFN_state"
#define cVAR_NAMED3             "Reserved"
#define cVAR_NAMED4             "Reserved"
#define cVAR_NAMED5             "Reserved"
#define cVAR_NAMED6             "Reserved"
#define cVAR_NAMED7             "Reserved"
#define cVAR_NAMED8             "Reserved"
#define cVAR_NAMED9             "Reserved"
#define cVAR_NAMEDA             "Reserved"
#define cVAR_NAMEDB             "Reserved"
#define cVAR_NAMEDC             "Reserved"
#define cVAR_NAMEDD             "Reserved"
#define cVAR_NAMEDE             "Reserved"
#define cVAR_NAMEDF             "Reserved"
#define cVAR_NAMEE0             "Reserved"
#define cVAR_NAMEE1             "Reserved"
#define cVAR_NAMEE2             "Reserved"
#define cVAR_NAMEE3             "Reserved"
#define cVAR_NAMEE4             "Reserved"
#define cVAR_NAMEE5             "Reserved"
#define cVAR_NAMEE6             "Reserved"
#define cVAR_NAMEE7             "Reserved"
#define cVAR_NAMEE8             "Reserved"
#define cVAR_NAMEE9             "Reserved"
#define cVAR_NAMEEA             "Reserved"
#define cVAR_NAMEEB             "Reserved"
#define cVAR_NAMEEC             "Reserved"
#define cVAR_NAMEED             "Reserved"
#define cVAR_NAMEEE             "Reserved"
#define cVAR_NAMEEF             "Reserved"
#define cVAR_NAMEF0             "Reserved"
#define cVAR_NAMEF1             "Reserved"
#define cVAR_NAMEF2             "Reserved"
#define cVAR_NAMEF3             "Reserved"
#define cVAR_NAMEF4             "Reserved"
#define cVAR_NAMEF5             "Reserved"
#define cVAR_NAMEF6             "Reserved"
#define cVAR_NAMEF7             "Reserved"
#define cVAR_NAMEF8             "ucQNIR_QCNDD_SS"
#define cVAR_NAMEF9             "Reserved"
#define cVAR_NAMEFA             "Reserved"
#define cVAR_NAMEFB             "Reserved"
#define cVAR_NAMEFC             "Reserved"
#define cVAR_NAMEFD             "Reserved"
#define cVAR_NAMEFE             "Reserved"
#define cVAR_NAMEFF             "Reserved"
#define cVAR_NAME100             "Reserved"
#define cVAR_NAME101             "Reserved"
#define cVAR_NAME102             "Reserved"
#define cVAR_NAME103             "Reserved"
#define cVAR_NAME104             "Reserved"
#define cVAR_NAME105             "Reserved"
#define cVAR_NAME106             "Reserved"
#define cVAR_NAME107             "Reserved"
#define cVAR_NAME108             "ucVCR_PS_CFG_Defval"
#define cVAR_NAME109             "Reserved"
#define cVAR_NAME10A             "Reserved"
#define cVAR_NAME10B             "Reserved"
#define cVAR_NAME10C             "Reserved"
#define cVAR_NAME10D             "Reserved"
#define cVAR_NAME10E             "Reserved"
#define cVAR_NAME10F             "Reserved"
#define cVAR_NAME110             "Reserved"
#define cVAR_NAME111             "Reserved"
#define cVAR_NAME112             "Reserved"
#define cVAR_NAME113             "Reserved"
#define cVAR_NAME114             "Reserved"
#define cVAR_NAME115             "Reserved"
#define cVAR_NAME116             "Reserved"
#define cVAR_NAME117             "Reserved"
#define cVAR_NAME118             "Reserved"
#define cVAR_NAME119             "Reserved"
#define cVAR_NAME11A             "Reserved"
#define cVAR_NAME11B             "Reserved"
#define cVAR_NAME11C             "Reserved"
#define cVAR_NAME11D             "Reserved"
#define cVAR_NAME11E             "Reserved"
#define cVAR_NAME11F             "Reserved"
#define cVAR_NAME120             "Reserved"
#define cVAR_NAME121             "Reserved"
#define cVAR_NAME122             "Reserved"
#define cVAR_NAME123             "ucStatusLog3"
#define cVAR_NAME124             "ucStatusLog2"
#define cVAR_NAME125             "ucStatusLog1"
#define cVAR_NAME126             "Reserved"
#define cVAR_NAME127             "Reserved"
#define cVAR_NAME128             "Reserved"
#define cVAR_NAME129             "Reserved"
#define cVAR_NAME12A             "u2StatusLog2_H"
#define cVAR_NAME12B             "u2StatusLog2_L"
#define cVAR_NAME12C             "u2StatusLog1_H"
#define cVAR_NAME12D             "u2StatusLog1_L"
#define cVAR_NAME12E             "ucStatusIndExtend"
#define cVAR_NAME12F             "Reserved"


enum
{
  cSTATUS_RST = 0x00,                  
  cPRA_FAIL,
  cPRA_TO,
  cVCR_FAIL,
  cVTR_FAIL,
  cEQ_PRETRAIN_VTR_FAIL = 0x0F,  // 0x0F 0x3C(60)	cEQ_PRETRAIN_VTR_FAIL_FN_CLR
  cEQ_TRAIN_TO = 0x10,      // 0x10 0x40(64)	cEQ_TRAIN_TO_FN_CLR
  cEQ_TRAIN_VTR_FAIL,       // 0x11 0x44(68)	cEQ_TRAIN_VTR_FAIL_FN_CLR
  cEQ_WTRACK_VTR_FAIL,      // 0x12 
  cEQ_WTRACK_TO,            // 0x13 0x4C(76)	cEQ_WTRACK_TO_FN_CLR
  cEQ_TRACK_VTR_FAIL,       // 0x14
  cEQ_TRACK_FRAME_TO,       // 0x15 0x54(84)	cEQ_TRACK_FRAME_TO_FN_FLR
  cFEC_TRACK_VTR_FAIL,      // 0x16
  cFEC_TRACK_TO_PRA_FAIL,   // 0x17
  cFEC_TRACK_TO_VCR_FAIL,   // 0x18
  cFEC_TRACK_TO_VTR_FAIL,   // 0x19
  cFEC_BLIND_REACQ,         // 0x1A
  cFEC_TRACK_TO_HIGH_ERR,   // 0x1B 0x6C(108) 	cFEC_TRACK_TO_HIGH_ERR_FN_CLR
  cFEC_TRACK_TO_PRA_FAIL2,  // 0x1C 0x70(112)	cFEC_TRACK_TO_PRA_FAIL2_FN_CLR
  cFEC_TRACK_TO_VCR_FAIL2,  // 0x1D 0x74(116)	cFEC_TRACK_TO_VCR_FAIL2_FN_CLR
  cFEC_TRACK_TO_VTR_FAIL2,  // 0x1E 0x78(120)	cFEC_TRACK_TO_VTR_FAIL2_FN_CLR
  cFEC_TRACK_NEWCFG1,       // 0x1F
  cFEC_TRACK_NEWCFG2,       // 0x20
  cFEC_TRACK_NEWCFG3,       // 0x21
  cFEC_TRACK_TO_VTR_FAIL3,  // 0x22
  cFEC_TRACK_LOW_SNR,       // 0x23
  cFEC_TRACK_FN_CLR,		// 0x24
  cQTR_FAIL = 0x80,         // 0x80
  cQCR_TO,                  // 0x81
  cQFRAME_SPC_INV,          // 0x82
  cQFRAME_QTR_FAIL,         // 0x83
  cQFEC_TS_TO,              // 0x84
  cQFEC_TRCK_HIGH_ERR,      // 0x85
  cQFEC_TRCK_QCR_FAIL,      // 0x86
  cQFEC_TRCK_FTS_FAIL,      // 0x87
  cQFEC_TRCK_PRA_FAIL,      // 0x88
  cQFEC_TRCK_TFO, 			// 0x89
  cQFRAME_SNR_FAIL,         // 0x8A
  cQFRAME_SPC_INV_TRIAL,    // 0x8B
  cQFEC_MPER_TFO = 0x90,	// 0x90
  cQFEC_TS_TFO,				// 0x91
  cQFEC_FRAME_TFO, 			// 0x92
  cQDD_TFO,					// 0x93
  cQEQ_TFO,					// 0x94
  cQCR_TFO,					// 0x95
  cMOD_64Q = 0xA0,			// 0xA0
  cMOD_256Q, 				// 0xA1
  cQCR_TO_AFR = 0xB1, 		// 0xB1
  cQCR_TO_CAPR, 			// 0xB2
  cQCR_TO_FAPR, 			// 0xB3
  cQCR_TO_LOCK,				// 0xB4
  cVCR_STAT_FAIL = 0xC0,    // 0xC0
  cVCR_TO,                  // 0xC1
  cVPS_STAT_FAIL,           // 0xC2
  cVPS_TO,                  // 0xC3
  cTSIF_RESET1 = 0xF0,      // 0xF0
  cTSIF_RESET2,             // 0xF1
  cTSIF_RESET3,             // 0xF2
  cQCN_CFS_TO = 0xE3,		// 0xE3
  cQCN_MGMMA_TO = 0xE4,		// 0xE4
  cQCN_GMMA_TO = 0xE5,		// 0xE5
  cQCN_CFO_FAIL = 0xE6,		// 0xE6
  cVTR_LTFO_PRELOAD = 0xE0,	// 0xE0
  cVTR_LTFO_DIFF_FAIL,		// 0xE1
  cVTR_LTFO_FALSE_ALARM,	// 0xE2
  cVCR_HPN_RECHECK		    // 0xE3
};

/***********************************************************************/
#define mcGET_QUEUE(pu2QuePtr, ePosOfst)    pu2QuePtr[cCMD_REG_NUM - 2 - (ePosOfst)]
#define mcSET_QUEUE1(pu2QuePtr, ePosOfst)   pu2QuePtr[cCMD_REG_NUM - 2 - (ePosOfst)]
#define mcGET_CMD(pu2QuePtr)                pu2QuePtr[cCMD_ID_POS]

/*********** Mask & LSB Position *************************************/

#if fcMT5111_ACQ

//#define cwQTR_STAT              2
//#define cpQTR_STAT              6
#define cwQTR_STAT              1
#define cpQTR_STAT              7
#define cmQTR_STAT              mcSET_MASKS(QTR_STAT)

#define cwQCR_STAT              3
#define cpQCR_STAT              1
#define cmQCR_STAT              mcSET_MASKS(QCR_STAT)

#define cmPRA_STAT              0xC0
#define cpPRA_STAT              6

#define cwVCR_STAT              2
#define cpVCR_STAT              6
#define cmVCR_STAT              mcSET_MASKS(VCR_STAT)

#define cwVTR_STAT              2
#define cpVTR_STAT              6
#define cmVTR_STAT              mcSET_MASKS(VTR_STAT)

#else

/**************************************************************************/
/*--------- uP 0x000 -----------------------------------------------------*/
#define cpUP_IR_RD              7

/* For MT5112_REG_uP_Ctrl */
#define cpUP_CHKSUM_OK          6
#define cpUP_RESET_CHKSUM       6
#define cpUP_READY              4
#define cpUP_MEM_INIT_DONE      3
#define cpUP_CHKSUM_DONE        2
#define cpUP_RESTART_CHKSUM     2
#define cpUP_DN_FREQ            1
#define cpUP_ENABLE             0

/* For MT5112_REG_uP_Ctrl_E */
#define cpUP_ICE_ENABLE         0
/*--------- UP_CTRL_E 0x004 -----------------------------------------------*/
#define cpGPIO_MODE_0       5
#define cpGPIO_MODE_1       6
#define cpGPIO_MODE_2       7               // Share GPIO0 with dem-int

/* For MT5112_REG_uP_CMDR_CFG */
#define cpUP_CMD_RX             7
#define cpUP_CMD_RX_IE          6
#define cpUP_RSP_TX             5
#define cpUP_RSP_TX_IE          4

/*--------- I2C 0x059 ----------------------------------------------------*/
#define cpI2C_TUNER_ON          0
#define cpI2C_AUTO_INC          1

/*--------- ChipCtl 0x0D0 ------------------------------------------------*/
#define cpCHIP_PLATFORM_TYP     3
#define cwCHIP_BONDING          2
#define cpCHIP_BONDING          0
#define cmCHIP_BONDING          mcSET_MASKS(CHIP_BONDING)

#define cpCHIP_TS_Pin_En        0
#define cpCHIP_POD_Pin_En       1
#define cpCHIP_Fs_Sel           0

#define cmFAT_RST               0x01
#define cpFAT_RST               0
#define cmFDC_RST               0x01
#define cpFDC_RST               1
#define cmRDC_RST               0x01
#define cpRDC_RST               2

#define cwCHIP_TS_DRIVING       3
#define cpCHIP_TS_DRIVING       4
#define cmCHIP_TS_DRIVING       mcSET_MASKS(CHIP_TS_DRIVING)

#define cwCHIP_ID_GEN           3
#define cpCHIP_ID_GEN           5
#define cmCHIP_ID_GEN           mcSET_MASKS(CHIP_ID_GEN)
#define cwCHIP_ID_MASK          5
#define cpCHIP_ID_MASK          0
#define cmCHIP_ID_MASK          mcSET_MASKS(CHIP_ID_MASK)

/* Mask Rev. */
#define MT5112_CHIP_REV_ES      0x00
#define MT5112_CHIP_REV_MP      0x01
#define MT5112_CHIP_REV_EE      0x02
#define MT5112_CHIP_REV_EE_MP   0x03
#define MT5112_CHIP_REV_5381    0x10
#define MT5112_CHIP_REV_5382P   0x11
#define MT5112_CHIP_REV_5381MP  0x12
#define MT5112_CHIP_REV_5391    0x18

/* Test mode selection */
#define cTEST_MODE_NONE         0
#define cTEST_MODE_ICE100       1
#define cTEST_MODE_ICE128       2
#define cTEST_MODE_PEC          3
#define cTEST_MODE_OLBT         4
#define cTEST_MODE_ADCAP        5
#define cTEST_MODE_OADCAP       6
#define cTEST_MODE_ADOUT        7

#define cMT538X_TEST_MODE_I2C   5
/*--------- CHIP_Mod_RegRst 0x0DD ----------------------------------------*/
#define cpANTIF_Sel         0
#define cpOLB_MD            1
#define cpTSIF_RST_B        2

/*--------- ANTIF_2 0x0FF ------------------------------------------------*/
#define cpStart             7

/*--------- Analog 0x100 ----------------------------------------------------*/
// #define cpANA_CLK_25            6 /* Move to reg.h*/


/*--------- VFD 0x120 ----------------------------------------------------*/
#define cwVFD_LPC2 		    	3
#define cpVFD_LPC2	        	0
#define cmVFD_LPC2				mcSET_MASKS(VFD_LPC2)

/*--------- VCR 0x140 ----------------------------------------------------*/
//#define cmVCR_IIR0_LEAKY		0xC0 //EC_code_D7723: For MT5112S/MT5382P
#define cwVCR_IIR0_LEAKY        2
#define cpVCR_IIR0_LEAKY        6
#define cmVCR_IIR0_LEAKY        mcSET_MASKS(VCR_IIR0_LEAKY)
#define cwVCR_DLY_M				3
#define cpVCR_DLY_M             0
#define cmVCR_DLY_M				mcSET_MASKS(VCR_DLY_M)
#define cwVCR_IIR_BW1			3  //EC_code_d7917: For MT5391 FPGA 
#define cpVCR_IIR_BW1           4
#define cmVCR_IIR_BW1           mcSET_MASKS(VCR_IIR_BW1)
#define cwVCR_IIR_BW0			3
#define cpVCR_IIR_BW0           0
#define cmVCR_IIR_BW0           mcSET_MASKS(VCR_IIR_BW0)
#define cwVCR_STAT              2
#define cpVCR_STAT              6
#define cmVCR_STAT              mcSET_MASKS(VCR_STAT)
#define cwVCR_PILOT             4 //EC_code_6818
#define cpVCR_PILOT             4
#define cmVCR_PILOT             mcSET_MASKS(VCR_PILOT)
#define cpHTV_IND               3
#define cpLTV_IND               2
#define cpHPN_IND               1
#define cpLPN_IND               0
#define cwVCR_LPN_TH 		    3
#define cpVCR_LPN_TH	        0
#define cmVCR_LPN_TH			mcSET_MASKS(VCR_LPN_TH)
#define cwVCR_HPN_TH 		    3
#define cpVCR_HPN_TH	        3
#define cmVCR_HPN_TH			mcSET_MASKS(VCR_HPN_TH)
/* ---------VCR_PS_CFG ---------------------------------------------------*/
#define cpPS_BYP                6

/* ---------VCR_PCHK_CFG -------------------------------------------------*/
#define cpEN_FPC                3
#define cpEN_BFC                2

/* ---------VCR Delay Line -------------------------------------------------*/
#define cwDSM_RATE_SEL            2
#define cpDSM_RATE_SEL            4
#define cmDSM_RATE_SEL            mcSET_MASKS(DSM_RATE_SEL)

#define cwDLY_M            3
#define cpDLY_M            0
#define cmDLY_M            mcSET_MASKS(DLY_M)

#define cwDLY_N            3
#define cpDLY_N            0
#define cmDLY_N            mcSET_MASKS(DLY_N)

#define cwDLY_K            3
#define cpDLY_K            4
#define cmDLY_K            mcSET_MASKS(DLY_K)

/* ---------VCR_FFLL -----------------------------------------------------*/
#define cwVCR_FSCR_LPC2 		3
#define cpVCR_FSCR_LPC2	        0
#define cmVCR_FSCR_LPC2			mcSET_MASKS(VCR_FSCR_LPC2)

/*--------- VTR 0x160 ----------------------------------------------------*/
#define cwVTR_STAT              2
#define cpVTR_STAT              0
#define cmVTR_STAT              mcSET_MASKS(VTR_STAT)
#define cpVTR_MODE_SEL          4
#define cpVTR_BAD               6
#define cpVTR_FS_ABSENT         7

#define cpVTR_DD_START          4
#define cpVTR_EN_DDTR           0

#define cpVTR_THD_2_MANUAL      7

#define cwVTR_MAG_CMP_THD       3
#define cpVTR_MAG_CMP_THD       4
#define cmVTR_MAG_CMP_THD       mcSET_MASKS(VTR_MAG_CMP_THD)

#define cwVTR_THD_2             4
#define cpVTR_THD_2             0
#define cmVTR_THD_2             mcSET_MASKS(VTR_THD_2)

#define cwVTR_POW_VAR_MARGIN    3
#define cpVTR_POW_VAR_MARGIN    0
#define cmVTR_POW_VAR_MARGIN    mcSET_MASKS(VTR_POW_VAR_MARGIN)

#define cwVTR_EQTPO_H           8
#define cwVTR_EQTPO_L           8


#define cwVTR_MP_POW_H          4
#define cpVTR_MP_POW_H          0
#define cmVTR_MP_POW_H          mcSET_MASKS(VTR_MP_POW_H)
#define cwVTR_MP_POW_L          8

//#define cwVTR_SEG_PREF          2
#define cwVTR_SEG_PREF          4
#define cpVTR_SEG_PREF          0
#define cmVTR_SEG_PREF          mcSET_MASKS(VTR_SEG_PREF)

#define cpVTR_ROTOR_EN          7
#define cpVTR_ROTOR_UPDATE      6

#define cwVTR_PROBE_0_H         8
#define cwVTR_PROBE_0_L         4
#define cwVTR_PROBE_1_H         4
#define cwVTR_PROBE_1_L         8

/*--------- QTR 0x180 ----------------------------------------------------*/
#define cwQTR_STAT              2
#define cpQTR_STAT              6
#define cmQTR_STAT              mcSET_MASKS(QTR_STAT)
#define cwQTR_STAT1             1
#define cpQTR_STAT1             (cpQTR_STAT + 1)
#define cmQTR_STAT1             mcSET_MASKS(QTR_STAT1)

#define cwQTR_KCONST            2
#define cpQTR_KCONST            4
#define cmQTR_KCONST            mcSET_MASKS(QTR_KCONST)
/*--------- QCR 0x190 ----------------------------------------------------*/
#define cwQCR_STAT              3
#define cpQCR_STAT              1
#define cmQCR_STAT              mcSET_MASKS(QCR_STAT)
/*--------- QCN 0x3CB ----------------------------------------------------*/
#define cwQCN_FSM         		3
#define cpQCN_FSM      		    0
#define cmQCN_FSM   	        mcSET_MASKS(QCN_FSM)
#define cwQCN_FORCE             2
#define cpQCN_FORCE             2
#define cmQCN_FORCE             mcSET_MASKS(QCN_FORCE)
#define cpQCN_STAY_SLEEP	    0
#define cpQCN_FORCE_QEQ_DIS     2
#define cpQCN_FORCE_QCR_DIS     (cpQCN_FORCE_QEQ_DIS+1)
#define cpQCN_STAY_DD			6

/*--------- NIR 0x1B0 ----------------------------------------------------*/
#define cpNIR_EN                7

#define cwNIR_SPEED             3
#define cpNIR_SPEED             3
#define cmNIR_SPEED             mcSET_MASKS(NIR_SPEED)

/*--------- PAT 0x1C0 ----------------------------------------------------*/
#define cpPAT_ACTIVE            2

#define cwPAT_C1_DDTR_CFG       3
#define cpPAT_C1_DDTR_CFG       4
#define cmPAT_C1_DDTR_CFG       mcSET_MASKS(PAT_C1_DDTR_CFG)

#define cwPAT_CG_NORM_IDX       3
#define cpPAT_CG_NORM_IDX       4
#define cmPAT_CG_NORM_IDX       mcSET_MASKS(PAT_CG_NORM_IDX)
#define cwPAT_CG_SLOW_IDX       3
#define cpPAT_CG_SLOW_IDX       2
#define cmPAT_CG_SLOW_IDX       mcSET_MASKS(PAT_CG_SLOW_IDX)

#define cwPAT_CP_NORM_IDX       3
#define cpPAT_CP_NORM_IDX       4
#define cmPAT_CP_NORM_IDX       mcSET_MASKS(PAT_CP_NORM_IDX)
#define cwPAT_CP_SLOW_IDX       3
#define cpPAT_CP_SLOW_IDX       2
#define cmPAT_CP_SLOW_IDX       mcSET_MASKS(PAT_CP_SLOW_IDX)

/*--------- VPS 0x1D0 ----------------------------------------------------*/
#define cwVPS_STAT              2
#define cpVPS_STAT              6
#define cmVPS_STAT              mcSET_MASKS(VPS_STAT)

/*--------- ISYS 0x1E0 ---------------------------------------------------*/
#define cmISYS_Mode             0x03
#define cpISYS_Mode             0
//#define ISYS_MD_MASK            cmISYS_Mode

#define cpISYS_RST3_TS          4
#define cpISYS_RST3_TSIF        5

#define cpISYS_REGRST2_TSIF     5

#define cpPRA_RST               2
#define cpVCR_RST               3
#define cpVTR_RST               4

#define cpEQ_RST                1
#define cpPAT_RST               5

/*--------- PAT 0x1F0 ----------------------------------------------------*/

/*--------- EQL 0x200 ----------------------------------------------------*/
#define cpEQL_LEN               4
#define cpEQL_LEN_LSB           0

#define cwEQL_STEP_SIZE         4
#define cpEQL_STEP_SIZE         4
#define cmEQL_STEP_SIZE         mcSET_MASKS(EQL_STEP_SIZE)

#define cwEQL_LEAKAGE_STEP      2
#define cpEQL_LEAKAGE_STEP      4
#define cmEQL_LEAKAGE_STEP      mcSET_MASKS(EQL_LEAKAGE_STEP)
#define cpEQL_SPARSE_EN         1
#define cpEQL_LEAKAGE_EN        0

#define cwEQL_SS_SNR_H          8
#define cwEQL_SS_SNR_L          5
#define cwEQL_SS_SNR_INT        6

#define cpEQ_DIS_UP_STAT        4
#define cwEQ_UP_STAT            4
#define cpEQ_UP_STAT            0
#define cmEQ_UP_STAT            mcSET_MASKS(EQ_UP_STAT)

#define cpEQ_DET_START          0

// MT5387 GS
#define cwGS_TYPE          		2
#define cpGS_TYPE          		6
#define cmGS_TYPE          		mcSET_MASKS(GS_TYPE)

#define cwGS_INDEX_H          	3
#define cwGS_INDEX_L          	8

#define cpGS_TABLE_RDY          4

#define cwGS_TABLE_CNT          5
#define cpGS_TABLE_CNT          0
#define cmGS_TABLE_CNT          mcSET_MASKS(GS_TABLE_CNT)

/*--------- QEQ 0x260 ----------------------------------------------------*/
/*--------- EQR 0x280 ----------------------------------------------------*/
#define cwEQR_TRN_CONS_LEVEL      5
#define cpEQR_TRN_CONS_LEVEL      0
#define cmEQR_TRN_CONS_LEVEL      mcSET_MASKS(EQR_TRN_CONS_LEVEL)

#define cwEQR_TRK_CONS_LEVEL      5
#define cpEQR_TRK_CONS_LEVEL      0
#define cmEQR_TRK_CONS_LEVEL      mcSET_MASKS(EQR_TRK_CONS_LEVEL)

#define cwEQR_STEP_SIZE         4
#define cpEQR_STEP_SIZE         4
#define cmEQR_STEP_SIZE         mcSET_MASKS(EQR_STEP_SIZE)

#define cwEQR_LEAKAGE_STEP      2
#define cpEQR_LEAKAGE_STEP      4
#define cmEQR_LEAKAGE_STEP      mcSET_MASKS(EQR_LEAKAGE_STEP)
#define cpEQR_LEAKAGE_EN        3

#define cpEQR_SPARSE_EN         2

/*--------- QDR 0x300 ----------------------------------------------------*/
/*--------- CPR 0x310 ----------------------------------------------------*/

/*--------- DC 0x319 ----------------------------------------------------*/
#define cpDC_SPEC_INV           0

/*--------- INT 0x330 ----------------------------------------------------*/
/*--------- PrA 0x350 ----------------------------------------------------*/
#define cmPRA_STAT              0x30
#define cpPRA_STAT              4

#define cwPRA_LOOP_SFH          8
#define cwPRA_LOOP_SFL          3

#define cwPRA_TARG_LEVEL_AD_L   3

#define cwPRA_ACC_TIME_OPC      2
#define cpPRA_ACC_TIME_OPC      6
#define cmPRA_ACC_TIME_OPC      mcSET_MASKS(PRA_ACC_TIME_OPC)

#define cwPRA_PROBE_SEL         4
#define cpPRA_PROBE_SEL         0
#define cmPRA_PROBE_SEL         mcSET_MASKS(PRA_PROBE_SEL)

#define cwPRA_TARG_LEVEL_SF_L   3
#define cwPRA_CUR_MAG_NIR_L     4
#define cwPRA_W_ADC_0_H         6
#define cwPRA_W_ADC_1_L         8

/* Probe signal selection */
#define cCUR_LEVEL_AD           0
#define cCUR_LEVEL_SF           1
#define cCUR_LEVEL_OPC1         2
#define cLOOP_OP                4
#define cW_ADC                  5
#define cW_ADC_EXP              6
#define cRF_BOOST               7
#define cIF_BOOST               8
#define cCUR_AD_DC              9

/* Reg0x354 Bit Definition*/
#define cpPRA_CFG_RF_EN         3 
#define cpPRA_CFG_IF_EN         1

/*--------- QNIR 0x3D0 ----------------------------------------------------*/  //Ivy MT5393 D8417
#define cwQNIR_TRK_STEP         4
#define cpQNIR_TRK_STEP         0
#define cmQNIR_TRK_STEP         mcSET_MASKS(QNIR_TRK_STEP)


/*--------- DEP 0x400 ----------------------------------------------------*/
#define cpDEP_SER_RDY           6
#define cwDEP_SER_H             5
#define cwDEP_SER_L             8
#define cwDEP_SER_INT           3

/*--------- FTS 0x410 ----------------------------------------------------*/
#define cpFTS_FS_OK             7

/*--------- TS 0x420 -----------------------------------------------------*/
#define cpTS_STAT               0

/*--------- TSIF 0x430 ---------------------------------------------------*/
#define cpTSIF_CFG_TEI_ERR      7
#define cpTSIF_CFG_VALID        6
#define cpTSIF_CFG_ERR          5
#define cpTSIF_CFG_SYNC         4
#define cpTSIF_CFG_TEI          3
#define cpTSIF_CFG_SP           2
#define cpTSIF_CFG_CLK_GATE     1
#define cpTSIF_CFG_CLK_INV      0

#define cwTSIF_CNT_PRD          7
#define cpTSIF_CNT_PRD          0
#define cmTSIF_CNT_PRD          mcSET_MASKS(TSIF_CNT_PRD)
#define cwTSIF_ERR_CNT          5
#define cpTSIF_ERR_CNT          0
#define cmTSIF_ERR_CNT          mcSET_MASKS(TSIF_ERR_CNT)
#define cpTSIF_ERR_RDY          7
#define cpTSIF_ERR_MIS          6
#define cpTSIF_ERR_OVF          5

#define cpTSIF_ADR_CNFLICT      2

/*----------TS 0x446 -----------------------------------------------------*/
// #if fcTSIF_MODIFY
#define cpTS_OUT_EN             3
//#endif

/*--------- RS 0x450 -----------------------------------------------------*/
#define cpRS_FS_Stat            0

/*--------- SMEM 0x470 ---------------------------------------------------*/
#define cpSMEM_CAPT_RUNNING     0

#define cmSMEM_DATAPATH_SEL     0x30
#define cpSMEM_DATAPATH_SEL     4
#define cmSMEM_CAPTURE_SEL      0x07
#define cpSMEM_CAPTURE_SEL      0

#define cSMEM_SEL_DISABLE       0
#define cSMEM_SEL_EQ_OUTPUT     1
#define cSMEM_SEL_CHIR          2
#define cSMEM_SEL_BASEBAND      3
#define cSMEM_SEL_UP_MEMORY     4
#define cSMEM_SEL_MEMORY_INIT   7

#define cpSMEM_LIMIT_EN         2
#define cpSMEM_CAPT_START       1

/*--------- RSICIF 0x4A4 ---------------------------------------------------*/
#define cpRISCIF_HOST_EN        0

/*--------- OLBT 0x8A1 ---------------------------------------------------*/
#define cpOLBT_ERR_RDY          7
#define cpOLBT_ERR_MISS         6

/*--------- OSYS 0x8B0 ---------------------------------------------------*/
#define cwOPRA_STAT             2
#define cpOPRA_STAT             6
#define cmOPRA_STAT             mcSET_MASKS(OPRA_STAT)
#define cwOTR_STAT              2
#define cpOTR_STAT              3
#define cmOTR_STAT              mcSET_MASKS(OTR_STAT)
#define cwOTR_STAT1             1
#define cpOTR_STAT1             (cpOTR_STAT + 1)
#define cmOTR_STAT1             mcSET_MASKS(OTR_STAT1)
#define cwOCR_STAT              2
#define cpOCR_STAT              0
#define cmOCR_STAT              mcSET_MASKS(OCR_STAT)

#define cmOSYS_Mode             0x03
#define cpOSYS_Mode             0
//#define OSYS_MD_MASK            cmOSYS_Mode

// OEQ_STTS
#define EQ_W_IDX_MASK           0x3C

/*--------- RDC 0x8D0 ---------------------------------------------------*/
#define cwRSYS_MD               2
#define cpRSYS_MD               0
#define cmRSYS_MD               mcSET_MASKS(RSYS_MD)

#endif

#define cpBZC                   0
#define cpR22_CH2               1
#define cpR22_CH3               2
#define cpR22_SET               4

//#if fcGS
#define cpR21_CH1_2_3           0
#define cpR21_CH4				1
#define cpR1					2
#define cpSlowCRC				3
#define cpFastCRC				4
#define fgFixR1					7

#define fgR1detect				0

#define GS_ChipInit				0
#define GS_VSBAcqInit			1
#define GS_MeasureInit			2

//#endif
#define cpSGP_SG_3DB          0
#define cpSGP_SG_6DB          1
#define cpSGP_SG_9DB          2
#define cpSGP_SG_ALL          3

/***********************************************************************/
/* For DEMOD_ACQ_STATUS Bit definition */
#define cpACQ_STATUS_AGC        5
#define cpACQ_STATUS_CR         4
#define cpACQ_STATUS_TR         3
#define cpACQ_STATUS_EQ         2
#define cpACQ_STATUS_1ST_FRAME  1
#define cpACQ_STATUS_1ST_TS_PKT 0

/* For DEMOD_ACQ_STATUS_FDC Bit definition */
#define cpACQ_STATUS_OPRA       2
#define cpACQ_STATUS_OTR        1
#define cpACQ_STATUS_OCR        0

//For MT5112S_FN FW flag Bit definition		 //EC_code_D7723: For MT5112S/MT5382P
/**************************************************************************/
/* For Vsb Detect Config Bit definition */
#define cpFN_Cfg                0
#define cpPN_Cfg                1
#define cpTV_Cfg                2
#define cpR22_Cfg               3
#define cp82FN_cfg				4           // For MT5382p FN / burstFN
    #define cNORMAL_CFG1        0
    #define cFN_CFG2            1    
#define cp82DBW_cfg             5
#define cp91HPN_cfg				6
#define cp91BN_cfg				7

/* For Vsb FN Detect Bit definition */
#define cpCFO_Dev_Detect        0
#define cpBurst_FN_Detect       1
#define cpFN_State_Set          6
#define cpForce_FN_Cfg          7

/* For VCR FW_SFR */
#define cpFW_SFR_PS_BYP         6
#define cpFW_SFR_EN_FPC         3
#define cpFW_SFR_EN_BFC         2

/* VSB Detection Indicator Extend 0x5D */
//		cpFN_Cfg				0	FN indicator at tracking state
#define cp_fgCHIR_Conflict		1
#define cp_fgINT_ITFO_PL 		2   // VSB INT with initial TFO preloaded

#define cp91FFLL2_cfg			0
#define cp91FNMon_cfg			1
#define cp91PNMon_cfg			2
#define cp91FN_cfg              3
#define cpSGP_Cfg               4  // For SGP Single echo, High doppler case

/*FW_SFR 0xFFAC*/
#define cpFW_SFR_fgSlowAcqAid           0
#define cpFW_SFR_fgFrequencyNoise       1
#define cpFW_SFR_fgEQChkMore            2
#define cpFW_SFR_fgQCRCtrl              3
#define cpFW_SFR_fgQamAutoDetect    	4
#define cpFW_SFR_fgQamTunerACI    		5
#define cpFW_SFR_fgQCR_PNFrz    		6
#define cpFW_SFR_fgQamSpecDriver    	7

/*FW_CFG_POOL 0xFFC1*/
#define cpFW_CFG_POOL_fgTTC_Cfg			0
#define cpFW_CFG_POOL_fgHPN_Aux			1
#define cpFW_CFG_POOL_fgSpecInvFix  	2    //Added for control of spectrum inverse. (Neo, 2008/4/21)
#define cpFW_CFG_POOL_fgGS_Enable      	3    //Neo: flag to enable MT5387 GS or not. (2009-3-17)
#define cpFW_CFG_POOL_fgGS_R1Enable     4    //Neo: flag to enable MT5387 GS R1 configuration or not. (2009-3-27)

/* For Manual AGC Enable Bit Definition */
#define cpMAGC_RF_EN            0
#define cpMAGC_IF_EN            1

/* QAM Function Enable */
#define cpQDFE_En            0	
#define cpQNIR_En            1

#endif  // COM_DEFS_H

/***********************************************************************/
/*              Version Control                                        */
/***********************************************************************/
/*
   $Log: Com_defs.h $
   Revision 1.0  2005/04/26 13:18:24 JWEI
*/
