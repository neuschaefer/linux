/*****************************************************************************
 * Copyright 2006 - 2009 Broadcom Corporation.  All rights reserved.
 *
 * Unless you and Broadcom execute a separate written software license
 * agreement governing use of this software, this software is licensed to you
 * under the terms of the GNU General Public License version 2, available at
 * http://www.broadcom.com/licenses/GPLv2.php (the "GPL").
 *
 * Notwithstanding the above, under no circumstances may you combine this
 * software in any way with any other Broadcom software provided under a
 * license other than the GPL, without Broadcom's express prior written
 * consent.
 *****************************************************************************/

#include <linux/types.h>
#include <linux/errno.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/clk.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/version.h>
#include <linux/hrtimer.h>
#include <linux/atomic.h>
#include <asm/uaccess.h>
#include <linux/sysctl.h>

#include <linux/broadcom/bcm_cir.h>
#include <chal/chal_cir.h>


/* ---- Private Constants and Types -------------------------------------- */

/* #define CIR_DMA_MEMORY_ALLOCATE_DYNAMIC 1 */
#define BCM_CIR_DEV_MINOR       0
#define CIR_GET_CARRIER_MODE(m) ((m==0)?CHAL_CIR_TX_MODE_CARRIERLESS:CHAL_CIR_TX_MODE_CARRIER)
#define PACKET_BUFFER_SIZE              (1+32+1) 
#define ROKU_REMOTE_PACKET_COUNT        (1+32+1) /* start word + 32 data words + stop word */
#define DIRECTV_REMOTE_PACKET_COUNT     10 
#define CIR_WAVE_QUEUE_COUNT    16       /* Num packets driver queues */
#define DEFAULT_RELEASE_TIMEOUT_MS      60  /* interference reduction should use 120 instead */
#define DEFAULT_FAILSAFE_RELEASE_TIMEOUT_MS  130  /* This must be >= 120 */

#define NO_REMOTE      0
#define ROKU_REMOTE    1
#define DIRECTV_REMOTE 2

#define PRINT_DEBUGS1 1
#define PRINT_DEBUGS2 2
#define PRINT_DEBUGS3 4
#define PRINT_DEBUGS4 8
#define PRINT_DEBUGS5 16

#define NEC_LEAD_MARK_TIME     11000
#define NEC_LEAD_SPACE_TIME     5900
#define NEC_REPEAT_SPACE_TIME   2250
#define NEC_BIT_MARK_TIME        720
#define NEC_BIT_1_SPACE_TIME    2000
#define NEC_BIT_0_SPACE_TIME     750
#define NEC_SUFFIX_SPACE_MIN    8000
#define NEC_RESYNC_TIME       100000

#define LOW_THRESH(v) (((v) * 70)/100)
#define HIGH_THRESH(v) (((v) * 135)/100)

#define LOW_THRESH_WIDE(v) (((v) * 50)/100)
#define HIGH_THRESH_WIDE(v) (((v) * 150)/100)

#define THRESHOLD(val_, limit_) ((val_ >= LOW_THRESH(limit_)) && (val_ <= HIGH_THRESH(limit_)))
#define THRESHOLD_WIDE(val_, limit_) ((val_ >= LOW_THRESH_WIDE(limit_)) && (val_ <= HIGH_THRESH_WIDE(limit_)))

//#define DPRINT printk
#define DPRINT(x...)


static int debugCIR = 0;
static unsigned short hi_top    = 0xffff;
static unsigned short hi_bottom = 0x450;
static unsigned short lo_top    = 0x700;
static unsigned short lo_bottom = 0x500;
static unsigned short dtv_end   = 0x800;


/*
 * Debug
 */
//#define __CIR_DRV_EXT_DBG__             1
#ifdef __CIR_DRV_EXT_DBG__
#define CIR_DBG1(x...) {if(debugCIR & PRINT_DEBUGS1){{printk(KERN_INFO "cir_dbg:%s(): ", __FUNCTION__);printk(x);}}}
#define CIR_DBG2(x...) {if(debugCIR & PRINT_DEBUGS2){{printk(KERN_INFO "cir_dbg:%s(): ", __FUNCTION__);printk(x);}}}
#define CIR_DBG3(x...) {if(debugCIR & PRINT_DEBUGS3){{printk(KERN_INFO "cir_dbg:%s(): ", __FUNCTION__);printk(x);}}}
#define CIR_DBG4(x...) {if(debugCIR & PRINT_DEBUGS4){{printk(KERN_INFO "cir_dbg:%s(): ", __FUNCTION__);printk(x);}}}
#define CIR_DBG5(x...) {if(debugCIR & PRINT_DEBUGS5){{printk(KERN_INFO "cir_dbg:%s(): ", __FUNCTION__);printk(x);}}}
#else
#define CIR_DBG1(x...)
#define CIR_DBG2(x...)
#define CIR_DBG3(x...)
#define CIR_DBG4(x...)
#define CIR_DBG5(x...)
#endif
#define CIR_ERR(x...) {printk(KERN_ERR "cir_err: ");printk(x);}

/* cir configuration */
struct bcm_dev
{
    /* char device configuration */
    struct class        *dev_class;
    struct device       *dev;
    int                 dev_major;
    int                 dev_minor;
    atomic_t            dev_is_open;
};

struct ir_packet {
    uint32_t           type;
    uint32_t           count;
    uint64_t           interval;
    uint32_t           data[PACKET_BUFFER_SIZE];
};

struct bcm_cir
{
    /* CIR hardware Configurations */
    CHAL_CIR_HANDLE_t    hw_handle;
    CHAL_CIR_CONFIG_t    hw_cfg;
    unsigned long        peri_clk_freq;
    struct bcm_cir_mode  mode;

    int                 reading_pkt;
    unsigned            pkt_read_ptr;    /* */
    unsigned            pkt_write_ptr;   /* */
    uint64_t            last_sample_ktime_; /* When we received end of last packet */

    struct timer_list   release_timer;
    struct timer_list   failsafe_release_timer;
    wait_queue_head_t   rx_waitq;
    struct ir_packet    pkt_queue[CIR_WAVE_QUEUE_COUNT];
};

static struct ctl_table_header * cir_sysctl_header;
static int parsingDirectv = 0;
static int directvOff     = 0;
static int directvOn      = 1;

static int noiseFilter    = 500;
static int oldNoiseFilter = 500;
static int minNoise       = 0;
static int maxNoise       = 65535;

static int minPulse    = NEC_MIN_PULSE_WIDTH;
static int oldMinPulse = NEC_MIN_PULSE_WIDTH;
static int minPulseMin = 0;
static int minPulseMax = 65535;

static int maxPulse    = NEC_MAX_PULSE_WIDTH;
static int oldMaxPulse = NEC_MAX_PULSE_WIDTH;
static int maxPulseMin = 0;
static int maxPulseMax = 65535;

static int releaseTimerTimeout       = DEFAULT_RELEASE_TIMEOUT_MS;
static int oldReleaseTimerTimeout    = DEFAULT_RELEASE_TIMEOUT_MS;
static int releaseTimerTimeoutMin    = 40;
static int releaseTimerTimeoutMax    = 2000;

static int enableInterferenceReduction      = 0;
static int enableInterferenceReductionMin   = 0;
static int enableInterferenceReductionMax   = 1;

static uint32_t irGlitchCount = 0;

static int enableGlitchLogging      = 0;
static int enableGlitchLoggingMin   = 0;
static int enableGlitchLoggingMax   = 1;



#define USE_INTERFERENCE_REDUCTION() (enableInterferenceReduction == 1)


/* ---- Private Function Prototypes -------------------------------------- */
static int  bcm_cir_open(struct inode *inode, struct file *file);
static int  bcm_cir_release(struct inode *inode, struct file *file);
static int  bcm_cir_read(struct file *file, char __user *rdBuff, size_t rdLen, loff_t *rdOff);
static int  bcm_cir_write(struct file *file, const char __user *wrBuff, size_t wrLen, loff_t *wrOff);
static long bcm_cir_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
static void bcm_cir_set_protocol(struct bcm_cir *cir, uint32_t sample);
static int  bcm_cir_advance_pkt_write_ptr(struct bcm_cir *cir);
static int  bcm_cir_conf_hwblock(enum bcm_cir_device device_mode, struct bcm_cir *cir, unsigned long clkFreq, int is_tx);
static int  bcm_cir_check_protocol_packet_count(struct bcm_cir *cir);
static struct bcm_cir *bcm_cir_create(void);
static uint32_t bcm_cir_hack_code(uint32_t code, uint64_t interval);
static uint32_t bcm_cir_decode(uint32_t rx_data[], uint32_t type, int count);
static int bcm_cir_read_packet(struct bcm_cir *cir, uint32_t rx_data[], uint64_t *interval, uint32_t *type, uint32_t *count);
static int bcm_cir_conf_hwblock(enum bcm_cir_device  cfg_mode, /**< IR Protocol */
                                struct bcm_cir   *cir,    /**< Conf structure */
                                unsigned long         clkFreq,/**< Peripheral clock */
                                int                   is_tx   /**< 0-rx, 1-tx. Used for custom config */
                               );
static int  bcm_cir_conf_ir_protocol(struct bcm_cir *cir, enum bcm_cir_device cfg_mode);
static void bcm_cir_reset_block(struct bcm_cir *cir, int is_tx);
static int  bcm_cir_init_hw (struct bcm_cir *cir);
static irqreturn_t bcm_cir_intr_handler(int irq, void *data);
static void bcm_cir_release_timeout(unsigned long arg);
static void bcm_cir_store_sample(struct bcm_cir *cir, uint32_t sample);
static void bcm_cir_advance_pkt_read_ptr(struct bcm_cir *cir);
static void bcm_cir_reset_current_packet_state(struct bcm_cir *cir);
static void bcm_cir_block_shutdown(struct bcm_cir *cir, int is_tx);
static int bcm_cir_disable_block(struct bcm_cir *cir, int is_tx);
static int bcm_cir_enable_block(struct bcm_cir *cir, size_t data_count, int is_tx);
static int bcm_cir_init_clocks(struct bcm_cir *cir);
static uint64_t bcm_cir_now(void);
static inline bool is_packet_queue_empty(struct bcm_cir *cir);

static inline int get_release_timer_timeout(void);
static inline int get_failsafe_release_timer_timeout(void);

/* ---- Private Variables ------------------------------------------------- */

/* File Operations (device driver entry points) */
static const struct file_operations cir_fops = {
    .owner          = THIS_MODULE,
    .open           = bcm_cir_open,
    .release        = bcm_cir_release,
    .read           = bcm_cir_read,
    .write          = bcm_cir_write,
    .unlocked_ioctl = bcm_cir_ioctl
};

/*only values between directvOff and directvOn can be written to directv. */
static ctl_table cir_table[] = {
    {
        .procname     = "directv",
        .data	      = &parsingDirectv,
        .maxlen	      = sizeof(int),
        .mode	      = 0666,
        .proc_handler = &proc_dointvec_minmax,
        .extra1       = &directvOff,
        .extra2       = &directvOn
    },
    {
        .procname     = "noiseFilter",
        .data	      = &noiseFilter,
        .maxlen	      = sizeof(int),
        .mode	      = 0666,
        .proc_handler = &proc_dointvec_minmax,
        .extra1       = &minNoise,
        .extra2       = &maxNoise
    },
    {
        .procname     = "minPulse",
        .data	      = &minPulse,
        .maxlen	      = sizeof(int),
        .mode	      = 0666,
        .proc_handler = &proc_dointvec_minmax,
        .extra1       = &minPulseMin,
        .extra2       = &minPulseMax
    },
    {
        .procname     = "maxPulse",
        .data	      = &maxPulse,
        .maxlen	      = sizeof(int),
        .mode	      = 0666,
        .proc_handler = &proc_dointvec_minmax,
        .extra1       = &maxPulseMin,
        .extra2       = &maxPulseMax
    },
    {
        .procname     = "releaseTimerTimeout",
        .data	      = &releaseTimerTimeout,
        .maxlen	      = sizeof(int),
        .mode	      = 0666,
        .proc_handler = &proc_dointvec_minmax,
        .extra1       = &releaseTimerTimeoutMin,
        .extra2       = &releaseTimerTimeoutMax
    },
    {
        .procname     = "enableInterferenceReduction",
        .data	      = &enableInterferenceReduction,
        .maxlen	      = sizeof(int),
        .mode	      = 0666,
        .proc_handler = &proc_dointvec_minmax,
        .extra1       = &enableInterferenceReductionMin,
        .extra2       = &enableInterferenceReductionMax
    },
    {
        .procname     = "irGlitchCount",
        .data	      = &irGlitchCount,
        .maxlen	      = sizeof(uint32_t),
        .mode	      = 0666,
        .proc_handler = &proc_dointvec
    },
    {
        .procname     = "enableGlitchLogging",
        .data	      = &enableGlitchLogging,
        .maxlen	      = sizeof(int),
        .mode	      = 0666,
        .proc_handler = &proc_dointvec_minmax,
        .extra1       = &enableGlitchLoggingMin,
        .extra2       = &enableGlitchLoggingMax
    },
    {}
};

static ctl_table cir_root_table[] = {
    {
        .procname = "cir",
        .mode     = 0555,
        .child    = cir_table
    },
    {}
};

static struct bcm_dev  bcm_dev;
static struct bcm_cir *the_bcm_cir = NULL;

/* ---- Functions -------------------------------------------------------- */

static inline void ir_glitch(char const *str)
{
    irGlitchCount++;
    if (enableGlitchLogging)
        printk("IR Glitch: %s\n", str);
}

static inline int get_release_timer_timeout(void)
{
    if (oldReleaseTimerTimeout != releaseTimerTimeout)
    {
        oldReleaseTimerTimeout = releaseTimerTimeout;
        printk(KERN_ERR "cir: changing release timer duration to %dms\n", releaseTimerTimeout);
    }

    return oldReleaseTimerTimeout;
}

static inline int get_failsafe_release_timer_timeout(void)
{
    int timeout = get_release_timer_timeout();

    // Make sure that the failsafe timeout is longer than the normal one
    if (timeout < DEFAULT_FAILSAFE_RELEASE_TIMEOUT_MS)
        timeout = DEFAULT_FAILSAFE_RELEASE_TIMEOUT_MS;
    else
        timeout += 20;

    return timeout;
}

/***************************************************************************/
/**
 */
static inline bool is_packet_queue_empty(struct bcm_cir *cir)
{
#if 0
    return (cir->pkt_read_ptr == cir->pkt_write_ptr);
#else
    bool ret = (cir->pkt_read_ptr == cir->pkt_write_ptr);
    // If the queue is not empty...
    if (!ret) {
        if (cir->pkt_queue[cir->pkt_read_ptr].type == ROKU_REMOTE) {
            // Only allow full ROKU packets
            if (cir->pkt_queue[cir->pkt_read_ptr].count != ROKU_REMOTE_PACKET_COUNT) {
                DPRINT("\n Read Flush (bad frame count: %u) ", cir->pkt_queue[cir->pkt_read_ptr].count);
                // Skip this packet, since it is malformed
                unsigned next_read_ptr = cir->pkt_read_ptr + 1;
                if (next_read_ptr >= CIR_WAVE_QUEUE_COUNT)
                    next_read_ptr = 0;
                cir->pkt_read_ptr = next_read_ptr;

                // This is a partial ROKU packet, so indicate there's nothing here
                ret = true;
            }
        }
    }
    return ret;
#endif
}

/***************************************************************************/
/**
 */
static uint64_t bcm_cir_now(void)
{
    ktime_t ktime = ktime_get();
    return ktime_to_ns(ktime);
}


/***************************************************************************/
/**
 * Initialize the cir clocks
 */
static int bcm_cir_init_clocks(struct bcm_cir *cir)
{
    unsigned long clock_freq = KPM_APB1_CLK_FREQ;
    struct clk * peri_clk = (struct clk *)NULL;

    /* Enable CIR (CCU) clock */
    peri_clk = clk_get(bcm_dev.dev, "cir_clk");
    if (IS_ERR(peri_clk)) {
        CIR_ERR("getting cir_clk failed\n");
        return (PTR_ERR(peri_clk));
    }

    if(0 != clk_enable(peri_clk)) {
        CIR_ERR("cir_clk enable failed\n");
        return (-EBUSY);
    }

    /* Find the APB1 frequency. This is the CIR frequency */
    peri_clk = clk_get(bcm_dev.dev, "apb1");
    if (IS_ERR(peri_clk)) {
        CIR_ERR("getting apb1 failed, setting clk to default\n");
        peri_clk = NULL;
    } else {
        clock_freq = clk_get_rate(peri_clk);
    }

    cir->peri_clk_freq = clock_freq;

    return 0;
}

/***************************************************************************/
/**
 * Enable CIR Block
 *   @return
 *      0      Success
 *      <= 0    Error
 */
static int bcm_cir_enable_block(struct bcm_cir *cir, size_t data_count, int is_tx)
{
    if (is_tx == BCM_CIR_TX_BLOCK) {
        if(CHAL_CIR_ERR_NONE != chal_cir_tx_block_enable(&cir->hw_handle, data_count)) {
            return (-EACCES);
        }
    } else {
        if(CHAL_CIR_ERR_NONE != chal_cir_rx_block_enable(&cir->hw_handle)) {
            return (-EACCES);
        }
        chal_cir_tx_dma_disable(&cir->hw_handle);
    }
    return 0;
}

/***************************************************************************/
/**
 * Disable CIR Block
 *   @return
 *      0      Success
 *      < 0    Error
 */
static int bcm_cir_disable_block(struct bcm_cir *cir, int is_tx)
{
    if(is_tx == BCM_CIR_TX_BLOCK) {
        /* Disable Transmitter */
        if(CHAL_CIR_ERR_NONE != chal_cir_tx_disable(&cir->hw_handle)) {
            return (-EACCES);
        }
        /* Reset Transmitter */
        chal_cir_tx_reset(&cir->hw_handle);

    } else {
        /* Disable Receiver */
        if(CHAL_CIR_ERR_NONE != chal_cir_rx_disable(&cir->hw_handle)) {
            return (-EACCES);
        }
        /* Reset Receiver */
        chal_cir_rx_reset(&cir->hw_handle);
    }
    return 0;
}

/***************************************************************************/
/**
 * Shutdown block
 */
static void bcm_cir_block_shutdown(struct bcm_cir *cir, int is_tx)
{
    bcm_cir_disable_block(cir, is_tx);
}

/***************************************************************************/
/**
 */
static int bcm_cir_check_protocol_packet_count(struct bcm_cir *cir){
    switch(cir->pkt_queue[cir->pkt_write_ptr].type){
    case ROKU_REMOTE:
        if (cir->pkt_queue[cir->pkt_write_ptr].count != ROKU_REMOTE_PACKET_COUNT) {
            CIR_DBG1("received incomplete ROKU packet, bailing\n");
            CIR_DBG1("packetCount: %i\n", (int)cir->pkt_queue[cir->pkt_write_ptr].count);
            return 0;
        }
        break;
    case DIRECTV_REMOTE:
        if (cir->pkt_queue[cir->pkt_write_ptr].count != DIRECTV_REMOTE_PACKET_COUNT) {
            CIR_DBG1("received incomplete DIRECTV packet, bailing\n");
            CIR_DBG1("packetCount: %i\n", (int)cir->pkt_queue[cir->pkt_write_ptr].count);
            cir->pkt_queue[cir->pkt_write_ptr].count    = 0;
            cir->pkt_queue[cir->pkt_write_ptr].type     = NO_REMOTE;
            cir->pkt_queue[cir->pkt_write_ptr].interval = 0;
            cir->reading_pkt                            = 0;
            return 0;
        }
        break;
    default:
        return 0;
    }
    return 1;
}

/***************************************************************************/
/**
 */
static void bcm_cir_reset_current_packet_state(struct bcm_cir *cir){
    int err = 0;
    cir->pkt_queue[cir->pkt_write_ptr].count    = 0;
    cir->pkt_queue[cir->pkt_write_ptr].type     = NO_REMOTE;
    cir->pkt_queue[cir->pkt_write_ptr].interval = 0;
    cir->reading_pkt                            = 0;

    DPRINT("\nReset_packet_state  ");

    if((noiseFilter != oldNoiseFilter) || 
       ( minPulse != oldMinPulse ) || 
       ( maxPulse != oldMaxPulse )) {
      //bcm_cir_conf_ir_protocol(cir, BCM_IR_DEV_NEC);

      chal_cir_rx_disable(&cir->hw_handle);
      /* Receiver CFG */
      cir->hw_cfg.rxConfig.minPulseWidth  = minPulse;
      cir->hw_cfg.rxConfig.maxPulseWidth  = maxPulse;
      cir->hw_cfg.rxConfig.pwNoise        = noiseFilter;
      err = (int)chal_cir_init(&cir->hw_handle, &cir->hw_cfg);
      if(err != 0) {
          err = -ENODEV;
          /* bcm_cir_reset_block(cir, BCM_CIR_TX_BLOCK); */
          /* bcm_cir_reset_block(cir, BCM_CIR_RX_BLOCK); */
          CIR_ERR("Failed to reconfigure hardware\n");
      }

      /* Might have to do this...  */
      chal_cir_rx_enable(&cir->hw_handle); 

    }
    else
    {
        // Reset the cir rx block so that we can start to
        // resync with the next valid IR packet.
        chal_cir_rx_disable(&cir->hw_handle);
        chal_cir_rx_enable(&cir->hw_handle); 
    }

}

/***************************************************************************/
/**
 */
static void bcm_cir_notify_reader(struct bcm_cir *cir){
    wake_up_locked(&cir->rx_waitq);
}

/***************************************************************************/
/**
 * Increment the write pointer in the pkt_queue queue.
 */
static int bcm_cir_advance_pkt_write_ptr(struct bcm_cir *cir)
{

    unsigned next_write_ptr = cir->pkt_write_ptr + 1;
    if (next_write_ptr >= CIR_WAVE_QUEUE_COUNT){
        next_write_ptr = 0;
    }

    if (next_write_ptr == cir->pkt_read_ptr) {
        CIR_DBG1("pkt queue overflow, dropping data\n");
        CIR_DBG1("packetCount: %i\n", (int)cir->pkt_queue[cir->pkt_write_ptr].count);
        return 0;
    }

    cir->pkt_write_ptr = next_write_ptr;

    bcm_cir_reset_current_packet_state(cir);
    bcm_cir_notify_reader(cir);
    return 1;
}

/***************************************************************************/
/**
 * Increment the read pointer in the pkt_queue queue.
 */
static void bcm_cir_advance_pkt_read_ptr(struct bcm_cir *cir)
{
    unsigned next_read_ptr = cir->pkt_read_ptr + 1;
    if (next_read_ptr >= CIR_WAVE_QUEUE_COUNT)
        next_read_ptr = 0;
    BUG_ON(is_packet_queue_empty(cir)); /* can't read an empty queue */
    cir->pkt_read_ptr = next_read_ptr;
}

/***************************************************************************/
/**
 */
static void bcm_cir_store_sample(struct bcm_cir *cir, uint32_t sample)
{
    uint64_t now_time = bcm_cir_now();
    bool valid = true;
    unsigned hi = sample >> 16;
    unsigned lo = sample & 0xFFFF;
    char const *glitch = NULL;

    DPRINT("  %u/%u <%u>  ", hi, lo, now_time - cir->last_sample_ktime_);

    /* 
       First element of the packet:
       - set the interval
       - determine the type of remote
    */
    if (cir->pkt_queue[cir->pkt_write_ptr].count == 0) {
        cir->pkt_queue[cir->pkt_write_ptr].interval = now_time - cir->last_sample_ktime_;
        bcm_cir_set_protocol(cir, sample);
    }

    switch(cir->pkt_queue[cir->pkt_write_ptr].type){
    case ROKU_REMOTE:
        if (cir->pkt_queue[cir->pkt_write_ptr].count >= ROKU_REMOTE_PACKET_COUNT) {
            CIR_DBG2("received too many bits for ROKU packet, bailing\n");
            CIR_DBG2("packetCount: %i\n", (int)cir->pkt_queue[cir->pkt_write_ptr].count);
            // Mark this packet as unknown
            cir->pkt_queue[cir->pkt_write_ptr].type = NO_REMOTE;
            // ("\n Write Flush (excess frame count) ");
            return;
        }
        if (cir->pkt_queue[cir->pkt_write_ptr].count == 0)
        {
            // Mark the preamble as valid
            valid = true;
        }
        else if (cir->pkt_queue[cir->pkt_write_ptr].count == ROKU_REMOTE_PACKET_COUNT - 1)
        {
            // Validate the suffix
            if (THRESHOLD(hi, NEC_BIT_MARK_TIME) && lo >= NEC_SUFFIX_SPACE_MIN /* THRESHOLD(lo, NEC_SUFFIX_SPACE_TIME) */)
            {
                // Mark the preamble as valid
                valid = true;
            }
            else
            {
                // Bad suffix timing
                DPRINT("\nFlush (bad suffix at %u: %u/%u ", cir->pkt_queue[cir->pkt_write_ptr].count, hi, lo);
                valid = false;
                glitch = "S";
            }
        }
        else
        {
            // Check bit timings
            if (THRESHOLD(hi, NEC_BIT_MARK_TIME) && (THRESHOLD(lo, NEC_BIT_0_SPACE_TIME) || THRESHOLD(lo, NEC_BIT_1_SPACE_TIME)))
            {
                // Good, keep the sample
                valid = true;

                DPRINT(" [%u] ", (lo > 2*hi) ? 1 : 0);
            }
            else
            {
                // Bad, so mark it as such
                DPRINT("\nFlush (bad bit at %u: %u/%u) ", cir->pkt_queue[cir->pkt_write_ptr].count, hi, lo);
                valid = false;
                glitch = "B";
            }
        }

        break;
    case DIRECTV_REMOTE:
        if (cir->pkt_queue[cir->pkt_write_ptr].count >= DIRECTV_REMOTE_PACKET_COUNT) {
            CIR_DBG2("received too many bits for DIRECTV packet, bailing\n");
            CIR_DBG2("packetCount: %i\n", (int)cir->pkt_queue[cir->pkt_write_ptr].count);
            return;
        }
        break;
    default:
        // Don't allow unknown protocols
        valid = false;
        // Don't set glitch here, since bcm_cir_set_protocol() will have logged the bad protocol already
        break;
    }

    if (valid)
    {
        cir->last_sample_ktime_ = now_time;
        cir->pkt_queue[cir->pkt_write_ptr].data[cir->pkt_queue[cir->pkt_write_ptr].count++] = sample;
    }
    else
    {
        // Flush everything and start over
        bcm_cir_reset_current_packet_state(cir);
        if (glitch)
            ir_glitch(glitch);
    }
}

/***************************************************************************/
/**
 * Too much time has passed after the end of the previous pkt.
 * Assume the button has been released; send a release event.
 */
static void bcm_cir_release_timeout(unsigned long arg)
{
    struct bcm_cir *cir = (struct bcm_cir *) arg;
    unsigned long irq_flags;

    spin_lock_irqsave(&cir->rx_waitq.lock, irq_flags);
    CIR_DBG4("release timeout\n");

    /* Reset packet state, advance the packet ptr, and notify the reader
       that data is available. The decode will return a zero because the 
       count is zero. This indicates a release event for the Roku Protocol.
    */
    DPRINT("\nRelease timeout ");
    bcm_cir_reset_current_packet_state(cir);
    bcm_cir_advance_pkt_write_ptr(cir);

    // Both the normal and failsafe release timers call here when they fire.
    // In case the regular one fires first, we don't need the failsafe to fire
    // later on (for this same situation), so we'll cancel it here.
    del_timer(&cir->failsafe_release_timer);

    spin_unlock_irqrestore(&cir->rx_waitq.lock, irq_flags);
}

/***************************************************************************/
/**
 * CIR Interrupt Handler
 * Write complete is handled here
 */
static irqreturn_t bcm_cir_intr_handler(int irq, void *data)
{
    struct bcm_cir *cir = (struct bcm_cir *)data;
    uint32_t intr_status;

    if (chal_cir_interrupt_pending_read(&cir->hw_handle, &intr_status) != CHAL_CIR_ERR_NONE){
        return IRQ_NONE;
    }
    chal_cir_interrupt_pending_clear(&cir->hw_handle, intr_status);

    if (intr_status & (CHAL_CIR_RX_DONE_INTR | CHAL_CIR_RX_FIFO_INTR)) {
        unsigned long irq_flags;

        spin_lock_irqsave(&cir->rx_waitq.lock, irq_flags);

        if( CHAL_CIR_RX_FIFO_INTR & intr_status){
            CIR_DBG4("bcm_cir_intr_handler:  CHAL_CIR_RX_FIFO_INTR\n");
        }

        if( CHAL_CIR_RX_DONE_INTR & intr_status){
            CIR_DBG4("bcm_cir_intr_handler:  CHAL_CIR_RX_DONE_INTR\n");
        }

        /* Read all data from the fifo. */
        for (;;) {
            uint32_t sample;
            CHAL_CIR_FIFO_STATUS_t fifo;
            if ((chal_cir_rx_fifo_status_get(&cir->hw_handle, &fifo) != CHAL_CIR_ERR_NONE) ||
                fifo.depth == 0) {
                /* No more data in fifo. */
                break;
            }
            chal_cir_data_read(&cir->hw_handle, &sample); // Read a 32 bit int
            bcm_cir_store_sample(cir, sample);            // store data from sample into 
            //                                               cir->pkt_queue[cir->pkt_write_ptr]
            //                                               and increment pkt_queue[cir->pkt_write_ptr].count
        }

        if (!USE_INTERFERENCE_REDUCTION()) {
            if (cir->pkt_queue[cir->pkt_write_ptr].count > 0) {
                /* Since we've received (valid) data, cancel the release timer. */
                del_timer(&cir->release_timer);
            }
        }

        CIR_DBG4("count:%i\n", cir->pkt_queue[cir->pkt_write_ptr].count);

        if (!(intr_status & CHAL_CIR_RX_DONE_INTR)) {
            /* Expect more data for this packet. */
            /* Don't allow an ioctl to reconfigure the driver while actively reading a packet */ 
            cir->reading_pkt = 1;
        } else {
            /* That was the end of the packet. 
               increment the packet pointer if 
               the right amount of data was in 
               the packet, and unblock the reader.
            */ 

            if (cir->pkt_queue[cir->pkt_write_ptr].type == NO_REMOTE) {
                DPRINT("\nFlush (unknown proto) ");
                bcm_cir_reset_current_packet_state(cir);
                ir_glitch("U");
            }
            else {
                if (USE_INTERFERENCE_REDUCTION()) {
                    // Now that we've received a sufficient amount of valid data, cancel the release timer.
                    DPRINT("\nCancel release timer (count: %u) ", cir->pkt_queue[cir->pkt_write_ptr].count);
                    del_timer(&cir->release_timer);
                }

                // Reset the failsafe
                del_timer(&cir->failsafe_release_timer);

                DPRINT("\nStart release timer (count: %u) ", cir->pkt_queue[cir->pkt_write_ptr].count);
                bcm_cir_advance_pkt_write_ptr(cir);

                // Start the normal timer for the release event.
                mod_timer(&cir->release_timer, 
                          jiffies + msecs_to_jiffies(get_release_timer_timeout()));

                // Start the failsafe timer for the release event.
                mod_timer(&cir->failsafe_release_timer, 
                          jiffies + msecs_to_jiffies(get_failsafe_release_timer_timeout()));

            }
        
#if 1
            /* For some reason, if we don't do this,
             * no further rx interrupts will be received. */
            chal_cir_rx_disable(&cir->hw_handle);
            chal_cir_rx_enable(&cir->hw_handle);
#endif
        }
        spin_unlock_irqrestore(&cir->rx_waitq.lock, irq_flags);
    }

    return IRQ_HANDLED;
}

/***************************************************************************/
/**
 * Initialize the cir hardware blocks 
 */
static int bcm_cir_init_hw (struct bcm_cir *cir)
{
    int err=0;

    if((err = bcm_cir_init_clocks(cir)) != 0) {
        goto bcm_cir_init_hw_err;
    }

    /* Initialize the cir hw block */
    err = bcm_cir_conf_hwblock(cir->mode.tx_mode, cir, cir->peri_clk_freq, 0);
    if(0 != err) {
        CIR_ERR("error configuring cir hardware\n");
        goto bcm_cir_init_hw_err;
    } else {
        CIR_DBG4("HW Configured for: %dHz\n", (int)cir->peri_clk_freq);
    }

  bcm_cir_init_hw_err:
    return err;
}

/***************************************************************************/
/**
 * Reset and disable the block
 */
static void bcm_cir_reset_block(struct bcm_cir *cir, int is_tx)
{
    if(is_tx == BCM_CIR_TX_BLOCK) {
        cir->mode.tx_mode = BCM_IR_DEV_DISABLED;
        (void)bcm_cir_disable_block(cir, BCM_CIR_TX_BLOCK);
        memset(&cir->hw_cfg.txConfig, 0, sizeof(CHAL_CIR_TX_CONFIG_t));
    } else {
        cir->mode.rx_mode = BCM_IR_DEV_DISABLED;
        (void)bcm_cir_disable_block(cir, BCM_CIR_RX_BLOCK);
        memset(&cir->hw_cfg.rxConfig, 0, sizeof(CHAL_CIR_RX_CONFIG_t));
    }
}

/***************************************************************************/
/**
 * Configure both TX and RX for IR Protocol
 *   @return
 *      0      Success
 *      <0    Error
 */
static int bcm_cir_conf_ir_protocol(struct bcm_cir *cir, enum bcm_cir_device cfg_mode)
{
    int err = 0;

    cir->hw_cfg.intrConfig = CHAL_CIR_TX_DONE_INTR;

    memset(&cir->hw_cfg, 0, sizeof(CHAL_CIR_CONFIG_t));
    
    cir->hw_cfg.intrConfig = CHAL_CIR_TX_DONE_INTR | CHAL_CIR_RX_DONE_INTR | CHAL_CIR_RX_FIFO_INTR;
    /* Receiver CFG */
    cir->hw_cfg.rxConfig.activeState    = CHAL_CIR_RX_IP_ACTIVE_LOW;
    cir->hw_cfg.rxConfig.fifoThreshold  = 1;
    cir->hw_cfg.rxConfig.minPulseWidth  = minPulse;
    cir->hw_cfg.rxConfig.maxPulseWidth  = maxPulse;
    cir->hw_cfg.rxConfig.pwNoise        = noiseFilter;
    cir->hw_cfg.rxConfig.samplingPeriod = BCM_CIR_CALC_SAMPLING_FREQ(cir->peri_clk_freq);
    /* Transmitter CFG */
    cir->hw_cfg.txConfig.fifoThreshold  = 3;
    cir->hw_cfg.txConfig.mode           = CHAL_CIR_TX_MODE_CARRIER;
    cir->hw_cfg.txConfig.samplingPeriod = 0;
    cir->hw_cfg.txConfig.tcOffTime      = BCM_CIR_CALC_TOFF(cir->peri_clk_freq,NEC_TX_CARRIER_FREQ);
    cir->hw_cfg.txConfig.tcOnTime       = BCM_CIR_CALC_TON(cir->peri_clk_freq,NEC_TX_CARRIER_FREQ);
    
    CIR_DBG4("2: configuring rx: dmaEnable:%i, enable:%i activeState:%i fifoThreshold:%i minPulseWidth:%i maxPulseWidth:%i pwNoise:%i samplingPeriod:%i \n", 
             (int)cir->hw_cfg.rxConfig.dmaEnable,     
             (int)cir->hw_cfg.rxConfig.enable,
             (int)cir->hw_cfg.rxConfig.activeState,   
             (int)cir->hw_cfg.rxConfig.fifoThreshold, 
             (int)cir->hw_cfg.rxConfig.minPulseWidth, 
             (int)cir->hw_cfg.rxConfig.maxPulseWidth, 
             (int)cir->hw_cfg.rxConfig.pwNoise,       
             (int)cir->hw_cfg.rxConfig.samplingPeriod);
    
    err = (int)chal_cir_init(&cir->hw_handle, &cir->hw_cfg);
    if(err != 0) {
      err = -ENODEV;
      bcm_cir_reset_block(cir, BCM_CIR_TX_BLOCK);
      bcm_cir_reset_block(cir, BCM_CIR_RX_BLOCK);
      CIR_ERR("Failed to configure hw for NEC, disabling blocks\n");
    } else {
      cir->mode.tx_mode = BCM_IR_DEV_NEC;
      cir->mode.rx_mode = BCM_IR_DEV_NEC;
      CIR_DBG4("Configured for NEC\n");
    }
    
    return err;
}

/***************************************************************************/
/**
 * Initialize CIR Hardware
 *   @return
 *      0      Success
 *      <0    Error
 */
static int bcm_cir_conf_hwblock(enum bcm_cir_device  cfg_mode, /**< IR Protocol */
                                struct bcm_cir   *cir,    /**< Conf structure */
                                unsigned long         clkFreq,/**< Peripheral clock */
                                int                   is_tx   /**< 0-rx, 1-tx. Used for custom config */
                               )
{
    int err = 0;

    cir->peri_clk_freq = clkFreq;

    CIR_DBG4("bcm_cir_conf_hwblock\n");

    switch(cfg_mode) {
    case BCM_IR_DEV_NEC:
        err = bcm_cir_conf_ir_protocol(cir, cfg_mode);
        break;

    case BCM_IR_DEV_CUSTOM:
        if(is_tx) { /* Custom TX configuration */
            /* Enable interrupt */
            cir->hw_cfg.intrConfig = CHAL_CIR_TX_DONE_INTR;
            (void)chal_cir_interrupt_enable(&cir->hw_handle, cir->hw_cfg.intrConfig);

            cir->hw_cfg.txConfig.dmaEnable = 0;
            cir->hw_cfg.txConfig.enable = 0;
            err = (int)chal_cir_tx_config(&cir->hw_handle, &cir->hw_cfg.txConfig);
            if(err != 0) {
                err = -ENODEV;
                bcm_cir_reset_block(cir, BCM_CIR_TX_BLOCK);
            } else {
                cir->mode.tx_mode = BCM_IR_DEV_CUSTOM;
            }
        } else { /* Custom RX configuration */
            cir->hw_cfg.rxConfig.dmaEnable = 0;
            cir->hw_cfg.rxConfig.enable = 0;

            err = (int)chal_cir_rx_config(&cir->hw_handle, &cir->hw_cfg.rxConfig);
            if(err != 0) {
                err = -ENODEV;
                bcm_cir_reset_block(cir, BCM_CIR_RX_BLOCK);
            } else {
                cir->mode.rx_mode = BCM_IR_DEV_CUSTOM;
            }
        }
        break;

    case BCM_IR_DEV_DISABLED:
    {
        bcm_cir_reset_block(cir, is_tx);
        break;
    }

    default:
    {/* Using the passed configuration directly */
        CIR_ERR("Invalid mode\n");
        err = (-EINVAL);
        break;
    }
    }

    return err;
}

/***************************************************************************/
/**
 * Read a complete packet consisting of ROKU_REMOTE_PACKET_COUNT words.
 */
static int bcm_cir_read_packet(struct bcm_cir *cir, uint32_t rx_data[], uint64_t *interval, uint32_t *type, uint32_t *count)
{
    struct ir_packet *pkt;
    int rc;

    spin_lock_irq(&cir->rx_waitq.lock);

    rc = wait_event_interruptible_locked_irq(cir->rx_waitq,
                                             (!is_packet_queue_empty(cir)));
    if (rc < 0) {
        CIR_DBG4("Maybe a signal???  \n");
        spin_unlock_irq(&cir->rx_waitq.lock);
        return rc; // probably a signal
    }
    BUG_ON(is_packet_queue_empty(cir));

    /* Copy the next packet to the caller. */
    pkt       = &cir->pkt_queue[cir->pkt_read_ptr];
    *interval = pkt->interval;
    *type     = pkt->type;
    *count    = pkt->count;
    memcpy(rx_data, pkt->data, sizeof(pkt->data));

    CIR_DBG4("buffer index: %i  \n", cir->pkt_read_ptr);
    bcm_cir_advance_pkt_read_ptr(cir);

    spin_unlock_irq(&cir->rx_waitq.lock);
    return 0;
}

static void bcm_cir_set_protocol(struct bcm_cir *cir, uint32_t sample){
    unsigned hi = sample >> 16;
    unsigned lo = sample & 0xFFFF;
    CIR_DBG5("bcm_cir_set_protocol low:%x high:%x\n", (unsigned int)lo, (unsigned int)hi);

    if(parsingDirectv){
        if (THRESHOLD_WIDE(hi, NEC_LEAD_MARK_TIME) && THRESHOLD_WIDE(lo, NEC_LEAD_SPACE_TIME)){
           //if((hi > 0x2d40) && (lo > 0x1630)){
            CIR_DBG5("Roku IR detected \n");
            cir->pkt_queue[cir->pkt_write_ptr].type = ROKU_REMOTE;
        } else if ((hi < 0x300) && (lo > 0x3000)){
            CIR_DBG5("DirecTV IR detected \n");
            cir->pkt_queue[cir->pkt_write_ptr].type = DIRECTV_REMOTE;
        } else if ((hi > 0x1700) && (lo > 0x4b0)){
            CIR_DBG5("DirecTV IR detected \n");
            cir->pkt_queue[cir->pkt_write_ptr].type = DIRECTV_REMOTE;
        } else if ((hi > 0x600) && (lo < 0xf00) && (lo > 100)){
            CIR_DBG5("DirecTV IR detected \n");
            cir->pkt_queue[cir->pkt_write_ptr].type = DIRECTV_REMOTE;
        } else {
            CIR_DBG5("No IR protocol detected \n");
            cir->pkt_queue[cir->pkt_write_ptr].type = NO_REMOTE;
        }
    
    }
    else {
        if (THRESHOLD_WIDE(hi, NEC_LEAD_MARK_TIME) && THRESHOLD_WIDE(lo, NEC_LEAD_SPACE_TIME))
        {
            // OK, this seems to be a valid NEC preamble, so mark this as ROKU
            cir->pkt_queue[cir->pkt_write_ptr].type = ROKU_REMOTE;
            DPRINT("\n[NEC] preamb ");
        }
        else {
            cir->pkt_queue[cir->pkt_write_ptr].type = NO_REMOTE;
            DPRINT("\nFlush (bad preamble) ");
            ir_glitch("P");
        }
    }
}

/***************************************************************************/
/**
 * Decode a packet into a 32 bit word.
 */
static uint32_t bcm_cir_decode(uint32_t rx_data[], uint32_t type, int count)
{
    uint32_t code = 0;
    int i;
    unsigned hi;
    unsigned lo;

    switch(type){
    case ROKU_REMOTE:
    {
        for (i = 1;  (i < count) && (i < ROKU_REMOTE_PACKET_COUNT-1);  i++) {
            hi = rx_data[i] >> 16;
            lo = rx_data[i] & 0xFFFF;
            code >>= 1; 
            if (lo > 2*hi){
                code |= 0x80000000;
            }
          
            CIR_DBG3("ROKU:%i low:%x high:%x set:%i\n", i, (unsigned int)lo, (unsigned int)hi, (int)(lo > 2*hi));
        }
        DPRINT("\nRead code: %08x\n", code);
    }
    break;
    case DIRECTV_REMOTE:
    {
        for (i = 1;  (i < count) && (i < 16);  ++i) {
            hi = rx_data[i] >> 16;
            lo = rx_data[i] & 0xFFFF;
          
            if( lo > dtv_end){
                CIR_DBG3("%i Found the end (lo > 0x800) low:%x high:%x\n", i, (unsigned int)lo, (unsigned int)hi);
                break;
            }

            code <<= 1; // first bit of a two bit message
            if((hi > hi_bottom) && (hi < hi_top)){
                code |= 0x00000001;
            }

            code <<= 1; // second bit of a two bit message
            if((lo < lo_top) && (lo > lo_bottom)){
                code |= 0x00000001;
            }

            if((lo != 0) && (hi != 0)){
                CIR_DBG3("DTV:%i low:%x high:%x set:%i\n", i, (unsigned int)lo, (unsigned int)hi, code&3);
            }

        }
    }
    break;
    case NO_REMOTE:
    {
        DPRINT("\nRead code: %08x (release)\n", code);
    }
    break;
    }
      
    CIR_DBG3("code:%x \n", code);
    
    return code;
}

/***************************************************************************/
/**
 */
static uint32_t bcm_cir_hack_code(uint32_t code, uint64_t interval)
{
    // --- BEGIN Harmony Remote Control bug workaround ----------------
    // This is a workaround for the Harmony remote. Some of the harmony
    // remotes will always send a non-repeat event. Worse, they will
    // send the code 3 times even if the button is released immediately.
    // Unfortunately, this workaround interferes with ideal IR handling.
    // It means that subsequent button presses will be "missed" if they
    // fall within certain timing intervals, and affects *all* users.
    // Fortunately, it is a small interval.
    uint64_t avg = 40050000LL;
    uint64_t epsilon = 1000000;
    if (avg - epsilon <= interval && interval <= avg + epsilon) {
        // pretend this is a "repeat" event
        code |=  0x00800000; 
        code &=~ 0x80000000; 
    }
    // --- END Harmony Remote Control bug workaround ------------------

    return code;
}

/***************************************************************************/
/**
 * Read (blocking) data from the CIR. Blocking read is implemented.
 *   @return
 *      readLen  Bytes read
 *      < 0      Error
 */
static int bcm_cir_read(struct file  *file,
                        char __user  *rdBuff,
                        size_t       rdLen,
                        loff_t       *rdOff)
{
    struct bcm_cir *cir = file->private_data;
    size_t total = 0;
    int rc = 0;
    (void)rdOff;

    if (!cir->mode.rx_mode) {
        CIR_ERR("receiver disabled\n");
        return -ENODEV;
    }

    while (rdLen >= sizeof(uint32_t)) {
        uint32_t rx_data[PACKET_BUFFER_SIZE];
        uint64_t interval;
        uint32_t code;
        uint32_t type;
        uint32_t count;
        rc = bcm_cir_read_packet(cir, rx_data, &interval, &type, &count);
        if (rc < 0)
            return rc;
        code = bcm_cir_decode(rx_data, type, count);
        code = bcm_cir_hack_code(code, interval);
        rc = copy_to_user(rdBuff, &code, sizeof(code));
        if(rc > 0) {
            rdBuff += rc;
            rdLen -= rc;
            total += rc;
            break;
        }
        rdBuff += sizeof(uint32_t);
        rdLen -= sizeof(uint32_t);
        total += sizeof(uint32_t);
    }

    return (rc > 0) ? rc : total;
}

/***************************************************************************/
/**
 * Write (unblocking) data to the CIR. Blocking write is implemented
 *   @return
 *      wrLen   Bytes written
 *      <0      Error
 */
static int bcm_cir_write(struct file        *file,
                         const char __user  *wrBuff,
                         size_t             wrLen,
                         loff_t             *wrOff)
{
    // Write is not supported
    return -ENODEV;
}

/***************************************************************************/
/**
 * CIR device control. Configurations can be changed here.
 *   @return
 *      0      Success
 *      <0     Error
 */
static long  bcm_cir_ioctl(struct file   *file,
                           unsigned int  cmd,
                           unsigned long arg )
{
    struct bcm_cir *cir = file->private_data;
    long  err = 0;
    (void)file;

    switch(cmd) {
    case BCM_CIR_IOCTL_SET_IR_DEV:
    {
        enum bcm_cir_device ir_mode;

        if(copy_from_user(&ir_mode, (void *)arg, sizeof(enum bcm_cir_device)) != 0) {
            CIR_ERR("ioctl=BCM_CIR_IOCTL_SET_IR_DEV failed: data copy failed\n");
            return (-EFAULT);
        }
        if((ir_mode != BCM_IR_DEV_SIRC) && (ir_mode != BCM_IR_DEV_NEC)) {
            CIR_ERR("ioctl=BCM_CIR_IOCTL_SET_IR_DEV failed: invalid ir device\n");
            return (-EINVAL);
        }
        spin_lock_irq(&cir->rx_waitq.lock);
        if (cir->reading_pkt) {
            err = -EAGAIN;
        } else {
            if((err = bcm_cir_conf_hwblock(ir_mode, cir, cir->peri_clk_freq, 0)) != 0) {
                CIR_ERR("ioctl=BCM_CIR_IOCTL_SET_IR_DEV failed: device conf failed\n");
            }
        }
        spin_unlock_irq(&cir->rx_waitq.lock);
        break;
    }

    case BCM_CIR_IOCTL_DISABLE_BLOCK:
    {/* This is intended for the driver to be configured to work only as TX or RX
      * Once disabled the block can't probably be enabled. This is not implemented.
      */
        char is_tx=0;
        if(copy_from_user(&is_tx, (void *)arg, sizeof(char)) != 0) {
            CIR_ERR("ioctl=BCM_CIR_IOCTL_DISABLE_BLOCK failed: data copy failed\n");
            return (-EFAULT);
        }
        is_tx = (is_tx) ? BCM_CIR_TX_BLOCK : BCM_CIR_RX_BLOCK;

        spin_lock_irq(&cir->rx_waitq.lock);
        if (!is_tx && cir->reading_pkt) {
            err = -EAGAIN;
        } else {
            err = bcm_cir_conf_hwblock(BCM_CIR_IOCTL_DISABLE_BLOCK,
                                       cir, cir->peri_clk_freq, is_tx);
            if(err != 0) {
                CIR_ERR("ioctl=BCM_CIR_IOCTL_DISABLE_BLOCK failed: device conf failed\n");
            }
            bcm_cir_block_shutdown(cir, BCM_CIR_TX_BLOCK);
        }
        spin_unlock_irq(&cir->rx_waitq.lock);
        break;
    }

    case BCM_CIR_IOCTL_CONF_TX:
    {
        struct bcm_cir_tx_conf tx_conf;

        if(copy_from_user(&tx_conf, (void *)arg, sizeof(struct bcm_cir_tx_conf)) != 0) {
            CIR_ERR("ioctl=BCM_CIR_IOCTL_CONF_TX failed: data copy failed\n");
            return (-EFAULT);
        }

        spin_lock_irq(&cir->rx_waitq.lock);
        cir->hw_cfg.txConfig.dmaEnable=0;
        cir->hw_cfg.txConfig.enable = 0;
        cir->hw_cfg.txConfig.fifoThreshold = 3;
        cir->hw_cfg.txConfig.mode=CIR_GET_CARRIER_MODE(tx_conf.mode);
        cir->hw_cfg.txConfig.samplingPeriod = tx_conf.samplingPeriod;
        cir->hw_cfg.txConfig.tcOffTime = BCM_CIR_CALC_TOFF(cir->peri_clk_freq,tx_conf.carrFreq);
        cir->hw_cfg.txConfig.tcOnTime = BCM_CIR_CALC_TON(cir->peri_clk_freq,tx_conf.carrFreq);

        err = bcm_cir_conf_hwblock(BCM_IR_DEV_CUSTOM,
                                   cir,
                                   cir->peri_clk_freq,
                                   BCM_CIR_TX_BLOCK);
        spin_unlock_irq(&cir->rx_waitq.lock);
        if(err != 0) {
            CIR_ERR("ioctl=BCM_CIR_IOCTL_CONF_TX failed: device conf failed\n");
        }
        break;
    }

    case BCM_CIR_IOCTL_CONF_RX:
    {
        struct bcm_cir_rx_config rx_conf;

        if(copy_from_user(&rx_conf, (void *)arg, sizeof(struct bcm_cir_rx_config)) != 0) {
            CIR_ERR("ioctl=BCM_CIR_IOCTL_CONF_RX failed: data copy failed\n");
            return (-EFAULT);
        }

        spin_lock_irq(&cir->rx_waitq.lock);
        if (cir->reading_pkt) {
            err = -EAGAIN;
        } else {
            cir->hw_cfg.rxConfig.dmaEnable      = 0;
            debugCIR                            = rx_conf.debug;
            hi_top                              = rx_conf.hi_top;
            hi_bottom                           = rx_conf.hi_bottom;
            lo_top                              = rx_conf.lo_top;
            lo_bottom                           = rx_conf.lo_bottom;
            dtv_end                             = rx_conf.dtv_end;
            cir->hw_cfg.rxConfig.enable         = 0;
            cir->hw_cfg.rxConfig.activeState    = (CHAL_CIR_RX_IP_STATE_t)rx_conf.activeState;
            cir->hw_cfg.rxConfig.fifoThreshold  = 1;
            cir->hw_cfg.rxConfig.minPulseWidth  = minPulse = (uint16_t)rx_conf.minPulseWidth;
            cir->hw_cfg.rxConfig.maxPulseWidth  = maxPulse = (uint16_t)rx_conf.maxPulseWidth;
            cir->hw_cfg.rxConfig.pwNoise        = noiseFilter = rx_conf.pwNoise;
            cir->hw_cfg.rxConfig.samplingPeriod = rx_conf.samplingPeriod;
   
            CIR_ERR("configuring rx: dmaEnable:%i, enable:%i activeState:%i fifoThreshold:%i minPulseWidth:%i maxPulseWidth:%i pwNoise:%i samplingPeriod:%i debug:%i\n", 
                    (int)cir->hw_cfg.rxConfig.dmaEnable,     
                    (int)cir->hw_cfg.rxConfig.enable,
                    (int)cir->hw_cfg.rxConfig.activeState,   
                    (int)cir->hw_cfg.rxConfig.fifoThreshold, 
                    (int)cir->hw_cfg.rxConfig.minPulseWidth, 
                    (int)cir->hw_cfg.rxConfig.maxPulseWidth, 
                    (int)cir->hw_cfg.rxConfig.pwNoise,       
                    (int)cir->hw_cfg.rxConfig.samplingPeriod, 
                    (int)debugCIR);

            err = bcm_cir_conf_hwblock(BCM_IR_DEV_CUSTOM,
                                       cir,
                                       cir->peri_clk_freq,
                                       BCM_CIR_RX_BLOCK);
        }
        spin_unlock_irq(&cir->rx_waitq.lock);
        if(err != 0) {
            CIR_ERR("ioctl=BCM_CIR_IOCTL_CONF_RX failed: device conf failed\n");
        }
        break;
    }

    case BCM_CIR_IOCTL_SET_RX_DMA_MEM:
        err = (-EINVAL);
        CIR_ERR("ioctl=BCM_CIR_IOCTL_SET_RX_DMA_MEM failed: not supported\n");
        break;

    case BCM_CIR_IOCTL_GET_IR_DEV:
        err = (-EINVAL);
        CIR_ERR("ioctl=BCM_CIR_IOCTL_GET_IR_DEV failed: not supported\n");
        break;

    case BCM_CIR_IOCTL_SET_TX_DMA_MEM:
        err = (-EINVAL);
        CIR_ERR("ioctl=BCM_CIR_IOCTL_SET_TX_DMA_MEM failed: not supported\n");
        break;

    default:
        err = (-EINVAL);
        CIR_ERR("unknown ioctl command\n");
        break;
    }
    chal_cir_rx_disable(&cir->hw_handle);
    chal_cir_rx_enable(&cir->hw_handle);

    return err;
}

/***************************************************************************/
/**
 * Open the cir. Initilization for the session is done here
 *   @return
 *      0      Success
 *      < 0    Error
 */
static int bcm_cir_open(struct inode *inode, struct file *file)
{
    struct bcm_cir *cir = the_bcm_cir;
    (void)inode;

    if (atomic_add_return(1, &bcm_dev.dev_is_open) != 1) {
        atomic_dec(&bcm_dev.dev_is_open);
        return -EBUSY;
    }

    file->private_data = cir;
    chal_cir_rx_enable(&cir->hw_handle);
    cir->last_sample_ktime_ = bcm_cir_now(); /* avoid huge interval in first sample. */

    CIR_DBG4("Open complete\n");
    return 0;
}

/***************************************************************************/
/**
 * Close CIR driver
 *   @return
 *      0      Success
 *      < 0    Error
 */
static int bcm_cir_release(struct inode *inode, struct file *file)
{
    struct bcm_cir *cir = file->private_data;

    /* shutdown hardware */
    bcm_cir_block_shutdown(cir, BCM_CIR_TX_BLOCK);
    bcm_cir_block_shutdown(cir, BCM_CIR_RX_BLOCK);

    atomic_dec(&bcm_dev.dev_is_open);
    CIR_DBG4("released\n");

    return 0;
}

/***************************************************************************/
static struct bcm_cir *bcm_cir_create(void)
{
    struct bcm_cir *cir;

    cir = kzalloc(sizeof(*cir), GFP_KERNEL);
    if (!cir) return ERR_PTR(-ENOMEM);
    cir->hw_handle.regBaseAddr = KONA_CIR_VA;
    cir->peri_clk_freq = KPM_APB1_CLK_FREQ;
    cir->mode.rx_mode = cir->mode.tx_mode = BCM_IR_DEV_NEC;
    init_waitqueue_head(&cir->rx_waitq);
    setup_timer(&cir->release_timer, &bcm_cir_release_timeout, (unsigned long) cir);
    setup_timer(&cir->failsafe_release_timer, &bcm_cir_release_timeout, (unsigned long) cir);

    return cir;
}

/***************************************************************************/
/**
 * Initialize the cir Character driver
 *   @return
 *      0      Success
 *      <0     Error
 */
static int __init bcm_cir_init(void)
{
    int err=0;

    atomic_set(&bcm_dev.dev_is_open, 0);

    /* register char device with dynamic major no */
    bcm_dev.dev_major = register_chrdev(0, BCM_CIR_DEV_NAME, &cir_fops);
    if(0 >= bcm_dev.dev_major) {
        CIR_ERR("char device create failed, major:%d\n",
                bcm_dev.dev_major);
        return (bcm_dev.dev_major);
    }

    /* create the device */
    bcm_dev.dev_class = class_create(THIS_MODULE, BCM_CIR_DEV_NAME);
    if (IS_ERR(bcm_dev.dev_class)) {
        CIR_ERR("class create failed\n");
        err = PTR_ERR(bcm_dev.dev_class);
        goto cir_init_err_class_create;
    }

    bcm_dev.dev = device_create(bcm_dev.dev_class, NULL,
                                MKDEV(bcm_dev.dev_major, BCM_CIR_DEV_MINOR),
                                NULL, BCM_CIR_DEV_NAME);
    if ( IS_ERR( bcm_dev.dev )) {
        CIR_ERR("device create failed\n");
        err = PTR_ERR(bcm_dev.dev);
        goto cir_init_err_device_create;
    } else {
        CIR_DBG4("cir char device created, major:%d\n", bcm_dev.dev_major);
    }

    the_bcm_cir = bcm_cir_create();
    if (IS_ERR(the_bcm_cir))
        return PTR_ERR(the_bcm_cir);

    err = bcm_cir_init_hw(the_bcm_cir);
    if (err < 0) {
        kfree(the_bcm_cir);
        CIR_ERR("hardware init failed\n");
        return err;
    }

    /* Install Interrupt Handler */
    if (( err = request_irq( BCM_INT_ID_CIR, bcm_cir_intr_handler,
                             IRQF_SHARED, "cir handler", the_bcm_cir)) != 0 )
        {
            CIR_ERR( KERN_ERR "request_irq for cir failed\n" );
        }

    cir_sysctl_header = register_sysctl_table(cir_root_table);



    return 0;

    /* Error handling */
  cir_init_err_device_create:
    class_destroy(bcm_dev.dev_class);

  cir_init_err_class_create:
    unregister_chrdev(bcm_dev.dev_major, BCM_CIR_DEV_NAME);

    return (err);
}

/***************************************************************************/
/**
 * Exit the cir driver
 */
static void __exit bcm_cir_exit(void)
{
    if (the_bcm_cir) {
        chal_cir_exit(&the_bcm_cir->hw_handle);
        free_irq(BCM_INT_ID_CIR, the_bcm_cir);
        kfree(the_bcm_cir);
        the_bcm_cir = NULL;
    }

    /* Destroy device */
    device_destroy(bcm_dev.dev_class, MKDEV(bcm_dev.dev_major, bcm_dev.dev_minor));
    class_destroy(bcm_dev.dev_class);
    unregister_chrdev(bcm_dev.dev_major, BCM_CIR_DEV_NAME);
    unregister_sysctl_table(cir_sysctl_header);

    CIR_DBG4("cir char device destroyed, major:%d, minor:%d\n",
             bcm_dev.dev_major, bcm_dev.dev_minor);
}


module_init(bcm_cir_init);
module_exit(bcm_cir_exit);

MODULE_AUTHOR("Broadcom");
MODULE_DESCRIPTION("Board Specific Consumer Infra-Red (CIR) Driver");
MODULE_VERSION("roku-brcm-ir-1.0.0.3:" __DATE__ " " __TIME__);
MODULE_LICENSE("GPL");
