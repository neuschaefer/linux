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
#ifndef _UTEST_ENUMERATION_H
#define _UTEST_ENUMERATION_H
/*==========================================================================
 *
 *      Library Inclusion Segment
 *
 *==========================================================================
 */

#include "musbdefs.h"
#include "mu_dev.h"

#define MUSB_MAX_RETRIES        8

// EP0 Transaction State
#define MGC_END0_SETUP                    0x10       // setup stage
#define MGC_END0_DATA                     0x20       // data stage
#define MGC_END0_STATUS                  0x40       // handshake stage
#define MGC_END0_FINISH                   0x80      // END0 enumeration done
// EP0 Data Stage
#define MGC_END0_DSTAGE_DATAIN     0x21
#define MGC_END0_DSTAGE_DATAOUT   0x22
#define MGC_END0_DSTAGE_NODATA    0x23


// Billionton Definition
#define AX_CMD_SET_SW_MII               0x06
#define AX_CMD_READ_MII_REG             0x07
#define AX_CMD_WRITE_MII_REG            0x08
#define AX_CMD_READ_MII_OPERATION_MODE      0x09
#define AX_CMD_SET_HW_MII               0x0a
#define AX_CMD_READ_EEPROM              0x0b
#define AX_CMD_WRITE_EEPROM             0x0c
#define AX_CMD_READ_RX_CONTROL_REG             0x0f
#define AX_CMD_WRITE_RX_CTL             0x10
#define AX_CMD_READ_IPG012              0x11
#define AX_CMD_WRITE_IPG0               0x12
#define AX_CMD_WRITE_IPG1               0x13
#define AX_CMD_WRITE_IPG2               0x14
#define AX_CMD_READ_MULTIFILTER_ARRAY           0x15
#define AX_CMD_WRITE_MULTI_FILTER       0x16
#define AX_CMD_READ_NODE_ID             0x17
#define AX_CMD_READ_PHY_ID                  0x19
#define AX_CMD_READ_MEDIUM_MODE    0x1a
#define AX_CMD_WRITE_MEDIUM_MODE        0x1b
#define AX_CMD_READ_MONITOR_MODE        0x1c
#define AX_CMD_WRITE_MONITOR_MODE       0x1d
#define AX_CMD_READ_GPIOS                   0x1e
#define AX_CMD_WRITE_GPIOS                  0x1f

#define AX_CMD_GUSBKR3_BREQ         0x05
#define AX_CMD_GUSBKR3_12e          0x12e
#define AX_CMD_GUSBKR3_120          0x120
#define AX_CMD_GUSBKR3_126          0x126
#define AX_CMD_GUSBKR3_134          0x134
#define AX_CMD_GUSBKR3_12f          0x12f
#define AX_CMD_GUSBKR3_130          0x130
#define AX_CMD_GUSBKR3_137          0x137
#define AX_CMD_GUSBKR3_02           0x02
#define AX_CMD_GUSBKR3_13e          0x13e

typedef struct enumeration_s
{
    unsigned char* pDesciptor;
    MUSB_DeviceRequest sDevReq;
} enumeration_t;

typedef struct ethenumeration_s
{
    unsigned char* pDesciptor;
    MUSB_DeviceRequest sDevReq;
} ethenumeration_t;

#if 0

static ethenumeration_t ethenumeration_step[] = {
    {"MUSB_REQ_VENDER_12e",  //out 1 byte
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_GUSBKR3_BREQ),
          (AX_CMD_GUSBKR3_12e),
          (0),
          (1)
        }
    },
    {"MUSB_REQ_VENDER_120",  //in 6 byte
        {(MUSB_DIR_IN|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_GUSBKR3_BREQ),
          (AX_CMD_GUSBKR3_120),
          (0),
          (6)
        }
    },
    {"MUSB_REQ_VENDER_126",  //out 8 byte
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_GUSBKR3_BREQ),
          (AX_CMD_GUSBKR3_126),
          (0),
          (8)
        }
    },
    {"MUSB_REQ_VENDER_134",  //out 1 byte
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_GUSBKR3_BREQ),
          (AX_CMD_GUSBKR3_134),
          (0),
          (1)
        }
    },
    {"MUSB_REQ_VENDER_134",  //in 1 byte
        {(MUSB_DIR_IN|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_GUSBKR3_BREQ),
          (AX_CMD_GUSBKR3_134),
          (0),
          (1)
        }
    },
    {"MUSB_REQ_VENDER_120",  //out 6 byte
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_GUSBKR3_BREQ),
          (AX_CMD_GUSBKR3_120),
          (0),
          (6)
        }
    },
    {"MUSB_REQ_VENDER_12f",  //out 1 byte
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_GUSBKR3_BREQ),
          (AX_CMD_GUSBKR3_12f),
          (0),
          (1)
        }
    },
    {"MUSB_REQ_VENDER_130",  //out 2 byte
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_GUSBKR3_BREQ),
          (AX_CMD_GUSBKR3_130),
          (0),
          (2)
        }
    },
    {"MUSB_REQ_VENDER_137",  //in 1 byte
        {(MUSB_DIR_IN|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_GUSBKR3_BREQ),
          (AX_CMD_GUSBKR3_137),
          (0),
          (1)
        }
    },
    {"MUSB_REQ_VENDER_12e",  //out 1 byte
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_GUSBKR3_BREQ),
          (AX_CMD_GUSBKR3_12e),
          (0),
          (1)
        }
    },
    {"MUSB_REQ_VENDER_02",  //in 1 byte
        {(MUSB_DIR_IN|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_GUSBKR3_BREQ),
          (AX_CMD_GUSBKR3_02),
          (0),
          (1)
        }
    },
    {"MUSB_REQ_VENDER_02",  //out 1 byte
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_GUSBKR3_BREQ),
          (AX_CMD_GUSBKR3_02),
          (0),
          (1)
        }
    },
    {"MUSB_REQ_VENDER_13e",  //out 1 byte
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_GUSBKR3_BREQ),
          (AX_CMD_GUSBKR3_13e),
          (0),
          (1)
        }
    },
    {"MUSB_REQ_VENDER_13e",  //in 1 byte
        {(MUSB_DIR_IN|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_GUSBKR3_BREQ),
          (AX_CMD_GUSBKR3_13e),
          (0),
          (1)
        }
    },
    {"MUSB_REQ_VENDER_130",  //out 2 byte
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_GUSBKR3_BREQ),
          (AX_CMD_GUSBKR3_130),
          (0),
          (2)
        }
    },
    {NULL, 
        {0,
          (0),
          (0),
          (0),
          (0)
        }
    },
};
#endif

#if 1
ethenumeration_t ethenumeration_step[] = {
    {"MUSB_REQ_VENDER_1F_1", //nodata
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_WRITE_GPIOS),
          (0x13),
          (0),
          (0)
        }
    },

    {"MUSB_REQ_VENDER_1F_2", //nodata
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_WRITE_GPIOS),
          (0x01),
          (0),
          (0)
        }
    },

    {"MUSB_REQ_VENDER_1F_3",  //nodata
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_WRITE_GPIOS),
          (0x03),
          (0),
          (0)
        }
    },

    {"MUSB_REQ_VENDER_10_4", //nodata
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_WRITE_RX_CTL),
          (0x80),
          (0),
          (0)
        }
    },

    {"MUSB_REQ_VENDER_17_5", //data in
        {(MUSB_DIR_IN|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_READ_NODE_ID),
          (0),
          (0),
          (0x06)
        }
    },

    {"MUSB_REQ_VENDER_19_6", // data in
        {(MUSB_DIR_IN|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_READ_PHY_ID),
          (0),
          (0),
          (0x02)
        }
    },

    {"MUSB_REQ_VENDER_06_7", // no data
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_SET_SW_MII),
          (0),
          (0),
          (0)
        }
    },
#if 1
    {"MUSB_REQ_VENDER_08_8", //data out
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_WRITE_MII_REG),
          (0x03),
          (0),
          (0x02)
        }
    },
#endif
    {"MUSB_REQ_VENDER_0A_9", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_SET_HW_MII),
          (0),
          (0),
          (0)
        }
    },

    {"MUSB_REQ_VENDER_06_10", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_SET_SW_MII),
          (0),
          (0),
          (0)
        }
    },
#if 1
    {"MUSB_REQ_VENDER_08_11", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_WRITE_MII_REG),
          (0x03),
          (0x04),
          (0x02)
        }
    },
#endif
    {"MUSB_REQ_VENDER_0A_12", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_SET_HW_MII),
          (0),
          (0),
          (0)
        }
    },

    {"MUSB_REQ_VENDER_06_13", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_SET_SW_MII),
          (0),
          (0),
          (0)
        }
    },

    {"MUSB_REQ_VENDER_07_14", 
        {(MUSB_DIR_IN|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_READ_MII_REG),
          (0x03),
          (0),
          (0x02)
        }
    },    

    {"MUSB_REQ_VENDER_0A_15", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_SET_HW_MII),
          (0),
          (0),
          (0)
        }
    },    

    {"MUSB_REQ_VENDER_06_16", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_SET_SW_MII),
          (0),
          (0),
          (0)
        }
    },    
#if 1
    {"MUSB_REQ_VENDER_08_17", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_WRITE_MII_REG),
          (0x03),
          (0),
          (0x02)
        }
    },    
#endif
    {"MUSB_REQ_VENDER_0A_18", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_SET_HW_MII),
          (0),
          (0),
          (0)
        }
    },  

// ifconfig
    {"MUSB_REQ_VENDER_10_19", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_WRITE_RX_CTL),
          (0x8c),
          (0),
          (0)
        }
    },  
#if 1
    {"MUSB_REQ_VENDER_16_20", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_WRITE_MULTI_FILTER),
          (0),
          (0),
          (0x08)
        }
    },     
#endif    
    {"MUSB_REQ_VENDER_10_21", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_WRITE_RX_CTL),
          (0x9c),
          (0),
          (0)
        }
    },     
#if 1
    {"MUSB_REQ_VENDER_16_22", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_WRITE_MULTI_FILTER),
          (0),
          (0),
          (0x08)
        }
    },     
#endif
    {"MUSB_REQ_VENDER_10_23", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_WRITE_RX_CTL),
          (0x9c),
          (0),
          (0)
        }
    },   
    
// Write Medium Status
    {"MUSB_REQ_VENDOR_1A_24",
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_WRITE_MEDIUM_MODE),
          (0x16),
          (0),
          (0)          
        }
    },
    
// Read Medium Status
    {"MUSB_REQ_VENDOR_1A_25",
        {(MUSB_DIR_IN|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_READ_MEDIUM_MODE),
          (0x00),
          (0),
          (0)          
        }
    },

#if defined USB2ETH_ADAPTER_DEBUG

// Read Operation Mode
    {"MUSB_REQ_VENDOR_09_26",
        {(MUSB_DIR_IN|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_READ_MII_OPERATION_MODE),
          (0x00),
          (0),
          (0x01)          
        }
    },    

// Read Rx Control Register
    {"MUSB_REQ_VENDOR_0F_27",
        {(MUSB_DIR_IN|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_READ_RX_CONTROL_REG),
          (0x00),
          (0),
          (0x02)          
        }
    },    

// Read IPG/IPG1/IPG2 Register    
    {"MUSB_REQ_VENDOR_11_28",
        {(MUSB_DIR_IN|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_READ_IPG012),
          (0x00),
          (0),
          (0x03)          
        }
    },

// Read Multi-Filter Array    
    {"MUSB_REQ_VENDOR_15_29",
        {(MUSB_DIR_IN|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_READ_MULTIFILTER_ARRAY),
          (0x00),
          (0),
          (0x08)          
        }
    },

// Read Monitor Mode Status
    {"MUSB_REQ_VENDOR_1c_30",
        {(MUSB_DIR_IN|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_READ_MONITOR_MODE),
          (0x00),
          (0),
          (0x01)          
        }
    },

// Read GPIOs
    {"MUSB_REQ_VENDOR_1e_31",
        {(MUSB_DIR_IN|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_READ_GPIOS),
          (0x00),
          (0),
          (0x01)          
        }
    },
#endif    
    
    {NULL, 
        {0,
          (0),
          (0),
          (0),
          (0)
        }
    },   

};
#endif
unsigned char eth_enum_write_index = 0;
unsigned char eth_enum_write_value[5][8] = { {0, 0x80}, {0xe1, 0x05}, {0, 0x32},      // 0x08
            {0, 0, 0, 0x80, 0, 0, 0, 0}, {0, 0, 0, 0x80, 0, 0, 0, 0} };  

enumeration_t hubdevenumeration_step[] = {
    {"MUSB_REQ_GET_STATUS",
		{(MUSB_DIR_IN|MUSB_TYPE_STANDARD|MUSB_RECIP_DEVICE),     // bmRequestType
         (MUSB_REQ_GET_STATUS),                               // bRequest
         (0),                                                     // wValue
         (0),                                                        // wIndex
         (2)                                                         // wLength
        }
	},
	{"MUSB_REQ_SET_FEATURE_PORT_POWER1",
	    {(MUSB_DIR_OUT|MUSB_TYPE_CLASS|MUSB_RECIP_OTHER),         // bmRequestType
	     (MUSB_REQ_SET_FEATURE),                               // bRequest
	     (0x8),//PORT_POWER                                                     // wValue
	     (0x1),                                                        // wIndex
	     (0)                                                         // wLength
	    }    
	},    
	{"MUSB_REQ_SET_FEATURE_PORT_POWER2",         
	    {(MUSB_DIR_OUT|MUSB_TYPE_CLASS|MUSB_RECIP_OTHER),         // bmRequestType
	     (MUSB_REQ_SET_FEATURE),                               // bRequest
	     (0x8),//PORT_POWER                                                     // wValue
	     (0x2),                                                        // wIndex
	     (0)                                                         // wLength        
	    }    
	},           
	{"MUSB_REQ_SET_FEATURE_PORT_POWER3",         
	    {(MUSB_DIR_OUT|MUSB_TYPE_CLASS|MUSB_RECIP_OTHER),         // bmRequestType          
	     (MUSB_REQ_SET_FEATURE),                               // bRequest          
	     (0x8),//PORT_POWER                                                     // wValue          
	     (0x3),                                                        // wIndex          
	     (0)                                                         // wLength        
	    }    
	},           
	{"MUSB_REQ_SET_FEATURE_PORT_POWER4",         
	    {(MUSB_DIR_OUT|MUSB_TYPE_CLASS|MUSB_RECIP_OTHER),         // bmRequestType          
	     (MUSB_REQ_SET_FEATURE),                               // bRequest          
	     (0x8),//PORT_POWER                                                     // wValue          
	     (0x4),                                                        // wIndex          
	     (0)                                                         // wLength        
	    }    
	},               
	{"MUSB_REQ_CLEAR_FEATURE_PORT_CONNECTION1",         
	    {(MUSB_DIR_OUT|MUSB_TYPE_CLASS|MUSB_RECIP_OTHER),         // bmRequestType          
	     (MUSB_REQ_CLEAR_FEATURE),                               // bRequest          
	     (0x10),//PORT_CONNECTION                                                     // wValue          
	     (0x1),                                                        // wIndex          
	     (0)                                                         // wLength        
	    }    
	},    
	{"MUSB_REQ_CLEAR_FEATURE_PORT_CONNECTION2",         
	    {(MUSB_DIR_OUT|MUSB_TYPE_CLASS|MUSB_RECIP_OTHER),         // bmRequestType          
	     (MUSB_REQ_CLEAR_FEATURE),                               // bRequest          
	     (0x10),//PORT_CONNECTION                                                     // wValue          
	     (0x2),                                                        // wIndex          
	     (0)                                                         // wLength        
	    }    
	},               
	{"MUSB_REQ_CLEAR_FEATURE_PORT_CONNECTION3",         
	    {(MUSB_DIR_OUT|MUSB_TYPE_CLASS|MUSB_RECIP_OTHER),         // bmRequestType          
	     (MUSB_REQ_CLEAR_FEATURE),                               // bRequest          
	     (0x10),//PORT_CONNECTION                                                     // wValue          
	     (0x3),                                                        // wIndex          
	     (0)                                                         // wLength        
	    }    
	},               
	{"MUSB_REQ_CLEAR_FEATURE_PORT_CONNECTION4",         
	    {(MUSB_DIR_OUT|MUSB_TYPE_CLASS|MUSB_RECIP_OTHER),         // bmRequestType          
	     (MUSB_REQ_CLEAR_FEATURE),                               // bRequest          
	     (0x10),//PORT_CONNECTION                                                     // wValue          
	     (0x4),                                                        // wIndex          
	     (0)                                                         // wLength        
	    }    
	},               
	{"MUSB_REQ_GET_STATUS_PORT1",         
	    {(MUSB_DIR_IN|MUSB_TYPE_CLASS|MUSB_RECIP_OTHER),         // bmRequestType          
	     (MUSB_REQ_GET_STATUS),                               // bRequest          
	     (0),                                                    //wValue
         (0x1),                                                        // wIndex          
         (4)                                                         // wLength        
        }    
	},                 
	{"MUSB_REQ_GET_STATUS_PORT2",         
	    {(MUSB_DIR_IN|MUSB_TYPE_CLASS|MUSB_RECIP_OTHER),         // bmRequestType          
	     (MUSB_REQ_GET_STATUS),                               // bRequest          
	     (0),                                                    // wValue          
	     (0x2),                                                        // wIndex          
	     (4)                                                         // wLength        
	    }    
	},          
	{"MUSB_REQ_GET_STATUS_PORT3",         
	    {(MUSB_DIR_IN|MUSB_TYPE_CLASS|MUSB_RECIP_OTHER),         // bmRequestType          
	     (MUSB_REQ_GET_STATUS),                               // bRequest          
	     (0),                                                    // wValue          
	     (0x3),                                                        // wIndex          
	     (4)                                                         // wLength        
	    }    
	},          
	{"MUSB_REQ_GET_STATUS_PORT4",         
	    {(MUSB_DIR_IN|MUSB_TYPE_CLASS|MUSB_RECIP_OTHER),         // bmRequestType          
	     (MUSB_REQ_GET_STATUS),                               // bRequest          
	     (0),                                                    // wValue          
	     (0x4),                                                        // wIndex          
	     (4)                                                         // wLength        
	    }    
	},
	#if 0
	{"MUSB_REQ_SET_FEATURE_PORT_RESET_1",         
	    {(MUSB_DIR_OUT|MUSB_TYPE_CLASS|MUSB_RECIP_OTHER),         // bmRequestType          
	     (MUSB_REQ_SET_FEATURE),                               // bRequest          
	     (0x4), //PORT_RESET                                                    // wValue          
	     (0x1),                                                        // wIndex          
	     (0)                                                         // wLength        
	    }    
	},                  
	{"MUSB_REQ_SET_FEATURE_PORT_RESET_2",         
	    {(MUSB_DIR_OUT|MUSB_TYPE_CLASS|MUSB_RECIP_OTHER),         // bmRequestType          
	     (MUSB_REQ_SET_FEATURE),                               // bRequest          
	     (0x4), //PORT_RESET                                                    // wValue          
	     (0x2),                                                        // wIndex          
	     (0)                                                         // wLength        
	    }    
	},                  
	{"MUSB_REQ_SET_FEATURE_PORT_RESET_3",         
	    {(MUSB_DIR_OUT|MUSB_TYPE_CLASS|MUSB_RECIP_OTHER),         // bmRequestType          
	     (MUSB_REQ_SET_FEATURE),                               // bRequest          
	     (0x4), //PORT_RESET                                                    // wValue          
	     (0x3),                                                        // wIndex          
	     (0)                                                         // wLength        
	    }    
	},                  
	{"MUSB_REQ_SET_FEATURE_PORT_RESET_4",         
	    {(MUSB_DIR_OUT|MUSB_TYPE_CLASS|MUSB_RECIP_OTHER),         // bmRequestType          
	     (MUSB_REQ_SET_FEATURE),                               // bRequest          
         (0x4), //PORT_RESET         // wValue          
         (0x4),                                                        // wIndex          
         (0)                                                         // wLength        
        }    
	},        
	
	{"MUSB_REQ_GET_STATUS_PORT1",         
	    {(MUSB_DIR_IN|MUSB_TYPE_CLASS|MUSB_RECIP_OTHER),         // bmRequestType          
	     (MUSB_REQ_GET_STATUS),                               // bRequest          
	     (0),                                                    // wValue          
	     (0x1),                                                        // wIndex          
	     (4)                                                         // wLength        
	    }    
	},        
	{"MUSB_REQ_GET_STATUS_PORT2",         
	    {(MUSB_DIR_IN|MUSB_TYPE_CLASS|MUSB_RECIP_OTHER),         // bmRequestType          
	     (MUSB_REQ_GET_STATUS),                               // bRequest          
	     (0),                                                    // wValue          
	     (0x2),                                                        // wIndex          
	     (4)                                                         // wLength        
	    }    
	},          
	{"MUSB_REQ_GET_STATUS_PORT3",         
	    {(MUSB_DIR_IN|MUSB_TYPE_CLASS|MUSB_RECIP_OTHER),         // bmRequestType          
	     (MUSB_REQ_GET_STATUS),                               // bRequest          
	     (0),                                                    // wValue          
	     (0x3),                                                        // wIndex          
	     (4)                                                         // wLength        
	    }    
	},          
	{"MUSB_REQ_GET_STATUS_PORT4",         
	    {(MUSB_DIR_IN|MUSB_TYPE_CLASS|MUSB_RECIP_OTHER),         // bmRequestType          
	     (MUSB_REQ_GET_STATUS),                               // bRequest          
	     (0),                                                    // wValue          
	     (0x4),                                                        // wIndex          
	     (4)                                                         // wLength        
	    }    
	},
	#endif
	{NULL, 
        {0,
          (0),
          (0),
          (0),
          (0)
        }
    },  
};

enumeration_t enumeration_step[] = {
    {"MUSB_REQ_SET_ADDRESS", 
        {(MUSB_DIR_OUT|MUSB_TYPE_STANDARD|MUSB_RECIP_DEVICE),         // bmRequestType
          (MUSB_REQ_SET_ADDRESS),                                     // bRequest
          (0),                                                        // wValue
          (0),                                                        // wIndex
          (0)                                                         // wLength
        }
    },
    {"MUSB_REQ_GET_DEV_DESCRIPTOR", 
        {(MUSB_DIR_IN|MUSB_TYPE_STANDARD|MUSB_RECIP_DEVICE),          // bmRequestType
          (MUSB_REQ_GET_DESCRIPTOR),                                  // bRequest
          (MUSB_DT_DEVICE << 8),                                      // wValue
          (0),                                                        // wIndex
          (0x40)                                                      // wLength
        }
    },
    {"MUSB_REQ_GET_CONF_DESCRIPTOR", 
        {(MUSB_DIR_IN|MUSB_TYPE_STANDARD|MUSB_RECIP_DEVICE),          // bmRequestType
          (MUSB_REQ_GET_DESCRIPTOR),                                  // bRequest
          (MUSB_DT_CONFIG << 8),                                      // wValue
          (0),                                                        // wIndex
          (0x39)
#if 0
#if defined MUSB_DEV_USB2ETH          
          (0x27)                                                      // wLength
#elif defined MUSB_DEV_MASSSTORAGE
          (0x27)
#elif defined MUSB_DEV_7108
          (0x43)
#endif      
#endif
        }
    },
    {"MUSB_REQ_SET_CONF1_DESCRIPTOR", 
        {(MUSB_DIR_OUT|MUSB_TYPE_STANDARD|MUSB_RECIP_DEVICE),         // bmRequestType
          (MUSB_REQ_SET_CONFIGURATION),                               // bRequest
          (0x01),                                                     // wValue
          (0),                                                        // wIndex
          (0)                                                         // wLength
        }
    },
#if 0
    {"MUSB_REQ_GET_STR0_DESCRIPTOR", 
        {(MUSB_DIR_IN|MUSB_TYPE_STANDARD|MUSB_RECIP_DEVICE),          // bmRequestType
          (MUSB_REQ_GET_DESCRIPTOR),                                  // bRequest
          (MUSB_DT_STRING << 8),                                      // wValue
          (0),                                                        // wIndex
          (0xff)                                                      // wLength
        }
    },
    {"MUSB_REQ_GET_STR1_DESCRIPTOR", 
        {(MUSB_DIR_IN|MUSB_TYPE_STANDARD|MUSB_RECIP_DEVICE),          // bmRequestType
          (MUSB_REQ_GET_DESCRIPTOR),                                  // bRequest
          (MUSB_DT_STRING << 8 | 1),                                  // wValue
          (0x409),                                                    // wIndex
          (0xff)                                                      // wLength
        }
    },
    {"MUSB_REQ_GET_STR2_DESCRIPTOR", 
        {(MUSB_DIR_IN|MUSB_TYPE_STANDARD|MUSB_RECIP_DEVICE),          // bmRequestType
          (MUSB_REQ_GET_DESCRIPTOR),                                  // bRequest
          (MUSB_DT_STRING << 8 | 2),                                  // wValue
          (0x409),                                                    // wIndex
          (0xff)                                                      // wLength
        }
    },
    {"MUSB_REQ_GET_STR3_DESCRIPTOR", 
        {(MUSB_DIR_IN|MUSB_TYPE_STANDARD|MUSB_RECIP_DEVICE),          // bmRequestType
          (MUSB_REQ_GET_DESCRIPTOR),                                  // bRequest
          (MUSB_DT_STRING << 8 | 3),                                  // wValue
          (0x409),                                                    // wIndex
          (0xff)                                                      // wLength
        }
    },
    #if 0
    {"MUSB_REQ_GET_FE_DESCRIPTOR", 
        {(0xA1),                                                      // bmRequestType
          (0xFE),                                                     // bRequest
          (0),                                                        // wValue
          (0),                                                        // wIndex
          (1)                                                         // wLength
        }
    },
    #endif
#endif
    {NULL}

};

#endif
