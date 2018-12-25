#include "disp_event.h"
#include "disp_display.h"
#include "disp_de.h"
#include "disp_video.h"
#include "disp_scaler.h"
#include "disp_clk.h"

//add by heyihang 2012-12-13
#include "disp_eink.h"

DECLARE_TASKLET(eink_tastlet, eink_refresh_wav_form_1, 0);


__s32 BSP_disp_cmd_cache(__u32 sel)
{
    gdisp.screen[sel].cache_flag = TRUE;
    return DIS_SUCCESS;
}

__s32 BSP_disp_cmd_submit(__u32 sel)
{
    gdisp.screen[sel].cache_flag = FALSE;

    return DIS_SUCCESS;
}

__s32 BSP_disp_cfg_start(__u32 sel)
{
	gdisp.screen[sel].cfg_cnt++;
	
	return DIS_SUCCESS;
}

__s32 BSP_disp_cfg_finish(__u32 sel)
{
	gdisp.screen[sel].cfg_cnt--;
	
	return DIS_SUCCESS;
}

void LCD_vbi_event_proc(__u32 sel, __u32 tcon_index)
{    
    __u32 cur_line = 0, start_delay = 0;
    __u32 i = 0;

//	if(BSP_disp_get_output_type(sel) == DISP_OUTPUT_TYPE_NONE)
//		return;
	
	Video_Operation_In_Vblanking(sel, tcon_index);


    cur_line = LCDC_get_cur_line(sel, tcon_index);
    start_delay = LCDC_get_start_delay(sel, tcon_index);
    if(cur_line > start_delay-3)
	{
	      //DE_INF("cur_line(%d) >= start_delay(%d)-3 in LCD_vbi_event_proc\n", cur_line, start_delay);
		return ;
	}

	IEP_Operation_In_Vblanking(sel, tcon_index);
		
    if(gdisp.screen[sel].LCD_CPUIF_ISR)
    {
    	(*gdisp.screen[sel].LCD_CPUIF_ISR)();
    }

    if(gdisp.screen[sel].cache_flag == FALSE && gdisp.screen[sel].cfg_cnt == 0)
    {
        for(i=0; i<2; i++)
        {
            if((gdisp.scaler[i].status & SCALER_USED) && (gdisp.scaler[i].screen_index == sel))
            {
                DE_SCAL_Set_Reg_Rdy(i);
                //DE_SCAL_Reset(i);
                //DE_SCAL_Start(i);
                gdisp.scaler[i].b_reg_change = FALSE;
            }
            if(gdisp.scaler[i].b_close == TRUE)
            {
                Scaler_close(i);
                gdisp.scaler[i].b_close = FALSE;
            }
            if((gdisp.scaler[i].status & SCALER_USED) && (gdisp.scaler[i].screen_index == sel) && (gdisp.scaler[i].coef_change == TRUE))
            {
            	__scal_src_size_t in_size;
            	__scal_out_size_t out_size;
            	__scal_src_type_t in_type;
            	__scal_out_type_t out_type;
            	__scal_scan_mod_t in_scan;
            	__scal_scan_mod_t out_scan;
                __disp_scaler_t * scaler;

                scaler = &(gdisp.scaler[i]);

            	in_scan.field = FALSE;
            	in_scan.bottom = FALSE;

            	in_type.fmt= Scaler_sw_para_to_reg(0,scaler->in_fb.format);
            	in_type.mod= Scaler_sw_para_to_reg(1,scaler->in_fb.mode);
            	in_type.ps= Scaler_sw_para_to_reg(2,(__u8)scaler->in_fb.seq);
            	in_type.byte_seq = 0;
            	in_type.sample_method = 0;

            	in_size.src_width = scaler->in_fb.size.width;
            	in_size.src_height = scaler->in_fb.size.height;
            	in_size.x_off = scaler->src_win.x;
            	in_size.y_off = scaler->src_win.y;
            	in_size.scal_width = scaler->src_win.width;
            	in_size.scal_height = scaler->src_win.height;

            	out_scan.field = (gdisp.screen[sel].iep_status & DE_FLICKER_USED)?FALSE: gdisp.screen[sel].b_out_interlace;

            	out_type.byte_seq = scaler->out_fb.seq;
            	out_type.fmt = scaler->out_fb.format;

            	out_size.width = scaler->out_size.width;
            	out_size.height = scaler->out_size.height;
                //__inf("vint\n");
                DE_SCAL_Set_Scaling_Coef(i, &in_scan, &in_size, &in_type, &out_scan, &out_size, &out_type, scaler->smooth_mode);

                gdisp.scaler[i].coef_change = FALSE;
            }
        }
        DE_BE_Cfg_Ready(sel);
		gdisp.screen[sel].have_cfg_reg = TRUE;
    }

#if 0
    cur_line = LCDC_get_cur_line(sel, tcon_index);
    
	if(cur_line > 5)
	{
    	DE_INF("%d\n", cur_line);
    }
#endif

    return ;
}

void LCD_line_event_proc(__u32 sel)
{    
	if(gdisp.screen[sel].have_cfg_reg)
	{   
	    gdisp.init_para.disp_int_process(sel);
	    gdisp.screen[sel].have_cfg_reg = FALSE;

        //del by heyihang.2012-12-13
        #if 0
        if(gdisp.screen[sel].b_screen_finished == 1 && (&gdisp.screen[sel].screen_queue != NULL))
        {
            gdisp.screen[sel].b_screen_finished = 2;
            wake_up_interruptible(&(gdisp.screen[sel].screen_queue));
        }
        #else
        //add by heyihang.2012-12-13
        tasklet_hi_schedule(&eink_tastlet);
        #endif
	}
}

