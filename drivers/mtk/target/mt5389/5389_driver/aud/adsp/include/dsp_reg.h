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
 * $RCSfile: dsp_reg.h,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file dsp_reg.h
 *  Brief of file dsp_reg.h.
 *  Details of file dsp_reg.h (optional).
 */

#ifdef AUD_EXTERN_FILE
#error "Please don't include this header file in release C file"
#endif
 
#ifndef DSP_REG_H
#define DSP_REG_H

#include "x_typedef.h"
#include "x_hal_5381.h"
#include "x_hal_926.h"
#include "rc2d_int.h"
#include "d2rc_int.h"
#include "dsp_common.h"
#include "ctrl_para.h"
#include "aud_hw.h"

//-----------------------------------------------------------------------------
// ADSP RISC Register Map
//-----------------------------------------------------------------------------
#define ADSP_REG_OFST                           (0x40)

// DSP Control Reg (5100h)
#define RW_ADSPCR                               (ADSP_REG_OFST)
    #define  PROM_WR                            ((UINT32)0x01 << 1)
    #define  PRAM_WR                            ((UINT32)0x01 << 2)
    #define  DROM_WR                            ((UINT32)0x01 << 3)
    #define  HOT_DNLD                           ((UINT32)0x01 << 6)    
    #define  PMEM_AUTO_INC                      ((UINT32)0x01 << 10)    
    #define  DR_RD_EN                           ((UINT32)0x01 << 12)    
// PROM Download Reg (5104h)
#define RW_PROM_DNLD                            (ADSP_REG_OFST + (0x0004 >> 2))
// PRAM Download Reg (5108h)
#define RW_PRAM_DNLD                            (ADSP_REG_OFST + (0x0008 >> 2))
// Counter Control Reg (FPGA only)(510Ch)
#define WO_CNTR_CLR                             (ADSP_REG_OFST + (0x000C >> 2))
    #define  CNTR_CLR                           (0x01 << 1)
// PRAM Addr Reg (5110h)
#define RW_DNLD_CNTR                            (ADSP_REG_OFST + (0x0010 >> 2))
// DROM Download Reg (FPGA only) (5158h)
#define RW_DROM_DNLD                            (ADSP_REG_OFST + (0x0058 >> 2))

//-----------------------------------------------------------------------------
// Register Access Macro
//-----------------------------------------------------------------------------
#define WriteREG(addr,value)        IO_WRITE32(AUDIO_BASE,((addr)<<2),(value))
#define ReadREG(addr)          	    IO_READ32(AUDIO_BASE,((addr)<<2))

//-----------------------------------------------------------------------------
// Memory Access Macro
//-----------------------------------------------------------------------------
//lint -emacro({717}, WriteDspCommDram)  to disable "do {...} while (0)" lint warning
#if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
#define WriteDspCommDram(addr, val)   do{                                                       \
                                      *((UINT32 *)(VIRTUAL(_u4DspBuf+((addr)<<2)))) = ((UINT32)(val)<<8);   \
                                      DSP_FlushInvalidateDCacheSmall(CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX, ((addr)<<2), 1);  \
                                      }while(0)
#define WriteDspCommDram32bit(addr, val)   do{                                                       \
                                      *((UINT32 *)(VIRTUAL(_u4DspBuf+((addr)<<2)))) = ((UINT32)(val));   \
                                      DSP_FlushInvalidateDCacheSmall(CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX, ((addr)<<2), 1);  \
                                      }while(0)

#define WriteDspCommDram2(addr, val)   do{                                                       \
                                      *((UINT32 *)(VIRTUAL(_u4DspBuf+((addr)<<2)))) = ((UINT32)(val)<<8);   \
                                      DSP_FlushInvalidateDCacheSmall2((addr));  \
                                      }while(0)

#define WriteDspCommDram32(addr, val)   do{                                                       \
                                      *((UINT32 *)(VIRTUAL(_u4DspBuf+((addr)<<2)))) = (val);   \
                                      DSP_FlushInvalidateDCacheSmall(CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX, ((addr)<<2), 1);  \
                                      }while(0)
#else
#define WriteDspCommDram(addr, val)   do{                                                       \
                                      *((UINT32 *)(VIRTUAL(_u4DspBuf+((addr)<<2)))) = (val);   \
                                      DSP_FlushInvalidateDCacheSmall(CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX, ((addr)<<2), 1);  \
                                      }while(0)
#define WriteDspCommDram32bit(addr, val)   do{                                                       \
                                      *((UINT32 *)(VIRTUAL(_u4DspBuf+((addr)<<2)))) = (val);   \
                                      DSP_FlushInvalidateDCacheSmall(CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX, ((addr)<<2), 1);  \
                                      }while(0)

#define WriteDspCommDram2(addr, val)   do{                                                       \
                                      *((UINT32 *)(VIRTUAL(_u4DspBuf+((addr)<<2)))) = (val);   \
                                      DSP_FlushInvalidateDCacheSmall2((addr));  \
                                      }while(0)
#endif

#if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
#define dReadDspCommDram(addr)       ((*((UINT32*)(VIRTUAL(_u4DspBuf+((addr)<<2)))))>>8)
#define dReadDspCommDram32(addr)       (*((UINT32*)(VIRTUAL(_u4DspBuf+((addr)<<2)))))
#else
#define dReadDspCommDram(addr)       (*((UINT32*)(VIRTUAL(_u4DspBuf+((addr)<<2)))))
#endif

#define dReadDspCommDramAddr(addr)  (VIRTUAL(_u4DspBuf+((addr)<<2)))

#define dReadDspNormDram(page,addr)     (*((UINT32*)(VIRTUAL(_u4DspDramBuf[(page)]+((addr)<<2)))))
#define dReadDspCmptDram(page,addr)     (*((UINT32*)(VIRTUAL(_u4DspCmptBuf[(page)]+((addr)<<2)))))

#define dReadDspNormDramAddr(page,addr)  (VIRTUAL(_u4DspDramBuf[(page)]+((addr)<<2)))
#define dReadDspCmptDramAddr(page,addr)  (VIRTUAL(_u4DspCmptBuf[(page)]+((addr)<<2)))


#if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
#define WriteDspDecDram(addr, val)   do{                                                            \
                                     *((UINT32 *)(VIRTUAL(_u4DspDramBuf[0]+ ((addr)<<2))) = ((UINT32)(val)<<8);   \
                                    DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_DEC1_DRAM_IDX);                                                 \
                                     }while(0)
#define WriteDspDecDram32(addr, val)   do{                                                            \
                                     *((UINT32 *)(VIRTUAL(_u4DspDramBuf[0]+ ((addr)<<2))) = (val);   \
                                    DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_DEC1_DRAM_IDX);                                                 \
                                     }while(0)
#else
#define WriteDspDecDram(addr, val)   do{                                                            \
                                     *((UINT32 *)(VIRTUAL(_u4DspDramBuf[0]+ ((addr)<<2))) = (val);   \
                                    DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_DEC1_DRAM_IDX);                                                 \
                                     }while(0)
#endif                                     

#define wReadDspWORD(arg1)           (UINT16)(dReadDspCommDram(arg1) >> 8)
#define vWriteDspWORD(arg1,arg2)     WriteDspCommDram(arg1, (UINT32)((arg2) << 8))

// using cache area
#if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
#define WriteDspDecCacheDram(addr, val) do{                                                             \
                                        (*((UINT32 *)VIRTUAL(_u4DspDramBuf[0] + ((addr)<<2)))) =((UINT32)(val)<<8);  \
                                        DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_DEC1_DRAM_IDX);                                               \
                                        }while(0)
#define WriteDspDecCacheDram32(addr, val) do{                                                             \
                                            (*((UINT32 *)VIRTUAL(_u4DspDramBuf[0] + ((addr)<<2)))) = (val);  \
                                            DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_DEC1_DRAM_IDX);                                               \
                                            }while(0)
///CC_VORBIS_SUPPORT
#define WriteDspDec2CacheDram(addr, val) do{                                                             \
                                        (*((UINT32 *)VIRTUAL(_u4DspDramBuf[1] + ((addr)<<2)))) =((UINT32)(val)<<8);  \
                                        DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_DEC2_DRAM_IDX);                                               \
                                        }while(0)
#define WriteDspDec2CacheDram32(addr, val) do{                                                             \
                                            (*((UINT32 *)VIRTUAL(_u4DspDramBuf[1] + ((addr)<<2)))) = (val);  \
                                            DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_DEC2_DRAM_IDX);                                               \
                                            }while(0)
#else
#define WriteDspDecCacheDram(addr, val) do{                                                             \
                                        (*((UINT32 *)VIRTUAL(_u4DspDramBuf[0] + ((addr)<<2)))) = (val);  \
                                        DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_DEC1_DRAM_IDX);                                               \
                                        }while(0)
///CC_VORBIS_SUPPORT
#define WriteDspDec2CacheDram(addr, val) do{                                                             \
                                        (*((UINT32 *)VIRTUAL(_u4DspDramBuf[1] + ((addr)<<2)))) = (val);  \
                                        DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_DEC2_DRAM_IDX);                                               \
                                        }while(0)
#endif                                        

#if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
#define ReadDspDecCacheDram(addr, data) do{                                                             \
                                        DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_DEC1_DRAM_IDX);                                   \
                                        data = ((*((UINT32 *)VIRTUAL(_u4DspDramBuf[0] + ((addr)<<2))))>>8); \
                                        }while(0)

#define ReadDspDec2CacheDram(addr, data) do{                                                             \
                                       DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_DEC2_DRAM_IDX);                                \
                                        data = ((*((UINT32 *)VIRTUAL(_u4DspDramBuf[1] + ((addr)<<2))))>>8); \
                                        }while(0)

#define ReadDspDecCacheDram32(addr, data) do{                                                             \
                                        DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_DEC1_DRAM_IDX);                                   \
                                        data = (*((UINT32 *)VIRTUAL(_u4DspDramBuf[0] + ((addr)<<2)))); \
                                        }while(0)

#define ReadDspDec2CacheDram32(addr, data) do{                                                             \
                                       DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_DEC2_DRAM_IDX);                                \
                                        data = (*((UINT32 *)VIRTUAL(_u4DspDramBuf[1] + ((addr)<<2)))); \
                                        }while(0)
#else
#define ReadDspDecCacheDram(addr, data) do{                                                             \
                                        DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_DEC1_DRAM_IDX);                                   \
                                        data = (*((UINT32 *)VIRTUAL(_u4DspDramBuf[0] + ((addr)<<2)))); \
                                        }while(0)

#define ReadDspDec2CacheDram(addr, data) do{                                                             \
                                       DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_DEC2_DRAM_IDX);                                \
                                        data = (*((UINT32 *)VIRTUAL(_u4DspDramBuf[1] + ((addr)<<2)))); \
                                        }while(0)
#endif

#if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
#define DspDecCacheDramData(addr) ((*((UINT32 *)VIRTUAL(_u4DspDramBuf[0] + ((addr)<<2))))>>8)
#define DspDecCacheDramData32(addr) (*((UINT32 *)VIRTUAL(_u4DspDramBuf[0] + ((addr)<<2))))
///CC_VORBIS_SUPPORT
#define DspDec2CacheDramData(addr) ((*((UINT32 *)VIRTUAL(_u4DspDramBuf[1] + ((addr)<<2))))>>8)
#define DspDec2CacheDramData32(addr) (*((UINT32 *)VIRTUAL(_u4DspDramBuf[1] + ((addr)<<2))))
///FEW_DRAM_FLUSH
#define DspDecCacheDramWriteData(addr, val)   (*((UINT32 *)VIRTUAL(_u4DspDramBuf[0] + ((addr)<<2)))) =((UINT32)(val)<<8)
#define DspDecCacheDramWriteData32(addr, val) (*((UINT32 *)VIRTUAL(_u4DspDramBuf[0] + ((addr)<<2)))) =((UINT32)(val))
#define DspDec2CacheDramWriteData(addr, val)  (*((UINT32 *)VIRTUAL(_u4DspDramBuf[1] + ((addr)<<2)))) =((UINT32)(val)<<8)
#define DspDec2CacheDramWriteData32(addr, val)(*((UINT32 *)VIRTUAL(_u4DspDramBuf[1] + ((addr)<<2)))) =((UINT32)(val))

#else
#define DspDecCacheDramData(addr) (*((UINT32 *)VIRTUAL(_u4DspDramBuf[0] + ((addr)<<2))))
///CC_VORBIS_SUPPORT
#define DspDec2CacheDramData(addr) (*((UINT32 *)VIRTUAL(_u4DspDramBuf[1] + ((addr)<<2))))
#endif

#define uReadDspShmBYTE(addr) (*((UINT8*)(_u4ShareMem+(addr))))
#define u2ReadDspShmWORD(addr) (*((UINT16*)(_u4ShareMem+(addr))))
#define u4ReadDspShmDWRD(addr) (*((UINT32*)(_u4ShareMem+(addr))))

#define vWriteDspShmBYTE(addr,value) (*((UINT8*)(_u4ShareMem + (addr))))=(UINT8)(value);
#define vWriteDspShmWORD(addr,value) (*((UINT16*)(_u4ShareMem + (addr)))) = (UINT16)(value);
#define vWriteDspShmDWRD(addr,value) (*((UINT32*)(_u4ShareMem + (addr)))) = (UINT32)(value);


#endif  // DSP_REG_H

