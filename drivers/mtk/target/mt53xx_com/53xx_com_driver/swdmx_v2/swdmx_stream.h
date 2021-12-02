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
 * Copyright(c) 2010, MediaTek, Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: swdmx_stream.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file swdmx_stream.h
 *  This header file declares exported APIs of SWDMX_STREAM.
 */

#ifndef SWDMX_STREAM_H
#define SWDMX_STREAM_H


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_common.h"
#include "x_typedef.h"
#include "x_timer.h"
#include "x_assert.h"


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define SD_INVALID_OFFSET       (0xFFFFFFFFFFFFFFFF)
#define SD_STREAM_EOF           (1)
#define SD_CHAIN_BUFF_NS        (32)
#define SD_STREAM_REQ_SIZE      (282*1024)
#define SD_STREAM_REQ_TIMEOUT   (20)
#define SD_INVALID_TIMETAG      (0xFFFFFFFF)
#define SD_FAST_SEEK            (1)
#define SD_SLOW_SEEK            (2)
#define SD_NO_SEEK              (3)


//define for stream control
#define SD_STREAM_CTRL_NONE  0
#define SD_STREAM_CTRL_RESET 1
#define SD_STREAM_CTRL_INPUT_MODE 2



//define for stream mode
#define SD_STREAM_MODE_XX    0


//define for stream type
#define SD_STREAM_TYPE_RING  1


//define for stream input mode
#define SD_STREAM_PUSH    1
#define SD_STREAM_PULL    2



//define for move data type
#define SD_STREAM_MOVE_VIDEO  1
#define SD_STREAM_MOVE_AUDIO  2
#define SD_STREAM_MOVE_TS     3


#define SD_STREAM_SRCH_MATCH    0
#define SD_STREAM_SRCH_NOMATCH  1


#define SD_STREAM_SRCH_MODE_4CC_FUNC 0
#define SD_STREAM_SRCH_MODE_4CC_MASK 1


#define SD_IBC_EOS   1


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef struct {
    UINT32 addr;
    UINT32 size;
    UINT32 time_tag;
    UINT64 fpos;      // for seekable stream
}sd_data_ack_t;

typedef struct {
    UINT32 type;
    UINT32 param;
    UINT32 time_tag;
}sd_ibc_t;


typedef struct _sd_chain_t{
    union
    {
        sd_data_ack_t data;
        sd_ibc_t      ibc;
    }u;
    struct _sd_chain_t *next;
}sd_chain_t;
  

typedef struct _sd_buff_t{
    char *buff;
    struct _sd_buff_t *next;
}sd_buff_t;

typedef struct _sd_cmd_t{
    UINT64 skip_len;
}sd_cmd_t;



typedef enum
{
    SD_STREAM_ST_IDLE = 0,
    SD_STREAM_ST_RUN,

    SD_STREAM_ST_MAX
} sd_stream_state_t;

typedef enum
{
    SD_STREAM_REQ_DEFAULT = 0,
    SD_STREAM_REQ_MULTI,
    SD_STREAM_REQ_SINGLE,
    SD_STREAM_REQ_MAX
} sd_stream_req_mode_t;

typedef enum
{
    SD_STREAM_CMD_NONE = 0,
    SD_STREAM_CMD_SKIPDATA,
    SD_STREAM_CMD_MAX
} sd_stream_cmd_t;


typedef struct {
   UINT32 start_addr; 
   UINT32 end_addr; 
} sd_stream_ring_t;


typedef struct {
    union
    {
        sd_stream_ring_t ring;
    }u;
} sd_stream_priv_t;


typedef void (*fp_release_data)(void *, void *);
typedef void (*fp_request_data)(void *);


typedef struct {
    UINT8 type;
    fp_request_data req_data;
    fp_release_data rel_data;
} sd_stream_in_t;


//swdmx stream
typedef struct {
    UINT8  type;
    UINT8  seekable;
    UINT8  eof;
    UINT64 fpos;
    UINT64 start_fpos;
    UINT64 end_fpos;


    //stream control 
    sd_stream_state_t state;
    sd_stream_req_mode_t req_mode;
    UINT16 req_timeout_cnt;
    UINT16 req_cnt;
    UINT32 req_id;
    UINT32 req_max_size;

    sd_stream_cmd_t cmd;
    sd_cmd_t        cmd_data;


    // read chain
    sd_chain_t *read_chain_head;
    sd_chain_t *read_chain_tail;
    UINT32      last_time_tag;          // if tail is null, use it
    UINT32      read_chain_total_size;
    UINT32      read_chain_remain_size;

    UINT32      curr_block_size;
    char *      curr_block_pos;
    

    // chain 
    sd_chain_t *chain_head;
    sd_chain_t *chain_tail;
    UINT32      chain_size;

    // backwards buffer chain
    sd_chain_t *b_chain_head;
    sd_chain_t *b_chain_tail;
    UINT32      b_chain_size;

    //IBC chain
    sd_chain_t *ibc_chain_head;
    sd_chain_t *ibc_chain_tail;

    // unused ack block
    sd_chain_t *unuse_chain_head;


    //BUFF
    sd_chain_t chain_buff[SD_CHAIN_BUFF_NS];
    sd_buff_t a_chain_buff;


    //swdmx instance pointer
    void * pv_host;
    
    //private data for special stream type
    UINT8 stream_type;
    sd_stream_priv_t priv;

    //input mode: push or pull
    sd_stream_in_t *in;

} sd_stream_t;



//search pattern
typedef struct {
    UINT32 patten;
    UINT32 mask;
} sd_stream_srch_pm; // patten with mask

typedef BOOL (*fp_4cc_test)(UINT32);


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
//position 
UINT8  sd_stream_eof(sd_stream_t *pt_stream);
UINT64 sd_stream_tell(sd_stream_t *pt_stream);
UINT8  sd_stream_seek(sd_stream_t *pt_stream, UINT64 offset);

//how many bytes in the stream
UINT32 sd_stream_capacity (sd_stream_t *pt_stream);

//big endian
UINT8  sd_stream_read_b1(sd_stream_t *pt_stream);
UINT16 sd_stream_read_b2(sd_stream_t *pt_stream);
UINT32 sd_stream_read_b3(sd_stream_t *pt_stream);
UINT32 sd_stream_read_b4(sd_stream_t *pt_stream);
UINT8  sd_stream_peek_b1(sd_stream_t *pt_stream);
UINT16 sd_stream_peek_b2(sd_stream_t *pt_stream);
UINT32 sd_stream_peek_b3(sd_stream_t *pt_stream);
UINT32 sd_stream_peek_b4(sd_stream_t *pt_stream);

//little endian
UINT8  sd_stream_read_l1(sd_stream_t *pt_stream);
UINT16 sd_stream_read_l2(sd_stream_t *pt_stream);
UINT32 sd_stream_read_l3(sd_stream_t *pt_stream);
UINT32 sd_stream_read_l4(sd_stream_t *pt_stream);
UINT8  sd_stream_peek_l1(sd_stream_t *pt_stream);
UINT16 sd_stream_peek_l2(sd_stream_t *pt_stream);
UINT32 sd_stream_peek_l3(sd_stream_t *pt_stream);
UINT32 sd_stream_peek_l4(sd_stream_t *pt_stream);

//char 
#define sd_stream_read_c1(pt_stream) sd_stream_read_b1(pt_stream)
#define sd_stream_read_c2(pt_stream) sd_stream_read_b2(pt_stream)
#define sd_stream_read_c3(pt_stream) sd_stream_read_b3(pt_stream)
#define sd_stream_read_c4(pt_stream) sd_stream_read_b4(pt_stream)
#define sd_stream_peek_c1(pt_stream) sd_stream_peek_b1(pt_stream)
#define sd_stream_peek_c2(pt_stream) sd_stream_peek_b2(pt_stream)
#define sd_stream_peek_c3(pt_stream) sd_stream_peek_b3(pt_stream)
#define sd_stream_peek_c4(pt_stream) sd_stream_peek_b4(pt_stream)

//any bytes 
UINT32 sd_stream_read_xx(sd_stream_t *pt_stream, UINT8 *s, UINT32 len);
UINT32 sd_stream_peek_xx(sd_stream_t *pt_stream, UINT8 *s, UINT32 len);

//skip
UINT8  sd_stream_skip(sd_stream_t *pt_stream, UINT64 len);


sd_stream_t * sd_stream_open(UINT8 mode);
void sd_stream_close(sd_stream_t *pt_stream);

void sd_stream_control(sd_stream_t *pt_stream, UINT16 cmd, UINT32 arg1);
void sd_stream_state(sd_stream_t *pt_stream);
UINT8 sd_stream_move_data(sd_stream_t *pt_stream, UINT8 type, UINT32 len, void *pv_data);
UINT8 sd_stream_search_4cc(sd_stream_t *pt_stream, UINT32 srch_depth, fp_4cc_test fp_test);
UINT8 sd_stream_search(sd_stream_t *pt_stream, ...);


void sd_stream_reset(sd_stream_t *pt_stream);
void sd_stream_set_input_mode(sd_stream_t *pt_stream, UINT32 mode);

inline UINT8 sd_stream_fullness(sd_stream_t *pt_stream);

//Macor



#endif  // SWDMX_STREAM_H


