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
/*-----------------------------------------------------------------------------
 * $IRRCSfile$
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/2 $
 * $SWAuthor: CS Kuo $
 * $MD5HEX: 2215e557ae1f0c8bd7da89235b28ac79 $
 *
 * Description:
 *         This header file contains Baseline HTML Parser specific
 *         definitions, which are exported.
 *---------------------------------------------------------------------------*/

#ifndef _U_HTML_PSR_H_
#define _U_HTML_PSR_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_common.h"
#include "u_lnk_list.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* return values from HTML Parser API's */
#define  HPR_OK                        ((INT32)   0)
#define  HPR_OUT_OF_MEM                ((INT32)  -1)
#define  HPR_INV_ARG                   ((INT32)  -2)
#define  HPR_INV_ATTR                  ((INT32)  -3)
#define  HPR_UNKNOWN_TAG               ((INT32)  -4)
#define  HPR_NULL_STREAM               ((INT32)  -5)
#define  HPR_PART_MEM_FAIL             ((INT32)  -6)
#define  HPR_CLI_FAIL                  ((INT32)  -7)

typedef  DLIST_T(_HP_EVNT_T)           HP_EVNT_LIST_T;
typedef  DLIST_ENTRY_T(_HP_EVNT_T)     HP_EVNT_LINK_T;
typedef  DLIST_T(_HP_ATTR_T)           HP_ATTR_LIST_T;
typedef  DLIST_ENTRY_T(_HP_ATTR_T)     HP_ATTR_LINK_T;

/* event types of Baseline HTML Parser */
typedef enum _HP_EVNT_TYPE_T
{
    HP_EVNT_TYPE_CHAR_STREAM = 0,      /* character stream */
    HP_EVNT_TYPE_OPEN_HTML,            /* <html> */
    HP_EVNT_TYPE_CLOSE_HTML,           /* </html> */
    HP_EVNT_TYPE_OPEN_BODY,            /* <body> */
    HP_EVNT_TYPE_CLOSE_BODY,           /* </body> */
    HP_EVNT_TYPE_OPEN_ANCHOR,          /* <a href> */
    HP_EVNT_TYPE_CLOSE_ANCHOR,         /* </a> */
    HP_EVNT_TYPE_OPEN_PARAGRAPH,       /* <p> */
    HP_EVNT_TYPE_CLOSE_PARAGRAPH,      /* </p> */
    HP_EVNT_TYPE_LINE_BREAK,           /* <br> */
    HP_EVNT_TYPE_OPEN_BOLD,            /* <b> */
    HP_EVNT_TYPE_CLOSE_BOLD,           /* </b> */
    HP_EVNT_TYPE_OPEN_ITALIC,          /* <i> */
    HP_EVNT_TYPE_CLOSE_ITALIC,         /* </i> */
    HP_EVNT_TYPE_OPEN_UNDERLINED,      /* <u> */
    HP_EVNT_TYPE_CLOSE_UNDERLINED,     /* </u> */
    HP_EVNT_TYPE_OPEN_CENTER,          /* <center> */
    HP_EVNT_TYPE_CLOSE_CENTER,         /* </center> */
    HP_EVNT_TYPE_OPEN_FONT,            /* <font> */
    HP_EVNT_TYPE_CLOSE_FONT            /* </font> */
}   HP_EVNT_TYPE_T;

/* attribute names */
typedef enum _HP_ATTR_NAME_T
{
    HP_ATTR_NAME_BGCOLOR = 0,          /* <body bgcolor> */
    HP_ATTR_NAME_TEXT,                 /* <body text> */
    HP_ATTR_NAME_LINK,                 /* <body link> */
    HP_ATTR_NAME_HREF,                 /* <a href> */
    HP_ATTR_NAME_ALIGN,                /* <p align> */
    HP_ATTR_NAME_COLOR,                /* <font color> */
    HP_ATTR_NAME_SIZE                  /* <font size> */
}   HP_ATTR_NAME_T;

/* alignment types */
typedef enum _HP_ALIGN_TYPE_T
{
    HP_ALIGN_TYPE_LEFT = 0,            /* <p align=left> */
    HP_ALIGN_TYPE_CENTER,              /* <p align=center> or <center> */
    HP_ALIGN_TYPE_RIGHT                /* <p align=right> */
}   HP_ALIGN_TYPE_T;

/* font sizes */
typedef enum _HP_FONT_SIZE_T
{
    HP_FONT_SIZE_SMALL = 0,            /* 2 or below */
    HP_FONT_SIZE_MEDIUM,               /* 3 ~ 4 */
    HP_FONT_SIZE_LARGE                 /* 5 or above */
}   HP_FONT_SIZE_T;


/*---------------------------------------------------------------------------
 Structures
----------------------------------------------------------------------------*/
/* structure of RGB */
typedef struct _HP_RGB_VALUE_T
{
    UINT8                  ui1_red;        /* red value */
    UINT8                  ui1_green;      /* green value */
    UINT8                  ui1_blue;       /* blue value */
}   HP_RGB_VALUE_T;

/* structure of an attribute */
typedef struct _HP_ATTR_T
{
    HP_ATTR_LINK_T         t_attr_link;    /* link list */
    HP_ATTR_NAME_T         e_attr_name;    /* attribute name */
    union
    {
        HP_RGB_VALUE_T     t_rgb_value;    /* rgb value */
        HP_ALIGN_TYPE_T    e_align_type;   /* alignment type */
        HP_FONT_SIZE_T     e_font_size;    /* font size */
        CHAR*              ps_url;         /* URL */
    } u_value;
}   HP_ATTR_T;

/* structure of an event */
typedef struct _HP_EVNT_T
{
    HP_EVNT_LINK_T         t_evnt_link;    /* link list */
    HP_EVNT_TYPE_T         e_evnt_type;    /* event type */
    union
    {
        CHAR*              ps_char_stream; /* character stream */
        HP_ATTR_LIST_T*    pt_attr_list;   /* attribute list */
    } u_content;
}   HP_EVNT_T;

#endif /* _U_HTML_PSR_H_ */
