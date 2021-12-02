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
 * Copyright (c) 2005, MediaTek Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: dmx.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx.h
 *  Demux driver - common definitions
 */


#ifndef DMX_H
#define DMX_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

//#include "dmx_if.h"
#include "x_hal_5381.h"


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

/// Enable PCR recovery
#define  __PCR_RECOVERY__


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//
// SRAM base addresses
//
#define DMX_SECTION_POSNEG		                (DEMUX4_BASE + 0x0)
#define DMX_SECTION_CONTROL		                (DEMUX4_BASE + 0x400)
#define DMX_SECTION_PATTERN		                (DEMUX4_BASE + 0x800)
#define DMX_SECTION_MASK		                (DEMUX4_BASE + 0xC00)
#define DMX_SECTION_CA_EVEN_KEY                 (DEMUX5_BASE)
#define DMX_SECTION_CA_ODD_KEY                  (DEMUX6_BASE)

#define DMX_PATTERN_MATCH_BASE                  (DEMUX2_BASE + 0xA50)
#define DMX_PATTERN_MATCH_BASE2                 (DEMUX2_BASE + 0xA00)
#define DMX_DMEM_CA_KEY_BASE                    (DEMUX2_BASE + 0xD50)
#define DMX_DMEM_CA_IV_BASE                     (DEMUX2_BASE + 0xD70)

#define DMX_PATTERN_MATCH_SEL                   (DEMUX2_BASE + 0xA4C)

//
// Misc
//
#define DMX_INT_QUEUE_DEPTH                     32
#define DMX_ES_FIFO_ALIGNMENT                   64
#define DMX_HEADER_FIFO_ALIGNMENT               32
#define DMX_DMEM_ENTRY_LEN                      20
#define DMX_DMEM_PATTERN_MATCH_INDEX            33
#define DMX_DMEM_PATTERN_MATCH_LEN              19
#define DMX_NULL_PIDX                           0xff
#define DMX_PSI_BUF_TIMES                       2

//
// Interrupt masks
//
#define DMX_INT_ERR_MASK                        0xffff
#define DMX_INT_ERR_DBM                         (1 << 0)
#define DMX_INT_ERR_DESCRAMBLER                 (1 << 1)
#define DMX_INT_ERR_PCR                         (1 << 2)
#define DMX_INT_ERR_RESERVED                    (1 << 3)
#define DMX_INT_ERR_STERRING                    (1 << 4)
#define DMX_INT_ERR_PES_FILTER                  (1 << 5)
#define DMX_INT_ERR_FTuP                        (1 << 6)
#define DMX_INT_ERR_MEM_ACCESS                  (1 << 7)
#define DMX_INT_ERR_PCR2                        (1 << 8)
#define DMX_INT_ERR_PVR                         (1 << 9)
#define DMX_INT_STATUS_MASK                     0xffff0000
#define DMX_INT_STATUS_DBM                      (1 << 16)
#define DMX_INT_STATUS_DESCRAMBLER              (1 << 17)
#define DMX_INT_STATUS_PCR                      (1 << 18)
#define DMX_INT_STATUS_RESERVED                 (1 << 19)
#define DMX_INT_STATUS_STERRING                 (1 << 20)
#define DMX_INT_STATUS_PES_FILTER               (1 << 21)
#define DMX_INT_STATUS_FTuP                     (1 << 22)
#define DMX_INT_STATUS_MEM_ACCESS               (1 << 23)
#define DMX_INT_STATUS_PCR2                     (1 << 24)
#define DMX_INT_STATUS_PVR                      (1 << 25)
#ifdef CC_CPU_DESCRAMBLER
#define DMX_INT_STATUS_CPU_DSCR                 (1 << 27)
#endif
#define DMX_INT_PCR_MASK                        (DMX_INT_ERR_PCR | \
                                                 DMX_INT_ERR_PCR2 | \
                                                 DMX_INT_STATUS_PCR | \
                                                 DMX_INT_STATUS_PCR2)

//
// Demux registers
//
#define DMX_REG_CONTROL                         0       // 0x0
#define DMX_REG_CONFIG1							1       // 0x4
#define DMX_REG_CONFIG2                         2       // 0x8
#define DMX_REG_MEM_CMD                         3       // 0xC
#define DMX_REG_MEM_DATA                        4       // 0x10
#define DMX_REG_REC_CONFIG1                     6       // 0x18
#define DMX_REG_REC_CONFIG2                     7       // 0x1C
#define DMX_REG_FRAMER_CONTROL                  8       // 0x20
#define DMX_REG_FRAMER_ERROR_HANDLE				9       // 0x24
#define DMX_REG_DBM_CONTROL                     10      // 0x28
#define DMX_REG_DBM_BYPASS_PID                  11      // 0x2C

#define DMX_REG_DBM_BYPASS_PID_2                12      // 0x30
#define DMX_REG_DBM_BYPASS_PID_3                13      // 0x34
#define DMX_REG_DBM_BYPASS_PID_4                14      // 0x38
#define DMX_REG_DBM_BUF_CTRL                    15      // 0x3C

#define DMX_REG_PES_CONTRL                      46      // 0xB8
#define DMX_REG_FTuP_CONTROL                    48      // 0xC0
#define DMX_REG_INT_STAUS                       64      // 0x100
#define DMX_REG_INT_MASK                        65      // 0x104
#define DMX_REG_CA_CTRL							16      // 0x40
#define DMX_REG_CA_IN_BUF_THRESHOLD				17      // 0x44
#define DMX_REG_CA_IN_BUF_START					19      // 0x4C
#define DMX_REG_CA_IN_BUF_END					20      // 0x50
#define DMX_REG_CA_IN_BUF_RP					21      // 0x54
#define DMX_REG_CA_IN_BUF_WP					22      // 0x58
#define DMX_REG_CA_OUT_BUF_THRESHOLD			18      // 0x48
#define DMX_REG_CA_OUT_BUF_START				23      // 0x5C
#define DMX_REG_CA_OUT_BUF_END					24      // 0x60
#define DMX_REG_CA_OUT_BUF_RP					25      // 0x64
#define DMX_REG_CA_OUT_BUF_WP					26      // 0x68
#define DMX_REG_DES_IV_LO_EVEN					27      // 0x6C
#define DMX_REG_DES_IV_HI_EVEN  				28      // 0x70
#define DMX_REG_M2_CBC_IV_LO					29      // 0x74
#define DMX_REG_M2_CBC_IV_HI					30      // 0x78
#define DMX_REG_M2_OFB_IV_LO					31      // 0x7C
#define DMX_REG_M2_OFB_IV_HI					32      // 0x80
#define DMX_REG_M2_CFG							33      // 0x84
#define DMX_REG_FTUPCR                          48      // 0xC0
#define DMX_REG_PES_DBM_STEER_CTRL              50      // 0xC8
#define DMX_REG_PID_STRUCT_OFFSET    			53      // 0xD4
#define DMX_REG_M2_SYS_KEY0						54      // 0xD8
#define DMX_REG_DBM_ERROR_STATUS_REG            80      // 0x140
#define DMX_REG_DBM_NONERR_STATUS_REG           81      // 0x144
#define DMX_REG_DESCRAMBLER_ERROR_STATUS_REG    88      // 0x160
#define DMX_REG_DESCRAMBLER_NONERR_STATUS_REG   89      // 0x164
#define DMX_REG_STEER_ERROR_STATUS_REG          96      // 0x180
#define DMX_REG_STEER_NONERR_STATUS_REG         97      // 0x184
#define DMX_REG_PES_ERROR_STATUS_REG            104     // 0x1A0
#define DMX_REG_PES_NONERR_STATUS_REG1          105     // 0x1A4
#define DMX_REG_PES_NONERR_STATUS_REG2          106     // 0x1A8
#define DMX_REG_FTuP_ERROR_STATUS_REG           112     // 0x1C0
#define DMX_REG_FTuP_NONERR_STATUS_REG1         113     // 0x1C4
#define DMX_REG_FTuP_NONERR_STATUS_REG2         114     // 0x1C8
#define DMX_REG_DES_IV_LO_ODD   				118     // 0x1d8
#define DMX_REG_DES_IV_HI_ODD					119     // 0x1dc
#define DMX_REG_AES_CTRL                        120
#define DMX_REG_AES_IV_0_EVEN                   121
#define DMX_REG_AES_IV_1_EVEN                   122
#define DMX_REG_AES_IV_2_EVEN                   123
#define DMX_REG_AES_IV_3_EVEN                   124
#define DMX_REG_AES_IV_0_ODD                    125
#define DMX_REG_AES_IV_1_ODD                    126
#define DMX_REG_AES_IV_2_ODD                    127
#define DMX_REG_AES_IV_3_ODD                    128
#define DMX_REG_PCR_CTRL               			130
#define DMX_REG_PCR_ERR_THRESHOLD               131
#define DMX_REG_PCR_FIR_FILTER_COEFF0           132
#define DMX_REG_PCR_FIR_FILTER_COEFF1           133
#define DMX_REG_PCR_FIR_FILTER_COEFF2           134
#define DMX_REG_PCR_FIR_FILTER_COEFF3           135
#define DMX_REG_PCR_EXTENSION             		136
#define DMX_REG_PCR_BASE                   		137
#define DMX_REG_STC_SNAP_EXTENSION             	138
#define DMX_REG_STC_SNAP_BASE                   139
#define DMX_REG_PCR_ERR_TERM_LOWER             	140
#define DMX_REG_PCR_ERR_TERM_UPPER             	141
#define DMX_REG_STC_EXTENSION             		142
#define DMX_REG_STC_BASE                   		143
#define DMX_REG_PCR_IIR_XN1                   	144
#define DMX_REG_PCR_IIR_YN1                   	145
#define DMX_REG_PCR_IIR_YN2                   	146
#define DMX_REG_PCR_DV_CTRL                   	147
#define DMX_REG_PCR2_EXTENSION                  170
#define DMX_REG_PCR2_BASE                       171
#define DMX_REG_PCR_ERROR_STATUS_REG          	180
#define DMX_REG_PCR_NONERR_STATUS_REG1          181
#define DMX_REG_PCR_NONERR_STATUS_REG2          182
#define DMX_REG_PCR2_ERROR_STATUS_REG          	200
#define DMX_REG_PCR2_NONERR_STATUS_REG1         201
#define DMX_REG_PCR2_NONERR_STATUS_REG2         202
#define DMX_REG_DV_CLOCK_RECOVERY				147
#define DMX_REG_PVR_CONTROL                     210     // 0x348
#define DMX_REG_PVR_PLAY_BUF_THRESHOLD          211     // 0x34C
#define DMX_REG_PVR_RECORD_BUF_THRESHOLD        212     // 0x350
#define DMX_REG_PVR_PLAY_BUF_START              213     // 0x354
#define DMX_REG_PVR_PLAY_BUF_END                214     // 0x358
#define DMX_REG_PVR_PLAY_BUF_RP                 215     // 0x35C
#define DMX_REG_PVR_PLAY_BUF_WP                 216     // 0x360
#define DMX_REG_PVR_RECORD_BUF_START            217     // 0x364
#define DMX_REG_PVR_RECORD_BUF_END              218     // 0x368
#define DMX_REG_PVR_RECORD_BUF_RP               219     // 0x36C
#define DMX_REG_PVR_RECORD_BUF_WP               220     // 0x370
#define DMX_REG_PVR_TIMESTAMP_THRESHOLD         223     // 0x37C
#define DMX_REG_PVR_ERROR_STATUS_REG            230     // 0x398
#define DMX_REG_PVR_NONERR_STATUS_REG           231     // 0x39C
#define DMX_REG_DBM_STATUS_REG_3                260     // 0x410
#define DMX_REG_FRAMER0_STATUS                  269
#define DMX_REG_FRAMER1_STATUS                  270

#define DMX_REG_REG_FILE_ADDR_REG               512     // 0x800
#define DMX_REG_LOCAL_ARBITOR_CTRL1             513     // 0x804
#define DMX_REG_LOCAL_ARBITOR_CTRL2             514     // 0x808
#define DMX_REG_LOCAL_ARBITOR_MONITOR           515     // 0x80C

#define DMX_REG_MDMX_CONTROL                    528     // 0x840
#define DMX_REG_MDMX_PACKET_STATUS              529     // 0x844
#define DMX_REG_MDMX_ERR_STATUS                 530     // 0x848
#define DMX_REG_MDMX_NONERR_STATUS              531     // 0x84C
//#ifdef CC_CPU_DESCRAMBLER
#define DMX_REG_CPU_DSCR_CONTROL                536     // 0x860
#define DMX_REG_CPU_DSCR_IN_BUF_THRESHOLD       537     // 0x864
#define DMX_REG_CPU_DSCR_OUT_BUF_THRESHOLD      538     // 0x868
#define DMX_REG_CPU_DSCR_IN_BUF_START           539     // 0x86C
#define DMX_REG_CPU_DSCR_IN_BUF_END             540     // 0x870
#define DMX_REG_CPU_DSCR_IN_BUF_RP              541     // 0x874
#define DMX_REG_CPU_DSCR_IN_BUF_WP              542     // 0x878
#define DMX_REG_CPU_DSCR_OUT_BUF_START          543     // 0x87C
#define DMX_REG_CPU_DSCR_OUT_BUF_END            544     // 0x880
#define DMX_REG_CPU_DSCR_OUT_BUF_RP             545     // 0x884
#define DMX_REG_CPU_DSCR_OUT_BUF_WP             546     // 0x888
#define DMX_REG_CPU_DSCR_IV_0                   547     // 0x88C
#define DMX_REG_CPU_DSCR_IV_1                   548     // 0x890
#define DMX_REG_CPU_DSCR_IV_2                   549     // 0x894
#define DMX_REG_CPU_DSCR_IV_3                   550     // 0x898
#define DMX_REG_CPU_DSCR_KEY_0                  551     // 0x89C
#define DMX_REG_CPU_DSCR_KEY_1                  552     // 0x8A0
#define DMX_REG_CPU_DSCR_KEY_2                  553     // 0x8A4
#define DMX_REG_CPU_DSCR_KEY_3                  554     // 0x8A8
#define DMX_REG_CPU_DSCR_KEY_4                  555     // 0x8AC
#define DMX_REG_CPU_DSCR_KEY_5                  556     // 0x8B0
#define DMX_REG_CPU_DSCR_KEY_6                  557     // 0x8B4
#define DMX_REG_CPU_DSCR_KEY_7                  558     // 0x8B8

#define DMX_REG_CPU_DSCR_INTR_STATUS            560     // 0x8C0
#define DMX_REG_CPU_DSCR_HASH_0                 561     // 0x8C4
#define DMX_REG_CPU_DSCR_HASH_1                 562     // 0x8C8
#define DMX_REG_CPU_DSCR_HASH_2                 563     // 0x8CC
#define DMX_REG_CPU_DSCR_HASH_3                 564     // 0x8D0
#define DMX_REG_CPU_DSCR_HASH_4                 565     // 0x8D4
#define DMX_REG_CPU_DSCR_HASH_5                 566     // 0x8D8
#define DMX_REG_CPU_DSCR_HASH_6                 567     // 0x8DC
#define DMX_REG_CPU_DSCR_HASH_7                 568     // 0x8E0
#define DMX_REG_CPU_DSCR_STATUS                 569     // 0x8E4
//#endif

#define DMX_FMR0                                (0)
#define DMX_FMR1                                (1)
#define DMX_FMR_DISABLE                         (0 << 0)
#define DMX_FMR_ENABLE                          (1 << 0)
#define DMX_FMR_SERIAL_INPUT                    (0 << 1)
#define DMX_FMR_PARALLEL_INPUT                  (1 << 1)
#define DMX_FMR_INTERNAL_SYNC                   (0 << 2)
#define DMX_FMR_EXTERNAL_SYNC                   (1 << 2)
#define DMX_FMR_NEGATIVE_EDGE                   (0 << 3)
#define DMX_FMR_POSITIVE_EDGE                   (1 << 3)   

//
// Demux message queue attributes
//
#define DMX_MSG_PRIORITY_HIGH			        0
#define DMX_MSG_PRIORITY_NORMAL             	128
#define DMX_MSG_PRIORITY_LOW                    255

#define DDI_TS_INDEX_FOR_NO_FRAMER              3
#define DTCP_DBM_BYPASS_PID_REG                 DMX_REG_DBM_BYPASS_PID_4

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
/// Get byte in a word
#define GET_BYTE(arg, n)                ((UINT8*)&(arg))[n]

///
/// Demux register access commands
///
#define DMXCMD_READ8(offset)			IO_READ8(DEMUX0_BASE, (offset))
#define DMXCMD_READ16(offset)			IO_READ16(DEMUX0_BASE, (offset))
#define DMXCMD_READ32(offset)			IO_READ32(DEMUX0_BASE, ((offset) * 4))
#define DMXCMD_WRITE8(offset, value)	IO_WRITE8(DEMUX0_BASE, (offset), (value))
#define DMXCMD_WRITE16(offset, value)	IO_WRITE16(DEMUX0_BASE, (offset), (value))
#define DMXCMD_WRITE32(offset, value)	IO_WRITE32(DEMUX0_BASE, ((offset) * 4), (value))
#define DMXCMD_REG32(offset)			DMXCMD_READ32(offset)


///
/// PID data structure access commands
///
#define PID_W(word)             ((volatile UINT32*)(DEMUX2_BASE))[(word)]
#define PID_S(pidx)             PID_W(DMX_DMEM_ENTRY_LEN * (pidx))
#define PID_S_W(pidx, word)     PID_W((DMX_DMEM_ENTRY_LEN * (pidx)) + (word))

/// PID index table access command
#define PID_INDEX_TABLE(pidx)   ((volatile UINT32*)(DEMUX5_BASE + 0xc00))[(pidx)]


//-----------------------------------------------------------------------------
// Prototype  of inter-file functions
//-----------------------------------------------------------------------------

#endif  // DMX_H

