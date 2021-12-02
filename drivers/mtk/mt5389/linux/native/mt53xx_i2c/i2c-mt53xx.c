/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: i2c-mt53xx.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file i2c-mt53xx
 *  MTK MT53xx I2C driver.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <asm/uaccess.h>
#include <linux/ioport.h>
#include <linux/errno.h>
#include <linux/sched.h>

#include <linux/i2c.h>

#include <x_typedef.h>
//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define MODULE_NAME "MTK I2C"

//#define	I2C_MTK_DEBUG
#ifdef I2C_MTK_DEBUG
static int i2c_debug = 0;
#define DEB0(format, arg...)	printk(KERN_DEBUG MODULE_NAME " DEBUG: " format "\n",  ## arg )
#define DEB1(format, arg...)	\
	if (i2c_debug>=1) {	\
		printk(KERN_DEBUG MODULE_NAME " DEBUG: " format "\n",  ## arg ); \
	}
#define DEB2(format, arg...)	\
	if (i2c_debug>=2) {	\
		printk(KERN_DEBUG MODULE_NAME " DEBUG: " format "\n",  ## arg ); \
	}
#define DEB3(format, arg...)	\
	if (i2c_debug>=3) {	\
		printk(KERN_DEBUG MODULE_NAME " DEBUG: " format "\n",  ## arg ); \
	}
#define DEB9(format, arg...)	\
	/* debug the protocol by showing transferred bits */	\
	if (i2c_debug>=9) {	\
		printk(KERN_DEBUG MODULE_NAME " DEBUG: " format "\n",  ## arg ); \
	}
#else
#define DEB0(fmt, args...)
#define DEB1(fmt, args...)
#define DEB2(fmt, args...)
#define DEB3(fmt, args...)
#define DEB9(fmt, args...)
#endif

#define err(format, arg...) printk(KERN_ERR MODULE_NAME " ERROR: " format "\n",  ## arg )
#define info(format, arg...) printk(KERN_INFO MODULE_NAME ": " format "\n",  ## arg )
#define warn(format, arg...) printk(KERN_WARNING MODULE_NAME " WARNING: " format "\n",  ## arg )
#define emerg(format, arg...) printk(KERN_EMERG MODULE_NAME " EMERGENCY: " format "\n",  ## arg )

#define MAX_MESSAGES	65536	/* max number of messages */

//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------
extern UINT16 SIF_ReadNoSubAddr(UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 *pu1Buf,
			UINT16 u2ByteCnt);
extern UINT16 SIF_WriteNoSubAddr(UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 *pu1Buf,
                 UINT16 u2ByteCnt);

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static unsigned short _i4MTK_i2c_div = 0x100;		/* Default: Fast Mode = 400 KHz, Standard Mode = 100 KHz */

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static int
mtk_i2c_xfer_msg(struct i2c_adapter *adap, struct i2c_msg *msg)
{
        u8 zero_byte = 0;
	int r;
	u8 *buf;
	size_t buf_len;

	DEB2("addr: 0x%04x, len: %d, flags: 0x%x",
	     msg->addr, msg->len, msg->flags);

	/* Sigh, seems we can't do zero length transactions. Thus, we
	 * can't probe for devices w/o actually sending/receiving at least
	 * a single byte. So we'll set count to 1 for the zero length
	 * transaction case and hope we don't cause grief for some
	 * arbitrary device due to random byte write/read during
	 * probes.
	 */
	if (msg->len == 0) {
		buf = &zero_byte;
                buf_len = 1;
	} else {
		buf = msg->buf;
		buf_len = msg->len;
	}

	if (msg->flags & I2C_M_TEN)
	{
	    return -EIO;
        }

	if (!(msg->flags & I2C_M_RD))
	{
            r = SIF_WriteNoSubAddr(_i4MTK_i2c_div, (msg->addr << 1), buf, buf_len);
	}
        else
        {
            r = SIF_ReadNoSubAddr(_i4MTK_i2c_div, (msg->addr << 1), buf, buf_len);
        }

	if (r != buf_len)
	{
	    return -EIO;
	}

        return msg->len;
}

/*
 * Prepare controller for a transaction and call mtk_i2c_rxbytes
 * to do the work.
 */
 static int
mtk_i2c_xfer(struct i2c_adapter *adap, struct i2c_msg msgs[], int num)
{
	int i;
	int r = 0;

	DEB1("msgs: %d", num);

	if (num < 1 || num > MAX_MESSAGES)
		return -EINVAL;

	/* Check for valid parameters in messages */
	for (i = 0; i < num; i++)
		if (msgs[i].buf == NULL)
			return -EINVAL;

	for (i = 0; i < num; i++) {
		DEB2("msg: %d, addr: 0x%04x, len: %d, flags: 0x%x",
		     i, msgs[i].addr, msgs[i].len, msgs[i].flags);

		r = mtk_i2c_xfer_msg(adap, &msgs[i]);

		DEB2("r: %d", r);

		if (r != msgs[i].len)
			break;
	}

	if (r >= 0 && num > 1)
		r = num;

	DEB1("r: %d", r);

	return r;
}

static u32
mtk_i2c_func(struct i2c_adapter *adap)
{
	return I2C_FUNC_I2C | I2C_FUNC_SMBUS_EMUL;
}

static int mtk_i2c_remove(struct device *dev)
{
        return 0;
}

static void mtk_i2c_device_release(struct device *dev)
{
        /* Nothing */
}

static struct i2c_algorithm mtk_i2c_algo = {
	.name = "MTK I2C algorithm",
	.id = I2C_ALGO_EXP,
	.master_xfer = mtk_i2c_xfer,
	.functionality = mtk_i2c_func,
};

static struct i2c_adapter mtk_i2c_adap = {
	.owner = THIS_MODULE,
	.name = "MTK I2C adapter",
	.id = I2C_ALGO_EXP,	/* REVISIT: register for id */
	.algo = &mtk_i2c_algo,
};

static struct device_driver mtk_i2c_driver = {
        .name           = "mtk_i2c",
        .bus            = &platform_bus_type,
        .remove         = mtk_i2c_remove,
};

static struct platform_device mtk_i2c_device = {
        .name       = "i2c",
        .id             = -1,
        .dev = {
                .driver         = &mtk_i2c_driver,
                .release        = mtk_i2c_device_release,
        },
};

int __init
mtk_i2c_init(void)
{
	int r = 0;

	//info("Driver ver. 1.0");
	DEB0("%s %s", __TIME__, __DATE__);

	r = i2c_add_adapter(&mtk_i2c_adap);
	if (r) {
		err("failed to add adapter");
		return r;
	}

	if(driver_register(&mtk_i2c_driver) != 0)
		printk(KERN_ERR "Driver register failed for mtk_i2c\n");

	if(platform_device_register(&mtk_i2c_device) != 0) {
		printk(KERN_ERR "Device register failed for i2c\n");
		driver_unregister(&mtk_i2c_driver);
	}

        return r;
}

static void __exit
mtk_i2c_exit(void)
{
	i2c_del_adapter(&mtk_i2c_adap);
        driver_unregister(&mtk_i2c_driver);
        platform_device_unregister(&mtk_i2c_device);
}

MODULE_AUTHOR("Max Liao, MTK Inc.");
MODULE_DESCRIPTION("MTK MT53XX I2C bus adapter");
MODULE_LICENSE("GPL");

#ifdef I2C_MTK_DEBUG
module_param(i2c_debug, int, 0);
MODULE_PARM_DESC(i2c_debug,
		 "debug level - 0 off; 1 normal; 2,3 more verbose; "
		 "9 mtk-protocol");
#endif

/* i2c may be needed to bring up other drivers */
//subsys_initcall(mtk_i2c_init);
//module_exit(mtk_i2c_exit);
