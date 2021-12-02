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
 * Copyright (c) 2004, MediaTek, Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 * $RCSfile: sbtl_mp4_bluray_page_inst_mngr.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: $
 * $SWAuthor: $
 * $MD5HEX: $
 *
 * Description:
 *
 * History:
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__
#ifdef _WIN32_
#include "inc/x_os.h"
#else
#include "os/inc/x_os.h"
#endif
#include "os/inc/os.h"
#include "inc/x_common.h"
#include "handle/handle.h"
#include "handle/u_handle_grp.h"
#include "dbg/x_dbg.h"
#include "util/x_lnk_list.h"

#include "mutil/mm_util.h"

#include "mutil/sbtl_engine/sbtl_dbg.h"
#include "mutil/sbtl_engine/sbtl_struct.h"
#include "mutil/sbtl_engine/sbtl_utils.h"
#include "mutil/sbtl_engine/sbtl_register.h"
#include "mutil/sbtl_engine/sbtl_state.h"
#include "mutil/sbtl_engine/sbtl_widget.h"
#include "mutil/sbtl_engine/sbtl_mp4/sbtl_mp4_bluray_page_inst_mngr.h"

#include "inc/x_mm_common.h"


#else /*__KERNEL__*/
#include "x_os.h"
#include "x_common.h"
#include "handle.h"
#include "u_handle_grp.h"
#include "x_dbg.h"
#include "x_lnk_list.h"
#include "x_mm_common.h"

#include "../../mm_util.h"

#include "../sbtl_dbg.h"
#include "../sbtl_struct.h"
#include "../sbtl_utils.h"
#include "../sbtl_register.h"
#include "../sbtl_state.h"
#include "../sbtl_widget.h"

#include "sbtl_mp4_bluray_page_inst_mngr.h"

#endif /*__KERNEL__*/

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/


#define PTS_FREQ        ((UINT64)(90000))
#define TIMER_PERIOD    500

/* Color Transform Table */
static const INT32 ai4_Y_tbl[256] =
{
       -18624,   -17460,   -16296,   -15132,   -13968,   -12804,   -11640,   -10476,
        -9312,    -8148,    -6984,    -5820,    -4656,    -3492,    -2328,    -1164,
            0,     1164,     2328,     3492,     4656,     5820,     6984,     8148,
         9312,    10476,    11640,    12804,    13968,    15132,    16296,    17460,
        18624,    19788,    20952,    22116,    23280,    24444,    25608,    26772,
        27936,    29100,    30264,    31428,    32592,    33756,    34920,    36084,
        37248,    38412,    39576,    40740,    41904,    43068,    44232,    45396,
        46560,    47724,    48888,    50052,    51216,    52380,    53544,    54708,
        55872,    57036,    58200,    59364,    60528,    61692,    62856,    64020,
        65184,    66348,    67512,    68676,    69840,    71004,    72168,    73332,
        74496,    75660,    76824,    77988,    79152,    80316,    81480,    82644,
        83808,    84972,    86136,    87300,    88464,    89628,    90792,    91956,
        93120,    94284,    95448,    96612,    97776,    98940,   100104,   101268,
       102432,   103596,   104760,   105924,   107088,   108252,   109416,   110580,
       111744,   112908,   114072,   115236,   116400,   117564,   118728,   119892,
       121056,   122220,   123384,   124548,   125712,   126876,   128040,   129204,
       130368,   131532,   132696,   133860,   135024,   136188,   137352,   138516,
       139680,   140844,   142008,   143172,   144336,   145500,   146664,   147828,
       148992,   150156,   151320,   152484,   153648,   154812,   155976,   157140,
       158304,   159468,   160632,   161796,   162960,   164124,   165288,   166452,
       167616,   168780,   169944,   171108,   172272,   173436,   174600,   175764,
       176928,   178092,   179256,   180420,   181584,   182748,   183912,   185076,
       186240,   187404,   188568,   189732,   190896,   192060,   193224,   194388,
       195552,   196716,   197880,   199044,   200208,   201372,   202536,   203700,
       204864,   206028,   207192,   208356,   209520,   210684,   211848,   213012,
       214176,   215340,   216504,   217668,   218832,   219996,   221160,   222324,
       223488,   224652,   225816,   226980,   228144,   229308,   230472,   231636,
       232800,   233964,   235128,   236292,   237456,   238620,   239784,   240948,
       242112,   243276,   244440,   245604,   246768,   247932,   249096,   250260,
       251424,   252588,   253752,   254916,   256080,   257244,   258408,   259572,
       260736,   261900,   263064,   264228,   265392,   266556,   267720,   268884,
       270048,   271212,   272376,   273540,   274704,   275868,   277032,   278196,
};

static const INT32 ai4_Cr_R_tbl[256] =
{
      -204288,  -202692,  -201096,  -199500,  -197904,  -196308,  -194712,  -193116,
      -191520,  -189924,  -188328,  -186732,  -185136,  -183540,  -181944,  -180348,
      -178752,  -177156,  -175560,  -173964,  -172368,  -170772,  -169176,  -167580,
      -165984,  -164388,  -162792,  -161196,  -159600,  -158004,  -156408,  -154812,
      -153216,  -151620,  -150024,  -148428,  -146832,  -145236,  -143640,  -142044,
      -140448,  -138852,  -137256,  -135660,  -134064,  -132468,  -130872,  -129276,
      -127680,  -126084,  -124488,  -122892,  -121296,  -119700,  -118104,  -116508,
      -114912,  -113316,  -111720,  -110124,  -108528,  -106932,  -105336,  -103740,
      -102144,  -100548,   -98952,   -97356,   -95760,   -94164,   -92568,   -90972,
       -89376,   -87780,   -86184,   -84588,   -82992,   -81396,   -79800,   -78204,
       -76608,   -75012,   -73416,   -71820,   -70224,   -68628,   -67032,   -65436,
       -63840,   -62244,   -60648,   -59052,   -57456,   -55860,   -54264,   -52668,
       -51072,   -49476,   -47880,   -46284,   -44688,   -43092,   -41496,   -39900,
       -38304,   -36708,   -35112,   -33516,   -31920,   -30324,   -28728,   -27132,
       -25536,   -23940,   -22344,   -20748,   -19152,   -17556,   -15960,   -14364,
       -12768,   -11172,    -9576,    -7980,    -6384,    -4788,    -3192,    -1596,
            0,     1596,     3192,     4788,     6384,     7980,     9576,    11172,
        12768,    14364,    15960,    17556,    19152,    20748,    22344,    23940,
        25536,    27132,    28728,    30324,    31920,    33516,    35112,    36708,
        38304,    39900,    41496,    43092,    44688,    46284,    47880,    49476,
        51072,    52668,    54264,    55860,    57456,    59052,    60648,    62244,
        63840,    65436,    67032,    68628,    70224,    71820,    73416,    75012,
        76608,    78204,    79800,    81396,    82992,    84588,    86184,    87780,
        89376,    90972,    92568,    94164,    95760,    97356,    98952,   100548,
       102144,   103740,   105336,   106932,   108528,   110124,   111720,   113316,
       114912,   116508,   118104,   119700,   121296,   122892,   124488,   126084,
       127680,   129276,   130872,   132468,   134064,   135660,   137256,   138852,
       140448,   142044,   143640,   145236,   146832,   148428,   150024,   151620,
       153216,   154812,   156408,   158004,   159600,   161196,   162792,   164388,
       165984,   167580,   169176,   170772,   172368,   173964,   175560,   177156,
       178752,   180348,   181944,   183540,   185136,   186732,   188328,   189924,
       191520,   193116,   194712,   196308,   197904,   199500,   201096,   202692,
};

static const INT32 ai4_Cb_B_tbl[256] =
{
      -258304,  -256286,  -254268,  -252250,  -250232,  -248214,  -246196,  -244178,
      -242160,  -240142,  -238124,  -236106,  -234088,  -232070,  -230052,  -228034,
      -226016,  -223998,  -221980,  -219962,  -217944,  -215926,  -213908,  -211890,
      -209872,  -207854,  -205836,  -203818,  -201800,  -199782,  -197764,  -195746,
      -193728,  -191710,  -189692,  -187674,  -185656,  -183638,  -181620,  -179602,
      -177584,  -175566,  -173548,  -171530,  -169512,  -167494,  -165476,  -163458,
      -161440,  -159422,  -157404,  -155386,  -153368,  -151350,  -149332,  -147314,
      -145296,  -143278,  -141260,  -139242,  -137224,  -135206,  -133188,  -131170,
      -129152,  -127134,  -125116,  -123098,  -121080,  -119062,  -117044,  -115026,
      -113008,  -110990,  -108972,  -106954,  -104936,  -102918,  -100900,   -98882,
       -96864,   -94846,   -92828,   -90810,   -88792,   -86774,   -84756,   -82738,
       -80720,   -78702,   -76684,   -74666,   -72648,   -70630,   -68612,   -66594,
       -64576,   -62558,   -60540,   -58522,   -56504,   -54486,   -52468,   -50450,
       -48432,   -46414,   -44396,   -42378,   -40360,   -38342,   -36324,   -34306,
       -32288,   -30270,   -28252,   -26234,   -24216,   -22198,   -20180,   -18162,
       -16144,   -14126,   -12108,   -10090,    -8072,    -6054,    -4036,    -2018,
            0,     2018,     4036,     6054,     8072,    10090,    12108,    14126,
        16144,    18162,    20180,    22198,    24216,    26234,    28252,    30270,
        32288,    34306,    36324,    38342,    40360,    42378,    44396,    46414,
        48432,    50450,    52468,    54486,    56504,    58522,    60540,    62558,
        64576,    66594,    68612,    70630,    72648,    74666,    76684,    78702,
        80720,    82738,    84756,    86774,    88792,    90810,    92828,    94846,
        96864,    98882,   100900,   102918,   104936,   106954,   108972,   110990,
       113008,   115026,   117044,   119062,   121080,   123098,   125116,   127134,
       129152,   131170,   133188,   135206,   137224,   139242,   141260,   143278,
       145296,   147314,   149332,   151350,   153368,   155386,   157404,   159422,
       161440,   163458,   165476,   167494,   169512,   171530,   173548,   175566,
       177584,   179602,   181620,   183638,   185656,   187674,   189692,   191710,
       193728,   195746,   197764,   199782,   201800,   203818,   205836,   207854,
       209872,   211890,   213908,   215926,   217944,   219962,   221980,   223998,
       226016,   228034,   230052,   232070,   234088,   236106,   238124,   240142,
       242160,   244178,   246196,   248214,   250232,   252250,   254268,   256286,
};

static const INT32 ai4_Cr_G_tbl[256] =
{
      -104064,  -103251,  -102438,  -101625,  -100812,   -99999,   -99186,   -98373,
       -97560,   -96747,   -95934,   -95121,   -94308,   -93495,   -92682,   -91869,
       -91056,   -90243,   -89430,   -88617,   -87804,   -86991,   -86178,   -85365,
       -84552,   -83739,   -82926,   -82113,   -81300,   -80487,   -79674,   -78861,
       -78048,   -77235,   -76422,   -75609,   -74796,   -73983,   -73170,   -72357,
       -71544,   -70731,   -69918,   -69105,   -68292,   -67479,   -66666,   -65853,
       -65040,   -64227,   -63414,   -62601,   -61788,   -60975,   -60162,   -59349,
       -58536,   -57723,   -56910,   -56097,   -55284,   -54471,   -53658,   -52845,
       -52032,   -51219,   -50406,   -49593,   -48780,   -47967,   -47154,   -46341,
       -45528,   -44715,   -43902,   -43089,   -42276,   -41463,   -40650,   -39837,
       -39024,   -38211,   -37398,   -36585,   -35772,   -34959,   -34146,   -33333,
       -32520,   -31707,   -30894,   -30081,   -29268,   -28455,   -27642,   -26829,
       -26016,   -25203,   -24390,   -23577,   -22764,   -21951,   -21138,   -20325,
       -19512,   -18699,   -17886,   -17073,   -16260,   -15447,   -14634,   -13821,
       -13008,   -12195,   -11382,   -10569,    -9756,    -8943,    -8130,    -7317,
        -6504,    -5691,    -4878,    -4065,    -3252,    -2439,    -1626,     -813,
            0,      813,     1626,     2439,     3252,     4065,     4878,     5691,
         6504,     7317,     8130,     8943,     9756,    10569,    11382,    12195,
        13008,    13821,    14634,    15447,    16260,    17073,    17886,    18699,
        19512,    20325,    21138,    21951,    22764,    23577,    24390,    25203,
        26016,    26829,    27642,    28455,    29268,    30081,    30894,    31707,
        32520,    33333,    34146,    34959,    35772,    36585,    37398,    38211,
        39024,    39837,    40650,    41463,    42276,    43089,    43902,    44715,
        45528,    46341,    47154,    47967,    48780,    49593,    50406,    51219,
        52032,    52845,    53658,    54471,    55284,    56097,    56910,    57723,
        58536,    59349,    60162,    60975,    61788,    62601,    63414,    64227,
        65040,    65853,    66666,    67479,    68292,    69105,    69918,    70731,
        71544,    72357,    73170,    73983,    74796,    75609,    76422,    77235,
        78048,    78861,    79674,    80487,    81300,    82113,    82926,    83739,
        84552,    85365,    86178,    86991,    87804,    88617,    89430,    90243,
        91056,    91869,    92682,    93495,    94308,    95121,    95934,    96747,
        97560,    98373,    99186,    99999,   100812,   101625,   102438,   103251,
};

static const INT32 ai4_Cb_G_tbl[256] =
{
       -50048,   -49657,   -49266,   -48875,   -48484,   -48093,   -47702,   -47311,
       -46920,   -46529,   -46138,   -45747,   -45356,   -44965,   -44574,   -44183,
       -43792,   -43401,   -43010,   -42619,   -42228,   -41837,   -41446,   -41055,
       -40664,   -40273,   -39882,   -39491,   -39100,   -38709,   -38318,   -37927,
       -37536,   -37145,   -36754,   -36363,   -35972,   -35581,   -35190,   -34799,
       -34408,   -34017,   -33626,   -33235,   -32844,   -32453,   -32062,   -31671,
       -31280,   -30889,   -30498,   -30107,   -29716,   -29325,   -28934,   -28543,
       -28152,   -27761,   -27370,   -26979,   -26588,   -26197,   -25806,   -25415,
       -25024,   -24633,   -24242,   -23851,   -23460,   -23069,   -22678,   -22287,
       -21896,   -21505,   -21114,   -20723,   -20332,   -19941,   -19550,   -19159,
       -18768,   -18377,   -17986,   -17595,   -17204,   -16813,   -16422,   -16031,
       -15640,   -15249,   -14858,   -14467,   -14076,   -13685,   -13294,   -12903,
       -12512,   -12121,   -11730,   -11339,   -10948,   -10557,   -10166,    -9775,
        -9384,    -8993,    -8602,    -8211,    -7820,    -7429,    -7038,    -6647,
        -6256,    -5865,    -5474,    -5083,    -4692,    -4301,    -3910,    -3519,
        -3128,    -2737,    -2346,    -1955,    -1564,    -1173,     -782,     -391,
            0,      391,      782,     1173,     1564,     1955,     2346,     2737,
         3128,     3519,     3910,     4301,     4692,     5083,     5474,     5865,
         6256,     6647,     7038,     7429,     7820,     8211,     8602,     8993,
         9384,     9775,    10166,    10557,    10948,    11339,    11730,    12121,
        12512,    12903,    13294,    13685,    14076,    14467,    14858,    15249,
        15640,    16031,    16422,    16813,    17204,    17595,    17986,    18377,
        18768,    19159,    19550,    19941,    20332,    20723,    21114,    21505,
        21896,    22287,    22678,    23069,    23460,    23851,    24242,    24633,
        25024,    25415,    25806,    26197,    26588,    26979,    27370,    27761,
        28152,    28543,    28934,    29325,    29716,    30107,    30498,    30889,
        31280,    31671,    32062,    32453,    32844,    33235,    33626,    34017,
        34408,    34799,    35190,    35581,    35972,    36363,    36754,    37145,
        37536,    37927,    38318,    38709,    39100,    39491,    39882,    40273,
        40664,    41055,    41446,    41837,    42228,    42619,    43010,    43401,
        43792,    44183,    44574,    44965,    45356,    45747,    46138,    46529,
        46920,    47311,    47702,    48093,    48484,    48875,    49266,    49657,
};


/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
static VOID _sbtl_mp4_bluray_remove(SBTL_PAGE_INST_MNGR_T*  pt_mngr);

static INT32 _sbtl_mp4_bluray_start(SBTL_PAGE_INST_MNGR_T*  pt_mngr);

static INT32 _sbtl_mp4_bluray_stop(SBTL_PAGE_INST_MNGR_T*   pt_mngr);

static INT32 _sbtl_mp4_bluray_need_dmx(SBTL_PAGE_INST_MNGR_T*   pt_mngr, 
                                   BOOL*                    pb_need);

static INT32 _sbtl_mp4_bluray_handle_timing(SBTL_PAGE_INST_MNGR_T*  pt_mngr,
                                        PTS_T                   t_pts);

static INT32 _sbtl_mp4_bluray_config(SBTL_PAGE_INST_MNGR_T*     pt_mngr,
                                            VOID*                      pv_get_info,
                                 UINT32                     ui4_set_type);


static INT32 _sbtl_mp4_bluray_new_page(SBTL_PAGE_INST_MNGR_T*   pt_mngr);


static SBTL_PAGE_INST_MNGR_T* _mp4_bluray_page_mngr_constructor (SBTL_ENGINE_T* pt_engine);

static VOID _mp4_bluray_page_mngr_destructor (SBTL_PAGE_INST_MNGR_T*    pt_mngr);

static INT32 _sbtl_timer_start(SBTL_PAGE_INST_MNGR_T*  pt_mngr);
static VOID _sbtl_timer_stop(SBTL_PAGE_INST_MNGR_T*  pt_mngr);

/*-----------------------------------------------------------------------------
                    data declarations, extern, static, const
 ----------------------------------------------------------------------------*/


static SBTL_MNGR_FCT_TBL_T t_mp4_bluray_fnct_tbl = 
{
    FALSE,
    _mp4_bluray_page_mngr_constructor,
    _mp4_bluray_page_mngr_destructor,
    NULL
};

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _mp4_bluray_free_all(SBTL_MP4_BLURAY_PAGE_MNGR_PRIVATE_T*  pt_private)
{
    SBTL_MP4_BLURAY_SUB_INFO_T*         pt_sub_info = NULL;
    UINT8                               ui1_idx = 0;

    pt_sub_info = &pt_private->t_sub_info;
    if(pt_sub_info == NULL)
    {
        return;
    }
    
    for(ui1_idx = 0; ui1_idx < 2; ui1_idx++)
    {
        if(pt_sub_info->t_rgn_mngr.apt_rgn_lst[ui1_idx].pui1_rgn_buf != NULL)
        {
            x_mem_free(pt_sub_info->t_rgn_mngr.apt_rgn_lst[ui1_idx].pui1_rgn_buf);
            pt_sub_info->t_rgn_mngr.apt_rgn_lst[ui1_idx].pui1_rgn_buf = NULL;
            pt_sub_info->t_rgn_mngr.apt_rgn_lst[ui1_idx].ui4_rgn_buf_len= 0;
        }
    }

    for(ui1_idx = 0; ui1_idx < 2; ui1_idx++)
    {
        if(pt_sub_info->t_obj_mngr.apt_obj_lst[ui1_idx].pui1_data!= NULL)
        {
            x_mem_free(pt_sub_info->t_obj_mngr.apt_obj_lst[ui1_idx].pui1_data);
            pt_sub_info->t_obj_mngr.apt_obj_lst[ui1_idx].pui1_data = NULL;
            pt_sub_info->t_obj_mngr.apt_obj_lst[ui1_idx].ui4_data_len = 0;
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _mp4_bluray_parse_PCS(SBTL_MP4_BLURAY_PAGE_MNGR_PRIVATE_T*  pt_private,
                                                    UINT8*                      pui1_data,
                                                    UINT16                      ui4_data_len)
{
    UINT8                         ui1_idx;
    SBTL_MP4_BLURAY_SUB_INFO_T*   pt_sub_info;

    if(pt_private == NULL || pui1_data == NULL || ui4_data_len <=0)
    {
         return MMSBTLR_INV_ARG;
    }
    if(ui4_data_len < 11)
    {
        return MMSBTLR_WRONG_SEGMENT_DATA;
    }
    pt_sub_info = &pt_private->t_sub_info;

    pt_sub_info->ui2_video_width = pui1_data[0];
    pt_sub_info->ui2_video_width = pt_sub_info->ui2_video_width << 8;
    pt_sub_info->ui2_video_width += pt_sub_info->ui2_video_width + pui1_data[1];

    pt_sub_info->ui2_video_height= pui1_data[2];
    pt_sub_info->ui2_video_height = pt_sub_info->ui2_video_height << 8;
    pt_sub_info->ui2_video_height += pt_sub_info->ui2_video_height + pui1_data[3];

    pt_sub_info->ui1_video_frame_rate = (pui1_data[4] & 0xF0) >> 4;

    pt_sub_info->ui2_page_version_number= pui1_data[5];
    pt_sub_info->ui2_page_version_number = pt_sub_info->ui2_page_version_number << 8;
    pt_sub_info->ui2_page_version_number += pt_sub_info->ui2_page_version_number + pui1_data[6];
    
    pt_sub_info->ui1_page_state = (pui1_data[7] & 0xC0) >> 6;
    pt_sub_info->ui1_ref_clut_id = pui1_data[9];
    pt_sub_info->ui1_rgn_comp_cnt = pui1_data[10];

    if(pt_sub_info->ui1_rgn_comp_cnt == 0)
    {
        pt_private->b_is_empty_smaple = TRUE;
    }
    else
    {
        pt_private->b_is_empty_smaple = FALSE;
    }

    for(ui1_idx=0; ui1_idx<pt_sub_info->ui1_rgn_comp_cnt && ui1_idx < 2; ui1_idx++)
    {
        pt_sub_info->t_rgn_comp_lst[ui1_idx].ui2_ref_obj_id = pui1_data[11 + ui1_idx*8];
        pt_sub_info->t_rgn_comp_lst[ui1_idx].ui2_ref_obj_id = pt_sub_info->t_rgn_comp_lst[ui1_idx].ui2_ref_obj_id << 8;
        pt_sub_info->t_rgn_comp_lst[ui1_idx].ui2_ref_obj_id += pui1_data[11 + ui1_idx*8 + 1];

        pt_sub_info->t_rgn_comp_lst[ui1_idx].ui1_ref_rgn_id = pui1_data[11 + ui1_idx*8 + 2];

        pt_sub_info->t_rgn_comp_lst[ui1_idx].ui2_obj_h_position= pui1_data[11 + ui1_idx*8 + 4];
        pt_sub_info->t_rgn_comp_lst[ui1_idx].ui2_obj_h_position = pt_sub_info->t_rgn_comp_lst[ui1_idx].ui2_obj_h_position << 8;
        pt_sub_info->t_rgn_comp_lst[ui1_idx].ui2_obj_h_position += pui1_data[11 + ui1_idx*8 + 5];

        pt_sub_info->t_rgn_comp_lst[ui1_idx].ui2_obj_v_position = pui1_data[11 + ui1_idx*8 + 6];
        pt_sub_info->t_rgn_comp_lst[ui1_idx].ui2_obj_v_position = pt_sub_info->t_rgn_comp_lst[ui1_idx].ui2_obj_v_position << 8;
        pt_sub_info->t_rgn_comp_lst[ui1_idx].ui2_obj_v_position += pui1_data[11 + ui1_idx*8 + 7];
    }
    return MMSBTLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _mp4_bluray_parse_RDS(SBTL_MP4_BLURAY_PAGE_MNGR_PRIVATE_T*  pt_private,
                                                    UINT8*                      pui1_data,
                                                    UINT16                      ui4_data_len)
{
    UINT8                         ui1_idx;
    MM_SBTL_RGN_MNGR_T*           pt_rgn_mngr;

    if(pt_private == NULL || pui1_data == NULL || ui4_data_len <=0)
    {
         return MMSBTLR_INV_ARG;
    }
    if(ui4_data_len < 10)
    {
        return MMSBTLR_WRONG_SEGMENT_DATA;
    }
    pt_rgn_mngr = &pt_private->t_sub_info.t_rgn_mngr;

    pt_rgn_mngr->ui1_rgn_cnt = pui1_data[0];
    for(ui1_idx=0; ui1_idx<pt_rgn_mngr->ui1_rgn_cnt && ui1_idx < 2; ui1_idx++)
    {
        pt_rgn_mngr->apt_rgn_lst[ui1_idx].ui1_id = pui1_data[1 + ui1_idx*9];

        pt_rgn_mngr->apt_rgn_lst[ui1_idx].ui2_h_position = pui1_data[1 + ui1_idx*9 + 1];
        pt_rgn_mngr->apt_rgn_lst[ui1_idx].ui2_h_position = pt_rgn_mngr->apt_rgn_lst[ui1_idx].ui2_h_position << 8;
        pt_rgn_mngr->apt_rgn_lst[ui1_idx].ui2_h_position += pui1_data[1 + ui1_idx*9 + 2];

        pt_rgn_mngr->apt_rgn_lst[ui1_idx].ui2_v_position = pui1_data[1 + ui1_idx*9 + 3];
        pt_rgn_mngr->apt_rgn_lst[ui1_idx].ui2_v_position = pt_rgn_mngr->apt_rgn_lst[ui1_idx].ui2_v_position << 8;
        pt_rgn_mngr->apt_rgn_lst[ui1_idx].ui2_v_position += pui1_data[1 + ui1_idx*9 + 4];

        pt_rgn_mngr->apt_rgn_lst[ui1_idx].ui2_width= pui1_data[1 + ui1_idx*9 + 5];
        pt_rgn_mngr->apt_rgn_lst[ui1_idx].ui2_width = pt_rgn_mngr->apt_rgn_lst[ui1_idx].ui2_width << 8;
        pt_rgn_mngr->apt_rgn_lst[ui1_idx].ui2_width += pui1_data[1 + ui1_idx*9 + 6];

        pt_rgn_mngr->apt_rgn_lst[ui1_idx].ui2_height= pui1_data[1 + ui1_idx*9 + 7];
        pt_rgn_mngr->apt_rgn_lst[ui1_idx].ui2_height = pt_rgn_mngr->apt_rgn_lst[ui1_idx].ui2_height << 8;
        pt_rgn_mngr->apt_rgn_lst[ui1_idx].ui2_height += pui1_data[1 + ui1_idx*9 + 8];
    }

    return MMSBTLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _mp4_bluray_parse_CDS(SBTL_MP4_BLURAY_PAGE_MNGR_PRIVATE_T*  pt_private,
                                                    UINT8*                      pui1_data,
                                                    UINT16                      ui4_data_len)
{
    UINT8                         ui1_idx;
    UINT8                         ui1_entry_id;
    UINT8                         ui1_Y;
    UINT8                         ui1_CR;
    UINT8                         ui1_CB;
    INT32                         i4_R, i4_G, i4_B;
    MM_SBTL_CLUT_T*               pt_clut;

    if(pt_private == NULL || pui1_data == NULL || ui4_data_len <=0)
    {
         return MMSBTLR_INV_ARG;
    }
    if(ui4_data_len < 2)
    {
        return MMSBTLR_WRONG_SEGMENT_DATA;
    }

    pt_clut = &pt_private->t_sub_info.t_clut;
    pt_clut->ui1_id = pui1_data[0];
    pt_clut->ui1_version_number = pui1_data[1];
    if(pt_clut->ui1_id != pt_private->t_sub_info.ui1_ref_clut_id)
    {
        return MMSBTLR_WRONG_SEGMENT_DATA;
    }
    
    for(ui1_idx=0; ui1_idx<(ui4_data_len-2)/5; ui1_idx++)
    {
        ui1_entry_id = pui1_data[2 + ui1_idx*5];
        
        ui1_Y= pui1_data[2 + ui1_idx*5 + 1];
        ui1_CR = pui1_data[2 + ui1_idx*5 + 2];
        ui1_CB = pui1_data[2 + ui1_idx*5 + 3];

        i4_R = ai4_Y_tbl[ui1_Y] + ai4_Cr_R_tbl[ui1_CR];
        i4_G = ai4_Y_tbl[ui1_Y] - ai4_Cr_G_tbl[ui1_CR] - ai4_Cb_G_tbl[ui1_CB];
        i4_B = ai4_Y_tbl[ui1_Y] + ai4_Cb_B_tbl[ui1_CB];

        i4_R = (i4_R / 1000);
        if (i4_R < 0)
        {
            i4_R = 0;
        }
        else if (i4_R > 255)
        {
            i4_R = 255;
        }

        i4_G = (i4_G / 1000);
        if (i4_G < 0)
        {
            i4_G = 0;
        }
        else if (i4_G > 255)
        {
            i4_G = 255;
        }
        
        i4_B = (i4_B / 1000);
        if (i4_B < 0)
        {
            i4_B = 0;
        }
        else if (i4_B > 255)
        {
            i4_B = 255;
        }
        pt_clut->at_color_entry[ui1_entry_id].u1.r = (UINT8) i4_R;
        pt_clut->at_color_entry[ui1_entry_id].u2.g = (UINT8) i4_G;
        pt_clut->at_color_entry[ui1_entry_id].u3.b = (UINT8) i4_B;
        pt_clut->at_color_entry[ui1_entry_id].a = pui1_data[2 + ui1_idx*5 + 4];
    }

    return MMSBTLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _mp4_bluray_parse_ODS(SBTL_MP4_BLURAY_PAGE_MNGR_PRIVATE_T*  pt_private,
                                                    UINT8*                      pui1_data,
                                                    UINT16                      ui4_data_len)
{
    UINT8                         ui1_idx;
    MM_SBTL_OBJ_MNGR_T*           pt_obj_mngr;
    MM_SBTL_OBJ_T*                pt_obj;
    MM_SBTL_RGN_COMP_T*           pt_rgn_comp_list;

    if(pt_private == NULL || pui1_data == NULL || ui4_data_len <=0)
    {
         return MMSBTLR_INV_ARG;
    }

    pt_obj_mngr = &pt_private->t_sub_info.t_obj_mngr;
    pt_rgn_comp_list = pt_private->t_sub_info.t_rgn_comp_lst;

    if(pt_obj_mngr->ui1_obj_cnt >= 2 || 0xC0 != pui1_data[3])
    {
        return MMSBTLR_INV_ARG;
    }
    
    pt_obj = &pt_obj_mngr->apt_obj_lst[pt_obj_mngr->ui1_obj_cnt];
    
    pt_obj->ui2_id = pui1_data[0];
    pt_obj->ui2_id = pt_obj->ui2_id << 8;
    pt_obj->ui2_id += pui1_data[1];

    pt_obj->ui1_version_number = pui1_data[2];
    
    pt_obj->ui4_data_len = pui1_data[4];
    pt_obj->ui4_data_len = pt_obj->ui4_data_len << 8;
    pt_obj->ui4_data_len += pui1_data[5];
    pt_obj->ui4_data_len = pt_obj->ui4_data_len << 8;
    pt_obj->ui4_data_len += (pui1_data[6] - 4);

    pt_obj->ui2_h_size= pui1_data[7];
    pt_obj->ui2_h_size = pt_obj->ui2_h_size << 8;
    pt_obj->ui2_h_size += pui1_data[8];

    pt_obj->ui2_v_size= pui1_data[9];
    pt_obj->ui2_v_size = pt_obj->ui2_v_size << 8;
    pt_obj->ui2_v_size += pui1_data[10];
    
    if(NULL != pt_obj->pui1_data)
    {
        x_mem_free(pt_obj->pui1_data);
        pt_obj->pui1_data = NULL;
    }
    pt_obj->pui1_data = x_mem_alloc(pt_obj->ui4_data_len);
    if(NULL == pt_obj->pui1_data)
    {
        return MMSBTLR_INSUFFICIENT_MEMORY;
    }
    
    x_memcpy(pt_obj->pui1_data, pui1_data+11, pt_obj->ui4_data_len);
    
    //pt_obj->pui1_data = pui1_data + 11;
    pt_obj_mngr->ui1_obj_cnt++;
    for(ui1_idx = 0; ui1_idx < pt_private->t_sub_info.ui1_rgn_comp_cnt; ui1_idx++)
    {
        if(pt_rgn_comp_list[ui1_idx].ui2_ref_obj_id == pt_obj->ui2_id)
        {
            pt_obj->ui2_h_position = pt_rgn_comp_list[ui1_idx].ui2_obj_h_position;
            pt_obj->ui2_v_position = pt_rgn_comp_list[ui1_idx].ui2_obj_v_position;
        }
    }
    return MMSBTLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _mp4_bluray_parse_sample(SBTL_PAGE_INST_MNGR_T*   pt_mngr,
                                            PTS_T                   t_pts_current,
                                            PTS_T*                  pt_pts_start,
                                            PTS_T*                  pt_pts_end)
{
    BOOL                                    b_found;
    UINT32                                  ui4_data_1;
    UINT32                                  ui4_data_2;
    UINT32                                  ui4_data_3;
    UINT32                                  ui4_data_4;
    UINT32                                  ui4_idx = 0;
    UINT32                                  ui4_pts = 0;
    UINT32                                  ui4_pts_1 = 0;
    UINT32                                  ui4_duration = 0;
    INT32                                   i4_ret = MMSBTLR_OK;
    UINT8                                   ui1_sgt_type = 0;//segment type
    UINT16                                  ui2_sgt_len = 0;//segment length

    SBTL_MP4_BLURAY_PAGE_MNGR_PRIVATE_T*    pt_private;
        
    pt_private = (SBTL_MP4_BLURAY_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;

    while(1)
    {
        b_found = sbtl_dmx_data_que_retrieve(pt_mngr->pt_engine,
                                             &ui4_data_1,
                                             &ui4_data_2,
                                             &ui4_pts,
                                             &ui4_duration,
                                             &pt_private->t_append_stc);
        if (!b_found)
        {
            return MMSBTLR_NOT_FOUND;
        }

        b_found = sbtl_dmx_data_que_query(pt_mngr->pt_engine,
                                          &ui4_data_3,
                                          &ui4_data_4,
                                          &ui4_pts_1,
                                          &ui4_duration);
        if (!b_found || ui4_pts_1 > t_pts_current)
        {
               if (ui4_data_2 > pt_private->ui4_pes_buf_size)
            {
                if (pt_private->aui1_pes_buf)
                {
                    x_mem_free(pt_private->aui1_pes_buf);
                    pt_private->aui1_pes_buf = NULL;
                }
                pt_private->ui4_pes_buf_size = 0;
                pt_private->aui1_pes_buf = x_mem_alloc(ui4_data_2);
                
                ASSERT(pt_private->aui1_pes_buf, ("pt_private->aui1_pes_buf=NULL\n"));
                if (NULL == pt_private->aui1_pes_buf)
                {
                    return MMSBTLR_INSUFFICIENT_MEMORY;
                }
                pt_private->ui4_pes_buf_size = ui4_data_2;
            }
             
            i4_ret = sbtl_dmx_retrive_data(pt_mngr->pt_engine,
                                                   ui4_data_1,
                                                   ui4_data_2,
                                                   pt_private->aui1_pes_buf);
            if (MMSBTLR_OK != i4_ret)
            {
                ASSERT(0, ("DEMUX_GET_TYPE_READ_BUFFER fail\n"));
                return MMSBTLR_DMX_FAIL;
            }

             i4_ret = sbtl_dmx_release_data(pt_mngr->pt_engine,
                                               ui4_data_1,
                                               ui4_data_2);
            if (MMSBTLR_OK != i4_ret)
            {
                ASSERT(0, ("DEMUX_SET_TYPE_UNLOCK_BUFFER fail\n"));
                return MMSBTLR_DMX_FAIL;
            } 
            break;
        }
        else
        {
            i4_ret = sbtl_dmx_release_data(pt_mngr->pt_engine,
                                               ui4_data_1,
                                               ui4_data_2);
            if (MMSBTLR_OK != i4_ret)
            {
                ASSERT(0, ("DEMUX_SET_TYPE_UNLOCK_BUFFER fail\n"));
                return MMSBTLR_DMX_FAIL;
            } 
            continue;
        }    
    }
    *pt_pts_start = ui4_pts;
    *pt_pts_end= 0xFFFFFFFFFFFFFFFF;
    
    if(ui4_data_2 < 35)
    {
        return MMSBTLR_WRONG_SEGMENT_DATA;
    }
    
//parse PCS segment
    ui1_sgt_type = pt_private->aui1_pes_buf[ui4_idx];
    ui2_sgt_len = pt_private->aui1_pes_buf[ui4_idx+1];
    ui2_sgt_len = ui2_sgt_len << 8;
    ui2_sgt_len += pt_private->aui1_pes_buf[ui4_idx+2];

    if(0x16 == ui1_sgt_type && (ui4_idx + ui2_sgt_len + 3) <= ui4_data_2)
    {
        i4_ret = _mp4_bluray_parse_PCS(pt_private,
                                          &(pt_private->aui1_pes_buf[ui4_idx+3]),
                                          ui2_sgt_len);

        if (MMSBTLR_OK != i4_ret)
        {
            return (i4_ret);
        }
        ui4_idx += (ui2_sgt_len + 3);
    }
    else
    {
        return MMSBTLR_WRONG_SEGMENT_DATA;
    }
    
//parse RDS segment    
    ui1_sgt_type = pt_private->aui1_pes_buf[ui4_idx];
    ui2_sgt_len = pt_private->aui1_pes_buf[ui4_idx+1];
    ui2_sgt_len = ui2_sgt_len << 8;
    ui2_sgt_len += pt_private->aui1_pes_buf[ui4_idx+2];
    if(0x17 == ui1_sgt_type && (ui4_idx+ui2_sgt_len+3) <= ui4_data_2)
    {
        i4_ret = _mp4_bluray_parse_RDS(pt_private,
                                          &(pt_private->aui1_pes_buf[ui4_idx+3]),
                                          ui2_sgt_len);

        if (MMSBTLR_OK != i4_ret)
        {
            return (i4_ret);
        }
        ui4_idx += (ui2_sgt_len + 3);
    }
    else
    {
        return MMSBTLR_WRONG_SEGMENT_DATA;
    }
    
//parse CDS segment        
    ui1_sgt_type = pt_private->aui1_pes_buf[ui4_idx];
    ui2_sgt_len = pt_private->aui1_pes_buf[ui4_idx+1];
    ui2_sgt_len = ui2_sgt_len << 8;
    ui2_sgt_len += pt_private->aui1_pes_buf[ui4_idx+2];
    if(0x14 == ui1_sgt_type && (ui4_idx+ui2_sgt_len+3) <= ui4_data_2)
    {
        i4_ret = _mp4_bluray_parse_CDS(pt_private,
                                          &(pt_private->aui1_pes_buf[ui4_idx+3]),
                                          ui2_sgt_len);

        if (MMSBTLR_OK != i4_ret)
        {
            return (i4_ret);
        }
        ui4_idx += (ui2_sgt_len + 3);
    }
    else
    {
        return MMSBTLR_WRONG_SEGMENT_DATA;
    }
    
//parse ODS and end segment 
    pt_private->t_sub_info.t_obj_mngr.ui1_obj_cnt = 0; //clear count
    
    while(ui4_idx < ui4_data_2)
    {
        ui1_sgt_type = pt_private->aui1_pes_buf[ui4_idx];
        ui2_sgt_len = pt_private->aui1_pes_buf[ui4_idx+1];
        ui2_sgt_len = ui2_sgt_len << 8;
        ui2_sgt_len += pt_private->aui1_pes_buf[ui4_idx+2];
        
        switch(ui1_sgt_type)
        {            
            case 0x15:
                i4_ret = _mp4_bluray_parse_ODS(pt_private,
                                                  &(pt_private->aui1_pes_buf[ui4_idx+3]),
                                                  ui2_sgt_len);

                if (MMSBTLR_OK != i4_ret)
                {
                    return (i4_ret);
                }
                ui4_idx += (ui2_sgt_len + 3);
                break;

            case 0x80://end segment
                ui4_idx += (ui2_sgt_len + 3);
                break;

            default:
                return MMSBTLR_WRONG_SEGMENT_DATA;
                            
        }
    }
    if(ui4_idx == ui4_data_2)
    {
        return MMSBTLR_OK;
    }
    else
    {
        return MMSBTLR_WRONG_SEGMENT_DATA;
    }

}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID _mp4_bluray_rgn_paint(MM_SBTL_RGN_T*           pt_rgn,
                                MM_SBTL_OBJ_MNGR_T*      pt_obj_mngr)
{
    UINT8                    ui1_idx;
    UINT32                   ui4_data_ofst = 0;
    UINT32                   ui4_obj_ofst = 0;
    UINT32                   ui4_rgn_ofst = 0;
    UINT16                   ui2_pixel_num= 0;
    UINT16                   ui2_pixel_idx= 0;
    UINT16                   ui2_rel_x = 0;
    UINT16                   ui2_rel_y = 0;
    MM_SBTL_OBJ_T*           pt_obj = NULL;
        
    if(pt_rgn == NULL || pt_obj_mngr == NULL)
    {
        return;
    }
    pt_rgn->ui4_rgn_buf_len = pt_rgn->ui2_height*pt_rgn->ui2_width;
    if(NULL != pt_rgn->pui1_rgn_buf)
    {
        x_mem_free(pt_rgn->pui1_rgn_buf);
        pt_rgn->pui1_rgn_buf = NULL;
    }
    pt_rgn->pui1_rgn_buf = x_mem_alloc(pt_rgn->ui4_rgn_buf_len);
    if(NULL == pt_rgn->pui1_rgn_buf)
    {
        pt_rgn->ui4_rgn_buf_len = 0;
        return;
    }
    x_memset(pt_rgn->pui1_rgn_buf, 0xFF , pt_rgn->ui4_rgn_buf_len);//set background color as default
    
    for(ui1_idx = 0; ui1_idx < pt_obj_mngr->ui1_obj_cnt && ui1_idx < 2; ui1_idx++)//find relative object
    {
        if(pt_obj_mngr->apt_obj_lst[ui1_idx].ui2_id == pt_rgn->ui2_ref_obj_id)
        {
            pt_obj = &pt_obj_mngr->apt_obj_lst[ui1_idx];
            break;
        }
    }
    
    if(NULL != pt_obj && ui1_idx < pt_obj_mngr->ui1_obj_cnt)
    {
        if(pt_obj->ui2_h_position < pt_rgn->ui2_h_position|| pt_obj->ui2_h_size > pt_rgn->ui2_width
            || pt_obj->ui2_v_position < pt_rgn->ui2_v_position || pt_obj->ui2_v_size > pt_rgn->ui2_height)
        {
            return;
        }
                
        if(pt_obj->ui4_data_len == 0 || pt_obj->pui1_data == NULL)
        {
            return;
        }
        
        ui2_rel_x = pt_obj->ui2_h_position - pt_rgn->ui2_h_position;
        ui2_rel_y = pt_obj->ui2_v_position - pt_rgn->ui2_v_position;
        //run length encoding decode
        while(ui4_data_ofst < pt_obj->ui4_data_len && ui4_obj_ofst < pt_obj->ui2_h_size*pt_obj->ui2_v_size)
        {
            if(pt_obj->pui1_data[ui4_data_ofst] != 0x00)
            {
                ui4_rgn_ofst = (ui4_obj_ofst/pt_obj->ui2_h_size + ui2_rel_y)*pt_rgn->ui2_width
                                    + ui4_obj_ofst%pt_obj->ui2_h_size + ui2_rel_x;
                pt_rgn->pui1_rgn_buf[ui4_rgn_ofst] = pt_obj->pui1_data[ui4_data_ofst];
                ui4_data_ofst++;
                ui4_obj_ofst++;
            }
            else //00000000 ******** ********
            {
                if((pt_obj->pui1_data[ui4_data_ofst+1] & 0x80)>>7 == 0)// 0*******b
                {
                    if((pt_obj->pui1_data[ui4_data_ofst+1] & 0x40)>>6 == 0)// 00******b
                    {
                        ui2_pixel_num = pt_obj->pui1_data[ui4_data_ofst+1] & 0x3F;
                        if(ui2_pixel_num != 0)//run length zero 1-63 
                        {    
                            ui4_rgn_ofst = (ui4_obj_ofst/pt_obj->ui2_h_size + ui2_rel_y)*pt_rgn->ui2_width
                                    + ui4_obj_ofst%pt_obj->ui2_h_size + ui2_rel_x;
                            for(ui2_pixel_idx = 0; ui2_pixel_idx < ui2_pixel_num; ui2_pixel_idx++)
                            {                                    
                                pt_rgn->pui1_rgn_buf[ui4_rgn_ofst + ui2_pixel_idx] = 0;
                            }
                            ui4_obj_ofst += ui2_pixel_num;
                            ui4_data_ofst += 2;
                        }
                        else
                        {//00000000 00000000 means end of string signal
                            ui4_data_ofst += 2;
                        }
                    }
                    else if((pt_obj->pui1_data[ui4_data_ofst+1] & 0x40)>>6 == 1)// 01******b
                    {
                        ui2_pixel_num = pt_obj->pui1_data[ui4_data_ofst+1] & 0x3F;
                        ui2_pixel_num = ui2_pixel_num << 8;
                        ui2_pixel_num = ui2_pixel_num | pt_obj->pui1_data[ui4_data_ofst+2];
                        if(ui2_pixel_num > 63)//run length zero 64-16K 
                        {
                            ui4_rgn_ofst = (ui4_obj_ofst/pt_obj->ui2_h_size + ui2_rel_y)*pt_rgn->ui2_width
                                        + ui4_obj_ofst%pt_obj->ui2_h_size + ui2_rel_x;
                            for(ui2_pixel_idx = 0; ui2_pixel_idx < ui2_pixel_num; ui2_pixel_idx++)
                            {                                    
                                pt_rgn->pui1_rgn_buf[ui4_rgn_ofst + ui2_pixel_idx] = 0;
                            }
                            ui4_obj_ofst += ui2_pixel_num;
                            ui4_data_ofst += 3;
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                else if((pt_obj->pui1_data[ui4_data_ofst+1] & 0x80)>>7 == 1)// 1*******b
                {
                    if((pt_obj->pui1_data[ui4_data_ofst+1] & 0x40)>>6 == 0)// 10******b
                    {
                        ui2_pixel_num = pt_obj->pui1_data[ui4_data_ofst+1] & 0x3F;
                        if(ui2_pixel_num > 2)//run length 3_63 
                        {
                            ui4_rgn_ofst = (ui4_obj_ofst/pt_obj->ui2_h_size + ui2_rel_y)*pt_rgn->ui2_width
                                        + ui4_obj_ofst%pt_obj->ui2_h_size + ui2_rel_x;
                            for(ui2_pixel_idx = 0; ui2_pixel_idx < ui2_pixel_num; ui2_pixel_idx++)
                            {                                    
                                pt_rgn->pui1_rgn_buf[ui4_rgn_ofst + ui2_pixel_idx] = pt_obj->pui1_data[ui4_data_ofst+2];
                            }
                            ui4_obj_ofst += ui2_pixel_num;
                            ui4_data_ofst += 3;
                        }
                        else
                        {
                            break;
                        }
                    }
                    else if((pt_obj->pui1_data[ui4_data_ofst+1] & 0x40)>>6 == 1)// 11******b
                    {
                        ui2_pixel_num = pt_obj->pui1_data[ui4_data_ofst+1] & 0x3F;
                        ui2_pixel_num = ui2_pixel_num << 8;
                        ui2_pixel_num = ui2_pixel_num | pt_obj->pui1_data[ui4_data_ofst+2];
                        if(ui2_pixel_num > 63)//run length 64-16K 
                        {
                            ui4_rgn_ofst = (ui4_obj_ofst/pt_obj->ui2_h_size + ui2_rel_y)*pt_rgn->ui2_width
                                        + ui4_obj_ofst%pt_obj->ui2_h_size + ui2_rel_x;
                            for(ui2_pixel_idx = 0; ui2_pixel_idx < ui2_pixel_num; ui2_pixel_idx++)
                            {                                    
                                pt_rgn->pui1_rgn_buf[ui4_rgn_ofst + ui2_pixel_idx] = pt_obj->pui1_data[ui4_data_ofst+3];
                            }
                            ui4_obj_ofst += ui2_pixel_num;
                            ui4_data_ofst += 4;
                        }
                        else
                        {
                            break;
                        }
                    }
                }
            }
        }
        return;
    }
    return;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _mp4_bluray_paint_all_rgn(SBTL_PAGE_INST_MNGR_T*   pt_mngr)
{
    UINT8                                   ui1_i;
    UINT8                                   ui1_j;
    SBTL_MP4_BLURAY_PAGE_MNGR_PRIVATE_T*    pt_private;
    SBTL_MP4_BLURAY_SUB_INFO_T*             pt_sub_info;
        
    pt_private = (SBTL_MP4_BLURAY_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    pt_sub_info = &pt_private->t_sub_info;
    
    for(ui1_i = 0; ui1_i < pt_sub_info->t_rgn_mngr.ui1_rgn_cnt && ui1_i < 2; ui1_i++)
    {
        for(ui1_j = 0; ui1_j < pt_sub_info->ui1_rgn_comp_cnt && ui1_j < 2; ui1_j++)
        {
            if(pt_sub_info->t_rgn_comp_lst[ui1_j].ui1_ref_rgn_id 
                    == pt_sub_info->t_rgn_mngr.apt_rgn_lst[ui1_i].ui1_id)
            {
                pt_sub_info->t_rgn_mngr.apt_rgn_lst[ui1_i].ui2_ref_obj_id = pt_sub_info->t_rgn_comp_lst[ui1_j].ui2_ref_obj_id;
                _mp4_bluray_rgn_paint(&pt_sub_info->t_rgn_mngr.apt_rgn_lst[ui1_i], &pt_sub_info->t_obj_mngr);
            }
        }
    }
    
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static UINT32 _sbtl_timer_period(SBTL_PAGE_INST_MNGR_T*  pt_mngr)
{
    ASSERT(pt_mngr->pt_engine->t_cfg.i4_speed>0, ("pt_mngr->pt_engine.t_cfg.i4_speed=%d\n", pt_mngr->pt_engine->t_cfg.i4_speed));
    if (pt_mngr->pt_engine->t_cfg.i4_speed <= 0)
    {
        return 500;
    }
    
    return (UINT32)((TIMER_PERIOD * 100)/pt_mngr->pt_engine->t_cfg.i4_speed);
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _sbtl_timer_cb_fct(HANDLE_T     pt_tm_handle,
                               VOID*        pv_tag)
{
    SBTL_PAGE_INST_MNGR_T*  pt_mngr;
    MM_SBTL_HDLR_MSG_T      t_msg;

    ASSERT(pv_tag, ("pv_tag=NULL\n"));
    if (NULL==pv_tag)
    {
        return;
    }

    pt_mngr = (SBTL_PAGE_INST_MNGR_T*)pv_tag;
    mm_sbtl_hdlr_send_timer_msg(t_msg, pt_mngr);
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _sbtl_timer_start(SBTL_PAGE_INST_MNGR_T*  pt_mngr)
{
    SBTL_MP4_BLURAY_PAGE_MNGR_PRIVATE_T*  pt_private;
    INT32                             i4_ret;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    ASSERT(pt_mngr->pv_private, ("pt_mngr->pv_private=NULL\n"));
    if ((NULL==pt_mngr) || (NULL==pt_mngr->pv_private))
    {
        return MMSBTLR_INV_ARG;
    }

    pt_private = (SBTL_MP4_BLURAY_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;

    if (pt_private->h_timer)
    {
        x_timer_delete(pt_private->h_timer);
        pt_private->h_timer = NULL_HANDLE;
    }

    i4_ret = x_timer_create(&pt_private->h_timer);
    if (i4_ret != OSR_OK)
    {
        return MMSBTLR_CORE;
    }

    i4_ret = x_timer_start(pt_private->h_timer,
                           _sbtl_timer_period(pt_mngr),
                           X_TIMER_FLAG_REPEAT,
                           _sbtl_timer_cb_fct,
                           pt_mngr);
                    
    if (i4_ret != OSR_OK)
    {
        return MMSBTLR_CORE;
    }
    return MMSBTLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _sbtl_timer_stop(SBTL_PAGE_INST_MNGR_T*  pt_mngr)
{
    SBTL_MP4_BLURAY_PAGE_MNGR_PRIVATE_T*  pt_private;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    ASSERT(pt_mngr->pv_private, ("pt_mngr->pv_private=NULL\n"));
    if ((NULL==pt_mngr) || (NULL==pt_mngr->pv_private))
    {
        return;
    }

    pt_private = (SBTL_MP4_BLURAY_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;

    if (pt_private->h_timer)
    {
        x_timer_delete(pt_private->h_timer);
        pt_private->h_timer = NULL_HANDLE;
    }
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _sbtl_mp4_bluray_lock_pib_fct(HANDLE_T   h_page_inst,
                                     VOID*      pv_tag)
{
    return MMSBTLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _sbtl_mp4_bluray_unlock_pib_fct(HANDLE_T     h_page_inst,
                                         VOID*        pv_tag)
{
    SBTL_PAGE_INST_MNGR_T*    pt_mngr;
    SBTL_MP4_BLURAY_PAGE_MNGR_PRIVATE_T*  pt_private;
    
    ASSERT(pv_tag, ("_sbtl_mp4_bluray_unlock_pib_fct pv_tag=NULL\n"));
    if (NULL==pv_tag)
    {
        return MMSBTLR_INV_ARG;
    }

    pt_mngr = (SBTL_PAGE_INST_MNGR_T*)pv_tag;

    pt_private = (SBTL_MP4_BLURAY_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_private, ("_sbtl_mp4_bluray_unlock_pib_fct pt_private=NULL\n"));

    if (NULL==pt_private)
    {
        return MMSBTLR_NOT_FOUND;
    }

    x_sema_unlock(pt_private->h_sem);

    return MMSBTLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _sbtl_mp4_bluray_get_pib_rgn_fct(HANDLE_T                  h_page_inst,
                                          VOID*                     pv_tag,
                                          UINT16                    ui2_rgn_idx,
                                          SBTL_WGL_SBTL_REGION_T*   pt_rgn)
{
    SBTL_PAGE_INST_MNGR_T*    pt_mngr;
    SBTL_MP4_BLURAY_PAGE_MNGR_PRIVATE_T*  pt_private;
    
    ASSERT(pv_tag, ("_sbtl_mp4_bluray_get_pib_rgn_fct pv_tag=NULL\n"));
    ASSERT(pt_rgn, ("_sbtl_mp4_bluray_get_pib_rgn_fct pt_rgn=NULL\n"));
    if ((NULL==pv_tag) || (NULL==pt_rgn))
    {
        return MMSBTLR_INV_ARG;
    }

    pt_mngr = (SBTL_PAGE_INST_MNGR_T*)pv_tag;

    pt_private = (SBTL_MP4_BLURAY_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_private, ("_sbtl_mp4_bluray_get_pib_rgn_fct pt_private=NULL\n"));

    if (NULL == pt_private)
    {
        return MMSBTLR_NOT_FOUND;
    }
    
    if(ui2_rgn_idx < pt_private->t_sub_info.t_rgn_mngr.ui1_rgn_cnt && ui2_rgn_idx < 2)
    {
        pt_private->t_rgn.at_clut = pt_private->t_sub_info.t_clut.at_color_entry;
        
        pt_private->t_rgn.b_dirty = TRUE;
        pt_private->t_rgn.e_clr_mode = SBTL_WGL_SBTL_COLOR_MODE_CLUT_8_BIT;
        
        pt_private->t_rgn.i4_top = pt_private->t_sub_info.t_rgn_mngr.apt_rgn_lst[ui2_rgn_idx].ui2_v_position;
        pt_private->t_rgn.i4_left = pt_private->t_sub_info.t_rgn_mngr.apt_rgn_lst[ui2_rgn_idx].ui2_h_position;
        pt_private->t_rgn.ui4_width = pt_private->t_sub_info.t_rgn_mngr.apt_rgn_lst[ui2_rgn_idx].ui2_width;
        pt_private->t_rgn.ui4_height = pt_private->t_sub_info.t_rgn_mngr.apt_rgn_lst[ui2_rgn_idx].ui2_height;
        
        pt_private->t_rgn.t_rc_dirty.i4_bottom = (INT32)(pt_private->t_sub_info.t_rgn_mngr.apt_rgn_lst[ui2_rgn_idx].ui2_height);
        pt_private->t_rgn.t_rc_dirty.i4_top = 0;
        pt_private->t_rgn.t_rc_dirty.i4_left = 0;
        pt_private->t_rgn.t_rc_dirty.i4_right = (INT32)(pt_private->t_sub_info.t_rgn_mngr.apt_rgn_lst[ui2_rgn_idx].ui2_width);
        
        pt_private->t_rgn.pv_bitmap = pt_private->t_sub_info.t_rgn_mngr.apt_rgn_lst[ui2_rgn_idx].pui1_rgn_buf;
        
        *pt_rgn = pt_private->t_rgn;
            
        return MMSBTLR_OK;
    }
    
    return MMSBTLR_NOT_FOUND;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _sbtl_mp4_bluray_remove(SBTL_PAGE_INST_MNGR_T*    pt_mngr)
{
    SBTL_MP4_BLURAY_PAGE_MNGR_PRIVATE_T*  pt_private;
    INT32                           i4_ret;
    
    pt_private = (SBTL_MP4_BLURAY_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;

    //if (!pt_private->b_page_exist)
    //{
    //    return;
    //}
    
    if (pt_private->e_state == MP4_BLURAY_STATE_DISPLAY)
    {
        i4_ret = sbtl_wgl_do_cmd(pt_private->h_sbtl_wgl,
                                 SBTL_WGL_CMD_SBTL_CLEAR_ALL,
                                 (VOID*)&pt_private->t_pts_s,
                                 NULL);
            
        if (MMSBTLR_OK != i4_ret)
        {
            ASSERT(0, ("clear widget fail!!/n"));
        }
        
        i4_ret = x_sema_lock(pt_private->h_sem, X_SEMA_OPTION_WAIT);
        
        if (i4_ret != OSR_OK)
        {
            ASSERT(0, ("x_sema_lock fail %d\r\n", i4_ret));
        }
    }

    pt_private->e_state = MP4_BLURAY_STATE_NONE;
    pt_private->b_page_exist = FALSE;
    _mp4_bluray_free_all(pt_private);
    //x_mem_free(pt_private->t_rgn.pv_bitmap);
    x_memset(&pt_private->t_rgn, 0, sizeof(SBTL_WGL_SBTL_REGION_T));
    
    return;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32   _sbtl_mp4_bluray_start(SBTL_PAGE_INST_MNGR_T*  pt_mngr)
{
    INT32                               i4_ret;
    GL_RECT_T                           t_sbtl_rc_scrn;
    SBTL_WGL_SBTL_INIT_T                t_sbtl_init;
    UINT32                              ui4_canvas_w = 0;
    UINT32                              ui4_canvas_h = 0;
    HANDLE_T                            h_canvas;
    SBTL_MP4_BLURAY_PAGE_MNGR_PRIVATE_T*    pt_private;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    ASSERT(pt_mngr->pv_private, ("pt_mngr->pv_private=NULL\n"));
    if ((NULL==pt_mngr) || (NULL==pt_mngr->pv_private))
    {
        return MMSBTLR_INV_ARG;
    }

    pt_private = (SBTL_MP4_BLURAY_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    
    if (pt_private->h_sbtl_wgl)
    {
        return MMSBTLR_EXIST;
    }
    
    /* Get canvas */
    h_canvas = pt_mngr->pt_engine->t_cfg.h_gl_plane;
    
    
    i4_ret = sbtl_wgl_get_canvas_size(h_canvas, &ui4_canvas_w, &ui4_canvas_h);
    if (MMSBTLR_OK != i4_ret)
    {
        return (MMSBTLR_WGL_FAIL);
    }


    t_sbtl_rc_scrn.i4_left = 0;
    t_sbtl_rc_scrn.i4_top = 0;
    t_sbtl_rc_scrn.i4_bottom = (INT32)ui4_canvas_h;
    t_sbtl_rc_scrn.i4_right = (INT32)ui4_canvas_w;


    pt_private->ui4_logical_w = (UINT32)pt_mngr->pt_engine->t_cfg.t_subtitle_mp4.i4_width;
    pt_private->ui4_logical_h = (UINT32)pt_mngr->pt_engine->t_cfg.t_subtitle_mp4.i4_height;
    
    t_sbtl_init.ui4_page_width       = pt_private->ui4_logical_w;
    t_sbtl_init.ui4_page_height      = pt_private->ui4_logical_h;

    t_sbtl_init.pf_lock_pib          = _sbtl_mp4_bluray_lock_pib_fct;
    t_sbtl_init.pv_tag_lock_pib      = (VOID*) pt_mngr;
    
    t_sbtl_init.pf_unlock_pib        = _sbtl_mp4_bluray_unlock_pib_fct;
    t_sbtl_init.pv_tag_unlock_pib    = (VOID*) pt_mngr;
    
    t_sbtl_init.pf_get_page_inst_rgn = _sbtl_mp4_bluray_get_pib_rgn_fct;
    t_sbtl_init.pv_tag_get_rgn       = (VOID*) pt_mngr;
    
    i4_ret = sbtl_wgl_create_widget(h_canvas,
                                    SBTL_WGL_WIDGET_SBTL,
                                    SBTL_WGL_BORDER_NULL,
                                    &t_sbtl_rc_scrn,
                                    255,
                                    &t_sbtl_init,
                                    NULL,
                                    &(pt_private->h_sbtl_wgl));
    
    if (MMSBTLR_OK != i4_ret)
    {
       return (MMSBTLR_WGL_FAIL);
    }

    i4_ret = sbtl_wgl_show(pt_private->h_sbtl_wgl, TRUE);
    
    if (MMSBTLR_OK != i4_ret)
    {
       return (MMSBTLR_WGL_FAIL);
    }

    i4_ret = x_sema_create(&pt_private->h_sem, 
                           X_SEMA_TYPE_BINARY, 
                           X_SEMA_STATE_LOCK);
    if (i4_ret != OSR_OK)
    {
        return MMSBTLR_CORE;
    }

    _sbtl_timer_start(pt_mngr);

    pt_mngr->b_started = TRUE;
    
    _sbtl_mp4_bluray_new_page(pt_mngr);
    
    return MMSBTLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _sbtl_mp4_bluray_stop(SBTL_PAGE_INST_MNGR_T*   pt_mngr)
{
    SBTL_MP4_BLURAY_PAGE_MNGR_PRIVATE_T*  pt_private;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    ASSERT(pt_mngr->pv_private, ("pt_mngr->pv_private=NULL\n"));
    if ((NULL==pt_mngr) || (NULL==pt_mngr->pv_private))
    {
        return MMSBTLR_INV_ARG;
    }

    pt_private = (SBTL_MP4_BLURAY_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;

    _sbtl_timer_stop(pt_mngr);

    _sbtl_mp4_bluray_remove(pt_mngr);
    
    /* The widget had already been closed */
    if (pt_private->h_sbtl_wgl)
    {
        sbtl_wgl_destroy_widget(pt_private->h_sbtl_wgl);
        pt_private->h_sbtl_wgl = NULL_HANDLE;
    }

    x_sema_delete(pt_private->h_sem);
    pt_private->h_sem = NULL_HANDLE;

    pt_mngr->b_started = FALSE;
    pt_private->e_state = MP4_BLURAY_STATE_NONE;
    pt_private->b_page_exist = FALSE;
    return MMSBTLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _sbtl_mp4_bluray_need_dmx(SBTL_PAGE_INST_MNGR_T*   pt_mngr, 
                                   BOOL*                    pb_need)
{
    *pb_need = TRUE;
    return MMSBTLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _sbtl_mp4_bluray_handle_timing(SBTL_PAGE_INST_MNGR_T*  pt_mngr,
                                        PTS_T                   t_pts)
{
    SBTL_MP4_BLURAY_PAGE_MNGR_PRIVATE_T*    pt_private;
    INT32                               i4_ret;
    PTS_T                               t_pts_current;
    
    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    ASSERT(pt_mngr->pv_private, ("pt_mngr->pv_private=NULL\n"));
    if ((NULL==pt_mngr) || (NULL==pt_mngr->pv_private))
    {
        return MMSBTLR_INV_ARG;
    }

    pt_private = (SBTL_MP4_BLURAY_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;

    i4_ret = sbtl_stc_pts(pt_mngr->pt_engine, &t_pts_current);

    if (i4_ret != MMSBTLR_OK)
    {
        ASSERT(0, ("Get STC Error!!\n"));
    }
        
    if (pt_private->b_page_exist)
    {
        if (t_pts < pt_private->t_pts_s)
        {
            pt_private->e_state = MP4_BLURAY_STATE_DATA_RDY;
            return MMSBTLR_OK;
        }
        else 
        {
            if (pt_private->e_state != MP4_BLURAY_STATE_DISPLAY)
            {
                i4_ret = sbtl_wgl_do_cmd(pt_private->h_sbtl_wgl,
                                     SBTL_WGL_CMD_SBTL_CLEAR_ALL,
                                     (VOID*)&pt_private->t_pts_pre_s,
                                     NULL);
                
                if (MMSBTLR_OK != i4_ret)
                {
                    ASSERT(0, ("clear widget fail!!/n"));
                }
                if(!pt_private->b_is_empty_smaple)
                {
                    i4_ret = sbtl_wgl_do_cmd(pt_private->h_sbtl_wgl,
                                          SBTL_WGL_CMD_SBTL_FLIP,
                                          (VOID*)pt_mngr,
                                          (VOID*)&pt_private->t_pts_s);
                    
                    if (i4_ret != MMSBTLR_OK)
                    {
                        ASSERT(0, ("wgl flip error!!\n"));
                    }
                }
            }
            pt_private->e_state = MP4_BLURAY_STATE_DISPLAY;
            pt_private->b_page_exist = FALSE;
            return MMSBTLR_OK;
        }
    }
    else
    {
        _sbtl_mp4_bluray_new_page(pt_mngr);
    }
    return MMSBTLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _sbtl_mp4_bluray_config(SBTL_PAGE_INST_MNGR_T*     pt_mngr,
                                            VOID*                      pv_get_info,
                                 UINT32                     ui4_set_type)
{
    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    if (NULL==pt_mngr)
    {
        return MMSBTLR_INV_ARG;
    }

    switch (ui4_set_type)
    {
    case EVN_MM_SBTL_SET_SPEED:
        _sbtl_timer_start(pt_mngr);
        break;

    default:
        break;
    }

    return MMSBTLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _sbtl_mp4_bluray_new_page(SBTL_PAGE_INST_MNGR_T*  pt_mngr)
{
    INT32                               i4_ret;
    SBTL_MP4_BLURAY_PAGE_MNGR_PRIVATE_T*    pt_private;
    PTS_T                               t_pts_start = 0;
    PTS_T                               t_pts_end = 0;
    PTS_T                               t_pts_current;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    ASSERT(pt_mngr->pv_private, ("pt_mngr->pv_private=NULL\n"));
    ASSERT(pt_mngr->pt_engine, ("pt_mngr->pt_engine=NULL\n"));
    if ((NULL==pt_mngr) || (NULL==pt_mngr->pv_private) || (NULL==pt_mngr->pt_engine))
    {
        return MMSBTLR_INV_ARG;
    }

    pt_private = (SBTL_MP4_BLURAY_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;

    if (!pt_mngr->b_started)
    {
        return MMSBTLR_OK;
    }
    
    if (pt_private->b_page_exist)
    {
        return MMSBTLR_OK;
    }
    
    pt_private->e_state = MP4_BLURAY_STATE_NONE;
    
    do
    {
        i4_ret = sbtl_stc_pts(pt_mngr->pt_engine, &t_pts_current);

        if (i4_ret != MMSBTLR_OK)
        {
            ASSERT(0, ("Get STC Error!!\n"));
        }

        i4_ret = _mp4_bluray_parse_sample(pt_mngr, t_pts_current, &t_pts_start, &t_pts_end);
        
        if (i4_ret != MMSBTLR_OK)
        {

            return i4_ret;
        }
        
        _mp4_bluray_paint_all_rgn(pt_mngr);  
        pt_private->t_pts_pre_s = pt_private->t_pts_s;
        pt_private->t_pts_s = t_pts_start;
        pt_private->t_pts_e = t_pts_end;

       
        if (t_pts_current < t_pts_start)
        {
            pt_private->e_state = MP4_BLURAY_STATE_DATA_RDY;
            pt_private->b_page_exist = TRUE;
            
            return MMSBTLR_OK;
        }
        else
        {
            i4_ret = sbtl_wgl_do_cmd(pt_private->h_sbtl_wgl,
                                     SBTL_WGL_CMD_SBTL_CLEAR_ALL,
                                     (VOID*)&pt_private->t_pts_pre_s,
                                     NULL);
                
            if (MMSBTLR_OK != i4_ret)
            {
                ASSERT(0, ("clear widget fail!!/n"));
            }
            if(!pt_private->b_is_empty_smaple)
            {
                /*display*/
                
                i4_ret = sbtl_wgl_do_cmd(pt_private->h_sbtl_wgl,
                                         SBTL_WGL_CMD_SBTL_FLIP,
                                         (VOID*)pt_mngr,
                                         (VOID*)&pt_private->t_pts_s);
                
                if (i4_ret != MMSBTLR_OK)
                {
                    ASSERT(0, ("wgl flip error!!\n"));
                }
            }
            pt_private->e_state = MP4_BLURAY_STATE_DISPLAY;
            pt_private->b_page_exist = FALSE;
            
            return MMSBTLR_OK;
        }
        
    }while(1);
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static SBTL_PAGE_INST_MNGR_T* _mp4_bluray_page_mngr_constructor (SBTL_ENGINE_T*   pt_engine)
{
    SBTL_PAGE_INST_MNGR_T*                  pt_mngr;
    SBTL_MP4_BLURAY_PAGE_MNGR_PRIVATE_T*    pt_private;
    
    ASSERT(pt_engine, ("_mp4_bluray_page_mngr_constructor pt_engine=NULL\n"));
    if (NULL==pt_engine)
    {
        return NULL;
    }

    do
    {
        pt_mngr = (SBTL_PAGE_INST_MNGR_T*)x_mem_alloc(sizeof(SBTL_PAGE_INST_MNGR_T));
        if (NULL == pt_mngr)
        {
            break;
        }
        x_memset(pt_mngr, 0, sizeof(SBTL_PAGE_INST_MNGR_T));

        pt_mngr->pf_start = _sbtl_mp4_bluray_start;
        pt_mngr->pf_stop = _sbtl_mp4_bluray_stop;
        pt_mngr->pf_cfg = _sbtl_mp4_bluray_config;
        pt_mngr->pf_handle_timing = _sbtl_mp4_bluray_handle_timing;
        pt_mngr->pf_need_dmx = _sbtl_mp4_bluray_need_dmx;
        pt_mngr->pf_new_page = _sbtl_mp4_bluray_new_page;
        
        pt_mngr->pt_engine = pt_engine;
        
        pt_mngr->pv_private = x_mem_alloc(sizeof(SBTL_MP4_BLURAY_PAGE_MNGR_PRIVATE_T));

        if (NULL == pt_mngr->pv_private)
        {
            break;
        }

        pt_private = (SBTL_MP4_BLURAY_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
        x_memset(pt_private, 0, sizeof(SBTL_MP4_BLURAY_PAGE_MNGR_PRIVATE_T));

        return pt_mngr;
    }while(0);

    if (pt_mngr)
    {
        if (pt_mngr->pv_private)
        {
            x_mem_free(pt_mngr->pv_private);
        }
        x_mem_free(pt_mngr);
    }
    return NULL;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _mp4_bluray_page_mngr_destructor (SBTL_PAGE_INST_MNGR_T*  pt_mngr)
{
    SBTL_MP4_BLURAY_PAGE_MNGR_PRIVATE_T*  pt_private;

    ASSERT(pt_mngr, ("_mp4_bluray_page_mngr_destructor pt_mngr=NULL\n"));
    if (NULL==pt_mngr)
    {
        return;
    }
    
    pt_private = (SBTL_MP4_BLURAY_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_private, ("_mp4_bluray_page_mngr_destructor pt_private=NULL\n"));
    if (NULL==pt_private)
    {
        return;
    }
    
    if (pt_private->aui1_pes_buf)
    {
        x_mem_free(pt_private->aui1_pes_buf);
    }

    x_mem_free(pt_private);
    pt_mngr->pt_engine->pt_page_mngr = NULL;
    x_mem_free(pt_mngr);
}

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID x_sbtl_mp4_bluray_init(VOID)
{
    sbtl_register(MM_SBTL_TYPE_MP4_BLURAY, 
                  &t_mp4_bluray_fnct_tbl);
}

