/*
  GPL LICENSE SUMMARY

  Copyright(c) 2008-2016 Intel Corporation.

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
*/

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/string.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/inetdevice.h>
#include <linux/rtnetlink.h>
#include <linux/ti_hil.h>
#include <linux/notifier.h>

#if PUMA7_OR_NEWER_SOC_TYPE
#ifdef CONFIG_ARM_AVALANCHE_SOC
#include "synopsys_gbe_interface.h"
#else
#include <linux/avalanche/puma7/synopsys_gbe_interface.h>
#include <linux/avalanche/puma7/puma7_defs.h>
#endif
#endif

/**************************************************************************
 ***************************** Static Declarations  ***********************
 **************************************************************************/

/* HIL Core Event Handler for all networking events. */
static int ti_hil_net_event_handler(struct notifier_block *chain, unsigned long event, void *ptr);

/* Dev Notifier Block for the HIL; this will get notifications for all the dev related events. */
static struct notifier_block hil_dev_notifier_block =
{
    .notifier_call = ti_hil_net_event_handler,
};

/* INET Addr Notifier Block for the PPM; this will get notifications for all the INET related events. */
static struct notifier_block hil_inetaddr_notifier_block =
{
    .notifier_call = ti_hil_net_event_handler,
};

/* PP Notifier Block for the PP; this will get notifications for all the PP related events. */
static struct notifier_block hil_pp_notifier_block =
{
    .notifier_call = ti_hil_net_event_handler,
};

/**************************************************************************
 * STRUCTURE NAME : TI_HIL_MCB
 **************************************************************************
 * DESCRIPTION   :
 *  The structure describes the HIL Master control block which is used
 *  to keep track of the HIL profiles which are active.
 **************************************************************************/
typedef struct TI_HIL_MCB
{
    /* HIL Status   */
    int              initialized;

    /* Active HIL Profile. */
    TI_HIL_PROFILE*  active_profile;
}TI_HIL_MCB;

/**************************************************************************
 ********************************* Globals ********************************
 **************************************************************************/

/* HIL Master Control Block. */
TI_HIL_MCB             hil_mcb;

/* Packet Processor Notifier Chain */
ATOMIC_NOTIFIER_HEAD(pp_chain);

#ifdef CONFIG_TI_PACKET_PROCESSOR_STATS

/* Mac Counter feature */
TI_HIL_MCS_START_SESSION ti_hil_mcs_ctl_start_session_notification_cb = NULL;
EXPORT_SYMBOL(ti_hil_mcs_ctl_start_session_notification_cb);

/* DOCSIS Packet processor start session notification Callback */
TI_HIL_START_SESSION ti_hil_start_session_notification_cb = NULL;
EXPORT_SYMBOL(ti_hil_start_session_notification_cb);
/* DOCSIS Packet processor delete session notification Callback */
TI_HIL_DELETE_SESSION ti_hil_delete_session_notification_cb = NULL;
EXPORT_SYMBOL(ti_hil_delete_session_notification_cb);
#endif /* CONFIG_TI_PACKET_PROCESSOR_STATS */

/**************************************************************************
 ******************************* Functions  *******************************
 **************************************************************************/

/**************************************************************************
 * FUNCTION NAME : ti_hil_net_event_handler
 **************************************************************************
 * DESCRIPTION   :
 *  HIL Core registered event handler. This function receives event from all
 *  over the networking subsystem.
 *
 * RETURNS       :
 *  Always returns NOTIFY_DONE
 **************************************************************************/
static int ti_hil_net_event_handler(struct notifier_block *chain, unsigned long event, void *ptr)
{
    unsigned int    module;

    /* Check if there exists an active profile. */
    if (hil_mcb.active_profile)
    {
        /* Use the chain to identify the module */
        if (chain == &hil_dev_notifier_block)
        {
            /* Device Interface Management Module. */
            module = TI_DEVICE;
        }
        else if (chain == &hil_inetaddr_notifier_block)
        {
            /* Inet address Management Module. */
            module = TI_INET;
        }
        else if (chain == &hil_pp_notifier_block)
        {
            /* PP Chain Management Module. */
            module = TI_PP;
        }
        else
        {
            /* Unrecognized chain: This is a FATAL Error and should not happen. */
            printk ("FATAL HIL Core Error: Received event from unknown chain 0x%p\n", chain);
            return NOTIFY_DONE;
        }

        /* Pass the event to the profile handler. */
        hil_mcb.active_profile->profile_handler (module, event, ptr);
    }

    /* Work is succesfully completed. */
    return NOTIFY_DONE;
}

/**************************************************************************
 * FUNCTION NAME : ti_hil_load_profile
 **************************************************************************
 * DESCRIPTION   :
 *  The function is used to load the specific HIL Profile.
 *
 * RETURNS       :
 *  0   -   Success
 *  <0  -   Error
 **************************************************************************/
static int ti_hil_load_profile (TI_HIL_PROFILE* ptr_profile)
{
    /* Unload and deinitialize the profile. */
    if (ptr_profile->profile_init() < 0)
    {
        printk ("HIL Core: Unable to load profile %s\n", ptr_profile->name);
        return -1;
    }

    /* Profile has been succesfully loaded. */
    printk ("HIL Core: Profile %s has been Loaded.\n", ptr_profile->name);
    return 0;
}

/**************************************************************************
 * FUNCTION NAME : ti_hil_unload_profile
 **************************************************************************
 * DESCRIPTION   :
 *  The function is used to unload the specific HIL Profile.
 *
 * RETURNS       :
 *  0   -   Success
 *  <0  -   Error
 **************************************************************************/
static int ti_hil_unload_profile (TI_HIL_PROFILE* ptr_profile)
{
    /* Unload and deinitialize the profile. */
    if (ptr_profile->profile_deinit() < 0)
    {
        printk ("HIL Core: Unable to unload profile %s\n", ptr_profile->name);
        return -1;
    }

    /* Profile has been succesfully unloaded. */
    printk ("HIL Core: Profile %s has been unloaded.\n", ptr_profile->name);
    return 0;
}

/**************************************************************************
 * FUNCTION NAME : ti_hil_register_profile
 **************************************************************************
 * DESCRIPTION   :
 *  The function is used to register the HIL Profile.
 *
 * RETURNS       :
 *  0   -   Success
 *  <0  -   Error
 **************************************************************************/
int ti_hil_register_profile (TI_HIL_PROFILE* ptr_profile)
{
    /* Ensure that the HIL has been initialized before proceeding. */
    if (hil_mcb.initialized == 0)
        return -1;

    /* Validate the arguments. */
    if (ptr_profile == NULL)
        return -1;

    /* Check if a profile already exists? */
    if (hil_mcb.active_profile != NULL)
    {
        /* Profile already exists. We need to unload the profile and load the
         * new one instead. */
        ti_hil_unload_profile (hil_mcb.active_profile);

        /* Cleanup the memory associated with the active profile. */
        kfree (hil_mcb.active_profile);
    }

    /* Allocate memory for the HIL Profile. */
    hil_mcb.active_profile = (TI_HIL_PROFILE *)kmalloc(sizeof(TI_HIL_PROFILE), GFP_KERNEL);
    if (hil_mcb.active_profile == NULL)
    {
        /* Memory Allocation Failed. */
        printk ("HIL Core: Unable to allocate memory for the HIL Profile %s\n", ptr_profile->name);
        return -1;
    }

    /* Copy the profile into the active profile. */
    memcpy ((void *)hil_mcb.active_profile, (void *)ptr_profile, sizeof(TI_HIL_PROFILE));

    /* Initialize the profile. */
    if (ti_hil_load_profile (hil_mcb.active_profile) == 0)
        return 0;

    /* Error: Unable to initialize the profile. */
    printk ("HIL Core: HIL Profile %s failed to initialize.\n", hil_mcb.active_profile->name);
    return -1;
}

/**************************************************************************
 * FUNCTION NAME : ti_hil_initialize
 **************************************************************************
 * DESCRIPTION   :
 *  Initialize the HIL Core Layer.
 *
 * RETURNS       :
 *  0   -   Success
 *  <0  -   Error
 **************************************************************************/
int ti_hil_initialize (void)
{
    /* Initialize the HIL Master Control Block */
    memset ((void *)&hil_mcb, 0, sizeof(TI_HIL_MCB));

    /* Register the HIL core to receive netdevice events */
    register_netdevice_notifier(&hil_dev_notifier_block);

    /* Register the HIL core to receive inet events */
    register_inetaddr_notifier(&hil_inetaddr_notifier_block);

    /* Register the HIL core to receive events */
    atomic_notifier_chain_register(&pp_chain, &hil_pp_notifier_block);

    /* The HIL core is operational now. */
    hil_mcb.initialized = 1;
    return 0;
}


/**************************************************************************
 * FUNCTION NAME : ti_hil_pp_event
 **************************************************************************
 * DESCRIPTION   :
 *  Notify the HIL of a PP event.
 *
 * RETURNS       :
 *  0   -   Success
 *  <0  -   Error
 **************************************************************************/
int ti_hil_pp_event(unsigned long val, void *v)
{
    return atomic_notifier_call_chain(&pp_chain, val, v);
}

#if PUMA7_OR_NEWER_SOC_TYPE
/**************************************************************************
 * FUNCTION NAME : ti_hil_is_gbe_device
 **************************************************************************
 * DESCRIPTION   :
 *  Return true if this is a gbe device
 *
 * RETURNS       :
 *  None
 **************************************************************************/
int ti_hil_is_gbe_device(struct net_device *origDev)
{
    if (0 == strcmp(origDev->name, NSGMII0_NAME))
    {
        return 1;
    }

    if (0 == strcmp(origDev->name, NSGMII1_NAME))
    {
        return 1;
    }

    if (0 == strcmp(origDev->name, NRGMII2_NAME))
    {
        return 1;
    }

    if (0 == strcmp(origDev->name, NRGMII3_NAME))
    {
        return 1;
    }

    if (0 == strcmp(origDev->name, NGMII_TO_ATOM_NAME))
    {
        return 1;
    }

    return 0;
}
#endif

/**************************************************************************
 * FUNCTION NAME : ti_hil_clone_netdev_pp_info
 **************************************************************************
 * DESCRIPTION   :
 *  Copies all the relevant PP PID information from one network device to another
 *
 * RETURNS       :
 *  None
 **************************************************************************/
void ti_hil_clone_netdev_pp_info(struct net_device *newDev, struct net_device *origDev)
{
    newDev->pid_handle              = origDev->pid_handle;
    memcpy((void *)&newDev->vpid_block, (void *)&origDev->vpid_block, sizeof(origDev->vpid_block));
#if PUMA7_OR_NEWER_SOC_TYPE
    strncpy(newDev->vpid_block.devName, newDev->name, VPID_IF_NAME_SIZE);
#endif
    newDev->qos_setup_hook          = origDev->qos_setup_hook;
    newDev->qos_shutdown_hook       = origDev->qos_shutdown_hook;
    newDev->qos_select_hook         = origDev->qos_select_hook;
#if PUMA7_OR_NEWER_SOC_TYPE
    newDev->qos_get_params_hook     = origDev->qos_get_params_hook;
    newDev->netdev_set_psi_hook     = origDev->netdev_set_psi_hook;
#endif
    newDev->qos_virtual_scheme_idx  = origDev->qos_virtual_scheme_idx;
    newDev->devInstance             = origDev->devInstance;

#if PUMA7_OR_NEWER_SOC_TYPE
#ifdef CONFIG_ARM_AVALANCHE_SOC
    if (ti_hil_is_gbe_device(origDev))
#endif
    {
        if (origDev->netdev_copy_priv_hook)
        {
            origDev->netdev_copy_priv_hook(newDev, origDev);
	        newDev->netdev_copy_priv_hook = origDev->netdev_copy_priv_hook;
        }
    }
#endif
}
EXPORT_SYMBOL(ti_hil_clone_netdev_pp_info);

#ifdef CONFIG_TI_PACKET_PROCESSOR_STATS

/**************************************************************************
 * FUNCTION NAME : ti_hil_mcs_ctl_register_start_session_notification
 **************************************************************************
 * DESCRIPTION   :
 *  Register the start session notification for mcs feature
 *  callback function
 *
 * RETURNS       :
 *  0   -   Success
 *  <0  -   Error
 **************************************************************************/
int ti_hil_mcs_ctl_register_start_session_notification(TI_HIL_MCS_START_SESSION ti_hil_mcs_start_session_notification)
{
    if (ti_hil_mcs_ctl_start_session_notification_cb != NULL)
    {
        printk( "Error: Mac Count element start session notification callback already exist\n");
        return -1;
    }

    printk( "Register ti_hil_mcs_ctl_register_start_session_notification  succeed \n");
    ti_hil_mcs_ctl_start_session_notification_cb = ti_hil_mcs_start_session_notification;

    return 0;
}

/**************************************************************************
 * FUNCTION NAME : ti_hil_mcs_ctl_unregister_start_session_notification
 **************************************************************************
 * DESCRIPTION   :
 *  Un-Register start session notification for mac aging feature
 *  callback function
 *
 * RETURNS       :
 *  0   -   Success
 *  <0  -   Error
 **************************************************************************/
int ti_hil_mcs_ctl_unregister_start_session_notification(void)
{
    if (ti_hil_mcs_ctl_start_session_notification_cb == NULL)
    {
        printk(KERN_DEBUG "Error: Mac Count element start session notification callback empty\n");
        return -1;
    }

    ti_hil_mcs_ctl_start_session_notification_cb = NULL;

    return 0;
}

/**************************************************************************
 * FUNCTION NAME : ti_hil_register_start_session_notification
 **************************************************************************
 * DESCRIPTION   :
 *  Register the DOCSIS Packet Processor start session notification
 *  callback function
 *
 * RETURNS       :
 *  0   -   Success
 *  <0  -   Error
 **************************************************************************/
int ti_hil_register_start_session_notification(TI_HIL_START_SESSION ti_hil_start_session_notification)
{
    if (ti_hil_start_session_notification_cb != NULL)
    {
        printk ("Error: DOCSIS Packet Processor start session notification callback already exist\n");
        return -1;
    }

    ti_hil_start_session_notification_cb = ti_hil_start_session_notification;

    return 0;
}

/**************************************************************************
 * FUNCTION NAME : ti_hil_unregister_start_session_notification
 **************************************************************************
 * DESCRIPTION   :
 *  Un-Register the DOCSIS Packet Processor start session notification
 *  callback function
 *
 * RETURNS       :
 *  0   -   Success
 *  <0  -   Error
 **************************************************************************/
int ti_hil_unregister_start_session_notification(void)
{
    if (ti_hil_start_session_notification_cb == NULL)
    {
        printk ("Error: DOCSIS Packet Processor start session notification callback empty\n");
        return -1;
    }

    ti_hil_start_session_notification_cb = NULL;

    return 0;
}

/**************************************************************************
 * FUNCTION NAME : ti_hil_register_delete_session_notification
 **************************************************************************
 * DESCRIPTION   :
 *  Register the DOCSIS Packet processor delete session notification
 *  callback function
 *
 * RETURNS       :
 *  0   -   Success
 *  <0  -   Error
 **************************************************************************/
int ti_hil_register_delete_session_notification(TI_HIL_DELETE_SESSION ti_hil_delete_session_notification)
{
    if (ti_hil_delete_session_notification_cb != NULL)
    {
        printk ("Error: DOCSIS Packet Processor delete session notification callback already exist\n");
        return -1;
    }

    ti_hil_delete_session_notification_cb = ti_hil_delete_session_notification;

    return 0;
}

/**************************************************************************
 * FUNCTION NAME : ti_hil_unregister_delete_session_notification
 **************************************************************************
 * DESCRIPTION   :
 *  Un-Register the DOCSIS Packet processor delete session notification
 *  callback function
 *
 * RETURNS       :
 *  0   -   Success
 *  <0  -   Error
 **************************************************************************/
int ti_hil_unregister_delete_session_notification(void)
{
    if (ti_hil_delete_session_notification_cb == NULL)
    {
        printk ("Error: DOCSIS Packet Processor delete session notification callback empty\n");
        return -1;
    }

    ti_hil_delete_session_notification_cb = NULL;

    return 0;
}
#endif /* CONFIG_TI_PACKET_PROCESSOR_STATS */

/* Export all the Symbols for Linux; so that these can be called from modules. */
EXPORT_SYMBOL(ti_hil_initialize);
EXPORT_SYMBOL(ti_hil_register_profile);
EXPORT_SYMBOL(ti_hil_pp_event);
#ifdef CONFIG_TI_PACKET_PROCESSOR_STATS

/* Mac counter */
EXPORT_SYMBOL(ti_hil_mcs_ctl_register_start_session_notification);
EXPORT_SYMBOL(ti_hil_mcs_ctl_unregister_start_session_notification);

/* DOCSIS Packet processor start/delete session notification API */
EXPORT_SYMBOL(ti_hil_register_start_session_notification);
EXPORT_SYMBOL(ti_hil_unregister_start_session_notification);
EXPORT_SYMBOL(ti_hil_register_delete_session_notification);
EXPORT_SYMBOL(ti_hil_unregister_delete_session_notification);
#endif /* CONFIG_TI_PACKET_PROCESSOR_STATS */

