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
#include "os/inc/x_os.h"
#include "xml_debug.h"
#include "xml.h"    

XML_T*  c_xml_new_element(const CHAR *ps_ele_name, const CHAR *ps_ele_value)
{
	return  x_xml_new_element(ps_ele_name, ps_ele_value);
}

XML_T*  c_xml_add_element (XML_T  *pt_xml_ele, const CHAR *ps_ele_name, const CHAR *ps_ele_value)
{
	return  x_xml_add_element(pt_xml_ele, ps_ele_name, ps_ele_value);
}

XML_T*  c_xml_delete_element (XML_T *pt_xml_ele)
{
	return  x_xml_delete_element(pt_xml_ele);
}

XML_ATTRIBUTE_T*  c_xml_new_attribute (const CHAR *ps_attr_name, const CHAR *ps_attr_value)
{
	return x_xml_new_attribute(ps_attr_name, ps_attr_value);
}

XML_ATTRIBUTE_T*  c_xml_add_attribute (XML_T *pt_xml_ele, const CHAR *ps_attr_name, const CHAR *ps_attr_value)
{
	return  x_xml_add_attribute(pt_xml_ele, ps_attr_name, ps_attr_value);
}

VOID  c_xml_delete_attribute (XML_T *pt_xml_ele, XML_ATTRIBUTE_T *pt_xml_attr)
{
    x_xml_delete_attribute(pt_xml_ele, pt_xml_attr);
}

VOID  c_xml_free_attribute (XML_ATTRIBUTE_T  *pt_xml_attr)
{
    x_xml_free_attribute(pt_xml_attr);
}

VOID  c_xml_free_node (XML_NODE_T  *pt_xml_node)
{
    x_xml_free_node(pt_xml_node);
}

VOID  c_xml_free(XML_T  *pt_xml_doc)
{
    x_xml_free(pt_xml_doc);
}

/* API for XML element lookup.  */
XML_T*  c_xml_get_element (XML_T *pt_xml_ele, const CHAR *ps_ele_name)
{
	return x_xml_get_element(pt_xml_ele, ps_ele_name);
}

XML_T*  c_xml_get_first_element (XML_T *pt_xml_doc)
{
	return  x_xml_get_first_element(pt_xml_doc);
}

XML_T*  c_xml_get_first_element_by_tag_name (XML_T *pt_xml_doc, const CHAR *ps_tag_name)
{
	return  x_xml_get_first_element_by_tag_name(pt_xml_doc, ps_tag_name);
}

XML_T*  c_xml_get_first_element_by_tag_name_ns (XML_T *pt_xml_doc, const CHAR *ps_xml_ns, const CHAR *ps_ele_name)
{
	return  x_xml_get_first_element_by_tag_name_ns(pt_xml_doc, ps_xml_ns, ps_ele_name);
}

CHAR *  c_xml_get_first_element_value (XML_T *pt_xml_doc, const CHAR *ps_ele_name)
{
	return  x_xml_get_first_element_value(pt_xml_doc, ps_ele_name);
}

XML_ATTRIBUTE_T*  c_xml_get_attribute (XML_T *pt_xml_ele, const CHAR *ps_attr_name)
{
	return  x_xml_get_attribute(pt_xml_ele, ps_attr_name);
}

CHAR*  c_xml_get_local_name (XML_T *pt_xml_ele)
{
	return  x_xml_get_local_name(pt_xml_ele);
}

CHAR*  c_xml_get_namespace_uri (XML_T *pt_xml_ele)
{
	return  x_xml_get_namespace_uri(pt_xml_ele);
}

/* Multiple elements lookup.  */
XML_NODE_T*  c_xml_get_elements_by_tag_name (XML_T *pt_xml_doc, const CHAR *ps_tag_name)
{
	return x_xml_get_elements_by_tag_name(pt_xml_doc, ps_tag_name);
}

#ifdef _XML_DEBUG
/* XML print functions.  */
VOID  c_xml_print (XML_T *pt_xml_doc)
{
	return x_xml_print(pt_xml_doc);
}
#endif


CHAR*   c_xml_print_document (XML_T *pt_xml_doc)
{
	return  x_xml_print_document(pt_xml_doc); 
}

XML_T*  c_xml_copy (XML_T *pt_xml_doc)
{
	return x_xml_copy(pt_xml_doc);
}

/* XML parse buffer.  */
INT32   c_xml_parse_message_init (XML_USER_DATA_T *pt_user_data)
{
	return x_xml_parse_message_init(pt_user_data);
}

INT32   c_xml_parse_message (XML_USER_DATA_T  *pt_user_data, CHAR *ps_buf, UINT32 ui4_length, INT32 i4_done)
{
	return  x_xml_parse_message(pt_user_data, ps_buf, ui4_length, i4_done);
}

VOID    c_xml_parse_message_done (XML_USER_DATA_T  *pt_user_data)
{
    x_xml_parse_message_done(pt_user_data);
	return;
}

/* XML parser.  */
INT32   c_xml_parse_buffer(CHAR *ps_buf, UINT32  ui4_length, XML_T **ppt_out_xml_doc)
{
	return x_xml_parse_buffer(ps_buf, ui4_length, ppt_out_xml_doc);
}

INT32   c_xml_parse_str(CHAR * ps_str,XML_T ** ppt_out_xml_doc)
{
	return x_xml_parse_str(ps_str, ppt_out_xml_doc);
}

