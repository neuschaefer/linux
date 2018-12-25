/* 
 * CNT touch driver based on
 * drivers/input/touchscreen/ft5x0x_ts.c
 *
 * FocalTech ft5x TouchScreen driver. 
 *
 * Copyright (c) 2010  Focal tech Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *
 *	note: only support mulititouch	Wenfs 2010-10-01
 *  for this touchscreen to work, it's slave addr must be set to 0x7e | 0x70
 */

#include <linux/i2c.h>
#include <linux/input.h>

#if defined(CONFIG_PM)
	#include <linux/pm.h>
	#include <linux/suspend.h>
	#include <linux/power/aw_pm.h>
#endif

#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/async.h>
#include <linux/hrtimer.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/uaccess.h>

#include <mach/irqs.h>
#include <mach/system.h>
#include <mach/hardware.h>
#include <mach/sys_config.h>
#include "ctp_platform_ops.h"
#include <linux/suspend.h>
#include <linux/proc_fs.h>
#include <mach/gpio.h>

#include "cyttsp.h"

#include "cypress.h"


#define I2C_MINORS 	256
#define I2C_MAJOR 	125

//suspend mode
#define SUSPEND_MODE 	0x1 //touch held in reset to reduce power consumption
#define LOCK_MODE 		0x2 //TS locked

#define PRINT_INT_INFO
#define PRINT_POINT_INFO
#define DEBUG

struct i2c_dev{
struct list_head list;	
struct i2c_adapter *adap;
struct device *dev;
};

static struct class *i2c_dev_class;
static LIST_HEAD (i2c_dev_list);
static DEFINE_SPINLOCK(i2c_dev_list_lock);

static struct i2c_client *this_client;

#ifdef PRINT_POINT_INFO 
#define print_point_info(fmt, args...)   \
        do{                              \
                pr_info(fmt, ##args);     \
        }while(0)
#else
#define print_point_info(fmt, args...)   //
#endif

#ifdef PRINT_INT_INFO 
#define print_int_info(fmt, args...)     \
        do{                              \
                pr_info(fmt, ##args);     \
        }while(0)
#else
#define print_int_info(fmt, args...)   //
#endif
///////////////////////////////////////////////
//specific tp related macro: need be configured for specific tp
//#define CTP_IRQ_NO			(gpio_int_info[0].port_num)

//#define CTP_IRQ_MODE			(NEGATIVE_EDGE)
#define CTP_IRQ_MODE			(POSITIVE_EDGE)
#define CTP_NAME			"cnt_ts"//"ft5x_ts"
#define TS_RESET_LOW_PERIOD		(10)
#define TS_INITIAL_HIGH_PERIOD		(30)
#define TS_WAKEUP_LOW_PERIOD	(20)
#define TS_WAKEUP_HIGH_PERIOD	(20)
#define TS_POLL_DELAY			(10)	/* ms delay between samples */
#define TS_POLL_PERIOD			(10)	/* ms delay between samples */
//#define SCREEN_MAX_X			(screen_max_x)
//#define SCREEN_MAX_Y			(screen_max_y)
//#define PRESS_MAX			(255)


static void* __iomem gpio_addr = NULL;
static int gpio_int_hdle = 0;
static int gpio_wakeup_hdle = 0;
static int gpio_reset_hdle = 0;
static int gpio_wakeup_enable = 1;
static int gpio_reset_enable = 1;

static int gpio_power_hdle = 0;

static int screen_max_x = 0;
static int screen_max_y = 0;
static int revert_x_flag = 0;
static int revert_y_flag = 0;
static int exchange_x_y_flag = 0;

static REPORT_FINGER_INFO_T _st_finger_infos[CFG_MAX_POINT_NUM];


#define MSG2133_DEBUG
#ifdef MSG2133_DEBUG
#define MSG2133_DBG(format, ...)	printk(KERN_INFO "MSG2133 " format "\n", ## __VA_ARGS__)
#else
#define MSG2133_DBG(format, ...)
#endif


#if MSG2133_UPDATE

struct class *firmware_class;
struct device *firmware_cmd_dev;
static int tp_type = TP_HUANGZE;
static  unsigned char g_dwiic_info_data[1024];
static int FwDataCnt;
static struct fw_version fw_v;
//static unsigned char temp[94][1024] = {};

#define FW_SIZE (33*1024)
static unsigned char fw_buffer[FW_SIZE];

static void msg2133_reset(void)
{
	MSG2133_DBG("..........msg2133_reset.........\n");

	if(gpio_reset_enable){
		pr_info("%s. \n", __func__);
		if(EGPIO_SUCCESS != gpio_write_one_pin_value(gpio_reset_hdle, 0, "ctp_reset")){
			pr_info("%s: err when operate gpio. \n", __func__);
		}
		msleep(10);
		if(EGPIO_SUCCESS != gpio_write_one_pin_value(gpio_reset_hdle, 1, "ctp_reset")){
			pr_info("%s: err when operate gpio. \n", __func__);
		}
		msleep(200);
	} else
		pr_err("%s reset not enabled!\n", __func__);
}



static bool msg2133_i2c_read(char *pbt_buf, int dw_lenth)
{
    int ret;
    MSG2133_DBG("The msg_i2c_client->addr=0x%x\n",this_client->addr);
    ret = i2c_master_recv(this_client, pbt_buf, dw_lenth);

    if(ret <= 0){
        MSG2133_DBG("msg_i2c_read_interface error\n");
        return false;
    }

    return true;
}

static bool msg2133_i2c_write(char *pbt_buf, int dw_lenth)
{
    int ret;
	MSG2133_DBG("hhh %s\n", __func__);
    MSG2133_DBG("The msg_i2c_client->addr=0x%x\n",this_client->addr);
    ret = i2c_master_send(this_client, pbt_buf, dw_lenth);

    if(ret <= 0){
        MSG2133_DBG("msg_i2c_read_interface error\n");
        return false;
    }

    return true;
}

static void i2c_read_msg2133(unsigned char *pbt_buf, int dw_lenth)
{
	MSG2133_DBG("hhh %s\n", __func__);

    this_client->addr = MSG2133_FW_ADDR;
	i2c_master_recv(this_client, pbt_buf, dw_lenth);	//0xC5_8bit
	this_client->addr = MSG2133_TS_ADDR;
}

static void i2c_write_msg2133(unsigned char *pbt_buf, int dw_lenth)
{
	MSG2133_DBG("hhh %s\n", __func__);

	this_client->addr = MSG2133_FW_ADDR;
	i2c_master_send(this_client, pbt_buf, dw_lenth);		//0xC4_8bit
	this_client->addr = MSG2133_TS_ADDR;
}

static void i2c_read_update_msg2133(unsigned char *pbt_buf, int dw_lenth)
{	
	MSG2133_DBG("hhh %s\n", __func__);

	this_client->addr = MSG2133_FW_UPDATE_ADDR;
	i2c_master_recv(this_client, pbt_buf, dw_lenth);	//0x93_8bit
	this_client->addr = MSG2133_TS_ADDR;
}

static void i2c_write_update_msg2133(unsigned char *pbt_buf, int dw_lenth)
{	
	MSG2133_DBG("hhh %s\n", __func__);

    this_client->addr = MSG2133_FW_UPDATE_ADDR;
	i2c_master_send(this_client, pbt_buf, dw_lenth);	//0x92_8bit
	this_client->addr = MSG2133_TS_ADDR;
}

static int msg2133_get_version(struct fw_version *fw)
{
	unsigned char dbbus_tx_data[3];
	unsigned char dbbus_rx_data[5] ;
	int i;
	
	MSG2133_DBG("%s\n", __func__);
/*	
	dbbusDWIICEnterSerialDebugMode();
	dbbusDWIICStopMCU();
	dbbusDWIICIICUseBus();
	dbbusDWIICIICReshape();
    */
	MSG2133_DBG("\n");
	for ( i = 0; i < 10; i++)
	{
	dbbus_tx_data[0] = 0x53;
	dbbus_tx_data[1] = 0x00;
	dbbus_tx_data[2] = 0x74;
	msg2133_i2c_write(&dbbus_tx_data[0], 3);
	mdelay(50);
	msg2133_i2c_read(&dbbus_rx_data[0], 5);

    MSG2133_DBG("dbbus_rx_data[0] = %x\n",dbbus_rx_data[0]);
    MSG2133_DBG("dbbus_rx_data[1] = %x\n",dbbus_rx_data[1]);
    MSG2133_DBG("dbbus_rx_data[2] = %x\n",dbbus_rx_data[2]);
    MSG2133_DBG("dbbus_rx_data[3] = %x\n",dbbus_rx_data[3]);
    MSG2133_DBG("dbbus_rx_data[4] = %x\n",dbbus_rx_data[4]);

	fw->major = dbbus_rx_data[0];
	fw->minor = (dbbus_rx_data[3] << 8) + dbbus_rx_data[2];
	fw->VenderID= (dbbus_rx_data[4]);
	MSG2133_DBG("%s, major = 0x%x, minor = 0x%x\n,VenderID = 0x%x\n", __func__, fw->major, fw->minor,fw->VenderID);
	if( (fw->major & 0xff00 )== 0)
	break;
	}
	return 0;
}


void dbbusDWIICEnterSerialDebugMode(void)//Check
{
    unsigned char data[5];
	MSG2133_DBG("hhh %s\n", __func__);
    // Enter the Serial Debug Mode
    data[0] = 0x53;
    data[1] = 0x45;
    data[2] = 0x52;
    data[3] = 0x44;
    data[4] = 0x42;
    i2c_write_msg2133(data, 5);
}

void dbbusDWIICStopMCU(void)//Check
{
    unsigned char data[1];
	MSG2133_DBG("hhh %s\n", __func__);
    // Stop the MCU
    data[0] = 0x37;
    i2c_write_msg2133(data, 1);
}

void dbbusDWIICIICUseBus(void)//Check
{
    unsigned char data[1];
	MSG2133_DBG("hhh %s\n", __func__);
    // IIC Use Bus
    data[0] = 0x35;
    i2c_write_msg2133(data, 1);
}

void dbbusDWIICIICReshape(void)//Check
{
    unsigned char data[1];
	MSG2133_DBG("hhh %s\n", __func__);
    // IIC Re-shape
    data[0] = 0x71;
    i2c_write_msg2133(data, 1);
}

void dbbusDWIICIICNotUseBus(void)//Check
{
    unsigned char data[1];
	MSG2133_DBG("hhh %s\n", __func__);
    // IIC Not Use Bus
    data[0] = 0x34;
    i2c_write_msg2133(data, 1);
}

void dbbusDWIICNotStopMCU(void)//Check
{
    unsigned char data[1];
	MSG2133_DBG("hhh %s\n", __func__);
    // Not Stop the MCU
    data[0] = 0x36;
    i2c_write_msg2133(data, 1);
}

void dbbusDWIICExitSerialDebugMode(void)//Check
{
    unsigned char data[1];
	MSG2133_DBG("hhh %s\n", __func__);
    // Exit the Serial Debug Mode
    data[0] = 0x45;
    i2c_write_msg2133(data, 1);
    // Delay some interval to guard the next transaction
}

void drvISP_EntryIspMode(void)//Check
{
    unsigned char bWriteData[5] =
    {
        0x4D, 0x53, 0x54, 0x41, 0x52
    };
	MSG2133_DBG("hhh %s\n", __func__);
    i2c_write_update_msg2133(bWriteData, 5);
    msleep(10);           // delay about 10ms
}

void drvISP_WriteEnable(void)//Check
{
    unsigned char bWriteData[2] =
    {
        0x10, 0x06
    };
    unsigned char bWriteData1 = 0x12;
	MSG2133_DBG("hhh %s\n", __func__);
    i2c_write_update_msg2133(bWriteData, 2);
    i2c_write_update_msg2133(&bWriteData1, 1);
}

void drvISP_ExitIspMode(void)//Check
{
	unsigned char bWriteData = 0x24;
	MSG2133_DBG("hhh %s\n", __func__);
	i2c_write_update_msg2133(&bWriteData, 1);
}


#if 0//Old function
unsigned char drvISP_Read(unsigned char n, unsigned char *pDataToRead)    //First it needs send 0x11 to notify we want to get flash data back.
{
    unsigned char Read_cmd = 0x11;
    unsigned char i = 0;
    unsigned char dbbus_rx_data[16] = {0};
    i2c_write_update_msg2133(&Read_cmd, 1);
    //if (n == 1)
    {
        i2c_read_update_msg2133(&dbbus_rx_data[0], n + 1);

        for(i = 0; i < n; i++)
        {
            *(pDataToRead + i) = dbbus_rx_data[i + 1];
        }
    }
    //else
    {
        //     i2c_read_update_msg2133(pDataToRead, n);
    }
    return 0;
}
#endif
static unsigned char drvISP_Read(unsigned char n, unsigned char *pDataToRead) //First it needs send 0x11 to notify we want to get flash data back. //Check
{
    unsigned char Read_cmd = 0x11;
    unsigned char dbbus_rx_data[2] = {0xFF, 0xFF};
	MSG2133_DBG("hhh %s\n", __func__);
    i2c_write_update_msg2133(&Read_cmd, 1);
    msleep(10);        // delay about 1000us*****
    if ( n == 1 )
    {
        i2c_read_update_msg2133( &dbbus_rx_data[0], 2 );

        // Ideally, the obtained dbbus_rx_data[0~1] stands for the following meaning:
        //  dbbus_rx_data[0]  |  dbbus_rx_data[1]  | status
        // -------------------+--------------------+--------
        //       0x00         |       0x00         |  0x00
        // -------------------+--------------------+--------
        //       0x??         |       0x00         |  0x??
        // -------------------+--------------------+--------
        //       0x00         |       0x??         |  0x??
        //
        // Therefore, we build this field patch to return the status to *pDataToRead.
        *pDataToRead = ( ( dbbus_rx_data[0] >= dbbus_rx_data[1] ) ? \
                         dbbus_rx_data[0]  : dbbus_rx_data[1] );
    }
    else
    {
        i2c_read_update_msg2133 ( pDataToRead, n );
    }

    return 0;
}


unsigned char drvISP_ReadStatus(void)//Check
{
    unsigned char bReadData = 0;
    unsigned char bWriteData[2] =
    {
        0x10, 0x05
    };
    unsigned char bWriteData1 = 0x12;
	MSG2133_DBG("hhh %s\n", __func__);
//    msleep(1);           // delay about 100us
    i2c_write_update_msg2133(bWriteData, 2);
    msleep(1);           // delay about 100us
    drvISP_Read(1, &bReadData);
//    msleep(10);           // delay about 10ms
    i2c_write_update_msg2133(&bWriteData1, 1);
    return bReadData;
}


void drvISP_SectorErase(unsigned int addr)//This might remove
{
	unsigned char bWriteData[5] = { 0x00, 0x00, 0x00, 0x00, 0x00 };
	unsigned char  bWriteData1 = 0x12;
	MSG2133_DBG("hhh %s\n", __func__);
	MSG2133_DBG("The drvISP_ReadStatus0=%d\n", drvISP_ReadStatus());
	drvISP_WriteEnable();
	MSG2133_DBG("The drvISP_ReadStatus1=%d\n", drvISP_ReadStatus());
	bWriteData[0] = 0x10;
	bWriteData[1] = 0x50;
	i2c_write_update_msg2133(&bWriteData, 2);
	i2c_write_update_msg2133(&bWriteData1, 1);
	bWriteData[0] = 0x10;
	bWriteData[1] = 0x01;
	bWriteData[2] = 0x00;
	i2c_write_update_msg2133(bWriteData, 3);
	i2c_write_update_msg2133(&bWriteData1, 1);
	bWriteData[0] = 0x10;
	bWriteData[1] = 0x04;
	i2c_write_update_msg2133(bWriteData, 2);
	i2c_write_update_msg2133(&bWriteData1, 1); 
	while((drvISP_ReadStatus() & 0x01) == 0x01);
	
	drvISP_WriteEnable();
	bWriteData[0] = 0x10;
	bWriteData[1] = 0x20; //Sector Erase
	bWriteData[2] = (( addr >> 16) & 0xFF);
	bWriteData[3] = (( addr >> 8 ) & 0xFF);
	bWriteData[4] = ( addr & 0xFF); 
	i2c_write_update_msg2133(&bWriteData, 5);
	i2c_write_update_msg2133(&bWriteData1, 1);
	while((drvISP_ReadStatus() & 0x01) == 0x01);
}

static void drvISP_BlockErase(unsigned int addr)//This might remove
{
    unsigned char bWriteData[5] = { 0x00, 0x00, 0x00, 0x00, 0x00 };
    unsigned char bWriteData1 = 0x12;
    unsigned int timeOutCount=0;
	MSG2133_DBG("hhh %s\n", __func__);
    drvISP_WriteEnable();
    //Enable write status register
    bWriteData[0] = 0x10;
    bWriteData[1] = 0x50;
    i2c_write_update_msg2133(bWriteData, 2);
    i2c_write_update_msg2133(&bWriteData1, 1);
    //Write Status
    bWriteData[0] = 0x10;
    bWriteData[1] = 0x01;
    bWriteData[2] = 0x00;
    i2c_write_update_msg2133(bWriteData, 3);
    i2c_write_update_msg2133(&bWriteData1, 1);
    //Write disable
    bWriteData[0] = 0x10;
    bWriteData[1] = 0x04;
    i2c_write_update_msg2133(bWriteData, 2);
    i2c_write_update_msg2133(&bWriteData1, 1);

    timeOutCount=0;
    msleep(1);           // delay about 100us
    while((drvISP_ReadStatus() & 0x01) == 0x01)
    {
        timeOutCount++;
	 if ( timeOutCount > 10000 ) 
            break; /* around 1 sec timeout */
    }

    //pr_ch("The drvISP_ReadStatus3=%d\n", drvISP_ReadStatus());
    drvISP_WriteEnable();
    //pr_ch("The drvISP_ReadStatus4=%d\n", drvISP_ReadStatus());
    bWriteData[0] = 0x10;
    bWriteData[1] = 0xC7;        //Block Erase
    //bWriteData[2] = ((addr >> 16) & 0xFF) ;
    //bWriteData[3] = ((addr >> 8) & 0xFF) ;
    // bWriteData[4] = (addr & 0xFF) ;
    i2c_write_update_msg2133(bWriteData, 2);
    //i2c_write_update_msg2133( &bWriteData, 5);
    i2c_write_update_msg2133(&bWriteData1, 1);

    timeOutCount=0;
    msleep(1);           // delay about 100us
    while((drvISP_ReadStatus() & 0x01) == 0x01)
    {
        timeOutCount++;
	 if ( timeOutCount > 10000 ) 
            break; /* around 1 sec timeout */
    }
}


void drvISP_Program(unsigned short k, unsigned char *pDataToWrite)//Check
{
    unsigned short i = 0;
    unsigned short j = 0;
    //U16 n = 0;
    unsigned char TX_data[133];
    unsigned char bWriteData1 = 0x12;
    unsigned int addr = k * 1024;
    unsigned int timeOutCount = 0; 
	MSG2133_DBG("hhh %s\n", __func__);

    for(j = 0; j < 8; j++)    //128*8 cycle
    {
        TX_data[0] = 0x10;
        TX_data[1] = 0x02;// Page Program CMD
        TX_data[2] = (addr + 128 * j) >> 16;
        TX_data[3] = (addr + 128 * j) >> 8;
        TX_data[4] = (addr + 128 * j);

        for(i = 0; i < 128; i++)
        {
            TX_data[5 + i] = pDataToWrite[j * 128 + i];
        }
        msleep(1);        // delay about 100us*****

        timeOutCount = 0;
        while ( ( drvISP_ReadStatus() & 0x01 ) == 0x01 )
        {
            timeOutCount++;
            if ( timeOutCount >= 100000 ) break; /* around 1 sec timeout */
        }

        drvISP_WriteEnable();
        i2c_write_update_msg2133( TX_data, 133);   //write 133 byte per cycle
        i2c_write_update_msg2133(&bWriteData1, 1);
    }
}

static void _HalTscrHWReset ( void )//This function must implement by customer
{
    msg2133_reset();
}

static void drvDB_WriteReg ( unsigned char bank, unsigned char addr, unsigned short data )//New. Check
{
    unsigned char tx_data[5] = {0x10, bank, addr, data & 0xFF, data >> 8};
    MSG2133_DBG("hhh %s\n", __func__);
    i2c_write_msg2133 ( tx_data, 5 );
}

static void drvDB_WriteReg8Bit ( unsigned char bank, unsigned char addr, unsigned char data )//New. Check
{
    unsigned char tx_data[4] = {0x10, bank, addr, data};
    MSG2133_DBG("hhh %s\n", __func__);
    i2c_write_msg2133 ( tx_data, 4 );
}

static unsigned short drvDB_ReadReg ( unsigned char bank, unsigned char addr )//New. Check
{
    unsigned char tx_data[3] = {0x10, bank, addr};
    unsigned char rx_data[2] = {0};
    MSG2133_DBG("hhh %s\n", __func__);

    i2c_write_msg2133 ( tx_data, 3 );
    i2c_read_msg2133 ( &rx_data[0], 2 );
    return ( rx_data[1] << 8 | rx_data[0] );
}

static unsigned int Reflect ( unsigned int ref, char ch )////New. Check
{
    unsigned int value = 0;
    unsigned int i = 0;
//    MSG2133_DBG("hhh %s\n", __func__);

    for ( i = 1; i < ( ch + 1 ); i++ )
    {
        if ( ref & 1 )
        {
            value |= 1 << ( ch - i );
        }
        ref >>= 1;
    }
    return value;
}

static void Init_CRC32_Table ( unsigned int *crc32_table )//New. Check
{
    unsigned int magicnumber = 0x04c11db7;
    unsigned int i = 0, j;
    MSG2133_DBG("hhh %s\n", __func__);

    for ( i = 0; i <= 0xFF; i++ )
    {
        crc32_table[i] = Reflect ( i, 8 ) << 24;
        for ( j = 0; j < 8; j++ )
        {
            crc32_table[i] = ( crc32_table[i] << 1 ) ^ ( crc32_table[i] & ( 0x80000000L ) ? magicnumber : 0 );
        }
        crc32_table[i] = Reflect ( crc32_table[i], 32 );
    }
}

unsigned int Get_CRC ( unsigned int text, unsigned int prevCRC, unsigned int *crc32_table )//New. Check
{
    unsigned int ulCRC = prevCRC;
//    MSG2133_DBG("hhh %s\n", __func__);
    {
        ulCRC = ( ulCRC >> 8 ) ^ crc32_table[ ( ulCRC & 0xFF ) ^ text];
    }
    return ulCRC ;
}

static ssize_t firmware_version_show(struct device *dev,
                                     struct device_attribute *attr, char *buf)//Check
{
    msg2133_get_version(&fw_v);
	MSG2133_DBG("tyd-tp: firmware_version_show\n");
	MSG2133_DBG("hhh %s\n", __func__);
	MSG2133_DBG("*** firmware_version_show fw_version = %03d.%03d.%02d***\n", fw_v.major, fw_v.minor,fw_v.VenderID);
//	return sprintf(buf, "%03x%03x\n", fw_v.major,fw_v.minor);
    return sprintf(buf, "%d.%d\n", fw_v.major,fw_v.minor);
}

static ssize_t firmware_version_store(struct device *dev,
                                      struct device_attribute *attr, const char *buf, size_t size)
{
	int i;
	MSG2133_DBG("hhh %s\n", __func__);


	unsigned char dbbus_tx_data[3];
	unsigned char dbbus_rx_data[5] ;
	
	MSG2133_DBG("%s\n", __func__);
/*	
	dbbusDWIICEnterSerialDebugMode();
	dbbusDWIICStopMCU();
	dbbusDWIICIICUseBus();
	dbbusDWIICIICReshape();
    */
	MSG2133_DBG("\n");

	for (i = 0; i < 10; i++)
	{
	dbbus_tx_data[0] = 0x53;
	dbbus_tx_data[1] = 0x00;
	dbbus_tx_data[2] = 0x74;

	msg2133_i2c_write(&dbbus_tx_data[0], 3);
	mdelay(50);
	msg2133_i2c_read(&dbbus_rx_data[0], 5);
	MSG2133_DBG("dbbus_rx_data[0] = %x\n",dbbus_rx_data[0]);
	MSG2133_DBG("dbbus_rx_data[1] = %x\n",dbbus_rx_data[1]);
	MSG2133_DBG("dbbus_rx_data[2] = %x\n",dbbus_rx_data[2]);
	MSG2133_DBG("dbbus_rx_data[3] = %x\n",dbbus_rx_data[3]);
	MSG2133_DBG("dbbus_rx_data[4] = %x\n",dbbus_rx_data[4]);

	fw_v.major = (dbbus_rx_data[0] << 8) + dbbus_rx_data[1];
	fw_v.minor = (dbbus_rx_data[3] << 8) + dbbus_rx_data[2];
	fw_v.VenderID= (dbbus_rx_data[4]);
	MSG2133_DBG("fw_major = %x, fw_minor = %x\n",fw_v.major,fw_v.minor);
	if ((fw_v.major & 0xff00) == 0)
		break;
	
	}
	return size;
}


static ssize_t firmware_data_show(struct device *dev,
                                  struct device_attribute *attr, char *buf)//Check
{
	MSG2133_DBG("tyd-tp: firmware_data_show\n");
	MSG2133_DBG("hhh %s\n", __func__);
    	return FwDataCnt;
}

static ssize_t firmware_data_store(struct device *dev,
                                   struct device_attribute *attr, const char *buf, size_t size)//Check
{
    	int i;
    	MSG2133_DBG("***FwDataCnt = %d ***\n", FwDataCnt);
		MSG2133_DBG("hhh %s\n", __func__);
	MSG2133_DBG("tyd-tp: firmware_data_store\n");
//    	for(i = 0; i < 1024; i++){
//        	memcpy(temp[FwDataCnt], buf, 1024);
//    	}

//    	FwDataCnt++;
    	return -ENODEV;
}

static ssize_t firmware_clear_show(struct device *dev,
                                   struct device_attribute *attr, char *buf)//Check
{
    unsigned short k = 0, i = 0, j = 0;
    unsigned char bWriteData[5] =
    {
        0x10, 0x03, 0, 0, 0
    };
    unsigned char RX_data[256];
    unsigned char bWriteData1 = 0x12;
    unsigned int addr = 0;
    MSG2133_DBG("\n");
	MSG2133_DBG("hhh %s\n", __func__);
	MSG2133_DBG("tyd-tp: firmware_clear_show\n");
    for(k = 0; k < 94; i++)    // total  94 KB : 1 byte per R/W
    {
        addr = k * 1024;

        for(j = 0; j < 8; j++)    //128*8 cycle
        {
            bWriteData[2] = (unsigned char)((addr + j * 128) >> 16);
            bWriteData[3] = (unsigned char)((addr + j * 128) >> 8);
            bWriteData[4] = (unsigned char)(addr + j * 128);

            while((drvISP_ReadStatus() & 0x01) == 0x01)
            {
                ;    //wait until not in write operation
            }

            i2c_write_update_msg2133(bWriteData, 5);     //write read flash addr
            drvISP_Read(128, RX_data);
            i2c_write_update_msg2133(&bWriteData1, 1);    //cmd end

            for(i = 0; i < 128; i++)    //log out if verify error{
                if(RX_data[i] != 0xFF){
                    MSG2133_DBG("k=%d,j=%d,i=%d===============erase not clean================", k, j, i);
                }
            }
     }
    MSG2133_DBG("read finish\n");
    return sprintf(buf, "%03d.%03d\n", fw_v.major, fw_v.minor);
}

static ssize_t firmware_clear_store(struct device *dev,
                                    struct device_attribute *attr, const char *buf, size_t size)
{
    unsigned char dbbus_tx_data[4];
    unsigned char dbbus_rx_data[2] = {0};
    //msctpc_LoopDelay ( 100 );        // delay about 100ms*****
    // Enable slave's ISP ECO mode
    MSG2133_DBG("hhh %s\n", __func__);
    dbbusDWIICEnterSerialDebugMode();
    dbbusDWIICStopMCU();
    dbbusDWIICIICUseBus();
    dbbusDWIICIICReshape();
    MSG2133_DBG("\n");
	MSG2133_DBG("tyd-tp: firmware_clear_store\n");
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x08;
    dbbus_tx_data[2] = 0x0c;
    dbbus_tx_data[3] = 0x08;
    // Disable the Watchdog
    i2c_write_msg2133(dbbus_tx_data, 4);
    //Get_Chip_Version();
    //FwVersion  = 2;
    //if (FwVersion  == 2)
    {
		dbbus_tx_data[0] = 0x10;
		dbbus_tx_data[1] = 0x11;
		dbbus_tx_data[2] = 0xE2;
		dbbus_tx_data[3] = 0x00;
		i2c_write_msg2133(dbbus_tx_data, 4);
	}
	dbbus_tx_data[0] = 0x10;
	dbbus_tx_data[1] = 0x3C;
	dbbus_tx_data[2] = 0x60;
	dbbus_tx_data[3] = 0x55;
	i2c_write_msg2133(dbbus_tx_data, 4);
	dbbus_tx_data[0] = 0x10;
	dbbus_tx_data[1] = 0x3C;
	dbbus_tx_data[2] = 0x61;
	dbbus_tx_data[3] = 0xAA;
	i2c_write_msg2133(dbbus_tx_data, 4);
	//Stop MCU
	dbbus_tx_data[0] = 0x10;
	dbbus_tx_data[1] = 0x0F;
	dbbus_tx_data[2] = 0xE6;
	dbbus_tx_data[3] = 0x01;
	i2c_write_msg2133(dbbus_tx_data, 4);
	//Enable SPI Pad
	dbbus_tx_data[0] = 0x10;
	dbbus_tx_data[1] = 0x1E;
	dbbus_tx_data[2] = 0x02;
	i2c_write_msg2133(dbbus_tx_data, 3);
	i2c_read_msg2133(&dbbus_rx_data[0], 2);
	MSG2133_DBG("dbbus_rx_data[0]=0x%x", dbbus_rx_data[0]);
	dbbus_tx_data[3] = (dbbus_rx_data[0] | 0x20);  //Set Bit 5
	i2c_write_msg2133(dbbus_tx_data, 4);
	dbbus_tx_data[0] = 0x10;
	dbbus_tx_data[1] = 0x1E;
	dbbus_tx_data[2] = 0x25;
	i2c_write_msg2133(dbbus_tx_data, 3);
	dbbus_rx_data[0] = 0;
	dbbus_rx_data[1] = 0;
	i2c_read_msg2133(&dbbus_rx_data[0], 2);
	MSG2133_DBG("dbbus_rx_data[0]=0x%x", dbbus_rx_data[0]);
	dbbus_tx_data[3] = dbbus_rx_data[0] & 0xFC;  //Clear Bit 1,0
	i2c_write_msg2133(dbbus_tx_data, 4);
	//WP overwrite
	dbbus_tx_data[0] = 0x10;
	dbbus_tx_data[1] = 0x1E;
	dbbus_tx_data[2] = 0x0E;
	dbbus_tx_data[3] = 0x02;
	i2c_write_msg2133(dbbus_tx_data, 4);
	//set pin high
	dbbus_tx_data[0] = 0x10;
	dbbus_tx_data[1] = 0x1E;
	dbbus_tx_data[2] = 0x10;
	dbbus_tx_data[3] = 0x08;
	i2c_write_msg2133(dbbus_tx_data, 4);
	dbbusDWIICIICNotUseBus();
	dbbusDWIICNotStopMCU();
	dbbusDWIICExitSerialDebugMode();
	///////////////////////////////////////
	// Start to load firmware
	///////////////////////////////////////
	drvISP_EntryIspMode();
	MSG2133_DBG("chip erase+\n");
	
	if(tp_type == TP_HUANGZE){
		drvISP_SectorErase(0x000000);
		drvISP_SectorErase(0x001000);
		drvISP_SectorErase(0x002000);
		drvISP_SectorErase(0x003000);
		drvISP_SectorErase(0x004000);
		drvISP_SectorErase(0x005000);
		drvISP_SectorErase(0x006000);
		drvISP_SectorErase(0x007000);
		drvISP_SectorErase(0x008000);
		drvISP_SectorErase(0x009000);
		drvISP_SectorErase(0x00a000);
		drvISP_SectorErase(0x00b000);
		drvISP_SectorErase(0x00c000);
		drvISP_SectorErase(0x00d000);
		drvISP_SectorErase(0x00e000);
		drvISP_SectorErase(0x00f000);

	}else{
		drvISP_BlockErase(0x00000);
	}  


	MSG2133_DBG("chip erase-\n");
	drvISP_ExitIspMode();
	return size;
}

static int drvTP_erase_emem_c33 ( EMEM_TYPE_t emem_type )//New, Check
{
    // stop mcu
    MSG2133_DBG("hhh %s\n", __func__);
    drvDB_WriteReg ( 0x0F, 0xE6, 0x0001 );

    //disable watch dog
    drvDB_WriteReg8Bit ( 0x3C, 0x60, 0x55 );
    drvDB_WriteReg8Bit ( 0x3C, 0x61, 0xAA );

    // set PROGRAM password
    drvDB_WriteReg8Bit ( 0x16, 0x1A, 0xBA );
    drvDB_WriteReg8Bit ( 0x16, 0x1B, 0xAB );

    //proto.MstarWriteReg(F1.loopDevice, 0x1618, 0x80);
    drvDB_WriteReg8Bit ( 0x16, 0x18, 0x80 );

    if ( emem_type == EMEM_ALL )
    {
        drvDB_WriteReg8Bit ( 0x16, 0x08, 0x10 ); //mark
    }

    drvDB_WriteReg8Bit ( 0x16, 0x18, 0x40 );
    mdelay ( 10 );

    drvDB_WriteReg8Bit ( 0x16, 0x18, 0x80 );

    // erase trigger
    if ( emem_type == EMEM_MAIN )
    {
        drvDB_WriteReg8Bit ( 0x16, 0x0E, 0x04 ); //erase main
    }
    else
    {
        drvDB_WriteReg8Bit ( 0x16, 0x0E, 0x08 ); //erase all block
    }

    return ( 1 );
}

static int drvTP_read_info_dwiic_c33 ( void )//New, Check
{
    unsigned char dwiic_tx_data[5];

    //drvDB_EnterDBBUS();
    MSG2133_DBG("hhh %s\n", __func__);
     _HalTscrHWReset();
     mdelay ( 300 );
    dbbusDWIICEnterSerialDebugMode();
    dbbusDWIICStopMCU();
    dbbusDWIICIICUseBus();
    dbbusDWIICIICReshape();
    mdelay ( 300 );

    // Stop Watchdog
    drvDB_WriteReg8Bit ( 0x3C, 0x60, 0x55 );
    drvDB_WriteReg8Bit ( 0x3C, 0x61, 0xAA );

    drvDB_WriteReg ( 0x3C, 0xE4, 0xA4AB );

    // TP SW reset
    drvDB_WriteReg ( 0x1E, 0x02, 0x829F );

    mdelay ( 50 );

    dwiic_tx_data[0] = 0x72;
    dwiic_tx_data[1] = 0x80;
    dwiic_tx_data[2] = 0x00;
    dwiic_tx_data[3] = 0x04;
    dwiic_tx_data[4] = 0x00;
    msg2133_i2c_write ( dwiic_tx_data, 5 );

    mdelay ( 50 );

    // recive info data
    msg2133_i2c_read ( &g_dwiic_info_data[0], 1024 );

    return ( 1 );
}

static int drvTP_info_updata_C33 ( unsigned short start_index, unsigned char *data, unsigned short size )//New, check
{
    // size != 0, start_index+size !> 1024
    unsigned short i;
    MSG2133_DBG("hhh %s\n", __func__);
    for ( i = 0;i < size; i++ )
    {
        g_dwiic_info_data[start_index] = * ( data + i );
        start_index++;
    }
    
    return ( 1 );
}

#if 0
static int firmware_update_c33 (EMEM_TYPE_t emem_type )//New, check
{
//    unsigned char dbbus_tx_data[4];
//    unsigned char  dbbus_rx_data[2] = {0};
    unsigned char  life_counter[2];
    unsigned int i, j;
    unsigned int crc_main, crc_main_tp;
    unsigned int crc_info, crc_info_tp;
    unsigned int crc_tab[256];

    int update_pass = 1;
    unsigned short reg_data = 0;
	MSG2133_DBG("hhh %s\n", __func__);

    crc_main = 0xffffffff;
    crc_info = 0xffffffff;

    drvTP_read_info_dwiic_c33();

    if ( g_dwiic_info_data[0] == 'M' && g_dwiic_info_data[1] == 'S' && g_dwiic_info_data[2] == 'T' && g_dwiic_info_data[3] == 'A' && g_dwiic_info_data[4] == 'R' && g_dwiic_info_data[5] == 'T' && g_dwiic_info_data[6] == 'P' && g_dwiic_info_data[7] == 'C' )
    {
        // updata FW Version
        pr_info("%s: read info\n", __func__);
        //drvTP_info_updata_C33 ( 8, &temp[32][8], 4 );

        // updata life counter
	// WTF???
//        life_counter[0] = ( ( g_dwiic_info_data[12] << 8 + g_dwiic_info_data[13] + 1 ) >> 8 ) & 0xFF;
  //      life_counter[1] = ( g_dwiic_info_data[12] << 8 + g_dwiic_info_data[13] + 1 ) & 0xFF;
        drvTP_info_updata_C33 ( 10, life_counter, 2 );

        drvDB_WriteReg ( 0x3C, 0xE4, 0x78C5 );

        // TP SW reset
        drvDB_WriteReg ( 0x1E, 0x02, 0x829F );

        mdelay ( 50 );

        //polling 0x3CE4 is 0x2F43
        do
        {
            reg_data = drvDB_ReadReg ( 0x3C, 0xE4 );
        }
        while ( reg_data != 0x2F43 );

        // transmit lk info data
        msg2133_i2c_write ( g_dwiic_info_data, 1024 );

        //polling 0x3CE4 is 0xD0BC
        do
        {
            reg_data = drvDB_ReadReg ( 0x3C, 0xE4 );
        }
        while ( reg_data != 0xD0BC );

    }


    //erase main
    drvTP_erase_emem_c33 ( EMEM_MAIN );
    mdelay ( 1000 );

    //ResetSlave();
    _HalTscrHWReset();

    //drvDB_EnterDBBUS();
    dbbusDWIICEnterSerialDebugMode();
    dbbusDWIICStopMCU();
    dbbusDWIICIICUseBus();
    dbbusDWIICIICReshape();
    mdelay ( 300 );

    /////////////////////////
    // Program
    /////////////////////////

    //polling 0x3CE4 is 0x1C70
    if ( ( emem_type == EMEM_ALL ) || ( emem_type == EMEM_MAIN ) )
    {
        do
        {
            reg_data = drvDB_ReadReg ( 0x3C, 0xE4 );
        }
        while ( reg_data != 0x1C70 );
    }

    switch ( emem_type )
    {
        case EMEM_ALL:
            drvDB_WriteReg ( 0x3C, 0xE4, 0xE38F );  // for all-blocks
            break;
        case EMEM_MAIN:
            drvDB_WriteReg ( 0x3C, 0xE4, 0x7731 );  // for main block
            break;
        case EMEM_INFO:
            drvDB_WriteReg ( 0x3C, 0xE4, 0x7731 );  // for info block


            drvDB_WriteReg8Bit ( 0x0F, 0xE6, 0x01 );

            drvDB_WriteReg8Bit ( 0x3C, 0xE4, 0xC5 ); //
            drvDB_WriteReg8Bit ( 0x3C, 0xE5, 0x78 ); //

            drvDB_WriteReg8Bit ( 0x1E, 0x04, 0x9F );
            drvDB_WriteReg8Bit ( 0x1E, 0x05, 0x82 );

            drvDB_WriteReg8Bit ( 0x0F, 0xE6, 0x00 );
            mdelay ( 100 );
            break;
    }

    // polling 0x3CE4 is 0x2F43
    do
    {
        reg_data = drvDB_ReadReg ( 0x3C, 0xE4 );
    }
    while ( reg_data != 0x2F43 );


    // calculate CRC 32
    Init_CRC32_Table ( &crc_tab[0] );


    for ( i = 0; i < 33; i++ ) // total  33 KB : 2 byte per R/W
    {
         if( emem_type == EMEM_INFO ) i = 32;

        if ( i < 32 )   //emem_main
        {
            if ( i == 31 )
            {
//                temp[i][1014] = 0x5A; //Fmr_Loader[1014]=0x5A;
//                temp[i][1015] = 0xA5; //Fmr_Loader[1015]=0xA5;
                fw_buffer[i * 1024 + 1014] = 0x5A;
                fw_buffer[i * 1024 + 1015] = 0xA5;

                for ( j = 0; j < 1016; j++ )
                {
                    //crc_main=Get_CRC(Fmr_Loader[j],crc_main,&crc_tab[0]);
                    crc_main = Get_CRC ( fw_buffer[i * 1024 + j], crc_main, &crc_tab[0] );
                }
            }
            else
            {
                for ( j = 0; j < 1024; j++ )
                {
                    //crc_main=Get_CRC(Fmr_Loader[j],crc_main,&crc_tab[0]);
                    crc_main = Get_CRC ( fw_buffer[i * 1024 + j], crc_main, &crc_tab[0] );
                }
            }
        }
        else  //emem_info
        {
            for ( j = 0; j < 1024; j++ )
            {
                //crc_info=Get_CRC(Fmr_Loader[j],crc_info,&crc_tab[0]);
                crc_info = Get_CRC ( g_dwiic_info_data[j], crc_info, &crc_tab[0] );
            }
            if ( emem_type == EMEM_MAIN ) break;
        }

        //drvDWIIC_MasterTransmit( DWIIC_MODE_DWIIC_ID, 1024, Fmr_Loader );
        msg2133_i2c_write ( &fw_buffer[i * 1024], 1024 );

        // polling 0x3CE4 is 0xD0BC
        do
        {
            reg_data = drvDB_ReadReg ( 0x3C, 0xE4 );
        }
        while ( reg_data != 0xD0BC );

        drvDB_WriteReg ( 0x3C, 0xE4, 0x2F43 );
    }

    if ( ( emem_type == EMEM_ALL ) || ( emem_type == EMEM_MAIN ) )
    {
        // write file done and check crc
        drvDB_WriteReg ( 0x3C, 0xE4, 0x1380 );
    }

    mdelay ( 10 ); //MCR_CLBK_DEBUG_DELAY ( 10, MCU_LOOP_DELAY_COUNT_MS );

    if ( ( emem_type == EMEM_ALL ) || ( emem_type == EMEM_MAIN ) )
    {
        // polling 0x3CE4 is 0x9432
        do
        {
            reg_data = drvDB_ReadReg ( 0x3C, 0xE4 );
        }
        while ( reg_data != 0x9432 );
    }

    crc_main = crc_main ^ 0xffffffff;
    crc_info = crc_info ^ 0xffffffff;

    if ( ( emem_type == EMEM_ALL ) || ( emem_type == EMEM_MAIN ) )
    {
        // CRC Main from TP
        crc_main_tp = drvDB_ReadReg ( 0x3C, 0x80 );
        crc_main_tp = ( crc_main_tp << 16 ) | drvDB_ReadReg ( 0x3C, 0x82 );
        MSG2133_DBG ( "crc_main=0x%x, crc_main_tp=0x%x\n",
                   crc_main, crc_main_tp );
    }

    if ( ( emem_type == EMEM_ALL ) || ( emem_type == EMEM_INFO ) )
    {
        // CRC Info from TP
        crc_info_tp = drvDB_ReadReg ( 0x3C, 0xA0 );
        crc_info_tp = ( crc_info_tp << 16 ) | drvDB_ReadReg ( 0x3C, 0xA2 );
        MSG2133_DBG ( "crc_info=0x%x, crc_info_tp=0x%x\n",
                   crc_info, crc_info_tp );
    }

    //drvDB_ExitDBBUS();

    update_pass = 1;
    if ( ( emem_type == EMEM_ALL ) || ( emem_type == EMEM_MAIN ) )
    {
        if ( crc_main_tp != crc_main )
            update_pass = 0;
    }

    if ( ( emem_type == EMEM_ALL ) || ( emem_type == EMEM_INFO ) )
    {
        if ( crc_info_tp != crc_info )
            update_pass = 0;
    }

    if ( !update_pass )
        MSG2133_DBG ( "update FAILED\n" );
    else
        MSG2133_DBG ( "update OK\n" );
    FwDataCnt = 0;
    _HalTscrHWReset();

    return update_pass;
}
#endif

static ssize_t firmware_update_show(struct device *dev,
                                    struct device_attribute *attr, char *buf)
{
	MSG2133_DBG("tyd-tp: firmware_update_show\n");
	MSG2133_DBG("hhh %s\n", __func__);
	return sprintf(buf, "%03d%03d\n", fw_v.major, fw_v.minor);
}


static int do_update_firmware(void) {
    unsigned char dbbus_tx_data[4];
    unsigned char dbbus_rx_data[2] = {0};
    int r;

	MSG2133_DBG("hhh %s\n", __func__);

    _HalTscrHWReset();

    // Erase TP Flash first
    dbbusDWIICEnterSerialDebugMode();
    dbbusDWIICStopMCU();
    dbbusDWIICIICUseBus();
    dbbusDWIICIICReshape();
    mdelay ( 300 );

    // Disable the Watchdog
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x3C;
    dbbus_tx_data[2] = 0x60;
    dbbus_tx_data[3] = 0x55;
    i2c_write_msg2133 ( dbbus_tx_data, 4 );
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x3C;
    dbbus_tx_data[2] = 0x61;
    dbbus_tx_data[3] = 0xAA;
    i2c_write_msg2133 ( dbbus_tx_data, 4 );

    // Stop MCU
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x0F;
    dbbus_tx_data[2] = 0xE6;
    dbbus_tx_data[3] = 0x01;
    i2c_write_msg2133 ( dbbus_tx_data, 4 );


    /////////////////////////
    // Difference between C2 and C3
    /////////////////////////

    //check id
    dbbus_tx_data[0] = 0x10;
    dbbus_tx_data[1] = 0x1E;
    dbbus_tx_data[2] = 0xCC;
    i2c_write_msg2133 ( dbbus_tx_data, 3 );
    i2c_read_msg2133 ( &dbbus_rx_data[0], 2 );
    MSG2133_DBG ( "dbbus_rx id[0]=0x%x", dbbus_rx_data[0] );

    if ( dbbus_rx_data[0] == 2 )
    {
        // check version
        dbbus_tx_data[0] = 0x10;
        dbbus_tx_data[1] = 0x3C;
        dbbus_tx_data[2] = 0xEA;
        i2c_write_msg2133 ( dbbus_tx_data, 3 );
        i2c_read_msg2133 ( &dbbus_rx_data[0], 2 );
        MSG2133_DBG ( "dbbus_rx version[0]=0x%x", dbbus_rx_data[0] );

        if ( dbbus_rx_data[0] == 3 )
        {
//            r = firmware_update_c33 ( EMEM_MAIN );
            _HalTscrHWReset();
            return r ? 1 : -EIO;
        }
        else
        {
            //return firmware_update_c32 ( dev, attr, buf, size, EMEM_ALL );
            pr_err("%s: unsupported version (c32)\n", __func__);
            _HalTscrHWReset();
            return -ENODEV;
        }
    }
    else
    {
        //return firmware_update_c2 ( dev, attr, buf, size );
        pr_err("%s: unsupported version (c2)\n", __func__);
        _HalTscrHWReset();
        return -ENODEV;
    }

}

static ssize_t firmware_update_store ( struct device *dev,
                                       struct device_attribute *attr, const char *buf, size_t size )//New, check
{
    int r;
    r = do_update_firmware();
    return (r < 0) ? r : size;
}

static DEVICE_ATTR(version, 0444, firmware_version_show, firmware_version_store);
static DEVICE_ATTR(update, 0777, firmware_update_show, firmware_update_store);
static DEVICE_ATTR(data, 0777, firmware_data_show, firmware_data_store);
static DEVICE_ATTR(clear, 0777, firmware_clear_show, firmware_clear_store);


//    ssize_t (*write)(struct file *,struct kobject *, struct bin_attribute *,
//             char *, loff_t, size_t);


ssize_t firmware_updatefw(struct file *filp, struct kobject *kobj,
         struct bin_attribute *bin_attr,
         char *buf, loff_t off, size_t count) {
    // store firmware in buffer
    if (off >= FW_SIZE)
        return -ENOMEM;
    if ((off + count) > FW_SIZE) {
        count = FW_SIZE - off;
    }
    memcpy(fw_buffer + off, buf, count);
    printk("%s: pos %lld(%#llx) size %d(%#x)\n", __func__, off, off, count, count);

    return count; // consume all
}

static struct bin_attribute bin_attr_updatefw = {
    .attr = {.name = "updatefw", .mode = 0200 },
    .read   = NULL,
    .write  = firmware_updatefw,
};

ssize_t firmware_startupdate(struct file *filp, struct kobject *kobj,
         struct bin_attribute *bin_attr,
         char *buf, loff_t off, size_t count) {
    int r;
    struct CNT_TS_DATA_T *cnt_ts = (struct CNT_TS_DATA_T *)bin_attr->private;
    // store firmware in buffer
    if (off != 0)
        return -ENOMEM;
    // do update
    if (cnt_ts)
        cnt_ts->in_suspend |= LOCK_MODE;
    r = do_update_firmware();
    if (cnt_ts)
        cnt_ts->in_suspend &= ~LOCK_MODE;
    return (r < 0) ? r : count;
}

static struct bin_attribute bin_attr_startupdate = {
    .attr = {.name = "startupdate", .mode = 0200 },
    .read   = NULL,
    .write  = firmware_startupdate,
};

static void msg2133_init_fw_class(struct CNT_TS_DATA_T *cnt_ts)
{
	firmware_class = class_create(THIS_MODULE,"mtk-tpd" );//client->name
	pr_err("hhh%s\n", __func__);

	if(IS_ERR(firmware_class))
		pr_err("Failed to create class(firmware)!\n");

	firmware_cmd_dev = device_create(firmware_class,
	                                     NULL, 0, NULL, "device");//device

	if(IS_ERR(firmware_cmd_dev))
		pr_err("Failed to create device(firmware_cmd_dev)!\n");
		
	// version /sys/class/mtk-tpd/device/version
	if(device_create_file(firmware_cmd_dev, &dev_attr_version) < 0)
		pr_err("Failed to create device file(%s)!\n", dev_attr_version.attr.name);

	// update /sys/class/mtk-tpd/device/update
//	if(device_create_file(firmware_cmd_dev, &dev_attr_update) < 0)
//		pr_err("Failed to create device file(%s)!\n", dev_attr_update.attr.name);

	// data /sys/class/mtk-tpd/device/data
//	if(device_create_file(firmware_cmd_dev, &dev_attr_data) < 0)
//		pr_err("Failed to create device file(%s)!\n", dev_attr_data.attr.name);

	// clear /sys/class/mtk-tpd/device/clear
//	if(device_create_file(firmware_cmd_dev, &dev_attr_clear) < 0)
//		pr_err("Failed to create device file(%s)!\n", dev_attr_clear.attr.name);

    bin_attr_updatefw.private = cnt_ts;
    if(device_create_bin_file(firmware_cmd_dev, &bin_attr_updatefw) < 0)
        pr_err("Failed to create device file(%s)!\n", bin_attr_updatefw.attr.name);

    bin_attr_startupdate.private = cnt_ts;
    if(device_create_bin_file(firmware_cmd_dev, &bin_attr_startupdate) < 0)
        pr_err("Failed to create device file(%s)!\n", bin_attr_updatefw.attr.name);
}

#endif /* MSG2133_UPDATE */


/* Addresses to scan */
static union{
	unsigned short dirty_addr_buf[2];
	const unsigned short normal_i2c[2];
}u_i2c_addr = {{0x00},};
static __u32 twi_id = 0;

//use sw gpio interrupt trigger

/*
 * ctp_get_pendown_state  : get the int_line data state,
 *
 * return value:
 *             return PRESS_DOWN: if down
 *             return FREE_UP: if up,
 *             return 0: do not need process, equal free up.
 */
static int ctp_get_pendown_state(void) {

	//not supported
	return 0;
}

/**
 * ctp_clear_penirq - clear int pending
 *
 */
static void ctp_clear_penirq(void) {

	sw_gpio_eint_clr_irqpd_sta(gpio_int_hdle);
	return;
}

/**
 * ctp_set_irq_mode - according sysconfig's subkey "ctp_int_port" to config int port.
 *
 * return value:
 *              0:      success;
 *              others: fail;
 */
static int ctp_set_irq_mode(char *major_key , char *subkey, ext_int_mode int_mode) {

	pr_info("%s: config gpio to int mode. \n", __func__);

	if(gpio_int_hdle){
		sw_gpio_irq_free(gpio_int_hdle);
	}

	gpio_int_hdle = sw_gpio_irq_request(major_key,subkey,int_mode);

	return gpio_int_hdle ? 0 : -EIO;
}

/**
 * ctp_set_gpio_mode - according sysconfig's subkey "ctp_io_port" to config io port.
 *
 * return value:
 *              0:      success;
 *              others: fail;
 */
static int ctp_set_gpio_mode(void) {

	//not supported
	return -1;
}

/**
 * ctp_judge_int_occur - whether interrupt occur.
 *
 * return value:
 *              0:      int occur;
 *              others: no int occur;
 */
static int ctp_judge_int_occur(void) {

	return (sw_gpio_eint_get_irqpd_sta(gpio_int_hdle) == 1) ? 0 : 1;
}

/**
 * ctp_free_platform_resource - corresponding with ctp_init_platform_resource
 *
 */
static void ctp_free_platform_resource(void)
{
	if(gpio_addr){
		iounmap(gpio_addr);
	}
	
	if(gpio_int_hdle){
		gpio_release(gpio_int_hdle, 2);
	}
	
	if(gpio_wakeup_hdle){
		gpio_release(gpio_wakeup_hdle, 2);
	}
	
	if(gpio_reset_hdle){
		gpio_release(gpio_reset_hdle, 2);
	}

	if(gpio_power_hdle){
		gpio_release(gpio_power_hdle, 2);
	}

	return;
}


/**
 * ctp_init_platform_resource - initialize platform related resource
 * return value: 0 : success
 *               -EIO :  i/o err.
 *
 */
static int ctp_init_platform_resource(void)
{
	int ret = 0;

	gpio_addr = ioremap(PIO_BASE_ADDRESS, PIO_RANGE_SIZE);
	//pr_info("%s, gpio_addr = 0x%x. \n", __func__, gpio_addr);
	if(!gpio_addr) {
		ret = -EIO;
		goto exit_ioremap_failed;	
	}
	//    gpio_wakeup_enable = 1;
	gpio_wakeup_hdle = gpio_request_ex("ctp_para", "ctp_wakeup");
	if(!gpio_wakeup_hdle) {
		pr_warning("%s: tp_wakeup request gpio fail!\n", __func__);
		gpio_wakeup_enable = 0;
	}

	gpio_reset_hdle = gpio_request_ex("ctp_para", "ctp_reset");
	if(!gpio_reset_hdle) {
		pr_warning("%s: tp_reset request gpio fail!\n", __func__);
		gpio_reset_enable = 0;
	}

	gpio_power_hdle = gpio_request_ex("ctp_para", "ctp_powerpin");
	if(!gpio_reset_hdle) {
		pr_warning("%s: tp_power request gpio fail!\n", __func__);
		gpio_reset_enable = 0;
	}

	return ret;

exit_ioremap_failed:
	ctp_free_platform_resource();
	return ret;
}


/**
 * ctp_fetch_sysconfig_para - get config info from sysconfig.fex file.
 * return value:  
 *                    = 0; success;
 *                    < 0; err
 */
static int ctp_fetch_sysconfig_para(void)
{
	int ret = -1;
	int ctp_used = -1;
	char name[I2C_NAME_SIZE];
	__u32 twi_addr = 0;
	//__u32 twi_id = 0;
	script_parser_value_type_t type = SCIRPT_PARSER_VALUE_TYPE_STRING;

	pr_info("%s. \n", __func__);

	if(SCRIPT_PARSER_OK != script_parser_fetch("ctp_para", "ctp_used", &ctp_used, 1)){
		pr_err("%s: script_parser_fetch err. \n", __func__);
		goto script_parser_fetch_err;
	}
	if(1 != ctp_used){
		pr_err("%s: ctp_unused. \n",  __func__);
		//ret = 1;
		return ret;
	}

	if(SCRIPT_PARSER_OK != script_parser_fetch_ex("ctp_para", "ctp_name", (int *)(&name), &type, sizeof(name)/sizeof(int))){
		pr_err("%s: script_parser_fetch err. \n", __func__);
		goto script_parser_fetch_err;
	}
	if(strcmp(CTP_NAME, name)){
		pr_err("%s: name %s does not match CTP_NAME. \n", __func__, name);
		pr_err(CTP_NAME);
		//ret = 1;
//		return ret;
	}

	twi_addr = 0x24;
	u_i2c_addr.dirty_addr_buf[0] = twi_addr;
	u_i2c_addr.dirty_addr_buf[1] = I2C_CLIENT_END;
	pr_info("%s: after: ctp_twi_addr is 0x%x, dirty_addr_buf: 0x%hx. dirty_addr_buf[1]: 0x%hx \n", __func__, twi_addr, u_i2c_addr.dirty_addr_buf[0], u_i2c_addr.dirty_addr_buf[1]);

	twi_id = 2;
	pr_info("%s: ctp_twi_id is %d. \n", __func__, twi_id);
	
	if(SCRIPT_PARSER_OK != script_parser_fetch("ctp_para", "ctp_screen_max_x", &screen_max_x, 1)){
		pr_err("%s: script_parser_fetch err. \n", __func__);
		goto script_parser_fetch_err;
	}
	pr_info("%s: screen_max_x = %d. \n", __func__, screen_max_x);

	if(SCRIPT_PARSER_OK != script_parser_fetch("ctp_para", "ctp_screen_max_y", &screen_max_y, 1)){
		pr_err("%s: script_parser_fetch err. \n", __func__);
		goto script_parser_fetch_err;
	}
	pr_info("%s: screen_max_y = %d. \n", __func__, screen_max_y);

	if(SCRIPT_PARSER_OK != script_parser_fetch("ctp_para", "ctp_revert_x_flag", &revert_x_flag, 1)){
		pr_err("%s: script_parser_fetch err. \n", __func__);
		goto script_parser_fetch_err;
	}
	pr_info("%s: revert_x_flag = %d. \n", __func__, revert_x_flag);

	if(SCRIPT_PARSER_OK != script_parser_fetch("ctp_para", "ctp_revert_y_flag", &revert_y_flag, 1)){
		pr_err("%s: script_parser_fetch err. \n", __func__);
		goto script_parser_fetch_err;
	}
	pr_info("%s: revert_y_flag = %d. \n", __func__, revert_y_flag);

	if(SCRIPT_PARSER_OK != script_parser_fetch("ctp_para", "ctp_exchange_x_y_flag", &exchange_x_y_flag, 1)){
		pr_err("cnt_ts: script_parser_fetch err. \n");
		goto script_parser_fetch_err;
	}
	pr_info("%s: exchange_x_y_flag = %d. \n", __func__, exchange_x_y_flag);

	return 0;

script_parser_fetch_err:
	pr_notice("=========script_parser_fetch_err============\n");
	return ret;
}

/**
 * ctp_reset - function
 *
 */
static void ctp_reset(void)
{
	if(gpio_reset_enable){
		pr_info("%s. \n", __func__);
		if(EGPIO_SUCCESS != gpio_write_one_pin_value(gpio_reset_hdle, 0, "ctp_reset")){
			pr_info("%s: err when operate gpio. \n", __func__);
		}
		mdelay(TS_RESET_LOW_PERIOD);
		if(EGPIO_SUCCESS != gpio_write_one_pin_value(gpio_reset_hdle, 1, "ctp_reset")){
			pr_info("%s: err when operate gpio. \n", __func__);
		}
		mdelay(TS_INITIAL_HIGH_PERIOD);
	}
}

/**
 * ctp_wakeup - function
 *
 */
static void ctp_wakeup(void)
{
	if(1 == gpio_wakeup_enable){  
		pr_info("%s. \n", __func__);
		if(EGPIO_SUCCESS != gpio_write_one_pin_value(gpio_wakeup_hdle, 0, "ctp_wakeup")){
			pr_info("%s: err when operate gpio. \n", __func__);
		}
		mdelay(TS_WAKEUP_LOW_PERIOD);
		if(EGPIO_SUCCESS != gpio_write_one_pin_value(gpio_wakeup_hdle, 1, "ctp_wakeup")){
			pr_info("%s: err when operate gpio. \n", __func__);
		}
		mdelay(TS_WAKEUP_HIGH_PERIOD);

	}
	return;
}

struct i2c_adapter *our_adapter;

/**
 * ctp_detect - Device detection callback for automatic device creation
 * return value:  
 *                    = 0; success;
 *                    < 0; err
 */
static int ctp_detect(struct i2c_client *client, struct i2c_board_info *info)
{
	struct i2c_adapter *adapter = client->adapter;

	if(twi_id == adapter->nr)
	{
		pr_info("%s: Detected chip %s at adapter %d, address 0x%02x\n",
			 __func__, CTP_NAME, i2c_adapter_id(adapter), client->addr);

		our_adapter = adapter;
		strlcpy(info->type, CTP_NAME, I2C_NAME_SIZE);
		return 0;
	}else{
		return -ENODEV;
	}
}
////////////////////////////////////////////////////////////////

static struct ctp_platform_ops ctp_ops = {
	.get_pendown_state = ctp_get_pendown_state,
	.clear_penirq	   = ctp_clear_penirq,
	.set_irq_mode      = ctp_set_irq_mode,
	.set_gpio_mode     = ctp_set_gpio_mode,	
	.judge_int_occur   = ctp_judge_int_occur,
	.init_platform_resource = ctp_init_platform_resource,
	.free_platform_resource = ctp_free_platform_resource,
	.fetch_sysconfig_para = ctp_fetch_sysconfig_para,
	.ts_reset =          ctp_reset,
	.ts_wakeup =         ctp_wakeup,
	.ts_detect = ctp_detect,
};

static struct i2c_dev *get_free_i2c_dev(struct i2c_adapter *adap) 
{
	struct i2c_dev *i2c_dev;

	if (adap->nr >= I2C_MINORS){
		pr_info("i2c-dev:out of device minors (%d) \n",adap->nr);
		return ERR_PTR (-ENODEV);
	}

	i2c_dev = kzalloc(sizeof(*i2c_dev), GFP_KERNEL);
	if (!i2c_dev){
		return ERR_PTR(-ENOMEM);
	}
	i2c_dev->adap = adap;

	spin_lock(&i2c_dev_list_lock);
	list_add_tail(&i2c_dev->list, &i2c_dev_list);
	spin_unlock(&i2c_dev_list_lock);
	
	return i2c_dev;
}

#if 0
static void cnt_ts_release(void)
{
	struct CNT_TS_DATA_T *data = i2c_get_clientdata(this_client);

	input_report_abs(data->input_dev, ABS_PRESSURE, 0);
	input_report_key(data->input_dev, BTN_TOUCH, 0);
	
	input_sync(data->input_dev);
	return;

}
#endif

#define MAX_RETRY_READ 4


/***********************************************************************
    [function]: 
		      callback:								calculate data checksum
    [parameters]:
			    msg:             				data buffer which is used to store touch data;
			    s32Length:             	the length of the checksum ;
    [return]:
			    												checksum value;
************************************************************************/
static u8 Calculate_8BitsChecksum( u8 *msg, int s32Length )
{
	int s32Checksum = 0;
	int i;

	for ( i = 0 ; i < s32Length; i++ )
	{
		s32Checksum += msg[i];
	}

	return (u8)( ( -s32Checksum ) & 0xFF );
}

/***********************************************************************
    [function]: 
		      callback:								read touch  data ftom controller via i2c interface;
    [parameters]:
			    rxdata[in]:             data buffer which is used to store touch data;
			    length[in]:             the length of the data buffer;
    [return]:
			    CNT_TRUE:              	success;
			    CNT_FALSE:             	fail;
************************************************************************/
static int cnt_i2c_rxdata(u8 *rxdata, int length)
{
	int ret;
	struct i2c_msg msg;

	msg.addr = this_client->addr;
	msg.flags = I2C_M_RD;
	msg.len = length;
	msg.buf = rxdata;

	ret = i2c_transfer(this_client->adapter, &msg, 1);
	if (ret < 0)
		pr_err("msg %s i2c write error: %d\n", __func__, ret);

	return ret;
}

static int cnt_i2c_txdata(u8 *txdata, int length)
{
	int ret;
	struct i2c_msg msg;

	memset(&msg, 0, sizeof(msg));
	msg.addr = this_client->addr;
	msg.flags = 0;
	msg.len = length;
	msg.buf = txdata;

	ret = i2c_transfer(this_client->adapter, &msg, 1);
	if (ret < 0)
		pr_err("msg %s i2c write error: %d\n", __func__, ret);

	return ret;
}


/***********************************************************************
    [function]: 
		      callback:            		gather the finger information and calculate the X,Y
		                           		coordinate then report them to the input system;
    [parameters]:
          null;
    [return]:
          null;
************************************************************************/

#define READ_DATA_SIZE 14

union xy_data {
	u8 buf[READ_DATA_SIZE];
	struct cyttsp_xydata cy;
};

#define DBG(x) x

static int slots[CY_NUM_TRK_ID];

static int find_next_slot(void)
{
  int i;
  for (i = 0; i < CY_NUM_TRK_ID; i++)
    if (slots[i] == -1)
      return i;
  return -1;
}
 
static int find_slot_by_id(int id)
{
  int i;
  for (i = 0; i < CY_NUM_TRK_ID; i++)
    if (slots[i] == id)
      return i;
  return -1;
}

//static void handle_multi_touch(struct cyttsp_track_data *t, struct cyttsp *ts)
static void handle_multi_touch(struct cyttsp_track_data *t, struct CNT_TS_DATA_T *ts)
{

	u8 id;
	void (*mt_sync_func)(struct input_dev *) = NULL;//ts->platform_data->mt_sync;
	u8 contacts_left = 0;
	int slot;



	/* terminate any previous touch where the track
	 * is missing from the current event */
	for (id = 0; id < CY_NUM_TRK_ID; id++) {
		if ((t->cur_trk[id] != CY_NTCH) || (ts->act_trk[id] == CY_NTCH) )
			continue;

		slot = find_slot_by_id(id);
		slots[slot] = -1;
		
//		input_report_abs(ts->input_dev, ABS_MT_TOUCH_MAJOR, 1);        	
		input_report_abs(ts->input_dev, ABS_MT_SLOT, slot);						
		input_report_abs(ts->input_dev, ABS_MT_TRACKING_ID, -1);
//		if (mt_sync_func)
//			mt_sync_func(ts->input_dev);

//		input_mt_sync(ts->input_dev);
//		input_sync(ts->input_dev);
		
		ts->act_trk[id] = CY_NTCH;
		ts->prv_mt_pos[id][CY_XPOS] = 0;
		ts->prv_mt_pos[id][CY_YPOS] = 0;
	}
	/* set Multi-Touch current event signals */
	for (id = 0; id < CY_NUM_MT_TCH_ID; id++) {
		if (t->cur_mt_tch[id] >= CY_NUM_TRK_ID)
			continue;
		
		if ((slot = find_slot_by_id(t->cur_mt_tch[id])) < 0)
		{
			slot = find_next_slot();
			slots[slot] = t->cur_mt_tch[id];				
		}
		
//		input_report_abs(ts->input_dev, ABS_MT_TOUCH_MAJOR, 1);        	
//		input_report_abs(ts->input_dev, ABS_MT_SLOT, slot);
//		input_report_abs(ts->input_dev, ABS_MT_TRACKING_ID, slot);

		input_report_abs(ts->input_dev, ABS_MT_POSITION_X,
						t->cur_mt_pos[id][CY_XPOS]);
		input_report_abs(ts->input_dev, ABS_MT_POSITION_Y,
						t->cur_mt_pos[id][CY_YPOS]);
//		if (mt_sync_func)
//			mt_sync_func(ts->input_dev);
		input_mt_sync(ts->input_dev);

		ts->act_trk[id] = CY_TCH;
		ts->prv_mt_pos[id][CY_XPOS] = t->cur_mt_pos[id][CY_XPOS];
		ts->prv_mt_pos[id][CY_YPOS] = t->cur_mt_pos[id][CY_YPOS];
		contacts_left++;
	}

	input_sync(ts->input_dev);

	//send_user_event(ts, ts->num_prv_st_tch, contacts_left);
	ts->num_prv_st_tch = contacts_left;
	
	input_report_key(ts->input_dev, BTN_TOUCH, contacts_left > 0);
	input_report_key(ts->input_dev, BTN_TOOL_FINGER, contacts_left >= 1);
	input_report_key(ts->input_dev, BTN_TOOL_DOUBLETAP, contacts_left >= 2);
	input_report_key(ts->input_dev, BTN_TOOL_TRIPLETAP, contacts_left == 3);
	
	return;
}


static void cnt_read_data(struct work_struct *work)
{

	//struct CNT_TS_DATA_T *data = i2c_get_clientdata(this_client);
//	struct CNT_TS_DATA_T *data = i2c_get_clientdata(this_client);    
//	int Touch_State = 0, temp_x0, temp_y0, dst_x, dst_y;

	union xy_data d;

#if 0
#ifdef SUPPORT_VIRTUAL_KEY						
	int key_id = 0x0, have_VK = 0;
#endif
#ifdef CNT_PROXIMITY
	int change_state = PROX_NORMAL;
#endif
	int i, ret = -1;
	int changed;
	static int need_report_pointup = 1;
#endif




//	struct cyttsp *ts = container_of(work, struct cyttsp, work);
	struct delayed_work *dw = container_of(work, struct delayed_work, work);
	struct CNT_TS_DATA_T *ts = container_of(dw,struct CNT_TS_DATA_T,pen_event_work);
	struct cyttsp_xydata xy_data;
	u8 id;
	struct cyttsp_track_data trc;
	s32 retval;

	
	DBG(printk(KERN_INFO "%s: Enter\n", __func__);)
	/* get event data from CYTTSP device */
	retval = cnt_i2c_rxdata(&xy_data, READ_DATA_SIZE);
//	retval = ttsp_read_block_data(ts, CY_REG_BASE,
//				      sizeof(xy_data), &xy_data);

	if (retval < 0) {
		printk(KERN_ERR "%s: Error, fail to read device on host interface bus\n",
			__func__);
		goto exit_xy_worker;
	}

#if 0
	printk("hst_mode   %d\n", xy_data.hst_mode);
	printk("tt_mode    %d\n", xy_data.tt_mode);
	printk("tt_stat    %d\n", xy_data.tt_stat);
	printk("x1         %d\n", xy_data.x1);
	printk("y1         %d\n", xy_data.y1);
	printk("z1         %d\n", xy_data.z1);
	printk("touch12_id %d\n", xy_data.touch12_id);
	printk("x2         %d\n", xy_data.x2);
	printk("y2         %d\n", xy_data.y2);
	printk("z2         %d\n", xy_data.z2);
#endif
	
	/* provide flow control handshake */
#if 0
	if (ts->irq > 0) {
		if (ts->platform_data->use_hndshk) {
			u8 cmd;
			cmd = xy_data.hst_mode & CY_HNDSHK_BIT ?
				xy_data.hst_mode & ~CY_HNDSHK_BIT :
				xy_data.hst_mode | CY_HNDSHK_BIT;
			retval = ttsp_write_block_data(ts, CY_REG_BASE,
						       sizeof(cmd), (u8 *)&cmd);
		}
	}
#endif

	trc.cur_tch = GET_NUM_TOUCHES(xy_data.tt_stat);

	if (0/*ts->platform_data->power_state == CY_IDLE_STATE*/)
		goto exit_xy_worker;
	else if (GET_BOOTLOADERMODE(xy_data.tt_mode)) {
#if 0
		/* TTSP device has reset back to bootloader mode */
		/* reset driver touch history */
		bool timeout;
		DBG3(printk(KERN_INFO
			"%s: Bootloader detected; reset driver\n",
			__func__);)
		cyttsp_init_tch(ts);
		if(ts->irq > 0) {
			free_irq(ts->irq, ts);
			ts->irq = -1;
		}
		cyttsp_soft_reset(ts, &timeout);
		cyttsp_exit_bl_mode(ts);
		cyttsp_set_operational_mode(ts);
		goto exit_xy_worker;
#endif
		;
	} else if (IS_LARGE_AREA(xy_data.tt_stat) == 1) {
		/* terminate all active tracks */
		trc.cur_tch = CY_NTCH;
		DBG(printk(KERN_INFO "%s: Large area detected\n",
			__func__);)
	} else if (trc.cur_tch > CY_NUM_MT_TCH_ID) {
		/* terminate all active tracks */
		trc.cur_tch = CY_NTCH;
		DBG(printk(KERN_INFO "%s: Num touch error detected\n",
			__func__);)
	} else if (IS_BAD_PKT(xy_data.tt_mode)) {
		/* terminate all active tracks */
		trc.cur_tch = CY_NTCH;
		DBG(printk(KERN_INFO "%s: Invalid buffer detected\n",
			__func__);)
	}

	/* clear current active track ID array and count previous touches */
	for (id = 0, trc.prv_tch = CY_NTCH; id < CY_NUM_TRK_ID; id++) {
		trc.cur_trk[id] = CY_NTCH;
		trc.prv_tch += ts->act_trk[id];
	}

	/* send no events if there were no previous touches */
	/* and no new touches */
	if ((trc.prv_tch == CY_NTCH) && ((trc.cur_tch == CY_NTCH) ||
				(trc.cur_tch > CY_NUM_MT_TCH_ID)))
		goto exit_xy_worker;

	DBG(printk(KERN_INFO "%s: prev=%d  curr=%d\n", __func__,
		   trc.prv_tch, trc.cur_tch);)

	/* clear current single-touch array */
	memset(trc.cur_st_tch, CY_IGNR_TCH, sizeof(trc.cur_st_tch));

	/* clear single touch positions */
	trc.st_x1 = trc.st_y1 = trc.st_z1 =
			trc.st_x2 = trc.st_y2 = trc.st_z2 = CY_NTCH;

	/* clear current multi-touch arrays */
	memset(trc.cur_mt_tch, CY_IGNR_TCH, sizeof(trc.cur_mt_tch));
	memset(trc.cur_mt_pos, CY_NTCH, sizeof(trc.cur_mt_pos));
	memset(trc.cur_mt_z, CY_NTCH, sizeof(trc.cur_mt_z));

#if 0
	if (trc.cur_tch) {
		unsigned i;
		u8 *pdata = (u8 *)&xy_data;

		printk(KERN_INFO "%s: TTSP data_pack: ", __func__);
		for (i = 0; i < sizeof(struct cyttsp_xydata); i++)
			printk(KERN_INFO "[%d]=0x%x ", i, pdata[i]);
		printk(KERN_INFO "\n");
	}
#endif

	/* process the touches */
	switch (trc.cur_tch) {
		/* do not break */
	case 2:
		xy_data.x2 = be16_to_cpu(xy_data.x2);
		xy_data.y2 = be16_to_cpu(xy_data.y2);
		id = GET_TOUCH2_ID(xy_data.touch12_id);
		if (1 || ts->platform_data->use_trk_id) {
			trc.cur_mt_pos[CY_MT_TCH2_IDX][CY_XPOS] = xy_data.x2;
			trc.cur_mt_pos[CY_MT_TCH2_IDX][CY_YPOS] = xy_data.y2;
			trc.cur_mt_z[CY_MT_TCH2_IDX] = xy_data.z2;
		}
		trc.cur_mt_tch[CY_MT_TCH2_IDX] = id;
		trc.cur_trk[id] = CY_TCH;
		if (ts->prv_st_tch[CY_ST_FNGR1_IDX] <	CY_NUM_TRK_ID) {
			if (ts->prv_st_tch[CY_ST_FNGR1_IDX] == id) {
				trc.st_x1 = xy_data.x2;
				trc.st_y1 = xy_data.y2;
				trc.st_z1 = xy_data.z2;
				trc.cur_st_tch[CY_ST_FNGR1_IDX] = id;
			} else if (ts->prv_st_tch[CY_ST_FNGR2_IDX] == id) {
				trc.st_x2 = xy_data.x2;
				trc.st_y2 = xy_data.y2;
				trc.st_z2 = xy_data.z2;
				trc.cur_st_tch[CY_ST_FNGR2_IDX] = id;
			}
		}
		DBG(printk(KERN_INFO"%s: 2nd XYZ:% 3d,% 3d,% 3d  ID:% 2d\n",
				__func__, xy_data.x2, xy_data.y2, xy_data.z2,
				(xy_data.touch12_id & 0x0F));)

		/* do not break */
	case 1:
		xy_data.x1 = be16_to_cpu(xy_data.x1);
		xy_data.y1 = be16_to_cpu(xy_data.y1);
		id = GET_TOUCH1_ID(xy_data.touch12_id);
		if (1 || ts->platform_data->use_trk_id) {
			trc.cur_mt_pos[CY_MT_TCH1_IDX][CY_XPOS] = xy_data.x1;
			trc.cur_mt_pos[CY_MT_TCH1_IDX][CY_YPOS] = xy_data.y1;
			trc.cur_mt_z[CY_MT_TCH1_IDX] = xy_data.z1;
		}
		trc.cur_mt_tch[CY_MT_TCH1_IDX] = id;
		trc.cur_trk[id] = CY_TCH;
		if (ts->prv_st_tch[CY_ST_FNGR1_IDX] <	CY_NUM_TRK_ID) {
			if (ts->prv_st_tch[CY_ST_FNGR1_IDX] == id) {
				trc.st_x1 = xy_data.x1;
				trc.st_y1 = xy_data.y1;
				trc.st_z1 = xy_data.z1;
				trc.cur_st_tch[CY_ST_FNGR1_IDX] = id;
			} else if (ts->prv_st_tch[CY_ST_FNGR2_IDX] == id) {
				trc.st_x2 = xy_data.x1;
				trc.st_y2 = xy_data.y1;
				trc.st_z2 = xy_data.z1;
				trc.cur_st_tch[CY_ST_FNGR2_IDX] = id;
			}
		}
		DBG(printk(KERN_INFO"%s: S1st XYZ:% 3d,% 3d,% 3d  ID:% 2d\n",
				__func__, xy_data.x1, xy_data.y1, xy_data.z1,
				((xy_data.touch12_id >> 4) & 0x0F));)

		break;
	case 0:
	default:
		break;
	}
	
	DBG(printk(KERN_ERR "x1 %d y1 %d x2 %d y2 %d ct %d", 
		xy_data.x1, xy_data.y1, xy_data.x2, xy_data.y2, trc.cur_tch);)
	
	input_report_abs(ts->input_dev, ABS_MT_TOUCH_MAJOR, 0);
	handle_multi_touch(&trc, ts);

	/* signal the view motion event */
	//input_sync(ts->input_dev);

	/* update platform data for the current multi-touch information */
	for (id = 0; id < CY_NUM_TRK_ID; id++)
		ts->act_trk[id] = trc.cur_trk[id];
		
exit_xy_worker:
	DBG(printk(KERN_INFO"%s: finished.\n", __func__);)
	return;



#if 0

	if (ret <= 0)
		printk("Error read\n");

	{
		static u8 copy_buf[READ_DATA_SIZE];
		
		changed = 0;
		for (i = 0; i < READ_DATA_SIZE; i++) {
			changed |= copy_buf[i] != d.buf[i];
			copy_buf[i] = d.buf[i];
		}
		{
		// pr_info("Touch data %#hhx %#hhx %#hhx %#hhx %#hhx %#hhx %#hhx %#hhx\n", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7]);
			pr_info("Touch data");
			for (i = 0; i < READ_DATA_SIZE; i++)
				printk(" %#hhx", d.buf[i]);
			printk("\n");
		}
	}

	if (ret <=  0)
		return 0;     
#ifdef CNT_PROXIMITY
	if (prox_enable == 1) {
		//if android phone is in call
		if (d.buf[0] == 0x5A)
			change_state = PROX_CLOSE;
		else if (d.buf[0] == 0x5E)
			change_state = PROX_OFF;

		if (change_state == PROX_CLOSE) {
			//chage backlight state
			printk("[TSP]:%s, prox close\n", __func__);
			input_report_abs(prox_input, ABS_DISTANCE, 0);
		}
		else if (change_state == PROX_OFF) {
			printk("[TSP]:%s, prox off\n", __func__);
			input_report_abs(prox_input, ABS_DISTANCE, 1);
		}
	}
#endif

	//Judge Touch State
	if ((d.buf[0] != 0x52) || (Calculate_8BitsChecksum(d.buf, 7) != d.buf[7])) {
		//Check data packet ID & Check the data if valid
		return 0;
	}
	else if ((d.buf[1]== 0xFF) && (d.buf[2]== 0xFF) && 
		 (d.buf[3]== 0xFF) && (d.buf[4]== 0xFF) && 
		 (d.buf[6]== 0xFF)) {
		//Scan finger number on panel

		if ((d.buf[5]== 0x0) || (d.buf[5]== 0xFF)) {
			Touch_State = Touch_State_No_Touch;
		}
#ifdef SUPPORT_VIRTUAL_KEY				
		else {
			Touch_State = Touch_State_VK;
			key_id = d.buf[5] & 0x1F;
		}
#endif
	}
	else {
		if ((d.buf[4]== 0x0) && (d.buf[5]== 0x0) && (d.buf[6]== 0x0))
			Touch_State = Touch_State_One_Finger;
		else
			Touch_State = Touch_State_Two_Finger;	
	} 

	temp_x0 = ((d.buf[1] & 0xF0) << 4) | d.buf[2];
	temp_y0 = ((d.buf[1] & 0x0F) << 8) | d.buf[3];
	dst_x = ((d.buf[4] & 0xF0) << 4) | d.buf[5];
	dst_y = ((d.buf[4] & 0x0F) <<8 ) | d.buf[6];

	pr_info("Touch state %d\n", Touch_State);

	if (Touch_State == Touch_State_One_Finger) {
		/*Mapping CNT touch coordinate to Android coordinate*/

		// SWAP and mirror
		_st_finger_infos[0].i2_y = SCREEN_MAX_X - (temp_x0*SCREEN_MAX_X) / CNT_RESOLUTION_X ;
		_st_finger_infos[0].i2_x = (temp_y0*SCREEN_MAX_Y) / CNT_RESOLUTION_Y ;

		pr_info("Reporting one finger touch %d %d\n", _st_finger_infos[0].i2_x, _st_finger_infos[0].i2_y);

		input_report_abs(data->input_dev, ABS_MT_TOUCH_MAJOR, 1);        	
		input_report_abs(data->input_dev, ABS_MT_POSITION_X, _st_finger_infos[0].i2_x);
		input_report_abs(data->input_dev, ABS_MT_POSITION_Y, _st_finger_infos[0].i2_y);
		input_mt_sync(data->input_dev);	
		input_sync(data->input_dev);
		need_report_pointup = 1;
	}
	else if (Touch_State == Touch_State_Two_Finger) {
		/*Mapping CNT touch coordinate to Android coordinate*/
		if (dst_x > 2048) 
			dst_x -= 4096;//transform the unsigh value to sign value
		if (dst_y > 2048)
			dst_y -= 4096;//transform the unsigh value to sign value

		// SWAP and mirror
		_st_finger_infos[0].i2_y = SCREEN_MAX_X - (temp_x0*SCREEN_MAX_X) / CNT_RESOLUTION_X ;
		_st_finger_infos[0].i2_x = (temp_y0*SCREEN_MAX_Y) / CNT_RESOLUTION_Y ;

//  		_st_finger_infos[1].i2_x = ((temp_x0 + dst_x)*SCREEN_MAX_X) / CNT_RESOLUTION_X ;
//  		_st_finger_infos[1].i2_y = ((temp_y0 + dst_x)*SCREEN_MAX_Y) / CNT_RESOLUTION_Y ;

		_st_finger_infos[1].i2_y = SCREEN_MAX_X - ((temp_x0 + dst_x)*SCREEN_MAX_X) / CNT_RESOLUTION_X ;
		_st_finger_infos[1].i2_x = ((temp_y0 + dst_y)*SCREEN_MAX_Y) / CNT_RESOLUTION_Y ;


		pr_info("Reporting two finger touch %d %d %d %d\n", 
		        _st_finger_infos[0].i2_x,
			_st_finger_infos[0].i2_y,
			_st_finger_infos[1].i2_x,
			_st_finger_infos[1].i2_y);

		/*report first point*/
		input_report_abs(data->input_dev, ABS_MT_TOUCH_MAJOR, 1);
		input_report_abs(data->input_dev, ABS_MT_POSITION_X, _st_finger_infos[0].i2_x);
		input_report_abs(data->input_dev, ABS_MT_POSITION_Y, _st_finger_infos[0].i2_y);
		input_mt_sync(data->input_dev);

		/*report second point*/
		input_report_abs(data->input_dev, ABS_MT_TOUCH_MAJOR, 1);
		input_report_abs(data->input_dev, ABS_MT_POSITION_X, _st_finger_infos[1].i2_x);
		input_report_abs(data->input_dev, ABS_MT_POSITION_Y, _st_finger_infos[1].i2_y);
		input_mt_sync(data->input_dev);

		input_sync(data->input_dev);
		need_report_pointup = 1;
	}
#ifdef SUPPORT_VIRTUAL_KEY					
	else if (Touch_State == Touch_State_VK) {
		/*you can implement your VK releated function here*/	
		if (key_id == 0x01) {
			input_report_key(data->input_dev, tsp_keycodes[0], 1);
		}
		else if(key_id = 0x02) {
			input_report_key(data->input_dev, tsp_keycodes[1], 1);
		}
		else if(key_id == 0x04) {
			input_report_key(data->input_dev, tsp_keycodes[2], 1);
		}
		else if(key_id == 0x08) {
			input_report_key(data->input_dev, tsp_keycodes[3], 1);
		}
		input_report_key(data->input_dev, BTN_TOUCH, 1);
		have_VK = 1;		
		need_report_pointup = 1;
	}
#endif
	else {
		/*Finger up*/
#ifdef SUPPORT_VIRTUAL_KEY						
		if (have_VK==1) {
			if(key_id == 0x01) {
				input_report_key(data->input_dev, tsp_keycodes[0], 0);
			}
			else if(key_id == 0x02) {
				input_report_key(data->input_dev, tsp_keycodes[1], 0);
			}
			else if(key_id == 0x04) {
				input_report_key(data->input_dev, tsp_keycodes[2], 0);
			}
			else if(key_id == 0x08) {
				input_report_key(data->input_dev, tsp_keycodes[3], 0);
			}
			input_report_key(data->input_dev, BTN_TOUCH, 0);
			have_VK = 0;
			need_report_pointup = 1;
		}
#endif	

		if (need_report_pointup) {
			input_report_abs(data->input_dev, ABS_MT_TOUCH_MAJOR, 0);
			input_mt_sync(data->input_dev);
			input_sync(data->input_dev);
			need_report_pointup = 0;
			pr_info("Reported pointup\n");
		}
	}

	return 1;
#endif
}


static void cnt_ts_pen_irq_work(struct work_struct *work)
{
//	int ret = -1;
//	static int retry_counter = 0;
	struct delayed_work *dw = container_of(work, struct delayed_work, work);
	struct CNT_TS_DATA_T *cnt_ts = container_of(dw,struct CNT_TS_DATA_T,pen_event_work);

	printk("Touch work\n");

	if (cnt_ts->in_suspend & LOCK_MODE)
		return;

//	if (this_client->dev.power.is_suspended) {
	//our_adapter
//	if (cnt_touch_suspend) {
//	if (our_adapter->dev.power.is_suspended) 

	if (cnt_ts->in_suspend & SUSPEND_MODE) {
		// defer read till woken up
		queue_delayed_work(cnt_ts->ts_workqueue, &cnt_ts->pen_event_work,5);
	}
	else {
		cnt_read_data(work);
	}
}

static irqreturn_t cnt_ts_interrupt(int irq, void *dev_id)
{
	struct CNT_TS_DATA_T *cnt_ts = dev_id;
		
	print_int_info("==========------cnt_ts TS Interrupt-----============\n"); 

	if (ctp_ops.judge_int_occur()) {
		print_int_info("Other Interrupt\n");
		return IRQ_NONE;
	}

//	print_int_info("==IRQ_EINT%d=\n",CTP_IRQ_NO);
	ctp_ops.clear_penirq();
	if (!delayed_work_pending(&cnt_ts->pen_event_work)) {
		print_int_info("Enter work %i\n",this_client->dev.power.is_suspended);
		queue_delayed_work(cnt_ts->ts_workqueue, &cnt_ts->pen_event_work,1);
	}

	return IRQ_HANDLED;
}

#if defined(CONFIG_PM)

extern suspend_state_t g_suspend_state;
extern __u32 standby_wakeup_event;

static void cnt_ts_off(void) {
	// turn off touch
	if(gpio_reset_enable){
		pr_info("%s. \n", __func__);
		if(EGPIO_SUCCESS != gpio_write_one_pin_value(gpio_reset_hdle, 0, "ctp_reset")){
			pr_info("%s: err when operate gpio. \n", __func__);
		}
	}
}

static int cnt_ts_suspend(struct i2c_client *client, pm_message_t mesg) {
	static uint8_t delay_mode_pkt[4] = {0xff, 0x11, 0xff, 0x01};
	int res;

	struct CNT_TS_DATA_T *cnt_ts = i2c_get_clientdata(client);
	printk("[%s] msg = %i\n",__func__,g_suspend_state);

	if ((g_suspend_state == PM_SUSPEND_PARTIAL) && !(cnt_ts->in_suspend & LOCK_MODE)) {
		standby_wakeup_event |= SUSPEND_WAKEUP_SRC_PIO;
		// inform touch about suspend
		pr_info("%s set touch delay mode\n", __func__);
		res = cnt_i2c_txdata(delay_mode_pkt, sizeof(delay_mode_pkt));
		pr_info("%s dleay mode result %d\n", __func__, res);

	} else {
		cnt_ts_off();

		cnt_ts->in_suspend |= SUSPEND_MODE;
	}
	return 0;
}

static int cnt_ts_resume(struct i2c_client *client) {

	struct CNT_TS_DATA_T *cnt_ts = i2c_get_clientdata(client);
	//printk("[%s] in suspend = %i\n",__func__,cnt_ts->in_suspend);
	if ((cnt_ts->in_suspend & SUSPEND_MODE) && !(cnt_ts->in_suspend & LOCK_MODE))	{
		ctp_ops.ts_reset();
	}
	cnt_ts->in_suspend = 0;

	return 0;
}
#endif

static int proc_keylock_read(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	struct CNT_TS_DATA_T *cnt_ts = (struct CNT_TS_DATA_T *)data;
	*eof = 1;
	if (cnt_ts)
		return snprintf(page, PAGE_SIZE, "%u\n", (cnt_ts->in_suspend & LOCK_MODE) ? 1 : 0);
	else
		return snprintf(page, PAGE_SIZE, "ERROR\n");
}

static int proc_keylock_write(struct file *file, const char __user *buffer, unsigned long count, void *data)
{
	char buf[16];
	struct CNT_TS_DATA_T *cnt_ts = (struct CNT_TS_DATA_T *)data;

	if (count > sizeof(buf) -1 )
		return -EINVAL;

	if (!count)
		return 0;

	if (copy_from_user(buf, buffer, count))
		return -EFAULT;

	buf[count] = '\0';

	switch (buf[0]) {
		case '0':
			ctp_ops.ts_reset();
			if (cnt_ts)
				cnt_ts->in_suspend &= ~(SUSPEND_MODE | LOCK_MODE);
			break;
		default:
			cnt_ts_off();
			if (cnt_ts)
				cnt_ts->in_suspend |= SUSPEND_MODE | LOCK_MODE;
			break;
	}

	return count;
}

int cnt_ts_lock_init(struct CNT_TS_DATA_T *cnt_ts)
{
	struct proc_dir_entry *dir, *file;

	dir = proc_mkdir("keylock", NULL);
	if (!dir) {
		printk("could not create /proc/keylock\n");
		return -1;
	}

	file = create_proc_entry("lock", S_IRUGO | S_IWUGO, dir);
	if (!file) {
		printk("could not create /proc/keylock/lock\n");
		return -1;
	}

	file->data = cnt_ts;
	file->read_proc = proc_keylock_read;
	file->write_proc = proc_keylock_write;

	return 0;
}


static int 
cnt_ts_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	struct CNT_TS_DATA_T *cnt_ts;
	struct input_dev *input_dev;
	struct device *dev;
	struct i2c_dev *i2c_dev;
	int err = 0;

	pr_info("====%s begin=====.  \n", __func__);
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		err = -ENODEV;
		goto exit_check_functionality_failed;
	}

	cnt_ts = kzalloc(sizeof(*cnt_ts), GFP_KERNEL);
	if (!cnt_ts)	{
		err = -ENOMEM;
		goto exit_alloc_data_failed;
	}

	pr_info("touch panel gpio addr: = 0x%x", gpio_addr);
	this_client = client;
	
	pr_info("cnt_ts_probe : client->addr = %d. \n", client->addr);
	this_client->addr = client->addr;
	pr_info("cnt_ts_probe : client->addr = %d. \n", client->addr);
	i2c_set_clientdata(client, cnt_ts);

	pr_info("==INIT_WORK=\n");
	INIT_DELAYED_WORK(&cnt_ts->pen_event_work, cnt_ts_pen_irq_work);
	cnt_ts->ts_workqueue = create_singlethread_workqueue(dev_name(&client->dev));
	if (!cnt_ts->ts_workqueue) {
		err = -ESRCH;
		goto exit_create_singlethread;
	}

	input_dev = input_allocate_device();
	if (!input_dev) {
		err = -ENOMEM;
		dev_err(&client->dev, "failed to allocate input device\n");
		goto exit_input_dev_alloc_failed;
	}
	
	cnt_ts->input_dev = input_dev;

	set_bit(ABS_MT_TOUCH_MAJOR, input_dev->absbit);
	set_bit(ABS_MT_POSITION_X, input_dev->absbit);
	set_bit(ABS_MT_POSITION_Y, input_dev->absbit);
	set_bit(ABS_MT_WIDTH_MAJOR, input_dev->absbit);	
	input_set_abs_params(input_dev,
			     ABS_MT_POSITION_X, 0, SCREEN_MAX_X, 0, 0);
	input_set_abs_params(input_dev,
			     ABS_MT_POSITION_Y, 0, SCREEN_MAX_Y, 0, 0);
	input_set_abs_params(input_dev,
			     ABS_MT_TOUCH_MAJOR, 0, PRESS_MAX, 0, 0);
	input_set_abs_params(input_dev,
			     ABS_MT_WIDTH_MAJOR, 0, 200, 0, 0);
	input_set_abs_params(input_dev,
			     ABS_MT_TRACKING_ID, 0, 4, 0, 0);

	set_bit(EV_ABS, input_dev->evbit);
	set_bit(EV_KEY, input_dev->evbit);

	input_dev->name		= CTP_NAME;		//dev_name(&client->dev)
	err = input_register_device(input_dev);
	if (err) {
		dev_err(&client->dev,
		"cnt_ts_probe: failed to register input device: %s\n",
		dev_name(&client->dev));
		goto exit_input_register_device_failed;
	}

	err = ctp_ops.set_irq_mode("ctp_para", "ctp_int_port", CTP_IRQ_MODE);
	if(0 != err){
		pr_info("%s:ctp_ops.set_irq_mode err. \n", __func__);
		goto exit_set_irq_mode;
	}

	err = request_irq(SW_INT_IRQNO_PIO, cnt_ts_interrupt, IRQF_SHARED, "cnt_ts", cnt_ts);
	if (err < 0) {
		dev_err(&client->dev, "cnt_ts_probe: request irq failed\n");
		goto exit_irq_request_failed;
	}

	if (gpio_int_hdle) {
		//enable gpio eint trigger;
		sw_gpio_eint_set_enable(gpio_int_hdle,1);

		//clear trigger state
		sw_gpio_eint_clr_irqpd_sta(gpio_int_hdle);
	}

	i2c_dev = get_free_i2c_dev(client->adapter);
	if (IS_ERR(i2c_dev)){	
		err = PTR_ERR(i2c_dev);		
		return err;	
	}
	dev = device_create(i2c_dev_class, &client->adapter->dev, MKDEV(I2C_MAJOR,client->adapter->nr), NULL, "aw_i2c_ts%d", client->adapter->nr);	
	if (IS_ERR(dev))	{		
			err = PTR_ERR(dev);		
			return err;	
	}

	cnt_ts_lock_init(cnt_ts);

#if MSG2133_UPDATE
	msg2133_init_fw_class(cnt_ts);
#endif

	return 0;

exit_irq_request_failed:
exit_set_irq_mode:
	cancel_delayed_work_sync(&cnt_ts->pen_event_work);
	destroy_workqueue(cnt_ts->ts_workqueue);
	enable_irq(SW_INT_IRQNO_PIO);
exit_input_register_device_failed:
	input_free_device(input_dev);
exit_input_dev_alloc_failed:
	free_irq(SW_INT_IRQNO_PIO, cnt_ts);
exit_create_singlethread:
	pr_info("==singlethread error =\n");
	i2c_set_clientdata(client, NULL);
	kfree(cnt_ts);
exit_alloc_data_failed:
exit_check_functionality_failed:
	return err;
}

static int __devexit cnt_ts_remove(struct i2c_client *client)
{

	struct CNT_TS_DATA_T *cnt_ts = i2c_get_clientdata(client);
	// power off touch
	if(gpio_reset_enable){
		pr_info("%s. \n", __func__);
		if(EGPIO_SUCCESS != gpio_write_one_pin_value(gpio_reset_hdle, 0, "ctp_reset")){
			pr_info("%s: err when operate gpio. \n", __func__);
		}
	}
	
	pr_info("==cnt_ts_remove=\n");
	free_irq(SW_INT_IRQNO_PIO, cnt_ts);
	input_unregister_device(cnt_ts->input_dev);
	input_free_device(cnt_ts->input_dev);
	cancel_delayed_work_sync(&cnt_ts->pen_event_work);
	destroy_workqueue(cnt_ts->ts_workqueue);
	kfree(cnt_ts);
    
	i2c_set_clientdata(client, NULL);
	ctp_ops.free_platform_resource();

	return 0;

}

static const struct i2c_device_id cnt_ts_id[] = {
	{ CTP_NAME, 0 },
	{}
};
MODULE_DEVICE_TABLE(i2c, cnt_ts_id);

static struct i2c_driver cnt_ts_driver = {
	.class = I2C_CLASS_HWMON,
	.probe		= cnt_ts_probe,
	.remove		= __devexit_p(cnt_ts_remove),
	.id_table	= cnt_ts_id,
	.driver	= {
		.name	= CTP_NAME,
		.owner	= THIS_MODULE,
	},
	.address_list	= u_i2c_addr.normal_i2c,
#ifdef CONFIG_PM
	.suspend	= cnt_ts_suspend,
	.resume		= cnt_ts_resume,
#endif
};

/*
 * TP-INT - PG11
 * TP-WAKE-RST - PB3
 *
 */
static int __init cnt_ts_init(void)
{ 
	int ret = -1;
	int err = -1;

	pr_info("===========================%s=====================\n", __func__);

	if (ctp_ops.fetch_sysconfig_para && ctp_ops.fetch_sysconfig_para()) {
		pr_info("%s: err.\n", __func__);
		return -1;
	}
	pr_info("%s: normal_i2c[0]: 0x%hx. normal_i2c[1]: 0x%hx \n", \
	        __func__, u_i2c_addr.normal_i2c[0], u_i2c_addr.normal_i2c[1]);

	err = ctp_ops.init_platform_resource();
	if(0 != err){
	    pr_info("%s:ctp_ops.init_platform_resource err. \n", __func__);    
	}

	//reset
	ctp_ops.ts_reset();
	//wakeup
	ctp_ops.ts_wakeup();  
	
	cnt_ts_driver.detect = ctp_ops.ts_detect;

	i2c_dev_class = class_create(THIS_MODULE,"aw_i2c_dev");
	if (IS_ERR(i2c_dev_class)) {
		ret = PTR_ERR(i2c_dev_class);
		class_destroy(i2c_dev_class);
	}

	ret = i2c_add_driver(&cnt_ts_driver);

	return ret;
}

static void __exit cnt_ts_exit(void)
{
	pr_info("==cnt_ts_exit==\n");
	i2c_del_driver(&cnt_ts_driver);
}

late_initcall(cnt_ts_init);
module_exit(cnt_ts_exit);

MODULE_AUTHOR("Dushes");
MODULE_DESCRIPTION("CNT TouchScreen driver");
MODULE_LICENSE("GPL");

