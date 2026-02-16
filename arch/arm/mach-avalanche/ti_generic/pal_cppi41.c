/*
 *
 * pal_cppi41.c
 * Description:
 * see below
 *
 *
 * GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2009-2017 Intel Corporation.
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

/** \file   pal_cppi41.c
    \brief  PAL CPPI 4 Source file

    This file contains the main PAL implementation for CPPI4 common peripherals,
    including the CPPI 4 DMA Controller, the Queue Manager, and the Buffer
    Manager.  Based upon PSP Framework architecture.

    @author     Greg Guyotte
    @author     Sekhar Nori
 */
#include <asm-arm/arch-avalanche/generic/pal.h> /* PAL SYS/OS services required */
#include <asm-arm/arch-avalanche/generic/pal_cppi41.h>
#include <asm-arm/arch-avalanche/generic/pal_cppi41pvt.h>
#include <asm-arm/arch-avalanche/generic/avalanche_pp_api.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>    /* for copy_from_user */

#if PUMA7_OR_NEWER_SOC_TYPE
#include <asm-arm/arch-avalanche/puma7/puma7.h>
#include <asm-arm/arch-avalanche/puma7/puma7_npcpu_appcpu_defs.h>
#include <asm-arm/arch-avalanche/puma7/cppi41_hw_mbox_if.h>
#include <asm-arm/arch-avalanche/generic/avalanche_pdsp_api.h>
#include <hw_mbox_drv.h>
#include "puma7_pp.h"
#include "hw_mbox_defs.h"                 /* HW_MBOX definitions and configuration */
#endif

/* Version macro */
#define CPPI4_PAL_MAJOR_VERSION           0
#define CPPI4_PAL_MINOR_VERSION           1

//#define  CPPI4_DEBUG 1
//#define  CPPI4_DEBUG_PUSH_POP 1

/*===================== Power Management =======================*/
#define PP_HAL_PPS_FETCH_ADDR_PHY 0xF3C313C0
#define PAL_CPPI_PP_SHARED_RX_LOW_1KB_BUFFER_POOL_LPM_DELTA  250
#define PAL_CPPI_PP_SHARED_RX_LOW_2KB_BUFFER_POOL_LPM_DELTA  250
#define PAL_CPPI_PP_SHARED_RX_HIGH_BUFFER_POOL_LPM_DELTA     250
#define PAL_CPPI_PP_PACKET_RAM_512B_BUFFER_POOL_LPM_DELTA    250
#define NOT_MONITORED                                        0
/*==============================================================*/

#  define errPrint(fmt, args...) printk(KERN_ERR "%s:%d " fmt, __FUNCTION__ , __LINE__ , ## args)

#ifdef CPPI4_DEBUG
#  define dbgPrint(fmt, args...) printk("%s:%d " fmt, __FUNCTION__ , __LINE__ , ## args)
#else
#  define dbgPrint(fmt, args...)
#endif

#ifdef CPPI4_DEBUG_PUSH_POP
#  define dbgPrintPushPop(fmt, args...) printk("%s:%d " fmt, __FUNCTION__ , __LINE__ , ## args)
#else
#  define dbgPrintPushPop(fmt, args...)
#endif

const static Char Cppi4PALVersionString[] = "CPPI4 PAL version 0.1";

/* Static Global Instance Variable */
static Cppi4PALObj      Cppi4PALObject  [CPPI41_DOMAIN_NUM];
static Uint32           Cppi4PALNumInst [CPPI41_DOMAIN_NUM] = { 0 };
static Cppi4InitCfg     Cppi4PvtInitCfg [CPPI41_DOMAIN_NUM];
static struct proc_dir_entry * cppiDebugDir = NULL;

#define OUTPUT_MAX_READ_OPTION_LEN 1024 
static unsigned char cppi_output_read_option_str[OUTPUT_MAX_READ_OPTION_LEN]; 
#ifdef CONFIG_PRINTK
#define PRINT_OUTPUT(fmt, args...) printk(fmt, ## args)
#else
#define PRINT_OUTPUT(fmt, args...) { snprintf(cppi_output_read_option_str, OUTPUT_MAX_READ_OPTION_LEN, fmt, ## args); }
#endif

/* External Global Instance Variables */
extern qsmonRal_t gBsmon[PAL_CPPI41_BMGR_MAX_POOLS];
extern Cppi4InitCfg gCppiPpInitCfg;
#if (!defined(CONFIG_INTEL_KERNEL_PP_DRIVER_LOCAL) && defined(CONFIG_MRPC_CPPI_CLIENT))
extern Uint32 avalanche_cppi_alloc_pp_buffer_pool(Uint32 size);
#endif

/* !@@
 * TODO: move in .h
 * TODO: Currenly using dummy routine, need to call proper PAL
 */

#define PAL_VirtToPhysMem(ptr) (virt_to_phys (ptr))
#define CPPI4_VIRT_TO_PHYS(ptr) (PAL_VirtToPhysMem (ptr))

/************************ CPPI4.1 PAL Internal functions prototypes **************/
static void __PAL_cppi4_get_buf_pool_resources_data_for_pm(Uint32 bufPoolID, Uint32* deltaP, Uint32* resourcesNumP);
int __PAL_cppi4_read_pm_info(struct file *filp, char *resultBuf, size_t count, loff_t *offp);

/************************ CPPI4.1 PAL FUNCTIONS (External Interface) *************/

/*
 * PAL_cppi4Init
 *  - initializes the PAL CPPI4.1 instance for first caller only
 *  - returns handle to PAL
 *
 */
PAL_Handle PAL_cppi4Init(Cppi4InitCfg * initCfg, Ptr param)
{
#if (PUMA5_SOC_TYPE || PUMA6_SOC_TYPE)
    Cppi4TeardownDesc * tempBD = NULL;
#else
    Int32 initalizedGQMGR = -1;
#endif
    Cppi4PALObj *palCppi4Obj;
    Uint32 i = 0, j = 0;


    if (CPPI41_DOMAIN_NUM <= (CPPI41_DOMAIN)param)
    {
        return NULL;
    }

    if (initCfg)
    {
        printk("%s:%d Domain is %d, cfg ptr is 0x%08X\n", __FUNCTION__, __LINE__, (Uint32)param, (Uint32)initCfg);
    }

    palCppi4Obj = &Cppi4PALObject[(CPPI41_DOMAIN)param];

    /* Check if CPPI4 is already initialized.
     * TODO: Ideally this should be protected by locks, but can't think
     * of a case where multiple guys will be in CPPI4.1 init simaltaneously.
     * */
    if (Cppi4PALNumInst[(CPPI41_DOMAIN)param]++ > 0)
    {
        return ((PAL_Handle) palCppi4Obj);
    }


    if(initCfg == NULL) goto init_failed;

    PAL_osMemSet (palCppi4Obj, 0, sizeof (Cppi4PALObj));
    palCppi4Obj->myDomain = (CPPI41_DOMAIN)param;

    /* Store a local copy of the initial configuration (so that caller is free to free up
     * his configuration structure)
     */
    PAL_osMemCopy(&Cppi4PvtInitCfg[(CPPI41_DOMAIN)param], initCfg, sizeof(Cppi4InitCfg));
    /* Set CPPI4 object variables.
     *
     * Note: I assume here that accessing config data from init configuration structure is OK.
     * It will be one more level of indirection this way, but will avoid repeated information.
     * If there are performance concerns, it may be possible to replicate some (oft used)
     * initial configuration parameters in the PAL CPPI 4.1 object as well.
     */
    palCppi4Obj->initCfg = &Cppi4PvtInitCfg[(CPPI41_DOMAIN)param];

    /* use the new (private) structure now on... */
    initCfg = palCppi4Obj->initCfg;
    /* Initialize the queue manager(s) */

    for(i = 0; i < PAL_CPPI41_NUM_QUEUE_MGR; i++)
    {
        Cppi4QueueMgrCfg *curMgr = &(initCfg->queueMgrInfo[i]);
        CSL_Queue_Manager_Region_RegsOvly qMgrRegs = curMgr->queueMgrRgnBase;
        Uint32 size = 0, index = 0;
        Ptr buf = NULL;
        Uint32 offChipStart = PAL_CPPI41_MAX_DESC_REGIONS, offChipEnd = PAL_CPPI41_MAX_DESC_REGIONS; /* Set offChipStart and offChipEnd indices to invalid values */

        if (NULL == qMgrRegs)
        {
            continue;
        }

        dbgPrint("QMGR [%d] Init ... \n",i);

        /* Check if the Linking ram is capable to hold total number of descriptors, if LinkingRAM1Base is NULL */
        if (!curMgr->LinkingRAM1Base)
        {
            Uint32 totalDesc = 0;
            for (j = 0; j < PAL_CPPI41_MAX_DESC_REGIONS; j++)
            {
                totalDesc += curMgr->descRegion[j].numDesc;
            }

            if (totalDesc > curMgr->LinkingRAM0Size)
            {
                errPrint("\nERROR: Total number of allocated descriptors: %d more than Linking RAM0 size: %d\n", totalDesc, curMgr->LinkingRAM0Size);
                goto init_failed;
            }

            dbgPrint("Total number of allocated descriptors: %d, Linking RAM0 size: %d\n", totalDesc, curMgr->LinkingRAM0Size);
        }

        qMgrRegs->Linking_RAM_Reg0_Base = curMgr->LinkingRAM0Base;
        qMgrRegs->Linking_RAM_Reg0_Size = curMgr->LinkingRAM0Size;
        qMgrRegs->Linking_RAM_Reg1_Base = curMgr->LinkingRAM1Base;

        dbgPrint("Configure Linking RAM0 base REG @0x%p=0x%08X, Linking RAM0 size REG @0x%p=0x%08X, Linking RAM1 base REG @0x%p=0x%08X\n",
                 IO_VIRT2PHY(&qMgrRegs->Linking_RAM_Reg0_Base), curMgr->LinkingRAM0Base,
                 IO_VIRT2PHY(&qMgrRegs->Linking_RAM_Reg0_Size), curMgr->LinkingRAM0Size,
                 IO_VIRT2PHY(&qMgrRegs->Linking_RAM_Reg1_Base), curMgr->LinkingRAM1Base);


        for (j = 0; j < PAL_CPPI41_MAX_DESC_REGIONS; j++)
        {
            if(curMgr->descRegion[j].isOnChip == False)
            {
                break;
            }
        }

        offChipStart = j;
        for (j = offChipStart; j < PAL_CPPI41_MAX_DESC_REGIONS; j++)
        {
            if(curMgr->descRegion[j].isOnChip == True) break;
        }

        offChipEnd = j;

        /* Find the total size of descriptor memory required */
        for (j = offChipStart; j < offChipEnd; j++)
        {
            if (curMgr->descRegion[j].isPreAllocated == False)
            {
                size += (curMgr->descRegion[j].szDesc * curMgr->descRegion[j].numDesc);
            }
        }

#if PUMA7_OR_NEWER_SOC_TYPE
        if ((CPPI41_DOMAIN_PP == palCppi4Obj->myDomain) && (PAL_CPPI_PP_QMGR_LOCAL != i))
        {
            if (initalizedGQMGR != -1)
            {
                buf =   palCppi4Obj->qMgrDescRegPtr [ initalizedGQMGR ];
                size =  palCppi4Obj->qMgrDescRegSize[ initalizedGQMGR ];
            }
            else
            {
                if (!(buf = PAL_osMemAllocSizeAligned(palCppi4Obj->myDomain, size)))
                {
                    printk("\nERROR: Failed to allocate descriptor memory\n");
                    goto init_failed;
                }
                initalizedGQMGR = i;
            }
        }
        else
#endif
        {
            if (size)
            {
                if (!(buf = PAL_osMemAllocSizeAligned(palCppi4Obj->myDomain, size)))
                {
                    printk("\nERROR: Failed to allocate descriptor memory\n");
                    goto init_failed;
                }
            }
        }

        if (size)
        {
            dbgPrint("offChip starting region: %d, offChip ending region: %d, offChip total memory allocated: %d Bytes, Base memory address: 0x%p", offChipStart, offChipEnd, size, CPPI4_VIRT_TO_PHYS(buf));
        }

        palCppi4Obj->qMgrDescRegPtr[i] = buf;
        palCppi4Obj->qMgrDescRegSize[i] = size;

        /* Populate the bases for all the reqions */
        for (j = offChipStart; j < offChipEnd; j++)
        {
            if(curMgr->descRegion[j].numDesc > 0)
            {
                if (False == curMgr->descRegion[j].isPreAllocated)
                {
                    curMgr->descRegion[j].base = buf;
                    buf += curMgr->descRegion[j].szDesc * curMgr->descRegion[j].numDesc;
                }
            }
            else
            {
                curMgr->descRegion[j].base = 0;
                curMgr->descRegion[j].szDesc = 0;
                curMgr->descRegion[j].numDesc = 0;
            }
        }

        /* Update the hardware on region bases et. al. */
        for (j = 0; j < PAL_CPPI41_MAX_DESC_REGIONS; j++)
        {
            Uint32 val, powSzDesc, powRegSize;
            CSL_Desc_Mem_Setup_Region_RegsOvly descMemRegs = curMgr->descMemRgnBase;

            if(curMgr->descRegion[j].numDesc > 0)
            {
                if ((curMgr->descRegion[j].isOnChip) || (curMgr->descRegion[j].isPreAllocated))
                {
                    descMemRegs->Desc_Mem_Setup[j].Mem_Reg_Base = (Uint32) curMgr->descRegion[j].base;
                }
                else
                {
                    descMemRegs->Desc_Mem_Setup[j].Mem_Reg_Base = CPPI4_VIRT_TO_PHYS((Ptr)curMgr->descRegion[j].base);
                }


                for (powSzDesc = 0; (32UL << powSzDesc) < curMgr->descRegion[j].szDesc; powSzDesc++);
                for (powRegSize = 0; (32UL << powRegSize) < curMgr->descRegion[j].numDesc; powRegSize++);

                /* Write the control word */
                val = (index << QMGR_MEMREG_CTRL_INDEX_SHIFT) & QMGR_MEMREG_CTRL_INDEX_MASK;
                val |= (powSzDesc << QMGR_MEMREG_CTRL_DESCSZ_SHIFT) & QMGR_MEMREG_CTRL_DESCSZ_MASK;
                val |= (powRegSize << QMGR_MEMREG_CTRL_REGSZ_SHIFT) & QMGR_MEMREG_CTRL_REGSZ_MASK;


                descMemRegs->Desc_Mem_Setup[j].Mem_Reg_Control = val;

                index += curMgr->descRegion[j].numDesc;

                dbgPrint("Descriptor region: %d, isOnChip: %d, numDesc: %d, szDesc: %d, MEM Base REG @0x%p=0x%08X, MEM Control REG @0x%p=0x%08X\n",
                         j, curMgr->descRegion[j].isOnChip, curMgr->descRegion[j].numDesc, curMgr->descRegion[j].szDesc,
                         IO_VIRT2PHY(&descMemRegs->Desc_Mem_Setup[j].Mem_Reg_Base), descMemRegs->Desc_Mem_Setup[j].Mem_Reg_Base,
                         IO_VIRT2PHY(&descMemRegs->Desc_Mem_Setup[j].Mem_Reg_Control), descMemRegs->Desc_Mem_Setup[j].Mem_Reg_Control);
            }
        }
    }
    /* Initialize the buffer manager functionality - Nothing to do right now */

#if (PUMA5_SOC_TYPE || PUMA6_SOC_TYPE)
    /*
     * Initialze teardown and DMA scheduler
     * Note: The code below breaks the orthogonality of the CPPI 4.1 API.. but nevermind as long as things work..
     */
    if (CPPI41_DOMAIN_PRIMARY_SR == palCppi4Obj->myDomain)
    {
        {
            Uint32          totalNumOfTearDownDesc = 0;
            Cppi4Queue*     tdFQueue;

            for (j = 0; j < PAL_CPPI41_NUM_DMA_BLOCK; j++)
            {
                Cppi4DMABlock* dmaBlock = &initCfg->dmaBlock[j];

                if (dmaBlock->globalCtrlBase)
                {
                    tdFQueue = &dmaBlock->tdFQueue;
                    totalNumOfTearDownDesc += PAL_CPPI41_NUM_TD_DESC;
                }
            }

            if (totalNumOfTearDownDesc)
            {
                /* Pre-allocate teardown descriptors - Two per channel (is it a ballpark? I just maintained
                * what was present in CPPI 4.1 implementation. Anyway, it seems to be an OK number.).
                */
                if ((tempBD = (Cppi4TeardownDesc*) PAL_cppi4AllocDesc(palCppi4Obj, tdFQueue->qMgr , totalNumOfTearDownDesc, sizeof(Cppi4TeardownDesc))) == NULL)
                {
                    errPrint("ERROR: Unable to allocate teardown desc\n");
                    goto init_failed;
                }
            }
        }
    }
#endif

    for (j = 0; j < PAL_CPPI41_NUM_DMA_BLOCK; j++)
    {
#if (PUMA5_SOC_TYPE || PUMA6_SOC_TYPE)
        PAL_Cppi4QueueHnd hnd;
#endif
        Cppi4DMABlock* dmaBlock = &initCfg->dmaBlock[j];
        Cppi4Queue* tdFQueue = &dmaBlock->tdFQueue;

        if (NULL == dmaBlock->globalCtrlBase)
        {
            continue;
        }

#if PUMA7_OR_NEWER_SOC_TYPE
        if (dmaBlock->schedCtrlBase)
        {
            if (CPPI41_DOMAIN_PP == palCppi4Obj->myDomain)
            {
                /* initialize the DMA scheduler parameters */
                CSL_Reg32 ctrlReg = DMA_SCHED_CTRL_ENABLE;

                ctrlReg |= ((dmaBlock->schedParams.noRecycleOnTx) ? (DMA_SCHED_CTRL_NO_RECYCLE_ON_TX) : 0);
                ctrlReg |= ((dmaBlock->schedParams.channelArbitrationMode) ? (DMA_SCHED_CTRL_CHANNEL_ARBITRATION_RR) : 0);
                ctrlReg |= ((dmaBlock->schedParams.rxTxArbitrationMode) ? (DMA_SCHED_CTRL_RX_TX_ARBITRATION_RR) : 0);
                ctrlReg |= ((dmaBlock->schedParams.portNum & DMA_SCHED_CTRL_PORT_NUMBER_MASK) << DMA_SCHED_CTRL_PORT_NUMBER_SHIFT);
                dmaBlock->schedCtrlBase->Control_Reg = ctrlReg;
                dbgPrint("Init DMA scheduler params: dmaNum: %d, noRecycleOnTx: %d, channelArbitrationMode: %d, rxTxArbitrationMode: %d, portNum: %d, Control REG @0x%p=0x%08X\n",
                         j, dmaBlock->schedParams.noRecycleOnTx, dmaBlock->schedParams.channelArbitrationMode, dmaBlock->schedParams.rxTxArbitrationMode,
                         dmaBlock->schedParams.portNum, IO_VIRT2PHY(&dmaBlock->schedCtrlBase->Control_Reg), ctrlReg);
            }
            else
            {
                dmaBlock->schedCtrlBase->Control_Reg |= (1 << DMA_SCHED_CTRL_ENABLE_SHIFT);
                dbgPrint("Enable DMA scheduler: dmaNum: %d, address: 0x%p, val: 0x%08X\n", j, IO_VIRT2PHY(&dmaBlock->schedCtrlBase->Control_Reg), (1 << DMA_SCHED_CTRL_ENABLE_SHIFT));
            }
        }

#endif

        /* Tell the hardware about the Teardown descriptors free queue manager and queue number */
        dmaBlock->globalCtrlBase->Teardown_FD_Queue_Control = (tdFQueue->qMgr << DMA_GLOBCTRL_TDFQ_QMGR_SHIFT) | (tdFQueue->qNum << DMA_GLOBCTRL_TDFQ_QNUM_SHIFT);

#if 0
        dbgPrint("Init DMA teardown: dmaNum: %d, tdFQueue->qMgr: %d, tdFQueue->qNum: %d, @0x%p=0x%08X\n",
                 j, tdFQueue->qMgr, tdFQueue->qNum,
                 IO_VIRT2PHY(&dmaBlock->globalCtrlBase->Teardown_FD_Queue_Control), dmaBlock->globalCtrlBase->Teardown_FD_Queue_Control);
#endif

#if (PUMA5_SOC_TYPE || PUMA6_SOC_TYPE) // In P7: 1. Teardown initializations are done at puma7_cppi.c, 2. no DMA scheduler configuration is needed
        if (CPPI41_DOMAIN_PRIMARY_SR == palCppi4Obj->myDomain)
        {
            /* Push all the teardown descriptors to the free teardown queue for the CPPI 4.1 system. */
            if((hnd = PAL_cppi4QueueOpen(palCppi4Obj, *tdFQueue)) == NULL)
            {
                errPrint("ERROR: Unable to open teardown free queue");
                goto init_failed;
            }

            /* Store the open queue handle for later use */
            palCppi4Obj->dmaBlock[j].tdFQueue = hnd;

            /* Push the just allocated BDs */
            for (i = 0; i < PAL_CPPI41_NUM_TD_DESC; i++, tempBD++)
            {
                tempBD->swDmaNumber = j;
                PAL_cppi4QueuePush(hnd, (Ptr) CPPI4_VIRT_TO_PHYS(tempBD), (sizeof (Cppi4TeardownDesc) - 24)/4, 0);
            }
        }
        /* initialize the DMA scheduler */
        dmaBlock->schedCtrlBase->Control_Reg = ((dmaBlock->schedTable.numEntries - 1) << DMA_SCHED_CTRL_LAST_ENTRY_SHIFT);
        for (i = 0; i < 16; i++)
        {
            dmaBlock->schedTableBase->Sched_Table_Word[i] =dmaBlock->schedTable.entry[(4 * i) ] |
                            (dmaBlock->schedTable.entry[(4 * i) +1] << 8)  |
                            (dmaBlock->schedTable.entry[(4 * i) +2] << 16) |
                            (dmaBlock->schedTable.entry[(4 * i) +3] << 24);
        }
        dmaBlock->schedCtrlBase->Control_Reg |= (1 << DMA_SCHED_CTRL_ENABLE_SHIFT);

        dbgPrint("DMA sched @address %x, value %x\n", dmaBlock->schedCtrlBase->Control_Reg, dmaBlock->schedCtrlBase->Control_Reg);
        dbgPrint("DMA sched @address %p, value %x\n", &initCfg->dmaBlock[j].schedTableBase->Sched_Table_Word[0], initCfg->dmaBlock[j].schedTable.entry[0]);

        dbgPrint("DMA sched initialized\n");
#endif
    }

    if (initCfg->debugToolBind)
    {
        if (NULL == cppiDebugDir)
        {
            if (NULL == (cppiDebugDir = proc_mkdir("cppi",     NULL)))
            {
                errPrint("ERROR: Could not create cppi proc directory\n");
            }
        }
        initCfg->debugToolBind((PAL_Handle) palCppi4Obj, (Ptr)cppiDebugDir);
    }

    return ((PAL_Handle) palCppi4Obj);

init_failed:
    --Cppi4PALNumInst[(CPPI41_DOMAIN)param];
    return NULL;
}

/*
 * PAL_cppi4Exit
 *  - delete the instance created via PAL_cppi4Init, if no other callers are
 *    holding a handler to the PAL instance (implemented via Cppi4PALNumInst
 *    counter)
 *  - if deleting the last instance, it also puts the NWSS in reset.
 */
PAL_Result PAL_cppi4Exit (PAL_Handle hnd, Ptr param)
{
    Cppi4PALObj *palCppi4Obj = (Cppi4PALObj *) hnd;
    Cppi4InitCfg* initCfg = palCppi4Obj->initCfg;
    Uint32 i;

    /* Check to see if an instance exists in order to exit */
    if (--Cppi4PALNumInst[palCppi4Obj->myDomain] > 0)
    {
        return (PAL_SOK);
    } else if (Cppi4PALNumInst[palCppi4Obj->myDomain] == 0)
    {
        PAL_sysResetCtrl (initCfg->resetLine, IN_RESET);

        for(i = 0; i < PAL_CPPI41_NUM_QUEUE_MGR; i++)
        {
            /* Need to free up the descriptor region only once. Descriptor region 0 for each
             * queue manager contains the base of the memory allocated for all the descriptor
             * regions.
             */
            PAL_osMemFreeSizeAligned(palCppi4Obj->myDomain, palCppi4Obj->qMgrDescRegPtr[i], palCppi4Obj->qMgrDescRegSize[i]);
        }

        /* close the teardown queues */
        for (i = 0; i < PAL_CPPI41_NUM_DMA_BLOCK; i++)
        {
            PAL_cppi4QueueClose(hnd,palCppi4Obj->dmaBlock[i].tdFQueue);
        }

        return (PAL_SOK);
    }

    return (CPPI4_ERR_DEV_NOT_INSTANTIATED);
}

#if PUMA7_OR_NEWER_SOC_TYPE
Bool PAL_cppi4IsBufPoolInitialized(PAL_Handle hnd, Cppi4BufPool pool)
{
    Cppi4PALObj *palCppi4Obj = (Cppi4PALObj *)hnd;

    if(palCppi4Obj->bufPoolPtr[pool.bMgr][pool.bPool] == NULL)
    {
        return False;
    }

    return (palCppi4Obj->initCfg->bufMgrBase[pool.bMgr]->Base_Config[pool.bPool].Buffer_Pool_Config & BUFMGR_POOL_ENABLE_MASK);
}

PAL_Result PAL_cppi4GetBufferPoolInfo(PAL_Handle hnd, Cppi4BufPool pool, Cppi4BufPoolInfo* bPoolInfo)
{
    Cppi4PALObj *palCppi4Obj = (Cppi4PALObj *)hnd;

    if(NULL != PAL_cppi4IsBufPoolInitialized(hnd, pool))
    {
        bPoolInfo->bPoolPhyAddr       = palCppi4Obj->bufPoolPtr[pool.bMgr][pool.bPool];
        if (virt_addr_valid(bPoolInfo->bPoolPhyAddr))
        {
            bPoolInfo->bPoolPhyAddr   = CPPI4_VIRT_TO_PHYS(bPoolInfo->bPoolPhyAddr);
        }
        bPoolInfo->bPoolSize      = palCppi4Obj->bufPoolSize[pool.bMgr][pool.bPool];
        bPoolInfo->bufSize        = palCppi4Obj->bufSize[pool.bMgr][pool.bPool];
        bPoolInfo->numBuf         = palCppi4Obj->numBuf[pool.bMgr][pool.bPool];
        bPoolInfo->bPoolTopPhyAddr   = bPoolInfo->bPoolPhyAddr + bPoolInfo->bPoolSize;
        bPoolInfo->bPoolOffset    = sizeof(CSL_Buffer_Mgr_Base_Config_Regs) * pool.bPool;
    }
    else 
    {
        return PAL_MINOR_ERROR;
    }

    return PAL_SOK;
}
EXPORT_SYMBOL(PAL_cppi4GetBufferPoolInfo);
#endif /* PUMA7_OR_NEWER_SOC_TYPE */

/*
 * PAL_bufPoolInit
 *  - Create a buffer pool for the given index
 */
//Ptr PAL_cppi4BufPoolInit (PAL_Handle hnd, Cppi4BufPool pool, Bool refCntEnable, Uint32 bufSize, Uint32 numBuf)
Ptr PAL_cppi4BufPoolDirectInit (PAL_Handle hnd, Cppi4BufPool pool, Bool refCntEnable, Uint32 bufSize, Uint32 numBuf, Ptr poolBase)
{
    Cppi4PALObj *palCppi4Obj = (Cppi4PALObj *) hnd;
    Cppi4InitCfg* initCfg = palCppi4Obj->initCfg;
    CSL_BufMgr_RegsOvly regs = initCfg->bufMgrBase[pool.bMgr];
    Uint32 size, bufSzOrder, numBufOrder;
    Ptr  buf = NULL;

    if (NULL == regs)
    {
        errPrint("ERROR\n");
        return NULL;
    }

#if PUMA7_OR_NEWER_SOC_TYPE
    if (PAL_cppi4IsBufPoolInitialized(hnd, pool) == True)
    {
        return palCppi4Obj->bufPoolPtr[pool.bMgr][pool.bPool];
    }
#endif

    for(bufSzOrder = 0; (1 << bufSzOrder) < bufSize; bufSzOrder++);
    for(numBufOrder = 0; (1 << numBufOrder) < numBuf; numBufOrder++);

    /* Basic size contrains from Buf manager doc */
    if(bufSzOrder < 6) return NULL;
    if(numBufOrder < 5) return NULL;

    bufSize = (1 << bufSzOrder);
    numBuf = (1 << numBufOrder);

    size = bufSize * numBuf;

    if (NULL != poolBase)
    {
        if (((Uint32)poolBase % size) != 0)
        {
            errPrint("\nERROR: Cannot configure pool %d , pool base address 0x%p is not aligned to pool size %d\n", pool.bPool, poolBase, size);
            return NULL;
        }
        buf = poolBase;
	}
    else
    {
        /* Buffers are allocated by the ATOM and address is returned by MRPC */
#if (!defined(CONFIG_INTEL_KERNEL_PP_DRIVER_LOCAL) && defined(CONFIG_MRPC_CPPI_CLIENT))
		buf = avalanche_cppi_alloc_pp_buffer_pool(size);
#else
        buf = PAL_osMemAllocSizeAligned(palCppi4Obj->myDomain, size);
#endif
    }

    /* Hardware requires the base to be naturally aligned to size. */
    if(buf == NULL) 
    {
        return buf;
    }

    /* init buffer pool memory in case the virtual or phyical address in known */
    if (virt_addr_valid(buf))
		PAL_osMemSet(buf, 0, size);
    else if (IO_PHY_VALID((u32) buf))
        PAL_osMemSet(IO_PHY2VIRT((u32)buf), 0, size);

    palCppi4Obj->bufPoolPtr[pool.bMgr][pool.bPool] = buf;
    palCppi4Obj->bufPoolSize[pool.bMgr][pool.bPool] = size;
    palCppi4Obj->bufSize[pool.bMgr][pool.bPool] = bufSize;
    palCppi4Obj->numBuf[pool.bMgr][pool.bPool] = numBuf;

    /* Allocate the buffer pool */
    regs->Base_Config[pool.bPool].Buffer_Pool_Base = (Uint32)((virt_addr_valid(buf)) ? CPPI4_VIRT_TO_PHYS(buf) : (CSL_Reg32) (buf));

    /* Enable the pool */
    regs->Base_Config[pool.bPool].Buffer_Pool_Config |= BUFMGR_POOL_ENABLE_MASK;

    /* Program the buffer size */
    regs->Base_Config[pool.bPool].Buffer_Pool_Config &= ~BUFMGR_POOL_BUFFER_SIZE_MASK;
    regs->Base_Config[pool.bPool].Buffer_Pool_Config |= (bufSzOrder - 6) << BUFMGR_POOL_BUFFER_SIZE_SHIFT;

    /* Program the pool size */
    regs->Base_Config[pool.bPool].Buffer_Pool_Config &= ~BUFMGR_POOL_POOL_SIZE_MASK;
    regs->Base_Config[pool.bPool].Buffer_Pool_Config |= (numBufOrder - 5) << BUFMGR_POOL_POOL_SIZE_SHIFT;

    /* enable reference counting on the pool */
    if(refCntEnable)
    {
        regs->Base_Config[pool.bPool].Buffer_Pool_Config |= BUFMGR_POOL_REFCNT_ENABLE_MASK;
    }
    else
    {
        regs->Base_Config[pool.bPool].Buffer_Pool_Config &= ~BUFMGR_POOL_REFCNT_ENABLE_MASK;
    }

    dbgPrint("Buffer Pool: %d, numBuf: %d, numBufOrder: %d, bufSize: %d, bufSzOrder: %d, pool base address: 0x%p, Base REG @0x%p=0x%08X, Config REG @0x%p=0x%08X\n",
          pool.bPool, numBuf, numBufOrder, bufSize, bufSzOrder, regs->Base_Config[pool.bPool].Buffer_Pool_Base,
          IO_VIRT2PHY(&regs->Base_Config[pool.bPool].Buffer_Pool_Base), regs->Base_Config[pool.bPool].Buffer_Pool_Base,
          IO_VIRT2PHY(&regs->Base_Config[pool.bPool].Buffer_Pool_Config), regs->Base_Config[pool.bPool].Buffer_Pool_Config);

    return buf;
}

//Ptr PAL_cppi4BufPoolDirectInit (PAL_Handle hnd, Cppi4BufPool pool, Bool refCntEnable, Uint32 bufSize, Uint32 numBuf, Ptr poolBase)
Ptr PAL_cppi4BufPoolInit (PAL_Handle hnd, Cppi4BufPool pool, Bool refCntEnable, Uint32 bufSize, Uint32 numBuf)
{
    return PAL_cppi4BufPoolDirectInit(hnd,pool,refCntEnable,bufSize,numBuf,NULL);
}


/*
 * PAL_cppi4BufPoolSetBuffers
 *  - Init buffer memory for each buffer
 */
PAL_Result PAL_cppi4BufPoolSetBuffers (PAL_Handle hnd, Cppi4BufPool dstPool, Ptr srcBuf, Uint32 srcBufLen)
{
    Cppi4PALObj *palCppi4Obj = (Cppi4PALObj *) hnd;
    Ptr dstBuf;
    Uint32 i;

    for(i = 0; i < palCppi4Obj->numBuf[dstPool.bMgr][dstPool.bPool]; ++i)
    {
        dstBuf = (Ptr)((Uint32)(palCppi4Obj->bufPoolPtr[dstPool.bMgr][dstPool.bPool]) + (palCppi4Obj->bufSize[dstPool.bMgr][dstPool.bPool])*i);
        memcpy(dstBuf, srcBuf, srcBufLen);
        PAL_CPPI4_CACHE_WRITEBACK(dstBuf, (32+srcBufLen)/32*32);
    }

    return PAL_SOK;
}

/*
 * PAL_cppi4BufIncRefCnt
 *  - Increment the reference count of the valid buffer
 */
PAL_Result PAL_cppi4BufIncRefCnt (PAL_Handle hnd, Cppi4BufPool pool, Ptr bufPtr)
{
    Cppi4PALObj *palCppi4Obj = (Cppi4PALObj *) hnd;
    Cppi4InitCfg* initCfg = palCppi4Obj->initCfg;
    CSL_BufMgr_RegsOvly regs = initCfg->bufMgrBase[pool.bMgr];

    regs->Ref_Cnt_Inc_Val = (Uint32)(((pool.bPool) << 16) | 1);
    regs->Ref_Cnt_Inc_Ptr = (Uint32)bufPtr;

    return PAL_SOK;
}

/*
 * PAL_cppi4BufDecRefCnt
 *  - Decrement the reference count of the valid buffer
 */
PAL_Result PAL_cppi4BufDecRefCnt (PAL_Handle hnd, Cppi4BufPool pool, Ptr bufPtr)
{
    Cppi4PALObj *palCppi4Obj = (Cppi4PALObj *) hnd;
    Cppi4InitCfg* initCfg = palCppi4Obj->initCfg;
    CSL_BufMgr_RegsOvly regs = initCfg->bufMgrBase[pool.bMgr];

    regs->Pointer_Size[pool.bPool].Buffer_Pool_Pointer = (Uint32)bufPtr;

    return PAL_SOK;
}

/*
 * PAL_cppi4BufPopBuf
 *  - Decrement the reference count of the valid buffer
 */
Ptr PAL_cppi4BufPopBuf (PAL_Handle hnd, Cppi4BufPool pool)
{
    Cppi4PALObj *palCppi4Obj = (Cppi4PALObj *) hnd;
    Cppi4InitCfg* initCfg = palCppi4Obj->initCfg;
    CSL_BufMgr_RegsOvly regs;
    if ((pool.bPool >= CSL_CPPI41_BUFMGR_NUM_BUF_POOLS) || (pool.bMgr >= PAL_CPPI41_NUM_BUF_MGR))
    {
        return NULL;
    }
    regs = initCfg->bufMgrBase[pool.bMgr];

    return (Ptr)(regs->Pointer_Size[pool.bPool].Buffer_Pool_Pointer);
}

/**
 * Retrieve all cppi buffers pools information from cppi driver
 * on ARM
 *
 * @param pools array to fill in the pools info
 *
 * @return int 0 on success, non-zero value otherwise
 */
PAL_Result PAL_cppi4GetAllBufferPoolsInfo(PAL_Handle hnd, Cppi4BufPoolInfo *pools)
{
    Cppi4BufPool pool;
    u32 i;

    if (NULL == pools) {
        errPrint("pools is null");
        return PAL_ERROR_FLAG;
    }

    /* Get pools info */
    for (i=0; i<PAL_CPPI41_BMGR_MAX_POOLS; i++) {
        pool.bPool = i;
        if(PAL_SOK == PAL_cppi4GetBufferPoolInfo(hnd, pool, &pools[i]))   {
            dbgPrint("buffer pool %u Info:"      , i                       );
            dbgPrint("\tbPoolPhyAddr    = 0x%08x", pools[i].bPoolPhyAddr   );
            dbgPrint("\tbPoolSize       = 0x%08x", pools[i].bPoolSize      );
            dbgPrint("\tbufSize         = 0x%08x", pools[i].bufSize        );
            dbgPrint("\tnumBuf          = 0x%08x", pools[i].numBuf         );
            dbgPrint("\tbPoolTopPhyAddr = 0x%08x", pools[i].bPoolTopPhyAddr);
            dbgPrint("\tbPoolOffset     = 0x%08x", pools[i].bPoolOffset    );
        }
        else {
            dbgPrint("buffer pool %u is not initialized", i);
        }
    }
    return (PAL_SOK);
}
EXPORT_SYMBOL(PAL_cppi4GetAllBufferPoolsInfo);

/*
 * PAL_bufPoolInit
 *  - Create a buffer pool for the given index
 */
PAL_Result PAL_cppi4BufPoolDestroy (PAL_Handle hnd, Cppi4BufPool pool)
{
    Cppi4PALObj *palCppi4Obj = (Cppi4PALObj *) hnd;
    Cppi4InitCfg* initCfg = palCppi4Obj->initCfg;
    CSL_BufMgr_RegsOvly regs = initCfg->bufMgrBase[pool.bMgr];

    if(palCppi4Obj->bufPoolPtr[pool.bMgr][pool.bPool] == NULL) {
        return PAL_ERROR_FLAG;
    }

    dbgPrint("freeing memory: %p\n", palCppi4Obj->bufPoolPtr[pool.bMgr][pool.bPool]);

    PAL_osMemFreeSizeAligned(palCppi4Obj->myDomain, palCppi4Obj->bufPoolPtr[pool.bMgr][pool.bPool], palCppi4Obj->bufPoolSize[pool.bMgr][pool.bPool]);

    palCppi4Obj->bufPoolPtr[pool.bMgr][pool.bPool] = NULL;

    /* Disable the pool */
    regs->Base_Config[pool.bPool].Buffer_Pool_Config &= ~BUFMGR_POOL_ENABLE_MASK;
    regs->Base_Config[pool.bPool].Buffer_Pool_Base = 0;

    return PAL_SOK;
}

#if PUMA7_OR_NEWER_SOC_TYPE

Bool PAL_cppi4IsDescRegionAllocated(PAL_Handle hnd, Uint32 qMgr, Uint32 descRegion)
{
    return ((Cppi4PALObj *)hnd)->initCfg->queueMgrInfo[qMgr].descRegion[descRegion].isAllocated;
}

Ptr PAL_cppi4AllocDescRegion(PAL_Handle hnd, Uint32 qMgr, Uint32 descRegion)
{
    Cppi4DescReg *descRegionP = &((Cppi4PALObj *)hnd)->initCfg->queueMgrInfo[qMgr].descRegion[descRegion];
    Ptr buf = NULL;
    Uint32 cookie;

    PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &cookie);

    if (descRegionP->isAllocated == False)
    {
        buf = descRegionP->base;
        descRegionP->isAllocated = True;
    }

    PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);

    return buf;
}

PAL_Result PAL_cppi4GetDescRegionMemInfo(PAL_Handle hnd, PAL_CPPI_PP_DESC_REGIONs_e descRegion, Uint32 qMgr, Ptr *addr, Uint32 *length)
{
    Cppi4DescReg *descRegionP = &((Cppi4PALObj *)hnd)->initCfg->queueMgrInfo[qMgr].descRegion[descRegion];
    *addr = NULL;
    *length = 0;

    if (descRegionP->isAllocated == True)
    {
        *addr = descRegionP->base;
        *length = descRegionP->numDesc * descRegionP->szDesc;
    }

    return PAL_SOK;
}

PAL_Result PAL_cppi4GetOffChipAddr(PAL_Handle hnd, Uint32 *physAddr, Uint32 *length)
{
    *physAddr   = (Uint32)((Cppi4PALObj *)hnd)->qMgrDescRegPtr[PAL_CPPI_PP_QMGR_G0];
    *length     = ((Cppi4PALObj *)hnd)->qMgrDescRegSize[PAL_CPPI_PP_QMGR_G0];

    return PAL_SOK;
}

PAL_Result PAL_cppi4GetOffChipBpoolAddr(PAL_Handle hnd, Uint32 pool, Uint32 *physAddr, Uint32 *length)
{
    *physAddr   = (Uint32)((Cppi4PALObj *)hnd)->bufPoolPtr[PAL_CPPI_PP_BUF_MGR][pool];
    *length     = ((Cppi4PALObj *)hnd)->bufPoolSize[PAL_CPPI_PP_BUF_MGR][pool];

    return PAL_SOK;
}

#else
Ptr PAL_cppi4AllocDesc (PAL_Handle hnd, Uint32 qMgr, Uint32 numDesc, Uint32 szDesc)
{
    Ptr buf = 0;
    Cppi4PALObj* palCppi4Obj = (Cppi4PALObj *) hnd;
    Cppi4InitCfg* initCfg = palCppi4Obj->initCfg;
    Cppi4QueueMgrCfg* curMgr = &(initCfg->queueMgrInfo[qMgr]);
    Cppi4DescReg *descRegion = &curMgr->descRegion[0];
    Uint32 i = 0, cookie;

    dbgPrint("requested: numDesc = %d, szDesc = %d\n", numDesc, szDesc);

    PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &cookie);

    /* Traverse the desc region array in reverse order to find the best fit */
    for (i = 0; i < PAL_CPPI41_MAX_DESC_REGIONS; i++)
    {
        if((descRegion[i].isAllocated == False) && (descRegion[i].isOnChip == False)) {
            dbgPrint("Desc region[%d], sz: %d, num: %d\n", i, descRegion[i].szDesc, descRegion[i].numDesc);
            if((descRegion[i].szDesc == szDesc) && (descRegion[i].numDesc == numDesc)) {
                dbgPrint("Got a buffer desc region of base: %p at address %p\n", curMgr->descRegion[i].base, &(curMgr->descRegion[i].base));
                buf = descRegion[i].base;
                descRegion[i].isAllocated = True;
                break;
            }
        }
    }

    PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);

    return buf;
}
#endif

/*
 * PAL_cppi4DeallocDesc
 *  - DeAlloctes descripor array.
 */
PAL_Result PAL_cppi4DeallocDesc (PAL_Handle hnd, Uint32 qMgr, Ptr base)
{
    Cppi4PALObj* palCppi4Obj = (Cppi4PALObj *) hnd;
    Cppi4InitCfg* initCfg = palCppi4Obj->initCfg;
    Cppi4QueueMgrCfg* curMgr = &(initCfg->queueMgrInfo[qMgr]);
    Cppi4DescReg *descRegion = &curMgr->descRegion[0];
    Uint32 i = 0, cookie;
    PAL_Result retVal = PAL_ERROR_FLAG;

    dbgPrint("De-alloc desc called.\n");

    PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &cookie);

    /* Traverse the desc region array in reverse order to find the best fit */
    for (i = 0; i < PAL_CPPI41_MAX_DESC_REGIONS; i++)
    {
        dbgPrint("Desc region[%d], sz: %d, num: %d\n", i, descRegion[i].szDesc, descRegion[i].numDesc);
        if((descRegion[i].isAllocated == True) && (descRegion[i].isOnChip == False)) {
            if(descRegion[i].base == base) {
                dbgPrint("desc region %d, base %p deallocated.\n", i, base);
                descRegion[i].isAllocated = False;
                retVal = PAL_SOK;
                break;
            }
        }
    }

    PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);

    return retVal;
}

/*
 * PAL_cppi4Control
 * Defines various CPPI 4.1 IOCTLs. Useful for debugging, monitoring etc.
 */
PAL_Result PAL_cppi4Control (PAL_Handle hnd, Uint32 cmd, Ptr cmdArg, Ptr param)
{
    Cppi4PALObj *palCppi4Obj = (Cppi4PALObj *) hnd;
    Cppi4InitCfg * initCfg = palCppi4Obj->initCfg;

    switch (cmd)
    {
        /*
         * TODO: add version query support !@@
         */
#ifdef PAL_CPPI_VER_SUPPORT
        case PAL_CPPI41_IOCTL_GET_SWVER:
        /* cmdArg is an ptr to an integer that will contain the integer version id
         and param is a double ptr to a string which will point to the static
         version string */
        *((Uint32 *) cmdArg) = (Uint32) palCppi4Obj->versionId;
        *((Char **) param) = (Char *) & Cppi4PALVersionString[0];
        break;

        case PAL_CPPI41_IOCTL_GET_HWVER:
        /* cmdArg is a ptr to an integer that will be written with the rev */
        *((Uint32 *) cmdArg) = palCppi4Obj->dmaRegs->Revision;
        /* param is a ptr to an integer that will be written with the rev */
        *((Uint32 *) param) = palCppi4Obj->qmRegs->Revision;
        break;
#endif /* PAL_CPPI_VER_SUPPORT */

#if (PUMA5_SOC_TYPE || PUMA6_SOC_TYPE) // This feature is not really used
        case PAL_CPPI41_IOCTL_GET_FDQ_STARVE_CNT:
        {
            Cppi4Queue* queue = (Cppi4Queue*) cmdArg;
            Uint32 qNum = queue->qNum;
            CSL_Queue_Manager_Region_RegsOvly regs = initCfg->queueMgrInfo[queue->qMgr].queueMgrRgnBase;
            qNum -= initCfg->queueMgrInfo[queue->qMgr].basefdQNum;
            *(Uint32*)param = regs->Free_Desc_Starvation[qNum/4];
        }
        break;
#endif

        case PAL_CPPI41_IOCTL_GET_FDBQ_STARVE_CNT:
        {
            Cppi4Queue* queue = (Cppi4Queue*) cmdArg;
            Uint32 qNum = queue->qNum;
            CSL_Queue_Manager_Region_RegsOvly regs = initCfg->queueMgrInfo[queue->qMgr].queueMgrRgnBase;
            qNum -= initCfg->queueMgrInfo[queue->qMgr].basefdbQNum;
            *(Uint32*)param = regs->Free_Desc_Buf_Starvation[qNum/4];
        }
        break;
        case PAL_CPPI41_IOCTL_GET_QUEUE_PEND_STATUS:
        {
            Cppi4Queue* queue = (Cppi4Queue*) cmdArg;
            CSL_Queue_Manager_Region_RegsOvly regs = initCfg->queueMgrInfo[queue->qMgr].queueMgrRgnBase;
            *(Uint32*)param = (regs->Queue_Pending[queue->qNum/32] >> (queue->qNum % 32)) & 0x1;
        }
        break;
        case PAL_CPPI41_IOCTL_GET_QUEUE_ENTRY_COUNT:
        {
            Cppi4Queue* queue = (Cppi4Queue*) cmdArg;
            CSL_Queue_Status_Regs* regs = &initCfg->queueMgrInfo[queue->qMgr].queueStatusRgnBase->Queue_Status[queue->qNum];
            *(Uint32*)param = regs->Queue_Status_Reg_A;
        }
        break;
        case PAL_CPPI41_IOCTL_GET_QUEUE_BYTE_COUNT:
        {
            Cppi4Queue* queue = (Cppi4Queue*) cmdArg;
            CSL_Queue_Status_Regs* regs = &initCfg->queueMgrInfo[queue->qMgr].queueStatusRgnBase->Queue_Status[queue->qNum];
            *(Uint32*)param = regs->Queue_Status_Reg_B;
        }
        break;
        case PAL_CPPI41_IOCTL_GET_QUEUE_HEAD_PKT_SIZE:
        {
            Cppi4Queue* queue = (Cppi4Queue*) cmdArg;
            CSL_Queue_Status_Regs* regs = &initCfg->queueMgrInfo[queue->qMgr].queueStatusRgnBase->Queue_Status[queue->qNum];
            *(Uint32*)param = regs->Queue_Status_Reg_C;
        }
        break;
        case PAL_CPPI41_IOCTL_QUEUE_DIVERT:
        {
            CSL_Queue_Manager_Region_RegsOvly regs = initCfg->queueMgrInfo[*(Uint32 *)param].queueMgrRgnBase;
            regs->Queue_Diversion = (Uint32)cmdArg;
        }
        break;
        case PAL_CPPI41_IOCTL_BUFMGR_SOFT_RESET:
        {
            CSL_BufMgr_RegsOvly regs = initCfg->bufMgrBase[(Uint32)cmdArg];
            regs->Soft_Reset = 1;
        }
        break;
        case PAL_CPPI41_IOCTL_BUF_REFCNT_INCR:
        {
            Uint32 bMgr = ((Uint32) cmdArg >> 8) & 0xF;
            CSL_BufMgr_RegsOvly regs = initCfg->bufMgrBase[bMgr];
            regs->Ref_Cnt_Inc_Val = (Uint32)cmdArg;
            regs->Ref_Cnt_Inc_Ptr = (Uint32)param;
        }
        break;
        default:
        errPrint ("\nWARN: PAL_cppi4Control: Unhandled ioctl code %d", cmd);
        break;
    }
    return (PAL_SOK);
}

#if PUMA7_OR_NEWER_SOC_TYPE
#define SIZE_IN_WORD(p) ((sizeof(p) + 0x3) >> 2)

static Int32 (*__pdsp_cmd_send)(pdsp_id_t, pdsp_cmd_t, void *, Uint32, void *, Uint32) = NULL;

PAL_Result PAL_cppi4PdspCmdSendUnregister(void)
{
    BUG_ON(!__pdsp_cmd_send);

    __pdsp_cmd_send = NULL;

    printk("%s:%d: pdsp_cmd_send unregister done.", __func__, __LINE__);
    return (PAL_SOK);
}
EXPORT_SYMBOL(PAL_cppi4PdspCmdSendUnregister);

PAL_Result PAL_cppi4PdspCmdSendRegister(Int32 (*cb)(pdsp_id_t, pdsp_cmd_t, void *, Uint32, void *, Uint32))
{
    BUG_ON(__pdsp_cmd_send);

    __pdsp_cmd_send = cb;

    printk("%s:%d: pdsp_cmd_send register done.", __func__, __LINE__);
    return (PAL_SOK);
}
EXPORT_SYMBOL(PAL_cppi4PdspCmdSendRegister);

#define PDSP_PREP_CMD(cmd, option, index)           \
                        (((cmd) & 0xffu) << 0) |    \
                        (((option) & 0xffu) << 8) | \
                        (((index) & 0xffffu) << 16)

static AVALANCHE_PP_RET_e __cppi4AccChClose(AVALANCHE_PP_ACC_CH_INFO_t *ptr_ch_cfg)
{
    AVALANCHE_PP_RET_e rc;

    BUG_ON(!__pdsp_cmd_send);

    rc = __pdsp_cmd_send(PDSP_ID_Accumulator,
                         PDSP_PREP_CMD(PDSP_ACCUMULATOR_DISABLE_CH, ptr_ch_cfg->Channel, 0),
                         NULL, 0, NULL, 0);
    if (rc)
    {
        printk("%s:%d ERROR !!! Failed to close accumulator channel !!!\n",__FUNCTION__,__LINE__);
        return (rc + PP_RC_FAILURE);
    }

	return (PP_RC_SUCCESS);
}

static AVALANCHE_PP_RET_e __cppi4AccChOpen(AVALANCHE_PP_ACC_CH_INFO_t *ptr_ch_cfg)
{
    AVALANCHE_PP_RET_e rc;

    BUG_ON(!__pdsp_cmd_send);

    rc = __pdsp_cmd_send(PDSP_ID_Accumulator,
                         PDSP_PREP_CMD(PDSP_ACCUMULATOR_ENABLE_CH, ptr_ch_cfg->Channel, 0),
                         &(ptr_ch_cfg->Param0Ret),
                         SIZE_IN_WORD(AVALANCHE_PP_ACC_CH_INFO_t) - SIZE_IN_WORD(Int32) /* Size of the parameters = total size - command size*/,
                         NULL,   0);

    if (rc)
    {
        printk("%s:%d ERROR !!! Failed to open accumulator channel !!!\n",__FUNCTION__,__LINE__);
        __cppi4AccChClose(ptr_ch_cfg);
        return (rc + PP_RC_FAILURE);
    }

	return (PP_RC_SUCCESS);
}

static AVALANCHE_PP_RET_e __cppi4AccChEnRateLimit(AVALANCHE_PP_ACC_CH_INFO_t *ptr_ch_cfg)
{
    AVALANCHE_PP_RET_e rc;

    BUG_ON(!__pdsp_cmd_send);

    rc = __pdsp_cmd_send(PDSP_ID_Accumulator,
                         PDSP_PREP_CMD(PDSP_ACCUMULATOR_ENABLE_CH_RATE_LIMIT, ptr_ch_cfg->Channel, ptr_ch_cfg->Index),
                         &(ptr_ch_cfg->Param0Ret),
                         SIZE_IN_WORD(AVALANCHE_PP_ACC_CH_INFO_t) - SIZE_IN_WORD(Int32) /* Size of the parameters = total size - command size*/,
                         NULL,   0);

    if (rc)
    {
        return (rc + PP_RC_FAILURE);
    }

    return (PP_RC_SUCCESS);
}

PAL_Cppi4AccChHnd PAL_cppi4AccChOpenAppCpu(PAL_Handle hnd, Cppi4AccumulatorCfg* accCfg)
{
    Uint32 cookie;
    PAL_Cppi4AccChObj *accChObj;
    AVALANCHE_PP_ACC_CH_INFO_t ptr_ch_cfg;
    AVALANCHE_PP_RET_e rc;

    if (PAL_osMemAlloc(0, sizeof(PAL_Cppi4AccChObj), 0, (Ptr *) &accChObj) != PAL_SOK) {
        errPrint ("\nERROR:PAL: PAL_cppi4AccChOpen: Failed to allocate Acc channel object structure.");
        return NULL;
    }

    PAL_osMemSet (accChObj, 0, sizeof (PAL_Cppi4AccChObj));

    PAL_osMemCopy(&accChObj->initCfg, accCfg, sizeof(Cppi4AccumulatorCfg));

    accChObj->palCppi4Obj = hnd;

    /* Need to protect the accumulator register writes. They are shared with pre-fetcher */
    PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &cookie);

    ptr_ch_cfg.Channel      =   accCfg->accChanNum ;
    ptr_ch_cfg.Command      =   0 ;
    ptr_ch_cfg.Param0Ret    =   (accCfg->list.listBase) ;
    ptr_ch_cfg.Param1       =   (accCfg->queue.qNum) | (accCfg->queue.qMgr << 12) | (accCfg->list.maxPageEntry << 16);
    ptr_ch_cfg.Param2       =   (accCfg->pacingTickCnt)             |
                                (accCfg->list.maxPageCnt    << 16)  | (accCfg->list.listEntrySize  << 18)|
                                (accCfg->list.listCountMode << 20)  | (accCfg->list.stallAvoidance << 21)|
                                (accCfg->list.pacingMode    << 22)  | (accCfg->mode << 31);

    rc = __cppi4AccChOpen(&ptr_ch_cfg);
    if (PP_RC_SUCCESS != rc)
    {
        errPrint("Error: Accumulator PDSP is not responding, return code: %u\n", rc);
        PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);

        PAL_osMemFree(0, accChObj, sizeof(PAL_Cppi4AccChObj));
        return NULL;
    }

    accChObj->curPage = 0;

    PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);

    return (PAL_Cppi4AccChHnd) accChObj;
}
#endif

#ifdef CONFIG_INTEL_KERNEL_PP_DRIVER_LOCAL
PAL_Cppi4AccChHnd PAL_cppi4AccChOpen(PAL_Handle hnd, Cppi4AccumulatorCfg* accCfg)
{
    Uint32 cookie;
    PAL_Cppi4AccChObj *accChObj;
#if PUMA7_OR_NEWER_SOC_TYPE
    AVALANCHE_PP_ACC_CH_INFO_t ptr_ch_cfg;
    AVALANCHE_PP_RET_e rc;
#else
    Uint32 i;
    Cppi4PALObj *palCppi4Obj = (Cppi4PALObj *) hnd;
    Cppi4InitCfg * initCfg = palCppi4Obj->initCfg;
    APDSP_Command_Status_RegsOvly cmdRegs = initCfg->apdspInfo.pdspCmdBase;
#endif

    if (PAL_osMemAlloc(0, sizeof(PAL_Cppi4AccChObj), 0, (Ptr *) &accChObj) != PAL_SOK) {
        errPrint ("\nERROR:PAL: PAL_cppi4AccChOpen: Failed to allocate Acc channel object structure.");
        return NULL;
    }

    PAL_osMemSet (accChObj, 0, sizeof (PAL_Cppi4AccChObj));

    PAL_osMemCopy(&accChObj->initCfg, accCfg, sizeof(Cppi4AccumulatorCfg));

    accChObj->palCppi4Obj = hnd;

    /* Need to protect the accumulator register writes. They are shared with pre-fetcher */
    PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &cookie);

#if PUMA7_OR_NEWER_SOC_TYPE
    ptr_ch_cfg.Channel      =   accCfg->accChanNum ;
    ptr_ch_cfg.Command      =   0 ;
    ptr_ch_cfg.Param0Ret    =   PAL_CPPI4_VIRT_2_PHYS(accCfg->list.listBase) ;
    ptr_ch_cfg.Param1       =   (accCfg->queue.qNum) | (accCfg->queue.qMgr << 12) | (accCfg->list.maxPageEntry << 16);
    ptr_ch_cfg.Param2       =   (accCfg->pacingTickCnt)             |
                                (accCfg->list.maxPageCnt    << 16)  | (accCfg->list.listEntrySize  << 18)|
                                (accCfg->list.listCountMode << 20)  | (accCfg->list.stallAvoidance << 21)|
                                (accCfg->list.pacingMode    << 22   | (accCfg->mode << 31));

    rc = __cppi4AccChOpen(&ptr_ch_cfg);
    if (PP_RC_SUCCESS != rc)
    {
        errPrint("Error: Accumulator PDSP is not responding, return code: %u\n", rc);
        PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);

        PAL_osMemFree(0, accChObj, sizeof(PAL_Cppi4AccChObj));
        return NULL;
    }
#else

    if(accCfg->mode) {
        /* monitor mode */
        cmdRegs->Config_A = (accCfg->queue.qNum) | (accCfg->queue.qMgr << 8) | (accCfg->monitor.pktCountThresh << 16);
        cmdRegs->Config_B = (accCfg->pacingTickCnt) | (accCfg->monitor.pacingMode << 22) | (0x1 << 31);
    } else {
        /* list mode */
        cmdRegs->List_Buffer_Address = PAL_CPPI4_VIRT_2_PHYS(accCfg->list.listBase);
        cmdRegs->Config_A = (accCfg->queue.qNum) | (accCfg->queue.qMgr << 8) | (accCfg->list.maxPageEntry << 16);
        cmdRegs->Config_B = (accCfg->pacingTickCnt) | (accCfg->list.maxPageCnt << 16)
                | (accCfg->list.listEntrySize << 18) | (accCfg->list.listCountMode << 20)
                | (accCfg->list.stallAvoidance << 21)| (accCfg->list.pacingMode << 22);
    }
    cmdRegs->Command = (accCfg->accChanNum) | (APDSP_CMD_ENABLE << 8);

    dbgPrint("APDSP config @%p, value %x\n", &cmdRegs->List_Buffer_Address, cmdRegs->List_Buffer_Address);
    dbgPrint("APDSP config @%p, value %x\n", &cmdRegs->Config_A, cmdRegs->Config_A);
    dbgPrint("APDSP config @%p, value %x\n", &cmdRegs->Config_B, cmdRegs->Config_B);
    dbgPrint("APDSP config @%p, value %x\n", &cmdRegs->Command, cmdRegs->Command);

    /* TODO: 1000000 is a magic word picked up from mike's code. Need to understand
     * timeout values and fix the code
     */
    for(i=0; (i < 1000000) && (cmdRegs->Command & (0xFF << 8)); i++);

    if(i==1000000)
    {
        errPrint("Error: APDSP firmware not responding!, APDSP return code: 0x%02X\n", (cmdRegs->Command & (0xFF << 24)));
        PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);

        PAL_osMemFree(0, accChObj, sizeof(PAL_Cppi4AccChObj));

        return NULL;
    }
#endif

    accChObj->curPage = 0;

    PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);

    return (PAL_Cppi4AccChHnd) accChObj;
}
#else
PAL_Cppi4AccChHnd PAL_cppi4AccChOpen(PAL_Handle hnd, Cppi4AccumulatorCfg* accCfg)
{
    /*Return pointer to the caller */
    PAL_Cppi4AccChObj *accChObj = NULL;
    /* transport message over HW_MBOX */
    Cppi41HwMboxAccChOpenMsg_t  openAccChObj = {0};
    /* local temporary varaibles */
    Cppi41HwMboxAccChOpenReplyMsg_t* tmp = NULL;
    unsigned long tmpPtr = 0;
    /* transport message over HW_MBOX */
    /* Return length of HW mailbox Op-Code channle */
    Uint32 dataLen = sizeof(Cppi41HwMboxAccChOpenMsg_t);
    if(!accCfg)
    {
        errPrint("NULL pointer reference.");
        return NULL;
    }

    /* kmalloc returns cache line aligned memory unless you are debugging the slab allocator (2.6.18) */
    accChObj = (PAL_Cppi4AccChObj *)kzalloc(sizeof(PAL_Cppi4AccChObj) ,GFP_KERNEL);
    if(!accChObj)
    {
        errPrint("could not allocate memeory for local accumulator ojbect");
        return NULL;
    }

    /*copy accCfg data to accumulator channel onject */
    if(!accCfg->list.listBase)
    {
        errPrint("NULL pointer reference. for accCfg.list.base");
        kfree(accChObj);
        return NULL;
    }

    /* Copy datapipe accumulator init paramters into the message container */
    memcpy(&openAccChObj.initCfg, accCfg, sizeof(Cppi4AccumulatorCfg));  
    dbgPrint(" Virtual  list.listBase=%p, address received=%p before sending to HWMbox.\n",  openAccChObj.initCfg.list.listBase,  accCfg->list.listBase);

    /* APPCPU virtual address need to converted to Physical address before sending to HW mailbox */
    tmpPtr = (unsigned long)PAL_CPPI4_VIRT_2_PHYS(openAccChObj.initCfg.list.listBase);
    openAccChObj.initCfg.list.listBase = (void*)tmpPtr; 
    dbgPrint(" Physical  list.listBase=%p, Original address received=%p before sending to HWMbox.\n",  openAccChObj.initCfg.list.listBase,  accCfg->list.listBase);

    /* hardware mailbox implementation to open accumulator channel goes here */
    if(hwMbox_isReady())
    { 
        errPrint("HW mailbox isn't ready yet.");
        kfree(accChObj);
        return NULL;
    }
    /* need to convert data from cpu_to_be(); */
     
    if(!Cppi41HwMboxAccChangeEndianness(&openAccChObj, endiannessBig))
    {
        errPrint("data conversion fo endianness failed");
        kfree(accChObj);
        return NULL;
    }

    /* need to send accumulator handler as well  though we are not using it right now but incase needed in future */
    /*  will receive back Object address in SendReplyOp() at npcpuAddress variable */
     openAccChObj.cmd = cpu_to_be32(CPPI41_HWMBOX_CMD_ACC_CH_OPEN);
    /* send a message to NP-CPU and expect a 64 byte reply back using SendReplyOp()*/
    dbgPrint(" size of data length=%d.", sizeof(Cppi41HwMboxAccChOpenMsg_t));
    if(hwMbox_sendOpcode(HW_MBOX_MASTER_APP_CPU, NPCPU_APPCPU_HW_MBOX_TAG_CPPI41_MBX , (uint8_t *)&openAccChObj, sizeof(Cppi41HwMboxAccChOpenMsg_t) , sizeof(Cppi41HwMboxAccChOpenMsg_t) , &dataLen))
    {
        errPrint("HW mailbox hwMbox_sendOpcode failed.");
        kfree(accChObj);
        return NULL;
    }
    if(dataLen != sizeof(Cppi41HwMboxAccChOpenReplyMsg_t))
    {
        errPrint("HW mailbox hwMbox_sendOpcode reply wasnt of desire length Cppi41HwMboxAccChOpenReplyMsg=%d dataLen=%d ",sizeof(Cppi41HwMboxAccChOpenReplyMsg_t), dataLen);
        kfree(accChObj);
        return NULL;
    }

    dbgPrint("HW mailbox adpHwMboxmessageObj.msgData.initCfg.list.listBase before Endian change=%p.", openAccChObj.initCfg.list.listBase);
    /* need to conver data from be_to_cpu(); */

    dbgPrint("HW mailbox Received adpHwMboxmessageObj.msgData.initCfg.list.listBase after Endian change=%p.", openAccChObj.initCfg.list.listBase);
    dbgPrint("HW mailbox called to  accumulator open successful.");
    /* copy HW_Mbox message to kmalloced object for return */
    dbgPrint("data length=%d.",dataLen);
    memcpy(&(accChObj->initCfg), accCfg, sizeof(Cppi4AccumulatorCfg)); 
    tmp = (Cppi41HwMboxAccChOpenReplyMsg_t *) &openAccChObj;
    accChObj->curPage = be32_to_cpu(tmp->curPage);
    dbgPrint("curPage=%d.", accChObj->curPage);
    accChObj->palCppi4Obj = (void *)be32_to_cpu((unsigned int)(tmp->accChHnd));
    dbgPrint("npcpuAddress=%d.", tmp->accChHnd);

    dbgPrint("HW mailbox Received accChObj->initCfg.list.listBase after phys_to_virt=%p.", accChObj->initCfg.list.listBase);

    return (PAL_Cppi4AccChHnd)accChObj;  
}
#endif

void PAL_cppi4AccChEnRateLimit(Cppi4AccumulatorRateLimitCfg* accCfg)
{
    Uint32                      cookie;
    AVALANCHE_PP_RET_e          rc;
    AVALANCHE_PP_ACC_CH_INFO_t  ptr_ch_cfg;

    /* Need to protect the accumulator register writes. They are shared with pre-fetcher */
    PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &cookie);

    ptr_ch_cfg.Index        =   1 ; // Enable Rate Limit
    ptr_ch_cfg.Channel      =   accCfg->accChanNum ;
    ptr_ch_cfg.Command      =   0 ;
    ptr_ch_cfg.Param0Ret    =   (accCfg->retQ);
    ptr_ch_cfg.Param1       =   (accCfg->upperThreshold << 16) | (accCfg->lowerThreshold);
    ptr_ch_cfg.Param2       =   0;

    rc = __cppi4AccChEnRateLimit(&ptr_ch_cfg);
    if (PP_RC_SUCCESS != rc)
    {
        errPrint("Error: Accumulator PDSP is not responding, return code: %u\n", rc);
        PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);
        return;
    }

    PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);
}

#ifdef CONFIG_INTEL_KERNEL_PP_DRIVER_LOCAL
PAL_Result PAL_cppi4AccChClose(PAL_Cppi4AccChHnd hnd, Ptr closeArgs)
{
    PAL_Cppi4AccChObj *accChObj = (PAL_Cppi4AccChObj *) hnd;

#if PUMA7_OR_NEWER_SOC_TYPE
    AVALANCHE_PP_ACC_CH_INFO_t ptr_ch_cfg;
    AVALANCHE_PP_RET_e rc; //return code

    ptr_ch_cfg.Channel      =   accChObj->initCfg.accChanNum ;
    ptr_ch_cfg.Command      =   0 ;
    ptr_ch_cfg.Param0Ret    =   0 ;
    ptr_ch_cfg.Param1       =   0 ;
    ptr_ch_cfg.Param2       =   0 ;

    if (__cppi4AccChClose(&ptr_ch_cfg) !=  PP_RC_SUCCESS)
    {
        dbgPrint("\nError: APDSP firmware not responding!");
        dbgPrint("APDSP return code: %d\n", rc);
        return PAL_ERROR_FLAG;
    }
#else
    Cppi4PALObj *palCppi4Obj = accChObj->palCppi4Obj;
    Cppi4InitCfg * initCfg = palCppi4Obj->initCfg;
    Uint32 i;
    APDSP_Command_Status_RegsOvly cmdRegs = initCfg->apdspInfo.pdspCmdBase;
    cmdRegs->List_Buffer_Address = 0;
    cmdRegs->Config_A = 0;
    cmdRegs->Config_B = 0;
    cmdRegs->Command = (accChObj->initCfg.accChanNum) | (APDSP_CMD_DISABLE << 8);

    /* TODO: 1000000 is a magic word picked up from mike's code. Need to understand
     * timeout values and fix the code
     */
    for(i=0; (i < 1000000) && (cmdRegs->Command & (0xFF << 8)); i++);
    if(i==1000000) {
        dbgPrint("\nError: APDSP firmware not responding!");
        dbgPrint("APDSP return code: %x\n", (cmdRegs->Command & (0xFF << 24)));
        return PAL_ERROR_FLAG;
    }
#endif
    PAL_osMemFree(0, hnd, sizeof(PAL_Cppi4AccChObj));

    return PAL_SOK;
}
#else
PAL_Result PAL_cppi4AccChClose(PAL_Cppi4AccChHnd hnd, void *closeArgs)
{
    /* local pointer to free */
    PAL_Cppi4AccChObj *accChObj;
    /* transport message over HW_MBOX */
    Cppi41HwMboxAccChCloseMsg_t adpHwMboxmessageObj;
    Uint32 dataLen = sizeof(Cppi41HwMboxAccChCloseMsg_t);

    if(!hnd)
    {
        errPrint("NULL pointer reference.");
        return PAL_ERROR_FLAG;
    }

    accChObj = (PAL_Cppi4AccChObj *)hnd;
    /*copy PAL_Cppi4AccChObj data to accumulator channel onject */
    /* convert data since CPPI need ch_num for accumulator close  */
    adpHwMboxmessageObj.accChHnd =  (void *)cpu_to_be32((unsigned int)(accChObj->palCppi4Obj));  
    dbgPrint("npcpuAddress=%d.", accChObj->palCppi4Obj);

    adpHwMboxmessageObj.cmd = cpu_to_be32(CPPI41_HWMBOX_CMD_ACC_CH_CLOSE);
    /* send a message to NP-CPU and expect to pointer get free in NPCPUaddress space make sure correct poiter by reply*/
    if(hwMbox_sendOpcode(HW_MBOX_MASTER_APP_CPU,NPCPU_APPCPU_HW_MBOX_TAG_CPPI41_MBX, (uint8_t *)&adpHwMboxmessageObj, sizeof(Cppi41HwMboxAccChCloseMsg_t), sizeof(Cppi41HwMboxAccChCloseMsg_t), &dataLen))
    {
        errPrint("HW mailbox hwMbox_sendOpcode failed.");
        return PAL_ERROR_FLAG;
    }
    /* free local onject which was created in Open call */
    kfree(accChObj);
    /* hardware mailbox implementation to close accumulator channel goes here */
    dbgPrint("HW mailbox called to free accumulator channel successful.");
    return PAL_SOK;
}
#endif
/*
 * Get the next accumulator page
 */
Ptr PAL_cppi4AccChGetNextList(PAL_Cppi4AccChHnd hnd)
{
    PAL_Cppi4AccChObj *accChObj = (PAL_Cppi4AccChObj *) hnd;
    Cppi4AccumulatorCfg* initCfg = &accChObj->initCfg;
    Ptr ret = 0;

    if(initCfg->mode) return NULL; /* no lists in monitor mode */

    /* data available at base + (current page * number of entries per page * size of each entry) */
    ret = initCfg->list.listBase + (accChObj->curPage * initCfg->list.maxPageEntry * (initCfg->list.listEntrySize + 1) * sizeof(Uint32));

    /* invalidate the list page */
    PAL_CPPI4_CACHE_INVALIDATE(ret, initCfg->list.maxPageEntry * (initCfg->list.listEntrySize + 1) * sizeof(Uint32));

    accChObj->curPage++;
    if(accChObj->curPage >= initCfg->list.maxPageCnt) accChObj->curPage = 0;

    return ret;
}


/*
 *  PAL_cppi4TxChOpen
 *    - Verify channel info (range checking etc)
 *    - Allocate memory for the channel
 *    - Channel information stored within PAL structure
 *  \note "chOpenArgs" is not used
 */
PAL_Cppi4TxChHnd PAL_cppi4TxChOpen (PAL_Handle hnd, Cppi4TxChInitCfg* info, Ptr chOpenArgs)
{
    Uint32 chNum = info->chNum, val;
    Cppi4PALObj *palCppi4Obj = (Cppi4PALObj *) hnd;
    Cppi4InitCfg * initCfg = palCppi4Obj->initCfg;
    PAL_Cppi4TxChObj *txChObj;
    Uint32 maxChNum = PAL_CPPI41_NUM_TOTAL_CHAN;

#if PUMA7_OR_NEWER_SOC_TYPE
    if (palCppi4Obj->myDomain == CPPI41_DOMAIN_PP)
    {
        maxChNum = PAL_CPPI41_PP_NUM_TOTAL_CHAN;
    }
    else if (palCppi4Obj->myDomain == CPPI41_DOMAIN_DSG)
    {
        maxChNum = PAL_CPPI41_DSG_NUM_TOTAL_CHAN;
    }
    else if (palCppi4Obj->myDomain == CPPI41_DOMAIN_DOCSIS_US)
    {
        // TBD - US team to set their own value
    }
#endif

    if (chNum >= maxChNum)
    {
        errPrint("\nERROR: chNum:%d out of range: 0-%d", chNum, maxChNum);
        return (NULL);
    }

    if (PAL_osMemAlloc(0, sizeof(PAL_Cppi4TxChObj), 0, (Ptr *) &txChObj) != PAL_SOK)
    {
        errPrint ("ERROR: Failed to allocate Tx channel object structure");
        return NULL;
    }

    PAL_osMemSet (txChObj, 0, sizeof (PAL_Cppi4TxChObj));

    PAL_osMemCopy(&txChObj->initCfg, info, sizeof(Cppi4TxChInitCfg));

    /* Populate channel Obj structure to return to calling function */
    txChObj->palCppi4Obj = hnd;

    /* initialize the hardware */

    /* setup the teardown complete queue */
    val = (info->tdQueue.qMgr << DMA_CHAN_CTRL_TX_GLOBAL_DEF_QMGR_SHIFT) & DMA_CHAN_CTRL_TX_GLOBAL_DEF_QMGR_MASK;
    val |= (info->tdQueue.qNum << DMA_CHAN_CTRL_TX_GLOBAL_DEF_QNUM_SHIFT) & DMA_CHAN_CTRL_TX_GLOBAL_DEF_QNUM_MASK;

    initCfg->dmaBlock[info->dmaNum].chCtrlStatusBase->Channel_Config[chNum].Tx_Global_Config = txChObj->txGlobalConfig = val;

    dbgPrint("dmaNum: %d, chNum: %d, tdQueue.qMgr: %d, tdQueue.qNum: %d, TX Global Config REG @0x%p=0x%08X\n",
             info->dmaNum, chNum, info->tdQueue.qMgr, info->tdQueue.qNum,
             IO_VIRT2PHY(&initCfg->dmaBlock[info->dmaNum].chCtrlStatusBase->Channel_Config[chNum].Tx_Global_Config), val);

    txChObj->isEnabled = False;

    return (PAL_Cppi4TxChHnd) txChObj;
}

/*
 *  PAL_cppi4RxChOpen
 *    - Verify channel info (range checking etc)
 *    - Allocate memory for the channel
*    - Channel information stored within PAL structure
 *  \note "chOpenArgs" is not used
 */
PAL_Cppi4RxChHnd PAL_cppi4RxChOpen (PAL_Handle hnd, Cppi4RxChInitCfg* info, Ptr chOpenArgs)
{
    Uint32 chNum;
    Uint32 val;
    Uint32 valA, valB;
    Uint32 dmaNum;
    Cppi4PALObj *palCppi4Obj;
    Cppi4InitCfg * initCfg;
    PAL_Cppi4RxChObj *rxChObj;
    Uint32 maxChNum = PAL_CPPI41_NUM_TOTAL_CHAN;

    chNum = info->chNum;
    dmaNum = info->dmaNum;
    palCppi4Obj = (Cppi4PALObj *) hnd;
    initCfg = palCppi4Obj->initCfg;


#if PUMA7_OR_NEWER_SOC_TYPE
    if (palCppi4Obj->myDomain == CPPI41_DOMAIN_PP)
    {
        maxChNum = PAL_CPPI41_PP_NUM_TOTAL_CHAN;
    }
    else if (palCppi4Obj->myDomain == CPPI41_DOMAIN_DSG)
    {
        maxChNum = PAL_CPPI41_DSG_NUM_TOTAL_CHAN;
    }
    else if (palCppi4Obj->myDomain == CPPI41_DOMAIN_DOCSIS_US)
    {
        // TBD - US team to set their own value
    }
#endif

    if (chNum >= maxChNum)
    {
        errPrint ("ERROR: chNum: %d, out of range: 0-%d", chNum, maxChNum);
        return (NULL);
    }

    if (PAL_osMemAlloc(0, sizeof(PAL_Cppi4RxChObj), 0, (Ptr *) &rxChObj) != PAL_SOK)
    {
        errPrint ("ERROR: Failed to allocate Rx channel object structure");
        return NULL;
    }

    PAL_osMemSet (rxChObj, 0, sizeof (PAL_Cppi4RxChObj));

    PAL_osMemCopy(&rxChObj->initCfg, info, sizeof(Cppi4RxChInitCfg));

    /* Populate channel Obj structure to return to calling function */
    rxChObj->palCppi4Obj = hnd;

    /* initialize the hardware */

    /* set the sop offset */
    val = (info->sopOffset << DMA_CHAN_CTRL_RX_GLOBAL_SOP_OFFSET_SHIFT) & DMA_CHAN_CTRL_RX_GLOBAL_SOP_OFFSET_MASK;
    /* set the desc type */
    val |= (info->defDescType << DMA_CHAN_CTRL_RX_GLOBAL_DEF_DESC_SHIFT) & DMA_CHAN_CTRL_RX_GLOBAL_DEF_DESC_MASK;
    /* set starvation retry policy */
    val |= (info->retryOnStarvation << DMA_CHAN_CTRL_RX_GLOBAL_ERROR_HANDLING_SHIFT) & DMA_CHAN_CTRL_RX_GLOBAL_ERROR_HANDLING_MASK;
    /* program completion queues */
    val |= (info->rxCompQueue.qMgr << DMA_CHAN_CTRL_RX_GLOBAL_RXCOMP_QMGR_SHIFT) & DMA_CHAN_CTRL_RX_GLOBAL_RXCOMP_QMGR_MASK;
    val |= (info->rxCompQueue.qNum << DMA_CHAN_CTRL_RX_GLOBAL_RXCOMP_QNUM_SHIFT) & DMA_CHAN_CTRL_RX_GLOBAL_RXCOMP_QNUM_MASK;


    initCfg->dmaBlock[dmaNum].chCtrlStatusBase->Channel_Config[chNum].Rx_Global_Config = rxChObj->rxGlobalConfig = val;

    dbgPrint("dmaNum: %d, chNum: %d, rxCompQueue.qMgr: %d, rxCompQueue.qNum: %d, sopOffset: %d, defDescType: %d, Global Config REG: @0x%p=0x%08X\n",
             info->dmaNum, info->chNum, info->rxCompQueue.qMgr, info->rxCompQueue.qNum, info->sopOffset, info->defDescType,
             IO_VIRT2PHY(&initCfg->dmaBlock[dmaNum].chCtrlStatusBase->Channel_Config[chNum].Rx_Global_Config), val);

    /* set up the packet configuration register based on the descriptor type */

    switch(info->defDescType)
    {
    case 0: /* embedded desc type */
        val = (info->u.embeddedPktCfg.fdQueue.qMgr << DMA_CHAN_CTRL_RX_EMBEDPKT_FDQ_QMGR_SHIFT) & DMA_CHAN_CTRL_RX_EMBEDPKT_FDQ_QMGR_MASK;
        val |= (info->u.embeddedPktCfg.fdQueue.qNum << DMA_CHAN_CTRL_RX_EMBEDPKT_FDQ_QNUM_SHIFT) & DMA_CHAN_CTRL_RX_EMBEDPKT_FDQ_QNUM_MASK;
        val |= (info->u.embeddedPktCfg.numBufSlot << DMA_CHAN_CTRL_RX_EMBEDPKT_NUM_SLOT_SHIFT) & DMA_CHAN_CTRL_RX_EMBEDPKT_NUM_SLOT_MASK;
        val |= (info->u.embeddedPktCfg.sopSlotNum << DMA_CHAN_CTRL_RX_EMBEDPKT_SOP_SLOT_SHIFT) & DMA_CHAN_CTRL_RX_EMBEDPKT_SOP_SLOT_MASK;
        initCfg->dmaBlock[dmaNum].chCtrlStatusBase->Channel_Config[chNum].Embedded_Pkt_Config_Reg_B = val;
        valB = val;

        val = (info->u.embeddedPktCfg.fBufPool[0].bPool << DMA_CHAN_CTRL_RX_EMBEDPKT_FBP0_PNUM_SHIFT) & DMA_CHAN_CTRL_RX_EMBEDPKT_FBP0_PNUM_MASK;
        val |= (info->u.embeddedPktCfg.fBufPool[0].bMgr << DMA_CHAN_CTRL_RX_EMBEDPKT_FBP0_BMGR_SHIFT) & DMA_CHAN_CTRL_RX_EMBEDPKT_FBP0_BMGR_MASK;

        val |= (info->u.embeddedPktCfg.fBufPool[1].bPool << DMA_CHAN_CTRL_RX_EMBEDPKT_FBP1_PNUM_SHIFT) & DMA_CHAN_CTRL_RX_EMBEDPKT_FBP1_PNUM_MASK;
        val |= (info->u.embeddedPktCfg.fBufPool[1].bMgr << DMA_CHAN_CTRL_RX_EMBEDPKT_FBP1_BMGR_SHIFT) & DMA_CHAN_CTRL_RX_EMBEDPKT_FBP1_BMGR_MASK;
        val |= (info->u.embeddedPktCfg.fBufPool[2].bPool << DMA_CHAN_CTRL_RX_EMBEDPKT_FBP2_PNUM_SHIFT) & DMA_CHAN_CTRL_RX_EMBEDPKT_FBP2_PNUM_MASK;
        val |= (info->u.embeddedPktCfg.fBufPool[2].bMgr << DMA_CHAN_CTRL_RX_EMBEDPKT_FBP2_BMGR_SHIFT) & DMA_CHAN_CTRL_RX_EMBEDPKT_FBP2_BMGR_MASK;
        val |= (info->u.embeddedPktCfg.fBufPool[3].bPool << DMA_CHAN_CTRL_RX_EMBEDPKT_FBP3_PNUM_SHIFT) & DMA_CHAN_CTRL_RX_EMBEDPKT_FBP3_PNUM_MASK;
        val |= (info->u.embeddedPktCfg.fBufPool[3].bMgr << DMA_CHAN_CTRL_RX_EMBEDPKT_FBP3_BMGR_SHIFT) & DMA_CHAN_CTRL_RX_EMBEDPKT_FBP3_BMGR_MASK;
        initCfg->dmaBlock[dmaNum].chCtrlStatusBase->Channel_Config[chNum].Embedded_Pkt_Config_Reg_A = val;
        valA = val;

        dbgPrint("dmaNum: %d, chNum: %d, bMgr: %d, bPool: %d, fdQueue.qMgr: %d, fdQueue.qNum: %d, numBufSlot: %d, sopSlotNum: %d, EMB Config REG A @0x%p=0x%08X, EMB Config REG B @0x%p=0x%08X\n",
                 info->dmaNum, info->chNum,
                 info->u.embeddedPktCfg.fBufPool[0].bMgr, info->u.embeddedPktCfg.fBufPool[0].bPool,
                 info->u.embeddedPktCfg.fdQueue.qMgr, info->u.embeddedPktCfg.fdQueue.qNum, info->u.embeddedPktCfg.numBufSlot, info->u.embeddedPktCfg.sopSlotNum,
                 IO_VIRT2PHY(&initCfg->dmaBlock[dmaNum].chCtrlStatusBase->Channel_Config[chNum].Embedded_Pkt_Config_Reg_A), valA,
                 IO_VIRT2PHY(&initCfg->dmaBlock[dmaNum].chCtrlStatusBase->Channel_Config[chNum].Embedded_Pkt_Config_Reg_B), valB);
        break;

    case 1: /* host desc type */
        val = (info->u.hostPktCfg.fdbQueue[0].qNum << DMA_CHAN_CTRL_RX_HOSTPKT_FDB0_QNUM_SHIFT) & DMA_CHAN_CTRL_RX_HOSTPKT_FDB0_QNUM_MASK;
        val |= (info->u.hostPktCfg.fdbQueue[0].qMgr << DMA_CHAN_CTRL_RX_HOSTPKT_FDB0_QMGR_SHIFT) & DMA_CHAN_CTRL_RX_HOSTPKT_FDB0_QMGR_MASK;
        val |= (info->u.hostPktCfg.fdbQueue[1].qNum << DMA_CHAN_CTRL_RX_HOSTPKT_FDB1_QNUM_SHIFT) & DMA_CHAN_CTRL_RX_HOSTPKT_FDB1_QNUM_MASK;
        val |= (info->u.hostPktCfg.fdbQueue[1].qMgr << DMA_CHAN_CTRL_RX_HOSTPKT_FDB1_QMGR_SHIFT) & DMA_CHAN_CTRL_RX_HOSTPKT_FDB1_QMGR_MASK;
        initCfg->dmaBlock[dmaNum].chCtrlStatusBase->Channel_Config[chNum].Host_Pkt_Config_Reg_A = val;
        valA = val;

        val = (info->u.hostPktCfg.fdbQueue[2].qNum << DMA_CHAN_CTRL_RX_HOSTPKT_FDB2_QNUM_SHIFT) & DMA_CHAN_CTRL_RX_HOSTPKT_FDB2_QNUM_MASK;
        val |= (info->u.hostPktCfg.fdbQueue[2].qMgr << DMA_CHAN_CTRL_RX_HOSTPKT_FDB2_QMGR_SHIFT) & DMA_CHAN_CTRL_RX_HOSTPKT_FDB2_QMGR_MASK;
        val |= (info->u.hostPktCfg.fdbQueue[3].qNum << DMA_CHAN_CTRL_RX_HOSTPKT_FDB3_QNUM_SHIFT) & DMA_CHAN_CTRL_RX_HOSTPKT_FDB3_QNUM_MASK;
        val |= (info->u.hostPktCfg.fdbQueue[3].qMgr << DMA_CHAN_CTRL_RX_HOSTPKT_FDB3_QMGR_SHIFT) & DMA_CHAN_CTRL_RX_HOSTPKT_FDB3_QMGR_MASK;
        initCfg->dmaBlock[dmaNum].chCtrlStatusBase->Channel_Config[chNum].Host_Pkt_Config_Reg_B = val;
        valB = val;

        dbgPrint("dmaNum: %d, chNum: %d, fdQueue.qMgr: %d, fdQueue.qNum: %d, HOST Config REG A @0x%p=0x%08X, HOST Config REG B @0x%p=0x%08X\n",
                 info->dmaNum, info->chNum,
                 info->u.hostPktCfg.fdbQueue[0].qMgr, info->u.hostPktCfg.fdbQueue[0].qNum,
                 IO_VIRT2PHY(&initCfg->dmaBlock[dmaNum].chCtrlStatusBase->Channel_Config[chNum].Host_Pkt_Config_Reg_A), valA,
                 IO_VIRT2PHY(&initCfg->dmaBlock[dmaNum].chCtrlStatusBase->Channel_Config[chNum].Host_Pkt_Config_Reg_B), valB);
        break;

    case 2: /* monolithic desc type */
        val = (info->u.monolithicPktCfg.fdQueue.qNum << DMA_CHAN_CTRL_RX_MONOLITHICPKT_FDQ_QNUM_SHIFT) & DMA_CHAN_CTRL_RX_MONOLITHICPKT_FDQ_QNUM_MASK;
        val |= (info->u.monolithicPktCfg.fdQueue.qMgr << DMA_CHAN_CTRL_RX_MONOLITHICPKT_FDQ_QMGR_SHIFT) & DMA_CHAN_CTRL_RX_MONOLITHICPKT_FDQ_QMGR_MASK;
        val |= (info->u.monolithicPktCfg.sopOffset << DMA_CHAN_CTRL_RX_MONOLITHICPKT_SOP_OFFSET_SHIFT) & DMA_CHAN_CTRL_RX_MONOLITHICPKT_SOP_OFFSET_MASK;
        initCfg->dmaBlock[dmaNum].chCtrlStatusBase->Channel_Config[chNum].Monolithic_Pkt_Config_Reg_A = val;

        dbgPrint("dmaNum: %d, chNum: %d, fdQueue.qMgr: %d, fdQueue.qNum: %d, sopOffset: %d, MONOLITHIC Config REG A @0x%p=0x%08X\n",
                 info->dmaNum, info->chNum, info->u.monolithicPktCfg.fdQueue.qMgr, info->u.monolithicPktCfg.fdQueue.qNum,
                 IO_VIRT2PHY(&initCfg->dmaBlock[dmaNum].chCtrlStatusBase->Channel_Config[chNum].Monolithic_Pkt_Config_Reg_A), val);
        break;
    }

    rxChObj->isEnabled = False;

    return (PAL_Cppi4RxChHnd) rxChObj;
}



/*
 * PAL_cppi4TxChStatus
 *    - Returns when the channel teardown is complete.
 *    - Used when Teardown descriptor is forwarded back to Free teardown queue.
 *   - Mostly used in case of Teardown of Embedded endpoint Channels and Infra mode channels
 */
PAL_Result PAL_cppi4TxChStatus (PAL_Cppi4TxChHnd hnd, Ptr chCloseArgs)
{

    PAL_Cppi4TxChObj *txChObj = (PAL_Cppi4TxChObj *) hnd;
    Cppi4PALObj *palCppi4Obj = txChObj->palCppi4Obj;
    Cppi4InitCfg * initCfg = palCppi4Obj->initCfg;
    Uint32 chNum = txChObj->initCfg.chNum, dmaNum = txChObj->initCfg.dmaNum;
    Uint32 status;
    Uint32 count = 0;
    do
    {
        status = initCfg->dmaBlock[dmaNum].chCtrlStatusBase->Channel_Config[chNum].Tx_Global_Config;
        status &= CPPI4_CH_ENABLE_MASK;
        count++;
    }while(status && count < 10000);
    if(count >= 10000)
    {
        return PAL_ERROR_FLAG;
        errPrint ("\nERROR:PAL: PAL_cppi4TxChStatus: Failed to Teardown Channel");
    }

    return PAL_SOK;
}

/*
 * PAL_cppi4TxChClose
 *    - Teardown a given Tx channel
 */
PAL_Result PAL_cppi4TxChClose (PAL_Cppi4TxChHnd hnd, Ptr chCloseArgs) {
    PAL_Cppi4TxChObj *txChObj = (PAL_Cppi4TxChObj *) hnd;
    Cppi4PALObj *palCppi4Obj = txChObj->palCppi4Obj;
    Cppi4InitCfg * initCfg = palCppi4Obj->initCfg;
    Uint32 chNum = txChObj->initCfg.chNum, dmaNum = txChObj->initCfg.dmaNum;

    /* initiate channel teardown */
    txChObj->txGlobalConfig |= (1 << DMA_CHAN_CTRL_TX_GLOBAL_CHAN_TD_SHIFT);
    initCfg->dmaBlock[dmaNum].chCtrlStatusBase->Channel_Config[chNum].Tx_Global_Config = txChObj->txGlobalConfig;

    dbgPrint("txChObj->txGlobalConfig: %x\n", txChObj->txGlobalConfig);

    return (PAL_SOK);
}

/*
 * PAL_cppi4TxChClose
 *    - Free all stored channel information for the given Transmit channel
 */
PAL_Result PAL_cppi4TxChDestroy (PAL_Cppi4TxChHnd hnd, Ptr chCloseArgs)
{
    PAL_Cppi4TxChObj *txChObj = (PAL_Cppi4TxChObj *) hnd;

    /* free up the handle itself */
    PAL_osMemFree(0, txChObj, sizeof(PAL_Cppi4TxChObj));

    return PAL_SOK;
}

/*
 * PAL_cppi4RxChStatus
 *    - Returns when the channel teardown is complete.
 *    - Used when Teardown descriptor is forwarded back to Free teardown queue.
 *   - Mostly used in case of Teardown of Embedded endpoint Channels and Infra mode channels
 */
PAL_Result PAL_cppi4RxChStatus (PAL_Cppi4RxChHnd hnd, Ptr chCloseArgs)
{
    PAL_Cppi4RxChObj *rxChObj = (PAL_Cppi4RxChObj *) hnd;
    Cppi4PALObj *palCppi4Obj = rxChObj->palCppi4Obj;
    Cppi4InitCfg * initCfg = palCppi4Obj->initCfg;
    Uint32 chNum = rxChObj->initCfg.chNum, dmaNum = rxChObj->initCfg.dmaNum;
    Uint32 status;
    Uint32 count = 0;

    do
    {
        status = initCfg->dmaBlock[dmaNum].chCtrlStatusBase->Channel_Config[chNum].Rx_Global_Config;
        status &= CPPI4_CH_ENABLE_MASK;
        count++;
    }while(status && count < 10000);
    if(count >= 10000)
    {
        return PAL_ERROR_FLAG;
        errPrint ("\nERROR:PAL: PAL_cppi4RxChStatus: Failed to Teardown Channel");
    }
    return PAL_SOK;
}

/*
 * PAL_cppi4RxChClose
 *    - Teardown the given Receive channel
 */
PAL_Result PAL_cppi4RxChClose (PAL_Cppi4RxChHnd  hnd, Ptr chCloseArgs) {
    PAL_Cppi4RxChObj *rxChObj = (PAL_Cppi4RxChObj *) hnd;
    Cppi4PALObj *palCppi4Obj = rxChObj->palCppi4Obj;
    Cppi4InitCfg * initCfg = palCppi4Obj->initCfg;
    Uint32 chNum = rxChObj->initCfg.chNum, dmaNum = rxChObj->initCfg.dmaNum;

    /* initiate channel teardown */
    rxChObj->rxGlobalConfig |= (1 << DMA_CHAN_CTRL_RX_GLOBAL_CHAN_TD_SHIFT);
    initCfg->dmaBlock[dmaNum].chCtrlStatusBase->Channel_Config[chNum].Rx_Global_Config = rxChObj->rxGlobalConfig;

    return (PAL_SOK);
}

/*
 * PAL_cppi4RxChDestroy
 *    - Free all stored channel information for the given Receive channel
 */
PAL_Result PAL_cppi4RxChDestroy (PAL_Cppi4RxChHnd  hnd, Ptr chCloseArgs)
{
    PAL_Cppi4RxChObj *rxChObj = (PAL_Cppi4RxChObj *) hnd;

    /* free up the handle itself */
    PAL_osMemFree(0, rxChObj, sizeof(PAL_Cppi4RxChObj));

    return (PAL_SOK);
}

/*
 *  PAL_cppi4EnableRxChannel
 *    - Channel DMA is enabled in hardware. Ready for data reception.
 */
PAL_Result PAL_cppi4EnableRxChannel (PAL_Cppi4RxChHnd hnd, Ptr chCloseArgs)
{
    PAL_Cppi4RxChObj *rxChObj = (PAL_Cppi4RxChObj *) hnd;
    Cppi4PALObj *palCppi4Obj = rxChObj->palCppi4Obj;
    Cppi4InitCfg * initCfg = palCppi4Obj->initCfg;
    Uint32 chNum = rxChObj->initCfg.chNum, dmaNum = rxChObj->initCfg.dmaNum;

    rxChObj->rxGlobalConfig |= (1 << DMA_CHAN_CTRL_RX_GLOBAL_CHAN_ENABLE_SHIFT);

    initCfg->dmaBlock[dmaNum].chCtrlStatusBase->Channel_Config[chNum].Rx_Global_Config = rxChObj->rxGlobalConfig;

    dbgPrint("dmaNum: %d, chNum: %d, RX Global Config REG @0x%p=0x%08X\n", dmaNum, chNum,
             IO_VIRT2PHY(&initCfg->dmaBlock[dmaNum].chCtrlStatusBase->Channel_Config[chNum].Rx_Global_Config), rxChObj->rxGlobalConfig);

    /* Mark channel open */
    rxChObj->isEnabled = True;

    return (PAL_SOK);
}

/*
 *  PAL_cppi4EnableTxChannel
 *    - Channel DMA is enabled in hardware. Ready for data transmission.
 */
PAL_Result PAL_cppi4EnableTxChannel (PAL_Cppi4TxChHnd hnd, Ptr chCloseArgs)
{
    PAL_Cppi4TxChObj *txChObj = (PAL_Cppi4TxChObj *) hnd;
    Cppi4PALObj *palCppi4Obj = txChObj->palCppi4Obj;
    Cppi4InitCfg * initCfg = palCppi4Obj->initCfg;
    Uint32 chNum = txChObj->initCfg.chNum, dmaNum = txChObj->initCfg.dmaNum;

    txChObj->txGlobalConfig |= (1 << DMA_CHAN_CTRL_TX_GLOBAL_CHAN_ENABLE_SHIFT);
    initCfg->dmaBlock[dmaNum].chCtrlStatusBase->Channel_Config[chNum].Tx_Global_Config = txChObj->txGlobalConfig;

    dbgPrint("dmaNum: %d, chNum: %d, RX Global Config REG @0x%p=0x%08X\n", dmaNum, chNum,
             IO_VIRT2PHY(&initCfg->dmaBlock[dmaNum].chCtrlStatusBase->Channel_Config[chNum].Tx_Global_Config), txChObj->txGlobalConfig);

    /* Mark channel open */
    txChObj->isEnabled = True;

    return (PAL_SOK);
}

/*
 *  PAL_cppi4DisableTxChannel
 *    - Channel DMA is disabled in hardware.
 *
 */
PAL_Result PAL_cppi4DisableTxChannel (PAL_Cppi4TxChHnd hnd, Ptr chCloseArgs)
{
    PAL_Cppi4TxChObj *txChObj = (PAL_Cppi4TxChObj *) hnd;
    Cppi4PALObj* palCppi4Obj = txChObj->palCppi4Obj;
    Cppi4InitCfg* initCfg = palCppi4Obj->initCfg;
    Uint32 chNum = txChObj->initCfg.chNum, dmaNum = txChObj->initCfg.dmaNum;


    txChObj->txGlobalConfig &=  ~(1 << DMA_CHAN_CTRL_TX_GLOBAL_CHAN_ENABLE_SHIFT);

    initCfg->dmaBlock[dmaNum].chCtrlStatusBase->Channel_Config[chNum].Tx_Global_Config = txChObj->txGlobalConfig;

    /* Mark channel open */
    txChObj->isEnabled = False;

    return (PAL_SOK);
}

/*
 *  PAL_cppi4DisableRxChannel
 *    - Channel DMA is disabled in hardware.
 */
PAL_Result PAL_cppi4DisableRxChannel (PAL_Cppi4RxChHnd hnd, Ptr chCloseArgs)
{
    PAL_Cppi4RxChObj *rxChObj = (PAL_Cppi4RxChObj *) hnd;
    Cppi4PALObj *palCppi4Obj = rxChObj->palCppi4Obj;
    Cppi4InitCfg * initCfg = palCppi4Obj->initCfg;
    Uint32 chNum = rxChObj->initCfg.chNum, dmaNum = rxChObj->initCfg.dmaNum;

    rxChObj->rxGlobalConfig &= ~(1 << DMA_CHAN_CTRL_RX_GLOBAL_CHAN_ENABLE_SHIFT);

    initCfg->dmaBlock[dmaNum].chCtrlStatusBase->Channel_Config[chNum].Rx_Global_Config = rxChObj->rxGlobalConfig;
            ;

    /* Mark channel open */
    rxChObj->isEnabled = False;

    return (PAL_SOK);
}

/*
 *  PAL_cppi4QueueOpen
 *  - Opens a CPPI4 queue for use.
 *  - The handle returned should be used for all push and pop
 *  operations on the queue.
 */
PAL_Cppi4QueueHnd PAL_cppi4QueueOpen (PAL_Handle hnd, Cppi4Queue  queue)
{
    Cppi4PALObj *palCppi4Obj = (Cppi4PALObj *) hnd;
    Cppi4InitCfg * initCfg = palCppi4Obj->initCfg;
    PAL_Cppi4QueueObj* queueObj;
    Uint32 retVal;
    Uint32 lockKey;

    /* Putting the whole process under lock since we are accessing the CPPI shared resources
     */
    PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &lockKey);
    if(palCppi4Obj->isQueueOpen[queue.qMgr][queue.qNum])
    {
        queueObj = (PAL_Cppi4QueueHnd) palCppi4Obj->qHnd[queue.qMgr][queue.qNum];
        queueObj->queueOpenCount++;
        PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, lockKey);
        return (PAL_Cppi4QueueHnd) palCppi4Obj->qHnd[queue.qMgr][queue.qNum];
    }
    else
    {
        /* allocate the Queue object structure */
        retVal = PAL_osMemAlloc(0, sizeof(PAL_Cppi4QueueObj), 0, (Ptr *) &queueObj);
        if (retVal != PAL_SOK)
        {
            errPrint ("\nERROR:PAL: PAL_cppi4QueueOpen: Failed to de-allocate descriptor memory");
            PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, lockKey);
            return NULL;
        }

        PAL_osMemSet (queueObj, 0, sizeof (PAL_Cppi4QueueObj));

        queueObj->palCppi4Obj = hnd;
        queueObj->queue = queue;
        queueObj->baseAddress       = &initCfg->queueMgrInfo[queue.qMgr].queueMgmtRgnBase->Queue_Mgmt[queue.qNum];
        if (initCfg->queueMgrInfo[queue.qMgr].queueProxyRgnBase)
        {
            queueObj->baseAddressProxy  = &initCfg->queueMgrInfo[queue.qMgr].queueProxyRgnBase->Queue_Mgmt[queue.qNum];
        }

        PAL_cppi4QueuePush(queueObj, 0, 0, 0);

#if PUMA7_OR_NEWER_SOC_TYPE
        // Dummy read due to QS Monitor feature
        PAL_cppi4QueuePop(queueObj);
#endif

        palCppi4Obj->isQueueOpen[queue.qMgr][queue.qNum] = TRUE;
        queueObj->queueOpenCount++;
        palCppi4Obj->qHnd[queue.qMgr][queue.qNum] = (Uint32)queueObj;

        PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, lockKey);
        return (PAL_Cppi4QueueHnd) (queueObj);
    }
}

/*
 *  PAL_cppi4QueueOpenNoReset
 *  - Opens a CPPI4 queue for use without resetting it.
 *  - The handle returned should be used for all push and pop
 *  operations on the queue.
 */
PAL_Cppi4QueueHnd PAL_cppi4QueueOpenNoReset (PAL_Handle hnd, Cppi4Queue  queue)
{
    Cppi4PALObj *palCppi4Obj = (Cppi4PALObj *) hnd;
    Cppi4InitCfg * initCfg = palCppi4Obj->initCfg;
    PAL_Cppi4QueueObj* queueObj;
    Uint32 retVal;
    Uint32 lockKey;

    if ((queue.qMgr >= PAL_CPPI41_NUM_QUEUE_MGR) || (queue.qNum >= PAL_CPPI41_SR_QMGR_TOTAL_Q_COUNT))
    {
        errPrint ("\nERROR:PAL: Local Array Index Out of Bounds qMgr %d qNum %d", queue.qMgr, queue.qNum);
        return NULL;
    }
    /* Putting the whole process under lock since we are accessing the CPPI shared resources
     */
    PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &lockKey);
    if(palCppi4Obj->isQueueOpen[queue.qMgr][queue.qNum])
    {
        queueObj = (PAL_Cppi4QueueHnd) palCppi4Obj->qHnd[queue.qMgr][queue.qNum];
        queueObj->queueOpenCount++;
        PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, lockKey);
        return (PAL_Cppi4QueueHnd) palCppi4Obj->qHnd[queue.qMgr][queue.qNum];
    }
    else
    {
        /* allocate the Queue object structure */
        retVal = PAL_osMemAlloc(0, sizeof(PAL_Cppi4QueueObj), 0, (Ptr *) &queueObj);
        if (retVal != PAL_SOK)
        {
            errPrint ("\nERROR:PAL: PAL_cppi4QueueOpen: Failed to de-allocate descriptor memory");
            PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, lockKey);
            return NULL;
        }

        PAL_osMemSet (queueObj, 0, sizeof (PAL_Cppi4QueueObj));

        queueObj->palCppi4Obj = hnd;
        queueObj->queue = queue;
        queueObj->baseAddress       = &initCfg->queueMgrInfo[queue.qMgr].queueMgmtRgnBase->Queue_Mgmt[queue.qNum];
        if (initCfg->queueMgrInfo[queue.qMgr].queueProxyRgnBase)
        {
            queueObj->baseAddressProxy  = &initCfg->queueMgrInfo[queue.qMgr].queueProxyRgnBase->Queue_Mgmt[queue.qNum];
        }

        palCppi4Obj->isQueueOpen[queue.qMgr][queue.qNum] = TRUE;
        queueObj->queueOpenCount++;
        palCppi4Obj->qHnd[queue.qMgr][queue.qNum] = (Uint32)queueObj;

        PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, lockKey);
        return (PAL_Cppi4QueueHnd) (queueObj);
    }
}

/*
 *  PAL_cppi4QueueClose
 *  - Closes a CPPI4 queue.
 */
PAL_Result PAL_cppi4QueueClose (PAL_Handle hnd, PAL_Cppi4QueueHnd qHnd)
{
    Int retVal;
    Cppi4PALObj *palCppi4Obj = (Cppi4PALObj *) hnd;
    PAL_Cppi4QueueObj* queueObj;
    Uint32 lockKey;

    queueObj = (PAL_Cppi4QueueHnd) qHnd;

    /* Disable irqs since we are accessing the CPPI shared resources
     */
    PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &lockKey);
    /* allocate the Queue object structure */
    if(!palCppi4Obj->isQueueOpen[queueObj->queue.qMgr][queueObj->queue.qNum])
    {
        errPrint("\nERROR: PAL_cppi4QueueClose: Queue is already closed\n");
        PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, lockKey);
        return CPPI4_ERR_DEV_ALREADY_CLOSED;
    }
    else
    {
        queueObj->queueOpenCount--;
    }

    if(queueObj->queueOpenCount == 0)
    {
        palCppi4Obj->isQueueOpen[queueObj->queue.qMgr][queueObj->queue.qNum] = 0;
        palCppi4Obj->qHnd[queueObj->queue.qMgr][queueObj->queue.qNum] = (Uint32)NULL;
        retVal = PAL_osMemFree(0, qHnd, sizeof(PAL_Cppi4QueueObj));
        if (retVal != PAL_SOK)
        {
            errPrint ("\nERROR:PAL: PAL_cppi4QueueClose: Failed to de-allocate descriptor memory");
            PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, lockKey);
            return PAL_OS_ERROR_NO_RESOURCES;
        }
    }
    PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, lockKey);
    return PAL_SOK;
}

/*
 * PAL_cppi4QueuePush
 *  - Pushes a descriptor to the given queue
 */
PAL_Result PAL_cppi4QueuePush (PAL_Cppi4QueueHnd hnd, Ptr dAddr, Uint32 dSize, Uint32 pSize)
{
    PAL_Cppi4QueueObj* qObj = (PAL_Cppi4QueueObj*) hnd;
    CSL_Queue_Mgmt_Regs* regs;
    Uint32 tmp;
    Uint32 lockKey;

    /* Write to tail of the queue. TODO: Cant think of a reason why a queue to head
     * may be required. If it is, the API may have to be extended.
     * Also, cant understand why packet size is required to queue up a descriptor. The spec
     * says packet size *must* be written prior to the packet write operaration
     */

    tmp = (dSize << QMGR_QUEUE_N_REG_D_DESCSZ_SHIFT) & QMGR_QUEUE_N_REG_D_DESCSZ_MASK;
    tmp|= ((Uint32)dAddr & QMGR_QUEUE_N_REG_D_DESC_ADDR_MASK);


    if (qObj->baseAddressProxy)
    {
        regs = qObj->baseAddressProxy;
        dbgPrintPushPop("QMGR: %d, QNUM: %d, @0x%p=0x%08X\n", qObj->queue.qMgr, qObj->queue.qNum, IO_VIRT2PHY(regs), tmp);

        PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &lockKey);
        regs->Queue_Reg_C = pSize;
        regs->Queue_Reg_D = tmp;
        PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, lockKey);
    }
    else
    {
        CSL_Queue_Mgmt_Regs     prep;
        regs = qObj->baseAddress;
        dbgPrintPushPop("QMGR: %d, QNUM: %d, @0x%p=0x%08X\n", qObj->queue.qMgr, qObj->queue.qNum, IO_VIRT2PHY(regs), tmp);

        if (pSize)
        {
            #if PUMA7_OR_NEWER_SOC_TYPE
            {
                Uint32 *    interruptAddr = (void *) AVALANCHE_PP_RECYCLER_INTRPT_BNDL_RGN_BASE + 4;
                Uint32 *    cmdAddr       = (void *) AVALANCHE_PP_RECYCLER_CMD_RGN_BASE + 0x100;

                while (*cmdAddr)
                {

                }

                *cmdAddr++  = IO_VIRT2PHY(&regs->Queue_Reg_C);
                *cmdAddr++  = pSize;
                *cmdAddr++  = tmp;

                *interruptAddr  = e_recycler_Interrupt_cmd_burst_Enable;
            }
            #else
            {
                prep.Queue_Reg_C = pSize;
                prep.Queue_Reg_D = tmp;
                memcpy(&regs->Queue_Reg_C,
                        &prep.Queue_Reg_C,
                        sizeof(prep.Queue_Reg_C) +
                        sizeof(prep.Queue_Reg_D));
            }
            #endif
        }
        else
        {
            regs->Queue_Reg_D = tmp;
        }
    }

    return (PAL_SOK);
}

PAL_Result PAL_cppi4QueuePushDirect (PAL_Cppi4QueueHnd hnd, Ptr dAddr, Uint32 dSize, Uint32 pSize)
{
    PAL_Cppi4QueueObj* qObj = (PAL_Cppi4QueueObj*) hnd;
    CSL_Queue_Mgmt_Regs* regs;
    Uint32 tmp;

    /* Write to tail of the queue. TODO: Cant think of a reason why a queue to head
     * may be required. If it is, the API may have to be extended.
     * Also, cant understand why packet size is required to queue up a descriptor. The spec
     * says packet size *must* be written prior to the packet write operaration
     */

    tmp = (dSize << QMGR_QUEUE_N_REG_D_DESCSZ_SHIFT) & QMGR_QUEUE_N_REG_D_DESCSZ_MASK;
    tmp|= ((Uint32)dAddr & QMGR_QUEUE_N_REG_D_DESC_ADDR_MASK);


    regs = qObj->baseAddress;
    regs->Queue_Reg_D = tmp;

    return (PAL_SOK);
}


/*
 * PAL_cppi4QueuePop
 *  - Pops a descriptor from a given queue
 */
PAL_Cppi4BD *PAL_cppi4QueuePop (PAL_Cppi4QueueHnd hnd)
{
    PAL_Cppi4BD* bd;

    CSL_Queue_Mgmt_Regs* regs = ((PAL_Cppi4QueueObj *)hnd)->baseAddress;

    bd = (regs->Queue_Reg_D & QMGR_QUEUE_N_REG_D_DESC_ADDR_MASK);

    dbgPrintPushPop("QMGR: %d, QNUM: %d, @0x%p=0x%08X\n", ((PAL_Cppi4QueueObj*)hnd)->queue.qMgr, ((PAL_Cppi4QueueObj*)hnd)->queue.qNum, IO_VIRT2PHY(regs), bd);

    return bd;
}

PAL_Result PAL_cppi4QueueGetEntryCount(PAL_Handle hnd, Cppi4Queue queue, Uint32 *entryCount)
{
    CSL_Queue_Status_Regs* regs = &(((Cppi4PALObj *)hnd)->initCfg->queueMgrInfo[queue.qMgr].queueStatusRgnBase->Queue_Status[queue.qNum]);
    *entryCount = regs->Queue_Status_Reg_A;

    return (PAL_SOK);
}

/*
 * PAL_cppi4GetTdInfo
 *  - Takes a Teardown descriptor and gives back the teardown info
 */
PAL_Result PAL_cppi4GetTdInfo(PAL_Handle hnd, PAL_Cppi4BD * cppi4TdBD, Bool* txRx, Uint32*  dmaNum, Uint32* chanNum)
{
    Cppi4TeardownDesc* tdBD = (Cppi4TeardownDesc*) cppi4TdBD;
    Cppi4PALObj *palCppi4Obj = (Cppi4PALObj *)hnd;
    Uint32 dmaNumPvt;

    Uint32 type;

    type = (tdBD->tdInfo & PAL_CPPI4_TDDESC_DESC_TYPE_MASK) >> PAL_CPPI4_TDDESC_DESC_TYPE_SHIFT;

    if(type != PAL_CPPI4_TDDESC_DESC_TYPE_TD)
        return (CPPI4_ERR_INVALID_DESC_TYPE);

    if(txRx) *txRx = (tdBD->tdInfo & CPPI4_TD_DESC_TX_RX_MASK) >> CPPI4_TD_DESC_TX_RX_SHIFT;
    /* dma number is taken from the software populated field and it was set during initial
     * push of the teardown descriptors onto the td descriptor free queue.  Hardware is  not
     * giving the current dma number as of now
     */
    dmaNumPvt = tdBD->swDmaNumber;
    //dmaNumPvt = (tdBD->tdInfo & CPPI4_TD_DESC_DMA_NUM_MASK) >> CPPI4_TD_DESC_DMA_NUM_SHIFT;
    if(dmaNum) *dmaNum = dmaNumPvt;
    if(chanNum) *chanNum = (tdBD->tdInfo & CPPI4_TD_DESC_CHAN_NUM_MASK) >> CPPI4_TD_DESC_CHAN_NUM_SHIFT;

    PAL_cppi4QueuePush(palCppi4Obj->dmaBlock[dmaNumPvt].tdFQueue, cppi4TdBD, (sizeof(Cppi4TeardownDesc) - 24)/4, 0);

    return (PAL_SOK);
}

PAL_Result PAL_cppi4EmbDescRecycle(PAL_Handle hnd, Ptr vDesc, Ptr physDesc)
{
    Uint32 descType;

    PAL_CPPI4_CACHE_INVALIDATE(vDesc, sizeof(Cppi4EmbdDesc));

    descType = (*(Uint32 *)vDesc & CPPI41_EM_DESCINFO_DTYPE_MASK) >> CPPI41_EM_DESCINFO_DTYPE_SHIFT;

    if (CPPI41_EM_DESCINFO_DTYPE_EMB == descType)
    {
        Cppi4EmbdDesc*      eDesc = (Cppi4EmbdDesc *)vDesc;
        Cppi4Queue          retQueue;
        PAL_Cppi4QueueHnd   retQueueHnd;

        if (eDesc->pktInfo & CPPI41_EM_PKTINFO_RETPOLICY_MASK)
        {
            Uint32 curSlot = 0;
            Uint32 eopSlot = (CPPI41_EM_PKTINFO_EOPIDX_MASK & eDesc->pktInfo) >> CPPI41_EM_PKTINFO_EOPIDX_SHIFT;

            while (curSlot <= eopSlot)
            {
                if (eDesc->Buf[curSlot].BufInfo & CPPI41_EM_BUF_VALID_MASK)
                {
                    Cppi4BufPool bPool;
                    bPool.bPool = (eDesc->Buf[curSlot].BufInfo & CPPI41_EM_BUF_POOL_MASK) >> CPPI41_EM_BUF_POOL_SHIFT;
                    bPool.bMgr  = (eDesc->Buf[curSlot].BufInfo & CPPI41_EM_BUF_MGR_MASK)  >> CPPI41_EM_BUF_MGR_SHIFT;

                    PAL_cppi4BufDecRefCnt(hnd, bPool, (Ptr)eDesc->Buf[curSlot].BufPtr);

                    eDesc->Buf[curSlot].BufInfo &= ~CPPI41_EM_BUF_VALID_MASK;
                }
                curSlot++;
            }
        }

        retQueue.qMgr =     (eDesc->pktInfo & CPPI41_EM_PKTINFO_RETQMGR_MASK)   >> CPPI41_EM_PKTINFO_RETQMGR_SHIFT;
        retQueue.qNum =     (eDesc->pktInfo & CPPI41_EM_PKTINFO_RETQ_MASK)      >> CPPI41_EM_PKTINFO_RETQ_SHIFT;

        retQueueHnd = PAL_cppi4QueueOpenNoReset(hnd, retQueue);

        PAL_CPPI4_CACHE_WRITEBACK(vDesc, sizeof(Cppi4EmbdDesc));

        PAL_cppi4QueuePush(retQueueHnd, physDesc, PAL_CPPI4_DESCSIZE_2_QMGRSIZE(sizeof(Cppi4EmbdDesc)), 0);

        PAL_cppi4QueueClose(hnd, retQueueHnd);

        return (PAL_SOK);
    }

    return (CPPI4_ERR_INVALID_DESC_TYPE);
}


#if PUMA7_OR_NEWER_SOC_TYPE

PAL_Result PAL_cppi4RalEnable(PAL_Handle hnd, Uint32 dmaNum, Uint32 qsmonBitmap)
{
    Cppi4PALObj *palCppi4Obj = (Cppi4PALObj *)hnd;

    if (palCppi4Obj->initCfg->dmaBlock[dmaNum].ralCtrlBase == NULL)
    {
        return -1;
    }

    palCppi4Obj->initCfg->dmaBlock[dmaNum].ralCtrlBase->Control_Reg = qsmonBitmap;
    dbgPrint("PAL_cppi4RalEnable: @0x%08X <-- 0x%x!!!\n", &palCppi4Obj->initCfg->dmaBlock[dmaNum].ralCtrlBase->Control_Reg, qsmonBitmap);

    return (PAL_SOK);
}

PAL_Result PAL_cppi4RalDisable(PAL_Handle hnd, Uint32 dmaNum)
{
    Cppi4PALObj *palCppi4Obj = (Cppi4PALObj *)hnd;

    if (palCppi4Obj->initCfg->dmaBlock[dmaNum].ralCtrlBase == NULL)
    {
        return -1;
    }

    palCppi4Obj->initCfg->dmaBlock[dmaNum].ralCtrlBase->Control_Reg = 1 << PAL_CPPI_RAL_BYPASS_OFFSET;
    return (PAL_SOK);
}

PAL_Result PAL_cppi4QSMonEnable(PAL_Handle hnd, PAL_CPPI_PP_QSMON_MGRs_e qsmonMgr, Uint32 thread, Cppi4Queue queue, Uint32 threshold, Bool isDown)
{
    Cppi4PALObj *palCppi4Obj = (Cppi4PALObj *)hnd;

    if(palCppi4Obj->initCfg->qsmon[qsmonMgr] == NULL)
    {
        return -1;
    }

    palCppi4Obj->initCfg->qsmon[qsmonMgr]->Cmds[thread].Register_A = threshold;
    palCppi4Obj->initCfg->qsmon[qsmonMgr]->Cmds[thread].Register_B = queue.qNum | (queue.qMgr << QSMON_CONFIG_MONITORED_QUEUE_MGR_SHIFT) | (isDown << QSMON_CONFIG_MONITORED_QUEUE_DIR_SHIFT);
    palCppi4Obj->initCfg->qsmon[qsmonMgr]->Cmds[thread].Register_B |= (1 << QSMON_CONFIG_MONITORED_QUEUE_EN_SHIFT);

    dbgPrint("qsmonMgr: %d, thread: %d, threshold: %d, REG A @0x%p=0x%08X, REG B @0x%p=0x%08X\n",
             qsmonMgr, thread, threshold,
             IO_VIRT2PHY(&palCppi4Obj->initCfg->qsmon[qsmonMgr]->Cmds[thread].Register_A), threshold,
             IO_VIRT2PHY(&palCppi4Obj->initCfg->qsmon[qsmonMgr]->Cmds[thread].Register_B), palCppi4Obj->initCfg->qsmon[qsmonMgr]->Cmds[thread].Register_B);

    return (PAL_SOK);
}

PAL_Result PAL_cppi4QSMonDisable(PAL_Handle hnd, PAL_CPPI_PP_QSMON_MGRs_e qsmonMgr, Uint32 thread)
{
    Cppi4PALObj *palCppi4Obj = (Cppi4PALObj *)hnd;

    if(palCppi4Obj->initCfg->qsmon[qsmonMgr] == NULL)
    {
        return -1;
    }

    palCppi4Obj->initCfg->qsmon[qsmonMgr]->Cmds[thread].Register_B = 0;

    return (PAL_SOK);
}

PAL_Result PAL_cppi4QSMonGetQdepthWatermark(PAL_Handle hnd, PAL_CPPI_PP_QSMON_MGRs_e qsmonMgr, Uint32 thread, CSL_Reg32* watermark)
{
    Cppi4PALObj *palCppi4Obj = (Cppi4PALObj *)hnd;

    if((palCppi4Obj->initCfg->qsmon[qsmonMgr] == NULL) || (watermark == NULL))
    {
        return -1;
    }

    palCppi4Obj->initCfg->qsmon[qsmonMgr]->Depth[thread].Stats_Qdepth_Watermark;

    return (PAL_SOK);
}

PAL_Result PAL_cppi4QSMonSetQdepthCnt(PAL_Handle hnd, PAL_CPPI_PP_QSMON_MGRs_e qsmonMgr, Uint32 thread, CSL_Reg32 count)
{
    Cppi4PALObj *palCppi4Obj = (Cppi4PALObj *)hnd;

    if(palCppi4Obj->initCfg->qsmon[qsmonMgr] == NULL)
    {
        return -1;
    }

    palCppi4Obj->initCfg->qsmon[qsmonMgr]->Depth[thread].Stats_Qdepth_Cnt = count;

    return (PAL_SOK);
}


#endif

/* =================================Power management==================================== */
/**
 *  \brief PAL CPPI4.1 Get the LPM threshold and maximal number
 *         of resources of buffer pool
 *
 *  Get the LPM threshold and maximal numbe of resources of
 *  buffer pool.
 *
 *  @param  bufPoolID     [IN]      buffer pool ID
 *  @param  deltaP        [OUT]     The LPM threshold
 *  @param  resourcesNumP [OUT]     The resources number
 *
 *  @return LPM threshold value
 */
static void __PAL_cppi4_get_buf_pool_resources_data_for_pm(Uint32 bufPoolID, Uint32* deltaP, Uint32* resourcesNumP)
{
#if PUMA7_OR_NEWER_SOC_TYPE
    avalanche_cppi_get_buffer_count(bufPoolID, resourcesNumP);
    switch (bufPoolID)
    {
    case PAL_CPPI_PP_SHARED_RX_LOW_1KB_BUFFER_POOL:
        *deltaP        = PAL_CPPI_PP_SHARED_RX_LOW_1KB_BUFFER_POOL_LPM_DELTA;
        break;
    case PAL_CPPI_PP_SHARED_RX_LOW_2KB_BUFFER_POOL:
        *deltaP        = PAL_CPPI_PP_SHARED_RX_LOW_2KB_BUFFER_POOL_LPM_DELTA;
        break;
    case PAL_CPPI_PP_SHARED_RX_HIGH_BUFFER_POOL:
        *deltaP        = PAL_CPPI_PP_SHARED_RX_HIGH_BUFFER_POOL_LPM_DELTA;
        break;
    case PAL_CPPI_PP_PACKET_RAM_512B_BUFFER_POOL:
        *deltaP        = PAL_CPPI_PP_PACKET_RAM_512B_BUFFER_POOL_LPM_DELTA;
        break;
    default:
        *deltaP = NOT_MONITORED;
    }
#else
    switch (bufPoolID)
    {
    case PAL_CPPI_PP_SHARED_RX_LOW_1KB_BUFFER_POOL:
        *deltaP        = PAL_CPPI_PP_SHARED_RX_LOW_1KB_BUFFER_POOL_LPM_DELTA;
        *resourcesNumP = PAL_CPPI_PP_SHARED_RX_LOW_1KB_BUFFER_COUNT;
        break;
    case PAL_CPPI_PP_SHARED_RX_LOW_2KB_BUFFER_POOL:
        *deltaP        = PAL_CPPI_PP_SHARED_RX_LOW_2KB_BUFFER_POOL_LPM_DELTA;
        *resourcesNumP = PAL_CPPI_PP_SHARED_RX_LOW_2KB_BUFFER_COUNT;
        break;
    case PAL_CPPI_PP_SHARED_RX_HIGH_BUFFER_POOL:
        *deltaP        = PAL_CPPI_PP_SHARED_RX_HIGH_BUFFER_POOL_LPM_DELTA;
        *resourcesNumP = PAL_CPPI_PP_SHARED_RX_HIGH_BUFFER_COUNT;
        break;
    case PAL_CPPI_PP_PACKET_RAM_512B_BUFFER_POOL:
        *deltaP        = PAL_CPPI_PP_PACKET_RAM_512B_BUFFER_POOL_LPM_DELTA;
        *resourcesNumP = PAL_CPPI_PP_PACKET_RAM_512B_BUFFER_COUNT;
        break;
    default:
        *deltaP = NOT_MONITORED;
    }
#endif
}

/**
* Fetch power management information and copy to user's memory
*/
int __PAL_cppi4_read_pm_info(struct file *filp, char *resultBuf, size_t count, loff_t *offp)
{
    CSL_QSMon_RegsOvly bsmon = (Ptr) gCppiPpInitCfg.qsmon[PAL_CPPI_PP_BSMON_MGR];
    Uint32                  i, pool, lpmThresholdDelta, maxResourcesNum;
    AVALANCHE_PP_PM_INFO_t  pmInfo;

    if (count < sizeof(pmInfo))
    {
        errPrint ("\nError: Size to copy to user is too small\n");
        return 0;
    }

    pmInfo.ppsStatCntPhyAddr = PP_HAL_PPS_FETCH_ADDR_PHY;
    pmInfo.validEntriesCount = 0;
    for (i = 0; i < (sizeof(gBsmon) / sizeof(qsmonRal_t)); i++)
    {
        if (gBsmon[i].qsmonChEn)
        {
            pool = (bsmon->Cmds[gBsmon[i].qsmonThread].Register_B) & QSMON_CONFIG_MONITORED_QUEUE_NUM_MASK;
            __PAL_cppi4_get_buf_pool_resources_data_for_pm(pool, &lpmThresholdDelta, &maxResourcesNum);
            if (lpmThresholdDelta != NOT_MONITORED)
            {
                pmInfo.PMobjectsInfos[pmInfo.validEntriesCount].currResourcesNumRegAddr = IO_VIRT2PHY (&(bsmon->Depth[gBsmon[i].qsmonThread].Stats_Qdepth_Cnt));
                if (maxResourcesNum >= lpmThresholdDelta)
                {
                    pmInfo.PMobjectsInfos[pmInfo.validEntriesCount].lpmThreshold = maxResourcesNum - lpmThresholdDelta;
                    pmInfo.validEntriesCount += 1;
                }
                else
                {
                    errPrint ("\nWARN: Resources for pool %u are less then LPM threshold\n", pool);
                }
            }
        }
    }
    copy_to_user(resultBuf, &pmInfo, count);
    return count;
}

/**
* CPPI Power management interface.
* User needs only read capabilities to fetch PM info.
*/
const struct file_operations PAL_cppi4PM_fops =
{
read:   __PAL_cppi4_read_pm_info
};

/* ================================Power management end================================= */

#if 1


/* ================================================================================ */

#define CHECK_GET_INPUT_NUM(output, params, base)                             \
{                                                                               \
    char * str;                                                                 \
    if (params && (NULL != (str = strsep(&params, sep))))                   \
    {                                                                           \
        char * endPtr;                                                          \
        unsigned int val;                                                       \
                                                                                \
        val = simple_strtol(str, &endPtr, base);                              \
        if (endPtr == str)                                                      \
        {                                                                       \
            printk("Error: Parameter is an invalid numeric input\n");           \
            return -EINVAL;                                                     \
        }                                                                       \
        else if ((*endPtr != '\0') && (*endPtr != '\n'))                        \
        {                                                                       \
            printk("Error: Extra characters (%s) on input\n", endPtr);          \
            return -EINVAL;                                                     \
        }                                                                       \
                                                                                \
        output = val;                                                           \
    }                                                                           \
}

/* ================================================================================ */
static ssize_t __PAL_cppi4_dbg_wr_proc(struct file *file, const char __user *buffer, size_t count, loff_t *f_pos)
{
    char        local_buffer[256];
    char *      params = &local_buffer[0];
    char *      token;
    char *      sep = ", \t.";
    pal_cppi_dbg_cfg_t  *   dbg_cfg = PDE_DATA(file->f_inode);

    if (count >= sizeof(local_buffer))
    {
        errPrint("Buffer Overflow\n");
        return -EINVAL;
    }

    if (copy_from_user(local_buffer, buffer, count))
    {
        errPrint("Failed to read from user space\n");
        return -EFAULT;
    }

    local_buffer[count] = 0;

    if (NULL != (token = strsep(&params, sep)))
    {
        /* ==================================================================================== */
        /*                                                                                      */
        /*                               POP                                                    */
        /*                                                                                      */
        /* ==================================================================================== */
        if (!strnicmp("pop", token, strlen("pop")))
        {
            Cppi4Queue          queue;
            PAL_Cppi4BD         bd;
            PAL_Cppi4QueueHnd   qHnd;

            CHECK_GET_INPUT_NUM(queue.qMgr , params, 0);
            CHECK_GET_INPUT_NUM(queue.qNum , params, 0);
            qHnd = PAL_cppi4QueueOpenNoReset (dbg_cfg->handle, queue);
            if (NULL == qHnd) 
                return -EFAULT;
            bd = PAL_cppi4QueuePop(qHnd);

			PRINT_OUTPUT("Q[ G%d.%-5d ] ==> BD:[ %08X ]\n", queue.qMgr, queue.qNum, bd);

            if (qHnd)
            {
                PAL_cppi4QueueClose(dbg_cfg->handle, qHnd);
            }
        }
        else
        /* ==================================================================================== */
        /*                                                                                      */
        /*                               PUSH                                                   */
        /*                                                                                      */
        /* ==================================================================================== */
        if (!strnicmp("push", token, strlen("push")))
        {
            Cppi4Queue          queue;
            PAL_Cppi4BD         bd;
            PAL_Cppi4QueueHnd   qHnd;
            unsigned int        pktLen = 0;

            CHECK_GET_INPUT_NUM(queue.qMgr , params, 0);
            CHECK_GET_INPUT_NUM(queue.qNum , params, 0);
            CHECK_GET_INPUT_NUM(bd ,         params, 16);
            CHECK_GET_INPUT_NUM(pktLen ,     params, 0);
            qHnd = PAL_cppi4QueueOpenNoReset(dbg_cfg->handle, queue);
            if (NULL == qHnd)
                 return -EFAULT;

            PAL_cppi4QueuePush(qHnd, bd, 0, pktLen);
			PRINT_OUTPUT("Q[ G%d.%-5d ] <== BD:[ %08X ] len=%d\n",queue.qMgr, queue.qNum, bd, pktLen);

            if (qHnd)
            {
                PAL_cppi4QueueClose(dbg_cfg->handle, qHnd);
            }
        }
        else
        /* ==================================================================================== */
        /*                                                                                      */
        /*                               Buffer POP                                             */
        /*                                                                                      */
        /* ==================================================================================== */
        if (!strnicmp("bpop", token, strlen("bpop")))
        {
            void *bp = NULL;
            Cppi4BufPool buff_pool;

            buff_pool.bMgr = 0;
            CHECK_GET_INPUT_NUM(buff_pool.bPool , params, 0);

            if (buff_pool.bPool >= PAL_CPPI41_BMGR_MAX_POOLS) {
				PRINT_OUTPUT("ERROR: Invalid buffer pool %d, max buffer pool is %d", buff_pool.bPool, PAL_CPPI41_BMGR_MAX_POOLS-1);
                return count;
            }

            bp = PAL_cppi4BufPopBuf(dbg_cfg->handle, buff_pool);
			PRINT_OUTPUT("Pool[%d] ==> BP:[%08p]\n", buff_pool.bPool, bp);
        }
        else
        /* ==================================================================================== */
        /*                                                                                      */
        /*                               Buffer PUSH                                            */
        /*                                                                                      */
        /* ==================================================================================== */
        if (!strnicmp("bpush", token, strlen("bpush")))
        {
            void *bp = NULL;
            Cppi4BufPool buff_pool;

            buff_pool.bMgr = 0;
            CHECK_GET_INPUT_NUM(buff_pool.bPool , params, 0);
            CHECK_GET_INPUT_NUM(bp              , params, 16);

            if (buff_pool.bPool >= PAL_CPPI41_BMGR_MAX_POOLS) {
				PRINT_OUTPUT("ERROR: Invalid buffer pool %d, max buffer pool is %d", buff_pool.bPool, PAL_CPPI41_BMGR_MAX_POOLS-1);
                return count;
            }

            PAL_cppi4BufDecRefCnt(dbg_cfg->handle, buff_pool, bp);
			PRINT_OUTPUT("Pool[%d] <== BP:[%08p]\n", buff_pool.bPool, bp);
        }
        else
        /* ==================================================================================== */
        /*                                                                                      */
        /*                          Make Packet                                                 */
        /*                                                                                      */
        /* ==================================================================================== */
        if (!strnicmp("mkpkt", token, strlen("mkpkt")))
        {
            Cppi4PALObj *palCppi4Obj = (Cppi4PALObj *) dbg_cfg->handle;
            Cppi4InitCfg* initCfg = palCppi4Obj->initCfg;

            PAL_Cppi4BD         bd;
            Cppi4Desc *         vbd;
            PAL_Cppi4QueueHnd   qHnd;
            Uint32              pktLen = 0;
            Uint32              ps = 0;
            Uint32              verbose = 0;

            CHECK_GET_INPUT_NUM(pktLen ,   params, 0);
            CHECK_GET_INPUT_NUM(ps ,       params, 16);
            CHECK_GET_INPUT_NUM(verbose ,  params, 0);

            if ((dbg_cfg->allocQueue.qNum  != -1) &&
                 (dbg_cfg->allocBuf.bPool   != -1) &&
                 (dbg_cfg->outputQueue.qNum != -1))
            {
                qHnd = PAL_cppi4QueueOpenNoReset(dbg_cfg->handle, dbg_cfg->allocQueue);
                if (NULL == qHnd)
                    return -EFAULT;
                bd = PAL_cppi4QueuePop(qHnd);
                if (qHnd)
                {
                    PAL_cppi4QueueClose(dbg_cfg->handle, qHnd);
                }

                if (bd)
                {
                    Bool            bdOnChip = False;
                    Ptr             startP;
                    Ptr             endP;
                    int             i;
                    Cppi4EmbdBuf *  Buff;

                    for (i=0; i<PAL_CPPI41_MAX_DESC_REGIONS; i++)
                    {
                        startP = initCfg->queueMgrInfo[ dbg_cfg->allocQueue.qMgr ].descRegion[ i ].base;
                        endP = startP + initCfg->queueMgrInfo[ dbg_cfg->allocQueue.qMgr ].descRegion[ i ].numDesc *
                                        initCfg->queueMgrInfo[ dbg_cfg->allocQueue.qMgr ].descRegion[ i ].szDesc;

                        if (initCfg->queueMgrInfo[ dbg_cfg->allocQueue.qMgr ].descRegion[ i ].isOnChip)
                        {
                            vbd = (Cppi4Desc *)IO_PHY2VIRT(bd);

                            if ((startP <= bd) && (bd < endP))
                            {
                                bdOnChip = True;
                                break;
                            }
                        }
                        else
                        {
                            vbd = (Cppi4Desc *)PAL_CPPI4_PHYS_2_VIRT(bd);

                            if ((startP <= vbd) && (vbd < endP))
                            {
                                break;
                            }
                        }
                    }

                    if (PAL_CPPI41_MAX_DESC_REGIONS == i)
                    {
						PRINT_OUTPUT("Error : Unable to identify BD location 0x%08X\n", bd);
                        return -EINVAL;
                    }

                    vbd->descInfo &= ~CPPI41_EM_DESCINFO_PKTLEN_MASK;
                    vbd->descInfo |= pktLen;
                    Buff = &vbd->Buf;

                    if (pktLen)
                    {
                        Buff->BufPtr     = PAL_cppi4BufPopBuf(dbg_cfg->handle, dbg_cfg->allocBuf);
                        if(Buff->BufPtr == NULL)
                        {
							PRINT_OUTPUT("Buff->BufPtr is Out of Bounds\n");
                            return -EINVAL;
                        }
                        Buff->BufInfo    =
                            (CPPI41_EM_BUF_VALID_MASK)              |
                            (CPPI41_EM_BUF_BUFFLEN_MASK & pktLen)   |
                            (dbg_cfg->allocBuf.bMgr  << CPPI41_EM_BUF_MGR_SHIFT) |
                            (dbg_cfg->allocBuf.bPool << CPPI41_EM_BUF_POOL_SHIFT);
                    }

                    {
                        Uint32 *    psPtr = (Uint32 *)vbd;
                        psPtr += sizeof(*vbd)/sizeof(Uint32);
                        *psPtr = ps;
                    }

                    if ((Buff->BufPtr) && (dbg_cfg->dataLen))
                    {
                        Uint8 * vBuf = PAL_CPPI4_PHYS_2_VIRT(Buff->BufPtr);

                        memcpy(vBuf, dbg_cfg->data, min(dbg_cfg->dataLen,pktLen));

                        PAL_CPPI4_CACHE_WRITEBACK(vBuf, min(dbg_cfg->dataLen,pktLen));
                    }
#if DEBUG_PRINT
                    {
                        Uint32 *    t=(Uint32 *) vbd;

                        if (verbose)
                        {
							PRINT_OUTPUT(" Allocated Descriptor: [ %08X ] [%08X]\n"
                                   "\t %08X"
                                   "\t %08X"
                                   "\t %08X"
                                   "\t %08X\n"
                                   "\t %08X"
                                   "\t %08X"
                                   "\t %08X"
                                   "\t %08X\n\n",
                                   bd, verbose,
                                   t[0],t[1],t[2],t[3],
                                   t[4],t[5],t[6],t[7]);
                        }
                    }
#endif

                    if (False == bdOnChip)
                    {
                        PAL_CPPI4_CACHE_WRITEBACK(vbd, sizeof(*vbd));
                    }
                    qHnd = PAL_cppi4QueueOpenNoReset(dbg_cfg->handle, dbg_cfg->outputQueue);
                    if (NULL == qHnd)
                        return -EFAULT;

                    PAL_cppi4QueuePush(qHnd, bd, 0, pktLen);
                    if (qHnd)
                    {
                        PAL_cppi4QueueClose(dbg_cfg->handle, qHnd);
                    }
                }
                else
                {
					PRINT_OUTPUT("Warning: No Descriptors available ...\n");
                }
            }
            else
            {
				PRINT_OUTPUT("Warning: Invalid configuration ...\n");
            }
        }
        else
        /* ==================================================================================== */
        /*                                                                                      */
        /*                               Dump Queue                                             */
        /*                                                                                      */
        /* ==================================================================================== */
        if (!strnicmp("dumpq", token, strlen("dumpq")))
        {
            Cppi4PALObj *palCppi4Obj = (Cppi4PALObj *) dbg_cfg->handle;
            Cppi4InitCfg* initCfg = palCppi4Obj->initCfg;
            Uint32 *            vbd;
            Cppi4Queue          queue;
            PAL_Cppi4BD         bd = 1;
            PAL_Cppi4QueueHnd   qHnd;

            CHECK_GET_INPUT_NUM(queue.qMgr , params, 0);
            CHECK_GET_INPUT_NUM(queue.qNum , params, 0);

            qHnd = PAL_cppi4QueueOpenNoReset(dbg_cfg->handle, queue);

            while (qHnd && bd)
            {
                bd = PAL_cppi4QueuePop(qHnd);
                if (bd)
                {
                    Bool    bdOnChip = False;
                    Ptr     startP;
                    Ptr     endP;
                    int     i;

                    for (i=0; i<PAL_CPPI41_MAX_DESC_REGIONS; i++)
                    {
                        startP = initCfg->queueMgrInfo[ dbg_cfg->allocQueue.qMgr ].descRegion[ i ].base;
                        endP = startP + initCfg->queueMgrInfo[ dbg_cfg->allocQueue.qMgr ].descRegion[ i ].numDesc *
                                        initCfg->queueMgrInfo[ dbg_cfg->allocQueue.qMgr ].descRegion[ i ].szDesc;

                        if (initCfg->queueMgrInfo[ dbg_cfg->allocQueue.qMgr ].descRegion[ i ].isOnChip)
                        {
                            vbd = (Cppi4Desc *)IO_PHY2VIRT(bd);

                            if ((startP <= bd) && (bd < endP))
                            {
                                bdOnChip = True;
                                break;
                            }
                        }
                        else
                        {
                            vbd = (Cppi4Desc *)PAL_CPPI4_PHYS_2_VIRT(bd);

                            if ((startP <= vbd) && (vbd < endP))
                            {
                                break;
                            }
                        }
                    }

                    if (PAL_CPPI41_MAX_DESC_REGIONS == i)
                    {
                        PAL_cppi4QueueClose(dbg_cfg->handle, qHnd);

						PRINT_OUTPUT("Error : Unable to identify BD location 0x%08X\n", bd);
                        return -EINVAL;
                    }

                    if (False == bdOnChip)
                    {
                        PAL_CPPI4_CACHE_INVALIDATE(vbd,32);
                    }

					PRINT_OUTPUT(" Descriptor: [ %08X ]\n"
                           "\t %08X"
                           "\t %08X"
                           "\t %08X\n"
                           "\t %08X"
                           "\t %08X\n"
                           "\t %08X"
                           "\t %08X\n"
                           "\t %08X\n\n",
                           bd,
                           vbd[0],vbd[1],vbd[2],vbd[3],
                           vbd[4],vbd[5],vbd[6],vbd[7]);

                }
            }

            if (qHnd)
            {
                PAL_cppi4QueueClose(dbg_cfg->handle, qHnd);
            }

        }
        else
        {
            errPrint("%s is unsupported command\n", token);
            return count;
        }
    }

    return count;
}
/* ================================================================================ */
static ssize_t __PAL_cppi4_dumpDbg(struct seq_file *m, void *v)
{
	seq_printf(m, "%s\n", cppi_output_read_option_str); 
	cppi_output_read_option_str[0]='\0'; 

    seq_printf(m, " The usage is:\n"
           "\tpop     <q   mgr> <q    num>\n"
           "\tpush    <buf mgr> <pool num> <desc> [<length>]\n"
           "\tmkpkt   [<length>] [<protocol specific word (hex)>]\n"
           "\tdumpq   <q   mgr> <q    num>\n"
          );
    return 0;
}

static int  __PAL_cppi4_dbg_open_proc(struct inode *inode, struct file *file)
{
    return single_open(file, __PAL_cppi4_dumpDbg, PDE_DATA(inode));
}
/* ================================================================================ */


/* ================================================================================ */
/*                                                                                  */
/*                                                                                  */
/*                                 DEBUG Config                                     */
/*                                                                                  */
/*                                                                                  */
/* ================================================================================ */
static ssize_t __PAL_cppi4_dumpDbgCfg(struct seq_file *m, void *v)
{
    int i;
    pal_cppi_dbg_cfg_t * dbg_cfg = (pal_cppi_dbg_cfg_t  *)m->private;
    seq_printf(m, "=============================\n");
    seq_printf(m, " Alloc  ");
    if (dbg_cfg->allocQueue.qNum != -1)
    {
        seq_printf(m, "Q[ G%d.%-5d ]\n",dbg_cfg->allocQueue.qMgr, dbg_cfg->allocQueue.qNum);
    }
    else
    {
        seq_printf(m, "Q Invalid\n");
    }
    seq_printf(m, " Alloc  ");
    if (dbg_cfg->allocBuf.bPool != -1)
    {
        seq_printf(m, "B[ G%d.%-5d ]\n",dbg_cfg->allocBuf.bMgr, dbg_cfg->allocBuf.bPool);
    }
    else
    {
        seq_printf(m, "B Invalid\n");
    }
    seq_printf(m, " Output ");
    if (dbg_cfg->outputQueue.qNum != -1)
    {
        seq_printf(m, "Q[ G%d.%-5d ]\n",dbg_cfg->outputQueue.qMgr, dbg_cfg->outputQueue.qNum);
    }
    else
    {
        seq_printf(m, "Q Invalid\n");
    }

    seq_printf(m, " Buffer (len=%d):", dbg_cfg->dataLen);
    for (i=0; i<dbg_cfg->dataLen; i++)
    {
        if (0 == i%16)
        {
            seq_printf(m, "\n");
        }
        seq_printf(m, "%02X ",dbg_cfg->data[i]);
    }
    seq_printf(m, "\n");

    seq_printf(m, "=============================\n");
    return 0;
}

static int  __PAL_cppi4_dbgCfg_open_proc(struct inode *inode, struct file *file)
{
    return single_open(file, __PAL_cppi4_dumpDbgCfg, PDE_DATA(inode));
}
/* ================================================================================ */

static ssize_t __PAL_cppi4_dbgCfg_wr_proc(struct file *file, const char __user *buffer, size_t count, loff_t *f_pos)
{
    char        local_buffer[256];
    char *      params = &local_buffer[0];
    char *      token;
    char *      sep = ", \t.";
    pal_cppi_dbg_cfg_t  *   dbg_cfg = PDE_DATA(file->f_inode);

    if (count >= sizeof(local_buffer))
    {
        errPrint("Buffer Overflow\n");
        return -EINVAL;
    }

    if (copy_from_user(local_buffer, buffer, count))
    {
        errPrint("Failed to read from user space\n");
        return -EFAULT;
    }

    local_buffer[count] = 0;

    if (NULL != (token = strsep(&params, sep)))
    {
        if (!strnicmp("allocq", token, strlen("allocq")))
        {
            CHECK_GET_INPUT_NUM(dbg_cfg->allocQueue.qMgr , params, 0);
            CHECK_GET_INPUT_NUM(dbg_cfg->allocQueue.qNum , params, 0);
        }
        else
        if (!strnicmp("outputq", token, strlen("outputq")))
        {
            CHECK_GET_INPUT_NUM(dbg_cfg->outputQueue.qMgr , params, 0);
            CHECK_GET_INPUT_NUM(dbg_cfg->outputQueue.qNum , params, 0);
        }
        else
        if (!strnicmp("allocb", token, strlen("allocb")))
        {
            CHECK_GET_INPUT_NUM(dbg_cfg->allocBuf.bMgr  , params, 0);
            CHECK_GET_INPUT_NUM(dbg_cfg->allocBuf.bPool , params, 0);
        }
        else
        if (!strnicmp("databuf", token, strlen("databuf")))
        {
            Uint32 idx;

            CHECK_GET_INPUT_NUM(dbg_cfg->dataLen  , params, 0);

            if (PAL_CPPI41_DEBUG_BUFF_MAXLEN < dbg_cfg->dataLen)
            {
                dbg_cfg->dataLen = 0;
            }

            for (idx = 0; idx < dbg_cfg->dataLen; idx++)
            {
                Uint32  tmp;

                CHECK_GET_INPUT_NUM(tmp, params, 16);
                if (0x100 > tmp)
                {
                    dbg_cfg->data[ idx ] = (Uint8)tmp;
                }
                else
                {
                    printk(" Error... Data format error at %d entry\n", idx);

                }
            }
        }
        else
        {
            printk(" The usage is:\n"
                   "\tallocq    <q   mgr> <q    num>\n"
                   "\tallocb    <buf mgr> <pool num>\n"
                   "\toutputq   <q   mgr> <q    num>\n"
                   "\tdatabuf   <len> <hex byte> .. <hex byte>\n"
                  );
        }
    }

    return count;
}

/* ================================================================================ */

const struct file_operations      PAL_cppi4dbg_fops =
{
    .open       = __PAL_cppi4_dbg_open_proc,
    .write      = __PAL_cppi4_dbg_wr_proc,
    .read       = seq_read,
    .llseek     = seq_lseek,
    .release    = single_release,
};

/* ================================================================================ */

const struct file_operations      PAL_cppi4dbgCfg_fops =
{
    .open       = __PAL_cppi4_dbgCfg_open_proc,
    .write      = __PAL_cppi4_dbgCfg_wr_proc,
    .read       = seq_read,
    .llseek     = seq_lseek,
    .release    = single_release,
};

/* ================================================================================ */
#endif


