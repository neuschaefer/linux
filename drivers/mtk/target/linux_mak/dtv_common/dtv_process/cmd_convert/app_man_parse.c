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
 * $RCSfile:  $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *    This file is use for parse AM communication command information.
 *---------------------------------------------------------------------------*/
/*------------------------------------------------------------------------
                    include files
 -----------------------------------------------------------------------*/
#include "app_man_parse.h"

/*------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 -----------------------------------------------------------------------*/
#define DIV_1  ':'
#define DIV_2  ','
#define DIV_3  '='
#define DIV_4  '['
#define DIV_5  ']'
#define DIV_EOF  '\0'

#define CLI_HEADER  "cli"
#define DBG_HEADER  "dbg"

#define DTV_APP_MTK_BTN_VOLUM_UP   "btn_volum_up"
#define DTV_APP_MTK_BTN_VOLUM_DOWN "btn_volum_down"
#define DTV_APP_MTK_BTN_MUTE "btn_mute"

/*------------------------------------------------------------------------
                    functions declarations
 -----------------------------------------------------------------------*/


/*------------------------------------------------------------------------
                    data declarations
 -----------------------------------------------------------------------*/
#if 1
/*------------------------------------------------------------------------
 * Name:  _trim
 *
 * Description: This API trim head and tail space in string.
 *
 * Inputs:  s String which would be trimed.
 *
 * Outputs: -
 *
 * Returns: -
 -----------------------------------------------------------------------*/
static int _trim( char **s )
{
     int i;
     if (s == NULL || *s == NULL)
        return -1;
     int len = strlen( *s );

     for (i = len-1; i >= 0; i--)
          if ((*s)[i] <= ' ')
               (*s)[i] = 0;
          else
               break;

     while (**s)
          if (**s <= ' ')
               (*s)++;
          else
               return 0;
     return 0;
}
#endif

/* [test1,test2,test3] to AMP_DATA_VALUE */
AMP_DATA_VALUE_T * amp_parse_datavalue(const char * s_cmd_str)
{
    
    char * ps_str = 0;
    char * ps_prs_beg = 0;
    char * ps_prs_end = 0;
    AMP_DATA_VALUE_T * pt_data_value = 0;
    AMP_DATA_VALUE_NODE * p_dv = 0;

    if((NULL == s_cmd_str)||('\0' == s_cmd_str[0]))
    {
        return 0;
    }

    ps_str = (char *)calloc( 1, strlen(s_cmd_str) + 1 );

    if( ps_str == NULL)
    {
        return 0;    
    }        
    memset( ps_str, 0, strlen(s_cmd_str)+1 );
    strncpy( ps_str, s_cmd_str, strlen(s_cmd_str) );

    pt_data_value = (AMP_DATA_VALUE_T *)calloc(1,sizeof(AMP_DATA_VALUE_T));
    if(NULL == pt_data_value)
    {    
        free(ps_str);
        return 0;
    }

    /*Have '['?*/
    ps_str[strlen(s_cmd_str)] = '\0';
    ps_prs_beg = ps_str;


    if((ps_prs_end = strchr( ps_prs_beg, DIV_4 )) != NULL)
            {
                ps_prs_beg = ps_prs_end + 1;
                
                /* Have ']'? */
                if((ps_prs_end = strchr( ps_prs_beg, DIV_5 )) != NULL)
                {
                    *ps_prs_end = DIV_EOF;

                    while(ps_prs_beg && ps_prs_beg[0])
                    {
                        p_dv = (AMP_DATA_VALUE_NODE *)calloc(1,sizeof(AMP_DATA_VALUE_NODE));
                        if(NULL == p_dv)
                        {   
                            AMP_DATA_VALUE_NODE * p_pvt = NULL;
                            
                            p_dv = pt_data_value->pt_data_value_node_list_head;
                            while(p_dv != NULL)
                            {
                                p_pvt = p_dv;
                                p_dv = p_dv->pt_next;
                                if(p_pvt->s_value != NULL)
                                {
                                    free(p_pvt->s_value);
                                }
                                free(p_pvt);
                            }
                            free(pt_data_value);   
                            free(ps_str);
                            return 0;
                        }

                        if((ps_prs_end = strchr( ps_prs_beg, DIV_2 )) != NULL)
                        {
                            *ps_prs_end++ = DIV_EOF;
                        }
                        _trim(&ps_prs_beg);
                        p_dv->s_value = (char *)calloc(1,strlen(ps_prs_beg) + 1);
                        if(NULL == p_dv->s_value)
                        {   
                            AMP_DATA_VALUE_NODE * p_pvt = NULL;

                            free(p_dv);
                            
                            p_dv = pt_data_value->pt_data_value_node_list_head;
                            while(p_dv != NULL)
                            {
                                p_pvt = p_dv;
                                p_dv = p_dv->pt_next;
                                if(p_pvt->s_value != NULL)
                                {
                                    free(p_pvt->s_value);
                                }
                                free(p_pvt);
                            }
                            
                            free(pt_data_value);
                            free(ps_str);
                            return 0;
                        }
                                                
                        strncpy( p_dv->s_value, ps_prs_beg, strlen(ps_prs_beg) );

                        if(NULL == pt_data_value->pt_data_value_node_list_head)
                        {
                            pt_data_value->pt_data_value_node_list_head = p_dv;
                            pt_data_value->pt_data_value_node_list_tail = p_dv;
                        }
                        else
                        {
                            p_dv->pt_prev = pt_data_value->pt_data_value_node_list_tail;
                            pt_data_value->pt_data_value_node_list_tail->pt_next = p_dv;
                            pt_data_value->pt_data_value_node_list_tail = p_dv;
                        }

                        ps_prs_beg = ps_prs_end;
                    }
                }
                else
                {   
                    free(pt_data_value);
                    free(ps_str);
                    return 0;
                }
            }
            else
            {
                p_dv = (AMP_DATA_VALUE_NODE *)calloc(1,sizeof(AMP_DATA_VALUE_NODE));
                if(NULL == p_dv)
                {
                    free(pt_data_value);
                    free(ps_str);
                    return 0;
                }
                _trim(&ps_prs_beg);
                p_dv->s_value = (char *)calloc(1,strlen(ps_prs_beg) + 1);
                if(NULL == p_dv->s_value)
                {    
                    free(p_dv);
                    free(pt_data_value);
                    free(ps_str);
                    return 0;
                }
                
                strncpy( p_dv->s_value, ps_prs_beg, strlen(ps_prs_beg));

                pt_data_value->pt_data_value_node_list_head = p_dv;
                pt_data_value->pt_data_value_node_list_tail = p_dv;
            }        
            if (ps_str)
            {
                free(ps_str);
            }
            return pt_data_value;
}


/*------------------------------------------------------------------------
 * Name:  _amp_free
 *
 * Description: This API free the cmd resource.
 *
 * Inputs:  pt_cmd      The command structrue which would be freed.
 *
 * Outputs: -
 *
 * Returns: 0   Executed sucess.
 -----------------------------------------------------------------------*/
int _amp_free (AMP_CMD_T* pt_cmd)
{
    void *pv_temp = 0;
    if(NULL == pt_cmd)
    {
        return 0;
    }
    
    if(pt_cmd->s_src != NULL)
    {
        free(pt_cmd->s_src);
    }
    
    if(pt_cmd->s_dst != NULL)
    {
        free(pt_cmd->s_dst);
    }
    
    if(pt_cmd->s_head_data != NULL)
    {
        free(pt_cmd->s_head_data);
    }

    if(pt_cmd->pt_data_field != NULL)
    {
        while(pt_cmd->pt_data_field->pt_data_field_node_list_head != NULL)
        {
            if(pt_cmd->pt_data_field->pt_data_field_node_list_head->s_keyword != NULL)
            {
                free(pt_cmd->pt_data_field->pt_data_field_node_list_head->s_keyword);
            }

            if(pt_cmd->pt_data_field->pt_data_field_node_list_head->pt_data_value != NULL)
            {
                while(pt_cmd->pt_data_field->pt_data_field_node_list_head->pt_data_value->pt_data_value_node_list_head != NULL)
                {
                    if(pt_cmd->pt_data_field->pt_data_field_node_list_head->pt_data_value->pt_data_value_node_list_head->s_value != NULL)
                    {
                        free(pt_cmd->pt_data_field->pt_data_field_node_list_head->pt_data_value->pt_data_value_node_list_head->s_value);
                    }
                    pv_temp = pt_cmd->pt_data_field->pt_data_field_node_list_head->pt_data_value->pt_data_value_node_list_head->pt_next;
                    free(pt_cmd->pt_data_field->pt_data_field_node_list_head->pt_data_value->pt_data_value_node_list_head);
                    pt_cmd->pt_data_field->pt_data_field_node_list_head->pt_data_value->pt_data_value_node_list_head = (AMP_DATA_VALUE_NODE *)pv_temp;
                }
                free(pt_cmd->pt_data_field->pt_data_field_node_list_head->pt_data_value);
            }

            pv_temp = pt_cmd->pt_data_field->pt_data_field_node_list_head->pt_next;
            free(pt_cmd->pt_data_field->pt_data_field_node_list_head);
            pt_cmd->pt_data_field->pt_data_field_node_list_head =(AMP_DATA_FIELD_NODE*) pv_temp;
        }
        
        free(pt_cmd->pt_data_field);
        
    }

    free(pt_cmd);

    return 0;
    
}

/*------------------------------------------------------------------------
 * Name:  amp_parse_cmd
 *
 * Description: This API parse special cmd string to cmd struct.
 *
 * Inputs:  s_cmd_str String which would be parsed.
 *
 * Outputs: ppt_cmd   the Parsed command information.
 *
 * Returns: 0   Executed sucess.
 *          1   s_cmd_str would not be parsed.
 *          -1  Arguments is invaild.
 *          -2  Memory alloc error.
 -----------------------------------------------------------------------*/
int amp_parse_cmd (const char* s_cmd_str, AMP_CMD_T** ppt_cmd)
{
    char * ps_str = NULL;
    char * ps_beg = NULL;
    char * ps_end = NULL;
    char * ps_prs_beg = NULL;
    char * ps_prs_end = NULL;
    AMP_CMD_T * pt_temp = NULL;
    int i4_ret = 0;

    /*
     *  COMMAND Format:
     *
     *       ---HEAD--- -----------DATA--------------
     *       |        | |                           |
     *      :src,dst,sn:data-field-1,data-field-2,...
     *
     *  data-field Format:
     *  
     *      keyword=[data-value-1,data-value-2,...]
     *      keyword
     *      keyword=data-value
     *  
     */
    
    if(NULL == s_cmd_str)
    {
        i4_ret = -1;
        goto error_format;
    }

    if('\0' == s_cmd_str[0])
    {
        i4_ret = -1;
        goto error_format;
    }
    
    if(NULL == ppt_cmd)
    {
        i4_ret = -1;
        goto error_format;
    }

    ps_str = (char *)calloc( 1, strlen(s_cmd_str) + 1 );
    if(NULL == ps_str)
    {
        i4_ret = -2;
        goto error_format;
    }
    
    strncpy( ps_str, s_cmd_str, strlen(s_cmd_str));

    /* HEAD
     * Format: 
     *      :src,dst,sn:
     */
    if(ps_str && ps_str[0] && ps_str[0] == DIV_1)
    {
        ps_beg = &(ps_str[0]) + 1;
        if((ps_end = strchr( ps_beg, DIV_1 )) != NULL)
        {
            *ps_end++ = DIV_EOF;
        }
        else
        {
           i4_ret = 1;
           goto error_format; 
        }
    }
    else
    {
        i4_ret = 1;
        goto error_format;
    }

    pt_temp = (AMP_CMD_T *)calloc(1,sizeof(AMP_CMD_T));
    if(NULL == pt_temp)
    {
        i4_ret = -2;
        goto error_format;
    }

    ps_prs_beg = ps_beg;

    /* while(ps_prs_beg && ps_prs_beg[0]) */
    {
        if ((ps_prs_end = strchr( ps_prs_beg, DIV_2 )) != NULL)
        {
            *ps_prs_end++ = DIV_EOF;
        }
        else
        {
            i4_ret = 1;
            goto error_format;
        }
        
        if(0 == strlen(ps_prs_beg))
        {
            i4_ret = 1;
            goto error_format;
        }

        /* Src */
        pt_temp->s_src = (char *)calloc(1,strlen(ps_prs_beg) + 1);
        if(NULL == pt_temp->s_src)
        {
            i4_ret = -2;
            goto error_format;
        }
        
        strncpy( pt_temp->s_src, ps_prs_beg, strlen(ps_prs_beg));

        ps_prs_beg = ps_prs_end;

        if ((ps_prs_end = strchr( ps_prs_beg, DIV_2 )) != NULL)
        {
            *ps_prs_end++ = DIV_EOF;
        }
        else
        {
            i4_ret = 1;
            goto error_format;
        }

        if(0 != strlen(ps_prs_beg))
        {
            /* Dst */
            pt_temp->s_dst= (char *)calloc(1,strlen(ps_prs_beg) + 1);
            if(NULL == pt_temp->s_dst)
            {
                i4_ret = -2;
                goto error_format;
            }
            
            strncpy( pt_temp->s_dst, ps_prs_beg, strlen(ps_prs_beg));
        }

        ps_prs_beg = ps_prs_end;

        if(0 != strlen(ps_prs_beg))
        {
            /* SN */
            pt_temp->s_head_data = (char *)calloc(1,strlen(ps_prs_beg) + 1);
            if(NULL == pt_temp->s_head_data)
            {
                i4_ret = -2;
                goto error_format;
            }
            
            strncpy( pt_temp->s_head_data, ps_prs_beg, strlen(ps_prs_beg));
        }  
    }

    /* DATA
     * Format: 
     *      data-field-1,data-field-2,...
     */

    ps_beg = ps_end;

    if(ps_beg && ps_beg[0])
    {
        pt_temp->pt_data_field = (AMP_DATA_FIELD_T*)calloc(1,sizeof(AMP_DATA_FIELD_T));
        if(NULL == pt_temp->pt_data_field)
        {
            i4_ret = -2;
            goto error_format;
        }

        /* If that is CLI and DBG command */
        {
            if(
                (pt_temp->s_src != NULL && 0 == strcmp(pt_temp->s_src, CLI_HEADER)) 
                ||
                (pt_temp->s_dst != NULL && 0 == strcmp(pt_temp->s_dst, CLI_HEADER)) 
                ||
                (pt_temp->s_src != NULL && 0 == strcmp(pt_temp->s_src, DBG_HEADER)) 
                ||
                (pt_temp->s_dst != NULL && 0 == strcmp(pt_temp->s_dst, DBG_HEADER))
               )
            {
                AMP_DATA_FIELD_NODE * p_df = (AMP_DATA_FIELD_NODE*)calloc(1,sizeof(AMP_DATA_FIELD_NODE));
                if(NULL == p_df)
                {
                    i4_ret = -2;
                    goto error_format;
                }
                
                p_df->s_keyword = (char *)calloc(1,strlen(ps_beg) + 1);
                if(NULL == p_df->s_keyword)
                {    
                    free(p_df);
                    i4_ret = -2;
                    goto error_format;
                }
                
                strncpy( p_df->s_keyword, ps_beg, strlen(ps_beg));

                pt_temp->pt_data_field->pt_data_field_node_list_head = p_df;
                pt_temp->pt_data_field->pt_data_field_node_list_tail = p_df;

                goto success_format;
            }
        }
    }
    
    while(ps_beg && ps_beg[0])
    {
        AMP_DATA_FIELD_NODE * p_df = (AMP_DATA_FIELD_NODE*)calloc(1,sizeof(AMP_DATA_FIELD_NODE));
        if(NULL == p_df)
        {    
            i4_ret = -2;
            goto error_format;
        }

        if((ps_end = strchr( ps_beg, DIV_2 )) != NULL)
        {
            *ps_end = DIV_EOF;

            if((ps_prs_end = strchr( ps_beg, DIV_4 )) != NULL)
            {
                *ps_end = DIV_2; /* Recover ',' */
                
                if((ps_end = strchr( ps_beg, DIV_5 )) == NULL)
                {
                    i4_ret = 1;
                    goto error_format;
                }

                ps_end += 1; /* is ',' or '\0' */
            } 
            else
            {
                *ps_end = DIV_2; /* Recover ',' */ 
            }

            if(*ps_end != DIV_EOF)
                *ps_end++ = DIV_EOF;

            
        }

        ps_prs_beg = ps_beg;

        /* Have '='? */
        if((ps_prs_end = strchr( ps_prs_beg, DIV_3 )) != NULL)
        {
            AMP_DATA_VALUE_NODE * p_dv = NULL;
                
            *ps_prs_end++ = DIV_EOF;

            p_df->s_keyword = (char *)calloc(1,strlen(ps_prs_beg) + 1);
            if(NULL == p_df->s_keyword)
            {    
                free(p_df);
                i4_ret = -2;
                goto error_format;
            }
            
            strncpy( p_df->s_keyword, ps_prs_beg, strlen(ps_prs_beg));

            p_df->pt_data_value = (AMP_DATA_VALUE_T *)calloc(1,sizeof(AMP_DATA_VALUE_T));
            if(NULL == p_df->pt_data_value)
            {    
                free(p_df->s_keyword);
                free(p_df);
                i4_ret = -2;
                goto error_format;
            }

            ps_prs_beg = ps_prs_end;

            /* Have '['? */
            if((ps_prs_end = strchr( ps_prs_beg, DIV_4 )) != NULL)
            {
                ps_prs_beg = ps_prs_end + 1;
                
                /* Have ']'? */
                if((ps_prs_end = strchr( ps_prs_beg, DIV_5 )) != NULL)
                {
                    *ps_prs_end = DIV_EOF;

                    while(ps_prs_beg && ps_prs_beg[0])
                    {
                        p_dv = (AMP_DATA_VALUE_NODE *)calloc(1,sizeof(AMP_DATA_VALUE_T));
                        if(NULL == p_dv)
                        {   
                            AMP_DATA_VALUE_NODE * p_pvt = NULL;
                            
                            p_dv = p_df->pt_data_value->pt_data_value_node_list_head;
                            while(p_dv != NULL)
                            {
                                p_pvt = p_dv;
                                p_dv = p_dv->pt_next;
                                if(p_pvt->s_value != NULL)
                                {
                                    free(p_pvt->s_value);
                                }
                                free(p_pvt);
                            }
                            
                            free(p_df->pt_data_value);
                            free(p_df->s_keyword);
                            free(p_df);
                            i4_ret = -2;
                            goto error_format;
                        }

                        if((ps_prs_end = strchr( ps_prs_beg, DIV_2 )) != NULL)
                        {
                            *ps_prs_end++ = DIV_EOF;
                        }
                        
                        p_dv->s_value = (char *)calloc(1,strlen(ps_prs_beg) + 1);
                        if(NULL == p_dv->s_value)
                        {   
                            AMP_DATA_VALUE_NODE * p_pvt = NULL;

                            free(p_dv);
                            
                            p_dv = p_df->pt_data_value->pt_data_value_node_list_head;
                            while(p_dv != NULL)
                            {
                                p_pvt = p_dv;
                                p_dv = p_dv->pt_next;
                                if(p_pvt->s_value != NULL)
                                {
                                    free(p_pvt->s_value);
                                }
                                free(p_pvt);
                            }
                            
                            free(p_df->pt_data_value);
                            free(p_df->s_keyword);
                            free(p_df);
                            i4_ret = -2;
                            goto error_format;
                        }
                                                
                        strncpy( p_dv->s_value, ps_prs_beg, strlen(ps_prs_beg));

                        if(NULL == p_df->pt_data_value->pt_data_value_node_list_head)
                        {
                            p_df->pt_data_value->pt_data_value_node_list_head = p_dv;
                            p_df->pt_data_value->pt_data_value_node_list_tail = p_dv;
                        }
                        else
                        {
                            p_dv->pt_prev = p_df->pt_data_value->pt_data_value_node_list_tail;
                            p_df->pt_data_value->pt_data_value_node_list_tail->pt_next = p_dv;
                            p_df->pt_data_value->pt_data_value_node_list_tail = p_dv;
                        }

                        ps_prs_beg = ps_prs_end;
                    }
                }
                else
                {
                    i4_ret = 1;
                    goto error_format; 
                }
            }
            else
            {
                p_dv = (AMP_DATA_VALUE_NODE *)calloc(1,sizeof(AMP_DATA_VALUE_T));
                if(NULL == p_dv)
                {
                    free(p_df->pt_data_value);
                    free(p_df->s_keyword);
                    free(p_df);
                    i4_ret = -2;
                    goto error_format;
                }
                
                p_dv->s_value = (char *)calloc(1,strlen(ps_prs_beg) + 1);
                if(NULL == p_dv->s_value)
                {    
                    free(p_dv);
                    free(p_df->pt_data_value);
                    free(p_df->s_keyword);
                    free(p_df);
                    i4_ret = -2;
                    goto error_format;
                }
                
                strncpy( p_dv->s_value, ps_prs_beg, strlen(ps_prs_beg));

                p_df->pt_data_value->pt_data_value_node_list_head = p_dv;
                p_df->pt_data_value->pt_data_value_node_list_tail = p_dv;
                
            }
        }
        else
        {
            p_df->s_keyword = (char *)calloc(1,strlen(ps_prs_beg) + 1);
            if(NULL == p_df->s_keyword)
            {    
                free(p_df);
                i4_ret = -2;
                goto error_format;
            }
            
            strncpy( p_df->s_keyword, ps_prs_beg, strlen(ps_prs_beg));
        }

        if(NULL == pt_temp->pt_data_field->pt_data_field_node_list_head)
        {
            pt_temp->pt_data_field->pt_data_field_node_list_head = p_df;
            pt_temp->pt_data_field->pt_data_field_node_list_tail = p_df;
        }
        else
        {
            p_df->pt_prev = pt_temp->pt_data_field->pt_data_field_node_list_tail;
            pt_temp->pt_data_field->pt_data_field_node_list_tail->pt_next = p_df;
            pt_temp->pt_data_field->pt_data_field_node_list_tail = p_df;
        }

        ps_beg = ps_end;
    }

success_format: 

    if(ps_str != NULL)
    {
        free(ps_str);
    }

    *ppt_cmd = pt_temp;

/*    _amp_print(pt_temp);*/
    
    return 0;
  

error_format:
    
    if(ps_str != NULL)
    {
        free(ps_str);
    }

    if(pt_temp != NULL)
    {
        /* Free pt_temp. */

        _amp_free(pt_temp);
    }
    
    return i4_ret;
}

/*------------------------------------------------------------------------
 * Name:  amp_free
 *
 * Description: This API free the cmd resource.
 *
 * Inputs:  pt_cmd      The command structrue which would be freed.
 *
 * Outputs: -
 *
 * Returns: 0   Executed sucess.
 -----------------------------------------------------------------------*/
int amp_free (AMP_CMD_T* pt_cmd)
{
    if(NULL == pt_cmd)
    {
        return 0;
    }

    _amp_free(pt_cmd);
    
    return 0;
}

int amp_free_datavalue(AMP_DATA_VALUE_T * pt_data_value)
{
    AMP_DATA_VALUE_NODE * p_dv = 0;
    AMP_DATA_VALUE_NODE * p_pvt = 0;

    
    if (pt_data_value == 0) return -1;
    
    p_dv = pt_data_value->pt_data_value_node_list_head;
    
    while(p_dv != NULL)
    {
        p_pvt = p_dv;
        p_dv = p_dv->pt_next;
        if(p_pvt->s_value != NULL)
        {
            free(p_pvt->s_value);
        }
        free(p_pvt);
    }
    free(p_dv);
    free(pt_data_value);
    pt_data_value = 0;
    
    return 0;
}


