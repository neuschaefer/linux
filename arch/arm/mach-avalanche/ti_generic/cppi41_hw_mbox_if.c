/*
 *
 * cppi41_hw_mbox_if.c 
 * Description:
 * CPPI access via HW mailbox
 *
 *
 * GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2015 Intel Corporation.
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


/************************************************/
/*                    Includes                  */
/************************************************/
#include <linux/module.h>
#include <linux/moduleparam.h>  // to support input parameters for the module
#include <linux/kernel.h>
#include <linux/byteorder/generic.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/interrupt.h>
#include <hw_mbox_drv.h>
#include "hw_mbox_defs.h"
#include "pal_cppi41.h"
#include "pal_cppi41pvt.h"
#include "puma7.h"
#include "avalanche_intd.h"
#include "puma7_npcpu_appcpu_defs.h"

#include <asm-arm/arch-avalanche/puma7/cppi41_hw_mbox_if.h>
#include <linux/mrpc/handshake_server.h>
/************************************************/
/*                    Defines                   */
/************************************************/
//#define CPPI_MBOX_DBG

#ifdef CPPI_MBOX_DBG
/* Debug print, also print function name and line number */
#  define DPRINTK(fmt, args...) printk("[CPPI-MBOX-DBG] %s(%d): " fmt "\n", __FUNCTION__ , __LINE__,  ## args)
#else
#  define DPRINTK(fmt, args...)
#endif
/* Error print, also print function name and line number */
#define EPRINTK(fmt, args...) printk(KERN_ERR "[CPPI-MBOX-ERROR] %s(%d): " fmt "\n", __FUNCTION__ , __LINE__, ## args)

/************************************************/
/*        hw mailbox driver  Functions          */
/************************************************/
extern int hwMbox_isReady(void);
extern int hwMbox_registerRecvOpcode(hw_mbox_Masters_e commander, hwMboxRecvOpcodeCallback pOpcodeCallbackFunc, Uint32 tag, Uint32 token);
extern int hwMbox_sendReplyOpcode(hw_mbox_Masters_e commander, Uint8 *dataBuf, Uint32 dataLen);
extern int hwMbox_sendAckOpcode(hw_mbox_Masters_e commander);

/************************************************/
/*          Interface Functions                 */
/************************************************/
static int cppi41_hw_mbox_callback(hw_mbox_Masters_e commander, Uint8 *dataBuf, Uint32 dataLen, Uint32 *token);
static int cppi41_hw_mbox_acc_ch_open_handler(Cppi4AccumulatorCfg *initCfg, PAL_Cppi4AccChHnd *accChHnd, Ptr *curPage);
static int cppi41_hw_mbox_acc_ch_close_handler(PAL_Cppi4AccChHnd accChHnd);
static int cppi41_hw_mbox_desc_region_mem_info_handler(PAL_CPPI_PP_DESC_REGIONs_e region, Uint32 qMgr, Ptr *regionPhyAddr, Uint32 *length);
static int cppi41_hw_mbox_offchip_info_handler(Uint32 *offChipPhyAddr, Uint32 *length);
static int cppi41_hw_mbox_offchip_bPool_info_handler(Uint32 pool, Uint32 *offChipPoolPhyAddr, Uint32 *length);


/************************************************/
/*         Global and Static Variables          */
/************************************************/

/* handle to PAL */
static PAL_Handle cppi_hw_mobx_hnd = NULL;

/*****************************************************************************************/
/**  ___  ___            _         _                                                    **/
/**  |  \/  |           | |       | |                                                   **/
/**  | .  . |  ___    __| | _   _ | |  ___                                              **/
/**  | |\/| | / _ \  / _` || | | || | / _ \                                             **/
/**  | |  | || (_) || (_| || |_| || ||  __/                                             **/
/**  \_|  |_/ \___/  \__,_| \__,_||_| \___|                                             **/
/**  ______                    _    _                                                   **/
/**  |  ___|                  | |  (_)                                                  **/
/**  | |_  _   _  _ __    ___ | |_  _   ___   _ __   ___                                **/
/**  |  _|| | | || '_ \  / __|| __|| | / _ \ | '_ \ / __|                               **/
/**  | |  | |_| || | | || (__ | |_ | || (_) || | | |\__ \                               **/
/**  \_|   \__,_||_| |_| \___| \__||_| \___/ |_| |_||___/                               **/
/*****************************************************************************************/

/*! \fn int cppi41_hw_mbox_init (void) 
  * \brief initialize CPPI hw mailbox interface 
  * \return 0 on succuss, non zero value otherwise 
  */
static int __init __cppi41_hw_mbox_init(void)
{
    /* HW Mailbox checking and registeration */
    if ( 0 != hwMbox_isReady() )
    {
        EPRINTK("HW Mailbox driver not ready, init fails!");
        return -1;
    }

    DPRINTK("HW Mailbox driver is ready");

    cppi_hw_mobx_hnd = PAL_cppi4Init(NULL, CPPI41_DOMAIN_PP);
    if (NULL == cppi_hw_mobx_hnd)
    {
        EPRINTK("Failed to get PP domain PAL handle");
        return -1;
    }

    DPRINTK("PP Domain PAL handle = %p", cppi_hw_mobx_hnd);


    /* HW Mailbox is ready, registering the callback */
    if(0 != hwMbox_registerRecvOpcode(HW_MBOX_MASTER_APP_CPU, cppi41_hw_mbox_callback,
                                      NPCPU_APPCPU_HW_MBOX_TAG_CPPI41_MBX, 0 ))
    {
        EPRINTK("hwMbox_registerRecvOpcode fails, Master = %d, Tag = %d",
                HW_MBOX_MASTER_APP_CPU, NPCPU_APPCPU_HW_MBOX_TAG_CPPI41_MBX);
        return -1;
    }

    printk("CPPI41 HW Mailbox: init DONE\n");

#ifdef CONFIG_MRPC_HANDSHAKE_SERVER
        boot_status_report(HANDSHAKE_ID_CPPI_HW_MBOX);
#endif

    return 0;
}
//EXPORT_SYMBOL(cppi41_hw_mbox_init);
late_initcall(__cppi41_hw_mbox_init);


/*! \fn int cppi41_hw_mbox_deinit(void)
  * \brief de initialize CPPI hw mailbox interface 
  * \return 0 on succuss
  */
static void __exit __cppi41_hw_mbox_deinit(void)
{
    DPRINTK("Unregister interface callback from hw mailbox driver");
    /* TBD - unregister callback */

    DPRINTK("Removing cppi hw mailbox interface");
    PAL_cppi4Exit(NULL, CPPI41_DOMAIN_PP);
}
//EXPORT_SYMBOL(cppi41_hw_mbox_deinit);
module_exit(__cppi41_hw_mbox_deinit);


/*!
 * \fn static int cppi41_hw_mbox_callback(hw_mbox_Masters_e commander, Uint8 *dataBuf, Uint32 dataLen, Uint32 *token) 
 * \brief handles all cppi41 incoming hw mailbox commands
 * 
 * \author iemergi (11/19/2015)
 * 
 * \param commander hw mailbox commander
 * \param dataBuf data buffer
 * \param dataLen data length
 * \param token token
 * 
 * \return 0 for succuss
 */
static int cppi41_hw_mbox_callback(hw_mbox_Masters_e commander, Uint8 *dataBuf, Uint32 dataLen, Uint32 *token)
{
    Cppi41HwMboxCmd_e cmd;
    Cppi41HwMboxFailMsg_t failReply = {0};

    DPRINTK("start");

    if (dataBuf == NULL)
    {
        EPRINTK("Received NULL pointer");
        goto out_err;
    }

    /* the CMD ID MUST be the first word (32bits) in all CPPI mbox msg types! */
    cmd = *((Cppi41HwMboxCmd_e *)dataBuf);

    switch(cmd)
    {
        case CPPI41_HWMBOX_CMD_ACC_CH_OPEN:
            {
                Cppi41HwMboxAccChOpenMsg_t *AccChOpenMsg;
                Cppi41HwMboxAccChOpenReplyMsg_t AccChOpenReply = {0};

                AccChOpenMsg = (Cppi41HwMboxAccChOpenMsg_t *) dataBuf;

                DPRINTK("calling cppi41_hw_mbox_acc_ch_open_handler");
                if(0 == cppi41_hw_mbox_acc_ch_open_handler(&AccChOpenMsg->initCfg, 
                                                           &AccChOpenReply.accChHnd, 
                                                           &AccChOpenReply.curPage))
                {
                    /* open channel done ok */
                    AccChOpenReply.cmd = CPPI41_HWMBOX_CMD_ACC_CH_OPEN_REPLY;
                    hwMbox_sendReplyOpcode(HW_MBOX_MASTER_APP_CPU, &AccChOpenReply, sizeof(AccChOpenReply));
                    return 0;
                }
                failReply.failCode = CPPI41_HWMBOX_RET_CODE_CMD_ERROR;
                break;
            }
        case CPPI41_HWMBOX_CMD_ACC_CH_CLOSE:
            {
                Cppi41HwMboxAccChCloseMsg_t *AccChCloseMsg;

                AccChCloseMsg = (Cppi41HwMboxAccChCloseMsg_t *) dataBuf;

                DPRINTK("calling cppi41_hw_mbox_acc_ch_close_handler");
                if(0 == cppi41_hw_mbox_acc_ch_close_handler(AccChCloseMsg->accChHnd))
                {
                    hwMbox_sendAckOpcode(HW_MBOX_MASTER_APP_CPU);
                    return 0;
                }
                failReply.failCode = CPPI41_HWMBOX_RET_CODE_CMD_ERROR;
                break;
            }
        case CPPI41_HWMBOX_CMD_GET_REGION_MEM_INFO_REPLY:
            {
                Cppi41HwMboxDescRegionMemInfoGetMsg_t *DescRegionMemInfoGetMsg;
                Cppi41HwMboxDescRegionMemInfoReplyMsg_t DescRegionMemInfoReply;

                DescRegionMemInfoGetMsg = (Cppi41HwMboxDescRegionMemInfoGetMsg_t *) dataBuf;
                DPRINTK("calling cppi41_hw_mbox_desc_region_mem_info_handler");
                if(0 == cppi41_hw_mbox_desc_region_mem_info_handler(DescRegionMemInfoGetMsg->region,
                                                                    DescRegionMemInfoGetMsg->qMgr,
                                                                    &DescRegionMemInfoReply.regionPhyAddr,
                                                                    &DescRegionMemInfoReply.length))
                {
                    DescRegionMemInfoReply.cmd = CPPI41_HWMBOX_CMD_GET_REGION_MEM_INFO_REPLY;
                    hwMbox_sendReplyOpcode(HW_MBOX_MASTER_APP_CPU, &DescRegionMemInfoReply, sizeof(DescRegionMemInfoReply));
                    return 0;
                }
                failReply.failCode = CPPI41_HWMBOX_RET_CODE_CMD_ERROR;
                break;
            }
        case CPPI41_HWMBOX_CMD_GET_OFFCHIP_MEM_INFO_REPLY:
            {
                Cppi41HwMboxOffChipMemInfoReplyMsg_t offChipMemInfoReply;

                DPRINTK("calling cppi41_hw_mbox_offchip_info_handler");
                if(0 == cppi41_hw_mbox_offchip_info_handler(&offChipMemInfoReply.offChipPhyAddr,
                                                            &offChipMemInfoReply.length))
                {
                    offChipMemInfoReply.cmd = CPPI41_HWMBOX_CMD_GET_OFFCHIP_MEM_INFO_REPLY;
                    hwMbox_sendReplyOpcode(HW_MBOX_MASTER_APP_CPU, &offChipMemInfoReply, sizeof(offChipMemInfoReply));
                    return 0;
                }
                failReply.failCode = CPPI41_HWMBOX_RET_CODE_CMD_ERROR;
                break;
            }
        case CPPI41_HWMBOX_CMD_GET_BPOOL_MEM_INFO_REPLY:
            {
                Cppi41HwMboxCppiBpoolMemInfoGetMsg_t *bPoolMemInfoGetMsg;
                Cppi41HwMboxCppiBpoolMemInfoReplyMsg_t bPoolMemInfoReply;

                bPoolMemInfoGetMsg = (Cppi41HwMboxCppiBpoolMemInfoGetMsg_t *) dataBuf;
                DPRINTK("calling cppi41_hw_mbox_offchip_bPool_info_handler with pool #%d", bPoolMemInfoGetMsg->poolNum);
                if(0 == cppi41_hw_mbox_offchip_bPool_info_handler(bPoolMemInfoGetMsg->poolNum,
                                                            &bPoolMemInfoReply.offChipPhyAddr,
                                                            &bPoolMemInfoReply.length))
                {
                    bPoolMemInfoReply.cmd = CPPI41_HWMBOX_CMD_GET_BPOOL_MEM_INFO_REPLY;
                    hwMbox_sendReplyOpcode(HW_MBOX_MASTER_APP_CPU, &bPoolMemInfoReply, sizeof(bPoolMemInfoReply));
                    return 0;
                }
                failReply.failCode = CPPI41_HWMBOX_RET_CODE_CMD_ERROR;
                break;
            }
        default:
            EPRINTK("Unsupported command code = %d, replying fail code", cmd);
            failReply.failCode = CPPI41_HWMBOX_RET_CODE_UNSUPPORTED_CMD;
            break;
    }

out_err:   
    failReply.cmd = CPPI41_HWMBOX_CMD_FAILED;
 
    hwMbox_sendReplyOpcode(HW_MBOX_MASTER_APP_CPU, &failReply, sizeof(failReply));
    EPRINTK("Failed to handle incoming message");
    
    return 0;
}

#define ACCUM_CH_PARAM_DEBUG(initCfg)     \
            DPRINTK("\n                                  \   
                     accChanNum=%d \n                    \   
                     mode=%d \n                          \   
                     qMg=%d \n                           \   
                     qNum=%d \n                          \   
                     pacingTickCnt=%d \n                 \   
                     list.istBase=%p \n                  \   
                     list.axPageEntry=%d \n              \   
                     list.acingMode=%d \n                \   
                     list.tallAvoidance=%d \n            \   
                     list.istCountMode=%d \n             \   
                     list.istEntrySize=%d \n             \   
                     list.axPageCnt=%d \n                \   
                     monitor.pktCountThresh=%d \n        \   
                     monitor.pacingMode=%d \n",          \                                        
                     initCfg->accChanNum,                \
                     initCfg->mode,                      \
                     initCfg->queue.qMgr,                \
                     initCfg->queue.qNum,                \
                     initCfg->pacingTickCnt,             \
                     initCfg->list.listBase,             \
                     initCfg->list.maxPageEntry,         \
                     initCfg->list.pacingMode,           \
                     initCfg->list.stallAvoidance,       \
                     initCfg->list.listCountMode,        \
                     initCfg->list.listEntrySize,        \
                     initCfg->list.maxPageCnt,           \
                     initCfg->monitor.pktCountThresh,    \
                     initCfg->monitor.pacingMode)


static int cppi41_hw_mbox_acc_ch_open_handler(Cppi4AccumulatorCfg *initCfg, PAL_Cppi4AccChHnd *accChHnd, Ptr *curPage)
{
    PAL_Cppi4AccChObj *accChObj;
    DPRINTK("start");

    if(NULL == initCfg)
    {
        EPRINTK("initCfg is NULL, cannot open accumulator channel without initCfg");
        return -1;
    }

    ACCUM_CH_PARAM_DEBUG(initCfg);

    accChObj = ( PAL_Cppi4AccChObj *) PAL_cppi4AccChOpenAppCpu(cppi_hw_mobx_hnd, initCfg);
    DPRINTK("PAL_cppi4AccChOpenAtom return accChObj = %p", accChObj);

    if(!accChObj)
    {
        EPRINTK("Failed to open accumulator channel %d", initCfg->accChanNum);
        return -1;
    }

    DPRINTK("succussfully open accumulator channel %d", initCfg->accChanNum);

    *curPage = accChObj->curPage;
    *accChHnd = accChObj;
    DPRINTK("accChHnd = *p, curPage = %p", *accChHnd, *curPage);
    return 0;
}

static int cppi41_hw_mbox_acc_ch_close_handler(PAL_Cppi4AccChHnd accChHnd)
{
    PAL_cppi4AccChClose(accChHnd, NULL);
    DPRINTK("close accumulator channel succussfully for accChHnd = %p",accChHnd);

    return 0;
}

static int cppi41_hw_mbox_desc_region_mem_info_handler(PAL_CPPI_PP_DESC_REGIONs_e region, Uint32 qMgr, Ptr *regionPhyAddr, Uint32 *length)
{
    PAL_cppi4GetDescRegionMemInfo(cppi_hw_mobx_hnd, region, qMgr, regionPhyAddr, length);
    *regionPhyAddr = PAL_CPPI4_VIRT_2_PHYS(*regionPhyAddr);
    DPRINTK("region%d info: base address = %p, length = %d" ,region, *regionPhyAddr, *length);

    return 0;
}

static int cppi41_hw_mbox_offchip_info_handler(Uint32 *offChipPhyAddr, Uint32 *length)
{
    PAL_cppi4GetOffChipAddr(cppi_hw_mobx_hnd, offChipPhyAddr, length);
    *offChipPhyAddr = PAL_CPPI4_VIRT_2_PHYS((void*)(*offChipPhyAddr));

    DPRINTK("offChip info: base address = 0x%x, length = %d", *offChipPhyAddr, *length);

    return 0;
}

static int cppi41_hw_mbox_offchip_bPool_info_handler(Uint32 pool, Uint32 *offChipPoolPhyAddr, Uint32 *length)
{
    PAL_cppi4GetOffChipBpoolAddr(cppi_hw_mobx_hnd, pool, offChipPoolPhyAddr, length);
    *offChipPoolPhyAddr = (virt_addr_valid(*offChipPoolPhyAddr) ? PAL_CPPI4_VIRT_2_PHYS((void*)(*offChipPoolPhyAddr)) : IO_VIRT2PHY(*offChipPoolPhyAddr));

    DPRINTK("offChip buffer pool info: base address = 0x%x, length = %d", *offChipPoolPhyAddr, *length);
    return 0;
}

bool Cppi41HwMboxAccChangeEndianness(Cppi41HwMboxAccChOpenMsg_t *destCfgData, endianness_e endianity)
{
    if( !destCfgData )
    {
        EPRINTK(" null pointer reference ");
        return false;
    }
    if( (endianity != endiannessBig) && (endianity != endiannessLittle) )
    {
        EPRINTK(" Endianness value pass in datapipe is not correct ");
        return false; 
    }

    if(endianity == endiannessBig)
    {
        (*destCfgData).initCfg.accChanNum               =   cpu_to_be32((*destCfgData).initCfg.accChanNum);
        (*destCfgData).initCfg.mode                     =   cpu_to_be32((*destCfgData).initCfg.mode);
        (*destCfgData).initCfg.queue.qMgr               =   cpu_to_be32((*destCfgData).initCfg.queue.qMgr);
        (*destCfgData).initCfg.queue.qNum               =   cpu_to_be32((*destCfgData).initCfg.queue.qNum);
        (*destCfgData).initCfg.pacingTickCnt            =   cpu_to_be32((*destCfgData).initCfg.pacingTickCnt);
        (*destCfgData).initCfg.list.listBase            =   (void *)cpu_to_be32((unsigned int)(*destCfgData).initCfg.list.listBase);
        (*destCfgData).initCfg.list.maxPageEntry        =   cpu_to_be32((*destCfgData).initCfg.list.maxPageEntry);
        (*destCfgData).initCfg.list.pacingMode          =   cpu_to_be32((*destCfgData).initCfg.list.pacingMode);
        (*destCfgData).initCfg.list.stallAvoidance      =   cpu_to_be32((*destCfgData).initCfg.list.stallAvoidance);
        (*destCfgData).initCfg.list.listCountMode       =   cpu_to_be32((*destCfgData).initCfg.list.listCountMode);
        (*destCfgData).initCfg.list.listEntrySize       =   cpu_to_be32((*destCfgData).initCfg.list.listEntrySize);
        (*destCfgData).initCfg.list.maxPageCnt          =   cpu_to_be32((*destCfgData).initCfg.list.maxPageCnt);
        (*destCfgData).initCfg.monitor.pktCountThresh   =   cpu_to_be32((*destCfgData).initCfg.monitor.pktCountThresh);
        (*destCfgData).initCfg.monitor.pacingMode       =   cpu_to_be32((*destCfgData).initCfg.monitor.pacingMode);
    }
    if(endianity == endiannessLittle)
    {
        (*destCfgData).initCfg.accChanNum               =   be32_to_cpu((*destCfgData).initCfg.accChanNum);
        (*destCfgData).initCfg.mode                     =   be32_to_cpu((*destCfgData).initCfg.mode);
        (*destCfgData).initCfg.queue.qMgr               =   be32_to_cpu((*destCfgData).initCfg.queue.qMgr);
        (*destCfgData).initCfg.queue.qNum               =   be32_to_cpu((*destCfgData).initCfg.queue.qNum);
        (*destCfgData).initCfg.pacingTickCnt            =   be32_to_cpu((*destCfgData).initCfg.pacingTickCnt);
        (*destCfgData).initCfg.list.listBase            =   (void *)be32_to_cpu((*destCfgData).initCfg.list.listBase);
        (*destCfgData).initCfg.list.maxPageEntry        =   be32_to_cpu((*destCfgData).initCfg.list.maxPageEntry);
        (*destCfgData).initCfg.list.pacingMode          =   be32_to_cpu((*destCfgData).initCfg.list.pacingMode);
        (*destCfgData).initCfg.list.stallAvoidance      =   be32_to_cpu((*destCfgData).initCfg.list.stallAvoidance);
        (*destCfgData).initCfg.list.listCountMode       =   be32_to_cpu((*destCfgData).initCfg.list.listCountMode);
        (*destCfgData).initCfg.list.listEntrySize       =   be32_to_cpu((*destCfgData).initCfg.list.listEntrySize);
        (*destCfgData).initCfg.list.maxPageCnt          =   be32_to_cpu((*destCfgData).initCfg.list.maxPageCnt);
        (*destCfgData).initCfg.monitor.pktCountThresh   =   be32_to_cpu((*destCfgData).initCfg.monitor.pktCountThresh);
        (*destCfgData).initCfg.monitor.pacingMode       =   be32_to_cpu((*destCfgData).initCfg.monitor.pacingMode);
    }
    return true;
}
