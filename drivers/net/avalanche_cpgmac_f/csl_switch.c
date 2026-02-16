/*
 *
 * csl_switch.c
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


#include "_tistdtypes.h"
#include <pal.h>

#include "ddc_cpgmac_f_Drv.h"
#include "ddc_switch.h"
#include "csl_switchPrvt.h"

#ifdef CONFIG_MIPS_AVALANCHE_KENDIN
#include "csl_kendin8995x.h"
#endif
#if defined(CONFIG_MIPS_AVALANCHE_MARVELL_6063) || defined(CONFIG_MIPS_AVALANCHE_MARVELL_6060)
#include "csl_marvell6063.h"
#endif

static int VlanCounter=0;
static CSL_SWITCH_VLAN_ENTRY * pVlanTable = NULL;

/*******************************************************************************
* cslConnectSwitch - switch initialization
*
*  This API does the callback structure initialization-> Here the type of the switch is considered->
*
* INPUTS:
*       [In/Out] PCSL_SWITCH_CLBKS pCslSwitchClbks
*
*******************************************************************************/

Int32 cslConnectSwitch(PCSL_SWITCH_CLBKS pCslSwitchClbks)
{

#if defined(CONFIG_MIPS_AVALANCHE_MARVELL_6063)

#if 0
  printk("IN 6063\n");
#endif

   pCslSwitchClbks->cslGetPreparedCounters      = cslMarvell6063GetPreparedCounters;

   pCslSwitchClbks->cslRegVal2operStatus        = cslMarvell6063RegVal2OperStatus;

   pCslSwitchClbks->cslOperStatus2regVal        = cslMarvell6063OperStatus2RegVal;

   pCslSwitchClbks->cslSwitchGetVersion         = cslMarvell6063SwitchGetVersion;

   pCslSwitchClbks->cslPrepareReadStatSequence  = cslMarvell6063PrepareReadStatSequence;

   pCslSwitchClbks->cslPortNum2RegAddr          = cslMarvell6063PortNum2RegAddr;

   pCslSwitchClbks->cslPortNum2PhyAddr          = cslMarvell6063PortNum2PHYAddr;

   pCslSwitchClbks->cslSwitchStart              = cslMarvell6063Start;

   pCslSwitchClbks->cslVlnCapsSet               = cslMarvell6063SpecialCapsSet;

   pCslSwitchClbks->cslVlnDefaultSet            = cslMarvell6063VlanDefaultSet;

   pCslSwitchClbks->cslVlnUpdate                = cslMarvell6063VlanUpdate;

   pCslSwitchClbks->cslVlnPriorityGet           = cslMarvell6063PriorityGet;

   pCslSwitchClbks->cslVlnPrioritySet           = cslMarvell6063PrioritySet;

   pCslSwitchClbks->cslDeleteMacAddEntry        = cslMarvell6063PurgeAtuEntry;

   pCslSwitchClbks->cslSwitchLearningDisable    = cslMarvell6063LearningDisable;

   return PAL_True;
#endif
#if defined(CONFIG_MIPS_AVALANCHE_MARVELL_6060)

   pCslSwitchClbks->cslGetPreparedCounters      = NULL;

   pCslSwitchClbks->cslRegVal2operStatus        = cslMarvell6063RegVal2OperStatus;

   pCslSwitchClbks->cslOperStatus2regVal        = cslMarvell6063OperStatus2RegVal;

   pCslSwitchClbks->cslSwitchGetVersion         = cslMarvell6060SwitchGetVersion;

   pCslSwitchClbks->cslPrepareReadStatSequence  = NULL;

   pCslSwitchClbks->cslPortNum2RegAddr          = cslMarvell6063PortNum2RegAddr;

   pCslSwitchClbks->cslPortNum2PhyAddr          = cslMarvell6063PortNum2PHYAddr;

   pCslSwitchClbks->cslSwitchStart              = cslMarvell6060Start;

   pCslSwitchClbks->cslVlnCapsSet               = cslMarvell6060SpecialCapsSet;

   pCslSwitchClbks->cslVlnDefaultSet            = NULL;

   pCslSwitchClbks->cslVlnUpdate                = NULL;

   pCslSwitchClbks->cslVlnPriorityGet           = NULL;

   pCslSwitchClbks->cslVlnPrioritySet           = NULL;

   pCslSwitchClbks->cslDeleteMacAddEntry        = cslMarvell6063PurgeAtuEntry;

   return PAL_True;
#endif
#ifdef CONFIG_ARM_AVALANCHE_ATHEROS_8328N

   cslAtheros8328N_global_init( pCslSwitchClbks );
   cslAtheros8328N_vlan_init  ( pCslSwitchClbks );
   cslAtheros8328N_portvlan_init( pCslSwitchClbks );

   return PAL_True;

#endif

   return PAL_False;

}


/*******************************************************************************
* cslCreateVLANEntry - creates and adds VLAN entry to the VLAN table
*
*  Creates and adds VLAN entry to the logical VLAN table. List manager.
*
* INPUTS:
*
*******************************************************************************/

CSL_SWITCH_VLAN_ENTRY * cslCreateVLANEntry(void)
{
    CSL_SWITCH_VLAN_ENTRY * tmp;
    CSL_SWITCH_VLAN_ENTRY * elem;
    VlanCounter++;

    /* The 6063 support up to 64 different VLAN Identifier */
    if(VlanCounter > 64)
       return NULL;

    /*elem = (PCSL_SWITCH_VLAN_TABLE) */
    if(PAL_osMemAlloc(0,sizeof(CSL_SWITCH_VLAN_ENTRY),0,(Ptr *) &elem) != PAL_SOK)
         return NULL;

    PAL_osMemSet(elem, 0, sizeof(CSL_SWITCH_VLAN_ENTRY));

    if (NULL != pVlanTable)
    {
        tmp = pVlanTable->next;
        pVlanTable->next = elem;
        elem->next = tmp;
    }
    else
    {
        pVlanTable = elem;
    }

    return elem;
}

/*******************************************************************************
* cslDestroyVLANEntry - destroys VLAN entry
*
*  Removes VLAN entry from VLAN table. List manager.
*
* INPUTS:
*       [In] PCSL_SWITCH_VLAN_TABLE elem
*
*******************************************************************************/

Int32 cslDestroyVLANEntry(CSL_SWITCH_VLAN_ENTRY * elem)
{
    CSL_SWITCH_VLAN_ENTRY * tmp;
    CSL_SWITCH_VLAN_ENTRY * prev;

    for (tmp = pVlanTable, prev = NULL; tmp != NULL; tmp = tmp->next)
    {
        /* elem is not NULL!!*/
        if(elem == tmp)
        {
            VlanCounter--;
            if (prev)
            {
                prev->next = elem->next;
            }
            else
            {
                pVlanTable = elem->next;
            }
            PAL_osMemFree(0,elem,sizeof(CSL_SWITCH_VLAN_ENTRY));
            return PAL_True;
        }
        prev = tmp;
    }

    return PAL_False;

}

/*******************************************************************************
* cslGetVLANEntry - gets VLAN entry
*
*  Retreives vlnId entry from VLAN table. List manager.
*
* INPUTS:
*       [In] Uint32 vlnId
*
*******************************************************************************/

CSL_SWITCH_VLAN_ENTRY * cslGetVLANtable(void)
{
    return pVlanTable;
}


/*******************************************************************************
* cslGetVLANEntry - gets VLAN entry
*
*  Retreives vlnId entry from VLAN table. List manager.
*
* INPUTS:
*       [In] Uint32 vlnId
*
*******************************************************************************/
CSL_SWITCH_VLAN_ENTRY * cslGetVLANEntry(Uint32 vlnId)
{
    CSL_SWITCH_VLAN_ENTRY * tmp;

    for (tmp = pVlanTable;tmp != NULL; tmp = tmp->next)
    {
        if(tmp->vid == vlnId)
        {
            return tmp;
        }
    }

    return NULL;
}

