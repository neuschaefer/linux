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
 * $RCSfile: pi_demod.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_demod.h
 *  This header file includes definitions and API prototype declaration
 *  in pi_demod.c.
 */

#ifndef _MT5391_PI_DEMOD_H_
#define _MT5391_PI_DEMOD_H_

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
//#include "ctx_driver.h"
#include"x_tuner.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


#define cMAX_READ_NUM           ccCMD_REG_NUM
#define ERR_BITS_PER_ERR_PKT    27

#define cPER_PRE_SCALE          10
#define cPER_SCALE              10000L
#define cPER_MAX                ((UINT16)(0.5 * cPER_SCALE * cPER_PRE_SCALE))

#define RF_AGC_SADC_INPUT_SWING    2.0//1.9494//2.3570//2.0  //Volt                              
#define RF_AGC_SADC_BIAS           0.2//0.1257//(RF_AGC_SADC_COMM_VOLT - RF_AGC_SADC_INPUT_SWING/2.0)

#define RF_AGC_RF_OP_VOL_TRY0      0.0//1.27       // Volt
#define RF_AGC_RF_OP_VOL_TRY1      1.4//0.53       // Volt
#define RF_AGC_RF_AGC_SLP_DBV_DF     (1.0/(-0.0448))   // dB/vol
#define RF_AGC_BB_AGC_SLP_DBV_DF     (1.0/(-0.0585))   // dB/vol
#define RF_AGC_WBPD_TRY0_DF_COEF   58.608//74.349//63.100//75.092
#define RF_AGC_WBPD_TRY1_DF_COEF   57.951//80.856//67.115//76.437
#define RF_AGC_WBPD_TRY0_DF_OFST   -89.5//-74.801//-75.647//-81.661
#define RF_AGC_WBPD_TRY1_DF_OFST   -60//-110.93//-107.5//-98.787
#define RF_AGC_MAX_RF_VOL          1.4  
#define RF_AGC_MIN_RF_VOL          0.0
#define RF_AGC_MAX_BB_VOL          1.8
#define RF_AGC_MIN_BB_VOL          0.0

#define RF_AGC_STEP_SIZE           (S16)32 
#define RF_AGC_STEP_PRD       	   (S16)256                                 
#define RF_AGC_STEP_NUM_INIT       (U8) 15  // max: 15, min: 1                            
#define RF_AGC_STEP_NUM            (U8) 1   // max: 15, min: 1                                 
#define RF_AGC_MAX_RF_GAIN_VOL     (S16)0   // (Volt *100)
#define RF_AGC_MAX_BB_GAIN_VOL     (S16)0   // (Volt *100)
#define RF_AGC_TOTAL_TUNER_GAIN    (S16)7600 // (dB *100)
#define RF_AGC_RESOLUTION_TRY0     (S16)-3200 // (dB *100)
#define RF_AGC_RF_ATT_TRY0         (S16)3000  // (dB *100)
#define RF_AGC_RF_ATT_TRY1         (S16)1270  // (dB *100)
#define RF_AGC_TOP                 (S16)(-55*100)  // (dBm *100)
#define RF_AGC_ATT_SLOPE           (S16)(0.1111 *256)
#define RF_AGC_ATT_INTCP           (S16)(1.1111 *100)
#define RF_AGC_MIN_ATT_SLOPE       (S16)(0      *256)
#define RF_AGC_MIN_ATT_INTCP       (S16)(5      *100)
#define RF_AGC_ACI_THRESHOLD       (S16)(0      *100)

#define RF_AGC_RF_OP_TRY0          (S16)((RF_AGC_RF_OP_VOL_TRY0 -1.65)/3.3 *256 *256)
#define RF_AGC_RF_OP_TRY1          (S16)((RF_AGC_RF_OP_VOL_TRY1 -1.65)/3.3 *256 *256)
#define RF_AGC_WBPD_COEF_TRY0      (S16)( RF_AGC_WBPD_TRY0_DF_COEF*RF_AGC_SADC_INPUT_SWING*100/1024 *256)
#define RF_AGC_WBPD_COEF_TRY1      (S16)( RF_AGC_WBPD_TRY1_DF_COEF*RF_AGC_SADC_INPUT_SWING*100/1024 *256)
#define RF_AGC_WBPD_OFST_TRY0      (S16)((RF_AGC_WBPD_TRY0_DF_OFST + RF_AGC_WBPD_TRY0_DF_COEF*RF_AGC_SADC_BIAS) *100)
#define RF_AGC_WBPD_OFST_TRY1      (S16)((RF_AGC_WBPD_TRY1_DF_OFST + RF_AGC_WBPD_TRY1_DF_COEF*RF_AGC_SADC_BIAS) *100)
#define RF_AGC_RF_AGC_SLP_VDB      (S16)((1/RF_AGC_RF_AGC_SLP_DBV_DF) *256*256)  // vol/dB  // vol/dB *256 *256
#define RF_AGC_RF_AGC_SLP_DBV      (S16)(RF_AGC_RF_AGC_SLP_DBV_DF *256) 
#define RF_AGC_BB_AGC_SLP_DBV      (S16)(RF_AGC_BB_AGC_SLP_DBV_DF *256)
#define RF_AGC_MAX_RF_VOL_OP        (S8)((RF_AGC_MAX_RF_VOL-1.65)/3.3 *256)
#define RF_AGC_MIN_RF_VOL_OP        (S8)((RF_AGC_MIN_RF_VOL-1.65)/3.3 *256)
#define RF_AGC_MAX_BB_VOL_OP        (S8)((RF_AGC_MAX_BB_VOL-1.65)/3.3 *256)
#define RF_AGC_MIN_BB_VOL_OP        (S8)((RF_AGC_MIN_BB_VOL-1.65)/3.3 *256)

#define RF_AGC_MAX_GAIN_VOL_RF   0
#define RF_AGC_MAX_GAIN_VOL_BB   0
#define RF_AGC_TARGET_LVL        -600 //-6dBm

#define MT5391_TUNER_MODE_BBIQ           0x01
#define MT5391_TUNER_MODE_SELF_RFAGC     0x02
#define MT5391_TUNER_MODE_RFAGC_LNA      0x04
#define MT5391_TUNER_MODE_RFAGC_I2C_AGC  0x08

#define fcDVBT_TSIF_SERIAL      1

#define fcSHORT_LINUX_INIT      0


#define cCONN_TYPE_DVBC         0x01
#define cCONN_TYPE_DTMB         0x02

enum
{
    MT5391_BW_6MHz = 0,
    MT5391_BW_7MHz,
    MT5391_BW_8MHz,
    MT5391_BW_5MHz
};


enum
{
    cUP_LOAD_OK = 0,
    cUP_LOAD_ERR_I2C,
    cUP_LOAD_ERR_HW_RDY,
    cUP_LOAD_ERR_ROM_CHKSUM_RDY,
    cUP_LOAD_ERR_ROM_CHKSUM_OK,
    cUP_LOAD_ERR_SW_CHKSUM_OK,
    cUP_LOAD_ERR_CHKSUM_RDY,
    cUP_LOAD_ERR_CHKSUM_OK,
    cUP_LOAD_ERR_CNT
};

enum
{
    SELECT_CCI_NOTCH = 0,
    SELECT_ANIR
};

enum
{
    CH_SEARCH_SCAN = 0,
    CH_SEARCH_UPDOWN
};

enum
{
    REACQ_NONE = 0,
    REACQ_AUTO
};

enum
{
    FAST_SYNC_NONE,
    FAST_SYNC_OLD,
    FAST_SYNC_AUTO,
    FAST_SYNC_MANUAL
};

enum
{
    EN_LP_DECODE = 0,
    EN_HP_DECODE,
};


#define cMSG_UP_ERR_I2C         "I2c error!"
#define cMSG_UP_ERR_HW_RDY      "HW not ready!"
#define cMSG_UP_ERR_CHKSUM_RDY  "Chksum not ready!"
#define cMSG_UP_ERR_CHKSUM_OK   "Chksum not Ok!"

#define cMAX_DIVERSITY_NUM      8 


/*********** Register Addr. *************************************/
#define DTMB_REG_uP_Ctrl      (UINT16) 0x00
#define DTMB_REG_uP_IRA_H     (UINT16) 0x24
#define DTMB_REG_uP_IRA_L     (UINT16) 0x25
#define DTMB_REG_uP_IRData    (UINT16) 0x28
#define DTMB_REG_uP_CMDR_CFG  (UINT16) 0x07
#define DTMB_REG_uP_GP_REG00  (UINT16) 0x08
#define DTMB_REG_uP_GP_REG01  (UINT16) 0x09
#define DTMB_REG_uP_GP_REG02  (UINT16) 0x0A
#define DTMB_REG_uP_GP_REG03  (UINT16) 0x0B
#define DTMB_REG_uP_GP_REG04  (UINT16) 0x0C
#define DTMB_REG_uP_GP_REG05  (UINT16) 0x0D
#define DTMB_REG_uP_GP_REG06  (UINT16) 0x0E
#define DTMB_REG_uP_GP_REG07  (UINT16) 0x0F

#define DTMB_REG_RFA4A0       (UINT16) 0x4A0
#define DTMB_REG_RISCIF_CFG   (UINT16) 0x4A4
#define DTMB_REG_RISCIF_WDATA (UINT16) 0x4B0
#define DTMB_REG_RISCIF_CTRL  (UINT16) 0x4B4
#define DTMB_REG_RISCIF_RDATA (UINT16) 0x4B8
#define DTMB_REG_RISCIF_STAT  (UINT16) 0x4BC

#define DTMB_REG_RSICIF_CMD0  (UINT16) 0x4C0
#define DTMB_REG_RSICIF_CMD1  (UINT16) 0x4C1
#define DTMB_REG_RSICIF_CMD2  (UINT16) 0x4C2
#define DTMB_REG_RSICIF_CMD3  (UINT16) 0x4C3
#define DTMB_REG_RSICIF_CMD4  (UINT16) 0x4C4
#define DTMB_REG_RSICIF_CMD5  (UINT16) 0x4C5
#define DTMB_REG_RSICIF_CMD6  (UINT16) 0x4C6
#define DTMB_REG_RSICIF_CMD7  (UINT16) 0x4C7
#define DTMB_REG_RSICIF_RSP0  (UINT16) 0x4C8
#define DTMB_REG_RSICIF_RSP1  (UINT16) 0x4C9
#define DTMB_REG_RSICIF_RSP2  (UINT16) 0x4CA
#define DTMB_REG_RSICIF_RSP3  (UINT16) 0x4CB
#define DTMB_REG_RSICIF_RSP4  (UINT16) 0x4CC
#define DTMB_REG_RSICIF_RSP5  (UINT16) 0x4CD
#define DTMB_REG_RSICIF_RSP6  (UINT16) 0x4CE
#define DTMB_REG_RSICIF_RSP7  (UINT16) 0x4CF
#define DTMB_REG_uP_PNGP_H    (UINT16) 0x2C
#define DTMB_REG_uP_PNGP_L    (UINT16) 0x2D
#define DTMB_REG_uP_LPA_H     (UINT16) 0x2E
#define DTMB_REG_uP_LPA_L     (UINT16) 0x2F
#define DTMB_REG_I2C_Cfg      (UINT16) 0x44
#define DTMB_REG_I2C_Deglitch (UINT16) 0x45
#define DTMB_REG_I2C_Pullup_Del (UINT16) 0x46
#define DTMB_REG_I2C_IF_Cfg   (UINT16) 0x47
#define DTMB_REG_TSIF_CTRL    (UINT16) 0x5D0
#define DTMB_REG_SB_CFG0    (UINT16) 0x5B0
#define DTMB_REG_SB_RATE    (UINT16) 0x5B2



/*********** Register Addr. *************************************/
#define MT5391_REG_uP_Ctrl      (UINT16) 0x00
#define MT5391_REG_uP_IRA_H     (UINT16) 0x24
#define MT5391_REG_uP_IRA_L     (UINT16) 0x25
#define MT5391_REG_uP_IRData    (UINT16) 0x28
#define MT5391_REG_uP_CMDR_CFG  (UINT16) 0x07
#define MT5391_REG_uP_GP_REG00  (UINT16) 0x08
#define MT5391_REG_uP_GP_REG01  (UINT16) 0x09
#define MT5391_REG_uP_GP_REG02  (UINT16) 0x0A
#define MT5391_REG_uP_GP_REG03  (UINT16) 0x0B
#define MT5391_REG_uP_GP_REG04  (UINT16) 0x0C
#define MT5391_REG_uP_GP_REG05  (UINT16) 0x0D
#define MT5391_REG_uP_GP_REG06  (UINT16) 0x0E
#define MT5391_REG_uP_GP_REG07  (UINT16) 0x0F

#define MT5391_REG_CORE_CONTROL (UINT16) 0x143
#define MT5391_REG_TSIF_CTRL    (UINT16) 0x192
#define MT5391_REG_ANA_PD_MODE  (UINT16) 0x1B4
#define MT5391_REG_PGA_CTRL_1   (UINT16) 0x1B6
#define MT5391_REG_TPS_REG_1    (UINT16) 0x171
#define MT5391_REG_DAGC_LF_GAIN0 (UINT16) 0x1CE

#define MT5391_REG_DVBT_PDN     (UINT16) 0x302

#define MT5391_REG_RISCIF_CFG   (UINT16) 0x4A4
#define MT5391_REG_RISCIF_WDATA (UINT16) 0x4B0
#define MT5391_REG_RISCIF_CTRL  (UINT16) 0x4B4
#define MT5391_REG_RISCIF_RDATA (UINT16) 0x4B8
#define MT5391_REG_RISCIF_STAT  (UINT16) 0x4BC

#define MT5391_REG_RSICIF_CMD0  (UINT16) 0x4C0
#define MT5391_REG_RSICIF_CMD1  (UINT16) 0x4C1
#define MT5391_REG_RSICIF_CMD2  (UINT16) 0x4C2
#define MT5391_REG_RSICIF_CMD3  (UINT16) 0x4C3
#define MT5391_REG_RSICIF_CMD4  (UINT16) 0x4C4
#define MT5391_REG_RSICIF_CMD5  (UINT16) 0x4C5
#define MT5391_REG_RSICIF_CMD6  (UINT16) 0x4C6
#define MT5391_REG_RSICIF_CMD7  (UINT16) 0x4C7
#define MT5391_REG_RSICIF_RSP0  (UINT16) 0x4C8
#define MT5391_REG_RSICIF_RSP1  (UINT16) 0x4C9
#define MT5391_REG_RSICIF_RSP2  (UINT16) 0x4CA
#define MT5391_REG_RSICIF_RSP3  (UINT16) 0x4CB
#define MT5391_REG_RSICIF_RSP4  (UINT16) 0x4CC
#define MT5391_REG_RSICIF_RSP5  (UINT16) 0x4CD
#define MT5391_REG_RSICIF_RSP6  (UINT16) 0x4CE
#define MT5391_REG_RSICIF_RSP7  (UINT16) 0x4CF

#define MT5391_REG_VARYING_AUTO_AR_ON (UINT16) 0x507

#define MT5391_REG_uP_PNGP_H    (UINT16) 0x2C
#define MT5391_REG_uP_PNGP_L    (UINT16) 0x2D
#define MT5391_REG_uP_LPA_H     (UINT16) 0x2E
#define MT5391_REG_uP_LPA_L     (UINT16) 0x2F
#define MT5391_REG_I2C_Cfg      (UINT16) 0x44
#define MT5391_REG_I2C_Deglitch (UINT16) 0x45
#define MT5391_REG_I2C_Pullup_Del (UINT16) 0x46
#define MT5391_REG_I2C_IF_Cfg   (UINT16) 0x47

#define DVBT_REG_Chip_ID   (UINT16) 0xE08
#define DVBC_REG_Chip_ID   (UINT16) 0xE09
#define MT5135_REG_Chip_ID   (UINT16) 0xE0A



/*--------- RISCIF_CTRL 0x4B4 -------------------------------------------*/
#define cpRISCIF_CTRL_DL_EN       (7 + (3 * 8))
#define cpRISCIF_CTRL_WR_EN       (6 + (3 * 8))
#define cpRISCIF_CTRL_RD_EN       (5 + (3 * 8))

#define cpRISCIF_CTRL_B3_EN       (3 + (2 * 8))
#define cpRISCIF_CTRL_B2_EN       (2 + (2 * 8))
#define cpRISCIF_CTRL_B1_EN       (1 + (2 * 8))
#define cpRISCIF_CTRL_B0_EN       (0 + (2 * 8))



/*--------- RISCIF_STAT 0x4BC -------------------------------------------*/
#define cwRISCIF_STAT_WSTAT       2/* bit length */
#define cpRISCIF_STAT_WSTAT       0/* position   */
#define cmRISCIF_STAT_WSTAT       mcSET_MASKS(RISCIF_STAT_WSTAT)

#define cpRISCIF_STAT_WSTAT_RDY   (0 + cpRISCIF_STAT_WSTAT)
#define cpRISCIF_STAT_WSTAT_ERR   (1 + cpRISCIF_STAT_WSTAT)
#define cRISCIF_STAT_WSTAT_RDY    0
#define cRISCIF_STAT_WSTAT_BUSY   1
#define cRISCIF_STAT_WSTAT_ERR    2

#define cwRISCIF_STAT_RSTAT       2/* bit length */
#define cpRISCIF_STAT_RSTAT       2/* position   */
#define cmRISCIF_STAT_RSTAT       mcSET_MASKS(RISCIF_STAT_RSTAT)

#define cpRISCIF_STAT_RSTAT_RDY   (0 + cpRISCIF_STAT_RSTAT)
#define cpRISCIF_STAT_RSTAT_ATTN  (1 + cpRISCIF_STAT_RSTAT)
#define cRISCIF_STAT_RSTAT_RDY    0
#define cRISCIF_STAT_RSTAT_BUSY   1
#define cRISCIF_STAT_RSTAT_ATTN   2
#define cRISCIF_STAT_RSTAT_ERR    3

#define cwRISCIF_STAT_DSTAT       1/* bit length */
#define cpRISCIF_STAT_DSTAT       4/* position   */
#define cmRISCIF_STAT_DSTAT       mcSET_MASKS(RISCIF_STAT_DSTAT)

#define cpRISCIF_STAT_DSTAT_RDY   (0 + cpRISCIF_STAT_DSTAT)
#define cRISCIF_STAT_DSTAT_RDY    0
#define cRISCIF_STAT_DSTAT_BUSY   1

#define cpRISCIF_STAT_CMD_VAL     6

#define cpRISCIF_STAT_WIRQ        (0 + (1 * 8))
#define cpRISCIF_STAT_RIRQ        (1 + (1 * 8))
#define cpRISCIF_STAT_UPIRQ       (2 + (1 * 8))


/******************                   MT5391                   *********************/

enum
{
    WAD_IN_ACQ = 0,
    WAD_IN_HWEM,
    WAD_IN_HWEM_WAIT_IFFT,
    WAD_ACQ_GO_TO_TRA,
    WAD_IN_TRA,
    WAD_TRA_GO_TO_ACQ
};

#define DEFAULT_I2C_ADDR        0x82


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define mcHOST_CMD(pDemd, Cmd, Rcnt, Wcnt)      fgHostCmdDTMB((DEMOD_CTX_T *) pDemd, Cmd, Rcnt, Wcnt)
#define mcCTMR_DBG_MSG(_x_)     mcSHOW_DBG_MSG2(_x_)
#define mcCTMR_DBG_MSG2(_x_)    mcSHOW_DBG_MSG2(_x_)

#define mcCHECK_BLOCK_I2C(ucRetSts)
#define mcCHECK_BLOCK_I2C0()
#define mcBLOCK_I2C(ucBlockSts)

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
UINT8  DTMB_SetReg(DEMOD_CTX_T *psDemodCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount);
UINT8  DTMB_GetReg(DEMOD_CTX_T *psDemodCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount);
BOOL   fgHostCmdDTMB(DEMOD_CTX_T *psDemodCtx, UCHAR *aucCmdData, UCHAR ucReadParacnt, UCHAR ucWriteCmdcnt);
BOOL   fgGetFwVars(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UCHAR *pucData, UCHAR ucCnt);
UINT8  ucGetFwVarDTMB(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr);
BOOL   fgSetFwVars(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UCHAR *pucData, UCHAR ucCnt);
void   vSetFwVarDTMB(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UINT8 ucValue);
void   vDTMBGetInfo(DEMOD_CTX_T *psDemodCtx);
void   vDTMBGetStatus(DEMOD_CTX_T *psDemodCtx);
UINT8  DTMB_ChipInit(DEMOD_CTX_T *psDemodCtx);
void   DTMB_AcqStart(DEMOD_CTX_T *psDemodCtx);
BOOL   fgDTMB_FwStart(DEMOD_CTX_T *psDemodCtx);
INT32  DTMB_Initialize(DEMOD_CTX_T *psDemodCtx, UINT8 u1I2cAddress, BOOL fgLoadCode);
INT32  DTMB_DemodReset(DEMOD_CTX_T *psDemodCtx);
UINT8  DTMB_GetSignalLevel(DEMOD_CTX_T *psDemodCtx);
UINT16 DTMB_GetSignalSNR(DEMOD_CTX_T *psDemodCtx);
INT32  DTMB_GetPER(DEMOD_CTX_T *psDemodCtx);
INT32  DTMB_GetLdpcERR(DEMOD_CTX_T *psDemodCtx);
UINT32 DTMB_GetTotPkt(DEMOD_CTX_T *psDemodCtx);
UINT32 DTMB_GetErrPkt(DEMOD_CTX_T *psDemodCtx);
UINT16 DTMB_GetIfAgcVol(DEMOD_CTX_T *psDemodCtx);
BOOL   DTMB_GetTsLock(DEMOD_CTX_T *psDemodCtx);
UINT8  DTMB_GetBW(DEMOD_CTX_T *psDemodCtx);
INT32  DTMB_GetFreqOffset_kHz(DEMOD_CTX_T *psDemodCtx);
UINT8  DTMB_SetNormalMode(DEMOD_CTX_T *psDemodCtx);
UINT8  DTMB_Connect(DEMOD_CTX_T *psDemodCtx, UINT8 ucBW);
VOID   DTMB_DisConnect(DEMOD_CTX_T *psDemodCtx);
UINT8  DTMB_GetSync(DEMOD_CTX_T *psDemodCtx);
UINT8  DTMB_GetSyncAfterAcq(DEMOD_CTX_T *psDemodCtx);
void   DTMB_UpdateInfo(DEMOD_CTX_T *psDemodCtx);
void   DTMB_SetBW(DEMOD_CTX_T *psDemodCtx, UINT8 ucBW);
void   DTMB_SetTsIf(DEMOD_CTX_T *psDemodCtx, UCHAR tsif_format, UCHAR tsif_ctrl, UCHAR tsif_rate);
void   DTMB_SetSearchParam(DEMOD_CTX_T *psDemodCtx, UCHAR search_mode, UCHAR mdMode, 
                            UCHAR tpsCr, UCHAR tpsCons, UCHAR tpsHier, INT8 freqBias_40kHz);
void   DTMB_TunerI2c(DEMOD_CTX_T *psDemodCtx, BOOL fgSwitchOn);
void   DTMB_DriverStart(DEMOD_CTX_T *psDemodCtx, UINT8 u1Mon1Sel, UINT8 u1Mon2Sel);
void   DTMB_DriverStop(DEMOD_CTX_T *psDemodCtx);
void   DTMB_SetConnType(DEMOD_CTX_T *psDemodCtx, UINT8 type);
UCHAR  DTMB_SetPowerMode(DEMOD_CTX_T *psDemodCtx, UCHAR ucMode);

UINT8  DTMB_GetSnrInfo(DEMOD_CTX_T *psDemodCtx);
UINT8 DTMB_DumpRegisters(DEMOD_CTX_T *psDemodCtx, UINT8 *buffer, UINT8 bank_no, UINT16 max_i2c_len);
UINT8 DTMB_ChipAnaInit(VOID);
void  DTMB_SetTsOutputMode(DEMOD_CTX_T *psDemodCtx, BOOL fgParallel);
UINT8 DTMB_SetTsPathMode(DEMOD_CTX_T *psDemodCtx, BOOL fgEnable);
TS_FMT_T DTMB_GetTsFmt(DEMOD_CTX_T *psDemodCtx);
VOID DTMB_SetTsOutput(DEMOD_CTX_T *psDemodCtx, BOOL fgOutputEnable);
UINT8 DTMB_MemoryRepair(DEMOD_CTX_T *psDemodCtx);


CHAR *DTMB_GetVer(DEMOD_CTX_T *psDemodCtx);
VOID vDTMB_GetAsicVer(DEMOD_CTX_T *psDemodCtx);

/**********************************************************************/
/*      I2C Interface                                                 */
/**********************************************************************/
/*
UINT8 u1I2cDemWrite(DEMOD_CTX_T *psDemodCtx, UINT8 u1I2cAddr, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount);
UINT8 u1I2cDemRead(DEMOD_CTX_T *psDemodCtx, UINT8 u1I2cAddr, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount);
#define ucI2cWriteSpeed(psDemodCtx, ucI2cAddr, ucRegAddr, pucBuffer, ucByteCount)  \
        u1I2cDemWrite(psDemodCtx, ucI2cAddr, ucRegAddr, pucBuffer, ucByteCount)
*/
DEMOD_CTX_T *DTMB_DemodCtxCreate(void);
void DTMB_DemodCtxDestroy(DEMOD_CTX_T *p);

#endif // _MT5391_PI_DEMOD_H_
