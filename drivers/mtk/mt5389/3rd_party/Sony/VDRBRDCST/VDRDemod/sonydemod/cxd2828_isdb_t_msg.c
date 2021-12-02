/*------------------------------------------------------------------------------

 Copyright 2010 Sony Corporation

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Sony Corporation.
 No part of this file may be copied, modified, sold, and distributed in any
 form or by any means without prior explicit permission in writing from
 Sony Corporation.

 Date: 2010/05/27
 Revision: 1.0.0.1

------------------------------------------------------------------------------*/
#include "cxd2828_common.h"
#include "cxd2828_isdb_t.h"
#include "cxd2828_isdb_t_msg.h"

static cxd2828_isdb_t_mode_t_msg_t cxd2828_isdb_t_mode_t_msg_table[] =
{
    {CXD2828_ISDB_T_MODE_1,       "Mode=1"},
    {CXD2828_ISDB_T_MODE_2,       "Mode=2"},
    {CXD2828_ISDB_T_MODE_3,       "Mode=3"},
    {CXD2828_ISDB_T_MODE_UNKNOWN, "Mode=Unknown"}
};

static cxd2828_isdb_t_guard_t_msg_t cxd2828_isdb_t_guard_t_msg_table[] =
{
    {CXD2828_ISDB_T_GUARD_1_32,    "GI=1/32"},
    {CXD2828_ISDB_T_GUARD_1_16,    "GI=1/16"},
    {CXD2828_ISDB_T_GUARD_1_8,     "GI=1/8"},
    {CXD2828_ISDB_T_GUARD_1_4,     "GI=1/4"},
    {CXD2828_ISDB_T_GUARD_UNKNOWN, "GI=Unknown"}
};

static cxd2828_isdb_t_tmcc_system_t_msg_t cxd2828_isdb_t_tmcc_system_t_msg_table[] =
{
    {CXD2828_ISDB_T_TMCC_SYSTEM_ISDB_T,    "System=ISDB-T"},
    {CXD2828_ISDB_T_TMCC_SYSTEM_ISDB_TSB,  "System=ISDB-Tsb"},
    {CXD2828_ISDB_T_TMCC_SYSTEM_RESERVE_2, "System=Reserved(2)"},
    {CXD2828_ISDB_T_TMCC_SYSTEM_RESERVE_3, "System=Reserved(3)"}
};

static cxd2828_isdb_t_tmcc_modulation_t_msg_t cxd2828_isdb_t_tmcc_modulation_t_msg_table[] =
{
    {CXD2828_ISDB_T_TMCC_MODULATION_DQPSK,         "Modulation=DQPSK"},
    {CXD2828_ISDB_T_TMCC_MODULATION_QPSK,          "Modulation=QPSK"},
    {CXD2828_ISDB_T_TMCC_MODULATION_16QAM,         "Modulation=16QAM"},
    {CXD2828_ISDB_T_TMCC_MODULATION_64QAM,         "Modulation=64QAM"},
    {CXD2828_ISDB_T_TMCC_MODULATION_RESERVED_4,    "Modulation=Reserved(4)"},
    {CXD2828_ISDB_T_TMCC_MODULATION_RESERVED_5,    "Modulation=Reserved(5)"},
    {CXD2828_ISDB_T_TMCC_MODULATION_RESERVED_6,    "Modulation=Reserved(6)"},
    {CXD2828_ISDB_T_TMCC_MODULATION_UNUSED_7,      "Modukation=Unused(7)"}
};

static cxd2828_isdb_t_tmcc_coding_rate_t_msg_t cxd2828_isdb_t_tmcc_coding_rate_t_msg_table[] =
{
    {CXD2828_ISDB_T_TMCC_CODING_RATE_1_2,          "CodingRate=1/2"},
    {CXD2828_ISDB_T_TMCC_CODING_RATE_2_3,          "CodingRate=2/3"},
    {CXD2828_ISDB_T_TMCC_CODING_RATE_3_4,          "CodingRate=3/4"},
    {CXD2828_ISDB_T_TMCC_CODING_RATE_5_6,          "CodingRate=5/6"},
    {CXD2828_ISDB_T_TMCC_CODING_RATE_7_8,          "CodingRate=7/8"},
    {CXD2828_ISDB_T_TMCC_CODING_RATE_RESERVED_5,   "CodingRate=Reserved(5)"},
    {CXD2828_ISDB_T_TMCC_CODING_RATE_RESERVED_6,   "CodingRate=Reserved(6)"},
    {CXD2828_ISDB_T_TMCC_CODING_RATE_UNUSED_7,     "CodingRate=Unused(7)"}
};

static cxd2828_isdb_t_tmcc_il_length_t_msg_t cxd2828_isdb_t_tmcc_il_length_t_msg_table[] =
{
    {CXD2828_ISDB_T_TMCC_IL_LENGTH_0_0_0,      "InterleavingLength=0(Mode1),0(Mode2),0(Mode3)"},
    {CXD2828_ISDB_T_TMCC_IL_LENGTH_4_2_1,      "InterleavingLength=4(Mode1),2(Mode2),1(Mode3)"},
    {CXD2828_ISDB_T_TMCC_IL_LENGTH_8_4_2,      "InterleavingLength=8(Mode1),4(Mode2),2(Mode3)"},
    {CXD2828_ISDB_T_TMCC_IL_LENGTH_16_8_4,     "InterleavingLength=16(Mode1),8(Mode2),4(Mode3)"},
    {CXD2828_ISDB_T_TMCC_IL_LENGTH_RESERVED_4, "InterleavingLength=Reserved(4)"},
    {CXD2828_ISDB_T_TMCC_IL_LENGTH_RESERVED_5, "InterleavingLength=Reserved(5)"},
    {CXD2828_ISDB_T_TMCC_IL_LENGTH_RESERVED_6, "InterleavingLength=Reserved(6)"},
    {CXD2828_ISDB_T_TMCC_IL_LENGTH_UNUSED_7,   "InterleavingLength=Unused(7)"}
};

static char cxd2828_isdb_t_msg_error[] = "Unknown value";

char* cxd2828_isdb_t_mode_t2msg(cxd2828_isdb_t_mode_t value)
{
    UINT8 index = 0;
    for(index = 0; index < (sizeof(cxd2828_isdb_t_mode_t_msg_table)/sizeof(cxd2828_isdb_t_mode_t_msg_table[0])); index++){
        if(cxd2828_isdb_t_mode_t_msg_table[index].value == value){
            break;
        }
    }
    if(index >= (sizeof(cxd2828_isdb_t_mode_t_msg_table)/sizeof(cxd2828_isdb_t_mode_t_msg_table[0]))){
        return cxd2828_isdb_t_msg_error;
    } else {
        return cxd2828_isdb_t_mode_t_msg_table[index].string;
    }
}

char* cxd2828_isdb_t_guard_t2msg(cxd2828_isdb_t_guard_t value)
{
    UINT8 index = 0;
    for(index = 0; index < (sizeof(cxd2828_isdb_t_guard_t_msg_table)/sizeof(cxd2828_isdb_t_guard_t_msg_table[0])); index++){
        if(cxd2828_isdb_t_guard_t_msg_table[index].value == value){
            break;
        }
    }
    if(index >= (sizeof(cxd2828_isdb_t_guard_t_msg_table)/sizeof(cxd2828_isdb_t_guard_t_msg_table[0]))){
        return cxd2828_isdb_t_msg_error;
    } else {
        return cxd2828_isdb_t_guard_t_msg_table[index].string;
    }
}

char* cxd2828_isdb_t_tmcc_system_t2msg(cxd2828_isdb_t_tmcc_system_t value)
{
    UINT8 index = 0;
    for(index = 0; index < (sizeof(cxd2828_isdb_t_tmcc_system_t_msg_table)/sizeof(cxd2828_isdb_t_tmcc_system_t_msg_table[0])); index++){
        if(cxd2828_isdb_t_tmcc_system_t_msg_table[index].value == value){
            break;
        }
    }
    if(index >= (sizeof(cxd2828_isdb_t_tmcc_system_t_msg_table)/sizeof(cxd2828_isdb_t_tmcc_system_t_msg_table[0]))){
        return cxd2828_isdb_t_msg_error;
    } else {
        return cxd2828_isdb_t_tmcc_system_t_msg_table[index].string;
    }
}

char* cxd2828_isdb_t_tmcc_modulation_t2msg(cxd2828_isdb_t_tmcc_modulation_t value)
{
    UINT8 index = 0;
    for(index = 0; index < (sizeof(cxd2828_isdb_t_tmcc_modulation_t_msg_table)/sizeof(cxd2828_isdb_t_tmcc_modulation_t_msg_table[0])); index++){
        if(cxd2828_isdb_t_tmcc_modulation_t_msg_table[index].value == value){
            break;
        }
    }
    if(index >= (sizeof(cxd2828_isdb_t_tmcc_modulation_t_msg_table)/sizeof(cxd2828_isdb_t_tmcc_modulation_t_msg_table[0]))){
        return cxd2828_isdb_t_msg_error;
    } else {
        return cxd2828_isdb_t_tmcc_modulation_t_msg_table[index].string;
    }
}

char* cxd2828_isdb_t_tmcc_coding_rate_t2msg(cxd2828_isdb_t_tmcc_coding_rate_t value)
{
    UINT8 index = 0;
    for(index = 0; index < (sizeof(cxd2828_isdb_t_tmcc_coding_rate_t_msg_table)/sizeof(cxd2828_isdb_t_tmcc_coding_rate_t_msg_table[0])); index++){
        if(cxd2828_isdb_t_tmcc_coding_rate_t_msg_table[index].value == value){
            break;
        }
    }
    if(index >= (sizeof(cxd2828_isdb_t_tmcc_coding_rate_t_msg_table)/sizeof(cxd2828_isdb_t_tmcc_coding_rate_t_msg_table[0]))){
        return cxd2828_isdb_t_msg_error;
    } else {
        return cxd2828_isdb_t_tmcc_coding_rate_t_msg_table[index].string;
    }
}

char* cxd2828_isdb_t_tmcc_il_length_t2msg(cxd2828_isdb_t_tmcc_il_length_t value)
{
    UINT8 index = 0;
    for(index = 0; index < (sizeof(cxd2828_isdb_t_tmcc_il_length_t_msg_table)/sizeof(cxd2828_isdb_t_tmcc_il_length_t_msg_table[0])); index++){
        if(cxd2828_isdb_t_tmcc_il_length_t_msg_table[index].value == value){
            break;
        }
    }
    if(index >= (sizeof(cxd2828_isdb_t_tmcc_il_length_t_msg_table)/sizeof(cxd2828_isdb_t_tmcc_il_length_t_msg_table[0]))){
        return cxd2828_isdb_t_msg_error;
    } else {
        return cxd2828_isdb_t_tmcc_il_length_t_msg_table[index].string;
    }
}
