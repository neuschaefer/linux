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
/************************************************************************* 
* 
* Filename: 
* --------- 
* 
* Description: 
* ------------ 
*    
*    
* Author: 
* ------- 
*    Menghau Wu
* 
* Last changed: 
* -------------  
* $Author: dtvbm11 $  
* 
* $Modtime: 2007/02/06 13:45$   
* 
* $Revision: #1 $
*************************************************************************/

#ifndef _MT5387ATD_PI_DEMOD_H_
#define _MT5387ATD_PI_DEMOD_H_

#include "pd_tuner_type.h"
#include "atd_register.h"
#include "tuner_interface_if.h"
#include "u_handle.h"
#include "fe_tuner_common_if.h"


/***********************************************************************/
/*      Defines                                                        */
/***********************************************************************/
#define SCAN_DIRECTION_DEC                0
#define SCAN_DIRECTION_INC                1
#define TV_FINE_TUNE_THRESHOLD_KHZ  4500    // -2.25MHz ~  +2.25MHz

#define cATD_VIF_UNLOCK        0
#define cATD_VIF_PARTIAL_LOCK  1
#define cATD_VIF_FULL_LOCK     2 //bit 2

#define cATD_VIF_PARTIAL_LOCK_RANGE   1500 //kHz
#define cATD_VIF_FULL_LOCK_RANGE      50   //kHz

#define cpANA_LOCK_STS_VIF      0
#define cpANA_LOCK_STS_TVD      1
#define cpANA_LOCK_STS_BREAK    2 //bit 2

#define MASK_PATCH_CR           0x01
#define MASK_PATCH_DRO          0x02
#define MASK_PATCH_CCI          0x04
#define MASK_PATCH_SENS         0x08
#define MASK_PATCH_AGC          0x10
#define MASK_PATCH_STCHR        0x20
#define MASK_PATCH_SBEAT        0x40

#ifndef tuner_break_fct
#define tuner_break_fct
typedef INT32     (*x_break_fct)(void *pArg);
#endif

typedef struct _ATD_CTX_T
{
// Hardware Configure
    UINT8           I2cAddress;
    UINT32         u4Frequency;             /* carrier frequency (in KHz)   */
    UINT8           u1SubSysIdx;
// Status
    UINT16	    u2VOPCpo;
    INT32	    s4Cfo;
    UINT8           u1ControlStatus;
    UINT8           u1VIFLock;
    BOOL            fgPRALock;
    BOOL            fgCRLock;
    BOOL            fgCheckCR;
    BOOL            fgDemodReset;
    BOOL            fgAutoSearch;
    BOOL            fgAFT;
    UINT8           u1PF;
    UINT8           u1UpChkCnt;
    UINT8           u1UpNakCnt;
    UINT8           u1UpMisCnt;
    UINT8           u1TunerType;

    INT8    s1RFmin;
    INT8    s1IFmin;

    //GEN_TUNER_CTX_T sTunerCtx;
    //void            *psI2cCtx;
    HANDLE_T          hHostCmdLock;  // Semaphore
    HANDLE_T          hRegLock;      // Semaphore
    HANDLE_T          hLogLock;      // Semaphore
    BOOL            fgDemodInitFlag;
    HANDLE_T          hDemodAcq;     // Semaphore

    VOID        *pDigiTunerCtx;
    x_break_fct isBreak;
//    VOID        *isBreak;
    VOID        *pvBreakArg;
    //THREAD_T        t_monitor_thread;

    ITUNER_CTX_T *pTCtx;

    
} ATD_CTX_T;

/***************** *********************/
//#define REG_SIZE                250
#define cMAX_READ_NUM           ccCMD_REG_NUM
//#define cMAX_CHAN_TABLE_NUM     128

enum
{
    cUP_LOAD_OK = 0,
    cUP_LOAD_ERR_I2C,
    cUP_LOAD_ERR_HW_RDY,
    cUP_LOAD_ERR_CHKSUM_RDY,
    cUP_LOAD_ERR_CHKSUM_OK,
    cUP_LOAD_ERR_CNT
};

enum
{
	CH_SEARCH_BLIND = 0,
	CH_SEARCH_UPDOWN
};

enum
{
	SELECT_CCI_NOTCH = 0,
	SELECT_ANIR
};


#define cMSG_UP_ERR_I2C         "I2c error!"
#define cMSG_UP_ERR_HW_RDY      "HW not ready!"
#define cMSG_UP_ERR_CHKSUM_RDY  "Chksum not ready!"
#define cMSG_UP_ERR_CHKSUM_OK   "Chksum not Ok!"

#define cMAX_DIVERSITY_NUM      8 


/*********** Register Addr. *************************************/

#define ATD_REG_RISCIF_CFG   (UINT16) 0x4A4
#define ATD_REG_RISCIF_WDATA (UINT16) 0x4B0
#define ATD_REG_RISCIF_CTRL  (UINT16) 0x4B4
#define ATD_REG_RISCIF_RDATA (UINT16) 0x4B8
#define ATD_REG_RISCIF_STAT  (UINT16) 0x4BC

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



/***********************************************************************/
//#define TUNER_SOURCE_LABEL      ""
#define cMAX_I2C_LEN            8

/***********************************************************************/
#define mcCTMR_DBG_MSG(_x_)     mcSHOW_DBG_MSG2(_x_)
#define mcCTMR_DBG_MSG2(_x_)    mcSHOW_DBG_MSG2(_x_)

#define mcCHECK_BLOCK_I2C(ucRetSts)
#define mcCHECK_BLOCK_I2C0()
#define mcBLOCK_I2C(ucBlockSts)

/***********************************************************************/
/*                                                                     */
/***********************************************************************/
UINT8  ATD_SetReg(ATD_CTX_T *psDemodCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount);
UINT8  ATD_GetReg(ATD_CTX_T *psDemodCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount);
INT32  ATD_Initialize(ATD_CTX_T *psDemodCtx, UINT8 u1I2cAddress, BOOL fgLoadCode);
INT32  ATD_DemodReset(ATD_CTX_T *psDemodCtx);
UINT8  ATD_GetSignalLevel(ATD_CTX_T *psDemodCtx);
UINT16 ATD_GetRFAGC(ATD_CTX_T *psDemodCtx);
UINT32 ATDPI_GetNoiseLevel(ATD_CTX_T *psDemodCtx);
UINT16 ATDPI_GetIFAGC(ATD_CTX_T *psDemodCtx);
BOOL   ATDPI_SetLNA(ATD_CTX_T *psDemodCtx, BOOL fgLNA);
UINT8  ATD_ChipInit(ATD_CTX_T *psDemodCtx);                                     
UINT8  ATD_StartAcq(ATD_CTX_T *psDemodCtx, UINT8 fgScanMode);
INT32  ATD_GetCFOKHz(ATD_CTX_T *psDemodCtx, BOOL fgLogMessage);
UINT16 ATD_GetVOPCpo(ATD_CTX_T *psDemodCtx);
UINT8  ATD_GetPRALock(ATD_CTX_T *psDemodCtx);
UINT8  ATD_GetVIFLock(ATD_CTX_T *psDemodCtx, BOOL fgLogMessage);
void   ATD_Patch(ATD_CTX_T *psDemodCtx); 
void   ATD_DisConnect(ATD_CTX_T *psDemodCtx);
void   ATD_SetEQ(ATD_CTX_T *psDemodCtx, ITUNEROP_T EqScriptType);
void   ATD_SetPF(ATD_CTX_T *psDemodCtx, UINT8 u1PF);
void   ATD_SetScanMode(ATD_CTX_T *psDemodCtx);
void   ATD_SetSystem(ATD_CTX_T *psDemodCtx, UINT8 u1SubSysIdx);
void   ATD_SetMonitor(ATD_CTX_T *psDemodCtx, BOOL fgMonitor);
void   ATD_SetNormalMode(ATD_CTX_T *psDemodCtx);
BOOL   ATD_SetFreq(ATD_CTX_T *psDemodCtx, UINT32 u4FreqInKHz, UINT8 fgAutoSearch);
BOOL   Qlab_SetMonIndicator(UINT32 TestItems,UINT32 ucPar1,UINT32 ucPar2);
void   Qlab_PrintSelectedIndicator(ATD_CTX_T *psDemodCtx);
void   Qlab_Indicator(ATD_CTX_T *psDemodCtx,UINT16* Values);

VOID ATD_SetAgc(ATD_CTX_T *psDemodCtx);
ATD_CTX_T *ATD_DemodCtxCreate(void);
void ATD_DemodCtxDestroy(ATD_CTX_T *p);

//INT16 Tuner_SetFreq_DVBT(VOID *psTunerCtx, UINT32 Freq, UINT8 u1Mode, UINT16 u2Step);
VOID *GetDigiDvbtTunerCtx(VOID);

EXTERN BOOL _fgAutoSearch;
extern void DigTunerBypassI2C(BOOL bSwitchOn);

EXTERN BOOL fgDrvTvdCheckTVDLock(UINT16 bTryLoopCnt);
BOOL fgGetTVDSts(ATD_CTX_T *psDemodCtx,UINT16 u1ChkTime);

#endif // _ATD_PI_DEMOD_H_
