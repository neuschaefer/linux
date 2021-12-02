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
 * $RCSfile: swdmx_mpg_psipsr.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file psipsr.c
 *  This header file declares public function prototypes of widget.
 */


#include "dmx_if.h"
#include "swdmx_drvif.h"
#include "swdmx_mpg.h"
#include "swdmx_mpg_dmx.h"
#include "swdmx_mpg_psipsr.h"
#include "swdmx_mpg_psipsr_if.h"
#include "swdmx_debug.h"
#include "x_os.h"


#include "x_printf.h"
#include "x_stl_lib.h"
#include "x_assert.h"
#include "x_demux.h"
#include "x_drv_cli.h"
#include "x_util.h"

//---------------------------------------------------------------------
// LOG macro related

#define PSI_TIMEOUT 5000

//#define _MAX_PMT_NUM 96
#define DMX_NULL_PIDX                           0xff

#define NVGT_MAX_DTV_CH 64
#define _MAX_TS_NUM 32

#define _STATE_INIT 0
#define _STATE_FILTER 1
#define _STATE_HIT 2
#define _STATE_FAIL 3

#define NVGT_PSI_START_PIDX 8
#define NVGT_MAX_TABLE 0x1000

#define STREAM_TYPE_H264        0x1B
#define STREAM_TYPE_MVC         0x20
#define STREAM_TYPE_MULTITRACK  0x101
#define STREAM_TYPE_VC1         0xEA
#define STREAM_TYPE_AVS         0x42
#define STREAM_TYPE_VIDEO		2
#define STREAM_TYPE_MPEG1		3
#define STREAM_TYPE_MPEG2		4
#define STREAM_TYPE_USER_DEFINE 6
#define STREAM_TYPE_AC3_EU		0xBD
#define STREAM_TYPE_AC3			0x81
#define STREAM_TYPE_EAC3		0x87
#define STREAM_TYPE_AAC			0x11
#define STREAM_TYPE_AAC_ISDB    0xF
#define STREAM_TYPE_CYPHER		0x80
#define STREAM_TYPE_UNKNOWN		0xFF
#define STREAM_TYPE_SWDMX_LPCM  0x100
#define VCT_SERVICE_TYPE_ATSC_DTV 0x2
#define VCT_SERVICE_LOCATION_DESCRIPTOR 0xa1
#define DESCRIPTOR_TAG_ID_MVC       0x31
#define DESCRIPTOR_TAG_ID_AC3       0x6A
#define DESCRIPTOR_TAG_ID_LPCM      0x05
#define DESCRIPTOR_TAG_ID_REGISTRATION  0x05
    #define FORMAT_IDENTIFIER_05_LPCM       0x44535342
    #define FORMAT_IDENTIFIER_80_LPCM       0x564D4448
    #define FORMAT_IDENTIFIER_RD3D          0x44334452

// static variable needed move to  SWDMX_MPG_INFO_T
static HANDLE_T _ahPsiSema[SWDMX_SOURCE_MAX];
#if 0
static UINT32 _u4State = _STATE_INIT;
//static PSIPSR_SECTION _arSection[_MAX_PMT_NUM+4]; // PAT,PMT*n,VCT*2,MGT
static UINT8 _au1Buf[4096];
static SWDMX_DMX_MSG_T _rDmxMsg;
#endif 

/*
BOOL SWDMX_PSIPSR_UtcToRtc(PSIPSR_RTC_T* prRtc, UINT64 u8Utc);
INT32 PSIPSR_ParsePSI(PSIPSR_TBL_PMT_T* prPMTTbl, UINT32* pu4NumTblEle, UINT32 u4Timeout, UINT32 u4TsIndex);
INT32 PSIPSR_ParseMGT(PSIPSR_TBL_MGT_T* prMgtTbl, UINT32* pu4NumTblEle, UINT32 u4Timeout, UINT32 u4TsIndex);
INT32 PSIPSR_ParseTVCT(PSIPSR_TBL_VCT_T* prVctTbl, UINT32* pu4NumTblEle, PSIPSR_TBL_TS2FREQ_T* prTsTbl, UINT32 u4Timeout, UINT32 u4TsIndex);
INT32 PSIPSR_ParseCVCT(PSIPSR_TBL_VCT_T* prVctTbl, UINT32* pu4NumTblEle, PSIPSR_TBL_TS2FREQ_T* prTsTbl, UINT32 u4Timeout, UINT32 u4TsIndex);
INT32 PSIPSR_ParseSTT(PSIPSR_TBL_STT_T* prSttTbl, UINT32* pu4NumTblEle, UINT32 u4Timeout, UINT32 u4TsIndex);
*/
#if 0
static INT32 _AllocatePidEntry(UINT32 u4Pid, UINT32* pu4PidEntry);
static INT32 _AllocateFilter(UINT32 u4Pidx, UINT32* pu4Filter);
#endif
static INT32 _SWDMX_ParsePsi(UINT8 u1SrcId, PSIPSR_PARAM_T* prInfo);
static INT32 _SWDMX_StartFilter(UINT8 u1SrcId, UINT32* pu4Filter, const PSIPSR_PARAM_T* prInfo);
#if 0
static VOID _EndFilter(UINT32 u4Filter);
#endif
static INT32 _SWDMX_HandleMgt(UINT8 u1SrcId, UINT32 u4Filter, PSIPSR_PARAM_T* prInfo);
static INT32 _SWDMX_HandlePat(UINT8 u1SrcId, UINT32 u4Filter, PSIPSR_PARAM_T* prInfo);
static INT32 _SWDMX_HandlePmt(UINT8 u1SrcId, UINT32 u4Filter, const PSIPSR_PARAM_T* prInfo);
static INT32 _SWDMX_HandleVct(UINT8 u1SrcId, UINT32 u4Filter, PSIPSR_PARAM_T* prInfo);
static INT32 _SWDMX_HandleStt(UINT8 u1SrcId, UINT32 u4Filter, const PSIPSR_PARAM_T* prInfo);
static INT32 _SWDMX_GetBuffer(UINT8 u1SrcId, UINT32 u4SectionIndex, UINT8* pu1Buf, SIZE_T* pzSize);


VOID SWDMX_PsiPsrDataNofify(UINT8 u1SrcId, UINT32 u4Tag)
{
    SWDMX_ERR_CODE_T eRet;
    SWDMX_DMX_MSG_T rMsg;
    
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);

    SWDMX_MPG_VERIFY_NULL(prSwdmxInst);
    
    if(u1SrcId >= SWDMX_SOURCE_MAX)
    {
        LOG(3, "(u1SrcId >= SWDMX_SOURCE_MAX)\n");
        return;
    }
    
    eRet = SWDMX_DmxPsiGetMessage(u1SrcId, (UINT8)u4Tag, &rMsg);
    if((rMsg.eType != SWDMX_DMX_MSG_TYPE_PSI) || (eRet != SWDMX_E_OK))
    {
        return;
    }

    if (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.u4State!=_STATE_HIT)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.u4State = _STATE_FAIL;
        x_memcpy(&prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.rDmxMsg, &rMsg, sizeof(SWDMX_DMX_MSG_T));
        if(x_sema_unlock(_ahPsiSema[u1SrcId])== OSR_OK)
        {
            LOG(3, "SWDMX data callback with OSR_OK\n");
        }

        return;
    }

    return;
}


static VOID _SWDMX_FilterNotify(UINT8 u1SrcId, UINT32 u4Tag)
{
    SWDMX_ERR_CODE_T eRet;
    SWDMX_DMX_MSG_T rMsg;
    
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL(prSwdmxInst);

    if(u1SrcId >= SWDMX_SOURCE_MAX)
    {
        LOG(3, "(u1SrcId >= SWDMX_SOURCE_MAX)\n");
        return;
    }
    
    eRet = SWDMX_DmxPsiGetMessage(u1SrcId, (UINT8)u4Tag, &rMsg);
    if((rMsg.eType != SWDMX_DMX_MSG_TYPE_PSI) || (eRet != SWDMX_E_OK))
    {
        return;
    }

    if (( prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.u4State==_STATE_FILTER) || (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.u4State==_STATE_FAIL))
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.u4State = _STATE_HIT;
        x_memcpy(&prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.rDmxMsg, &rMsg, sizeof(SWDMX_DMX_MSG_T));
        UNUSED(_SWDMX_MpgTsFillBufferEnd(u1SrcId));
        if(x_sema_unlock(_ahPsiSema[u1SrcId])== OSR_OK)
        {
            LOG(6, "PSI callback with OSR_OK\n");
        }

        return;
    }

    return;
}

static INT32 _SWDMX_StartFilter(UINT8 u1SrcId, UINT32* pu4Filter, const PSIPSR_PARAM_T* prInfo)
{
    SWDMX_ERR_CODE_T rRetCode;
    UINT8 u1FilterId;
    SWDMX_DMX_FILTER_T rFilter;
    UINT8 au1Pattern[4] = {0};
    UINT8 au1Mask[4] = {0};
    UINT32 u4Length = 1;
    //DMX PIC comment
    //UINT8 u1Offset = 0;
    UINT8 u1Offset = 1;
    UINT32 u4Pid;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    if((prInfo == NULL) || (pu4Filter == NULL))
    {
        LOG(3,"_SWDMX_StartFilter prInfo or pu4Filter NULL\n");
    	return (INT32)E_PSIPSR_ERROR;
    }
    
    if (prInfo->u4TblType == (UINT32)E_PSIPSR_TBL_PAT)
    {
        au1Pattern[0] = 0x0;
        au1Mask[0] = 0xff;
        u4Pid = 0x0;
    }
    else if (prInfo->u4TblType == (UINT32)E_PSIPSR_TBL_MGT)
    {
        au1Pattern[0] = 0xc7;
        au1Mask[0] = 0xff;
        u4Pid = 0x1ffb;        
    }
    else if (prInfo->u4TblType == (UINT32)E_PSIPSR_TBL_TVCT)
    {
        au1Pattern[0] = 0xc8;
        au1Mask[0] = 0xff;
        u4Pid = 0x1ffb;
    }
    else if (prInfo->u4TblType == (UINT32)E_PSIPSR_TBL_CVCT)
    {
        au1Pattern[0] = 0xc9;
        au1Mask[0] = 0xff;
        u4Pid = 0x1ffb;        
    }
    else if (prInfo->u4TblType == (UINT32)E_PSIPSR_TBL_PMT)
    {
        UINT32 u4ProgNum;//, u4Pattern;
        PSIPSR_TBL_PMT_T* prPMTTbl = prInfo->prPMTTbl;
        if(prPMTTbl == NULL)
        {
            LOG(3,"_SWDMX_StartFilter prPMTTbl NULL\n");
        	return (INT32)E_PSIPSR_ERROR;
        }
        u4ProgNum = prPMTTbl[prInfo->u4PMTTblIdx].u4ProgNum;
        au1Pattern[0] = 0;        
        au1Pattern[1] = (UINT8)((u4ProgNum >> 8) & 0xff);
        au1Pattern[2] = (UINT8)((u4ProgNum) & 0xff);
        au1Pattern[3] = 0;
        au1Mask[0] = 0;
        au1Mask[1] = 0xff;
        au1Mask[2] = 0xff;
        au1Mask[3] = 0;
        u4Length = 3;
        u1Offset = 3;
        u4Pid = prPMTTbl[prInfo->u4PMTTblIdx].u4PID;
        /*
        rFilter.au4Data[0] = (u4ProgNum<<8)&0x00ffff00;
        rFilter.au4Data[1] = 0;
        rFilter.au4Mask[0] = 0x00ffff00;
        rFilter.au4Mask[1] = 0;
        rFilter.u1Offset = 3;
        */
    }
    else if (prInfo->u4TblType == (UINT32)E_PSIPSR_TBL_STT)
    {
        au1Pattern[0] = 0xcd;
        au1Mask[0] = 0xff;
        u4Pid = 0x1ffb;        
    }
    else
    {
        u4Pid = 0;
        return (INT32)E_PSIPSR_ERROR;
    }
    x_memset(&rFilter, 0, sizeof(rFilter));
    rFilter.pu1Pattern = au1Pattern;
    rFilter.pu1PositiveMask = au1Mask;
    rFilter.pu1NegativeMask = NULL;    
    rFilter.u1PatternLength = (UINT8)u4Length;
    rFilter.u1Offset = u1Offset;
    rFilter.u4BufferSize = 2048*2;
    rFilter.pfnHandler = (PFN_SWDMX_CALLBACK)_SWDMX_FilterNotify;
    rFilter.fgCheckCrc = TRUE;
    rFilter.u2Pid = (UINT16)u4Pid;

    rRetCode = SWDMX_DmxPsiAllocateFilter(u1SrcId, &rFilter, &u1FilterId);
    if(rRetCode != SWDMX_E_OK)
    {
    	return (INT32)E_PSIPSR_ERROR;
    }
    *pu4Filter = u1FilterId;
    rRetCode = SWDMX_DmxPsiStartFilter(u1SrcId, u1FilterId);
    if(rRetCode != SWDMX_E_OK)
    {
    	return (INT32)E_PSIPSR_ERROR;
    }
    return (INT32)E_PSIPSR_OK;
}
#if 0
static VOID _EndFilterNew(UINT32 u4Filter)
{
    SWDMX_ERR_CODE_T rRetCode;
    rRetCode = SWDMX_DMX_PSI_ReleaseFilter(u4Filter);
    if(rRetCode != SWDMX_E_OK)
    {
        LOG(5, "SWDMX_DMX_PSI_ReleaseFilter fail...\n");
    }
}
#endif


INT32 SWDMX_PsiPsrParseTvct(UINT8 u1SrcId, PSIPSR_TBL_VCT_T* prVctTbl, UINT32* pu4NumTblEle, PSIPSR_TBL_TS2FREQ_T* prTsTbl, UINT32 u4Timeout, UINT32 u4TsIndex)
{
    INT32 i4Ret;
    PSIPSR_PARAM_T rInfo;
    if((pu4NumTblEle == NULL) || (prVctTbl == NULL))
    {
        LOG(1, "SWDMX_PsiPsrParseTvct pu4NumTblEle or prVctTbl NULL\n");
    	return (INT32)E_PSIPSR_OUT_OF_RESOURCE;
    }
    x_memset(&rInfo, 0, sizeof(PSIPSR_PARAM_T));
    rInfo.u4MaxEleNum = *pu4NumTblEle;
    rInfo.prVctTbl = prVctTbl;
    rInfo.prTsTbl = prTsTbl;
    rInfo.u4Timeout = u4Timeout;
    rInfo.u4TblType = (UINT32)E_PSIPSR_TBL_TVCT;
    rInfo.u4TsIndex = u4TsIndex;
    i4Ret = _SWDMX_ParsePsi(u1SrcId, &rInfo);
    *pu4NumTblEle = rInfo.u4RetEleNum;
    return i4Ret;
}


INT32 SWDMX_PsiPsrParseCvct(UINT8 u1SrcId, PSIPSR_TBL_VCT_T* prVctTbl, UINT32* pu4NumTblEle, PSIPSR_TBL_TS2FREQ_T* prTsTbl, UINT32 u4Timeout, UINT32 u4TsIndex)
{
    INT32 i4Ret;
    PSIPSR_PARAM_T rInfo;
    if((pu4NumTblEle == NULL) || (prVctTbl == NULL))
    {
        LOG(1, "SWDMX_PsiPsrParseCvct pu4NumTblEle or prVctTbl NULL\n");
    	return (INT32)E_PSIPSR_OUT_OF_RESOURCE;
    }
    x_memset(&rInfo, 0, sizeof(PSIPSR_PARAM_T));
    rInfo.u4MaxEleNum = *pu4NumTblEle;
    rInfo.prVctTbl = prVctTbl;
    rInfo.prTsTbl = prTsTbl;
    rInfo.u4Timeout = u4Timeout;
    rInfo.u4TblType = (UINT32)E_PSIPSR_TBL_CVCT;
    rInfo.u4TsIndex = u4TsIndex;
    i4Ret = _SWDMX_ParsePsi(u1SrcId, &rInfo);
    *pu4NumTblEle = rInfo.u4RetEleNum;
    return i4Ret;
}


INT32 SWDMX_PsiPsrParseMgt(UINT8 u1SrcId,  PSIPSR_TBL_MGT_T* prMgtTbl, UINT32* pu4NumTblEle, UINT32 u4Timeout, UINT32 u4TsIndex)
{
    INT32 i4Ret;
    PSIPSR_PARAM_T rInfo;
    if((pu4NumTblEle == NULL) || (prMgtTbl == NULL))
    {
        LOG(1, "SWDMX_PsiPsrParseMgt pu4NumTblEle or prMgtTbl NULL\n");
    	return (INT32)E_PSIPSR_OUT_OF_RESOURCE;
    }
    x_memset(&rInfo, 0, sizeof(PSIPSR_PARAM_T));
    rInfo.u4MaxEleNum = *pu4NumTblEle;
    rInfo.prMgtTbl = prMgtTbl;
    rInfo.u4Timeout = u4Timeout;
    rInfo.u4TblType = (UINT32)E_PSIPSR_TBL_MGT;
    rInfo.u4TsIndex = u4TsIndex;
    i4Ret = _SWDMX_ParsePsi(u1SrcId, &rInfo);
    *pu4NumTblEle = rInfo.u4RetEleNum;
    return i4Ret;
}

INT32 SWDMX_PsiPsrParseStt(UINT8 u1SrcId, PSIPSR_TBL_STT_T* prSttTbl, UINT32* pu4NumTblEle, UINT32 u4Timeout, UINT32 u4TsIndex)
{
    INT32 i4Ret;
    PSIPSR_PARAM_T rInfo;
    
    if((pu4NumTblEle == NULL) || (prSttTbl == NULL))
    {
        LOG(1, "SWDMX_PsiPsrParseStt pu4NumTblEle or prSttTbl NULL\n");
    	return (INT32)E_PSIPSR_OUT_OF_RESOURCE;
    }
    x_memset(&rInfo, 0, sizeof(PSIPSR_PARAM_T));
    rInfo.u4MaxEleNum = *pu4NumTblEle;
    rInfo.prSttTbl = prSttTbl;
    rInfo.u4Timeout = u4Timeout;
    rInfo.u4TblType = (UINT32)E_PSIPSR_TBL_STT;
    rInfo.u4TsIndex = u4TsIndex;
    i4Ret = _SWDMX_ParsePsi(u1SrcId, &rInfo);
    *pu4NumTblEle = rInfo.u4RetEleNum;
    return i4Ret;
}


// public function
INT32 SWDMX_PsiPsrParsePsi(UINT8 u1SrcId, PSIPSR_TBL_PMT_T* prPMTTbl, UINT32* pu4NumTblEle, UINT32 u4Timeout, UINT32 u4TsIndex)
{
    UINT32 u4Idx = 0;
    INT32 i4Ret;
    BOOL fgHitOnce = FALSE;
    PSIPSR_PARAM_T rInfo;
    
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);
    
    if((pu4NumTblEle == NULL) || (prPMTTbl == NULL))
    {
        LOG(1, "SWDMX_PsiPsrParsePsi pu4NumTblEle or prPMTTbl NULL\n");
    	return (INT32)E_PSIPSR_OUT_OF_RESOURCE;
    }
    x_memset(&rInfo, 0, sizeof(rInfo));
    rInfo.u4MaxEleNum = *pu4NumTblEle;
    rInfo.prPMTTbl = prPMTTbl;
    rInfo.u4Timeout = u4Timeout;
    rInfo.u4TblType = (UINT32)E_PSIPSR_TBL_PAT;
    rInfo.u4TsIndex = u4TsIndex;
    i4Ret = _SWDMX_ParsePsi(u1SrcId, &rInfo);
    if ((i4Ret != (INT32)E_PSIPSR_OK) || (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.u4State!=_STATE_HIT))
    {
        return (INT32)E_PSIPSR_TIMEOUT;
    }
    *pu4NumTblEle = rInfo.u4RetEleNum;
    LOG(7,"total program number: %d\n", *pu4NumTblEle);
    for ( ; u4Idx < rInfo.u4RetEleNum; u4Idx++)
    {
        rInfo.u4TblType = (UINT32)E_PSIPSR_TBL_PMT;
        rInfo.u4PMTTblIdx = u4Idx;
        i4Ret = _SWDMX_ParsePsi(u1SrcId, &rInfo);
        if (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.u4State!=_STATE_HIT)
        {
            continue;
            //return (INT32)E_PSIPSR_TIMEOUT;
        }
        fgHitOnce = TRUE;
    }
    //return (INT32)((_u4State==_STATE_HIT)?E_PSIPSR_OK:E_PSIPSR_TIMEOUT);
    return (INT32)((fgHitOnce)?E_PSIPSR_OK:E_PSIPSR_TIMEOUT); 
}


static INT32 _SWDMX_ParsePsi(UINT8 u1SrcId, PSIPSR_PARAM_T* prInfo)
{
    INT32 i4Ret = (INT32)E_PSIPSR_OK;
    static BOOL fgInitSema = FALSE;
    static BOOL afgPsiSema[SWDMX_SOURCE_MAX] = {0};
    static HANDLE_T hParseSema;

    UINT32 u4Filter = 0;
    
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);
    
    if(prInfo == NULL)
    {
        LOG(3, "_SWDMX_ParsePsi prInfo NULL\n");
    	return (INT32)E_PSIPSR_OUT_OF_RESOURCE;
    }

    if(u1SrcId >= SWDMX_SOURCE_MAX)
    {
        LOG(3, "(u1SrcId >= SWDMX_SOURCE_MAX)\n");
    	return (INT32)E_PSIPSR_OUT_OF_RESOURCE;
    }
        
    if (!fgInitSema)
    {
        VERIFY(x_sema_create(&hParseSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
        fgInitSema = TRUE;
    }
    if (!afgPsiSema[u1SrcId])
    {
        VERIFY(x_sema_create(&_ahPsiSema[u1SrcId], X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
        afgPsiSema[u1SrcId] = TRUE;
    }

    VERIFY(x_sema_lock(hParseSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    
    i4Ret = _SWDMX_StartFilter(u1SrcId, &u4Filter, prInfo);
    
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.u4State = _STATE_FILTER;
    if (i4Ret!=(INT32)E_PSIPSR_OK)
    {
        if(SWDMX_DmxPsiStopFilter(u1SrcId, (UINT8)u4Filter) != SWDMX_E_OK)
        {
            LOG(5, "SWDMX_DmxPsiStopFilter fail...\n");
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.u4State = _STATE_FAIL;
            return i4Ret;
        }
        if(SWDMX_DmxPsiReleaseFilter(u1SrcId, (UINT8)u4Filter) != SWDMX_E_OK)
        {
            LOG(5, "SWDMX_DmxPsiReleaseFilter fail...\n");
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.u4State = _STATE_FAIL;
            return i4Ret;
        }
        VERIFY(x_sema_unlock(hParseSema) == OSR_OK);
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.u4State = _STATE_FAIL;
        return i4Ret;
    }
    
    
    // ilii insert FillData here.
    if(!_SWDMX_MpgTsFillBufferStart(u1SrcId))
    {
        LOG(3, "_SWDMX_MpgTsFillBufferStart fail\n");
    }
    // ~insert
    
    if((x_sema_lock_timeout(_ahPsiSema[u1SrcId], prInfo->u4Timeout))==OSR_OK) //state==2
    {
        switch((E_PSIPSR_TBL_TYPE)prInfo->u4TblType)
        {
            case E_PSIPSR_TBL_TVCT:
            case E_PSIPSR_TBL_CVCT:
                LOG(7, "Handling VCT...\n");
                i4Ret = _SWDMX_HandleVct(u1SrcId, u4Filter, prInfo);
                break;
            case E_PSIPSR_TBL_MGT:
                LOG(7, "Handling MGT...\n");
                i4Ret = _SWDMX_HandleMgt(u1SrcId, u4Filter, prInfo);
                break;
            case E_PSIPSR_TBL_PAT:
                LOG(7, "Handling PAT...\n");
                i4Ret = _SWDMX_HandlePat(u1SrcId, u4Filter, prInfo);
                break;
            case E_PSIPSR_TBL_PMT:
                LOG(7, "Handling PMT...\n");
                i4Ret = _SWDMX_HandlePmt(u1SrcId, u4Filter, prInfo);
                break;
            case E_PSIPSR_TBL_STT:
                LOG(7, "Handling STT...\n");
                i4Ret = _SWDMX_HandleStt(u1SrcId, u4Filter, prInfo);
                break;
            default:
                break;
        }

        if ( (i4Ret != (INT32)E_PSIPSR_OK) && (i4Ret != (INT32)E_PSIPSR_SLD_NOT_MATCH))
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.u4State = _STATE_FAIL;
        }
    }
    else // timeout
    {
         // to disable interupt after semaphone timeout...
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.u4State = _STATE_FAIL;    
        i4Ret = (INT32)E_PSIPSR_TIMEOUT;
        LOG(7,"Parse table timeout!\n");
    }
    
    if(SWDMX_DmxPsiStopFilter(u1SrcId, (UINT8)u4Filter) != SWDMX_E_OK)
    {
        LOG(5, "SWDMX_DmxPsiStopFilter fail...\n");
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.u4State = _STATE_FAIL;
        return i4Ret;
    }
    if(SWDMX_DmxPsiReleaseFilter(u1SrcId, (UINT8)u4Filter) != SWDMX_E_OK)
    {
        LOG(5, "SWDMX_DmxPsiReleaseFilter fail...\n");
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.u4State = _STATE_FAIL;
        return i4Ret;
    }
    
    VERIFY(x_sema_unlock(hParseSema) == OSR_OK);

    return i4Ret;
}



/* Load PSI buffer to pu1buf
*/
static INT32 _SWDMX_GetBuffer(UINT8 u1SrcId, UINT32 u4SectionIndex, UINT8* pu1Buf, SIZE_T* pzSize)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);
    
    if((pu1Buf == NULL) || (pzSize == NULL))
    {
        LOG(1, "_SWDMX_GetBuffer prInfo or pzSize NULL\n");
    	return (INT32)E_PSIPSR_OUT_OF_RESOURCE;
    }
    
    if (SWDMX_DmxPsiCopyData(u1SrcId, (UINT8)u4SectionIndex, 
    	                                             &(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.rDmxMsg.rData), pu1Buf) != SWDMX_E_OK)
    {
        return (INT32)E_PSIPSR_TABLE_ERROR;
    }
    if (SWDMX_DmxPsiUnlockData(u1SrcId, (UINT8)u4SectionIndex, 
    	                                               &(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.rDmxMsg.rData)) != SWDMX_E_OK)
    {
        return (INT32)E_PSIPSR_TABLE_ERROR;
    }
    *pzSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.rDmxMsg.rData.u4Size;

    return (INT32)E_PSIPSR_OK;
}

static INT32 _SWDMX_HandleMgt(UINT8 u1SrcId, UINT32 u4Filter, PSIPSR_PARAM_T* prInfo)
{
    SIZE_T zSize;
    UINT32 i, j;
    UINT32 u4TableNum = 0, u4DescriptorLength;
    PSIPSR_TBL_MGT_T* prMgtTbl;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);
    
    if(prInfo == NULL)
    {
        LOG(1, "_SWDMX_HandleMgt prInfo NULL\n");
    	return (INT32)E_PSIPSR_OUT_OF_RESOURCE;
    }
    prMgtTbl = prInfo->prMgtTbl;

    zSize = 0;
    if ((E_PSIPSR_MSG)_SWDMX_GetBuffer(u1SrcId,
    	                                                             u4Filter, 
    	                                                             prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf, &zSize)!=E_PSIPSR_OK)
    {
        return (INT32)E_PSIPSR_TABLE_ERROR;
    }
    LOG(6, "Table ID: 0x%x\n", prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[0]);
    u4TableNum = (UINT32)(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[9]<<16) + 
    	                     (UINT32)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[10];
    LOG(7, "HandleMGT: u4TableNum = %d\n", u4TableNum);
    if (u4TableNum > prInfo->u4MaxEleNum)
    {
        LOG(7, "_SWDMX_HandleMgt: u4TableNum=%d exceeds _u4MaxMgtNum=%d\n", u4TableNum, prInfo->u4MaxEleNum);
        u4TableNum = prInfo->u4MaxEleNum;
    }
    //*_pu4Num = u4TableNum;
    prInfo->u4RetEleNum = u4TableNum;

    j = 11;
    for (i=0; i<u4TableNum; i++)
    {
        if (j>=zSize)
        {
            LOG(7, "_SWDMX_HandleMgt: table error, j=%d, zSize=%d\n", j, zSize);
            return (INT32)E_PSIPSR_TABLE_ERROR;
        }
        prMgtTbl[i].u4Type = (UINT32)(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[j]<<8) + 
        	                                           (UINT32)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[j+1];
        j += 2;
        prMgtTbl[i].u4Pid = ((UINT32)(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[j]<<8) +
        					   (UINT32)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[j+1]) & 0x1fff ;
        j += 2;
        prMgtTbl[i].u4Version = (UINT32)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[j] & 0x1f;
        j += 1;
        prMgtTbl[i].u4Length = ( (UINT32)(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[j]<<24) +
        						  (UINT32)(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[j+1]<<16) +
        						  (UINT32)(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[j+2]<<8) +
        						  (UINT32)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[j+3] );
        j += 4;
        u4DescriptorLength = ((UINT32)(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[j]<<8) +
        						  (UINT32)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[j+1]) & 0xfff ;

        j += (u4DescriptorLength+2);
    }

    return (INT32)E_PSIPSR_OK;
}


static INT32 _SWDMX_HandleVct(UINT8 u1SrcId, UINT32 u4Filter, PSIPSR_PARAM_T* prInfo)
{
    SIZE_T zSize;
    UINT32 i, j, k;
    // number of service location descriptor
    UINT32 u4SLDNum = 0, u4ChNum;
    UINT32 u4TotalProgNum;
    PSIPSR_TBL_TS2FREQ_T* prTsTbl;
    PSIPSR_TBL_VCT_T* prVctTbl;
    UINT32 u4MaxEleNum;

    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);
    
    if(prInfo == NULL)
    {
        LOG(1, "_SWDMX_HandleVct prInfo NULL\n");
    	return (INT32)E_PSIPSR_OUT_OF_RESOURCE;
    }
    prTsTbl = prInfo->prTsTbl;
    prVctTbl = prInfo->prVctTbl;
    u4MaxEleNum = prInfo->u4MaxEleNum;

    zSize = 0;
    if (_SWDMX_GetBuffer(u1SrcId, u4Filter, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf, &zSize) != (INT32)E_PSIPSR_OK)
    {
        return (INT32)E_PSIPSR_TABLE_ERROR;
    }
    LOG(6, "Table ID: 0x%x\n", prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[0]);
    // reset VCT
    u4TotalProgNum = 0;

    i = 3;
    prTsTbl->u2TsId = (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i]<<8) | 
    	                           prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i+1];
    prTsTbl->u1Version = (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i+2] >> 1) & 0x1f;
    u4ChNum = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i+6];
    if (u4ChNum > u4MaxEleNum)
    {
        LOG(6, "_SWDMX_HandleVct: u4ChNum=%d exceeds u4MaxVctNum=%d", u4ChNum,u4MaxEleNum);
        u4ChNum = u4MaxEleNum;
    }
    LOG(7, "_SWDMX_HandleVct: TsId=0x%02x, version=%d, u4ChNum=%d\n", prTsTbl->u2TsId,
    	prTsTbl->u1Version, u4ChNum);
    i += 7;
    for (j = 0; j < (UINT32)u4ChNum; j++)
    {
        UINT8 u1ElemNum;
        UINT16 u2TotalDescLen; // descriptor length
        UINT32 u4ServiceType;

        for (k=0; k<7; k++)
        {
        	prVctTbl->szShortName[k] = (UCHAR)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i + (k*2) +1];
        }
        prVctTbl->szShortName[k] = (UCHAR)'\0';

        i += 14;
        prVctTbl->u2MajorChNum = ((UINT16)(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i]<<6) |
        						  (UINT16)(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i+1]>>2)) & 0x3ff;
        prVctTbl->u2MinorChNum = ((UINT16)(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i+1]<<8) |
        						  (UINT16)(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i+2])) & 0x3ff;
        i += 8; // 3+5
        prVctTbl->u2ChTsId = (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i] << 8) | 
        	                                 prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i+1];
        prVctTbl->u2ProgNum = (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i+2] << 8) | 
        	                                  prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i+3];
        i += 5; //4+1
        u4ServiceType = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i] & 0x3f;
        i += 3;
        u2TotalDescLen = ((UINT16)(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i]<<8) | 
        	                           (UINT16)(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i+1])) & 0x3ff;
        i += 2;

        LOG(7, "\tname=%.7s, chNum=%d.%d, chTsId=0x%02x, progNum=%d, sType=0x%02x\n",
        	prVctTbl->szShortName, prVctTbl->u2MajorChNum, prVctTbl->u2MinorChNum,
        	prVctTbl->u2ChTsId, prVctTbl->u2ProgNum, u4ServiceType);

        if ( (i + u2TotalDescLen) > zSize)
        {
            LOG(6, "_SWDMX_HandleVct: table error, i=%d, u2TotalDescLen=%d, zSize=%d\n",
        	i, u2TotalDescLen, zSize);
            return (INT32)E_PSIPSR_TABLE_ERROR;
        }

        // descriptor
        while (u2TotalDescLen > 0)
        {
            UINT8 u1DescType = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i];
            UINT8 u1DescLen = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i+1];

            if ( (i+u1DescLen) > zSize)
            {
                LOG(6, "_SWDMX_HandleVct: table error, i=%d, u1DescLen=%d, zSize=%d\n",
                	i, u1DescLen, zSize);
                return (INT32)E_PSIPSR_TABLE_ERROR;
            }

            // service location descriptor
            if (u1DescType==VCT_SERVICE_LOCATION_DESCRIPTOR)
            {
                UINT32 u4NumAPID = 0;

                i += 2;
                prVctTbl->u2PCR = ((UINT16)(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i]<<8) | \
                				   (UINT16)(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i+1])) & 0x1fff;
                u1ElemNum = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i+2];
                i += 3;

                LOG(7, "\t\tPCR=0x%02x\n", prVctTbl->u2PCR);

                for (k=0; k<u1ElemNum; k++)
                {
                    UINT16 u2Type = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i];
                    UINT16 u2PID = ((UINT16)(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i+1]<<8) | \
                    				(UINT16)(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i+2])) & 0x1fff;

                    LOG(7, "\t\ttype=0x%02x, PID=0x%02x\n", u2Type, u2PID);

                    if ((u2Type == STREAM_TYPE_VIDEO) || 
                        (u2Type == STREAM_TYPE_H264) ||
                        (u2Type == STREAM_TYPE_VC1) ||
                        (u2Type == STREAM_TYPE_AVS))
                    {
                        prVctTbl->u2VPID = u2PID;
                        prVctTbl->u2VPIDType = u2Type;
                    }
                    else if ( (u2Type==STREAM_TYPE_MPEG1) ||
                        (u2Type==STREAM_TYPE_MPEG2) ||
                        (u2Type==STREAM_TYPE_AC3) ||
                        (u2Type==STREAM_TYPE_AC3_EU) ||
                        (u2Type==STREAM_TYPE_EAC3) ||
                        (u2Type==STREAM_TYPE_AAC) ||
                        (u2Type==STREAM_TYPE_AAC_ISDB))
                    {
                        if (u4NumAPID == 0)
                        {
                        	prVctTbl->u2APID1 = u2PID;
                        	prVctTbl->u2APID1Type = u2Type;
                        	u4NumAPID++;
                        }
                        else if (u4NumAPID == 1)
                        {
                        	prVctTbl->u2APID2 = u2PID;
                        	prVctTbl->u2APID2Type = u2Type;
                        	u4NumAPID++;
                        }
                        else if (u4NumAPID == 2)
                        {
                        	prVctTbl->u2APID3 = u2PID;
                        	prVctTbl->u2APID3Type = u2Type;
                        	u4NumAPID++;
                        }

                        /*
                        if (prVctTbl->u2APID1==0)
                        {
                        	prVctTbl->u2APID1 = u2PID;
                        	prVctTbl->u2APID1Type = u2Type;
                        }
                        else if (prVctTbl->u2APID2==0)
                        {
                        	prVctTbl->u2APID2 = u2PID;
                        	prVctTbl->u2APID2Type = u2Type;
                        }
                        else if (prVctTbl->u2APID3==0)
                        {
                        	prVctTbl->u2APID3 = u2PID;
                        	prVctTbl->u2APID3Type = u2Type;
                        }
                        */
                	}
                	i += 6;
                }
            	u4SLDNum++;
            }
            else
            {
            	i += u1DescLen+2;
            }
            u2TotalDescLen -= (u1DescLen+2);
        } // while descriptor
    	if (u4ServiceType==VCT_SERVICE_TYPE_ATSC_DTV)
    	{
    		prVctTbl++;
    		u4TotalProgNum++;
    	}
    	else // don't support other service yet
    	{
    	}
    } // for

    //*_pu1ChNum = u4TotalProgNum;
    prInfo->u4RetEleNum = u4TotalProgNum;

    // check if number of service descriptor matches with program number,
    // if not, parse PAT/PMT
    if (u4TotalProgNum != u4SLDNum)
    {
        return (INT32)E_PSIPSR_SLD_NOT_MATCH;
    }
    return (INT32)E_PSIPSR_OK;
}

static INT32 _SWDMX_HandlePat(UINT8 u1SrcId, UINT32 u4Filter, PSIPSR_PARAM_T* prInfo)
{
    SIZE_T zSize;
    UINT32 i;
    UINT32 j;
    UINT32 u4ProgNum;
    UINT32 u4Pid;

    UINT32 u4SecLength;

    UINT32 u4TotalProgNum;
    PSIPSR_TBL_PMT_T* arPMTTbl;
    
     SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);
    
    if(prInfo == NULL)
    {
        LOG(1, "_SWDMX_HandlePat prInfo NULL\n");
    	return (INT32)E_PSIPSR_OUT_OF_RESOURCE;
    }
    arPMTTbl = prInfo->prPMTTbl;

    zSize = 0;
    if ((E_PSIPSR_MSG)_SWDMX_GetBuffer(u1SrcId, u4Filter, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf, &zSize)!=E_PSIPSR_OK)
    {
        LOG(3, "_SWDMX_HandlePat _SWDMX_GetBuffer E_PSIPSR_TABLE_ERROR\n");
    	return (INT32)E_PSIPSR_TABLE_ERROR;
    }
    LOG(6, "Table ID: 0x%x\n", prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[0]);
    // reset data
    u4TotalProgNum = 0;
    x_memset(arPMTTbl, 0, sizeof(arPMTTbl));

    u4SecLength = ((UINT32)(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[1]<<8) |
    	                      (UINT32)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[2]) & 0xfff;
    LOG(6, "Buf length: %d, Section Length: %d\n", zSize, u4SecLength);

    //for (i=8; i<zSize-4; i+=4)
    for (i=8; i < (u4SecLength - 1); i+=4)
    {
        if (u4TotalProgNum==SWDMX_MAX_PMT_NUM)
        {
            LOG(6, "_SWDMX_HandlePat: u4TotalProgNum=%d is full\n", u4TotalProgNum);
            break;
        }
        u4ProgNum = (UINT32)(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i] << 8) | 
        	                                 (UINT32)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i+1];
        u4Pid = ((UINT32)(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i+2] << 8) | 
        	                           (UINT32)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i+3]) & 0x1fff;
        for (j=0;j<SWDMX_MAX_PMT_NUM;j++)
        {
            if ((u4ProgNum!=0) &&
                (arPMTTbl[j].u4ProgNum != u4ProgNum))
            {
                arPMTTbl[u4TotalProgNum].u4PID = u4Pid;
                arPMTTbl[u4TotalProgNum].u4ProgNum = u4ProgNum;

                u4TotalProgNum++;
                break;
            }
        }
        LOG(7, "_SWDMX_HandlePat: u4ProgNum=0x%x, u4Pid=0x%x\n", u4ProgNum, u4Pid);
    }
    prInfo->u4RetEleNum = u4TotalProgNum;
    return (INT32)E_PSIPSR_OK;
}


static INT32 _SWDMX_HandlePmt(UINT8 u1SrcId, UINT32 u4Filter, const PSIPSR_PARAM_T* prInfo)
{
    SIZE_T zSize;
    UINT32 i;
    UINT32 u4Pid;
    PSIPSR_TBL_PMT_T* prPMTTbl;
    UINT32 u4VPidCount = 0;
    UINT32 u4APidCount = 0;

    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);
    
    if(prInfo == NULL)
    {
        LOG(1, "_SWDMX_HandlePmt prInfo NULL\n");
    	return (INT32)E_PSIPSR_OUT_OF_RESOURCE;
    }
    zSize = 0;
    x_memset(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf, 0, NVGT_MAX_TABLE);
    if ((E_PSIPSR_MSG)_SWDMX_GetBuffer(u1SrcId,
    	                                                            u4Filter, 
    	                                                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf, &zSize)!=E_PSIPSR_OK)
    {
        LOG(3, "_SWDMX_HandlePmt _SWDMX_GetBuffer E_PSIPSR_TABLE_ERROR\n");
    	return (INT32)E_PSIPSR_TABLE_ERROR;
    }
    LOG(6, "Table ID: 0x%x\n", prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[0]);
    prPMTTbl = prInfo->prPMTTbl + prInfo->u4PMTTblIdx;// &arPMTTbl[u4ChNo];

    prPMTTbl->u4PCR = ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[8]<<8) +
    	                                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[9]) & 0x1fff;
    i = (UINT32)(12 + (((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[10] << 8) + 
    	                           prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[11]) & 0xfff));
    while ( (i + 1) < (zSize - 4) )
    {
        u4Pid = ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i+1]<<8) + 
        	             prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i+2]) & 0x1fff;
        LOG(8, "_SWDMX_HandlePmt: type=0x%x, u4Pid=0x%x\n", prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i], u4Pid);
        if (( prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i] == STREAM_TYPE_VIDEO) ||
            /*(_au1Buf[i] == 0x80 && prPMTTbl->u4VPID==0) || */
            ( prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i] == STREAM_TYPE_H264) ||
            ( prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i] == STREAM_TYPE_VC1) ||
            ( prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i] == STREAM_TYPE_AVS) ||
            ( prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i] == STREAM_TYPE_MVC))
        {
            UINT32 u4EsLength = 0;
            UINT32 u4Idx = (i+5);
            UINT32 u4FormatIdentifier = 0;
            u4EsLength = ( prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i+3] & 0xF);
            u4EsLength <<= 8;
            u4EsLength +=  prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i+4];
            if(u4VPidCount == 0)
            {
                prPMTTbl->u4VPID = u4Pid;
                prPMTTbl->u4VPIDType =  prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i];
                u4VPidCount++;
            }
            else if(u4VPidCount == 1)
            {
                if(( prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i] == STREAM_TYPE_MVC) &&
                   (prPMTTbl->u4VPIDType == STREAM_TYPE_H264))
                {
                    prPMTTbl->u4SubVPID = u4Pid;
                    prPMTTbl->u4SubVPIDType = STREAM_TYPE_MVC;
                }
                else
                {
                    prPMTTbl->u4VPID2 = u4Pid;
                    prPMTTbl->u4VPID2Type =  prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i];
                    u4VPidCount++;
                }
            }
            while(u4EsLength > 0)
            {
                x_memcpy(&u4FormatIdentifier,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf+u4Idx+2,
                    4);
                if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[u4Idx] == DESCRIPTOR_TAG_ID_MVC) &&
                   (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[u4Idx+1] == 8)) // MVC header size = 8
                {
                    if(prPMTTbl->u4VPIDType == STREAM_TYPE_MVC)
                    {
                        x_memcpy(prPMTTbl->au1VData,
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf+u4Idx+2, SWDMX_MVC_HDR_SIZE);
                    }
                    break;
                }
                else if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[u4Idx] ==
                         DESCRIPTOR_TAG_ID_REGISTRATION) &&
                        (u4FormatIdentifier == FORMAT_IDENTIFIER_RD3D))
                {
                    UCHAR *pucBuf = (UCHAR*)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf+u4Idx+6;
                    if(*pucBuf == 0) // RealD_descriptor_type == 0, an RD3D program
                    {
                        pucBuf ++;
                        if(*pucBuf == 0)
                        {
                            // Real D critically sampled method
                        }
                        else if((*pucBuf == 0x80))
                        {
                            // RealD non critically sampled method
                        }
                    }
                    else if(*pucBuf == 1) // base layer
                    {
                        // already assigned
                        //prPMTTbl->u4VPID = u4Pid;
                        //prPMTTbl->u4VPIDType =  prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i];
                    }
                    else if(*pucBuf == 2) // enhancement layer
                    {
                        prPMTTbl->u4SubVPID = u4Pid;
                        prPMTTbl->u4SubVPIDType = STREAM_TYPE_MULTITRACK;
                    }
                }
                if(u4EsLength > ( prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[u4Idx+1] + 2))
                {
                    u4EsLength -= ( prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[u4Idx+1] + 2);
                }
                else
                {
                    u4EsLength = 0;
                }
                u4Idx += ( prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[u4Idx+1] + 2);
            }
        }
        else if (( prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i]==STREAM_TYPE_MPEG1) ||
            ( prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i]==STREAM_TYPE_MPEG2) ||
            ( prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i]==STREAM_TYPE_AC3) ||
            ( prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i]==STREAM_TYPE_AC3_EU) ||
            ( prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i]==STREAM_TYPE_EAC3) ||
            ( prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i]==STREAM_TYPE_AAC) ||
            ( prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i]==STREAM_TYPE_AAC_ISDB))            
        {
            if(u4APidCount == 0)
            {
                prPMTTbl->u4APID1 = u4Pid;
                prPMTTbl->u4APID1Type =  prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i];
                u4APidCount++;
            }
            else if(u4APidCount == 1)
            {
                prPMTTbl->u4APID2 = u4Pid;
                prPMTTbl->u4APID2Type =  prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i];
                u4APidCount++;
            }
            else if(u4APidCount == 2)
            {
                prPMTTbl->u4APID3 = u4Pid;
                prPMTTbl->u4APID3Type =  prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i];
                u4APidCount++;
            }
            /*
            if (prPMTTbl->u4APID1==0)
            {
            	prPMTTbl->u4APID1 = u4Pid;
            	prPMTTbl->u4APID1Type = au1Buf[i];
            }
            else if (prPMTTbl->u4APID2==0)
            {
            	prPMTTbl->u4APID2 = u4Pid;
            	prPMTTbl->u4APID2Type = au1Buf[i];
            }
            else if (prPMTTbl->u4APID3==0)
            {
            	prPMTTbl->u4APID3 = u4Pid;
            	prPMTTbl->u4APID3Type = au1Buf[i];
            }
            */
        }
        else if ( prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i]==STREAM_TYPE_CYPHER)
        {
            UINT32 u4EsLength = 0;
            UINT32 u4Idx = (i+5);
            UINT32 u4FormatIdentifier = 0;
            u4EsLength = ( prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i+3] & 0xF);
            u4EsLength <<= 8;
            u4EsLength +=  prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i+4];
            while(u4EsLength > 0)
            {
                x_memcpy(&u4FormatIdentifier,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf+u4Idx+2,
                    4);
                if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[u4Idx] ==
                         DESCRIPTOR_TAG_ID_LPCM) &&
                        (u4FormatIdentifier == FORMAT_IDENTIFIER_80_LPCM))
                {
                    if(u4APidCount == 0)
                    {
                        prPMTTbl->u4APID1 = u4Pid;
                        prPMTTbl->u4APID1Type = STREAM_TYPE_SWDMX_LPCM;
                        u4APidCount++;
                    }
                    else if(u4APidCount == 1)
                    {
                        prPMTTbl->u4APID2 = u4Pid;
                        prPMTTbl->u4APID2Type = STREAM_TYPE_SWDMX_LPCM;
                        u4APidCount++;
                    }
                    else if(u4APidCount == 2)
                    {
                        prPMTTbl->u4APID3 = u4Pid;
                        prPMTTbl->u4APID3Type = STREAM_TYPE_SWDMX_LPCM;
                        u4APidCount++;
                    }
                    break;
                }
                if(u4EsLength > ( prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[u4Idx+1] + 2))
                {
                    u4EsLength -= ( prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[u4Idx+1] + 2);
                }
                else
                {
                    u4EsLength = 0;
                }
                u4Idx += ( prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[u4Idx+1] + 2);
            }
        }
        else if ( prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i]==STREAM_TYPE_USER_DEFINE)
        {
            UINT32 u4EsLength = 0;
            UINT32 u4Idx = (i+5);
            UINT32 u4FormatIdentifier = 0;
            u4EsLength = ( prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i+3] & 0xF);
            u4EsLength <<= 8;
            u4EsLength +=  prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i+4];
            while(u4EsLength > 0)
            {
                x_memcpy(&u4FormatIdentifier,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf+u4Idx+2,
                    4);
                if( prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[u4Idx] == DESCRIPTOR_TAG_ID_AC3)
                {
                    if(u4APidCount == 0)
                    {
                        prPMTTbl->u4APID1 = u4Pid;
                        prPMTTbl->u4APID1Type = STREAM_TYPE_AC3;
                        u4APidCount++;
                    }
                    else if(u4APidCount == 1)
                    {
                        prPMTTbl->u4APID2 = u4Pid;
                        prPMTTbl->u4APID2Type = STREAM_TYPE_AC3;
                        u4APidCount++;
                    }
                    else if(u4APidCount == 2)
                    {
                        prPMTTbl->u4APID3 = u4Pid;
                        prPMTTbl->u4APID3Type = STREAM_TYPE_AC3;
                        u4APidCount++;
                    }
                    break;
                }
                else if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[u4Idx] ==
                         DESCRIPTOR_TAG_ID_LPCM) &&
                        (u4FormatIdentifier == FORMAT_IDENTIFIER_05_LPCM))
                {
                    if(u4APidCount == 0)
                    {
                        prPMTTbl->u4APID1 = u4Pid;
                        prPMTTbl->u4APID1Type = STREAM_TYPE_SWDMX_LPCM;
                        u4APidCount++;
                    }
                    else if(u4APidCount == 1)
                    {
                        prPMTTbl->u4APID2 = u4Pid;
                        prPMTTbl->u4APID2Type = STREAM_TYPE_SWDMX_LPCM;
                        u4APidCount++;
                    }
                    else if(u4APidCount == 2)
                    {
                        prPMTTbl->u4APID3 = u4Pid;
                        prPMTTbl->u4APID3Type = STREAM_TYPE_SWDMX_LPCM;
                        u4APidCount++;
                    }
                    break;
                }
                if(u4EsLength > ( prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[u4Idx+1] + 2))
                {
                    u4EsLength -= ( prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[u4Idx+1] + 2);
                }
                else
                {
                    u4EsLength = 0;
                }
                u4Idx += ( prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[u4Idx+1] + 2);
            }
        }

        i += 5 + ((( prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i+3] << 8) +  
        	             prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[i+4]) & 0xfff);
    }

    LOG(7, "_SWDMX_HandlePmt: u4Filter=%d, vpid=0x%x, apid1=0x%x, apid2=0x%x, apid3=0x%x\n",
	u4Filter, prPMTTbl->u4VPID, prPMTTbl->u4APID1, prPMTTbl->u4APID2, prPMTTbl->u4APID3);

    return (INT32)E_PSIPSR_OK;
}


static INT32 _SWDMX_HandleStt(UINT8 u1SrcId, UINT32 u4Filter, const PSIPSR_PARAM_T* prInfo)
{
    SIZE_T zSize;
    PSIPSR_TBL_STT_T* prSttTbl;
    
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);
    
    if(prInfo == NULL)
    {
        LOG(1, "_SWDMX_HandlePmt prInfo NULL\n");
    	return (INT32)E_PSIPSR_OUT_OF_RESOURCE;
    }
    zSize = 0;
    x_memset(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf, 0, NVGT_MAX_TABLE);
    if ((E_PSIPSR_MSG)_SWDMX_GetBuffer(u1SrcId,
    	                                                            u4Filter, 
    	                                                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf, &zSize)!=E_PSIPSR_OK)
    {
        LOG(3, "_SWDMX_HandleStt _SWDMX_GetBuffer E_PSIPSR_TABLE_ERROR\n");
    	return (INT32)E_PSIPSR_TABLE_ERROR;
    }
    LOG(6, "Table ID: 0x%x\n", prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[0]);
    prSttTbl = prInfo->prSttTbl;

    prSttTbl->u4SysTime = (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[9] << 24) |
    	                                  (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[10] << 16) | 
    	                                  (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[11] << 8) | 
    	                                  prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[12];
    prSttTbl->u1GPSUTCOffset = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[13] & 0xff;
    prSttTbl->u2DayLightSavings = (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[14] << 8) | 
    	                                              (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rPsi.au1Buf[15] & 0xffff);

    return (INT32)E_PSIPSR_OK;
}
#if 0
static INT32 _AllocatePidEntry(UINT32 u4Pid, UINT32* pu4PidEntry)
{
    UINT16 u2Entry, u2Tmp;
    BOOL fgEnable, fgAllocate=FALSE;
    DMX_PID_T rPid;


    if(pu4PidEntry == NULL)
    {
    	return (INT32)E_PSIPSR_ERROR;
    }
    u2Entry = (*pu4PidEntry< DMX_NUM_PID_INDEX)?*pu4PidEntry:0;

    for (; u2Entry<DMX_NUM_PID_INDEX; u2Entry++)
    {
        VERIFY(DMX_GetPid(u2Entry, DMX_PID_FLAG_VALID, &rPid));
        fgEnable = rPid.fgEnable;
        if (fgEnable)
        {

            VERIFY(DMX_GetPid(u2Entry, DMX_PID_FLAG_PID, &rPid));
            u2Tmp = rPid.u2Pid;
            if (u4Pid==u2Tmp)
            {
                *pu4PidEntry = u2Entry;
                return (INT32)E_PSIPSR_OK;
            }
        }
        else if (!fgAllocate)// a free one
        {
            *pu4PidEntry = u2Entry;
            fgAllocate = TRUE;
        }
    }
    if (fgAllocate)
    {
        return (INT32)E_PSIPSR_OK;
    }
    return (INT32)E_PSIPSR_ERROR;

}
#endif
#if 0
static INT32 _AllocateFilter(UINT32 u4Pidx, UINT32* pu4Filter)
{
    UINT16 u2Entry, u2Tmp;
    BOOL fgEnable, fgAllocate=FALSE;
    DMX_FILTER_T rFilter;


    if(pu4Filter == NULL)
    {
    	return (INT32)E_PSIPSR_ERROR;
    }
    u2Entry = (*pu4Filter< DMX_NUM_FILTER_INDEX)?*pu4Filter:0;

    for (; u2Entry<DMX_NUM_FILTER_INDEX; u2Entry++)
    {
        VERIFY(DMX_GetFilter(u2Entry, 0, DMX_FILTER_FLAG_VALID, &rFilter));
        fgEnable = rFilter.fgEnable;
        if (fgEnable)
        {

            VERIFY(DMX_GetFilter(u2Entry, 0, DMX_FILTER_FLAG_PIDX, &rFilter));
            u2Tmp = rFilter.u1Pidx;
            if (u4Pidx == u2Tmp)
            {
                *pu4Filter = u2Entry;
                return (INT32)E_PSIPSR_OK;
            }
        }
        else if (!fgAllocate)// a free one
        {
            *pu4Filter = u2Entry;
            fgAllocate = TRUE;
        }
    }
    if (fgAllocate)
    {
    	return (INT32)E_PSIPSR_OK;
    }
    return (INT32)E_PSIPSR_ERROR;

}
#endif

#define MAX_COUNT_YEAR                  99
// UTC bounds
//#define UTC_LOWER_BOUND                 946656000   // Jan  1 00:00:00, 2000
#define UTC_LOWER_BOUND                 (630720000-28800)   // Jan  1 00:00:00, 2000 since Jan 6 00:00:00 1980
#define UTC_UPPER_BOUND                 2147483640  // Jan 19 11:14:00, 2038

/*
static const UINT8 _au1MonthDays[12] =
    { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

static BOOL _SWDMX_IsLeapYear(UINT8 u1Year)
{
    return (((u1Year % 4) == 0) &&
        (((u1Year % 100) != 0) || ((u1Year % 400) == 0)));
}
BOOL SWDMX_PSIPSR_UtcToRtc(PSIPSR_RTC_T* prRtc, UINT64 u8Utc)
{
    UINT32 u4Days, u4Seconds;
    UINT8 u1Year, u1Month;
    BOOL fgLeapYear;
    const UINT32 u4SecondsPerDay = 24 * 60 * 60;

    #if 1//#ifdef __KERNEL__
    UINT64 u8Remainder;
    #endif
        
    if(prRtc == NULL)
    {
        return FALSE;
    }

    // Check if the given UTC is big enough
    if ( (u8Utc < UTC_LOWER_BOUND) || (u8Utc > UTC_UPPER_BOUND))
    {
        return FALSE;
    }

    // Compute total days and remaining seconds
    u8Utc -= UTC_LOWER_BOUND;
    #if 1//#ifdef __KERNEL__
    u4Days = u8Div6432(u8Utc, (UINT64)u4SecondsPerDay, &u8Remainder);
    #else
    u4Days = (UINT32)(u8Utc / u4SecondsPerDay);
    #endif            
    u4Seconds = (UINT32)(u8Utc - ((UINT64)u4Days * u4SecondsPerDay));

    // Compute seconds, minutes and hours
    prRtc->u1Second = (UINT8)(u4Seconds % 60);
    u4Seconds /= 60;
    prRtc->u1Minute = (UINT8)(u4Seconds % 60);
    u4Seconds /= 60;
    VERIFY(u4Seconds < 24);
    prRtc->u1Hour = (UINT8)(u4Seconds);

    // Compute years
    u1Year = 0;
    while (u4Days > 0)
    {
        UINT32 u4YearDays;

        if (_SWDMX_IsLeapYear(u1Year))
        {
            u4YearDays = 366;
        }
        else
        {
            u4YearDays = 365;
        }

        if (u4Days >= u4YearDays)
        {
            u4Days -= u4YearDays;
            u1Year++;
        }
        else
        {
            break;
        }
    }
    if (u1Year > MAX_COUNT_YEAR)
    {
        return FALSE;
    }

    // Compute months
    fgLeapYear = _SWDMX_IsLeapYear(u1Year);
    u1Month = 0;
    while (u4Days > 0)
    {
        UINT8 u1MonthDays;

        u1MonthDays = _au1MonthDays[u1Month];
        if ( (u1Month == 1) && fgLeapYear)
        {
            u1MonthDays++;
        }

        if (u4Days >= u1MonthDays)
        {
            u4Days -= u1MonthDays;
            u1Month++;
        }
        else
        {
            break;
        }
    }
    VERIFY(u1Month < 12);

    prRtc->u1Year = u1Year;
    prRtc->u1Month = u1Month + 1;
    prRtc->u1Day = (UINT8)(u4Days + 1);

    return TRUE;
}*/
