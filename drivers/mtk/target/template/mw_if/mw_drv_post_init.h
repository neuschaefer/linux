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
#if ! defined(MW_DRV_POST_INIT_H)
#define MW_DRV_POST_INIT_H

#define DMX_SEC_COMP_ID_START   (DMX_SECTION_MEM_COMP_ID_START)
#define DMX_SEC_COMP_NUM        (DMX_SECTION_MEM_COMP_NUM)
#define DMX_FLT_COMP_ID_START   (DMX_SECTION_FILTER_COMP_ID_START)
#define DMX_FLT_COMP_NUM        (DMX_SECTION_FILTER_COMP_NUM)

/* working comments


*/

/*  these compile time definitions should add in .cfg file of each release setup file
#define N_DRVC_TUNER    1
#define N_DRVC_PCR      1
#define N_DRVC_PES      2
#define N_DRVC_SEC      8
#define N_DRVC_FLT      16
#define N_DRVC_TVD      1
#define N_DRVC_MPV      1
#define N_DRVC_MPVHD    1
#define N_DRVC_AUD      1
#define N_DRVC_VDP      1
#define N_DRVC_PMX      1
#define N_DRVC_OSD      1
*/

#if ! defined( N_DRVC_TUNER )
    #define N_DRVC_TUNER    2
#endif

#if ! defined( N_DRVC_ANATUNER )
    #define N_DRVC_ANATUNER 2
#endif

#if ! defined( N_DRVC_PCR )
    #define N_DRVC_PCR      DMX_PCR_COMP_NUM
#endif

#if ! defined( N_DRVC_PES )
    #define N_DRVC_PES      DMX_PES_COMP_NUM
#endif

#if ! defined( N_DRVC_PES_MEM )
    #define N_DRVC_PES_MEM  DMX_PES_MEM_COMP_NUM
#endif

#if ! defined( N_DRVC_DES )
    #define N_DRVC_DES      N_DRVC_PES
#endif

#if ! defined( N_DRVC_SEC )
    #define N_DRVC_SEC      DMX_SECTION_MEM_COMP_NUM
#endif

#if ! defined( N_DRVC_FLT )
    #define N_DRVC_FLT      DMX_SECTION_FILTER_COMP_NUM
#endif

#if ! defined( N_DRVC_TVD )
    #define N_DRVC_TVD      TVD_COMP_NS
#endif

#if ! defined( N_DRVC_MPV )
    #define N_DRVC_MPV      MPV_COMP_NS
#endif

#if ! defined( N_DRVC_MPVHD )
    #define N_DRVC_MPVHD    (MPV_COMP_NS - MPV_COMP_SD_NS)
#endif

#if ! defined( N_DRVC_AUD )
    #define N_DRVC_AUD      AUD_COMP_NS
#endif

#if ! defined( N_DRVC_VDP )
    #define N_DRVC_VDP      VDP_COMP_NS
#endif

#if ! defined( N_DRVC_OSD )
    #define N_DRVC_OSD      OSD_COMP_NS
#endif

#if ! defined( N_DRVC_PMX )
    #define N_DRVC_PMX      PMX_COMP_NS
#endif

#if ! defined( N_DRVC_JPG )
    #define N_DRVC_JPG      1
#endif

#if ! defined( N_DRVC_SCART )
    #define N_DRVC_SCART      1
#endif

#if ! defined( N_DRVC_BAGT )
    #define N_DRVC_BAGT      1
#endif

#define N_GRP_MAIN \
    ( \
        N_DRVC_ANATUNER + \
        N_DRVC_TUNER + \
        1 + /* pcr0 */ \
        1 + /* mpv4 */ \
        1 + /* aud0 */ \
        1 + /* vdp0 */ \
        1 + /* pmx0 */ \
        N_DRVC_TVD     \
    )

#define N_GRP_SUB \
    ( \
        N_DRVC_ANATUNER + \
        N_DRVC_TUNER + \
        1 + /* pcr1 */ \
        N_DRVC_MPV +   \
        1 + /* aud0 */ \
        1 + /* vdp1 */ \
        1 + /* pmx0 */ \
        N_DRVC_TVD   \
    )

#define N_GRP_AUX 0     /* no aux group in 537x */

/*#define N_GRP_AUX \ */
/*  ( \
      N_DRVC_TUNER + \
      1 + \ *//* pcr2 */
/*      N_DRVC_MPV + \
      1 + \ *//* aud1 */
/*      1 + \ *//* vdp1 */
/*      1 \ *//* pmx1 */
/*  )*/

#define N_GRP_AUX_SUND \
    ( \
        1 + /* pcr1 */ \
        1  /* aud1 */ \
    )

/*************************************************
*       GLOBAL configuration
*************************************************/
#define N_DRVC_ALL \
    (N_DRVC_TUNER + N_DRVC_PCR + N_DRVC_PES +\
    N_DRVC_PES_MEM + \
    N_DRVC_SEC + N_DRVC_FLT + N_DRVC_MPV + \
    N_DRVC_AUD + N_DRVC_PMX + N_DRVC_VDP + \
    N_DRVC_OSD + N_DRVC_DES + N_DRVC_TVD + \
    N_DRVC_ANATUNER + N_DRVC_JPG + \
    N_DRVC_SCART + N_DRVC_BAGT)


#define N_GRP_ALL \
    (N_GRP_MAIN + N_GRP_SUB + N_GRP_AUX + N_GRP_AUX_SUND)


#endif /* MW_DRV_POST_INIT_H */
