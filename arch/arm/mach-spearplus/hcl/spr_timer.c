/*
 *
 *  Copyright (C) 2006 ST MICROELECTRONICS
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
 *-----------------------------------------------------------------------------
 * This module provides some support routines for the TIMER(DTU/MTU)
 *-----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------*
 * Includes                                                                     *
 *------------------------------------------------------------------------------*/
#include <asm/arch/spr_timer.h>
#include <asm/arch/spr_timer_p.h>


/*-------------------------------------------------------------------------------
 * Defines
 *------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------
 * Variables
 *------------------------------------------------------------------------------*/

#ifdef __DEBUG
#define MY_DEBUG_LEVEL_VAR_NAME myDebugLevel_TMR
#define MY_DEBUG_ID             myDebugID_TMR
PRIVATE t_dbg_level             MY_DEBUG_LEVEL_VAR_NAME = DEBUG_LEVEL0;
PRIVATE t_dbg_id                MY_DEBUG_ID = TIMER_HCL_DBG_ID;
#endif /* end of #ifdef __DEBUG*/


/*Global definitions for the TIMER driver*/
PRIVATE t_tmr_system_context    g_tmr_system_context;



/********************************************************************************/
/* NAME:	  t_tmr_error TMR_Init ( t_tmr_device_id tmr_device_id ,        */
/*                                       t_logical_address tmr_base_address )   */
/*                                                   	                        */
/*------------------------------------------------------------------------------*/
/* DESCRIPTION   : This routine initializes the TMR units. This function is  	*/
/*                 called twice, once for each timer unit.                   	*/
/* PARAMETERS    :				                   	        */
/* IN            : tmr_device_id                                                */       
/*                 tmr_base_address                                        	*/
/* INOUT         : None                                                      	*/
/* OUT           : None                                                      	*/
/* RETURN VALUE  : t_tmr_error						  	*/
/* TYPE          : Public                                                    	*/
/*------------------------------------------------------------------------------*/
/* REENTRANCY: NA							    	*/
/********************************************************************************/

PUBLIC t_tmr_error TMR_Init ( t_tmr_device_id tmr_device_id ,
        t_logical_address tmr_base_address )
{
    t_uint32 tmr_device_id_index = 0;

    switch ( tmr_device_id )
    {
        case TMR_DEVICE_ID_0:
            tmr_device_id_index = 0;
            break;

        case TMR_DEVICE_ID_1:
            tmr_device_id_index = 1;
            break;
            
        case TMR_DEVICE_ID_2:
            tmr_device_id_index = 2;
            break;
            
        case TMR_DEVICE_ID_3:
            tmr_device_id_index = 3;
            break;

        case TMR_DEVICE_ID_INVALID:
        default:
            return ( TMR_INVALID_PARAMETER );
    }

    if ( NULL != tmr_base_address )                              
    {

        /* Initializing the registers structure */
        g_tmr_system_context.p_tmr_register[tmr_device_id_index] = ( t_tmr_register * ) tmr_base_address;    /* TBC TBD */

    }

    return ( TMR_INVALID_PARAMETER );
}

/********************************************************************************/
/* NAME:  t_tmr_error TMR_Reset(t_tmr_device_id tmr_device_id)               	*/
/*---------------------------------------------------------------------------	*/
/* DESCRIPTION: This routine initializes the TMR units. This function is     	*/
/*              called twice, one for each timer unit.It should be called    	*/
/*              after SetBaseAddress.                                        	*/
/*               		                                                */
/* PARAMETERS:                                                               	*/
/* IN  :         tmr_device_id   	                                        */
/*                                                                           	*/
/* OUT :                                                                     	*/
/*                                                                           	*/
/* RETURN: t_tmr_error  TMR_UNSUPPORTED_HW in case the                         	*/
/*                      hw unit is not congruent with the ID code   		*/
/*                           	                                                */
/*---------------------------------------------------------------------------	*/
/* REENTRANCY: NA                                                            	*/
/********************************************************************************/

PUBLIC t_tmr_error TMR_Reset ( t_tmr_device_id tmr_device_id )
{
    t_tmr_error tmr_error = TMR_OK;
    t_uint32    mask;

    if ( TMR_DEVICE_ID_0 == tmr_device_id )
    {
        TMR_ClearIRQSrc ( TMR_TIMER_IDS_IN_TMR_UNIT0 );
    }
    else if ( TMR_DEVICE_ID_1 == tmr_device_id )
    {
        TMR_ClearIRQSrc ( TMR_TIMER_IDS_IN_TMR_UNIT1 );
    } 
    else if ( TMR_DEVICE_ID_2 == tmr_device_id )
    {
        TMR_ClearIRQSrc ( TMR_TIMER_IDS_IN_TMR_UNIT2 );
    } 
    else if ( TMR_DEVICE_ID_3 == tmr_device_id )
    {
        TMR_ClearIRQSrc ( TMR_TIMER_IDS_IN_TMR_UNIT3 );
    } 
    else
    {
        tmr_error = TMR_INVALID_PARAMETER;
        return tmr_error;
    }

    g_tmr_system_context.number_of_total_timers = TMR_TOTAL_NUMBER_OF_TIMERS;

    /*initialize the masks*/
    g_tmr_system_context.tmr_mask[0] = TMR_ID_0;
    g_tmr_system_context.tmr_mask[1] = TMR_ID_1;
    g_tmr_system_context.tmr_mask[2] = TMR_ID_2;
    g_tmr_system_context.tmr_mask[3] = TMR_ID_3;
    g_tmr_system_context.tmr_mask[4] = TMR_ID_4;
    g_tmr_system_context.tmr_mask[5] = TMR_ID_5;
    g_tmr_system_context.tmr_mask[6] = TMR_ID_6;
    g_tmr_system_context.tmr_mask[7] = TMR_ID_7;

    switch ( tmr_device_id )
    {
        case TMR_DEVICE_ID_0:
            tmr_EnterCriticalSection ( &mask );
            g_tmr_system_context.tmr_state[0] = TMR_STATE_FREE;
            g_tmr_system_context.tmr_state[1] = TMR_STATE_FREE;
       
            g_tmr_system_context.tmr_temp_ptr[0] = ( t_tmr_register_subset * ) 
                    & ( g_tmr_system_context.p_tmr_register[0]->tmr_control1 );
            g_tmr_system_context.tmr_temp_ptr[1] = ( t_tmr_register_subset * ) 
                    & ( g_tmr_system_context.p_tmr_register[0]->tmr_control2 );
            tmr_ExitCriticalSection ( mask );

            TMR_EnableIRQSrc ( TMR_TIMER_IDS_IN_TMR_UNIT0 );
            break;

        case TMR_DEVICE_ID_1:
            tmr_EnterCriticalSection ( &mask );
            g_tmr_system_context.tmr_state[2] = TMR_STATE_FREE;
            g_tmr_system_context.tmr_state[3] = TMR_STATE_FREE;
           
            g_tmr_system_context.tmr_temp_ptr[2] = ( t_tmr_register_subset * ) 
                    & ( g_tmr_system_context.p_tmr_register[1]->tmr_control1 );
            g_tmr_system_context.tmr_temp_ptr[3] = ( t_tmr_register_subset * ) 
                    & ( g_tmr_system_context.p_tmr_register[1]->tmr_control2 );
            tmr_ExitCriticalSection ( mask );

            TMR_EnableIRQSrc ( TMR_TIMER_IDS_IN_TMR_UNIT1 );
            break;

        case TMR_DEVICE_ID_2:
            tmr_EnterCriticalSection ( &mask );
            g_tmr_system_context.tmr_state[4] = TMR_STATE_FREE;
            g_tmr_system_context.tmr_state[5] = TMR_STATE_FREE;
            
            g_tmr_system_context.tmr_temp_ptr[4] = ( t_tmr_register_subset * ) 
                    & ( g_tmr_system_context.p_tmr_register[2]->tmr_control1 );
            g_tmr_system_context.tmr_temp_ptr[5] = ( t_tmr_register_subset * ) 
                    & ( g_tmr_system_context.p_tmr_register[2]->tmr_control2 );
            tmr_ExitCriticalSection ( mask );

            TMR_EnableIRQSrc ( TMR_TIMER_IDS_IN_TMR_UNIT2 );
            break;

        case TMR_DEVICE_ID_3:
            tmr_EnterCriticalSection ( &mask );
            g_tmr_system_context.tmr_state[6] = TMR_STATE_FREE;
            g_tmr_system_context.tmr_state[7] = TMR_STATE_FREE;
            
            g_tmr_system_context.tmr_temp_ptr[6] = ( t_tmr_register_subset * ) 
                    & ( g_tmr_system_context.p_tmr_register[3]->tmr_control1 );
            g_tmr_system_context.tmr_temp_ptr[7] = ( t_tmr_register_subset * ) 
                    & ( g_tmr_system_context.p_tmr_register[3]->tmr_control2 );
            tmr_ExitCriticalSection ( mask );

            TMR_EnableIRQSrc ( TMR_TIMER_IDS_IN_TMR_UNIT3 );
            break;

        case TMR_DEVICE_ID_INVALID:
        default:
            tmr_error = TMR_INVALID_PARAMETER;
            return ( tmr_error );
    }

    return ( tmr_error );
}

/********************************************************************************/
/* NAME:  t_tmr_error TMR_Configure(t_tmr_id  tmr_id,     		        */
/*               		    t_tmr_mode tmr_mode,                        */
/*                                  t_tmr_capt_mode  tmr_capt_mode,             */
/*               		    t_uint16 load_value,                        */
/*               		    t_tmr_clk_prescaler tmr_clk_prescaler)     	*/
/*               		                                               	*/
/*---------------------------------------------------------------------------	*/
/* DESCRIPTION: This routine initializes the timer parameters		       	*/
/*               		                                               	*/
/* PARAMETERS:                                                                  */
/*                                                                 	        */
/* IN :      tmr_id :          : Gives the timer ID to initialize   	        */
/* IN :      tmr_mode:         : Gives the timer mode(Auto-reload,one shot      */    
/*                               or capture                                     */
/* IN :      load_value        : To which counting to be done	     	        */
/* IN :      tmr_clk_prescaler : The variable gives the timer ID to initialize  */
/* IN :      tmr_capt_mode     : The capture mode of the timer               	*/       
/* OUT :                       		                             	       	*/
/*                                                                           	*/
/* RETURN: t_tmr_error : TMR error code. This function will return  	     	*/
/*                       TMR_OK 				   	       	*/
/* 		         TMR_ERROR_INVALID_TIMER_ID invalid tmr_id	       	*/
/* 		         TMR_ERROR_TIMER_ALREADY_STARTED timer already runing  	*/
/* 		         TMR_INVALID_PARAMETER One parameter is invalid       	*/
/*                       tmr_clk_prescaler has not been set to 1,2,4,8,16,      */
/*                                                            32,64,128or25    	*/
/*                                		                              	*/
/* COMMENTS: The Timer must have been allocated with TMR_AllocTimer and not  	*/
/*           started             					       	*/
/*---------------------------------------------------------------------------	*/
/* REENTRANCY:The function is non-reentrant as it modifies global 		*/
/*	      g_tmr_system_context.tmr_state                	                */
/*            Hence it has been put into critical section                    	*/
/********************************************************************************/

PUBLIC t_tmr_error TMR_Configure
(
 t_tmr_id            tmr_id,
 t_tmr_mode          tmr_mode,
 t_tmr_capt_mode     tmr_capt_mode,                      
 t_uint16            load_value,
 t_tmr_clk_prescaler tmr_clk_prescaler
 )
{
    t_tmr_error             tmr_error = TMR_OK;
    t_tmr_register_subset   *p_tmr_register_subset;        
    t_uint32                index;
    t_uint32                mask = 0;

    if (0 == load_value)
    {
        tmr_error = TMR_INVALID_PARAMETER;
        return(tmr_error);
    }

    /*Check that the timer ID is a valid Timer ID */
    for (index = 0; index < g_tmr_system_context.number_of_total_timers; index++)
    {
        if (g_tmr_system_context.tmr_mask[index] == tmr_id)
        {
            /*check if the timer is in use or not */
            tmr_EnterCriticalSection(&mask);
            switch (g_tmr_system_context.tmr_state[index])
            {
                /*check that the timer has been initialized*/
                case TMR_STATE_BUSY:        /*initialize the timer*/
                    break;

                case TMR_STATE_INITIALIZED: /*re initialize the timer*/
                    g_tmr_system_context.tmr_state[index] = TMR_STATE_BUSY;
                    break;

                case TMR_STATE_FREE:
                    tmr_ExitCriticalSection(mask);
                    return(TMR_ERROR_INVALID_TIMER_ID);

                case TMR_STATE_RUNNING:
                    tmr_ExitCriticalSection(mask);
                    return(TMR_ERROR_TIMER_ALREADY_STARTED);

                default:
                    return(TMR_INVALID_PARAMETER);
            }

            tmr_ExitCriticalSection(mask);
            break;
        }
    }

    if (index == g_tmr_system_context.number_of_total_timers)
    {
        return(TMR_ERROR_INVALID_TIMER_ID);
    }

    /* End Check that the timer ID is a valid Timer ID */
    /* Get the ptr to the structure in use             */
    /*address of a specific timer area                 */
    p_tmr_register_subset = g_tmr_system_context.tmr_temp_ptr[index]; 

    /* End Get the ptr to the structure in use        */
    /* Set the timer mode to the passed value         */

    switch (tmr_mode)
    {
        case TMR_MODE_AUTO_RELOAD:
            tmr_EnterCriticalSection(&mask);
            g_tmr_system_context.tmr_mode[index] = TMR_MODE_AUTO_RELOAD;
            tmr_ExitCriticalSection(mask);
            TMR_TIMER_ONE_SHOT_SET(TIMER_AUTO_RELOAD);
            break;

        case TMR_MODE_ONE_SHOT:
            tmr_EnterCriticalSection(&mask);
            g_tmr_system_context.tmr_mode[index] = TMR_MODE_ONE_SHOT;
            tmr_ExitCriticalSection(mask);
            TMR_TIMER_ONE_SHOT_SET(TIMER_ONE_SHOT);
            break;

        default:
            return(TMR_INVALID_PARAMETER);
    }

    switch(tmr_capt_mode)
    {
        case TMR_MODE_NO_CAPTURE:
            tmr_EnterCriticalSection(&mask);
            g_tmr_system_context.tmr_mode[index] = TMR_MODE_NO_CAPTURE;
            tmr_ExitCriticalSection(mask);
            TMR_CAPTURE_SET(TMR_MODE_NO_CAPTURE);
            break;

        case TMR_MODE_CAPT_REDGE:
            tmr_EnterCriticalSection(&mask);
            g_tmr_system_context.tmr_mode[index] = TMR_MODE_CAPT_REDGE ;
            tmr_ExitCriticalSection(mask);
            TMR_CAPTURE_SET(TMR_MODE_CAPT_REDGE);
            break;

        case TMR_MODE_CAPT_FEDGE:
            tmr_EnterCriticalSection(&mask);
            g_tmr_system_context.tmr_mode[index] = TMR_MODE_CAPT_FEDGE;
            tmr_ExitCriticalSection(mask);
            TMR_CAPTURE_SET(TMR_MODE_CAPT_FEDGE);
            break;

        case TMR_MODE_CAPT_RFEDGE:
            tmr_EnterCriticalSection(&mask);
            g_tmr_system_context.tmr_mode[index] = TMR_MODE_CAPT_RFEDGE;
            tmr_ExitCriticalSection(mask);
            TMR_CAPTURE_SET(TMR_MODE_CAPT_RFEDGE);
            break;

        default:
            return(TMR_INVALID_PARAMETER);
    }


    /* END Set the timer mode to the passed value      */
    /* Set the passed loadvalue in the down counter    */
    TMR_SET_COMPARE_REG(load_value);

    /* END set the passed loadvalue in the down counter*/
    /* Set the clk division                            */
    switch (tmr_clk_prescaler)
    {
        case TMR_CLK_PRESCALER_1:
            TMR_PRESCALER_SET(0x0);
            break;

        case TMR_CLK_PRESCALER_2:
            TMR_PRESCALER_SET(0x1);
            break;

        case TMR_CLK_PRESCALER_4:
            TMR_PRESCALER_SET(0x2);
            break;

        case TMR_CLK_PRESCALER_8:
            TMR_PRESCALER_SET(0x3);
            break;

        case TMR_CLK_PRESCALER_16:
            TMR_PRESCALER_SET(0x4);
            break;

        case TMR_CLK_PRESCALER_32:
            TMR_PRESCALER_SET(0x5);
            break;

        case TMR_CLK_PRESCALER_64:
            TMR_PRESCALER_SET(0x6);
            break;

        case TMR_CLK_PRESCALER_128:
            TMR_PRESCALER_SET(0x7);
            break;

        case TMR_CLK_PRESCALER_256:
            TMR_PRESCALER_SET(0x8);
            break;
        default:
            return(TMR_INVALID_PARAMETER);
    }

    /*END set the clk division*/
    tmr_EnterCriticalSection(&mask);
    g_tmr_system_context.tmr_state[index] = TMR_STATE_INITIALIZED;
    tmr_ExitCriticalSection(mask);
    return(TMR_OK);
}

/********************************************************************************/
/* NAME:  t_tmr_error TMR_AllocTimer(t_tmr_id  *p_tmr_id)     	         	*/
/*------------------------------------------------------------------------------*/
/* DESCRIPTION: This routine allocates a timer and returns its number	     	*/
/*               		                                               	*/
/* PARAMETERS:                                                               	*/
/* IN :      	                  	                  	               	*/
/*                                                                           	*/
/* OUT :     *p_tmr_id     :      the variable points to a new timer ID if   	*/
/*                                available. This Timer ID is the physical   	*/
/*                                timer number                               	*/
/*                                                                           	*/
/* RETURN:   t_tmr_error  :       This function will return TMR_OK or         	*/
/*                                TMR_ERROR_NO_MORE_TIMER_AVAILABLE	        */
/*                                if no more timer can be allocated          	*/
/*                                                                           	*/
/* COMMENTS: Before to be used a timer ID must be allocated using this function	*/
/*           TimerIDs are always integers with only one bit set, to allow    	*/
/*           ProcessIT to return a timer ID list using a 32 bit mask         	*/
/*           The returned value must be used to check the interrupt mask when	*/
/*           an interrupt occurs                                             	*/
/*------------------------------------------------------------------------------*/
/* REENTRANCY: The function is non-reentrant as it modifies global 		*/
/*	       g_tmr_system_context.tmr_state 	                                */
/*             Hence it has been put into critical section                   	*/
/********************************************************************************/

PUBLIC t_tmr_error TMR_AllocTimer(t_tmr_id *p_tmr_id)
{
    t_uint32    index;
    t_tmr_error tmr_error = TMR_OK;
    t_uint32    mask;

    for (index = 0 ;index < g_tmr_system_context.number_of_total_timers ;index++)
    {
        tmr_EnterCriticalSection(&mask);
        if (TMR_STATE_FREE == g_tmr_system_context.tmr_state[index])
        {
            g_tmr_system_context.tmr_state[index] = TMR_STATE_BUSY;
            *p_tmr_id = g_tmr_system_context.tmr_mask[index];
            tmr_ExitCriticalSection(mask);
            return(tmr_error);
        }
        tmr_ExitCriticalSection(mask);
    }
    if (index == g_tmr_system_context.number_of_total_timers)
    {
        tmr_error = TMR_ERROR_NO_MORE_TIMER_AVAILABLE;
    }

    return(tmr_error);
}

/********************************************************************************/
/* NAME:  t_tmr_error TMR_StartTimer(t_tmr_id tmr_id)     	       	 	*/
/*---------------------------------------------------------------------------	*/
/* DESCRIPTION: This routine starts a virtual  timer incrementing          	*/
/*               		                                               	*/
/* PARAMETERS:                                                               	*/
/*                                                                           	*/
/* IN :    tmr_id      : The variable gives the timer ID to start     	        */
/*                                                                           	*/
/* OUT :      	                  	                  	               	*/
/*                                                                           	*/
/* RETURN: t_tmr_error : TMR error code. This function will return:             */
/*                       --TMR_OK  			                     	*/
/*                       --TMR_ERROR_INVALID_TIMER_ID if it is invalid timer    */
/*                    	 --TMR_ERROR_TIMER_ALREADY_STARTED timer already running*/
/* 		              	                                                */
/*                                                                              */
/* COMMENTS: A Timer ID must be queried and initialized before starting it.  	*/
/*---------------------------------------------------------------------------	*/
/* REENTRANCY:The function is non-reentrant as it modifies global 		*/
/*	      g_tmr_system_context.tmr_state  					*/
/*            Hence it has been put into critical section                    	*/
/********************************************************************************/

PUBLIC t_tmr_error TMR_StartTimer(t_tmr_id tmr_id)
{
    t_tmr_register_subset   *p_tmr_register_subset;          
    t_uint32                index;
    t_uint32                mask = 0;

    for (index = 0 ;index < g_tmr_system_context.number_of_total_timers ;index++)
    {
        if ( g_tmr_system_context.tmr_mask[index] == tmr_id )
        {
            tmr_EnterCriticalSection ( &mask );

            /*check the state of the timer */
            switch ( g_tmr_system_context.tmr_state[index] )
            {
                case TMR_STATE_FREE:

                case TMR_STATE_BUSY:
                    tmr_ExitCriticalSection ( mask );
                    return (TMR_ERROR_INVALID_STATE );

                case TMR_STATE_RUNNING:
                    tmr_ExitCriticalSection ( mask );
                    return (TMR_ERROR_TIMER_ALREADY_STARTED );

                    /* check that the timer has been initialized         */
                case TMR_STATE_INITIALIZED:

                    /* start the initialized timer                   */
                    /* get the ptr to the structure in use           */
                    p_tmr_register_subset = g_tmr_system_context.tmr_temp_ptr[index];  
                    g_tmr_system_context.tmr_state[index] = TMR_STATE_RUNNING;

                    /* start the previously initialized timer,       */
                    /*  by setting the enable bit in the             */
                    /* CTRL register   		                     */
                    TMR_TIMER_ENABLE;
                    break;

                default:
                    return ( TMR_INVALID_PARAMETER );
            }
            tmr_ExitCriticalSection ( mask );
            break;
        }
    }

    if ( index == g_tmr_system_context.number_of_total_timers )
    {
        return (TMR_ERROR_INVALID_TIMER_ID );
    }

    return (TMR_OK );
}



/********************************************************************************/
/* NAME:  t_tmr_error TMR_StopTimer(t_tmr_id  tmr_id)     		        */
/*---------------------------------------------------------------------------	*/
/* DESCRIPTION: This routine stops a timer			 	        */
/*               		                                                */
/* PARAMETERS:                                                               	*/
/*                                                                           	*/
/* IN :     tmr_id     :  The variable gives the timer ID to stop      	        */
/*                                                                           	*/
/* OUT :      	                  	                  	                */
/*                                                                           	*/
/* RETURN: t_tmr_error :  TMR error code. This function will return             */
/*                        TMR_OK  		             	                */
/*                        TMR_ERROR_INVALID_TIMER_ID  if the given timer is    	*/
/*                        invalid or unasable			                */
/* 		          TMR_ERROR_TIMER_ALREADY_STOPPED timer already stopped */
/*                                                                           	*/
/* COMMENTS: A Timer ID must be queried and started before to this call.     	*/
/*           It is possible to free a running timer without stopping it      	*/
/*---------------------------------------------------------------------------	*/
/* REENTRANCY:The function is non-reentrant as it modifies global 	 	*/
/* 				g_tmr_system_context.tmr_state  	 	*/
/*            Hence it has been put into critical section                    	*/
/********************************************************************************/

PUBLIC t_tmr_error TMR_StopTimer ( t_tmr_id tmr_id )
{
    t_tmr_register_subset   *p_tmr_register_subset;
    t_uint32                index;
    t_uint32                mask = 0;

    for ( index = 0; index < g_tmr_system_context.number_of_total_timers; index++ )
    {
        if ( g_tmr_system_context.tmr_mask[index] == tmr_id )
        {
            /*check if the timer is in use or not */
            tmr_EnterCriticalSection ( &mask );
            switch ( g_tmr_system_context.tmr_state[index] )
            {
                case TMR_STATE_RUNNING: /*stop the running timer*/
                    /*Get the ptr to the structure in use             */
                    p_tmr_register_subset = g_tmr_system_context.tmr_temp_ptr[index];
                    TMR_TIMER_DISABLE;
                    g_tmr_system_context.tmr_state[index] = TMR_STATE_BUSY;
                    break;

                case TMR_STATE_BUSY:
                    tmr_ExitCriticalSection ( mask );
                    return ( TMR_ERROR_TIMER_ALREADY_STOPPED );

                case TMR_STATE_INITIALIZED:
                case TMR_STATE_FREE:
                    tmr_ExitCriticalSection ( mask );
                    return ( TMR_ERROR_INVALID_TIMER_ID );

                default:
                    return ( TMR_INVALID_PARAMETER );
            }

            tmr_ExitCriticalSection ( mask );
            break;
        }
    }

    if ( index == g_tmr_system_context.number_of_total_timers )
    {
        return ( TMR_ERROR_INVALID_TIMER_ID );
    }

    return ( TMR_OK );
}

/********************************************************************************/
/* NAME:  t_tmr_error TMR_ReStartTimer()     		                        */
/*---------------------------------------------------------------------------	*/
/* DESCRIPTION: This is to restart the ONE SHOT timer(initialize as well as  	*/
/*              start it.                                                    	*/
/*               		                                                */
/* PARAMETERS:                                                               	*/
/*                                                                           	*/
/* IN : tmr_id    The variable gives the timer ID to start     	                */
/*      load_value  in Hexadecimal                             	                */
/*      tmr_clk_prescaler : tmr_clk_prescaler                           	*/
/*                                                                           	*/
/* OUT :      	                  	                  	                */
/*                                                                            	*/
/* RETURN: t_tmr_error  :  This function will return            	        */
/*                         TMR_OK  			             	        */
/*                         TMR_ERROR_INVALID_TIMER_ID   if the given timer is   */
/*                         invalid			                       	*/
/* 		           TMR_ERROR_TIMER_ALREADY_STARTED timer already runnin	*/
/*                                                                           	*/
/* COMMENTS: This API is only for ONE SHOT TIMER ,it loads count and starts  	*/
/*           the ONE SHOT Timer.                                             	*/
/*---------------------------------------------------------------------------	*/
/* REENTRANCY:The function is non-reentrant as it modifies global 		*/
/*            g_tmr_system_context.tmr_state  		 	                */
/*            Hence it has been put into critical section                    	*/
/********************************************************************************/

PUBLIC t_tmr_error TMR_ReStartTimer ( t_tmr_id tmr_id,
        t_uint16 load_value, t_tmr_clk_prescaler tmr_clk_prescaler )
{
    t_tmr_error             tmr_error;
    t_tmr_register_subset   *p_tmr_register_subset; 
    t_uint32                index;
    t_uint32                mask = 0;

    if ( 0 == load_value )
    {
        tmr_error = TMR_INVALID_PARAMETER;
        return ( tmr_error );
    }

    /*Check that the timer ID is a valid Timer ID */
    for ( index = 0; index < g_tmr_system_context.number_of_total_timers; index++)
    {
        if ( g_tmr_system_context.tmr_mask[index] == tmr_id )
        {
            /*check if the timer is in use or not */
            if ( g_tmr_system_context.tmr_mode[index] != TMR_MODE_ONE_SHOT )
            {
                return ( TMR_ERROR_NOT_ONE_SHOT );
            }

            tmr_EnterCriticalSection ( &mask );
            switch ( g_tmr_system_context.tmr_state[index] )
            {
                case TMR_STATE_FREE:
                    tmr_ExitCriticalSection ( mask );
                    return ( TMR_ERROR_INVALID_STATE );

                case TMR_STATE_RUNNING:
                    tmr_ExitCriticalSection ( mask );
                    return ( TMR_ERROR_TIMER_ALREADY_STARTED );

                case TMR_STATE_INITIALIZED:
                case TMR_STATE_BUSY:
                    /* start the initialized timer */
                    /* Get the ptr to the structure in use             */
                    /*address of a specific timer area*/
                    p_tmr_register_subset = g_tmr_system_context.tmr_temp_ptr[index]; 
                    g_tmr_system_context.tmr_state[index] = TMR_STATE_INITIALIZED;

                    /* start the previously initialized timer, by setting the enable bit in the */
                    /* CTRL register */
                    break;

                default:
                    return ( TMR_INVALID_PARAMETER );
            }
            tmr_ExitCriticalSection ( mask );
            break;
        }
    }

    if ( index == g_tmr_system_context.number_of_total_timers )
    {
        return ( TMR_ERROR_INVALID_TIMER_ID );
    }

    /*End Check that the timer ID is a valid Timer ID */
    /*Get the ptr to the structure in use             */
    p_tmr_register_subset = g_tmr_system_context.tmr_temp_ptr[index];

    /*End Get the ptr to the structure in use         */
    /*set the passed loadvalue in the down counter*/
    TMR_SET_COMPARE_REG(load_value);

    /*END set the passed loadvalue in the down counter*/
    /*set the clk division*/
    switch ( tmr_clk_prescaler )
    {
        case TMR_CLK_PRESCALER_1:
            TMR_PRESCALER_SET ( 0x0 );
            break;

        case TMR_CLK_PRESCALER_2:
            TMR_PRESCALER_SET ( 0x1 );
            break;

        case TMR_CLK_PRESCALER_4:
            TMR_PRESCALER_SET ( 0x2 );
            break;

        case TMR_CLK_PRESCALER_8:
            TMR_PRESCALER_SET ( 0x3 );
            break; 

        case TMR_CLK_PRESCALER_16:
            TMR_PRESCALER_SET ( 0x4 );
            break;

        case TMR_CLK_PRESCALER_32:
            TMR_PRESCALER_SET ( 0x5 );
            break;

        case TMR_CLK_PRESCALER_64:
            TMR_PRESCALER_SET ( 0x6 );
            break;

        case TMR_CLK_PRESCALER_128:
            TMR_PRESCALER_SET ( 0x7 );
            break;

        case TMR_CLK_PRESCALER_256:
            TMR_PRESCALER_SET ( 0x8 );
            break;

        default:
            return(TMR_INVALID_PARAMETER);
    }

    tmr_EnterCriticalSection ( &mask );
    g_tmr_system_context.tmr_state[index] = TMR_STATE_RUNNING;
    TMR_TIMER_ENABLE;
    tmr_ExitCriticalSection ( mask );

    return ( TMR_OK );
}

/********************************************************************************/
/* NAME:  t_tmr_error TMR_GetCounterValue(t_tmr_id tmr_id,  		       	*/
/*               		          t_uint16 *p_current_value)           	*/
/*---------------------------------------------------------------------------	*/
/* DESCRIPTION:This routine gets the current counter value.                  	*/
/*               		                                               	*/
/* PARAMETERS:                                                               	*/
/*                                                                           	*/
/* IN :     tmr_id  The tmr_id from where to get the value       	 	*/
/*                                                                           	*/
/* OUT :    *p_current_value   The current value		             	*/
/*                                                                           	*/
/* RETURN: t_tmr_error   This function will return                            	*/
/*                      TMR_OK  			                     	*/
/*                      TMR_ERROR_INVALID_TIMER_ID if the given timer is   	*/
/*                      invalid or unasable			           	*/
/* 		        TMR_ERROR_TIMER_ALREADY_STOPPED timer already stopped   */
/*                                                                           	*/
/* COMMENTS:                                                                 	*/
/*                                                                           	*/
/*---------------------------------------------------------------------------	*/
/* REENTRANCY: Reentrant                                                     	*/
/********************************************************************************/

PUBLIC t_tmr_error TMR_GetCounterValue (t_tmr_id tmr_id, t_uint32 *p_current_value) 
{
    t_tmr_error             tmr_error = TMR_OK;
    t_uint32                index;
    t_tmr_register_subset   *p_tmr_register_subset; 

    /*check that the timer has been queried*/
    for ( index = 0; index < g_tmr_system_context.number_of_total_timers; index++ )
    {
        if ( g_tmr_system_context.tmr_mask[index] == tmr_id )
        {
            /*check the state of the timer*/
            switch (g_tmr_system_context.tmr_state[index])
            {
                case TMR_STATE_FREE:
                case TMR_STATE_BUSY:
                    return(TMR_ERROR_INVALID_TIMER_ID);

                case TMR_STATE_INITIALIZED:
                    return(TMR_ERROR_TIMER_ALREADY_STOPPED);

                case TMR_STATE_RUNNING:
                    /* return the counter*/
                    /* Get the ptr to the structure in use    */
                    p_tmr_register_subset = g_tmr_system_context.tmr_temp_ptr[index]; 

                    /* return the content of the timerXvalue register*/
                    TMR_GET_VALUE_REG(*p_current_value);
                    break;

                default:
                    return(TMR_INVALID_PARAMETER);
            }
            break;
        }
    }

    if (index == g_tmr_system_context.number_of_total_timers)
    {
        return(TMR_ERROR_INVALID_TIMER_ID);
    }

    return(tmr_error);
}


/********************************************************************************/
/* NAME:  t_tmr_error TMR_GetRedgeCounterValue(t_tmr_id tmr_id,                 */
/*                                        t_uint16 *p_current_value)            */
/*---------------------------------------------------------------------------   */
/* DESCRIPTION:This routine gets the current counter value.                     */
/*                                                                              */
/* PARAMETERS:                                                                  */
/*                                                                              */
/* IN :     tmr_id  The tmr_id from where to get the value                      */
/*                                                                              */
/* OUT :    *p_current_value   The current value                                */
/*                                                                              */
/* RETURN: t_tmr_error  This function will return                               */
/*                      TMR_OK                                                  */
/*                      TMR_ERROR_INVALID_TIMER_ID if the given timer is        */
/*                      invalid or unasable                                     */
/*                      TMR_ERROR_TIMER_ALREADY_STOPPED timer already stopped   */
/*                                                                              */
/*                                                                              */
/*---------------------------------------------------------------------------   */
/* REENTRANCY: Reentrant                                                        */
/********************************************************************************/
t_tmr_error TMR_GetRedgeCounterValue ( t_tmr_id tmr_id, t_uint16 *p_current_value )
{
    t_tmr_error             tmr_error = TMR_OK;
    t_uint32                index;
    t_tmr_register_subset   *p_tmr_register_subset;               
    switch ( tmr_id )
    {
        case TMR_ID_0:
            index = 0;
            break;
        case TMR_ID_1:
            index = 1;
            break;
        case TMR_ID_2:
            index = 2;
            break;
        case TMR_ID_3:
            index = 3;
            break;
        case TMR_ID_4:
            index = 4;
            break;
        case TMR_ID_5:
            index = 5;
            break;
        case TMR_ID_6:
            index = 6;
            break;
        case TMR_ID_7:
            index = 7;
            break;
        default:
            return ( TMR_INVALID_PARAMETER );
    }
    /*address of a specific timer area*/
    p_tmr_register_subset = g_tmr_system_context.tmr_temp_ptr[index]; 

    TMR_GET_VALUE_REG ( *p_current_value );
    return ( tmr_error );
}



/********************************************************************************/
/* NAME:  t_tmr_error TMR_GetFedgeCounterValue(t_tmr_id tmr_id,                 */
/*                                        t_uint16 *p_current_value)            */
/*---------------------------------------------------------------------------   */
/* DESCRIPTION:This routine gets the current counter value.                     */
/*                                                                              */
/* PARAMETERS:                                                                  */
/*                                                                              */
/* IN :     tmr_id  The tmr_id from where to get the value                      */
/*                                                                              */
/* OUT :    *p_current_value   The current value                                */
/*                                                                              */
/* RETURN: t_tmr_error  This function will return                               */
/*                      TMR_OK                                                  */
/*                      TMR_ERROR_INVALID_TIMER_ID if the given timer is        */
/*                      invalid or unasable                                     */
/*                      TMR_ERROR_TIMER_ALREADY_STOPPED timer already stopped   */
/*                                                                              */
/*                                                                              */
/*                                                                              */
/*---------------------------------------------------------------------------   */
/* REENTRANCY: Reentrant                                                        */
/********************************************************************************/
t_tmr_error TMR_GetFedgeCounterValue ( t_tmr_id tmr_id, t_uint16 *p_current_value )
{
    t_tmr_error             tmr_error = TMR_OK;
    t_uint32                index;
    t_tmr_register_subset   *p_tmr_register_subset;

    switch ( tmr_id )
    {
        case TMR_ID_0:
            index = 0;
            break;
        case TMR_ID_1:
            index = 1;
            break;
        case TMR_ID_2:
            index = 2;
            break;
        case TMR_ID_3:
            index = 3;
            break;
        case TMR_ID_4:
            index = 4;
            break;
        case TMR_ID_5:
            index = 5;
            break;
        case TMR_ID_6:
            index = 6;
            break;
        case TMR_ID_7:
            index = 7;
            break;
        default:
            return ( TMR_INVALID_PARAMETER );
    }
    /*address of a specific timer area*/
    p_tmr_register_subset = g_tmr_system_context.tmr_temp_ptr[index]; 

    TMR_GET_VALUE_REG ( *p_current_value );
    return ( tmr_error );
}

/********************************************************************************/
/* NAME:  t_tmr_error TMR_FreeTimer(t_tmr_id  tmr_id)     		       	*/
/*------------------------------------------------------------------------------*/
/* DESCRIPTION: This routine frees a precedently allocated timer	        */
/*               		                                                */
/* PARAMETERS:                                                               	*/
/* IN :    tmr_id   :   the variable gives the timer ID to free      	        */
/*                                                                           	*/
/* OUT :      	                  	                  	                */
/*                                                                           	*/
/* RETURN: t_tmr_error  :  TMR error code. This function will return 	        */
/*                         TMR_OK                                               */
/*                         TMR_ERROR_INVALID_TIMER_ID                           */
/*                                  if the given timer is invalid             	*/
/*TYPE:   Public                                                                */
/*                                                                           	*/
/* COMMENTS: The Timer must be valid. It is possible to free a running timer 	*/
/*           without stopping it					        */
/*------------------------------------------------------------------------------*/
/* REENTRANCY:The function is non-reentrant as it modifies global 		*/
/*	      g_tmr_system_context.tmr_state		                        */
/*            Hence it has been put into critical section                    	*/
/********************************************************************************/
PUBLIC t_tmr_error TMR_FreeTimer(t_tmr_id tmr_id)
{
    t_uint32    index;
    t_tmr_error err_status = TMR_OK;
    t_uint32    mask = 0;

    /*check that the timer to free is really busy */
    for (index = 0; index < g_tmr_system_context.number_of_total_timers; index++ )
    {
        if ( g_tmr_system_context.tmr_mask[index] == tmr_id )
        {
            tmr_EnterCriticalSection ( &mask );
            switch ( g_tmr_system_context.tmr_state[index] )
            {
                case TMR_STATE_FREE:
                case TMR_STATE_BUSY:
                case TMR_STATE_INITIALIZED:
                    break;

                case TMR_STATE_RUNNING:
                    /*stop the running timer*/
                    err_status = TMR_StopTimer ( tmr_id );
                    break;

                default:
                    return ( TMR_INVALID_PARAMETER );
            } 

            g_tmr_system_context.tmr_state[index] = TMR_STATE_FREE;
            tmr_ExitCriticalSection ( mask );
            break;
        }
    }

    if ( index == g_tmr_system_context.number_of_total_timers )
    {
        err_status = TMR_ERROR_INVALID_TIMER_ID;
    }

    return ( err_status );
}

/********************************************************************************/
/* NAME:	t_tmr_error TMR_SetDbgLevel(t_dbg_level dbg_level)     	 	*/
/*------------------------------------------------------------------------------*/
/* DESCRIPTION: This routine enables to choose between different debug       	*/
/*              comments levels                                              	*/
/*                                                                           	*/
/* PARAMETERS:                                                               	*/
/* IN :            dbg_level:          identify TMR debug level            	*/
/*                                                                           	*/
/*                                                                           	*/
/* RETURN:         t_tmr_error :          TMR error code                     	*/
/*                                                                           	*/
/*-----------------------------------------------------------------------------	*/
/* REENTRANCY: NA                                                            	*/
/********************************************************************************/

PUBLIC t_tmr_error TMR_SetDbgLevel ( t_dbg_level dbg_level )
{
    dbg_level = dbg_level;  /*for PC LINT*/
#ifdef __DEBUG
    MY_DEBUG_LEVEL_VAR_NAME = dbg_level;
#endif
    return ( TMR_OK );
}

/********************************************************************************/
/* NAME:	t_tmr_error TMR_GetDbgLevel(t_dbg_level *dbg_level)          	*/
/*---------------------------------------------------------------------------	*/
/* DESCRIPTION: This routine enables to choose between different debug       	*/
/*              comments levels                                              	*/
/*                                                                           	*/
/* PARAMETERS:                                                               	*/
/* IN :                                                                      	*/
/*                                                                           	*/
/* OUT:           dbg_level:           identify TMR debug level            	*/
/*                                                                           	*/
/* RETURN:        t_tmr_error :           TMR error code                     	*/
/*                                                                           	*/
/*---------------------------------------------------------------------------	*/
/* REENTRANCY: NA                                                            	*/
/********************************************************************************/

PUBLIC t_tmr_error TMR_GetDbgLevel ( t_dbg_level *dbg_level )
{
#ifdef __DEBUG
    * dbg_level = myDebugLevel_TMR;
#endif
    dbg_level = dbg_level;
    return ( TMR_OK );
}

/********************************************************************************/
/* NAME:  t_tmr_error TMR_GetVersion(t_version *p_version)     	         	*/
/*---------------------------------------------------------------------------	*/
/* DESCRIPTION: This routine returns the TMR HCL version	             	*/
/*               		                                               	*/
/* PARAMETERS:                                                               	*/
/* IN :      	                  	                  	               	*/
/*                                                                           	*/
/* OUT :    *p_version  : The Timer HCL version                        	        */
/*                                                                           	*/
/* RETURN:  t_tmr_error : TMR error code                     	                */
/*                                                                           	*/
/*---------------------------------------------------------------------------	*/
/* REENTRANCY: NA                                                            	*/
/********************************************************************************/

PUBLIC t_tmr_error TMR_GetVersion ( t_version *p_version )
{
    t_tmr_error tmr_error = TMR_OK;

    p_version->version = TMR_HCL_VERSION_ID;
    p_version->major = TMR_MAJOR_VERSION;
    p_version->minor = TMR_MINOR_VERSION;

    return ( tmr_error );
}


/********************************************************************************/
/* NAME:	void TMR_SaveDeviceContext()					*/
/*------------------------------------------------------------------------------*/
/* DESCRIPTION: This routine saves current values of TMR hardware for power 	*/
/*		management.		                                        */
/*										*/
/* PARAMETERS:									*/
/* IN			NONE							*/
/* OUT			NONE							*/
/* RETURN:		void							*/
/* TYPE:                Public					                */
/*------------------------------------------------------------------------------*/
/* REENTRANCY: NA   								*/
/********************************************************************************/

PUBLIC void TMR_SaveDeviceContext(void)
{

    /*UNIT 0*/
    g_tmr_system_context.tmr_device_context[0] = g_tmr_system_context.p_tmr_register[0]->tmr_control1;
    g_tmr_system_context.tmr_device_context[1] = g_tmr_system_context.p_tmr_register[0]->tmr_compare1;
    g_tmr_system_context.tmr_device_context[2] = g_tmr_system_context.p_tmr_register[0]->tmr_count1;
    g_tmr_system_context.tmr_device_context[3] = g_tmr_system_context.p_tmr_register[0]->tmr_redge_capt1;
    g_tmr_system_context.tmr_device_context[4] = g_tmr_system_context.p_tmr_register[0]->tmr_fedge_capt1;

    g_tmr_system_context.tmr_device_context[5] = g_tmr_system_context.p_tmr_register[0]->tmr_control2;
    g_tmr_system_context.tmr_device_context[6] = g_tmr_system_context.p_tmr_register[0]->tmr_compare2;
    g_tmr_system_context.tmr_device_context[7] = g_tmr_system_context.p_tmr_register[0]->tmr_count2;
    g_tmr_system_context.tmr_device_context[8] = g_tmr_system_context.p_tmr_register[0]->tmr_redge_capt2;
    g_tmr_system_context.tmr_device_context[9] = g_tmr_system_context.p_tmr_register[0]->tmr_fedge_capt2;



    /*UNIT 1*/
    g_tmr_system_context.tmr_device_context[10] = g_tmr_system_context.p_tmr_register[1]->tmr_control1;
    g_tmr_system_context.tmr_device_context[11] = g_tmr_system_context.p_tmr_register[1]->tmr_compare1;
    g_tmr_system_context.tmr_device_context[12] = g_tmr_system_context.p_tmr_register[1]->tmr_count1;
    g_tmr_system_context.tmr_device_context[13] = g_tmr_system_context.p_tmr_register[1]->tmr_redge_capt1;
    g_tmr_system_context.tmr_device_context[14] = g_tmr_system_context.p_tmr_register[1]->tmr_fedge_capt1;

    g_tmr_system_context.tmr_device_context[15] = g_tmr_system_context.p_tmr_register[1]->tmr_control2;
    g_tmr_system_context.tmr_device_context[16] = g_tmr_system_context.p_tmr_register[1]->tmr_compare2;
    g_tmr_system_context.tmr_device_context[17] = g_tmr_system_context.p_tmr_register[1]->tmr_count2;
    g_tmr_system_context.tmr_device_context[18] = g_tmr_system_context.p_tmr_register[1]->tmr_redge_capt2;
    g_tmr_system_context.tmr_device_context[19] = g_tmr_system_context.p_tmr_register[1]->tmr_fedge_capt2;


}

/********************************************************************************/
/* NAME:	void TMR_RestoreDeviceContext()					*/
/*------------------------------------------------------------------------------*/
/* DESCRIPTION: This routine restore values of TMR hardware.			*/
/*              Counting resumes from where it was halted when request for sleep*/
/*		mode came                                     	                */
/*										*/
/* PARAMETERS:									*/
/* IN			NONE							*/
/* OUT			NONE						        */
/* RETURN:		void							*/
/* TYPE:		Public							*/
/*------------------------------------------------------------------------------*/
/* REENTRANCY:NA   								*/
/********************************************************************************/

PUBLIC void TMR_RestoreDeviceContext(void)
{

    /*restore the load and Value registers first*/
    g_tmr_system_context.p_tmr_register[0]->tmr_compare1 = g_tmr_system_context.tmr_device_context[1];
    g_tmr_system_context.p_tmr_register[0]->tmr_count1 = g_tmr_system_context.tmr_device_context[2]  ;
    g_tmr_system_context.p_tmr_register[0]->tmr_redge_capt1 = g_tmr_system_context.tmr_device_context[3] ;
    g_tmr_system_context.p_tmr_register[0]->tmr_fedge_capt1 = g_tmr_system_context.tmr_device_context[4] ;
    g_tmr_system_context.p_tmr_register[0]->tmr_compare2 = g_tmr_system_context.tmr_device_context[6];
    g_tmr_system_context.p_tmr_register[0]->tmr_count2 = g_tmr_system_context.tmr_device_context[7]  ;
    g_tmr_system_context.p_tmr_register[0]->tmr_redge_capt2 = g_tmr_system_context.tmr_device_context[8] ;
    g_tmr_system_context.p_tmr_register[0]->tmr_fedge_capt2 = g_tmr_system_context.tmr_device_context[9] ;

    g_tmr_system_context.p_tmr_register[1]->tmr_compare1 = g_tmr_system_context.tmr_device_context[11];
    g_tmr_system_context.p_tmr_register[1]->tmr_count1 = g_tmr_system_context.tmr_device_context[12]  ;
    g_tmr_system_context.p_tmr_register[1]->tmr_redge_capt1 = g_tmr_system_context.tmr_device_context[13] ;
    g_tmr_system_context.p_tmr_register[1]->tmr_fedge_capt1 = g_tmr_system_context.tmr_device_context[14] ;
    g_tmr_system_context.p_tmr_register[1]->tmr_compare2 = g_tmr_system_context.tmr_device_context[16];
    g_tmr_system_context.p_tmr_register[1]->tmr_count2 = g_tmr_system_context.tmr_device_context[17]  ;
    g_tmr_system_context.p_tmr_register[1]->tmr_redge_capt2 = g_tmr_system_context.tmr_device_context[18] ;
    g_tmr_system_context.p_tmr_register[1]->tmr_fedge_capt2 = g_tmr_system_context.tmr_device_context[19] ;

    /*now restore the control registers*/
    g_tmr_system_context.p_tmr_register[0]->tmr_control1 = g_tmr_system_context.tmr_device_context[0] ;
    g_tmr_system_context.p_tmr_register[0]->tmr_control2 = g_tmr_system_context.tmr_device_context[5] ;
    g_tmr_system_context.p_tmr_register[0]->tmr_control1 = g_tmr_system_context.tmr_device_context[10] ;
    g_tmr_system_context.p_tmr_register[0]->tmr_control2 = g_tmr_system_context.tmr_device_context[15] ;

}


/********************************************************************************/
/* NAME:	 TMR_GetIRQSrcStatus()                                          */
/*---------------------------------------------------------------------------   */
/* DESCRIPTION   : This function updates a <t_tmr_irq_status> structure         */
/*		   according to the <t_tmr_irq_src> parameter. It allows to     */
/*		   store the reason of the interrupt within the                 */
/*		  <t_tmr_irq_status> parameter.                                 */
/* PARAMETERS    :								*/
/* IN            : tmr_irq_src : irqsource number                               */
/*	          * p_status : pointer to the structure to be updated            */
/* INOUT         : None                                                         */
/* OUT           : None                                                         */
/* RETURN VALUE  : void								*/
/* TYPE          : Public                                                       */
/*---------------------------------------------------------------------------   */
/* REENTRANCY: Re-entrant							*/
/********************************************************************************/

PUBLIC void TMR_GetIRQSrcStatus ( t_tmr_irq_src tmr_irq_src ,
        t_tmr_irq_status *p_status )
{
    t_tmr_irq_src   tmrstat = 0;
    t_tmr_device_id tmr_device_id = TMR_DEVICE_ID_0;
    tmr_device_id = TMR_GetDeviceID ( tmr_irq_src );

    if ( TMR_DEVICE_ID_INVALID == tmr_device_id )
    {
        return;
    }

    tmrstat = TMR_GetIRQSrc ( tmr_device_id ); /* Take care TBC TBD*/
    p_status-> pending_irq = tmrstat;       
    p_status-> initial_irq = tmrstat;
    p_status-> interrupt_state = TMR_IRQ_STATE_NEW;
}

/********************************************************************************/
/* NAME:	 TMR_FilterProcessIRQSrc()                                      */
/*---------------------------------------------------------------------------   */
/* DESCRIPTION :This iterative routine allows to process the device             */
/*              interrupt sources identified through the tmr_error structure.   */
/*		It processes the interrupt sources one by one, updates tmr_error*/
/*              structure and keeps an internal history of the events generated */
/* PARAMETERS  :	 				                        */
/* IN          : *p_tmr_irq_status : pointer to interrupt tmr_error registered  */
/*		 *p_tmr_event      : pointer to store multiple events generated */
/*			             in the current iteration of this function  */
/*		filter_mode        : optional parameters which allows           */
/*                                   to filter the processing and the generation*/
/*                                   of events                                  */
/* INOUT         : None                                                         */
/* OUT           : None                                                         */
/* RETURN VALUE  : t_tmr_error 					        	*/
/* TYPE          : Public                                                       */
/*---------------------------------------------------------------------------   */
/* REENTRANCY: Non -Reentrant as it modifies global variable                    */
/*	       g_tmr_system_context.tmr_state.Hence                             */
/*             it should be called from within a critical section.		*/
/********************************************************************************/

PUBLIC t_tmr_error TMR_FilterProcessIRQSrc
(
 t_tmr_irq_status    *p_tmr_irq_status,
 t_tmr_event         *p_tmr_event,
 t_tmr_filter_mode   filter_mode
 )
{
    t_tmr_irq_src   tmr_irq_src = 0;
    t_uint32        temp_filter;

    if ( ( ( p_tmr_irq_status )->interrupt_state
                == TMR_IRQ_STATE_NEW ) && ( ( p_tmr_irq_status )->pending_irq == 0 ) )
    {
        return ( TMR_NO_PENDING_EVENT_ERROR );
    }
    ( p_tmr_irq_status ) -> interrupt_state = TMR_IRQ_STATE_OLD;
    if ( filter_mode == TMR_NO_FILTER_MODE )
    {
        temp_filter = ( p_tmr_irq_status ) -> pending_irq;
        if ( 0 == temp_filter )
        {
            if ( 0 == ( g_tmr_system_context.tmr_event ) )
            {
                return ( TMR_NO_MORE_PENDING_EVENT );
            }
            else
            {
                return ( TMR_INTERNAL_EVENT );
            }
        }
    }
    else
    {
        temp_filter = filter_mode & (p_tmr_irq_status)->pending_irq;    
        if ( 0 == temp_filter )
        {
            if ( 0 == ( ( g_tmr_system_context.tmr_event ) & filter_mode ) )
            {
                if ( 0 == ( g_tmr_system_context.tmr_event ) )
                {
                    return ( TMR_NO_MORE_PENDING_EVENT );
                }
                else
                {
                    return ( TMR_NO_MORE_FILTER_PENDING_EVENT );
                }
            }
            else
            {
                return ( TMR_INTERNAL_EVENT );
            }
        }
    }
    {
        t_uint8     count;
        t_uint32    mask = 7;
        for ( count = 0; count < g_tmr_system_context.number_of_total_timers; count++ )
        {
            if ( temp_filter & mask )
            {
                tmr_irq_src = mask;
                break;
            }
            mask = mask << 3;
        }
    }
    ( p_tmr_irq_status ) -> pending_irq &= ~ ( tmr_irq_src );

    /*assigning the p_tmr_event to the global variable ,it shd b cleared in Acknowledge fn*/
    ( g_tmr_system_context.tmr_event ) |= tmr_irq_src;
    *p_tmr_event = (t_tmr_event) tmr_irq_src;
    TMR_ClearIRQSrc ( tmr_irq_src );
    {
        t_uint8 index = 0;
        if ( tmr_irq_src & ( TMR_ID_0 | TMR_ID_1 | TMR_ID_2 | TMR_ID_3 ) )
        {
            if ( tmr_irq_src & TMR_ID_0 )
                index = 0;
            else if ( tmr_irq_src & TMR_ID_0 )
                index = 1;
            else if ( tmr_irq_src & TMR_ID_2 )
                index = 2;
            else if ( tmr_irq_src & TMR_ID_3 )
                index = 3;
            if (TMR_STATE_RUNNING == g_tmr_system_context.tmr_state[index])
            {
                if (TMR_MODE_ONE_SHOT == g_tmr_system_context.tmr_mode[index])
                {
                    t_tmr_register_subset   *p_tmr_register_subset;
                    p_tmr_register_subset = g_tmr_system_context.tmr_temp_ptr[index];
                    TMR_TIMER_DISABLE;
                    g_tmr_system_context.tmr_state[index] = TMR_STATE_BUSY;
                }
            }
        }
    }

    return(TMR_REMAINING_PENDING_EVENTS);
}

/********************************************************************************/
/* NAME:	 TMR_AcknowledgeEvent()                                         */
/*---------------------------------------------------------------------------   */
/* DESCRIPTION   : This routine allows to acknowledge the related internal      */
/*		   p_tmr_event                                                  */
/* PARAMETERS    :                                                              */
/* IN            : *p_event  pointer to acknowledge multiple events generated   */
/* INOUT         : None                                                         */
/* OUT           : None                                                         */
/* RETURN VALUE  : void                                                         */
/* TYPE          : Public                                                       */
/*---------------------------------------------------------------------------   */
/* REENTRANCY: Re-entrant                                                       */
/********************************************************************************/

PUBLIC void TMR_AcknowledgeEvent ( const t_tmr_event *p_event )
{
    if ( p_event != NULL )
    {
        g_tmr_system_context.tmr_event &= ~ ( *p_event );
    }

}

/********************************************************************************/
/* NAME:	 TMR_IsEventActive()                                            */
/*---------------------------------------------------------------------------   */
/* DESCRIPTION  :This routine allows to check whether the given event is        */
/*		 active or not by checking the global event variable of the HCL.*/
/* PARAMETERS   :								*/
/* IN           : *p_event : the event for which its status has                 */
/*		  to be checked                                                 */
/* INOUT        : None                                                          */
/* OUT          : None                                                          */
/* RETURN VALUE : t_bool   TRUE if the event is active.		   	        */
/* TYPE         : Public                                                        */
/*---------------------------------------------------------------------------   */
/* REENTRANCY: Re-entrant						        */
/********************************************************************************/

PUBLIC t_bool TMR_IsEventActive ( const t_tmr_irq_src *p_event )
{
    if ( ( ( t_tmr_event) ( *p_event ) & g_tmr_system_context.tmr_event ) != 0 )
    {
        return ( TRUE );
    }
    else
    {
        return ( FALSE );
    }
}

/********************************************************************************/
/*End of PUBLIC functions, start PRIVATE area of functions declaration       	*/

/********************************************************************************/
PRIVATE void tmr_EnterCriticalSection(t_uint32 *maskstat)
{

}

PRIVATE void tmr_ExitCriticalSection(t_tmr_irq_src tmr_irq_src_maskstat)
{

}


unsigned int gptGetTimerVal(unsigned int timer)
{
    return(0);
}

/* End of file - tmr.c */













