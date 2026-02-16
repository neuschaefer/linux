/*
 * ti_ppd.c
 *  Description:
 *  Packet processor driver file.
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


#include <asm-arm/arch-avalanche/generic/pal_cppi41.h>

#include <asm-arm/arch-avalanche/generic/ti_ppd.h>
#include "ppd_pvt.h"
#include "ppd_os.h"
#include <linux/in.h>
#include <linux/if_ether.h>
#include <linux/if_vlan.h>
#include <linux/ppp_defs.h>


/* Forward declaration */
static void  ppd_event_timer(unsigned long data);

/* Flags used for validity checks in PPD */
Uint32 g_ppd_init_done    = 0;
Uint32 g_session_slots [TI_PP_MAX_ACCLERABLE_SESSIONS/32];

#ifdef PPD_ENABLE_LUT_DUMP
Uint32 g_lut_dump_buff [TI_PP_MAX_ACCLERABLE_SESSIONS][PPD_LUT_DUMP_WORDS];
#endif /* PPD_ENABLE_LUT_DUMP */

#if ((PPD_DEBUG) & PPD_DEBUG_MSG)
static char dump_buff[1000];
#endif



/*
 * PSM mode enable status. Checked by various timers to avoid scheduling further
 * timer
 */
Uint32 g_is_in_psm = 0;

ppd_ses_info_blk_t *gp_ses_blk = 0;

typedef union block32
{
    union block32 *next;
    Uint8         data[32];

} ppd_block32_t;

static ppd_block32_t *gp_free_blk32_list = 0;

static Int32 ppd_init_blk32(ppd_block32_t *last_blk32_addr)
{
    ppd_block32_t *p_blk32 = gp_free_blk32_list;

    for(; p_blk32  < last_blk32_addr; p_blk32++)
    {
        p_blk32->next = p_blk32 + 1;
//  p_blk32       = p_blk32->next;
    }

    p_blk32->next     = 0;

    return (0);
}

static inline union block32* ppd_get_free_blk32(void)
{
    ppd_block32_t *p_blk32;
    Uint32 cookie;

    PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &cookie);
    p_blk32 = gp_free_blk32_list;

    if(p_blk32) gp_free_blk32_list = p_blk32->next;

    PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);
    return (p_blk32);
}

static inline void ppd_free_blk32(union block32 *p_blk32)
{
    Uint32 cookie;

    PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &cookie);
    p_blk32->next      = gp_free_blk32_list;
    gp_free_blk32_list = p_blk32;
    PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);
}

/* Note that size of ppd_l2_frame has to be aligned to 2^n, meaning that the
 * size of the following structure can not be anything but has to be one of
 * 16, 32, 64, 128, 256, .......
 *
 * Currently, the size of 128 B is being used.
 *
 * If the structure is not aligned this way, the implemented logic will not
 * work...
 */
typedef struct ppd_l2_frame
{
#define PPD_L2_HBLKSZ_MASK      (sizeof(ppd_l2_frame_t) - 1)
#define PPD_L2_FRM_MAX_HLEN     (116)
#define PPD_L2_FRM_HLEN(p)      ((Uint32)p & PPD_L2_HBLKSZ_MASK)

    union
    {
        Uint8  buf[PPD_L2_FRM_MAX_HLEN];
        struct ppd_l2_frame *next;
    } u;

    Uint8   *data;
    Uint8   *end;
    Uint8   len;
    Uint8   pad[3];

} ppd_l2_frame_t;

#define PPD_HBLK_POOL_SZ    (TI_PP_MAX_EGRESS_PROPERTY \
                                * TI_PP_MAX_ACCLERABLE_SESSIONS)
#define PPD_HBLK_LAST_BLK   ((Uint32)(gp_hblk_pool + PPD_HBLK_POOL_SZ - 1))

static Uint8 hblk_pool [(PPD_HBLK_POOL_SZ+1)*sizeof(ppd_l2_frame_t)];
static ppd_l2_frame_t* gp_hblk_pool = (ppd_l2_frame_t*)hblk_pool;

static Int32 ppd_init_hblk(void)
{
    ppd_l2_frame_t *p_hblk;

    /* Ensure that we start of with aligned memory location. */
    gp_hblk_pool =
        (ppd_l2_frame_t*) ((Uint32)(gp_hblk_pool + 1) & (~PPD_L2_HBLKSZ_MASK));

    for(p_hblk = gp_hblk_pool; (Uint32)p_hblk  < PPD_HBLK_LAST_BLK; p_hblk++)
        p_hblk->u.next = p_hblk + 1;

    printMsg ("%s: p_hblk = %#x, gp_hblk_pool = %#x\n", __FUNCTION__,
            (Uint32)p_hblk, (Uint32)gp_hblk_pool);

    p_hblk->u.next     = 0;

    return (0);
}







static inline void ppd_wr_word(volatile Uint32 *ram_ptr, Uint32 *word)
{
    *ram_ptr = *word;
}

static inline void ppd_rd_word(volatile Uint32 *ram_ptr, Uint32 *word)
{
    *word = *ram_ptr;
}

/*
 *                  PP init/management APIs
 */
typedef Uint32 pdsp_cmd_t;
typedef pdsp_cmd_t cpdsp_cmd_t;
typedef pdsp_cmd_t mpdsp_cmd_t;
typedef pdsp_cmd_t qpdsp_cmd_t;

typedef volatile struct pdsp_info pdsp_info_t;

typedef Int32 (*pdsp_trg_cmd_func_t)(pdsp_info_t*, pdsp_cmd_t);
typedef Int32 (*pdsp_rsp_sts_func_t)(pdsp_info_t*);

/* volatile */ struct pdsp_info
{
    Uint32              id;
    Uint32 *            cmd_rsp_ram;
    Uint32 *            param_ram;
    Uint32              wait_count;
    Uint32 *            isr_reg;
    Uint32              isr_mask;

    pdsp_trg_cmd_func_t trg_cmd;
    pdsp_rsp_sts_func_t rsp_sts;

};

typedef  volatile struct sram_info
{
    Uint32 *            sram;
    Uint32              sram_size;
}
scr_ram_info_t;



static inline Int32 pdsp_trigger_cmd(pdsp_info_t *pdsp, pdsp_cmd_t cmd)
{
    *pdsp->cmd_rsp_ram = cmd;
    if (pdsp->isr_reg)
    {
        *pdsp->isr_reg = pdsp->isr_mask;
    }
    return 0;
}

static inline Int32 pdsp_rsp_status(pdsp_info_t *pdsp)
{
    Uint32 wait_count = pdsp->wait_count;

    if (pdsp->isr_reg)
    {
        while((*pdsp->isr_reg & pdsp->isr_mask) && wait_count)
        {
            wait_count--;
        }
    }
    else
    {
        while((*pdsp->cmd_rsp_ram & 0xff) && wait_count)
        {
            wait_count--;
        }
    }

    if(!wait_count)
    {
        printErr ("pdsp_rsp_status: Timeout waiting for PDSDP(%d).\n", pdsp->id);
        return (-1);
    }

    return (*pdsp->param_ram);
}

static scr_ram_info_t g_ram_info[] =
{
    { .sram = (Uint32*)ppd_os_get_io_virt(0x03100000),   .sram_size = 0x8000 },
    { .sram = (Uint32*)ppd_os_get_io_virt(0x03110000),   .sram_size = 0x8000 },
    { .sram = (Uint32*)ppd_os_get_io_virt(0x03120000),   .sram_size = 0x8000 },
};

static pdsp_info_t g_pdsp_info[ ] =
{
    {   /* BOOTS - CPDSP */
        .id             = TI_PP_BOOTS_FW                                /*  id  */,
        .cmd_rsp_ram    = (Uint32*)ppd_os_get_io_virt(0x0300c000)       /* cmd_rsp */,
        .param_ram      = (Uint32*)ppd_os_get_io_virt(0x03100004)       /* param ram */,
        .wait_count     = 10000000                                      /* wait count */,
        .trg_cmd        = pdsp_trigger_cmd              ,
        .rsp_sts        = pdsp_rsp_status
    },
    {   /* CPDSP */
        .id             = TI_PP_CPDSP_FW                                /*  id  */,
        .cmd_rsp_ram    = (Uint32*)ppd_os_get_io_virt(0x0300c000)       /* cmd_rsp */,
        .param_ram      = (Uint32*)ppd_os_get_io_virt(0x03100004)       /* param ram */,
        .wait_count     = 10000000                                      /* wait count */,
        .trg_cmd        = pdsp_trigger_cmd              ,
        .rsp_sts        = pdsp_rsp_status
    },

    {   /* MPDSP */
        .id             = TI_PP_MPDSP_FW                                /*  id  */,
        .cmd_rsp_ram    = (Uint32*)ppd_os_get_io_virt(0x0300C100)       /* cmd_rsp */,
        .param_ram      = (Uint32*)ppd_os_get_io_virt(0x03110004)       /* param ram */,
        .wait_count     = 10000000                                      /* wait count */,
        .trg_cmd        = pdsp_trigger_cmd              ,
        .rsp_sts        = pdsp_rsp_status
    },
    {   /* QPDSP */
        .id             = TI_PP_QPDSP_FW                                /*  id  */,
        .cmd_rsp_ram    = (Uint32*)ppd_os_get_io_virt(0x0300C200)       /* cmd_rsp */,
        .param_ram      = (Uint32*)ppd_os_get_io_virt(0x03120004)       /* param ram */,
        .wait_count     = 10000000                                      /* wait count */,
        .trg_cmd        = pdsp_trigger_cmd              ,
        .rsp_sts        = pdsp_rsp_status
    },
};

PDSP_RegsOvly g_pdsp_regs [PP_NUM_PDSP] = {

    (PDSP_RegsOvly)ppd_os_get_io_virt(0x0300D000) /* CPDSP */,
    (PDSP_RegsOvly)ppd_os_get_io_virt(0x0300D100) /* MPDSP */,
    (PDSP_RegsOvly)ppd_os_get_io_virt(0x0300D200) /* QPDSP */
};


ppd_pdsp_counts     g_pdsp_counts [PP_NUM_PDSP];
ppd_ses_timestamp   g_ses_timestamp [TI_PP_MAX_ACCLERABLE_SESSIONS];

#define SIZE_IN_WORD(p) ((sizeof(p) + 0x3) >> 2)

#define WORD_B3_0(b3, b2, b1, b0) (((Uint8)b3 << 24) | ((Uint8)b2 << 16) | ((Uint8)b1 << 8) | ((Uint8)b0))
#define WORD_B1_0_S0(b1, b0, s0)  (((Uint8)b1 << 24) | ((Uint8)b0 << 16) | ((Uint16)s0))
#define WORD_S0_B1_0(s0, b1, b0)  (((Uint16)s0 << 16) | ((Uint8)b1 <<  8)| ((Uint8)b0))
#define WORD_S1_0(s1, s0)         (((Uint16)s1 << 16) | ((Uint16)s0))
#define WORD_W0(w0)               ((Uint32)w0)



static inline Int32 pdsp_cmd_rsp_by_sz(pdsp_info_t *pdsp, pdsp_cmd_t cmd_word, void *wr_ptr, Uint32 wr_word, void *rd_ptr, Uint32 rd_word)
{
    volatile Uint32 *param_buf    = pdsp->param_ram;
    Uint32* wr_wptr = (Uint32*) wr_ptr;
    Uint32* rd_wptr = (Uint32*) rd_ptr;
    Uint32 cookie, ret_code;

    PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &cookie);

    printCmd("COMMAND: %#x = %#x: ", (Uint32)pdsp->cmd_rsp_ram, (Uint32)cmd_word);
    while(wr_word--)
    {
        printCmd ("%#x = %#x ", (Uint32)param_buf, *wr_wptr);
        *param_buf++ = *wr_wptr++;
    }
    printCmd ("\n");

    pdsp->trg_cmd(pdsp, cmd_word);

    if(-1 == pdsp->rsp_sts(pdsp))
    {
        PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);
        return (-1);
    }

    param_buf    = pdsp->param_ram + 1;
    while(rd_word--)
        *rd_wptr++ = *param_buf++;

    ret_code = *pdsp->param_ram;
    PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);

    return ((ret_code == SR_RETCODE_SUCCESS) ? 0 : -ret_code);
}

static Int32 cpdsp_cmd_rsp_by_sz (cpdsp_cmd_t cmd_word, void *wr_ptr, Uint32 wr_word, void *rd_ptr, Uint32 rd_word) 
{
    pdsp_info_t *pdsp = g_pdsp_info + TI_PP_CPDSP_FW;
    return (pdsp_cmd_rsp_by_sz (pdsp, cmd_word, wr_ptr, wr_word, rd_ptr, rd_word));
}

static inline Uint32* ppd_memset_word(Uint32 *s, Int32 v, Uint32 n_word)
{
    while(n_word--) *s++ = v;

    return (s);
}

static inline Uint32* ppd_memcpy_word (Uint32 *dest, volatile Uint32 *src,
                                        Uint32 n_word)
{
    while(n_word--)
    {
        *dest++ = *src++;
    }
    return (dest);
}

#if ((PPD_DEBUG) & PPD_DEBUG_MSG)
static Int32 ppd_dump_sr_open (Uint32* ptr, Uint8* dump_buff_p, Int32 buff_len)
{
    Int32 len;
    len =  sprintf(dump_buff_p, WSTR_S1_0(0, *ptr, "Dflt Tag", "Dflt RxQ"));
    len += sprintf(dump_buff_p + len, WSTR_S1_0(1, *(ptr+1), "Event QM", "Event Q"));
    len += sprintf(dump_buff_p + len, WSTR_W0(2, *(ptr+2), "SYNC Maxpkt"));
    len += sprintf(dump_buff_p + len, WSTR_W0(3, *(ptr+3), "SYNC Tmo 10us"));
    return len;
}
#endif /* (PPD_DEBUG) & PPD_DEBUG_MSG */

typedef struct
{
    union
    {
        STRUCT_S0_1 (
                dflt_host_rx_q,         /* Default Host Rx Queue */
                dflt_host_rx_dst_tag    /* Default Host Rx Destination Tag */
                ) s;
        Uint32 frags;
    } w0;

    union
    {
        STRUCT_S0_1 (
                host_ev_queue,           /* Index of host event queue */
                dflt_host_rx_dst_tag     /* Q Manager of host event queue */
                ) s;
        Uint32 frags;
    } w1;

    Uint32  w2_sync_max_pkt;        /* Max pkts in syncrnzn queue */
    Uint32  w3_sync_timeout_10us;   /* Max session sync time multiple of 10us */
    Uint32  w4_buff_pool_indx;      /* Pool for PP internal 128 byte buffers */


} ppd_sr_open_params_t;


ppd_event_hdlr_cfg_t g_ppd_event_hdlr_cfg;
ppd_pdsp_poll_cfg    g_pdsp_poll_cfg;

Int32 ti_ppd_sram_test(void)
{
    Int32 i = 0;
    Int32 arrLen = sizeof(g_ram_info) / sizeof(g_ram_info[0]);
    Int32 ret;

    /* Test Scratch RAM for all PDSPs */
    for (i = 0; i < arrLen; i++)
    {
        if ((ret = avalanche_do_ram_test((volatile unsigned int*)(g_ram_info[i].sram), g_ram_info[i].sram_size)))
        {
            printErr("###############################################################################\n");
            printErr("PP PDSP(%d) SRAM check failed, ret=%d (Address range 0x%08X-0x%08X)\n", i, ret, (Uint32)(g_ram_info[i].sram), ((Uint32)(g_ram_info[i].sram)) + g_ram_info[i].sram_size);
            printErr("###############################################################################\n");
            BUG();
            return -1;
        }
    }
    return 0;
}

Int32 ti_ppd_init(TI_PPD_CONFIG *cfg)
{
    cpdsp_cmd_t cmd_word = WORD_S0_B1_0(0, cfg->buff_pool_indx, SRPDSP_SR_OPEN);
    struct srl_ret_init_param
    {
        Uint32 ses_tbl_phy_mem;
        Uint32 ses_byte_size;
    } ret_param;
    Int32 i = 0, ret_val = 0;
    ppd_sr_open_params_t sr_open_params;

    /* CHK: Not locking the init sequence here since this function is expected
     * to be called as part of system initialization. May need to use locks
     * otherwise.
     */
    if (g_ppd_init_done)
    {
        return 0;
    }

    /* Open command */
    sr_open_params.w0.frags             = WORD_S1_0(cfg->dflt_host_rx_dst_tag, cfg->dflt_host_rx_q);
    sr_open_params.w1.frags             = WORD_S1_0(cfg->host_q_mgr, cfg->host_ev_queue);
    sr_open_params.w2_sync_max_pkt      = cfg->sync_max_pkt;
    sr_open_params.w3_sync_timeout_10us = cfg->sync_timeout_10us;
    sr_open_params.w4_buff_pool_indx    = cfg->buff_pool_indx;

#if ((PPD_DEBUG) & PPD_DEBUG_MSG)
    printMsg ("ti_ppd_init: SR_OPEN dump:\n");
    ppd_dump_sr_open ((Uint32*)&sr_open_params, dump_buff, sizeof(dump_buff));
    printMsg("%s\n",dump_buff);
#endif

    ret_val = cpdsp_cmd_rsp_by_sz(cmd_word, &sr_open_params, SIZE_IN_WORD(sr_open_params), &ret_param, SIZE_IN_WORD(ret_param));
    printMsg ("ti_ppd_init: SR_OPEN returned %d.\n", ret_val);
    if (ret_val)
    {
        printMsg ("ERROR: ti_ppd_init: SR_OPEN returned %d.\n", ret_val);
        return ret_val;
    }

    gp_ses_blk  =  (ppd_ses_info_blk_t *)ppd_os_get_io_virt(ret_param.ses_tbl_phy_mem);
    gp_free_blk32_list =  (ppd_block32_t *) (((Uint8*)gp_ses_blk) + TI_PP_MAX_ACCLERABLE_SESSIONS*sizeof(ppd_ses_info_blk_t));
    {
        Uint32 last_free_blk32 = (Uint32)gp_ses_blk + ret_param.ses_byte_size - 32;
        ppd_init_blk32((ppd_block32_t*)last_free_blk32);
        ppd_init_hblk();
        printMsg ("ti_ppd_init: gp_ses_blk = %#x\n", (unsigned)gp_ses_blk);
    }
    
    ppd_memset_word (g_session_slots, 0, SIZE_IN_WORD(g_session_slots));

    g_is_in_psm = 0;

    /* Initialize and start evenet poll timer.
     *  Note that at this time there may not be any evenet handler registerd. So
     *  in that case, all this timer would do is to recycle event BDs thus
     *  ensuring that the user gets fresh events as soon as the handler is
     *  registered.
     */
    {
        PAL_Handle hnd;
        Cppi4Queue tmpQ;    /* Just used for filling Q info for opening */

        /* Get PAL CPPI4 Handle to use its APIs */
        hnd = PAL_cppi4Init (NULL, NULL);

        if (hnd == NULL)
        {
            printErr ("%s: PAL_cppi4Init failed! Make sure CPPI Library is"
                      "successfully initialized *before* initializing PPD\n",
                      __FUNCTION__);
            goto lbl_ppd_init_err;
        }


        /* Open and store handles to event queuse for use by event timer */
        tmpQ.qMgr   = cfg->host_q_mgr;
        tmpQ.qNum   = cfg->host_ev_queue;
        g_ppd_event_hdlr_cfg.eventQHnd = PAL_cppi4QueueOpen (hnd, tmpQ);
        tmpQ.qMgr   = PAL_CPPI41_QUEUE_MGR_PARTITION_SR;
        tmpQ.qNum   = PAL_CPPI41_SR_PP_EVENTS_FD_Q_NUM;
        g_ppd_event_hdlr_cfg.eventFQHnd = PAL_cppi4QueueOpen (hnd, tmpQ);
        g_ppd_event_hdlr_cfg.polltime_msecs = EVENT_POLLTIME_MSECS;
        if (PAL_osTimerCreate (ppd_event_timer, (Uint32)&g_ppd_event_hdlr_cfg, &g_ppd_event_hdlr_cfg.hTimer) != PAL_SOK)
        {
            printErr ("%s: Event timer installation failed\n", __FUNCTION__);
            goto lbl_ppd_init_err;
        }
        PAL_osTimerStart (g_ppd_event_hdlr_cfg.hTimer, g_ppd_event_hdlr_cfg.polltime_msecs);
    }

    /* Initialize and start PDSP poll timer */
    g_pdsp_poll_cfg.polltime_msecs  = PDSP_POLLTIME_MSECS;
    if (PAL_osTimerCreate (ppd_pdsp_poll_timer, (Uint32)&g_pdsp_poll_cfg, &g_pdsp_poll_cfg.hTimer) != PAL_SOK)
    {
        printErr ("%s: PDSP event timer installation failed\n\n*** PDSP PMONs and session uptime won't be available\n", __FUNCTION__);
        goto lbl_ppd_init_err;
    }
    else
    {
        /* There is no API currently to 'enable' PDSP PMON, so force pdsp
         * counter enable here
         */
        for (i = 0; i < PP_NUM_PDSP; i++)
        {
            g_pdsp_regs [i]->control |= PDSP_REG_COUNT_ENABLE_BIT;
        }

        PAL_osTimerStart (g_pdsp_poll_cfg.hTimer, g_pdsp_poll_cfg.polltime_msecs);
    }

    g_ppd_init_done = 1;
    return 0;

lbl_ppd_init_err:
    if (g_ppd_event_hdlr_cfg.hTimer)
    {
        PAL_osTimerDestroy (g_ppd_event_hdlr_cfg.hTimer);
    }
    return -1;
}


Int32 ti_ppd_get_srl_pkt_stats(TI_PP_GLOBAL_STATS *stats)
{
    cpdsp_cmd_t cmd_word = WORD_S0_B1_0(0, STATS_TYPE_SRGLOBAL, SRPDSP_SR_STATS_GET);

    return (cpdsp_cmd_rsp_by_sz(cmd_word, 0, 0, stats, SIZE_IN_WORD(*stats)));
}

Int32 ti_ppd_clear_srl_pkt_stats(void)
{
    cpdsp_cmd_t cmd_word =
        WORD_S0_B1_0(0, STATS_TYPE_SRGLOBAL, SRPDSP_SR_STATS_CLEAR);

    return (cpdsp_cmd_rsp_by_sz(cmd_word, 0, 0, 0, 0));
}

Int32 ti_ppd_get_n_clear_srl_pkt_stats(TI_PP_GLOBAL_STATS *stats)
{
    cpdsp_cmd_t cmd_word =
        WORD_S0_B1_0(0, STATS_TYPE_SRGLOBAL, SRPDSP_SR_STATS_CLEAR);

    return (cpdsp_cmd_rsp_by_sz(cmd_word, 0, 0, stats, SIZE_IN_WORD(*stats)));
}

Int32 ppd_set_psm (Uint32 enable)
{
    cpdsp_cmd_t  cmd_word    = WORD_S0_B1_0(enable, 0,
                                            SRPDSP_SR_SETPSM);
    return (cpdsp_cmd_rsp_by_sz(cmd_word, 0, 0, 0, 0));
}

/*
 *                      PID Management APIs
 */


#if ((PPD_DEBUG) & PPD_DEBUG_MSG)
static Int32 ppd_dump_pid_add (Uint32* ptr, Uint8* dump_buff_p, Int32 buff_len)
{
    Int32 len;
    len = sprintf(dump_buff_p,
                WSTR_B3_0(0, *ptr, "Pri Map", "Pri Drop", "Framing", "Flags"));
    len += sprintf(dump_buff_p + len,
                WSTR_S1_0(1, *(ptr+1), "Dst Tag", "Fwd Q"));
    return len;
}
#endif /* ((PPD_DEBUG) & PPD_DEBUG_MSG) */


#define CPDSP_CMD(i, o, c) ((i << 16) | (i << 8) | c)

Int32 ti_ppd_config_pid_range(TI_PP_PID_RANGE *cfg)
{
    cpdsp_cmd_t cmd_word = WORD_S0_B1_0(0, PID_INSTALL_CONFIG_RANGE, SRPDSP_PID_COMMAND);

    return (cpdsp_cmd_rsp_by_sz(cmd_word, (void*)cfg, 1, 0, 0));
}

Int32 ti_ppd_remove_pid_range(Uint32 port_num)
{
    cpdsp_cmd_t cmd_word = WORD_S0_B1_0(0, PID_REMOVE_CONFIG_RANGE, SRPDSP_PID_COMMAND);

    /* CHK: Tough the spec doesn't say so, we need to put port number in the
     * MSB, otherwise we get port mapping error.
     */
    port_num = WORD_B3_0(port_num, 0, 0, 0);

    return (cpdsp_cmd_rsp_by_sz(cmd_word, &port_num, 1, 0, 0));
}

typedef struct
{
    union
    {
        STRUCT_B0_3 (
                flags,           /* Flags, Host provided */
                in_framing,      /* Allowed ingress framing, Host provided */
                dflt_pri_drp,    /* Drop priority, Host provided */
                pri_map          /* Priority mapping */
                ) s;
        Uint32 frags;

    } w0;

    union
    {
        STRUCT_S0_1 (
                dflt_q_index,    /* Default fowarding queue manager and index */
                dflt_dst_tag     /* Destination tag for default forwarding */
                ) s;
        Uint32 frags;

    } w1;

} ppd_pid_info_blk_t;

Int32 ti_ppd_create_pid(TI_PP_PID *params)
{
    TI_PP_pid_t  pid         = params->pid_handle;
    cpdsp_cmd_t  cmd_word    = WORD_S0_B1_0(pid, PID_ADD, SRPDSP_PID_COMMAND);
    ppd_pid_info_blk_t pd;

    params->priv_flags = 0;
    pd.w0.frags = WORD_B3_0(params->pri_mapping, params->dflt_pri_drp,
                params->ingress_framing, params->priv_flags);

    pd.w1.frags = WORD_S1_0(params->dflt_dst_tag, params->dflt_fwd_q);

#if ((PPD_DEBUG) & PPD_DEBUG_MSG)
    printMsg ("Parameter block for PID %d:\n", pid);
    ppd_dump_pid_add ((Uint32 *)&pd, dump_buff, sizeof(dump_buff));
    printMsg("%s\n",dump_buff);
#endif
    return (cpdsp_cmd_rsp_by_sz(cmd_word, &pd, SIZE_IN_WORD(pd), 0, 0));
}

/*
 *                      VPID Management APIs
 */
typedef struct
{
    union
    {
        STRUCT_B0_3 (
                flags,       /* Configuration flags, Host provided */
                pad0,
                pad1,
                pad2
                ) s;
        Uint32 frags;

    } w0;

} ppd_vpid_params_t;


#if ((PPD_DEBUG) & PPD_DEBUG_MSG)
static Int32 ppd_dump_vpid_add (Uint32* ptr, Uint8* dump_buff_p, Int32 buff_len)
{
    Int32 len;
    len = sprintf(dump_buff_p,
                    WSTR_B3_0(0, *ptr, "byte3", "byte2", "byte1", "Flags"));
    return len;
}
#endif /* ((PPD_DEBUG) & PPD_DEBUG_MSG) */

Int32 ti_ppd_create_vpid(TI_PP_VPID *params)
{
    TI_PP_vpid_t        vpid = params->vpid_handle;
    ppd_vpid_params_t   vpid_params;
    cpdsp_cmd_t         cmd_word    =
        WORD_S0_B1_0(vpid, VPID_ADD, SRPDSP_VPID_COMMAND);

    vpid_params.w0.frags = WORD_B3_0(0, 0, 0, 0);
    params->priv_vpid_flags = 0;

#if ((PPD_DEBUG) & PPD_DEBUG_MSG)
    printMsg ("Parameter block for VPID %d:\n", vpid);
    ppd_dump_vpid_add ((Uint32 *)&vpid_params, dump_buff, sizeof(dump_buff));
    printMsg("%s\n",dump_buff);
#endif
    return (cpdsp_cmd_rsp_by_sz (cmd_word, &vpid_params,
                                SIZE_IN_WORD(vpid_params), 0, 0));
}

Int32 ti_ppd_set_vpid_flags(TI_PP_VPID *params, Uint32 new_flags)
{
    TI_PP_vpid_t        vpid        = params->vpid_handle;
    cpdsp_cmd_t  cmd_word       =
        WORD_S0_B1_0(vpid, VPID_CHANGE_FLAGS, SRPDSP_VPID_COMMAND);

    params->priv_vpid_flags = new_flags;

    printMsg ("New flags for VPID(%d): %#x\n", vpid, params->priv_vpid_flags);
    new_flags = WORD_B3_0(0, 0, 0, params->priv_vpid_flags);
    return (cpdsp_cmd_rsp_by_sz(cmd_word, &new_flags, 1, 0, 0));
}

Int32 ti_ppd_delete_vpid(Uint8 vpid_handle)
{
    cpdsp_cmd_t     cmd_word =
        WORD_S0_B1_0(vpid_handle, VPID_REMOVE, SRPDSP_VPID_COMMAND);

    return (cpdsp_cmd_rsp_by_sz(cmd_word, 0, 0, 0, 0));
}

Int32 ti_ppd_get_vpid_stats(TI_PP_vpid_t vpid, TI_PP_VPID_STATS *stats)
{
    cpdsp_cmd_t cmd_word = WORD_S0_B1_0(vpid, STATS_TYPE_VPID, SRPDSP_SR_STATS_GET);

    return (cpdsp_cmd_rsp_by_sz(cmd_word, 0, 0, stats, SIZE_IN_WORD(*stats)));
}


Int32 ti_ppd_clear_vpid_stats(Uint8 vpid_handle)
{
    cpdsp_cmd_t cmd_word =
        WORD_S0_B1_0(vpid_handle, STATS_TYPE_VPID, SRPDSP_SR_STATS_CLEAR);

    return (cpdsp_cmd_rsp_by_sz(cmd_word, 0, 0, 0, 0));
}

Int32 ti_ppd_get_n_clear_vpid_stats(TI_PP_vpid_t vpid, TI_PP_VPID_STATS *stats)
{
    cpdsp_cmd_t cmd_word =
        WORD_S0_B1_0(vpid, STATS_TYPE_VPID, SRPDSP_SR_STATS_CLEAR);

    return (cpdsp_cmd_rsp_by_sz(cmd_word, 0, 0, stats, SIZE_IN_WORD(*stats)));
}


Int32 ti_ppd_set_pid_flags(TI_PP_PID *pid_params, Uint32 new_flags)
{
    TI_PP_pid_t  pid         = pid_params->pid_handle;
    cpdsp_cmd_t  cmd_word    = WORD_S0_B1_0(pid, PID_CHANGE_FLAGS, SRPDSP_PID_COMMAND);

    pid_params->priv_flags = new_flags;
    printMsg ("New flags for PID(%d): %#x\n", pid, new_flags);
    new_flags = WORD_B3_0(0, 0, 0, pid_params->priv_flags);
    return (cpdsp_cmd_rsp_by_sz(cmd_word, &new_flags, 1, 0, 0));
}

Int32 ti_ppd_delete_pid(Uint8 pid_handle)
{
    cpdsp_cmd_t  cmd_word = WORD_S0_B1_0(pid_handle, PID_REMOVE, SRPDSP_PID_COMMAND);

    return (cpdsp_cmd_rsp_by_sz(cmd_word, 0, 0, 0, 0));
}



/*
 *                      Session Management APIs
 */

ppd_l2_frame_t* ppd_get_l2_buf(void)
{
    ppd_l2_frame_t *p_hblk;
    Uint32 cookie;

    PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &cookie);
    p_hblk = gp_hblk_pool;

    if (!p_hblk)
    {
        PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);
        return NULL;
    }

    gp_hblk_pool    = p_hblk->u.next;
    PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);
    printMsg ("%s: p_hblk = %#x, gp_hblk_pool = %#x\n",
                __FUNCTION__, (Uint32)p_hblk, (Uint32)gp_hblk_pool);
    p_hblk->data = (Uint8*) p_hblk;
    p_hblk->end  = (Uint8*) p_hblk;
    p_hblk->len  = 0;
    return p_hblk;
}

static Uint8 ppd_l2_buf_get_len (ppd_l2_frame_t *f)
{
    return (f->len);
}

static Int32 ppd_l2_buf_hdr_reserve(ppd_l2_frame_t *f, Uint8 len)
{
    f->data += len;
    f->end  += len;
    return (0);
}

static Int32 ppd_l2_buf_append(ppd_l2_frame_t *f, Uint8 *buf, Uint8 len)
{
    if(len > (PPD_L2_FRM_MAX_HLEN - PPD_L2_FRM_HLEN(f->data) - f->len))
        return (-1);

    printMsg ("%s: p_hblk = %#x\n", __FUNCTION__, (Uint32)f);
    memcpy(f->end, buf, len); f->len  += len; f->end += len;
    return (0);
}

static Int32 ppd_l2_buf_prepend(ppd_l2_frame_t *f, Uint8 *buf, Uint8 len)
{
    if(len > PPD_L2_FRM_HLEN(f->data)) return (-1);

    printMsg ("%s: p_hblk = %#x\n", __FUNCTION__, (Uint32)f);
    f->data -= len;
    f->len  += len;
    memcpy(f->data, buf, len);
    return (0);
}

void ppd_l2_buf_free(Uint8 *hdr)
{
    ppd_l2_frame_t *f = (ppd_l2_frame_t*)((Uint32)hdr & ~PPD_L2_HBLKSZ_MASK);
    Uint32 cookie;

    printMsg ("%s: p_hblk = %#x, gp_hblk_pool = %#x\n",
                __FUNCTION__, (Uint32)f, (Uint32)gp_hblk_pool);

    PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &cookie);
    f->u.next = gp_hblk_pool; gp_hblk_pool = f;
    PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);

}

/*
 * CHK: Returns '0' when invalid/unsupported protocol.
 */
static inline Uint16 ppd_get_eth_type_val (TI_PP_PACKET_DESC * ingress_l2_pkt_desc,
                                            TI_PP_PACKET_TYPE   pkt_type)
{
    if (ingress_l2_pkt_desc->u.eth_desc.pppoe_sid != TI_PP_SESSION_PPPOE_INVALID)
    {
    	return 0x8864;
    }
    else
    {
        if (pkt_type == TI_PP_IPV4_TYPE)
        {
            return 0x0800;
        }
        else if (pkt_type == TI_PP_IPV6_TYPE)
        {
            return 0x86DD;
        }
    }

    return 0;
}
/*
 * CHK: Returns '0' when invalid/unsupported protocol.
 */
static inline Uint16 ppd_get_pkt_type_val (TI_PP_VPID_TYPE  vpid_type,
                                    TI_PP_PACKET_TYPE pkt_type)
{
	if (pkt_type == TI_PP_IPV4_TYPE)
	{
		return 0x0800;
	}
	else if (pkt_type == TI_PP_IPV6_TYPE)
	{
		return 0x86DD;
	}

    return 0;
}

typedef struct
{
    TI_PP_SESSION*  ses_cfg;
    TI_PP_VPID*     ingress_vpid;
} ppd_lut_t;

#define IDX2PID(x)                  ((x)|0x40)

static Int32 ppd_wr_l2_lut(volatile Uint32 *ram_ptr, ppd_lut_t *lut_data)
{
    Int32 ret_val = 0;
    TI_PP_SESSION   *   ses_cfg         = lut_data->ses_cfg;
    TI_PP_VPID      *   ingress_vpid    = lut_data->ingress_vpid;
    TI_PP_PACKET_DESC * pkt_desc        = &ses_cfg->ingress.l2_packet;
    TI_PP_PACKET_TYPE   pkt_type        = ses_cfg->ingress.l3l4_packet.packet_type;

    Uint32              enable_flags    = TI_PP_L2_LUT_PKT_KEY_EN;

    Uint16              vlan_id     = 0;
    Uint8               vlan_prty   = 0;

    switch(pkt_desc->packet_type)
    {
        case TI_PP_ETH_TYPE:
        {
            TI_PP_ETH_DESC* l2_desc = &pkt_desc->u.eth_desc;
            Uint16 eth_type_val;

            if (l2_desc->enables & TI_PP_SESSION_L2_DSTMAC_VALID)
                enable_flags |= TI_PP_L2_LUT_DST_MAC_EN;
            if (l2_desc->enables & TI_PP_SESSION_L2_SRCMAC_VALID)
                enable_flags |= TI_PP_L2_LUT_SRC_MAC_EN;


            if ((eth_type_val = ppd_get_eth_type_val(pkt_desc, pkt_type)))
            {
                enable_flags |= TI_PP_L2_LUT_ETH_TYPE_EN;
            }

            if (!eth_type_val && (pkt_type != TI_PP_NO_TYPE))
            {
                printErr ("%s: Unsupported payload type %d for vpid type %d\n", __FUNCTION__, pkt_type, ingress_vpid->type);
                return -1;
            }

            enable_flags |= TI_PP_L2_LUT_IN_PID_EN;

            *ram_ptr++ = WORD_B3_0(l2_desc->dstmac[2], l2_desc->dstmac[3], l2_desc->dstmac[4], l2_desc->dstmac[5]);
            *ram_ptr++ = WORD_B3_0(l2_desc->srcmac[2], l2_desc->srcmac[3], l2_desc->srcmac[4], l2_desc->srcmac[5]);
            *ram_ptr++ = WORD_B3_0(l2_desc->srcmac[0], l2_desc->srcmac[1], l2_desc->dstmac[0], l2_desc->dstmac[1]);

            /* Since PP supports only one VLAN in LUT - we prefer to classify according to VLAN in packet rather than VPID's VLAN if both exist */
            if (l2_desc->enables & TI_PP_SESSION_L2_VLAN_VALID)
            {
                enable_flags |= TI_PP_L2_LUT_VLAN_ID_EN;
                enable_flags |= TI_PP_L2_LUT_VLAN_PRI_EN;
                vlan_id     =         l2_desc->vlan_tag & 0xFFF;
                vlan_prty   = (Uint8)(l2_desc->vlan_tag >> 13);
            }
            else if (ingress_vpid->type == TI_PP_VLAN)
            {
                enable_flags |= TI_PP_L2_LUT_VLAN_ID_EN;
                vlan_id = ingress_vpid->vlan_identifier;
            }

            *ram_ptr++ = WORD_S1_0(vlan_id, eth_type_val);

            {
                Uint32 docsis_DSID_Index = -1;

                if (ses_cfg->ingress.app_specific_data.u.app_desc.enables != 0)
                {
                    enable_flags |= TI_PP_L2_LUT_DOCSIS_ID_EN;
                    docsis_DSID_Index = ses_cfg->ingress.app_specific_data.u.app_desc.u.raw_app_info1;
                }

                *ram_ptr++ = WORD_B3_0( docsis_DSID_Index,
                                        vlan_prty,
                    IDX2PID(ingress_vpid->parent_pid_handle),
                                        TI_PP_LUT_DATA_L2_ETH );
            }
            *ram_ptr++ = WORD_W0(enable_flags);

#if ((PPD_DEBUG) & PPD_DEBUG_MSG)
            printMsg ("Eth LUT Dump:\n");
            ppd_dump_eth_lut ((Uint32*)ram_ptr - 6, dump_buff, sizeof(dump_buff));
            printMsg("%s\n",dump_buff);
#endif
        }
        break;

        default:
            printErr ("Non Ethernet 1st level lookup (L2) is NOT supported.\n");
            ret_val = -1;
    }

    return(ret_val);
}

static Int32 ppd_wr_l3_lut(volatile Uint32 *ram_ptr, ppd_lut_t *lut_data)
{
    Int32 ret_val = 0;
    TI_PP_SESSION       *ses_cfg        = lut_data->ses_cfg;
    TI_PP_VPID          *ingress_vpid   = lut_data->ingress_vpid;
    Uint32          enable_flags    = TI_PP_L3_LUT_PKT_KEY_EN;

    switch(ses_cfg->ingress.l3l4_packet.packet_type)
    {
        case TI_PP_IPV4_TYPE:
            {
                TI_PP_IPV4_DESC *dat =
                    &ses_cfg->ingress.l3l4_packet.u.ipv4_desc;
                Uint16 ppp_ses_id = 0xffff;

                if (dat->enables & TI_PP_SESSION_IPV4_DSTIP_VALID)
                    enable_flags |= TI_PP_L3_LUT_DST_IPV4_EN;
                if (dat->enables & TI_PP_SESSION_IPV4_SRCIP_VALID)
                    enable_flags |= TI_PP_L3_LUT_SRC_IPV4_EN;
                if (dat->enables & TI_PP_SESSION_IPV4_DST_PORT_VALID)
                    enable_flags |= TI_PP_L3_LUT_TCP_UDP_DST_PORT_EN;
                if (dat->enables & TI_PP_SESSION_IPV4_SRC_PORT_VALID)
                    enable_flags |= TI_PP_L3_LUT_TCP_UDP_SRC_PORT_EN;
                if (dat->enables & TI_PP_SESSION_IPV4_PROTOCOL_VALID)
                    enable_flags |= TI_PP_L3_LUT_IPV4_PROTO_EN;
                if (dat->enables & TI_PP_SESSION_IPV4_TOS_VALID)
                    enable_flags |= TI_PP_L3_LUT_IPV4_TOS_EN;

                if (ses_cfg->ingress.l2_packet.u.eth_desc.pppoe_sid != TI_PP_SESSION_PPPOE_INVALID)
                {
                	enable_flags |= TI_PP_L3_LUT_PPPOE_SES_ID_EN;
                	ppp_ses_id = __constant_htons(ses_cfg->ingress.l2_packet.u.eth_desc.pppoe_sid);
                }
                /* TODO: TI_PP_L3_LUT_IPV4_FRAG_FLAG_EN : Set to '0' for
                 * 'routable' sessions, dont enable for 'bridged' sessions.
                 * Two problems: 1) Seems like even the fragmented packets are
                 * 'matched' and 2) Ideally session should go in diversion on
                 * receiving the first fragmented pkt which won't happen with
                 * this LUT configuration
                 */

                *ram_ptr++ = WORD_W0(dat->dst_ip);
                *ram_ptr++ = WORD_W0(dat->src_ip);
                *ram_ptr++ = WORD_S1_0(dat->src_port, dat->dst_port);
                *ram_ptr++ = WORD_S1_0(0, ppp_ses_id);
                if (ses_cfg->ingress.l2_packet.u.eth_desc.enables & TI_PP_SESSION_L2_GRE_DS_VALID)
                {
                    *ram_ptr++ = WORD_B3_0(dat->tos,  dat->protocol, 0, TI_PP_LUT_DATA_L3_DS_GRE);
                }
                else
                {
                    *ram_ptr++ = WORD_B3_0(dat->tos,  dat->protocol, 0, TI_PP_LUT_DATA_L3_IPV4);
                }
                *ram_ptr++ = WORD_W0(enable_flags);

#if ((PPD_DEBUG) & PPD_DEBUG_MSG)
                printMsg ("IPv4 LUT Dump:\n");
                ppd_dump_ipv4_lut ((Uint32*)ram_ptr - 6,
                                    dump_buff, sizeof(dump_buff));
                printMsg("%s\n",dump_buff);
#endif
                break;
            }

        case TI_PP_IPV6_TYPE:
            {
                TI_PP_IPV6_DESC *dat =
                    &ses_cfg->ingress.l3l4_packet.u.ipv6_desc;
                Uint16 ppp_ses_id = 0xffff;

                if (dat->enables & TI_PP_SESSION_IPV6_DSTIP_VALID)
                    enable_flags |= TI_PP_L3_LUT_DST_IPV6_EN;
                if (dat->enables & TI_PP_SESSION_IPV6_SRCIP_VALID)
                    enable_flags |= TI_PP_L3_LUT_SRC_IPV6_EN;
                if (dat->enables & TI_PP_SESSION_IPV6_DST_PORT_VALID)
                    enable_flags |= TI_PP_L3_LUT_TCP_UDP_DST_PORT_EN;
                if (dat->enables & TI_PP_SESSION_IPV6_SRC_PORT_VALID)
                    enable_flags |= TI_PP_L3_LUT_TCP_UDP_SRC_PORT_EN;
                if (dat->enables & TI_PP_SESSION_IPV6_NEXTHDR_VALID)
                    enable_flags |= TI_PP_L3_LUT_IPV6_NEXT_HDR_EN;
                if (dat->enables & TI_PP_SESSION_IPV6_TRCLASS_VALID)
                    enable_flags |= TI_PP_L3_LUT_IPV6_T_CLASS_EN;
                if (dat->enables & TI_PP_SESSION_IPV6_FLOWLBL_VALID)
                    enable_flags |= TI_PP_L3_LUT_IPV6_FLOW_LBL_EN ;

				
                if (ses_cfg->ingress.l2_packet.u.eth_desc.pppoe_sid != TI_PP_SESSION_PPPOE_INVALID)
                {
                	enable_flags |= TI_PP_L3_LUT_PPPOE_SES_ID_EN;
                	ppp_ses_id = __constant_htons(ses_cfg->ingress.l2_packet.u.eth_desc.pppoe_sid);
                }

                *ram_ptr++ = WORD_W0(0); /* Hash to be generated bt FW */
                *ram_ptr++ = WORD_W0(0); /* -"- */
                *ram_ptr++ = WORD_S1_0(dat->src_port, dat->dst_port);
                if (dat->enables & TI_PP_SESSION_IPV6_DSLITE_DSTIP_VALID)
                {
                    enable_flags |= TI_PP_L3_LUT_IPV6_DSLITE_EN;
                    *ram_ptr++ = WORD_W0(dat->dsLite_dst_ip);
                    *ram_ptr++ = WORD_B3_0(dat->traffic_class, dat->next_header, (dat->flow_label >> 16), TI_PP_LUT_DATA_L3_DSLITE);
                }
                else
                {
                    *ram_ptr++ = WORD_S1_0((dat->flow_label & 0xffff), ppp_ses_id);
                    if (ses_cfg->ingress.l2_packet.u.eth_desc.enables & TI_PP_SESSION_L2_GRE_DS_VALID)
                    {
                        *ram_ptr++ = WORD_B3_0(dat->traffic_class, dat->next_header, (dat->flow_label >> 16), TI_PP_LUT_DATA_L3_DS_GRE);
                    }
                    else
                    {
                        *ram_ptr++ = WORD_B3_0(dat->traffic_class, dat->next_header, (dat->flow_label >> 16), TI_PP_LUT_DATA_L3_IPV6);
                    }
                }
                *ram_ptr++ = WORD_W0(enable_flags);

#if ((PPD_DEBUG) & PPD_DEBUG_MSG)
                printMsg ("IPv6 LUT Dump:\n");
                ppd_dump_ipv6_lut((Uint32*)ram_ptr - 6,
                                    dump_buff, sizeof(dump_buff));
                printMsg("%s\n",dump_buff);
#endif
                break;
            }

        case TI_PP_NO_TYPE:
            {
                *(ram_ptr + 10) = WORD_B3_0(0, 0, 0, TI_PP_LUT_DATA_L3_UNDEF);
                *ram_ptr++ = WORD_W0(enable_flags);
                break;
            }

        default:
            ret_val = -1;
    }

    return(ret_val);
}


static Int32 ppd_wr_lut(volatile Uint32 *ram_ptr, void *in_obj)
{
    Int32 ret_val;
    ppd_lut_t *lut_data = (ppd_lut_t*) in_obj;

    ret_val = ppd_wr_l2_lut(ram_ptr, lut_data);
    ret_val |= ppd_wr_l3_lut(ram_ptr + 6, lut_data); /* start at 25th B */
   
    return (ret_val);
}

void ppd_cleanup_egress_list (ppd_egress_rec_t *egress)
{
    ppd_egress_rec_t *next_egress;
    while (egress)
    {
        if (egress->w6_mod_rec_ptr)
            ppd_free_blk32((ppd_block32_t*) ppd_os_get_io_virt(egress->w6_mod_rec_ptr));

        next_egress = egress->w0_next_egr_ptr
                        ? (ppd_egress_rec_t*)ppd_os_get_io_virt(egress->w0_next_egr_ptr)
                        : 0;

        if (egress->w5_l2_hdr_ptr)
            ppd_l2_buf_free ((Uint8*)PAL_osMemPhy2Virt(egress->w5_l2_hdr_ptr));

        ppd_free_blk32 ((ppd_block32_t*)(egress));

        printMsg ("%s: next-egr @%#x\n", __FUNCTION__, (unsigned)next_egress);
        egress = next_egress;
    }
}

Int32 ppd_destroy_session(ppd_ses_info_blk_t *ses)
{
    ppd_egress_rec_t *egress;
    egress = ses->w3_egress_ptr
                ? (ppd_egress_rec_t*)ppd_os_get_io_virt(ses->w3_egress_ptr)
                : 0;

    printMsg ("ppd_destroy_session: session @%#x, egress @%#x\n",
                (unsigned)ses, (unsigned)egress);

    if (ses->w1_ipv6_rec)
        ppd_free_blk32((ppd_block32_t*)ppd_os_get_io_virt(ses->w1_ipv6_rec));

    ppd_cleanup_egress_list (egress);

    printMsg ("ppd_destroy_session: session destroyed\n");
    return (0);
}

Uint32 ppd_xcsum_u32(Uint32 csum_delta, Uint32 old_word, Uint32 new_word)
{
    csum_delta += (old_word & 0xFFFF) + ((old_word >> 16) & 0xFFFF);
    csum_delta -= (new_word & 0xFFFF) + ((new_word >> 16) & 0xFFFF);

    //CHK: Doesn't work when old_word < new_word csum_delta  = (csum_delta &
    //0xFFFF) + ((csum_delta >> 16) & 0xFFFF);
    return (csum_delta);
}

Uint32 ppd_xcsum_u16 (Uint32 csum_delta, Uint16 old_hword, Uint16 new_hword)
{
    csum_delta += old_hword - new_hword;
    csum_delta  = (csum_delta & 0xFFFF) + ((csum_delta >> 16) & 0xFFFF);
    return (csum_delta);
}

Int32 ppd_get_ipv4_mod_rec (TI_PP_IPV4_DESC *in_ipv4, TI_PP_IPV4_DESC *out_ipv4,
                            ppd_pkt_mod_rec_t **pkt_mod_ptr)
{
    ppd_pkt_mod_rec_t   *pkt_mod = NULL;
    Uint16              l3_mod_flags = 0, l4_mod_flags = 0;
    Uint32              l3_xcsum = 0;
    Uint16              l4_xcsum = 0;

    /* Make sure we have all the required ingress information for Header mod */
    if ((out_ipv4->enables & in_ipv4->enables) != out_ipv4->enables)
    {
        printErr ("ERROR: Ingress fields reqd for egress L3 chksum \n");
        return -1;
    }

    if (!out_ipv4->enables)
    {
        *pkt_mod_ptr = NULL;
        return 0;
    }

    pkt_mod = (ppd_pkt_mod_rec_t*)
        ppd_get_free_blk32();

    if(!pkt_mod)
        return -1;

    if (out_ipv4->enables & TI_PP_SESSION_IPV4_DSTIP_VALID)
    {
        l3_mod_flags |= TI_PP_MOD_IPDST_VALID;
        l3_xcsum = ppd_xcsum_u32(l3_xcsum, in_ipv4->dst_ip, out_ipv4->dst_ip);
    }

    if (out_ipv4->enables & TI_PP_SESSION_IPV4_SRCIP_VALID)
    {
        l3_mod_flags |= TI_PP_MOD_IPSRC_VALID;
        l3_xcsum = ppd_xcsum_u32(l3_xcsum, in_ipv4->src_ip, out_ipv4->src_ip);
    }

    if (l3_mod_flags)
        l3_mod_flags |= TI_PP_MOD_L3CHK_VALID;

    if (out_ipv4->enables & TI_PP_SESSION_IPV4_TOS_VALID)
        l3_mod_flags |= TI_PP_MOD_IPTOS_VALID;

    l3_xcsum  = (l3_xcsum & 0xFFFF) + ((l3_xcsum >> 16) & 0xFFFF);
    l4_xcsum = l3_xcsum;

    if (out_ipv4->enables & TI_PP_SESSION_IPV4_DST_PORT_VALID)
    {
        l4_mod_flags |= TI_PP_MOD_DSTPORT_VALID;
        l4_xcsum =
            ppd_xcsum_u16(l4_xcsum, in_ipv4->dst_port, out_ipv4->dst_port);
    }

    if (out_ipv4->enables & TI_PP_SESSION_IPV4_SRC_PORT_VALID)
    {
        l4_mod_flags |= TI_PP_MOD_SRCPORT_VALID;
        l4_xcsum =
            ppd_xcsum_u16(l4_xcsum, in_ipv4->src_port, out_ipv4->src_port);
    }

    if (l4_mod_flags)
        l4_mod_flags |= TI_PP_MOD_L4CHK_VALID;

    pkt_mod->w0.frags   = WORD_B1_0_S0(0, out_ipv4->tos,
                                        (l3_mod_flags | l4_mod_flags));
    pkt_mod->w1_ip_src  = WORD_W0(out_ipv4->src_ip);
    pkt_mod->w2_ip_dst  = WORD_W0(out_ipv4->dst_ip);
    pkt_mod->w3.frags   = WORD_S1_0(out_ipv4->src_port, out_ipv4->dst_port);
    pkt_mod->w4.frags   = WORD_S1_0(l3_xcsum, l4_xcsum);
    printMsg ("L3 Checksum Delta = %#x L4 Checksum Delta = %#x\n",
                l3_xcsum, l4_xcsum);

   *pkt_mod_ptr = pkt_mod; return 0;
}

Int32 ppd_get_l3_mod_rec(TI_PP_PACKET_DESC *in_pkt, TI_PP_PACKET_DESC *out_pkt, ppd_pkt_mod_rec_t **pkt_mod_ptr)
{
    /* Currently IPv4 < --- > IPv6 not supported */
    if(in_pkt->packet_type != out_pkt->packet_type)
    {
        if((in_pkt->packet_type == TI_PP_IPV4_TYPE && out_pkt->packet_type == TI_PP_IPV6_TYPE) ||
           (in_pkt->packet_type == TI_PP_IPV6_TYPE && out_pkt->packet_type == TI_PP_IPV4_TYPE))
        {
            printMsg("Ingress-Egress L3 Packet type mismatch, possible DSLite or GRE!!!\n");
            *pkt_mod_ptr = NULL;
            return 0;
        }
        printErr ("ERROR: Ingress-Egress L3/L4 Packet type mismatch (%d!=%d).\n", in_pkt->packet_type, out_pkt->packet_type);
        return -1;
    }

    if (out_pkt->u.ipv4_desc.protocol == IPPROTO_GRE || in_pkt->u.ipv4_desc.protocol == IPPROTO_GRE)
    {
        /* For GRE sessions there is no modification record */
        *pkt_mod_ptr = NULL;
        return 0;
    }
    else if(out_pkt->packet_type == TI_PP_IPV4_TYPE)
    {
        return ppd_get_ipv4_mod_rec (&in_pkt->u.ipv4_desc, &out_pkt->u.ipv4_desc, pkt_mod_ptr);
    }
    else if (out_pkt->packet_type == TI_PP_IPV6_TYPE)
    {
        *pkt_mod_ptr = NULL;
        return 0;
    }

    printErr ("ERROR: Unsupported egress L3/L4 packet type (%d).\n", out_pkt->packet_type);
    return -1;
}


#define TI_PP_ETH_HDR_MIN   (TI_PP_SESSION_L2_DSTMAC_VALID | TI_PP_SESSION_L2_SRCMAC_VALID)

typedef struct {
    ppd_l2_frame_t* hdr_blk;
    Uint8           offset_802_3_len;
    Uint8           l2_frame_code;
} ppd_l2_hdr_info_t;


Int32 ppd_get_802_3_hdr (TI_PP_ETH_DESC *out_eth, TI_PP_VPID *out_vpid, Uint8 priority, Uint8 rsv_hdr_len, Uint16 pkt_type, ppd_l2_hdr_info_t *l2_hdr_info)
{
    Uint16 pkt_type_val;
    ppd_l2_frame_t* hdr_blk;
    Uint32 vlanTag;
    Uint16 val;

    ppd_memset_word ((Uint32*)l2_hdr_info, 0, SIZE_IN_WORD(*l2_hdr_info));
    /* Check if new header needs to be applied */
    if(!rsv_hdr_len && !out_eth->enables)
    {
        return 0;
    }

    if (out_eth->enables && (out_eth->enables & TI_PP_ETH_HDR_MIN) != TI_PP_ETH_HDR_MIN)
    {
        printErr ("Minimum required egress header not provided (%#x != %#x).\n", out_eth->enables, TI_PP_ETH_HDR_MIN);
        return -1;
    }

    pkt_type_val = ppd_get_pkt_type_val (out_vpid->type, pkt_type);
    if (!pkt_type_val && (pkt_type != TI_PP_NO_TYPE))
    {
        printErr ("%s: ERROR - Unsupported payload type %d for vpid type %d\n", __FUNCTION__, pkt_type, out_vpid->type);
        return -1;
    }

    hdr_blk = l2_hdr_info->hdr_blk = ppd_get_l2_buf ();
    if (!hdr_blk)
    {
        return -1;
    }

    printMsg ("Got l2 hdr block @%#x from pool @%#x\n", (unsigned)hdr_blk, (unsigned)hblk_pool);

    ppd_l2_buf_hdr_reserve (hdr_blk, rsv_hdr_len);
    if (!out_eth->enables)
    {
        return 0;
    }

    ppd_l2_buf_append(hdr_blk, out_eth->dstmac, 6);
    ppd_l2_buf_append(hdr_blk, out_eth->srcmac, 6);

    /* Add 802.1Q VLAN header if needed */
    vlanTag = ETH_P_8021Q << 16;

    /* In case VLAN exist both in VPID and in packet, we will first add the VPID's VLAN and then the packet's VLAN */
    if (out_vpid->type == TI_PP_VLAN)
    {
        /* For VPID of type VLAN we take the ID from VPID and the priority from the packet */
        vlanTag |= (out_eth->vlan_tag & VLAN_PRIO_MASK) | (out_vpid->vlan_identifier & VLAN_VID_MASK);
        ppd_l2_buf_append(hdr_blk, (Uint8*)&vlanTag, sizeof(vlanTag));
    }
    if (out_eth->enables & TI_PP_SESSION_L2_VLAN_VALID)
    {
        /* For egress packet with VPID, we take all information from the packet */
        vlanTag  |= out_eth->vlan_tag;
        ppd_l2_buf_append(hdr_blk, (Uint8*)&vlanTag, sizeof(vlanTag));
    }

    if (out_eth->pppoe_sid != TI_PP_SESSION_PPPOE_INVALID)
    {
        /* Apply header as per RFC 2516 */

    	val = __constant_htons(0x8864); /* Ether type for PPPoE Session packets */
        ppd_l2_buf_append(hdr_blk, (Uint8*)&val, sizeof(val));

        val = __constant_htons((((1<<4) | 1) << 8) | 0); /* ver=1, type=1, code=0 */
        ppd_l2_buf_append(hdr_blk, (Uint8*)&val, sizeof(val));

        val = __constant_htons(out_eth->pppoe_sid);
        ppd_l2_buf_append(hdr_blk, (Uint8*)&val, sizeof(val));

        val = 0;
        ppd_l2_buf_append(hdr_blk, (Uint8*)&val, sizeof(val));

        if (pkt_type == TI_PP_IPV4_TYPE)
        {
        	val = __constant_htons(PPP_IP);
        }
        else
        {
        	val = __constant_htons(PPP_IPV6);
        }
        ppd_l2_buf_append(hdr_blk, (Uint8*)&val, sizeof(val));

        l2_hdr_info->l2_frame_code |= TI_PP_EGR_FRM_PPPOE_HDR;
    }

    /* This setting is actually only supported for PPPoE classified packets
     * (with IP as PPP type). But should be don't care for other packets.
     */
    if ((pkt_type == TI_PP_IPV4_TYPE) || (pkt_type == TI_PP_IPV6_TYPE))
    {
        l2_hdr_info->l2_frame_code |= TI_PP_EGR_FRM_REFRAME_IP;
    }

    /* TODO : Support for TI_PP_EGR_FRM_PATCH_802_3 */

    /* TODO: Handle other cases of 802.3 lenght patching. e.g., 802.3 -> VLAN
     * taga, 802.3 -> 802.3. May need to check ingress information for
     * determining this case.
     */

    /* PPPoE Eth type is already added */
    if ((pkt_type != TI_PP_NO_TYPE) && !(l2_hdr_info->l2_frame_code & TI_PP_EGR_FRM_PPPOE_HDR))
    {
        ppd_l2_buf_append(hdr_blk, (Uint8*)&pkt_type_val, 2);
    }

    return 0;
}


Int32 ppd_get_l2_raw_hdr ( TI_PP_L2_RAW_DESC *out_raw,
                           Uint8 rsv_hdr_len,
                           Uint16 pkt_type,
                           ppd_l2_hdr_info_t *l2_hdr_info)
{
    ppd_l2_frame_t* hdr_blk;
    ppd_memset_word ((Uint32*)l2_hdr_info, 0, SIZE_IN_WORD(*l2_hdr_info));

    /* Check if new header needs to be applied */
    if(!rsv_hdr_len && !out_raw->enables)
        return 0;

    hdr_blk = l2_hdr_info->hdr_blk = ppd_get_l2_buf ();

    if (!hdr_blk)
        return -1;

    printMsg ("%s: Got l2 hdr block @%#x from pool @%#x\n", __FUNCTION__,
                (unsigned)hdr_blk, (unsigned)hblk_pool);

    ppd_l2_buf_hdr_reserve (hdr_blk, rsv_hdr_len);

    if (!out_raw->enables)
        return 0;

    ppd_l2_buf_append(hdr_blk, out_raw->tx_buff, out_raw->tx_buff_len);

    /* This setting is actually only supported for PPPoE classified packets
     * (with IP as PPP type). But should be don't care for other packets.
     */
    if ((pkt_type == TI_PP_IPV4_TYPE) || (pkt_type == TI_PP_IPV6_TYPE))
        l2_hdr_info->l2_frame_code |= TI_PP_EGR_FRM_REFRAME_IP;

    return 0;
}


Int32 ppd_get_l2_hdr(TI_PP_PACKET_DESC *out_pkt, TI_PP_VPID *out_vpid, Uint8 priority, Uint8 rsv_hdr_len, TI_PP_PACKET_TYPE pkt_type, ppd_l2_hdr_info_t* hdr_info)
{
    if(out_pkt->packet_type == TI_PP_ETH_TYPE)
    {
        return ppd_get_802_3_hdr(&out_pkt->u.eth_desc, out_vpid, priority, rsv_hdr_len, pkt_type, hdr_info);
    }

    printErr ("ERROR: Unsupported egress L2 packet type (%d).\n", out_pkt->packet_type);
    return -1;
}

/* Flag that indicates if to do priority mapping according to: 0.both  1.TOS fiels  2.VLAN prio field 3.Feature disabled */
static Uint8 gTrafficPrioritizationMode = TI_PP_PRIORITY_DISABLE_PRIORITIZATION;

/**************************************************************************
 * FUNCTION NAME : Int32 ti_ppd_set_traffic_prioritization_mode(Uint8 priorityMode)
 **************************************************************************
 * DESCRIPTION   :
 *  The function sets the packet processor to do traffic prioritization according to:
 *  TOS fiels only / VLAN priority field only / The highest priority between both.
 *  param[in] priority - "0" = both - default "1" = TOS only "2" = VLAN only
 * RETURNS       :
 *      0  -   Success
 *      >0  -   Error
 **************************************************************************/
Int32 ti_ppd_set_traffic_prioritization_mode(Uint8 priorityMode)
{
    gTrafficPrioritizationMode = priorityMode;
    return 0;
}

/**************************************************************************
 * FUNCTION NAME : Int32 ti_ppd_mapping_egress_queue (TI_PP_SESSION_PROPERTY * out_prop, Uint8 * priority, TI_PPD_IF * egress_if, TI_PPD_IF * ingress_if, Uint8 cluster, Uint8 is_routable)
 **************************************************************************
 * DESCRIPTION   :
 *  The function uses the information passed to set the egress queue according
 *  to the VLAN priority (0-7) and the TOS priority (0-7).
 *  param[in] out_prop - pointer to session egress information
 *            priority - pointer to Uint8 var to return the final priority (0=high priority..3=low priority)
 *            egress_if - pionter to PID information
 *            cluster - cluster ID for egress VPID
 * RETURNS       :
 *      0  -   Success
 *      >0  -   Error
 **************************************************************************/
Int32 ti_ppd_mapping_egress_queue(TI_PP_SESSION_PROPERTY * out_prop, Uint8 * priority, TI_PPD_IF * egress_if, TI_PPD_IF * ingress_if, Uint8 cluster, Uint8 is_routable)
{
    Uint8 vlan_priority             = TI_PP_PRIORITY_INVALID_VALUE;
    Uint8 tos_priority              = TI_PP_PRIORITY_INVALID_VALUE;
    Uint8 mapping_divider           = TI_PP_MAPPING_DIVIDER_FOR_2_PRIORITIES_LEVELS; // Mapping divider for US traffic (in this value we mapp the priority for 2 levels of priority)
    Uint8 originalPriorityValue;

    /* If the session is DS session and not routeble session (bridged session) -> mapped according to DS priority (ignore VLAN/TOS priority) */
    if ( (ingress_if->pid->type == TI_PP_PID_TYPE_DOCSIS) && (is_routable == 0) )
    {
        printk(KERN_DEBUG "%s : Priority mapping is according to DS traffic priority (ignoring VLAN/TOS priority)\n",__FUNCTION__);
        return (0);
    }

    /* Set mapping divider according to number of qos queues for the relevant cluster */
    if (egress_if->vpid->qos_cluster[cluster]->qos_q_cnt >= TI_PP_MAX_PRIORITIES_LEVELS)
    {
        mapping_divider = TI_PP_MAPPING_DIVIDER_FOR_4_PRIORITIES_LEVELS;
    }

    /* Set vlan_priority if there is a VLAN (VLAN packet!), and we are not in TOS_ONLY mode */
    if ( (out_prop->l2_packet.u.eth_desc.enables  & TI_PP_SESSION_L2_VLAN_VALID ) && (gTrafficPrioritizationMode != TI_PP_PRIORITY_TOS_ONLY) )
    {
        originalPriorityValue = (Uint8)((out_prop->l2_packet.u.eth_desc.vlan_tag >> VLAN_PRIO_SHIFT) & TI_PP_PRIORITY_3BIT_MASK );
        /* Highest priority in originalPriorityValue is 7 we need to invert the logic couse the highest priority queue is 0. */
        vlan_priority = ((~originalPriorityValue) & TI_PP_PRIORITY_3BIT_MASK ) / mapping_divider ;
        printk(KERN_DEBUG "%s : VLAN-priority:%d \n", __FUNCTION__, originalPriorityValue );
    }

    /* Set TOS priority if we are not in VLAN_ONLY mode*/
    if (gTrafficPrioritizationMode != TI_PP_PRIORITY_VLAN_ONLY )
    {
        originalPriorityValue = TI_PP_PRIORITY_INVALID_VALUE;
        /* Set tos_priority if the packet L3 is IPv4 */
        if (out_prop->l3l4_packet.packet_type == TI_PP_IPV4_TYPE )
        {
            originalPriorityValue = (out_prop->l3l4_packet.u.ipv4_desc.tos >> TI_PP_PRIORITY_TOS_PRIO_SHIFT) & TI_PP_PRIORITY_3BIT_MASK;
        }
        else if (out_prop->l3l4_packet.packet_type == TI_PP_IPV6_TYPE )
        {
            originalPriorityValue = (out_prop->l3l4_packet.u.ipv6_desc.traffic_class >> TI_PP_PRIORITY_TOS_PRIO_SHIFT) & TI_PP_PRIORITY_3BIT_MASK;
        }

        if (originalPriorityValue != TI_PP_PRIORITY_INVALID_VALUE)
        {
            /* This is unusual case when the TOS priority == 5(EF) then set the highest priority (egress queue #0) */
            if (originalPriorityValue >= 5 ) // DSCP >= EF  (in case priority>=5)  -> must go to queue #0
            {
                tos_priority = 0;
            }
            else
            {
                /* Highest priority in originalPriorityValue is 7 we need to invert the logic couse the highest priority queue is 0. */
                tos_priority = ((~originalPriorityValue) & TI_PP_PRIORITY_3BIT_MASK ) / mapping_divider; 
            }
            printk(KERN_DEBUG "%s : TOS-priority:%d \n", __FUNCTION__, originalPriorityValue );
        }
    }

    /* If we set the vlan_priority and tos_priority, set the min(vlan_priority,tos_priority) -> highest priority */
    if ((vlan_priority != TI_PP_PRIORITY_INVALID_VALUE) && (tos_priority != TI_PP_PRIORITY_INVALID_VALUE))
    {
        *priority = (vlan_priority < tos_priority) ? vlan_priority : tos_priority; 
    }
    /* If we set the vlan_priority only -> set priority according to vlan */
    else if (vlan_priority != TI_PP_PRIORITY_INVALID_VALUE)
    {
        *priority = vlan_priority;
    }
    /* If we set the tos_priority only -> set priority according to TOS */
    else if (tos_priority != TI_PP_PRIORITY_INVALID_VALUE)
    {
        *priority = tos_priority;
    }
    printk(KERN_DEBUG "%s : Session was Mapped to queue #%d \n", __FUNCTION__, *priority );

    return (0);
}




ppd_egress_rec_t* ppd_get_egress_rec (Uint16 num_egress, TI_PPD_IF* egress_if,
                                        TI_PPD_IF* ingress_if,
                                        TI_PP_SESSION_PROPERTY* in_prop,
                                        TI_PP_SESSION_PROPERTY* out_prop,
                                        Uint8 is_routable,
                                        Uint8 priority,
                                        Uint8 cluster,
                                        int isTunnel,
                                        Uint32 *dsLiteFlag)
{
    ppd_egress_rec_t    *first_egress, *egress, *next_egress;
    ppd_pkt_mod_rec_t   *pkt_mod = NULL;
    ppd_l2_hdr_info_t   hdr_info;
    ppd_l2_frame_t      *l2_hdr = NULL;
    Uint32  turbo_tcp_ack_num = 0;          /* TODO: */
    Uint16  egress_queue;
    Uint8   specificProtocolExtraInfo = 0; /* For DSLIte US - Offset of 'payload length' field in template, For GRE US - Offset of payload length' field in added header */
    Uint8   egressFlags = 0;

    *dsLiteFlag = 0;

    first_egress = egress = (ppd_egress_rec_t*) ppd_get_free_blk32();

    while (num_egress--)
    {
        TI_PP_VPID *vpid    = egress_if->vpid;
        TI_PP_PID *pid      = egress_if->pid;
        Uint8   frame_code =0, l2_hdr_size = 0;

        if(!egress) return NULL;;

        egress->w0_next_egr_ptr  = WORD_W0(0);
        egress->w5_l2_hdr_ptr    = WORD_W0(0);
        egress->w6_mod_rec_ptr   = WORD_W0(0);

        if (is_routable)
        {
            /* Note :- In case of multiple egress records, the current
             * implementation  only considers L3/L4 modification for the first
             * egress record. Thus, all subsequent records will have the same
             * modification as in the first record. __IMPORTANT__ This puts
             * restriction on caller to have only one modification allowed
             * across all applicable egress' as well as forces to have the
             * modification to be provided as part of first egress since the
             * succeeding mods will be skipped.
             */
            if((first_egress == egress) && ppd_get_l3_mod_rec(&in_prop->l3l4_packet, &out_prop->l3l4_packet, &pkt_mod))
            {
                goto lbl_mod_err;
            }
            if(pkt_mod)
                egress->w6_mod_rec_ptr = (Uint32)ppd_os_get_io_phys(pkt_mod);
        }

        if(out_prop->l2_packet.packet_type != TI_PP_L2_RAW_TYPE)
        {
            if(ppd_get_l2_hdr(&out_prop->l2_packet, vpid, 0, (vpid->priv_tx_data_len+pid->tx_hw_data_len), out_prop->l3l4_packet.packet_type, &hdr_info))
            {
                goto lbl_frm_err;
            }
        }
        else
        {
            if(ppd_get_l2_raw_hdr(&out_prop->l2_raw_packet.u.l2raw_desc, (vpid->priv_tx_data_len+pid->tx_hw_data_len), out_prop->l3l4_packet.packet_type, &hdr_info))
            {
                goto lbl_frm_err;
            }
        }

        l2_hdr = hdr_info.hdr_blk;
        if (is_routable && (!l2_hdr || !ppd_l2_buf_get_len(l2_hdr)))
        {
            printErr ("ERROR: Required L2 header information not provided for routable session.\n");
            goto lbl_hdr_err;
        }
        if (l2_hdr)
        { 
            if ((in_prop->l3l4_packet.packet_type != out_prop->l3l4_packet.packet_type) && 
                !(in_prop->l2_packet.u.eth_desc.enables & TI_PP_SESSION_L2_GRE_DS_VALID) &&
                !(out_prop->l2_packet.u.eth_desc.enables & TI_PP_SESSION_L2_GRE_US_VALID)) // If ((in->packet_type != out->packet_type) && !gre)
            {
                // This is a DS-Lite packet
                if (in_prop->l3l4_packet.packet_type == TI_PP_IPV4_TYPE && out_prop->l3l4_packet.packet_type == TI_PP_IPV6_TYPE)
                {
                    *dsLiteFlag = TI_PP_EGR_FLAG_DSLITE_US;
                }
                else if (in_prop->l3l4_packet.packet_type == TI_PP_IPV6_TYPE && out_prop->l3l4_packet.packet_type == TI_PP_IPV4_TYPE)
                {
                    *dsLiteFlag = TI_PP_EGR_FLAG_DSLITE_DS;
                }
            }

            ppd_l2_buf_prepend(l2_hdr, &vpid->priv_tx_data[0], vpid->priv_tx_data_len);

            ppd_l2_buf_prepend(l2_hdr, &pid->tx_hw_data[0], pid->tx_hw_data_len);

            if(*dsLiteFlag == TI_PP_EGR_FLAG_DSLITE_US)
            {
                specificProtocolExtraInfo = ppd_l2_buf_get_len(l2_hdr) + TI_PP_IPV6_HEADER_PAYLOAD_LENGTH_OFFSET;
                ppd_l2_buf_append(l2_hdr, out_prop->l3l4_packet.u.ipv6_desc.ipv6HdrRaw, out_prop->l3l4_packet.u.ipv6_desc.ipv6HdrTotalSize);
            }

            /* Check if only L1 data needs to be applied with L2 intact */
            if (ppd_l2_buf_get_len(l2_hdr))
            {
                if (out_prop->l2_packet.u.eth_desc.enables & TI_PP_SESSION_L2_GRE_US_VALID)
                {
                    /* For US GRE - we want to add the L3/GRE/L2 template */
                    if(out_prop->l3l4_packet.packet_type == TI_PP_IPV4_TYPE)
                    {
                        ppd_l2_buf_append(l2_hdr, out_prop->l3l4_packet.u.ipv4_desc.ipv4HdrRaw, out_prop->l3l4_packet.u.ipv4_desc.ipv4HdrRawLen);
                    }
                    else if(out_prop->l3l4_packet.packet_type == TI_PP_IPV6_TYPE)
                    {
                        ppd_l2_buf_append(l2_hdr, out_prop->l3l4_packet.u.ipv6_desc.ipv6HdrRaw, out_prop->l3l4_packet.u.ipv6_desc.ipv6HdrTotalSize);
                    }
                }
                if (!isTunnel)
                {
                    /* For tunnels, PP have different mecahnism for stripping (if needed at all) */
                    frame_code = TI_PP_EGR_FRM_STRIP_L2;
                }
            }

            frame_code |= hdr_info.l2_frame_code;

            l2_hdr_size = ppd_l2_buf_get_len(l2_hdr);
            PAL_CPPI4_CACHE_WRITEBACK_INVALIDATE((void*)l2_hdr->data, PPD_L2_FRM_MAX_HLEN);
            l2_hdr->data = (Uint8*)PAL_osMemVirt2Phy(l2_hdr->data);
            egress->w5_l2_hdr_ptr = (Uint32)WORD_W0(l2_hdr->data);
        }

        next_egress = 0;
        if(num_egress)
        {
            next_egress = (ppd_egress_rec_t*)ppd_get_free_blk32();
            egress->w0_next_egr_ptr  = WORD_W0(ppd_os_get_io_phys((Uint32)next_egress));
            pkt_mod = NULL;
        }

        /* Support TurboDox */
        if (out_prop->app_specific_data.u.app_desc.enables & TI_PP_SESSION_APP_RAW_INFO1_B3_VALID)
        {
            frame_code |= TI_PP_EGR_FRM_TURBODOX_EN;
            turbo_tcp_ack_num = out_prop->app_specific_data.u.app_desc.raw_app_info2;

          /* Support advanced TurboDox option to allow suppression of 12 bytes length TCP option */
            if (out_prop->app_specific_data.u.app_desc.enables & TI_PP_SESSION_APP_RAW_INFO1_B3_PLUS_VALID)
            {
                frame_code |= TI_PP_EGR_FRM_TURBODOX_ADV_EN;
            }
        }

        if ((0 == vpid->qos_clusters_count) || (0xFF == cluster) || (NULL == vpid->qos_cluster[cluster]))
        {
            egress_queue = pid->tx_pri_q_map[0];
        }
        else
        {
            egressFlags |= TI_PP_EGR_FLAG_SESS_MAPPED_TO_LOW_PRIO;// Flag to notice turbodox updates that have permission to move the session to low priority if needed.
            if (gTrafficPrioritizationMode != TI_PP_PRIORITY_DISABLE_PRIORITIZATION)
            {
                /* ti_ppd_mapping_egress_queue() returned 
                 * the priority according to the VLAN and TOS.
                 * 0=highest priority..3=lowest priority */
                ti_ppd_mapping_egress_queue(out_prop, &priority, egress_if, ingress_if, cluster, is_routable);

                /* In order to support the mapping prioritization
                 * in spite of turbo dox changes*/
                if ((pid->type == TI_PP_PID_TYPE_DOCSIS) && 
                    (out_prop->l3l4_packet.u.ipv4_desc.protocol == IPPROTO_TCP) && 
                    (out_prop->app_specific_data.u.app_desc.enables & TI_PP_SESSION_APP_RAW_INFO1_B3_VALID))
                {
                    if (priority)
                    {
                        priority = 0;
                    }
                    else
                    {
                        egressFlags &= ~TI_PP_EGR_FLAG_SESS_MAPPED_TO_LOW_PRIO;
                    }
                }
            }
            egress_queue = vpid->qos_cluster[cluster]->qos_q_cfg[priority].q_num;
            egress_queue += PAL_CPPI41_SR_QPDSP_QOS_Q_BASE;
        }

        /* TODO: Add support for Ingress Priority Mode */
        egress->w1.frags    = WORD_B3_0(/*CHK*/pid->dflt_pri_drp, vpid->vpid_handle, frame_code, l2_hdr_size);
        egress->w2.frags    = WORD_S1_0(/*CHK*/pid->dflt_dst_tag, egress_queue );



        if (in_prop->l2_packet.u.eth_desc.enables & TI_PP_SESSION_L2_GRE_DS_VALID)
        {
            egressFlags |= TI_PP_EGR_FLAG_GRE_DS;
        }
        else if (out_prop->l2_packet.u.eth_desc.enables & TI_PP_SESSION_L2_GRE_US_VALID)
        {
            egressFlags |= TI_PP_EGR_FLAG_GRE_US;
            if (out_prop->l3l4_packet.packet_type == TI_PP_IPV6_TYPE )
            {
                specificProtocolExtraInfo = out_prop->l3l4_packet.u.ipv6_desc.ipv6HdrRawOffset + TI_PP_IPV6_HEADER_PAYLOAD_LENGTH_OFFSET;
                egressFlags |= TI_PP_EGR_FLAG_NEW_HEADER_HAS_IPV6;
            }
            else
            {
                specificProtocolExtraInfo = out_prop->l3l4_packet.u.ipv4_desc.ipv4HdrRawOffset + TI_PP_IPV4_HEADER_TOTAL_LENGTH_OFFSET;
            }
        }
        else if(*dsLiteFlag == TI_PP_EGR_FLAG_DSLITE_US)
        {
            egressFlags |= TI_PP_EGR_FLAG_DSLITE_US | TI_PP_EGR_FLAG_NEW_HEADER_HAS_IPV6;
        }
        else if(*dsLiteFlag == TI_PP_EGR_FLAG_DSLITE_DS)
        {
            egressFlags |= TI_PP_EGR_FLAG_DSLITE_DS;
        }
        

        egress->w3.frags = WORD_B3_0(specificProtocolExtraInfo, egressFlags, pid->type, hdr_info.offset_802_3_len);
        egress->w4_turbo_tcp_ack_num = WORD_W0(turbo_tcp_ack_num);

        /* DOCSIS/TurboDox application specific handling */
        if (out_prop->app_specific_data.u.app_desc.enables != 0)
        {
            egress->w7_proto_spec = WORD_W0(out_prop->app_specific_data.u.app_desc.u.raw_app_info1);
        }
        else
        {
            egress->w7_proto_spec = WORD_W0(0);
        }


        egress = next_egress;
        egress_if++;
        out_prop++;
    }

    return first_egress;

lbl_hdr_err:
lbl_frm_err:
lbl_mod_err:
    ppd_cleanup_egress_list (first_egress);
    return NULL;
}

ppd_ipv6_rec_t* ppd_get_ipv6_rec (TI_PP_IPV6_DESC *ipv6_desc,
                                    Uint16* ipv6_addr_flags)
{
    ppd_ipv6_rec_t* ipv6_rec = NULL;

    if (ipv6_desc->enables & TI_PP_SESSION_IPV6_DSTIP_VALID)
        *ipv6_addr_flags |= TI_PP_SES_FLAG_USE_FULL_DST_IPV6;

    if (ipv6_desc->enables & TI_PP_SESSION_IPV6_SRCIP_VALID)
        *ipv6_addr_flags |= TI_PP_SES_FLAG_USE_FULL_SRC_IPV6;

    if (*ipv6_addr_flags
            && ((ipv6_rec = (ppd_ipv6_rec_t*)ppd_get_free_blk32())))
    {
        ppd_memset_word ((Uint32*)ipv6_rec, 0, SIZE_IN_WORD(ppd_ipv6_rec_t));
        if (*ipv6_addr_flags & TI_PP_SES_FLAG_USE_FULL_DST_IPV6)
        {
            ipv6_rec->w4_dst_w3 = WORD_W0(ipv6_desc->dst_ip[0]);
            ipv6_rec->w5_dst_w2 = WORD_W0(ipv6_desc->dst_ip[1]);
            ipv6_rec->w6_dst_w1 = WORD_W0(ipv6_desc->dst_ip[2]);
            ipv6_rec->w7_dst_w0 = WORD_W0(ipv6_desc->dst_ip[3]);
        }

        if (*ipv6_addr_flags & TI_PP_SES_FLAG_USE_FULL_SRC_IPV6)
        {
            ipv6_rec->w0_src_w3 = WORD_W0(ipv6_desc->src_ip[0]);
            ipv6_rec->w1_src_w2 = WORD_W0(ipv6_desc->src_ip[1]);
            ipv6_rec->w2_src_w1 = WORD_W0(ipv6_desc->src_ip[2]);
            ipv6_rec->w3_src_w0 = WORD_W0(ipv6_desc->src_ip[3]);
        }
    }

    return ipv6_rec;
}


#define TDOX_QOS_QUEUE_COUNT                2
#define TDOX_HIGH_PRIORITY_QOS_QUEUE_INDEX  0
#define TDOX_LOW_PRIORITY_QOS_QUEUE_INDEX   1

int ti_ppd_session_is_tdox_set( unsigned char ses_id )
{
    ppd_ses_info_blk_t  *ses    = (ppd_ses_info_blk_t*)(gp_ses_blk + ses_id);
    ppd_egress_rec_t    *egress = (ppd_egress_rec_t*)ppd_os_get_io_virt(ses->w3_egress_ptr);

    return ((egress->w1.s.frame_code & TI_PP_EGR_FRM_TURBODOX_EN) != 0);
}

void ti_ppd_session_tdox_change(unsigned char ses_id, unsigned char tdoxId, Uint32 enableFlags)
{
    ppd_ses_info_blk_t  *ses    = (ppd_ses_info_blk_t*)(gp_ses_blk + ses_id);
    ppd_egress_rec_t    *egress = (ppd_egress_rec_t*)ppd_os_get_io_virt(ses->w3_egress_ptr);

    if (enableFlags & TI_PP_SESSION_APP_RAW_INFO1_B3_VALID)
    {
        egress->w1.s.frame_code |= TI_PP_EGR_FRM_TURBODOX_EN;
        if ((egress->w2.s.fwd_q_index & (TDOX_QOS_QUEUE_COUNT - 1)) == TDOX_LOW_PRIORITY_QOS_QUEUE_INDEX)
        {
            egress->w2.s.fwd_q_index -= 1; // Move to high priority QoS queue
        }
        egress->w7_proto_spec = (egress->w7_proto_spec & 0xFFFFFF00) | (Uint32) tdoxId;
    }
    else
    {
        egress->w1.s.frame_code &= ~TI_PP_EGR_FRM_TURBODOX_EN;
        if (((egress->w2.s.fwd_q_index & (TDOX_QOS_QUEUE_COUNT - 1)) == TDOX_HIGH_PRIORITY_QOS_QUEUE_INDEX)
            && (egress->w3.s.flags & TI_PP_EGR_FLAG_SESS_MAPPED_TO_LOW_PRIO))   // If we want to move to low priority chack first if this session was mapped to high priority in the session creation
        {
            egress->w2.s.fwd_q_index += 1; // Move to low priority QoS queue
        }
    }

    if (enableFlags & TI_PP_SESSION_APP_RAW_INFO1_B3_PLUS_VALID)
    {
        egress->w1.s.frame_code |= TI_PP_EGR_FRM_TURBODOX_ADV_EN;
    }
    else
    {
        egress->w1.s.frame_code &= ~TI_PP_EGR_FRM_TURBODOX_ADV_EN;
    }

}


Int32 ti_ppd_create_session (TI_PP_SESSION* ses_cfg, TI_PPD_IF *ingress_if, TI_PPD_IF *egress_if, int isTunnel)
{
    TI_PP_ses_id_t  ses_id  = (TI_PP_ses_id_t)ses_cfg->session_handle;
    TI_PP_VPID *ingress_vpid    = ingress_if->vpid;
    Int32   ret_val = -1;
    Uint32  cookie;
    Uint32 dsLiteFlag;  
    /*
     * CHK: Always setting IDLE base timeout since there is no way to configure
     * otherwise in current data structure
     */
    Uint16  session_flags       = TI_PP_SES_FLAG_IDLE_TMOUT;

    ppd_ses_info_blk_t  *ses    = (ppd_ses_info_blk_t*)(gp_ses_blk + ses_id);
    ppd_egress_rec_t    *egress;

    if (!g_ppd_init_done)
    {
        return -1;
    }

    PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &cookie);
    if (IS_SESSION_SLOT_FILLED(ses_id))
    {
        printErr ("%s: ***Error: session %d already exists or being created.\n", __FUNCTION__, ses_id);
        PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);
        return -1;
    }

    g_session_slots[(Uint32)(ses_id)/32] |= (1<<((Uint32)(ses_id)%32));

    PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);

    /* Clears leftover stats */
    ppd_memset_word ((Uint32 *)ses, 0, SIZE_IN_WORD(ppd_ses_info_blk_t));

    egress = ppd_get_egress_rec (ses_cfg->num_egress, egress_if, ingress_if,
                                    &ses_cfg->ingress, &ses_cfg->egress[0],
                                    ses_cfg->is_routable_session,
                                    ses_cfg->priority,
                                    ses_cfg->cluster, isTunnel, &dsLiteFlag);
    if (!egress)
        goto lbl_session_err;

    ses->w3_egress_ptr = (Uint32)ppd_os_get_io_phys(egress);

    printMsg ("ti_ppd_create_session: session @%#x, egress = %#x, "
            "Phys = %#x\n", (unsigned)ses, (unsigned)egress,
            (unsigned)ppd_os_get_io_phys(egress)/*ses->w3_egress_ptr*/);

    if (ses_cfg->session_timeout == 0)
    {
        session_flags |= TI_PP_SES_FLAG_PASS_AFTER_TMOUT;
    }

    /* Check if this is a bridged or routed session? */
    if (ses_cfg->is_routable_session == 1)
    {
        /* All routable sessions will need their TTL Flags decremented by the
         * PDSP.
         */
        session_flags |= TI_PP_SES_FLAG_UPDATE_TTL;
    }
    else
    {
        /* All bridged sessions dont need to have their TTL Flags updated, since this is layer2 operation.
         * Besides for bridged sessions Fragmented can be handled by the PDSP firmware itself. 
         * The host bridging layer does not care about such frames. Same applies for packets with IP options */
        session_flags |= TI_PP_SES_FLAG_IP_FRAG | TI_PP_SES_FLAG_PROC_IP_OPTS;
    }

    /* TBD - open this in case we do not want PP to send DS GRE fragmented packets to the host
    if (ses_cfg->ingress.l2_packet.u.eth_desc.enables & TI_PP_SESSION_GRE_DS_VALID)
    {
        session_flags |= TI_PP_SES_FLAG_IP_FRAG;
    }
    */

    // support DSLite DS Fragment Reassembly
    if (dsLiteFlag == TI_PP_EGR_FLAG_DSLITE_DS)
    {
        session_flags |= TI_PP_SES_FLAG_IP_FRAG;
    }

    /* Set IPv6 Address Record */
    if (ses_cfg->ingress.l3l4_packet.packet_type == TI_PP_IPV6_TYPE)
    {
        Uint16 ipv6_addr_flags = 0;
        ppd_ipv6_rec_t* ipv6_rec = ppd_get_ipv6_rec (&ses_cfg->ingress.
                                    l3l4_packet.u.ipv6_desc, &ipv6_addr_flags);

        if (ipv6_addr_flags)
        {
            if (!ipv6_rec)
                goto lbl_session_err;

            session_flags |= ipv6_addr_flags;
            ses->w1_ipv6_rec  = ppd_os_get_io_phys(ipv6_rec);
        }

        /* Set IPv6 packet class level.
         * TODO: Currently forcing highest possible class level so that all
         * majority of the session packets pass trhough. Once we have some way
         * to get this value from application, we need to modify here
         */
        session_flags |= TI_PP_SES_FLAG_IPV6_CLASS_SET(0x3);
    }

    ses->w0.frags = WORD_B1_0_S0(ingress_vpid->vpid_handle, 0, session_flags);
    ses->w2.frags = WORD_B3_0(0, 0, 0, 0);

    /* Convert provided usecs as 10^-5 seconds with mimnimum of 10us */
#ifdef CONFIG_INTEL_PP_TUNNEL_SUPPORT
    if (isTunnel)
    {
        ses->w4.w4_tunnel_config = ses_cfg->session_timeout;
        ses->w5.w5_tunnel_vlans = ses_cfg->tunnel_vlans;
    }
    else
#endif
    {
        ses->w4.w4_ses_timeout  = (ses_cfg->session_timeout/10)
                                + ((ses_cfg->session_timeout%10) ? 1 : 0);
    }

    {
        ppd_lut_t lut_data = {ses_cfg, ingress_vpid};
        cpdsp_cmd_t cmd_word = WORD_S0_B1_0(ses_id, SESSION_ADD_SYNC, SRPDSP_SESSION_COMMAND);

        if (isTunnel)
        {
            // Tunnel mode is not using the LUT
            ret_val = cpdsp_cmd_rsp_by_sz(cmd_word, 0, 0, 0, 0);
        }
        else
        {
            Uint32 cmdParam[20];
            Uint32 writeSize;
            
            ret_val = ppd_wr_lut(cmdParam, &lut_data);
            writeSize = 12;
            
            ret_val |= cpdsp_cmd_rsp_by_sz(cmd_word, cmdParam, writeSize, 0, 0); 

#if 0
            printk("Add ses_id=%d, LUT1: 0x%08X, 0x%08X, 0x%08X, 0x%08X ,0x%08X ,0x%08X ,0x%08X ,0x%08X ,0x%08X ,0x%08X\n",
                   ses_id, cmdParam[0], cmdParam[1], cmdParam[2], cmdParam[3], cmdParam[4], cmdParam[6], cmdParam[7], cmdParam[8], cmdParam[9], cmdParam[10]);
            printk("LUT2: 0x%08X, 0x%08X, 0x%08X, 0x%08X ,0x%08X ,0x%08X ,0x%08X ,0x%08X\n",
                   cmdParam[12], cmdParam[13], cmdParam[14], cmdParam[15], cmdParam[16], cmdParam[17], cmdParam[18], cmdParam[19]);
#endif
        }
    }

    if (ret_val)
    {
        goto lbl_session_err;
    }


     /* Sample (CPDSP) cycle counts to determine session age at later time.
      * Since we are sampling the pdsp poll timer maintained counter, in worst
      * case we will read around 10 seconds less, which means session age will
      * be 9-10 sec less than the actual in such case.
      */
    g_ses_timestamp [ses_id].cycle_cnt_hi = g_pdsp_counts [0].cycle_sec_cnt_hi;
    g_ses_timestamp [ses_id].cycle_cnt_lo = g_pdsp_counts [0].cycle_sec_cnt_lo;

    return ret_val;

lbl_session_err:
    if (ses->w1_ipv6_rec)
       ppd_free_blk32((ppd_block32_t*) ppd_os_get_io_virt(ses->w1_ipv6_rec));

    ppd_cleanup_egress_list (egress);
    PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &cookie);
    g_session_slots[(Uint32)(ses_id)/32] &= ~(1<<((Uint32)(ses_id)%32));
    PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);
    return ret_val;
}

Int32 ti_ppd_delete_session(Uint8 session_handle)
{
    Int32 ret_val;
    Uint32 cookie;
    
    ppd_ses_info_blk_t  *ses    = (ppd_ses_info_blk_t*)(gp_ses_blk + session_handle);
    cpdsp_cmd_t cmd_word = WORD_S0_B1_0(session_handle, SESSION_REMOVE, SRPDSP_SESSION_COMMAND);

    PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &cookie);
    if (!IS_SESSION_SLOT_FILLED(session_handle)) 
    {
        printErr ("%s: ***Error: session %d does not exist.\n", __FUNCTION__, session_handle);
        PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);
        return -1;
    }
    PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);

    ret_val = cpdsp_cmd_rsp_by_sz(cmd_word, 0, 0, 0, 0);

    if (ret_val)
    {
        printErr ("Failed to destroy session @%#x\n", (Uint32)ses);
        return ret_val;
    }

    /* CHK: We are cleaning up after destroying the session. This will work if
     * the egress lists are not cleared by firmware on session remove.
     */

    ppd_destroy_session(ses);

    g_ses_timestamp [session_handle].cycle_cnt_hi = 0;
    g_ses_timestamp [session_handle].cycle_cnt_lo = 0;

    PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &cookie);
    g_session_slots[(Uint32)(session_handle)/32] &= ~(1<<((Uint32)(session_handle)%32));
    PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);

    return 0;
}

Int32 ti_ppd_get_session_pkt_stats (TI_PP_ses_id_t ses_id, TI_PP_SESSION_STATS *stats)
{

    cpdsp_cmd_t cmd_word =
        WORD_S0_B1_0(ses_id, STATS_TYPE_SESSION, SRPDSP_SR_STATS_GET);

    return (cpdsp_cmd_rsp_by_sz(cmd_word, 0, 0, stats, SIZE_IN_WORD(*stats)));
}

Int32 ti_ppd_clear_session_pkt_stats(Uint8 session_handle)
{
    cpdsp_cmd_t cmd_word =
        WORD_S0_B1_0(session_handle, STATS_TYPE_SESSION, SRPDSP_SR_STATS_CLEAR);

    return (cpdsp_cmd_rsp_by_sz(cmd_word, 0, 0, 0, 0));
}


Int32 ti_ppd_get_n_clear_session_pkt_stats (TI_PP_ses_id_t ses_id,
                                            TI_PP_SESSION_STATS *stats)
{





    /* todo: we are not reading all the return params */
    cpdsp_cmd_t cmd_word =
        WORD_S0_B1_0(ses_id, STATS_TYPE_SESSION, SRPDSP_SR_STATS_CLEAR);

    return (cpdsp_cmd_rsp_by_sz(cmd_word, 0, 0, stats, SIZE_IN_WORD(*stats)));
}

Int32 ti_ppd_set_ack_suppression(int enDis)
{
    cpdsp_cmd_t cmd_word = WORD_S0_B1_0(enDis, 0, SRPDSP_SR_ACK_SUPPRESS);
    return (cpdsp_cmd_rsp_by_sz(cmd_word, 0, 0, 0, 0));
}
#ifdef CONFIG_INTEL_PP_TUNNEL_SUPPORT
Int32 ti_ppd_set_tunnel_mode(int tunnelMode)
{
    cpdsp_cmd_t cmd_word =
        WORD_S0_B1_0(tunnelMode, 0, SRPDSP_SR_TUNNEL_MODE);

    return (cpdsp_cmd_rsp_by_sz(cmd_word, 0, 0, 0, 0));
}

Int32 ti_ppd_set_cm_mac_address(Uint8* cmAddress)
{
    cpdsp_cmd_t cmd_word =
        WORD_S0_B1_0(0, 0, SRPDSP_SR_CM_ADDR);

    return (cpdsp_cmd_rsp_by_sz(cmd_word, cmAddress, 2, 0, 0));
}
#endif

#define TI_PPD_TDOX_EVALUATION_PERIOD_IN_SEC           (1)
#define TI_PPD_TDOX_EVALUATION_ONCE_IN_X_TIMER         (TI_PPD_TDOX_EVALUATION_PERIOD_IN_SEC * 1000 / EVENT_POLLTIME_MSECS)    /* Divide by EVENT_POLLTIME_MSECS to get X */
static void  ppd_event_timer(unsigned long data)
{
    Cppi4EmbdDesc* emDesc = NULL;
    ppd_event_hdlr_cfg_t* event_cfg = (ppd_event_hdlr_cfg_t*)data;
    int pacing_iteration = 16;
    static Uint32 evaluationIteration = 0;

    if (g_is_in_psm)
    {
        return;
    }

    do
    {
        if ((emDesc = (Cppi4EmbdDesc*)PAL_cppi4QueuePop (event_cfg->eventQHnd)))
        {
            emDesc = (Cppi4EmbdDesc*)PAL_CPPI4_PHYS_2_VIRT(emDesc);

            PAL_CPPI4_CACHE_INVALIDATE(emDesc, sizeof(Cppi4EmbdDesc)); 

            printMsg("ppd_event_timer: Got event -> EPI0 = %x\n", emDesc->EPI[0]);

            if (g_ppd_event_hdlr_cfg.pfnEventHdlr)
            {
                event_cfg->pfnEventHdlr ((emDesc->EPI[0] & 0xff), ((emDesc->EPI[0]>>16) & 0xffff), ((emDesc->EPI[0]>>8) & 0xff));
            }

            PAL_cppi4QueuePush (event_cfg->eventFQHnd, (Ptr)PAL_CPPI4_VIRT_2_PHYS(emDesc), PAL_CPPI4_DESCSIZE_2_QMGRSIZE(sizeof(Cppi4EmbdDesc)), 0);
        }
        pacing_iteration--;
    }
    while ((emDesc) && (pacing_iteration));

    if (++evaluationIteration >= TI_PPD_TDOX_EVALUATION_ONCE_IN_X_TIMER)
    {
        /* TDOX sessions evaluation */
        event_cfg->pfnEventHdlr(TI_PPD_INTERNAL_EVENT_TDOX_EVALUATION, 0, 0);
        evaluationIteration = 0;
    }

    PAL_osTimerStart (event_cfg->hTimer, event_cfg->polltime_msecs);
}

Int32 ti_ppd_register_event_handler (TI_PPD_EVENT_HANDLER ptr_event_handler)
{
    Uint32 cookie;

    PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &cookie);
    if (g_ppd_event_hdlr_cfg.pfnEventHdlr)
    {
        PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);
        return -1;
    }
    g_ppd_event_hdlr_cfg.pfnEventHdlr = ptr_event_handler;
    PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);


    return 0;
}

Int32 ti_ppd_deregister_event_handler ()
{
    Uint32 cookie;

    PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &cookie);
        g_ppd_event_hdlr_cfg.pfnEventHdlr = NULL;
    PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);

    return 0;
}

Int32 ti_ppd_health_check()
{
    Int32 ret_val;
    Uint32 pdspStatus[2];

    cpdsp_cmd_t cmd_word = WORD_S0_B1_0(0, 0, SRPDSP_SR_STATUS);

    if ((ret_val = cpdsp_cmd_rsp_by_sz(cmd_word, 0, 0, pdspStatus, 2)) == 0)
    {
        if (pdspStatus[0])
        {
            ret_val |= (1<<0); /* CPDSP */
        }
        if (pdspStatus[1])
        {
            ret_val |= (1<<1); /* CPDSP */
        }
    }

    return ret_val;
}

/*
 *                      QOS APIs
 */
#if ((PPD_DEBUG) & PPD_DEBUG_MSG)
static Int32 ppd_dump_qos_qcfg (Uint32* ptr, Uint8* dump_buff_p, Int32 buff_len)
{
    Int32 len;
    len =  sprintf(dump_buff_p,  WSTR_S0_B1_0(0, *ptr,     "Iter Cr", "Egr Q", "Flags"));
    len += sprintf(dump_buff_p + len, WSTR_W0(1, *(ptr+1), "Tot Cr"));
    len += sprintf(dump_buff_p + len, WSTR_W0(2, *(ptr+2), "Max Cr"));
    len += sprintf(dump_buff_p + len, WSTR_W0(3, *(ptr+3), "Con Thrsh"));
    len += sprintf(dump_buff_p + len, WSTR_W0(4, *(ptr+4), "Con Thrsh Pkts"));

    return len;
}

static Int32 ppd_dump_qos_clst (Uint32* ptr, Uint8* dump_buff_p, Int32 buff_len)
{
    Int32 len;
    len = sprintf(dump_buff_p, WSTR_W0(0, *ptr, "Glbl Cr"));
    len += sprintf(dump_buff_p + len, WSTR_W0(1, *(ptr+1), "Max Glbl"));
    len += sprintf(dump_buff_p + len,
                WSTR_B3_0(2, *(ptr+2), "QOS Q2", "QOS Q1", "QOS Q0", "QOS Qs"));
    len += sprintf(dump_buff_p + len,
                WSTR_B3_0(3, *(ptr+3), "QOS Q6", "QOS Q5", "QOS Q4", "QOS Q3"));
    len += sprintf(dump_buff_p + len,
                WSTR_B3_0(4, *(ptr+4), "EGR Q0", "EGR Qs", "QOS Q8", "QOS Q7"));
    len += sprintf(dump_buff_p + len,
                WSTR_B3_0(5, *(ptr+5), "EGR Q4", "EGR Q3", "EGR Q2", "EGR Q1"));
    len += sprintf(dump_buff_p + len,
                WSTR_B3_0(6, *(ptr+6), "EGR Q8", "EGR Q7", "EGR Q6", "EGR Q5"));
    len += sprintf(dump_buff_p + len,
                WSTR_W0(7, *(ptr+7), "Egr Thrsh1"));
    len += sprintf(dump_buff_p + len,
                WSTR_W0(8, *(ptr+8), "Egr Thrsh2"));
    len += sprintf(dump_buff_p + len,
                WSTR_W0(9, *(ptr+9), "Egr Thrsh3"));
    len += sprintf(dump_buff_p + len,
                WSTR_W0(10, *(ptr+10), "Egr Thrsh4"));
    len += sprintf(dump_buff_p + len,
                WSTR_W0(11, *(ptr+11), "Egr Thrsh Pkts"));

    return len;
}

#endif /* ((PPD_DEBUG) & PPD_DEBUG_MSG) */

#define PP_MTA_MAC_BASE                 ((Uint32)ppd_os_get_io_virt(0x03100070))
#define PP_SES_STATE_BLK_BASE           ((Uint32)ppd_os_get_io_virt(0x03100900))
#define PP_PID_BLK_BASE                 ((Uint32)ppd_os_get_io_virt(0x03100100))
#define PP_QOS_CLST_BLK_BASE_VIRT       ((Uint32)ppd_os_get_io_virt(0x03120040))
#define PP_QOS_QCFG_BLK_BASE_VIRT       ((Uint32)ppd_os_get_io_virt(0x03100c00))
#define PP_QOS_CLST_MAX_QCNT            9

#define PP_QOS_CLST_MAX_INDX            31
#define PP_QOS_QUEUE_MAX_INDX           (PAL_CPPI41_SR_QPDSP_QOS_Q_LAST - PAL_CPPI41_SR_QPDSP_QOS_Q_BASE)

typedef struct
{
    union
    {
        STRUCT_B0_1_S0 (
                reserved,
                flags,                      /* Specifies how the frames in the QOS queue should be handled */
                egr_q                       /* The Queue index of the forwarding queue */
                ) s;
        Uint32 frags;
    } w0;


    union
    {
        STRUCT_S0_1 (
                iteration_credit_pkts,       /* The amount of packet credit that the queue receives every 25us. */
                iteration_credit_bytes     /* The amount of byte credit that the queue receives every 25us. */
                ) s;
        Uint32 frags;
    }w1;

    Uint32 w2_total_credit_bytes;         /* The total amount of forwarding byte credit that the queue is currently holding */

    Uint32 w3_max_credit_bytes;          /* The max amount of forwarding byte crerdit that the queue is allowed to hold at the end of the 25US iteration */

    union
    {
        STRUCT_S0_1 (
                total_credit_pkts,           /* The total amount of forwarding pkt credit that the queue is currently holding */
                max_credit_pkts             /* The max amount of forwarding packet crerdit that the queue is allowed to hold at the end of the 25US iteration */
                ) s;
        Uint32 frags;
    }w4;

    union
    {
        STRUCT_S0_1 (
                congst_thrsh_pkts,           /* The size in packets at which point the Qos queue is considered to be congested */
                congst_thrsh_bytes         /* The size in bytes at which point the Qos queue is considered to be congested */
                ) s;
        Uint32 frags;
    }w5;

    Uint32 w6_fwd_pkts_cnt;                 /* Number of pkts forwarded from this queue to the egress queue */
    Uint32 w7_drp_pkts_cnt;                 /* Number of pkts discarded from this queue */

} ppd_qos_qcfg_blk_t;



typedef struct
{

    Uint32 w0_global_credit_bytes;        /* The amount of global credit bytes available to the next Qos queue in the cluster */


    union
    {
        STRUCT_S0_1 (
                max_global_credit_pkts,   /* The max amount of global credit pkts allowed carring over to the next queue. */
                global_credit_pkts        /* The amount of global credit packet available to the next Qos queue in the cluster */

                ) s;
        Uint32 frags;
    }w1;

    Uint32 w2_max_global_credit_bytes;     /* The max amount of global credit bytes allowed carring over to the next queue. */

    union
    {
        STRUCT_B0_3 (
                qos_q_cnt,                 /* The number of QOS queues in the cluster (1 to 9) */
                qos_q0,                    /* The queue index (0 to 127) of each QOS queue in the cluster listed in priority order */
                qos_q1,
                qos_q2
                ) s;
        Uint32 frags;
    } w3;

    union
    {
        STRUCT_B0_3 (
                qos_q3,
                qos_q4,
                qos_q5,
                qos_q6
                ) s;
        Uint32 frags;
    } w4;

    union
    {
        STRUCT_B0_3 (
                qos_q7,
                qos_q8,
                egr_q_cnt,                   /* The total number of egress queues sampled to obtain the egress queue congestion estimation */
                egr_q0                       /* The Queue index of every egress queue enumerated in Egress Queue Count */
                ) s;
        Uint32 frags;
    } w5;

    union
    {
        STRUCT_B0_3 (
                egr_q1,
                egr_q2,
                egr_q3,
                egr_q4
                ) s;
        Uint32 frags;
    } w6;

    union
    {
        STRUCT_B0_3 (
                egr_q5,
                egr_q6,
                egr_q7,
                egr_q8
                ) s;
        Uint32 frags;
    } w7;

     union
    {
        STRUCT_S0_1 (
                egr_congst_thrsh_bytes2,     /* Egress Congestion Threshold Bytes point 2 (Resolution: 64 bytes)*/
                egr_congst_thrsh_bytes1      /* Egress Congestion Threshold Bytes point 1 (Resolution: 64 bytes)*/
                ) s;
        Uint32 frags;
    }w8;

     union
    {
        STRUCT_S0_1 (
                egr_congst_thrsh_bytes4,     /* Egress Congestion Threshold Bytes point 4 (Resolution: 64 bytes)*/
                egr_congst_thrsh_bytes3      /* Egress Congestion Threshold Bytes point 3 (Resolution: 64 bytes) */
                ) s;
        Uint32 frags;
    }w9;

    union
    {
        STRUCT_B0_3 (
                egr_congst_thrsh_pkts4,      /* Egress Congestion Threshold Packets point 1 (Resolution: 16 packets) */
                egr_congst_thrsh_pkts3,      /* Egress Congestion Threshold Packets point 2 (Resolution: 16 packets) */
                egr_congst_thrsh_pkts2,      /* Egress Congestion Threshold Packets point 3 (Resolution: 16 packets) */
                egr_congst_thrsh_pkts1       /* Egress Congestion Threshold Packets point 4 (Resolution: 16 packets) */
                ) s;
        Uint32 frags;
    } w10;
    Uint32  w11;                            /* Reserve*/

} ppd_qos_clst_blk_t;


Int32 ti_ppd_set_mta_mac_address(Uint8* mtaAddress)
{
    Uint8* dest = (Uint8*)(PP_MTA_MAC_BASE);
    memcpy(dest, mtaAddress, 6);
    return 0;
}

/*
 * This function populates the egress queue array with unique egress queues and
 * provides the count of number of unique entries in the array.
 */
void ppd_qos_add_egr_q(Uint16* p_egrq, Uint8* egrq_cnt, Uint16 egrq)
{
    int i, is_match_found = 0;

    for (i = 0; i < *egrq_cnt; i++)
    {
        if (p_egrq[i] == egrq)
        {
            is_match_found = 1;
            break;
        }
    }

    if (!is_match_found)
    {
        p_egrq[i] = egrq;
        *egrq_cnt += 1;
    }
}

Int32 ti_ppd_qos_cluster_setup (Uint8 clst_indx, TI_PP_QOS_CLST_CFG* clst_cfg)
{
    Int32   i;
    ppd_qos_clst_blk_t  *clst = (ppd_qos_clst_blk_t*)(PP_QOS_CLST_BLK_BASE_VIRT) + clst_indx;
    Uint16  egr_q[PP_QOS_QUEUE_MAX_INDX], qos_qnum[PP_QOS_QUEUE_MAX_INDX];
    Uint8   egr_q_cnt = 0;

    if (!g_ppd_init_done || (clst_indx > PP_QOS_CLST_MAX_INDX) || (clst_cfg->qos_q_cnt > PP_QOS_CLST_MAX_QCNT)) 
    {
        return -1;
    }

    memset (egr_q, 0, sizeof(egr_q));
    memset (qos_qnum, 0, sizeof(qos_qnum));

    /* Configure QOS queues */
    for (i = 0; i < clst_cfg->qos_q_cnt; i++)
    {
        TI_PP_QOS_QUEUE* qos_q_cfg = &clst_cfg->qos_q_cfg[i];
        ppd_qos_qcfg_blk_t  *qcfg;

        /* Store the queue number to be used for cluster setup */
        qos_qnum[i] = qos_q_cfg->q_num;

        if (qos_qnum[i] > PP_QOS_QUEUE_MAX_INDX)
        {
            return -1;
        }
        qcfg =  (ppd_qos_qcfg_blk_t*)(PP_QOS_QCFG_BLK_BASE_VIRT) + qos_qnum[i];

        /* Collect the egress queue if it was not set by prior Qs, to be
         * configured in cluster
         */
        ppd_qos_add_egr_q(&egr_q[0], &egr_q_cnt, qos_q_cfg->egr_q);

        qcfg->w0.frags      = WORD_S0_B1_0(qos_q_cfg->egr_q, qos_q_cfg->flags, 0);
        qcfg->w1.frags      = WORD_S1_0(qos_q_cfg->it_credit_bytes, qos_q_cfg->it_credit_packets);

        /* init total credit bytes and total credit pkts to zero, this values will be calculate in the Qos pdsp */
        qcfg->w2_total_credit_bytes      = 0;

        qcfg->w3_max_credit_bytes =  WORD_W0(qos_q_cfg->max_credit_bytes);

        qcfg->w4.frags      = WORD_S1_0(qos_q_cfg->max_credit_packets, 0);


        qcfg->w5.frags  = WORD_S1_0(qos_q_cfg->congst_thrsh_bytes, qos_q_cfg->congst_thrsh_packets);


        qcfg->w6_fwd_pkts_cnt   = 0;
        qcfg->w7_drp_pkts_cnt   = 0;

#if ((PPD_DEBUG) & PPD_DEBUG_MSG)
        printMsg ("QOS: Queue %d config dump:\n", i);
        ppd_dump_qos_qcfg ((Uint32*)qcfg, dump_buff, sizeof(dump_buff));
        printMsg("%s\n",dump_buff);
#endif
    }

    /* Setup the cluster */
    clst->w0_global_credit_bytes = WORD_W0(clst_cfg->global_credit_bytes);

    clst->w1.frags      = WORD_S1_0(clst_cfg->global_credit_packets, clst_cfg->max_global_credit_packets);

    clst->w2_max_global_credit_bytes = WORD_W0(clst_cfg->max_global_credit_bytes);



    clst->w3.frags                    = WORD_B3_0(qos_qnum[2], qos_qnum[1],
                                                  qos_qnum[0], clst_cfg->qos_q_cnt);
    clst->w4.frags                    = WORD_B3_0(qos_qnum[6], qos_qnum[5],
                                                  qos_qnum[4], qos_qnum[3]);
    clst->w5.frags                    = WORD_B3_0(egr_q[0], egr_q_cnt,
                                                  qos_qnum[8], qos_qnum[7]);
    clst->w6.frags                    = WORD_B3_0(egr_q[4], egr_q[3],
                                                  egr_q[2], egr_q[1]);
    clst->w7.frags                    = WORD_B3_0(egr_q[8], egr_q[7],
                                                  egr_q[6], egr_q[5]);
    clst->w8.frags                    = WORD_S1_0(clst_cfg->egr_congst_thrsh_bytes1, clst_cfg->egr_congst_thrsh_bytes2);
    clst->w9.frags                    =  WORD_S1_0(clst_cfg->egr_congst_thrsh_bytes3, clst_cfg->egr_congst_thrsh_bytes4);

    /* If the first congst tresh hold is zero the congestion thrshold mechanism is disable */
    if( (clst_cfg->egr_congst_thrsh_packets1) ||(clst_cfg->egr_congst_thrsh_packets2) || (clst_cfg->egr_congst_thrsh_packets3) ||(clst_cfg->egr_congst_thrsh_packets4))
    {
        clst->w10.frags = WORD_B3_0(clst_cfg->egr_congst_thrsh_packets1, clst_cfg->egr_congst_thrsh_packets2,
                                   clst_cfg->egr_congst_thrsh_packets3, clst_cfg->egr_congst_thrsh_packets4);
    }
    else
    {   /* In order for the thrshold to be disabled - setting highest values */
        clst->w10.frags = WORD_B3_0(0xFF, 0xFF, 0xFF, 0xFF);
    }


#if ((PPD_DEBUG) & PPD_DEBUG_MSG)
        printMsg ("QOS: Cluster %d config dump\n", clst_indx);
        ppd_dump_qos_clst ((Uint32*)clst, dump_buff, sizeof(dump_buff));
        printMsg("%s\n",dump_buff);
#endif

    return 0;
}

Int32 ti_ppd_qos_cluster_enable (Uint8 clst_indx)
{
    cpdsp_cmd_t  cmd_word    = WORD_S0_B1_0(clst_indx, QOS_CLUSTER_ENABLE, SRPDSP_QOS_CLUSTER);
    if (!g_ppd_init_done || (clst_indx > PP_QOS_CLST_MAX_INDX)) 
    {
        return -1;
    }

    return (cpdsp_cmd_rsp_by_sz(cmd_word, 0, 0, 0, 0));
}

Int32 ti_ppd_qos_cluster_disable (Uint8 clst_indx)
{
    cpdsp_cmd_t  cmd_word    = WORD_S0_B1_0(clst_indx, QOS_CLUSTER_DISABLE, SRPDSP_QOS_CLUSTER);
    if (!g_ppd_init_done || (clst_indx > PP_QOS_CLST_MAX_INDX)) 
    {
        return -1;
    }

    return (cpdsp_cmd_rsp_by_sz(cmd_word, 0, 0, 0, 0));
}


Int32 ti_ppd_get_qos_q_stats(Uint8 qos_qnum, TI_PP_QOS_QUEUE_STATS *stats)
{
    ppd_qos_qcfg_blk_t* qcfg = (ppd_qos_qcfg_blk_t*)(PP_QOS_QCFG_BLK_BASE_VIRT) + qos_qnum;

    if (!g_ppd_init_done || (qos_qnum > PP_QOS_QUEUE_MAX_INDX)) 
    {
        return -1;
    }

    stats->fwd_pkts = qcfg->w6_fwd_pkts_cnt;
    stats->drp_cnt  = qcfg->w7_drp_pkts_cnt;

    return 0;
}

Int32 ti_ppd_get_n_clear_qos_q_stats (Uint8 qos_qnum, TI_PP_QOS_QUEUE_STATS *stats)
{
    ppd_qos_qcfg_blk_t* qcfg = (ppd_qos_qcfg_blk_t*)(PP_QOS_QCFG_BLK_BASE_VIRT) + qos_qnum;

    if (!g_ppd_init_done || (qos_qnum > PP_QOS_QUEUE_MAX_INDX)) 
    {
        return -1;
    }

    stats->fwd_pkts = qcfg->w6_fwd_pkts_cnt;
    stats->drp_cnt  = qcfg->w7_drp_pkts_cnt;

    qcfg->w6_fwd_pkts_cnt = 0;
    qcfg->w7_drp_pkts_cnt = 0;

    printMsg ("Total credit for Q %d = %d\n", qos_qnum, qcfg->w2_total_credit_bytes);

    return 0;
}

Int32 ti_ppd_get_version ( TI_PP_VERSION *ver )
{

    cpdsp_cmd_t cmd_word = WORD_S0_B1_0(0, 0, SRPDSP_SR_VERSION);

    return (cpdsp_cmd_rsp_by_sz(cmd_word, 0, 0, ver, SIZE_IN_WORD(*ver)));
}

Int32 ti_ppd_dispaly_session_info(int session_id)
{
    Uint32 flags;
    Uint32 sessionState;
    ppd_ses_info_blk_t *ses = (ppd_ses_info_blk_t*)(gp_ses_blk + session_id);

    if (session_id >= TI_PP_MAX_ACCLERABLE_SESSIONS)
    {
        printk("Session %d out of rnage\n", session_id);
        return 0;
    }

    flags = ses->w0.s.status;
    if ((flags & 0x1) == 0)
    {
        printk("Session %d not valid\n", session_id);
        return 0;
    }
    printk("\nBase Session Information Table:");
    printk("\n-------------------------------\n");
    printk("IngressVPID         = %d\n", ses->w0.s.ingress_vpid);
    printk("StatusFlags         = 0x%02X ", flags);
    if (flags)
    {
        printk("[ ");
        if (flags & 0x1) {printk("fSessValid ");}
        if (flags & 0x2) {printk("fStatEvent ");}
        if (flags & 0x4) {printk("fTimeout ");}
        printk("]");
    }
    printk("\n");

    flags = ses->w0.s.flags;
    printk("Flags               = 0x%04X ", flags);
    if (flags)
    {
        printk("[ ");
        if (flags & TI_PP_SES_FLAG_IDLE_TMOUT)          {printk("fIdleTime ");}
        if (flags & TI_PP_SES_FLAG_PASS_AFTER_TMOUT)    {printk("fAutoRestart ");}
        if (flags & TI_PP_SES_FLAG_NO_INGRESS_STATS)    {printk("fNoRxStats ");}
        if (flags & TI_PP_SES_FLAG_NO_EGRESS_STATS)     {printk("fNoTxStats ");}
        if (flags & TI_PP_SES_FLAG_XLUDE_ETH_HDR_STATS) {printk("fNoEthBytes ");}
        if (flags & TI_PP_SES_FLAG_UPDATE_TTL)          {printk("fDecIpTtl ");}
        if (flags & TI_PP_SES_FLAG_PROC_TTL_EXP)        {printk("fDoIpTtlExp ");}
        if (flags & TI_PP_SES_FLAG_IP_FRAG)             {printk("fDoIpFrags ");}
        if (flags & TI_PP_SES_FLAG_PROC_IP_OPTS)        {printk("fDoIpOpt ");}
        if (flags & TI_PP_SES_FLAG_USE_FULL_SRC_IPV6)   {printk("fSrcIPv6 ");}
        if (flags & TI_PP_SES_FLAG_USE_FULL_DST_IPV6)   {printk("fDstIPv6 ");}
        if (flags & TI_PP_SES_FLAG_TCP_CONTROL)         {printk("fDoTcpCtrl ");}
        printk("]");
    }
    printk("\n");
    printk("IPv6RecPtr          = 0x%08X\n", ses->w1_ipv6_rec);
    printk("EgressRecPtr        = 0x%08X\n", ses->w3_egress_ptr);
    printk("TimeoutThresh       = %d usec\n", ses->w4.w4_ses_timeout*10);
    printk("RefernceTime        = 0x%08X\n", ses->w5.w5_ses_ref_time);
    printk("PacketCount         = %d\n\n", ses->w6_fwd_pkts_cnt);

    sessionState = *(Uint16*)(PP_SES_STATE_BLK_BASE + (session_id * 2));

    printk("Session State Table:\n");
    printk("--------------------\n");
    printk("SerialNumber        = %d\n", sessionState & 0xF);
    printk("State               = %d [", (sessionState >> 4) & 0x7);
    switch ((sessionState >> 4) & 0x7)
    {
    case TI_PP_SES_STATE_IDLE:          printk("Idle");             break;
    case TI_PP_SES_STATE_NEEDSYNC:      printk("Need Synch");       break;
    case TI_PP_SES_STATE_NEEDSYNC_END:  printk("Need Synch Ended"); break;
    case TI_PP_SES_STATE_QUEUE:         printk("Queueing");         break;
    case TI_PP_SES_STATE_FWD:           printk("Forwarding");       break;
    case TI_PP_SES_STATE_DIVERT:        printk("Diverting");        break;
    default:                            printk("Unknown!!!");       break;
    }
    printk("]\n");
    printk("PostQueuing         = %d\n", (sessionState >> 7) & 0x1);
    printk("AllocatedSynchQ     = %d\n\n", (sessionState >> 8) & 0xFF);

    if (ses->w3_egress_ptr)
    {
        ppd_egress_rec_t *egress = (ppd_egress_rec_t *)ppd_os_get_io_virt((void*)ses->w3_egress_ptr);
        TI_PP_VPID  vpid;

        ti_ppm_get_vpid_info (egress->w1.s.egress_vpid, &vpid);

        printk("Egress Record:\n");
        printk("--------------\n");

        if (vpid.priv_vpid_flags & TI_PP_VPID_FLG_TX_DISBL)
        {
            printk("EgressVPID          = %d %s \n", egress->w1.s.egress_vpid, "[DROP]" );
        }
        else
        {
            printk("NextEgressRecPtr    = 0x%08X\n", egress->w0_next_egr_ptr);
            printk("Priority            = %d\n", egress->w1.s.priority);
            printk("EgressVPID          = %d\n", egress->w1.s.egress_vpid);
            flags = egress->w1.s.frame_code;
            printk("FrameCode           = 0x%02X ", flags);
            if (flags)
            {
                printk("[ ");
                if (flags & TI_PP_EGR_FRM_STRIP_L2)         {printk("fStripL2 ");}
                if (flags & TI_PP_EGR_FRM_TURBODOX_EN)      {printk("fDoTurboDox ");}
                if (flags & TI_PP_EGR_FRM_PPPOE_HDR)        {printk("fPatchPPPoE ");}
                if (flags & TI_PP_EGR_FRM_REFRAME_IP)       {printk("fReframeIP ");}
                if (flags & TI_PP_EGR_FRM_TURBODOX_ADV_EN)  {printk("fDoTurboDoxIgnoreTSOption ");}
                printk("]");
            }
            printk("\n");
            printk("TxDestTag           = 0x%02X\n", egress->w2.s.fwd_dst_tag);
            printk("TxQueueBase         = %d [%s]\n",  egress->w2.s.fwd_q_index, PAL_CPPI41_GET_QNAME(egress->w2.s.fwd_q_index));

            printk("DeviceType          = %d ", egress->w3.s.egress_dev);
            switch (egress->w3.s.egress_dev)
            {
            case TI_PP_PID_TYPE_UNDEFINED:          printk("[PID_TYPE_UNDEFINED]\n");       break;
            case TI_PP_PID_TYPE_ETHERNET:           printk("[PID_TYPE_ETHERNET]\n");        break;
            case TI_PP_PID_TYPE_INFRASTRUCTURE:     printk("[PID_TYPE_INFRASTRUCTURE]\n");  break;
            case TI_PP_PID_TYPE_USBBULK:            printk("[PID_TYPE_USB_RNDIS]\n");       break;
            case TI_PP_PID_TYPE_CDC:                printk("[PID_TYPE_USB_CDC]\n");         break;
            case TI_PP_PID_TYPE_DOCSIS:             printk("[PID_TYPE_DOCSIS]\n");          break;
            case TI_PP_PID_TYPE_ETHERNETSWITCH:     printk("[PID_TYPE_ETHERNETSWITCH]\n");  break;
            default:                                printk("[???]\n");                      break;
            }
            flags = egress->w3.s.flags;
            printk("Flags               = 0x%02X ", flags);
            if (flags)
            {
                printk("[ ");
                if (flags & TI_PP_EGR_FLAG_DSLITE_US)              {printk("flgDSLiteUS ");}
                if (flags & TI_PP_EGR_FLAG_DSLITE_DS)              {printk("flgDSLiteDS ");}
                if (flags & TI_PP_EGR_FLAG_GRE_US)                 {printk("flgGreUS ");}
                if (flags & TI_PP_EGR_FLAG_GRE_DS)                 {printk("flgGreDS ");}
                if (flags & TI_PP_EGR_FLAG_SESS_MAPPED_TO_LOW_PRIO){printk("flgEgrSessMapToLowPrio ");}
                printk("]");
            }
            printk("\n");
            if (egress->w3.s.dsLiteIpv6PayloadLengthOffset)
            {
                printk("L3 'TotalLen' offst = %d\n", egress->w3.s.dsLiteIpv6PayloadLengthOffset);
            }
            printk("L2Size              = %d\n", egress->w1.s.l2_hdr_size);
            printk("NewL2HeaderPtr      = 0x%08X ", egress->w5_l2_hdr_ptr);
            if (egress->w5_l2_hdr_ptr)
            {
                int i, j;
                printk("[0x");
                for (i = 0, j = 0; i < egress->w1.s.l2_hdr_size; i += 4, j++)
                {
                    printk("%08X.", *((Uint32*)(egress->w5_l2_hdr_ptr) + j));
                }
                printk("%c]\n", 8);
            }
            else
            {
                printk("\n");
            }
            printk("ModRecPtr           = 0x%08X\n\n", egress->w6_mod_rec_ptr);

            if (egress->w6_mod_rec_ptr)
            {
                ppd_pkt_mod_rec_t *pkt_mod = (ppd_pkt_mod_rec_t *)ppd_os_get_io_virt((void*)egress->w6_mod_rec_ptr);
                printk("Modification Record:\n");
                printk("--------------------\n");

                flags = pkt_mod->w0.s.flags;
                printk("Flags               = 0x%04X\n", flags);
                if (flags & TI_PP_MOD_IPSRC_VALID)      {printk("New IPSrc           = 0x%08X\n", pkt_mod->w1_ip_src);}
                if (flags & TI_PP_MOD_IPDST_VALID)      {printk("New IPDst           = 0x%08X\n", pkt_mod->w2_ip_dst);}
                if (flags & TI_PP_MOD_L3CHK_VALID)      {printk("L3ChecksumDelta     = 0x%04X\n", (pkt_mod->w4.frags >> 16) & 0xFFFF);}
                if (flags & TI_PP_MOD_SRCPORT_VALID)    {printk("New PortSrc         = 0x%04X\n", (pkt_mod->w3.frags >> 16) & 0xFFFF);}
                if (flags & TI_PP_MOD_DSTPORT_VALID)    {printk("New DstSrc          = 0x%04X\n", pkt_mod->w3.frags & 0xFFFF);}
                if (flags & TI_PP_MOD_L4CHK_VALID)      {printk("L4ChecksumDelta     = 0x%04X\n", pkt_mod->w4.frags & 0xFFFF);}
                if (flags & TI_PP_MOD_IPTOS_VALID)      {printk("New IP TOS          = 0x%02X\n", pkt_mod->w0.s.tos);}
                printk("\n");
            }
        }
    }

    return 0;
}


Int32 ti_ppd_set_qos_cluster_max_global_credit(int byteCredit, int cluster_id, int max_global_credit)
{
    ppd_qos_clst_blk_t *clst = (ppd_qos_clst_blk_t*)(PP_QOS_CLST_BLK_BASE_VIRT) + cluster_id;


    if (cluster_id > PP_QOS_CLST_MAX_INDX)
    {
        printk("cluster_id(%d) out of range\n", cluster_id);
        return -1;
    }
    if (clst->w3.s.qos_q_cnt == 0)
    {
        printk("cluster_id(%d) not configured\n", cluster_id);
        return -1;
    }
    if (byteCredit)
    {
         clst->w2_max_global_credit_bytes = WORD_W0(max_global_credit);
    }
    else
    {
        clst->w1.frags   = WORD_S1_0(clst->w1.s.global_credit_pkts, max_global_credit);
    }
    return 0;
}
Int32 ti_ppd_set_qos_queue_max_credit(int byteCredit, int queue_id, int max_credit)
{
    ppd_qos_qcfg_blk_t *qcfg = (ppd_qos_qcfg_blk_t*)(PP_QOS_QCFG_BLK_BASE_VIRT) + queue_id;

    if (queue_id > PP_QOS_QUEUE_MAX_INDX)
    {
        printk("queue_id(%d) out of range\n", queue_id);
        return -1;
    }

    if (byteCredit)
    {
        qcfg->w3_max_credit_bytes =     WORD_W0(max_credit);
        printk("setting queue %d with %d byteCredit\n",queue_id, max_credit);
    }
    else
    {
        qcfg->w4.frags =     WORD_S1_0( max_credit, qcfg->w4.s.total_credit_pkts);
        printk("setting queue %d with %d pktCredit\n",queue_id, max_credit);
    }

    return 0;
}

Int32 ti_ppd_set_qos_queue_iteration_credit(int byteCredit, int queue_id, int it_credit)
{
    ppd_qos_qcfg_blk_t *qcfg = (ppd_qos_qcfg_blk_t*)(PP_QOS_QCFG_BLK_BASE_VIRT) + queue_id;

    if (queue_id > PP_QOS_QUEUE_MAX_INDX)
    {
        printk("queue_id(%d) out of range\n", queue_id);
        return -1;
    }

    if (byteCredit)
    {
        qcfg->w1.frags =     WORD_S1_0(it_credit, qcfg->w1.s.iteration_credit_pkts);
        printk("setting queue %d with %d Iteration byteCredit\n",queue_id, it_credit);
    }
    else
    {
        qcfg->w1.frags =     WORD_S1_0(qcfg->w1.s.iteration_credit_bytes, it_credit);
        printk("setting queue %d with %d Iteration pktCredit\n",queue_id, it_credit);
    }

    return 0;
}

Int32 ti_ppd_dispaly_qos_queue_info(int queue_id)
{
    Uint32  temp;
    ppd_qos_qcfg_blk_t *qcfg = (ppd_qos_qcfg_blk_t*)(PP_QOS_QCFG_BLK_BASE_VIRT) + queue_id;

    if (queue_id > PP_QOS_QUEUE_MAX_INDX)
    {
        printk("queue_id(%d) out of range\n", queue_id);
        return 0;
    }

    if (qcfg->w3_max_credit_bytes == 0)
    {
        printk("queue_id(%d) not configured\n", queue_id);
        return 0;
    }

    printk("IterationCreditBytes     = %d\n", qcfg->w1.s.iteration_credit_bytes);
    printk("IterationCreditPkts      = %d\n", qcfg->w1.s.iteration_credit_pkts);

    printk("EgressQueue         = %d [%s]\n", qcfg->w0.s.egr_q,PAL_CPPI41_GET_QNAME(qcfg->w0.s.egr_q));

    temp = qcfg->w0.s.flags;
    printk("Flags               = 0x%02X ", temp);
    if (temp)
    {
        printk("[ ");
        if (temp & 0x1)
        {
            printk("fQQRealTime ");
        }
        printk("]");
    }
    printk("\n");
    printk("TotalCreditBytes    = %d\n", qcfg->w2_total_credit_bytes);
    printk("TotalCreditPkts     = %d\n", qcfg->w4.s.total_credit_pkts);
    printk("MaxCreditBytes      = %d\n", qcfg->w3_max_credit_bytes);
    printk("MaxCreditPkts       = %d\n", qcfg->w4.s.max_credit_pkts);
    printk("CongestedBytes      = %d\n", qcfg->w5.s.congst_thrsh_bytes);
    printk("CongestedPkts       = %d\n", qcfg->w5.s.congst_thrsh_pkts);
    printk("PktForward          = %d\n", qcfg->w6_fwd_pkts_cnt);
    printk("PktDrop             = %d\n\n", qcfg->w7_drp_pkts_cnt);

    return 0;
}

Int32 ti_ppd_dispaly_qos_cluster_info(int cluster_id)
{
    Uint32  temp;
    ppd_qos_clst_blk_t *clst = (ppd_qos_clst_blk_t*)(PP_QOS_CLST_BLK_BASE_VIRT) + cluster_id;


    if (cluster_id > PP_QOS_CLST_MAX_INDX)
    {
        printk("cluster_id(%d) out of range\n", cluster_id);
        return 0;
    }
    if (clst->w3.s.qos_q_cnt == 0)
    {
        printk("cluster_id(%d) not configured\n", cluster_id);
        return 0;
    }

    printk("GlobalCreditBytes        = %d\n", clst->w0_global_credit_bytes);
    printk("GlobalCreditPkts         = %d\n", clst->w1.s.global_credit_pkts);

    printk("MaxGlobalBytes           = %d\n", clst->w2_max_global_credit_bytes);
    printk("MaxGlobalPkts            = %d\n", clst->w1.s.max_global_credit_pkts);

    temp = clst->w3.s.qos_q_cnt;

    printk("QQCount             = %d\n", temp);
    if (temp) {printk("                      %d+%d=%d[%s]\n", clst->w3.s.qos_q0, PAL_CPPI41_SR_QPDSP_QOS_Q_BASE, clst->w3.s.qos_q0 + PAL_CPPI41_SR_QPDSP_QOS_Q_BASE, PAL_CPPI41_GET_QNAME(clst->w3.s.qos_q0 + PAL_CPPI41_SR_QPDSP_QOS_Q_BASE)); temp--;}
    if (temp) {printk("                      %d+%d=%d[%s]\n", clst->w3.s.qos_q1, PAL_CPPI41_SR_QPDSP_QOS_Q_BASE, clst->w3.s.qos_q1 + PAL_CPPI41_SR_QPDSP_QOS_Q_BASE, PAL_CPPI41_GET_QNAME(clst->w3.s.qos_q1 + PAL_CPPI41_SR_QPDSP_QOS_Q_BASE)); temp--;}
    if (temp) {printk("                      %d+%d=%d[%s]\n", clst->w3.s.qos_q2, PAL_CPPI41_SR_QPDSP_QOS_Q_BASE, clst->w3.s.qos_q2 + PAL_CPPI41_SR_QPDSP_QOS_Q_BASE, PAL_CPPI41_GET_QNAME(clst->w3.s.qos_q2 + PAL_CPPI41_SR_QPDSP_QOS_Q_BASE)); temp--;}
    if (temp) {printk("                      %d+%d=%d[%s]\n", clst->w4.s.qos_q3, PAL_CPPI41_SR_QPDSP_QOS_Q_BASE, clst->w4.s.qos_q3 + PAL_CPPI41_SR_QPDSP_QOS_Q_BASE, PAL_CPPI41_GET_QNAME(clst->w4.s.qos_q3 + PAL_CPPI41_SR_QPDSP_QOS_Q_BASE)); temp--;}
    if (temp) {printk("                      %d+%d=%d[%s]\n", clst->w4.s.qos_q4, PAL_CPPI41_SR_QPDSP_QOS_Q_BASE, clst->w4.s.qos_q4 + PAL_CPPI41_SR_QPDSP_QOS_Q_BASE, PAL_CPPI41_GET_QNAME(clst->w4.s.qos_q4 + PAL_CPPI41_SR_QPDSP_QOS_Q_BASE)); temp--;}
    if (temp) {printk("                      %d+%d=%d[%s]\n", clst->w4.s.qos_q5, PAL_CPPI41_SR_QPDSP_QOS_Q_BASE, clst->w4.s.qos_q5 + PAL_CPPI41_SR_QPDSP_QOS_Q_BASE, PAL_CPPI41_GET_QNAME(clst->w4.s.qos_q5 + PAL_CPPI41_SR_QPDSP_QOS_Q_BASE)); temp--;}
    if (temp) {printk("                      %d+%d=%d[%s]\n", clst->w4.s.qos_q6, PAL_CPPI41_SR_QPDSP_QOS_Q_BASE, clst->w4.s.qos_q6 + PAL_CPPI41_SR_QPDSP_QOS_Q_BASE, PAL_CPPI41_GET_QNAME(clst->w4.s.qos_q6 + PAL_CPPI41_SR_QPDSP_QOS_Q_BASE)); temp--;}
    if (temp) {printk("                      %d+%d=%d[%s]\n", clst->w5.s.qos_q7, PAL_CPPI41_SR_QPDSP_QOS_Q_BASE, clst->w5.s.qos_q7 + PAL_CPPI41_SR_QPDSP_QOS_Q_BASE, PAL_CPPI41_GET_QNAME(clst->w5.s.qos_q7 + PAL_CPPI41_SR_QPDSP_QOS_Q_BASE)); temp--;}
    if (temp) {printk("                      %d+%d=%d[%s]\n", clst->w5.s.qos_q8, PAL_CPPI41_SR_QPDSP_QOS_Q_BASE, clst->w5.s.qos_q8 + PAL_CPPI41_SR_QPDSP_QOS_Q_BASE, PAL_CPPI41_GET_QNAME(clst->w5.s.qos_q8 + PAL_CPPI41_SR_QPDSP_QOS_Q_BASE)); temp--;}

    temp = clst->w5.s.egr_q_cnt;

    printk("EQCount             = %d\n", temp);
    if (temp) {printk("                      %d[%s]\n", clst->w5.s.egr_q0,PAL_CPPI41_GET_QNAME(clst->w5.s.egr_q0)); temp--;}
    if (temp) {printk("                      %d[%s]\n", clst->w6.s.egr_q1,PAL_CPPI41_GET_QNAME(clst->w6.s.egr_q1)); temp--;}
    if (temp) {printk("                      %d[%s]\n", clst->w6.s.egr_q2,PAL_CPPI41_GET_QNAME(clst->w6.s.egr_q2)); temp--;}
    if (temp) {printk("                      %d[%s]\n", clst->w6.s.egr_q3,PAL_CPPI41_GET_QNAME(clst->w6.s.egr_q3)); temp--;}
    if (temp) {printk("                      %d[%s]\n", clst->w6.s.egr_q4,PAL_CPPI41_GET_QNAME(clst->w6.s.egr_q4)); temp--;}
    if (temp) {printk("                      %d[%s]\n", clst->w7.s.egr_q5,PAL_CPPI41_GET_QNAME(clst->w7.s.egr_q5)); temp--;}
    if (temp) {printk("                      %d[%s]\n", clst->w7.s.egr_q6,PAL_CPPI41_GET_QNAME(clst->w7.s.egr_q6)); temp--;}
    if (temp) {printk("                      %d[%s]\n", clst->w7.s.egr_q7,PAL_CPPI41_GET_QNAME(clst->w7.s.egr_q7)); temp--;}
    if (temp) {printk("                      %d[%s]\n", clst->w7.s.egr_q8,PAL_CPPI41_GET_QNAME(clst->w7.s.egr_q8)); temp--;}

    printk("ECThresh1           = %d * 64 = %d\n", clst->w8.s.egr_congst_thrsh_bytes1, (clst->w8.s.egr_congst_thrsh_bytes1 * 64));
    printk("ECThresh2           = %d * 64 = %d\n", clst->w8.s.egr_congst_thrsh_bytes2, (clst->w8.s.egr_congst_thrsh_bytes2 * 64));
    printk("ECThresh3           = %d * 64 = %d\n", clst->w9.s.egr_congst_thrsh_bytes3, (clst->w9.s.egr_congst_thrsh_bytes3 * 64));
    printk("ECThresh4           = %d * 64 = %d\n", clst->w9.s.egr_congst_thrsh_bytes4, (clst->w9.s.egr_congst_thrsh_bytes4 * 64));
    printk("ECThreshPkts1       = %d * 16 = %d\n", clst->w10.s.egr_congst_thrsh_pkts1, (clst->w10.s.egr_congst_thrsh_pkts1 * 16));
    printk("ECThreshPkts2       = %d * 16 = %d\n", clst->w10.s.egr_congst_thrsh_pkts2, (clst->w10.s.egr_congst_thrsh_pkts2 * 16));
    printk("ECThreshPkts3       = %d * 16 = %d\n", clst->w10.s.egr_congst_thrsh_pkts3, (clst->w10.s.egr_congst_thrsh_pkts3 * 16));
    printk("ECThreshPkts4       = %d * 16 = %d\n", clst->w10.s.egr_congst_thrsh_pkts4, (clst->w10.s.egr_congst_thrsh_pkts4 * 16));

    return 0;
}








