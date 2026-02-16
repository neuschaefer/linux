/*
 *
 * ddc_switch.h
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


#ifndef __SWITCH_INTERFACE_H__
#define __SWITCH_INTERFACE_H__

#include <pal_defs.h>

/****************************************************************************************/
/* eswitchInterface.h - External  Switch Interface                                                                              */
/*
   DESCRIPTION

   This file contains the interface to the basic Switch functionality.

   SWITCH MANAGEMENT

   switch_reset()       -   Reset the switch
   switch_getVersion()  -   Returns the switch Vendor ID / Version
   SwitchDrvCreate()    -   Creates instance of the switch driver. After the call the driver is inactive state.
   SwitchDrvStart()     -   Starts the driver. After the call the driver is in working state.
   SwitchDrvStop()      -   Stops the driver. Move the driver into inactive state.
   SwitchDrvDelete()    -   Deletes instance of the switch driver.


   PORT MANAGEMENT

   switch_specCapsSet()   -   Passes a special configuration of the port
   switch_specCapsGet()   -   Retreives a special configuration of the port
   switch_portStatusSet()   -   Sets the port status ,- disable/listening/learning/forwarding (if possible)
   switch_portStatusGet()   -   Gets the port status ,- disable/listening/learning/forwarding
   switch_portLinkGet()     -   Gets port link status(Up/down)
   switch_portFCGet()       -   Gets flow control status(On/off)
   switch_portFCSet()       -   Sets flow control (On/off)
   switch_portDuplexGet()   -   Gets port duplex status (Full/Half)
   switch_portDuplexSet()   -   Sets port duplex (Full/Half)
   switch_portSpeedGet()    -   Gets port speed status(10/100)
   switch_portSpeedSet()    -   Sets port speed (10/100)
   switch_portAutoGet()     -   Gets port auto status (ENABLE/DISABLE/RESTART)
   switch_portAutoSet()     -   Sets port auto (ENABLE/DISABLE/RESTART)

   STATISTICS

   switch_statPortGet()     -   retreives the statistic from the given port
   switch_statPortReset()   -   resets port statistic for the given port.

   VLAN MANAGEMENT

   switch_vlanCreate()            -   Creates VLAN with the given ID.
   switch_vlanDelete()            -   Deletes VLAN.
   switch_vlanPortAdd()           -   Adds port to VLAN.
   switch_vlanPortDelete()        -   Deletes port from VLAN.
   switch_vlanDefaultSet()        -   Sets default VLAN for the given port.
   switch_vlanPortUpdate()        -   Updates port status in VLAN.
   switch_vlanUpdate()            -   Updates VLAN.
   switch_vlantSetPortPriority()  -   Sets the priority of a port.
   switch_vlanGetPortPriority()   -   Gets the priority of a port.
   ******************************************************************************* */


/***********************************************************************************************
            SWITCH MANAGEMENT
***********************************************************************************************/
#ifdef EXTERN_SWITCH_INFO
#define PUBLIC
#else
#define PUBLIC extern
#endif

PUBLIC PHY_DEVICE *pSwitchInfo;
typedef Uint32 HANDLE;

#define MAX_PORT_NUM 5
/* Switch Identification */
typedef struct {
    Uint32 Model;
    Uint32 maxNumberOfPorts;
    Uint32 maxNumberOfVLANs;
    Uint32 maxNumberOfPriorities;       /* port based */
    Uint32 is802_1_Q_capable;
    Uint32 is802_1_P_capable;
    /* ...
       ... */
} SW_VER_INFO, *PSW_VER_INFO;


typedef enum {
    SWITCH_MARVELL_6060,
    SWITCH_MARVELL_6063,
    SWITCH_KENDIN_8995X,
    SWITCH_ADM_6995,
    SWITCH_RTL_8335
} SUPPORTED_SWITCHES_E;

extern int ext_switch_reset_gpio;

/*************************************************************************************************
* switch_reset
*
* DESCRIPTION:
*       Reset 
*
* INPUTS:
*       init - For startup, only release, cannot sleep (too early and not necessary)
*
*
***************************************************************************************************/
PAL_Result switch_reset(int init);

/*******************************************************************************
* switch_getVersion
*
* DESCRIPTION:
*       This function returns the version of the switch.
*
* INPUTS:
*       [Out] PSW_VER_INFO pSwitchInfo
*
*******************************************************************************/
PAL_Result switch_getVersion(PSW_VER_INFO pSwitchInfo);


/*******************************************************************************
* switch_init
*
* DESCRIPTION: Initialize the switch.- The internal Port Num,mii Base address and the channel Number.
*
* INPUTS:
*      [IN] PHY_DEVICE   - switchInit
*
* RETURNS:
*       handle to the switch driver.
*
*******************************************************************************/
PAL_Result switch_init(PHY_DEVICE * pSetSwitchInfo);

/*************************************************************************************************
* switch_exit
*
* DESCRIPTION:
*       Clean up memory
*
* INPUTS:
*
*
***************************************************************************************************/
PAL_Result switch_exit(void);

/***********************************************************************************************
            PORT MANAGEMENT
***********************************************************************************************/
typedef enum {
    PORT_LINK_UP = 1,
    PORT_LINK_DOWN,
} PORT_LINK;

/*
   Duplex status descriptor
 */

typedef enum {
    PORT_DUPLEX_HALF = 1,
    PORT_DUPLEX_FULL
} PORT_DUPLEX;

/*
   Speed status descriptor
 */

typedef enum {
    PORT_SPEED_10 = 1,
    PORT_SPEED_100,
    PORT_SPEED_1000
} PORT_SPEED;

/*
   Flow Control status descriptor
 */

typedef enum {
    /* autorecognition of the flow control (if exists) */
    PORT_FC_AUTO = 1,

    /* flow control on */
    PORT_FC_ON,

    /* flow control off */
    PORT_FC_OFF
} PORT_FC;

/*
   AutoNegotiation for duplex and speed status descriptor
 */

typedef enum {
    PORT_AUTO_YES = 1,
    PORT_AUTO_NO,
    PORT_AUTO_RESTART
} PORT_AUTO;

/* Port status */
typedef enum {
    /* Port Disabled */
    PORT_DISABLE = 1,

    /* Special frames only (BPDU e.t.c.) */
    PORT_BLOCK,

    /* Packets are dropped. Macs are learned */
    PORT_LEARNING,

    /* Forwarding - Normal switch mode */
    PORT_FORWARDING
} PORT_STATUS;

/* Ingress VLAN behaviour */
typedef enum {
    /* VLAN check is on (VLAN must exist). Ingress check is on (ingress port must be part of the VLAN).
       If not both drop the packet
     */
    INGRESS_VLN_MODE_8021Q_SECURE,

    /* VLAN check is on (VLAN must exist). If not drop the packet */
    INGRESS_VLN_MODE_8021Q_CHECK,

    /* Use Base Port VLAN only */
    INGRESS_VLN_MODE_8021Q_DISABLE
} INGRESS_VLN_MODE;



/* Extended features - unique for each switch model */
typedef struct switch_special_caps {
    /* VLAN tunneling on/off is the ability to forward packets to the dest port according to the address table
       even if this port is not part of the VLAN (if off the packet is dropped)
     */

    Bool VlanTunnel;

    INGRESS_VLN_MODE IngressMode;

    /* The ability to add the port Id to the packet itself, thus allowing the CPU to recognize source port */

    Bool PortRecognitionOn;

    /* The ability to send the frame to the given port */

    Bool TxOnFixedPort;

    /* ...
       ... */
} SWITCH_SPECIAL_CAPS, *PSWITCH_SPECIAL_CAPS;

/*******************************************************************************
*
* switch_specCapsGet -
*
* INPUTS:
*     [Out]   PSWITCH_SPECIAL_CAPS    PortCfg
*
*
* RETURNS:
*       Int32 - Ok/Error
*
*
*/
Int32 switch_specCapsGet(PSWITCH_SPECIAL_CAPS PortCfg   /* the port configuration structure */
    );

/*******************************************************************************
*
* switch_specCapsSet - Retreives a full configuration structure from the port
*
*
* INPUTS:
*     [In]    PSWITCH_SPECIAL_CAPS    PortCfg
*
* RETURNS:
*       Int32 - Ok/Error
*
*
*/

Int32 switch_specCapsSet(PSWITCH_SPECIAL_CAPS PortCfg   /* the port configuration structure */
    );


/*******************************************************************************
*
* switch_setPortBasedVlanTable
*
*
* INPUTS:
*     [In]    Uint8                   portId ( 1-6 )
*     [In]    Uint8                   mask (see user manual)
*
* RETURNS:
*       Int32 - Ok/Error
*
*
*/
Int32 switch_setPortBasedVlanTable(Uint8 portId, Uint8 mask);

/*******************************************************************************
*
* switch_trailerSet
*
*
* INPUTS:
*     [In]    Bool                   TrailerMode
*
* RETURNS:
*       Int32 - Ok/Error
*
*
*/
Int32 switch_trailerSet(Bool TrailerMode        /* trailer mode */
    );


/*******************************************************************************
*
* switch_disableLearning
*
*
* INPUTS:
*
*
* RETURNS:
*       Int32 - Ok/Error
*
*
*/

Int32 switch_learningDisable(void);

/*******************************************************************************
*
* switch_portStatusSet - enable the given port
*
* Enable/Disable a selected port
*
* INPUTS:
*     [In]    Uint32         PortNum
*     [In]    PORT_Int32     PortCfg
*
*
* The status can be:
*  PORT_ENABLE
*  PORT_DISABLE
*  PORT_BLOCK
*  PORT_LEARNING
*
* RETURNS:
*       Int32 - Ok/Error
*
*
*/

Int32 switch_portStatusSet(Uint32 PortNum,      /* port number */
                           PORT_STATUS status   /* the port status */
    );

/*******************************************************************************
*
* switch_portStatusGet - Returns the port status
*
* The API gets the port status On/Off.
*
* INPUTS:
*     [In]    Uint32         PortNum
*
*
* RETURNS: PORT_ENABLE/PORT_DISABLE
*
*/

PORT_STATUS switch_portStatusGet(Uint32 PortNum /* logical port number */
    );

/*******************************************************************************
*
* switch_portLinkGet - get port link status(Up/down)
*
* The function retrieves port link status.
*
* INPUTS:
*     [In]    Uint32         PortNum
*
* RETURNS: PORT_LINK - link up/down
*
*/

PORT_LINK switch_portLinkGet(Uint32 portNum     /* logical port number */
    );

/****************************************************************************
* switch_portFCSet - set flow control (On/off)
*
* Set flow control to on/off. all the ports in this trunk will be switched in this mode.
*
* * INPUTS:
*     [In]    Uint32         PortNum
*     [In]    PORT_FC         FCStatus
*
* The FCStatus can be:
*  PORT_FC_AUTO
*  PORT_FC_YES
*  PORT_FC_NO
*
* RETURNS: Int32 - Ok/Error
*
*/

Int32 switch_portFCSet(Uint32 portNum,  /* logical port number */
                       PORT_FC FCStatus /* flow control On/Off */
    );

/****************************************************************************
* switch_portFCGet - get flow control status(On/off)
*
* The function retrieves Flow Control status of the given port.
*
* INPUTS:
*     [In]    Uint32         PortNum
*
* RETURNS: PORT_FC -  Flow control On / Flow control Off
*
*/

PORT_FC switch_portFCGet(Uint32 portNum /* logical port number */
    );

/****************************************************************************
* switch_portDuplexSet - set port duplex (Full/Half)
*
* Set the duplex state of the port to Full/Half.
*
* INPUTS:
*     [In]    Uint32         PortNum
*     [In]    PORT_DUPLEX duplexStatus
*
* The duplexStatus can be:
*  PORT_DUPLEX_HALF
*  PORT_DUPLEX_FULL
*
* RETURNS: Int32 - Ok/Error
*
*/

Int32 switch_portDuplexSet(Uint32 portNum, PORT_DUPLEX duplexStatus);

/****************************************************************************
* switch_portDuplexGet - get port duplex status (Full/Half)
*
* Retrieves Duplex status (half/full).
*
* INPUTS:
*     [In]    Uint32         PortNum
*
* RETURNS: UND_PORT_DUPLEX - half/full
*
*
*/

PORT_DUPLEX switch_portDuplexGet(Uint32 portNum);

/****************************************************************************
* switch_portSpeedSet - set port speed (10/100)
*
* Set the current port to work in the given speed.
*
* INPUTS:
*     [In]    Uint32         PortNum
*     [In]    PORT_SPEED  speedStatus
*
* The speedStatus can be:
*  PORT_SPEED_10
*  PORT_SPEED_100
*
* RETURNS: Int32 - Ok/Error
*
*/

Int32 switch_portSpeedSet(Uint32 portNum, PORT_SPEED speedStatus);

/****************************************************************************
* switch_portSpeedGet - get port speed status(10/100)
*
* Retrieves current speed parameter for the given port.
*
* INPUTS:
*     [In]    Uint32         PortNum
*
* RETURNS: PORT_SPEED - 10/100
*
*/

PORT_SPEED switch_portSpeedGet(Uint32 portNum);


/****************************************************************************
* switch_portAutoSet - set port auto (ENABLE/DISABLE/RESTART)
*
* Set autonegotiation status for the given port, which is Yes/No/Restart.
*
* INPUTS:
*     [In]    Uint32         PortNum
*     [In]    PORT_AUTO       autoStatus
*
* autoStatus can be:
*  PORT_AUTO_YES
*  PORT_AUTO_NO
*  PORT_AUTO_RESTART
*
* RETURNS: Int32 - Ok/Error
*
*/

Int32 switch_portAutoSet(Uint32 portNum, PORT_AUTO autoStatus);

/****************************************************************************
* switch_portAutoGet - get port auto status (ENABLE/DISABLE/RESTART)
*
* Get the autonegotaiation for "speed/duplex" status, which is Yes/No.
*
* INPUTS:
*     [In]    Uint32         PortNum
*
* RETURNS: PORT_AUTO - YES/NO
*
*/

PORT_AUTO switch_portAutoGet(Uint32 portNum);


/***********************************************************************************************
            S T A T I S T I C S
***********************************************************************************************/


typedef struct {
    Uint32 ethAlignmentErrors;
    Uint32 ethFCSErrors;
    Uint32 ethSingleCollisions;
    Uint32 ethMultipleCollisions;
    Uint32 ethSQETestErrors;
    Uint32 ethDeferredTxFrames;
    Uint32 ethLateCollisions;
    Uint32 ethExcessiveCollisions;
    Uint32 ethInternalMacTxErrors;
    Uint32 ethCarrierSenseErrors;
    Uint32 ethTooLongRxFrames;
    Uint32 ethInternalMacRxErrors;
    Uint32 ethSymbolErrors;

} SWITCH_COUNTERS, *PSWITCH_COUNTERS;

/**************************************************************************/
/*                 MIB-2 byte/packet Counter Types                        */
/**************************************************************************/
typedef struct {
    Uint32 InBytes;
    Uint32 InBytesHigh;
    Uint32 InUnicastPkts;
    Uint32 InUnicastPktsHigh;
    Uint32 InMulticastPkts;
    Uint32 InMulticastPktsHigh;
    Uint32 InBroadcastPkts;
    Uint32 InBroadcastPktsHigh;
    Uint32 InDiscardPkts;
    Uint32 InErrorPkts;
    Uint32 InUnknownProtPkts;
    Uint32 OutBytes;
    Uint32 OutBytesHigh;
    Uint32 OutUnicastPkts;
    Uint32 OutUnicastPktsHigh;
    Uint32 OutMulticastPkts;
    Uint32 OutMulticastPktsHigh;
    Uint32 OutBroadcastPkts;
    Uint32 OutBroadcastPktsHigh;
    Uint32 OutDiscardPkts;
    Uint32 OutErrorPkts;
    SWITCH_COUNTERS swCounters;
} MIB2_COUNTER, *PMIB2_COUNTER;


/**************************************************************************************
*
* switch_statPortGet - retreives the statistic from the given port
*
* Retreives the statistic from the given port.
*
* INPUTS:
*     [In]    Uint32         PortNum
*
* RETURNS: Int32 - Ok/Error
*
*/

Int32 switch_statPortGet(Uint32 PortNum,        /* logical port number */
                         PMIB2_COUNTER counters /* output structure with all the
                                                   relevant counters for the given port  */
    );

/**************************************************************************************
*
* switch_statPortReset - resets port statistic for the given port.
*
* Resets the statistic on given port.
*
* INPUTS:
*     [In]    Uint32         PortNum
*
* RETURNS: Int32 - Ok/Error
*
*/

Int32 switch_statPortReset(Uint32 PortNum       /* logical port to reset statistic to */
    );


/***********************************************************************************************
                                VLAN SUBMODULE
************************************************************************************************/

/* group ID type */

typedef Int32 VLN_GROUP_ID;


/* VLAN Tagging */
typedef enum
{
    VLN_PORT_UNTAG,
    VLN_PORT_TAG,
    VLN_PORT_UNMODIFIED,
    VLN_PORT_HYBRID,
}
VLN_TAG;


/**************************************************************************************
*
* switch_vlanCreate - Create VLAN.
*
* Create logical VLAN. No real updates are performed.\
*
* INPUTS:
*     [In]    Uint32         vlnId
*
* RETURNS: Int32 - Ok/Error
*
*/

Int32 switch_vlanCreate(Uint32 vlnId    /* VLAN Id */
    );

/**************************************************************************************
*
* switch_vlanDelete - Destroy the existing VLAN.
*
* Deletes the VLAN
*
* INPUTS:
*     [In]    Uint32         vlnId
*
* RETURNS: Int32 - Ok/Error
*
*/

Int32 switch_vlanDelete(Uint32 vlnId);

/**************************************************************************************
*
* switch_vlanPortAdd - add port to VLAN.
*
* Adds port to the existing VLAN.
*
* INPUTS:
*     [In]    Uint32         PortNum
*     [In]    Uint32         tag
*
* RETURNS: Int32 - Ok/Error
*
*/

Int32 switch_vlanPortAdd(Uint32 vlnId,  /* VLAN Id */
                         Uint32 portNum,        /* logical port number */
                         VLN_TAG tag    /* port tagged/untagged */
    );

/**************************************************************************************
*
* switch_vlanPortDelete - remove port from VLAN.
*
* Remove port from the existing VLAN.
*
* INPUTS:
*     [In]    Uint32         PortNum
*     [In]    Uint32         vlnId
*
* RETURNS: Int32 - Ok/Error
*
*/

Int32 switch_vlanPortDelete(Uint32 portNum,     /* port number */
                            Uint32 vlnId        /* VLAN Id */
    );

/**************************************************************************************
*
* switch_vlanPortUpdate - updates port status in VLAN
*
* Updates port to tagged/untagged in VLAN.
*
* INPUTS:
*     [In]    Uint32         PortNum
*     [In]    Uint32         vlnId
*     [In]    VLN_TAG         tag
*
* RETURNS: Int32 - Ok/Error
*
*/

Int32 switch_vlanPortUpdate(Uint32 PortNum,     /* logical port number */
                            Uint32 vlnId,       /* logical VLAN number in which the port should be updated */
                            VLN_TAG tag /* new tag state */
    );

/**************************************************************************************
*
* switch_vlanDefaultSet  -   Sets default VLAN for the given port.
*
* Updates port to tagged/untagged in VLAN.
*
* INPUTS:
*     [In]    Uint32       PortNum
*     [In]    Uint32         vlnId
*
* RETURNS: Int32 - Ok/Error
*
*/

Int32 switch_vlanDefaultSet(Uint32 PortNum,     /* logical port number */
                            Uint32 vlnId        /* logical VLAN number in which the port should be updated */
    );


/*******************************************************************************
*
* switch_vlanUpdate - Update logical VLAN.
*
* Update of the logical VLAN.
*
* INPUTS:
*     [In]    Uint32       priority
*     [In]    Uint32         vlnId
*
* RETURNS: OK OR ERROR
*
*/

Int32 switch_vlanUpdate(Uint32 priority,        /* 0 ..7 */
                        Uint32 vlnId    /* VLAN Number */
    );

/*******************************************************************************
*
* switch_vlanSetPortPriority -  Set the priority of a port
*
* INPUTS:
*     [In]    Uint32         portNum
*     [In]    Uint32         priority
*
* RETURNS: OK OR ERROR
*
*/

Int32 switch_vlanSetPortPriority(Uint32 PortNum,        /* Port Number */
                                 Uint32 priority        /* switch specific */
    );

/*******************************************************************************
*
* switch_vlanGetPortPriority -  Get the priority of a port
*
* INPUTS:
*     [In]    Uint32         portNum
*
* RETURNS: OK OR ERROR
*
*/

Uint32 switch_vlanGetPortPriority(Uint32 PortNum        /* Port Number */
    );

/* Purge an entry from the address translation Unit table. */
Int32 switch_deleteMacAddrFromAtu(unsigned char MacAdd[6]       /* Mac Address   */
    );

Int32 switch_trailerSet(Bool mode);

/*******************************************************************************
* switch_dbgRegRead
*******************************************************************************/
PAL_Result switch_dbgRegRead( Uint32 addr, Uint32 * data );

/*******************************************************************************
* switch_dbgRegWrite
*******************************************************************************/
PAL_Result switch_dbgRegWrite( Uint32 addr, Uint32 * data );


#endif /*__SWITCH_INTERFACE_H__*/
