/*
 *
 *  Copyright (C)2006 ST MICROELECTRONICS
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
* This module provides some support routines for the TIMER (DTU/MTU)
*------------------------------------------------------------------------------
*/

/*----------------------------------------------------------------------------*
 * Includes                                                                   *
 *----------------------------------------------------------------------------*/
#include <asm/arch/spr_timer_irq.h>
#include <asm/arch/spr_timer_irqp.h>
#include <linux/kernel.h>

/*----------------------------------------------------------------------------*
 * Defines
 *----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Variables
 *----------------------------------------------------------------------------*/
PRIVATE t_tmr_register  *gp_tmr_register[TMR_INSTANCES_OF_TMR_UNITS];

/*******************************************************************************
PRIVATE APIs Declarations
********************************************************************************/

/*PUBLIC API FUNCTIONS*/

/********************************************************************************/
/* NAME:	 TMR_SetBaseAddress()						*/
/*---------------------------------------------------------------------------   */
/* DESCRIPTION   : This routine initializes the TMR units. This function is     */
/*                 called twice, once for each timer unit.                      */
/* PARAMETERS    :							        */
/* IN            : t_logical_address                                            */
/* INOUT         : None                                                         */
/* OUT           : None                                                         */
/* RETURN VALUE  : void							        */
/* TYPE          : Public                                                       */
/*------------------------------------------------------------------------------*/
/* REENTRANCY: NA							        */
/*------------------------------------------------------------------------------*/
/********************************************************************************/
PUBLIC void 
TMR_SetBaseAddress(t_tmr_device_id tmr_device_id, t_logical_address tmr_base_address)
{
    if (tmr_base_address)
    {
        switch (tmr_device_id)
        {
            case TMR_DEVICE_ID_0:
                gp_tmr_register[0] = (t_tmr_register *) tmr_base_address;
                break;

            case TMR_DEVICE_ID_1:
                gp_tmr_register[1] = (t_tmr_register *) tmr_base_address;
                break;

            case TMR_DEVICE_ID_2:
                gp_tmr_register[2] = (t_tmr_register *) tmr_base_address;
                break;

            case TMR_DEVICE_ID_3:
                gp_tmr_register[3] = (t_tmr_register *) tmr_base_address;
                break;

            case TMR_DEVICE_ID_INVALID:
            default:
             //   return( TMR_INVALID_PARAMETER );
             printk("\nTMR_SetBaseAddress::TMR_INVALID_PARAMETER\n");//???
        }/* end of switch */
    }       
}           

/********************************************************************************/
/* NAME:	 TMR_GetIRQSrc()                                                */
/*---------------------------------------------------------------------------   */
/* DESCRIPTION   : This function returns the IRQ source numbers of the available*/
/*				  interrupts.                                   */
/* PARAMETERS    :			    	                                */
/* IN            : tmr_device_id -- (DTU1 or DTU2 )	   	                */
/* INOUT         : None                                                         */
/* OUT           : None                                                         */
/* RETURN VALUE  : t_tmr_irq_src						*/
/* TYPE          : Public                                                       */
/*                             3     2        1       0	                        */
/*                           ------  -----  ------ -----			*/
/*                          |      |      |      |      |			*/
/*                          |Timer1|REDGE0|FEDGE0|Timer0|			*/
/*                           ------  -----  ------ -----			*/
/*---------------------------------------------------------------------------   */
/* REENTRANCY: Re-entrant							*/
/********************************************************************************/
PUBLIC 
t_tmr_irq_src TMR_GetIRQSrc(t_tmr_device_id tmr_device_id)
{
    t_tmr_irq_src   tmr_irq_src = 0;

    switch (tmr_device_id)
    {
        case TMR_DEVICE_ID_0:
            tmr_irq_src |= DTU_INTERRUPT_STATUS_0  ;
            tmr_irq_src |= ((DTU_INTERRUPT_STATUS_1) << 3); 
            /*SPEAR shift by 3 bits as interrupts are of 3 bits */
            break;

        case TMR_DEVICE_ID_1:
            tmr_irq_src |= ((DTU_INTERRUPT_STATUS_2) << 6);
            tmr_irq_src |= ((DTU_INTERRUPT_STATUS_3) << 9);
            break;

        case TMR_DEVICE_ID_2:
            tmr_irq_src |= ((DTU_INTERRUPT_STATUS_4) << 12);
            tmr_irq_src |= ((DTU_INTERRUPT_STATUS_5) << 15);
            break;

        case TMR_DEVICE_ID_3:
            tmr_irq_src |= ((DTU_INTERRUPT_STATUS_6) << 18);
            tmr_irq_src |= ((DTU_INTERRUPT_STATUS_7) << 21);
            break;

        default: 
            break;
    }      /* end of switch */

    return(tmr_irq_src);
}  /* end of TMR_GetIRQSrc function */


/********************************************************************************/
/* NAME:	 TMR_EnableIRQSrc()                                             */
/*---------------------------------------------------------------------------   */
/* DESCRIPTION   : This routine enables single/ multiple interrupt sources      */
/* PARAMETERS    :							        */
/* IN            : tmr_irq_src -- irqsource identification                      */
/* INOUT         : None                                                         */
/* OUT           : None                                                         */
/* RETURN VALUE  : void 						        */
/* TYPE          : Public                                                       */
/*---------------------------------------------------------------------------   */
/* REENTRANCY: Re-entrant							*/

/********************************************************************************/
PUBLIC void 
TMR_EnableIRQSrc(t_tmr_irq_src tmr_irq_src)
{
    if (tmr_irq_src & (t_uint32)TMR_ID_0)
    {
        DTU_TIMER_INTERRUPT_ENABLE_0;
    }
    if (tmr_irq_src & (t_uint32)TMR_ID_1)
    {
        DTU_TIMER_INTERRUPT_ENABLE_1;
    }
    if (tmr_irq_src & (t_uint32)TMR_ID_2)
    {
        DTU_TIMER_INTERRUPT_ENABLE_2;
    }
    if (tmr_irq_src & (t_uint32)TMR_ID_3)
    {
        DTU_TIMER_INTERRUPT_ENABLE_3;
    }
    if (tmr_irq_src & (t_uint32)TMR_ID_4)
    {
        DTU_TIMER_INTERRUPT_ENABLE_4;
    }
    if (tmr_irq_src & (t_uint32)TMR_ID_5)
    {
        DTU_TIMER_INTERRUPT_ENABLE_5;
    }
    if (tmr_irq_src & (t_uint32)TMR_ID_6)
    {
        DTU_TIMER_INTERRUPT_ENABLE_6;
    }
    if (tmr_irq_src & (t_uint32)TMR_ID_7)
    {
        DTU_TIMER_INTERRUPT_ENABLE_7;
    }


    /*clearing Fedge ints*/
    if (tmr_irq_src & (t_uint32) TMR_ID_0 <<1)
    {
        DTU_TIMER_FEDGE_INTERRUPT_ENABLE_0;
    }
    if (tmr_irq_src & (t_uint32) TMR_ID_1 <<1)
    {
        DTU_TIMER_FEDGE_INTERRUPT_ENABLE_1;
    }
    if (tmr_irq_src & (t_uint32) TMR_ID_2 <<1)
    {
        DTU_TIMER_FEDGE_INTERRUPT_ENABLE_2;
    }
    if (tmr_irq_src & (t_uint32) TMR_ID_3 <<1)
    {
        DTU_TIMER_FEDGE_INTERRUPT_ENABLE_3;
    }
    if (tmr_irq_src & (t_uint32) TMR_ID_4 <<1)
    {
        DTU_TIMER_FEDGE_INTERRUPT_ENABLE_4;
    }
    if (tmr_irq_src & (t_uint32)TMR_ID_5 <<1 )
    {
        DTU_TIMER_FEDGE_INTERRUPT_ENABLE_5;
    }
    if (tmr_irq_src & (t_uint32)TMR_ID_6 <<1 )
    {
        DTU_TIMER_FEDGE_INTERRUPT_ENABLE_6;
    }
    if (tmr_irq_src & (t_uint32)TMR_ID_7 <<1 )
    {
        DTU_TIMER_FEDGE_INTERRUPT_ENABLE_7;
    }

    /*clearing Redge ints*/
    if (tmr_irq_src & (t_uint32)TMR_ID_0 <<2 )
    {
        DTU_TIMER_REDGE_INTERRUPT_ENABLE_0;
    }
    if (tmr_irq_src & (t_uint32)TMR_ID_1 <<2 )
    {
        DTU_TIMER_REDGE_INTERRUPT_ENABLE_1;
    }
    if (tmr_irq_src & (t_uint32)TMR_ID_2 <<2 )
    {
        DTU_TIMER_REDGE_INTERRUPT_ENABLE_2;
    }
    if (tmr_irq_src & (t_uint32)TMR_ID_3 <<2 )
    {
        DTU_TIMER_REDGE_INTERRUPT_ENABLE_3;
    }
    if (tmr_irq_src & (t_uint32)TMR_ID_4 <<2 )
    {
        DTU_TIMER_REDGE_INTERRUPT_ENABLE_4;
    }
    if (tmr_irq_src & (t_uint32)TMR_ID_5 <<2 )
    {
        DTU_TIMER_REDGE_INTERRUPT_ENABLE_5;
    }
    if (tmr_irq_src & (t_uint32)TMR_ID_6 <<2 )
    {
        DTU_TIMER_REDGE_INTERRUPT_ENABLE_6;
    }
    if (tmr_irq_src & (t_uint32)TMR_ID_7 <<2 )
    {
        DTU_TIMER_REDGE_INTERRUPT_ENABLE_7;
    }

} /* end of TMR_EnableIRQSrc function */


/********************************************************************************/
/* NAME:	 TMR_DisableIRQSrc()                                            */
/*---------------------------------------------------------------------------   */
/* DESCRIPTION   : This routine disables single/ multiple interrupt sources	*/
/* PARAMETERS    :								*/
/* IN            : tmr_irq_src -- irqsource identification                      */
/* INOUT         : None                                                         */
/* OUT           : None                                                         */
/* RETURN VALUE  : void 							*/
/* TYPE          : Public                                                       */
/*---------------------------------------------------------------------------   */
/* REENTRANCY: Re-entrant							*/
/********************************************************************************/
PUBLIC void 
TMR_DisableIRQSrc(t_tmr_irq_src tmr_irq_src)
{
    if (tmr_irq_src & (t_uint32)TMR_ID_0)
    {
        DTU_TIMER_INTERRUPT_DISABLE_0;
    }
    if (tmr_irq_src & (t_uint32)TMR_ID_1)
    {
        DTU_TIMER_INTERRUPT_DISABLE_1;
    }
    if (tmr_irq_src & (t_uint32)TMR_ID_2)
    {
        DTU_TIMER_INTERRUPT_DISABLE_2;
    }
    if (tmr_irq_src & (t_uint32)TMR_ID_3)
    {
        DTU_TIMER_INTERRUPT_DISABLE_3;
    }
    if (tmr_irq_src & (t_uint32)TMR_ID_4)
    {
        DTU_TIMER_INTERRUPT_DISABLE_4;
    }
    if (tmr_irq_src & (t_uint32)TMR_ID_5)
    {
        DTU_TIMER_INTERRUPT_DISABLE_5;
    }
    if (tmr_irq_src & (t_uint32)TMR_ID_6)
    {
        DTU_TIMER_INTERRUPT_DISABLE_6;
    }
    if (tmr_irq_src & (t_uint32)TMR_ID_7)
    {
        DTU_TIMER_INTERRUPT_DISABLE_7;
    }


    /*clearing Fedge ints*/
    if (tmr_irq_src & ((t_uint32) TMR_ID_0 <<1))
    {
        DTU_TIMER_FEDGE_INTERRUPT_DISABLE_0;
    }
    if (tmr_irq_src & ((t_uint32) TMR_ID_1 <<1))
    {
        DTU_TIMER_FEDGE_INTERRUPT_DISABLE_1;
    }
    if (tmr_irq_src & ((t_uint32) TMR_ID_2 <<1))
    {
        DTU_TIMER_FEDGE_INTERRUPT_DISABLE_2;
    }
    if (tmr_irq_src & ((t_uint32)TMR_ID_3 <<1 ))
    {
        DTU_TIMER_FEDGE_INTERRUPT_DISABLE_3;
    }
    if (tmr_irq_src & ((t_uint32) TMR_ID_4 <<1))
    {
        DTU_TIMER_FEDGE_INTERRUPT_DISABLE_4;
    }
    if (tmr_irq_src & ((t_uint32) TMR_ID_5 <<1))
    {
        DTU_TIMER_FEDGE_INTERRUPT_DISABLE_5;
    }
    if (tmr_irq_src & ((t_uint32) TMR_ID_6 <<1))
    {
        DTU_TIMER_FEDGE_INTERRUPT_DISABLE_6;
    }
    if (tmr_irq_src & ((t_uint32)TMR_ID_7 <<1 ))
    {
        DTU_TIMER_FEDGE_INTERRUPT_DISABLE_7;
    }


    /*clearing Redge ints*/
    if (tmr_irq_src & ((t_uint32)TMR_ID_0 <<2 ))
    {
        DTU_TIMER_REDGE_INTERRUPT_DISABLE_0;
    }
    if (tmr_irq_src & ((t_uint32)TMR_ID_1 <<2 ))
    {
        DTU_TIMER_REDGE_INTERRUPT_DISABLE_1;
    }
    if (tmr_irq_src & ((t_uint32)TMR_ID_2 <<2 ))
    {
        DTU_TIMER_REDGE_INTERRUPT_DISABLE_2;
    }
    if (tmr_irq_src & ((t_uint32)TMR_ID_3 <<2 ))
    {
        DTU_TIMER_REDGE_INTERRUPT_DISABLE_3;
    }
    if (tmr_irq_src & ((t_uint32)TMR_ID_4 <<2 ))
    {
        DTU_TIMER_REDGE_INTERRUPT_DISABLE_4;
    }
    if (tmr_irq_src & ((t_uint32)TMR_ID_5 <<2 ))
    {
        DTU_TIMER_REDGE_INTERRUPT_DISABLE_5;
    }
    if (tmr_irq_src & ((t_uint32)TMR_ID_6 <<2 ))
    {
        DTU_TIMER_REDGE_INTERRUPT_DISABLE_6;
    }
    if (tmr_irq_src & ((t_uint32)TMR_ID_7 <<2 ))
    {
        DTU_TIMER_REDGE_INTERRUPT_DISABLE_7;
    }


}  /*end of TMR_DisableIRQSrc function */

/********************************************************************************/
/* NAME:	 TMR_ClearIRQSrc()                                              */
/*---------------------------------------------------------------------------   */
/* DESCRIPTION   : Clears the interrupt source in the Interrupt Clear Register .*/
/* PARAMETERS    :		   			                        */
/* IN            : tmr_irq_src (TIMER_x)                                        */        
/* INOUT         : None                                                         */
/* OUT           : None                                                         */
/* RETURN VALUE  : void                                                         */
/* TYPE          : Public                                                       */
/*---------------------------------------------------------------------------   */
/* REENTRANCY: Re-entrant							*/
/********************************************************************************/
PUBLIC void 
TMR_ClearIRQSrc(t_tmr_irq_src tmr_irq_src)
{
    if (tmr_irq_src & (t_uint32)TMR_ID_0)
    {
        DTU_TIMER_INTERRUPT_CLEAR_0;
    }
    if (tmr_irq_src & (t_uint32)TMR_ID_1)
    {
        DTU_TIMER_INTERRUPT_CLEAR_1;
    }
    if (tmr_irq_src & (t_uint32)TMR_ID_2)
    {
        DTU_TIMER_INTERRUPT_CLEAR_2;
    }
    if (tmr_irq_src & (t_uint32)TMR_ID_3)
    {
        DTU_TIMER_INTERRUPT_CLEAR_3;
    }
    if (tmr_irq_src & (t_uint32)TMR_ID_4)
    {
        DTU_TIMER_INTERRUPT_CLEAR_4;
    }
    if (tmr_irq_src & (t_uint32)TMR_ID_5)
    {
        DTU_TIMER_INTERRUPT_CLEAR_5;
    }
    if (tmr_irq_src & (t_uint32)TMR_ID_6)
    {
        DTU_TIMER_INTERRUPT_CLEAR_6;
    }
    if (tmr_irq_src & (t_uint32)TMR_ID_7)
    {
        DTU_TIMER_INTERRUPT_CLEAR_7;
    }


    /*clearing Fedge ints*/
    if (tmr_irq_src & (t_uint32) TMR_ID_0 <<1)
    {
        DTU_TIMER_FEDGE_INTERRUPT_CLEAR_0;
    }
    if (tmr_irq_src & (t_uint32) TMR_ID_1 <<1)
    {
        DTU_TIMER_FEDGE_INTERRUPT_CLEAR_1;
    }
    if (tmr_irq_src & (t_uint32) TMR_ID_2 <<1)
    {
        DTU_TIMER_FEDGE_INTERRUPT_CLEAR_2;
    }
    if (tmr_irq_src & (t_uint32)TMR_ID_3 <<1 )
    {
        DTU_TIMER_FEDGE_INTERRUPT_CLEAR_3;
    }
    if (tmr_irq_src & (t_uint32) TMR_ID_4 <<1)
    {
        DTU_TIMER_FEDGE_INTERRUPT_CLEAR_4;
    }
    if (tmr_irq_src & (t_uint32) TMR_ID_5 <<1)
    {
        DTU_TIMER_FEDGE_INTERRUPT_CLEAR_5;
    }
    if (tmr_irq_src & (t_uint32) TMR_ID_6 <<1)
    {
        DTU_TIMER_FEDGE_INTERRUPT_CLEAR_6;
    }
    if (tmr_irq_src & (t_uint32)TMR_ID_7 <<1 )
    {
        DTU_TIMER_FEDGE_INTERRUPT_CLEAR_7;
    }


    /*clearing Redge ints*/
    if (tmr_irq_src & (t_uint32)TMR_ID_0 <<2 )
    {
        DTU_TIMER_REDGE_INTERRUPT_CLEAR_0;
    }
    if (tmr_irq_src & (t_uint32)TMR_ID_1 <<2 )
    {
        DTU_TIMER_REDGE_INTERRUPT_CLEAR_1;
    }
    if (tmr_irq_src & (t_uint32)TMR_ID_2 <<2 )
    {
        DTU_TIMER_REDGE_INTERRUPT_CLEAR_2;
    }
    if (tmr_irq_src & (t_uint32)TMR_ID_3 <<2 )
    {
        DTU_TIMER_REDGE_INTERRUPT_CLEAR_3;
    }
    if (tmr_irq_src & (t_uint32)TMR_ID_4 <<2 )
    {
        DTU_TIMER_REDGE_INTERRUPT_CLEAR_4;
    }
    if (tmr_irq_src & (t_uint32)TMR_ID_5 <<2 )
    {
        DTU_TIMER_REDGE_INTERRUPT_CLEAR_5;
    }
    if (tmr_irq_src & (t_uint32)TMR_ID_6 <<2 )
    {
        DTU_TIMER_REDGE_INTERRUPT_CLEAR_6;
    }
    if (tmr_irq_src & (t_uint32)TMR_ID_7 <<2 )
    {
        DTU_TIMER_REDGE_INTERRUPT_CLEAR_7;
    }

} /*end of TMR_ClearIRQSrc*/

/********************************************************************************/
/* NAME:	 TMR_GetDeviceID()		 			        */
/*---------------------------------------------------------------------------   */
/* DESCRIPTION   : This function provides the Device ID number associated       */
/*             	  with the IRQ Source ID number.                                */
/*SPEARAssumption: that at a time irq src contains the interrupts of one timer  */
/* PARAMETERS    				    	                        */ 
/* IN            : t_tmr_irq_src id                                             */
/* INOUT         : None                                                         */
/* OUT           : None                                                         */
/* RETURN VALUE  : t_tmr_device_id 						*/
/* TYPE          : Public                                                       */
/*---------------------------------------------------------------------------   */
/* REENTRANCY: Re-entrant							*/
/********************************************************************************/
PUBLIC 
t_tmr_device_id TMR_GetDeviceID(t_tmr_irq_src tmr_irq_src)
{
    t_tmr_device_id tmr_device_id;

    unsigned int NoOfIntr = 0;

    if (TMR_MASK_INT_UNIT0 & tmr_irq_src )
    {
        tmr_device_id = TMR_DEVICE_ID_0;
        NoOfIntr++; 
    }

	if (TMR_MASK_INT_UNIT1 & tmr_irq_src )
    {
        tmr_device_id = TMR_DEVICE_ID_1;
        NoOfIntr++; 
    }

    if (TMR_MASK_INT_UNIT2 & tmr_irq_src )
    {
        tmr_device_id = TMR_DEVICE_ID_2;
        NoOfIntr++; 
    }

    if (TMR_MASK_INT_UNIT3 & tmr_irq_src )
    {
        tmr_device_id = TMR_DEVICE_ID_3;
        NoOfIntr++; 
    }

    if(NoOfIntr != 1)
    {
        tmr_device_id = TMR_DEVICE_ID_INVALID;
    }

    return(tmr_device_id);
}/* end of TMR_GetDeviceID function */

/********************************************************************************/
/* NAME:	 TMR_IsPendingIRQSrc()                                          */
/*---------------------------------------------------------------------------   */
/* DESCRIPTION   : This function allows to know if the IRQ Source is            */
/*			  active or not.t_tmr_irq_src is a single interrupt.    */
/* PARAMETERS    :							        */
/* IN            : tmr_irq_src                                                  */
/* INOUT         : None                                                         */
/* OUT           : None                                                         */
/* RETURN VALUE  : t_bool						        */
/* TYPE          : Public                                                       */
/*---------------------------------------------------------------------------   */
/* REENTRANCY: Re-entrant							*/
/********************************************************************************/
PUBLIC t_bool TMR_IsPendingIRQSrc(t_tmr_irq_src tmr_irq_src)
{
    if ((( TMR_GetIRQSrc(TMR_DEVICE_ID_0) | TMR_GetIRQSrc(TMR_DEVICE_ID_1) | TMR_GetIRQSrc(TMR_DEVICE_ID_2) | TMR_GetIRQSrc(TMR_DEVICE_ID_3)) & tmr_irq_src))
    {
        return(TRUE);
    }
    else
    {
        return(FALSE);
    }
} /* end of TMR_IsPendingIRQSrc function */

/* End of file - tmr_irq.c */

