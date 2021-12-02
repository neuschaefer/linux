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

/*************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile: drv_contrast_int.c $
 *
 * Project:
 * --------
 *
 *
 * Description:
 * ------------
 *
 *
 * Author:
 * -------
 *
 *
 * Last changed:
 * -------------
 * $Author: dtvbm11 $
 *
 * $Modtime: 06/06/01 13:00p $
 *
 * $Revision: #1 $
*************************************************************************/


#include "drv_contrast.h"
#include "drv_ycproc.h"
#include "api_backlt.h"
#include "nptv_debug.h"
#include "hw_nr.h"

#define AL_LOW_START    8
#define AL_LOW_END      32
#define	AL_HIGH_START   32
#define	AL_HIGH_END     52
#define	AL_LOW_BIN      13
#define	AL_HIGH_BIN     9
#define AbsDiff(a, b)  (((a)>(b))?((a)-(b)):((b)-(a)))

#define COLOR_DEPEND_BS_RATIO 1

#if 0
#define AL_DBG(x...) LOG(3, x)
#define AL_INFO(x...) LOG(3, x)
#else
#define AL_DBG(x...) do { } while (0)
#define AL_INFO(x...) do { } while (0)
#endif


const CODE UINT16 GAMMA_105_TABLE[257]=
{
    0   , 3     , 6     , 10    , 13    , 16    , 20    , 23    , 27    , 30    , 34    , 38    , 41    , 45    , 48    , 52,
    56  , 59    , 63    , 67    , 70    , 74    , 78    , 82    , 85    , 89    , 93    , 97    , 100   , 104   , 108   , 112,
    115 , 119   , 123   , 127   , 131   , 134   , 138   , 142   , 146   , 150   , 153   , 157   , 161   , 165   , 169   , 173,
    177 , 180   , 184   , 188   , 192   , 196   , 200   , 204   , 208   , 212   , 215   , 219   , 223   , 227   , 231   , 235,
    239 , 243   , 247   , 251   , 255   , 258   , 262   , 266   , 270   , 274   , 278   , 282   , 286   , 290   , 294   , 298,
    302 , 306   , 310   , 314   , 318   , 322   , 326   , 330   , 334   , 338   , 342   , 346   , 350   , 354   , 358   , 362,
    366 , 370   , 374   , 378   , 382   , 386   , 390   , 394   , 398   , 402   , 406   , 410   , 414   , 418   , 422   , 426,
    430 , 434   , 438   , 442   , 446   , 450   , 454   , 458   , 462   , 466   , 470   , 474   , 478   , 482   , 486   , 491,
    495 , 499   , 503   , 507   , 511   , 515   , 519   , 523   , 527   , 531   , 535   , 539   , 543   , 547   , 552   , 556,
    560 , 564   , 568   , 572   , 576   , 580   , 584   , 588   , 592   , 596   , 601   , 605   , 609   , 613   , 617   , 621,
    625 , 629   , 633   , 637   , 642   , 646   , 650   , 654   , 658   , 662   , 666   , 670   , 674   , 679   , 683   , 687,
    691 , 695   , 699   , 703   , 707   , 712   , 716   , 720   , 724   , 728   , 732   , 736   , 740   , 745   , 749   , 753,
    757 , 761   , 765   , 769   , 774   , 778   , 782   , 786   , 790   , 794   , 798   , 803   , 807   , 811   , 815   , 819,
    823 , 828   , 832   , 836   , 840   , 844   , 848   , 853   , 857   , 861   , 865   , 869   , 873   , 878   , 882   , 886,
    890 , 894   , 898   , 903   , 907   , 911   , 915   , 919   , 923   , 928   , 932   , 936   , 940   , 944   , 949   , 953,
    957 , 961   , 965   , 969   , 974   , 978   , 982   , 986   , 990   , 995   , 999   , 1003  , 1007  , 1011  , 1016  , 1020,1024
};


const CODE UINT16 GAMMA_110_TABLE[257]=
{
    0   , 2     , 5     , 8     , 11    , 13    , 16    , 20    , 23    , 26    , 29    , 32    , 35    , 39    , 42    , 45    ,
    49  , 52    , 55    , 59    , 62    , 65    , 69    , 72    , 76    , 79    , 83    , 86    , 90    , 93    , 97    , 100   ,
    104 , 108   , 111   , 115   , 118   , 122   , 126   , 129   , 133   , 137   , 140   , 144   , 148   , 151   , 155   , 159   ,
    162 , 166   , 170   , 174   , 177   , 181   , 185   , 189   , 192   , 196   , 200   , 204   , 208   , 211   , 215   , 219   ,
    223 , 227   , 231   , 234   , 238   , 242   , 246   , 250   , 254   , 258   , 261   , 265   , 269   , 273   , 277   , 281   ,
    285 , 289   , 293   , 297   , 301   , 305   , 308   , 312   , 316   , 320   , 324   , 328   , 332   , 336   , 340   , 344   ,   
    348 , 352   , 356   , 360   , 364   , 368   , 372   , 376   , 380   , 384   , 388   , 392   , 396   , 400   , 404   , 408   ,
    412 , 417   , 421   , 425   , 429   , 433   , 437   , 441   , 445   , 449   , 453   , 457   , 461   , 465   , 470   , 474   ,
    478 , 482   , 486   , 490   , 494   , 498   , 502   , 507   , 511   , 515   , 519   , 523   , 527   , 531   , 535   , 540   ,
    544 , 548   , 552   , 556   , 560   , 565   , 569   , 573   , 577   , 581   , 585   , 590   , 594   , 598   , 602   , 606   ,
    611 , 615   , 619   , 623   , 627   , 632   , 636   , 640   , 644   , 649   , 653   , 657   , 661   , 665   , 670   , 674   ,
    678 , 682   , 687   , 691   , 695   , 699   , 704   , 708   , 712   , 716   , 721   , 725   , 729   , 733   , 738   , 742   ,
    746 , 750   , 755   , 759   , 763   , 768   , 772   , 776   , 780   , 785   , 789   , 793   , 798   , 802   , 806   , 811   ,
    815 , 819   , 824   , 828   , 832   , 836   , 841   , 845   , 849   , 854   , 858   , 862   , 867   , 871   , 875   , 880   ,
    884 , 888   , 893   , 897   , 901   , 906   , 910   , 915   , 919   , 923   , 928   , 932   , 936   , 941   , 945   , 949   ,
    954 , 958   , 963   , 967   , 971   , 976   , 980   , 984   , 989   , 993   , 998   , 1002  , 1006  , 1011  , 1015  , 1020,1024
};

const CODE UINT16 GAMMA_115_TABLE[257]=
{
    0   , 2     , 4     , 6     , 9     , 11    , 14    , 16    , 19    , 22    , 25    , 27    , 30    , 33    , 36    , 39,
    42  , 45    , 48    , 51    , 55    , 58    , 61    , 64    , 67    , 71    , 74    , 77    , 80    , 84    , 87    , 90,
    94  , 97    , 100   , 104   , 107   , 111   , 114   , 118   , 121   , 125   , 128   , 132   , 135   , 139   , 142   , 146,
    149 , 153   , 157   , 160   , 164   , 167   , 171   , 175   , 178   , 182   , 186   , 189   , 193   , 197   , 200   , 204,
    208 , 212   , 215   , 219   , 223   , 227   , 231   , 234   , 238   , 242   , 246   , 250   , 253   , 257   , 261   , 265,
    269 , 273   , 277   , 280   , 284   , 288   , 292   , 296   , 300   , 304   , 308   , 312   , 316   , 320   , 324   , 327,
    331 , 335   , 339   , 343   , 347   , 351   , 355   , 359   , 363   , 367   , 371   , 376   , 380   , 384   , 388   , 392,
    396 , 400   , 404   , 408   , 412   , 416   , 420   , 424   , 428   , 433   , 437   , 441   , 445   , 449   , 453   , 457,
    461 , 466   , 470   , 474   , 478   , 482   , 486   , 491   , 495   , 499   , 503   , 507   , 512   , 516   , 520   , 524,
    528 , 533   , 537   , 541   , 545   , 550   , 554   , 558   , 562   , 567   , 571   , 575   , 579   , 584   , 588   , 592,
    596 , 601   , 605   , 609   , 614   , 618   , 622   , 627   , 631   , 635   , 639   , 644   , 648   , 652   , 657   , 661,
    666 , 670   , 674   , 679   , 683   , 687   , 692   , 696   , 700   , 705   , 709   , 714   , 718   , 722   , 727   , 731,
    736 , 740   , 744   , 749   , 753   , 758   , 762   , 766   , 771   , 775   , 780   , 784   , 789   , 793   , 798   , 802,
    806 , 811   , 815   , 820   , 824   , 829   , 833   , 838   , 842   , 847   , 851   , 856   , 860   , 865   , 869   , 874,
    878 , 883   , 887   , 892   , 896   , 901   , 905   , 910   , 914   , 919   , 923   , 928   , 933   , 937   , 942   , 946,
    951 , 955   , 960   , 964   , 969   , 974   , 978   , 983   , 987   , 992   , 996   , 1001  , 1006  , 1010  , 1015  , 1019,1024
};




const CODE UINT16 GAMMA_950_TABLE[257]=
{
	0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60,
	64, 69, 74, 79, 84, 88, 93, 97, 102, 106, 111, 115, 120, 124, 128, 133,
	137, 141, 146, 150, 154, 158, 163, 167, 171, 175, 180, 184, 188, 192, 197, 201,
	205, 209, 213, 217, 222, 226, 230, 234, 238, 242, 246, 251, 255, 259, 263, 267,
	271, 275, 279, 283, 288, 292, 296, 300, 304, 308, 312, 316, 320, 324, 328, 332,
	336, 340, 344, 348, 352, 356, 360, 364, 368, 372, 377, 381, 385, 389, 393, 397,
	401, 405, 409, 413, 416, 420, 424, 428, 432, 436, 440, 444, 448, 452, 456, 460,
	464, 468, 472, 476, 480, 484, 488, 492, 496, 500, 504, 508, 512, 515, 519, 523,
	527, 531, 535, 539, 543, 547, 551, 555, 559, 563, 566, 570, 574, 578, 582, 586,
	590, 594, 598, 602, 606, 609, 613, 617, 621, 625, 629, 633, 637, 641, 644, 648,
	652, 656, 660, 664, 668, 672, 675, 679, 683, 687, 691, 695, 699, 703, 706, 710,
	714, 718, 722, 726, 730, 733, 737, 741, 745, 749, 753, 757, 760, 764, 768, 772,
	776, 780, 783, 787, 791, 795, 799, 803, 806, 810, 814, 818, 822, 826, 829, 833,
	837, 841, 845, 849, 852, 856, 860, 864, 868, 871, 875, 879, 883, 887, 891, 894,
	898, 902, 906, 910, 913, 917, 921, 925, 929, 932, 936, 940, 944, 948, 952, 956,
	960, 964, 968, 972, 976, 980, 984, 988, 992, 996, 1000, 1004, 1008, 1012, 1016, 1020,1024
};

const CODE UINT16  GAMMA_925_TABLE[257]=
{
	0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60,
	64, 70, 75, 81, 86, 91, 95, 100, 105, 110, 114, 119, 124, 128, 133, 137,
	142, 146, 151, 155, 160, 164, 169, 173, 177, 182, 186, 190, 195, 199, 203, 208,
	212, 216, 220, 225, 229, 233, 237, 242, 246, 250, 254, 258, 263, 267, 271, 275,
	279, 283, 287, 292, 296, 300, 304, 308, 312, 316, 320, 324, 328, 333, 337, 341,
	345, 349, 353, 357, 361, 365, 369, 373, 377, 381, 385, 389, 393, 397, 401, 405,
	409, 413, 417, 421, 425, 429, 433, 437, 441, 445, 449, 453, 457, 461, 465, 469,
	473, 476, 480, 484, 488, 492, 496, 500, 504, 508, 512, 516, 520, 523, 527, 531,
	535, 539, 543, 547, 551, 555, 558, 562, 566, 570, 574, 578, 582, 585, 589, 593,
	597, 601, 605, 609, 612, 616, 620, 624, 628, 632, 635, 639, 643, 647, 651, 655,
	658, 662, 666, 670, 674, 677, 681, 685, 689, 693, 696, 700, 704, 708, 712, 715,
	719, 723, 727, 731, 734, 738, 742, 746, 749, 753, 757, 761, 765, 768, 772, 776,
	780, 783, 787, 791, 795, 798, 802, 806, 810, 813, 817, 821, 825, 828, 832, 836,
	840, 843, 847, 851, 855, 858, 862, 866, 869, 873, 877, 881, 884, 888, 892, 896,
	899, 903, 907, 910, 914, 918, 921, 925, 929, 933, 936, 940, 944, 948, 952, 956,
	960, 964, 968, 972, 976, 980, 984, 988, 992, 996, 1000, 1004, 1008, 1012, 1016, 1020,1024
};

const CODE UINT16  GAMMA_900_TABLE[257]=
{
	0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60,
	64, 71, 77, 82, 88, 93, 98, 104, 109, 114, 118, 123, 128, 133, 138, 142,
	147, 152, 156, 161, 166, 170, 175, 179, 184, 188, 193, 197, 202, 206, 210, 215,
	219, 224, 228, 232, 236, 241, 245, 249, 254, 258, 262, 266, 271, 275, 279, 283,
	287, 292, 296, 300, 304, 308, 312, 317, 321, 325, 329, 333, 337, 341, 345, 349,
	354, 358, 362, 366, 370, 374, 378, 382, 386, 390, 394, 398, 402, 406, 410, 414,
	418, 422, 426, 430, 434, 438, 442, 446, 450, 454, 457, 461, 465, 469, 473, 477,
	481, 485, 489, 493, 497, 501, 504, 508, 512, 516, 520, 524, 528, 532, 535, 539,
	543, 547, 551, 555, 558, 562, 566, 570, 574, 578, 581, 585, 589, 593, 597, 600,
	604, 608, 612, 616, 619, 623, 627, 631, 635, 638, 642, 646, 650, 653, 657, 661,
	665, 668, 672, 676, 680, 683, 687, 691, 695, 698, 702, 706, 710, 713, 717, 721,
	724, 728, 732, 736, 739, 743, 747, 750, 754, 758, 761, 765, 769, 773, 776, 780,
	784, 787, 791, 795, 798, 802, 806, 809, 813, 817, 820, 824, 828, 831, 835, 839,
	842, 846, 849, 853, 857, 860, 864, 868, 871, 875, 879, 882, 886, 889, 893, 897,
	900, 904, 908, 911, 915, 918, 922, 926, 929, 933, 936, 940, 944, 948, 952, 956,
	960, 964, 968, 972, 976, 980, 984, 988, 992, 996, 1000, 1004, 1008, 1012, 1016, 1020,1024
};

const CODE UINT16  GAMMA_875_TABLE[257]=
{
	0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60,
	64, 72, 78, 85, 90, 96, 102, 107, 112, 118, 123, 128, 133, 138, 143, 148,
	153, 158, 162, 167, 172, 177, 181, 186, 191, 195, 200, 204, 209, 213, 218, 222,
	227, 231, 236, 240, 244, 249, 253, 258, 262, 266, 271, 275, 279, 283, 288, 292,
	296, 300, 305, 309, 313, 317, 321, 325, 330, 334, 338, 342, 346, 350, 354, 358,
	363, 367, 371, 375, 379, 383, 387, 391, 395, 399, 403, 407, 411, 415, 419, 423,
	427, 431, 435, 439, 443, 447, 451, 455, 458, 462, 466, 470, 474, 478, 482, 486,
	490, 494, 497, 501, 505, 509, 513, 517, 521, 524, 528, 532, 536, 540, 544, 547,
	551, 555, 559, 563, 566, 570, 574, 578, 581, 585, 589, 593, 597, 600, 604, 608,
	612, 615, 619, 623, 626, 630, 634, 638, 641, 645, 649, 653, 656, 660, 664, 667,
	671, 675, 678, 682, 686, 689, 693, 697, 700, 704, 708, 711, 715, 719, 722, 726,
	730, 733, 737, 741, 744, 748, 751, 755, 759, 762, 766, 769, 773, 777, 780, 784,
	788, 791, 795, 798, 802, 805, 809, 813, 816, 820, 823, 827, 830, 834, 838, 841,
	845, 848, 852, 855, 859, 862, 866, 870, 873, 877, 880, 884, 887, 891, 894, 898,
	901, 905, 908, 912, 915, 919, 922, 926, 929, 933, 936, 940, 944, 948, 952, 956,
	960, 964, 968, 972, 976, 980, 984, 988, 992, 996, 1000, 1004, 1008, 1012, 1016, 1020,1024
};

const CODE UINT16  GAMMA_850_TABLE[257]=
{
	0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60,
	64, 73, 80, 87, 93, 99, 105, 111, 117, 122, 128, 133, 138, 143, 149, 154,
	159, 164, 169, 174, 179, 183, 188, 193, 198, 202, 207, 212, 216, 221, 226, 230,
	235, 239, 244, 248, 253, 257, 262, 266, 270, 275, 279, 284, 288, 292, 297, 301,
	305, 309, 314, 318, 322, 326, 330, 335, 339, 343, 347, 351, 355, 360, 364, 368,
	372, 376, 380, 384, 388, 392, 396, 400, 404, 408, 412, 416, 420, 424, 428, 432,
	436, 440, 444, 448, 452, 456, 460, 464, 468, 471, 475, 479, 483, 487, 491, 495,
	499, 502, 506, 510, 514, 518, 522, 525, 529, 533, 537, 541, 544, 548, 552, 556,
	559, 563, 567, 571, 574, 578, 582, 586, 589, 593, 597, 600, 604, 608, 612, 615,
	619, 623, 626, 630, 634, 637, 641, 645, 648, 652, 656, 659, 663, 667, 670, 674,
	677, 681, 685, 688, 692, 695, 699, 703, 706, 710, 713, 717, 721, 724, 728, 731,
	735, 738, 742, 746, 749, 753, 756, 760, 763, 767, 770, 774, 777, 781, 784, 788,
	791, 795, 798, 802, 805, 809, 812, 816, 819, 823, 826, 830, 833, 837, 840, 844,
	847, 851, 854, 858, 861, 865, 868, 872, 875, 878, 882, 885, 889, 892, 896, 899,
	902, 906, 909, 913, 916, 920, 923, 926, 930, 933, 937, 940, 944, 948, 952, 956,
	960, 964, 968, 972, 976, 980, 984, 988, 992, 996, 1000, 1004, 1008, 1012, 1016, 1020,1024
};

const CODE UINT16  GAMMA_825_TABLE[257]=
{
	0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60,
	64, 74, 82, 89, 96, 103, 109, 115, 121, 127, 133, 138, 144, 149, 155, 160,
	165, 170, 175, 181, 186, 191, 196, 200, 205, 210, 215, 220, 225, 229, 234, 239,
	243, 248, 252, 257, 262, 266, 271, 275, 279, 284, 288, 293, 297, 301, 306, 310,
	314, 319, 323, 327, 332, 336, 340, 344, 348, 353, 357, 361, 365, 369, 373, 377,
	381, 386, 390, 394, 398, 402, 406, 410, 414, 418, 422, 426, 430, 434, 438, 442,
	446, 450, 454, 457, 461, 465, 469, 473, 477, 481, 485, 488, 492, 496, 500, 504,
	508, 511, 515, 519, 523, 527, 530, 534, 538, 542, 545, 549, 553, 557, 560, 564,
	568, 571, 575, 579, 583, 586, 590, 594, 597, 601, 605, 608, 612, 616, 619, 623,
	626, 630, 634, 637, 641, 645, 648, 652, 655, 659, 662, 666, 670, 673, 677, 680,
	684, 687, 691, 694, 698, 702, 705, 709, 712, 716, 719, 723, 726, 730, 733, 737,
	740, 744, 747, 751, 754, 758, 761, 764, 768, 771, 775, 778, 782, 785, 789, 792,
	795, 799, 802, 806, 809, 813, 816, 819, 823, 826, 830, 833, 836, 840, 843, 847,
	850, 853, 857, 860, 863, 867, 870, 873, 877, 880, 884, 887, 890, 894, 897, 900,
	904, 907, 910, 914, 917, 920, 923, 927, 930, 933, 937, 940, 944, 948, 952, 956,
	960, 964, 968, 972, 976, 980, 984, 988, 992, 996, 1000, 1004, 1008, 1012, 1016, 1020,1024
};

UINT32 bPreDarkPixlCnt = 0;
UINT8 bPreAPL;
UINT8 bCurGainL;
UINT8 bCurGainM;
UINT8 bCurGainH;
UINT8 bValueL;
UINT8 bValueH;
UINT16 waLumaArray[LUMA_CURVE_NUM];

extern UINT8 bSceneChange;
extern UINT8 bAPL;
extern UINT8 fgCurveComputeOn;
extern UINT8 bAdapLumaGain;
extern UINT8 bAdapLumaOffset;
extern UINT8 bAdapLumaLimit;
extern UINT8 bMaxDiffThreshold;
extern UINT8 bTotalDiffThreshold;
extern UINT8 bALOnOff;
extern UINT8 bBWSOnOff;
extern UINT8 bBSOnOff;
extern UINT8 bBSLevel;
extern UINT8 bBSGain;
extern UINT8 bBSOffset;
extern UINT8 bBSRatio;
extern UINT8 bBSLimit;
extern UINT8 bWSOnOff;
extern UINT8 bWSLevel;
extern UINT8 bWSGain;
extern UINT8 bWSOffset;
extern UINT8 bWSRatio;
extern UINT8 bWSLimit;
extern UINT8 bForceYGamma;
extern UINT8 bDynamicBSRange;
extern UINT8 bDynamicBSOnOff;
extern UINT8 bDynamicBSDbg;
extern UINT8 bDBSPxlThd1;
extern UINT8 bDBSPxlThd2;
extern UINT8 bDBSDistThdH;
extern UINT8 bDBSDistThdL;
extern UINT8 bDBSLmtBtm;
extern UINT8 bDBSLmtTop;
extern UINT8 bDBSGainBtm;
extern UINT8 bDBSGainTop;
extern UINT8 bDBSOfstBtm;
extern UINT8 bDBSOfstTop;
extern UINT8 bBWSKeepMidOnOff;
extern UINT8 bBWSKeepMidX;
extern UINT16 wBWSKeepMidY;

extern UINT8 bYGammaDecayL;
extern UINT8 bYGammaDecayH;
extern UINT8 bDarkSceneBrit;

extern UINT8 u1BSColorToneOnOff;

extern UINT8 u1RRatioOffset; 
extern UINT8 u1GRatioOffset;
extern UINT8 u1BRatioOffset;  


UINT8 bDBSGain;
UINT8 bDBSOffset;
UINT8 bDBSLimit;
UINT32 dwDistribution;
UINT32 dwDarkPxlCount;
UINT32 dwHistSum;

void vDrvContrastUpdate(void)
{
	vDrvSetLumaCurve(waLumaArray);
}

void vDrvGetAdaptiveInfo(UINT8 bindex)
{
	switch (bindex)
	{
	case 0:
		Printf
		("\nbBSOnOff %d  bBSLevel %d   bBSGain %d   bBSOffset %d   bBSRatio %d   bBSLimit %d\n",
		 bBSOnOff, bBSLevel, bBSGain, bBSOffset, bBSRatio, bBSLimit);
		Printf
		("bWSOnOff %d  bWSLevel %d   bWSGain %d   bWSOffset %d   bWSRatio %d   bWSLimit %d\n",
		 bWSOnOff, bWSLevel, bWSGain, bWSOffset, bWSRatio, bWSLimit);
		Printf
		("bALOnOff %d   bAdapLumaGain %d   bAdapLumaOffset %d   bAdapLumaLimit %d  bYGamma %d\n",
		 bALOnOff, bAdapLumaGain, bAdapLumaOffset, bAdapLumaLimit, bForceYGamma);
		break;
	case 1:
		Printf
		("bDynamicBSRange = %d  bDynamicBSOnOff = %d  bDynamicBSDbg = %d\n",
		 bDynamicBSRange, bDynamicBSOnOff, bDynamicBSDbg);
		Printf
		("bDBSPxlThd1 = %d  bDBSPxlThd2 = %d\n",
		 bDBSPxlThd1, bDBSPxlThd2);
		Printf
		("bDBSDistThdH = %d  bDBSDistThdL = %d\n",
		 bDBSDistThdH, bDBSDistThdL);
		break;
	case 2:
		Printf
		("bBWSKeepMidOnOff = %d  bBWSKeepMidX = %d  wBWSKeepMidY = %d\n",
		 bBWSKeepMidOnOff, bBWSKeepMidX, wBWSKeepMidY);
		break;
	default:
		break;
	}
	Printf("\n");
}


static void vBuildDefault(UINT16 * Curve)
{
	UINT8 bi;

	if (Curve == NULL)
	{
		return;
	}

	for (bi = 0; bi < LUMA_HIST_NUM; bi++)
	{
		Curve[bi] = (UINT16) bi << 5;
	}

	Curve[LUMA_HIST_NUM] = 1023;
}

static UINT8 bGetNormalizedHist(UINT16 * waHistCur)
{
	UINT8 bi;

	if (waHistCur == NULL)
	{
		return SV_FALSE;
	}

	for (bi = 0, dwHistSum =0; bi < LUMA_HIST_NUM; bi++)
	{
		waHistCur[bi] = wReadHist32Cur(bi);
		dwHistSum += waHistCur[bi];
	}

	if (dwHistSum != 0)
	{
		for (bi = 0; bi < LUMA_HIST_NUM; bi++)
		{
			waHistCur[bi] =
			    (UINT16) ((((UINT32) waHistCur[bi] << 9) +
			               (dwHistSum >> 1)) / dwHistSum);

			AL_DBG("Hist %d %d\n", bi, waHistCur[bi]);
		}

		return SV_TRUE;
	}
	else
	{
		return SV_FALSE;
	}
}

static void vDetectSceneChanged(const UINT16 * waHistCur)
{
	UINT8 bi;
	UINT16 wMaxDiff = 0, wTotalDiff = 0, wDiff = 0;

	if (waHistCur == NULL)
	{
		return;
	}

	for (bi = 0; bi < LUMA_HIST_NUM; bi++)
	{
		wDiff = AbsDiff(waHistCur[bi], (wReadHist32Rec(bi)));
		wMaxDiff = (wMaxDiff < wDiff) ? wDiff : wMaxDiff;

		wTotalDiff += wDiff;
		vWriteHist32Rec(bi, waHistCur[bi]);
	}

       bSceneChange = SV_FALSE; // This flag maybe used by other agent.
	//if (bSceneChange == SV_TRUE)
	//{
	//	return;
	//}

	if ((wMaxDiff > bMaxDiffThreshold)
	        && ((wTotalDiff >> 2) > bTotalDiffThreshold) && (AbsDiff(bAPL, bPreAPL) > 10))
	{
		AL_INFO("Scene Changed bMaxDiff = %d, wTotalDiff = %d\n", wMaxDiff, wTotalDiff);
		bSceneChange = SV_TRUE;
	}
	else
	{
		bSceneChange = SV_FALSE;
	}

	bPreAPL = bAPL;
}

static void vDrvGetDistribution(const UINT16 * waHistCur)
{
	UINT8 bi;

	for (bi = 0, dwDistribution = 0; bi < LUMA_HIST_NUM; bi++)
	{
		dwDistribution += AbsDiff(bi * 8, bAPL) * waHistCur[bi];
	}

	// shift 8 bits to make distribution a 8 bits value
	dwDistribution = dwDistribution >> 8;
	vIO32WriteFldAlign(ADAP_LUMA_06, dwDistribution, SWREG_AL_DIST);
}

static void vDrvGetDrkArea(const UINT16 * waHistCur)
{
	UINT8 bi;

	if (bDynamicBSRange < 32)
	{
		for (bi = 0, dwDarkPxlCount = 0; bi <= bDynamicBSRange; bi++)
		{
			dwDarkPxlCount += waHistCur[bi];
		}
	}

	// quality table use 8 bit resolution, but histogram was normalized to 512,
	// so we need to shift 1 bit.
	dwDarkPxlCount >>= 1;

	if (!bSceneChange)
	{
		dwDarkPxlCount = (bPreDarkPixlCnt*7 + dwDarkPxlCount) >> 3;
	}

	bPreDarkPixlCnt = dwDarkPxlCount;
	vIO32WriteFldAlign(ADAP_LUMA_06, dwDarkPxlCount, SWREG_AL_DRK_PXL);
}

// DBS_INTP_METHOD 0 : use linear interpolation
// DBS_INTP_METHOD 1 : do not use interpolation
#define DBS_INTP_METHOD 0
UINT32 dwDBSCnt = 0;
static void vDynamicBlackStretch(const UINT16 * waHistCur)
{
	bDBSOffset = bBSOffset ;
	bDBSGain = bBSGain;

	
	AL_INFO("bAPL = %d, bDarkPxl = %d, Dist = %d\n", bAPL, dwDarkPxlCount, dwDistribution);

	if (bDynamicBSOnOff)
	{
		UINT8 bDBSLimitStep, bDBSGainStep, bDBSOffsetStep;

		// 1. Dynamic BSLimit
		if (dwDarkPxlCount > bDBSPxlThd2)
		{
			bDBSLimit = bBSLimit + (bDBSLmtTop - 0x80);
		}
		else if (dwDarkPxlCount > bDBSPxlThd1 )
		{
			// interpolate between top / bottom
			bDBSLimitStep = (((bDBSLmtTop - bDBSLmtBtm)*(dwDarkPxlCount - bDBSPxlThd1))/(bDBSPxlThd2 - bDBSPxlThd1)) + bDBSLmtBtm;

			if (bDBSLimitStep > 0x80)
			{
				bDBSLimit = bBSLimit + (bDBSLimitStep - 0x80);
			}
			else
			{
				bDBSLimit = (bBSLimit > (0x80 - bDBSLimitStep) ) ? bBSLimit - (0x80 - bDBSLimitStep) : 0;
			}
		}
		else
		{
			if (bDBSLmtBtm > 0x80)
			{
				bDBSLimit = bBSLimit + (bDBSLmtBtm - 0x80);
			}
			else
			{
				bDBSLimit = (bBSLimit > (0x80 - bDBSLmtBtm) ) ? bBSLimit - (0x80 - bDBSLmtBtm) : 0;
			}
		}

		// 2. Dynamic BSGain / BSOffset
		if (dwDistribution > bDBSDistThdH)
		{
			if (bDBSOfstBtm > 0x80)
			{
				bDBSOffset = bBSOffset + (bDBSOfstBtm - 0x80);
			}
			else
			{
				bDBSOffset = (bBSOffset > (0x80 - bDBSOfstBtm) ) ? bBSOffset - (0x80 - bDBSOfstBtm) : 0;
			}

			bDBSGain = bBSGain + (bDBSGainTop - 0x80);
		}
		else if (dwDistribution < bDBSDistThdL)
		{
			bDBSOffset = bBSOffset + (bDBSOfstTop - 0x80);

			if (bDBSGainBtm > 0x80)
			{
				bDBSGain = bBSGain + (bDBSGainBtm - 0x80);
			}
			else
			{
				bDBSGain = (bBSGain > (0x80 - bDBSGainBtm) ) ? bBSGain - (0x80 - bDBSGainBtm) : 0;
			}
		}
		else
		{
			bDBSOffsetStep = (((bDBSOfstTop - bDBSOfstBtm)*(dwDistribution - bDBSDistThdL))/(bDBSDistThdH - bDBSDistThdL)) + bDBSOfstBtm;
			bDBSGainStep = (((bDBSGainTop - bDBSGainBtm)*(dwDistribution- bDBSDistThdL))/(bDBSDistThdH - bDBSDistThdL)) + bDBSGainBtm;

			// interpolate dynamic bs offset
			if (bDBSOffsetStep > 0x80)
			{
				bDBSOffset= bBSOffset+ (bDBSOffsetStep - 0x80);
			}
			else
			{
				bDBSOffset= (bBSOffset > (0x80 - bDBSOffsetStep) ) ? bBSOffset - (0x80 - bDBSOffsetStep) : 0;
			}

			// interpolate dynamic bs gain
			if (bDBSGainStep > 0x80)
			{
				bDBSGain = bBSGain + (bDBSGainStep - 0x80);
			}
			else
			{
				bDBSGain = (bBSGain > (0x80 - bDBSGainStep) ) ? bBSGain - (0x80 - bDBSGainStep) : 0;
			}
		}

		if (bDynamicBSDbg)
		{
			if (dwDBSCnt++ % 30 == 0)
			{
				Printf("bDBSGain = %d, bDBSOfst = %d, bDBSLmt = %d\n", bDBSGain, bDBSOffset, bDBSLimit);
			}
		}
	}
}

static void vGetApdaptiveGain(const UINT16 * waHistCur)
{
	UINT8 bi;
	UINT8 bCountL = 0, bCountM = 0, bCountH = 0;
	UINT8 bDiffCur;
	UINT8 bDynGain = ((UINT16) bAdapLumaGain * (255 - bAPL)) >> 8;
	UINT8 bGainL = ((16 + bAdapLumaOffset) * (UINT16) bDynGain) >> 7;
	UINT8 bGainM = ((8 + bAdapLumaOffset) * (UINT16) bDynGain) >> 7;
	UINT8 bGainH = bGainL;

	bGainL = (bGainL > 16) ? 16 : bGainL;

	if (waHistCur == NULL)
	{
		return;
	}

	for (bValueL = AL_LOW_START; bValueL < AL_LOW_END; bValueL += 4)
	{
		bCountL = 0;

		for (bi = 0; bi < LUMA_HIST_NUM; bi++)
		{
			if (waHistCur[bi] < bValueL)
			{
				bCountL++;
			}
		}

		if (bCountL >= AL_LOW_BIN)
		{
			break;
		}
	}

	for (bValueH = AL_HIGH_START; bValueH < AL_HIGH_END; bValueH += 4)
	{
		bCountH = 0;

		for (bi = 0; bi < LUMA_HIST_NUM; bi++)
		{
			if (waHistCur[bi] >= bValueH)
			{
				bCountH++;
			}
		}

		if (bCountH <= AL_HIGH_BIN)
		{
			break;
		}
	}

	bCountM = LUMA_HIST_NUM - (bCountH + bCountL);
	bCountL -= 4;

	AL_DBG("\nbDynGain %d  bValueL %d  bValueH %d \n", bDynGain, bValueL, bValueH);
	AL_DBG("bCountL %d  bCountM %d  bCountH %d \n", bCountL, bCountM, bCountH);

	bDiffCur = AL_LOW_END - bValueL;

	bCurGainL = bGainL;
	bCurGainM = ((UINT16) bGainM * bDiffCur) / (AL_LOW_END - AL_LOW_START);
	bCurGainH = ((UINT16) bGainH * bDiffCur) / (AL_LOW_END - AL_LOW_START);

	AL_DBG("CurGainL %d  CurGainM %d  bCurGainH %d \n", bCurGainL, bCurGainM, bCurGainH);

	if ((bCountL != 0) && (bCountH != 0))
	{
		bCurGainL =
		    (((UINT16) bCurGainH * bCountH) + ((UINT16) bCurGainM * bCountM) +
		     (bCountL >> 1)) / bCountL;

		if (bCurGainL > bGainL)
		{
			bCurGainM =
			    (((UINT16) bGainL * bCurGainM) + (bCurGainL >> 1)) / bCurGainL;
			bCurGainH =
			    (((UINT16) bGainL * bCountL) -
			     (((UINT16) bCurGainM * bCountM) + (bCountH >> 1))) / bCountH;
			bCurGainL = bGainL;
		}
	}
	else
	{
		bCurGainL = 0;
		bCurGainM = 0;
		bCurGainH = 0;
	}

	AL_DBG("CurGainL %d  CurGainM %d  bCurGainH %d \n", bCurGainL, bCurGainM, bCurGainH);
}

static void vBuildDynCurve(const UINT16 * waHistCur, UINT16 * waDynCurve)
{
	UINT8 bi, bGainH, bGainM, bGainL;

	if ((waDynCurve == NULL) || (waHistCur == NULL))
	{
		return;
	}

	vBuildDefault(waDynCurve);

    if(bALOnOff == SV_TRUE)
    {
	    bGainL = 32 - bCurGainL;
	    bGainM = 32 + bCurGainM;
	    bGainH = 32 + bCurGainH;
	    
	    for (bi = 3; bi < (LUMA_HIST_NUM - 2); bi++)
	    {
		    if (waHistCur[bi - 1] < bValueL)
		    {
			    waDynCurve[bi] = waDynCurve[bi - 1] + bGainL;
		    }
		    else if (waHistCur[bi - 1] >= bValueH)
		    {
		    	waDynCurve[bi] = waDynCurve[bi - 1] + bGainH;
		    }
		    else
		    {
		    	waDynCurve[bi] = waDynCurve[bi - 1] + bGainM;
		    }

		    waDynCurve[bi] = (waDynCurve[bi] > 0x3C0) ? 0x3C0 : waDynCurve[bi];
	    }
    }
}

#define BSGAIN_START_POS    0x10
#define BSGAIN_MID_POS    	0x40
#define BSGAIN_END_POS      0x80

static UINT8 bGetBSGain(void)
{
	if (bAPL < BSGAIN_START_POS)
	{
		return 0;
	}
	else if (bAPL < BSGAIN_MID_POS)
	{
		return (UINT8) (((UINT16) (bBSRatio) * (bAPL - BSGAIN_START_POS))
		                / (BSGAIN_MID_POS - BSGAIN_START_POS));
	}
	else if (bAPL < bAdapLumaLimit)
	{
		return (UINT8) ((((UINT16) (0xFF - bBSRatio) * (bAPL - BSGAIN_MID_POS))
		                 / (bAdapLumaLimit - BSGAIN_MID_POS)) + bBSRatio);
	}
	else
	{
		return 0xFF;
	}
}

static UINT8 bGetWSGain(void)
{
	UINT8 bRet;

	// treat 235 as 255 (top)
	if (bAPL < 192)
	{
		bRet = (UINT8) (((UINT16) (255 - bWSRatio) * bAPL) / 192);
	}
	else
	{
		bRet =
		    (UINT8) ((((UINT16) bWSRatio * (bAPL - 192)) >> 6) +
		             (255 - bWSRatio));
	}

	return bRet;
}

UINT16 u2CURRENT_Y_GAMMA_TABLE[257];

static void vDrvApplyYGamma(UINT16* waFinalCurve)
{
#if 1
	UINT32 i;	
	const UINT16* wTbl;
	UINT8 bDecay;
	UINT8 bDarkSceneBrightRaise;//Raising the Brightness for Dark Scene
	UINT16 u2RemapIndex;

	switch (bForceYGamma)
	{
	case 1:
		wTbl = GAMMA_950_TABLE;
		break;
	case 2:
		wTbl = GAMMA_925_TABLE;
		break;
	case 3:
		wTbl = GAMMA_900_TABLE;
		break;
	case 4:
		wTbl = GAMMA_875_TABLE;
		break;
	case 5:
		wTbl = GAMMA_850_TABLE;
		break;
	case 6:
		wTbl = GAMMA_825_TABLE;
		break;
    case 7:
        wTbl = GAMMA_105_TABLE;
        break;
	case 8:
		wTbl = GAMMA_110_TABLE;
		break;
	case 9:
		wTbl = GAMMA_115_TABLE;
		break;				
	default:
		return;
	}

    //Boundary Protection
    bYGammaDecayH = MIN(bYGammaDecayH,0x80);
    bYGammaDecayL = MIN(bYGammaDecayL,0x80);

    if(bAPL<0x40)
    {        
        bDecay = bAPL * (0x80 - bYGammaDecayL) /0x40 + bYGammaDecayL;
        bDarkSceneBrightRaise = (0x40-bAPL) * bDarkSceneBrit / 0x40;        
    }
    else if(bAPL>0xC0)
    {    
        bDecay = (0x100-bAPL) * (0x80 - bYGammaDecayH) /(0x100-0xC0) + bYGammaDecayH;
        bDarkSceneBrightRaise =0;
    }
    else
	{
        bDecay = 0x80;
        bDarkSceneBrightRaise = 0;
    }

    for(i=0;i<=256;i++)
    {
        u2CURRENT_Y_GAMMA_TABLE[i]=((INT32)(wTbl[i])- i*4) * bDecay / 0x80+ i*4;
	}

	for (i = 3; i < 30; i++)
	{
	    u2RemapIndex = waFinalCurve[i]>>2; 

	    if(u2RemapIndex <= 256) // fix klockwork
	    {
		    waFinalCurve[i] = MIN(u2CURRENT_Y_GAMMA_TABLE[u2RemapIndex]+ bDarkSceneBrightRaise, 1024);
	    }
	    else
	    {
            waFinalCurve[i] = i << 5;
	    }
	}
#endif	
}

UINT32 u4DebugCounter1 = 0;
UINT8 u1RemapBSRatio()
{
    UINT16  u2aHueHist[8], u2aSatHist[8];
    UINT8 u1SatHistBoundary[7] = {0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80};
    UINT16 u2RemappedRatio;
    UINT32 u4ColorPercentage;
    UINT8 u1RPercentage, u1GPercentage, u1BPercentage;
    UINT8 u1ASL;
    UINT8 u1RChannelGain, u1GChannelGain, u1BChannelGain;
    UINT8 u1WhiteDecay;
    
    // 1. get normalized hue/saturation histogram
    vDrvGetHueHist(u2aHueHist, SV_TRUE, 0xFF);
    vDrvGetChromaHist(u2aSatHist, SV_TRUE, 0xFF);    

    // 2. get color ratio offset (read from sw register)
//    u1RRatioOffset = IO32ReadFldAlign(ADAP_LUMA_08, BS_COLOR_KR);
//    u1GRatioOffset = IO32ReadFldAlign(ADAP_LUMA_08, BS_COLOR_KG);
//    u1BRatioOffset = IO32ReadFldAlign(ADAP_LUMA_08, BS_COLOR_KB);    

    // 3. calculate color distribution (R/G/B)
    u4ColorPercentage = u4RetRGBOfHuePercentage(u2aHueHist);
    u1RPercentage = (UINT8)((u4ColorPercentage >> 16) & 0xFF);
    u1GPercentage = (UINT8)((u4ColorPercentage >> 8)  & 0xFF);
    u1BPercentage = (UINT8)( u4ColorPercentage        & 0xFF);    

    // 4. get average saturation level (ASL)
    u1ASL = u1RetASL(u1SatHistBoundary, u2aSatHist, 0xFF);
    
    // 5. get channel gain from remapped saturation
    u1RChannelGain = u1GetChannelGainFromASL(u1ASL, SV_RGB_R);
    u1GChannelGain = u1GetChannelGainFromASL(u1ASL, SV_RGB_G);
    u1BChannelGain = u1GetChannelGainFromASL(u1ASL, SV_RGB_B);    

    u1WhiteDecay = u1GetWhiteDecayFromASL(u2aSatHist);
    //TODO(4) : add remapped gain here.
    if(IO32ReadFldAlign(ADAP_LUMA_0B, BS_COLOR_DBG_04))
    {
        if(u4DebugCounter1 % 30 == 0)    
        {
            Printf("[4] ChannelGainR = %d ChannelGainG = %d ChannelGainB = %d White Decay = %d \n", (int)u1RChannelGain, (int)u1GChannelGain, (int)u1BChannelGain,(int)u1WhiteDecay);
        }
    }    

    // 6. remapping BSRatio
    //    u2RemappedRatio (Offset):  0x80 as 0, 0x0 as -128, 0xFF as +127 
    //    TODO: refine process here
    u2RemappedRatio = (((UINT32)((UINT32)u1RRatioOffset * u1RChannelGain * u1RPercentage)) + 
                       ((UINT32)((UINT32)u1GRatioOffset * u1GChannelGain * u1GPercentage)) + 
                       ((UINT32)((UINT32)u1BRatioOffset * u1BChannelGain * u1BPercentage)))/(0x80 * 0xFF);


    u2RemappedRatio = ((INT32)u2RemappedRatio-0x80)*u1WhiteDecay/0x80+0x80;

    //TODO(5) : add final remapped ratio here
    if(IO32ReadFldAlign(ADAP_LUMA_0B, BS_COLOR_DBG_05))
        {
        if(u4DebugCounter1 % 30 == 0)    
        {
            Printf("[5-1] u2RemappedRatio = %d\n", u2RemappedRatio);
        }
    }

    u2RemappedRatio = MIN(u2RemappedRatio* bBSRatio / 0x80 , 0xFF);

    if(IO32ReadFldAlign(ADAP_LUMA_0B, BS_COLOR_DBG_05))
    {
        if(u4DebugCounter1 % 30 == 0)    
        {
            Printf("[5-2] u2RemappedRatio = %d\n", u2RemappedRatio);
        }
    }    

    u4DebugCounter1++;

    return u2RemappedRatio;
    //TODO : fake return    
//    return u2RemappedRatio;
}

static void vBuildTargetCurve(UINT16 * waTargetCurve)
{
	UINT8 bi;

	if (waTargetCurve == NULL)
	{
		return;
	}

	vBuildDefault(waTargetCurve);

	if (bBWSOnOff == SV_TRUE)
	{
		UINT16 wDiff;
		UINT8 bStep;
		UINT8 bBsGainMap, bWsGainMap, bWSDetailEnhace;
		UINT8 bWSAdjustGain, bBSAdjustGain;
		UINT8 bWSAdjustOffset, bBSAdjustOffset;

		UINT8 bBSGainApplied = bBSGain;
		UINT8 bBSOffsetApplied = bBSOffset;
		UINT8 bBSLimitApplied = bBSLimit;

		if (bDynamicBSOnOff)
		{
		bBSGainApplied = bDBSGain;
		bBSOffsetApplied = bDBSOffset;
		bBSLimitApplied = bDBSLimit;
		}

#if COLOR_DEPEND_BS_RATIO
        if(u1BSColorToneOnOff)
        {
        bBSRatio = u1RemapBSRatio();
        }
#endif //COLOR_DEPEND_BS_RATIO

		bBsGainMap = bGetBSGain();
		bWsGainMap = bGetWSGain();

		// BSGain
		wDiff = AbsDiff(bBSGainApplied, 0x80 + bBSLimitApplied);
		bBSAdjustGain =
		    (UINT8) (bBSGainApplied + ((wDiff * (0xFF - bBsGainMap)) >> 8));

		// WS Gain
		wDiff = AbsDiff(bWSGain, 0x80 + bWSLimit);
		bWSAdjustGain = (UINT8) (bWSGain + ((wDiff * bWsGainMap) >> 8));

		// BS Offset
		//wDiff = bBSOffsetApplied + bBSLimit;
		wDiff = bBSOffsetApplied;
		bBSAdjustOffset = (UINT8) ((wDiff * bBsGainMap) >> 8);

		// WS Offset
		wDiff = bWSOffset + bWSLimit;
		bWSAdjustOffset = (UINT8) ((wDiff * (0xFF - bWsGainMap)) >> 8);

		if (bWSAdjustOffset > bWSLimit)
		{
			bWSAdjustOffset = bWSAdjustOffset - bWSLimit;
			bWSDetailEnhace = SV_FALSE;
		}
		else
		{
			bWSAdjustOffset = bWSLimit - bWSAdjustOffset;
			bWSDetailEnhace = SV_TRUE;
		}

		AL_DBG("bAPL %d  bBsGainMap %d  bWsGainMap %d\n", bAPL,	bBsGainMap, bWsGainMap);
		AL_DBG("bBSAdjustGain %d  bBSAdjustOffset %d  \n", bBSAdjustGain, bBSAdjustOffset);
		AL_DBG("bWSAdjustGain %d  bWSAdjustOffset %d  \n", bWSAdjustGain, bWSAdjustOffset);

		if (bBSOnOff == SV_TRUE)
		{
			for (bi = 0; bi < bBSLevel; bi++)
			{
				bStep = (bBSAdjustGain >> 2);
				if (bStep > bBSAdjustOffset)
				{
					bStep -= bBSAdjustOffset;
					bBSAdjustOffset = 0;
				}
				else
				{
					bBSAdjustOffset -= bStep;
					bStep = 0;
				}

				waTargetCurve[bi + 3] = waTargetCurve[bi + 2] + bStep;
			}
		}

		if (bWSOnOff == SV_TRUE)
		{
			for (bi = 0; bi < bWSLevel; bi++)
			{
				if (bWSDetailEnhace == SV_FALSE)
				{
					bStep = (bWSAdjustGain >> 2);
					if (bStep > bWSAdjustOffset)
					{
						bStep -= bWSAdjustOffset;
						bWSAdjustOffset = 0;
					}
					else
					{
						bWSAdjustOffset -= bStep;
						bStep = 0;
					}
				}
				else
				{
					bWSDetailEnhace = SV_FALSE;
					bStep = (bWSAdjustGain >> 2) + bWSAdjustOffset;
					bWSAdjustOffset = 0;
				}

				assert(bi <= 29);
				waTargetCurve[29 - bi] = waTargetCurve[30 - bi] - bStep;
			}
		}

		UINT16 wBDiff, wWDiff, wTmp;

		// Keep BWS Mid Point
		if (bBWSKeepMidOnOff)
		{
			wBDiff =
			    (wBWSKeepMidY - waTargetCurve[2 + bBSLevel]) / (bBWSKeepMidX - bBSLevel);

			for (bi = (bBSLevel + 3); bi < (bBWSKeepMidX + 3) ; bi++)
			{
				waTargetCurve[bi] = waTargetCurve[bi - 1] + wBDiff;
			}

			wWDiff =
			    (waTargetCurve[30 - bWSLevel] - wBWSKeepMidY) / (28 - (bWSLevel + bBWSKeepMidX));

			for (bi = bBWSKeepMidX + 3; bi < (30 - bWSLevel); bi++)
			{
				waTargetCurve[bi] = waTargetCurve[bi - 1] + wWDiff;
			}
                    wTmp = (waTargetCurve[bBWSKeepMidX + 1] + waTargetCurve[bBWSKeepMidX + 3] ) >> 1;
                    waTargetCurve[bBWSKeepMidX + 2] = (waTargetCurve[bBWSKeepMidX + 2] + wTmp) >> 1;
		}
		else
		{
			wDiff =
			    ((waTargetCurve[30 - bWSLevel] - waTargetCurve[2 + bBSLevel]) + ((28 - (bWSLevel + bBSLevel)) / 2))
			    / (28 - (bWSLevel + bBSLevel));

			for (bi = (bBSLevel + 3); bi < (30 - bWSLevel); bi++)
			{
				waTargetCurve[bi] = waTargetCurve[bi - 1] + wDiff;
			}
		}

	}
}

UINT32 hue_counter = 0;

static void vBuildLumaCurve(const UINT16 * waDynCurve,
                            const UINT16 * waTargetCurve)
{
	UINT8 bi, bIndex;
	UINT16 waFinalCurve[LUMA_HIST_NUM + 1];
#ifdef AL_SKIN_TONE_PROTECTION	
	UINT16 waHueHist[8];
	UINT16 wSkinToneRatio = 0;
	UINT8  bSkinToneAttenuate = 0;
#endif //AL_SKIN_TONE_PROTECTION

	if ((waDynCurve == NULL) || (waTargetCurve == NULL))
	{
		return;
	}


	for (bi = 3; bi < 30; bi++)
	{
		bIndex = waTargetCurve[bi] >> 5;

		waFinalCurve[bi] =
		    ((waDynCurve[bIndex + 1] * (waTargetCurve[bi] - (bIndex << 5))) +
		     (waDynCurve[bIndex] *
		      (((bIndex + 1) << 5) - waTargetCurve[bi])) + 16) >> 5;
	}

	vDrvApplyYGamma(waFinalCurve);

	if (bSceneChange == SV_TRUE)
	{
		for (bi = 3; bi < 30; bi++)
		{
			waLumaArray[bi] = waFinalCurve[bi];
		}
	}
	else
	{
		for (bi = 3; bi < 30; bi++)
		{
			waLumaArray[bi] =
			    ((waLumaArray[bi] * 7) + waFinalCurve[bi]) >> 3;
		}
	}

	// get hue histogram

#ifdef AL_SKIN_TONE_PROTECTION
	if (RegReadFldAlign(DUMMY_02, AL_HUE_PROTECT_EN))
	{
		vDrvGetHueHistogram(waHueHist);

		// todo : add attenuate to skin tone ratio
		wSkinToneRatio = waHueHist[2] + waHueHist[3];

		bSkinToneAttenuate = RegReadFldAlign(DUMMY_02, AL_HUE_PROTECT_ATTEN);
		wSkinToneRatio >>= bSkinToneAttenuate;

		for (bi = 3; bi < 30; bi++)
		{
			waLumaArray[bi] =
			    ((waLumaArray[bi] * (256 - wSkinToneRatio)) + ((bi << 5) * wSkinToneRatio)) >> 8;
		}

#if 0
		if (hue_counter++ % 30 == 0)
			Printf("[0] %d [1] %d [2] %d [3] %d [4] %d [5] %d [6] %d [7] %d ", waHueHist[0], waHueHist[1], waHueHist[2], waHueHist[3]
			       , waHueHist[4], waHueHist[5], waHueHist[6], waHueHist[7]);
#endif
	}
#endif //AL_SKIN_TONE_PROTECTION

	AL_DBG("\nwaDynCurve waTargetCurve baLumaArray\n");

	for (bi = 0; bi < LUMA_CURVE_NUM; bi++)
	{
		AL_DBG("%d\t\t%d\t\t%d\n", waDynCurve[bi], waTargetCurve[bi],
		       waLumaArray[bi]);
	}
}

///////////////////////////////////////////////////////////////////////////////////

void vDrvAutoContrastInit_int(void)
{
	vBuildDefault(waLumaArray);

	bSceneChange = SV_TRUE;
	bForceYGamma = 0;
}

void vDrvContrastProc_int(void)
{
	UINT16 waHistCur[LUMA_HIST_NUM];
	UINT16 waTargetCurve[LUMA_CURVE_NUM];
	UINT16 waDynCurve[LUMA_CURVE_NUM];

	vIO32WriteFldAlign(ADAP_LUMA_03, bAPL, SWREG_AL_APL);

	if (bGetNormalizedHist(waHistCur) == SV_TRUE)
	{
		vDetectSceneChanged(waHistCur);

        vDrvGetDistribution(waHistCur);
        vDrvGetDrkArea(waHistCur);

		if (bDynamicBSOnOff)
		{
			vDynamicBlackStretch(waHistCur);
		}

		vGetApdaptiveGain(waHistCur);
		vBuildDynCurve(waHistCur, waDynCurve);
		vBuildTargetCurve(waTargetCurve);
		//vDetectSceneChanged(waHistCur);
		vBuildLumaCurve(waDynCurve, waTargetCurve);
	}

	//bSceneChange = SV_FALSE;
}


/*******************************************************************************************************************
 ****************************************************************** Dynamic Backlight ******************************
 *******************************************************************************************************************/
#define LUMA_TABLE_SIZE         17
#define OPC_HIST_NORMBASE     1000
#define OPC_HIST_MAX_THD       990
#define OPC_MAX_DIFF_THD     28000
#define OPC_TOTAL_DIFF_THD   50000
#define PWM_TABLE_MAX            3

#define AbsDiff(a, b)  (((a)>(b))?((a)-(b)):((b)-(a)))

UINT32 opc_cnt = 0;

typedef struct _opc_info
{
    UINT8 OPC_CURVE_P1;
    UINT8 OPC_CURVE_P2;    
    UINT8 OPC_CURVE_G1;    
    UINT8 OPC_CURVE_G2;    
    UINT8 OPC_CURVE_GSLP;    
    UINT8 OPC_CURVE_PWM;    
} OPC_INFO;

OPC_INFO PreOPCInfo[16];

UINT16 OPC_PWM_INDEX[PWM_TABLE_MAX][LUMA_TABLE_SIZE] = 
    {
     {   0,  10,  20,  30,  40,  50,  60,  70,  80,  90, 100, 110, 120, 130, 140, 150, 160, },   // strong
     {  40,  47,  55,  62,  70,  77,  85,  92, 100, 107, 115, 122, 130, 137, 145, 152, 160, },   // middle
     {  80,  85,  90,  95, 100, 105, 110, 115, 120, 125, 130, 135, 140, 145, 150, 155, 160, }    // low
    };

UINT16 OPCLuminanceTable[PWM_TABLE_MAX][LUMA_TABLE_SIZE][LUMA_TABLE_SIZE] = // content x pwm
{
    {
        { 14, 17, 20, 23, 25, 27, 29, 32, 34, 35, 39, 40, 42, 46, 50, 56, 57, },
        { 25, 28, 33, 36, 40, 44, 47, 52, 55, 58, 63, 66, 69, 73, 81, 90, 93, },
        { 93, 108, 121, 137, 152, 165, 180, 193, 208, 220, 235, 249, 262, 277, 302, 339, 354, },
        { 266, 308, 352, 393, 434, 475, 516, 555, 595, 634, 675, 715, 754, 795, 869, 974, 1019, },
        { 578, 673, 766, 858, 948, 1036, 1125, 1213, 1299, 1390, 1475, 1561, 1650, 1739, 1900, 2128, 2230, },
        { 1005, 1168, 1334, 1492, 1648, 1805, 1957, 2110, 2262, 2414, 2564, 2717, 2869, 3023, 3304, 3700, 3878, },
        { 1574, 1830, 2087, 2335, 2580, 2823, 3064, 3303, 3540, 3777, 4014, 4250, 4489, 4731, 5171, 5790, 6071, },
        { 2244, 2612, 2976, 3332, 3682, 4028, 4371, 4711, 5050, 5388, 5725, 6063, 6404, 6748, 7371, 8259, 8659, },
        { 2948, 3432, 3910, 4377, 4838, 5292, 5742, 6189, 6635, 7079, 7523, 7967, 8414, 8867, 9681, 10850, 11379, },
        { 3756, 4371, 4981, 5577, 6163, 6742, 7315, 7887, 8453, 9018, 9584, 10150, 10720, 11300, 12340, 13859, 14530, },
        { 4761, 5543, 6314, 7070, 7814, 8548, 9276, 9998, 10720, 11429, 12150, 12869, 13619, 14350, 15669, 17559, 18430, },
        { 5893, 6862, 7817, 8750, 9673, 10579, 11479, 12379, 13290, 14180, 15069, 15959, 16859, 17769, 19419, 21750, 22819, },
        { 7084, 8248, 9396, 10520, 11629, 12720, 13830, 14900, 15980, 17050, 18119, 19190, 20269, 21359, 23319, 26139, 27430, },
        { 8420, 9806, 11170, 12500, 13840, 15150, 16440, 17719, 19000, 20269, 21540, 22819, 24100, 25400, 27730, 31069, 32619, },
        { 9831, 11450, 13040, 14619, 16169, 17690, 19200, 20700, 22190, 23669, 25159, 26650, 28150, 29660, 32369, 36280, 38089, },
        { 11490, 13390, 15269, 17100, 18900, 20680, 22440, 24190, 25930, 27669, 29410, 31150, 32900, 34669, 37830, 42389, 44519, },
        { 12400, 14469, 16490, 18459, 20409, 22330, 24230, 26119, 28000, 29869, 31739, 33630, 35519, 37439, 40830, 45750, 48060, }
    }
    ,
    {
        { 25, 27, 28, 29, 32, 34, 34, 36, 39, 40, 41, 44, 46, 48, 53, 56, 57, },
        { 40, 42, 46, 48, 52, 54, 56, 59, 63, 66, 68, 70, 73, 79, 84, 92, 94, },
        { 153, 162, 173, 184, 194, 204, 216, 225, 237, 247, 258, 268, 279, 297, 322, 347, 358, },
        { 441, 469, 501, 529, 562, 591, 623, 650, 683, 711, 744, 771, 805, 858, 927, 999, 1030, },
        { 960, 1025, 1096, 1158, 1228, 1291, 1365, 1425, 1494, 1557, 1628, 1689, 1762, 1880, 2032, 2185, 2255, },
        { 1676, 1787, 1912, 2021, 2144, 2253, 2375, 2482, 2605, 2712, 2835, 2944, 3069, 3277, 3539, 3809, 3932, },
        { 2626, 2801, 2996, 3167, 3360, 3528, 3721, 3889, 4082, 4249, 4442, 4613, 4809, 5134, 5543, 5965, 6159, },
        { 3750, 3997, 4277, 4521, 4796, 5036, 5310, 5550, 5825, 6064, 6342, 6584, 6865, 7326, 7912, 8515, 8793, },
        { 4929, 5256, 5622, 5942, 6306, 6623, 6984, 7298, 7660, 7973, 8339, 8659, 9026, 9634, 10409, 11200, 11559, },
        { 6285, 6698, 7167, 7575, 8037, 8440, 8901, 9304, 9764, 10170, 10629, 11040, 11500, 12279, 13290, 14300, 14769, },
        { 7970, 8495, 9090, 9607, 10190, 10709, 11290, 11800, 12379, 12890, 13509, 14030, 14619, 15609, 16850, 18140, 18740, },
        { 9870, 10520, 11259, 11890, 12620, 13280, 14009, 14640, 15359, 16000, 16730, 17369, 18109, 19330, 20869, 22459, 23209, },
        { 11890, 12670, 13559, 14330, 15209, 15969, 16840, 17609, 18480, 19240, 20119, 20890, 21780, 23250, 25090, 27010, 27910, },
        { 14140, 15069, 16119, 17040, 18090, 19000, 20030, 20940, 21980, 22890, 23930, 24850, 25900, 27650, 29830, 32130, 33200, },
        { 16509, 17600, 18830, 19909, 21119, 22190, 23400, 24459, 25669, 26730, 27950, 29019, 30250, 32289, 34839, 37530, 38780, },
        { 19290, 20569, 22009, 23269, 24690, 25930, 27350, 28589, 30000, 31239, 32660, 33919, 35369, 37739, 40710, 43850, 45310, },
        { 20830, 22200, 23769, 25130, 26660, 28000, 29530, 30869, 32389, 33730, 35269, 36630, 38189, 40739, 43950, 47339, 48939, }
    }
    ,
    {
        { 34, 35, 36, 38, 39, 40, 40, 42, 44, 45, 46, 47, 50, 53, 56, 57, 58, },
        { 56, 57, 59, 61, 64, 66, 67, 68, 70, 72, 75, 79, 81, 85, 91, 93, 95, },
        { 212, 218, 227, 233, 241, 247, 254, 262, 268, 275, 283, 297, 308, 325, 345, 356, 362, },
        { 612, 632, 653, 673, 692, 713, 733, 754, 775, 795, 816, 858, 890, 941, 993, 1026, 1042, },
        { 1341, 1383, 1428, 1474, 1517, 1563, 1607, 1651, 1696, 1742, 1787, 1878, 1948, 2060, 2176, 2248, 2285, },
        { 2335, 2412, 2491, 2567, 2646, 2723, 2801, 2878, 2957, 3035, 3116, 3274, 3396, 3589, 3796, 3921, 3984, },
        { 3659, 3781, 3903, 4024, 4146, 4267, 4389, 4511, 4634, 4757, 4882, 5131, 5321, 5624, 5943, 6142, 6242, },
        { 5224, 5396, 5571, 5743, 5917, 6090, 6264, 6440, 6617, 6792, 6970, 7323, 7596, 8026, 8485, 8770, 8910, },
        { 6867, 7095, 7323, 7551, 7779, 8009, 8237, 8468, 8698, 8929, 9162, 9626, 9987, 10550, 11159, 11529, 11720, },
        { 8751, 9042, 9332, 9623, 9915, 10209, 10500, 10790, 11079, 11379, 11679, 12270, 12729, 13480, 14250, 14730, 14959, },
        { 11090, 11459, 11829, 12200, 12570, 12940, 13330, 13709, 14080, 14459, 14840, 15590, 16169, 17080, 18059, 18680, 18969, },
        { 13759, 14219, 14680, 15130, 15590, 16050, 16509, 16969, 17440, 17900, 18369, 19300, 20019, 21150, 22350, 23119, 23490, },
        { 16540, 17090, 17640, 18190, 18740, 19290, 19840, 20400, 20959, 21519, 22080, 23200, 24059, 25419, 26869, 27780, 28239, },
        { 19650, 20309, 20969, 21619, 22269, 22930, 23590, 24250, 24909, 25569, 26239, 27569, 28589, 30200, 31919, 33030, 33560, },
        { 22940, 23709, 24469, 25230, 26000, 26760, 27530, 28300, 29069, 29850, 30630, 32180, 33369, 35239, 37260, 38550, 39169, },
        { 26789, 27680, 28569, 29460, 30360, 31250, 32150, 33039, 33939, 34860, 35769, 37569, 38969, 41150, 43500, 45010, 45739, },
        { 28900, 29869, 30830, 31789, 32760, 33719, 34689, 35660, 36630, 37610, 38600, 40539, 42030, 44369, 46919, 48560, 49339, }
    }
};


// todo : change type to UINT8, return true if u4Total != 0
// if u4Total == 0, means no need to do opc...
void vDrvGetRGBHistNorm(UINT32 * u4aHist, UINT8 bSupportNorm, UINT16 u2NormBase)
{
    UINT8 bi;
    UINT32 u4Total = 0;    
    
    for (bi = 0; bi < RGB_HIST_NUM; bi++)
    {
        u4aHist[bi] = RegReadFldAlign(OS_40 + (bi * 4), CNT00);
    }

	if (bSupportNorm)
	{
		for (bi = 0; bi < RGB_HIST_NUM; bi++)
		{
			u4Total += u4aHist[bi];
		}

		for (bi = 0; bi < RGB_HIST_NUM; bi++)
		{
			if (u4Total == 0)
			{
				u4aHist[bi] = 0;
			}
			else
			{
				u4aHist[bi] =
				    (UINT16) (((UINT32)u4aHist[bi] * u2NormBase) / u4Total);				
			}
		}    
	}
}

UINT32 u4aPreNormHist[RGB_HIST_NUM];

UINT8 bDrvOPCSceneChange(const UINT32 *u4aNormHist)
{
	UINT32 u4MaxDiff = 0;
    UINT32 u4Diff = 0;	
	UINT32 u4TotalDiff = 0;
	UINT8  bi;

	if (u4aNormHist == NULL)
	{
		return SV_FALSE;
	}

	for (bi = 0; bi < 16; bi++)
	{
		u4Diff = AbsDiff(u4aNormHist[bi], u4aPreNormHist[bi]);
		u4TotalDiff += u4Diff;
		
		u4MaxDiff = (u4MaxDiff < u4Diff) ? u4Diff : u4MaxDiff;

		u4aPreNormHist[bi] = u4aNormHist[bi];
	}

	if ((u4MaxDiff > OPC_MAX_DIFF_THD ) && 
	    (u4TotalDiff > OPC_TOTAL_DIFF_THD))
	{
		return SV_TRUE;
	}
	else
	{
		return SV_FALSE;
	}
}

UINT8 bDrvOPCFindMaxContent(const UINT32 *u4aNormHist)
{
    UINT16 bi;
    UINT32 u4HistCount;
    UINT32 u4MaxThd;
    
    UINT16 u2HStart, u2HEnd, u2VStart, u2VEnd;
    UINT16 u2MaxThdRatio = IO32ReadFldAlign(OPC_0A, OPC_MAX_CONTENT_TH);

    u2HStart = RegReadFldAlign(OS_3C, MAX_METER_HSTART);
    u2HEnd = RegReadFldAlign(OS_3C, MAX_METER_HEND);
    u2VStart = RegReadFldAlign(OS_3D, MAX_METER_VSTART);
    u2VEnd = RegReadFldAlign(OS_3D, MAX_METER_VEND);    

    u4HistCount = (u2HEnd - u2HStart) * (u2VEnd - u2VStart);

    u4MaxThd = (((u2HEnd - u2HStart) * (u2VEnd - u2VStart)) * u2MaxThdRatio) / 1000;

    // todo : adaptive max threshold here

    for(bi = RGB_HIST_NUM - 1; bi > 0; bi--)
    {
        u4HistCount -= u4aNormHist[bi];

        if(u4HistCount < u4MaxThd)
            break;
    }
    
    vIO32WriteFldAlign(OPC_01, MIN((bi+1)<<4, 255), OPC_MAX_CONTENT);   
    
    return MIN((bi+1)<<4, 255);
}

void vDrvOPCTableSearch(UINT16 u2LuminanceTable[3][LUMA_TABLE_SIZE][LUMA_TABLE_SIZE], UINT8 bMaxContent, OPC_INFO *OPCInfo)
{
    UINT8 u1PWMIndex = MIN(IO32ReadFldAlign(OPC_0E, PWM_TABLE_IDX), PWM_TABLE_MAX-1);
    UINT16 u2CurrentLuminance = u2LuminanceTable[u1PWMIndex][(((UINT16)bMaxContent+1)>>4)][LUMA_TABLE_SIZE-1]; // content x pwm
    UINT8 bSearchedPWM, bSearchedContent;

    // find where PWM can reduce to
    for(bSearchedPWM = 0; bSearchedPWM < LUMA_TABLE_SIZE; bSearchedPWM++)
    {
        if(u2LuminanceTable[u1PWMIndex][LUMA_TABLE_SIZE - 1][bSearchedPWM] >= u2CurrentLuminance)
        {
            break;
        }
    }

    // for klockwork fix
    bSearchedPWM = bSearchedPWM>16?16:bSearchedPWM;

    // find where content should extend to
    for(bSearchedContent = LUMA_TABLE_SIZE - 1; bSearchedContent > 0; bSearchedContent--)
    {
        if(u2LuminanceTable[u1PWMIndex][bSearchedContent][bSearchedPWM] <= u2CurrentLuminance)
        {
            break;
        }
    }

    // calculate curve info
    bSearchedContent = (UINT8)MIN((UINT16)bSearchedContent<<4 , 255);
    
//    OPCInfo->OPC_CURVE_PWM = (UINT8)MIN(((UINT16)bSearchedPWM)<<4, 255);
    if(u1PWMIndex > 2)  // for klockwork
    {
        u1PWMIndex = 2;
    }

    if(bSearchedPWM > 16) // for klockwork
    {
        bSearchedPWM = 16;
    }
    
    OPCInfo->OPC_CURVE_PWM = OPC_PWM_INDEX[u1PWMIndex][bSearchedPWM];

    vIO32WriteFldAlign(OPC_0A, OPCInfo->OPC_CURVE_PWM, OPC_SEARCHED_PWM);
        
    if(bSearchedContent > (bMaxContent<<1)) // constrain max content gain to 2.0
    {
        OPCInfo->OPC_CURVE_P2 = MIN(bMaxContent<<1, 128);
        OPCInfo->OPC_CURVE_P1 = MIN(bMaxContent, 128);
        OPCInfo->OPC_CURVE_G2 = 0xFF;
        OPCInfo->OPC_CURVE_G1 = 0xFF;                     
    }
    else
    {
        OPCInfo->OPC_CURVE_P2 = (UINT8)MIN((UINT16)bSearchedContent, 255);
        OPCInfo->OPC_CURVE_P1 = bSearchedContent >> 1;
        OPCInfo->OPC_CURVE_G2 = (UINT8)MIN(MAX((((UINT16)bSearchedContent)<<7)/bMaxContent, 128), 255);
        OPCInfo->OPC_CURVE_G1 = OPCInfo->OPC_CURVE_G2;       
    }

    OPCInfo->OPC_CURVE_GSLP = (((INT16)OPCInfo->OPC_CURVE_G1-(INT16)OPCInfo->OPC_CURVE_G2)<<7)/((INT16)OPCInfo->OPC_CURVE_P2-(INT16)OPCInfo->OPC_CURVE_P1);        
}

typedef enum
{
	OPC_ITEM_P1 = 0,
	OPC_ITEM_P2 = 1,
	OPC_ITEM_G1  = 2,
	OPC_ITEM_G2  = 3,
	OPC_ITEM_PWM  = 4,
	OPC_ITEM_MAX,
} OPC_QUE_ITEM;

UINT32 OPCDataQue[5][256];
UINT32 OPCQueCounter[5] = {0, 0, 0, 0, 0};

UINT32 u4ReturnQueAverage(UINT32 dwValue, UINT8 bItem)
{
	UINT32 bi;
	UINT32 PWMAverage=0;
	UINT32 OPCQueSize = IO32ReadFldAlign(OPC_0E, RUN_QUE_SIZE);

	if (OPCQueCounter[bItem] == 0)
	{
		OPCQueCounter[bItem]++;
		OPCDataQue[bItem][0] = dwValue;
		return dwValue;
	}

	if (OPCQueCounter[bItem] < OPCQueSize)
	{
		OPCQueCounter[bItem]++;

	}
	else
	{
        OPCQueCounter[bItem] = OPCQueSize;
	}

	for (bi = OPCQueCounter[bItem] - 1; bi > 0 ; bi--)
	{
		OPCDataQue[bItem][bi] = OPCDataQue[bItem][bi - 1];
	}

	// Insert New Item
	OPCDataQue[bItem][0] = dwValue;

	// Average Running Que
	for (bi = 0; bi < OPCQueCounter[bItem]; bi++)
	{
		PWMAverage += OPCDataQue[bItem][bi];
	}

	PWMAverage = PWMAverage / OPCQueCounter[bItem];

	return PWMAverage;
}

void vDrvClearTPF()
{
	OPCQueCounter[0] = 0;
	OPCQueCounter[1] = 0;
	OPCQueCounter[2] = 0;
	OPCQueCounter[3] = 0;
	OPCQueCounter[4] = 0;
}

UINT32 u4MotionThd[8] = {15000, 8000, 4000, 2000, 1000, 250,  50,  10};    // from fast (short queue) to slow (long queue)
UINT16 u2MotionQue[8] = {    8,   16,   48,   64,   80,  96, 128, 255};    // from fast (short queue) to slow (long queue)
#define OPC_MOTION_THD 10000
#define OPC_MAX_QUE_SIZE 128
#define OPC_MIN_QUE_SIZE  64

UINT32 u4DrvGet3DNrMotion()
{
    UINT32 u4Cur3DNrMotion = RegReadFldAlign(NR_3DNR5C, PW_MoCnt_Final);

    return u4Cur3DNrMotion;
}

UINT32 u4DrvGetMotionQueue(UINT32 u4CurrentMotion, UINT32 u4CurrentQueueSize)
{
    if(u4CurrentMotion > OPC_MOTION_THD)
    {
        u4CurrentQueueSize--;
    }
    else
    {
        u4CurrentQueueSize++;
    }

       
    return MAX(MIN(u4CurrentQueueSize, OPC_MAX_QUE_SIZE), OPC_MIN_QUE_SIZE);
}

void vDrvOPCInfoTPF(OPC_INFO *OPCInfo, UINT8 bOPCSceneChange)
{
    OPC_INFO TargetOPCInfo;
    
    UINT8 bRunQueSize = IO32ReadFldAlign(OPC_0E, RUN_QUE_SIZE);
    
    if(!bOPCSceneChange)
    {
        // todo : fast rising, slow falling      
    	TargetOPCInfo.OPC_CURVE_G1 = OPCInfo->OPC_CURVE_G1;
	    TargetOPCInfo.OPC_CURVE_G2 = OPCInfo->OPC_CURVE_G2;
    	TargetOPCInfo.OPC_CURVE_P1 = OPCInfo->OPC_CURVE_P1;
	    TargetOPCInfo.OPC_CURVE_P2 = OPCInfo->OPC_CURVE_P2;
        TargetOPCInfo.OPC_CURVE_PWM = OPCInfo->OPC_CURVE_PWM;        
      
        // adaptively change queue size
        bRunQueSize = u4DrvGetMotionQueue(u4DrvGet3DNrMotion(), bRunQueSize);
        
        vIO32WriteFldAlign(OPC_0E, bRunQueSize, RUN_QUE_SIZE);

        if(TargetOPCInfo.OPC_CURVE_PWM > OPCInfo->OPC_CURVE_PWM)    // need to rise-up in PWM, content from dark to bright
        {
           	OPCInfo->OPC_CURVE_G1 = u4ReturnQueAverage(TargetOPCInfo.OPC_CURVE_G1, OPC_ITEM_P1);
    	    OPCInfo->OPC_CURVE_G2 = u4ReturnQueAverage(TargetOPCInfo.OPC_CURVE_G2, OPC_ITEM_P2);
    	    OPCInfo->OPC_CURVE_P1 = u4ReturnQueAverage(TargetOPCInfo.OPC_CURVE_P1, OPC_ITEM_G1);
	        OPCInfo->OPC_CURVE_P2 = u4ReturnQueAverage(TargetOPCInfo.OPC_CURVE_P2, OPC_ITEM_G2);
            OPCInfo->OPC_CURVE_PWM = u4ReturnQueAverage(TargetOPCInfo.OPC_CURVE_PWM, OPC_ITEM_PWM);
        
        	OPCInfo->OPC_CURVE_G1 = u4ReturnQueAverage(TargetOPCInfo.OPC_CURVE_G1, OPC_ITEM_P1);
    	    OPCInfo->OPC_CURVE_G2 = u4ReturnQueAverage(TargetOPCInfo.OPC_CURVE_G2, OPC_ITEM_P2);
    	    OPCInfo->OPC_CURVE_P1 = u4ReturnQueAverage(TargetOPCInfo.OPC_CURVE_P1, OPC_ITEM_G1);
	        OPCInfo->OPC_CURVE_P2 = u4ReturnQueAverage(TargetOPCInfo.OPC_CURVE_P2, OPC_ITEM_G2);
            OPCInfo->OPC_CURVE_PWM = u4ReturnQueAverage(TargetOPCInfo.OPC_CURVE_PWM, OPC_ITEM_PWM);        
        }
        else
        {
        	OPCInfo->OPC_CURVE_G1 = u4ReturnQueAverage(TargetOPCInfo.OPC_CURVE_G1, OPC_ITEM_P1);
    	    OPCInfo->OPC_CURVE_G2 = u4ReturnQueAverage(TargetOPCInfo.OPC_CURVE_G2, OPC_ITEM_P2);
        	OPCInfo->OPC_CURVE_P1 = u4ReturnQueAverage(TargetOPCInfo.OPC_CURVE_P1, OPC_ITEM_G1);
    	    OPCInfo->OPC_CURVE_P2 = u4ReturnQueAverage(TargetOPCInfo.OPC_CURVE_P2, OPC_ITEM_G2);
            OPCInfo->OPC_CURVE_PWM = u4ReturnQueAverage(TargetOPCInfo.OPC_CURVE_PWM, OPC_ITEM_PWM);
        }
    }
    else
    {  

		OPCQueCounter[0] = 0;
		OPCQueCounter[1] = 0;
		OPCQueCounter[2] = 0;
		OPCQueCounter[3] = 0;
		OPCQueCounter[4] = 0;            
		
    	OPCInfo->OPC_CURVE_G1 = u4ReturnQueAverage(OPCInfo->OPC_CURVE_G1, OPC_ITEM_P1);
        OPCInfo->OPC_CURVE_G2 = u4ReturnQueAverage(OPCInfo->OPC_CURVE_G2, OPC_ITEM_P2);
    	OPCInfo->OPC_CURVE_P1 = u4ReturnQueAverage(OPCInfo->OPC_CURVE_P1, OPC_ITEM_G1);
        OPCInfo->OPC_CURVE_P2 = u4ReturnQueAverage(OPCInfo->OPC_CURVE_P2, OPC_ITEM_G2);
        OPCInfo->OPC_CURVE_PWM = u4ReturnQueAverage(OPCInfo->OPC_CURVE_PWM, OPC_ITEM_PWM);        

    }
}

void vDrvOPCInfoToSwReg(OPC_INFO *OPCInfo)
{
    vIO32WriteFldAlign(OPC_0B, OPCInfo->OPC_CURVE_P1, OPC_SW_P1);
    vIO32WriteFldAlign(OPC_0B, OPCInfo->OPC_CURVE_P2, OPC_SW_P2);        
    vIO32WriteFldAlign(OPC_0B, OPCInfo->OPC_CURVE_G1, OPC_SW_G1);
    vIO32WriteFldAlign(OPC_0B, OPCInfo->OPC_CURVE_G2, OPC_SW_G2);
    vIO32WriteFldAlign(OPC_0C, OPCInfo->OPC_CURVE_GSLP, OPC_SW_G_SLOPE);    
    vIO32WriteFldAlign(OPC_0C, OPCInfo->OPC_CURVE_PWM, OPC_SW_PWM);            
    vApiRegisterVideoEvent(PE_ARG_OPC_CURVE, SV_VP_MAIN, SV_ON);		  
}

void vDrvOPCSetDefault()
{
    vIO32WriteFldAlign(OPC_0B, 0x80, OPC_SW_P1);
    vIO32WriteFldAlign(OPC_0B, 0x80, OPC_SW_P2);        
    vIO32WriteFldAlign(OPC_0B, 0x80, OPC_SW_G1);
    vIO32WriteFldAlign(OPC_0B, 0x80, OPC_SW_G2);
    vIO32WriteFldAlign(OPC_0C, 0x0, OPC_SW_G_SLOPE);    
    vIO32WriteFldAlign(OPC_0C, 0xFF, OPC_SW_PWM);        
    vApiRegisterVideoEvent(PE_ARG_OPC_CURVE, SV_VP_MAIN, SV_ON);		  
}

UINT16 u2MaxContentThd = 994;

void vDrvAdaptiveSetMaxThd(const UINT32 *u4aNormHist)    
{
    UINT8 bi;
    UINT32 u4HistCount = 0;
    UINT32 u4HistSum = 0;    
    UINT32 u4APL = 0;    
    UINT16 u2MaxContentThdApplied = u2MaxContentThd;
    
    for(bi = 0; bi < RGB_HIST_NUM; bi++)
    {
        u4HistSum += u4aNormHist[bi] * MIN(((bi+1) << 4),255);
        u4HistCount += u4aNormHist[bi];
    }

    if(u4HistCount == 0)
    {
        u4APL = 0x0;
    }
    else
    {
        u4APL = u4HistSum / u4HistCount;
    }  

    if(u4APL > 0x80)
    {
        u2MaxContentThdApplied = u2MaxContentThdApplied + ((u4APL-0x80)>>5);
    }
    else
    {
        u2MaxContentThdApplied = u2MaxContentThdApplied - ((0x80-u4APL)>>5);
    }

    vIO32WriteFldAlign(OPC_0A, u2MaxContentThdApplied, OPC_MAX_CONTENT_TH);
}

void vDrvOPCProc()
{
    UINT32 OPCRGBMaxNormHist[RGB_HIST_NUM];
    UINT8  bOPCSceneChange = SV_FALSE;
    UINT8  bMaxContent = 0;
    OPC_INFO sOPCInfo;
    // a. parameters:
        // a. 17x17 luminance array
        // b. rgb histogram
    // 0. if on
    if(IO32ReadFldAlign(OPC_01, OPC_ON_OFF))
    {
        // 0. read curve (with normalization)
            // in : rgb histogram
            // out : rgb normalized histogram
        vDrvGetRGBHistNorm(OPCRGBMaxNormHist, SV_FALSE, OPC_HIST_NORMBASE);        
        // 1. detect scene change
            // in : histogram
            // out : scene change flag
        bOPCSceneChange = bDrvOPCSceneChange(OPCRGBMaxNormHist);
        // 2. find maximum
            // in : rgb histogram
            // out : maximum value

        vDrvAdaptiveSetMaxThd(OPCRGBMaxNormHist);
        
        bMaxContent = bDrvOPCFindMaxContent(OPCRGBMaxNormHist);        
        // 3. table searching
            // in : 17x17 luminance table, current pwm, max content
            // in : maximum value
            // out : (pwm, content) of maximum content
        vDrvOPCTableSearch(OPCLuminanceTable, bMaxContent, &sOPCInfo);
        // 4. temporal filtering
            // in : opc info, scene change flag
            // out : filtered opc info
        vDrvOPCInfoTPF(&sOPCInfo, bOPCSceneChange);
        // 5. update to sw register          
        vDrvOPCInfoToSwReg(&sOPCInfo);
    }
    else
    {
        vDrvClearTPF();
        // 7. set default
        vDrvOPCSetDefault();      
    }
    opc_cnt++;    
}
///////////////////////////////////////////////////////////////////////////
