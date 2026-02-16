/*
 *
 * hw_mbox_drv.c
 * Description:
 * Kernel implementation for HW_MBOX driver.
 *
 *
 * GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2014-2017 Intel Corporation.
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

#define DRV_NAME    "Intel(R) HW_MBOX Controller Driver"
#define DRV_VERSION "0.0.4"

/************************************************************************/
/*     INCLUDES                                                         */
/************************************************************************/

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/interrupt.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/device.h>
#include <linux/cdev.h>     /* cdev utilities */
#include <linux/uaccess.h>  /* copy_(to,from)_user */
#include <linux/slab.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/poll.h>
#include <linux/jiffies.h>      /* jiffies */
#include <linux/kthread.h>      /* Kernel Threads */
#include <linux/delay.h>        /* for usleep_range */

/* Uncomment according to CPU:
   [can't use the HW_MBOX defines as they are defined inside the following file... ]
*/
#include "hw_mbox_defs.h"                 /* HW_MBOX definitions and configuration */
//#include <linux/hw_mbox_defs.h>         /* HW_MBOX definitions and configuration */

#include "hw_mbox_mem_access.h"         /* HW_MBOX Register and Memory Access    */

#ifdef HW_MBOX_SOCKETS_API
#include <linux/errno.h>
#include <net/sock.h>
#include <linux/stat.h>
#include <linux/hashtable.h>

#if defined HW_MBOX_IS_NP_CPU
#include "hw_mbox_socket.h"
#elif defined HW_MBOX_IS_APP_CPU
#include <linux/hw_mbox_socket.h>
#endif
#endif /* HW_MBOX_SOCKETS_API */

#if defined HW_MBOX_IS_NP_CPU
#include "puma7.h"                      /* for irq number */
#include "puma7_npcpu_appcpu_defs.h"
#elif defined HW_MBOX_IS_APP_CPU
#include <linux/netip_subsystem.h>
#endif

#if defined HW_MBOX_IS_NP_CPU
#include "hw_mbox_drv.h"                /* Driver API */
#elif defined HW_MBOX_IS_APP_CPU
#include <linux/hw_mailbox.h>           /* Driver API */
#endif

#ifdef HW_MBOX_IS_APP_CPU
/* APP-CPU gets HW MBOX Interrupts as MSI messages triggered upon rising edge of the HW MBOX
   interrupt line [as sampled by the bridge]. Reading the Interrupt Status Register resets its
   value [all unmasked bits] and de-asserts the interrupt line. If the line is re-asserted
   immediately after, the bridge might not be able to sample again and there will not be
   another MSI message. To handle this issue we read the Interrupt Status Registers in
   the ISR until there are no more new channel interrupts...

   Note: as we are reading the Interrupt Status Registers again in the same ISR if the bridge was
         able to sample the re-assertion there will be an MSI message that will be interpreted
         as a ghost interrupt [reading will get no set unmasked bits].
*/
#define APP_CPU_HW_MBOX_IRQ_SAMPLE_CLK_WA
#endif /* HW_MBOX_IS_APP_CPU */

/************************************************************************/
/*     Defines and Macros                                               */
/************************************************************************/

//#define HW_MBOX_KDBG /*! Use this define to toggle HW_MBOX kernel debug prints */
#ifdef HW_MBOX_KDBG
/* note: prints Module header and '\n' is included inside */
#  define DPRINTK(fmt, args...) printk("HW_MBOX_KDBG[%s:%d]: " fmt "\n", __FUNCTION__, __LINE__, ## args)
#else
#  define DPRINTK(fmt, args...) do{ } while(0)
#endif

#define PRINTK_INFO(fmt, args...) printk(KERN_INFO "HW_MBOX_KERNEL_INFO: " fmt "\n" , ## args)
#define PRINTK_ERR(fmt, args...)  printk(KERN_ERR  "HW_MBOX_KERNEL_ERROR[%s:%d]: " fmt "\n" , __FUNCTION__, __LINE__, ## args)

/* General Macros: */

#ifndef BIT
#define BIT(i)  (1UL << (i))
#endif

/*! YESNO_STR(logicalTerm): return "YES" if (logicalTerm evaluates as True) else return "NO" */
#define YESNO_STR(logicalTerm)    ((logicalTerm)?("YES"):("NO"))

#ifndef CONCAT2
#define CONCAT2(a, b)   a ## b
#endif

#ifndef SEC_TO_MSEC
#define SEC_TO_MSEC( _valueInSec )      (( _valueInSec ) * 1000)
#endif

#ifndef min
#define min( _a, _b )      ((( _a ) < ( _b ))?( _a ):( _b ))
#endif

/*** HW_MBOX driver Macros and Defines:  ***/

#ifdef HW_MBOX_IS_NP_CPU
/*! \def HW_MBOX_INT_LINE
    \brief irq number of the HW_MBOX. */
#define HW_MBOX_INT_LINE    AVALANCHE_HW_MBOX_INT

/*! \def HW_MBOX_INT_NAME
    \brief irq name of the HW_MBOX. */
#define HW_MBOX_INT_NAME        "hw_mbox_irq"
#endif /* HW_MBOX_IS_NP_CPU */

#ifdef HW_MBOX_IS_APP_CPU
/* define HW_MBOX_BASE as the memory mapped reg base address: */
#define HW_MBOX_BASE            hwMboxDriverData.reg_base

#ifdef HW_MBOX_EXTENDED_MEMORY
/* define HW_MBOX_EXT_MEM_BASE as the memory mapped reg base address: */
#define HW_MBOX_EXTMEM_BASE     hwMboxDriverData.extMem_base
#endif /* HW_MBOX_EXTENDED_MEMORY */
#endif /* HW_MBOX_IS_APP_CPU */

/*! \def HW_MBOX_ERROR_BIT_POS_SUPERVISOR
    \brief bit position in Interrupt Status H that signals of HW_MBOX access violation Error!
    \attention this is only true for the supervisor!
*/
#define HW_MBOX_ERROR_BIT_POS_SUPERVISOR    ((HW_MBOX_ERROR_NOTIFY_CID_FOR_SUPERVISOR) % 32)

/*! \def HW_MBOX_ERROR_BIT_FOR_SUPERVISOR
    \brief single set bit value in Interrupt Status H that signals of HW_MBOX access violation Error!
    \attention This is only true for the supervisor!
*/
#define HW_MBOX_ERROR_BIT_FOR_SUPERVISOR    BIT(HW_MBOX_ERROR_BIT_POS_SUPERVISOR)

/*! \def HW_MBOX_IS_ERROR_BIT_SET_FOR_SUPERVISOR(interruptStatus_H)
    \brief use this as supervisor to find out whether error bit is set on interrupt status high register value.
    \param[in] interruptStatus_H - HW_MBOX_MASTER_INTR_STATUS_H register value that was read.
    \attention Only use as supervisor!
*/
#define HW_MBOX_IS_ERROR_BIT_SET_FOR_SUPERVISOR(interruptStatus_H)      (((interruptStatus_H) & HW_MBOX_ERROR_BIT_FOR_SUPERVISOR) != 0)

#ifdef HW_MBOX_IS_APP_CPU
/*! \def HW_MBOX_INTERNAL_MESSAGE_COPY_TO_SRAM( _pHwMboxMsg , _destSramOffset )
    \brief Copy a HW MBOX OPCODE Internal Message to the SRAM.
    \note If CPU is APP_CPU which is LE, handles swap to BE before writing to the SRAM.
    \param[in] _pHwMboxMsg - pointer to a buffer that holds the message to be sent.
    \param[in] _destSramOffset - internal destination offset within the SRAM.
*/
#define HW_MBOX_INTERNAL_MESSAGE_COPY_TO_SRAM( _pHwMboxMsg, _destSramOffset ) \
do{\
    Uint32 _byteCnt = 0;                                    \
    Uint32 *_pWord = (Uint32 *)( _pHwMboxMsg );             \
    Uint32 _wordSwapped;                                    \
    while ( _byteCnt < HW_MBOX_OPCODE_MESSAGE_SIZE )                \
    {                                                               \
        _wordSwapped = HW_MBOX_CONVERT_TO_BIG_ENDIAN32(*_pWord);    \
        HW_MBOX_COPY_TO_SRAM( &_wordSwapped,                        \
                              sizeof(Uint32),                       \
                              _destSramOffset + _byteCnt );         \
        _pWord++;                                                   \
        _byteCnt += 4;                                              \
    }                                                               \
} while (0)
#else /* HW_MBOX_IS_APP_CPU */
#define HW_MBOX_INTERNAL_MESSAGE_COPY_TO_SRAM( _pHwMboxMsg, _destSramOffset ) \
do{\
    HW_MBOX_COPY_TO_SRAM( _pHwMboxMsg,                  \
                          HW_MBOX_OPCODE_MESSAGE_SIZE,  \
                          _destSramOffset );            \
} while (0)
#endif /* ! HW_MBOX_IS_APP_CPU */

#ifdef HW_MBOX_IS_APP_CPU
/*! \def HW_MBOX_INTERNAL_MESSAGE_COPY_FROM_SRAM( _srcSramOffset, _pHwMboxMsg )
    \brief Copy a HW MBOX OPCODE Internal Message from the SRAM.
    \note If CPU is APP_CPU which is LE, handles swap from BE after reading from the SRAM.
    \param[in]  _srcSramOffset - internal source offset within the SRAM.
    \param[out] _pHwMboxMsg - pointer to a buffer that will hold the message that was sent.
*/
#define HW_MBOX_INTERNAL_MESSAGE_COPY_FROM_SRAM( _srcSramOffset, _pHwMboxMsg ) \
do{\
    Uint32 _byteCnt = 0;                                \
    Uint32 *_pWord = (Uint32 *)( _pHwMboxMsg );         \
    while ( _byteCnt < HW_MBOX_OPCODE_MESSAGE_SIZE )    \
    {                                                           \
        HW_MBOX_COPY_FROM_SRAM( _srcSramOffset + _byteCnt,      \
                                sizeof(Uint32),                 \
                                _pWord );                       \
        *_pWord = HW_MBOX_CONVERT_FROM_BIG_ENDIAN32(*_pWord);   \
        _pWord++;                                               \
        _byteCnt += 4;                                          \
    }                                                           \
} while (0)
#else /* HW_MBOX_IS_APP_CPU */
#define HW_MBOX_INTERNAL_MESSAGE_COPY_FROM_SRAM( _srcSramOffset, _pHwMboxMsg ) \
do{\
    HW_MBOX_COPY_FROM_SRAM( _srcSramOffset,               \
                            HW_MBOX_OPCODE_MESSAGE_SIZE,  \
                            _pHwMboxMsg );                \
} while (0)
#endif /* ! HW_MBOX_IS_APP_CPU */

#ifdef HW_MBOX_SOCKETS_API

#ifdef HW_MBOX_KDBG
/* note: prints Module header and '\n' is included inside */
#  define DPRINTK_SKT( _pSocket, fmt, args...) printk("HW_MBOX_KDBG_SKT[%s:%d]: (pSocket=0x%p) " fmt "\n", __FUNCTION__, __LINE__, _pSocket, ## args)
#else
#  define DPRINTK_SKT( _pSocket, fmt, args...) do{ } while(0)
#endif

#define HW_MBOX_COPY_KERNEL     (1)
#define HW_MBOX_COPY_USER       (0)

#define HW_MBOX_PROTOCOL_BY_APPID( _appid ) \
    ( ( ((_appid) >= HW_MBOX_APPID_OPCODE_MIN ) && ((_appid) <= HW_MBOX_APPID_OPCODE_MAX ) ) ?  \
       hw_mbox_opcode :                                                                         \
       ( (_appid) == HW_MBOX_APPID_COMMAND ) ?                                                  \
       hw_mbox_command :                                                                        \
       ( (_appid) == HW_MBOX_APPID_EVENT   ) ?                                                  \
       hw_mbox_event :                                                                          \
       (-1) )

#define HW_MBOX_SOCKETS_APPID_VALIDITY_CHECK(paramAPPID) \
    (( (paramAPPID) >= HW_MBOX_APPID_MIN_VALUE ) && ( (paramAPPID) <= HW_MBOX_APPID_MAX_VALUE ))

#define HW_MBOX_USER_COPY_FROM_ADDR(srcAddr, srcAddrLen, pDestBuffer)          copy_to_user(pDestBuffer,srcAddr, srcAddrLen)
#define HW_MBOX_USER_COPY_TO_ADDR(pSrcBuffer, srcBufferLen, destAddr)          copy_from_user(destAddr, pSrcBuffer, srcBufferLen)
#define HW_MBOX_USER_COPY_FROM_SRAM(srcSramOffset, srcSramLen, pDestBuffer)    HW_MBOX_USER_COPY_FROM_ADDR(HW_MBOX_POINTER_TO_SRAM(srcSramOffset), srcSramLen, pDestBuffer)
#define HW_MBOX_USER_COPY_TO_SRAM(pSrcBuffer, srcBufferLen, destSramOffset)    HW_MBOX_USER_COPY_TO_ADDR(pSrcBuffer, srcBufferLen, HW_MBOX_POINTER_TO_SRAM(destSramOffset))
#endif /* HW_MBOX_SOCKETS_API */

#ifdef HW_MBOX_EXTENDED_MEMORY
typedef struct hwMbox_extMemMsgData
{
    Uint32 offset;
    Uint32 lenInBytes;
} hwMbox_extMemMsgData_t;

typedef Uint32 hwMbox_extMemCtrlHdr_t;

#define HW_MBOX_FLAG_OWNER            0x00000001 /*! if owner bit is set than the owner of EXT MEM is the subject */

#define HW_MBOX_IS_OWNER( _ctrl )     ( HW_MBOX_FLAG_OWNER & ( _ctrl ) )
#define HW_MBOX_SET_OWNER( _ctrl )    do { ( _ctrl ) |=  HW_MBOX_FLAG_OWNER; } while(0)
#define HW_MBOX_CLR_OWNER( _ctrl )    do { ( _ctrl ) &= ~HW_MBOX_FLAG_OWNER; } while(0)

#define HW_MBOX_EXTMEM_IS_SUBJECT_OWNER( _ctrl )        ( HW_MBOX_IS_OWNER( _ctrl ) != 0 /* bit is set */ )
#define HW_MBOX_EXTMEM_IS_COMMANDER_OWNER( _ctrl )      ( HW_MBOX_IS_OWNER( _ctrl ) == 0 /* bit is NOT set */ )

#define HW_MBOX_USER_COPY_FROM_EXTMEM(srcExtmemOffset, srcExtmemLen, pDestBuffer)    HW_MBOX_USER_COPY_FROM_ADDR(HW_MBOX_POINTER_TO_EXTMEM(srcExtmemOffset), srcExtmemLen, pDestBuffer)
#define HW_MBOX_USER_COPY_TO_EXTMEM(pSrcBuffer, srcBufferLen, destExtmemOffset)      HW_MBOX_USER_COPY_TO_ADDR(pSrcBuffer, srcBufferLen, HW_MBOX_POINTER_TO_EXTMEM(destExtmemOffset))
#endif /* HW_MBOX_EXTENDED_MEMORY */

/************************************************************************/
/*     HW_MBOX Package configuration data arrays:                       */
/************************************************************************/

/* List HW_MBOX Master names (using the Master Table X-MACRO): */

/*! \var static const char *HW_MBOX_MASTER_STRS[]
    \brief List all HW_MBOX Master names listed by MID: "<_desc>"
    \attention use HW_MBOX_MASTER_GET_NAME() to access.
*/
HW_MBOX_MASTER_TABLE_STRS(HW_MBOX_MASTER_STRS);

/*! \def HW_MBOX_MASTER_GET_NAME(paramMID)
    \brief Get Name of HW_MBOX Master by MID. */
#define HW_MBOX_MASTER_GET_NAME(paramMID)    HW_MBOX_MASTER_STRS[paramMID]

/* List HW_MBOX Master configurations (using the Master Table X-MACRO): */

/*! \var static const hw_mbox_masterConfig_t HW_MBOX_MASTER_CONFIG[]
    \brief List all Masters configurations in an Array enabling access.
    \note index in the array is the MID.
    \attention use HW_MBOX_MASTER_GET_CONFIG() to access.
*/
HW_MBOX_MASTER_TABLE_DB(HW_MBOX_MASTER_CONFIG);

/*! \def HW_MBOX_MASTER_GET_CONFIG(paramMID)
    \brief Gets a Master's configuration.
    \note returns a hw_mbox_masterConfig_t struct.
*/
#define HW_MBOX_MASTER_GET_CONFIG(paramMID)    HW_MBOX_MASTER_CONFIG[paramMID]

/* List HW_MBOX static channel configurations (using the Static Channel Table X-MACRO): */

/*! \var static const hw_mbox_channelConfig_t HW_MBOX_STATIC_CHANNEL_CONFIG[]
    \brief List all static Channel configurations in an Array enabling access.
    \note index in the array is NOT the CID.
    \attention use HW_MBOX_STATIC_CHANNEL_GET_CONFIG() to access.
*/
HW_MBOX_STATIC_CHANNEL_TABLE_DB(HW_MBOX_STATIC_CHANNEL_CONFIG);

/*! \def HW_MBOX_STATIC_CHANNEL_GET_CONFIG(staticChannelIndex)
    \brief Gets a static Channel's configuration.
    \note returns a hw_mbox_channelConfig_t struct.
*/
#define HW_MBOX_STATIC_CHANNEL_GET_CONFIG(staticChannelIndex)    HW_MBOX_STATIC_CHANNEL_CONFIG[staticChannelIndex]

/**************************************************************************/
/*      LOCAL GENERAL DRIVER VARIABLES:                                   */
/**************************************************************************/

/*! \var static bool isInitialized
    \brief to be set when driver is fully initialized.
    \note use bool to be consistent with module_param macro.
*/
static bool isInitialized = false;
module_param(isInitialized, bool, S_IRUGO); /* read-only 'cat'-able kernel param */

/*! \var static wait_queue_head_t waitForInitializedWq
    \brief used for channel device blocking open operation.
*/
static DECLARE_WAIT_QUEUE_HEAD(waitForInitializedWq);

/*! \def SET_INITIALIZED()
    \brief use once the driver is initialized and ready.
    \note also wakes up any process that opened a device in a blocking
          manner prior to driver initialization.
*/
#define SET_INITIALIZED() \
do{ \
    isInitialized =  true;                                \
    /* since the sleeps are not exclusive also the non */ \
    /* _all variant will wake up all... */                \
    wake_up_interruptible_all(&waitForInitializedWq);     \
} while (0)

#define IS_INITIALIZED()    ( isInitialized == true )

/*! \def CHECK_IS_DRV_INITIALIZED( __onNotInitialized )
    \brief use to check if the driver is initialized, if it is not
           execute __noNotInitialized.
    \param[in] __onNotInitialized - executed in case of driver not
                                    initialized.
*/
#define CHECK_IS_DRV_INITIALIZED( __onNotInitialized ) \
do{ \
    if (!IS_INITIALIZED())                      \
    {                                           \
        DPRINTK("Driver not initialized yet!"); \
        __onNotInitialized;                     \
    }                                           \
} while (0)

/*! \var static Bool isInitComplete
    \brief to be set when supervisor finishes initial configuration of
           the HW MBOX.
    \note non supervisor receives notification via opcode channel.
*/
static Bool isInitComplete = False;

/*! \var static wait_queue_head_t waitForInitCompleteWq
    \brief used to wait for init Complete by supervisor.
    \note used by the first recvOpcode listener to wait for init Complete
          notification from supervisor.
*/
static DECLARE_WAIT_QUEUE_HEAD(waitForInitCompleteWq);

#define IS_INIT_COMPLETE()    (isInitComplete == True)

/*! \def SET_INIT_COMPLETE()
    \brief use once the supervisor finished HW MBOX initial configuration.
*/
#define SET_INIT_COMPLETE() \
do{ \
    isInitComplete =  True;                               \
    /* since the sleeps are not exclusive also the non */ \
    /* _all variant will wake up all... */                \
    wake_up_interruptible_all(&waitForInitCompleteWq);    \
} while (0)

#if defined(HW_MBOX_DEBUG) && defined(HW_MBOX_KDBG)
/*! \var static bool enableDevicePrintk
    \brief to be set when verbose device prints are required.
    \note use bool to be consistent with module_param macro.
*/
static bool enableDevicePrintk = false;
module_param(enableDevicePrintk, bool, S_IRUGO | S_IWUSR); /* read/write 'cat/echo'-able kernel param */

#define IS_DEV_PRINTK_ENABLED()    ( enableDevicePrintk == true )

/*! \def DEV_PRINTK(fmt, args...)
    \brief verbose debug prints for the char devices, enabled by module param.
    \note prints are disabled by default, change <enableDevicePrintks> module params
          to enable the prints.
    \attention feature is only available when HW_MBOX_DEBUG precompilation flag is set.
*/
#   define DEV_PRINTK(fmt, args...) \
do{ \
    if ( IS_DEV_PRINTK_ENABLED() )              \
    {                                           \
        DPRINTK("DEV_PRINTK: " fmt , ## args);  \
    }                                           \
} while (0)
#else
#   define DEV_PRINTK(fmt, args...) do{ } while(0)
#endif /* HW_MBOX_DEBUG && HW_MBOX_KDBG */

/************************************************************************/
/*     HW_MBOX Kernel Defines & Types                                   */
/************************************************************************/

/*** Channel Interrupts: ***/

#define CHN_INTR_NO_TIMEOUT  (-1)

#ifdef HW_MBOX_DEFAULT_TIMEOUT_SEC
#define CHN_INTR_DEFAULT_TIMEOUT_MSEC   SEC_TO_MSEC(HW_MBOX_DEFAULT_TIMEOUT_SEC)
#else
#define CHN_INTR_DEFAULT_TIMEOUT_MSEC   CHN_INTR_NO_TIMEOUT /* no Timeout: Used for debug */
#endif /* HW_MBOX_DEFAULT_TIMEOUT_SEC */

/*! \enum chnIntrState_e
    \brief Channel Interrupt state. */
typedef enum chnIntrState
{
    CHN_INTR_STATE_NONE = 0,
    CHN_INTR_STATE_PENDING
} chnIntrState_e;

/*! \struct chnIntr_t
    \brief Channel Interrupt abstraction. */
typedef struct chnIntr
{
    wait_queue_head_t       waitQueue; /* wait queue for channel interrupts */
    chnIntrState_e          state;     /* indicates channel interrupt state */
} chnIntr_t;

/*! \struct chnint_t
    \brief Channel Interrupt Device structure. */
typedef struct chnint
{
    Uint32              chn_cid;      /* CID of the channel this device is connected to */
    struct cdev         chnint_cdev;  /* channel's char device structure */
} chnint_t;

/*** Receive Opcode Infrastructure: ***/

#define RECV_OPCODE_LISTENER_PREFIX    "hwMboxKRecvOp" /*! thread name prefix */

/*! \def RECV_OPCODE_INIT_INDEX
    \brief on non-supervisor the first opcode listener is used to continue the
           driver initialization process.
*/
#define RECV_OPCODE_INIT_INDEX      0

#define IS_RECV_OPCODE_INIT_INDEX( _threadIdx )     ( ( _threadIdx ) == RECV_OPCODE_INIT_INDEX )

/*! \struct recvOpcodeListenerThreadData_t
    \brief data relevant to the recieve opcode listener. */
typedef struct recvOpcodeListenerThreadData
{
    Uint32 index;
    Uint32 cid;
} recvOpcodeListenerThreadData_t;

/*! \struct recvOpcodeRegisteredTag_t
    \brief A single entry registered by application. */
typedef struct recvOpcodeRegisteredTag
{
    Uint32                      tag;
    hwMboxRecvOpcodeCallback    callbackFunc;
    Uint32                      token;

    /* statistics: */
    Uint32                  opcodeCount; /* counts opcodes that arrived with this tag */
} recvOpcodeRegisteredTag_t;

/*! \struct recvOpcodeInfrastructure_t
    \brief required infrastructure for receive opcode channels. */
typedef struct recvOpcodeInfrastructure
{
    struct task_struct              *listenerThread;
    recvOpcodeRegisteredTag_t       registeredTags[HW_MBOX_OPCODE_TAG_COUNT];

    chnIntr_t appWq; /* Application's wait queue */

    Uint32 registeredCount; /* amount of registered tags/applications */

    /* statistics: */
    Uint32 wakeupCount;     /* counts listener thread wake ups */
    Uint32 invalidTagCount; /* counts opcodes that arrived with an invlaid tag */
} recvOpcodeInfrastructure_t;

/*! \struct statistics_t
    \brief HW MBOX statistics. */
typedef struct hwMbox_statistics
{
    /* Service Counters:
       How many times each service was invoked.
       Note: Sizing and dimensions are the same as CID LUT.
    */
    Uint32 serviceCounters[HW_MBOX_MASTER_COUNT][HW_MBOX_TYPE_COUNT << 1];
} hwMbox_statistics_t;

/************************************************************************/
/*     HW_MBOX Kernel Local Declarations                                */
/************************************************************************/

static int hwMbox_initAfterStarted(void);
static int hwMbox_setChannelConfig(hw_mbox_channelConfig_t config);
static int hwMbox_setChannelValid(Uint32 paramCID, Bool isValid);
static int hwMbox_waitForChannelInterrupt(int cid, int timeoutInMsec, HW_MBOX_STATUS_e *pHwMboxStatus);

static int hwMbox_recvOpcodeListenerEntry(void *data);

#ifdef HW_MBOX_SOCKETS_API
/************************************************************************/
/*     HW_MBOX sockets Local Declarations                               */
/************************************************************************/
static struct proto hwMbox_proto;
static const struct net_proto_family hwMbox_family_ops;
static const struct proto_ops hwMbox_proto_ops;

static int _hwMbox_SendMsg(Uint32 paramCID, Uint32 paramAppID, Uint8 *dataBuf, Uint32 dataLen, Uint32 userOrKernel);
static int hwMbox_sendCmdMsg(hw_mbox_Masters_e subject, Uint32 paramAppID, Uint8 *dataBuf, Uint32 dataLen, Uint32 userOrKernel);
static int hwMbox_sendReplyMsg(hw_mbox_Masters_e commander, Uint32 paramAppID, Uint8 *dataBuf, Uint32 dataLen, Uint32 userOrKernel);

static int _hwMbox_RecvMsg(Uint32 paramCID, Uint32 paramAppID, Uint8 *dataBuf, Uint32 dataLen, Uint32 userOrKernel, Uint32 eraseMemAfter, HW_MBOX_STATUS_e *pHwMboxStatus);
static int hwMbox_recvReplyMsg(hw_mbox_Masters_e subject, Uint32 paramAppID, Uint8 *dataBuf, Uint32 dataLen, Uint32 userOrKernel, Uint32 eraseMemAfter);
static int hwMbox_recvCmdMsg(hw_mbox_Masters_e commander, Uint32 paramAppID, Uint8 *dataBuf, Uint32 dataLen, Uint32 userOrKernel, Uint32 eraseMemAfter);

static int _hwMbox_sendOpcodeMsg(Uint32 paramCID, HW_MBOX_STATUS_e status);
static int hwMbox_socketRecvOpCb(hw_mbox_Masters_e commander, Uint8 *dataBuf, Uint32 dataLen, Uint32 *token);
#endif /* HW_MBOX_SOCKETS_API */

/************************************************************************/
/*     HW_MBOX driver data                                              */
/************************************************************************/

/*! \struct ctrlDb_t
    \brief holds general control information. */
typedef struct ctrlDbEntry
{
    Uint32 coreVerMajor;
    Uint32 coreVerMinor;

    hw_mbox_Masters_e supervisor;
    hw_mbox_Masters_e monitor;    /* if is HW_MBOX_MID_INVALID then no monitor is defined */

    Bool isHwDebugMode; /* is in HW Debug Mode? */
    Bool isStarted;     /* is Started? (initial configuration by supervisor done) */
    Bool isLocked;      /* is Locked? (once set configuration is fixed, value cannot be changed once set) */
} ctrlDbEntry_t;

/*! \struct masterDbEntry_t
    \brief a single Master entry in driver DB. */
typedef struct masterDbEntry
{
    Bool valid;         /* flag is set if Master is valid (Not fused out and has HW_MBOX SW support) */
    Bool initialized;   /* flag is set once Master responds to the initial command the supervisor sends [to notify the Masters that HW MBOX is started...] */

    hw_mbox_masterConfig_t config; /* Master's configuration */

    Uint32 errorCount;  /* only relevant for supervisor [currently only incremented in User-Space] */
} masterDbEntry_t;

/*! \struct chnDbEntry_t
    \brief a single channel entry in driver DB. */
typedef struct chnDbEntry
{
    Bool assigned;         /* flag is set once configuration has been written */
    Bool valid;            /* flag is set once channel is valid and can be used by Master */
    Bool isInitChannel;    /* special flag that marks that this is a channel for initial configuration */

    hw_mbox_channelConfig_t config;     /* Channel's configuration */

#ifdef HW_MBOX_EXTENDED_MEMORY
    Bool        isExtMem;
    Uint32      extMemOffset;
    Uint32      extMemSize;
#endif /* HW_MBOX_EXTENDED_MEMORY */

    Uint32      intrCount;       /* count channel interrupts */
    Uint32      recvOpcodeIndex; /* indicates relevant entry in recvOpData [only relevant for recvOpcode] */
} chnDbEntry_t;

/*! \def CHN_DB_ENTRY_SIZE
    \brief Channel entry struct's size in driver DB. */
#define CHN_DB_ENTRY_SIZE    sizeof(chnDbEntry_t)

/*! \struct hw_mbox_drv_data_t
    \brief struct for HW_MBOX driver Local Data. */
typedef struct hw_mbox_drv_data
{
    /* General Control Information: */
    ctrlDbEntry_t ctrlData;

    /* Channel Database: */
    chnDbEntry_t chnDb[HW_MBOX_CHANNEL_COUNT];

    /* Master Database: */
    masterDbEntry_t masterDb[HW_MBOX_MASTER_COUNT];

    /* Statistics: */
    Uint32 errIntr;     /* incremented only for supervisor */
    Uint32 ghostIntr;   /* incremented if there's an interrupt but no set bits in Interrupt Status Registers */

#ifdef HW_MBOX_IS_APP_CPU
    void __iomem *reg_base;     /* Mapped io reg base address */
#ifdef HW_MBOX_EXTENDED_MEMORY
    void __iomem *extMem_base;  /* Mapped io extended memory base address */
#endif /* HW_MBOX_EXTENDED_MEMORY */
#endif /* HW_MBOX_IS_APP_CPU */
} hw_mbox_drv_data_t;

/*! \def HW_MBOX_DRIVER_DATA_SIZE
    \brief HW_MBOX driver Data struct's size. */
#define HW_MBOX_DRIVER_DATA_SIZE    sizeof(hw_mbox_drv_data_t)

/*! \var static hw_mbox_drv_data_t hwMboxDriverData
    \brief HW_MBOX driver Local Data. */
static hw_mbox_drv_data_t hwMboxDriverData;

/***** DATABASE ACCESS MACRO HELPERS: *****/

#define RESET_DRIVER_DATA()    (memset(&hwMboxDriverData, 0, HW_MBOX_DRIVER_DATA_SIZE))

/*** Ctrl DB: ***/

#define CTRL_GET_CORE_VER_MAJ()             (hwMboxDriverData.ctrlData.coreVerMajor)
#define CTRL_SET_CORE_VER_MAJ( _value )     (hwMboxDriverData.ctrlData.coreVerMajor = ( _value ))

#define CTRL_GET_CORE_VER_MIN()             (hwMboxDriverData.ctrlData.coreVerMinor)
#define CTRL_SET_CORE_VER_MIN( _value )     (hwMboxDriverData.ctrlData.coreVerMinor = ( _value ))

#define CTRL_GET_IS_HW_DEBUG_MODE()         (hwMboxDriverData.ctrlData.isHwDebugMode)
#define CTRL_SET_IS_HW_DEBUG_MODE( _value ) (hwMboxDriverData.ctrlData.isHwDebugMode = ( _value ))

#define CTRL_GET_IS_STARTED()               (hwMboxDriverData.ctrlData.isStarted)
#define CTRL_SET_IS_STARTED( _value )       (hwMboxDriverData.ctrlData.isStarted = ( _value ))

#define CTRL_GET_IS_LOCKED()                (hwMboxDriverData.ctrlData.isLocked)
#define CTRL_SET_IS_LOCKED( _value )        (hwMboxDriverData.ctrlData.isLocked = ( _value ))

#define CTRL_GET_SUPERVISOR()               (hwMboxDriverData.ctrlData.supervisor)
#define CTRL_SET_SUPERVISOR( _value )       (hwMboxDriverData.ctrlData.supervisor = ( _value ))
#define IS_SUPERVISOR()                     (hwMboxDriverData.ctrlData.supervisor == HW_MBOX_MID)

#define CTRL_GET_MONITOR()                  (hwMboxDriverData.ctrlData.monitor)
#define CTRL_SET_MONITOR( _value )          (hwMboxDriverData.ctrlData.monitor = ( _value ))
#define IS_MONITOR()                        (hwMboxDriverData.ctrlData.monitor == HW_MBOX_MID)

/*** Master DB: ***/

#define MASTER_GET_VALID( _mid )               (hwMboxDriverData.masterDb[ _mid ].valid)
#define MASTER_SET_VALID( _mid, _value)        (hwMboxDriverData.masterDb[ _mid ].valid = ( _value ))
#define IS_MASTER_VALID( _mid )                (hwMboxDriverData.masterDb[ _mid ].valid == True)

#define MASTER_GET_INITIALIZED( _mid )         (hwMboxDriverData.masterDb[ _mid ].initialized)
#define MASTER_SET_INITIALIZED( _mid, _value)  (hwMboxDriverData.masterDb[ _mid ].initialized = ( _value ))
#define IS_MASTER_INIT( _mid )                 (hwMboxDriverData.masterDb[ _mid ].initialized == True)

/* Configuration: */

#define MASTER_CFG( _mid )                      (hwMboxDriverData.masterDb[ _mid ].config)

#define MASTER_CFG_GET_SUPPORT( _mid )          (hwMboxDriverData.masterDb[ _mid ].config.support)

#define MASTER_GET_ERROR_COUNT( _mid )          (hwMboxDriverData.masterDb[ _mid ].errorCount)

/*** Channels DB: ***/

#define CHN_ENTRY( _cid )                       (hwMboxDriverData.chnDb[ _cid ])

#define CHN_IS_ASSIGNED( _cid )                 (hwMboxDriverData.chnDb[ _cid ].assigned == True)
#define CHN_SET_ASSIGNED( _cid, _value)         (hwMboxDriverData.chnDb[ _cid ].assigned = ( _value ))

#define CHN_IS_VALID( _cid )                    (hwMboxDriverData.chnDb[ _cid ].valid == True)
#define CHN_SET_VALID( _cid, _value)            (hwMboxDriverData.chnDb[ _cid ].valid = ( _value ))

#define CHN_IS_INIT( _cid )                     (hwMboxDriverData.chnDb[ _cid ].isInitChannel == True)
#define CHN_SET_INIT( _cid, _value)             (hwMboxDriverData.chnDb[ _cid ].isInitChannel = ( _value ))

/* Configuration: */

#define CHN_CFG( _cid )                         (hwMboxDriverData.chnDb[ _cid ].config)

#define CHN_CFG_GET_TYPE( _cid )                (hwMboxDriverData.chnDb[ _cid ].config.mboxType)
#define CHN_CFG_GET_COMMANDER( _cid )           (hwMboxDriverData.chnDb[ _cid ].config.commander)
#define CHN_CFG_GET_SUBJECT( _cid )             (hwMboxDriverData.chnDb[ _cid ].config.subject)

#define CHN_CFG_MEMBUFF_GET_SIZE( _cid )        (hwMboxDriverData.chnDb[ _cid ].config.memBuff.size)
#define CHN_CFG_MEMBUFF_GET_START( _cid )       (hwMboxDriverData.chnDb[ _cid ].config.memBuff.start)

#ifdef HW_MBOX_EXTENDED_MEMORY
#define CHN_CFG_IS_EXTMEM( _cid )                (hwMboxDriverData.chnDb[ _cid ].isExtMem == True)
#define CHN_CFG_SET_EXTMEM( _cid, _val )         (hwMboxDriverData.chnDb[ _cid ].isExtMem = ( _val ))

#define CHN_CFG_EXTMEM_SET_OFFSET( _cid, _val )  (hwMboxDriverData.chnDb[ _cid ].extMemOffset = ( _val ))
#define CHN_CFG_EXTMEM_GET_OFFSET( _cid )        (hwMboxDriverData.chnDb[ _cid ].extMemOffset)

#define CHN_CFG_EXTMEM_SET_SIZE( _cid, _val )    (hwMboxDriverData.chnDb[ _cid ].extMemSize = ( _val ))
#define CHN_CFG_EXTMEM_GET_SIZE( _cid )          (hwMboxDriverData.chnDb[ _cid ].extMemSize)
#endif /* HW_MBOX_EXTENDED_MEMORY */

/* Check if it's a receive Opcode channel: */
#define CHN_IS_RECV_OP( _cid )  ( ((CHN_CFG_GET_TYPE( _cid ) == hw_mbox_opcode)  && \
                                   (CHN_CFG_GET_SUBJECT( _cid ) == HW_MBOX_MID)) == True )

/* infrastracture: */

#define CHN_INTR_GET_COUNT( _cid )                  (hwMboxDriverData.chnDb[ _cid ].intrCount)
#define CHN_INTR_INC_COUNT( _cid )                  (hwMboxDriverData.chnDb[ _cid ].intrCount++)

#define CHN_RECV_OPCODE_GET_INDEX( _cid )           (hwMboxDriverData.chnDb[ _cid ].recvOpcodeIndex)
#define CHN_RECV_OPCODE_SET_INDEX( _cid, _value )   (hwMboxDriverData.chnDb[ _cid ].recvOpcodeIndex = ( _value ))

#define CHN_RECV_OPCODE_INDEX_INVALID  (-1) /*! use for non recvOpcode channel */

/*** Statistics ***/

#define GET_ERR_INTR()                          (hwMboxDriverData.errIntr)
#define INC_ERR_INTR()                          (hwMboxDriverData.errIntr++)

#define GET_GHOST_INTR()                        (hwMboxDriverData.ghostIntr)
#define INC_GHOST_INTR()                        (hwMboxDriverData.ghostIntr++)

/*** SRAM allocation: ***/

#define GET_FREE_SRAM_LEFT()                    (HW_MBOX_SRAM_SIZE - __currentNonallocatedSramOffset)
#define GET_NEXT_ALLOCATION_SRAM_OFFSET()       (__currentNonallocatedSramOffset)

#define ALLOCATE_SRAM(paramCID, requiredSizeInBytes)    \
do{ \
     /* Update Memory Map registers: */                                         \
    HW_MBOX_MEMORY_MAP_START_SET(paramCID, __currentNonallocatedSramOffset);    \
    HW_MBOX_MEMORY_MAP_SIZE_SET(paramCID, requiredSizeInBytes);                 \
    /* advance current SRAM offset by size of allocated area: */                \
    __currentNonallocatedSramOffset += requiredSizeInBytes;                     \
} while (0)

/************************************************************************/
/*     HW_MBOX Channel Devices:                                         */
/************************************************************************/

/* Channel Devices Function Declarations: */

/* File Operations: */
static int chnint_open(struct inode *inode, struct file *filp);
static ssize_t chnint_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
static unsigned int chnint_poll(struct file *filp, struct poll_table_struct *wait);
static long chnint_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);
static int chnint_release(struct inode *inode, struct file *filp);

static void chnint_cleanup(int count);

/* Channel interrupt Devices Local Variables: */

/*! \var static dev_t chnint_dev
    \brief The first channel interrupt device that will be created, Contains major and first minor number.
    \note This is the reference point for allocation of all other devices and the chrdev region. */
static dev_t chnint_dev;

/*! \var static struct class *chnint_class
    \brief Channel interrupt devices class. */
static struct class *chnint_class;

/*! \var static chnint_t *chnint_devices
    \brief Array of all the channel interrupt Device structures, one entry per channel.
    \note Index in the array is the CID of the device. */
static chnint_t *chnint_devices;

#define RESET_CHN_DEVS()    (memset(chnint_devices, 0, HW_MBOX_CHANNEL_COUNT * sizeof(chnint_t)))

/*! \var static struct file_operations chnint_fops
    \brief file operations structure for the channel interrupt devices. */
static struct file_operations chnint_fops = {
    .owner          = THIS_MODULE,
    .read           = chnint_read,
    .poll           = chnint_poll,
    .unlocked_ioctl = chnint_ioctl,
    .open           = chnint_open,
    .release        = chnint_release,
};

/************************************************************************/
/*     SEQ PROC Interface Macros:                                       */
/************************************************************************/

//#define DEBUG_SEQ_PROC    /*! debug prints for seq_file interface */
#ifdef DEBUG_SEQ_PROC
/* note: prints Feature header */
#  define DPRINT_SEQ(m, fmt, args...) seq_printf(m, fmt, ## args)
#else
#  define DPRINT_SEQ(m, fmt, args...)
#endif

/* Defines and Structs: */

typedef struct seq_proc_if_control_data
{
    Bool endOfData;    /* if True then there's no more data left and proc should end */
} seq_proc_if_control_data_t;

#define SEQ_PROC_IF_CONTROL_DATA_SIZE   sizeof(seq_proc_if_control_data_t)

/* Function Declare Macros: */

#define SEQ_PROC_DECLARE_OPEN(procName) \
static int CONCAT2(procName, _open)(struct inode *inode, struct file *file)

#define SEQ_PROC_DECLARE_START(procName) \
static void *CONCAT2(procName, _start)(struct seq_file *m, loff_t *pos)

#define SEQ_PROC_DECLARE_NEXT(procName) \
static void *CONCAT2(procName, _next)(struct seq_file *m, void *v, loff_t *pos)

#define SEQ_PROC_DECLARE_STOP(procName) \
static void CONCAT2(procName, _stop)(struct seq_file *m, void *v)

#define SEQ_PROC_DECLARE_SHOW(procName) \
static int CONCAT2(procName, _show)(struct seq_file *m, void *v)

#define SEQ_PROC_DECLARE_SIMPLE(procName) \
static int procName(struct seq_file *m, void *v)

/* OPs Define Macros: */

#define SEQ_PROC_DEFINE_FOPS(procName) \
static const struct file_operations CONCAT2(procName, _fops) = {        \
    .owner   = THIS_MODULE,                                             \
    .open    = CONCAT2(procName ,_open),                                \
    .read    = seq_read,                                                \
    .llseek  = seq_lseek,                                               \
    .release = seq_release_private /* release also the private data */  \
}

#define SEQ_PROC_DEFINE_SEQOPS(procName) \
static const struct seq_operations CONCAT2(procName, _seqops) = { \
    .start = CONCAT2(procName, _start),                           \
    .next  = CONCAT2(procName, _next),                            \
    .stop  = CONCAT2(procName, _stop),                            \
    .show  = CONCAT2(procName, _show)                             \
}

#define SEQ_PROC_DEFINE_SIMPLE_FOPS(procName) \
static const struct file_operations CONCAT2(procName, _fops) = { \
    .owner   = THIS_MODULE,                                      \
    .open    = CONCAT2(procName ,_open),                         \
    .read    = seq_read,                                         \
    .llseek  = seq_lseek,                                        \
    .release = single_release                                    \
}

/* Function Define Macros: */

#define SEQ_PROC_DEFINE_OPEN(procName) \
static int CONCAT2(procName, _open)(struct inode *inode, struct file *file) \
{                                                                           \
    seq_proc_if_control_data_t *controlData;                                \
    int retVal = -ENOMEM;                                                   \
    controlData = kzalloc(SEQ_PROC_IF_CONTROL_DATA_SIZE, GFP_KERNEL);       \
    if (controlData != NULL)                                                \
    {                                                                       \
        retVal = seq_open(file, &CONCAT2(procName, _seqops));               \
        if (!retVal)                                                        \
        {                                                                   \
            struct seq_file *m = file->private_data;                        \
            m->private = controlData;                                       \
        }                                                                   \
        else                                                                \
        {                                                                   \
            kfree(controlData);                                             \
        }                                                                   \
    }                                                                       \
    return retVal;                                                          \
}

#define SEQ_PROC_DEFINE_SIMPLE_OPEN(procName) \
static int CONCAT2(procName, _open)(struct inode *inode, struct file *file) \
{                                                                           \
    return single_open(file, procName, NULL);                               \
}

#define SEQ_PROC_DEFINE_START(procName) \
static void *CONCAT2(procName, _start)(struct seq_file *m, loff_t *pos)                                     \
{                                                                                                           \
    /* This function is entered when proc is first called or a upon new page [also after stop!!!] */        \
    seq_proc_if_control_data_t *controlData = m->private;                                                   \
    loff_t *spos = NULL; /* our iterator, default to NULL */                                                \
    if (!controlData->endOfData)                                                                            \
    {                                                                                                       \
        /* New Run or new page, more data, initialize the iterator */                                       \
        spos = kzalloc(sizeof(loff_t), GFP_KERNEL);                                                         \
        if (!spos)                                                                                          \
        {                                                                                                   \
            /* Allocation Failed!!! */                                                                      \
            PRINTK_ERR("Can't allocate memory!");                                                           \
            return NULL;                                                                                    \
        }                                                                                                   \
        *spos = *pos;                                                                                       \
        DPRINT_SEQ(m, "START (*spos=%lld)\n", (long long)*spos);                                            \
    }                                                                                                       \
    return spos; /* Iterator if New Run\Page, defaultive NULL in case returning after stop to mark EOF */   \
}

#define SEQ_PROC_DEFINE_NEXT(procName) \
static void *CONCAT2(procName, _next)(struct seq_file *m, void *v, loff_t *pos)     \
{                                                                                   \
    seq_proc_if_control_data_t *controlData = m->private;                           \
    loff_t *spos = v; /* the current position */                                    \
    if (controlData->endOfData)                                                     \
    {                                                                               \
        DPRINT_SEQ(m, "LAST NEXT (*spos=%lld)\n", *spos);                           \
        return NULL; /* NULL to mark EOF */                                         \
    }                                                                               \
    else                                                                            \
    {                                                                               \
        DPRINT_SEQ(m, "NEXT ");                                                     \
        DPRINT_SEQ(m, "*pos=%lld->", (long long)*pos);                              \
        (*spos)++;      /* increment current position */                            \
        *pos = *spos;   /* set it in *pos [set "next" item] */                      \
        DPRINT_SEQ(m, "%lld\n", (long long)*pos);                                   \
        return spos;    /* return ["next"] current position */                      \
    }                                                                               \
}

#define SEQ_PROC_DEFINE_STOP(procName) \
static void CONCAT2(procName, _stop)(struct seq_file *m, void *v) \
{                                                                 \
    DPRINT_SEQ(m, "STOP, cleanup.\n");                            \
    kfree(v); /* finished with the iterator */                    \
}

/* Interface API Macros: */

/*! \def SEQ_PROC_DEFINE(procName)
    \brief macro that will define a generic seq proc template, position advances by one per step.
    \param[in] procName - name designation of the seq proc, to be used as prefix to all the needed functions.
    \attention user should define a show function with prototype/name: static int <procName>_show(struct seq_file *m, void *v)
    \attention user must signal end of data setting <endOfData> field of seq_proc_if_control_data_t struct to True.
    \attention seq_proc_if_control_data_t struct is contained in the seq file private field.
    \note <procName> can be different then the actual file name that will be used for the proc [see SEQ_PROC_CREATE].
*/
#define SEQ_PROC_DEFINE(procName) \
/* Function Declarations: */      \
SEQ_PROC_DECLARE_OPEN(procName);  \
SEQ_PROC_DECLARE_START(procName); \
SEQ_PROC_DECLARE_NEXT(procName);  \
SEQ_PROC_DECLARE_STOP(procName);  \
SEQ_PROC_DECLARE_SHOW(procName);  \
/* Define OPs structures: */      \
SEQ_PROC_DEFINE_FOPS(procName);   \
SEQ_PROC_DEFINE_SEQOPS(procName); \
/* Define Functions:  */          \
SEQ_PROC_DEFINE_OPEN(procName);   \
SEQ_PROC_DEFINE_START(procName);  \
SEQ_PROC_DEFINE_NEXT(procName);   \
SEQ_PROC_DEFINE_STOP(procName);

/*! \def SEQ_PROC_DEFINE_SIMPLE(procName)
    \brief macro that will define a simple generic seq proc template, where no more than one page is needed.
    \param[in] procName - name designation of the seq proc, to be used as prefix to all the needed functions and also the name of the actual proc function.
    \attention user should define the proc function with prototype/name: static int <procName>(struct seq_file *m, void *v)
    \attention This should be used over SEQ_PROC_DEFINE for short procs that surely will not exceed a page.
    \note <procName> can be different then the actual file name that will be used for the proc [see SEQ_PROC_CREATE].
*/
#define SEQ_PROC_DEFINE_SIMPLE(procName) \
/* Function Declarations: */             \
SEQ_PROC_DECLARE_OPEN(procName);         \
SEQ_PROC_DECLARE_SIMPLE(procName);       \
/* Define OPs structures: */             \
SEQ_PROC_DEFINE_SIMPLE_FOPS(procName);   \
/* no need for SEQ_OPS for simple */     \
/* Define Functions:  */                 \
SEQ_PROC_DEFINE_SIMPLE_OPEN(procName);

/*! \def SEQ_PROC_CREATE(procName, procDir)
    \brief macro that will create a generic seq proc file.
    \param[in] procFileName - string, actual name of the seq proc file to be created.
    \param[in] procName - name designation of the seq proc, that was used with SEQ_PROC_DEFINE/SEQ_PROC_DEFINE_SIMPLE.
    \param[in] procDir - struct proc_dir_entry * of the directory that the seq proc file should be created in.
    \attention Corresponding call with same <procName> to SEQ_PROC_DEFINE/SEQ_PROC_DEFINE_SIMPLE macro must exist!
    \attention On unlikely fail return -1 to exit calling function with fail (assumes int type calling function).
    \note <procFileName> is the name of the file that will be used with shell's # cat command.
*/
#define SEQ_PROC_CREATE(procFileName, procName, procDir) \
if (NULL == (proc_create_data(procFileName, 0, procDir, &CONCAT2(procName, _fops), NULL))) \
{                                                                                          \
    PRINTK_ERR("Can't create proc %s!", procFileName);                                     \
    return -1;                                                                             \
}

/*! \def PROC_CREATE_DIR(procDirName, procDir)
    \brief macro that will create a proc directory.
    \param[in] procDirName - string, directory name.
    \param[out] procDir - struct proc_dir_entry * for the directory.
    \attention On unlikely fail return -1 to exit calling function with fail (assumes int type calling function).
*/
#define PROC_CREATE_DIR(procDirName, procDir) \
if (NULL == (procDir = proc_mkdir(procDirName, NULL)))    \
{                                                         \
    PRINTK_ERR("Can't create proc dir %s!", procDirName); \
    return -1;                                            \
}

/************************************************************************/
/*     HW_MBOX Kernel Local Variables                                   */
/************************************************************************/

#ifdef HW_MBOX_DEBUG
/*! \var volatile Uint32 __debug_HW_MBOX_MID
    \brief enables changing MID during runtime.
    \note backing field to an extern variable in hw_mbox_mem_access.h
    \note starts off with the hardcoded MID.
    \attention don't access directly use the macros!
*/
volatile Uint32 __debug_HW_MBOX_MID = HW_MBOX_HARDCODED_MID;
#endif /* HW_MBOX_DEBUG */

#ifdef HW_MBOX_SOCKETS_API
/*! \var struct hlist_head hwMbox_sock_hlist
    \brief hash list for sockets of AF_HWMBOX family.
*/
struct hlist_head hwMbox_sock_hlist = { .first = NULL };

/*! \var static struct semaphore hwMbox_sock_hlist_sem
    \brief lock for /proc/hw_mbox/sockstat (binary semaphore).
*/
struct semaphore hwMbox_sock_hlist_sem;
#endif /* HW_MBOX_SOCKETS_API */

/*! \var static Uint32 __currentNonallocatedSramOffset
    \brief keeps track of offset of currently allocated SRAM.
    \warning Do not modify directly use the Macros!
*/
static Uint32 __currentNonallocatedSramOffset = 0;

/*! \var static chnIntr_t chnIntrData[]
    \brief array containing channel interrupts information.
    \note index in the array is CID.
*/
static chnIntr_t chnIntrData[HW_MBOX_CHANNEL_COUNT];

#define RESET_CHN_INTR_DATA()               (memset(chnIntrData, 0, sizeof(chnIntrData)))
#define CHN_INTR_WQ( _cid )                 (chnIntrData[ _cid ].waitQueue)
#define CHN_INTR_GET_STATE( _cid )          (chnIntrData[ _cid ].state)
#define CHN_INTR_SET_STATE( _cid, _value )  (chnIntrData[ _cid ].state = ( _value ))

/*! \var static struct semaphore chnLocks[]
    \brief array containing channel locks (binary semaphores).
    \note index in the array is CID.
*/
static struct semaphore chnLocks[HW_MBOX_CHANNEL_COUNT];

#define RESET_CHN_LOCKS()                   (memset(chnLocks, 0, sizeof(chnLocks)))
#define CHN_LOCK( _cid )                    (chnLocks[ _cid ])

/*** CID Look Up Table: ***/

static Uint32 cidLUT[HW_MBOX_MASTER_COUNT][HW_MBOX_TYPE_COUNT << 1];

/* This LUT allows to find the correct CID for each peer according to type.
   It is required because the channels are an internal entity and only have
   meaning inside the HW MBOX package code. Outside users invoke services
   specifying a destination peer (Commander/Subject).

   Table should be accessed using FIND_CID_IN_LUT/SET_CID_IN_LUT Macros.

   First dimension is the destination peer (indexed by MID).
   Second dimension is the required channel type (implicitly specified
   according to service that was invoked).

   Please note: second dimension size is HW_MBOX_TYPE_COUNT << 1 ( * 2 )
                because one CID is required for send variants (peer is subject);
                and another CID is required for recv variants (peer is commander).

   Currently per each < Commander, Subject, type > combination exist no more than one CID.
*/

#define RESET_CID_LUT()                                         (memset(cidLUT, 0xFF, sizeof(cidLUT)))
#define FIND_CID_IN_LUT( _peer, _type, _isPeerCommander)        (cidLUT[ _peer ][ ( ( _type ) << 1 ) + ( _isPeerCommander ) ])
#define SET_CID_IN_LUT( _peer, _type, _isPeerCommander, _value) (cidLUT[ _peer ][ ( ( _type ) << 1 ) + ( _isPeerCommander ) ] = ( _value ))

/*! \def CHECK_FIND_CID_IN_LUT( _peer, _type, _isPeerCommander, __onNotFound, _paramCID)
    \brief looks for CID in LUT, checks if found, if not executes __onNotFound.
    \param[in] _peer - peer of the channel, value from <hw_mbox_Masters_e>.
    \param[in] _type - channel type, value from <hw_mbox_type_e>.
    \param[in] _isPeerCommander - is <peer> the channel's commander.
    \param[in] __onNotFound - execute if CID not found.
    \param[out] _paramCID - upon returning will hold the CID [or HW_MBOX_CID_INVALID if not found].
*/
#define CHECK_FIND_CID_IN_LUT( _peer, _type, _isPeerCommander, __onNotFound, _paramCID) \
do{ \
    ( _paramCID ) = FIND_CID_IN_LUT(_peer, _type, _isPeerCommander);           \
    if (( _paramCID ) == HW_MBOX_CID_INVALID)                                  \
    {                                                                          \
        DPRINTK("There's no %s channel configured with %s(%d) as %s...",       \
                HW_MBOX_TYPE_STR( _type ),                                     \
                HW_MBOX_MASTER_GET_NAME( _peer ), ( _peer ),                   \
                (( _isPeerCommander )?("commander"):("subject")));             \
        __onNotFound;                                                          \
    }                                                                          \
    DPRINTK("found correct cid, using cid %d", _paramCID);                     \
} while (0)

#define CID_LUT_PEER_IS_COMMANDER   ( True )
#define CID_LUT_PEER_IS_SUBJECT     ( False )

/*! \var static recvOpcodeListenerThreadData_t recvOpThreadsData[]
    \brief data of the receive opcode listeners.
*/
static recvOpcodeListenerThreadData_t recvOpThreadsData[HW_MBOX_OPCODE_RECV_COUNT];

#define RESET_RECV_OP_THREAD_DATA()    (memset(recvOpThreadsData, 0, sizeof(recvOpThreadsData)))

#define RECV_OP_THREAD_DATA( _recvOpIndex )                  (recvOpThreadsData[ _recvOpIndex ])

#define RECV_OP_THREAD_SET_CID( _recvOpIndex, _paramCID )    (recvOpThreadsData[ _recvOpIndex ].cid = ( _paramCID ))
#define RECV_OP_THREAD_SET_INDEX( _recvOpIndex )             (recvOpThreadsData[ _recvOpIndex ].index = ( _recvOpIndex ))

/*! \var static recvOpcodeInfrastructure_t recvOpData[]
    \brief required infrastructure for receive opcode channels.
*/
static recvOpcodeInfrastructure_t recvOpData[HW_MBOX_OPCODE_RECV_COUNT];

#ifdef HW_MBOX_SOCKETS_API
#define RESET_RECV_OP_DATA() \
do{ \
    int __i,__k;                                                                \
    memset(recvOpData, 0, sizeof(recvOpData));                                  \
    for ( __i = 0; __i < HW_MBOX_OPCODE_RECV_COUNT; __i++ )                     \
    {                                                                           \
        for ( __k = 0; __k < HW_MBOX_OPCODE_TAG_COUNT; __k++ )                  \
        {                                                                       \
            recvOpData[ __i ].registeredTags[ __k ].tag = HW_MBOX_TAG_INVALID;  \
        }                                                                       \
    }                                                                           \
} while (0)
#else
#define RESET_RECV_OP_DATA()    (memset(recvOpData, 0, sizeof(recvOpData)))
#endif

/* Access By Receive Op Index Macros: */

/* Listener Thread: */
#define RECV_OP_THREAD_BY_INDEX( _recvOpIndex )                         (recvOpData[ _recvOpIndex ].listenerThread)

/* Regsitered Count: */
#define RECV_OP_GET_TAG_COUNT_BY_INDEX( _recvOpIndex )                  (recvOpData[ _recvOpIndex ].registeredCount)
#define RECV_OP_INC_TAG_COUNT_BY_INDEX( _recvOpIndex )                  (recvOpData[ _recvOpIndex ].registeredCount++)

#ifdef HW_MBOX_SOCKETS_API
# define RECV_OP_DEC_TAG_COUNT_BY_INDEX( _recvOpIndex )                  (recvOpData[ _recvOpIndex ].registeredCount--)
#endif /* HW_MBOX_SOCKETS_API */

/* Application Queue: */
#define RECV_OP_APP_WQ_BY_INDEX( _recvOpIndex )                         (recvOpData[ _recvOpIndex ].appWq.waitQueue)
#define RECV_OP_APP_GET_STATE_BY_INDEX( _recvOpIndex )                  (recvOpData[ _recvOpIndex ].appWq.state)
#define RECV_OP_APP_SET_STATE_BY_INDEX( _recvOpIndex, _stateVal )       (recvOpData[ _recvOpIndex ].appWq.state = ( _stateVal ))

/* Tag: */
#define RECV_OP_GET_TAG_BY_INDEX( _recvOpIndex, _tagIndex )             (recvOpData[ _recvOpIndex ].registeredTags[ _tagIndex ].tag)
#define RECV_OP_SET_TAG_BY_INDEX( _recvOpIndex, _tagIndex, _tagVal )    (recvOpData[ _recvOpIndex ].registeredTags[ _tagIndex ].tag = ( _tagVal ))

/* Callback: */
#define RECV_OP_GET_CB_BY_INDEX( _recvOpIndex, _tagIndex )              (recvOpData[ _recvOpIndex ].registeredTags[ _tagIndex ].callbackFunc)
#define RECV_OP_SET_CB_BY_INDEX( _recvOpIndex, _tagIndex, _cbPtr)       (recvOpData[ _recvOpIndex ].registeredTags[ _tagIndex ].callbackFunc = ( _cbPtr ))

/* Token: */
#define RECV_OP_GET_TOKEN_BY_INDEX( _recvOpIndex, _tagIndex)            (recvOpData[ _recvOpIndex ].registeredTags[ _tagIndex ].token)
#define RECV_OP_SET_TOKEN_BY_INDEX( _recvOpIndex, _tagIndex, _tokenVal) (recvOpData[ _recvOpIndex ].registeredTags[ _tagIndex ].token = ( _tokenVal ))

/* Statistics: */
#define RECV_OP_GET_WAKEUP_COUNT_BY_INDEX( _recvOpIndex )               (recvOpData[ _recvOpIndex ].wakeupCount)
#define RECV_OP_INC_WAKEUP_COUNT_BY_INDEX( _recvOpIndex )               (recvOpData[ _recvOpIndex ].wakeupCount++)

#define RECV_OP_GET_INVALID_TAG_COUNT_BY_INDEX( _recvOpIndex )          (recvOpData[ _recvOpIndex ].invalidTagCount)
#define RECV_OP_INC_INVALID_TAG_COUNT_BY_INDEX( _recvOpIndex )          (recvOpData[ _recvOpIndex ].invalidTagCount++)

#define RECV_OP_GET_OPCODE_CNT_BY_INDEX( _recvOpIndex, _tagIndex)       (recvOpData[ _recvOpIndex ].registeredTags[ _tagIndex ].opcodeCount)
#define RECV_OP_INC_OPCODE_CNT_BY_INDEX( _recvOpIndex, _tagIndex)       (recvOpData[ _recvOpIndex ].registeredTags[ _tagIndex ].opcodeCount++)
#define RECV_OP_CLR_OPCODE_CNT_BY_INDEX( _recvOpIndex, _tagIndex)       (recvOpData[ _recvOpIndex ].registeredTags[ _tagIndex ].opcodeCount = 0)

/* Access By CID Macros: */
/* IMPORTANT: Only use the following functions for valid recv Opcode channels! */

/* Regsitered Count: */
#define RECV_OP_GET_TAG_COUNT( _cid )                   (RECV_OP_GET_TAG_COUNT_BY_INDEX( CHN_RECV_OPCODE_GET_INDEX( _cid ) ))
#define RECV_OP_INC_TAG_COUNT( _cid )                   (RECV_OP_INC_TAG_COUNT_BY_INDEX( CHN_RECV_OPCODE_GET_INDEX( _cid ) ))

#ifdef HW_MBOX_SOCKETS_API
#define RECV_OP_DEC_TAG_COUNT( _cid )                   (RECV_OP_DEC_TAG_COUNT_BY_INDEX( CHN_RECV_OPCODE_GET_INDEX( _cid ) ))
#endif /* HW_MBOX_SOCKETS_API */

/* Application Queue: */
#define RECV_OP_APP_WQ( _cid )                          (RECV_OP_APP_WQ_BY_INDEX( CHN_RECV_OPCODE_GET_INDEX( _cid ) ))
#define RECV_OP_APP_GET_STATE( _cid )                   (RECV_OP_APP_GET_STATE_BY_INDEX( CHN_RECV_OPCODE_GET_INDEX( _cid ) ))
#define RECV_OP_APP_SET_STATE( _cid, _stateVal )        (RECV_OP_APP_SET_STATE_BY_INDEX( CHN_RECV_OPCODE_GET_INDEX( _cid ), ( _stateVal ) ))

/* Tag: */
#define RECV_OP_GET_TAG( _cid, _tagIndex )              (RECV_OP_GET_TAG_BY_INDEX( CHN_RECV_OPCODE_GET_INDEX( _cid ), ( _tagIndex ) ))
#define RECV_OP_SET_TAG( _cid, _tagIndex, _tagVal)      (RECV_OP_SET_TAG_BY_INDEX( CHN_RECV_OPCODE_GET_INDEX( _cid ), ( _tagIndex ), ( _tagVal ) ))

/* Callback: */
#define RECV_OP_GET_CB( _cid, _tagIndex )               (RECV_OP_GET_CB_BY_INDEX( CHN_RECV_OPCODE_GET_INDEX( _cid ), ( _tagIndex ) ))
#define RECV_OP_SET_CB( _cid, _tagIndex, _cbPtr)        (RECV_OP_SET_CB_BY_INDEX( CHN_RECV_OPCODE_GET_INDEX( _cid ), ( _tagIndex ), ( _cbPtr ) ))

/* Token: */
#define RECV_OP_GET_TOKEN( _cid, _tagIndex )            (RECV_OP_GET_TOKEN_BY_INDEX( CHN_RECV_OPCODE_GET_INDEX( _cid ), ( _tagIndex ) ))
#define RECV_OP_SET_TOKEN( _cid, _tagIndex, _tokenVal)  (RECV_OP_SET_TOKEN_BY_INDEX( CHN_RECV_OPCODE_GET_INDEX( _cid ), ( _tagIndex ), ( _tokenVal ) ))

/* Statistics: */
#define RECV_OP_GET_WAKEUP_COUNT( _cid )                (RECV_OP_GET_WAKEUP_COUNT_BY_INDEX( CHN_RECV_OPCODE_GET_INDEX( _cid ) ))
#define RECV_OP_INC_WAKEUP_COUNT( _cid )                (RECV_OP_INC_WAKEUP_COUNT_BY_INDEX( CHN_RECV_OPCODE_GET_INDEX( _cid ) ))

#define RECV_OP_GET_INVALID_TAG_COUNT( _cid )           (RECV_OP_GET_INVALID_TAG_COUNT_BY_INDEX( CHN_RECV_OPCODE_GET_INDEX( _cid ) ))
#define RECV_OP_INC_INVALID_TAG_COUNT( _cid )           (RECV_OP_INC_INVALID_TAG_COUNT_BY_INDEX( CHN_RECV_OPCODE_GET_INDEX( _cid ) ))

#define RECV_OP_GET_OPCODE_CNT( _cid, _tagIndex )       (RECV_OP_GET_OPCODE_CNT_BY_INDEX( CHN_RECV_OPCODE_GET_INDEX( _cid ), ( _tagIndex ) ))
#define RECV_OP_INC_OPCODE_CNT( _cid, _tagIndex )       (RECV_OP_INC_OPCODE_CNT_BY_INDEX( CHN_RECV_OPCODE_GET_INDEX( _cid ), ( _tagIndex ) ))
#define RECV_OP_CLR_OPCODE_CNT( _cid, _tagIndex )       (RECV_OP_CLR_OPCODE_CNT_BY_INDEX( CHN_RECV_OPCODE_GET_INDEX( _cid ), ( _tagIndex ) ))

/*** Kernel Statistics: ***/

static hwMbox_statistics_t stats;

#define RESET_STATS()                                         (memset(&stats, 0, sizeof(stats)))
#define STATS_GET_COUNTER( _peer, _type, _isPeerCommander)    (stats.serviceCounters[ _peer ][ ( ( _type ) << 1 ) + ( _isPeerCommander ) ])
#define STATS_INC_COUNTER( _peer, _type, _isPeerCommander)    (stats.serviceCounters[ _peer ][ ( ( _type ) << 1 ) + ( _isPeerCommander ) ]++)
#define STATS_DEC_COUNTER( _peer, _type, _isPeerCommander)    (stats.serviceCounters[ _peer ][ ( ( _type ) << 1 ) + ( _isPeerCommander ) ]--)

#define STATS_PEER_IS_COMMANDER   ( True )
#define STATS_PEER_IS_SUBJECT     ( False )

/*** High Level Function Helpers ***/

/*! \def WAIT_FOR_CHN_INTR( _cid, _timeOutInMsec)
    \brief invokes waitForChannelInterrupt function.
    \attention releases channel lock on error.
    \note assumes int calling function.
    \param[in] _cid - CID to wait for.
    \param[in] _timeOutInMsec - Timeout value (use CHN_INTR_NO_TIMEOUT for no timeout).
*/
#define WAIT_FOR_CHN_INTR( _cid, _timeOutInMsec ) \
do{ \
    HW_MBOX_STATUS_e hwMboxStatus;                                                      \
    if (hwMbox_waitForChannelInterrupt( ( _cid ), ( _timeOutInMsec ), &hwMboxStatus))   \
    {                                                                                   \
        /* release Lock... */                                                       \
        up(&CHN_LOCK( ( _cid )));                                                   \
        if (hwMboxStatus == HW_MBOX_STATUS_INTERRUPTED_BY_SIGNAL)                   \
        {                                                                           \
            DPRINTK("cid %d: wait interrupted by signal!", ( _cid ));               \
            return -ERESTARTSYS;                                                    \
        }                                                                           \
        else if (hwMboxStatus == HW_MBOX_STATUS_ERROR_TIMEOUT_EXPIRED)              \
        {                                                                           \
            /* TBD: what to do in this case... */                                   \
            PRINTK_ERR("cid %d: timeout expired!", ( _cid ));                       \
            return -ETIMEDOUT;                                                      \
        }                                                                           \
    }                                                                               \
} while (0)

/************************************************************************/
/*     HW_MBOX Kernel INTERFACE FUNCTIONS Implementation:               */
/************************************************************************/

/**************************************************************************/
/*! \fn int hwMbox_isReady(void)
 **************************************************************************
 *  \brief Returns whether HW MBOX driver is initailized or not.
 *  \return [ 0 on initialized / -1 on NOT initialized ].
 **************************************************************************/
int hwMbox_isReady(void)
{
    CHECK_IS_DRV_INITIALIZED(return (-1));
    return 0;
}
EXPORT_SYMBOL(hwMbox_isReady);

/**************************************************************************/
/*! \fn int hwMbox_getChannelConfig(hw_mbox_Masters_e peer,
                                    hw_mbox_type_e mboxType,
                                    Bool isPeerCommander,
                                    hw_mbox_channelConfig_t *cfg)
 **************************************************************************
 *  \brief Get channel configuration.
 *  \param[in] peer - channel peer .
 *  \param[in] mboxType - channel type.
 *  \param[in] isPeerCommander - indicates whether the peer is the commander
 *                               of the channel.
 *  \param[out] cfg - channel configuration, if found.
 *  \return [ 0 on success / error otherwise ].
 **************************************************************************/
int hwMbox_getChannelConfig(hw_mbox_Masters_e peer,
                            hw_mbox_type_e mboxType,
                            Bool isPeerCommander,
                            hw_mbox_channelConfig_t *cfg)
{
    Uint32 paramCID;

    if (cfg == NULL)
    {
        PRINTK_ERR("cfg == NULL!");
        return (-1);
    }

    if (!HW_MBOX_MID_VALIDITY_CHECK(peer))
    {
        PRINTK_ERR("invalid mid value! [peer = %d]",
                   peer);
        return (-1);
    }

    if (!HW_MBOX_TYPE_IS_VALID(mboxType))
    {
        PRINTK_ERR("invalid mbox type value! [mboxType = %d]",
                   mboxType);
        return (-1);
    }

    CHECK_IS_DRV_INITIALIZED(return (-1));

    CHECK_FIND_CID_IN_LUT(peer,
                          mboxType,
                          isPeerCommander,
                          return (-1),
                          paramCID);

    memcpy(cfg, &CHN_CFG(paramCID), HW_MBOX_CHANNEL_CONFIG_SIZE);

    return 0;
}
EXPORT_SYMBOL(hwMbox_getChannelConfig);

/**************************************************************************/
/*! \fn int hwMbox_sendCommand(hw_mbox_Masters_e subject,
                               Uint32 *cmd,
                               Uint8 *dataBuf, Uint32 dataLen)
 **************************************************************************
 *  \brief Send a command to <subject> in a blocking manner using the HW_MBOX.
 *  \attention will only return upon reply/ACK from <subject> (or error).
 *  \param[in]     subject - To which Master should the opcode be sent.
 *  \param[in,out] cmd - Command to send, upon returning will hold the reply.
 *  \param[in,out] dataBuf - pointer to a Buffer containing more data to be
 *                           sent, upon returning will hold the reply data
 *                           Buffer (copies as much as the buffer can hold).
 *  \param[in]     dataLen - Length in Bytes of buffer specified in <dataBuf>.
 *  \return [ 0 on success / error otherwise ].
 **************************************************************************/
int hwMbox_sendCommand(hw_mbox_Masters_e subject,
                       Uint32 *cmd,
                       Uint8 *dataBuf, Uint32 dataLen)
{
    Uint32 paramCID;

    CHECK_IS_DRV_INITIALIZED(return (-1));

    if (cmd == NULL)
    {
        PRINTK_ERR("cmd == NULL!");
        return (-1);
    }

    DPRINTK("send Command to %s(%d) [command value 0x%08x with %uB of data]",
            HW_MBOX_MASTER_GET_NAME(subject), subject,
            *cmd, dataLen);

    CHECK_FIND_CID_IN_LUT(subject, hw_mbox_command, CID_LUT_PEER_IS_SUBJECT, return (-1), paramCID);

    if (!IS_MASTER_INIT(subject))
    {
        DPRINTK("send Command to %s(%d) - subject not initialized!",
                HW_MBOX_MASTER_GET_NAME(subject), subject);
        return (-1);
    }

    /* seize channel lock */
    if (down_interruptible(&CHN_LOCK(paramCID)))
    {
        DPRINTK("down interrupted by signal...");
        return -ERESTARTSYS;
    }

    if ((dataBuf != NULL) && (dataLen > 0))
    {
        /* command has data! save to SRAM: */

        /* verify SRAM size: */
        if (dataLen > CHN_CFG_MEMBUFF_GET_SIZE(paramCID))
        {
            PRINTK_ERR("send Command to %s(%d) - Command data Buffer size (%uB) > SRAM size allocated to channel (%uB)!",
                       HW_MBOX_MASTER_GET_NAME(subject), subject, dataLen, CHN_CFG_MEMBUFF_GET_SIZE(paramCID));

            /* release Lock... */
            up(&CHN_LOCK(paramCID));
            return (-1);
        }

        DPRINTK("send Command to %s(%d) - copying %uB of command data to SRAM",
                HW_MBOX_MASTER_GET_NAME(subject), subject, dataLen);

        DPRINTK("cid %d: copying %uB [out of total %uB channel has] to SRAM at offset=0x%08x",
                paramCID, dataLen, CHN_CFG_MEMBUFF_GET_SIZE(paramCID), CHN_CFG_MEMBUFF_GET_START(paramCID));

        HW_MBOX_COPY_TO_SRAM(dataBuf, dataLen, CHN_CFG_MEMBUFF_GET_START(paramCID));
    }

    /* Send Command to subject: */
    DPRINTK("cid %d: before write to command register - state=%d", paramCID, HW_MBOX_CHN_STATE_REG_GET(paramCID));
    DPRINTK("cid %d: writing 0x%08x to the command register of the channel...", paramCID, *cmd);
    HW_MBOX_CHN_COMMAND_REG_SET(paramCID, *cmd);
    DPRINTK("cid %d: after write to command register - state=%d", paramCID, HW_MBOX_CHN_STATE_REG_GET(paramCID));

    WAIT_FOR_CHN_INTR(paramCID, CHN_INTR_DEFAULT_TIMEOUT_MSEC);

    DPRINTK("cid %d: Reply/ACK from subject!", paramCID);

    /* get the Reply: */
    *cmd = HW_MBOX_CHN_COMMAND_REG_GET(paramCID);
    DPRINTK("cid %d: subject reply=0x%08x", paramCID, *cmd);

    if ((dataBuf != NULL) && (dataLen > 0))
    {
        /* get reply data from SRAM: */
        DPRINTK("send Command to %s(%d) - copying %uB of reply command data from SRAM",
                HW_MBOX_MASTER_GET_NAME(subject), subject, dataLen);

        DPRINTK("cid %d: copying %uB [out of total %uB channel has] from SRAM at offset=0x%08x",
                paramCID, dataLen, CHN_CFG_MEMBUFF_GET_SIZE(paramCID), CHN_CFG_MEMBUFF_GET_START(paramCID));

        HW_MBOX_COPY_FROM_SRAM(CHN_CFG_MEMBUFF_GET_START(paramCID), dataLen, dataBuf);
    }

    STATS_INC_COUNTER(subject, hw_mbox_command, STATS_PEER_IS_SUBJECT);

    /* release Lock... */
    up(&CHN_LOCK(paramCID));

    return 0;
}
EXPORT_SYMBOL(hwMbox_sendCommand);

/**************************************************************************/
/*! \fn int hwMbox_recvCommand(hw_mbox_Masters_e commander,
                               Uint32 *cmd,
                               Uint8 *dataBuf, Uint32 dataLen)
 **************************************************************************
 *  \brief Receive a command from <commander> using the HW_MBOX.
 *  \attention will only return upon arrival of a command from <commander>
 *             (or error).
 *  \note To reply call HW_MBOX_sendReply [if a meaningful response is expected].
 *  \note To ACK call HW_MBOX_sendAck [to simply ACK the command].
 *  \warning The subsequent call to HW_MBOX_sendReply/HW_MBOX_sendAck is extermely
 *           important as until it is called the channel is not released!!!
 *  \param[in]  commander - From which Master should a command be received.
 *  \param[out] cmd - Command that was received.
 *  \param[out] dataBuf - pointer to a Buffer containing more data that was sent.
 *  \param[in]  dataLen - Length in Bytes of buffer specified in <dataBuf>.
 *  \return [ 0 on success / error otherwise ].
 **************************************************************************/
int hwMbox_recvCommand(hw_mbox_Masters_e commander,
                       Uint32 *cmd,
                       Uint8 *dataBuf, Uint32 dataLen)
{
    Uint32 paramCID;

    CHECK_IS_DRV_INITIALIZED(return (-1));

    if (cmd == NULL)
    {
        PRINTK_ERR("cmd == NULL!");
        return (-1);
    }

    DPRINTK("receive Command from %s(%d) [incoming data buffer size %uB]",
            HW_MBOX_MASTER_GET_NAME(commander), commander, dataLen);

    CHECK_FIND_CID_IN_LUT(commander, hw_mbox_command, CID_LUT_PEER_IS_COMMANDER, return (-1), paramCID);

    if (dataBuf != NULL)
    {
        if (dataLen > CHN_CFG_MEMBUFF_GET_SIZE(paramCID))
        {
            /* verify SRAM size: */
            PRINTK_ERR("receive Command from %s(%d) - incoming data buffer size (%uB) > SRAM allocated to channel (%uB)!",
                       HW_MBOX_MASTER_GET_NAME(commander), commander, dataLen, CHN_CFG_MEMBUFF_GET_SIZE(paramCID));

            return (-1);
        }
    }

    /* seize channel lock
       [handle possible simultaneous access from both User-Space and Kernel] */
    if (down_interruptible(&CHN_LOCK(paramCID)))
    {
        DPRINTK("down interrupted by signal...");
        return -ERESTARTSYS;
    }

    DPRINTK("receive Command from %s(%d) - start waiting for commands...",
            HW_MBOX_MASTER_GET_NAME(commander), commander);

    DPRINTK("cid %d: before wait - state=%d", paramCID, HW_MBOX_CHN_STATE_REG_GET(paramCID));

    WAIT_FOR_CHN_INTR(paramCID, CHN_INTR_NO_TIMEOUT);

    DPRINTK("cid %d: after wait - state=%d", paramCID, HW_MBOX_CHN_STATE_REG_GET(paramCID));

    /* get the Command: */
    *cmd = HW_MBOX_CHN_COMMAND_REG_GET(paramCID);

    DPRINTK("receive Command from %s(%d) - New command received from commander!",
            HW_MBOX_MASTER_GET_NAME(commander), commander);

    if ((dataBuf != NULL) && (dataLen > 0))
    {
        if (CHN_CFG_MEMBUFF_GET_SIZE(paramCID) > 0)
        {
            /* get command data from SRAM: */
            DPRINTK("receive Command from %s(%d) - copying %uB of command data from SRAM",
                     HW_MBOX_MASTER_GET_NAME(commander), commander, dataLen);

            DPRINTK("cid %d: copying %uB [out of total %uB channel has] from SRAM at offset=0x%08x",
                    paramCID, dataLen, CHN_CFG_MEMBUFF_GET_SIZE(paramCID), CHN_CFG_MEMBUFF_GET_START(paramCID));

            HW_MBOX_COPY_FROM_SRAM(CHN_CFG_MEMBUFF_GET_START(paramCID), dataLen, dataBuf);
        }
    }

    DPRINTK("receive Command from %s(%d) - command value 0x%08x (%u) with %uB of data",
            HW_MBOX_MASTER_GET_NAME(commander), commander, *cmd, *cmd, dataLen);

    STATS_INC_COUNTER(commander, hw_mbox_command, STATS_PEER_IS_COMMANDER);

    /* release Lock... */
    up(&CHN_LOCK(paramCID));

    return 0;
}
EXPORT_SYMBOL(hwMbox_recvCommand);

/**************************************************************************/
/*! \fn int hwMbox_sendReplyCommand(hw_mbox_Masters_e commander,
                                    Uint32 reply,
                                    Uint8 *dataBuf, Uint32 dataLen)
 **************************************************************************
 *  \brief Send back a reply to <commander>, from which a command has been
 *         received using the HW_MBOX.
 *  \note should be called when a meaningful response is expected.
 *  \attention should only be used after a command has been received from
 *             <commander> using hwMbox_recvCommand.
 *  \param[in] commander - To which Master should the reply be sent.
 *  \param[in] reply - reply to send.
 *  \param[in] dataBuf - pointer to a Buffer containing more data to be sent.
 *  \param[in] dataLen - Length in Bytes of buffer specified in <dataBuf>.
 *  \return [ 0 on success / error otherwise ].
 **************************************************************************/
int hwMbox_sendReplyCommand(hw_mbox_Masters_e commander,
                            Uint32 reply,
                            Uint8 *dataBuf, Uint32 dataLen)
{
    Uint32 paramCID;

    CHECK_IS_DRV_INITIALIZED(return (-1));

    DPRINTK("send Reply Command to %s(%d) [reply value 0x%08x with %uB of data]",
            HW_MBOX_MASTER_GET_NAME(commander), commander,
            reply, dataLen);

    CHECK_FIND_CID_IN_LUT(commander, hw_mbox_command, CID_LUT_PEER_IS_COMMANDER, return (-1), paramCID);

    /* seize channel lock
       [handle possible simultaneous access from both User-Space and Kernel] */
    if (down_interruptible(&CHN_LOCK(paramCID)))
    {
        DPRINTK("down interrupted by signal...");
        return -ERESTARTSYS;
    }

    if ((dataBuf != NULL) && (dataLen > 0))
    {
        /* reply has data! save to SRAM: */

        /* verify SRAM size: */
        if (dataLen > CHN_CFG_MEMBUFF_GET_SIZE(paramCID))
        {
            PRINTK_ERR("send Reply Command to %s(%d) - Reply Command Data size (%uB) > SRAM size allocated to channel (%uB)!",
                       HW_MBOX_MASTER_GET_NAME(commander), commander, dataLen, CHN_CFG_MEMBUFF_GET_SIZE(paramCID));

            /* release Lock... */
            up(&CHN_LOCK(paramCID));
            return (-1);
        }

        DPRINTK("send Reply Command to %s(%d) - copying %uB of reply command data to SRAM",
                HW_MBOX_MASTER_GET_NAME(commander), commander, dataLen);

        DPRINTK("cid %d: copying %uB [out of total %uB channel has] to SRAM at offset=0x%08x",
                paramCID, dataLen, CHN_CFG_MEMBUFF_GET_SIZE(paramCID), CHN_CFG_MEMBUFF_GET_START(paramCID));

        HW_MBOX_COPY_TO_SRAM(dataBuf, dataLen, CHN_CFG_MEMBUFF_GET_START(paramCID));
    }

    /* Send Reply to commander: */
    DPRINTK("send Reply Command to %s(%d) - sending reply to commander!",
            HW_MBOX_MASTER_GET_NAME(commander), commander);

    DPRINTK("cid %d: before write to command register - state=%d", paramCID, HW_MBOX_CHN_STATE_REG_GET(paramCID));
    DPRINTK("cid %d: writing 0x%08x to the command register of the channel...", paramCID, reply);
    HW_MBOX_CHN_COMMAND_REG_SET(paramCID, reply);
    DPRINTK("cid %d: after write to command register - state=%d", paramCID, HW_MBOX_CHN_STATE_REG_GET(paramCID));

    /* release Lock... */
    up(&CHN_LOCK(paramCID));

    return 0;
}
EXPORT_SYMBOL(hwMbox_sendReplyCommand);

/**************************************************************************/
/*! \fn int hwMbox_sendAckCommand(hw_mbox_Masters_e commander)
 **************************************************************************
 *  \brief Send back an ACK to <commander>, from which a command has been
 *         received using the HW_MBOX.
 *  \note should be called when no meaningful response is expected in order
 *        to simply ACK the command.
 *  \note Using this function leaves the command register's value unchanged
 *        (writes the same value that was received).
 *  \attention should only be used after a command has been received from
 *             <commander> using hwMbox_recvCommand.
 *  \param[in] commander - To which Master should the reply be sent.
 *  \return [ 0 on success / error otherwise ].
 **************************************************************************/
int hwMbox_sendAckCommand(hw_mbox_Masters_e commander)
{
    Uint32 paramCID, currCmdRegVal;

    CHECK_IS_DRV_INITIALIZED(return (-1));

    DPRINTK("send ACK Command to %s(%d)", HW_MBOX_MASTER_GET_NAME(commander), commander);

    CHECK_FIND_CID_IN_LUT(commander, hw_mbox_command, CID_LUT_PEER_IS_COMMANDER, return (-1), paramCID);

    /* seize channel lock
       [handle possible simultaneous access from both User-Space and Kernel] */
    if (down_interruptible(&CHN_LOCK(paramCID)))
    {
        DPRINTK("down interrupted by signal...");
        return -ERESTARTSYS;
    }

    /* write the current command (command that was sent),
       to better assist the HW_MBOX Monitor... */

    currCmdRegVal = HW_MBOX_CHN_COMMAND_REG_GET(paramCID);

    /* Send ACK to commander: */
    DPRINTK("send ACK Command to %s(%d) - sending ACK to commander!",
            HW_MBOX_MASTER_GET_NAME(commander), commander);

    DPRINTK("cid %d: before write to command register - state=%d", paramCID, HW_MBOX_CHN_STATE_REG_GET(paramCID));
    DPRINTK("cid %d: writing 0x%08x to the command register of the channel...", paramCID, currCmdRegVal);
    HW_MBOX_CHN_COMMAND_REG_SET(paramCID, currCmdRegVal);
    DPRINTK("cid %d: after write to command register - state=%d", paramCID, HW_MBOX_CHN_STATE_REG_GET(paramCID));

    /* release Lock... */
    up(&CHN_LOCK(paramCID));

    return 0;
}
EXPORT_SYMBOL(hwMbox_sendAckCommand);

/**************************************************************************/
/*! \fn int hwMbox_sendOpcode(hw_mbox_Masters_e subject,
                              Uint32 tag,
                              Uint8 *dataBuf, Uint32 *dataLen,
                              Uint32 opcodeDataLen,
                              Uint32 *replyDataLen)
 **************************************************************************
 *  \brief Send an opcode to <subject> in a blocking manner using the HW_MBOX.
 *  \attention will only return upon reply from <subject> (or error).
 *  \attention specified buffer should be able to hold generic messages that
 *             sent on opcode channels, meaning size should be at least
 *             HW_MBOX_OPCODE_MINIMAL_SRAM_ALLOCATION_IN_BYTES.
 *  \param[in]     subject - To which Master should the opcode be sent.
 *  \param[in]     tag - indicates opcode destination.
 *  \param[in,out] dataBuf - pointer to a Buffer containing opcode data to be sent,
 *                           upon returning will hold the reply data Buffer.
 *  \param[in]     dataLen - Length in Bytes of buffer specified in <dataBuf>.
 *  \param[in]     opcodeDataLen - Length in Bytes of opcode data to send.
 *  \param[out]    replyDataLen - upon returning will hold the length in Bytes
 *                                of returned reply data.
 *  \note if function returns -1, and replyDataLen outputs > 0 value then
 *        the user supplied buffer is populated with a <hwMbox_opcodeMessage_t>
 *        of type HW_MBOX_OPCODE_MESSAGE_TYPE_STATUS containing extended error
 *        information.
 *  \return [ 0 on success / error otherwise ].
 **************************************************************************/
int hwMbox_sendOpcode(hw_mbox_Masters_e subject,
                      Uint32 tag,
                      Uint8 *dataBuf, Uint32 dataLen,
                      Uint32 opcodeDataLen,
                      Uint32 *replyDataLen)
{
    Uint32 paramCID, cmdRegVal, tagMaxVal;
    hwMbox_opcode_t opFlds = { 0 };
    hwMbox_opcodeMessage_t *pOpcodeMsg;

    CHECK_IS_DRV_INITIALIZED(return (-1));

    if (replyDataLen == NULL)
    {
        PRINTK_ERR("replyDataLen == NULL!");
        return (-1);
    }

    /* Initialize with default output value: */
    *replyDataLen = 0;

    DPRINTK("send Opcode to %s(%d) [tag %u with %uB of data]",
            HW_MBOX_MASTER_GET_NAME(subject), subject,
            tag, opcodeDataLen);

    CHECK_FIND_CID_IN_LUT(subject, hw_mbox_opcode, CID_LUT_PEER_IS_SUBJECT, return (-1), paramCID);

    if (!IS_MASTER_INIT(subject))
    {
        DPRINTK("send Opcode to %s(%d) - subject not initialized!",
                HW_MBOX_MASTER_GET_NAME(subject), subject);
        return (-1);
    }

    DPRINTK("cid %d: size of buffer %uB", paramCID, dataLen);

    tagMaxVal = HW_MBOX_OPCODE_TAG_MAX_VALUE;
    if (tag > tagMaxVal)
    {
        PRINTK_ERR("send Opcode to %s(%d) - tag value (%u) > max possible value (%u)!",
                   HW_MBOX_MASTER_GET_NAME(subject), subject,
                   tag, tagMaxVal);
        return (-1);
    }

#ifdef HW_MBOX_KDBG
    if (tag >= HW_MBOX_OPCODE_RESERVED_TAGS_START)
    {
        DPRINTK("cid %d: NOTICE - using reserved HW_MBOX opcode tag %u", paramCID, tag);
    }
#endif /* HW_MBOX_KDBG */

    if ((dataBuf == NULL) || (dataLen == 0))
    {
        /* No Buffer specified! */
        PRINTK_ERR("send Opcode to %s(%d) - No Buffer specified! [dataBuf %p, dataLen %uB]",
                   HW_MBOX_MASTER_GET_NAME(subject), subject, dataBuf, dataLen);
        return (-1);
    }

    if (dataLen < HW_MBOX_OPCODE_MINIMAL_SRAM_ALLOCATION_IN_BYTES)
    {
        /* Insufficient Size Buffer specified! */
        PRINTK_ERR("send Opcode to %s(%d) - Insufficient Size Buffer specified! [dataLen %uB, minimal size is %dB]",
                   HW_MBOX_MASTER_GET_NAME(subject), subject,
                   dataLen, HW_MBOX_OPCODE_MINIMAL_SRAM_ALLOCATION_IN_BYTES);
        return (-1);
    }

    if (opcodeDataLen == 0)
    {
        /* No Data specified! */
        PRINTK_ERR("send Opcode to %s(%d) - No Data specified! [opcodeDataLen %uB]",
                   HW_MBOX_MASTER_GET_NAME(subject), subject, opcodeDataLen);
        return (-1);
    }

    if (opcodeDataLen > dataLen)
    {
        /* Opcode data longer than specified buffer... */
        PRINTK_ERR("send Opcode to %s(%d) - opcode data length (%uB) > specified buffer size (%uB)!",
                   HW_MBOX_MASTER_GET_NAME(subject), subject,
                   opcodeDataLen, dataLen);
        return (-1);
    }

    if ((opcodeDataLen % 4) != 0)
    {
        /* Opcode data is always a multiple of 4 Bytes! */
        PRINTK_ERR("send Opcode to %s(%d) - opcode data length (%uB) must be a multiple of 4B!",
                   HW_MBOX_MASTER_GET_NAME(subject), subject, opcodeDataLen);
        return (-1);
    }

    /* seize channel lock */
    if (down_interruptible(&CHN_LOCK(paramCID)))
    {
        DPRINTK("down interrupted by signal...");
        return -ERESTARTSYS;
    }

    /* Save Opcode data to SRAM: */

    /* verify SRAM size: */
    if (opcodeDataLen > CHN_CFG_MEMBUFF_GET_SIZE(paramCID))
    {
        PRINTK_ERR("send Opcode to %s(%d) - opcode data length (%uB) > SRAM allocated to channel (%uB)!",
                   HW_MBOX_MASTER_GET_NAME(subject), subject, opcodeDataLen, CHN_CFG_MEMBUFF_GET_SIZE(paramCID));

        /* release Lock... */
        up(&CHN_LOCK(paramCID));
        return (-1);
    }

    DPRINTK("send Opcode to %s(%d) - copying %uB of opcode data to SRAM",
            HW_MBOX_MASTER_GET_NAME(subject), subject, opcodeDataLen);

    DPRINTK("cid %d: copying %uB [out of total %uB channel has] to SRAM at offset=0x%08x",
            paramCID, opcodeDataLen, CHN_CFG_MEMBUFF_GET_SIZE(paramCID), CHN_CFG_MEMBUFF_GET_START(paramCID));

    HW_MBOX_COPY_TO_SRAM(dataBuf, opcodeDataLen, CHN_CFG_MEMBUFF_GET_START(paramCID));

    /* Build the opcode: */
    opFlds.tag = tag;
    opFlds.lenInBytes = opcodeDataLen;

    /* Create the opcode value: */
    HW_MBOX_OPCODE_FIELDS_SET_FROM_STRUCT(cmdRegVal, &opFlds);

    DPRINTK("cid %d: opcode fields: tag=%u, lenInBytes=%u -> cmdRegVal=0x%08x",
            paramCID, opFlds.tag, opFlds.lenInBytes, cmdRegVal);

    /* Send Opcode to subject: */

    DPRINTK("cid %d: before write to command register - state=%d", paramCID, HW_MBOX_CHN_STATE_REG_GET(paramCID));
    DPRINTK("cid %d: writing 0x%08x to the command register of the channel...", paramCID, cmdRegVal);
    HW_MBOX_CHN_COMMAND_REG_SET(paramCID, cmdRegVal);
    DPRINTK("cid %d: after write to command register - state=%d", paramCID, HW_MBOX_CHN_STATE_REG_GET(paramCID));

    WAIT_FOR_CHN_INTR(paramCID, CHN_INTR_DEFAULT_TIMEOUT_MSEC);

    DPRINTK("cid %d: Reply from subject!", paramCID);

    /* Get the opcode reply value: */
    cmdRegVal = HW_MBOX_CHN_COMMAND_REG_GET(paramCID);

    /* parse to opcode fields: */
    HW_MBOX_OPCODE_FIELDS_GET_TO_STRUCT(cmdRegVal, &opFlds);

    DPRINTK("cid %d: cmdRegVal=0x%08x -> opcode fields: tag=%u, lenInBytes=%u",
            paramCID, cmdRegVal, opFlds.tag, opFlds.lenInBytes);

    /* First check for internal replies: */
    if (opFlds.tag == HW_MBOX_OPCODE_TAG)
    {
        /* 3 possible options here:
             1. this was a HW MBOX TAG sendOpcode.
             2. this was not a HW MBOX sendOpcode but it failed and the
                data returned is a Status Message with the error code.
             3. this was not a HW MBOX sendOpcode and subject used
                sendAckOpcode to simply ACK the sent opcode.
        */

        /* Either way we should check for a status message: */
        if (opFlds.lenInBytes != HW_MBOX_OPCODE_MESSAGE_SIZE)
        {
            PRINTK_ERR("send Opcode to %s(%d) - invalid message for tag %u [got size %uB, excpected size %uB]!",
                       HW_MBOX_MASTER_GET_NAME(subject), subject,
                       HW_MBOX_OPCODE_TAG, opFlds.lenInBytes, HW_MBOX_OPCODE_MESSAGE_SIZE);

            /* Output status messge in User Buffer: */
            pOpcodeMsg = (hwMbox_opcodeMessage_t *)dataBuf;

            /* Change to correct Opcode message to output to User: */
            pOpcodeMsg->msgType = HW_MBOX_OPCODE_MESSAGE_TYPE_STATUS;
            pOpcodeMsg->msgData.msgStatus.status = HW_MBOX_STATUS_OPCODE_ERROR_INVALID_MESSAGE;

            /* Set the correct size: */
            *replyDataLen = HW_MBOX_OPCODE_MESSAGE_SIZE;

            /* release Lock... */
            up(&CHN_LOCK(paramCID));
            return (-1);
        }

        /* Opcode message was sent: */
        pOpcodeMsg = (hwMbox_opcodeMessage_t *)dataBuf;

        /* Copy the Opcode Message from SRAM: */

        DPRINTK("cid %d: copying %uB [out of total %uB channel has] from SRAM at offset 0x%08x",
                paramCID, opFlds.lenInBytes, CHN_CFG_MEMBUFF_GET_SIZE(paramCID), CHN_CFG_MEMBUFF_GET_START(paramCID));

        HW_MBOX_INTERNAL_MESSAGE_COPY_FROM_SRAM(CHN_CFG_MEMBUFF_GET_START(paramCID),
                                                pOpcodeMsg);

        /* output the Reply size: */
        *replyDataLen = opFlds.lenInBytes;

        DPRINTK("cid %d: received HW_MBOX_OPCODE_MESSAGE - msgType %d",
                paramCID, pOpcodeMsg->msgType);

        if (pOpcodeMsg->msgType != HW_MBOX_OPCODE_MESSAGE_TYPE_STATUS)
        {
            PRINTK_ERR("send Opcode to %s(%d) - unexpected message type for tag %u at this point [got type %d, excpected type %d]!",
                       HW_MBOX_MASTER_GET_NAME(subject), subject,
                       HW_MBOX_OPCODE_TAG, pOpcodeMsg->msgType, HW_MBOX_OPCODE_MESSAGE_TYPE_STATUS);

            /* Change to correct Opcode message to output to User: */
            pOpcodeMsg->msgType = HW_MBOX_OPCODE_MESSAGE_TYPE_STATUS;
            pOpcodeMsg->msgData.msgStatus.status = HW_MBOX_STATUS_OPCODE_ERROR_ILLEGAL_MESSAGE;

            /* release Lock... */
            up(&CHN_LOCK(paramCID));
            return (-1);
        }

        /* Opcode Reply Message of type STATUS: */

        DPRINTK("cid %d: received HW_MBOX_OPCODE_MESSAGE of type STATUS(%d) - STATUS = 0x%x (%u)",
                paramCID, pOpcodeMsg->msgType,
                pOpcodeMsg->msgData.msgStatus.status, pOpcodeMsg->msgData.msgStatus.status);

        /* Check the returned STATUS value: */
        if (pOpcodeMsg->msgData.msgStatus.status != HW_MBOX_STATUS_OK)
        {
            /* Internal Error! */
            PRINTK_ERR("send Opcode to %s(%d) - subject replied with internal error! STATUS = 0x%x (%u)",
                       HW_MBOX_MASTER_GET_NAME(subject), subject,
                       pOpcodeMsg->msgData.msgStatus.status, pOpcodeMsg->msgData.msgStatus.status);

            /* release Lock... */
            up(&CHN_LOCK(paramCID));
            return (-1);
        }
        else
        {
            DPRINTK("send Opcode to %s(%d) - subject replied with ACK!",
                    HW_MBOX_MASTER_GET_NAME(subject), subject);

            /* ACK should succeed and return with no Data: */
            *replyDataLen = 0;
        }
    }
    else
    {
        if (opFlds.lenInBytes > 0)
        {
            /* Reply has data! */

            if (opFlds.lenInBytes > dataLen)
            {
                /* Opcode reply data longer than specified buffer... */
                PRINTK_ERR("send Opcode to %s(%d) - opcode reply data length (%uB) > specified buffer size (%uB)!",
                           HW_MBOX_MASTER_GET_NAME(subject), subject, opFlds.lenInBytes, dataLen);

                /* Output status messge in User Buffer: */
                pOpcodeMsg = (hwMbox_opcodeMessage_t *)dataBuf;

                pOpcodeMsg->msgType = HW_MBOX_OPCODE_MESSAGE_TYPE_STATUS;
                pOpcodeMsg->msgData.msgStatus.status = HW_MBOX_STATUS_OPCODE_ERROR_MESSAGE_TOO_LONG;

                /* Set the correct size: */
                *replyDataLen = HW_MBOX_OPCODE_MESSAGE_SIZE;

                /* release Lock... */
                up(&CHN_LOCK(paramCID));
                return (-1);
            }

            /* Get Reply Opcode Data from SRAM: */
            DPRINTK("send Opcode to %s(%d) - copying %uB of reply opcode data from SRAM",
                    HW_MBOX_MASTER_GET_NAME(subject), subject, opFlds.lenInBytes);

            DPRINTK("cid %d: copying %uB [out of total %uB channel has] from SRAM at offset 0x%08x",
                    paramCID, opFlds.lenInBytes, CHN_CFG_MEMBUFF_GET_SIZE(paramCID), CHN_CFG_MEMBUFF_GET_START(paramCID));

            HW_MBOX_COPY_FROM_SRAM(CHN_CFG_MEMBUFF_GET_START(paramCID), opFlds.lenInBytes, dataBuf);
        }

        /* output the Reply size: */
        *replyDataLen = opFlds.lenInBytes;

        DPRINTK("send Opcode to %s(%d) - opcode reply with %uB of data",
                HW_MBOX_MASTER_GET_NAME(subject), subject, *replyDataLen);
    }

    STATS_INC_COUNTER(subject, hw_mbox_opcode, STATS_PEER_IS_SUBJECT);

    /* release Lock... */
    up(&CHN_LOCK(paramCID));

    return 0;
}
EXPORT_SYMBOL(hwMbox_sendOpcode);

/**************************************************************************/
/*! \fn int hwMbox_sendReplyOpcode(hw_mbox_Masters_e commander,
                                   Uint8 *dataBuf, Uint32 *dataLen)
 **************************************************************************
 *  \brief Send a reply to an opcode received from <commander> using the HW_MBOX.
 *  \attention should only be used after an opcode has been received from <commander>.
 *  \param[in] commander - To which Master should the reply be sent.
 *  \param[in] dataBuf - pointer to a Buffer containing data to be sent.
 *  \param[in] dataLen - Length in Bytes of data to be sent from buffer
 *                       specified in <dataBuf>.
 *  \return [ 0 on success / error otherwise ].
 **************************************************************************/
int hwMbox_sendReplyOpcode(hw_mbox_Masters_e commander,
                           Uint8 *dataBuf, Uint32 dataLen)
{
    Uint32 paramCID, cmdRegVal;
    hwMbox_opcode_t opFlds = { 0 };

    CHECK_IS_DRV_INITIALIZED(return (-1));

    DPRINTK("send Reply Opcode to %s(%d) [with %uB of data]",
             HW_MBOX_MASTER_GET_NAME(commander), commander, dataLen);

    if ((dataBuf != NULL) && ((dataLen % 4) != 0))
    {
        /* Opcode Reply data is always a multiple of 4 Bytes! */
        PRINTK_ERR("send Reply Opcode to %s(%d) - opcode reply data length (%uB) must be a multiple of 4B!",
                   HW_MBOX_MASTER_GET_NAME(commander), commander, dataLen);

        return (-1);
    }

    CHECK_FIND_CID_IN_LUT(commander, hw_mbox_opcode, CID_LUT_PEER_IS_COMMANDER, return (-1), paramCID);

    if ((dataBuf != NULL) && (dataLen > 0))
    {
        /* Opcode Reply has data! save to SRAM: */

        /* Verify SRAM size: */
        if (dataLen > CHN_CFG_MEMBUFF_GET_SIZE(paramCID))
        {
            PRINTK_ERR("send Reply Opcode to %s(%d) - opcode reply data length (%uB) > SRAM allocated to channel (%uB)!",
                       HW_MBOX_MASTER_GET_NAME(commander), commander, dataLen, CHN_CFG_MEMBUFF_GET_SIZE(paramCID));

            return (-1);
        }

        DPRINTK("send Reply Opcode to %s(%d) - copying %uB of opcode reply data to SRAM",
                HW_MBOX_MASTER_GET_NAME(commander), commander, dataLen);

        DPRINTK("cid %d: copying %uB [out of total %uB channel has] to SRAM at offset=0x%08x",
                paramCID, dataLen, CHN_CFG_MEMBUFF_GET_SIZE(paramCID), CHN_CFG_MEMBUFF_GET_START(paramCID));

        HW_MBOX_COPY_TO_SRAM(dataBuf, dataLen, CHN_CFG_MEMBUFF_GET_START(paramCID));
    }

    /* Get the current opcode to modify: */
    cmdRegVal = HW_MBOX_CHN_COMMAND_REG_GET(paramCID);

    /* parse to opcode fields: */
    HW_MBOX_OPCODE_FIELDS_GET_TO_STRUCT(cmdRegVal, &opFlds);

    DPRINTK("cid %d: cmdRegVal=0x%08x -> opcode fields - tag=%u, lenInBytes=%u",
            paramCID, cmdRegVal, opFlds.tag, opFlds.lenInBytes);

    /* Update with the required size: */
    opFlds.lenInBytes = dataLen;

    /* Create opcode reply value: */
    HW_MBOX_OPCODE_FIELDS_SET_FROM_STRUCT(cmdRegVal, &opFlds);

    DPRINTK("cid %d: opcode fields - tag=%u, lenInBytes=%u -> cmdRegVal=0x%08x",
            paramCID, opFlds.tag, opFlds.lenInBytes, cmdRegVal);

    /* Send Opcode Reply to commander */
    DPRINTK("cid %d: before write to command register - state=%d", paramCID, HW_MBOX_CHN_STATE_REG_GET(paramCID));
    DPRINTK("cid %d: writing 0x%08x to the command register of the channel...", paramCID, cmdRegVal);
    HW_MBOX_CHN_COMMAND_REG_SET(paramCID, cmdRegVal);
    DPRINTK("cid %d: after write to command register - state=%d", paramCID, HW_MBOX_CHN_STATE_REG_GET(paramCID));

    return 0;
}
EXPORT_SYMBOL(hwMbox_sendReplyOpcode);

/**************************************************************************/
/*! \fn int hwMbox_sendAckOpcode(hw_mbox_Masters_e commander)
 **************************************************************************
 *  \brief Send back an ACK to <commander>, from which an opcode has been
 *         received using the HW_MBOX.
 *  \attention should only be used after an opcode has been received from
 *             <commander> using HW_MBOX_recvCommand.
 *  \param[in] commander - To which Master should the ACK be sent.
 *  \return [ 0 on success / error otherwise ].
 **************************************************************************/
int hwMbox_sendAckOpcode(hw_mbox_Masters_e commander)
{
    Uint32 paramCID, cmdRegVal;

    hwMbox_opcode_t opFlds = { 0 };
    hwMbox_opcodeMessage_t opcodeMsg;

    CHECK_IS_DRV_INITIALIZED(return (-1));

    DPRINTK("send ACK Opcode to %s(%d)",
             HW_MBOX_MASTER_GET_NAME(commander), commander);

    CHECK_FIND_CID_IN_LUT(commander, hw_mbox_opcode, CID_LUT_PEER_IS_COMMANDER, return (-1), paramCID);

    /* To send an ACK to the Commander we send back
       a HW MBOX opcode message of type STATUS with STATUS_OK. */

    opcodeMsg.msgType = HW_MBOX_OPCODE_MESSAGE_TYPE_STATUS;
    opcodeMsg.msgData.msgStatus.status = HW_MBOX_STATUS_OK;

    DPRINTK("send ACK Opcode to %s(%d) - copying %uB of opcode ACK data to SRAM",
             HW_MBOX_MASTER_GET_NAME(commander), commander, HW_MBOX_OPCODE_MESSAGE_SIZE);

    DPRINTK("cid %d: copying %uB [out of total %uB channel has] to SRAM at offset=0x%08x",
            paramCID, HW_MBOX_OPCODE_MESSAGE_SIZE, CHN_CFG_MEMBUFF_GET_SIZE(paramCID), CHN_CFG_MEMBUFF_GET_START(paramCID));

    /* Copy the opcode message to the SRAM: */
    HW_MBOX_INTERNAL_MESSAGE_COPY_TO_SRAM(&opcodeMsg,
                                          CHN_CFG_MEMBUFF_GET_START(paramCID));

    /* Build the opcode: */
    opFlds.tag = HW_MBOX_OPCODE_TAG;
    opFlds.lenInBytes = HW_MBOX_OPCODE_MESSAGE_SIZE;

    /* Create opcode reply value: */
    HW_MBOX_OPCODE_FIELDS_SET_FROM_STRUCT(cmdRegVal, &opFlds);

    DPRINTK("cid %d: opcode fields - tag=%u, lenInBytes=%u -> cmdRegVal=0x%08x",
            paramCID, opFlds.tag, opFlds.lenInBytes, cmdRegVal);

    /* Send Opcode ACK to commander: */
    DPRINTK("send ACK Opcode to %s(%d) - sending opcode ACK to commander!",
             HW_MBOX_MASTER_GET_NAME(commander), commander);

    DPRINTK("cid %d: before write to command register - state=%d", paramCID, HW_MBOX_CHN_STATE_REG_GET(paramCID));
    DPRINTK("cid %d: writing 0x%08x to the command register of the channel...", paramCID, cmdRegVal);
    HW_MBOX_CHN_COMMAND_REG_SET(paramCID, cmdRegVal);
    DPRINTK("cid %d: after write to command register - state=%d", paramCID, HW_MBOX_CHN_STATE_REG_GET(paramCID));

    return 0;
}
EXPORT_SYMBOL(hwMbox_sendAckOpcode);

/**************************************************************************/
/*! \fn int hwMbox_registerRecvOpcode(hw_mbox_Masters_e commander,
                                      hwMboxRecvOpcodeCallback pOpcodeCallbackFunc,
                                      Uint32 tag,
                                      Uint32 token)
 **************************************************************************
 *  \brief Register for Receive Opcode from <commander> using the HW_MBOX.
 *  \attention This function should be called once per tag.
 *  \param[in] commander - From which Master should the opcode be received.
 *  \param[in] pOpcodeCallbackFunc - pointer to the callback function to be
 *                                   invoked upon receiving an opcode from
 *                                   <commander>.
 *  \param[in] tag - indicates the destination.
 *  \param[in] token - 32 bit value saved in DB that can be used by application.
 *  \return [ 0 on success / error otherwise ].
 **************************************************************************/
int hwMbox_registerRecvOpcode(hw_mbox_Masters_e commander,
                              hwMboxRecvOpcodeCallback pOpcodeCallbackFunc,
                              Uint32 tag,
                              Uint32 token)
{
    int i;
    Uint32 paramCID;
    Int32 nextTagIndex;

    CHECK_IS_DRV_INITIALIZED(return (-1));

    if (pOpcodeCallbackFunc == NULL)
    {
        PRINTK_ERR("pOpcodeCallbackFunc == NULL!");
        return (-1);
    }

    DPRINTK("register Receive Opcode from %s(%d) [tag %u token 0x%x (%u)]",
            HW_MBOX_MASTER_GET_NAME(commander), commander, tag, token, token);

    CHECK_FIND_CID_IN_LUT(commander, hw_mbox_opcode, CID_LUT_PEER_IS_COMMANDER, return (-1), paramCID);

    if (CHN_RECV_OPCODE_GET_INDEX(paramCID) == CHN_RECV_OPCODE_INDEX_INVALID)
    {
        /* should generally not happen since it is changed by
           set channel config, but it doesn't hurt to make sure... */

        PRINTK_ERR("recvOpIndex == CHN_RECV_OPCODE_INDEX_INVALID");
        return (-1);
    }

    if (tag >= HW_MBOX_OPCODE_RESERVED_TAGS_START)
    {
        PRINTK_ERR("register Receive Opcode from %s(%d) - can't register tag %u, it is reserved for HW MBOX!",
                   HW_MBOX_MASTER_GET_NAME(commander), commander, tag);
        return (-1);
    }

    /* seize channel lock */
    if (down_interruptible(&CHN_LOCK(paramCID)))
    {
        return -ERESTARTSYS;
    }

    /* invalid index initially */
    nextTagIndex = -1;

    for (i = 0; i < HW_MBOX_OPCODE_TAG_COUNT; i++)
    {
        /* save first free slot to nextTagIndex */
        if ( (nextTagIndex < 0) &&
             (HW_MBOX_TAG_INVALID == RECV_OP_GET_TAG(paramCID, i)) )
        {
            nextTagIndex = i;
        }

        if (tag == RECV_OP_GET_TAG(paramCID, i))
        {
            DPRINTK("register Receive Opcode from %s(%d) - tag %u is already registered!",
                    HW_MBOX_MASTER_GET_NAME(commander), commander, tag);

            /* release lock */
            up(&CHN_LOCK(paramCID));
            return (-1);
        }
    }

    /* there is no free slot if nextTagIndex remains invalid */
    if (nextTagIndex < 0)
    {
        DPRINTK("register Receive Opcode from %s(%d) - max registered tag count (%d) reached!",
                HW_MBOX_MASTER_GET_NAME(commander), commander, HW_MBOX_OPCODE_TAG_COUNT);

        up(&CHN_LOCK(paramCID));
        return (-1);
    }

    RECV_OP_SET_TAG(paramCID, nextTagIndex, tag);
    RECV_OP_SET_CB(paramCID, nextTagIndex, pOpcodeCallbackFunc);
    RECV_OP_SET_TOKEN(paramCID, nextTagIndex, token);

    RECV_OP_INC_TAG_COUNT(paramCID);

    DPRINTK("register Receive Opcode from %s(%d) - tag %u successfully registered!",
            HW_MBOX_MASTER_GET_NAME(commander), commander, tag);

    /* release lock */
    up(&CHN_LOCK(paramCID));
    return 0;
}
EXPORT_SYMBOL(hwMbox_registerRecvOpcode);

/**************************************************************************/
/*! \fn int hwMbox_unregisterRecvOpcode(hw_mbox_Masters_e commander, Uint32 tag)
 **************************************************************************
 *  \brief Unregister Receive Opcode from <commander> using the HW_MBOX.
 *  \attention This function should be called once per tag.
 *  \param[in] commander - From which Master should the opcode be received.
 *  \param[in] tag - indicates the destination.
 *  \return [ 0 on success / error otherwise ].
 **************************************************************************/
int hwMbox_unregisterRecvOpcode(hw_mbox_Masters_e commander, Uint32 tag)
{
    int i;
    Uint32 paramCID;
    Int32 retVal = -1;

    CHECK_IS_DRV_INITIALIZED(return (-1));

    if (!HW_MBOX_MID_VALIDITY_CHECK(commander))
    {
        PRINTK_ERR("invalid commander value! [commander = %d]",
                   commander);
        return (-1);
    }

    CHECK_FIND_CID_IN_LUT(commander,
                          hw_mbox_opcode,
                          CID_LUT_PEER_IS_COMMANDER,
                          return (-1),
                          paramCID);

    if (CHN_RECV_OPCODE_GET_INDEX(paramCID) == CHN_RECV_OPCODE_INDEX_INVALID)
    {
        /* should generally not happen since it is changed by
           set channel config, but it doesn't hurt to make sure... */

        PRINTK_ERR("recvOpIndex == CHN_RECV_OPCODE_INDEX_INVALID");
        return (-1);
    }

    /* seize channel lock */
    if (down_interruptible(&CHN_LOCK(paramCID)))
    {
        return -ERESTARTSYS;
    }

    for (i = 0; i < HW_MBOX_OPCODE_TAG_COUNT; i++)
    {
        if (tag == RECV_OP_GET_TAG(paramCID, i))
        {
            DPRINTK("unregister Receive Opcode from %s(%d) - tag %u",
                    HW_MBOX_MASTER_GET_NAME(commander), commander, tag);

#ifdef HW_MBOX_SOCKETS_API
            if (RECV_OP_GET_CB(paramCID, i) == hwMbox_socketRecvOpCb)
            {
                /* This is a subject opcode socket: */
                struct socket *sock;
                struct hwMbox_sock *sk;

                sock = (struct socket *)RECV_OP_GET_TOKEN(paramCID, i);
                if ((sock) && (sk = hwMbox_sk(sock->sk)))
                {
                    if (CHN_INTR_STATE_PENDING == (chnIntrState_e)sk->chnIntr)
                    {
                        /* if there's a pending channel Interrupt send opcode reply back */
                        DPRINTK_SKT(sock, "chnIntr is PENDING, sending HW_MBOX_STATUS_OPCODE_ERROR_INVALID_TAG message...");
                        if (_hwMbox_sendOpcodeMsg(paramCID, HW_MBOX_STATUS_OPCODE_ERROR_INVALID_TAG) < 0)
                        {
                            PRINTK_ERR("failed to clear pending chnIntr! [paramCID=%u, tag=%u]",
                                       paramCID, tag);
                        }

                        /* Need to fix statistics in this case:
                           1. Increment the invalid tag message count.
                           2. Decrement the received OPCODE count since this one wasn't received by application.
                           [we are holding channel's semaphore so no race with Listener on the fields].
                        */
                        RECV_OP_INC_INVALID_TAG_COUNT(paramCID);
                        STATS_DEC_COUNTER(sk->mid, hw_mbox_opcode, STATS_PEER_IS_COMMANDER);
                    }
                }
                else
                {
                    PRINTK_ERR("NULL pointer! [sock=%p]", sock);
                }
            }
#endif /* HW_MBOX_SOCKETS_API */

            RECV_OP_SET_CB(paramCID, i, NULL);
            RECV_OP_SET_TOKEN(paramCID, i, 0);
            RECV_OP_CLR_OPCODE_CNT(paramCID, i);
            RECV_OP_SET_TAG(paramCID, i, HW_MBOX_TAG_INVALID);
            RECV_OP_DEC_TAG_COUNT(paramCID);

            retVal = 0;
            break;
        }
    }

    if (retVal < 0)
    {
        DPRINTK("unregister Receive Opcode from %s(%d) - tag %u failed!!!",
                HW_MBOX_MASTER_GET_NAME(commander), commander, tag);
    }

    /* release lock */
    up(&CHN_LOCK(paramCID));
    return retVal;
}
EXPORT_SYMBOL(hwMbox_unregisterRecvOpcode);

/************************************************************************/
/*     HW_MBOX proc file system utilities:                              */
/************************************************************************/

/*! \def HW_MBOX_PROC_DIR_STR
    \brief proc dir: /proc/HW_MBOX */
#define HW_MBOX_PROC_DIR_STR    "hw_mbox"

/* Define all Simple Procs here: */

SEQ_PROC_DEFINE_SIMPLE(proc_dump_core_version);
SEQ_PROC_DEFINE_SIMPLE(proc_dump_control_regs);
SEQ_PROC_DEFINE_SIMPLE(proc_dump_status);
SEQ_PROC_DEFINE_SIMPLE(proc_dump_ctrlDb);

/* Define all SEQ Procs here: */

SEQ_PROC_DEFINE(proc_dump_masters_regs);
SEQ_PROC_DEFINE(proc_dump_channels_regs);
SEQ_PROC_DEFINE(proc_dump_assignedChannels_regs);
SEQ_PROC_DEFINE(proc_dump_interrupts);
SEQ_PROC_DEFINE(proc_dump_LUT);
SEQ_PROC_DEFINE(proc_dump_serviceCounter);
SEQ_PROC_DEFINE(proc_dump_masterDb);
SEQ_PROC_DEFINE(proc_dump_chnDb);
SEQ_PROC_DEFINE(proc_dump_recvOpDb);
SEQ_PROC_DEFINE(proc_dump_recvOpTagsDb);
#ifdef HW_MBOX_SOCKETS_API
SEQ_PROC_DEFINE(proc_dump_sockstat);
#endif /* HW_MBOX_SOCKETS_API */

/* Implement all simple procs here: */

/*! \fn static int proc_dump_core_version(struct seq_file *m, void *v)
    \brief dump HW_MBOX Core Version proc (reads ID registers). */
static int proc_dump_core_version(struct seq_file *m, void *v)
{
    seq_printf(m, "HW_MBOX Core Version: %d.%d\n", HW_MBOX_ID_MAJOR_GET(), HW_MBOX_ID_MINOR_GET());
    return 0;
}

/*! \fn static int proc_dump_control_regs(struct seq_file *m, void *v)
    \brief dump HW_MBOX Control Registers proc. */
static int proc_dump_control_regs(struct seq_file *m, void *v)
{
    seq_printf(m, "ID:     0x%08x\n", HW_MBOX_ID_REG_GET());
    seq_printf(m, "CTRL:   0x%08x\n", HW_MBOX_CTRL_REG_GET());
    seq_printf(m, "ERROR:  0x%08x\n", HW_MBOX_ERROR_REG_GET());
    return 0;
}

/*! \fn static int proc_dump_status(struct seq_file *m, void *v)
    \brief dump HW_MBOX Status proc (parses data from Control Registers). */
static int proc_dump_status(struct seq_file *m, void *v)
{
    seq_printf(m, "HW_MBOX status:\n");
    seq_printf(m, "LOCKED:          %s\n", YESNO_STR(HW_MBOX_CTRL_LOCK_GET()));
    seq_printf(m, "STARTED:         %s\n", YESNO_STR(HW_MBOX_CTRL_START_GET()));
    seq_printf(m, "HW DEBUG MODE:   %s\n", YESNO_STR(HW_MBOX_CTRL_DEBUG_GET()));
    seq_printf(m, "Supervisor:      %s(%d)\n", HW_MBOX_MASTER_GET_NAME(HW_MBOX_CTRL_SUPERVISOR_GET()),
                                               HW_MBOX_CTRL_SUPERVISOR_GET());
    if (HW_MBOX_CTRL_MON_EXIST_GET())
    {
        seq_printf(m, "Monitor:         %s(%d)\n", HW_MBOX_MASTER_GET_NAME(HW_MBOX_CTRL_MONITOR_GET()),
                                                   HW_MBOX_CTRL_MONITOR_GET());
    }
    else
    {
        seq_printf(m, "Monitor:         There is no Monitor defined.\n");
    }

    return 0;
}

/*! \fn static int proc_dump_ctrlDb(struct seq_file *m, void *v)
    \brief dump HW_MBOX Control DB proc. */
static int proc_dump_ctrlDb(struct seq_file *m, void *v)
{
    seq_printf(m, "HW_MBOX Ctrl DB:\n");
    seq_printf(m, "LOCKED:          %s\n", YESNO_STR(CTRL_GET_IS_LOCKED()));
    seq_printf(m, "STARTED:         %s\n", YESNO_STR(CTRL_GET_IS_STARTED()));
    seq_printf(m, "HW DEBUG MODE:   %s\n", YESNO_STR(CTRL_GET_IS_HW_DEBUG_MODE()));
    seq_printf(m, "Supervisor:      %s(%d)\n", HW_MBOX_MASTER_GET_NAME(CTRL_GET_SUPERVISOR()),
                                               CTRL_GET_SUPERVISOR());
    if (CTRL_GET_MONITOR() != HW_MBOX_MID_INVALID)
    {
        seq_printf(m, "Monitor:         %s(%d)\n", HW_MBOX_MASTER_GET_NAME(CTRL_GET_MONITOR()),
                                                   CTRL_GET_MONITOR());
    }
    else
    {
        seq_printf(m, "Monitor:         There is no Monitor defined.\n");
    }

    return 0;
}

/* Implement all SEQ show functions here: */

/*! \fn static int proc_dump_masters_regs_show(struct seq_file *m, void *v)
    \brief dump HW_MBOX Masters Registers. */
static int proc_dump_masters_regs_show(struct seq_file *m, void *v)
{
    seq_proc_if_control_data_t *controlData = m->private;
    loff_t *spos = v;
    Uint32 i = (Uint32)(*spos);
    DPRINT_SEQ(m, "SHOW (i=%d, *spos = %lld)\n", i,(long long)*spos);
    if (i < HW_MBOX_MASTER_COUNT)
    {
        if (0 == i)
        {
            /* We are in the first run, print line and headers */
            seq_printf(m, "+------+-----------------------+-----------------------+-------------------------+-----------------------+\n");
            seq_printf(m, "| MID# |   SHADOW_H SHADOW_L   |     MASK_H MASK_L     | COMMANDER_H COMMANDER_L |  SUBJECT_H SUBJECT_L  |\n");
            /* Value fmt: "|  ##  | 0x00000000 0x00000000 | 0x00000000 0x00000000 |  0x00000000 0x00000000  | 0x00000000 0x00000000 |" */
        }

        seq_printf(m, "|  %2d  | 0x%08x 0x%08x | 0x%08x 0x%08x |  0x%08x 0x%08x  | 0x%08x 0x%08x |\n", i,
                   HW_MBOX_MASTER_PARAM_INTR_CLEAR_H_REG_GET(i), HW_MBOX_MASTER_PARAM_INTR_CLEAR_L_REG_GET(i),
                   HW_MBOX_MASTER_PARAM_INTR_MASK_H_REG_GET(i), HW_MBOX_MASTER_PARAM_INTR_MASK_L_REG_GET(i),
                   HW_MBOX_MASTER_PARAM_COMMANDER_H_REG_GET(i), HW_MBOX_MASTER_PARAM_COMMANDER_L_REG_GET(i),
                   HW_MBOX_MASTER_PARAM_SUBJECT_H_REG_GET(i), HW_MBOX_MASTER_PARAM_SUBJECT_L_REG_GET(i));

        if ((HW_MBOX_MASTER_COUNT - 1) == i)
        {
            /* End of table print trailing line */
            seq_printf(m, "+------+-----------------------+-----------------------+-------------------------+-----------------------+\n");
            controlData->endOfData = True;
        }
    }

    return 0;
}

/*! \fn static int proc_dump_channels_regs_show(struct seq_file *m, void *v)
    \brief dump HW_MBOX Channels Registers. */
static int proc_dump_channels_regs_show(struct seq_file *m, void *v)
{
    seq_proc_if_control_data_t *controlData = m->private;
    loff_t *spos = v;
    Uint32 i = (Uint32)(*spos);
    DPRINT_SEQ(m, "SHOW (i=%d, *spos = %lld)\n", i,(long long)*spos);
    if (i < HW_MBOX_CHANNEL_COUNT)
    {
        if (0 == i)
        {
            /* We are in the first run, print line and headers */
            seq_printf(m, "+------+-------------+-------------+-------------+-------------+\n");
            seq_printf(m, "| CHN# |   CHN_CFG   |  CHN_STATE  | CHN_COMMAND |   MEM_MAP   |\n");
            /* Value fmt: "|  ##  |  0x00000000 |  0x00000000 |  0x00000000 |  0x00000000 |" */
        }

        seq_printf(m, "|  %2d  |  0x%08x |  0x%08x |  0x%08x |  0x%08x |\n", i,
                   HW_MBOX_CHN_CFG_REG_GET(i), HW_MBOX_CHN_STATE_REG_GET(i),
                   HW_MBOX_CHN_COMMAND_REG_GET(i), HW_MBOX_MEMORY_MAP_REG_GET(i));

        if ((HW_MBOX_CHANNEL_COUNT - 1) == i)
        {
            /* End of table print trailing line */
            seq_printf(m, "+------+-------------+-------------+-------------+-------------+\n");
            controlData->endOfData = True;
        }
    }

    return 0;
}

/*! \fn static int proc_dump_assignedChannels_regs_show(struct seq_file *m, void *v)
    \brief dumps assigned HW_MBOX Channels Registers (according to DB). */
static int proc_dump_assignedChannels_regs_show(struct seq_file *m, void *v)
{
    seq_proc_if_control_data_t *controlData = m->private;
    loff_t *spos = v;
    Uint32 i = (Uint32)(*spos);
    DPRINT_SEQ(m, "SHOW (i=%d, *spos = %lld)\n", i,(long long)*spos);
    if (i < HW_MBOX_CHANNEL_COUNT)
    {
        if (0 == i)
        {
            /* We are in the first run, print line and headers */
            seq_printf(m, "+------+-------------+-------------+-------------+-------------+\n");
            seq_printf(m, "| CHN# |   CHN_CFG   |  CHN_STATE  | CHN_COMMAND |   MEM_MAP   |\n");
            /* Value fmt: "|  ##  |  0x00000000 |  0x00000000 |  0x00000000 |  0x00000000 |" */
        }

        if (CHN_IS_ASSIGNED(i))
        {
            seq_printf(m, "|  %2d  |  0x%08x |  0x%08x |  0x%08x |  0x%08x |\n", i,
                       HW_MBOX_CHN_CFG_REG_GET(i), HW_MBOX_CHN_STATE_REG_GET(i),
                       HW_MBOX_CHN_COMMAND_REG_GET(i), HW_MBOX_MEMORY_MAP_REG_GET(i));
        }

        if ((HW_MBOX_CHANNEL_COUNT - 1) == i)
        {
            /* End of table print trailing line */
            seq_printf(m, "+------+-------------+-------------+-------------+-------------+\n");
            controlData->endOfData = True;
        }
    }

    return 0;
}

/*! \fn static int proc_dump_interrupts_show(struct seq_file *m, void *v)
    \brief dump HW_MBOX Interrupts Data. */
static int proc_dump_interrupts_show(struct seq_file *m, void *v)
{
    seq_proc_if_control_data_t *controlData = m->private;
    loff_t *spos = v;
    Uint32 i = (Uint32)(*spos);
    DPRINT_SEQ(m, "SHOW (i=%d, *spos = %lld)\n", i,(long long)*spos);
    if (i < HW_MBOX_CHANNEL_COUNT)
    {
        if (0 == i)
        {
            /* We are in the first run, print line and headers */
            seq_printf(m, " CID         INTR\n");
        }
        seq_printf(m, " %2d:   %10u\n", i, CHN_INTR_GET_COUNT(i));

        if (i == HW_MBOX_CHANNEL_COUNT - 1) /* Last Line */
        {
            seq_printf(m, "err:   %10u\n", GET_ERR_INTR());
            seq_printf(m, "ghost: %10u\n", GET_GHOST_INTR());
            controlData->endOfData = True;
        }
    }

    return 0;
}

/*! \fn static int proc_dump_LUT_show(struct seq_file *m, void *v)
    \brief dump HW_MBOX channel LUT [invalid CID is 0xffffffff]. */
static int proc_dump_LUT_show(struct seq_file *m, void *v)
{
    seq_proc_if_control_data_t *controlData = m->private;
    loff_t *spos = v;
    Uint32 i = (Uint32)(*spos);

    hw_mbox_type_e type;

    DPRINT_SEQ(m, "SHOW (i=%d, *spos = %lld)\n", i,(long long)*spos);
    if (i < HW_MBOX_MASTER_COUNT)
    {
        if (0 == i)
        {
            /* We are in the first run, print line and headers */

            /* "+------+------------+------------+------------+ ... "
               "|      |            |  SEND      |  RECV      | ... "
               "| MID# |   MASTER   |  CHN_TYPE  |  CHN_TYPE  | ... "
          fmt: "|  ##  | ********** | 0x00000000 | 0x00000000 | for every type..." */

            seq_printf(m, "+------+------------+");
            for (type = 0; type < HW_MBOX_TYPE_COUNT; type++)
            {
                seq_printf(m, "------------+------------+");
            }
            seq_printf(m, "\n");

            seq_printf(m, "|      |            |");

            for (type = 0; type < HW_MBOX_TYPE_COUNT; type++)
            {
                seq_printf(m, "  SEND      |  RECV      |");
            }
            seq_printf(m, "\n");

            seq_printf(m, "| MID# |   MASTER   |");

            for (type = 0; type < HW_MBOX_TYPE_COUNT; type++)
            {
                seq_printf(m, "  %-8s  |  %-8s  |", HW_MBOX_TYPE_STR(type), HW_MBOX_TYPE_STR(type));
            }
            seq_printf(m, "\n");
        }

        seq_printf(m, "|  %2d  | %-10s |", i, HW_MBOX_MASTER_GET_NAME(i));

        for (type = 0; type < HW_MBOX_TYPE_COUNT; type++)
        {
            seq_printf(m, " 0x%08x | 0x%08x |", FIND_CID_IN_LUT(i, type, CID_LUT_PEER_IS_SUBJECT),
                                                FIND_CID_IN_LUT(i, type, CID_LUT_PEER_IS_COMMANDER));
            /* fmt:          SEND  |  RECV  | */
        }
        seq_printf(m, "\n");

        if (i == HW_MBOX_MASTER_COUNT - 1) /* Last Line */
        {
            seq_printf(m, "+------+------------+");
            for (type = 0; type < HW_MBOX_TYPE_COUNT; type++)
            {
                seq_printf(m, "------------+------------+");
            }
            seq_printf(m, "\n");

            controlData->endOfData = True;
        }
    }

    return 0;
}

/*! \fn static int proc_dump_serviceCounter_show(struct seq_file *m, void *v)
    \brief dump HW_MBOX service counter statistics data. */
static int proc_dump_serviceCounter_show(struct seq_file *m, void *v)
{
    seq_proc_if_control_data_t *controlData = m->private;
    loff_t *spos = v;
    Uint32 i = (Uint32)(*spos);

    hw_mbox_type_e type;

    DPRINT_SEQ(m, "SHOW (i=%d, *spos = %lld)\n", i,(long long)*spos);
    if (i < HW_MBOX_MASTER_COUNT)
    {
        if (0 == i)
        {
            /* We are in the first run, print line and headers */

            /* "+------+------------+------------+------------+ ... "
               "|      |            |  SEND      |  RECV      | ... "
               "| MID# |   MASTER   |  CHN_TYPE  |  CHN_TYPE  | ... "
          fmt: "|  ##  | ********** | 0000000000 | 0000000000 | for every type..." */

            seq_printf(m, "+------+------------+");
            for (type = 0; type < HW_MBOX_TYPE_COUNT; type++)
            {
                seq_printf(m, "------------+------------+");
            }
            seq_printf(m, "\n");

            seq_printf(m, "|      |            |");

            for (type = 0; type < HW_MBOX_TYPE_COUNT; type++)
            {
                seq_printf(m, "  SEND      |  RECV      |");
            }
            seq_printf(m, "\n");

            seq_printf(m, "| MID# |   MASTER   |");

            for (type = 0; type < HW_MBOX_TYPE_COUNT; type++)
            {
                seq_printf(m, "  %-8s  |  %-8s  |", HW_MBOX_TYPE_STR(type), HW_MBOX_TYPE_STR(type));
            }
            seq_printf(m, "\n");
        }

        seq_printf(m, "|  %2d  | %-10s |", i, HW_MBOX_MASTER_GET_NAME(i));

        for (type = 0; type < HW_MBOX_TYPE_COUNT; type++)
        {
            seq_printf(m, " %10u | %10u |", STATS_GET_COUNTER(i, type, STATS_PEER_IS_SUBJECT), STATS_GET_COUNTER(i, type, STATS_PEER_IS_COMMANDER));
            /* fmt:         SEND | RECV | */
        }
        seq_printf(m, "\n");

        if (i == HW_MBOX_MASTER_COUNT - 1) /* Last Line */
        {
            seq_printf(m, "+------+------------+");
            for (type = 0; type < HW_MBOX_TYPE_COUNT; type++)
            {
                seq_printf(m, "------------+------------+");
            }
            seq_printf(m, "\n");

            controlData->endOfData = True;
        }
    }

    return 0;
}

/*! \fn static int proc_dump_masterDb_show(struct seq_file *m, void *v)
    \brief dump HW_MBOX master DB Data. */
static int proc_dump_masterDb_show(struct seq_file *m, void *v)
{
    seq_proc_if_control_data_t *controlData = m->private;
    loff_t *spos = v;
    Uint32 i = (Uint32)(*spos);
    DPRINT_SEQ(m, "SHOW (i=%d, *spos = %lld)\n", i,(long long)*spos);
    if (i < HW_MBOX_MASTER_COUNT)
    {
        if (0 == i)
        {
            /* We are in the first run, print line and headers */
            seq_printf(m, "+------+------------+-------+------+------------+------------+\n");
            seq_printf(m, "| MID# |   MASTER   | VALID | INIT | SW SUPPORT |   ERRORS   |\n");
            /*       fmt: "|  ##  | ********** |  ***  | ***  | *******    | ########## |" */
        }

        seq_printf(m, "|  %2d  | %-10s |  %-3s  | %-3s  | %-7s    | %10u |\n",
                   i,
                   HW_MBOX_MASTER_GET_NAME(i),
                   YESNO_STR(IS_MASTER_VALID(i)),
                   YESNO_STR(IS_MASTER_INIT(i)),
                   HW_MBOX_SUPPORT_STR(MASTER_CFG_GET_SUPPORT(i)),
                   MASTER_GET_ERROR_COUNT(i));

        if (i == HW_MBOX_MASTER_COUNT - 1) /* Last Line */
        {
            seq_printf(m, "+------+------------+-------+------+------------+------------+\n");
            controlData->endOfData = True;
        }
    }

    return 0;
}

/*! \fn static int proc_dump_chnDb_show(struct seq_file *m, void *v)
    \brief dump HW_MBOX channel DB Data. */
static int proc_dump_chnDb_show(struct seq_file *m, void *v)
{
    seq_proc_if_control_data_t *controlData = m->private;
    loff_t *spos = v;
    Uint32 i = (Uint32)(*spos);
    DPRINT_SEQ(m, "SHOW (i=%d, *spos = %lld)\n", i,(long long)*spos);
    if (i < HW_MBOX_CHANNEL_COUNT)
    {
        if (0 == i)
        {
            /* We are in the first run, print line and headers */
            seq_printf(m, "+------+-------+---------------+---------------+---------------+-------------+-----------+---------------+--------------+\n");
            seq_printf(m, "| CHN# | VALID |   MBOX TYPE   |   COMMANDER   |    SUBJECT    | SRAM OFFSET | SRAM SIZE |  CHN_INTR_ST  | RECVOP INDEX |\n");
                  /* fmt: "|  ##  |  ***  | ************  | ## ********** | ## ********** |  0x00000000 |  00000000 |            00 |           00 |" */
        }

        if (CHN_IS_ASSIGNED(i))
        {
            /* only prints assigned channels */
            seq_printf(m, "|  %2d  |  %-3s  |  %-12s | %02d %-10s | %02d %-10s |  0x%08x |  %8u |            %2d |           %2d |\n",
                       i,
                       YESNO_STR(CHN_IS_VALID(i)),
                       HW_MBOX_TYPE_STR(CHN_CFG_GET_TYPE(i)),
                       CHN_CFG_GET_COMMANDER(i) , HW_MBOX_MASTER_GET_NAME(CHN_CFG_GET_COMMANDER(i)),
                       CHN_CFG_GET_SUBJECT(i) , HW_MBOX_MASTER_GET_NAME(CHN_CFG_GET_SUBJECT(i)),
                       CHN_CFG_MEMBUFF_GET_START(i), CHN_CFG_MEMBUFF_GET_SIZE(i),
                       CHN_INTR_GET_STATE(i),
                       CHN_RECV_OPCODE_GET_INDEX(i));

        }

        if (i == HW_MBOX_CHANNEL_COUNT - 1) /* Last Line */
        {
            seq_printf(m, "+------+-------+---------------+---------------+---------------+-------------+-----------+---------------+--------------+\n");
            controlData->endOfData = True;
        }
    }

    return 0;
}

/*! \fn static int proc_dump_recvOpDb_show(struct seq_file *m, void *v)
    \brief dump HW_MBOX receive Opcode DB Data. */
static int proc_dump_recvOpDb_show(struct seq_file *m, void *v)
{
    seq_proc_if_control_data_t *controlData = m->private;
    loff_t *spos = v;
    Uint32 i = (Uint32)(*spos);
    DPRINT_SEQ(m, "SHOW (i=%d, *spos = %lld)\n", i,(long long)*spos);

    if (i < HW_MBOX_OPCODE_RECV_COUNT)
    {
        if (0 == i)
        {
            /* We are in the first run, print line and headers */
            seq_printf(m, "+--------+-------------+-------------+---------------+------------------+\n");
            seq_printf(m, "| RECVOP | REG TAG CNT | WAKE UP CNT |  APP_INTR_ST  | INVALID TAG MSGS |\n");
                  /* fmt: "|     ## |  0000000000 |  0000000000 |            00 |       0000000000 |" */
        }

        if (RECV_OP_THREAD_BY_INDEX(i) != NULL)
        {
            /* only prints assigned recvOp indices */
            seq_printf(m, "|     %2d |  %10u |  %10u |            %2d |       %10u |\n",
                       i,
                       RECV_OP_GET_TAG_COUNT_BY_INDEX(i),
                       RECV_OP_GET_WAKEUP_COUNT_BY_INDEX(i),
                       RECV_OP_APP_GET_STATE_BY_INDEX(i),
                       RECV_OP_GET_INVALID_TAG_COUNT_BY_INDEX(i));
        }
        else
        {
            /* no more data, we can already end here... */
            seq_printf(m, "+--------+-------------+-------------+---------------+------------------+\n");
            controlData->endOfData = True;
        }

        if (i == HW_MBOX_OPCODE_RECV_COUNT - 1) /* Last Line */
        {
            seq_printf(m, "+--------+-------------+-------------+---------------+------------------+\n");
            controlData->endOfData = True;
        }
    }

    return 0;
}

#ifdef HW_MBOX_SOCKETS_API
/*! \fn static int proc_dump_sockstat_show(struct seq_file *m, void *v)
    \brief dump sockets of AF_HWMBOX family */
static int proc_dump_sockstat_show(struct seq_file *m, void *v)
{
    seq_proc_if_control_data_t *controlData = m->private;
    static struct sock *curr_sk = NULL;
    loff_t *spos = v;
    Uint32 i = (Uint32)(*spos);
    DPRINT_SEQ(m, "SHOW (i=%d, *spos = %lld)\n", i, (long long)*spos);

    if (0 == i)
    {
        seq_printf(m, "+------------+-----------+---------------+-------+----------+------+-----------+------+-------+-----------------------+-----------------------+\n");
        seq_printf(m, "|   SOCKET   | SOCK TYPE |    MASTER     | APPID | PROTOCOL | CHN# |   STATE   | PEND |  PID  |  SENT: CNT BYTES      |  RECV: CNT BYTES      |\n");
              /* fmt: "| 0x00000000 | ######### | 00 ########## | ##### | ######## |  00  | ######### |  00  | 65535 | 4294967295 0x00000000 | 4294967295 0x00000000 |" */

        if (down_interruptible(&hwMbox_sock_hlist_sem))
        {
            return (-1);
        }

        curr_sk = hlist_entry_safe(hwMbox_sock_hlist.first, struct sock, sk_node);
    }
    else
    {
        curr_sk = hlist_entry_safe(curr_sk->sk_node.next, struct sock, sk_node);
    }

    if (curr_sk == NULL)
    {
        up(&hwMbox_sock_hlist_sem);
        seq_printf(m, "+------------+-----------+---------------+-------+----------+------+-----------+------+-------+-----------------------+-----------------------+\n");
        controlData->endOfData = True;
        return 0;
    }

    DPRINT_SEQ(m, "SHOW (i=%d, curr_sk = %p)\n", i, curr_sk);

    if (curr_sk->sk_socket)
    {
        seq_printf(m, "| 0x%p | %-9s | %2d %-10s | %5d | %-8s |  %2d  | %-9s |  %2d  | %5d | %10u 0x%08x | %10u 0x%08x |\n",
                   curr_sk->sk_socket,
                   HW_MBOX_SOCK_TYPE_GET_STR(curr_sk->sk_socket->type),
                   hwMbox_sk(curr_sk)->mid,
                   HW_MBOX_MASTER_GET_NAME(hwMbox_sk(curr_sk)->mid),
                   hwMbox_sk(curr_sk)->appid,
                   HW_MBOX_TYPE_STR(HW_MBOX_PROTOCOL_BY_APPID(hwMbox_sk(curr_sk)->appid)),
                   hwMbox_sk(curr_sk)->cid,
                   HW_MBOX_SOCK_STATE_STR(curr_sk->sk_socket->state),
                   (curr_sk->sk_socket->type == SOCK_COMMANDER) ? CHN_INTR_GET_STATE(hwMbox_sk(curr_sk)->cid) /* commander waits on Channel's WQ */
                                                                : hwMbox_sk(curr_sk)->chnIntr                 /* subject waits on Socket's WQ */,
                   hwMbox_sk(curr_sk)->owner_pid,
                   hwMbox_sk(curr_sk)->stat.sent_cnt,
                   hwMbox_sk(curr_sk)->stat.sent_bytes,
                   hwMbox_sk(curr_sk)->stat.recv_cnt,
                   hwMbox_sk(curr_sk)->stat.recv_bytes);
    }

    return 0;
}
#endif /* HW_MBOX_SOCKETS_API */

/*! \fn static int proc_dump_recvOpTagsDb_show(struct seq_file *m, void *v)
    \brief dump HW_MBOX receive Opcode Tags DB Data. */
static int proc_dump_recvOpTagsDb_show(struct seq_file *m, void *v)
{
    seq_proc_if_control_data_t *controlData = m->private;
    loff_t *spos = v;
    Uint32 i = (Uint32)(*spos);
    Uint32 recvOpIndex, tagIndex;
    DPRINT_SEQ(m, "SHOW (i=%d, *spos = %lld)\n", i,(long long)*spos);

    if (i < (HW_MBOX_OPCODE_RECV_COUNT * HW_MBOX_OPCODE_TAG_COUNT))
    {
        if (0 == i)
        {
            /* We are in the first run, print line and headers */
            seq_printf(m, "+--------+------+------------+------------+------------+\n");
            seq_printf(m, "| RECVOP | TAG  |   CB_PTR   |   TOKEN    | OPCODE_CNT |\n");
                  /* fmt: "|     ## | #### | 0x00000000 | 0x00000000 | 4294967295 |" */
        }

        /* find the correct internal indices */
        tagIndex = i % HW_MBOX_OPCODE_TAG_COUNT;

        recvOpIndex = 0;
        while ((recvOpIndex * HW_MBOX_OPCODE_TAG_COUNT) <= i) /* to avoid division... */
        {
            recvOpIndex++;
        }
        recvOpIndex--;

        if (RECV_OP_THREAD_BY_INDEX(recvOpIndex) != NULL)
        {
            if ( (tagIndex < HW_MBOX_OPCODE_TAG_COUNT) &&
                 (RECV_OP_GET_TAG_BY_INDEX(recvOpIndex, tagIndex) != HW_MBOX_TAG_INVALID) )
            {
                /* only prints assigned recvOp indices */
                seq_printf(m, "|     %2d | %4d | 0x%p | 0x%08x | %10u |\n",
                           recvOpIndex,
                           RECV_OP_GET_TAG_BY_INDEX(recvOpIndex, tagIndex),
                           RECV_OP_GET_CB_BY_INDEX(recvOpIndex, tagIndex),
                           RECV_OP_GET_TOKEN_BY_INDEX(recvOpIndex, tagIndex),
                           RECV_OP_GET_OPCODE_CNT_BY_INDEX(recvOpIndex, tagIndex));
            }
        }
        else
        {
            /* no more data, we can already end here... */
            seq_printf(m, "+--------+------+------------+------------+------------+\n");
            controlData->endOfData = True;
        }

        if (i == (HW_MBOX_OPCODE_RECV_COUNT * HW_MBOX_OPCODE_TAG_COUNT) - 1) /* Last Line */
        {
            seq_printf(m, "+--------+------+------------+------------+------------+\n");
            controlData->endOfData = True;
        }
    }

    return 0;
}

/************************************************************************/
/*     HW_MBOX Kernel Local Functions                                   */
/************************************************************************/

/**************************************************************************/
/*! \fn int hwMbox_initAfterStarted(void)
 **************************************************************************
 *  \brief Dynamically initialize the HW MBOX according to the set HW
 *         configuration, previously set by supervisor.
 *  \return [ 0 on success /(-1) on error ].
 **************************************************************************/
static int hwMbox_initAfterStarted(void)
{
    Uint32 paramCID;

    hw_mbox_channelConfig_t chnConfig;

    if (!HW_MBOX_IS_STARTED())
    {
        /* HW_MBOX is not started yet... */
        PRINTK_ERR("HW_MBOX is not started! can't learn configuration yet...");
        return (-1);
    }

    /* HW MBOX is started!!! */
    CTRL_SET_IS_STARTED(True); /* set in DB... */

    /* dynamically learn set configuration from HW: */
    DPRINTK("dynamically learn set configuration from HW...");

    /* General configuration values: */

    /* Monitor: */
    if (HW_MBOX_CTRL_MON_EXIST_GET() == True)
    {
        CTRL_SET_MONITOR(HW_MBOX_CTRL_MONITOR_GET());
        DPRINTK("Monitor = %s(%d)", HW_MBOX_MASTER_GET_NAME(CTRL_GET_MONITOR()),
                                    CTRL_GET_MONITOR());
    }
    else
    {
        CTRL_SET_MONITOR(HW_MBOX_MID_INVALID);
        DPRINTK("There's no Monitor configured...");
    }

    /* HW Debug Mode: */
    if (HW_MBOX_CTRL_DEBUG_GET() == True)
    {
        DPRINTK("HW Debug Mode is set!");
        CTRL_SET_IS_HW_DEBUG_MODE(True);
    }
    else
    {
        DPRINTK("HW Debug Mode is not set!");
        CTRL_SET_IS_HW_DEBUG_MODE(False);
    }

    /* HW Lock: */
    if (HW_MBOX_CTRL_LOCK_GET() == True)
    {
        DPRINTK("HW Lock is set!");
        CTRL_SET_IS_LOCKED(True);
    }
    else
    {
        DPRINTK("HW Lock is not set yet...");
        CTRL_SET_IS_LOCKED(False);
    }

    /* Channel Configurations: */
    for (paramCID = 0; paramCID < HW_MBOX_CHANNEL_COUNT; paramCID++)
    {
        DPRINTK("checking channel %d...", paramCID);
        if (HW_MBOX_CHN_CFG_VALID_GET(paramCID) == True)
        {
            DPRINTK("channel %d is assigned!", paramCID);

            HW_MBOX_CHANNEL_CONFIG_GET_TO_STRUCT(paramCID, &chnConfig);

            DPRINTK("found new static channel (cid=%d): %s MBOX, %s(%d) -> %s(%d) [%uB of SRAM]",
                    paramCID, HW_MBOX_TYPE_STR(chnConfig.mboxType),
                    HW_MBOX_MASTER_GET_NAME(chnConfig.commander), chnConfig.commander,
                    HW_MBOX_MASTER_GET_NAME(chnConfig.subject), chnConfig.subject,
                    chnConfig.memBuff.size);

            if ((IS_SUPERVISOR() /* supervisor gets all channels */) ||
                (IS_MONITOR()    /* monitor gets all channels */)    ||
                (chnConfig.commander == HW_MBOX_MID) ||
                (chnConfig.subject == HW_MBOX_MID))
            {
                if (!CHN_IS_ASSIGNED(paramCID))
                {
                    /* some channels might already be assigned [like initial RecvOp]... */
                    hwMbox_setChannelConfig(chnConfig);

                    if (chnConfig.memBuff.size > 0)
                    {
                        /* if this master is the commander it should reset SRAM
                           [Note: at this point it must be possible since START must be already set] */

                        if (chnConfig.commander == HW_MBOX_MID)
                        {
                            DPRINTK("cid=%d has this master as commander, do initial SRAM reset...", paramCID);
                            DPRINTK("cid=%d resetting %uB of SRAM at start offset 0x%08x...",
                                    paramCID, chnConfig.memBuff.size, chnConfig.memBuff.start);

                            HW_MBOX_RESET_SRAM(chnConfig.memBuff.start, chnConfig.memBuff.size);
                        }
                    }
                }
                else
                {
                    DPRINTK("cid=%d is already updated in DB...", paramCID);
                }

                if (!CHN_IS_RECV_OP(paramCID))
                {
                    hwMbox_setChannelValid(paramCID, True); /* also updates LUTs */
                    /* for recv Op channel valid is set by listener... */
                }
            }
        }
    }

    return 0;
}

/**************************************************************************/
/*! \fn int hwMbox_setChannelConfig(hw_mbox_channelConfig_t config)
 **************************************************************************
 *  \brief Set a channel's configuration in DB.
 *  \param[in] config - struct containing channel's configuration.
 *  \return [ 0 on success /(-1) on error ].
 **************************************************************************/
static int hwMbox_setChannelConfig(hw_mbox_channelConfig_t config)
{
    static int recvOpcodeChannelsCount = 0;
    Uint32 paramCID;
    Int32 retVal = 0;

    paramCID = config.cid;

    /* Verify channel configuration validity: */

    if (!HW_MBOX_MID_VALIDITY_CHECK(config.commander))
    {
        PRINTK_ERR("channel configuration is not valid! [invalid commander value (%d)]",
                   config.commander);
        return (-1);
    }

    if (!HW_MBOX_MID_VALIDITY_CHECK(config.subject))
    {
        PRINTK_ERR("channel configuration is not valid! [invalid subject value (%d)]",
                   config.subject);
        return (-1);
    }

    if (!HW_MBOX_CID_VALIDITY_CHECK(config.cid))
    {
        PRINTK_ERR("channel configuration is not valid! [invalid cid value (%u)]",
                   config.cid);
        return (-1);
    }

    if (!HW_MBOX_TYPE_IS_VALID(config.mboxType))
    {
        PRINTK_ERR("channel configuration is not valid! [invalid mbox type value (%d)]",
                   config.mboxType); 
        return (-1);
    }

    DPRINTK("cid %d: update configuration in DB", paramCID);

    /* recv Opcode channel... */
    if ( (config.mboxType == hw_mbox_opcode) &&
         (config.subject == HW_MBOX_MID) )
    {
        if (recvOpcodeChannelsCount == HW_MBOX_OPCODE_RECV_COUNT)
        {
            PRINTK_ERR("cid %d cannot be configured! no more room for recv Opcode Channels [current recv Opcode Channel Count = %d]...",
                       paramCID, recvOpcodeChannelsCount);

            return (-1);
        }
    }

    /* Copy configuration to the DB: */
    memcpy(&CHN_CFG(paramCID), &config, HW_MBOX_CHANNEL_CONFIG_SIZE);

#ifdef HW_MBOX_EXTENDED_MEMORY
    /* Opcode Channels between NP-CPU and APP-CPU use EXTENDED Memory: */
    if ( ( ((HW_MBOX_MASTER_NP_CPU == config.commander) && (HW_MBOX_MASTER_APP_CPU == config.subject)) ||
           ((HW_MBOX_MASTER_APP_CPU == config.commander) && (HW_MBOX_MASTER_NP_CPU == config.subject)) ) &&
         (hw_mbox_opcode == config.mboxType) )
    {
        CHN_CFG_SET_EXTMEM(paramCID, True);

        /* Hardcoded configuration:
           Part A is allocated to NP-CPU ---> APP-CPU;
           Part B is allocated to APP-CPU ---> NP-CPU;
        */
        if ( (HW_MBOX_MASTER_NP_CPU == config.commander) && (HW_MBOX_MASTER_APP_CPU == config.subject) )
        {
            /* NP-CPU ---> APP-CPU: */
            CHN_CFG_EXTMEM_SET_OFFSET(paramCID, HW_MBOX_EXTMEM_PART_A_OFFSET);
        }
        else
        {
            /* APP-CPU ---> NP-CPU: */
            CHN_CFG_EXTMEM_SET_OFFSET(paramCID, HW_MBOX_EXTMEM_PART_B_OFFSET);
        }

        /* Size is the same for all parts: */
        CHN_CFG_EXTMEM_SET_SIZE(paramCID, HW_MBOX_EXTMEM_PART_SIZE);

        DPRINTK("cid %d: configured EXTMEM - offset=0x%08x, size=0x%08x...",
                paramCID,
                CHN_CFG_EXTMEM_GET_OFFSET(paramCID),
                CHN_CFG_EXTMEM_GET_SIZE(paramCID));

        /* clear the ownership flag if commander */
        if (HW_MBOX_MID == config.commander)
        {
            hwMbox_extMemCtrlHdr_t extMemHdr = { 0 };

            DPRINTK("cid %d: clear EXTMEM header at extMem offset=0x%08x...",
                    paramCID, CHN_CFG_EXTMEM_GET_OFFSET(paramCID));

            HW_MBOX_CLR_OWNER(extMemHdr);
            HW_MBOX_REG32_WRITE(HW_MBOX_EXTMEM_ADDR_BY_OFFSET(CHN_CFG_EXTMEM_GET_OFFSET(paramCID)), extMemHdr);

            DPRINTK("cid %d: entering reading loop on header at extMem offset=0x%x (current value 0x%08x)...",
                    paramCID, CHN_CFG_EXTMEM_GET_OFFSET(paramCID),
                    HW_MBOX_REG32_READ(HW_MBOX_EXTMEM_ADDR_BY_OFFSET(CHN_CFG_EXTMEM_GET_OFFSET(paramCID))));

            /* wait till ownership bit will be changed */
            do
            {
                extMemHdr = HW_MBOX_REG32_READ(HW_MBOX_EXTMEM_ADDR_BY_OFFSET(CHN_CFG_EXTMEM_GET_OFFSET(paramCID)));
                DPRINTK("cid %d: reading loop on header at extMem offset=0x%x (current value 0x%08x)...",
                        paramCID, CHN_CFG_EXTMEM_GET_OFFSET(paramCID), extMemHdr);
            } while (!HW_MBOX_EXTMEM_IS_COMMANDER_OWNER(extMemHdr) /* Loop until the commander the owner */);

            DPRINTK("cid %d: exited from reading loop...", paramCID);
        }
    }
    else
    {
        CHN_CFG_SET_EXTMEM(paramCID, False);
    }
#endif /* HW_MBOX_EXTENDED_MEMORY */

    CHN_SET_ASSIGNED(paramCID, True);

    if ( (config.mboxType == hw_mbox_opcode) &&
         (config.subject == HW_MBOX_MID) )
    {
        /* recv Opcode channel: create the required infrastructure... */

        if (NULL == RECV_OP_THREAD_BY_INDEX(recvOpcodeChannelsCount))
        {
            /* initialize the listener:
               This is not required for the first Receive Opcode Listener... */
            RECV_OP_THREAD_SET_CID(recvOpcodeChannelsCount, paramCID);
            RECV_OP_THREAD_SET_INDEX(recvOpcodeChannelsCount);

            RECV_OP_THREAD_BY_INDEX(recvOpcodeChannelsCount) = kthread_run(hwMbox_recvOpcodeListenerEntry,
                                                                           &RECV_OP_THREAD_DATA(recvOpcodeChannelsCount),
                                                                           RECV_OPCODE_LISTENER_PREFIX"%02d",
                                                                           recvOpcodeChannelsCount);
            if (IS_ERR(RECV_OP_THREAD_BY_INDEX(recvOpcodeChannelsCount)))
            {
                retVal = PTR_ERR(RECV_OP_THREAD_BY_INDEX(recvOpcodeChannelsCount));
                PRINTK_ERR("Creating %s%02d for CID %d Failed!!! [error %d]",
                           RECV_OPCODE_LISTENER_PREFIX, recvOpcodeChannelsCount, paramCID, retVal);

                RECV_OP_THREAD_BY_INDEX(recvOpcodeChannelsCount) = NULL;
                CHN_RECV_OPCODE_SET_INDEX(paramCID, CHN_RECV_OPCODE_INDEX_INVALID);

                return (-1);
            }
        }

        CHN_RECV_OPCODE_SET_INDEX(paramCID, recvOpcodeChannelsCount);
        recvOpcodeChannelsCount++;
    }
    else
    {
        /* not a receive Opcode channel... */
        CHN_RECV_OPCODE_SET_INDEX(paramCID, CHN_RECV_OPCODE_INDEX_INVALID);
    }

    return 0;
}

/**************************************************************************/
/*! \fn int hwMbox_setChannelValid(Uint32 paramCID, Bool isValid)
 **************************************************************************
 *  \brief Sets channel <paramCID> as valid/invalid according to <isValid>
 *         in DB.
 *  \note Also updates the LUTs.
 *  \param[in] paramCID - which channel.
 *  \param[in] isValid - valid/invalid.
 *  \return 0 on success, negative error code otherwise.
 **************************************************************************/
static int hwMbox_setChannelValid(Uint32 paramCID, Bool isValid)
{
    hw_mbox_type_e paramProto;

    if (!HW_MBOX_CID_VALIDITY_CHECK(paramCID))
    {
        PRINTK_ERR("invalid cid value! [paramCID=%u]",
                   paramCID);
        return -EINVAL;
    }

    DPRINTK("cid %d: set%s valid in DB",
            paramCID, ((isValid) ? ("") : (" NOT")));

    paramProto = CHN_CFG_GET_TYPE(paramCID);

    if (!HW_MBOX_TYPE_IS_VALID(paramProto)) 
    {
        PRINTK_ERR("invalid proto value in DB! [paramCID=%u, paramProto=%d]",
                   paramCID, paramProto);
        return -EINVAL;
    }

    if (!CHN_IS_ASSIGNED(paramCID))
    {
        PRINTK_ERR("Trying to modify Valid for channel %d that has not been assigned!",
                   paramCID);
        return -EPERM;
    }

    CHN_SET_VALID(paramCID, isValid);

    if ((CHN_CFG_GET_COMMANDER(paramCID) != HW_MBOX_MID) &&
        (CHN_CFG_GET_SUBJECT(paramCID) != HW_MBOX_MID))
    {
        /* Master is not part of this channel... Don't add to LUTs!!!
           (possible because supervisor sets valid for all channels) */
        return 0;
    }

    if (paramProto == hw_mbox_event)
    {
        /* Kernel doesn't currently support event channels */
        DPRINTK("channel %d will not be set in LUT, as Kernel doesn't support event Channels yet!",
                paramCID);
        return 0;
    }

    /* Update CID LUT: */
    if (CHN_CFG_GET_COMMANDER(paramCID) == HW_MBOX_MID)
    {
        /* Master is commander on this channel [peer is subject]: */
        SET_CID_IN_LUT(CHN_CFG_GET_SUBJECT(paramCID),
                       paramProto,
                       False,
                       ((isValid) ? (paramCID) : (HW_MBOX_CID_INVALID)));
    }
    else if (CHN_CFG_GET_SUBJECT(paramCID) == HW_MBOX_MID)
    {
        /* Master is subject on this channel [peer is commander]: */
        SET_CID_IN_LUT(CHN_CFG_GET_COMMANDER(paramCID),
                       paramProto,
                       True,
                       ((isValid) ? (paramCID) : (HW_MBOX_CID_INVALID)));
    }

    return 0;
}

/**************************************************************************/
/*! \fn int hwMbox_waitForChannelInterrupt(int cid, int timeoutInMsec)
 **************************************************************************
 *  \brief wait for channel interrupt on <cid>
 *  \note receive opcode channel waits on a different queue [Application Queue,
 *        owned by the listener].
 *  \param[in] cid - cid to wait on.
 *  \param[in] timeoutInMsec - timeout [use CHN_INTR_NO_TIMEOUT for no timeout].
 *  \param[out] pHwMboxStatus - extended status information.
 *  \note this function is blocking, returning only upon channel interrupt [or error].
 *  \note this function also clears the channel interrupt.
 *  \attnetion this is just the wait, if you need a lock then acquire it before...
 *  \return [ 0 on success / error otherwise ].
 **************************************************************************/
static int hwMbox_waitForChannelInterrupt(int cid, int timeoutInMsec, HW_MBOX_STATUS_e *pHwMboxStatus)
{
    long rc = 0;
    wait_queue_head_t *pWq; /* pointer to the wait queue we should wait on */
    chnIntrState_e *pState; /* pointer to the channel interrupt state we should query */

    DPRINTK("cid %d: wait for channel interrupts...", cid);

    *pHwMboxStatus = HW_MBOX_STATUS_OK;

    if (CHN_IS_RECV_OP(cid))
    {
        /* Receive Opcode channel should wait on application queue */
        pWq = &RECV_OP_APP_WQ(cid);
        pState = &RECV_OP_APP_GET_STATE(cid);
        DPRINTK("cid %d: using application queue...", cid);
    }
    else
    {
        /* all other cases, wait on channel interrupt */
        pWq = &CHN_INTR_WQ(cid);
        pState = &CHN_INTR_GET_STATE(cid);
        DPRINTK("cid %d: using channel queue...", cid);
    }

    DPRINTK("cid %d: pWQ=%p, pSTATE=%p", cid, pWq, pState);

    /* wait for channel interrupts, on wait queue */
    if (timeoutInMsec == CHN_INTR_NO_TIMEOUT)
    {
        DPRINTK("cid %d: no timeout...", cid);
        if ( wait_event_interruptible(*pWq,
                                      *pState != CHN_INTR_STATE_NONE) )
        {
            DPRINTK("cid %d, wait interrupted by signal!", cid);
            *pHwMboxStatus = HW_MBOX_STATUS_INTERRUPTED_BY_SIGNAL;
            return (-1);
        }
    }
    else
    {
        DPRINTK("cid %d: using timeout %d MSEC", cid, timeoutInMsec);
        rc = wait_event_interruptible_timeout(*pWq,
                                              *pState != CHN_INTR_STATE_NONE,
                                              msecs_to_jiffies(timeoutInMsec));

        DPRINTK("cid %d: rc=%ld", cid, rc);

        /* if evaluated to True before timeout expires
           returns the remaining amount of jiffies, else errors: */
        if (rc <= 0)
        {
            if (0 == rc)
            {
                DPRINTK("cid %d: timeout expired!", cid);
                *pHwMboxStatus = HW_MBOX_STATUS_ERROR_TIMEOUT_EXPIRED;
                return (-1);
            }
            else
            {
                DPRINTK("cid %d: wait interrupted by signal!", cid);
                *pHwMboxStatus = HW_MBOX_STATUS_INTERRUPTED_BY_SIGNAL;
                return(-1);
            }
        }
    }

    if (*pState != CHN_INTR_STATE_PENDING)
    {
        /* required if we will support more channel interrupt states... */
        DPRINTK("cid %d: channel interrupt state is not pending! state=%d", cid, *pState);
    }

    *pState = CHN_INTR_STATE_NONE; /* we got the state, set to none... */
    return 0;
}

/**************************************************************************/
/*! \fn int hwMbox_recvOpcodeListenerEntry(void *data)
 **************************************************************************
 *  \brief entry function for a receive opcode listener.
 **************************************************************************/
static int hwMbox_recvOpcodeListenerEntry(void *data)
{
#define LOG_RECVOP_LISTENER(fmt, args...) \
    DPRINTK("[RECV_OP_LISTENER#%d ON CID#%d]: " fmt, threadData->index, threadData->cid, ## args)

    recvOpcodeListenerThreadData_t *threadData; /* This listener's data */

    int i, cbRc, paramMID;
    Uint32 cmdRegVal; /* also re-used for listener replies */
    Uint32 paramToken;

    hwMbox_opcode_t opFlds = { 0 };
    hwMbox_opcodeMessage_t opcodeMsg;
    hwMbox_opcode_t replyOpFlds = { 0 };
    hwMbox_opcodeMessage_t replyOpcodeMsg;

    hw_mbox_channelConfig_t chnCfg = { 0 }; /* used only if first listener... */

    hwMboxRecvOpcodeCallback recvOpCb;

    threadData = data;

    if ( IS_SUPERVISOR() || !IS_RECV_OPCODE_INIT_INDEX(threadData->index) )
    {
        /* Supervisor should never have to create a Listener to be used for initialization
           Non-supervisor should only have the first Listener be used for intialization
        */

        DPRINTK("start waiting for INITIALIZED [RECV OPCODE LISTENER#%d on CID#%d]",
                threadData->index, threadData->cid);

        if( wait_event_interruptible(waitForInitializedWq, IS_INITIALIZED()) )
        {
            PRINTK_ERR("*** wait on waitForInitializedWq interrupted by signal [RECV OPCODE LISTENER#%d on CID#%d] ***",
                       threadData->index, threadData->cid);
            return -ERESTARTSYS;
        }

        DPRINTK("RECV OPCODE LISTENER#%d kernel thread on CID#%d started running...",
                threadData->index, threadData->cid);
    }
    else
    {
        /* Driver is waiting for INIT_COMPLETE from supervisor,
           this is the first Listener used for initialization!

           It should wait for INIT_COMPLETE notification,
           It should then set its own configuration in DB...

           IMPORTANT Note:
           Non-supervisor should not use the IS_INIT_COMPLETE as indication of first listener
           since there's a chance the interrupt is already waiting [and IS_INIT_COMPLETE is
           already signaled] while the first listener is being created [we can use the index
           to know if it's the first listener].

           Thread sets its own configuration so that ISR can do the bare possible minimum...
        */

        PRINTK_INFO("First Listener thread used for INITIALIZATION started running... [RECV OPCODE LISTENER#%d]",
                    threadData->index);

        DPRINTK("start waiting for INIT_COMPLETE from supervisor");

        if( wait_event_interruptible(waitForInitCompleteWq, IS_INIT_COMPLETE()) )
        {
            DPRINTK("First Listener thread interrupted by signal while waiting for INIT_COMPLETE...");
            PRINTK_ERR("*** CRITICAL ERROR in HW MBOX initialization, no HW MBOX services will be avaiable ***");
            return -ERESTARTSYS;
        }

        /* INIT_COMPLETE signaled by ISR!
           correct CID was set in thread Data by ISR, listener should do the rest... */

        LOG_RECVOP_LISTENER("driver NOT initialized yet, This is the first Listener!");
        LOG_RECVOP_LISTENER("set channel configuration in DB...");

        HW_MBOX_CHANNEL_CONFIG_GET_TO_STRUCT(threadData->cid, &chnCfg);
        hwMbox_setChannelConfig(chnCfg);
    }

    while (!CHN_IS_ASSIGNED(threadData->cid))
    {
        /* Make sure channel configuration has been written to DB... */
        usleep_range(20, 100);
    }

    /* setup any required infrastructure: */
    init_waitqueue_head(&RECV_OP_APP_WQ(threadData->cid));
    RECV_OP_APP_SET_STATE(threadData->cid, CHN_INTR_STATE_NONE);

    /* once all required infrastructure is ready we can set valid... */
    hwMbox_setChannelValid(threadData->cid, True); /* also updates LUTs */

    while (1)
    {
        /* cannot use hwMbox_waitForChannelInterrupt as it will redirect us to application
           queue and we should wait on Channel Queue [without Timeout]... */

        LOG_RECVOP_LISTENER("start waiting for channel interrupts...");

        if ( wait_event_interruptible(CHN_INTR_WQ(threadData->cid),
                                      CHN_INTR_GET_STATE(threadData->cid) != CHN_INTR_STATE_NONE) )
        {
            LOG_RECVOP_LISTENER("wait interrupted by signal!");
            continue;
        }

        if (CHN_INTR_GET_STATE(threadData->cid) != CHN_INTR_STATE_PENDING)
        {
            /* required if we will support more channel interrupt states... */
            LOG_RECVOP_LISTENER("channel interrupt state is not pending! state=%d", CHN_INTR_GET_STATE(threadData->cid));
        }

        CHN_INTR_SET_STATE(threadData->cid, CHN_INTR_STATE_NONE); /* we got the state, set to none... */

        RECV_OP_INC_WAKEUP_COUNT(threadData->cid);

        LOG_RECVOP_LISTENER("received channel interrupt!");

        /* Get the opcode value: */
        cmdRegVal = HW_MBOX_CHN_COMMAND_REG_GET(threadData->cid);

        /* Parse the opcode fields to struct: */
        HW_MBOX_OPCODE_FIELDS_GET_TO_STRUCT(cmdRegVal, &opFlds);

        LOG_RECVOP_LISTENER("cmdRegVal=0x%08x -> opcode fields: tag=%u, lenInBytes=%u",
                            cmdRegVal, opFlds.tag, opFlds.lenInBytes);

        /* Setup the structs for listener replies [HW MBOX opcode message of type STATUS]: */
        replyOpFlds.tag = HW_MBOX_OPCODE_TAG;
        replyOpFlds.lenInBytes = HW_MBOX_OPCODE_MESSAGE_SIZE;
        replyOpcodeMsg.msgType = HW_MBOX_OPCODE_MESSAGE_TYPE_STATUS;

        /* First handle Kernel HW MBOX tag: */
        if (opFlds.tag == HW_MBOX_OPCODE_TAG)
        {
            LOG_RECVOP_LISTENER("Handling Kernel HW MBOX OPCODE TAG(%u)", HW_MBOX_OPCODE_TAG);

            /* Verify message validity: */
            if (opFlds.lenInBytes != HW_MBOX_OPCODE_MESSAGE_SIZE)
            {
                LOG_RECVOP_LISTENER("invalid message for tag %u [got size %uB, excpected size %uB]!",
                                    HW_MBOX_OPCODE_TAG, opFlds.lenInBytes, HW_MBOX_OPCODE_MESSAGE_SIZE);

                /* Invalid message, prepare the response: */
                replyOpcodeMsg.msgData.msgStatus.status = HW_MBOX_STATUS_OPCODE_ERROR_INVALID_MESSAGE;

                goto SEND_OP_REPLY;
            }

            LOG_RECVOP_LISTENER("copying %uB [out of total %uB channel has] from SRAM at offset=0x%08x",
                                HW_MBOX_OPCODE_MESSAGE_SIZE,
                                CHN_CFG_MEMBUFF_GET_SIZE(threadData->cid),
                                CHN_CFG_MEMBUFF_GET_START(threadData->cid));

            /* Get the opcode message from the SRAM: */
            HW_MBOX_INTERNAL_MESSAGE_COPY_FROM_SRAM(CHN_CFG_MEMBUFF_GET_START(threadData->cid),
                                                    &opcodeMsg);

            LOG_RECVOP_LISTENER("incoming message is of type %d", opcodeMsg.msgType);

            if (!IS_INITIALIZED())
            {
                /* Driver is not initialized yet... */
                LOG_RECVOP_LISTENER("driver NOT initialized yet...");

                /* At this point message must be of type CODE
                   and Code should be INIT_COMPLETE... */

                if (opcodeMsg.msgType != HW_MBOX_OPCODE_MESSAGE_TYPE_CODE)
                {
                    /* shouldn't happen, kept here as a safety precaution... */
                    LOG_RECVOP_LISTENER("First opcode message should be of type code (%u) [got message of type %u]...",
                                        HW_MBOX_OPCODE_MESSAGE_TYPE_CODE, opcodeMsg.msgType);

                    /* Driver not initialized! prepare the response: */
                    replyOpcodeMsg.msgData.msgStatus.status = HW_MBOX_STATUS_DRIVER_NOT_INITIALIZED;

                    goto SEND_OP_REPLY;
                }

                /* Opcode message is of type CODE: */
                LOG_RECVOP_LISTENER("incoming code message has code %u",
                                    opcodeMsg.msgData.msgCode.code);

                if (opcodeMsg.msgData.msgCode.code != HW_MBOX_CODE_INIT_COMPLETE)
                {
                    /* shouldn't happen, kept here as a safety precaution... */
                    LOG_RECVOP_LISTENER("First opcode code message should have INIT_COMPLETE code (%u) [got code message with code %u]...",
                                        HW_MBOX_CODE_INIT_COMPLETE, opcodeMsg.msgData.msgCode.code);

                    /* Driver not initialized! prepare the response: */
                    replyOpcodeMsg.msgData.msgStatus.status = HW_MBOX_STATUS_DRIVER_NOT_INITIALIZED;

                    goto SEND_OP_REPLY;
                }

                LOG_RECVOP_LISTENER("Handling INIT_COMPLETE code (%u)", HW_MBOX_CODE_INIT_COMPLETE);
                hwMbox_initAfterStarted();

                /* Currently hwMbox_initAfterStarted will always succeed here, since it is called in response
                   to an interrupt so HW MBOX must be started and the function can only fail if it is called
                   and start is not set yet...
                */

                for (paramMID = 0; paramMID < HW_MBOX_MASTER_COUNT; paramMID++)
                {
                    if (IS_MASTER_VALID(paramMID))
                    {
                        /* Currently for non-supervisor,
                           all valid masters are considered initialized. */
                        MASTER_SET_INITIALIZED(paramMID, True);
                    }
                }

                /* Let all driver services that the device is initialized! */
                SET_INITIALIZED();

                /* Success! */
                LOG_RECVOP_LISTENER("INIT_COMPLETE! report back to supervisor...");
                PRINTK_INFO("HW MBOX INITIALIZATION complete!");

                /* Prepare the response: */
                replyOpcodeMsg.msgData.msgStatus.status = HW_MBOX_STATUS_OK;
            }
            else
            {
                /* Initialized! */
                LOG_RECVOP_LISTENER("driver is initialized!!!");

                switch (opcodeMsg.msgType)
                {
                    case HW_MBOX_OPCODE_MESSAGE_TYPE_CODE:
                        LOG_RECVOP_LISTENER("Handling messages of type CODE (%d)!", opcodeMsg.msgType);

                        switch (opcodeMsg.msgData.msgCode.code)
                        {
                            case HW_MBOX_CODE_INIT_COMPLETE:
                                LOG_RECVOP_LISTENER("Handling INIT_COMPLETE code %u", HW_MBOX_CODE_INIT_COMPLETE);
                                LOG_RECVOP_LISTENER("Driver already initialized! report back...");
                                replyOpcodeMsg.msgData.msgStatus.status = HW_MBOX_STATUS_OK;
                                break;

                            case HW_MBOX_CODE_PING:
                                LOG_RECVOP_LISTENER("Handling PING code %u", HW_MBOX_CODE_PING);
                                replyOpcodeMsg.msgData.msgStatus.status = HW_MBOX_STATUS_OK;    /* ACK ping */
                                break;

                            default:
                                LOG_RECVOP_LISTENER("invalid opcode CODE message code value %u!",
                                                    opcodeMsg.msgData.msgCode.code);
                                replyOpcodeMsg.msgData.msgStatus.status = HW_MBOX_STATUS_OPCODE_ERROR;
                        }
                        break;

                    case HW_MBOX_OPCODE_MESSAGE_TYPE_STATUS:
                        LOG_RECVOP_LISTENER("incoming message of type STATUS %d should not be received at this point!",
                                            opcodeMsg.msgType);
                        replyOpcodeMsg.msgData.msgStatus.status = HW_MBOX_STATUS_OPCODE_ERROR_ILLEGAL_MESSAGE;
                        break;

                    default:
                        LOG_RECVOP_LISTENER("incoming message has invalid type %d!",
                                            opcodeMsg.msgType);

                        replyOpcodeMsg.msgData.msgStatus.status = HW_MBOX_STATUS_OPCODE_ERROR_INVALID_MESSAGE;
                }
            }

            goto SEND_OP_REPLY;
        }

        if (!IS_INITIALIZED())
        {
            /* if we are here, driver is not initialized yet and it's a non Kernel HW MBOX tag... */
            LOG_RECVOP_LISTENER("driver NOT initialized! can't handle tag %u yet...", opFlds.tag);
            replyOpcodeMsg.msgData.msgStatus.status = HW_MBOX_STATUS_DRIVER_NOT_INITIALIZED;
            goto SEND_OP_REPLY;
        }

        if (opFlds.tag == HW_MBOX_OPCODE_USER_SPACE_TAG)
        {
            /* it's an opcode for HW MBOX in User-Space! signal APP Queue */
            LOG_RECVOP_LISTENER("HW MBOX User-Space opcode! signaling APP queue...");

            RECV_OP_APP_SET_STATE(threadData->cid, CHN_INTR_STATE_PENDING);
            wake_up_interruptible(&RECV_OP_APP_WQ(threadData->cid));

            continue;
        }

        /* start checking the registered callbacks... */
        LOG_RECVOP_LISTENER("start checking the registered callbacks...");
        recvOpCb = NULL;

        /* seize channel lock */
        if (down_interruptible(&CHN_LOCK(threadData->cid)))
        {
            LOG_RECVOP_LISTENER("down interrupted by signal...");
            continue;
        }

        for (i = 0; i < HW_MBOX_OPCODE_TAG_COUNT; i++)
        {
            if (RECV_OP_GET_TAG(threadData->cid, i) == opFlds.tag)
            {
                /* found the correct tag! */
                recvOpCb = RECV_OP_GET_CB(threadData->cid, i);
                LOG_RECVOP_LISTENER("found tag %u in DB [index %d], invoking callback %p, token %u=0x%08x",
                                    opFlds.tag, i, recvOpCb,
                                    RECV_OP_GET_TOKEN(threadData->cid, i),
                                    RECV_OP_GET_TOKEN(threadData->cid, i));

                RECV_OP_INC_OPCODE_CNT(threadData->cid, i);
                break;
            }
        }

        if (recvOpCb == NULL)
        {
#ifdef HW_MBOX_SOCKETS_API
#ifdef HW_MBOX_IS_NP_CPU
            /* For NP-CPU forward only PM tags to user-space */
            if ( (opFlds.tag == NPCPU_APPCPU_HW_MBOX_TAG_BOOT) ||
                 (opFlds.tag == NPCPU_APPCPU_HW_MBOX_TAG_PM)   ||
                 (opFlds.tag == NPCPU_APPCPU_HW_MBOX_TAG_LAN)  ||
                 (opFlds.tag == NPCPU_APPCPU_HW_MBOX_TAG_MOCA) )
            {
                /* no registered kernel tag found... */
                /* This means that if it's a valid tag it's for User-Space! */
                LOG_RECVOP_LISTENER("tag %u is not for kernel! send to User-Space...", opFlds.tag);

                RECV_OP_APP_SET_STATE(threadData->cid, CHN_INTR_STATE_PENDING);
                wake_up_interruptible(&RECV_OP_APP_WQ(threadData->cid));

                /* release Lock... */
                up(&CHN_LOCK(threadData->cid));
                continue;
            }
            else
#endif /* HW_MBOX_IS_NP_CPU */
            {
                LOG_RECVOP_LISTENER("tag %u is invalid! send back internal error message...", opFlds.tag);

                replyOpcodeMsg.msgData.msgStatus.status = HW_MBOX_STATUS_OPCODE_ERROR_INVALID_TAG;

                RECV_OP_INC_INVALID_TAG_COUNT(threadData->cid);

                /* release Lock... */
                up(&CHN_LOCK(threadData->cid));
                goto SEND_OP_REPLY;
            }
#else /* HW_MBOX_SOCKETS_API */
            /* no registered kernel tag found... */
            LOG_RECVOP_LISTENER("tag %u is not for kernel!", opFlds.tag);

#ifndef HW_MBOX_IS_APP_CPU
            /* This means that if it's a valid tag it's for User-Space! */
            LOG_RECVOP_LISTENER("Send tag %u to User-Space...", opFlds.tag);

            RECV_OP_APP_SET_STATE(threadData->cid, CHN_INTR_STATE_PENDING);
            wake_up_interruptible(&RECV_OP_APP_WQ(threadData->cid));

            /* release Lock... */
            up(&CHN_LOCK(threadData->cid));
            continue;
#else /* ! HW_MBOX_IS_APP_CPU */

            /* APP-CPU does not support User-space HW MBOX SW,
               so it must be an invalid tag! */
            LOG_RECVOP_LISTENER("tag %u is invalid! send back internal error message...", opFlds.tag);

            replyOpcodeMsg.msgData.msgStatus.status = HW_MBOX_STATUS_OPCODE_ERROR_INVALID_TAG;

            RECV_OP_INC_INVALID_TAG_COUNT(threadData->cid);

            /* release Lock... */
            up(&CHN_LOCK(threadData->cid));
            goto SEND_OP_REPLY;
#endif /* HW_MBOX_IS_APP_CPU */
#endif /* ! HW_MBOX_SOCKETS_API */
        }

        paramToken = RECV_OP_GET_TOKEN(threadData->cid, i);

        cbRc = recvOpCb(CHN_CFG_GET_COMMANDER(threadData->cid),
                        HW_MBOX_POINTER_TO_SRAM(CHN_CFG_MEMBUFF_GET_START(threadData->cid)),
                        opFlds.lenInBytes,
                        &paramToken);

        LOG_RECVOP_LISTENER("callback retuned with %d, paramToken after callback %u=0x%08x, channel state is %d.",
                            cbRc, paramToken, paramToken, HW_MBOX_CHN_STATE_REG_GET(threadData->cid));

        if (cbRc == -1)
        {
            /* reserved return value indicating the the Callback failed!
               Token should hold a generic HW MBOX return code <HW_MBOX_STATUS_e>
               that will indicate of the error cause [i.e. ILLEGAL_MESSAGE]. */

            LOG_RECVOP_LISTENER("callback failed, sending back error 0x%08x (%u)",
                                paramToken, paramToken);

            replyOpcodeMsg.msgData.msgStatus.status = paramToken;

            /* release Lock... */
            up(&CHN_LOCK(threadData->cid));
            goto SEND_OP_REPLY;
        }
        else
        {
            RECV_OP_SET_TOKEN(threadData->cid, i, paramToken);    /* update in DB the possibly modified Token */
        }

        /* Opcode successfully received, INC count! */
        STATS_INC_COUNTER(CHN_CFG_GET_COMMANDER(threadData->cid), hw_mbox_opcode, STATS_PEER_IS_COMMANDER);

        /* release Lock... */
        up(&CHN_LOCK(threadData->cid));
        continue;

SEND_OP_REPLY:
        /* Opcode successfully received, INC count! */
        STATS_INC_COUNTER(CHN_CFG_GET_COMMANDER(threadData->cid), hw_mbox_opcode, STATS_PEER_IS_COMMANDER);

        LOG_RECVOP_LISTENER("send opcode reply to subject!");

        /* Use the current values in the Reply OPCODE message and the Reply OPCODE fields structs: */
        LOG_RECVOP_LISTENER("sending back opcode message of type STATUS (%d), with status value 0x%x (%d)",
                            replyOpcodeMsg.msgType,
                            replyOpcodeMsg.msgData.msgStatus.status, replyOpcodeMsg.msgData.msgStatus.status);

        LOG_RECVOP_LISTENER("copying %uB [out of total %uB channel has] to SRAM at offset=0x%08x",
                            replyOpFlds.lenInBytes, CHN_CFG_MEMBUFF_GET_SIZE(threadData->cid), CHN_CFG_MEMBUFF_GET_START(threadData->cid));

        /* Copy the opcode message to the SRAM: */
        HW_MBOX_INTERNAL_MESSAGE_COPY_TO_SRAM(&replyOpcodeMsg,
                                              CHN_CFG_MEMBUFF_GET_START(threadData->cid));

        /* Create opcode reply value: */
        HW_MBOX_OPCODE_FIELDS_SET_FROM_STRUCT(cmdRegVal, &replyOpFlds);

        LOG_RECVOP_LISTENER("opcode fields - tag=%u, lenInBytes=%u -> cmdRegVal=0x%08x",
                            replyOpFlds.tag, replyOpFlds.lenInBytes, cmdRegVal);

        LOG_RECVOP_LISTENER("before write to command register - state=%d", HW_MBOX_CHN_STATE_REG_GET(threadData->cid));
        LOG_RECVOP_LISTENER("writing 0x%08x to the command register of the channel...", cmdRegVal);
        HW_MBOX_CHN_COMMAND_REG_SET(threadData->cid, cmdRegVal); /* send Command... */
        LOG_RECVOP_LISTENER("after write to command register - state=%d", HW_MBOX_CHN_STATE_REG_GET(threadData->cid));
    }

    return 0;
#undef LOG_RECVOP_LISTENER
}

/*** Channel interrupt Devices Functions Definitions: ***/

/* File Operations: */

static int chnint_open(struct inode *inode, struct file *filp)
{
    chnint_t *dev;
    dev = container_of(inode->i_cdev, chnint_t, chnint_cdev);
    DEV_PRINTK("chnint_open dev %p, chn_cid %d", dev, dev->chn_cid);
    DEV_PRINTK("filp=%p", filp);

    if ((filp->f_flags & O_NONBLOCK) && (!IS_INITIALIZED()))
    {
        /* support non blocking open operation... */
        return -EAGAIN;
    }
    else
    {
        /* otherwise block and wait for driver initialization! */
        if( wait_event_interruptible(waitForInitializedWq, IS_INITIALIZED()) )
        {
            /* following return code used internally by VFS (virtual filesystem) layer.
               it will either restart the system call or return -EINTR to User-Space.
               (to indicate that it was interrupted by signal...) */

            return -ERESTARTSYS;
        }
    }

    filp->private_data = dev;   /* store for future use */
    return 0;
}

static ssize_t chnint_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    HW_MBOX_STATUS_e hwMboxStatus;
    Bool replyToUserBuffer; /* initial preparation for full channel devices */

    chnint_t *dev = filp->private_data; /* dev was stored in filp during open */

    DEV_PRINTK("chnint_read dev %p, chn_cid %d, interruptPending=%d", dev, dev->chn_cid, CHN_INTR_GET_STATE(dev->chn_cid));
    DEV_PRINTK("filp=%p, count=%u, *f_pos=%lld", filp, count, (long long)*f_pos);

    if (hwMbox_waitForChannelInterrupt(dev->chn_cid, CHN_INTR_NO_TIMEOUT, &hwMboxStatus))
    {
        /* error, no timeout so it can only be interrupted by signal... */
        if (hwMboxStatus == HW_MBOX_STATUS_INTERRUPTED_BY_SIGNAL)
        {
            /* following return code used internally by VFS (virtual filesystem) layer.
               it will either restart the system call or return -EINTR to User-Space.
               (to indicate that it was interrupted by signal...) */

            return -ERESTARTSYS;
        }
    }

    replyToUserBuffer = True;

    if (copy_to_user(buf, &replyToUserBuffer, sizeof(replyToUserBuffer)))
    {
        return -EFAULT;
    }

    *f_pos += sizeof(replyToUserBuffer);
    return sizeof(replyToUserBuffer);
}

static unsigned int chnint_poll(struct file *filp, struct poll_table_struct *wait)
{
    chnint_t *dev = filp->private_data; /* dev was stored in filp during open */
    unsigned int mask = 0;

    wait_queue_head_t *pWq; /* pointer to the wait queue we should wait on */
    chnIntrState_e *pState; /* pointer to the channel interrupt state we should query */

    if (CHN_IS_RECV_OP(dev->chn_cid))
    {
        /* Receive Opcode channel should wait on application queue */
        pWq = &RECV_OP_APP_WQ(dev->chn_cid);
        pState = &RECV_OP_APP_GET_STATE(dev->chn_cid);
        DEV_PRINTK("cid %d: using application queue...", dev->chn_cid);
    }
    else
    {
        /* all other cases, wait on channel interrupt */
        pWq = &CHN_INTR_WQ(dev->chn_cid);
        pState = &CHN_INTR_GET_STATE(dev->chn_cid);
        DEV_PRINTK("cid %d: using channel queue...", dev->chn_cid);
    }

    DEV_PRINTK("chnint_poll dev %p, chn_cid %d: poll check intrState=%d", dev, dev->chn_cid, *pState);
    DEV_PRINTK("filp=%p", filp);

    poll_wait(filp, pWq, wait);
    if (*pState != CHN_INTR_STATE_NONE)
    {
        DEV_PRINTK("chnint_poll dev %p, chn_cid %d waked up", dev, dev->chn_cid);
        mask |= POLLIN | POLLRDNORM; /* readable */
    }
    DEV_PRINTK("chnint_poll dev %p, chn_cid %d: mask=0x%08x", dev, dev->chn_cid, mask);
    return mask;
}

static long chnint_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    chnint_t *dev = filp->private_data; /* dev was stored in filp during open */

    hw_mbox_Masters_e   peer;
    HW_MBOX_STATUS_e    status;
    int ret;

    status = HW_MBOX_STATUS_OK;
    peer = HW_MBOX_MID_INVALID;
    ret = 0;

    switch (cmd)
    {
        case HW_MBOX_CHN_DEV_IOCTL_LOCK_CMD:
        {
            DPRINTK("[CHN_DEV_CID%d]: Received HW_MBOX_CHN_DEV_IOCTL_LOCK_CMD",
                    dev->chn_cid);

            if ( (CHN_CFG_GET_COMMANDER(dev->chn_cid) != HW_MBOX_MID) &&
                 (CHN_CFG_GET_SUBJECT(dev->chn_cid) != HW_MBOX_MID) )
            {
                PRINTK_ERR("[CHN_DEV_CID%d]: current master %s(%d) is neither commander nor subject of the channel!!!",
                           dev->chn_cid, HW_MBOX_MASTER_GET_NAME(HW_MBOX_MID), HW_MBOX_MID);

                status = HW_MBOX_STATUS_NOK;
                ret = -1;
                break;
            }

            if (CHN_CFG_GET_COMMANDER(dev->chn_cid) == HW_MBOX_MID)
            {
                peer = CHN_CFG_GET_SUBJECT(dev->chn_cid);
            }
            else if (CHN_CFG_GET_SUBJECT(dev->chn_cid) == HW_MBOX_MID)
            {
                peer = CHN_CFG_GET_COMMANDER(dev->chn_cid);
            }

            if (!HW_MBOX_MID_VALIDITY_CHECK(peer)) 
            {
                PRINTK_ERR("[CHN_DEV_CID%d]: invalid peer mid value in DB! [peer=%d]",
                           dev->chn_cid, peer);

                status = HW_MBOX_STATUS_NOK;
                ret = -1;
                break;
            }

            if (!IS_MASTER_INIT(peer))
            {
                DPRINTK("[CHN_DEV_CID%d]: peer %s(%d) is not initialized!",
                        dev->chn_cid, HW_MBOX_MASTER_GET_NAME(peer), peer);
                status = HW_MBOX_STATUS_PEER_NOT_INITIALIZED;
                break;
            }

            /* seize channel lock */
            if (down_interruptible(&CHN_LOCK(dev->chn_cid)))
            {
                DPRINTK("down interrupted by signal...");
                ret = -ERESTARTSYS;
                break;
            }

            break;
        }
        case HW_MBOX_CHN_DEV_IOCTL_UNLOCK_CMD:
        {
            DPRINTK("[CHN_DEV_CID%d]: Received HW_MBOX_CHN_DEV_IOCTL_UNLOCK_CMD",
                    dev->chn_cid);

            /* No need to check for Master initialized because the
               channel was already successfully locked to get here... */

            /* release Lock... */
            up(&CHN_LOCK(dev->chn_cid));
            break;
        }
        default:
        {
            PRINTK_ERR("[CHN_DEV_CID%d]: invalid IOCTL(0x%08x) has been received!",
                       dev->chn_cid, cmd);
            ret = -1;
            break;
        }
    }

    if (copy_to_user((void __user *)arg, &status, sizeof(HW_MBOX_STATUS_e)))
    {
        PRINTK_ERR("Failed to copy to user!!!");
        ret = -EFAULT;
    }

    return ret;
}

static int chnint_release(struct inode *inode, struct file *filp)
{
#ifdef HW_MBOX_KDBG
    chnint_t *dev;
    dev = container_of(inode->i_cdev, chnint_t, chnint_cdev);

    DEV_PRINTK("chnint_release: dev %p, chn_cid %d", dev, dev->chn_cid);
#endif /* HW_MBOX_KDBG */
    return 0;
}

/*! \fn static void chnint_cleanup(int count)
    \brief use to clean up the Channel Interrupt devices.
    \param[in] count - amount of devices to clean up.
    \note also clears the chrdev region.
*/
static void chnint_cleanup(int count)
{
    int i;
    DPRINTK("cleanup function, count=%d", count);
    if (chnint_devices)
    {
        for (i = 0; i < count; i++)
        {
            cdev_del(&chnint_devices[i].chnint_cdev);
        }
        kfree(chnint_devices);
    }
    unregister_chrdev_region(chnint_dev, HW_MBOX_CHANNEL_COUNT);
}

/************************************************************************/
/*     HW_MBOX driver functions                                         */
/************************************************************************/

/**************************************************************************/
/*! \fn irqreturn_t hwMbox_drv_isr(int irq, void *dev_id)
 **************************************************************************
 *  \brief HW_MBOX driver ISR.
 *  \return [ IRQ_HANDLED ].
 **************************************************************************/
static irqreturn_t hwMbox_drv_isr(int irq, void *dev_id)
{
    Uint32 highIntrStatusReg; /* Interrupts from Channels 63-32 [for supervisor 63 is error notify] */
    Uint32 lowIntrStatusReg;  /* Interrupts from Channels 31-0 */

#ifdef APP_CPU_HW_MBOX_IRQ_SAMPLE_CLK_WA
    Uint32 highIntrStatusRegBuffer; /* Buffer for Interrupts from Channels 63-32 [for supervisor 63 is error notify] */
    Uint32 lowIntrStatusRegBuffer;  /* Buffer for Interrupts from Channels 31-0 */
#endif /* APP_CPU_HW_MBOX_IRQ_SAMPLE_CLK_WA */

    Uint32 highIntrMaskReg; /* Interrupt Mask for Channels 63-32 */
    Uint32 lowIntrMaskReg;  /* Interrupt Mask for Channels 31-0  */

    Uint32 highCidRange, regVal, cfgRegVal, chnint_cid;

#ifdef APP_CPU_HW_MBOX_IRQ_SAMPLE_CLK_WA
    highIntrStatusRegBuffer = 0;
    lowIntrStatusRegBuffer = 0;
#endif /* APP_CPU_HW_MBOX_IRQ_SAMPLE_CLK_WA */

    /* We got a HW MBOX interrupt! */

    /* Now we must read all the set bits from both HW_MBOX_MASTER_INTR_STATUS_H and
       HW_MBOX_MASTER_INTR_STATUS_L. All interrupts must be handled as the bits are
       reset upon read! [except masked ones...] */

#ifdef APP_CPU_HW_MBOX_IRQ_SAMPLE_CLK_WA
READ_INTERRUPT_STATUS_REGS:
#endif
    highIntrStatusReg = HW_MBOX_MASTER_INTR_STATUS_H_REG_GET();
    lowIntrStatusReg = HW_MBOX_MASTER_INTR_STATUS_L_REG_GET();

    DPRINTK("intrStatusRegs(H L): 0x%08x 0x%08x", highIntrStatusReg, lowIntrStatusReg);

    DPRINTK("channel interrupt bits should now be cleared [all unmasked ones]...");
    DPRINTK("verifying using shadowRegs(H L): 0x%08x 0x%08x", HW_MBOX_MASTER_INTR_CLEAR_H_REG_GET(),
                                                              HW_MBOX_MASTER_INTR_CLEAR_L_REG_GET());

    /* Now we should remove the masked bits [interrupt is disabled for them]: */

    highIntrMaskReg = HW_MBOX_MASTER_INTR_MASK_H_REG_GET();
    lowIntrMaskReg  = HW_MBOX_MASTER_INTR_MASK_L_REG_GET();
    DPRINTK("intrMaskRegs(H L): 0x%08x 0x%08x", highIntrMaskReg, lowIntrMaskReg);

    highIntrStatusReg &= (~highIntrMaskReg);
    lowIntrStatusReg  &= (~lowIntrMaskReg);
    DPRINTK("intrStatusRegs(H L) [after removed masked CIDs]: 0x%08x 0x%08x", highIntrStatusReg, lowIntrStatusReg);

#ifdef APP_CPU_HW_MBOX_IRQ_SAMPLE_CLK_WA
    /* save the current values to the buffers, read again, see if we get new bits... */
    if ((0 != highIntrStatusReg) || (0 != lowIntrStatusReg))
    {
        highIntrStatusRegBuffer |= highIntrStatusReg;
        lowIntrStatusRegBuffer |= lowIntrStatusReg;
        DPRINTK("APP-CPU: read again interrupt status regs values...");
        goto READ_INTERRUPT_STATUS_REGS;
    }

    /* no more new bits, save back to values from the buffers the regular variables */
    DPRINTK("APP-CPU: no more new bits, continuing...");
    highIntrStatusReg = highIntrStatusRegBuffer;
    lowIntrStatusReg = lowIntrStatusRegBuffer;
    DPRINTK("APP-CPU: intrStatusRegs(H L) [final values]: 0x%08x 0x%08x", highIntrStatusReg, lowIntrStatusReg);
#endif /* APP_CPU_HW_MBOX_IRQ_SAMPLE_CLK_WA */

    if ((0 == highIntrStatusReg) && (0 == lowIntrStatusReg))
    {
        /* we got an interrupt, but no set bits in interrupt status! */
        DPRINTK("We got a ghost Interrupt!");
        INC_GHOST_INTR();
    }
    else
    {
        /* First we will check if error is set, so we can notify as quickly as possible */
        if ((IS_SUPERVISOR()) && (HW_MBOX_IS_ERROR_BIT_SET_FOR_SUPERVISOR(highIntrStatusReg)))
        {
            DPRINTK("Hardware signals of Access Violation Error!");
            INC_ERR_INTR();

            DPRINTK("interrupt on channel %d, signaling queue...", HW_MBOX_ERROR_NOTIFY_CID_FOR_SUPERVISOR);

            CHN_INTR_SET_STATE(HW_MBOX_ERROR_NOTIFY_CID_FOR_SUPERVISOR, CHN_INTR_STATE_PENDING);
            wake_up_interruptible(&CHN_INTR_WQ(HW_MBOX_ERROR_NOTIFY_CID_FOR_SUPERVISOR));

            /* Clear the error interrupt from the register value buffer: */
            highIntrStatusReg &= (~HW_MBOX_ERROR_BIT_FOR_SUPERVISOR);
        }

        for (highCidRange = 0; highCidRange < 2; highCidRange++)
        {
            /* highCidRange determines what CID range we are querying:
               highCidRange = 0 -> CID range [31-0]
               highCidRange = 1 -> CID range [63-32] */

            DPRINTK("Handling Interrupts in %s CID range", (highCidRange)?("high [63-32]"):("low [31-0]"));

            regVal = (highCidRange)?(highIntrStatusReg):(lowIntrStatusReg);

            while (regVal)
            {
                chnint_cid = fls(regVal) - 1; /* fls gets (position of LSB that is set) + 1 */
                DPRINTK("position of LSB that is set on regVal=0x%08x is %d", regVal, chnint_cid);
                regVal &= (~BIT(chnint_cid));       /* turn off bit, we got it... */
                chnint_cid += (32 * highCidRange);  /* adjusting to range */
                CHN_INTR_INC_COUNT(chnint_cid);

                if ( !IS_INITIALIZED() && !IS_SUPERVISOR() )
                {
                    /* non-supervisor getting a HW MBOX interrupt at Initialization stage.
                       It should look if this channel is a Receive Opcode channel from the
                       supervisor. */

                    DPRINTK("non-supervisor and HW MBOX not initialized yet...");

                    cfgRegVal = HW_MBOX_CHN_CFG_REG_GET(chnint_cid);

                    if ( (HW_MBOX_CHN_CFG_COMMANDER_GET_FROM_REGVAL(cfgRegVal) == CTRL_GET_SUPERVISOR()) &&
                         (HW_MBOX_CHN_CFG_TYPE_GET_FROM_REGVAL(cfgRegVal) == hw_mbox_opcode) )
                    {
                        DPRINTK("cid %d is the receive opcode channel used for initial configuration! signal INIT_COMPLETE WQ...",
                                chnint_cid);

                        /* Signal the first listener!
                           just signal the listener, it'll do the rest...*/

                        RECV_OP_THREAD_SET_CID(RECV_OPCODE_INIT_INDEX, chnint_cid);
                        RECV_OP_THREAD_SET_INDEX(RECV_OPCODE_INIT_INDEX);

                        SET_INIT_COMPLETE();
                    }

                    /* continue normal flow, either way we should signal the channel queue...
                       To release the first listener to continue init, or to save the channel interrupt for later use...
                    */
                }

                if (CHN_IS_INIT(chnint_cid))
                {
                    /* Special flag on channel that indicates that it's part of
                       HW MBOX intialization process. This is always an Opcode channel
                       with supervisor as commander and another master as subject.

                       First message on this channel acts as ACK from subject that it is
                       initialized.
                    */
                    DPRINTK("interrupt on INIT channel %d from Master %d",
                            chnint_cid, CHN_CFG_GET_SUBJECT(chnint_cid));

                    MASTER_SET_INITIALIZED(CHN_CFG_GET_SUBJECT(chnint_cid), True);
                    CHN_SET_INIT(chnint_cid, False); /* This channel is no longer used for INIT... */
                }
                else
                {
                    DPRINTK("interrupt on channel %d, signaling queue...", chnint_cid);
                    CHN_INTR_SET_STATE(chnint_cid, CHN_INTR_STATE_PENDING);
                    wake_up_interruptible(&CHN_INTR_WQ(chnint_cid));
                }
                DPRINTK("new regVal=0x%08x (after handling one channel interrupt)", regVal);
            }
        }
    }

#ifdef HW_MBOX_IS_NP_CPU
    /* Use ack_irq:
       Signal that we handled the Interrupt to clear the bit in the interrupt controller.
       Otherwise ISR would get called again as the bit is not cleared and it is Level.

       Code preceding the ISR will clear the bit in the Interrupt Controller as the INTR Line
       is not on (but the ISR runs again anyway when it shouldn't).

       If the INTR Line is back up (after we read INTR_STATUS but before calling ack_irq) then
       upon exit from ISR the bit in the controller will get turned back on, so we can't miss
       interrupts by using ack_irq but we will save the unnecessary ISR just to clear the bit
       in the interrupt controller.
    */
    ack_irq(irq);
#endif /* HW_MBOX_IS_NP_CPU */
    return IRQ_HANDLED;
}

#ifdef HW_MBOX_IS_APP_CPU
/**************************************************************************/
/*! \fn int hwMbox_drv_registerWithNetss(void)
 **************************************************************************
 *  \brief Register the HW_MBOX driver with Net Subsystem.
 *  \return [ 0 on success / error otherwise ].
 **************************************************************************/
int hwMbox_drv_registerWithNetss(void)
{
    int ret = -ENODEV;
    netss_dev_info_t hwMbox_mmio;

    DPRINTK("About to register driver with Net Subsystem...");

    DPRINTK("Checking netss driver ready...");
    if(!netss_driver_ready())
    {
        PRINTK_ERR("Net Subsystem Driver is not ready!");
        return ret;
    }

    DPRINTK("Getting MMIO info...");
    if(netss_device_get_info(NETSS_DEV_HW_MAILBOX, &hwMbox_mmio))
    {
        PRINTK_ERR("Failed to get mmio info!");
        return -1;
    }
    else
    {
        PRINTK_INFO("HW_MBOX MMIO: mem_iobase = 0x%x, mem_iosize = 0x%x",
                    (unsigned int)hwMbox_mmio.base, (unsigned int)hwMbox_mmio.size);
    }

    DPRINTK("ioremap registers...");
    hwMboxDriverData.reg_base = (void __iomem *)ioremap_nocache(hwMbox_mmio.base, hwMbox_mmio.size);
    if (!hwMboxDriverData.reg_base)
    {
        PRINTK_ERR("Failed to ioremap registers!");
        ret = -ENOMEM;
        return ret;
    }

#ifdef HW_MBOX_EXTENDED_MEMORY
    DPRINTK("Getting extended memory MMIO info...");
    if(netss_device_get_info(NETSS_DEV_PERIPHERAL_SRAM, &hwMbox_mmio))
    {
        PRINTK_ERR("Failed to get extended memory mmio info!");
        return -1;
    }
    else
    {
        PRINTK_INFO("EXT_MEM MMIO: mem_iobase = 0x%x, mem_iosize = 0x%x",
                    (unsigned int)hwMbox_mmio.base, (unsigned int)hwMbox_mmio.size);
    }

    DPRINTK("ioremap extended memory...");
    hwMboxDriverData.extMem_base = (void __iomem *)ioremap_nocache(hwMbox_mmio.base, hwMbox_mmio.size);
    if (!hwMboxDriverData.extMem_base)
    {
        PRINTK_ERR("Failed to ioremap extended memory!");
        ret = -ENOMEM;
        return ret;
    }
#endif /* HW_MBOX_EXTENDED_MEMORY */
    DPRINTK("Registered driver with Net Subsystem successfully!");
    return 0;
}
#endif /* HW_MBOX_IS_APP_CPU */

/**************************************************************************/
/*! \fn int __init hwMbox_drv_init(void)
 **************************************************************************
 *  \brief HW_MBOX driver Initialization function.
 *  \return [ 0 on success / error otherwise ].
 **************************************************************************/
static int __init hwMbox_drv_init(void)
{
    int i, r, devno, paramMID;
    Uint32 paramCID;

    hw_mbox_channelConfig_t chnConfig;

    Int32 retVal;
    Uint32 cmdRegVal, regVal;

    hwMbox_opcode_t opFlds = { 0 };
    hwMbox_opcodeMessage_t opcodeMsg;

    PRINTK_INFO("Initializing %s version %s", DRV_NAME, DRV_VERSION);

#ifdef HW_MBOX_DEBUG
    /* Verify initial disguise: */
    PRINTK_INFO("Master Disguise Feature: initial MID (%d) is hardcoded MID (%d)",
                HW_MBOX_MID, HW_MBOX_HARDCODED_MID);
#endif /* HW_MBOX_DEBUG */

    /* Initialize the Driver Local Data: */
    RESET_DRIVER_DATA();

    /* and other local variables: */
    RESET_CID_LUT();
    RESET_RECV_OP_THREAD_DATA();
    RESET_RECV_OP_DATA();
    RESET_STATS();

#ifdef HW_MBOX_IS_APP_CPU
    if(0 > hwMbox_drv_registerWithNetss())
    {
        PRINTK_ERR("Failed to register driver with Net Subsystem!!!");
        return -ENODEV;
    }
#endif /* HW_MBOX_IS_APP_CPU */

    /* Create Channel interrupt Devices: */
    if ((r = alloc_chrdev_region(&chnint_dev, 0, HW_MBOX_CHANNEL_COUNT, HW_MBOX_CHNINT_DEV_NAME)) != 0)
    {
        PRINTK_ERR("Failed to register character device region %s", HW_MBOX_CHNINT_DEV_NAME);
        return r;
    }

    chnint_class = class_create(THIS_MODULE, HW_MBOX_CHNINT_DEV_NAME);
    DPRINTK("registered with major number %i", MAJOR(chnint_dev));

    if ((chnint_devices = kmalloc(HW_MBOX_CHANNEL_COUNT * sizeof(chnint_t), GFP_KERNEL)) == NULL)
    {
        PRINTK_ERR("Failed to allocate %d devices", HW_MBOX_CHANNEL_COUNT);
        chnint_cleanup(0);
        return -ENOMEM;
    }

    DPRINTK("Successfully allocated %d devices!", HW_MBOX_CHANNEL_COUNT);

    RESET_CHN_DEVS();
    RESET_CHN_INTR_DATA();
    RESET_CHN_LOCKS();

    for (i = 0; i < HW_MBOX_CHANNEL_COUNT; i++)
    {
        /* Initialize the channel's infrastructure [channel interrupt and lock]: */
        /* note: this should be done prior to ISR request to ensure that there's a channel interrupt to catch the signal. */
        init_waitqueue_head(&CHN_INTR_WQ(i));
        CHN_INTR_SET_STATE(i, CHN_INTR_STATE_NONE);
        sema_init(&CHN_LOCK(i), 1);
        DPRINTK("initialized infrastructure for cid %d [WQ %p, LOCK %p]", i, &CHN_INTR_WQ(i), &CHN_LOCK(i));

        /* Initialize the channel device: */
        chnint_devices[i].chn_cid = i;

        /* Connect the file operations with the cdev */
        cdev_init(&chnint_devices[i].chnint_cdev, &chnint_fops);

        devno = MKDEV(MAJOR(chnint_dev), MINOR(chnint_dev) + i);
        if ((r = cdev_add(&chnint_devices[i].chnint_cdev, devno, 1)) != 0)
        {
            PRINTK_ERR("Failed to add %s%02d with error %d", HW_MBOX_CHNINT_DEV_NAME, i, r);
            chnint_cleanup(i);
            return (-1);
        }
        else
        {
            DPRINTK("Successfully added %s%02d", HW_MBOX_CHNINT_DEV_NAME, i);
        }

        device_create(chnint_class, 0, MKDEV(MAJOR(chnint_dev), MINOR(chnint_dev) + i), 0, "%s/%s%02d", HW_MBOX_DRV_DEV_DIR_NAME, HW_MBOX_CHNINT_DEV_NAME, i);
        DPRINTK("Created device /dev/%s/%s%02d", HW_MBOX_DRV_DEV_DIR_NAME, HW_MBOX_CHNINT_DEV_NAME, i);
    }

    DPRINTK("Created %d /dev/%s/%sXX devices", HW_MBOX_CHANNEL_COUNT, HW_MBOX_DRV_DEV_DIR_NAME, HW_MBOX_CHNINT_DEV_NAME);

#ifdef HW_MBOX_SOCKETS_API
    /* Initialize HW_MBOX sockets */
    retVal = proto_register(&hwMbox_proto, 0);
    if (retVal != 0)
    {
        PRINTK_ERR("Couldn't initialize hwMbox sockets!");
        return retVal;
    }

    sock_register(&hwMbox_family_ops);
    DPRINTK("AF_HWMBOX socket family registered");

    /* Initialize semaphore for sockstat */
    sema_init(&hwMbox_sock_hlist_sem, 1);
#endif /* HW_MBOX_SOCKETS_API */

    /* Create the Proc filesystem utilities ( /proc/<HW_MBOX_PROC_DIR_STR>/ ): */
    {
        struct proc_dir_entry *procDir;

        /* Directory: */
        PROC_CREATE_DIR(HW_MBOX_PROC_DIR_STR, procDir);

        /* Simple Procs: */
        SEQ_PROC_CREATE("core_version", proc_dump_core_version, procDir);
        SEQ_PROC_CREATE("control_regs", proc_dump_control_regs, procDir);
        SEQ_PROC_CREATE("status", proc_dump_status, procDir);
        SEQ_PROC_CREATE("ctrlDb", proc_dump_ctrlDb, procDir);

        /* SEQ Procs: */
        SEQ_PROC_CREATE("masters_regs", proc_dump_masters_regs, procDir);
        SEQ_PROC_CREATE("channels_regs", proc_dump_channels_regs, procDir);
        SEQ_PROC_CREATE("assignedChannels_regs", proc_dump_assignedChannels_regs, procDir);
        SEQ_PROC_CREATE("interrupts", proc_dump_interrupts, procDir);
        SEQ_PROC_CREATE("LUT", proc_dump_LUT, procDir);
        SEQ_PROC_CREATE("counters", proc_dump_serviceCounter, procDir);
        SEQ_PROC_CREATE("masterDb", proc_dump_masterDb, procDir);
        SEQ_PROC_CREATE("chnDb", proc_dump_chnDb, procDir);
        SEQ_PROC_CREATE("recvOpDb", proc_dump_recvOpDb, procDir);
        SEQ_PROC_CREATE("tagsDb", proc_dump_recvOpTagsDb, procDir);

#ifdef HW_MBOX_SOCKETS_API
        /* AF_HWMBOX sockets Procs: */
        SEQ_PROC_CREATE("sockstat", proc_dump_sockstat, procDir);
#endif /* HW_MBOX_SOCKETS_API */
    }
    DPRINTK("Created proc directory: /proc/%s", HW_MBOX_PROC_DIR_STR);

    /* Get core version from HW_MBOX_ID register: */
    CTRL_SET_CORE_VER_MAJ(HW_MBOX_ID_MAJOR_GET());
    CTRL_SET_CORE_VER_MIN(HW_MBOX_ID_MINOR_GET());

    DPRINTK("HW_MBOX core version: %d.%d", CTRL_GET_CORE_VER_MAJ(), CTRL_GET_CORE_VER_MIN());

#ifdef __HW_MBOX_CHANGE_DEFAULT_SUPERVISOR_TO
    /* Check if we need to change default supervisor: */
    /* This needs to be done now since our next step is finding out whether we are the supervisor */
    if (HW_MBOX_CTRL_SUPERVISOR_GET() != __HW_MBOX_CHANGE_DEFAULT_SUPERVISOR_TO)
    {
        PRINTK_INFO("changing supervisor to: %s(%d)",
                    HW_MBOX_MASTER_GET_NAME(__HW_MBOX_CHANGE_DEFAULT_SUPERVISOR_TO),
                    __HW_MBOX_CHANGE_DEFAULT_SUPERVISOR_TO);

        HW_MBOX_CTRL_AS_DEFAULT_SUPERVISOR_SUPERVISOR_SET(__HW_MBOX_CHANGE_DEFAULT_SUPERVISOR_TO);
    }
#endif /* __HW_MBOX_CHANGE_DEFAULT_SUPERVISOR_TO */

    /* Set Supervisor Value in DB: */
    CTRL_SET_SUPERVISOR(HW_MBOX_CTRL_SUPERVISOR_GET());
    DPRINTK("supervisor is: %s(%d)", HW_MBOX_MASTER_GET_NAME(CTRL_GET_SUPERVISOR()),
                                     CTRL_GET_SUPERVISOR());

    /* Check which Masters are Valid: */
    for (paramMID = 0; paramMID < HW_MBOX_MASTER_COUNT; paramMID++)
    {
        memcpy(&MASTER_CFG(paramMID), &HW_MBOX_MASTER_GET_CONFIG(paramMID), HW_MBOX_MASTER_CONFIG_SIZE);

        /* TBD: add check for fused out Masters */

        if (MASTER_CFG_GET_SUPPORT(paramMID) != hw_mbox_support_none)
        {
            MASTER_SET_VALID(paramMID, True);
        }
        else
        {
            MASTER_SET_VALID(paramMID, False);
        }

        MASTER_SET_INITIALIZED(paramMID, False);
    }

    if (IS_SUPERVISOR())
    {
        DPRINTK("Current Master is the supervisor!");
        DPRINTK("Setting up general control configuration values...");

#ifdef HW_MBOX_MONITOR
        DPRINTK("Setting Monitor = %s(%d)", HW_MBOX_MASTER_GET_NAME(HW_MBOX_MONITOR), HW_MBOX_MONITOR);

        /* Setup HW_MBOX Configuration registers: */
        HW_MBOX_CTRL_MON_EXIST_SET(True);
        HW_MBOX_CTRL_MONITOR_SET(HW_MBOX_MONITOR);

        /* Set Monitor Value in DB: */
        CTRL_SET_MONITOR(HW_MBOX_MONITOR);
#else /* HW_MBOX_MONITOR */
        /* Setup HW_MBOX Configuration registers: */
        HW_MBOX_CTRL_MON_EXIST_SET(False); /* Default should be False, but it doesn't hurt to make sure */

        /* Set Monitor Value in DB: */
        CTRL_SET_MONITOR(HW_MBOX_MID_INVALID);
#endif /* ! HW_MBOX_MONITOR */

#ifdef HW_MBOX_START_IN_HW_DEBUG_MODE
        DPRINTK("Setting HW Debug Mode...");

        /* Setup HW_MBOX Configuration registers: */
        HW_MBOX_CTRL_DEBUG_SET(True);
        CTRL_SET_IS_HW_DEBUG_MODE(True);
#else /* HW_MBOX_START_IN_HW_DEBUG_MODE */
        HW_MBOX_CTRL_DEBUG_SET(False); /* Default should be False, but it doesn't hurt to make sure */
        CTRL_SET_IS_HW_DEBUG_MODE(False);
#endif /* ! HW_MBOX_START_IN_HW_DEBUG_MODE */

        /* INIT static channels: */
        DPRINTK("Initializing %d static channels", HW_MBOX_STATIC_CHANNEL_COUNT);
        for (i = 0; i < HW_MBOX_STATIC_CHANNEL_COUNT; i++)
        {
            memcpy(&chnConfig, &HW_MBOX_STATIC_CHANNEL_GET_CONFIG(i), HW_MBOX_CHANNEL_CONFIG_SIZE);
            paramCID = chnConfig.cid;

            DPRINTK("static channel entry %d (cid=%d): mboxType=%d, commander=%d, subject=%d, memSize=%uB",
                    i, paramCID, chnConfig.mboxType, chnConfig.commander, chnConfig.subject, chnConfig.memBuff.size);

            /* First we should check if the configuration is valid: */
            DPRINTK("static channel entry %d (cid=%d): verifying configuration...", i, paramCID);

            /* check that both Masters are Valid */
            if ((!IS_MASTER_VALID(chnConfig.commander)) ||
                (!IS_MASTER_VALID(chnConfig.subject)))
            {
                PRINTK_INFO("static channel entry %d (cid=%d): Invalid configuration! One of the Masters is invalid!",
                            i, paramCID);

                continue; /* invalid configuration skip this channel */
            }

            /* check that CID is not yet allocated: */
            if (HW_MBOX_CHN_CFG_VALID_GET(paramCID))
            {
                /* CHN_CFG is valid... This CID has already been assigned! */
                PRINTK_INFO("static channel entry %d (cid=%d): Invalid configuration! CID already assigned!",
                            i, paramCID);

                continue; /* invalid configuration skip this channel */
            }

            if (paramCID == HW_MBOX_ERROR_NOTIFY_CID_FOR_SUPERVISOR)
            {
                /* We can't allocate CID that's used for error notification with supervisor as Commmander/Subject  */
                if ((chnConfig.commander == HW_MBOX_MID) || (chnConfig.subject == HW_MBOX_MID))
                {
                    PRINTK_INFO("static channel entry %d (cid=%d): Invalid configuration! Notify Error CID %d can't be allocated with supervisor as Commander/Subject!",
                                i, paramCID, HW_MBOX_ERROR_NOTIFY_CID_FOR_SUPERVISOR);

                    continue; /* invalid configuration skip this channel */
                }

#ifdef HW_MBOX_MONITOR
                /* If Supervisor is also Monitor we can't allocate CID that's used for error notification at all! */
                if (IS_MONITOR())
                {
                    PRINTK_INFO("static channel entry %d (cid=%d): Invalid configuration! Notify Error CID %d can't be allocated at ALL if supervisor is also Monitor!",
                                i, paramCID, HW_MBOX_ERROR_NOTIFY_CID_FOR_SUPERVISOR);

                    continue; /* invalid configuration skip this channel */
                }
#endif /* HW_MBOX_MONITOR */
            }

            if ((chnConfig.memBuff.size % HW_MBOX_SRAM_CHUNK) != 0)
            {
                PRINTK_INFO("static channel entry %d (cid=%d): Invalid configuration! SRAM size must be an integral Multiple of %dB (requested Size = %uB)",
                            i, paramCID, HW_MBOX_SRAM_CHUNK, chnConfig.memBuff.size);

                continue; /* invalid configuration skip this channel */
            }

            if (chnConfig.mboxType == hw_mbox_event)
            {
                if (chnConfig.memBuff.size == 0)
                {
                    PRINTK_INFO("static channel entry %d (cid=%d): Invalid configuration! for event channels SRAM size must be > 0 [for event Buffer] (requested Size = %uB)",
                                i, paramCID, chnConfig.memBuff.size);

                    continue; /* invalid configuration skip this channel */
                }
                if ((chnConfig.memBuff.size / HW_MBOX_SRAM_CHUNK) % 2 != 0)
                {
                    PRINTK_INFO("static channel entry %d (cid=%d): Invalid configuration! for event channels SRAM size must be an evenly divisible amount of %dB chunks (requested Size = %uB)",
                                i, paramCID, HW_MBOX_SRAM_CHUNK, chnConfig.memBuff.size);

                    continue; /* invalid configuration skip this channel */
                }
            }
            else if (chnConfig.mboxType == hw_mbox_opcode)
            {
                if (chnConfig.memBuff.size < HW_MBOX_OPCODE_MINIMAL_SRAM_ALLOCATION_IN_BYTES)
                {
                    PRINTK_INFO("static channel entry %d (cid=%d): Invalid configuration! for opcode channels SRAM size must be >= %dB (requested Size = %uB)",
                                i, paramCID, HW_MBOX_OPCODE_MINIMAL_SRAM_ALLOCATION_IN_BYTES, chnConfig.memBuff.size);

                    continue; /* invalid configuration skip this channel */
                }
            }

            if (chnConfig.memBuff.size > GET_FREE_SRAM_LEFT())
            {
                PRINTK_ERR("static channel entry %d (cid=%d): Cannot be configured! Not enough SRAM memory Left (requested Size = %uB, free SRAM left = %uB)!",
                           i, paramCID, chnConfig.memBuff.size, GET_FREE_SRAM_LEFT());

                continue; /* error, not more memory left! skip this channel */
            }

            /* If we are here the channel configuration is OK we can proceed in setting up the HW_MBOX channel
               registers with its configuration. */
            HW_MBOX_CHANNEL_CONFIG_SET(paramCID, chnConfig.mboxType, 0 /* reqId is not used yet */, chnConfig.commander, chnConfig.subject);

            if (chnConfig.memBuff.size > 0)
            {
                DPRINTK("static channel entry %d (cid=%d): current Nonallocated SRAM Offset=0x%08x, need %uB of SRAM",
                        i, paramCID, GET_NEXT_ALLOCATION_SRAM_OFFSET(), chnConfig.memBuff.size);

                chnConfig.memBuff.start = GET_NEXT_ALLOCATION_SRAM_OFFSET();

                ALLOCATE_SRAM(paramCID, chnConfig.memBuff.size);

                DPRINTK("static channel entry %d (cid=%d): %uB of SRAM allocated, new current Nonallocated SRAM Offset=0x%08x",
                        i, paramCID, chnConfig.memBuff.size, GET_NEXT_ALLOCATION_SRAM_OFFSET());

                /* We would have liked to reset channel's SRAM if we are Commander,
                   but it is impossible to do so at this point since START is not set...
                   it will be done later in this function... */
            }

            HW_MBOX_CHN_CFG_VALID_SET(paramCID, True);

            hwMbox_setChannelConfig(chnConfig);

            if (!CHN_IS_RECV_OP(paramCID))
            {
                hwMbox_setChannelValid(paramCID, True); /* also updates LUTs */

                /* for recv Op channel valid is set by listener... */
            }

            DPRINTK("configured new static channel (cid=%d): %s MBOX, %s(%d) -> %s(%d) [%uB of SRAM]",
                    paramCID, HW_MBOX_TYPE_STR(chnConfig.mboxType),
                    HW_MBOX_MASTER_GET_NAME(chnConfig.commander), chnConfig.commander,
                    HW_MBOX_MASTER_GET_NAME(chnConfig.subject), chnConfig.subject,
                    chnConfig.memBuff.size);
        }
    }
    else
    {
        /* non-supervisor and HW MBOX is not yet started... */
        DPRINTK("continue configuration upon first interrupt from supervisor...");

        /* Create the first receive opcode listener, it'll wait for the
           first interrupt from supervisor (INIT_COMPLETE) */

        RECV_OP_THREAD_BY_INDEX(RECV_OPCODE_INIT_INDEX) = kthread_run(hwMbox_recvOpcodeListenerEntry,
                                                                      &RECV_OP_THREAD_DATA(RECV_OPCODE_INIT_INDEX),
                                                                      RECV_OPCODE_LISTENER_PREFIX"%02d",
                                                                      RECV_OPCODE_INIT_INDEX);

        if (IS_ERR(RECV_OP_THREAD_BY_INDEX(RECV_OPCODE_INIT_INDEX)))
        {
            retVal = PTR_ERR(RECV_OP_THREAD_BY_INDEX(RECV_OPCODE_INIT_INDEX));
            PRINTK_ERR("Creating %s%02d used for initialization Failed!!! [error %d]",
                       RECV_OPCODE_LISTENER_PREFIX, RECV_OPCODE_INIT_INDEX, retVal);

            PRINTK_ERR("*** CRITICAL ERROR in HW MBOX initialization, no HW MBOX services will be avaiable ***");

            RECV_OP_THREAD_BY_INDEX(RECV_OPCODE_INIT_INDEX) = NULL;

            /* HW MBOX initialization cannot continue... */
            return (-1);
        }
    }

    /* Setup ISR: */
#if defined HW_MBOX_IS_NP_CPU
    if (request_irq(HW_MBOX_INT_LINE, hwMbox_drv_isr, IRQF_TRIGGER_HIGH, HW_MBOX_INT_NAME, NULL /* no need for dev as line is not shared */))
    {
        PRINTK_ERR("Unable to allocate IRQ!");
        return -ENODEV;
    }
#elif defined HW_MBOX_IS_APP_CPU
    {
       netss_interrupt_info_t irq_info;
       irq_info.func = &hwMbox_drv_isr;
       irq_info.args = &hwMboxDriverData;
       netss_interrupt_register(NETSS_INTERUPT_HWMBOX, -1, &irq_info);
    }
#endif

    if (IS_SUPERVISOR())
    {
        /* Now that all configuration values and static channels are set,
           and Supervisor Driver is ready after ISR setup we can set START! */
        PRINTK_INFO("initial configuration completed, setting START!");
        HW_MBOX_CTRL_START_SET(True);

        CTRL_SET_IS_STARTED(True);

        /* Now that everything is ready send interrupt to all valid Masters: */
        DPRINTK("send INIT_COMPLETE command to valid masters...");

        /* Prepare the INIT_COMPLETE message: */
        opcodeMsg.msgType = HW_MBOX_OPCODE_MESSAGE_TYPE_CODE;
        opcodeMsg.msgData.msgCode.code = HW_MBOX_CODE_INIT_COMPLETE;

        /* Build the opcode: */
        opFlds.tag = HW_MBOX_OPCODE_TAG;
        opFlds.lenInBytes = HW_MBOX_OPCODE_MESSAGE_SIZE;

        /* Create the opcode value: */
        HW_MBOX_OPCODE_FIELDS_SET_FROM_STRUCT(cmdRegVal, &opFlds);

        DPRINTK("opcode fields - tag=%u, lenInBytes=%u -> cmdRegVal=0x%08x",
                opFlds.tag, opFlds.lenInBytes, cmdRegVal);

        for (paramMID = 0; paramMID < HW_MBOX_MASTER_COUNT; paramMID++)
        {
            if (IS_MASTER_VALID(paramMID))
            {
                if (paramMID == HW_MBOX_MID)
                {
                    MASTER_SET_INITIALIZED(paramMID, True);

                    /* no need to send to ourself */
                    continue;
                }

                if (MASTER_CFG_GET_SUPPORT(paramMID) == hw_mbox_support_static)
                {
                    MASTER_SET_INITIALIZED(paramMID, True);

                    /* no need to send to Masters with static SW support */
                    continue;
                }

                paramCID = FIND_CID_IN_LUT(paramMID, hw_mbox_opcode, CID_LUT_PEER_IS_SUBJECT);
                if (paramCID != HW_MBOX_CID_INVALID)
                {
                    /* Set a special flag so that ISR can identify it's an INIT channel: */
                    DPRINTK("using cid %d as INIT CID master %d, sending opcode message", paramCID, paramMID);
                    CHN_SET_INIT(paramCID, True);

                    DPRINTK("cid %d: copying %uB [out of total %uB channel has] to SRAM at offset=0x%08x",
                            paramCID, opFlds.lenInBytes, CHN_CFG_MEMBUFF_GET_SIZE(paramCID), CHN_CFG_MEMBUFF_GET_START(paramCID));

                    /* Send the opcode message: */
                    HW_MBOX_INTERNAL_MESSAGE_COPY_TO_SRAM(&opcodeMsg,
                                                          CHN_CFG_MEMBUFF_GET_START(paramCID));

                    DPRINTK("using cid %d as INIT CID master %d, sending cmdRegVal=0x%08x", paramCID, paramMID, cmdRegVal);

                    HW_MBOX_CHN_COMMAND_REG_SET(paramCID, cmdRegVal);
                }
                else
                {
                    PRINTK_ERR("can't find an INIT CID for valid master %d", paramMID);
                }
            }
        }

        /* currently INIT_COMPLETE notification is not used in supervisor's driver... */
        SET_INIT_COMPLETE();

        /* reset SRAM of channel where this Master [supervisor] is Commander: */
        DPRINTK("Reset SRAM of channels where supervisor is commander");

        regVal = HW_MBOX_MASTER_COMMANDER_L_REG_GET();
        DPRINTK("commander initial SRAM reset: master commander low reg 0x%08x", regVal);

        for (i = 0; i < HW_MBOX_CHANNEL_COUNT; i++)
        {
            if (0 == regVal)
            {
                if (i < 32)
                {
                    DPRINTK("no more channels in commander low reg val...");

                    /* go to high register */
                    regVal = HW_MBOX_MASTER_COMMANDER_H_REG_GET();
                    DPRINTK("commander initial SRAM reset: master commander high reg 0x%08x", regVal);

                    i = 31;     /* jump to high CID range... */
                    continue;
                }
                else
                {
                    DPRINTK("no more channels in commander high reg val...");
                    break; /* done! */
                }
            }

            DPRINTK("commander initial SRAM reset: checking cid %d", i);

            if ((regVal & BIT(i % 32)) != 0)
            {
                /* commander on this channel! */
                DPRINTK("commander initial SRAM reset: master is commander on cid %d!", i);

                if (CHN_CFG_MEMBUFF_GET_SIZE(i) > 0)
                {
                    /* Reset Channel SRAM: */
                    DPRINTK("commander initial SRAM reset: resetting %uB of SRAM at offset=0x%08x",
                            CHN_CFG_MEMBUFF_GET_SIZE(i), CHN_CFG_MEMBUFF_GET_START(i));

                    HW_MBOX_RESET_SRAM(CHN_CFG_MEMBUFF_GET_START(i), CHN_CFG_MEMBUFF_GET_SIZE(i));
                }

                regVal &= (~BIT(i % 32));    /* turn off bit, we got it... */
            }
        }

        /* Let all driver services that the device is initialized! */
        SET_INITIALIZED();
    }

#ifdef HW_MBOX_IS_APP_CPU
    if (!IS_SUPERVISOR())
    {
        /* APP-CPU and Non-Supervisor:
           If APP-CPU is not the supervisor there can be set bits in the
           Interrupt Status Registers prior to setting up the ISR. In this
           case the sent MSI message will not be intercepted. To make the
           bridge sample the Interrupt Status Registers again a mask/Un-mask
           of the interrupts will be performed. Should there be any set bits
           then a new MSI message will be sent [and now ISR is properly setup].
        */

        DPRINTK("APP-CPU: Disabling all interrupts!");
        HW_MBOX_DISABLE_ALL_INTERRUPTS();

        DPRINTK("APP-CPU: intrMaskRegs(H L): 0x%08x 0x%08x",
                HW_MBOX_MASTER_INTR_MASK_H_REG_GET(),
                HW_MBOX_MASTER_INTR_MASK_L_REG_GET());

        DPRINTK("APP-CPU: Enabling all interrupts!");
        HW_MBOX_ENABLE_ALL_INTERRUPTS();
    }
#endif /* HW_MBOX_IS_APP_CPU */

    return 0;
}

/**************************************************************************/
/*! \fn void __exit hwMbox_drv_exit(void)
 **************************************************************************
 *  \brief HW_MBOX driver exit function.
 *  \return VOID.
 **************************************************************************/
static void __exit hwMbox_drv_exit(void)
{
#ifdef HW_MBOX_SOCKETS_API
    sock_unregister(PF_HWMBOX);
    proto_unregister(&hwMbox_proto);
    DPRINTK("AF_HWMBOX socket family unregistered");
#endif /* HW_MBOX_SOCKETS_API */

#if defined HW_MBOX_IS_NP_CPU
    free_irq(HW_MBOX_INT_LINE, NULL);
#elif defined HW_MBOX_IS_APP_CPU
    {
        netss_interrupt_info_t irq_info;
        irq_info.func = NULL;
        irq_info.args = NULL;
        netss_interrupt_register(NETSS_INTERUPT_HWMBOX, -1, &irq_info);
    }

    iounmap(hwMboxDriverData.reg_base);
#ifdef HW_MBOX_EXTENDED_MEMORY
    iounmap(hwMboxDriverData.extMem_base);
#endif /* HW_MBOX_EXTENDED_MEMORY */
#endif
   chnint_cleanup(HW_MBOX_CHANNEL_COUNT);
}

/**************************************************************************/
/*! \fn int _hwMbox_sendOpcodeMsg(Uint32 paramCID, HW_MBOX_STATUS_e status)
 **************************************************************************
 *  \brief send status message in channel.
 *  \return 0 for success, negative error code otherwise.
 **************************************************************************/
static int _hwMbox_sendOpcodeMsg(Uint32 paramCID, HW_MBOX_STATUS_e status)
{
    hwMbox_opcodeMessage_t replyOpcodeMsg;
    hwMbox_opcode_t replyOpFlds = { 0 };
    Uint32 cmdRegVal;

    if (!HW_MBOX_CID_VALIDITY_CHECK(paramCID))
    {
        PRINTK_ERR("invalid cid value! [paramCID=%u]",
                   paramCID);
        return -EINVAL;
    }

    replyOpFlds.tag = HW_MBOX_OPCODE_TAG;
    replyOpFlds.lenInBytes = HW_MBOX_OPCODE_MESSAGE_SIZE;

    replyOpcodeMsg.msgType = HW_MBOX_OPCODE_MESSAGE_TYPE_STATUS;
    replyOpcodeMsg.msgData.msgStatus.status = status;

    /* Use the current values in the Reply OPCODE message and the Reply OPCODE fields structs: */
    DPRINTK("cid %d: sending back opcode message of type STATUS (%d), with status value 0x%x (%d)",
            paramCID, replyOpcodeMsg.msgType,
            replyOpcodeMsg.msgData.msgStatus.status, replyOpcodeMsg.msgData.msgStatus.status);

    DPRINTK("cid %d: copying %uB [out of total %uB channel has] to SRAM at offset=0x%08x",
            paramCID, replyOpFlds.lenInBytes, CHN_CFG_MEMBUFF_GET_SIZE(paramCID), CHN_CFG_MEMBUFF_GET_START(paramCID));

    /* Copy the opcode message to the SRAM: */
    HW_MBOX_INTERNAL_MESSAGE_COPY_TO_SRAM(&replyOpcodeMsg, CHN_CFG_MEMBUFF_GET_START(paramCID));

    /* Create opcode reply value: */
    HW_MBOX_OPCODE_FIELDS_SET_FROM_STRUCT(cmdRegVal, &replyOpFlds);

    DPRINTK("cid %d: opcode fields - tag=%u, lenInBytes=%u -> cmdRegVal=0x%08x",
            paramCID, replyOpFlds.tag, replyOpFlds.lenInBytes, cmdRegVal);

    DPRINTK("cid %d: before write to command register - state=%d", paramCID, HW_MBOX_CHN_STATE_REG_GET(paramCID));
    DPRINTK("cid %d: writing 0x%08x to the command register of the channel...", paramCID, cmdRegVal);
    HW_MBOX_CHN_COMMAND_REG_SET(paramCID, cmdRegVal); /* send Command... */
    DPRINTK("cid %d: after write to command register - state=%d", paramCID, HW_MBOX_CHN_STATE_REG_GET(paramCID));

    return 0;
}

/**************************************************************************/
/*! \fn int _hwMbox_SendMsg(Uint32 paramCID, Uint32 paramAppID,
 *                          Uint8 *dataBuf, Uint32 dataLen,
 *                          Uint32 userOrKernel)
 **************************************************************************
 *  \brief internal routine for hwMbox_sendCmdMsg() and hwMbox_sendReplyMsg().
 *  \return transmitted bytes on success, negative error code otherwise.
 **************************************************************************/
static int _hwMbox_SendMsg(Uint32 paramCID, Uint32 paramAppID,
                           Uint8 *dataBuf, Uint32 dataLen,
                           Uint32 userOrKernel)
{
    hw_mbox_type_e paramProto;
    Uint32 cmdRegVal;
    Int32 retVal;

    retVal = 0;

    CHECK_IS_DRV_INITIALIZED(return -ENETDOWN);

    if (!HW_MBOX_CID_VALIDITY_CHECK(paramCID))
    {
        PRINTK_ERR("invalid cid value! [paramCID=%u]",
                   paramCID);
        return -EINVAL;
    }

    if (!HW_MBOX_SOCKETS_APPID_VALIDITY_CHECK(paramAppID))
    {
        PRINTK_ERR("invalid appId value! [paramAppID = %u]",
                   paramAppID);
        return -EINVAL;
    }

    paramProto = HW_MBOX_PROTOCOL_BY_APPID(paramAppID);

    DPRINTK("cid %d: size of buffer %uB", paramCID, dataLen);

    if ((dataBuf == NULL) || (dataLen == 0))
    {
        PRINTK_ERR("cid %d: No Buffer specified! [dataBuf %p, dataLen %uB]",
                   paramCID, dataBuf, dataLen);
        return -ENOMEM;
    }

    if (hw_mbox_command == paramProto)
    {
        /* command protocol */

        if (dataLen > CHN_CFG_MEMBUFF_GET_SIZE(paramCID))
        {
            PRINTK_ERR("cid %d: command - data Buffer size (%uB) > SRAM size allocated to channel (%uB)!",
                       paramCID, dataLen, CHN_CFG_MEMBUFF_GET_SIZE(paramCID));
            return -EMSGSIZE;
        }

        switch (userOrKernel)
        {
            case HW_MBOX_COPY_USER:
                get_user(cmdRegVal, (Uint32 *)dataBuf);
                if (dataLen > 4)
                {
                    /* copy from dataBuf with 4 bytes offset */
                    if (HW_MBOX_USER_COPY_TO_SRAM((dataBuf + sizeof(cmdRegVal)), dataLen, CHN_CFG_MEMBUFF_GET_START(paramCID)))
                    {
                        return -ENOMEM;
                    }
                }
                break;

            case HW_MBOX_COPY_KERNEL:
                cmdRegVal = *((Uint32 *)dataBuf);
                if (dataLen > 4)
                {
                    /* copy from dataBuf with 4 bytes offset */
                    HW_MBOX_COPY_TO_SRAM((dataBuf + sizeof(cmdRegVal)), dataLen, CHN_CFG_MEMBUFF_GET_START(paramCID));
                }
                break;

            default:
                return -EINVAL;
        }

        DPRINTK("cid %d: command - [command value 0x%08x with %uB of data]",
                paramCID, cmdRegVal, dataLen);
    }
    else if (hw_mbox_opcode == paramProto)
    {
        /* opcode protocol */
        hwMbox_opcode_t opFlds = { 0 };
#ifdef HW_MBOX_EXTENDED_MEMORY
        hwMbox_extMemMsgData_t sramMsg = { 0 };
#endif /* HW_MBOX_EXTENDED_MEMORY */

        DPRINTK("cid %d: opcode - [tag %u with %uB of data]",
                paramCID, paramAppID, dataLen);

        if (dataLen < HW_MBOX_OPCODE_MINIMAL_SRAM_ALLOCATION_IN_BYTES)
        {
            PRINTK_ERR("cid %d: opcode - Insufficient Size Buffer specified! [dataLen %uB, minimal size is %dB]",
                       paramCID, dataLen, HW_MBOX_OPCODE_MINIMAL_SRAM_ALLOCATION_IN_BYTES);

            return -EBADMSG;
        }

        opFlds.tag = paramAppID;
        opFlds.lenInBytes = dataLen;

        retVal = dataLen;

#ifdef HW_MBOX_EXTENDED_MEMORY
        if (CHN_CFG_IS_EXTMEM(paramCID))
        {
            if ((dataLen + sizeof(hwMbox_extMemCtrlHdr_t)) > HW_MBOX_EXTMEM_PART_SIZE)
            {
                PRINTK_ERR("cid %d: sendOpcode msg over EXTMEM is too long [(dataLen(%u) + sizeof(hwMbox_extMemCtrlHdr_t)(%u)) > extMem part size (%u)]!",
                           paramCID, dataLen, sizeof(hwMbox_extMemCtrlHdr_t), HW_MBOX_EXTMEM_PART_SIZE);
                return -EMSGSIZE;
            }

            DPRINTK("cid %d: copying %uB to EXTMEM at extMem offset=0x%x",
                    paramCID, dataLen, CHN_CFG_EXTMEM_GET_OFFSET(paramCID) + sizeof(hwMbox_extMemCtrlHdr_t));

            /* if we have data copy to EXTENDED MEMORY */
            if (dataLen > 0)
            {
                switch (userOrKernel)
                {
                    case HW_MBOX_COPY_USER:
                        if (HW_MBOX_USER_COPY_TO_EXTMEM(dataBuf, dataLen, (CHN_CFG_EXTMEM_GET_OFFSET(paramCID) + sizeof(hwMbox_extMemCtrlHdr_t))))
                        {
                            return -ENOMEM;
                        }
                        break;

                    case HW_MBOX_COPY_KERNEL:
                        HW_MBOX_COPY_TO_EXTMEM(dataBuf, dataLen, (CHN_CFG_EXTMEM_GET_OFFSET(paramCID) + sizeof(hwMbox_extMemCtrlHdr_t)));
                        break;

                    default:
                        return -EINVAL;
                }
            }

            {
                /*  set ownership bit */
                hwMbox_extMemCtrlHdr_t  extMemHdr;

                /* first word of Extended Memory is control header */
                extMemHdr = HW_MBOX_REG32_READ(HW_MBOX_EXTMEM_ADDR_BY_OFFSET(CHN_CFG_EXTMEM_GET_OFFSET(paramCID)));

                /* determine side. set ownership to 1 if commander, 0 overwise */
                if (HW_MBOX_MID == CHN_CFG_GET_COMMANDER(paramCID))
                {
                    HW_MBOX_SET_OWNER(extMemHdr);
                }
                else
                {
                    HW_MBOX_CLR_OWNER(extMemHdr);
                }

                DPRINTK("cid %d: writing new control header of extended memory (0x%08x) at extMem offset 0x%x, previous value=0x%08x",
                        paramCID, extMemHdr, CHN_CFG_EXTMEM_GET_OFFSET(paramCID),
                        HW_MBOX_REG32_READ(HW_MBOX_EXTMEM_ADDR_BY_OFFSET(CHN_CFG_EXTMEM_GET_OFFSET(paramCID))));

                HW_MBOX_REG32_WRITE(HW_MBOX_EXTMEM_ADDR_BY_OFFSET(CHN_CFG_EXTMEM_GET_OFFSET(paramCID)), extMemHdr);

                DPRINTK("cid %d: after writing control header = 0x%08x",
                        paramCID, HW_MBOX_REG32_READ(HW_MBOX_EXTMEM_ADDR_BY_OFFSET(CHN_CFG_EXTMEM_GET_OFFSET(paramCID))));
            }

            sramMsg.lenInBytes = dataLen;
            sramMsg.offset = CHN_CFG_EXTMEM_GET_OFFSET(paramCID) + sizeof(hwMbox_extMemCtrlHdr_t);

            DPRINTK("cid %d: sramMsg: len=%u, offset=0x%08x",
                    paramCID, sramMsg.lenInBytes, sramMsg.offset);

            /* overwrite opcode len with 8 bytes of extmem structure */
            opFlds.lenInBytes = sizeof(sramMsg);

            /* Send the opcode message: */
            HW_MBOX_INTERNAL_MESSAGE_COPY_TO_SRAM(&sramMsg, CHN_CFG_MEMBUFF_GET_START(paramCID));
        }
        else
#endif /* HW_MBOX_EXTENDED_MEMORY */
        {
            if (dataLen > CHN_CFG_MEMBUFF_GET_SIZE(paramCID))
            {
                PRINTK_ERR("cid %d: opcode - data Buffer size (%uB) > SRAM size allocated to channel (%uB)!",
                           paramCID, dataLen, CHN_CFG_MEMBUFF_GET_SIZE(paramCID));
                return -EMSGSIZE;
            }

            if (dataLen > 0)
            {
                switch (userOrKernel)
                {
                    case HW_MBOX_COPY_USER:
                        if (HW_MBOX_USER_COPY_TO_SRAM(dataBuf, dataLen, CHN_CFG_MEMBUFF_GET_START(paramCID)))
                        {
                            return -ENOMEM;
                        }
                    break;

                    case HW_MBOX_COPY_KERNEL:
                        HW_MBOX_COPY_TO_SRAM(dataBuf, dataLen, CHN_CFG_MEMBUFF_GET_START(paramCID));
                        break;

                    default:
                        return -EINVAL;
                }
            }
        }

        /* Create the opcode value: */
        HW_MBOX_OPCODE_FIELDS_SET_FROM_STRUCT(cmdRegVal, &opFlds);

        DPRINTK("cid %d: opcode - fields: tag=%u, lenInBytes=%u -> cmdRegVal=0x%08x",
                paramCID, opFlds.tag, opFlds.lenInBytes, cmdRegVal);
    }
    else
    {
        /* other protocols not supported*/
        return -EINVAL;
    }

    DPRINTK("cid %d: copying %uB [out of total %uB channel has] to SRAM at offset=0x%08x",
            paramCID, dataLen, CHN_CFG_MEMBUFF_GET_SIZE(paramCID), CHN_CFG_MEMBUFF_GET_START(paramCID));

    DPRINTK("cid %d: before write to command register - state=%d", paramCID, HW_MBOX_CHN_STATE_REG_GET(paramCID));
    DPRINTK("cid %d: writing 0x%08x to the command register of the channel...", paramCID, cmdRegVal);
    HW_MBOX_CHN_COMMAND_REG_SET(paramCID, cmdRegVal);
    DPRINTK("cid %d: after write to command register - state=%d", paramCID, HW_MBOX_CHN_STATE_REG_GET(paramCID));

    DPRINTK("cid %d: returning %d", paramCID, retVal);
    return retVal;
}

/**************************************************************************/
/*! \fn int hwMbox_sendCmdMsg(hw_mbox_Masters_e subject, Uint32 paramAppID,
                              Uint8 *dataBuf, Uint32 dataLen,
                              Uint32 userOrKernel)
 **************************************************************************
 *  \brief lock channel and write message to subject.
 *  \return transmitted bytes on success, negative error code otherwise.
 **************************************************************************/
static int hwMbox_sendCmdMsg(hw_mbox_Masters_e subject, Uint32 paramAppID,
                             Uint8 *dataBuf, Uint32 dataLen,
                             Uint32 userOrKernel)
{
    Uint32 paramCID;
    Int32 retVal;

    CHECK_IS_DRV_INITIALIZED(return -ENETDOWN);

    if (!HW_MBOX_MID_VALIDITY_CHECK(subject))
    {
        PRINTK_ERR("invalid subject value! [subject=%d]",
                   subject);
        return -EINVAL;
    }

    if (!HW_MBOX_SOCKETS_APPID_VALIDITY_CHECK(paramAppID))
    {
        PRINTK_ERR("invalid appId value! [paramAppID=%u]",
                   paramAppID);
        return -EINVAL;
    }

    if (!IS_MASTER_INIT(subject))
    {
        DPRINTK("master %s(%d) is not initialized!",
                HW_MBOX_MASTER_GET_NAME(subject), subject);
        return -EHOSTDOWN;
    }

    CHECK_FIND_CID_IN_LUT(subject,
                          HW_MBOX_PROTOCOL_BY_APPID(paramAppID),
                          CID_LUT_PEER_IS_SUBJECT,
                          return -EHOSTUNREACH,
                          paramCID);

    /* seize channel lock */
    if (down_interruptible(&CHN_LOCK(paramCID)))
    {
        DPRINTK("down interrupted by signal...");
        return -ERESTARTSYS;
    }

    retVal = _hwMbox_SendMsg(paramCID, paramAppID, dataBuf, dataLen, userOrKernel);

    /* unlock the channel in case of error */
    if (retVal < 0)
    {
        up(&CHN_LOCK(paramCID));
    }

    return retVal;
}

/**************************************************************************/
/*! \fn int hwMbox_sendReplyMsg(hw_mbox_Masters_e commander, Uint32 paramAppID,
                                Uint8 *dataBuf, Uint32 dataLen,
                                Uint32 userOrKernel)
 **************************************************************************
 *  \brief write reply to commander and unlock channel.
 *  \return [ transmitted bytes on success / -1 on error ].
 **************************************************************************/
static int hwMbox_sendReplyMsg(hw_mbox_Masters_e commander, Uint32 paramAppID,
                               Uint8 *dataBuf, Uint32 dataLen,
                               Uint32 userOrKernel)
{
    Uint32 paramCID;
    Int32 retVal;

    CHECK_IS_DRV_INITIALIZED(return -ENETDOWN);

    if (!HW_MBOX_MID_VALIDITY_CHECK(commander))
    {
        PRINTK_ERR("invalid commander value! [commander = %d]",
                   commander);
        return (-1);
    }

    if (!HW_MBOX_SOCKETS_APPID_VALIDITY_CHECK(paramAppID))
    {
        PRINTK_ERR("invalid appId value! [paramAppID = %u]",
                   paramAppID);
        return (-1);
    }

    CHECK_FIND_CID_IN_LUT(commander,
                          HW_MBOX_PROTOCOL_BY_APPID(paramAppID),
                          CID_LUT_PEER_IS_COMMANDER,
                          return -EHOSTUNREACH,
                          paramCID);

    retVal = _hwMbox_SendMsg(paramCID, paramAppID, dataBuf, dataLen, userOrKernel);

    return retVal;
}

/**************************************************************************/
/*! \fn int _hwMbox_RecvMsg(Uint32 paramCID, Uint32 paramAppID,
                            Uint8 *dataBuf, Uint32 dataLen,
                            Uint32 userOrKernel,
                            Uint32 eraseMemAfter,
                            HW_MBOX_STATUS_e *pHwMboxStatus)
 **************************************************************************
 *  \brief internal routine for hwMbox_RecvCmd(), hwMbox_ReadReply().
 *  \return received bytes on success, negative error code otherwise.
 **************************************************************************/
static int _hwMbox_RecvMsg(Uint32 paramCID, Uint32 paramAppID,
                           Uint8 *dataBuf, Uint32 dataLen,
                           Uint32 userOrKernel,
                           Uint32 eraseMemAfter,
                           HW_MBOX_STATUS_e *pHwMboxStatus)
{
    hw_mbox_type_e paramProto;
    Uint32 cmdRegVal;
    Int32 retVal;

    retVal = 0;

    if (!HW_MBOX_CID_VALIDITY_CHECK(paramCID))
    {
        PRINTK_ERR("invalid cid value! [paramCID=%u]",
                   paramCID);
        return -EINVAL;
    }

    if (!HW_MBOX_SOCKETS_APPID_VALIDITY_CHECK(paramAppID))
    {
        PRINTK_ERR("invalid appId value! [paramAppID=%u]",
                   paramAppID);
        return -EINVAL;
    }

    paramProto = HW_MBOX_PROTOCOL_BY_APPID(paramAppID);

    if ((dataBuf == NULL) || (dataLen == 0))
    {
        PRINTK_ERR("cid %d: No Buffer specified! [dataBuf %p, dataLen %uB]",
                   paramCID, dataBuf, dataLen);

        return -ENOBUFS;
    }

    /* get the Reply: */
    cmdRegVal = HW_MBOX_CHN_COMMAND_REG_GET(paramCID);

    DPRINTK("cid %d: reply from subject cmdRegVal=0x%08x", paramCID, cmdRegVal);

    retVal = dataLen;

    if (hw_mbox_command == paramProto)
    {
        /* command protocol */

        if (dataLen > CHN_CFG_MEMBUFF_GET_SIZE(paramCID))
        {
            PRINTK_ERR("cid %d: command - data len (%uB) > SRAM size allocated to channel (%uB)!",
                       paramCID, dataLen, CHN_CFG_MEMBUFF_GET_SIZE(paramCID));
            return -ENOBUFS;
        }

        switch (userOrKernel)
        {
            case HW_MBOX_COPY_USER:
                put_user(cmdRegVal, (Uint32 *)dataBuf);
                if (dataLen > 4)
                {
                    /* copy to dataBuf with 4 bytes offset */
                    if (HW_MBOX_USER_COPY_FROM_SRAM(CHN_CFG_MEMBUFF_GET_START(paramCID), dataLen, (dataBuf + sizeof(cmdRegVal))))
                    {
                        return -ENOMEM;
                    }
                }
                break;

            case HW_MBOX_COPY_KERNEL:
                *(Uint32 *)dataBuf = cmdRegVal;
                if (dataLen > 4)
                {
                    /* copy to dataBuf with 4 bytes offset */
                    HW_MBOX_COPY_FROM_SRAM(CHN_CFG_MEMBUFF_GET_START(paramCID), dataLen, (dataBuf + sizeof(cmdRegVal)));
                }
                break;

            default:
                return -EINVAL;
        }
    }
    else if (hw_mbox_opcode == paramProto)
    {
        /* opcode protocol */
        hwMbox_opcode_t opFlds = { 0 };
        hwMbox_opcodeMessage_t *pOpcodeMsg;
#ifdef HW_MBOX_EXTENDED_MEMORY
        hwMbox_extMemMsgData_t sramMsg = { 0 };
#endif /* HW_MBOX_EXTENDED_MEMORY */

        /* parse to opcode fields: */
        HW_MBOX_OPCODE_FIELDS_GET_TO_STRUCT(cmdRegVal, &opFlds);

        /* First check for internal replies: */
        if (opFlds.tag == HW_MBOX_OPCODE_TAG)
        {
            /* 3 possible options here:
                 1. this was a HW MBOX TAG sendOpcode.
                 2. this was not a HW MBOX sendOpcode but it failed and the
                    data returned is a Status Message with the error code.
                 3. this was not a HW MBOX sendOpcode and subject used
                    sendAckOpcode to simply ACK the sent opcode.
            */

            /* Either way we should check for a status message: */
            if (opFlds.lenInBytes != HW_MBOX_OPCODE_MESSAGE_SIZE)
            {
                PRINTK_ERR("cid %d: invalid message for tag %u [got size %uB, excpected size %uB]!",
                           paramCID, HW_MBOX_OPCODE_TAG, opFlds.lenInBytes, HW_MBOX_OPCODE_MESSAGE_SIZE);

                /* Output status messge in User Buffer: */
                pOpcodeMsg = (hwMbox_opcodeMessage_t *)dataBuf;

                /* Change to correct Opcode message to output to User: */
                pOpcodeMsg->msgType = HW_MBOX_OPCODE_MESSAGE_TYPE_STATUS;
                pOpcodeMsg->msgData.msgStatus.status = HW_MBOX_STATUS_OPCODE_ERROR_INVALID_MESSAGE;

                /* Set the correct size: */
                retVal = HW_MBOX_OPCODE_MESSAGE_SIZE;

                return -EBADMSG;
            }

            /* Opcode message was sent: */
            pOpcodeMsg = (hwMbox_opcodeMessage_t *)dataBuf;

            /* Copy the Opcode Message from SRAM: */

            DPRINTK("cid %d: copying %uB [out of total %uB channel has] from SRAM at offset 0x%08x",
                    paramCID, opFlds.lenInBytes, CHN_CFG_MEMBUFF_GET_SIZE(paramCID), CHN_CFG_MEMBUFF_GET_START(paramCID));

            HW_MBOX_INTERNAL_MESSAGE_COPY_FROM_SRAM(CHN_CFG_MEMBUFF_GET_START(paramCID),
                                                    pOpcodeMsg);

            /* output the Reply size: */
            retVal = opFlds.lenInBytes;

            DPRINTK("cid %d: received HW_MBOX_OPCODE_MESSAGE - msgType %d",
                    paramCID, pOpcodeMsg->msgType);

            if (pOpcodeMsg->msgType != HW_MBOX_OPCODE_MESSAGE_TYPE_STATUS)
            {
                PRINTK_ERR("cid %d: send Opcode - unexpected message type for tag %u at this point [got type %d, excpected type %d]!",
                           paramCID, HW_MBOX_OPCODE_TAG, pOpcodeMsg->msgType, HW_MBOX_OPCODE_MESSAGE_TYPE_STATUS);

                /* Change to correct Opcode message to output to User: */
                pOpcodeMsg->msgType = HW_MBOX_OPCODE_MESSAGE_TYPE_STATUS;
                pOpcodeMsg->msgData.msgStatus.status = HW_MBOX_STATUS_OPCODE_ERROR_ILLEGAL_MESSAGE;

                return -EBADMSG;
            }

            /* Opcode Reply Message of type STATUS: */

            DPRINTK("cid %d: received HW_MBOX_OPCODE_MESSAGE of type STATUS(%d) - STATUS = 0x%x (%u)",
                    paramCID, pOpcodeMsg->msgType,
                    pOpcodeMsg->msgData.msgStatus.status, pOpcodeMsg->msgData.msgStatus.status);

            /* Check the returned STATUS value: */
            switch (pOpcodeMsg->msgData.msgStatus.status)
            {
                case HW_MBOX_STATUS_OK:
                    DPRINTK("cid %d: send Opcode - subject replied with ACK!", paramCID);

                    /* ACK should succeed and return with no Data: */
                    retVal = 0;
                    break;

                case HW_MBOX_STATUS_OPCODE_ERROR_INVALID_TAG:
                    DPRINTK("cid %d: send Opcode - subject replied with OPCODE INVALID TAG!", paramCID);
                    retVal = (-ECONNREFUSED);
                    break;

                default:
                    /* Internal Error! */
                    PRINTK_ERR("cid %d: send Opcode - subject replied with internal error! STATUS = 0x%x (%u)",
                               paramCID, pOpcodeMsg->msgData.msgStatus.status, pOpcodeMsg->msgData.msgStatus.status);

                    retVal = (-EBADMSG);
            }

#ifdef HW_MBOX_EXTENDED_MEMORY
            /* clear the ownership flag if commander and the channel is with extended memory support */
            if ( CHN_CFG_IS_EXTMEM(paramCID) &&
                 (CHN_CFG_GET_COMMANDER(paramCID) == HW_MBOX_MID) )
            {
                hwMbox_extMemCtrlHdr_t extMemHdr = { 0 };
                HW_MBOX_CLR_OWNER(extMemHdr);
                HW_MBOX_REG32_WRITE(HW_MBOX_EXTMEM_ADDR_BY_OFFSET(CHN_CFG_EXTMEM_GET_OFFSET(paramCID)), extMemHdr);

                DPRINTK("cid %d: entering reading loop on header at ext mem offset=%x (current value 0x%08x)...",
                        paramCID, CHN_CFG_EXTMEM_GET_OFFSET(paramCID),
                        HW_MBOX_REG32_READ(HW_MBOX_EXTMEM_ADDR_BY_OFFSET(CHN_CFG_EXTMEM_GET_OFFSET(paramCID))));

                /* wait till ownership bit will be changed */
                do
                {
                    extMemHdr = HW_MBOX_REG32_READ(HW_MBOX_EXTMEM_ADDR_BY_OFFSET(CHN_CFG_EXTMEM_GET_OFFSET(paramCID)));
                    DPRINTK("cid %d: reading loop on header at ext mem offset=%x (current value 0x%08x)...",
                            paramCID, CHN_CFG_EXTMEM_GET_OFFSET(paramCID), extMemHdr);
                } while (!HW_MBOX_EXTMEM_IS_COMMANDER_OWNER(extMemHdr) /* Loop until the commander the owner */);

                DPRINTK("cid %d: exited from reading loop...", paramCID);
            }
#endif /* HW_MBOX_EXTENDED_MEMORY */

            /* erase is not needed in this case */
            eraseMemAfter = 0;
        }
        else
        {
            DPRINTK("cid %d: cmdRegVal=0x%08x -> opcode fields: tag=%u, lenInBytes=%u",
                    paramCID, cmdRegVal, opFlds.tag, opFlds.lenInBytes);

            /* Copy the Opcode Message from SRAM: */
            DPRINTK("cid %d: copying %uB [out of total %uB channel has] from SRAM at offset 0x%08x",
                    paramCID, opFlds.lenInBytes, CHN_CFG_MEMBUFF_GET_SIZE(paramCID), CHN_CFG_MEMBUFF_GET_START(paramCID));

            /* Reply has data! */
            if (opFlds.lenInBytes > dataLen)
            {
                /* Opcode reply data longer than specified buffer... */
                PRINTK_ERR("cid %d: opcode reply data length (%uB) > specified buffer size (%uB)!",
                           paramCID, opFlds.lenInBytes, dataLen);

                *pHwMboxStatus = HW_MBOX_STATUS_OPCODE_ERROR_MESSAGE_TOO_LONG;

                return -EMSGSIZE;
            }

            DPRINTK("cid %d: opcode reply with %uB of data", paramCID, dataLen);

            if ((dataBuf != NULL) && (opFlds.lenInBytes > 0))
            {
                DPRINTK("cid %d: copying %uB [out of total %uB channel has] from SRAM at offset=0x%08x",
                        paramCID, opFlds.lenInBytes, CHN_CFG_MEMBUFF_GET_SIZE(paramCID), CHN_CFG_MEMBUFF_GET_START(paramCID));

#ifdef HW_MBOX_EXTENDED_MEMORY
                if (CHN_CFG_IS_EXTMEM(paramCID))
                {
                    if (sizeof(sramMsg) != opFlds.lenInBytes)
                    {
                        /* ext mem data length has wrong length: */
                        PRINTK_ERR("cid %d: ext mem data length (%uB) != size of extMemMsgData_t (%uB)",
                                   paramCID, opFlds.lenInBytes, sizeof(sramMsg));

                        *pHwMboxStatus = HW_MBOX_STATUS_OPCODE_ERROR_INVALID_MESSAGE;

                        return -EBADMSG;
                    }

                    HW_MBOX_INTERNAL_MESSAGE_COPY_FROM_SRAM(CHN_CFG_MEMBUFF_GET_START(paramCID),
                                                            &sramMsg);

                    DPRINTK("cid %d: received op message of EXTMEM type with offset=0x%08x and lenInBytes=%u",
                            paramCID, sramMsg.offset, sramMsg.lenInBytes);

                    /* data in EXTMEM more than maximum: */
                    if ((sramMsg.lenInBytes + sizeof(hwMbox_extMemCtrlHdr_t)) > HW_MBOX_EXTMEM_PART_SIZE)
                    {
                        PRINTK_ERR("cid %d: recvOpcode msg over EXTMEM is too long [(lenInBytes(%u) + sizeof(hwMbox_extMemCtrlHdr_t)(%u)) > extMem part size (%u)]!",
                                   paramCID,
                                   sizeof(hwMbox_extMemCtrlHdr_t),
                                   sramMsg.lenInBytes,
                                   HW_MBOX_EXTMEM_PART_SIZE);

                        *pHwMboxStatus = HW_MBOX_STATUS_OPCODE_ERROR_MESSAGE_TOO_LONG;

                        return -EMSGSIZE;
                    }

                    /* Sent data over EXTMEM is more than available buffer: */
                    if (sramMsg.lenInBytes > dataLen)
                    {
                        PRINTK_ERR("cid %d: recvOpcode EXTMEM - lenInBytes(%u) > dataLen (%u)!",
                                   paramCID, sramMsg.lenInBytes, dataLen);

                        *pHwMboxStatus = HW_MBOX_STATUS_OPCODE_ERROR_MESSAGE_TOO_LONG;

                        return -EMSGSIZE;
                    }

                    DPRINTK("cid %d: copying %uB from extMem offset=0x%08x",
                            paramCID, sramMsg.lenInBytes, sramMsg.offset);

                    {
                        /* wait for desired ownership value before read data from extended memory */
                        hwMbox_extMemCtrlHdr_t    extMemHdr;

                        /* determine side. set ownership to 1 if commander, 0 overwise */
                        DPRINTK("cid %d: Entering reading loop on header at ext mem offset=%x ...",
                                paramCID, sramMsg.offset - sizeof(hwMbox_extMemCtrlHdr_t));

                        /* wait till ownership bit will be changed */
                        do
                        {
                            extMemHdr = HW_MBOX_REG32_READ(HW_MBOX_EXTMEM_ADDR_BY_OFFSET(sramMsg.offset) - sizeof(hwMbox_extMemCtrlHdr_t));
                            DPRINTK("cid %d: entering reading loop on header at ext mem offset=%x (current value 0x%08x)...",
                                    paramCID, sramMsg.offset - sizeof(hwMbox_extMemCtrlHdr_t), extMemHdr);

                        } while ( (HW_MBOX_MID == CHN_CFG_GET_COMMANDER(paramCID)) ? (!HW_MBOX_EXTMEM_IS_COMMANDER_OWNER(extMemHdr) /* Loop until the commander the owner */)
                                                                                   : (!HW_MBOX_EXTMEM_IS_SUBJECT_OWNER(extMemHdr) /* Loop until the subject the owner */) );

                        DPRINTK("cid %d: exited from reading loop...", paramCID);
                    }

                    if (sramMsg.lenInBytes > 0)
                    {
                        switch (userOrKernel)
                        {
                            case HW_MBOX_COPY_USER:
                                if (HW_MBOX_USER_COPY_FROM_EXTMEM(sramMsg.offset, sramMsg.lenInBytes, dataBuf))
                                {
                                    *pHwMboxStatus = HW_MBOX_STATUS_OPCODE_ERROR;

                                    return -ENOMEM;
                                }
                                break;

                            case HW_MBOX_COPY_KERNEL:
                                HW_MBOX_COPY_FROM_EXTMEM(sramMsg.offset, sramMsg.lenInBytes, dataBuf);
                                break;

                            default:
                                return -EINVAL;
                        }

                        /* erase ExtMem if eraseMemAfter flag is set */
                        if (eraseMemAfter)
                        {
                            HW_MBOX_RESET_EXTMEM(sramMsg.offset, sramMsg.lenInBytes);
                        }
                    }

                    DPRINTK("cid %d: AFTER copying %uB from ext memory at offset=0x%08x",
                            paramCID, dataLen, sramMsg.offset);

                    /* overwrite retVal with len of EXTMEM message size */
                    retVal = sramMsg.lenInBytes;
                }
                else
#endif /* HW_MBOX_EXTENDED_MEMORY */
                {
                    switch (userOrKernel)
                    {
                        case HW_MBOX_COPY_USER:
                            if (HW_MBOX_USER_COPY_FROM_SRAM(CHN_CFG_MEMBUFF_GET_START(paramCID), dataLen, dataBuf))
                            {
                                *pHwMboxStatus =  HW_MBOX_STATUS_OPCODE_ERROR;

                                return -ENOMEM;
                            }
                            break;

                        case HW_MBOX_COPY_KERNEL:
                            HW_MBOX_COPY_FROM_SRAM(CHN_CFG_MEMBUFF_GET_START(paramCID), dataLen, dataBuf);
                            break;

                        default:
                            return -EINVAL;
                    }
                }
            }
        }
    }
    else
    {
        /* other except COMMAND and OPCODE protocols are not supported */
        return -EINVAL;
    }

    if (eraseMemAfter)
    {
        HW_MBOX_RESET_SRAM(CHN_CFG_MEMBUFF_GET_START(paramCID), CHN_CFG_MEMBUFF_GET_SIZE(paramCID));    /* TBD: or dataLen? */
    }

    return retVal;
}

/**************************************************************************/
/*! \fn int hwMbox_recvReplyMsg(hw_mbox_Masters_e subject, Uint32 paramAppID,
                                Uint8 *dataBuf, Uint32 dataLen,
                                Uint32 userOrKernel,
                                Uint32 eraseMemAfter)
 **************************************************************************
 *  \brief wait for reply from subject, read it and unlock channel.
 *  \return [ received bytes on success / -1 on error ].
 **************************************************************************/
static int hwMbox_recvReplyMsg(hw_mbox_Masters_e subject, Uint32 paramAppID,
                               Uint8 *dataBuf, Uint32 dataLen,
                               Uint32 userOrKernel,
                               Uint32 eraseMemAfter)
{
    Uint32 paramCID;
    Int32 retVal;
    HW_MBOX_STATUS_e HwMboxStatus;

    CHECK_IS_DRV_INITIALIZED(return (-1));

    if (!HW_MBOX_MID_VALIDITY_CHECK(subject))
    {
        PRINTK_ERR("invalid subject value! [subject = %d]",
                   subject);
        return (-1);
    }

    if (!HW_MBOX_SOCKETS_APPID_VALIDITY_CHECK(paramAppID))
    {
        PRINTK_ERR("invalid appId value! [paramAppID = %u]",
                   paramAppID);
        return (-1);
    }

    if (!IS_MASTER_INIT(subject))
    {
        DPRINTK("master %s(%d) is not initialized!",
                HW_MBOX_MASTER_GET_NAME(subject), subject);
        return -EHOSTDOWN;
    }

    CHECK_FIND_CID_IN_LUT(subject,
                          HW_MBOX_PROTOCOL_BY_APPID(paramAppID),
                          CID_LUT_PEER_IS_SUBJECT,
                          return -EHOSTUNREACH,
                          paramCID);

    /* We should always use the channel's WQ: */
    DPRINTK("cid %d: before WAIT_FOR_CHN_INTR sem=%d",
            paramCID, CHN_LOCK(paramCID).count);

    WAIT_FOR_CHN_INTR(paramCID, CHN_INTR_DEFAULT_TIMEOUT_MSEC);

    retVal = _hwMbox_RecvMsg(paramCID,
                             paramAppID,
                             dataBuf,
                             dataLen,
                             userOrKernel,
                             eraseMemAfter,
                             &HwMboxStatus);

    STATS_INC_COUNTER(subject,
                      HW_MBOX_PROTOCOL_BY_APPID(paramAppID),
                      STATS_PEER_IS_SUBJECT);

    /* release lock */
    up(&CHN_LOCK(paramCID));

    return retVal;
}

/**************************************************************************/
/*! \fn int hwMbox_recvCmdMsg(hw_mbox_Masters_e commander, Uint32 paramAppID,
                              Uint8 *dataBuf, Uint32 dataLen,
                              Uint32 userOrKernel,
                              Uint32 eraseMemAfter)
 **************************************************************************
 *  \brief receive command message from commander.
 *  \note first waits on channel's WQ if it is required.
 *  \return [ received bytes on success / -1 on error ].
 **************************************************************************/
static int hwMbox_recvCmdMsg(hw_mbox_Masters_e commander, Uint32 paramAppID,
                             Uint8 *dataBuf, Uint32 dataLen,
                             Uint32 userOrKernel,
                             Uint32 eraseMemAfter)
{
    Uint32 paramCID;
    Int32 retVal;
    hw_mbox_type_e paramProto;
    HW_MBOX_STATUS_e hwMboxStatus = HW_MBOX_STATUS_OK;

    CHECK_IS_DRV_INITIALIZED(return -ENETDOWN);

    if (!HW_MBOX_MID_VALIDITY_CHECK(commander))
    {
        PRINTK_ERR("invalid commander value! [commander = %d]",
                   commander);
        return (-1);
    }

    if (!HW_MBOX_SOCKETS_APPID_VALIDITY_CHECK(paramAppID))
    {
        PRINTK_ERR("invalid appId value! [paramAppID = %u]",
                   paramAppID);
        return (-1);
    }

    paramProto = HW_MBOX_PROTOCOL_BY_APPID(paramAppID);

    CHECK_FIND_CID_IN_LUT(commander,
                          paramProto,
                          CID_LUT_PEER_IS_COMMANDER,
                          return -EHOSTUNREACH,
                          paramCID);

    if ( !(CHN_IS_RECV_OP(paramCID) && (HW_MBOX_COPY_USER == userOrKernel)) )
    {
        /* Any case not socket and recvOpcode should use the channel's WQ.
           Subject opcode sockets should use their own WQ.
        */
        DPRINTK("cid %d: before WAIT_FOR_CHN_INTR", paramCID);
        WAIT_FOR_CHN_INTR(paramCID, CHN_INTR_NO_TIMEOUT);
    }

    retVal = _hwMbox_RecvMsg(paramCID,
                             paramAppID,
                             dataBuf,
                             dataLen,
                             userOrKernel,
                             eraseMemAfter,
                             &hwMboxStatus);

    if (hw_mbox_command == paramProto)
    {
        /* Increment the counters only for command,
           for opcode the Listener does it...
        */
        STATS_INC_COUNTER(commander,
                          HW_MBOX_PROTOCOL_BY_APPID(paramAppID),
                          STATS_PEER_IS_COMMANDER);
    }

    if ((hw_mbox_opcode == paramProto) && (retVal < 0))
    {
        if (hwMboxStatus != HW_MBOX_STATUS_OK)
        {
            /* Send internal opcode message to commander */
            if (_hwMbox_sendOpcodeMsg(paramCID, hwMboxStatus) < 0)
            {
                PRINTK_ERR("failed to send internal opcode message to commander! [paramCID=%u, paramAppID=%u, hwMboxStatus=%d]",
                           paramCID, paramAppID, hwMboxStatus);
            }
        }
    }

    return retVal;
}

static int hwMbox_RecvEvent(hw_mbox_Masters_e paramMID, Uint32 paramAppID, Uint8 *dataBuf, Uint32 dataLen, Uint32 userOrKernel, Uint32 eraseMemAfter)
{
    /* TBD */
    return -EPROTONOSUPPORT;
}

#ifdef HW_MBOX_SOCKETS_API
/**************************************************************************/
/*! \fn int hwMbox_socketRecvOpCb(hw_mbox_Masters_e commander, Uint8 *dataBuf,
 *                                Uint32 dataLen, Uint32 *token)
 **************************************************************************
 *  \brief recvOp callback function used for sockets.
 *  \return [ 0 on success / -1 on error ].
 **************************************************************************/
static int hwMbox_socketRecvOpCb(hw_mbox_Masters_e commander, Uint8 *dataBuf,
                                 Uint32 dataLen, Uint32 *token)
{
    struct socket *sock = NULL;
    struct hwMbox_sock *sk;

    if (!token)
    {
        PRINTK_ERR("token is NULL!");
        return (-1);
    }

    sock = (struct socket *)(*token);
    if ((sock) && (sk = hwMbox_sk(sock->sk)))
    {
        if (sock->wq)
        {
            DPRINTK_SKT(sock, "before trigger: chnIntr=%d", sk->chnIntr);
            sk->chnIntr = (__s32)CHN_INTR_STATE_PENDING;
            wake_up_interruptible(&sock->wq->wait);
            DPRINTK_SKT(sock, "after trigger: chnIntr=%d", sk->chnIntr);
        }
    }
    else
    {
        PRINTK_ERR("NULL pointer! [sock=%p]", sock);
    }

    return 0;
}

/************************************************************************/
/*     AF_HWMBOX socket family functions                                */
/************************************************************************/

/**************************************************************************/
/*! \fn int hwMbox_socket_create(struct net *net, struct socket *sock,
 *                               int protocol, int kern)
 **************************************************************************
 *  \brief AF_HWMBOX socket family socket() handler.
 *  \return [ 0 on success / error otherwise ].
 **************************************************************************/
static int hwMbox_socket_create(struct net *net, struct socket *sock,
                                int protocol, int kern)
{
    struct hwMbox_sock *sk = NULL;

    CHECK_IS_DRV_INITIALIZED(return (-ENETDOWN));

    /* two types of socket are supported */
    if ((SOCK_COMMANDER != sock->type) && (SOCK_SUBJECT != sock->type))
    {
        return -ESOCKTNOSUPPORT;
    }

    /* kernel sockets are not supported yet */
    if (kern)
    {
        return -EPFNOSUPPORT;
    }

    /* protocol is defined via appid */
    if (protocol != 0)
    {
        return -EPROTONOSUPPORT;
    }

    sk = hwMbox_sk(sk_alloc(net, PF_HWMBOX, GFP_ATOMIC, &hwMbox_proto));
    if (sk == NULL)
    {
        return -ENOMEM;
    }

    sock_init_data(sock, (struct sock *)sk);

    sock->ops = &hwMbox_proto_ops;

    /* initialize the state */
    sock->state = (socket_state)HW_MBOX_SOCK_STATE_CREATED;

    /* set invalid initial mid value with typecast,
       it will be used later to test is socket was bound already or not... */
    sk->mid   = (__typeof__(sk->mid)) HW_MBOX_MID_INVALID;

    /* initialize the chnIntr: */
    sk->chnIntr = (__s32)CHN_INTR_STATE_NONE;

    /* store pid of creator */
    sk->owner_pid = current->pid;

    /* zero stat */
    sk->stat.sent_cnt = 0;
    sk->stat.sent_bytes = 0;
    sk->stat.recv_cnt = 0;
    sk->stat.recv_bytes = 0;

    /* add socket to hwMbox_sock_hlist */
    if (down_interruptible(&hwMbox_sock_hlist_sem))
    {
        return -ERESTARTSYS;
    }

    hlist_add_head(&sock->sk->sk_node, &hwMbox_sock_hlist);

    up(&hwMbox_sock_hlist_sem);

    return 0;
}

/**************************************************************************/
/*! \fn int hwMbox_socket_bind(struct socket *sock, struct sockaddr *uaddr,
 *                             int addr_len)
 **************************************************************************
 *  \brief AF_HWMBOX socket family bind() handler.
 *  \return [ 0 on success / error otherwise ].
 **************************************************************************/
static int hwMbox_socket_bind(struct socket *sock, struct sockaddr *uaddr,
                              int addr_len)
{
    struct sockaddr_hwMbox *addr = (struct sockaddr_hwMbox *)uaddr;
    struct hwMbox_sock *sk;

    hw_mbox_type_e paramProto;
    Uint32 paramCID;

    if (!(sk = hwMbox_sk(sock->sk)))
    {
        PRINTK_ERR("NULL pointer! [sk=%p]", sk);
        return -EFAULT;
    }

    if (addr->shwMbox_family != AF_HWMBOX)
    {
        return -EINVAL;
    }

    if (addr_len != sizeof(struct sockaddr_hwMbox))
    {
        return -EINVAL;
    }

    /* multiple bind() calls not allowed */
    if (HW_MBOX_MID_VALIDITY_CHECK(sk->mid))
    {
        return -EOPNOTSUPP;
    }

    if (!HW_MBOX_MID_VALIDITY_CHECK(addr->shwMbox_mid))
    {
        return -EADDRNOTAVAIL;
    }

    if (!HW_MBOX_SOCKETS_APPID_VALIDITY_CHECK(addr->shwMbox_appid))
    {
        return -EADDRNOTAVAIL;
    }

    paramProto = HW_MBOX_PROTOCOL_BY_APPID(addr->shwMbox_appid);

    CHECK_FIND_CID_IN_LUT(addr->shwMbox_mid,
                          paramProto,
                          (SOCK_COMMANDER == sock->type) ? CID_LUT_PEER_IS_SUBJECT
                                                         : CID_LUT_PEER_IS_COMMANDER,
                          return (-EHOSTUNREACH),
                          paramCID);

    if (SOCK_COMMANDER == sock->type)
    {
        if (!IS_MASTER_INIT(addr->shwMbox_mid))
        {
            DPRINTK("master %s(%d) is not initialized!",
                    HW_MBOX_MASTER_GET_NAME(addr->shwMbox_mid), addr->shwMbox_mid);
            return -EHOSTDOWN;
        }

        /* event proto for COMMANDER is not available */
        if (hw_mbox_event == paramProto)
        {
            return -EPROTONOSUPPORT;
        }
    }

    if ( (SOCK_SUBJECT == sock->type) &&
         (hw_mbox_opcode == paramProto) )
    {
        /* register callback function for the tag with token equal to address of socket: */
        if (hwMbox_registerRecvOpcode(addr->shwMbox_mid, hwMbox_socketRecvOpCb, addr->shwMbox_appid, (Uint32)sock))
        {
            return -EADDRINUSE;
        }
    }

    sk->appid = addr->shwMbox_appid;
    sk->mid   = addr->shwMbox_mid;
    sk->cid   = paramCID;

    sock->state = HW_MBOX_SOCK_STATE_BOUND;

    return 0;
}

/**************************************************************************/
/*! \fn int hwMbox_socket_sendmsg(struct kiocb *kiocb, struct socket *sock,
 *                                struct msghdr *msg, size_t len)
 **************************************************************************
 *  \brief AF_HWMBOX socket family send() handler.
 *  \return [ amount of bytes sent on success / error otherwise ].
 **************************************************************************/
static int hwMbox_socket_sendmsg(struct kiocb *kiocb, struct socket *sock,
                                 struct msghdr *msg, size_t len)
{
    struct hwMbox_sock *sk;
    Uint8 *pUserBuf = (Uint8 *)msg->msg_iov->iov_base;
    Int32 retVal = 0;

    hw_mbox_type_e paramProto;

    DPRINTK_SKT(sock, "Enter sendmsg()...");

    if (!(sk = hwMbox_sk(sock->sk)))
    {
        PRINTK_ERR("NULL pointer! [sk=%p]", sk);
        return -EFAULT;
    }

    /* socket is not bound */
    if (!HW_MBOX_MID_VALIDITY_CHECK(sk->mid))
    {
        return -EOPNOTSUPP;
    }

    /* vectored IO not supported */
    if (msg->msg_iovlen > 1)
    {
        return -EOPNOTSUPP;
    }

    paramProto = HW_MBOX_PROTOCOL_BY_APPID(sk->appid);

    DPRINTK_SKT(sock,
                "type=%d, mid=%d, appId=%d, proto=%d, cid=%d",
                sock->type, sk->mid, sk->appid, paramProto, sk->cid);

    sock->state = HW_MBOX_SOCK_STATE_SENDING;
    DPRINTK_SKT(sock, "new socket state: %d", sock->state);

    /* only command and opcode protocols are supported at the moment... */
    if ((paramProto != hw_mbox_command) && (paramProto != hw_mbox_opcode))
    {
        return -EPROTONOSUPPORT;
    }

    if (SOCK_COMMANDER == sock->type)
    {
        retVal = hwMbox_sendCmdMsg(sk->mid, sk->appid, pUserBuf, len, HW_MBOX_COPY_USER);
    }
    else if (SOCK_SUBJECT == sock->type)
    {
        retVal = hwMbox_sendReplyMsg(sk->mid, sk->appid, pUserBuf, len, HW_MBOX_COPY_USER);
    }
    else
    {
        return -ESOCKTNOSUPPORT;
    }

    if (retVal >= 0)
    {
        sock->state = HW_MBOX_SOCK_STATE_SENT;
    }
    else
    {
        sock->state = HW_MBOX_SOCK_STATE_SEND_ERROR;
    }
    DPRINTK_SKT(sock, "new socket state: %d", sock->state);

    sk->stat.sent_cnt++;

    if (retVal > 0)
    {
        sk->stat.sent_bytes += len;
    }

    return retVal;
}

/**************************************************************************/
/*! \fn int hwMbox_socket_recvmsg(struct kiocb *kiocb, struct socket *sock,
                                  struct msghdr *msg, size_t len, int flags)
 **************************************************************************
 *  \brief AF_HWMBOX socket family recv() handler.
 *  \return [ amount of bytes received on success / error otherwise ].
 **************************************************************************/
static int hwMbox_socket_recvmsg(struct kiocb *kiocb, struct socket *sock,
                                 struct msghdr *msg, size_t len, int flags)
{
    struct hwMbox_sock *sk;
    Uint8 *pUserBuf = (Uint8 *)msg->msg_iov->iov_base;
    Uint32 eraseMemAfter = flags & MSG_TRUNC;
    Int32 retVal = 0;

    hw_mbox_type_e paramProto;

    DPRINTK_SKT(sock, "Enter recvmsg()...");

    if (!(sk = hwMbox_sk(sock->sk)))
    {
        PRINTK_ERR("NULL pointer! [sk=%p]", sk);
        return -EFAULT;
    }

    /* socket is not bound */
    if (!HW_MBOX_MID_VALIDITY_CHECK(sk->mid))
    {
        return -EOPNOTSUPP;
    }

    if (!HW_MBOX_SOCKETS_APPID_VALIDITY_CHECK(sk->appid))
    {
        return -EADDRNOTAVAIL;
    }

    /* vectored IO not supported */
    if (msg->msg_iovlen > 1)
    {
        return -EOPNOTSUPP;
    }

    paramProto = HW_MBOX_PROTOCOL_BY_APPID(sk->appid);

    DPRINTK_SKT(sock,
                "type=%d, mid=%d, appId=%d, proto=%d, cid=%d",
                sock->type, sk->mid, sk->appid, paramProto, sk->cid);

    sock->state = HW_MBOX_SOCK_STATE_RECEIVING;
    DPRINTK_SKT(sock, "new socket state: %d", sock->state);

    if (SOCK_COMMANDER == sock->type)
    {
        /* Commanders recv reply from subjects: */

        if (hw_mbox_command == paramProto)
        {
            /* For command commander we should wait on channel's wq [with timeout]...
               [inside hwMbox_recvReplyMsg]
            */
            retVal = hwMbox_recvReplyMsg(sk->mid, sk->appid, pUserBuf, len, HW_MBOX_COPY_USER, eraseMemAfter);
        }
        else if (hw_mbox_opcode == paramProto)
        {
            /* For opcode commander we should wait on channel's wq [with timeout]...
               [inside hwMbox_recvReplyMsg]
            */
            retVal = hwMbox_recvReplyMsg(sk->mid, sk->appid, pUserBuf, len, HW_MBOX_COPY_USER, eraseMemAfter);
        }
        else
        {
            /* for commander socket only command and opcode
               protocols are supported... */
            return -EPROTONOSUPPORT;
        }
    }
    else if (SOCK_SUBJECT == sock->type)
    {
        /* Subjects recv cmd from commanders: */

        if (hw_mbox_command == paramProto)
        {
            /* For command subject we should wait on channel's wq [without timeout]...
               [inside hwMbox_recvCmdMsg]
            */
            retVal = hwMbox_recvCmdMsg(sk->mid, sk->appid, pUserBuf, len, HW_MBOX_COPY_USER, eraseMemAfter);
        }
        else if (hw_mbox_opcode == paramProto)
        {
            DPRINTK_SKT(sock, "before wait on socket's WQ [&sock->wq->wait=0x%p, chnIntr=%d]",
                        &sock->wq->wait, sk->chnIntr);
            if (wait_event_interruptible(sock->wq->wait, CHN_INTR_STATE_PENDING == (chnIntrState_e)sk->chnIntr))
            {
                DPRINTK_SKT(sock, "recvmsg() interrupted by signal while waiting on socket's WQ...");
                return -ECONNABORTED;
            }
            DPRINTK_SKT(sock, "after wait on socket's WQ...");

            DPRINTK_SKT(sock, "before clear: chnIntr=%d", sk->chnIntr);
            sk->chnIntr = (__s32)CHN_INTR_STATE_NONE;
            DPRINTK_SKT(sock, "after clear: chnIntr=%d", sk->chnIntr);

            /* Now we can receive... */
            retVal = hwMbox_recvCmdMsg(sk->mid, sk->appid, pUserBuf, len, HW_MBOX_COPY_USER, eraseMemAfter);
        }
        else if (hw_mbox_event == paramProto)
        {
            /* event proto */
            retVal = hwMbox_RecvEvent(sk->mid, sk->appid, pUserBuf, len, HW_MBOX_COPY_USER, eraseMemAfter);
        }
        else
        {
            return -EPROTONOSUPPORT;
        }
    }
    else
    {
        return -ESOCKTNOSUPPORT;
    }

    sk->stat.recv_cnt++;

    if (retVal > 0)
    {
        sk->stat.recv_bytes += retVal;
    }

    return retVal;
}

/**************************************************************************/
/*! \fn int hwMbox_socket_release(struct socket *sock)
 **************************************************************************
 *  \brief AF_HWMBOX socket family release() handler.
 *  \return [ 0 ].
 **************************************************************************/
static int hwMbox_socket_release(struct socket *sock)
{
    struct hwMbox_sock *sk = hwMbox_sk(sock->sk);

    if (sk)
    {
        /* unregister our callback function for SUBJECT-OPCODE protocol */
        if ( (SOCK_SUBJECT == sock->type) &&
             (hw_mbox_opcode == HW_MBOX_PROTOCOL_BY_APPID(sk->appid)) &&
             HW_MBOX_MID_VALIDITY_CHECK(sk->mid) )
        {
            DPRINTK_SKT(sock, "unregistering socket's recvOpcode callback...");

            if (hwMbox_unregisterRecvOpcode(sk->mid, sk->appid))
            {
                PRINTK_ERR("failed to unregister socket's recvOpcode callback for tag=%u from %s(%d)!",
                           sk->appid, HW_MBOX_MASTER_GET_NAME(sk->mid), sk->mid);
            }
        }

        if (down_interruptible(&hwMbox_sock_hlist_sem))
        {
            return -ERESTARTSYS;
        }

        hlist_del_init(&sock->sk->sk_node);

        up(&hwMbox_sock_hlist_sem);

        sock->sk = NULL;
        lock_sock((struct sock *)sk);
        release_sock((struct sock *)sk);
        sock_put((struct sock *)sk);
    }

    return 0;
}
#endif /* HW_MBOX_SOCKETS_API */

#ifdef HW_MBOX_SOCKETS_API
/************************************************************************/
/*     HW_MBOX socket API data                                          */
/************************************************************************/

static struct proto hwMbox_proto = {
    .name       = "HWMBOX",
    .owner      = THIS_MODULE,
    .obj_size   = sizeof(struct hwMbox_sock),
};

static const struct net_proto_family hwMbox_family_ops = {
    .family     = PF_HWMBOX,
    .create     = hwMbox_socket_create,
    .owner      = THIS_MODULE,
};

static const struct proto_ops hwMbox_proto_ops = {
    .family     = PF_HWMBOX,
    .owner      = THIS_MODULE,
    .release    = hwMbox_socket_release,
    .bind       = hwMbox_socket_bind,
    .sendmsg    = hwMbox_socket_sendmsg,
    .recvmsg    = hwMbox_socket_recvmsg,
    .connect    = sock_no_connect,
    .socketpair = sock_no_socketpair,
    .accept     = sock_no_accept,
    .getname    = sock_no_getname,
    .poll       = sock_no_poll,
    .ioctl      = sock_no_ioctl,
    .listen     = sock_no_listen,
    .shutdown   = sock_no_shutdown,
    .setsockopt = sock_no_setsockopt,
    .getsockopt = sock_no_getsockopt,
    .mmap       = sock_no_mmap,
    .sendpage   = sock_no_sendpage,
};
#endif /* HW_MBOX_SOCKETS_API */

subsys_initcall(hwMbox_drv_init);
module_exit(hwMbox_drv_exit);

/* Driver identification: */
#ifdef HW_MBOX_SOCKETS_API
MODULE_ALIAS_NETPROTO(PF_HWMBOX);
#endif /* HW_MBOX_SOCKETS_API */

MODULE_DESCRIPTION(DRV_NAME);
MODULE_VERSION(DRV_VERSION);
MODULE_AUTHOR ("Intel Corporation");
MODULE_LICENSE("GPL");
