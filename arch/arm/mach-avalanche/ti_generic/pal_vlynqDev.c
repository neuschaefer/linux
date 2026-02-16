/*
 * pal_vlynqDev.c
 * Description:
 * See below.
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
 */

/******************************************************************************
 * FILE PURPOSE:    PAL VLYNQ Device File
 ******************************************************************************
 * FILE NAME:       pal_vlynqDev.c
 *
 * DESCRIPTION:     PAL VLYNQ Device File
 *
 *******************************************************************************/

#include "pal_vlynqDev.h"

typedef struct pal_vlynq_dev_cb_t
{
    PAL_VLYNQ_DEV_CB_FN       cb_fn;
    void                      *cb_param;
    struct pal_vlynq_dev_cb_t *next;

} PAL_VLYNQ_DEV_CB_T;

#define CB_COUNT_PER_DEV 2

static PAL_VLYNQ_DEV_CB_T vlynq_dev_cb[MAX_DEV_COUNT*CB_COUNT_PER_DEV];
static PAL_VLYNQ_DEV_CB_T *gp_free_vlynq_dev_cb = NULL;

static PAL_Result vlynq_dev_cb_init(void)
{
    PAL_VLYNQ_DEV_CB_T *p_vlynq_dev_cb = gp_free_vlynq_dev_cb = &vlynq_dev_cb[0];
    Int32 i = 0;

    for(i = 0; i < (CB_COUNT_PER_DEV*MAX_DEV_COUNT); i++)
    {
        p_vlynq_dev_cb->cb_param = NULL;
	p_vlynq_dev_cb->cb_fn    = NULL;
	
	p_vlynq_dev_cb->next = (i == (CB_COUNT_PER_DEV*MAX_DEV_COUNT) - 1) ? NULL: &vlynq_dev_cb[i+1];

	p_vlynq_dev_cb = p_vlynq_dev_cb->next;
    }
   
    return (PAL_VLYNQ_OK);
}

static PAL_VLYNQ_DEV_CB_T *vlynq_dev_get_free_dev_cb(void)
{
    PAL_VLYNQ_DEV_CB_T *p_vlynq_dev_cb;

    p_vlynq_dev_cb = gp_free_vlynq_dev_cb;
    if(p_vlynq_dev_cb)
        gp_free_vlynq_dev_cb = gp_free_vlynq_dev_cb->next;

    return (p_vlynq_dev_cb);
}

static void vlynq_dev_free_dev_cb(PAL_VLYNQ_DEV_CB_T *p_vlynq_dev_cb)
{
    p_vlynq_dev_cb->cb_param = NULL;
    p_vlynq_dev_cb->cb_fn    = NULL;
    p_vlynq_dev_cb->next     = gp_free_vlynq_dev_cb;
    gp_free_vlynq_dev_cb     = p_vlynq_dev_cb;
}

typedef struct pal_vlynq_dev_t
{
    char                   name[32];
    PAL_VLYNQ_HND          *vlynq;
    struct pal_vlynq_dev_t *next;
    Uint8                  reset_bit;
    Uint8                  irq_count;
    Uint8                  instance;
    Bool                   peer;
    Int8                   irq[8];
    PAL_VLYNQ_DEV_CB_T     *dev_cb;    

} PAL_VLYNQ_DEV_T;

PAL_VLYNQ_DEV_T vlynq_dev[MAX_DEV_COUNT];
static PAL_VLYNQ_DEV_T *gp_free_vlynq_dev_list = NULL;
static PAL_VLYNQ_DEV_T *gp_used_vlynq_dev_list = NULL;

static PAL_VLYNQ_DEV_T *vlynq_dev_get_free_dev(void)
{
    PAL_VLYNQ_DEV_T *p_vlynq_dev;

    p_vlynq_dev = gp_free_vlynq_dev_list;
    if(p_vlynq_dev)
    {
        gp_free_vlynq_dev_list = gp_free_vlynq_dev_list->next;
	p_vlynq_dev->next      = gp_used_vlynq_dev_list;
	gp_used_vlynq_dev_list = p_vlynq_dev;
    }

    return (p_vlynq_dev);
}

static void vlynq_dev_free_dev(PAL_VLYNQ_DEV_T *p_vlynq_dev)
{
    PAL_VLYNQ_DEV_T *p_tmp_vlynq_dev;
    PAL_VLYNQ_DEV_T *p_last_vlynq_dev;

    for(p_tmp_vlynq_dev  = gp_used_vlynq_dev_list, p_last_vlynq_dev = NULL;
	p_tmp_vlynq_dev != NULL;
	p_last_vlynq_dev = p_tmp_vlynq_dev, p_tmp_vlynq_dev = p_tmp_vlynq_dev->next)
    {
        if(p_tmp_vlynq_dev == p_vlynq_dev)
	{
            if(!p_last_vlynq_dev)
	        gp_used_vlynq_dev_list = gp_used_vlynq_dev_list->next;
	    else
		p_last_vlynq_dev->next = p_tmp_vlynq_dev->next;

            break;
	}
    }

    if(!p_tmp_vlynq_dev)
	goto vlynq_dev_free_dev_exit;

    p_vlynq_dev->next      = gp_free_vlynq_dev_list;
    gp_free_vlynq_dev_list = p_vlynq_dev;

vlynq_dev_free_dev_exit:

    return;
}

PAL_Result PAL_vlynqDevCbRegister(PAL_VLYNQ_DEV_HND   *vlynq_dev, 
	   	                     PAL_VLYNQ_DEV_CB_FN cb_fn,
				     void                *this_driver)
{
    PAL_VLYNQ_DEV_T    *p_vlynq_dev = (PAL_VLYNQ_DEV_T*)vlynq_dev;
    PAL_VLYNQ_DEV_CB_T *p_dev_cb;
    Uint32             cookie;

    if(!p_vlynq_dev || !this_driver)
        return (PAL_VLYNQ_INVALID_PARAM);

    PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &cookie);
    p_dev_cb = vlynq_dev_get_free_dev_cb();
    PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);

    if(!p_dev_cb)
        return (PAL_VLYNQ_INTERNAL_ERROR);

    p_dev_cb->cb_fn    = cb_fn;
    p_dev_cb->cb_param = this_driver;

    p_dev_cb->next      = p_vlynq_dev->dev_cb;
    p_vlynq_dev->dev_cb = p_dev_cb; 

    return (PAL_VLYNQ_OK);
}

PAL_Result PAL_vlynqDevCbUnregister(PAL_VLYNQ_DEV_HND *vlynq_dev,
     		                       void              *this_driver)
{
    PAL_Result         ret_val = PAL_VLYNQ_INVALID_PARAM;
    PAL_VLYNQ_DEV_T    *p_vlynq_dev = vlynq_dev;
    PAL_VLYNQ_DEV_CB_T *p_dev_cb, *p_last_dev_cb;

    if(!p_vlynq_dev)
        return (ret_val);

    for(p_dev_cb  = p_vlynq_dev->dev_cb, p_last_dev_cb = NULL;
	p_dev_cb != NULL;
	p_last_dev_cb = p_dev_cb, p_dev_cb = p_dev_cb->next)
    {
        if(p_dev_cb->cb_param == this_driver)
	{
	    Uint32 cookie;

	    if(!p_last_dev_cb)
	        p_vlynq_dev->dev_cb = NULL;
	    else
	        p_last_dev_cb->next = p_dev_cb->next;

            PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &cookie);
	    vlynq_dev_free_dev_cb(p_dev_cb);
            PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);

	    ret_val = PAL_VLYNQ_OK;
	    break;
	}
    }

    return (ret_val);
}

PAL_VLYNQ_DEV_HND *PAL_vlynqDevFind(char *name, Uint8 instance)
{
    PAL_VLYNQ_DEV_T *p_vlynq_dev;

    for(p_vlynq_dev = gp_used_vlynq_dev_list;
	p_vlynq_dev != NULL;
	p_vlynq_dev = p_vlynq_dev->next)
    {
        if(!strcmp(p_vlynq_dev->name, name) &&
	    (p_vlynq_dev->instance == instance))
	    break;
    }
    
    return (p_vlynq_dev);
}

PAL_VLYNQ_HND *PAL_vlynqDevGetVlynq(PAL_VLYNQ_DEV_HND *vlynq_dev)
{
    PAL_VLYNQ_DEV_T *p_vlynq_dev = (PAL_VLYNQ_DEV_HND*) vlynq_dev;

    if(p_vlynq_dev)
      return (p_vlynq_dev->vlynq);
    else
        return (NULL);	    
}

PAL_Result pal_vlynq_dev_ioctl(PAL_VLYNQ_DEV_HND *vlynq_dev, Uint32 cmd, 
		               Uint32 cmd_val)
{
    PAL_VLYNQ_DEV_T *p_vlynq_dev = vlynq_dev;
    
    if(!p_vlynq_dev)
        return (PAL_VLYNQ_INVALID_PARAM);

    switch(cmd)
    {
        case PAL_VLYNQ_DEV_ADD_IRQ:
	{
	    Int32 i;    

	    if(p_vlynq_dev->irq_count == MAX_IRQ_PER_VLYNQ)
	        return (-2);

            for(i = 0; i < MAX_IRQ_PER_VLYNQ; i++)
	    {
	        if(p_vlynq_dev->irq[i] == -1)
		{
		    p_vlynq_dev->irq[i] = cmd_val;
                    p_vlynq_dev->irq_count++;
                    break;
		}
            }
        }
	    break;

	case PAL_VLYNQ_DEV_REMOVE_IRQ:
	{
	    Int32 i;

	    if(p_vlynq_dev->irq_count == 0)
		 return (-2);

	    for(i = 0; i < MAX_IRQ_PER_VLYNQ; i++)
	    {
	        if(p_vlynq_dev->irq[i] == cmd_val)
		{
		    p_vlynq_dev->irq[i] = -1;
		    p_vlynq_dev->irq_count--;
		    break;
		} 
            }

            if(i== MAX_IRQ_PER_VLYNQ)
	        return (PAL_VLYNQ_INTERNAL_ERROR);
        }
	    break;

	default:
	     break;
    } 

    return(PAL_VLYNQ_OK);
}

PAL_Result PAL_vlynqDevFindIrq(PAL_VLYNQ_DEV_HND *vlynq_dev, Uint8 *irq, 
		                  Uint32 num_irq)
{
    PAL_VLYNQ_DEV_T *p_vlynq_dev = vlynq_dev;
    Int32           i = 0;
    Int8            *p_dev_irq;

    if(!p_vlynq_dev)
        return (PAL_VLYNQ_INVALID_PARAM);

    if(p_vlynq_dev->irq_count != num_irq)
	return (PAL_VLYNQ_INTERNAL_ERROR);

    p_dev_irq = p_vlynq_dev->irq;

    for(i = 0; i < MAX_IRQ_PER_VLYNQ; i++)
    {
        if(p_dev_irq[i] != -1)
	    irq[--num_irq] = p_dev_irq[i];
    }

    return (PAL_VLYNQ_OK);
 
}

PAL_VLYNQ_DEV_HND* PAL_vlynqDevCreate(PAL_VLYNQ_HND *vlynq, char *name,
			              Uint32 instance, Int32 reset_bit,
				      Bool peer)
{
    PAL_VLYNQ_DEV_T *p_vlynq_dev = NULL;

    p_vlynq_dev = vlynq_dev_get_free_dev();
    if(!p_vlynq_dev || !vlynq)
        return (NULL);

    strcpy(p_vlynq_dev->name, name);
    p_vlynq_dev->instance  = instance;
    p_vlynq_dev->reset_bit = reset_bit;
    p_vlynq_dev->vlynq     = vlynq;
    p_vlynq_dev->peer      = peer;

    return (p_vlynq_dev);
}

PAL_Result PAL_vlynqDevDestroy(PAL_VLYNQ_DEV_HND *vlynq_dev)
{
    PAL_VLYNQ_DEV_T *p_vlynq_dev = vlynq_dev;
    int j;

    if(!p_vlynq_dev)
        return (PAL_VLYNQ_INVALID_PARAM);

    if(p_vlynq_dev->dev_cb)
        return (PAL_VLYNQ_INTERNAL_ERROR);

    p_vlynq_dev->vlynq      = NULL;
    p_vlynq_dev->irq_count  = 0;
    
    for(j = 0; j < MAX_IRQ_PER_VLYNQ; j++)
    {
        p_vlynq_dev->irq[j] = -1;
    }

    vlynq_dev_free_dev(p_vlynq_dev);

    return (PAL_VLYNQ_OK);
}

PAL_Result PAL_vlynqDevGetResetBit(PAL_VLYNQ_DEV_HND *vlynq_dev, Uint32 *reset_bit)
{
    PAL_VLYNQ_DEV_T *p_vlynq_dev = vlynq_dev;
   
    if(!p_vlynq_dev)
        return (PAL_VLYNQ_INVALID_PARAM);

    *reset_bit = p_vlynq_dev->reset_bit;
  
    return (PAL_VLYNQ_OK);
}

PAL_Result pal_vlynq_dev_init(void)
{
    PAL_VLYNQ_DEV_T *p_vlynq_dev = gp_free_vlynq_dev_list = &vlynq_dev[0];
    Uint32 i, j;

    for(i = 0; i < MAX_DEV_COUNT; i++)
    {
        p_vlynq_dev->vlynq     = NULL;
	p_vlynq_dev->irq_count = 0;
        p_vlynq_dev->dev_cb    = NULL;

	for(j = 0; j < MAX_IRQ_PER_VLYNQ; j++)
	{
            p_vlynq_dev->irq[j] = -1;
	}

	p_vlynq_dev->next = (i==MAX_DEV_COUNT-1) ? NULL: &vlynq_dev[i+1];
	p_vlynq_dev       = p_vlynq_dev->next;
    }

    vlynq_dev_cb_init();

    return (PAL_VLYNQ_OK);
}

PAL_Result pal_vlynq_dev_handle_event(PAL_VLYNQ_DEV_HND *vlynq_dev,
                                      Uint32 cmd, Uint32 val)
{
    PAL_VLYNQ_DEV_T *p_vlynq_dev = vlynq_dev;
    PAL_Result      ret_val      = PAL_VLYNQ_INTERNAL_ERROR;
 
    if(p_vlynq_dev)
    {
        PAL_VLYNQ_DEV_CB_T *p_dev_cb;

        for(p_dev_cb  = p_vlynq_dev->dev_cb;
	    p_dev_cb != NULL;
	    p_dev_cb  = p_dev_cb->next)
        {	    
            p_dev_cb->cb_fn(p_dev_cb->cb_param, cmd, val);
        }

        ret_val = PAL_VLYNQ_OK;
    }

    return (ret_val);
}

Int32 vlynq_dev_dump_dev(PAL_VLYNQ_DEV_HND *vlynq_dev, char *buf, Int32 limit, 
                         Int32 *eof)
{
    Int32 irq_count, len = 0;
    PAL_VLYNQ_DEV_T  *p_vlynq_dev = vlynq_dev;

    if(!p_vlynq_dev)
        return(len);

    if(len < limit)
        len += sprintf(buf + len, "    %s%d\n", p_vlynq_dev->name, 
                       p_vlynq_dev->instance);

    if(len < limit)
        len += sprintf(buf + len, "      IRQ-id(s): ");

    for (irq_count = 0; irq_count < MAX_IRQ_PER_VLYNQ; irq_count++)
    {
        Int32 val;

        if(p_vlynq_dev->irq[irq_count] == -1)
            continue;

        val = p_vlynq_dev->irq[irq_count];
        
        if(len < limit)
            len += sprintf(buf + len, "%d, ", val);
    }

    if(len < limit)
        len += sprintf(buf + len, "\n");

    return(len);
}
