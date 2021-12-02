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
 * $RCSfile: sif_if.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file sif_if.h
 *  This file declare the public function prototype in sif_if.c.
 */

#ifndef SIF_IF_H

#define SIF_IF_H

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#include "x_lint.h"


LINT_EXT_HEADER_BEGIN
#include "x_os.h"
#include "x_typedef.h"

LINT_EXT_HEADER_END

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------
typedef enum {
	SIF_BUS_SYSTEM_I2C=0, 
	SIF_BUS_TUNER_I2C=1,
	//SIF_BUS_EDID_I2C
}  SIF_BUS_ID;

typedef struct _SIFDEVPARAM_T
{
    UINT16 u2ClkDiv;  ///< parameter of divider to divide 27 Mhz for SCL. 12 bits width.
    UINT8 u1DevAddr;  ///< device address: 7 bits width.
    UINT32 u4WordAddrNum; ///< number of word address:  2 bits width, 0, 1, 2, 3.
    UINT32 u4WordAddr; ///< word address:  24 bits width.
    UINT8 *pu1Buf;    ///< point to user's buffer.
    UINT16 u2ByteCnt; ///< the data length that user want to read/write.
    UINT16 u2ProcCnt; ///< the data length that SIF read/write completely.
    BOOL fgTuner;       ///< output pins are TUNER_DATA & TUNER_CLK.
    BOOL fgNonStart;   //<send start bit or not> only in 5391
    BOOL fgNonStop;     //<send stop bit or not> only in 5391
    BOOL fgOpenDrain;     //<OpenDrain> only in 5391
    BOOL fgSclStrech;       //<SCL_STRETCH bit> byte/bit synchronization
} SIFDEVPARAM_T;
//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------
EXTERN INT32 _SIF_Diag(VOID);

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

/** SIF_SetNonOSAI
 *  set true when call sif before OSAI initialize.
 *  @param  fgNoOSAI = true : OSAI not initialize, can not call funciton in x_os.h.
 *  @return  VOID
 */
EXTERN VOID SIF_SetNonOSAI(BOOL fgNoOSAI);

/** SIF_Init
 *  Initialize SIF module.
 *  @param  VOID
 *  @return  VOID
 */
EXTERN VOID SIF_Init(VOID);

/** SIF_Stop
 *  terminate SIF module.
 *  @param  VOID
 *  @return  VOID
 */
EXTERN VOID SIF_Stop(VOID);

/** SIF_Read
 *  read data with word address from SIF module.
 *  @param  u2ClkDiv 	Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr 	Serial interface device address.
 *  @param  u1WordAddr 	Serial interface word address.
 *  @param  pu1Buf 		Pointer to user's buffer.
 *  @param  u2ByteCnt 	Number of byte to read. Max = 256 bytes.
 *  @retval   > 0		Actual number of bytes to be read.
 *  @retval   = 0		Error
 */
EXTERN UINT16 SIF_Read(UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddr,
                       UINT8 *pu1Buf, UINT16 u2ByteCnt);
                       
/** SIF_Read_Only
 *  read data with word address = 0 from SIF module.
 *  This function is used by NIM module control.
 *  @param  u2ClkDiv 	Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr 	Serial interface device address.
 *  @param  pu1Buf 		Pointer to user's buffer.
 *  @param  u2ByteCnt 	Number of byte to read. Max = 256 bytes.
 *  @retval   > 0		Actual number of bytes to be read.
 *  @retval   = 0		Error
 */
EXTERN UINT16 SIF_Read_Only(UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 *pu1Buf, 
			UINT16 u2ByteCnt);
			
/** SIF_ReadNoSubAddr
 *  read data without word address from SIF module.
 *  @param  u2ClkDiv 	Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr 	Serial interface device address.
 *  @param  pu1Buf 		Pointer to user's buffer.
 *  @param  u2ByteCnt 	Number of byte to read. Max = 256 bytes.
 *  @retval   > 0		Actual number of bytes to be read.
 *  @retval   = 0		Error
 */
EXTERN UINT16 SIF_ReadNoSubAddr(UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 *pu1Buf, 
			UINT16 u2ByteCnt);
			
/** SIF_TunerReadNoSubAddr
 *  NIM module api: read data without word address from SIF module.
 *  @param  u2ClkDiv 	Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr 	Serial interface device address.
 *  @param  pu1Buf 		Pointer to user's buffer.
 *  @param  u2ByteCnt 	Number of byte to read. Max = 256 bytes.
 *  @retval   > 0		Actual number of bytes to be read.
 *  @retval   = 0		Error
 */
EXTERN UINT16 SIF_TunerReadNoSubAddr(UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 *pu1Buf, 
			UINT16 u2ByteCnt);
			
/** SIF_NIMReadNoSubAddr
 *  NIM module api: read data without word address from SIF module.
 *  @param  u2ClkDiv 	Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr 	Serial interface device address.
 *  @param  pu1Buf 		Pointer to user's buffer.
 *  @param  u2ByteCnt 	Number of byte to read. Max = 256 bytes.
 *  @retval   > 0		Actual number of bytes to be read.
 *  @retval   = 0		Error
 */
EXTERN UINT16 SIF_NIMReadNoSubAddr(UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 *pu1Buf, 
			UINT16 u2ByteCnt);
			
/** SIF_Write
 *  write data with word address to SIF module.
 *  @param  u2ClkDiv 	Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr 	Serial interface device address.
 *  @param  u4WordAddr 	Serial interface word address.
 *  @param  pu1Buf 		Pointer to user's buffer.
 *  @param  u2ByteCnt 	Number of byte to write. Max = 256 bytes.
 *  @retval   > 0		Actual number of bytes to be read.
 *  @retval   = 0		Error
 */
EXTERN UINT16 SIF_Write(UINT16 u2ClkDiv,     UINT8 u1DevAddr, UINT8 u1WordAddr,
                       UINT8 *pu1Buf, UINT16 u2ByteCnt);
                       
/** SIF_WriteNoSubAddr
 *  write data without word address to SIF module.
 *  @param  u2ClkDiv 	Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr 	Serial interface device address.
 *  @param  u4WordAddr 	Serial interface word address.
 *  @param  pu1Buf 		Pointer to user's buffer.
 *  @param  u2ByteCnt 	Number of byte to write. Max = 256 bytes.
 *  @retval   > 0		Actual number of bytes to be read.
 *  @retval   = 0		Error
 */
EXTERN UINT16 SIF_WriteNoSubAddr(UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 *pu1Buf,
                 UINT16 u2ByteCnt);

/** SIF_TunerWriteNoSubAddr
 *  NIM module api: write data without word address to SIF module.
 *  @param  u2ClkDiv 	Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr 	Serial interface device address.
 *  @param  u1WordAddr 	Serial interface word address.
 *  @param  pu1Buf 		Pointer to user's buffer.
 *  @param  u2ByteCnt 	Number of byte to write. Max = 256 bytes.
 *  @retval   > 0		Actual number of bytes to be read.
 *  @retval   = 0		Error
 */
EXTERN UINT16 SIF_TunerWriteNoSubAddr(UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 *pu1Buf,
                 UINT16 u2ByteCnt);
              
/** SIF_NIMWriteNoSubAddr
 *  NIM module api: write data without word address to SIF module.
 *  @param  u2ClkDiv 	Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr 	Serial interface device address.
 *  @param  u1WordAddr 	Serial interface word address.
 *  @param  pu1Buf 		Pointer to user's buffer.
 *  @param  u2ByteCnt 	Number of byte to write. Max = 256 bytes.
 *  @retval   > 0		Actual number of bytes to be read.
 *  @retval   = 0		Error
 */
EXTERN UINT16 SIF_NIMWriteNoSubAddr(UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 *pu1Buf,
                 UINT16 u2ByteCnt);

/** SIF_ReadMultipleSubAddress
 *  read data with word address, of which length up to three, from SIF module.
 *  @param  u2ClkDiv 	Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr 	Serial interface device address.
 *  @param  u1WordAddrNum 	Serial interface word address length.
 *  @param  u4WordAddr 	Serial interface word address.
 *  @param  pu1Buf 		Pointer to user's buffer.
 *  @param  u2ByteCnt 	Number of byte to read. Max = 256 bytes.
 *  @retval   > 0		Actual number of bytes to be read.
 *  @retval   = 0		Error
 */
 EXTERN UINT16 SIF_ReadMultipleSubAddr(UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddrNum, UINT32 u4WordAddr, 
	          UINT8 *pu1Buf, UINT16 u2ByteCnt);

/** SIF_WriteMultipleSubAddress
 *  Write data with word address, of which length up to three, to SIF module.
 *  @param  u2ClkDiv 	Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr 	Serial interface device address.
 *  @param  u1WordAddrNum 	Serial interface word address length.
 *  @param  u4WordAddr 	Serial interface word address.
 *  @param  pu1Buf 		Pointer to user's buffer.
 *  @param  u2ByteCnt 	Number of byte to read. Max = 256 bytes.
 *  @retval   > 0		Actual number of bytes to be read.
 *  @retval   = 0		Error
 */
 EXTERN UINT16 SIF_WriteMultipleSubAddr(UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddrNum , UINT32 u4WordAddr,
                 UINT8 *pu1Buf, UINT16 u2ByteCnt);

 EXTERN UINT16 SIF_X_Read(UINT8 u1CHannelID, UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddrNum, UINT32 u4WordAddr, 
			UINT8 *pu1Buf, UINT16 u2ByteCnt);
 EXTERN UINT16 SIF_X_Write(UINT8 u1CHannelID, UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddrNum , UINT32 u4WordAddr, 
			UINT8 *pu1Buf, UINT16 u2ByteCnt);
#endif /* SIF_IF_H */
