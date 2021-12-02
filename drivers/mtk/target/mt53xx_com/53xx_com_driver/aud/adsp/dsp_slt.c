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
#include "dsp_reg.h"
#include "dsp_intf.h"
#include "dsp_slt.h"
#include "dsp_rg_ctl.h"
#include "adsp_task.h"
 
#include "drv_common.h"

#include "x_printf.h"
#include "x_os.h"
#include "x_hal_5381.h"

#if 0 //def __MODEL_slt__

/******************************************************************************
* Compile Time Option
******************************************************************************/
#define     _BYPASS_PUTBITS
#define     _MAX_WAIT_DSP_CNT       ((UINT32)(0x2FFFFFFFF))

/******************************************************************************
* Global Varibles
******************************************************************************/
UINT8 u1ADSPSLTPatID;

/******************************************************************************
* External Varibles
******************************************************************************/
//extern UINT8  _ucAdspWorkingBuffer[];
extern UINT32 _u4DspBuf;
//extern UINT8  _pbDSLT[];//build error
/******************************************************************************
* External Modules
******************************************************************************/
extern void AUD_DspClkSel(UINT8);
extern void vResetAndHoldDSP (void);
extern void vTriggerDSP (void);
extern void vSetDspBootFromRam (void);
extern void vSetCacheSize (UINT8 bSize);
extern void vSetCacheInstLen (UINT8 bSize);
extern void vSetCmptRDThreshold(UINT8 bThreshold);

/******************************************************************************
* Function prototype
******************************************************************************/
static void _ADSP_SLT_hdl(VOID*  pv_arg);
static void _ADSP_SLT_hdl_init(void);
static void vDspLoadTestPattern(UINT8 u1Type);

/******************************************************************************
* static varibles
******************************************************************************/
static UINT32 *pu4ADSPslt_done;
static UINT32 *pu4ADSPslt_rslt;
static UINT32  u4RunCnt;

/******************************************************************************
* Function      : _ADSP_GetDRAMDec1_Addr(addr)
* Description   : Get Decoder #1 DRAM Address
* Parameter     : None
* Return        : None
******************************************************************************/
#define _ADSP_GetDRAMDec1_Addr(addr)   ( (UINT32*)( _u4DspDramBuf[TYPE_DEC1_DRAM_IDX] + ((addr)<<2) ) )

/******************************************************************************
* Function      : _ADSP_SLT_hdl(void)
* Description   : ADSP SLT service routine
* Parameter     : None
* Return        : None
******************************************************************************/
static void _ADSP_SLT_hdl(VOID*  pv_arg)
{
    UNUSED(pv_arg);
    Printf("ADSP SLT service routine entering....\n");
    
    while (1)
    {
_ADSP_SLT_STR:
#ifdef _BYPASS_PUTBITS
        if (u1ADSPSLTPatID == ADSP_TEST_PATTERN5)
        {
            Printf("\n");
            Printf("Pattern #5 has been skipped.\n");
            Printf("Please try another test pattern.\n");
            u1ADSPSLTPatID = 0;
        }
        else if ( (u1ADSPSLTPatID != ADSP_TEST_PATTERNNULL) && (u1ADSPSLTPatID <= ADSP_PATTERN_CNT) && 
                  (u1ADSPSLTPatID != ADSP_TEST_PATTERN5) )
#else
        if ( (u1ADSPSLTPatID != ADSP_TEST_PATTERNNULL) && (u1ADSPSLTPatID <= ADSP_PATTERN_CNT) )
#endif
        {
            UINT32  u4Wait_cnt = 0;
            u4RunCnt++;
            _ADSP_CFG_Reset();

#ifdef _BYPASS_PUTBITS
            if (u1ADSPSLTPatID == ADSP_TEST_PATTERN5)
            {
                Printf("\n");
                Printf("Pattern #5 has been skipped.\n");
                Printf("Please try another test pattern.\n");
                goto _ADSP_PID_RESET;
            }
            else
#endif
            {
                // Load specified pattern
                vDspLoadTestPattern(u1ADSPSLTPatID);
            }

            Printf("Pattern #%d testing..", u1ADSPSLTPatID);
            
            // Wait DSP Response
            while ( (*pu4ADSPslt_done == 0) && (u4Wait_cnt < _MAX_WAIT_DSP_CNT) )
            {
                u4Wait_cnt++;
                if ( (u4Wait_cnt % 32768) == 0)
                {
                    Printf(".");
                }
            }

            Printf("\n");

            // Check Test Result
            if (*pu4ADSPslt_done !=0 )
            {
                if (*pu4ADSPslt_rslt ==0 )
                {
                    Printf("Test Passed.\n");
                }
                else
                {
                    Printf("******* Test Failed!!!!!!\n");
                    goto _ADSP_SLT_ERROR;
                }
            }
            else
            {
                Printf("ADSP No Response!!\n");
                goto _ADSP_SLT_ERROR;
            }
_ADSP_PID_RESET:
            // Reset
            #if 0
            //u1ADSPSLTPatID = 0;
            #else
            #if 1
            // run all patterns sequencially
            if (u1ADSPSLTPatID < ADSP_PATTERN_CNT)
            {
                u1ADSPSLTPatID++;
            }
            else
            {
                u1ADSPSLTPatID=1;
            }

            if (u1ADSPSLTPatID == 5) 
            {
                u1ADSPSLTPatID++;
            }
            #endif
            #endif
        }
    }

_ADSP_SLT_ERROR:
    Printf("Error Pattern: %d", u1ADSPSLTPatID);
    Printf("Total Run: %d", u4RunCnt);
    // Reset
    u1ADSPSLTPatID = 0;
    u4RunCnt = 0;

    goto _ADSP_SLT_STR;
    
}


/******************************************************************************
* Function      : u4GetSLTDspImgAddr(void)
* Description   : Get ADSPfullSLT.c
* Parameter     : None
* Return        : None
******************************************************************************/
/*
static UINT32 u4GetSLTDspImgAddr(void)
{
    return (UINT32)(&_pbDSLT[0x20]); // point to header of flash file 
}
*/

/******************************************************************************
* Function      : vDspLoadTestPattern(UINT8 u1Type)
* Description   : Load ADSP Test pattern
* Parameter     : None
* Return        : None
******************************************************************************/
static void vDspLoadTestPattern(UINT8 u1Type)
{
    // Down Load From Flash
    UINT32  dwDECICAddr;
    UINT8   uRamCodeType;
    UINT32  dwOfst, dwSz;
    BOOL    fgLoadCode;
    UINT32  dwFlashOfst;

    fgLoadCode  = FALSE;
    //dwFlashOfst = u4GetSLTDspImgAddr ();   // set to dsp image addr//build error
    switch (u1Type)
    {
    case ADSP_TEST_PATTERN1:
        dwOfst  = u4GetFlashData (dwFlashOfst + (DSPF_MEM_PATTERN_SA << 2));
        dwSz    = u4GetFlashData (dwFlashOfst + (DSPF_MEM_PATTERN_SZ << 2));
        pu4ADSPslt_done = _ADSP_GetDRAMDec1_Addr(ADSPSLT_PAT_1_DONE);
        pu4ADSPslt_rslt = _ADSP_GetDRAMDec1_Addr(ADSPSLT_PAT_1_RSLT);
        uRamCodeType = TYPE_ROM_ICACHE;
        fgLoadCode   = TRUE;
        break;
    case ADSP_TEST_PATTERN2:
        dwOfst  = u4GetFlashData (dwFlashOfst + (DSPF_INST_PATTERN_SA << 2));
        dwSz    = u4GetFlashData (dwFlashOfst + (DSPF_INST_PATTERN_SZ << 2));
        pu4ADSPslt_done = _ADSP_GetDRAMDec1_Addr(ADSPSLT_PAT_2_DONE);
        pu4ADSPslt_rslt = _ADSP_GetDRAMDec1_Addr(ADSPSLT_PAT_2_RSLT);
        uRamCodeType = TYPE_ROM_ICACHE;
        fgLoadCode   = TRUE;
        break;
    case ADSP_TEST_PATTERN3:
        dwOfst  = u4GetFlashData (dwFlashOfst + (DSPF_DRAM_PATTERN_SA << 2));
        dwSz    = u4GetFlashData (dwFlashOfst + (DSPF_DRAM_PATTERN_SZ << 2));
        pu4ADSPslt_done = _ADSP_GetDRAMDec1_Addr(ADSPSLT_PAT_3_DONE);
        pu4ADSPslt_rslt = _ADSP_GetDRAMDec1_Addr(ADSPSLT_PAT_3_RSLT);
        uRamCodeType = TYPE_ROM_ICACHE;
        fgLoadCode   = TRUE;
        break;
    case ADSP_TEST_PATTERN4:
        dwOfst  = u4GetFlashData (dwFlashOfst + (DSPF_GETBITS_PATTERN_SA << 2));
        dwSz    = u4GetFlashData (dwFlashOfst + (DSPF_GETBITS_PATTERN_SZ << 2));
        pu4ADSPslt_done = _ADSP_GetDRAMDec1_Addr(ADSPSLT_PAT_4_DONE);
        pu4ADSPslt_rslt = _ADSP_GetDRAMDec1_Addr(ADSPSLT_PAT_4_RSLT);
        uRamCodeType = TYPE_ROM_ICACHE;
        fgLoadCode   = TRUE;
        break;
    case ADSP_TEST_PATTERN5:
        dwOfst  = u4GetFlashData (dwFlashOfst + (DSPF_PUTBITS_PATTERN_SA << 2));
        dwSz    = u4GetFlashData (dwFlashOfst + (DSPF_PUTBITS_PATTERN_SZ << 2));
        pu4ADSPslt_done = _ADSP_GetDRAMDec1_Addr(ADSPSLT_PAT_5_DONE);
        pu4ADSPslt_rslt = _ADSP_GetDRAMDec1_Addr(ADSPSLT_PAT_5_RSLT);
        uRamCodeType = TYPE_ROM_ICACHE;
        fgLoadCode   = TRUE;
        break;
    case ADSP_TEST_PATTERN6:
        dwOfst  = u4GetFlashData (dwFlashOfst + (DSPF_CORDIC_PATTERN_SA << 2));
        dwSz    = u4GetFlashData (dwFlashOfst + (DSPF_CORDIC_PATTERN_SZ << 2));
        pu4ADSPslt_done = _ADSP_GetDRAMDec1_Addr(ADSPSLT_PAT_6_DONE);
        pu4ADSPslt_rslt = _ADSP_GetDRAMDec1_Addr(ADSPSLT_PAT_6_RSLT);
        uRamCodeType = TYPE_ROM_ICACHE;
        fgLoadCode   = TRUE;
        break;
    default:
        fgLoadCode   = FALSE;
        Printf("\nError: Not an Allowed ADSP SLT Pattern!! \n");
        return;
    }

    // to non-cache: hard coded here
    pu4ADSPslt_done += (0x30000000/4);
    pu4ADSPslt_rslt += (0x30000000/4);

    *pu4ADSPslt_done = 0;
    *pu4ADSPslt_rslt = 0;

     DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_ICACHE, uRamCodeType);

    dwDECICAddr = dwGetDspIcacheAdr (uRamCodeType);

    // Check dwSz > 0 for preventing take empty entry code
    /*lint -e{774}*/
    if (fgLoadCode && (dwSz >= 1))
    {
        // Flush Cache
        //vFlash2DramDma ((dwDECICAddr * 4), u4GetSLTDspImgAddr () + (dwOfst * 4),//build error
        //                (dwSz - 1) * 4);
        // Ram Boot
        vSetDspBootFromRam ();
        // Let DSP go
        vTriggerDSPSLT ();
    }
    else
    {
        Printf("\nError: Loading ADSP SLT Pattern failed!! \n");
    }

}

/******************************************************************************
* Function      : _ADSP_SLT_hdl_init(void)
* Description   : GUI static varibles initialization
* Parameter     : None
* Return        : None
******************************************************************************/
static void _ADSP_SLT_hdl_init(void)
{

    //AUD_DspClkSel((UINT8)3);        // 1/2 APLL: 147MHz
	AUD_DspClkSel((UINT8)5);        // 1/2 APLL: 147MHz
    vDspMemInit((UINT8 *)u4GetDspWorkBuf());
    IniBlkRegs();
    _ADSP_CFG_Reset();

    pu4ADSPslt_done = NULL;
    pu4ADSPslt_rslt = NULL;
    u1ADSPSLTPatID = 0;
    u4RunCnt = 0;
}

/******************************************************************************
* Function      : ADSP_SLT_SetPatID
* Description   : Set ADSP SLT Pattern ID
* Parameter     : None
* Return        : None
******************************************************************************/
void ADSP_SLT_SetPatID(UINT8 u1PID)
{
    u1ADSPSLTPatID = u1PID;
}

/******************************************************************************
* Function      : ADSP_SLT_Init
* Description   : GUI input initialization
* Parameter     : None
* Return        : None
******************************************************************************/
void ADSP_SLT_Init(void)
{
    HANDLE_T hThread1;

    _ADSP_SLT_hdl_init();
    // Create ADSP SLT thread
    if (x_thread_create(    &hThread1, 
                            ADSPSLT_THREAD_NAME, 
                            ADSPSLT_STACK_SIZE, 
                            ADSPSLT_THREAD1_PRIORITY,
                            _ADSP_SLT_hdl, 
                            0, 
                            NULL
                        ) != OSR_OK)
    {
        Printf("\nError: Create ADSP SLT thread fail\n");
        while (1) {}    // Hold the system
    }

}

#endif // end of #ifdef __MODEL_slt__

