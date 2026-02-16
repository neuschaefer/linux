/*
 *
 * ddc_switch.c
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


#include <linux/delay.h>

#include "_tistdtypes.h"
#include <pal.h>
#include "ddc_cpgmac_f_Drv.h"

#define EXTERN_SWITCH_INFO
#include "ddc_switch.h"
#include "csl_switchPrvt.h"

PCSL_SWITCH_CLBKS        pCslSwitchClbks;
SWITCH_SPECIAL_CAPS      specCaps;

/**
 * \brief The following  macro return False if !expr  */
#define RESULT_ASSERT(expr) \
    if(!(expr)) \
        return PAL_False

/*************************************************************************************************
* switch_init
*
* DESCRIPTION:
*       Adds and software initialize another switch
*
* INPUTS:
*       [In] PSW_VER_INFO pSwitchInfo
*
***************************************************************************************************/
PAL_Result switch_init(PHY_DEVICE * pSetSwitchInfo)
{
    if (pSetSwitchInfo == NULL)
    {
        return PAL_False;
    }

    if (PAL_osMemAlloc(0,sizeof(CSL_SWITCH_CLBKS),0,(Ptr *) &pCslSwitchClbks) != PAL_SOK)
    {
        return PAL_False;
    }

    PAL_osMemSet(pCslSwitchClbks,0,sizeof(CSL_SWITCH_CLBKS));

    pSwitchInfo = pSetSwitchInfo; /* Point to the CPMAC DDC->PhyDev handler */

    if (cslConnectSwitch(pCslSwitchClbks) == PAL_False)
    {
        PAL_osMemFree(0,(Ptr *)pCslSwitchClbks,sizeof(CSL_SWITCH_CLBKS));
        return PAL_False;
    }
	
    /* check for mandatory functionality */
    if(
       (pCslSwitchClbks->cslOperStatus2regVal == NULL) || 
       (pCslSwitchClbks->cslPortNum2PhyAddr   == NULL) ||
       (pCslSwitchClbks->cslPortNum2RegAddr   == NULL) ||
       (pCslSwitchClbks->cslRegVal2operStatus == NULL) ||
       (pCslSwitchClbks->cslSwitchGetVersion  == NULL) ||
	   (pCslSwitchClbks->cslSwitchStart       == NULL)
      )
    {
       PAL_osMemFree(0,(Ptr *)pCslSwitchClbks,sizeof(CSL_SWITCH_CLBKS));
       return PAL_False;  
    }

    pCslSwitchClbks->cslSwitchStart();

    return PAL_True;
}

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
PAL_Result switch_exit(void)
{
   PAL_osMemFree(0,(Ptr *)pCslSwitchClbks,sizeof(CSL_SWITCH_CLBKS));
   return PAL_True;
}

/**********************************************************************************************************/

/*************************************************************************************************
* switch_reset
*
* DESCRIPTION:
*       Reset 
*
* INPUTS:
*       init - For startup, only release, cannot sleep (too early and not necessary)
*
***************************************************************************************************/
PAL_Result switch_reset(int init)
{
    int i;

    if ( -1 != ext_switch_reset_gpio )
    {
        if (init)
        {
            /* Necessary only once */
            PAL_sysGpioCtrl( ext_switch_reset_gpio, GPIO_PIN, GPIO_OUTPUT_PIN );
        }
        /* Toggle the pin, unless init */
        if (!init)
        {
            /* Into reset */
            printk(KERN_DEBUG "\nSWITCH --> Reset: gpio %d, val 0\n", ext_switch_reset_gpio);
            PAL_sysGpioOutBit( ext_switch_reset_gpio, 0 );
            /* Wait 10Msec */
            msleep(10);
        }
        /* Take the external switch out of reset  */
        printk(KERN_DEBUG "\nSWITCH --> OUT of Reset: gpio %d, val 1\n", ext_switch_reset_gpio);
        PAL_sysGpioOutBit( ext_switch_reset_gpio, 1 );
    }

   return PAL_True;
}

/**********************************************************************************************************/



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
PAL_Result switch_getVersion(PSW_VER_INFO pSwitchVerInfo )
{

    pCslSwitchClbks->cslSwitchGetVersion(pSwitchVerInfo);

    return PAL_True;
}

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
PAL_Result switch_dbgRegRead( Uint32 addr, Uint32 * data )
{
    if (pCslSwitchClbks->cslRegGet)
    {
        pCslSwitchClbks->cslRegGet( 0, addr, data, 4 );
    }
    return PAL_True;
}

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
PAL_Result switch_dbgRegWrite( Uint32 addr, Uint32 * data )
{
    if (pCslSwitchClbks->cslRegSet)
    {
        pCslSwitchClbks->cslRegSet( 0, addr, data, 4 );
    }
    return PAL_True;
}


/*********************************************************************************************************
*
* switch_portStatusSet - enable the given port
*
* Enable/Disable a selected port
*
* INPUTS:
*     [In]    Uint32         PortNum
*     [In]    PORT_STATUS     PortCfg
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

Int32 switch_portStatusSet
    (
        Uint32 PortNum,          /* port number     */
        PORT_STATUS status       /* the port status */
    )
{
    Uint16 data;
    Uint32 regaddr,phyaddr;

    RESULT_ASSERT(PortNum < 6);

    /* PORT_STATUS enum Start from 1 - the register values start from 0 */
    status--;
    /* calculate the target address */
    regaddr = pCslSwitchClbks->cslPortNum2RegAddr(OPER_STATUS,PortNum);
    phyaddr = pCslSwitchClbks->cslPortNum2PhyAddr(OPER_STATUS,PortNum);

    /* here is read modify write sequence */

    data = _cpswHalCommonMiiMdioUserAccessRead(pSwitchInfo,regaddr, phyaddr);

    /* data - In, data - Out  */
    data = pCslSwitchClbks->cslOperStatus2regVal(OPER_STATUS,(Int32) status,data);

    _cpswHalCommonMiiMdioUserAccessWrite(pSwitchInfo,regaddr, phyaddr,data);

    return PAL_True;

}

/*********************************************************************************************************
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

PORT_STATUS switch_portStatusGet
    (
        Uint32    PortNum  /* logical port number */
    )
{
    Uint16 data;
    PORT_STATUS portStatus;
    Uint32 regaddr,phyaddr;

    RESULT_ASSERT(PortNum < 6);

    /* calculate the target address */

    regaddr = pCslSwitchClbks->cslPortNum2RegAddr(OPER_STATUS,PortNum);

    phyaddr = pCslSwitchClbks->cslPortNum2PhyAddr(OPER_STATUS,PortNum);

    data = _cpswHalCommonMiiMdioUserAccessRead(pSwitchInfo,regaddr, phyaddr);
    /* PORT_STATUS enum Start from 1 - the register values start from 0 */
    portStatus = (PORT_STATUS) (pCslSwitchClbks->cslRegVal2operStatus(OPER_STATUS,data) + 1);

    return portStatus;

}

/***********************************************************************************************************
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

PORT_LINK switch_portLinkGet
    (
        Uint32 PortNum /* logical port number */
    )
{
    Uint16 data;
    PORT_LINK portLink;
    Uint32 regaddr,phyaddr;

    RESULT_ASSERT(PortNum < 6);

    /* calculate the target address */
    regaddr = pCslSwitchClbks->cslPortNum2RegAddr(OPER_LINK,PortNum);
    phyaddr = pCslSwitchClbks->cslPortNum2PhyAddr(OPER_LINK,PortNum);

    data = _cpswHalCommonMiiMdioUserAccessRead(pSwitchInfo,regaddr, phyaddr);

    portLink = (PORT_LINK) pCslSwitchClbks->cslRegVal2operStatus(OPER_LINK, data);

    return portLink;
}

/***********************************************************************************************************
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

Int32 switch_portFCSet
    (
        Uint32 PortNum,      /* logical port number */
        PORT_FC FCStatus     /* flow control On/Off */
    )
{
    Uint16 data;
    Uint32 regaddr,phyaddr;

    RESULT_ASSERT(PortNum < 6);

    /* calculate the target address */

    regaddr = pCslSwitchClbks->cslPortNum2RegAddr(OPER_FC,PortNum);
    phyaddr = pCslSwitchClbks->cslPortNum2PhyAddr(OPER_FC,PortNum);

    data = _cpswHalCommonMiiMdioUserAccessRead(pSwitchInfo,regaddr, phyaddr);

    data = pCslSwitchClbks->cslOperStatus2regVal(OPER_FC,(PORT_FC) FCStatus,data);

    _cpswHalCommonMiiMdioUserAccessWrite(pSwitchInfo,regaddr, phyaddr,data);

    return PAL_True;
}

/*********************************************************************************************************
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

PORT_FC switch_portFCGet
    (
        Uint32 PortNum /* logical port number */
    )
{
    Uint16 data;
    PORT_FC portFC;
    Uint32 regaddr,phyaddr;

    RESULT_ASSERT(PortNum < 6);

    /* calculate the target address */

    regaddr = pCslSwitchClbks->cslPortNum2RegAddr(OPER_FC,PortNum);
    phyaddr = pCslSwitchClbks->cslPortNum2PhyAddr(OPER_FC,PortNum);

    data = _cpswHalCommonMiiMdioUserAccessRead(pSwitchInfo,regaddr, phyaddr);

    portFC = (PORT_FC) pCslSwitchClbks->cslRegVal2operStatus(OPER_FC,data);

    return portFC;

}

/*************************************************************************************************************
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

Int32 switch_portDuplexSet
    (
        Uint32 PortNum,
        PORT_DUPLEX duplexStatus
    )
{
    Uint16 data;
    Uint32 regaddr,phyaddr;

    RESULT_ASSERT(PortNum < 6);
    /* calculate the target address */

    regaddr = pCslSwitchClbks->cslPortNum2RegAddr(OPER_DUPLEX,PortNum);
    phyaddr = pCslSwitchClbks->cslPortNum2PhyAddr(OPER_DUPLEX_SET,PortNum);

    data = _cpswHalCommonMiiMdioUserAccessRead(pSwitchInfo,regaddr, phyaddr);

    data = pCslSwitchClbks->cslOperStatus2regVal(OPER_DUPLEX_SET,(Int32) duplexStatus,data);

    _cpswHalCommonMiiMdioUserAccessWrite(pSwitchInfo,regaddr, phyaddr,data);

    return PAL_True;

}

/*********************************************************************************************************
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

PORT_DUPLEX switch_portDuplexGet
    (
        Uint32 PortNum
    )
{
    Uint16 data;
    PORT_DUPLEX portDuplex;
    Uint32 regaddr,phyaddr;

    RESULT_ASSERT(PortNum < 6);

    /* calculate the target address */

    regaddr = pCslSwitchClbks->cslPortNum2RegAddr(OPER_DUPLEX,PortNum);
    phyaddr = pCslSwitchClbks->cslPortNum2PhyAddr(OPER_DUPLEX,PortNum);

    data = _cpswHalCommonMiiMdioUserAccessRead(pSwitchInfo,regaddr, phyaddr);
    portDuplex = (PORT_DUPLEX) pCslSwitchClbks->cslRegVal2operStatus(OPER_DUPLEX,data);

    return portDuplex;

}

/***********************************************************************************************************
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

Int32 switch_portSpeedSet
    (
        Uint32 PortNum,
        PORT_SPEED  speedStatus
    )
{
    Uint16 data;
    Uint32 regaddr,phyaddr;

    RESULT_ASSERT(PortNum < 6);

    /* calculate the target address */

    regaddr = pCslSwitchClbks->cslPortNum2RegAddr(OPER_SPEED,PortNum);
    phyaddr = pCslSwitchClbks->cslPortNum2PhyAddr(OPER_SPEED_SET,PortNum);

    data = _cpswHalCommonMiiMdioUserAccessRead(pSwitchInfo,regaddr, phyaddr);

    data = pCslSwitchClbks->cslOperStatus2regVal(OPER_SPEED_SET,(Int32) speedStatus,data);

    _cpswHalCommonMiiMdioUserAccessWrite(pSwitchInfo,regaddr, phyaddr,data);

    return PAL_True;

}

/*********************************************************************************************************
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

PORT_SPEED switch_portSpeedGet
    (
        Uint32 PortNum
    )
{
    Uint16 data;
    PORT_SPEED portSpeed;
    Uint32 regaddr,phyaddr;

    RESULT_ASSERT(PortNum < 6);

    /* calculate the target address */

    regaddr = pCslSwitchClbks->cslPortNum2RegAddr(OPER_SPEED,PortNum);
    phyaddr = pCslSwitchClbks->cslPortNum2PhyAddr(OPER_SPEED,PortNum);

    data = _cpswHalCommonMiiMdioUserAccessRead(pSwitchInfo,regaddr, phyaddr);

    portSpeed = (Int32) pCslSwitchClbks->cslRegVal2operStatus(OPER_SPEED,data);

    return portSpeed;
}


/************************************************************************************************************
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

Int32 switch_portAutoSet
    (
        Uint32 PortNum,
        PORT_AUTO  autoStatus
    )
{
    Uint16 data;
    Uint32 regaddr,phyaddr;

    RESULT_ASSERT(PortNum < 6);

    /* calculate the target address */

    regaddr = pCslSwitchClbks->cslPortNum2RegAddr(OPER_AUTO,PortNum);
    phyaddr = pCslSwitchClbks->cslPortNum2PhyAddr(OPER_AUTO,PortNum);

    data = _cpswHalCommonMiiMdioUserAccessRead(pSwitchInfo,regaddr, phyaddr);

    data = pCslSwitchClbks->cslOperStatus2regVal(OPER_AUTO,(Int32) autoStatus,data);

    _cpswHalCommonMiiMdioUserAccessWrite(pSwitchInfo,regaddr, phyaddr,data);

    return PAL_True;

}

/**********************************************************************************************************
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

PORT_AUTO switch_portAutoGet
    (
        Uint32 PortNum
    )
{
    Uint16 data;
    PORT_AUTO portAuto;
    Uint32 regaddr,phyaddr;

    RESULT_ASSERT(PortNum < 6);

    /* calculate the target address */

    regaddr = pCslSwitchClbks->cslPortNum2RegAddr(OPER_AUTO,PortNum);
    phyaddr = pCslSwitchClbks->cslPortNum2PhyAddr(OPER_AUTO,PortNum);

    data = _cpswHalCommonMiiMdioUserAccessRead(pSwitchInfo,regaddr, phyaddr);

    portAuto = (PORT_AUTO) pCslSwitchClbks->cslRegVal2operStatus(OPER_AUTO,data);

    return portAuto;

}


/**********************************************************************************************************
            S T A T I S T I C S
***********************************************************************************************************/

/**********************************************************************************************************
*
* switch_statPortGet - retreives all the statistic from the given port
*
* Retreives the statistic from the given port.
*
* INPUTS:
*     [In]    Uint32         PortNum
*
* RETURNS: Int32 - Ok/Error
*
*/

Int32 switch_statPortGet
    (
        Uint32         PortNum, /* logical port number*/
        PMIB2_COUNTER  counters /* output structure with all the
                                            relevant counters for the given port  */
    )
{
#define OFFSET_FROM_STRUCT(x)  x * (sizeof(Uint32))

    SWITCH_STAT_E  statIndex;


    /* calculate the target address */

    if((pCslSwitchClbks->cslPrepareReadStatSequence == NULL) || (pCslSwitchClbks->cslGetPreparedCounters == NULL))
        return PAL_False;

    pCslSwitchClbks->cslPrepareReadStatSequence(PortNum);

    for(statIndex = InBytes;statIndex <= ethSymbolErrors;statIndex++)
    {
        pCslSwitchClbks->cslGetPreparedCounters(statIndex,(Uint32*)((Uint32)counters + OFFSET_FROM_STRUCT(statIndex)));
    }

    return PAL_True;
}

/**********************************************************************************************************
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

Int32 switch_statPortReset
    (
        Uint32 PortNum /* logical port to reset statistic to*/
    )
{
    Uint16 data = 0;
    Uint32 regaddr,phyaddr;


    RESULT_ASSERT(PortNum < 6);

    regaddr = pCslSwitchClbks->cslPortNum2RegAddr(OPER_RESET_STAT,PortNum);
    phyaddr = pCslSwitchClbks->cslPortNum2PhyAddr(OPER_RESET_STAT,PortNum);

    data = pCslSwitchClbks->cslOperStatus2regVal(OPER_RESET_STAT,PortNum,data);

    _cpswHalCommonMiiMdioUserAccessWrite(pSwitchInfo,regaddr, phyaddr,data);

    return PAL_True;
}

/*************************************************************************************************************
                                VLAN SUBMODULE
**************************************************************************************************************/

/*************************************************************************************************************
*
* switch_vlanCreate - Create VLAN.
*
* Create logical VLAN. No real updates are performed.
*
* INPUTS:
*     [In]    Uint32         vlnId
*
* RETURNS: Int32 - Ok/Error
*
*/

Int32 switch_vlanCreate
    (
        Uint32              vlnId         /* VLAN Id */
    )
{
    CSL_SWITCH_VLAN_ENTRY * elem;

    RESULT_ASSERT(vlnId < 0xFFF);

    if (pCslSwitchClbks->cslVlnUpdate == NULL)
        return PAL_False;

    /* non-critical error VLAN already exists */

    elem = cslGetVLANEntry(vlnId);
    RESULT_ASSERT(elem == NULL);

    elem = cslCreateVLANEntry();
    RESULT_ASSERT(elem != NULL);

    elem->valid = 1;
    elem->vid = vlnId;

    if (NULL != pCslSwitchClbks->vlan_creat)        /* 0x610 */
    {
        pCslSwitchClbks->vlan_creat(0,elem->vid);
    }

    if (NULL != pCslSwitchClbks->vlan_learning_state_set)
    {
//        pCslSwitchClbks->vlan_learning_state_set(0,elem->vid,False);
    }

    return PAL_True;
}

/**************************************************************************************************************
*
* switch_vlanDelete - Destroy the existing VLAN.
*
* Deletes the VLAN from VLAN table.
*
* INPUTS:
*     [In]    Uint32         vlnId
*
* RETURNS: Int32 - Ok/Error
*
*/

Int32 switch_vlanDelete
    (
        Uint32              vlnId
    )
{
    CSL_SWITCH_VLAN_ENTRY * elem;


    if(pCslSwitchClbks->cslVlnUpdate == NULL)
       return PAL_False;

    /* non-critical error VLAN doesn't exist*/

    elem = cslGetVLANEntry(vlnId);

    RESULT_ASSERT(elem != NULL);

    elem->valid = 0;
    pCslSwitchClbks->cslVlnUpdate(elem);

    if (NULL != pCslSwitchClbks->vlan_delete)
    {
        pCslSwitchClbks->vlan_delete(0,elem->vid);
    }

    cslDestroyVLANEntry(elem);
    return PAL_True;
}

/*************************************************************************************************************
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

Int32 switch_vlanPortAdd
    (
        Uint32              vlnId,         /* VLAN Id */
        Uint32              portNum,       /* logical port number */
        VLN_TAG             tag            /* port tagged/untagged */
    )
{
    CSL_SWITCH_VLAN_ENTRY * elem;
    Uint32 currPortMask = (1 << portNum);

    RESULT_ASSERT(portNum < 6);

    /* non-critical error VLAN doesn't exist*/

    if(pCslSwitchClbks->cslVlnUpdate == NULL)
       return PAL_False;

    elem = cslGetVLANEntry(vlnId);

    RESULT_ASSERT(elem != NULL);

    elem->mem_ports |= currPortMask;

    switch (tag)
    {
        case VLN_PORT_UNTAG:
            elem->untagged_ports |=   currPortMask;
            elem->tagged_ports   &= (~currPortMask);
            elem->unmodify_ports &= (~currPortMask);
            break;
        case VLN_PORT_TAG:
            elem->tagged_ports   |=   currPortMask;
            elem->untagged_ports &= (~currPortMask);
            elem->unmodify_ports &= (~currPortMask);
            break;
        case VLN_PORT_UNMODIFIED:
            elem->unmodify_ports |=   currPortMask;
            elem->untagged_ports &= (~currPortMask);
            elem->tagged_ports   &= (~currPortMask);
            break;
        default:
            return PAL_False;
    }


    pCslSwitchClbks->cslVlnUpdate(elem);

    if (pCslSwitchClbks->vlan_member_add)           /* 0x610 */
    {
        pCslSwitchClbks->vlan_member_add(0,elem->vid, portNum, tag);
    }

    if (pCslSwitchClbks->port_1qmode_set)           /* 0x660 */
    {
        pCslSwitchClbks->port_1qmode_set(0,portNum, CSL_1Q_CHECK);
    }

    if (pCslSwitchClbks->port_egvlanmode_set)       /* 0x424, 0xc80 */
    {
        CSL_SWITCH_VLAN_ENTRY portSuperSet;
        CSL_SWITCH_VLAN_ENTRY * tmp;

        portSuperSet.tagged_ports   =
        portSuperSet.untagged_ports =
        portSuperSet.unmodify_ports = 0;

        for (tmp = cslGetVLANtable();tmp != NULL; tmp = tmp->next)
        {
            portSuperSet.tagged_ports      |= (tmp->tagged_ports   & currPortMask);
            portSuperSet.untagged_ports    |= (tmp->untagged_ports & currPortMask);
            portSuperSet.unmodify_ports    |= (tmp->unmodify_ports & currPortMask);
        }

        if (portSuperSet.untagged_ports && portSuperSet.unmodify_ports)
        {
            pCslSwitchClbks->port_egvlanmode_set(0,portNum, VLN_PORT_UNMODIFIED);
        }
        else
        if (portSuperSet.tagged_ports && portSuperSet.unmodify_ports)
        {
            pCslSwitchClbks->port_egvlanmode_set(0,portNum, VLN_PORT_UNMODIFIED);
        }
        else
        if (portSuperSet.tagged_ports && portSuperSet.untagged_ports)
        {
            pCslSwitchClbks->port_egvlanmode_set(0,portNum, VLN_PORT_UNMODIFIED);
        }
        else
        {
            pCslSwitchClbks->port_egvlanmode_set(0,portNum, tag);
        }
    }

    if (pCslSwitchClbks->portvlan_member_update)    /* 0x660 */
    {
        Uint32 portIdx = 0;

        for (portIdx=0; portIdx < 7 ;portIdx++)
        {
            if ((1 << portIdx) & elem->mem_ports)
            {
                Uint32 portBitmap = elem->mem_ports & (~(1 << portIdx));

                pCslSwitchClbks->portvlan_member_update(0,portIdx,portBitmap);
            }
        }
    }

    if (pCslSwitchClbks->port_default_cvid_set)     /* 0x420 */
    {
        if (tag == VLN_PORT_TAG)
        {
            pCslSwitchClbks->port_default_cvid_set(0,portNum, 0);  // set the default CVID
        }
        else
        {
            pCslSwitchClbks->port_default_cvid_set(0,portNum, elem->vid);  // set the default CVID
        }
    }


    return PAL_True;
}

/*******************************************************************************************************
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

Int32 switch_vlanPortDelete
    (
        Uint32              portNum,       /* port number */
        Uint32              vlnId         /* VLAN Id */


    )
{
    CSL_SWITCH_VLAN_ENTRY * elem;

    RESULT_ASSERT(portNum < 8);

    /* non-critical error VLAN doesn't exist*/
    if(pCslSwitchClbks->cslVlnUpdate == NULL)
       return PAL_False;

    elem = cslGetVLANEntry(vlnId);

    RESULT_ASSERT(elem != NULL);

    elem->mem_ports &= (~(1 << portNum));

    pCslSwitchClbks->cslVlnUpdate(elem);


    if (pCslSwitchClbks->vlan_member_del)           /* 0x610 */
    {
        pCslSwitchClbks->vlan_member_del(0,elem->vid, portNum);
    }

    if (pCslSwitchClbks->portvlan_member_update)    /* 0x660 */
    {
        Uint32 portIdx = 0;

        for (portIdx=0; portIdx < 7 ;portIdx++)
        {
            if (((1 << portIdx) & elem->mem_ports) ||
                (portNum == portIdx))
            {
                Uint32 portBitmap = elem->mem_ports & (~(1 << portIdx));

                pCslSwitchClbks->portvlan_member_update(0,portIdx,portBitmap);
            }
        }
    }

    return PAL_True;
}

/**********************************************************************************************************
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

Int32 switch_vlanPortUpdate
    (
        Uint32       portNum,        /* logical port number */
        Uint32       vlnId,          /* logical VLAN number in which the port should be updated*/
        VLN_TAG      tag             /* new tag state */
    )
{

    CSL_SWITCH_VLAN_ENTRY * elem;

    RESULT_ASSERT(portNum < 6);

    /* non-critical error VLAN doesn't exist*/
    if(pCslSwitchClbks->cslVlnUpdate == NULL)
       return PAL_False;

    elem = cslGetVLANEntry(vlnId);

    RESULT_ASSERT(elem != NULL);

    /* If this port is not a part of this VLAN return error */
    if(!(elem->mem_ports & (1 << portNum )))
       return PAL_False;

    if (tag == VLN_PORT_UNTAG)
        elem->tagged_ports &= (~(1 << portNum));

    else
        elem->tagged_ports |= (1 << portNum);

    pCslSwitchClbks->cslVlnUpdate(elem);

    return PAL_True;
}

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

Int32 switch_vlanDefaultSet
    (
        Uint32       portNum,       /* logical port number */
        Uint32       vlnId          /* logical VLAN number in which the port should be updated*/

    )
{
    CSL_SWITCH_VLAN_ENTRY * elem;


    RESULT_ASSERT(portNum < 6);

    /* non-critical error VLAN doesn't exist*/

    elem = cslGetVLANEntry(vlnId);

    RESULT_ASSERT(elem != NULL);

    if(pCslSwitchClbks->cslVlnDefaultSet != NULL)
        pCslSwitchClbks->cslVlnDefaultSet(portNum,elem);

    return PAL_True;
}


/*******************************************************************************
*
* switch_vlanSetPortPriority -  Set the priority of a port
*
* INPUTS:
*     [In]    Uint32         portNum
*     [In]    Uint32         priority
*
* RETURNS: PAL_True OR PAL_False
*
*/

Int32 switch_vlanSetPortPriority
    (
        Uint32 PortNum,    /* Port Number */
        Uint32 priority    /* switch specific */
    )
{


    RESULT_ASSERT(PortNum < 6);

    if(pCslSwitchClbks->cslVlnPrioritySet != NULL)
        pCslSwitchClbks->cslVlnPrioritySet(PortNum,priority);

    return PAL_True;
}

/*******************************************************************************
*
* switch_vlanGetPortPriority -  Get the priority of a port
*
* INPUTS:
*     [In]    Uint32         portNum
*
* RETURNS: PAL_True OR PAL_False
*
*/

Uint32 switch_vlanGetPortPriority
    (
        Uint32 PortNum              /* Port Number */
    )
{



    RESULT_ASSERT(PortNum < 6);

    if(pCslSwitchClbks->cslVlnPriorityGet != NULL)
        return pCslSwitchClbks->cslVlnPriorityGet(PortNum);

    return PAL_True;
}

/*******************************************************************************
*
* switch_specCapsGet - gets special capabilities configuration of the switch
*
* INPUTS:
*     [Out]   PSWITCH_SPECIAL_CAPS   pSwitchSpecialCaps
*
*
* RETURNS:
*       Int32 - Ok/Error
*
*
*/

PAL_Result switch_specCapsGet
    (
        PSWITCH_SPECIAL_CAPS    pSwitchSpecialCaps

    )
{

    pSwitchSpecialCaps->IngressMode         = specCaps.IngressMode;
    pSwitchSpecialCaps->PortRecognitionOn   = specCaps.PortRecognitionOn;
    pSwitchSpecialCaps->TxOnFixedPort       = specCaps.TxOnFixedPort;
    pSwitchSpecialCaps->VlanTunnel          = specCaps.VlanTunnel;

    return PAL_True;
}

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
Int32 switch_setPortBasedVlanTable( Uint8 portId, Uint8 mask )
{
  /* Send port mask multiplexed with port ID */
  return pCslSwitchClbks->cslVlnCapsSet((Uint32)OPER_VLAN_TABLE_SET, ( ( mask & 0x7F ) | ( portId << 12 ) ) );
}


/*******************************************************************************
*
* switch_specCapsSet - sets special capabilities configuration of the switch
*
*
* INPUTS:
*     [In]    PSWITCH_SPECIAL_CAPS   pSwitchSpecialCaps
*
* RETURNS:
*       Int32 - Ok/Error
*
*
*/

Int32 switch_specCapsSet
    (
        PSWITCH_SPECIAL_CAPS    pSwitchSpecialCaps  /* the port configuration structure */
    )
{
    pCslSwitchClbks->cslVlnCapsSet((Uint32)OPER_INGRESS,pSwitchSpecialCaps->IngressMode);
    specCaps.IngressMode = pSwitchSpecialCaps->IngressMode;
    pCslSwitchClbks->cslVlnCapsSet((Uint32)OPER_PORT_RECOGNITION_ON,pSwitchSpecialCaps->PortRecognitionOn);
    specCaps.PortRecognitionOn = pSwitchSpecialCaps->PortRecognitionOn;
    pCslSwitchClbks->cslVlnCapsSet((Uint32)OPER_TRANSMIT_ON_FIXED_PORT,pSwitchSpecialCaps->TxOnFixedPort);
    specCaps.TxOnFixedPort = pSwitchSpecialCaps->TxOnFixedPort;
    pCslSwitchClbks->cslVlnCapsSet((Uint32)OPER_VLANTUNNEL,pSwitchSpecialCaps->VlanTunnel);
    specCaps.VlanTunnel = pSwitchSpecialCaps->VlanTunnel;

    return PAL_True;
}

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

Int32 switch_learningDisable(void)
{
    pCslSwitchClbks->cslSwitchLearningDisable();
    return PAL_True;
}



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
Int32 switch_trailerSet
    (
        Bool                    mode    /* trailer mode */
    )
{

    pCslSwitchClbks->cslVlnCapsSet((Uint32)OPER_PORT_RECOGNITION_ON,mode);
    specCaps.PortRecognitionOn = mode;
    return PAL_True;
}

/*******************************************************************************
*
* switch_deleteMacAddrFromAtu - Delete MAC address from the Address table
*
*
* INPUTS:
*     [In]    char[6]                MAC address
*
* RETURNS:
*       Int32 - Ok/Error
*
*
*/
Int32 switch_deleteMacAddrFromAtu
(
 unsigned char           MacAdd[6]      /* Mac Address   */
 )
{

   if(pCslSwitchClbks->cslDeleteMacAddEntry != NULL)
      pCslSwitchClbks->cslDeleteMacAddEntry(MacAdd);

   return PAL_True;

}

/*******************************************************************************
*
* Sample appplications for VxWorks only */

#ifdef TORNADO2_2
extern int  printf ();

void PrintVlanMode(void)
{
    SWITCH_SPECIAL_CAPS    switchSpecialCaps;

    switch_specCapsGet(&switchSpecialCaps);

    printf("switchSpecialCaps.VlanTunnel = %d switchSpecialCaps.IngressMode = %d \r\n",switchSpecialCaps.VlanTunnel,switchSpecialCaps.IngressMode);
}

void VlnModeOn(void)
{
    SWITCH_SPECIAL_CAPS    switchSpecialCaps;

    switchSpecialCaps.VlanTunnel = PAL_True;
    switchSpecialCaps.PortRecognitionOn = PAL_False;
    switchSpecialCaps.TxOnFixedPort = PAL_False;
    switchSpecialCaps.IngressMode = INGRESS_VLN_MODE_8021Q_CHECK;
    switch_specCapsSet(&switchSpecialCaps);

}

void VlnModeOff(void)
{
    SWITCH_SPECIAL_CAPS    switchSpecialCaps;

    switchSpecialCaps.VlanTunnel = PAL_False;
    switchSpecialCaps.PortRecognitionOn = PAL_False;
    switchSpecialCaps.TxOnFixedPort = PAL_False;
    switchSpecialCaps.IngressMode = INGRESS_VLN_MODE_8021Q_DISABLE;
    switch_specCapsSet(&switchSpecialCaps);

}


void PrintAllStats(
        Uint32         PortNum /* logical port number*/
)
{
    MIB2_COUNTER mibCnt;
    PAL_Result retVal;
    SWITCH_STAT_E  statIndex;
    retVal = switch_statPortGet(PortNum,&mibCnt);

    for (statIndex = InBytes;statIndex <= ethSymbolErrors;statIndex++)
    {
        printf("index = %d value = %d \r\n",statIndex,*((Uint32*)((Uint32)(&mibCnt) + OFFSET_FROM_STRUCT(statIndex))));
    }
}


void DumpAllRegs(void)
{
    Uint32 regaddr,phyaddr,data;

    printf("Switch port registers \r\n");

    for (phyaddr = 0x18;phyaddr <0x1f;phyaddr++)
    {
        for (regaddr = 0; regaddr < 0x20;regaddr++)
        {
            data = _cpswHalCommonMiiMdioUserAccessRead(pSwitchInfo,regaddr, phyaddr);
            printf("phyaddr = 0x%x regaddr = 0x%x value = 0x%x \r\n",phyaddr,regaddr,data);
        }
    }

    printf("Switch global registers \r\n");

    for (regaddr = 0; regaddr < 0x20;regaddr++)
    {
        data = _cpswHalCommonMiiMdioUserAccessRead(pSwitchInfo,regaddr, phyaddr);
        printf("regaddr = 0x%x value = 0x%x \r\n",regaddr,data);
    }

    printf("Switch phy registers \r\n");

    for (phyaddr = 0x10;phyaddr <0x15;phyaddr++)
    {
        for (regaddr = 0; regaddr < 0x20;regaddr++)
        {
            data = _cpswHalCommonMiiMdioUserAccessRead(pSwitchInfo,regaddr, phyaddr);
            printf("phyaddr = 0x%x regaddr = 0x%x value = 0x%x \r\n",phyaddr,regaddr,data);
        }
    }
}
#endif
