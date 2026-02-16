/*
 *
 * csl_atheros8328N_private.h
 * Description:
 * atheros8328n driver private definitions
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

#ifndef __SWITCH_CSL_ATHEROS_8328N_PRIVATE_H__
#define __SWITCH_CSL_ATHEROS_8328N_PRIVATE_H__

#define HSL_LOCAL static
#define HSL_API_LOCK
#define HSL_API_UNLOCK
#define MDIO_LOCKER_LOCK
#define MDIO_LOCKER_UNLOCK



typedef Uint32  fal_pbmp_t;
typedef Uint32  fal_port_t;
typedef enum {
    SW_OK              = 0,       /* Operation succeeded                 */
    SW_FAIL            = -1,      /* Operation failed                    */
    SW_BAD_VALUE       = -2,      /* Illegal value                       */
    SW_OUT_OF_RANGE    = -3,      /* Value is out of range               */
    SW_BAD_PARAM       = -4,      /* Illegal parameter(s)                */
    SW_BAD_PTR         = -5,      /* Illegal pointer value               */
    SW_BAD_LEN         = -6,      /* Wrong length                        */
    SW_BAD_STATE       = -7,      /* Wrong state of state machine        */
    SW_READ_ERROR      = -8,      /* Read operation failed               */
    SW_WRITE_ERROR     = -9,      /* Write operation failed              */
    SW_CREATE_ERROR    = -10,     /* Fail in creating an entry           */
    SW_DELETE_ERROR    = -11,     /* Fail in deleteing an entry          */
    SW_NOT_FOUND       = -12,     /* Entry not found                     */
    SW_NO_CHANGE       = -13,     /* The parameter(s) is the same        */
    SW_NO_MORE         = -14,     /* No more entry found                 */
    SW_NO_SUCH         = -15,     /* No such entry                       */
    SW_ALREADY_EXIST   = -16,     /* Tried to create existing entry      */
    SW_FULL            = -17,     /* Table is full                       */
    SW_EMPTY           = -18,     /* Table is empty                      */
    SW_NOT_SUPPORTED   = -19,     /* This request is not support         */
    SW_NOT_IMPLEMENTED = -20,     /* This request is not implemented     */
    SW_NOT_INITIALIZED = -21,     /* The item is not initialized         */
    SW_BUSY            = -22,     /* Operation is still running          */
    SW_TIMEOUT         = -23,     /* Operation Time Out                  */
    SW_DISABLE         = -24,     /* Operation is disabled               */
    SW_NO_RESOURCE     = -25,     /* Resource not available (memory ...) */
    SW_INIT_ERROR      = -26,     /* Error occured while INIT process    */
    SW_NOT_READY       = -27,     /* The other side is not ready yet     */
    SW_OUT_OF_MEM      = -28,     /* Cpu memory allocation failed.       */
    SW_ABORTED         = -29      /* Operation has been aborted.         */
} sw_error_t;


#define HSL_DEV_ID_CHECK(dev_id) \
do { \
    if (dev_id >= 1) \
        return SW_OUT_OF_RANGE; \
} while (0)


#define SW_RTN_ON_ERROR(rtn) \
    do { if (rtn != 0) return(rtn); } while(0);

#define SW_OUT_ON_ERROR(rtn) \
    do { \
        if (rtn != 0) { \
            rv = rtn; \
            goto out;\
        } \
    } while(0);

#define SW_RTN_ON_ERROR_EXCEPT_COND1(rtn, cond1) \
    do { \
        if ((rtn != 0) && (rtn != cond1)) \
            return rtn; \
    }while(0);

#define SW_RTN_ON_NULL(op)  \
    do { \
        if ((op) == NULL) \
            return -1;\
    }while(0);

/* register functions */
#define SW_BIT_MASK_U32(nr) (~(0xFFFFFFFF << (nr)))

#define SW_FIELD_MASK_U32(offset, len) \
    ((SW_BIT_MASK_U32(len) << (offset)))

#define SW_FIELD_MASK_NOT_U32(offset,len) \
    (~(SW_BIT_MASK_U32(len) << (offset)))

#define SW_FIELD_2_REG(field_val, bit_offset) \
    (field_val << (bit_offset) )

#define SW_REG_2_FIELD(reg_val, bit_offset, field_len) \
    (((reg_val) >> (bit_offset)) & ((1 << (field_len)) - 1))

#define SW_REG_SET_BY_FIELD_U32(reg_value, field_value, bit_offset, field_len)\
    do { \
        (reg_value) = \
            (((reg_value) & SW_FIELD_MASK_NOT_U32((bit_offset),(field_len))) \
              | (((field_value) & SW_BIT_MASK_U32(field_len)) << (bit_offset)));\
    } while (0)

#define SW_FIELD_GET_BY_REG_U32(reg_value, field_value, bit_offset, field_len)\
    do { \
        (field_value) = \
            (((reg_value) >> (bit_offset)) & SW_BIT_MASK_U32(field_len)); \
    } while (0)

#define SW_SWAP_BITS_U8(x)         \
    ((((x)&0x80)>>7) | (((x)&0x40)>>5) | (((x)&0x20)>>3) | (((x)&0x10)>>1) \
      |(((x)&0x1)<<7) | (((x)&0x2)<<5) | (((x)&0x4)<<3) |(((x)&0x8)<<1) )


#define SW_OFFSET_U8_2_U16(byte_offset)   ((byte_offset) >> 1)

#define SW_OFFSET_U16_2_U8(word16_offset)   ((word16_offset) << 1)

#define SW_OFFSET_BIT_2_U8_ALIGN16(bit_offset)   (((bit_offset) / 16) * 2)

#define SW_SET_REG_BY_FIELD(reg, field, field_value, reg_value) \
    SW_REG_SET_BY_FIELD_U32(reg_value, field_value, reg##_##field##_BOFFSET, \
                            reg##_##field##_BLEN)

#define SW_GET_FIELD_BY_REG(reg, field, field_value, reg_value) \
    SW_FIELD_GET_BY_REG_U32(reg_value, field_value, reg##_##field##_BOFFSET, \
                            reg##_##field##_BLEN)

/* port bitmap functions */
#define SW_IS_PBMP_MEMBER(pbm, port)  ((pbm & (1 << port)) ? True: False)
#define SW_IS_PBMP_EQ(pbm0, pbm1)  ((pbm0 == pbm1) ? True: False)

#define SW_PBMP_AND(pbm0, pbm1)  ((pbm0) &= (pbm1))
#define SW_PBMP_OR(pbm0, pbm1)  ((pbm0) |= (pbm1))
#define SW_IS_PBMP_INCLUDE(pbm0, pbm1) \
    ((pbm1 == SW_PBMP_AND(pbm0, pbm1)) ? True: False)

#define SW_PBMP_CLEAR(pbm) ((pbm) = 0)
#define SW_PBMP_ADD_PORT(pbm, port) ((pbm) |= (1U << (port)))
#define SW_PBMP_DEL_PORT(pbm,port) ((pbm) &= ~(1U << (port)))


#define HSL_REG_ENTRY_GET(rv, dev, reg, index, value, val_len) \
do { \
        rv = cslAtheros8328n_reg_get(dev, reg##_OFFSET + ((Uint32)index) * reg##_E_OFFSET,\
                                   (Uint8*)value, (Uint8)val_len); \
} while (0);

#define HSL_REG_ENTRY_SET(rv, dev, reg, index, value, val_len) \
do { \
        rv = cslAtheros8328n_reg_set (dev, reg##_OFFSET + ((Uint32)index) * reg##_E_OFFSET,\
                                   (Uint8*)value, (Uint8)val_len); \
} while (0);

#define HSL_REG_FIELD_GET(rv, dev, reg, index, field, value, val_len) \
do { \
        rv = cslAtheros8328n_reg_field_get(dev, reg##_OFFSET + ((Uint32)index) * reg##_E_OFFSET,\
                                  reg##_##field##_BOFFSET, \
                                  reg##_##field##_BLEN, (Uint8*)value, val_len);\
} while (0);

#define HSL_REG_FIELD_SET(rv, dev, reg, index, field, value, val_len) \
do { \
        rv = cslAtheros8328n_reg_field_set(dev, reg##_OFFSET + ((Uint32)index) * reg##_E_OFFSET,\
                                  reg##_##field##_BOFFSET, \
                                  reg##_##field##_BLEN, (Uint8*)value, val_len);\
} while (0);

#define HSL_REG_ENTRY_GEN_GET(rv, dev, addr, reg_len, value, val_len) \
do { \
        rv = cslAtheros8328n_reg_get(dev, addr, (Uint8*)value, val_len);\
} while (0);

#define HSL_REG_ENTRY_GEN_SET(rv, dev, addr, reg_len, value, val_len) \
do { \
        rv = cslAtheros8328n_reg_set(dev, addr, (Uint8*)value, val_len); \
} while (0);

#define HSL_REG_FIELD_GEN_GET(rv, dev, regaddr, bitlength, bitoffset, value, val_len) \
do { \
        rv = cslAtheros8328n_reg_field_get(dev, regaddr, bitoffset, bitlength, \
                                   (Uint8 *) value, val_len);\
} while (0);

#define HSL_REG_FIELD_GEN_SET(rv, dev, regaddr, bitlength, bitoffset, value, val_len) \
do { \
        rv = cslAtheros8328n_reg_field_set(dev, regaddr, bitoffset, bitlength, \
                                   (Uint8 *) value, val_len);\
} while (0);

#define HSL_PHY_GET(rv, dev, phy_addr, reg, value) \
do { \
        rv = cslAtheros8328n_phy_get(dev, phy_addr, reg, value); \
} while (0);

#define HSL_PHY_SET(rv, dev, phy_addr, reg, value) \
do { \
        rv = cslAtheros8328n_phy_set(dev, phy_addr, reg, value); \
} while (0);



/**
  @details  Fields description:

 o_vid - original vlan id
 s_vid - service vid id
 c_vid - custom vid id
 bi_dir - entry search direction
 forward_dir - entry search direction only be forward
 reverse_dir - entry search direction only be reverse
 o_vid_is_cvid - o_vid in entry means c_vid not s_vid
 s_vid_enable  - s_vid in entry is valid
 c_vid_enable  - c_vid in entry is valid
 one_2_one_vlan- the entry used for 1:1 vlan
@brief This structure defines the vlan translation entry.

*/
typedef struct {
    Uint32      o_vid;
    Uint32      s_vid;
    Uint32      c_vid;
    Bool        bi_dir;       /**< lookup can be forward and reverse*/
    Bool        forward_dir;  /**< lookup direction only can be from o_vid to s_vid and/or c_vid*/
    Bool        reverse_dir;  /**< lookup direction only can be from s_vid and/or c_vid to o_vid*/
    Bool        o_vid_is_cvid;
    Bool        s_vid_enable;
    Bool        c_vid_enable;
    Bool        one_2_one_vlan;
}fal_vlan_trans_entry_t;

/**
@brief This enum defines qinq working mode.
*/
typedef enum {
    FAL_QINQ_CTAG_MODE = 0,
    FAL_QINQ_STAG_MODE,
    FAL_QINQ_MODE_BUTT
}fal_qinq_mode_t;

/**
@brief This enum defines port role in qinq mode.
*/
typedef enum {
    FAL_QINQ_EDGE_PORT = 0,
    FAL_QINQ_CORE_PORT,
    FAL_QINQ_PORT_ROLE_BUTT
}fal_qinq_port_role_t;




#endif  /* __SWITCH_CSL_ATHEROS_8328N_PRIVATE_H__ */
