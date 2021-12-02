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
/************************************************************************* 
* 
* Filename: 
* --------- 
* 
* Description: 
* ------------ 
*    
*    
* Author: 
* ------- 
*    Justin Wu
* 
* Last changed: 
* -------------  
* $Author: dtvbm11 $  
* 
* $Modtime: 2005/12/17 $   
* 
*************************************************************************/

#ifndef _PD_API_H_
#define _PD_API_H_

/***********************************************************************/
/*              Includes                                               */
/***********************************************************************/
#include "pi_demod.h"

#include "drvapi_tuner.h"
#include "x_typedef.h"
#include "TDIS.h"
#include "TDMgr.h"
#include "UtilAPI.h"
/***********************************************************************/
/*      Conditional Compiler                                           */
/***********************************************************************/
#define fcCHK_CHIP_REV          (1 && fcDEMOD_TYPE)

/***********************************************************************/
//#define TUNER_SOURCE_LABEL      ""
//#define cMAX_I2C_LEN            64
//#define cMAX_ACQ_TIME           2000        // in ms
//#define cUNCLOCK_POLLING_TIME   200         // in ms
//#define cLOCK_POLLING_TIME      20          // min resolution = 20 ms
#define cTUNER_DEFAULT_FREQ     533000
//#define cTUNER_DEFAULT_MOD      MOD_ATSC_8VSB

/***********************************************************************/
#define mcCTMR_DBG_MSG(_x_)     mcSHOW_DBG_MSG2(_x_)
//#define mcCTMR_DBG_MSG(_x_)     mcPRINTF _x_
#define mcCTMR_DBG_MSG2(_x_)    mcSHOW_DBG_MSG2(_x_)

#define mcCHECK_BLOCK_I2C(ucRetSts)
#define mcCHECK_BLOCK_I2C0()
#define mcBLOCK_I2C(ucBlockSts)

#define	TRACE(x)						
#define	TRACE0	

#define	mcDELAY_LOOP			mcDELAY_MS
#define	SPRINTF(x)	
#define GETDURATIONMS   GetDurationMs
//#define	strcmp				x_strcmp
//#define	Sleep				mcDELAY_MS
//#define mcGET_REGs(addr, reg, num, data)    RegGetRegisters(addr, reg, num, data)
//#define mcSET_REGs(addr, reg, num, data)    RegSetRegisters(addr, reg, num, data)
//#define mcGET_REG(addr, reg, data)	        RegGetRegisters(addr, reg, 1, data)
//#define mcSET_REG(addr, reg, data)	        RegSetRegisters(addr, reg, 1, data)

// *** RegMap Structure *** // momo
// for Toolx_SetData()
// same define in Demod/Demod.h
typedef struct
{
    CHAR*       Name;       // Name
    U16         Addr;       // Address
    UCHAR       Attrib;     // R/W
    UCHAR       Default;    // Default value
    UCHAR       Value;      // Current value
//    double      Store;      // Stored value
    UINT32      Store;      // Stored value
} REGISTER;

typedef struct
{
    CHAR*       Name;       // Name
    S32         RegBegin;   // Start index of registers in the group
    S32         RegEnd;     // End index of registers in the group
} GROUP;

typedef struct
{
    CHAR*       Name;       // Name
    S32         Reg;        // Register index
    UCHAR       Lsb;        // LSB position
    UCHAR       Bits;       // Bit width
    UCHAR       Type;       // Signed or unsigned
    UCHAR       Mask;       // Mask compute with width and position
} FIELD;

typedef struct
{
    REGISTER    *RegMap;
    S32         RegLen;
    GROUP       *GroupMap;
    S32         GroupLen;
    FIELD       *FieldMap;
    S32         FieldLen;
} REGMAP;

/***********************************************************************/
/*              Interface APIs                                         */
/***********************************************************************/
S16  RegGetRegisters(U16 i2c_addr, S32 reg_id, U16 reg_num, UCHAR *data);
S16  RegSetRegisters(U16 i2c_addr, S32 reg_id, U16 reg_num, UCHAR *data);
/***********************************************************************/
/*              OS-Specific Functions                                  */
/***********************************************************************/
DEMOD_CTX_T *psDemodCtxInit(BOOL *fgDemodCtxInit);
void vDemodCtxDestroy(DEMOD_CTX_T *psDemodCtx, BOOL *fgDemodCtxInit);
/**********************************************************************/
/*              I2C Interface                                         */
/**********************************************************************/
UINT32 GetDurationMs(BOOL fgGetInit, UINT32 *pu8StartTime);


//S16  RegSetRegistersTuner(U16 i2c_addr, U16 reg_num, UCHAR *data);
//S16 I2CRead(U16 i2c_addr, U16 reg_num, UCHAR *data);

//UINT32 GetDurationMs(BOOL fgGetInit, UINT32 *pu8StartTime);


#endif // _PD_I2C_H_
