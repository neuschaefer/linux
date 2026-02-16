/*
 * pal_vlynqIoctl.c
 * Description:
 * See below.
 *
 *
 * Copyright (C) 2008, Texas Instruments, Incorporated
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 */

/******************************************************************************
 * FILE PURPOSE:    PAL VLYNQ IOCTL File
 ******************************************************************************
 * FILE NAME:       pal_vlynqIoctl.c
 *
 * DESCRIPTION:     PAL VLYNQ IOCTL File
 *
 *******************************************************************************/

#include "pal_vlynq.h"
#include "pal_vlynqIoctl.h"

#define IOCTL_WR         0x01

typedef struct pal_vlynq_ioctl_info
{
    Int16         id;
    Uint16        offset; /* Register offset or bit offset. */
    const char    *name;
    Uint32        mask;
    Uint32        start_revision;
    Uint32        end_revision;
    Uint8         access_flags; /* 0x01 - write, otherwise read, can be enhanced later. */

} PAL_VLYNQ_IOCTL_INFO_T;

static PAL_VLYNQ_IOCTL_INFO_T control_reg_ioctl[ ] = {

    { PAL_VLYNQ_IOCTL_CNT_RESET_CMD,       0, "reset"        , 0x00000001, 0x00010200, LATEST_VLYNQ_REV, 0        },
    { PAL_VLYNQ_IOCTL_CNT_ILOOP_CMD,       1, "iloop"        , 0x00000002, 0x00010100, LATEST_VLYNQ_REV, 0        },
    { PAL_VLYNQ_IOCTL_CNT_AOPT_CMD,        2, "aopt disable" , 0x00000004, 0x00010200, LATEST_VLYNQ_REV, IOCTL_WR },
    { PAL_VLYNQ_IOCTL_CNT_INT2CFG_CMD,     7, "int2cfg"      , 0x00000080, 0x00010100, LATEST_VLYNQ_REV, 0        },
    { PAL_VLYNQ_IOCTL_CNT_INTVEC_CMD,      8, "intvec"       , 0x00001f00, 0x00010100, LATEST_VLYNQ_REV, 0        },
    { PAL_VLYNQ_IOCTL_CNT_INT_EN_CMD,     13, "intenable"    , 0x00002000, 0x00010100, LATEST_VLYNQ_REV, IOCTL_WR },
    { PAL_VLYNQ_IOCTL_CNT_INT_LOC_CMD,    14, "intlocal"     , 0x00004000, 0x00010100, LATEST_VLYNQ_REV, 0        },
    { PAL_VLYNQ_IOCTL_CNT_CLK_DIR_CMD,    15, "clkdir"       , 0x00008000, 0x00010100, LATEST_VLYNQ_REV, 0        },
    { PAL_VLYNQ_IOCTL_CNT_CLK_DIV_CMD,    16, "clkdiv"       , 0x00070000, 0x00010100, LATEST_VLYNQ_REV, IOCTL_WR },
    { PAL_VLYNQ_IOCTL_CNT_CLK_MOD_CMD,    21, "mode"         , 0x00e00000, 0x00010100, 0x000101ff      , 0        },
    { PAL_VLYNQ_IOCTL_CNT_TX_FAST_CMD,    21, "txfastpath"   , 0x00200000, 0x00010205, LATEST_VLYNQ_REV, 0        },
    { PAL_VLYNQ_IOCTL_CNT_RTM_SELECT_CMD, 22, "rtmenable"    , 0x00400000, 0x00010205, LATEST_VLYNQ_REV, 0        },
    { PAL_VLYNQ_IOCTL_CNT_RTM_SAMPLE_CMD, 23, "wr-rtmval"    , 0x07800000, 0x00010205, LATEST_VLYNQ_REV, 0        },
    { PAL_VLYNQ_IOCTL_CNT_CLK_SLKPU_CMD,  30, "slkpudis"     , 0x40000000, 0x00010204, LATEST_VLYNQ_REV, IOCTL_WR },
    { PAL_VLYNQ_IOCTL_CNT_PMEM_CMD,       31, "pmem"         , 0x80000000, 0x00010200, LATEST_VLYNQ_REV, IOCTL_WR },
    {  -1,                                32, "dead end"     , 0xdeadfeed, 0x00000000, 0xdeadfeed      , 0        }

};

static PAL_VLYNQ_IOCTL_INFO_T status_reg_ioctl[ ] = {

    { PAL_VLYNQ_IOCTL_STS_LINK,     0, "link"         , 0x00000001, 0x00010100, LATEST_VLYNQ_REV, 0 },
    { PAL_VLYNQ_IOCTL_STS_MPEND,    1, "mpend"        , 0x00000002, 0x00010100, LATEST_VLYNQ_REV, 0 },
    { PAL_VLYNQ_IOCTL_STS_SPEND,    2, "spend"        , 0x00000004, 0x00010100, LATEST_VLYNQ_REV, 0 },
    { PAL_VLYNQ_IOCTL_STS_NFEMP0,   3, "nfempty0"     , 0x00000008, 0x00010100, LATEST_VLYNQ_REV, 0 },
    { PAL_VLYNQ_IOCTL_STS_NFEMP1,   4, "nfempty1"     , 0x00000010, 0x00010100, LATEST_VLYNQ_REV, 0 },
    { PAL_VLYNQ_IOCTL_STS_NFEMP2,   5, "nfempty2"     , 0x00000020, 0x00010100, LATEST_VLYNQ_REV, 0 },
    { PAL_VLYNQ_IOCTL_STS_NFEMP3,   6, "nfempty3"     , 0x00000040, 0x00010100, LATEST_VLYNQ_REV, 0 },
    { PAL_VLYNQ_IOCTL_STS_LERR,     7, "lerror"       , 0x00000080, 0x00010100, LATEST_VLYNQ_REV, 0 },
    { PAL_VLYNQ_IOCTL_STS_RERR,     8, "rerror"       , 0x00000100, 0x00010100, LATEST_VLYNQ_REV, 0 },
    { PAL_VLYNQ_IOCTL_STS_OFLOW,    9, "oflow"        , 0x00000200, 0x00010100, LATEST_VLYNQ_REV, 0 },
    { PAL_VLYNQ_IOCTL_STS_IFLOW,   10, "iflow"        , 0x00000400, 0x00010100, LATEST_VLYNQ_REV, 0 },
    { PAL_VLYNQ_IOCTL_STS_RTM,     11, "rtm"          , 0x00000800, 0x00010205, LATEST_VLYNQ_REV, 0 },
    { PAL_VLYNQ_IOCTL_STS_RTM_VAL, 12, "rtmcurrval"   , 0x00007000, 0x00010205, LATEST_VLYNQ_REV, 0 },
    { PAL_VLYNQ_IOCTL_STS_SWIDOUT, 20, "swidthout"    , 0x00f00000, 0x00010200, LATEST_VLYNQ_REV, 0 },
    { PAL_VLYNQ_IOCTL_STS_MODESUP, 21, "modesup"      , 0x00700000, 0x00010100, LATEST_VLYNQ_REV, 0 },
    { PAL_VLYNQ_IOCTL_STS_SWIDTH,  24, "swidth"       , 0x03000000, 0x00010100, 0x00010200      , 0 },
    { PAL_VLYNQ_IOCTL_STS_SWIDIN,  24, "swidthin"     , 0x0f000000, 0x00010200, LATEST_VLYNQ_REV, 0 },
    { PAL_VLYNQ_IOCTL_STS_DEBUG,   29, "debug"        , 0x70000000, 0x00010100, 0x000101ff      , 0 },
    {  -1,                         32, "dead end"     , 0xdeadfeed, 0x00000000, 0xdeadfeed      , 0 }

};

static PAL_VLYNQ_IOCTL_INFO_T vlynq_reg32_ioctl[ ] = {

    { PAL_VLYNQ_IOCTL_REVSION_REG,  0x00, "revision"     , 0xffffffff, 0x00010100, LATEST_VLYNQ_REV, 0        },
    { PAL_VLYNQ_IOCTL_CONTROL_REG,  0x04, "control"      , 0xffffffff, 0x00010100, LATEST_VLYNQ_REV, 0        },
    { PAL_VLYNQ_IOCTL_STATUS_REG,   0x08, "status"       , 0xffffffff, 0x00010100, LATEST_VLYNQ_REV, 0        },
    { PAL_VLYNQ_IOCTL_INT_PRIR_REG, 0x0c, "intPriority"  , 0xffffffff, 0x00010200, LATEST_VLYNQ_REV, 0        },
    { PAL_VLYNQ_IOCTL_INT_STS_REG,  0x10, "intStatus"    , 0xffffffff, 0x00010100, LATEST_VLYNQ_REV, 0        },
    { PAL_VLYNQ_IOCTL_INT_PEND_REG, 0x14, "intPending"   , 0xffffffff, 0x00010100, LATEST_VLYNQ_REV, 0        },
    { PAL_VLYNQ_IOCTL_INT_PTR_REG,  0x18, "intPtr"       , 0xffffffff, 0x00010100, LATEST_VLYNQ_REV, 0        },
    { PAL_VLYNQ_IOCTL_TX_MAP_REG,   0x1c, "txMap"        , 0xffffffff, 0x00010100, LATEST_VLYNQ_REV, 0        },
    { PAL_VLYNQ_IOCTL_RX1_SZ_REG,   0x20, "rxSize1"      , 0xffffffff, 0x00010100, LATEST_VLYNQ_REV, 0        },
    { PAL_VLYNQ_IOCTL_RX1_OFF_REG,  0x24, "rxoffset1"    , 0xffffffff, 0x00010100, LATEST_VLYNQ_REV, 0        },
    { PAL_VLYNQ_IOCTL_RX2_SZ_REG,   0x28, "rxSize2"      , 0xffffffff, 0x00010100, LATEST_VLYNQ_REV, 0        },
    { PAL_VLYNQ_IOCTL_RX2_OFF_REG,  0x2c, "rxoffset2"    , 0xffffffff, 0x00010100, LATEST_VLYNQ_REV, 0        },
    { PAL_VLYNQ_IOCTL_RX3_SZ_REG,   0x30, "rxSize3"      , 0xffffffff, 0x00010100, LATEST_VLYNQ_REV, 0        },
    { PAL_VLYNQ_IOCTL_RX3_OFF_REG,  0x34, "rxoffset3"    , 0xffffffff, 0x00010100, LATEST_VLYNQ_REV, 0        },
    { PAL_VLYNQ_IOCTL_RX4_SZ_REG,   0x38, "rxSize4"      , 0xffffffff, 0x00010100, LATEST_VLYNQ_REV, 0        },
    { PAL_VLYNQ_IOCTL_RX4_OFF_REG,  0x3c, "rxoffset4"    , 0xffffffff, 0x00010100, LATEST_VLYNQ_REV, 0        },
    { PAL_VLYNQ_IOCTL_CVR_REG,      0x40, "chipVersion"  , 0xffffffff, 0x00010100, LATEST_VLYNQ_REV, 0        },
    { PAL_VLYNQ_IOCTL_AUTO_NEG_REG, 0x44, "autoNegn"     , 0xffffffff, 0x00010200, LATEST_VLYNQ_REV, 0        },
    { PAL_VLYNQ_IOCTL_MAN_NEG_REG,  0x48, "manualNegn"   , 0xffffffff, 0x00010200, LATEST_VLYNQ_REV, 0        },
    { PAL_VLYNQ_IOCTL_NEG_STS_REG,  0x4c, "negnStatus"   , 0xffffffff, 0x00010200, LATEST_VLYNQ_REV, 0        },
    { PAL_VLYNQ_IOCTL_ENDIAN_REG,   0x5c, "endian"       , 0xffffffff, 0x00010200, 0x00010204,       0        },
    { PAL_VLYNQ_IOCTL_IVR30_REG,    0x60, "ivr30"        , 0xffffffff, 0x00010100, LATEST_VLYNQ_REV, 0        },
    { PAL_VLYNQ_IOCTL_IVR74_REG,    0x64, "ivr74"        , 0xffffffff, 0x00010100, LATEST_VLYNQ_REV, 0        },
    { -1,                             32, "dead end"     , 0xdeadfeed, 0x00000000, 0xdeadfeed                 }
};

extern Bool   vlynq_swap_hack;
extern Uint32 vlynq_endian_swap(Uint32);

/* Definitely, not re-entrant. */
static Uint32 vlynq_check_and_swap(Uint32 val)
{
    if(vlynq_swap_hack == True) 
        val = vlynq_endian_swap(val);

    return (val);	
}

static Int32 vlynq_dump_cmd(PAL_VLYNQ_IOCTL_INFO_T *vlynq_ioctl, volatile Uint32 *p_start_reg, 
                            char *buf, Int32 limit, Uint32 revision, Bool bit_op)
{
    Int32 len = 0;
    PAL_VLYNQ_IOCTL_INFO_T *p_ioctl;

    for(p_ioctl = vlynq_ioctl; p_ioctl->id > -1; p_ioctl++)
    {
        Uint32 val = 0;

        if(revision < p_ioctl->start_revision || 
           revision > p_ioctl->end_revision)
            continue;

        if(len < limit)
            len += sprintf(buf + len, "%20s : ", p_ioctl->name);
        else
            break;

        if(bit_op)
            val = (vlynq_check_and_swap((*p_start_reg)) & p_ioctl->mask) >> p_ioctl->offset;
        else
            val =  vlynq_check_and_swap(*(p_start_reg + p_ioctl->offset/0x04)) & p_ioctl->mask;

        if(len < limit)
            len += sprintf(buf + len, "0x%08x.\n", val);
        else
            break;
    }

    return (len);
}


static PAL_Result vlynq_write_cmd(PAL_VLYNQ_IOCTL_INFO_T *vlynq_ioctl, volatile Uint32 *p_reg, 
                                  Uint32 cmd, Uint32 val, Uint32 revision, Bool bit_op)
{
    PAL_Result ret_val = PAL_VLYNQ_INVALID_PARAM;
    PAL_VLYNQ_IOCTL_INFO_T *p_ioctl;

    for(p_ioctl = vlynq_ioctl; p_ioctl->id > -1; p_ioctl++)
    {
        if(revision < p_ioctl->start_revision ||
           revision > p_ioctl->end_revision)
            continue;

        if(cmd != p_ioctl->id)
            continue;

	/* Can we write ? */
	if(!(p_ioctl->access_flags & IOCTL_WR))
	    break;

        /* Hack. One of the case. We need to enhance the data structure for 
         * generic implementation. */
        if(cmd == PAL_VLYNQ_IOCTL_CNT_CLK_DIV_CMD)
        {
            if(val != 0)
                val -= 1; /* Adjusting the clock divisor. */
            else
                break;
        }
       
        if(bit_op)
        {
            *p_reg &= vlynq_check_and_swap(~p_ioctl->mask);
            *p_reg |= vlynq_check_and_swap((val << p_ioctl->offset) & p_ioctl->mask);
        }    
        else
        {
            *(p_reg + p_ioctl->offset/0x04) = vlynq_check_and_swap(val & p_ioctl->mask);
        }

        ret_val = PAL_VLYNQ_OK;
        break;
    }

    return (ret_val);
}

static PAL_Result vlynq_read_cmd(PAL_VLYNQ_IOCTL_INFO_T *vlynq_ioctl, volatile Uint32 *p_reg, 
                                 Uint32 cmd, Uint32 *val, Uint32 revision, Bool bit_op)
{
    PAL_Result ret_val = PAL_VLYNQ_INVALID_PARAM;
    PAL_VLYNQ_IOCTL_INFO_T *p_ioctl;

    for(p_ioctl = vlynq_ioctl; p_ioctl->id > -1; p_ioctl++)
    {
        if(revision < p_ioctl->start_revision ||
           revision > p_ioctl->end_revision)
            continue;

        if(cmd != p_ioctl->id)
            continue;

        if(bit_op)
            *val = (vlynq_check_and_swap(*p_reg) & p_ioctl->mask) >> p_ioctl->offset;
        else
            *val = vlynq_check_and_swap(*(p_reg + p_ioctl->offset/0x4)) & p_ioctl->mask;

        ret_val = PAL_VLYNQ_OK;
        break;
    }

    return(ret_val);
}

PAL_Result vlynq_read_write_ioctl(volatile Uint32 *p_start_reg, Uint32 cmd, 
                                  Uint32 val, Uint32 vlynq_rev)
{
    Bool        read_flag = 0;
    Uint32      sub_cmd   = PAL_VLYNQ_IOCTL_MINOR_DE_VAL(cmd);
    PAL_Result  ret_val   = PAL_VLYNQ_INVALID_PARAM; 
    Int32       valid     = 1;
    Bool        bit_op    = 1;

    PAL_VLYNQ_IOCTL_INFO_T *p_ioctl = NULL;

    if(cmd & PAL_VLYNQ_IOCTL_READ_CMD)
        read_flag = 1;

    /* 
     * Since we are supporting just one non bit operation, we are hacking 
     * the implementation. Ideally, we need to have a separate function.
     */

    if(!(cmd & PAL_VLYNQ_IOCTL_BIT_CMD))
    {
        bit_op = 0;

        if(!PAL_VLYNQ_IOCTL_MAJOR_DE_VAL(cmd))
            p_ioctl = vlynq_reg32_ioctl;
        else
            valid = 0;
    }
    else
    {
        /* We are doing bit accesses. */
        switch(PAL_VLYNQ_IOCTL_MAJOR_DE_VAL(cmd))
        {
            case PAL_VLYNQ_IOCTL_CONTROL_REG:
            
                p_ioctl = control_reg_ioctl;
                break;

            case PAL_VLYNQ_IOCTL_STATUS_REG: 

                p_ioctl = status_reg_ioctl;
                break;

            default:

                valid = 0;
                break;
        }
    }

    if(valid)
    {
        if(read_flag)
            ret_val = vlynq_read_cmd(p_ioctl, p_start_reg, sub_cmd,
                                     (Uint32*)val, vlynq_rev, bit_op);
        else
            ret_val = vlynq_write_cmd(p_ioctl, p_start_reg, sub_cmd,
                                      val, vlynq_rev, bit_op);
    }


    return(ret_val);
}

Int32 vlynq_dump_ioctl(volatile Uint32 *start_reg, Uint32 dump_type, char *buf, 
                       Int32 limit, Uint32 vlynq_rev)   
{
    Int32       len = 0;
    Uint32 revision = vlynq_rev;

    switch(dump_type)
    {
        case PAL_VLYNQ_DUMP_STS_REG: 
        {
            if(len <  limit)
                len += vlynq_dump_cmd(status_reg_ioctl, start_reg, buf + len,
                                      limit - len, revision, 1);
            break;              
        }

        case PAL_VLYNQ_DUMP_CNTL_REG:
        {
            if(len < limit)
                len += vlynq_dump_cmd(control_reg_ioctl, start_reg, buf + len, 
                                      limit - len, revision, 1);
            break;
        }

        case PAL_VLYNQ_DUMP_ALL_REGS:
        {
            if(len < limit)
                len += vlynq_dump_cmd(vlynq_reg32_ioctl, start_reg, buf + len, 
                                      limit - len, revision, 0);
            break;
        }

        default:
            break;
    }

    return (len);
}
	
