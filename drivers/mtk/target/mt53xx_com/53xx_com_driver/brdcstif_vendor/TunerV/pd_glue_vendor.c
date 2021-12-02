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
//#include "drvapi_tuner.h"
//#include "TDIS.h"
//#include "TDMgr.h"
#include "pd_glue_vendor.h"
#include "TDIS_header_forV.h"
#include "u_rm.h" //need DRV_CONN_TYPE_T
#include "nonImplementPDAPI.h"
#include "u_tuner.h"
#include "x_tuner.h"
#include "tunerDebug.h"

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define mcSEMA_LOCK_MTFE(sema)                                       \
{                                                                    \
    mcDBG_MSG_L4((" [ _Lock_%s-%s-%d ]\r\n", __FUNCTION__, __LINE__)); \
    x_sema_lock(sema, X_SEMA_OPTION_WAIT);                           \
    mcDBG_MSG_L4((" [ _Excape_%s-%s-%d ]\r\n", __FUNCTION__, __LINE__));     \
}
#define mcSEMA_UNLOCK_MTFE(sema)                                     \
{                                                                    \
    mcDBG_MSG_L4((" [ _Unlock_%s-%s-%d ]\r\n", __FUNCTION__, __LINE__));     \
    x_sema_unlock(sema);                                             \
}

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static DVB_PD_CTX_T  *pPdCtx;

//-----------------------------------------------------------------------------
// Static Functions
//-----------------------------------------------------------------------------
//-------------------------------------------------------------------------
/**
 *  This API implement close tuner. It should performs
 *  1. free all allocated resource.
 *  2. do demod/tuner initialization.
 *  3. do LTDIS API mapping
 *  @param   ptTDSpecificCtx   Pointer to tuner driver context, there is no use for third parity tuner.
 *  @retval  DRVAPI_TUNER_OK    Success
 *  @retval  DRVAPI_TUNER_ERROR Fail
 */
//-------------------------------------------------------------------------
static INT32 Vendor_TunerClose(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);

    //add driver implementation code here
    x_sema_delete(pPdCtx->hHalLock);
    x_mem_free(pPdCtx);

    //If you have allocated any resource in TunerOpen(), release them here.

    return (DRVAPI_TUNER_OK);
}
//-------------------------------------------------------------------------
/**
 *  This API do channel acuqusition.
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context, there is no use for third parity tuner.
 *  @param   e_conn_type	tuner connection type, refer x_tuner.h	
 *  @param   pv_conn_info	middleware tuner info structure, refer u_tuner.h
 *  @param   z_conn_info_len    length of pv_conn_info 
 *  @param   _BreakFct 		TunerHAL callback API for Vendor_TunerAcq to check whether need to break TunerAcq
 *  @param   *pvArg		pass this parameter to _BreakFct when calling _BreakFct
 *  @retval  1			TPS LOCK
 *  @retval  0             	TPS UNLOCK
 */
//-------------------------------------------------------------------------
static BOOL Vendor_TunerAcq(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type, void* pv_conn_info,
                            SIZE_T z_conn_info_len, x_break_fct _BreakFct, void* pvArg)
{
    BOOL    fgTPSLockStatus=0;
    //UINT32  u4Freq;
//    UINT8   fgChannelScan = FALSE;
    //please check whether middleware will change TUNER_TER_DIG_TUNE_INFO_T definition for DVB-C implementation 
    TUNER_TER_DIG_TUNE_INFO_T *pt_tuner_info = (TUNER_TER_DIG_TUNE_INFO_T *) pv_conn_info;


    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);

    //middleware's definition, if e_mode=MOD_UNLNOWN, middleware is doing channel scan mode 
    //otherwise it is doing change change 
    if(pt_tuner_info->e_mod == MOD_UNKNOWN){  
//        fgChannelScan = TRUE;
    }

    //add driver implementation code here
    //u4Freq = pt_tuner_info->ui4_freq/1000; //example of get freq parameter from middleware and convert to KHz

    //set tuner saw bw according to demod setting
    //if DVB-C don't need to set saw bw, just remove these code
    switch(pt_tuner_info->e_bandwidth) //get bandwidth setting from middleware 
    {
    case BW_6_MHz:
    //add Vendor PI API here: set bandwidth API
        break;
    case BW_7_MHz:
        break;
    case BW_8_MHz:
        break;
    default:
        break;
    }

    //set frequency to tuner

    //add Vendor PI API here : set parameter to demod? 


#if 0 //reference sample code about how to use _BreakFct
    for (ii = 0; ii < 100; ii ++)
    {
	//PI may add check lock status API in a loop,

        if(_BreakFct(pvArg))  //give high level software a chance to break out for loop if this loop take too long 
        {
            mcDBG_MSG_L2((" PD break happen\r\n"));
	    fgTPSLockStatus =0;
	    break;
        }
        x_thread_delay(10);
    }
#endif

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);

	if(fgTPSLockStatus){
  	    mcDBG_MSG_L2((" TPS Lock\r\n"));	
	}
	else{
  	    mcDBG_MSG_L2((" TPS UNLock\r\n"));	
	}

    return fgTPSLockStatus;//if lock return 1, unlock return 0
}

//-------------------------------------------------------------------------
/**
 *  This API check lock status
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context, there is no use for third parity tuner.
 *  @param   e_conn_type        tuner connection type, refer x_tuner.h
 *  @retval  1    		LOCK
 *  @retval  0	                UNLOCK
 */
//-------------------------------------------------------------------------
static INT16 Vendor_TunerGetSync(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    UINT16 u2Ret=0;

    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
    //add driver implementation code here

    //Vendor PI APIs add check lock status API here 

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
    return u2Ret;
}

//-------------------------------------------------------------------------
/**
 *  This API do disconnect tuner 
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context, there is no use for third parity tuner.
 *  @param   e_conn_type        tuner connection type, refer x_tuner.h
 *  @retval  void 
 */
//-------------------------------------------------------------------------
static VOID  Vendor_TunerDisc(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
    //add driver implementation code here

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
}
 
//-------------------------------------------------------------------------
/**
 *  This API get modulation information.  
 *  Inputs:
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context, there is no use for third parity tuner.
 *  @param   e_conn_type        tuner connection type, refer x_tuner.h
 *  Outputs:
 *  @param   *_pSignal          Modulation info. structure to be filled.
 *  @retval  void 
 */
//-------------------------------------------------------------------------
static void Vendor_TunerGetSignal(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type, SIGNAL *_pSignal)
{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
    //add driver implementation code here

#if 0
    _pSignal->Frequency = get from Vendor PI APIs
    _pSignal->SymbolRate = 0;
    _pSignal->QAMSize = 0;
    switch (get from Vendor PI APIs)
    {
	case 0:  _pSignal->e_bandwidth = BW_6_MHz;   break;
	case 1:  _pSignal->e_bandwidth = BW_7_MHz;   break;
	case 2:  _pSignal->e_bandwidth = BW_8_MHz;   break;
	case 3:
	default: _pSignal->e_bandwidth = BW_UNKNOWN; break;
    }
    //define in u_tuner.h TUNER_HIERARCHY_PRIORITY_T
    _pSignal->e_hierarchy_priority = HIR_PRIORITY_HIGH/HIR_PRIORITY_LOW/HIR_PRIORITY_NONE;
#endif

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);

}

//-------------------------------------------------------------------------
/**
 *  This API get signal level in terms of percentage.  
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context, there is no use for third parity tuner.
 *  @param   e_conn_type        tuner connection type, refer x_tuner.h
 *  @retval  0~100    		signal level	
 */
//-------------------------------------------------------------------------
static UINT8 Vendor_TunerGetSignalLevel(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    UINT8 u1Ret=0;
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
    //add driver implementation code here
    //u1Ret = Vendor PI get signal level, return vlaue should be range from 0 ~ 100

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
    //example of return 
    return u1Ret;
}

//-------------------------------------------------------------------------
/**
 *  This APi get signal level in multiples of (0.1dBm). 
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context, there is no use for third parity tuner.
 *  @param   e_conn_type        tuner connection type, refer x_tuner.h
 *  @retval  0 ~ -900           signal level
 */
//-------------------------------------------------------------------------
static INT16 Vendor_TunerGetSignalLeveldBm(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    INT16 i2Ret=0;
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
    //add driver implementation code here
    //i2Ret = Vendor PI get signal level in dBm, return vlaue should be range from 0 ~ -900

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
    return i2Ret;
}

//-------------------------------------------------------------------------
/**
 *  This API get attribute from tuner driver
 *  Inputs:
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context, there is no use for third parity tuner.
 *  @param   e_get_type         tuner get type, refer x_tuner.h TUNER_GET_TYPE_XXXXX
 *  Outputs:
 *  @param   *pzAttribute    	Attribute structure to be filled. Refer x_tuner.h structure definition TUNER_XXXXX_ATTRIBUTE_T
 *  @param   *pzAttributeLen	length to be filled.
 *  @retval  DRVAPI_TUNER_OK	success. 
 *  @retval  DRVAPI_TUNER_ERROR fail.
 */
//-------------------------------------------------------------------------
static INT16 Vendor_TunerGetAttribute(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_GET_TYPE_T  e_get_type, 
				VOID *pzAttribute, SIZE_T* pzAttributeLen)
{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);

    //add driver implementation code here
    switch (e_get_type)
{
    case TUNER_GET_TYPE_BER: //get attribute depend on what attribute middleware need to know. Here just give an example.
        *(INT32*)pzAttribute =0;// Get BER from Vendor PI APIs
        *pzAttributeLen = sizeof(INT32);
        break;
     case TUNER_GET_TYPE_SIGNAL_STATUS://Get Demod lock status
         *(UINT8*)pzAttribute =0;// Get lock status from demod //1 means demod lock signal.0 means demod unlock
        *pzAttributeLen = sizeof(UINT8);
        break;
     default:
        mcDBG_MSG_L2(("unknown: get Attribute\r\n"));
        break;
    }

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
    return DRVAPI_TUNER_OK;
}

//-------------------------------------------------------------------------
/**
 *  This API set attribute to tuner driver 
 *  Inputs:
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context, there is no use for third parity tuner.
 *  @param   e_set_type         tuner set type, check x_tuner.h set operation TUNER_SET_TYPE_XXXX
 *  @param   *pvAttribute       set value.	
 *  @param   zAttributeLen	length of *pvAttribute.
 *  @param   e_conn_type        tuner connection type, refer x_tuner.h
 *  @retval  DRVAPI_TUNER_OK	success. 
 *  @retval  DRVAPI_TUNER_ERROR fail.
 */
//-------------------------------------------------------------------------
static INT32 Vendor_TunerSetAttribute(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_SET_TYPE_T  e_set_type,
                           		const VOID *pvAttribute, SIZE_T zAttributeLen)
{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
    //add driver implementation code here
#if 0
    switch (e_set_type)
    {
//set attribute depend on what attribute middleware need to set to driver. Here just give an example
    case TUNER_SET_TYPE_CI_CARD_TYPE:
        break;

    default:
        Printf("unknown: set Attribute\r\n");
        break;
    }
#endif

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
    return DRVAPI_TUNER_OK;
}
//-------------------------------------------------------------------------
/**
 *  This API return the PER from digital demod
 *  Inputs:
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context, there is no use for third parity tuner.
 *  @param   e_conn_type        tuner connection type,please skip it
 *  @retval  PER(unit is 1e-5)
 */
//-------------------------------------------------------------------------
static UINT32 Vendor_TunerGetSignalPER (PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T  e_conn_type)
{
    INT16 i2Ret=0;
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
    //add driver implementation code here
    //i2Ret = Vendor PI get PER , return vlaue should be in terms of 1e-5

    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
    return i2Ret;
}
//-------------------------------------------------------------------------
/**
 *  This API return the SNR from digital demod
 *  Inputs:
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context, there is no use for third parity tuner.
 *  @param   e_conn_type        tuner connection type,please skip it
 *  @retval  SNR
 */
//-------------------------------------------------------------------------
static UINT16 Vendor_TunerGetSignalSNR (PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T  e_conn_type)
{
    INT16 i2Ret=0;
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
    //add driver implementation code here
    //i2Ret = Vendor PI get SNR 
    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
    return i2Ret;
}
//-------------------------------------------------------------------------
/**
 *  This API return the SNR from digital demod
 *  Inputs:
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context, there is no use for third parity tuner.
 *  @param   i4Argc             The count of arguments
 *  @retval  aszArgv          The buffer of arguments
 */
//-------------------------------------------------------------------------
static void Vendor_TunerDTDCommand(PTD_SPECIFIC_CTX  	ptTDSpecificCtx,INT32 i4Argc,const char ** aszArgv)
{
	 
	  UINT8  ucCmdId = 0;

    if(i4Argc<1)
    	{
    	Printf("\t s:                   show signal/ts/tps status\n");	
    	}
   mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
   if (i4Argc > 0)
        ucCmdId = *((CHAR *) aszArgv[0]);
        
    switch (ucCmdId)
    {
     	case 's':
     	    // Printf(("Front-end Lock = %d\n", ISDBT_GetSync(psDemodCtx)));
          
            break;
      // case бн
//add driver implementation code here
//add other cmd code here

        default:
            break;
}    

   mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
}
//-------------------------------------------------------------------------
/**
*  This API  control  the i2c bypass to tuner. When zap from digital to ananlog,we need this api to bypass i2c to tuner. 
* Inputs:
* @param   ptTDSpecificCtx  Pointer to tuner driver context, there is no use for third party tuner.  
* @param    bSwitchOn        TRUE(bypass i2c to tuner on).
                             FALSE(bypass i2c to tuner off) 
* @Retval                    void
*/
//-------------------------------------------------------------------------
static void Vendor_TunerBypassI2C (PTD_SPECIFIC_CTX ptTDSpecificCtx, BOOL bSwitchOn)

{
    mcSEMA_LOCK_MTFE(pPdCtx->hHalLock);
    //add driver implementation code here


    mcSEMA_UNLOCK_MTFE(pPdCtx->hHalLock);
}

//-------------------------------------------------------------------------
/**
 *  This API register LTDIS APIs for TunerHAL TDMgr 
 *  @param   ptTDCtx  this structure include LTDIS function table pointer 
 *  @retval  void 
 */
//-------------------------------------------------------------------------
static void PD_Vendor_Register_LTDIS_Fct(PTD_CTX ptTDCtx)
{
    TUNER_DEVICE_CTX_T*  pTunerDeviceCtx = (TUNER_DEVICE_CTX_T*)ptTDCtx;

    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerClose 	= Vendor_TunerClose;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerAcq 	= Vendor_TunerAcq;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGerSync 	= Vendor_TunerGetSync;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignal 	= Vendor_TunerGetSignal;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalLevel = Vendor_TunerGetSignalLevel;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalLeveldBm = Vendor_TunerGetSignalLeveldBm;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalPER = Vendor_TunerGetSignalPER;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalSNR = Vendor_TunerGetSignalSNR;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDtdCommand 	= Vendor_TunerDTDCommand;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDisc 	= Vendor_TunerDisc;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetAttribute= Vendor_TunerSetAttribute;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetAttribute= Vendor_TunerGetAttribute;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl. pf_TunerBypassI2C= Vendor_TunerBypassI2C;
    //NonImplement apis
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetMpgFmt 	= NonImplement_TunerSetMpgFmt;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetVer 	= NonImplement_TunerGetVer;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerNimTest 	= NonImplement_TunerNimTest;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetRegSetting = NonImplement_TunerSetRegSetting;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerShowRegSetting = NonImplement_TunerShowRegSetting;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerTestI2C 	= NonImplement_TunerTestI2C;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerCommand 	= NonImplement_TunerCommand;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDetachI2C 	= NonImplement_TunerDetachI2C;
 #ifdef fcADD_DIAG_INFO
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetDiagCmd	= NonImplement_TunerSetDiagCmd;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetDiagInfo = NonImplement_TunerGetDiagInfo;
#endif
}
//-------------------------------------------------------------------------
/**
 *  This API is driver entry point. It performs 
 *  1. allocate and initialize resource.
 *  2. do demod/tuner initialization.
 *  3. do LTDIS API mapping
 *  @param   ptTDCtx		TunerHAL Context use for LTDIS API mapping
 *  @param   ptTunerCfgInfo	Tuner configuration information for MTK, there is no use for third parity tuner.
 *  @param   pptTDSpecificCtx   Pointer to tuner driver context, there is no use for third parity tuner.
 *  @retval  DRVAPI_TUNER_OK    Success 
 *  @retval  DRVAPI_TUNER_ERROR Fail 
 */
//-------------------------------------------------------------------------
INT32 Vendor_TunerOpen(PTD_CTX ptTDCtx, TUNER_CONFIG_T * ptTunerCfgInfo, PTD_SPECIFIC_CTX* pptTDSpecificCtx)
{

    //create pdctx used in this file, you also can add any declaration you want in pdctx
    pPdCtx = (DVB_PD_CTX_T *) x_mem_alloc(sizeof(DVB_PD_CTX_T));
    if (pPdCtx == NULL)
    {
        Printf("DVB_PD_CTX_T malloc fail\n");
        return (DRVAPI_TUNER_ERROR);
    }

    //Create a semaphore for low level demod PI access protection
    if (x_sema_create(&pPdCtx->hHalLock, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        Printf("%s: DVB x_sema_create(pPdCtx->hHalLock) Fail!\n");
        return (DRVAPI_TUNER_ERROR);
    }
    x_sema_unlock(pPdCtx->hHalLock);

    //register LTDIS APIs to HAL
    PD_Vendor_Register_LTDIS_Fct(ptTDCtx);
    
    //todo: add tuner initialize API here.

    //todo: add demod initialize API here.

    return (DRVAPI_TUNER_OK);
}

