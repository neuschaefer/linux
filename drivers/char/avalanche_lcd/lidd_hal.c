/*
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

/**************************************************************************
 * FILE PURPOSE :   HAL code for LCD LIDD controller.
 **************************************************************************
 * FILE NAME    :   lidd_hal.c
 *
 * DESCRIPTION  :
 *  HAL code for LCD LIDD controller.
 *
 *************************************************************************/
typedef void TI_HD47780_T;




typedef struct 
{
    volatile unsigned int config;
    volatile unsigned int addr;
    volatile unsigned int data;
} TI_LIDD_REGS_T;

typedef struct 
{
    volatile unsigned int   revision;
    volatile unsigned int   cntl;
    volatile unsigned int   status;
    volatile unsigned int   lidd_cntl;
    volatile TI_LIDD_REGS_T cs[2];
    volatile unsigned int   raster_cntl;
    volatile unsigned int   raster_timing_0;
    volatile unsigned int   raster_timing_1;
    volatile unsigned int   raster_timing_2;
    volatile unsigned int   subpanel_disp;
    volatile unsigned int   reserved;
    volatile unsigned int   dma_cntl;
    volatile unsigned int   frame0_base_addr;
    volatile unsigned int   frame0_ceiling;
    volatile unsigned int   frame1_base_addr;
    volatile unsigned int   frame1_ceiling;

} TI_LCD_CNTL_REGS_T; 

typedef struct
{
    unsigned char     num_lcd_inst;
    TI_HD47780_T*      lcd_inst[2];
    TI_LCD_CNTL_REGS_T*       regs;
    unsigned int       active_inst;
    unsigned int          disp_row;
    unsigned int          disp_col;
    unsigned int         line_wrap;
    unsigned int      cursor_state;
    unsigned int      cursor_shift;
    unsigned int               row;
    unsigned int               col;

} LIDD_HAL_OBJ_T;


#include "hd44780_hal.h"
#include "haltypes.h"
#include "lidd_hal.h"
#include <pal_os.h>


static int ti_lidd_p_set_to_xy(LIDD_HAL_OBJ_T*, int, int);
static int ti_lidd_p_update_properties(LIDD_HAL_OBJ_T*);
static int ti_lidd_p_lwrap_cursor_move(LIDD_HAL_OBJ_T *p, unsigned int direction);
static int ti_lidd_p_wr_or_rd(LIDD_HAL_OBJ_T *p, unsigned int cmd, unsigned int data);



/*-----------------------------------------------------------------------------
 * Updates the properties of the LCD. 
 *
 * Returns 0 on success or -1 on failure.
 *---------------------------------------------------------------------------*/
int ti_lidd_p_update_properties(LIDD_HAL_OBJ_T *p)
{
    unsigned int passive_inst = !p->active_inst;

    if(p->num_lcd_inst > 1)
/*      ti_hd47780_ioctl(p->lcd_inst + passive_inst, TI_LIDD_CURSOR_STATE, 0); */
        ti_hd47780_ioctl(p->lcd_inst[passive_inst], TI_LIDD_CURSOR_STATE , p->cursor_state);

/*  ti_hd47780_ioctl(p->lcd_inst + p->active_inst, TI_LIDD_CURSOR_STATE , p->cursor_state); */
    ti_hd47780_ioctl(p->lcd_inst[p->active_inst], TI_LIDD_CURSOR_STATE , p->cursor_state);

    return(0);
}

/*---------------------------------------------------------
 * Sets the cursor at the specified location.
 * x is the column, y is the row. 
 *
 * Returns 0 on success otherwise -1.
 *-------------------------------------------------------*/
int ti_lidd_p_set_to_xy(LIDD_HAL_OBJ_T *p, int x, int y)
{
    int update = 0;

    if(p->line_wrap)
    {
	    /* wrap the row and col if required. */
        p->col  = x = x % p->disp_col;
	    p->row  = y = y % p->disp_row; 
    }

    /* Figure out if this causes a moving to lcd 0 to lcd 1. */
    if((p->num_lcd_inst == 2) && (y > p->disp_row/2 - 1))
    {
        y = y%2;

	if(p->active_inst == 0)
	    update = 1;

        p->active_inst = 1;
    }
    else
    {
        /* Figure out if this causes a moving to lcd 1 to lcd 0. */
        if(p->active_inst == 1)
	    update = 1;

        p->active_inst = 0;
    }
       
    if(update) ti_lidd_p_update_properties(p);

    ti_hd47780_ioctl(p->lcd_inst[p->active_inst], TI_LIDD_GOTO_XY, x << 8 | y);

    return (0);
}

/*-----------------------------------------------------------------------------
 * Wraps a line i.e. when the last of column of the row is reached, the cursor
 * is moved to the next row, first col. When the last row and last col is reached 
 * the cursor is moved to the first row. 
 *
 * Returns: 0 if now wrap was done. 1, if a wrap was carried out.   
 *---------------------------------------------------------------------------*/
int ti_lidd_p_lwrap_cursor_move(LIDD_HAL_OBJ_T *p, unsigned int direction)
{   
    int ret_val = 0;

    if(direction) /* move right. */
    {
        p->col = (++p->col) % (p->disp_col);
        if(!p->col) 
        {
    	    /* line wrapped. */
    	    p->row = (++p->row) % p->disp_row;
            

    	    /* Indicate the lcd of the line wrapping. */
            ti_lidd_p_set_to_xy(p, p->col, p->row);

            

    	    ret_val = 1;
	    }
    }
    else     /* move left. */
    {
    	if(!p->col)
    	{
    	    /* line wrapped. */
    	    p->col = p->disp_col - 1;
    	    p->row = (!p->row) ? p->disp_row - 1 : --p->row;

    	    /* Indicate the lcd of the line wrapping. */
                ti_lidd_p_set_to_xy(p, p->col, p->row);

            
    	    ret_val = 1;
    	}
	    else
	    {
	        p->col--;
	    }
    }
    return(ret_val);
}

/*-----------------------------------------------------------------------------
 * Writes or reads a character to/from LCD.
 *
 * Returns 0 on success or -1 on failure. 
 *---------------------------------------------------------------------------*/
int ti_lidd_p_wr_or_rd(LIDD_HAL_OBJ_T *p, unsigned int cmd, unsigned int data)
{

    /* Write the character on the active hd47780 instance. */
    ti_hd47780_ioctl(p->lcd_inst[p->active_inst], cmd, data);

    /* Update the rows and columns for the next read or write, only if enabled
     * for line wrap. */
    if(p->line_wrap)
        ti_lidd_p_lwrap_cursor_move(p, p->cursor_shift);

    return (0);
}



/* LCD controller configuration */

#define LCD_LIDD_MCKL_FREQ                          6250 /* specify in KHz */

#define LCD_LIDD_P0_RS_SETUP_CYC     1
#define LCD_LIDD_P0_RS_HOLD_CYC      1
#define LCD_LIDD_P0_RS_DUR_CYC       4

#define LCD_LIDD_P1_RS_SETUP_CYC     1
#define LCD_LIDD_P1_RS_HOLD_CYC      1
#define LCD_LIDD_P1_RS_DUR_CYC       4

#define LCD_LIDD_P0_WS_SETUP_CYC     1
#define LCD_LIDD_P0_WS_HOLD_CYC      1
#define LCD_LIDD_P0_WS_DUR_CYC       4

#define LCD_LIDD_P1_WS_SETUP_CYC     1       
#define LCD_LIDD_P1_WS_HOLD_CYC      1
#define LCD_LIDD_P1_WS_DUR_CYC       4


#define LCD_CLK_DIVISOR(x)          ((x) << 8)
#define LCD_CLK_DIVISOR_MASK        LCD_CLK_DIVISOR(0xFF)
#define LCD_MODE_SEL_MASK           0x01

#define LCD_END_OF_FRAME1           (1 << 9)
#define LCD_END_OF_FRAME0           (1 << 8)
#define LCD_PALETTE_LOADED          (1 << 6)
#define LCD_FIFO_UNDERFLOW          (1 << 5)
#define LCD_AC_BIAS_COUNT_STATUS    (1 << 3)
#define LCD_SYNC_LOST               (1 << 2)
#define LCD_FRAME_DONE              (1 << 0)


#define LCD_CS1_E1_POL_INV          0x80
#define LCD_CS0_E0_POL_INV          0x40
#define LCD_WE_RW_POL_INV           0x20
#define LCD_OE_E_POL_INV            0x10
#define LCD_ALE_POL_INV             0x08
#define LCD_LIDD_MODE(x)            ((x) << 0)

#define LCD_HITACHI_MODE            0x04
#define LCD_MPU80_ASYNC_MODE        0x03
#define LCD_MPU80_SYNC_MODE         0x02
#define LCD_MPU68_ASYNC_MODE        0x01
#define LCD_MPU68_SYNC_MODE         0x00

#define LCD_LIDD_MODE_MASK          LCD_LIDD_MODE(0x07)

#define LCD_WS_SETUP(x)             ((x) << 27)
#define LCD_WS_WIDTH(x)             ((x) << 21)
#define LCD_WS_HOLD(x)              ((x) << 17)
#define LCD_RS_SETUP(x)             ((x) << 12)
#define LCD_RS_WIDTH(x)             ((x) << 6)
#define LCD_RS_HOLD(x)              ((x) << 2)
#define LCD_LIDD_DELAY(x)           ((x) << 0)

#define LCD_WS_SETUP_MASK           LCD_WS_SETUP(0x1F)
#define LCD_WS_WIDTH_MASK           LCD_WS_WIDTH(0x3F)
#define LCD_WS_HOLD_MASK            LCD_WS_HOLD(0x0F)
#define LCD_RS_SETUP_MASK           LCD_RS_SETUP(0x1F)
#define LCD_RS_WIDTH_MASK           LCD_RS_WIDTH(0x3F)
#define LCD_RS_HOLD_MASK            LCD_RS_HOLD(0x0F)

#define LCD_LIDD_DELAY_MASK         LCD_LIDD_DELAY(0x03)


#define LCD_PASSIVE_565_MODE        (1 << 24)
#define LCD_ACTIVE_ALT_MAP_MODE     (1 << 23)
#define LCD_NIBBLE_MODE             (1 << 22)
#define LCD_PALETTE_LOAD_MODE(x)    ((x) << 20)
#define PALETTE_AND_DATA            0x00
#define PALETTE_ONLY                0x01
#define DATA_ONLY                   0x02
#define LCD_FIFO_DMA_DELAY_REQ(x)   ((x) << 12)
#define LCD_MONO_8BIT_MODE          (1 << 9)
#define LCD_RASTER_DATA_ORDER       (1 << 8)
#define LCD_TFT_MODE                (1 << 7)
#define LCD_UNDERFLOW_INT_ENA       (1 << 6)
#define LCD_SYNC_LOST_INT_ENA       (1 << 5)
#define LCD_LOAD_DONE_INT_ENA       (1 << 4)
#define LCD_FRAME_DONE_INT_ENA      (1 << 3)
#define LCD_AC_BIAS_INT_ENA         (1 << 2)
#define LCD_MONOCHROME_MODE         (1 << 1)
#define LCD_RASTER_ENABLE           (1 << 0)


#define LCD_HSYNC_BACK_PORCH(x)     ((x) << 24)
#define LCD_HSYNC_FRONT_PORCH(x)    ((x) << 16)
#define LCD_HSYNC_PULSE_WIDTH(x)    ((x) << 10)
#define LCD_PIXELS_PER_LINE(x)      ((((x) / 16) - 1) << 4)


#define LCD_VSYNC_BACK_PORCH(x)     ((x) << 24)
#define LCD_VSYNC_FRONT_PORCH(x)    ((x) << 16)
#define LCD_VSYNC_PULSE_WIDTH(x)    ((x) << 10)
#define LCD_LINES_PER_PANEL(x)      (((x) - 1) << 0)


#define LCD_SYNC_PIXEL_CLK_CTRL     (1 << 25)
#define LCD_SYNC_FALLING_EDGE       (1 << 24)
#define LCD_INVERT_OE               (1 << 23)
#define LCD_INVERT_PIXEL_CLK        (1 << 22)
#define LCD_INVERT_HSYNC            (1 << 21)
#define LCD_INVERT_VSYNC            (1 << 20)
#define LCD_AC_BIAS_TRANSITIONS_PER_INT(x)      ((x) << 16)
#define LCD_AC_BIAS_FREQUENCY(x)    ((x) << 8)


#define LCD_SUBPANEL_ENABLE         (1 << 31)
#define LCD_SUBPANEL_HIGH_OR_LOW    (1 << 29)
#define LCD_SUBPANEL_THRESHOLD(x)    ((x) << 16)
#define LCD_DEFAULT_PIXEL_DATA(x)   ((x) << 4)


#define LCD_DMA_BURST_SIZE(x)       ((x) << 4)
#define LCD_DMA_BURST_1             0x0
#define LCD_DMA_BURST_2             0x1
#define LCD_DMA_BURST_4             0x2
#define LCD_DMA_BURST_8             0x3
#define LCD_DMA_BURST_16            0x4
#define LCD_END_OF_FRAME_INT_ENA    (1 << 2)
#define LCD_DMA_BIG_ENDIAN          (1 << 1)
#define LCD_DUAL_FRAME_BUFFER_ENABLE  (1 << 0)


LIDD_HAL_OBJ_T* ti_lidd_hal_init(TI_LIDD_INFO_T* p_lidd_info)
{
    static int g_lcd_inst = 0;
    LIDD_HAL_OBJ_T *p = NULL;        
    TI_LCD_CNTL_REGS_T *p_regs = NULL;
    
    int i;

    /* Initialize the controller, if not done already. */
    if(PAL_osMemAlloc(0, sizeof(LIDD_HAL_OBJ_T), 0, ( void *)&p)  != PAL_SOK) {
        return (NULL);
    }

    PAL_osMemSet(p, 0, sizeof(LIDD_HAL_OBJ_T));

    p->regs = (TI_LCD_CNTL_REGS_T*) p_lidd_info->base_addr;

    if(g_lcd_inst == 2) return (NULL);

    if(g_lcd_inst == 0)
    {
        p_regs              = p->regs;

	    p_regs->cntl       &= ~1;                  /* Sets it in the HD47780 mode. */
        p_regs->cntl       &= ~(LCD_CLK_DIVISOR_MASK);
        p_regs->cntl       |=  LCD_CLK_DIVISOR(10); /* modify the code here for 
					      instance where mclk is required. */
        p_regs->cs[0].config |=  LCD_RS_SETUP(LCD_LIDD_P0_RS_SETUP_CYC) 
		               | LCD_RS_WIDTH(LCD_LIDD_P0_RS_DUR_CYC) 
		               | LCD_RS_HOLD(LCD_LIDD_P0_RS_HOLD_CYC);
        p_regs->cs[1].config |=  LCD_RS_SETUP(LCD_LIDD_P1_RS_SETUP_CYC) 
		               | LCD_RS_WIDTH(LCD_LIDD_P1_RS_DUR_CYC) 
			       | LCD_RS_HOLD(LCD_LIDD_P1_RS_HOLD_CYC);
        p_regs->cs[0].config |=  LCD_WS_SETUP(LCD_LIDD_P0_WS_SETUP_CYC) 
		               | LCD_WS_WIDTH(LCD_LIDD_P0_WS_DUR_CYC) 
			       | LCD_WS_HOLD(LCD_LIDD_P0_WS_HOLD_CYC);
        p_regs->cs[1].config |=  LCD_WS_SETUP(LCD_LIDD_P1_WS_SETUP_CYC) 
		               | LCD_WS_WIDTH(LCD_LIDD_P1_WS_DUR_CYC) 
			       | LCD_WS_HOLD(LCD_LIDD_P1_WS_HOLD_CYC);

    }

    p->active_inst  = 0;
    p->num_lcd_inst = p_lidd_info->num_lcd;
    p->disp_row     = p_lidd_info->disp_row;
    p->disp_col     = p_lidd_info->disp_col;
    p->cursor_state = p_lidd_info->cursor_show;
    p->line_wrap    = p_lidd_info->line_wrap;
    p->row          = 0;
    p->col          = 0;
    p->cursor_shift = RIGHT;
   
    if(p_lidd_info->lcd_type == 4)
    {
	p_regs->lidd_cntl &= 0xfffffff8;
	p_regs->lidd_cntl |= 0x4;

    for(i = 0; i < p->num_lcd_inst; i++)
	{
            if(!(p->lcd_inst[i] = ti_hd47780_init((unsigned int*)(&p_regs->cs[i + g_lcd_inst].addr), 
			                                      (unsigned int*) &p_regs->cs[i + g_lcd_inst].data, 
		                                          (p->disp_row/p->num_lcd_inst), 
		                                          (p->disp_col/p->num_lcd_inst),
		                                           p_lidd_info->cpu_freq)))
	        goto hd47780_init_error;

	    ti_hd47780_ioctl(p->lcd_inst[i], TI_LIDD_CURSOR_SHIFT, p->cursor_shift);
        ti_hd47780_ioctl(p->lcd_inst[i], TI_LIDD_DISPLAY, ON);
	    ti_hd47780_ioctl(p->lcd_inst[i], TI_LIDD_CURSOR_STATE,  (p->cursor_state && i==0) ? ON : OFF);
	    ti_hd47780_ioctl(p->lcd_inst[i], TI_LIDD_DISPLAY_MOVE, OFF);
	    ti_hd47780_ioctl(p->lcd_inst[i], TI_LIDD_BLINK, (p_lidd_info->cursor_blink && i==0) ? ON: OFF);
	    ti_hd47780_ioctl(p->lcd_inst[i], TI_LIDD_CLEAR_SCREEN, 0);


        }
    }

    g_lcd_inst += p->num_lcd_inst;

    return (p);

hd47780_init_error: 
    while(i)
    {
        ti_hd47780_cleanup(p->lcd_inst + i);
	p->num_lcd_inst--;
    }

    PAL_osMemFree(0, p, sizeof(LIDD_HAL_OBJ_T));

    return (NULL);

}

int ti_lidd_hal_ioctl(LIDD_HAL_OBJ_T *p, unsigned int cmd, unsigned int val)
{
    switch(cmd)
    {
        case TI_LIDD_CLEAR_SCREEN:
	    ti_hd47780_ioctl(p->lcd_inst[0], cmd, val);
	    if(p->num_lcd_inst > 1)
	        ti_hd47780_ioctl(p->lcd_inst[1], cmd, val);
	    p->row  = 0;
	    p->col  = 0;
	    break;

	case TI_LIDD_LINE_WRAP:
	    p->line_wrap = val ? 1 : 0;
	    if(p->line_wrap)
            { 
	        p->active_inst         = 0;
	        ti_lidd_p_update_properties(p);

		/* disable display shift */
	        if(p->num_lcd_inst > 1)
	            ti_hd47780_ioctl(p->lcd_inst[1], TI_LIDD_DISPLAY_MOVE, val);
	        ti_hd47780_ioctl(p->lcd_inst[0], TI_LIDD_DISPLAY_MOVE, val);
	    }
	    break;

	case TI_LIDD_CURSOR_HOME:
	    if(p->num_lcd_inst > 1)
	    {
		    ti_hd47780_ioctl(p->lcd_inst[1], cmd, 0);
	        ti_hd47780_ioctl(p->lcd_inst[1], TI_LIDD_CURSOR_STATE, 0);
	    }
	    ti_hd47780_ioctl(p->lcd_inst[0], cmd, 0);
	    p->row  = 0;
	    p->col  = 0;
	    break;

	case TI_LIDD_DISPLAY:
	    if(p->num_lcd_inst > 1)
	        ti_hd47780_ioctl(p->lcd_inst[1], cmd, val);
	    ti_hd47780_ioctl(p->lcd_inst[0], cmd, val);
	    break;

	case TI_LIDD_GOTO_XY:
            ti_lidd_p_set_to_xy(p, (val & 0xff00) >> 8 , val & 0xff);
	    break;

	case TI_LIDD_BLINK:
	    if(p->num_lcd_inst > 1)
	        ti_hd47780_ioctl(p->lcd_inst[1], cmd, val);
	    ti_hd47780_ioctl(p->lcd_inst[0], cmd, val);
	    break;

	case TI_LIDD_CURSOR_STATE:
	    ti_hd47780_ioctl(p->lcd_inst[p->active_inst], cmd, val);
	    p->cursor_state = val ? 1 : 0;
	    break;

	case TI_LIDD_CURSOR_SHIFT:
	    if(p->num_lcd_inst > 1)
	        ti_hd47780_ioctl(p->lcd_inst[1], cmd, val);
	    ti_hd47780_ioctl(p->lcd_inst[0], cmd, val);
	    p->cursor_shift = (val) ? 1:0;
	    break;

	case TI_LIDD_CURSOR_MOVE:
	    {
	        int action = 1;

	        if(p->line_wrap && ti_lidd_p_lwrap_cursor_move(p, val))
	            action = 0;

	        if(action)
	            ti_hd47780_ioctl(p->lcd_inst[p->active_inst], cmd, val);
	    }
	    break;

	case TI_LIDD_DISPLAY_SHIFT:
	    if(p->num_lcd_inst > 1)
	        ti_hd47780_ioctl(p->lcd_inst[1], cmd, val);
	    ti_hd47780_ioctl(p->lcd_inst[0], cmd, val);
	    p->line_wrap = 0; /* p->row = 0; p->col = 0; */
	    break;

	case TI_LIDD_DISPLAY_MOVE:
	    ti_hd47780_ioctl(p->lcd_inst[p->active_inst], cmd, val);
	    break;

	case TI_LIDD_WR_CHAR:
	case TI_LIDD_RD_CHAR:
            ti_lidd_p_wr_or_rd(p, cmd, val);
	    break;

	case TI_LIDD_RD_CMD:
	    ti_hd47780_ioctl(p->lcd_inst[p->active_inst], cmd, val);
	    break;
	
	    
	default:
	    break;
        
    }

    return(0);
}

int ti_lidd_hal_open(LIDD_HAL_OBJ_T *p)
{
    return (0);
}

int ti_lidd_hal_close(LIDD_HAL_OBJ_T *p)
{
    return (0);
}

int ti_lidd_hal_write(LIDD_HAL_OBJ_T *p, char *data, unsigned int len)
{
    int i = 0;
    for(i= 0; i< len; i++)
    {
        /* Ignore NULLs and NLs */
        if( !data[ i ] || data[ i ] == 0xa )
           continue;

        if(ti_lidd_p_wr_or_rd(p, TI_LIDD_WR_CHAR, data[i]))
	    break;
       /* taskDelay(1); */
    }
    return(i);
}

int ti_lidd_hal_read(LIDD_HAL_OBJ_T *p, char *data, unsigned int len)
{
    int i = 0;
    for (i= 0; i< len; i++)
    {
        if(ti_lidd_p_wr_or_rd(p, TI_LIDD_RD_CHAR, (unsigned int)&(data[i])))
	    break;
     
    } 
    return(i);
}

int ti_lidd_hal_cleanup(LIDD_HAL_OBJ_T *p)
{
    while(p->num_lcd_inst--)
        ti_hd47780_cleanup(p->lcd_inst[p->num_lcd_inst]);

    PAL_osMemFree(0, p, sizeof(LIDD_HAL_OBJ_T));

    return(0);
}
