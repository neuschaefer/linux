/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/
#include "arabic.h"
#include "internal_arabic.h"
#include "join_type.h"
#include "font/x_fe.h"


#define ITEM_TYPE(pt_item)   (pt_item)->t_bidi_type
#define ITEM_LEN(pt_item)    (pt_item)->t_len
#define ITEM_POS(pt_item)    (pt_item)->ui2_pos
#define ITEM_LEVEL(pt_item)  (pt_item)->ui1_level
#define ITEM_PREV(pt_item)   ((pt_item)->t_lnk).pt_prev
#define ITEM_NEXT(pt_item)   ((pt_item)->t_lnk).pt_next


static HANDLE_T     h_free_list_lock = NULL_HANDLE;
static ITEM_DLIST_T t_free_list;
static BOOL b_init = TRUE;
#define ARABIC_CHECK_ID(id)        (((id) < Ui2_Bidi_Mirrored_Count)? (id): 0)


VOID free_list_lock(
    VOID)
{
    INT32 i4_ret;

    i4_ret = x_sema_lock( h_free_list_lock, X_SEMA_OPTION_WAIT );
    if ( i4_ret != OSR_OK )
    {
        DBG_ABORT(DBG_MOD_WGL);
    }
}

VOID free_list_unlock(
    VOID)
{
    INT32 i4_ret;

    i4_ret = x_sema_unlock( h_free_list_lock );
    if ( i4_ret != OSR_OK )
    {
        DBG_ABORT(DBG_MOD_WGL);
    }
}



static BOOL is_arabic_char(UTF16_T w2_char)
{
    if (((w2_char >= 0x0600)&&(w2_char <= 0x06FF))
        || ((w2_char >= 0x0750)&&(w2_char <= 0x077F))
        || ((w2_char >= 0xFB50)&&(w2_char <= 0xFDFF))
        || ((w2_char >= 0xFE70)&&(w2_char <= 0xFEFF))
        || ((w2_char >= 0x0591)&&(w2_char <= 0x05F4)))// add Herbew.
    {
        return TRUE;
    }
    return FALSE;
}


BOOL check_exist_arabic(UTF16_T* w2s_source, UINT16 ui2_max_len)
{
	
	 UTF16_T*	 		w2s_start;  
	 UTF16_T*	 		w2s_end;

	 if (w2s_source == NULL)
	 {
		 return FALSE;
	 }	 
     if (ui2_max_len == 0)
     {
		 return FALSE;
     }
     w2s_end = w2s_source + ui2_max_len ; 
     for (w2s_start = w2s_source; w2s_start < w2s_end;)
     {
		 if(*w2s_start == WGL_CHAR_NULL)
		 {
			break;
		 }
    	 if (is_arabic_char(*w2s_start))
    	 {
			 return TRUE;
    	 }
    	 w2s_start++;
     }
     return FALSE;
}



static UC_BIDI_CLASS_T get_bidi_class(UTF16_T w2_char)
{
	 UC_BIDI_CLASS_T  *pt_char_type = NULL;
	 UINT16				ui2_div;
	 
	 if ( w2_char < 0x100)
	 {
#ifdef DIVX_PLUS_CER
        if (w2_char == 0x2d || w2_char == 0xa)//[CR334802]
        {
            return BIDI_TYPE_L;
        }
#endif
	     pt_char_type = Bidi_Type_Block0000;
	     ui2_div = 256; 
	 }
	 else if ((w2_char >= 0x0591)&&(w2_char <= 0x05F4))
	 {
	 	return BIDI_TYPE_R; // add Hebrew support.
	 }
	 else if (w2_char > 0x5FF && w2_char < 0x0700)/*0x0600---0x06ff arabic */
	 {
		  pt_char_type = Bidi_Type_Block0600;
		  ui2_div = 256; 
	 }
	 else if (w2_char > 0x0749 && w2_char < 0x0780) /*0x0750---0x077f arabic supplement*/
	 {
		  pt_char_type = Bidi_Type_Block0750;
		  ui2_div = 48; 
	 }
	 else if (w2_char >= 0xFB50 && w2_char <= 0xFDFD) 
	 {
		  return BIDI_TYPE_AL;
	 }
	  else if (w2_char >= 0xFE70 && w2_char <= 0xFEFC) 
	 {
		  return BIDI_TYPE_AL;
	 }
	 else if (w2_char >= 0xFC00 && w2_char <= 0xFCFF) 
	 {
		  return BIDI_TYPE_AL;
	 }
	 else if (w2_char >= 0xFD00 && w2_char <= 0xFDFF)
	 {
		  if (w2_char >= 0xFD3E && w2_char <= 0xFD4F)
		  {
			  return BIDI_TYPE_ON;
		  }
		   return BIDI_TYPE_AL;
		  
	 }
	 else if (w2_char == 0x200E)  //CR[323276]
	 {	
	 	return BIDI_TYPE_R;
	 }	
	 else if (w2_char == 0x200F)
	 {	
	 	return BIDI_TYPE_L;
	 }
	 
	 if (pt_char_type)
	 {
	     return pt_char_type[w2_char % ui2_div];
	 } 	
	 return BIDI_TYPE_L;
	 
}



static JOINING_TYPE_T get_joining_type(UTF16_T w2_char)
{
    JOINING_TYPE_T  *pt_join_type = NULL;
    UINT16				ui2_div;
    
    if (w2_char > 0x5FF && w2_char < 0x0700)/*0x0600---0x06ff arabic */
    {
  	  pt_join_type = Joining_Block0600;
  	  ui2_div = 256; 
    }
    else if (w2_char > 0x0749 && w2_char < 0x0780) /*0x0750---0x077f arabic supplement*/
    {
  	  pt_join_type = Joining_Block0750;
  	  ui2_div = 48; 
    }
    else if (w2_char == 0x00AD)
    {
		return JOIN_TYPE_T;
    }
    else if (w2_char == 0x200D || w2_char == 0x07FA)
    {
		return JOIN_TYPE_C;
    }
    if (pt_join_type)
    {
      return pt_join_type[w2_char % ui2_div];
    }
    else
    {
      return JOIN_TYPE_N;
    }
}

#if 0
UTF16_T binary_search(LIGATURE_T *pt_ligature,  UTF16_T ui4_rchar, UTF16_T ui2_lchar)
{
	INT32 i4_low = 1;
	INT32 i4_high = 1;
	INT32 i4_mid;

	while(i4_low <= i4_high)
	{
		i4_mid = (i4_low + i4_high) / 2;
		if (ui4_rchar == pt_ligature[i4_mid].elements[0])
		{
			return pt_ligature[i4_mid].ligature;
		}
		else if (ui4_rchar < pt_ligature[i4_mid].elements[0])
		{
			i4_high = i4_mid - 1;
		}
		else
		{
			i4_low = i4_mid + 1;

		}
	}
	return WGL_CHAR_NULL;
}
#endif


BOOL get_mirror_char(
		       UTF16_T ch,
		       UTF16_T *mirrored_ch)
{
  INT32 i4_step;
  UINT32 ui4_pos;
  BOOL found = FALSE;
  UTF16_T cmp_ch;

   i4_step = (Ui2_Bidi_Mirrored_Count / 2) + 1;
   ui4_pos = (UINT32)i4_step;

  while(i4_step > 1)
  {	
      if (ui4_pos >= (UINT32)Ui2_Bidi_Mirrored_Count)
      {
         break;
      }
      cmp_ch = Bidi_Mirroring_Char[ARABIC_CHECK_ID(ui4_pos)].base_char;
      i4_step = (i4_step + 1) / 2;
      
      if (cmp_ch < ch)
	  {
		  ui4_pos += (UINT32)i4_step;
		  if (ui4_pos > (UINT32)(Ui2_Bidi_Mirrored_Count - 1))
		  {
		      ui4_pos = (UINT32)(Ui2_Bidi_Mirrored_Count - 1);
		      break;
		  }
	  }
      else if (cmp_ch > ch)
	  {
	    
	      if (ui4_pos < (UINT32)i4_step)
	      {
	          ui4_pos = 0;
	          break;
	      }
	      else
	      {
			  ui4_pos = ui4_pos - i4_step ;
	      }
	 }
     else
     {
	     break;
	 }
  }
  if (ui4_pos < (UINT32)Ui2_Bidi_Mirrored_Count)
  {
      if (Bidi_Mirroring_Char[ARABIC_CHECK_ID(ui4_pos)].base_char == ch)
      {
    	  *mirrored_ch = Bidi_Mirroring_Char[ARABIC_CHECK_ID(ui4_pos)].mirr_char;
    	  found = TRUE;
      }
  }
  return found;
}


static void wstr_safe_cpy(UTF16_T* w2s_input,UINT16 ui2_max_len,UTF16_T* w2s_output)
{
	UTF16_T*  w2s_dst;
	UTF16_T*  w2s_src;
	UINT16	 ui2_len = 0;   
	
	/* copy src to dst  */  	
	w2s_dst = w2s_output + ui2_max_len - 1; 
	w2s_src = w2s_input + ui2_max_len - 1; 
	while(ui2_len != ui2_max_len) 
	{ 
		*(w2s_dst--) = *(w2s_src--);
		ui2_len++;
	} 
}



static void string_reverse(UTF16_T *w2s_source, UINT16 ui2_max_len)
{
  UINT16 ui2_idx;
  UTF16_T w2_tmp;
  for (ui2_idx = 0; ui2_idx < (UINT16)(ui2_max_len / 2); ui2_idx++)
    {
      w2_tmp = w2s_source[ui2_idx];
      w2s_source[ui2_idx] = w2s_source[ui2_max_len - 1 - ui2_idx];
      w2s_source[ui2_max_len - 1 - ui2_idx] = w2_tmp;
    }
}




static ITEM_T* create_run_item(UINT16 ui2_sub_len, 
								  UC_BIDI_CLASS_T t_bidi_type, 
								  UINT16 ui2_pos, 
								  UINT8 ui1_level)
{	
    ITEM_T            	 *pt_item;	
	 
	free_list_lock();
  	if (DLIST_IS_EMPTY(&t_free_list))
  	{  	
		pt_item =  (ITEM_T*) WGL_MEM_ALLOC(sizeof(ITEM_T));
	    if (pt_item == NULL)
	    {         
	        return NULL;
	    }
	    x_memset(pt_item, WGL_CHAR_NULL, sizeof(ITEM_T));   		 		
	}
	else
	{
	    pt_item = DLIST_TAIL(&t_free_list);
		DLIST_REMOVE(pt_item,&t_free_list,t_lnk);
		ITEM_PREV(pt_item)= NULL;
		ITEM_NEXT(pt_item) = NULL;
		
	}
	free_list_unlock();
	ITEM_TYPE(pt_item) = t_bidi_type;
	ITEM_LEN(pt_item) = ui2_sub_len;
	ITEM_POS(pt_item) =  ui2_pos; 
	ITEM_LEVEL(pt_item) =  ui1_level;  
	
	return pt_item;	
}


static VOID free_run_item(ITEM_T* pt_item)
{
    free_list_lock();
	DLIST_INSERT_TAIL(pt_item,&t_free_list,t_lnk);
	free_list_unlock();
	
	
}

#if 0
static void update_single_table(LIGATURE_T* pt_lgt_table,INT32 i4_lgt_len,HFONT_T h_fnt)
{
	INT32     		i4_table_idx;
	INT32  			i4_ret;
	FE_GLYPH_T      t_glyph;

	 Printf("update_single_table\n");

	for (i4_table_idx = 0; i4_table_idx < i4_lgt_len; i4_table_idx++)
    {    	
    	i4_ret = x_fe_load_char(h_fnt,pt_lgt_table[i4_table_idx].ligature,&t_glyph);
		if (i4_ret == FER_OK)
		{
			pt_lgt_table[i4_table_idx].b_exist = TRUE;
			Printf("pt_lgt_table[i4_table_idx].ligature  =0x%x\n ",pt_lgt_table[i4_table_idx].ligature);
		}
    }
  
}


static VOID update_ligature_tables(const CHAR*             ps_font /* font name                     */)
{	
	HFONT_T			h_fnt;
	INT32           i4_max_idx;
	INT32  			i4_ret;
	UINT32          ui4_sys_tick;


	
 
    if (i4_ret != FER_OK)
    {
        Printf("x_fe_create_font error=%d\n",i4_ret);
		return;
    }
    Printf("update_ligature_tables\n");
	i4_max_idx = (INT32)(sizeof(isolate_lgt_table)/ sizeof(LIGATURE_T));  
    update_single_table((LIGATURE_T*)isolate_lgt_table,i4_max_idx,h_fnt);
    i4_max_idx = (INT32)(sizeof(final_lgt_table)/ sizeof(LIGATURE_T));     
    update_single_table((LIGATURE_T*)final_lgt_table,i4_max_idx,h_fnt);
    i4_max_idx = (INT32)(sizeof(medial_lgt_table)/ sizeof(LIGATURE_T));     
    update_single_table((LIGATURE_T*)medial_lgt_table,i4_max_idx,h_fnt);
    i4_max_idx = (INT32)(sizeof(initial_lgt_table)/ sizeof(LIGATURE_T)); 
    update_single_table((LIGATURE_T*)initial_lgt_table,i4_max_idx,h_fnt);   		
	ui4_sys_tick = os_get_sys_tick() - ui4_sys_tick;    
	Printf("update_ligature_tables ui4_sys_tick = 0x%x\n",ui4_sys_tick);

}

#endif


static ITEM_T* get_initial_bidi_runs( UTF16_T *w2s_source,UINT16 ui2_w2s_len,
									   UINT8 *pui1_embed_level,INT8 *pi1_embed_dir, 
									   UINT16* pui2_bidi_type, BOOL b_find_level)
{
	ITEM_T *pt_item_head = NULL;
	ITEM_T *pt_item_prev = NULL;
	ITEM_T *pt_item = NULL;
	ITEM_T *pt_item_next = NULL;
	UC_BIDI_CLASS_T *pt_bidi_class = NULL;	
	UC_BIDI_CLASS_T  t_prev_class = BIDI_TYPE_UN;
	UC_BIDI_CLASS_T  t_cur_class =  BIDI_TYPE_UN;
	UINT16 ui2_len;
	INT32  i4_pos;
	UINT16 ui2_idx;
	INT32   i4_ret;
	
	pt_bidi_class = (UC_BIDI_CLASS_T*)WGL_MEM_ALLOC(sizeof(UC_BIDI_CLASS_T) * ui2_w2s_len);
    if (pt_bidi_class == NULL)
    {
		return NULL;
    }
    x_memset(pt_bidi_class, 0, sizeof(UC_BIDI_CLASS_T) * ui2_w2s_len); 
    if (!b_find_level)
    {
        *pi1_embed_dir = (*pui1_embed_level == 0) ? BIDI_TYPE_L : BIDI_TYPE_R;
    }
    else
    {
        *pui1_embed_level = 0;
        if (*pi1_embed_dir != -1)
        {
            b_find_level = FALSE;
            *pui1_embed_level = 1;
            *pi1_embed_dir = BIDI_TYPE_R;
        }
    }
    *pui2_bidi_type = 0;
    t_prev_class = BIDI_TYPE_UN;
    
    
    /*get bidi class for every char, embed level and embed direction*/
    for (ui2_idx = 0; ui2_idx < ui2_w2s_len; ui2_idx++)
    {
        pt_bidi_class[ui2_idx] = get_bidi_class(w2s_source[ui2_idx]);
#ifdef	DIVX_PLUS_CER
        if (ui2_idx == 0 && pt_bidi_class[ui2_idx] == BIDI_TYPE_CS)
        {
            pt_bidi_class[ui2_idx] = BIDI_TYPE_L;
        }
#endif
        if (b_find_level)
        {
			if (pt_bidi_class[ui2_idx] == BIDI_TYPE_R || pt_bidi_class[ui2_idx] == BIDI_TYPE_AL)
			{
				*pui1_embed_level = 1;
				b_find_level = FALSE;
				*pi1_embed_dir = BIDI_TYPE_R;
			}
			else if (pt_bidi_class[ui2_idx] == BIDI_TYPE_L)
			{
				*pui1_embed_level = 0;
				b_find_level = FALSE;
				*pi1_embed_dir = BIDI_TYPE_L;
			}
        }
    }
    /*init free list*/
    if (b_init)
    {
        i4_ret = x_sema_create( &h_free_list_lock,
                            X_SEMA_TYPE_MUTEX,
                            X_SEMA_STATE_UNLOCK );
        if ( i4_ret!=OSR_OK || h_free_list_lock==NULL_HANDLE )
        {
			x_mem_free(pt_bidi_class);
            return NULL;
        }
		DLIST_INIT(&t_free_list);	
		b_init = FALSE;		
    }
   
    /*create sor item*/     			
    pt_item = create_run_item(0,BIDI_TYPE_SOR,0,*pui1_embed_level);	
    if(pt_item == NULL)
    {
    	WGL_MEM_FREE(pt_bidi_class);
		return NULL;
    }
    pt_item_head = pt_item;	
    pt_item_prev = pt_item;	
    
    ui2_len = 0;
    i4_pos = -1;
	
    for (ui2_idx = 0; ui2_idx <= ui2_w2s_len; ui2_idx++)
    {
		if (ui2_idx < ui2_w2s_len)
		{
			t_cur_class =  *(pt_bidi_class + ui2_idx);  
		}
		if (ui2_idx == ui2_w2s_len || t_cur_class != t_prev_class)
     	{    
     		if (i4_pos >= 0 && ui2_len > 0)
     		{     			
				/*create  item*/
	   	   	    pt_item = create_run_item(ui2_len,t_prev_class,(UINT16)i4_pos,*pui1_embed_level); 
	   	   	    if (pt_item == NULL)
	   	   	    {
	   	   	    	pt_item = pt_item_head;
				    while (pt_item != NULL)       
				    {
						pt_item_next = ITEM_NEXT(pt_item);
						free_run_item(pt_item);
						pt_item = pt_item_next;
				    }
      
					WGL_MEM_FREE(pt_bidi_class);	
					return NULL;
	   	   	    }
				ITEM_PREV(pt_item) = pt_item_prev;	
	   	   	    ITEM_NEXT(pt_item_prev) = pt_item;  	   	   	    
	   	   	    pt_item_prev = pt_item;  
				if (t_prev_class >= BIDI_TYPE_B && t_prev_class <= BIDI_TYPE_ON)
  			    {
  				    *pui2_bidi_type |= U_BIDI_NEUTRAL;
  			    }
  			    else if (t_prev_class >= BIDI_TYPE_PDF && t_prev_class <= BIDI_TYPE_BN)
  			    {
					*pui2_bidi_type |= U_BIDI_WEAK;
  			    }  			         	    
     	    }
 	    	ui2_len = 0;
     	    i4_pos = (INT32)ui2_idx;  
     	}
     	t_prev_class = t_cur_class;   
     	ui2_len++;         	
    }   
   
    /*create eor item*/ 	
    pt_item = create_run_item(0,BIDI_TYPE_EOR,ui2_w2s_len,(UINT8)(*pui1_embed_level + 1));  
    if (pt_item == NULL)
	{
		pt_item = pt_item_head;
	    while (pt_item != NULL)       
	    {
			pt_item_next = ITEM_NEXT(pt_item);
			free_run_item(pt_item);
			pt_item = pt_item_next;
	    }
		WGL_MEM_FREE(pt_bidi_class);	
		return NULL;
	}
    ITEM_PREV(pt_item)= pt_item_prev;    
    ITEM_NEXT(pt_item_prev) = pt_item;	
    WGL_MEM_FREE(pt_bidi_class);
    return pt_item_head;
}






static void merge_item_by_type(ITEM_T *pt_item_list)
{
	ITEM_T *pt_item_prev;
	ITEM_T *pt_item;
	ITEM_T *pt_item_next;
	
    for (pt_item =  ITEM_NEXT(pt_item_list); ITEM_NEXT(pt_item);)
    {
    	pt_item_prev = ITEM_PREV(pt_item) ;    	
    	if(ITEM_TYPE(pt_item) > BIDI_TYPE_ON || (pt_item_prev && ITEM_TYPE(pt_item_prev) > BIDI_TYPE_ON))
    	{
    		 pt_item = ITEM_NEXT(pt_item);
			continue;
    	}
        if (pt_item_prev && ITEM_TYPE(pt_item_prev) == ITEM_TYPE(pt_item))
   	    {
            pt_item_next = ITEM_NEXT(pt_item);
   	        ITEM_NEXT(pt_item_prev) = pt_item_next;
   	        ITEM_LEN(pt_item_prev) = ITEM_LEN(pt_item_prev) + ITEM_LEN(pt_item);
   	        free_run_item(pt_item);
   	        if (pt_item_next)
   	        {
   	            ITEM_PREV(pt_item_next) = pt_item_prev;
   	            pt_item = pt_item_next;
   	        }
   	        else
   	        {
				break;
   	        } 	   	       
        } 
        else
        {
			 pt_item = ITEM_NEXT(pt_item);
        }
    }
}


static void merge_item_by_level(ITEM_T *pt_item_list)
{
	ITEM_T *pt_item_prev;
	ITEM_T *pt_item;
	ITEM_T *pt_item_next;
	
    for (pt_item =  ITEM_NEXT(pt_item_list); ITEM_NEXT(pt_item) != NULL; )
    {
    	pt_item_prev = ITEM_PREV(pt_item) ;
    	if(ITEM_TYPE(pt_item) > BIDI_TYPE_ON ||(pt_item_prev && ITEM_TYPE(pt_item_prev) > BIDI_TYPE_ON))
    	{
    		pt_item = ITEM_NEXT(pt_item);
			continue;
    	}
        if (pt_item_prev && ITEM_LEVEL(pt_item_prev) == ITEM_LEVEL(pt_item))
   	    {
            pt_item_next = ITEM_NEXT(pt_item);
   	        ITEM_NEXT(pt_item_prev) = pt_item_next;
   	        ITEM_LEN(pt_item_prev) = ITEM_LEN(pt_item_prev) + ITEM_LEN(pt_item);
   	        free_run_item(pt_item);
   	        if (pt_item_next)
   	        {
   	            ITEM_PREV(pt_item_next) = pt_item_prev;
   	            pt_item = pt_item_next;
   	        }
   	        else
   	        {
				break;
   	        } 	   	       
        }
        else
        {
			pt_item = ITEM_NEXT(pt_item);
        }
    }
}

static void resolving_weak_types (ITEM_T *pt_item_list)
{
	UC_BIDI_CLASS_T ui1_prev_type;
	UC_BIDI_CLASS_T ui1_item_type;
	UC_BIDI_CLASS_T ui1_next_type;
	UINT8 ui1_last_strong ;	
	ITEM_T *pt_item;  
	BOOL  b_merge;
	

	ui1_prev_type = BIDI_TYPE_SOR;
	ui1_next_type = BIDI_TYPE_UN;
	ui1_item_type = BIDI_TYPE_UN;
	ui1_last_strong = BIDI_TYPE_SOR;
	b_merge = FALSE;
	
	for (pt_item = ITEM_NEXT(pt_item_list); ITEM_NEXT(pt_item) != NULL; pt_item = ITEM_NEXT(pt_item))
	{
		
        ui1_prev_type = ui1_item_type;  
        ui1_item_type = ITEM_TYPE(pt_item);
       
        /* Remember the last strong character */
        if ( ui1_item_type == BIDI_TYPE_AL || ui1_item_type == BIDI_TYPE_R  || ui1_item_type == BIDI_TYPE_L )
        {
	         ui1_last_strong = ui1_item_type;
        }
      
        /* W1. Examine each nonspacing mark (NSM) in the level run, and change the type of the NSM to the type 
	                of the previous character. If the NSM is at the start of the level run, it will get the type of sor.*/
        if (ui1_item_type == BIDI_TYPE_NSM)
	    {
		    if (ui1_prev_type == BIDI_TYPE_SOR)
		    {		    	
		    	if(ITEM_LEVEL(ITEM_PREV(pt_item)) % 2 == 0)
		    	{
		        	ITEM_TYPE(pt_item) = BIDI_TYPE_L;   
		        }
		        else
		        {
					ITEM_TYPE(pt_item) = BIDI_TYPE_R;   
		        }
		    }
		    else
		    {
		  	    ITEM_TYPE(pt_item) = ui1_prev_type;
            }
	    }
        /* W2:  If last strong type is AL, change the type of the European number to Arabic number. */
        else if (ITEM_TYPE(pt_item) == BIDI_TYPE_EN && ui1_last_strong == BIDI_TYPE_AL)
        {
			ITEM_TYPE(pt_item) = BIDI_TYPE_AN;
		}
		if (!b_merge && ITEM_TYPE(pt_item)  == ui1_prev_type)
		{
			b_merge = TRUE;
		}
	}

	if (b_merge)
    {
    	merge_item_by_type(pt_item_list);
    }
	b_merge = FALSE;
	
   /*w3 - w7*/
	ui1_prev_type = BIDI_TYPE_SOR;
	ui1_item_type = BIDI_TYPE_SOR; 
	ui1_next_type = BIDI_TYPE_UN; 
	ui1_last_strong = BIDI_TYPE_SOR;
	for (pt_item = ITEM_NEXT(pt_item_list);  ITEM_NEXT(pt_item) != NULL; pt_item = ITEM_NEXT(pt_item))
	{       
        ui1_prev_type = ui1_item_type;        
        ui1_item_type = ITEM_TYPE(pt_item);
        if (ITEM_NEXT(pt_item) != NULL)
		{
        	ui1_next_type = ITEM_TYPE(ITEM_NEXT(pt_item));
        }
        else
        {
			ui1_next_type = BIDI_TYPE_UN; 

        }
        
		/* W3: Change ALs to R */
	    if (ui1_item_type == BIDI_TYPE_AL)
	    {
		    ITEM_TYPE(pt_item) = BIDI_TYPE_R;
		    ui1_last_strong = ui1_item_type;
	    }
        /* Remember the last strong character */
        else if (ui1_item_type == BIDI_TYPE_R  || ui1_item_type == BIDI_TYPE_L)
        {
	         ui1_last_strong = ui1_item_type;
        }

       
        /* W4. 1 A single European separator between two European numbers changes to a European number.*/
        if (ITEM_LEN(pt_item) == 1 && ui1_item_type == BIDI_TYPE_ES) 
		{
			
			if(ui1_prev_type == BIDI_TYPE_EN && ui1_next_type == BIDI_TYPE_EN)
			{
				ITEM_TYPE(pt_item) = BIDI_TYPE_EN;
			}  
		}
		 /* W4.2   A single common separator between two numbers of the same type changes to that type.*/
		else if (ITEM_LEN(pt_item) == 1 && ITEM_TYPE(pt_item) == BIDI_TYPE_CS) 
		{
			
			if(ui1_prev_type == ui1_next_type && ui1_prev_type != BIDI_TYPE_UN)
			{
				ITEM_TYPE(pt_item) = ui1_prev_type;
			}
		  
		}

        /* W5. A sequence of European terminators adjacent to European numbers changes to all European numbers.*/
        if (ITEM_TYPE(pt_item) == BIDI_TYPE_ET)
  	    {
   	        if (ui1_next_type == BIDI_TYPE_EN || ui1_prev_type == BIDI_TYPE_EN)
   	        {
   	            ITEM_TYPE(pt_item) = BIDI_TYPE_EN;
   	        }
  	    }      
       
      	ui1_item_type = ITEM_TYPE(pt_item);
        /* W6. Otherwise, separators and terminators change to Other Neutral *///TODO BIDI_TYPE_B and BIDI_TYPE_S
        if (ui1_item_type == BIDI_TYPE_ET || ui1_item_type == BIDI_TYPE_CS
             || ui1_item_type == BIDI_TYPE_ES)
        {
		    ITEM_TYPE(pt_item) = BIDI_TYPE_ON;
	    }

        /* W7.  If last strong type is L , then change the type of the European number to L. */
        else if (ui1_item_type == BIDI_TYPE_EN && ui1_last_strong == BIDI_TYPE_L)
        {
	    	ITEM_TYPE(pt_item) = BIDI_TYPE_L;
	    }	   
	    if (!b_merge)
        {
		    if (!b_merge && ITEM_TYPE(pt_item) == ui1_prev_type)
			{
				b_merge = TRUE;
			}
		}
    }
    if (b_merge)
    {
    	merge_item_by_type(pt_item_list);
    }
}


static void resolving_neutral_types (ITEM_T *pt_item_list,INT8 i1_embed_dir)
{
	UC_BIDI_CLASS_T ui1_prev_type;
	UC_BIDI_CLASS_T ui1_item_type;
	UC_BIDI_CLASS_T ui1_next_type;
	ITEM_T *pt_item;  	
	BOOL  b_merge;

	ui1_prev_type = BIDI_TYPE_UN;
	ui1_next_type = BIDI_TYPE_UN; 
	ui1_item_type = BIDI_TYPE_UN; 
	b_merge = FALSE;
	
	for (pt_item = ITEM_NEXT(pt_item_list); ITEM_NEXT(pt_item)!= NULL; pt_item = ITEM_NEXT(pt_item))
    {
        
        ui1_prev_type = ui1_item_type;        
        ui1_item_type = ITEM_TYPE(pt_item);
        if (ITEM_NEXT(pt_item) != NULL)
		{
        	ui1_next_type = ITEM_TYPE(ITEM_NEXT(pt_item));
        }
        else
        {
			ui1_next_type = BIDI_TYPE_UN; 

        }

        if (ui1_item_type >= BIDI_TYPE_B && ui1_item_type <= BIDI_TYPE_ON )  
    	{
    	    /* European and Arabic numbers act as if they were R in terms of their influence on neutrals*/    
    	    if (ui1_prev_type == BIDI_TYPE_EN || ui1_prev_type == BIDI_TYPE_AN)
    	    {
    	    	ui1_prev_type = BIDI_TYPE_R;
    	    } 
    	    else if (ui1_prev_type == BIDI_TYPE_SOR || ui1_prev_type == BIDI_TYPE_EOR)
    	    {
				if (ITEM_LEVEL(pt_item)% 2 == 0)
				{
					ui1_prev_type = BIDI_TYPE_L; 
				}
				else
				{
					ui1_prev_type = BIDI_TYPE_R; 
				}
    	    }
    	    if (ui1_next_type == BIDI_TYPE_EN || ui1_next_type == BIDI_TYPE_AN)
    	    {
    	        ui1_next_type = BIDI_TYPE_R;
    	    }
			#if 0
    	    else if (ui1_next_type == BIDI_TYPE_SOR || ui1_next_type == BIDI_TYPE_EOR)
    	    {
				if (ITEM_LEVEL(pt_item)% 2 == 0)
				{
					ui1_next_type = BIDI_TYPE_L; 
				}
				else
				{
					ui1_next_type = BIDI_TYPE_R; 
				}
    	    }
			#endif
    	    /* N1. A sequence of neutrals takes the direction of the surrounding strong text if the text on both sides has the same direction. */
    	    if (ui1_prev_type == ui1_next_type)
    	    {
				 ITEM_TYPE(pt_item)  = ui1_prev_type;
    	    }
			else if ((ui1_next_type > BIDI_TYPE_RLO || ui1_next_type == BIDI_TYPE_EOR)&& 
					ui1_prev_type <= BIDI_TYPE_RLO)
			{
				ITEM_TYPE(pt_item) = ui1_prev_type;
			}
			else if ((ui1_prev_type > BIDI_TYPE_RLO || ui1_prev_type == BIDI_TYPE_SOR)&& 
					ui1_next_type <= BIDI_TYPE_RLO)
			{
				ITEM_TYPE(pt_item) = ui1_next_type;
			}
    	    /* N2. Any remaining neutrals take the embedding direction*/
    	    else 
    	    {
    	        ITEM_TYPE(pt_item) = (UC_BIDI_CLASS_T)i1_embed_dir;
    	    }
    	}
    	
    	if (!b_merge && ITEM_TYPE(pt_item)  == ui1_prev_type)
		{
			b_merge = TRUE;
		}
		
    }

    if (b_merge)
    {
    	merge_item_by_type(pt_item_list);   
    }

}



static void resolving_implicit_level (ITEM_T *pt_item_list,INT8 i1_embed_dir,UINT8 ui1_embed_level,UINT8* pui1_max_level)
{
	ITEM_T *pt_item = NULL;     
    UC_BIDI_CLASS_T ui1_item_type;
   
    
	*pui1_max_level = ui1_embed_level;
	
	ui1_item_type = BIDI_TYPE_UN; 
	
	for (pt_item = ITEM_NEXT(pt_item_list); ITEM_NEXT(pt_item) != NULL; pt_item = ITEM_NEXT(pt_item))
  	{
  		//ui1_prev_type = ui1_item_type;
		ui1_item_type = ITEM_TYPE(pt_item);		
		/* This code should be expanded to handle explicit directions! */
		/* I1. For all characters with an even (left-to-right) embedding direction, those of type R go up one level
		and those of type AN or EN go up two levels.*/
		if (ui1_embed_level % 2 == 0)
		{
		    if (ui1_item_type == BIDI_TYPE_R)
		    {
		        ITEM_LEVEL(pt_item) = (UINT8)(ui1_embed_level + 1);
		     }
		    else if (ui1_item_type == BIDI_TYPE_AN || ui1_item_type == BIDI_TYPE_EN)
		    {
		        ITEM_LEVEL(pt_item) = (UINT8)(ui1_embed_level + 2);
		    }		   
		    else
		    {
		        ITEM_LEVEL(pt_item) = ui1_embed_level;
		    }
		}
		/*I2. For all characters with an odd (right-to-left) embedding direction, those of type L, EN or AN go up one level*/
		else
		{
		    if (ui1_item_type == BIDI_TYPE_L
			|| ui1_item_type == BIDI_TYPE_AN
			|| ui1_item_type == BIDI_TYPE_EN)
			{
		        ITEM_LEVEL(pt_item) = (UINT8)(ui1_embed_level + 1);
		    }		   
		}

        if (ITEM_LEVEL(pt_item) > *pui1_max_level)
        {
            *pui1_max_level= ITEM_LEVEL(pt_item);
        }        
    }
	
}


static void  seperate_string(
               /* input */
		       UTF16_T *w2s_source,
		       UINT16 ui2_max_len,
		       REORDER_ARRTRIBUTE_T* pt_attr,
		       /* output */
               ITEM_T **ppt_item_list,
		       UINT8 *pui1_max_level,
		       INT8  *pi1_embed_dir)
{	
	ITEM_T *pt_item_list;
	ITEM_T *pt_item;
	UINT16  ui2_bidi_type;    
    
    /* group text by bidi class of chars*/
   
   	pt_item_list = get_initial_bidi_runs(w2s_source,
   	                                    ui2_max_len,
   	                                    &pt_attr->ui1_embedding_level,
   	                                    pi1_embed_dir,
   	                                    &ui2_bidi_type,
   	                                    pt_attr->b_find_init_level);
    
    if (pt_item_list == NULL)
    {
		return;
    }  
 	if (ui2_bidi_type & U_BIDI_WEAK)
 	{
    	resolving_weak_types(pt_item_list); 
    }
    else
    {
         /*W3. Change all ALs to R. Need do this because the spec describe this step in resolving weak types but acturally it also change strong types*/
	     for (pt_item = pt_item_list; ITEM_NEXT(pt_item) != NULL; pt_item = ITEM_NEXT(pt_item))
		 {
			  /* W3: Change ALs to R */
			  if (ITEM_TYPE(pt_item) == BIDI_TYPE_AL)
			  {
				   ITEM_TYPE(pt_item) = BIDI_TYPE_R;
			  }
		 }
		 merge_item_by_type(pt_item_list);

    }
    if (ui2_bidi_type & U_BIDI_NEUTRAL)
 	{
        resolving_neutral_types(pt_item_list,*pi1_embed_dir);
    }
    resolving_implicit_level(pt_item_list,*pi1_embed_dir,pt_attr->ui1_embedding_level,pui1_max_level);   
     
    *ppt_item_list = pt_item_list;
   
}


INT32 reorder_text(/* input */
		     UTF16_T *w2s_source,
		     UINT16 ui2_max_len,
		     REORDER_ARRTRIBUTE_T* pt_attr,
		     /* output */
		     UTF16_T *w2s_output,
			 INT8  *pi1_embed_dir
		     )

{
    ITEM_T 	  *pt_item_list = NULL;
    ITEM_T    *pt_item = NULL;
    ITEM_T    *pt_item_next = NULL;
    UINT8 	  ui1_max_level = 0;   
    UINT8     ui1_item_level;
	BOOL 	  has_mirror_char;
	UINT8     level_idx;
	UINT16    ui2_idx;
	UTF16_T   mirrored_ch;      
  
    seperate_string(w2s_source, 
                    ui2_max_len , 
                    pt_attr,
                    /* output */
                    &pt_item_list,
                    &ui1_max_level,
                    pi1_embed_dir);       
    
    /* L4. A character is depicted by a mirrored glyph if and only if (a) the resolved directionality of that character is R, 
              and (b) the Bidi_Mirrored property value of that character is true */
    for (pt_item = pt_item_list; ITEM_NEXT(pt_item) != NULL; pt_item = ITEM_NEXT(pt_item))
    {
    	if (ITEM_TYPE(pt_item) >= BIDI_TYPE_SOR)
    	{
			continue;
    	}
        if (ITEM_LEVEL(pt_item) % 2 == 1)
        {
         
            for (ui2_idx = ITEM_POS(pt_item); ui2_idx < (UINT16)(ITEM_POS(pt_item) + ITEM_LEN(pt_item)); ui2_idx++)
            {
                 has_mirror_char = get_mirror_char(w2s_output[ui2_idx], &mirrored_ch);
                 if (has_mirror_char)
				 {
					  w2s_output[ui2_idx] = mirrored_ch;
				 }
	            
            }
        }
     }


    /* L2. From the highest level found in the text to the lowest odd level on each line, including intermediate levels not actually
           present in the text, reverse any contiguous sequence of characters that are at that level or higher.*/
    merge_item_by_level(pt_item_list);
    for (level_idx = ui1_max_level; level_idx > 0; level_idx--)
    {
        for (pt_item = ITEM_NEXT(pt_item_list); ITEM_NEXT(pt_item)!= NULL; pt_item = ITEM_NEXT(pt_item))
        {
			if (ITEM_TYPE(pt_item) >= BIDI_TYPE_SOR)
			{
				continue;
			}
        	ui1_item_level =  ITEM_LEVEL(pt_item) ;
            if (ui1_item_level == level_idx)
            {					   
			    UINT16 ui2_item_len = (UINT16)ITEM_LEN(pt_item);
			    UINT16 ui2_item_pos = ITEM_POS(pt_item);
			    if (ITEM_LEN(pt_item) > 1)
			    {
			        string_reverse(w2s_output + ui2_item_pos, ui2_item_len);
			    }
			    ITEM_LEVEL(pt_item) = (UINT8)(ui1_item_level - 1);
			    
  			}
  		}
  		merge_item_by_level(pt_item_list);
    }    

	pt_item = pt_item_list;
    while (pt_item != NULL)       
    {
		pt_item_next = ITEM_NEXT(pt_item);
		free_run_item(pt_item);
		pt_item = pt_item_next;
    }
      
	return 0;
  
}


void get_chars_glyph(
                   JOINING_TYPE_T*     pt_join_type,
                   UINT16 ui2_max_len,
                   FORM_TYPE_T* pt_form_type)
{		
	JOINING_TYPE_T   t_r_type;
	JOINING_TYPE_T   t_m_type;	
	JOINING_TYPE_T   t_l_type;	
	INT32 		     i4_idx;	
	INT32 		     i4_left_idx;	

	t_r_type = JOIN_TYPE_N;
	t_m_type = JOIN_TYPE_N;
	t_l_type = JOIN_TYPE_N;

	for (i4_idx = ui2_max_len - 1; i4_idx >= 0; i4_idx--)
    {
		/*Joining Rules 1: Transparent characters do not affect the joining behavior of base (spacing) characters.*/
		if (t_m_type != JOIN_TYPE_T)
		{
    		t_r_type = t_m_type;
    	}
        t_m_type = pt_join_type[i4_idx];
        
        if (i4_idx > 0)
        {
			t_l_type = pt_join_type[i4_idx - 1];
			if (t_l_type == JOIN_TYPE_C)
			{
				t_l_type = JOIN_TYPE_D;
			}
			else if (t_l_type == JOIN_TYPE_T)
			{
				i4_left_idx = i4_idx - 2;
				while (i4_left_idx >= 0)
				{
				    if(pt_join_type[i4_left_idx] !=JOIN_TYPE_T || i4_left_idx == 0)
				    {
						if (pt_join_type[i4_left_idx] == JOIN_TYPE_C)
						{
							t_l_type = JOIN_TYPE_D;
						}
						else
						{
							t_l_type = pt_join_type[i4_left_idx];
						}
				    }
					i4_left_idx--;
				}
			}
        }
        else
        {
			t_l_type = JOIN_TYPE_N; 
        }
        /*select glyph form right join char*/
        if (t_m_type == JOIN_TYPE_R)
        {            
			if (t_r_type == JOIN_TYPE_D)
			{
				/*Joining Rules 2: A right-joining character X that has a right join-causing character on the right will adopt the form Xr .*/
				pt_form_type[i4_idx] = FORM_TYPE_R;
			}
			else
			{
				/*Joining Rules 7:If none of the preceding rules applies to a character X, then it will adopt the nominal form Xn*/
				pt_form_type[i4_idx] = FORM_TYPE_N;
			}
        }
         /*select glyph form dual join char*/
        else if (t_m_type == JOIN_TYPE_D)
        {
			if(t_r_type == JOIN_TYPE_D)
			{
				if(t_l_type == JOIN_TYPE_D || t_l_type == JOIN_TYPE_R)
				{
				    /*Joining Rules 4: A dual-joining character X that has a right join-causing character on the right and a left join-causing character on the left will adopt the form Xm.*/
					pt_form_type[i4_idx] = FORM_TYPE_M;
				}
				else
				{
				    /*Joining Rules 5: A dual-joining character X that has a right join-causing character on the right and no left join-causing character on the left will adopt the form Xr .*/
					pt_form_type[i4_idx] = FORM_TYPE_R;
				}
			}
			else
			{
				if(t_l_type == JOIN_TYPE_D || t_l_type == JOIN_TYPE_R)
				{
				   /*Joining Rules 6: A dual-joining character X that has a left join-causing character on the left and no right join-causing character on the right will adopt the form Xl. For example:*/
					pt_form_type[i4_idx] = FORM_TYPE_L;
				}
				else
				{
					/*both right and left can not joing */
					pt_form_type[i4_idx] = FORM_TYPE_N;
				}
			}
        }
        else
        {
        	/*Joining Rules 7:If none of the preceding rules applies to a character X, then it will adopt the nominal form Xn*/
			pt_form_type[i4_idx] = FORM_TYPE_N;
        }
        /*Join-causing characters are distinguished from the dual-joining characters in that they do not change shape themselves*/
        if (t_m_type == JOIN_TYPE_C)
        {
        	t_m_type = JOIN_TYPE_D;
        }
        
	}
	
}


#ifndef MW_WGL_DISABLE_ARABIC_LIGATURE
static  BOOL search_ligature_table(const LIGATURE_T* pt_ligature, 
									   FORM_TYPE_T t_form, INT32 i4_lgt_len,
                                       UTF16_T w2_char, UTF16_T w2_rchar,
                                       FORMS_T* pt_lgt_forms)
{  
	INT32	  i4_table_idx;		
	
    for (i4_table_idx = 0; i4_table_idx < i4_lgt_len; i4_table_idx++)
    {
    	if (w2_rchar < pt_ligature[i4_table_idx].elements[0] )
    	{			
   			 break;
    	}
        else if (w2_rchar == pt_ligature[i4_table_idx].elements[0])
        {
          if( w2_char < pt_ligature[i4_table_idx].elements[1])
          {
			    break;
          }
          else if( w2_char == pt_ligature[i4_table_idx].elements[1])
          {
			  
          	  ((UTF16_T*)pt_lgt_forms)[(UINT8)t_form] = pt_ligature[i4_table_idx].ligature;
          	  return  TRUE;
          }
        }  
    }
    
	return FALSE;
}
#endif


static  BOOL find_ligature2( UTF16_T w2_char, UTF16_T w2_rchar, FORMS_T* pt_lgt_forms)
{  
#ifdef MW_WGL_DISABLE_ARABIC_LIGATURE
	return FALSE;
#else
    INT32     i4_max_idx;
    BOOL      b_found = FALSE;
  
 	pt_lgt_forms->isolated = WGL_CHAR_NULL;
 	pt_lgt_forms->final = WGL_CHAR_NULL;
 	pt_lgt_forms->medial = WGL_CHAR_NULL;
 	pt_lgt_forms->initial = WGL_CHAR_NULL;
 	
    i4_max_idx = (INT32)(sizeof(isolate_lgt_table)/ sizeof(LIGATURE_T));  	
    b_found |= search_ligature_table(isolate_lgt_table,FORM_TYPE_N, i4_max_idx, w2_char, w2_rchar,pt_lgt_forms);
	i4_max_idx = (INT32)(sizeof(final_lgt_table)/ sizeof(LIGATURE_T));	   
    b_found |= search_ligature_table(final_lgt_table,FORM_TYPE_R, i4_max_idx, w2_char, w2_rchar,pt_lgt_forms);
	/*i4_max_idx = (INT32)(sizeof(medial_lgt_table)/ sizeof(LIGATURE_T));   
    b_found |= search_ligature_table(medial_lgt_table,FORM_TYPE_M, i4_max_idx, w2_char, w2_rchar,pt_lgt_forms);*/
	i4_max_idx = (INT32)(sizeof(initial_lgt_table)/ sizeof(LIGATURE_T)); 
    b_found |= search_ligature_table(initial_lgt_table,FORM_TYPE_L, i4_max_idx, w2_char, w2_rchar,pt_lgt_forms);
   
    return b_found;
#endif
}


VOID select_char_glyph(INT32  i4_idx, UTF16_T* w2s_output, FORM_TYPE_T *pt_form_type)
{
	INT32 i4_form_idx = 0;
	for (i4_form_idx = 0; i4_form_idx <(INT32)( sizeof(forms_table)/sizeof(FORMS_T)); i4_form_idx++)
	{
		if (forms_table[i4_form_idx].isolated >  w2s_output[i4_idx])
		{	
		     break;
		}
	    if (forms_table[i4_form_idx].isolated == w2s_output[i4_idx] )
	    {
	   		if (pt_form_type[i4_idx] != FORM_TYPE_N)
	   		{
				if (pt_form_type[i4_idx] == FORM_TYPE_R)
		   		{
					w2s_output[i4_idx] = forms_table[i4_form_idx].final;
					break;
		   		}
		   		else if (pt_form_type[i4_idx] == FORM_TYPE_M)
		   		{
					w2s_output[i4_idx] = forms_table[i4_form_idx].medial;
					break;
		   		}
		   		else 
		   		{
					w2s_output[i4_idx] = forms_table[i4_form_idx].initial;
					break;
		   		}
	   		}
	   		
	    }
	}

}





INT32 reorder_arabic(UTF16_T* w2s_input,
                        UINT16 ui2_max_len,
                        VOID* pv_obj,
                        UTF16_T* w2s_output,
                        UINT16* pui2_new_len)
{
	
	JOINING_TYPE_T*     pt_join_type;
	FORM_TYPE_T         *pt_form_type;
	FORM_TYPE_T 		*pt_form_dst;
	FORM_TYPE_T 		*pt_form_src;
	JOINING_TYPE_T      t_yjoin_type;	
	JOINING_TYPE_T      t_xjoin_type;
	JOINING_TYPE_T      t_l1join_type;
	JOINING_TYPE_T      t_r1join_type;
	JOINING_TYPE_T      t_left_type;
	FORMS_T             t_ligature;
	INT32				i4_idx;	
	INT32               i4_word_idx;
	UTF16_T             w2_rchar;
	UTF16_T             w2_mchar;	
	UTF16_T				w2_ligature;	
	UTF16_T 			*w2s_dst;
	UTF16_T 			*w2s_src;	
	UINT16 		        ui2_idx;
	UINT16				ui2_out_len;
	UINT16				ui2_len;	
	UINT16				ui2_pgh_len;
	UINT16              ui2_pgh_pos;
	BOOL 				b_found_lgt;
	BOOL 				b_found_word;
	INT8                i1_embed_dir;
	REORDER_ARRTRIBUTE_T* pt_rdattr = NULL;
    
    
	if (w2s_input == NULL || w2s_output == NULL)
	{		
		return WGLR_INV_ARG;
	}
	ui2_len  = (UINT16)x_uc_w2s_strlen(w2s_input);
    if (pv_obj == NULL)
    {
        pt_rdattr = (REORDER_ARRTRIBUTE_T*)WGL_MEM_ALLOC(sizeof(REORDER_ARRTRIBUTE_T));
        if (pt_rdattr == NULL)
        {
            return WGLR_OUT_OF_MEMORY;
        }
#ifdef DIVX_PLUS_CER
        pt_rdattr->b_find_init_level = FALSE;  //[CR334802]
#else
        pt_rdattr->b_find_init_level = TRUE;
#endif
        pt_rdattr->ui1_embedding_level = 0;
    }
    else
    {
        pt_rdattr = (REORDER_ARRTRIBUTE_T* )pv_obj;
    }
    if (ui2_len  == 0)
    {
    	if (pui2_new_len != NULL)
    	{
			*pui2_new_len = 0;
    	}
		if (pv_obj == NULL && pt_rdattr != NULL)
	    {
		    x_mem_free(pt_rdattr);
	    }
        return WGLR_OK;
    }
    else if (ui2_len  == 1)
    {
        *w2s_output = *w2s_input;
        if (pui2_new_len != NULL)
    	{
			*pui2_new_len = 1;
    	}
		if (pv_obj == NULL && pt_rdattr != NULL)
	    {
		    x_mem_free(pt_rdattr);
	    }
        return WGLR_OK;
    }
    if (ui2_max_len < ui2_len )
    {
		ui2_len  = ui2_max_len;
    }  
	

	/*Seperate Paragraph*/
	ui2_pgh_len = 0;
	ui2_pgh_pos = 0;
	i1_embed_dir = -1;
	ui2_len  = (UINT16)x_uc_w2s_strlen(w2s_input);	
	for (i4_idx  = 0; i4_idx < (INT32)ui2_len; i4_idx++)
	{
		ui2_pgh_len++;
		if (w2s_input[i4_idx] == 0x000A ||w2s_input[i4_idx] == 0x000D ||w2s_input[i4_idx] == 0x001C 
			||w2s_input[i4_idx] == 0x001D || w2s_input[i4_idx] == 0x001E ||w2s_input[i4_idx] == 0x0085 
			||w2s_input[i4_idx] == 0x2029 ||  i4_idx == ui2_len - 1)
		{
			wstr_safe_cpy(w2s_input+ ui2_pgh_pos, ui2_pgh_len, w2s_output);
			if (ui2_pgh_len > 1)
			{
				reorder_text(w2s_input + ui2_pgh_pos,
                            ui2_pgh_len,
                            pt_rdattr,
                            w2s_output, 
                            &i1_embed_dir);
			}			
#ifdef DIVX_PLUS_CER
            if (w2s_output[ui2_pgh_len-1] == 0xA)//[CR334802] 
            {
                UINT16	ui2_i;
                for (ui2_i = ui2_pgh_len-1; ui2_i > 0; ui2_i--)
                {
                    w2s_output[ui2_i] = w2s_output[ui2_i-1];
                }
                w2s_output[0] = 0xA;
            }
#endif
			wstr_safe_cpy(w2s_output, ui2_pgh_len, w2s_output + ui2_len - ui2_pgh_pos - ui2_pgh_len);
			ui2_pgh_pos += ui2_pgh_len;
			ui2_pgh_len = 0;
			
		}
		
	}      
    if (pv_obj == NULL && pt_rdattr != NULL)
    {
	    WGL_MEM_FREE(pt_rdattr);
    }
	//reorder_text(w2s_input,ui2_len,w2s_output);
	
	if (ui2_len < 2)
	{
		return WGLR_OK;
	}
	
	ui2_out_len = ui2_len;
	pt_join_type= (JOINING_TYPE_T*)WGL_MEM_ALLOC(sizeof(JOINING_TYPE_T) * ui2_len);
    if (pt_join_type == NULL)
    {
	   return WGLR_OUT_OF_MEMORY;
    }
    x_memset(pt_join_type, 0, sizeof(JOINING_TYPE_T) * ui2_len); 
   
    for (ui2_idx = 0; ui2_idx < ui2_len; ui2_idx++)
    {
	    pt_join_type[ui2_idx] = get_joining_type(w2s_output[ui2_idx]);
    }	


    pt_form_type = (FORM_TYPE_T*)WGL_MEM_ALLOC(sizeof(FORM_TYPE_T) * (ui2_len + 1));
    if (pt_form_type == NULL)
    {
        WGL_MEM_FREE(pt_join_type);
		return WGLR_OUT_OF_MEMORY;
    }
    x_memset(pt_form_type, 0, sizeof(FORM_TYPE_T) * (ui2_len + 1)); 
    
	 get_chars_glyph(pt_join_type,ui2_len,pt_form_type);
    /*step 2. select ligature */    
	
	w2_mchar = WGL_CHAR_NULL;
	w2_rchar = WGL_CHAR_NULL;
	t_yjoin_type = JOIN_TYPE_N;
	t_xjoin_type = JOIN_TYPE_N;
	t_l1join_type = JOIN_TYPE_N;
	t_r1join_type = JOIN_TYPE_N;
	
	
	for (i4_idx  = ui2_len - 1; i4_idx >= 0; i4_idx--)
	{	
		t_r1join_type = t_yjoin_type;
		t_yjoin_type = t_xjoin_type;     	
        t_xjoin_type = pt_join_type[i4_idx];      
        
        if (i4_idx > 0)
        {
			t_l1join_type = pt_join_type[i4_idx - 1];
        }
        else
        {
			t_l1join_type = JOIN_TYPE_N; 
        }       
        
		w2_rchar = w2_mchar;
        w2_mchar = w2s_output[i4_idx];

        if (!is_arabic_char(w2_mchar))
        {
			continue;
        }
        b_found_lgt = FALSE;
        b_found_word = FALSE;
        if (is_arabic_char(w2_rchar))
        {
            if (w2_mchar == 0x0647 && w2_rchar == 0x0644)
            {
            	t_left_type = JOIN_TYPE_N;
            	for (i4_word_idx  = i4_idx; i4_word_idx >= 0; i4_word_idx--)
				{					
					if (pt_join_type[i4_word_idx] == JOIN_TYPE_C)
					{
						t_left_type = JOIN_TYPE_D;
						break;
					}
					else if (pt_join_type[i4_word_idx] != JOIN_TYPE_T)
					{
						t_left_type = pt_join_type[i4_word_idx] ;
						break;
					}
					
				}
				if (t_left_type != JOIN_TYPE_D && t_left_type != JOIN_TYPE_R)
				{
					if (i4_idx <  ui2_len - 3)
					{
						if( w2s_output[i4_idx + 2] == 0x0644 && w2s_output[i4_idx + 3] == 0x0627)
						{
							b_found_word = TRUE;
						}
					}
				}            	
				
            }
            
            if (!b_found_word)
            {
				b_found_lgt = find_ligature2(w2_mchar,w2_rchar, &t_ligature);
			}
        }
        

		if (b_found_word)
		{
			w2s_output[i4_idx] = 0xFDF2;
			/* copy src to dst  */  	
			w2s_dst = &w2s_output[i4_idx + 1]; 
			w2s_src = &w2s_output[i4_idx + 4]; 
			while((*(w2s_dst++) = *(w2s_src++)) != 0) {     
			} 
			pt_form_dst = &pt_form_type[i4_idx + 1]; 
			pt_form_src = &pt_form_type[i4_idx + 4]; 
			while((*(pt_form_dst++) = *(pt_form_src++)) != 0) {     
			} 
			ui2_out_len = (UINT16)(ui2_out_len - 3); 
		}
        else if (b_found_lgt)
        {
        	w2_ligature = WGL_CHAR_NULL;
			
        	if (t_ligature.isolated)	
        	{
        		
        		if(t_r1join_type != JOIN_TYPE_D)
        		{
        			w2_ligature  = t_ligature.isolated;
					if(t_xjoin_type == JOIN_TYPE_D)
					{
						if(t_l1join_type == JOIN_TYPE_D || t_l1join_type == JOIN_TYPE_R)
						{
							w2_ligature  = WGL_CHAR_NULL;
						}
					}
        		}				
				
        	}
        	if (w2_ligature == WGL_CHAR_NULL && t_ligature.final)	
        	{
				if(t_r1join_type == JOIN_TYPE_D)
        		{
        			w2_ligature  = t_ligature.final;
					if(t_xjoin_type == JOIN_TYPE_D)
					{
						if(t_l1join_type != JOIN_TYPE_D && t_l1join_type != JOIN_TYPE_R)
						{
							w2_ligature  = WGL_CHAR_NULL;
						}
					}
        		}
        		else
        		{
					w2_ligature  = WGL_CHAR_NULL;
        		}
        	}        	
			if (w2_ligature == WGL_CHAR_NULL && t_ligature.initial) 
			{
				if(t_r1join_type != JOIN_TYPE_D)
        		{
        			w2_ligature  = t_ligature.initial;					
					if(t_l1join_type != JOIN_TYPE_D && t_l1join_type != JOIN_TYPE_R)
					{
						w2_ligature  = WGL_CHAR_NULL;
					}					
        		}
			}
			if (w2_ligature == WGL_CHAR_NULL && t_ligature.medial) 
			{
				if(t_r1join_type == JOIN_TYPE_D)
        		{
        			w2_ligature  = t_ligature.medial;			
					if(t_l1join_type != JOIN_TYPE_D && t_l1join_type != JOIN_TYPE_R)
					{
						w2_ligature  = WGL_CHAR_NULL;
					}					
        		}
			}
			
			/*use ligature replace char*/
			if (w2_ligature != WGL_CHAR_NULL)
			{
				w2s_output[i4_idx] = w2_ligature;
				/* copy src to dst  */  	
				w2s_dst = &w2s_output[i4_idx + 1]; 
				w2s_src = &w2s_output[i4_idx + 2]; 
				while((*(w2s_dst++) = *(w2s_src++)) != 0) {     
				} 
				pt_form_dst = &pt_form_type[i4_idx + 1]; 
				pt_form_src = &pt_form_type[i4_idx + 2]; 
				while((*(pt_form_dst++) = *(pt_form_src++)) != 0) {     
				} 
				ui2_out_len--; 
			}
			else
			{
				select_char_glyph(i4_idx,w2s_output,pt_form_type);    
			}

        } 
        else
        {

            if (w2s_output[i4_idx] == 0x0647 && pt_form_type[i4_idx] == FORM_TYPE_N)
            {
                /* 0x0647 at the string head*/
                if ((i4_idx == ui2_len - 1) && (i4_idx - 1 >= 0 ))
                {
                    /* if the fllow character is an arabic character 0x0647 --> 0xFEE9*/
                    if (is_arabic_char(w2s_output[i4_idx - 1]))
                    {
                        w2s_output[i4_idx] = 0xFEE9;
                    }
                }
                /*0x0647 in the middle of the string*/
                else if ((i4_idx < ui2_len - 1 )&&(i4_idx - 1 >=0 ))
                {
                    /* if the fllow character or front character is an arabic character 0x0647 --> 0xFEE9*/
                    if (is_arabic_char(w2s_output[i4_idx + 1]) || is_arabic_char(w2s_output[i4_idx - 1]))
                    {
                        w2s_output[i4_idx] = 0xFEE9;
                    }
                }
                /*0x0647 in the final of the string*/
                else if (i4_idx == 0 && (i4_idx < ui2_len - 1 ))
                {
                    /*if the front character is an arabic character 0x0647 --> 0xFEE9*/
                    if (is_arabic_char(w2s_output[i4_idx + 1]))
                    {
                        w2s_output[i4_idx] = 0xFEE9;
                    }
                }
                /*else no change the ox0647*/
            }        
            
        	select_char_glyph(i4_idx,w2s_output,pt_form_type);    	    
			
        }
		
	}
	if(pui2_new_len != NULL)
	{
		*pui2_new_len = ui2_out_len;
	}
	WGL_MEM_FREE(pt_join_type);
	WGL_MEM_FREE(pt_form_type);	
		
	return WGLR_OK;
}


