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

#ifndef _MT5135C_COM_DEFS_H
#define _MT5135C_COM_DEFS_H

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define fcADD_MSG_TIME          0

/***********************************************************************/
#define ccCMD_REG_NUM            8
#define ccCMD_ID_POS             (ccCMD_REG_NUM - 1)
#define ccFW_RUN_TAG             0x66
#define ccFW_WDT_TAG             0x99
//#define ccFW_ADV_TAG             0x55
//#define ccMSG_TAG                0xDD
//#define ccSIG_LVL_BOUND          150

/***********************************************************************/
#ifndef ccCMD_ID_DEMOD_RESET
#define ccCMD_ID_DEMOD_RESET     0x10
enum
{
    ccOFST_DEMOD_RESET_STATUS,
    ccOFST_DEMOD_RESET_SIZE
};
#endif

#ifndef ccCMD_ID_TUNER_I2C_ONOFF
#define ccCMD_ID_TUNER_I2C_ONOFF 0x13
enum
{
    ccOFST_TUNER_I2C_ONOFF,
    ccOFST_TUNER_I2C_DEGLITCH,
    ccOFST_TUNER_I2C_PULLUP_DEL,
    ccOFST_TUNER_I2C_ONOFF_SIZE,
    ccOFST_TUNER_I2C_ONOFF_STATUS = ccOFST_TUNER_I2C_ONOFF,
    ccOFST_TUNER_I2C_ONOFF_RSIZE
};
#endif

#ifndef ccCMD_ID_SET_ANALOG
#define ccCMD_ID_SET_ANALOG      0x14
enum
{
    ccOFST_SET_ANALOG_PD_MODE,
    ccOFST_SET_ANALOG_CKGEN_CTRL,
    ccOFST_SET_ANALOG_PGA_CTRL_1,
    ccOFST_SET_ANALOG_PGA_CTRL_2,
    ccOFST_SET_ANALOG_ADC_CTRL_1,
    ccOFST_SET_ANALOG_ADC_CTRL_2,
    ccOFST_SET_ANALOG_SADC_CTRL,
    ccOFST_SET_ANALOG_SIZE
};
#endif

#ifndef ccCMD_ID_SET_TSIF
#define ccCMD_ID_SET_TSIF        0x15
enum
{
    ccOFST_SET_TSIF_FORMAT,
    ccOFST_SET_TSIF_CTRL,
    ccOFST_SET_TSIF_RATE,
    ccOFST_SET_TSIF_SIZE
};
#endif

#ifndef ccCMD_ID_SET_TUNER_PARA0
#define ccCMD_ID_SET_TUNER_PARA0 0x16
enum
{
    ccOFST_SET_TUNER_PARA0_SLP,
    ccOFST_SET_TUNER_PARA0_INT,
    ccOFST_SET_TUNER_PARA0_MAX,
    ccOFST_SET_TUNER_PARA0_MIN,
    ccOFST_SET_TUNER_PARA0_SIZE
};
#endif

#ifndef ccCMD_ID_SET_TUNER_PARA1
#define ccCMD_ID_SET_TUNER_PARA1 0x17
enum
{
    ccOFST_SET_TUNER_PARA1_TARGET_POW,
    ccOFST_SET_TUNER_PARA1_LF_CTRL,
    ccOFST_SET_TUNER_PARA1_IF_FREQ0,
    ccOFST_SET_TUNER_PARA1_IF_FREQ1,
    ccOFST_SET_TUNER_PARA1_SIZE
};
#endif

/*
#define ccCMD_ID_SET_TUNER_PARA2 0x18
enum
{
    ccOFST_SET_TUNER_PARA2_0,
    ccOFST_SET_TUNER_PARA2_1,
    ccOFST_SET_TUNER_PARA2_2,
    ccOFST_SET_TUNER_PARA2_3,
    ccOFST_SET_TUNER_PARA2_SIZE
};  
*/
#ifndef ccCMD_ID_SET_BW1
#define ccCMD_ID_SET_BW1          0x19
enum
{
    ccOFST_SET_BW1_PARA1,
    ccOFST_SET_BW1_PARA2,
    ccOFST_SET_BW1_PARA3,
    ccOFST_SET_BW1_PARA4,
    ccOFST_SET_BW1_PARA5,
    ccOFST_SET_BW1_PARA6,
    ccOFST_SET_BW1_PARA7,
    ccOFST_SET_BW1_SIZE
};
#endif

#ifndef ccCMD_ID_SET_BW2
#define ccCMD_ID_SET_BW2          0x1A
enum
{
    ccOFST_SET_BW2_PARA8,
    ccOFST_SET_BW2_PARA9,
    ccOFST_SET_BW2_PARA10,
    ccOFST_SET_BW2_PARA11,
    ccOFST_SET_BW2_PARA12,
    ccOFST_SET_BW2_BW,   // add parameters before this line
    ccOFST_SET_BW2_IIR,  // 0:CCI notch , 1:ANIR
    ccOFST_SET_BW2_SIZE
};
#endif

// Internal use
#ifndef ccCMD_ID_START_IFFT_PLOT
#define ccCMD_ID_START_IFFT_PLOT   0x1B
// Internal use
#define ccCMD_ID_STOP_IFFT_PLOT    0x1C

#define ccCMD_ID_START_DRIVER      0x1D
enum
{
    ccOFST_SET_DRIVER_MON1,
    ccOFST_SET_DRIVER_MON2,// add parameters before this line
    ccOFST_SET_DRIVER_SIZE
};
#endif

#define ccCMD_ID_STOP_DRIVER       0x1E

#ifndef ccCMD_ID_CCI_NOTCH
#define ccCMD_ID_CCI_NOTCH         0x1F
enum
{
    ccOFST_CCI_NOTCH_IDX,
    ccOFST_CCI_NOTCH_COEF0,
    ccOFST_CCI_NOTCH_COEF1,
    ccOFST_CCI_NOTCH_COEF2,
    ccOFST_CCI_NOTCH_COEF3,
    ccOFST_CCI_NOTCH_COEF4,
    ccOFST_CCI_NOTCH_COEF5,
    ccOFST_CCI_NOTCH_RSIZE
};
#endif

#ifndef ccCMD_ID_DEMOD_GET_STATUS1
#define ccCMD_ID_DEMOD_GET_STATUS1 0x20
enum
{
    ccOFST_DEMOD_STATUS1_IF_AGC,
    ccOFST_DEMOD_STATUS1_RSSI,        // not used, reserved
    ccOFST_DEMOD_STATUS1_SC_STATE,
    ccOFST_DEMOD_STATUS1_AGC_LOCK,
    ccOFST_DEMOD_STATUS1_MD_FFTMODE,
    ccOFST_DEMOD_STATUS1_TPS_LOCK,
    ccOFST_DEMOD_STATUS1_TS_LOCK,
    ccOFST_DEMOD_STATUS1_SIZE
};
#endif

#ifndef ccCMD_ID_DEMOD_GET_INFO1
#define ccCMD_ID_DEMOD_GET_INFO1 0x21
enum
{
    ccOFST_DEMOD_INFO1_VER0,
    ccOFST_DEMOD_INFO1_VER1,
    ccOFST_DEMOD_INFO1_MD_STATUS0,
    ccOFST_DEMOD_INFO1_TPS_REG0,
    ccOFST_DEMOD_INFO1_TPS_REG1,
    ccOFST_DEMOD_INFO1_TPS_REG5,
    ccOFST_DEMOD_INFO1_CHANL_SEL,
    ccOFST_DEMOD_INFO1_SIZE
};
#endif

// Internal use
#ifndef ccCMD_ID_DEMOD_GET_INFO2
#define ccCMD_ID_DEMOD_GET_INFO2 0x22
enum
{
    ccOFST_DEMOD_INFO2_TPS_TIME0,
    ccOFST_DEMOD_INFO2_TPS_TIME1,
    ccOFST_DEMOD_INFO2_ACQ_TIME0,
    ccOFST_DEMOD_INFO2_ACQ_TIME1,
    ccOFST_DEMOD_INFO2_AR_COEF,

    // not completed
    ccOFST_DEMOD_INFO2_MD_TIME0,
    ccOFST_DEMOD_INFO2_CE_TIME0,
    ccOFST_DEMOD_INFO2_SIZE
};
#endif

// Internal use
#ifndef ccCMD_ID_DEMOD_GET_INFO3
#define ccCMD_ID_DEMOD_GET_INFO3 0x23
enum
{
    ccOFST_DEMOD_INFO3_AGC_IF_OP2,
    ccOFST_DEMOD_INFO3_AGC_RF_OP2,
    ccOFST_DEMOD_INFO3_DAGC_GAIN,
    ccOFST_DEMOD_INFO3_SNR_FIXED, // not used, reserved
    ccOFST_DEMOD_INFO3_PREV_BER0,
    ccOFST_DEMOD_INFO3_PREV_BER1,
    ccOFST_DEMOD_INFO3_PREV_BER2,
    
    ccOFST_DEMOD_INFO3_SIZE
};
#endif

#ifndef ccCMD_ID_DEMOD_GET_INFO4
#define ccCMD_ID_DEMOD_GET_INFO4 0x24
enum
{
    ccOFST_DEMOD_INFO4_RS_COR_BIT0,
    ccOFST_DEMOD_INFO4_RS_COR_BIT1,
    ccOFST_DEMOD_INFO4_RS_COR_BIT2,
    ccOFST_DEMOD_INFO4_RS_ERR_PKT0,
    ccOFST_DEMOD_INFO4_RS_ERR_PKT1,
    ccOFST_DEMOD_INFO4_RS_TOT_PKT0,
    ccOFST_DEMOD_INFO4_RS_TOT_PKT1,
    
    ccOFST_DEMOD_INFO4_SIZE
};
#endif

// Internal use
#ifndef ccCMD_ID_DEMOD_GET_INFO5
#define ccCMD_ID_DEMOD_GET_INFO5 0x25
enum
{
//  ccOFST_DEMOD_INFO5_TIMER0,
//  ccOFST_DEMOD_INFO5_TIMER1,
    ccOFST_DEMOD_INFO5_WAD_MODE,
    ccOFST_DEMOD_INFO5_RFAGC_STATE,
    ccOFST_DEMOD_INFO5_RESET_COUNT,
    ccOFST_DEMOD_INFO5_ACQ_COUNT,
    
    ccOFST_DEMOD_INFO5_SIZE
};
#endif

// Internal use
#ifndef ccCMD_ID_DEMOD_GET_INFO6
#define ccCMD_ID_DEMOD_GET_INFO6 0x26
enum
{
    ccOFST_DEMOD_INFO6_IFS_ADJ_LO,
    ccOFST_DEMOD_INFO6_IFS_ADJ_HI,
    ccOFST_DEMOD_INFO6_MD_STATUS0,
    ccOFST_DEMOD_INFO6_MD_STATUS1,
    ccOFST_DEMOD_INFO6_FTT_LF_FREQ0,
    ccOFST_DEMOD_INFO6_FTT_LF_FREQ1,
    ccOFST_DEMOD_INFO6_FTT_LF_FREQ2,
    ccOFST_DEMOD_INFO6_SIZE
};
#endif

// Internal use
#ifndef ccCMD_ID_DEMOD_GET_INFO7
#define ccCMD_ID_DEMOD_GET_INFO7 0x27
enum
{
    ccOFST_DEMOD_INFO7_CHANL_SEL,
    ccOFST_DEMOD_INFO7_FTT_LF_TIME0,
    ccOFST_DEMOD_INFO7_FTT_LF_TIME1,
    ccOFST_DEMOD_INFO7_FTT_CPE_ADJ0,
    ccOFST_DEMOD_INFO7_FTT_CPE_ADJ1,
    ccOFST_DEMOD_INFO7_TOPE_MAX_INDEX,
    //ccOFST_DEMOD_INFO7_RFARC_WBPD, 
    //ccOFST_DEMOD_INFO7_RFARC_INB, 
    ccOFST_DEMOD_INFO7_SIZE
};
#endif

// Internal use
#define ccCMD_ID_DEMOD_GET_INFO8 0x28
enum
{
    ccOFST_DEMOD_INFO8_INR_ON_CNT,
    ccOFST_DEMOD_INFO8_INR_TIME_OUT_CNT,
    ccOFST_DEMOD_INFO8_INR_ON_TO_CNT,
    ccOFST_DEMOD_INFO8_WND_DRIFT0,
    ccOFST_DEMOD_INFO8_WND_DRIFT1,
    ccOFST_DEMOD_INFO8_SNR_TPS_AR0, // momo, 060901, add new SNR indicator
    ccOFST_DEMOD_INFO8_SNR_TPS_AR1, 
    ccOFST_DEMOD_INFO8_SIZE
};

// Internal use
#define ccCMD_ID_DEMOD_GET_INFO9 0x29
enum
{
    //ccOFST_DEMOD_INFO9_SNR_CSI_L,
    //ccOFST_DEMOD_INFO9_SNR_CSI_H,
    ccOFST_DEMOD_INFO9_TOTAL_PEAK,
    ccOFST_DEMOD_INFO9_TOTAL_COMB,
    ccOFST_DEMOD_INFO9_BEST_COMB,
    ccOFST_DEMOD_INFO9_IFFT_DIFF0,
    ccOFST_DEMOD_INFO9_IFFT_DIFF1,
    ccOFST_DEMOD_INFO9_SIZE
};

#ifndef ccCMD_ID_DEMOD_GET_INFO10
#define ccCMD_ID_DEMOD_GET_INFO10 0x2A
enum
{
    ccOFST_DEMOD_INFO10_SNR_CSI_L,
    ccOFST_DEMOD_INFO10_SNR_CSI_H,
    ccOFST_DEMOD_INFO10_CID_L,
    ccOFST_DEMOD_INFO10_CID_H,
    ccOFST_DEMOD_INFO10_RFARC_WBPD,
    ccOFST_DEMOD_INFO10_RFARC_INB,
    ccOFST_DEMOD_INFO10_SIZE
};
#endif

// Internal use
#ifndef ccCMD_ID_DEMOD_GET_INFO11
#define ccCMD_ID_DEMOD_GET_INFO11 0x2B
enum
{
    ccOFST_DEMOD_INFO11_RS_PKT0,
    ccOFST_DEMOD_INFO11_RS_PKT1,
    ccOFST_DEMOD_INFO11_RS_PKT2,
    ccOFST_DEMOD_INFO11_RS_PKT3,
    ccOFST_DEMOD_INFO11_RS_PKT4,
    ccOFST_DEMOD_INFO11_RS_PKT5,
    ccOFST_DEMOD_INFO11_RS_PKT6,

    ccOFST_DEMOD_INFO11_SIZE
};
#endif

// Internal use
#ifndef ccCMD_ID_DEMOD_GET_IFFT
#define ccCMD_ID_DEMOD_GET_IFFT  0x2C
enum
{
    ccOFST_DEMOD_IFFT_INDEX,   // input parameters
    ccOFST_DEMOD_IFFT_TSIZE,
    // return values
    ccOFST_DEMOD_IFFT_POS_L = ccOFST_DEMOD_IFFT_INDEX,
    ccOFST_DEMOD_IFFT_POS_H,
    ccOFST_DEMOD_IFFT_VAL_L,
    ccOFST_DEMOD_IFFT_VAL_H,
    ccOFST_DEMOD_IFFT_RSIZE
};
#endif

// Internal use
#ifndef ccCMD_ID_DEMOD_SET_REG
#define ccCMD_ID_DEMOD_SET_REG   0x30
enum
{
    ccOFST_REG_NUM,
    ccOFST_REG1_ADDR_L,
    ccOFST_REG1_ADDR_H,
    ccOFST_REG1_DATA,
    ccOFST_REG1_SIZE,
    ccOFST_REG2_DATA = ccOFST_REG1_SIZE,
    ccOFST_REG3_DATA,
    ccOFST_REG4_DATA,
    ccOFST_REG4_SIZE
//    ccOFST_REG2_ADDR_L = cOFST_REG1_SIZE,
//    ccOFST_REG2_ADDR_H,
//    ccOFST_REG2_DATA,
//    ccOFST_REG2_SIZE
};
#endif

// Internal use
#ifndef ccCMD_ID_DEMOD_CFG
#define ccCMD_ID_DEMOD_CFG       0x31
enum
{
    ccOFST_CFG_TYPE,
    ccOFST_CFG_ENABLE,
    ccOFST_CFG_NUM,
//    ccOFST_CFG_ADDR_H,
    ccOFST_CFG_SIZE
};
#endif

// Internal use
#ifndef ccCMD_ID_DEMOD_CFG_PARA
#define ccCMD_ID_DEMOD_CFG_PARA  0x32
enum
{
    ccOFST_CFG_PARA_TYPE,
    ccOFST_CFG_IDX,
    ccOFST_CFG_ADDR1_L,
    ccOFST_CFG_ADDR1_H,
    ccOFST_CFG_VALUE1,
    ccOFST_CFG_PARA1_SIZE
};
#endif

#ifndef ccCMD_ID_FAST_SET_BW
#define ccCMD_ID_FAST_SET_BW     0x34
enum
{
    ccOFST_FAST_SET_BW_BW,
    ccOFST_FAST_SET_BW_IIR,
    ccOFST_FAST_SET_BW_SIZE,
    ccOFST_FAST_SET_BW_STATUS = ccOFST_FAST_SET_BW_BW,
    ccOFST_FAST_SET_BW_RSIZE
};
#endif

#ifndef ccCMD_ID_DEMOD_SET_CH_SEARCH
#define ccCMD_ID_DEMOD_SET_CH_SEARCH  0x35
enum
{
    ccOFST_SET_CH_SEARCH_MODE,
    ccOFST_SET_CH_SEARCH_MD_MODE,
    ccOFST_SET_CH_SEARCH_TPS_CR,
    ccOFST_SET_CH_SEARCH_TPS_CONS,
    ccOFST_SET_CH_SEARCH_TPS_HIER,
    ccOFST_SET_CH_SEARCH_FREQ_BIAS_40KHZ,
    ccOFST_SET_CH_SEARCH_SIZE
};
#endif

#define ccCMD_ID_DEMOD_SET_VAR   0x37
#define ccCMD_ID_ENABLE_BBIQ     0x38

// Internal use
#ifndef ccCMD_ID_GET_VER
#define ccCMD_ID_GET_VER         0x40
enum
{
    ccOFST_VER_YEAR_MONTH,
    ccOFST_VER_DATE,
    ccOFST_VER_HOUR,
    ccOFST_VER_MINUTE,
    ccOFST_VER_SIZE
};
#endif

// Internal use
#define ccCMD_ID_DEMOD_GET_REG   0x41

#define ccCMD_ID_DEMOD_GET_VAR   0x47

#define ccCMD_ID_GET_LABEL        0x4B
enum
{
    ccOFST_LABEL_BRANCH_TYPE,
	ccOFST_LABEL_BRANCH_ID_1,
	ccOFST_LABEL_BRANCH_ID_2,
	ccOFST_LABEL_ID1,
	ccOFST_LABEL_ID2,
	ccOFST_LABEL_SIZE
};

               
// Internal use
#define ccCMD_ID_LOG_GET_ADDR    0x50
enum
{
    ccOFST_LOG_ADDR_L,
    ccOFST_LOG_ADDR_H,
    ccOFST_LOG_ADDR_SIZE,
    ccOFST_LOG_DATA_L = ccOFST_LOG_ADDR_SIZE,
    ccOFST_LOG_DATA_H,
    ccOFST_LOG_DATA_SIZE
};

// Internal use
#define ccCMD_ID_LOG_GET_SIZE    0x51
// Internal use
#define ccCMD_ID_LOG_SET_SIZE    0x52
enum
{
    ccOFST_LOG_OVERFLOW,
    ccOFST_LOG_SET_SIZE
};
// Internal use
#define ccCMD_ID_LOG_SET_TYPE    0x53
enum
{
    ccOFST_LOG_TYPE,
    ccOFST_LOG_SET_TYPE_SIZE
};

// Internal use
#define ccCMD_ID_UP_RESET        0x60
// Internal use
#define ccCMD_ID_UP_PWR_DN       0x62
// Internal use
#define ccCMD_ID_UP_PWR_UP       0x63
// Internal use
#define ccCMD_ID_UP_WDT_RESET    0x64

// for I2C Master R/W
/*
#define ccCMD_ID_I2C_CLK_DIV     0x70
enum
{
   ccOFST_I2C_CLK_DIV_H,   // total 12 bits
   ccOFST_I2C_CLK_DIV_L,
   ccOFST_I2C_CLK_DIV_SIZE
};
#define ccCMD_ID_I2C_WRITE_1     0x71
enum
{
    ccOFST_I2C_WRITE1_DEVICE_ADDR,
    ccOFST_I2C_WRITE1_WORD_ADDR,
    ccOFST_I2C_WRITE1_LENGTH,
    ccOFST_I2C_WRITE1_DATA0,
    ccOFST_I2C_WRITE1_DATA1,
    ccOFST_I2C_WRITE1_DATA2,
    ccOFST_I2C_WRITE1_DATA3,
    ccOFST_I2C_WRITE1_TSIZE,
    // return value
    ccOFST_I2C_WRITE1_STATUS = ccOFST_I2C_WRITE1_DEVICE_ADDR,
    ccOFST_I2C_WRITE1_RSIZE
};
#define ccCMD_ID_I2C_WRITE_2     0x72
enum
{
    ccOFST_I2C_WRITE2_DATA4,
    ccOFST_I2C_WRITE2_DATA5,
    ccOFST_I2C_WRITE2_DATA6,
    ccOFST_I2C_WRITE2_DATA7,
    ccOFST_I2C_WRITE2_TSIZE,
    // return value
    ccOFST_I2C_WRITE2_STATUS = ccOFST_I2C_WRITE2_DATA4,
    ccOFST_I2C_WRITE2_RSIZE
};

#define ccCMD_ID_I2C_READ_1      0x73
enum
{
    ccOFST_I2C_READ1_DEVICE_ADDR,
    ccOFST_I2C_READ1_WORD_ADDR,
    ccOFST_I2C_READ1_CURRENT,
    ccOFST_I2C_READ1_LENGTH,
    ccOFST_I2C_READ1_TSIZE,
    // return value
    ccOFST_I2C_READ1_STATUS = ccOFST_I2C_READ1_DEVICE_ADDR,
    ccOFST_I2C_READ1_DATA0 ,
    ccOFST_I2C_READ1_DATA1,
    ccOFST_I2C_READ1_DATA2,
    ccOFST_I2C_READ1_DATA3,
    ccOFST_I2C_READ1_RSIZE
};
#define ccCMD_ID_I2C_READ_2      0x74
enum
{
    ccOFST_I2C_READ2_DATA4,
    ccOFST_I2C_READ2_DATA5,
    ccOFST_I2C_READ2_DATA6,
    ccOFST_I2C_READ2_DATA7,
    ccOFST_I2C_READ2_RSIZE
};
*/

#define ccCMD_ID_DVBTC_SWITCH    0x7F
enum
{
    ccOFST_DVBTC_SWITCH,   // 1: DVBC, 2: DVBT
    ccOFST_DVBTC_SWITCH_SIZE,
    ccOFST_DVBTC_SWITCH_STATUS = ccOFST_DVBTC_SWITCH,
    ccOFST_DVBTC_SWITCH_RSIZE
};
/***********************************************************************/
// Followings are for DVBC
enum
{
    DVBC_16QAM  = 0,
    DVBC_32QAM,
    DVBC_64QAM,
    DVBC_128QAM,
    DVBC_256QAM,
    DVBC_AUTO
};
    
#define ccCMD_ID_DVBC_SET_QAM         0x80
enum
{
    ccOFST_DVBC_SET_QAM_MODE,   // 0: 16, 1: 32, 2: 64, 3: 128 4: 256QAM
    ccOFST_DVBC_SET_QAM_TSIZE,
    // return value
    ccOFST_DVBC_SET_QAM_STATUS = ccOFST_DVBC_SET_QAM_MODE,   // 1: OK, 0: FAIL
    ccOFST_DVBC_SET_QAM_RSIZE
};

#define ccCMD_ID_DVBC_SET_SYMBOL_RATE 0x81
enum
{
    ccOFST_DVBC_SET_SYMBOL_RATE_H,   // High byte of symbol rate, kSym/sec
    ccOFST_DVBC_SET_SYMBOL_RATE_L,   // Low  byte of symbol rate, kSym/sec
    ccOFST_DVBC_SET_SYMBOL_RATE_TSIZE,
    // return value
    ccOFST_DVBC_SET_SYMBOL_RATE_STATUS = ccOFST_DVBC_SET_SYMBOL_RATE_H,   // 1: OK, 0: FAIL
    ccOFST_DVBC_SET_SYMBOL_RATE_RSIZE
};

#define ccCMD_ID_DVBC_ACQ_START       0x82
enum
{
    // return value
    ccOFST_DVBC_ACQ_START_STATUS,   // 1: OK, 0: FAIL
    ccOFST_DVBC_ACQ_START_RSIZE
};

#define ccCMD_ID_DVBC_REACQ_START     0x83
enum
{
    // return value
    ccOFST_DVBC_REACQ_START_STATUS,   // 1: OK, 0: FAIL
    ccOFST_DVBC_REACQ_START_RSIZE
};

#define ccCMD_ID_DVBC_REACQ_STOP      0x84
enum
{
    // return value
    ccOFST_DVBC_REACQ_STOP_STATUS,   // 1: OK, 0: FAIL
    ccOFST_DVBC_REACQ_STOP_RSIZE
};

#define ccCMD_ID_DVBC_SET_TS_MODE      0x85
enum
{
    ccOFST_DVBC_SET_TS_MODE,   // 0: Parallel, 1: Serial
    ccOFST_DVBC_SET_TS_MODE_SIZE,
    ccOFST_DVBC_SET_TS_MODE_STATUS = ccOFST_DVBC_SET_TS_MODE,
    ccOFST_DVBC_SET_TS_MODE_RSIZE    
};

#define ccCMD_ID_DVBC_SET_TUNER_PARA0 0x86
enum
{
    ccOFST_DVBC_SET_TUNER_PARA0_SLP,
    ccOFST_DVBC_SET_TUNER_PARA0_INT,
    ccOFST_DVBC_SET_TUNER_PARA0_MAX,
    ccOFST_DVBC_SET_TUNER_PARA0_MIN,
    ccOFST_DVBC_SET_TUNER_PARA0_SIZE,
    ccOFST_DVBC_SET_TUNER_PARA0_STATUS = ccOFST_DVBC_SET_TUNER_PARA0_SLP,
    ccOFST_DVBC_SET_TUNER_PARA0_RSIZE
};


#define ccCMD_ID_DVBC_SET_TUNER_PARA1 0x87
enum
{
    ccOFST_DVBC_SET_TUNER_PARA1_TARGET_POW,
    ccOFST_DVBC_SET_TUNER_PARA1_IF_FREQ0,
    ccOFST_DVBC_SET_TUNER_PARA1_IF_FREQ1,
    ccOFST_DVBC_SET_TUNER_PARA1_SIZE,
    ccOFST_DVBC_SET_TUNER_PARA1_STATUS = ccOFST_DVBC_SET_TUNER_PARA1_TARGET_POW,
    ccOFST_DVBC_SET_TUNER_PARA1_RSIZE
};

#define ccCMD_ID_DVBC_SET_ADV_AQM     0x88
enum
{
    ccOFST_DVBC_SET_ADV_AQM_ENABLE,   // 1: Enable, 0: Disable
    ccOFST_DVBC_SET_ADV_AQM_TSIZE,
    // return value
    ccOFST_DVBC_SET_ADV_AQM_STATUS = ccOFST_DVBC_SET_ADV_AQM_ENABLE,   // 1: OK, 0: FAIL
    ccOFST_DVBC_SET_ADV_AQM_RSIZE
};

#define ccCMD_ID_DVBC_SET_ADV_ASR       0x89
enum
{
    ccOFST_DVBC_SET_ADV_ASR_ENABLE,   // 1: Enable, 0: Disable
    ccOFST_DVBC_SET_ADV_ASR_TSIZE,
    // return value
    ccOFST_DVBC_SET_ADV_ASR_STATUS = ccOFST_DVBC_SET_ADV_ASR_ENABLE, // 1: OK, 0: FAIL
    ccOFST_DVBC_SET_ADV_ASR_RSIZE
};


#define ccCMD_ID_DVBC_GET_STATUS0     0x90
enum
{
    ccOFST_DVBC_STATUS00,   // b'[0~1]: PRA, b'[2]: QNIR, b'[3]: QTR, b'[4~5]: QCR, b'[6]: RSH
    ccOFST_DVBC_STATUS_QCN_FAIL_CNT,   // reserved
    ccOFST_DVBC_STATUS_ASR_FAIL_CNT,   // reserved
    ccOFST_DVBC_STATUS_REACQ_CNT,      // reserved
    ccOFST_DVBC_STATUS0_SIZE
};

// 0x91 reserved

#define ccCMD_ID_DVBC_GET_INFO0       0x92
enum
{
    ccOFST_DVBC_INFO0_RS_COR_BIT2,
    ccOFST_DVBC_INFO0_RS_COR_BIT1,
    ccOFST_DVBC_INFO0_RS_COR_BIT0,
    ccOFST_DVBC_INFO0_RS_ERR_PKT1,
    ccOFST_DVBC_INFO0_RS_ERR_PKT0,
    ccOFST_DVBC_INFO0_RS_TOT_PKT1,
    ccOFST_DVBC_INFO0_RS_TOT_PKT0,    
    ccOFST_DVBC_INFO0_SIZE
};

#define ccCMD_ID_DVBC_GET_INFO1       0x93
enum
{
    ccOFST_DVBC_INFO1_AGC_GAIN_H,
    ccOFST_DVBC_INFO1_AGC_GAIN_L,
    ccOFST_DVBC_INFO1_QAM_MODE,   // 0: 16, 1: 32, 2: 64, 3: 128 4: 256QAM
    ccOFST_DVBC_INFO1_SYMBOL_RATE_H,
    ccOFST_DVBC_INFO1_SYMBOL_RATE_L,
    ccOFST_DVBC_INFO1_SYMBOL_RATE_RDY,
    ccOFST_DVBC_INFO1_DSM_SEL,
    ccOFST_DVBC_INFO1_SIZE
};

#define ccCMD_ID_DVBC_GET_INFO2       0x94
enum
{
    ccOFST_DVBC_INFO2_CARRIER_OFFSET3,
    ccOFST_DVBC_INFO2_CARRIER_OFFSET2,
    ccOFST_DVBC_INFO2_CARRIER_OFFSET1,
    ccOFST_DVBC_INFO2_CARRIER_OFFSET0,
    ccOFST_DVBC_INFO2_EQ_SNR1,
    ccOFST_DVBC_INFO2_EQ_SNR0,
    ccOFST_DVBC_INFO2_QCR_DD_BW,
    ccOFST_DVBC_INFO2_SIZE
};

#define ccCMD_ID_DVBC_GET_INFO3       0x95
enum
{
    ccOFST_DVBC_INFO3_SYSC,
    ccOFST_DVBC_INFO3_QCN,
    ccOFST_DVBC_INFO3_QCR_RETURN_RC,
    ccOFST_DVBC_INFO3_QTR_FFT_SEARCH_RESULT,
    ccOFST_DVBC_INFO3_PRA,
    ccOFST_DVBC_INFO3_BN_MONITOR,
    ccOFST_DVBC_INFO3_CARRIER_INVERSE,
    ccOFST_DVBC_INFO3_SIZE
};

#define ccCMD_ID_DVBC_GET_INFO4       0x96
enum
{
    ccOFST_DVBC_INFO4_TIMING_OFFSET_4,
    ccOFST_DVBC_INFO4_TIMING_OFFSET_3,
    ccOFST_DVBC_INFO4_TIMING_OFFSET_2,
    ccOFST_DVBC_INFO4_TIMING_OFFSET_1,
    ccOFST_DVBC_INFO4_TIMING_OFFSET_0,
    ccOFST_DVBC_INFO4_SIZE
};

#define ccCMD_ID_DVBC_GET_INFO5       0x97
enum
{
    ccOFST_DVBC_INFO5_ASR_CONFIDENCE,
    ccOFST_DVBC_INFO5_ASR_CFO3,
    ccOFST_DVBC_INFO5_ASR_CFO2,
    ccOFST_DVBC_INFO5_ASR_CFO1,
    ccOFST_DVBC_INFO5_ASR_CFO0,
    ccOFST_DVBC_INFO5_SIZE
};

#define ccCMD_ID_DVBC_GET_INFO6       0x98
enum
{
    ccOFST_DVBC_INFO6_AQM_00,
    ccOFST_DVBC_INFO6_AQM_01,
    ccOFST_DVBC_INFO6_AQM_02,
    ccOFST_DVBC_INFO6_AQM_03,
    ccOFST_DVBC_INFO6_AQM_04,
    ccOFST_DVBC_INFO6_AQM_Index,
    ccOFST_DVBC_INFO6_SIZE
};

#define ccCMD_ID_DVBC_GET_INFO7       0x99
enum
{
    ccOFST_DVBC_INFO7_LockTime,
    ccOFST_DVBC_INFO7_SpecInv_QCN_State,
    ccOFST_DVBC_INFO7_SYSC_State,
    ccOFST_DVBC_INFO7_ucCurAcqState,
    ccOFST_DVBC_INFO7_uDev_AWGN_PN,
    ccOFST_DVBC_INFO7_uDev_AWGN,
    ccOFST_DVBC_INFO7_uCor_AWGN,
    ccOFST_DVBC_INFO7_SIZE
};

#define ccCMD_ID_DVBC_GET_INFO8       0x9A
enum
{
    ccOFST_DVBC_INFO8_JTD_00,
    ccOFST_DVBC_INFO8_JTD_01,
    ccOFST_DVBC_INFO8_LockTime_00,
    ccOFST_DVBC_INFO8_LockTime_01,
    ccOFST_DVBC_INFO8_SIZE
};

#define ccCMD_ID_DVBC_GET_INFO9       0x9B
enum
{
    ccOFST_DVBC_INFO9_SLD_PRA_STATUS_MONITOR,
    ccOFST_DVBC_INFO9_SLD_PRA_RFAGC_CTRL_WORD_H,
    ccOFST_DVBC_INFO9_SLD_PRA_RFAGC_CTRL_WORD_L,
    ccOFST_DVBC_INFO9_SLD_PRA_IFAGC_CTRL_WORD_H,
    ccOFST_DVBC_INFO9_SLD_PRA_IFAGC_CTRL_WORD_L,
    ccOFST_DVBC_INFO9_SLD_PRA_PGA_GAIN_INDEX,
    ccOFST_DVBC_INFO9_SIZE
};

#define ccCMD_ID_DVBC_GET_INFOA       0x9C
enum
{
    ccOFST_DVBC_INFOA_SLD_PRA_MAG_REF_OUT_H,
    ccOFST_DVBC_INFOA_SLD_PRA_MAG_REF_OUT_L,
    ccOFST_DVBC_INFOA_LEGACY_PRAD_Cur_Level_sf_H,
    ccOFST_DVBC_INFOA_LEGACY_PRAD_Cur_Level_sf_L,
    ccOFST_DVBC_INFOA_SIZE
};

#define ccCMD_ID_DVBC_SET_TS_OUTPUT   0x9D
enum
{
    ccOFST_DVBC_SET_TS_OUTPUT_ENABLE,   // 1: Enable, 0: Disable
    ccOFST_DVBC_SET_TS_OUTPUT_TSIZE,
    // return value
    ccOFST_DVBC_SET_TS_OUTPUT_STATUS = ccOFST_DVBC_SET_TS_OUTPUT_ENABLE,   // 1: OK, 0: FAIL
    ccOFST_DVBC_SET_TS_OUTPUT_RSIZE
};
//lei_code_dB110713;
#ifndef ccCMD_ID_SET_PGAGAININDEX 
#define ccCMD_ID_SET_PGAGAININDEX        0xA0
enum
{
	ccOFST_FAST_SET_PGAGAININDEX,
    ccOFST_FAST_SET_PGAGAININDEX_TSIZE,
    ccOFST_FAST_SET_PGAGAININDEX_STATUS = ccOFST_FAST_SET_PGAGAININDEX,
	ccOFST_FAST_SET_PGAGAININDEX_RSIZE
};
#endif

#define ccCMD_ID_DVBC_SET_QTRER_EN 0xA1
enum
{
	ccOFST_DVBC_SET_QTRER_ENABLE,   // 1: Enable, 0: Disable
    ccOFST_DVBC_SET_QTRER_TSIZE,
    // return value
    ccOFST_DVBC_SET_QTRER_STATUS = ccOFST_DVBC_SET_QTRER_ENABLE, // 1: OK, 0: FAIL
    ccOFST_DVBC_SET_QTRER_RSIZE
};

//XJ Add for LTE test
#define cCMD_ID_DVBC_SET_BNPORTESTION_EN 0xA2
enum
{
	cOFST_DVBC_SET_BNPORTESTION_ENABLE,   // 1: Enable, 0: Disable
    cOFST_DVBC_SET_BNPORTESTION_TSIZE,
    // return value
    cOFST_DVBC_SET_BNPORTESTION_STATUS = cOFST_DVBC_SET_BNPORTESTION_ENABLE, // 1: OK, 0: FAIL
    cOFST_DVBC_SET_BNPORTESTION_RSIZE
};

/***********************************************************************/
#define mccGET_QUEUE(pu2QuePtr, ePosOfst)    pu2QuePtr[ccCMD_REG_NUM - 2 - (ePosOfst)]
#define mccSET_QUEUE1(pu2QuePtr, ePosOfst)   pu2QuePtr[ccCMD_REG_NUM - 2 - (ePosOfst)]
#define mccSET_QUEUE2(pu2QuePtr, ePosOfst)   pu2QuePtr[ccCMD_REG_NUM - 2 - (ePosOfst) + 1]
#define mccGET_CMD(pu2QuePtr)                pu2QuePtr[ccCMD_ID_POS]
#define mccSET_CMD2(pu2QuePtr)               pu2QuePtr[ccCMD_ID_POS + 1]

///**************************************************************************/
///*--------- uP 0x000 -----------------------------------------------------*/
#define ccpUP_IR_RD              7

/* For MT5112_REG_uP_Ctrl */
#define ccpUP_CHKSUM_OK          6
#define ccpUP_RESET_CHKSUM       6
#define ccpUP_ROMCODE_CHKSUM     5  //lei_code_110512;
#define ccpUP_READY              4
#define ccpUP_MEM_INIT_DONE      3
#define ccpUP_CHKSUM_DONE        2
#define ccpUP_RESTART_CHKSUM     2
#define ccpUP_DN_FREQ            1
#define ccpUP_ENABLE             0
//
/* For MT5112_REG_uP_Ctrl_E */
#define ccpUP_ICE_ENABLE         0
/*--------- UP_CTRL_E 0x004 -----------------------------------------------*/
#define ccpGPIO_MODE_0           5
#define ccpGPIO_MODE_1           6

/* For MT5112_REG_uP_CMDR_CFG */
#define ccpUP_CMD_RX             7
#define ccpUP_CMD_RX_IE          6
#define ccpUP_RSP_TX             5
#define ccpUP_RSP_TX_IE          4

/*--------- I2C 0x024 ----------------------------------------------------*/
#define ccpI2C_AUTO_INC          1
/*--------- I2C_CON1 0x024 ----------------------------------------------------*/
#define ccpI2C_TUNER_ON          1

/**************************************************************************/

enum
{
    DVBC_STATE_RFAGC_ACQ_TRY0 = 0,
    DVBC_STATE_RFAGC_ACQ_WAIT0,
    DVBC_STATE_RFAGC_ACQ_TRY1,
    DVBC_STATE_RFAGC_ACQ_WAIT1,
    DVBC_STATE_RFAGC_ACQ_CAL_OP,
    DVBC_STATE_RFAGC_ACQ_VALID,
    DVBC_STATE_RFAGC_TRA_IDLE,
    DVBC_STATE_RFAGC_TRA
};

#define TAXES_MASK_MANUAL       0x20
#define TAXES_MASK_SENS_MODE    0x10
#define TAXES_MASK_LNA_VALUE    0x0F
#define DVBC_REG_PAGE_NUM       0xFF
#if 1
/************ Define MASK ************/
// AREG_SET_SFP_WIDTH   0x17
#define DVBC_MASK_SEARCH_PEAK     0x40
#define DVBC_MASK_NEW_IFFT        0x20
#define DVBC_MASK_SFP_INTE_WIDTH  0x1F

// AREG_IFS_MODE        0x22
#define DVBC_MASK_BBIQ_24M_PHASE  0x40   
#define DVBC_MASK_BBIQ_CLK24M     0x20
#define DVBC_MASK_BBIQ_EN         0x10
       
#define DVBC_MASK_CHANL_SEL       0x03

// AREG_CHANL_SEL       0x23
#define DVBC_MASK_CHANL_SEL       0x03

// DAGC_MAXLVL2         0x3A
#define DVBC_MASK_DE1_VALID       0x80
#define DVBC_MASK_DE0_VALID       0x40

// SET_MD             0x40      
#define DVBC_MASK_EN_2K             0x80
#define DVBC_MASK_EN_4K             0x40
#define DVBC_MASK_EN_8K             0x20
#define DVBC_MASK_SET_GI_MODE       0x1C
// SET_MD_RATIO       0x41      
#define DVBC_MASK_THLD_RATIO        0xF0
#define DVBC_MASK_AVG_RATIO         0x0E
// SET_MD_THLD        0x42      
#define DVBC_MASK_SET_TOL_POS       0xC0
#define DVBC_MASK_SET_VAL_THLD      0x30
// CORE_CONTROL       0x43      
#define DVBC_MASK_SI_MODE           0xC0
#define DVBC_MASK_SOFT_RESET        0x08
#define DVBC_MASK_FEC_EN            0x80
#define DVBC_MASK_AGC_EN            0x04
#define DVBC_MASK_DAGC_EN           0x02
#define DVBC_MASK_MD_EN             0x01
#define DVBC_MASK_CRF_EN            0x40

// CORE_CONTROL       0x43
#define DVBC_MASK_DE1_EN            0x20
#define DVBC_MASK_DE0_EN            0x10

// EM_RESULT          0x4C
#define DVBC_MASK_EM_VALID          0x10
#define DVBC_MASK_EM_MIN_COMB       0x07

// SET_START_POS1     0x4F
#define DVBC_MASK_IFFT_TRACK        0x20

//AREG_SET_CE_POS1
#define DVBC_MASK_CE_INDEX          0x60

//AREG_SFP_POS_01_MSB         0x54  
#define DVBC_MASK_SFP_VALID         0x80
#define DVBC_MASK_SFP_POSITION_1    0x70
#define DVBC_MASK_SFP_POSITION_0    0x07

//AREG_TPS_REG_0          0x60
#define DVBC_MASK_TPS_HIER_sel      0x80
#define DVBC_MASK_TPS_CON_sel       0x40
#define DVBC_MASK_TPS_INDEPTH       0x10
#define DVBC_MASK_TPS_HIER          0x0C
#define DVBC_MASK_TPS_CON           0x03

//AREG_TPS_REG_1          0x61
#define DVBC_MASK_TPS_CR_sel        0x80
#define DVBC_MASK_TPS_HP_sel        0x40
#define DVBC_MASK_TPS_LP_CR         0x38
#define DVBC_MASK_TPS_HP_CR         0x07

//AREG_CE_FD_COEF_NO      0x62
#define DVBC_MASK_CE_COEF_sel       0x80
#define DVBC_MASK_CE_COEF_NO        0x3F

//AREG_CE_FD_COEF_0       0x63
//AREG_CE_FD_COEF_1       0x64

//AREG_CE_FD_COEF_CTRL    0x65
#define DVBC_MASK_CE_COEF_mod       0x02
#define DVBC_MASK_CE_COEF_set       0x01

//AREG_CE_AR_COEF         0x66
#define DVBC_MASK_CE_AR_COEF        0x3F

//AREG_TPS_INTERRUPT_CTRL 0x67
#define DVBC_MASK_BCH_INT_EN        0x02
#define DVBC_MASK_TPS_CHANGE_INT_EN 0x01

//AREG_CE_TD_INTERP         0x69  
#define DVBC_MASK_FD_BW_SEL        0x06
#define DVBC_MASK_TD_INTERP_SEL     0x01

//AREG_TPS_LOSS_STEP        0x6A
#define DVBC_MASK_TPS_LOSS_COUNT_UP   0xF0      
#define DVBC_MASK_TPS_LOSS_COUNT_DOWN 0x0F  

//AREG_CE_FD_COEF_CTRL      0x75
#define DVBC_MASK_CE_COEF_mod         0x02  
#define DVBC_MASK_CE_COEF_set         0x01

//AREG_TPS_CONFIRM_NUMBER
#define DVBC_MASK_TPS_CONFIRM_NUMBER  0x03

//AREG_CE_TD_INTERP
#define DVBC_MASK_CONS_SEL            0x08

//AREG_I2C_CON1           0x90       
#define DVBC_MASK_I2C_ON             0x80
#define DVBC_MASK_DG_CNT             0x7C
#define DVBC_MASK_DG_SEL             0x03

//AREG_I2C_CON2           0x91       
#define DVBC_MASK_PULLUP_DLY         0x1F

//AREG_TSIF_CTRL          0x92   
#define DVBC_MASK_TSIF_CTRL          0x7F

//AREG_EN_CTRL            0x93      
#define DVBC_MASK_DIC_CLK_REV        0x40
#define DVBC_MASK_DIC_OUT_EN         0x30
#define DVBC_MASK_DESC_LOCK          0x08
#define DVBC_MASK_SB_EN              0x04 
#define DVBC_MASK_RS_EN              0x02
#define DVBC_MASK_DESC_EN            0x01

//AREG_DIC_CTRL           0x94
#define DVBC_MASK_SHR                0xC0
#define DVBC_MASK_SHL1               0x30
#define DVBC_MASK_SHL0               0x0C
#define DVBC_MASK_DIC_MODE           0x03


//AREG_TEST_FORM_SEL      0x9A
#define DVBC_MASK_IN_OFFSET          0x01
#define DVBC_MASK_OUT_OFFSET         0x02

//AREG_FTT_LF_STATE         0xcd 
#define DVBC_MASK_LF_TIME_STATE         0x30
#define DVBC_MASK_LF_FREQ_STATE         0x03

//AREG_DAGC_STATE           0xce
#define DVBC_MASK_DAGC_STATE            0x03

//AREG_LOOP_BANDWIDTH       0xcf 
#define DVBC_MASK_FREQ_LOOP_KI          0xC0
#define DVBC_MASK_FREQ_LOOP_KP          0x30
#define DVBC_MASK_TIME_LOOP_KI          0x0C
#define DVBC_MASK_TIME_LOOP_KP          0x03

// MD_STATUS0         0xd0  
#define DVBC_MASK_MD_FFT_MODE           0xC0
#define DVBC_MASK_MD_GI_MODE            0x30

// MD_STATUS1         0xd1  

// CONTROL_STATUS     0xd2  
#define DVBC_MASK_SC_SI_MODE            0x30
#define DVBC_MASK_SC_STATE              0x0F

//AREG_AGC_STA_IND         0xdf             
#define DVBC_MASK_AGC_STAT              0x60
#define DVBC_MASK_TRA_STAT              0x10
#define DVBC_MASK_ACQ_STAT              0x08
#define DVBC_MASK_MANU_STAT             0x04
#define DVBC_MASK_TOO_HIGH              0x02
#define DVBC_MASK_TOO_LOW               0x01

//AREG_TPS_REG_2            0xe0      
//AREG_TPS_REG_3            0xe1      

//AREG_TPS_REG_4            0xe2      
#define DVBC_MASK_TPS_FRAME         0xC0
#define DVBC_MASK_TPS_CID_L         0x3F

//AREG_TPS_REG_5            0xe3      
#define DVBC_MASK_SYS_CR            0xE0
#define DVBC_MASK_TPS_GI            0x0C
#define DVBC_MASK_TPS_mode          0x03

//AREG_TPS_SATUS            0xe4      
#define DVBC_MASK_TPS_CHANGE        0x04
#define DVBC_MASK_BCH_OK            0x02
#define DVBC_MASK_TPS_SYNC          0x01

//AREG_CORE_STATUS          0xea   
#define DVBC_MASK_TPSLOCK           0x01

//AREG_TPS_LOSS_COUNT        0xEB
#define DVBC_MASK_TPS_LOSS_COUNT    0x0F

//AREG_RSH_RESULT            0xfa
#define DVBC_MASK_RSH_LOCK            0x80
#define DVBC_MASK_LOCK_CUNT           0x70
#define DVBC_MASK_UNLOCK_CUNT         0x0f
#endif

#endif  // _DVBC_COM_DEFS_H

/***********************************************************************/
/*              Version Control                                        */
/***********************************************************************/
/*
   $Log: Com_defs.h $
   Revision 1.0  2005/04/26 13:18:24 JWEI
*/

