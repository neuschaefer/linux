/*
 *
 * pal_cppi41pvt.h
 * Description:
 *
 *

  This file is provided under a dual BSD/GPLv2 license.  When using or 
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2008-2014 Intel Corporation.

  This program is free software; you can redistribute it and/or modify 
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but 
  WITHOUT ANY WARRANTY; without even the implied warranty of 
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
  General Public License for more details.

  You should have received a copy of the GNU General Public License 
  along with this program; if not, write to the Free Software 
  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
  The full GNU General Public License is included in this distribution 
  in the file called LICENSE.GPL.


  Contact Information:
  Intel Corporation
  2200 Mission College Blvd.
  Santa Clara, CA  97052

  BSD LICENSE 

  Copyright(c) 2008-2014 Intel Corporation. All rights reserved.

  Redistribution and use in source and binary forms, with or without 
  modification, are permitted provided that the following conditions 
  are met:

    * Redistributions of source code must retain the above copyright 
      notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright 
      notice, this list of conditions and the following disclaimer in 
      the documentation and/or other materials provided with the 
      distribution.

    * Neither the name of Intel Corporation nor the names of its 
      contributors may be used to endorse or promote products derived 
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/** \file   pal_cppi41pvt.h
    \brief  CPPI4 PAL internal header file

    This file contains data structures and interfaces internal to CPPI4
    specific PAL implementation.This file is compliant to the PSP Framework 1.0
    definitions and prototypes.

    @author     Sekhar Nori
    @author     Greg Guyotte
 */

#ifndef __PAL_CPPI4_PVT_H__
#define __PAL_CPPI4_PVT_H__

/* This file will be included ONLY by CPPI4 PAL "implementation" files and hence
   must define CPPI4_PAL macro to get the right definition of Cppi4PALObj */
#define CPPI4_PAL

/**
 * \defgroup CPPI4_PAL_Internal CPPI4 PAL Internal
 *
 *  CPPI4 PAL Layer Internal Data Structures/Functions
 */
/*@{*/

/* Required for hiding internal object name behind the public name */
#include "pal_cppi41.h"          /* PAL CPPI4 Interface */

/*
 *  \brief PAL CPPI4 Debug flags
 *
 *  The makefile must #define CPPI4_PAL_DEBUG in order to enable debug.
 *  IMPORTANT NOTE: The debug flags need to be enabled carefully as it could
 *  flood the console/sink point of the debug traces and also affect the
 *  functionality of the overall system
 */
#define CPPI4_DEBUG_FUNCTION_ENTRY          (0x1 << 1)
#define CPPI4_DEBUG_FUNCTION_EXIT           (0x1 << 2)
#define CPPI4_DEBUG_BUSY_FUNCTION_ENTRY     (0x1 << 3)
#define CPPI4_DEBUG_BUSY_FUNCTION_EXIT      (0x1 << 4)
#define CPPI4_DEBUG_TEARDOWN                (0x1 << 5)

#ifdef CPPI4_PAL_DEBUG
#define CPPI4_PAL_LOGERR(format, args...)         printf(format, ##args);
#define CPPI4_PAL_LOGMSG(flag, format, args... )  { if (flag & Cppi4PALDebug) printf(format, ## args); }
#else /*
 */
#define CPPI4_PAL_LOGERR(format, args...)
#define CPPI4_PAL_LOGMSG(flag, format, args... )
#endif  /*
 */

struct Cppi4PALObj_t;

/**
 *  \brief CPPI4 PAL Transmit Channel object 
 *
 *  CPPI4 PAL layer Object - encapsulates all bookeeping and data structures for
 *  CPPI4 PAL
 */
typedef struct PAL_Cppi4TxChObj_t
{
    Cppi4TxChInitCfg initCfg;           /**< Reference to initial configuration for this channel */
    
    struct Cppi4PALObj_t *palCppi4Obj;  /**< Back reference to the CPPI 4.1 structure */

    Bool isEnabled;                     /**< Book Keeping */
    Bool txInterruptDisable;            /**< Tx interrupt disabled on this channel */
    Uint32 txGlobalConfig;              /**< Tx global configuration backed-up value */
    Uint32 curPage;                     /**< Current accumulator page. Valid for Host channels */
    

    /* TODO: Add more book keeping variable as deemed suitable during implementation */

#ifdef CPPI4_PAL_GETSTATS
    Uint32 txPopCount;                  /**< TX Pop Descriptor count */
#endif 
    
} PAL_Cppi4TxChObj;

/**
 *  \brief CPPI4 PAL Receive Channel object 
 *
 *  CPPI4 PAL layer Object - encapsulates all bookeeping and data structures for
 *  CPPI4 PAL
 */
typedef struct PAL_Cppi4RxChObj_t
{
    Cppi4RxChInitCfg initCfg;           /**< Reference to initial configuration for this channel */
    
    struct Cppi4PALObj_t *palCppi4Obj;  /**< Back reference to the CPPI 4.1 structure */
    
    Bool isEnabled;                     /**< Book Keeping */
    Bool rxInterruptDisable;            /**< Rx interrupt disabled on this channel */
    Uint32 rxGlobalConfig;              /**< Rx global configuration backed-up value */
    Uint32 curPage;                     /**< Current accumulator page. Valid for Host channels */    

    /* TODO: Add more book keeping variable as deemed suitable during implementation */

#ifdef CPPI4_PAL_GETSTATS
    Uint32 rxPopCount[CPPI4_NUM_RX_PRI_LEVELS];      /**< TX Pop Descriptor count */
    Uint32 rxPushCount;            /**< Tx Push Descriptor count */
#endif  
    
} PAL_Cppi4RxChObj;

/**
 *  \brief CPPI4 PAL Accumulator Channel object 
 *
 *  CPPI4 PAL layer Object - encapsulates all bookeeping and data structures for
 *  CPPI4 PAL
 */
typedef struct PAL_Cppi4AccChObj_t
{    
    struct Cppi4PALObj_t *palCppi4Obj;  /**< Back reference to the CPPI 4.1 structure */
    Uint32 curPage;                     /**< Current accumulator page. */
    Cppi4AccumulatorCfg initCfg;         /**< The accumulator channel init configuration */    
        
} PAL_Cppi4AccChObj;

/**
 *  \brief CPPI4 PAL Queue object 
 *
 *  CPPI4 PAL layer Object - encapsulates all bookeeping and data structures for
 *  CPPI4 PAL
 */
typedef struct PAL_Cppi4QueueObj_t
{    
    struct Cppi4PALObj_t*   palCppi4Obj;        /**< Back reference to the CPPI 4.1 structure */
    Cppi4Queue              queue;              /**< The queue this object represents */
    CSL_Queue_Mgmt_Regs*    baseAddress;        /**< The base address of the queue management registers overlay structure */
    CSL_Queue_Mgmt_Regs*    baseAddressProxy;   /**< The base address of the queue management registers overlay structure */    
    Uint32                  queueOpenCount;     /**< Reference count that how many times queue is opened Queue may be opened multiple times */     
    
}PAL_Cppi4QueueObj;

/**
 *  \brief CPPI4 PAL DMA object 
 *
 *  CPPI4 PAL layer Object - encapsulates all bookeeping and data structures for
 *  CPPI4 PAL
 */
typedef struct PAL_Cppi4DMAObj_t
{    
    PAL_Cppi4QueueHnd tdFQueue;  /**< Reference to the free teardown queue handle. */

} PAL_Cppi4DMAObj;

/* Teardown info word bits */
#define CPPI4_TD_DESC_TYPE_SHIFT        27
#define CPPI4_TD_DESC_TYPE_MASK         (0x1F << CPPI4_TD_DESC_TYPE_SHIFT)
#define CPPI4_TD_DESC_TX_RX_SHIFT       16
#define CPPI4_TD_DESC_TX_RX_MASK        (1 << CPPI4_TD_DESC_TX_RX_SHIFT)
#define CPPI4_TD_DESC_DMA_NUM_SHIFT     10
#define CPPI4_TD_DESC_DMA_NUM_MASK      (0x3F << CPPI4_TD_DESC_DMA_NUM_SHIFT)
#define CPPI4_TD_DESC_CHAN_NUM_SHIFT    0
#define CPPI4_TD_DESC_CHAN_NUM_MASK     (0x1F << CPPI4_TD_DESC_CHAN_NUM_SHIFT)

/* Channel Status/Control word */
#define CPPI4_CH_ENABLE_SHIFT          31
#define CPPI4_CH_ENABLE_MASK           (0x1u << CPPI4_CH_ENABLE_SHIFT)


/**
 *  \brief CPPI4 PAL object 
 *
 *  CPPI4 PAL layer Object - encapsulates all bookkeeping and data structures for
 *  CPPI4 PAL
 */
typedef struct Cppi4PALObj_t
{

    Cppi4InitCfg* initCfg;                        /**< Reference to initial CPPI 4.1 configuration */
    Uint32          myDomain;       /**< Back reference to domain object represents  */
    PAL_Cppi4DMAObj dmaBlock[PAL_CPPI41_NUM_DMA_BLOCK];

    Ptr     bufPoolPtr      [PAL_CPPI41_NUM_BUF_MGR][PAL_CPPI41_BMGR_MAX_POOLS];
    Uint32  bufPoolSize     [PAL_CPPI41_NUM_BUF_MGR][PAL_CPPI41_BMGR_MAX_POOLS];
    Uint32  bufSize         [PAL_CPPI41_NUM_BUF_MGR][PAL_CPPI41_BMGR_MAX_POOLS];
    Uint32  numBuf          [PAL_CPPI41_NUM_BUF_MGR][PAL_CPPI41_BMGR_MAX_POOLS];

    Ptr     qMgrDescRegPtr  [PAL_CPPI41_NUM_QUEUE_MGR];
    Uint32  qMgrDescRegSize [PAL_CPPI41_NUM_QUEUE_MGR];
    Uint32  qHnd            [PAL_CPPI41_NUM_QUEUE_MGR][PAL_CPPI41_SR_QMGR_TOTAL_Q_COUNT];
    Uint32  isQueueOpen     [PAL_CPPI41_NUM_QUEUE_MGR][PAL_CPPI41_SR_QMGR_TOTAL_Q_COUNT];


#if 0    
    PAL_Cppi4RxChObj rxCh[CPPI4_MAX_RX_CHANNELS];    /**< RX channel object */
    PAL_Cppi4TxChObj txCh[CPPI4_MAX_TX_CHANNELS];    /**< TX channel object */
#endif

} Cppi4PALObj;


/*@}*/

#endif  /* __PAL_CPPI4_PVT_H__ */




