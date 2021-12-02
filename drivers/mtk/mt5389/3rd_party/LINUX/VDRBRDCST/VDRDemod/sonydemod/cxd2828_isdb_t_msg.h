/*------------------------------------------------------------------------------

 Copyright 2010 Sony Corporation

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Sony Corporation.
 No part of this file may be copied, modified, sold, and distributed in any
 form or by any means without prior explicit permission in writing from
 Sony Corporation.

 Date: 2010/03/19
 Revision: 1.0.0.0

------------------------------------------------------------------------------*/
#ifndef CXD2828_ISDB_T_MSG_H
#define CXD2828_ISDB_T_MSG_H

#include "cxd2828_common.h"
#include "cxd2828_isdb_t.h"

#define CXD2828_ISDB_T_MSG_LENGTH 50

typedef struct {
    cxd2828_isdb_t_mode_t value;
    char               string[CXD2828_ISDB_T_MSG_LENGTH];
} cxd2828_isdb_t_mode_t_msg_t;

typedef struct {
    cxd2828_isdb_t_guard_t value;
    char                string[CXD2828_ISDB_T_MSG_LENGTH];
} cxd2828_isdb_t_guard_t_msg_t;

typedef struct {
    cxd2828_isdb_t_tmcc_system_t value;
    char                      string[CXD2828_ISDB_T_MSG_LENGTH];
} cxd2828_isdb_t_tmcc_system_t_msg_t;

typedef struct {
    cxd2828_isdb_t_tmcc_modulation_t value;
    char                      string[CXD2828_ISDB_T_MSG_LENGTH];
} cxd2828_isdb_t_tmcc_modulation_t_msg_t;

typedef struct {
    cxd2828_isdb_t_tmcc_coding_rate_t value;
    char                           string[CXD2828_ISDB_T_MSG_LENGTH];
} cxd2828_isdb_t_tmcc_coding_rate_t_msg_t;

typedef struct {
    cxd2828_isdb_t_tmcc_il_length_t value;
    char                         string[CXD2828_ISDB_T_MSG_LENGTH];
} cxd2828_isdb_t_tmcc_il_length_t_msg_t;

char* cxd2828_isdb_t_mode_t2msg(cxd2828_isdb_t_mode_t value);
char* cxd2828_isdb_t_guard_t2msg(cxd2828_isdb_t_guard_t value);
char* cxd2828_isdb_t_tmcc_system_t2msg(cxd2828_isdb_t_tmcc_system_t value);
char* cxd2828_isdb_t_tmcc_modulation_t2msg(cxd2828_isdb_t_tmcc_modulation_t value);
char* cxd2828_isdb_t_tmcc_coding_rate_t2msg(cxd2828_isdb_t_tmcc_coding_rate_t value);
char* cxd2828_isdb_t_tmcc_il_length_t2msg(cxd2828_isdb_t_tmcc_il_length_t value);

#endif /* CXD2828_ISDB_T_MSG_H */
