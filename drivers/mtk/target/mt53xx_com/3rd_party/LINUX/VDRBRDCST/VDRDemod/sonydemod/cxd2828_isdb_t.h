/*------------------------------------------------------------------------------

 Copyright 2010 Sony Corporation

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Sony Corporation.
 No part of this file may be copied, modified, sold, and distributed in any
 form or by any means without prior explicit permission in writing from
 Sony Corporation.

 Date: 2010/03/19
 Revision: 1.0.0.0

------------------------------------------------------------------------------*/
#ifndef CXD2828_ISDB_T_H
#define CXD2828_ISDB_T_H

#include "cxd2828_common.h"
#include "cxd2828_i2c.h"

typedef enum cxd2828_isdb_t_mode_t{
    CXD2828_ISDB_T_MODE_1,
    CXD2828_ISDB_T_MODE_2,
    CXD2828_ISDB_T_MODE_3,
    CXD2828_ISDB_T_MODE_UNKNOWN
}cxd2828_isdb_t_mode_t;

typedef enum cxd2828_isdb_t_guard_t{
    CXD2828_ISDB_T_GUARD_1_32, /* GI = 1/32 */
    CXD2828_ISDB_T_GUARD_1_16, /* GI = 1/16 */
    CXD2828_ISDB_T_GUARD_1_8,  /* GI = 1/8  */
    CXD2828_ISDB_T_GUARD_1_4,  /* GI = 1/4  */
    CXD2828_ISDB_T_GUARD_UNKNOWN
}cxd2828_isdb_t_guard_t;


/* ===== TMCC Information ===== */

typedef enum cxd2828_isdb_t_tmcc_system_t{
    CXD2828_ISDB_T_TMCC_SYSTEM_ISDB_T,
    CXD2828_ISDB_T_TMCC_SYSTEM_ISDB_TSB,
    CXD2828_ISDB_T_TMCC_SYSTEM_RESERVE_2,
    CXD2828_ISDB_T_TMCC_SYSTEM_RESERVE_3
}cxd2828_isdb_t_tmcc_system_t;

typedef enum cxd2828_isdb_t_tmcc_modulation_t{
    CXD2828_ISDB_T_TMCC_MODULATION_DQPSK,
    CXD2828_ISDB_T_TMCC_MODULATION_QPSK,
    CXD2828_ISDB_T_TMCC_MODULATION_16QAM,
    CXD2828_ISDB_T_TMCC_MODULATION_64QAM,
    CXD2828_ISDB_T_TMCC_MODULATION_RESERVED_4,
    CXD2828_ISDB_T_TMCC_MODULATION_RESERVED_5,
    CXD2828_ISDB_T_TMCC_MODULATION_RESERVED_6,
    CXD2828_ISDB_T_TMCC_MODULATION_UNUSED_7
}cxd2828_isdb_t_tmcc_modulation_t;

typedef enum cxd2828_isdb_t_tmcc_coding_rate_t{
    CXD2828_ISDB_T_TMCC_CODING_RATE_1_2,        /* 1/2 */
    CXD2828_ISDB_T_TMCC_CODING_RATE_2_3,        /* 2/3 */
    CXD2828_ISDB_T_TMCC_CODING_RATE_3_4,        /* 3/4 */
    CXD2828_ISDB_T_TMCC_CODING_RATE_5_6,        /* 5/6 */
    CXD2828_ISDB_T_TMCC_CODING_RATE_7_8,        /* 7/8 */
    CXD2828_ISDB_T_TMCC_CODING_RATE_RESERVED_5,
    CXD2828_ISDB_T_TMCC_CODING_RATE_RESERVED_6,
    CXD2828_ISDB_T_TMCC_CODING_RATE_UNUSED_7
}cxd2828_isdb_t_tmcc_coding_rate_t;

typedef enum cxd2828_isdb_t_tmcc_il_length_t{
    CXD2828_ISDB_T_TMCC_IL_LENGTH_0_0_0,      /* Mode1: 0, Mode2: 0, Mode3: 0 */
    CXD2828_ISDB_T_TMCC_IL_LENGTH_4_2_1,      /* Mode1: 4, Mode2: 2, Mode3: 1 */
    CXD2828_ISDB_T_TMCC_IL_LENGTH_8_4_2,      /* Mode1: 8, Mode2: 4, Mode3: 2 */
    CXD2828_ISDB_T_TMCC_IL_LENGTH_16_8_4,     /* Mode1:16, Mode2: 8, Mode3: 4 */
    CXD2828_ISDB_T_TMCC_IL_LENGTH_RESERVED_4,
    CXD2828_ISDB_T_TMCC_IL_LENGTH_RESERVED_5,
    CXD2828_ISDB_T_TMCC_IL_LENGTH_RESERVED_6,
    CXD2828_ISDB_T_TMCC_IL_LENGTH_UNUSED_7
}cxd2828_isdb_t_tmcc_il_length_t;

typedef struct {
    cxd2828_isdb_t_tmcc_modulation_t  modulation; /* Modulation */
    cxd2828_isdb_t_tmcc_coding_rate_t codingRate; /* Coding rate */
    cxd2828_isdb_t_tmcc_il_length_t   ilLength;   /* Interleaving length */
    UINT8 segmentsNum; /* 1-13:Seguments number, 0,14:Reserved, 15:Unused */
}cxd2828_isdb_t_tmcc_layer_info_t;

typedef struct {
    UINT8 isPartial; /* 0:No partial reception, 1:Partial receiption available */
    cxd2828_isdb_t_tmcc_layer_info_t layerA;
    cxd2828_isdb_t_tmcc_layer_info_t layerB;
    cxd2828_isdb_t_tmcc_layer_info_t layerC;
}cxd2828_isdb_t_tmcc_group_param_t;

typedef struct {
    cxd2828_isdb_t_tmcc_system_t      system;
    UINT8                        countDownIndex; // 4bit 0x0F:normal, 0x0E - 0x00:(X+1)frames prior to switching
    UINT8                        ewsFlag;        // 1bit 0:Not start EWS, 1:Start EWS 
    cxd2828_isdb_t_tmcc_group_param_t next;
    cxd2828_isdb_t_tmcc_group_param_t  xcurrent;
}cxd2828_isdb_t_tmcc_info_t;

#endif /* CXD2828_ISDB_T_H */

