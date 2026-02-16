/*
 *
 * dda_switch.c
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
/*
Includes Intel Corporation's changes/modifications dated: 2014.
Changed/modified portions - Copyright © 2014, Intel Corporation.
*/


/*****************************************************************************
 * FILE PURPOSE:     - External Switch functions Source
 ******************************************************************************
 * FILE NAME:     dda_switch.c
 *
 * DESCRIPTION:   External Switch IOCTL and proc entries implementation.
 *
 * REVISION HISTORY:
 * 1 FEB 2004
 *
 *******************************************************************************/

#include <linux/module.h>
#include <asm/uaccess.h>
#include <linux/proc_fs_macros.h>
#include <linux/version.h>

#include "dda_switch.h"
#include "ddc_switch.h"

#define PROC_END_DEBUG  0
#define procPrint if (PROC_END_DEBUG) printk


#define LAN0_PORT               0
#define LAN1_PORT               1
#define LAN2_PORT               2
#define LAN3_PORT               3
#define LAN4_PORT               4
#define LAN5_PORT               5
#define LAN6_PORT               6

static struct ctl_table_header *port_config_dir = NULL;
static struct proc_dir_entry *port_status_dir = NULL;
static struct proc_dir_entry *port_lan_status_file[LAN6_PORT + 1];

static int switch_reset_config(ctl_table *table, int write, void __user *buffer, size_t *lenp, loff_t *ppos);
static int switch_port_read_config(char* buf, int PortNum, loff_t *ppos);
static int switch_port_config(ctl_table *table, int write, void __user *buffer, size_t *lenp, loff_t *ppos);
static ssize_t switch_port_status(struct seq_file *m, void *v);

static struct ctl_table ext_switch_instance_sysctl_table[] = {
    {
        .procname       = "reset",
        .data           = (void *)0,
        .mode		    = 0644,
        .proc_handler	= switch_reset_config,
        .child = NULL,
    },
    {
        .procname       = "0",
        .data           = (void *)0,
        .mode		    = 0644,
        .proc_handler	= switch_port_config,
        .child = NULL,
    },
    {
        .procname       = "1",
        .data           = (void *)1,
        .mode		    = 0644,
        .proc_handler	= switch_port_config,
        .child = NULL,
    },
    {
        .procname       = "2",
        .data           = (void *)2,
        .mode		    = 0644,
        .proc_handler	= switch_port_config,
        .child = NULL,
    },
    {
        .procname       = "3",
        .data           = (void *)3,
        .mode		    = 0644,
        .proc_handler	= switch_port_config,
        .child = NULL,
    },
    {
        .procname       = "4",
        .data           = (void *)4,
        .mode		    = 0644,
        .proc_handler	= switch_port_config,
        .child = NULL,
    },
    {
        .procname       = "5",
        .data           = (void *)5,
        .mode		    = 0644,
        .proc_handler	= switch_port_config,
        .child = NULL,
    },
    {
        .procname       = "6",
        .data           = (void *)6,
        .mode		    = 0644,
        .proc_handler	= switch_port_config,
        .child = NULL,
    },
	{ .procname = NULL}
};

static struct ctl_table ext_switch_dir_sysctl_table[] = {
	{
		.procname	= "ext_switch",
        .mode		= 0644,
        .child      = ext_switch_instance_sysctl_table,
	},
	{ }
};

static struct ctl_table ext_switch_sysctl_dev[] = {
	{
		.procname	= "dev",
        .mode		= 0644,
        .child      = ext_switch_dir_sysctl_table,
	},
	{ }
};

DECLARE_PROCFS_READ_ENTRY(switch_port_status, switch_port_status)


/*****************************************************************************************************/
/*                                    PROC FUNCTIONS                                                 */
/*****************************************************************************************************/

/******************************************************************************************************/
/*switch_create_proc_entry - Create two proc entries one for read/write named sys/dev/marvell         */
/*                           and one for read named avalanche/marvell.                                */
/*                                                                                                    */
/******************************************************************************************************/
void switch_create_proc_entry(void)
{
    int i,index;

    /**** Create Read and Write entry for each Port ****/
    /* Create ext_switch */
    port_config_dir = register_sysctl_table(ext_switch_sysctl_dev);

   /**** Create Read entry for each Port ****/
   port_status_dir = proc_mkdir("avalanche/ext_switch", NULL);
   for(i = LAN0_PORT, index = 0; i<= LAN6_PORT ;i++,index++)
   {
      char str[10];

      sprintf(str,"%d",index);

      port_lan_status_file[index] = proc_create_data(str, 0444, port_status_dir, &switch_port_status_proc_fops, (void*) index);
      
   }
   return;
}

/******************************************************************************************************/
/*switch_remove_proc_entry - Remove the ExtSwitch proc entries named sys/dev/marvell                  */
/*                           and avalanche/marvell.                                                   */
/*                                                                                                    */
/******************************************************************************************************/
void switch_remove_proc_entry(void)
{
   int i,index;

   if(port_config_dir)
   {
      for(i = LAN0_PORT, index = 0; i<= LAN6_PORT ;i++,index++)
      {
         char str[10];

         sprintf(str,"%d",index + 1);
         remove_proc_entry(str, port_status_dir);

      }
      unregister_sysctl_table(port_config_dir);
      remove_proc_entry("avalanche/ext_switch", NULL);
   }
}


/*****************************************************************************************************/
/* switch_port_read_config - The read call back function for the proc entry.                        */
/*****************************************************************************************************/
static int switch_port_read_config(char* buf, int PortNum, loff_t *ppos)
{
   int len;
   unsigned int port_status = 0,Duplex,Speed;
   char str[22];

   if (*ppos != 0)
   {
       /* Data already written for this transaction ==> EOF */
       return 0;
   }

   procPrint("read ....\n");
   if(switch_portLinkGet(PortNum) == PORT_LINK_DOWN)
   {
      procPrint("LINK is down Port num = %d\n",PortNum);
      port_status = 5;
   }
   else
   {

      if( switch_portAutoGet(PortNum) == PORT_AUTO_YES)
      {
         procPrint("Auto negt. is enabled \n");
         port_status = 0;

      }else /* NO AUTO */
      {

         Duplex = switch_portDuplexGet(PortNum);
         Speed = switch_portSpeedGet(PortNum);

         if( Speed == PORT_SPEED_100)
         {
            if(Duplex == PORT_DUPLEX_FULL)
            {
               procPrint("Speed is 100Mbps FULL Duplex.\n");
               port_status = 4;
            }else
            {
               procPrint("Speed is 10oMbps HALF Duplex.\n");
               port_status = 3;
            }
         }else
         {

            if(Duplex == PORT_DUPLEX_FULL)
            {
               procPrint("Speed is 10Mbps FULL Duplex.\n");
               port_status = 2;
            }else
            {
               procPrint("Speed is 10Mbps HALF Duplex.\n");
               port_status = 1;
            }

     }
      }
   }

   len = snprintf(str, sizeof(str) - 1, "%d", port_status);
   if (copy_to_user(buf, str, len) != 0)
   {
       return -EFAULT;
   }
   else
   {
       /* Report amount of used buffer */
       *ppos += len;
       return len;
   }
}


/*****************************************************************************************************/
/*switch_reset_config - The write call back function for the proc entry.                              */
/*                      *table - sysctl table written to for this transaction                        */
/*                      *buffer - contain the user command :                                         */
/*                      *lenp - Length of input, upon completion, update used input                  */
/*                      *ppos - Position in file                                                     */
/*****************************************************************************************************/
static int switch_reset_config(ctl_table *table, int write, void __user *buffer, size_t *lenp, loff_t *ppos)
{
    unsigned long count = *lenp;

    if (write)
    {
        switch_reset(0);

        return count;
    }
    else
    {
        if (*ppos != 0)
        {
            /* Data already written for this transaction ==> EOF */
            *lenp = 0;
            return 0;
        }
        if (copy_to_user(buffer, "0", 1) != 0)
        {
            return -EFAULT;
        }
        else
        {
            /* Report amount of used buffer */
            *ppos += 1;
            *lenp = 1;
            return 0;
        }
    }

    return 0;
}

/*****************************************************************************************************/
/*switch_port_config - The write call back function for the proc entry.                              */
/*                      *table - sysctl table written to for this transaction                        */
/*                      *buffer - contain the user command :                                         */
/*                              0 - Restart Auto Negotiation.                                        */
/*                              1 - Disable Auto Negotiation and Set to Half Duplex 10 Mbps.         */
/*                              2 - Disable Auto Negotiation and Set to Full Duplex 10 Mbps.         */
/*                              3 - Disable Auto Negotiation and Set to Half Duplex 100 Mbps.        */
/*                              4 - Disable Auto Negotiation and Set to Full Duplex 100 Mbps.        */
/*                              vlan commands ...                                                    */
/*                      *lenp - Length of input, upon completion, update used input                  */
/*                      *ppos - Position in file                                                     */
/*****************************************************************************************************/
static int switch_port_config(ctl_table *table, int write, void __user *buffer, size_t *lenp, loff_t *ppos)
{
    char    proc_cmd[100];
    char*   argv[10];
    int     argc = 0;
    char*   ptr_cmd;
    char*   delimitters = " \n\t";
    char*   ptr_next_tok;
    int     PortNum;
    unsigned long count = *lenp;

    PortNum = (int)table->data;

    if (!write)
    {
        /* Read */
        *lenp = switch_port_read_config(buffer, PortNum, ppos);
        return 0;
    }

    /* Write */

    /* Validate the length of data passed. */
    if (count > sizeof(proc_cmd))
        count = sizeof(proc_cmd);

    /* Initialize the buffer before using it. */
    memset ((void *)&proc_cmd[0], 0, sizeof(proc_cmd));
    memset ((void *)&argv[0],     0, sizeof(argv));

    /* Copy from user space. */
    if (copy_from_user (&proc_cmd, buffer, count))
        return -EFAULT;

    ptr_next_tok = &proc_cmd[0];

    /* Tokenize the command. Check if there was a NULL entry. If so be the case the
     * user did not know how to use the entry. Print the help screen. */
    ptr_cmd = strsep(&ptr_next_tok, delimitters);
    if (ptr_cmd == NULL)
        return -1;

    /* Parse all the commands typed. */
    do
    {
        /* Extract the first command. */
        argv[argc++] = ptr_cmd;

        /* Validate if the user entered more commands.*/
        if (argc >=10)
        {
            printk ("ERROR: Incorrect too many parameters dropping the command\n");
            return -EFAULT;
        }

        /* Get the next valid command. */
        ptr_cmd = strsep(&ptr_next_tok, delimitters);
    } while (ptr_cmd != NULL);

    /* We have an extra argument when strsep is used instead of strtok */
    argc--;

    /******************************* Command Handlers *******************************/

    if(strcmp("0", argv[0]) == 0)
    {
        if( switch_portAutoGet(PortNum) == PORT_AUTO_YES)
        {
            procPrint("Auto negt is already enabled, Restart Auto Negt.\n");
            switch_portAutoSet(PortNum,PORT_AUTO_RESTART);
        }else
        {
            procPrint("Auto negt is disabled, Enable Auto Negt.\n");
            switch_portAutoSet(PortNum,PORT_AUTO_YES);
        }
    }
    else

    if(strcmp("1", argv[0]) == 0)
    {
        switch_portAutoSet(PortNum,PORT_AUTO_NO);
        switch_portDuplexSet(PortNum,PORT_DUPLEX_HALF);
        switch_portSpeedSet(PortNum,PORT_SPEED_10);
        procPrint("Disable auto negt.And force to 10Mbps Half Duplex\n");
    }
    else

    if(strcmp("2", argv[0]) == 0)
    {
        switch_portAutoSet(PortNum,PORT_AUTO_NO);
        switch_portDuplexSet(PortNum,PORT_DUPLEX_FULL);
        switch_portSpeedSet(PortNum,PORT_SPEED_10);
        procPrint("Disable auto negt.And force to 10Mbps Full Duplex\n");
    }
    else

    if(strcmp("3", argv[0]) == 0)
    {
        switch_portAutoSet(PortNum,PORT_AUTO_NO);
        switch_portDuplexSet(PortNum,PORT_DUPLEX_HALF);
        switch_portSpeedSet(PortNum,PORT_SPEED_100);
        procPrint("Disable auto negt.And force to 100Mbps Half Duplex\n");
    }
    else

    if(strcmp("4", argv[0]) == 0)
    {
        switch_portAutoSet(PortNum,PORT_AUTO_NO);
        switch_portDuplexSet(PortNum,PORT_DUPLEX_FULL);
        switch_portSpeedSet(PortNum,PORT_SPEED_100);
        procPrint("Disable auto negt.And force to 100Mbps Full Duplex\n");
    }
    else

    /************************************************************************/
    /*                                                                      */
    /*                VLAN COMMAND                                          */
    /*                                                                      */
    /************************************************************************/
    if (0 == strcmp(argv[0], "vlan"))
    {
        if (0 == strcmp(argv[1], "create"))
        {
            switch_vlanCreate( simple_strtol(argv[2], NULL, 16) );
        }
        else
        if (0 == strcmp(argv[1], "delete"))
        {
            switch_vlanDelete( simple_strtol(argv[2], NULL, 16) );
        }
        else
        if (0 == strcmp(argv[1], "portAdd"))
        {
            VLN_TAG   tag;            /* port tagged/untagged/unchanged */

            if (0 == strcmp(argv[3], "tag"))
            {
                tag = VLN_PORT_TAG;
            }else
            if (0 == strcmp(argv[3], "untag"))
            {
                tag = VLN_PORT_UNTAG;
            }else
            if (0 == strcmp(argv[3], "unchanged"))
            {
                tag = VLN_PORT_UNMODIFIED;
            }else
            {
                return count;
            }
            switch_vlanPortAdd( simple_strtol(argv[2], NULL, 16), PortNum, tag );
        }
        else
        if (0 == strcmp(argv[1], "portDel"))
        {
            switch_vlanPortDelete( PortNum, simple_strtol(argv[2], NULL, 16) );
        }
        else
        {
            printk(" Error vlan command parsing port=%d [%s][%s][%s] ...\n", PortNum, argv[1], argv[2], argv[3]);
        }
    }
    else

    /************************************************************************/
    /*                                                                      */
    /*                DEBUGGING COMMAND                                     */
    /*                                                                      */
    /************************************************************************/
    if (0 == strcmp(argv[0], "reg"))
    {
        if (0 == strcmp(argv[1], "rd"))
        {
            Uint32 data;
            Uint32 addr = simple_strtol(argv[2], NULL, 16);

            switch_dbgRegRead( addr, &data );

            printk(" Switch REG[0x%08X] => 0x%08X\n", addr, data );
        }
        else
        if (0 == strcmp(argv[1], "wr"))
        {
            Uint32 addr = simple_strtol(argv[2], NULL, 16);
            Uint32 data = simple_strtol(argv[3], NULL, 16);

            switch_dbgRegWrite( addr, &data );

            printk(" Switch REG[0x%08X] <= 0x%08X\n", addr, data );
        }
        else
        {
            printk(" Error reg command parsing ...\n");
        }
    }

    return 0;
}


/*****************************************************************************************************/
/*switch_port_status - The read call back function for the proc entry.                               */
/*                      *data - Port Number                                                          */
/*                      RETURN - The Speed and Duplex of the port. The Returns value can be:         */
/*                              1 - Half Duplex 10 Mbps.                                             */
/*                              2 - Full Duplex 10 Mbps.                                             */
/*                              3 - Half Duplex 100 Mbps.                                            */
/*                              4 - Full Duplex 100 Mbps.                                            */
/*                              5 - Link is Down.                                                    */
/******************************************************************************************************/
static ssize_t switch_port_status(struct seq_file *m, void *v)
{
    unsigned int port_status = 0,Duplex,Speed;
    int PortNum = (int)m->private;

    procPrint("read ....\n");
    if(switch_portLinkGet(PortNum) == PORT_LINK_DOWN)
    {
        procPrint("LINK is down Port num = %d\n",PortNum);
        port_status = 5;
    }
    else
    {

        if( switch_portAutoGet(PortNum) == PORT_AUTO_YES)
        {
            procPrint("Auto negt. is enabled \n");
        }

        Duplex = switch_portDuplexGet(PortNum);
        Speed = switch_portSpeedGet(PortNum);

        if( Speed == PORT_SPEED_100)
        {
            if(Duplex == PORT_DUPLEX_FULL)
            {
                procPrint("Speed is 100Mbps FULL Duplex.\n");
                port_status = 4;
            }else
            {
                procPrint("Speed is 100Mbps HALF Duplex.\n");
                port_status = 3;
            }
        }else
        {

            if(Duplex == PORT_DUPLEX_FULL)
            {
                procPrint("Speed is 10Mbps FULL Duplex.\n");
                port_status = 2;
            }else
            {
                procPrint("Speed is 10Mbps HALF Duplex.\n");
                port_status = 1;
            }
        }
    }

    seq_printf(m, "%d\n", port_status);

    return 0;
}


/*********************************************************************/
/*                  IOCTL FUNCTIONS                                  */
/*********************************************************************/

int external_switch_ioctl(SwitchIoctlType *ioctl_cmd)
{
    int ret = 0;


    switch (ioctl_cmd->t_opcode)
    {

            /* SWITCH MANAGEMENT */

        case SWITCH_GETVERSION:
         ret = switch_getVersion(&ioctl_cmd->msg.SWITCH_verinfo.verinfo);
            break;

        case SWITCH_SPECCAPGET:
         ret = switch_specCapsGet(&ioctl_cmd->msg.SWITCH_specialcap.SpecialCap);
            break;

        case SWITCH_SPECCAPSET:
         ret = switch_specCapsSet(&ioctl_cmd->msg.SWITCH_specialcap.SpecialCap);
            break;

        case SWITCH_TRAILERSET:
         ret = switch_trailerSet(ioctl_cmd->msg.PORT_config.Status);
            break;

            /* PORT MANAGEMENT */

        case PORT_LINKGET:
         ret = (int)switch_portLinkGet(ioctl_cmd->msg.PORT_config.PortNum);
          ioctl_cmd->msg.PORT_config.Status = ret;
            break;

        case PORT_SPEEDSET:
         ret = switch_portSpeedSet(ioctl_cmd->msg.PORT_config.PortNum,(PORT_SPEED)ioctl_cmd->msg.PORT_config.Status/*Speed*/);
            break;

        case PORT_SPEEDGET:
         ret = switch_portSpeedGet(ioctl_cmd->msg.PORT_config.PortNum);
          ioctl_cmd->msg.PORT_config.Status = ret;
            break;

      case PORT_AUTOSET:
         ret = switch_portAutoSet(ioctl_cmd->msg.PORT_config.PortNum,(PORT_AUTO)ioctl_cmd->msg.PORT_config.Status/*autoStatus*/);
         break;

        case PORT_AUTOGET:
         ret = switch_portAutoGet(ioctl_cmd->msg.PORT_config.PortNum);
         ioctl_cmd->msg.PORT_config.Status = ret; /*autoStatus*/
            break;

      case PORT_DUPLEXSET:
         ret = switch_portDuplexSet(ioctl_cmd->msg.PORT_config.PortNum,(PORT_DUPLEX)ioctl_cmd->msg.PORT_config.Status/*DuplexStatus*/);
            break;

      case PORT_DUPLEXGET:
         ret = switch_portDuplexGet(ioctl_cmd->msg.PORT_config.PortNum/*port num*/);
         ioctl_cmd->msg.PORT_config.Status = ret; /*DuplexStatus*/
         break;

      case PORT_STATUSSET:
         ret = switch_portStatusSet(ioctl_cmd->msg.PORT_config.PortNum,ioctl_cmd->msg.PORT_config.Status);
         break;

      case PORT_STATUSGET:
         ret = switch_portStatusGet(ioctl_cmd->msg.PORT_config.PortNum);
         ioctl_cmd->msg.PORT_config.Status = ret;
        break;

      case PORT_FCSET:
         ret = switch_portFCSet(ioctl_cmd->msg.PORT_config.PortNum,ioctl_cmd->msg.PORT_config.Status);
         break;

      case PORT_FCGET:
         ret = switch_portFCGet(ioctl_cmd->msg.PORT_config.PortNum);
         ioctl_cmd->msg.PORT_config.Status = ret;
         break;

         /* S T A T I S T I C S */

      case PORT_STATISTICGET:
         ret = switch_statPortGet(ioctl_cmd->msg.PORT_counters.PortNum,&ioctl_cmd->msg.PORT_counters.Counters);
         break;

      case PORT_STATISTICRESET:
         ret = switch_statPortReset(ioctl_cmd->msg.PORT_counters.PortNum);
         break;

         /*    VLAN SUBMODULE  */
      case VLAN_CREATE:     /* Creates VLAN with the given ID. */
         ret = switch_vlanCreate(ioctl_cmd->msg.VLAN_info.vlnId);
         break;


      case VLAN_DELETE:     /* Deletes VLAN. */
         ret = switch_vlanDelete(ioctl_cmd->msg.VLAN_info.vlnId);
         break;

      case VLAN_PORT_ADD:       /* Adds port to VLAN. */
         ret = switch_vlanPortAdd(ioctl_cmd->msg.VLAN_info.vlnId,ioctl_cmd->msg.VLAN_info.PortNum,ioctl_cmd->msg.VLAN_info.tag);
         break;

      case VLAN_PORT_DELETE:        /* Deletes port from VLAN. */
         ret = switch_vlanPortDelete(ioctl_cmd->msg.VLAN_info.PortNum,ioctl_cmd->msg.VLAN_info.vlnId);

         break;

      case VLAN_SET_DEF:        /* Sets default VLAN for the given port. */
         ret = switch_vlanDefaultSet(ioctl_cmd->msg.VLAN_info.PortNum,ioctl_cmd->msg.VLAN_info.vlnId);

         break;

      case VLAN_PORT_UPDATE:        /* Updates port status in VLAN. */
         ret = switch_vlanPortUpdate(ioctl_cmd->msg.VLAN_info.PortNum,ioctl_cmd->msg.VLAN_info.vlnId,ioctl_cmd->msg.VLAN_info.tag);
         break;

      case VLAN_SET_PORT_PRI:       /* Sets the priority of a port. */
         ret = switch_vlanSetPortPriority(ioctl_cmd->msg.VLAN_info.PortNum,ioctl_cmd->msg.VLAN_info.priority);
         break;

      case VLAN_GET_PORT_PRI:       /* Gets the priority of a port. */
         ret = switch_vlanGetPortPriority(ioctl_cmd->msg.VLAN_info.PortNum);

         break;

      case DELETE_MAC_ADDR:
         ret = switch_deleteMacAddrFromAtu(ioctl_cmd->msg.MAC_addr.MacAddr);

         break;

        default:
            ret = -EINVAL;
    }

   /* IOCTL - return 0 on Sucsess or -EINVAL on Error */
   if(ret == PAL_False || ret == -EINVAL)
      return -EINVAL;
   else
       return 0;
}
