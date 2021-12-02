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
 * $Author: kai.wang $
 * $Date: 2012/06/28 $
 * $RCSfile: dsp_rg_ctl.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file dsp_rg_ctl.c
 *  Brief of file dsp_rg_ctl.c.
 *  Details of file dsp_rg_ctl.c (optional).
 */

#define DSP_RGCTL_C
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "dsp_common.h"
#include "dsp_reg.h"
#include "dsp_data.h"
#include "dsp_rg_ctl.h"
#include "dsp_func.h"
#include "dsp_shm.h"
#include "adsp_task.h"
#include "dsp_table.h"

#include "aud_if.h"
#include "aud_debug.h"
#include "drv_adsp.h"
#include "d2rc_shm.h"
#include "aud_hw.h"
#include "drvcust_if.h"

#include "x_os.h"
#include "x_hal_5381.h"
#include "x_ckgen.h"
#ifdef CC_HDMI_ARC_SPDIF_CTRL_89
#include "x_pdwnc.h"
#endif

LINT_EXT_HEADER_END

//Check DRAM Allocation
#include "../../fbm/fbm_pool_config.h"
#if ((DSP_SHARED_INF0_SZ + DSP_CODE_SZ + DSP_DATA_SZ) != FBM_AUD_BASE)
#error "DSP_SHARED_INFO_SZ + DSP_CODE_SZ != FBM_AUD_BASE"
#endif
#if (DSP_AFIFO_SZ != FBM_AUD_AFIFO)
#error "DSP_AFIFO_SZ != FBM_AUD_AFIFO"
#endif
#if (DSP_AFIFO_SZ_DEC2 != FBM_AUD_AFIFO2)
#error "DSP_AFIFO_SZ_DEC2 != FBM_AUD_AFIFO2"
#endif
#if (DSP_AFIFO_SZ_DEC3 != FBM_AUD_AFIFO3)
#error "DSP_AFIFO_SZ_DEC3 != FBM_AUD_AFIFO3"
#endif
#if (DSP_AFIFO_SZ_DEC4 != FBM_AUD_AFIFO4)
#error "DSP_AFIFO_SZ_DEC4 != FBM_AUD_AFIFO4"
#endif
#if (DSP_AFIFO_HDMI_PARSER_SZ != FBM_AUD_HDMI_PARSER)
#error "DSP_AFIFO_HDMI_PARSER_SZ != FBM_AUD_HDMI_PARSER"
#endif
#if (DSP_MIXSNDBUF_ALL_SZ != FBM_AUD_MIXSNDBUF)
#error "DSP_MIXSNDBUF_ALL_SZ != FBM_AUD_MIXSNDBUF"
#endif
//End

UINT8 _uDspState;
BOOL _fgRamBoot;
UINT8 _uDspSIntAddr;
UINT32 _u4DspSIntSD;
UINT32 _u4DspSIntLD;

//-----------------------------------------------------------------------------
// IniBlkRegs
//
/** Initial (1) Start block registers.
 *          (2) Putbits registers.
 *          (3) Compact mode block registers.
 *          (4) Instruction cache registers.
 */
//-----------------------------------------------------------------------------
void IniBlkRegs (void)
{
    UINT32 dwBlkData;

    dwBlkData = (_u4DspDramBuf[0] >> 8) & 0xFFFF;
    dwBlkData = dwBlkData | ((_u4DspDramBuf[1] << 8) & 0xFFFF0000);
    AUD_WRITE32(REG_DSP_DRAM_SBLK01, dwBlkData);

    dwBlkData = (_u4DspDramBuf[2] >> 8) & 0xFFFF;
    dwBlkData = dwBlkData | ((_u4DspDramBuf[3] << 8) & 0xFFFF0000);
    AUD_WRITE32(REG_DSP_DRAM_SBLK23, dwBlkData);

    dwBlkData = (_u4DspDramBuf[4] >> 8) & 0xFFFF;
    dwBlkData = dwBlkData | ((_u4DspDramBuf[5] << 8) & 0xFFFF0000);
    AUD_WRITE32(REG_DSP_DRAM_SBLK45, dwBlkData);

    dwBlkData = (_u4DspDramBuf[6] >> 8) & 0xFFFF;
    dwBlkData = dwBlkData | ((_u4DspDramBuf[7] << 8) & 0xFFFF0000);
    AUD_WRITE32(REG_DSP_DRAM_SBLK67, dwBlkData);

    dwBlkData = (_u4DspCmptBuf[0] >> 8) & 0xFFFF;
    dwBlkData = dwBlkData | ((_u4DspCmptBuf[1] << 8) & 0xFFFF0000);
    AUD_WRITE32(REG_DSP_CMPT_DRAM_SBLK01, dwBlkData);  // Temp hack for cmpt mode

    dwBlkData = (_u4DspCmptBuf[2] >> 8) & 0xFFFF;
    dwBlkData = dwBlkData | ((_u4DspCmptBuf[3] << 8) & 0xFFFF0000);
    AUD_WRITE32(REG_DSP_CMPT_DRAM_SBLK23, dwBlkData);  // Temp hack for cmpt mode

    dwBlkData = (_u4DspCmptBuf[4] >> 8) & 0xFFFF;
    dwBlkData = dwBlkData | ((_u4DspCmptBuf[5] << 8) & 0xFFFF0000);
    AUD_WRITE32(REG_DSP_CMPT_DRAM_SBLK45, dwBlkData);  // Temp hack for cmpt mode

    dwBlkData = (_u4DspCmptBuf[6] >> 8) & 0xFFFF;
    dwBlkData = dwBlkData | ((_u4DspCmptBuf[7] << 8) & 0xFFFF0000);
    AUD_WRITE32(REG_DSP_CMPT_DRAM_SBLK67, dwBlkData);  // Temp hack for cmpt mode

    dwBlkData = _u4DspICacheAddr[0] & 0xFFFF;
    dwBlkData = dwBlkData | (_u4DspICacheAddr[1] << 16);
    AUD_WRITE32(REG_DSP_IC_DRAM_SBLK01, dwBlkData);

    dwBlkData = ((UINT32) (_u4DspICacheAddr[2])) & 0xFFFF;
    dwBlkData = dwBlkData | (_u4DspICacheAddr[3] << 16);
    AUD_WRITE32(REG_DSP_IC_DRAM_SBLK23, dwBlkData);

    dwBlkData = ((UINT32) (_u4DspICacheAddr[4])) & 0xFFFF;
    dwBlkData = dwBlkData | (_u4DspICacheAddr[5] << 16);
    AUD_WRITE32(REG_DSP_IC_DRAM_SBLK45, dwBlkData);

    dwBlkData = ((UINT32) (_u4DspICacheAddr[6])) & 0xFFFF;
    dwBlkData = dwBlkData | (_u4DspICacheAddr[7] << 16);
    AUD_WRITE32(REG_DSP_IC_DRAM_SBLK67, dwBlkData);

#if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
	AUD_WRITE32(REG_DSP_IPGSET0,
				(((DSP_POS_PG_OFST>>10)&0xff)<<0)|
				(((DSP_CTR_PG_OFST>>10)&0xff)<<8)|
				(((DSP_CMM_PG_OFST>>10)&0xff)<<16) |
				(((DSP_DEC4_PG_OFST>>10)&0xff)<<24));

	AUD_WRITE32(REG_DSP_IPGSET1,
				(((DSP_DEC1_PG_OFST>>10)&0xff)<<0)|
				(((DSP_DEC3_PG_OFST>>10)&0xff)<<8)|
				(((DSP_DEC2_PG_OFST>>10)&0xff)<<16));
#else
    AUD_WRITE32(REG_DSP_IPGSET0,
                (((DSP_POS_PG_OFST>>8)&0xff)<<0)|
                (((DSP_CTR_PG_OFST>>8)&0xff)<<8)|
                (((DSP_CMM_PG_OFST>>8)&0xff)<<16) |
                (((DSP_DEC4_PG_OFST>>8)&0xff)<<24));

    AUD_WRITE32(REG_DSP_IPGSET1,
                (((DSP_DEC1_PG_OFST>>8)&0xff)<<0)|
                (((DSP_DEC3_PG_OFST>>8)&0xff)<<8)|
                (((DSP_DEC2_PG_OFST>>8)&0xff)<<16));
#endif	

#if 1 //def DSP_TEST_PUTBITS                         // Putbits test
    {
    extern UINT32 _u4DspMemBufAddr;
    UINT32 _u4DspPutbBufAddr = _u4DspMemBufAddr;
    UINT32 _u4DspPutbBuf0=0, _u4DspPutbBuf1=0;
	
    dwBlkData= ((_u4DspPutbBufAddr + DSP_PUTBITS_SA) >> 8) & 0x0ffff;
    _u4DspPutbBuf0 |= (dwBlkData << 16);
    dwBlkData += 4;
    _u4DspPutbBuf0 |= dwBlkData;
    AUD_WRITE32(REG_PUTBS_BLK0_RISC, _u4DspPutbBuf0);
    _u4DspPutbBuf1 |= (dwBlkData << 16);
    dwBlkData += 4;
    _u4DspPutbBuf1 |= dwBlkData;
    AUD_WRITE32(REG_PUTBS_BLK1_RISC, _u4DspPutbBuf1);

    #if 0
    {
    UINT32 _u4BlkData;    
    _u4BlkData = (_u4AFIFO[AUD_DEC_MAIN][0] >> 2) & 0x00FFFFFF;
    AUD_WRITE32(REG_DMX_STR, _u4BlkData);
    AUD_WRITE32(REG_DMX_WRITE_PNT, _u4BlkData);
    _u4BlkData = (_u4AFIFO[AUD_DEC_MAIN][1] >> 2) & 0x00FFFFFF;
    AUD_WRITE32(REG_DMX_END, _u4BlkData);

    _u4BlkData = AUD_READ32(REG_BS_CFG);
    _u4BlkData &= ~(0x03);
    _u4BlkData |= 2;
    AUD_WRITE32(REG_BS_CFG, _u4BlkData);
    }
    #endif
    }
#endif
}

//-----------------------------------------------------------------------------
// fgDspBusy
//
/** Check DSP Status.
 *
 *  @retval     TRUE (DSP busy) / FALSE (DSP not busy).
 */
//-----------------------------------------------------------------------------
BOOL fgDspBusy (void)
{
    return (AUD_READ32(REG_R2D_STA) & DSP_B);
}

//-----------------------------------------------------------------------------
// vDSPSendInt
//
/** Send Interrupt to DSP.
 */
//-----------------------------------------------------------------------------
void vDspSendInt (void)
{
    static INT32 i4IntCnt = 0;
    
    LOG(9, "[DSP_TASK] vDspSendInt (%d)\n",i4IntCnt);
    i4IntCnt ++;
    AUD_WRITE32(REG_LD, _u4DspSIntLD); // long data
    SendDSPInt(_uDspSIntAddr, _u4DspSIntSD);   // short data & interrupt
    _rDspStatus[AUD_DEC_MAIN].fgDspSInt = FALSE;
}

//-----------------------------------------------------------------------------
// dwReadDspLongData
//
/** Read Long data from DSP.
 */
//-----------------------------------------------------------------------------
UINT32 dwReadDspLongData (void)
{
    return (AUD_READ32(REG_D2R_RD));
}

//-----------------------------------------------------------------------------
// vSendDspLongInt
//
/** Send Interrupt to DSP.
 */
//-----------------------------------------------------------------------------
void vSendDspLongInt (UINT32 u4Addr, UINT32 u4ShortD, UINT32 u4LongD)
{
    CRIT_STATE_T rCritState;                
    static INT32 i4IntCnt = 0;
    
    rCritState = x_crit_start();
    if (_rDspStatus[AUD_DEC_MAIN].fgDspSInt)
    {
        LOG(10, "[DSP_TASK] vSendDspLongInt (%d)\n",i4IntCnt);
        i4IntCnt ++;
    }
    AUD_WRITE32(REG_LD, u4LongD);  // long data
    SendDSPInt(u4Addr, u4ShortD);  // short data & interrupt
    x_crit_end(rCritState);
}

#ifdef SPDIF_MUTE_BY_PINMUX
extern UINT8 bSpdifFunc; // 1: SPDIF 2: HDMI_BYPASS
#endif

//-----------------------------------------------------------------------------
// vCloseIec
//
/** Close IEC.
 */
//-----------------------------------------------------------------------------
void vCloseIec (void)
{
#ifdef SPDIF_MUTE_BY_PINMUX
    #if defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)
#ifdef CC_HDMI_ARC_SPDIF_CTRL_89
		vIO32WriteFldAlign(PDWNC_PINMUX0, 0, FLD_PAD1_ASPDIF0);
#else
		vIO32WriteFldAlign(CKGEN_PMUX0, 0, FLD_PAD_ASPDIF0);
#endif
    #endif
#else
    UINT32 u4IecConfig;

    u4IecConfig = AUD_READ32(REG_SPOCFG0);
    AUD_WRITE32(REG_SPOCFG0, (u4IecConfig | SPOUT_MUTE));   // Bit[5]
#endif    
}

//-----------------------------------------------------------------------------
// vOpenIec
//
/** Open IEC.
 */
//-----------------------------------------------------------------------------
void vOpenIec (void)
{
#ifdef SPDIF_MUTE_BY_PINMUX
    #if defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)
#ifdef CC_HDMI_ARC_SPDIF_CTRL_89
		vIO32WriteFldAlign(PDWNC_PINMUX0, 2, FLD_PAD1_ASPDIF0);
#else
		vIO32WriteFldAlign(CKGEN_PMUX0, bSpdifFunc, FLD_PAD_ASPDIF0);
#endif
    #endif
#else
    UINT32 u4IecConfig;

    u4IecConfig = AUD_READ32(REG_SPOCFG0);
    AUD_WRITE32(REG_SPOCFG0, (u4IecConfig & (~SPOUT_MUTE)));    // Bit[5]
#endif    
}

//-----------------------------------------------------------------------------
// vSetIecDownSample
//
/** Set IEC down sample.
 *
 *  @param  u4DownSample        Down sample value.
 */
//-----------------------------------------------------------------------------
void vSetIecDownSample (UINT32 u4DownSample)
{
    UINT32 u4IecConfig;

    u4IecConfig = AUD_READ32(REG_SPOCFG0);
    u4IecConfig &= (~SPOUT_DOWNSAMPLE_MASK);
    u4IecConfig |= (u4DownSample << SPOUT_DOWNSAMPLE_POS);
    AUD_WRITE32(REG_SPOCFG0, u4IecConfig);
}

//-----------------------------------------------------------------------------
// vSetIecConfigReg
//
/** Set audio configuration (only for first decoder).
 *
 *  @param  u1DecId      Decoder ID. 
 *  @param  prAudCfg    Audio config structure.
 */
//-----------------------------------------------------------------------------
/*lint -e{818}*/
void vSetIecConfigReg(UINT8 u1DecId, AUDIO_CFG_T * prAudCfg)
{
    UINT32 u4IecConfigOrg, u4IecConfig;

    if (prAudCfg == NULL)
    {
        return;
    }

    u4IecConfigOrg = AUD_READ32(REG_SPOCFG0);
    u4IecConfig = u4IecConfigOrg & (~SPOUT_BASIC_MASK);
    u4IecConfig |= (((UINT32)prAudCfg->u1IecOutBitNum) << SPOUT_BIT_NUM_POS) |
        (((UINT32)prAudCfg->u1IecChSel) << SPOUT_CHSEL_POS) |
        #ifndef SPDIF_MUTE_BY_PINMUX
        (((UINT32)prAudCfg->fgIecMute) << SPOUT_MUTE_POS) |
        #endif
        (((UINT32)prAudCfg->u1IecDownSamp) << SPOUT_DOWNSAMPLE_POS) |
        (((UINT32)prAudCfg->u1IecDecSel) << SPOUT_SPDF_SEL_POS);

#ifndef SPDIF_MUTE_BY_PINMUX
    if (u4IecConfig == u4IecConfigOrg)
    {
        return;
    }

    // 20040316 sammy modified, if we change downsample, the amplifier will unlock
    if ((((u4IecConfigOrg & SPOUT_DOWNSAMPLE_MASK) >> SPOUT_DOWNSAMPLE_POS) !=
        (UINT32)prAudCfg->u1IecDownSamp) &&
        ((u4IecConfigOrg & SPOUT_MUTE) != 0))
    {
        vCloseIec ();
    }
#else
    #if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
    if (prAudCfg->fgIecMute)
    {
#ifdef CC_HDMI_ARC_SPDIF_CTRL_89
		vIO32WriteFldAlign(PDWNC_PINMUX0, 0, FLD_PAD1_ASPDIF0);
#else
		vIO32WriteFldAlign(CKGEN_PMUX0, 0, FLD_PAD_ASPDIF0);
#endif
      
    }
    else
    {

#ifdef CC_HDMI_ARC_SPDIF_CTRL_89
		vIO32WriteFldAlign(PDWNC_PINMUX0, 2, FLD_PAD1_ASPDIF0);
#else
		vIO32WriteFldAlign(CKGEN_PMUX0, bSpdifFunc, FLD_PAD_ASPDIF0);
#endif
   
    }
    #endif
#endif 
    AUD_WRITE32(REG_SPOCFG0, u4IecConfig);
}

void vSetIecChannel(AUD_IEC_CH_T eIecChannel)
{
    UINT32 u4IecConfigOrg, u4IecConfig;

    u4IecConfigOrg = AUD_READ32(REG_SPOCFG0);
    u4IecConfig = (u4IecConfigOrg & ~(SPOUT_CHSEL_MASK | SPOUT_SPDF_SEL_MASK));
    if (eIecChannel == AUD_IEC_AUX)
    {
        u4IecConfig |= (SPOUT_SPDF_SEL_AUX<<SPOUT_SPDF_SEL_POS);
    }
    else
    {
        u4IecConfig |= ((SPOUT_SPDF_SEL_MAIN<<SPOUT_SPDF_SEL_POS) | (eIecChannel<<SPOUT_CHSEL_POS));
    }

    AUD_WRITE32(REG_SPOCFG0, u4IecConfig);
}

static CHAR* _aszIecCh[] = {"LR","SLR","CLFE","CH78","LINEIN", "CH910"};

void vIecQuery(void)
{
    UINT8 u1Index;
    UINT32 u4IecConfig = AUD_READ32(REG_SPOCFG0);

    Printf("[IEC Setting]\n");

#ifdef SPDIF_MUTE_BY_PINMUX
    #if defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)
#ifdef CC_HDMI_ARC_SPDIF_CTRL_89
    if (IO32ReadFldAlign(PDWNC_PINMUX0, FLD_PAD_ASPDIF0)==0)
#else
	if (IO32ReadFldAlign(CKGEN_PMUX0, FLD_PAD_ASPDIF0)==0)
#endif
    {
        Printf("IEC Mute (SPDIF0 pinmux=GPIO)\n");
    }
    else
    {
        Printf("IEC UnMute (SPDIF0 pinmux=SPDIF)\n");
    }
    #endif
#else
    if (u4IecConfig & SPOUT_MUTE)
    {
        Printf("IEC Mute\n");
    }
    else
    {
        Printf("IEC UnMute\n");
    }
#endif    

    if (u4IecConfig & (SPOUT_SPDF_SEL_AUX<<SPOUT_SPDF_SEL_POS))
    {
        Printf("IEC Ch: AUX\n");
    }
    else
    {
        u1Index = ((u4IecConfig&SPOUT_CHSEL_MASK)>>SPOUT_CHSEL_POS);
        if (u1Index < 6)
            Printf("IEC Ch: %s\n", _aszIecCh[u1Index]);
    }
}

//-----------------------------------------------------------------------------
// vFlushDspICache
//
/** Directly flush the instruction cache. If we need to take care the timing issue,
 *  we run it in the flush state machine. See DspState.c.
 *
 *  @param  fgWaitDsp   No use.
 */
//-----------------------------------------------------------------------------
void vFlushDspICache (BOOL fgWaitDsp)
{
    UINT32 dwTmp;
    UINT32 dwDramStatus;
    UINT32 dwCounter = 0;

    dwTmp = AUD_READ32(REG_ADSP_CFG);
    AUD_WRITE32(REG_ADSP_CFG, (dwTmp & (~ADSP_CFG_ICACHE_FLUSH)));
    AUD_WRITE32(REG_ADSP_CFG, (dwTmp | ADSP_CFG_ICACHE_FLUSH));
    do
    {
        dwDramStatus = AUD_READ32(REG_ADSP_CFG) & ADSP_CFG_ICACHE_FLUSH;
        dwCounter++;
    }
    while ( (dwDramStatus != 0) && (dwCounter < 5) );
    
    UNUSED(fgWaitDsp);
}

//-----------------------------------------------------------------------------
// vResetAndHoldDsp
//
/** Reset and hold DSP.
 */
//-----------------------------------------------------------------------------
void vResetAndHoldDSP (void)
{
    UINT32 dwTmp;

    dwTmp = AUD_READ32(REG_ADSP_CFG);
    AUD_WRITE32(REG_ADSP_CFG, (dwTmp & (~ADSP_CFG_RST_B)));  // reset
}
    
//-----------------------------------------------------------------------------
// HdStpDsp
//
/** Harrware stop DSP.
 */
//-----------------------------------------------------------------------------
void HdStpDsp (void)
{
    vResetAndHoldDSP ();
}

//-----------------------------------------------------------------------------
// vSetCacheSize
//
/** Set cache size.
 *
 *  @param  u1Size      Cache size value.
 */
//-----------------------------------------------------------------------------
void vSetCacheSize (UINT8 bSize)
{
    UINT32 dwTmp;

    dwTmp = AUD_READ32(REG_ADSP_CFG);
    dwTmp &= (~ADSP_CACHE_SIZE_MASK);
    dwTmp |= ((bSize << ADSP_CACHE_SIZE_POS) & ADSP_CACHE_SIZE_MASK);
    AUD_WRITE32(REG_ADSP_CFG, dwTmp);
}

//-----------------------------------------------------------------------------
// vSetCacheInstLen
//
/** Set cache buffer size.
 *
 *  @param  u1Size      Cache buffer size value.
 */
//-----------------------------------------------------------------------------
void vSetCacheInstLen (UINT8 bSize)
{
    UINT32 dwTmp;

    dwTmp = AUD_READ32(REG_ADSP_CFG);
    dwTmp &= (~ADSP_CACHE_INST_SIZE_MASK);
    dwTmp |= ((bSize << ADSP_CACHE_INST_SIZE_POS) & ADSP_CACHE_INST_SIZE_MASK);
    AUD_WRITE32(REG_ADSP_CFG, dwTmp);
}

//-----------------------------------------------------------------------------
// vSetCmptRDThreshold
//
/** Set DSP compact read pre-fetch threshold.
 *
 *  @param  u1Threshold     Compact read threshold value.
 */
//-----------------------------------------------------------------------------
void vSetCmptRDThreshold(UINT8 bThreshold)
{
    UINT32 dwTmp;

    dwTmp = AUD_READ32(REG_ADSP_CFG);
    dwTmp &= (~ADSP_CMPTRD_THRES_MASK);
    dwTmp |= ((bThreshold << ADSP_CMPTRD_THRES_POS) & ADSP_CMPTRD_THRES_MASK);
    AUD_WRITE32(REG_ADSP_CFG, dwTmp);
}

void DownloadRomCode(void)
{
    if (BSP_IsFPGA())
    {
        //Rice: load ROM code into FPGA SRAM
        UINT32 dwOfst;
        UINT32 dwSz;
        UINT32 dwFlashOfst;
        //UINT32 u4Datatmp[1024];
        
#if 1 //PROM
        dwFlashOfst = u4GetDspImgAddr ();   // set to dsp image addr    
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_PROM_SA << 2));
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_PROM_SZ << 2));

        if (dwSz >= 1)
        {
            UINT32 i;
            UINT32* puSrc= ((UINT32*) (u4GetDspImgAddr () + ((dwOfst)*4)));

            // Src: u4GetDspImgAddr () + (dwOfst * 4)
            // Size: (dwSz - 1) * 4 bytes
            WriteREG (RW_ADSPCR, (DR_RD_EN|PMEM_AUTO_INC|PROM_WR |HOT_DNLD));  // disable dsp
            WriteREG (RW_DNLD_CNTR, 0); // reset download counter to 0

            for (i=0; i<(dwSz-1); i++)
            {
                WriteREG (RW_PROM_DNLD, puSrc[i]);
            }

            // Verify
            WriteREG (RW_DNLD_CNTR, 0); // reset download counter to 0
            for (i= 0; i<(dwSz-1); i++)
            {
                //u4Datatmp[i] = ReadREG (RW_PROM_DNLD);
                UINT32 u4Data = ReadREG (RW_PROM_DNLD);
                if (u4Data != puSrc[i])
                {
                    break;
                }
            }

            WriteREG (RW_ADSPCR, 0);    // clear datarom write
#if 0 //For PROM test pattern
            //PROM Size-1
            if(dwSz>257)    dwSz=257;
            WriteDspDecCacheDram(0x1, (dwSz-1)-1);
            WriteDspDecCacheDram(0x3, 0x800500);

            // write to share memory
            for (i= 0; i<(dwSz-1); i++)
            {
                WriteDspDecCacheDram (0x5+i, puSrc[i]);
            }
#endif
        }       
#endif
#if 0 //DROM
        dwFlashOfst = u4GetDspImgAddr ();   // set to dsp image addr
        //dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_PL2_POST_SA << 2));
        //dwSz = u4GetFlashData (dwFlashOfst + (DSPF_PL2_POST_SZ << 2));
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_DROM_SA << 2));
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_DROM_SZ << 2));
        
        if (dwSz >= 1)
        {
            UINT32 i;
            UINT32* puSrc= ((UINT32*) (u4GetDspImgAddr () + ((dwOfst)*4)));

            // Src: u4GetDspImgAddr () + (dwOfst * 4)
            // Size: (dwSz - 1) * 4 bytes
            WriteREG (RW_ADSPCR, DROM_WR);  // disable dsp
            WriteREG (RW_DNLD_CNTR, 0); // reset download counter to 0

            for (i=0; i<(dwSz-1); i++)
            {
                WriteREG (RW_DROM_DNLD, puSrc[i]);
            }

            // Verify
            WriteREG (RW_DNLD_CNTR, 0); // reset download counter to 0
            for (i= 0; i<(dwSz-1); i++)
            {
                UINT32 u4Data = ReadREG (RW_DROM_DNLD);
                //u4Datatmp[i] = ReadREG (RW_DROM_DNLD);
                if (u4Data != puSrc[i])
                {
                    break;
                }
            }

            WriteREG (RW_ADSPCR, 0);    // clear datarom write
#if 0 //For DROM test pattern
            //DROM Size-1
            if(dwSz>257)    dwSz=257;
            WriteDspDecCacheDram(0x2, (dwSz-1)-1);
            WriteDspDecCacheDram(0x4, 0x800500);
            
            // write to share memory
            for (i= 0; i<(dwSz-1)*4; i+=3)
            {
                tmpW = (puSrc[i]<<16)|(puSrc[i+1]<<8)|(puSrc[i+2]);
                WriteDspDecCacheDram (0x105+(i/3), tmpW);    // clear datarom write
            }
#endif
        }
#endif
    }
}

//-----------------------------------------------------------------------------
// vTriggerDsp
//
/** Let DSP go.
 */
//-----------------------------------------------------------------------------
void vTriggerDSP(void)
{
  UINT32 dwTmp;
  UINT32 dwDramStatus;
  UINT32 dwRetryCnt=0;
  
  dwTmp = AUD_READ32(REG_ADSP_CFG);
  if ((dwTmp & ADSP_CFG_RST_B) == ADSP_CFG_RST_B)
  {
      x_thread_delay(300); 
  }
  while(1)
  {
    #if 1
    AUD_WRITE32(REG_ADSP_CFG, ((dwTmp & (~ADSP_CFG_ICACHE_FLUSH)) & (~ADSP_CFG_RST_B)));
    AUD_WRITE32(REG_ADSP_CFG, (dwTmp | ADSP_CFG_ICACHE_FLUSH));

    // Light: Remove delay
    //x_thread_delay(10);                                      // delay 10 ms
    AUD_WRITE32(REG_ADSP_CFG, (dwTmp | ADSP_CFG_RST_B));
    #else
    AUD_WRITE32(REG_ADSP_CFG, dwTmp & (~ADSP_CFG_ICACHE_FLUSH) & (~ADSP_CFG_RST_B));
    AUD_WRITE32(REG_ADSP_CFG, dwTmp | ADSP_CFG_ICACHE_FLUSH | ADSP_CFG_RST_B);
    AUD_WRITE32(REG_ADSP_CFG, dwTmp | ADSP_CFG_ICACHE_FLUSH | ADSP_CFG_RST_B);    
    #endif
    
    // wait cycles to flush
    x_thread_delay(20); // Light: delay 20 ms
    // check if flush taken
    dwDramStatus = AUD_READ32(REG_ADSP_CFG) & ADSP_CFG_ICACHE_FLUSH ;
    if( (dwDramStatus == 0) || (dwRetryCnt > 20) )
    {
      if(dwDramStatus != 0)
      {
      	//_dwDspFlushFailCnt ++;
      	LOG(1, "DSP flush cache fail\n");
      }
      break;
    }
    dwRetryCnt ++;
  }

  // Light add LOG of Icache flush retry count
  if(dwRetryCnt)
  {
      LOG(9,"ADSP ICache flush retry count = %d\n",dwRetryCnt);
  }
#if 0
  dwTmp = AUD_READ32(REG_ADSP_CFG);
  AUD_WRITE32(REG_ADSP_CFG, (dwTmp & (~ADSP_CFG_RST_B)));  // Reset again
  dwTmp |= ADSP_CFG_RST_B;  // enable dsp  
  AUD_WRITE32(REG_ADSP_CFG, dwTmp);  // enable dsp
#endif
}

//-----------------------------------------------------------------------------
// vSetDspBootFromRam
//
/** Set boot from RAM flag. It is needed to be set when DSP is in reset and hold status.
 */
//-----------------------------------------------------------------------------
void vSetDspBootFromRam (void)
{
    UINT32 u4Data;

    u4Data = AUD_READ32(REG_ADSP_CFG);
    AUD_WRITE32(REG_ADSP_CFG, (u4Data | ADSP_BOOT_FROM_RAM));
}

//-----------------------------------------------------------------------------
// vSetDspBootFromRom
//
/** Set boot from ROM flag. It is needed to be set when DSP is in reset and hold status.
 */
//-----------------------------------------------------------------------------
void vSetDspBootFromRom (void)
{
    UINT32 u4Data;

    u4Data = AUD_READ32(REG_ADSP_CFG);
    AUD_WRITE32(REG_ADSP_CFG, (u4Data & (~ADSP_BOOT_FROM_RAM)));
}

//-----------------------------------------------------------------------------
// HdRstDsp
//
/** Reset DSP hardware. Set cache size, flush cache, load rom code if needed.
 */
//-----------------------------------------------------------------------------
void HdRstDsp (void)
{
    // Disable DRAM timeout reset function
    AUD_WRITE32(REG_ADSP_CFG, ((AUD_READ32(REG_ADSP_CFG)) & (~(ADSP_DRAM_TIMEOUT_RESET | ADSP_DRAM_TIMEOUT_FLUSH)))); 

    // Enable DSP clock
    AUD_WRITE32(REG_ADSP_CFG, ((AUD_READ32(REG_ADSP_CFG)) | ADSP_CFG_CLK_ENA)); 

    //Reset Dsp
    // First reset and hold dsp
    vResetAndHoldDSP ();
    // Set cache size
    //vSetCacheSize (CACHE_256);
    vSetCacheSize (CACHE_SIZE_2K);
    // set instruction buffer size
    vSetCacheInstLen (CBUF_SIZE_16X128);
    // set compact read threshold
    vSetCmptRDThreshold(THRES_UNFULL);

    // Download Rom Code for FPGA
    DownloadRomCode();

#if defined(CC_MT5396)
    if (BSP_GetIcVersion() == IC_VER_5396_AA)
    {
        // Process for Version A
        _fgRamBoot = TRUE;
    }
    else
    {
        // Process for Version B
        _fgRamBoot = FALSE;
    }
#elif defined(CC_MT5368)
    _fgRamBoot = TRUE;
#elif defined(CC_MT5389)
    _fgRamBoot = FALSE;
#elif defined (CC_MT5365)
    if (BSP_GetIcVersion() == IC_VER_5365_AA)
    {
        _fgRamBoot = TRUE;
    }
    else
    {
        _fgRamBoot = FALSE;
    }
    #ifdef MIXSOUND_MIPS_MEASURE
    _fgRamBoot = TRUE;
    #endif
#elif defined (CC_MT5395)
    if (BSP_GetIcVersion() == IC_VER_5395_AA)
    {
        _fgRamBoot = TRUE;
    }
    else
    {
        _fgRamBoot = FALSE;
    }
    #ifdef MIXSOUND_MIPS_MEASURE
    _fgRamBoot = TRUE;
    #endif    
#else
    _fgRamBoot = TRUE;
#endif

#ifdef DSP_TEST_CODE_FETCH
    vDspLoadRamCode (AUD_ROMCOD);
    vDspLoadRamCode (AUD_MPEG_DEC3);
    vDspLoadRamCode (AUD_MPEG_DEC1);
    vDspLoadRamCode (AUD_MPEG_DEC2);
    vDspLoadRamCode (AUD_SBCENC_DEC4);
    vDspLoadRamCode (AUD_POSTRAM);
#endif    

    // Rice: changed and modified _fgRamBoot to FALSE @20060731
    /*lint -e{506}*/
    /*lint -e{774}*/

    #ifdef DSP_SUPPORT_17BIT
    // Load Rom Code
    vDspLoadRamCode (AUD_ROMCOD);
    #endif

    if (_fgRamBoot)
    {
        #ifndef DSP_SUPPORT_17BIT
        // Load Rom Code
        vDspLoadRamCode (AUD_ROMCOD);
        #endif
        
        // Enable Ram Boot
        vSetDspBootFromRam ();
    }
	
    vDspLoadRamCode (AUD_AININTRAM);

#ifdef DSP_TEST_CORDIC
    {
        UINT32 i;
        UINT32 u4Size;
        UINT32 *Dst;
        
        u4Size = sizeof(u4CordicTest)/sizeof(UINT32);
        Dst = _u4DspDramBuf[TYPE_DEC1_DRAM_IDX] + (0x400*4);
        for (i=0; i<u4Size; i++)
        {
            *Dst++ = u4CordicTest[i];
        }
        DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_DEC1_DRAM_IDX);
    }
#endif
    // Let dsp go
    vTriggerDSP ();
    
	// ramboot
    // Flush cache
    //vFlushDspICache (FALSE);

}

void vResetDSP (void)
{
    UINT32 dwTmp;

    dwTmp = AUD_READ32(REG_ADSP_CFG);

    AUD_WRITE32(REG_ADSP_CFG, ((dwTmp & (~ADSP_CFG_ICACHE_FLUSH)) & (~ADSP_CFG_RST_B)));
    AUD_WRITE32(REG_ADSP_CFG, (dwTmp | ADSP_CFG_ICACHE_FLUSH));
    // Light: Remove delay
    //x_thread_delay(10);                                      // delay 10 ms
    AUD_WRITE32(REG_ADSP_CFG, (dwTmp | ADSP_CFG_RST_B));
}

//-----------------------------------------------------------------------------
// dwGetDSPNSADR
//
/** Get DSP normal DRAM start block address.
 *
 *  @param  u4Type      Type. 
 *
 *  @retval  Normal DRAM buffer block address.
 */
//-----------------------------------------------------------------------------
UINT32 dwGetDSPNSADR (UINT32 u4Type)
{
    UINT32 u4Addr;

    u4Addr = (_u4DspDramBuf[TYPE_DEC1_DRAM_IDX]);
    if (u4Type < TYPE_AUD_DEC2)
    {
        u4Addr = (_u4DspDramBuf[TYPE_DEC1_DRAM_IDX]);
    }
    else if (( (u4Type >= TYPE_AUD_DEC2) && (u4Type < TYPE_AUD_MIX) ))
    {
        u4Addr = _u4DspDramBuf[TYPE_DEC2_DRAM_IDX];
    }
    else if (( (u4Type >= TYPE_AUD_DEC3) && (u4Type < TYPE_AUD_DEC4) ))
    {
        u4Addr = _u4DspDramBuf[TYPE_DEC3_DRAM_IDX];
    }
    else if (( (u4Type >= TYPE_AUD_DEC4) && (u4Type < TYPE_AUD_CMM) ))
    {
        u4Addr = _u4DspDramBuf[TYPE_DEC4_DRAM_IDX];
    }
    else
    {
        u4Addr = _u4DspDramBuf[TYPE_COMM_DRAM_IDX];
    }
    return u4Addr;
}

//-----------------------------------------------------------------------------
// dwGetDSPCSADR
//
/** Get DSP compact DRAM start block address.
 *
 *  @param  u4Type      Type. 
 *
 *  @retval  Compact DRAM buffer block address.
 */
//-----------------------------------------------------------------------------
UINT32 dwGetDSPCSADR (UINT32 u4Type)
{
    UINT32 u4Addr;

    u4Addr = _u4DspCmptBuf[TYPE_DEC1_DRAM_IDX];
    if (u4Type < TYPE_AUD_DEC2)
    {
        u4Addr = _u4DspCmptBuf[TYPE_DEC1_DRAM_IDX];
    }
    else if (( (u4Type >= TYPE_AUD_DEC2) && (u4Type < TYPE_AUD_MIX) ))
    {
        u4Addr = _u4DspCmptBuf[TYPE_DEC2_DRAM_IDX];
    }
    else if (( (u4Type >= TYPE_AUD_MIX) && (u4Type < TYPE_AUD_DEC4) ))
    {
        u4Addr = _u4DspCmptBuf[TYPE_DEC3_DRAM_IDX];
    }
    else if (( (u4Type >= TYPE_AUD_DEC4) && (u4Type < TYPE_AUD_CMM) ))
    {
        u4Addr = _u4DspCmptBuf[TYPE_DEC4_DRAM_IDX];
    }
    else
    {
        u4Addr = _u4DspCmptBuf[TYPE_COMM_DRAM_IDX];
    }
    return u4Addr;
}

//-----------------------------------------------------------------------------
// dwGetDspIcacheAdr
//
/** Get DSP instruction cache address.
 *
 *  @param  uRamCodeType        RAM code type. 
 *
 *  @retval  DSP instruction cache address.
 */
//-----------------------------------------------------------------------------
UINT32 dwGetDspIcacheAdr (UINT16 u2RamCodeType)
{
    return (((UINT32) _u4DspICacheAddr[u2RamCodeType]) << 6);
}

//-----------------------------------------------------------------------------
// vSetGetBitsNotHold
//
/** Set get bits hold or not hold when bitstream buffer is empty.
 *  It is depend on decoder setting, for ex. vorbis is set to not hold, default is hold.
 *  It is configured in LoadDramTable.
 *
 *  @param  fgGetBitsNotHold    TRUE(not hold) / FALSE(hold).
 */
//-----------------------------------------------------------------------------
void vSetGetBitsNotHold (BOOL fgGetBitsNotHold)
{
    UINT32 u4Data;

    u4Data = AUD_READ32(REG_BS_CFG);
    u4Data = u4Data & (~NO_HOLD_GBS);
    if (fgGetBitsNotHold)
    {
        u4Data |= NO_HOLD_GBS;
    }
    AUD_WRITE32(REG_BS_CFG, u4Data);    
}

//-----------------------------------------------------------------------------
// vSetSPDIFInFIFO
//
/** Set SPDIF/Line-in FIFO.
 *
 *  @param  u4StartAdr    FIFO start address.
 *  @param  u4EndAdr      FIFO end address. 
 */
//-----------------------------------------------------------------------------
void vSetSPDIFInFIFO(UINT8 u1DecId, UINT32 u4StartAdr,UINT32 u4EndAdr)
{
    UINT32 u4SpLinBlk=0;
    
    u4SpLinBlk = (u4StartAdr << 8) | ((u4EndAdr >> 8) & 0xFFFF);
    if (u1DecId == AUD_DEC_AUX)
    {
        AUD_WRITE32(REG_DEC2_LINBLK, u4SpLinBlk);
    }
    else
    {
        AUD_WRITE32(REG_LINBLK, u4SpLinBlk);
    }
}

//-----------------------------------------------------------------------------
// u4DspRegR_D2rSda
//
/** Read DSP REG - D2R_SDA.
 *
 *  @retval     D2R_SDA data.
 */
//-----------------------------------------------------------------------------
UINT32 u4DspRegR_D2rSda(void)
{
    return (AUD_READ32(REG_D2R_SDA));
}

//-----------------------------------------------------------------------------
// vADCInit
//
/** Initialize TV audio ADC.
 */
//-----------------------------------------------------------------------------
void vADCInit(void)
{
    UINT32 u4Data;
    UINT32 u4AudioTunerATD = 0;
    
    // Set AUDIN_CFG
    u4Data = (AUD_READ32(REG_AUDIN_CFG) & (~REG_AUDIN_CFG_MASK));
    u4Data = u4Data | APGA_GAIN_0DB | APGA_OFFSET_0V | APGA_GAIN_BY_DSP | ADCK_INV_RISING;    // | ADC_SEL_DEMOD;  default set sif ADC 
    if (DRVCUST_OptQuery(eAudioTunerATD,&u4AudioTunerATD) == 0)
    {   
        if ((BOOL)u4AudioTunerATD == TRUE)
        {
            u4Data = u4Data | ADC_SEL_DEMOD;
        }
    }
    AUD_WRITE32(REG_AUDIN_CFG, u4Data);

#if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
    #ifdef CC_AUD_SUPPORT_SIF_ADC
    ANA_WRITE32(REG_CVBS_CFG4, (AUD_READ32(REG_CVBS_CFG4)&0x00ffff7f) | 0x40000080);
    //[31:24] RG_AISEL_2    => 8'b01000000
    //[7] RG_C_SELA_2       => 1
    ANA_SET_BIT(REG_CVBS_CFG6, 0x20000000);
    //[29] RG_SIFA_EN_2     => 1    
    ANA_CLR_BIT(REG_CVBS_CFG7, 0x00a0b880);
    //[23] RG_CVBSADC_SEL_CKPLL_2 => 0
    //[21] RG_CVBSADC_PWD_2 => 0
    //[15] RG_CVBS_PWD_2 => 0
    //[13] RG_INMUX_PWD_2 => 0
    //[12] RG_PROT_PWD_2 => 0
    //[11] RG_PGABUFNA_PWD_2 => 0
    //[9] RG_SHIFTA_PWD_2 => 1
    //[7] RG_OFFCURA_PWD_2 => 0
    
    u4Data = (AUD_READ32(REG_AUDIN_CFG) & (~ADC_SEL_DEMOD));
    AUD_WRITE32(REG_AUDIN_CFG, u4Data);
    
    #endif
#else
    // Set ATVCFG0
    u4Data = (AUD_READ32(REG_ATVCFG0) & (~RG_PGA_SIF_PWD_MASK) & (~RG_SIF_SEL_MASK));
    u4Data = u4Data | RG_PGA_SIF_PWD_ON | SIF_SEL_SIF;
    AUD_WRITE32(REG_ATVCFG0, u4Data);
    
    // Set ATVCFG1
    u4Data = (AUD_READ32(REG_ATVCFG1) & (~RG_SIF_CHSEL_MASK) & (~RG_SIF_ADC_PWD_MASK)
               & (~RG_SIF_SIF_EN_MASK) & (~RG_BIAS_SIF_PWD_MASK));
    u4Data = u4Data | RG_SIF_CHSEL_SIF | RG_SIF_ADC_PWD_ON | RG_SIF_SIF_EN_ON | RG_BIAS_SIF_PWD_ON;
    AUD_WRITE32(REG_ATVCFG1, u4Data);
#endif    
}

#if 0
//-----------------------------------------------------------------------------
// vADCPowerOn
//
/** Power on TV audio ADC.
 */
//-----------------------------------------------------------------------------
void vADCPowerOn(void)
{
#if defined(CC_MT5365) || defined(CC_MT5395)
    // Power on APGA & ADC
    UINT32 u4Data;
    
    u4Data = AUD_READ32(REG_ATVCFG0);
    u4Data = (u4Data & (~RG_PGA_SIF_PWD_MASK)) | RG_PGA_SIF_PWD_ON;
    AUD_WRITE32(REG_ATVCFG0, u4Data);
    u4Data = AUD_READ32(REG_ATVCFG1);
    u4Data = (u4Data & (~RG_SIF_ADC_PWD_MASK) & (~RG_BIAS_SIF_PWD_MASK))
    	     | RG_SIF_ADC_PWD_ON| RG_BIAS_SIF_PWD_ON;
    AUD_WRITE32(REG_ATVCFG1, u4Data);
#endif    
}

//-----------------------------------------------------------------------------
// vADCPowerDown
//
/** Power down TV audio ADC.
 */
//-----------------------------------------------------------------------------
void vADCPowerDown(void)
{
#if defined(CC_MT5365) || defined(CC_MT5395)
     UINT32 u4Data;
     
    // Power down APGA & ADC
    u4Data = AUD_READ32(REG_ATVCFG0);
    u4Data = (u4Data & (~RG_PGA_SIF_PWD_MASK)) | RG_PGA_SIF_PWD_DOWN;
    AUD_WRITE32(REG_ATVCFG0, u4Data);

    u4Data = AUD_READ32(REG_ATVCFG1);
    u4Data = (u4Data & (~RG_SIF_ADC_PWD_MASK) & (~RG_BIAS_SIF_PWD_MASK))
    	     | RG_SIF_ADC_PWD_DOWN | RG_BIAS_SIF_PWD_DOWN;
    AUD_WRITE32(REG_ATVCFG1, u4Data); 
#endif    
}
#endif

//-----------------------------------------------------------------------------
// DSP_SetAIFMux
//
/** Set TV audio ADC for AF or SIF path (NTSC).
 *
 *  @param  uAIFPath    0: SIF path,1: AF path.
 */
//-----------------------------------------------------------------------------
void DSP_SetAIFMux(UINT8 uAIFPath)
{
#if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
    UNUSED(uAIFPath);
#else
    UINT32 u4Data;

    if (uAIFPath==AF_PATH)
    {
        // Set AF path, bias current isn't set (default)
        u4Data = AUD_READ32(REG_ATVCFG0);
        u4Data = (u4Data & (~RG_SIF_SEL_MASK)) | SIF_SEL_AF;
        AUD_WRITE32(REG_ATVCFG0, u4Data);
    }
    else // SIF path
    {
        u4Data = AUD_READ32(REG_ATVCFG0);
        u4Data = (u4Data & (~RG_SIF_SEL_MASK)) | SIF_SEL_SIF;
        AUD_WRITE32(REG_ATVCFG0, u4Data);
    }
#endif    
}

//-----------------------------------------------------------------------------
// DSP_SPDIFOutSetSrc
//
/** Set the input PCM data of SPDIF-out from dec1 or dec2.
 *  Also change the AOUT CLK to the sampling rate of specified decoder.
 *
 *  @param  u1DecId     Decoder ID.
 */
//-----------------------------------------------------------------------------
void DSP_SPDIFOutSetSrc(UINT8 u1DecId)
{
    UINT32 u4Tmp;
    UINT32 u4Set = (UINT32)u1DecId;
    
    u4Tmp = AUD_READ32(REG_SPOCFG0);
    u4Tmp &= ~(SPOUT_SPDF_SEL_MASK);
    u4Tmp |= (u4Set << SPOUT_SPDF_SEL_POS);
    AUD_WRITE32(REG_SPOCFG0, u4Tmp);
}

//-----------------------------------------------------------------------------
// DSP_ClearInt
//
/** Clear DSP interrupt.
 */
//-----------------------------------------------------------------------------
void DSP_ClearInt(void)
{
    AUD_WRITE32(REG_D2R_SDA, 0x0);
}

//-----------------------------------------------------------------------------
// DSP_SetAdcTuneCfg
//
/** Set ADC tuner configuration.
 */
//-----------------------------------------------------------------------------
void DSP_SetAdcTuneCfg(void)
{
    UINT32 u4DspRIntData;
    UINT32 u4TempData;    

    DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);
    u4DspRIntData = dReadDspCommDram(ADDR_D2RC_ADC_TUNE_CFG);    

    // Set ADC gain
    u4TempData = AUD_READ32(REG_AUDIN_CFG);
    u4TempData = (u4TempData & (~APGA_GAIN_MASK)) |
        (((u4DspRIntData & DRAM_ADC_GAIN_MASK) >> DRAM_ADC_GAIN_POS) << APGA_GAIN_POS);
    AUD_WRITE32(REG_AUDIN_CFG, u4TempData);

    // Set Offset
    u4TempData = AUD_READ32(REG_AUDIN_CFG);
    u4TempData = (u4TempData & (~APGA_OFFSET_MASK)) |
        (((u4DspRIntData & DRAM_ADC_OFFSET_MASK) >> DRAM_ADC_OFFSET_POS) << APGA_OFFSET_POS);
    AUD_WRITE32(REG_AUDIN_CFG, u4TempData);
}

//-----------------------------------------------------------------------------
// vTriggerDSPSLT
//
/** SLT DSP Trigger.
 */
//-----------------------------------------------------------------------------
void vTriggerDSPSLT(void)
{
    UINT32 dwTmp;
    UINT32 dwDramStatus;
    UINT32 dwRetryCnt = 0;
    
    dwTmp = AUD_READ32(REG_ADSP_CFG);
  
    while(1)
    {
        #if 1
        AUD_WRITE32(REG_ADSP_CFG, ((dwTmp & (~ADSP_CFG_ICACHE_FLUSH)) & (~ADSP_CFG_RST_B)));
        AUD_WRITE32(REG_ADSP_CFG, (dwTmp | ADSP_CFG_ICACHE_FLUSH));
        //x_thread_delay(10);                                      // delay 10 ms
        AUD_WRITE32(REG_ADSP_CFG, (dwTmp | ADSP_CFG_RST_B));    
        #else
        AUD_WRITE32(REG_ADSP_CFG, dwTmp & (~ADSP_CFG_ICACHE_FLUSH) & (~ADSP_CFG_RST_B));
        AUD_WRITE32(REG_ADSP_CFG, dwTmp | ADSP_CFG_ICACHE_FLUSH | ADSP_CFG_RST_B);
        AUD_WRITE32(REG_ADSP_CFG, dwTmp | ADSP_CFG_ICACHE_FLUSH | ADSP_CFG_RST_B);    
        #endif
        // wait cycles to flush
        //x_thread_delay(10);                                      // delay 10 ms
        // check if flush taken
        dwDramStatus = AUD_READ32(REG_ADSP_CFG) & ADSP_CFG_ICACHE_FLUSH;
        if ( (dwDramStatus == 0) || (dwRetryCnt > 20) )
        {
            if (dwDramStatus != 0)
            {
                //_dwDspFlushFailCnt ++;
                Printf("DSP flush cache fail\n");
            }
            break;
        }
        dwRetryCnt ++;
    }
    // Light add LOG of Icache flush retry count
    if (dwRetryCnt)
    {
        Printf("ADSP ICache flush retry count = %d\n",dwRetryCnt);
    }
    #if 0
    AUD_WRITE32(REG_ADSP_CFG, dwTmp & ~ADSP_CFG_RST);  // Reset again
    dwTmp |= ADSP_CFG_RST;                         // enable dsp
    x_thread_delay(20);                              // delay 10 ms
    AUD_WRITE32(REG_ADSP_CFG, dwTmp);                    // enable dsp
    #endif
}

//-----------------------------------------------------------------------------
// DSP_ResetDsp
//
/** Reset DSP.
 */
//-----------------------------------------------------------------------------
void DSP_ResetDsp(void)
{
    AUD_WRITE32(REG_ADSP_CFG, (AUD_READ32(REG_ADSP_CFG) & (~ADSP_CFG_RST_B)));
    AUD_WRITE32(REG_ADSP_CFG, (AUD_READ32(REG_ADSP_CFG) | ADSP_CFG_RST_B));
}

//-----------------------------------------------------------------------------
// DSP_SetAudDramBlock
//
/** Setup Audio DRAM block.
 *
 *  @retval     Sucess or failed.
 */
//-----------------------------------------------------------------------------
CHAR DSP_SetAudDramBlock(UINT8 u1MemBlock)
{
  #if defined(CC_MT5396)
    if (u1MemBlock > 127)
    {
        return 1;
    }
  #elif defined(CC_MT5368)||defined(CC_MT5389)
    if (u1MemBlock > 63)
    {
        return 1;
    }
  #else  
    if (u1MemBlock > 31)
    {
        return 1;
    }
  #endif
  
    AUD_WRITE32(REG_ADSP_CFG, (AUD_READ32(REG_ADSP_CFG) & (~ADSP_AUDIO_BLOCK_MASK)) | (((UINT32)u1MemBlock)<<ADSP_AUDIO_BLOCK_POS)); 
  
    return 0;
}

//-----------------------------------------------------------------------------
// IecInfoInit
//
/** Iec power on init (for both decoders).
 */
//-----------------------------------------------------------------------------
void IecInfoInit (void)
{
    UINT32 dwDspData   = 0;
    UINT16 wPcmChan    = 0;          // Default is LR output
    UINT16 wIecMute    = 0;
    BOOL fgIecMute     = FALSE;
    BOOL fgIecFlag     = FALSE;
    UINT32 dwOtherCfg;

    dwOtherCfg = AUD_READ32(REG_SPOCFG0) & 0x000000FF; // Remaining not iec config
    
    /* for first decoder */
    fgIecFlag = uReadDspShmBYTE (B_IECFLAG)  & 0x01;
    fgIecMute = uReadDspShmBYTE (B_IEC_MUTE) & 0x01;    // IEC signal on/off state
    _rDspStatus[AUD_DEC_MAIN].fgIecMuteState = FALSE;    // IEC mute on DSP side

#ifndef SPDIF_MUTE_BY_PINMUX
    // decide IEC mute
    if ((fgIecFlag && fgIecMute))
    {
        wIecMute = (UINT16) SPOUT_MUTE;
    }
#endif

    // get IEC output selection
    wPcmChan = ((UINT32) uReadDspShmBYTE (B_IEC_PCMCH)) << SPOUT_CHSEL_POS; // bit[4:2]
    // update IEC info
    dwDspData = ((wPcmChan | dwOtherCfg) & (~(1<<SPOUT_MUTE_POS))) | wIecMute;

    // update REG_SPOUT_CFG
    AUD_WRITE32(REG_SPOCFG0, (UINT32) (dwDspData));

#ifdef SPDIF_MUTE_BY_PINMUX
    #if defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)
    if (fgIecFlag && fgIecMute)
    {
        //MUTE
#ifdef CC_HDMI_ARC_SPDIF_CTRL_89
		vIO32WriteFldAlign(PDWNC_PINMUX0, 0, FLD_PAD1_ASPDIF0);
#else
		vIO32WriteFldAlign(CKGEN_PMUX0, 0, FLD_PAD_ASPDIF0);
#endif

    }
    else
    {
        //UNMUTE
#ifdef CC_HDMI_ARC_SPDIF_CTRL_89
		vIO32WriteFldAlign(PDWNC_PINMUX0, 2, FLD_PAD1_ASPDIF0);
#else
		vIO32WriteFldAlign(CKGEN_PMUX0, bSpdifFunc, FLD_PAD_ASPDIF0);
#endif

    }
    #endif
#endif
}

//-----------------------------------------------------------------------------
// _ADSP_CFG_Reset
//
/** Reset ADSP Cfg.
 */
//-----------------------------------------------------------------------------
void _ADSP_CFG_Reset(void)
{
    AUD_WRITE32(REG_ADSP_CFG, ((AUD_READ32(REG_ADSP_CFG)) | ADSP_CFG_CLK_ENA));    // Enable DSP clock
    vResetAndHoldDSP();                         // Reset and hold DSP
    vSetCacheSize(CACHE_SIZE_2K);               // Set cache size
    vSetCacheInstLen(CBUF_SIZE_16X128);         // Set instruction buffer size
    vSetCmptRDThreshold(THRES_UNFULL);          // Set compact read threshold
}

void vSetDspRomCode(UINT32* puSrc,UINT32 dwSz)
{
    UINT32 i;

    // Src: u4GetDspImgAddr () + (dwOfst * 4)
    // Size: (dwSz - 1) * 4 bytes
    WriteREG (RW_ADSPCR, (DR_RD_EN|PMEM_AUTO_INC|PROM_WR |HOT_DNLD));  // disable dsp
    WriteREG (RW_DNLD_CNTR, 0); // reset download counter to 0

    for (i=0; i<dwSz; i++)
    {
        WriteREG (RW_PROM_DNLD, puSrc[i]);
    }

    // Verify
    WriteREG (RW_DNLD_CNTR, 0); // reset download counter to 0
    for (i= 0; i<dwSz; i++)
    {
        //u4Datatmp[i] = ReadREG (RW_PROM_DNLD);
        UINT32 u4Data = ReadREG (RW_PROM_DNLD);
        if (u4Data != puSrc[i])
        {
            Printf("!!! Rom code download fail !!!\n");        
            break;
        }
    }

    WriteREG (RW_ADSPCR, 0);    // clear datarom write
}


