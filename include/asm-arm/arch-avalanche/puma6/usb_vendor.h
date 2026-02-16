/*
 *
 * usb_vendor.h 
 * Description:
 *
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
 *
 */
#ifndef _USB_VENDOR_H
#define _USB_VENDOR_H

/* vendor specific configuration definitons
   for usb cdc/rndis gadget driver */
#define DEFAULT_USB_VENDOR_ID           0x0451 /* TI */
#define DEFAULT_USB_PRODUCT_PUMA3       0x6104 /* TI-CM Puma3*/
#define DEFAULT_USB_PRODUCT_PUMAS       0x8050 /* TI-CM PumaS*/
#define DEFAULT_USB_PRODUCT_PUMA5       0x6060 /* Puma5*/
#define DEFAULT_USB_PRODUCT_ID          (DEFAULT_USB_PRODUCT_PUMA5)
#define DEFAULT_USB_VENDOR_NAME         "Texas Instruments"
#define DEFAULT_USB_VENDOR_DESC         "Texas Instruments RNDIS Adapter"
#define DEFAULT_USB_INSTANCE            "000001"
#define DEFAULT_USB_CONFIG              "USB RNDIS Configuartion"
#define DEFAULT_USB_COMM_IF             "Communication Interface"
#define DEFAULT_USB_DATA_IF             "Data Interface"
#define DEFAULT_USB_PC_MAC_ADDRESS      "00:e1:a7:76:76:85"
#define DEFAULT_USB_DEVICE_MAC_ADDRESS  "00:e0:a6:75:75:80"

#endif 
