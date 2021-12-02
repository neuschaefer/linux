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
/*----------------------------------------------------------------------------*/
/*! @file u_cl_zip_eng.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This file exports all compression library specific definitions and structures.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_CL
  *  @{  
  */
/*----------------------------------------------------------------------------*/
#ifndef _U_CL_ZIP_ENG_H_
#define _U_CL_ZIP_ENG_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/****************************************************************************/
/*                       zlib compression parameter                         */
/****************************************************************************/
#define CL_ZIP_NO_COMPRESSION         0    /**<0 gives no compression at all (the input data is simply copied a block at a time).       */
#define CL_ZIP_BEST_SPEED             1    /**<1 gives best speed.        */
#define CL_ZIP_BEST_COMPRESSION       9    /**<9 gives best compression.        */
#define CL_ZIP_DEFAULT_COMPRESSION    CL_ZIP_BEST_COMPRESSION    /**<Default compression level.        */
/* compression levels */
/*
The compression level must be between 0 and 9: 
1 gives best speed, 
9 gives best compression, 
0 gives no compression at all (the input data is simply copied a block at a time).
*/
#define CL_ZIP_MAX_WBITS               15    /**<The windowBits parameter is the base two logarithm of the window size (the size of the history buffer). It should be in the range 8..15 for this version of the library. Larger values of this parameter result in better compression at the expense of memory usage.         */
#define CL_ZIP_DEF_WBITS               CL_ZIP_MAX_WBITS    /**<The default value is 15 if deflateInit is used instead.CAUTION: The decompression window bits should be larger than compression.        */
/*
The windowBits parameter is the base two logarithm of the window size (the size of the history buffer). 
It should be in the range 8..15 for this version of the library. 
Larger values of this parameter result in better compression at the expense of memory usage. 
The default value is 15 if deflateInit is used instead.
CAUTION: The decompression window bits should be larger than compression.
*/
#define CL_ZIP_DEF_MEM_LEVEL 8    /**<Specifies how much memory should be allocated for the internal compression state.  The default value is 8. Deflate memory usage (bytes) = (1 << (windowBits+2)) + (1 << (memLevel+9)) .      */
#define CL_ZIP_MAX_MEM_LEVEL 9    /**<Maximum memory for optimal speed.         */
#define CL_ZIP_MIN_MEM_LEVEL 1    /**<Minimum memory but is slow and reduces compression ratio.        */
/*
The memLevel parameter specifies how much memory should be allocated for the internal compression state. 
memLevel=1 uses minimum memory but is slow and reduces compression ratio ; 
memLevel=9 uses maximum memory for optimal speed. 
The default value is 8. 
*/

/*
The memory requirements for compression depend on two parameters, windowBits and memLevel:

    deflate memory usage (bytes) = (1 << (windowBits+2)) + (1 << (memLevel+9)) 
    inflate memory usage (bytes) = (1 << windowBits) + 1440*2*sizeof(int)
*/

/*------------------------------------------------------------------*/
/*! @enum CL_ZIP_COMPRESSION_STRATEGY_T
 *  @brief   The strategy parameter is used to tune the compression algorithm. Use the
 *              value Z_DEFAULT_STRATEGY for normal data, Z_FILTERED for data produced by a
 *              filter (or predictor), Z_HUFFMAN_ONLY to force Huffman encoding only (no
 *              string match), or Z_RLE to limit match distances to one (run-length
 *              encoding). Filtered data consists mostly of small values with a somewhat
 *              random distribution. In this case, the compression algorithm is tuned to
 *              compress them better. The effect of Z_FILTERED is to force more Huffman
 *              coding and less string matching; it is somewhat intermediate between
 *              Z_DEFAULT and Z_HUFFMAN_ONLY. Z_RLE is designed to be almost as fast as
 *              Z_HUFFMAN_ONLY, but give better compression for PNG image data. The strategy
 *              parameter only affects the compression ratio but not the correctness of the
 *              compressed output even if it is not set appropriately.
 *  @code
 *  typedef enum 
 *  {
 *      CL_ZIP_DEFAULT_STRATEGY = 0,
 *      CL_ZIP_FILTERED,
 *      CL_ZIP_HUFFMAN_ONLY,
 *      CL_ZIP_RLE,
 *      CL_ZIP_LAST_ONE 
 *  } CL_ZIP_COMPRESSION_STRATEGY_T;
 *  @endcode
 *  @li@c  CL_ZIP_DEFAULT_STRATEGY                           -Normal data.
 *  @li@c  CL_ZIP_FILTERED                                   -Data produced by a filter (or predictor).
 *  @li@c  CL_ZIP_HUFFMAN_ONLY                               -Force Huffman encoding only (no string match).
 *  @li@c  CL_ZIP_RLE                                        - Limit match distances to one (run-length encoding).
 *  @li@c  CL_ZIP_LAST_ONE                                   - No use, just a latest one indicator.
 */
/*------------------------------------------------------------------*/
typedef enum 
{
    CL_ZIP_DEFAULT_STRATEGY = 0,
    CL_ZIP_FILTERED,
    CL_ZIP_HUFFMAN_ONLY,
    CL_ZIP_RLE,
    CL_ZIP_LAST_ONE /* no use, just a latest one indicator*/
} CL_ZIP_COMPRESSION_STRATEGY_T;
/* The strategy parameter is used to tune the compression algorithm. Use the
   value Z_DEFAULT_STRATEGY for normal data, Z_FILTERED for data produced by a
   filter (or predictor), Z_HUFFMAN_ONLY to force Huffman encoding only (no
   string match), or Z_RLE to limit match distances to one (run-length
   encoding). Filtered data consists mostly of small values with a somewhat
   random distribution. In this case, the compression algorithm is tuned to
   compress them better. The effect of Z_FILTERED is to force more Huffman
   coding and less string matching; it is somewhat intermediate between
   Z_DEFAULT and Z_HUFFMAN_ONLY. Z_RLE is designed to be almost as fast as
   Z_HUFFMAN_ONLY, but give better compression for PNG image data. The strategy
   parameter only affects the compression ratio but not the correctness of the
   compressed output even if it is not set appropriately.
 */
/*------------------------------------------------------------------*/
/*! @struct CL_ZIP_COMPRESSION_T
 *  @brief   Compression parameter.
 *  @code
 *  typedef struct _CL_ZIP_COMPRESSION_T
 *  {
 *      UINT8                           ui1_level;
 *      UINT8                           ui1_windowbits;
 *      UINT8                           ui1_mem_level;
 *      CL_ZIP_COMPRESSION_STRATEGY_T   e_strategy;
 *  } CL_ZIP_COMPRESSION_T;
 *  @endcode
 *  @li@c  ui1_level                                         - Compression levels,The compression level must be between 0 and 9: 1 gives best speed, 9 gives best compression, 0 gives no compression at all (the input data is simply copied a block at a time).
 *  @li@c  ui1_windowbits                                    - The windowBits parameter is the base two logarithm of the window size (the size of the history buffer). It should be in the range 8..15 for this version of the library. 
 *  @li@c  ui1_mem_level                                     - The memLevel parameter specifies how much memory should be allocated for the internal compression state.
 *  @li@c  e_strategy                                        - The strategy parameter is used to tune the compression algorithm.
 */
/*------------------------------------------------------------------*/
typedef struct _CL_ZIP_COMPRESSION_T
{
    UINT8                           ui1_level;/*0~9*/
    UINT8                           ui1_windowbits;/*8~15*/
    UINT8                           ui1_mem_level;/*1~9*/
    CL_ZIP_COMPRESSION_STRATEGY_T   e_strategy;
} CL_ZIP_COMPRESSION_T;

/*------------------------------------------------------------------*/
/*! @struct CL_ZIP_DECOMPRESSION_T
 *  @brief The memory requirements for compression depend on two parameters, windowBits and memLevel: deflate memory usage (bytes) = (1 << (windowBits+2)) + (1 << (memLevel+9))  inflate memory usage (bytes) = (1 << windowBits) + 1440*2*sizeof(int)
 *  @code
 *  typedef struct _CL_ZIP_DECOMPRESSION_T
 *  {
 *      UINT8                           ui1_windowbits;
 *  } CL_ZIP_DECOMPRESSION_T;
 *  @endcode
 *  @li@c  ui1_windowbits                                    - The windowBits parameter is the base two logarithm of the window size (the size of the history buffer). It should be in the range 8..15 for this version of the library. 
 */
/*------------------------------------------------------------------*/
typedef struct _CL_ZIP_DECOMPRESSION_T
{
    UINT8                           ui1_windowbits;/*8~15*/
} CL_ZIP_DECOMPRESSION_T;

#define CL_ZIP_ENGINE_COMPRESS_PARAM_TYPE    0x2    /**<Compress parameter type.        */
#define CL_ZIP_ENGINE_DECOMPRESS_PARAM_TYPE  0x3    /**<Decompress parameter type.        */

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

#endif /* _U_CL_ZIP_ENG_H_ */
/*----------------------------------------------------------------------------*/
/*! @} */                                                 /* end of groupMW_CL*/
/*----------------------------------------------------------------------------*/
