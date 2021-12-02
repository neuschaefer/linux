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
#include "x_typedef.h"
#include "jpg_if.h"

#ifdef JPEG_AUTO_TEST

typedef struct _JPGCLI_TEST_SET_T
{
	CHAR *szShortName;
	UINT32 u4ByteSize;
	CHAR *szFileName;
} JPGCLI_TEST_SET_T;

typedef struct _JPGCLI_TEST_SETS_T
{
	JPGCLI_TEST_SET_T *prTestSet;
	CHAR *szInputDir;
} JPGCLI_TEST_SETS_T;


void JPGCLI_GetTestSet(INT32 i4Index, JPGCLI_TEST_SET_T **pprSet, CHAR **pszInputDir);
INT32 JPGCLI_GetMaxSet(void);



/*static CHAR _szDefaultDir[] =
	"d:/jpg_test/cases";

JPGCLI_TEST_SET_T _arDefaultSet[] =
{
	{"koala", 37586, "KOALA_BASE_433x600_37586.jpg"},
	{"t1", 1572518, "SONY_T1_2048x1536_1572518.JPG"},
	{"mavica", 1378264, "SONY_MAVICA_1360x2048_1378264.JPG"},
	{"c220-1", 415536, "OLYMPUS_C220Z_1600x1200_415536.JPG"},
	{"c220-2", 392469, "OLYMPUS_C220Z_1200x1600_392469.JPG"},
	{"sp2k", 903718, "FUJI_SP2000_1840x1232_903718.JPG"},
	{"finepix500", 609347, "FUJI_FINEPIX500_1280x1024_609347.jpg"},
	{"s410", 1181151, "CANON_S410_1704x2272_1181151.JPG"},
	{"g2", 133404, "CANON_G2_1024x768_133404.JPG"},
	// add more here
	{NULL, NULL, NULL}
};

static CHAR _szAgfaDir[] =
        "D:/jpg_test/pattern_database/SonyDisc/AGFA_SQA_11pcs";
static JPGCLI_TEST_SET_T _arAgfaSet[] =
{
	//{"agfa1",  220160, "AGFA0001.JPG"},
	{"agfa2",  182620, "AGFA0002.JPG"},
	{"agfa3",  213360, "AGFA0003.JPG"},
	{"agfa4",  529540, "AGFA0004.JPG"},
	{"agfa6",  186049, "AGFA0006.JPG"},
	{"agfa7",  313786, "AGFA0007.JPG"},
	{"agfa8",  261036, "AGFA0008.JPG"},
	{"agfa9",  581121, "AGFA0009.JPG"},
	{"agfa10", 278216, "AGFA0010.JPG"},
	{"agfa11", 327706, "AGFA0011.JPG"},
	{"agfa12", 283636, "AGFA0012.JPG"},
	// add more here
	{NULL, NULL, NULL}
};*/

static CHAR _szCanonNewDir[] =
      "D:\\jpg_test\\pattern_database\\SonyDisc\\CANON_NEW_17pcs";
static JPGCLI_TEST_SET_T _arCanonNewSet[] =
{
	{"cnew1",  3363924, "EOS 10D.jpg"},
	{"cnew2",  2139924, "EOS 1D.jpg"},
	{"cnew3",  4563938, "EOS 1Ds.jpg"},
	{"cnew4",  4511997, "EOS Kiss digital.jpg"},
	{"cnew5",  1656004, "IXY DIGITAL 400.jpg"},
	{"cnew6",  1324301, "IXY DIGITAL L.jpg"},
	{"cnew7",   930882, "PowerShot A200.jpg"},
	{"cnew8",  1105625, "PowerShot A300.jpg"},
	{"cnew9",  1167777, "PowerShot A60.jpg"},
	{"cnew10", 1453091, "PowerShot A70.jpg"},
	{"cnew11", 1241484, "PowerShot A80.jpg"},
	{"cnew12", 1738849, "PowerShot G5.jpg"},
	{"cnew13", 2675556, "PowerShot S50.jpg"},
	{"cnew14", 1551637, "ixy digital 30.jpg"},
	{"cnew15", 1340053, "ixy digital 320.jpg"},
	{"cnew16", 1016734, "ixy ditital 200a.jpg"},
	{"cnew17", 745339, "ixy ditital 300a.jpg"},
	// add more here
	{NULL, NULL, NULL}
};

static CHAR _sz89CanNotDir[] =
      "D:\\jpg_test\\pattern_database\\other\\050314_can_not_play";
//	"D:/usr/jpgs/050314_can_not_play";
static JPGCLI_TEST_SET_T _ar89CanNotSet[] =
{
	{"cn1", 1528682, "100_0508.jpg"},
	{"cn2", 1344845, "100_0510.jpg"},
	// add more here
	{NULL, NULL, NULL}
};
#if 0
static CHAR _szProgr1Dir[] =
      "D:/jpg_test/pattern_database/other/prog1";
//	"D:/usr/jpgs/pscan/prog1";
static JPGCLI_TEST_SET_T _arProgr1Set[] =
{
	{"small", 8317, "small_pscan_3scan.jpg"},
	{"baselineTest", 3750947, "spl01_r1.jpg"},
	{"img1", 111856, "image001.jpg"},
	{"img2", 104757, "image002.jpg"},
	{"img3", 83314, "image003.jpg"},
	{"img4", 127341, "image004.jpg"},
	{"img5", 172378, "image005.jpg"},
	{"img6", 112033, "image006.jpg"},
	{"img7", 150747, "image007.jpg"},
	{"img8", 107231, "image008.jpg"},		
	{"4", 244293, "04.jpg"},
	{"5", 282006, "05.jpg"},
	{"6", 190176, "06.jpg"},
	{"7", 318592, "07.jpg"},
	{"8", 161612, "08.jpg"},
	{"aut", 51409, "autumn.jpg"},
	{"tower", 258606, "de_tower_progressive.jpg"},
	{"horse", 27268, "horse_p.jpg"},
	{"hvhs", 516096, "hvhs.jpg"},
	{"n1", 31504, "no5-1.jpg"},
	{"n2", 39649, "no5-2.jpg"},
	{"n3", 46333, "no5-3.jpg"},
	{"n4", 27835, "no5-4.jpg"},
	{"rose", 5655, "rose.jpg"},
	{"red", 44884, "red_flower.jpg"},
	{"van", 20964, "van_gogh.jpg"},
	// add more here
	{NULL, NULL, NULL}
};
#endif
static CHAR _sz200504Dis2Dir[] =
      "D:\\jpg_test\\pattern_database\\other\\2005_04_Dis2";
//	"D:/usr/jpgs/2005_04_Dis2";
static JPGCLI_TEST_SET_T _ar04Dis2Set[] =
{
	{"et1", 853059, "et001.jpg"},
	{"et2", 755532, "et002.jpg"},
	{"et3", 872588, "et003.jpg"},
	{"et4", 730541, "et004.jpg"},
	{"et5", 633228, "et005.jpg"},
	{"et6", 607250, "et006.jpg"},
	{"et7", 568858, "et007.jpg"},
	{"et8", 853548, "et008.jpg"},
	{"et9", 860306, "et009.jpg"},
	{"et10", 858926, "et010.jpg"},
	// add more here
	{NULL, NULL, NULL}
};

static CHAR _szAbortDir[] =
        "D:\\jpg_test\\JpegTestFiles\\Abort";
static JPGCLI_TEST_SET_T _arAbortSet[]=
{
        {"0", 4407427, "DCSPro4M.jpg"},
        {"1", 327843, "diesel01.jpg"}, //progressive
        {"2", 300599, "diesel02.jpg"}, //progressive
        {"3", 328451, "diesel03.jpg"}, //progressive
        {"4", 333226, "diesel04.jpg"}, //progressive
        {"5", 4488719, "Sany4M.jpg"},
        {NULL, NULL, NULL}
};

static CHAR _szBP111Dir[] =
        "D:\\jpg_test\\JpegTestFiles\\BP111";
static JPGCLI_TEST_SET_T _arBP111Set[]=
{
        {"0", 3459277, "e20sp_02.jpg"},
        {"1", 2189217, "e20sp_03.jpg"},
        {"2", 730541, "et004.jpg"},
        {"3", 633228, "et005.jpg"},
        {"4", 853548, "et008.jpg"},
        {"5", 860306, "et009.jpg"},
        {"6", 2139924, "Z_EOS1D.jpg"},
        {NULL, NULL, NULL}
};

static CHAR _szBP211Dir[] =
        "D:\\jpg_test\\JpegTestFiles\\BP211";
static JPGCLI_TEST_SET_T _arBP211Set[]=
{
        {"0", 1753350, "40i_03.jpg"},
        {"1", 1698561, "50i_01.jpg"},
        {"2", 1501071, "Allegretto_M81.jpg"},
        {"3", 2204402, "c5060wzsp_03.jpg"},
        {"4", 2257965, "c755uzsp_02.jpg"},
        {"5", 1770766, "CANO0013.JPG"},
        {"6", 1660283, "CANO0017.JPG"},
        {"7", 1312098, "CaplioG3.jpg"},
        {"8", 82915, "CLIPIT50.jpg"},
        {"9", 915573, "COOLPIX3100.jpg"},
        {"10", 651930, "DCR_trv50.JPG"},
        {"11", 90721, "DS10.jpg"},
        {"12", 89575, "DS20.jpg"},
        {"13", 2749285, "DSC_AZ1.jpg"},
        {"14", 1812133, "DSC_F77A.jpg"},
        {"15", 1020502, "DSC_MZ3.jpg"},
        {"16", 1300505, "DSC_P72.jpg"},
        {"17", 336405, "DSC_R1.jpg"},
        {"18", 521957, "DSC_U40.jpg"},
        {"19", 737181, "EPSO0003.JPG"},
        {"20", 858219, "FinepixA210_03.jpg"},
        {"21", 1620044, "FinepixF420_01.jpg"},
        {"22", 1452007, "FinepixF420_02.jpg"},
        {"23", 3714724, "FinepixS7000_03.jpg"},
        {"24", 2443256, "mju20dsp_04.jpg"},
        {"25", 2489126, "mju25dsp_01.jpg"},
        {"26", 758272, "RICOH0007.jpg"},
        {"27", 787968, "RICOH0008.jpg"},
        {"28", 580608, "RICOH0010.jpg"},
        {"29", 604241, "SL300R.jpg"},
        {"30", 979426, "SORA_T20.jpg"},
        {"31", 1187521, "SORA_T30.jpg"},
        {"32", 1026476, "TOSH0002.jpg"},
        {"33", 869683, "TOSH0004.jpg"},
        {"34", 1197909, "TOSH0006.jpg"},
        {"35", 1683383, "x250sp_03.jpg"},
        {"36", 2312553, "x2sp_04.jpg"},
        {"37", 1750366, "z4900_04.jpg"},
        {NULL, NULL, NULL}
};

static CHAR _szBP411Dir[] =
        "D:\\jpg_test\\JpegTestFiles\\BP411";
static JPGCLI_TEST_SET_T _arBP411Set[]=
{
        {"0", 1417227, "CASI0008.jpg"},
        {"1", 1850194, "CASI0009.jpg"},
        {"2", 150079, "DIGIdb100.jpg"},
        {"3", 553481, "EasyShareDX3600.jpg"},
        {"4", 795344, "EasyShareDX3900.jpg"},
        {"5", 1163067, "PENT0001.jpg"},
        {"6", 313945, "pict0366.jpg"},
        {"7", 552001, "qm200_1.jpg"},
        {NULL, NULL, NULL}
};

static CHAR _szBrandDir[] =
        "D:\\jpg_test\\JpegTestFiles\\Brand";
static JPGCLI_TEST_SET_T _arBrandSet[]=
{
        {"0", 180626, "anessa.jpg"},
        {"1", 188863, "anessa_a2.jpg"},
        {"2", 265204, "anessa_a3.jpg"},
        {"3", 80420, "anessa_b2.jpg"},
        {"4", 113229, "anessa_b3.jpg"},
        {"5", 77019, "annasui01.jpg"},
        {"6", 58425, "annasui02.jpg"},
        {"7", 97437, "chanel.jpg"},
        {"8", 270393, "dior01.jpg"},
        {"9", 255823, "dior02.jpg"},
        {"10", 221123, "lancome.jpg"},
        {NULL, NULL, NULL}
};

static CHAR _szFBallDir[] =
        "D:\\jpg_test\\JpegTestFiles\\FBall";
static JPGCLI_TEST_SET_T _arFBallSet[]=
{
        {"0", 38045, "526173927.jpg"},
        {"1", 42722, "52980515.jpg"},
        {"2", 46817, "52980519.jpg"},
        {"3", 41492, "52980541.jpg"},
        {"4", 33672, "52980547.jpg"},
        {"5", 57694, "52980548.jpg"},
        {"6", 41475, "52980550.jpg"},
        {"7", 7182, "52980564.jpg"},  // mw version, fill data issue
        {NULL, NULL, NULL}
};

static CHAR _szThumbDir[] =
        "D:\\jpg_test\\JpegTestFiles\\Thumb";
static JPGCLI_TEST_SET_T _arThumbSet[]=
{
        {"0", 133404, "CANON_G2_1024x768_133404.JPG"},
        {"1", 1181151, "CANON_S410_1704x2272_1181151.JPG"},
        {"2", 609347, "FUJI_FINEPIX500_1280x1024_609347.jpg"},
        {"3", 37586, "KOALA_BASE_433x600_37586.jpg"},
        {"4", 415536, "OLYMPUS_C220Z_1600x1200_415536.JPG"},
        {NULL, NULL, NULL}
};

static CHAR _szVariousDir[] =
        "D:\\jpg_test\\JpegTestFiles\\various jpg";
static JPGCLI_TEST_SET_T _arVariousSet[]=
{
        {"0", 973, "CAT_16x16.jpg"},
        {"1", 1421, "CAT_16x64.jpg"},
        {"2", 1868, "CAT_16x128.jpg"},
        {"3", 2814, "CAT_16x256.jpg"},
        {"4", 4267, "CAT_16x512.jpg"},
        {"5", 7045, "CAT_16x1024.jpg"},
        {"6", 2752, "CAT_64x64.jpg"},
        {"7", 4536, "CAT_64x128.jpg"},
        {"8", 8069, "CAT_64x256.jpg"},
        {"9", 12669, "CAT_64x512.jpg"},
        {"10", 22607, "CAT_64x1024.jpg"},
        {"11", 7380, "CAT_128x128.jpg"},
        {"12", 13091, "CAT_128x256.jpg"},
        {"13", 23136, "CAT_128x512.jpg"},        
        {"14", 43376, "CAT_128x1024.jpg"},
        {"15", 22460, "CAT_256x256.jpg"},
        {"16", 41113, "CAT_256x512.jpg"},        
        {"17", 85065, "CAT_256x1024.jpg"},    
        {"18", 73905, "CAT_512x512.jpg"},    
        {"19", 138398, "CAT_512x1024.jpg"},            
        {"20", 247521, "CAT_1024x1024.jpg"},    
        {"21", 286319, "CAT_1366x768.jpg"},          
        //{"22", 824526, "MTR_L1_boss.jpg"},             
        {NULL, NULL, NULL}
};
#if 0
static CHAR _szErrorDir[] =
        "D:/jpg_test/JpegTestFiles/error";
static JPGCLI_TEST_SET_T _arErrorSet[]=
{
        {"0", 1726808, "concert.jpg"},
        {"1", 278321, "W11.jpg"},
        {"2", 478362, "1004.jpg"},
        {"3", 1225917, "DataLost_jpg_2.jpg"},
        {"4", 2424587, "DCAM00044.JPG"},
        {NULL, NULL, NULL}
};
#endif
static CHAR _sz_d_30_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Canon\\d 30";
static JPGCLI_TEST_SET_T _ar_d_30_Set[] =
{
	{"0", 1119832, "001004-1355-05.jpg"},
	{"1", 1039433, "001004-1400-14.jpg"},
	{"2", 792233, "001009-2001-06.jpg"},
	{"3", 1275393, "001009-2005-22.jpg"},
	{"4", 833069, "001009-2007-57.jpg"},
	{"5", 179118, "davies-neil-020.jpg"},
	{"6", 557048, "schoolsproms-neil-285.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_digital_IXUS_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Canon\\digital IXUS";
static JPGCLI_TEST_SET_T _ar_digital_IXUS_Set[] =
{
	{"0", 1530801, "000517-1622-50.jpg"},
	{"1", 1414263, "000517-1626-30.jpg"},
	{"2", 1874996, "000517-1629-28.jpg"},
	{"3", 719066, "000525-1108-49.jpg"},
	{"4", 813379, "000525-1110-52.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_digital_IXUS_300_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Canon\\IXUS 300";
static JPGCLI_TEST_SET_T _ar_digital_IXUS_300_Set[] =
{
	{"00", 7182, "52980564.jpg"},
    {"00", 5425249, "CRW_6067.jpg"},
    {"00", 7182, "52980564.jpg"},	
	{"0", 37586, "koala.jpg"},
	{"1", 928358, "010319-0951-50.jpg"},
//	{"2", 968854, "010319-0953-24.jpg"},
	{"2", 8317, "small-p3scan.jpg"},
	{"3", 924016, "010319-1037-25.jpg"},
	{"4", 657148, "010319-1040-06.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_digital_IXUS_330_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Canon\\digital IXUS 330";
static JPGCLI_TEST_SET_T _ar_digital_IXUS_330_Set[] =
{
	{"0", 1023780, "020403-1144-46.jpg"},
	{"1", 96716, "020403-1144-46[1].jpg"},
	//{"2", 835584, "020403-1145-19.jpg"},  //image damaged
	{"3", 95824, "020403-1145-19[1].jpg"},
	{"4", 74417, "020403-1146-49[1].jpg"},
	{"5", 68274, "020403-1147-53[1].jpg"},
	{"6", 71589, "020403-1149-29[1].jpg"},
	{"7", 112751, "020403-1150-54[1].jpg"},
	{"8", 104168, "020403-1204-45[1].jpg"},
	{"9", 79336, "020403-1210-24[1].jpg"},
	{"10", 95782, "020403-1215-25[1].jpg"},
	{"11", 115218, "020403-1237-33[1].jpg"},
	{"12", 121138, "020403-1241-57[1].jpg"},
	{"13", 68353, "020416-1011-09[1].jpg"},
	{"14", 1630518, "020416-1032-22.jpg"},
	{"15", 142299, "020416-1032-22[1].jpg"},
	{"16", 783927, "020419-1835-42.jpg"},
	{"17", 68829, "020419-1835-42[1].jpg"},
	{"18", 772167, "020419-1839-21.jpg"},
	{"19", 72382, "020419-1839-21[1].jpg"},
	{"20", 18894, "thumbs[3].jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_digital_IXUS_V_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Canon\\digital IXUS V";
static JPGCLI_TEST_SET_T _ar_digital_IXUS_V_Set[] =
{
	{"0", 1154327, "010503-1237-10.jpg"},
	{"1", 1010615, "010503-1259-07.jpg"},
	{"2", 844332, "010503-1301-07.jpg"},
	{"3", 658950, "010503-1303-14.jpg"},
	{"4", 843843, "010506-1404-16.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_eos_10d_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Canon\\eos 10d";
static JPGCLI_TEST_SET_T _ar_eos_10d_Set[] =
{
	{"0", 3768714, "030315-1453-32.jpg"},
	{"1", 2143400, "030316-1027-13.jpg"},
	{"2", 1932513, "030316-1100-46.jpg"},
	{"3", 2573050, "img_9266a.jpg"},
	{"4", 1944802, "img_9269s.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_eos_1d_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Canon\\eos 1d";
static JPGCLI_TEST_SET_T _ar_eos_1d_Set[] =
{
	{"0", 1599436, "7e9a0423.jpg"},
	{"1", 1749472, "7e9a0438.jpg"},
	{"2", 1887504, "7e9a0442.jpg"},
	{"3", 1702928, "7e9a1881.jpg"},
	{"4", 1856960, "7e9a1909.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_eos_1d_mk_II_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Canon\\eos 1d mk II";
static JPGCLI_TEST_SET_T _ar_eos_1d_mk_II_Set[] =
{
	{"0", 5610965, "gr2o0116-raw.jpg"},
	{"1", 2884663, "gr2o0121.jpg"},
	{"2", 2394781, "gr2o0142.jpg"},
	{"3", 1061854, "gr2o3974-raw.jpg"},
	{"4", 2553135, "gr2o3976.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_eos_1d_mk_II_B_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Canon\\eos 1d mk II B";
static JPGCLI_TEST_SET_T _ar_eos_1d_mk_II_B_Set[] =
{
	{"0", 9961587, "vb2c0009.jpg"},
	{"1", 8632165, "vb2c0016.jpg"},
	{"2", 7865883, "vb2c0023.jpg"},
	{"3", 9832509, "vb2c0114.jpg"},
	{"4", 7129454, "vb2c0120.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_eos_1ds_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Canon\\eos 1ds";
static JPGCLI_TEST_SET_T _ar_eos_1ds_Set[] =
{
	//{"0", 2676044, "021104-1112-01-capt1.jpg"},   //E_JPG_ERR_MULTI_APP1
	{"1", 4159520, "021104-1143-14.jpg"},
	{"2", 5451808, "021104-1149-30.jpg"},
	{"3", 2955976, "p020921-1812-36.jpg"},
	{"4", 4830624, "p4a1s0238.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_eos_20d_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Canon\\eos 20d";
static JPGCLI_TEST_SET_T _ar_eos_20d_Set[] =
{
	{"0", 2979751, "040919-1106-34.jpg"},
	{"1", 3872486, "040919-1123-08.jpg"},
	{"2", 3042201, "040919-1225-20.jpg"},
	{"3", 4768030, "img_9380_raw_dpp.jpg"},
	{"4", 2878227, "img_9459_raw_dpp.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_eos_20d_b_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Canon\\eos 20d b";
static JPGCLI_TEST_SET_T _ar_eos_20d_b_Set[] =
{
	{"0", 3450164, "img_0010.jpg"},
	{"1", 1440762, "img_0012_raw.jpg"},
	{"2", 3983701, "img_0019.jpg"},
	{"3", 1211287, "img_8807_raw.jpg"},
	{"4", 2640096, "img_8824.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_eos_300d_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Canon\\eos 300d";
static JPGCLI_TEST_SET_T _ar_eos_300d_Set[] =
{
	{"0", 4076380, "030912-1336-50.jpg"},
	{"1", 2587512, "030912-1705-37.jpg"},
	{"2", 2120802, "030921-0732-10.jpg"},
	{"3", 2808706, "030927-1952-00.jpg"},
	{"4", 3296160, "030928-1050-07.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_eos_300d_b_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Canon\\eos 300d b";
static JPGCLI_TEST_SET_T _ar_eos_300d_b_Set[] =
{
	{"0", 5305154, "030826-1321-16_rj.jpg"},
	{"1", 2172930, "030826-1324-32.jpg"},
	{"2", 4023510, "030826-1327-47.jpg"},
	{"3", 2741202, "030827-2358-40.jpg"},
	{"4", 2379330, "030828-0015-49.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_eos_350d_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Canon\\eos 350d";
static JPGCLI_TEST_SET_T _ar_eos_350d_Set[] =
{
	{"0", 2507500, "img_0061.jpg"},
	{"1", 3335251, "img_2265.jpg"},
	{"2", 3104528, "img_3949.jpg"},
	{"3", 2081413, "img_3967.jpg"},
	{"4", 2194488, "img_3979-raw-acr.jpg"},  //E_JPG_ERR_MULTI_APP1 (-17)
	{"5", 3063833, "img_4448.jpg"},
	{"6", 1859994, "img_4450-raw-acr.jpg"}, //E_JPG_ERR_MULTI_APP1 (-17)
	{NULL, NULL, NULL}
};

static CHAR _sz_eosd_60_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Canon\\eosd 60";
static JPGCLI_TEST_SET_T _ar_eosd_60_Set[] =
{
	{"0", 2286333, "020316-1133-20.jpg"},
	//{"1", 1973436, "020316-1136-22_rt8.jpg"}, //E_JPG_ERR_MULTI_APP1 (-17)
	//{"2", 2117955, "020316-1143-37_rt8.jpg"}, //E_JPG_ERR_MULTI_APP1 (-17)
	{"3", 1922021, "st020315-1752-16.jpg"},
	{"4", 1333260, "st020315-1804-54.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_G1_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Canon\\G1";
static JPGCLI_TEST_SET_T _ar_G1_Set[] =
{
	{"0", 656288, "000926-1352-12.jpg"},
	{"1", 2431064, "000927-1103-27.jpg"},
	{"2", 1625356, "000927-1105-00.jpg"},
	{"3", 2124126, "001001-1327-43.jpg"},
	{"4", 2120077, "001001-1330-26.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_G2_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Canon\\G2";
static JPGCLI_TEST_SET_T _ar_G2_Set[] =
{
	{"0", 2043630, "010814-1615-33.jpg"},
	{"1", 2328534, "010814-1627-59.jpg"},
	{"2", 2008826, "010814-1628-16.jpg"},
	{"3", 2127274, "010816-1742-21.jpg"},
	{"4", 2116106, "010816-1757-02.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_G3_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Canon\\G3";
static JPGCLI_TEST_SET_T _ar_G3_Set[] =
{
	{"0", 1397489, "021104-1109-10.jpg"},
	{"1", 1294002, "021104-1112-02.jpg"},
	{"2", 1619280, "021104-1150-12.jpg"},
	{"3", 1958882, "021203-1742-04.jpg"},
	{"4", 1696372, "021203-1744-32.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_G5_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Canon\\G5";
static JPGCLI_TEST_SET_T _ar_G5_Set[] =
{
	{"0", 1810459, "030316-1009-21.jpg"},
	{"1", 1841290, "030316-1010-43.jpg"},
	{"2", 2740032, "030317-0832-45.jpg"},
	{"3", 3137969, "030328-1201-42.jpg"},
	{"4", 3621476, "030328-1202-02.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_G6_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Canon\\G6";
static JPGCLI_TEST_SET_T _ar_G6_Set[] =
{
	{"0", 2612649, "040710-1805-58.jpg"},
	{"1", 4306187, "040714-1440-08.jpg"},
	{"2", 3332658, "040714-1520-02.jpg"},
	{"3", 2904997, "040722-2047-23.jpg"},
	{"4", 3766727, "040728-1643-57.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_powershot_a20_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Canon\\powershot a20";
static JPGCLI_TEST_SET_T _ar_powershot_a20_Set[] =
{
	{"0", 528032, "010421-1106-44.jpg"},
	{"1", 1269648, "010421-1132-19.jpg"},
	{"2", 1133042, "010421-1135-53.jpg"},
	{"3", 835171, "010421-1920-12.jpg"},
	{"4", 878197, "010421-2122-28.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_powershot_a510_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Canon\\powershot a510";
static JPGCLI_TEST_SET_T _ar_powershot_a510_Set[] =
{
	{"0", 1087496, "img_0596.jpg"},
	{"1", 2140166, "img_0599.jpg"},
	{"2", 1868835, "img_0761.jpg"},
	{"3", 1927026, "img_0767.jpg"},
	{"4", 2391210, "img_1000.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_powershot_a520_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Canon\\powershot a520";
static JPGCLI_TEST_SET_T _ar_powershot_a520_Set[] =
{
	{"0", 958721, "img_0259.jpg"},
	{"1", 1003118, "img_0266.jpg"},
	{"2", 934788, "img_0923.jpg"},
	{"3", 578353, "img_0934.jpg"},
	{"4", 887079, "img_0935.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_powershot_a70_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Canon\\powershot a70";
static JPGCLI_TEST_SET_T _ar_powershot_a70_Set[] =
{
	{"0", 1211151, "030713-1129-28.jpg"},
	{"1", 1547204, "030728-0954-11.jpg"},
	{"2", 1600611, "030728-0958-42.jpg"},
	{"3", 1976887, "030728-1013-55.jpg"},
	{"4", 1508853, "030728-1015-54.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_powershot_a95_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Canon\\powershot a95";
static JPGCLI_TEST_SET_T _ar_powershot_a95_Set[] =
{
	{"0", 921114, "img_8468.jpg"},
	{"1", 789742, "img_8472.jpg"},
	{"2", 1457238, "img_8854.jpg"},
	{"3", 1299557, "img_9032.jpg"},
	{"4", 1497986, "img_9522.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_pro1_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Canon\\pro1";
static JPGCLI_TEST_SET_T _ar_pro1_Set[] =
{
	{"0", 3955889, "040330-1044-07.jpg"},
	{"1", 3969180, "040330-1048-13.jpg"},
	{"2", 4354778, "040330-1050-53.jpg"},
	{"3", 4344215, "040331-1335-33.jpg"},
	{"4", 4652118, "040331-1341-16.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_pro90_IS_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Canon\\pro90 IS";
static JPGCLI_TEST_SET_T _ar_pro90_IS_Set[] =
{
	{"0", 1530260, "001223-0843-43.jpg"},
	{"1", 1320149, "001223-1241-47.jpg"},
	{"2", 1226622, "001223-1636-25.jpg"},
	{"3", 1357335, "001228-1602-10.jpg"},
	{"4", 1439853, "010101-0016-12.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_s_400_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Canon\\s 400";
static JPGCLI_TEST_SET_T _ar_s_400_Set[] =
{
	{"0", 921897, "030225-1342-29.jpg"},
	{"1", 1590014, "030225-1346-18.jpg"},
	{"2", 1528488, "030305-0213-50.jpg"},
	{"3", 1340618, "030329-1544-54.jpg"},
	{"4", 1304053, "030329-1545-43.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_S1_IS_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Canon\\S1 IS";
static JPGCLI_TEST_SET_T _ar_S1_IS_Set[] =
{
	{"0", 1843072, "img_0021.jpg"},
	{"1", 1589958, "img_0024.jpg"},
	{"2", 1491162, "img_0057.jpg"},
	{"3", 1246349, "img_7772.jpg"},
	{"4", 1525379, "img_7773.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_s10_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Canon\\s10";
static JPGCLI_TEST_SET_T _ar_s10_Set[] =
{
	{"0", 578093, "991022-2113-57.jpg"},
	{"1", 702963, "991023-0754-23.jpg"},
	{"2", 351543, "991023-1431-36.jpg"},
	{"3", 598733, "991031-1807-18.jpg"},
	{"4", 1617453, "991103-2153-55.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_s20_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Canon\\s20";
static JPGCLI_TEST_SET_T _ar_s20_Set[] =
{
	{"0", 1132037, "000112-1522-54.jpg"},
	{"1", 1025520, "000112-1533-34.jpg"},
	{"2", 1196680, "000112-1534-01.jpg"},
	{"3", 893639, "000206-1558-02.jpg"},
	{"4", 638582, "000206-1608-18.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_s40_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Canon\\s40";
static JPGCLI_TEST_SET_T _ar_s40_Set[] =
{
	{"0", 1660369, "011003-1020-15.jpg"},
	{"1", 1860538, "011003-1020-44.jpg"},
	{"2", 2459676, "011003-1027-35.jpg"},
	{"3", 1789868, "011003-1525-31.jpg"},
	{"4", 2050300, "img_6110.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_s45_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Canon\\s45";
static JPGCLI_TEST_SET_T _ar_s45_Set[] =
{
	{"0", 1677104, "021017-1034-18.jpg"},
	{"1", 1691447, "021017-1040-42.jpg"},
	{"2", 1767780, "021017-1042-42.jpg"},
	{"3", 1807345, "021107-1127-18.jpg"},
	{"4", 2318889, "021107-1158-35.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_s50_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Canon\\s50";
static JPGCLI_TEST_SET_T _ar_s50_Set[] =
{
	{"0", 2443300, "030412-1336-13.jpg"},
	{"1", 2469805, "030412-1354-32.jpg"},
	{"2", 2427930, "030412-1357-55.jpg"},
	{"3", 2710884, "030728-1146-04.jpg"},
	{"4", 2453606, "030728-1153-38.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_s60_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Canon\\s60";
static JPGCLI_TEST_SET_T _ar_s60_Set[] =
{
	{"0", 2603234, "040716-1202-17.jpg"},
	{"1", 2521452, "040716-1204-00.jpg"},
	{"2", 2038200, "040716-1354-22.jpg"},
	{"3", 2850530, "040808-1003-13.jpg"},
	{"4", 3183783, "040808-1012-24.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_s70_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Canon\\s70";
static JPGCLI_TEST_SET_T _ar_s70_Set[] =
{
	{"0", 2698748, "img_0140.jpg"},
	{"1", 2310964, "img_0151.jpg"},
	{"2", 3229948, "img_0160.jpg"},
	{"3", 3298930, "img_8846.jpg"},
	{"4", 2872028, "img_8853.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_sd_300_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Canon\\sd 300";
static JPGCLI_TEST_SET_T _ar_sd_300_Set[] =
{
	{"0", 1950312, "cj_img_0213.jpg"},
	{"1", 1055073, "cj_img_0224.jpg"},
	{"2", 973722, "cj_img_0236.jpg"},
	{"3", 2591711, "sk_img_0141.jpg"},
	{"4", 3507038, "sk_img_0149.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_sd_500_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Canon\\sd 500";
static JPGCLI_TEST_SET_T _ar_sd_500_Set[] =
{
	{"0", 3061374, "img_0390.jpg"},
	{"1", 2661401, "img_0393.jpg"},
	{"2", 1914850, "img_0409.jpg"},
	{"3", 4757564, "img_1092.jpg"},
	{"4", 4734026, "img_1093.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_sd100_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Canon\\sd100";
static JPGCLI_TEST_SET_T _ar_sd100_Set[] =
{
	{"0", 1423012, "030417-1117-27.jpg"},
	{"1", 1521185, "030417-1123-05.jpg"},
	{"2", 1642753, "030417-1145-22.jpg"},
	{"3", 1043393, "030428-2017-40.jpg"},
	{"4", 1750913, "030501-1108-57.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_EX_P505_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Casio\\EX-P505";
static JPGCLI_TEST_SET_T _ar_EX_P505_Set[] =
{
	{"0", 1944497, "CIMG0001.JPG"},
	{"1", 1979425, "CIMG0005.JPG"},
	{"2", 1941139, "CIMG0015.JPG"},
	{"3", 1947851, "CIMG0025.JPG"},
	{"4", 1960426, "CIMG0036.JPG"},
	{NULL, NULL, NULL}
};

static CHAR _sz_EX_P700_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Casio\\EX-P700";
static JPGCLI_TEST_SET_T _ar_EX_P700_Set[] =
{
	{"0", 4442153, "cimg0036.jpg"},
	{"1", 3499923, "cimg0039.jpg"},
	{"2", 4629535, "cimg0047.jpg"},
	{"3", 4439379, "cimg0132.jpg"},
	{"4", 4496678, "cimg0139.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_EX_S3_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Casio\\EX-S3";
static JPGCLI_TEST_SET_T _ar_EX_S3_Set[] =
{
	{"0", 1667321, "CIMG0164.JPG"},
	{"1", 1615299, "CIMG0169.JPG"},
	{"2", 1667235, "CIMG0170.JPG"},
	{NULL, NULL, NULL}
};

static CHAR _sz_EX_Z3_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Casio\\EX-Z3";
static JPGCLI_TEST_SET_T _ar_EX_Z3_Set[] =
{
	{"0", 1220377, "030412-1328-15.jpg"},
	{"1", 1177210, "030412-1332-25.jpg"},
	{"2", 1201122, "030412-1335-36.jpg"},
	{"3", 1686623, "030417-1214-32.jpg"},
	{"4", 1652268, "030501-1014-13.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_EX_Z750_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Casio\\EX-Z750";
static JPGCLI_TEST_SET_T _ar_EX_Z750_Set[] =
{
	{"0", 4515215, "CIMG0019.jpg"},
	{"1", 4566711, "CIMG0045.JPG"},
	{"2", 4384194, "CIMG0050.JPG"},
	{"3", 4539555, "CIMG0052.JPG"},
	{"4", 4369419, "CIMG0118.JPG"},
	{NULL, NULL, NULL}
};

static CHAR _sz_GV_10_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Casio\\GV-10";
static JPGCLI_TEST_SET_T _ar_GV_10_Set[] =
{
	{"0", 505245, "05010001.JPG"},
	{"1", 455370, "05010010.JPG"},
	{"2", 511770, "05010012.JPG"},
	{NULL, NULL, NULL}
};

static CHAR _sz_QV_3000_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Casio\\QV-3000";
static JPGCLI_TEST_SET_T _ar_QV_3000_Set[] =
{
	{"0", 893850, "000331-1009-53.jpg"},
	{"1", 1391102, "000331-1013-18.jpg"},
	{"2", 908948, "000406-1440-40.jpg"},
	{"3", 987303, "000406-1712-21.jpg"},
	{"4", 933876, "000406-1747-42.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_QV_4000_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Casio\\QV-4000";
static JPGCLI_TEST_SET_T _ar_QV_4000_Set[] =
{
	{"0", 1573705, "020928-1525-01.jpg"},
	{"1", 1495248, "020928-1527-34.jpg"},
	{"2", 1275651, "020928-1620-57.jpg"},
	{"3", 1561631, "020928-1629-51.jpg"},
	{"4", 1272629, "11040070.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_QV_8000SX_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Casio\\QV-8000SX";
static JPGCLI_TEST_SET_T _ar_QV_8000SX_Set[] =
{
	{"0", 431925, "08200005.jpg"},
	{"1", 574077, "08200006.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_QV_R4_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Casio\\QV-R4";
static JPGCLI_TEST_SET_T _ar_QV_R4_Set[] =
{
	{"0", 1843623, "CIMG0013.JPG"},
	{"1", 1837242, "CIMG0014.JPG"},
	{"2", 1884600, "CIMG0021.JPG"},
	{NULL, NULL, NULL}
};

static CHAR _sz_QV_R51_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Casio\\QV-R51";
static JPGCLI_TEST_SET_T _ar_QV_R51_Set[] =
{
	{"0", 2253206, "CIMG0130.JPG"},
	{"1", 2284199, "CIMG0134.JPG"},
	{"2", 2213196, "CIMG0141.JPG"},
	{"3", 2212632, "CIMG0152.JPG"},
	{NULL, NULL, NULL}
};

static CHAR _sz_TVS_digital_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Contax\\TVS digital";
static JPGCLI_TEST_SET_T _ar_TVS_digital_Set[] =
{
	{"0", 2010037, "KICX0046.JPG"},
	{"1", 1930692, "KICX0055.JPG"},
	{"2", 2032750, "KICX0058.JPG"},
	{NULL, NULL, NULL}
};

static CHAR _sz_photoPC_3000Z_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Epson\\photoPC 3000Z";
static JPGCLI_TEST_SET_T _ar_photoPC_3000Z_Set[] =
{
	{"0", 744723, "000712-1433-42.jpg"},
	{"1", 764410, "000713-1541-55.jpg"},
	{"2", 723426, "000713-1545-38.jpg"},
	{"3", 797076, "000801-1543-28.jpg"},
	{"4", 786578, "000801-1545-49.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_1500_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Fuji\\1500";
static JPGCLI_TEST_SET_T _ar_1500_Set[] =
{
	{"0", 382122, "DSCF0019.JPG"},
	{"1", 366293, "DSCF0020.JPG"},
	{"2", 354180, "DSCF0021.JPG"},
	{"3", 380262, "DSCF0022.JPG"},
	{"4", 381188, "DSCF0023.JPG"},
	{"5", 400265, "DSCF0024.JPG"},
	{"6", 376283, "DSCF0025.JPG"},
	{NULL, NULL, NULL}
};

static CHAR _sz_3800_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Fuji\\3800";
static JPGCLI_TEST_SET_T _ar_3800_Set[] =
{
	{"0", 1307507, "DSCF0010.JPG"},
	{"1", 1342148, "DSCF0011.JPG"},
	{"2", 1230807, "DSCF0105.JPG"},
	{"3", 1260780, "DSCF0110.JPG"},
	{NULL, NULL, NULL}
};

static CHAR _sz_A310_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Fuji\\A310";
static JPGCLI_TEST_SET_T _ar_A310_Set[] =
{
	{"0", 642332, "DSCF0045.JPG"},
	{"1", 691599, "DSCF0052.JPG"},
	{"2", 733004, "DSCF0058.JPG"},
	{NULL, NULL, NULL}
};

static CHAR _sz_E550_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Fuji\\E550";
static JPGCLI_TEST_SET_T _ar_E550_Set[] =
{
	{"0", 1474087, "dscf0026.jpg"},
	{"1", 1622733, "dscf0030.jpg"},
	{"2", 756311, "dscf0035.jpg"},
	{"3", 4627902, "dscf0858.jpg"},
	{"4", 4348818, "dscf0862.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_F10_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Fuji\\F10";
static JPGCLI_TEST_SET_T _ar_F10_Set[] =
{
	{"0", 2882752, "dscf0139.jpg"},
	{"1", 3199682, "dscf0156.jpg"},
	{"2", 3135876, "dscf0164.jpg"},
	{"3", 2951104, "sk_dscf0046.jpg"},
	{"4", 2896798, "sk_dscf0060.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_FinePix_40i_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Fuji\\FinePix 40i";
static JPGCLI_TEST_SET_T _ar_FinePix_40i_Set[] =
{
	{"0", 584208, "000726-1907-25.jpg"},
	{"1", 1702669, "000726-1908-20.jpg"},
	{"2", 1712057, "000726-1908-35.jpg"},
	{"3", 713939, "000812-1444-54.jpg"},
	{"4", 106265, "000812-1444-54[1].jpg"},
	{"5", 732603, "000812-1449-36.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_FinePix_4700z_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Fuji\\FinePix 4700z";
static JPGCLI_TEST_SET_T _ar_FinePix_4700z_Set[] =
{
	{"0", 783246, "000318-1111-54.jpg"},
	{"1", 781390, "000318-1113-07.jpg"},
	{"2", 788451, "000318-1126-33.jpg"},
	{"3", 775800, "000327-1024-21.jpg"},
	{"4", 777159, "000327-1027-16.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_FinePix_4900z_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Fuji\\FinePix 4900z";
static JPGCLI_TEST_SET_T _ar_FinePix_4900z_Set[] =
{
	{"0", 1674926, "000927-1117-35.jpg"},
	{"1", 344515, "000927-1132-09.jpg"},
	{"2", 735686, "000927-1134-56.jpg"},
	{"3", 761563, "001008-1431-20.jpg"},
	{"4", 743653, "001008-1434-02.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_FinePix_6800z_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Fuji\\FinePix 6800z";
static JPGCLI_TEST_SET_T _ar_FinePix_6800z_Set[] =
{
	{"0", 1099637, "010330-1127-19.jpg"},
	{"1", 1100196, "010330-1138-35.jpg"},
	{"2", 1054401, "010330-1150-31.jpg"},
	{"3", 1413071, "010402-1151-24.jpg"},
	{"4", 2360583, "010402-1203-56.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_FinePix_6900z_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Fuji\\FinePix 6900z";
static JPGCLI_TEST_SET_T _ar_FinePix_6900z_Set[] =
{
	{"0", 2502057, "010618-1021-06.jpg"},
	{"1", 2346050, "010618-1032-18.jpg"},
	{"2", 2296661, "010618-1032-28.jpg"},
	{"3", 2408411, "dscf0692.jpg"},
	{"4", 2382472, "dscf0695.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_FinePix_E550_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Fuji\\FinePix E550";
static JPGCLI_TEST_SET_T _ar_FinePix_E550_Set[] =
{
	{NULL, NULL, NULL}
};

static CHAR _sz_FinePix_F601Z_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Fuji\\FinePix F601Z";
static JPGCLI_TEST_SET_T _ar_FinePix_F601Z_Set[] =
{
	{"0", 2384850, "020515-1455-39.jpg"},
	{"1", 2392306, "020515-1458-39.jpg"},
	{"2", 2313759, "020515-1514-12.jpg"},
	{"3", 2429453, "020617-1603-59.jpg"},
	{"4", 2341071, "020617-1607-14.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_FinePix_F700_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Fuji\\FinePix F700";
static JPGCLI_TEST_SET_T _ar_FinePix_F700_Set[] =
{
	{"0", 1470356, "030809-1445-25.jpg"},
	{"1", 1392642, "030809-1453-51.jpg"},
	{"2", 1362496, "030814-1427-07.jpg"},
	{"3", 777176, "030826-1404-09.jpg"},
	{"4", 783102, "030826-1406-22.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_FinePix_F810_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Fuji\\FinePix F810";
static JPGCLI_TEST_SET_T _ar_FinePix_F810_Set[] =
{
	{"0", 713121, "dscf0081.jpg"},
	{"1", 4276959, "dscf0152.jpg"},
	{"2", 4506060, "dscf0153.jpg"},
	{"3", 1662134, "dscf0555.jpg"},
	{"4", 1596242, "dscf0559.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_MX_2700_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Fuji\\MX-2700";
static JPGCLI_TEST_SET_T _ar_MX_2700_Set[] =
{
	{"0", 890515, "990701-0837-43.jpg"},
	{"1", 452589, "990701-1114-56.jpg"},
	{"2", 459120, "990701-1117-03.jpg"},
	{"3", 439884, "990704-1640-01.jpg"},
	{"4", 434660, "990704-1641-20.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_MX_2900_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Fuji\\MX-2900";
static JPGCLI_TEST_SET_T _ar_MX_2900_Set[] =
{
	{"0", 427075, "990915-1703-20.jpg"},
	{"2", 463859, "990915-1727-27.jpg"},
	{"3", 425772, "990918-1301-16.jpg"},
	{"4", 436688, "990918-1309-34.jpg"},
	{"5", 451121, "990918-1319-12.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_S1_pro_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Fuji\\S1 pro";
static JPGCLI_TEST_SET_T _ar_S1_pro_Set[] =
{
	{"0", 2606096, "000805-1113-42.jpg"},
	{"1", 2534088, "000805-1125-05.jpg"},
	{"2", 2275901, "000805-1125-57.jpg"},
	{"3", 2453075, "000805-1126-44.jpg"},
	{"4", 2471287, "000806-2200-06.jpg"},
	{"5", 2515013, "000806-2227-15.jpg"},
	{"6", 2414073, "000806-2229-11.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_S2_pro_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Fuji\\S2 pro";
static JPGCLI_TEST_SET_T _ar_S2_pro_Set[] =
{
	{"0", 4885975, "020627-1231-04.jpg"},
	{"1", 4466776, "020628-0801-46.jpg"},
	{"2", 5360302, "020628-1004-31.jpg"},
	//{"3", 1232544, "020731-1114-20-rawa.jpg"}, //E_JPG_ERR_MULTI_APP1 (-17)
	//{"4", 1147765, "020731-1117-14-rawa.jpg"}, //E_JPG_ERR_MULTI_APP1 (-17)
	{NULL, NULL, NULL}
};

static CHAR _sz_S3_pro_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Fuji\\S3 pro";
static JPGCLI_TEST_SET_T _ar_S3_pro_Set[] =
{
	{"0", 2855167, "dscf0338.jpg"},
	{"1", 2811603, "dscf0372.jpg"},
	{"2", 2988054, "dscf0375.jpg"},
	{"3", 4548259, "dscf0979.jpg"},
	{"4", 4353466, "dscf1005.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_S5000Z_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Fuji\\S5000Z";
static JPGCLI_TEST_SET_T _ar_S5000Z_Set[] =
{
	{"0", 1535236, "030809-1440-38.jpg"},
	{"1", 1543220, "030809-1446-08.jpg"},
	{"2", 1491216, "030809-1447-17.jpg"},
	{"3", 1526776, "030826-1447-49.jpg"},
	{"4", 1453481, "030826-1510-51.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_S5100Z_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Fuji\\S5100Z";
static JPGCLI_TEST_SET_T _ar_S5100Z_Set[] =
{
	{"0", 1818956, "cj_dscf0533.jpg"},
	{"1", 1902709, "cj_dscf0536.jpg"},
	{"2", 1945613, "dscf0027.jpg"},
	{"3", 1849791, "sj_dscf0522.jpg"},
	{"4", 2034575, "sj_dscf0524.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_S602Z_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Fuji\\S602Z";
static JPGCLI_TEST_SET_T _ar_S602Z_Set[] =
{
	{"0", 2381093, "020416-0952-23.jpg"},
	{"1", 2332577, "020416-0955-06.jpg"},
	{"2", 2351647, "020416-1000-33.jpg"},
	{"3", 2159710, "020416-1546-20.jpg"},
	{"4", 2353262, "020416-1550-08.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_S7000Z_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Fuji\\S7000Z";
static JPGCLI_TEST_SET_T _ar_S7000Z_Set[] =
{
	{"0", 4415236, "031128-1406-16.jpg"},
	{"1", 4357747, "031128-1414-32.jpg"},
	{"2", 4559263, "031128-1420-21.jpg"},
	{"3", 1502387, "031214-1346-31.jpg"},
	{"4", 1488965, "031214-1349-00.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_812_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\HP\\812";
static JPGCLI_TEST_SET_T _ar_812_Set[] =
{
	{"0", 1282434, "020807-1037-56.jpg"},
	{"1", 1492950, "020807-1038-32.jpg"},
	{"2", 910388, "020807-1048-34.jpg"},
	{"3", 1540595, "020807-1120-15.jpg"},
	{"4", 1152878, "020807-1123-28.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_850_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\HP\\850";
static JPGCLI_TEST_SET_T _ar_850_Set[] =
{
	{"0", 1980490, "021203-1752-56.jpg"},
	{"1", 2175818, "021212-1203-46.jpg"},
	{"2", 1899319, "021212-1208-59.jpg"},
	{"3", 2361678, "030104-1330-06.jpg"},
	{"4", 2167110, "030104-1331-02.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_935_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\HP\\935";
static JPGCLI_TEST_SET_T _ar_935_Set[] =
{
	{"0", 2417984, "030713-1231-48.jpg"},
	{"1", 2308028, "030713-1232-58.jpg"},
	{"2", 2554889, "030728-1052-55.jpg"},
	{"3", 3386597, "030728-1147-12.jpg"},
	{"4", 2555353, "030728-1157-12.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_C500_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\HP\\C500";
static JPGCLI_TEST_SET_T _ar_C500_Set[] =
{
	{"0", 483386, "IM000016.JPG"},
	{"1", 341832, "IM000021.JPG"},
	{"2", 192291, "IM000036.JPG"},
	{"3", 192329, "IM000039.JPG"},
	{NULL, NULL, NULL}
};

static CHAR _sz_R707_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\HP\\R707";
static JPGCLI_TEST_SET_T _ar_R707_Set[] =
{
	{"0", 1351608, "hpim0044.jpg"},
	{"1", 2165854, "hpim0050.jpg"},
	{"2", 1935934, "hpim0107.jpg"},
	{"3", 2441132, "hpim0124.jpg"},
	{"4", 3081920, "hpim0625.jpg"},
	{"5", 2044094, "hpim0631.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_DC265_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\kodak\\DC265";
static JPGCLI_TEST_SET_T _ar_DC265_Set[] =
{
	{"0", 265326, "990514-2147-25.jpg"},
	{"1", 304667, "990514-2148-40.jpg"},
	{"2", 260694, "990515-1308-35.jpg"},
	{"3", 262565, "990522-1623-54.jpg"},
	{"4", 504328, "990522-1626-14.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_DC280_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\kodak\\DC280";
static JPGCLI_TEST_SET_T _ar_DC280_Set[] =
{
	{"0", 324641, "990707-1030-42.jpg"},
	{"1", 512332, "990708-1550-17.jpg"},
	{"2", 269180, "990711-1359-16.jpg"},
	{"3", 230928, "990813-1408-36.jpg"},
	{"4", 353863, "dc280_990708-1532-37.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_DC290_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\kodak\\DC290";
static JPGCLI_TEST_SET_T _ar_DC290_Set[] =
{
	{"0", 764563, "991110-2052-55.jpg"},
	{"1", 464232, "991111-1143-24.jpg"},
	{"2", 602877, "991112-1244-28.jpg"},
	{"3", 456093, "991126-1253-30.jpg"},
	{"4", 390988, "dc290_991126-1500-01.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_DC4800_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\kodak\\DC4800";
static JPGCLI_TEST_SET_T _ar_DC4800_Set[] =
{
	{"0", 496740, "001004-1350-28.jpg"},
	{"1", 538496, "001004-1402-42.jpg"},
	{"2", 714720, "001004-1404-42.jpg"},
	{"3", 752584, "001022-1539-26.jpg"},
	{"4", 594191, "001023-2314-57.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_DCS_pro_14n_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\kodak\\DCS pro 14n";
static JPGCLI_TEST_SET_T _ar_DCS_pro_14n_Set[] =
{
	{"0", 2925869, "kp145201.jpg"},
	{"1", 3878745, "kp145214.jpg"},
	{"2", 3493120, "kp145215.jpg"},
	{"3", 3813867, "kp160152.jpg"},
	{"4", 1664977, "kp160224.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_DCS_pro_SLRc_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\kodak\\DCS pro SLRc";
static JPGCLI_TEST_SET_T _ar_DCS_pro_SLRc_Set[] =
{
	//{"0", 3128485, "f6fm0421-raw-acr.jpg"},  //E_JPG_ERR_MULTI_APP1 (-17)
	//{"1", 4345346, "f6fm0424-raw-pd.jpg"},
	//{"2", 3093583, "f6fm0797-raw-acr.jpg"},
	//{"3", 4971637, "f6fm1107-raw-acr.jpg"},
	//{"4", 2616357, "f6fm1111-raw-pd.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_DCS_pro_SLRn_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\kodak\\DCS pro SLRn";
static JPGCLI_TEST_SET_T _ar_DCS_pro_SLRn_Set[] =
{
	//{"0", 3532608, "g2d20873.jpg"},  //E_JPG_ERR_MULTI_APP1 (-17)
	//{"1", 2961411, "g2d20885.jpg"},
	//{"2", 3042483, "g2d20886.jpg"},
	//{"3", 2855881, "g2d20910.jpg"},
	//{"4", 2631337, "g2d20915.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_DX4900_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\kodak\\DX4900";
static JPGCLI_TEST_SET_T _ar_DX4900_Set[] =
{
	{"0", 903237, "020501-1022-28.jpg"},
	{"1", 851840, "020501-1028-17.jpg"},
	{"2", 873515, "020501-1034-19.jpg"},
	{"3", 867356, "020501-1126-14.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_DX7590_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\kodak\\DX7590";
static JPGCLI_TEST_SET_T _ar_DX7590_Set[] =
{
	{"0", 1135556, "020501-1105-27.jpg"},
	{"1", 1584660, "100_0004.jpg"},
	{"2", 1606149, "100_0058.jpg"},
	{"3", 1375060, "102_0313.jpg"},
	{"4", 1396744, "102_0320.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_LS743_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\kodak\\LS743";
static JPGCLI_TEST_SET_T _ar_LS743_Set[] =
{
	{"0", 1100020, "100_0008.JPG"},
	{"1", 1026332, "100_0009.JPG"},
	{"2", 1231908, "101_0004.JPG"},
	{NULL, NULL, NULL}
};

static CHAR _sz_pro_DCS_520_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\kodak\\pro DCS 520";
static JPGCLI_TEST_SET_T _ar_pro_DCS_520_Set[] =
{
	{"0", 417792, "ahr00941.jpg"},
	{"1", 466944, "ahr00968.jpg"},
	{"2", 548864, "ahr01238.jpg"},
	{"3", 589824, "ahr02112.jpg"},
	{"4", 1064960, "pahr00846.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_pro_DCS_620_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\kodak\\pro DCS 620";
static JPGCLI_TEST_SET_T _ar_pro_DCS_620_Set[] =
{
	{"0", 1122304, "g8bs1291.jpg"},
	{"1", 901120, "g8bs1293.jpg"},
	{"2", 868352, "g8bs1296.jpg"},
	{"3", 483328, "g8bs1374.jpg"},
	{"4", 917504, "g8bs1382.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_pro_DCS_620x_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\kodak\\pro DCS 620x";
static JPGCLI_TEST_SET_T _ar_pro_DCS_620x_Set[] =
{
	{"0", 700366, "783p1365.jpg"},
	{"1", 937859, "783p1369.jpg"},
	{"2", 581635, "783p1373.jpg"},
	{"3", 863584, "783p2537.jpg"},
	{"4", 1116775, "783p2541.jpg"},
	{"5", 146945, "783p2541[1].jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_pro_DCS_760_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\kodak\\pro DCS 760";
static JPGCLI_TEST_SET_T _ar_pro_DCS_760_Set[] =
{
	{"0", 1553028, "8g350089.jpg"},
	{"1", 2074839, "8g350116.jpg"},
	{"2", 1382392, "8g350140.jpg"},
	{"3", 1200408, "8g350949.jpg"},
	{"4", 1902262, "8g350980.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_Z740_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\kodak\\Z740";
static JPGCLI_TEST_SET_T _ar_Z740_Set[] =
{
	{"0", 1468009, "100_0018.JPG"},
	{"1", 1339071, "100_0020.JPG"},
	{"2", 571685, "100_0032.JPG"},
	{NULL, NULL, NULL}
};

static CHAR _sz_Digitial_Revio_KD_400Z_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Konica\\Digitial Revio KD-400Z";
static JPGCLI_TEST_SET_T _ar_Digitial_Revio_KD_400Z_Set[] =
{
	{"0", 1139153, "PICT0008.JPG"},
	{"1", 1824621, "PICT0018.JPG"},
	{"2", 1755049, "PICT0019.JPG"},
	{NULL, NULL, NULL}
};

static CHAR _sz_DiMAGE_5_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Konica\\DiMAGE 5";
static JPGCLI_TEST_SET_T _ar_DiMAGE_5_Set[] =
{
	{"0", 1418664, "010901-1114-24.jpg"},
	{"1", 1934663, "010901-1117-39.jpg"},
	{"2", 1623416, "010901-1119-14.jpg"},
	{"3", 940966, "010922-1412-40.jpg"},
	{"4", 1492024, "010922-1419-06.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_DiMAGE_7_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Konica\\DiMAGE 7";
static JPGCLI_TEST_SET_T _ar_DiMAGE_7_Set[] =
{
	{"0", 1654255, "010628-1705-03.jpg"},
	{"1", 2628013, "010701-1855-08.jpg"},
	{"2", 2416469, "010701-1855-51.jpg"},
	{"3", 2380641, "010701-1906-27.jpg"},
	{"4", 2041761, "010704-1345-39.jpg"},
	{"5", 1649967, "010704-1346-40.jpg"},
	{"6", 1697230, "010708-2052-14.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_DiMAGE_7Hi_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Konica\\DiMAGE 7Hi";
static JPGCLI_TEST_SET_T _ar_DiMAGE_7Hi_Set[] =
{
	{"0", 2599769, "020910-1031-15.jpg"},
	{"1", 2600794, "020910-1037-54.jpg"},
	{"2", 2283815, "020910-1050-21-a.jpg"},
	{"3", 2297448, "021017-1108-36.jpg"},
	{"4", 4196343, "021017-1111-55.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_DiMAGE_7i_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Konica\\DiMAGE 7i";
static JPGCLI_TEST_SET_T _ar_DiMAGE_7i_Set[] =
{
	{"0", 2530593, "020601-1223-26.jpg"},
	{"1", 2292096, "020601-1241-34.jpg"},
	{"2", 2162912, "020601-1244-42.jpg"},
	{"3", 1993784, "020617-1545-48.jpg"},
	{"4", 1750336, "020617-1553-39.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_DiMAGE_A1_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Konica\\DiMAGE A1";
static JPGCLI_TEST_SET_T _ar_DiMAGE_A1_Set[] =
{
	{"0", 2305884, "030913-1610-42.jpg"},
	{"1", 2707040, "030922-1030-56.jpg"},
	{"2", 2561178, "030922-1031-39.jpg"},
	{"3", 2240852, "030923-2038-48-a.jpg"},
	{"4", 2065661, "030923-2132-18-a.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_DiMAGE_EX_1500_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Konica\\DiMAGE EX 1500";
static JPGCLI_TEST_SET_T _ar_DiMAGE_EX_1500_Set[] =
{
	{"0", 368546, "010299-171136.jpg"},
	{"1", 458981, "010299-232753.jpg"},
	{"2", 519331, "260199-152451.jpg"},
	{"3", 453900, "310199-133251.jpg"},
	{"4", 515687, "310199-133329.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_DiMAGE_F100_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Konica\\DiMAGE F100";
static JPGCLI_TEST_SET_T _ar_DiMAGE_F100_Set[] =
{
	{"0", 1308882, "020515-1530-00.jpg"},
	{"1", 1245902, "020515-1538-26.jpg"},
	{"2", 1403770, "020811-1358-55.jpg"},
	{"3", 1349022, "020822-1549-59.jpg"},
	{"4", 1232868, "020822-1601-01.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_DiMAGE_G500_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Konica\\DiMAGE G500";
static JPGCLI_TEST_SET_T _ar_DiMAGE_G500_Set[] =
{
	{"0", 1937072, "PICT0074.JPG"},
	{"1", 2331261, "PICT0079.JPG"},
	{"2", 1989514, "PICT0084.JPG"},
	{NULL, NULL, NULL}
};

static CHAR _sz_DiMAGE_S304_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Konica\\DiMAGE S304";
static JPGCLI_TEST_SET_T _ar_DiMAGE_S304_Set[] =
{
	{"0", 1764701, "010901-1131-12.jpg"},
	{"1", 1621739, "010901-1157-42.jpg"},
	{"2", 1771020, "010901-1210-50.jpg"},
	{"3", 1543285, "010904-1252-20r.jpg"},
	{"4", 1739795, "010904-1252-50r.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_DiMAGE_S404_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Konica\\DiMAGE S404";
static JPGCLI_TEST_SET_T _ar_DiMAGE_S404_Set[] =
{
	{"0", 1735637, "020403-1139-05.jpg"},
	{"1", 1737445, "020403-1143-04.jpg"},
	{"2", 1310755, "020403-1157-52.jpg"},
	{"3", 1857112, "020403-1240-30.jpg"},
	{"4", 1085056, "020408-1503-15.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_DiMAGE_X_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Konica\\DiMAGE X";
static JPGCLI_TEST_SET_T _ar_DiMAGE_X_Set[] =
{
	{"0", 668151, "pict0018.jpg"},
	{"1", 766847, "pict0020.jpg"},
	{"2", 772720, "pict0028.jpg"},
	{"3", 685139, "pict0047.jpg"},
	{"4", 717689, "pict0053.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_Minolta_7D_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Konica\\Minolta 7D";
static JPGCLI_TEST_SET_T _ar_Minolta_7D_Set[] =
{
	{"0", 2748042, "pict0029.jpg"},
	{"1", 2873927, "pict0078.jpg"},
	{"2", 1575138, "pict0481-master.jpg"},
	//{"3", 1461842, "pict0694-acr.jpg"},  //E_JPG_ERR_MULTI_APP1 (-17)
	{"4", 2918874, "pict0704.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_Minolta_7D_B_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Konica\\Minolta 7D B";
static JPGCLI_TEST_SET_T _ar_Minolta_7D_B_Set[] =
{
	{"0", 2174472, "pict0012.jpg"},
	{"1", 2594222, "pict0014.jpg"},
	{"2", 2149309, "pict0047.jpg"},
	{"3", 2344011, "pict0233.jpg"},
	{"4", 2274423, "pict0242.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_Minolta_A2_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Konica\\Minolta A2";
static JPGCLI_TEST_SET_T _ar_Minolta_A2_Set[] =
{
	{"0", 3151844, "040331-1113-46.jpg"},
	{"1", 3066653, "040331-1114-51.jpg"},
	{"2", 2847060, "040331-1122-54.jpg"},
	{"3", 4593071, "040331-1300-45.jpg"},
	{"4", 4082372, "040331-1302-06.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_Minolta_A200_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Konica\\Minolta A200";
static JPGCLI_TEST_SET_T _ar_Minolta_A200_Set[] =
{
	{"0", 2145382, "pict0059.jpg"},
	{"1", 2077758, "pict0084.jpg"},
	{"2", 1966214, "pict0108.jpg"},
	{"3", 1899917, "sj_311204_kma200_107.jpg"},
	{"4", 1893817, "sj_pict0016.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_Minolta_Z2_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Konica\\Minolta Z2";
static JPGCLI_TEST_SET_T _ar_Minolta_Z2_Set[] =
{
	{"0", 1270754, "040511-2352-41.jpg"},
	{"1", 1234136, "040511-2358-08.jpg"},
	{"2", 1251269, "040512-0017-38.jpg"},
	{"3", 1436642, "pict0214.jpg"},
	{"4", 1224866, "pict0221.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_Minolta_Z5_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Konica\\Minolta Z5";
static JPGCLI_TEST_SET_T _ar_Minolta_Z5_Set[] =
{
	{"0", 1875725, "pa_pict0002.jpg"},
	{"1", 2497359, "pict0028.jpg"},
	{"2", 3056154, "pict0041.jpg"},
	{"3", 3249352, "sj_19-03_pict0028.jpg"},
	{"4", 1987461, "sj_19-03_pict0047.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_FINEcam_L3v_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Kyocera\\FINEcam L3v";
static JPGCLI_TEST_SET_T _ar_FINEcam_L3v_Set[] =
{
	{"0", 1324420, "KIF_0032.JPG"},
	{"1", 1294610, "KIF_0034.JPG"},
	{"2", 1352832, "KIF_0035.JPG"},
	{NULL, NULL, NULL}
};

static CHAR _sz_FINEcam_M410R_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Kyocera\\FINEcam M410R";
static JPGCLI_TEST_SET_T _ar_FINEcam_M410R_Set[] =
{
	{"0", 1729536, "KIF_0007.JPG"},
	{"1", 1357824, "KIF_0013.JPG"},
	{"2", 1108992, "KIF_0015.JPG"},
	{"3", 1061376, "KIF_0022.JPG"},
	{NULL, NULL, NULL}
};

static CHAR _sz_Finecam_s3_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Kyocera\\Finecam s3";
static JPGCLI_TEST_SET_T _ar_Finecam_s3_Set[] =
{
	{"0", 875945, "010711-1103-45.jpg"},
	{"1", 842281, "010711-1106-24.jpg"},
	{"2", 785133, "010711-1107-30.jpg"},
	{"3", 767024, "kif_0030.jpg"},
	{"4", 741715, "kif_0033.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_FINEcam_SL300R_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Kyocera\\FINEcam SL300R";
static JPGCLI_TEST_SET_T _ar_FINEcam_SL300R_Set[] =
{
	{"0", 1367336, "KIF_0116.JPG"},
	{"1", 1318733, "KIF_0126.JPG"},
	{"2", 1531459, "KIF_0164.JPG"},
	{"3", 1583878, "KIF_0167.JPG"},
	{NULL, NULL, NULL}
};

static CHAR _sz_Digilux_2_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Leica\\Digilux 2";
static JPGCLI_TEST_SET_T _ar_Digilux_2_Set[] =
{
	{"0", 3838899, "040419-1102-48.jpg"},
	{"1", 3595661, "040419-1107-32.jpg"},
	{"2", 3569361, "040419-1111-18.jpg"},
	{"3", 3607190, "040423-1337-54.jpg"},
	{"4", 3903150, "040423-1341-22.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_Coolpix_2500_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Nikon\\Coolpix 2500";
static JPGCLI_TEST_SET_T _ar_Coolpix_2500_Set[] =
{
	{"0", 633230, "020228-1045-24.jpg"},
	{"1", 751405, "020228-1658-23.jpg"},
	{"2", 691150, "020307-0650-12.jpg"},
	{"3", 669280, "020307-0705-35.jpg"},
	{"4", 597554, "020307-0723-59.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_Coolpix_3100_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Nikon\\Coolpix 3100";
static JPGCLI_TEST_SET_T _ar_Coolpix_3100_Set[] =
{
	{"0", 1043914, "030713-1224-13.jpg"},
	{"1", 938679, "030728-0834-04.jpg"},
	{"2", 969551, "030728-0839-00.jpg"},
	{"3", 752984, "030809-1128-38.jpg"},
	{"4", 726377, "030809-1132-39.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_Coolpix_4500_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Nikon\\Coolpix 4500";
static JPGCLI_TEST_SET_T _ar_Coolpix_4500_Set[] =
{
	{"0", 1229279, "020727-1313-43.jpg"},
	{"1", 1353242, "020727-1323-37.jpg"},
	{"2", 1221828, "020727-1501-41.jpg"},
	{"3", 975873, "020814-1309-01.jpg"},
	{"4", 1438325, "020814-1326-19.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_Coolpix_4800_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Nikon\\Coolpix 4800";
static JPGCLI_TEST_SET_T _ar_Coolpix_4800_Set[] =
{
	{"0", 843466, "dscn0044.jpg"},
	{"1", 789194, "dscn0109.jpg"},
	{"2", 779679, "dscn0184.jpg"},
	{"3", 1484632, "dscn0822.jpg"},
	{"4", 1474345, "dscn0904.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_Coolpix_5000_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Nikon\\Coolpix 5000";
static JPGCLI_TEST_SET_T _ar_Coolpix_5000_Set[] =
{
	{"0", 1647721, "020104-1254-22.jpg"},
	{"1", 1716577, "020104-1257-45.jpg"},
	{"2", 1618305, "020118-0942-22.jpg"},
	{"3", 1857860, "020119-1213-49.jpg"},
	{"4", 1807108, "020119-1217-31.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_Coolpix_5200_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Nikon\\Coolpix 5200";
static JPGCLI_TEST_SET_T _ar_Coolpix_5200_Set[] =
{
	{"0", 1807399, "dscn0637.jpg"},
	{"1", 1624149, "dscn0644.jpg"},
	{"2", 1687838, "dscn0653.jpg"},
	{"3", 1561057, "dscn0910.jpg"},
	{"4", 1810984, "dscn0914.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_Coolpix_5400_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Nikon\\Coolpix 5400";
static JPGCLI_TEST_SET_T _ar_Coolpix_5400_Set[] =
{
	{"0", 2452213, "030624-1128-44.jpg"},
	{"1", 2632261, "030624-1133-01.jpg"},
	{"2", 2433916, "030624-1139-45.jpg"},
	{"3", 2414763, "030626-1158-06.jpg"},
	{"4", 2665331, "030626-1204-01.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_Coolpix_5700_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Nikon\\Coolpix 5700";
static JPGCLI_TEST_SET_T _ar_Coolpix_5700_Set[] =
{
	{"0", 1584534, "020727-1316-56.jpg"},
	{"1", 1754360, "020727-1318-40.jpg"},
	{"2", 1750363, "020727-1321-29.jpg"},
	{"3", 1944707, "020822-1605-11.jpg"},
	{"4", 2046784, "dscn1375.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_Coolpix_700_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Nikon\\Coolpix 700";
static JPGCLI_TEST_SET_T _ar_Coolpix_700_Set[] =
{
	{"0", 462659, "990515-1312-48.jpg"},
	{"1", 438920, "990515-1312-59.jpg"},
	{"2", 408802, "990515-1322-56.jpg"},
	{"3", 417822, "990515-1543-17.jpg"},
	{"4", 403435, "990515-1544-10.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_Coolpix_775_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Nikon\\Coolpix 775";
static JPGCLI_TEST_SET_T _ar_Coolpix_775_Set[] =
{
	{"0", 603616, "010716-1046-10.jpg"},
	{"1", 578895, "010716-1050-46.jpg"},
	{"2", 647519, "010716-1103-31.jpg"},
	{"3", 595643, "010801-1313-49.jpg"},
	{"4", 645082, "010801-1330-54.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_Coolpix_7900_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Nikon\\Coolpix 7900";
static JPGCLI_TEST_SET_T _ar_Coolpix_7900_Set[] =
{
	{"0", 1279952, "dscn0026.jpg"},
	{"1", 1242641, "dscn0029.jpg"},
	{"2", 2561625, "dscn0049.jpg"},
	{"3", 2430558, "sk_dsc00042.jpg"},
	{"4", 2558732, "sk_dsc00050.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_Coolpix_800_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Nikon\\Coolpix 800";
static JPGCLI_TEST_SET_T _ar_Coolpix_800_Set[] =
{
	{"0", 418043, "991023-1340-47.jpg"},
	{"1", 423053, "991030-1146-32.jpg"},
	{"2", 417613, "991030-1155-07.jpg"},
	{"3", 431358, "991127-1328-05.jpg"},
	{"4", 428442, "991127-1359-33.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_Coolpix_8400_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Nikon\\Coolpix 8400";
static JPGCLI_TEST_SET_T _ar_Coolpix_8400_Set[] =
{
	{"0", 2484752, "dscn0127.jpg"},
	{"1", 2683475, "dscn0144.jpg"},
	{"2", 2484695, "dscn0278.jpg"},
	{"3", 2375588, "dscn0359.jpg"},
	{"4", 2569547, "dscn0367.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_Coolpix_8700_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Nikon\\Coolpix 8700";
static JPGCLI_TEST_SET_T _ar_Coolpix_8700_Set[] =
{
	{"0", 2453289, "040330-1042-49.jpg"},
	{"1", 2480403, "040330-1043-57.jpg"},
	{"2", 2560366, "040330-1047-23.jpg"},
	{"3", 3009829, "040331-1347-21.jpg"},
	{"4", 3102307, "040331-1349-52.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_Coolpix_880_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Nikon\\Coolpix 880";
static JPGCLI_TEST_SET_T _ar_Coolpix_880_Set[] =
{
	{"0", 684905, "000814-1134-17.jpg"},
	{"1", 1051916, "000815-1531-33.jpg"},
	{"2", 1042131, "000815-1535-33.jpg"},
	{"3", 1062283, "000825-1556-19.jpg"},
	{"4", 1024956, "000825-1930-48.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_Coolpix_8800_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Nikon\\Coolpix 8800";
static JPGCLI_TEST_SET_T _ar_Coolpix_8800_Set[] =
{
	{"0", 2355084, "dscn0096.jpg"},
	{"1", 2395987, "dscn0100.jpg"},
	{"2", 2408286, "dscn0153.jpg"},
	{"3", 2816279, "dscn0235.jpg"},
	{"4", 3067695, "dscn0240.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_Coolpix_885_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Nikon\\Coolpix 885";
static JPGCLI_TEST_SET_T _ar_Coolpix_885_Set[] =
{
	{"0", 712467, "010825-1308-59.jpg"},
	{"1", 682729, "010825-1540-26.jpg"},
	{"2", 755929, "010825-1625-36.jpg"},
	{"3", 981984, "010826-1446-19.jpg"},
	{"4", 682893, "010826-1504-16.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_Coolpix_950_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Nikon\\Coolpix 950";
static JPGCLI_TEST_SET_T _ar_Coolpix_950_Set[] =
{
	{"0", 801317, "990408-1225-08.jpg"},
	{"1", 651969, "990408-1228-07.jpg"},
	{"2", 652251, "990408-1400-40.jpg"},
	{"3", 619064, "990417-1529-59.jpg"},
	{"4", 314514, "dscn0197.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_Coolpix_990_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Nikon\\Coolpix 990";
static JPGCLI_TEST_SET_T _ar_Coolpix_990_Set[] =
{
	{"0", 1083036, "000405-1533-42.jpg"},
	{"1", 1001569, "000405-1546-48.jpg"},
	{"2", 1110216, "000405-1553-36.jpg"},
	{"3", 648279, "000414-1638-26.jpg"},
	{"4", 690867, "000415-1648-03.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_Coolpix_995_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Nikon\\Coolpix 995";
static JPGCLI_TEST_SET_T _ar_Coolpix_995_Set[] =
{
	{"0", 1006685, "010618-1028-15.jpg"},
	{"1", 1043956, "010618-1031-19.jpg"},
	{"2", 990227, "010618-1034-37.jpg"},
	{"3", 1042039, "dscn0373.jpg"},
	{"4", 1138674, "dscn0380.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_D1_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Nikon\\D1";
static JPGCLI_TEST_SET_T _ar_D1_Set[] =
{
	{"0", 1388616, "000902-0705-13.jpg"},
	{"1", 1123119, "000916-0123-48.jpg"},
	{"2", 1341869, "000920-1844-33.jpg"},
	{"3", 1274993, "001112-1612-54.jpg"},
	{"4", 1224363, "001112-1631-21.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_D100_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Nikon\\D100";
static JPGCLI_TEST_SET_T _ar_D100_Set[] =
{
	{"0", 2966544, "020704-0957-47.jpg"},
	{"1", 3051393, "020704-1003-51.jpg"},
	{"2", 1623240, "020714-1314-51-raw.jpg"},
	{"3", 1777485, "020714-1610-48-rawa.jpg"},
	{"4", 2943164, "dsc_0010.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_D1H_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Nikon\\D1H";
static JPGCLI_TEST_SET_T _ar_D1H_Set[] =
{
	{"0", 1344021, "dsc_0026.jpg"},
	{"1", 1251436, "dsc_0183.jpg"},
	{"2", 1225017, "dsc_0189.jpg"},
	{"3", 1111120, "dsc_0215.jpg"},
	{"4", 1199764, "dsc_1071.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_D1X_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Nikon\\D1X";
static JPGCLI_TEST_SET_T _ar_D1X_Set[] =
{
	{"0", 1501704, "010621-1144-05.jpg"},
	{"1", 2692889, "010621-1152-30.jpg"},
	{"2", 2397791, "010621-1353-46.jpg"},
	{"3", 2696647, "010624-0856-33.jpg"},
	{"4", 2627844, "010627-2302-31.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_D2H_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Nikon\\D2H";
static JPGCLI_TEST_SET_T _ar_D2H_Set[] =
{
	{"0", 1703625, "dsc_0016.jpg"},
	//{"1", 1222290, "dsc_0107.jpg"},  //E_JPG_ERR_MULTI_SOI (-9)
	{"2", 1997772, "dsc_0633.jpg"},
	//{"3", 3474096, "dsc_0892.jpg"},  //E_JPG_ERR_MULTI_SOI (-9)
	{"4", 1964386, "dsc_0930.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_D70_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Nikon\\D70";
static JPGCLI_TEST_SET_T _ar_D70_Set[] =
{
	{"0", 2699251, "dsc_0069.jpg"},
	{"1", 2396428, "dsc_0097.jpg"},
	{"2", 3316469, "dsc_0232.jpg"},
	{"3", 2374733, "dsc_0318.jpg"},
	{"4", 2972276, "dsc_0352.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_C_2000Z_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Olympus\\C-2000Z";
static JPGCLI_TEST_SET_T _ar_C_2000Z_Set[] =
{
	{"0", 483913, "990702-1711-51.jpg"},
	{"1", 440588, "990708-1720-23.jpg"},
	{"2", 412575, "990711-1513-33.jpg"},
	{"3", 438671, "990711-1513-50.jpg"},
	{"4", 436522, "990711-1533-25.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_C_2100UZ_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Olympus\\C-2100UZ";
static JPGCLI_TEST_SET_T _ar_C_2100UZ_Set[] =
{
	{"0", 476770, "001027-1233-08.jpg"},
	{"1", 1007846, "001029-1346-30.jpg"},
	{"2", 485815, "001112-1216-27.jpg"},
	{"3", 472108, "001112-1241-31.jpg"},
	{"4", 440112, "001112-1246-04.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_C_2500L_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Olympus\\C-2500L";
static JPGCLI_TEST_SET_T _ar_C_2500L_Set[] =
{
	{"0", 543991, "991115-2051-37.jpg"},
	{"1", 517084, "991120-1241-36.jpg"},
	{"2", 511381, "991127-1316-22.jpg"},
	{"3", 513387, "991127-1318-20.jpg"},
	{"4", 532017, "991127-1328-08.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_C_3030Z_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Olympus\\C-3030Z";
static JPGCLI_TEST_SET_T _ar_C_3030Z_Set[] =
{
	{"0", 629814, "000429-1246-51.jpg"},
	{"1", 683887, "000429-1300-22.jpg"},
	{"2", 758320, "000430-1210-34.jpg"},
	{"3", 776173, "000508-1549-50.jpg"},
	{"4", 752465, "000508-1552-57.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_C_3040Z_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Olympus\\C-3040Z";
static JPGCLI_TEST_SET_T _ar_C_3040Z_Set[] =
{
	{"0", 744483, "010327-1438-08.jpg"},
	{"1", 746132, "010327-1440-23.jpg"},
	{"2", 1683921, "010402-1212-15.jpg"},
	{"3", 2160285, "010417-1424-21.jpg"},
	{"4", 1604804, "010417-1425-56.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_C_40Z_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Olympus\\C-40Z";
static JPGCLI_TEST_SET_T _ar_C_40Z_Set[] =
{
	{"0", 2258545, "011003-1017-42.jpg"},
	{"1", 2500699, "011003-1024-47.jpg"},
	{"2", 856465, "011003-1129-16.jpg"},
	{"3", 711056, "011003-1144-44.jpg"},
	{"4", 834079, "pb180022.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_C_5050Z_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Olympus\\C-5050Z";
static JPGCLI_TEST_SET_T _ar_C_5050Z_Set[] =
{
	{"0", 3182021, "030104-1135-36.jpg"},
	{"1", 3054507, "030104-1153-03.jpg"},
	{"2", 2738650, "030104-1200-45.jpg"},
	{"3", 2991337, "030104-1311-42.jpg"},
	{"4", 2141392, "030109-1149-01.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_C_50Z_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Olympus\\C-50Z";
static JPGCLI_TEST_SET_T _ar_C_50Z_Set[] =
{
	{"0", 2117262, "030412-1329-01.jpg"},
	{"1", 2152664, "030412-1339-22.jpg"},
	{"2", 2180981, "030412-1353-17.jpg"},
	{"3", 2148710, "030417-1149-20.jpg"},
	{"4", 2403299, "030417-1155-40.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_C_5500sz_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Olympus\\C-5500sz";
static JPGCLI_TEST_SET_T _ar_C_5500sz_Set[] =
{
	{"0", 2878149, "P3250031.JPG"},
	{"1", 3027672, "P4040038.JPG"},
	{"2", 3147987, "P4240069.JPG"},
	{NULL, NULL, NULL}
};

static CHAR _sz_C_7000Z_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Olympus\\C-7000Z";
static JPGCLI_TEST_SET_T _ar_C_7000Z_Set[] =
{
	{"0", 3726654, "pa220034.jpg"},
	{"1", 3287204, "pa220054.jpg"},
	{"2", 3371079, "pa220089.jpg"},
	{"3", 4332320, "pa250064.jpg"},
	{"4", 4196972, "pa250067.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_C_700UZ_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Olympus\\C-700UZ";
static JPGCLI_TEST_SET_T _ar_C_700UZ_Set[] =
{
	{"0", 423761, "010508-1039-36.jpg"},
	{"1", 454739, "010508-1041-00.jpg"},
	{"2", 458551, "010508-1146-27.jpg"},
	{"3", 971824, "p5050006.jpg"},
	{"4", 1011607, "p5110370.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_C_8080WZ_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Olympus\\C-8080WZ";
static JPGCLI_TEST_SET_T _ar_C_8080WZ_Set[] =
{
	{"0", 3407458, "040331-1110-57.jpg"},
	{"1", 3904554, "040331-1112-18.jpg"},
	{"2", 3755199, "040331-1129-35.jpg"},
	{"3", 2652886, "040331-1209-56.jpg"},
	{"4", 3541109, "040331-1252-06.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_E_1_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Olympus\\E-1";
static JPGCLI_TEST_SET_T _ar_E_1_Set[] =
{
	{"0", 3542934, "pb230837.jpg"},
	{"1", 3559107, "pb271150.jpg"},
	{"2", 3634081, "pb271247.jpg"},
	{"3", 3180838, "_9271881.jpg"},
	{"4", 3239876, "_9272135.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_E_10_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Olympus\\E-10";
static JPGCLI_TEST_SET_T _ar_E_10_Set[] =
{
	{"0", 1778430, "001113-0740-36.jpg"},
	{"1", 1786125, "001113-0745-18.jpg"},
	{"2", 784951, "001114-0452-05.jpg"},
	{"3", 1793537, "001114-0452-14.jpg"},
	{"4", 1734193, "001226-1527-21.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_E_20_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Olympus\\E-20";
static JPGCLI_TEST_SET_T _ar_E_20_Set[] =
{
	{"0", 2411039, "011208-1407-01.jpg"},
	{"1", 1234101, "011210-1311-41.jpg"},
	{"2", 2540993, "011210-1436-23.jpg"},
	{"3", 2561077, "020119-0912-14.jpg"},
	{"4", 2670444, "020119-0949-48.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_E_300_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Olympus\\E-300";
static JPGCLI_TEST_SET_T _ar_E_300_Set[] =
{
	{"0", 3405618, "p1020367.jpg"},
	{"1", 3631288, "p1020468-a.jpg"},
	{"2", 4044189, "p1030574.jpg"},
	{"3", 4848257, "pc050169.jpg"},
	{"4", 5460349, "pc250114.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_stylus_Verve_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Olympus\\stylus Verve";
static JPGCLI_TEST_SET_T _ar_stylus_Verve_Set[] =
{
	{"0", 2779112, "p1010068.jpg"},
	{"1", 2821843, "p1010074.jpg"},
	{"2", 2738701, "p1010079.jpg"},
	//{"3", 871462, "sk_pc250017.jpg"},
	{"4", 911961, "sk_pc270023.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_FX7_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Panasonic\\FX7";
static JPGCLI_TEST_SET_T _ar_FX7_Set[] =
{
	{"0", 2212772, "p1000044.jpg"},
	{"1", 2460041, "p1000092.jpg"},
	{"2", 2395573, "p1000094.jpg"},
	{"3", 1847645, "p1000120.jpg"},
	{"4", 2383639, "p1020585.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_FZ20_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Panasonic\\FZ20";
static JPGCLI_TEST_SET_T _ar_FZ20_Set[] =
{
	{"0", 1912475, "040919-1100-10.jpg"},
	{"1", 2521825, "p1010115.jpg"},
	{"2", 1809563, "p1010495.jpg"},
	{"3", 2342785, "p1010532.jpg"},
	{"4", 2474066, "p1020094.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_FZ3_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Panasonic\\FZ3";
static JPGCLI_TEST_SET_T _ar_FZ3_Set[] =
{
	{"0", 1343801, "p1010011.jpg"},
	{"1", 1480533, "p1010019.jpg"},
	{"2", 1342419, "p1010170.jpg"},
	{"3", 1470124, "p1010395.jpg"},
	{"4", 1343980, "p1010550.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_FZ5_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Panasonic\\FZ5";
static JPGCLI_TEST_SET_T _ar_FZ5_Set[] =
{
	{"0", 2480608, "p1000001.jpg"},
	{"1", 2007275, "p1000018.jpg"},
	{"2", 2551673, "p1000030.jpg"},
	{"3", 2314872, "p1000525.jpg"},
	{"4", 2459966, "p1000896.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_LZ2_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Panasonic\\LZ2";
static JPGCLI_TEST_SET_T _ar_LZ2_Set[] =
{
	{"0", 2242422, "cj_p1010543.jpg"},
	{"1", 2463400, "jaskp1010003.jpg"},
	{"2", 1911133, "jaskp1010071.jpg"},
	{"3", 2482728, "sj_p1010546.jpg"},
	{"4", 2035258, "sk_1010507.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_ist_D_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Pentax\\ist D";
static JPGCLI_TEST_SET_T _ar_ist_D_Set[] =
{
	{"0", 3135942, "030917-1032-35.jpg"},
	{"1", 2297363, "030917-1038-23.jpg"},
	{"2", 2178502, "030922-0843-01.jpg"},
	{"3", 2662818, "030922-0857-44.jpg"},
	{"4", 2039164, "030922-0929-34-a.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_ist_DS_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Pentax\\ist DS";
static JPGCLI_TEST_SET_T _ar_ist_DS_Set[] =
{
	{"0", 2873189, "imgp0145.jpg"},
	//{"1", 1701236, "imgp0158-raw-acr.jpg"},
	{"2", 1917852, "imgp0475.jpg"},
	{"3", 2785861, "imgp0514.jpg"},
	{"4", 3468297, "imgp0546.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_Optio_330_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Pentax\\Optio 330";
static JPGCLI_TEST_SET_T _ar_Optio_330_Set[] =
{
	{"0", 1229392, "010928-1533-25.jpg"},
	{"1", 1557434, "010928-1559-42.jpg"},
	{"2", 1503972, "010928-1621-35.jpg"},
	{"3", 1109104, "010928-1629-59.jpg"},
	{"4", 1467225, "010928-1657-23.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_Optio_430_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Pentax\\Optio 430";
static JPGCLI_TEST_SET_T _ar_Optio_430_Set[] =
{
	{"0", 1656293, "020122-1110-48.jpg"},
	{"1", 2056407, "020122-1130-31.jpg"},
	{"2", 1398852, "020122-1142-54.jpg"},
	{"3", 1665696, "020329-1202-24.jpg"},
	{"4", 1936642, "020329-1216-50.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_Optio_550_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Pentax\\Optio 550";
static JPGCLI_TEST_SET_T _ar_Optio_550_Set[] =
{
	{"0", 1174196, "030412-1327-44.jpg"},
	{"1", 1108998, "030412-1341-37.jpg"},
	{"2", 1163606, "030412-1344-49.jpg"},
	{"3", 1297448, "030412-1406-05.jpg"},
	{"4", 2885044, "030417-1147-00.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_Optio_750_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Pentax\\Optio 750";
static JPGCLI_TEST_SET_T _ar_Optio_750_Set[] =
{
	{"0", 3825762, "imgp0013.jpg"},
	{"1", 3806777, "imgp0047.jpg"},
	{"2", 3818587, "imgp0093.jpg"},
	{"3", 3584069, "imgp0240.jpg"},
	{"4", 3045913, "imgp0242.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_Optio_S_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Pentax\\Optio S";
static JPGCLI_TEST_SET_T _ar_Optio_S_Set[] =
{
	{"0", 1052620, "030305-1149-45.jpg"},
	{"1", 1056734, "030306-1250-02.jpg"},
	{"2", 1022841, "030306-1309-25.jpg"},
	{"3", 1023404, "030306-1356-52.jpg"},
	{"4", 2035494, "030417-1138-55.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_Optio_S5i_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Pentax\\Optio S5i";
static JPGCLI_TEST_SET_T _ar_Optio_S5i_Set[] =
{
	{"0", 3321264, "cj_imgp0178.jpg"},
	{"1", 3033678, "cj_imgp0187.jpg"},
	{"2", 3478722, "imgp0567.jpg"},
	{"3", 3253876, "sj_imgp0191.jpg"},
	{"4", 3415638, "sk_imgp0062.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_v700_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Samsung\\v700";
static JPGCLI_TEST_SET_T _ar_v700_Set[] =
{
	{"0", 1826377, "cam2_sv100004.jpg"},
	{"1", 3544080, "sv100331.jpg"},
	{"2", 3537789, "sv100357.jpg"},
	{"3", 3623240, "sv100949.jpg"},
	{"4", 3387757, "sv100993.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_SD10_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Sigma\\SD10";
static JPGCLI_TEST_SET_T _ar_SD10_Set[] =
{
	{"0", 1022486, "img00247.jpg"},
	{"1", 1332234, "img00301.jpg"},
	{"2", 866469, "img07183.jpg"},
	{"3", 3154432, "img09728.jpg"},
	{"4", 1931282, "img09911.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_SD9_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Sigma\\SD9";
static JPGCLI_TEST_SET_T _ar_SD9_Set[] =
{
	{"0", 1613363, "img00901.jpg"},
	{"1", 1837388, "img01016.jpg"},
	{"2", 1317788, "img01309.jpg"},
	{"3", 2367591, "img01327.jpg"},
	{"4", 2283896, "img01410.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_DSC_D700_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Sony\\DSC-D700";
static JPGCLI_TEST_SET_T _ar_DSC_D700_Set[] =
{
	{"0", 681241, "990528-1240-00.jpg"},
	{"1", 663953, "990528-1817-17.jpg"},
	{"2", 383001, "990604-1834-10.jpg"},
	{"3", 361400, "990605-1536-32.jpg"},
	{"4", 382940, "990609-1611-31.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_DSC_F505_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Sony\\DSC-F505";
static JPGCLI_TEST_SET_T _ar_DSC_F505_Set[] =
{
	{"0", 780501, "990828-1239-17.jpg"},
	{"1", 402572, "990828-1300-08.jpg"},
	{"2", 403656, "990828-1427-54.jpg"},
	{"3", 436359, "990831-1104-33.jpg"},
	{"4", 471492, "990831-1113-12.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_DSC_F505V_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Sony\\DSC-F505V";
static JPGCLI_TEST_SET_T _ar_DSC_F505V_Set[] =
{
	{"0", 1169571, "000712-1403-01.jpg"},
	{"1", 1075885, "000712-1408-07.jpg"},
	{"2", 1133562, "000713-1550-53.jpg"},
	{"3", 1201767, "000713-1638-53.jpg"},
	{"4", 1160827, "000715-1541-02.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_DSC_F707_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Sony\\DSC-F707";
static JPGCLI_TEST_SET_T _ar_DSC_F707_Set[] =
{
	{"0", 2020659, "010904-1101-05.jpg"},
	{"1", 2327331, "010904-1102-46.jpg"},
	{"2", 1792379, "010904-1112-58.jpg"},
	{"3", 2154213, "010904-1134-55.jpg"},
	{"4", 2213517, "010904-1247-07.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_DSC_F717_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Sony\\DSC-F717";
static JPGCLI_TEST_SET_T _ar_DSC_F717_Set[] =
{
	{"0", 2310573, "020907-1134-12.jpg"},
	{"1", 2319488, "020908-0910-50.jpg"},
	{"2", 2271197, "020914-1849-07.jpg"},
	{"3", 1888331, "021006-0210-23.jpg"},
	{"4", 1896611, "021006-0218-38.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_DSC_F828_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Sony\\DSC-F828";
static JPGCLI_TEST_SET_T _ar_DSC_F828_Set[] =
{
	{"0", 2676301, "040104-1053-26.jpg"},
	{"1", 3888160, "040106-0058-39.jpg"},
	{"2", 3753080, "040106-0940-54.jpg"},
	{"3", 3417617, "040106-1033-42.jpg"},
	{"4", 3548212, "040107-1833-57.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_DSC_F88_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Sony\\DSC-F88";
static JPGCLI_TEST_SET_T _ar_DSC_F88_Set[] =
{
	{"0", 2409211, "dsc00077.jpg"},
	{"1", 2280606, "dsc00126.jpg"},
	{"2", 2351615, "dsc00224.jpg"},
	{"3", 2283149, "dsc00247.jpg"},
	{"4", 1567830, "dsc00583.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_DSC_L1_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Sony\\DSC-L1";
static JPGCLI_TEST_SET_T _ar_DSC_L1_Set[] =
{
	{"0", 1601055, "dsc00014.jpg"},
	{"1", 1744362, "dsc00046.jpg"},
	{"2", 1596547, "dsc00275.jpg"},
	{"3", 1683132, "dsc00517.jpg"},
	{"4", 619781, "dsc00773.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_DSC_P1_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Sony\\DSC-P1";
static JPGCLI_TEST_SET_T _ar_DSC_P1_Set[] =
{
	{"0", 1293906, "000909-1117-03.jpg"},
	{"1", 144940, "000909-1117-03[1].jpg"},
	{"2", 1436872, "000909-1515-16.jpg"},
	{"3", 1413827, "000909-1517-03.jpg"},
	{"4", 1358030, "000909-1523-24.jpg"},
	{"5", 1435148, "000910-1543-55.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_DSC_P10_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Sony\\DSC-P10";
static JPGCLI_TEST_SET_T _ar_DSC_P10_Set[] =
{
	{"0", 1726081, "DSC01146.JPG"},
	{"1", 1976452, "DSC04570.JPG"},
	{"2", 2209883, "DSC04613.JPG"},
	{NULL, NULL, NULL}
};

static CHAR _sz_DSC_P150_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Sony\\DSC-P150";
static JPGCLI_TEST_SET_T _ar_DSC_P150_Set[] =
{
	{"0", 2797082, "dsc00628.jpg"},
	{"1", 2483752, "dsc00655.jpg"},
	{"2", 2755263, "dsc00785.jpg"},
	{"3", 2967724, "dsc00819.jpg"},
	{"4", 3127661, "dsc00829.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_DSC_P5_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Sony\\DSC-P5";
static JPGCLI_TEST_SET_T _ar_DSC_P5_Set[] =
{
	{"0", 1192778, "010730-1436-23.jpg"},
	{"1", 1441035, "010814-1627-48.jpg"},
	{"2", 1439806, "010814-1630-20.jpg"},
	{"3", 1466473, "010814-1636-02.jpg"},
	{"4", 1387721, "dsc00075.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_DSC_P71_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Sony\\DSC-P71";
static JPGCLI_TEST_SET_T _ar_DSC_P71_Set[] =
{
	{"0", 1452874, "020807-1040-34.jpg"},
	{"1", 1302089, "020807-1109-43.jpg"},
	{"2", 1340859, "020807-1113-28.jpg"},
	{"3", 1451782, "020807-1119-34.jpg"},
	{"4", 1443131, "020807-1133-58.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_DSC_P9_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Sony\\DSC-P9";
static JPGCLI_TEST_SET_T _ar_DSC_P9_Set[] =
{
	{"0", 1573466, "020416-1017-08.jpg"},
	{"1", 1712540, "020416-1508-49.jpg"},
	{"2", 1812481, "020416-1518-51.jpg"},
	{"3", 1779464, "020416-1548-52.jpg"},
	{"4", 1850000, "020416-1550-45.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_DSC_S70_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Sony\\DSC-S70";
static JPGCLI_TEST_SET_T _ar_DSC_S70_Set[] =
{
	{"0", 519852, "000529-1605-41.jpg"},
	{"1", 1346666, "000613-1420-19.jpg"},
	{"2", 1338771, "000614-1516-53.jpg"},
	{"3", 1266017, "000621-1138-14.jpg"},
	{"4", 1337720, "000624-1646-02.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_DSC_S75_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Sony\\DSC-S75";
static JPGCLI_TEST_SET_T _ar_DSC_S75_Set[] =
{
	{"0", 1405894, "010206-1005-59.jpg"},
	{"1", 1447985, "010206-1054-44.jpg"},
	{"2", 1297132, "010207-0845-39.jpg"},
	{"3", 807595, "010215-1319-48.jpg"},
	{"4", 816637, "010215-1400-44.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_DSC_S85_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Sony\\DSC-S85";
static JPGCLI_TEST_SET_T _ar_DSC_S85_Set[] =
{
	{"0", 1525651, "010508-0934-07.jpg"},
	{"1", 1869657, "010508-1052-32.jpg"},
	{"2", 1807420, "010508-1111-31.jpg"},
	{"3", 1866181, "010508-1209-12.jpg"},
	{"4", 1845931, "010508-1211-59.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_DSC_S90_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Sony\\DSC-S90";
static JPGCLI_TEST_SET_T _ar_DSC_S90_Set[] =
{
	{"0", 1858724, "dsc00110.jpg"},
	{"1", 1571696, "dsc00292.jpg"},
	{"2", 1816651, "dsc00396.jpg"},
	{"3", 1702476, "dsc00446.jpg"},
	{"4", 1849245, "sk_dsc00038.jpg"},
	{"5", 1830794, "sk_dsc00065.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_DSC_V1_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Sony\\DSC-V1";
static JPGCLI_TEST_SET_T _ar_DSC_V1_Set[] =
{
	{"0", 1873244, "030614-1001-40.jpg"},
	{"1", 1880669, "030620-1517-13.jpg"},
	{"2", 2333988, "030620-1819-16.jpg"},
	{"3", 2180854, "030626-1146-08.jpg"},
	{"4", 2311376, "030626-1148-31.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_DSC_V3_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Sony\\DSC-V3";
static JPGCLI_TEST_SET_T _ar_DSC_V3_Set[] =
{
	{"0", 3131272, "dsc01519.jpg"},
	{"1", 2640257, "dsc01536.jpg"},
	{"2", 2557334, "dsc01651.jpg"},
	{"3", 3085449, "dsc01664.jpg"},
	{"4", 2704693, "dsc01709.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_DSC_W7_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Sony\\DSC-W7";
static JPGCLI_TEST_SET_T _ar_DSC_W7_Set[] =
{
	{"0", 3222117, "DSC00001.JPG"},
	{"1", 3411876, "DSC00006.JPG"},
	{"2", 3178826, "DSC00008.JPG"},
	{"3", 2843068, "DSC00012.JPG"},
	{"4", 3232445, "DSC00015.JPG"},
	{"5", 2630009, "DSC00034.JPG"},
	{NULL, NULL, NULL}
};

static CHAR _sz_MVC_CD1000_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Sony\\MVC-CD1000";
static JPGCLI_TEST_SET_T _ar_MVC_CD1000_Set[] =
{
	{"0", 763530, "000801-1021-46.jpg"},
	{"1", 883084, "000801-1022-19.jpg"},
	{"2", 883047, "000812-0854-56.jpg"},
	{"3", 919711, "000812-0944-58.jpg"},
	{"4", 802711, "000812-0955-21.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_MVC_CD200_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Sony\\MVC-CD200";
static JPGCLI_TEST_SET_T _ar_MVC_CD200_Set[] =
{
	{"0", 850170, "010225-1140-52.jpg"},
	{"1", 887917, "010225-1218-51.jpg"},
	{"2", 884004, "010225-1225-28.jpg"},
	{"3", 851716, "010225-1230-14.jpg"},
	{"4", 838383, "010225-1251-03.jpg"},
	{NULL, NULL, NULL}
};

static CHAR _sz_MVC_CD300_Dir[] = 
	"D:\\jpg_test\\pattern_database\\dpreview\\Sony\\MVC-CD300";
static JPGCLI_TEST_SET_T _ar_MVC_CD300_Set[] =
{
	{"0", 1380640, "010225-1145-33.jpg"},
	{"1", 1454577, "010225-1212-00.jpg"},
	{"2", 1401371, "010225-1221-34.jpg"},
	{"3", 1345698, "010225-1240-28.jpg"},
	{"4", 1395107, "010225-1248-41.jpg"},
	{"5", 1487709, "010225-1300-17.jpg"},
	{NULL, NULL, NULL}
};


#define D_MAX_JPG_TEST_SETS 213

static JPGCLI_TEST_SETS_T _arTestSets[] =
{
       {_ar_digital_IXUS_300_Set, _sz_digital_IXUS_300_Dir},
       {_ar_d_30_Set, _sz_d_30_Dir},
       {_ar_digital_IXUS_Set, _sz_digital_IXUS_Dir},
       {_ar_digital_IXUS_330_Set, _sz_digital_IXUS_330_Dir},
       {_ar_digital_IXUS_V_Set, _sz_digital_IXUS_V_Dir},
       {_ar_eos_10d_Set, _sz_eos_10d_Dir},
       {_ar_eos_1d_Set, _sz_eos_1d_Dir},
       {_ar_eos_1d_mk_II_Set, _sz_eos_1d_mk_II_Dir},
       {_ar_eos_1d_mk_II_B_Set, _sz_eos_1d_mk_II_B_Dir},
       {_ar_eos_1ds_Set, _sz_eos_1ds_Dir},
       {_ar_eos_20d_Set, _sz_eos_20d_Dir},
       {_ar_eos_20d_b_Set, _sz_eos_20d_b_Dir},
       {_ar_eos_300d_Set, _sz_eos_300d_Dir},
       {_ar_eos_300d_b_Set, _sz_eos_300d_b_Dir},
       {_ar_eos_350d_Set, _sz_eos_350d_Dir},
       {_ar_eosd_60_Set, _sz_eosd_60_Dir},
       {_ar_G1_Set, _sz_G1_Dir},
       {_ar_G2_Set, _sz_G2_Dir},
       {_ar_G3_Set, _sz_G3_Dir},
       {_ar_G5_Set, _sz_G5_Dir},
       {_ar_G6_Set, _sz_G6_Dir},
       {_ar_powershot_a20_Set, _sz_powershot_a20_Dir},
       {_ar_powershot_a510_Set, _sz_powershot_a510_Dir},
       {_ar_powershot_a520_Set, _sz_powershot_a520_Dir},
       {_ar_powershot_a70_Set, _sz_powershot_a70_Dir},
       {_ar_powershot_a95_Set, _sz_powershot_a95_Dir},
       {_ar_pro1_Set, _sz_pro1_Dir},
       {_ar_pro90_IS_Set, _sz_pro90_IS_Dir},
       {_ar_s_400_Set, _sz_s_400_Dir},
       {_ar_S1_IS_Set, _sz_S1_IS_Dir},
       {_ar_s10_Set, _sz_s10_Dir},
       {_ar_s20_Set, _sz_s20_Dir},
       {_ar_s40_Set, _sz_s40_Dir},
       {_ar_s45_Set, _sz_s45_Dir},
       {_ar_s50_Set, _sz_s50_Dir},
       {_ar_s60_Set, _sz_s60_Dir},
       {_ar_s70_Set, _sz_s70_Dir},
       {_ar_sd_300_Set, _sz_sd_300_Dir},
       {_ar_sd_500_Set, _sz_sd_500_Dir},
       {_ar_sd100_Set, _sz_sd100_Dir},
       {_ar_EX_P505_Set, _sz_EX_P505_Dir},
       {_ar_EX_P700_Set, _sz_EX_P700_Dir},
       {_ar_EX_S3_Set, _sz_EX_S3_Dir},
       {_ar_EX_Z3_Set, _sz_EX_Z3_Dir},
       {_ar_EX_Z750_Set, _sz_EX_Z750_Dir},
       {_ar_GV_10_Set, _sz_GV_10_Dir},
       {_ar_QV_3000_Set, _sz_QV_3000_Dir},
       {_ar_QV_4000_Set, _sz_QV_4000_Dir},
       {_ar_QV_8000SX_Set, _sz_QV_8000SX_Dir},
       {_ar_QV_R4_Set, _sz_QV_R4_Dir},
       {_ar_QV_R51_Set, _sz_QV_R51_Dir},
       {_ar_TVS_digital_Set, _sz_TVS_digital_Dir},
       {_ar_photoPC_3000Z_Set, _sz_photoPC_3000Z_Dir},
       {_ar_1500_Set, _sz_1500_Dir},
       {_ar_3800_Set, _sz_3800_Dir},
       {_ar_A310_Set, _sz_A310_Dir},
       {_ar_E550_Set, _sz_E550_Dir},
       {_ar_F10_Set, _sz_F10_Dir},
       {_ar_FinePix_40i_Set, _sz_FinePix_40i_Dir},
       {_ar_FinePix_4700z_Set, _sz_FinePix_4700z_Dir},
       {_ar_FinePix_4900z_Set, _sz_FinePix_4900z_Dir},
       {_ar_FinePix_6800z_Set, _sz_FinePix_6800z_Dir},
       {_ar_FinePix_6900z_Set, _sz_FinePix_6900z_Dir},
       {_ar_FinePix_E550_Set, _sz_FinePix_E550_Dir},
       {_ar_FinePix_F601Z_Set, _sz_FinePix_F601Z_Dir},
       {_ar_FinePix_F700_Set, _sz_FinePix_F700_Dir},
       {_ar_FinePix_F810_Set, _sz_FinePix_F810_Dir},
       {_ar_MX_2700_Set, _sz_MX_2700_Dir},
       {_ar_MX_2900_Set, _sz_MX_2900_Dir},
       {_ar_S1_pro_Set, _sz_S1_pro_Dir},
       {_ar_S2_pro_Set, _sz_S2_pro_Dir},
       {_ar_S3_pro_Set, _sz_S3_pro_Dir},
       {_ar_S5000Z_Set, _sz_S5000Z_Dir},
       {_ar_S5100Z_Set, _sz_S5100Z_Dir},
       {_ar_S602Z_Set, _sz_S602Z_Dir},
       {_ar_S7000Z_Set, _sz_S7000Z_Dir},
       {_ar_812_Set, _sz_812_Dir},
       {_ar_850_Set, _sz_850_Dir},
       {_ar_935_Set, _sz_935_Dir},
       {_ar_C500_Set, _sz_C500_Dir},
       {_ar_R707_Set, _sz_R707_Dir},
       {_ar_DC265_Set, _sz_DC265_Dir},
       {_ar_DC280_Set, _sz_DC280_Dir},
       {_ar_DC290_Set, _sz_DC290_Dir},
       {_ar_DC4800_Set, _sz_DC4800_Dir},
       {_ar_DCS_pro_14n_Set, _sz_DCS_pro_14n_Dir},
       {_ar_DCS_pro_SLRc_Set, _sz_DCS_pro_SLRc_Dir},
       {_ar_DCS_pro_SLRn_Set, _sz_DCS_pro_SLRn_Dir},
       {_ar_DX4900_Set, _sz_DX4900_Dir},
       {_ar_DX7590_Set, _sz_DX7590_Dir},
       {_ar_LS743_Set, _sz_LS743_Dir},
       {_ar_pro_DCS_520_Set, _sz_pro_DCS_520_Dir},
       {_ar_pro_DCS_620_Set, _sz_pro_DCS_620_Dir},
       {_ar_pro_DCS_620x_Set, _sz_pro_DCS_620x_Dir},
       {_ar_pro_DCS_760_Set, _sz_pro_DCS_760_Dir},
       {_ar_Z740_Set, _sz_Z740_Dir},
       {_ar_Digitial_Revio_KD_400Z_Set, _sz_Digitial_Revio_KD_400Z_Dir},
       {_ar_DiMAGE_5_Set, _sz_DiMAGE_5_Dir},
       {_ar_DiMAGE_7_Set, _sz_DiMAGE_7_Dir},
       {_ar_DiMAGE_7Hi_Set, _sz_DiMAGE_7Hi_Dir},
       {_ar_DiMAGE_7i_Set, _sz_DiMAGE_7i_Dir},
       {_ar_DiMAGE_A1_Set, _sz_DiMAGE_A1_Dir},
       {_ar_DiMAGE_EX_1500_Set, _sz_DiMAGE_EX_1500_Dir},
       {_ar_DiMAGE_F100_Set, _sz_DiMAGE_F100_Dir},
       {_ar_DiMAGE_G500_Set, _sz_DiMAGE_G500_Dir},
       {_ar_DiMAGE_S304_Set, _sz_DiMAGE_S304_Dir},
       {_ar_DiMAGE_S404_Set, _sz_DiMAGE_S404_Dir},
       {_ar_DiMAGE_X_Set, _sz_DiMAGE_X_Dir},
       {_ar_Minolta_7D_Set, _sz_Minolta_7D_Dir},
       {_ar_Minolta_7D_B_Set, _sz_Minolta_7D_B_Dir},
       {_ar_Minolta_A2_Set, _sz_Minolta_A2_Dir},
       {_ar_Minolta_A200_Set, _sz_Minolta_A200_Dir},
       {_ar_Minolta_Z2_Set, _sz_Minolta_Z2_Dir},
       {_ar_Minolta_Z5_Set, _sz_Minolta_Z5_Dir},
       {_ar_FINEcam_L3v_Set, _sz_FINEcam_L3v_Dir},
       {_ar_FINEcam_M410R_Set, _sz_FINEcam_M410R_Dir},
       {_ar_Finecam_s3_Set, _sz_Finecam_s3_Dir},
       {_ar_FINEcam_SL300R_Set, _sz_FINEcam_SL300R_Dir},
       {_ar_Digilux_2_Set, _sz_Digilux_2_Dir},
       {_ar_Coolpix_2500_Set, _sz_Coolpix_2500_Dir},
       {_ar_Coolpix_3100_Set, _sz_Coolpix_3100_Dir},
       {_ar_Coolpix_4500_Set, _sz_Coolpix_4500_Dir},
       {_ar_Coolpix_4800_Set, _sz_Coolpix_4800_Dir},
       {_ar_Coolpix_5000_Set, _sz_Coolpix_5000_Dir},
       {_ar_Coolpix_5200_Set, _sz_Coolpix_5200_Dir},
       {_ar_Coolpix_5400_Set, _sz_Coolpix_5400_Dir},
       {_ar_Coolpix_5700_Set, _sz_Coolpix_5700_Dir},
       {_ar_Coolpix_700_Set, _sz_Coolpix_700_Dir},
       {_ar_Coolpix_775_Set, _sz_Coolpix_775_Dir},
       {_ar_Coolpix_7900_Set, _sz_Coolpix_7900_Dir},
       {_ar_Coolpix_800_Set, _sz_Coolpix_800_Dir},
       {_ar_Coolpix_8400_Set, _sz_Coolpix_8400_Dir},
       {_ar_Coolpix_8700_Set, _sz_Coolpix_8700_Dir},
       {_ar_Coolpix_880_Set, _sz_Coolpix_880_Dir},
       {_ar_Coolpix_8800_Set, _sz_Coolpix_8800_Dir},
       {_ar_Coolpix_885_Set, _sz_Coolpix_885_Dir},
       {_ar_Coolpix_950_Set, _sz_Coolpix_950_Dir},
       {_ar_Coolpix_990_Set, _sz_Coolpix_990_Dir},
       {_ar_Coolpix_995_Set, _sz_Coolpix_995_Dir},
       {_ar_D1_Set, _sz_D1_Dir},
       {_ar_D100_Set, _sz_D100_Dir},
       {_ar_D1H_Set, _sz_D1H_Dir},
       {_ar_D1X_Set, _sz_D1X_Dir},
       {_ar_D2H_Set, _sz_D2H_Dir},
       {_ar_D70_Set, _sz_D70_Dir},
       {_ar_C_2000Z_Set, _sz_C_2000Z_Dir},
       {_ar_C_2100UZ_Set, _sz_C_2100UZ_Dir},
       {_ar_C_2500L_Set, _sz_C_2500L_Dir},
       {_ar_C_3030Z_Set, _sz_C_3030Z_Dir},
       {_ar_C_3040Z_Set, _sz_C_3040Z_Dir},
       {_ar_C_40Z_Set, _sz_C_40Z_Dir},
       {_ar_C_5050Z_Set, _sz_C_5050Z_Dir},
       {_ar_C_50Z_Set, _sz_C_50Z_Dir},
       {_ar_C_5500sz_Set, _sz_C_5500sz_Dir},
       {_ar_C_7000Z_Set, _sz_C_7000Z_Dir},
       {_ar_C_700UZ_Set, _sz_C_700UZ_Dir},
       {_ar_C_8080WZ_Set, _sz_C_8080WZ_Dir},
       {_ar_E_1_Set, _sz_E_1_Dir},
       {_ar_E_10_Set, _sz_E_10_Dir},
       {_ar_E_20_Set, _sz_E_20_Dir},
       {_ar_E_300_Set, _sz_E_300_Dir},
       {_ar_stylus_Verve_Set, _sz_stylus_Verve_Dir},
       {_ar_FX7_Set, _sz_FX7_Dir},
       {_ar_FZ20_Set, _sz_FZ20_Dir},
       {_ar_FZ3_Set, _sz_FZ3_Dir},
       {_ar_FZ5_Set, _sz_FZ5_Dir},
       {_ar_LZ2_Set, _sz_LZ2_Dir},
       {_ar_ist_D_Set, _sz_ist_D_Dir},
       {_ar_ist_DS_Set, _sz_ist_DS_Dir},
       {_ar_Optio_330_Set, _sz_Optio_330_Dir},
       {_ar_Optio_430_Set, _sz_Optio_430_Dir},
       {_ar_Optio_550_Set, _sz_Optio_550_Dir},
       {_ar_Optio_750_Set, _sz_Optio_750_Dir},
       {_ar_Optio_S_Set, _sz_Optio_S_Dir},
       {_ar_Optio_S5i_Set, _sz_Optio_S5i_Dir},
       {_ar_v700_Set, _sz_v700_Dir},
       {_ar_SD10_Set, _sz_SD10_Dir},
       {_ar_SD9_Set, _sz_SD9_Dir},
       {_ar_DSC_D700_Set, _sz_DSC_D700_Dir},
       {_ar_DSC_F505_Set, _sz_DSC_F505_Dir},
       {_ar_DSC_F505V_Set, _sz_DSC_F505V_Dir},
       {_ar_DSC_F707_Set, _sz_DSC_F707_Dir},
       {_ar_DSC_F717_Set, _sz_DSC_F717_Dir},
       {_ar_DSC_F828_Set, _sz_DSC_F828_Dir},
       {_ar_DSC_F88_Set, _sz_DSC_F88_Dir},
       {_ar_DSC_L1_Set, _sz_DSC_L1_Dir},
       {_ar_DSC_P1_Set, _sz_DSC_P1_Dir},
       {_ar_DSC_P10_Set, _sz_DSC_P10_Dir},
       {_ar_DSC_P150_Set, _sz_DSC_P150_Dir},
       {_ar_DSC_P5_Set, _sz_DSC_P5_Dir},
       {_ar_DSC_P71_Set, _sz_DSC_P71_Dir},
       {_ar_DSC_P9_Set, _sz_DSC_P9_Dir},
       {_ar_DSC_S70_Set, _sz_DSC_S70_Dir},
       {_ar_DSC_S75_Set, _sz_DSC_S75_Dir},
       {_ar_DSC_S85_Set, _sz_DSC_S85_Dir},
       {_ar_DSC_S90_Set, _sz_DSC_S90_Dir},
       {_ar_DSC_V1_Set, _sz_DSC_V1_Dir},
       {_ar_DSC_V3_Set, _sz_DSC_V3_Dir},
       {_ar_DSC_W7_Set, _sz_DSC_W7_Dir},
       {_ar_MVC_CD1000_Set, _sz_MVC_CD1000_Dir},
       {_ar_MVC_CD200_Set, _sz_MVC_CD200_Dir},
       {_ar_MVC_CD300_Set, _sz_MVC_CD300_Dir},
	{_arCanonNewSet, _szCanonNewDir},
	{_ar89CanNotSet, _sz89CanNotDir},
	//{_arProgr1Set, _szProgr1Dir}, //set 205
	{_ar04Dis2Set, _sz200504Dis2Dir},
      {_arAbortSet, _szAbortDir},
      {_arBP111Set, _szBP111Dir},
      {_arBP211Set, _szBP211Dir},
      {_arBP411Set, _szBP411Dir},
      {_arBrandSet, _szBrandDir},
      {_arFBallSet, _szFBallDir},
      {_arThumbSet, _szThumbDir},	 
      {_arVariousSet, _szVariousDir},	//set 213	
     // {_arErrorSet, _szErrorDir},    //set 214
	//{_arDefaultSet, _szDefaultDir}, //missing image	
	//{_arAgfaSet, _szAgfaDir},	 //multi SOI
};

void JPGCLI_GetTestSet(INT32 i4Index, JPGCLI_TEST_SET_T **pprSet, CHAR **pszInputDir)
{
	if (i4Index >= D_MAX_JPG_TEST_SETS)
	{
		i4Index = 0;
	}
	if ((pprSet == NULL) || (pszInputDir == NULL))
	{
		return;
	}
	*pprSet = _arTestSets[i4Index].prTestSet;
	*pszInputDir = _arTestSets[i4Index].szInputDir;
}

INT32 JPGCLI_GetMaxSet(void)
{
      return D_MAX_JPG_TEST_SETS;   
}

#endif

