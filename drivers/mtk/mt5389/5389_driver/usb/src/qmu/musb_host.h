/******************************************************************
 * Copyright 2005 Mentor Graphics Corporation
 *
 * This file is part of the Inventra Controller Driver for Linux.
 * 
 * The Inventra Controller Driver for Linux is free software; you 
 * can redistribute it and/or modify it under the terms of the GNU 
 * General Public License version 2 as published by the Free Software 
 * Foundation.
 * 
 * The Inventra Controller Driver for Linux is distributed in 
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public 
 * License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with The Inventra Controller Driver for Linux ; if not, 
 * write to the Free Software Foundation, Inc., 59 Temple Place, 
 * Suite 330, Boston, MA  02111-1307  USA
 * 
 * ANY DOWNLOAD, USE, REPRODUCTION, MODIFICATION OR DISTRIBUTION 
 * OF THIS DRIVER INDICATES YOUR COMPLETE AND UNCONDITIONAL ACCEPTANCE 
 * OF THOSE TERMS.THIS DRIVER IS PROVIDED "AS IS" AND MENTOR GRAPHICS 
 * MAKES NO WARRANTIES, EXPRESS OR IMPLIED, RELATED TO THIS DRIVER.  
 * MENTOR GRAPHICS SPECIFICALLY DISCLAIMS ALL IMPLIED WARRANTIES 
 * OF MERCHANTABILITY; FITNESS FOR A PARTICULAR PURPOSE AND 
 * NON-INFRINGEMENT.  MENTOR GRAPHICS DOES NOT PROVIDE SUPPORT 
 * SERVICES OR UPDATES FOR THIS DRIVER, EVEN IF YOU ARE A MENTOR 
 * GRAPHICS SUPPORT CUSTOMER. 
 ******************************************************************/

#ifndef _MUSB_HOST_H
#define _MUSB_HOST_H

#include "musbdefs.h"
#include "mu_dev.h"
#include "debug.h"

#if defined MUSB_DEV_MASSSTORAGE
/*****************************************************************/
/* Mass Storage                                                                                              */
/*****************************************************************/

/*
  * Bulk only data structures
  */

/* command block wrapper */
typedef struct bulk_cb_wrap_s {
    unsigned int Signature;                    /* contains 'USBC' */
    unsigned int Tag;                             /* unique per command id */
    unsigned int DataTransferLength;     /* size of data */
    unsigned char Flags;                       /* direction in bit 0 */
    unsigned char Lun;                          /* LUN normally 0 */
    unsigned char Length;                     /* of of the CDB */
    unsigned char opcode;
    unsigned int CDB[15];                     /* max command */
} bulk_cb_wrap_t;
//} __attribute__ ((__aligned__(1), packed)) bulk_cb_wrap_t;

#define US_BULK_CB_WRAP_LEN     31
#define US_BULK_CB_SIGN         0x43425355      /*spells out USBC */
#define US_BULK_FLAG_IN         1
#define US_BULK_FLAG_OUT        0

/* command status wrapper */
struct bulk_cs_wrap_s {
    unsigned int  Signature;              		/* should = 'USBS' */
    unsigned int Tag;                    		/* same as original command */
    unsigned int Residue;                		/* amount not transferred */
    unsigned char Status;                 		/* see below */
    unsigned char Filler[18];             		
} bulk_cs_wrap_t;
//}__attribute__ ((__aligned__(1), packed)) bulk_cs_wrap_t;

#define US_BULK_CS_WRAP_LEN     13
#define US_BULK_CS_SIGN         0x53425355      /* spells out 'USBS' */
#define US_BULK_STAT_OK         0
#define US_BULK_STAT_FAIL       1
#define US_BULK_STAT_PHASE      2

/* bulk-only class specific requests */
#define US_BULK_RESET_REQUEST   0xff
#define US_BULK_GET_MAX_LUN     0xfe
#endif


/*****************************************************************/
/* Transfer Type */
#define TRANSTYPE_CONTROL                0
#define TRANSTYPE_ISOCHRONOUS        1
#define TRANSTYPE_BULK                      2
#define TRANSTYPE_INTERRUPT              3

#if MUSB_DEBUG > 0
#define GET_ASSIGN_ADDRESS() wAssignAddress++;
#else
#define GET_ASSIGN_ADDRESS() wAssignAddress;
#endif

#if 0 //!PORTING
#define min(x,y) ({ \
            typeof(x) _x = (x);     \
            typeof(y) _y = (y);     \
            (void) (&_x == &_y);    \
            _x < _y ? _x : _y; })
#endif

/****************************** CONSTANTS ********************************/
#if !PORTING
unsigned static short wAssignAddress = 2;
#endif

typedef struct enumeration_s
{
    unsigned char* pDesciptor;
    MUSB_DeviceRequest sDevReq;
} enumeration_t;

static enumeration_t enumeration_step[] = {
    {"MUSB_REQ_SET_ADDRESS", 
        {(MUSB_DIR_OUT|MUSB_TYPE_STANDARD|MUSB_RECIP_DEVICE),         // bmRequestType
          (MUSB_REQ_SET_ADDRESS),                                                         // bRequest
          (0),                                                                                         // wValue
          (0),                                                                                         // wIndex
          (0)                                                                                          // wLength
        }
    },

    {"MUSB_REQ_GET_DEV_DESCRIPTOR", 
        {(MUSB_DIR_IN|MUSB_TYPE_STANDARD|MUSB_RECIP_DEVICE),           // bmRequestType
          (MUSB_REQ_GET_DESCRIPTOR),                                                    // bRequest
          (MUSB_DT_DEVICE << 8),                                                            // wValue
          (0),                                                                                         // wIndex
          (0x40)                                                                                     // wLength
        }
    },

    {"MUSB_REQ_GET_CONF_DESCRIPTOR", 
        {(MUSB_DIR_IN|MUSB_TYPE_STANDARD|MUSB_RECIP_DEVICE),           // bmRequestType
          (MUSB_REQ_GET_DESCRIPTOR),                                                    // bRequest
          (MUSB_DT_CONFIG << 8),                                                            // wValue
          (0),                                                                                         // wIndex
#if defined MUSB_DEV_USB2ETH          
          (0x27)                                                                                     // wLength
#elif defined MUSB_DEV_MASSSTORAGE
          (0x27)
#elif defined MUSB_DEV_7108
          (0x43)
#endif          
        }
    },

    {"MUSB_REQ_SET_CONF1_DESCRIPTOR", 
        {(MUSB_DIR_OUT|MUSB_TYPE_STANDARD|MUSB_RECIP_DEVICE),         // bmRequestType
          (MUSB_REQ_SET_CONFIGURATION),                                               // bRequest
          (0x01),                                                                                    // wValue
          (0),                                                                                         // wIndex
          (0)                                                                                          // wLength
        }
    },

#if defined MUSB_DEV_MASSSTORAGE
    {"MUSB_REQ_GET_STR0_DESCRIPTOR", 
        {(MUSB_DIR_IN|MUSB_TYPE_STANDARD|MUSB_RECIP_DEVICE),          // bmRequestType
          (MUSB_REQ_GET_DESCRIPTOR),                                                   // bRequest
          (MUSB_DT_STRING << 8),                                                            // wValue
          (0),                                                                                         // wIndex
          (0xff)                                                                                      // wLength
        }
    },

    {"MUSB_REQ_GET_STR1_DESCRIPTOR", 
        {(MUSB_DIR_IN|MUSB_TYPE_STANDARD|MUSB_RECIP_DEVICE),          // bmRequestType
          (MUSB_REQ_GET_DESCRIPTOR),                                                   // bRequest
          (MUSB_DT_STRING << 8 | 1),                                                      // wValue
          (0x409),                                                                                   // wIndex
          (0xff)                                                                                      // wLength
        }
    },

    {"MUSB_REQ_GET_STR2_DESCRIPTOR", 
        {(MUSB_DIR_IN|MUSB_TYPE_STANDARD|MUSB_RECIP_DEVICE),          // bmRequestType
          (MUSB_REQ_GET_DESCRIPTOR),                                                   // bRequest
          (MUSB_DT_STRING << 8 | 2),                                                      // wValue
          (0x409),                                                                                   // wIndex
          (0xff)                                                                                      // wLength
        }
    },

    {"MUSB_REQ_GET_STR3_DESCRIPTOR", 
        {(MUSB_DIR_IN|MUSB_TYPE_STANDARD|MUSB_RECIP_DEVICE),          // bmRequestType
          (MUSB_REQ_GET_DESCRIPTOR),                                                   // bRequest
          (MUSB_DT_STRING << 8 | 3),                                                      // wValue
          (0x409),                                                                                   // wIndex
          (0xff)                                                                                      // wLength
        }
    },

    {"MUSB_REQ_GET_FE_DESCRIPTOR", 
        {(0xA1),                                                                                    // bmRequestType
          (0xFE),                                                                                    // bRequest
          (0),                                                                                         // wValue
          (0),                                                                                         // wIndex
          (1)                                                                                          // wLength
        }
    },
#endif

    {NULL}

};

#if defined MUSB_DEV_MASSSTORAGE

//#define USBDEVICE_MS_SONY
#define USBDEVICE_MS_MEMOREX
//#define USBDEVICE_MS_MEMOREX_NT

// Mass Storage
typedef struct MSenumeration_s
{
    unsigned char* pDesciptor;
    bulk_cb_wrap_t sCBW;
} MSenumeration_t;

#if defined USBDEVICE_MS_SONY
// Mass Storage Enumeration for Linux -device ming (sony)
static MSenumeration_t msenumeration_step[] = {
    {"SCSI command : INQUIRY", 
        {(US_BULK_CB_SIGN),                                                                   // dCBWSignature
          (0x00000010),                                                                           // dCBWTag
          (0x00000024),                                                                           // dCBWDataTransferLength
          (0x80),                                                                                     // bmCBWFlags
          (0x00),                                                                                     // bCBWLUN
          (0x06),                                                                                     // bCBWCBLength
          (0x12),                                                                                     // opcode
          {0x24000000, 0x00000000, 0x00000000, 0x00000000}                      //CBWCB
        }
    },
    {"TEST UNIT READY", 
        {(US_BULK_CB_SIGN),                                                                   // dCBWSignature
          (0x00000002),                                                                           // dCBWTag
          (0x00000000),                                                                           // dCBWDataTransferLength
          (0x00),                                                                                     // bmCBWFlags
          (0x00),                                                                                     // bCBWLUN
          (0x06),                                                                                     // bCBWCBLength
          (0x00),                                                                                     // opcode
          {0x00000000, 0x00000000, 0x00000000, 0x00000000}                      //CBWCB
        }
    },
    {"READ CAPACITY(10)", 
        {(US_BULK_CB_SIGN),                                                                   // dCBWSignature
          (0x00000002),                                                                           // dCBWTag
          (0x00000008),                                                                           // dCBWDataTransferLength
          (0x80),                                                                                     // bmCBWFlags
          (0x00),                                                                                     // bCBWLUN
          (0x0A),                                                                                     // bCBWCBLength
          (0x25),                                                                                     // opcode
          {0x00000000, 0x00000000, 0x00000000, 0x00000000}                      //CBWCB
        }
    },
    {"TEST UNIT READY", 
        {(US_BULK_CB_SIGN),                                                                  // dCBWSignature
          (0x00000003),                                                                           // dCBWTag
          (0x00000000),                                                                           // dCBWDataTransferLength
          (0x00),                                                                                     // bmCBWFlags
          (0x00),                                                                                     // bCBWLUN
          (0x06),                                                                                     // bCBWCBLength
          (0x00),                                                                                     // opcode
          {0x00000000, 0x00000000, 0x00000000, 0x00000000}                      //CBWCB
        }
    },
    {"PREVENT ALLW MDM RMVL", 
        {(US_BULK_CB_SIGN),                                                                   // dCBWSignature
          (0x00000014),                                                                           // dCBWTag
          (0x00000000),                                                                           // dCBWDataTransferLength
          (0x00),                                                                                     // bmCBWFlags
          (0x00),                                                                                     // bCBWLUN
          (0x06),                                                                                     // bCBWCBLength
          (0x1E),                                                                                     // opcode
          {0x01000000, 0x00000000, 0x00000000, 0x00000000}                      //CBWCB
        }
    },  
    {"REQUEST SENSE", 
        {(US_BULK_CB_SIGN),                                                                   // dCBWSignature
          (0x80000006),                                                                           // dCBWTag
          (0x00000012),                                                                           // dCBWDataTransferLength
          (0x80),                                                                                     // bmCBWFlags
          (0x00),                                                                                     // bCBWLUN
          (0x06),                                                                                     // bCBWCBLength
          (0x03),                                                                                     // opcode
          {0x12000000, 0x00000000, 0x00000000, 0x00000000}                      //CBWCB
        }
    },
    {"TEST UNIT READY", 
        {(US_BULK_CB_SIGN),                                                                   // dCBWSignature
          (0x00000007),                                                                           // dCBWTag
          (0x00000000),                                                                           // dCBWDataTransferLength
          (0x00),                                                                                     // bmCBWFlags
          (0x00),                                                                                     // bCBWLUN
          (0x06),                                                                                     // bCBWCBLength
          (0x00),                                                                                     // opcode
          {0x00000000, 0x00000000, 0x00000000, 0x00000000}                      //CBWCB
        }
    },
    {"READ CAPACITY(10)", 
        {(US_BULK_CB_SIGN),                                                                   // dCBWSignature
          (0x00000008),                                                                           // dCBWTag
          (0x00000008),                                                                           // dCBWDataTransferLength
          (0x80),                                                                                     // bmCBWFlags
          (0x00),                                                                                     // bCBWLUN
          (0x0A),                                                                                     // bCBWCBLength
          (0x25),                                                                                     // opcode
          {0x00000000, 0x00000000, 0x00000000, 0x00000000}                      //CBWCB
        }
    },
    {"READ(10)", 
        {(US_BULK_CB_SIGN),                                                                   // dCBWSignature
          (0x00000009),                                                                           // dCBWTag
          (0x00001000),                                                                           // dCBWDataTransferLength
          (0x80),                                                                                     // bmCBWFlags
          (0x00),                                                                                     // bCBWLUN
          (0x0A),                                                                                     // bCBWCBLength
          (0x28),                                                                                     // opcode
          {0x00000000, 0x08000000, 0x00000000, 0x00000000}                      //CBWCB
        }
    },
    {NULL}
};

#elif defined USBDEVICE_MS_MEMOREX

// Mass Storage Enumeration for Linux - device 1 (memorex)
static MSenumeration_t msenumeration_step[] = {
    {"SCSI command : INQUIRY", 
        {(US_BULK_CB_SIGN),                                                                   // dCBWSignature
          (0x00000001),                                                                           // dCBWTag
          (0x00000024),                                                                           // dCBWDataTransferLength
          (0x80),                                                                                     // bmCBWFlags
          (0x00),                                                                                     // bCBWLUN
          (0x06),                                                                                     // bCBWCBLength
          (0x12),                                                                                     // opcode
          {0x24000000, 0x00000000, 0x00000000, 0x00000000}                      //CBWCB
        }
    },
    {"TEST UNIT READY", 
        {(US_BULK_CB_SIGN),                                                                   // dCBWSignature
          (0x00000002),                                                                           // dCBWTag
          (0x00000000),                                                                           // dCBWDataTransferLength
          (0x00),                                                                                     // bmCBWFlags
          (0x00),                                                                                     // bCBWLUN
          (0x06),                                                                                     // bCBWCBLength
          (0x00),                                                                                     // opcode
          {0x00000000, 0x00000000, 0x00000000, 0x00000000}                      //CBWCB
        }
    },
    {"REQUEST SENSE", 
        {(US_BULK_CB_SIGN),                                                                   // dCBWSignature
          (0x80000002),                                                                           // dCBWTag
          (0x00000012),                                                                           // dCBWDataTransferLength
          (0x80),                                                                                     // bmCBWFlags
          (0x00),                                                                                     // bCBWLUN
          (0x06),                                                                                     // bCBWCBLength
          (0x03),                                                                                     // opcode
          {0x12000000, 0x00000000, 0x00000000, 0x00000000}                      //CBWCB
        }
    },
    {"TEST UNIT READY", 
        {(US_BULK_CB_SIGN),                                                                  // dCBWSignature
          (0x00000003),                                                                           // dCBWTag
          (0x00000000),                                                                           // dCBWDataTransferLength
          (0x00),                                                                                     // bmCBWFlags
          (0x00),                                                                                     // bCBWLUN
          (0x06),                                                                                     // bCBWCBLength
          (0x00),                                                                                     // opcode
          {0x00000000, 0x00000000, 0x00000000, 0x00000000}                      //CBWCB
        }
    },
    {"READ CAPACITY(10)", 
        {(US_BULK_CB_SIGN),                                                                   // dCBWSignature
          (0x00000004),                                                                           // dCBWTag
          (0x00000008),                                                                           // dCBWDataTransferLength
          (0x80),                                                                                     // bmCBWFlags
          (0x00),                                                                                     // bCBWLUN
          (0x0A),                                                                                     // bCBWCBLength
          (0x25),                                                                                     // opcode
          {0x00000000, 0x00000000, 0x00000000, 0x00000000}                      //CBWCB
        }
    },
    {"TEST UNIT READY", 
        {(US_BULK_CB_SIGN),                                                                   // dCBWSignature
          (0x00000005),                                                                           // dCBWTag
          (0x00000000),                                                                           // dCBWDataTransferLength
          (0x00),                                                                                     // bmCBWFlags
          (0x00),                                                                                     // bCBWLUN
          (0x06),                                                                                     // bCBWCBLength
          (0x00),                                                                                     // opcode
          {0x00000000, 0x00000000, 0x00000000, 0x00000000}                      //CBWCB
        }
    },
    {"PREVENT ALLW MDM RMVL", 
        {(US_BULK_CB_SIGN),                                                                   // dCBWSignature
          (0x00000006),                                                                           // dCBWTag
          (0x00000000),                                                                           // dCBWDataTransferLength
          (0x00),                                                                                     // bmCBWFlags
          (0x00),                                                                                     // bCBWLUN
          (0x06),                                                                                     // bCBWCBLength
          (0x1E),                                                                                     // opcode
          {0x01000000, 0x00000000, 0x00000000, 0x00000000}                      //CBWCB
        }
    },
    {"REQUEST SENSE", 
        {(US_BULK_CB_SIGN),                                                                   // dCBWSignature
          (0x80000006),                                                                           // dCBWTag
          (0x00000012),                                                                           // dCBWDataTransferLength
          (0x80),                                                                                     // bmCBWFlags
          (0x00),                                                                                     // bCBWLUN
          (0x06),                                                                                     // bCBWCBLength
          (0x03),                                                                                     // opcode
          {0x12000000, 0x00000000, 0x00000000, 0x00000000}                      //CBWCB
        }
    },
    {"TEST UNIT READY", 
        {(US_BULK_CB_SIGN),                                                                   // dCBWSignature
          (0x00000007),                                                                           // dCBWTag
          (0x00000000),                                                                           // dCBWDataTransferLength
          (0x00),                                                                                     // bmCBWFlags
          (0x00),                                                                                     // bCBWLUN
          (0x06),                                                                                     // bCBWCBLength
          (0x00),                                                                                     // opcode
          {0x00000000, 0x00000000, 0x00000000, 0x00000000}                      //CBWCB
        }
    },
    {"READ CAPACITY(10)", 
        {(US_BULK_CB_SIGN),                                                                   // dCBWSignature
          (0x00000008),                                                                           // dCBWTag
          (0x00000008),                                                                           // dCBWDataTransferLength
          (0x80),                                                                                     // bmCBWFlags
          (0x00),                                                                                     // bCBWLUN
          (0x0A),                                                                                     // bCBWCBLength
          (0x25),                                                                                     // opcode
          {0x00000000, 0x00000000, 0x00000000, 0x00000000}                      //CBWCB
        }
    },
    {"READ(10)", 
        {(US_BULK_CB_SIGN),                                                                   // dCBWSignature
          (0x00000009),                                                                           // dCBWTag
          (0x00001000),                                                                           // dCBWDataTransferLength
          (0x80),                                                                                     // bmCBWFlags
          (0x00),                                                                                     // bCBWLUN
          (0x0A),                                                                                     // bCBWCBLength
          (0x28),                                                                                     // opcode
          {0x00000000, 0x08000000, 0x00000000, 0x00000000}                      //CBWCB
        }
    },
    {NULL}
};

#elif defined USBDEVICE_MS_MEMOREX_NT

// Mass Storage Enumeration for Windows
static MSenumeration_t msenumeration_step[] = {    
    {"SCSI command : INQUIRY", 
        {(US_BULK_CB_SIGN),                                                                   // dCBWSignature
          (0x8533ACF0),                                                                           // dCBWTag
          (0x00000024),                                                                           // dCBWDataTransferLength
          (0x80),                                                                                    // bmCBWFlags
          (0x00),                                                                                    // bCBWLUN
          (0x06),                                                                                    // bCBWCBLength
          (0x12),                                                                                    // opcode
          {0x24000000, 0x00000000, 0x00000000, 0x00000000}                     //CBWCB
        }
    },
    {"READ FORMAT CAPACTIES", 
        {(US_BULK_CB_SIGN),                                                                   // dCBWSignature
          (0x8533ACF0),                                                                           // dCBWTag
          (0x000000FC),                                                                           // dCBWDataTransferLength
          (0x80),                                                                                     // bmCBWFlags
          (0x00),                                                                                     // bCBWLUN
          (0x0A),                                                                                     // bCBWCBLength
          (0x23),                                                                                     // opcode
          {0x00000000, 0xFC000000, 0x00000000, 0x00000000}                      //CBWCB
        }
    },
    {"REQUEST SENSE", 
        {(US_BULK_CB_SIGN),                                                                   // dCBWSignature
          (0x8533ACF0),                                                                           // dCBWTag
          (0x00000012),                                                                           // dCBWDataTransferLength
          (0x80),                                                                                     // bmCBWFlags
          (0x00),                                                                                     // bCBWLUN
          (0x0C),                                                                                     // bCBWCBLength
          (0x03),                                                                                     // opcode
          {0x12000000, 0x00000000, 0x00000000, 0x00000000}                      //CBWCB
        }
    },
    {"READ FORMAT CAPACTIES", 
        {(US_BULK_CB_SIGN),                                                                  // dCBWSignature
          (0x8533ACF0),                                                                           // dCBWTag
          (0x000000FC),                                                                           // dCBWDataTransferLength
          (0x80),                                                                                     // bmCBWFlags
          (0x00),                                                                                     // bCBWLUN
          (0x0A),                                                                                     // bCBWCBLength
          (0x23),                                                                                     // opcode
          {0x00000000, 0xFC000000, 0x00000000, 0x00000000}                      //CBWCB
        }
    },
    {"READ CAPACITY(10)", 
        {(US_BULK_CB_SIGN),                                                                   // dCBWSignature
          (0x85D79008),                                                                           // dCBWTag
          (0x00000008),                                                                           // dCBWDataTransferLength
          (0x80),                                                                                     // bmCBWFlags
          (0x00),                                                                                     // bCBWLUN
          (0x0A),                                                                                     // bCBWCBLength
          (0x25),                                                                                     // opcode
          {0x00000000, 0x00000000, 0x00000000, 0x00000000}                      //CBWCB
        }
    },
    {"READ(10)", 
        {(US_BULK_CB_SIGN),                                                                   // dCBWSignature
          (0x85D79008),                                                                           // dCBWTag
          (0x00000200),                                                                           // dCBWDataTransferLength
          (0x80),                                                                                     // bmCBWFlags
          (0x00),                                                                                     // bCBWLUN
          (0x0A),                                                                                     // bCBWCBLength
          (0x28),                                                                                     // opcode
          {0x00000000, 0x01000000, 0x00000000, 0x00000000}                      //CBWCB
        }
    },
    {"MODE SENSE(6)", 
        {(US_BULK_CB_SIGN),                                                                   // dCBWSignature
          (0x85A17E28),                                                                           // dCBWTag
          (0x000000C0),                                                                           // dCBWDataTransferLength
          (0x80),                                                                                     // bmCBWFlags
          (0x00),                                                                                     // bCBWLUN
          (0x06),                                                                                     // bCBWCBLength
          (0x1A),                                                                                     // opcode
          {0xC0001C00, 0x00000000, 0x00000000, 0x00000000}                      //CBWCB
        }
    },
    {"MODE SENSE(6)", 
        {(US_BULK_CB_SIGN),                                                                   // dCBWSignature
          (0x85D79008),                                                                           // dCBWTag
          (0x000000C0),                                                                           // dCBWDataTransferLength
          (0x80),                                                                                     // bmCBWFlags
          (0x00),                                                                                     // bCBWLUN
          (0x06),                                                                                     // bCBWCBLength
          (0x1A),                                                                                     // opcode
          {0x00000000, 0x00000000, 0x00000000, 0x00000000}                      //CBWCB
        }
    },
    {"READ CAPACTITY(10)", 
        {(US_BULK_CB_SIGN),                                                                   // dCBWSignature
          (0x85D79008),                                                                           // dCBWTag
          (0x00000008),                                                                           // dCBWDataTransferLength
          (0x80),                                                                                     // bmCBWFlags
          (0x00),                                                                                     // bCBWLUN
          (0x0A),                                                                                     // bCBWCBLength
          (0x25),                                                                                     // opcode
          {0x00000000, 0x00000000, 0x00000000, 0x00000000}                      //CBWCB
        }
    },
    {"READ CAPACITY(10)", 
        {(US_BULK_CB_SIGN),                                                                   // dCBWSignature
          (0x85D79008),                                                                           // dCBWTag
          (0x00000008),                                                                           // dCBWDataTransferLength
          (0x80),                                                                                     // bmCBWFlags
          (0x00),                                                                                     // bCBWLUN
          (0x0A),                                                                                     // bCBWCBLength
          (0x25),                                                                                     // opcode
          {0x00000000, 0x00000000, 0x00000000, 0x00000000}                      //CBWCB
        }
    },
    {"READ(10)", 
        {(US_BULK_CB_SIGN),                                                                   // dCBWSignature
          (0x85D79008),                                                                           // dCBWTag
          (0x00000200),                                                                           // dCBWDataTransferLength
          (0x80),                                                                                     // bmCBWFlags
          (0x00),                                                                                     // bCBWLUN
          (0x0A),                                                                                     // bCBWCBLength
          (0x28),                                                                                     // opcode
          {0x00000000, 0x01000000, 0x00000000, 0x00000000}                      //CBWCB
        }
    },
    {"READ(10)", 
        {(US_BULK_CB_SIGN),                                                                   // dCBWSignature
          (0x85D79008),                                                                           // dCBWTag
          (0x00000200),                                                                           // dCBWDataTransferLength
          (0x80),                                                                                     // bmCBWFlags
          (0x00),                                                                                     // bCBWLUN
          (0x0A),                                                                                     // bCBWCBLength
          (0x28),                                                                                     // opcode
          {0x00000000, 0x01000000, 0x00000000, 0x00000000}                      //CBWCB
        }
    },
    {"READ CAPACITY(10)", 
        {(US_BULK_CB_SIGN),                                                                   // dCBWSignature
          (0x85D79008),                                                                           // dCBWTag
          (0x00000000),                                                                           // dCBWDataTransferLength
          (0x80),                                                                                     // bmCBWFlags
          (0x00),                                                                                     // bCBWLUN
          (0x0A),                                                                                     // bCBWCBLength
          (0x25),                                                                                     // opcode
          {0x00000000, 0x00000000, 0x00000000, 0x00000000}                      //CBWCB
        }
    },
    {"READ CAPACITY(10)", 
        {(US_BULK_CB_SIGN),                                                                   // dCBWSignature
          (0x85D79008),                                                                           // dCBWTag
          (0x00000008),                                                                           // dCBWDataTransferLength
          (0x80),                                                                                     // bmCBWFlags
          (0x00),                                                                                     // bCBWLUN
          (0x0A),                                                                                     // bCBWCBLength
          (0x25),                                                                                     // opcode
          {0x00000000, 0x00000000, 0x00000000, 0x00000000}                      //CBWCB
        }
    },
    {"READ(10)", 
        {(US_BULK_CB_SIGN),                                                                   // dCBWSignature
          (0x85D79008),                                                                           // dCBWTag
          (0x00000200),                                                                           // dCBWDataTransferLength
          (0x80),                                                                                     // bmCBWFlags
          (0x00),                                                                                     // bCBWLUN
          (0x0A),                                                                                     // bCBWCBLength
          (0x28),                                                                                     // opcode
          {0x00000000, 0x01000000, 0x00000000, 0x00000000}                      //CBWCB
        }
    },
    {"TEST UNIT READY", 
        {(US_BULK_CB_SIGN),                                                                   // dCBWSignature
          (0x85357008),                                                                           // dCBWTag
          (0x00000000),                                                                           // dCBWDataTransferLength
          (0x00),                                                                                     // bmCBWFlags
          (0x00),                                                                                     // bCBWLUN
          (0x06),                                                                                     // bCBWCBLength
          (0x00),                                                                                     // opcode
          {0x00000000, 0x00000000, 0x00000000, 0x00000000}                      //CBWCB
        }
    },
    {NULL}
};

#endif

#endif

#if 1//defined MUSB_DEV_USB2ETH

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



// USB2ETH
typedef struct ethenumeration_s
{
    unsigned char* pDesciptor;
    MUSB_DeviceRequest sDevReq;
} ethenumeration_t;

#if defined MUSB_HS_DEVICE
static ethenumeration_t ethenumeration_step[] = {
    {"MUSB_REQ_VENDER_1F_1", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_WRITE_GPIOS),
          (0x13),
          (0),
          (0)
        }
    },

    {"MUSB_REQ_VENDER_1F_2", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_WRITE_GPIOS),
          (0x01),
          (0),
          (0)
        }
    },

    {"MUSB_REQ_VENDER_1F_3", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_WRITE_GPIOS),
          (0x03),
          (0),
          (0)
        }
    },

    {"MUSB_REQ_VENDER_10_4", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_WRITE_RX_CTL),
          (0x80),
          (0),
          (0)
        }
    },

    {"MUSB_REQ_VENDER_17_5", 
        {(MUSB_DIR_IN|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_READ_NODE_ID),
          (0),
          (0),
          (0x06)
        }
    },

    {"MUSB_REQ_VENDER_19_6", 
        {(MUSB_DIR_IN|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_READ_PHY_ID),
          (0),
          (0),
          (0x02)
        }
    },

    {"MUSB_REQ_VENDER_06_7", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_SET_SW_MII),
          (0),
          (0),
          (0)
        }
    },

    {"MUSB_REQ_VENDER_08_8", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_WRITE_MII_REG),
          (0x03),
          (0),
          (0x02)
        }
    },

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

    {"MUSB_REQ_VENDER_08_11", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_WRITE_MII_REG),
          (0x03),
          (0x04),
          (0x02)
        }
    },

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

    {"MUSB_REQ_VENDER_08_17", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_WRITE_MII_REG),
          (0x03),
          (0),
          (0x02)
        }
    },    

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

    {"MUSB_REQ_VENDER_16_20", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_WRITE_MULTI_FILTER),
          (0),
          (0),
          (0x08)
        }
    },     
    
    {"MUSB_REQ_VENDER_10_21", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_WRITE_RX_CTL),
          (0x9c),
          (0),
          (0)
        }
    },     

    {"MUSB_REQ_VENDER_16_22", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_WRITE_MULTI_FILTER),
          (0),
          (0),
          (0x08)
        }
    },     

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

static unsigned char eth_enum_write_index = 0;
static unsigned char eth_enum_write_value[5][8] = { {0, 0x80}, {0xe1, 0x05}, {0, 0x32},      // 0x08
            {0, 0, 0, 0x80, 0, 0, 0, 0}, {0, 0, 0, 0x80, 0, 0, 0, 0} };                                           // 0x16
            
/* end of #if defined MUSB_HS_DEVICE */
#elif defined MUSB_FS_DEVICE
static ethenumeration_t ethenumeration_step[] = {
    {"MUSB_REQ_VENDER_17_1", 
        {(MUSB_DIR_IN|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_READ_NODE_ID),
          (0),
          (0),
          (0x06)
        }
    },

    {"MUSB_REQ_VENDER_06_2", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_SET_SW_MII),
          (0),
          (0),
          (0)
        }
    },

    {"MUSB_REQ_VENDER_19_3", 
        {(MUSB_DIR_IN|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_READ_PHY_ID),
          (0),
          (0),
          (0x02)
        }
    },

    {"MUSB_REQ_VENDER_08_4", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_WRITE_MII_REG),
          (0x03),
          (0),
          (0x02)
        }
    },

    {"MUSB_REQ_VENDER_08_5", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_WRITE_MII_REG),
          (0x03),
          (0),
          (0x02)
        }
    },

    {"MUSB_REQ_VENDER_1B_6", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_WRITE_MEDIUM_MODE),
          (0x16),
          (0),
          (0)
        }
    },

    {"MUSB_REQ_VENDER_12_7", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_WRITE_IPG0),
          (0x15),
          (0),
          (0)
        }
    },

    {"MUSB_REQ_VENDER_13_8", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_WRITE_IPG1),
          (0x0c),
          (0),
          (0)
        }
    },

    {"MUSB_REQ_VENDER_14_9", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_WRITE_IPG2),
          (0x12),
          (0),
          (0)
        }
    },

    {"MUSB_REQ_VENDER_08_10", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_WRITE_MII_REG),
          (0x03),
          (0x04),
          (0x02)
        }
    },

    {"MUSB_REQ_VENDER_08_11", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_WRITE_MII_REG),
          (0x03),
          (0),
          (0x02)
        }
    },

    {"MUSB_REQ_VENDER_0A_12", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_SET_HW_MII),
          (0),
          (0),
          (0)
        }
    },

    {"MUSB_REQ_VENDER_10_13", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_WRITE_RX_CTL),
          (0x8c),
          (0),
          (0)
        }
    },

// EP 1 IN

    {"MUSB_REQ_VENDER_06_14", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_SET_SW_MII),
          (0),
          (0),
          (0)
        }
    },    

    {"MUSB_REQ_VENDER_07_15", 
        {(MUSB_DIR_IN|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_READ_MII_REG),
          (0x03),
          (0x05),
          (0x02)
        }
    },    

    {"MUSB_REQ_VENDER_07_16", 
        {(MUSB_DIR_IN|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_READ_MII_REG),
          (0x03),
          (0x02),
          (0x02)
        }
    },    

    {"MUSB_REQ_VENDER_07_17", 
        {(MUSB_DIR_IN|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_READ_MII_REG),
          (0x03),
          (0x03),
          (0x02)
        }
    },    

    {"MUSB_REQ_VENDER_07_18", 
        {(MUSB_DIR_IN|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_READ_MII_REG),
          (0x03),
          (0x19),
          (0x02)
        }
    },  

// ifconfig
    {"MUSB_REQ_VENDER_0A_19", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_SET_HW_MII),
          (0),
          (0),
          (0)
        }
    },  

    {"MUSB_REQ_VENDER_1B_20", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_WRITE_MEDIUM_MODE),
          (0),
          (0),
          (0)
        }
    },     
    
    {"MUSB_REQ_VENDER_10_21", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_WRITE_RX_CTL),
          (0x8e),
          (0),
          (0)
        }
    },     

    {"MUSB_REQ_VENDER_10_22", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_WRITE_RX_CTL),
          (0x80),
          (0),
          (0)
        }
    },     

    {"MUSB_REQ_VENDER_10_23", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_WRITE_RX_CTL),
          (0x8e),
          (0),
          (0)
        }
    },   

    {"MUSB_REQ_VENDER_16_24", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_WRITE_MULTI_FILTER),
          (0),
          (0),
          (0x08)
        }
    },  

// EP 1 IN token

    {"MUSB_REQ_VENDER_06_25", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_SET_SW_MII),
          (0),
          (0),
          (0)
        }
    },

    {"MUSB_REQ_VENDER_07_26", 
        {(MUSB_DIR_IN|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_READ_MII_REG),
          (0x03),
          (0x05),
          (0x02)
        }
    }, 

    {"MUSB_REQ_VENDER_0A_27", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_SET_HW_MII),
          (0),
          (0),
          (0)
        }
    }, 

    {"MUSB_REQ_VENDER_1B_28", 
        {(MUSB_DIR_OUT|MUSB_TYPE_VENDOR|MUSB_RECIP_DEVICE),
          (AX_CMD_WRITE_MEDIUM_MODE),
          (0x16),
          (0),
          (0)
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

static unsigned char eth_enum_write_index = 0;
static unsigned char eth_enum_write_value[5][8] = { {0, 0}, {0, 0x80}, {0xe1, 0x05}, {0, 0x12},       // 0x08
                                                                             {0x40, 0x16, 0, 0, 0, 0, 0x08, 0} };                  // 0x16
                                                                             
#endif
/* end of #elif defined MUSB_FS_DEVICE */
    
#endif


/************************************************************************/

//int EP0ControlRequest(hdrc_t *pCtrl, MUSB_DeviceRequest *pDevReq);
void MGC_HdrcServiceDefaultEnd(hdrc_t* pCtrl);
void MGC_HdrcServiceDefaultEnd(hdrc_t* pCtrl);
void HdrcServiceDefaultEnd(hdrc_t* pCtrl);
void MGC_HdrcStartTx(hdrc_t* pCtrl, unsigned char bEnd);
unsigned char MGC_SetPortReset(hdrc_t* pCtrl, unsigned char bIPReset);
unsigned int MGC_SendRecvEP (hdrc_t* pCtrl, unsigned char bEnd, unsigned int bXmt,
                                                    unsigned char* pBuffer, unsigned int dwLength, unsigned char bDMAMode);
extern int HdrcStop (hdrc_t* pCtrl);

#if defined MUSB_DEV_USB2ETH
int ETHControlRequest (hdrc_t *pCtrl, MUSB_DeviceRequest *pDevReq);
STATIC unsigned char ETHEPRequest (hdrc_t* pCtrl, unsigned int dwCount);
void HdrcServiceETHEnd (hdrc_t* pCtrl);

#elif defined MUSB_DEV_MASSSTORAGE
int MSEPRequest (hdrc_t *pCtrl, bulk_cb_wrap_t *pCBW);
void HdrcServiceMSEnd(hdrc_t* pCtrl);

#elif defined MUSB_DEV_7108
#if 0 //!PORTING
STATIC void Set7108EndConfig (hdrc_t *pCtrl);
int USB7108ControlRequest (hdrc_t *pCtrl, MUSB_DeviceRequest *pDevReq);
STATIC unsigned char USB7108EPRequest (hdrc_t* pCtrl, unsigned int dwCount);
void HdrcService7108End (hdrc_t* pCtrl);
#endif
#endif

#endif /* _MUSB_HOST_H */

