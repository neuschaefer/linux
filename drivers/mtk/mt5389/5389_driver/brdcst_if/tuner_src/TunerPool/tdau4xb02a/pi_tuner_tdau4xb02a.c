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
 * $RCSfile: pi_tuner_tdau4xb02a.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_tuner_tdau4xb02a.c
 *  Tuner control for SS TDAU4XB02A
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
//#include "demod_tuner_interface.h"
//#include "pd_i2capi.h"
#include "tuner_interface_if.h"
#include "ctrl_bus.h"
#include "PI_Def.h"
#include "fe_tuner_common_if.h"
//#include "pd_tuner_type.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define ALPS_TDAU4XB02A_BASE            ((UINT16) 0xC2)
#define ALPS_TDAU4XB02A_IF_FREQUENCY    ((UINT16) 44000)   /* KHz */
#define ALPS_TDAU4XB02A_LO_STEP         ((UINT16) 3125)  /* 10Hz */
#define ALPS_TDAU4XB02A_ANA_LO_STEP         ((UINT16) 3125)  /* 10Hz */
#define ALPS_TDAU4XB02A_STR             "ALPS_TDAU4XB02A 2010-01-01\n 2011-04-09\n v1.0"

//-----------------------------------------------------------------------------
// pi  constant definition
//-----------------------------------------------------------------------------
//-------------------------------------------------------------staticvariable--------------------------------------
//Signal Power Samlpe Array
STATIC INT16 IFCTRL_VHFL[cIF_CTRL_NUM] =  {4096,  3938, 3088, 2491 , 2042, 1844,  1803, 1778, 1751,-70};//From -70dBm
STATIC INT16 IFCTRL_VHFH[cIF_CTRL_NUM] =  {4096,  3440 ,2757, 2243 , 1912, 1834,  1801, 1777, 1749,-70};//From -70dBm
STATIC INT16 IFCTRL_UHF[cIF_CTRL_NUM] =  {4096,  3297, 2649, 2159 , 1911, 1813,  1753, 1717, 1693,-65};//From -65dBm
//----------------------------------------------------------------------------------------------------
/** TDAU4XB02A_TunerInit
 *  Tuner initialzation forTDAU4XB02A.
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @param  i2cAddr     Tuner I2C address.
 *  @retval void
 */
//-----------------------------------------------------------------------------
VOID TDAU4XB02A_TunerInit(
    ITUNER_CTX_T *pTCtx
    )
{
    ITUNER_CTX_T *psTunerCtx = pTCtx;
    SPECIFIC_MEMBER_US_CTX *psTunerSpecificCtx=&(psTunerCtx->specific_member.us_ctx);

    /* Common part: All tuners need to do the following procedures */
    //Tuner_ParaInit(psTunerCtx);   // move to InitializeTunerCtx() (tuner_interface.c)
    psTunerCtx->I2cAddress = ALPS_TDAU4XB02A_BASE;
    psTunerCtx->u4RF_Freq = 0;

    /* Specific part: Only for this tuner model */
    psTunerCtx->u2IF_Freq = ALPS_TDAU4XB02A_IF_FREQUENCY;
    
   // psTunerCtx->u2Ana_LO_Step = psTunerCtx->u2LO_Step;

    psTunerSpecificCtx->m_aucPara[TUNER_FUNC_EN_TARGET_LEVEL] = TRUE;
    psTunerSpecificCtx->m_aucPara[TUNER_PARA_TARGET_LEVEL_MSB] = 0x3D;
    psTunerSpecificCtx->m_aucPara[TUNER_PARA_TARGET_LEVEL_LSB] = 0x00;   
   
    //IFAGC sample array
    //psTunerSpecificCtx->pSignalLvlArray[TUNER_SIGNAL_LVL_ARRAY_VHF][TUNER_SIGNAL_LVL_ARRAY_8VSB]= ;//8VSB VHF band
    
    mcSHOW_DBG_MSG(("Init " ALPS_TDAU4XB02A_STR "\n"));
}


//----------------------------------------------------------------------------- 
/** TDAU4XB02A_TunerSetFreq
 *  Set Tuner PLL forTDAU4XB02A/TDAU4XB02A to tune RF frequency.
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @param 	param    Pointer of structure of parameters
 *  @retval  0 success
                    1  fail   out of range 
                   -1 fail  I2C error 
 */
//-----------------------------------------------------------------------------
INT16 TDAU4XB02A_TunerSetFreq(
    ITUNER_CTX_T *pTCtx,
    PARAM_SETFREQ_T* param
    )
{
ITUNER_CTX_T *psTunerCtx = pTCtx; 
UINT32  Lo;
UINT8   Nband;
UINT16  Ndivider;
BOOL 	 fgAnalogSignal = FALSE;
UINT8   TunerS[5], TunerDataCnt;
UINT32 freq;

   mcSHOW_DBG_MSG(("ALPS TDAU_TunerSetFreq %dKHz\n",param->Freq));
  if (param->Modulation >= MOD_ANA_TYPE_BEGIN )
        fgAnalogSignal = TRUE;

    TunerDataCnt = 5;
    freq=param->Freq;
    if(fgAnalogSignal)freq+=1750;//analog IF 45.75MHz
    
    Lo = freq+ psTunerCtx->u2IF_Freq;
    
    if(!fgAnalogSignal){//digital  mode use digital LO step
      Ndivider = (UINT16) ((Lo * 100) / ALPS_TDAU4XB02A_LO_STEP);
    }
    else{
     Ndivider = (UINT16) ((Lo * 100) /ALPS_TDAU4XB02A_ANA_LO_STEP);
    }
    freq = freq /1000;//convert to unit MHZ

/* Assign Control word for specific tuner */
    if (freq < 53 || freq > 865)
    {
       mcSHOW_HW_MSG(("Out of range for LO!\n"));
       return (+1);
    }
    if (freq < 138)
        Nband = 0x80;
    else if (freq < 378)
        Nband = 0x83;                       
    else if (freq < 738)
        Nband = 0x48;
    else
        Nband = 0xC8;

    
/* LO settings */
    TunerS[0] = (UINT8) 0x7F & (Ndivider >> 8);
    TunerS[1] = (UINT8) 0xFF & Ndivider;
    TunerS[2] = 0x9C;
    TunerS[3] = Nband;
    TunerS[4] = fgAnalogSignal?0xE6:0xC2;
    
 /* Write to tuner device */
    if (ICtrlBus_I2cTunerWrite(270,psTunerCtx->I2cAddress, TunerS, TunerDataCnt))
    {
        mcSHOW_HW_MSG(("TunerWrite PLL failed!\n"));
        return (-1);
    }
    else//I2C bus  connect
    {
        if(fgAnalogSignal){
            psTunerCtx->u4RF_Freq=(Ndivider*ALPS_TDAU4XB02A_ANA_LO_STEP)/100-psTunerCtx->u2IF_Freq-1750;
        }
        else{
            psTunerCtx->u4RF_Freq=(Ndivider*ALPS_TDAU4XB02A_LO_STEP)/100-psTunerCtx->u2IF_Freq;
        }
        mcSHOW_DBG_MSG(("LO_%02X: (%d) %02X-%02X-%02X-%02X-%02X(%dKHz)\n",psTunerCtx->I2cAddress, TunerDataCnt, TunerS[0], TunerS[1], TunerS[2], TunerS[3],TunerS[4],psTunerCtx->u4RF_Freq));
    }

    mcDELAY_MS(10);

    return (0);
}

//----------------------------------------------------------------------------- 
/**  TDAU4XB02A_TunerGetStatus
 *  Set Tuner PLL staus forTDAU4XB02A/TDAU4XB02A
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @retval TRUE        PLL lock
 *  @retval FALSE       PLL unlock
 */
//-----------------------------------------------------------------------------
STATIC BOOL TDAU4XB02A_TunerGetStatus(ITUNER_CTX_T *pTCtx)
{
    ITUNER_CTX_T *psTunerCtx = pTCtx;

    UINT8 TunerS[4];

    if (ICtrlBus_I2cTunerRead(270,psTunerCtx->I2cAddress, TunerS, 1))
    {
        mcSHOW_HW_MSG(("Tuner read failed!\n"));
        return FALSE;
    }

    if (mcTEST_BIT(TunerS[0], 6))
        return TRUE;
    mcSHOW_HW_MSG(("PLL Unlock\n"));
    return FALSE;
}

//----------------------------------------------------------------------------- 
/** TDAU4XB02A_TunerGetStatus
 *  Set Tuner PLL staus forTDAU4XB02A/TDAU4XB02A
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @retval tuner model name
 */
//-----------------------------------------------------------------------------
STATIC CHAR *TDAU4XB02A_TunerGetVer(VOID)
{
    return (ALPS_TDAU4XB02A_STR);
}
/***********************************************************************/
//----------------------------------------------------------------------------- 
/** TDAU4XB02A_SignalLevel_Handler
 *  Get IFAGC Releated Value,ex:SignalLeveldBm,SignalLevel,Vol2Ctrl,Ctrl2Vol
 *  @param  rCmd transfer parameter in such as Freq and Modulation
 *  @param  Retval  In/Out parameter
 *  @retval TRUE Get successful
                  FALSE Found Error
 */
//-----------------------------------------------------------------------------
STATIC INT16 TDAU4XB02A_SignalLevel_Handler(ITUNER_CTX_T * pTCtx,CMD_SIGLVL_PARAM_T * rCmd){
    INT16 ret=ITUNER_OK;
     INT32 * Retval=&(rCmd->IOpara);
     
     switch(rCmd->Id){
            case IFAGCCTRL2VOL://Covert IfAGC Ctrl Word to Voltage
            {
             INT32 i4CtrlWord=*Retval;
             INT32 Freq=rCmd->Freq;
             if (i4CtrlWord > 4000)
             {
             *Retval= 2600;	
	         }
	         else
            {
    	    if (Freq<= 165000 ) // VHF-Low band
            {
            *Retval = 717 * (i4CtrlWord - 4016) / 2238 + 2544;
            }
	        else if (Freq <=465000 ) // VHF-High band
            {
            *Retval = 493 * (i4CtrlWord - 3314) / 1548 + 2317;
            }
  	        else // UHF band
            {
            *Retval= 672 * (i4CtrlWord - 3898) / 2107 + 2505;
            }
            }
            }
            break;
            case IFAGCVOL2CTRL://Convert IfAGC Voltage to Ctrl Word
            {
               INT32 i4Vol=*Retval;
               if ( i4Vol > 2500 ){
                   *Retval= 4096;
               }
               else{
                   *Retval = 1544 * (i4Vol - 2315) / 495 + 3297;
               }
            }
             
            break;
            
            case IFAGCCTRL2PWR://Conert IfAGC Ctrl Word to Signal Level dBm
            {
               INT16 *pIFCtrl=NULL;
               INT16 i2RFPwr = 0;
               INT32 Freq=rCmd->Freq;
               INT16 i2Low_Pwr_Level=0;
               INT32 i2CtrlWord=*Retval;
               UINT8 valid_len=cIF_CTRL_NUM-1;//not include lower power
               UINT8 ii = 0;
               INT16 i2Value_0, i2Value_1;
               //Check if Ctrl Word is negative 14bits
               if(mcTEST_BIT(i2CtrlWord, 13)){//if ctrl word negative
                 i2CtrlWord=~i2CtrlWord;
		         i2CtrlWord++;
		         i2CtrlWord&=0x1FFF;
		         i2CtrlWord*=-1;
                }
                //Get Address of Sample Array                
               if(Freq<= 168000){
                    pIFCtrl=IFCTRL_VHFL;
               }
               else if(Freq<= 465000){
                    pIFCtrl=IFCTRL_VHFH;
               }
               else{
                   pIFCtrl=IFCTRL_UHF;
               }
               //Check Valid length of Array
               if(pIFCtrl!=NULL){
                   for(ii=cIF_CTRL_NUM-2;ii!=0;ii--){
                    if(pIFCtrl[ii-1]==pIFCtrl[cIF_CTRL_NUM-2])valid_len--;
                    else break;		   
                }
	           }
               else{//pIFCtrl is null!
                   ret=ITUNER_NOT_OK;
                   break;
               }
               
                i2Low_Pwr_Level=pIFCtrl[cIF_CTRL_NUM-1]*cPWR_CONST;//Base power
                
                 if (i2CtrlWord >= pIFCtrl[0]) // Larger than Max IFCtrl
                     i2RFPwr = i2Low_Pwr_Level;
                 else if (i2CtrlWord <= pIFCtrl[valid_len - 1]) // Smaller than Min IFCtrl
                     i2RFPwr = i2Low_Pwr_Level + cPWR_INTERVAL*(valid_len - 1);
                  else
                  {
          // Linear Inner-Interpolation
                      for (ii = 0; ii < (valid_len - 1); ii++)
                       {
                         if ((i2CtrlWord <= pIFCtrl[ii]) && (i2CtrlWord > pIFCtrl[ii + 1]))
                          {
                            i2Value_0 = pIFCtrl[ii];
                            i2Value_1 = pIFCtrl[ii + 1];
                            i2RFPwr = i2Low_Pwr_Level + cPWR_INTERVAL * ii;
                            i2RFPwr = i2RFPwr + cPWR_INTERVAL * (i2CtrlWord - i2Value_0) / (i2Value_1 - i2Value_0);
                            break;
                          }
                        }
                 }
                i2RFPwr = i2RFPwr/cPWR_CONST;//Signal Power
                *Retval=(INT32)i2RFPwr;
            }
            break;
            
            case IFAGCCTRL2LVL://Convert IfAGC Ctrl Word to SignalLevel
            {
              INT32 Freq=rCmd->Freq;
              INT32 i2RFPwr=*Retval;
              INT16 i2Low_Pwr_Level=0;
            // RF Signal Strength (0-100)
             if  (Freq > 465000)
	          {
                  if (i2RFPwr > -65)
	                  i2Low_Pwr_Level = -70 * cPWR_CONST;
		          if (i2RFPwr > -29)
	                    i2RFPwr -= 5;
               }
              *Retval =(cLOW_SIG_STRENGTH +cSIG_INTERVAL * ( i2RFPwr - (i2Low_Pwr_Level / cPWR_CONST )));
             }
            break;
            default:
            mcSHOW_USER_MSG(("Unknown operation acquisition id %d",rCmd->Id));
            ret=ITUNER_NOT_SUPPORTED;
            break;
     }
     return ret;
}
//----------------------------------------------------------------------------- 
/** TDAU4XB02A_TunerOP
 * Set/Get function
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @param  eOperation       To Get/Set operation id
 *  @retval  ITUNER_OK    Operation Success
 *  @retval  ITUNER_NOT_OK    Operation fail
 *  @retval  ITUNER_NOT_SUPPORTED Operation not supported in  driver
 */
//-----------------------------------------------------------------------------
INT16 TDAU4XB02A_TunerOP(ITUNER_CTX_T * pTCtx, ITUNEROP_T eOperation, UINT8 SetVal,  VOID * pInOutVal){
         
         switch(eOperation){
              case itGetStatus:
                {
                    BOOL * Retval=(BOOL *)pInOutVal;
                    *Retval=TDAU4XB02A_TunerGetStatus(pTCtx);
                }
                break;
              case itGetVer:
                 {
                    CHAR ** RetStr=(CHAR **)pInOutVal;
                    *RetStr=TDAU4XB02A_TunerGetVer();
                 }
                 break;
              case itGetSignalLevel:
                 {
                   CMD_SIGLVL_PARAM_T * SigLvlParam=(CMD_SIGLVL_PARAM_T *)pInOutVal;
                   return TDAU4XB02A_SignalLevel_Handler(pTCtx,SigLvlParam);
                 }
              default:
                 return ITUNER_NOT_SUPPORTED;
        }
         return ITUNER_OK;
}
