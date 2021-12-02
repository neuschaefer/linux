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
 
 /** @file tuner_registeration.c
 *  tuner registration file.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "tuner_interface_if.h"
#include "PI_Def.h"
#include "pd_tuner_type.h"
#include "queue.h"

typedef struct _ITUNER_FCT_MAP_ENTITY_T {
    GENERIC_LIST_ENTRY  link;
    UINT16              id;
    ITUNER_FCT_T*       fct;
} ITUNER_FCT_MAP_ENTITY_T;

EXTERN BOOL IPhysicalTunerRegistration(ITUNER_FCT_T* tunerFct);

/*******************************************************************/
//! Import tuner function tables
/** [Reminder] Add new tuner here
*/
#if (fcTUNER_TYPE == cNXP_UV1336) || (fcTUNER_TYPE == cITUNER_ALL)
    EXTERN ITUNER_FCT_T piTunerFct_UV1336;      // ROW Latam
#endif
#if (fcTUNER_TYPE == cNXP_UV1356E) || (fcTUNER_TYPE == cITUNER_ALL)
    EXTERN ITUNER_FCT_T piTunerFct_UV1356E;     // ROW China
#endif
#if (fcTUNER_TYPE == cNXP_UV1316E) || (fcTUNER_TYPE == cITUNER_ALL)
    EXTERN ITUNER_FCT_T piTunerFct_UV1316E;     // ROW AP
#endif
#if (fcTUNER_TYPE == cSHARP_VA1G5BF)
    EXTERN ITUNER_FCT_T piTunerFct_VA1G5BF;     // ISDB-T
#endif
#if (fcTUNER_TYPE == cLG_TDVRH751F) 
    EXTERN ITUNER_FCT_T piTunerFct_TDVRH751F;     // TW spec
#endif
#if (fcTUNER_TYPE == cLG_TDTGG902D) 
    EXTERN ITUNER_FCT_T piTunerFct_TDTGG902D;     // DVBT tuner
#endif
#if (fcTUNER_TYPE == cALPS_TDAG4B03A) 
    EXTERN ITUNER_FCT_T piTunerFct_TDAG4B03A;     // DVBT tuner
#endif

//For 5387
#if (fcTUNER_TYPE == cALPS_TDAU4XB02A) || (fcTUNER_TYPE == cDTD_TUNER_ALL)
    EXTERN ITUNER_FCT_T piTunerFct_TDAU4XB02A;
#endif

#if (fcTUNER_TYPE ==cALPS_TDQU8X001A) || (fcTUNER_TYPE == cDTD_TUNER_ALL)
EXTERN ITUNER_FCT_T piTunerFct_TDQU8;
#endif

#if (fcTUNER_TYPE ==cLG_TDVWH810F) || (fcTUNER_TYPE == cTPV2K10_TUNERLST)
EXTERN ITUNER_FCT_T piTunerFct_TDVWH810F;
#endif

#if (fcTUNER_TYPE ==cNUTUNE_FA2307A)
EXTERN ITUNER_FCT_T piTunerFct_FA2307A;
#endif

#if (fcTUNER_TYPE ==cNXP_TD1636)
EXTERN ITUNER_FCT_T piTunerFct_TD1636;
#endif

#if (fcTUNER_TYPE ==cALPS_TEQE3) || (fcTUNER_TYPE == cEMCS_TUNERLST)
EXTERN ITUNER_FCT_T piTunerFct_TEQE3;
#endif

#if (fcTUNER_TYPE ==cALPS_TEQE3_SUNBOARD) || (fcTUNER_TYPE == cEMCS_TUNERLST)
EXTERN ITUNER_FCT_T piTunerFct_TEQE3_SUNBOARD;
#endif

#if (fcTUNER_TYPE ==cALPS_TDAU4D01A) || (fcTUNER_TYPE ==cALPS_TDAU4D02A) || (fcTUNER_TYPE == cEMCS_TUNERLST)
EXTERN ITUNER_FCT_T piTunerFct_TDAU4D01A;
#endif

#if (fcTUNER_TYPE ==cALPS_TEQH3L01A) || (fcTUNER_TYPE == cEMCS_TUNERLST)
EXTERN ITUNER_FCT_T piTunerFct_TEQH3L01A;
#endif

#if (fcTUNER_TYPE ==cALPS_TEQH3L01A_SUNBOARD) || (fcTUNER_TYPE == cEMCS_TUNERLST)
EXTERN ITUNER_FCT_T piTunerFct_TEQH3L01A_SUNBOARD;
#endif

#if (fcTUNER_TYPE ==cPANASONIC_ENV56S) || (fcTUNER_TYPE == cTTE_TUNERLST) || (fcTUNER_TYPE == cTPV2K10_TUNERLST)
EXTERN ITUNER_FCT_T piTunerFct_ENV56S;
#endif

#if (fcTUNER_TYPE ==cTCL_DA58CT) || (fcTUNER_TYPE == cTTE_TUNERLST)
EXTERN ITUNER_FCT_T piTunerFct_DA58CT;
#endif
#if (fcTUNER_TYPE ==cSAMSUNG_DTVA50CVH)
EXTERN ITUNER_FCT_T piTunerFct_DTVA50CVH;
#endif
//For 5363
#if (fcTUNER_TYPE ==cLG_TDTWS710D)
EXTERN ITUNER_FCT_T piTunerFct_TDTWS710D;
#endif

#if (fcTUNER_TYPE ==cLG_TAFTZ716D)
EXTERN ITUNER_FCT_T piTunerFct_TAFTZ716D;
#endif

#if (fcTUNER_TYPE ==cPANASONIC_ENV57S) ||(fcTUNER_TYPE ==cDVBT_TUNER_ALL) ||(fcTUNER_TYPE == cDVBT_2K10_TUNERLST)||(fcTUNER_TYPE ==cTTE_I2CALLLST) //liuqu,20090716 for tunerall
EXTERN ITUNER_FCT_T piTunerFct_ENV57S;
#endif
#if (fcTUNER_TYPE ==cNXP_FT2101) ||(fcTUNER_TYPE ==cDVBT_TUNER_ALL) ||(fcTUNER_TYPE == cDVBT_2K10_TUNERLST)  //liuqu,20090716 for tunerall
EXTERN ITUNER_FCT_T piTunerFct_FT2101;
#endif
#if (fcTUNER_TYPE ==cNuTune_FT2607) ||(fcTUNER_TYPE ==cDVBT_TUNER_ALL) ||(fcTUNER_TYPE == cDVBT_2K10_TUNERLST)  //liuqu,20090716 for tunerall
EXTERN ITUNER_FCT_T piTunerFct_FT2607;
#endif
#if (fcTUNER_TYPE ==cLG_TDTWS810D) ||(fcTUNER_TYPE ==cDVBT_TUNER_ALL) ||(fcTUNER_TYPE == cDVBT_2K10_TUNERLST)  //liuqu,20090716 for tunerall
EXTERN ITUNER_FCT_T piTunerFct_TDTWS810D;
#endif
//#if (fcTUNER_TYPE ==cLG_TDTWS810D) ||(fcTUNER_TYPE ==cDVBT_TUNER_ALL) ||(fcTUNER_TYPE == cDVBT_2K10_TUNERLST)  //liuqu,20090716 for tunerall
#if (fcTUNER_TYPE ==cTCL_DT70WI)||(fcTUNER_TYPE ==cTTE_I2CALLLST) //meirui,20091124 for tunerall
EXTERN ITUNER_FCT_T piTunerFct_DT70WI;
#endif
#if (fcTUNER_TYPE ==cPANASONIC_ENV5SDF)//meirui
EXTERN ITUNER_FCT_T piTunerFct_ENV5SDF;
#endif
#if (fcTUNER_TYPE ==cXUGUANG_DTT8D1C)//meirui
EXTERN ITUNER_FCT_T piTunerFct_DTT8D1C;
#endif
#if (fcTUNER_TYPE ==cTCL_F05WT) //meirui,20100108
EXTERN ITUNER_FCT_T piTunerFct_F05WT;
#endif
#if (fcTUNER_TYPE ==cSHARP_VA1E1BF)
EXTERN ITUNER_FCT_T piTunerFct_VA1E1BF;
#endif
#if (fcTUNER_TYPE ==cTCL_F20WT)
EXTERN ITUNER_FCT_T piTunerFct_F20WT;
#endif


/*******************************************************************/
//! Entitize Tuner function tables
/** [Reminder] Add new tuner here
*/
#if (fcTUNER_TYPE == cNXP_UV1336) || (fcTUNER_TYPE == cITUNER_ALL)
    static ITUNER_FCT_MAP_ENTITY_T fm_UV1336 = {
        .id     = cNXP_UV1336,
        .fct    = &piTunerFct_UV1336
    };
#endif
#if (fcTUNER_TYPE == cNXP_UV1356E) || (fcTUNER_TYPE == cITUNER_ALL)
    static ITUNER_FCT_MAP_ENTITY_T fm_UV1356E = {
        .id     = cNXP_UV1356E,
        .fct    = &piTunerFct_UV1356E
    };
#endif
#if (fcTUNER_TYPE == cNXP_UV1316E) || (fcTUNER_TYPE == cITUNER_ALL)
    static ITUNER_FCT_MAP_ENTITY_T fm_UV1316E = {
        .id     = cNXP_UV1316E,
        .fct    = &piTunerFct_UV1316E
    };
#endif
#if (fcTUNER_TYPE == cSHARP_VA1G5BF)
    static ITUNER_FCT_MAP_ENTITY_T fm_VA1G5BF = {
        .id     = cSHARP_VA1G5BF,
        .fct    = &piTunerFct_VA1G5BF
    };
#endif
#if (fcTUNER_TYPE == cLG_TDVRH751F) 
    static ITUNER_FCT_MAP_ENTITY_T fm_TDVRH751F = {
        .id     = cLG_TDVRH751F,
        .fct    = &piTunerFct_TDVRH751F
    };
#endif
#if (fcTUNER_TYPE == cLG_TDTGG902D) 
    static ITUNER_FCT_MAP_ENTITY_T fm_TDTGG902D = {
        .id     = cLG_TDTGG902D,
        .fct    = &piTunerFct_TDTGG902D
    };
#endif
#if (fcTUNER_TYPE == cALPS_TDAG4B03A) 
    static ITUNER_FCT_MAP_ENTITY_T fm_TDAG4B03A = {
        .id     = cALPS_TDAG4B03A,
        .fct    = &piTunerFct_TDAG4B03A
    };
#endif

//For 5387/88
#if (fcTUNER_TYPE == cALPS_TDAU4XB02A) || (fcTUNER_TYPE == cDTD_TUNER_ALL)
    static ITUNER_FCT_MAP_ENTITY_T fm_TDAU4XB02A = {
        .id     = cALPS_TDAU4XB02A,
        .fct    = &piTunerFct_TDAU4XB02A
    };
#endif
#if (fcTUNER_TYPE ==cALPS_TDQU8X001A) || (fcTUNER_TYPE == cDTD_TUNER_ALL)
   static ITUNER_FCT_MAP_ENTITY_T fm_TDQU8X001A = {
        .id     = cALPS_TDQU8X001A,
        .fct    = &piTunerFct_TDQU8
    };
#endif
#if (fcTUNER_TYPE ==cLG_TDVWH810F) || (fcTUNER_TYPE == cTPV2K10_TUNERLST)
static ITUNER_FCT_MAP_ENTITY_T fm_TDVWH810F = {
        .id     = cLG_TDVWH810F,
        .fct    = &piTunerFct_TDVWH810F
    };
#endif

#if (fcTUNER_TYPE ==cNUTUNE_FA2307A)
static ITUNER_FCT_MAP_ENTITY_T fm_FA2307A = {
        .id     = cNUTUNE_FA2307A,
        .fct    = &piTunerFct_FA2307A
    };
#endif

#if (fcTUNER_TYPE ==cNXP_TD1636)
static ITUNER_FCT_MAP_ENTITY_T fm_TD1636 = {
        .id     = cNXP_TD1636,
        .fct    = &piTunerFct_TD1636
    };
#endif
#if (fcTUNER_TYPE ==cALPS_TEQE3) || (fcTUNER_TYPE == cEMCS_TUNERLST)
static ITUNER_FCT_MAP_ENTITY_T fm_TEQE3 = {
        .id     = cALPS_TEQE3,
        .fct    = &piTunerFct_TEQE3
    };
#endif
#if (fcTUNER_TYPE ==cALPS_TEQE3_SUNBOARD) || (fcTUNER_TYPE == cEMCS_TUNERLST)
static ITUNER_FCT_MAP_ENTITY_T fm_TEQE3_SUNBOARD = {
        .id     = cALPS_TEQE3_SUNBOARD,
        .fct    = &piTunerFct_TEQE3_SUNBOARD
    };
#endif
#if (fcTUNER_TYPE ==cALPS_TDAU4D01A) || (fcTUNER_TYPE ==cALPS_TDAU4D02A) || (fcTUNER_TYPE == cEMCS_TUNERLST)
static ITUNER_FCT_MAP_ENTITY_T fm_TDAU4D01A= {
        .id     = cALPS_TDAU4D01A,
        .fct    = &piTunerFct_TDAU4D01A
    };
#endif
#if (fcTUNER_TYPE ==cALPS_TEQH3L01A) || (fcTUNER_TYPE == cEMCS_TUNERLST)
static ITUNER_FCT_MAP_ENTITY_T fm_TEQH3L01A= {
        .id     = cALPS_TEQH3L01A,
        .fct    = &piTunerFct_TEQH3L01A
    };
#endif
#if (fcTUNER_TYPE ==cALPS_TEQH3L01A_SUNBOARD) || (fcTUNER_TYPE == cEMCS_TUNERLST)
static ITUNER_FCT_MAP_ENTITY_T fm_TEQH3L01A_SUNBOARD= {
        .id     = cALPS_TEQH3L01A_SUNBOARD,
        .fct    = &piTunerFct_TEQH3L01A_SUNBOARD
    };
#endif
#if (fcTUNER_TYPE ==cPANASONIC_ENV56S) || (fcTUNER_TYPE == cTTE_TUNERLST) || (fcTUNER_TYPE == cTPV2K10_TUNERLST)
static ITUNER_FCT_MAP_ENTITY_T fm_ENV56S= {
        .id     = cPANASONIC_ENV56S,
        .fct    = &piTunerFct_ENV56S
    };
#endif

#if (fcTUNER_TYPE ==cTCL_DA58CT) || (fcTUNER_TYPE == cTTE_TUNERLST)
static ITUNER_FCT_MAP_ENTITY_T fm_DA58CT= {
        .id     = cTCL_DA58CT,
        .fct    = &piTunerFct_DA58CT
    };
#endif
#if (fcTUNER_TYPE == cSAMSUNG_DTVA50CVH)
static ITUNER_FCT_MAP_ENTITY_T fm_DTVA50CVH= {
        .id     = cSAMSUNG_DTVA50CVH,
        .fct    = &piTunerFct_DTVA50CVH
    };
#endif

//5387/88 Tuner End

#if (fcTUNER_TYPE ==cLG_TDTWS710D)
static ITUNER_FCT_MAP_ENTITY_T fm_TDTWS710D= {
        .id     = cLG_TDTWS710D,
        .fct    = &piTunerFct_TDTWS710D
    };
#endif

#if (fcTUNER_TYPE ==cLG_TAFTZ716D)
static ITUNER_FCT_MAP_ENTITY_T fm_TAFTZ716D= {
        .id     = cLG_TAFTZ716D,
        .fct    = &piTunerFct_TAFTZ716D
    };
#endif

#if (fcTUNER_TYPE ==cPANASONIC_ENV57S) ||(fcTUNER_TYPE ==cDVBT_TUNER_ALL) ||(fcTUNER_TYPE == cDVBT_2K10_TUNERLST)||(fcTUNER_TYPE ==cTTE_I2CALLLST) 
static ITUNER_FCT_MAP_ENTITY_T fm_ENV57S= {
        .id     = cPANASONIC_ENV57S,
        .fct    = &piTunerFct_ENV57S
    };
#endif
#if (fcTUNER_TYPE ==cNXP_FT2101) ||(fcTUNER_TYPE ==cDVBT_TUNER_ALL)  ||(fcTUNER_TYPE == cDVBT_2K10_TUNERLST)
static ITUNER_FCT_MAP_ENTITY_T fm_FT2101= {
        .id     = cNXP_FT2101,
        .fct    = &piTunerFct_FT2101
    };
#endif
#if (fcTUNER_TYPE ==cNuTune_FT2607) ||(fcTUNER_TYPE ==cDVBT_TUNER_ALL)  ||(fcTUNER_TYPE == cDVBT_2K10_TUNERLST)
static ITUNER_FCT_MAP_ENTITY_T fm_FT2607= {
        .id     = cNuTune_FT2607,
        .fct    = &piTunerFct_FT2607
    };
#endif
#if (fcTUNER_TYPE ==cLG_TDTWS810D) ||(fcTUNER_TYPE ==cDVBT_TUNER_ALL)  ||(fcTUNER_TYPE == cDVBT_2K10_TUNERLST)
static ITUNER_FCT_MAP_ENTITY_T fm_TDTWS810D= {
        .id     = cLG_TDTWS810D,
        .fct    = &piTunerFct_TDTWS810D
    };
#endif

#if (fcTUNER_TYPE ==cSHARP_VA1E1BF)
static ITUNER_FCT_MAP_ENTITY_T fm_VA1E1BF= {
        .id     = cSHARP_VA1E1BF,
        .fct    = &piTunerFct_VA1E1BF
    };
#endif
#if (fcTUNER_TYPE ==cTCL_DT70WI)||(fcTUNER_TYPE ==cTTE_I2CALLLST)
static ITUNER_FCT_MAP_ENTITY_T fm_DT70WI= {
        .id     = cTCL_DT70WI,
        .fct    = &piTunerFct_DT70WI
    };
#endif
#if (fcTUNER_TYPE ==cPANASONIC_ENV5SDF)
static ITUNER_FCT_MAP_ENTITY_T fm_ENV5SDF= {
        .id     = cPANASONIC_ENV5SDF,
        .fct    = &piTunerFct_ENV5SDF
    };
#endif
#if (fcTUNER_TYPE ==cXUGUANG_DTT8D1C)
static ITUNER_FCT_MAP_ENTITY_T fm_DTT8D1C= {
        .id     = cXUGUANG_DTT8D1C,
        .fct    = &piTunerFct_DTT8D1C
    };
#endif
#if (fcTUNER_TYPE ==cTCL_F05WT)
static ITUNER_FCT_MAP_ENTITY_T fm_F05WT= {
        .id     = cTCL_F05WT,
        .fct    = &piTunerFct_F05WT
    };
#endif
#if (fcTUNER_TYPE ==cTCL_F20WT)
static ITUNER_FCT_MAP_ENTITY_T fm_F20WT= {
        .id     = cTCL_F20WT,
        .fct    = &piTunerFct_F20WT
    };
#endif


/*******************************************************************/
//! Insert tuner function tables into entity list
/** [Reminder] Add new tuner here
*/
static GENERIC_LIST_ENTRY ituner_fct_list;
static INT32 CreateTunerFctList()
{
    QueueInitList(&ituner_fct_list);

#if (fcTUNER_TYPE == cNXP_UV1336) || (fcTUNER_TYPE == cITUNER_ALL)
    QueuePutTail(&ituner_fct_list, &fm_UV1336.link);
#endif
#if (fcTUNER_TYPE == cNXP_UV1356E) || (fcTUNER_TYPE == cITUNER_ALL)
    QueuePutTail(&ituner_fct_list, &fm_UV1356E.link);
#endif
#if (fcTUNER_TYPE == cNXP_UV1316E) || (fcTUNER_TYPE == cITUNER_ALL)
    QueuePutTail(&ituner_fct_list, &fm_UV1316E.link);
#endif
#if (fcTUNER_TYPE == cSHARP_VA1G5BF)
    QueuePutTail(&ituner_fct_list, &fm_VA1G5BF.link);
#endif
#if (fcTUNER_TYPE == cLG_TDVRH751F) 
    QueuePutTail(&ituner_fct_list, &fm_TDVRH751F.link);
#endif
#if (fcTUNER_TYPE == cLG_TDTGG902D) 
    QueuePutTail(&ituner_fct_list, &fm_TDTGG902D.link);
#endif
#if (fcTUNER_TYPE == cALPS_TDAG4B03A) 
    QueuePutTail(&ituner_fct_list, &fm_TDAG4B03A.link);
#endif
//For 5387/88
#if (fcTUNER_TYPE == cALPS_TDAU4XB02A) || (fcTUNER_TYPE == cDTD_TUNER_ALL)
   QueuePutTail(&ituner_fct_list, &fm_TDAU4XB02A.link);
#endif

#if (fcTUNER_TYPE ==cALPS_TDQU8X001A) || (fcTUNER_TYPE == cDTD_TUNER_ALL)
QueuePutTail(&ituner_fct_list, &fm_TDQU8X001A.link);
#endif

#if (fcTUNER_TYPE ==cLG_TDVWH810F) || (fcTUNER_TYPE == cTPV2K10_TUNERLST)
QueuePutTail(&ituner_fct_list, &fm_TDVWH810F.link);
#endif

#if (fcTUNER_TYPE ==cNUTUNE_FA2307A)
QueuePutTail(&ituner_fct_list, &fm_FA2307A.link);
#endif

#if (fcTUNER_TYPE ==cNXP_TD1636)
QueuePutTail(&ituner_fct_list, &fm_TD1636.link);
#endif

#if (fcTUNER_TYPE ==cALPS_TEQE3) || (fcTUNER_TYPE == cEMCS_TUNERLST)
QueuePutTail(&ituner_fct_list, &fm_TEQE3.link);
#endif

#if (fcTUNER_TYPE ==cALPS_TEQE3_SUNBOARD) || (fcTUNER_TYPE == cEMCS_TUNERLST)
QueuePutTail(&ituner_fct_list, &fm_TEQE3_SUNBOARD.link);
#endif

#if (fcTUNER_TYPE ==cALPS_TDAU4D01A) || (fcTUNER_TYPE ==cALPS_TDAU4D02A) || (fcTUNER_TYPE == cEMCS_TUNERLST)
QueuePutTail(&ituner_fct_list, &fm_TDAU4D01A.link);
#endif

#if (fcTUNER_TYPE ==cALPS_TEQH3L01A) || (fcTUNER_TYPE == cEMCS_TUNERLST)
QueuePutTail(&ituner_fct_list, &fm_TEQH3L01A.link);
#endif

#if (fcTUNER_TYPE == cALPS_TEQH3L01A_SUNBOARD) || (fcTUNER_TYPE == cEMCS_TUNERLST)
QueuePutTail(&ituner_fct_list, &fm_TEQH3L01A_SUNBOARD.link);
#endif

#if (fcTUNER_TYPE ==cPANASONIC_ENV56S) || (fcTUNER_TYPE == cTTE_TUNERLST) || (fcTUNER_TYPE == cTPV2K10_TUNERLST)
QueuePutTail(&ituner_fct_list, &fm_ENV56S.link);
#endif

#if (fcTUNER_TYPE ==cTCL_DA58CT) || (fcTUNER_TYPE == cTTE_TUNERLST)
QueuePutTail(&ituner_fct_list, &fm_DA58CT.link);
#endif
#if (fcTUNER_TYPE ==cSAMSUNG_DTVA50CVH)
QueuePutTail(&ituner_fct_list, &fm_DTVA50CVH.link);
#endif

//For 5363
#if (fcTUNER_TYPE ==cLG_TDTWS710D)
QueuePutTail(&ituner_fct_list, &fm_TDTWS710D.link);
#endif

#if (fcTUNER_TYPE ==cLG_TAFTZ716D)
QueuePutTail(&ituner_fct_list, &fm_TAFTZ716D.link);
#endif

#if (fcTUNER_TYPE == cALPS_TDAG4B03A) 
    QueuePutTail(&ituner_fct_list, &fm_TDAG4B03A.link);
#endif
#if (fcTUNER_TYPE ==cPANASONIC_ENV57S) ||(fcTUNER_TYPE ==cDVBT_TUNER_ALL) ||(fcTUNER_TYPE == cDVBT_2K10_TUNERLST)||(fcTUNER_TYPE ==cTTE_I2CALLLST)
QueuePutTail(&ituner_fct_list, &fm_ENV57S.link);
#endif
#if (fcTUNER_TYPE ==cNXP_FT2101) ||(fcTUNER_TYPE ==cDVBT_TUNER_ALL) ||(fcTUNER_TYPE == cDVBT_2K10_TUNERLST)
QueuePutTail(&ituner_fct_list, &fm_FT2101.link);
#endif

#if (fcTUNER_TYPE ==cNuTune_FT2607) ||(fcTUNER_TYPE ==cDVBT_TUNER_ALL) ||(fcTUNER_TYPE == cDVBT_2K10_TUNERLST)
QueuePutTail(&ituner_fct_list, &fm_FT2607.link);
#endif

#if (fcTUNER_TYPE ==cLG_TDTWS810D) ||(fcTUNER_TYPE ==cDVBT_TUNER_ALL) ||(fcTUNER_TYPE == cDVBT_2K10_TUNERLST)
QueuePutTail(&ituner_fct_list, &fm_TDTWS810D.link);
#endif

#if (fcTUNER_TYPE ==cSHARP_VA1E1BF)
QueuePutTail(&ituner_fct_list, &fm_VA1E1BF.link);
#endif
#if (fcTUNER_TYPE ==cTCL_DT70WI)||(fcTUNER_TYPE ==cTTE_I2CALLLST)
	QueuePutTail(&ituner_fct_list, &fm_DT70WI.link);
#endif
#if (fcTUNER_TYPE ==cPANASONIC_ENV5SDF)
QueuePutTail(&ituner_fct_list, &fm_ENV5SDF.link);
#endif
#if (fcTUNER_TYPE ==cXUGUANG_DTT8D1C)
		QueuePutTail(&ituner_fct_list, &fm_DTT8D1C.link);
#endif
#if (fcTUNER_TYPE ==cTCL_F05WT)
			QueuePutTail(&ituner_fct_list, &fm_F05WT.link);
#endif
#if (fcTUNER_TYPE ==cTCL_F20WT)
				QueuePutTail(&ituner_fct_list, &fm_F20WT.link);
#endif

    return ituner_fct_list.Count;
}

INT32 ITunerRegistration(UINT16 id)
{
    ITUNER_FCT_MAP_ENTITY_T* entity = NULL;
    ITUNER_FCT_T* fct = NULL;
    INT32 num = CreateTunerFctList();

    if (num == 0)
    {// Error handling
        mcSHOW_DRVERR_MSG(("[fatal] Zero tuner found in the list! (%s)\n", __FILE__));
    }
    else if (num == 1)
    {// Register the only one tuner
        entity = (ITUNER_FCT_MAP_ENTITY_T*)QueueGetHead(&ituner_fct_list);
        if (entity)
            fct = entity->fct;
    }
    else
    {// More than one tuner existed in the list, find out the one with tuner type "id"

        entity = (ITUNER_FCT_MAP_ENTITY_T*)QueueGetHead(&ituner_fct_list);
        // Find the corresponding TunerCtx
        while (entity){
            if (entity->id == id){
                fct = entity->fct;
                break;
            }
            entity = (ITUNER_FCT_MAP_ENTITY_T*)QueueGetNext(&ituner_fct_list, &entity->link);
        }
    }

    if (TRUE == IPhysicalTunerRegistration(fct))
    {
        mcSHOW_USER_MSG(("Find corresponding tuner for registration. (total %d tuners in pool)(TunerType=0x%02X)\n",
                        ituner_fct_list.Count,entity->id));
        return (DRVAPI_TUNER_OK);
    }

    mcSHOW_DRVERR_MSG(("[fatal] Fail in ITunerRegistration()! (%s)\n", __FILE__));
    return (DRVAPI_TUNER_ERROR);
}

