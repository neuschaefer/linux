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
 * $RCSfile: _cl_huffman_decode_engine.c,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/2 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 8ef23fb86a9316f02511ecb034464f36 $ Asa
 *
 * Description:
 *         This file contains all Huffman decoding engine related API's.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "os/inc/x_os.h"
#include "cl/x_cl.h"
#include "cl/_cl.h"
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* bit reader return value */
#define BIT_READER_OK ((INT32)0)
#define BIT_READER_RUN_OUT ((INT32)1)
#define BIT_READER_BAD_PARAM ((INT32)-1)

#define BYTE_WRITE_OK ((INT32)0)
#define BYTE_WRITE_END ((INT32)1)
#define BYTE_WRITE_INV_ARG ((INT32)-1)

/*bit reader internal structure for keep tracking bit_reader read position*/
typedef struct _BIT_READER_POS_T
{
    UINT8    ui1_bit_pos;    /* bit position in a byte */
    UINT32     ui4_byte_pos;   /* byte position in the array and it is 0 base*/
} BIT_READER_POS_T;


/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/
/* extern program title and program description decoding table */
extern UCHAR _title_dec_table[];
extern UCHAR _desc_dec_table[];
extern UCHAR _dvbt_dec_table1[];
extern UCHAR _dvbt_dec_table2[];


/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name:  _cl_byte_writer
 *
 * Description: This API will write a byte to destination buffer.
 *
 * Inputs:  uc_bits_char    contains the character which will be written.
 *          pv_dest_buf     specified destination buffer.
 *          z_skip_len      specified skip bytes number of output bytes.
 *          pz_dest_len     specified the length of destination buffer .
 *
 * Outputs: ui4_pos   contain the writing position of the byte array
 *
 * Returns: BYTE_WRITE_OK          Success.
 *          BYTE_WRITE_INV_ARG     FAIL. Invalid arguments.
 ----------------------------------------------------------------------------*/

static INT32 _cl_byte_writer(
                UCHAR           uc_bits_char,
                VOID*           pv_dest_buf,
                SIZE_T          z_skip_len,
                SIZE_T*         pz_dest_len,
                UINT32*         ui4_pos
                )
{
    SIZE_T z_pos;
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/

    if ( ((ui4_pos == NULL) || (pz_dest_len == NULL))
          || ((pv_dest_buf == NULL) && (*pz_dest_len != 0)) )
    {
        return BYTE_WRITE_INV_ARG;
    }

    /************************************************************************/
    /* Perform byte writer                                                  */
    /************************************************************************/
    /* protect z_pos underflow*/
    if (*ui4_pos >= z_skip_len)
    {
        z_pos = *ui4_pos - z_skip_len;

        if (pv_dest_buf)
        {
            if (z_pos < *pz_dest_len)
            { /* really write byte */
                ((UCHAR*)pv_dest_buf)[z_pos] = uc_bits_char;
            }
        }
    }

    *ui4_pos = *ui4_pos + 1;
    return BYTE_WRITE_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _cl_bit_reader
 *
 * Description: This API will read the number ui1_num_of_bits of bits form
 *              the source buffer to pui1_bit_return.
 *
 * Inputs:  ui1_num_of_bits contains the number of bits which want to be read
 *          pv_src_buf      point to source buffer
 *          z_src_len       the length of source buffer
 *
 * Outputs: pui1_bit_return   contain the value after read these bits
 *          pt_bitreader_pos  point to bit reader position structure
 *
 * Returns: BIT_READER_OK           Success.
 *          BIT_READER_RUN_OUT      Success. no data available in the source buffer.
 *          BIT_READER_BAD_PARAM    FAIL. Invalid arguments.
 ----------------------------------------------------------------------------*/

static INT32 _cl_bit_reader (
        UINT8                       ui1_num_of_bits,
        const VOID*                 pv_src_buf,
        SIZE_T                      z_src_len,
        UINT8*                      pui1_bit_return,
        BIT_READER_POS_T*           pt_bitreader_pos
        )
{
    register UINT8 ui1_bit_read;
    register UINT8 ui1_bit_read_left = ui1_num_of_bits;
    UINT8 ui1_res = 0;
    register UINT8 tmp;
    register UINT32 ui4_byte_pos;
    register UINT8  ui1_bit_pos;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/

    if ( ((ui1_num_of_bits > 8) || (pv_src_buf == NULL) || (pui1_bit_return == NULL) || (pt_bitreader_pos == NULL))
        || ((z_src_len != 0) && (pv_src_buf == NULL)) )
    {
        return BIT_READER_BAD_PARAM;
    }

    /************************************************************************/
    /* Perform bit_reader                                                   */
    /************************************************************************/
    ui4_byte_pos = pt_bitreader_pos->ui4_byte_pos;
    ui1_bit_pos = pt_bitreader_pos->ui1_bit_pos;

    do{
        if (ui4_byte_pos/*0 base*/ >= z_src_len/*1 base*/)
        {
            pt_bitreader_pos->ui4_byte_pos = ui4_byte_pos;
            pt_bitreader_pos->ui1_bit_pos = ui1_bit_pos;
            return BIT_READER_RUN_OUT;
        }

        ui1_bit_read = (UINT8)(MAX_BIT_NUM_IN_BYTE - ui1_bit_pos);

        if (ui1_bit_read > ui1_bit_read_left)
        {
            ui1_bit_read = ui1_bit_read_left;
        }
        /* read bit*/
        tmp = (((UCHAR*)pv_src_buf)[ui4_byte_pos] << ui1_bit_pos);
        ui1_res = (ui1_res << ui1_bit_read) | (tmp >> (MAX_BIT_NUM_IN_BYTE - ui1_bit_read));

        ui1_bit_read_left = ui1_bit_read_left - ui1_bit_read;
        /************************************************************************/
        /*  update pt_bitreader_pos                                             */
        /************************************************************************/

        ui1_bit_pos = ui1_bit_pos + ui1_bit_read;

        if (ui1_bit_pos == MAX_BIT_NUM_IN_BYTE)
        {
            ui4_byte_pos ++;
            ui1_bit_pos = 0;
        }
    }while(ui1_bit_read_left);

    pt_bitreader_pos->ui4_byte_pos = ui4_byte_pos;
    pt_bitreader_pos->ui1_bit_pos = ui1_bit_pos;

    *pui1_bit_return = ui1_res;

    return BIT_READER_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _cl_huffman_decode
 *
 * Description: This API will use e_huffman_type table to decode data
 *              from source buffer to destination buffer
 *
 * Inputs:  e_huffman_type  Points to the handle control structure
 *          pv_src_buf  source buffer
 *          z_src_len   the length of source buffer
 *          z_skip_len  the skip byte number of output
 *          pz_dest_len     the length of destination buffer
 *
 * Outputs: pv_dest_buf     destination buffer, and it can be NULL for geting length
 *          pz_dest_len     the real length of destination buffer
 *
 * Returns: CLR_OK                      Success.
 *          CLR_INSUFFICIENT_BUFFER     FAIL. destination buffer is not enough.
 *          CLR_INV_ARG                 FAIL. Invalid arguments.
 ----------------------------------------------------------------------------*/

extern INT32 _cl_huffman_decode (
        CL_HUFFMAN_TYPE_T        e_huffman_type,     /* use what kind of table */
        const VOID*              pv_src_buf,
        SIZE_T                   z_src_len,
        VOID*                    pv_dest_buf, /*it can be NULL for geting total length*/
        SIZE_T                   z_skip_len,
        SIZE_T*                  pz_dest_len
        )
{

    register UCHAR uc_prior_char = 0;
    UINT32 ui4_byte_writer_pos = 0;
    register UINT16 ui2_base_index = 0;
    register UINT32 ui4_byte_length = z_src_len;
    register UCHAR* puc_table;
    register UCHAR tmp = 0;
    UCHAR uc_bits_char;
    INT32 i4_res = 0;
    BIT_READER_POS_T t_bitreader_pos;
    SIZE_T z_origial_dest_len;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ( ((e_huffman_type != CL_PROGRAM_DESCRIPTION_TYPE) 
                && (e_huffman_type != CL_PROGRAM_TITLE_TYPE )
                && (e_huffman_type != CL_PROGRAM_DVBT_TABLE1_TYPE)
                && (e_huffman_type != CL_PROGRAM_DVBT_TABLE2_TYPE))
        || ((pv_src_buf == NULL) || (pz_dest_len == NULL))
        || ((z_src_len != 0) && (pv_src_buf == NULL ))
        || ((*pz_dest_len != 0) && (pv_dest_buf == NULL )) 
        || (z_src_len == 0) 
        || (pv_src_buf == NULL))
    {
        return CLR_INV_ARG;
    }
    /************************************************************************/
    /* Perform decode algorithm                                             */
    /************************************************************************/

    /* set up decoding table*/
    if (e_huffman_type == CL_PROGRAM_TITLE_TYPE)
    {
        puc_table = _title_dec_table;
    }
    else if(e_huffman_type == CL_PROGRAM_DESCRIPTION_TYPE)
    {
        puc_table = _desc_dec_table;
    }
    else if (e_huffman_type == CL_PROGRAM_DVBT_TABLE1_TYPE)
    {
        puc_table = _dvbt_dec_table1;
    }
    else
    {
        puc_table = _dvbt_dec_table2;
    }
    /* back up original dest length */
    z_origial_dest_len = *pz_dest_len;
    x_memset(&t_bitreader_pos, 0, sizeof(BIT_READER_POS_T));
    /************************************************************************/
    /* Decode loop                                                          */
    /************************************************************************/

    if (((UCHAR*)pv_src_buf)[z_src_len - 1] == 0 )
    {   /*NULL terminate character, the ending character is ignored*/
        ui4_byte_length = z_src_len - 1;
        z_src_len = ui4_byte_length;
    }

    while (ui4_byte_length > t_bitreader_pos.ui4_byte_pos)
    {
        /* check is it ASCII code? */
        if (uc_prior_char&0x80)
        { /* prior char is not ASCII code, read 8 bit */
            i4_res = _cl_bit_reader(8, pv_src_buf, z_src_len, &uc_bits_char, &t_bitreader_pos);
            if (i4_res != BIT_READER_OK)
            {   /* no more data */
                if (i4_res == BIT_READER_RUN_OUT)
                {   /*end of memory boundary*/
                    break;
                }
                else
                {
                    return CLR_INV_ARG;
                }
            }
            /* write 8 bit data */
            if (_cl_byte_writer(uc_bits_char, pv_dest_buf, z_skip_len, pz_dest_len, &ui4_byte_writer_pos) != BYTE_WRITE_OK)
            {
                return CLR_INV_ARG;
            }
            /* store to prior char*/
            tmp = 0;
            uc_prior_char = uc_bits_char;
        }
        else
        {   /* it is ASCII code and find base */
            ui2_base_index = (UINT16)((puc_table[(uc_prior_char * 2)]) << 8) + puc_table[(uc_prior_char * 2 + 1)];

            while (tmp < 128)
            {   /* look up table */
                /* read 1 bit*/
                i4_res = _cl_bit_reader(1, pv_src_buf, z_src_len, &uc_bits_char, &t_bitreader_pos);
                if (i4_res != BIT_READER_OK)
                {
                    if (i4_res == BIT_READER_RUN_OUT)
                    {   /*end of memory boundary*/
                        break;
                    }
                    else
                    {
                        return CLR_INV_ARG;
                    }
                }

                tmp = puc_table[ui2_base_index + tmp*2 + uc_bits_char];
            }

            if (i4_res == BIT_READER_RUN_OUT)
            {   /*end of memory boundary*/
                break;
            }

            if (tmp == 155)/*ESC*/
            {
                i4_res = _cl_bit_reader(8, pv_src_buf, z_src_len, &uc_bits_char, &t_bitreader_pos);
                if (i4_res != BIT_READER_OK)
                {
                    if (i4_res == BIT_READER_RUN_OUT)
                    {   /*end of memory boundary*/

                        break;
                    }
                    else
                    {
                        return CLR_INV_ARG;
                    }
                }

                if (_cl_byte_writer(uc_bits_char, pv_dest_buf, z_skip_len, pz_dest_len, &ui4_byte_writer_pos) != BYTE_WRITE_OK)
                {
                    return CLR_INV_ARG;
                }
                /* store to prior char*/
                tmp = 0;
                uc_prior_char = uc_bits_char;
            }
            else /*normal char*/
            {
                uc_prior_char = (UCHAR)(tmp - 128);
                if (_cl_byte_writer(uc_prior_char, pv_dest_buf, z_skip_len, pz_dest_len, &ui4_byte_writer_pos) != BYTE_WRITE_OK)
                {
                    return CLR_INV_ARG;
                }
                tmp = 0;
            }
        }
    }
    /************************************************************************/
    /*   successful                                                         */
    /************************************************************************/

    if (ui4_byte_writer_pos > 0)
    {
        /* decompression ok */
        /* append NULL termination */
        if (_cl_byte_writer(0, pv_dest_buf, z_skip_len, pz_dest_len, &ui4_byte_writer_pos) != BYTE_WRITE_OK)
        {
            return CLR_INV_ARG;
        }

        if (ui4_byte_writer_pos > z_skip_len)
        {
            *pz_dest_len = ui4_byte_writer_pos - z_skip_len + 1; /* 0 base -> 1 base */
        }
        else
        {
            *pz_dest_len = 0;
        }
    }
    else
    {
        *pz_dest_len = 0;
    }

    if ((pv_dest_buf)
        && (*pz_dest_len > z_origial_dest_len))
    {
        return CLR_INSUFFICIENT_BUFFER;
    }
    else
    {
        return CLR_OK;
    }
}
