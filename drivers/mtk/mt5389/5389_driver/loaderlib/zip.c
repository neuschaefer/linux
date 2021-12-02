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
 * Description:
 *         ZIP operations
 *
 *---------------------------------------------------------------------------*/

//===========================================================================

#include "x_os.h"
#include "zip.h"


//---------------------------------------------------------------------------
// Callback functions for zlib
//---------------------------------------------------------------------------


/*----------------------------------------------------------------------------
 * Function:
 *
 * Description:
 *
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
static voidpf _zcalloc (voidpf opaque, unsigned items, unsigned size)
{
    return x_mem_alloc(items * size);
}


/*----------------------------------------------------------------------------
 * Function:
 *
 * Description:
 *
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
static void _zcfree (voidpf opaque, voidpf ptr)
{
    x_mem_free(ptr);
}


//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------


/*----------------------------------------------------------------------------
 * Function: _Compress
 *
 * Description:
 *      Compress source block to destination address
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
#if 0
static int _Compress(Bytef* dest, uLongf* destLen, const Bytef* source,
    uLong sourceLen, int level)
{
    z_stream stream;
    int err;

    stream.next_in = (Bytef*)source;
    stream.avail_in = (uInt)sourceLen;

    stream.next_out = dest;
    stream.avail_out = (uInt)*destLen;
    if ((uLong)stream.avail_out != *destLen) return Z_BUF_ERROR;

    stream.zalloc = (alloc_func)_zcalloc;
    stream.zfree = (free_func)_zcfree;
    stream.opaque = (voidpf)0;

    err = deflateInit(&stream, level);
    if (err != Z_OK) return err;

    err = deflate(&stream, Z_FINISH);
    if (err != Z_STREAM_END) {
        deflateEnd(&stream);
        return err == Z_OK ? Z_BUF_ERROR : err;
    }
    *destLen = stream.total_out;

    err = deflateEnd(&stream);
    return err;
}
#endif


/*----------------------------------------------------------------------------
 * Function: _Uncompress
 *
 * Description:
 *      Decompress from source block to destination address
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
static int _Uncompress (Bytef* dest, uLongf* destLen, const Bytef* source,
    uLong sourceLen)
{
    z_stream stream;
    int err;

    stream.next_in = (Bytef*)source;
    stream.avail_in = (uInt)sourceLen;
    /* Check for source > 64K on 16-bit machine: */
    if ((uLong)stream.avail_in != sourceLen) return Z_BUF_ERROR;

    stream.next_out = dest;
    stream.avail_out = (uInt)*destLen;
    if ((uLong)stream.avail_out != *destLen) return Z_BUF_ERROR;

    stream.zalloc = (alloc_func)_zcalloc;
    stream.zfree = (free_func)_zcfree;

    err = inflateInit(&stream);
    if (err != Z_OK) return err;

    err = inflate(&stream, Z_FINISH);
    if (err != Z_STREAM_END) {
        inflateEnd(&stream);
        if (err == Z_NEED_DICT || (err == Z_BUF_ERROR && stream.avail_in == 0))
            return Z_DATA_ERROR;
        return err;
    }
    *destLen = stream.total_out;

    err = inflateEnd(&stream);
    return err;
}


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------


/*----------------------------------------------------------------------------
 * Function:
 *
 * Description:
 *
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
const CHAR* ZIP_ErrorString(INT32 i4ErrCode)
{
    const CHAR* psz = NULL;

    switch (i4ErrCode)
    {
    case Z_OK:
        psz = "OK";
        break;

	case Z_MEM_ERROR:
		psz = "Not enough memory for decompression";
		break;

	case Z_BUF_ERROR:
		psz = "Decompressed data larger than original";
		break;

	case Z_DATA_ERROR:
		psz = "Data error";
		break;

	default:
		psz = "Unknown error";
		break;
    }

    return psz;
}


/*----------------------------------------------------------------------------
 * Function:
 *
 * Description:
 *
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
INT32 ZIP_Compress(UINT32 u4SrcAddr, UINT32 u4SrcSize, UINT32 u4DstAddr,
    UINT32* pu4DstSize)
{
#if 0
    INT32 i4Ret;

    *pu4DstSize = compressBound(u4SrcSize);

    i4Ret = _Compress((UINT8*)u4DstAddr, (unsigned long*)pu4DstSize,
        (UINT8*)u4SrcAddr, u4SrcSize, Z_DEFAULT_COMPRESSION);

    return i4Ret;
#endif
}


/*----------------------------------------------------------------------------
 * Function:
 *
 * Description:
 *
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
INT32 ZIP_Decompress(UINT32 u4SrcAddr, UINT32 u4SrcSize, UINT32 u4DstAddr,
    UINT32* pu4DstSize)
{
    INT32 i4Ret;

    *pu4DstSize = 0x10000000;

	i4Ret = _Uncompress((UINT8*)u4DstAddr, (unsigned long*)pu4DstSize,
	    (UINT8*)u4SrcAddr, u4SrcSize);

    return i4Ret;
}

