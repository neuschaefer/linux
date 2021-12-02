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

/* please reference vga_table.h, should fix as 8 */
#define USERMODE_TIMING 8 


#if SUPPORT_VGA_AMBIGUOUS_H_DETECT
const UINT8 bHdtvTimings = 26;
const UINT8 bVgaTimings = 66;
const UINT8 bUserVgaTimings = 8;
const UINT8 bUserVgaTimingBegin= bHdtvTimings+bVgaTimings;
const UINT8 bAllTimings= bHdtvTimings+bVgaTimings+bUserVgaTimings;
#endif

#if SUPPORT_VGA_AMBIGUOUS_USER_SEL
const UINT8 bHdtvTimings = 26;
const UINT8 bVgaTimings = 54;
const UINT8 bUserVgaTimings = 8;
const UINT8 bUserVgaTimingBegin= bHdtvTimings+bVgaTimings;
const UINT8 bAllTimings= bHdtvTimings+bVgaTimings+bUserVgaTimings;
#endif

VGAMODE  VGATIMING_TABLE[bAllTimings] =
{
#if SUPPORT_VGA_AMBIGUOUS_H_DETECT
	//HF	VF	P-CLK	HTOTAL	VTOTAL	H(Bp+W)	H-Act	H-BP	V(BP+w)	V-Act
/*000*/{0,	0,	0,	0,	0,	0,	720,	0,	0,	480,	0x00},  // 00: No Signal
/*001*/{157,	60,	270,	1716,	525,	252,	720,	118,	19,	480,	0x40},  // 01: 480i(1440x480)(6,7)
/*002*/{157,	50,	270,	1728,	625,	264,	720,	140,	23,	576,	0x40},  // 02: 576i(1440x576)(21,22)
/*003*/{315,	60,	270,	858,	525,	126,	720,	52,	36,	480,	0x02},  // 03: 480p(720x480)(2,3)
/*004*/{313,	50,	270,	864,	625,	134,	720,	60,	44,	576,	0x80},  // 04: 576p(720x576) (17,18)
/*005*/{375,	50,	742,	1980,	750,	302,	1280,	270,	25,	720,	0x00},  // 05: 720p50 (19)
/*006*/{450,	60,	742,	1650,	750,	301,	1280,	270,	25,	720,	0x00},  // 06: 720p60 (4)
/*007*/{270,	48,	742,	2750,	1125,	192,	1920,	148,	10,	1080,	0x40},  // 07: 1080i-24
/*008*/{281,	50,	742,	2640,	1125,	237,	1920,	202,	21,	1080,	0x40},  // 08: 1080i-25
/*009*/{338,	60,	742,	2200,	1125,	238,	1920,	202,	20,	1080,	0x40},  // 09: 1080i-30
/*010*/{270,	24,	742,	2750,	1125,	235,	1920,	202,	41,	1080,	0x00},  // 10: 1080p-24 (32)
/*011*/{281,	25,	742,	2640,	1125,	235,	1920,	202,	41,	1080,	0x00},  // 11: 1080p-25 (33)
/*012*/{337,	30,	742,	2200,	1125,	235,	1920,	202,	41,	1080,	0x00},  // 12: 1080p-30 (34)
/*013*/{562,	50,	1485,	2640,	1125,	235,	1920,	205,	41,	1080,	0x00},  // 13: 1080p-50 (31)
/*014*/{675,	60,	1485,	2200,	1125,	235,	1920,	205,	41,	1080,	0x00},  // 14: 1080p-60 (16)
/*015*/{157,	60,	540,	3432,	525,	504,	1440,	221,	19,	480,	0x41},  // 15: 480i oversample(10,11)
/*016*/{157,	50,	540,	3456,	625,	528,	1440,	269,	23,	576,	0x41},  // 16: 576i oversample(25,26)
/*017*/{315,	60,	540,	1716,	525,	252,	1440,	128,	36,	480,	0x01},  // 17: 480p oversample(14,15)
/*018*/{313,	50,	540,	1728,	625,	268,	1440,	144,	44,	576,	0x81},  // 18: 576p oversample(29,30)
/*019*/{0,	0,	252,	800,	525,	144,	640,	48,	35,	480,	0x00},  // 19: 640*480 may need to support(1),hv-ambiguous
/*020*/{0,	0,	270,	1716,	262,	144,	720,	48,	35,	240,	0x00},  // 20: 240p(720x1440) may need to support(8,9*),hv-ambiguous
/*021*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},  // 21: 240p oversample may need to support(12,13*),hv-ambiguous
/*022*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},  // 22: 288p(1440x288) may need to support(23,24**),hv-ambiguous
/*023*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},  // 23: 288p oversample may need to support(27,28**),hv-ambiguous
/*024*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},  // 24: Blank
/*025*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},  // 25: Blank
	//HF	VF	P-CLK	HTOTAL	VTOTAL	H(Bp+W)	H-Act	H-BP	V(BP+w)	V-Act 
/*026*/{244,	60,	312,	1280,	407,	224,	1024,	160,	22,	384,	0x01},  // 26: 512*384*60hz Mac-12M(over sampling)
/*027*/{313,	69,	500,	1600,	449,	288,	1280,	96,	61,	350,	0xB1},  // 27: 640*350*70hz (!+-)
/*028*/{314,	70,	566,	1800,	449,	324,	1440,	108,	62,	350,	0xB1},  // 28: 720*350*70hz text (+ -)??
/*029*/{314,	70,	566,	1800,	449,	324,	1440,	108,	36,	400,	0x89},  // 29: 720*400*70hz  text  (!-+)
/*030*/{379,	85,	630,	1664,	445,	320,	1280,	192,	63,	350,	0x91},  // 30: 640*350*85hz (+-)
/*031*/{379,	85,	630,	1664,	445,	320,	1280,	192,	44,	400,	0x89},  // 31: 640*400 85Hz (!-+)
/*032*/{379,	85,	355,	936,	446,	180,	720,	108,	45,	400,	0x88},  // 32: 720*400 85Hz(-+)
/*033*/{315,	60,	504,	1600,	525,	288,	1280,	96,	35,	480,	0x03},  // 33: 640*480*60hz (!--)
/*034*/{350,	67,	604,	1728,	525,	320, 1280,	192,	42,	480,	0x01},  // 34: 640*480*67Hz Mac_13C
/*035*/{379,	72,	630,	1664, 520,	336,	1280,	240,	31,	480,	0x01},  // 35: 640*480*72hz(!--)
/*036*/{375,	75,	630,	1680,	500,	368,	1280,	240,	19,	480,	0x01},  // 36: 640*480*75hz(!--)
/*037*/{433,	85,	360,	832,	509,	136,	640,	80,	28,	480,	0x00},  // 37: 640*480*85hz(!--)
/*038*/{352,	56,	360,	1024,	625,	200,	800,	128,	24,	600,	0x18},  // 38: 800*600*56hz(!++)
/*039*/{379,	60,	400,	1056,	628,	216,	800,	88,	27,	600,	0x18},  // 39: 800*600*60hz(!++)
/*040*/{480,	72,	500,	1040,	666,	184,	800,	64,	29,	600,	0x18},  // 40: 800*600*72hz(!++)
/*041*/{469,	75,	495,	1056,	625,	240,	800,	160,	24,	600,	0x18},  // 41: 800*600*75hz(!++)
/*042*/{536,	85,	563,	1048,	631,	216,	800,	152,	30,	600,	0x18},  // 42: 800*600*85hz(!++)
/*043*/{497,	74,	572,	1152,	667,	288,	832,	224,	42,	624,	0x00},  // 43: 832*624*74.6hz Mac-16(!--)
/*044*/{298,	60,	630,	2112,	500,	368,	1696,	208,	17,	480,	0x01},  // 44:848*480 60Hz(Chroma)
/*045*/{310,	60,	676,	2176,	517,	448,	1696,	224,	31,	480,	0x19},  // 45: 848*480 60Hz(!++)
/*046*/{484,	60,	650,	1344,	806,	296,	1024,	160,	34,	768,	0x00},  // 46: 1024*768*60hz(--DMTvs1280cvt)
/*047*/{481,	60,	650,	1352,	804,	298,	1024,	202,	33,	768,	0x04},  // 47: 1024*768*60hz(--Astro848 Timing)
/*048*/{484,	60,	816,	1688,	806,	304,	1280,	174,	25,	768,	0x04},  // 48: 1280*768*60hz(--Chroma2229 Timing)
/*049*/{482,	60,	801,	1664,	798,	336,	1280,	200,	26,	768,	0x04},  // 49: 1280*768*60hz(--DMTvs1280cvt) // P-Customers suggest remove
/*050*/{478,	60,	635,	1328,	798,	256,	1024,	152,	27,	768,	0x00},  // 50: 1024*768 60Hz CVT!
/*051*/{478,	60,	795,	1664,	798,	320,	1280,	192,	27,	768,	0x04},  // 51: 1280*768 60Hz CVT!
/*052*/{478,	60,	848,	1776,	798,	344,	1360,	208,	27,	768,	0x04},  // 52: 1360*768*60Hz CVT
/*053*/{477,	60,	855,	1792,	795,	368,	1360,	256,	24,	768,	0x04},  // 53: 1360*768*60Hz VESA
/*054*/{473,	60,	560,	1184,	790,	112,	1024,	80,	19,	768,	0x00},  // 54: 1024*768*60Hz CVT-RB!
/*055*/{474,	60,	683,	1440,	790,	112,	1280,	80,	19,	768,	0x04},  // 55: 1280*768*60Hz CVT-RB!
/*056*/{474,	60,	720,	1520,	790,	112,	1360,	80,	20,	768,	0x04},  // 56: 1360*768*60Hz CVT-RB!
/*057*/{565,	70,	750,	1328,	806,	280,	1024,	144,	34,	768,	0x00},  // 57: 1024*768*70hz (!--)
/*058*/{600,	75,	788,	1312,	800,	272,	1024,	176,	31,	768,	0x18},  // 58: 1024*768*75hz(DMTvs1280cvt)
/*059*/{603,	75,	820,	1360,	805,	272,	1024,	168,	34,	768,	0x0c},  // 59: 1024*768 75Hz CVT
/*060*/{603,	75,	1023,	1696,	805,	336,	1280,	208,	34,	768,	0x0c}, // 60: 1280*768 75Hz CVT
/*061*/{687,	85,	945,	1376,	809,	280,	1024,	176,	38,	768,	0x08},  // 61: 1024*768 85Hz CVT
/*062*/{686,	85,	1175,	1712,	809,	352,	1280,	216,	38,	768,	0x0C},  // 62: 1280*768 85Hz CVT
/*063*/{611,	76,	831,	1360,	803,	280,	1024,	168,	34,	768,	0x00},  // 63: 1024*768 76Hz 
/*064*/{537,	60,	817,	1520,	897,	304,	1152,	184,	30,	864,	0x00},  // 64: 1152*864 60Hz new
/*065*/{638,	70,	945,	1480,	912,	296,	1152,	200,	47,	864,	0x00},  // 65: DMT1170: 1152*864 70Hz
/*066*/{675,	75,	1080,	1600,	900,	384,	1152,	256,	35,	864,	0x18},  // 66: 1152*864*75hz(!++)
/*067*/{686,	75,	1000,	1456,	915,	272,	1152,	144,	42,	870,	0x00},  // 67: Mac-21: 1152*870 75Hz
/*068*/{771,	85,	1215,	1576,	907,	360,	1152,	232,	42,	864,	0x00},  // 68: DMT1185(802BT)
/*069*/{617,	65,	929,	1504,	937,	323,	1152,	195,	35,	900,	0x00},  // 69: SUN1166: 1152*900 65Hz
/*070*/{717,	76,	1055,	1472,	943,	304,	1152,	208,	41,	900,	0x00},  // 70: SUN1176: 1152*900 76Hz
/*071*/{560,	70,	950,	1696,	802,	336,	1280,	208,	31,	768,	0x00},  // 71: 1280*768 70Hz CVT(70 nonstd)
/*072*/{603,	75,	1023,	1696,	805,	336,	1280,	208,	34,	768,	0x00},  // 72: 1280*768 75Hz CVT
/*073*/{600,	60,	1080,	1800,	1000,	424,	1280,	312,	39,	960,	0x18},  // 73: 1280x0960x60HZ(!++)
/*074*/{752,	75,	1300,	1728,	1005,	360,	1280,	224,	42,	960,	0x08},  // 74: 1280*960 85Hz CVT
/*075*/{859,	85,	1485,	1728,	1011,	384,	1280,	224,	50,	960,	0x18},  // 75: 1280x0960x85HZ(++)
/*076*/{639,	60,	1080,	1688,	1066,	360,	1280,	248,	41,	1024,	0x18},// 76: 1280*1024*60hz(++)
/*077*/{800,	75,	1330,	1664,	1066,	392,	1280,	248,	41,	1024,	0x18},  // 77: 1280*1024*75hz(++)
/*078*/{911,	85,	1575,	1728,	1072,	384,	1280,	224,	47,	1024,	0x18},  // 78: 1280*1024*85hz(++)
/*079*/{647,	60,	1010,	1560,	1080,	112,	1400,	80,	27,	1050,	0x10},  // 79: 1400*1050*60hz CVT-RB
/*080*/{647,	60,	1190,	1840,	1080,	112,	1680,	80,	27,	1050,	0x14},  // 80: 1680*1050*60hz CVT-RB
/*081*/{653,	60,	1218,	1864,	1089,	376,	1400,	232,	36,	1050,	0x08},  // 81: 1400*1050*60hz CVT
/*082*/{653,	60,	1463,	2240,	1089,	456,	1680,	280,	36,	1050,	0x0C},  // 82: 1680*1050*60hz CVT
/*083*/{823,	75,	1560,	1896,	1099,	392,	1400,	248,	46,	1050,	0x08},  // 83: 1400*1050*75hz CVT
/*084*/{555,	60,	888,	1600,	926,	112,	1440,	80,	23,	900,	0x10},  // 84: 1440*900*60hz CVT-RB
/*085*/{559,	60,	1065,	1904,	934,	384,	1440,	232,	31,	900,	0x08},  // 85: 1440*900*60hz CVT
/*086*/{706,	75,	1368,	1936,	942,	400,	1440,	248,	39,	900,	0x08},  // 86: 1440*900*75hz CVT
/*087*/{750,	60,	1620,	2160,	1250,	496,	1600,	304,	49,	1200,	0x00},  // 87: 1600*1200*60hz VESA1200-60
/*088*/{666,	60,	1385,	2080,	1111,	112,	1920,	80,	28,	1080,	0x10},  // 88: 1920*1080*60hz CVT-RB
/*089*/{740,	60,	1540,	2080,	1235,	112,	1920,	80,	32,	1200,	0x10},  // 89: 1920*1200*60hz CVT-RB
/*090*/{717,	67,	1170,	1632,	1067,	336,	1280,	224,	41,	1024,	0x10},  // 90: 1280*1024*67hz
/*091*/{670,	60,	1728,	2576,	1118,	536,	1920,	328,	37,	1080,	0x00},  // 91: 1920*1080*60Hz(VSC 1960H)
//HF	VF	P-CLK	HTOTAL	VTOTAL	H(Bp+W)	H-Act	H-BP	V(BP+w)	V-Act  //USER MODE
/*092*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},  // 92: UserMode Entry
/*093*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},  // 93: UserMode Entry
/*094*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},  // 94: UserMode Entry
/*095*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},  // 95: UserMode Entry
/*096*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},  // 96: UserMode Entry
/*097*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},  // 97: UserMode Entry
/*098*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},  // 98: UserMode Entry
/*099*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},  // 99: UserMode Entry
//HF	VF	P-CLK	HTOTAL	VTOTAL	H(Bp+W)	H-Act	H-BP	V(BP+w)	V-Act

#endif

#if SUPPORT_VGA_AMBIGUOUS_USER_SEL
	//HF	VF	P-CLK	HTOTAL	VTOTAL	H(Bp+W)	H-Act	H-BP	V(BP+w)	V-Act
/*000*/{0,	0,	0,	0,	0,	0,	720,	0,	0,	480,	0x00},  // 00: No Signal
/*001*/{157,	60,	270,	1716,	525,	252,	720,	118,	19,	480,	0x40},  // 01: 480i(1440x480)(6,7)
/*002*/{157,	50,	270,	1728,	625,	264,	720,	140,	23,	576,	0x40},  // 02: 576i(1440x576)(21,22)
/*003*/{315,	60,	270,	858,		525,	126,	720,	52,	36,	480,	0x02},  // 03: 480p(720x480)(2,3)
/*004*/{313,	50,	270,	864,		625,	134,	720,	60,	44,	576,	0x80},  // 04: 576p(720x576) (17,18)
/*005*/{375,	50,	742,	1980,	750,	302,	1280,	270,	25,	720,	0x00},  // 05: 720p50 (19)
/*006*/{450,	60,	742,	1650,	750,	301,	1280,	270,	25,	720,	0x00},  // 06: 720p60 (4)
/*007*/{270,	48,	742,	2750,	1125,	192,	1920,	148,	10,	1080,	0x40},  // 07: 1080i-24
/*008*/{281,	50,	742,	2640,	1125,	237,	1920,	202,	21,	1080,	0x40},  // 08: 1080i-25
/*009*/{338,	60,	742,	2200,	1125,	238,	1920,	202,	20,	1080,	0x40},  // 09: 1080i-30
/*010*/{270,	24,	742,	2750,	1125,	235,	1920,	202,	41,	1080,	0x00},  // 10: 1080p-24 (32)
/*011*/{281,	25,	742,	2640,	1125,	235,	1920,	202,	41,	1080,	0x00},  // 11: 1080p-25 (33)
/*012*/{337,	30,	742,	2200,	1125,	235,	1920,	202,	41,	1080,	0x00},  // 12: 1080p-30 (34)
/*013*/{562,	50,	1485,2640,	1125,	235,	1920,	205,	41,	1080,	0x00},  // 13: 1080p-50 (31)
/*014*/{675,	60,	1485,2200,	1125,	235,	1920,	205,	41,	1080,	0x00},  // 14: 1080p-60 (16)
/*015*/{157,	60,	540,	3432,	525,	504,	1440,	221,	19,	480,	0x41},  // 15: 480i oversample(10,11)
/*016*/{157,	50,	540,	3456,	625,	528,	1440,	269,	23,	576,	0x41},  // 16: 576i oversample(25,26)
/*017*/{315,	60,	540,	1716,	525,	252,	1440,	128,	36,	480,	0x01},  // 17: 480p oversample(14,15)
/*018*/{313,	50,	540,	1728,	625,	268,	1440,	144,	44,	576,	0x81},  // 18: 576p oversample(29,30)
/*019*/{0,	0,	252,	800,		525,	144,	640,	48,	35,	480,	0x00},  // 19: 640*480 may need to support(1),hv-ambiguous
/*020*/{0,	0,	270,	1716,	262,	144,	720,	48,	35,	240,	0x00},  // 20: 240p(720x1440) may need to support(8,9*),hv-ambiguous
/*021*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},  // 21: 240p oversample may need to support(12,13*),hv-ambiguous
/*022*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},  // 22: 288p(1440x288) may need to support(23,24**),hv-ambiguous
/*023*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},  // 23: 288p oversample may need to support(27,28**),hv-ambiguous
/*024*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},  // 24: Blank
/*025*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},  // 25: Blank
	//HF	VF	P-CLK	HTOTAL	VTOTAL	H(Bp+W)	H-Act	H-BP	V(BP+w)	V-Act 
/*026*/{244,	60,	312,	1280,	407,		224,	1024,	160,	22,	384,	0x01},  // 26: 512*384*60hz Mac-12M(over sampling)
/*027*/{298,	60,	630,	2112,	500,		368,	1696,	208,	17,	480,	0x01},  // 27:848*480 60Hz(Chroma)
/*028*/{310,	60,	676,	2176,	517,		448,	1696,	224,	31,	480,	0x19},  // 28: 848*480 60Hz(!++)
/*029*/{313,	70,	500,	1600,	449,		288,	1280,	96,	61,	350,	0xB5},  // 29: 640*350*70hz (!+-)			//Ambiguous group 0
/*030*/{314,	70,	566,	1800,	449,		324,	1440,	108,	62,	350,	0xB5},  // 30: 720*350*70hz text (+ -)?? 	//Ambiguous group 0
/*031*/{314,	70,	566,	1800,	449,		324,	1440,	108,	36,	400,	0x8D},  // 31: 720*400*70hz  text  (!-+)		//Ambiguous group 0
/*032*/{315,	60,	504,	1600,	525,		288,	1280,	96,	35,	480,	0x03},  // 32: 640*480*60hz (!--)
/*033*/{350,	67,	604,	1728,	525,		320, 1280,	192,	42,	480,	0x01},  // 33: 640*480*67Hz Mac_13C
/*034*/{352,	56,	360,	1024,	625,		200,	 800,	128,	24,	600,	0x18},  // 34: 800*600*56hz(!++)
/*035*/{375,	75,	630,	1680,	500,		368,	1280,	240,	19,	480,	0x01},  // 35: 640*480*75hz(!--)
/*036*/{379,	60,	400,	1056,	628,		216,	 800,	88,	27,	600,	0x18},  // 36: 800*600*60hz(!++)
/*037*/{379,	72,	630,	1664, 	520,		336,	1280,	240,	31,	480,	0x01},  // 37: 640*480*72hz(!--)
/*038*/{379,	85,	630,	1664,	445,		320,	1280,	192,	63,	350,	0x95},  // 38: 640*350*85hz (+-)			//Ambiguous group 1
/*039*/{379,	85,	630,	1664,	445,		320,	1280,	192,	44,	400,	0x8D},  // 39: 640*400 85Hz (!-+)			//Ambiguous group 1
/*040*/{379,	85,	355,	 936,	446,		180,  720,	108,	45,	400,	0x8C},  // 40: 720*400 85Hz(-+)			//Ambiguous group 1
/*041*/{433,	85,	360,	 832,	509,		136,	 640,	80,	28,	480,	0x00},  // 41: 640*480*85hz(!--)
/*042*/{469,	75,	495,	1056,	625,		240,  800,	160,	24,	600,	0x18},  // 42: 800*600*75hz(!++)
///*046*/{473,	60,	560,	1184,	790,		112,	1024,	80,	19,	768,	0x04},  // 46: 1024*768*60Hz CVT-RB! //Ambiguous group x
///*047*/{474,	60,	683,	1440,	790,		112,	1280,	80,	19,	768,	0x04},  // 47: 1280*768*60Hz CVT-RB! //Ambiguous group x
///*048*/{474,	60,	720,	1520,	790,		112,	1360,	80,	20,	768,	0x04},  // 48: 1360*768*60Hz CVT-RB! //Ambiguous group x
///*049*/{477,	60,	855,	1792,	795,		368,	1360,	256,	24,	768,	0x98},  // 49: 1360*768*60Hz VESA (++)
/*043*/{478,	60,	635,	1328,	798,		256,	1024,	152,	27,	768,	0x04},  // 43: 1024*768 60Hz CVT!			//Ambiguous group 2
/*044*/{478,	60,	795,	1664,	798,		320,	1280,	192,	27,	768,	0x04},  // 44: 1280*768 60Hz CVT!			//Ambiguous group 2
/*045*/{478,	60,	848,	1776,	798,		344,	1360,	208,	27,	768,	0x04},  // 45: 1360*768*60Hz CVT			//Ambiguous group 2
/*046*/{480,	72,	500,	1040,	666,		184,	 800,	64,	29,	600,	0x18},  // 46: 800*600*72hz(!++)
///*059*/{484,	60,	650,	1344,	806,		296,	1024,	160,	34,	768,	0x04},  // 59: 1024*768*60hz(--DMTvs1280cvt)       	//Ambiguous group x
///*060*/{484,	60,	816,	1688,	806,		304,	1280,	174,	25,	768,	0x04},  // 60: 1280*768*60hz(--Chroma2229 Timing)	//Ambiguous group x
/*047*/{497,	75,	572,	1152,	667,		288,  832,	224,	42,	624,	0x00},  // 47: 832*624*74.6hz Mac-16(!--)
/*048*/{536,	85,	563,	1048,	631,		216,	 800,	152,	30,	600,	0x18},  // 48: 800*600*85hz(!++)
/*049*/{537,	60,	817,	1520,	897,		304,	1152,	184,	30,	864,	0x00},  // 49: 1152*864 60Hz new
///*069*/{555,	60,  888,	1600,	926,		112,	1440,	80,	23,	900,	0x10},  // 69: 1440*900*60hz CVT-RB
/*050*/{559,	60, 1065,	1904,	934,		384,	1440,	232,	31,	900,	0x08},  // 50: 1440*900*60hz CVT
/*051*/{560,	70,  950,	1696,	802,		336,	1280,	208,	31,	768,	0x04},  // 51: 1280*768 70Hz CVT(70 nonstd)	//Ambiguous group 3
/*052*/{565,	70,	750,	1328,	806,		280,	1024,	144,	34,	768,	0x04},  // 52: 1024*768*70hz (!--)			//Ambiguous group 3
/*053*/{600,	60, 1080,	1800,	1000,	424,	1280,	312,	39,	960,	0x18},  // 53: 1280x0960x60HZ(!++)
///*053*/{600,	75,	788,	1312,	800,		272,	1024,	176,	31,	768,	0x18},  // 53: 1024*768*75hz(DMTvs1280cvt)
/*054*/{603,	75, 1023,	1696,	805,		336,	1280,	208,	34,	768,	0x04},  // 54: 1280*768 75Hz CVT			//Ambiguous group 4
/*055*/{603,	75,	820,	1360,	805,		272,	1024,	168,	34,	768,	0x0C},  // 55: 1024*768 75Hz CVT			//Ambiguous group 4
///*058*/{603,	75, 1023,	1696,	805,		336,	1280,	208,	34,	768,	0x0C},  // 58: 1280*768 75Hz CVT
///*055*/{611,	76,	831,	1360,	803,		280,	1024,	168,	34,	768,	0x00},  // 55: 1024*768 76Hz 
/*056*/{617,	65,	929,	1504,	937,		323,	1152,	195,	35,	900,	0x00},  // 56: SUN1166: 1152*900 65Hz
/*057*/{638,	70,	945,	1480,	912,		296,	1152,	200,	47,	864,	0x00},  // 57: DMT1170: 1152*864 70Hz
/*058*/{639,	60, 1080,	1688,	1066,	360,	1280,	248,	41, 1024,	0x18},  // 58: 1280*1024*60hz(++)
/*059*/{647,	60, 1010,	1560,	1080,	112,	1400,	80,	27, 1050,	0x14},  // 59: 1400*1050*60hz CVT-RB	//Ambiguous group x
/*060*/{647,	60, 1190,	1840,	1080,	112,	1680,	80,	27, 1050,	0x14},  // 60: 1680*1050*60hz CVT-RB	//Ambiguous group x
/*061*/{000,	00, 0000,	0000,	0000,	000,	0000,	00,	00, 0000,	0x00},  // 61: NULL Items
/*062*/{653,	60, 1218,	1864,	1089,	376,	1400,	232,	36, 1050,	0x0C},  // 62: 1400*1050*60hz CVT			//Ambiguous group 5
/*063*/{653,	60, 1463,	2240,	1089,	456,	1680,	280,	36, 1050,	0x0C},  // 63: 1680*1050*60hz CVT			//Ambiguous group 5
/*064*/{666,	60, 1385,	2080,	1111,	112,	1920,	80,	28, 1080,	0x10},  // 64: 1920*1080*60hz CVT-RB
///*087*/{670,	60, 1728,	2576,	1118,	536,	1920,	328,	37, 1080,	0x00},  // 87: 1920*1080*60Hz(VSC 1960H)
/*065*/{675,	75, 1080,	1600,	900,		384,	1152,	256,	35,	864,	0x18},  // 65: 1152*864*75hz(!++)
/*066*/{686,	75, 1000,	1456,	915,		272,	1152,	144,	42,	870,	0x00},  // 66: Mac-21: 1152*870 75Hz
/*067*/{686,	85, 1175,	1712,	809,		352,	1280,	216,	38,	768,	0x0C},  // 67: 1280*768 85Hz CVT			//Ambiguous group 6
/*068*/{687,	85,	945,	1376,	809,		280,	1024,	176,	38,	768,	0x0C},  // 68: 1024*768 85Hz CVT			//Ambiguous group 6
/*069*/{706,	75, 1368,	1936,	942,		400,	1440,	248,	39,	900,	0x08},  // 69: 1440*900*75hz CVT
/*070*/{717,	67, 1170,	1632,	1067,	336,	1280,	224,	41, 1024,	0x10},  // 70: 1280*1024*67hz
/*071*/{717,	76, 1055,	1472,	943,		304,	1152,	208,	41,	900,	0x00},  // 71: SUN1176: 1152*900 76Hz
/*072*/{740,	60, 1540,	2080,	1235,	112,	1920,	80,	32, 1200,	0x14},  // 72: 1920*1200*60hz CVT-RB		//Ambiguous group 7
/*073*/{750,	60, 1620,	2160,	1250,	496,	1600,	304,	49, 1200,	0x04},  // 73: 1600*1200*60hz VESA1200-60	//Ambiguous group 7
/*074*/{752,	75, 1300,	1728,	1005,	360,	1280,	224,	42,	960,	0x08},  // 74: 1280*960 85Hz CVT
/*075*/{771,	85, 1215,	1576,	907,		360,	1152,	232,	42,	864,	0x00},  // 75: DMT1185(802BT)
/*076*/{800,	75, 1330,	1664,	1066,	392,	1280,	248,	41, 1024,	0x18},  // 76: 1280*1024*75hz(++)
/*077*/{823,	75, 1560,	1896,	1099,	392,	1400,	248,	46, 1050,	0x08},  // 77: 1400*1050*75hz CVT
/*078*/{859,	85, 1485,	1728,	1011,	384,	1280,	224,	50,	960,	0x18},  // 78: 1280x0960x85HZ(++)
/*079*/{911,	85, 1575,	1728,	1072,	384,	1280,	224,	47, 1024,	0x18},  // 79: 1280*1024*85hz(++)
//HF	VF	P-CLK	HTOTAL	VTOTAL	H(Bp+W)	H-Act	H-BP	V(BP+w)	V-Act  //USER MODE
/*080*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},  // 80: UserMode Entry
/*081*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},  // 81: UserMode Entry
/*082*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},  // 82: UserMode Entry
/*083*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},  // 83: UserMode Entry
/*084*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},  // 84: UserMode Entry
/*085*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},  // 85: UserMode Entry
/*086*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},  // 86: UserMode Entry
/*087*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},  // 87(97): UserMode Entry
//HF	VF	P-CLK	HTOTAL	VTOTAL	H(Bp+W)	H-Act	H-BP	V(BP+w)	V-Act
#endif
};


/* VGA options ------------------------------*/
UINT8 bVgaOpt01_MDmute0=10; //for MD , waiting lock mute
UINT8 bVgaOpt01_MDmute1=4; //for MD , waiting lock mute
UINT8 bVgaOpt03_SoGen=1; //for MC , waiting stable mute
UINT8 bVgaOpt04_SearchHDTV=1; //for MD, std timing search


/* temperally modify to test Amtran setting*/
UINT8 bVgaOpt05_SearchNewMode=1; //for MD, new mode & user timing search
UINT8 bVgaOpt06_AutoCenter=1; //for Auto Center, if 0 will support grayscale pattern


/* HDTV options ------------------------------*/
UINT8 bHdtvOpt01_MDmute0=10; //for MD , waiting lock mute
UINT8 bHdtvOpt01_MDmute1=4; //for MD , after lock stable
/* Opt03 leave for sync slicer*/
UINT8 bHdtvOpt04_AutoPhase=0;//for auto phase
UINT8 bHdtvOpt05_CustPos=0; //for save each timing custom position in EEPROM


#endif // _VGA_CUSTOMIZATION_H_
