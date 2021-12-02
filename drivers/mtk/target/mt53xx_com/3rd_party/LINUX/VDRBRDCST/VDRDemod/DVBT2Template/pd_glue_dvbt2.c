/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2008, MediaTek, Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: pd_glue_dvbt2.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
/** @file pd_glue_dvbt2.c
 *  dvbt2 glue layer
 */
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#ifdef INTERNAL_3RD_DEMOD
#include "TDIS.h"
#include "pd_tuner_type.h"
#include "queue.h"
#include "TDMgr.h"
#include "drvapi_tuner.h"
#include "UtilAPI.h"
#include "pod_if.h"
#include "ctrl_bus.h"
#else
#include "TDIS_header.h"
#include "vdr_PD_Def.h"
#include "tunerDebug.h"


#endif 
#include "pd_dvbt2_if.h"
#include "pi_dvbt_if.h"
#include "pi_def_dvbt.h"

#include "u_drv_cust.h"
#include "u_tuner.h"
#include "u_handle.h"

#include "x_os.h"
#include "x_stl_lib.h"
#include "x_gpio.h"
#include "x_rm.h"
#include "x_tuner.h"


#include "tuner_if.h"
#include "tuner_interface_if.h"

#include "drvcust_if.h"
#include "eeprom_if.h"
#include "i2c_api.h"




// This is DUMMY header for SONY Porting!!!
//#include "MTKdef.h"

// for Sony CXD2834 driver
#include "sony_i2c_MTK.h"
#include "sony_dvb_demod.h"
#include "sony_dvb_demodT2.h"
#include "sony_dvb_demod_monitor.h"
#include "sony_dvb_demod_monitorT.h"
#include "sony_dvb_demod_monitorT2.h"
#include "sony_dvb_demod_monitorC.h"
#include "sony_dvb_demod_monitor_PER.h"
#include "sony_dvb_demod_monitor_SSI.h"

#define EMB_L1_DRIVER_CORE          /* empty  */
#define SLAB_WINAPI                 /* empty  */
#define EMB_L1_COMMON_DEMOD_API     /* empty  */
#define EMB_L1_MULTISTANDARD_API    /* empty  */
#define EMB_L1_DVB_C_DEMOD_API      /* empty  */
//#define WINAPI                      /* empty  */ by SONY


   

#ifndef  INTERNAL_3RD_DEMOD
#warning "************Demod VDR_3RD_DEMOD"

#define mcSHOW_USER_MSG(_x_)             mcDBG_MSG_L2(_x_)
#define mcSHOW_DRVERR_MSG(_x_)           mcDBG_MSG_L2(_x_)
#define mcSHOW_OSAI_MSG(_x_)             mcDBG_MSG_L2(_x_)
#define mcSHOW_OSAIOK_MSG(_x_)           mcDBG_MSG_L2(_x_)
#define mcSHOW_DRVAPI_MSG(_x_)           mcDBG_MSG_L2(_x_)
#define mcSHOW_DRVAPIOK_MSG(_x_)         mcDBG_MSG_L2(_x_)
#define mcSHOW_DRVLOCK_MSG(_x_)          mcDBG_MSG_L2(_x_)
#define mcSHOW_API_MSG(_x_)              mcDBG_MSG_L2(_x_)            
#define mcSHOW_DBG_MSG4(_x_)   mcDBG_MSG_L4(_x_)
#define mcSHOW_DBG_MSG2(_x_)   mcDBG_MSG_L2(_x_)
#define mcSHOW_DBG_MSG(_x_)    mcDBG_MSG_L2(_x_)
#define mcSHOW_DBG_MSG1(_x_)   mcDBG_MSG_L1(_x_)
#define mcSHOW_DBG_MSG3(_x_)   mcDBG_MSG_L4(_x_)
#else
#warning "************Demod INTERNAL_3RD_DEMOD"

#define mcDBG_MSG_ERR(_x_)  	mcSHOW_USER_MSG(_x_)  
#define mcDBG_MSG_INFO(_x_)  	mcSHOW_USER_MSG(_x_) 
#define mcDBG_MSG_L1(_x_)  	mcSHOW_USER_MSG(_x_) 
#define mcDBG_MSG_L2(_x_)  	mcSHOW_USER_MSG(_x_) 			
#define mcDBG_MSG_L4(_x_)  	mcSHOW_DBG_MSG4(_x_) 


#endif



//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define CXD2834_DEMOD_I2C_ADDRESS  0xD8  // I2C Slave Address in 8bit form
#define WAIT_TUNER_AGC_STABLE         100   // RF tuner waiting time for Sony Silicon tuner
#define WAIT_TS_LOCK_TIMEOUT_DVBT     1000  // TS Lock waiting time for T
#define WAIT_DEMOD_LOCK_TIMEOUT_DVBT2 3500  // Demod Lock waiting time for T2
#define WAIT_TS_LOCK_TIMEOUT_DVBT2    1500  // TS Lock waiting time for T2
#define WAIT_TS_LOCK_TIMEOUT_DVBC     1000  // TS Lock waiting time for C
#define WAIT_LOCK_INTERVAL            50    // 50ms interval lock check

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define mcSEMA_LOCK_MTFE(sema)                                       \
{                                                                    \
    mcSHOW_DBG_MSG4((" [ _Lock_%s-%s-%d ]\r\n", __FUNCTION__, __LINE__)); \
    x_sema_lock(sema, X_SEMA_OPTION_WAIT);                           \
    mcSHOW_DBG_MSG4((" [ _Excape_%s-%s-%d ]\r\n", __FUNCTION__, __LINE__));     \
}
#define mcSEMA_UNLOCK_MTFE(sema)                                     \
{                                                                    \
    mcSHOW_DBG_MSG4((" [ _Unlock_%s-%s-%d ]\r\n", __FUNCTION__, __LINE__));     \
    x_sema_unlock(sema);                                             \
}

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static DVBT2_PD_CTX_T *pPdCtxDVBT2;
typedef struct _DEMOD_CXD2834_CTX_T // Sony CXD2834 Driver Specific
{ 
    sony_dvb_demod_t demod;
    sony_i2c_t i2c;
    BOOL autoPLP;  // PLP Autodetect ON/OFF
    UINT32 frequency; // in kHz
} DEMOD_CXD2834_CTX_T; 

static DEMOD_CXD2834_CTX_T *pSonyDemod;


//-----------------------------------------------------------------------------
// global variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
/***********************************************************************/
/*              External declarations                                  */
/***********************************************************************/




/***********************************************************************/
/*              Private (static) Function Prototypes                   */
/***********************************************************************/
extern  UINT8       u1DbgLevel;
//change DbgLevel,   because default DbgLevel is 0,nothing log for debug. use this for important LOG ,such as abort!Demod GPIO rest
VOID vEnableMTKLog(UINT8 fg)
{
static	UINT8  CurrDbgLevel=0,fgGetDbgLeveled=0;

	if(fg){
		if(0==fgGetDbgLeveled)
			{fgGetDbgLeveled=1;//already Get it
			CurrDbgLevel=u1DbgLevel;
			}
		u1DbgLevel=2;
		}
	else{	
		fgGetDbgLeveled=0;
		u1DbgLevel=CurrDbgLevel;//recover DbgLevel
		}
}




STATIC VOID vResetDemod(UINT16 u2DelayTime)
{

		 UINT32 u4RstPinNum=201;
		 GPIO_SetOut(208, 1);
		 mcDBG_MSG_L2(("Patch :208->1\n"));
			 // Config GPIO to reset demod
			 mcDBG_MSG_L2(("vResetDemod\n"));
			 if (DRVCUST_OptQuery(eDemodResetGpio, &u4RstPinNum) == 0)
			 {
				 GPIO_SetOut(u4RstPinNum, 0);
				 x_thread_delay((UINT32)u2DelayTime);
				 GPIO_SetOut(u4RstPinNum, 1);
				 x_thread_delay((UINT32)20); // Delay 20 ms for CR[DTV00030459]. Or it will init fail in rel image
				 mcDBG_MSG_L2(("Reset CXD2834 Demod GPIO=0x%x, 0->1, %dms\r\n",u4RstPinNum, u2DelayTime));
			 }



}

STATIC BOOL sony_GetTsFmt(sony_dvb_demod_t *pDemod, TS_FMT_T *pTsfmt)
{
    sony_result_t sony_result = SONY_RESULT_OK;
    UINT8 data = 0;

    // Set Bank 0
    sony_result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x00, 0x00);
    if(sony_result != SONY_RESULT_OK){ return 0; }

    // OSERIALEN (Bank:00h, Addr:F3h, Bit[7])
    sony_result = pDemod->pI2c->ReadRegister(pDemod->pI2c, pDemod->i2cAddress, 0xF3, &data, 1);
    if(sony_result != SONY_RESULT_OK){ return 0; }
    pTsfmt->SerOrPar = (data & 0x80) ? SP_SERIAL : SP_PARALLEL;
    // OWFMT_CKDISBYBGAP (Bank:00h, Addr:F3h, Bit[2])
    pTsfmt->fgGatedClock = (data & 0x04) ? BOOL_TRUE : BOOL_FALSE;

    // OWFMT_CKINV (Bank:00h, Addr:F4h, Bit[0])
    sony_result = pDemod->pI2c->ReadRegister(pDemod->pI2c, pDemod->i2cAddress, 0xF4, &data, 1);
    if(sony_result != SONY_RESULT_OK){ return 0; }
    pTsfmt->ClockPolarity = (data & 0x01) ? POL_HIGH_ACTIVE : POL_LOW_ACTIVE;

    // TS Output Enable (Bank:00h, Addr:F1h, Bit[0])
    sony_result = pDemod->pI2c->ReadRegister(pDemod->pI2c, pDemod->i2cAddress, 0xF1, &data, 1);
    if(sony_result != SONY_RESULT_OK){ return 0; }
    pTsfmt->fgTristate = (data == 0x01) ? BOOL_FALSE : BOOL_TRUE ;

    pTsfmt->u4TsClockInKHz = 82000; // Set 82MHz fixed value.
    pTsfmt->fgExternalDemod = BOOL_TRUE;

    return 1;
}

// This function is used in DtdCommand function
STATIC UINT32 sony_strtoul(const char* str)
{
    const char* ptr = str;
    UINT32 value = 0;

    if(!str){
        return 0;
    }
    if((x_strlen(str) > 2) && (str[0] == '0') && ((str[1] == 'x') || (str[1] == 'X'))){
        // Hex
        for(ptr = &(str[2]); *ptr; ptr++){
            if(('0' <= *ptr) && (*ptr <= '9')){
                value <<= 4; value += (UINT32)(*ptr - '0');
            }else if(('a' <= *ptr) && (*ptr <= 'f')){
                value <<= 4; value += (UINT32)(*ptr - 'a' + 10); 
            }else if(('A' <= *ptr) && (*ptr <= 'F')){
                value <<= 4; value += (UINT32)(*ptr - 'A' + 10);
            }else{
                break;
            }
        }
    }else{
        // Decimal
        for(ptr = str; *ptr; ptr++){
            if(('0' <= *ptr) && (*ptr <= '9')){
                value *= 10; value += (UINT32)(*ptr - '0');
            }else{
                break;
            }
        }
    }
    
    return value;
}

//-------------------------------------------------------------------------
/**
 *  This API implement close tuner. It should performs
 *  1. free all allocated resource.
 *  2. do demod/tuner initialization.
 *  3. do LTDIS API mapping
 *  @param   ptTDSpecificCtx   Pointer to tuner driver context.
 *  @retval  DRVAPI_TUNER_OK    Success
 *  @retval  DRVAPI_TUNER_ERROR Fail
 */
//-------------------------------------------------------------------------
INT32 DVBT2_TunerClose(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    sony_dvb_demod_t *pDemod = &pSonyDemod->demod;

    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);

    mcDBG_MSG_L2(("%s \n", __FUNCTION__));
    
    sony_dvb_demod_Sleep(pDemod);

    x_sema_delete(pPdCtxDVBT2->hHalLock);
    x_mem_free(pPdCtxDVBT2);
    x_mem_free(pSonyDemod);

    return (DRVAPI_TUNER_OK);
}

//-------------------------------------------------------------------------
/**
 *  This API do channel acuqusition.
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context
 *  @param   e_conn_type        tuner connection type, refer x_tuner.h
 *  @param   pv_conn_info       middleware tuner info structure, refer u_tuner.h
 *  @param   z_conn_info_len    length of pv_conn_info
 *  @param   _BreakFct          TunerHAL callback API for Vendor_TunerAcq to check whether need to break TunerAcq
 *  @param   *pvArg             pass this parameter to _BreakFct when calling _BreakFct
 *  @retval  1                  Demod Lock
 *  @retval  0                  Demod Unlock
 */
//-------------------------------------------------------------------------
// This function is used to wait lock.
// Called from TunerAcq.
STATIC BOOL sony_WaitLockLoop(sony_dvb_demod_t *pDemod, UINT32 timeout, BOOL isWaitDemodLock, x_break_fct break_fct, void* pvArg)
{
    sony_result_t sony_result = SONY_RESULT_OK;
	sony_dvb_demod_lock_result_t lockStatus = DEMOD_LOCK_RESULT_NOTDETECT;
    UINT32 cnt = 0;

    for(cnt=0; cnt < timeout/WAIT_LOCK_INTERVAL; cnt++)
    {
        if(break_fct(pvArg)){
            mcDBG_MSG_L1(("CXD2834 Wait Lock Suspended.\n"));
            return FALSE;
        }

        if(isWaitDemodLock){ // Wait Demod Lock or TS Lock
            sony_result = sony_dvb_demod_CheckDemodLock(pDemod, &lockStatus);
        }else{
            sony_result = sony_dvb_demod_CheckTSLock(pDemod, &lockStatus);
        }
        
        if(sony_result != SONY_RESULT_OK){
            mcDBG_MSG_L1(("CXD2834 Wait Lock Error.\n"));
            return FALSE;
        }

        switch(lockStatus){
        case DEMOD_LOCK_RESULT_LOCKED:
            if(isWaitDemodLock){
                mcDBG_MSG_L1(("CXD2834 Demod Locked.\n"));
            }else{
                mcDBG_MSG_L1(("CXD2834 TS Locked.\n"));
            }
            return TRUE;
        case DEMOD_LOCK_RESULT_UNLOCKED:
            mcDBG_MSG_L1(("CXD2834 Unlocked.\n"));
            return FALSE;
        case DEMOD_LOCK_RESULT_NOTDETECT:
            // Continue wait
            break;
        default:
            mcDBG_MSG_L1(("CXD2834 Wait Lock Error.\n"));
            return FALSE;
        }

        x_thread_delay((UINT32)WAIT_LOCK_INTERVAL); // wait 50ms
    }

    mcDBG_MSG_L1(("CXD2834 Wait Lock Timeout.\n"));
    return FALSE; // Timeout
}

// CXD2834 tuning sequence.
// Called from TunerAcq.
STATIC BOOL sony_TuneSequence(sony_dvb_demod_t *pDemod, sony_dvb_tune_params_t *pTuneParam, x_break_fct _BreakFct, void* pvArg)
{
    sony_result_t sony_result = SONY_RESULT_OK;
    BOOL fgLock = 0;
    UINT32 cnt = 0;

    // ------------------------ CXD2834 Tune Sequence --------------------------
    // Note: This part may be changed in future version
    // -------------------------------------------------------------------------
    // Start tuning now.
    mcDBG_MSG_L2(("Start to Tuning.\n"));

    // Prepare Tuning
    sony_result = sony_dvb_demod_TuneInitialize(pDemod, pTuneParam);
    if(sony_result != SONY_RESULT_OK){ goto ErrorExit; }

    // Prepare Acquisition
    sony_result = sony_dvb_demod_Tune(pDemod, pTuneParam->system, pTuneParam->bwMHz);
    if(sony_result != SONY_RESULT_OK){ goto ErrorExit; }

    mcDBG_MSG_L2(("Start to tune Tuner.\n"));

    // RF Tuner Tune
    {
        PARAM_SETFREQ_T param;
        UINT8 sawbw = 0;
        
        param.Freq = pTuneParam->centreFreqkHz;
        switch(pTuneParam->system)
        {
        case SONY_DVB_SYSTEM_DVBT:
        default:
            param.Modulation = MOD_DVBT;
            break;
        case SONY_DVB_SYSTEM_DVBT2:
            param.Modulation = MOD_DVBT2;
            break;
        case SONY_DVB_SYSTEM_DVBC:
            param.Modulation = MOD_DVBC;
            break;
        }
        
        // Set Bandwidth
        switch(pTuneParam->bwMHz){
        case 6:
            sawbw = SAW_BW_6M; break;
        case 7:
            sawbw = SAW_BW_7M; break;
        case 8:
        default:
        	  sawbw = SAW_BW_8M; break;
        }
        ITuner_OP(ITunerGetCtx(), itSetSawBw, sawbw, NULL);
        
        // Set Tuner PLL
        if(ITuner_SetFreq(ITunerGetCtx(), &param))
        {
            mcDBG_MSG_ERR(("Set Freq fail\n"));
            goto ErrorExit;
        }
        
        mcDBG_MSG_L2(("RF Tuner SetFreq Ended.\n"));
        mcDBG_MSG_L2(("Frequency = %d\n", param.Freq));
        mcDBG_MSG_L2(("Modulation = %d.\n", param.Modulation));
        mcDBG_MSG_L2(("Bandwidth = %d.\n", sawbw));
    }

    // Wait for tuner AGC stable
    for(cnt = 0; cnt < WAIT_TUNER_AGC_STABLE/WAIT_LOCK_INTERVAL; cnt++){
        if(_BreakFct(pvArg)){
            // Break
            goto ErrorExit;
        }
        
        x_thread_delay((UINT32)WAIT_LOCK_INTERVAL); // wait 50ms
    }

    // Demod Soft Reset
    sony_result = sony_dvb_demod_TuneEnd(pDemod);
    if(sony_result != SONY_RESULT_OK){ goto ErrorExit; }

    // Wait Demod Lock First (Only for DVB-T2)
    if(pTuneParam->system == SONY_DVB_SYSTEM_DVBT2){
        if(!sony_WaitLockLoop(pDemod, WAIT_DEMOD_LOCK_TIMEOUT_DVBT2, 1, _BreakFct, pvArg)){
            // Unlock or Error or Break
            goto SuccessExit;
        }

        // T2 optimization
        sony_result = sony_dvb_demodT2_OptimizeMISO(pDemod);
        if(sony_result != SONY_RESULT_OK){ goto ErrorExit; }
    }

    // Wait TS Lock
    {
        UINT32 timeout = 0;
        switch(pTuneParam->system)
        {
        case SONY_DVB_SYSTEM_DVBT:
        default:
            timeout = WAIT_TS_LOCK_TIMEOUT_DVBT;
            break;
        case SONY_DVB_SYSTEM_DVBT2:
            timeout = WAIT_TS_LOCK_TIMEOUT_DVBT2;
            break;
        case SONY_DVB_SYSTEM_DVBC:
            timeout = WAIT_TS_LOCK_TIMEOUT_DVBC;
            break;
        }
        
        if(sony_WaitLockLoop(pDemod, timeout, 0, _BreakFct, pvArg)){
            fgLock = TRUE;
        }else{
            fgLock = FALSE;
        }
    }

ErrorExit:
SuccessExit:
    // End of Tuning
    sony_dvb_demod_TuneFinalize(pDemod, pTuneParam,
        fgLock ? SONY_RESULT_OK : SONY_RESULT_ERROR_UNLOCK);

    // Check PLP Detection Error
    if((pTuneParam->system == SONY_DVB_SYSTEM_DVBT2)
        && (!pSonyDemod->autoPLP) && fgLock)
    {
        // Auto PLP detection is disabled.
        // If PLP error occured, return unlock result.
        if(pTuneParam->t2Params.tuneResult & SONY_DVBT2_TUNE_RESULT_DATA_PLP_NOT_FOUND)
        {
            mcDBG_MSG_L1(("CXD2834 PLP Not Found Error.\n"));
            fgLock = 0;
        }
    }
    mcDBG_MSG_L1(("CXD2834 PLP %d, fgLock%d\n",pSonyDemod->autoPLP,fgLock));
    return fgLock;
}

BOOL DVBT2_TunerAcq(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type, void* pv_conn_info,
                            SIZE_T z_conn_info_len, x_break_fct _BreakFct, void* pvArg)
{
    TUNER_TER_DIG_TUNE_INFO_T *pt_tuner_info = 
                    (TUNER_TER_DIG_TUNE_INFO_T *) pv_conn_info;
    //UINT8   fgChannelScan = FALSE;
    BOOL fgLock = 0;
    UINT32  u4TickCnt, u4ConnTime;

    // CXD2834 driver specific
    sony_result_t sony_result = SONY_RESULT_OK;
    sony_dvb_demod_t *pDemod = &pSonyDemod->demod;
    sony_dvb_tune_params_t sonyTuneParam;

    //MUTEX lock
    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);

    //Set channel scan indicator
    if(pt_tuner_info->e_mod == MOD_UNKNOWN){ 
       // fgChannelScan = TRUE;
    }

    u4TickCnt = x_os_get_sys_tick();

    // Set sonyTuneParam
    // Frequency
    sonyTuneParam.centreFreqkHz = pt_tuner_info->ui4_freq/1000;
    pSonyDemod->frequency = sonyTuneParam.centreFreqkHz;

    // BandWidth
    switch(pt_tuner_info->e_bandwidth){
    case BW_6_MHz:
        sonyTuneParam.bwMHz = 6; break;
    case BW_7_MHz:
        sonyTuneParam.bwMHz = 7; break;
    case BW_8_MHz:
    default:
        sonyTuneParam.bwMHz = 8; break;
    }

    if(e_conn_type == TUNER_CONN_TYPE_CAB_DIG){ // DVB-C
        // DVB-C Scan Mode setting
        sony_result = sony_dvb_demod_SetConfig(pDemod, DEMOD_CONFIG_DVBC_SCANMODE, pt_tuner_info->e_conn_mode ? 1 : 0);
        if(sony_result != SONY_RESULT_OK){
            mcDBG_MSG_L2(("SetConfig Error.\n"));
            goto ErrorExit;
        }
        // Parameter setting
        sonyTuneParam.system = SONY_DVB_SYSTEM_DVBC;
        sonyTuneParam.bwMHz = 8;
        // Go to tune sequence
        fgLock = sony_TuneSequence(pDemod, &sonyTuneParam, _BreakFct, pvArg);
    }else if(e_conn_type == TUNER_CONN_TYPE_TER_DIG){ // DVB-T/T2
        // Parameter setting (Both DVB-T and T2)
        if (pt_tuner_info->e_hierarchy_priority == DVBT_HIR_PRIORITY_LOW) {
            sonyTuneParam.tParams.profile = SONY_DVBT_PROFILE_LP;
        } else {
            sonyTuneParam.tParams.profile = SONY_DVBT_PROFILE_HP;
        }
        sonyTuneParam.tParams.usePresets = 0;
        sonyTuneParam.t2Params.dataPLPId = pt_tuner_info->ui1_plp_id;
        sonyTuneParam.t2Params.tuneResult = 0;

#ifdef MW_DVBT2_MPLP_SUPPORT
				
				   mcSHOW_DBG_MSG(("MW_DVBT2_MPLP_SUPPORT------T2 seperate tuning Set AAplpID= %d\n",sonyTuneParam.t2Params.dataPLPId));
		#endif
		mcSHOW_DBG_MSG(("MW_DVBT2_MPLP_SUPPORT------T2 seperate tuning Set plpID= %d\n",pt_tuner_info->ui1_plp_id));
        // pt_tuner_info->e_mod was used in legacy code. (MOD_DVBT/MOD_DVBT2)
        switch(pt_tuner_info->e_dvb_t_t2_mode){
        case TUNER_DVB_T:
            sonyTuneParam.system = SONY_DVB_SYSTEM_DVBT;
            fgLock = sony_TuneSequence(pDemod, &sonyTuneParam, _BreakFct, pvArg);
            break;
        case TUNER_DVB_T2:
            sonyTuneParam.system = SONY_DVB_SYSTEM_DVBT2;
            fgLock = sony_TuneSequence(pDemod, &sonyTuneParam, _BreakFct, pvArg);
            break;
        case TUNER_DVB_AUTO_T_T2: // Try both DVB-T and T2.
            // Try DVB-T
            mcDBG_MSG_L2(("Try DVB-T\n"));
            sonyTuneParam.system = SONY_DVB_SYSTEM_DVBT;
            fgLock = sony_TuneSequence(pDemod, &sonyTuneParam, _BreakFct, pvArg);
            if(fgLock){ break; } // DVB-T Locked! Stop here.
            mcDBG_MSG_L2(("Try DVB-T2\n"));
            // Try DVB-T2
            sonyTuneParam.system = SONY_DVB_SYSTEM_DVBT2;
            fgLock = sony_TuneSequence(pDemod, &sonyTuneParam, _BreakFct, pvArg);
            break;
        default:
            mcSHOW_USER_MSG(("Error T T2 MODE = %d!", pt_tuner_info->e_dvb_t_t2_mode));
            // Try DVB-T
            mcDBG_MSG_L2(("Try DVB-T\n"));
            sonyTuneParam.system = SONY_DVB_SYSTEM_DVBT;
            fgLock = sony_TuneSequence(pDemod, &sonyTuneParam, _BreakFct, pvArg);
            if(fgLock){ break; } // DVB-T Locked! Stop here.
            mcDBG_MSG_L2(("Try DVB-T2\n"));
            // Try DVB-T2
            sonyTuneParam.system = SONY_DVB_SYSTEM_DVBT2;
            fgLock = sony_TuneSequence(pDemod, &sonyTuneParam, _BreakFct, pvArg);
            break;
        }
    }else{
        mcSHOW_USER_MSG(("Error TUNER CONN TYPE = %d!",e_conn_type));
        goto ErrorExit;
    }

    mcSHOW_DRVAPI_MSG(("Freq = %d\n", sonyTuneParam.centreFreqkHz));
    mcSHOW_DRVAPI_MSG(("system = %d\n", pDemod->system));
    mcSHOW_DRVAPI_MSG(("Bandwidth = %d\n", pDemod->bandWidth));

    //Check tuner conncection time
    u4ConnTime = (x_os_get_sys_tick() - u4TickCnt) * x_os_get_sys_tick_period();
    mcSHOW_DBG_MSG(("DVBT2 acquisition time = %dms\n", u4ConnTime));

ErrorExit:
    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);

    /* Init Disconnect Status */
    pPdCtxDVBT2->fgDisStatus = FALSE;   

    return fgLock; //if lock return 1, unlock return 0
}
//-------------------------------------------------------------------------
/**
 *  This API check lock status
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context.
 *  @param   e_conn_type        tuner connection type, refer x_tuner.h
 *  @retval  1                  LOCK
 *  @retval  0                  UNLOCK
 */
//-------------------------------------------------------------------------
#ifdef  INTERNAL_3RD_DEMOD
INT16 DVBT2_TunerGetSync(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
#else
BOOL DVBT2_TunerGetSync(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
#endif
{
    UINT16 u2Ret = 0;
    sony_result_t sony_result = SONY_RESULT_OK;
    sony_dvb_demod_t *pDemod = &pSonyDemod->demod;
    sony_dvb_demod_lock_result_t lockStatus = DEMOD_LOCK_RESULT_NOTDETECT;

    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);

    mcDBG_MSG_L4(("%s \n", __FUNCTION__));

    sony_result = sony_dvb_demod_CheckTSLock(pDemod, &lockStatus);
    if(sony_result == SONY_RESULT_OK){
        if(lockStatus == DEMOD_LOCK_RESULT_LOCKED){
            u2Ret = 1;
        }else{
            u2Ret = 0;
        }
    }else{
        u2Ret = 0;
    }

    // Check PLP Detection Error
    if((pDemod->system == SONY_DVB_SYSTEM_DVBT2)
        && (!pSonyDemod->autoPLP) && u2Ret)
    {
        UINT8 plpError = 0;
		    mcDBG_MSG_L4(("sync autoPLP\n"));
        sony_result = sony_dvb_demod_monitorT2_DataPLPError(pDemod, &plpError);
        if(sony_result != SONY_RESULT_OK){
            u2Ret = 0;
        }
        if(plpError){
            u2Ret = 0;
        }
    }
    mcDBG_MSG_L4(("sync %d -%d\n",u2Ret,pSonyDemod->autoPLP));
    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
	#ifdef  INTERNAL_3RD_DEMOD
    return u2Ret;
	#else
	return (BOOL)u2Ret;
	#endif
}

//-------------------------------------------------------------------------
/**
 *  This API do disconnect tuner
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context.
 *  @param   e_conn_type        tuner connection type, refer x_tuner.h
 *  @retval  void
 */
//-------------------------------------------------------------------------
VOID  DVBT2_TunerDisc(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    sony_dvb_demod_t *pDemod = &pSonyDemod->demod;

    if(pPdCtxDVBT2->fgDisStatus == TRUE)
    {
        /* Has Disconnected, return */
        mcSHOW_DBG_MSG(("DVBT2 Has Disconnect!\n"));
        return;
    }
    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);

    mcDBG_MSG_L2(("%s \n", __FUNCTION__));

    sony_dvb_demod_Sleep(pDemod);

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);

    /* set disconnect status */
    pPdCtxDVBT2->fgDisStatus = TRUE;
}



/*<<<<<<<<< Need to do further implementation <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
void DVBT2_TunerBypassI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx, BOOL bSwitchOn)
{
    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);


    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
}

//-------------------------------------------------------------------------
/**
 *  This API get modulation information.
 *  Inputs:
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context.
 *  @param   e_conn_type        tuner connection type, refer x_tuner.h
 *  Outputs:
 *  @param   *_pSignal          Modulation info. structure to be filled.
 *  @retval  void
 */
//-------------------------------------------------------------------------
void DVBT2_TunerGetSignal(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type, SIGNAL *_pSignal)
{
    sony_result_t sony_result = SONY_RESULT_OK;
    sony_dvb_demod_t *pDemod = &pSonyDemod->demod;
	_pSignal->u1DVBT_T2_indicator = TUNER_DVB_AUTO_T_T2;

    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);

    mcDBG_MSG_L2(("%s \n", __FUNCTION__));
    
    //add driver implementation code here
    x_memset(_pSignal, 0, sizeof(SIGNAL));

    mcSHOW_DBG_MSG(("system  = %d (0:None, 1:DVBC, 2:DVBT, 3:DVBT2)\n", pDemod->system));

    // Frequency -----------------------------------------------------
    _pSignal->Frequency = pSonyDemod->frequency;

    // BandWidth -----------------------------------------------------
    switch(pDemod->bandWidth){
    //case 5:
    //    _pSignal->e_bandwidth = BW_5_MHz; break;
    case 6:
        _pSignal->e_bandwidth = BW_6_MHz; break;
    case 7:
        _pSignal->e_bandwidth = BW_7_MHz; break;
    case 8:
        _pSignal->e_bandwidth = BW_8_MHz; break;
    default:
        _pSignal->e_bandwidth = BW_UNKNOWN; break;
    }

    // SymbolRate ----------------------------------------------------
    if(pDemod->system == SONY_DVB_SYSTEM_DVBC){
        UINT32 symRate = 0;
        sony_result = sony_dvb_demod_monitorC_SymbolRate(pDemod, &symRate);
        if(sony_result == SONY_RESULT_OK){
            _pSignal->SymbolRate = symRate * 1000;
        }else{
            _pSignal->SymbolRate = 0;
        }

        // DVB-C bandwidth is 8MHz
        _pSignal->e_bandwidth = BW_8_MHz;
    }

    // QAMSize / Hierarchy / PLP -----------------------------------------------
    if(pDemod->system == SONY_DVB_SYSTEM_DVBT){
        sony_dvbt_tpsinfo_t tpsInfo;
		_pSignal->u1DVBT_T2_indicator = TUNER_DVB_T ;
        sony_result = sony_dvb_demod_monitorT_TPSInfo(pDemod, &tpsInfo);
        if(sony_result == SONY_RESULT_OK){
            switch(tpsInfo.constellation){
            case SONY_DVBT_CONSTELLATION_QPSK:
                _pSignal->QAMSize = (UINT32)MOD_QPSK; break;
            case SONY_DVBT_CONSTELLATION_16QAM:
                _pSignal->QAMSize = (UINT32)MOD_QAM_16; break;
            case SONY_DVBT_CONSTELLATION_64QAM:
                _pSignal->QAMSize = (UINT32)MOD_QAM_64; break;
            default:
                _pSignal->QAMSize = (UINT32)MOD_UNKNOWN; break;
            }
            
            switch(tpsInfo.hierarchy){
            case SONY_DVBT_HIERARCHY_NON:
            default:
                _pSignal->sHierInfo.eAlpha = ALPHA_0; break;
            case SONY_DVBT_HIERARCHY_1:
                _pSignal->sHierInfo.eAlpha = ALPHA_1; break;
            case SONY_DVBT_HIERARCHY_2:
                _pSignal->sHierInfo.eAlpha = ALPHA_2; break;
            case SONY_DVBT_HIERARCHY_4:
                _pSignal->sHierInfo.eAlpha = ALPHA_4; break;
            }

            if (tpsInfo.hierarchy == SONY_DVBT_HIERARCHY_NON) {
                _pSignal->e_hierarchy_priority = DVBT_HIR_PRIORITY_NONE;
            } else {
                uint8_t data = 0;
                /* Set bank 0 */
                pDemod->pI2c->WriteOneRegister (pDemod->pI2c, pDemod->i2cAddress, 0x00, 0x00);
                pDemod->pI2c->ReadRegister (pDemod->pI2c, pDemod->i2cAddress, 0x67, &data, 1);
                _pSignal->e_hierarchy_priority = ((data & 0x01) == 0x01) ? DVBT_HIR_PRIORITY_LOW : DVBT_HIR_PRIORITY_HIGH;
            }
        }else{
            _pSignal->QAMSize = (UINT32)MOD_UNKNOWN;
            _pSignal->sHierInfo.eAlpha = ALPHA_0;
            _pSignal->e_hierarchy_priority = DVBT_HIR_PRIORITY_UNKNOWN;
        }
    }else if(pDemod->system == SONY_DVB_SYSTEM_DVBT2){
        sony_dvbt2_plp_t plpInfo;
        sony_result = sony_dvb_demod_monitorT2_ActivePLP(pDemod, SONY_DVBT2_PLP_DATA, &plpInfo);
        if(sony_result == SONY_RESULT_OK){
            switch(plpInfo.constell){
            case SONY_DVBT2_QPSK:
                _pSignal->QAMSize = (UINT32)MOD_QPSK; break;
            case SONY_DVBT2_QAM16:
                _pSignal->QAMSize = (UINT32)MOD_QAM_16; break;
            case SONY_DVBT2_QAM64:
                _pSignal->QAMSize = (UINT32)MOD_QAM_64; break;
            case SONY_DVBT2_QAM256:
                _pSignal->QAMSize = (UINT32)MOD_QAM_256; break;
            default:
                _pSignal->QAMSize = (UINT32)MOD_UNKNOWN; break;
            }
            _pSignal->ui1_plp_id = plpInfo.id;
			_pSignal->u1DVBT_T2_indicator = TUNER_DVB_T2 ;
		mcSHOW_DBG_MSG(("AA---Get signal :SONY_GetPlpInfo = %d\n", _pSignal->ui1_plp_id ));
        }else{
            _pSignal->QAMSize = (UINT32)MOD_UNKNOWN;
            _pSignal->ui1_plp_id = 0;
        }
    }else if(pDemod->system == SONY_DVB_SYSTEM_DVBC){
        sony_dvbc_constellation_t constellation = SONY_DVBC_CONSTELLATION_16QAM;
        sony_result = sony_dvb_demod_monitorC_QAM(pDemod, &constellation);
        if(sony_result == SONY_RESULT_OK){
            switch(constellation){
            case SONY_DVBC_CONSTELLATION_16QAM:
                _pSignal->QAMSize = (UINT32)MOD_QAM_16; break;
            case SONY_DVBC_CONSTELLATION_32QAM:
                _pSignal->QAMSize = (UINT32)MOD_QAM_32; break;
            case SONY_DVBC_CONSTELLATION_64QAM:
                _pSignal->QAMSize = (UINT32)MOD_QAM_64; break;
            case SONY_DVBC_CONSTELLATION_128QAM:
                _pSignal->QAMSize = (UINT32)MOD_QAM_128; break;
            case SONY_DVBC_CONSTELLATION_256QAM:
                _pSignal->QAMSize = (UINT32)MOD_QAM_256; break;
            default:
                _pSignal->QAMSize = (UINT32)MOD_UNKNOWN; break;
            }
        }else{
            _pSignal->QAMSize = (UINT32)MOD_UNKNOWN;
        }
    }else{
        _pSignal->QAMSize = (UINT32)MOD_UNKNOWN;
    }

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
}

//-------------------------------------------------------------------------
/**
 *  This API get signal level in terms of percentage.
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context.
 *  @param   e_conn_type        tuner connection type, refer x_tuner.h
 *  @retval  0~100              signal level
 */
//-------------------------------------------------------------------------
UINT8 DVBT2_TunerGetSignalLevel(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    UINT8 ssi = 0;
    sony_result_t sony_result = SONY_RESULT_OK;
    sony_dvb_demod_t *pDemod = &pSonyDemod->demod;

    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);

    mcDBG_MSG_L2(("%s \n", __FUNCTION__));

    sony_result = sony_dvb_demod_monitor_SSI(pDemod, &ssi);
    if(sony_result != SONY_RESULT_OK){
        ssi = 0;
    }
    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
    //example of return
    return ssi;
}

//-------------------------------------------------------------------------
/**
 *  This API get signal level in multiples of (0.1dBm).
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context.
 *  @param   e_conn_type        tuner connection type, refer x_tuner.h
 *  @retval  0 ~ -900           signal level
 */
//-------------------------------------------------------------------------
INT16 DVBT2_TunerGetSignalLeveldBm(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    INT16 i2Ret=0;
    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
    //add driver implementation code here
    mcSHOW_DRVERR_MSG(("No implementation in DVBT2_TunerGetSignalLeveldBm()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
    return i2Ret;
}

static UINT32 sony_GetBER(void)
{
    UINT32 ber = 100000; // unit is 1e-5
    sony_result_t sony_result = SONY_RESULT_OK;
    sony_dvb_demod_t *pDemod = &pSonyDemod->demod;
    
    /* unit is 1e5 */
    switch(pDemod->system)
    {
    case SONY_DVB_SYSTEM_DVBT:
        sony_result = sony_dvb_demod_monitorT_PreRSBER(pDemod, &ber); /* 1e7 */
		mcSHOW_DBG_MSG(("T BER\n"));
        if(sony_result == SONY_RESULT_OK){
            ber = (ber + 50) / 100;
        }else{
            ber = 100000;
        }
        break;
    case SONY_DVB_SYSTEM_DVBT2:
        sony_result = sony_dvb_demod_monitorT2_PreBCHBER(pDemod, &ber); /* 1e9 */
        if(sony_result == SONY_RESULT_OK){
            ber = (ber + 5000) / 10000;
			mcSHOW_DBG_MSG(("-G T2 BER\n"));
        }else{
            ber = 100000;
			mcSHOW_DBG_MSG(("-NG T2 BER\n"));
        }
        break;
    case SONY_DVB_SYSTEM_DVBC:
        sony_result = sony_dvb_demod_monitorC_PreRSBER(pDemod, &ber); /* 1e7 */
		mcSHOW_DBG_MSG(("C BER\n"));
        if(sony_result == SONY_RESULT_OK){
            ber = (ber + 50) / 100;
        }else{
            ber = 100000;
        }
        break;
    default:
        ber = 100000;
		mcSHOW_DBG_MSG(("NG def BER\n"));
        break;
    }

    return ber;
}

UINT32 DVBT2_TunerGetSignalBER(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    UINT32 ber = 0;

    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);

    ber = sony_GetBER();

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);

    return ber;
}

#ifdef  INTERNAL_3RD_DEMOD
UINT16 DVBT2_TunerGetSignalPER(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
#else
UINT32 DVBT2_TunerGetSignalPER(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
#endif
{
    UINT32 u4_ret = 100000; // unit is 1e-5
    sony_result_t sony_result = SONY_RESULT_OK;
    sony_dvb_demod_t *pDemod = &pSonyDemod->demod;

    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);

    // PER unit is 1e-5
    sony_result = sony_dvb_demod_monitor_PER(pDemod, &u4_ret);
    if(sony_result != SONY_RESULT_OK){
        u4_ret = 100000; // Error Rate = 100%
    }else{
        // NOTE: sony_dvb_demod_monitor_PER returns 1e6 unit value.
        u4_ret = (u4_ret + 5) / 10;
    }

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
#ifdef  INTERNAL_3RD_DEMOD

    return (UINT16)u4_ret;//modify return value
#else
	return (UINT32)u4_ret;//modify return value

#endif
}

UINT16 DVBT2_TunerGetSignalSNR(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    UINT16 u2_ret = 0;
    INT32 snr = 0;
    sony_dvb_demod_t *pDemod = &pSonyDemod->demod;
    sony_result_t sony_result = SONY_RESULT_OK;

    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);

    // NOTE: sony_dvb_demod_monitorX_SNR returns dB * 1000.
    switch(pDemod->system){
    case SONY_DVB_SYSTEM_DVBT:
        sony_result = sony_dvb_demod_monitorT_SNR(pDemod, &snr);
        break;
    case SONY_DVB_SYSTEM_DVBT2:
        sony_result = sony_dvb_demod_monitorT2_SNR(pDemod, &snr);
        break;
    case SONY_DVB_SYSTEM_DVBC:
        sony_result = sony_dvb_demod_monitorC_SNR(pDemod, &snr);
        break;
    default:
        snr = 0;
        break;
    }
    
    if(sony_result != SONY_RESULT_OK){
        snr = 0;
    }

    u2_ret = (UINT16)(snr + 500)/1000; // rounding

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);

    return u2_ret;
}

void DVBT2_TunerSetMpgFmt(PTD_SPECIFIC_CTX ptTDSpecificCtx, MPEG_FMT_T  *pt_mpeg_fmt)
{
    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
	//add your implementation here
    mcSHOW_DRVERR_MSG(("No implementation in DVBT2_TunerSetMpgFmt()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
}

char *DVBT2_TunerGetVer(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    static  char * version = "SONY CXD2834 Demod driver version\n";

    return version;
}

INT32 DVBT2_TunerNimTest(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    INT32 i4Ret = 0;
    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);

	//add your implementation here
    
   mcSHOW_DRVERR_MSG(("No implementation in DVBT2_TunerNimTest()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
    return i4Ret;
}
void DVBT2_TunerSetRegSetting(PTD_SPECIFIC_CTX ptTDSpecificCtx, UCHAR ucRegSet, UCHAR ucRegAddr, UCHAR ucRegValue)
{
    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
	//add your implementation here
    mcSHOW_DRVERR_MSG(("No implementation in DVBT2_TunerSetRegSetting()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
}

void DVBT2_TunerShowRegSetting(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
	//add your implementation here
    mcSHOW_DRVERR_MSG(("No implementation in DVBT2_TunerShowRegSetting()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
}

void DVBT2_TunerTestI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
	//add your implementation here
    mcSHOW_DRVERR_MSG(("No implementation in DVBT2_TunerTestI2C()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
}




void DVBT2_TunerCommand(PTD_SPECIFIC_CTX ptTDSpecificCtx, INT32 i4Argc, const CHAR** aszArgv)
{

    //UCHAR   ucCmdSize = 0;

    
	mcSHOW_DBG_MSG4(("DVBT2_TunerCommand pPdCtx->hHalLock\n"));
    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
	mcSHOW_DBG_MSG4(("escape DVBT2_TunerCommand pPdCtx->hHalLock\n"));
    	    
   

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
    mcSHOW_DBG_MSG4(("DVBT2_TunerCommand unlock pPdCtx->hHalLock\n"));
}

 INT32     T2test(void *pArg)
 	{
 	return 0;
 	};


void DVBT2_TunerDVBT2Command(PTD_SPECIFIC_CTX ptTDSpecificCtx,INT32 i4Argc, const CHAR** aszArgv)
{
    sony_result_t sony_result=SONY_RESULT_OK;
    sony_dvb_demod_t *pDemod = &pSonyDemod->demod;

    //TODO you can add command for debug use here
   
    UCHAR ucCmdId = 0;
    if (i4Argc < 1)
    {
    	mcDBG_MSG_L1(("\t c                    MTK Show checklist info\n"));  
		mcDBG_MSG_L1(("\t g [freqKhz] [plpid]  [bw](0=6M,1=7M,2=8M)  MTK CLI lock T2 signal\n"));  
		mcDBG_MSG_L1(("\t s                    Show signal/ts/tps status\n"));          
        mcDBG_MSG_L1(("\t d [Bank]             Demod Register Page Dump Result\n")); 
        mcDBG_MSG_L1(("\t r [RegAddr] [Num]    Read  Demod Register\n"));
        mcDBG_MSG_L1(("\t w [RegAddr] [Value]  Write Demod Register\n"));
        mcDBG_MSG_L1(("\t f [Freq]             Set Freq (kHz)\n"));
        mcDBG_MSG_L1(("\t t                    Set TS interface parrel(1) /serial(0)\n"));
        mcDBG_MSG_L1(("\t l                    Make sony demod sleep\n"));
    }   
    mcDBG_MSG_L2((" %s \n",__FUNCTION__));

    if (i4Argc > 0)
        ucCmdId = *((CHAR *) aszArgv[0]);
        
    switch (ucCmdId)
    {
    case 'c':{
		/*********NO1 .check Demod rest config*********/
		UINT32 u4RstPinNum=0xFFFF;
		 if ( 0== DRVCUST_OptQuery(eDemodResetGpio, &u4RstPinNum))
		 	{
		 	mcDBG_MSG_L2(("DRVCUST DEMOD rest GPIO=%d\n",u4RstPinNum));
		 	}
		 else
		 	{mcDBG_MSG_L2(("!!!No config DRVCUST DEMOD rest GPIO=%d\n",u4RstPinNum));}
    	/*********NO2 .check Demod&Tuner I2C  busID config*********/
		UINT32 u4DemodBusId=0xFFFF;
		if (0==DRVCUST_OptQuery(eDemodI2cBusID,&u4DemodBusId))
			{
			mcDBG_MSG_L2(("Demod  I2C  busID =%d\n",u4DemodBusId));
			}
		else 
			{mcDBG_MSG_L2(("!!!No config Demod I2C  busID =%d\n",u4DemodBusId));}
		/*********NO3 .check Demod crystal Freq config*********/
			#ifdef SONY_DEMOD_CRYSTAL_41M
			mcDBG_MSG_L2(("SONY_DEMOD_CRYSTAL_41M\n"));
			#warning "SONY_DEMOD_CRYSTAL_41M"
			#else
			mcDBG_MSG_L2(("SONY_DEMOD_CRYSTAL_20.5M\n"));
			#warning "SONY_DEMOD_CRYSTAL_20.5M"
			#endif
		/*********NO4 .check Demod TS output mode config*********/
			#ifdef SONY_DEMOD_TSOUTPUT_SERIAL
			//	2 for TS output mode
			mcDBG_MSG_L2(("SONY_DEMOD_TSOUTPUT:  SERIAL\n"));
			#else
			mcDBG_MSG_L2(("SONY_DEMOD_TSOUTPUT:  PARAlLEL\n"));
			#endif
		/*********NO5 .check system  config*********/
			#ifdef __KERNEL__
			mcDBG_MSG_L2(("System config:  LINUX\n"));
			#else
			mcDBG_MSG_L2(("System config:  ncls\n"));
			#endif
			/*********NO6 .check internal tuner_driver  config*********/
			#ifdef INTERNAL_3RD_DEMOD
			// 3 for internal tuner_src use
			mcDBG_MSG_L2(("INTERNAL_3RD_DEMOD\n"));
			#else
			mcDBG_MSG_L2(("VDRDEMOD_3RD_DEMOD\n"));
			#endif
			/*********NO7 .check i2c Demod Gateway config*********/
			#ifdef SONY_DEMOD
			// 4 for sony_demod bypass i2c Gateway which use 0xd8 0x09 i2c data to tuner 
			mcDBG_MSG_L2(("SONY_DEMOD I2C Gateway\n"));
			#else
			mcDBG_MSG_L2(("I2C direct ACESS\n"));
			#endif

    	}

		break;
    case 'g':
	{//sony_result_t sony_result = SONY_RESULT_OK;
    sony_dvb_demod_t *pDemod = &pSonyDemod->demod;
    sony_dvb_tune_params_t sonyTuneParam;
	BOOL   fgLock = 0;
	UINT32 plpID =0;	// use default setting! 
	UINT32 freq = 666000;
	INT32  Bw = 0;       

	if (i4Argc >3)
       {
           freq = (UINT32)StrToInt(aszArgv[1]);	    
           plpID =(UINT8)StrToInt(aszArgv[2]);
	       Bw =(UINT8)StrToInt(aszArgv[3]);	   
		}

	
		sonyTuneParam.tParams.usePresets = 0;
        sonyTuneParam.t2Params.dataPLPId = plpID;
        sonyTuneParam.t2Params.tuneResult = 0;

			sonyTuneParam.centreFreqkHz = freq;
			pSonyDemod->frequency = sonyTuneParam.centreFreqkHz;
			
			// BandWidth
			switch(Bw){
			case  0://BW_6_MHz:
				sonyTuneParam.bwMHz = 6; break;
			case 1://BW_7_MHz:
				sonyTuneParam.bwMHz = 7; break;
			case 2://BW_8_MHz:
			default:
				sonyTuneParam.bwMHz = 8; break;
			}

		
		 	sonyTuneParam.system = SONY_DVB_SYSTEM_DVBT2;
            fgLock = sony_TuneSequence(pDemod, &sonyTuneParam, T2test, 0);

			if (fgLock == 1)
		 {
            pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x00, 0x00);
            pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0xF1,  0);  
		 		mcSHOW_USER_MSG(("Front-end lock\n"));			 
		  }
		   else
		   {
			
               mcSHOW_USER_MSG(("Front-end unlock %d\n",fgLock));
		   }	   
     	}
	break;
	  // added 20111125 for tuner dump/write/read
	    case 'a':
        {
            /* Tuner Dump/Read/Write via I2C Gateway */
            const UINT8 tunerI2cAddress = 0xC0; /* Please fix it if tuner address is changed. */
            sony_i2c_t i2cTuner;

            sony_i2c_MTK_CreateI2cGw(&i2cTuner, pDemod->i2cAddress, 0x09);

            switch(aszArgv[0][1]){
            case 'd':
            default:
                {
                    UINT8 data[256];
                    
                    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock); // Enter Critical Section
                    sony_result = i2cTuner.ReadRegister(&i2cTuner, tunerI2cAddress, 0x00, data, 256);
                    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock); // Leave Critical Section
                    if(sony_result != SONY_RESULT_OK){
                        mcDBG_MSG_L1(("I2C Error.\n"));
                        return;
                    }
                    mcDBG_MSG_L1(("Register Dump Result. (Tuner)\n"));
                    mcDBG_MSG_L1(("     00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n"));
                    {
                        int i = 0;
                        int j = 0;
                        char dumpline[53];
                        char* ptr = dumpline;
                        const char hexchar[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

                        dumpline[52] = '\0';
                        for(i=0; i<16; i++){
                            ptr = dumpline;
                            for(j=0; j<16; j++){
                                if(j == 0){
                                    *ptr++ = hexchar[(i*16) >> 4];
                                    *ptr++ = hexchar[(i*16) & 0x0F];
                                    *ptr++ = ' ';
                                    *ptr++ = '|';
                                }
                                *ptr++ = ' ';
                                *ptr++ = hexchar[data[(i*16)+j] >> 4];
                                *ptr++ = hexchar[data[(i*16)+j] & 0x0F];
                            }
                            mcDBG_MSG_L1(("%s\n", dumpline));
                        }
                    }
                    mcDBG_MSG_L1(("\n"));
                }
                break;
            case 'r':
                {
                    UINT8 data[256];
                    UINT8 subAddress = 0;
                    UINT32 length = 0;
                    UINT32 i = 0;
                    if(i4Argc < 3){
                        mcDBG_MSG_L1(("Error: No read register and length.\n"));
                        return;
                    }
                    subAddress = (UINT8)sony_strtoul(aszArgv[1]);
                    length = (UINT32)sony_strtoul(aszArgv[2]);
                    if(length > 256){
                        length = 256;
                    }
                    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
                    sony_result = i2cTuner.ReadRegister(&i2cTuner, tunerI2cAddress, subAddress, data, length);
                    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
                    if(sony_result != SONY_RESULT_OK){
                        mcDBG_MSG_L1(("I2C Error.\n"));
                        return;
                    }
                    for(i=0; i<length; i++){
                        mcDBG_MSG_L1(("Tuner Register 0x%02X = 0x%02X\n", i + subAddress, data[i]));
                    }
                }
                break;
            case 'w':
                {
                    UINT8 subAddress = 0;
                    UINT8 value = 0;
                    if(i4Argc < 3){
                        mcDBG_MSG_L1(("Error: No write register and data.\n"));
                        return;
                    }
                    subAddress = (UINT8)sony_strtoul(aszArgv[1]);
                    value = (UINT8)sony_strtoul(aszArgv[2]);
                    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
                    sony_result = i2cTuner.WriteOneRegister(&i2cTuner, tunerI2cAddress, subAddress, value);
                    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
                    if(sony_result != SONY_RESULT_OK){
                        mcDBG_MSG_L1(("I2C Error.\n"));
                        return;
                    }
                    mcDBG_MSG_L1(("Wrote Tuner Register 0x%02X = 0x%02X\n", subAddress, value));
                }
                break;
            }
            break;
        }  
	  // finished 20111125
    case 's':
        mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
        
        /* Demod Lock Status */
        if(pDemod->system == SONY_DVB_SYSTEM_DVBT){
            UINT8 syncStat = 0;
            UINT8 tsLock = 0;
            UINT8 unlock = 0;
            sony_result = sony_dvb_demod_monitorT_SyncStat(pDemod, &syncStat, &tsLock, &unlock);
            if(sony_result == SONY_RESULT_OK){
                mcDBG_MSG_L1(("Sync State : %d\n", syncStat));
                mcDBG_MSG_L1(("TS Lock    : %d\n", tsLock));
                mcDBG_MSG_L1(("Unlock Det : %d\n", unlock));
            }else{
                mcDBG_MSG_L1(("Error in monitorT_SyncStat\n"));
            }
        }else if(pDemod->system == SONY_DVB_SYSTEM_DVBT2){
            UINT8 syncStat = 0;
            UINT8 tsLock = 0;
            UINT8 unlock = 0;
            sony_result = sony_dvb_demod_monitorT2_SyncStat(pDemod, &syncStat, &tsLock, &unlock);
            if(sony_result == SONY_RESULT_OK){
                mcDBG_MSG_L1(("Sync State : %d\n", syncStat));
                mcDBG_MSG_L1(("TS Lock    : %d\n", tsLock));
                mcDBG_MSG_L1(("Unlock Det : %d\n", unlock));
            }else{
                mcDBG_MSG_L1(("Error in monitorT2_SyncStat\n"));
            }
        }else if(pDemod->system == SONY_DVB_SYSTEM_DVBC){
            UINT8 arLock = 0;
            UINT8 tsLock = 0;
            UINT8 unlock = 0;
            sony_result = sony_dvb_demod_monitorC_SyncStat(pDemod, &arLock, &tsLock, &unlock);
            if(sony_result == SONY_RESULT_OK){
                mcDBG_MSG_L1(("AR Lock    : %d\n", arLock));
                mcDBG_MSG_L1(("TS Lock    : %d\n", tsLock));
                mcDBG_MSG_L1(("Unlock Det : %d\n", unlock));
            }else{
                mcDBG_MSG_L1(("Error in monitorC_SyncStat\n"));
            }
        }else{
            mcDBG_MSG_L1(("Invalid System\n"));
        }

        /* IFAGC */
        {
            UINT32 ifAgc = 0;
            sony_result = sony_dvb_demod_monitor_IFAGCOut(pDemod, &ifAgc);
            if(sony_result == SONY_RESULT_OK){
                mcDBG_MSG_L1(("IFAGC (%%) : %d\n", ifAgc * 100 / 4095));
            }else{
                mcDBG_MSG_L1(("Error in monitor_IFAGCOut\n"));
            }
        }
        /* Carrier Offset */
        {
            INT32 offset = 0;
            sony_result = sony_dvb_demod_monitor_CarrierOffset(pDemod, &offset);
            if(sony_result == SONY_RESULT_OK){
                mcDBG_MSG_L1(("Carrier Offset (kHz) : %d\n", offset));
            }else{
                mcDBG_MSG_L1(("Error in monitor_CarrierOffset\n"));
            }
        }
        /* BER */
        if(pDemod->system == SONY_DVB_SYSTEM_DVBT){
            UINT32 ber = 0;
            sony_result = sony_dvb_demod_monitorT_PreViterbiBER(pDemod, &ber);
            if(sony_result == SONY_RESULT_OK){
                mcDBG_MSG_L1(("PreViterbi BER (1e7) : %d\n", ber));
            }else{
                mcDBG_MSG_L1(("Error in monitorT_PreViterbiBER\n"));
            }
            sony_result = sony_dvb_demod_monitorT_PreRSBER(pDemod, &ber);
            if(sony_result == SONY_RESULT_OK){
                mcDBG_MSG_L1(("PreRS BER (1e7) : %d\n", ber));
            }else{
                mcDBG_MSG_L1(("Error in monitorT_PreRSBER\n"));
            }
        }else if(pDemod->system == SONY_DVB_SYSTEM_DVBT2){
            UINT32 ber = 0;
            sony_result = sony_dvb_demod_monitorT2_PreLDPCBER(pDemod, &ber);
            if(sony_result == SONY_RESULT_OK){
                mcDBG_MSG_L1(("PreLDPC BER (1e7) : %d\n", ber));
            }else{
                mcDBG_MSG_L1(("Error in monitorT2_PreLDPCBER\n"));
            }
            sony_result = sony_dvb_demod_monitorT2_PreBCHBER(pDemod, &ber);
            if(sony_result == SONY_RESULT_OK){
                mcDBG_MSG_L1(("PreBCH BER (1e9) : %d\n", ber));
            }else{
                mcDBG_MSG_L1(("Error in monitorT2_PreBCHBER\n"));
            }
        }else if(pDemod->system == SONY_DVB_SYSTEM_DVBC){
            UINT32 ber = 0;
            sony_result = sony_dvb_demod_monitorC_PreRSBER(pDemod, &ber);
            if(sony_result == SONY_RESULT_OK){
                mcDBG_MSG_L1(("PreRS BER (1e7) : %d\n", ber));
            }else{
                mcDBG_MSG_L1(("Error in monitorC_PreRSBER\n"));
            }
        }else{
            mcDBG_MSG_L1(("Invalid System\n"));
        }

        /* PER */
        {
            UINT32 per = 0;
            sony_result = sony_dvb_demod_monitor_PER(pDemod, &per);
            if(sony_result == SONY_RESULT_OK){
                mcDBG_MSG_L1(("PER (1e6) : %d\n", per));
            }else{
                mcDBG_MSG_L1(("Error in monitor_PER\n"));
            }
        }

        /* SNR */
        if(pDemod->system == SONY_DVB_SYSTEM_DVBT){
            INT32 snr = 0;
            sony_result = sony_dvb_demod_monitorT_SNR(pDemod, &snr);
            if(sony_result == SONY_RESULT_OK){
                mcDBG_MSG_L1(("SNR (dB) : %d.%03d\n", snr/1000U, snr%1000U));
            }else{
                mcDBG_MSG_L1(("Error in monitorT_SNR\n"));
            }
        }else if(pDemod->system == SONY_DVB_SYSTEM_DVBT2){
            INT32 snr = 0;
            sony_result = sony_dvb_demod_monitorT2_SNR(pDemod, &snr);
            if(sony_result == SONY_RESULT_OK){
                mcDBG_MSG_L1(("SNR (dB) : %d.%03d\n", snr/1000U, snr%1000U));
            }else{
                mcDBG_MSG_L1(("Error in monitorT2_SNR\n"));
            }
        }else if(pDemod->system == SONY_DVB_SYSTEM_DVBC){
            INT32 snr = 0;
            sony_result = sony_dvb_demod_monitorC_SNR(pDemod, &snr);
            if(sony_result == SONY_RESULT_OK){
                mcDBG_MSG_L1(("SNR (dB) : %d.%03d\n", snr/1000U, snr%1000U));
            }else{
                mcDBG_MSG_L1(("Error in monitorC_SNR\n"));
            }
        }else{
            mcDBG_MSG_L1(("Invalid System\n"));
        }

        /* SQI and SSI */
        if((pDemod->system == SONY_DVB_SYSTEM_DVBT) || (pDemod->system == SONY_DVB_SYSTEM_DVBT2)){
            uint8_t sqi = 0;
            uint8_t ssi = 0;
            sony_result = sony_dvb_demod_monitor_Quality(pDemod, &sqi);
            if(sony_result == SONY_RESULT_OK){
                mcDBG_MSG_L1(("SQI (%%) : %d\n", sqi));
            }else{
                mcDBG_MSG_L1(("Error in monitor_Quality\n"));
            }

            sony_result = sony_dvb_demod_monitor_SSI(pDemod, &ssi);
            if(sony_result == SONY_RESULT_OK){
                mcDBG_MSG_L1(("SSI (%%) : %d\n", ssi));
            }else{
                mcDBG_MSG_L1(("Error in monitor_SSI\n"));
            }
        }

        /* System Dependent Information */
        if(pDemod->system == SONY_DVB_SYSTEM_DVBT){
            sony_dvbt_tpsinfo_t tpsinfo;
            sony_result = sony_dvb_demod_monitorT_TPSInfo(pDemod, &tpsinfo);
            if(sony_result == SONY_RESULT_OK){
                mcDBG_MSG_L1(("DVB-T TPS Information ----\n"));
                mcDBG_MSG_L1(("Constellation : "));
                switch(tpsinfo.constellation){
                case SONY_DVBT_CONSTELLATION_QPSK:
                    mcDBG_MSG_L1(("QPSK\n")); break;
                case SONY_DVBT_CONSTELLATION_16QAM:
                    mcDBG_MSG_L1(("16QAM\n")); break;
                case SONY_DVBT_CONSTELLATION_64QAM:
                    mcDBG_MSG_L1(("64QAM\n")); break;
                default:
                    mcDBG_MSG_L1(("Unknown(%d)\n", (int)tpsinfo.constellation));
                    break;
                }

                mcDBG_MSG_L1(("Hierarchy : "));
                switch(tpsinfo.hierarchy){
                case SONY_DVBT_HIERARCHY_NON:
                    mcDBG_MSG_L1(("Non hierarchical\n")); break;
                case SONY_DVBT_HIERARCHY_1:
                    mcDBG_MSG_L1(("a=1\n")); break;
                case SONY_DVBT_HIERARCHY_2:
                    mcDBG_MSG_L1(("a=2\n")); break;
                case SONY_DVBT_HIERARCHY_4:
                    mcDBG_MSG_L1(("a=4\n")); break;
                default:
                    mcDBG_MSG_L1(("Unknown(%d)\n", (int)tpsinfo.hierarchy));
                    break;
                }

                mcDBG_MSG_L1(("RateHP : "));
                switch(tpsinfo.rateHP){
                case SONY_DVBT_CODERATE_1_2:
                    mcDBG_MSG_L1(("1/2\n")); break;
                case SONY_DVBT_CODERATE_2_3:
                    mcDBG_MSG_L1(("2/3\n")); break;
                case SONY_DVBT_CODERATE_3_4:
                    mcDBG_MSG_L1(("3/4\n")); break;
                case SONY_DVBT_CODERATE_5_6:
                    mcDBG_MSG_L1(("5/6\n")); break;
                case SONY_DVBT_CODERATE_7_8:
                    mcDBG_MSG_L1(("7/8\n")); break;
                default:
                    mcDBG_MSG_L1(("Unknown(%d)\n", (int)tpsinfo.rateHP));
                    break;
                }

                mcDBG_MSG_L1(("RateLP : "));
                switch(tpsinfo.rateLP){
                case SONY_DVBT_CODERATE_1_2:
                    mcDBG_MSG_L1(("1/2\n")); break;
                case SONY_DVBT_CODERATE_2_3:
                    mcDBG_MSG_L1(("2/3\n")); break;
                case SONY_DVBT_CODERATE_3_4:
                    mcDBG_MSG_L1(("3/4\n")); break;
                case SONY_DVBT_CODERATE_5_6:
                    mcDBG_MSG_L1(("5/6\n")); break;
                case SONY_DVBT_CODERATE_7_8:
                    mcDBG_MSG_L1(("7/8\n")); break;
                default:
                    mcDBG_MSG_L1(("Unknown(%d)\n", (int)tpsinfo.rateLP));
                    break;
                }

                mcDBG_MSG_L1(("Guard : "));
                switch(tpsinfo.guard){
                case SONY_DVBT_GUARD_1_32:
                    mcDBG_MSG_L1(("1/32\n")); break;
                case SONY_DVBT_GUARD_1_16:
                    mcDBG_MSG_L1(("1/16\n")); break;
                case SONY_DVBT_GUARD_1_8:
                    mcDBG_MSG_L1(("1/8\n")); break;
                case SONY_DVBT_GUARD_1_4:
                    mcDBG_MSG_L1(("1/4\n")); break;
                default:
                    mcDBG_MSG_L1(("Unknown(%d)\n", (int)tpsinfo.guard));
                    break;
                }

                mcDBG_MSG_L1(("Mode : "));
                switch(tpsinfo.mode){
                case SONY_DVBT_MODE_2K:
                    mcDBG_MSG_L1(("2k mode\n")); break;
                case SONY_DVBT_MODE_8K:
                    mcDBG_MSG_L1(("8k mode\n")); break;
                default:
                    mcDBG_MSG_L1(("Unknown(%d)\n", (int)tpsinfo.mode));
                    break;
                }

                mcDBG_MSG_L1(("Fnum : %02x\n", tpsinfo.fnum));
                mcDBG_MSG_L1(("LengthIndicator : %02x\n", tpsinfo.lengthIndicator));
                mcDBG_MSG_L1(("CellID : 0x%04X\n", tpsinfo.cellID));
            }else{
                mcDBG_MSG_L1(("Error in monitorT_TPSInfo\n"));
            }
        }else if(pDemod->system == SONY_DVB_SYSTEM_DVBT2){
            sony_dvbt2_ofdm_t ofdmInfo;
            sony_dvbt2_plp_t PLPInfo;

            sony_result = sony_dvb_demod_monitorT2_Ofdm(pDemod, &ofdmInfo);
            if(sony_result == SONY_RESULT_OK){
                mcDBG_MSG_L1(("DVB-T2 OFDM Information ----\n"));
                mcDBG_MSG_L1(("Mixed : "));
                if(ofdmInfo.mixed){
                    mcDBG_MSG_L1(("Mixed\n"));
                }else{
                    mcDBG_MSG_L1(("Not mixed\n"));
                }
                
                mcDBG_MSG_L1(("S1 : "));
                switch(ofdmInfo.s1){
                case SONY_DVBT2_S1_SISO:
                    mcDBG_MSG_L1(("SISO\n")); break;
                case SONY_DVBT2_S1_MISO:
                    mcDBG_MSG_L1(("MISO\n")); break;
                default:
                    mcDBG_MSG_L1(("Unknown\n")); break;
                }
                
                mcDBG_MSG_L1(("Mode : "));
                switch(ofdmInfo.mode){
                case SONY_DVBT2_M2K:
                    mcDBG_MSG_L1(("2K\n")); break;
                case SONY_DVBT2_M8K:
                    mcDBG_MSG_L1(("8K\n")); break;
                case SONY_DVBT2_M4K:
                    mcDBG_MSG_L1(("4K\n")); break;
                case SONY_DVBT2_M1K:
                    mcDBG_MSG_L1(("1K\n")); break;
                case SONY_DVBT2_M16K:
                    mcDBG_MSG_L1(("16K\n")); break;
                case SONY_DVBT2_M32K:
                    mcDBG_MSG_L1(("32K\n")); break;
                default:
                    mcDBG_MSG_L1(("Unknown\n")); break;
                }
                
                mcDBG_MSG_L1(("GI : "));
                switch(ofdmInfo.gi){
                case SONY_DVBT2_G1_32:
                    mcDBG_MSG_L1(("1/32\n")); break;
                case SONY_DVBT2_G1_16:
                    mcDBG_MSG_L1(("1/16\n")); break;
                case SONY_DVBT2_G1_8:
                    mcDBG_MSG_L1(("1/8\n")); break;
                case SONY_DVBT2_G1_4:
                    mcDBG_MSG_L1(("1/4\n")); break;
                case SONY_DVBT2_G1_128:
                    mcDBG_MSG_L1(("1/128\n")); break;
                case SONY_DVBT2_G19_128:
                    mcDBG_MSG_L1(("19/128\n")); break;
                case SONY_DVBT2_G19_256:
                    mcDBG_MSG_L1(("19/256\n")); break;
                default:
                    mcDBG_MSG_L1(("Unknown\n")); break;
                }
                
                mcDBG_MSG_L1(("PP : "));
                switch(ofdmInfo.pp){
                case SONY_DVBT2_PP1:
                    mcDBG_MSG_L1(("PP1\n")); break;
                case SONY_DVBT2_PP2:
                    mcDBG_MSG_L1(("PP2\n")); break;
                case SONY_DVBT2_PP3:
                    mcDBG_MSG_L1(("PP3\n")); break;
                case SONY_DVBT2_PP4:
                    mcDBG_MSG_L1(("PP4\n")); break;
                case SONY_DVBT2_PP5:
                    mcDBG_MSG_L1(("PP5\n")); break;
                case SONY_DVBT2_PP6:
                    mcDBG_MSG_L1(("PP6\n")); break;
                case SONY_DVBT2_PP7:
                    mcDBG_MSG_L1(("PP7\n")); break;
                case SONY_DVBT2_PP8:
                    mcDBG_MSG_L1(("PP8\n")); break;
                default:
                    mcDBG_MSG_L1(("Unknown\n")); break;
                }
                
                mcDBG_MSG_L1(("BWExt : %u\n", ofdmInfo.bwExt));

                mcDBG_MSG_L1(("PAPR : "));
                switch(ofdmInfo.papr){
                case SONY_DVBT2_PAPR_NONE:
                    mcDBG_MSG_L1(("None\n")); break;
                case SONY_DVBT2_PAPR_ACE:
                    mcDBG_MSG_L1(("ACE\n")); break;
                case SONY_DVBT2_PAPR_TR:
                    mcDBG_MSG_L1(("TR\n")); break;
                case SONY_DVBT2_PAPR_TR_ACE:
                    mcDBG_MSG_L1(("ACE and TR\n")); break;
                default:
                    mcDBG_MSG_L1(("Unknown\n")); break;
                }
                
                mcDBG_MSG_L1(("Num of Symbols : %u\n", ofdmInfo.numSymbols));
            }else{
                mcDBG_MSG_L1(("Error in monitorT2_Ofdm\n"));
            }

            sony_result = sony_dvb_demod_monitorT2_ActivePLP(pDemod, SONY_DVBT2_PLP_DATA, &PLPInfo);
            if(sony_result == SONY_RESULT_OK){
                mcDBG_MSG_L1(("DVB-T2 PLP Information ----\n"));
                mcDBG_MSG_L1(("ID : %u\n", PLPInfo.id));

                mcDBG_MSG_L1(("Type : "));
                switch(PLPInfo.type){
                case SONY_DVBT2_PLP_TYPE_COMMON:
                    mcDBG_MSG_L1(("Common\n")); break;
                case SONY_DVBT2_PLP_TYPE_DATA1:
                    mcDBG_MSG_L1(("Data Type 1\n")); break;
                case SONY_DVBT2_PLP_TYPE_DATA2:
                    mcDBG_MSG_L1(("Data Type 2\n")); break;
                default:
                    mcDBG_MSG_L1(("Unknown\n")); break;
                }

                mcDBG_MSG_L1(("Payload : "));
                switch(PLPInfo.payload){
                case SONY_DVBT2_PLP_PAYLOAD_GFPS:
                    mcDBG_MSG_L1(("GFPS\n")); break;
                case SONY_DVBT2_PLP_PAYLOAD_GCS:
                    mcDBG_MSG_L1(("GCS\n")); break;
                case SONY_DVBT2_PLP_PAYLOAD_GSE:
                    mcDBG_MSG_L1(("GSE\n")); break;
                case SONY_DVBT2_PLP_PAYLOAD_TS:
                    mcDBG_MSG_L1(("TS\n")); break;
                default:
                    mcDBG_MSG_L1(("Unknown\n")); break;
                }

                mcDBG_MSG_L1(("FF : %u\n", PLPInfo.ff));
                mcDBG_MSG_L1(("First RF Index : %u\n", PLPInfo.firstRfIdx));
                mcDBG_MSG_L1(("First Frame Index : %u\n", PLPInfo.firstFrmIdx));
                mcDBG_MSG_L1(("Group ID : %u\n", PLPInfo.groupId));

                mcDBG_MSG_L1(("Constellation : "));
                switch(PLPInfo.constell){
                case SONY_DVBT2_QPSK:
                    mcDBG_MSG_L1(("QPSK\n")); break;
                case SONY_DVBT2_QAM16:
                    mcDBG_MSG_L1(("16QAM\n")); break;
                case SONY_DVBT2_QAM64:
                    mcDBG_MSG_L1(("64QAM\n")); break;
                case SONY_DVBT2_QAM256:
                    mcDBG_MSG_L1(("256QAM\n")); break;
                default:
                    mcDBG_MSG_L1(("Unknown\n")); break;
                }

                mcDBG_MSG_L1(("Code Rate : "));
                switch(PLPInfo.plpCr){
                case SONY_DVBT2_R1_2:
                    mcDBG_MSG_L1(("1/2\n")); break;
                case SONY_DVBT2_R3_5:
                    mcDBG_MSG_L1(("3/5\n")); break;
                case SONY_DVBT2_R2_3:
                    mcDBG_MSG_L1(("2/3\n")); break;
                case SONY_DVBT2_R3_4:
                    mcDBG_MSG_L1(("3/4\n")); break;
                case SONY_DVBT2_R4_5:
                    mcDBG_MSG_L1(("4/5\n")); break;
                case SONY_DVBT2_R5_6:
                    mcDBG_MSG_L1(("5/6\n")); break;
                default:
                    mcDBG_MSG_L1(("Unknown\n")); break;
                }

                mcDBG_MSG_L1(("Rotation : %s\n", PLPInfo.rot ? "Used" : "Not Used"));

                mcDBG_MSG_L1(("FEC Type : "));
                switch(PLPInfo.fec){
                case SONY_DVBT2_FEC_LDPC_16K:
                    mcDBG_MSG_L1(("16K LDPC\n")); break;
                case SONY_DVBT2_FEC_LDPC_64K:
                    mcDBG_MSG_L1(("64K LDPC\n")); break;
                default:
                    mcDBG_MSG_L1(("Unknown\n")); break;
                }

                mcDBG_MSG_L1(("Max Num of PLP Blocks : %u\n", PLPInfo.numBlocksMax));
                mcDBG_MSG_L1(("Frame Interval : %u\n", PLPInfo.frmInt));
                mcDBG_MSG_L1(("Time IL Length : %u\n", PLPInfo.tilLen));
                mcDBG_MSG_L1(("Time IL Type : %u\n", PLPInfo.tilType));
                mcDBG_MSG_L1(("In-Band Flag : %u\n", PLPInfo.inBandFlag));
            }else{
                mcDBG_MSG_L1(("Error in monitorT2_ActivePLP\n"));
            }

            // List PLP IDs
            {
                UINT8 aPLPId[255];
                UINT8 nNumPLPs = 0;
                UINT32 i = 0;
                sony_result = sony_dvb_demod_monitorT2_DataPLPs(pDemod, aPLPId, &nNumPLPs);
                if(sony_result == SONY_RESULT_OK){
                    mcDBG_MSG_L1(("DVB-T2 PLP ID List ----\n"));
                    mcDBG_MSG_L1(("Num of PLPs : %u\n", nNumPLPs));
                    for(i=0; i<nNumPLPs; i++){
                        mcDBG_MSG_L1(("PLP(%u) : 0x%02x\n", i+1, aPLPId[i]));
                    }
                }else{
                    mcDBG_MSG_L1(("Error in monitorT2_DataPLPs\n"));
                }
            }
        }else if(pDemod->system == SONY_DVB_SYSTEM_DVBC){
            sony_dvbc_constellation_t qam = SONY_DVBC_CONSTELLATION_16QAM;
            UINT32 symRate = 0;

            sony_result = sony_dvb_demod_monitorC_QAM(pDemod, &qam);
            mcDBG_MSG_L1(("Constellation : "));
            if(sony_result == SONY_RESULT_OK){
                switch(qam){
                case SONY_DVBC_CONSTELLATION_16QAM:
                    mcDBG_MSG_L1(("16 QAM\n")); break;
                case SONY_DVBC_CONSTELLATION_32QAM:
                    mcDBG_MSG_L1(("32 QAM\n")); break;
                case SONY_DVBC_CONSTELLATION_64QAM:
                    mcDBG_MSG_L1(("64 QAM\n")); break;
                case SONY_DVBC_CONSTELLATION_128QAM:
                    mcDBG_MSG_L1(("128 QAM\n")); break;
                case SONY_DVBC_CONSTELLATION_256QAM:
                    mcDBG_MSG_L1(("256 QAM\n")); break;
                default:
                    mcDBG_MSG_L1(("Unknown\n")); break;
                }
            }else{
                mcDBG_MSG_L1(("Error in monitorC_QAM\n"));
            }
            
            sony_result = sony_dvb_demod_monitorC_SymbolRate(pDemod, &symRate);
            if(sony_result == SONY_RESULT_OK){
                mcDBG_MSG_L1(("SymbolRate : %d kSymbol/sec\n", symRate));
            }else{
                mcDBG_MSG_L1(("Error in monitorC_SymbolRate\n"));
            }
        }else{
            mcDBG_MSG_L1(("Invalid System\n"));
        }

        /* TS Format */
        /*
        {
            TS_FMT_T tsfmt;
            if(sony_GetTsFmt(pDemod, &tsfmt)){
                mcDBG_MSG_L1(("SerOrPar : %d\n", tsfmt.SerOrPar));
                mcDBG_MSG_L1(("ClockPolarity : %d\n", tsfmt.ClockPolarity));
                mcDBG_MSG_L1(("Tristate : %d\n", tsfmt.fgTristate));
                mcDBG_MSG_L1(("TsClock : %d\n", tsfmt.u4TsClockInKHz));
                mcDBG_MSG_L1(("GatedClock : %d\n", tsfmt.fgGatedClock));
                mcDBG_MSG_L1(("ExternalDemod : %d\n", tsfmt.fgExternalDemod));
            }else{
                mcDBG_MSG_L1(("Error in GetTsFmt\n"));
            }
        }
        */

        mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
        break;
    case 'd':
        {
            UINT8 data[256];
            UINT8 bank = 0;
            
            if(i4Argc < 2){
                mcDBG_MSG_INFO(("Error: No Bank.\n"));
                return;
            }
            bank = (UINT8)sony_strtoul(aszArgv[1]);

            mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock); // Enter Critical Section
            sony_result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x00, bank);
            if(sony_result != SONY_RESULT_OK){
                mcDBG_MSG_L1(("I2C Error.\n"));
                mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
                return;
            }
            sony_result = pDemod->pI2c->ReadRegister(pDemod->pI2c, pDemod->i2cAddress, 0x00, data, 256);
            mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock); // Leave Critical Section
            if(sony_result != SONY_RESULT_OK){
                mcDBG_MSG_L1(("I2C Error.\n"));
                return;
            }
            mcDBG_MSG_L1(("Register Dump Result. (Bank:0x%02X)\n", bank));
            mcDBG_MSG_L1(("     00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n"));
            {
                int i = 0;
                int j = 0;
                char dumpline[53];
                char* ptr = dumpline;
                const char hexchar[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

                dumpline[52] = '\0';
                for(i=0; i<16; i++){
                    ptr = dumpline;
                    for(j=0; j<16; j++){
                        if(j == 0){
                            *ptr++ = hexchar[(i*16) >> 4];
                            *ptr++ = hexchar[(i*16) & 0x0F];
                            *ptr++ = ' ';
                            *ptr++ = '|';
                        }
                        *ptr++ = ' ';
                        *ptr++ = hexchar[data[(i*16)+j] >> 4];
                        *ptr++ = hexchar[data[(i*16)+j] & 0x0F];
                    }
                    mcDBG_MSG_L1(("%s\n", dumpline));
                }
            }
            mcDBG_MSG_L1(("\n"));
        }
        break;
    case 'r':
        {
            UINT8 data[256];
            UINT8 subAddress = 0;
            UINT32 length = 0;
            UINT32 i = 0;
            if(i4Argc < 3){
                mcDBG_MSG_L1(("Error: No read register and length.\n"));
                return;
            }
            subAddress = (UINT8)sony_strtoul(aszArgv[1]);
            length = (UINT32)sony_strtoul(aszArgv[2]);
            if(length > 256){
                length = 256;
            }
            mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
            sony_result = pDemod->pI2c->ReadRegister(pDemod->pI2c, pDemod->i2cAddress, subAddress, data, length);
            mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
            if(sony_result != SONY_RESULT_OK){
                mcDBG_MSG_L1(("I2C Error.\n"));
                return;
            }
            for(i=0; i<length; i++){
                mcDBG_MSG_L1(("Demod Register 0x%02X = 0x%02X\n", i + subAddress, data[i]));
            }
        }
        break;
    case 'w':
        {
            UINT8 subAddress = 0;
            UINT8 value = 0;
            if(i4Argc < 3){
                mcDBG_MSG_L1(("Error: No write register and data.\n"));
                return;
            }
            subAddress = (UINT8)sony_strtoul(aszArgv[1]);
            value = (UINT8)sony_strtoul(aszArgv[2]);
            mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
            sony_result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, subAddress, value);
            mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
            if(sony_result != SONY_RESULT_OK){
                mcDBG_MSG_L1(("I2C Error.\n"));
                return;
            }
            mcDBG_MSG_L1(("Wrote Demod Register 0x%02X = 0x%02X\n", subAddress, value));
        }
        break;
    case 'f':
        {
            UINT32 u4Freq = 0;
            PARAM_SETFREQ_T param;

            if(i4Argc < 2){
                mcDBG_MSG_L1(("Error: No Frequency.\n"));
                return;
            }
            u4Freq = (UINT32)sony_strtoul(aszArgv[1]); // freq
            
            param.Freq = u4Freq;
            switch(pDemod->system){
            case SONY_DVB_SYSTEM_DVBT:
                param.Modulation = MOD_DVBT; break;
            case SONY_DVB_SYSTEM_DVBT2:
                param.Modulation = MOD_DVBT2; break;
            case SONY_DVB_SYSTEM_DVBC:
                param.Modulation = MOD_DVBC; break;
            default:
                mcDBG_MSG_L1(("Error: Demod is not active.\n"));
                return;
            }
            mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
            if(ITuner_SetFreq(ITunerGetCtx(), &param))
            {
                mcDBG_MSG_L1(("Failed on Config tuner (%d KHz)\n", u4Freq));
            }
            else
            {
                mcDBG_MSG_L1(("Config tuner(%d KHz) OK!\n", u4Freq));
            }
            mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
        }
        break;
    case 't':
        {
            UINT32 tsSel = 0;
            if(i4Argc < 2){
                mcDBG_MSG_L1(("Error: No TS Setting.\n"));
                return;
            }
            tsSel = (UINT32)sony_strtoul(aszArgv[1]);
            mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
            // TS Output Parallel/Serial
            sony_result = sony_dvb_demod_SetConfig(pDemod, DEMOD_CONFIG_PARALLEL_SEL, tsSel ? 1 : 0);
            if(sony_result != SONY_RESULT_OK){
                mcDBG_MSG_L1(("Error: SetConfig Failed.\n"));
            }
            mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
        }
        break;
    case 'l':
        {
            mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
            // TS Output Parallel/Serial
            sony_result = sony_dvb_demod_Sleep(pDemod);
            if(sony_result != SONY_RESULT_OK){
                mcDBG_MSG_L1(("Error: Sleep Failed.\n"));
            }
            mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
        }
        break;
    default:
        break;
    }
}

void DVBT2_TunerDetachI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx, BOOL bDetach)
{
    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
	//add your implementation here
    mcSHOW_DBG_MSG2(("No implementation in DVBT2_TunerDetachI2C()\n"));

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);
}

#define C_T2_FREQ_DBOUND_UPPER   859*1000*1000     //modified for DVB-C application, Menghau, 091020
#define C_T2_FREQ_DBOUND_LOWER    48*1000*1000
//-------------------------------------------------------------------------
/**
 *  This API get attribute from tuner driver
 *  Inputs:
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context.
 *  @param   e_get_type         tuner get type, refer x_tuner.h TUNER_GET_TYPE_XXXXX
 *  Outputs:
 *  @param   *pzAttribute       Attribute structure to be filled. Refer x_tuner.h structure definition TUNER_XXXXX_ATTRIBUTE_T
 *  @param   *pzAttributeLen    length to be filled.
 *  @retval  DRVAPI_TUNER_OK    success.
 *  @retval  DRVAPI_TUNER_ERROR fail.
 */
//-------------------------------------------------------------------------
INT16 DVBT2_TunerGetAttribute(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_GET_TYPE_T  e_get_type,
                                VOID *pzAttribute, SIZE_T* pzAttributeLen)
{
     TUNER_DIG_ATTRIBUTE_T *pAttri;
      ITUNER_CTX_T *pTunerCtx;
          INT32 i4Temp = 0;
        pTunerCtx = ITunerGetCtx();
    sony_dvb_demod_t *pDemod = &pSonyDemod->demod;

    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);

    mcDBG_MSG_L2(("%s \n", __FUNCTION__));

    //add driver implementation code here
    switch (e_get_type)
    {
    case TUNER_GET_TYPE_BER:
        mcSHOW_DBG_MSG3(("e_get_type = TUNER_GET_TYPE_BER\r\n"));
        {
            UINT32 ber = sony_GetBER();
            i4Temp = (INT32)ber;
        }
        *pzAttributeLen = sizeof(INT32);
        x_memcpy(pzAttribute, &i4Temp, *pzAttributeLen);
        break;
    case TUNER_GET_TYPE_DIG_ATTRIBUTE:
    	
    	        mcSHOW_DBG_MSG3(("e_get_type = TUNER_GET_TYPE_DIG_ATTRIBUTE\r\n"));
        pAttri = (TUNER_DIG_ATTRIBUTE_T *) pzAttribute;
        //Tuner frequency boundary
        pAttri->ui4_lower_bound_freq = C_T2_FREQ_DBOUND_LOWER;
        pAttri->ui4_upper_bound_freq = C_T2_FREQ_DBOUND_UPPER;
        // freq offset tolerence range in Hz (1M mapping to 100K)
		pAttri->b_auto_qam_capability = TRUE;
		pAttri->b_auto_sr_capability = TRUE;
     switch (pDemod->system)
     	{

	    case SONY_DVB_SYSTEM_DVBC:
               pAttri->ui4_fine_tune_delta_freq = 400000;
              break;	    	
	     case SONY_DVB_SYSTEM_DVBT:
	     case SONY_DVB_SYSTEM_DVBT2: 	
                	{
		        switch (pDemod->bandWidth)
			 {
				case 6:  pAttri->ui4_fine_tune_delta_freq = 536000; break; //6MHz
				case 7:  pAttri->ui4_fine_tune_delta_freq = 625000; break; //7MHz
				case 8:  pAttri->ui4_fine_tune_delta_freq = 714000; break; //8MHz
				
				default: pAttri->ui4_fine_tune_delta_freq = 0;		break;
			}	     	
		 break;
		 default:
		 	break;
		 }
     	}	 
        
        break;
    case TUNER_GET_TYPE_RAW_SIGNAL_LEVEL:
    	       mcSHOW_DBG_MSG3(("e_get_type = TUNER_GET_TYPE_RAW_SIGNAL_LEVEL\r\n"));
        *pzAttributeLen = sizeof(S8);
	//   i4Temp =DVBT2_GetIFAGC(pSonyDemod);
        x_memcpy(pzAttribute, &i4Temp, *pzAttributeLen);

        break;
    case TUNER_GET_TYPE_BER_TOLERANCE:
           mcSHOW_DBG_MSG3(("e_get_type = TUNER_GET_TYPE_BER_TOLERANCE\r\n"));
        *pzAttributeLen = sizeof(INT32);
        i4Temp = 430;  // copy from in-house DVBT
        x_memcpy(pzAttribute, &i4Temp, *pzAttributeLen);
        break;
    case TUNER_GET_TYPE_SIGNAL_LEVEL_TOLERANCE:
    	        mcSHOW_DBG_MSG3(("e_get_type = TUNER_GET_TYPE_SIGNAL_LEVEL_TOLERANCE\r\n"));
        *pzAttributeLen = sizeof(UINT8);
        x_memcpy(pzAttribute, &(pTunerCtx->specific_member.eu_ctx.m_SigLvTh), *pzAttributeLen);

        break;
    case TUNER_GET_TYPE_TS_FMT:
        {
            TS_FMT_T tsFmt;
            *pzAttributeLen = sizeof(TS_FMT_T);
            sony_GetTsFmt(pDemod, &tsFmt);
            x_memcpy(pzAttribute, &tsFmt, *pzAttributeLen);
        }
        break;
 case TUNER_GET_TYPE_DVBT2_PLP:
 		 {
	        TUNER_DVBT2_PLP_LIST_INFO sPlpIdList;
			//DVBT2_PARAM mDVBT2_PARAM;
			UINT8 u1plpcnt=0;
          	*pzAttributeLen = sizeof(TUNER_DVBT2_PLP_LIST_INFO);
			//mDVBT2_PARAM.eOP=GET_MPLP_LIST;
			//MTK_DVBT2_GetInfo(&mDVBT2_PARAM);
			//sPlpIdList.ui1_current_count=mDVBT2_PARAM.outplpNumb;//d110916_Haibo:PLPid table length
				sony_result_t sony_result=SONY_RESULT_OK;
				UINT8 aPLPId[255];
                UINT8 nNumPLPs = 0;
                UINT32 i = 0;
                sony_result = sony_dvb_demod_monitorT2_DataPLPs(pDemod, aPLPId, &nNumPLPs);
                if(sony_result == SONY_RESULT_OK){
                    mcDBG_MSG_L1(("DVB-T2 PLP ID List ----\n"));
                    mcDBG_MSG_L1(("Num of PLPs : %u\n", nNumPLPs));
                    for(i=0; i<nNumPLPs; i++){
                        mcDBG_MSG_L1(("PLP(%u) : 0x%02x\n", i+1, aPLPId[i]));
                    }
                }else{
                    mcDBG_MSG_L1(("Error in monitorT2_DataPLPs\n"));
					nNumPLPs=1;//default splp
                }
			sPlpIdList.ui1_current_count=nNumPLPs;//mDVBT2_PARAM.outplpdataNumb;//d110920_Haibo:Get data plp table length
			for(;u1plpcnt<sPlpIdList.ui1_current_count;u1plpcnt++)
			{
			   mcDBG_MSG_L2(("index %d data plp id=%d\n",u1plpcnt,aPLPId[u1plpcnt]));
			}
			x_memcpy(&(sPlpIdList.ui1_PLPLIST[0]), &aPLPId[0], sPlpIdList.ui1_current_count*sizeof(UINT8));  
            x_memcpy(pzAttribute, &sPlpIdList, *pzAttributeLen);   
		}
 		break;
    default:
        mcSHOW_DBG_MSG3(("unknown: e_get_type = %d\r\n", e_get_type));
        break;
    }

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);

    return DRVAPI_TUNER_OK;
}

//-------------------------------------------------------------------------
/**
 *  This API get attribute from tuner driver
 *  Inputs:
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context.
 *  @param   e_get_type         tuner get type, refer x_tuner.h TUNER_GET_TYPE_XXXXX
 *  Outputs:
 *  @param   *pzAttribute       Attribute structure to be filled. Refer x_tuner.h structure definition TUNER_XXXXX_ATTRIBUTE_T
 *  @param   *pzAttributeLen    length to be filled.
 *  @retval  DRVAPI_TUNER_OK    success.
 *  @retval  DRVAPI_TUNER_ERROR fail.
 */
//-------------------------------------------------------------------------
INT16 DVBT2_TunerGetAttribute2(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_GET_TYPE_T  e_get_type,
                                VOID *pzAttribute, SIZE_T* pzAttributeLen, DRV_CONN_TYPE_T e_conn_type)
{
     TUNER_DIG_ATTRIBUTE_T *pAttri;
      ITUNER_CTX_T *pTunerCtx;
          INT32 i4Temp = 0;
        pTunerCtx = ITunerGetCtx();
    sony_result_t sony_result = SONY_RESULT_OK;
    sony_dvb_demod_t *pDemod = &pSonyDemod->demod;

    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);

    mcDBG_MSG_L1(("222%s \n", __FUNCTION__));

    //add driver implementation code here
    switch (e_get_type)
    {
    case TUNER_GET_TYPE_BER:
        mcSHOW_DBG_MSG3(("e_get_type = TUNER_GET_TYPE_BER\r\n"));
        {
            UINT32 ber = sony_GetBER();
            i4Temp = (INT32)ber;
        }
        *pzAttributeLen = sizeof(INT32);
        x_memcpy(pzAttribute, &i4Temp, *pzAttributeLen);
        break;
    case TUNER_GET_TYPE_DIG_ATTRIBUTE:
            mcSHOW_DBG_MSG3(("e_get_type = TUNER_GET_TYPE_DIG_ATTRIBUTE\r\n"));
        pAttri = (TUNER_DIG_ATTRIBUTE_T *) pzAttribute;
        //Tuner frequency boundary
        pAttri->ui4_lower_bound_freq = C_T2_FREQ_DBOUND_LOWER;
        pAttri->ui4_upper_bound_freq = C_T2_FREQ_DBOUND_UPPER;
        // freq offset tolerence range in Hz (1M mapping to 100K)
		pAttri->b_auto_qam_capability = TRUE;
		pAttri->b_auto_sr_capability = TRUE;
     switch (pDemod->system)
     	{

	    case SONY_DVB_SYSTEM_DVBC:
               pAttri->ui4_fine_tune_delta_freq = 400000;
              break;	    	
	     case SONY_DVB_SYSTEM_DVBT:
	     case SONY_DVB_SYSTEM_DVBT2: 	
                	{
		        switch (pDemod->bandWidth)
			 {
				case 6:  pAttri->ui4_fine_tune_delta_freq = 536000; break; //6MHz
				case 7:  pAttri->ui4_fine_tune_delta_freq = 625000; break; //7MHz
				case 8:  pAttri->ui4_fine_tune_delta_freq = 714000; break; //8MHz
				
				default: pAttri->ui4_fine_tune_delta_freq = 0;		break;
			}	     	
		 break;
		 default:
		 	break;
		 }
     	}	 
        
        break;
    case TUNER_GET_TYPE_RAW_SIGNAL_LEVEL:
             mcSHOW_DBG_MSG3(("e_get_type = TUNER_GET_TYPE_RAW_SIGNAL_LEVEL\r\n"));
        *pzAttributeLen = sizeof(S8);
	//   i4Temp =DVBT2_GetIFAGC(pSonyDemod);
        x_memcpy(pzAttribute, &i4Temp, *pzAttributeLen);
        break;
    case TUNER_GET_TYPE_BER_TOLERANCE:
               mcSHOW_DBG_MSG3(("e_get_type = TUNER_GET_TYPE_BER_TOLERANCE\r\n"));
        *pzAttributeLen = sizeof(INT32);
        i4Temp = 430;  // copy from in-house DVBT
        x_memcpy(pzAttribute, &i4Temp, *pzAttributeLen);
        break;
    case TUNER_GET_TYPE_SIGNAL_LEVEL_TOLERANCE:
              mcSHOW_DBG_MSG3(("e_get_type = TUNER_GET_TYPE_SIGNAL_LEVEL_TOLERANCE\r\n"));
        *pzAttributeLen = sizeof(UINT8);
        x_memcpy(pzAttribute, &(pTunerCtx->specific_member.eu_ctx.m_SigLvTh), *pzAttributeLen);
        break;
    case TUNER_GET_TYPE_TS_FMT:
        {
            TS_FMT_T tsFmt;
            *pzAttributeLen = sizeof(TS_FMT_T);
            sony_GetTsFmt(pDemod, &tsFmt);
            x_memcpy(pzAttribute, &tsFmt, *pzAttributeLen);
        }
        break;
    case TUNER_GET_TYPE_SQI:
        *pzAttributeLen = sizeof(INT32);
        {
            UINT8 sqi = 0;
            sony_result = sony_dvb_demod_monitor_Quality(pDemod, &sqi);
            if(sony_result != SONY_RESULT_OK){
                sqi = 0;
            }
            i4Temp = (INT32)sqi;
            mcSHOW_DBG_MSG3(("SQI = %d\r\n", sqi));             
            x_memcpy(pzAttribute, &i4Temp, *pzAttributeLen);        
        }
        break;
    case TUNER_GET_TYPE_SSI:
        *pzAttributeLen = sizeof(INT32);
        {
            UINT8 ssi = 0;
            sony_result = sony_dvb_demod_monitor_SSI(pDemod, &ssi);
            if(sony_result != SONY_RESULT_OK){
                ssi = 0;
            }
            i4Temp = (INT32)ssi;
            mcSHOW_DBG_MSG3(("SSI = %d\r\n", ssi));     
            x_memcpy(pzAttribute, &i4Temp, *pzAttributeLen);    
        }
        break;
	 case TUNER_GET_TYPE_DVBT2_PLP:
 		 {
	        TUNER_DVBT2_PLP_LIST_INFO sPlpIdList;
			//DVBT2_PARAM mDVBT2_PARAM;
			UINT8 u1plpcnt=0;
          	*pzAttributeLen = sizeof(TUNER_DVBT2_PLP_LIST_INFO);
			//mDVBT2_PARAM.eOP=GET_MPLP_LIST;
			//MTK_DVBT2_GetInfo(&mDVBT2_PARAM);
			//sPlpIdList.ui1_current_count=mDVBT2_PARAM.outplpNumb;//d110916_Haibo:PLPid table length
				sony_result_t sony_result=SONY_RESULT_OK;
				UINT8 aPLPId[255];
                UINT8 nNumPLPs = 0;
                UINT32 i = 0;
                sony_result = sony_dvb_demod_monitorT2_DataPLPs(pDemod, aPLPId, &nNumPLPs);
                if(sony_result == SONY_RESULT_OK){
                    mcDBG_MSG_L1(("DVB-T2 PLP ID List ----\n"));
                    mcDBG_MSG_L1(("Num of PLPs : %u\n", nNumPLPs));
                    for(i=0; i<nNumPLPs; i++){
                        mcDBG_MSG_L1(("PLP(%u) : 0x%02x\n", i+1, aPLPId[i]));
                    }
                }else{
                    mcDBG_MSG_L1(("Error in monitorT2_DataPLPs\n"));
					nNumPLPs=1;//default splp
                }
			sPlpIdList.ui1_current_count=nNumPLPs;//mDVBT2_PARAM.outplpdataNumb;//d110920_Haibo:Get data plp table length
			for(;u1plpcnt<sPlpIdList.ui1_current_count;u1plpcnt++)
			{
			   mcDBG_MSG_L2(("index %d data plp id=%d\n",u1plpcnt,aPLPId[u1plpcnt]));
			}
			x_memcpy(&(sPlpIdList.ui1_PLPLIST[0]), &aPLPId[0], sPlpIdList.ui1_current_count*sizeof(UINT8));  
            x_memcpy(pzAttribute, &sPlpIdList, *pzAttributeLen);   
		}
 		break;
    default:
        mcSHOW_DBG_MSG3(("unknown: e_get_type = %d\r\n", e_get_type));
        break;
    }

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);

    return DRVAPI_TUNER_OK;
}
//-------------------------------------------------------------------------
/**
 *  This API set attribute to tuner driver
 *  Inputs:
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context.
 *  @param   e_set_type         tuner set type, check x_tuner.h set operation TUNER_SET_TYPE_XXXX
 *  @param   *pvAttribute       set value.
 *  @param   zAttributeLen      length of *pvAttribute.
 *  @param   e_conn_type        tuner connection type, refer x_tuner.h
 *  @retval  DRVAPI_TUNER_OK    success.
 *  @retval  DRVAPI_TUNER_ERROR fail.
 */
//-------------------------------------------------------------------------
INT32 DVBT2_TunerSetAttribute(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_SET_TYPE_T  e_set_type,
                                        const VOID *pvAttribute, SIZE_T zAttributeLen)
{
    UCHAR ucData = *(UCHAR *)pvAttribute;
    //UCHAR *pucData = (UCHAR *)pvAttribute;
    sony_dvb_demod_t *pDemod = &pSonyDemod->demod;

    mcSEMA_LOCK_MTFE(pPdCtxDVBT2->hHalLock);
    //add driver implementation code here
    mcDBG_MSG_L1(("111%s \n", __FUNCTION__));

    switch (e_set_type)
    {
    case TUNER_SET_TYPE_TS_TRISTATE:
        {
            UINT8 data = 0;
            if(ucData == BOOL_FALSE){
                mcSHOW_DBG_MSG(("[DVBT2] SetTSPath : pass-through\n"));
                //Enable TS output
                data = 0;
            }else if (ucData == BOOL_TRUE){
                mcSHOW_DBG_MSG(("[DVBT2] SetTSPath : tri-state\n"));
                //Set TS output to high-Z
                data = 1;
            }
            // TS Output Enable (Bank:00h, Addr:F1h, Bit[0])
            // NOTE: This register will be overwritten in tune sequence in CXD2834 driver!
            pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x00, 0x00);
            pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0xF1, data);
        }
        break;
    case TUNER_SET_TYPE_TS_SP:
        break;
    case TUNER_SET_TYPE_CI_CARD_TYPE:
        break;
   /* case TUNER_SET_TYPE_AUTOPLP:
        // Save autoPLP mode to DEMOD_CXD2834_CTX_T struct
        if (ucData == BOOL_FALSE) {
            mcSHOW_DBG_MSG(("[DVBT2] disable auto plp\n"));
            pSonyDemod->autoPLP = 0;
        }
        else if (ucData == BOOL_TRUE) {
            mcSHOW_DBG_MSG(("[DVBT2] auto plp\n"));
            pSonyDemod->autoPLP = 1;
        }
        break;
        */
    default:
        mcSHOW_DBG_MSG3(("unknown: set Attribute:%d\r\n",e_set_type));
        break;
    }

    mcSEMA_UNLOCK_MTFE(pPdCtxDVBT2->hHalLock);

    return DRVAPI_TUNER_OK;
   
}	

//-----------------------------------------------------------------------------
// public functions
//-----------------------------------------------------------------------------
/*typedef INT16 (*LTDIS_TunerGetSync_fct) (PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                        DRV_CONN_TYPE_T e_conn_type);
INT16 DVBT2_TunerGetSync(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
*/

void PD_DVBT2_Register_LTDIS_Fct(PTD_CTX ptTDCtx)
{
    TUNER_DEVICE_CTX_T*		pTunerDeviceCtx = (TUNER_DEVICE_CTX_T*)ptTDCtx;
	
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerClose 	= DVBT2_TunerClose;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerAcq 	= DVBT2_TunerAcq;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGerSync 	= DVBT2_TunerGetSync;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignal 	= DVBT2_TunerGetSignal;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalLevel = DVBT2_TunerGetSignalLevel;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalLeveldBm = DVBT2_TunerGetSignalLeveldBm;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalPER = DVBT2_TunerGetSignalPER;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalSNR = DVBT2_TunerGetSignalSNR;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetMpgFmt 	= DVBT2_TunerSetMpgFmt;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetVer 	= DVBT2_TunerGetVer;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerNimTest 	= DVBT2_TunerNimTest;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetRegSetting = DVBT2_TunerSetRegSetting;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerShowRegSetting = DVBT2_TunerShowRegSetting;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerTestI2C 	= DVBT2_TunerTestI2C;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerCommand 	= DVBT2_TunerCommand;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDtdCommand 	= DVBT2_TunerDVBT2Command;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDetachI2C 	= DVBT2_TunerDetachI2C;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDisc 	= DVBT2_TunerDisc;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetAttribute= DVBT2_TunerSetAttribute;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetAttribute= DVBT2_TunerGetAttribute;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerBypassI2C = DVBT2_TunerBypassI2C;
#if defined(PD_REMAP_TER_DIG) || defined(PD_REMAP_CAB_DIG) || defined(PD_REMAP_SAT_DIG) || defined(PD_NO_USE_REMAP)
	pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetAttribute2 =  DVBT2_TunerGetAttribute2;
#endif
}

//-------------------------------------------------------------------------
/**
 *  This API is driver entry point. It performs
 *  1. allocate and initialize resource.
 *  2. do demod/tuner initialization.
 *  3. do LTDIS API mapping
 *  @param   ptTDCtx            TunerHAL Context use for LTDIS API mapping
 *  @param   ptTunerCfgInfo     Tuner configuration information.
 *  @param   pptTDSpecificCtx   Pointer to tuner driver context.
 *  @retval  DRVAPI_TUNER_OK    Success
 *  @retval  DRVAPI_TUNER_ERROR Fail
 */
//-------------------------------------------------------------------------
// Called from TunerOpen
// If the driver should re-initialize demod, this function should be called again.
STATIC INT32 sony_InitializeDemod(sony_dvb_demod_t *pDemod)
{
    sony_result_t sony_result = SONY_RESULT_OK;

    //Initialize Demod
    sony_result = sony_dvb_demod_Initialize(pDemod);
    if(sony_result != SONY_RESULT_OK){ return DRVAPI_TUNER_ERROR; };

    // Demod parameter configuration
    // TS output format, IFAGC negative/positive, Tuner spectrum, IF Shared...
    // Please check sony_dvb_demod_SetConfig functions

    // Disable RF Level monitor
    sony_result = sony_dvb_demod_SetConfig(pDemod, DEMOD_CONFIG_RFLVMON_ENABLE, 0);
    if(sony_result != SONY_RESULT_OK){ return DRVAPI_TUNER_ERROR; }

    // TS Output Serial (0: Serial, 1: Parallel(default))
    #ifdef SONY_DEMOD_TSOUTPUT_SERIAL
	 mcDBG_MSG_L1(("SONY_DEMOD_TSOUTPUT_SERIAL: Serial\n"));
   	sony_result = sony_dvb_demod_SetConfig(pDemod, DEMOD_CONFIG_PARALLEL_SEL, 0);
    if(sony_result != SONY_RESULT_OK){ return DRVAPI_TUNER_ERROR; }
 	#endif
    // Shared IF setting
    sony_result = sony_dvb_demod_SetConfig(pDemod, DEMOD_CONFIG_SHARED_IF, 1);
    if(sony_result != SONY_RESULT_OK){ return DRVAPI_TUNER_ERROR; }
    
    return DRVAPI_TUNER_OK;
}

INT32 DVBT2_TunerOpen(PTD_CTX ptTDCtx, TUNER_CONFIG_T * ptTunerCfgInfo, PTD_SPECIFIC_CTX* pptTDSpecificCtx)
{
//    ITUNER_CTX_T *tTunerCtx;

    sony_result_t sony_result = SONY_RESULT_OK;
    sony_dvb_demod_t *pDemod = NULL;
    sony_i2c_t *pI2c = NULL;
	vEnableMTKLog(1);//enable LOG 

    //create pdctx used in this file, you also can add any declaration you want in pdctx
    pPdCtxDVBT2 = (DVBT2_PD_CTX_T *) x_mem_calloc(1, sizeof(DVBT2_PD_CTX_T));
    if (pPdCtxDVBT2 == NULL)
    {
        mcDBG_MSG_L1(("DVB_PD_CTX_T malloc fail\n"));
        return (DRVAPI_TUNER_ERROR);
    }

    pSonyDemod = (DEMOD_CXD2834_CTX_T *) x_mem_calloc(1,sizeof(DEMOD_CXD2834_CTX_T));
    /* Init Disconnect Status */
    pPdCtxDVBT2->fgDisStatus = FALSE;

    pSonyDemod->autoPLP = 1;
    pDemod = &pSonyDemod->demod;
    pI2c = &pSonyDemod->i2c;

    //Create a semaphore for low level demod PI access protection
    if (x_sema_create(&pPdCtxDVBT2->hHalLock, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        mcDBG_MSG_L1(("DVB x_sema_create(pPdCtxDVBT2->hHalLock) Fail!\n"));
        return (DRVAPI_TUNER_ERROR);
    }
    x_sema_unlock(pPdCtxDVBT2->hHalLock);

    //register LTDIS APIs to HAL
    PD_DVBT2_Register_LTDIS_Fct(ptTDCtx);

    /* demod instance initialization (to do only once) */
    vResetDemod(100);

    // -------------------------------------------------------------------------
    // CXD2834 Specific Initialization
    // -------------------------------------------------------------------------

    // Initialize I2C struct used in CXD2834 driver
    sony_result = sony_i2c_MTK_CreateI2c(pI2c);
    if(sony_result != SONY_RESULT_OK){
		mcDBG_MSG_L1(("sony_i2c_MTK_CreateI2c Fail!\n"));
		return DRVAPI_TUNER_ERROR; }

    // Initialize CXD2834 driver demod struct instance
    // Xtal is 20.5MHz
    #ifdef SONY_DEMOD_CRYSTAL_41M
	mcDBG_MSG_L1(("sony demod crystal config 41Mhz\n"));
    sony_result = sony_dvb_demod_Create(SONY_DVB_41000kHz, CXD2834_DEMOD_I2C_ADDRESS, pI2c, pDemod);
	#else
	mcDBG_MSG_L1(("sony demod crystal config 20.5Mhz\n"));
	sony_result = sony_dvb_demod_Create(SONY_DVB_20500kHz, CXD2834_DEMOD_I2C_ADDRESS, pI2c, pDemod);
	#endif
	if(sony_result != SONY_RESULT_OK){
				mcDBG_MSG_L1(("sony_dvb_demod_Create Fail!\n"));
				return DRVAPI_TUNER_ERROR; }
	
	
	

    // IF frequency setting for CXD2834
    // This value is optimized for Sony silicon tuner ASCOT2D.
    pDemod->iffreq_config.config_DVBT5 = DVB_DEMOD_MAKE_IFFREQ_CONFIG(3.6);
    pDemod->iffreq_config.config_DVBT6 = DVB_DEMOD_MAKE_IFFREQ_CONFIG(3.6);
    pDemod->iffreq_config.config_DVBT7 = DVB_DEMOD_MAKE_IFFREQ_CONFIG(4.2);
    pDemod->iffreq_config.config_DVBT8 = DVB_DEMOD_MAKE_IFFREQ_CONFIG(4.8);

    pDemod->iffreq_config.config_DVBT2_5 = DVB_DEMOD_MAKE_IFFREQ_CONFIG(3.6);
    pDemod->iffreq_config.config_DVBT2_6 = DVB_DEMOD_MAKE_IFFREQ_CONFIG(3.6);
    pDemod->iffreq_config.config_DVBT2_7 = DVB_DEMOD_MAKE_IFFREQ_CONFIG(4.2);
    pDemod->iffreq_config.config_DVBT2_8 = DVB_DEMOD_MAKE_IFFREQ_CONFIG(4.8);

    pDemod->iffreq_config.config_DVBC = DVB_DEMOD_MAKE_IFFREQ_CONFIG(4.9);
    pDemod->iffreq_config.config_DVBC2_6 = DVB_DEMOD_MAKE_IFFREQ_CONFIG(3.7);
    pDemod->iffreq_config.config_DVBC2_8 = DVB_DEMOD_MAKE_IFFREQ_CONFIG(4.9);

    pDemod->is_ascot = DVB_TUNER_ASCOT2D;

    if(sony_InitializeDemod(pDemod) != DRVAPI_TUNER_OK){
		mcDBG_MSG_L1(("sony_InitializeDemod Fail!\n"));
       // return DRVAPI_TUNER_ERROR;
       return DRVAPI_TUNER_OK;//return error will cause system abort.
    }

    // -------------------------------------------------------------------------
    // CXD2834 Specific Initialization End
    // -------------------------------------------------------------------------

    /* tuner initialization */
    ITuner_Init(ITunerGetCtx());

/**demod initilize finish*****************************************/
	vEnableMTKLog(0);//disable LOG 

    return (DRVAPI_TUNER_OK);
}

// Sony CXD2834 Function trace (For Debug)
void sony_trace_log_enter(const char* funcname, const char* filename, unsigned int linenum)
{
    mcDBG_MSG_L4(("CXD2834_ENTER : %s (%d)\n", funcname, linenum));
}

void sony_trace_log_return(sony_result_t result, const char* filename, unsigned int linenum)
{
    if(result != SONY_RESULT_OK){
        mcDBG_MSG_L4(("CXD2834_ERROR_RETURN(%d) : (line %d)\n", result, linenum));
    }else{
        mcDBG_MSG_L4(("CXD2834_RETURN : (line %d)\n", linenum));
    }
}

// Function trace for I2C functions (For Debug)
void sony_trace_i2c_log_enter(const char* funcname, const char* filename, unsigned int linenum)
{
    sony_trace_log_enter(funcname, filename, linenum);
}
void sony_trace_i2c_log_return(sony_result_t result, const char* filename, unsigned int linenum)
{
    sony_trace_log_return(result, filename, linenum);
}
