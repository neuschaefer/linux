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

#define SUPPORT_VGA_AMBIGUOUS_USER_SEL 0            //0: disable user selection for ambiguous VGA, 1: enable user selection for ambiguous VGA

/* please reference vga_table.h, should fix as 8 */


#ifdef CC_UP8032_ATV
VGAMODE code VGATIMING_TABLE[]=
#else
VGAMODE  VGATIMING_TABLE[]=
#endif
{
#if !SUPPORT_VGA_AMBIGUOUS_USER_SEL
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
/*019*/{180,	24,	743,	3300,	750,	40,	1280,	262,	23,	720,	0x00},	// 019: 720p24
/*020*/{188,	25,	743,	3960,	750,	40,	1280,	262,	23,	720,	0x00},	// 020: 720p25
/*021*/{225,	30,	743,	3300,	750,	40,	1280,	261,	23,	720,	0x00},	// 021: 720p30
/*022*/{0,	0,	540,	3432,	262,	283,	2880,	229,	16,	240,	0x01},  // 022: 240p(720x1440) may need to support(8,9*),hv-ambiguous
/*023*/{0,	0,	742,	2200,	563,	35,	1920,	201,	19,	540,	0x00},	// 023: 540p (33.7, 60)
/*024*/{0,	0,	540,	3456,	312,	259,	2880,	277,	20,	288,	0x01}, // 024: 288p(1440x576)
/*025*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},	// 025: 480P_24 (for DTV)
/*026*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},	// 026: 480P_30 (for DTV)
/*027*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},	// 027: 576P_25 (for DTV)
/*028*/{0,	0,	252,	800,	525,	96,	640,	48,	35,	480,	0x00},	// 028: for HDMI
	//HF	VF	P-CLK	HTOTAL	VTOTAL	H-W 	H-Act	H-BP	V(BP+w)	V-Act 
/*029*/{0,	0,	312,	1280,	407,	64,	1024,	160,	22,	384,	0x01},	// 029: 512*384*60hz Mac-12M(over sampling)
/*030*/{314,	70,	283,	900,	449,	108,	720,	54,	36,	400,	0x88},	// 030: 720*400*70hz  text  (!-+)
/*031*/{313,	70,	250,	800,	449,	96,	640,	48,	61,	350,	0x90},	// 031: 640*350*70hz (!+-)
/*032*/{314,	70,	283,	900,	449,	108,	720,	54,	62,	350,	0x90},	// 032: 720*350*70hz text (+ -)??
/*033*/{379,	85,	315,	832,	445,	64,	640,	96,	63,	350,	0x90},	// 033: 640*350*85hz (+-)
/*034*/{379,	85,	315,	832,	445,	64,	640,	96,	44,	400,	0x88},	// 034: 640*400 85Hz (!-+)
/*035*/{379,	85,	355,	936,	446,	72,	720,	108,	45,	400,	0x88},	// 035: 720*400 85Hz(-+)
/*036*/{315,	60,	252,	800,	525,	96,	640,	48,	35,	480,	0x02},	// 036: 640*480*60hz (!--)
/*037*/{297,	60,	238,	800,	500,	64,	640,	80,	17,	480,	0x00},	// 037: 640*480*60hz CVT   
/*038*/{294,	60,	235,	800,	494,	32,	640,	80,	11,	480,	0x00},	// 038: 640*480*60hz CVT-RB
/*039*/{350,	67,	302,	864,	525,	64,	640,	96,	42,	480,	0x00},	// 039: 640*480*67Hz Mac_13C
/*040*/{379,	72,	315,	832,	520,	48,	640,	120,	31,	480,	0x00},	// 040: 640*480*72hz(!--)
/*041*/{375,	75,	315,	840,	500,	64,	640,	120,	19,	480,	0x00},	// 041: 640*480*75hz(!--)
/*042*/{433,	85,	360,	832,	509,	56,	640,	80,	28,	480,	0x00},	// 042: 640*480*85hz(!--)
/*043*/{352,	56,	360,	1024,	625,	72,	800,	128,	24,	600,	0x98},	// 043: 800*600*56hz(!++)
/*044*/{352,	56,	360,	1024,	625,	184,	800,	32,	24,	600,	0x80},	// 044: 800*600*56hz(!--) 
/*045*/{379,	60,	400,	1056,	628,	128,	800,	88,	27,	600,	0x18},	// 045: 800*600*60hz(!++)
/*046*/{374,	60,	383,	1024,	624,	80,	800,	112,	21,	600,	0x00},	// 046: 800*600*60hz CVT
/*047*/{370,	60,	355,	960,	618,	32,	800,	80,	15,	600,	0x00},	// 047: 800*600*60hz CVT-RB
/*048*/{480,	72,	500,	1040,	666,	120,	800,	64,	29,	600,	0x18},	// 048: 800*600*72hz(!++)
/*049*/{469,	75,	495,	1056,	625,	80,	800,	160,	24,	600,	0x18},	// 049: 800*600*75hz(!++)
/*050*/{536,	85,	563,	1048,	631,	64,	800,	152,	30,	600,	0x18},	// 050: 800*600*85hz(!++)
/*051*/{497,	75,	572,	1152,	667,	64,	832,	224,	42,	624,	0x00},	// 051: 832*624*74.6hz Mac-16(!--)
/*052*/{298,	60,	315,	1056,	500,	80,	848,	104,	17,	480,	0x00},	// 052:848*480 60Hz(Chroma)
/*053*/{310,	60,	338,	1088,	517,	112,	848,	112,	31,	480,	0x18},	// 053: 848*480 60Hz(!++)
/*054*/{295,	60,	298,	1008,	494,	32,	848,	80,	11,	480,	0x00},	// 054: 848*480 60Hz CVT-RB
/*055*/{302,	60,	336,	1112,	504,	104,	856,	104,	23,	480,	0x00},	// 055: 856*480 60Hz
/*056*/{484,	60,	650,	1344,	806,	136,	1024,	160,	34,	768,	0x00},	// 056: 1024*768*60hz(--DMTvs1280cvt)
/*057*/{0,	0,	650,	1352,	804,	96,	1024,	202,	33,	768,	0x00},	// 057: 1024*768*60hz(--Astro848 Timing)
/*058*/{0,	0,	816,	1688,	806,	130,	1280,	174,	25,	768,	0x00},	// 058: 1280*768*60hz(--Chroma2229 Timing)
/*059*/{0,	0,	801,	1664,	798,	136,	1280,	200,	26,	768,	0x00},	// 059: 1280*768*60hz(--DMTvs1280cvt) // P-Customers suggest remove
/*060*/{478,	60,	635,	1328,	798,	104,	1024,	152,	27,	768,	0x24},	// 060: 1024*768 60Hz CVT! --start
/*061*/{478,	60,	795,	1664,	798,	128,	1280,	192,	27,	768,	0x24},	// 061: 1280*768 60Hz CVT!
/*062*/{478,	60,	848,	1776,	798,	136,	1360,	208,	27,	768,	0x24},	// 062: 1360*768*60Hz CVT  --end
/*063*/{477,	60,	855,	1792,	795,	112,	1360,	256,	24,	768,	0x00},	// 063: 1360*768*60Hz VESA
/*064*/{473,	60,	560,	1184,	790,	32,	1024,	80,	19,	768,	0x24},	// 064: 1024*768*60Hz CVT-RB! --start
/*065*/{474,	60,	683,	1440,	790,	32,	1280,	80,	19,	768,	0x24},	// 065: 1280*768*60Hz CVT-RB!
/*066*/{474,	60,	720,	1520,	790,	32,	1360,	80,	20,	768,	0x24},	// 066: 1360*768*60Hz CVT-RB! --end
/*067*/{565,	70,	750,	1328,	806,	136,	1024,	144,	34,	768,	0x00},	// 067: 1024*768*70hz (!--)
/*068*/{600,	75,	788,	1312,	800,	96,	1024,	176,	31,	768,	0x18},	// 068: 1024*768*75hz(DMTvs1280cvt)
/*069*/{603,	75,	820,	1360,	805,	104,	1024,	168,	34,	768,	0x2C},	// 069: 1024*768 75Hz CVT --start
/*070*/{603,	75,	1023,	1696,	805,	128,	1280,	208,	34,	768,	0x2C},	// 070: 1280*768 75Hz CVT	 --end
/*071*/{687,	85,	945,	1376,	809,	104,	1024,	176,	38,	768,	0x2C},	// 071: 1024*768 85Hz CVT --start
/*072*/{686,	85,	1175,	1712,	809,	136,	1280,	216,	38,	768,	0x2C},	// 072: 1280*768 85Hz CVT --end
/*073*/{611,	76,	831,	1360,	803,	112,	1024,	168,	34,	768,	0x00},	// 073: 1024*768 76Hz 
/*074*/{537,	60,	817,	1520,	897,	120,	1152,	184,	30,	864,	0x00},	// 074: 1152*864 60Hz new
/*075*/{638,	70,	945,	1480,	912,	96,	1152,	200,	47,	864,	0x00},	// 075: DMT1170 1152*864 70Hz
/*076*/{675,	75,	1080,	1600,	900,	128,	1152,	256,	35,	864,	0x18},	// 076: 1152*864*75hz(!++)
/*077*/{686,	75,	1000,	1456,	915,	128,	1152,	144,	42,	870,	0x00},	// 077: Mac-21 1152*870 75Hz
/*078*/{771,	85,	1215,	1576,	907,	128,	1152,	232,	42,	864,	0x00},	// 078: DMT1185(802BT)
/*079*/{617,	65,	929,	1504,	937,	128,	1152,	195,	35,	900,	0x00},	// 079: SUN1166 1152*900 65Hz
/*080*/{717,	76,	1055,	1472,	943,	96,	1152,	208,	41,	900,	0x00},	// 080: SUN1176 1152*900 76Hz
/*081*/{450,	60,	742,	1650,	750,	31,	1280,	270,	25,	720,	0x00},	// 081: 720P
/*082*/{560,	70,	950,	1696,	802,	128,	1280,	208,	31,	768,	0x00},	// 082: 1280*768 70Hz CVT(70 nonstd)
/*083*/{603,	75,	1023,	1696,	805,	128,	1280,	208,	34,	768,	0x00},	// 083: 1280*768 75Hz CVT
/*084*/{600,	60,	1080,	1800,	1000,	112,	1280,	312,	39,	960,	0x18},	// 084: 1280x0960x60HZ(!++)
/*085*/{752,	75,	1300,	1728,	1005,	136,	1280,	224,	42,	960,	0x08},	// 085: 1280*960 75Hz CVT
/*086*/{859,	85,	1485,	1728,	1011,	160,	1280,	224,	50,	960,	0x18},	// 086: 1280x0960x85HZ(++)
/*087*/{639,	60,	1080,	1688,	1066,	112,	1280,	248,	41,	1024,	0x18},	// 087: 1280*1024*60hz(++)
/*088*/{800,	75,	1330,	1664,	1066,	144,	1280,	248,	41,	1024,	0x18},	// 088: 1280*1024*75hz(++)
/*089*/{911,	85,	1575,	1728,	1072,	160,	1280,	224,	47,	1024,	0x18},	// 089: 1280*1024*85hz(++)
/*090*/{647,	60,	1010,	1560,	1080,	32,	1400,	80,	27,	1050,	0x34},	// 090: 1400*1050*60hz CVT-RB --start
/*091*/{647,	60,	1190,	1840,	1080,	32,	1680,	80,	27,	1050,	0x34},	// 091: 1680*1050*60hz CVT-RB --end 
/*092*/{653,	60,	1218,	1864,	1089,	144,	1400,	232,	36,	1050,	0x2C},	// 092: 1400*1050*60hz CVT --start
/*093*/{653,	60,	1463,	2240,	1089,	176,	1680,	280,	36,	1050,	0x2C},	// 093: 1680*1050*60hz CVT --end
/*094*/{823,	75,	1560,	1896,	1099,	144,	1400,	248,	46,	1050,	0x08},	// 094: 1400*1050*75hz CVT
/*095*/{555,	60,	888,	1600,	926,	32,	1440,	80,	23,	900,	0x10},	// 095: 1440*900*60hz CVT-RB
/*096*/{559,	60,	1065,	1904,	934,	152,	1440,	232,	31,	900,	0x08},	// 096: 1440*900*60hz CVT
/*097*/{706,	75,	1368,	1936,	942,	152,	1440,	248,	39,	900,	0x08},	// 097: 1440*900*75hz CVT
/*098*/{750,	60,	1620,	2160,	1250,	192,	1600,	304,	49,	1200,	0x00},	// 098: 1600*1200*60hz VESA1200-60
/*099*/{666,	60,	1385,	2080,	1111,	32,	1920,	80,	28,	1080,	0x10},	// 099: 1920*1080*60hz CVT-RB
/*100*/{740,	60,	1540,	2080,	1235,	32,	1920,	80,	32,	1200,	0x10},	// 100: 1920*1200*60hz CVT-RB
/*101*/{717,	67,	1170,	1632,	1067,	112,	1280,	224,	41,	1024,	0x10},	// 101: 1280*1024*67hz
/*102*/{670,	60,	1728,	2576,	1118,	208,	1920,	328,	37,	1080,	0x00},	// 102: 1920*1080*60Hz(VSC 1960H)
/*103*/{675,	60,	1485,	2200,	1125,	30,	1920,	205,	41,	1080,	0x00},	// 103: 1080p-60
//HF	VF	P-CLK	HTOTAL	VTOTAL	H(Bp+W)	H-Act	H-BP	V(BP+w)	V-Act  //USER MODE
/*104*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},	// 104: UserMode Entry
/*105*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},	// 105: UserMode Entry
/*106*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},	// 106: UserMode Entry
/*107*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},	// 107: UserMode Entry
/*108*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},	// 108: UserMode Entry
/*109*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},	// 109: UserMode Entry
/*110*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},	// 110: UserMode Entry
/*111*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},	// 111: UserMode Entry
//HF	VF	P-CLK	HTOTAL	VTOTAL	H(Bp+W)	H-Act	H-BP	V(BP+w)	V-Act

#else 
//   VGA table for SUPPORT_VGA_AMBIGUOUS_USER_SEL

	//HF	VF	P-CLK	HTOTAL	VTOTAL	H(Bp+W)	H-Act	H-BP	V(BP+w)	V-Act
/*000*/{0,	0,	0,	0,	0,	0,	720,	0,	0,	480,	0x00},	// 000: No Signal
/*001*/{157,	60,	540,	3432,	525,	283,	2880,	221,	19,	480,	0x41},	// 001: 480i oversample(10,11)
/*002*/{157,	50,	540,	3456,	625,	259,	2880,	269,	23,	576,	0x41},	// 002: 576i oversample(25,26)
/*003*/{315,	60,	540,	1716,	525,	124,	1440,	128,	36,	480,	0x01},	// 003: 480p oversample(14,15)
/*004*/{313,	50,	540,	1728,	625,	124,	1440,	144,	44,	576,	0x81},	// 004: 576p oversample(29,30)
/*005*/{375,	50,	742,	1980,	750,	32,	1280,	270,	25,	720,	0x00},	// 005: 720p50 (19)
/*006*/{450,	60,	742,	1650,	750,	31,	1280,	270,	25,	720,	0x00},	// 006: 720p60 (4)
/*007*/{270,	48,	742,	2750,	1125,	44,	1920,	148,	10,	1080,	0x40},	// 007: 1080i-24
/*008*/{281,	50,	742,	2640,	1125,	35,	1920,	202,	21,	1080,	0x40},	// 008: 1080i-25
/*009*/{338,	60,	742,	2200,	1125,	36,	1920,	202,	20,	1080,	0x40},	// 009: 1080i-30
/*010*/{270,	24,	742,	2750,	1125,	33,	1920,	202,	41,	1080,	0x00},	// 010: 1080p-24 (32)
/*011*/{281,	25,	742,	2640,	1125,	33,	1920,	202,	41,	1080,	0x00},	// 011: 1080p-25 (33)
/*012*/{337,	30,	742,	2200,	1125,	33,	1920,	202,	41,	1080,	0x00},	// 012: 1080p-30 (34)
/*013*/{562,	50,	1485,	2640,	1125,	30,	1920,	205,	41,	1080,	0x00},	// 013: 1080p-50 (31)
/*014*/{675,	60,	1485,	2200,	1125,	30,	1920,	205,	41,	1080,	0x00},	// 014: 1080p-60 (16)
/*015*/{157,	60,	270,	1716,	525,	134,	1440,	118,	19,	480,	0x40},	// 015: 480i(1440x480)(6,7)
/*016*/{157,	50,	270,	1728,	625,	124,	1440,	140,	23,	576,	0x40},	// 016: 576i(1440x576)(21,22)
/*017*/{315,	60,	270,	858,	525,	74,	720,	52,	36,	480,	0x02},	// 017: 480p(720x480)(2,3)
/*018*/{313,	50,	270,	864,	625,	74,	720,	60,	44,	576,	0x80},	// 018: 576p(720x576) (17,18)
/*019*/{180,	24,	743,	4125,	750,	40,	1280,	260,	25,	720,	0x00},	// 019: 720p24
/*020*/{188,	25,	743,	3960,	750,	40,	1280,	260,	25,	720,	0x00},	// 020: 720p25
/*021*/{225,	30,	743,	3300,	750,	40,	1280,	260,	25,	720,	0x00},	// 021: 720p30
/*022*/{0,	0,	252,	800,	525,	96,	640,	48,	35,	480,	0x00},	// 022: 640*480 may need to support(1),hv-ambiguous
/*023*/{0,	0,	270,	1716,	262,	96,	720,	48,	35,	240,	0x00},	// 023: 240p(720x1440) may need to support(8,9*),hv-ambiguous
/*024*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},	// 024: 240p oversample may need to support(12,13*),hv-ambiguous
/*025*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},	// 025: 288p(1440x288) may need to support(23,24**),hv-ambiguous
/*026*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},	// 026: 288p oversample may need to support(27,28**),hv-ambiguous
/*027*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},	// 027: Blank
/*028*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},	// 028: Blank
	//HF	VF	P-CLK	HTOTAL	VTOTAL	H(Bp+W)	H-Act	H-BP	V(BP+w)	V-Act 
/*029*/{244,	60,	312,	1280,	407,	64,	1024,	160,	22,	384,	0x01},	// 029: 512*384*60hz Mac-12M(over sampling)
/*030*/{298,	60,	630,	2112,	500,	160,	1696,	208,	17,	480,	0x01},	// 030:848*480 60Hz(Chroma)
/*031*/{310,	60,	676,	2176,	517,	224,	1696,	224,	31,	480,	0x19},	// 031: 848*480 60Hz(!++)
/*032*/{313,	70,	500,	1600,	449,	192,	1280,	96,	61,	350,	0xB5},	// 032: 640*350*70hz (!+-)                      //Ambiguous group 0
/*033*/{314,	70,	566,	1800,	449,	216,	1440,	108,	62,	350,	0xB5},	// 033: 720*350*70hz text (+ -)??              //Ambiguous group 0
/*034*/{314,	70,	566,	1800,	449,	216,	1440,	108,	36,	400,	0x8D},	// 034: 720*400*70hz  text  (!-+)             //Ambiguous group 0
/*035*/{315,	60,	504,	1600,	525,	192,	1280,	96,	35,	480,	0x03},	// 035: 640*480*60hz (!--)
/*036*/{350,	67,	604,	1728,	525,	128,	1280,	192,	42,	480,	0x01},	// 036: 640*480*67Hz Mac_13C
/*037*/{352,	56,	360,	1024,	625,	72,	800,	128,	24,	600,	0x18},	// 037: 800*600*56hz(!++)
/*038*/{375,	75,	630,	1680,	500,	128,	1280,	240,	19,	480,	0x01},	// 038: 640*480*75hz(!--)
/*039*/{379,	60,	400,	1056,	628,	128,	800,	88,	27,	600,	0x18},	// 039: 800*600*60hz(!++)
/*040*/{379,	72,	630,	1664,	520,	96,	1280,	240,	31,	480,	0x01},	// 040: 640*480*72hz(!--)
/*041*/{379,	85,	630,	1664,	445,	128,	1280,	192,	63,	350,	0x95},	// 041: 640*350*85hz (+-)			//Ambiguous group 1
/*042*/{379,	85,	630,	1664,	445,	128,	1280,	192,	44,	400,	0x8D},	// 042: 640*400 85Hz (!-+)			//Ambiguous group 1
/*043*/{379,	85,	355,	936,	446,	72,	720,	108,	45,	400,	0x8C},	// 043: 720*400 85Hz(-+)			                    //Ambiguous group 1
/*044*/{433,	85,	360,	832,	509,	56,	640,	80,	28,	480,	0x00},	// 044: 640*480*85hz(!--)
/*045*/{469,	75,	495,	1056,	625,	80,	800,	160,	24,	600,	0x18},	// 045: 800*600*75hz(!++)
/*046*/{478,	60,	635,	1328,	798,	104,	1024,	152,	27,	768,	0x04},	// 046: 1024*768 60Hz CVT!			//Ambiguous group 2
/*047*/{478,	60,	795,	1664,	798,	128,	1280,	192,	27,	768,	0x04},	// 047: 1280*768 60Hz CVT!			//Ambiguous group 2
/*048*/{478,	60,	848,	1776,	798,	136,	1360,	208,	27,	768,	0x04},	// 048: 1360*768*60Hz CVT			//Ambiguous group 2
/*049*/{480,	72,	500,	1040,	666,	120,	800,	64,	29,	600,	0x18},	// 049: 800*600*72hz(!++)
/*050*/{497,	75,	572,	1152,	667,	64,	832,	224,	42,	624,	0x00},	// 050: 832*624*74.6hz Mac-16(!--)
/*051*/{536,	85,	563,	1048,	631,	64,	800,	152,	30,	600,	0x18},	// 051: 800*600*85hz(!++)
/*052*/{537,	60,	817,	1520,	897,	120,	1152,	184,	30,	864,	0x00},	// 052: 1152*864 60Hz new
/*053*/{559,	60,	1065,	1904,	934,	152,	1440,	232,	31,	900,	0x08},	// 053: 1440*900*60hz CVT
/*054*/{560,	70,	950,	1696,	802,	128,	1280,	208,	31,	768,	0x04},	// 054: 1280*768 70Hz CVT(70 nonstd)	//Ambiguous group 3
/*055*/{565,	70,	750,	1328,	806,	136,	1024,	144,	34,	768,	0x04},	// 055: 1024*768*70hz (!--)			//Ambiguous group 3
/*056*/{600,	60,	1080,	1800,	1000,	112,	1280,	312,	39,	960,	0x18},	// 056: 1280x0960x60HZ(!++)
/*057*/{603,	75,	1023,	1696,	805,	128,	1280,	208,	34,	768,	0x04},	// 057: 1280*768 75Hz CVT		//Ambiguous group 4
/*058*/{603,	75,	820,	1360,	805,	104,	1024,	168,	34,	768,	0x0C},	// 058: 1024*768 75Hz CVT			//Ambiguous group 4
/*059*/{617,	65,	929,	1504,	937,	128,	1152,	195,	35,	900,	0x00},	// 059: SUN1166 1152*900 65Hz
/*060*/{638,	70,	945,	1480,	912,	96,	1152,	200,	47,	864,	0x00},	// 060: DMT1170 1152*864 70Hz
/*061*/{639,	60,	1080,	1688,	1066,	112,	1280,	248,	41,	1024,	0x18},	// 061: 1280*1024*60hz(++)
/*062*/{647,	60,	1010,	1560,	1080,	32,	1400,	80,	27,	1050,	0x14},	// 062: 1400*1050*60hz CVT-RB	//Ambiguous group 5
/*063*/{647,	60,	1190,	1840,	1080,	32,	1680,	80,	27,	1050,	0x14},	// 063: 1680*1050*60hz CVT-RB	//Ambiguous group 5
/*064*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},	// 064: NULL Items
/*065*/{653,	60,	1218,	1864,	1089,	144,	1400,	232,	36,	1050,	0x0C},	// 065: 1400*1050*60hz CVT			//Ambiguous group 6
/*066*/{653,	60,	1463,	2240,	1089,	176,	1680,	280,	36,	1050,	0x0C},	// 066: 1680*1050*60hz CVT			//Ambiguous group 6
/*067*/{666,	60,	1385,	2080,	1111,	32,	1920,	80,	28,	1080,	0x10},	// 067: 1920*1080*60hz CVT-RB
/*068*/{675,	75,	1080,	1600,	900,	128,	1152,	256,	35,	864,	0x18},	// 068: 1152*864*75hz(!++)
/*069*/{686,	75,	1000,	1456,	915,	128,	1152,	144,	42,	870,	0x00},	// 069: Mac-21 1152*870 75Hz
/*070*/{686,	85,	1175,	1712,	809,	136,	1280,	216,	38,	768,	0x0C},	// 070: 1280*768 85Hz CVT			 //Ambiguous group 7
/*071*/{687,	85,	945,	1376,	809,	104,	1024,	176,	38,	768,	0x0C},	// 071: 1024*768 85Hz CVT			 //Ambiguous group 7
/*072*/{706,	75,	1368,	1936,	942,	152,	1440,	248,	39,	900,	0x08},	// 072: 1440*900*75hz CVT
/*073*/{717,	67,	1170,	1632,	1067,	112,	1280,	224,	41,	1024,	0x10},	// 073: 1280*1024*67hz
/*074*/{717,	76,	1055,	1472,	943,	96,	1152,	208,	41,	900,	0x00},	// 074: SUN1176 1152*900 76Hz
/*075*/{740,	60,	1540,	2080,	1235,	32,	1920,	80,	32,	1200,	0x14},	// 075: 1920*1200*60hz CVT-RB		//Ambiguous group 8
/*076*/{750,	60,	1620,	2160,	1250,	192,	1600,	304,	49,	1200,	0x04},	// 076: 1600*1200*60hz VESA1200-60	//Ambiguous group 8
/*077*/{752,	75,	1300,	1728,	1005,	136,	1280,	224,	42,	960,	0x08},	// 077: 1280*960 85Hz CVT
/*078*/{771,	85,	1215,	1576,	907,	128,	1152,	232,	42,	864,	0x00},	// 078: DMT1185(802BT)
/*079*/{800,	75,	1330,	1664,	1066,	144,	1280,	248,	41,	1024,	0x18},	// 079: 1280*1024*75hz(++)
/*080*/{823,	75,	1560,	1896,	1099,	144,	1400,	248,	46,	1050,	0x08},	// 080: 1400*1050*75hz CVT
/*081*/{859,	85,	1485,	1728,	1011,	160,	1280,	224,	50,	960,	0x18},	// 081: 1280x0960x85HZ(++)
/*082*/{911,	85,	1575,	1728,	1072,	160,	1280,	224,	47,	1024,	0x18},	// 082: 1280*1024*85hz(++)
//HF	VF	P-CLK	HTOTAL	VTOTAL	H(Bp+W)	H-Act	H-BP	V(BP+w)	V-Act  //USER MODE
/*083*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},	// 083: UserMode Entry
/*084*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},	// 084: UserMode Entry
/*085*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},	// 085: UserMode Entry
/*086*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},	// 086: UserMode Entry
/*087*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},	// 087: UserMode Entry
/*088*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},	// 088: UserMode Entry
/*089*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},	// 089: UserMode Entry
/*090*/{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0x00},	// 090: UserMode Entry
//HF	VF	P-CLK	HTOTAL	VTOTAL	H(Bp+W)	H-Act	H-BP	V(BP+w)	V-Act
#endif
};


//don't change the vga timing number when you add  vga timing , only hdtv/user timing number is needed to be changed.
#define USERMODE_TIMING 8 
#define HDTV_TIMING_NUM	29
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


/* HDTV options ------------------------------*/
UINT8 bHdtvOpt01_MDmute0=10; //for MD , waiting lock mute
UINT8 bHdtvOpt01_MDmute1=4; //for MD , after lock stable
/* Opt03 leave for sync slicer*/
UINT8 bHdtvOpt04_AutoPhase=0;//for auto phase
//UINT8 bHdtvOpt05_CustPos=0; //for save each timing custom position in EEPROM
UINT8 bHdtvOpt05_AdaptiveSlicer=1; //Enable adaptive slicer


#endif // _VGA_CUSTOMIZATION_H_
