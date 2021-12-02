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
 * $RCSfile: main.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *---------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <time.h>

#include "keyword.h"
#include "x_dlm.h"
#include "dlm_fm.h"
#include "dlm.h"

/* Header storage */
DEFAULT_CODE_FILE_HDR_T    hdr;
/* Append list storage */
Append_T    *append = NULL;

/* output file */
char gOutputFileName[128];

int parse_script_file(char *filename);

static void n_string(char *str)
{
    int len = strlen (str);

    while (len)
    {
        if ((*str > 0x0) && 
            (*str < ' '))
            *str = 0x0; /* replace with space char */
        str++; len --;
    }
}

int parse_script_file(char *filename)
{
    FILE *in_file;
    char line1[256], line2[256];
    char *s_token;
    char delimit[] = " \r\n\t,";

    memset((void*) &hdr, 0, sizeof(DEFAULT_CODE_FILE_HDR_T));

    if ((in_file = fopen(filename, "r")) == NULL)
    {
        printf("Can not locate file -> %s\n", filename);
        exit(-1);
    }

    while (!feof(in_file))
    {
        if (fgets(line1, 256, in_file) == NULL)
            continue;
        /* backup, casue strtok will modify content */
        strcpy(line2, line1);
        s_token = strtok(line1, delimit);

        if (!s_token)
            continue;

        if (s_token[0] == '#')
        {
            continue;
        }

        if (strcmp(kVID, s_token) == 0)
        {
            s_token = strtok(NULL, delimit);
            memcpy(hdr.uac_id, s_token, 4);
        }
        else if (strcmp(s_token, kModeName) == 0)
        {
            s_token = strtok(NULL, delimit);
            strcpy(hdr.s_model, line2+ (s_token - line1));
            n_string(hdr.s_model);
        }
        else if (strcmp(s_token, kControl) == 0)
        {
            /* control [ignore, id, major_version, minor_version, model] */
            while ((s_token = strtok(NULL, delimit))!= NULL)
            {
                if (strcmp (s_token, kVID) == 0)
                {
                    hdr.uac_control[0] |= kValidateID;
                    continue;
                }

                if (strcmp (s_token, kControl_ignore) == 0)
                {
                    continue;
                }

                if (strcmp (s_token, kControl_version) == 0)
                {
                    hdr.uac_control[0] |= kValidateVersion;
                    continue;
                }

                if (strcmp (s_token, kModeName) == 0)
                {
                    hdr.uac_control[0] |= kValidateModelName;
                    continue;
                }
            }
        }
        else if (strcmp(s_token, kAppend) == 0)
        {
            Append_T *pt_new;

            if ((pt_new = (Append_T *) malloc (sizeof (Append_T)))== NULL)
            {
                printf("Allocate memory failed\n");
                exit(-1);
            }

            memset(pt_new, 0, sizeof (Append_T));

            /* privacy */
            s_token = strtok(NULL, delimit);
            if (strcmp(kAppend_nPriv, s_token) == 0)
                pt_new->uac_privacy[0] = (char) 0;
            else
                pt_new->uac_privacy[0] = (char) 1;

            /* type */   
            s_token = strtok(NULL, delimit);
            x_memcpy(pt_new->uc_type,s_token,4);

            /* filename */
            s_token =  strtok(NULL, delimit);
            pt_new->ps_filename = (char *) malloc (strlen(s_token) +1);
            strcpy(pt_new->ps_filename, s_token);

            /* insert to list */
            if (append != NULL)
            {
                pt_new->pt_next = append;
                append = pt_new;
            }
            else
                append = pt_new;
        }
        else if (strcmp(s_token, kOutput) == 0)
        {
            s_token = strtok(NULL, delimit);
            strcpy(gOutputFileName, s_token);
        }
    }
    fclose (in_file);

    return 0;
}

void free_append()
{
    Append_T *pt_tmp = append;

    while (pt_tmp)
    {
        Append_T *pt_free;
        pt_free = pt_tmp;
        free (pt_tmp->ps_filename);
        pt_tmp = pt_tmp->pt_next;
        free (pt_free);
    }
}

void Install_DLM_CallBack();

void download_test(char *file)
{
        int ret;
        DEFAULT_CODE_FILE_HDR_T t_code_hdr;
        
        memset((void*)&t_code_hdr, 0, sizeof(DEFAULT_CODE_FILE_HDR_T));
        memcpy(t_code_hdr.uac_id, "cmtk", 4);
        memcpy(t_code_hdr.s_model, "mt5370_demo 112 333", sizeof("mt5370_demo 112 333"));
        strcpy(t_code_hdr.s_version, "CQAM_IDTV_001027_7");

        /* register my append data callback API */
        Install_DLM_CallBack();
        
        x_dlm_boot_init((CHAR*) &t_code_hdr);
        x_dlm_set_ignore_check_item(0xf0);
        ret = x_dlm_integrity_check(file, NULL);

        // Test case...
        //ret = x_dlm_integrity_check("mt5380_demo_dbg.bin", NULL);  /* wrong format, should not upgrade */
        //ret = x_dlm_integrity_check("a.bat", NULL);  /* file size small than our header, should not upgrade */

        if (ret == 0)
        {
            printf("Do firmware upgrade\n");
            x_dlm_download(file, NULL, 0);
            //x_dlm_download_by_tag(file, "fir2", NULL);
        }
        else
            printf("No proper firmware exist\n"); 
}

INT32 test_type_1(DLM_ACCESS_T e_flag, UCHAR *pauc_buffer, INT32 *ui4_len)
{
    //INT32  out_file;
    //static UINT32 cnt = 0;

    //out_file = dlm_fopen ("type_1.bin", kDlm_Append);
    //dlm_fwrite(out_file, (CHAR*)pauc_buffer, *ui4_len);
    /*dlm_dump_buffer("", pauc_buffer, ui4_len);*/
    //dlm_fclose(out_file);
    //cnt += *ui4_len;
    /*DLM_TRACE(("Total %u bytes in type_1\n", cnt));*/
    return 0;
}

INT32 test_type_fir2(DLM_ACCESS_T e_flag, UCHAR *pauc_buffer, INT32 *ui4_len)
{
    INT32  out_file;
    //static UINT32 cnt = 0;

    out_file = dlm_fopen ("type_fir2.bin", kDlm_Append);
    dlm_fwrite(out_file, (CHAR*)pauc_buffer, *ui4_len);
    /*dlm_dump_buffer("", pauc_buffer, ui4_len);*/
    dlm_fclose(out_file);
    //cnt += *ui4_len;
    /*DLM_TRACE(("Total %u bytes in type_1\n", cnt));*/
    return 0;
}

INT32 test_type_2(DLM_ACCESS_T e_flag, UCHAR *pauc_buffer, INT32 *ui4_len)
{
    INT32 out_file;
    static UINT32 cnt = 0;

    out_file = dlm_fopen ("type_2.bin", kDlm_Append);
    dlm_fwrite(out_file, (CHAR *)pauc_buffer, *ui4_len);
    /*dlm_dump_buffer("", pauc_buffer, ui4_len); */
    dlm_fclose(out_file);
    cnt += *ui4_len;
    /*DLM_TRACE(("Total %u bytes in type_2\n", cnt));*/
    return 0;
}

VOID Install_DLM_CallBack()
{
    x_dlm_reg_append_callback("scn1", 131072, test_type_1);
    x_dlm_reg_append_callback("rcps", 131072, test_type_1);
    x_dlm_reg_append_callback("elnk", 131072, test_type_1);
    x_dlm_reg_append_callback("pqda", 131072, test_type_1);
    x_dlm_reg_append_callback("font", 131072, test_type_1);
    x_dlm_reg_append_callback("v4uu", 131072, test_type_1);
    x_dlm_reg_append_callback("mic2", 131072, test_type_1);
    x_dlm_reg_append_callback("micp", 131072, test_type_1);
    x_dlm_reg_append_callback("fir2", 131072, test_type_fir2);    
    x_dlm_reg_append_callback("firm", 131072, test_type_2);
}

VOID printf_buffer(char *ps_msg, UCHAR* puac_buf, UINT32 ui4_len)
{
    INT32 i4_i, i4_j, i4_cnt = 16;
    UCHAR *pc_tmp = puac_buf;

    printf("%s\n", ps_msg);

    for (i4_i =0; i4_i < (INT32) ui4_len; i4_i++)
    {
        i4_cnt--;
        printf("%02x ", *(pc_tmp++));

        if (i4_cnt == 0)
        {
            printf("   ");
            for (i4_j=0; i4_j<16; i4_j++)
            {
                if (i4_i != 0)
                {
                    if(*(pc_tmp-16+i4_j) > 32 && 
                       *(pc_tmp-16+i4_j) <= 126)
                    {
                        printf("%c", *(pc_tmp-16+i4_j));
                    }
                    else
                        printf(".");
                }
            }
            printf("\n");
            i4_cnt = 16;
        }
    }
    printf("\n");
}



int main(int argc, char *argv[])
{
    static unsigned char digest[32];
    int ret;
    Append_T *pt_tmp;
    time_t build_time;
#if 1    
    if (argc < 5)
    {
        printf("\nUsage: codefile script output_file customer model version\n");
        printf("[script],      command file for this tool\n");
        printf("[customer],    name of customer\n");
        printf("[model],       model name\n");
        printf("[version],     firmware version\n");
        exit(0);
    }
    printf("[AES],     firmware encrypt with AES algorithm\n");
    time(&build_time);
    
    printf("Read script file -> %s\n",argv[1]);
    memset ((char*) &hdr, 0, sizeof (DEFAULT_CODE_FILE_HDR_T));
    printf("sizeof (DEFAULT_CODE_FILE_HDR_T) %d\n",sizeof (DEFAULT_CODE_FILE_HDR_T));
    parse_script_file(argv[1]);
   
    printf("Vendor name = %s\n", argv[2]);
    printf("Model name  = %s\n", argv[3]);
    printf("Version     = %s\n", argv[4]);
    printf("Control     = ");
    if (hdr.uac_control[0])
    {
        if (hdr.uac_control[0] & kValidateID)
        {
            printf("id ");
        }
        if (hdr.uac_control[0] & kValidateVersion)
        {
            printf("version ");
        }
        if (hdr.uac_control[0] & kValidateModelName)
        {
            printf("model ");
        }
        printf("\n");
    }
    else 
    {
        printf("Only check file consistency\n");
    }


    printf("Content: \n");
    pt_tmp = append;
    while (pt_tmp)
    {
        printf("Append %s, privacy = %d, tag = %c%c%c%c\n", 
               pt_tmp->ps_filename, 
               pt_tmp->uac_privacy[0], 
               pt_tmp->uc_type[0],
               pt_tmp->uc_type[1],
               pt_tmp->uc_type[2],
               pt_tmp->uc_type[3]);
        pt_tmp = pt_tmp->pt_next;
    }
    printf("\n");
    
    /* build up header */
    //hdr.ui4_time = (UINT32) build_time;
    
    // ID
    if (strlen(argv[2])>4)
    {
        memcpy(hdr.uac_id, argv[2],4);
    }
    else
        strcpy(hdr.uac_id, argv[2]);
        
    // model
    strcpy(hdr.s_model, argv[3]);
    
    printf_buffer("hdr.s_model :\n",hdr.s_model, 32);
    
    // Version
    strcpy(hdr.s_version, argv[4]);


    printf_buffer("Header File", &hdr, sizeof (DEFAULT_CODE_FILE_HDR_T));
    
    printf("generate code file -> %s\n", gOutputFileName);
    ret = x_dlm_generate_code_file(&hdr,append, gOutputFileName);
    if (ret != DLM_OK)
    {
        printf("generate code file failed -> %d\n", ret);
        exit(ret);
    }
    
    /* Compute code hash */
    ret = x_dlm_generate_hash(gOutputFileName, digest);
    if (ret != DLM_OK)
    {
        printf("generate code file hash failed -> %d\n", ret);
        exit(ret);
    }        
    
    /* update code file has */
    ret = x_dlm_update_signature(gOutputFileName, digest);
    if (ret != DLM_OK)
    {
        printf("update code file hash failed -> %d\n", ret);
        exit(ret);
    }        
#else    
    //download_test(gOutputFileName);
    download_test("philips_LC09M_IDTV0801_001030_4_001_58_cli_MTK_LC09M_cli_code_ap.pkg");
#endif
    free_append();
    return 0;
}

