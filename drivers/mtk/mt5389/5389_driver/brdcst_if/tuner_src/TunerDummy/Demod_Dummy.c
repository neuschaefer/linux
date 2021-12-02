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
#include "drvapi_tuner.h"
#include "TDIS.h"
#include "TDMgr.h"

#if 0
void DVBT_SetDmxExt(void)
{
//do nothing
	return;
}

//----------------------------------------------------------------------------- 
/** DVBT_SetTSInterface
 *  Set TS interface
 *  @param  fgParallel  set interface to parallel or serial mode. TRUE: Parallel, FALSE: serial
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
void DVBT_SetTSInterface(BOOL fgParallel)
{
    //do nothing
    return;
}

//----------------------------------------------------------------------------- 
/** DVBT_SetTSPath
 *  Set TS routing path
 *  @param  fgEnable  set TS path to tri-state or pass-through. TRUE: pass-through, FALSE: tri-state
 *  @retval  demod lock status, 0: unlock, 1: lock.
 */
//-----------------------------------------------------------------------------
UINT8 DVBT_SetTSPath(BOOL fgEnable)
{
    //do nothing
    return 0;
}
#endif
/**********************************************************************/
#define fcTUNER_I2C_DEMOD       0           // 0: None
                                            // 1: MT511x
                                            // 2: MT513x

#define cI2C_TIMING             0x100
#if fcTUNER_I2C_DEMOD == 1
#define cDEMOD_I2C_ADDR         0x92
#define cREG_TUNER_I2C          0x59
#define cpTUNER_I2C_ON          0
#elif fcTUNER_I2C_DEMOD == 2
#define cDEMOD_I2C_ADDR         0x82
#define cREG_TUNER_I2C          0x90
#define cpTUNER_I2C_ON          7
#endif

/**********************************************************************/
#if fcADD_ACQ_CHK
BOOL Dummy_TunerAcq(PTD_SPECIFIC_CTX ptTDSpecificCtx,
#else
void Dummy_TunerAcq(PTD_SPECIFIC_CTX ptTDSpecificCtx,
#endif
                    DRV_CONN_TYPE_T e_conn_type,
                    void* pv_conn_info,
                    SIZE_T z_conn_info_len,
                    x_break_fct _BreakFct,
                    void* pvArg);
S32 Dummy_TunerClose(PTD_SPECIFIC_CTX ptTDSpecificCtx);
void Dummy_TunerGetSignal(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                    DRV_CONN_TYPE_T e_conn_type,
                    SIGNAL *_pSignal);
U8 Dummy_TunerGetSignalLevel(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                DRV_CONN_TYPE_T e_conn_type);
S16 Dummy_TunerGetSignalLeveldBm(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                    DRV_CONN_TYPE_T e_conn_type);
UINT16 Dummy_TunerGetSignalPER(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                    DRV_CONN_TYPE_T e_conn_type);
UINT16 Dummy_TunerGetSignalSNR(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                    DRV_CONN_TYPE_T e_conn_type);
S16 Dummy_TunerGetSync(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                        DRV_CONN_TYPE_T e_conn_type);
char *Dummy_TunerGetVer(PTD_SPECIFIC_CTX ptTDSpecificCtx);
S32 Dummy_TunerNimTest(PTD_SPECIFIC_CTX ptTDSpecificCtx);
S32 Dummy_TunerOpen(PTD_CTX ptTDCtx, TUNER_CONFIG_T * ptTunerCfgInfo,
                     PTD_SPECIFIC_CTX* pptTDSpecificCtx);
S32 Dummy_TunerRegRead(UCHAR ucI2cAddress, UCHAR ucRegAddr, UCHAR *pucBuffer, U16 ucByteCount);
void Dummy_TunerSetMpgFmt(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                            MPEG_FMT_T *pt_mpeg_fmt);
void Dummy_TunerSetRegSetting(PTD_SPECIFIC_CTX ptTDSpecificCtx, UCHAR ucRegSet,
                            UCHAR ucRegAddr, UCHAR ucRegValue);
void Dummy_TunerShowRegSetting(PTD_SPECIFIC_CTX ptTDSpecificCtx);
void Dummy_TunerTestI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx);
void Dummy_TunerDetachI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx, BOOL bDetach);
void Dummy_TunerCommand(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                        INT32 i4Argc, const CHAR** aszArgv);
VOID Dummy_TunerDisc(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                DRV_CONN_TYPE_T e_conn_type);
VOID Dummy_TunerDtdCommand(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                        INT32 i4Argc, const CHAR** aszArgv);
VOID Dummy_TunerAtdCommand(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                        INT32 i4Argc, const CHAR** aszArgv);
INT32 Dummy_TunerSetAttribute(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                           DRV_SET_TYPE_T  e_set_type,
                           const VOID *pvAnaAttribute,
                           SIZE_T zAnaAttributeLen);
INT16 Dummy_TunerGetAttribute(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                           DRV_GET_TYPE_T  e_get_type,
                           void *pvAnaAttribute,
                           SIZE_T* pzAnaAttributeLen);
#ifdef fcADD_DIAG_INFO
INT32 Dummy_TunerSetDiagCmd(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                        DRV_CUSTOM_TUNER_SET_TYPE_T     e_tuner_set_type,
                        VOID*                           pv_set_info,
                        SIZE_T                          z_size);
INT32 Dummy_TunerGetDiagInfo(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                        DRV_CUSTOM_TUNER_GET_TYPE_T     e_tuner_get_type,
                        VOID*                           pv_get_info,
                        SIZE_T*                         pz_size);
#endif

UINT32 SIF_Read(UINT32 u4ClkDiv, UCHAR ucDev, UINT32 u4Addr, UCHAR *pucValue, UINT32 u4Count);
UINT32 SIF_Read_Only(UINT32 u4ClkDiv, UCHAR ucDev, UCHAR *pucValue, UINT32 u4Count);
UINT32 SIF_Write(UINT32 u4ClkDiv, UCHAR ucDev, UINT32 u4Addr, UCHAR *pucValue, UINT32 u4Count);
/**********************************************************************/
void PD_Dummy_Register_LTDIS_Fct(PTD_CTX ptTDCtx)
{
    TUNER_DEVICE_CTX_T*     pTunerDeviceCtx = ptTDCtx;

    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerClose = Dummy_TunerClose;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerAcq = Dummy_TunerAcq;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDisc = Dummy_TunerDisc;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGerSync = Dummy_TunerGetSync;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignal = Dummy_TunerGetSignal;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalLevel = Dummy_TunerGetSignalLevel;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalLeveldBm = Dummy_TunerGetSignalLeveldBm;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalPER = Dummy_TunerGetSignalPER;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalSNR = Dummy_TunerGetSignalSNR;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetMpgFmt = Dummy_TunerSetMpgFmt;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetVer = Dummy_TunerGetVer;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerNimTest = Dummy_TunerNimTest;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerCommand = Dummy_TunerCommand;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDtdCommand = Dummy_TunerDtdCommand;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerAtdCommand = Dummy_TunerAtdCommand;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetRegSetting = Dummy_TunerSetRegSetting;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerShowRegSetting = Dummy_TunerShowRegSetting;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerTestI2C = Dummy_TunerTestI2C;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDetachI2C = Dummy_TunerDetachI2C;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetAttribute = Dummy_TunerSetAttribute;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetAttribute = Dummy_TunerGetAttribute;
#ifdef fcADD_DIAG_INFO
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetDiagCmd = Dummy_TunerSetDiagCmd;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetDiagInfo = Dummy_TunerGetDiagInfo;
#endif

}

/***********************************************************************/
/*      Private (static) Function Prototypes                           */
/***********************************************************************/
#if fcTUNER_I2C_DEMOD
S16 RegGetRegisters(U16 i2c_addr, S32 reg_id, U16 reg_num, UCHAR *data)
{
    if (reg_num > 8)
    {
        mcSHOW_HW_MSG(("I2C transaction size > 8\n"));
        ASSERT(0);
    }
    if (SIF_Read(cI2C_TIMING, i2c_addr, reg_id, data, reg_num) == 0)
    {
        // Report failure
        return -1;
    }
    return 0;
}

S16 RegSetRegisters(U16 i2c_addr, S32 reg_id, U16 reg_num, UCHAR *data)
{
    if (reg_num > 8)
    {
        mcSHOW_HW_MSG(("I2C transaction size > 8\n"));
        ASSERT(0);
    }
    if (SIF_Write(cI2C_TIMING, i2c_addr, reg_id, data, reg_num) == 0)
    {
        // Report failure
        return -1;
    }
    return 0;
}

void Set_TunerI2C(BOOL fgTurnOn)
{
UCHAR   ucValue;

    RegGetRegisters(cDEMOD_I2C_ADDR, cREG_TUNER_I2C, 1, &ucValue);
    if (fgTurnOn)
        mcSET_BIT(ucValue, cpTUNER_I2C_ON); // Access Tuner
    else
        mcCLR_BIT(ucValue, cpTUNER_I2C_ON); // Bypass Tuner

//    mcSHOW_USER_MSG(("TunerI2C (%d): %02X_%02X = %02X\n",
//             fgTurnOn, cDEMOD_I2C_ADDR, cREG_TUNER_I2C, ucValue));
    RegSetRegisters(cDEMOD_I2C_ADDR, cREG_TUNER_I2C, 1, &ucValue);
}
#endif

/***********************************************************************/
char *Dummy_TunerGetVer(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    return ("Tuner_Dummy\n");
}

/***********************************************************************/
void Dummy_TunerSetRegSetting(PTD_SPECIFIC_CTX ptTDSpecificCtx, UCHAR ucRegSet,
                            UCHAR ucRegAddr, UCHAR ucRegValue)
{
}

void Dummy_TunerShowRegSetting(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
}

void Dummy_TunerTestI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
}

/**********************************************************************/
S32 Dummy_TunerRegRead(UCHAR ucI2cAddress, UCHAR ucRegAddr, UCHAR *pucBuffer, U16 ucByteCount)
{
    return (DRVAPI_TUNER_OK);
}

/**********************************************************************/
S32 Dummy_TunerNimTest(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    return 0;
}



/**********************************************************************/
S32 Dummy_TunerOpen(PTD_CTX ptTDCtx, TUNER_CONFIG_T * ptTunerCfgInfo,
                     PTD_SPECIFIC_CTX* pptTDSpecificCtx)
{
    PD_Dummy_Register_LTDIS_Fct(ptTDCtx);
    return (DRVAPI_TUNER_OK);
}

/**********************************************************************/
S32 Dummy_TunerClose(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    return (DRVAPI_TUNER_OK);
}

/**********************************************************************/
S16 Dummy_TunerGetSync(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                        DRV_CONN_TYPE_T e_conn_type)
{
    return 0;
}

#if fcADD_ACQ_CHK
BOOL Dummy_TunerAcq(PTD_SPECIFIC_CTX ptTDSpecificCtx,
#else
void Dummy_TunerAcq(PTD_SPECIFIC_CTX ptTDSpecificCtx,
#endif
                    DRV_CONN_TYPE_T e_conn_type,
                    void* pv_conn_info,
                    SIZE_T z_conn_info_len,
                    x_break_fct _BreakFct,
                    void* pvArg)
{

#if fcADD_ACQ_CHK
BOOL    fgRetSts = FALSE;

    return fgRetSts;
#endif
}

/**********************************************************************/
void Dummy_TunerGetSignal(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                    DRV_CONN_TYPE_T e_conn_type,
                    SIGNAL *_pSignal)
{
}

/**********************************************************************/
U8   Dummy_TunerGetSignalLevel(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                DRV_CONN_TYPE_T e_conn_type)
{
    return 90;
}

/**********************************************************************/
S16 Dummy_TunerGetSignalLeveldBm(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                    DRV_CONN_TYPE_T e_conn_type)
{
    return -300;
}

/**********************************************************************/
UINT16  Dummy_TunerGetSignalSNR(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                    DRV_CONN_TYPE_T e_conn_type)
{
    return 0;
}

UINT16  Dummy_TunerGetSignalPER(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                    DRV_CONN_TYPE_T e_conn_type)
{
    return 0;
}

/**********************************************************************/
void Dummy_TunerSetMpgFmt(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                            MPEG_FMT_T *pt_mpeg_fmt)
{
}

/**********************************************************************/
void Dummy_TunerDetachI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx, BOOL bDetach)
{
#if fcTUNER_I2C_DEMOD
    Set_TunerI2C(bDetach);
#endif
}

/**********************************************************************/
void Dummy_TunerCommand(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                        INT32 i4Argc, const CHAR** aszArgv)
{
UCHAR   ucCmdId = 0;
UINT16   u2RegAddr;

    if (i4Argc < 1)
    {
        mcSHOW_USER_MSG(("Cmd: \n"));
        mcSHOW_USER_MSG(("\t t test\n"));
    }

    if (i4Argc > 0)
        ucCmdId = *((CHAR *) aszArgv[0]);

    switch (ucCmdId)
    {
        case 'f':
            if (i4Argc > 1)
            {
            //    u2RegAddr = axtoi((CHAR *) aszArgv[1]);
                u2RegAddr = StrToInt(aszArgv[1]);
                mcSHOW_USER_MSG(("u2RegAddr = %d\n", u2RegAddr));
            }
            break;

        default:
            break;
    }
}

/**********************************************************************/
VOID Dummy_TunerDisc(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                DRV_CONN_TYPE_T e_conn_type)
{
}

/**********************************************************************/
VOID Dummy_TunerDtdCommand(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                        INT32 i4Argc, const CHAR** aszArgv)
{
}

/**********************************************************************/
VOID Dummy_TunerAtdCommand(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                        INT32 i4Argc, const CHAR** aszArgv)
{
}

/**********************************************************************/
INT32 Dummy_TunerSetAttribute(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                           DRV_SET_TYPE_T  e_set_type,
                           const VOID *pvAnaAttribute,
                           SIZE_T zAnaAttributeLen)
{
    return RMR_OK;
}


STATIC TUNER_ANA_ATTRIBUTE_T sTunerAnaAttribute =
{
    45150000,
    873225000,
    (0x5a / 2) * 50000
};
STATIC UINT32  au4TvSysGroup[] =
{
    (TV_SYS_MASK_B | TV_SYS_MASK_G),
    (TV_SYS_MASK_D | TV_SYS_MASK_K),
    (TV_SYS_MASK_I),
    (TV_SYS_MASK_L),
    (TV_SYS_MASK_L_PRIME),
    (TV_SYS_MASK_M)
};
/**********************************************************************/
INT16 Dummy_TunerGetAttribute(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                           DRV_GET_TYPE_T  e_get_type,
                           void *pvAnaAttribute,
                           SIZE_T* pzAnaAttributeLen)
{
    switch (e_get_type)
    {
        case TUNER_GET_TYPE_ANA_ATTRIBUTE:
        {
            *(TUNER_ANA_ATTRIBUTE_T *) pvAnaAttribute = sTunerAnaAttribute;
            break;
        }
        case TUNER_GET_TYPE_TV_SYS_GROUP:
        {
        UINT8   u1TvSysGroupIdx;
        TUNER_TV_SYS_GROUP_INFO_T   *psTvSysGroup;

            psTvSysGroup = (TUNER_TV_SYS_GROUP_INFO_T *) pvAnaAttribute;
            u1TvSysGroupIdx = psTvSysGroup->ui1_tv_sys_group_idx;
            psTvSysGroup->ui4_tv_sys_mask = TV_SYS_MASK_NONE;
            if (u1TvSysGroupIdx < 1)
            {
                psTvSysGroup->ui4_tv_sys_mask =
                        au4TvSysGroup[u1TvSysGroupIdx];
            }
            break;
        }
    }
    return RMR_OK;
}

#ifdef fcADD_DIAG_INFO
/**********************************************************************/
INT32 Dummy_TunerSetDiagCmd(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                        DRV_CUSTOM_TUNER_SET_TYPE_T     e_tuner_set_type,
                        VOID*                           pv_set_info,
                        SIZE_T                          z_size)
{
    return RMR_OK;
}

/**********************************************************************/
INT32 Dummy_TunerGetDiagInfo(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                        DRV_CUSTOM_TUNER_GET_TYPE_T     e_tuner_get_type,
                        VOID*                           pv_get_info,
                        SIZE_T*                         pz_size)
{
    return RMR_OK;
}
#endif
#if 0
/***********************************************************************/
/*              For ATD RISC IF                                        */
/***********************************************************************/
UINT8 u1AnaDemRead(void *psI2cCtx, UINT8 u1I2cAddr, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount)
{
UINT8 u1RegAddr;
UINT8 u1RetSts = 0;

    u1RegAddr = mcLOW_BYTE(u2RegAddr);

    if (SIF_Read(cI2C_TIMING, u1I2cAddr, u1RegAddr, pu1Buffer, u2ByteCount) == 0)
    {
        u1RetSts = 1;
    }
    return u1RetSts;
}
/***********************************************************************/
UINT8 u1AnaDemWrite(void *psI2cCtx, UINT8 u1I2cAddr, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount)
{
UINT8 u1RegAddr;
UINT8 u1RetSts = 0;

    u1RegAddr = mcLOW_BYTE(u2RegAddr);
    if (SIF_Write(cI2C_TIMING, u1I2cAddr, u1RegAddr, pu1Buffer, u2ByteCount) == 0)
    {
        u1RetSts = 1;
    }
    return u1RetSts;
}
BOOL Ana_SwitchRfAgc(VOID)
{
    mcSHOW_USER_MSG(("TunerDummy without provide API Ana_SwitchRfAgc() implementation\n"));
    return FALSE;
}
#endif

