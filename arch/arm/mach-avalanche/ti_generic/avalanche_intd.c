/*
 *
 * avalanche_intd.c
 * Description:
 * Interrupt distributor implementation
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
/** \file   Avalanche_intd.c
    \brief  Interrupt Distributor API implementation

    FOR DOCUMENTATION OF FUNCTIONS: Refer file avalanche_intd.h
    \author     PSP TII
    \version    0.1
 */

#include <asm-arm/arch-avalanche/generic/pal.h>

extern AVALANCHE_SOC_INFO soc_info;

#define INTD_MAX_ENABLE_REGS       ( 32 )
#define INTD_MAX_STATUS_REGS       ( 32 )
#define INTD_MAX_POLARITY_REGS     ( 32 )
#define INTD_MAX_TYPE_REGS         ( 32 )
#define INTD_MAX_COUNT_REGS        ( 64 )
#define INTD_MAX_PACER_REGS        ( 50 )
#define INTD_MAX_VECT_VAL          ( 255 )    
#define INTD_MAX_INTERRUPT_COUNT   ( 255 )
#define INTD_SET_BIT_0             ( 0x00000001 )
#define INTD_CLEAR_BIT_0           ( 0xFFFFFFFE )

/*Avalanche Interrupt Distributor registers */
struct avalanche_idist_regs *avalanche_hw0_idregs  = (struct avalanche_idist_regs*) AVALANCHE_IDIST_REGS_BASE;
#if PUMA7_OR_NEWER_SOC_TYPE
/* Puma7 introduce second INTD */
struct avalanche_idist_regs *avalanche_hw0_id1regs = (struct avalanche_idist_regs*) AVALANCHE_IDIST1_REGS_BASE;
#endif

#define INTD_GLOBAL_PCAER_MODE_FLAG      ( 0xF0000000 )

#define INTD_RESTART_AFTER_INT           ( 0x01000000 )
#define INTD_PACER_NORMAL_MODE           ( 0x00000EFF )
#define INTD_PACEMODE_FLAG               ( 0x30000000 )
#define INTD_PACER_COUNT_BIT_POS         ( 16 )
#define INTD_MAX_PRESCALE_COUNT          ( 0xFFF )
#define INTD_PACER_COUNT_FLAG            ( 0x00FF0000 )
#define INTD_PACER_RESTART_MODE_ON       ( 0x01000000 )
#define INTD_PACER_RESTART_MODE_OFF      ( 0xF7FFFFFF )



#define INTD_PACER_MAX_VAL               ( 0xFFFF )
#define INTD_PACER_MAX_VAL_FLAG          ( 0x0000FFFF )
#define INTD_PACER_TEST_BIT              ( 30 ) 
#define INTD_PACER_BYPASS_BIT              ( 31 ) 

/* Utility function */
static inline unsigned char get_bit_position ( unsigned int ip_int_num );
#define INTD_PACER_FREQ_MAX_VAL             0xFFFF // Check 
#define INTD_PACER_FREQ_VAL_FLAG            0x00

#define INTD_PACER_TEST_MODE               ( 0x40000000 )
#define INTD_PACER_FREQ_BASED              ( 0xCFFFFFFF )
#define INTD_PACER_COUNT_BASED             ( 0x10000000 )
#define INTD_PACER_TIME_BASED              ( 0x20000000 )
#define INTD_PACER_COUNT_AND_TIME_BASED    ( 0x30000000 )


/**********************************************************************************
*   if Required put the below function declerations in
*   include/asm-arm/arch-avalanche/generic/avalanche_intd.h
**********************************************************************************/
#define INTD_TYPE_LEVEL             ( 0 )
#define INTD_TYPE_EDGE              ( 1 )
#define INTD_POLARITY_ACTIVE_LOW    ( 0 )
#define INTD_POLARITY_ACTIVE_HIGH   ( 1 )

int avalanche_intd_set_interrupt_polarity( unsigned int host_num,
                                           unsigned int ip_int_num,
                                           unsigned int polarity );

int avalanche_intd_get_interrupt_polarity(unsigned int host_num,
                                          unsigned int ip_int_num);

int avalanche_intd_set_interrupt_type( unsigned int host_num,
                                       unsigned int ip_int_num,
                                       unsigned int int_type );

int avalanche_intd_get_interrupt_type( unsigned int host_num,
                                       unsigned int ip_int_num );



#define INTD_GLOBAL_PACER_TEST_MODE      ( 0x80000000 )
#define INTD_GLOBAL_PACER_BYPASSMODE     ( 0x20000000 )
#define INTD_GLOBAL_PACER_NORMALMODE     ( 0x000FFFFF )
/* Noraml mode disables tset and bypass modes */ 

int avalanche_intd_get_glb_pacer_mode( void );

int avalanche_intd_set_glb_pacer_mode( unsigned int glb_pacer_mode );

unsigned int avalanche_intd_get_glb_pacer_prescale_count( void ); 

int avalanche_intd_set_glb_pacer_prescale_count( unsigned int prescale_cnt );


/* By default in all pacers bypass is enabled */

int avalanche_intd_enable_pacer_bypass( unsigned int host_num,
                                        unsigned int ip_int_num );

int avalanche_intd_disable_pacer_bypass( unsigned int host_num, 
                                         unsigned int ip_int_num );

int avalanche_intd_enable_pacer_testmode( unsigned int host_num,
                                          unsigned int ip_int_num);
                                          
int avalanche_intd_disable_pacer_testmode( unsigned int host_num,
                                           unsigned int ip_int_num );

/* The following API disbales the bypass abd testmode */
int avalanche_intd_set_pacer_normalmode( unsigned int host_num,
                                         unsigned int ip_int_num );

#define INTD_PACEMODE_FREQ_BASED            ( 0x00000000 )
#define INTD_PACEMODE_COUNT_BASED           ( 0x10000000 )
#define INTD_PACEMODE_TIME_BASED            ( 0x20000000 )
#define INTD_PACEMODE_COUNT_AND_TIME_BASED  ( 0x30000000 )

int avalanche_intd_get_pacer_pacemode( unsigned int host_num,
                                       unsigned int ip_int_num );

int avalanche_intd_set_pacer_pacemode( int host_num, 
                                       unsigned int ip_int_num,
                                       unsigned int pacer_type );

/* max_value Sets the interrupt max value.
 * For frequency-based pacing this is the maximum number 
 * of interrupts per millisecond value. For time-based pacing 
 * this is the time in ticks to count to.
 */

int avalanche_intd_get_pacer_max( unsigned int host_num,
                                         unsigned int ip_int_num );
int avalanche_intd_set_pacer_max( unsigned int host_num, 
                                  unsigned int ip_int_num,
                                  unsigned int max_value );


int avalanche_intd_get_pacer_cnt_thresh( unsigned int host_num,
                                         unsigned int ip_int_num );
int avalanche_intd_set_pacer_cnt_thresh( unsigned int host_num,
                                         unsigned int ip_int_num,
                                         unsigned int count_val );



/* Restart Mode. Determines whether the timer starts after the last 
 * paced interrupt or the next input interrupt. 
 * Used for timer-based pacing only. 
 * Set = after the next input interrupt
 * clear  = after the last paced output interrupt
 */
int avalanche_intd_set_pacer_restart_mode( unsigned int host_num, 
                                           unsigned int ip_int_num );
int avalanche_intd_clear_pacer_restart_mode( unsigned int host_num, 
                                             unsigned int ip_int_num );

/**********************************************************************************
*   if Required put the above function declerations in
*   include/asm-arm/arch-avalanche/generic/avalanche_intd.h
**********************************************************************************/


/* Functions Definition */
int avalanche_intd_enable_interrupt( unsigned int host_num,
                                     unsigned int ip_int_num )
{
    if( ( host_num >= 0 && host_num < soc_info.max_num_hosts ) &&
        ( ip_int_num >= 0 && 
          ip_int_num < soc_info.host_info[host_num].max_ip_ints_mapped ))
    { 
        unsigned int host_enb_reg_indx = host_num +
                                         (ip_int_num % NUM_INTS_PER_REG);
        if( host_enb_reg_indx < INTD_MAX_ENABLE_REGS )
        {
            /* Set the enable set  bit for the proper reg */       
            avalanche_hw0_idregs->idenbsr[host_enb_reg_indx] =
                      (1 << get_bit_position ( ip_int_num ) );
        }
        else
        {
           return(-1);    
        }
    }  
    else
    {
        return(-1);    
    }
    return(0);
}



int avalanche_intd_disable_interrupt( unsigned int host_num,
                                      unsigned int ip_int_num )
{
    if( ( host_num >= 0 && host_num < soc_info.max_num_hosts) &&
        ( ip_int_num >= 0 && 
          ip_int_num < soc_info.host_info[host_num].max_ip_ints_mapped ))
    { 
        unsigned int host_enb_reg_indx = host_num +
                                         (ip_int_num / NUM_INTS_PER_REG);
        if( host_enb_reg_indx < INTD_MAX_ENABLE_REGS )
        {
            /* Set the enable Clear  bit for the proper reg */       
            avalanche_hw0_idregs->idenbcr[host_enb_reg_indx]  =
                      (1 << get_bit_position ( ip_int_num ) );
        }
        else
        {
           return(-1);    
        }
    }  
    else
    {
        return(-1);    
    }
    return(0);
}



int avalanche_intd_set_interrupt_status( unsigned int host_num,
                                         unsigned int ip_int_num ) 
{
    if( ( host_num >= 0 && host_num < soc_info.max_num_hosts ) &&
        ( ip_int_num >= 0 && 
          ip_int_num < soc_info.host_info[host_num].max_ip_ints_mapped ))
    { 
        unsigned int host_sta_reg_indx = host_num +
                                         (ip_int_num / NUM_INTS_PER_REG);
        if( host_sta_reg_indx < INTD_MAX_STATUS_REGS )
        {
            /* Set the enable bit for the proper reg */               
            avalanche_hw0_idregs->idstasr[ host_sta_reg_indx ] =
                        (1 <<  get_bit_position ( ip_int_num ) );
        }
        else
        {
           return(-1);    
        }
   }  
    else
    {
        return(-1);    
    }
    return(0);
}   



int avalanche_intd_clear_interrupt_status( unsigned int host_num,
                                           unsigned int ip_int_num ) 
{
    if( ( host_num >= 0 && host_num < soc_info.max_num_hosts ) &&
        ( ip_int_num >= 0 && 
          ip_int_num < soc_info.host_info[ host_num ].max_ip_ints_mapped ))
    { 
        unsigned int host_sta_reg_indx = host_num + 
                                        (ip_int_num / NUM_INTS_PER_REG);
        if( host_sta_reg_indx < INTD_MAX_STATUS_REGS )
        {
             /* Set the enable bit for the proper reg */             
             avalanche_hw0_idregs->idstacr [ host_sta_reg_indx ] = 
                          ( 1 << get_bit_position ( ip_int_num ) );
        }
        else
        {
           return(-1);    
        }
    }  
    else
    {
        return(-1);    
    }
    return(0);
}


 
int avalanche_intd_is_status_set( unsigned int host_num,
                                  unsigned int ip_int_num )
{    
    if( ( host_num >= 0 && host_num < soc_info.max_num_hosts ) &&
        ( ip_int_num >= 0 && 
          ip_int_num < soc_info.host_info[host_num].max_ip_ints_mapped ))
    { 
        unsigned int host_sta_reg_indx = host_num +
                                         (ip_int_num / NUM_INTS_PER_REG); 
        if( host_sta_reg_indx < INTD_MAX_STATUS_REGS )
        {
            return(( avalanche_hw0_idregs->idstasr[ host_sta_reg_indx ] >>
                                        get_bit_position ( ip_int_num )) & 0x1 );
        }
        else
        {
           return(-1);    
        }
    }  
    else
    {       
      return(-1);
    }
}



int avalanche_intd_is_status_cleared( unsigned int host_num, 
                                      unsigned int ip_int_num )
{
    if( ( host_num >= 0 && host_num < soc_info.max_num_hosts ) &&
        ( ip_int_num >= 0 && 
          ip_int_num < soc_info.host_info[host_num].max_ip_ints_mapped ) )
    { 
        unsigned int host_sta_reg_indx =  host_num +
                                          (ip_int_num / NUM_INTS_PER_REG);

        if( host_sta_reg_indx < INTD_MAX_STATUS_REGS )
        {
            return(( avalanche_hw0_idregs->idstacr[ host_sta_reg_indx ] >>
                                       get_bit_position ( ip_int_num )) & 0x1 );
        }
        else
        {
            return(-1);    
        }
    }
    else
    {       
      return(-1);
    }
}   



int avalanche_intd_read_eoi( void )
{
    return( avalanche_hw0_idregs->ideoir );
}


int avalanche_intd_write_eoi( unsigned int vect_val )
{
    if( vect_val >= 0 && vect_val <= INTD_MAX_VECT_VAL ) 
    {
#if PUMA7_OR_NEWER_SOC_TYPE
        if (AVALANCHE_IDIST_SYS_INT_NUM <= vect_val)
        {
            avalanche_hw0_id1regs->ideoir = vect_val - AVALANCHE_IDIST_SYS_INT_NUM ;
        }
        else
#endif
        {
            avalanche_hw0_idregs->ideoir = vect_val ;
        }
        /* printk ("Writing %x to EOI reg @ %x\n", vect_val, &avalanche_hw0_idregs->ideoir); */
        return(0);
    } 
    return(-1);
}

/* Clarify whether its vector index or interrupt number stored in
 * EOI Interrupt Vector Regsiter (legacy) 
 */
int avalanche_intd_get_priority_interrupt_vect_indx( void )
{
    return ( avalanche_hw0_idregs->ideoiintvr );
}

long avalanche_intd_get_priority_interrupt_isr_addr( unsigned int host_num ,
                                 unsigned char priority_interrupt_vect_indx )
{
      /* do we need host for this evaluation */
    return (long)(avalanche_hw0_idregs->idintvr[priority_interrupt_vect_indx]);
      
/* Clarify 
    if( host_num >= 0 && host_num < soc_info.max_num_hosts &&
        priority_interrupt_vect_indx >=0 && 
        priority_interrupt_vect_indx < soc_info.host_info[host_num].max_ip_ints_mapped )
    {
            unsigned int vect_host_base_indx = host_num + 
            ( priority_interrupt_vect_indx / NUM_INTS_PER_REG);
            
            return (long) (avalanche_hw0_idregs->idintvr[ vect_host_base_indx +
                                                priority_interrupt_vect_indx]);
       }
   }
*/
   return (long)NULL;
}


int avalanche_intd_get_interrupt_count( unsigned int host_num,
                                                 unsigned int ip_int_num )
{ 
   if( ( host_num >= 0 && host_num < soc_info.max_num_hosts ) && 
       ( ip_int_num >= 0 && ip_int_num < INTD_MAX_COUNT_REGS )&&
       ( ip_int_num < soc_info.host_info[host_num].max_ip_ints_mapped ))
   { 
        unsigned int ip_ints_per_host = 0,counter_reg_indx = 0, i;  

        if(host_num > 0  )
        {
            for( i = 0; i < host_num; i++ )
            {
             ip_ints_per_host += soc_info.host_info[i].max_ip_ints_mapped;
            }
            counter_reg_indx = ip_ints_per_host + ip_int_num ;
        }
        else
        {
            counter_reg_indx = ip_int_num;
#if PUMA7_OR_NEWER_SOC_TYPE
            counter_reg_indx -= (AVALANCHE_IDIST_IP_INT_NUM <= ip_int_num) ? AVALANCHE_IDIST_IP_INT_NUM : 0;
#endif

        }

        if( counter_reg_indx < INTD_MAX_COUNT_REGS )
        {   
#if PUMA7_OR_NEWER_SOC_TYPE
            if (AVALANCHE_IDIST_IP_INT_NUM <= ip_int_num)
            {
                return (avalanche_hw0_id1regs->idcounterr[counter_reg_indx]);
            }
#endif
            return (avalanche_hw0_idregs->idcounterr[counter_reg_indx]);
        }
        else
        {
            return(-1);    
        }
    }
    else
    {
        return(-1);    
    }
}


/* write of count_val will substruct count_val from the ip_int_num counter register */
int avalanche_intd_set_interrupt_count( unsigned int host_num,
                                        unsigned int ip_int_num, 
                                        unsigned int count_val ) 
{
    if( ( host_num >= 0 && host_num < soc_info.max_num_hosts ) && 
        ( ip_int_num >= 0 && ip_int_num < INTD_MAX_COUNT_REGS) &&
        ( ip_int_num < soc_info.host_info[host_num].max_ip_ints_mapped ) &&
        ( count_val < INTD_MAX_INTERRUPT_COUNT ) )
   { 
        unsigned int ip_ints_per_host = 0,counter_reg_indx = 0, i;  
        if( host_num > 0 )
        {
            for( i = 0; i < host_num; i++ )
            {
             ip_ints_per_host += soc_info.host_info[i].max_ip_ints_mapped;
            } 
        counter_reg_indx = ip_ints_per_host + ip_int_num ;
        }
        else
        {
            counter_reg_indx = ip_int_num ;
#if PUMA7_OR_NEWER_SOC_TYPE
            counter_reg_indx -= (AVALANCHE_IDIST_IP_INT_NUM <= ip_int_num) ? AVALANCHE_IDIST_IP_INT_NUM : 0;
#endif
        }
        if( counter_reg_indx < INTD_MAX_COUNT_REGS )
        {
#if PUMA7_OR_NEWER_SOC_TYPE
            if (AVALANCHE_IDIST_IP_INT_NUM < ip_int_num)
            {
                avalanche_hw0_id1regs->idcounterr[counter_reg_indx] = count_val;
            }
            else
#endif
            {
                avalanche_hw0_idregs->idcounterr[counter_reg_indx] = count_val;
                /* printk ("Writing %x to count reg %d @ %x\n", count_val,
                                counter_reg_indx,
                                &avalanche_hw0_idregs->idcounterr[counter_reg_indx]); */
            }

        }
        else
        {
            return(-1);
        }
    }
    else
    {
        return(-1);    
    }
    return(0);
}

int  avalanche_intd_set_interrupt_polarity( unsigned int host_num, 
                                            unsigned int ip_int_num,  
                                            unsigned int polarity )
{
    if( ( host_num >= 0 && host_num < soc_info.max_num_hosts ) && 
        ( ip_int_num < soc_info.host_info[ host_num ].max_ip_ints_mapped &&
          ip_int_num >= 0 ) && 
        ( polarity == INTD_POLARITY_ACTIVE_LOW || 
          polarity == INTD_POLARITY_ACTIVE_HIGH ) )   
   {
        unsigned int pol_reg_indx = host_num + 
                                    ( ip_int_num / NUM_INTS_PER_REG);
        if( pol_reg_indx < INTD_MAX_POLARITY_REGS )
        {
            if(polarity)
            {   
                avalanche_hw0_idregs->idpolr[pol_reg_indx] |= 
                   (1 << get_bit_position( ip_int_num ));
            }
            else
            {
                avalanche_hw0_idregs->idpolr[pol_reg_indx] &= 
                   ~(1 << get_bit_position( ip_int_num ));
            }
        }   
        else
        {
            return(-1);
        }    
    }   
    else
   {
        return(-1);
   }    
  return(0);       

}
    
int avalanche_intd_get_interrupt_polarity( unsigned int host_num, 
                                           unsigned int ip_int_num)
{
    if( ( host_num >= 0 && host_num < soc_info.max_num_hosts ) &&
        ( ip_int_num >= 0 && 
          ip_int_num < soc_info.host_info[ host_num ].max_ip_ints_mapped ))
    {
        unsigned int pol_reg_indx = host_num + 
                                    (ip_int_num / NUM_INTS_PER_REG);
        if( pol_reg_indx < INTD_MAX_POLARITY_REGS )
        {   
            return ((avalanche_hw0_idregs->idpolr[pol_reg_indx]  >>
                       get_bit_position( ip_int_num )) & 0x1);
        }   
        else
        {
            return(-1);
        }    
    }   
    else
   {
        return(-1);
   }    
}

 
int avalanche_intd_set_interrupt_type( unsigned int host_num,
                                              unsigned int ip_int_num,  
                                              unsigned int int_type ) 
{
    if( ( host_num >= 0 && host_num < soc_info.max_num_hosts ) &&
        ( int_type == INTD_TYPE_LEVEL || int_type == INTD_TYPE_EDGE )&&  
        ( ip_int_num >= 0 && 
          ip_int_num < soc_info.host_info[host_num].max_ip_ints_mapped ))
    {
        unsigned int type_reg_indx = host_num + 
                                     (ip_int_num / NUM_INTS_PER_REG);
        if( type_reg_indx < INTD_MAX_TYPE_REGS )
        {
            if(int_type)
            {
                avalanche_hw0_idregs->idtypr[type_reg_indx] |= 
                    (1 << get_bit_position( ip_int_num ));     
            }
            else
            {
                avalanche_hw0_idregs->idtypr[type_reg_indx] &= 
                    ~(1 << get_bit_position( ip_int_num ));     
            }
        }
        else
        {
             return(-1);
        }
    }   
    else
   {
        return(-1);
   }    
   return(0);      
}

 
int avalanche_intd_get_interrupt_type( unsigned int host_num,
                                                   unsigned int ip_int_num) 
{
    if( ( host_num >= 0 && host_num < soc_info.max_num_hosts ) &&
        ( ip_int_num >= 0 && ip_int_num < soc_info.host_info[host_num].max_ip_ints_mapped ))
    {
        unsigned int type_reg_indx = host_num + (ip_int_num / NUM_INTS_PER_REG);
        if( type_reg_indx < INTD_MAX_TYPE_REGS )
        {
            return (( avalanche_hw0_idregs->idtypr[ type_reg_indx ] >>
                        get_bit_position( ip_int_num )) & 0x1);
        }
        else
        {
           return(-1);    
        }
    }   
    else
    {
        return(-1);
    }    
}


void __init avalanche_intd_hw_init( void )

{
    /*unsigned int host_num, ip_int_num; 
    avalanche_hw0_idregs = ; */

    /* Set Global registers */    
}



int avalanche_intd_get_glb_pacer_mode( void )
{
    return( INTD_GLOBAL_PCAER_MODE_FLAG  & 
            avalanche_hw0_idregs->idpprer );
}

int avalanche_intd_set_glb_pacer_mode( unsigned int glb_pacer_mode )

{
    if( (glb_pacer_mode == INTD_GLOBAL_PACER_TEST_MODE) ||
        (glb_pacer_mode == INTD_GLOBAL_PACER_BYPASSMODE) ||
        (glb_pacer_mode == INTD_GLOBAL_PACER_NORMALMODE) )
    {
        if( glb_pacer_mode == INTD_GLOBAL_PACER_NORMALMODE)
        { 
            avalanche_hw0_idregs->idpprer &= INTD_GLOBAL_PACER_NORMALMODE;
        }
        avalanche_hw0_idregs->idpprer |= glb_pacer_mode;
    }
    else
    {
        return(-1); 
    }
    return(0);
}

  
int avalanche_intd_set_glb_pacer_prescale_count( unsigned int prescale_cnt )
{
    if((prescale_cnt >= 0) && (prescale_cnt < INTD_MAX_PRESCALE_COUNT))
    {
        avalanche_hw0_idregs->idpprer |= prescale_cnt;
    }       
    else
    { 
        return(-1);
    }       
    return 0;
}

 
unsigned int avalanche_intd_get_glb_pacer_prescale_count( void )
{
       return( (unsigned int)( INTD_MAX_PRESCALE_COUNT & 
                        avalanche_hw0_idregs->idpprer) );
 
}
  
int avalanche_intd_enable_pacer_bypass( unsigned int host_num,
                                        unsigned int ip_int_num )
{
    int i=0;
    if( ( host_num >= 0 && host_num < soc_info.max_num_hosts ) &&
        ( ip_int_num >= 0 && ip_int_num < INTD_MAX_PACER_REGS ) &&
        ( ip_int_num <= soc_info.host_info[i].max_ip_ints_mapped) )
    { 
        unsigned int pacer_per_host = 0,i,pacer_reg_indx = 0;   
        if(host_num > 0) 
        {
            for( i = 0; i < host_num; i++ )
            {
                 /* Each IP interrupt mapped to hosti, maps single pacer 
                  * reg.So we can use IP interrupt count to compute  
                  * number of pacer per host.
                  */
                  pacer_per_host += soc_info.host_info[i].max_ip_ints_mapped; 
            } 
            pacer_reg_indx = ip_int_num + pacer_per_host;
        }
        else
        {
            pacer_reg_indx = ip_int_num;
        }
        avalanche_hw0_idregs[pacer_reg_indx].idpacer->idpparamr |= 
                               (1 << INTD_PACER_BYPASS_BIT);
       }
    else
   {
        return(-1);
   }    
   return(0);  
}   
     
int avalanche_intd_disable_pacer_bypass( unsigned int host_num,
                                         unsigned int ip_int_num )
{
    int i=0;
     if( ( host_num >= 0 && host_num < soc_info.max_num_hosts ) &&
        ( ip_int_num >= 0 && ip_int_num < INTD_MAX_PACER_REGS ) &&
        ( ip_int_num <= soc_info.host_info[i].max_ip_ints_mapped) )
     { 
        unsigned int pacer_per_host = 0,i,pacer_reg_indx = 0;
        if(host_num > 0)
        {
            for( i = 1; i < host_num; i++ )
            {
             pacer_per_host += soc_info.host_info[i].max_ip_ints_mapped; 
            }
            pacer_reg_indx = ip_int_num + pacer_per_host;
        }
        else
        {
            pacer_reg_indx = ip_int_num;
        }  
        avalanche_hw0_idregs[ pacer_reg_indx ].idpacer->idpparamr &=
                               ~(1 << INTD_PACER_BYPASS_BIT);
      }
    else
   {
        return(-1);
   }    
   return(0);  
}   
 
//TBD
int avalanche_intd_enable_pacer_testmode( unsigned int host_num,
                                          unsigned int ip_int_num)
{
    int i = 0;
     if( ( host_num >= 0 && host_num < soc_info.max_num_hosts ) &&
        ( ip_int_num >= 0 && ip_int_num < INTD_MAX_PACER_REGS ) &&
        ( ip_int_num <= soc_info.host_info[i].max_ip_ints_mapped) )
     { 
        unsigned int pacer_per_host = 0,i,pacer_reg_indx = 0;
        if(host_num > 0)
        {
            for( i = 1; i < host_num; i++ )
            {
             pacer_per_host += soc_info.host_info[i].max_ip_ints_mapped; 
            }
        pacer_reg_indx = ip_int_num + pacer_per_host;
        }
        else
        {
        pacer_reg_indx = ip_int_num;
        }  
        avalanche_hw0_idregs[ pacer_reg_indx].idpacer->idpparamr |=
                                          (1 << INTD_PACER_TEST_BIT);
                                                      
      }
    else
   {
        return(-1);
   }    
   return(0);  
}


int avalanche_intd_disable_pacer_testmode( unsigned int host_num,
                                           unsigned int ip_int_num )
{
    int i=0;
    if( ( host_num >= 0 && host_num < soc_info.max_num_hosts ) &&
        ( ip_int_num >= 0 && ip_int_num < INTD_MAX_PACER_REGS ) &&
        ( ip_int_num <= soc_info.host_info[i].max_ip_ints_mapped) )
     { 
        unsigned int pacer_per_host = 0,i,pacer_reg_indx = 0;
        if(host_num > 0)
        {
            for( i = 1; i < host_num; i++ )
            {
             pacer_per_host += soc_info.host_info[i].max_ip_ints_mapped; 
            }
            pacer_reg_indx = ip_int_num + pacer_per_host;
        }
        else
        {
            pacer_reg_indx = ip_int_num;
        }  
        avalanche_hw0_idregs[ pacer_reg_indx ].idpacer->idpparamr &= 
                                        ~(1 << INTD_PACER_TEST_BIT);
      }
   else
   {
        return(-1);
   }    
   return(0);  
}

 /*Disables Test and Bypass */
int avalanche_intd_set_pacer_normalmode( unsigned int host_num,
                                         unsigned int ip_int_num )
{    
    int i = 0;
    if( ( host_num >= 0 && host_num < soc_info.max_num_hosts ) &&
        ( ip_int_num >= 0 && ip_int_num < INTD_MAX_PACER_REGS ) &&
        ( ip_int_num <= soc_info.host_info[i].max_ip_ints_mapped) )
 
   { 
        unsigned int pacers_per_host = 0,i,pacer_reg_indx =0;   
        for( i = 1 ; i < host_num ;i++)
        {
            pacers_per_host += soc_info.host_info[i].max_ip_ints_mapped; 
        }
        pacer_reg_indx = ip_int_num + pacers_per_host;
        avalanche_hw0_idregs[ pacer_reg_indx ].idpacer->idpparamr &=
                                              INTD_PACER_NORMAL_MODE;
      }
    else
   {
        return(-1);
   }    
   return(0);  
}



int avalanche_intd_set_pacer_pacemode( int host_num,
                                       unsigned int ip_int_num,
                                       unsigned int pacemode )

{
    int i=0;
    if( ( host_num >= 0 && host_num < soc_info.max_num_hosts ) &&
        ( ip_int_num >= 0 && ip_int_num < INTD_MAX_PACER_REGS ) &&
        ( ip_int_num <= soc_info.host_info[i].max_ip_ints_mapped) &&
        ( pacemode == INTD_PACER_FREQ_BASED ||
          pacemode == INTD_PACER_TIME_BASED ||
          pacemode == INTD_PACER_COUNT_BASED ||   
          pacemode == INTD_PACER_COUNT_AND_TIME_BASED ) )   
    { 
        unsigned int pacers_per_host = 0,i,pacer_reg_indx = 0;  

        for( i = 1 ; i < host_num ;i++)
        {
            pacers_per_host += soc_info.host_info[ i ].max_ip_ints_mapped; 
        }
        pacer_reg_indx = ip_int_num + pacers_per_host;

        /* cleanup bits */
        avalanche_hw0_idregs[ pacer_reg_indx ].idpacer->idpparamr &= 
                                                  INTD_PACER_FREQ_BASED;
        if(pacemode != INTD_PACER_FREQ_BASED )
        {
            avalanche_hw0_idregs[ pacer_reg_indx ].idpacer->idpparamr |= 
                                                               pacemode;
        }  
    }
    else
    {
        return(-1);
    }    
   return(0);      
} 

 
int  avalanche_intd_get_pacer_type ( unsigned int host_num,
                                     unsigned int ip_int_num )
{    
    int i =0;
    if( ( host_num >= 0 && host_num < soc_info.max_num_hosts ) &&
        ( ip_int_num >= 0 && ip_int_num < INTD_MAX_PACER_REGS ) &&
        ( ip_int_num <= soc_info.host_info[i].max_ip_ints_mapped) ) 
   {
        unsigned int pacers_per_host = 0,i,pacer_reg_indx = 0;  
        for( i = 1 ; i < host_num ;i++)
        {
            pacers_per_host += soc_info.host_info[i].max_ip_ints_mapped; 
        }
        pacer_reg_indx = ip_int_num + pacers_per_host;
        return ( INTD_PACEMODE_FLAG &  
                 avalanche_hw0_idregs[pacer_reg_indx].idpacer->idpparamr); 
    }
    else
   {
        return(-1);
   }    
}


int avalanche_intd_set_pacer_cnt_thresh( unsigned int host_num,
                                         unsigned int ip_int_num, 
                                         unsigned int cnt_val )
{
    int i=0;
     if( ( host_num >= 0 && host_num < soc_info.max_num_hosts ) &&
         ( ip_int_num >= 0 && ip_int_num < INTD_MAX_PACER_REGS ) &&
         ( ip_int_num <= soc_info.host_info[i].max_ip_ints_mapped) && 
         ( cnt_val > 0 && cnt_val <= INTD_MAX_PRESCALE_COUNT ) )
    {
        unsigned int pacers_per_host = 0,i,pacer_reg_indx = 0;  
        for( i = 1 ; i < host_num ;i++)
        {
            pacers_per_host += soc_info.host_info[i].max_ip_ints_mapped; 
        }
        pacer_reg_indx = ip_int_num + pacers_per_host;
        avalanche_hw0_idregs[ pacer_reg_indx ].idpacer->idpparamr |=  
                                (cnt_val << INTD_PACER_COUNT_BIT_POS) ;
    }
    else
   {
        return(-1);
   }    
   return(0);      
}


int avalanche_intd_get_pacer_cnt_thresh( unsigned int host_num,
                                         unsigned int ip_int_num )
{    
    int i=0;
    if( ( host_num >= 0 && host_num < soc_info.max_num_hosts ) &&
         ( ip_int_num >= 0 && ip_int_num < INTD_MAX_PACER_REGS ) &&
         ( ip_int_num <= soc_info.host_info[i].max_ip_ints_mapped)) 
 
   {
        unsigned int pacers_per_host = 0,i,pacer_reg_indx = 0;  
        for( i = 1 ; i < host_num ;i++)
        {
            pacers_per_host += soc_info.host_info[i].max_ip_ints_mapped; 
        }
        
        pacer_reg_indx = ip_int_num + pacers_per_host;
        return ( INTD_PACER_COUNT_FLAG & 
                avalanche_hw0_idregs[ pacer_reg_indx ].idpacer->idpparamr );  
                                                 
    }
    else
   {
        return(-1);
   }    
}   

int   avalanche_intd_get_serviced_int_count( unsigned int host_num,
                                             unsigned int ip_int_num )
{
    int i=0;
     if( ( host_num >= 0 && host_num < soc_info.max_num_hosts ) &&
         ( ip_int_num >= 0 && ip_int_num < INTD_MAX_PACER_REGS ) &&
         ( ip_int_num <= soc_info.host_info[i].max_ip_ints_mapped)) 
   {
        unsigned int pacers_per_host = 0,i,pacer_reg_indx = 0;  
        for( i = 1 ; i < host_num ;i++)
        {
            pacers_per_host += soc_info.host_info[i].max_ip_ints_mapped; 
        }
        pacer_reg_indx = ip_int_num + pacers_per_host;
        return ( avalanche_hw0_idregs[ pacer_reg_indx ].idpacer->idpdecr );  
   }
   else
   { 
        return(-1);
   }    
}   


int avalanche_intd_set_serviced_int_count( unsigned int host_num,
                                           unsigned int ip_int_num,
                                           unsigned int cnt_val )
{  
    int i=0; 
    if( ( host_num >= 0 && host_num < soc_info.max_num_hosts ) &&
         ( ip_int_num >= 0 && ip_int_num < INTD_MAX_PACER_REGS ) &&
         ( ip_int_num <= soc_info.host_info[i].max_ip_ints_mapped ) && 
         ( cnt_val >=0 && cnt_val < 0xFF ))
    {
        unsigned int pacers_per_host = 0,i,pacer_reg_indx = 0;  
        for( i = 1 ; i < host_num ;i++)
        {
            pacers_per_host += soc_info.host_info[i].max_ip_ints_mapped; 
        }
        pacer_reg_indx = ip_int_num + pacers_per_host;
        avalanche_hw0_idregs[ pacer_reg_indx ].idpacer->idpdecr = cnt_val;
    }
    else
   {
        return(-1);
   }    
   return(0);      
}   



/* Restart Mode. Determines whether the timer starts after the last
 * paced interrupt or the next input interrupt.
 * Used for timer-based pacing only.
 * Set = after the next input interrupt
 * clear  = after the last paced output interrupt
 */

int avalanche_intd_set_pacer_restart_mode( unsigned int host_num,
                                           unsigned int ip_int_num )
{
    int i = 0;
    if( ( host_num >= 0 && host_num < soc_info.max_num_hosts ) &&
        ( ip_int_num >= 0 && ip_int_num < INTD_MAX_PACER_REGS ) &&
        ( ip_int_num <= soc_info.host_info[i].max_ip_ints_mapped )) 
    {
        unsigned int pacers_per_host = 0,i,pacer_reg_indx = 0;  
        if(host_num >0)
        {  
            for( i = 0; i < host_num; i++)
            {
                pacers_per_host += soc_info.host_info[ i ] .max_ip_ints_mapped; 
            }
            pacer_reg_indx = ip_int_num + pacers_per_host;
        }
        else
        {
            pacer_reg_indx = ip_int_num;
        }
       avalanche_hw0_idregs[ pacer_reg_indx ].idpacer->idpparamr |=
                                          INTD_PACER_RESTART_MODE_ON;    
    }
    else
   {
        return(-1);
   }    
   return(0);      
} 


int avalanche_intd_clear_pacer_restart_mode( unsigned int host_num,
                                             unsigned int ip_int_num )
{
    int i=0;
    if( ( host_num >= 0 && host_num < soc_info.max_num_hosts ) &&
        ( ip_int_num >= 0 && ip_int_num < INTD_MAX_PACER_REGS ) &&
        ( ip_int_num <= soc_info.host_info[i].max_ip_ints_mapped )) 
    {
        unsigned int pacers_per_host = 0,i,pacer_reg_indx = 0;  
        if(host_num >0)
        {  
            for( i = 0; i < host_num; i++)
            {
                pacers_per_host += soc_info.host_info[i].max_ip_ints_mapped; 
            }
            pacer_reg_indx = ip_int_num + pacers_per_host;
        }
        else
        {
            pacer_reg_indx = ip_int_num;
        }
        avalanche_hw0_idregs[ pacer_reg_indx ].idpacer->idpparamr  &=
                                         INTD_PACER_RESTART_MODE_OFF;    
    }
    else
   {
        return(-1);
   }    
   return(0);       
}

/* For frequency-based pacing this is the maximum number of interrupts
 * per millisecond value.
 * For time-based pacing this is the time in ticks to count to.
 */

int avalanche_intd_set_pacer_max( unsigned int host_num,
                                  unsigned int ip_int_num, 
                                  unsigned int max_value )
{ 
    int i=0;
    if( ( host_num >= 0 && host_num < soc_info.max_num_hosts ) &&
         ( ip_int_num >= 0 && ip_int_num < INTD_MAX_PACER_REGS ) &&
         ( ip_int_num <= soc_info.host_info[i].max_ip_ints_mapped ) && 
         ( max_value > 0 && max_value <= INTD_PACER_FREQ_MAX_VAL))
    {
        unsigned int pacers_per_host = 0,i,pacer_reg_indx = 0;  
        if(host_num >0)
        {  
            for( i = 0 ; i < host_num ;i++)
            {
                pacers_per_host += soc_info.host_info[i].max_ip_ints_mapped; 
            }
            pacer_reg_indx = ip_int_num + pacers_per_host;
        }
        else
        {
            pacer_reg_indx = ip_int_num;
        }
        /* cleanup bits */
        avalanche_hw0_idregs[ pacer_reg_indx ].idpacer->idpparamr &= ~0xFFFF;

        avalanche_hw0_idregs[ pacer_reg_indx ].idpacer->idpparamr |= max_value;
    }
    else
    {
        return(-1);
    }    
    return(0);      
}

 
int avalanche_intd_get_pacer_max( unsigned int host_num,
                                  unsigned int ip_int_num )
{   
    int i=0;
    if( ( host_num >= 0 && host_num < soc_info.max_num_hosts ) &&
        ( ip_int_num >= 0 && ip_int_num < INTD_MAX_PACER_REGS ) &&
        ( ip_int_num <= soc_info.host_info[i].max_ip_ints_mapped )) 
   {
        unsigned int pacers_per_host = 0,i,pacer_reg_indx = 0;  
        if(host_num > 0)
        {
            for( i = 1 ; i < host_num ;i++)
            {
                pacers_per_host += soc_info.host_info[i].max_ip_ints_mapped; 
            }
            pacer_reg_indx = ip_int_num + pacers_per_host;
        }
        else
        {
            pacer_reg_indx = ip_int_num;
        }
        return ( INTD_PACER_FREQ_VAL_FLAG & 
                     avalanche_hw0_idregs[pacer_reg_indx].idpacer->idpparamr);  
    }
    else
   {
        return(-1);
   }    
   return(0);      
 }


/******************************************************************************
 * Utility functions 
 ******************************************************************************/
static inline unsigned char get_bit_position ( unsigned int ip_int_num )
{
    return (ip_int_num  % NUM_INTS_PER_REG);
}
/***************************************************************************************/



