/*****************************************************************************
* Copyright (c) 2011 Broadcom Corporation.  All rights reserved.
*
* This program is the proprietary software of Broadcom Corporation and/or
* its licensors, and may only be used, duplicated, modified or distributed
* pursuant to the terms and conditions of a separate, written license
* agreement executed between you and Broadcom (an "Authorized License").
* Except as set forth in an Authorized License, Broadcom grants no license
* (express or implied), right to use, or waiver of any kind with respect to
* the Software, and Broadcom expressly reserves all rights in and to the
* Software and all intellectual property rights therein.  IF YOU HAVE NO
* AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY
* WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF
* THE SOFTWARE.
*
* Except as expressly set forth in the Authorized License,
* 1. This program, including its structure, sequence and organization,
*    constitutes the valuable trade secrets of Broadcom, and you shall use
*    all reasonable efforts to protect the confidentiality thereof, and to
*    use this information only in connection with your use of Broadcom
*    integrated circuit products.
* 2. TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
*    AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
*    WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
*    RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL
*    IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS
*    FOR A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS,
*    QUIET ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU
*    ASSUME THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
* 3. TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
*    LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT,
*    OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO
*    YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN
*    ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS
*    OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER
*    IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF
*    ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*****************************************************************************/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/wait.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/freezer.h>
#include <linux/proc_fs.h>
#include <linux/clk.h>
#include <linux/i2c.h>
#include <linux/gpio.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <asm/io.h>
#include <asm/gpio.h>
#include <asm/system.h>
#include <asm/uaccess.h>
#include <linux/poll.h>
#include <linux/kfifo.h>
#include <linux/version.h>
#include <linux/input.h>
#include <linux/irq.h>
#include <linux/firmware.h>

#include <linux/i2c/bcm915500_i2c_ts.h>

#define BCM_TSC_CHIP_REV 64
#define BCM_TSC_CHIP_ID0 65
#define BCM_TSC_CHIP_ID1 66
#define BCM_TSC_CHIP_ID2 67

#define BCM_TSC_SPM_SLAVE 0x66
#define BCM_TSC_AHB_SLAVE 0x68

/* ----------- */
/* - Defines - */
/* ----------- */

#define BRING_UP_WORKAROUND                             1
#define BOOT_FROM_RAM                                   1

#define NAPA_INIT_SUCCESS                               0
#define NAPA_INIT_ERROR                                 1

#define NAPA_MUTEX                                      0

#define NAPA_I2C_SLAVE_ADDR                          0x66
#define NAPA_I2C_AHB_SLAVE_ADDR                      0x68

//mode values for DMA control
#define MahbRead                                        1
#define MahbWrite                                       3

#define I2C_REG_DMA_ADDR                              128
#define I2C_REG_STATUS                                137
#define I2C_REG_WFIFO_DATA                            146
#define I2C_REG_RFIFO_DATA                            162


#define TCC_REG_SPM_RevisionID                         64
#define TCC_REG_SPM_ChipID0                            65
#define TCC_REG_SPM_ChipID1                            66
#define TCC_REG_SPM_ChipID2                            67
#define TCC_REG_SPM_SPI_I2C_MODE                       68

#define TCC_REG_SPM_PSR                                72
#define TCC_REG_SPM_Request_from_Host                  76
#define TCC_REG_SPM_MSG_TO_HOST                        77
#define TCC_REG_SPM_MSG_FROM_HOST                      78

#define TCC_REG_SPM_SOFT_RESETS                        89

#define TCC_REG_SPM_ALFO_CTRL                          96

#define TOFE_MESSAGE_FW_READY                         128
#define TOFE_COMMAND_NO_COMMAND                         0
#define SPM_POWER_STATE_SLEEP                           0
#define SPM_POWER_STATE_ACTIVE                          4
#define TCC_HOST_IF_I2C_MODE                            0
#define TCC_SPM_REQUEST_FROM_HOST_WAKE_REQUEST          2
#define TCC_SPM_REQUEST_FROM_HOST_RELEASE_WAKE_REQUEST  0

#define COMMON_FLL_CTRL0                               (0x30110000+0x104)
#define COMMON_FLL_LPF_CTRL2                           (0x30110000+0x114)
#define COMMON_FLL_TEST_CTRL1                          (0x30110000+0x144)
#define SPM_PWR_CTRL                                   (0x30000000 + 0x1c + 0x00100000)
#define COMMON_SYS_HCLK_CTRL                           (0x30110000+0x20)
#define COMMON_CLOCK_ENABLE                            (0x30110000+0x48)
#define TCH_VERSION_REGISTER                           (0x30000000+0x00300000)
#define COMMON_ARM_REMAP_ADDR                          (0x30110000)
#define SPM_STICKY_BITS                                (0x30000000 + 0x00100000 + 0x144)

#define SPM_READ_ERROR_MAX                             200

#define VECTORS_OFFSET                                 0
#define ROM_OFFSET                                     (0x10000000)
#define DATA_OFFSET                                    (0x10008000)

#define BCM915500_TSC_FW_CODE "bcmtchfw_code"
#define BCM915500_TSC_FW_DATA "bcmtchfw_data"
#define BCM915500_TSC_FW_VECT "bcmtchfw_vect"

struct bcmtsc_i2c
{
   struct mutex i2c_mutex;
   struct i2c_client *p_i2c_client;
   struct i2c_client *p_i2c_client1;
   int pagesize;
};

static struct bcmtsc_i2c *p_bcmtsc_i2c = NULL;

/*
struct i2c_board_info
{
   char            type[I2C_NAME_SIZE];
   unsigned short  flags;
   unsigned short  addr;
   void            *platform_data;
   struct dev_archdata     *archdata;
   struct device_node *of_node;
   int             irq;
};
*/

static struct i2c_board_info bcm915500_i2c_boardinfo =
{
   .type = BCM915500_TSC_NAME,
   .addr = BCM915500_SLAVE_2,
   .platform_data = NULL,
};

/* -------------------- */
/* - Local Variables. - */
/* -------------------- */

static unsigned char I2Cheader[8] = {I2C_REG_DMA_ADDR, /*addr*/ 0x00, 0x00, 0x00, 0x00, /*len*/ 0x00, 0x00, MahbWrite};

static unsigned char *gp_code;
static unsigned char *gp_vector;
static unsigned char *gp_data;
int g_dowload_fw_count = 0;
int g_code_size;
int g_vector_size;
int g_data_size;

static struct i2c_client *gp_i2c_client_spm = NULL;
static struct i2c_client *gp_i2c_client_ahb = NULL;

int napa_mutex_lock(int mutex);
int napa_mutex_release(int mutex);
void napa_sleep_ms(int ms);
int napa_i2c_read(int slave_addr, int len, unsigned char *buffer);
int napa_i2c_write(int slave_addr, int len, unsigned char *buffer);
unsigned char napa_i2c_read_reg(int slave_addr, int reg);
int napa_i2c_write_reg(int slave_addr, int reg, unsigned char data);
int napa_i2c_write_ahb( int ahb_addr, int len, void *data);
int napa_i2c_read_ahb(int ahb_addr, int len, void *data);
int napa_i2c_write_ahb_reg32(int ahb_addr, int data);
int napa_i2c_read_ahb_reg32( int ahb_addr, void *data);
int napa_download(void);
int napa_wait_for_fw_ready(void);

/* ------------- */
/* - Functions - */
/* ------------- */

int napa_init(struct i2c_client *p_i2c_client_spm, struct i2c_client *p_i2c_client_ahb)
{
   unsigned char chipId[4];
   unsigned char regVal;
   int           data;
   int           spmReadError;
   int           length = 1;
   u8            buffer[10];
   int           count;
   unsigned char dataByte;
   unsigned char alfo_ctrl;
   int           ret;

   unsigned int  reg;
   unsigned int  tch_version = 0xdeadbeef;
   unsigned int  remap;
   unsigned int  firmware_ok;

   gp_i2c_client_spm = p_i2c_client_spm;
   gp_i2c_client_ahb = p_i2c_client_ahb;

   /* 0. Set proper communication interface - SPI or I2C mode */
   ret = napa_i2c_write_reg(NAPA_I2C_SLAVE_ADDR, TCC_REG_SPM_SPI_I2C_MODE, TCC_HOST_IF_I2C_MODE);
   printk("%s() line %d ret: %d\n", __func__, __LINE__, ret);

   /** 1. Read ChipId & VersionID --> Verify */
   printk("%s() line %d 1. Read ChipId & VersionID --> Verify\n", __func__, __LINE__);
   chipId[0] = napa_i2c_read_reg(NAPA_I2C_SLAVE_ADDR, TCC_REG_SPM_ChipID0);
   chipId[1] = napa_i2c_read_reg(NAPA_I2C_SLAVE_ADDR, TCC_REG_SPM_ChipID1);
   chipId[2] = napa_i2c_read_reg(NAPA_I2C_SLAVE_ADDR, TCC_REG_SPM_ChipID2);
   chipId[3] = napa_i2c_read_reg(NAPA_I2C_SLAVE_ADDR, TCC_REG_SPM_RevisionID);

   /** 2. Read Power Mode Status Register */
   printk("%s() line %d 2. Read Power Mode Status Register\n", __func__, __LINE__);
   regVal = napa_i2c_read_reg(NAPA_I2C_SLAVE_ADDR, TCC_REG_SPM_PSR);

   printk("%s() line %d regVal = %d\n", __func__, __LINE__, regVal);

   if(regVal != SPM_POWER_STATE_SLEEP)
   {
      // Napa sleep request.
      napa_mutex_lock(NAPA_MUTEX);
      napa_i2c_write_reg(NAPA_I2C_SLAVE_ADDR, TCC_REG_SPM_MSG_FROM_HOST, TOFE_COMMAND_NO_COMMAND);

      data = 0;
      napa_i2c_write_ahb_reg32(SPM_PWR_CTRL, data);
      napa_mutex_release(NAPA_MUTEX);

      spmReadError = 0;
      while ((++spmReadError<SPM_READ_ERROR_MAX)&&
             (napa_i2c_read_reg(NAPA_I2C_SLAVE_ADDR, TCC_REG_SPM_PSR) != SPM_POWER_STATE_SLEEP));

      if (spmReadError == SPM_READ_ERROR_MAX)
      {
         // Force chip RESET
         napa_i2c_write_reg(NAPA_I2C_SLAVE_ADDR, TCC_REG_SPM_SOFT_RESETS, 1);
         napa_i2c_write_reg(NAPA_I2C_SLAVE_ADDR, TCC_REG_SPM_SOFT_RESETS, 2);

         // Comm interface will be lost at reset
         napa_i2c_write(NAPA_I2C_SLAVE_ADDR, TCC_REG_SPM_SPI_I2C_MODE, TCC_HOST_IF_I2C_MODE);

         if (napa_i2c_read_reg(NAPA_I2C_SLAVE_ADDR, TCC_REG_SPM_PSR) != SPM_POWER_STATE_SLEEP)
         {
            printk(KERN_ERR "ERROR: Napa sleep request failure.\n");
         }
      }

   }

   /** 3. Write WAKEUP */

   // Napa request wakeup
   napa_mutex_lock(NAPA_MUTEX);
   napa_i2c_write_reg(NAPA_I2C_SLAVE_ADDR, TCC_REG_SPM_MSG_FROM_HOST, TOFE_COMMAND_NO_COMMAND);

   napa_i2c_write_reg(NAPA_I2C_SLAVE_ADDR, TCC_REG_SPM_Request_from_Host, TCC_SPM_REQUEST_FROM_HOST_WAKE_REQUEST);
   napa_mutex_release(NAPA_MUTEX);

   // Napa check wakeup
   count = 2;
   while ((napa_i2c_read_reg(NAPA_I2C_SLAVE_ADDR, TCC_REG_SPM_PSR) != SPM_POWER_STATE_ACTIVE) && count--);

   dataByte = napa_i2c_read_reg(NAPA_I2C_SLAVE_ADDR, TCC_REG_SPM_PSR);
   if (dataByte != SPM_POWER_STATE_ACTIVE)
   {
      printk(KERN_ERR "ERROR: Napa chip wake up failed.");
      return NAPA_INIT_ERROR;
   }
   else
   {
      printk("Napa chip wake up succeeded/n");
   }

   /* OTP is not programmed, so override with SW setting */
   alfo_ctrl = 0x16;
   if (alfo_ctrl) 
   {
      napa_i2c_write_ahb_reg32(COMMON_FLL_CTRL0, 0xe0000002);
   }

   napa_i2c_write_ahb_reg32(COMMON_FLL_LPF_CTRL2, 0x01001007);

#if BRING_UP_WORKAROUND
   if (alfo_ctrl)
   {
      /* If SW ALFO trimming, enable FLL from SW else SPM will power it up */
      napa_i2c_write_ahb_reg32(COMMON_FLL_CTRL0, 0x00000001);
   }
#endif

   /* Set the clock dividers and clock values (as per SOC team's request) */
   napa_i2c_write_ahb_reg32(COMMON_SYS_HCLK_CTRL, 0xF01);

   /* Turn off clocks that are not needed, and enable needed clocks */
   /* For FBGA parts */
   napa_i2c_write_ahb_reg32(COMMON_CLOCK_ENABLE, 0x4387F);

#if BRING_UP_WORKAROUND
   if (alfo_ctrl) 
   {
      // SW over-ride for ALFO.
      napa_i2c_write_reg(NAPA_I2C_SLAVE_ADDR, TCC_REG_SPM_ALFO_CTRL, alfo_ctrl);
   }else
   {
      // FLL powered up by SPM and ALFO trimming bits used from OTP
   }
#endif

   // Napa wait for flllock

   // Wait for FLL to lock before doing further operation.
   napa_i2c_read_ahb_reg32(COMMON_CLOCK_ENABLE, &reg);

   reg &= (1 << 18);
   if (reg == 0)
   {
      do
      {
         napa_i2c_read_ahb_reg32(COMMON_FLL_TEST_CTRL1, &reg);
         reg &= (1 << 28);
      } while (reg == 0);

      napa_i2c_read_ahb_reg32(COMMON_CLOCK_ENABLE, &reg);
      napa_i2c_write_ahb_reg32(COMMON_CLOCK_ENABLE, reg | (0x01 << 18));
   }

   napa_i2c_read_ahb_reg32(TCH_VERSION_REGISTER, &tch_version);
   printk(KERN_ERR "INFO: Napa Touch block version number  %08X", tch_version);

   /* Un-remap */
   napa_i2c_read_ahb_reg32(COMMON_ARM_REMAP_ADDR, &remap);
   if (remap < 0) 
   {
      printk(KERN_ERR "ERROR: Napa common blob query failure.\n");
      return(NAPA_INIT_ERROR);
   }
   remap &= ~2U; // Turn off ARM RAM1 remap bit.
   if (napa_i2c_write_ahb_reg32(COMMON_ARM_REMAP_ADDR, remap) < 0) 
   {
      printk(KERN_ERR "ERROR: Napa common blob update failure.\n");
      return(NAPA_INIT_ERROR);
   }

   /* load firmware */
   firmware_ok = 0;
   if (napa_download() == NAPA_INIT_SUCCESS) 
   {
      printk(KERN_INFO "Napa downloaded and booted.\n");
      firmware_ok = 1;
   }
   else
   {
      printk(KERN_ERR "ERROR: Napa downloaded and boot failed.\n");
   }

   /** 4. Release WAKE up request so chip can go into power save mode */
   napa_mutex_lock(NAPA_MUTEX);
   napa_i2c_write_reg(NAPA_I2C_SLAVE_ADDR,TCC_REG_SPM_MSG_FROM_HOST,TOFE_COMMAND_NO_COMMAND);
   napa_i2c_write_reg(NAPA_I2C_SLAVE_ADDR,TCC_REG_SPM_Request_from_Host,TCC_SPM_REQUEST_FROM_HOST_RELEASE_WAKE_REQUEST);
   napa_mutex_release(NAPA_MUTEX);

   printk("%s() LOOKING GOOD! %d\n", __func__, __LINE__);

   return NAPA_INIT_SUCCESS;
}  /* napa_init */

int napa_download(void)
{
   unsigned int remap = 0;
   int ret;

   unsigned int   vect_size = 100;
   unsigned int   code_size  = 100;
   unsigned int   data_size  = 100;

   /* Hold ARM in RESET before we reload firmware */
   ret = napa_i2c_write_reg(NAPA_I2C_SLAVE_ADDR, TCC_REG_SPM_SOFT_RESETS, 0x02);
   printk("%s() line %d ret: %d\n", __func__, __LINE__, ret);

#if BOOT_FROM_RAM
   if (napa_i2c_read_ahb_reg32(COMMON_ARM_REMAP_ADDR, &remap) < 0) 
   {
      printk(KERN_ERR "ERROR: Common blob query failure.\n");
      return NAPA_INIT_ERROR;
   }
   remap |= 1; // Turn on boot from RAM remap bit.
   if (napa_i2c_write_ahb_reg32(COMMON_ARM_REMAP_ADDR, remap) < 0) 
   {
      printk(KERN_ERR "ERROR: Common blob update failure.\n");
      return NAPA_INIT_ERROR;
   }

   /* Load vectors image */
   ret = napa_i2c_write_ahb(VECTORS_OFFSET, g_vector_size, gp_vector);
   printk("%s() napa_i2c_write_ahb() wrote vector, ret = %d\n", __func__, ret);
#endif

#if FPGA_BUILD || BOOT_FROM_RAM
   /* Load ROM code image */
   /* ROM_OFFSET is 0 for FPGA ROM load and ARM RAM0 (0x10000000) for RAM load */
   ret = napa_i2c_write_ahb(ROM_OFFSET, g_code_size, gp_code);
   printk("%s() napa_i2c_write_ahb() wrote firwmare, ret = %d\n", __func__, ret);
#endif

   /* load ram image from binary file */
   if(1) 
   {

#if 0
      /* TODO: TBD - Channel Setup */
      command_channel = new CTOFEChannel(ram_image->GetChannelConfig(TOFE_CHANNEL_ID_COMMAND));
      touch_channel = new CTOFEChannel(ram_image->GetChannelConfig(TOFE_CHANNEL_ID_TOUCH));
      response_channel = new CTOFEChannel(ram_image->GetChannelConfig(TOFE_CHANNEL_ID_RESPONSE));
      log_channel = new CTOFEChannel(ram_image->GetChannelConfig(TOFE_CHANNEL_ID_LOG));
      record_channel = new CTOFEChannel(ram_image->GetChannelConfig(TOFE_CHANNEL_ID_RECORD));
#endif

#if 0
      /* TODO: TBD - Init INT Handler */
      napa_init_int_handler(ram_image->GetConfig(TOFE_TOC_INDEX_TOFE));
#endif

      ret = napa_i2c_write_ahb(DATA_OFFSET, g_data_size, gp_data);
      printk("%s() napa_i2c_write_ahb() wrote data, ret = %d\n", __func__, ret);

      /* Write ARM_SW_RESET bit to 0 in SPM_SOFT_RESETS register to get ARM out of reset
       * Clear PIN_RESET in SPM_STICKY_BITS register so microcode jumps to ARM reset vector
       */
      ret = napa_i2c_write_reg(NAPA_I2C_SLAVE_ADDR,TCC_REG_SPM_SOFT_RESETS, 0x00);
      printk("%s() line %d ret: %d\n", __func__, __LINE__, ret);

      ret = napa_i2c_write_ahb_reg32(SPM_STICKY_BITS, 0x02);
      printk("%s() line %d ret: %d\n", __func__, __LINE__, ret);

      /* We need to wait until firmware is running so we can proceed to send a command
       * We will sleep fro couple of seconds for now. In future we need to wait for
       * message form firmware to inform that the boot is complete
       */
      return (napa_wait_for_fw_ready());

   }
   else
   {

#ifdef LOAD_N_RUN
      napa_i2c_write_reg(NAPA_I2C_SLAVE_ADDR,TCC_REG_SPM_SOFT_RESETS, 0x00);
      napa_i2c_write_ahb_reg32(SPM_STICKY_BITS, 0x02)
      return (NAPA_INIT_SUCCESS);
#else
      printk(KERN_ERR "ERROR: Napa images not loaded.\n");
      return(NAPA_INIT_ERROR);
#endif
   }

   return -1;

}

int napa_wait_for_fw_ready(void)
{
   int count = 5000;
   unsigned char ready;

   /*
    * IMPORTANT
    * This function looks for specific message from FW.  If FW writes any other
    * message to host, this will fail.  Hence FW must only report error messages
    * after FW READY has been set.
    */

   ready = napa_i2c_read_reg(NAPA_I2C_SLAVE_ADDR,TCC_REG_SPM_MSG_TO_HOST);
   printk(KERN_INFO "INFO: Downloading firmware and waiting for firmware ready\n");
   while ((ready != TOFE_MESSAGE_FW_READY) && count--) 
   {
      printk(KERN_INFO "INFO: Waiting for -firmware ready- 0x%0x\n", ready);
      ready = napa_i2c_read_reg(NAPA_I2C_SLAVE_ADDR,TCC_REG_SPM_MSG_TO_HOST);
      printk("%s() line %d ready = %d\n", __func__, __LINE__, ready);
   }

   if (count <= 0) 
   {
      printk(KERN_ERR "ERROR: Failed to communicate with Napa FW. Error: 0x%x", ready);
      return NAPA_INIT_ERROR;
   }

   printk("%s() returning SUCCESS!\n", __func__);
   return(NAPA_INIT_SUCCESS);
}

int napa_mutex_lock(int mutex)
{
    /* TODO: Put mutex call here. */
    mutex_lock(&p_bcmtsc_i2c->i2c_mutex);
    return 0;
}

int napa_mutex_release(int mutex)
{
    /* TODO: Put mutex call here. */
    mutex_unlock(&p_bcmtsc_i2c->i2c_mutex);
    return 0;
}

void napa_sleep_ms(int ms)
{
    /* TODO: Put sleep call here. */
    msleep(ms);
}

int napa_i2c_read(int slave_addr, int len, unsigned char *buffer)
{
    int count=0;

    /* TODO: Put I2C read call here. */
   struct i2c_client *p_i2c_client;

   if (slave_addr == gp_i2c_client_spm->addr)
      p_i2c_client = gp_i2c_client_spm;
   else if (slave_addr == gp_i2c_client_ahb->addr)
      p_i2c_client = gp_i2c_client_ahb;
   else
   {
      printk(KERN_ERR "%s() Invalid slave address 0x%x\n",
             __func__, slave_addr);
      return -1;
   }

//   count = i2c_master_send(p_i2c_client, buffer, len);
   count = i2c_master_recv(p_i2c_client, buffer, len);

   if (count != len)
   {
      printk(KERN_ERR "%s() Unable to send i2c slave 0x%x count %d\n",
             __func__, p_i2c_client->addr, count);
   }

    return count; // Number of bytes read
}

int napa_i2c_write(int slave_addr, int length, unsigned char *buffer)
{
    int count=0;

    // TODO: Put I2C write call here.
   struct i2c_client *p_i2c_client;

   if (slave_addr == gp_i2c_client_spm->addr)
   {
      p_i2c_client = gp_i2c_client_spm;
   }
   else if (slave_addr == gp_i2c_client_ahb->addr)
   {
      p_i2c_client = gp_i2c_client_ahb;
   }
   else
   {
      printk(KERN_ERR "%s() Invalid slave address 0x%x\n",
             __func__, slave_addr);
      return -1;
   }
   printk("%s() sending to slave 0x%x\n", __func__, p_i2c_client->addr);

   count = i2c_master_send(p_i2c_client, buffer, length);

   printk("%s() i2c_master_send() returned %d\n", __func__, count);

   if (count != length)
   {
      printk(KERN_ERR "%s() Unable to send i2c slave 0x%x count %d\n",
             __func__, p_i2c_client->addr, count);
   }

   return count;  // Number of bytes written
}

unsigned char napa_i2c_read_reg(int slave_addr, int reg)
{
   int count=0;
   unsigned char buffer[2];

   printk("%s() reading register %d\n", __func__, reg);
   // Write register that we want to read
   buffer[0] = reg;
   count = napa_i2c_write(slave_addr, 1, buffer);

   // Read register
   count = napa_i2c_read(slave_addr, 1, buffer);

   return buffer[0];
}

int napa_i2c_write_reg(int slave_addr, int reg, unsigned char data)
{
   int count=0;
   unsigned char buffer[2];

   buffer[0] = (unsigned char)reg;
   buffer[1] = data;

   count = napa_i2c_write(slave_addr, 2, buffer);

   if (1)
   {
      printk("%s() count = %d\n", __func__, count);
   }

   return count;  // Number of bytes written
}

int napa_i2c_write_ahb( int ahb_addr, int len, void *data)     // port note, WriteAHBI2C ahb_addr was DWORD
{
   int count;
   int length = 8;
   unsigned char *I2CWritePkt;

   //allocate buffer for the num_bytes
   //ATT: we need one byte more for the offset
   I2CWritePkt = kzalloc(len + 1, GFP_KERNEL);

   //set the address
   I2Cheader[1] = (unsigned char)(ahb_addr & 0xFF);
   I2Cheader[2] = (unsigned char)((ahb_addr & 0xFF00) >> 8);
   I2Cheader[3] = (unsigned char)((ahb_addr & 0xFF0000) >> 16);
   I2Cheader[4] = (unsigned char)((ahb_addr & 0xFF000000) >> 24);
   //set the length
   I2Cheader[5] = (unsigned char)(len & 0xFF);
   I2Cheader[6] = (unsigned char)((len & 0xFF00) >> 8);
   //set the command
   I2Cheader[7] = (unsigned char)MahbWrite;

   count = (int)sizeof(I2Cheader);
   printk("%s() ahb_addr %d len %d before count = %d\n", __func__, ahb_addr, len, count);

   if (0)
   {
      printk("%s() leaving early %d\n", __func__, __LINE__);
      return 0;
   }

   count = napa_i2c_write(NAPA_I2C_AHB_SLAVE_ADDR, length, (unsigned char *)I2Cheader);
   printk("%s() after count = %d\n", count);

   if (count < 0)
   {
      printk(KERN_ERR "%s() error,  napa_i2c_write failed, count = %d\n", __func__, count);
      return count;
   }

   if (0)
   {
      printk("%s() leaving early %d\n", __func__, __LINE__);
      return 0;
   }

   /* Should we wait here also a bit? */

   I2CWritePkt[0] = I2C_REG_WFIFO_DATA;    //select the write FiFo
   /* Prepare the dat packet, select the write FiFo. */
   memcpy(I2CWritePkt + 1, data, len);

   /* Send the data (payload). */
   count = napa_i2c_write(NAPA_I2C_AHB_SLAVE_ADDR, (int)len+1, (unsigned char *)I2CWritePkt);

   if (count < 0) 
   {
      printk(KERN_ERR "ERROR: Napa AHB write failed, count = %d\n", count);
   }
   else if (count == 0) 
   {
      printk(KERN_ERR "ERROR: Napa AHB write failed - no bytes written, do you have right slave address\n");
   }
   else if (count != (len + 1)) 
   {
      printk(KERN_ERR "ERROR: Napa AHB write failed - only a partial number of bytes written, (%d)instead of (%d)\n",
      count,len+1);
   }
   else
      printk("%s() line %d ok!\n", __func__, __LINE__);

   kfree(I2CWritePkt);

   return count;
}

int napa_i2c_read_ahb(int ahb_addr, int len, void *data)     /* port note, ReadAHBI2C ahb_addr was DWORD */
{
   int count;
   unsigned char I2CreadPkt;

   //set the address
   I2Cheader[1] = (unsigned char)(ahb_addr & 0xFF);
   I2Cheader[2] = (unsigned char)((ahb_addr & 0xFF00) >> 8);
   I2Cheader[3] = (unsigned char)((ahb_addr & 0xFF0000) >> 16);
   I2Cheader[4] = (unsigned char)((ahb_addr & 0xFF000000) >> 24);
   //set the length
   I2Cheader[5] = (unsigned char)(len & 0xFF);
   I2Cheader[6] = (unsigned char)((len & 0xFF00) >> 8);
   //set the command
   I2Cheader[7] = (unsigned char)MahbRead;

   count = napa_i2c_write(NAPA_I2C_AHB_SLAVE_ADDR, (int)sizeof(I2Cheader), (unsigned char *)I2Cheader);

   /* Check the FiFo if result is available and we can start reading. */
   int timeOut = 0;
   unsigned char status;
   int read_counter = 0;

   while (1)
   {
      status = napa_i2c_read_reg(NAPA_I2C_AHB_SLAVE_ADDR, I2C_REG_STATUS);
      printk("%s() line %d status %d\n", __func__, __LINE__, status);

      /* PPTEST CLEAN UP NEEDED HERE! */
      if (status == 1)
         break;

      napa_sleep_ms(1);
      timeOut++;
      if (timeOut > 1000)
      {
         printk(KERN_ERR "ERROR: Napa i2c ahb read timeout\n");
         return 0;
      }
      read_counter++;

      if (read_counter > 3)
         break;
   }                 //I2C complete ?

   printk("%s() line %d leaving while() loop\n", __func__, __LINE__);


   I2CreadPkt = I2C_REG_RFIFO_DATA;

   count = napa_i2c_write(NAPA_I2C_AHB_SLAVE_ADDR,
                          (int)sizeof(I2CreadPkt),
                          (unsigned char *)&I2CreadPkt);

   count = napa_i2c_read(NAPA_I2C_AHB_SLAVE_ADDR, len, (unsigned char *)data);
   //count = napa_i2c_read_reg(NAPA_I2C_AHB_SLAVE_ADDR, len, (unsigned char *)data);
   //int reg_val;

   //reg_val = napa_i2c_read_reg(NAPA_I2C_AHB_SLAVE_ADDR, I2C_REG_RFIFO_DATA);

   printk("%s() line %d count %d data %d\n", __func__, __LINE__, count, data);
   return count;
}

int napa_i2c_write_ahb_reg32(int ahb_addr, int data)
{
   int len;
   int count;

   len = sizeof(int);

   count = napa_i2c_write_ahb(ahb_addr, len, (unsigned char *)&data);

   return count;
}

int napa_i2c_read_ahb_reg32( int ahb_addr, void *data)
{
   int len;
   int count;

   len = sizeof(int);

   count = napa_i2c_read_ahb(ahb_addr, len, (unsigned char *)data);
   printk("%s() returning %d read: %d\n", __func__, count, (unsigned char *)data);

   return count;
}

static
int32_t
bcmtsc_firmware_download(struct bcmtsc_i2c *p_device, uint16_t len, const uint8_t *p_data, uint32_t addr)
{
   printk(KERN_INFO "Downloading %d bytes from 0x%x to address 0x%x\n", len, p_data, addr);

   if (addr == VECTORS_OFFSET)
   {
      g_vector_size = len;
      memcpy(gp_vector, p_data, len);
   }

   if (addr == ROM_OFFSET)
   {
      g_code_size = len;
      memcpy(gp_code, p_data, len);
   }

   if (addr == DATA_OFFSET)
   {
      g_data_size = len;
      memcpy(gp_data, p_data, len);
   }
   return 0;
}

static
int32_t
bcmtsc_i2c_firmware_load(struct bcmtsc_i2c *p_bcmtsc_i2c, const char *p_name, int addr)
{
   const struct firmware *fw;
   int32_t err;

   printk(KERN_INFO "%s calling request_firmware for %s\n", __func__, p_name);

   /** call kernel to start firmware load **/
   /* request_firmware(const struct firmware **fw,
    *                  const char *name,
    *                  struct device *device);
    */
   err = request_firmware(&fw, p_name, &p_bcmtsc_i2c->p_i2c_client1->dev);
   if (err)
   {
      printk(KERN_ERR "%s: Firmware request failed (%d)\n", __func__, err);
      return(err);
   }

   /** download to chip **/
   err = bcmtsc_firmware_download(p_bcmtsc_i2c, fw->size, fw->data, addr);

   /** free kernel structure */
   release_firmware(fw);

   return(err);
}

//int bcmtsc_read_regs(struct i2c_client *p_i2c_client, u8 *p_reg, u8 *p_val, int length)
int bcmtsc_write_regs(struct bcmtsc_i2c *p_bcmtsc_i2c,
                      int slave_addr,
                      u8 *p_reg,
                      u8 *p_val,
                      int length)
{
   u8  buf[10];
   int i;
   int ret = 0;
   struct i2c_client *p_i2c_client;

   if (slave_addr == p_bcmtsc_i2c->p_i2c_client->addr)
      p_i2c_client = p_bcmtsc_i2c->p_i2c_client;
   else if (slave_addr == p_bcmtsc_i2c->p_i2c_client1->addr)
      p_i2c_client = p_bcmtsc_i2c->p_i2c_client1;
   else
   {
      printk(KERN_ERR "%s() Invalid slave address 0x%x\n",
             __func__, slave_addr);
      return -1;
   }

   for (i = 0; i < length; i++)
   {
      buf[0] = p_reg[i];
      /* Sending to the slave is an I2C write operation. */
      ret = i2c_master_send(p_i2c_client, buf, 1);

      if (ret != 1)
      {
         printk(KERN_ERR "%s() Unable to send i2c slave 0x%x reg 0x%x, ret %d\n",
                __func__, p_i2c_client->addr, p_reg[i], ret);
         return -1;
      }

   }
   return 0;
}



int bcmtsc_read_regs(struct bcmtsc_i2c *p_bcmtsc_i2c,
                     int slave_addr,
                     u8 *p_reg,
                     u8 *p_val,
                     int length)
{
   u8  buf[10];
   int i;
   int ret = 0;
   struct i2c_client *p_i2c_client;

   if (slave_addr == p_bcmtsc_i2c->p_i2c_client->addr)
      p_i2c_client = p_bcmtsc_i2c->p_i2c_client;
   else if (slave_addr == p_bcmtsc_i2c->p_i2c_client1->addr)
      p_i2c_client = p_bcmtsc_i2c->p_i2c_client1;
   else
   {
      printk(KERN_ERR "%s() Invalid slave address 0x%x\n",
             __func__, slave_addr);
      return -1;
   }

   for (i = 0; i < length; i++)
   {
      buf[0] = p_reg[i];
      /* Sending to the slave is an I2C write operation. */
      ret = i2c_master_send(p_i2c_client, buf, 1);

      if (ret != 1)
      {
         printk(KERN_ERR "%s() Unable to send i2c slave 0x%x reg 0x%x, ret %d\n",
                __func__, p_i2c_client->addr, p_reg[i], ret);
         return -1;
      }

      /* Receiving from the slave still requires the sending of a I2C message
         but is an I2C read operation.                                         */
      ret = i2c_master_recv(p_i2c_client, buf, 1);
      if (ret != 1)
      {
         printk(KERN_ERR "%s() Unable to recv i2c slave 0x%x reg 0x%x, ret %d\n",
                __func__, p_i2c_client->addr, p_reg[i], ret);
         return -2;
      }
      else
      {
         printk("%s() slave 0x%x Reg 0x%x val %02x\n",
                __func__, p_i2c_client->addr, p_reg[i], buf[0]);
      }

      p_val[i] = buf[0];
   }
   return 0;
}

int bcmtsc_get_chip_info(struct bcmtsc_i2c *p_bcmtsc_i2c)
{
   int ret    = 0;
   int length = 4;
   u8 chip_regs[4] =
   {
      BCM_TSC_CHIP_ID0,
      BCM_TSC_CHIP_ID1,
      BCM_TSC_CHIP_ID2,
      BCM_TSC_CHIP_REV,
   };
   u8 chip_vals[4];

   ret = bcmtsc_read_regs(p_bcmtsc_i2c,
                          BCM_TSC_SPM_SLAVE,
                          &chip_regs,
                          &chip_vals,
                          length);
   if (ret < 0)
   {
      printk("%s() failed to retrieve chip info\n", __func__);
   }
   else
   {
      printk("%s() bcm915500 chip info: %2x%2x%2x revision: %d\n",
             __func__, chip_vals[0], chip_vals[3], chip_vals[2], chip_vals[1]);
   }

   length = 1;
   ret = bcmtsc_write_regs(p_bcmtsc_i2c,
                           BCM_TSC_AHB_SLAVE,
                           &chip_regs,
                           &chip_vals,
                           length);

   return ret;
}

static struct input_dev *allocate_Input_Dev(void)
{
   int ret;
   struct input_dev *pInputDev = NULL;

   pInputDev = input_allocate_device();
   if (pInputDev == NULL)
   {
      printk("%s() Failed to allocate input device\n", __func__);
      return NULL;
   }

   pInputDev->name = "BCM915500 Touch Screen";
   pInputDev->phys = "I2C";
   pInputDev->id.bustype = BUS_I2C;
   pInputDev->id.vendor = 0x0EEF;
   pInputDev->id.product = 0x0020;
   pInputDev->id.version = 0x0000;

   set_bit(EV_ABS, pInputDev->evbit);

   input_set_abs_params(pInputDev, ABS_MT_POSITION_X, 0, 2047, 0, 0);
   input_set_abs_params(pInputDev, ABS_MT_POSITION_Y, 0, 2047, 0, 0);
   input_set_abs_params(pInputDev, ABS_MT_TOUCH_MAJOR, 0, 255, 0, 0);
   input_set_abs_params(pInputDev, ABS_MT_WIDTH_MAJOR, 0, 255, 0, 0);
   input_set_abs_params(pInputDev, ABS_MT_TRACKING_ID, 0, 10, 0, 0);

   ret = input_register_device(pInputDev);
   if (ret)
   {
      printk("%s() Unable to register input device\n", __func__);
      input_free_device(pInputDev);
      pInputDev = NULL;
      return NULL;
   }

   return pInputDev;
}

int bcmtsc_init_hw(struct bcmtsc_i2c *p_bcmtsc_i2c)
{
   int ret;
   int length = 1;

   if (1)
   {
      ret = napa_init(p_bcmtsc_i2c->p_i2c_client,
                      p_bcmtsc_i2c->p_i2c_client1);
      if (ret != 0)
      {
         printk(KERN_ERR "bcm915500_i2c_ts.c %s(): bcmtsc_write_regs() failed, ret %d\n",
                __func__, ret);
         return -1;
      }
      else
      {
        printk("%s() succeeded\n", __func__);
      }
      return 0;
   }

   /* Write 0 to reg 68. */
   u8 reg = 68;
   u8 val = 0;

   ret = bcmtsc_write_regs(p_bcmtsc_i2c,
                           BCM_TSC_SPM_SLAVE,
                           &reg,
                           &val,
                           length);

   if (ret != 0)
   {
      printk(KERN_ERR "bcm915500_i2c_ts.c %s(): bcmtsc_write_regs() failed, ret %d\n",
              __func__, ret);
      return -1;
   }
   else
   {
      printk("%s() succeeded\n", __func__);
   }
   return 0;
}


static struct platform_device bcmtsc_core_device =
{
   .name              = BCM915500_TSC_NAME,
   .id                = -1,
   .dev.platform_data = NULL,
};

static int bcmtsc_i2c_probe(struct i2c_client *p_i2c_client,
             const struct i2c_device_id *id)
{
   int ret = 0;
   struct bcm915500_platform_data *pdata;
   struct i2c_client  *p_new_i2c_client;
   struct i2c_adapter *p_i2c_adapter;
   //struct bcmtsc_i2c *p_bcmtsc_i2c;

   if (p_bcmtsc_i2c != NULL)
   {
      printk(KERN_INFO "PPTEST %s already called\n", __func__);
      return 0;
   }

   pdata = (struct bcm915500_platform_data *)p_i2c_client->dev.platform_data;

   printk(KERN_INFO "PPTEST %s called\n", __func__);

   if (pdata == NULL)
   {
      printk("%s() error, pdata == NULL\n", __func__);
      return -1;
   }

#if 1
   p_bcmtsc_i2c = kzalloc(sizeof(struct bcmtsc_i2c), GFP_KERNEL);
   if (p_bcmtsc_i2c == NULL)
   {
      printk(KERN_ERR "%s: failed to alloc mem.\n", __func__);
      ret = -ENOMEM;
      goto err;
   }

   gp_code   = kzalloc(40000, GFP_KERNEL);
   gp_vector = kzalloc(5000, GFP_KERNEL);
   gp_data   = kzalloc(10000, GFP_KERNEL);

   if (gp_code == NULL || gp_vector == NULL || gp_data == NULL)
   {
      printk("ERROR: cannot alloc\n");
      ret = -ENOMEM;
      goto err;
   }

   //i2c_set_clientdata(p_i2c_client, bcmpmu);
   //bcmpmu->dev = &p_i2c_client->dev;
   p_bcmtsc_i2c->p_i2c_client = p_i2c_client;

   /* Configure the second I2C slave address. */
   p_i2c_adapter = i2c_get_adapter(pdata->i2c_adapter_id);

   if (p_i2c_adapter == NULL)
   {
      printk("%s() p_i2c_adapter == NULL, adapter_id: 0x%x\n",
             __func__, pdata->i2c_adapter_id);
      return -1;
   }
   else
   {
      printk("%s() p_i2c_adapter != NULL, adapter_id: 0x%x\n",
             __func__, pdata->i2c_adapter_id);
   }

   if (0)
   {
      printk("%s() no error, pdata != NULL\n", __func__);
      return -1;
   }

   /*
    * i2c_new_device(struct i2c_adapter *adap,
    *                struct i2c_board_info const *info);
    */
   p_new_i2c_client = i2c_new_device(p_i2c_adapter,
//                                     pdata->p_i2c_board_info_map1);
                                     &bcm915500_i2c_boardinfo);

   if (p_new_i2c_client == NULL)
   {
      printk("%s() p_new_i2c_client == NULL, slave address: 0x%x\n",
             __func__, bcm915500_i2c_boardinfo.addr);
      return -1;
   }
   else
   {
      printk("%s() p_new_i2c_client != NULL, slave address: 0x%x\n",
             __func__, bcm915500_i2c_boardinfo.addr);
   }

   p_bcmtsc_i2c->p_i2c_client1 = p_new_i2c_client;
   mutex_init(&p_bcmtsc_i2c->i2c_mutex);
   
   platform_device_register(&bcmtsc_core_device);

   ret = bcmtsc_i2c_firmware_load(p_bcmtsc_i2c, BCM915500_TSC_FW_VECT,
                                  VECTORS_OFFSET); /* Vectors */
   printk("%s() bcmtsc_i2c_firmware_load() returned %d\n", __func__, ret);

   if (ret != 0)
   {
      printk("Vector download failed\n");
      return -1;
   }

   ret = bcmtsc_i2c_firmware_load(p_bcmtsc_i2c, BCM915500_TSC_FW_DATA,
                                  DATA_OFFSET); /* Data */
   if (ret != 0)
   {
      printk("Data download failed\n");
      return -1;
   }
   ret = bcmtsc_i2c_firmware_load(p_bcmtsc_i2c, BCM915500_TSC_FW_CODE,
                                  ROM_OFFSET); /* Firmware */
   if (ret != 0)
   {
      printk("Firmware download failed\n");
      return -1;
   }

   printk("%s() downloads successful, vector size: %d, data size: %d firmware size: %d\n",
          __func__, g_vector_size, g_data_size, g_code_size);


   if ((ret = bcmtsc_init_hw(p_bcmtsc_i2c)) != 0)
   {
      printk("%s() bcmtsc_init_hw() failed, rc = %d\n", __func__, ret);
      //return -1;
   }

   printk(KERN_INFO "%s() exiting, SUCCESS!\n", __func__);
   return ret;

err:
   //kfree(bcmpmu->accinfo);
   //kfree(bcmpmu);
   return ret;
#endif
}

static int bcmtsc_i2c_remove(struct i2c_client *p_i2c_client)
{
   //struct bcmpmu *bcmpmu = i2c_get_clientdata(p_i2c_client);

   platform_device_unregister(&bcmtsc_core_device);
   //kfree(bcmpmu->accinfo);
   //kfree(bcmpmu);

   return 0;
}

static const struct i2c_device_id bcmtsc_i2c_id[] = {
       { BCM915500_TSC_NAME, 0 },
       { }
};
MODULE_DEVICE_TABLE(i2c, bcmtsc_i2c_id);


static struct i2c_driver bcmtsc_i2c_driver = {
   .driver = {
         .name = BCM915500_TSC_NAME,
         .owner = THIS_MODULE,
   },
   .probe = bcmtsc_i2c_probe,
   .remove = bcmtsc_i2c_remove,
   .id_table = bcmtsc_i2c_id,
};

static int __init bcmtsc_i2c_init(void)
{
   return i2c_add_driver(&bcmtsc_i2c_driver);
}
/* init early so consumer devices can complete system boot */
subsys_initcall(bcmtsc_i2c_init);

static void __exit bcmtsc_i2c_exit(void)
{
   i2c_del_driver(&bcmtsc_i2c_driver);
}
module_exit(bcmtsc_i2c_exit);

MODULE_DESCRIPTION("I2C support for BCM915500 Touchscreen");
MODULE_LICENSE("GPL");

