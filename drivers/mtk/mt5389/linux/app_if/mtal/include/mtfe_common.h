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
 * $RCSfile: mtfe_common.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file mtfe_common.h
 *  This header file declares exported APIs of DTD module.
 */

#ifndef MTFE_COMMON_H
#define MTFE_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "mttype.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef int (*MTFE_CB_FCT) (void *pvTag);


/**The modulation type
*/
typedef enum
{
    MTFE_BW_TYPE_BEGIN,
    MTFE_BW_6MHz = MTFE_BW_TYPE_BEGIN,
    MTFE_BW_7MHz,
    MTFE_BW_8MHz,
    MTFE_BW_TYPE_END
}MTFE_BW_T; 

/**The RF mode type
*/
typedef enum
{
    MTFE_RF_AIR,                  // Air mode
    MTFE_RF_CABLE                 // Cable mode
}MTFE_RF_MOD_T;

/**The surf mode type
*/
typedef enum
{
    MTFE_SURF_SCAN,              // Channel scan
    MTFE_SURF_CHANCHANGE         // Channel change
}MTFE_SURF_MOD_T;

/**The demod lock status
*/
typedef enum
{
    MTFE_DTV_UNLOCK=0,                                  // Unlock
    MTFE_DTV_LOCK,                                      // Lock
    MTFE_DTV_BREAK_BY_SECOND_CONNECT_EVENT,             // Break
    MTFE_DTV_EARLY_BREAK_UNLOCK,                        // Early-break unlock
    MTFE_DTV_UNSTABLE,                                  //Unstable, new added,lei110803
    MTFE_DTV_UNKNOWN
}MTFE_DTV_STATUS_T;

/**The analog VIF lock status
*/
typedef enum
{
    MTFE_ATV_VIF_LOCK_STATUS_BIT=0,       //bit 0
    MTFE_ATV_TVD_LOCK_STATUS_BIT=1,       //bit 1
    MTFE_ATV_BREAK_SEARCH_NEARBY_BIT=2   //bit 2  // Break happen
}MTFE_ATV_STATUS_T;

/**The analog TVD lock status
*/
typedef enum
{
    MTFE_ATV_TVD_UNLOCK,             // Unlock
    MTFE_ATV_TVD_LOCK              // Lock
}MTFE_ATV_TVD_STATUS_T;   //jackson: todo remove this definition

/** Front-end Callback structure ID ENUM (US standard)
*/
typedef	enum
{
	MTAL_FE_ATSC_LOCK_STATUS = 0,
	MTAL_FE_NTSC_LOCK_STATUS
} MTAL_FE_US_TUNER_CALLBACK_ID_ENUM_T;


/** Callback structure of MTAL_FE_TUNER_US (US Standard)
*/
typedef struct
{
	MTAL_FE_US_TUNER_CALLBACK_ID_ENUM_T		CbId;
	union
	{
		int			LockStatus;
	} CbIdInfo;
    union
	{
		struct{
            UINT32 u4Freq;
        }atvParams;
        struct{
            UINT32 u4Freq;
            UCHAR mod;
        }dtvParams;
	} CbParam;

} MTAL_FE_US_TUNER_CALLBACK_INFO_T;

/**
ARIB Japanl callback event id definition
**/
typedef	enum
{
    MTAL_FE_ISDB_EWS_START,
    MTAL_FE_ISDB_EWS_END,	
} MTAL_FE_JAPAN_EWS_STATUS_T;

/** Front-end Callback structure ID ENUM (JAPAN ARIB standard)
*/
typedef	enum
{
    MTAL_FE_ISDB_LOCK_STATUS = 0,
    MTAL_FE_ATV_NTSC_LOCK_STATUS = MTAL_FE_NTSC_LOCK_STATUS,
    MTAL_FE_ISDB_EWS_STATUS
} MTAL_FE_JAPAN_TUNER_CALLBACK_ID_ENUM_T;


/** Callback structure of MTAL_FE_TUNER_JAPAN (JAPAN ARIB Standard)
*/
typedef struct
{
    MTAL_FE_JAPAN_TUNER_CALLBACK_ID_ENUM_T  CbId;
    union
    {
        int	LockStatus;
        MTAL_FE_JAPAN_EWS_STATUS_T EwsEvent;
    } CbIdInfo;

    union
    {
        struct
        {
            UINT32 u4Freq;
        }atvParams;
        struct
        {
            UINT32 u4Freq;
            UCHAR mod;
        }dtvParams;
    } CbParam;
} MTAL_FE_JAPAN_TUNER_CALLBACK_INFO_T;

/** Front-end EU Callback structure ID ENUM
*/
typedef	enum
{
	MTAL_FE_DVBT_LOCK_STATUS = 1,
	MTAL_FE_PAL_LOCK_STATUS=2
} MTAL_FE_EU_TUNER_CALLBACK_ID_ENUM_T;


/** Callback structure of MTAL_FE_TUNER_EU 
*/
typedef struct
{
	MTAL_FE_EU_TUNER_CALLBACK_ID_ENUM_T		CbId;
	union
	{
		int   LockStatus;
	} CbIdInfo;
    union
    {
        struct{
            UINT32 u4Freq;
        }atvParams;
        struct{
        }dtvParams;
    } CbParam;
} MTAL_FE_EU_TUNER_CALLBACK_INFO_T;

/** Front-end DTMB Callback structure ID ENUM
*/
typedef	enum
{
	MTAL_FE_DTMB_LOCK_STATUS = 1,
	MTAL_FE_PAL_NTSC_LOCK_STATUS=MTAL_FE_PAL_LOCK_STATUS
} MTAL_FE_CH_TUNER_CALLBACK_ID_ENUM_T;


/** Callback structure of MTAL_FE_TUNER_EU 
*/
typedef struct
{
	MTAL_FE_CH_TUNER_CALLBACK_ID_ENUM_T		CbId;
	union
	{
		int   LockStatus;
	} CbIdInfo;
    union
    {
        struct{
            UINT32 u4Freq;
        }atvParams;
        struct{
        }dtvParams;
    } CbParam;
} MTAL_FE_CH_TUNER_CALLBACK_INFO_T;
/**
Brazil callback event id definition
**/
typedef	enum
{
	MTAL_FE_SBTVD_EWS_START,
	MTAL_FE_SBTVD_EWS_END,	
} MTAL_FE_BZ_EWS_STATUS_T;

typedef	enum
{
	MTAL_FE_SBTVD_LOCK_STATUS,
//	MTAL_FE_NTSC_LOCK_STATUS,
	MTAL_FE_SBTVD_EWS_STATUS
} MTAL_FE_BZ_TUNER_CALLBACK_ID_ENUM_T;
/**
Brazil callback parameter structure definition
**/
typedef struct
{
	MTAL_FE_BZ_TUNER_CALLBACK_ID_ENUM_T		CbId;
	union
	{
		int   LockStatus;
		MTAL_FE_BZ_EWS_STATUS_T EwsEvent;
	} CbIdInfo;
/*
    union
    {
        struct{
           
        }atvParams;
        struct{
		 UINT32 u4Freq;
        }dtvParams;
    } CbParam;
*/
} MTAL_FE_BZ_TUNER_CALLBACK_INFO_T;

#ifdef __cplusplus
}
#endif

#endif //MTFE_COMMON_H
