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
#ifndef  _L1_Driver_H_
#define _L1_Driver_H_

#define EMB_L0_API                      /* empty  */
#define SLAB_WINAPI                     /* empty  */
#define EMB_L1_DRIVER_CORE              /* empty  */

#include "Emb_L0_API.h"

#define PAGE_ADDRESS        0xFF
#define SILABS_I2C

/*      INDEXING_MODE_8_BITS : to restrict to 8 bits i2c addressing */
#define INDEXING_MODE_8_BITS

/* Layer 1 core types definitions */
typedef enum ErrorCode {
    Error_CODE_BASE 			    = 2000,
    Error_INCORRECT_NB_READ,		/* 2001 */
    Error_INCORRECT_NB_WRITTEN,		/* 2002 */
    Error_DLL_LOAD,     			/* 2003 */
    Error_ENUM_VAL_UNKNOWN,			/* 2004 */
    Error_COULD_NOT_WRITE_ITEM,		/* 2005 */
    Error_REGISTER_TYPE_UNKNOWN,	/* 2006 */
    Error_ENUM_OUT_OF_RANGE,		/* 2007 */
    Error_TYPE_IS_NOT_ENUMERATE,	/* 2008 */
    Error_VALUE_NOT_IN_ENUM_LIST,	/* 2009 */
    Error_COULD_NOT_UPDATE_ITEM,	/* 2010 */
    Error_VALUE_OUT_OF_RANGE,		/* 2011 */
    Error_UNKNOW_REGISTER,    		/* 2012 */
    Error_READ_TRACES_ERROR,     	/* 2013 */
    Error_WRITE_TRACES_ERROR,    	/* 2014 */
    Error_UNKNOWN_COMMAND,     		/* 2015 */
    Error_BUFFER_DOES_NOT_CONTAIN_REQUIRED_DATA     		/* 2016 */
} ErrorCode;

typedef struct {
  L0_Context *i2c;
  void       *specific;
#ifdef    INDEXING_MODE_8_BITS
  unsigned int currentPage;
#endif /* INDEXING_MODE_8_BITS */
  long  i4IfFreqHz;
} L1_Context;

#ifdef    SYSTEM_WAIT
    int system_wait(int time_ms);
#endif /* #ifdef SYSTEM_WAIT */
/* Layer 1 Core access functions prototypes */
#ifdef __cplusplus
extern "C" {
#endif
    EMB_L1_DRIVER_CORE int     SLAB_WINAPI L1_ReadBytes          (L1_Context* context, unsigned int iI2CIndex, int iNbBytes, UINT8 *pbtDataBuffer);
    EMB_L1_DRIVER_CORE int     SLAB_WINAPI L1_WriteBytes         (L1_Context* context, unsigned int iI2CIndex, int iNbBytes, UINT8 *pbtDataBuffer);
#ifdef    INDEXING_MODE_8_BITS
    EMB_L1_DRIVER_CORE int     SLAB_WINAPI L1_SetBusMode         (L1_Context* context, int mode);
    //EMB_L1_DRIVER_CORE char    SLAB_WINAPI L1_InitComm           (L1_Context** ppContext, LPCTSTR share_name);
    EMB_L1_DRIVER_CORE int     SLAB_WINAPI L1_BusMode                 (L1_Context* context);
    EMB_L1_DRIVER_CORE void    SLAB_WINAPI L1_Init8BitsIndexing       (L1_Context *context);
    EMB_L1_DRIVER_CORE int     SLAB_WINAPI L1_ReadRegister8Bits       (L1_Context* context, unsigned int add, unsigned char offset, unsigned char nbbit, unsigned int isSigned);
    EMB_L1_DRIVER_CORE int     SLAB_WINAPI L1_ReadRegisterTrace8Bits  (L1_Context* context, char* name, unsigned int add, unsigned char offset, unsigned char nbbit, unsigned int isSigned);
    EMB_L1_DRIVER_CORE int     SLAB_WINAPI L1_WriteRegister8Bits      (L1_Context* context, unsigned int add, unsigned char offset, unsigned char nbbit, unsigned char alone, int value);
    EMB_L1_DRIVER_CORE int     SLAB_WINAPI L1_WriteRegisterTrace8Bits (L1_Context* context, char* name, char* valtxt, unsigned int add, unsigned char offset, unsigned char nbbit, unsigned char alone, int Value);
    EMB_L1_DRIVER_CORE int     SLAB_WINAPI L1_ReadBytes8Bits          (L1_Context* context, unsigned int iI2CIndex, int iNbBytes, UINT8 *pbtDataBuffer);
    EMB_L1_DRIVER_CORE int     SLAB_WINAPI L1_WriteBytes8Bits         (L1_Context* context, unsigned int iI2CIndex, int iNbBytes, UINT8  *pbtDataBuffer);
#endif /* INDEXING_MODE_8_BITS */
#ifdef __cplusplus
}
#endif

#endif /*end of _L1_Driver_H_ */
