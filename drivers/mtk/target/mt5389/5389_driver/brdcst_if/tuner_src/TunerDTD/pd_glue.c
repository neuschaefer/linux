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
/*  FILE DESCRIPTION                                                   */
/*                                                                     */
/*  $RCSfile: pd_glue.c,v $                                            */
/*                                                                     */
/***********************************************************************/
/***********************************************************************/
/*              Includes                                               */
/***********************************************************************/

#include "PI_Def_dbg.h"
#include "pi_demod.h"
#include "dtd_if.h"
#include "pd_api.h"
//#include "dtd_pd_glue_if.h"

#include "u_tuner.h"
#include "TDMgr.h"
#include "UtilAPI.h"

//#include "rf_tuner_api.h"

#include "tuner_interface_if.h"
#include "ctrl_bus.h"

/***********************************************************************/
#ifdef fcADD_DIAG_INFO
#include "u_drv_cust.h"
#endif
#include "x_stl_lib.h"

/***********************************************************************/
/*              Defines                                                */
/***********************************************************************/
//#if !defined(CC_MTAL) && !defined(CC_SDAL) //linux platform
//#define DIGUS_ENABLE_MONITOR
//#else

//#endif

#if fcMEASURE_DRIVER_TIME
TIME_TICK_T u4TickStartMeasure, u4PrevStartTime, u4TickCurMeasure;
TIME_DIFF_T u4TickDiffCnt0, u4TickDiffCnt1;
#endif

static TUNER_DEVICE_CTX_T*      pDTDunerDeviceCtx = NULL;

TIME_TICK_T u4ConStartTick;
UINT32 u4PreMonitorTime;


#define cMONITOR_PERIOD_MS      700

#define cI2C_TIMING             270

UINT32  u4TunerCtrlForce = 0;
UINT8   u1TunerDefTypeForce = 0;

/***********************************************************************/
/*              Global variables                                       */
UINT8 gfgIFTrap=0x00;//To control IF Trap ON/OFF for tuner TDAU4D01A
/***********************************************************************/
#if 0
#if (fcTUNER_TYPE == cDTD_TUNER_ALL)
STATIC UINT8 _u1TunerType;
#endif
#endif
/***********************************************************************/
/*              External declarations                                  */
/***********************************************************************/
//EXTERN TUNER_API_FCT_TBL_T *pTuner_API_Fct;
//#if (fcTUNER_TYPE != cDTD_TUNER_ALL)
//EXTERN VOID Tuner_Register_Fct(TUNER_API_FCT_TBL_T *ptuner_Fct);
//#else
//EXTERN VOID Tuner_Register_Fct(TUNER_API_FCT_TBL_T *ptuner_Fct, UINT8 u1TunerType);
//#endif


/***********************************************************************/
/*              Private (static) Function Prototypes                   */
/***********************************************************************/
#ifdef fcEXTERN_DEMOD
STATIC VOID vResetDemod(UINT16 u2DelayTime)
{
UINT32 u4RstPinNum;

    // Config GPIO to reset demod
    mcSHOW_DRVAPI_MSG(("vResetDemod\n"));
    if (DRVCUST_OptQuery(eDemodResetGpio, &u4RstPinNum) == 0)
    {
        GPIO_SetOut(u4RstPinNum, 0);
        mcDELAY_MS(u2DelayTime);
        GPIO_SetOut(u4RstPinNum, 1);
        mcDELAY_MS(20); // Delay 20 ms for CR[DTV00030459]. Or it will init fail in rel image
        mcSHOW_DBG_MSG(("DTD Reset Demod GPIO=0x%x, 0->1, %dms\r\n",u4RstPinNum, u2DelayTime));
    }
}
#endif

STATIC VOID pi_setRegSetting(TD_DTD_CTX_T* psTDDTDCtx, UINT8 u1RegSet,
                            UINT8 u1RegAddr, UINT8 u1RegValue)
{
}

//-----------------------------------------------------------------------------
/*
 *  PD_DTDStopMonitorEngine
 *  To create and start the monitor thread
 *  @param  v0od.
 *  @retval   0 ==> Close successful.
 */
//-----------------------------------------------------------------------------
//#ifdef DIGUS_ENABLE_MONITOR
STATIC S32 PD_DTDStopMonitorEngine(TD_DTD_CTX_T *psTDDTDCtx)
{
//    psTDDTDCtx->fgMonitorThreadRunning = FALSE; //set to FALSE before unlock sema t_escape_mon_suspend
    while (psTDDTDCtx->t_monitor_thread){
        mcDELAY_MS(10); // in ms
    }
    mcSHOW_DRVAPI_MSG(("DTD PD_StopMonitorEngine success\n"));
    return (DRVAPI_TUNER_OK);
}
//#endif
//-----------------------------------------------------------------------------
/*
 *  MonitorProcess
 *  Main Monitor thread ==> ISP
 *  @param  *pvArg  for pdctx.
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
//#ifdef DIGUS_ENABLE_MONITOR
STATIC void MonitorProcess(VOID *pvArg)
{

    TD_DTD_CTX_T *psTDDTDCtx = *(TD_DTD_CTX_T **) pvArg;
    DEMOD_CTX_T *psDemodCtx = psTDDTDCtx->psDemodInbInfo;
    TIME_TICK_T u4MonitorNowTick;
    TIME_DIFF_T u4MonitorPreTick;
    UINT32  u4DiffTime;


    mcSHOW_DBG_MSG(("DTD MonitorProcess start\n"));
    while (psTDDTDCtx->fgMonitorThreadRunning)
    {
     //   if (fgIsDisconnected)
        if (psTDDTDCtx->fgBreakMonitor)
        {
            // After mcMUTEX_LOCK(t_escape_mon_suspend), monitor thread will be suspended until
            // TunerAcq or TunerClose invoked.
            mcSHOW_DBG_MSG(("Entering DTD mon_suspend\n"));
            mcMUTEX_LOCK(psTDDTDCtx->t_escape_mon_suspend);
            mcSHOW_DBG_MSG(("Escape DTD mon_suspend\n"));
        }
        else
        {
         //   mcSEMA_LOCK_HAL(psTDDTDCtx->hHalLock);

             mcSHOW_DBG_MSG4(("===DTD Monitor Thread ===\n"));

             mcGET_SYS_TIME(u4MonitorNowTick);
             u4DiffTime = mcCONV_SYS_TIME(u4MonitorNowTick) - u4PreMonitorTime;
             mcDELAY_MS(cMONITOR_PERIOD_MS - u4DiffTime);
             mcGET_SYS_TIME(u4MonitorPreTick);
             u4PreMonitorTime =  mcCONV_SYS_TIME(u4MonitorPreTick);

             mcSEMA_LOCK_HAL(psTDDTDCtx->hHalLock);
             DTD_UpdateInfo(psDemodCtx);
             DTD_AccumPER(psDemodCtx);
             mcSEMA_UNLOCK_HAL(psTDDTDCtx->hHalLock);

             //mcSHOW_DBG_MSG4(("===DTD Monitor Thread ===\n"));
          //  mcSEMA_UNLOCK_HAL(psTDDTDCtx->hHalLock);
          //  mcDELAY_MS(cMONITOR_PERIOD_MS);
        }
    }
    psTDDTDCtx->t_monitor_thread = (HANDLE_T)NULL;
    mcSHOW_DBG_MSG(("DTD MonitorProcess exit!\n"));
    mcTHREAD_DESTROY();
}
//#endif

//-----------------------------------------------------------------------------
/**  PD_DTDStartMonitorEngine
 *  To create and start the monitor thread
 *  @param  *psTDDTDCtx Pd handler.
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
//#ifdef DIGUS_ENABLE_MONITOR
STATIC S32 PD_DTDStartMonitorEngine(TD_DTD_CTX_T *psTDDTDCtx)
{
    // x_thread_create(HANDLE_T *ph_th_hdl, CHAR *ps_name, SIZE_T z_stack_size,
    //                 UINT8 ui1_pri, x_os_thread_main_fct pf_main_rtn,
    //                  SIZE_T z_arg_size, VOID *pv_arg);
    // Important: x_thread_create will copy pv_arg to its memory while not
    // directly use pv_arg. Therefore, if we want thread to share the same
    // context as caller, we need set pv_arg as "pointer of context".
    psTDDTDCtx->fgMonitorThreadRunning = TRUE;
    if (mcTHREAD_CREATE(&psTDDTDCtx->t_monitor_thread,
            "US_DTD_MonThread",
             cTHREAD_STACK_SIZE, cTHREAD_PRIORITY,
            MonitorProcess, sizeof(void*), (VOID*)&psTDDTDCtx) != OSR_OK)
    {
        mcSHOW_DBG_MSG(("PD_DTDStartMonitorEngine (mcTHREAD_CREATE): DRVAPI_TUNER_ERROR!\n"));
        return (DRVAPI_TUNER_ERROR);
    }
    mcSHOW_DRVAPI_MSG(("PD_DTDStartMonitorEngine success\n"));
    return (DRVAPI_TUNER_OK);
}
//#endif

/***********************************************************************/
INT32 pi_regRead(UINT8 u1I2cAddress, UINT8 u1RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount)
{


   // if (u1I2cRead(NULL, u1I2cAddress, u1RegAddr, pu1Buffer, u2ByteCount))
if (ICtrlBus_I2cDemodRead(270, u1I2cAddress, u1RegAddr, pu1Buffer, u2ByteCount))
    {
         mcSHOW_HW_MSG(("IIC Bus Read Error!\n"));
         return (DRVAPI_TUNER_ERROR);
    }
    else
    {
        return (DRVAPI_TUNER_OK);
    }

}


/***********************************************************************/
VOID pi_showRegSetting(TD_DTD_CTX_T* psTDDTDCtx)
{
}

/***********************************************************************/
VOID pi_testI2C(TD_DTD_CTX_T* psTDDTDCtx)
{
    //vEnableI2C_Mode();
    fgEnableI2C_Flag = TRUE;
}

/***********************************************************************/
VOID pi_TunerBypassI2C(TD_DTD_CTX_T* psTDDTDCtx, BOOL bSwitchOn)
{
DEMOD_CTX_T *psDemodCtx = psTDDTDCtx->psDemodInbInfo;

    if (bSwitchOn)
    {
    // Set TunerBypass TRUE to allow Analog driver to control analog tuner.
        //DTD_Acquire(psDemodCtx, FALSE);
        DTD_TunerI2c(psDemodCtx, TRUE);
        //DTD_Sleep(psDemodCtx);
    }
    else
    {
        DTD_TunerI2c(psDemodCtx, FALSE);
    }
}

/***********************************************************************/
INT32 pi_nimTest(TD_DTD_CTX_T* psTDDTDCtx)
{
DEMOD_CTX_T *psDemodCtx = psTDDTDCtx->psDemodInbInfo;
//UINT8   aucData[cMAX_READ_NUM + 1];         // For Host Cmd
UINT8   ucValue;
PARAM_SETFREQ_T param;

#if 1
    //for (ii = 0; ii < cPHY_TUNER_NUM; ii++)
    //{
        //psDemodCtx->I2cAddress = ucGetI2cAddr(ii, DEMOD_INB_ADDR);
        if (DTD_GetReg(psDemodCtx, DTD_REG_Chip_ID, &ucValue, 1) == 0)
        {
            mcSHOW_USER_MSG(("NG"));
            return -1;                      // I2C fail
        }
        if (mcGET_FIELD_CMB(ucValue, CHIP_ID_GEN) != 1)    // Chip Generation
        {
            mcSHOW_USER_MSG(("NG"));
            return -1;                      // Not DTD
        }

        psDemodCtx->Modulation = MOD_ATSC_8VSB;
        param.Freq=cTUNER_DEFAULT_FREQ;
        param.Modulation=MOD_ATSC_8VSB;
    //    Rf_Tuner_Init(&(psDemodCtx->sTunerCtx), ucGetI2cAddr(ii, TUNER_INB_ADDR));
        if (DTD_SetFreq(psDemodCtx, &param) != cRET_SETFREQ_OK)
        {
            mcSHOW_USER_MSG(("NG"));
            return -1;                      // I2C fail
        }
        mcSHOW_USER_MSG(("%d.%d", (UINT8) mcGET_FIELD_CMB(ucValue, CHIP_ID_GEN),    // Chip Generation, 0 = MT5111
                                  (UINT8) mcGET_FIELD_CMB(ucValue, CHIP_ID_MASK) + 1)); // Mask Rev., 1 = BO62350A
    //}
#endif
    return 0;
}

/***********************************************************************/
#define mcMONI_PRC_MSG()        mcPRN_MSG_MONI("%s-DTD_TunerMonitoring.\n", __FILE__);

#define mcMONI_PRC_FUNC()       DTD_TunerMonitoring(psPdCtx)



/***********************************************************************/
EXTERN VOID vEnableI2C_Mode(VOID);
#define mcREAD_EEPROM_FUNC(u1Func, psDemodCtx)              \
{                                                           \
    u1ByteCountRd = u1ByteCount;                            \
    if (u1ByteCountRd > cMAX_READ_NUM + 1)                  \
        u1ByteCountRd = cMAX_READ_NUM + 1;                  \
    if (EEPROM_Read((UINT64) u2RegAddr + ii, (UINT32) (VOID *) au1Data, u1ByteCountRd)) \
        break;                                              \
}

#define cASIZE                               (cMAX_READ_NUM + 4)
/***********************************************************************/
VOID pi_command(TD_DTD_CTX_T* psTDDTDCtx, INT32 i4Argc, const CHAR** aszArgv)
{
DEMOD_CTX_T *psDemodCtx = psTDDTDCtx->psDemodInbInfo;
#if ((cMAX_READ_NUM + 1) >= (cMAX_CFG_SIZE))
UINT8   au1Data[cASIZE];         // For Host Cmd
#else
//UINT8   au1Data[cMAX_CFG_SIZE];
#endif
UINT8   ii, u1ByteCount = 8;
UINT8   ucCmdId = 0;
UINT8   u1ByteCountRd;
UINT16  u2RegAddr = 0;
UINT8    fgDebug;
UINT8    u1SpecInv;
UINT16   u2IfAgcVol;
UINT16   u2SignalLevel;
INT32    SignalLeveldBm;
UINT32   u4Value1 = 0, u4Value2 = 0;

    // Initial
    mcMEMSET(au1Data, 0, sizeof(UINT8)*cASIZE);

    if (i4Argc < 1)
    {
        mcSHOW_USER_MSG(("Cmd: \n"));

        mcSHOW_USER_MSG(("\t w (0x)Addr Value: Write register\n"));
        mcSHOW_USER_MSG(("\t r (0x)Addr Count: Read register\n"));
        mcSHOW_USER_MSG(("\t v (0x)Addr Count: Read FW variables\n"));
        mcSHOW_USER_MSG(("\t x (0x)Addr Value: Write FW variable\n"));
        mcSHOW_USER_MSG(("\t h (0x)CmdId WrCmdCnt RdParaCnt Paras...: Host Cmd\n"));
        mcSHOW_USER_MSG(("\t a Direct Gain Pol: SmartAnt\n"));
    #if fcTUNER_TYPE >= cTUNER_ALL_START
        mcSHOW_USER_MSG(("\t e[1..9,a] Cfg0 Cfg1...: Write NIM_Cfg\n"));
        #if fcINTERNAL_TEST
        mcSHOW_USER_MSG(("\t ee (0x)Addr Count: Read EEPROM\n"));
        mcSHOW_USER_MSG(("\t ef u4TunerCtrlForce: Force DRVCUST_OptGet(eTunerCtrl)\n"));
        mcSHOW_USER_MSG(("\t ep u1TunerDefTypeForce: Force DRVCUST_OptGet(eTunerTypeDefault)\n"));
        #endif
    #endif
        mcSHOW_USER_MSG(("\t i MT5381 I2C Test Mode\n"));
        mcSHOW_USER_MSG(("\t 2r (0x)Addr Count: Read registers via I2C\n"));
        mcSHOW_USER_MSG(("\t 2w (0x)Addr0 Value0 Value1 ...: Write registers via I2C\n"));
        mcSHOW_USER_MSG(("\t s fgDbgOn(1:manuel,0:auto) SpecInv:(1:Inv,0:Normal): Set Spectrum polarity\n"));
        mcSHOW_USER_MSG(("\t t fgOut(1:Out,0:Normal): Set TS out\n "));
        mcSHOW_USER_MSG(("\t d: Get IfAgcVol, SignalLevel(0~100) and SignalLevel in dBm\n"));
        mcSHOW_USER_MSG(("\t b: Get TcmBer\n"));
        mcSHOW_USER_MSG(("\t n  Value: 8297 transmission phase\n"));
        mcSHOW_USER_MSG(("\t p: Parsing phase test: Pass/Fail\n"));
        mcSHOW_USER_MSG(("\t eq SciptOpt: EQBIST test\n"));
     mcSHOW_USER_MSG(("\t gs: Cmd related to DTD EQ\n"));

    }

    if (i4Argc > 0)
    {
        ucCmdId = *((CHAR *) aszArgv[0]);

        switch (ucCmdId)
        {
             case 'r':
                if (i4Argc > 1)
                {
                    mcREAD_CHK_INPUT(1);
                    mcREAD_SHOW_VALUE_MULTI(u1GetRegs, psDemodCtx);
                }
                break;

            case 'v':
                if (i4Argc > 1)
                {
                    mcREAD_CHK_INPUT(1);
                    mcREAD_SHOW_VALUE_MULTI(u1GetFwVars, psDemodCtx);
                }
                break;

            case 'w':
                mcWRITE_SET_VALUE_NORM(1, u1SetRegs, psDemodCtx);
                break;

            case 'x':
                mcWRITE_SET_VALUE_NORM(1, u1SetFwVars, psDemodCtx);
                break;

            case 's':
                if (i4Argc > 1)
                {
                    fgDebug = StrToInt((CHAR *) aszArgv[1]);
                    if (i4Argc > 2){
                        u1SpecInv = StrToInt((CHAR *) aszArgv[2]);
                    }
                    else{
                        u1SpecInv = 0;
                    }
                    DTD_SetSpcPolarity(psDemodCtx,  fgDebug,  u1SpecInv);                   
                }
                break;

            case 't':
                if (i4Argc > 1)
                {
                    fgDebug = StrToInt((CHAR *) aszArgv[1]);
                    DTD_SetTSDirectOut(psDemodCtx, fgDebug);
                }
                break;

            case 'd':
                DTD_GetSync(psDemodCtx);
                DTD_GetIfAgcVol(psDemodCtx, &u2IfAgcVol);
                u2SignalLevel = DTD_GetSignalLevel(psDemodCtx);
                SignalLeveldBm = DTD_GetSignalLeveldBm(psDemodCtx);
                mcSHOW_USER_MSG(("Signallevel= %d, SignalLeveldBm= %d dBm\n", u2SignalLevel,SignalLeveldBm));
                break;

            case 'b':
                DTD_GetTcmBER(psDemodCtx, &u4Value1, &u4Value2);
                mcSHOW_USER_MSG(("TCM BER(%d, %d)\n", u4Value1, u4Value2));
                break;

            case 'h':
                if (i4Argc > 1)
                {
                    u2RegAddr = StrToInt((CHAR *) aszArgv[1]);

                    u1ByteCount = 1;
                    u1ByteCountRd = 0;
                    if (i4Argc > 2)
                        u1ByteCount = StrToInt((CHAR *) aszArgv[2]);

                    if (i4Argc > 3)
                        u1ByteCountRd = StrToInt((CHAR *) aszArgv[3]);

                    if (i4Argc > (cCMD_REG_NUM + 1))
                        i4Argc = cCMD_REG_NUM + 1;

                    for (ii = 4; ii < i4Argc; ii ++)
                    {
                        mcSET_QUEUE2(au1Data, ii - 4) = StrToInt((CHAR *) aszArgv[ii]);
                    }
                    mcSET_CMD2(au1Data) = u2RegAddr;        // Host Cmd Id
                    if (mcHOST_CMD(psDemodCtx, au1Data, u1ByteCountRd, u1ByteCount))
                    {
                        if (u1ByteCountRd)
                        {
                            mcSHOW_USER_MSG(("%03X: ", u2RegAddr));

                            if (u1ByteCountRd > (cMAX_READ_NUM - 2))
                                u1ByteCountRd = cMAX_READ_NUM -2;

                            for (ii = 0; ii < u1ByteCountRd; ii ++)
                            {
                                mcSHOW_USER_MSG((" %02X", mcGET_QUEUE(au1Data, ii)));
                            }
                            mcSHOW_USER_MSG(("\n"));
                        }
                    }
                }
                break;

            case 'a':
            #ifdef fcSMART_ANT_DRV
                u2RegAddr = 0;
                u1ByteCount = 0;
                ii = 0;
                if (i4Argc > 1)
                {
                    u2RegAddr = StrToInt(aszArgv[1]);
                    if (i4Argc > 2)
                        u1ByteCount = StrToInt(aszArgv[2]);
                    if (i4Argc > 3)
                        ii = StrToInt(aszArgv[3]);

                    vWriteSmartAnt(psDemodCtx, u2RegAddr, u1ByteCount, ii);
                }
                mcSHOW_USER_MSG(("AntIF Detect: %d\n", fgDetectSmartAnt(psDemodCtx)));
                if (u1GetRegs(psDemodCtx, DTD_REG_ANTIF_1, au1Data, 2))
                {
                    mcSHOW_USER_MSG(("AntIF Status %02X.%02X\n", au1Data[0], au1Data[1]));
                }
            #endif
                break;

           case 'e':
                {
                    UINT8   ucCmdExt, u1Opt;
                    UINT16  u2DelayMs1 = 10, u2DelayMs2 = 150;
                    ucCmdExt = ((CHAR *) aszArgv[0])[1];
                    if (ucCmdExt == 'q')
                    {
                        u1Opt = StrToInt((CHAR *) aszArgv[1]);
                        //u2DelayMs1 = StrToInt((CHAR *) aszArgv[2]);
                        //u2DelayMs2 = StrToInt((CHAR *) aszArgv[3]);

                        // Stop demod acquistion
                        DTD_Acquire(psDemodCtx, 0);
                        // Delay
                        mcDELAY_MS(u2DelayMs1);
                        //mcSHOW_USER_MSG(("Delay %d ms\n", u2DelayMs1));
                        // Set EQ bist register
                        DTD_SetEQBist_RegScript(psDemodCtx, u1Opt);
                        // Delay
                        mcDELAY_MS(u2DelayMs2);
                        //mcSHOW_USER_MSG(("Delay %d ms\n", u2DelayMs2));
                        // Check result
                        DTD_CheckEQBistResult(psDemodCtx, u1Opt);
                    }
                }
                break;

        #if fcTUNER_TYPE >= cTUNER_ALL_START && 0  // [
            case 'e':
                {
                UINT8   ucCmdExt;
                UINT8   u1CfgPos = 0;

                    ucCmdExt = ((CHAR *) aszArgv[0])[1];

                    if (ucCmdExt == 'e')
                    {
            #if fcINTERNAL_TEST || 1
                        mcREAD_CHK_INPUT(1);
                        mcREAD_SHOW_VALUE_FUNC(u1Func, psDemodCtx, mcREAD_EEPROM_FUNC);
            #endif
                    }
                    else if (ucCmdExt == 'f')
                    {
            #if fcINTERNAL_TEST || 1
                        if (i4Argc > 1)
                            u4TunerCtrlForce =  StrToInt((CHAR *) aszArgv[1]);
                        mcSHOW_USER_MSG(("u4TunerCtrlForce=0x%02X", u4TunerCtrlForce));
            #endif
                    }
                    else if (ucCmdExt == 'p')
                    {
            #if fcINTERNAL_TEST || 1
                        if (i4Argc > 1)
                            u1TunerDefTypeForce =  StrToInt((CHAR *) aszArgv[1]);
                        mcSHOW_USER_MSG(("u1TunerDefTypeForce=0x%02X", u1TunerDefTypeForce));
            #endif
                    }
                    else
                    {
                        if ((ucCmdExt >= '0') && (ucCmdExt <= '9'))
                        {
                            u1CfgPos = ucCmdExt - '0';
                        }
                        u1ByteCountRd = 4;
                        if (i4Argc > 1)
                        {
                        //    u1ByteCount = StrToInt(aszArgv[1]);
                        //    if (u1ByteCount > (i4Argc - 2))
                                u1ByteCount = (i4Argc - 1);

                            if (u1ByteCount)
                            {
                            UINT8   u1ChkSum = 0;

                                for (ii = 0; ii < u1ByteCount; ii ++)
                                {
                                    au1Data[ii] = StrToInt((CHAR *) aszArgv[ii + 1]);
                                    u1ChkSum ^= au1Data[ii];
                                }
                                mcSHOW_USER_MSG(("Auto generate checksum? (Y/N)"));
                            //    ii = mcGETCHAR();
                            //    mcSHOW_USER_MSG((" (%02X, %c)", ii, (ii | mcBIT(5))));
                                if ((mcGETCHAR() | mcBIT(5)) == 'y')
                                {
                                    au1Data[u1ByteCount] = u1ChkSum;
                                    u1ByteCount ++;
                                }
                                for (ii = 0; ii < u1ByteCount; ii ++)
                                {
                                    mcSHOW_USER_MSG((" %02X", au1Data[ii]));
                                }
                                mcSHOW_USER_MSG(("\nConfirm writing to EEPROM? (Y/N)"));
                                if ((mcGETCHAR() | mcBIT(5)) == 'y')
                                {
                                UINT8   u1CfgPosMax = u1CfgPos + 1;

                                    if (ucCmdExt == 'a')
                                    {
                                        u1CfgPosMax = EEPDTV_NIMCFG_COPY;
                                        u1CfgPos = 0;
                                    }
                                    for (ii = u1CfgPos; ii < u1CfgPosMax; ii ++)
                                        ucSetNIM_Config(psDemodCtx, au1Data, u1ByteCount, ii);
                                }
                                u1ByteCountRd = u1ByteCount;
                            }
                        }
                        else
                        {
                            mcSHOW_USER_MSG(("\t Cmd: %c 5A xx 00\n", aszArgv[0][0]));
                        #if fcINTERNAL_TEST
                            mcSHOW_USER_MSG(("\t xx = %2X for %s\n", DTD_TUNER_SAMSUNG_DTVS203, SAMSUNG_DTVS203_STR));
                            mcSHOW_USER_MSG(("\t xx = %2X for %s\n", DTD_TUNER_SAMSUNG_DTVS203EH, SAMSUNG_DTVS203EH_STR));
                            mcSHOW_USER_MSG(("\t xx = %2X for %s\n", DTD_TUNER_SAMSUNG_DTVS227CH, SAMSUNG_DTVS227CH_STR));
                        #endif
                            mcSHOW_USER_MSG(("\t xx = %2X for %s\n", DTD_TUNER_PHILIPS_FQD1236, PHILIPS_FQD1236_STR));
                            mcSHOW_USER_MSG(("\t xx = %2X for %s\n", DTD_TUNER_FORWARD_TAEH6, FORWARD_TAEH6_STR));
                            mcSHOW_USER_MSG(("\t xx = %2X for %s\n", DTD_TUNER_PHILIPS_FQD1236E, PHILIPS_FQD1236E_STR));
                            mcSHOW_USER_MSG(("\t xx = %2X for %s\n", DTD_TUNER_ALPS_TDQU2, ALPS_TDQU2_STR));
                            mcSHOW_USER_MSG(("\t xx = %2X for %s\n", DTD_TUNER_ALPS_TDQU4X524A, ALPS_TDQU4X524A_STR));
                            mcSHOW_USER_MSG(("\t xx = %2X for %s\n", DTD_TUNER_ALPS_TDQU8XT01A, ALPS_TDQU8_STR));
                    //    #if fcINTERNAL_TEST
                            if (mcTEST_BIT(psDemodCtx->u4DrvCustTunerCtrl, cCUST_REQ_3))
                            {
                            UINT8 u1TunerTypeMap;

                                mcSHOW_USER_MSG(("\n\t Tuner type mapping list:\n"));
                                for (ii = 0; ii < 10; ii ++)
                                {
                                    u1TunerTypeMap = u1GetCustTunerMap(ii);
                                    if (u1TunerTypeMap == (UINT8) DTD_TUNER_TYPE_NULL)
                                        break;
                                    mcSHOW_USER_MSG(("\t %d.%2X\n", ii, u1TunerTypeMap));
                                }
                            }
                    //    #endif
                        }
                        if (u1ByteCountRd)
                        {
                        UINT8   u1CfgPosMax = u1CfgPos + 1;

                            if (ucCmdExt == 'a')
                            {
                                u1CfgPosMax = EEPDTV_NIMCFG_COPY;
                                u1CfgPos = 0;
                            }
                            for (; u1CfgPos < u1CfgPosMax; u1CfgPos ++)
                            {
                                u1ByteCount = u1GetNIM_Config(psDemodCtx, au1Data, u1ByteCountRd, u1CfgPos);
                                for (ii = 0; ii < u1ByteCount; ii ++)
                                {
                                    mcSHOW_USER_MSG((" %02X", au1Data[ii]));
                                }
                                mcSHOW_USER_MSG(("\n"));
                            }
                        }
                    }
                }
                break;

             case 'i':
                vEnableI2C_Mode();
                break;

            case '2':
                {
                UINT8   ucCmdExt;

                    ucCmdExt = ((CHAR *) aszArgv[0])[1];

                    if (ucCmdExt == 'r')
                    {
                        if (i4Argc > 1)
                        {
                            mcREAD_CHK_INPUT(1);
                            mcREAD_SHOW_VALUE_ONCE(DTD_GetReg, psDemodCtx, au1Data);
                        }
                    }
                    else if (ucCmdExt == 'w')
                    {
                        mcWRITE_SET_VALUE_NORM(1, DTD_SetReg, psDemodCtx);
                    }
                }
                break;

    #endif  // ]

             case 'n':
                psDemodCtx->u1TransPhase = mcSTR_2_INT(aszArgv[1]);
                mcSHOW_USER_MSG(("u1TransPhase=0x%02X\n", psDemodCtx->u1TransPhase));
                break;

             case 'p':
                mcSHOW_USER_MSG(("[Phase]0x%02X: SNR %ddB\n", psDemodCtx->u1TransPhase, DTD_GetSignalSNR(psDemodCtx)));
                break;

             case 'g':
                {
                UINT8   ucCmdExt;
          UINT8   ucCmdRdFw;
          UINT8   fgGSOperation;

              ucCmdExt = ((CHAR *) aszArgv[0])[1];
              if( ucCmdExt == 's' )
                {
                        if (i4Argc > 1)
                        {
                ucCmdRdFw = *((CHAR *) aszArgv[1]);
                            if (ucCmdRdFw == 'c')
                            {
                 DTD_Get_GSCfg(psDemodCtx);
                            }
                            else if (ucCmdRdFw == 'o')
                            {
                                fgGSOperation = StrToInt((CHAR *) aszArgv[2]);
                    DTD_ControlGS(psDemodCtx, fgGSOperation);
                            }
                   else if (ucCmdRdFw == 'h')
                            {
                                DTD_HwEqGSIndex(psDemodCtx);
                            }
                else if (ucCmdRdFw == 'f')
                {
                DTD_FwEqGSIndex(psDemodCtx);
                }
                   else if (ucCmdRdFw == 'b')
                   {
                       DTD_Get_BrazilCfg(psDemodCtx);
                   }
                else
                {
                    mcSHOW_USER_MSG(("Wrong gs sub cmd: \n"));
                            }
               }
               else
               {
                   mcSHOW_USER_MSG(("Sub Cmd: \n"));
                   mcSHOW_USER_MSG(("\t c Get the information of GS target channel configuration\n"));
                mcSHOW_USER_MSG(("\t b Get the information of Brazil target channel detection\n"));
                mcSHOW_USER_MSG(("\t o fgEnable (1:enable,0:disable)Control of EQ GS operation\n"));
                mcSHOW_USER_MSG(("\t h Check HW-selected EQ GS index\n"));
                mcSHOW_USER_MSG(("\t f Check FW-selected EQ GS index\n"));
               }
                }
                }

              break;

            default:
            //    mcSHOW_USER_MSG(("Invalid parameter!\n"));
                break;
        }
    }
}


/***********************************************************************/
#define cTUNER_I2C_NUM    8
#define cVSB_HIGH_RES_SNR_SCALE     512
#define cQAM_HIGH_RES_SNR_SCALE     256

VOID pi_external_command(TD_DTD_CTX_T* psTDDTDCtx, INT32 i4Argc, const CHAR** aszArgv)
{
DEMOD_CTX_T *psDemodCtx = psTDDTDCtx->psDemodInbInfo;
UINT8   au1Data[cTUNER_I2C_NUM + 1];
UINT8   ii, u1ByteCount = 8;
UINT8   ucCmdId = 0;
UINT8   u1Status = 0;
UINT8   fgParallel = 0;
                UINT8   uTemp;
                BOOL    fgIsCFO;
                INT32   ai4Data[cAGC_INFO_SIZE];
                INT32    u4SymbolRate;
                UINT16  u2SNR;
                CHAR   ucCmdExt;                     

    if (i4Argc < 1)
    {
        mcSHOW_USER_MSG(("Cmd Usage: nim.dtd [OPTION] ...\n"));
        mcSHOW_USER_MSG(("\t f [Freq (kHz)]: Tuner_SetFreq\n"));
        mcSHOW_USER_MSG(("\t p : Read Tuner Status(PLL)\n"));
        mcSHOW_USER_MSG(("\t t (0x)Value0 Value1...: Write Tuner register\n"));
        mcSHOW_USER_MSG(("\t c [RF_En( 1(Manual)/0(Auto) ) IF_En(1/0) RF_Gain(mV) IF_Gain(mV)]: Manual AGC\n"));
        mcSHOW_USER_MSG(("\t q 1(Start) / 0 (Stop): Demod Acq Start/Stop \n"));
        mcSHOW_USER_MSG(("\t y ObserveTime(sec) RefreshTime(sec): Start PEC \n"));
        mcSHOW_USER_MSG(("\t z: Stop PEC \n"));
        mcSHOW_USER_MSG(("\t d: Dump registers \n"));
        mcSHOW_USER_MSG(("\t s: Demod status\n"));                
        mcSHOW_USER_MSG(("\t st: [1(Parallel) / 0(Serial)]: Set TS format\n"));
        mcSHOW_USER_MSG(("\t i: [IF_Freq (kHz)]: Set Demo IF\n"));

        //#if  fcTUNER_TYPE==cALPS_TDAU4D01A
        if (psDemodCtx->u1TunerType == cALPS_TDAU4D01A)
        {
            mcSHOW_USER_MSG(("\t j: Digital tuner adjacent trap control. 1--ON,0--OFF\n"));
        }
        //#endif
//#ifdef DIGUS_ENABLE_MONITOR       
        if(pDTDunerDeviceCtx->u1MWIntf != cMTAL_IF && pDTDunerDeviceCtx->u1MWIntf != cSDAL_IF)
        {
            mcSHOW_USER_MSG(("\t mp: Pause monitor thread\n"));
            mcSHOW_USER_MSG(("\t mr: Resume monitor thread\n"));
        }
//#endif

    }

    if (i4Argc > 0)
    {
        ucCmdId = *((CHAR *) aszArgv[0]);

        switch (ucCmdId)
        {
            //#if  fcTUNER_TYPE==cALPS_TDAU4D01A
            case 'j':
                if (psDemodCtx->u1TunerType == cALPS_TDAU4D01A)
                {
                    if (i4Argc > 1)
                    {
                        ii = StrToInt(aszArgv[1]);
                        if(1==ii)
                        {
                            gfgIFTrap=0x03;//IF Trap ON
                        }
                        else
                        {
                            gfgIFTrap=0x02;//IF Trap OFF
                        }
                    }
                }
                else
                {
                    mcSHOW_USER_MSG(("Not support!\n"));
                }
             break;
            //#endif
            case 't': /* Write Tuner I2C sequence */
                if (i4Argc > 1)
                {
                    u1ByteCount = (i4Argc - 1);
                    if (u1ByteCount > cTUNER_I2C_NUM)
                        u1ByteCount = cTUNER_I2C_NUM;
                    for (ii = 1; ii < u1ByteCount + 1; ii ++)
                    {
                        au1Data[ii - 1] = StrToInt((CHAR *) aszArgv[ii]);
                    }
                    mcSHOW_USER_MSG(("%02X: ", (psDemodCtx->sTunerCtx)->I2cAddress));
                    for (ii = 0; ii < u1ByteCount; ii ++)
                    {
                        mcSHOW_USER_MSG((" %02X", au1Data[ii]));
                    }
                    mcSHOW_USER_MSG(("\n"));
                    DTD_TunerI2c(psDemodCtx, TRUE);
                   // u1I2cWriteTuner(psDemodCtx->sTunerCtx.psI2cCtx, (psDemodCtx->sTunerCtx).I2cAddress,au1Data, u1ByteCount);
                   ICtrlBus_I2cTunerWrite(270,(psDemodCtx->sTunerCtx)->I2cAddress,au1Data,u1ByteCount);
                    DTD_TunerI2c(psDemodCtx, FALSE);
                }
                else
                {
                    mcSHOW_USER_MSG(("\t t (0x)Value0 Value1...: Write Tuner register\n"));
                }
                break;

            case 'f':  /* Tuner Set_Freq */
                if (i4Argc > 1)
                {
                UINT32   u4Freq;
                INT8    i1Err;
                PARAM_SETFREQ_T param;
                    u4Freq = StrToInt(aszArgv[1]);
                 param.Freq=u4Freq;
                 param.Modulation=psDemodCtx->Modulation;
                 param.fgAutoSearch=0;
                    i1Err = DTD_SetFreq(psDemodCtx, &param);
                        mcSHOW_USER_MSG(("Set Tuner Freq (%"cFMT_UL")\n", u4Freq));
                    if (i1Err == cRET_SETFREQ_I2C_ERROR)
                    {
                        mcSHOW_USER_MSG(("I2C bus error\n"));
                    }
                    else if (i1Err == cRET_SETFREQ_RANGE)
                    {
                        mcSHOW_USER_MSG(("Out of freq range\n"));
                    }
                    else if (i1Err == cRET_SETFREQ_PLL_LOCK)
                    {
                        mcSHOW_USER_MSG(("PLL unlock in 100 ms\n"));
                    }
                }
                else
                {
                    mcSHOW_USER_MSG(("\t f [Freq (kHz)]: Tuner_SetFreq\n"));
                }
                break;

            case 'p':  /* Read tuner status */
                u1Status = DTD_GetTunerStatus(psDemodCtx);
                mcSHOW_USER_MSG(("PLL lock state = %d\n", u1Status));
                break;

            case 'y': /* Start PEC */
                if (i4Argc > 1)
                {
                    psDemodCtx->u4PecObserveTime = StrToInt(aszArgv[1]); // Input1: Observe time(sec)
                    if (psDemodCtx->u4PecObserveTime > 600)
                    {
                        psDemodCtx->u4PecObserveTime = 600;
                        mcSHOW_USER_MSG(("Max observation time is 600 sec!"));
                    }

                    psDemodCtx->u4PecObserveTime *= 1000;
                    psDemodCtx->u2PecRefreshTime = StrToInt(aszArgv[2])*1000; // Input2: Refresh time(sec)
                    psDemodCtx->u1PecFlag = 1;
                    vDTDGetStatus(psDemodCtx);
                    psDemodCtx->u2AccReAcqCnt = 0; // clear reacq count for qtest log
                }
                else
                {
                    mcSHOW_USER_MSG(("\t y ObserveTime(sec) RefreshTime(sec): Start PEC \n"));
                }
                break;

            case 'z': /* Stop PEC */
                psDemodCtx->u1PecFlag = 3;
                break;

            case 'd': /* Dump initial registers (UPIF) */
                DTD_Get_UPIF_Reg(psDemodCtx);
                break;

            case 's':
                ucCmdExt = ((CHAR *) aszArgv[0])[1];
                if(ucCmdExt == 't')    // gs: get status
                {
                    if (i4Argc > 1)
                    {
                         fgParallel = StrToInt((CHAR *) aszArgv[1]);
                         DTD_SetTsMode(psDemodCtx, fgParallel);
                    }                                
                }
                else
                {
                
                       mcSHOW_USER_MSG(("=================================\n"));    
                       switch(psDemodCtx->Modulation)
                       {
                           case MOD_ATSC_8VSB: mcSHOW_USER_MSG(("Modulation Type = VSB\n")); break;
                           case MOD_J83B_64QAM: mcSHOW_USER_MSG(("Modulation Type = J83B_64\n")); break;
                           case MOD_J83B_256QAM: mcSHOW_USER_MSG(("Modulation Type = J83B_256\n")); break;                       	  
                           case MOD_J83B_AUTO_QAM: mcSHOW_USER_MSG(("Modulation Type = J83B_Auto\n")); break;                       	  
                           default: mcSHOW_USER_MSG(("Modulation Type = unknow\n")); break;                       	  
                       }
                       uTemp = ucGetReg(psDemodCtx, DTD_REG_ISYS_SYS_MD);
                       switch(uTemp&0x03 )
                       {
                           case 0: mcSHOW_USER_MSG(("QAM mode = VSB\n")); u4SymbolRate = 10762; break;
                           case 1: mcSHOW_USER_MSG(("QAM mode = Reserved\n"));u4SymbolRate = 0; break;
                           case 2: mcSHOW_USER_MSG(("QAM mode = Q64\n"));u4SymbolRate = 5057; break;
                           case 3: mcSHOW_USER_MSG(("QAM mode = Q256\n"));u4SymbolRate =5361 ; break;                          
                           default: mcSHOW_USER_MSG(("QAM mode = Unknow\n")); u4SymbolRate = 0; break;
                       }
                       
                       DTD_GetTFO(psDemodCtx, ai4Data);
                       
                       mcSHOW_USER_MSG(("SymbolRate =  %04d\n", (u4SymbolRate+ai4Data[0]/1000)));                       
                       
                       //DTD_GetTFO(psDemodCtx, ai4Data);
                       mcSHOW_USER_MSG(("TFO =  %04dppm\n", ai4Data[0]*100000/u4SymbolRate*10));                       
                       
                       DTD_Get_AGCGain_CFO(psDemodCtx, ai4Data, &fgIsCFO);
                       mcSHOW_USER_MSG(("CFO =  %04dkhz\n", ai4Data[cVCR_CFO]/1000));                       
                       

                       if (psDemodCtx->Modulation == MOD_ATSC_8VSB)
                       	u2SNR = 10 * psDemodCtx->u2SNRRaw_HighRes / cVSB_HIGH_RES_SNR_SCALE;
                       else
                       	u2SNR = 10 * psDemodCtx->u2SNRRaw / cQAM_HIGH_RES_SNR_SCALE;                       

                       mcSHOW_USER_MSG(("SNR = %02d/10dB\n", u2SNR));
                       mcSHOW_USER_MSG(("=================================\n"));  
                       mcSHOW_USER_MSG((" AccPER: <%"cFMT_UL"e-5> (<%"cFMT_UL"> <%"cFMT_UL">), <%"cFMT_UL"> \n", psDemodCtx->u4AccErrorRate, psDemodCtx->u4AccPktErr, psDemodCtx->u4AccPktCnt, psDemodCtx->u2AccReAcqCnt));                       
                       mcSHOW_USER_MSG(("=================================\n"));  
                       #if fcDEMOD_GEN >= cMT5396_DTD  
                           if(psDemodCtx->Modulation == MOD_ATSC_8VSB )
                           {
                               mcSHOW_DBG_MSG(("vFN state(0x95) = 0x%02X\n",ucGetFwVar(psDemodCtx,eVAR_VCR_FN_STATE)));
                               mcSHOW_DBG_MSG(("vPN state(0xD0) = 0x%02X\n",ucGetFwVar(psDemodCtx,eVAR_VCR_PN_STATE)));                       
                               mcSHOW_DBG_MSG(("vGQ state(0x10B) = 0x%02X\n",ucGetFwVar(psDemodCtx,eVAR_10B_GQ)));                          
                               mcSHOW_DBG_MSG(("vPilotLevel = 0x%02X\n",ucGetReg(psDemodCtx,DTD_REG_VCR_PCFGIDX)));                          
                               mcSHOW_DBG_MSG(("vMainPathStr = 0x%02X\n",(ucGetReg(psDemodCtx,DTD_REG_VTR_MP_POW_0)&0x0F)));
                               mcSHOW_DBG_MSG(("vPS_IIR_BW = 0x%02X\n",ucGetReg(psDemodCtx,DTD_REG_VCR_PCFG_DAT)));                           
                               mcSHOW_DBG_MSG(("=================================\n"));                                                      
                           }
                           else if(psDemodCtx->Modulation <= MOD_J83B_AUTO_QAM)
                           {
                               switch(ucGetFwVar(psDemodCtx,eVAR_PRESYND))
                               {
                                   case 0: mcSHOW_DBG_MSG(("QCR_PreSyn(0x3F) = Normal\n")); break;
                                   case 1: mcSHOW_DBG_MSG(("QCR_PreSyn(0x3F) = PN1\n")); break;
                                   case 2: mcSHOW_DBG_MSG(("QCR_PreSyn(0x3F) = SMALL\n")); break;
                                   case 3: mcSHOW_DBG_MSG(("QCR_PreSyn(0x3F) = PN0\n")); break;
                                   default: break;
                               }
                               mcSHOW_DBG_MSG(("BN_Mon = 0x%02X(6b: BN_FLAG, 5b: BN_APPEAR)\n",ucGetReg(psDemodCtx,DTD_REF_QEQ_BN_MON))); 
                               mcSHOW_DBG_MSG(("=================================\n"));                                                      
                          
                           }
                           else
                           {
                           }

                           mcSHOW_USER_MSG(("SLD RFAGC CW = 0.%d\n", psDemodCtx->s2SldPraRfAgc/41));
                           mcSHOW_USER_MSG(("SLD IFAGC CW = 0.%d\n", psDemodCtx->s2SldPraIfAgc/41));
                           mcSHOW_USER_MSG(("SLD PRA Gain Idx = 0x%02X\n", psDemodCtx->ucSldPraGainIdx));
                           mcSHOW_USER_MSG(("SLD Target LV(ADout, Mag) = %02d\n", psDemodCtx->s2SldPraOut));                                              		
                           mcSHOW_USER_MSG(("=================================\n"));                         
                         
                       #endif
                       
                
                
               }

                

                break;

            case 'c': /* Manual AGC */
                if (i4Argc > 1)
                {
                BOOL fgRFEn, fgIFEn, fgValid;
                UINT16 u2GainVol[2];

                    fgValid = TRUE;
                    fgRFEn = (BOOL) StrToInt(aszArgv[1]);   // RF Gain Manual En (Manual=1, Auto=0)
                    fgIFEn = (BOOL) StrToInt(aszArgv[2]);   // IF Gain Manual En
                    u2GainVol[0] = (UINT16) StrToInt(aszArgv[3]); // RF Gain Voltage
                    u2GainVol[1] = (UINT16) StrToInt(aszArgv[4]); // IF Gain Voltage
                    if ((fgRFEn > 1) || (fgIFEn > 1))
                    {
                        fgValid = FALSE;
                        mcSHOW_USER_MSG(("fgRFEn/fgIFEn is invalid!\n"));
                    }
                    if (u2GainVol[0] > 3300)
                    {
                        fgValid = FALSE;
                      mcSHOW_USER_MSG(("RF Voltage range should be 0~3300mV!\n"));
                    }
                    if (u2GainVol[1] > 3300)
                    {
                        fgValid = FALSE;
                      mcSHOW_USER_MSG(("IF Voltage range should be 0~3300mV!\n"));
                    }

                    if (fgValid)
                        DTD_ManualAGC(psDemodCtx, fgRFEn, fgIFEn, &u2GainVol[0]);
                }
                else
                {
                    mcSHOW_USER_MSG(("[RF_En(1/0) IF_En(1/0) RF_Gain(mV) IF_Gain(mV)]: Manual AGC\n"));
                }
                break;

            case 'q': /* Demod acquistion start */
                if (i4Argc > 1)
                {
                BOOL fgStart;

                    fgStart = (BOOL) StrToInt(aszArgv[1]);
                    mcSHOW_USER_MSG(("fgStart = %d\n", fgStart));
                    DTD_Acquire(psDemodCtx, fgStart);
                }
                break;
//#ifdef DIGUS_ENABLE_MONITOR           
            case 'm':            
                if(pDTDunerDeviceCtx->u1MWIntf != cMTAL_IF && pDTDunerDeviceCtx->u1MWIntf != cSDAL_IF)
                                {   
                    UINT8   ucCmdExt;                           
                    ucCmdExt = ((CHAR *) aszArgv[0])[1];

                    if (ucCmdExt == 'p'){
                        psTDDTDCtx->fgBreakMonitor = TRUE;
                    }
                    else if (ucCmdExt == 'r'){
                        psTDDTDCtx->fgBreakMonitor = FALSE;
                        mcMUTEX_UNLOCK(psTDDTDCtx->t_escape_mon_suspend);
                    }
                    else{
                        mcSHOW_USER_MSG(("do nothing\n"));
                    }
                }               
                break;
//#endif
            case 'i':
            	{
            	    UINT16 u2IF_Freq = 0;
            	    u2IF_Freq = StrToInt(aszArgv[1]);
            	    psDemodCtx->sTunerCtx->u2IF_Freq = u2IF_Freq;
            	    DTD_SetIF(psDemodCtx, u2IF_Freq);
            	}
            	break;
            default:
                mcSHOW_USER_MSG(("Invalid parameter!\n"));
                break;
        }
    }
}

/***********************************************************************/
UCHAR pd_transConnType(DRV_CONN_TYPE_T e_conn_type)
{
    UCHAR   conn_type;

    switch (e_conn_type){
        case TUNER_CONN_TYPE_TER_DIG:
            conn_type = CONN_TYPE_TER;
            break;
        case TUNER_CONN_TYPE_CAB_DIG:
            conn_type = CONN_TYPE_CAB;
            break;
        case TUNER_CONN_TYPE_CAB_DIG_OOB_RX:
            conn_type = CONN_TYPE_OOB_RX;
            break;
        default:
            conn_type = CONN_TYPE_TER;
            break;
    }
    return (conn_type);
}

STATIC INT16 DTD_PdGetSync(DEMOD_CTX_T *psDemodCtx,
                        DRV_CONN_TYPE_T e_conn_type)
{
INT16   i2RetValue;

    i2RetValue = DTD_GetSync(psDemodCtx);

    return (i2RetValue);
}


#if 0
//-------------------------------------------------------------------------
/*
 *  _bCheckValidTunerType
 *  check whether tuner Type is valid
 *  @param  u1TestTunerType   TunerType for test
 *  @retval TRUE    valid tuner type
 *  @retval FALSE    invalid tuner type
 */
//-------------------------------------------------------------------------
#if (fcTUNER_TYPE == cDTD_TUNER_ALL)
STATIC BOOL _bCheckValidTunerType(UINT8 u1TestTunerType)
{
BOOL fgRet = FALSE;

    if( (u1TestTunerType > cDTD_TUNER_TYPE_START) && (u1TestTunerType <= cDTD_VALID_TYPE_END))
    {
        fgRet = TRUE;
    }
    return fgRet;
}


//-------------------------------------------------------------------------
/*
 *  u1CheckTunerTypeFromEeprom
 *  1. Tuner id exist in three additional EEPROM address for check
 *  2. Judgement:
 *  Tuner ID invalid, ex. 0xff -> Use the default Tuner,
 *  Three address are with the same valid Tuner ID -> Trust the tuner ID
 *  Two address are with the same valid Tuner ID -> Trust the tuner ID
 *  All address are with different Tuner ID -> Use the default Tuner
 *  The invalid value need to write back per SGP spec. Add in 2008.8.25
 *  @param  void
 *  @retval tunerid    return tuner id depend on the matched judgement rule
 */
//-------------------------------------------------------------------------
STATIC UINT8 _u1CheckTunerTypeFromEeprom(VOID)
{
    UINT8  u1Val[3];
    UINT8  u1DeftType,u1ResultType;
    UINT32 u4Ofst[3];
    UINT32 u4Default;
//    UINT8  ii = 0;

    /* Read three offset value and default tuner type from drv_cust */
    u4Ofst[0] = DRVCUST_OptGet(eTunerTypeOfst0);
    u4Ofst[1] = DRVCUST_OptGet(eTunerTypeOfst1);
    u4Ofst[2] = DRVCUST_OptGet(eTunerTypeOfst2);
    u4Default = DRVCUST_OptGet(eTunerTypeDefault);
    mcSHOW_HW_MSG(("Ofst0=0x%x, ofst1=0x%x, ofst2=0x%x, def=0x%x\r\n",u4Ofst[0], u4Ofst[1], u4Ofst[2], u4Default));

    /* Read three tuner IDs from EEPROM offset */
    VERIFY(0 == EEPROM_Read((UINT64)u4Ofst[0], (UINT32)(void *)&u1Val[0], 1));
    VERIFY(0 == EEPROM_Read((UINT64)u4Ofst[1], (UINT32)(void *)&u1Val[1], 1));
    VERIFY(0 == EEPROM_Read((UINT64)u4Ofst[2], (UINT32)(void *)&u1Val[2], 1));
    mcSHOW_HW_MSG(("Read TunerType from EEPROM, addr0x%x=0x%x, addr0x%x=0x%x, addr0x%x=0x%x\r\n",u4Ofst[0],u1Val[0],u4Ofst[1],u1Val[1],u4Ofst[2],u1Val[2]));

    /* Default tuner: SGP TDQU8-T01A */
    u1DeftType = cALPS_TDQU8;
    /* Or you can change it from drv_cust*/
    if (_bCheckValidTunerType(u4Default) == TRUE)
        u1DeftType = u4Default;

    /* If two of tuner types in eeprom are the same and are valid tuner type, then trust this value, other condition return default type */
    if ((_bCheckValidTunerType(u1Val[0]) == TRUE) && (u1Val[0]==u1Val[1])){
        u1ResultType = u1Val[0];
    }
    else if((_bCheckValidTunerType(u1Val[0]) == TRUE) && (u1Val[0]==u1Val[2])){
        u1ResultType = u1Val[0];
    }
    else if((_bCheckValidTunerType(u1Val[1]) == TRUE) && (u1Val[1]==u1Val[2])){
        u1ResultType = u1Val[1];
    }
    else{
        //Return default tuner type
        u1ResultType = u1DeftType;
    }
    mcSHOW_HW_MSG(("u1ResultType=0x%x\r\n", u1ResultType));

#if 0
    // Write result tuner ID into EEPROM. Add in mt539x
    for (ii = 0; ii < 3; ii++)
    {
        if ( u1ResultType != u1Val[ii])
        {
            VERIFY(0 == EEPROM_Write((UINT64)u4Ofst[ii], (UINT32) &u1ResultType, 1));
            mcSHOW_HW_MSG(("Write TunerType(0x%x) from EEPROM offset 0x%x\r\n", u1Val[ii], u4Ofst[ii]));
        }
    }
#endif

     return u1ResultType;
}

//-----------------------------------------------------------------------------
/*
 *  GetTunerType: Get tuner type
 *  @param  void.
 *  @retval TunerType ID.
 */
//-----------------------------------------------------------------------------
UINT8 GetTunerType(VOID)
{
    mcSHOW_DBG_MSG(("TunerType(0x%x) from digital PD\r\n", _u1TunerType));
    return (_u1TunerType);
}
#endif

#endif



/***********************************************************************/
/*              Public Functions                                       */
/***********************************************************************/
#if fcADD_CUST_IF
EXTERN UINT32 au4CfgPos[];

UINT32 DTD_GetCtmrReq(DEMOD_CTX_T *psDemodCtx)
{
UINT32  TunerCtrl;

    TunerCtrl = DRVCUST_OptGet(eTunerCtrl);

    if (u4TunerCtrlForce)
        TunerCtrl = u4TunerCtrlForce;
    mcPRN_DBG_MSG("DRVCUST_OptGet(eTunerCtrl) = 0x%02X\n", TunerCtrl);

    #if (fcTUNER_TYPE >= cTUNER_ALL_START) && 0
    au4CfgPos[0] = EEPDTV_NIMCFG_TAG;
    if (mcTEST_BIT(TunerCtrl, cCUST_REQ_2))
        au4CfgPos[0] ++;
    if (mcTEST_BIT(TunerCtrl, cCUST_REQ_3))
        au4CfgPos[0] = DRVCUST_OptGet(eTunerTypeOfst0);
//    if (mcTEST_BIT(TunerCtrl, cCUST_REQ_2))
    {
        psDemodCtx->u1DrvCustTunerDefType = DRVCUST_OptGet(eTunerTypeDefault);
        if (u1TunerDefTypeForce)
            psDemodCtx->u1DrvCustTunerDefType = u1TunerDefTypeForce;
        mcPRN_DBG_MSG("DRVCUST_OptGet(eTunerTypeDefault) = 0x%02X\n", psDemodCtx->u1DrvCustTunerDefType);
        au4CfgPos[1] = DRVCUST_OptGet(eTunerTypeOfst1);
        au4CfgPos[2] = DRVCUST_OptGet(eTunerTypeOfst2);
    }
    #endif
    return TunerCtrl;
}
#endif

/***********************************************************************/
/*              Interface APIs                                         */
/***********************************************************************/
INT16 DTD_TunerGetSync(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                        DRV_CONN_TYPE_T e_conn_type)
{
TD_DTD_CTX_T *psTDDTDCtx = (TD_DTD_CTX_T *) ptTDSpecificCtx;
DEMOD_CTX_T *psDemodCtx = psTDDTDCtx->psDemodInbInfo;
INT16   i2RetValue;

    mcSEMA_LOCK_HAL(psTDDTDCtx->hHalLock);   // Avoid re-entry

    i2RetValue = DTD_PdGetSync(psDemodCtx, e_conn_type);

    mcSEMA_UNLOCK_HAL(psTDDTDCtx->hHalLock);
    return (i2RetValue);
}

#if fcADD_ACQ_CHK
BOOL DTD_TunerAcq(PTD_SPECIFIC_CTX ptTDSpecificCtx,
#else
VOID DTD_TunerAcq(PTD_SPECIFIC_CTX ptTDSpecificCtx,
#endif
                    DRV_CONN_TYPE_T e_conn_type,
                    VOID* pv_conn_info,
                    SIZE_T z_conn_info_len,
                    x_break_fct _BreakFct,
                    VOID* pvArg)
{
TD_DTD_CTX_T *psTDDTDCtx = (TD_DTD_CTX_T *) ptTDSpecificCtx;
DEMOD_CTX_T *psDemodCtx = psTDDTDCtx->psDemodInbInfo;
UINT16  ii;
UINT32  _Freq1;
INT32   _QAMSize;

TUNER_MODULATION_T  _Mode;
//UCHAR   _ConnType;
UCHAR   eModType;
#if fcADD_ACQ_CHK
BOOL    fgRetSts = FALSE;
#endif
//INT32 i4State;
//#ifdef DIGUS_ENABLE_MONITOR
TIME_TICK_T u4MonitorStartTick;
//#endif
#ifdef CC_TUNER_CONN_DEFAULT_SURF
TIME_DIFF_T u4TickDiff;
TIME_TICK_T u4GetSyncNowTick;
#endif
UCHAR eConnType;
TUNER_CONNECTION_MODE_T e_conn_mode = CONN_MODE_SCAN;

    psTDDTDCtx->fgBreakMonitor = TRUE;//pause monitor
   
    mcSEMA_LOCK_HAL(psTDDTDCtx->hHalLock);   // Avoid re-entry

#if fcMEASURE_DRIVER_TIME == 2
    mcGET_SYS_TIME(u4TickStartMeasure);
    u4PrevStartTime = u4TickStartMeasure;
    mcSHOW_DRVTIME_MSG(("DTD_TunerAcq Start\n"));
#endif

   // Get connection type: Channel scan or channel change (surf) mode form e_conn_mode
    if (e_conn_type == TUNER_CONN_TYPE_TER_DIG)
    {
        TUNER_TER_DIG_TUNE_INFO_T *pt_tuner_info =(TUNER_TER_DIG_TUNE_INFO_T *) pv_conn_info;
        
        if (pt_tuner_info->ui4_freq > DRVAPI_TUNER_TER_FREQ_HIGH ||
            pt_tuner_info->ui4_freq < DRVAPI_TUNER_TER_FREQ_LOW ||
            ((pt_tuner_info->e_mod != MOD_VSB_8)
                && (pt_tuner_info->e_mod != MOD_QAM_256)
                && (pt_tuner_info->e_mod != MOD_QAM_64)
                && (pt_tuner_info->e_mod != MOD_UNKNOWN)
            )) /*MOD_UNKNOWN is autoQAM*/
        {
            mcSHOW_DRVERR_MSG(("Invalid input to DTD_TunerAcq!!\n"));
            mcSEMA_UNLOCK_HAL(psTDDTDCtx->hHalLock);
            return fgRetSts;
        }
        
       e_conn_mode = pt_tuner_info->e_conn_mode;
       _Freq1 = pt_tuner_info->ui4_freq;
       _Mode = pt_tuner_info->e_mod;
    }
    else if (e_conn_type == TUNER_CONN_TYPE_CAB_DIG)
    {
        TUNER_CAB_DIG_TUNE_INFO_T *pt_tuner_info = (TUNER_CAB_DIG_TUNE_INFO_T *) pv_conn_info;
         
         if (pt_tuner_info->ui4_freq > DRVAPI_TUNER_CABLE_FREQ_HIGH ||
            pt_tuner_info->ui4_freq < DRVAPI_TUNER_CABLE_FREQ_LOW ||
            // Mark this to enable VSB mode in CAB_DIG
            //pt_tuner_info->ui4_sym_rate > DRVAPI_TUNER_CABLE_BAUD_HIGH ||
            //pt_tuner_info->ui4_sym_rate < DRVAPI_TUNER_CABLE_BAUD_LOW ||
            ((pt_tuner_info->e_mod != MOD_QAM_256) &&
             (pt_tuner_info->e_mod != MOD_QAM_128) &&
             (pt_tuner_info->e_mod != MOD_QAM_64) &&
             (pt_tuner_info->e_mod != MOD_QAM_32) &&
             (pt_tuner_info->e_mod != MOD_QAM_16) &&
             (pt_tuner_info->e_mod != MOD_QAM_4_NR) &&
             (pt_tuner_info->e_mod != MOD_UNKNOWN) &&
             (pt_tuner_info->e_mod != MOD_VSB_8))) /*MOD_UNKNOWN is autoQAM*/   
            {
            mcSHOW_DRVERR_MSG(("Invalid input to DTD_TunerAcq!!\n"));
            mcSEMA_UNLOCK_HAL(psTDDTDCtx->hHalLock);
            return fgRetSts;            
            }
                e_conn_mode = pt_tuner_info->e_conn_mode;
            //  u4SymbolRate=pt_tuner_info->ui4_sym_rate;
                _Freq1 = pt_tuner_info->ui4_freq;
                _Mode = pt_tuner_info->e_mod;
    }
    else
    { 
          mcSHOW_DRVERR_MSG(("unknown conn type %d\n",e_conn_type));
          mcSEMA_UNLOCK_HAL(psTDDTDCtx->hHalLock);
            return fgRetSts;    
    }

#if fcTUNE_NOT_START
    if (psDemodCtx->u1TunerAcqType != ACQ_DEMOD_ONLY)
#endif
        mcSHOW_DBG_MSG((" Freq = %d ", (int) _Freq1));
//    mcSHOW_USER_MSG((" Freq = %d ", (int) _Freq1));

    _Freq1 = _Freq1 / 1000;
     _QAMSize = Util_TunerModSize(_Mode);

    if (_QAMSize == 8)
    {
        eModType = MOD_ATSC_8VSB;
    }
    else if (_QAMSize == 64)
    {
        eModType = MOD_J83B_64QAM;
    }
#if fcADD_AUTO_QAM
    else if (_QAMSize == 999)
    {
        eModType = MOD_J83B_AUTO_QAM;
    }
#endif
    else
    {
        eModType = MOD_J83B_256QAM;
    }

    // Transform e_conn_mode to eConnType
    // e_conn_mode = CONN_MODE_SURF;
    if (e_conn_mode == CONN_MODE_SCAN)
    {
        eConnType = DEMOD_CONN_TYPE_SCAN;
    }
    else if (e_conn_mode == CONN_MODE_SURF)
    {
        eConnType = DEMOD_CONN_TYPE_SURF;
    }
    else
    {
        eConnType = DEMOD_CONN_TYPE_UNKNOWN;
    }
    mcSHOW_DBG_MSG(("e_conn_mode = %d, eConnType = %d ", e_conn_mode, eConnType));

    psTDDTDCtx->QAMSize = _QAMSize;
    psTDDTDCtx->isBreak = _BreakFct;
   // DTD_SetIF(psDemodCtx,psDemodCtx->sTunerCtx->u2IF_Freq);    // move to DTD_Connect, iven 20110706
    
#if (fcDEMOD_GEN < cMT5396_DTD)    // iven, only use for IC befor 5396, IC after 5396, Spectrum polarity is decided by tuner contex. by Tuner. 20110706    
    if(psDemodCtx->sTunerCtx->u2IF_Freq<25000)//Low IF boundary
    {
        DTD_SetSpcPolarity(psDemodCtx,TRUE,TRUE);//Spec inverse
    }
#endif

// Call DTD_Connect
    if (DTD_Connect(psDemodCtx, (UINT32)_Freq1, eModType, eConnType))
    {
//#ifndef CC_SDAL
        if(pDTDunerDeviceCtx->u1MWIntf != cSDAL_IF)
        {
             mcGET_SYS_TIME(u4ConStartTick);
            for (ii = 0; ii < ((cMAX_ACQ_TIME - cDEMOD_SETTING_TIME) / (cLOCK_POLLING_TIME + cDEMOD_CHECK_TIME)); ii ++)
            {
                /* Hui add for refresh status after remove UpdateInfo in Dtd_GetSync. */
                vDTDGetStatus(psDemodCtx);
                if (DTD_PdGetSync(psDemodCtx, e_conn_type))
                {
                    DTD_Connected(psDemodCtx);
#if fcADD_ACQ_CHK
                    fgRetSts = TRUE;
#endif
                    break;
                }

                if (DTD_CheckTimeout(psDemodCtx))
                    break;

                //++DelayBreak(ucTunerIdx, cLOCK_POLLING_TIME);
                mcDELAY_MS(cLOCK_POLLING_TIME);

                if (psTDDTDCtx->isBreak(pvArg))
                {
                    break;
                }
                //--
        #ifdef CC_TUNER_CONN_DEFAULT_SURF
                // Add a criteria for MOSAIC_UNLOCK
                if(psDemodCtx->ConnectionType == DEMOD_CONN_TYPE_SURF)
                {
                    mcGET_SYS_TIME(u4GetSyncNowTick);
                    mcGET_DIFF_TIME(u4TickDiff, u4ConStartTick, u4GetSyncNowTick);
                    mcSHOW_DBG_MSG(("Time of get enough pkt = %d ms\n", mcCONV_SYS_TIME(u4TickDiff)));

                    if ((mcCONV_SYS_TIME(u4TickDiff)>=cMONITOR_PERIOD_MS)&&(psDemodCtx->fgMosaicUnLock))
                    {
                        u4ConStartTick = u4GetSyncNowTick;
                        DTD_AccumPER(psDemodCtx);
                    }
                }
        #endif
                mcCHK_STATE();
            }
        }
//#endif
    }
    DTD_AcqEnd(psDemodCtx, psTDDTDCtx->isBreak(pvArg));
   
//#ifdef DIGUS_ENABLE_MONITOR
    #if fcMEASURE_DRIVER_TIME != 2//For Driver ATP,To avoid "DTD_TunerAcq=?" to be interrupted by monitor thread 
    mcSHOW_DBG_MSG(("QamSize(%d) Freq(%dKHz) DTD_TunerAcq = %d\n ", _QAMSize, _Freq1, fgRetSts));
    #endif
    if(pDTDunerDeviceCtx->u1MWIntf != cMTAL_IF && pDTDunerDeviceCtx->u1MWIntf != cSDAL_IF)
    {
        if (!psTDDTDCtx->isBreak(pvArg))
        {
            mcGET_SYS_TIME(u4MonitorStartTick);
            u4PreMonitorTime = mcCONV_SYS_TIME(u4MonitorStartTick);
            psTDDTDCtx->fgBreakMonitor = FALSE;//resume monitor
            mcMUTEX_UNLOCK(psTDDTDCtx->t_escape_mon_suspend);
            mcSHOW_DBG_MSG(("Resume DTD monitor thread in DTD_TunerAcq\n"));
        }
    }
//#endif

#if fcMEASURE_DRIVER_TIME == 2
    mcESTIMATE_TIME((" DTD_TunerAcq = %d ", fgRetSts));
#endif

    mcSEMA_UNLOCK_HAL(psTDDTDCtx->hHalLock);

#if fcADD_ACQ_CHK
    return fgRetSts;
#endif
}

/***********************************************************************/
INT32 DTD_TunerClose(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
TD_DTD_CTX_T *psTDDTDCtx = (TD_DTD_CTX_T *) ptTDSpecificCtx;
DEMOD_CTX_T *psDemodCtx = psTDDTDCtx->psDemodInbInfo;
INT32   retsts;

//#ifdef DIGUS_ENABLE_MONITOR
    if(pDTDunerDeviceCtx->u1MWIntf != cMTAL_IF && pDTDunerDeviceCtx->u1MWIntf != cSDAL_IF)
    {
        psTDDTDCtx->fgBreakMonitor = TRUE;
        psTDDTDCtx->fgMonitorThreadRunning = FALSE; //set to FALSE before unlock sema t_escape_mon_suspend
        mcMUTEX_UNLOCK(psTDDTDCtx->t_escape_mon_suspend);
        PD_DTDStopMonitorEngine(psTDDTDCtx);
        mcMUTEX_DESTROY(psTDDTDCtx->t_escape_mon_suspend);
    }
//#endif

    // Call DTD_Close
    retsts = DTD_Close(psDemodCtx);

#if fcADD_HAL_LOCK
    if (mcSEMA_DESTROY(psTDDTDCtx->hHalLock) != OSR_OK)
    {
        mcSHOW_HW_MSG(("%s: mcSEMA_DESTROY(hHalLock) Fail!\n", __FUNCTION__));
        return (DRVAPI_TUNER_ERROR);
    }
#endif

    //++ The release oder is very important,
    //   make sure the following order is correct.
    mcFREE_MEM(psDemodCtx);
    mcFREE_MEM(ptTDSpecificCtx);
    //--

    return (retsts);
}

VOID DTD_TunerCommand(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                        INT32 i4Argc, const CHAR** aszArgv)
{
TD_DTD_CTX_T *psTDDTDCtx = (TD_DTD_CTX_T *) ptTDSpecificCtx;

    mcSEMA_LOCK_HAL(psTDDTDCtx->hHalLock);   // Avoid re-entry

    pi_command(psTDDTDCtx, i4Argc, aszArgv);

    mcSEMA_UNLOCK_HAL(psTDDTDCtx->hHalLock);
}

VOID DTD_TunerDtdCommand(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                        INT32 i4Argc, const CHAR** aszArgv)
{
TD_DTD_CTX_T *psTDDTDCtx = (TD_DTD_CTX_T *) ptTDSpecificCtx;

    mcSEMA_LOCK_HAL(psTDDTDCtx->hHalLock);   // Avoid re-entry

    pi_external_command(psTDDTDCtx, i4Argc, aszArgv);

    mcSEMA_UNLOCK_HAL(psTDDTDCtx->hHalLock);
}

VOID DTD_TunerAtdCommand(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                        INT32 i4Argc, const CHAR** aszArgv)
{
#if fcADD_HAL_LOCK
TD_DTD_CTX_T *psTDDTDCtx = (TD_DTD_CTX_T *) ptTDSpecificCtx;

    mcSEMA_LOCK_HAL(psTDDTDCtx->hHalLock);   // Avoid re-entry
#endif

    //pi_command(psTDDTDCtx, i4Argc, aszArgv);

    mcSEMA_UNLOCK_HAL(psTDDTDCtx->hHalLock);
}

VOID  DTD_TunerDisc(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                DRV_CONN_TYPE_T e_conn_type)
{
TD_DTD_CTX_T *psTDDTDCtx = (TD_DTD_CTX_T *) ptTDSpecificCtx;
DEMOD_CTX_T *psDemodCtx = psTDDTDCtx->psDemodInbInfo;
//UCHAR   _ConnType;


    mcSEMA_LOCK_HAL(psTDDTDCtx->hHalLock);   // Avoid re-entry

    psTDDTDCtx->fgBreakMonitor = TRUE;

    DTD_DisConnect(psDemodCtx);

    mcSEMA_UNLOCK_HAL(psTDDTDCtx->hHalLock);
}

VOID DTD_TunerGetSignal(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                    DRV_CONN_TYPE_T e_conn_type,
                    SIGNAL *_pSignal)
{
TD_DTD_CTX_T *psTDDTDCtx = (TD_DTD_CTX_T *) ptTDSpecificCtx;
DEMOD_CTX_T *psDemodCtx = psTDDTDCtx->psDemodInbInfo;

    mcSEMA_LOCK_HAL(psTDDTDCtx->hHalLock);   // Avoid re-entry

    _pSignal->Frequency  = psDemodCtx->Frequency;
    _pSignal->SymbolRate = psDemodCtx->SymbolRate;
    _pSignal->QAMSize    = psTDDTDCtx->QAMSize;
    if (_pSignal->QAMSize == 999)
    {
    UINT8 u1Mod;

        DTD_GetMod(psDemodCtx, &u1Mod);
        if (u1Mod == MOD_J83B_64QAM)
            _pSignal->QAMSize = 64;
        else
            _pSignal->QAMSize = 256;
    }

    mcSEMA_UNLOCK_HAL(psTDDTDCtx->hHalLock);
}

UINT8  DTD_TunerGetSignalLevel(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                DRV_CONN_TYPE_T e_conn_type)
{
TD_DTD_CTX_T *psTDDTDCtx = (TD_DTD_CTX_T *) ptTDSpecificCtx;
DEMOD_CTX_T *psDemodCtx = psTDDTDCtx->psDemodInbInfo;
UINT8   u1RetValue;

    mcSEMA_LOCK_HAL(psTDDTDCtx->hHalLock);   // Avoid re-entry

#if 0  //Hui
    DTD_GetSync(psDemodCtx);
#endif

    u1RetValue = DTD_GetSignalLevel(psDemodCtx);

    mcSEMA_UNLOCK_HAL(psTDDTDCtx->hHalLock);
    return (u1RetValue);
}

INT16 DTD_TunerGetSignalLeveldBm(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                    DRV_CONN_TYPE_T e_conn_type)
{
TD_DTD_CTX_T *psTDDTDCtx = (TD_DTD_CTX_T *) ptTDSpecificCtx;
DEMOD_CTX_T *psDemodCtx = psTDDTDCtx->psDemodInbInfo;
INT16   i2RetValue;

    mcSEMA_LOCK_HAL(psTDDTDCtx->hHalLock);   // Avoid re-entry

#if 0  //Hui
    DTD_GetSync(psDemodCtx);
#endif

    i2RetValue = DTD_GetSignalLeveldBm(psDemodCtx);

    mcSEMA_UNLOCK_HAL(psTDDTDCtx->hHalLock);
    return (i2RetValue);
}

UINT16  DTD_TunerGetSignalPER(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                    DRV_CONN_TYPE_T e_conn_type)
{
TD_DTD_CTX_T *psTDDTDCtx = (TD_DTD_CTX_T *) ptTDSpecificCtx;
DEMOD_CTX_T *psDemodCtx = psTDDTDCtx->psDemodInbInfo;
UINT16  u2RetValue;

    mcSEMA_LOCK_HAL(psTDDTDCtx->hHalLock);   // Avoid re-entry

#if 0  //Hui
    DTD_GetSync(psDemodCtx);
#endif

    u2RetValue = DTD_GetSignalPER(psDemodCtx);

    mcSEMA_UNLOCK_HAL(psTDDTDCtx->hHalLock);
    return (u2RetValue);
}

UINT16  DTD_TunerGetSignalSNR(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                    DRV_CONN_TYPE_T e_conn_type)
{
TD_DTD_CTX_T *psTDDTDCtx = (TD_DTD_CTX_T *) ptTDSpecificCtx;
DEMOD_CTX_T *psDemodCtx = psTDDTDCtx->psDemodInbInfo;
UINT16  u2RetValue;

    mcSEMA_LOCK_HAL(psTDDTDCtx->hHalLock);   // Avoid re-entry

#if 0  //Hui
    DTD_GetSync(psDemodCtx);
#endif

    u2RetValue = DTD_GetSignalSNR(psDemodCtx);

    mcSEMA_UNLOCK_HAL(psTDDTDCtx->hHalLock);
    return (u2RetValue);
}

CHAR *DTD_TunerGetVer(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
TD_DTD_CTX_T *psTDDTDCtx = (TD_DTD_CTX_T *) ptTDSpecificCtx;
DEMOD_CTX_T *psDemodCtx = psTDDTDCtx->psDemodInbInfo;
CHAR    *szRetValue;

    mcSEMA_LOCK_HAL(psTDDTDCtx->hHalLock);   // Avoid re-entry

    szRetValue = DTD_GetVer(psDemodCtx);

    mcSEMA_UNLOCK_HAL(psTDDTDCtx->hHalLock);
    return (szRetValue);
}

INT32 DTD_TunerNimTest(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
TD_DTD_CTX_T *psTDDTDCtx = (TD_DTD_CTX_T *) ptTDSpecificCtx;
INT32   i4RetValue;

    mcSEMA_LOCK_HAL(psTDDTDCtx->hHalLock);   // Avoid re-entry

    i4RetValue = pi_nimTest(psTDDTDCtx);

    mcSEMA_UNLOCK_HAL(psTDDTDCtx->hHalLock);
    return (i4RetValue);
}

INT32 DTD_TunerRegRead(UCHAR ucI2cAddress, UCHAR ucRegAddr, UCHAR *pucBuffer, UINT16 ucByteCount)
{
    return (pi_regRead(ucI2cAddress, ucRegAddr, pucBuffer, ucByteCount));
}

/***********************************************************************/
VOID DTD_TunerSetMpgFmt(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                            MPEG_FMT_T *pt_mpeg_fmt)
{
TD_DTD_CTX_T *psTDDTDCtx = (TD_DTD_CTX_T *) ptTDSpecificCtx;
DEMOD_CTX_T *psDemodCtx = psTDDTDCtx->psDemodInbInfo;
UINT8   u1SetMode;

    mcSEMA_LOCK_HAL(psTDDTDCtx->hHalLock);   // Avoid re-entry

    u1SetMode = 0;
    if (pt_mpeg_fmt->eValidPolarity == POL_LOW_ACTIVE)  // Valid Low Active
        mcSET_BIT(u1SetMode, DEMOD_TS_FORMAT_VALID_LOW);
    if (pt_mpeg_fmt->eSyncPolarity == POL_LOW_ACTIVE)   // Sync Low Active
        mcSET_BIT(u1SetMode, DEMOD_TS_FORMAT_SYNC_LOW);
    if (pt_mpeg_fmt->eClockPolarity == POL_LOW_ACTIVE)  // Data Falling Edge
        mcSET_BIT(u1SetMode, DEMOD_TS_FORMAT_DATA_FALL);
    if (pt_mpeg_fmt->fgParallelOutput)                  // Output Parallel
        mcSET_BIT(u1SetMode, DEMOD_TS_FORMAT_PARALLEL);
    if (!pt_mpeg_fmt->fgGatedClock)                     // Clock Continuous
        mcSET_BIT(u1SetMode, DEMOD_TS_FORMAT_CLK_CONT);

    DTD_SetTsFmt(psDemodCtx, u1SetMode);

    mcSEMA_UNLOCK_HAL(psTDDTDCtx->hHalLock);
}

VOID DTD_TunerSetRegSetting(PTD_SPECIFIC_CTX ptTDSpecificCtx, UCHAR ucRegSet,
                            UCHAR ucRegAddr, UCHAR ucRegValue)
{
TD_DTD_CTX_T *psTDDTDCtx = (TD_DTD_CTX_T *) ptTDSpecificCtx;

    mcSEMA_LOCK_HAL(psTDDTDCtx->hHalLock);   // Avoid re-entry

    pi_setRegSetting(psTDDTDCtx, ucRegSet, ucRegAddr, ucRegValue);

    mcSEMA_UNLOCK_HAL(psTDDTDCtx->hHalLock);
}

VOID DTD_TunerShowRegSetting(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
TD_DTD_CTX_T *psTDDTDCtx = (TD_DTD_CTX_T *) ptTDSpecificCtx;

    mcSEMA_LOCK_HAL(psTDDTDCtx->hHalLock);   // Avoid re-entry

    pi_showRegSetting(psTDDTDCtx);

    mcSEMA_UNLOCK_HAL(psTDDTDCtx->hHalLock);
}

/***********************************************************************/
VOID DTD_TunerTestI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
TD_DTD_CTX_T *psTDDTDCtx = (TD_DTD_CTX_T *) ptTDSpecificCtx;

    mcSEMA_LOCK_HAL(psTDDTDCtx->hHalLock);   // Avoid re-entry

    pi_testI2C(psTDDTDCtx);

    mcSEMA_UNLOCK_HAL(psTDDTDCtx->hHalLock);
}

VOID DTD_TunerDetachI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx, BOOL bDetach)
{
TD_DTD_CTX_T *psTDDTDCtx = (TD_DTD_CTX_T *) ptTDSpecificCtx;

    mcSEMA_LOCK_HAL(psTDDTDCtx->hHalLock);   // Avoid re-entry

   /* No implement */
   
    mcSEMA_UNLOCK_HAL(psTDDTDCtx->hHalLock);
}

//------------------------------------------------------------------------- 
/*
 *  DTD_TunerBypassI2C
 *  To control digital demod tunerBypass switch ON or OFF
 *  - Only control the switch when external demod case (ex. MT5112EE)
 *  - No implement when internal demod case (ex. MT5388)
 *  @param  ptTDSpecificCtx    PD DTD context
 *  @param  bSwitchOn          TRUE(TunerBypass ON) or FALSE(TunerBypass OFF)
 *  @retval void
 */
//-------------------------------------------------------------------------
VOID DTD_TunerBypassI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx, BOOL bSwitch)
{
TD_DTD_CTX_T *psTDDTDCtx = (TD_DTD_CTX_T *) ptTDSpecificCtx;

    mcSEMA_LOCK_HAL(psTDDTDCtx->hHalLock);   // Avoid re-entry

   // Add your driver code here
#ifdef fcEXTERN_DEMOD 
    pi_TunerBypassI2C(psTDDTDCtx, bSwitch);
#endif

    mcSEMA_UNLOCK_HAL(psTDDTDCtx->hHalLock);
}

/***********************************************************************/
INT16 DTD_TunerGetAttribute(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                           DRV_GET_TYPE_T  e_get_type,
                           void *pvAnaAttribute,
                           SIZE_T* pzAnaAttributeLen)
{
#if fcADD_MW_DEMOD_INFO
TD_DTD_CTX_T *psTDDTDCtx = (TD_DTD_CTX_T *) ptTDSpecificCtx;
DEMOD_CTX_T *psDemodCtx = psTDDTDCtx->psDemodInbInfo;
UINT32 u4Value;
INT16 i2Value;
UINT8 u1Value;
INT32   ai4Data[cAGC_INFO_SIZE];
BOOL    fgIsCFO;

    mcSEMA_LOCK_HAL(psTDDTDCtx->hHalLock);   // Avoid re-entry

    switch (e_get_type)
    {
        case TUNER_GET_TYPE_DBM_SNR:
#if 0  //Hui
            DTD_GetSync(psDemodCtx);
#endif
            i2Value = (INT16) DTD_GetSignalSNR(psDemodCtx);
            *(UINT16*)pvAnaAttribute = i2Value;
            *pzAnaAttributeLen = sizeof(INT16);
            break;

        case TUNER_GET_TYPE_AGC:
#if 0  //Hui
            DTD_GetSync(psDemodCtx);
#endif
            u1Value = DTD_GetSignalLevel(psDemodCtx);
            *(UINT8*)pvAnaAttribute = u1Value;
            *pzAnaAttributeLen = sizeof(UINT8);
            break;

        case TUNER_GET_TYPE_PACKET_ERRORS:
            //u4Value = (UINT32)psDemodCtx->u4AccPktErr;
            if (psDemodCtx->u2PktErr != 0xFFFF)
            {
                  // Update when PktCnt is ready
                psDemodCtx->u2PrePktErr = psDemodCtx->u2PktErr;
                u4Value = (UINT32)psDemodCtx->u2PktErr;
            }
            else
            {
                u4Value = (UINT32)psDemodCtx->u2PrePktErr;
            }
            *(UINT32*)pvAnaAttribute = u4Value;
            *pzAnaAttributeLen = sizeof(UINT32);
            break;

        case TUNER_GET_TYPE_PACKET_STATUS:
              ((TUNER_PACKET_STATUS_T *) pvAnaAttribute)->ui1_error_rate = psDemodCtx->u1ErrRateIdx;
              ((TUNER_PACKET_STATUS_T *) pvAnaAttribute)->ui1_lost_rate = psDemodCtx->u1LossRateIdx;
            *pzAnaAttributeLen = sizeof(TUNER_PACKET_STATUS_T);
            mcSHOW_DBG_MSG2(("TUNER_GET_TYPE_PACKET_STATUS %d-%d ",
                              ((TUNER_PACKET_STATUS_T *) pvAnaAttribute)->ui1_error_rate, ((TUNER_PACKET_STATUS_T *) pvAnaAttribute)->ui1_lost_rate));
            break;

        case TUNER_GET_TYPE_INTERLEAVING:
            DTD_GetQAMInterleave(psDemodCtx,
                                    &(((TUNER_INTERLEAVING_T *) pvAnaAttribute)->ui1_interleaving_i),
                                    &(((TUNER_INTERLEAVING_T *) pvAnaAttribute)->ui1_interleaving_j));

            *pzAnaAttributeLen = sizeof(TUNER_INTERLEAVING_T);
            break;

        case TUNER_GET_TYPE_KHZ_CFO:
            DTD_Get_AGCGain_CFO(psDemodCtx, ai4Data, &fgIsCFO); /* Get CFO */
            *(INT32*)pvAnaAttribute = ai4Data[cVCR_CFO]/1000;  /*Return (KHz) back*/
            //mcSHOW_DBG_MSG(("CFO=%d\n", ai4Data[cVCR_CFO]));
            *pzAnaAttributeLen = sizeof(INT32);
            break;
        case TUNER_GET_TYPE_TS_FMT:
              {
                TS_FMT_T *pTsFmt=(TS_FMT_T *)pvAnaAttribute;
                
                mcSHOW_DBG_MSG(("%s TUNER_SET_TYPE_TS_TRISTATE\n",__FUNCTION__));
                
           if(DTD_GetTsOutFmt(psDemodCtx,&u1Value))
                {
                //default
                    pTsFmt->SerOrPar=SP_SERIAL;
                pTsFmt->ClockPolarity=POL_HIGH_ACTIVE;
                pTsFmt->fgTristate=BOOL_FALSE;
                pTsFmt->fgGatedClock=BOOL_TRUE;
                 pTsFmt->fgExternalDemod=BOOL_FALSE;//default inside  SOC
                 pTsFmt->u4TsClockInKHz=50000;//50MHz Serial
                
              if(mcTEST_BIT(u1Value,DEMOD_TS_FORMAT_PARALLEL))//PARALLEL
              {
                  pTsFmt->SerOrPar=SP_PARALLEL;
              }
              if(mcTEST_BIT(u1Value,DEMOD_TS_FORMAT_SYNC_LOW))//LOW active
              {
                pTsFmt->ClockPolarity=POL_LOW_ACTIVE;
              }
              if(mcTEST_BIT(u1Value,DEMOD_TS_FORMAT_OUTPUT_EN))//Output enable
              {
                pTsFmt->fgTristate=BOOL_TRUE;
              }
              
              if(mcTEST_BIT(u1Value, DEMOD_TS_FORMAT_CLK_CONT))//If gated clock
              {
               pTsFmt->fgGatedClock=BOOL_FALSE;
              }
              
               if(pTsFmt->SerOrPar==SP_PARALLEL)
               {
                  pTsFmt->u4TsClockInKHz/=8;
               }
               
               
               #ifdef fcEXTERN_DEMOD 
                pTsFmt->fgExternalDemod=BOOL_TRUE;
               #endif
              
                 //DTD_TransTsOutFmt(&u1Value,(TS_FMT_T *)pvAnaAttribute,TRUE);//translate value to ts format structure
                 *pzAnaAttributeLen = sizeof(TS_FMT_T);
              }
            }
             break;
    }

    mcSEMA_UNLOCK_HAL(psTDDTDCtx->hHalLock);
#endif

    return 0;
}

/***********************************************************************/
INT32 DTD_TunerSetAttribute(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                           DRV_SET_TYPE_T  e_set_type,
                           const VOID *pvAnaAttribute,
                           SIZE_T zAnaAttributeLen)
{
    UINT8 u1Value=0;
    
#if defined(TUNER_SET_TYPE_SMART_ANTENNA_PARS) || fcTUNE_NOT_START
TD_DTD_CTX_T *psTDDTDCtx = (TD_DTD_CTX_T *) ptTDSpecificCtx;
DEMOD_CTX_T *psDemodCtx = psTDDTDCtx->psDemodInbInfo;
#endif

#if (defined(TUNER_SET_TYPE_SMART_ANTENNA_PARS) || fcTUNE_NOT_START)
    mcSEMA_LOCK_HAL(psTDDTDCtx->hHalLock);   // Avoid re-entry
#endif

    switch (e_set_type)
    {
#ifdef TUNER_SET_TYPE_SMART_ANTENNA_PARS
        case TUNER_SET_TYPE_SMART_ANTENNA_PARS:
            if (zAnaAttributeLen)
            {
            PSMART_ANTENNA_PARS_T pt_smart_ant = (PSMART_ANTENNA_PARS_T) pvAnaAttribute;

                vWriteSmartAnt(psDemodCtx, pt_smart_ant->Direction,
                               pt_smart_ant->Gain, pt_smart_ant->Polarity);
            }
            break;
#endif

#if fcTUNE_NOT_START
        case TUNER_SET_SPECIAL_TYPE_ACQ_ACTION_CLASS:
            if (zAnaAttributeLen)
            {
            TUNER_ACQ_ACTION_T *peActionType = (TUNER_ACQ_ACTION_T *) pvAnaAttribute;

                mcSHOW_DBG_MSG(("%s: (%d, %X) eActionType=%d!\n", __FUNCTION__,
                                TUNER_SET_SPECIAL_TYPE_ACQ_ACTION_CLASS,
                                TUNER_SET_SPECIAL_TYPE_ACQ_ACTION_CLASS, *peActionType));
                psDemodCtx->u1TunerAcqType = *peActionType;
            }
            break;
#endif

#if fcADD_MW_DEMOD_INFO
        case TUNER_SET_TYPE_DIAGNOSTICS:
            if (zAnaAttributeLen)
            {
            UINT8 fgStartDiagErrCnt = *((UINT8*) pvAnaAttribute);

                if (fgStartDiagErrCnt)
                {
                    psDemodCtx->u1PecFlag = 1;
                    psDemodCtx->u4PecObserveTime = cUINT32_MAX;
                    psDemodCtx->u2PecRefreshTime = 2;
                    DTD_PERInit(psDemodCtx);
                }
                else
                {
                    psDemodCtx->u1PecFlag = 3;
                }
            }
            break;
#endif
        case TUNER_SET_TYPE_TS_SP:
            { SIG_SP_T  SP;
                
                mcSHOW_DBG_MSG(("%s TUNER_SET_TYPE_TS_SP\n",__FUNCTION__));
                
             if (zAnaAttributeLen)
                {
                    SP=*((SIG_SP_T *)pvAnaAttribute);
                    
                    if(DTD_GetTsOutFmt(psDemodCtx,&u1Value))
                {
                    if(SP==SP_SERIAL)
                        {
                            mcCLR_BIT(u1Value, DEMOD_TS_FORMAT_PARALLEL);
                        }
                    else if(SP==SP_PARALLEL)
                        {
                            mcSET_BIT(u1Value, DEMOD_TS_FORMAT_PARALLEL);
                        }
                    else//UNKOWN
                        {
                            break;
                        }
                     DTD_SetTsOutFmt(psDemodCtx,u1Value);
                    }
                }
           break;
        }
          case TUNER_SET_TYPE_TS_TRISTATE:
            { BOOL_T  fgOut;
                
                mcSHOW_DBG_MSG(("%s TUNER_SET_TYPE_TS_TRISTATE\n",__FUNCTION__));
                
             if (zAnaAttributeLen)
                {
                    fgOut=*((BOOL_T *)pvAnaAttribute);
                    
                    if(DTD_GetTsOutFmt(psDemodCtx,&u1Value))
                {
                    if(fgOut==BOOL_TRUE)
                        {
                            mcCLR_BIT(u1Value, DEMOD_TS_FORMAT_OUTPUT_EN);
                        }
                    else if(fgOut==BOOL_FALSE)
                        {
                            mcSET_BIT(u1Value, DEMOD_TS_FORMAT_OUTPUT_EN);
                        }
                    else//UNKOWN
                        {
                            break;
                        }
                
                     DTD_SetTsOutFmt(psDemodCtx,u1Value);
                    }
                }
           break;
        }
        default:
            mcSHOW_DRVERR_MSG(("DTD_TunerSetAttribute: RMR_DRV_INV_SET_INFO!\n"));
#if (defined(TUNER_SET_TYPE_SMART_ANTENNA_PARS) || fcTUNE_NOT_START)
            mcSEMA_UNLOCK_HAL(psTDDTDCtx->hHalLock);
#endif
            return (RMR_DRV_INV_SET_INFO);
        //    break;
    }

#if (defined(TUNER_SET_TYPE_SMART_ANTENNA_PARS) || fcTUNE_NOT_START)
    mcSEMA_UNLOCK_HAL(psTDDTDCtx->hHalLock);
#endif
#if defined(TUNER_SET_TYPE_SMART_ANTENNA_PARS) || fcTUNE_NOT_START
    return (RMR_OK);
#endif
}

/***********************************************************************/
#ifdef fcADD_DIAG_INFO
INT32 DTD_TunerSetDiagCmd(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                        DRV_CUSTOM_TUNER_SET_TYPE_T     e_tuner_set_type,
                        VOID*                           pv_set_info,
                        SIZE_T                          z_size)
{
    #if fcADD_HAL_LOCK
TD_DTD_CTX_T *psTDDTDCtx = (TD_DTD_CTX_T *) ptTDSpecificCtx;

    mcSEMA_LOCK_HAL(psTDDTDCtx->hHalLock);   // Avoid re-entry
    #endif

    switch(e_tuner_set_type)
    {
        case DRV_CUSTOM_TUNER_SET_TYPE_DIAG_OPEN:
            // Do initialization here
            break;

        case DRV_CUSTOM_TUNER_SET_TYPE_DIAG_CLOSE:
            // Do cleanup here
            break;

        default:
            mcSEMA_UNLOCK_HAL(psTDDTDCtx->hHalLock);
            return RMR_INV_SET;
    }

    mcSEMA_UNLOCK_HAL(psTDDTDCtx->hHalLock);
    return RMR_OK;
}

#define DRV_CUSTOM_TUNER_DIAG_ITEMS         19
EXTERN CHAR *DebugChPtr;

INT32 DTD_TunerGetDiagInfo(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                        DRV_CUSTOM_TUNER_GET_TYPE_T     e_tuner_get_type,
                        VOID*                           pv_get_info,
                        SIZE_T*                         pz_size)
{
TD_DTD_CTX_T *psTDDTDCtx = (TD_DTD_CTX_T *) ptTDSpecificCtx;
DEMOD_CTX_T *psDemodCtx = psTDDTDCtx->psDemodInbInfo;
/*
TUNER_TYPE_DIAG_CONTENT_T *pDiagContent = (TUNER_TYPE_DIAG_CONTENT_T*) pv_get_info;
UCHAR   aucData[cMAX_READ_NUM + 1];         // For Host Cmd
CHAR    *cString1;
UINT8   u1Value[4];
INT32   ai4Data[cAGC_INFO_SIZE];
BOOL    fgIsCFO, fgRetSts;
char    szCFO[cCFO_STR_SIZE];
*/
//CHAR*       ptmpch;
//CHAR        char1;
//CHAR        char2;

    mcSEMA_LOCK_HAL(psTDDTDCtx->hHalLock);   // Avoid re-entry

    switch (e_tuner_get_type)
    {
        case DRV_CUSTOM_TUNER_GET_TYPE_DIAG_CONTROL:
            if ((*pz_size) < sizeof(TUNER_TYPE_DIAG_CONTROL_T))
            {
                *pz_size = sizeof(TUNER_TYPE_DIAG_CONTROL_T);
                mcSEMA_UNLOCK_HAL(psTDDTDCtx->hHalLock);
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }
            ((TUNER_TYPE_DIAG_CONTROL_T *) pv_get_info)->u2PollPeriodMs = 2000;
            ((TUNER_TYPE_DIAG_CONTROL_T *) pv_get_info)->ucCntOfDiagStruc = DRV_CUSTOM_TUNER_DIAG_ITEMS;
            psDemodCtx->u4PecObserveTime = 300 * 1000;  // Default: 5min
            psDemodCtx->u2PecRefreshTime = 2 * 1000;    // Default: 2sec
            psDemodCtx->u1PecFlag = 1;              // Start PEC
            break;

     /*   case DRV_CUSTOM_TUNER_GET_TYPE_DIAG_CONTENT:
            if ((*pz_size) < DRV_CUSTOM_TUNER_DIAG_ITEMS * sizeof(TUNER_TYPE_DIAG_CONTENT_T))
            {
                *pz_size = DRV_CUSTOM_TUNER_DIAG_ITEMS * sizeof(TUNER_TYPE_DIAG_CONTENT_T);
                mcSEMA_UNLOCK_HAL(psTDDTDCtx->hHalLock);
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }

            // Record 1
            // To add new items, please remember to modify DRV_CUSTOM_TUNER_DIAG_ITEMS
            // Note that both cDisplayName and cDisplayValue can NOT exceed 15 bytes
            sprintf(pDiagContent->cDisplayName, "%s", "Version:");
            //ptmpch = (CHAR*)pDiagContent->cDisplayName;
            //char1 = *ptmpch;
            //mcSHOW_USER_MSG(("pg_glue begin, first char = %c\n", *ptmpch));
            sprintf(pDiagContent->cDisplayValue, "%s", "--");
            mcSET_CMD2(aucData) = cCMD_ID_GET_LABEL;
            if (mcHOST_CMD(psDemodCtx, aucData, cOFST_LABEL_SIZE, 1))
            {
                sprintf(pDiagContent->cDisplayValue,"%d.%d %c%02X%02X-%02X%02X",
                                psDemodCtx->u1ChipGen + 1, psDemodCtx->u1ChipRev + 1,
                                mcGET_QUEUE(aucData, cOFST_LABEL_BRANCH_TYPE),
                                mcGET_QUEUE(aucData, cOFST_LABEL_BRANCH_ID_1), mcGET_QUEUE(aucData, cOFST_LABEL_BRANCH_ID_2),
                                mcGET_QUEUE(aucData, cOFST_LABEL_ID_1), mcGET_QUEUE(aucData, cOFST_LABEL_ID_2));
            }  
            pDiagContent++;

            // Record 2
            sprintf(pDiagContent->cDisplayName, "%s", "Tuner:");
            ITuner_OP(psDemodCtx->sTunerCtx,itGetVer,0,&cString1);
            sprintf(pDiagContent->cDisplayValue, "%s",cString1);
            pDiagContent++;

            // Record 3
            sprintf(pDiagContent->cDisplayName, "%s", "Modulation:");
            if (psDemodCtx->Modulation == MOD_ATSC_8VSB)
                cString1 = "ATSC_VSB";
            else if (psDemodCtx->Modulation == MOD_J83B_64QAM)
                cString1 = "J83B_64QAM";
            else
                cString1 = "J83B_256QAM";
            sprintf(pDiagContent->cDisplayValue, "%s", cString1 );
            pDiagContent++;

            // Record 4
            sprintf(pDiagContent->cDisplayName, "%s", "RF Freq (KHz):");
          //  sprintf(pDiagContent->cDisplayValue, "%6d (Ch%3d)", psDemodCtx->Frequency, u2ConvFreq2Chan(psDemodCtx));
            pDiagContent++;

            // Record 5
            sprintf(pDiagContent->cDisplayName, "%s", "SpcInv, Samp:");
            sprintf(pDiagContent->cDisplayValue, "%s", "--" );
            mcSET_CMD2(aucData) = cCMD_ID_DEMOD_GET_INFO3;
            if (mcHOST_CMD(psDemodCtx, aucData, cOFST_DEMOD_INFO3_SIZE, 1))
            {
                cString1 = "Norm";
                if (mcTEST_BIT(mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_SPECTRUM), 1))
                    cString1 = "Inv";

                u1Value[0] = 50;
                if (mcTEST_BIT(mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_SPECTRUM), 4))
                    u1Value[0] = 60;

                sprintf(pDiagContent->cDisplayValue, "%s, %2d MHz", cString1, u1Value[0] );
            }
            pDiagContent++;

            // Record 6
            sprintf(pDiagContent->cDisplayName, "%s", "AD TargetLevel:");
            u1GetRegs(psDemodCtx, DTD_REG_PRA_Loop_AD0, &u1Value[0], 2);
            sprintf(pDiagContent->cDisplayValue, "0x%02X%02X", u1Value[0], u1Value[1]);
            pDiagContent++;

            // Record 7
            sprintf(pDiagContent->cDisplayName, "%s", "FW AcqTime:");
            sprintf(pDiagContent->cDisplayValue, "%d ms", psDemodCtx->u2FwAcqTime );
            pDiagContent++;

            // Record 8
            sprintf(pDiagContent->cDisplayName, "%s", "SignalSNR:");
            sprintf(pDiagContent->cDisplayValue, "%d dB", DTD_GetSignalSNR(psDemodCtx));
            pDiagContent++;

            // Record 9
            sprintf(pDiagContent->cDisplayName, "%s", "AccPER:");
            if (psDemodCtx->u1ErrClass == 0)
                cString1 = "NoErr";
            else if (psDemodCtx->u1ErrClass == 1)
                cString1 = "MnrErr";
            else if (psDemodCtx->u1ErrClass == 2)
                cString1 = "HvyErr";
            else
                cString1 = "Unsync";
         //   sprintf(pDiagContent->cDisplayValue, "%u (%s)", psDemodCtx->u4AccErrorRate, cString1);
            pDiagContent++;

            // Record 10
            sprintf(pDiagContent->cDisplayName, "%s", "ErrPkt, RcvPkt:");
         //   sprintf(pDiagContent->cDisplayValue, "(%u %u)", psDemodCtx->u4AccPktErr, psDemodCtx->u4AccPktCnt);
            pDiagContent++;

            // Record 11
            fgRetSts = DTD_Get_AGCGain_CFO(psDemodCtx, ai4Data, &fgIsCFO); // Get RFGain,IFGain,CFO 
            sprintf(pDiagContent->cDisplayName, "%s", "AGC Ctrl:");
            sprintf(pDiagContent->cDisplayValue, "%s", "--" );
         //   if (fgRetSts)
         //       sprintf(pDiagContent->cDisplayValue, "%2dR %2dI", ai4Data[cAGC_RF_CTRL], ai4Data[cAGC_IF_CTRL]);
            pDiagContent++;

            // Record 12
            sprintf(pDiagContent->cDisplayName, "%s", "CFO:");
            sprintf(pDiagContent->cDisplayValue, "%s", "--" );
            if (fgRetSts)
            {
                mcMEMSET(szCFO, 0, cCFO_STR_SIZE);
                szGetCFOStr(ai4Data[cVCR_CFO], fgIsCFO, &szCFO[0]);
                sprintf(pDiagContent->cDisplayValue, "%s", szCFO);
            }
            pDiagContent++;

            // Record 13
            sprintf(pDiagContent->cDisplayName, "%s", "TFO:");
            DTD_GetTFO(psDemodCtx, ai4Data);
            if (psDemodCtx->Modulation == MOD_ATSC_8VSB)
            {
                sprintf(pDiagContent->cDisplayValue, "%d Hz", ai4Data[0]);
            }
            else
            {
               sprintf(pDiagContent->cDisplayValue, "%d ppm", ai4Data[0]);
            }
            pDiagContent++;

            // Record 14
            if (psDemodCtx->Modulation == MOD_ATSC_8VSB)
                sprintf(pDiagContent->cDisplayName, "%s", "EQ Step, Leak:");
         //   else
         //       sprintf(pDiagContent->cDisplayName, "%s", "PN State, Level:");
            // Get Parameter from DTD_GetTFO()
         //   sprintf(pDiagContent->cDisplayValue, "0x%02X, 0x%02X", (UINT8)ai4Data[1], (UINT8)ai4Data[2] );
            pDiagContent++;

            // Record 15
            vDTDGetStatus(psDemodCtx);
            sprintf(pDiagContent->cDisplayName, "%s", "Lock State:");
            sprintf(pDiagContent->cDisplayValue, "%02X", psDemodCtx->ucLockState );
            pDiagContent++;

            // Record 16
            sprintf(pDiagContent->cDisplayName, "%s", "ReAcqCnt (Ind):");
            sprintf(pDiagContent->cDisplayValue, "%d (%02X)", psDemodCtx->u2AccReAcqCnt, psDemodCtx->u1StatusInd );
            pDiagContent++;

            // Record 17
            sprintf(pDiagContent->cDisplayName, "%s", "FTS Status:");
            u1GetRegs(psDemodCtx, DTD_REG_FTS_STATUS, &u1Value[0], 1);
            sprintf(pDiagContent->cDisplayValue, "0x%02X", u1Value[0] );
            pDiagContent++;

            // Record 18
            sprintf(pDiagContent->cDisplayName, "%s", "TV/PN/FN Cfg:");
            sprintf(pDiagContent->cDisplayValue, "0x%02X", ucGetFwVar(psDemodCtx, eVAR_UCVSBDETECTCFG) );
            pDiagContent++;

            // Record 19
            vDTDGetInfo(psDemodCtx);
           // sprintf(pDiagContent->cDisplayName, "%s", "TSIF ReSync Cnt:");
            sprintf(pDiagContent->cDisplayValue, "%d", psDemodCtx->u1CurTSIFCnt);
            pDiagContent++;

            //char2 = *ptmpch;
            //mcSHOW_USER_MSG(("pg_glue first/end = %c %c\n", char1, char2));
            //mcSHOW_USER_MSG(("DebugChPtr = %x\n", DebugChPtr));
            break; /// End of case DRV_CUSTOM_TUNER_GET_TYPE_DIAG_CONTENT
        */   
        default:
            mcSEMA_UNLOCK_HAL(psTDDTDCtx->hHalLock);
            return RMR_DRV_NO_GET_INFO;
    }

    mcSEMA_UNLOCK_HAL(psTDDTDCtx->hHalLock);
    return RMR_OK;
}
#endif

/***********************************************************************/
VOID PD_DTD_Register_LTDIS_Fct(PTD_CTX ptTDCtx)
{
TUNER_DEVICE_CTX_T *pTunerDeviceCtx = ptTDCtx;

    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerClose = DTD_TunerClose;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerAcq = DTD_TunerAcq;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDisc = DTD_TunerDisc;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGerSync = DTD_TunerGetSync;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignal = DTD_TunerGetSignal;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalLevel = DTD_TunerGetSignalLevel;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalLeveldBm = DTD_TunerGetSignalLeveldBm;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalPER = DTD_TunerGetSignalPER;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalSNR = DTD_TunerGetSignalSNR;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetMpgFmt = DTD_TunerSetMpgFmt;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetVer = DTD_TunerGetVer;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerNimTest = DTD_TunerNimTest;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerCommand = DTD_TunerCommand;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDtdCommand = DTD_TunerDtdCommand;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerAtdCommand = DTD_TunerAtdCommand;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetRegSetting = DTD_TunerSetRegSetting;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerShowRegSetting = DTD_TunerShowRegSetting;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerTestI2C = DTD_TunerTestI2C;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDetachI2C = DTD_TunerDetachI2C;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetAttribute = DTD_TunerSetAttribute;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetAttribute = DTD_TunerGetAttribute;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerBypassI2C = DTD_TunerBypassI2C;
#ifdef fcADD_DIAG_INFO
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetDiagCmd = DTD_TunerSetDiagCmd;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetDiagInfo = DTD_TunerGetDiagInfo;
#endif
}

INT32 DTD_TunerOpen(PTD_CTX ptTDCtx, TUNER_CONFIG_T * ptTunerCfgInfo,
                     PTD_SPECIFIC_CTX* pptTDSpecificCtx)
{
DEMOD_CTX_T *psDemodCtx;
TD_DTD_CTX_T *psTDDTDCtx;
INT32   retsts;
#ifdef __MODEL_slt__  // iven temp mark
UINT32 u4RegValue;
#endif

    pDTDunerDeviceCtx = (TUNER_DEVICE_CTX_T*)ptTDCtx;
#if fcMEASURE_DRIVER_TIME == 3
    mcGET_SYS_TIME(u4TickStartMeasure);
    u4PrevStartTime = u4TickStartMeasure;
    mcSHOW_DRVTIME_MSG((" [%s/%s-%d Start]\n", __FILE__, __FUNCTION__, __LINE__));
#endif

    PD_DTD_Register_LTDIS_Fct(ptTDCtx);
#ifdef __MODEL_slt__  // iven temp mark
    #if defined(CC_MT5396)

    mcSHOW_DRVERR_MSG(("Reset Demod\n"));

    u4RegValue = mcRISC_Reg(0xf0000000, 0xd270);
    u4RegValue = (u4RegValue & 0xFFFFCFFF) | 0x00000000;
    mcRISC_Reg(0xf0000000, 0xd270) = u4RegValue;    



    u4RegValue = mcRISC_Reg(0xf0000000, 0xd270);
    u4RegValue = (u4RegValue & 0xFFFFCFFF) | 0x00003000;
    mcRISC_Reg(0xf0000000, 0xd270) = u4RegValue;        

    
    #elif defined(CC_MT5368)

    mcSHOW_DRVERR_MSG(("Reset Demod\n"));
    u4RegValue = mcRISC_Reg(0xf0000000, 0xd270);
    u4RegValue = (u4RegValue & 0xFF9FFFFF) | 0x00000000;
    mcRISC_Reg(0xf0000000, 0xd270) = u4RegValue;


    u4RegValue = mcRISC_Reg(0xf0000000, 0xd270);
    u4RegValue = (u4RegValue & 0xFF9FFFFF) | 0x00600000;
    mcRISC_Reg(0xf0000000, 0xd270) = u4RegValue;    

    

    
    #endif
#endif

    // Allocate TS_SPECIFIC_CTX for DTD Instance
//    psTDDTDCtx = (TD_DTD_CTX_T *) mcALLOC_MEM(sizeof(TD_DTD_CTX_T));
//    psTDDTDCtx = (TD_DTD_CTX_T *) x_mem_alloc(sizeof(TD_DTD_CTX_T));
    psTDDTDCtx = (TD_DTD_CTX_T *) x_mem_calloc(1, sizeof(TD_DTD_CTX_T));
    if (psTDDTDCtx == NULL)
    {
        return (DRVAPI_TUNER_ERROR);
    }
    *pptTDSpecificCtx = psTDDTDCtx;
    //++ Initialize some variables.

    // Allocate Inband Demod Info
    psDemodCtx = (DEMOD_CTX_T *) x_mem_calloc(1, sizeof(DEMOD_CTX_T));
    if (psDemodCtx == NULL)
    {
        mcFREE_MEM(psTDDTDCtx);
        return (DRVAPI_TUNER_ERROR);
    }
    psTDDTDCtx->psDemodInbInfo = psDemodCtx;

#if fcADD_HAL_LOCK
//    if (mcSEMA_CREATE(&psTDDTDCtx->hHalLock, X_SEMA_STATE_UNLOCK) != OSR_OK)
    if (mcSEMA_CREATE(&psTDDTDCtx->hHalLock, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        mcSHOW_HW_MSG(("%s: mcSEMA_CREATE(hHalLock) Fail!\n", __FUNCTION__));
        return (DRVAPI_TUNER_ERROR);
    }
    mcSEMA_UNLOCK_HAL(psTDDTDCtx->hHalLock);
#endif
    psDemodCtx->sTunerCtx=ITunerGetCtx();//Get address of piTunerCtx

#if 0
    // RF Tuner registertration
#if (fcTUNER_TYPE != cDTD_TUNER_ALL)
   // Tuner_Register_Fct(pTuner_API_Fct);
   retsts=ITunerRegistration(cITUNER_DONT_CARE);
#else
    _u1TunerType = _u1CheckTunerTypeFromEeprom();
    retsts=ITunerRegistration(_u1TunerType);
#endif
#endif

    // RF Tuner registertration
    retsts=ITunerRegistration(((TUNER_DEVICE_CTX_T*)ptTDCtx)->tunerType);
    psDemodCtx->u1TunerType = ((TUNER_DEVICE_CTX_T*)ptTDCtx)->tunerType;

if(DRVAPI_TUNER_ERROR==retsts){
    return retsts;
}

#ifdef ERR_I2C_DIG_DEMOD
       mcSHOW_HW_MSG(("Err Code(ERR_I2C_DIG_DEMOD) is defined(%d)\n", ERR_I2C_DIG_DEMOD));
#endif
#ifdef ERR_I2C_TUNER
        mcSHOW_HW_MSG(("Err Code(ERR_I2C_TUNER) is defined(%d)\n", ERR_I2C_TUNER));
#endif
#ifdef ERR_I2C_ANA_DEMOD
        mcSHOW_HW_MSG(("Err Code(ERR_I2C_ANA_DEMOD) is defined(%d)\n", ERR_I2C_ANA_DEMOD));
#endif

//JWEI 2006/10/20
#if fcINTERNAL_TEST || 0
//    u1DbgLevel = 2;
#endif
    psDemodCtx->u4DrvCustTunerCtrl = 0;

#ifdef fcEXTERN_DEMOD
    // Reset demod
    vResetDemod(20); // 20 ms
#endif

    // Call pi_init
    retsts = DTD_Initialize(psDemodCtx, ptTunerCfgInfo->ucDemodInbAddr,TRUE);

#ifdef fcEXTERN_DEMOD
    // If DTD init fail, reset demod again
    if (retsts)
    {
        vResetDemod(100); // 100 ms
        retsts = DTD_Initialize(psDemodCtx, ptTunerCfgInfo->ucDemodInbAddr,TRUE);
    }
#endif

    if (retsts) // If tuner open fail, return
    {
       #ifdef ERR_I2C_DIG_DEMOD
       PDWNC_WriteErrorCode(ERR_I2C_DIG_DEMOD); // Write DIG_DEMOD error code into EEPROM
       mcSHOW_HW_MSG(("####Err Code: ERR_I2C_DIG_DEMOD\n"));
       #endif
       mcSHOW_DBG_MSG(("DTD_Initialize fail!\n"));
       return (DRVAPI_TUNER_ERROR);
    }

//#ifdef DIGUS_ENABLE_MONITOR
    if(pDTDunerDeviceCtx->u1MWIntf != cMTAL_IF && pDTDunerDeviceCtx->u1MWIntf != cSDAL_IF)
    {
        if (mcSEMA_CREATE(&psTDDTDCtx->t_escape_mon_suspend, X_SEMA_STATE_LOCK) != OSR_OK)
        {
            mcSHOW_DBG_MSG(("MT5391_TunerOpen (mcSEMA_CREATE t_escape_mon_suspend): ERROR!\n"));
            return (DRVAPI_TUNER_ERROR);
        }

        psTDDTDCtx->fgBreakMonitor = TRUE;
        // Start Monitor Engine
        PD_DTDStartMonitorEngine(psTDDTDCtx);
    }
//#endif

//#ifdef fcEXTERN_DEMOD 
    DTD_DisConnect(psDemodCtx);  // Disable the interference to the ATV when power on
//#endif

#if fcMEASURE_DRIVER_TIME == 3
    mcESTIMATE_TIME((" [%s/%s-%d End] ", __FILE__, __FUNCTION__, __LINE__));
#endif

    return (retsts);
}

