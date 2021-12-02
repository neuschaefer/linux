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

#ifndef _PD_DEF_5133_H_
#define _PD_DEF_5133_H_
#include "PD_Def.h"
#include "drvapi_tuner.h"
#include "x_typedef.h"
#include "TDMgr.h"
#include "UtilAPI.h"

/*---------------------------------------------------------------------*/
/*                  include files                                      */
/*---------------------------------------------------------------------*/

/*---------------------------------------------------------------------*/
/*                  defines, typedefs                                  */
/*---------------------------------------------------------------------*/
//typedef unsigned char  UCHAR; //remove this due to linux platform build error
#ifndef memset
#define memset  x_memset
#endif
/***********************************************************************/
// Constant, usually need not to be changed when porting
//#define OSR_OK                  0
//#define DRVAPI_TUNER_OK         0
//#define DRVAPI_TUNER_ERROR      -1
//#define cSHORT_SLEEP_TIME       5           // ms

//#define fcTUNER_TYPE			cMICROTUNE_MT2266

/************************** Common Macro *********************/
#define mcDELAY(x) mcDELAY_MS(x)                

#define CREATE_OBJECT(_p, _T) \
{ \
	_p = (_T *) x_mem_calloc(1, sizeof(_T)); \
}

#define DESTROY_OBJECT(_p) \
{ \
	if (_p) \
		x_mem_free(_p); \
}

/***********************************************************************/
/*      Conditional compiler dependant Macro                           */
/***********************************************************************/

#define fcUSB_APP               0
#define	FUNC_EXPORT			
#define	fcFOR_CUSTOMER		0
#define	UNIX_CONVENTION		

/********************************************************************************/
// Followings need not to be changed for proting

/************************** Release Related *********************/
#define cCTMR_MTK               0
#define cCTMR_COMTECH           1
#define cCTMR_WAVETEK           2
#define cCTMR_NI                3
#define cCTMR_MW                4
#define cCTMR_SAMSUNG           5
#define cCTMR_SAMSUNG_MT5131    6   // Ken, 060327
#define cCTMR_COMTECH_MT5131    7   // Ken, 060327
#define cCTMR_ALPS_MT5131       8   // momo, 060509
#define cCTMR_ALI_MT5131        8   // momo, 060526
#define cCTMR_GEN_PCTV          9   // Ken, 070308

//#define fcFOR_CUSTOMER          1           // Or could be set in project settings --> Preprocessor definitions

#define fcCUSTOMER_OOB          0

//#define cCTMR_ID                cCTMR_COMTECH
//#define cCTMR_ID                cCTMR_WAVETEK
//#define cCTMR_ID                cCTMR_NI
//#define cCTMR_ID                cCTMR_MTK
//#define cCTMR_ID                cCTMR_SAMSUNG_MT5131
//#define cCTMR_ID                cCTMR_COMTECH_MT5131
//#define cCTMR_ID                cCTMR_ALPS_MT5131
//#define cCTMR_ID                cCTMR_ALI_MT5131
#define cCTMR_ID				cCTMR_GEN_PCTV

#define fcINTERNAL_DIAG         0
#define fcTEST_MODE             0
#define fcSINGLE_TEMPO          1

#define cEXT_CLASS

#define fcFOR_CTMR(Ctmr)

/***********************************************************************/
/*      Conditional compiler                                           */
/***********************************************************************/
#define fcINTERNAL_TEST         0
#define fcERROR_CHECK           1

/************************** Common Constants *********************/
#define cINB_ADDRESS          
#define cOOB_ADDRESS          
#define cBER_TOV                
#define cPER_TOV              
//#define cTCM_TOV                ((float) 5e-4)
#define cTCM_TOV               
#define cSNR_GOOD_LOWER_BOUND  
#define cSNR_GOOD_UPPER_BOUND  
#define cECHO_STRONG_LEVEL     
#define cRESERVED_REG_NUM 

#endif  // _PD_DEF_5133_H_
