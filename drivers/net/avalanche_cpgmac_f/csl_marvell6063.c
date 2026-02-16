/*
 *
 * csl_marvell6063.c
 * Description:
 * marvell6063 csl layer
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



#include "_tistdtypes.h"
#include "pal_defs.h"
#include "ddc_cpgmac_f_Drv.h"

#include "cpswhalcommon_stddef.h"
#include "cpswhalcommon_miimdio.h"

#include "ddc_switch.h"
#include "csl_switchPrvt.h"
#ifdef VX_COMPILATION
#include "vxWorkspal.h"
#endif
#include "ddc_cpgmac_f_Drv.h"


#define MARVELL_6063_SWRESET_SHIFT_MASK    9
#define MARVELL_6063_SWRESET_MASK          0x1

#define MARVELL_6063_STATUS_SHIFT_MASK   0
#define MARVELL_6063_STATUS_MASK         0x3

#define MARVELL_6063_LINK_SHIFT_MASK     12
#define MARVELL_6063_LINK_MASK           0x1

#define MARVELL_6063_DUPLEX_SHIFT_MASK   9
#define MARVELL_6063_DUPLEX_MASK         0x1

#define MARVELL_6063_DUPLEX_SET_SHIFT_MASK   8

#define MARVELL_6063_SPEED_SHIFT_MASK    8
#define MARVELL_6063_SPEED_MASK          0x1

#define MARVELL_6063_SPEED_SET_SHIFT_MASK   13

#define MARVELL_6063_FC_SHIFT_MASK       15
#define MARVELL_6063_FC_MASK             0x1

#define MARVELL_6063_AUTO_ENABLE_SHIFT_MASK      12
#define MARVELL_6063_AUTO_ENABLE_MASK            0x1

#define MARVELL_6063_AUTO_RESTART_SHIFT_MASK     9
#define MARVELL_6063_AUTO_RESTART_MASK           0x1


/* statistic command register*/

#define MARVELL_6063_STAT_STATSBUSY              0x1
#define MARVELL_6063_STAT_SHIFT_STATSBUSY        15
#define MARVELL_6063_STAT_STATSOP                0x7
#define MARVELL_6063_STAT_SHIFT_STATSOP          12
#define MARVELL_6063_STAT_RESERVED               0x7f
#define MARVELL_6063_STAT_SHIFT_RESERVED         6
#define MARVELL_6063_STAT_STATSPR                0x3f
#define MARVELL_6063_STAT_SHIFT_STATSPR          0

/* statistic module commands */

#define MARVELL_6063_STAT_MODULE_NOP                0x0
#define MARVELL_6063_STAT_MODULE_RESET_ALL_PORTS    0x1
#define MARVELL_6063_STAT_MODULE_RESET_PORT         0x2
#define MARVELL_6063_STAT_MODULE_RESERVED1          0x3
#define MARVELL_6063_STAT_MODULE_READ_COUNTER       0x4
#define MARVELL_6063_STAT_MODULE_CAPTURE            0x5
#define MARVELL_6063_STAT_MODULE_RESERVED2          0x6


/* general commands*/
#define PHY_PHY_ADDR(portNum)       portNum + 0x10
#define PORT_PHY_ADDR(portNum)      portNum + 0x18
#define GLOBAL_PHY_ADDR(portNum)    0x1F

#define INTERNAL_PORT_NUMBER 0x05

PUBLIC PHY_DEVICE * pSwitchInfo;


/*******************************************************************************
*
* cslMarvell6063Start - Does the primary initialization of the switch.
*
* Performs the initialization of the switch and switch specific SW data.
*
* INPUTS:
*
*
* RETURNS: 
*       operational status to be returned to user
*
*
*/

Int32 cslMarvell6063Start (void)
{
    Int32 retVal = -1;
    Uint32 regaddr,phyaddr,data;
    Uint32 portNum;
    

    regaddr = 0x4;

    for (portNum = 0; portNum < 7;portNum++)
    {
        phyaddr = PORT_PHY_ADDR(portNum);

        data = _cpswHalCommonMiiMdioUserAccessRead(pSwitchInfo,regaddr, phyaddr);

        /* Use VLAN tagging. All port are in forwarding state. Special features are of. */

        data |= 0x53;

        _cpswHalCommonMiiMdioUserAccessWrite(pSwitchInfo,regaddr, phyaddr,data);
        
    }

    return retVal;
}

/*******************************************************************************
*
* cslMarvell6060Start - Does the primary initialization of the switch.
*
* Performs the initialization of the switch and switch specific SW data.
*
* INPUTS:
*
*
* RETURNS: 
*       operational status to be returned to user
*
*
*/

Int32 cslMarvell6060Start (void)
{
    Int32 retVal = -1;
    Uint32 regaddr,phyaddr,data;
    Uint32 portNum;
    
    /* used for the VLAN database choice */
    /*swHead->additionalInfo = 0;

    swHead->numberOfPorts = 6;*/
    regaddr = 0x4;

    for (portNum = 0; portNum < 6;portNum++)
    {
        phyaddr = PORT_PHY_ADDR(portNum);

         data = _cpswHalCommonMiiMdioUserAccessRead(pSwitchInfo,regaddr, phyaddr);
        /* All port are in forwarding state. Special features are of. */

        data |= 0x03;


        _cpswHalCommonMiiMdioUserAccessWrite(pSwitchInfo,regaddr, phyaddr,data);
        
    }

    return retVal;
}

/*******************************************************************************
*
* cslMarvell6063RegVal2OperStatus - extract the user operational status out of register value.
*
* This code contains switch specific information. The main purpose is converting switch specific info to 
* the neutral code. As a neutral data types we're using external interfaces.
*
* INPUTS:
*     [In]    OPER_TYPE oper 
*     [In]    Uint16   data
* 
*
*
* RETURNS: 
*       operational status to be returned to user
*
*
*/

Int32 cslMarvell6063RegVal2OperStatus (OPER_TYPE_E oper, Uint16 data)
{
    Int32 retVal = -1;
    
    if (oper == OPER_STATUS)
    {
        /* status is two bits, which coincide with the interface value(enum) */

        retVal = (Int32)data & (MARVELL_6063_STATUS_MASK << MARVELL_6063_STATUS_SHIFT_MASK);
        return retVal;
    }

    else if (oper == OPER_LINK)
    {
        retVal = (Int32)data & (MARVELL_6063_LINK_MASK << MARVELL_6063_LINK_SHIFT_MASK);
        if(retVal == 0)
            return PORT_LINK_DOWN;
        return PORT_LINK_UP;
    }

    else if (oper == OPER_DUPLEX)
    {
        retVal = (Int32)data & (MARVELL_6063_DUPLEX_MASK << MARVELL_6063_DUPLEX_SHIFT_MASK);
        if(retVal == 0)
            return PORT_DUPLEX_HALF;
        return PORT_DUPLEX_FULL;
    }

    else if (oper == OPER_SPEED)
    {
        retVal = (Int32)data & (MARVELL_6063_SPEED_MASK << MARVELL_6063_SPEED_SHIFT_MASK);
        if(retVal == 0)
            return PORT_SPEED_10;
        return PORT_SPEED_100;
    }

    else if (oper == OPER_FC)
    {
        retVal = (Int32)data & (MARVELL_6063_LINK_MASK << MARVELL_6063_LINK_SHIFT_MASK);
        if (retVal == 0)
            return PORT_FC_OFF;
        return PORT_FC_ON;
    }
    else if (oper == OPER_AUTO)
    {
        retVal = (Int32)data & (MARVELL_6063_LINK_MASK << MARVELL_6063_LINK_SHIFT_MASK);
        if (retVal == 0)
            return PORT_AUTO_NO;
        return PORT_AUTO_YES;
    }
    
    return retVal;
}

/*******************************************************************************
*
* cslMarvell6063OperStatus2RegVal - constructs the register value to be written out of data and user request
*
* Enable/Disable a selected port
*
* INPUTS:
*     [In]    OPER_TYPE     oper 
*     [In]    Int32        operValue
*     [In]    Uint16       data
* 
*
*
* RETURNS: 
*       The final register value
*
*
*/

Uint16 cslMarvell6063OperStatus2RegVal (OPER_TYPE_E oper,Int32 operValue,Uint16 data)
{
    Uint16 retVal = 0xffff;

    
    if (oper == OPER_STATUS)
    {
        retVal = (data & (~ ((Uint16)(MARVELL_6063_STATUS_MASK << MARVELL_6063_STATUS_SHIFT_MASK)))) | ((Uint16)operValue);

    }
    else if (oper == OPER_DUPLEX_SET)
    {
        retVal = data & (~((Uint16)(MARVELL_6063_DUPLEX_MASK << MARVELL_6063_DUPLEX_SET_SHIFT_MASK)));
        if(operValue == PORT_DUPLEX_FULL)
            retVal |= (MARVELL_6063_DUPLEX_MASK << MARVELL_6063_DUPLEX_SET_SHIFT_MASK);

        /* assert software reset, otherwise the changes will be ignored */
        retVal |= (Uint16)(MARVELL_6063_AUTO_RESTART_MASK << MARVELL_6063_AUTO_RESTART_SHIFT_MASK);  

    }
    
    else if (oper == OPER_SPEED_SET)
    {
        retVal = data & (~((Uint16)(MARVELL_6063_SPEED_MASK << MARVELL_6063_SPEED_SET_SHIFT_MASK)));
        if(operValue == PORT_SPEED_100)
            retVal |= (MARVELL_6063_SPEED_MASK << MARVELL_6063_SPEED_SET_SHIFT_MASK);

        /* assert software reset, otherwise the changes will be ignored */
        retVal |= (Uint16)(MARVELL_6063_AUTO_RESTART_MASK << MARVELL_6063_AUTO_RESTART_SHIFT_MASK); 
    }

    else if (oper == OPER_FC)
    {
        retVal = data & (~((Uint16)(MARVELL_6063_FC_MASK << MARVELL_6063_FC_SHIFT_MASK)));
        if(operValue == PORT_FC_ON)
            retVal |= (MARVELL_6063_DUPLEX_MASK << MARVELL_6063_DUPLEX_SHIFT_MASK);
    }

    else if (oper == OPER_AUTO)
    {
        retVal = data & (~((Uint16)(MARVELL_6063_AUTO_ENABLE_MASK << MARVELL_6063_AUTO_ENABLE_SHIFT_MASK)));
        if(operValue != PORT_AUTO_NO)
        {
            retVal |= (Uint16)(MARVELL_6063_AUTO_ENABLE_MASK << MARVELL_6063_AUTO_ENABLE_SHIFT_MASK);
           
        }
        
        /* assert software reset, otherwise the changes will be ignored */
        retVal |= (Uint16)(MARVELL_6063_AUTO_RESTART_MASK << MARVELL_6063_AUTO_RESTART_SHIFT_MASK); 
    }

    else if (oper == OPER_RESET_STAT)
    {
        
        retVal = (MARVELL_6063_STAT_STATSBUSY << MARVELL_6063_STAT_SHIFT_STATSBUSY) | 
           (MARVELL_6063_STAT_MODULE_RESET_PORT << MARVELL_6063_STAT_SHIFT_STATSOP) |
           (operValue << MARVELL_6063_STAT_SHIFT_STATSPR);
    }

    return retVal;
    
}

/*******************************************************************************
*
* cslMarvell6063PrepareReadStatSequence - prepare the switch for reading statistic counters.
*
* Prepare the switch for reading statistic counters (if needed)
*
* INPUTS:
*     [In]    Uint32            port
* 
*
*
* RETURNS: 
*       Status
*
*
*/


Int32 cslMarvell6063PrepareReadStatSequence(Uint32 port)
{
    Uint32 data;
    Uint32 regaddr,phyaddr;

    /*  In case of Marvell 6063 lock the counters */

    regaddr = 0x1D;

    phyaddr = GLOBAL_PHY_ADDR(port);
    data = (MARVELL_6063_STAT_STATSBUSY << MARVELL_6063_STAT_SHIFT_STATSBUSY) | 
           (MARVELL_6063_STAT_MODULE_CAPTURE << MARVELL_6063_STAT_SHIFT_STATSOP) |
           (port << MARVELL_6063_STAT_SHIFT_STATSPR);

    _cpswHalCommonMiiMdioUserAccessWrite(pSwitchInfo,regaddr, phyaddr,data);

    return PAL_True;
}

/*******************************************************************************
*
* cslMarvell6063LearningDisable - Disables the learning of all the Marvell 
*                                 ports except the CPU port
*
*
* INPUTS:
*       None
* 
*
*
* RETURNS: 
*       Status
*
*
*/


Int32 cslMarvell6063LearningDisable(void)
{
    Uint32 data;
    Uint32 regaddr,phyaddr;

    /*  In case of Marvell 6063 lock the counters */

    regaddr = 0x0A;
    phyaddr = GLOBAL_PHY_ADDR(0);

    data = _cpswHalCommonMiiMdioUserAccessRead(pSwitchInfo,regaddr, phyaddr);

    data |= 0x4000;

    _cpswHalCommonMiiMdioUserAccessWrite(pSwitchInfo,regaddr, phyaddr,data);

    return PAL_True;
}

/*******************************************************************************
*
* cslMarvell6063GetPreparedCounters - read statistics counters.
*
* INPUTS:
*     [In]    Uint32           statIndex
*     [Out]   Uint32*          counter
* 
*
*
* RETURNS: 
*       Status
*
*
*/


Int32 cslMarvell6063GetPreparedCounters(Uint32 statIndex,Uint32* counter)
{
    Uint32 data,cnt;
    Uint32 regaddr,phyaddr;
    Uint16 readData1,readData2;

    /*  In case of Marvell 6063 lock the counters */
    
    regaddr = 0x1D;
    phyaddr = GLOBAL_PHY_ADDR(0);

    /* Convert the counter from driver interface number to switch interface number */
    switch ((SWITCH_STAT_E) statIndex)
    {
    case    InBytes :                 
        cnt = 0x6;
        break;

    case    InBytesHigh :                
        *counter  = 0;
        return PAL_True;

    case    InUnicastPkts  :             
        cnt = 0x0;
        break;

    case    InUnicastPktsHigh :          
        *counter  = 0;
        return PAL_True;

    case    InMulticastPkts :             
        cnt = 0x3;
        break;

    case    InMulticastPktsHigh :        
        *counter  = 0;
        return PAL_True;

    case    InBroadcastPkts  :           
        cnt = 0x1;
        break;

    case    InBroadcastPktsHigh :        
        *counter  = 0;
        return PAL_True;

    case    InDiscardPkts :               
        cnt = 0x12;
        break;

    case    InErrorPkts :   
        *counter  = 0;
        return PAL_True;
                      
    case    InUnknownProtPkts :          
        *counter  = 0;
        return PAL_True;

    case    OutBytes :                   
        cnt = 0x19;
        break;

    case    OutBytesHigh  :   
        *counter  = 0;
        return PAL_True;
                   
    case    OutUnicastPkts :             
        cnt = 0x14;
        break;

    case    OutUnicastPktsHigh :  
        *counter  = 0;
        return PAL_True;
               
    case    OutMulticastPkts :            
        cnt = 0x17;
        break;

    case    OutMulticastPktsHigh  :      
        *counter  = 0;
        return PAL_True;

    case    OutBroadcastPkts  :  
        cnt = 0x15;
        break;
                
    case    OutBroadcastPktsHigh :       
        *counter  = 0;
        return PAL_True;

    case    OutDiscardPkts :             
        cnt = 0x26;
        break;

    case    OutErrorPkts :    
        *counter  = 0;
        return PAL_True;
                    

    case    ethAlignmentErrors :         
        cnt = 0x5;
        break;

    case    ethFCSErrors :               
        cnt = 0x4;
        break;

    case    ethSingleCollisions  :       
        cnt = 0x24;
        break;

    case    ethMultipleCollisions  :     
        cnt = 0x23;
        break;

    case    ethSQETestErrors :           
        *counter  = 0;
        return PAL_True;

    case    ethDeferredTxFrames  :       
        cnt = 0x25;
        break;

    case    ethLateCollisions :          
        cnt = 0x21;
        break;

    case    ethExcessiveCollisions :     
        cnt = 0x22;
        break;

    case    ethInternalMacTxErrors :    
        *counter  = 0;
        return PAL_True;

    case    ethCarrierSenseErrors :
        *counter  = 0;
        return PAL_True;
               
    case    ethTooLongRxFrames :          
        cnt = 0x11;
        break;

    case    ethInternalMacRxErrors :  
        *counter  = 0;
        return PAL_True;
           
    case    ethSymbolErrors :    
        *counter  = 0;
        return PAL_True;
                  
    default:
        *counter  = 0;
        return PAL_True;
    }

    
    data = MARVELL_6063_STAT_STATSBUSY << MARVELL_6063_STAT_SHIFT_STATSBUSY;
    while(data >> MARVELL_6063_STAT_SHIFT_STATSBUSY)
    {
       data = _cpswHalCommonMiiMdioUserAccessRead(pSwitchInfo,regaddr, phyaddr);
    }

    data = (MARVELL_6063_STAT_STATSBUSY << MARVELL_6063_STAT_SHIFT_STATSBUSY) | 
           (MARVELL_6063_STAT_MODULE_READ_COUNTER << MARVELL_6063_STAT_SHIFT_STATSOP) |
           (cnt << MARVELL_6063_STAT_SHIFT_STATSPR);

    _cpswHalCommonMiiMdioUserAccessWrite(pSwitchInfo,regaddr, phyaddr,data);

    regaddr = 0x1E;
    readData1 = _cpswHalCommonMiiMdioUserAccessRead(pSwitchInfo,regaddr, phyaddr);

    regaddr = 0x1F;
    readData2 = _cpswHalCommonMiiMdioUserAccessRead(pSwitchInfo,regaddr, phyaddr);

    *counter = PAL_MK_UINT32(readData1,readData2);

    return PAL_True;
}

/*******************************************************************************
*
* cslMarvell6063SwitchGetVersion - returns the switch specific information
*
* 
*
* INPUTS:
*     [Out]   PSW_VER_INFO pSwitchInfo
* 
*
*
* RETURNS: 
*       Status
*
*
*/

void cslMarvell6063SwitchGetVersion(PSW_VER_INFO pSwitchVerInfo)
{
    Uint32 regaddr,phyaddr,data;

    regaddr = 0x3;

    phyaddr = PORT_PHY_ADDR(0);

    pSwitchVerInfo->is802_1_P_capable      = PAL_True; 
    pSwitchVerInfo->is802_1_Q_capable      = PAL_True;
    pSwitchVerInfo->maxNumberOfPorts       = 7;
    pSwitchVerInfo->maxNumberOfPriorities  = 4;
    pSwitchVerInfo->maxNumberOfVLANs       = 64;

    pSwitchVerInfo->Model = _cpswHalCommonMiiMdioUserAccessRead(pSwitchInfo,regaddr, phyaddr);

     phyaddr = PORT_PHY_ADDR(0x5);
     regaddr = 0x4;

     data = _cpswHalCommonMiiMdioUserAccessRead(pSwitchInfo,regaddr, phyaddr);

}

/*******************************************************************************
*
* cslMarvell6060SwitchGetVersion - returns the switch specific information
*
* 
*
* INPUTS:
*     [Out]   PSW_VER_INFO pSwitchInfo
* 
*
*
* RETURNS: 
*       Status
*
*
*/

void cslMarvell6060SwitchGetVersion(PSW_VER_INFO pSwitchVerInfo)
{
    Uint32 regaddr,phyaddr;

    regaddr = 0x3;

    phyaddr = PORT_PHY_ADDR(0);

    pSwitchVerInfo->is802_1_P_capable      = PAL_True; 
    pSwitchVerInfo->is802_1_Q_capable      = PAL_True;
    pSwitchVerInfo->maxNumberOfPorts       = 6;
    pSwitchVerInfo->maxNumberOfPriorities  = 0;
    pSwitchVerInfo->maxNumberOfVLANs       = 0;
    pSwitchVerInfo->Model = _cpswHalCommonMiiMdioUserAccessRead(pSwitchInfo,regaddr, phyaddr);

}

         
/*******************************************************************************
*
* cslMarvell6063PortNum2RegAddr - returns the register address
*
* Retreive register address from port and operator type.
*
* INPUTS:
*     [In]   OPER_TYPE_E    type
*     [In]   Uint32         portNum
*
*
* RETURNS: 
*       Register address
*
*
*/

Uint32 cslMarvell6063PortNum2RegAddr(OPER_TYPE_E type, Uint32 portNum)
{
    Uint32 regAddr = 0;

    switch (type) 
            { 
                case OPER_STATUS: 
                    regAddr = 0x4; 
                    break; 
                case OPER_LINK: 
                    regAddr = 0x0; 
                    break; 
                case OPER_DUPLEX: 
                    regAddr = 0x0; 
                    break; 
                case OPER_SPEED: 
                    regAddr = 0x0; 
                    break; 
                case OPER_AUTO: 
                    regAddr = 0x0; 
                    break; 
                case OPER_FC: 
                    regAddr = 0x4; 
                    break; 
                case OPER_RESET_STAT: 
                    regAddr = 0x1d; 
                    break; 
                default: 
                    break; 
        } 

    return regAddr;
}

/*******************************************************************************
*
* cslMarvell6063PortNum2PHYAddr - returns the PHY address
*
* Retreive PHY address from port and operator type.
*
* INPUTS:
*     [In]   OPER_TYPE_E    type
*     [In]   Uint32         portNum
*
*
* RETURNS: 
*       Register address
*
*
*/


Uint32 cslMarvell6063PortNum2PHYAddr(OPER_TYPE_E type, Uint32 portNum)
{
    Uint32 phyAddr = 0;
    
    switch (type) 
        { 
            case OPER_STATUS: 
                phyAddr = PORT_PHY_ADDR(portNum); 
                break; 
            case OPER_LINK: 
                phyAddr = PORT_PHY_ADDR(portNum); 
                break; 
            case OPER_DUPLEX: 
                phyAddr = PORT_PHY_ADDR(portNum); 
                break; 
            case OPER_DUPLEX_SET: 
                phyAddr = PHY_PHY_ADDR(portNum); 
                break; 

            case OPER_SPEED: 
                phyAddr = PORT_PHY_ADDR(portNum); 
                break; 
          
            case OPER_SPEED_SET: 
                phyAddr = PHY_PHY_ADDR(portNum); 
                break; 

            case OPER_AUTO: 
                phyAddr = PHY_PHY_ADDR(portNum); 
                break; 
            case OPER_FC: 
                phyAddr = PORT_PHY_ADDR(portNum); 
                break; 
            case OPER_VLANTUNNEL: 
                phyAddr = PORT_PHY_ADDR(portNum); 
                break; 
            case OPER_RESET_STAT: 
                phyAddr = GLOBAL_PHY_ADDR(portNum); 
                break; 
            default: 
                break; 
        }

    return phyAddr;
}
/**************************************************** VLAN submodule **************************************/

#define VLN_DEFAULT_MASK        0xFFF
#define VLN_DEFAULT_MASK_SHIFT  0

#define VLN_DEFAULT_PRIOR_MASK        0x7
#define VLN_DEFAULT_PRIOR_MASK_SHIFT  13


#define PORT_MEMBER_MASK(isMember,tag)  (((isMember) & 1) ? (2|((tag) & 1)) : 1)

#define VTU_BUSY_MASK               (0x1 << 15)

#define VTU_OP_MASK(oper)           ((oper) << 12)

#define VTU_MEMBER_VIOLATION_MASK   (0x1 << 6)
#define VTU_MISS_VIOLIATION_MASK    (0x1 << 5)
#define VTU_FULL_MASK               (0x1 << 4)
#define VTU_DBNUM_MASK              (0xF << 0)
#define VTU_DBNUM_MASK_SHIFT        (0)

#define VTU_OP_FLUSH                0x1
#define VTU_OP_LOAD                 0x3
#define VTU_OP_GET                  0x7

/*******************************************************************************
*
* cslMarvell6063VlanDefaultSet - Defines the default VLAN
*
* Defines the default VLAN. The meaning can change from implementation to implementation.
*
* INPUTS:
*     [In]   Uint32         portNum
*     [In]   PCSL_SWITCH_VLAN_TABLE pVlnEntry
*
*
* RETURNS: 
*       Status
*
*
*/


Int32 cslMarvell6063VlanDefaultSet(Uint32 portNum, CSL_SWITCH_VLAN_ENTRY * pVlnEntry)
{
    Uint32 regaddr,phyaddr,data;

    regaddr = 0x7;

    phyaddr = PORT_PHY_ADDR(portNum);

     data = _cpswHalCommonMiiMdioUserAccessRead(pSwitchInfo,regaddr, phyaddr);

    data = (data & (~(VLN_DEFAULT_MASK << VLN_DEFAULT_MASK_SHIFT))) | (pVlnEntry->vid & (VLN_DEFAULT_MASK << VLN_DEFAULT_MASK_SHIFT));

    _cpswHalCommonMiiMdioUserAccessWrite(pSwitchInfo,regaddr, phyaddr,data);

    return PAL_True;
}



/*******************************************************************************
*
* cslMarvell6063VlanUpdate - Updates the VLAN in VLAN table
*
* Writes the VLAN data to the VLAN table.
*
* INPUTS:
*     [In]   PCSL_SWITCH_VLAN_TABLE pVlnEntry
*
*
* RETURNS: 
*       Status
*
*
*/

Int32 cslMarvell6063VlanUpdate(CSL_SWITCH_VLAN_ENTRY * pVlnEntry)
{
    Uint32 regaddr,phyaddr,portMask,idx,dbNum = 0;
    Uint32 data = 0;
    
    phyaddr = GLOBAL_PHY_ADDR(0);

    if (pVlnEntry->valid)
    {
        /*  update the port mask register 0*/
   
        for(portMask = pVlnEntry->mem_ports,idx=0;idx < 4; idx++,portMask >>= 1) 
        {
            data |= ((PORT_MEMBER_MASK(portMask,pVlnEntry->tagged_ports >> idx) ) << (idx*4));
        }
        
        regaddr = 7;
    
        _cpswHalCommonMiiMdioUserAccessWrite(pSwitchInfo,regaddr, phyaddr,data);
    
        /*  update the port mask register 1 */
    
        for(data = 0;idx < 7; idx++,portMask >>= 1)
        {
            data |= (PORT_MEMBER_MASK(portMask,pVlnEntry->tagged_ports >> idx) ) << ((idx - 4)*4);
        }
        
        regaddr = 8;

        _cpswHalCommonMiiMdioUserAccessWrite(pSwitchInfo,regaddr, phyaddr,data);
    
    }

    /* update VLAN id*/
        
    data = pVlnEntry->vid | (pVlnEntry->valid << 12);
    
    regaddr = 6;
    

    _cpswHalCommonMiiMdioUserAccessWrite(pSwitchInfo,regaddr, phyaddr,data);

    /* we're ready to write the data to VTU, perform it */

    regaddr = 5;
    

    /* run on all data bases */

    /*if(!swHead->specCaps.VlanTunnel)
        swHead->additionalInfo = (swHead->additionalInfo + 1) % 16;*/
        
    data = VTU_OP_MASK(VTU_OP_LOAD) | VTU_BUSY_MASK | ((dbNum << VTU_DBNUM_MASK_SHIFT) & VTU_DBNUM_MASK);
    

    _cpswHalCommonMiiMdioUserAccessWrite(pSwitchInfo,regaddr, phyaddr,data);

    /* check the violation status */
    data = _cpswHalCommonMiiMdioUserAccessRead(pSwitchInfo,regaddr, phyaddr);


    /* VTU full violation */
    if(data & VTU_FULL_MASK)
    {
        /* clear the violation status */
        data = VTU_OP_MASK(VTU_OP_GET) | VTU_BUSY_MASK ;


        _cpswHalCommonMiiMdioUserAccessWrite(pSwitchInfo,regaddr, phyaddr,data);


        return PAL_False;
    }
        
    return PAL_True;
}


/*******************************************************************************
*
* cslMarvell6063PrioritySet - Sets the default priority of the packets from this port
*   
*   Sets the default priority of the packets from this port.
*
* INPUTS:
*     [In]   Uint32                 portNum
*     [In]   Uint32                 priority
*
*
* RETURNS: 
*       Status
*
*
*/


Int32 cslMarvell6063PrioritySet(Uint32 portNum, Uint32 priority)
{
    Uint32 regaddr,phyaddr,data;

    regaddr = 0x7;

    phyaddr = PORT_PHY_ADDR(portNum);

     data = _cpswHalCommonMiiMdioUserAccessRead(pSwitchInfo,regaddr, phyaddr);

    data = (data & (~(VLN_DEFAULT_PRIOR_MASK << VLN_DEFAULT_PRIOR_MASK_SHIFT))) | (priority & (~(VLN_DEFAULT_PRIOR_MASK << VLN_DEFAULT_PRIOR_MASK_SHIFT)));

    _cpswHalCommonMiiMdioUserAccessWrite(pSwitchInfo,regaddr, phyaddr,data);

    return PAL_True;
}


/*******************************************************************************
*
* cslMarvell6063PriorityGet - Gets the default priority of the packets from this port
*   
*   Gets the default priority of the packets from this port.
*
* INPUTS:
*     [In]   Uint32                 portNum
*     [In]   Uint32                 priority
*
*
* RETURNS: 
*       Status
*
*
*/


Uint32 cslMarvell6063PriorityGet(Uint32 portNum)
{
    Uint32 regaddr,phyaddr,data;

    regaddr = 0x7;

    phyaddr = PORT_PHY_ADDR(portNum);

    /*phyRegRead(pSwitchInfo, regaddr, phyaddr, data);*/
    data = _cpswHalCommonMiiMdioUserAccessRead(pSwitchInfo,regaddr, phyaddr);

    data = data >> VLN_DEFAULT_PRIOR_MASK_SHIFT;

    return (Uint32) data;
}

/*******************************************************************************
*
* cslMarvell6063SpecialCapsSet - sets special capabilities configuration of the switch
*   
*
* INPUTS:
*     [In]   Uint32                 portNum
*     [In]   Uint32                 arg -the value of the specifice capability
*
*
* RETURNS: 
*       Status
*
*
*/

Int32 cslMarvell6063SpecialCapsSet(Uint32 cmd, Uint32 arg)
{
    Uint32 regaddr,phyaddr,data,port;                    

    switch ((OPER_TYPE_E)cmd)
    {
/* all the ports are ingress checkin/ no ingress checking */

    case OPER_VLAN_TABLE_SET:
        regaddr = 0x6;
        port = ( arg & 0xF000 ) >> 12;

        if( port > 6 )
            return -1;

        arg &= 0x7F;
        phyaddr = PORT_PHY_ADDR(port);
        data = _cpswHalCommonMiiMdioUserAccessRead(pSwitchInfo,regaddr, phyaddr);

        /* Prevent sending packet to the same port */
        data = ( ( data & ~0x7F ) | (arg & (~(0x01 << port ))) | (( 0x1 + port ) << 12 ) );

        /* Write register */
        _cpswHalCommonMiiMdioUserAccessWrite(pSwitchInfo,regaddr, phyaddr,data);
        break;

    case OPER_INGRESS:            
        regaddr = 0x6;
        {
            INGRESS_VLN_MODE mode = (INGRESS_VLN_MODE)arg;

            switch ( mode )
            {
            default:
            case INGRESS_VLN_MODE_8021Q_DISABLE:
                data = 0x0;
                break;

            case INGRESS_VLN_MODE_8021Q_CHECK:
                data = 0x800;
                break;

            case INGRESS_VLN_MODE_8021Q_SECURE:
                data = 0xC00;
                break;
            }
        }
        for (port = 0; port < 7;port++)
        {
            Uint32 indata;

            phyaddr = PORT_PHY_ADDR(port);
            indata = _cpswHalCommonMiiMdioUserAccessRead(pSwitchInfo,regaddr, phyaddr);

            data |= (indata & ~0xC00);
                        
            _cpswHalCommonMiiMdioUserAccessWrite(pSwitchInfo,regaddr, phyaddr,data);
        }

        break;
    case OPER_INGRESS_OFF:
        regaddr = 0x6;
        
        for (port = 0; port < 7;port++)
        {
           data = 0x7F;
           phyaddr = PORT_PHY_ADDR(port);
           data = (data & (~(0x01 << port )) );
           _cpswHalCommonMiiMdioUserAccessWrite(pSwitchInfo,regaddr, phyaddr,data);
        }

       break;
    case OPER_PORT_RECOGNITION_ON:   
        phyaddr = PORT_PHY_ADDR(INTERNAL_PORT_NUMBER);
        regaddr = 0x4;

        data = _cpswHalCommonMiiMdioUserAccessRead(pSwitchInfo,regaddr, phyaddr);
        if(!arg)
            data &= 0xBFFF;
        else
            data |= 0x4000;

        _cpswHalCommonMiiMdioUserAccessWrite(pSwitchInfo,regaddr, phyaddr,data);
        break;

    case OPER_TRANSMIT_ON_FIXED_PORT:
        phyaddr = PORT_PHY_ADDR(INTERNAL_PORT_NUMBER);
        regaddr = 0x4;

        data = _cpswHalCommonMiiMdioUserAccessRead(pSwitchInfo,regaddr, phyaddr);

        if(!arg)
            data &= 0xEFF;
        else
            data |= 0x100;

        _cpswHalCommonMiiMdioUserAccessWrite(pSwitchInfo,regaddr, phyaddr,data);
        break;

    /* nothing to be done here*/
    case OPER_VLANTUNNEL:
    default:
            break;           

    }

    return PAL_True;
}

/*******************************************************************************
*
* cslMarvell6060SpecialCapsSet - sets special capabilities configuration of the switch
*   
*
* INPUTS:
*     [In]   Uint32                 portNum
*     [In]   Uint32                 arg -the value of the specifice capability
*
*
* RETURNS: 
*       Status
*
*
*/

Int32 cslMarvell6060SpecialCapsSet(Uint32 cmd, Uint32 arg)
{
    Uint32 regaddr,phyaddr,data,port;                    

    switch ((OPER_TYPE_E)cmd)
    {
/* all the ports are ingress checkin/ no ingress checking */

    case OPER_INGRESS:            
        break;

    case OPER_VLAN_TABLE_SET:
        regaddr = 0x6;
        port = ( arg & 0xF000 ) >> 12;

        if( port > 5 )
            return -1;

        arg &= 0x3F;
        phyaddr = PORT_PHY_ADDR(port);

        data = _cpswHalCommonMiiMdioUserAccessRead(pSwitchInfo,regaddr, phyaddr);

        /* Prevent sending packet to the same port */
        data = ( ( data & ~0x3F ) | (arg & (~(0x01 << port ))) | (( 0x1 + port ) << 12 ) );

        /* Write register */
        _cpswHalCommonMiiMdioUserAccessWrite(pSwitchInfo,regaddr, phyaddr,data);
         break;

    case OPER_INGRESS_OFF:
        regaddr = 0x6;

        for (port = 0; port < 6;port++)
        {
            data = 0x3F;
            phyaddr = PORT_PHY_ADDR(port);
            data = (data & (~(0x01 << port )) );
            _cpswHalCommonMiiMdioUserAccessWrite(pSwitchInfo,regaddr, phyaddr,data);
        }
        break;

    case OPER_PORT_RECOGNITION_ON:   
        phyaddr = PORT_PHY_ADDR(INTERNAL_PORT_NUMBER);
        regaddr = 0x4;

        data = _cpswHalCommonMiiMdioUserAccessRead(pSwitchInfo,regaddr, phyaddr);
        if(!arg)
            data &= 0xBFFF;
        else
            data |= 0x4000;

        _cpswHalCommonMiiMdioUserAccessWrite(pSwitchInfo,regaddr, phyaddr,data);
        break;

    case OPER_TRANSMIT_ON_FIXED_PORT:
        phyaddr = PORT_PHY_ADDR(INTERNAL_PORT_NUMBER);
        regaddr = 0x4;

        data = _cpswHalCommonMiiMdioUserAccessRead(pSwitchInfo,regaddr, phyaddr);
        if(!arg)
            data &= 0xEFF;
        else
            data |= 0x100;

        _cpswHalCommonMiiMdioUserAccessWrite(pSwitchInfo,regaddr, phyaddr,data);
        break;

    /* nothing to be done here*/
    case OPER_VLANTUNNEL:
    default:
            break;           

    }

    return PAL_True;
}

#define ATU_PURGE_OPERATION_VALUE 0xB000
/*******************************************************************************
*
* cslMarvell6063PurgeAtuEntry - Purge an entry from the address translation Unit table.
*
*
*
* INPUTS:
*     [In]   char MacAddress[6]
*
*
* RETURNS: 
*       Status
*
*
*/

Int32 cslMarvell6063PurgeAtuEntry(char MacAddress[6])
{
    Uint32 regaddr,phyaddr;
    Uint32 data = 0;
    unsigned short data1=0;
    phyaddr = GLOBAL_PHY_ADDR(0);

   /* ATU MAC ADDRESS Register Bytes 0 & 1 */
    data1 = ((MacAddress[0] << 8) | MacAddress[1] );
    regaddr = 0x0D;
    
    _cpswHalCommonMiiMdioUserAccessWrite(pSwitchInfo,regaddr, phyaddr,(Uint32)data1);

   /* ATU MAC ADDRESS Register Bytes 2 & 3 */

    data1 = ((MacAddress[2] << 8) | MacAddress[3] );     
    regaddr = 0x0E;
    
    
    _cpswHalCommonMiiMdioUserAccessWrite(pSwitchInfo,regaddr, phyaddr,(Uint32)data1);
   
    /* ATU MAC ADDRESS Register Bytes 4 & 5 */    
    data1 = ((MacAddress[4] << 8) | MacAddress[5] );

    regaddr = 0x0F;
        
    _cpswHalCommonMiiMdioUserAccessWrite(pSwitchInfo,regaddr, phyaddr,(Uint32)data1);

    /* we're ready to write the data to ATU, perform it */
    /* ATU Data Register - Entry State must be zero for purge*/
    regaddr = 0x0C;
        
    data = 0x00;

    _cpswHalCommonMiiMdioUserAccessWrite(pSwitchInfo,regaddr, phyaddr,data);

    /* ATU Operation Register */
    regaddr = 0x0B;
    /* Set Opcode to Purge and set on the ATUBusy bit */
    data = ATU_PURGE_OPERATION_VALUE;

    _cpswHalCommonMiiMdioUserAccessWrite(pSwitchInfo,regaddr, phyaddr,data);

    return PAL_True;
}

