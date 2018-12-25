#include "disp_eink.h"
#include <linux/cpufreq.h>
Disp_eink_t disp_eink;

static void eink_init_fb_para(__disp_fb_create_para_t *fb_create_para, 
                               __u32 width, 
                               __u32 height,
                               __u32 buffer_num)
{
    memset(fb_create_para, 0, sizeof(__disp_fb_create_para_t));
    
    fb_create_para->fb_mode         = FB_MODE_SCREEN0;
    fb_create_para->mode            = DISP_LAYER_WORK_MODE_NORMAL;
    fb_create_para->buffer_num      = buffer_num;
    fb_create_para->width           = width;
    fb_create_para->height          = height;
    fb_create_para->output_width    = width;
    fb_create_para->output_height   = height;
}

static __u32* eink_load_waveform(char * path)
{
    ES_FILE *fp = NULL;
    __u32 *wav_buf = NULL;
    __s32 file_len = 0, read_len = 0;    
    mm_segment_t fs;
    loff_t pos;
    __u32 wf_data_len = 0;           //waveform数据总长度
    
    fp = filp_open(path, O_RDONLY, 0);
	if(IS_ERR(fp))
	{
		__EINK_WRN("open gc16 waveform failed! (path %s)", path);
		return NULL;
	}	
    else
    {
    	__EINK_WRN("open gc16 waveform successfully!\n");
    }
    
    fs = get_fs();
    set_fs(KERNEL_DS);
    pos = 0;
    
    file_len = fp->f_dentry->d_inode->i_size;
   
    wf_data_len = file_len+1023;
	wav_buf = vmalloc(wf_data_len);
    __EINK_MSG("eink_load_waveform: wav_buf = %x\n", wav_buf);
    if(!wav_buf)
    {        
        __EINK_WRN("palloc failed for eink gc16 waveform...\n");
        goto error;
    }
    
    read_len = vfs_read(fp, (char *)wav_buf, file_len, &pos);
    if(read_len != file_len)
    {
        __EINK_WRN("fread gc16 waveform fail...\n");        
        goto error;
    }
    if(fp)  
    {
        filp_close(fp, NULL);        
        set_fs(fs);
    }
    return wav_buf;
    
error:
    if(wav_buf != NULL)
    {
        vfree(wav_buf);
    }
    if(fp)  
    {
        filp_close(fp, NULL);        
        set_fs(fs);
    }    
    __EINK_MSG("error in load gc16 waveform\n");
    return NULL;
}

void eink_loadWaveFormData(void) 
{
	if (disp_eink.wav_du_data == NULL) disp_eink.wav_du_data = eink_load_waveform(EINK_WAV_DU_PATH);
	if (disp_eink.wav_short_du_data == NULL) disp_eink.wav_short_du_data = eink_load_waveform(EINK_WAV_SHORT_DU_PATH);
	if (disp_eink.wav_short_gc16_local_data == NULL) disp_eink.wav_short_gc16_local_data = eink_load_waveform(EINK_WAV_SHORT_GC16_LOCAL_PATH);
	if (disp_eink.wav_gc16_data == NULL) disp_eink.wav_gc16_data = eink_load_waveform(EINK_WAV_GC16_PATH);
	if (disp_eink.wav_common_data == NULL) disp_eink.wav_common_data = eink_load_waveform(EINK_WAV_COMMON_PATH);
}

static __s32 eink_gc16_update_waveform(__u32* p_wf_file)
{
    __u32 frame_id = 0, temperature = 26, temp_index = 0;
    register __u32* point = NULL;
    register __u8* p_wf_index = NULL;
              
    register const __u8* p_wf_data = NULL;
    register __u32 row, col = 0, data = 0, xmask;

    register __u32 pixel1 = 0, pixel2 = 0, pixel3 = 0, pixel4 = 0;
    
    __u32 *input_wf_file = p_wf_file;
    __s32 buf_index, wav_compose_index;

    buf_index = disp_eink.bufQueue.curBufIndex;
    wav_compose_index = disp_eink.bufQueue.slots[buf_index].wav_compose_index;

    if (wav_compose_index == 0)
    {
        temperature = disp_eink.temperature;
        if(temperature < 0) temperature = 0;
        if(temperature >= T_MAX) temperature = T_MAX - 1;
        
        disp_eink.wf_len_divider = *input_wf_file;
        input_wf_file++;
            
	__u32* ptemp = input_wf_file + temperature;		
        __u16* pFrame = (__u16*)((__u32)input_wf_file + *ptemp);
        disp_eink.bufQueue.slots[buf_index].total_frame = *pFrame; 
        
        pFrame++;
    	disp_eink.pframe_data = (__s32*)pFrame;                  
        __EINK_LOG("eink_gc16_update_waveform: total_frame = %d\n", disp_eink.bufQueue.slots[buf_index].total_frame);
    }

    if (wav_compose_index < disp_eink.bufQueue.slots[buf_index].total_frame)
    {   
	frame_id = disp_eink.bufQueue.slots[buf_index].wav_compose_index;
	p_wf_data = (__u8*)disp_eink.pframe_data;
    	temp_index = disp_eink.bufQueue.slots[buf_index].wav_compose_index % EINK_WAV_BUFFER_NUM;
        p_wf_index = disp_eink.wav_form_index;
        
        for (row = EINK_FSL+EINK_FBL; row < (EINK_LCD_H-EINK_FEL); row++)
        {
            point = (__u32 *)disp_eink.wav_buffer[temp_index].wav_address + (row * disp_eink.wav_width + EINK_LSL + EINK_LBL);
            xmask = (SPV | CKV | XOE | MODE | DMASK);

            for(col=0; col<EINK_LDL; col++)
            {	
				pixel1 = *(p_wf_data + ((*p_wf_index<< disp_eink.wf_len_divider) + frame_id));
				p_wf_index++;
				pixel2 = *(p_wf_data + ((*p_wf_index<< disp_eink.wf_len_divider) + frame_id));
				p_wf_index++;
				pixel3 = *(p_wf_data + ((*p_wf_index<< disp_eink.wf_len_divider) + frame_id));
				p_wf_index++;
				pixel4 = *(p_wf_data + ((*p_wf_index<< disp_eink.wf_len_divider) + frame_id));
				p_wf_index++;  
                
				data = pixel1<<11;               //D12, D11
				
				data |= (pixel2&0x02)<<9;        //D10
				data |= (pixel2&0x01)<<7;        //D7
				
				data |= pixel3<<5;               //D6,D5
				data |= pixel4<<3;               //D4,D3                                               
				
                                *point++ = (xmask | data);
                                if (col == 4) xmask |= XSTL;
            }
        } 

        disp_eink.wav_buffer[temp_index].wav_index = wav_compose_index;
        disp_eink.bufQueue.slots[buf_index].wav_compose_index++;
    }
    else
    {
    	return -1;
    }

    return 0;
}

static __s32 eink_unload_waveform(__u32 * wav_buf)
{
    if(wav_buf != NULL)
    {
        vfree(wav_buf);        		
    }  
    return 0;
}

static __s32 eink_gc16_update_waveindex(void)
{
    register int col = 0, row = 0, width = 0, height = 0;
    register __u8 index = 0;
    register __u8* p_new_data;			//新显示数据
    register __u8* p_old_data = disp_eink.wav_last_frame; 					//前一次显示的数据
    register __u8* p_wf_index = disp_eink.wav_form_index;					//波形文件索引号    
    register __u8 new = 0,old = 0;    

	p_new_data = disp_eink.bufQueue.slots[disp_eink.bufQueue.curBufIndex].buffer;
	__eink_update_mode update_mode = disp_eink.bufQueue.slots[disp_eink.bufQueue.curBufIndex].flushMode;
	
    width = disp_eink.frame_width;
    height = disp_eink.frame_height;
    __EINK_LOG("width = %d, height = %d\n", width, height);
    
    for(row =0; row < height; row++)
    {
        for(col = 0; col < width; col++)
        {   
            new = *p_new_data>>4;                //8bpp -> 4bpp
            old = *p_old_data>>4;  
    
			/*
			 *20130402,yuanlian
			 *currently that short gc16 local update mode caused by keboard,
			 *it maybe lost some letters or got any relict black pixels.
			 *need to reset all the region to white for compare if found same pixels.
			 * */
			if((update_mode==EINK_SHORT_GC16_LOCAL_MODE) && (new==old)){
				old = 0xff >> 4;
			}

            index = ((old << 4) + new);
            *p_wf_index++ = index;                
            *p_old_data++ = *p_new_data++;
        }
    }     
    __EINK_LOG("----------index---------\n");    
    
    return 0;
}

__s32 eink_du_update_local_waveindex(void)
{
    int col = 0, row = 0, width = 0, height = 0;
    __u8 index = 0;
    __u8* p_new_data;			//新显示数据
    __u8* p_old_data = disp_eink.wav_last_frame; 					//前一次显示的数据
    __u8* p_wf_index = disp_eink.wav_form_index;					//波形文件索引号    
    __u8 new = 0,old = 0;   
    __u8 eink_type = OPM060A1;

	p_new_data = disp_eink.bufQueue.slots[disp_eink.bufQueue.curBufIndex].buffer;

    __EINK_INF("-----------------------DU local MODE---------------------------\n");

    if (disp_eink.eink_moudule_type == 0)
    {
        eink_type = OPM060A1;
    }
    else if ((disp_eink.eink_moudule_type == 1)
          && (disp_eink.eink_version_type == 0))
    {
        eink_type = OPM060A1;//ED060SC4;
    }
    else if ((disp_eink.eink_moudule_type == 1)
          && (disp_eink.eink_version_type == 1))
    {
        eink_type = ED060SC7;
    }
    
    width  = disp_eink.frame_width;
    height = disp_eink.frame_height;
    __EINK_INF("width = %d, height = %d\n", width, height);
    for(row =0; row < height; row++)
    {
        for(col = 0; col < width; col++)
        {            
            new = *p_new_data>>4;                //8bpp -> 4bpp
            old = *p_old_data>>4;  
            
            if (eink_type == OPM060A1) 
            {
                if(new == old)                      //局部模式,若相等，则index = 0,waveform波形送0电平
                {
                    index = 0;
                }
                else
                {
                    if(new)
                    {
                        index = old + 16;               //G0,G15-->G15
                    }
                    else
                    {
                        index = old;                    // G0, G15 --> G0
                    }
                }
                *p_wf_index++ = index;
                *p_old_data++ = *p_new_data++;    
            }
            else if ((eink_type == ED060SC7)||(eink_type == ED060SC4)) 
            {
                if(new == old)                      //局部模式,若相等，则index = 0,waveform波形送0电平
                {
                    index = 0;
                }
                else
                {
                    index = old * 16 + new;             //
                }
                *p_wf_index++ = index;                
                *p_old_data++ = *p_new_data++;
            }
        }
    } 
    
    return EPDK_OK;
}

__s32 eink_du_update_waveindex(void)
{
    int col = 0, row = 0, width = 0, height = 0;
    register __u8 index = 0;
    register __u8* p_new_data;			//新显示数据
    register __u8* p_old_data = disp_eink.wav_last_frame; 					//前一次显示的数据
    register __u8* p_wf_index = disp_eink.wav_form_index;					//波形文件索引号    
    register __u8 new = 0,old = 0;   
    __u8 eink_type = OPM060A1;
    
    __EINK_INF("-----------------------DU All area MODE---------------------------\n");
	p_new_data = disp_eink.bufQueue.slots[disp_eink.bufQueue.curBufIndex].buffer;

    if (disp_eink.eink_moudule_type == 0)
    {
        eink_type = OPM060A1;
    }
    else if ((disp_eink.eink_moudule_type == 1)
          && (disp_eink.eink_version_type == 0))
    {
        eink_type = OPM060A1;//ED060SC4;
    }
    else if ((disp_eink.eink_moudule_type == 1)
          && (disp_eink.eink_version_type == 1))
    {
        eink_type = ED060SC7;
    }
    
    width  = disp_eink.frame_width; //PIXEL_MONO_8BPP,图层中每个byte存放一个pixel
    height = disp_eink.frame_height;
    __EINK_INF("width = %d, height = %d\n", width, height);
    
    for(row =0; row < height; row++)
    {
        for(col = 0; col < width; col++)
        {   
            //1----------------------------------------------
            new = *p_new_data>>4;                //8bpp -> 4bpp
            old = *p_old_data>>4;  
            
            if (eink_type == OPM060A1) 
            {
                if(new)
                {
                    index = old + 16;               //G0,G15-->G15
                }
                else
                {
                    index = old;                    // G0, G15 --> G0
                }
                *p_wf_index++ = index;
                *p_old_data++ = *p_new_data++;   
            }
            else if ((eink_type == ED060SC7)||(eink_type == ED060SC4)) 
            {
                index = old * 16 + new;             //
                *p_wf_index++ = index;                
                *p_old_data++ = *p_new_data++;
            }
        }
    }
    
    return EPDK_OK;
}


static __s32 eink_compose_one_frame(void)
{
	int buf_index = disp_eink.bufQueue.curBufIndex;
	int update_mode = disp_eink.bufQueue.slots[buf_index].flushMode;
	
//	__EINK_LOG("eink_compose_one_frame update_mode = %x\n", update_mode);

        if (disp_eink.bufQueue.slots[buf_index].wav_compose_index == 0)
        {		
            if (update_mode == EINK_GC16_MODE) eink_gc16_update_waveindex();
            if (update_mode == (EINK_GC16_MODE|EINK_LOCAL_MODE)) eink_gc16_update_waveindex();
            if (update_mode == EINK_SHORT_GC16_LOCAL_MODE) eink_gc16_update_waveindex();
            if (update_mode == EINK_DU_MODE) eink_du_update_waveindex();
            if (update_mode == (EINK_DU_MODE | EINK_LOCAL_MODE)) eink_du_update_local_waveindex();
            if(update_mode == EINK_SHORT_DU_MODE) eink_du_update_local_waveindex();
            if (update_mode == (EINK_A2_MODE)) eink_du_update_local_waveindex();
	}

        if (update_mode == EINK_GC16_MODE) eink_gc16_update_waveform(disp_eink.wav_gc16_data);
        if (update_mode == EINK_SHORT_GC16_LOCAL_MODE) eink_gc16_update_waveform(disp_eink.wav_short_gc16_local_data);
        if (update_mode == (EINK_GC16_MODE | EINK_LOCAL_MODE)) eink_gc16_update_waveform(disp_eink.wav_common_data);
        if (update_mode == EINK_DU_MODE) eink_gc16_update_waveform(disp_eink.wav_du_data);
        if (update_mode == (EINK_DU_MODE | EINK_LOCAL_MODE)) eink_gc16_update_waveform(disp_eink.wav_du_data);
        if (update_mode == EINK_SHORT_DU_MODE) eink_gc16_update_waveform(disp_eink.wav_short_du_data);
	if (update_mode == EINK_A2_MODE) eink_gc16_update_waveform(disp_eink.wav_short_du_data);
	return 0;
}

void eink_TCON_IO_enable(void)
{
    unsigned int* pReg;
    pReg = (unsigned int*)0xf1c0c08c;            
    *pReg &= (~0x04FFFFFF);
}

void eink_TCON_IO_disable(void)
{   
    unsigned int* pReg;
    pReg = (unsigned int*)0xf1c0c08c;            
    *pReg |= (0x04FFFFFF);
}

__s32 eink_FreeBuffer(__s32 index) 
{
	down(disp_eink.buffer_lock_sem);

	disp_eink.bufQueue.slots[index].bufState = FREE;
	disp_eink.bufQueue.slots[index].wav_compose_index = 0;
	disp_eink.bufQueue.slots[index].wav_refresh_index = 0;
	disp_eink.bufQueue.slots[index].total_frame = 0;
	disp_eink.bufQueue.slots[index].current_frame = 0;
	up(disp_eink.buffer_lock_sem);

	return 0;
}

__s32 eink_UpdateBuffer(void) 
{
	__s32 i = 0, found = -1;
	bool isOld;

	down(disp_eink.buffer_lock_sem);

	/*find the oldest DEQUEUED buffer*/
	for (i = 0; i < NUM_BUFFER_SLOTS; i++) 
	{
		if (disp_eink.bufQueue.slots[i].bufState == DEQUEUED) 
		{
			if (found != -1) 
			{
				/*find the oldest buffer*/
				isOld = disp_eink.bufQueue.slots[i].alloc_index < disp_eink.bufQueue.slots[found].alloc_index;
			}
			
			if (found < 0 || isOld) 
			{
				found = i;
			}
			
		}
	}

	
	__EINK_LOG("eink_UpdateBuffer found = %d\n", found);

	if (found == -1) 
	{
		up(disp_eink.buffer_lock_sem);
		return -1;
	}
		
	disp_eink.bufQueue.slots[found].bufState = QUEUED;
	disp_eink.bufQueue.useCount++;
	disp_eink.bufQueue.slots[found].use_index = disp_eink.bufQueue.useCount;
	disp_eink.bufQueue.curBufIndex = found;
	
	up(disp_eink.buffer_lock_sem);

	return 0;
}

static __s32 eink_refresh_pwr_ctrl(__s32 sel)
{
    __u32 i;
	__s32 buf_index;

    down(disp_eink.wav_compose_sem);
	__EINK_LOG("eink_refresh_pwr_ctrl \n");

	if (eink_UpdateBuffer() != 0) 
	{
		return 0;
	}

	disp_eink.update_status = 1;
    for (i = 0; i < EINK_WAV_BUFFER_NUM / 4; i++)
    {
        eink_compose_one_frame();
    }
    
    buf_index = disp_eink.bufQueue.curBufIndex;
    __EINK_LOG("Lcd on ... \n");

	disp_eink.bufQueue.slots[buf_index].wav_refresh_index++;
    Drv_disp_fb_set_layer_addr_eink(sel, WF_FB_ID, 0);

    eink_TCON_IO_enable();
    Drv_disp_eink_panel_on(sel);
 
    down(disp_eink.power_off_sem);
    Drv_disp_eink_panel_off(sel);
    __EINK_LOG("Lcd off ... \n");

    for (i = 0; i < EINK_WAV_BUFFER_NUM; i++)
	{
        disp_eink.wav_buffer[i].wav_index = -1;
	}

	disp_eink.update_status = 0;
	
	eink_FreeBuffer(buf_index);
    
	return 0;
}

void eink_refresh_wav_form_1(unsigned long sel)
{

	BufSlot* bufSlot = &disp_eink.bufQueue.slots[disp_eink.bufQueue.curBufIndex];

	if (bufSlot->bufState != QUEUED) {
		__EINK_WRN("come to interrupt handle function but Eink not initial!\n");
		return;
	}

/*    __EINK_LOG("wav_refresh_index = %d\n", bufSlot->wav_refresh_index);*/
    if (bufSlot->wav_refresh_index == bufSlot->total_frame)
    {
        disp_eink.wav_buffer[(bufSlot->current_frame - 1) % EINK_WAV_BUFFER_NUM].wav_index = -1;
        bufSlot->wav_refresh_index++;
    }
    else if( bufSlot->wav_refresh_index == (bufSlot->total_frame + 1))
    {
        disp_eink.wav_buffer[bufSlot->current_frame % EINK_WAV_BUFFER_NUM].wav_index = -1;
        bufSlot->current_frame = -1;
			
		disp_eink.last_update_mode = bufSlot->flushMode;

        eink_TCON_IO_disable();
        up(disp_eink.power_off_sem);

		//20130402, yuanlian
		//check the update mode and trigger the internal update task
		if((bufSlot->flushMode==EINK_SHORT_DU_MODE) && (disp_eink.internal_update == 0)){
			int i = 0;
			//check if buffer queue is empty or not
			for(i=0; i<NUM_BUFFER_SLOTS; i++){
				if(i==disp_eink.bufQueue.curBufIndex){
					continue;
				}
				else if(disp_eink.bufQueue.slots[i].bufState !=FREE){
					break;
				}
			}

			if(i == NUM_BUFFER_SLOTS){
				disp_eink.internal_update = 1;
				__EINK_LOG("enable internal_update\n");
				up(disp_eink.internal_update_sem);
			}
		}
		else if (disp_eink.internal_update != 0){
			__EINK_LOG("disable internal_update\n");
			disp_eink.internal_update = 0;
		}
		else{}
    }
    
    if (bufSlot->wav_refresh_index < bufSlot->total_frame)
    {
        if (disp_eink.wav_buffer[bufSlot->wav_refresh_index % EINK_WAV_BUFFER_NUM].wav_index != -1)
        {
        	bufSlot->current_frame = bufSlot->wav_refresh_index;
			if (bufSlot->current_frame >= 1)
			{
				disp_eink.wav_buffer[(bufSlot->current_frame - 1) % EINK_WAV_BUFFER_NUM].wav_index = -1;
			}

			
            Drv_disp_fb_set_layer_addr_eink(sel, WF_FB_ID, bufSlot->wav_refresh_index % EINK_WAV_BUFFER_NUM);
        	bufSlot->wav_refresh_index++;
        }
        else
        {
            __EINK_LOG("frame not ready ... \n\n");
        }
    }
    
    eink_compose_one_frame();
}

static __s32 internal_update_thread(void *p_arg)
{
	while (1)
	{
		down(disp_eink.internal_update_sem);
		msleep(500);
		if(disp_eink.internal_update!=0){
			if(disp_eink.bufQueue.slots[disp_eink.bufQueue.curBufIndex].current_frame==0 
				&& (disp_eink.last_update_mode==EINK_SHORT_DU_MODE))
			{
				int i = 0;
				//check if buffer queue is empty or not
				for(i=0; i<NUM_BUFFER_SLOTS; i++){
					if(disp_eink.bufQueue.slots[i].bufState !=FREE){
						break;
					}
				}

				//if all buffer all free, update once with short gc16 local mode
				if(i==NUM_BUFFER_SLOTS){
					__EINK_LOG("internal update oncen");
					Disp_eink_update(0,0, EINK_SHORT_GC16_LOCAL_MODE);
				}
				else{
					__EINK_LOG("buffer queue is not empty,skip internal flush\n");
				}
			}
			else{
				__EINK_LOG("eink is busy, skip internal_update::current_frame->[%d], mode->[%d]n", disp_eink.bufQueue.slots[disp_eink.bufQueue.curBufIndex].current_frame, disp_eink.last_update_mode);
			}
			disp_eink.internal_update = 0;
		}
	}

	return 0;
}




static __s32 eink_refresh_thread(void *p_arg)
{
	while (1)
	{
        eink_refresh_pwr_ctrl(0);
	}

	return 0;
}

__s32 Disp_eink_get_sys_config(__u32 sel, Disp_eink_t* para)
{
    __s32 value = 0;
    char primary_key[20];
    __s32 ret = 0;
    
    sprintf(primary_key, "lcd%d_para", sel);
    
    ret = OSAL_Script_FetchParser_Data(primary_key, "lcd_eink_module_type", &value, 1);
    if(ret < 0)
    {
        DE_WRN("fetch script data %s.lcd_eink_module_type fail\n", primary_key);
    }
    else
    {
        para->eink_moudule_type = value;
        DE_INF("lcd_eink_module_type = %d\n", value);
    }

    ret = OSAL_Script_FetchParser_Data(primary_key, "lcd_eink_version_type", &value, 1);
    if(ret < 0)
    {
        DE_WRN("fetch script data %s.lcd_eink_version_type fail\n", primary_key);
    }
    else
    {
        para->eink_version_type = value;
        DE_INF("lcd_eink_version_type = %d\n", value);
    }

    ret = OSAL_Script_FetchParser_Data(primary_key, "lcd_eink_ctrl_data_type", &value, 1);
    if(ret < 0)
    {
        DE_WRN("fetch script data %s.lcd_eink_ctrl_data_type fail\n", primary_key);
    }
    else
    {
        para->eink_ctrl_data_type = value;
        DE_INF("lcd_eink_ctrl_data_type = %d\n", value);
    }

    return 0;
}

void eink_dequeueBuffer(__u32* indexBuf, __u32 mode) 
{
	__s32 i = 0, found = -1;
	bool isOld;

	down(disp_eink.buffer_lock_sem);
	for (i = 0; i < NUM_BUFFER_SLOTS; i++) 
	{
		if (disp_eink.bufQueue.slots[i].bufState == FREE) 
		{
			if (found != -1) 
			{
				/*find the oldest buffer*/
				isOld = disp_eink.bufQueue.slots[i].use_index < disp_eink.bufQueue.slots[found].use_index;
			}
			
			if (found < 0 || isOld) 
			{
				found = i;
			}
			
		}
	}

	__EINK_LOG("dequueBufer found = %d\n", found);

	if (found >= 0) 
	{
		disp_eink.bufQueue.slots[found].bufState = DEQUEUED;
		
		*indexBuf = found;
		
	} 
	else 
	{
		/*cover the last alloc buffer*/
		for (i = 0; i < NUM_BUFFER_SLOTS; i++) 
		{
			if (disp_eink.bufQueue.slots[i].bufState == DEQUEUED) 
			{
				if (found != -1) 
				{
					/*find the oldest alloc buffer*/
					isOld = disp_eink.bufQueue.slots[i].alloc_index < disp_eink.bufQueue.slots[found].alloc_index;
				}
				
				if (found < 0 || isOld) 
				{
					found = i;
				}
				
			}
		}

		disp_eink.bufQueue.slots[found].use_index = 0;
		disp_eink.bufQueue.slots[found].bufState = DEQUEUED;

		*indexBuf = found;
	}

	disp_eink.bufQueue.allocCount++;
	disp_eink.bufQueue.slots[found].alloc_index = disp_eink.bufQueue.allocCount;
	disp_eink.bufQueue.slots[found].flushMode = mode;
	disp_eink.bufQueue.slots[found].wav_compose_index = 0;
	disp_eink.bufQueue.slots[found].wav_refresh_index = 0;
	disp_eink.bufQueue.slots[found].total_frame = 0;
	

	__EINK_LOG("dequueBufer amend index = %d\n", found);
	up(disp_eink.buffer_lock_sem);
}


__u8* eink_get32BppBuffer(void)
{
    __u32 xoffset, yoffset;
    unsigned long screen_base;
    __u32 bits_per_pixel;
    __u32 pixel_offset;
	__u8 *src32BppBuf = NULL;

	/*get 32bpp framebuffer*/
	Drv_disp_close_fb(0, 0);
   	Drv_disp_get_fb_info(0, &xoffset, &yoffset, &screen_base);
   
	bits_per_pixel = 32;
    pixel_offset = disp_eink.frame_width * yoffset + xoffset;
   	src32BppBuf = (__u8 *)screen_base + pixel_offset * bits_per_pixel / 8;

	return src32BppBuf;
}

__u8* eink_get8BppBuffer(__u32 mode) 
{
	__u32 indexBuf;
	eink_dequeueBuffer(&indexBuf, mode);
	return disp_eink.bufQueue.slots[indexBuf].buffer;
}

void eink_setCurBufFlushMode(__u32 mode)
{
	down(disp_eink.buffer_lock_sem);
	disp_eink.bufQueue.slots[disp_eink.bufQueue.curBufIndex].flushMode = mode;
	up(disp_eink.buffer_lock_sem);
}

__u32 eink_getCurBufFlushMode(void) 
{
	__u32 mode;
	
	down(disp_eink.buffer_lock_sem);
	mode = disp_eink.bufQueue.slots[disp_eink.bufQueue.curBufIndex].flushMode;
	up(disp_eink.buffer_lock_sem);

	return mode;
}

static void init_wav_buffer(__u8 *ptr)
{
    __u32 *p = (__u32 *)ptr;
    int w = disp_eink.wav_width;
    int v1 = (EINK_LSL + EINK_LBL + EINK_LDL + 1) & ~1;
    int v2 = w - v1;
    int i, line;

    for (line=0; line<EINK_FSL; line++) {
	for (i=0; i<w; i++) *p++ = (SPV | XSTL | DMASK);
    }

    for (i=0; i<v1/2; i++) *p++ = (SPV | CKV | XSTL | MODE | DMASK);
    for (i=0; i<v1/2; i++) *p++ = (CKV | XSTL | MODE | DMASK);
    for (i=0; i<v2; i++) *p++ = (XSTL | MODE | DMASK);

    for (i=0; i<v1/2; i++) *p++ = (CKV | XSTL | MODE | DMASK);
    for (i=0; i<v1/2; i++) *p++ = (SPV | CKV | XSTL | MODE | DMASK);
    for (i=0; i<v2; i++) *p++ = (SPV | XSTL | MODE | DMASK);

    for (i=0; i<v1; i++) *p++ = (SPV | CKV | XSTL | MODE | DMASK);
    for (i=0; i<v2; i++) *p++ = (SPV | XSTL | MODE | DMASK);

    for (line=0; line<EINK_FDL; line++) {
        for (i=0; i<EINK_LSL; i++) *p++ = (SPV | CKV | XSTL | XLE | MODE | DMASK);
        for (i=0; i<EINK_LBL-1; i++) *p++ = (SPV | CKV | XSTL | MODE | DMASK);
        *p++ = (SPV | CKV | XOE | XSTL | MODE | DMASK);
        for (i=0; i<4; i++) *p++ = (SPV | CKV | XOE | MODE | DMASK);
        for (i=0; i<EINK_LDL-3; i++) *p++ = (SPV | CKV | XOE | XSTL | MODE | DMASK);
        for (i=0; i<EINK_LEL-4; i++) *p++ = (SPV | XOE | XSTL | MODE | DMASK);
        for (i=0; i<3; i++) *p++ = (SPV | XSTL | MODE | DMASK);
    }

    for (i=0; i<v1; i++) *p++ = (SPV | CKV | XSTL | MODE | DMASK);
    for (i=0; i<v2; i++) *p++ = (SPV | XSTL | MODE | DMASK);

    for (line=0; line<EINK_FEL-1; line++) {
	for (i=0; i<w; i++) *p++ = (SPV | XSTL | MODE | DMASK);
    }

}

__s32 Disp_eink_init(__u32 sel, __eink_init_para* para)
{    
    __disp_fb_create_para_t fb_create_para[1];
    unsigned long screen_base;
    __u32 xoffset, yoffset;
    int i, buf_size;
    __u8 *slots;

    memset((void*)(&disp_eink), 0, sizeof(Disp_eink_t));

    ADC_init();
    
    disp_eink.internal_update = 0;
    disp_eink.last_update_mode = EINK_INIT_MODE;

    disp_eink.frame_width = para->width;
    disp_eink.frame_height = para->height;
    disp_eink.frame_pixelfmt = para->pixelfmt;
    disp_eink.wav_width    = disp_eink.frame_width / 4 + EINK_HYNC;
    disp_eink.wav_height = disp_eink.frame_height + EINK_VYNC;
    disp_eink.temperature = 26;
    disp_eink.orientation = 0;

    sema_init(disp_eink.buffer_lock_sem, 1);

    buf_size = disp_eink.frame_width * disp_eink.frame_height;
    slots = (__u8 *)vmalloc(NUM_BUFFER_SLOTS * buf_size);
    if (slots == NULL)
    {
        __EINK_ERR("Balloc memory for buffer slots failed\n");
        return EPDK_FAIL;
    }
    disp_eink.bufQueue.bufCount = NUM_BUFFER_SLOTS;
    disp_eink.bufQueue.curBufIndex  = 0; 
    for (i = 0; i < NUM_BUFFER_SLOTS; i++) 
    {    
        disp_eink.bufQueue.slots[i].use_index = 0;
        disp_eink.bufQueue.slots[i].alloc_index = 0;
        disp_eink.bufQueue.slots[i].bufState = FREE;
        disp_eink.bufQueue.slots[i].flushMode = EINK_INIT_MODE;
        disp_eink.bufQueue.slots[i].wav_compose_index = 0;
        disp_eink.bufQueue.slots[i].wav_refresh_index = 0;
        disp_eink.bufQueue.slots[i].buffer = (__u8 *)(slots + i * buf_size);
    }    
       
    disp_eink.wav_last_frame = (__u8 *)vmalloc(disp_eink.frame_width * disp_eink.frame_height);
    if(disp_eink.wav_last_frame == NULL)
    {
        __EINK_ERR("Balloc memory for last frame buffer failed\n");        
        return EPDK_FAIL;
    }
    memset(disp_eink.wav_last_frame, 0xFF, disp_eink.frame_width * disp_eink.frame_height);
    
    disp_eink.wav_form_index = (__u8 *)vmalloc(disp_eink.frame_width * disp_eink.frame_height);
    if(disp_eink.wav_form_index == NULL)
    {
        __EINK_ERR("Balloc memory for last frame buffer failed\n");
        return EPDK_FAIL;
    }
    
    eink_init_fb_para(fb_create_para, disp_eink.wav_width, disp_eink.wav_height, EINK_WAV_BUFFER_NUM + 1);
    Drv_disp_Fb_Request(WF_FB_ID, fb_create_para);
    Drv_disp_get_fb_info(WF_FB_ID, &xoffset, &yoffset, &screen_base);
    Drv_disp_fb_layer_set_top(sel, WF_FB_ID);    
    for(i = 0; i < EINK_WAV_BUFFER_NUM; i++)
    {
        disp_eink.wav_buffer[i].wav_address = (__u8 *)(screen_base + 4 * i * disp_eink.wav_width * disp_eink.wav_height);
        disp_eink.wav_buffer[i].wav_index = -1;
        init_wav_buffer(disp_eink.wav_buffer[i].wav_address);
    }
    
    sema_init(disp_eink.wav_compose_sem, 0);       
    sema_init(disp_eink.wav_refresh_sem, 0);
    disp_eink.wav_refresh_task = kthread_create(eink_refresh_thread, NULL, "eink refresh proc");
    if (IS_ERR(disp_eink.wav_refresh_task))
    {    
        __EINK_WRN("Unable to start kernel thread %s.\n", "eink update proc");
        return PTR_ERR(disp_eink.wav_refresh_task);
    }

    sema_init(disp_eink.stby_act_lock, 1);
    sema_init(disp_eink.power_off_sem, 0);
    wake_up_process(disp_eink.wav_refresh_task);


    sema_init(disp_eink.internal_update_sem, 0);
    disp_eink.internal_update_task = kthread_create(internal_update_thread, NULL, "internal update task");
    if (IS_ERR(disp_eink.internal_update_task))
    {    
        __EINK_WRN("Unable to start kernel thread %s.\n", "internal update task");
        return PTR_ERR(disp_eink.internal_update_task);
    }
    wake_up_process(disp_eink.internal_update_task);


    Disp_eink_get_sys_config(sel, &disp_eink);
    
    Drv_disp_eink_panel_off(sel);
    return 0;
}

__s32 Disp_eink_exit(__u32 sel)
{
	if(disp_eink.wav_refresh_task)
	{
		kthread_stop(disp_eink.wav_refresh_task);
		disp_eink.wav_refresh_task = NULL;
	}
    
    if(disp_eink.wav_compose_task)
	{
		kthread_stop(disp_eink.wav_compose_task);
		disp_eink.wav_compose_task = NULL;
	}
    
    Drv_disp_Fb_Release(WF_FB_ID);
    Drv_disp_Fb_Release(SCALER_OUT_FB_ID);
    
    if (disp_eink.wav_form_index != NULL)
    {
        vfree(disp_eink.wav_form_index);
        disp_eink.wav_form_index = NULL;
    }

    if (disp_eink.wav_last_frame != NULL)
    {
        vfree(disp_eink.wav_last_frame);
        disp_eink.wav_last_frame = NULL;
    }
    
	eink_unload_waveform(disp_eink.wav_du_data);
	eink_unload_waveform(disp_eink.wav_gc16_data);
	eink_unload_waveform(disp_eink.wav_common_data);
	eink_unload_waveform(disp_eink.wav_short_du_data);
	eink_unload_waveform(disp_eink.wav_short_gc16_local_data);
    
    Drv_disp_eink_panel_off(sel);

	return 0;
}


__s32 Disp_eink_update(__u32 sel, __u32 fb_id, __u32 mode)
{    
    __u8 *in_buffer;
    __u8 *out_buffer;
    __u8 *src, *dst;
    int x, y, inc;

    __EINK_MSG("Disp_eink_update\n");

    if (mode == EINK_INIT_MODE) return 0;

    disp_eink.temperature = ADC_Get_temperature();
    disp_eink.internal_update = 0;
    cpufreq_driver_target(cpufreq_cpu_get(0), 1008000, 0);

    in_buffer = eink_get32BppBuffer();
    out_buffer = eink_get8BppBuffer(mode);

    for (y=0; y<600; y++) {
        switch (disp_eink.orientation) {
          default: // UR
        	src = in_buffer + y * 800;
        	inc = 1;
        	break;
          case 1:  // CW
        	src = in_buffer + 799 * 608 + y;
        	inc = -608;
        	break;
          case 2:  // UD
        	src = in_buffer + 800 * (599 - y) + 799;
        	inc = -1;
        	break;
          case 3:  // CCW
        	src = in_buffer + (599-y);
        	inc = 608;
        	break;
        }
        dst = out_buffer + y * 800;
        for (x=0; x<200; x++) {
        	*dst++ = *src;
        	src += inc;
        	*dst++ = *src;
        	src += inc;
        	*dst++ = *src;
        	src += inc;
        	*dst++ = *src;
        	src += inc;
        }
    }

    eink_loadWaveFormData();
    up(disp_eink.wav_compose_sem);
    return 0;
}

__s32 Disp_eink_set_mode(__u32 sel, __eink_update_mode mode)
{
    eink_setCurBufFlushMode(mode);
    return 0;
}

__s32 Disp_eink_set_temperature(__u32 sel, __u32 temperature)
{
    disp_eink.temperature = temperature;
    return 0;
}

void Disp_eink_set_stby_act(Eink_stby_act_e act)
{
    disp_eink.stby_act = act;
}

Eink_stby_act_e Disp_eink_get_stby_act(void)
{
    return disp_eink.stby_act;
}

__s32 Disp_eink_get_update_status(__u32 sel)
{
    return disp_eink.update_status;
}

void Disp_eink_stby_act_lock(void)
{
    down(disp_eink.stby_act_lock);
}

void Disp_eink_stby_act_unlock(void)
{
    up(disp_eink.stby_act_lock);
}

__s32 Disp_eink_set_orientation(__u32 sel, __u32 fb_id, __u32 o)
{
    disp_eink.orientation = o;
    return 0;
}

