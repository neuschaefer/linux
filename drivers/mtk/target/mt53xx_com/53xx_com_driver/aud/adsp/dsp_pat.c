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
 * $RCSfile: dsp_pat.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file dsp_pat.c
 *  Brief of file dsp_pat.c.
 *  Source file for ADSP related pattern test.
 */


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"
#include "x_ckgen.h"
LINT_EXT_HEADER_BEGIN
#include "x_hal_5381.h"
#include "dsp_common.h"
#include "ctrl_para.h"
#include "adsp_task.h"
#include "dsp_uop.h"
#include "dsp_shm.h"
#include "dsp_intf.h"
#include "dsp_func.h"
#include "dsp_rg_ctl.h"
#include "aud_debug.h"
#include "aud_hw.h"
#include "fbm_drvif.h"  // for DSP memory allocation
#include "drv_common.h"
#include "drv_adsp.h"
#include "drvcust_if.h"

#include "x_os.h"
#include "x_assert.h"

#include "hw_ckgen.h"

#if ((defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)) && !defined(CC_AUD_DISABLE_DSP_PAT))
#include "dsp_pat_table.h"

enum
{
    UINST_TST_PAT = 1,
    TRAP_TST_PAT = 2,
    BS64_TST_PAT = 3,
    BUTFLY_TST_PAT = 4,
    CORDIC_TST_LACTM_PAT = 5,
    CORDIC_TST_PCRDC_PAT = 6,
    DRAM_TST_PAT = 7,
    FADD_TST_PAT = 8,
    GETBITS_TST_NORMAL_PAT = 9,
    GETBITS_TST_14BIT_PAT = 10,
    GETBITS_TST_VORBIS_PAT = 11,
    IIR64_TST_PAT = 12,
    INST_TST_PAT = 13,
    INDEX_TST_PAT = 14,
    MEM_TST_PAT = 15,
    PRAM_TST_PAT = 16,
    PROM_TST_PAT = 17,
    PSRDMA_TST_PAT = 18,
    PUTB_TST_PAT = 19,
    RAND_TST_PAT = 20,
    RORC_TST_PAT = 21,
    SQURT_TST_PAT = 22,
    PRAM_17BIT_TST_PAT = 23,
    ROM_CS_TST_PAT = 24,
    MIDI_TST_PAT = 25,
    SBRCRC_TST_PAT = 26,
    CRC11_TST_PAT = 27,
    MADS64_TST_PAT = 28,
    MAX_TST_PAT
};

typedef struct
{
    const CHAR* szDspPatName;
    UINT16 u2PollAddr;
} DSP_PAT_T;

DSP_PAT_T _arDspPatType[MAX_TST_PAT] =
{
    {"NA",0},
    {"uinst_tst",0},
    {"trap_tst",0x60},
    {"bs64_tst",0x1400},
    {"butfly_tst",0x1586},
    {"cordic_tst_lactm",0x1404},
    {"cordic_tst_pcrdc",0x1404},
    {"dram_tst",0x67},
    {"fadd_tst",0x2003},
    {"getbits_tst_normal",0x10},
    {"getbits_tst_14bit",0x10},
    {"getbits_tst_vorbis",0x10},
    {"iir64_tst",0x1800},
    {"inst_tst",0x1453},
    {"max_min_index_tst",0x192d},
    {"mem_tst",0x60},
    {"pram_tst",0x60},
    {"prom_tst",0x60},
    {"psr_tst",0x67},
    {"putb_tst",0x2012},    
    {"rand_tst",0x62},
    {"rorc_tst",0x2000},
    {"sqrt_tst",0x2007},
    {"pram_17bit_tst",0x60},
    {"rom_cs_tst",0x2020},
    {"midi_tst",0x63},
    {"sbrcrc_tst", 0x10},
    {"crc11_tst", 0x60},
    {"mads64_tst", 0x60}
};


extern UINT32 AUD_DspProbePause(void);
extern UINT32 AUD_DspProbeGo(void);
extern void vSetDspBootFromRam (void);
extern void vResetDSP (void);
extern void vSetDspBootFromRom (void);
extern void vSetDspRomCode(UINT32* puSrc,UINT32 dwSz);
extern void vResetAndHoldDSP (void);
extern void DownloadRomCode(void);
#ifndef CC_FPGA
extern BOOL MID_Reset(void);
#endif

void AUD_DspPatternTest(UINT8 u1PatId,UINT8 u1ShowHelp)
{
    if (u1PatId == 0)
    {
        UINT8 i;
        Printf("[Pattern Revision] %s\n", PAT_REV);                
        Printf("aud.dsp.pat [u1PatId] (0xf00005510 = 11101212 OK)\n");
        for (i=1;i<MAX_TST_PAT;i++)
        {
            Printf("  %2d - %s\n", i, _arDspPatType[i].szDspPatName);
        }        
        Printf("[Note] Please stop all of the audio jobs first by aud.stop, also please stop mid (mid.r)\n");
        Printf("       If you need to step trace from the pattern start 0, please set the bp at 0 first. (aud.dsp.pr.stb + 0 0)\n");
        Printf("       Once you need to retrace the pattern, please reset the dsp (aud.dsp.pr.reset)\n");
    }
    else
    {
        #ifndef CC_FPGA
        MID_Reset();
        #endif
        
        switch (u1PatId)
        {
            case UINST_TST_PAT: //UNDEF
                // 1. pause
                AUD_DspProbePause();
                // 2. boot from ram
                vResetAndHoldDSP();
                vSetDspBootFromRam();
                // 3. load code
                x_memcpy((VOID*)VIRTUAL(_u4DspICacheAddr[0]<<8), (VOID*)UINST_TST_TABLE, UINST_TST_SIZE);
			    // flush cache
				DSP_FlushInvalidateDCacheFree(_u4DspICacheAddr[0]<<8, UINST_TST_SIZE);
				vFlushDspICache (FALSE);
                // 4. reset dsp
                vResetDSP();
                // 5. go DSP
                AUD_DspProbeGo();
                // 6. Print Check Step
                if (u1ShowHelp)
                {
                Printf("[uinst_tst]\n");
                Printf("  aud.dsp.pr.p\n");
                Printf("  aud.dsp.pr.stsr 1 x n\n");
                Printf("  Please check if MONI_DSP_2[14] is set to 1 when encounter undef instruction\n");
                }
                break;
                
            case TRAP_TST_PAT: //TRAP
                // 1. pause
                AUD_DspProbePause();
                // 2. boot from ram
                vResetAndHoldDSP();
                vSetDspBootFromRam();
                // 3. load code
                x_memcpy((VOID*)VIRTUAL(_u4DspICacheAddr[0]<<8), (VOID*)TRAP_TST_TABLE, TRAP_TST_SIZE);
			    // flush cache
				DSP_FlushInvalidateDCacheFree(_u4DspICacheAddr[0]<<8, TRAP_TST_SIZE);
				vFlushDspICache (FALSE);
                // 4. reset dsp
                vResetDSP();
                // 5. go DSP
                AUD_DspProbeGo();
                // 6. Print Check Step
                if (u1ShowHelp)
                {                
                Printf("[trap_tst]\n");
                Printf("  aud.dsp.pc x n => check not hang in local loop\n");
                Printf("  aud.dsp.pr.r 0x60 (loopcnt)\n");                
                Printf("  aud.dsp.pr.stsr 1 x n\n");                
                Printf("  Please check the PC sequence -\n  50~53=>58\n  60~63=>68\n  70~73=>78\n  80~83=>88\n  90~93=>98\n  A0~A3=>A8\n  B0~B3=>B8\n  C0~C3=>C8\n");
                }
                break;
                
            case BS64_TST_PAT: //BS64
                // 1. pause
                AUD_DspProbePause();
                // 2. boot from ram
                vResetAndHoldDSP();                
                vSetDspBootFromRam();
                // 3. load code
                x_memcpy((VOID*)VIRTUAL(_u4DspICacheAddr[0]<<8), (VOID*)BS64_TST_TABLE, BS64_TST_SIZE);
			    // flush cache
				DSP_FlushInvalidateDCacheFree(_u4DspICacheAddr[0]<<8, BS64_TST_SIZE);
				vFlushDspICache (FALSE);
				// 4. reset dsp
                vResetDSP();
                // 5. go DSP
                AUD_DspProbeGo();
                // 6. Print Check Step
                if (u1ShowHelp)
                {                
                Printf("[bs64_tst]\n");
                Printf("  aud.dsp.pc x n\n");
                Printf("  aud.dsp.pr.r 0x1400 (loopcnt)\n");
                }
                break;
                
            case IIR64_TST_PAT: //IIR64
                // 1. pause
                AUD_DspProbePause();
                // 2. boot from ram
                vResetAndHoldDSP();                
                vSetDspBootFromRam();
                // 3. load code
                x_memcpy((VOID*)VIRTUAL(_u4DspICacheAddr[0]<<8), (VOID*)IIR64_TST_TABLE, IIR64_TST_SIZE);
			    // flush cache
				DSP_FlushInvalidateDCacheFree(_u4DspICacheAddr[0]<<8, IIR64_TST_SIZE);
				vFlushDspICache (FALSE);				
                // 4. reset dsp
                vResetDSP();
                // 5. go DSP
                AUD_DspProbeGo();
                // 6. Print Check Step
                if (u1ShowHelp)
                {                
                Printf("[iir64_tst]\n");
                Printf("  aud.dsp.pc x n\n");
                Printf("  aud.dsp.pr.r 0x1800 (loopcnt)\n");
                }
                break;
                
            case CORDIC_TST_LACTM_PAT: //CORDIC
                // 1. pause
                AUD_DspProbePause();
                // 2. boot from ram
                vResetAndHoldDSP();                
                vSetDspBootFromRam();
                // 3. load code
                x_memcpy((VOID*)VIRTUAL(_u4DspICacheAddr[0]<<8), (VOID*)CORDIC_TST_LACTM_TABLE, CORDIC_TST_LACTM_SIZE);
				// flush cache
				DSP_FlushInvalidateDCacheFree(_u4DspICacheAddr[0]<<8, CORDIC_TST_LACTM_SIZE);
				vFlushDspICache (FALSE);	
                // 3.1 load table
                {
                    int i;
                    //x_memcpy((VOID*)VIRTUAL(_u4DspCmptBuf[0]+0x400*4), (VOID*)CORDIC_VECTOR_TABLE, CORDIC_VECTOR_SIZE);
                    for (i=0;i<CORDIC_VECTOR_SIZE;i+=4)
                    {
                        *(UINT32*)(VIRTUAL(_u4DspCmptBuf[0]+0x400*4+i)) = ((*((UINT32*)(CORDIC_VECTOR_TABLE+i))) << 8);
                    }
                }
				DSP_FlushInvalidateDCacheFree(_u4DspCmptBuf[0]+0x400*4, CORDIC_VECTOR_SIZE);
				// 4. reset dsp
                vResetDSP();
                // 5. go DSP
                AUD_DspProbeGo();
                // 6. Print Check Step
                if (u1ShowHelp)
                {                
                Printf("[cordic_tst_lactm]\n");
                Printf("  aud.dsp.pr.r 0x1405 = 00550000h\n");
                Printf("  aud.dsp.pr.r 0x1404 (loopcnt)\n");
                Printf("  aud.dsp.pc x n\n");
                }
                break;

            case CORDIC_TST_PCRDC_PAT: //CORDIC
                // 1. pause
                AUD_DspProbePause();
                // 2. boot from ram
                vResetAndHoldDSP();                
                vSetDspBootFromRam();
                // 3. load code
                x_memcpy((VOID*)VIRTUAL(_u4DspICacheAddr[0]<<8), (VOID*)CORDIC_TST_PCRDC_TABLE, CORDIC_TST_PCRDC_SIZE);
			    // flush cache
				DSP_FlushInvalidateDCacheFree(_u4DspICacheAddr[0]<<8, CORDIC_TST_PCRDC_SIZE);
				vFlushDspICache (FALSE);	
				
                // 3.1 load table
                {
                    int i;
                    //x_memcpy((VOID*)VIRTUAL(_u4DspCmptBuf[0]+0x400*4), (VOID*)CORDIC_VECTOR_TABLE, CORDIC_VECTOR_SIZE);
                    for (i=0;i<CORDIC_VECTOR_SIZE;i+=4)
                    {
                        *(UINT32*)(VIRTUAL(_u4DspCmptBuf[0]+0x400*4+i)) = ((*((UINT32*)(CORDIC_VECTOR_TABLE+i))) << 8);                            
                    }
                }
				
				DSP_FlushInvalidateDCacheFree(_u4DspCmptBuf[0]+0x400*4, CORDIC_VECTOR_SIZE);
                // 4. reset dsp
                vResetDSP();
                // 5. go DSP
                AUD_DspProbeGo();
                // 6. Print Check Step
                if (u1ShowHelp)
                {                
                Printf("[cordic_tst_pcrdc]\n");
                Printf("  aud.dsp.pr.r 0x1405 = 00AA0000h\n");
                Printf("  aud.dsp.pr.r 0x1404 (loopcnt)\n");
                Printf("  aud.dsp.pc x n\n");
                }
                break;

            case BUTFLY_TST_PAT: //BUTFLY
                // 1. pause
                AUD_DspProbePause();
                // 2. boot from ram
                vResetAndHoldDSP();                
                vSetDspBootFromRam();
                // 3. load code
                x_memcpy((VOID*)VIRTUAL(_u4DspICacheAddr[0]<<8), (VOID*)BUTFLY_TST_TABLE, BUTFLY_TST_SIZE);
			    // flush cache
				DSP_FlushInvalidateDCacheFree(_u4DspICacheAddr[0]<<8, BUTFLY_TST_SIZE);
				vFlushDspICache (FALSE);	
				// 4. reset dsp
                vResetDSP();
                // 5. go DSP
                AUD_DspProbeGo();
                // 6. Print Check Step
                if (u1ShowHelp)
                {                
                Printf("[butfly_tst]\n");
                Printf("  aud.dsp.pc x n\n");
                Printf("  aud.dsp.pr.r 0x1586 (loopcnt)\n");                
                }
                break;

            case DRAM_TST_PAT: //DRAM_TST
                // 1. pause
                AUD_DspProbePause();
                // 2. boot from ram
                vResetAndHoldDSP();                
                vSetDspBootFromRam();
                // 3. load code
                x_memcpy((VOID*)VIRTUAL(_u4DspICacheAddr[0]<<8), (VOID*)DRAM_TST_TABLE, DRAM_TST_SIZE);
				// flush
				DSP_FlushInvalidateDCacheFree(_u4DspICacheAddr[0]<<8, DRAM_TST_SIZE);
				vFlushDspICache (FALSE);				
                // 4. reset dsp
                vResetDSP();
                // 5. go DSP
                AUD_DspProbeGo();
                // 6. Print Check Step
                if (u1ShowHelp)
                {                
                Printf("[dram_tst]\n");
                Printf("  aud.dsp.pc x n\n");
                Printf("  aud.dsp.pr.r 0x67 (loopcnt)\n");
                }
                break;

            case FADD_TST_PAT: //FADD_TST
                // 1. pause
                AUD_DspProbePause();
                // 2. boot from ram
                vResetAndHoldDSP();                
                vSetDspBootFromRam();
                // 3. load code
                x_memcpy((VOID*)VIRTUAL(_u4DspICacheAddr[0]<<8), (VOID*)FADD_TST_TABLE, FADD_TST_SIZE);
                // flush cache
				DSP_FlushInvalidateDCacheFree(_u4DspICacheAddr[0]<<8, FADD_TST_SIZE);
				vFlushDspICache (FALSE);
				// 4. reset dsp
                vResetDSP();
                // 5. go DSP
                AUD_DspProbeGo();
                // 6. Print Check Step
                if (u1ShowHelp)
                {                
                Printf("[fadd_tst]\n");
                Printf("  aud.dsp.pc x n\n");
                Printf("  aud.dsp.pr.r 0x2003 (loopcnt)\n");
                }
                break;

            case GETBITS_TST_NORMAL_PAT: //GETBITS_NORMAL
                // 1. pause
                AUD_DspProbePause();
                // 2. boot from ram
                vResetAndHoldDSP();                
                vSetDspBootFromRam();
                // 3. load code
                x_memcpy((VOID*)VIRTUAL(_u4DspICacheAddr[0]<<8), (VOID*)GETBITS_TST_NORMAL_TABLE, GETBITS_TST_NORMAL_SIZE);
                // flush cache
				DSP_FlushInvalidateDCacheFree(_u4DspICacheAddr[0]<<8, GETBITS_TST_NORMAL_SIZE);
				vFlushDspICache (FALSE);

				// 4. reset dsp
                vResetDSP();
                // 5. go DSP
                AUD_DspProbeGo();
                // 6. Print Check Step
                if (u1ShowHelp)
                {                
                Printf("[getbits_tst]\n");
                Printf("  aud.dsp.pc x n\n");
                Printf("  aud.dsp.pr.r 0x1600 = 00000000\n");
                Printf("  aud.dsp.pr.r 0x10 (loopcnt)\n");
                }
                break;

            case GETBITS_TST_14BIT_PAT: //GETBITS_14BIT
                // 1. pause
                AUD_DspProbePause();
                // 2. boot from ram
                vResetAndHoldDSP();                
                vSetDspBootFromRam();
                // 3. load code
                x_memcpy((VOID*)VIRTUAL(_u4DspICacheAddr[0]<<8), (VOID*)GETBITS_TST_14BIT_TABLE, GETBITS_TST_14BIT_SIZE);
                // flush cache
				DSP_FlushInvalidateDCacheFree(_u4DspICacheAddr[0]<<8, GETBITS_TST_14BIT_SIZE);
				vFlushDspICache (FALSE);
				// 4. reset dsp
                vResetDSP();
                // 5. go DSP
                AUD_DspProbeGo();
                // 6. Print Check Step
                if (u1ShowHelp)
                {                
                Printf("[getbits_tst_14bit]\n");
                Printf("  aud.dsp.pc x n\n");
                Printf("  aud.dsp.pr.r 0x1600 = 0003000h\n");                
                Printf("  aud.dsp.pr.r 0x10 (loopcnt)\n");
                }
                break;

            case GETBITS_TST_VORBIS_PAT: //GETBITS_VORBIS
                // 1. pause
                AUD_DspProbePause();
                // 2. boot from ram
                vResetAndHoldDSP();                
                vSetDspBootFromRam();
                // 3. load code
                x_memcpy((VOID*)VIRTUAL(_u4DspICacheAddr[0]<<8), (VOID*)GETBITS_TST_VORBIS_TABLE, GETBITS_TST_VORBIS_SIZE);
                // flush cache
				DSP_FlushInvalidateDCacheFree(_u4DspICacheAddr[0]<<8, GETBITS_TST_VORBIS_SIZE);
				vFlushDspICache (FALSE);

				// 4. reset dsp
                vResetDSP();
                // 5. go DSP
                AUD_DspProbeGo();
                // 6. Print Check Step
                if (u1ShowHelp)
                {                
                Printf("[getbits_tst_vorbis]\n");
                Printf("  aud.dsp.pc x n\n");
                Printf("  aud.dsp.pr.r 0x1600 = 00040000h\n");                
                Printf("  aud.dsp.pr.r 0x10 (loopcnt)\n");
                }
                break;

            case INST_TST_PAT: //INST
                // 1. pause
                AUD_DspProbePause();
                // 2. boot from ram
                vResetAndHoldDSP();                
                vSetDspBootFromRam();
                // 3. load code
                x_memcpy((VOID*)VIRTUAL(_u4DspICacheAddr[0]<<8), (VOID*)INST_TST_TABLE, INST_TST_SIZE);

                // flush
				DSP_FlushInvalidateDCacheFree(_u4DspICacheAddr[0]<<8, INST_TST_SIZE);
				vFlushDspICache (FALSE);	
				
                // 4. reset dsp
                vResetDSP();
                // 5. go DSP
                AUD_DspProbeGo();
                // 6. Print Check Step
                if (u1ShowHelp)
                {                
                Printf("[inst_tst]\n");
                Printf("  aud.dsp.pc x n\n");
                Printf("  aud.dsp.pr.r 0x1453 (loopcnt)\n");
                }
                break;

            case INDEX_TST_PAT: //MAX_MIN_INDEX
                // 1. pause
                AUD_DspProbePause();
                // 2. boot from ram
                vResetAndHoldDSP();                
                vSetDspBootFromRam();
                // 3. load code
                x_memcpy((VOID*)VIRTUAL(_u4DspICacheAddr[0]<<8), (VOID*)INDEX_TST_TABLE, INDEX_TST_SIZE);

                // flush
				DSP_FlushInvalidateDCacheFree(_u4DspICacheAddr[0]<<8, INDEX_TST_SIZE);
				vFlushDspICache (FALSE);

				// 4. reset dsp
                vResetDSP();
                // 5. go DSP
                AUD_DspProbeGo();
                // 6. Print Check Step
                if (u1ShowHelp)
                {                
                Printf("[max_min_index_tst]\n");
                Printf("  aud.dsp.pc x n\n");
                Printf("  aud.dsp.pr.r 0x192d (loopcnt)\n");
                }
                break;

            case MEM_TST_PAT: //MEM_TST
                // 1. pause
                AUD_DspProbePause();
                // 2. boot from ram
                vResetAndHoldDSP();                
                vSetDspBootFromRam();
                // 3. load code
                x_memcpy((VOID*)VIRTUAL(_u4DspICacheAddr[0]<<8), (VOID*)MEM_TST_TABLE, MEM_TST_SIZE);
				// flush
				DSP_FlushInvalidateDCacheFree(_u4DspICacheAddr[0]<<8, MEM_TST_SIZE);
				vFlushDspICache (FALSE);

				// 4. reset dsp
                vResetDSP();
                // 5. go DSP
                AUD_DspProbeGo();
                // 6. Print Check Step
                if (u1ShowHelp)
                {                
                Printf("[mem_tst]\n");
                Printf("  aud.dsp.pc x n\n");
                Printf("  aud.dsp.pr.r 0x60 (loopcnt)\n");
                }                
                break;

            case PRAM_TST_PAT: //PRAM_TST
                // 1. pause
                AUD_DspProbePause();
                // 2. boot from ram
                vResetAndHoldDSP();                
                vSetDspBootFromRam();
                // 3. load code
                x_memcpy((VOID*)VIRTUAL(_u4DspICacheAddr[0]<<8), (VOID*)PRAM_TST_ROM_TABLE, PRAM_TST_ROM_SIZE); //ROM   - main 2K cache   
				DSP_FlushInvalidateDCacheFree(_u4DspICacheAddr[0]<<8, PRAM_TST_ROM_SIZE);
				
                x_memcpy((VOID*)VIRTUAL(_u4DspICacheAddr[2]<<8), (VOID*)PRAM_TST_CRI_TABLE, PRAM_TST_CRI_SIZE); //CRI   - sec 1K cache
				DSP_FlushInvalidateDCacheFree(_u4DspICacheAddr[2]<<8, PRAM_TST_CRI_SIZE);
                
                x_memcpy((VOID*)VIRTUAL(_u4DspICacheAddr[7]<<8), (VOID*)PRAM_TST_DEC2_TABLE, PRAM_TST_DEC2_SIZE); //DEC2  - third 0.25K cache
				DSP_FlushInvalidateDCacheFree(_u4DspICacheAddr[7]<<8, PRAM_TST_DEC2_SIZE);

				vFlushDspICache (FALSE);
				// 4. reset dsp
                vResetDSP();
                // 5. go DSP
                AUD_DspProbeGo();
                // 6. Print Check Step
                if (u1ShowHelp)
                {                
                Printf("[pram_tst]\n");
                Printf("  r 0xf0005080 = xxxxx870\n");                
                Printf("  aud.dsp.pc x n\n");
                Printf("  aud.dsp.pr.r 0x60 (loopcnt)\n");
                Printf("  aud.dsp.cache 0~3 (please check if cache hit rate 0~3=1000)\n");
                }
                break;            
           #ifdef CC_AUD_FPGA  
            case PROM_TST_PAT: //PROM_TST
                // 1. pause
                AUD_DspProbePause();
                // 2. boot from rom
                vResetAndHoldDSP();                
                vSetDspBootFromRom();
                // 3. load code
                vSetDspRomCode((VOID*)PROM_TST_ROM_TABLE, (PROM_TST_ROM_SIZE+2)/4);
                x_memcpy((VOID*)VIRTUAL(_u4DspICacheAddr[2]<<8), (VOID*)PRAM_TST_CRI_TABLE, PRAM_TST_CRI_SIZE); //CRI   - sec 1K cache
               	DSP_FlushInvalidateDCacheFree(_u4DspICacheAddr[2]<<8, PRAM_TST_CRI_SIZE);
				
                x_memcpy((VOID*)VIRTUAL(_u4DspICacheAddr[7]<<8), (VOID*)PRAM_TST_DEC2_TABLE, PRAM_TST_DEC2_SIZE); //DEC2  - third 0.25K cache
                DSP_FlushInvalidateDCacheFree(_u4DspICacheAddr[7]<<8, PRAM_TST_DEC2_SIZE);

				vFlushDspICache (FALSE);
                // 4. reset dsp
                vResetDSP();
                // 5. go DSP
                AUD_DspProbeGo();
                // 6. Print Check Step
                if (u1ShowHelp)
                {                
                Printf("[prom_tst]\n");
                Printf("  r 0xf0005080 = xxxxx830\n");
                Printf("  aud.dsp.pc x n\n");
                Printf("  aud.dsp.pr.r 0x60 (loopcnt)\n");
                Printf("  aud.dsp.cache 0~3 (please check if cache hit rate 0~3=1000)\n");
                }
                break;  
		    #endif		

            case PRAM_17BIT_TST_PAT: //PRAM_17BIT_TST
                // 1. pause
                AUD_DspProbePause();
                // 2. boot from ram
                vResetAndHoldDSP();                
                vSetDspBootFromRam();
                // 3. load code
                x_memcpy((VOID*)VIRTUAL(_u4DspICacheAddr[0]<<8), (VOID*)PRAM_17BIT_TST_ROM_TABLE, PRAM_17BIT_TST_ROM_SIZE); //ROM   - main 2K cache
               	DSP_FlushInvalidateDCacheFree(_u4DspICacheAddr[0]<<8, PRAM_17BIT_TST_ROM_SIZE);
                x_memcpy((VOID*)VIRTUAL(_u4DspICacheAddr[2]<<8), (VOID*)PRAM_17BIT_TST_CRI_TABLE, PRAM_17BIT_TST_CRI_SIZE); //CRI   - sec 1K cache
               	DSP_FlushInvalidateDCacheFree(_u4DspICacheAddr[2]<<8, PRAM_17BIT_TST_CRI_SIZE);
				x_memcpy((VOID*)VIRTUAL((_u4DspICacheAddr[7]<<8)+0x20000), (VOID*)PRAM_17BIT_TST_DEC2_TABLE, PRAM_17BIT_TST_DEC2_SIZE); //DEC2  - third 0.25K cache
               	DSP_FlushInvalidateDCacheFree((_u4DspICacheAddr[7]<<8)+0x20000, PRAM_17BIT_TST_DEC2_SIZE);

				vFlushDspICache (FALSE);

				// 4. reset dsp
                vResetDSP();
                // 5. go DSP
                AUD_DspProbeGo();
                // 6. Print Check Step
                if (u1ShowHelp)
                {                
                Printf("[pram_17bit_tst]\n");
                Printf("  aud.dsp.pc x n\n");
                Printf("  aud.dsp.pr.r 0x60 (loopcnt)\n");
                Printf("  aud.dsp.cache 0~3 (please check if cache hit rate #0<1000,#1<1000,#2=0,#3=1000)\n");
                }
                break;            

            case PSRDMA_TST_PAT: //PSRDMA_TST
                //Parer init
                PARSER_WRITE32(REG_PAR_CTRL, 0);          // Reset parser
                PARSER_WRITE32(REG_PAR_CTRL, 0x3);        // Enable parser
                // 1. pause
                AUD_DspProbePause();    
                // 2. boot from ram
                vResetAndHoldDSP();                
                vSetDspBootFromRam();
                // 3. load code
                x_memcpy((VOID*)VIRTUAL(_u4DspICacheAddr[0]<<8), (VOID*)PSRDMA_TST_TABLE, PSRDMA_TST_SIZE);
				DSP_FlushInvalidateDCacheFree((_u4DspICacheAddr[0]<<8), PSRDMA_TST_SIZE);
				vFlushDspICache (FALSE);
                // 4. reset dsp
                vResetDSP();
                // 5. go DSP
                AUD_DspProbeGo();
                // 6. Print Check Step
                if (u1ShowHelp)
                {                
                Printf("[pram_tst]\n");
                Printf("  aud.dsp.pc x n\n");
                Printf("  aud.dsp.pr.r 0x67 (loopcnt)\n");
                }
                break;            

            case PUTB_TST_PAT: //PUTB_TST
                // 1. pause
                AUD_DspProbePause();    
                // 2. boot from ram
                vResetAndHoldDSP();                
                vSetDspBootFromRam();
                // 3. load code
                x_memcpy((VOID*)VIRTUAL(_u4DspICacheAddr[0]<<8), (VOID*)PUTB_TST_TABLE, PUTB_TST_SIZE);
				DSP_FlushInvalidateDCacheFree((_u4DspICacheAddr[0]<<8), PUTB_TST_SIZE);
				vFlushDspICache (FALSE);
                // 4. reset dsp
                vResetDSP();
                // 5. go DSP
                AUD_DspProbeGo();
                // 6. Print Check Step
                if (u1ShowHelp)
                {                
                Printf("[putb_tst]\n");
                Printf("  aud.dsp.pc x n\n");
                Printf("  aud.dsp.pr.r 0x2012 (loopcnt)\n");
                }
                break;            

            case RAND_TST_PAT: //RAND_TST
                // 1. pause
                AUD_DspProbePause();    
                // 2. boot from ram
                vResetAndHoldDSP();                
                vSetDspBootFromRam();
                // 3. load code
                x_memcpy((VOID*)VIRTUAL(_u4DspICacheAddr[0]<<8), (VOID*)RAND_TST_TABLE, RAND_TST_SIZE);
			    DSP_FlushInvalidateDCacheFree((_u4DspICacheAddr[0]<<8), RAND_TST_SIZE);
				vFlushDspICache (FALSE);
                // 4. reset dsp
                vResetDSP();
                // 5. go DSP
                AUD_DspProbeGo();
                // 6. Print Check Step
                if (u1ShowHelp)
                {                
                Printf("[rand_tst]\n");
                Printf("  aud.dsp.pc x n\n");
                Printf("  aud.dsp.pr.r 0x62 (loopcnt)\n");                
                Printf("  aud.dsp.cm 0x1000 0x10 x n\n");
                Printf("  Please repeat the upper steps several times to check if the generate random value is changed or not\n");
                }                
                break;            

            case RORC_TST_PAT: //RORC_TST
                // 1. pause
                AUD_DspProbePause();    
                // 2. boot from ram
                vResetAndHoldDSP();                
                vSetDspBootFromRam();
                // 3. load code
                x_memcpy((VOID*)VIRTUAL(_u4DspICacheAddr[0]<<8), (VOID*)RORC_TST_TABLE, RORC_TST_SIZE);
			    DSP_FlushInvalidateDCacheFree((_u4DspICacheAddr[0]<<8), RORC_TST_SIZE);
				vFlushDspICache (FALSE);
                // 4. reset dsp
                vResetDSP();
                // 5. go DSP
                AUD_DspProbeGo();
                // 6. Print Check Step
                if (u1ShowHelp)
                {                
                Printf("[rorc_tst]\n");
                Printf("  aud.dsp.pc x n\n");
                Printf("  aud.dsp.pr.r 0x2000 (loopcnt)\n");
                }
                break;            
   
            case SQURT_TST_PAT: //SQURT_TST
                // 1. pause
                AUD_DspProbePause();    
                // 2. boot from ram
                vResetAndHoldDSP();                
                vSetDspBootFromRam();
                // 3. load code
                x_memcpy((VOID*)VIRTUAL(_u4DspICacheAddr[0]<<8), (VOID*)SQURT_TST_TABLE, SQURT_TST_SIZE);
				DSP_FlushInvalidateDCacheFree((_u4DspICacheAddr[0]<<8), SQURT_TST_SIZE);
				vFlushDspICache (FALSE);
                // 4. reset dsp
                vResetDSP();
                // 5. go DSP
                AUD_DspProbeGo();
                // 6. Print Check Step
                if (u1ShowHelp)
                {                
                Printf("[squrt_tst]\n");
                Printf("  aud.dsp.pc x n\n");
                Printf("  aud.dsp.pr.r 0x2007 (loopcnt)\n");
                }
                break;            

            case ROM_CS_TST_PAT: //ROM_CS_TST
                // 1. pause
                AUD_DspProbePause();
                // 1.5 download system rom code
                DownloadRomCode();                
                // 2. boot from ram
                vResetAndHoldDSP();                
                vSetDspBootFromRam();
                // 3. load code
                x_memcpy((VOID*)VIRTUAL(_u4DspICacheAddr[0]<<8), (VOID*)ROM_CS_TST_TABLE, ROM_CS_TST_SIZE);
				DSP_FlushInvalidateDCacheFree((_u4DspICacheAddr[0]<<8), ROM_CS_TST_SIZE);
				vFlushDspICache (FALSE);				
                // 4. reset dsp
                vResetDSP();
                // 5. go DSP
                AUD_DspProbeGo();
                // 6. Print Check Step
                if (u1ShowHelp)
                {                
                Printf("[rom_cs_tst]\n");
                Printf("  aud.dsp.pc x n\n");
                Printf("  aud.dsp.pr.r 0x2020 (loopcnt)\n");
                }
                break;            

            case MIDI_TST_PAT: //MIDI_TST
                // 1. pause
                AUD_DspProbePause();
                // 1.5 download system rom code
                DownloadRomCode();                
                // 2. boot from ram
                vResetAndHoldDSP();                
                vSetDspBootFromRam();
                // 3. load code
                x_memcpy((VOID*)VIRTUAL(_u4DspICacheAddr[0]<<8), (VOID*)MIDI_TST_TABLE, MIDI_TST_SIZE);
				DSP_FlushInvalidateDCacheFree((_u4DspICacheAddr[0]<<8), MIDI_TST_SIZE);
				vFlushDspICache (FALSE);					
                // 4. reset dsp
                vResetDSP();
                // 5. go DSP
                AUD_DspProbeGo();
                // 6. Print Check Step
                if (u1ShowHelp)
                {                
                Printf("[midi_tst]\n");
                Printf("  aud.dsp.pc x n\n");
                Printf("  aud.dsp.pr.r 0x63 (loopcnt)\n");
                Printf("  aud.dsp.pr.r 0x65 (Phase or err address)\n");
                Printf("  aud.dsp.pr.r 0x1800 0x200 (MIDI data)\n");
                }
                break;            

            case SBRCRC_TST_PAT: //SBRCRC_TST
                // 1. pause
                AUD_DspProbePause();
                // 1.5 download system rom code
                DownloadRomCode();                
                // 2. boot from ram
                vResetAndHoldDSP();                
                vSetDspBootFromRam();
                // 3. load code
                x_memcpy((VOID*)VIRTUAL(_u4DspICacheAddr[0]<<8), (VOID*)SBRCRC_TST_TABLE, SBRCRC_TST_SIZE);
			    DSP_FlushInvalidateDCacheFree((_u4DspICacheAddr[0]<<8), SBRCRC_TST_SIZE);
				vFlushDspICache (FALSE);
                // 4. reset dsp
                vResetDSP();
                // 5. go DSP
                AUD_DspProbeGo();
                // 6. Print Check Step
                if (u1ShowHelp)
                {                
                Printf("[sbrcrc_tst]\n");
                Printf("  aud.dsp.pc x n\n");
                Printf("  aud.dsp.pr.r 0x10 (loopcnt)\n");
                }
                break;                            

            case CRC11_TST_PAT: //CRC11_TST
                // 1. pause
                AUD_DspProbePause();
                // 1.5 download system rom code
                DownloadRomCode();                
                // 2. boot from ram
                vResetAndHoldDSP();                
                vSetDspBootFromRam();
                // 3. load code
                x_memcpy((VOID*)VIRTUAL(_u4DspICacheAddr[0]<<8), (VOID*)CRC11_TST_TABLE, CRC11_TST_SIZE);
			    DSP_FlushInvalidateDCacheFree((_u4DspICacheAddr[0]<<8), CRC11_TST_SIZE);
				vFlushDspICache (FALSE);				
                // 4. reset dsp
                vResetDSP();
                // 5. go DSP
                AUD_DspProbeGo();
                // 6. Print Check Step
                if (u1ShowHelp)
                {                
                Printf("[crc11_tst]\n");
                Printf("  aud.dsp.pc x n\n");
                Printf("  aud.dsp.pr.r 0x60 (loopcnt)\n");
                }
                break;                       

            case MADS64_TST_PAT: //MADS64_TST
                // 1. pause
                AUD_DspProbePause();
                // 1.5 download system rom code
                DownloadRomCode();                
                // 2. boot from ram
                vResetAndHoldDSP();                
                vSetDspBootFromRam();
                // 3. load code
                x_memcpy((VOID*)VIRTUAL(_u4DspICacheAddr[0]<<8), (VOID*)MADS64_TST_TABLE, MADS64_TST_SIZE);
			    DSP_FlushInvalidateDCacheFree((_u4DspICacheAddr[0]<<8), MADS64_TST_SIZE);
				vFlushDspICache (FALSE);
                // 4. reset dsp
                vResetDSP();
                // 5. go DSP
                AUD_DspProbeGo();
                // 6. Print Check Step
                if (u1ShowHelp)
                {                
                Printf("[mads64_tst]\n");
                Printf("  aud.dsp.pc x n\n");
                Printf("  aud.dsp.pr.r 0x60 (loopcnt)\n");
                }
                break;                   

            default:
                if (u1ShowHelp)
                {                
                Printf("unsupport pattern\n");
                }
                break;
        }
    }
}

extern UINT32 AUD_DspProbePause(void);
extern UINT32 AUD_DspProbeGo(void);            
extern UINT32 u4AUD_DspProbeRead(UINT32 u4Addr);

void AUD_DspPatternAutoTest(void)
{
    UINT8 i,j;
    UINT8 u1LocalLoopCnt = 0;
    UINT32 u4LoopCnt,u4Tmp;

    #ifndef CC_FPGA
    MID_Reset();
    #endif

    Printf("[Pattern Revision] %s\n", PAT_REV);
    for (i=2;i<MAX_TST_PAT;i++)
    {
        Printf("Test %2d: %s ....", i, _arDspPatType[i].szDspPatName);
		if (i == PROM_TST_PAT)
			{
			  Printf("FPGA test only\n");
			  continue;
			}
        AUD_DspPatternTest(i,0);

        u4LoopCnt = 0;
        for (j=0;j<250;j++)
        {
_RETEST_:        
            x_thread_delay(10);
            AUD_DspProbePause();
            u4Tmp = u4AUD_DspProbeRead(_arDspPatType[i].u2PollAddr);
            AUD_DspProbeGo();

            #if 1
            if (u4Tmp == u4LoopCnt)
            {
                u1LocalLoopCnt++;
                if (u1LocalLoopCnt > 10)
                {
                    break;
                }
                goto _RETEST_;
            }
            #else
            if (u4Tmp == u4LoopCnt)
                break;
            #endif
            u4LoopCnt = u4Tmp;
            u1LocalLoopCnt = 0;
        }        

        if (j==250)
        {
            Printf("Pass\n");
        }
        else
        {
            Printf("Fail\n");
            break;
        }
    }
}

#if 0
const UINT8 Tone_1K[]=
{
	0x00,0x00,0x00,
	0x00,0xab,0x01,
	0x00,0x50,0x03,
	0x00,0xe5,0x04,
	0x00,0x66,0x06,
	0x00,0xca,0x07,
	0x00,0x0c,0x09,
	0x00,0x27,0x0a,
	0x00,0x15,0x0b,
	0x00,0xd3,0x0b,
	0x00,0x5d,0x0c,
	0x00,0xb0,0x0c,
	0x00,0xcc,0x0c,
	0x00,0xb0,0x0c,
	0x00,0x5d,0x0c,
	0x00,0xd3,0x0b,
	0x00,0x15,0x0b,
	0x00,0x27,0x0a,
	0x00,0x0c,0x09,
	0x00,0xca,0x07,
	0x00,0x66,0x06,
	0x00,0xe5,0x04,
	0x00,0x50,0x03,
	0x00,0xab,0x01,
	0x00,0x00,0x00,
	0x00,0x55,0xfe,
	0x00,0xb0,0xfc,
	0x00,0x1b,0xfb,
	0x00,0x9a,0xf9,
	0x00,0x36,0xf8,
	0x00,0xf4,0xf6,
	0x00,0xd9,0xf5,
	0x00,0xeb,0xf4,
	0x00,0x2d,0xf4,
	0x00,0xa3,0xf3,
	0x00,0x50,0xf3,
	0x00,0x34,0xf3,
	0x00,0x50,0xf3,
	0x00,0xa3,0xf3,
	0x00,0x2d,0xf4,
	0x00,0xeb,0xf4,
	0x00,0xd9,0xf5,
	0x00,0xf4,0xf6,
	0x00,0x36,0xf8,
	0x00,0x9a,0xf9,
	0x00,0x1b,0xfb,
	0x00,0xb0,0xfc,
	0x00,0x55,0xfe,
};

const UINT8 Tone_2K[]=
{
	0x00,0x00,0x00,
	0x00,0x50,0x03,
	0x00,0x66,0x06,
	0x00,0x0c,0x09,
	0x00,0x15,0x0b,
	0x00,0x5d,0x0c,
	0x00,0xcc,0x0c,
	0x00,0x5d,0x0c,
	0x00,0x15,0x0b,
	0x00,0x0c,0x09,
	0x00,0x66,0x06,
	0x00,0x50,0x03,
	0x00,0x00,0x00,
	0x00,0xb0,0xfc,
	0x00,0x9a,0xf9,
	0x00,0xf4,0xf6,
	0x00,0xeb,0xf4,
	0x00,0xa3,0xf3,
	0x00,0x34,0xf3,
	0x00,0xa3,0xf3,
	0x00,0xeb,0xf4,
	0x00,0xf4,0xf6,
	0x00,0x9a,0xf9,
	0x00,0xb0,0xfc,
};

const UINT8 Tone_3K[]=
{
	0x00,0x00,0x00,
	0x00,0xe5,0x04,
	0x00,0x0c,0x09,
	0x00,0xd3,0x0b,
	0x00,0xcc,0x0c,
	0x00,0xd3,0x0b,
	0x00,0x0c,0x09,
	0x00,0xe5,0x04,
	0x00,0x00,0x00,
	0x00,0x1b,0xfb,
	0x00,0xf4,0xf6,
	0x00,0x2d,0xf4,
	0x00,0x34,0xf3,
	0x00,0x2d,0xf4,
	0x00,0xf4,0xf6,
	0x00,0x1b,0xfb,
};

const UINT8 Tone_4K[]=
{
	0x00,0x00,0x00,
	0x00,0x66,0x06,
	0x00,0x15,0x0b,
	0x00,0xcc,0x0c,
	0x00,0x15,0x0b,
	0x00,0x66,0x06,
	0x00,0x00,0x00,
	0x00,0x9a,0xf9,
	0x00,0xeb,0xf4,
	0x00,0x34,0xf3,
	0x00,0xeb,0xf4,
	0x00,0x9a,0xf9,
};

const UINT8 Tone_6K[]=
{
	0x00,0x00,0x00,
	0x00,0x0c,0x09,
	0x00,0xcc,0x0c,
	0x00,0x0c,0x09,
	0x00,0x00,0x00,
	0x00,0xf4,0xf6,
	0x00,0x34,0xf3,
	0x00,0xf4,0xf6,
};

void AUD_FillAFIFO1Buffer(UINT8 u1Case,UINT32 interval)
{
    int i,j;
    UINT32 ch_addr[10];

    for (i=0;i<10;i++)
    {
        ch_addr[i] = (_u4DspDramBuf[0]) + 0x3600*4 + 0x2400*4*i;
    }

    switch (u1Case)
    {
        case 0:    
        default:
            for (i=ch_addr[0];i<ch_addr[0]+0x2400*4;i+=sizeof(Tone_1K))
            {
                x_memcpy((VOID*)i, (VOID*)Tone_1K, sizeof(Tone_1K));
            }

            for (i=ch_addr[1];i<ch_addr[1]+0x2400*4;i+=sizeof(Tone_1K))
            {
                x_memcpy((VOID*)i, (VOID*)Tone_1K, sizeof(Tone_1K));
            }

            for (i=ch_addr[2];i<ch_addr[2]+0x2400*4;i+=sizeof(Tone_2K))
            {
                x_memcpy((VOID*)i, (VOID*)Tone_2K, sizeof(Tone_2K));
            }

            for (i=ch_addr[3];i<ch_addr[3]+0x2400*4;i+=sizeof(Tone_2K))
            {
                x_memcpy((VOID*)i, (VOID*)Tone_2K, sizeof(Tone_2K));
            }

            for (i=ch_addr[4];i<ch_addr[4]+0x2400*4;i+=sizeof(Tone_3K))
            {
                x_memcpy((VOID*)i, (VOID*)Tone_3K, sizeof(Tone_3K));
            }

            for (i=ch_addr[5];i<ch_addr[5]+0x2400*4;i+=sizeof(Tone_3K))
            {
                x_memcpy((VOID*)i, (VOID*)Tone_3K, sizeof(Tone_3K));
            }

            for (i=ch_addr[6];i<ch_addr[6]+0x2400*4;i+=sizeof(Tone_4K))
            {
                x_memcpy((VOID*)i, (VOID*)Tone_4K, sizeof(Tone_4K));
            }

            for (i=ch_addr[7];i<ch_addr[7]+0x2400*4;i+=sizeof(Tone_4K))
            {
                x_memcpy((VOID*)i, (VOID*)Tone_4K, sizeof(Tone_4K));
            }

            for (i=ch_addr[8];i<ch_addr[8]+0x2400*4;i+=sizeof(Tone_6K))
            {
                x_memcpy((VOID*)i, (VOID*)Tone_6K, sizeof(Tone_6K));
            }

            for (i=ch_addr[9];i<ch_addr[9]+0x2400*4;i+=sizeof(Tone_6K))
            {
                x_memcpy((VOID*)i, (VOID*)Tone_6K, sizeof(Tone_6K));
            }
            break;

        case 1:
            for (i=ch_addr[0];i<ch_addr[0]+0x2400*4;i++)
            {
                *((UINT8*)i) = 0x11;
            }
            for (i=ch_addr[1];i<ch_addr[1]+0x2400*4;i++)
            {
                *((UINT8*)i) = 0x22;
            }            
            for (i=ch_addr[2];i<ch_addr[2]+0x2400*4;i++)
            {
                *((UINT8*)i) = 0x33;
            }
            for (i=ch_addr[3];i<ch_addr[3]+0x2400*4;i++)
            {
                *((UINT8*)i) = 0x44;
            }
            for (i=ch_addr[4];i<ch_addr[4]+0x2400*4;i++)
            {
                *((UINT8*)i) = 0x55;
            }
            for (i=ch_addr[5];i<ch_addr[5]+0x2400*4;i++)
            {
                *((UINT8*)i) = 0x66;
            }
            for (i=ch_addr[6];i<ch_addr[6]+0x2400*4;i++)
            {
                *((UINT8*)i) = 0x77;
            }
            for (i=ch_addr[7];i<ch_addr[7]+0x2400*4;i++)
            {
                *((UINT8*)i) = 0x88;
            }
            for (i=ch_addr[8];i<ch_addr[8]+0x2400*4;i++)
            {
                *((UINT8*)i) = 0x99;
            }
            for (i=ch_addr[9];i<ch_addr[9]+0x2400*4;i++)
            {
                *((UINT8*)i) = 0xaa;
            }
            break;

        case 2:
            for (i=ch_addr[0];i<ch_addr[0]+0x2400*4;i+=interval)
            {
                //pa
                *((UINT8*)i) = 0x00;
                *((UINT8*)i+1) = 0x72;
                *((UINT8*)i+2) = 0xf8;
                //pc
                *((UINT8*)i+3) = 0x00;
                *((UINT8*)i+4) = 0x01;
                *((UINT8*)i+5) = 0x00;                
                for (j=6;j<0x246;j++)
                {
                    *((UINT8*)i+j) = 0x11;
                }
                for (j=0x246;j<interval;j++)
                {
                    *((UINT8*)i+j) = 0x00;
                }                
            }
            for (i=ch_addr[1];i<ch_addr[1]+0x2400*4;i+=interval)
            {
                //pb
                *((UINT8*)i) = 0x00;
                *((UINT8*)i+1) = 0x1f;
                *((UINT8*)i+2) = 0x4e;
                //pd
                *((UINT8*)i+3) = 0x00;
                *((UINT8*)i+4) = 0x00;  // 0x300 * 8 = 0x1800
                *((UINT8*)i+5) = 0x18;
                
                for (j=6;j<0x246;j++)
                {
                    *((UINT8*)i+j) = 0x22;
                }
                for (j=0x246;j<interval;j++)
                {
                    *((UINT8*)i+j) = 0x00;
                }                
            }            
            for (i=ch_addr[2];i<ch_addr[2]+0x2400*4;i++)
            {
                *((UINT8*)i) = 0x33;
            }
            for (i=ch_addr[3];i<ch_addr[3]+0x2400*4;i++)
            {
                *((UINT8*)i) = 0x44;
            }
            for (i=ch_addr[4];i<ch_addr[4]+0x2400*4;i++)
            {
                *((UINT8*)i) = 0x55;
            }
            for (i=ch_addr[5];i<ch_addr[5]+0x2400*4;i++)
            {
                *((UINT8*)i) = 0x66;
            }
            for (i=ch_addr[6];i<ch_addr[6]+0x2400*4;i++)
            {
                *((UINT8*)i) = 0x77;
            }
            for (i=ch_addr[7];i<ch_addr[7]+0x2400*4;i++)
            {
                *((UINT8*)i) = 0x88;
            }
            for (i=ch_addr[8];i<ch_addr[8]+0x2400*4;i++)
            {
                *((UINT8*)i) = 0x99;
            }
            for (i=ch_addr[9];i<ch_addr[9]+0x2400*4;i++)
            {
                *((UINT8*)i) = 0xaa;
            }
            break;          

        case 3: //No use, order is not right
            for (i=ch_addr[0];i<ch_addr[0]+0x2400*4;i+=interval)
            {
                //pa
                *((UINT8*)i) = 0x00;
                *((UINT8*)i+1) = 0x72;
                *((UINT8*)i+2) = 0xf8;
                for (j=3;j<0x93;j++)
                {
                    *((UINT8*)i+j) = 0x11;
                }
                for (j=0x93;j<interval;j++)
                {
                    *((UINT8*)i+j) = 0x00;
                }                
            }
            for (i=ch_addr[1];i<ch_addr[1]+0x2400*4;i+=interval)
            {                
                for (j=0;j<0x90;j++)
                {
                    *((UINT8*)i+j) = 0x22;
                }
                for (j=0x90;j<interval;j++)
                {
                    *((UINT8*)i+j) = 0x00;
                }                
            }            
            for (i=ch_addr[2];i<ch_addr[2]+0x2400*4;i+=interval)
            {
                //pb
                *((UINT8*)i) = 0x00;
                *((UINT8*)i+1) = 0x1f;
                *((UINT8*)i+2) = 0x4e;
                for (j=3;j<0x93;j++)
                {
                    *((UINT8*)i+j) = 0x33;
                }
                for (j=0x93;j<interval;j++)
                {
                    *((UINT8*)i+j) = 0x00;
                }                
            }
            for (i=ch_addr[3];i<ch_addr[3]+0x2400*4;i+=interval)
            {
                for (j=0;j<0x90;j++)
                {
                    *((UINT8*)i+j) = 0x44;
                }
                for (j=0x90;j<interval;j++)
                {
                    *((UINT8*)i+j) = 0x00;
                }                                
            }
            for (i=ch_addr[4];i<ch_addr[4]+0x2400*4;i+=interval)
            {
                //pc
                *((UINT8*)i+0) = 0x00;
                *((UINT8*)i+1) = 0x01;
                *((UINT8*)i+2) = 0x00;
                for (j=3;j<0x93;j++)
                {
                    *((UINT8*)i+j) = 0x55;
                }
                for (j=0x93;j<interval;j++)
                {
                    *((UINT8*)i+j) = 0x00;
                }
            }
            for (i=ch_addr[5];i<ch_addr[5]+0x2400*4;i+=interval)
            {
                for (j=0;j<0x90;j++)
                {
                    *((UINT8*)i+j) = 0x66;
                }
                for (j=0x90;j<interval;j++)
                {
                    *((UINT8*)i+j) = 0x00;
                }                                
            }
            for (i=ch_addr[6];i<ch_addr[6]+0x2400*4;i+=interval)
            {
                //pd
                *((UINT8*)i+0) = 0x00;
                *((UINT8*)i+1) = 0x00;  // 0x300 * 8 = 0x1800
                *((UINT8*)i+2) = 0x18;

                for (j=3;j<0x93;j++)
                {
                    *((UINT8*)i+j) = 0x77;
                }
                for (j=0x93;j<interval;j++)
                {
                    *((UINT8*)i+j) = 0x00;
                }
            }
            for (i=ch_addr[7];i<ch_addr[7]+0x2400*4;i+=interval)
            {
                for (j=0;j<0x90;j++)
                {
                    *((UINT8*)i+j) = 0x88;
                }
                for (j=0x90;j<interval;j++)
                {
                    *((UINT8*)i+j) = 0x00;
                }                
            }
            for (i=ch_addr[8];i<ch_addr[8]+0x2400*4;i++)
            {
                *((UINT8*)i) = 0x99;
            }
            for (i=ch_addr[9];i<ch_addr[9]+0x2400*4;i++)
            {
                *((UINT8*)i) = 0xaa;
            }
            break;            
            
        case 4:
            for (i=ch_addr[0];i<ch_addr[0]+0x2400*4;i+=interval)
            {
                //pa
                *((UINT8*)i) = 0x00;
                *((UINT8*)i+1) = 0x72;
                *((UINT8*)i+2) = 0xf8;
                for (j=3;j<0x93;j++)
                {
                    *((UINT8*)i+j) = 0x11;
                }
                for (j=0x93;j<interval;j++)
                {
                    *((UINT8*)i+j) = 0x00;
                }                
            }
            for (i=ch_addr[1];i<ch_addr[1]+0x2400*4;i+=interval)
            {                
                //pb
                *((UINT8*)i) = 0x00;
                *((UINT8*)i+1) = 0x1f;
                *((UINT8*)i+2) = 0x4e;
                for (j=3;j<0x93;j++)
                {
                    *((UINT8*)i+j) = 0x22;
                }
                for (j=0x93;j<interval;j++)
                {
                    *((UINT8*)i+j) = 0x00;
                }                            
            }            
            for (i=ch_addr[2];i<ch_addr[2]+0x2400*4;i+=interval)
            {
                //pc
                *((UINT8*)i+0) = 0x00;
                *((UINT8*)i+1) = 0x01;
                *((UINT8*)i+2) = 0x00;
                for (j=3;j<0x93;j++)
                {
                    *((UINT8*)i+j) = 0x33;
                }
                for (j=0x93;j<interval;j++)
                {
                    *((UINT8*)i+j) = 0x00;
                }            
            }
            for (i=ch_addr[3];i<ch_addr[3]+0x2400*4;i+=interval)
            {
                //pd
                *((UINT8*)i+0) = 0x00;
                *((UINT8*)i+1) = 0x00;  // 0x300 * 8 = 0x1800
                *((UINT8*)i+2) = 0x18;

                for (j=3;j<0x93;j++)
                {
                    *((UINT8*)i+j) = 0x44;
                }
                for (j=0x93;j<interval;j++)
                {
                    *((UINT8*)i+j) = 0x00;
                }
            }
            for (i=ch_addr[4];i<ch_addr[4]+0x2400*4;i+=interval)
            {
                for (j=0;j<0x90;j++)
                {
                    *((UINT8*)i+j) = 0x55;
                }
                for (j=0x90;j<interval;j++)
                {
                    *((UINT8*)i+j) = 0x00;
                }                
            
            }
            for (i=ch_addr[5];i<ch_addr[5]+0x2400*4;i+=interval)
            {
                for (j=0;j<0x90;j++)
                {
                    *((UINT8*)i+j) = 0x66;
                }
                for (j=0x90;j<interval;j++)
                {
                    *((UINT8*)i+j) = 0x00;
                }                                
            }
            for (i=ch_addr[6];i<ch_addr[6]+0x2400*4;i+=interval)
            {
                for (j=0;j<0x90;j++)
                {
                    *((UINT8*)i+j) = 0x77;
                }
                for (j=0x90;j<interval;j++)
                {
                    *((UINT8*)i+j) = 0x00;
                }                                            
            }
            for (i=ch_addr[7];i<ch_addr[7]+0x2400*4;i+=interval)
            {
                for (j=0;j<0x90;j++)
                {
                    *((UINT8*)i+j) = 0x88;
                }
                for (j=0x90;j<interval;j++)
                {
                    *((UINT8*)i+j) = 0x00;
                }                
            }
            for (i=ch_addr[8];i<ch_addr[8]+0x2400*4;i++)
            {
                *((UINT8*)i) = 0x99;
            }
            for (i=ch_addr[9];i<ch_addr[9]+0x2400*4;i++)
            {
                *((UINT8*)i) = 0xaa;
            }
            break;            

        case 5: //DTS-16
            for (i=ch_addr[0];i<ch_addr[0]+0x2400*4;i+=interval)
            {
                //pa
                *((UINT8*)i) = 0x00;
                *((UINT8*)i+1) = 0xfe;
                *((UINT8*)i+2) = 0x7f;
                //pc
                *((UINT8*)i+3) = 0x00;
                *((UINT8*)i+4) = 0x00;
                *((UINT8*)i+5) = 0xfc;                
                for (j=6;j<0x246;j++)
                {
                    *((UINT8*)i+j) = 0x11;
                }
                for (j=0x246;j<interval;j++)
                {
                    *((UINT8*)i+j) = 0x00;
                }                
            }
            for (i=ch_addr[1];i<ch_addr[1]+0x2400*4;i+=interval)
            {
                //pb
                *((UINT8*)i) = 0x00;
                *((UINT8*)i+1) = 0x01;
                *((UINT8*)i+2) = 0x80;
                //pd
                *((UINT8*)i+3) = 0x00;
                *((UINT8*)i+4) = 0x00;  // 0x300 * 8 = 0x1800
                *((UINT8*)i+5) = 0x00;
                
                for (j=6;j<0x246;j++)
                {
                    *((UINT8*)i+j) = 0x22;
                }
                for (j=0x246;j<interval;j++)
                {
                    *((UINT8*)i+j) = 0x00;
                }                
            }            
            for (i=ch_addr[2];i<ch_addr[2]+0x2400*4;i++)
            {
                *((UINT8*)i) = 0x33;
            }
            for (i=ch_addr[3];i<ch_addr[3]+0x2400*4;i++)
            {
                *((UINT8*)i) = 0x44;
            }
            for (i=ch_addr[4];i<ch_addr[4]+0x2400*4;i++)
            {
                *((UINT8*)i) = 0x55;
            }
            for (i=ch_addr[5];i<ch_addr[5]+0x2400*4;i++)
            {
                *((UINT8*)i) = 0x66;
            }
            for (i=ch_addr[6];i<ch_addr[6]+0x2400*4;i++)
            {
                *((UINT8*)i) = 0x77;
            }
            for (i=ch_addr[7];i<ch_addr[7]+0x2400*4;i++)
            {
                *((UINT8*)i) = 0x88;
            }
            for (i=ch_addr[8];i<ch_addr[8]+0x2400*4;i++)
            {
                *((UINT8*)i) = 0x99;
            }
            for (i=ch_addr[9];i<ch_addr[9]+0x2400*4;i++)
            {
                *((UINT8*)i) = 0xaa;
            }
            break;              

        case 6: //DTS-14
            for (i=ch_addr[0];i<ch_addr[0]+0x2400*4;i+=interval)
            {
                //pa
                *((UINT8*)i) = 0x00;
                *((UINT8*)i+1) = 0xFF;
                *((UINT8*)i+2) = 0x1F;
                //pc
                *((UINT8*)i+3) = 0x00;
                *((UINT8*)i+4) = 0xF0;
                *((UINT8*)i+5) = 0x07;                
                for (j=6;j<0x246;j++)
                {
                    *((UINT8*)i+j) = 0x11;
                }
                for (j=0x246;j<interval;j++)
                {
                    *((UINT8*)i+j) = 0x00;
                }                
            }
            for (i=ch_addr[1];i<ch_addr[1]+0x2400*4;i+=interval)
            {
                //pb
                *((UINT8*)i) = 0x00;
                *((UINT8*)i+1) = 0x00;
                *((UINT8*)i+2) = 0xE8;
                //pd
                *((UINT8*)i+3) = 0x00;
                *((UINT8*)i+4) = 0x00;  // 0x300 * 8 = 0x1800
                *((UINT8*)i+5) = 0x00;
                
                for (j=6;j<0x246;j++)
                {
                    *((UINT8*)i+j) = 0x22;
                }
                for (j=0x246;j<interval;j++)
                {
                    *((UINT8*)i+j) = 0x00;
                }                
            }            
            for (i=ch_addr[2];i<ch_addr[2]+0x2400*4;i++)
            {
                *((UINT8*)i) = 0x33;
            }
            for (i=ch_addr[3];i<ch_addr[3]+0x2400*4;i++)
            {
                *((UINT8*)i) = 0x44;
            }
            for (i=ch_addr[4];i<ch_addr[4]+0x2400*4;i++)
            {
                *((UINT8*)i) = 0x55;
            }
            for (i=ch_addr[5];i<ch_addr[5]+0x2400*4;i++)
            {
                *((UINT8*)i) = 0x66;
            }
            for (i=ch_addr[6];i<ch_addr[6]+0x2400*4;i++)
            {
                *((UINT8*)i) = 0x77;
            }
            for (i=ch_addr[7];i<ch_addr[7]+0x2400*4;i++)
            {
                *((UINT8*)i) = 0x88;
            }
            for (i=ch_addr[8];i<ch_addr[8]+0x2400*4;i++)
            {
                *((UINT8*)i) = 0x99;
            }
            for (i=ch_addr[9];i<ch_addr[9]+0x2400*4;i++)
            {
                *((UINT8*)i) = 0xaa;
            }
            break;                      
    }
            
}
#endif

#endif

