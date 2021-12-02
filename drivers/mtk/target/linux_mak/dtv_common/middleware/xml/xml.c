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
#include "xml_list.h"
#include "xml_sbuf.h"
#include "xml.h"

static INT32 _xml_isspace (INT32  in_char)
{
	return (in_char ==  ' ' ||
		    in_char ==  '\f'||
		    in_char ==  '\n'||
		    in_char ==  '\r'||
		    in_char ==  '\t'|| 
		    in_char ==  '\v');
}

static BOOL _is_valid_value(CHAR *ps_in_val)
{
    UINT32 ui4_i;
    UINT32 ui4_len;
  
    XML_ASSERT(ps_in_val, ("ps_in_val = %p\n", ps_in_val));
  
    ui4_len = x_strlen((CHAR *)ps_in_val);
    for (ui4_i = 0;ui4_i < ui4_len;ui4_i++) 
	{
	   if (!_xml_isspace(ps_in_val[ui4_i])) 
	   {
		  return TRUE;
	   }
    }
	return FALSE;
}

  
/*-----------------------------------------------------------------------------
 * Name: x_xml_new_element
 * Description:  Allocate new XML.
 * Inputs:  
 *          ps_ele_name, name of xml element
 *          ps_ele_value, xml element content
 * Outputs: 
 *          N/A
 * Returns: 
 *          new xml element
 ----------------------------------------------------------------------------*/
XML_T *x_xml_new_element (const CHAR *ps_ele_name, const CHAR *ps_ele_value)
{
	XML_T *pt_new_ele;

#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif

	XML_ASSERT(ps_ele_name, ("ps_ele_name = %p\n", ps_ele_name));

	pt_new_ele = (XML_T *) x_mem_alloc (sizeof (XML_T));
	if (pt_new_ele) 
	{
		x_memset(pt_new_ele, 0, sizeof(XML_T));
		pt_new_ele->pt_parent = NULL;
		pt_new_ele->ps_name = x_strdup (ps_ele_name);
		pt_new_ele->ps_value = x_strdup (ps_ele_value ? ps_ele_value : (CHAR *) "");
		if (!pt_new_ele->ps_name || !pt_new_ele->ps_value) 
		{
			goto failed;
		}
	}
	else
    {
        return NULL;  
    }	     
  
	/*init list*/
	 xml_ListInit(&pt_new_ele->child_ele_list);
	 xml_ListInit(&pt_new_ele->attrlist);
  return pt_new_ele;

failed:
    if (pt_new_ele) 
    {
        if (pt_new_ele->ps_name) 
	    {
            x_mem_free(pt_new_ele->ps_name);
        }
        if (pt_new_ele->ps_value) 
	    {
            x_mem_free(pt_new_ele->ps_value);
        }
        x_mem_free(pt_new_ele);
     }
  return NULL;
}

/*-----------------------------------------------------------------------------
 * Name: x_xml_add_element
 * Description:   Add new child XML element to specified parent element.
 * Inputs: 
 * 			pt_xml_ele,  parent xml element 
 *          ps_ele_name, name of child xml element
 *          ps_ele_value, child xml element content
 * Outputs: 
 *          N/A
 * Returns: 
 *          child xml element
 ----------------------------------------------------------------------------*/
XML_T * x_xml_add_element (XML_T *pt_xml_ele, const CHAR *ps_ele_name, const CHAR *ps_ele_value)
{
	XML_T *pt_new_ele;
	
#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif

	XML_ASSERT(pt_xml_ele, ("pt_xml_ele = %p\n", pt_xml_ele));
	XML_ASSERT(ps_ele_name, ("ps_ele_name = %p\n", ps_ele_name));
	
	pt_new_ele = x_xml_new_element (ps_ele_name, ps_ele_value);
	if (pt_new_ele) 
	{
		pt_new_ele->pt_parent = pt_xml_ele;
		xml_ListAdd(&pt_xml_ele->child_ele_list, pt_new_ele);
	}
	return pt_new_ele;
}

/*-----------------------------------------------------------------------------
 * Name: x_xml_delete_element
 * Description:   Delete specfied element from the xml document.
 * Inputs: 
 * 			pt_xml_ele,  element will be deleted
 * Outputs: 
 *          N/A
 * Returns: 
 *          pt_xml_ele
 ----------------------------------------------------------------------------*/
XML_T *x_xml_delete_element (XML_T *pt_xml_ele)
{
#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif

	XML_ASSERT(pt_xml_ele, ("pt_xml_ele = %p\n", pt_xml_ele));
	
	if (pt_xml_ele->pt_parent) 
	{
		xml_ListDelete (&pt_xml_ele->pt_parent->child_ele_list, pt_xml_ele);
		pt_xml_ele->pt_parent = NULL;
	}
	return pt_xml_ele;
}

/*-----------------------------------------------------------------------------
 * Name: _xml_free_element
 * Description:   free xml element .
 * Inputs: 
 * 			pt_xml_ele,  element will be destroyed
 * Outputs: 
 *          N/A
 * Returns: 
 *          N/A
 ----------------------------------------------------------------------------*/
 static VOID  _xml_free_element (XML_T *pt_xml_ele)
{
#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif

	XML_ASSERT(pt_xml_ele, ("pt_xml_ele = %p\n", pt_xml_ele));
	
	/* Remove from the parenet doc.  */
	x_xml_delete_element(pt_xml_ele);
	
	if (pt_xml_ele->ps_name) 
	{
		x_mem_free (pt_xml_ele->ps_name);
	}
	if (pt_xml_ele->ps_value) 
	{
		x_mem_free (pt_xml_ele->ps_value);
	}
	x_mem_free (pt_xml_ele);
}

/*-----------------------------------------------------------------------------
 * Name: x_xml_new_attribute
 * Description:   Constructor of XML_Attribute.
 * Inputs: 
 *  		ps_attr_name,  name of attribute
 *  		ps_attr_value, value of attribute
 * Outputs: 
 *          N/A
 * Returns: 
 *          new xml attribute
 ----------------------------------------------------------------------------*/
XML_ATTRIBUTE_T * x_xml_new_attribute (const CHAR *ps_attr_name, const CHAR *ps_attr_value)
{
	XML_ATTRIBUTE_T * pt_new_attr;
	
#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif

	XML_ASSERT(ps_attr_name, ("ps_attr_name = %p\n", ps_attr_name));
	
	pt_new_attr = (XML_ATTRIBUTE_T *) x_mem_alloc (sizeof (XML_ATTRIBUTE_T));
	if (pt_new_attr) 
	{
		x_memset(pt_new_attr, 0, sizeof(XML_ATTRIBUTE_T));

		pt_new_attr->ps_name = x_strdup (ps_attr_name);
		pt_new_attr->ps_value = x_strdup (ps_attr_value != NULL ? ps_attr_value : (CHAR *)"");

		if (!pt_new_attr->ps_name || !pt_new_attr->ps_value) 
		{
			goto failed;
		}
	}
	else
	{
	    return NULL;
	}
	
	return pt_new_attr;
	
failed:
	x_dbg_stmt("run here\n\n");
	if (pt_new_attr) 
	{
		if (pt_new_attr->ps_name) 
		{
			x_mem_free(pt_new_attr->ps_name);
		}
		if (pt_new_attr->ps_value) 
		{
			x_mem_free(pt_new_attr->ps_value);
		}
		x_mem_free(pt_new_attr);
	}
	return NULL;
}

/*-----------------------------------------------------------------------------
 * Name: x_xml_add_attribute
 * Description:   Add new XML attribute to specified xml element.
 * Inputs: 
 *  		pt_xml_ele,    xml element
 * 			ps_attr_name,  name of new xml attribute
 *  		ps_attr_value, value of new xml attribute
 * Outputs: 
 *          N/A
 * Returns: 
 *          new xml attribute
 ----------------------------------------------------------------------------*/
XML_ATTRIBUTE_T * x_xml_add_attribute (XML_T *pt_xml_ele, const CHAR *ps_attr_name, const CHAR *ps_attr_value)
{
	XML_ATTRIBUTE_T *pt_new_attr;

#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif

	XML_ASSERT(pt_xml_ele, ("pt_xml_ele = %p\n", pt_xml_ele));
	XML_ASSERT(ps_attr_name, ("ps_attr_name = %p\n", ps_attr_name));
	
	if (! pt_xml_ele) 
	{
		return NULL;
	}
	
	pt_new_attr = x_xml_new_attribute(ps_attr_name, ps_attr_value);
	if (pt_new_attr) 
	{		
		xml_ListAdd (&pt_xml_ele->attrlist, pt_new_attr);
	}
	return pt_new_attr;
}

/*-----------------------------------------------------------------------------
 * Name: x_xml_delete_attribute
 * Description:   delete xml attribute from specified xml element.
 * Inputs: 
 *  		pt_xml_ele,    xml element
 * 			pt_xml_attr,   xml attribute
 * Outputs: 
 *          N/A
 * Returns: 
 *          N/A
 ----------------------------------------------------------------------------*/
VOID  x_xml_delete_attribute(XML_T *pt_xml_ele, XML_ATTRIBUTE_T  *pt_xml_attr)
{
#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif

	XML_ASSERT(pt_xml_ele, ("pt_xml_ele = %p\n", pt_xml_ele));
	XML_ASSERT(pt_xml_attr, ("pt_xml_attr = %p\n", pt_xml_attr));
	
	xml_ListDelete (&pt_xml_ele->attrlist, pt_xml_attr);
}

/*-----------------------------------------------------------------------------
 * Name: x_xml_free_attribute
 * Description:   free xml attribute.
 * Inputs: 
 * 			pt_xml_attr,   xml attribute
 * Outputs: 
 *          N/A
 * Returns: 
 *          N/A
 ----------------------------------------------------------------------------*/
VOID x_xml_free_attribute(XML_ATTRIBUTE_T *pt_xml_attr)
{
#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif

	XML_ASSERT(pt_xml_attr, ("pt_xml_attr = %p\n", pt_xml_attr));

	if (pt_xml_attr->ps_name) 
	{
		x_mem_free (pt_xml_attr->ps_name);
	}
	if (pt_xml_attr->ps_value) 
	{
		x_mem_free (pt_xml_attr->ps_value);
	}
	x_mem_free (pt_xml_attr);
}

/*-----------------------------------------------------------------------------
 * Name: _xml_new_node
 * Description:   Allocate new XML node.
 * Inputs: 
 * 			ui4_size,   alloced size assign to index of xml node 
 * Outputs: 
 *          N/A
 * Returns: 
 *         new xml node
 ----------------------------------------------------------------------------*/
static XML_NODE_T *  _xml_new_node(UINT32 ui4_size)
{
	XML_NODE_T  *pt_new_node;
	
#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif
	
	if (ui4_size < XML_NODE_INIT_SIZE) 
	{
		ui4_size = XML_NODE_INIT_SIZE;
	}
	
	pt_new_node = (XML_NODE_T *) x_mem_alloc (sizeof (struct _XML_NODE_T));
	if (pt_new_node) 
	{
		x_memset(pt_new_node , 0, sizeof (struct _XML_NODE_T));/*init*/
		pt_new_node->ppv_index = (VOID **) x_mem_alloc ( sizeof (VOID *) * ui4_size);
		if (!pt_new_node->ppv_index) 
		{
			goto failed;
		}
		x_memset(pt_new_node->ppv_index, 0, sizeof (VOID *) * ui4_size); /*init*/
		pt_new_node->ui4_alloced = ui4_size;
	}
	else
	{
	    return NULL;
	}

	return pt_new_node;
		
failed:
	x_mem_free(pt_new_node);
	return NULL;
}

/*-----------------------------------------------------------------------------
 * Name: x_xml_free_node
 * Description:   free xml node
 * Inputs: 
 * 			pt_xml_node,   xml node 
 * Outputs: 
 *          N/A
 * Returns: 
 *         N/A
 ----------------------------------------------------------------------------*/
VOID  x_xml_free_node(XML_NODE_T *pt_xml_node)
{
#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif

	XML_ASSERT(pt_xml_node, ("pt_xml_node =%p\n", pt_xml_node));

	if (! pt_xml_node) 
	{
		return;
	}
	if (pt_xml_node->ppv_index) 
	{
		x_mem_free ( pt_xml_node->ppv_index);
	}
	x_mem_free (pt_xml_node);
}

/*-----------------------------------------------------------------------------
 * Name: _xml_free
 * Description:   free xml document
 * Inputs: 
 * 			pt_xml_doc,   xml document
 * Outputs: 
 *          N/A
 * Returns: 
 *         N/A
 ----------------------------------------------------------------------------*/
static VOID  _xml_free (XML_T  *pt_xml_doc)
{
	XML_ATTRIBUTE_T  *pt_attr;
	XML_T            *pt_ele;

#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif

	XML_ASSERT(pt_xml_doc, ("pt_xml_doc =%p\n", pt_xml_doc));
	/* Check the document itself.  */
	if (! pt_xml_doc) 
	{
		return;
	}
	
	/* Free the child elements.  */
	pt_ele = xml_ListHead (&pt_xml_doc->child_ele_list);
    while (pt_ele) 
	{
		x_xml_delete_element(pt_ele);
		x_xml_free(pt_ele);
		pt_ele = xml_ListHead (&pt_xml_doc->child_ele_list);
    }

    /* Free the XML attributes.  */
	pt_attr = xml_ListHead (&pt_xml_doc->attrlist);
    while (pt_attr) 
	{
       xml_ListDelete (&pt_xml_doc->attrlist, pt_attr);
       x_xml_free_attribute (pt_attr);
	   pt_attr = xml_ListHead (&pt_xml_doc->attrlist);
    }

	/* Free myself.  */
	_xml_free_element (pt_xml_doc);
}

/*-----------------------------------------------------------------------------
 * Name: x_xml_increase_ref_cnt
 * Description: refers to the xml document
 * Inputs: 
 * 			pt_xml_doc,   xml document
 * Outputs: 
 *          N/A
 * Returns: 
 *         N/A
 ----------------------------------------------------------------------------*/
static XML_T * _xml_increase_ref_cnt (XML_T *pt_xml_doc)
{
#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif

	XML_ASSERT(pt_xml_doc, ("pt_xml_doc =%p\n", pt_xml_doc));

	if (pt_xml_doc) 
	{
		pt_xml_doc->ui4_reference_counter++;
	}
	return pt_xml_doc;
}

/*-----------------------------------------------------------------------------
 * Name: x_xml_free
 * Description: release the reference of the xml document
 * Inputs: 
 * 			pt_xml_doc,   xml document
 * Outputs: 
 *          N/A
 * Returns: 
 *         N/A
 ----------------------------------------------------------------------------*/
VOID  x_xml_free (XML_T *pt_xml_doc)
{
#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif

	XML_ASSERT(pt_xml_doc, ("pt_xml_doc =%p\n", pt_xml_doc));
	
	if (pt_xml_doc) 
	{
		if ((! pt_xml_doc->ui4_reference_counter)|| 
	        (! -- pt_xml_doc->ui4_reference_counter)) 
		{
			_xml_free (pt_xml_doc);
		}
	}
}

/*-----------------------------------------------------------------------------
 * Name: _xml_cmp_name
 * Description: compare the xml name of element/attribute
 * Inputs: 
 * 			pt_xml_doc,   xml document
 * Outputs: 
 *          N/A
 * Returns: 
 *         N/A
 ----------------------------------------------------------------------------*/
INT32 _xml_cmp_name (const CHAR *ps_name_a, const CHAR *ps_name_b)
{
#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif
	if (! ps_name_a) 
	{
		return -1;
	}
	if (! ps_name_b) 
	{
		return 1;
	}
	return x_strcmp (ps_name_a, ps_name_b);
}

/*-----------------------------------------------------------------------------
 * Name: _xml_cmp_name_ns
 * Description: compare the xml name of element/attribute with namespace
 * Inputs: 
 * 			pt_xml_doc,   xml document
 * Outputs: 
 *          N/A
 * Returns: 
 *         N/A
 ----------------------------------------------------------------------------*/
static INT32 _xml_cmp_name_ns (const CHAR *ps_name_a, const CHAR *ps_ns,  const CHAR *ps_name_b)
{
	CHAR *p;
	INT32 i4_strcmp_ret;
	
#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif

	if (! ps_name_a) 
	{
		return -1;
	}
	if (! ps_name_b) 
	{
		return 1;
	}
	/* Keep x_strcmp result. */
	i4_strcmp_ret = x_strcmp (ps_name_a, ps_name_b);
	
	/* Same string just return.  */
	if (i4_strcmp_ret == 0) 
	{
		return 0;
	}
	
	/* Find ':'.  */
	p = x_strchr (ps_name_a, ':');
	
	/* There is no ':', return x_strcmp result.  */
	if (! p) 
	{
		return i4_strcmp_ret;
	}
	/* Increment ponter.  */
	p++;
	if (*p == '\0') 
	{
		return i4_strcmp_ret;
	}
	return x_strcmp (p, ps_name_b);
}

/*-----------------------------------------------------------------------------
 * Name: x_xml_get_element
 * Description: get child elment from specified parent element by element name
 * Inputs: 
 *  		pt_xml_ele,   parent xml element
 *  		ps_ele_name,  name of child xml element
 * Outputs: 
 *          N/A
 * Returns: 
 *         child element
 ----------------------------------------------------------------------------*/
XML_T * x_xml_get_element (XML_T *pt_xml_ele, const CHAR *ps_ele_name)
{
	XML_T *pt_ele;
	
#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif

	XML_ASSERT(pt_xml_ele, ("pt_xml_ele = %p\n", pt_xml_ele));
	XML_ASSERT(ps_ele_name, ("ps_ele_name = %p\n", ps_ele_name));
	
	xml_ListLoop (&pt_xml_ele->child_ele_list, pt_ele) 
	{
		if (_xml_cmp_name (XML_Name (pt_ele), ps_ele_name) == 0) 
		{
			return pt_ele;
		}
	}
	return NULL;
}

/*-----------------------------------------------------------------------------
 * Name: x_xml_get_first_element
 * Description: get the root element of xml document
 * Inputs: 
 *  		pt_xml_doc,   xml document
 * Outputs: 
 *          N/A
 * Returns: 
 *         child element
 ----------------------------------------------------------------------------*/
XML_T * x_xml_get_first_element (XML_T *pt_xml_doc)
{
#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif

	XML_ASSERT(pt_xml_doc, ("pt_xml_doc = %p\n", pt_xml_doc));

	return xml_ListHead (&pt_xml_doc->child_ele_list);
}

/*-----------------------------------------------------------------------------
 * Name: x_xml_get_first_element_by_tag_name
 * Description: get the first element which name equal to specified tag name 
 *              from xml document
 * Inputs: 
 *  		pt_xml_doc,   xml document
 *  		ps_tag_name,  tag name
 * Outputs: 
 *          N/A
 * Returns: 
 *         xml element
 ----------------------------------------------------------------------------*/
XML_T * x_xml_get_first_element_by_tag_name (XML_T *pt_xml_doc, const CHAR *ps_tag_name)
{
	XML_T  *pt_ele;
	XML_T  *pt_find;
	
#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif

	XML_ASSERT(pt_xml_doc, ("pt_xml_doc = %p\n", pt_xml_doc));
	XML_ASSERT(ps_tag_name, ("ps_tag_name = %p\n", ps_tag_name));
	
	if (_xml_cmp_name(XML_Name (pt_xml_doc), ps_tag_name) == 0) 
	{
		return pt_xml_doc;
	}
	xml_ListLoop (&pt_xml_doc->child_ele_list, pt_ele) 
	{
		pt_find = x_xml_get_first_element_by_tag_name (pt_ele, ps_tag_name);
		if (pt_find) 
		{
			return pt_find;
		}
	}
	
	return NULL;
}
	
/*-----------------------------------------------------------------------------
 * Name: x_xml_get_first_element_by_tag_name_ns
 * Description: get the first element which name equal to specified tag name 
 *              from xml document with namespace
 * Inputs: 
 *  		pt_xml_doc,   xml document
 *  		ps_xml_ns,    namespace
 *  		ps_ele_name,  tag name
 * Outputs: 
 *          N/A
 * Returns: 
 *         xml element
 ----------------------------------------------------------------------------*/
XML_T * x_xml_get_first_element_by_tag_name_ns (XML_T *pt_xml_doc, const CHAR *ps_xml_ns, 
                                                            const CHAR *ps_ele_name)
{
	XML_T    *pt_ele;
	XML_T    *pt_find;
	
#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif

	XML_ASSERT(pt_xml_doc, ("pt_xml_doc = %p\n", pt_xml_doc));
	XML_ASSERT(ps_ele_name, ("ps_ele_name = %p\n", ps_ele_name));
	
	if (_xml_cmp_name_ns (XML_Name (pt_xml_doc), ps_xml_ns, ps_ele_name) == 0) 
	{
		return pt_xml_doc;
	}
	
	xml_ListLoop (&pt_xml_doc->child_ele_list, pt_ele) 
	{
		pt_find = x_xml_get_first_element_by_tag_name_ns(pt_ele, ps_xml_ns, ps_ele_name);
		if (pt_find) 
		{
			return pt_find;
		}
	}
	
	return NULL;
}

/*-----------------------------------------------------------------------------
 * Name: x_xml_get_first_element_value
 * Description: get the value of first element which name equal to specified 
 *              tag name from xml document
 * Inputs: 
 *  		pt_xml_doc,   xml document
 *  		ps_ele_name,  tag name
 * Outputs: 
 *          N/A
 * Returns: 
 *         value of xml element
 ----------------------------------------------------------------------------*/
CHAR * x_xml_get_first_element_value (XML_T *pt_xml_doc, const CHAR *ps_ele_name)
{
	XML_T *pt_ele;
	
#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif

	XML_ASSERT(pt_xml_doc, ("pt_xml_doc = %p\n", pt_xml_doc));
	XML_ASSERT(ps_ele_name, ("ps_ele_name = %p\n", ps_ele_name));
	
	pt_ele = x_xml_get_first_element_by_tag_name (pt_xml_doc, ps_ele_name);
	if (pt_ele) 
	{
		return XML_Value (pt_ele);
	}
	return NULL;
}

/*-----------------------------------------------------------------------------
 * Name: x_xml_get_attribute
 * Description: get the attribute of xml element by attribute name
 * Inputs: 
 *  		pt_xml_ele,   xml element
 *  		ps_attr_name,  attribute name
 * Outputs: 
 *          N/A
 * Returns: 
 *         xml attribute
 ----------------------------------------------------------------------------*/
XML_ATTRIBUTE_T * x_xml_get_attribute (XML_T *pt_xml_ele, const CHAR *ps_attr_name)
{
	XML_ATTRIBUTE_T  *pt_attr;
	
#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif

	XML_ASSERT(pt_xml_ele, ("pt_xml_ele = %p\n", pt_xml_ele));
	XML_ASSERT(ps_attr_name, ("ps_attr_name = %p\n", ps_attr_name));
	
	xml_ListLoop (&pt_xml_ele->attrlist, pt_attr) 
	{
		if (_xml_cmp_name(pt_attr->ps_name, ps_attr_name) == 0) 
		{
			return  pt_attr;
		}
	}
	return NULL;
}

/*-----------------------------------------------------------------------------
 * Name: x_xml_get_local_name
 * Description: get the local name of xml element
 * Inputs: 
 *  		pt_xml_ele,   xml element
 * Outputs: 
 *          N/A
 * Returns: 
 *         local name
 ----------------------------------------------------------------------------*/
CHAR * x_xml_get_local_name (XML_T *pt_xml_ele)
{
	CHAR *p;
	
#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif

	XML_ASSERT(pt_xml_ele, ("pt_xml_ele = %p\n", pt_xml_ele));
	
	if (! pt_xml_ele->ps_name) 
	{
		return pt_xml_ele->ps_name;
	}
	p = x_strchr (pt_xml_ele->ps_name, ':');
	if (p) 
	{
		return ++p;
	}
	else 
	{
		return pt_xml_ele->ps_name;
	}
}

/*-----------------------------------------------------------------------------
 * Name: x_xml_get_namespace_uri
 * Description: get the uri of namespace
 * Inputs: 
 *  		pt_xml_ele,   xml element
 * Outputs: 
 *          N/A
 * Returns: 
 *         uri
 ----------------------------------------------------------------------------*/
CHAR *
x_xml_get_namespace_uri (XML_T *pt_xml_ele)
{
	XML_ATTRIBUTE_T   *pt_attr;
	
#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif

	XML_ASSERT(pt_xml_ele, ("pt_xml_ele = %p\n", pt_xml_ele));
	
	xml_ListLoop (&pt_xml_ele->attrlist, pt_attr) 
	{
		if (! x_strncmp (pt_attr->ps_name, (CHAR *) "xmlns", x_strlen ((CHAR *) "xmlns"))) 
		{
			return  pt_attr->ps_value;
		}
	}
	
	return NULL;
}

/*-----------------------------------------------------------------------------
 * Name: _xml_empty_slot
 * Description:  Utility function for get empty XML node slot.
 * Inputs: 
 *  		pt_xml_node,   xml node
 * Outputs: 
 *          N/A
 * Returns: 
 *         the first index which is empty in xml node
 ----------------------------------------------------------------------------*/
static INT32  _xml_empty_slot (XML_NODE_T  *pt_xml_node)
{
	UINT32  ui4_i;
	
#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif

	XML_ASSERT(pt_xml_node, ("pt_xml_node = %p\n", pt_xml_node));
	
	if (pt_xml_node->ui4_max == 0) 
	{
		return 0;
	}
	for (ui4_i = 0; ui4_i < pt_xml_node->ui4_max; ui4_i++) 
	{
		if (pt_xml_node->ppv_index[ui4_i] == 0) 
		{
			return (INT32)ui4_i;
		}
	}
	
	return (INT32)ui4_i;
}

/*-----------------------------------------------------------------------------
 * Name: _xml_node_ensure
 * Description:  Utility function for ensure XML node index.
 * Inputs: 
 *  		n,   xml node
 *  		num,   the index of xml node index         
 * Outputs: 
 *          N/A
 * Returns: 
 *  	   TRUE: 
 *  	   FALSE: 
 ----------------------------------------------------------------------------*/
static BOOL  _xml_node_ensure (XML_NODE_T *pt_node, UINT32 ui4_num)
{
#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif

	XML_ASSERT(pt_node, ("n = %p\n", pt_node));
	
	if (pt_node->ui4_alloced > ui4_num) 
	{
		return TRUE;
	}
		
	pt_node->ppv_index = (VOID **) x_mem_realloc(pt_node->ppv_index, 
                           sizeof (VOID *) * (pt_node->ui4_alloced * 2));
	if (pt_node->ppv_index) 
	{
		x_memset (&pt_node->ppv_index[pt_node->ui4_alloced], 0, 
                     sizeof (VOID *) * pt_node->ui4_alloced);
		pt_node->ui4_alloced *= 2;
		
		if (pt_node->ui4_alloced <= ui4_num) 
		{
			if (!_xml_node_ensure (pt_node, ui4_num)) 
			{
				return FALSE;
			}
		}
		return TRUE;
	} 
	else 
	{
		return FALSE;
	}
}

/*-----------------------------------------------------------------------------
 * Name: _xml_set_node
 * Description:  Utility function for setting value to XML node.
 * Inputs: 
 *  		n,   xml node
 *  		val,  xml element         
 * Outputs: 
 *          N/A
 * Returns: 
 *  	   -1: failed
 *  	   others: ok
 ----------------------------------------------------------------------------*/
static INT32  _xml_set_node(XML_NODE_T *pt_node, VOID *pv_val)
{
	UINT32 ui4_i;
	
#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif

	XML_ASSERT(pt_node, ("pt_node = %p\n", pt_node));
	
	ui4_i = (UINT32)_xml_empty_slot (pt_node);
	if (!_xml_node_ensure (pt_node, ui4_i)) 
	{
		return -1;
	}
	
	pt_node->ppv_index[ui4_i] = pv_val;
	
	if (pt_node->ui4_max <= ui4_i) 
	{
		pt_node->ui4_max = ui4_i + 1;
	}
	return (INT32)ui4_i;
}

/*-----------------------------------------------------------------------------
 * Name: _xml_get_elements_by_tag_name_func
 * Description:  Utility function for x_xml_get_elements_by_tag_name.
 * Inputs: 
 *  		pt_xml_doc,   xml document
 *  		name,         tag name of xml element         
 * Outputs: 
 *          n, xml node
 * Returns: 
 *  	   N/A
 ----------------------------------------------------------------------------*/
static VOID  _xml_get_elements_by_tag_name_func (XML_T        *pt_xml_doc, 
                                                             const CHAR   *name,
                                                             XML_NODE_T   *pt_node)
{
	XML_T *pt_ele;
	
#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif

	XML_ASSERT(pt_xml_doc, ("pt_xml_doc = %p\n", pt_xml_doc));
	XML_ASSERT(pt_node, ("n = %p\n", pt_node));

	xml_ListLoop (&pt_xml_doc->child_ele_list, pt_ele) 
	{
		_xml_get_elements_by_tag_name_func (pt_ele, name, pt_node);
	}
	if (_xml_cmp_name(XML_Name (pt_xml_doc), name) == 0) 
	{
		_xml_set_node(pt_node, pt_xml_doc);
	}
}

/*-----------------------------------------------------------------------------
 * Name: x_xml_get_elements_by_tag_name
 * Description:  get xml elements by tag name
 * Inputs: 
 *  		pt_xml_doc,   xml document
 *  		ps_tag_name,         tag name of xml element         
 * Outputs: 
 *          N/A
 * Returns: 
 *  	   xml node
 ----------------------------------------------------------------------------*/
XML_NODE_T * x_xml_get_elements_by_tag_name (XML_T *pt_xml_doc, const CHAR *ps_tag_name)
{
	XML_NODE_T  *pt_node;

#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif

	XML_ASSERT(pt_xml_doc, ("pt_xml_doc = %p\n", pt_xml_doc));
	if(!pt_xml_doc)
	{
		return NULL;
	}
	pt_node = _xml_new_node (XML_NODE_INIT_SIZE);
	if (pt_node) 
	{
		_xml_get_elements_by_tag_name_func (pt_xml_doc, ps_tag_name, pt_node);
	}
	return pt_node;
}

#ifdef _XML_DEBUG
/*-----------------------------------------------------------------------------
 * Name: _xml_print_func
 * Description:  Print XML document to standard out.
 * Inputs: 
 *  		doc,    xml document
 *  		depth,  start level to print      
 * Outputs: 
 *          N/A
 * Returns: 
 *  	   N/A
 ----------------------------------------------------------------------------*/
static VOID _xml_print_func (XML_T *doc, UINT32 ui4_depth)
{
	XML_ATTRIBUTE_T   *pt_attr;
	XML_T             *pt_ele;

#define XML_PrintNumIndent   (2)
	
#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif

	XML_ASSERT(doc, ("doc = %p\n", doc));
	if (! doc) 
	{
		return;
	}
	
	x_dbg_stmt ("%*s<%s", ui4_depth * XML_PrintNumIndent, " ", doc->ps_name);
	
	xml_ListLoop (&doc->attrlist, pt_attr) 
	{
		x_dbg_stmt (" %s=\"%s\"", pt_attr->ps_name, pt_attr->ps_value);
	}
	if (xml_ListEmpty (&doc->child_ele_list)) 
	{
		if (_is_valid_value(doc->ps_value)) 
		{
			x_dbg_stmt (">");
			x_dbg_stmt ("%s", doc->ps_value);
			x_dbg_stmt ("</%s>\n", doc->ps_name);
		} 
		else 
		{
			x_dbg_stmt ("/>\n");
		}
	} 
	else 
	{
		XML_ASSERT(!_is_valid_value(doc->ps_value), ("...."));
		x_dbg_stmt (">");
		x_dbg_stmt ("\n");
		xml_ListLoop (&doc->child_ele_list, pt_ele) 
		{
			_xml_print_func (pt_ele, ui4_depth + 1); /* Recursive */
		}
		x_dbg_stmt ("%*s</%s>\n", ui4_depth * XML_PrintNumIndent," ", doc->ps_name);
	}
}

/*-----------------------------------------------------------------------------
 * Name: x_xml_print
 * Description:  Print XML document to standard out.
 * Inputs: 
 *  		doc,    xml document  
 * Outputs: 
 *          N/A
 * Returns: 
 *  	   N/A
 ----------------------------------------------------------------------------*/
VOID  x_xml_print (XML_T *pt_xml_doc)
{
#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif

	XML_ASSERT(pt_xml_doc, ("pt_xml_doc = %p\n", pt_xml_doc));
	
	_xml_print_func (pt_xml_doc, 0);
}
#endif /* _XML_DEBUG */


/*-----------------------------------------------------------------------------
 * Name: _xml_print_document_str
 * Description:  Format XML document as  string buffer.
 * Inputs: 
 *  		doc,    xml document	
 * Outputs: 
 *          sbuf,   string buffer
 * Returns: 
 *  	  XML_SUCCESS
 ----------------------------------------------------------------------------*/
INT32
_xml_print_document_str (XML_T *pt_doc, XML_SBUF_T   *pt_sbuf)
{
	XML_ATTRIBUTE_T    *pt_attr;
	XML_T              *pt_ele;
	
#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif
		
	XML_ASSERT(pt_doc, ("pt_doc = %p\n", pt_doc));
	XML_ASSERT(pt_sbuf, ("pt_sbuf = %p\n", pt_sbuf));
	if (! pt_doc) 
	{
		return XML_SUCCESS;
	}
	xml_sbuf_add_str (pt_sbuf, (CHAR *) "<");
	xml_sbuf_add_str (pt_sbuf, pt_doc->ps_name);
	
	xml_ListLoop (&pt_doc->attrlist, pt_attr) 
	{
		xml_sbuf_add_str (pt_sbuf, (CHAR *) " ");
		xml_sbuf_add_str (pt_sbuf, pt_attr->ps_name);
		xml_sbuf_add_str (pt_sbuf, (CHAR *) "=\"");
		xml_sbuf_add_str (pt_sbuf, pt_attr->ps_value);
		xml_sbuf_add_str (pt_sbuf, (CHAR *) "\"");
	}
	
	if (xml_ListEmpty (&pt_doc->child_ele_list)) 
	{
		 
		if (_is_valid_value(pt_doc->ps_value)) 
		{
			xml_sbuf_add_str (pt_sbuf, (CHAR *) ">");
			
			xml_sbuf_add_escape (pt_sbuf, pt_doc->ps_value);
			
			xml_sbuf_add_str (pt_sbuf, (CHAR *) "</");
			xml_sbuf_add_str (pt_sbuf, pt_doc->ps_name);
			xml_sbuf_add_str (pt_sbuf, (CHAR *) ">\n");
		} 
		else 
		{
			xml_sbuf_add_str (pt_sbuf, (CHAR *) "/>\n");
		}
	} 
	else 
	{
		XML_ASSERT((!_is_valid_value(pt_doc->ps_value)), ("...."));
				
		xml_sbuf_add_str (pt_sbuf, (CHAR *) ">");
		xml_sbuf_add_str (pt_sbuf, (CHAR *) "\n");
		xml_ListLoop (&pt_doc->child_ele_list, pt_ele) 
		{
			_xml_print_document_str (pt_ele, pt_sbuf); /* Recursive */
		}
		xml_sbuf_add_str (pt_sbuf, (CHAR *) "</");
		xml_sbuf_add_str (pt_sbuf, pt_doc->ps_name);
		xml_sbuf_add_str (pt_sbuf, (CHAR *) ">\n");
	}
	return XML_SUCCESS;
}

/*-----------------------------------------------------------------------------
 * Name: x_xml_print_document
 * Description:  Return XML document as string data.
 * Inputs: 
 *  		pt_xml_doc,    xml document	
 * Outputs: 
 *          N/A
 * Returns: 
 *  	  string buffer which store xml document
 ----------------------------------------------------------------------------*/
CHAR * x_xml_print_document (XML_T *pt_xml_doc)
{
	XML_SBUF_T  t_sbuf;
	
#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif

	XML_ASSERT(pt_xml_doc, ("pt_xml_doc = %p\n", pt_xml_doc));
	
	xml_sbuf_init (&t_sbuf);
	
	if (_xml_print_document_str (pt_xml_doc, &t_sbuf) != XML_SUCCESS) 
	{
		return NULL;
	}
	return t_sbuf.ps_buf;
}

/*-----------------------------------------------------------------------------
 * Name: x_xml_copy
 * Description:  copy xml document
 * Inputs: 
 *  		pt_xml_doc,    original xml document	
 * Outputs: 
 *          N/A
 * Returns: 
 *  	  new document
 ----------------------------------------------------------------------------*/
XML_T *x_xml_copy (XML_T *pt_xml_doc)
{
	XML_SBUF_T   t_sbuf;
	XML_T        *pt_new_doc = NULL;
	
#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif

	XML_ASSERT(pt_xml_doc, ("pt_xml_doc = %p\n", pt_xml_doc));
	
	xml_sbuf_init (&t_sbuf);
	if (_xml_print_document_str (pt_xml_doc, &t_sbuf) != XML_SUCCESS) 
	{
		return NULL;
	}
		
	if (x_xml_parse_buffer(t_sbuf.ps_buf, t_sbuf.ui4_size, &pt_new_doc) != XML_SUCCESS) 
	{
		goto failed;
	}
failed:
	xml_sbuf_free_buffer(&t_sbuf);
	return pt_new_doc;
}


/*-----------------------------------------------------------------------------
 * Name: _xml_get_top_parent
 * Description:  get the top parent element of specified xml element
 * Inputs: 
 *  		self,    xml element   
 * Outputs: 
 *          N/A
 * Returns: 
 *  	   parent element
 ----------------------------------------------------------------------------*/
static XML_T* _xml_get_top_parent(XML_T *pt_self)
{
	XML_T *pt_ele = pt_self;
	
#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif

	XML_ASSERT(pt_self, ("pt_self = %p\n", pt_self));
	
	while (pt_ele->pt_parent) 
	{
		pt_ele = pt_ele->pt_parent;
	}
	
	return  pt_ele;
}

/*-----------------------------------------------------------------------------
 * Name: _xml_is_child
 * Description:  make sure the "in_e" if a child of the "self"
 * Inputs: 
 *  		self,    xml element
 *  		in_e,    child element
 * Outputs: 
 *          N/A
 * Returns: 
 *  	   TRUE:
 *  	   FALSE:
 ----------------------------------------------------------------------------*/
static BOOL _xml_is_child(XML_T *pt_self, XML_T *pt_child_ele)
{
	XML_T *pt_ele;
	
#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif

	XML_ASSERT(pt_self, ("pt_self = %p\n", pt_self));
	XML_ASSERT(pt_child_ele, ("pt_child_ele = %p\n", pt_child_ele));
	
	xml_ListLoop (&pt_self->child_ele_list, pt_ele) 
	{
		if (pt_ele == pt_child_ele)
		 {
			return TRUE;
		}
	}
	return FALSE;
}

/*-----------------------------------------------------------------------------
 * Name: _xml_is_descendant
 * Description:  make sure the "in_e" if a Descendant of the "self"
 * Inputs: 
 *  		self,    xml element
 *  		in_e,    child element
 * Outputs: 
 *          N/A
 * Returns: 
 *  	   TRUE:
 *  	   FALSE:
 ----------------------------------------------------------------------------*/
static BOOL  _xml_is_descendant(XML_T *pt_self, XML_T *pt_descendant_ele)
{
	XML_T *pt_ele;
	
#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif

    XML_ASSERT(pt_self, ("pt_self = %p\n", pt_self));
	XML_ASSERT(pt_descendant_ele, ("pt_descendant_ele = %p\n", pt_descendant_ele));
  
	if (pt_self == pt_descendant_ele) 
	{
		return TRUE;
	}
	
	if (_xml_is_child(pt_self, pt_descendant_ele)) 
	{
		return TRUE;
	}
		
	xml_ListLoop (&pt_self->child_ele_list, pt_ele) 
	{
		if (_xml_is_descendant(pt_ele, pt_descendant_ele)) 
		{
			return TRUE;
		}
	}
	return FALSE;
}

/*-----------------------------------------------------------------------------
 * Name: _xml_deque_push
 * Description: Deque push utility function.
 * Inputs: 
 *  		u,    User data
 *  		e,    xml element
 * Outputs: 
 *          N/A
 * Returns: 
 *  	   XML_SUCCESS
 ----------------------------------------------------------------------------*/
static INT32  _xml_deque_push (XML_USER_DATA_T *pt_user_data, XML_T *pt_ele)
{
    XML_DEQUE_T  *pt_new_deque;
  
#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif

    XML_ASSERT(pt_user_data, ("pt_user_data = %p\n", pt_user_data));
	XML_ASSERT(pt_ele, ("pt_ele = %p\n", pt_ele));

	pt_new_deque = (XML_DEQUE_T *) x_mem_alloc (sizeof (XML_DEQUE_T));
	if (! pt_new_deque) 
	{
		return XML_OUT_OF_MEMORY_ERR;
	}
	x_memset(pt_new_deque , 0, sizeof(XML_DEQUE_T));
	pt_new_deque->pt_xml_element = pt_ele;
	pt_new_deque->pt_prev = pt_user_data->pt_deque;
	pt_user_data->pt_deque = pt_new_deque;
	
	return XML_SUCCESS;
}

/*-----------------------------------------------------------------------------
 * Name: _xml_deque_pop
 * Description: Deque pop utility function.
 * Inputs: 
 *  		u,    User data
 * Outputs: 
 *          N/A
 * Returns: 
 *  	   xml element
 ----------------------------------------------------------------------------*/
static XML_T * _xml_deque_pop (XML_USER_DATA_T *pt_user_data)
{
	XML_DEQUE_T  *pt_bottom;
	XML_T        *pt_ele = NULL;
	
#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif

	XML_ASSERT(pt_user_data, ("pt_user_data = %p\n", pt_user_data));
	
	pt_bottom = pt_user_data->pt_deque;
	if (pt_bottom) 
	{
		pt_user_data->pt_deque = pt_bottom->pt_prev;
		pt_ele = pt_bottom->pt_xml_element;
		x_mem_free (pt_bottom);
	}
	return  pt_ele;
}

/*-----------------------------------------------------------------------------
 * Name: _xml_deque_free
 * Description: Free deque.
 * Inputs: 
 *  		u,    User data
 * Outputs: 
 *          N/A
 * Returns: 
 *  	  N/A
 ----------------------------------------------------------------------------*/
static VOID _xml_deque_free (XML_USER_DATA_T *pt_user_data)
{
	XML_T  *pt_ele;
	
#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif

	XML_ASSERT(pt_user_data, ("pt_user_data = %p\n", pt_user_data));
	
	while (pt_user_data->pt_deque) 
	{
		/* Returns an element that needs to be freed.  */
		pt_ele = _xml_deque_pop (pt_user_data);
		if (pt_ele)
		{
			XML_ASSERT(pt_user_data->pt_doc == _xml_get_top_parent(pt_ele), ("..."));
			XML_ASSERT(_xml_is_descendant(pt_user_data->pt_doc, pt_ele), ("..."));
		}
	}
}

#ifdef XML_XMLDECL_PARSE
/*-----------------------------------------------------------------------------
 * Name: _xml_xmldecl_handler
 * Description: XML declaration handler.
 * Inputs: 
 *  		data,    User data
 *  		version,
 *  		encoding,
 *  		standalone,
 * Outputs: 
 *          N/A
 * Returns: 
 *  	  N/A
 ----------------------------------------------------------------------------*/
static VOID _xml_xmldecl_handler (VOID *pv_data, const CHAR *ps_version,
                                      const CHAR *ps_encoding, INT32  i4_standalone)
{
	XML_USER_DATA_T *pt_user_data = (XML_USER_DATA_T *) pv_data;
	
#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif

	XML_ASSERT(pv_data, ("pv_data = %p\n", pv_data));
	
	if (ps_version) 
	{
		pt_user_data->version = x_strdup (ps_version);
	}
	if (ps_encoding) 
	{
		pt_user_data->encoding = x_strdup (ps_encoding);
	}
}
#endif /* XML_XMLDECL_PARSE */

/*-----------------------------------------------------------------------------
 * Name: _xml_start_handler
 * Description: XML start tag handler.
 * Inputs: 
 *  		data,    User data
 *  		name,
 *  		attr,
 * Outputs: 
 *          N/A
 * Returns: 
 *  	  N/A
 ----------------------------------------------------------------------------*/
static VOID
_xml_start_handler (VOID *pv_data, const ssXML_Char *ps_name, const ssXML_Char **pps_attr)
{
	XML_USER_DATA_T *pt_user_data = (XML_USER_DATA_T *) pv_data;
	INT32 i4_i;
	INT32 i4_ret;
	
#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif

	XML_ASSERT(pv_data, ("pv_data = %p\n", pv_data));
	XML_ASSERT(ps_name, ("ps_name = %p\n", ps_name));
	
		/* Element name.  */
	if (! pt_user_data->pt_doc) 
	{
		pt_user_data->pt_doc = pt_user_data->pt_xml_current = x_xml_new_element((CHAR *) ps_name, NULL);
		if (!pt_user_data->pt_doc) 
		{
			pt_user_data->i4_error = XML_OUT_OF_MEMORY_ERR;
			return;
		}
	} 
	else 
	{
		i4_ret = _xml_deque_push (pt_user_data, pt_user_data->pt_xml_current);
		if (i4_ret != XML_SUCCESS) 
		{
			pt_user_data->i4_error = i4_ret;
			return;
		}
		pt_user_data->pt_xml_current = x_xml_add_element(pt_user_data->pt_xml_current, (CHAR *)ps_name, NULL);
		if (! pt_user_data->pt_xml_current) 
		{
			pt_user_data->i4_error = XML_OUT_OF_MEMORY_ERR;
			return;
		}
	}
	
		/* Attributes.  */
	for (i4_i = 0; pps_attr[i4_i]; i4_i += 2) 
	{
		XML_ATTRIBUTE_T  *pt_new_attr;
		
		pt_new_attr = x_xml_add_attribute(pt_user_data->pt_xml_current, (CHAR *) pps_attr[i4_i], (CHAR *) pps_attr[i4_i + 1]);
		if (! pt_new_attr) 
		{
			pt_user_data->i4_error = XML_OUT_OF_MEMORY_ERR;
			return;
		}
	}
}

/*-----------------------------------------------------------------------------
 * Name: _xml_normalize_char
 * Description: Normalize the string..
 * Inputs: 
 *  		str,   
 * Outputs: 
 *          N/A
 * Returns: 
 *  	  N/A
 ----------------------------------------------------------------------------*/
static VOID _xml_normalize_char (CHAR *ps_str)
{
	INT32  i4_start;
	INT32  i4_end;
	INT32  i4_total;
	
#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif

	XML_ASSERT(ps_str, ("...."));
	
	i4_end = (INT32)x_strlen (ps_str);
	
	while (i4_end > 0 && _xml_isspace (ps_str[i4_end - 1])) 
	{
		ps_str[--i4_end] = '\0';
	}
	/*x_strspn: Find the first sub-string*/
	i4_start = (INT32)x_strspn (ps_str, (CHAR *) " \n\r\t\v");
	i4_total = i4_end - i4_start;
	x_memmove (ps_str, &ps_str[i4_start], (SIZE_T)i4_total);
	ps_str[i4_total] = '\0';
}

/*-----------------------------------------------------------------------------
 * Name: _xml_end_handler
 * Description: XML end tag handler. 
 * Inputs: 
 *  		data, user data
 *  		el,
 * Outputs: 
 *          N/A
 * Returns: 
 *  	  N/A
 ----------------------------------------------------------------------------*/
static void  _xml_end_handler (VOID *pv_data, const ssXML_Char *ps_ele)
{
	XML_USER_DATA_T *pt_user_data = (XML_USER_DATA_T *) pv_data;
	
#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif

	XML_ASSERT(pv_data, ("pv_data = %p\n", pv_data));
	XML_ASSERT(ps_ele, ("ps_ele = %p\n", ps_ele));
	
	if (pt_user_data->pt_xml_current && pt_user_data->pt_xml_current->ps_value) 
	{
		_xml_normalize_char (pt_user_data->pt_xml_current->ps_value);
	}
	pt_user_data->pt_xml_current = _xml_deque_pop (pt_user_data);
}

/*-----------------------------------------------------------------------------
 * Name: _xml_cdata_start_handler
 * Description: XML cdata start handler. 
 * Inputs: 
 *  		data, user data
 * Outputs: 
 *          N/A
 * Returns: 
 *  	  N/A
 ----------------------------------------------------------------------------*/
static VOID  _xml_cdata_start_handler (VOID *pv_data)
{
#if 0
	XML_USER_DATA_T *pt_user_data = (XML_USER_DATA_T *) pv_data;
	
#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif

	XML_ASSERT(pv_data, ("pv_data = %p\n", pv_data));
	
	/* Set the CDATA flag to skip the CDATA.  */
	XML_SET_FLAG (pt_user_data->i4_flags, XML_FLAG_CDATA);
#endif
}

/*-----------------------------------------------------------------------------
 * Name: _xml_cdata_end_handler
 * Description: XML cdata end handler. 
 * Inputs: 
 *  		data, user data
 * Outputs: 
 *          N/A
 * Returns: 
 *  	  N/A
 ----------------------------------------------------------------------------*/
static void  _xml_cdata_end_handler (VOID *pv_data)
{
#if 0
	XML_USER_DATA_T *pt_user_data = (XML_USER_DATA_T *) pv_data;
	
#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif

	XML_ASSERT(pv_data, ("pv_data = %p\n", pv_data));
	
		/* Unset the CDATA flag to skip the CDATA.  */
	XML_UNSET_FLAG (pt_user_data->i4_flags, XML_FLAG_CDATA);
#endif
}

/*-----------------------------------------------------------------------------
 * Name: _xml_char_handler
 * Description: XML data handler.
 * Inputs: 
 *  		data, user data
 * Outputs: 
 *          N/A
 * Returns: 
 *  	  N/A
 ----------------------------------------------------------------------------*/
static VOID _xml_char_handler (VOID *pv_data, const ssXML_Char *ps_char, int i4_len)
{
	XML_USER_DATA_T  *pt_user_data = (XML_USER_DATA_T *) pv_data;
	UINT32           ui4_current_length = 0;
	UINT32           ui4_new_length;
	
#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif
		
	XML_ASSERT(pv_data, ("pv_data = %p\n", pv_data));
	XML_ASSERT(ps_char, ("ps_char = %p\n", ps_char));
	XML_ASSERT((i4_len >= 0), ("len = %d\n",i4_len));
	
	/* Current XML node must be there.  */
	if (! pt_user_data->pt_xml_current) 
	{
		return;
	}

	/* Skip the CDATA payloads.  */
	if (XML_CHECK_FLAG (pt_user_data->i4_flags, XML_FLAG_CDATA)) 
	{
		return;
	}
	/* Current string length.  */
	if (pt_user_data->pt_xml_current->ps_value) 
	{
		ui4_current_length = x_strlen (pt_user_data->pt_xml_current->ps_value);
	}
	/* Resulting length including terminate character.  */
	ui4_new_length = (ui4_current_length + i4_len + 1) * sizeof (CHAR);
	
	/* Allocate memory for the string.  */
	if (! pt_user_data->pt_xml_current->ps_value) 
	{
		pt_user_data->pt_xml_current->ps_value = (CHAR *) x_mem_alloc (ui4_new_length);
	} 
	else 
	{
		pt_user_data->pt_xml_current->ps_value
		 = (CHAR *) x_mem_realloc (pt_user_data->pt_xml_current->ps_value, ui4_new_length);
	}
	/* Copy string.  */
	if (pt_user_data->pt_xml_current->ps_value) 
	{
		x_strncpy (pt_user_data->pt_xml_current->ps_value + ui4_current_length, (CHAR *) ps_char, (SIZE_T)i4_len);
		pt_user_data->pt_xml_current->ps_value[ui4_new_length - 1] = '\0';
	}
}

/*-----------------------------------------------------------------------------
 * Name: x_xml_ParseMessageInit
 * Description: create expat XML_Parser, and register xml handler to expat XML_Parser 
 * Inputs:  
 *          pt_user_data, 
 * Outputs: 
 *          N/A
 * Returns: 
 *         0
 ----------------------------------------------------------------------------*/
INT32  x_xml_parse_message_init (XML_USER_DATA_T *pt_user_data)
{
#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif
	XML_Parser *pt_temp_parser;

	XML_ASSERT(pt_user_data, ("..."));
	
	/* Clear user data.  */
	x_memset (pt_user_data, 0, sizeof (XML_USER_DATA_T));
	
	/* Create a XML parser.  */
	pt_temp_parser = x_mem_alloc(sizeof(XML_Parser));
	
	if (pt_temp_parser == NULL)
	{
	    return -1;
	}
	
	x_memset(pt_temp_parser, 0, sizeof(XML_Parser));

    *pt_temp_parser = XML_ParserCreate (NULL);

    if (pt_temp_parser == NULL)
	{
	    return -1;
	}

	pt_user_data->pv_parser = pt_temp_parser;
	
 
	if (! pt_user_data->pv_parser) 
	{
		return -1;
	}
	/* Set XML handler.  */
#ifdef XML_XMLDECL_PARSE
	XML_SetXmlDeclHandler (*pt_temp_parser, _xml_xmldecl_handler);
#endif /* XML_XMLDECL_PARSE */
	XML_SetElementHandler (*pt_temp_parser, _xml_start_handler, _xml_end_handler);
	XML_SetCdataSectionHandler (*pt_temp_parser, _xml_cdata_start_handler,
		    											_xml_cdata_end_handler);
	XML_SetCharacterDataHandler (*pt_temp_parser, _xml_char_handler);
	
	/* Set user data.  */
	XML_SetUserData (*pt_temp_parser, (VOID *) pt_user_data);
	pt_user_data->i4_error = XML_SUCCESS;
	
	return 0;
}

/*-----------------------------------------------------------------------------
 * Name: x_xml_parse_message
 * Description: parse message
 * Inputs:  
 *  		pt_user_data, User data
 *  		ps_buf,       buffer will be parsed
 *  		ui4_length,   length of ps_buf
 *  		i4_done,      0: ps_buf is not a complete xml document;others : complete
 * Outputs: 
 *          N/A
 * Returns: 
 *         parse result
 ----------------------------------------------------------------------------*/
INT32  x_xml_parse_message (XML_USER_DATA_T *pt_user_data, CHAR *ps_buf, 
                                   UINT32 ui4_length, INT32 i4_done)
                   
{
	INT32 ret;
	XML_Parser *pt_temp_parser;
	
#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif
		
	XML_ASSERT(pt_user_data, ("pt_user_data = %p\n", pt_user_data));

	pt_temp_parser = (XML_Parser *)pt_user_data->pv_parser;
	
	ret = XML_Parse (*pt_temp_parser, (ssXML_Char *) ps_buf, (int)ui4_length, i4_done);
	if (! ret) 
	{
		pt_user_data->i4_error = XML_PARSE_FAILURE;
	}
	return pt_user_data->i4_error;
}

/*-----------------------------------------------------------------------------
 * Name: x_xml_parse_message_done
 * Description:  Clean up user data after parse
 * Inputs:  
 *  		pt_user_data, User data
 * Outputs: 
 *          N/A
 * Returns: 
 *         N/A
 ----------------------------------------------------------------------------*/
VOID  x_xml_parse_message_done (XML_USER_DATA_T *pt_user_data)
{
#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif
	XML_Parser *pt_temp_parser;

	XML_ASSERT(pt_user_data, ("..."));
	
	/* Free XML parser.  */
	if (pt_user_data->pv_parser) 
	{
		pt_temp_parser = (XML_Parser *)pt_user_data->pv_parser;
		XML_ParserFree (*pt_temp_parser);
		x_mem_free(pt_user_data->pv_parser);
		pt_user_data->pv_parser = NULL;
	}
	
#ifdef XML_XMLDECL_PARSE
	if (pt_user_data->ps_version) 
	{
		x_mem_free (pt_user_data->ps_version);
		pt_user_data->ps_version = NULL;
	}
	if (pt_user_data->ps_encoding) 
	{
		x_mem_free (pt_user_data->ps_encoding);
		pt_user_data->ps_encoding = NULL;
	}
#endif /* XML_XMLDECL_PARSE */
	
	/* Free deque.  */
	_xml_deque_free (pt_user_data);
	
	/* Free XML document.  */
	if (pt_user_data->pt_doc)
	{
		x_xml_free (pt_user_data->pt_doc);
		pt_user_data->pt_doc = 0;
	}
}

/*-----------------------------------------------------------------------------
 * Name: x_xml_parse_buffer
 * Description:   XML parse against to the buffer.
 * Inputs:  
 *  		ps_buf, buffer will be parsed
 *  		ui4_length, length of ps_buf
 * Outputs: 
 *          ppt_xml_doc, xml documet
 * Returns: 
 *         parse result
 ----------------------------------------------------------------------------*/
INT32 x_xml_parse_buffer (CHAR *ps_buf, UINT32  ui4_length, XML_T **ppt_xml_doc)
{
	INT32              i4_ret;
	XML_USER_DATA_T    t_user_data;
	XML_Parser         *pt_temp_parser;

#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif

	XML_ASSERT(ps_buf, ("..."));
	
	/* Initialize user data.  */
	x_xml_parse_message_init (&t_user_data); 
	
	/* Clear document.  */
	*ppt_xml_doc = NULL;

	pt_temp_parser = (XML_Parser *)t_user_data.pv_parser;
	/* Call XML parser.  */
	i4_ret = XML_Parse (*pt_temp_parser, (ssXML_Char *) ps_buf, (int)ui4_length, 1);

	if (!i4_ret) 
	{
		x_xml_parse_message_done (&t_user_data);
		return XML_PARSE_FAILURE;
	}
	
	/* When document is returned, return to the caller.  */
	if (t_user_data.pt_doc) 
	{
		*ppt_xml_doc = t_user_data.pt_doc;
		t_user_data.pt_doc = NULL;
	} 
	else 
	{
		t_user_data.i4_error = XML_INVALID_STATE_ERR;
	}
	/* Free user data.  */
	x_xml_parse_message_done (&t_user_data);
	
	/* Return error code.  */
	return t_user_data.i4_error;
}

/*-----------------------------------------------------------------------------
 * Name: x_xml_parse_str
 * Description:   XML parse string.
 * Inputs:  
 *  		ps_str, string will be parsed
 * Outputs: 
 *          ppt_xml_doc, xml documet
 * Returns: 
 *         parse result
 ----------------------------------------------------------------------------*/
INT32  x_xml_parse_str (CHAR *ps_str, XML_T **ppt_xml_doc)
{
	INT32   i4_ret;

#ifdef _XML_DEBUG
	x_dbg_stmt("==========%s==========\n", __FUNCTION__);
#endif
	
	XML_ASSERT(ps_str ,("..."));

	i4_ret = x_xml_parse_buffer (ps_str, x_strlen (ps_str), ppt_xml_doc);
	if (i4_ret == XML_SUCCESS && *ppt_xml_doc) 
	{
		_xml_increase_ref_cnt(*ppt_xml_doc);
	}
	return i4_ret;
}



