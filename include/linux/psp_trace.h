/*
 *
 * psp_trace.h
 * Description:
 * see below
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


/** \file   psp_trace.h
    \brief  PSP tracer Header file

    This file contains driver domain interfaces for trace service:

    \author     PSP Team
    \version    1.0
 */

#ifndef _TRACER_H
#define _TRACER_H

/** 
	\brief TRACE_NUM_ENTRIES : defines number of lines in the trace buffer.
			Each line consist of traced event + 2 parameters and timestamp.
			For the performance reasons should be power of 2 					
			Memory requirements for tracer utility might be calculated : TRACE_NUM_ENTRIES * 4 * sizeof(int)
*/ 	
#define TRACE_NUM_ENTRIES 1024  

/** 
	\brief MAX_USER_DEFINED_EVENTS : defines number of events that can be dynamically registered
			by user through the trace_register_new_event API
*/ 	
#define MAX_USER_DEFINED_EVENTS 32  

/** 
	\brief Event is composed from two parts : group_id and event_id
			This macro is for the internal use.
			User should not define new events by using this macro, but by adding the 
			new event to appropriate enum : 	tracer_xxx_group_events 
*/
#define BUILD_EVENT_ID(group,event) (((unsigned int)(group) << 16) | ((unsigned int)(event) & 0xFFFF))

/** 
	\brief logical values defining event's state 
*/
typedef enum 
{
    Event_Disable = 0, 
    Event_Enable = 1
} event_state;

/** 
*	\brief Describes all available groups of events.
*			For user's convenience all events assigned into different logical groups.
*			Each group can be managed (disabled/enabled) separately through trace service control proc entry.	
*			Should be extended only if new group is required
*/
typedef enum 
{	
	TRACER_GENERAL_GROUP = 0,   /**< general purpose events */
	TRACER_ATM_DRIVER_GROUP, 	/**< events related to the ATM data path: driver, 2684 bridge */ 
	TRACER_ETH_DRIVER_GROUP, 	/**< events related to the ethernet driver */ 
	TRACER_USB_DRIVER_GROUP,  	/**< events related to the USB driver, RNDIS layer*/
	TRACER_BRIDGE_GROUP,      	/**< events related to the L2 bridge, VLAN interfaces */
	TRACER_IP_STACK_GROUP,    	/**< events related to the IP stack, NAT, Firewall, */
	TRACER_WLAN_DRIVER_GROUP,	/**< events related to the AP driver, BSS bridge */
	TRACER_VOICE_APP_GROUP,		/**< events related to the voice application */
	TRACER_QOS_GROUP,		    /**< events related to the NSP QoS Framework */
	TRACER_USER_DEFINED_GROUP,	/**<  dynamically registered events */
	TRACER_GROUPS_NUMBER 		/**< always keep as last element in enum  */
} tracer_event_groups;

#define FIRST_PREDEFINE_EVENT(group) group##_FIRST_EVENT
#define LAST_PREDEFINE_EVENT(group) group##_LAST_EVENT

/** 
	\brief Events list for general purposes
			New events for this group should be declared here, between first and last events.
*/
typedef enum
{
	FIRST_PREDEFINE_EVENT(TRACER_GENERAL_GROUP) = BUILD_EVENT_ID(TRACER_GENERAL_GROUP,0),

	GNR_EXAMPLE_EVENT = FIRST_PREDEFINE_EVENT(TRACER_GENERAL_GROUP),	/**< example */
	
	LAST_PREDEFINE_EVENT(TRACER_GENERAL_GROUP)
}tracer_general_events;

/** 
	\brief Events list for the ATM stack
			New events for this group should be declared here, between first and last events.
*/
typedef enum
{
	FIRST_PREDEFINE_EVENT(TRACER_ATM_DRIVER_GROUP) = BUILD_EVENT_ID(TRACER_ATM_DRIVER_GROUP,0),

	ATM_DRV_SAR_ISR_ENTER = FIRST_PREDEFINE_EVENT(TRACER_ATM_DRIVER_GROUP) , /**< This pair of events embraces the SAR interrupt handler */
	ATM_DRV_SAR_ISR_EXIT,              /**< This pair of events embraces the SAR interrupt handler  */      

	ATM_DRV_TASKLET_ENTER,         /**< This pair of events embraces the tasklet routine  */      
	ATM_DRV_TASKLET_EXIT,          /**< This pair of events embraces the tasklet routine  */

	ATM_DRV_PKT_TX_ENTER,          /**< This pair of events embraces the packet's  send     routine  */   
	ATM_DRV_PKT_TX_EXIT,           /**< This pair of events should be used with skb as parameter */       

	ATM_DRV_PKT_TX_COMPLETE_ENTER, /**< This pair of events embraces the packet's  send     complete logic */       
	ATM_DRV_PKT_TX_COMPLETE_EXIT,  /**< This pair of events should   be  used      with     skb      as    parameter */       

	ATM_DRV_TX_COMPLETE_ENTER,     /**< This pair of events embraces the transmit  complete handler  in    the       tasklet*/
	ATM_DRV_TX_COMPLETE_EXIT,     

	ATM_DRV_RX_ENTER,              /**< This pair of events embraces the receive   handler  in       the   tasklet   */       
	ATM_DRV_RX_EXIT,              

	ATM_DRV_PKT_RX_ENTER,          /**< This pair of events embraces the packet’s  receive  path     */   
	ATM_DRV_PKT_RX_EXIT,           /**< This pair of events should   be  used      with     skb      as    parameter */       

    ATM_DRV_DSL_ISR_ENTER,         /**< This pair of events embraces the DSL interrupt handler  */      
	ATM_DRV_DSL_ISR_EXIT,          /**< This pair of events embraces the DSL interrupt handler  */

	LAST_PREDEFINE_EVENT(TRACER_ATM_DRIVER_GROUP)
}tracer_atm_events;

/** 
	\brief Events list for the ethernet driver
			New events for this group should be declared here, between first and last events.

	\warning 	Should be extended to support two instances !!!
				Instance number for now might be passed as parameter.

*/
typedef enum
{
	FIRST_PREDEFINE_EVENT(TRACER_ETH_DRIVER_GROUP) = BUILD_EVENT_ID(TRACER_ETH_DRIVER_GROUP,0),/**< This pair of events embraces the interrupt handler */
#ifdef CONFIG_MACH_PUMA5
	ETH_DRV_TX_ISR_ENTER = FIRST_PREDEFINE_EVENT(TRACER_ETH_DRIVER_GROUP),             
	ETH_DRV_TX_ISR_EXIT,             /**< This pair of events embraces the interrupt handler  */     
        ETH_DRV_RX_ISR_ENTER,             /**< This pair of events embraces the interrupt handler  */ 
        ETH_DRV_RX_ISR_EXIT,             /**< This pair of events embraces the interrupt handler  */ 	
	ETH_DRV_TX_TASKLET_ENTER,        /**< This pair of events embraces the tasklet routine  */       
	ETH_DRV_TX_TASKLET_EXIT,         /**< This pair of events embraces the tasklet routine  */
	ETH_DRV_RX_TASKLET_ENTER,        /**< This pair of events embraces the tasklet routine  */       
	ETH_DRV_RX_TASKLET_EXIT,         /**< This pair of events embraces the tasklet routine  */
#else
	ETH_DRV_ISR_ENTER = FIRST_PREDEFINE_EVENT(TRACER_ETH_DRIVER_GROUP),             
	ETH_DRV_ISR_EXIT,             /**< This pair of events embraces the interrupt handler  */      
	ETH_DRV_TASKLET_ENTER,        /**< This pair of events embraces the tasklet routine  */       
	ETH_DRV_TASKLET_EXIT,         /**< This pair of events embraces the tasklet routine  */
#endif
	ETH_DRV_TMR_ENTER,	/**< This pair of events embraces the USB timer routine */
	ETH_DRV_TMR_EXIT,
	ETH_DRV_PKT_TX_ENTER,         /**< This pair of events embraces the packet's  send     complete logic */        
	ETH_DRV_PKT_TX_EXIT,          /**< This pair of events should   be  used      with     skb      as    parameter */       
	ETH_DRV_PKT_TX_COMPLETE_ENTER,/**< This pair of events embraces the transmit  complete handler  in    the       tasklet*/ 
	ETH_DRV_PKT_TX_COMPLETE_EXIT, 
	ETH_DRV_TX_COMPLETE_ENTER,    /**< This pair of events embraces the receive   handler  in       the   tasklet   */        
	ETH_DRV_TX_COMPLETE_EXIT,     
	ETH_DRV_RX_ENTER,             /**< This pair of events embraces the packet’s  receive  path     */    
	ETH_DRV_RX_EXIT,              /**< This pair of events should   be  used      with     skb      as    parameter */       
	ETH_DRV_PKT_RX_ENTER,          
	ETH_DRV_PKT_RX_EXIT,          

	LAST_PREDEFINE_EVENT(TRACER_ETH_DRIVER_GROUP)
}tracer_eth_events;

/** 
	\brief Events list for the USB driver
			New events for this group should be declared here, between first and last events.
*/
typedef enum
{
	FIRST_PREDEFINE_EVENT(TRACER_USB_DRIVER_GROUP) = BUILD_EVENT_ID(TRACER_USB_DRIVER_GROUP,0),

	USB_DRV_ISR_ENTER = FIRST_PREDEFINE_EVENT(TRACER_USB_DRIVER_GROUP), /**< This pair of events embraces the VLYNQ interrupt handler */
	USB_DRV_ISR_EXIT,
	USB_DRV_TASKLET_ENTER, /**< This pair of events embraces the USB tasklet routine */
	USB_DRV_TASKLET_EXIT,
	USB_DRV_TMR_ENTER,	/**< This pair of events embraces the USB timer routine which is main handler.*/
	USB_DRV_TMR_EXIT,
	USB_DRV_PKT_TX_ENTER, /**< This pair of events embraces the packet's send routine */
	USB_DRV_PKT_TX_EXIT,	
	USB_DRV_PKT_TX_COMPLETE_ENTER, /**< This pair of events embraces the packet's send complete logic */
	USB_DRV_PKT_TX_COMPLETE_EXIT,
	USB_DRV_TX_COMPLETE_ENTER, /**< This pair of events embraces the transmit complete handler in the tasklet*/
	USB_DRV_TX_COMPLETE_EXIT,
	USB_DRV_RX_ENTER, /**< This pair of events embraces the receive handler in the tasklet */
	USB_DRV_RX_EXIT,
	USB_DRV_PKT_RX_ENTER, /**< This pair of events embraces the packet’s receive path */
	USB_DRV_PKT_RX_EXIT,
	USB_DRV_TX_ENTER, /**< This pair of events embraces the transmit portion of the polling routine */
	USB_DRV_TX_EXIT,

	LAST_PREDEFINE_EVENT(TRACER_USB_DRIVER_GROUP)
}tracer_usb_events;

/** 
	\brief Events list for the IP stack
			New events for this group should be declared here, between first and last events.
*/
typedef enum
{
	FIRST_PREDEFINE_EVENT(TRACER_IP_STACK_GROUP) = BUILD_EVENT_ID(TRACER_IP_STACK_GROUP,0),
	IP_NET_PROTOCOL_ENTER = FIRST_PREDEFINE_EVENT(TRACER_IP_STACK_GROUP),
	IP_NET_PROTOCOL_EXIT,
    IP_PRE_CONNTRACK_ENTER,
    IP_PRE_CONNTRACK_EXIT,
    IP_POST_CONNTRACK_ENTER,
    IP_POST_CONNTRACK_EXIT,
    IP_PRE_NAT_ENTER,
    IP_PRE_NAT_EXIT,
    IP_POST_NAT_ENTER,
    IP_POST_NAT_EXIT,
    IP_MANGLE_ENTER,
    IP_MANGLE_EXIT,
    IP_FILTER_ENTER,
    IP_FILTER_EXIT,
	LAST_PREDEFINE_EVENT(TRACER_IP_STACK_GROUP)
}tracer_ip_events;

/** 
	\brief Events list for the bridge
			New events for this group should be declared here, between first and last events.
*/
typedef enum
{
	FIRST_PREDEFINE_EVENT(TRACER_BRIDGE_GROUP) = BUILD_EVENT_ID(TRACER_BRIDGE_GROUP,0),
	BR_HANDLE_FRAME_ENTER = FIRST_PREDEFINE_EVENT(TRACER_BRIDGE_GROUP),
	BR_HANDLE_FRAME_EXIT,
    BR_DEV_XMIT_ENTER,
    BR_DEV_XMIT_EXIT,
	LAST_PREDEFINE_EVENT(TRACER_BRIDGE_GROUP)
}tracer_bridge_events;

/** 
	\brief Events list for the AP driver
			New events for this group should be declared here, between first and last events.
*/
typedef enum
{
	FIRST_PREDEFINE_EVENT(TRACER_WLAN_DRIVER_GROUP) = BUILD_EVENT_ID(TRACER_WLAN_DRIVER_GROUP,0),

	AP_DRV_ISR_ENTER = FIRST_PREDEFINE_EVENT(TRACER_WLAN_DRIVER_GROUP), /**< This pair of events embraces the VLYNQ interrupt handler */
	AP_DRV_ISR_EXIT,
	AP_DRV_TASKLET_ENTER, /**< This pair of events embraces the AP tasklet routine */
	AP_DRV_TASKLET_EXIT,
	AP_DRV_TMR_ENTER,	/**< This pair of events embraces the AP timer routine */
	AP_DRV_TMR_EXIT,
	AP_DRV_PKT_TX_ENTER, /**< This pair of events embraces the packet's send routine */
	AP_DRV_PKT_TX_EXIT,
	AP_DRV_PKT_TX_COMPLETE_ENTER, /**< This pair of events embraces the packet's send complete logic */
	AP_DRV_PKT_TX_COMPLETE_EXIT,
	AP_DRV_TX_COMPLETE_ENTER, /**< This pair of events embraces the transmit complete handler in the tasklet*/
	AP_DRV_TX_COMPLETE_EXIT,
	AP_DRV_RX_ENTER, /**< This pair of events embraces the receive handler in the tasklet */
	AP_DRV_RX_EXIT,
	AP_DRV_PKT_RX_ENTER, /**< This pair of events embraces the packet’s receive path */
	AP_DRV_PKT_RX_EXIT,

	LAST_PREDEFINE_EVENT(TRACER_WLAN_DRIVER_GROUP)
}tracer_wlan_group_events;

/** 
	\brief Events list for voice application 
			New events for this group should be declared here, between first and last events.
*/
typedef enum
{
	FIRST_PREDEFINE_EVENT(TRACER_VOICE_APP_GROUP) = BUILD_EVENT_ID(TRACER_VOICE_APP_GROUP,0),

	VOIP_EXAMPLE_EVENT = FIRST_PREDEFINE_EVENT(TRACER_VOICE_APP_GROUP),	/**< example */
	VOIP_1_EXAMPLE_EVENT_ENTER,
	VOIP_1_EXAMPLE_EVENT_EXIT,
	VOIP_2_EXAMPLE_EVENT,

	LAST_PREDEFINE_EVENT(TRACER_VOICE_APP_GROUP)
}tracer_voice_app_events;

/**
    \brief Events list for the NSP QoS Framework.
            New events for this group should be declared here, between first and last events.
*/
typedef enum
{
    FIRST_PREDEFINE_EVENT(TRACER_QOS_GROUP) = BUILD_EVENT_ID(TRACER_QOS_GROUP,0),

    /* NSP QoS Framework Hooks. */
    NSP_QOS_ENQUEUE_ENTER = FIRST_PREDEFINE_EVENT(TRACER_QOS_GROUP),
    NSP_QOS_ENQUEUE_EXIT,
    NSP_QOS_DEQUEUE_ENTER,
    NSP_QOS_DEQUEUE_EXIT,
    NSP_QOS_CONGESTION_ENTER,
    NSP_QOS_CONGESTION_EXIT,
    NSP_QOS_PIL_ETH_ENTER,
    NSP_QOS_PIL_ETH_EXIT,
    NSP_QOS_PIL_IP_ENTER,
    NSP_QOS_PIL_IP_EXIT,
    NSP_QOS_PIL_WLAN_ENTER,
    NSP_QOS_PIL_WLAN_EXIT,
    NSP_QOS_TU_ENTER,
    NSP_QOS_TU_EXIT,
    NSP_QOS_SHAPER_ENTER,
    NSP_QOS_SHAPER_EXIT,
    LAST_PREDEFINE_EVENT(TRACER_QOS_GROUP)
}tracer_qos_events;

/** 
*	\brief	Error codes for the tracer module.
*/
#define EEVENTNAME	1 /**< Invalid event's description */
#define EMAXEV		2 /**< Number of registered events exceeded at MAX_USER_DEFINED_EVENTS */

/** 
*		Tracer service might be completely compiled out.
*/
#ifdef CONFIG_PSP_TRACE

/**
	\brief  	Main trace function.Stores a trace line in the trace buffer.
	\warning	This function disables interrupts for ~5 instructions.

	\param  	event_id   [IN]	Traced event ID as declared above : GNR_EXAMPLE_EVENT
	\param  	par    	  [IN]	General purpose parameter
	\param  	sec_par    [IN]	General purpose parameter
 */
void trace_event(int event_id,int par,int sec_par);

/**
	\brief trace_print_2_scr - prints a content of the trace buffer on the screen.
			Migth be used in case that proc file system is unavailable.
			Call to this function might be added to the oops handler.
 */
void trace_print_2_scr(void);

/**
	\brief trace_register_new_event - Creates and adds a new event to the TRACER_USER_DEFINED_GROUP group.
			Allows the user to add new events without rebuild the kernel.
			Number of dynamic events limited by MAX_USER_DEFINED_EVENTS
	\param event_name [IN] string decsribing event's name 
	\param state      [IN] event's default state (Event_Enable/Event_Disable)

	\return	event id of the new event if succesfull;
				-EEVENTNAME or -EMAXEV if fails 

	\warning	This function has critical section inside.
 */
int trace_register_new_event(char* event_name,event_state state);

/**
	\brief	psp_trace,psp_trace_par,psp_trace_2_par - Store the event with parameters ( if exists ) 
			in the trace buffer
			Those macros will compiled out in case that CONFIG_PSP_TRACE isn't defined.
			The usage of those macros is recommended by not required. 
			trace_register_new_event API can be invoked directly.
*/
	#define psp_trace(x) trace_event(x,0,0)
	#define psp_trace_par(a,b) trace_event(a,((int) b),0)
	#define psp_trace_2_par(a,b,c) trace_event(a,((int) b),((int) c))
/**
	\brief	psp_add_event - Adds a new event to the TRACER_USER_DEFINED_GROUP group and
			sets it's state to enable.
			This macro will compiled out in case that CONFIG_PSP_TRACE isn't defined.
			The usage of this macro is recommended by not required. 
			trace_register_new_event API can be invoked directly.
*/
	#define psp_add_event(event) trace_register_new_event(event,Event_Enable) 
/**
	\brief	psp_trace_print - Prints a content of the trace buffer ( with appropriate formatting )
			to the serial console.
			This macro will compiled out in case that CONFIG_PSP_TRACE isn't defined.
			The usage of this macro is recommended by not required. 
			trace_print_2_scr API can be invoked directly.
*/
	#define psp_trace_print trace_print_2_scr

#else
	#define psp_trace(a)
	#define psp_trace_par(a,b)
	#define psp_trace_par(a,b)
	#define psp_trace_2_par(a,b,c)
	#define psp_add_event(event)
#endif

#endif /* _TRACER_H */
