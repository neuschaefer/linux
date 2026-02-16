/*
 *
 * csl_atheros8328n.c
 * Description:
 * atheros8328n driver csl layer
 *
 * Copyright (c) 2007-2009 Atheros Communications, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer,
 *    without modification.
 * 2. Redistributions in binary form must reproduce at minimum a disclaimer
 *    similar to the "NO WARRANTY" disclaimer below ("Disclaimer") and any
 *    redistribution must be conditioned upon including a substantially
 *    similar Disclaimer requirement for further binary redistribution.
 * 3. Neither the names of the above-listed copyright holders nor the names
 *    of any contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") version 2 as published by the Free
 * Software Foundation.
 *
 * NO WARRANTY
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF NONINFRINGEMENT, MERCHANTIBILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGES.
 *
 * Includes Intel Corporation's changes/modifications dated: May 2011.
 * Changed/modified portions - Copyright © 2011, Intel Corporation.
 *
 */



#include "_tistdtypes.h"
#include "pal_defs.h"
#include "ddc_cpgmac_f_Drv.h"

#include "cpswhalcommon_stddef.h"
#include "cpswhalcommon_miimdio.h"

#include "ddc_switch.h"
#include "csl_switchPrvt.h"
#include "ddc_cpgmac_f_Drv.h"

#include "csl_atheros8328N.h"
#include "csl_atheros8328N_private.h"
#include "csl_atheros8328N_regs.h"

PUBLIC  PHY_DEVICE * pSwitchInfo;

/**************************************************** VLAN submodule **************************************/

#define FAL_NEXT_ENTRY_FIRST_ID 0xffffffff

/**
@brief This enum defines packets transmitted out vlan tagged mode.
*/
typedef enum {
    FAL_EG_UNMODIFIED = 0,  /**<  egress transmit packets unmodified */
    FAL_EG_UNTAGGED,        /**<  egress transmit packets without vlan tag*/
    FAL_EG_TAGGED,          /**<  egress transmit packets with vlan tag     */
    FAL_EG_HYBRID,          /**<  egress transmit packets in hybrid tag mode     */
    FAL_EG_UNTOUCHED,
    FAL_EG_MODE_BUTT
} fal_pt_1q_egmode_t;


/*******************************************************************************************************/
/*                                                                                                     */
/*                                                                                                     */
/*                                                                                                     */
/*                                                                                                     */
/*                                                                                                     */
/*******************************************************************************************************/

#define MAX_VLAN_ID         4095

#define VLAN_FLUSH          1
#define VLAN_LOAD_ENTRY     2
#define VLAN_PURGE_ENTRY    3
#define VLAN_REMOVE_PORT    4
#define VLAN_NEXT_ENTRY     5
#define VLAN_FIND_ENTRY     6

static void
_cslAtheros8328N_vlan_hw_to_sw(Uint32 reg[], CSL_SWITCH_VLAN_ENTRY * vlan_entry)
{
    Uint32 i, data, tmp;

    memset(vlan_entry, 0, sizeof (CSL_SWITCH_VLAN_ENTRY));

    SW_GET_FIELD_BY_REG(VLAN_TABLE_FUNC1, VLAN_ID, data, reg[1]);
    vlan_entry->vid = data & 0xfff;

    SW_GET_FIELD_BY_REG(VLAN_TABLE_FUNC0, IVL_EN, data, reg[0]);
    if (1 == data) {
        vlan_entry->fid = vlan_entry->vid;
    } else {
        vlan_entry->fid = 0xFFFF;
    }

    SW_GET_FIELD_BY_REG(VLAN_TABLE_FUNC0, LEARN_DIS, data, reg[0]);
    if (1 == data) {
        vlan_entry->learn_dis = True;
    } else {
        vlan_entry->learn_dis = False;
    }

    SW_GET_FIELD_BY_REG(VLAN_TABLE_FUNC0, VT_PRI_EN, data, reg[0]);
    if (1 == data) {
        vlan_entry->vid_pri_en = True;

        SW_GET_FIELD_BY_REG(VLAN_TABLE_FUNC0, VT_PRI, data, reg[0]);
        vlan_entry->vid_pri = data & 0xff;
    } else {
        vlan_entry->vid_pri_en = False;
    }

    SW_GET_FIELD_BY_REG(VLAN_TABLE_FUNC0, VID_MEM, data, reg[0]);
    for (i = 0; i < 7; i++) {
        tmp = (data >> (i << 1)) & 0x3UL;
        if (0 == tmp) {
            vlan_entry->mem_ports |= (0x1UL << i);
            vlan_entry->unmodify_ports |= (0x1UL << i);
        } else if (1 == tmp) {
            vlan_entry->mem_ports |= (0x1UL << i);
            vlan_entry->untagged_ports |= (0x1UL << i);
        } else if (2 == tmp) {
            vlan_entry->mem_ports |= (0x1UL << i);
            vlan_entry->tagged_ports |= (0x1UL << i);
        }
    }

    return;
}

static Int32
_cslAtheros8328N_vlan_sw_to_hw(Uint32 dev_id, const CSL_SWITCH_VLAN_ENTRY * vlan_entry,
                    Uint32 reg[])
{
    Uint32 i, tag, untag, unmodify, member = 0;

    if (vlan_entry->vid > MAX_VLAN_ID) {
        return SW_OUT_OF_RANGE;
    }

//    if (False ==
//        hsl_mports_prop_check(dev_id, vlan_entry->mem_ports, HSL_PP_INCL_CPU)) {
//        return SW_BAD_PARAM;
//    }

    SW_SET_REG_BY_FIELD(VLAN_TABLE_FUNC0, VT_VALID, 1, reg[0]);

    if (0xFFFF == vlan_entry->fid) {
        SW_SET_REG_BY_FIELD(VLAN_TABLE_FUNC0, IVL_EN, 0, reg[0]);
    } else if (vlan_entry->vid == vlan_entry->fid) {
        SW_SET_REG_BY_FIELD(VLAN_TABLE_FUNC0, IVL_EN, 1, reg[0]);
    } else {
        return SW_BAD_VALUE;
    }

    if (True == vlan_entry->learn_dis) {
        SW_SET_REG_BY_FIELD(VLAN_TABLE_FUNC0, LEARN_DIS, 1, reg[0]);
    } else {
        SW_SET_REG_BY_FIELD(VLAN_TABLE_FUNC0, LEARN_DIS, 0, reg[0]);
    }

    for (i = 0; i < 7; i++) {
        if ((vlan_entry->mem_ports >> i) & 0x1UL) {
            tag = (vlan_entry->tagged_ports >> i) & 0x1UL;
            untag = (vlan_entry->untagged_ports >> i) & 0x1UL;
            unmodify = (vlan_entry->unmodify_ports >> i) & 0x1UL;

            if ((0 == (tag + untag + unmodify))
                || (1 < (tag + untag + unmodify))) {
                return SW_BAD_VALUE;
            }

            if (tag) {
                member |= (2 << (i << 1));
            } else if (untag) {
                member |= (1 << (i << 1));
            }
        } else {
            member |= (3 << (i << 1));
        }
    }
    SW_SET_REG_BY_FIELD(VLAN_TABLE_FUNC0, VID_MEM, member, reg[0]);

    if (True == vlan_entry->vid_pri_en) {
        SW_SET_REG_BY_FIELD(VLAN_TABLE_FUNC0, VT_PRI_EN, 1, reg[0]);
        SW_SET_REG_BY_FIELD(VLAN_TABLE_FUNC0, VT_PRI, vlan_entry->vid_pri,
                            reg[0]);
    } else {
        SW_SET_REG_BY_FIELD(VLAN_TABLE_FUNC0, VT_PRI_EN, 0, reg[0]);
    }

    SW_SET_REG_BY_FIELD(VLAN_TABLE_FUNC1, VLAN_ID, vlan_entry->vid, reg[1]);

    return SW_OK;
}

static Int32
_cslAtheros8328N_vlan_down_to_hw(Uint32 dev_id, Uint32 reg[])
{
    Int32 rv;

    HSL_REG_ENTRY_SET(rv, dev_id, VLAN_TABLE_FUNC0, 0,
                      (Uint8 *) (&reg[0]), sizeof (Uint32));
    SW_RTN_ON_ERROR(rv);

    HSL_REG_ENTRY_SET(rv, dev_id, VLAN_TABLE_FUNC1, 0,
                      (Uint8 *) (&reg[1]), sizeof (Uint32));
    return rv;
}

static Int32
_cslAtheros8328N_vlan_up_to_sw(Uint32 dev_id, Uint32 reg[])
{
    Int32 rv;

    HSL_REG_ENTRY_GET(rv, dev_id, VLAN_TABLE_FUNC0, 0,
                      (Uint8 *) (&reg[0]), sizeof (Uint32));
    SW_RTN_ON_ERROR(rv);

    HSL_REG_ENTRY_GET(rv, dev_id, VLAN_TABLE_FUNC1, 0,
                      (Uint8 *) (&reg[1]), sizeof (Uint32));
    return rv;
}

static Int32
_cslAtheros8328N_vlan_commit(Uint32 dev_id, Uint32 op)
{
    Uint32 vt_busy = 1, i = 0x1000, vt_full, val;
    Int32 rv;

    while (vt_busy && --i) {
        HSL_REG_FIELD_GET(rv, dev_id, VLAN_TABLE_FUNC1, 0, VT_BUSY,
                          (Uint8 *) (&vt_busy), sizeof (Uint32));
        SW_RTN_ON_ERROR(rv);
//        aos_udelay(5);
    }

    if (i == 0)
        return SW_BUSY;

    HSL_REG_ENTRY_GET(rv, dev_id, VLAN_TABLE_FUNC1, 0,
                      (Uint8 *) (&val), sizeof (Uint32));
    SW_SET_REG_BY_FIELD(VLAN_TABLE_FUNC1, VT_FUNC, op, val);
    SW_SET_REG_BY_FIELD(VLAN_TABLE_FUNC1, VT_BUSY, 1, val);

    HSL_REG_ENTRY_SET(rv, dev_id, VLAN_TABLE_FUNC1, 0,
                      (Uint8 *) (&val), sizeof (Uint32));

    SW_RTN_ON_ERROR(rv);

    vt_busy = 1;
    i = 0x1000;
    while (vt_busy && --i) {
        HSL_REG_FIELD_GET(rv, dev_id, VLAN_TABLE_FUNC1, 0, VT_BUSY,
                          (Uint8 *) (&vt_busy), sizeof (Uint32));
        SW_RTN_ON_ERROR(rv);
//        aos_udelay(5);
    }

    if (i == 0)
        return SW_FAIL;

    HSL_REG_FIELD_GET(rv, dev_id, VLAN_TABLE_FUNC1, 0, VT_FULL_VIO,
                      (Uint8 *) (&vt_full), sizeof (Uint32));

    SW_RTN_ON_ERROR(rv);

    if (vt_full) {
        val = 0x10;
        HSL_REG_ENTRY_SET(rv, dev_id, VLAN_TABLE_FUNC1, 0,
                          (Uint8 *) (&val), sizeof (Uint32));
        SW_RTN_ON_ERROR(rv);
        if (VLAN_LOAD_ENTRY == op) {
            return SW_FULL;
        } else if (VLAN_PURGE_ENTRY == op) {
            return SW_NOT_FOUND;
        }
    }

    HSL_REG_FIELD_GET(rv, dev_id, VLAN_TABLE_FUNC0, 0, VT_VALID,
                      (Uint8 *) (&val), sizeof (Uint32));

    SW_RTN_ON_ERROR(rv);

    if (!val) {
        if (VLAN_FIND_ENTRY == op)
            return SW_NOT_FOUND;

        if (VLAN_NEXT_ENTRY == op)
            return SW_NO_MORE;
    }

    return SW_OK;
}

static Int32
_cslAtheros8328N_vlan_hwentry_get(Uint32 dev_id, Uint32 vlan_id, Uint32 reg[])
{
    Int32 rv;

    if (vlan_id > MAX_VLAN_ID) {
        return SW_OUT_OF_RANGE;
    }

    SW_SET_REG_BY_FIELD(VLAN_TABLE_FUNC1, VLAN_ID, vlan_id, reg[1]);
    HSL_REG_ENTRY_SET(rv, dev_id, VLAN_TABLE_FUNC1, 0, (Uint8 *) (&reg[1]), sizeof (Uint32));
    SW_RTN_ON_ERROR(rv);

    rv = _cslAtheros8328N_vlan_commit(dev_id, VLAN_FIND_ENTRY);
    SW_RTN_ON_ERROR(rv);

    rv = _cslAtheros8328N_vlan_up_to_sw(dev_id, reg);
    return rv;
}

static Int32
_cslAtheros8328N_vlan_entry_append(Uint32 dev_id, const CSL_SWITCH_VLAN_ENTRY * vlan_entry)
{
    Int32 rv;
    Uint32 reg[2] = { 0 };

//    HSL_DEV_ID_CHECK(dev_id);

    rv = _cslAtheros8328N_vlan_sw_to_hw(dev_id, vlan_entry, reg);
    SW_RTN_ON_ERROR(rv);

    rv = _cslAtheros8328N_vlan_down_to_hw(dev_id, reg);
    SW_RTN_ON_ERROR(rv);

    rv = _cslAtheros8328N_vlan_commit(dev_id, VLAN_LOAD_ENTRY);
    return rv;
}

static Int32
_cslAtheros8328N_vlan_create(Uint32 dev_id, Uint32 vlan_id)
{
    Int32 rv;
    Uint32 reg[2] = { 0 };

//    HSL_DEV_ID_CHECK(dev_id);

    if (vlan_id > MAX_VLAN_ID) {
        return SW_OUT_OF_RANGE;
    }

    SW_SET_REG_BY_FIELD(VLAN_TABLE_FUNC0, VT_VALID, 1,          reg[0]);
    SW_SET_REG_BY_FIELD(VLAN_TABLE_FUNC0, IVL_EN,   1,          reg[0]);
    SW_SET_REG_BY_FIELD(VLAN_TABLE_FUNC0, LEARN_DIS, 0,         reg[0]);
    SW_SET_REG_BY_FIELD(VLAN_TABLE_FUNC0, VID_MEM,  0x3fff,     reg[0]);
    SW_SET_REG_BY_FIELD(VLAN_TABLE_FUNC1, VLAN_ID,  vlan_id,    reg[1]);

    rv = _cslAtheros8328N_vlan_down_to_hw(dev_id, reg);
    SW_RTN_ON_ERROR(rv);

    rv = _cslAtheros8328N_vlan_commit(dev_id, VLAN_LOAD_ENTRY);
    return rv;
}

static Int32
_cslAtheros8328N_vlan_next(Uint32 dev_id, Uint32 vlan_id, CSL_SWITCH_VLAN_ENTRY * p_vlan)
{
    Int32 rv;
    Uint32 reg[2] = { 0 };

//    HSL_DEV_ID_CHECK(dev_id);

    if (FAL_NEXT_ENTRY_FIRST_ID == vlan_id) {
        rv = _cslAtheros8328N_vlan_hwentry_get(dev_id, 0, reg);

        if (SW_OK == rv) {
            _cslAtheros8328N_vlan_hw_to_sw(reg, p_vlan);
            return SW_OK;
        } else {
            vlan_id = 0;
        }
    }

    if (vlan_id > MAX_VLAN_ID)
        return SW_OUT_OF_RANGE;

    SW_SET_REG_BY_FIELD(VLAN_TABLE_FUNC1, VLAN_ID, vlan_id, reg[1]);
    HSL_REG_ENTRY_SET(rv, dev_id, VLAN_TABLE_FUNC1, 0,
                      (Uint8 *) (&reg[1]), sizeof (Uint32));

    SW_RTN_ON_ERROR(rv);

    rv = _cslAtheros8328N_vlan_commit(dev_id, VLAN_NEXT_ENTRY);
    SW_RTN_ON_ERROR(rv);

    rv = _cslAtheros8328N_vlan_up_to_sw(dev_id, reg);
    SW_RTN_ON_ERROR(rv);

    _cslAtheros8328N_vlan_hw_to_sw(reg, p_vlan);

    if (0 == p_vlan->vid) {
        return SW_NO_MORE;
    } else {
        return SW_OK;
    }
}

static Int32
_cslAtheros8328N_vlan_find(Uint32 dev_id, Uint32 vlan_id, CSL_SWITCH_VLAN_ENTRY * p_vlan)
{
    Int32 rv;
    Uint32 reg[2] = { 0 };

//    HSL_DEV_ID_CHECK(dev_id);

    rv = _cslAtheros8328N_vlan_hwentry_get(dev_id, vlan_id, reg);
    SW_RTN_ON_ERROR(rv);

    _cslAtheros8328N_vlan_hw_to_sw(reg, p_vlan);
    return SW_OK;
}

static Int32
_cslAtheros8328N_vlan_delete(Uint32 dev_id, Uint32 vlan_id)
{
    Int32 rv;
    Uint32 reg;

//    HSL_DEV_ID_CHECK(dev_id);

    if (vlan_id > MAX_VLAN_ID) {
        return SW_OUT_OF_RANGE;
    }

    reg = (Int32) vlan_id;
    HSL_REG_FIELD_SET(rv, dev_id, VLAN_TABLE_FUNC1, 0, VLAN_ID,
                      (Uint8 *) (&reg), sizeof (Uint32));
    SW_RTN_ON_ERROR(rv);

    rv = _cslAtheros8328N_vlan_commit(dev_id, VLAN_PURGE_ENTRY);
    return rv;
}

static Int32
_cslAtheros8328N_vlan_flush(Uint32 dev_id)
{
    Int32 rv;

//    HSL_DEV_ID_CHECK(dev_id);

    rv = _cslAtheros8328N_vlan_commit(dev_id, VLAN_FLUSH);
    return rv;
}

static Int32
_cslAtheros8328N_vlan_fid_set(Uint32 dev_id, Uint32 vlan_id, Uint32 fid)
{
    Int32 rv;
    Uint32 reg[2] = { 0 };

//    HSL_DEV_ID_CHECK(dev_id);

    if ((MAX_VLAN_ID < fid) && (0xFFFF != fid)) {
        return SW_BAD_PARAM;
    }

    if ((MAX_VLAN_ID >= fid) && (vlan_id != fid)) {
        return SW_BAD_PARAM;
    }

    rv = _cslAtheros8328N_vlan_hwentry_get(dev_id, vlan_id, reg);
    SW_RTN_ON_ERROR(rv);

    if (0xFFFF == fid) {
        SW_SET_REG_BY_FIELD(VLAN_TABLE_FUNC0, IVL_EN, 0, reg[0]);
    } else {
        SW_SET_REG_BY_FIELD(VLAN_TABLE_FUNC0, IVL_EN, 1, reg[0]);
    }

    rv = _cslAtheros8328N_vlan_down_to_hw(dev_id, reg);
    SW_RTN_ON_ERROR(rv);

    rv = _cslAtheros8328N_vlan_commit(dev_id, VLAN_LOAD_ENTRY);
    if (SW_FULL == rv) {
        rv = SW_OK;
    }
    return rv;
}

static Int32
_cslAtheros8328N_vlan_fid_get(Uint32 dev_id, Uint32 vlan_id, Uint32 * fid)
{
    Int32 rv;
    Uint32 data, reg[2] = { 0 };

//    HSL_DEV_ID_CHECK(dev_id);

    rv = _cslAtheros8328N_vlan_hwentry_get(dev_id, vlan_id, reg);
    SW_RTN_ON_ERROR(rv);

    SW_GET_FIELD_BY_REG(VLAN_TABLE_FUNC0, IVL_EN, data, reg[0]);
    if (data) {
        *fid = vlan_id;
    } else {
        *fid = 0xFFFF;
    }
    return SW_OK;
}

static Int32
_cslAtheros8328N_vlan_member_update(Uint32 dev_id, Uint32 vlan_id,
                         fal_port_t port_id, Uint32 port_info)
{
    Int32 rv;
    Uint32 data, reg[2] = { 0 };

    HSL_DEV_ID_CHECK(dev_id);

    rv = _cslAtheros8328N_vlan_hwentry_get(dev_id, vlan_id, reg);
    SW_RTN_ON_ERROR(rv);

    SW_GET_FIELD_BY_REG(VLAN_TABLE_FUNC0, VID_MEM, data, reg[0]);
    data &= (~(0x3 << (port_id << 1)));
    data |= ((port_info & 0x3) << (port_id << 1));
    SW_SET_REG_BY_FIELD(VLAN_TABLE_FUNC0, VID_MEM, data, reg[0]);

    rv = _cslAtheros8328N_vlan_down_to_hw(dev_id, reg);
    SW_RTN_ON_ERROR(rv);

    rv = _cslAtheros8328N_vlan_commit(dev_id, VLAN_LOAD_ENTRY);
    if (SW_FULL == rv) {
        rv = SW_OK;
    }
    return rv;
}

static Int32
_cslAtheros8328N_vlan_member_add(Uint32 dev_id, Uint32 vlan_id,
                      fal_port_t port_id, fal_pt_1q_egmode_t port_info)
{
    Int32 rv;
    Uint32 info = 0;

    if (FAL_EG_UNMODIFIED == port_info) {
        info = 0;
    } else if (FAL_EG_TAGGED == port_info) {
        info = 0x2;
    } else if (FAL_EG_UNTAGGED == port_info) {
        info = 0x1;
    } else {
        return SW_BAD_PARAM;
    }

    rv = _cslAtheros8328N_vlan_member_update(dev_id, vlan_id, port_id, info);
    return rv;
}

static Int32
_cslAtheros8328N_vlan_member_del(Uint32 dev_id, Uint32 vlan_id, fal_port_t port_id)
{
    Int32 rv;
    Uint32 info = 0x3;

    rv = _cslAtheros8328N_vlan_member_update(dev_id, vlan_id, port_id, info);
    return rv;
}

static Int32
_cslAtheros8328N_vlan_learning_state_set(Uint32 dev_id, Uint32 vlan_id,
                              Bool enable)
{
    Int32 rv;
    Uint32 reg[2] = { 0 };

//    HSL_DEV_ID_CHECK(dev_id);

    rv = _cslAtheros8328N_vlan_hwentry_get(dev_id, vlan_id, reg);
    SW_RTN_ON_ERROR(rv);

    if (True == enable) {
        SW_SET_REG_BY_FIELD(VLAN_TABLE_FUNC0, LEARN_DIS, 0, reg[0]);
    } else if (False == enable) {
        SW_SET_REG_BY_FIELD(VLAN_TABLE_FUNC0, LEARN_DIS, 1, reg[0]);
    } else {
        return SW_BAD_PARAM;
    }

    rv = _cslAtheros8328N_vlan_down_to_hw(dev_id, reg);
    SW_RTN_ON_ERROR(rv);

    rv = _cslAtheros8328N_vlan_commit(dev_id, VLAN_LOAD_ENTRY);
    if (SW_FULL == rv) {
        rv = SW_OK;
    }
    return rv;
}

static Int32
_cslAtheros8328N_vlan_learning_state_get(Uint32 dev_id, Uint32 vlan_id,
                              Bool * enable)
{
    Int32 rv;
    Uint32 data, reg[2] = { 0 };

//    HSL_DEV_ID_CHECK(dev_id);

    rv = _cslAtheros8328N_vlan_hwentry_get(dev_id, vlan_id, reg);
    SW_RTN_ON_ERROR(rv);

    SW_GET_FIELD_BY_REG(VLAN_TABLE_FUNC0, LEARN_DIS, data, reg[0]);
    if (data) {
        *enable = False;
    } else {
        *enable = True;
    }
    return SW_OK;
}

/**
 * @brief Append a vlan entry on paticular device.
 * @param[in] dev_id device id
 * @param[in] vlan_entry vlan entry
 * @return SW_OK or error code
 */
HSL_LOCAL Int32
cslAtheros8328N_vlan_entry_append(Uint32 dev_id, const CSL_SWITCH_VLAN_ENTRY * vlan_entry)
{
    Int32 rv;

    HSL_API_LOCK;
    rv = _cslAtheros8328N_vlan_entry_append(dev_id, vlan_entry);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Creat a vlan entry through vlan id on a paticular device.
 *   @details   Comments:
 *    After this operation the member ports of the created vlan entry are null.
 * @param[in] dev_id device id
 * @param[in] vlan_id vlan id
 * @return SW_OK or error code
 */
HSL_LOCAL Int32
cslAtheros8328N_vlan_create(Uint32 dev_id, Uint32 vlan_id)
{
    Int32 rv;

    HSL_API_LOCK;
    rv = _cslAtheros8328N_vlan_create(dev_id, vlan_id);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Next a vlan entry through vlan id on a paticular device.
 *   @details   Comments:
 *    If the value of vid is zero this operation will get the first entry.
 * @param[in] dev_id device id
 * @param[in] vlan_id vlan id
 * @param[out] p_vlan vlan entry
 * @return SW_OK or error code
 */
HSL_LOCAL Int32
cslAtheros8328N_vlan_next(Uint32 dev_id, Uint32 vlan_id, CSL_SWITCH_VLAN_ENTRY * p_vlan)
{
    Int32 rv;

    HSL_API_LOCK;
    rv = _cslAtheros8328N_vlan_next(dev_id, vlan_id, p_vlan);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Find a vlan entry through vlan id on paticular device.
 * @param[in] dev_id device id
 * @param[in] vlan_id vlan id
 * @param[out] p_vlan vlan entry
 * @return SW_OK or error code
 */
HSL_LOCAL Int32
cslAtheros8328N_vlan_find(Uint32 dev_id, Uint32 vlan_id, CSL_SWITCH_VLAN_ENTRY * p_vlan)
{
    Int32 rv;

    HSL_API_LOCK;
    rv = _cslAtheros8328N_vlan_find(dev_id, vlan_id, p_vlan);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Delete a vlan entry through vlan id on a paticular device.
 * @param[in] dev_id device id
 * @param[in] vlan_id vlan id
 * @return SW_OK or error code
 */
HSL_LOCAL Int32
cslAtheros8328N_vlan_delete(Uint32 dev_id, Uint32 vlan_id)
{
    Int32 rv;

    HSL_API_LOCK;
    rv = _cslAtheros8328N_vlan_delete(dev_id, vlan_id);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Flush all vlan entries on a paticular device.
 * @param[in] dev_id device id
 * @return SW_OK or error code
 */
HSL_LOCAL Int32
cslAtheros8328N_vlan_flush(Uint32 dev_id)
{
    Int32 rv;

    HSL_API_LOCK;
    rv = _cslAtheros8328N_vlan_flush(dev_id);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Set FID of a paticular vlan entry on a paticular device.
 * @param[in] dev_id device id
 * @param[in] vlan_id vlan id
 * @param[in] fid FDB id
 * @return SW_OK or error code
 */
HSL_LOCAL Int32
cslAtheros8328N_vlan_fid_set(Uint32 dev_id, Uint32 vlan_id, Uint32 fid)
{
    Int32 rv;

    HSL_API_LOCK;
    rv = _cslAtheros8328N_vlan_fid_set(dev_id, vlan_id, fid);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get FID of a paticular vlan entry on a paticular device.
 * @param[in] dev_id device id
 * @param[in] vlan_id vlan id
 * @param[out] fid FDB id
 * @return SW_OK or error code
 */
HSL_LOCAL Int32
cslAtheros8328N_vlan_fid_get(Uint32 dev_id, Uint32 vlan_id, Uint32 * fid)
{
    Int32 rv;

    HSL_API_LOCK;
    rv = _cslAtheros8328N_vlan_fid_get(dev_id, vlan_id, fid);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Add a port member to a paticular vlan entry on a paticular device.
 * @param[in] dev_id device id
 * @param[in] vlan_id vlan id
 * @param[in] port_id port id
 * @param[in] port_info port tag information
 * @return SW_OK or error code
 */
HSL_LOCAL Int32
cslAtheros8328N_vlan_member_add(Uint32 dev_id, Uint32 vlan_id, fal_port_t port_id, VLN_TAG port_info)
{
    Int32 rv;
    fal_pt_1q_egmode_t tmptag;

    switch (port_info)
    {
        case VLN_PORT_UNTAG:        tmptag = FAL_EG_UNTAGGED;   break;
        case VLN_PORT_TAG:          tmptag = FAL_EG_TAGGED;     break;
        case VLN_PORT_UNMODIFIED:   tmptag = FAL_EG_UNMODIFIED; break;
        case VLN_PORT_HYBRID:       tmptag = FAL_EG_HYBRID;     break;
        default:    break;
    }

    HSL_API_LOCK;
    rv = _cslAtheros8328N_vlan_member_add(dev_id, vlan_id, port_id, tmptag);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Del a port member from a paticular vlan entry on a paticular device.
 * @param[in] dev_id device id
 * @param[in] vlan_id vlan id
 * @param[in] port_id port id
 * @return SW_OK or error code
 */
HSL_LOCAL Int32
cslAtheros8328N_vlan_member_del(Uint32 dev_id, Uint32 vlan_id, fal_port_t port_id)
{
    Int32 rv;

    HSL_API_LOCK;
    rv = _cslAtheros8328N_vlan_member_del(dev_id, vlan_id, port_id);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Set FDB learning status of a paticular vlan entry on a paticular device.
 * @param[in] dev_id device id
 * @param[in] vlan_id vlan id
 * @param[in] enable True or False
 * @return SW_OK or error code
 */
HSL_LOCAL Int32
cslAtheros8328N_vlan_learning_state_set(Uint32 dev_id, Uint32 vlan_id,
                             Bool enable)
{
    Int32 rv;

    HSL_API_LOCK;
    rv = _cslAtheros8328N_vlan_learning_state_set(dev_id, vlan_id, enable);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get FDB learning status of a paticular vlan entry on a paticular device.
 * @param[in] dev_id device id
 * @param[in] vlan_id vlan id
 * @param[out] enable True or False
 * @return SW_OK or error code
 */
HSL_LOCAL Int32
cslAtheros8328N_vlan_learning_state_get(Uint32 dev_id, Uint32 vlan_id,
                             Bool * enable)
{
    Int32 rv;

    HSL_API_LOCK;
    rv = _cslAtheros8328N_vlan_learning_state_get(dev_id, vlan_id, enable);
    HSL_API_UNLOCK;
    return rv;
}

Int32 cslAtheros8328N_empty_func(void)
{
    return SW_OK;
}















static Int32 _cslAtheros8328n_mdio_reg_get(Uint32 dev_id, Uint32 reg_addr, Uint8 value[], Uint32 value_len)
{
    Uint32 phyadr;
    Uint32 regadr;
    Uint32 data;

    if (value_len != sizeof (Uint32))
        return -1;

    reg_addr &= ~3;

    _cpswHalCommonMiiMdioUserAccessWrite( pSwitchInfo, 0, 0x18, (reg_addr >> 9) );

    phyadr = 0x10 | (0x7  & (reg_addr >> 6));
    regadr = 0    | (0x1F & (reg_addr >> 1));
    data = _cpswHalCommonMiiMdioUserAccessRead( pSwitchInfo, regadr, phyadr );

    phyadr = 0x10 | (0x7  & (reg_addr >> 6));
    regadr = 1    | (0x1F & (reg_addr >> 1));
    data |= ((_cpswHalCommonMiiMdioUserAccessRead( pSwitchInfo, regadr, phyadr )) <<  16);

    memcpy(value, &data, sizeof (Uint32));

    return 0;
}

static Int32 _cslAtheros8328n_mdio_reg_set(Uint32 dev_id, Uint32 reg_addr, Uint8 value[], Uint32 value_len)
{
    Uint32 phyadr;
    Uint32 regadr;
    Uint32 data;

    if (value_len != sizeof (Uint32))
        return -1;

    memcpy(&data, value, sizeof (Uint32));

    reg_addr &= ~3;

    _cpswHalCommonMiiMdioUserAccessWrite( pSwitchInfo, 0, 0x18, (reg_addr >> 9) );

    phyadr = 0x10 | (0x7  & (reg_addr >> 6));
    regadr = 0    | (0x1F & (reg_addr >> 1));
    _cpswHalCommonMiiMdioUserAccessWrite( pSwitchInfo, regadr, phyadr, data & 0xFFFF );

    phyadr = 0x10 | (0x7  & (reg_addr >> 6));
    regadr = 1    | (0x1F & (reg_addr >> 1));
    _cpswHalCommonMiiMdioUserAccessWrite( pSwitchInfo, regadr, phyadr, data >> 16    );

    return 0;
}

Int32 cslAtheros8328n_phy_get(Uint32 dev_id, Uint32 phy_addr, Uint32 reg, Uint16 * value)
{
    MDIO_LOCKER_LOCK;
    *value = _cpswHalCommonMiiMdioUserAccessRead( pSwitchInfo, reg, phy_addr );
    MDIO_LOCKER_UNLOCK;

    return 0;
}

Int32 cslAtheros8328n_phy_set(Uint32 dev_id, Uint32 phy_addr,Uint32 reg, Uint16 value)
{
    MDIO_LOCKER_LOCK;
    _cpswHalCommonMiiMdioUserAccessWrite( pSwitchInfo, reg, phy_addr, value );
    MDIO_LOCKER_UNLOCK;

    return 0;
}

Int32 cslAtheros8328n_reg_get(Uint32 dev_id, Uint32 reg_addr, void *value, Uint32 value_len)
{
    Int32 rv;
    //printk(" %s: reg:%08x data_ptr:%08x [0x%08x]\n",__FUNCTION__,reg_addr,value,*(Uint32 *)value);

    MDIO_LOCKER_LOCK;
    rv = _cslAtheros8328n_mdio_reg_get(dev_id, reg_addr, value, value_len);
    MDIO_LOCKER_UNLOCK;

    return rv;
}

Int32 cslAtheros8328n_reg_set(Uint32 dev_id, Uint32 reg_addr, void *value, Uint32 value_len)
{
    Int32 rv;
    //printk(" %s: reg:%08x data_ptr:%08x [0x%08x]\n",__FUNCTION__,reg_addr,value,*(Uint32 *)value);

    MDIO_LOCKER_LOCK;
    rv = _cslAtheros8328n_mdio_reg_set(dev_id, reg_addr, value, value_len);
    MDIO_LOCKER_UNLOCK;

    return rv;
}

Int32 cslAtheros8328n_reg_field_get(Uint32 dev_id,
                         Uint32 reg_addr,
                         Uint32 bit_offset,
                         Uint32 field_len,
                         Uint8  value[],
                         Uint32 value_len)
{
    Uint32 reg_val = 0;

    if ((bit_offset >= 32 || (field_len > 32)) || (field_len == 0))
        return -1;

    if (value_len != sizeof (Uint32))
        return -1;

    SW_RTN_ON_ERROR(cslAtheros8328n_reg_get(dev_id, reg_addr, (Uint8 *) & reg_val, sizeof (Uint32)));

    *((Uint32 *) value) = SW_REG_2_FIELD(reg_val, bit_offset, field_len);
    return 0;
}

Int32 cslAtheros8328n_reg_field_set( Uint32 dev_id,
                          Uint32 reg_addr,
                          Uint32 bit_offset,
                          Uint32 field_len,
                          const Uint8 value[],
                          Uint32 value_len)
{
    Uint32 reg_val;
    Uint32 field_val = *((Uint32 *) value);

    if ((bit_offset >= 32 || (field_len > 32)) || (field_len == 0))
        return -1;

    if (value_len != sizeof (Uint32))
        return -1;

    SW_RTN_ON_ERROR(cslAtheros8328n_reg_get(dev_id, reg_addr, (Uint8 *) & reg_val, sizeof (Uint32)));

    SW_REG_SET_BY_FIELD_U32(reg_val, field_val, bit_offset, field_len);

    SW_RTN_ON_ERROR(cslAtheros8328n_reg_set(dev_id, reg_addr, (Uint8 *) & reg_val, sizeof (Uint32)));

    return 0;
}


Uint32 cslAtheros8328n_reg_addr_get(OPER_TYPE_E type, Uint32 portNum)
{
    return 0;
}

Uint32 cslAtheros8328n_phy_addr_get(OPER_TYPE_E type, Uint32 portNum)
{
    return 0;
}

Int32 cslAtheros8328nRegVal2OperStatus (OPER_TYPE_E oper, Uint16 data)
{
    Int32 retVal = -1;

    if (oper == OPER_STATUS)
    {
        return 0;
    }
    else if (oper == OPER_LINK)
    {
        return PORT_LINK_UP;
    }
    else if (oper == OPER_DUPLEX)
    {
        return PORT_DUPLEX_FULL;
    }
    else if (oper == OPER_SPEED)
    {
        return PORT_SPEED_1000;
    }
    else if (oper == OPER_FC)
    {
        return PORT_FC_ON;
    }
    else if (oper == OPER_AUTO)
    {
        return PORT_AUTO_YES;
    }

    return retVal;
}


Uint16 cslAtheros8328nOperStatus2RegVal (OPER_TYPE_E oper,Int32 operValue,Uint16 data)
{
    Uint16 retVal = data;

    return retVal;
}


void cslAtheros8328nSwitchGetVersion(PSW_VER_INFO pSwitchVerInfo)
{
    pSwitchVerInfo->is802_1_P_capable      = PAL_True;
    pSwitchVerInfo->is802_1_Q_capable      = PAL_True;
    pSwitchVerInfo->maxNumberOfPorts       = 7;
    pSwitchVerInfo->maxNumberOfPriorities  = 4;
    pSwitchVerInfo->maxNumberOfVLANs       = 64;

    pSwitchVerInfo->Model = 8328;
}

Int32 cslAtheros8328N_global_init(CSL_SWITCH_CLBKS * p_api)
{
    SW_RTN_ON_NULL(p_api);

    p_api->cslSwitchStart =             cslAtheros8328N_empty_func;
    p_api->cslRegGet =                  cslAtheros8328n_reg_get;
    p_api->cslRegSet =                  cslAtheros8328n_reg_set;
    p_api->cslPortNum2RegAddr =         cslAtheros8328n_reg_addr_get;
    p_api->cslPortNum2PhyAddr =         cslAtheros8328n_phy_addr_get;
    p_api->cslRegVal2operStatus =       cslAtheros8328nRegVal2OperStatus;
    p_api->cslOperStatus2regVal =       cslAtheros8328nOperStatus2RegVal;
    p_api->cslSwitchGetVersion  =       cslAtheros8328nSwitchGetVersion;

    return SW_OK;
}

Int32 cslAtheros8328N_vlan_init(CSL_SWITCH_CLBKS * p_api)
{
    SW_RTN_ON_NULL(p_api);

    p_api->cslVlnUpdate =               cslAtheros8328N_empty_func;

    p_api->vlan_entry_append =          cslAtheros8328N_vlan_entry_append;
    p_api->vlan_creat =                 cslAtheros8328N_vlan_create;
    p_api->vlan_delete =                cslAtheros8328N_vlan_delete;
    p_api->vlan_next =                  cslAtheros8328N_vlan_next;
    p_api->vlan_find =                  cslAtheros8328N_vlan_find;
    p_api->vlan_flush =                 cslAtheros8328N_vlan_flush;
    p_api->vlan_fid_set =               cslAtheros8328N_vlan_fid_set;
    p_api->vlan_fid_get =               cslAtheros8328N_vlan_fid_get;
    p_api->vlan_member_add =            cslAtheros8328N_vlan_member_add;
    p_api->vlan_member_del =            cslAtheros8328N_vlan_member_del;
    p_api->vlan_learning_state_set =    cslAtheros8328N_vlan_learning_state_set;
    p_api->vlan_learning_state_get =    cslAtheros8328N_vlan_learning_state_get;

    return SW_OK;
}


/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/


#define MAX_VLAN_ID          4095
#define ISIS_MAX_VLAN_TRANS  64
#define ISIS_VLAN_TRANS_ADDR 0x5ac00

#if 1
static sw_error_t
_cslAtheros8328N_port_route_defv_set(Uint32 dev_id, fal_port_t port_id)
{
    sw_error_t rv;
    Uint32 data, reg;

    HSL_REG_FIELD_GET(rv, dev_id, PORT_VLAN1, port_id,
                      COREP_EN, (Uint8 *) (&data), sizeof (Uint32));

    SW_RTN_ON_ERROR(rv);

    if (data) {
        HSL_REG_FIELD_GET(rv, dev_id, PORT_VLAN0, port_id,
                      DEF_SVID, (Uint8 *) (&data), sizeof (Uint32));
    } else {
        HSL_REG_FIELD_GET(rv, dev_id, PORT_VLAN0, port_id,
                      DEF_CVID, (Uint8 *) (&data), sizeof (Uint32));
    }

    HSL_REG_ENTRY_GET(rv, dev_id, ROUTER_DEFV, (port_id / 2),
                      (Uint8 *) (&reg), sizeof (Uint32));
    SW_RTN_ON_ERROR(rv);

    if (port_id % 2) {
        reg &= 0xffff;
        reg |= ((data & 0xfff) << 16);
    } else {
       reg &= 0xffff0000;
       reg |= (data & 0xfff);
    }

    HSL_REG_ENTRY_SET(rv, dev_id, ROUTER_DEFV, (port_id / 2),
                      (Uint8 *) (&reg), sizeof (Uint32));
    return rv;
}
#endif

static sw_error_t
_cslAtheros8328N_port_1qmode_set(Uint32 dev_id, fal_port_t port_id, csl_pt_1qmode_t port_1qmode)
{
    sw_error_t rv;
    Uint32 data, regval[CSL_1Q_MODE_BUTT] = { 0, 3, 2, 1 };

    HSL_DEV_ID_CHECK(dev_id);

    if (CSL_1Q_MODE_BUTT <= port_1qmode) {
        return SW_BAD_PARAM;
    }

    HSL_REG_FIELD_SET(rv, dev_id, PORT_LOOKUP_CTL, port_id, DOT1Q_MODE,
                      (Uint8 *) (&regval[port_1qmode]),
                      sizeof (Uint32));
    SW_RTN_ON_ERROR(rv);

    if (CSL_1Q_DISABLE == port_1qmode) {
        data = 1;
    } else {
        data = 0;
    }

    HSL_REG_FIELD_SET(rv, dev_id, PORT_VLAN1, port_id, VLAN_DIS,
                      (Uint8 *) (&data), sizeof (Uint32));
    return rv;
}

static sw_error_t
_cslAtheros8328N_port_1qmode_get(Uint32 dev_id, fal_port_t port_id, csl_pt_1qmode_t * pport_1qmode)
{
    sw_error_t rv;
    Uint32 regval = 0;
    csl_pt_1qmode_t retval[4] = { CSL_1Q_DISABLE, CSL_1Q_FALLBACK,
        CSL_1Q_CHECK, CSL_1Q_SECURE
    };

    HSL_DEV_ID_CHECK(dev_id);

    SW_RTN_ON_NULL(pport_1qmode);

    HSL_REG_FIELD_GET(rv, dev_id, PORT_LOOKUP_CTL, port_id, DOT1Q_MODE,
                      (Uint8 *) (&regval), sizeof (Uint32));

    SW_RTN_ON_ERROR(rv);

    *pport_1qmode = retval[regval & 0x3];

    return SW_OK;
}


static sw_error_t
_cslAtheros8328N_port_egvlanmode_set(Uint32 dev_id, fal_port_t port_id, fal_pt_1q_egmode_t port_egvlanmode)
{
    sw_error_t rv;
    Uint32 data, regval[FAL_EG_MODE_BUTT] = { 0, 1, 2, 3, 3 };

    HSL_DEV_ID_CHECK(dev_id);

    if ((FAL_EG_MODE_BUTT <= port_egvlanmode)
        || (FAL_EG_HYBRID == port_egvlanmode)) {
        return SW_BAD_PARAM;
    }

    HSL_REG_FIELD_SET(rv, dev_id, PORT_VLAN1, port_id, EG_VLAN_MODE,
                      (Uint8 *) (&regval[port_egvlanmode]),
                      sizeof (Uint32));

    SW_RTN_ON_ERROR(rv);

    HSL_REG_ENTRY_GET(rv, dev_id, ROUTER_EG, 0,
                      (Uint8 *) (&data), sizeof (Uint32));
    SW_RTN_ON_ERROR(rv);

    data &= (~(0x3 << (port_id << 2)));
    data |= (regval[port_egvlanmode] << (port_id << 2));

    HSL_REG_ENTRY_SET(rv, dev_id, ROUTER_EG, 0,
                      (Uint8 *) (&data), sizeof (Uint32));
    return rv;
}

static sw_error_t
_cslAtheros8328N_port_egvlanmode_get(Uint32 dev_id, fal_port_t port_id, fal_pt_1q_egmode_t * pport_egvlanmode)
{
    sw_error_t rv;
    Uint32 regval = 0;
    fal_pt_1q_egmode_t retval[4] = { FAL_EG_UNMODIFIED, FAL_EG_UNTAGGED,
        FAL_EG_TAGGED, FAL_EG_UNTOUCHED
    };

    HSL_DEV_ID_CHECK(dev_id);

    SW_RTN_ON_NULL(pport_egvlanmode);

    HSL_REG_FIELD_GET(rv, dev_id, PORT_VLAN1, port_id, EG_VLAN_MODE,
                      (Uint8 *) (&regval), sizeof (Uint32));

    SW_RTN_ON_ERROR(rv);

    *pport_egvlanmode = retval[regval & 0x3];

    return SW_OK;
}

static sw_error_t
_cslAtheros8328N_portvlan_member_add(Uint32 dev_id, fal_port_t port_id,
                          Uint32 mem_port_id)
{
    sw_error_t rv;
    Uint32 regval = 0;

    HSL_DEV_ID_CHECK(dev_id);

    HSL_REG_FIELD_GET(rv, dev_id, PORT_LOOKUP_CTL, port_id,
                      PORT_VID_MEM, (Uint8 *) (&regval),
                      sizeof (Uint32));

    SW_RTN_ON_ERROR(rv);

    regval |= (0x1UL << mem_port_id);

    HSL_REG_FIELD_SET(rv, dev_id, PORT_LOOKUP_CTL, port_id,
                      PORT_VID_MEM, (Uint8 *) (&regval),
                      sizeof (Uint32));

    return rv;
}

static sw_error_t
_cslAtheros8328N_portvlan_member_del(Uint32 dev_id, fal_port_t port_id,
                          Uint32 mem_port_id)
{
    sw_error_t rv;
    Uint32 regval = 0;

    HSL_DEV_ID_CHECK(dev_id);

    HSL_REG_FIELD_GET(rv, dev_id, PORT_LOOKUP_CTL, port_id,
                      PORT_VID_MEM, (Uint8 *) (&regval),
                      sizeof (Uint32));

    SW_RTN_ON_ERROR(rv);

    regval &= (~(0x1UL << mem_port_id));

    HSL_REG_FIELD_SET(rv, dev_id, PORT_LOOKUP_CTL, port_id,
                      PORT_VID_MEM, (Uint8 *) (&regval),
                      sizeof (Uint32));

    return rv;
}

static sw_error_t
_cslAtheros8328N_portvlan_member_update(Uint32 dev_id, fal_port_t port_id,
                             fal_pbmp_t mem_port_map)
{
    sw_error_t rv;

    HSL_DEV_ID_CHECK(dev_id);

    HSL_REG_FIELD_SET(rv, dev_id, PORT_LOOKUP_CTL, port_id,
                      PORT_VID_MEM, (Uint8 *) (&mem_port_map),
                      sizeof (Uint32));

    return rv;
}

static sw_error_t
_cslAtheros8328N_portvlan_member_get(Uint32 dev_id, fal_port_t port_id,
                          fal_pbmp_t * mem_port_map)
{
    sw_error_t rv;

    HSL_DEV_ID_CHECK(dev_id);

    *mem_port_map = 0;
    HSL_REG_FIELD_GET(rv, dev_id, PORT_LOOKUP_CTL, port_id,
                      PORT_VID_MEM, (Uint8 *) mem_port_map,
                      sizeof (Uint32));

    SW_RTN_ON_ERROR(rv);

    return SW_OK;
}

#if 1
static sw_error_t
_cslAtheros8328N_port_force_default_vid_set(Uint32 dev_id, fal_port_t port_id,
                                 Bool enable)
{
    sw_error_t rv;
    Uint32 val;

    HSL_DEV_ID_CHECK(dev_id);

    if (True == enable) {
        val = 1;
    } else if (False == enable) {
        val = 0;
    } else {
        return SW_BAD_PARAM;
    }

    HSL_REG_FIELD_SET(rv, dev_id, PORT_VLAN1, port_id,
                      FORCE_DEF_VID, (Uint8 *) (&val), sizeof (Uint32));
    return rv;
}

static sw_error_t
_cslAtheros8328N_port_force_default_vid_get(Uint32 dev_id, fal_port_t port_id,
                                 Bool * enable)
{
    sw_error_t rv;
    Uint32 val;

    HSL_DEV_ID_CHECK(dev_id);

    HSL_REG_FIELD_GET(rv, dev_id, PORT_VLAN1, port_id,
                      FORCE_DEF_VID, (Uint8 *) (&val), sizeof (Uint32));
    SW_RTN_ON_ERROR(rv);

    if (val) {
        *enable = True;
    } else {
        *enable = False;
    }

    return SW_OK;
}

static sw_error_t
_cslAtheros8328N_port_force_portvlan_set(Uint32 dev_id, fal_port_t port_id,
                              Bool enable)
{
    sw_error_t rv;
    Uint32 val;

    HSL_DEV_ID_CHECK(dev_id);

    if (True == enable) {
        val = 1;
    } else if (False == enable) {
        val = 0;
    } else {
        return SW_BAD_PARAM;
    }

    HSL_REG_FIELD_SET(rv, dev_id, PORT_LOOKUP_CTL, port_id,
                      FORCE_PVLAN, (Uint8 *) (&val), sizeof (Uint32));
    return rv;
}

static sw_error_t
_cslAtheros8328N_port_force_portvlan_get(Uint32 dev_id, fal_port_t port_id,
                              Bool * enable)
{
    sw_error_t rv;
    Uint32 val;

    HSL_DEV_ID_CHECK(dev_id);

    HSL_REG_FIELD_GET(rv, dev_id, PORT_LOOKUP_CTL, port_id,
                      FORCE_PVLAN, (Uint8 *) (&val), sizeof (Uint32));
    SW_RTN_ON_ERROR(rv);

    if (val) {
        *enable = True;
    } else {
        *enable = False;
    }

    return SW_OK;
}

static sw_error_t
_cslAtheros8328N_nestvlan_tpid_set(Uint32 dev_id, Uint32 tpid)
{
    sw_error_t rv;
    Uint32 val;

    HSL_DEV_ID_CHECK(dev_id);

    val = tpid;
    HSL_REG_FIELD_SET(rv, dev_id, SERVICE_TAG, 0,
                      TAG_VALUE, (Uint8 *) (&val), sizeof (Uint32));
    return rv;
}

static sw_error_t
_cslAtheros8328N_nestvlan_tpid_get(Uint32 dev_id, Uint32 * tpid)
{
    sw_error_t rv;
    Uint32 val;

    HSL_DEV_ID_CHECK(dev_id);

    HSL_REG_FIELD_GET(rv, dev_id, SERVICE_TAG, 0,
                      TAG_VALUE, (Uint8 *) (&val), sizeof (Uint32));
    SW_RTN_ON_ERROR(rv);

    *tpid = val;
    return SW_OK;
}

static sw_error_t
_cslAtheros8328N_port_invlan_mode_set(Uint32 dev_id, fal_port_t port_id,
                           fal_pt_invlan_mode_t mode)
{
    sw_error_t rv;
    Uint32 regval[FAL_INVLAN_MODE_BUTT] = { 0, 1, 2 };

    HSL_DEV_ID_CHECK(dev_id);

    if (FAL_INVLAN_MODE_BUTT <= mode) {
        return SW_BAD_PARAM;
    }

    HSL_REG_FIELD_SET(rv, dev_id, PORT_VLAN1, port_id, IN_VLAN_MODE,
                      (Uint8 *) (&regval[mode]), sizeof (Uint32));
    return rv;
}

static sw_error_t
_cslAtheros8328N_port_invlan_mode_get(Uint32 dev_id, fal_port_t port_id,
                           fal_pt_invlan_mode_t * mode)
{
    sw_error_t rv;
    Uint32 regval = 0;
    fal_pt_invlan_mode_t retval[FAL_INVLAN_MODE_BUTT] = { FAL_INVLAN_ADMIT_ALL,
        FAL_INVLAN_ADMIT_TAGGED, FAL_INVLAN_ADMIT_UNTAGGED
    };

    HSL_DEV_ID_CHECK(dev_id);

    SW_RTN_ON_NULL(mode);

    HSL_REG_FIELD_GET(rv, dev_id, PORT_VLAN1, port_id, IN_VLAN_MODE,
                      (Uint8 *) (&regval), sizeof (Uint32));

    SW_RTN_ON_ERROR(rv);

    if (regval >= 3) {
        return SW_FAIL;
    }
    *mode = retval[regval & 0x3];

    return rv;
}

static sw_error_t
_cslAtheros8328N_port_tls_set(Uint32 dev_id, fal_port_t port_id, Bool enable)
{
    sw_error_t rv;
    Uint32 val;

    HSL_DEV_ID_CHECK(dev_id);

    if (True == enable) {
        val = 1;
    } else if (False == enable) {
        val = 0;
    } else {
        return SW_BAD_PARAM;
    }

    HSL_REG_FIELD_SET(rv, dev_id, PORT_VLAN1, port_id,
                      TLS_EN, (Uint8 *) (&val), sizeof (Uint32));
    return rv;
}

static sw_error_t
_cslAtheros8328N_port_tls_get(Uint32 dev_id, fal_port_t port_id, Bool * enable)
{
    sw_error_t rv;
    Uint32 val;

    HSL_DEV_ID_CHECK(dev_id);

    HSL_REG_FIELD_GET(rv, dev_id, PORT_VLAN1, port_id,
                      TLS_EN, (Uint8 *) (&val), sizeof (Uint32));
    SW_RTN_ON_ERROR(rv);

    if (1 == val) {
        *enable = True;
    } else {
        *enable = False;
    }

    return SW_OK;
}

static sw_error_t
_cslAtheros8328N_port_pri_propagation_set(Uint32 dev_id, fal_port_t port_id,
                               Bool enable)
{
    sw_error_t rv;
    Uint32 val;

    HSL_DEV_ID_CHECK(dev_id);

    if (True == enable) {
        val = 1;
    } else if (False == enable) {
        val = 0;
    } else {
        return SW_BAD_PARAM;
    }

    HSL_REG_FIELD_SET(rv, dev_id, PORT_VLAN1, port_id,
                      PRI_PROPAGATION, (Uint8 *) (&val),
                      sizeof (Uint32));
    return rv;
}

static sw_error_t
_cslAtheros8328N_port_pri_propagation_get(Uint32 dev_id, fal_port_t port_id,
                               Bool * enable)
{
    sw_error_t rv;
    Uint32 val;

    HSL_DEV_ID_CHECK(dev_id);

    HSL_REG_FIELD_GET(rv, dev_id, PORT_VLAN1, port_id,
                      PRI_PROPAGATION, (Uint8 *) (&val),
                      sizeof (Uint32));
    SW_RTN_ON_ERROR(rv);

    if (1 == val) {
        *enable = True;
    } else {
        *enable = False;
    }

    return SW_OK;
}

static sw_error_t
_cslAtheros8328N_port_default_svid_set(Uint32 dev_id, fal_port_t port_id,
                            Uint32 vid)
{
    sw_error_t rv;
    Uint32 val;

    HSL_DEV_ID_CHECK(dev_id);

    if (vid > MAX_VLAN_ID) {
        return SW_BAD_PARAM;
    }

    val = vid;
    HSL_REG_FIELD_SET(rv, dev_id, PORT_VLAN0, port_id,
                      DEF_SVID, (Uint8 *) (&val), sizeof (Uint32));

    SW_RTN_ON_ERROR(rv);

    rv = _cslAtheros8328N_port_route_defv_set(dev_id, port_id);
    return rv;
}

static sw_error_t
_cslAtheros8328N_port_default_svid_get(Uint32 dev_id, fal_port_t port_id,
                            Uint32 * vid)
{
    sw_error_t rv;
    Uint32 val;

    HSL_DEV_ID_CHECK(dev_id);

    HSL_REG_FIELD_GET(rv, dev_id, PORT_VLAN0, port_id,
                      DEF_SVID, (Uint8 *) (&val), sizeof (Uint32));

    *vid = val & 0xfff;
    return rv;
}

static sw_error_t
_cslAtheros8328N_port_default_cvid_set(Uint32 dev_id, fal_port_t port_id,
                            Uint32 vid)
{
    sw_error_t rv;
    Uint32 val;

    HSL_DEV_ID_CHECK(dev_id);

    if (vid > MAX_VLAN_ID) {
        return SW_BAD_PARAM;
    }

    val = vid;
    HSL_REG_FIELD_SET(rv, dev_id, PORT_VLAN0, port_id,
                      DEF_CVID, (Uint8 *) (&val), sizeof (Uint32));

    SW_RTN_ON_ERROR(rv);

    rv = _cslAtheros8328N_port_route_defv_set(dev_id, port_id);
    return rv;
}

static sw_error_t
_cslAtheros8328N_port_default_cvid_get(Uint32 dev_id, fal_port_t port_id,
                            Uint32 * vid)
{
    sw_error_t rv;
    Uint32 val;

    HSL_DEV_ID_CHECK(dev_id);

    HSL_REG_FIELD_GET(rv, dev_id, PORT_VLAN0, port_id,
                      DEF_CVID, (Uint8 *) (&val), sizeof (Uint32));

    *vid = val & 0xfff;
    return rv;
}

static sw_error_t
_cslAtheros8328N_port_vlan_propagation_set(Uint32 dev_id, fal_port_t port_id,
                                fal_vlan_propagation_mode_t mode)
{
    sw_error_t rv;
    Uint32 reg, p, c;

    HSL_DEV_ID_CHECK(dev_id);


    if (FAL_VLAN_PROPAGATION_DISABLE == mode) {
        p = 0;
        c = 0;
    } else if (FAL_VLAN_PROPAGATION_CLONE == mode) {
        p = 1;
        c = 1;
    } else if (FAL_VLAN_PROPAGATION_REPLACE == mode) {
        p = 1;
        c = 0;
    } else {
        return SW_BAD_PARAM;
    }

    HSL_REG_ENTRY_GET(rv, dev_id, PORT_VLAN1, port_id,
                      (Uint8 *) (&reg), sizeof (Uint32));
    SW_RTN_ON_ERROR(rv);

    SW_SET_REG_BY_FIELD(PORT_VLAN1, PROPAGATION_EN, p, reg);
    SW_SET_REG_BY_FIELD(PORT_VLAN1, CLONE, c, reg);

    HSL_REG_ENTRY_SET(rv, dev_id, PORT_VLAN1, port_id,
                      (Uint8 *) (&reg), sizeof (Uint32));
    return rv;
}

static sw_error_t
_cslAtheros8328N_port_vlan_propagation_get(Uint32 dev_id, fal_port_t port_id,
                                fal_vlan_propagation_mode_t * mode)
{
    sw_error_t rv;
    Uint32 reg, p, c;

    HSL_DEV_ID_CHECK(dev_id);

    HSL_REG_ENTRY_GET(rv, dev_id, PORT_VLAN1, port_id,
                      (Uint8 *) (&reg), sizeof (Uint32));
    SW_RTN_ON_ERROR(rv);

    SW_GET_FIELD_BY_REG(PORT_VLAN1, PROPAGATION_EN, p, reg);
    SW_GET_FIELD_BY_REG(PORT_VLAN1, CLONE, c, reg);

    if (p) {
        if (c) {
            *mode = FAL_VLAN_PROPAGATION_CLONE;
        } else {
            *mode = FAL_VLAN_PROPAGATION_REPLACE;
        }
    } else {
        *mode = FAL_VLAN_PROPAGATION_DISABLE;
    }

    return SW_OK;
}

static sw_error_t
_cslAtheros8328N_vlan_trans_read(Uint32 dev_id, Uint32 entry_idx,
                      fal_pbmp_t * pbmp, fal_vlan_trans_entry_t * entry)
{
    sw_error_t rv;
    Uint32 i, addr, dir, table[2];

    *pbmp = 0;
    memset(entry,0, sizeof (fal_vlan_trans_entry_t));

    addr = ISIS_VLAN_TRANS_ADDR + (entry_idx << 3);
    /* get vlan trans table */
    for (i = 0; i < 2; i++) {
        HSL_REG_ENTRY_GEN_GET(rv, dev_id, addr + (i << 2), sizeof (Uint32),
                              (Uint8 *) (&(table[i])), sizeof (Uint32));
        SW_RTN_ON_ERROR(rv);
    }

    dir = 0x3 & (table[1] >> 4);
    if (!dir) {
        return SW_EMPTY;
    }

    entry->o_vid = table[0] & 0xfff;
    *pbmp = (table[1] >> 6) & 0x7f;

    if (3 == dir) {
        entry->bi_dir = True;
        entry->forward_dir = True;
        entry->reverse_dir = True;
    } else if (1 == dir) {
        entry->bi_dir = False;
        entry->forward_dir = True;
        entry->reverse_dir = False;
    } else {
        entry->bi_dir = False;
        entry->forward_dir = False;
        entry->reverse_dir = True;
    }

    entry->o_vid_is_cvid = (table[1] >> 13) & 0x1UL;
    entry->one_2_one_vlan = (table[1] >> 16) & 0x1UL;
    entry->s_vid_enable = (table[1] >> 14) & 0x1UL;
    entry->c_vid_enable = (table[1] >> 15) & 0x1UL;

    if (True == entry->s_vid_enable) {
        entry->s_vid = (table[0] >> 12) & 0xfff;
    }

    if (True == entry->c_vid_enable) {
        entry->c_vid = ((table[0] >> 24) & 0xff) | ((table[1] & 0xf) << 8);
    }

    return SW_OK;
}

#endif

static sw_error_t
_cslAtheros8328N_vlan_trans_write(Uint32 dev_id, Uint32 entry_idx, fal_pbmp_t pbmp,
                       fal_vlan_trans_entry_t entry)
{
    sw_error_t rv;
    Uint32 i, addr, table[2] = { 0 };

    addr = ISIS_VLAN_TRANS_ADDR + (entry_idx << 3);

    if (0 != pbmp) {
        table[0] = entry.o_vid & 0xfff;
        table[0] |= ((entry.s_vid & 0xfff) << 12);
        table[0] |= ((entry.c_vid & 0xff) << 24);
        table[1] = (entry.c_vid >> 8) & 0xf;

        if (True == entry.bi_dir) {
            table[1] |= (0x3 << 4);
        }

        if (True == entry.forward_dir) {
            table[1] |= (0x1 << 4);
        }

        if (True == entry.reverse_dir) {
            table[1] |= (0x1 << 5);
        }

        table[1] |= (pbmp << 6);
        table[1] |= ((0x1UL & entry.o_vid_is_cvid) << 13);
        table[1] |= ((0x1UL & entry.s_vid_enable) << 14);
        table[1] |= ((0x1UL & entry.c_vid_enable) << 15);
        table[1] |= ((0x1UL & entry.one_2_one_vlan) << 16);
    }

    /* set vlan trans table */
    for (i = 0; i < 2; i++) {
        HSL_REG_ENTRY_GEN_SET(rv, dev_id, addr + (i << 2), sizeof (Uint32),
                              (Uint8 *) (&(table[i])), sizeof (Uint32));
        SW_RTN_ON_ERROR(rv);
    }

    return SW_OK;
}

#if 1
static sw_error_t
_cslAtheros8328N_port_vlan_trans_convert(fal_vlan_trans_entry_t * entry,
                              fal_vlan_trans_entry_t * local)
{
    memcpy(local, entry, sizeof (fal_vlan_trans_entry_t));

    if ((True == local->bi_dir)
        || ((True == local->forward_dir)
            && (True == local->reverse_dir))) {
        local->bi_dir = True;
        local->forward_dir = True;
        local->reverse_dir = True;
    }

    if (False == local->s_vid_enable) {
        local->s_vid = 0;
    }

    if (False == local->c_vid_enable) {
        local->c_vid = 0;
    }

    return SW_OK;
}

static sw_error_t
_cslAtheros8328N_port_vlan_trans_add(Uint32 dev_id, fal_port_t port_id,
                          fal_vlan_trans_entry_t * entry)
{
    sw_error_t rv;
    fal_pbmp_t t_pbmp;
    Uint32 idx, entry_idx = ISIS_MAX_VLAN_TRANS;
    fal_vlan_trans_entry_t temp, local;

    HSL_DEV_ID_CHECK(dev_id);

    rv = _cslAtheros8328N_port_vlan_trans_convert(entry, &local);
    SW_RTN_ON_ERROR(rv);

    for (idx = 0; idx < ISIS_MAX_VLAN_TRANS; idx++) {
        rv = _cslAtheros8328N_vlan_trans_read(dev_id, idx, &t_pbmp, &temp);
        if (SW_EMPTY == rv) {
            entry_idx = idx;
            continue;
        }
        SW_RTN_ON_ERROR(rv);

        if (!memcmp(&local, &temp, sizeof (fal_vlan_trans_entry_t))) {
            if (SW_IS_PBMP_MEMBER(t_pbmp, port_id)) {
                return SW_ALREADY_EXIST;
            }
            entry_idx = idx;
            break;
        } else {
            t_pbmp = 0;
        }
    }

    if (ISIS_MAX_VLAN_TRANS != entry_idx) {
        t_pbmp |= (0x1 << port_id);
    } else {
        return SW_NO_RESOURCE;
    }

    return _cslAtheros8328N_vlan_trans_write(dev_id, entry_idx, t_pbmp, local);
}

static sw_error_t
_cslAtheros8328N_port_vlan_trans_del(Uint32 dev_id, fal_port_t port_id,
                          fal_vlan_trans_entry_t * entry)
{
    sw_error_t rv;
    fal_pbmp_t t_pbmp;
    Uint32 idx, entry_idx = ISIS_MAX_VLAN_TRANS;
    fal_vlan_trans_entry_t temp, local;

    HSL_DEV_ID_CHECK(dev_id);

    rv = _cslAtheros8328N_port_vlan_trans_convert(entry, &local);
    SW_RTN_ON_ERROR(rv);

    for (idx = 0; idx < ISIS_MAX_VLAN_TRANS; idx++) {
        rv = _cslAtheros8328N_vlan_trans_read(dev_id, idx, &t_pbmp, &temp);
        if (SW_EMPTY == rv) {
            continue;
        }
        SW_RTN_ON_ERROR(rv);

        if (!memcmp(&temp, &local, sizeof (fal_vlan_trans_entry_t))) {
            if (SW_IS_PBMP_MEMBER(t_pbmp, port_id)) {
                entry_idx = idx;
                break;
            }
        }
    }

    if (ISIS_MAX_VLAN_TRANS != entry_idx) {
        t_pbmp &= (~(0x1 << port_id));
    } else {
        return SW_NOT_FOUND;
    }

    return _cslAtheros8328N_vlan_trans_write(dev_id, entry_idx, t_pbmp, local);
}

static sw_error_t
_cslAtheros8328N_port_vlan_trans_get(Uint32 dev_id, fal_port_t port_id,
                          fal_vlan_trans_entry_t * entry)
{
    sw_error_t rv;
    fal_pbmp_t t_pbmp;
    Uint32 idx;
    fal_vlan_trans_entry_t temp, local;

    HSL_DEV_ID_CHECK(dev_id);

    rv = _cslAtheros8328N_port_vlan_trans_convert(entry, &local);
    SW_RTN_ON_ERROR(rv);

    for (idx = 0; idx < ISIS_MAX_VLAN_TRANS; idx++) {
        rv = _cslAtheros8328N_vlan_trans_read(dev_id, idx, &t_pbmp, &temp);
        if (SW_EMPTY == rv) {
            continue;
        }
        SW_RTN_ON_ERROR(rv);

        if (!memcmp(&temp, &local, sizeof (fal_vlan_trans_entry_t))) {
            if (SW_IS_PBMP_MEMBER(t_pbmp, port_id)) {
                return SW_OK;
            }
        }
    }

    return SW_NOT_FOUND;
}

static sw_error_t
_cslAtheros8328N_port_vlan_trans_iterate(Uint32 dev_id, fal_port_t port_id,
                              Uint32 * iterator,
                              fal_vlan_trans_entry_t * entry)
{
    Uint32 index;
    sw_error_t rv;
    fal_vlan_trans_entry_t entry_t;
    fal_pbmp_t pbmp_t;

    HSL_DEV_ID_CHECK(dev_id);

    if ((NULL == iterator) || (NULL == entry)) {
        return SW_BAD_PTR;
    }

    if (ISIS_MAX_VLAN_TRANS < *iterator) {
        return SW_BAD_PARAM;
    }

    for (index = *iterator; index < ISIS_MAX_VLAN_TRANS; index++) {
        rv = _cslAtheros8328N_vlan_trans_read(dev_id, index, &pbmp_t, &entry_t);
        if (SW_EMPTY == rv) {
            continue;
        }

        if (SW_IS_PBMP_MEMBER(pbmp_t, port_id)) {
            memcpy(entry, &entry_t, sizeof (fal_vlan_trans_entry_t));
            break;
        }
    }

    if (ISIS_MAX_VLAN_TRANS == index) {
        return SW_NO_MORE;
    }

    *iterator = index + 1;
    return SW_OK;
}

static sw_error_t
_cslAtheros8328N_qinq_mode_set(Uint32 dev_id, fal_qinq_mode_t mode)
{
    sw_error_t rv;
    Uint32 stag = 0;

    HSL_DEV_ID_CHECK(dev_id);

    if (FAL_QINQ_MODE_BUTT <= mode) {
        return SW_BAD_PARAM;
    }

    if (FAL_QINQ_STAG_MODE == mode) {
        stag = 1;
    }

    HSL_REG_FIELD_SET(rv, dev_id, SERVICE_TAG, 0,
                      STAG_MODE, (Uint8 *) (&stag), sizeof (Uint32));

    return rv;
}

static sw_error_t
_cslAtheros8328N_qinq_mode_get(Uint32 dev_id, fal_qinq_mode_t * mode)
{
    sw_error_t rv;
    Uint32 stag = 0;

    HSL_DEV_ID_CHECK(dev_id);

    HSL_REG_FIELD_GET(rv, dev_id, SERVICE_TAG, 0,
                      STAG_MODE, (Uint8 *) (&stag), sizeof (Uint32));

    SW_RTN_ON_ERROR(rv);

    if (stag) {
        *mode = FAL_QINQ_STAG_MODE;
    } else {
        *mode = FAL_QINQ_CTAG_MODE;
    }

    return SW_OK;
}

static sw_error_t
_cslAtheros8328N_port_qinq_role_set(Uint32 dev_id, fal_port_t port_id,
                         fal_qinq_port_role_t role)
{
    sw_error_t rv;
    Uint32 core = 0;

    HSL_DEV_ID_CHECK(dev_id);

    if (FAL_QINQ_PORT_ROLE_BUTT <= role) {
        return SW_BAD_PARAM;
    }

    if (FAL_QINQ_CORE_PORT == role) {
        core = 1;
    }

    HSL_REG_FIELD_SET(rv, dev_id, PORT_VLAN1, port_id,
                      COREP_EN, (Uint8 *) (&core), sizeof (Uint32));

    SW_RTN_ON_ERROR(rv);

    rv = _cslAtheros8328N_port_route_defv_set(dev_id, port_id);
    return rv;
}

static sw_error_t
_cslAtheros8328N_port_qinq_role_get(Uint32 dev_id, fal_port_t port_id,
                         fal_qinq_port_role_t * role)
{
    sw_error_t rv;
    Uint32 core = 0;

    HSL_DEV_ID_CHECK(dev_id);

    HSL_REG_FIELD_GET(rv, dev_id, PORT_VLAN1, port_id,
                      COREP_EN, (Uint8 *) (&core), sizeof (Uint32));

    SW_RTN_ON_ERROR(rv);

    if (core) {
        *role = FAL_QINQ_CORE_PORT;
    } else {
        *role = FAL_QINQ_EDGE_PORT;
    }

    return SW_OK;
}

#endif
/**
 * @brief Set 802.1q work mode on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[in] port_1qmode 802.1q work mode
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
cslAtheros8328N_port_1qmode_set(Uint32 dev_id, fal_port_t port_id, csl_pt_1qmode_t port_1qmode)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _cslAtheros8328N_port_1qmode_set(dev_id, port_id, port_1qmode);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get 802.1q work mode on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[out] port_1qmode 802.1q work mode
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
cslAtheros8328N_port_1qmode_get(Uint32 dev_id, fal_port_t port_id, csl_pt_1qmode_t * pport_1qmode)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _cslAtheros8328N_port_1qmode_get(dev_id, port_id, pport_1qmode);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Set packets transmitted out vlan tagged mode on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[in] port_egvlanmode packets transmitted out vlan tagged mode
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
cslAtheros8328N_port_egvlanmode_set(Uint32 dev_id, fal_port_t port_id, VLN_TAG   port_egvlanmode)
{
    sw_error_t rv;
    fal_pt_1q_egmode_t tmptag;

    switch (port_egvlanmode)
    {
        case VLN_PORT_UNTAG:        tmptag = FAL_EG_UNTAGGED;   break;
        case VLN_PORT_TAG:          tmptag = FAL_EG_TAGGED;     break;
        case VLN_PORT_UNMODIFIED:   tmptag = FAL_EG_UNMODIFIED; break;
        case VLN_PORT_HYBRID:       tmptag = FAL_EG_HYBRID;     break;
        default:    return SW_BAD_PARAM; break;
    }

    HSL_API_LOCK;
    rv = _cslAtheros8328N_port_egvlanmode_set(dev_id, port_id, tmptag);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get packets transmitted out vlan tagged mode on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[out] port_egvlanmode packets transmitted out vlan tagged mode
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
cslAtheros8328N_port_egvlanmode_get(Uint32 dev_id, fal_port_t port_id, VLN_TAG * pport_egvlanmode)
{
    fal_pt_1q_egmode_t tmpmode;
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _cslAtheros8328N_port_egvlanmode_get(dev_id, port_id, &tmpmode);
    HSL_API_UNLOCK;

    switch (tmpmode)
    {
        case FAL_EG_UNTAGGED:       *pport_egvlanmode = VLN_PORT_UNTAG;         break;
        case FAL_EG_TAGGED:         *pport_egvlanmode = VLN_PORT_TAG;           break;
        case FAL_EG_UNMODIFIED:     *pport_egvlanmode = VLN_PORT_UNMODIFIED;    break;
        case FAL_EG_HYBRID:         *pport_egvlanmode = VLN_PORT_HYBRID;        break;
        case FAL_EG_UNTOUCHED:      *pport_egvlanmode = VLN_PORT_UNMODIFIED;    break;
        default:    return SW_BAD_PARAM; break;
    }
    return rv;
}

/**
 * @brief Add member of port based vlan on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[in] mem_port_id port member
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
cslAtheros8328N_portvlan_member_add(Uint32 dev_id, fal_port_t port_id,
                         Uint32 mem_port_id)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _cslAtheros8328N_portvlan_member_add(dev_id, port_id, mem_port_id);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Delete member of port based vlan on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[in] mem_port_id port member
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
cslAtheros8328N_portvlan_member_del(Uint32 dev_id, fal_port_t port_id,
                         Uint32 mem_port_id)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _cslAtheros8328N_portvlan_member_del(dev_id, port_id, mem_port_id);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Update member of port based vlan on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[in] mem_port_map port members
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
cslAtheros8328N_portvlan_member_update(Uint32 dev_id, fal_port_t port_id,
                            fal_pbmp_t mem_port_map)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _cslAtheros8328N_portvlan_member_update(dev_id, port_id, mem_port_map);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get member of port based vlan on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[out] mem_port_map port members
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
cslAtheros8328N_portvlan_member_get(Uint32 dev_id, fal_port_t port_id,
                         fal_pbmp_t * mem_port_map)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _cslAtheros8328N_portvlan_member_get(dev_id, port_id, mem_port_map);
    HSL_API_UNLOCK;
    return rv;
}

#if 1
/**
 * @brief Set force default vlan id status on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[in] enable True or False
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
cslAtheros8328N_port_force_default_vid_set(Uint32 dev_id, fal_port_t port_id,
                                Bool enable)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _cslAtheros8328N_port_force_default_vid_set(dev_id, port_id, enable);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get force default vlan id status on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[out] enable True or False
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
cslAtheros8328N_port_force_default_vid_get(Uint32 dev_id, fal_port_t port_id,
                                Bool * enable)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _cslAtheros8328N_port_force_default_vid_get(dev_id, port_id, enable);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Set force port based vlan status on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[in] enable True or False
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
cslAtheros8328N_port_force_portvlan_set(Uint32 dev_id, fal_port_t port_id,
                             Bool enable)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _cslAtheros8328N_port_force_portvlan_set(dev_id, port_id, enable);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get force port based vlan status on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[out] enable True or False
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
cslAtheros8328N_port_force_portvlan_get(Uint32 dev_id, fal_port_t port_id,
                             Bool * enable)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _cslAtheros8328N_port_force_portvlan_get(dev_id, port_id, enable);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Set nest vlan tpid on a particular device.
 * @param[in] dev_id device id
 * @param[in] tpid tag protocol identification
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
cslAtheros8328N_nestvlan_tpid_set(Uint32 dev_id, Uint32 tpid)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _cslAtheros8328N_nestvlan_tpid_set(dev_id, tpid);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get nest vlan tpid on a particular device.
 * @param[in] dev_id device id
 * @param[out] tpid tag protocol identification
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
cslAtheros8328N_nestvlan_tpid_get(Uint32 dev_id, Uint32 * tpid)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _cslAtheros8328N_nestvlan_tpid_get(dev_id, tpid);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Set ingress vlan mode mode on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[in] mode ingress vlan mode
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
cslAtheros8328N_port_invlan_mode_set(Uint32 dev_id, fal_port_t port_id,
                          fal_pt_invlan_mode_t mode)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _cslAtheros8328N_port_invlan_mode_set(dev_id, port_id, mode);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get ingress vlan mode mode on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[out] mode ingress vlan mode
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
cslAtheros8328N_port_invlan_mode_get(Uint32 dev_id, fal_port_t port_id,
                          fal_pt_invlan_mode_t * mode)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _cslAtheros8328N_port_invlan_mode_get(dev_id, port_id, mode);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Set tls status on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[in] enable True or False
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
cslAtheros8328N_port_tls_set(Uint32 dev_id, fal_port_t port_id, Bool enable)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _cslAtheros8328N_port_tls_set(dev_id, port_id, enable);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get tls status on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[out] enable True or False
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
cslAtheros8328N_port_tls_get(Uint32 dev_id, fal_port_t port_id, Bool * enable)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _cslAtheros8328N_port_tls_get(dev_id, port_id, enable);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Set priority propagation status on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[in] enable True or False
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
cslAtheros8328N_port_pri_propagation_set(Uint32 dev_id, fal_port_t port_id,
                              Bool enable)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _cslAtheros8328N_port_pri_propagation_set(dev_id, port_id, enable);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get priority propagation status on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[out] enable True or False
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
cslAtheros8328N_port_pri_propagation_get(Uint32 dev_id, fal_port_t port_id,
                              Bool * enable)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _cslAtheros8328N_port_pri_propagation_get(dev_id, port_id, enable);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Set default s-vid on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[in] vid s-vid
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
cslAtheros8328N_port_default_svid_set(Uint32 dev_id, fal_port_t port_id,
                           Uint32 vid)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _cslAtheros8328N_port_default_svid_set(dev_id, port_id, vid);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get default s-vid on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[out] vid s-vid
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
cslAtheros8328N_port_default_svid_get(Uint32 dev_id, fal_port_t port_id,
                           Uint32 * vid)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _cslAtheros8328N_port_default_svid_get(dev_id, port_id, vid);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Set default c-vid on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[in] vid c-vid
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
cslAtheros8328N_port_default_cvid_set(Uint32 dev_id, fal_port_t port_id,
                           Uint32 vid)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _cslAtheros8328N_port_default_cvid_set(dev_id, port_id, vid);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get default c-vid on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[out] vid c-vid
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
cslAtheros8328N_port_default_cvid_get(Uint32 dev_id, fal_port_t port_id,
                           Uint32 * vid)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _cslAtheros8328N_port_default_cvid_get(dev_id, port_id, vid);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Set vlan propagation status on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[in] mode vlan propagation mode
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
cslAtheros8328N_port_vlan_propagation_set(Uint32 dev_id, fal_port_t port_id,
                               fal_vlan_propagation_mode_t mode)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _cslAtheros8328N_port_vlan_propagation_set(dev_id, port_id, mode);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get vlan propagation status on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[out] mode vlan propagation mode
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
cslAtheros8328N_port_vlan_propagation_get(Uint32 dev_id, fal_port_t port_id,
                               fal_vlan_propagation_mode_t * mode)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _cslAtheros8328N_port_vlan_propagation_get(dev_id, port_id, mode);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Add a vlan translation entry to a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param entry vlan translation entry
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
cslAtheros8328N_port_vlan_trans_add(Uint32 dev_id, fal_port_t port_id,
                         fal_vlan_trans_entry_t * entry)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _cslAtheros8328N_port_vlan_trans_add(dev_id, port_id, entry);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Delete a vlan translation entry from a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param entry vlan translation entry
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
cslAtheros8328N_port_vlan_trans_del(Uint32 dev_id, fal_port_t port_id,
                         fal_vlan_trans_entry_t * entry)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _cslAtheros8328N_port_vlan_trans_del(dev_id, port_id, entry);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get a vlan translation entry from a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param entry vlan translation entry
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
cslAtheros8328N_port_vlan_trans_get(Uint32 dev_id, fal_port_t port_id,
                         fal_vlan_trans_entry_t * entry)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _cslAtheros8328N_port_vlan_trans_get(dev_id, port_id, entry);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Iterate all vlan translation entries from a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[in] iterator translation entry index if it's zero means get the first entry
 * @param[out] iterator next valid translation entry index
 * @param[out] entry vlan translation entry
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
cslAtheros8328N_port_vlan_trans_iterate(Uint32 dev_id, fal_port_t port_id,
                             Uint32 * iterator,
                             fal_vlan_trans_entry_t * entry)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _cslAtheros8328N_port_vlan_trans_iterate(dev_id, port_id, iterator, entry);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Set switch qinq work mode on a particular device.
 * @param[in] dev_id device id
 * @param[in] mode qinq work mode
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
cslAtheros8328N_qinq_mode_set(Uint32 dev_id, fal_qinq_mode_t mode)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _cslAtheros8328N_qinq_mode_set(dev_id, mode);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get switch qinq work mode on a particular device.
 * @param[in] dev_id device id
 * @param[out] mode qinq work mode
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
cslAtheros8328N_qinq_mode_get(Uint32 dev_id, fal_qinq_mode_t * mode)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _cslAtheros8328N_qinq_mode_get(dev_id, mode);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Set qinq role on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[in] role port role
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
cslAtheros8328N_port_qinq_role_set(Uint32 dev_id, fal_port_t port_id,
                        fal_qinq_port_role_t role)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _cslAtheros8328N_port_qinq_role_set(dev_id, port_id, role);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get qinq role on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[out] role port role
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
cslAtheros8328N_port_qinq_role_get(Uint32 dev_id, fal_port_t port_id,
                        fal_qinq_port_role_t * role)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _cslAtheros8328N_port_qinq_role_get(dev_id, port_id, role);
    HSL_API_UNLOCK;
    return rv;
}

#endif
































Int32 cslAtheros8328N_portvlan_init(CSL_SWITCH_CLBKS * p_api)
{
    Uint32 i;
    sw_error_t rv;
    fal_vlan_trans_entry_t      entry_init;

    SW_RTN_ON_NULL(p_api);

    memset(&entry_init, 0, sizeof (fal_vlan_trans_entry_t));

    for (i = 0; i < ISIS_MAX_VLAN_TRANS; i++)
    {
        rv = _cslAtheros8328N_vlan_trans_write(0, i, 0, entry_init);
        SW_RTN_ON_ERROR(rv);
    }


    p_api->port_1qmode_get =                cslAtheros8328N_port_1qmode_get;
    p_api->port_1qmode_set =                cslAtheros8328N_port_1qmode_set;
    p_api->port_egvlanmode_get =            cslAtheros8328N_port_egvlanmode_get;
    p_api->port_egvlanmode_set =            cslAtheros8328N_port_egvlanmode_set;
    p_api->portvlan_member_add =            cslAtheros8328N_portvlan_member_add;
    p_api->portvlan_member_del =            cslAtheros8328N_portvlan_member_del;
    p_api->portvlan_member_update =         cslAtheros8328N_portvlan_member_update;
    p_api->portvlan_member_get =            cslAtheros8328N_portvlan_member_get;
    p_api->port_force_default_vid_set =     cslAtheros8328N_port_force_default_vid_set;
    p_api->port_force_default_vid_get =     cslAtheros8328N_port_force_default_vid_get;
    p_api->port_force_portvlan_set =        cslAtheros8328N_port_force_portvlan_set;
    p_api->port_force_portvlan_get =        cslAtheros8328N_port_force_portvlan_get;
    p_api->port_nestvlan_tpid_set =         cslAtheros8328N_nestvlan_tpid_set;
    p_api->port_nestvlan_tpid_get =         cslAtheros8328N_nestvlan_tpid_get;
    p_api->port_invlan_mode_set =           cslAtheros8328N_port_invlan_mode_set;
    p_api->port_invlan_mode_get =           cslAtheros8328N_port_invlan_mode_get;
    p_api->port_tls_set =                   cslAtheros8328N_port_tls_set;
    p_api->port_tls_get =                   cslAtheros8328N_port_tls_get;
    p_api->port_pri_propagation_set =       cslAtheros8328N_port_pri_propagation_set;
    p_api->port_pri_propagation_get =       cslAtheros8328N_port_pri_propagation_get;
    p_api->port_default_svid_set =          cslAtheros8328N_port_default_svid_set;
    p_api->port_default_svid_get =          cslAtheros8328N_port_default_svid_get;
    p_api->port_default_cvid_set =          cslAtheros8328N_port_default_cvid_set;
    p_api->port_default_cvid_get =          cslAtheros8328N_port_default_cvid_get;
#if 0
    p_api->port_vlan_propagation_set =      cslAtheros8328N_port_vlan_propagation_set;
    p_api->port_vlan_propagation_get =      cslAtheros8328N_port_vlan_propagation_get;
    p_api->port_vlan_trans_add =            cslAtheros8328N_port_vlan_trans_add;
    p_api->port_vlan_trans_del =            cslAtheros8328N_port_vlan_trans_del;
    p_api->port_vlan_trans_get =            cslAtheros8328N_port_vlan_trans_get;
    p_api->qinq_mode_set =                  cslAtheros8328N_qinq_mode_set;
    p_api->qinq_mode_get =                  cslAtheros8328N_qinq_mode_get;
    p_api->port_qinq_role_set =             cslAtheros8328N_port_qinq_role_set;
    p_api->port_qinq_role_get =             cslAtheros8328N_port_qinq_role_get;
    p_api->port_vlan_trans_iterate =        cslAtheros8328N_port_vlan_trans_iterate;
#endif

    return SW_OK;
}

