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
/*  $RCSfile: pd_api.c,v $                                             */
/*                                                                     */
/***********************************************************************/
/***********************************************************************/
/*              Includes                                               */
/***********************************************************************/
#include "PI_Def_dbg.h"
#include "pi_demod.h"
#include "dtd_if.h"
#include "pd_api.h"

#include "TDMgr.h"

#include "sif_if.h"                         // For SIF_NIMReadNoSubAddr & SIF_NIMWriteNoSubAddr

#include "UtilAPI.h" // Move fromd pd_api_dbg.c

//#include "ctrl_bus.h"

/***********************************************************************/
/*              Defines                                                */
/***********************************************************************/
//#define fcRISC_DIRECT_RW        1

/***********************************************************************/
// Changed from 105k to 100k.
//#define cI2C_TIMING             0x100
//#define cI2C_TIMING             270

#define cMAX_I2C_LEN_HW         8
#define c8297_DEV_ADDR      0x30

EXTERN volatile UINT32 *pu4DTD_Reg;

#ifndef fcEXTERN_DEMOD
PARAM_RISC_T sRiscCtx;
#endif
/***********************************************************************/
#define mcSHOW_TIME_MSG         mcSHOW_DBG_MSG2


/***********************************************************************/
/*              OS-Specific Functions                                  */
/***********************************************************************/
/***********************************************************************/
/*              Hardware-Specific Functions                            */
/***********************************************************************/
/***********************************************************************/
/*              I2C Interface                                          */
/***********************************************************************/
//-------------------------------------------------------------------------
/** u1I2cWriteSpeed
 *  @retval 0          Success
 *  @retval 1          Fail
 */
//-------------------------------------------------------------------------
UINT8 u1I2cWriteSpeed(VOID *psI2cCtx, UINT16 u2I2cClkDiv, UINT8 u1I2cAddr,
                      UINT8 u1RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount)
{
UINT8 u1Ret;

    #ifdef fcEXTERN_DEMOD
    u1Ret = ICtrlBus_I2cDemodWrite(u2I2cClkDiv, u1I2cAddr, u1RegAddr, pu1Buffer, u2ByteCount );
    #else
    sRiscCtx.pu4RegBase = pu4DTD_Reg;
    u1Ret = ICtrlBus_Risc_LoadCode(&sRiscCtx, u1RegAddr, pu1Buffer, u2ByteCount);
    #endif
    
    return u1Ret;
}


/***********************************************************************/
UINT8 u1I2cMT8297_SetReg(UINT8 u1Offset, UINT8 u1Data)
{
#ifdef CC_MT5391
     if (SIF_MT8297_DirectWrite8(u1Offset, u1Data) == -1 )
     {
        mcSHOW_HW_MSG(("SIF_MT8297_DirectWrite8 failed\n"));
        return 1;
     }
     mcSHOW_DBG_MSG(("SIF_MT8297_DirectWrite8(%02X) = %02X\n", u1Offset, u1Data)); 
#endif /* CC_MT5391 */
    return 0;                               // TRUE
    	     
}


/***********************************************************************/
UINT8 u1I2cMT8297_GetReg(UINT8 u1Offset, UINT8* pu1Buffer)
{
#ifdef CC_MT5391
     INT8 i1Value;

     i1Value = SIF_MT8297_DirectRead8(u1Offset);
     if (i1Value == -1 )
     {
        mcSHOW_HW_MSG(("SIF_MT8297_DirectRead8 failed\n"));
        return 1;
     }
     else
     {
        *pu1Buffer = i1Value;
        mcSHOW_DBG_MSG(("SIF_MT8297_DirectRead8(%02X) = %02X\n", u1Offset, *pu1Buffer ));
     }
#endif /* CC_MT5391 */
 
    return 0;                               // TRUE
    	     
}

/***********************************************************************/
UINT8 u1I2cMT8297_InDirect_SetReg(UINT8 u1Offset, UINT8 u1Data)
{
#ifdef CC_MT5391
     if (SIF_MT8297_InDirectWrite8(u1Offset, u1Data) < 0 )
     {
        mcSHOW_HW_MSG(("SIF_MT8297_InDirectWrite failed\n"));
        return 1;
     }
     mcSHOW_DBG_MSG(("SIF_MT8297_InDirectWrite(%02X) = %02X\n", u1Offset, u1Data)); 
#endif /* CC_MT5391 */
    return 0;                               // TRUE
    	     
}

/***********************************************************************/
UINT8 u1I2cMT8297_SetRegMask(UINT8 u1Offset, UINT8 u1Data, UINT8 u1Msk)
{
#ifdef CC_MT5391
     if (SIF_MT8297_DirectWrite8Msk(u1Offset, u1Data, u1Msk) < 0 )
     {
        mcSHOW_HW_MSG(("SIF_MT8297_DirectWrite8Msk failed\n"));
        return 1;
     }
     mcSHOW_DBG_MSG(("SIF_MT8297_DirectWrite8Msk(%02X, mask%02X) = %02X\n", u1Offset, u1Msk, u1Data)); 
#endif /* CC_MT5391 */
    return 0;                               // TRUE
    	     
}

/***********************************************************************/
#ifdef fcEXTERN_DEMOD
static UINT16 _u1HostReadRsp_ExtI2c(DEMOD_CTX_T *psDemodCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT8 u1ParaCnt, UINT8 u1ParaOfst)
{
    return DTD_GetReg(psDemodCtx, u2RegAddr + u1ParaOfst,
    	              pu1Buffer + u1ParaOfst, u1ParaCnt + 1);    
}
#endif

UINT16 u1HostReadRsp(DEMOD_CTX_T *psDemodCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT8 u1ParaCnt, UINT8 u1ParaOfst)	
{
UINT16 u2Ret;

#ifdef fcEXTERN_DEMOD
    u2Ret = _u1HostReadRsp_ExtI2c(psDemodCtx, u2RegAddr, pu1Buffer, u1ParaCnt, u1ParaOfst);
#else
    sRiscCtx.pu4RegBase = pu4DTD_Reg;
    u2Ret = (UINT16)ICtrlBus_Risc_HostReadRsp(&sRiscCtx, u2RegAddr, pu1Buffer, u1ParaCnt, u1ParaOfst);
#endif
    return u2Ret;
}


#ifdef fcEXTERN_DEMOD
static UINT16 _u1HostWriteCmd_ExtI2c(DEMOD_CTX_T *psDemodCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT8 u1ParaCnt, UINT8 u1ParaOfst)
{
    return DTD_SetReg(psDemodCtx, u2RegAddr + u1ParaOfst,                             
      	    pu1Buffer + u1ParaOfst + 1, u1ParaCnt + 1);
}
#endif

UINT16 u1HostWriteCmd(DEMOD_CTX_T *psDemodCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT8 u1ParaCnt, UINT8 u1ParaOfst)
{
UINT16 u2Ret;


#ifdef fcEXTERN_DEMOD
    u2Ret = _u1HostWriteCmd_ExtI2c(psDemodCtx, u2RegAddr, pu1Buffer, u1ParaCnt, u1ParaOfst);
#else
    sRiscCtx.pu4RegBase = pu4DTD_Reg;
    u2Ret = (UINT16)ICtrlBus_Risc_HostWriteCmd(&sRiscCtx, u2RegAddr, pu1Buffer, u1ParaCnt, u1ParaOfst);
#endif
    return u2Ret;
}


/***********************************************************************/

//-------------------------------------------------------------------------
/** u1I2cDemRead
 *  @retval 0           Success
 *  @retval 1           Fail
 */
//-------------------------------------------------------------------------
UINT8 u1I2cDemRead(VOID *psI2cCtx, UINT8 u1I2cAddr, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount)
{
UINT8 u1Ret;


    #ifdef fcEXTERN_DEMOD	    
    u1Ret = ICtrlBus_I2cDemRead_ExtI2c(psI2cCtx, u1I2cAddr, u2RegAddr, pu1Buffer, u2ByteCount);
    #else
    sRiscCtx.pu4RegBase = pu4DTD_Reg;
    u1Ret = ICtrlBus_RiscDemRead(&sRiscCtx, u2RegAddr, pu1Buffer, u2ByteCount);
    #endif

    return u1Ret;
}



//-------------------------------------------------------------------------
/** u1I2cDemWrite
 *  @retval 0           Success
 *  @retval 1           Fail
 */
//-------------------------------------------------------------------------
UINT8 u1I2cDemWrite(VOID *psI2cCtx, UINT8 u1I2cAddr, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount)
{
UINT8 u1Ret;


    #ifdef fcEXTERN_DEMOD
    u1Ret = ICtrlBus_I2cDemWrite_ExtI2c(psI2cCtx, u1I2cAddr, u2RegAddr, pu1Buffer, u2ByteCount);
    #else
    sRiscCtx.pu4RegBase = pu4DTD_Reg;
    u1Ret = ICtrlBus_RiscDemWrite(&sRiscCtx, u2RegAddr, pu1Buffer, u2ByteCount);
    #endif

    return u1Ret;
}

/***********************************************************************/
/*              Private (Static) Function Prototypes                   */
/***********************************************************************/
/***********************************************************************/
/*              Public Functions                                       */
/***********************************************************************/
/***********************************************************************/
/*              Interface APIs                                         */
/***********************************************************************/
/***********************************************************************/
/*              Configuration APIs                                     */
/***********************************************************************/


/***********************************************************************/
/*              Acquisition APIs                                       */
/***********************************************************************/
VOID *GetDigiTunerCtx(VOID)
{
    DEMOD_CTX_T *psDemodCtx = NULL;
    TD_DTD_CTX_T *pTdDtdCtx = NULL;
    
    if (TunerGetCtx != NULL)
    {
        pTdDtdCtx = TunerGetCtx(MTK_MT5112_0);
        if (pTdDtdCtx != NULL)
        {
            psDemodCtx = pTdDtdCtx->psDemodInbInfo;
        }
    }    	
        
    return &(psDemodCtx->sTunerCtx);
}

VOID *GetDigiDemodCtx(VOID)
{
    DEMOD_CTX_T *psDemodCtx = NULL;
    TD_DTD_CTX_T *pTdDtdCtx = NULL;
   
    if (TunerGetCtx != NULL)
    {
        pTdDtdCtx = TunerGetCtx(MTK_MT5112_0);
        if (pTdDtdCtx != NULL)
        {
            psDemodCtx = pTdDtdCtx->psDemodInbInfo;
        }
    }    	

    return psDemodCtx;
}

/***********************************************************************/
/*              Query APIs                                             */
/***********************************************************************/
