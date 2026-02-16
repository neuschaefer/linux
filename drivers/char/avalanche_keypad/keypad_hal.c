/*
 * keypad_hal.c
 * Description:
 * Source code for Keypad Module HAL
 *
 *  Copyright (C) 2009 Texas Instruments Incorporated - http://www.ti.com/
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

#include <pal.h>

#undef KEYPAD_SW_TEST

/* Structure defining the registers of Keypad module. */
typedef struct KEYPAD_HAL_REG_tag
{
    volatile UINT32 data_in_reg;
    volatile UINT32 data_out_reg;
    volatile UINT32 pin_dir_reg;
    volatile UINT32 pin_enable_reg;
    volatile UINT32 int_mask_pol_reg;
    volatile UINT32 count_reg;
    volatile UINT32 ctrl_reg;

} KEYPAD_HAL_REG_T;

#define MAX_COL                8
#define MAX_ROW                8

typedef struct 
{
    char   col_line[MAX_COL];
    char   row_line[MAX_ROW];
    unsigned short column_map;
    unsigned short row_map;
    volatile KEYPAD_HAL_REG_T *regs;
    unsigned int   module_freq;
    unsigned int   prev_inreg;
    unsigned int   pressed_key_mask;
    unsigned int   num_row;
    unsigned int   num_column;
    unsigned int   output_row;
} KPAD_HAL_OBJ_T;




#include "keypad_hal.h"



/* KBGPIO control register */
#define KBGPIO_DEBOUNCE_CLK_DIV_MASK            0x0000000C
#define KBGPIO_DEBOUNCE_CLK_DIV_POS             2
#define KBGPIO_INTERNAL_INT_CLR_MASK            0x00000002
#define KBGPIO_DEBOUNCE_ENABLE_MASK             0x00000001
#define KBGPIO_INT_MASK                         0xFFFF
#define KBGPIO_INT_MASK_OFFSET                  16
#define MAX_DEBOUNCE_COUNT                      ((1<<16)-1)

static INT32 ti_kpad_halp_set_debounce_clk(KPAD_HAL_OBJ_T *p_kpad_hal_obj, 
		                            unsigned int debounce_time);

/* The keypad columns are configured as inputs, can occupy the GPIOs from 
 * 8 to 15. The keypad rows are configured as outputs, and can occupy the
 * GPIOs from 0 to 7.
 *
 * The column map should be of the order XX00 and the row map should be of
 * the order 00XX.
 */
 
/* TNI this mapping does not match Telogy hardware so code has been modified  */
/* to support either rows or columns as output. Configuration to match the    */ 
/* platform is a runtime option determined by the location of the row bits.   */ 
/*******************************************************************************
 * FUNCTION NAME : kpad_hal_init
 *******************************************************************************
 * DESCRIPTION   : This function initializes the keypad hardware and internal 
 *                 HAL data structures. 
 * INPUTS        : base_address : base address of the keypad module in the SoC
 *                                The base address passed should be in KSEG1
 *                 debounce_time: keypad debounce time in nano secs.
 *                 
 * RETURNS       : KEYPAD_HAL_OK on success, KEYPAD_HAL_ERROR otherwise.
 *
 ******************************************************************************/
KPAD_HAL_OBJ_T* ti_kpad_hal_init (KEYPAD_DEV_INFO_T *p_kpad_dev)
{
    UINT32 i = 0, ii=0;
    volatile KEYPAD_HAL_REG_T*  kpad_hal_p_regs;
    KPAD_HAL_OBJ_T* p_kpad_hal_obj;
    volatile unsigned int *reset_base = (unsigned int *) AVALANCHE_RESET_CONTROL_BASE;


    
    /* Put code for parameter check */ 
    if((p_kpad_dev->column_map & p_kpad_dev->row_map))
        return (NULL);

    if(PAL_osMemAlloc(0, sizeof(KPAD_HAL_OBJ_T), 0, (Ptr*)&p_kpad_hal_obj)) 
    {
        return NULL;        
    }
    /* Put the KEYPAD module out of reset and configure the KEYPAD PIN */
    *reset_base |= 1 << AVALANCHE_KEYPAD_CTRL_RESET_BIT;

    for(ii=0;ii<MAX_COL;ii++)
        p_kpad_hal_obj->col_line[ii] = 0;

    for(ii=0;ii<MAX_ROW;ii++)
        p_kpad_hal_obj->row_line[ii] = 0;                    

    p_kpad_hal_obj->column_map          = 0;
    p_kpad_hal_obj->row_map             = 0;    
    p_kpad_hal_obj->regs                = 0;   
    p_kpad_hal_obj->module_freq         = 0;
    p_kpad_hal_obj->prev_inreg          = 0;
    p_kpad_hal_obj->pressed_key_mask    = 0;
    p_kpad_hal_obj->num_row             = 0;
    p_kpad_hal_obj->num_column          = 0;
    
    
    p_kpad_hal_obj->regs          = (volatile KEYPAD_HAL_REG_T*)(p_kpad_dev->base_address);
    p_kpad_hal_obj->module_freq   = p_kpad_dev->module_freq;
    p_kpad_hal_obj->column_map    = p_kpad_dev->column_map;
    p_kpad_hal_obj->row_map       = p_kpad_dev->row_map;

    
    p_kpad_hal_obj->output_row    = 1;
    if(p_kpad_hal_obj->row_map & 0xff00)
    {
        /* if rows are on the high (input) bits then columns are outputs*/
        p_kpad_hal_obj->output_row = 0;
    }

    ti_kpad_halp_set_debounce_clk(p_kpad_hal_obj,p_kpad_dev->debounce_time);

    kpad_hal_p_regs = p_kpad_hal_obj->regs;
    
    /* Reset the internal interrupt of the keypad, the Legacy code */
    kpad_hal_p_regs->ctrl_reg |= KBGPIO_INTERNAL_INT_CLR_MASK;
    kpad_hal_p_regs->ctrl_reg &= ~KBGPIO_INTERNAL_INT_CLR_MASK; 
   
    /* Enable debounce circuit for interrupts */
    kpad_hal_p_regs->ctrl_reg |= KBGPIO_DEBOUNCE_ENABLE_MASK;
    
    /* set pin dir to out */
    p_kpad_hal_obj->regs->pin_dir_reg = 0x0; /* mark all lines as o/p */

    p_kpad_hal_obj->regs->pin_enable_reg    = 0x0; /* disable all pins */

    p_kpad_hal_obj->regs->int_mask_pol_reg  = 0x0; /* no line generates intr, low polarity */
    
    
	p_kpad_hal_obj->regs->data_out_reg   = 0;

    
    if(p_kpad_hal_obj->output_row)
        p_kpad_hal_obj->regs->pin_dir_reg|= p_kpad_hal_obj->column_map;
    else
        p_kpad_hal_obj->regs->pin_dir_reg|= p_kpad_hal_obj->row_map;
    
    { /* Let us prepare the rows in the kpad controller. */
	unsigned short row_map = p_kpad_hal_obj->row_map;
        while(row_map)
        {
            if(row_map & 1)
	        p_kpad_hal_obj->row_line[p_kpad_hal_obj->num_row++] = i; 

	    row_map >>= 1;
	    i++;
        }

    /* Enable the row pins ONLY OUTPUT pins  */
    if(p_kpad_hal_obj->output_row)
        p_kpad_hal_obj->regs->pin_enable_reg |= p_kpad_hal_obj->row_map;
    }
  
    i = 0;
    {   /* Let us prepare the columns in the kpad controller. */
	unsigned short column_map = p_kpad_hal_obj->column_map;
        /*column_map >>= 8; */ 
        while(column_map)
        {
            if(column_map & 1)
	        p_kpad_hal_obj->col_line[p_kpad_hal_obj->num_column++] = i;

	    column_map >>= 1;
	    i++;
        }

	/* mark columns as input, others by default in output direction.*/
    /* TNI switch rows to input to match our hardware */
    if(!p_kpad_hal_obj->output_row)
        p_kpad_hal_obj->regs->pin_enable_reg |= p_kpad_hal_obj->column_map;
    }

    return (p_kpad_hal_obj);
}

int ti_kpad_hal_start(KPAD_HAL_OBJ_T* p_kpad_hal_obj)
{
    volatile KEYPAD_HAL_REG_T* kpad_hal_p_regs = p_kpad_hal_obj->regs;

    /* House keeping parameters. */
    /* TNI rows as input */
    if(p_kpad_hal_obj->output_row)
        p_kpad_hal_obj->prev_inreg = p_kpad_hal_obj->column_map;
    else
        p_kpad_hal_obj->prev_inreg = p_kpad_hal_obj->row_map;
    p_kpad_hal_obj->pressed_key_mask = 0;
    
    /* Set up the bits for columns to generate interrupt. */
    /* TNI rows as input */
    if(p_kpad_hal_obj->output_row)
        kpad_hal_p_regs->int_mask_pol_reg |= p_kpad_hal_obj->column_map << KBGPIO_INT_MASK_OFFSET;
    else
        kpad_hal_p_regs->int_mask_pol_reg |= p_kpad_hal_obj->row_map << KBGPIO_INT_MASK_OFFSET; 
 
    return(0);
}

int ti_kpad_hal_stop(KPAD_HAL_OBJ_T *p_kpad_hal_obj)
{
    volatile KEYPAD_HAL_REG_T* kpad_hal_p_regs = p_kpad_hal_obj->regs;
/*  kpad_hal_p_regs->ctrl_reg &= ~KBGPIO_DEBOUNCE_ENABLE_MASK; */
    kpad_hal_p_regs->int_mask_pol_reg &= KBGPIO_INT_MASK; 

    return (0);
}

int ti_kpad_hal_cleanup(KPAD_HAL_OBJ_T *p_kpad_hal_obj)
{
    PAL_osMemFree(0, p_kpad_hal_obj, sizeof(p_kpad_hal_obj));
    return (0);
}

/*******************************************************************************
 * FUNCTION NAME : kpad_hal_key_scan
 *******************************************************************************
 * DESCRIPTION   : This function scans for a pressed key and returns the row
 *                 and column of the pressed key.                 
 *                 
 * RETURNS       : KEYPAD_HAL_OK on success, KEYPAD_HAL_ERROR otherwise.
 *BCG-SPTC 
 ******************************************************************************/
int ti_kpad_hal_key_scan(KPAD_HAL_OBJ_T* p_kpad_hal_obj)
{   
    volatile KEYPAD_HAL_REG_T* kpad_hal_p_regs = p_kpad_hal_obj->regs;
    unsigned int  temp_col, temp_row,i = 0;
    int ret_val    = -1;

#ifdef TO_GO
    if(!new_col_map = (~kpad_hal_p_regs->data_in_reg & p_kpad_hal_obj->column_map))
        return (-1) /* not hit */
#endif

    if(!p_kpad_hal_obj->pressed_key_mask)
        return (ret_val); /* not hit */

/*  TNI switch row (input) and column (output) */
    if(p_kpad_hal_obj->output_row)
    {
        /* Identify the column */
        temp_col    = p_kpad_hal_obj->pressed_key_mask;
    i = 0;
        temp_col >>= p_kpad_hal_obj->col_line[0];
        while(temp_col != 1)
    {
            temp_col >>= p_kpad_hal_obj->col_line[i+1] - p_kpad_hal_obj->col_line[i];
        i++;	
    }

    ret_val = i << 8;

        /* Identify the row */
        for(i=0; i < p_kpad_hal_obj->num_row; i++)
    {
            int wait;
	        int temp_row = (1 << p_kpad_hal_obj->row_line[i]);
            kpad_hal_p_regs->data_out_reg = temp_row;
            for(wait = 0 ; wait < 10; wait++);
	if(kpad_hal_p_regs->data_in_reg & (p_kpad_hal_obj->pressed_key_mask))
	    break;
    }

        kpad_hal_p_regs->data_out_reg &= ~p_kpad_hal_obj->row_map;
        ret_val |= i;

        if (i == p_kpad_hal_obj->num_row)
            ret_val = -1;
        
    }
    else
    {
        /* Identify the row */
        temp_row    = p_kpad_hal_obj->pressed_key_mask;
        i = 0;
        temp_row >>= p_kpad_hal_obj->row_line[0];
        while(temp_row != 1)
        {
            temp_row >>= p_kpad_hal_obj->row_line[i+1] - p_kpad_hal_obj->row_line[i];
            i++;	
        }

        ret_val = i;
    
        /* Identify the column */
        for(i=0; i < p_kpad_hal_obj->num_column; i++)
        {
            int wait;
	        int temp_col = (1 << p_kpad_hal_obj->col_line[i]);
            kpad_hal_p_regs->data_out_reg = temp_col;
            for(wait = 0 ; wait < 10; wait++);
	        if(kpad_hal_p_regs->data_in_reg & (p_kpad_hal_obj->pressed_key_mask))
	        break;
        }

        kpad_hal_p_regs->data_out_reg &= ~p_kpad_hal_obj->column_map;
        ret_val |= i << 8;


        if (i == p_kpad_hal_obj->num_column)
            ret_val = -1;
    }
    return (ret_val);  
}

/*******************************************************************************
 * FUNCTION NAME : kpad_halp_set_debounce_clock
 *******************************************************************************
 * DESCRIPTION   : Sets the debounce clock rate.
 *                 
 * RETURNS       : vbus clock divider value set
 *
 ******************************************************************************/
static INT32 ti_kpad_halp_set_debounce_clk(KPAD_HAL_OBJ_T *p_kpad_hal_obj, 
		                            unsigned int debounce_time)
{
    unsigned int div      = 32; /* the divisor */
    unsigned int val      =  3; /* <val>:<div> => 00:2048, 01:512, 02:128, 03:32 */
    unsigned int temp_div =  0;
    volatile KEYPAD_HAL_REG_T* kpad_hal_p_regs = p_kpad_hal_obj->regs;

    /* set the count field to acheive the requested debounce time. 
     * dividing by 1000 to convert the debounce time into secs from milli-secs. */
    temp_div  = (debounce_time * (p_kpad_hal_obj->module_freq/MAX_DEBOUNCE_COUNT))/1000;
    while(temp_div/div)
    { 
        val--; div*=4;
    }

    /* Put these lines under critical section, sometimes. */
    kpad_hal_p_regs->ctrl_reg &= ~KBGPIO_DEBOUNCE_CLK_DIV_MASK;
    kpad_hal_p_regs->ctrl_reg |= val << KBGPIO_DEBOUNCE_CLK_DIV_POS;
    /* dividing by 1000 to convert the debounce time into secs from milli-secs. */
    kpad_hal_p_regs->count_reg   = debounce_time*(p_kpad_hal_obj->module_freq/div)/1000;

    return val;
}

/*******************************************************************************
 * FUNCTION NAME : kpad_hal_isr
 *******************************************************************************
 * DESCRIPTION   : The keypad ISR. Resets the interrupt and calls the registered
 *                 ISR.
 *                 
 * RETURNS       : N/A
 *
 ******************************************************************************/
int ti_kpad_hal_isr(KPAD_HAL_OBJ_T *p_kpad_hal_obj)
{
    volatile KEYPAD_HAL_REG_T* kpad_hal_p_regs = p_kpad_hal_obj->regs;
/* TNI switch row, column */
    unsigned int current_inreg;
    int ret_val = 0;
    unsigned short temp_map;

    if(p_kpad_hal_obj->output_row)
        current_inreg =  kpad_hal_p_regs->data_in_reg & p_kpad_hal_obj->column_map;
    else
        current_inreg =  kpad_hal_p_regs->data_in_reg & p_kpad_hal_obj->row_map;
    /* Clear the MASK register to disable interrupts */
    kpad_hal_p_regs->int_mask_pol_reg &= KBGPIO_INT_MASK;

    if(p_kpad_hal_obj->prev_inreg == current_inreg)
    {
   	    goto key_scan_end;
    }

/* TNI switch row, column */
    if(p_kpad_hal_obj->output_row)
    {
        temp_map = p_kpad_hal_obj->column_map;
    }
    else
    {
        temp_map = p_kpad_hal_obj->row_map;
    }
    if(p_kpad_hal_obj->prev_inreg != temp_map)
    {
   	if(p_kpad_hal_obj->pressed_key_mask ==0 || 
	   p_kpad_hal_obj->pressed_key_mask & ~current_inreg)
    {
   	    goto key_scan_end;
    }
   	else
	{
	    p_kpad_hal_obj->pressed_key_mask = 0;
	    ret_val    = 2; /* key released. */
	}
    }
    else
    {
        unsigned int i =current_inreg ^ p_kpad_hal_obj->prev_inreg;
        if( i &(i-1))
        {
   	        goto key_scan_end;
        }

  	    p_kpad_hal_obj->pressed_key_mask = i;
	    ret_val        = 1; /* key pressed. */
    }

key_scan_end:

    p_kpad_hal_obj->prev_inreg = current_inreg;
    return (ret_val);
}

int ti_kpad_hal_ioctl(KPAD_HAL_OBJ_T *p_kpad_hal_obj, unsigned int cmd, 
		       unsigned int val)
{
    volatile KEYPAD_HAL_REG_T* kpad_hal_p_regs = p_kpad_hal_obj->regs;

    switch(cmd)
    {
	case TI_KEY_DEBOUNCE_VAL:
            ti_kpad_halp_set_debounce_clk(p_kpad_hal_obj,val);
	    break;

	case TI_KEY_RELEASE_DETECT: 
	{
	    /* Essentially, we are looking for low to high transition. */
            kpad_hal_p_regs->ctrl_reg |= KBGPIO_DEBOUNCE_ENABLE_MASK;

	    /* sets the polarity */
	    kpad_hal_p_regs->int_mask_pol_reg  = p_kpad_hal_obj->pressed_key_mask;
	    /* unmasks the line for intr. */
	  /*  kpad_hal_p_regs->int_mask_pol_reg |= p_kpad_hal_obj->pressed_key_mask << KBGPIO_INT_MASK_OFFSET; */

/* TNI switch row, column */
        if(p_kpad_hal_obj->output_row)
	        kpad_hal_p_regs->int_mask_pol_reg = p_kpad_hal_obj->column_map << KBGPIO_INT_MASK_OFFSET;
        else
	        kpad_hal_p_regs->int_mask_pol_reg = p_kpad_hal_obj->row_map << KBGPIO_INT_MASK_OFFSET;

        
	}
	    break;

	case TI_KEY_PRESS_DETECT:
	{
	    /* Sets the low polarity and lines for intr generation. */
/* TNI switch row, column */
        if(p_kpad_hal_obj->output_row)
            kpad_hal_p_regs->data_out_reg    &= ~(0xffff & p_kpad_hal_obj->row_map);
        else
            kpad_hal_p_regs->data_out_reg    &= ~(0xffff & p_kpad_hal_obj->column_map);
            kpad_hal_p_regs->ctrl_reg |= KBGPIO_DEBOUNCE_ENABLE_MASK;
	    /* also sets up the polarity. */

/* TNI switch row, column */
        if(p_kpad_hal_obj->output_row)
	        kpad_hal_p_regs->int_mask_pol_reg = p_kpad_hal_obj->column_map << KBGPIO_INT_MASK_OFFSET;
        else
	        kpad_hal_p_regs->int_mask_pol_reg = p_kpad_hal_obj->row_map << KBGPIO_INT_MASK_OFFSET;
	    break;
	}

	default: 
	    break;
    }
return 0;
} 



/**************************************************************************
 * TEST CODE 
 **************************************************************************/

#ifdef KEYPAD_SW_TEST
int main()
{
    kpad_hal_init((UINT32)calloc(sizeof(KEYPAD_HAL_REG_T), 1), 20000000, 8000, NULL);
    kpad_halp_print_regs();
    return 0;
}
#endif

#ifdef KEYPAD_DEBUG
void kpad_halp_print_regs()
{
    logMsg("\nDATA IN REG   : 0x%08x\n", kpad_hal_p_regs->data_in_reg,0,0,0,0,0);
    logMsg("DATA OUT REG  : 0x%08x\n", kpad_hal_p_regs->data_out_reg,0,0,0,0,0);
    logMsg("PIN DIR REG   : 0x%08x\n", kpad_hal_p_regs->pin_dir_reg,0,0,0,0,0);
    logMsg("PIN ENABLE REG: 0x%08x\n", kpad_hal_p_regs->pin_enable_reg,0,0,0,0,0);
    logMsg("MASK/POL REG  : 0x%08x\n", kpad_hal_p_regs->int_mask_pol_reg,0,0,0,0,0);
    logMsg("COUNT REG     : 0x%08x\n", kpad_hal_p_regs->count_reg,0,0,0,0,0);
    logMsg("CTRL REG      : 0x%08x\n\n", kpad_hal_p_regs->ctrl_reg,0,0,0,0,0);
}
#endif
