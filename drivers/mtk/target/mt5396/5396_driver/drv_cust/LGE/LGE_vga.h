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

/*****************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile: vga_table.c $ 
 *
 * Project:
 * --------
 *   MT8205
 *
 * Description:
 * ------------
 *   VGA/HDTV customization
 *   
 * Author:
 * -------
 *   
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: mtk_vga.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
#ifndef _VGA_CUSTOMIZATION_H_
#define _VGA_CUSTOMIZATION_H_

//#define SUPPORT_VGA_AMBIGUOUS_USER_SEL 0            //0: disable user selection for ambiguous VGA, 1: enable user selection for ambiguous VGA
//change to SUPPORT_VGA_UI_AMBIGUOUS_TIMING_SELECT define in drv_opt
/* please reference vga_table.h, should fix as 8 */

#ifdef CC_UP8032_ATV
VGAMODE code VGATIMING_TABLE[]=
#else
VGAMODE  VGATIMING_TABLE[]=
#endif
{
//HF	VF	P-CLK	HTOTAL	VTOTAL	H-W 	H-Act	H-BP	V(BP+w)	V-Act
/*000*/{0,	0,	0,	0,	0,	0,	720,	0,	0,	480,	0x00},	// 000: No Signal
/*001*/{157,	60,	540,	3432,	525,	283,	2880,	229,	16,	480,	0x41},	// 001: 480i oversample(10,11)
/*002*/{157,	50,	540,	3456,	625,	259,	2880,	277,	20,	576,	0x41},	// 002: 576i oversample(25,26)
/*003*/{315,	60,	540,	1716,	525,	120,	1440,	123,	34,	480,	0x03},	// 003: 480p oversample(14,15)
/*004*/{313,	50,	540,	1728,	625,	124,	1440,	139,	42,	576,	0x01},	// 004: 576p oversample(29,30)
/*005*/{375,	50,	742,	1980,	750,	32,	1280,	262,	23,	720,	0x00},	// 005: 720p50 (19)
/*006*/{450,	60,	742,	1650,	750,	31,	1280,	262,	23,	720,	0x00},	// 006: 720p60 (4)
/*007*/{270,	48,	742,	2750,	1125,	44,	1920,	193,	18,	1080,	0x40},	// 007: 1080i-24
/*008*/{281,	50,	742,	2640,	1125,	35,	1920,	194,	18,	1080,	0x40},	// 008: 1080i-25
/*009*/{338,	60,	742,	2200,	1125,	36,	1920,	194,	18,	1080,	0x40},	// 009: 1080i-30
/*010*/{270,	24,	742,	2750,	1125,	33,	1920,	194,	39,	1080,	0x00},	// 010: 1080p-24 (32)
/*011*/{281,	25,	742,	2640,	1125,	33,	1920,	194,	39,	1080,	0x00},	// 011: 1080p-25 (33)
/*012*/{337,	30,	742,	2200,	1125,	33,	1920,	194,	39,	1080,	0x00},	// 012: 1080p-30 (34)
/*013*/{562,	50,	1485,	2640,	1125,	30,	1920,	193,	39,	1080,	0x00},	// 013: 1080p-50 (31)
/*014*/{675,	60,	1485,	2200,	1125,	30,	1920,	193,	39,	1080,	0x00},	// 014: 1080p-60 (16)
/*015*/{157,	60,	270,	1716,	525,	134,	1440,	114,	16,	480,	0x41},	// 015: 480i(1440x480)(6,7)
/*016*/{157,	50,	270,	1728,	625,	124,	1440,	138,	20,	576,	0x41},	// 016: 576i(1440x576)(21,22)
/*017*/{315,	60,	270,	858,	525,	60,	720,	61,	36,	480,	0x00},	// 017: 480p(720x480)(2,3)
/*018*/{313,	50,	270,	864,	625,	74,	720,	69,	42,	576,	0x00},	// 018: 576p(720x576) (17,18)
/*019*/{180,	24,	743,	4125,	750,	40,	1280,	262,	23,	720,	0x00},	// 019: 720p24
/*020*/{188,	25,	743,	3960,	750,	40,	1280,	262,	23,	720,	0x00},	// 020: 720p25
/*021*/{225,	30,	743,	3300,	750,	40,	1280,	261,	23,	720,	0x00},	// 021: 720p30
/*022*/{0,	0,	540,	3432,	262,	283,	2880,	229,	16,	240,	0x01},	// 022: 240p(720x1440)
/*023*/{0,	0,	742,	2200,	563,	35,	1920,	201,	19,	540,	0x00},	// 023: 540p (33.7, 60)
/*024*/{0,	0,	540,	3456,	312,	259,	2880,	277,	20,	288,	0x01},	// 024: 288p(1440x576)
/*025*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},	// 025: 480P_24 (for DTV)
/*026*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},	// 026: 480P_30 (for DTV)
/*027*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},	// 027: 576P_25 (for DTV)
/*028*/{0,	0,	252,	800,	525,	96,	640,	48,	35,	480,	0x00},	// 028: for HDMI 640x480
/*029*/{180,	24,	594,	3300,	750,	40,	1280,	262,	23,	720,	0x00},	// 029: 720p24(HDMI use)
/*030*/{750,	50,	1485,	1980,	1500,	40,	1280,	220,	30,	1470,	0x700},	// 030: for HDMI 3D 720p 50
/*031*/{900,	60,	1485,	1650,	1500,	40,	1280,	220,	30,	1470,	0x700},	// 031: for HDMI 3D 720p 60
/*032*/{540,	24,	1485,	2750,	2250,	44,	1920,	148,	45,	2205,	0x700},	// 032: for HDMI 3D 1080p 24
/*033*/{675,	30,	1485,	2200,	2250,	44,	1920,	148,	22,	2228,	0x700},	// 033: for HDMI 3D 1080i 60, frame packing.
/*034*/{629,	60,	540,	858,	1050,	40,	720,	220,	45,	1005,	0x700},	// 034: for HDMI 3D 480p FP
/*035*/{625,	50,	540,	864,	1250,	40,	720,	220,	49,	1201,	0x700},	// 035: for HDMI 3D 576p FP
/*036*/{359,	24,	1186,	3300,	1500,	40,	1280,	220,	30,	1470,	0x700},	// 036: for HDMI 3D 720p 24 FP
/*037*/{449,	30,	1486,	3300,	1500,	40,	1280,	220,	30,	1470,	0x700},	// 037: for HDMI 3D 720p 30 FP
/*038*/{673,	30,	1482,	2200,	2250,	44,	1920,	148,	45,	2205,	0x700},	// 038: for HDMI 3D 1080p 30 FP
/*039*/{314,	30,	539,	1716,	1050,	283,	1440,	229,	22,	1028,	0x700},	// 039:  for HDMI 3D 480i 60, frame packing.
/*040*/{312,	25,	539,	1728,	1250,	277,	1440,	277,	24,	1226,	0x700},	// 040:for HDMI 3D 576i 50, frame packing.
/*041*/{563,	25,	1485,	2640,	2250,	44,	1920,	148,	22,	2228,	0x700},	// 041: for HDMI 3D 1080i 50, frame packing.
/*042*/{1125,	50,	2970,	2640,	2250,	44,	1920,	148,	45,	2205,	0x700},	// 042: for HDMI 3D 1080p 50 FP
/*043*/{1350,	60,	2970,	2200,	2250,	44,	1920,	148,	45,	2205,	0x700},	// 043: for HDMI 3D 1080p 60 FP
/*044*/{450,	60,	742,	1650,	750,	40,	1280,	220,	15,	735,	0x700},	// 044: 1650x 750x60 , Blank = 15, frame packing
/*045*/{450,	30,	742,	1650,	1500,	40,	1280,	220,	15,	1485,	0x700},	// 045: 1650x1500x30 , Blank = 15, frame packing
/*046*/{270,	24,	1485,	5500,	1125,	66,	3840,	388,	39,	1080,	0x700},	// 046: for HDMI 3D 1080p 24 side-by-side full
/*047*/{337,	30,	1485,	4400,	1125,	66,	3840,	388,	39,	1080,	0x700},	// 047: for HDMI 3D 1080p 30 side-by-side full
/*048*/{281,	50,	1486,	5280,	1125,	70,	3840,	388,	18,	1080,	0x740},	// 048: for HDMI 3D 1080i 50 side-by-side full
/*049*/{337,	60,	1486,	4400,	1125,	72,	3840,	388,	18,	1080,	0x740},	// 049: for HDMI 3D 1080i 60 side-by-side full
/*050*/{180,	24,	1186,	6600,	750,	80,	2560,	524,	23,	720,	0x700},	// 050: for HDMI 3D 720p 24 side-by-side full
/*051*/{225,	30,	1486,	6600,	750,	80,	2560,	522,	23,	720,	0x700},	// 051: for HDMI 3D 720p 30 side-by-side full
/*052*/{375,	50,	1486,	3960,	750,	64,	2560,	524,	23,	720,	0x700},	// 052: for HDMI 3D 720p 50 side-by-side full
/*053*/{450,	60,	1485,	3300,	750,	62,	2560,	524,	23,	720,	0x700},	// 053: for HDMI 3D 720p 60 side-by-side full
/*054*/{315,	60,	540,	1716,	525,	120,	1440,	122,	36,	480,	0x700},	// 054: for HDMI 3D 480p 60 side-by-side full
/*055*/{312,	50,	540,	1728,	625,	148,	1440,	138,	42,	576,	0x700},	// 055: for HDMI 3D 576p 50 side-by-side full
/*056*/{157,	60,	540,	3432,	525,	268,	2880,	228,	16,	480,	0x740},	// 056: for HDMI 3D 480i 60 side-by-side full
/*057*/{157,	50,	540,	3456,	625,	248,	2880,	276,	20,	576,	0x740},	// 057: for HDMI 3D 576i 50 side-by-side full
/*058*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},	// 058: 3D reserved;
	//HF	VF	P-CLK	HTOTAL	VTOTAL	H-W 	H-Act	H-BP	V(BP+w)	V-Act 
/*059*/{0,	0,	312,	1280,	407,	64,	1024,	160,	22,	384,	0x01},	// 059: 512*384*60hz Mac-12M(over sampling)
/*060*/{314,	70,	283,	900,	449,	108,	720,	54,	36,	400,	0x88},	// 060: 720*400*70hz  text  (!-+)
/*061*/{313,	70,	250,	800,	449,	96,	640,	48,	61,	350,	0x90},	// 061: 640*350*70hz (!+-)
/*062*/{0,	0,	283,	900,	449,	108,	720,	54,	62,	350,	0x90},	// 062: 720*350*70hz text (+ -)??
/*063*/{379,	85,	315,	832,	445,	64,	640,	96,	63,	350,	0x90},	// 063: 640*350*85hz (+-) DMT1.2
/*064*/{379,	85,	315,	832,	445,	64,	640,	96,	44,	400,	0x88},	// 064: 640*400 85Hz (!-+) DMT1.2
/*065*/{379,	85,	355,	936,	446,	72,	720,	108,	45,	400,	0x88},	// 065: 720*400 85Hz(-+) DMT1.2
/*066*/{315,	60,	252,	800,	525,	96,	640,	48,	35,	480,	0x02},	// 066: 640*480*60hz (!--)  DMT1.2
/*067*/{297,	60,	238,	800,	500,	64,	640,	80,	17,	480,	0x00},	// 067: 640*480*60hz CVT   
/*068*/{294,	60,	235,	800,	494,	32,	640,	80,	11,	480,	0x00},	// 068: 640*480*60hz CVT-RB
/*069*/{350,	67,	302,	864,	525,	64,	640,	96,	42,	480,	0x00},	// 069: 640*480*67Hz Mac_13C
/*070*/{379,	72,	315,	832,	520,	48,	640,	120,	31,	480,	0x00},	// 070: 640*480*72hz(!--)  DMT1.2
/*071*/{375,	75,	315,	840,	500,	64,	640,	120,	19,	480,	0x00},	// 071: 640*480*75hz(!--)   DMT1.2
/*072*/{433,	85,	360,	832,	509,	56,	640,	80,	28,	480,	0x00},	// 072: 640*480*85hz(!--)   DMT1.2
/*073*/{352,	60,	355,	896,	587,	96,	720,	25,	35,	480,	0x00},	// 073: 720*480*60hz for A1 spec 43
/*074*/{352,	56,	360,	1024,	625,	72,	800,	128,	24,	600,	0x98},	// 074: 800*600*56hz(!++)  DMT1.2
/*075*/{352,	56,	360,	1024,	625,	184,	800,	32,	24,	600,	0x80},	// 075: 800*600*56hz(!--) x
/*076*/{379,	60,	400,	1056,	628,	128,	800,	88,	27,	600,	0x18},	// 076: 800*600*60hz(!++) DMT1.2
/*077*/{374,	60,	383,	1024,	624,	80,	800,	112,	21,	600,	0x00},	// 077: 800*600*60hz CVT
/*078*/{370,	60,	355,	960,	618,	32,	800,	80,	15,	600,	0x00},	// 078: 800*600*60hz CVT-RB
/*079*/{480,	72,	500,	1040,	666,	120,	800,	64,	29,	600,	0x18},	// 079: 800*600*72hz(!++) DMT1.2
/*080*/{469,	75,	495,	1056,	625,	80,	800,	160,	24,	600,	0x18},	// 080: 800*600*75hz(!++) DMT1.2
/*081*/{536,	85,	563,	1048,	631,	64,	800,	152,	30,	600,	0x18},	// 081: 800*600*85hz(!++) DMT1.2
/*082*/{497,	75,	572,	1152,	667,	64,	832,	224,	42,	624,	0x00},	// 082: 832*624*74.6hz Mac-16(!--)
/*083*/{298,	60,	315,	1056,	500,	80,	848,	104,	17,	480,	0x00},	// 083:848*480 60Hz(Chroma)
/*084*/{310,	60,	338,	1088,	517,	112,	848,	112,	31,	480,	0x18},	// 084: 848*480 60Hz(!++) DMT1.2
/*085*/{295,	60,	298,	1008,	494,	32,	848,	80,	11,	480,	0x00},	// 085: 848*480 60Hz CVT-RB
/*086*/{302,	60,	336,	1112,	504,	104,	856,	104,	23,	480,	0x00},	// 086: 856*480 60Hz
/*087*/{484,	60,	650,	1344,	806,	136,	1024,	160,	34,	768,	0x04},	// 087: 1024*768*60hz(--DMTvs1280cvt) DMT1.2  --start
/*088*/{484,	60,	813,	1680,	806,	170,	1280,	200,	34,	768,	0x04},	// 088: 1280*768*60hz from 1024*768*60hz
/*089*/{484,	60,	863,	1785,	806,	181,	1360,	213,	34,	768,	0x04},	// 089: 1360*768*60hz from 1024*768*60hz	--end
/*090*/{0,	0,	650,	1352,	804,	96,	1024,	202,	33,	768,	0x00},	// 090: 1024*768*60hz(--Astro848 Timing)
/*091*/{0,	0,	816,	1688,	806,	130,	1280,	174,	25,	768,	0x00},	// 091: 1280*768*60hz(--Chroma2229 Timing)
/*092*/{0,	0,	801,	1664,	798,	136,	1280,	200,	26,	768,	0x00},	// 092: 1280*768*60hz(--DMTvs1280cvt) // P-Customers suggest remove
/*093*/{478,	60,	635,	1328,	798,	104,	1024,	152,	27,	768,	0x04},	// 093: 1024*768 60Hz CVT! --start
/*094*/{478,	60,	795,	1664,	798,	128,	1280,	192,	27,	768,	0x04},	// 094: 1280*768 60Hz CVT! (-+) DMT1.2
/*095*/{478,	60,	848,	1776,	798,	136,	1360,	208,	27,	768,	0x04},	// 095: 1360*768*60Hz CVT  --end
/*096*/{477,	60,	855,	1792,	795,	112,	1360,	256,	24,	768,	0x00},	// 096: 1360*768*60Hz DMT1.2
/*097*/{473,	60,	560,	1184,	790,	32,	1024,	80,	19,	768,	0x24},	// 097: 1024*768*60Hz CVT-RB! --start
/*098*/{474,	60,	683,	1440,	790,	32,	1280,	80,	19,	768,	0x24},	// 098: 1280*768*60Hz CVT-RB! DMT1.2
/*099*/{474,	60,	720,	1520,	790,	32,	1360,	80,	20,	768,	0x24},	// 099: 1360*768*60Hz CVT-RB! --end
/*100*/{539,	66,	716,	1328,	816,	136,	1024,	144,	34,	768,	0x00},	// 100: 1024*768*66hz for A1 spec 61
/*101*/{565,	70,	750,	1328,	806,	136,	1024,	144,	34,	768,	0x00},	// 101: 1024*768*70hz (!--) DMT1.2
/*102*/{600,	75,	788,	1312,	800,	96,	1024,	176,	31,	768,	0x18},	// 102: 1024*768*75hz(++) DMT1.2
/*103*/{603,	75,	1023,	1702,	802,	96,	1280,	176,	31,	768,	0x00},	// 103: 1280*768*75hz for A1 spec 89/90
/*104*/{603,	75,	820,	1360,	805,	104,	1024,	168,	34,	768,	0x28},	// 104: 1024*768 75Hz CVT --start
/*105*/{603,	75,	1023,	1696,	805,	128,	1280,	208,	34,	768,	0x28},	// 105: 1280*768 75Hz CVT (-+) DMT1.2	 --end
/*106*/{687,	85,	945,	1376,	809,	104,	1024,	176,	38,	768,	0x28},	// 106: 1024*768 85Hz CVT --start
/*107*/{686,	85,	1175,	1712,	809,	136,	1280,	216,	38,	768,	0x28},	// 107: 1280*768 85Hz CVT (-+) DMT1.2 --end
/*108*/{611,	76,	831,	1360,	803,	112,	1024,	168,	34,	768,	0x00},	// 108: 1024*768 76Hz 
/*109*/{537,	60,	817,	1520,	897,	120,	1152,	184,	30,	864,	0x00},	// 109: 1152*864 60Hz new
/*110*/{638,	70,	945,	1480,	912,	96,	1152,	200,	47,	864,	0x00},	// 110: DMT1170 1152*864 70Hz
/*111*/{675,	75,	1080,	1600,	900,	128,	1152,	256,	35,	864,	0x18},	// 111: 1152*864*75hz(!++) DMT1.2
/*112*/{686,	75,	1000,	1456,	915,	128,	1152,	144,	42,	870,	0x00},	// 112: Mac-21 1152*870 75Hz
/*113*/{771,	85,	1215,	1576,	907,	128,	1152,	232,	42,	864,	0x00},	// 113: DMT1185(802BT)
/*114*/{617,	65,	929,	1504,	937,	128,	1152,	195,	35,	900,	0x00},	// 114: SUN1166 1152*900 65Hz
/*115*/{717,	76,	1055,	1472,	943,	96,	1152,	208,	41,	900,	0x00},	// 115: SUN1176 1152*900 76Hz
/*116*/{450,	60,	742,	1650,	750,	31,	1280,	270,	25,	720,	0x00},	// 116: 720P(++) DMT1.2
/*117*/{560,	70,	950,	1696,	802,	128,	1280,	208,	31,	768,	0x00},	// 117: 1280*768 70Hz CVT(70 nonstd)
/*118*/{603,	75,	1023,	1696,	805,	128,	1280,	208,	34,	768,	0x00},	// 118: 1280*768 75Hz CVT
/*119*/{493,	60,	710,	1440,	823,	32,	1280,	112,	20,	800,	0x00},	// 119: 1280*800 60Hz CVR for A1]
/*120*/{600,	60,	1080,	1800,	1000,	112,	1280,	312,	39,	960,	0x18},	// 120: 1280x0960x60HZ(!++)  DMT1.2
/*121*/{752,	75,	1300,	1728,	1005,	136,	1280,	224,	42,	960,	0x08},	// 121: 1280*960 75Hz CVT
/*122*/{859,	85,	1485,	1728,	1011,	160,	1280,	224,	50,	960,	0x18},	// 122: 1280x0960x85HZ(++) DMT1.2
/*123*/{639,	60,	1080,	1688,	1066,	112,	1280,	248,	41,	1024,	0x18},	// 123: 1280*1024*60hz(++) DMT1.2
/*124*/{781,	72,	1350,	1728,	1085,	112,	1280,	248,	41,	1024,	0x00},	// 124: 1280*1024*72 for A1 spec
/*125*/{800,	75,	1330,	1664,	1066,	144,	1280,	248,	41,	1024,	0x18},	// 125: 1280*1024*75hz(++) DMT1.2
/*126*/{911,	85,	1575,	1728,	1072,	160,	1280,	224,	47,	1024,	0x18},	// 126: 1280*1024*85hz(++) DMT1.2
/*127*/{647,	60,	1010,	1560,	1080,	32,	1400,	80,	27,	1050,	0x34},	// 127: 1400*1050*60hz CVT-RB(+-) DMT1.2--start
/*128*/{647,	60,	1190,	1840,	1080,	32,	1680,	80,	27,	1050,	0x34},	// 128: 1680*1050*60hz CVT-RB (+-) DMT1.2--end 
/*129*/{653,	60,	1218,	1864,	1089,	144,	1400,	232,	36,	1050,	0x28},	// 129: 1400*1050*60hz CVT (-+) DMT1.2--start 
/*130*/{653,	60,	1463,	2240,	1089,	176,	1680,	280,	36,	1050,	0x28},	// 130: 1680*1050*60hz CVT (-+) DMT1.2--end
/*131*/{823,	75,	1560,	1896,	1099,	144,	1400,	248,	46,	1050,	0x08},	// 131: 1400*1050*75hz CVT (-+) DMT1.2
/*132*/{555,	60,	888,	1600,	926,	32,	1440,	80,	23,	900,	0x10},	// 132: 1440*900*60hz CVT-RB (+-) DMT1.2
/*133*/{559,	60,	1065,	1904,	934,	152,	1440,	232,	31,	900,	0x08},	// 133: 1440*900*60hz CVT (-+) DMT1.2
/*134*/{706,	75,	1368,	1936,	942,	152,	1440,	248,	39,	900,	0x08},	// 134: 1440*900*75hz CVT (-+) DMT1.2
/*135*/{750,	60,	1620,	2160,	1250,	192,	1600,	304,	49,	1200,	0x00},	// 135: 1600*1200*60hz (++) DMT1.2
/*136*/{740,	60,	1310,	1760,	1225,	32,	1600,	112,	32,	1200,	0x00},	// 136: 1600*1200*60hz CVR for A1 88
/*137*/{621,	60,	1123,	1808,	1040,	29,	1600,	123,	49,	1024,	0x00},	// 137: 1600*1024*60hz for Apple monitor
/*138*/{666,	60,	1385,	2080,	1111,	32,	1920,	80,	28,	1080,	0x10},	// 138: 1920*1080*60hz CVT-RB (-+) DMT1.2
/*139*/{740,	60,	1540,	2080,	1235,	32,	1920,	80,	32,	1200,	0x10},	// 139: 1920*1200*60hz CVT-RB
/*140*/{727,	60,	1560,	2147,	1211,	32,	1920,	80,	32,	1200,	0x10},	// 140: 1920*1200*60hz for A1 spec 128
/*141*/{717,	67,	1170,	1632,	1067,	112,	1280,	224,	41,	1024,	0x10},	// 141: 1280*1024*67hz
/*142*/{670,	60,	1728,	2576,	1118,	208,	1920,	328,	37,	1080,	0x00},	// 142: 1920*1080*60Hz(VSC 1960H)
/*143*/{673,	60,	1557,	2320,	1120,	208,	1728,	328,	37,	1080,	0x00},	// 143: 1728*1080*60Hz for A1, 125
/*144*/{675,	60,	1485,	2200,	1125,	30,	1920,	205,	41,	1080,	0x00},	// 144: 1080p-60 (++) DMT1.2
/*145*/{313,	50,	270,	864,	625,	74,	720,	69,	42,	576,	0x00},	// 145: 576p(720x576) 
/*146*/{396,	50,	653,	1648,	793,	312,	1280,	184,	22,	768,	0x00},	// 146:1280*768@50

//HF	VF	P-CLK	HTOTAL	VTOTAL	H(Bp+W) H-Act	H-BP	V(BP+w) V-Act  //USER MODE
/*147*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},	// 147: UserMode Entry
/*148*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},	// 148: UserMode Entry
/*149*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},	// 149: UserMode Entry
/*150*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},	// 150: UserMode Entry
/*151*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},	// 151: UserMode Entry
/*152*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},	// 152: UserMode Entry
/*153*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},	// 153: UserMode Entry
/*154*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},	// 154: UserMode Entry


};

//don't change the vga timing number when you add  vga timing , only hdtv/user timing number is needed to be changed.
#define USERMODE_TIMING 8 
#define HDTV_TIMING_NUM	58
#define ALL_TIMING_NUM (sizeof(VGATIMING_TABLE)/sizeof(VGAMODE))

const UINT8 bHdtvTimings = HDTV_TIMING_NUM;
const UINT8 bUserVgaTimings = USERMODE_TIMING;
const UINT8 bAllTimings= (sizeof(VGATIMING_TABLE)/sizeof(VGAMODE));
const UINT8 bVgaTimings = (ALL_TIMING_NUM - HDTV_TIMING_NUM -USERMODE_TIMING); 
const UINT8 bUserVgaTimingBegin= (ALL_TIMING_NUM - USERMODE_TIMING);

/* VGA options ------------------------------*/
UINT8 bVgaOpt01_MDmute0=10; //for MD , waiting lock mute
UINT8 bVgaOpt01_MDmute1=4; //for MD , waiting lock mute
UINT8 bVgaOpt03_SoGen=1; //for MC , waiting stable mute
UINT8 bVgaOpt04_SearchHDTV=0; //for MD, std timing search

/* temperally modify to test Amtran setting*/
UINT8 bVgaOpt05_SearchNewMode=1; //for MD, new mode & user timing search
UINT8 bVgaOpt06_SogMaxVsyncWidth=13 ;
UINT8 bVgaOpt07_AutoKeepOldVal= 1 ;

/* HDTV options ------------------------------*/
UINT8 bHdtvOpt01_MDmute0=10; //for MD , waiting lock mute
UINT8 bHdtvOpt01_MDmute1=4; //for MD , after lock stable
/* Opt03 leave for sync slicer*/
UINT8 bHdtvOpt04_AutoPhase=0;//for auto phase
//UINT8 bHdtvOpt05_CustPos=0; //for save each timing custom position in EEPROM
UINT8 bHdtvOpt05_AdaptiveSlicer=1; //Enable adaptive slicer

#endif // _VGA_CUSTOMIZATION_H_
