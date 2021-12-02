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
#include "inc/common.h"
#include "os/inc/x_os.h"
#include "xml_debug.h"
#include "xml_sbuf.h"

/*-----------------------------------------------------------------------------
 * Name: xml_sbuf_alloc
 * Description: Allocate a new string buffer.
 * Inputs: 
 *        N/A 
 * Outputs: 
 *       N/A 
 * Returns: 
 *      new object of xml_sbuf
 ----------------------------------------------------------------------------*/
XML_SBUF_T * xml_sbuf_alloc (VOID)
{
    XML_SBUF_T *pt_new_sbuf;
    pt_new_sbuf = (XML_SBUF_T *) x_mem_alloc (sizeof (XML_SBUF_T));
	
    XML_ASSERT(pt_new_sbuf, ("pt_new_sbuf = %p\n",pt_new_sbuf));

	x_memset(pt_new_sbuf, 0, sizeof (XML_SBUF_T));

	return pt_new_sbuf;
}

/*-----------------------------------------------------------------------------
 * Name: xml_sbuf_free
 * Description: Free string buffer
 * Inputs: 
 *        pt_xml_sbuf, buffer will be released 
 * Outputs: 
 *       N/A 
 * Returns: 
 *      N/A
 ----------------------------------------------------------------------------*/
VOID  xml_sbuf_free (XML_SBUF_T * pt_xml_sbuf)
{
    XML_ASSERT(pt_xml_sbuf, ("pt_xml_sbuf  = %p\n", pt_xml_sbuf));

    if (pt_xml_sbuf->ps_buf) 
    {
    	x_mem_free (pt_xml_sbuf->ps_buf);
    }
    x_mem_free (pt_xml_sbuf);
}

/*-----------------------------------------------------------------------------
 * Name: xml_sbuf_free_buffer
 * Description: Free buffer only.
 * Inputs: 
 *        pt_xml_sbuf, ps_buf of buffer will be released 
 * Outputs: 
 *       N/A 
 * Returns: 
 *      N/A
 ----------------------------------------------------------------------------*/
VOID xml_sbuf_free_buffer (XML_SBUF_T *pt_xml_sbuf)
{
    XML_ASSERT(pt_xml_sbuf, ("pt_xml_sbuf = %p\n", pt_xml_sbuf));

    if (pt_xml_sbuf->ps_buf)
    {
    	x_mem_free (pt_xml_sbuf->ps_buf);
    	pt_xml_sbuf->ps_buf = NULL;
    }
    pt_xml_sbuf->ui4_size = 0;
    pt_xml_sbuf->ui4_alloc = 0;
}

/*-----------------------------------------------------------------------------
 * Name: xml_sbuf_init
 * Description: Init string buffer
 * Inputs: 
 *        pt_xml_sbuf, string buffer will be initialized
 * Outputs: 
 *       N/A 
 * Returns: 
 *      N/A
 ----------------------------------------------------------------------------*/
VOID
xml_sbuf_init (XML_SBUF_T *pt_xml_sbuf)
{
    XML_ASSERT(pt_xml_sbuf, ("pt_xml_sbuf = %p\n", pt_xml_sbuf));

    pt_xml_sbuf->ps_buf = NULL;
    pt_xml_sbuf->ui4_size = 0;
    pt_xml_sbuf->ui4_alloc = 0;
}

/*-----------------------------------------------------------------------------
 * Name: xml_sbuf_reset
 * Description: reset string buffer
 * Inputs: 
 *        pt_xml_sbuf, string buffer will be reseted
 * Outputs: 
 *       N/A 
 * Returns: 
 *      N/A
 ----------------------------------------------------------------------------*/
VOID
xml_sbuf_reset (XML_SBUF_T *pt_xml_sbuf)
{
    pt_xml_sbuf->ui4_size = 0;
}

/*-----------------------------------------------------------------------------
 * Name: xml_sbuf_ensure
 * Description: Make it sure that buffer has enough size. 
 * Inputs: 
 *        pt_xml_sbuf, string buffer 
 *        ui4_new_size, 
 * Outputs: 
 *       N/A 
 * Returns: 
 *      0
 ----------------------------------------------------------------------------*/
INT32
xml_sbuf_ensure (XML_SBUF_T *pt_xml_sbuf, UINT32 ui4_new_size)
{
    UINT32  ui4_req_size;
    UINT32  ui4_alloc_size;
    
    XML_ASSERT(pt_xml_sbuf, ("pt_xml_sbuf = %p\n", pt_xml_sbuf));
  
    ui4_req_size = ui4_new_size + 1;

    if (ui4_req_size > pt_xml_sbuf->ui4_alloc) 
    {
    	CHAR  *ps_buf;

    	if (pt_xml_sbuf->ui4_alloc) 
    	{
         	ui4_alloc_size = pt_xml_sbuf->ui4_alloc;
    	} 
    	else 
    	{
      		ui4_alloc_size = XML_SBUF_ALLOC_SIZE;
    	}
    	while (ui4_req_size > ui4_alloc_size) 
    	{
         	ui4_alloc_size *= 2;
        }
    	if (pt_xml_sbuf->ps_buf) 
    	{
      		ps_buf = (CHAR *) x_mem_realloc (pt_xml_sbuf->ps_buf, ui4_alloc_size);
                                    
    	} 
    	else 
    	{
         	ps_buf = (CHAR *) x_mem_alloc (ui4_alloc_size);
    	}
    	if (! ps_buf) 
    	{
        	 return -1;
    	}
    	pt_xml_sbuf->ui4_alloc = ui4_alloc_size;
       pt_xml_sbuf->ps_buf = ps_buf;
  	}
  	return 0;
}

/*-----------------------------------------------------------------------------
 * Name: xml_sbuf_add
 * Description: Add specified buffer and length to string buffer.
 * Inputs: 
 *        pt_xml_sbuf, string buffer 
 *        ps_buf,     buffer
 *        ui4_len,    length of ps_buf
 * Outputs: 
 *       N/A 
 * Returns: 
 *      0
 ----------------------------------------------------------------------------*/
INT32
xml_sbuf_add (XML_SBUF_T *pt_xml_sbuf, const CHAR *ps_buf, UINT32  ui4_len)
{
  	INT32  i4_ret;
    
    XML_ASSERT(pt_xml_sbuf, ("pt_xml_sbuf = %p\n", pt_xml_sbuf));
    XML_ASSERT(ps_buf, ("ps_buf = %p\n", ps_buf));
   // XML_ASSERT((ui4_len >= 0), ("ui4_len = %d\n", ui4_len));

  /* Size check. */
  	i4_ret = xml_sbuf_ensure (pt_xml_sbuf, pt_xml_sbuf->ui4_size + ui4_len);

  /* Out of memory.  */
    if (i4_ret < 0) 
    {
    	return -1;
    }
  /* Add the string.  */
    x_memcpy (pt_xml_sbuf->ps_buf + pt_xml_sbuf->ui4_size, ps_buf, ui4_len);
    pt_xml_sbuf->ui4_size += ui4_len;
    pt_xml_sbuf->ps_buf[pt_xml_sbuf->ui4_size] = '\0';

    return 0;
}

/*-----------------------------------------------------------------------------
 * Name: xml_sbuf_add_char
 * Description: Add character to string buffer. 
 * Inputs: 
 *        pt_xml_sbuf, string buffer 
 *        ch_c,        character
 * Outputs: 
 *       N/A 
 * Returns: 
 *      0
 ----------------------------------------------------------------------------*/
INT32
xml_sbuf_add_char (XML_SBUF_T *pt_xml_sbuf, CHAR ch_c)
{
    XML_ASSERT(pt_xml_sbuf, ("pt_xml_sbuf = %p\n", pt_xml_sbuf));

    if (xml_sbuf_ensure (pt_xml_sbuf, pt_xml_sbuf->ui4_size + 1) < 0) 
    {
    	return -1;
    }
    pt_xml_sbuf->ps_buf[pt_xml_sbuf->ui4_size] = ch_c;
    pt_xml_sbuf->ui4_size++;
  
    return 0;
}

/*-----------------------------------------------------------------------------
 * Name: xml_copy_escape
 * Description: Copy character with XML escape. 
 * Inputs: 
 *        pt_xml_sbuf, string buffer 
 *        ps_p,        characters
 *        ui4_len,     length of ps_p
 * Outputs: 
 *       N/A 
 * Returns: 
 *       N/A
 ----------------------------------------------------------------------------*/
static VOID
xml_copy_escape (XML_SBUF_T *pt_xml_sbuf, CHAR  *ps_p, UINT32 ui4_len)
{
    UINT32 ui4_i;
    
    XML_ASSERT(pt_xml_sbuf, ("pt_xml_sbuf = %p\n", pt_xml_sbuf));
    XML_ASSERT(ps_p, ("ps_p = %p\n", ps_p));

    if (! ps_p) 
    {
    	return;
  	}
  
    for (ui4_i = 0; ui4_i < ui4_len; ui4_i++) 
    {
    	switch (ps_p[ui4_i]) 
    	{
    	case '<':
      		xml_sbuf_add_str (pt_xml_sbuf, (CHAR *) "&lt;");
      		break;

    	case '>':
      		xml_sbuf_add_str (pt_xml_sbuf, (CHAR *) "&gt;");
      		break;

   		case '&':
#ifdef HAVE_DTCP
	 		if (ui4_len - ui4_i > 5) 
	 		{
        		if (x_strncmp(&ps_p[ui4_i+1], (CHAR *) "quot;", 5) == 0) 
        		{
	      			xml_sbuf_add_str (pt_xml_sbuf, (CHAR *) "&amp;");
          			break;
	    		}
	 		}
#endif /* HAVE_DTCP */
      		xml_sbuf_add_str (pt_xml_sbuf, (CHAR *) "&amp;amp;");
      		break;

    	case '\'':
      		xml_sbuf_add_str (pt_xml_sbuf, (CHAR *) "&apos;");
      		break;

    	case '\"':
      		xml_sbuf_add_str (pt_xml_sbuf, (CHAR *) "&quot;");
      		break;

    	default:
      		xml_sbuf_add_char (pt_xml_sbuf, ps_p[ui4_i]);
      		break;
    }
  }
}

/*-----------------------------------------------------------------------------
 * Name: xml_sbuf_add_escape
 * Description: Add the buffer with XML escape.
 * Inputs: 
 *        pt_xml_sbuf, string buffer 
 *        ps_buf,       
 * Outputs: 
 *       N/A 
 * Returns: 
 *       0: ok
 *       -1: failed
 ----------------------------------------------------------------------------*/
INT32
xml_sbuf_add_escape (XML_SBUF_T *pt_xml_sbuf, CHAR *ps_buf)
{
    INT32  i4_ret;
    UINT32 ui4_len;
    
    XML_ASSERT(pt_xml_sbuf, ("pt_xml_sbuf = %p\n", pt_xml_sbuf));
    XML_ASSERT(ps_buf, ("ps_buf = %p\n", ps_buf));

    ui4_len = x_strlen (ps_buf);

  /* Size check. */
    i4_ret = xml_sbuf_ensure (pt_xml_sbuf, pt_xml_sbuf->ui4_size + (ui4_len * 4));

  /* Out of memory.  */
  	if (i4_ret < 0) 
  	{
    	return -1;
    }
  /* Add the string.  */
    xml_copy_escape (pt_xml_sbuf, ps_buf, ui4_len);

   pt_xml_sbuf->ps_buf[pt_xml_sbuf->ui4_size] = '\0';

    return 0;
}

/*-----------------------------------------------------------------------------
 * Name: xml_sbuf_add_str
 * Description: Add string
 * Inputs: 
 *        pt_xml_sbuf, string buffer 
 *        ps_str,   string      
 * Outputs: 
 *       N/A 
 * Returns: 
 *       0
 ----------------------------------------------------------------------------*/
INT32
xml_sbuf_add_str (XML_SBUF_T *pt_xml_sbuf, const CHAR *ps_str)
{
    XML_ASSERT(pt_xml_sbuf, ("pt_xml_sbuf = %p\n", pt_xml_sbuf));
    XML_ASSERT(ps_str, ("ps_str = %p\n", ps_str));
  
    return xml_sbuf_add (pt_xml_sbuf, ps_str, x_strlen (ps_str));
}

/*-----------------------------------------------------------------------------
 * Name: xml_sbuf_add_num
 * Description: Add number. 
 * Inputs: 
 *        pt_xml_sbuf, string buffer 
 *        ui4_num,   number      
 * Outputs: 
 *       N/A 
 * Returns: 
 *       0
 ----------------------------------------------------------------------------*/
INT32
xml_sbuf_add_num (XML_SBUF_T *pt_xml_sbuf, UINT32 ui4_num)
{
    CHAR pac_buf[XML_SBUF_NUM_BUFFER_SIZE];

    XML_ASSERT(pt_xml_sbuf, ("pt_xml_sbuf = %p\n", pt_xml_sbuf));

    x_snprintf (pac_buf, XML_SBUF_NUM_BUFFER_SIZE, (CHAR *) "%d", ui4_num);

    return xml_sbuf_add (pt_xml_sbuf, pac_buf, x_strlen (pac_buf));
}


/*-----------------------------------------------------------------------------
 * Name: xml_sbuf_insert
 * Description: Insert the buffer to specified index. 
 * Inputs: 
 *        pt_xml_sbuf, string buffer 
 *        ps_buf,
 *        ui4_len,
 *        ui4_index,
 * Outputs: 
 *       N/A 
 * Returns: 
 *       0
 ----------------------------------------------------------------------------*/
INT32
xml_sbuf_insert (XML_SBUF_T *pt_xml_sbuf, CHAR *ps_buf, UINT32 ui4_len, UINT32 ui4_index)
{
    INT32  i4_ret;

    XML_ASSERT(pt_xml_sbuf, ("pt_xml_sbuf = %p\n", pt_xml_sbuf));
    XML_ASSERT(ps_buf, ("ps_buf = %p\n", ps_buf));

    if (ui4_index > pt_xml_sbuf->ui4_size) 
    {
    	return -2;
  	}

    i4_ret = xml_sbuf_ensure (pt_xml_sbuf, pt_xml_sbuf->ui4_size + ui4_len);
    if (i4_ret < 0) 
    {
    	return i4_ret;
    }
    x_memmove (pt_xml_sbuf->ps_buf + ui4_index + ui4_len, pt_xml_sbuf->ps_buf + ui4_index, pt_xml_sbuf->ui4_size - ui4_index);
    x_memcpy (pt_xml_sbuf->ps_buf + ui4_index, ps_buf, ui4_len);
    pt_xml_sbuf->ui4_size += ui4_len;
    pt_xml_sbuf->ps_buf[pt_xml_sbuf->ui4_size] = '\0';

    return 0;
}

/*-----------------------------------------------------------------------------
 * Name: xml_sbuf_assign
 * Description: Assing the buffer to the string buffer.
 * Inputs: 
 *        pt_xml_sbuf, string buffer 
 *        ps_buf,
 *        ui4_len,
 * Outputs: 
 *       N/A 
 * Returns: 
 *       0
 ----------------------------------------------------------------------------*/
INT32
xml_sbuf_assign (XML_SBUF_T *pt_xml_sbuf, CHAR *ps_buf, UINT32 ui4_len)
{
    INT32 i4_ret;

  	XML_ASSERT(pt_xml_sbuf, ("pt_xml_sbuf = %p\n", pt_xml_sbuf));
    //XML_ASSERT((ui4_len >= 0), ("ui4_len = %d\n", ui4_len));

    if (! ps_buf) 
    {
    	xml_sbuf_free_buffer (pt_xml_sbuf);
        return 0;
    }

    i4_ret = xml_sbuf_ensure (pt_xml_sbuf, ui4_len);
    if (i4_ret != 0) 
    {
    	return i4_ret;
    }
    x_memcpy (pt_xml_sbuf->ps_buf, ps_buf, ui4_len);
    pt_xml_sbuf->ui4_size = ui4_len;
    pt_xml_sbuf->ps_buf[pt_xml_sbuf->ui4_size] = '\0';

    return 0;
}

/*-----------------------------------------------------------------------------
 * Name: xml_sbuf_assign_str
 * Description: Assing the string to the string buffer.
 * Inputs: 
 *        pt_xml_sbuf, string buffer 
 *        ps_str,
 * Outputs: 
 *       N/A 
 * Returns: 
 *       0
 ----------------------------------------------------------------------------*/
INT32
xml_sbuf_assign_str (XML_SBUF_T *pt_xml_sbuf, CHAR *ps_str)
{
    XML_ASSERT(pt_xml_sbuf, ("pt_xml_sbuf = %p\n", pt_xml_sbuf));
    //XML_ASSERT(ps_str, ("ps_str = %p\n", ps_str));

    return xml_sbuf_assign (pt_xml_sbuf, ps_str, x_strlen (ps_str));
}

/*-----------------------------------------------------------------------------
 * Name: xml_sbuf_delete
 * Description: Delete string. 
 * Inputs: 
 *        pt_xml_sbuf, string buffer 
 *        ui4_index,
 *        ui4_len,
 * Outputs: 
 *       N/A 
 * Returns: 
 *       0
 ----------------------------------------------------------------------------*/
VOID
xml_sbuf_delete (XML_SBUF_T *pt_xml_sbuf, UINT32 ui4_index, UINT32 ui4_len)
{
    XML_ASSERT(pt_xml_sbuf, ("pt_xml_sbuf = %p\n", pt_xml_sbuf));
   // XML_ASSERT((ui4_len >= 0), ("ui4_len = %d\n", ui4_len));

    if (! pt_xml_sbuf->ui4_size) 
    {
   	    return;
    }

    if ((ui4_index + ui4_len) > pt_xml_sbuf->ui4_size) 
    {
        ui4_len = pt_xml_sbuf->ui4_size - ui4_index;
    } 
    else 
    {
    	x_memmove (pt_xml_sbuf->ps_buf + ui4_index, pt_xml_sbuf->ps_buf + ui4_index + ui4_len,
                  pt_xml_sbuf->ui4_size - (ui4_index + ui4_len));
     }
    pt_xml_sbuf->ui4_size -= ui4_len;
    pt_xml_sbuf->ps_buf[pt_xml_sbuf->ui4_size] = '\0';
}

/*-----------------------------------------------------------------------------
 * Name: xml_sbuf_detach
 * Description: Detach string from string buffer.
 * Inputs: 
 *        pt_xml_sbuf, string buffer 
 * Outputs: 
 *       N/A 
 * Returns: 
 *       string
 ----------------------------------------------------------------------------*/
CHAR *
xml_sbuf_detach (XML_SBUF_T *pt_xml_sbuf)
{
    CHAR *ps_buf;

    XML_ASSERT(pt_xml_sbuf, ("pt_xml_sbuf = %p\n", pt_xml_sbuf));

    ps_buf = pt_xml_sbuf->ps_buf;

    xml_sbuf_init (pt_xml_sbuf);

    return ps_buf;
}

/*-----------------------------------------------------------------------------
 * Name: xml_sbuf_attach
 * Description: Attach string to the string buffer. 
 * Inputs: 
 *        pt_xml_sbuf, string buffer
 *        ps_buf,
 *        ui4_size,
 * Outputs: 
 *       N/A 
 * Returns: 
 *       N/A
 ----------------------------------------------------------------------------*/
VOID
xml_sbuf_attach (XML_SBUF_T *pt_xml_sbuf, CHAR *ps_buf, UINT32  ui4_size)
{
    XML_ASSERT(pt_xml_sbuf, ("pt_xml_sbuf = %p\n", pt_xml_sbuf));

    xml_sbuf_free_buffer (pt_xml_sbuf);
    pt_xml_sbuf->ps_buf = ps_buf;
    pt_xml_sbuf->ui4_size = ui4_size;
    pt_xml_sbuf->ui4_alloc = ui4_size;
}


