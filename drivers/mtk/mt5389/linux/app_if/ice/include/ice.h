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
 * Copyright(c) 2005, MediaTek, Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *---------------------------------------------------------------------------
 * $RCSfile: ice.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/ 

/** @file ice.h
 *  This header file declares exported ICE APIs.
 */

#ifndef ICE_H
#define ICE_H

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "drv_common.h"
#include "ice_ioctl.h"
#include "ice_debug.h"
#include "frost/frost_basictypes.h"
#include "mtal.h"
#include "mtutil.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define VIDEO_STATUS_CALLBACK 0
#define EXTERNAL_STATUS_CALLBACK 1
#define VGA_AUTO_CALLBACK 2
#define AFD_CALLBACK 3

#define WRITE_PARTITION (((frost_uint32)0xa5) << 8)

typedef enum
{
    ICEDEC_ASPECT_RATIO_NONE,
    ICEDEC_ASPECT_RATIO_1_1,   ///< Aspect ration 1:1
    ICEDEC_ASPECT_RATIO_4_3,   ///< Aspect ration 4:3
    ICEDEC_ASPECT_RATIO_16_9,  ///< Aspect ration 16:9
    ICEDEC_ASPECT_RATIO_221_1, ///< Aspect ration 2.21:1
    ICEDEC_ASPECT_RATIO_MAX
}ICEDEC_ASPECT_RATIO_T;
 typedef struct 
{
    BOOL fgAfdPrs;            ///<colour_description in Sequence display extension
    BOOL fgHdrInvalid;
    UCHAR ucAfd;                        ///<AFD in Picture user data
    UCHAR ucCodecId;
    ICEDEC_ASPECT_RATIO_T eAspectRatio;
} ICEDEC_HDR_AFD_INFO_T;

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef void (*ice_video_cb_func)(MTVDO_PATH_T path, MTVDECEX_SIG_STATUS eSigStatus);
typedef void (*ice_ext_cb_func)(MTVDO_PATH_T path, MTVDECEX_SIG_STATUS eSigStatus);


//extern frost_bool ICE_Callback_Vdo_RegFunc(UINT8 cb_type, void (*funcptr)());
extern MT_RESULT_T ICE_MTKInit(void);

#ifdef __cplusplus
}
#endif

#endif //ICE_H
