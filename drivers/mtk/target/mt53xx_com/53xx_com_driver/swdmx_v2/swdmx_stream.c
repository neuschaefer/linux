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
 * Copyright (c) 2010, MediaTek Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: swdmx_cmpb.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file swdmx_stream.c
 *   stream  for demxuer - internal interface
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "swdmx_stream.h"
#include "vdec_drvif.h"
#include "swdmx_debug.h"
#include "aud_drvif.h"
#include "x_util.h"
#include "x_timer.h"

#include "swdmx_drvif.h"



//-----------------------------------------------------------------------------
// Declare
//-----------------------------------------------------------------------------
static void sd_stream_release_data_pull(void *, void *);
static void sd_stream_req_data_pull(void *);
static void sd_stream_release_data_push(void *, void *);
static void sd_stream_req_data_push(void *);

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static sd_stream_in_t ins[] = {
   {SD_STREAM_PULL, sd_stream_req_data_pull, sd_stream_release_data_pull},
   {SD_STREAM_PUSH, sd_stream_req_data_push, sd_stream_release_data_push}
};



//-----------------------------------------------------------------------------
// Function
//-----------------------------------------------------------------------------

UINT8  sd_stream_eof(sd_stream_t *pt_stream)
{
    UNUSED(pt_stream);
    return pt_stream->eof;
}

UINT64 sd_stream_tell(sd_stream_t *pt_stream)
{
    UINT64 ret = SD_INVALID_OFFSET;
    ASSERT(pt_stream);
    
    if (pt_stream->seekable != SD_NO_SEEK)
    {
        if (pt_stream->read_chain_head)
        {
            ret  = (UINT32)(pt_stream->curr_block_pos) - pt_stream->read_chain_head->u.data.addr;
            ret += pt_stream->read_chain_head->u.data.fpos;
        }
        else
        {
            ret = pt_stream->fpos;
        }
    }
    return ret;
}

UINT8  sd_stream_seek(sd_stream_t *pt_stream, UINT64 offset)
{
    ASSERT(pt_stream);

    while (pt_stream->seekable != SD_NO_SEEK)
    {
        UINT64 curr;

        if (pt_stream->start_fpos > offset || pt_stream->end_fpos < offset)
        {
            break;
        }
        
        curr = sd_stream_tell(pt_stream);

        //LOG(2 ,"stream_seek  : curr offset:%d, new offset:%d\n", (UINT32)curr, (UINT32)offset);
        //Printf("stream_seek  : curr offset:%d, new offset:%d\n", (UINT32)curr, (UINT32)offset);

        if (offset > curr)
        {
            if (curr + pt_stream->read_chain_remain_size >=  offset)
            {
                sd_stream_skip(pt_stream, offset - curr);
                break;
            }
        }
        else if (offset < curr) 
        {
            if ((pt_stream->read_chain_head && 
                ((offset + pt_stream->b_chain_size) >= pt_stream->read_chain_head->u.data.fpos))
                || ((offset + pt_stream->b_chain_size) >= curr))
            {
                //todo reorder backward chain
                //break;
            }
        }
        else // offset ==  curr
        {
            break;        
        }

        //flush
        sd_stream_reset(pt_stream);
        pt_stream->fpos = offset;
        break;
    }

    return 0;
}




UINT32 sd_stream_capacity (sd_stream_t *pt_stream)
{
    ASSERT(pt_stream);
    return pt_stream->read_chain_remain_size;
}

static sd_chain_t * sd_get_unused_ack_block(sd_stream_t *pt_stream, UINT8 n)
{
    sd_chain_t *p;
    p = pt_stream->unuse_chain_head;
    if (p)
    {
        if (n && !p->next)
        {
            return NULL;
        }
        pt_stream->unuse_chain_head = p->next;
    }
    else
    {
        //to do dynamic allocate 
    }
    return p;
}

static void sd_put_to_unused_chain(sd_stream_t *pt_stream, sd_chain_t *pt_c)
{
    ASSERT(pt_stream);
    if (pt_c)
    {
        pt_c->next = pt_stream->unuse_chain_head;
        pt_stream->unuse_chain_head = pt_c;
    }
}



static void sd_stream_flush(sd_stream_t *pt_stream)
{
    sd_chain_t *pt_c;
    UINT8 i;
    ASSERT(pt_stream);

    pt_stream->b_chain_head = pt_stream->b_chain_tail = NULL;
    pt_stream->b_chain_size = 0;

    pt_stream->chain_head = pt_stream->chain_tail = NULL;
    pt_stream->chain_size = 0;

    pt_stream->read_chain_head = pt_stream->read_chain_tail = NULL;
    pt_stream->last_time_tag = SD_INVALID_TIMETAG;
    pt_stream->read_chain_total_size = 0;
    pt_stream->read_chain_remain_size = 0;
    pt_stream->curr_block_size = 0;
    pt_stream->curr_block_pos = NULL;

    pt_stream->ibc_chain_head = pt_stream->ibc_chain_tail = NULL;
    

    pt_stream->req_timeout_cnt = 0;
    pt_stream->req_cnt = 0;
    pt_stream->req_id += 1000;
        
    pt_c = &pt_stream->chain_buff[0];
    pt_stream->unuse_chain_head = pt_c;
    for (i = 1; i < SD_CHAIN_BUFF_NS;i++)
    {
        pt_c->next = &pt_stream->chain_buff[i];
        pt_c = pt_c->next;
        pt_c->next = NULL;
    }
    
}

static UINT8 sd_stream_read_char(sd_stream_t *pt_stream);
static UINT8 sd_stream_read_char(sd_stream_t *pt_stream)
{
    ASSERT(pt_stream);
    if (pt_stream->curr_block_size)
    {
        ASSERT(pt_stream->curr_block_pos);
        pt_stream->curr_block_size--;
        pt_stream->read_chain_remain_size--;
        return (*(pt_stream->curr_block_pos++));
    }
    else if (!(pt_stream->read_chain_remain_size))
    {
        return 0xFF;
    }
    else 
    {
        //move to next 
        if (pt_stream->b_chain_head)
        {
            pt_stream->b_chain_tail->next = pt_stream->read_chain_head;
            pt_stream->b_chain_tail =  pt_stream->read_chain_head;
        }
        else
        {
            pt_stream->b_chain_head = pt_stream->b_chain_tail = pt_stream->read_chain_head;
        }
        pt_stream->read_chain_total_size -= pt_stream->b_chain_tail->u.data.size;
                
        pt_stream->read_chain_head = pt_stream->read_chain_head->next;
        pt_stream->b_chain_tail->next = NULL;

        pt_stream->curr_block_pos = (char *)(pt_stream->read_chain_head->u.data.addr);
        pt_stream->curr_block_size = pt_stream->read_chain_head->u.data.size;

        return sd_stream_read_char(pt_stream);
    }
}

inline UINT32 sd_stream_read_xx_char(sd_stream_t *pt_stream, UINT8 *s, UINT32 len)
{
    UINT32 part = 0;
    UINT8 *to;
    UINT32 _len;

    ASSERT(pt_stream||s);

    len = pt_stream->read_chain_remain_size < len ? 0:len;

    _len = len;
    to = s;

    while (_len)
    {
        if (!pt_stream->curr_block_size)   //move to next block
        {
            if (pt_stream->b_chain_head)
            {
                pt_stream->b_chain_tail->next = pt_stream->read_chain_head;
                pt_stream->b_chain_tail =  pt_stream->read_chain_head;
            }
            else
            {
                pt_stream->b_chain_head = pt_stream->b_chain_tail = pt_stream->read_chain_head;
            }
            pt_stream->read_chain_total_size -= pt_stream->b_chain_tail->u.data.size;

            pt_stream->read_chain_head = pt_stream->read_chain_head->next;
            pt_stream->b_chain_tail->next = NULL;
            
            pt_stream->curr_block_pos = (char *)(pt_stream->read_chain_head->u.data.addr);
            pt_stream->curr_block_size = pt_stream->read_chain_head->u.data.size;
        }

        part = MIN(_len, pt_stream->curr_block_size);
        x_memcpy((void *)to, (const void *)pt_stream->curr_block_pos, part);

        _len -= part;
        to += part;
        pt_stream->curr_block_pos += part;
        pt_stream->curr_block_size -= part;

    }

    pt_stream->read_chain_remain_size -= len;

    return len;
}

inline UINT32 sd_stream_peek_xx_char(sd_stream_t *pt_stream, UINT8 *s, UINT32 len)
{

    UINT32 part = 0;
    UINT8 *to;
    UINT32 _len;
    sd_chain_t *p_chain;
    UINT32 curr_block_size;
    char *curr_block_pos;

    ASSERT(pt_stream||s);

    len = pt_stream->read_chain_remain_size < len ? 0:len;

    _len = len;
    to = s;
    curr_block_size = pt_stream->curr_block_size; 
    curr_block_pos = pt_stream->curr_block_pos;
    p_chain = pt_stream->read_chain_head;
    
    while (_len)
    {
        if (!curr_block_size)   //move to next block
        {
            p_chain = p_chain->next;
            curr_block_pos = (char *)(p_chain->u.data.addr);
            curr_block_size = p_chain->u.data.size;
        }

        part = MIN(_len, curr_block_size);
        x_memcpy((void *)to, (const void *)curr_block_pos, part);
        _len -= part;
        to += part;
        curr_block_pos += part;
        curr_block_size -= part;

    }
    return len;
}

UINT8 sd_stream_read_b1(sd_stream_t *pt_stream)
{
    return sd_stream_read_char(pt_stream);
}

UINT16 sd_stream_read_b2(sd_stream_t *pt_stream)
{
    UINT16 d;
    d = sd_stream_read_char(pt_stream);
    d = (d<<8)|sd_stream_read_char(pt_stream);
    return d;
}

UINT32 sd_stream_read_b3(sd_stream_t *pt_stream)
{
    UINT32 d;
    d = sd_stream_read_char(pt_stream);
    d = (d<<8)|sd_stream_read_char(pt_stream);
    d = (d<<8)|sd_stream_read_char(pt_stream);
    return d;
}

UINT32 sd_stream_read_b4(sd_stream_t *pt_stream)
{
    UINT32 d;
    d = sd_stream_read_char(pt_stream);
    d = (d<<8)|sd_stream_read_char(pt_stream);
    d = (d<<8)|sd_stream_read_char(pt_stream);
    d = (d<<8)|sd_stream_read_char(pt_stream);
    return d;
}

UINT8 sd_stream_peek_b1(sd_stream_t *pt_stream)
{
    UINT8 d = 0;
    sd_stream_peek_xx_char(pt_stream, &d, 1);
    return d;
}

UINT16 sd_stream_peek_b2(sd_stream_t *pt_stream)
{
    UINT8 d[2] ={0};
    sd_stream_peek_xx_char(pt_stream, d, 2);
    return ((((UINT16)d[0])<<8) | d[1]);
}

UINT32 sd_stream_peek_b3(sd_stream_t *pt_stream)
{
    UINT8 d[3] = {0};
    sd_stream_peek_xx_char(pt_stream, d, 3);
    return ((((UINT32)d[0])<<16) | (((UINT32)d[1])<<8) | d[2]);
}

UINT32 sd_stream_peek_b4(sd_stream_t *pt_stream)
{
    UINT8 d[4] = {0};
    sd_stream_peek_xx_char(pt_stream, d, 4);
    return ((((UINT32)d[0])<<24) | (((UINT32)d[1])<<16) | (((UINT32)d[2])<<8) | d[3]);
}

UINT8 sd_stream_read_l1(sd_stream_t *pt_stream)
{
    return sd_stream_read_char(pt_stream);
}

UINT16 sd_stream_read_l2(sd_stream_t *pt_stream)
{
    UINT16 d;
    d = sd_stream_read_char(pt_stream);
    d = d|(((UINT16)sd_stream_read_char(pt_stream))<<8);
    return d;
}

UINT32 sd_stream_read_l3(sd_stream_t *pt_stream)
{
    UINT32 d;
    d = sd_stream_read_char(pt_stream);
    d = d|(((UINT32)sd_stream_read_char(pt_stream))<<8);
    d = d|(((UINT32)sd_stream_read_char(pt_stream))<<16);
    return d;
}

UINT32 sd_stream_read_l4(sd_stream_t *pt_stream)
{
    UINT32 d;
    d = sd_stream_read_char(pt_stream);
    d = d|(((UINT32)sd_stream_read_char(pt_stream))<<8);
    d = d|(((UINT32)sd_stream_read_char(pt_stream))<<16);
    d = d|(((UINT32)sd_stream_read_char(pt_stream))<<24);
    return d;
}

UINT8 sd_stream_peek_l1(sd_stream_t *pt_stream)
{
    UINT8 d = 0;
    sd_stream_peek_xx_char(pt_stream, &d, 1);
    return d;
}

UINT16 sd_stream_peek_l2(sd_stream_t *pt_stream)
{
    UINT8 d[2] ={0};
    sd_stream_peek_xx_char(pt_stream, d, 2);
    return ((((UINT16)d[1])<<8) | d[0]);
}

UINT32 sd_stream_peek_l3(sd_stream_t *pt_stream)
{
    UINT8 d[3] = {0};
    sd_stream_peek_xx_char(pt_stream, d, 3);
    return ((((UINT32)d[2])<<16) | (((UINT32)d[1])<<8) | d[0]);
}

UINT32 sd_stream_peek_l4(sd_stream_t *pt_stream)
{
    UINT8 d[4] = {0};
    sd_stream_peek_xx_char(pt_stream, d, 4);
    return ((((UINT32)d[3])<<24) | (((UINT32)d[2])<<16) | (((UINT32)d[1])<<8) | d[0]);
}

UINT32 sd_stream_read_xx(sd_stream_t *pt_stream, UINT8 *s, UINT32 len)
{
    return  sd_stream_read_xx_char(pt_stream, s, len);
}

UINT32 sd_stream_peek_xx(sd_stream_t *pt_stream, UINT8 *s, UINT32 len)
{
    return  sd_stream_peek_xx_char(pt_stream, s, len);
}

UINT8 sd_stream_move_data(sd_stream_t *pt_stream, UINT8 type, UINT32 len, void *pv_data)
{
    DMX_MM_DATA_T dmx_data;
    UINT32 _len;
    UINT32 part = 0;
    SWDMX_INFO_T * swdmx_inst;

    ASSERT(pt_stream);

    swdmx_inst = (SWDMX_INFO_T *)pt_stream->pv_host;

    if (pt_stream->read_chain_remain_size < len)
    {
        return 0;
    }

    switch(type)
    {
    case SD_STREAM_MOVE_VIDEO:
    case SD_STREAM_MOVE_AUDIO:
	case SD_STREAM_MOVE_TS:	
        ASSERT(pv_data);
        x_memcpy((void *)&dmx_data, pv_data, sizeof(DMX_MM_DATA_T));
        break;
    default:
        break;
    }

    _len = len;

    while (_len)
    {
        if (!pt_stream->curr_block_size)   //move to next block
        {
            if (pt_stream->b_chain_head)
            {
                pt_stream->b_chain_tail->next = pt_stream->read_chain_head;
                pt_stream->b_chain_tail =  pt_stream->read_chain_head;
            }
            else
            {
                pt_stream->b_chain_head = pt_stream->b_chain_tail = pt_stream->read_chain_head;
            }
            pt_stream->read_chain_total_size -= pt_stream->b_chain_tail->u.data.size;

            pt_stream->read_chain_head = pt_stream->read_chain_head->next;
            pt_stream->b_chain_tail->next = NULL;
            
            pt_stream->curr_block_pos = (char *)(pt_stream->read_chain_head->u.data.addr);
            pt_stream->curr_block_size = pt_stream->read_chain_head->u.data.size;
        }

        part = MIN(_len, pt_stream->curr_block_size);
    
        dmx_data.u4BufStart  = (pt_stream->read_chain_head->u.data.addr & 0xFFFFFFF0);
        dmx_data.u4BufEnd    = (pt_stream->read_chain_head->u.data.addr + pt_stream->read_chain_head->u.data.size + 0x10+188)&0xFFFFFFF0;
        dmx_data.u4StartAddr = (UINT32)(pt_stream->curr_block_pos);
        dmx_data.u4FrameSize = part;

        _SWDMX_MoveData(swdmx_inst->u1SwdmxId, &dmx_data);

        _len -= part;

        pt_stream->curr_block_pos += part;
        pt_stream->curr_block_size -= part;
    }

    pt_stream->read_chain_remain_size -= len;

    return TRUE;
}


//skip
UINT8  sd_stream_skip(sd_stream_t *pt_stream, UINT64 len)
{
    ASSERT(pt_stream);
    if ((UINT64)pt_stream->read_chain_remain_size >= len)
    {
        UINT32 part = 0;
        UINT32 _len;
        _len = len;
        while (_len)
        {
            if (!pt_stream->curr_block_size)   //move to next block
            {
                if (pt_stream->b_chain_head)
                {
                    pt_stream->b_chain_tail->next = pt_stream->read_chain_head;
                    pt_stream->b_chain_tail =  pt_stream->read_chain_head;
                }
                else
                {
                    pt_stream->b_chain_head = pt_stream->b_chain_tail = pt_stream->read_chain_head;
                }

                pt_stream->read_chain_total_size -= pt_stream->b_chain_tail->u.data.size;
                        
                pt_stream->read_chain_head = pt_stream->read_chain_head->next;
                pt_stream->b_chain_tail->next = NULL;
                pt_stream->curr_block_pos = (char *)(pt_stream->read_chain_head->u.data.addr);
                pt_stream->curr_block_size = pt_stream->read_chain_head->u.data.size;
            }

            part = MIN(_len, pt_stream->curr_block_size);
            _len -= part;
            pt_stream->curr_block_pos += part;
            pt_stream->curr_block_size -= part;
        }

        pt_stream->read_chain_remain_size -= len;

    }
    else
    {
        sd_stream_skip(pt_stream, pt_stream->read_chain_remain_size);
        pt_stream->cmd_data.skip_len = len - pt_stream->read_chain_remain_size;
        pt_stream->cmd = SD_STREAM_CMD_SKIPDATA;
    }

    return 0;
}

UINT8 sd_stream_srch_4cc_mask(sd_stream_t *pt_stream, UINT32 srch_depth, sd_stream_srch_pm *pat, UINT32 n)
{
    #define MIN_SIZE_SRCH_4CC 4
    UINT32 i;
    UINT32 _len;

    UINT32 value;
    UINT8  ret = SD_STREAM_SRCH_NOMATCH;

    ASSERT(pt_stream || pat);

    if (pt_stream->read_chain_remain_size < MIN_SIZE_SRCH_4CC)
    {
        return ret;
    }

    _len = MIN(srch_depth + MIN_SIZE_SRCH_4CC, pt_stream->read_chain_remain_size);
    srch_depth = _len - MIN_SIZE_SRCH_4CC;
    
    //read first 4 bytes
    value = sd_stream_peek_c4(pt_stream);
    
    do 
    {
        //test patten
        for (i = 0; i < n; i++)
        {
            if ((value & pat[i].mask) == pat[i].patten)
            {
                //match 
                ret = SD_STREAM_SRCH_MATCH;
                goto srch_end;
            }
        }

        if (_len <= MIN_SIZE_SRCH_4CC) goto srch_end;

        //read next byte
        if (!pt_stream->curr_block_size)   //move to next block
        {
            if (pt_stream->b_chain_head)
            {
                pt_stream->b_chain_tail->next = pt_stream->read_chain_head;
                pt_stream->b_chain_tail =  pt_stream->read_chain_head;
            }
            else
            {
                pt_stream->b_chain_head = pt_stream->b_chain_tail = pt_stream->read_chain_head;
            }
            pt_stream->read_chain_total_size -= pt_stream->b_chain_tail->u.data.size;

            pt_stream->read_chain_head = pt_stream->read_chain_head->next;
            pt_stream->b_chain_tail->next = NULL;
            
            pt_stream->curr_block_pos = (char *)(pt_stream->read_chain_head->u.data.addr);
            pt_stream->curr_block_size = pt_stream->read_chain_head->u.data.size;
        }
        _len--;
        pt_stream->curr_block_pos ++;
        pt_stream->curr_block_size --;
        value = (value << 8) | (*(UINT8 *)pt_stream->curr_block_pos);

    }while (1);

srch_end:

    pt_stream->read_chain_remain_size -= srch_depth;
    return ret;
}

static UINT8 sd_stream_srch_4cc_func(sd_stream_t *pt_stream, UINT32 srch_depth, fp_4cc_test fp_test)
{
    #define MIN_SIZE_SRCH_4CC 4
    UINT32 len;
    UINT32 value;
    UINT8  ret = SD_STREAM_SRCH_NOMATCH;
    sd_chain_t *p_chain;
    UINT32 curr_block_size;
    char *curr_block_pos;

    ASSERT(pt_stream || fp_test);

    if (pt_stream->read_chain_remain_size < MIN_SIZE_SRCH_4CC)
    {
        return ret;
    }

    len = MIN(srch_depth + MIN_SIZE_SRCH_4CC, pt_stream->read_chain_remain_size);
    srch_depth = len - MIN_SIZE_SRCH_4CC;
    
    curr_block_size = pt_stream->curr_block_size; 
    curr_block_pos = pt_stream->curr_block_pos;
    p_chain = pt_stream->read_chain_head;
    value = 0;
    while (len)
    {
        if (!curr_block_size)   //move to next block
        {
            p_chain = p_chain->next;
            curr_block_pos = (char *)(p_chain->u.data.addr);
            curr_block_size = p_chain->u.data.size;
        }
        value <<= 8;
        value |= (*(UINT8 *)curr_block_pos);
        len--;
        curr_block_pos ++;
        curr_block_size --;

        if (len <= srch_depth && fp_test(value))
        {
            //match 
            ret = SD_STREAM_SRCH_MATCH;
            break;
        }
    }

    srch_depth -= len;
    
    if (srch_depth > 0) sd_stream_skip(pt_stream, srch_depth);

    return ret;
}

UINT8 sd_stream_search_4cc(sd_stream_t *pt_stream, UINT32 srch_depth, fp_4cc_test fp_test)
{
    return sd_stream_srch_4cc_func(pt_stream, srch_depth, fp_test);
}


UINT8 sd_stream_search(sd_stream_t *pt_stream, ...)
{
    return SD_STREAM_SRCH_NOMATCH;
}

static sd_stream_t * sd_get_instance(void)
{
    //to do multi-instance, use instance pool 
    static sd_stream_t sd_instance; 
    return &sd_instance;
}

void sd_stream_control(sd_stream_t *pt_stream, UINT16 cmd, UINT32 arg1)
{
    ASSERT(pt_stream);
    switch(cmd)
    {
    case SD_STREAM_CTRL_RESET:
    {
        //flush feeder
        UINT16 u2Cnt;
        UINT16 u2MsgQIdx;
        SIZE_T zMsgSize;
        INT32 i4Ret;
        FEEDER_REQ_DATA_T rData;
        SWDMX_INFO_T * swdmx_inst = (SWDMX_INFO_T *)pt_stream->pv_host;

        zMsgSize = sizeof(FEEDER_REQ_DATA_T);
        do 
        {
            x_msg_q_receive(&u2MsgQIdx, &rData, &zMsgSize,
                                           &(swdmx_inst->hDmxFeederReqDataQ), 1, X_MSGQ_OPTION_NOWAIT);

            i4Ret = x_msg_q_num_msgs(swdmx_inst->hDmxFeederReqDataQ, &u2Cnt);
        
        } while (u2Cnt != 0 && i4Ret == OSR_OK);

        FeederSetRing(swdmx_inst->eFeederSrc,TRUE);

        //flush stream chain 
        sd_stream_flush(pt_stream);

        //
        pt_stream->eof = !SD_STREAM_EOF;
        
        //reset cmd
        pt_stream->cmd = SD_STREAM_CMD_NONE;
        //LOG(2 ,"SD_STREAM_CTRL_RESET\n");
    }
        break;
    case SD_STREAM_CTRL_INPUT_MODE:
    {
        UINT8 i;
        UINT8 input_mode = (UINT8)arg1;
        pt_stream->in = &ins[0];
        for (i = 0; i < sizeof(ins)/sizeof(ins[0]); i++)
        {
            if (input_mode == ins[i].type)
            {
               pt_stream->in = &ins[i];
            }
        }
    }
        break;
    default:
        break;
    }

}


sd_stream_t * sd_stream_open(UINT8 mode)
{
    sd_stream_t *pt;
    pt = sd_get_instance();
    ASSERT(pt);
    if (pt)
    {
        x_memset((void *)pt, 0, sizeof(sd_stream_t));

        sd_stream_flush(pt);

        //default settings
        pt->cmd          = SD_STREAM_CMD_NONE;
        pt->req_mode     = SD_STREAM_REQ_SINGLE;
        pt->req_max_size = SD_STREAM_REQ_SIZE;
        pt->eof          = !SD_STREAM_EOF;
        pt->seekable     = SD_NO_SEEK;
        pt->stream_type  = SD_STREAM_TYPE_RING;

        sd_stream_control(pt, SD_STREAM_CTRL_INPUT_MODE, SD_STREAM_PULL);
    }
    return pt;
}

void sd_stream_close(sd_stream_t *pt_stream)
{
    //to do put instance into pool,release dynamic memory
    ASSERT(pt_stream);
}



static void sd_insert_data_chain(sd_stream_t *pt_stream, sd_chain_t * pt_c)
{
    ASSERT(pt_c && pt_stream);
    if (!pt_stream->chain_head)
    {
       pt_stream->chain_head =  pt_stream->chain_tail = pt_c;
    }
    else
    {
        if (pt_stream->chain_tail->u.data.time_tag < pt_c->u.data.time_tag)
        {
            pt_stream->chain_tail->next  = pt_c;
            pt_stream->chain_tail = pt_c;
        }
        else if (pt_stream->chain_head->u.data.time_tag > pt_c->u.data.time_tag)
        {
            pt_c->next = pt_stream->chain_head;
            pt_stream->chain_head = pt_c;
        }
        else 
        {
            sd_chain_t *p = pt_stream->chain_head;
            while(p->next)
            {
                if ((p->u.data.time_tag < pt_c->u.data.time_tag)&& 
                    (p->next->u.data.time_tag > pt_c->u.data.time_tag))
                {
                    pt_c->next= p->next;
                    p->next = pt_c;
                }
            }
        }
    }
                
}

static void sd_remove_data_chain(sd_stream_t *pt_stream, sd_chain_t * pt_c)
{
    sd_chain_t *p =NULL;
    ASSERT(pt_c && pt_stream);
	if(pt_stream==NULL)
	{
       return;
	}
	
    p= pt_stream->chain_head;
    if (p && (p == pt_c))
    {
        pt_stream->chain_head = p->next;
        if (!pt_stream->chain_head) pt_stream->chain_tail = pt_stream->chain_head;
    }
    else
    {
        while(p && p->next)
        {
            if (p->next == pt_c)
            {
                p->next = p->next->next;
                if (pt_c == pt_stream->chain_tail) pt_stream->chain_tail = p; 
                break;
            }
            p = p->next;
        }
    }
}

static void sd_insert_ibc_chain(sd_stream_t *pt_stream, sd_chain_t * pt_c)
{
    ASSERT(pt_c && pt_stream);
	if(pt_stream==NULL)
	{
       return;
	}
    if (!pt_stream->ibc_chain_head)
    {
        pt_stream->ibc_chain_head =  pt_stream->ibc_chain_tail = pt_c;
    }
    else
    {
        pt_stream->ibc_chain_tail->next = pt_c;
        pt_stream->ibc_chain_tail = pt_c;
    }
                
}

void sd_remove_ibc_chain(sd_stream_t *pt_stream, sd_chain_t * pt_c)
{
    sd_chain_t *p = pt_stream->ibc_chain_head;
    ASSERT(pt_c && pt_stream);

    if (p && (p == pt_c))
    {
        pt_stream->ibc_chain_head = p->next;
        if (!pt_stream->ibc_chain_head) pt_stream->ibc_chain_tail = pt_stream->ibc_chain_head;
    }
    else
    {
        while(p && p->next)
        {
            if (p->next == pt_c)
            {
                p->next = p->next->next;
                if (pt_c == pt_stream->ibc_chain_tail) pt_stream->ibc_chain_tail = p; 
                break;
            }
            p = p->next;
        }
    }
}



static void sd_stream_release_data(sd_stream_t *pt_stream, sd_chain_t * pt_c)
{
    pt_stream->in->rel_data((void*)pt_stream, (void*)pt_c);
}

static void sd_stream_req_data(sd_stream_t *pt_stream)
{
    pt_stream->in->req_data((void*)pt_stream);
}

static void sd_stream_release_data_pull(void *pv_stream, void *pv_c)
{
    sd_stream_t *pt_stream = (sd_stream_t *)pv_stream;
    sd_chain_t  *pt_c      = (sd_chain_t *)pv_c;
    SWDMX_INFO_T * swdmx_inst = (SWDMX_INFO_T *)pt_stream->pv_host;

    if (pt_c)
    {
        pt_c->next = pt_stream->unuse_chain_head;
        pt_stream->unuse_chain_head = pt_c;

        //update data    
        // Update read ptr to feeder
        FeederUpdateReadPointer(swdmx_inst->eFeederSrc, VIRTUAL(pt_c->u.data.addr + pt_c->u.data.size));
    }
}


static void sd_stream_req_data_pull(void *pv_stream)
{
    sd_stream_t *pt_stream = (sd_stream_t *)pv_stream;
    ASSERT(pt_stream);

    switch(pt_stream->req_mode)
    {
    case SD_STREAM_REQ_DEFAULT:
    case SD_STREAM_REQ_SINGLE:
    default:
    {
        SWDMX_INFO_T * swdmx_inst = (SWDMX_INFO_T *)pt_stream->pv_host;

        //issue request
        if (!pt_stream->req_cnt)
        {
            do
            {
                INT32 ret;
                FEEDER_TRANSMIT_COND rFeederCond;
                UINT32 u4ReadSize;
                //check buffer size
                
                //set request size
                if (pt_stream->eof == SD_STREAM_EOF)
                {
                    break;
                }

                if (pt_stream->fpos >= pt_stream->end_fpos)
                {
                    pt_stream->eof = SD_STREAM_EOF;
                    break;
                }
                
                u4ReadSize = pt_stream->req_max_size;
                if (pt_stream->fpos + u4ReadSize > pt_stream->end_fpos)
                {
                    u4ReadSize = pt_stream->end_fpos - pt_stream->fpos;
                }
                ret = FeederSetRequest(swdmx_inst->eFeederSrc,
                    u4ReadSize,
                    pt_stream->fpos,
                    &rFeederCond,
                    pt_stream->req_id);
                
                if (FEEDER_E_OK != ret)
                {
                    break;
                }

                LOG(2 ,"send req  : id:%d, size:%d, file pos:%d\n", pt_stream->req_id, u4ReadSize, (UINT32)pt_stream->fpos);
                //Printf("send req  : id:%d, size:%d, file pos:%d\n", pt_stream->req_id, u4ReadSize, (UINT32)pt_stream->fpos);

                pt_stream->req_id ++;
                pt_stream->req_cnt ++;
                pt_stream->req_timeout_cnt = 0;
                
            } while(0);
        }

        //receive data        
        if (pt_stream->req_cnt)
        {
            sd_chain_t * pt_c;

            do
            {
                UINT16 u2MsgQIdx;
                SIZE_T zMsgSize;
                INT32 i4Ret;
                FEEDER_REQ_DATA_T rData;
                UINT32 u4ReadSize;

                pt_c = sd_get_unused_ack_block(pt_stream, 1);

                if (!pt_c)
                {
                    break;
                }
                    
                zMsgSize = sizeof(FEEDER_REQ_DATA_T);
                i4Ret = x_msg_q_receive_timeout(&u2MsgQIdx, &rData, &zMsgSize,
                        &(swdmx_inst->hDmxFeederReqDataQ), 1, SWDMX_FEEDER_TIMEOUT_HIGH_SPEED);
            
                if (i4Ret == OSR_TIMEOUT)
                {
                    pt_stream->req_timeout_cnt++;
                    break;
                }
                else if (i4Ret != OSR_OK)
                {
                    break;
                }

                //check 
                if (rData.eDataType == FEEDER_SOURCE_INVALID)
                {
                    break;
                }

                if (!rData.u4WriteAddr)
                {
                    break;
                }

                if (rData.u4Id + 1 != pt_stream->req_id)
                {
                    break;
                }

                //ASSERT(rData.u4WriteAddr);
                u4ReadSize = pt_stream->end_fpos - pt_stream->fpos;
                u4ReadSize = MIN(rData.u4ReadSize - rData.u4AlignOffset, u4ReadSize);
                
                pt_c->u.data.fpos = pt_stream->fpos;
                pt_c->u.data.addr = VIRTUAL(rData.u4WriteAddr + rData.u4AlignOffset);
                pt_c->u.data.size = u4ReadSize;//rData.u4ReadSize - rData.u4AlignOffset;
                pt_c->u.data.time_tag = rData.u4Id;
                pt_c->next = NULL;
                sd_insert_data_chain(pt_stream, pt_c);
                
                LOG(2, "get  ack  : id:%d, size:%d, file pos:%d\n", pt_c->u.data.time_tag, pt_c->u.data.size, (UINT32)pt_c->u.data.fpos);
                //Printf("get  ack  : id:%d, size:%d, file pos:%d\n", pt_c->u.data.time_tag, pt_c->u.data.size, (UINT32)pt_c->u.data.fpos);

                pt_stream->fpos += pt_c->u.data.size;

                if (pt_c->u.data.addr + pt_c->u.data.size > pt_stream->priv.u.ring.end_addr)
                {
                    sd_chain_t * pt_d;
                    pt_c->u.data.size = pt_stream->priv.u.ring.end_addr - pt_c->u.data.addr;

                    LOG(2, "get  ack-r: id:%d, size:%d, file pos:%d\n", pt_c->u.data.time_tag, pt_c->u.data.size, (UINT32)pt_c->u.data.fpos);
                    
                    pt_d = sd_get_unused_ack_block(pt_stream, 0);
                    
                    pt_d->u.data.fpos = pt_c->u.data.fpos + pt_c->u.data.size;
                    pt_d->u.data.addr = pt_stream->priv.u.ring.start_addr;
                    pt_d->u.data.size = u4ReadSize - pt_c->u.data.size; //rData.u4ReadSize - rData.u4AlignOffset - pt_c->u.data.size;
                    pt_d->u.data.time_tag = pt_c->u.data.time_tag + 1;
                    pt_d->next = NULL;
                    sd_insert_data_chain(pt_stream, pt_d);

                    LOG(2, "get  ack  : id:%d, size:%d, file pos:%d\n", pt_d->u.data.time_tag, pt_d->u.data.size, (UINT32)pt_d->u.data.fpos);

                    pt_stream->req_id ++;
                }
                
                pt_stream->req_cnt--;
                pt_c = NULL;

            }while (0);

            if (pt_c) sd_put_to_unused_chain(pt_stream, pt_c);

            if (pt_stream->req_timeout_cnt > SD_STREAM_REQ_TIMEOUT)
            {
                //to do:
                LOG(0, "warning!!! request time out. line %d @ %s \n", __LINE__, __FILE__);
            }
            
        }
    }
        break;
    case SD_STREAM_REQ_MULTI:
        break;
        
    }

    
}

static void sd_stream_release_data_push(void *pv_stream, void * pv_c)
{
    sd_stream_t *pt_stream = (sd_stream_t *)pv_stream;
    sd_chain_t  *pt_c      = (sd_chain_t *)pv_c;

    SWDMX_INFO_T * swdmx_inst = (SWDMX_INFO_T *)pt_stream->pv_host;
    if (pt_c)
    {
        pt_c->next = pt_stream->unuse_chain_head;
        pt_stream->unuse_chain_head = pt_c;

        //update data    
        // Update read ptr to feeder
        FeederUpdateReadPointer(swdmx_inst->eFeederSrc, 0);
    }
}

static void sd_stream_req_data_push(void *pv_stream)
{
    sd_stream_t *pt_stream = (sd_stream_t *)pv_stream;

    ASSERT(pt_stream);

    switch(pt_stream->req_mode)
    {
    case SD_STREAM_REQ_DEFAULT:
    case SD_STREAM_REQ_SINGLE:
    default:
    {
        SWDMX_INFO_T * swdmx_inst = (SWDMX_INFO_T *)pt_stream->pv_host;

        //issue request
        if (!pt_stream->req_cnt)
        {
            do
            {
                INT32 ret;
                FEEDER_TRANSMIT_COND rFeederCond;
                //check buffer size
                
                ret = FeederSetRequest(swdmx_inst->eFeederSrc,
                    1,
                    0,
                    &rFeederCond,
                    pt_stream->req_id);
                
                if (FEEDER_E_OK != ret)
                {
                    break;
                }

                LOG(2 ,"send req<push>  : id:%d\n", pt_stream->req_id);

                pt_stream->req_id ++;
                pt_stream->req_cnt ++;
                pt_stream->req_timeout_cnt = 0;
                
            } while(0);
        }

        //receive data        
        if (pt_stream->req_cnt)
        {
            sd_chain_t * pt_c;

            do
            {
                UINT16 u2MsgQIdx;
                SIZE_T zMsgSize;
                INT32 i4Ret;
                FEEDER_REQ_DATA_T rData;
    
                pt_c = sd_get_unused_ack_block(pt_stream, 1);

                if (!pt_c)
                {
                    break;
                }

                if (pt_stream->eof == SD_STREAM_EOF)
                {
                    UINT16 num_msgs = 0;
                    x_msg_q_num_msgs(swdmx_inst->hDmxFeederReqDataQ, &num_msgs);
                    if (!num_msgs) break;
                }
                    
                zMsgSize = sizeof(FEEDER_REQ_DATA_T);
                i4Ret = x_msg_q_receive_timeout(&u2MsgQIdx, &rData, &zMsgSize,
                        &(swdmx_inst->hDmxFeederReqDataQ), 1, SWDMX_FEEDER_TIMEOUT_HIGH_SPEED);
            
                if (i4Ret == OSR_TIMEOUT)
                {
                    pt_stream->req_timeout_cnt++;
                    break;
                }
                else if (i4Ret != OSR_OK)
                {
                    break;
                }

                //check 
                if (rData.eDataType == FEEDER_SOURCE_INVALID)
                {
                    break;
                }

                if (rData.fgEof)
                {
                    pt_c->u.ibc.type = SD_IBC_EOS;
                    pt_c->u.ibc.time_tag = rData.u4Id;
                    pt_c->next = NULL;
                    sd_insert_ibc_chain(pt_stream, pt_c);
                    break;
                }

                if (!rData.u4WriteAddr)
                {
                    break;
                }
                //ASSERT(rData.u4WriteAddr);
                pt_c->u.data.fpos = SD_INVALID_OFFSET;
                pt_c->u.data.addr = VIRTUAL(rData.u4WriteAddr);
                pt_c->u.data.size = rData.u4ReadSize;
                pt_c->u.data.time_tag = rData.u4Id;
                pt_c->next = NULL;
                sd_insert_data_chain(pt_stream, pt_c);
                
                LOG(2, "get  ack<push>  : id:%d, size:%d, addr:%d\n", pt_c->u.data.time_tag, pt_c->u.data.size, pt_c->u.data.addr);
               
                pt_stream->req_cnt--;
                pt_c = NULL;

            }while (0);

            if (pt_c) sd_put_to_unused_chain(pt_stream, pt_c);

            if (pt_stream->req_timeout_cnt > SD_STREAM_REQ_TIMEOUT)
            {
                //to do:
            }
            
        }
    }
        break;
    case SD_STREAM_REQ_MULTI:
        break;
        
    }

    
}



static void sd_stream_data_chain(sd_stream_t *pt_stream)
{
    sd_chain_t *p, *q  = NULL;
    ASSERT(pt_stream);

    //move to read chain
    p = pt_stream->chain_head;
    while(p)
    {
        q = p->next;
        if (!pt_stream->read_chain_head)
        {
            if (pt_stream->last_time_tag == SD_INVALID_TIMETAG)
            {
                sd_remove_data_chain(pt_stream, p);
                pt_stream->read_chain_head = pt_stream->read_chain_tail = p;
                pt_stream->read_chain_total_size  += p->u.data.size;
                pt_stream->read_chain_remain_size += p->u.data.size;
                pt_stream->curr_block_size =  p->u.data.size;
                pt_stream->curr_block_pos = (char *)(p->u.data.addr);
                pt_stream->last_time_tag = p->u.data.time_tag;
                p->next = NULL;
            }
            else
            {
                //broken to do later
            }
        }
        else
        {
            ASSERT(pt_stream->read_chain_tail);

            if (p->u.data.time_tag !=  pt_stream->read_chain_tail->u.data.time_tag + 1)
            {
                break;
            }
            sd_remove_data_chain(pt_stream, p);

            pt_stream->read_chain_tail->next = p;
            p->next = NULL;
            pt_stream->read_chain_tail = p;
            pt_stream->read_chain_total_size  += p->u.data.size;
            pt_stream->read_chain_remain_size += p->u.data.size;
            pt_stream->last_time_tag = p->u.data.time_tag;
        }

        p = q;
        
    }

    //backwards chain 
    while(pt_stream->b_chain_head)
    {
        p = pt_stream->b_chain_head;
        pt_stream->b_chain_head = pt_stream->b_chain_head->next;
        sd_stream_release_data(pt_stream, p);
    }
    if (!pt_stream->b_chain_head) pt_stream->b_chain_tail = NULL;
    
}

static void sd_stream_ibc_cmd(sd_stream_t *pt_stream)
{
    ASSERT(pt_stream);

    //move to read chain
    while (pt_stream->ibc_chain_head)
    {
        if(SD_IBC_EOS == pt_stream->ibc_chain_head->u.ibc.type)
        {
            sd_chain_t  *pt_c;
            pt_stream->eof = 1;
            pt_c = pt_stream->ibc_chain_head;
            pt_stream->ibc_chain_head = pt_stream->ibc_chain_head->next;
            pt_c->next = pt_stream->unuse_chain_head;
            pt_stream->unuse_chain_head = pt_c;
            break;
        }
        else 
        {
            return;
        }
    }

    if (!pt_stream->b_chain_head) pt_stream->b_chain_tail = NULL;
}


static void sd_stream_proc_cmd(sd_stream_t *pt_stream)
{
    ASSERT(pt_stream);

    //IBC CMD
    if (pt_stream->ibc_chain_head)
    {
        sd_stream_ibc_cmd(pt_stream);
    }

    //internel cmd
    switch(pt_stream->cmd)
    {
    case SD_STREAM_CMD_SKIPDATA:
    {
        UINT64 _len;
        _len = MIN(pt_stream->cmd_data.skip_len, pt_stream->read_chain_remain_size);
        sd_stream_skip(pt_stream, _len);
        pt_stream->cmd_data.skip_len -= _len;

        if (pt_stream->cmd_data.skip_len == 0)
        {
            pt_stream->cmd = SD_STREAM_CMD_NONE;
        }
    }
        break;
    default:
        break;
    }
}

void sd_stream_state(sd_stream_t *pt_stream)
{
    //ASSERT(pt_stream);
    if (!pt_stream)
    {
        return;
    }
        
    switch(pt_stream->state)
    {
    case SD_STREAM_ST_RUN:
        sd_stream_req_data(pt_stream);
        sd_stream_data_chain(pt_stream);
        break;
    default:
        break;
    };

    sd_stream_proc_cmd(pt_stream);
}


void sd_stream_reset(sd_stream_t *pt_stream)
{
    sd_stream_control(pt_stream, SD_STREAM_CTRL_RESET, 0);
}

void sd_stream_set_input_mode(sd_stream_t *pt_stream, UINT32 mode)
{
    sd_stream_control(pt_stream, SD_STREAM_CTRL_INPUT_MODE, mode);
}

inline UINT8 sd_stream_fullness(sd_stream_t *pt_stream)
{
    if (!pt_stream->unuse_chain_head ||
        !(pt_stream->unuse_chain_head->next))
    {
        return TRUE;
    }
    return FALSE;
}

// auto test function
void sd_stream_test_case(void)
{

}




