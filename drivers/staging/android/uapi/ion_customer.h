////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   ion_customer.h
/// @brief  ion customer command driver IO interface
/// @author MStar Semiconductor Inc.
///
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _UAPI_LINUX_ION_CUSTOMER_H
#define _UAPI_LINUX_ION_CUSTOMER_H

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "mdrv_mtlb_st.h"

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef enum {    
    E_ADDR_MTLB = 0,
    E_ADDR_PHY
} EN_MTLB_AddressType;

typedef struct {
    //input parameters
    ion_user_handle_t handle;
    EN_MTLB_TLB_Client client;
    EN_MTLB_AddressType addrType;

    //output parameters
    unsigned long mapaddr;
    unsigned char miu;
    unsigned long miu_offset;

    unsigned char tlb_table_change;
} ion_cust_alloc_data;

typedef struct {
    //input parameters
    pid_t pid;
    int fd;

    //output parameters
    int newfd;
} ion_cust_import;

//-------------------------------------------------------------------------------------------------
//  IO command
//-------------------------------------------------------------------------------------------------
#define MTLB_CUST_IOC_MAGIC   'C'

#define MTLB_CUST_IOC_GET_HWINO             _IOR(MTLB_CUST_IOC_MAGIC, 0x00, mtlb_hardware_info)
#define MTLB_CUST_IOC_GET_HWSTATUS          _IOR(MTLB_CUST_IOC_MAGIC, 0x01, mtlb_hardware_status)
#define MTLB_CUST_IOC_GET_TABLESIZE         _IOR(MTLB_CUST_IOC_MAGIC, 0x02, unsigned long)
#define MTLB_CUST_IOC_INIT                  _IO(MTLB_CUST_IOC_MAGIC, 0x03)
#define MTLB_CUST_IOC_ENABLE                _IOW(MTLB_CUST_IOC_MAGIC, 0x04, mtlb_tlbclient_enable)
#define MTLB_CUST_IOC_LOCK                  _IOWR(MTLB_CUST_IOC_MAGIC, 0x05, ion_cust_alloc_data)
#define MTLB_CUST_IOC_UNLOCK                _IOWR(MTLB_CUST_IOC_MAGIC, 0x06, ion_cust_alloc_data)
#define MTLB_CUST_IOC_IMPORT                _IOWR(MTLB_CUST_IOC_MAGIC, 0x07, ion_cust_import)

#endif

