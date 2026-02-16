/*
 *
 * aep_core.c
 * Description:
 * aep driver
 *
 *  GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2013-2014 Intel Corporation. All rights reserved.
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
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *  The full GNU General Public License is included in this distribution
 *  in the file called LICENSE.GPL.
 *
 *  Contact Information:
 *    Intel Corporation
 *    2200 Mission College Blvd.
 *    Santa Clara, CA  97052
 *
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/workqueue.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/string.h>
#include <linux/spinlock.h>
#include <linux/proc_fs_macros.h>
#include <puma6.h>
#include <sram_api.h>
#include <docsis_ip_boot_params.h>
#include <aep.h>

/******************************************************/
/** Local defines                                     */
/******************************************************/

#define DEVICE_NAME "AEP"
#define AEP_MAX_REG_DESCRIPTOR 50

/* AEP Base Address */
//#define AEP_BASE_ADDRESS                             IO_ADDRESS(0x00000000)
#define AEP_BASE_ADDRESS                             AVALANCHE_AEP_BASE

/* AEP Registers */
#define AEP_REG_OFFSET_PV_CONTROL                    (AEP_BASE_ADDRESS + 0x0000)
#define AEP_REG_OFFSET_ICACHE_BASE_ADDRESS           (AEP_BASE_ADDRESS + 0x0004)
#define AEP_REG_OFFSET_WATCHDOG_TIMER                (AEP_BASE_ADDRESS + 0x0008)
#define AEP_REG_OFFSET_CACHELINE_INVALIDATE          (AEP_BASE_ADDRESS + 0x0010)
#define AEP_REG_OFFSET_PV_SLAVE_SECURITY_ATTR        (AEP_BASE_ADDRESS + 0x0020)
#define AEP_REG_OFFSET_PV_MASTER_SYS_SECURITY        (AEP_BASE_ADDRESS + 0x0024)
#define AEP_REG_OFFSET_PV_MASTER_ATOM_SECURITY       (AEP_BASE_ADDRESS + 0x0028)
#define AEP_REG_OFFSET_PV_MASTER_NP_SECURITY         (AEP_BASE_ADDRESS + 0x002C)
#define AEP_REG_OFFSET_PV_TMR_VALUE0                 (AEP_BASE_ADDRESS + 0x0030)
#define AEP_REG_OFFSET_PV_TMR_LOAD0                  (AEP_BASE_ADDRESS + 0x0034)
#define AEP_REG_OFFSET_PV_TMR_CONTROL0               (AEP_BASE_ADDRESS + 0x0038)
#define AEP_REG_OFFSET_PV_TMR_VALUE1                 (AEP_BASE_ADDRESS + 0x003C)
#define AEP_REG_OFFSET_PV_TMR_LOAD1                  (AEP_BASE_ADDRESS + 0x0040)
#define AEP_REG_OFFSET_PV_TMR_CONTROL1               (AEP_BASE_ADDRESS + 0x0044)
#define AEP_REG_OFFSET_PV_TMR_SCALAR                 (AEP_BASE_ADDRESS + 0x0048)
#define AEP_REG_OFFSET_PV_TMR_SCALAR_PRESET          (AEP_BASE_ADDRESS + 0x004C)
#define AEP_REG_OFFSET_SYS_FUSE_DISABLE              (AEP_BASE_ADDRESS + 0x0068)
#define AEP_REG_OFFSET_PV_MASTERS_ATTR_SEL           (AEP_BASE_ADDRESS + 0x006C)
#define AEP_REG_OFFSET_EXT_EVENT_ENABLE              (AEP_BASE_ADDRESS + 0x0070)
#define AEP_REG_OFFSET_EXT_EVENT_STATUS              (AEP_BASE_ADDRESS + 0x0074)
#define AEP_REG_OFFSET_PROXY_MODE                    (AEP_BASE_ADDRESS + 0x0078)
#define AEP_REG_OFFSET_EXT_INPUT_EVENT_STATUS        (AEP_BASE_ADDRESS + 0x0084)
#define AEP_REG_OFFSET_PV_SEC_STATUS                 (AEP_BASE_ADDRESS + 0x00A0)
#define AEP_REG_OFFSET_ATOM_EMMC_BASE1               (AEP_BASE_ADDRESS + 0x1000)
#define AEP_REG_OFFSET_ATOM_MAILBOX                  (AEP_BASE_ADDRESS + 0x1280)
#define AEP_REG_OFFSET_ATOM_PV_MAILBOX               (AEP_BASE_ADDRESS + 0x12C0)
#define AEP_REG_OFFSET_ATOM_DOORBELL                 (AEP_BASE_ADDRESS + 0x1400)
#define AEP_REG_OFFSET_ATOM_IPC_STATUS               (AEP_BASE_ADDRESS + 0x1404)
#define AEP_REG_OFFSET_ATOM_PV_DOORBELL              (AEP_BASE_ADDRESS + 0x1408)
#define AEP_REG_OFFSET_ATOM_PV_IPC_STATUS            (AEP_BASE_ADDRESS + 0x140C)
#define AEP_REG_OFFSET_PV2ATOM_SW_INT                (AEP_BASE_ADDRESS + 0x1410)
#define AEP_REG_OFFSET_NP_EMMC_BASE2                 (AEP_BASE_ADDRESS + 0x1800)
#define AEP_REG_OFFSET_NP_MAILBOX                    (AEP_BASE_ADDRESS + 0x1A80)  
#define AEP_REG_OFFSET_NP_PV_MAILBOX                 (AEP_BASE_ADDRESS + 0x1AC0)  /* On Send, write by NP, read by AEP. */
#define AEP_REG_OFFSET_NP_DOORBELL                   (AEP_BASE_ADDRESS + 0x1C00)  /* write by AEP, read by NP, */
#define AEP_REG_OFFSET_NP_IPC_STATUS                 (AEP_BASE_ADDRESS + 0x1C04)  /* read by AEP, (NP write 'done') */
#define AEP_REG_OFFSET_NP_PV_DOORBELL                (AEP_BASE_ADDRESS + 0x1C08)  /* write by NP (auto clear 'ready' and 'done' in NP_PV_IPC_STATUS) , read by AEP (auto set ready in NP_PV_IPC_STATUS) */
#define AEP_REG_OFFSET_NP_PV_IPC_STATUS              (AEP_BASE_ADDRESS + 0x1C0C)  /* read by NP  (AEP write 1 to 'done') */
#define AEP_REG_OFFSET_PV2NP_SW_INT                  (AEP_BASE_ADDRESS + 0x1C10)


#define AEP_NP_MAILBOX_SIZE              0x40
#define AEP_IPC_STATUS_READY_MASK        0x00000001
#define AEP_IPC_STATUS_DONE_MASK         0x00000002
#define AEP_IPC_1_SEC_IN_US              1000000   /* 1 sec timeout */
#define AEP_IPC_CLIENT_STATE_FREE       0 
#define AEP_IPC_CLIENT_STATE_WAIT_RSP   1 

/* Read and Write resgister macros */
#define reg_write_32(addr, data) (( *(volatile unsigned int *) (addr) ) = (data))
#define reg_read_32(addr)        ( *(volatile unsigned int *) (addr) )

/******************************************************/
/** Local type definitions                            */
/******************************************************/


/* IPC message structure for AEP_IPC_CMD_ID_AEP_FW_VERSION command output payload*/
typedef struct _fw_version_output_payload_t
{
  unsigned int status;
  unsigned int aep_fw_version;
  unsigned int punit_fw_build;
  unsigned int punit_fw_version;
} fw_version_output_payload_t;

/* structure for registers test descriptors */
typedef struct _reg_descriptor_t
{
  unsigned int offset;
  char  name[AEP_MAX_REG_DESCRIPTOR];
} reg_descriptor_t;

/* Structure that hold inframtion on IPC client drivers */
typedef struct _aep_ipc_client_t
{
    aep_recv_message_cb_t cb;   /* Callback function for AEP IPC messages */
    int msg_size;               /* Expected message length in bytes */
    int rsp_size;               /* Expected response message length in bytes */
    int state;
}aep_ipc_client_t;


/******************************************************/
/** Local functions prototypes                        */
/******************************************************/
static int aep_send_command(unsigned int cmd_id, int* payload, unsigned int length);
static int aep_recv_command(int* payload, int length);
static int aep_wait_for_complete(unsigned int cmd_id, unsigned int timeout_in_seconds);
static irqreturn_t aep_ipc_isr(int irq, void *dev_id);
static void aep_workq_func(struct work_struct *work);
static int get_fw_version(char *fw_ver_soc, char *fw_ver_stepping, char *fw_ver_major ,char *fw_ver_minor);
static int aep_proc_dump_fw_version (struct seq_file *m, void *v);
static void dump_regs(void);

/******************************************************/
/** global variables                                  */
/******************************************************/


/* Gloabl registers text descriptors table */
static reg_descriptor_t g_aep_regs[] = {
    {AEP_REG_OFFSET_PV_CONTROL,             "PV Control"},
    {AEP_REG_OFFSET_ICACHE_BASE_ADDRESS,    "ICache base address"},      
    {AEP_REG_OFFSET_WATCHDOG_TIMER,         "Watch-Dog timer"},                 
    {AEP_REG_OFFSET_CACHELINE_INVALIDATE,   "Cacheline invalidate"},     
    {AEP_REG_OFFSET_PV_SLAVE_SECURITY_ATTR, "PV Slave Security Attributes"}, 
    {AEP_REG_OFFSET_PV_MASTER_SYS_SECURITY, "PV Master SYS Security"}, 
    {AEP_REG_OFFSET_PV_MASTER_ATOM_SECURITY,"PV Master ATOM Security"}, 
    {AEP_REG_OFFSET_PV_MASTER_NP_SECURITY,  "PV Master NP Security"}, 
    {AEP_REG_OFFSET_PV_TMR_VALUE0,          "PV GP Timer0 Value"}, 
    {AEP_REG_OFFSET_PV_TMR_LOAD0,           "PV GP Timer0 Load"}, 
    {AEP_REG_OFFSET_PV_TMR_CONTROL0,        "PV GP Timer0 Control"}, 
    {AEP_REG_OFFSET_PV_TMR_VALUE1,          "PV GP Timer1 Value"}, 
    {AEP_REG_OFFSET_PV_TMR_LOAD1,           "PV GP Timer1 Load"}, 
    {AEP_REG_OFFSET_PV_TMR_CONTROL1,        "PV GP Timer1 control"}, 
    {AEP_REG_OFFSET_PV_TMR_SCALAR,          "PV GP Timer Pre-Scale value"}, 
    {AEP_REG_OFFSET_PV_TMR_SCALAR_PRESET,   "PV GP Timer Pre-Scale preset"}, 
    {AEP_REG_OFFSET_SYS_FUSE_DISABLE,       "System Fuse Disable"}, 
    {AEP_REG_OFFSET_PV_MASTERS_ATTR_SEL,    "PV Master Attributes Select"}, 
    {AEP_REG_OFFSET_EXT_EVENT_ENABLE,       "External event interrupt enable "}, 
    {AEP_REG_OFFSET_EXT_EVENT_STATUS,       "External event status "}, 
    {AEP_REG_OFFSET_PROXY_MODE,             "PROXY enabled indication"}, 
    {AEP_REG_OFFSET_EXT_INPUT_EVENT_STATUS, "External event input signal status "}, 
    {AEP_REG_OFFSET_PV_SEC_STATUS,          "PV Security status"}, 
    {AEP_REG_OFFSET_ATOM_EMMC_BASE1,        "eMMC1 Emulation Register base address"}, 
    {AEP_REG_OFFSET_ATOM_MAILBOX,           "ATOM Mailbox"}, 
    {AEP_REG_OFFSET_ATOM_PV_MAILBOX,        "ATOM PV Mailbox"}, 
    {AEP_REG_OFFSET_ATOM_DOORBELL,          "ATOM Doorbell"}, 
    {AEP_REG_OFFSET_ATOM_IPC_STATUS,        "ATOM IPC Status"}, 
    {AEP_REG_OFFSET_ATOM_PV_DOORBELL,       "ATOM PV Doorbell"},
    {AEP_REG_OFFSET_ATOM_PV_IPC_STATUS,     "ATOM PV IPC Status"}, 
    {AEP_REG_OFFSET_PV2ATOM_SW_INT,         "PV 2 ATOM SW interrupt trigger"}, 
    {AEP_REG_OFFSET_NP_EMMC_BASE2,          "eMMC2 Emulation Register base address"}, 
    {AEP_REG_OFFSET_NP_MAILBOX,             "NP Mailbox"}, 
    {AEP_REG_OFFSET_NP_PV_MAILBOX,          "NP PV Mailbox"}, 
    {AEP_REG_OFFSET_NP_DOORBELL,            "NP Doorbell"}, 
    {AEP_REG_OFFSET_NP_IPC_STATUS,          "NP IPC Status"}, 
    {AEP_REG_OFFSET_NP_PV_DOORBELL,         "NP PV Doorbell"}, 
    {AEP_REG_OFFSET_NP_PV_IPC_STATUS,       "NP PV IPC Status"}, 
    {AEP_REG_OFFSET_PV2NP_SW_INT,           "PV 2 NP SW interrupt trigger"}, 
};

#define AEP_REGS_ARRAY_SIZE  (sizeof(g_aep_regs)/sizeof(reg_descriptor_t))


static int                      g_aep_active = 0;                      /* main ACTIVE flag*/
static int                      g_workq_command_id    = 0;             /* Keep last receive command id */
static unsigned int             g_timeout_in_seconds = 0;              /* Set AEP response timeout */
static aep_ipc_client_t         g_ipc_client[AEP_IPC_CMD_ID_LAST];     /* Keep callback's for each command id */ 
static struct workqueue_struct  *g_work_queue;                         /* Work Queue structure*/ 
static struct work_struct       g_work;                                /* A 'work' in a queue */
static DEFINE_SPINLOCK(g_lock);     /* Global spin lock protection */

/* Interrupts:
   When we write to the doorbell register:
    it generate intrrupt on the other cpu.
    it clear the Ready and Done bit.
   When we read from the doorbell register:
    it clear the interrupt
    it set the Ready bit on other cpu
*/


/******************************************************/
/** Driver API functions                              */
/******************************************************/


/* 
 * Get AEP F/W Status.
 * 
 * int aep_is_active(void)
 *
 * Description:
 *  This API is used to check if the AEP F/W is exist and running.
 *  On A0 or B0 parts there is no AEP IP. The API will return 0
 *  On C0 part and above the AEP can be active or not active, depend on the product design.  
 *
 * Precondition:
 *  This API can be call with out any precondition.
 *
 * Parameters:
 *  None.
 *
 * Return:
 *  0 on Not Active
 *  1 on Active.
 */
int aep_is_active(void)
{
    return g_aep_active;
}
EXPORT_SYMBOL(aep_is_active);

/* 
 * Get AEP F/W Version.
 * 
 * int aep_get_aep_fw_version(void)
 *
 * Description:
 *  This API is used to read the AEP F/W version 
 *
 * Precondition:
 *  This API can be only if the AEP is active.
 *
 * Parameters:
 *  None.
 *
 * Return:
 *  AEP F/W version
 */
int aep_get_aep_fw_version(char *fw_ver_soc, char *fw_ver_stepping, char *fw_ver_major ,char *fw_ver_minor)
{
    char ver_minor     = 0;
    char ver_major     = 0;
    char ver_stepping  = 0;
    char ver_soc       = 0;

    /* Check if the AEP is active */
    if (!g_aep_active) 
    {
        printk(KERN_ERR "AEP: AEP is not loaded\n");
        return -1;
    }

    /* Send IPC mesage - get FW version */
    get_fw_version(&ver_soc,&ver_stepping,&ver_major,&ver_minor);

    /* Return AEP F/W version */
    if (fw_ver_soc      != NULL) *fw_ver_soc = ver_soc;
    if (fw_ver_stepping != NULL) *fw_ver_soc = ver_stepping;
    if (fw_ver_major    != NULL) *fw_ver_soc = ver_major;
    if (fw_ver_minor    != NULL) *fw_ver_soc = ver_minor;

    return 0;
}
EXPORT_SYMBOL(aep_get_aep_fw_version);

/* 
 * Clear MMC interrupt.
 * 
 * int aep_clear_mmc_interrupt(void)
 *
 * Description:
 *  Clear AEP interrupt, once the  driver get interrupt from the AEP, it must clear the interrupt register 
 *
 * Precondition:
 *  This API can be only if the AEP is active.
 *
 * Parameters:
 *  None.
 *
 * Return:
 *  0 on Success, <0 on error.
 */
int aep_clear_mmc_interrupt(void)
{
    volatile unsigned int reg;

    /* Check if the AEP is active */
    if (!g_aep_active) 
    {
        printk(KERN_ERR "AEP: AEP is not loaded\n");
        return -1;
    }

    // read  
    reg = *(volatile unsigned int *) (AEP_REG_OFFSET_PV2NP_SW_INT);
    // mask out interrupt bits
    reg &= 0xEAFFFFFF;
    // set eMMC interrupt (set to clear)
    reg |= 0x01000000;
    //write back
    ( *(volatile unsigned int *) (AEP_REG_OFFSET_PV2NP_SW_INT) ) = reg;

    return 0;
}
EXPORT_SYMBOL(aep_clear_mmc_interrupt);


/* 
 * Send Sync IPC Message
 * 
 * int aep_ipc_send_message(unsigned int cmd_id, int* message, int msg_length, int* response, int res_length)
 *
 * Description:
 *  This API is used to send sync AEP IPC message.
 *  The function wait for response message from the AEP before it return.
 *
 * Precondition:
 *  This API can be only if the AEP is active.
 *
 * Parameters:
 *  cmd_id      - AEP IPC Command ID to send.
 *  message     - AEP IPC Command Payload (must be 4 bytes aligned).
 *  msg_length  - AEP IPC Command Payload length
 *  response    - [output] pointer to AEP IPC Response Payload (must be 4 bytes aligned)
 *  res_length  - Max size of AEP IPC Response Payload buffer (expected size). 
 *
 * Return:
 *  0 on Success, <0 on error.
 */
int aep_ipc_send_message(unsigned int cmd_id, int* message, int msg_length, int* response, int res_length)
{
    unsigned long irq_flags;
    // printk("AEP: [debug] aep_ipc_send_message(cmd_id=%d, message=0x%.8X, msg_length=%d, response=0x%.8X, res_length=%d\n",cmd_id,message,msg_length,response,res_length);

    if (!g_aep_active) 
    {
        printk(KERN_ERR "AEP: AEP is not loaded\n");
        return -1;
    }

    /* Take spin lock - disable kernel preemption and disable interrupts */
    //printk("AEP: [debug] aep_ipc_send_message - Take spin lock\n");
    spin_lock_irqsave(&g_lock, irq_flags);
    
    /* Send message */
    //printk("AEP: [debug] aep_ipc_send_message - Send message\n");
    if (aep_send_command(cmd_id,message,msg_length) != 0)
    {
        printk(KERN_ERR "AEP: Error - aep_send_command(cmd_id = 0x%X) failed\n",cmd_id);
        spin_unlock_irqrestore(&g_lock, irq_flags);
        return -1;
    }

    /* Wait for complete event */
    //printk("AEP: [debug] aep_ipc_send_message - Wait for complete event\n");
    if (aep_wait_for_complete(cmd_id, g_timeout_in_seconds) != 0)
    {
        printk(KERN_ERR "AEP: Error - aep_wait_for_complete failed \n");
        spin_unlock_irqrestore(&g_lock, irq_flags);
        return -1;
    }

    //if ((g_workq_command_id & (~AEP_IPC_CMD_ID_COMPLETE_FLAG)) != cmd_id) 
    //{
    //    printk(KERN_ERR "AEP: Error got unexpected response type (0x%X) - expected to (0x%X)\n",g_workq_command_id,cmd_id);
    //}

    /* Read the response payload from the mailbox. and set 'Done' bit */
    //printk("AEP: [debug] aep_ipc_send_message - Read the response payload from the mailbox. and set 'Done' bit\n");
    if (aep_recv_command(response,res_length) != 0 )
    {
        printk(KERN_ERR "AEP: Error - aep_recv_command failed\n");
        spin_unlock_irqrestore(&g_lock, irq_flags);
        return -1;
    }

    /* Release spin lock */
    //printk("AEP: [debug] aep_ipc_send_message - Release spin lock\n");
    spin_unlock_irqrestore(&g_lock, irq_flags);
    

    return 0;
}
EXPORT_SYMBOL(aep_ipc_send_message);


/* 
 * Register Receive Callback function
 * 
 * int aep_ipc_register_event_recv(unsigned int cmd_id,aep_recv_message_cb_t cb)
 *
 * Description:
 *  This API is used to register callback function for specific IPC command ID.
 *
 * Precondition:
 *  This API can be only if the AEP is active.
 *
 * Parameters:
 *  cmd_id           - AEP IPC Command ID to register.
 *  cmd_message_size - AEP IPC Command expected message size.
 *  cb               - Pointer to the call back function.
 *
 * Return:
 *  0 on Success, <0 on error.
 */
int aep_ipc_register_event_recv(unsigned int cmd_id, unsigned int cmd_message_size, aep_recv_message_cb_t cb)
{
    if (!g_aep_active) 
    {
        printk(KERN_ERR "AEP: AEP is not loaded\n");
        return -1;
    }

    /* register the function pointer */
    if (cmd_id >= AEP_IPC_CMD_ID_LAST)
    {
        printk(KERN_ERR "AEP: register IPC Event callback for cmd id #%d failed, illigal command id.\n",cmd_id);
        return -1;
    }

    if (cmd_message_size > AEP_NP_MAILBOX_SIZE)
    {
        printk(KERN_ERR "AEP: register IPC Event callback for cmd id #%d failed, message size (%d) it too big\n",cmd_id,cmd_message_size);
        return -1;
    }

    /* Set the call back function */
    g_ipc_client[cmd_id].cb = cb;

    /* Set the message size */
    g_ipc_client[cmd_id].msg_size = cmd_message_size;
    
    return 0;
}
EXPORT_SYMBOL(aep_ipc_register_event_recv);

/* 
 * Send IPC response message
 * 
 * int aep_ipc_send_response(unsigned int cmd_id, int* response, int res_length)
 *
 * Description:
 *  This API is used to send a response message, after the AEP F/W call the callback function, 
 *  with a new AEP command message.
 *
 * Precondition:
 *  This API can be only if the AEP is active.
 *
 * Parameters:
 *  cmd_id      - AEP IPC Command ID to response.
 *  response    - AEP IPC Command Payload to response.
 *  res_length  - Playload length in bytes.
 *
 * Return:
 *  0 on Success, <0 on error.
 */
int aep_ipc_send_response(unsigned int cmd_id, int* response, int res_length)
{
    // printk("AEP: [debug] aep_ipc_send_response(cmd_id=%d, response=0x%.8X, res_length=%d\n",cmd_id,response,res_length);

    if (!g_aep_active) 
    {
        printk(KERN_ERR "AEP: Error - AEP is not loaded\n");
        return -1;
    }

    if (g_ipc_client[cmd_id].state !=  AEP_IPC_CLIENT_STATE_WAIT_RSP) 
    {
        printk(KERN_ERR "AEP: Error - Not expecting to send 'Command Complete' for command id %d\n",cmd_id);
        return -1;
    }

    /* Set State as "not waiting to response - free" */
    g_ipc_client[cmd_id].state =  AEP_IPC_CLIENT_STATE_FREE;

    /* Write payload to mailbox and send 'Complete' response */
    if (aep_send_command(AEP_IPC_CMD_ID_COMPLETE_FLAG|cmd_id,response,res_length) != 0)
    {
        printk(KERN_ERR "AEP: Error - aep_send_command(cmd_id = 0x%X) failed */\n",AEP_IPC_CMD_ID_COMPLETE_FLAG|cmd_id);
        return -1;
    }
    return 0; 
}
EXPORT_SYMBOL(aep_ipc_send_response);


/******************************************************/
/** Local functions                                   */
/******************************************************/

/* The 'Ready' bit is 'set' immediately when the other CPU read from his doorbell register. */
/* The 'Done' bit is 'set' when the other CPU, write '1' to his status register */
/* Both 'Done' and 'Ready' bits are cleared when we set new message in the doorbell reggister */
/* Both 'Done' and 'Ready' are 'set' by reset */


/* 
 * Send IPC message 
 * 
 * int aep_send_command(unsigned int cmd_id, int* payload, unsigned int length)
 *
 * Description:
 *  This function is used to send IPC message.
 *
 * Precondition:
 *  This API can be only if the AEP is active.
 *
 * Parameters:
 *  cmd_id      - AEP IPC Command ID to send.
 *  payload     - AEP IPC Command payload to send.
 *  length      - Payload length in bytes.
 *
 * Return:
 *  0 on Success, <0 on error.
 */
static int aep_send_command(unsigned int cmd_id, int* payload, unsigned int length)
{
    unsigned int *offset_start;
    unsigned int *offset_end;
    unsigned int mask = 0; 
    unsigned int timeout = 0;

    // printk("AEP: [debug] aep_send_command (cmd_id=%d, payload=0x%.8X, length=%d)\n",cmd_id,payload,length);


    /* validate input params */
    if (length > AEP_NP_MAILBOX_SIZE)
    {
        printk(KERN_ERR "AEP: Error - aep_send_command() failed - length (%d) is too long\n", length);
        return -1;
    }

    /* wait for 'ready' and 'done' */
    mask = (AEP_IPC_STATUS_READY_MASK | AEP_IPC_STATUS_DONE_MASK);
    mask = cpu_to_le32(mask); // convert mask to Little Endian
    while( ( reg_read_32(AEP_REG_OFFSET_NP_PV_IPC_STATUS) & mask ) != mask )
    {
        udelay(1); 
        if (timeout >= AEP_IPC_1_SEC_IN_US) 
        {
            printk(KERN_EMERG "AEP: Fatal Error - aep_send_command() failed on timeout, PV's 'Ready' and 'Done' never set\n");
            dump_regs();
            g_aep_active = 0;
            return -1;
        }
        timeout++;
    }
  
    /* Copy the message payload to mailbox*/
    if ((payload != NULL) && (length > 0))
    {
        /* Convert to length of "int", divide by 4 (by sizeof int) */
        length = (length + 3) / sizeof(int); 
        // printk("AEP: [debug] aep_send_command: set length to %d\n",length);

        /* Set start and end offsets */
        offset_start = (unsigned int *)AEP_REG_OFFSET_NP_PV_MAILBOX;
        offset_end   = offset_start + length;

        // printk("AEP: [debug] aep_send_command: offset_start=0x%.8X, offset_end=0x%.8X\n",offset_start,offset_end);

        /* Copy loop */
        while(offset_start < offset_end)
        {
            // printk("AEP: [debug] aep_send_command: reg_write_32(offset_start=0x%.8X, *payload=0x%.8X)\n",offset_start,*payload);
            reg_write_32(offset_start, *payload);
            offset_start++;
            payload++;
        }
    }

    /* Send the command */
    reg_write_32(AEP_REG_OFFSET_NP_PV_DOORBELL, cpu_to_le32(cmd_id));   /* Clear the 'Ready' and 'Done' bits on NP PV STATUS register, and trigger interrupt on AEP F/W */


    return 0;
}

/* 
 * Recv IPC message 
 * 
 * int aep_recv_command(int* payload, int length)
 *
 * Description:
 *  This function is used to read IPC message from mail box
 *
 * Precondition:
 *  This API can be only if the AEP is active.
 *
 * Parameters:
 *  payload     - AEP IPC Command payload to recv.
 *  length      - Max payload length in bytes.
 *
 * Return:
 *  0 on Success, <0 on error.
 */
static int aep_recv_command(int* payload, int length) 
{

    unsigned int *offset_start;
    unsigned int *offset_end;
    int int_length = 0;
    int msg[AEP_NP_MAILBOX_SIZE/ sizeof(int)]; /* temp buffer */
    int i=0;

    // printk("AEP: [debug] aep_recv_command (payload=0x%.8X, length=%d)\n",payload,length);

    /* Note: reading from register must be done in 4 bytes size,
       Payload buffer, does not have too be align to 4 bytes size, for example it can by only two byte.
       In such a case we must read 4 bytes to a temp buffer, and then copy only 2 bytes from the temp buffer
       to the payload buffer */
 
    /* Copy the message from mailbox to temp buffer */
    if ((payload != NULL) && (length > 0))
    {
        /* Convert to length of "int", divide by sizeof int */
        int_length = (length + 3) / sizeof(int);  
        // printk("AEP: [debug] aep_recv_command: set int_length to %d\n",int_length);

        /* Set start and end offsets */
        offset_start = (unsigned int *)AEP_REG_OFFSET_NP_MAILBOX;
        offset_end   = offset_start + int_length;
        // printk("AEP: [debug] aep_recv_command: offset_start=0x%.8X, offset_end=0x%.8X\n",offset_start,offset_end);

        /* Copy loop */
        while(offset_start < offset_end)
        {
            msg[i] = reg_read_32(offset_start);
            // printk("AEP: [debug] aep_recv_command: reg_read_32(offset_start=0x%.8X) = *payload=0x%.8X\n",offset_start,*payload);
            offset_start++;
            i++;
        }

        /* Copy from temp buffer to payload buffer (output buffer) */
        // printk("AEP: [debug] aep_recv_command: Copy from temp buffer to payload buffer (output buffer)\n");
        memcpy((void*)payload,(void*)msg,length);
    }

    /* Set 'Done' bit */
    // printk("AEP: [debug] aep_recv_command:  Set 'Done' bit\n");
    reg_write_32(AEP_REG_OFFSET_NP_IPC_STATUS, cpu_to_le32(AEP_IPC_STATUS_DONE_MASK)); 

    return 0;
}

/* 
 * Wait for comlpete 
 * 
 * int aep_wait_for_complete(unsigned int cmd_id, unsigned int timeout_in_seconds)
 *
 * Description:
 *  This function is used wait until AEP F/W is finish to handle the send message
 *  When the function return, it is safe to read the response from the mailbox
 *
 * Precondition:
 *  This API can be only if the AEP is active.
 *
 * Parameters:
 *  timeout_in_jiffies     - Max waiting timeout.
 *
 * Return:
 *  0 on Success, <0 on error.
 */
static int aep_wait_for_complete(unsigned int cmd_id, unsigned int timeout_in_seconds)
{
    unsigned int mask = (AEP_IPC_STATUS_READY_MASK | AEP_IPC_STATUS_DONE_MASK); 
    unsigned int timeout = 0;
    unsigned int cmd_id_read;
    
    mask = cpu_to_le32(mask); // convert mask to Little Endian
    
    /* wait for ARM to be 'not ready' and 'not done', indicate that a new command is waiting to be read in the doorbell */
    while ((reg_read_32(AEP_REG_OFFSET_NP_IPC_STATUS) & mask) != 0)
    {
        udelay(1); 
        if (timeout >= AEP_IPC_1_SEC_IN_US) 
        {
            printk(KERN_ERR "AEP: Error - aep_wait_for_complete() failed on timeout, NP 'Ready' and 'Done' never assert (set to zero)\n");
            dump_regs();
            if (!timeout_in_seconds--)
            {
                printk(KERN_EMERG "AEP: Fatal Error - aep_wait_for_complete() failed on timeout, NP 'Ready' and 'Done' never assert (set to zero)\n");
                g_aep_active = 0;
                return -1;
            }
            else
            {
                printk(KERN_ERR "AEP: Error - aep_wait_for_complete() timeout counter %d (seconds)\n",timeout_in_seconds);
                timeout = 0; 
            }
        }
        timeout++;
    }

    /* Read command id */
    cmd_id_read = reg_read_32(AEP_REG_OFFSET_NP_DOORBELL); /* This de-assert the 'Ready' bit (set to '1') on AEP_REG_OFFSET_NP_IPC_STATUS */
                                                           /* By design we will not get new interrupt until we de-assert the 'Done' bit */

    cmd_id_read = le32_to_cpu(cmd_id_read);    // convert from Little Endian

    // printk("AEP: [debug] get interrupt (polling) g_workq_command_id = 0x%.8X (complete=%d)\n",g_workq_command_id&0x7FFFFFFF,((g_workq_command_id&0x80000000) != 0));
    

    /* if the interrrupt a response (Compelte flag is set in command id), 
       then just signal to the waiting thread */
    if ((cmd_id_read & AEP_IPC_CMD_ID_COMPLETE_FLAG) == 0) 
    {
        /* this is a "Complete Command" response message */
        printk("AEP: Error - Got response command without 'Complete' bit (cmd=0x%.8X)\n",cmd_id_read);
        return -1;
    }

    if ((cmd_id_read & (~AEP_IPC_CMD_ID_COMPLETE_FLAG)) != cmd_id) 
    {
        printk(KERN_ERR "AEP: Error - Got unexpected response type (0x%X) - expected to (0x%X)\n",cmd_id_read,cmd_id);
        return -1;
    }


    return 0;
}




/* 
 * Interrupt service routine 
 * 
 * irqreturn_t aep_ipc_isr(int irq, void *dev_id)
 *
 * Description:
 * Interrupt happen when the AEP F/W set new "Command" on the dooebell register (AEP_REG_OFFSET_NP_DOORBELL) 
 * By design, it is a sure that the AEP F/W will not send new command (and we will not get new interrupt),
 * until we set the 'Done' bit in the status register 
 *
 * Precondition:
 *  This function called in Interrupt Context
 *
 * Parameters:
 *  irq    - interrupt number
 *  dev_id - device private date
 *
 * Return:
 *  IRQ_HANDLED.
 */
static irqreturn_t aep_ipc_isr(int irq, void *dev_id)
{
    /* Read command id */
    g_workq_command_id = reg_read_32(AEP_REG_OFFSET_NP_DOORBELL); /* This de-assert the 'Ready' bit on AEP_REG_OFFSET_NP_IPC_STATUS */
                                                                  /* By design we will not get new interrupt until we de-assert the 'Done' bit */

    g_workq_command_id = le32_to_cpu(g_workq_command_id);    // convert to Little Endian

    //printk("AEP: [debug] get interrupt(irq=%d) g_workq_command_id = 0x%.8X (complete=%d)\n",irq, g_workq_command_id&0x7FFFFFFF,((g_workq_command_id&0x80000000) != 0));
    

    /* if the interrrupt a response (Compelte flag is set in command id), 
       then just signal to the waiting thread */
    if (g_workq_command_id & AEP_IPC_CMD_ID_COMPLETE_FLAG) 
    {
        /* this is a "Complete Command" response message */
        printk("AEP: [debug] get interrupt\n");
    }
    /* if he interrrupt a new request, then add a work to the workqueue */
    else
    {
        /* this is a new request command from AEP - queue a work */
        queue_work(g_work_queue, &g_work);
    }
    
    return IRQ_HANDLED;
}


/* 
 * Work Queue worker function.
 * 
 * void aep_workq_func(struct work_struct *work)
 *
 * Description:
 * This is a work queue worker function, called when ever there is new AEP IPC message
 * to proccess.
 *
 * Precondition:
 *  This function called in Work Queue Context
 *
 * Parameters:
 *  work_struct     - The calling WQ structure.
 *
 * Return:
 *  none.
 */
static void aep_workq_func(struct work_struct *work)
{
    char msg[AEP_NP_MAILBOX_SIZE];
    char rsp[AEP_NP_MAILBOX_SIZE];
    unsigned int cmd_id  = 0;  
    int has_rsp = 1;

    /* Store the Command ID */
    /* Note: we need to store g_workq_command_id locally, 
       becasue after we set the 'Done' Bit, we can get new interrupt (that will over rigth the g_workq_command_id) */
    cmd_id  = g_workq_command_id;

    if (cmd_id >= AEP_IPC_CMD_ID_LAST) 
    {
        printk(KERN_ERR "AEP: Error - Illegal command id %d\n",cmd_id);
        return;
    }

    /* Read the payload from the mailbox, and set 'Done' bit */
    if (aep_recv_command((int*)msg,g_ipc_client[cmd_id].msg_size) != 0)
    {
        printk(KERN_ERR "AEP: Error - aep_recv_command failed\n");
        return;
    }

    /* Proccess the response - call the callback function */
    if (g_ipc_client[cmd_id].cb != NULL)
    {
        /* Set State as "Wait to response", and call the callback function */
        g_ipc_client[cmd_id].state =  AEP_IPC_CLIENT_STATE_WAIT_RSP;
        g_ipc_client[cmd_id].cb(g_workq_command_id,msg,rsp,AEP_NP_MAILBOX_SIZE,&has_rsp);
    }
    else
    {
       printk(KERN_WARNING "AEP: Warnning - aep_workq_func() no call back is set to command id %d\n",cmd_id);
    }

    /* Response now or later ? */
    if (has_rsp == 1) 
    {
        /* Set State as "not waiting to response - free" */
        g_ipc_client[cmd_id].state =  AEP_IPC_CLIENT_STATE_FREE;

        /* Write payload to mailbox and send 'Complete' response */
        if (aep_send_command(AEP_IPC_CMD_ID_COMPLETE_FLAG|cmd_id,(int*)rsp,AEP_NP_MAILBOX_SIZE) != 0)
        {
            printk(KERN_ERR "AEP: Error - aep_send_command(cmd_id = %d) failed\n",AEP_IPC_CMD_ID_COMPLETE_FLAG|g_workq_command_id);
            return;
        }
    }

    return;
}


/* Send IPC message to get AEP FW verasion */
static int get_fw_version(char *fw_ver_soc, char *fw_ver_stepping, char *fw_ver_major ,char *fw_ver_minor)
{
    fw_version_output_payload_t  fw_version = {0};

    /* Read FW version into global variable*/
    if (aep_ipc_send_message(AEP_IPC_CMD_ID_AEP_FW_VERSION,NULL,0,(int*)&fw_version,sizeof(fw_version_output_payload_t)) != 0)
    {
        printk(KERN_ERR "AEP: Failed to send message (Read AEP F/W). AEP is not loaded */\n");
        return 1;
    }
    /* Check response status */
    if (le32_to_cpu(fw_version.status) != 0) 
    {
        printk(KERN_ERR "AEP: Failed to read FW version. AEP is not loaded */\n");
        return 1;
    }

    fw_version.aep_fw_version = le32_to_cpu(fw_version.aep_fw_version);

    /* Print AEP F/W version */
    *fw_ver_minor    = ((fw_version.aep_fw_version >> 0 )&0xFF);
    *fw_ver_major    = ((fw_version.aep_fw_version >> 8 )&0xFF);
    *fw_ver_stepping = ((fw_version.aep_fw_version >> 16)&0xFF);
    *fw_ver_soc      = ((fw_version.aep_fw_version >> 24)&0xFF); 
    
    return 0;
}

static int aep_proc_status (struct seq_file *m, void *v)
{
    seq_printf(m, "%s\n", ((g_aep_active!=0)?"active":"not active"));
    return 0;
}

static int aep_proc_dump_fw_version (struct seq_file *m, void *v)
{
    char fw_ver_minor     = 0;
    char fw_ver_major     = 0;
    char fw_ver_stepping  = 0;
    char fw_ver_soc       = 0;

    /* Send IPC mesage - get FW version */
    get_fw_version(&fw_ver_soc,&fw_ver_stepping,&fw_ver_major,&fw_ver_minor);

    seq_printf(m, "AEP FW version = %d.%d.%d.%d\n",fw_ver_soc,fw_ver_stepping,fw_ver_major,fw_ver_minor);
    return 0;
}
/* off    - offset in file */
/* count  - page size */
static int aep_proc_dump (struct seq_file *m, void *v)
{
    char dots[AEP_MAX_REG_DESCRIPTOR] = ".................................................\0";   /* 50 dots */
    char dot = '.';
    char null_terminator = '\0';
    int  i, dots_len = 0;

    seq_printf(m, "AEP regs:\n");

    for (i=0;i<AEP_REGS_ARRAY_SIZE;i++) 
    {
        dots_len = AEP_MAX_REG_DESCRIPTOR - strlen(g_aep_regs[i].name);
        dots[dots_len] = null_terminator;
        seq_printf(m, "  %s %s 0x%08X\n",g_aep_regs[i].name,dots,le32_to_cpu(reg_read_32(g_aep_regs[i].offset)));
        dots[dots_len] = dot;
    }

    /* return number of bytes writen */
    return 0;
}

#if 1
static void dump_regs(void)
{
    int i;
    char dots[AEP_MAX_REG_DESCRIPTOR] = ".................................................\0";   /* 50 dots */
    char dot = '.';
    char null_terminator = '\0';
    int  dots_len = 0;

    printk("AEP regs:\n");
    for (i=0;i< AEP_REGS_ARRAY_SIZE ;i++)
    {
        dots_len = AEP_MAX_REG_DESCRIPTOR - strlen(g_aep_regs[i].name);
        dots[dots_len] = null_terminator;  
        printk("  %s %s 0x%08X\n",g_aep_regs[i].name,dots,le32_to_cpu(reg_read_32(g_aep_regs[i].offset)));
        dots[dots_len] = dot;              
    }

    return;
}
#endif

DECLARE_PROCFS_READ_ENTRY(aep, aep_proc_dump)
DECLARE_PROCFS_READ_ENTRY(aep_fw_version, aep_proc_dump_fw_version)
DECLARE_PROCFS_READ_ENTRY(aep_status, aep_proc_status)

/**************************************************************************/
/*! \fn static int __init aep_init(void)
 **************************************************************************
 *  \brief This function is the aep module init function.
 *  \return long - 0 on success else negative number.
 **************************************************************************/
static int __init aep_init(void)
{
    char fw_ver_minor;
    char fw_ver_major;
    char fw_ver_stepping;
    char fw_ver_soc;
    unsigned int silicon_step = 0;
    unsigned int aep_mode = 0;
    
    printk(KERN_INFO "AEP: Intel(R) AEP driver built on %s @ %s\n", __DATE__, __TIME__);

    /* Get AEP mode and Silicom stepping from Boot Params */
    SRAM_Get_Boot_Param(SILICON_STEPPING_ID,&silicon_step);
    SRAM_Get_Boot_Param(AEP_MODE_ID,&aep_mode);
    
    /* If silicon stepping is lower than 'C0', or AEP mode is disabled in boot param, then AEP is disabled. */
    if ((silicon_step < SILICON_STEPPING_ID_C_0) || (aep_mode != AEP_MODE_ACTIVE))
    {
        g_aep_active = 0;
        printk(KERN_INFO "AEP: Disabled.\n");
        return 0;
    } 

    /* Create work queue for interrupt handler*/
    g_work_queue = create_workqueue("AEP_Workqueue");
    if (g_work_queue == NULL)
    {
        printk(KERN_ERR "AEP: Failed to create workqueue\n");
        return -1;
    }

    /* Create "work" for workqueue */
    INIT_WORK(&g_work, aep_workq_func);

    /* reset ipc client structure */
    memset(g_ipc_client,0,sizeof(g_ipc_client));

    /* Set timeout for AEP response - 5sec */
    g_timeout_in_seconds = 5;


    /* Enable all interrups from AEP f/w, and clear all interrupts status */
    //reg_write_32(AEP_REG_OFFSET_PV2NP_SW_INT,cpu_to_le32(0x0000007F));
    reg_write_32(AEP_REG_OFFSET_PV2NP_SW_INT,cpu_to_le32(0x00000017));

    /* Set AEP driver as "ready to use" */
    g_aep_active = 1;
     
    /* Create /proc/aep  */
    {
        struct proc_dir_entry * dir;

        if (NULL == (dir = proc_mkdir("aep", NULL)))
        {
            printk(KERN_ERR "%s:%d ERROR ....\n",__FUNCTION__,__LINE__);
            return -1;
        }

        if (NULL == (proc_create( "dump" , 0, dir, &aep_proc_fops)))
        {
            printk(KERN_ERR "%s:%d ERROR ....\n",__FUNCTION__,__LINE__);
            return -1;
        }

        if (NULL == (proc_create( "fw_version" , 0, dir, &aep_fw_version_proc_fops)))
        {
          printk(KERN_ERR "%s:%d ERROR ....\n",__FUNCTION__,__LINE__);
          return -1;
        }

        if (NULL == (proc_create( "status" , 0, dir, &aep_status_proc_fops)))
        {
          printk(KERN_ERR "%s:%d ERROR ....\n",__FUNCTION__,__LINE__);
          return -1;
        }

        printk(KERN_INFO "AEP: Create proc files /proc/aep/ \n");
    }

    /* Send IPC mesage - get FW version */
    if (get_fw_version(&fw_ver_soc,&fw_ver_stepping,&fw_ver_major,&fw_ver_minor) != 0)
    {
        g_aep_active = 0;
        printk(KERN_ERR "AEP: Failed to read AEP FW version.\n");
        printk(KERN_ERR "AEP: Disabled.\n");

    }

    printk(KERN_INFO "AEP: FW version = %d.%d.%d.%d\n",fw_ver_soc,fw_ver_stepping,fw_ver_major,fw_ver_minor);

    return 0;
}


/**************************************************************************/
/*! \fn static void __exit aep_exit(void)
 **************************************************************************
 *  \brief This function is the aep module exit function.
 **************************************************************************/
static void __exit aep_exit(void)
{
    // Set AEP driver as "not ready to use"
    g_aep_active = 0;

    destroy_workqueue( g_work_queue );

}
/*************************************************************************************/

subsys_initcall(aep_init);
module_exit(aep_exit);

/* Driver identification */
MODULE_DESCRIPTION("AEP Core Driver");
MODULE_AUTHOR("Intel Corporation");
MODULE_LICENSE("GPL");

