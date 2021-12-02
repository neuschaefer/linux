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
#include "user_i2c_api.h"
#include "sif_if.h"     // For SIF_NIMReadNoSubAddr & SIF_NIMWriteNoSubAddr
#include "ctrl_bus.h"
#include "x_assert.h"
#include "PI_Def.h"
#include "PD_Def.h"

#include "tmNxTypes.h"
#include "tmbslFrontEndTypes.h"
#include "tmCompId.h"



/***********************************************************************/
/*              I2C Interface                                          */
/***********************************************************************/
/***********************************************************************/
/*                                                                     */
/*   FUNCTION DESCRIPTION                        u1I2cRead             */
/*                                                                     */
/*                                                                     */
/*   ROUTINES CALLED                                                   */
/*                                                                     */
/*   INPUTS                                                            */
/*      UINT8 u1RegAddr - Register Address to read                     */
/*      UINT8 u2ByteCount - number of bytes to read                    */
/*                                                                     */
/*   OUTPUTS                                                           */
/*      UINT8 *pu1Buffer - read data bytes are copied to this buffer   */
/*                                                                     */
/*   RETURN:                                                           */
/*    0 for success                                                    */
/*    non-zero for failure                                             */
/*                                                                     */
/***********************************************************************/

#if 0

tmErrorCode_t 	UserWrittenI2CRead(tmUnitSelect_t tUnit,UInt32 AddrSize, UInt8* pAddr,UInt32 ReadLen, UInt8* pData)
{

   
	tmErrorCode_t   err = TM_OK;
//	UINT8 data1;        //AddrNum
	UINT16 data2;      //ByteCnt
//	data1 =(UINT8) AddrSize & 0xFF;
	data2 = (UINT16) ReadLen & 0xFFFF;
//	if (SIF_X_Read(SIF_BUS_TUNER_I2C, cI2C_TIMING, cTUNER_I2C_ADDR, data1, (UINT32)(* pAddr), pData, data2) == 0)
        if (ICtrlBus_I2cTunerRead(cI2C_TIMING,cTUNER_I2C_ADDR,pData, data2) > 0)
	   {
		  mcSHOW_DBG_MSG(("UserWrittenI2CRead failed! TunerAddr:%02X,RegsterAddr:%02X!\n", cTUNER_I2C_ADDR, *pAddr));
		  err = 1;
	   }
	else
	   {
	     mcSHOW_DBG_MSG(("UserWrittenI2CRead success! TunerAddr:%02X,RegsterAddr:%02X!\n", cTUNER_I2C_ADDR, *pAddr));
	    }
	return err;
}


tmErrorCode_t 	UserWrittenI2CWrite (tmUnitSelect_t tUnit, UInt32 AddrSize, UInt8* pAddr,UInt32 WriteLen, UInt8* pData)
{
  
	tmErrorCode_t   err = TM_OK;
	//UINT8 data1;        //AddrNum
	UINT16 data2;      //ByteCnt
	//data1 =(UINT8) (AddrSize & 0xFF);
	data2 = (UINT16) (WriteLen & 0xFFFF);
    if ( WriteLen > cMAX_I2C_LEN)
        mcSHOW_DBG_MSG(("%d exceed max I2C length! (%d)\n", WriteLen, cMAX_I2C_LEN));
        
   // if (SIF_X_Write(SIF_BUS_TUNER_I2C, cI2C_TIMING, cTUNER_I2C_ADDR, data1, (UINT32)(* pAddr),pData, data2) == 0)
       if (ICtrlBus_I2cTunerWrite(cI2C_TIMING,cTUNER_I2C_ADDR,pData, data2) >0)
       {
        mcSHOW_DBG_MSG(("UserWrittenI2CWrite failed! TunerAddr:%02X,RegsterAddr:%02X!\n", cTUNER_I2C_ADDR, * pAddr));
        err = 1;
       }
	else
	{
	mcSHOW_DBG_MSG(("UserWrittenI2CWrite success! TunerAddr:%02X,RegsterAddr:%02X!\n", cTUNER_I2C_ADDR, *pAddr));
	}
    return err; 

}

#else

tmErrorCode_t 	UserWrittenI2CRead(tmUnitSelect_t tUnit,UInt32 AddrSize, UInt8* pAddr,UInt32 ReadLen, UInt8* pData)
{

   
	tmErrorCode_t   err = TM_OK;
	UINT8 data1;        //AddrNum
	UINT16 data2;      //ByteCnt
	data1 =(UINT8) AddrSize & 0xFF;
	data2 = (UINT16) ReadLen & 0xFFFF;
        
        
	if (SIF_X_Read(SIF_BUS_TUNER_I2C, cI2C_TIMING, cTUNER_I2C_ADDR, data1, (UINT32)(* pAddr), pData, data2) == 0)
	   {
		  mcSHOW_DBG_MSG(("UserWrittenI2CRead failed! TunerAddr:%02X,RegsterAddr:%02X!\n", cTUNER_I2C_ADDR, *pAddr));
		  err = 1;
	   }
	else
	   {
	     mcSHOW_DBG_MSG(("UserWrittenI2CRead success! TunerAddr:%02X,RegsterAddr:%02X!\n", cTUNER_I2C_ADDR, *pAddr));
	    }
	return err;
}


tmErrorCode_t 	UserWrittenI2CWrite (tmUnitSelect_t tUnit, UInt32 AddrSize, UInt8* pAddr,UInt32 WriteLen, UInt8* pData)
{
  
	tmErrorCode_t   err = TM_OK;
	UINT8 data1;        //AddrNum
	UINT16 data2;      //ByteCnt
	data1 =(UINT8) (AddrSize & 0xFF);
	data2 = (UINT16) (WriteLen & 0xFFFF);
    if ( WriteLen > cMAX_I2C_LEN)
        mcSHOW_DBG_MSG(("%d exceed max I2C length! (%d)\n", WriteLen, cMAX_I2C_LEN));
        
    if (SIF_X_Write(SIF_BUS_TUNER_I2C, cI2C_TIMING, cTUNER_I2C_ADDR, data1, (UINT32)(* pAddr),pData, data2) == 0)
       {
        mcSHOW_DBG_MSG(("UserWrittenI2CWrite failed! TunerAddr:%02X,RegsterAddr:%02X!\n", cTUNER_I2C_ADDR, * pAddr));
        err = 1;
       }
	else
	{
	mcSHOW_DBG_MSG(("UserWrittenI2CWrite success! TunerAddr:%02X,RegsterAddr:%02X!\n", cTUNER_I2C_ADDR, *pAddr));
	}
    return err; 

}


#endif

tmErrorCode_t 	UserWrittenWait(tmUnitSelect_t tUnit, UInt32 tms)
{
     tmErrorCode_t   err = TM_OK;
	 mcDELAY_MS(tms);
	 return err;
}


/************function  UserWrittenPrint(UInt32 level, const char* format, ...)  not used*****************/
tmErrorCode_t 	UserWrittenPrint(UInt32 level, const char* format, ...)
{
  UInt32 DebugLevel = level;
  tmErrorCode_t   err = TM_OK;
  if (DebugLevel >= 4)
  	{
      DebugLevel = 4;
      mcDBG_LVL_MSG(DebugLevel, (format));
  	}
  else if (DebugLevel <= 0)
  	{
      mcSHOW_USER_MSG((format));
  	}
   else 
   	{
      mcDBG_LVL_MSG(DebugLevel, (format)); 
    }
   return err;
}

tmErrorCode_t  	UserWrittenMutexInit(ptmbslFrontEndMutexHandle *ppMutexHandle)
{
	tmErrorCode_t	err = TM_OK;


	return err;
}


tmErrorCode_t  	UserWrittenMutexDeInit( ptmbslFrontEndMutexHandle pMutex)
{

   tmErrorCode_t	err = TM_OK;

	
  return err;
}


tmErrorCode_t  	UserWrittenMutexAcquire(ptmbslFrontEndMutexHandle pMutex, UInt32 timeOut)
{

   tmErrorCode_t	err = TM_OK;

	
   return err;
}


tmErrorCode_t  	UserWrittenMutexRelease(ptmbslFrontEndMutexHandle pMutex)
{
	tmErrorCode_t	err = TM_OK;
	
	return err;
}

