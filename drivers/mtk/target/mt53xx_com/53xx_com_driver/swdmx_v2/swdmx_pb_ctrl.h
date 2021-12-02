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
 * $RCSfile: swdmx_pb_ctrl.h, $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file swdmx_pb_ctrl.h
 */

#ifndef SWDMX_PB_CTRL_H
#define SWDMX_PB_CTRL_H

#include "x_common.h"
#include "x_typedef.h"
#include "x_timer.h"
#include "x_assert.h"
#include "swdmx_demuxer.h"

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
typedef enum 
{
    INIT_S = 0,
    BUFFERING_S,
    PLAYING_S,
    PAUSE_S,
    STOP_S,
    FORWARD_S,
    BACKWARD_S,
    END_S,
    ERROR_S,
} ctrl_state;


/*
 * A rate above 1000 plays the movie faster,
 * A rate below 1000 plays the movie slower.
 */
#define DEFAULT_SPEED                    1000         /*Normal speed*/
#define MIN_FORWARD_SPEED                (1000>>5)    /* Up to 1/32 */
#define MAX_FORWARD_SPEED                32000        /* Up to 32 */
#define MIN_BACKWARD_SPEED               (-2000)      /* Up to 32 */
#define MAX_BACKWARD_SPEED               (-32000)     /* Up to 32 */



#define IS_IDX_BASED_MODE(_speed)         ((_speed) > 2000 || (_speed) < 0)
#define IS_FORWARD(_speed)                ((_speed) > 0)
#define IS_STEP(_speed)                   ((_speed) == 1)




/*
playback cmd
 */
#define SD_PB_CMD_NONE               0
#define SD_PB_CMD_CHG_AUDIO          1
#define SD_PB_CMD_CHG_SUBTITLE       2
#define SD_PB_CMD_CHG_SPEED          3
#define SD_PB_CMD_SEEK_TIME          4

#define SD_PB_CMD_IDLE               0 
#define SD_PB_CMD_DOING              1

#define SD_PB_CMD_LV1                1
#define SD_PB_CMD_LV2                2


#define SD_PB_IDX_BASE_ST_SEND       1
#define SD_PB_IDX_BASE_ST_SENDING    2
#define SD_PB_IDX_BASE_ST_DONE       3

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef struct
{
    ctrl_state state;
    UINT32     arg1;
    INT32      speed;
} sd_pb_state_t;

typedef struct
{
    UINT16 cmd;
    UINT8  state;
    UINT8  priority;
    UINT32 arg1;
    UINT32 arg2;
    UINT32 arg3;
} sd_pb_cmd_t;

typedef struct
{
    sd_pb_state_t last_s;
    sd_pb_state_t curr_s;
    sd_pb_cmd_t   cmd;
    void *pv_host;

    //video info
    UINT32 video_fps;    
    

    //step op
    UINT32 step_cnt;

    //index table based playback mode
    UINT32 repeat_last_pack_no;      
    UINT32 repeat_st;      
    UINT32 repeat_cnt;      
    UINT32 curr_idx;      
    MM_RANGE_ELMT_IDXTBL_T *curr_idx_tbl;

    UINT32 next_idx;      
    MM_RANGE_ELMT_IDXTBL_T *next_idx_tbl;

} sd_pb_ctrl_t;


typedef struct
{
    UINT32 pts;      //in &out
    UINT16 sup_flag; //in
    UINT16 pos;      //out
    MM_RANGE_ELMT_IDXTBL_T *idx_tbl; //out
} sd_pb_idx_entry_t;


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
void swdmx_change_state(sd_pb_ctrl_t *pt_this, ctrl_state new_state);

//-----------------------------------------------------------------------------
// Macro functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
BOOL _SWDMX_EnableStrmX(UINT8 u1SrcId, UINT8 u1StrmType, VOID* pfnCbFunc);
BOOL _SWDMX_DisableStrmX(UINT8 u1SrcId, UINT8 u1StrmType);
BOOL _SWDMX_StopX(UINT8 u1SrcId);
BOOL _SWDMX_GetInfoX(UINT8 u1SrcId,UINT8 u1Type,UINT32* pu4Para1,UINT32* pu4Para2,UINT32* pu4Para3);
BOOL _SWDMX_SetSpeedX(UINT8 u1SrcId, INT32 i4Speed);
BOOL _SWDMX_SeekTimeX(UINT8 u1SrcId, UINT32 u4SeekTime, UINT64* pu8FilePos);
BOOL _SWDMX_SeekPosX(UINT8 u1SrcId, UINT64 u8SeekPos, UINT64 u8SeekPosI);
BOOL _SWDMX_SeekPercentX(UINT8 u1SrcId, UINT32 u4SeekPercent, UINT64* pu8FilePos);
BOOL _SWDMX_SeekOffsetX(UINT8 u1SrcId, VOID *prPosition);
BOOL _SWDMX_AudioCbX(UINT8 u1SrcId, ENUM_SWDMX_AUDIO_NOTIFY_TYPE_T eAudioNotifyType);


#endif  // SWDMX_PB_CTRL_H


