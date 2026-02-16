/*
 *
 * puma_spi_master.c
 * Description:
 * spi master controller driver.
 *
 *
 * GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2014-2015 Intel Corporation.
 *
 *  This program is free software; you can redistribute it and/or modify 
 *  it under the terms of version 2 of the GNU General Public License as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
 *  General Public License for more details.
 *
 *
 *  You should have received a copy of the GNU General Public License 
 *  along with this program; if not, write to the Free Software 
 *  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *  The full GNU General Public License is included in this distribution 
 *  in the file called LICENSE.GPL.
 *
 *  Contact Information:
 *  Intel Corporation
 *  2200 Mission College Blvd.
 *  Santa Clara, CA  97052
 */

#include <linux/proc_fs_macros.h>
#include <asm-arm/arch-avalanche/generic/pal.h>
#include <asm-arm/arch-avalanche/puma7/spi_master/puma_spi_master.h>
#include <asm-arm/arch-avalanche/puma7/spi_master/puma_spi_master_proxy.h>

// #define NEED_SPI_MUX_MUTEX
#ifdef NEED_SPI_MUX_MUTEX
#include <puma7_bootcfg_ctrl.h>
#endif

/* Number of iterations to wait for transfer completed */
#define WAIT_NUMBER  5000

/* Delay of an iteration, in microseconds */
#define WAIT_TIMEOUT 100


#define USE_REAL_REGISTERS


#define CHAR_LEN_MASK    0x000003FF
#define CLK_DIVIDER_MASK 0x0000FFFF
#define DELAY_MASK       0x0000000F
#define GO_BSY_BIT       0x00010000
#define LSB_BIT          0x00001000
#define RX_NEG_BIT       0x00000400
#define TX_NEG_BIT       0x00000800

/* SPI controller registers */
struct puma_spi_regs {
    /* TX data */
    volatile u32 spi_tx[MAX_TRANSFER/4];

    /* RX data */
    volatile u32 spi_rx[MAX_TRANSFER/4];

    /* Control and status register */
    volatile u32 ctrl;

    /* Divider register */
    volatile u32 divider;

    /* Slave Select register */
    volatile u32 ss;
};

static struct puma_spi_regs __iomem *io_base;


static u32 board_ref_freq;


#ifndef USE_REAL_REGISTERS
struct puma_spi_regs FAKE_REGISTERS;
#endif


static struct puma_spi_device devices[MAX_CHIPSELECT] = {
    {
        .chipselect = 1,
        .speed_hz = DEFAULT_SPEED_HZ
    },
    {
        .chipselect = 2,
        .speed_hz = DEFAULT_SPEED_HZ
    },
    {
        .chipselect = 3,
        .speed_hz = DEFAULT_SPEED_HZ
    },
    {
        .chipselect = 4,
        .speed_hz = DEFAULT_SPEED_HZ
    },
    {
        .chipselect = 5,
        .speed_hz = DEFAULT_SPEED_HZ
    },
    {
        .chipselect = 6,
        .speed_hz = DEFAULT_SPEED_HZ
    },
    {
        .chipselect = 7,
        .speed_hz = DEFAULT_SPEED_HZ
    },
    {
        .chipselect = 8,
        .speed_hz = DEFAULT_SPEED_HZ
    }
};

static int print_16_bytes(struct seq_file *m, volatile u8 *ptr, unsigned start)
{
    ptr += start;
    seq_printf(m,
        "%04x: "
        "%02x %02x %02x %02x %02x %02x %02x %02x "
        "%02x %02x %02x %02x %02x %02x %02x %02x\n",
        start,
        ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5], ptr[6], ptr[7],
        ptr[8], ptr[9], ptr[10], ptr[11], ptr[12], ptr[13], ptr[14], ptr[15]);

}

static int puma_spi_proc_read(struct seq_file *m, void *v)
{
    int i;

    seq_printf(m, "ctrl:    %08x\n", io_base->ctrl);
    seq_printf(m, "divider: %08x\n", io_base->divider);
    seq_printf(m, "ss:      %08x\n", io_base->ss);

    seq_printf(m, "spi_tx:\n");
    for (i = 0; i < 8; ++i) {
        print_16_bytes(m, io_base->spi_tx, i * 16);
    }

    seq_printf(m, "spi_rx:\n");
    for (i = 0; i < 8; ++i) {
        print_16_bytes(m, io_base->spi_rx, i * 16);
    }

    return 0;
}

int puma_spi_device_open(unsigned chipselect)
{
    int ret = 0;

    if ((chipselect == 0) || (chipselect > MAX_CHIPSELECT)) {
        ret = -ENODEV;
    }

    return ret;
}

int puma_spi_device_close(unsigned chipselect)
{
    /* Nothing to do here */
    return 0;
}

static int wait_for_transfer(const void *tx_buf, void *rx_buf, unsigned len)
{
    int finished;
    u8 i;
    int ret = 0;
    u8 *temp1;
    int word_pos =0;
    int byte_id = len;
    
    /* Copy Tx data */
    if (tx_buf) 
    {
        temp1 = tx_buf;

        while (byte_id)
        {
            unsigned char i;
            unsigned int word = 0;

            for (i = 0; (i < 4) && (byte_id); i++, byte_id--)
            {
                word |= (temp1[byte_id-1] << (i*8));
            }
            io_base->spi_tx[word_pos] = word;
            word_pos++;
        }
        //printk("copy tx buff to mem len=%d\n", len);
    }
    else
    {
        for (i = 0; i < MAX_TRANSFER/4 ; i++)
        {
            io_base->spi_tx[i] = 0;
        }
    }

    /* Set GO_BSY bit */
    io_base->ctrl |= GO_BSY_BIT;

    /* Wait for GO_BSY bit to be cleared */
    finished = 0;
    for (i = 0; i < WAIT_NUMBER; ++i) {
        if (io_base->ctrl & GO_BSY_BIT) {
            udelay(WAIT_TIMEOUT);
        }
        else {
            finished = 1;
            break;
        }
    }

    if (finished) 
    {
        /* Copy Rx data */
        if (rx_buf) 
        {
            byte_id = len;
            word_pos = 0;
            u8 * rx_bufB = (u8*)rx_buf; 
            
            while (byte_id)
            {
              for (i = 0; (i < 4) && (byte_id); i++, byte_id--)
              {
                  u32 var = io_base->spi_rx[word_pos];
                  rx_bufB[byte_id - 1] = (var >> (i*8)) & 0xff;
              }
              word_pos++;
            }
        }
    }
    else {
        ret = -EBUSY;
    }

    return ret;
}

int puma_spi_device_transfer(unsigned chipselect, const u8 *tx, u8 *rx,
                                    unsigned len)
{
    struct puma_spi_device *device;
    u32 char_len;
    u32 clk_div;
    u32 delays;
    int ret = 0;

    if ((chipselect == 0) || (chipselect > MAX_CHIPSELECT)) {
        ret = -ENODEV;
        goto out;
    }
    if (len > MAX_TRANSFER) {
        ret = -EMSGSIZE;
        goto out;
    }
#ifdef NEED_SPI_MUX_MUTEX
    /* We set the SPI MUX to be ARM11 and we lock the HW-Mutex - for cases that MOCA CPU will need the SPI. */
    if (PAL_sysBootCfgCtrl_SetAndLockSPIMux(BOOTCFG_SPI_MASTER_ARM11))
    {
        ret = -EPERM;
        goto out;
    }
#endif
    device = &devices[chipselect - 1];

    /* Divider register: Fsclk = Fclk / ((divider + 1) * 2) */
    //clk_div = (board_ref_freq / (2 * device->speed_hz)) - 1;
    /*TBD: Need to set the clock correctly*/
    clk_div = 24;
    io_base->divider = clk_div & CLK_DIVIDER_MASK;

    /* Control and status register */
    io_base->ctrl = 0;

    /* Set start/end transaction delays. Delays are represented by 4 bits. */
    delays = device->end_delay & DELAY_MASK;
    delays <<= 4;
    delays |= (device->start_delay & DELAY_MASK);
    io_base->ctrl |= (delays << 24);

    /* Set LSB/MSB */
    if (device->lsb) {
        io_base->ctrl |= LSB_BIT;
    }

    /* Set RX/TX NEG bits */
    if (device->tx_neg) {
        io_base->ctrl |= TX_NEG_BIT;
    }
    if (device->rx_neg) {
        io_base->ctrl |= RX_NEG_BIT;
    }
    /*TBD: Need to change device default to RX_NEG*/
    io_base->ctrl |= RX_NEG_BIT;
    /* Prepare an actual transfer */
    char_len = len << 3;
    char_len &= CHAR_LEN_MASK;
    io_base->ctrl |= char_len;

    ret = wait_for_transfer(tx, rx, len);

#ifdef NEED_SPI_MUX_MUTEX
    /* Free the SPI Mux LOCK (HW-Mutex) */
    PAL_sysBootCfgCtrl_FreeSPIMux();
#endif

out:
    return ret;
}


static int __init puma_spi_probe(struct platform_device *pdev)
{
    struct resource *r;
    int ret = 0;

    r = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if (r == NULL) {
        ret = -ENODEV;
        goto out;
    }

#ifdef USE_REAL_REGISTERS
    io_base = (struct puma_spi_regs __iomem *)(r->start);
    if (io_base == NULL) {
        ret = -ENODEV;
        goto out;
    }
#else
    io_base = &FAKE_REGISTERS;
#endif

    board_ref_freq = PAL_sysClkcGetFreq(AVALANCHE_SPI_MASTER_REF_CLOCK);

out:
    return ret;
}

static int puma_spi_remove(struct platform_device *pdev)
{
    return 0;
}


static struct class *spidev_class;

static int create_devices(void)
{
    struct device *dev;
    dev_t devt;
    char dev_name[64];
    int i;
    int ret = 0;

    spidev_class = class_create(THIS_MODULE, "spidev");
    if (IS_ERR(spidev_class)) {
        ret = PTR_ERR(spidev_class);
        spidev_class = NULL;
        goto out;
    }

    for (i = 0; i < MAX_CHIPSELECT; ++i) {
        devt = MKDEV(PUMA_SPI_MASTER_MAJOR, i + 1);
        snprintf(dev_name, sizeof(dev_name), "puma_spi%u", i + 1);
        dev = device_create(spidev_class, NULL, devt, NULL, dev_name);
        if (IS_ERR(dev)) {
            ret = PTR_ERR(dev);
            goto out;
        }
    }

out:
    return ret;
}

static struct platform_driver puma_spi_master_driver = {
    .driver = {
        .name  = "puma_spi_master",
        .owner = THIS_MODULE
    },
    .probe  = puma_spi_probe,
    .remove = puma_spi_remove
};


DECLARE_PROCFS_ENTRY(puma_spi, puma_spi_proc_read, NULL)

int puma_spi_master_init(struct platform_device *master_device)
{
    struct proc_dir_entry *parent;
    struct proc_dir_entry *proc_entry;
    int ret;

    ret = platform_device_register(master_device);
    if (ret < 0) {
        goto out;
    }

    ret = platform_driver_register(&puma_spi_master_driver);
    if (ret < 0) {
        platform_device_unregister(master_device);
        goto out;
    }

    parent = proc_mkdir("puma_spi_master", NULL);
    if (parent == NULL) {
        ret = -ENOMEM;
        goto out;
    }
    proc_entry = proc_create("regs", 0444, parent, &puma_spi_proc_fops);
    if (proc_entry == NULL) {
        ret = -ENOMEM;
        goto out;
    }

    ret = create_devices();

out:
    return ret;
}

int puma_alloc_spi_device(struct puma_spi_device *device)
{
    struct puma_spi_device *dev;
    int index;
    int ret = 0;

    if ((device->chipselect == 0) || (device->chipselect > MAX_CHIPSELECT)) {
        ret = -ENODEV;
        goto out;
    }

    index = device->chipselect - 1;
    dev = &devices[index];

    dev->speed_hz = device->speed_hz;
    dev->end_delay = device->end_delay;
    dev->start_delay = device->start_delay;
    dev->lsb = device->lsb;
    dev->tx_neg = device->tx_neg;
    dev->rx_neg = device->rx_neg;

out:
    return ret;
}
