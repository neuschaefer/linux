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
 *  This file describes SIF module exported functions, error code and definitions.
 */

#ifndef SIF_IF_H
#define SIF_IF_H


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#include "x_lint.h"
#include "drvcust_if.h"

LINT_EXT_HEADER_BEGIN
#include "x_os.h"
#include "x_typedef.h"
LINT_EXT_HEADER_END


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------
typedef enum {
    SIF_BUS_SYSTEM_I2C = 0,
    SIF_BUS_TUNER_I2C  = 1,
    SIF_BUS_SECOND_I2C = 2,
    SIF_BUS_PDWNC_I2C  = 3,
    SIF_BUS_8295B_I2C_0 = 4,
    SIF_BUS_8295B_I2C_1 = 5,
    SIF_BUS_MAX
} SIF_BUS_ID_T;

typedef enum
{
    SIF_ERROR_NODEV       = -1,       // SIF device doesn't ack to SIF master
    SIF_ERROR_SUBADDR     = -2,       // There is error when SIF master transmits subaddress data
    SIF_ERROR_DATA        = -3,       // There is error when SIF master transmits/receives data
    SIF_ERROR_BUS_BUSY    = -4,       // SIF bus is busy, can't access
    SIF_ERROR_PARAM       = -5,       // Function parameter is invalid
    SIF_ERROR_TIMEOUT     = -6,       // SIF semaphore timeout
} SIF_ERROR_T;

/* function pointer type */
typedef VOID (*SIF_ASYNC_NFY_FCT)(VOID *pvNfyData, UINT16 u2Result);

typedef enum
{
    SIF_ASYNC_REQ_READ   =  0,  
    SIF_ASYNC_REQ_WRITE   =  1,  
} SIF_ASYNC_REQ_TYPE_T;

typedef struct
{
    UINT8 u1RequestType;      /* SIF_ASYNC_REQ_READ or  SIF_ASYNC_REQ_WRITE */
    UINT8 u1CHannelID;      /* SIF bus ID */
    UINT16 u2ClkDiv;      /* Parameter of divider to divide 27Mhz for SCL */
    UINT8 u1DevAddr;      /* SIF device address */
    UINT8 u1WordAddrNum;      /* SIF word address length */
    UINT32 u4WordAddr;      /* SIF word address. Format XX B2 B1 B0. B2 is sent on SIF bus first */
    UINT8 *pu1Buf;      /* Pointer to user's buffer */
    UINT16 u2ByteCnt;      /* Number of byte to transfer. Max = 256 bytes */
    SIF_ASYNC_NFY_FCT pfnNfy;      /* callback function pointer */
    VOID *pvNfyData;      /* callback function parameter */
} SIF_ASYNC_REQ_T;
	
typedef struct _SIF_OPERATIONS_T 
{  
    VOID (*pfn_Init)(VOID);
    VOID (*pfn_Stop)(VOID);    
    UINT16 (*pfn_X_Read)(UINT8, UINT16, UINT8, UINT8, UINT32, UINT8*, UINT16);    
    UINT16 (*pfn_X_Write)(UINT8, UINT16, UINT8, UINT8, UINT32, UINT8*, UINT16);
    VOID (*pfn_ISRHandler)(UINT16);    
} SIF_OPERATIONS_T;

typedef struct _SIF_STRUCT_T 
{
    UINT8 u1SifVer;
    SIF_OPERATIONS_T rSifOps;
} SIF_STRUCT_T;


//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------
EXTERN INT32 _SIF_Diag(VOID);


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------
/** SIF_Init
 *  Initialize SIF module.
 *  @param   VOID
 *  @return  VOID
 */
EXTERN VOID SIF_Init(VOID);


/** SIF_Stop
 *  Terminate SIF module.
 *  @param   VOID
 *  @return  VOID
 */
EXTERN VOID SIF_Stop(VOID);


/** SIF_Read
 *  Read data with word address from SIF module from SIF_BUS_SYSTEM_I2C.
 *  @param  u2ClkDiv    Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr   Serial interface device address.
 *  @param  u1WordAddr  Serial interface word address.
 *  @param  pu1Buf      Pointer to user's buffer.
 *  @param  u2ByteCnt   Number of byte to read. Max = 256 bytes.
 *  @retval > 0         Actual number of bytes read.
 *  @retval = 0         Error
 */
EXTERN UINT16 SIF_Read(UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddr,
                       UINT8 *pu1Buf, UINT16 u2ByteCnt);


/** SIF_ReadNoSubAddr
 *  Read data without word address from SIF module from SIF_BUS_SYSTEM_I2C.
 *  @param  u2ClkDiv    Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr   Serial interface device address.
 *  @param  pu1Buf      Pointer to user's buffer.
 *  @param  u2ByteCnt   Number of byte to read. Max = 256 bytes.
 *  @retval > 0         Actual number of bytes read.
 *  @retval = 0         Error
 */
EXTERN UINT16 SIF_ReadNoSubAddr(UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 *pu1Buf,
                                UINT16 u2ByteCnt);


/** SIF_TunerReadNoSubAddr
 *  NIM module api: read data without word address from SIF module from SIF_BUS_TUNER_I2C.
 *  @param  u2ClkDiv    Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr   Serial interface device address.
 *  @param  pu1Buf      Pointer to user's buffer.
 *  @param  u2ByteCnt   Number of byte to read. Max = 256 bytes.
 *  @retval > 0         Actual number of bytes read.
 *  @retval = 0         Error
 */
EXTERN UINT16 SIF_TunerReadNoSubAddr(UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 *pu1Buf,
                                     UINT16 u2ByteCnt);


/** SIF_Write
 *  Write data with word address to SIF module on SIF_BUS_SYSTEM_I2C.
 *  @param  u2ClkDiv    Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr   Serial interface device address.
 *  @param  u1WordAddr  Serial interface word address.
 *  @param  pu1Buf      Pointer to user's buffer.
 *  @param  u2ByteCnt   Number of byte to write. Max = 256 bytes.
 *  @retval > 0         Actual number of bytes written.
 *  @retval = 0         Error
 */
EXTERN UINT16 SIF_Write(UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddr,
                        UINT8 *pu1Buf, UINT16 u2ByteCnt);


/** SIF_WriteNoSubAddr
 *  Write data without word address to SIF module on SIF_BUS_SYSTEM_I2C.
 *  @param  u2ClkDiv    Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr   Serial interface device address.
 *  @param  pu1Buf      Pointer to user's buffer.
 *  @param  u2ByteCnt   Number of byte to write. Max = 256 bytes.
 *  @retval > 0         Actual number of bytes written.
 *  @retval = 0         Error
 */
EXTERN UINT16 SIF_WriteNoSubAddr(UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 *pu1Buf,
                                 UINT16 u2ByteCnt);


/** SIF_TunerWriteNoSubAddr
 *  NIM module api: write data without word address to SIF module on SIF_BUS_TUNER_I2C.
 *  @param  u2ClkDiv    Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr   Serial interface device address.
 *  @param  pu1Buf      Pointer to user's buffer.
 *  @param  u2ByteCnt   Number of byte to write. Max = 256 bytes.
 *  @retval > 0         Actual number of bytes written.
 *  @retval = 0         Error
 */
EXTERN UINT16 SIF_TunerWriteNoSubAddr(UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 *pu1Buf,
                                      UINT16 u2ByteCnt);


/** SIF_ReadMultipleSubAddress
 *  Read data with word address, of which length up to three, from SIF module from SIF_BUS_SYSTEM_I2C.
 *  @param  u2ClkDiv        Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr       Serial interface device address.
 *  @param  u1WordAddrNum   Serial interface word address length.
 *  @param  u4WordAddr      Serial interface word address. Format XX B2 B1 B0. B2 is sent on SIF bus first.
 *  @param  pu1Buf          Pointer to user's buffer.
 *  @param  u2ByteCnt       Number of byte to read. Max = 256 bytes.
 *  @retval > 0             Actual number of bytes read.
 *  @retval = 0             Error
 */
EXTERN UINT16 SIF_ReadMultipleSubAddr(UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddrNum, UINT32 u4WordAddr,
                                      UINT8 *pu1Buf, UINT16 u2ByteCnt);


/** SIF_WriteMultipleSubAddress
 *  Write data with word address, of which length up to three, to SIF module on SIF_BUS_SYSTEM_I2C.
 *  @param  u2ClkDiv        Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr       Serial interface device address.
 *  @param  u1WordAddrNum   Serial interface word address length.
 *  @param  u4WordAddr      Serial interface word address. Format XX B2 B1 B0. B2 is sent on SIF bus first.
 *  @param  pu1Buf          Pointer to user's buffer.
 *  @param  u2ByteCnt       Number of byte to read. Max = 256 bytes.
 *  @retval > 0             Actual number of bytes written.
 *  @retval = 0             Error
 */
EXTERN UINT16 SIF_WriteMultipleSubAddr(UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddrNum, UINT32 u4WordAddr,
                                       UINT8 *pu1Buf, UINT16 u2ByteCnt);


/** SIF_X_Read
 *  Read data with channel ID, word address, of which length up to three, from SIF module.
 *  @param  u1ChannelID     SIF bus ID.
 *  @param  u2ClkDiv        Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr       Serial interface device address.
 *  @param  u1WordAddrNum   Serial interface word address length.
 *  @param  u4WordAddr      Serial interface word address. Format XX B2 B1 B0. B2 is sent on SIF bus first.
 *  @param  pu1Buf          Pointer to user's buffer.
 *  @param  u2ByteCnt       Number of byte to read. Max = 256 bytes.
 *  @retval > 0             Actual number of bytes read.
 *  @retval = 0             Error
 */
EXTERN UINT16 SIF_X_Read(UINT8 u1CHannelID, UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddrNum,
                         UINT32 u4WordAddr, UINT8 *pu1Buf, UINT16 u2ByteCnt);


/** SIF_X_Write
 *  Write data with channel ID, word address, of which length up to three, to SIF module.
 *  @param  u1ChannelID     SIF bus ID.
 *  @param  u2ClkDiv        Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr       Serial interface device address.
 *  @param  u1WordAddrNum   Serial interface word address length.
 *  @param  u4WordAddr      Serial interface word address. Format XX B2 B1 B0. B2 is sent on SIF bus first.
 *  @param  pu1Buf          Pointer to user's buffer.
 *  @param  u2ByteCnt       Number of byte to write. Max = 256 bytes.
 *  @retval > 0             Actual number of bytes written.
 *  @retval = 0             Error
 */

EXTERN UINT16 SIF_X_Write(UINT8 u1CHannelID, UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddrNum,
                          UINT32 u4WordAddr, UINT8 *pu1Buf, UINT16 u2ByteCnt);

/** SIF_X_Write_SpecialDemod
 *  Write data with channel ID, word address, of which length up to three, to SIF module.
 *  @param  u1ChannelID     SIF bus ID.
 *  @param  u2ClkDiv        Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr       Serial interface device address.
 *  @param  u1WordAddrNum   Serial interface word address length.
 *  @param  u4WordAddr      Serial interface word address. Format XX B2 B1 B0. B2 is sent on SIF bus first.
 *  @param  pu1Buf          Pointer to user's buffer.
 *  @param  u2ByteCnt       Number of byte to write. Max = 256 bytes.
 *  @retval > 0             Actual number of bytes written.
 *  @retval = 0             Error
 */

EXTERN UINT16 SIF_X_Write_SpecialDemod(UINT8 u1CHannelID, UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddrNum, UINT32 u4WordAddr, UINT8 *pu1Buf, UINT16 u2ByteCnt);


/** SIF_AsyncRead
 *  Read data with channel ID, word address, of which length up to three, from SIF module.
 *  @param  u1ChannelID     SIF bus ID.
 *  @param  u2ClkDiv        Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr       Serial interface device address.
 *  @param  u1WordAddrNum   Serial interface word address length.
 *  @param  u4WordAddr      Serial interface word address. Format XX B2 B1 B0. B2 is sent on SIF bus first.
 *  @param  pu1Buf          Pointer to user's buffer.
 *  @param  u2ByteCnt       Number of byte to read. Max = 256 bytes.
 *  @param  pfnNfy       Callback function pointer.
 *  @param  pvNfyData       Callback function parameter
 *  @retval > 0             Success
 *  @retval = 0             Error
 */

EXTERN UINT16 SIF_X_AsyncRead(UINT8 u1CHannelID, UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddrNum, UINT32 u4WordAddr,
                  UINT8 *pu1Buf, UINT16 u2ByteCnt, SIF_ASYNC_NFY_FCT pfnNfy, VOID *pvNfyData);

/** SIF_X_AsyncWrite
 *  Write data with channel ID, word address, of which length up to three, to SIF module.
 *  @param  u1ChannelID     SIF bus ID.
 *  @param  u2ClkDiv        Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr       Serial interface device address.
 *  @param  u1WordAddrNum   Serial interface word address length.
 *  @param  u4WordAddr      Serial interface word address. Format XX B2 B1 B0. B2 is sent on SIF bus first.
 *  @param  pu1Buf          Pointer to user's buffer.
 *  @param  u2ByteCnt       Number of byte to write. Max = 256 bytes.
 *  @param  pfnNfy       Callback function pointer.
 *  @param  pvNfyData       Callback function parameter
 *  @retval > 0             Success
 *  @retval = 0             Error
 */
EXTERN UINT16 SIF_X_AsyncWrite(UINT8 u1CHannelID, UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddrNum, UINT32 u4WordAddr,
                  UINT8 *pu1Buf, UINT16 u2ByteCnt, SIF_ASYNC_NFY_FCT pfnNfy, VOID *pvNfyData);

/** SIF_WaitLastTransaction
 *  Signal SIF driver to stop issueing any SIF read/write and wait the completion of the last SIF transaction
 *  @param  u4Timeout     Safety guard to avoid wait forever.
 *  @retval void
 */
EXTERN void SIF_WaitLastTransaction(UINT32 u4Timeout);

#endif /* SIF_IF_H */
