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
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: nav_cmd.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file nav_cmd.c
 *  Navigator CLI commands
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "vdp_drvif.h"
#include "nav_if.h"
#include "x_drv_cli.h"
#include "x_stl_lib.h"
#include "x_printf.h"
#include "x_ckgen.h"
//#include "x_assert.h"
#include "dmx_if.h"
#include "psipsr_if.h"

#include "x_fm.h"
#include "fm_ufs.h"
#include "x_fm_fat.h"

//---------------------------------------------------------------------
// LOG macro related

// #define DEFINE_IS_LOG	CLI_IsLog
#define DEFINE_IS_LOG CLI_IsLog
#include "x_debug.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

CLIMOD_DEBUG_FUNCTIONS(NAV)

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define MAX_CAL_NS 10

#define CLK_BOUND     1000000    //60000000/256
//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------
#if (defined(CC_MT5365) || defined(CC_MT5395))
// removed!
#else
extern UINT32 dwSI_DISPLAY_DCLK_TYPICAL;
#endif
//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define min(x, y) ((x < y) ? x : y)
#define max(x, y) ((x >= y) ? x : y)

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
EXTERN UINT32 ePIPMode;
EXTERN INT32 NAV_SetMode(UINT32 u4Mode);
EXTERN INT32 NAV_Init(const NAV_OPTION_T* prOpt);

static INT32 _i4CmdMountUsb(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4_ret;
    UINT32 ui4_part_ns;

    UINT32 ui4_cnt;

    /*  0. Init fm */
    if (fm_init() != FMR_OK)    
    {    		
        Printf("\nError: Init FM fail\n");    
    }    
    if (x_fm_fat_init(NULL) != FMR_OK)    
    {    		
    	Printf("\nError: Init FAT fail\n");    
    }

    x_thread_delay(1000);

    /*  1. dev point creation */

    i4_ret = x_fm_create_dir(FM_ROOT_HANDLE, "/dev", 0666);

    if (i4_ret == FMR_OK)
    {
        printf("/dev is created.\n");
    }
    else if (i4_ret != FMR_OK)
    {
        if (i4_ret == FMR_EXIST)
        {
            printf("/dev is already existed.  That is OK.\n");            
        }
        else
        {
            printf("make /dev fail\n");
            printf("return value:%d\n", i4_ret);
            goto func_exit;
        }
    }

    /*  2. mount point creation */

    i4_ret = x_fm_create_dir(FM_ROOT_HANDLE, "/mnt", 0666);

    if (i4_ret == FMR_OK)
    {
        printf("/mnt is created.\n");
    }
    else if (i4_ret != FMR_OK)
    {
        if (i4_ret == FMR_EXIST)
        {
            printf("/mnt is already existed.  That is OK.\n");            
        }
        else
        {
            printf("make /mnt fail\n");
            printf("return value:%d\n", i4_ret);
            goto func_exit;
        }
    }

    /*  3. make entry for USB */

    i4_ret = x_fm_make_entry(FM_ROOT_HANDLE, 
                             "/dev/usb",
                             FM_MODE_TYPE_BLK | FM_MODE_DEV_TRUE | 0660, 
                             FM_DRVT_USB_MASS_STORAGE, 
                             0);

    if (i4_ret == FMR_OK)
    {
        printf("/dev/usb is created.\n");
    }
    else if (i4_ret != FMR_OK)
    {
        if (i4_ret == FMR_EXIST)
        {
            printf("/dev/usb is already existed.  That is OK.\n");            
        }
        else
        {
            printf("make /dev/usb fail\n");
            printf("return value:%d\n", i4_ret);
            goto func_exit;
        }
    }

    /*  4. parse drive  */

    i4_ret = x_fm_parse_drive(FM_ROOT_HANDLE, 
                              "/dev/usb",
                              (VOID *) NULL);

    if (i4_ret != FMR_OK)
    {
        printf("parse drive error.");
    }

    /*  5. get partition ns */

    i4_ret = x_fm_get_part_ns(FM_ROOT_HANDLE, 
                              (const CHAR *) "/dev/usb", 
                              &ui4_part_ns);

    if (i4_ret != FMR_OK)
    {
        printf("x_fm_get_part_ns fails.\n");
        printf("return value:%d\n", i4_ret);
        goto func_exit;
    }

    for (ui4_cnt = 0; ui4_cnt < ui4_part_ns; ui4_cnt++)
    {
        FM_PART_INFO_T t_part_info;

        CHAR ps_dev_name[32];
        CHAR ps_mnt_name[32];

        i4_ret = x_fm_get_part_info(FM_ROOT_HANDLE, 
                                    (const CHAR *) "/dev/usb",
                                    ui4_cnt,
                                    &t_part_info);             

        if (i4_ret != FMR_OK)
        {
            printf("x_fm_get_part_info fails.\n");
            printf("return value:%d\n", i4_ret);
            goto func_exit;
        }

        x_strcpy((CHAR *) ps_dev_name, "/dev/");
        x_strcpy((CHAR *) ps_mnt_name, "/mnt/");

        x_strcat((CHAR *) ps_dev_name, (const CHAR *) t_part_info.ps_part_name);
        x_strcat((CHAR *) ps_mnt_name, (const CHAR *) t_part_info.ps_part_name);

        i4_ret = x_fm_create_dir(FM_ROOT_HANDLE, ps_mnt_name, 0666);

        if (i4_ret == FMR_OK)
        {
            printf("%s is created.\n", ps_mnt_name);
        }
        else if (i4_ret != FMR_OK)
        {
            if (i4_ret == FMR_EXIST)
            {
                printf("%s is already existed.  That is OK.\n", ps_mnt_name);
            }
            else
            {
                printf("make %s fail.\n", ps_mnt_name);
                printf("return value:%d\n", i4_ret);
                goto func_exit;
            }
        }

        i4_ret = x_fm_mount(FM_ROOT_HANDLE, ps_dev_name, FM_ROOT_HANDLE, ps_mnt_name);

        if (i4_ret == FMR_OK)
        {
            printf("%s is mounted as %s.\n", ps_dev_name, ps_mnt_name);
        }
        if (i4_ret != FMR_OK)
        {
            printf("ERR: mount %s as %s fail.\n", ps_dev_name, ps_mnt_name);
            printf("return value:%d\n", i4_ret);
            goto func_exit;
        }
    }

func_exit:
  
    return 0;
}

static INT32 _i4CmdDir(
    INT32 i4_argc,
    const CHAR** ps_argv)
{
    HANDLE_T    h_dir;
    FM_DIR_ENTRY_T at_dir_entry[1];
    UINT32  ui4_num_entries;

    if (i4_argc < 2)
    {
        printf("Usage: dir [directory_name]\n");
        return -1024;
    }    

    if (FMR_OK != x_fm_open_dir(FM_ROOT_HANDLE, ps_argv[1], &h_dir))
    {
        printf("Open this directory fail!\n");
        return -1024;
    }

    while (x_fm_read_dir_entries(h_dir, at_dir_entry, 1, &ui4_num_entries) == FMR_OK)
    {       
        printf("%s\n", at_dir_entry[0].s_name);    
    }
    
    x_fm_close(h_dir);

    return 0;
}


static INT32 _Seperate(const CHAR* szFull, CHAR* szDir, CHAR* szFile)
{
    UINT32 u4Len = x_strlen(szFull);
    INT32 i;
    
    for(i = u4Len - 1; i >= 0; i--)
    {
        if(szFull[i] == '/' || szFull[i] == '\\')
        {
            if(i == u4Len - 1)
            {
                return -1;
            }        
            x_strncpy(szDir, szFull, u4Len - i);
            x_strcpy(szFile, &szFull[i + 1]);	
            return 0;
        }
    }

    x_strcpy(szFile, szFull);	
    
    return 0;
}

static UINT8 _au1Buf[1024];

static INT32 _i4CmdCat(
    INT32 i4_argc,
    const CHAR** ps_argv)
{
    HANDLE_T    h_dir;
    HANDLE_T h_file;

    CHAR szDir[256];
    CHAR szFile[256];

    UINT32 u4Read;
    UINT32 i;
    
    if (i4_argc < 2)
    {
        printf("Usage: cat [file_name]\n");
        return -1024;
    }    

    if(_Seperate(ps_argv[1], szDir, szFile) != 0)
    {
        printf("Open directory %s fail!\n", ps_argv[1]);    
        return -1024;
    }
 
    if (FMR_OK != x_fm_open_dir(FM_ROOT_HANDLE, szDir, &h_dir))
    {
        printf("Open directory %s fail!\n", szDir);
        return -1024;
    }

    if (FMR_OK != x_fm_open(FM_ROOT_HANDLE, ps_argv[1], FM_READ_ONLY, 0777, FALSE, &h_file))
    {
        printf("Open file %s fail!\n", ps_argv[1]);
        return -1024;    
    }

    if (FMR_OK != x_fm_read(h_file, _au1Buf, 1024, &u4Read))
    {
        printf("Read file %s fail!\n", ps_argv[1]);
        return -1024;        
    }
        
    x_fm_close(h_dir);

    for(i = 0; i < u4Read; i++)
    {
        printf("%2X ", _au1Buf[i]);
    }

    return 0;
}


static INT32 _i4CmdViewer(
    INT32 i4_argc,
    const CHAR** ps_argv)
{
    if (i4_argc < 2)
    {
        printf("Usage: viewer [file_name]\n");
        return -1024;
    }    

    DRV_OSD_Init();

    NAV_VIEWER_Init();

    NAV_VIEWER_Start(ps_argv[1]);    

    NAV_VIEWER_Stop();

    return 0;    
}

static INT32 _i4CmdSetDTVScanRegion(INT32 i4Argc, const CHAR ** szArgv)
{
    if(i4Argc < 2)
    {
        return -1024;
    }
	
    return NAV_SetDTVChannelRange(1, StrToInt(szArgv[1]), (i4Argc == 2)? StrToInt(szArgv[1]):StrToInt(szArgv[2]));
}

static INT32 _i4CmdSetDTVSystem(INT32 i4Argc, const CHAR ** szArgv)
{
	if(i4Argc < 2)
	{
		return -1024; // no parameter specifying the system
	}
	else 
	{
		if(x_strcmp(szArgv[1], "atsc") == 0)
		{
		 NAV_SetDTVSystem("Terrestrial");  
		}
		else if(x_strcmp(szArgv[1], "cable") == 0)
		{
			NAV_SetDTVSystem("Cable");	 
		}
		else if(x_strcmp(szArgv[1], "cable256") == 0)
		{
			NAV_SetDTVSystem("Cable");	 
		}
		else if(x_strcmp(szArgv[1], "dvbt") == 0)
		{
			NAV_SetDTVSystem("DVB-T UK");	
		}
		else if(x_strcmp(szArgv[1], "dvbtw") == 0)
		{
			NAV_SetDTVSystem("DVB-T TW");	
		}

		return 0;
	}
}

static INT32 _i4CmdScanDTVChannel(INT32 i4Argc, const CHAR ** szArgv)
{
	UINT32 u4VdpId = (i4Argc < 2) ? VDP_1 : StrToInt(szArgv[1]);

	if(u4VdpId > VDP_2)
	{
		Printf("u4VdpId invalid, please input 0 (main) or 1 (sub).\n");
		return -1;
	}

	if(i4Argc >= 3)
	{
	    NAV_SetDTVChannelRange(1, StrToInt(szArgv[2]), (i4Argc==3)?StrToInt(szArgv[2]):StrToInt(szArgv[3]));
	}

	
	NAV_SetSrc(u4VdpId, E_NAV_VS_DT1); 
	NAV_ScanDTVChannel(u4VdpId);

	return 0;
}

extern BOOL _fgCliATVSearch;
extern UINT32 _i4ATVChStart;
extern UINT32 _i4ATVChEnd;
#ifdef CC_NAV_DEMOD_DVBT
static INT32 _i4CmdScanATVEURange(INT32 i4Argc, const CHAR ** szArgv)
{
  UINT32 u4Start;
  UINT32 u4End;
  UINT32 u4VdpId = (i4Argc < 3) ? VDP_1 : StrToInt(szArgv[1]);
	if(i4Argc < 3)
    {
        return -1024;
    }

	u4Start =StrToInt(szArgv[2]);
	u4End = StrToInt(szArgv[3]);
	if((u4Start<45750)||(u4Start>863250))
		{
		Printf("u4Start invalid, please set 45750-->863250 \n");
		return 0;
		}
	if((u4End<45750)||(u4End>8632500))
		{
		Printf("u4End invalid, please set 45750-->863250 \n");
		return 0;
		}	
    _fgCliATVSearch =TRUE;
    _i4ATVChStart = 1000*u4Start;
    _i4ATVChEnd = 1000*u4End;
	NAV_SetSrc(u4VdpId, E_NAV_VS_TUNER1); 	
	NAV_ScanATVChannel(u4VdpId);
	return 0;	
}
#else
static INT32 _i4CmdScanATVUSRange(INT32 i4Argc, const CHAR ** szArgv)
{
   UINT32 u4Start;
   UINT32 u4End;
   UINT32 u4VdpId = (i4Argc < 3) ? VDP_1 : StrToInt(szArgv[1]);
   if(i4Argc < 3)
    {
        return -1024;
    }
   u4Start =StrToInt(szArgv[2]);
   u4End = StrToInt(szArgv[3]);
	if((u4Start<1)||(u4End>135)||(u4Start>135)||(u4End<1))
		{
		Printf("channel invalid, please set 1-->169 \n");
		return 0;
		}	
	_fgCliATVSearch =TRUE;
    _i4ATVChStart = u4Start;
    _i4ATVChEnd = u4End;
	NAV_SetSrc(u4VdpId, E_NAV_VS_TUNER1); 	
	NAV_ScanATVChannel(u4VdpId);
	return 0;	
}
#endif
static INT32 _i4CmdScanATVChannel(INT32 i4Argc, const CHAR ** szArgv)
{
	UINT32 u4VdpId = (i4Argc < 2) ? VDP_1 : StrToInt(szArgv[1]);

	if(u4VdpId > VDP_2)
	{
		Printf("u4VdpId invalid, please input 0 (main) or 1 (sub).\n");
		return -1;
	}

	NAV_SetSrc(u4VdpId, E_NAV_VS_TUNER1); 	
	NAV_ScanATVChannel(u4VdpId);

	return 0;	
}

static INT32 _i4CmdStopScan(INT32 i4Argc, const CHAR ** szArgv)
{
	NAV_StopScan();

	return 0;	
}

static INT32 _i4CmdPlay(INT32 i4Argc, const CHAR ** szArgv)
{  
    UINT32 u4VPid;
    UINT32 u4APid;
    UINT32 u4PcrPid;
    UINT32 u4Phych;
    //UINT32 u4System;
    UINT32 u4VdpId;
    UINT32 u4VPidType = (i4Argc > 8) ? (StrToInt(szArgv[8])) : (0);
    UINT32 u4APidType = (i4Argc > 9) ? (StrToInt(szArgv[9])) : (0x81);

    BOOL fgQam64;
    
    if(i4Argc < 3)
    {
        return -1024;
    }

    if(x_strcmp(szArgv[1], "m") == 0)
    {
        u4VdpId = 0;
    }
    else if (x_strcmp(szArgv[1], "s") == 0)
    {
        u4VdpId = 1;
    }
    else
    {
        return -1024;
    }

    
    if(x_strcmp(szArgv[2], "DTV") == 0 || x_strcmp(szArgv[2], "dtv") == 0) //treat dtv as special case because it needs more parameters than others
    {
        if(i4Argc > 7)
        {        
            u4VPid = StrToInt(szArgv[3]);
            u4APid = StrToInt(szArgv[4]);        
            u4PcrPid = StrToInt(szArgv[5]);        
    
            if(x_strcmp(szArgv[6], "atsc") == 0)
            {
       	     NAV_SetDTVSystem("Terrestrial");  
            }
            else if(x_strcmp(szArgv[6], "cable") == 0)
            {
                NAV_SetDTVSystem("Cable");   
                fgQam64 = FALSE;
            }
            else if(x_strcmp(szArgv[6], "cable256") == 0)
            {
                NAV_SetDTVSystem("Cable");   
                fgQam64 = TRUE;
            }
            else if(x_strcmp(szArgv[6], "dvbt") == 0)
            {
                NAV_SetDTVSystem("DVB-T UK");   
            }
            else if(x_strcmp(szArgv[6], "dvbtw") == 0)
            {
                NAV_SetDTVSystem("DVB-T TW");   
            }
            else
            {
                return -1024;
            }
            u4Phych = StrToInt(szArgv[7]);             
                   
            NAV_SetPid(u4VPid, u4VPidType, u4APid, u4APidType, u4PcrPid, u4Phych, fgQam64);        
        }
        NAV_SetSrc(u4VdpId, E_NAV_VS_DT1); 
    }
    else
    {
        if(x_strcmp(szArgv[2], "AV1") == 0 || x_strcmp(szArgv[2], "av1") == 0)
        {
            NAV_SetSrc(u4VdpId, E_NAV_VS_CVBS1); 
        }
        else if(x_strcmp(szArgv[2], "AV2") == 0 || x_strcmp(szArgv[2], "av2") == 0)
        {
            NAV_SetSrc(u4VdpId, E_NAV_VS_CVBS2); 
        }
        else if(x_strcmp(szArgv[2], "SV1") == 0 || x_strcmp(szArgv[2], "sv1") == 0)
        {
            NAV_SetSrc(u4VdpId, E_NAV_VS_S1); 
        }        
        else if(x_strcmp(szArgv[2], "SV2") == 0 || x_strcmp(szArgv[2], "sv2") == 0)
        {
            NAV_SetSrc(u4VdpId, E_NAV_VS_S2); 
        }                
        else if(x_strcmp(szArgv[2], "YPBPR1") == 0 || x_strcmp(szArgv[2], "ypbpr1") == 0)
        {
            NAV_SetSrc(u4VdpId, E_NAV_VS_YPbPr1); 
        }                
        else if(x_strcmp(szArgv[2], "YPBPR2") == 0 || x_strcmp(szArgv[2], "ypbpr2") == 0)
        {
            NAV_SetSrc(u4VdpId, E_NAV_VS_YPbPr2); 
        }      
        else if(x_strcmp(szArgv[2], "HDMI1") == 0 || x_strcmp(szArgv[2], "hdmi1") == 0)
        {
            NAV_SetSrc(u4VdpId, E_NAV_VS_HDMI); 
        }                        
        else if(x_strcmp(szArgv[2], "HDMI2") == 0 || x_strcmp(szArgv[2], "hdmi2") == 0)
        {
            NAV_SetSrc(u4VdpId, E_NAV_VS_HDMI2); 
        }                                
        else if(x_strcmp(szArgv[2], "HDMI3") == 0 || x_strcmp(szArgv[2], "hdmi3") == 0)
        {
            NAV_SetSrc(u4VdpId, E_NAV_VS_HDMI3); 
        }                                
        else if(x_strcmp(szArgv[2], "VGA") == 0 || x_strcmp(szArgv[2], "vga") == 0)
        {
            NAV_SetSrc(u4VdpId, E_NAV_VS_VGA); 
        }                                        
        else if(x_strcmp(szArgv[2], "ATV") == 0 || x_strcmp(szArgv[2], "atv") == 0)
        {
            NAV_SetSrc(u4VdpId, E_NAV_VS_TUNER1); 
        }               
        else
        {
            printf("Input source %s not supported!\n", szArgv[2]);
            return -1024;
        }
    }
    return 0;
}

static INT32 _i4CmdSetMode(INT32 i4Argc, const CHAR ** szArgv)
{
    if(i4Argc < 2)
    {
        return -1024;
    }
    if(x_strcmp(szArgv[1], "pip") == 0)
    {
        NAV_SetMode((UINT32)E_NAV_MODE_PIP);
        ePIPMode = (UINT32)E_NAV_MODE_PIP;  
    }
    else if(x_strcmp(szArgv[1], "pop") == 0)
    {
        NAV_SetMode((UINT32)E_NAV_MODE_POP);
        ePIPMode = (UINT32)E_NAV_MODE_POP;      
    }
    else if(x_strcmp(szArgv[1], "single") == 0)
    {
        NAV_SetMode((UINT32)E_NAV_MODE_SINGLE);
        ePIPMode = (UINT32)E_NAV_MODE_SINGLE;      
    }    
    else
    {
        return -1024;
    }
    return 0;
}


static INT32 _i4CmdInit(INT32 i4Argc, const CHAR ** szArgv)
{
    NAV_OPTION_T rOpt =
    {
	TRUE,
	TRUE,
	TRUE,
	TRUE,
	TRUE,
	TRUE,
	FALSE,
	TRUE,
	FALSE,
	TRUE,
       FALSE,
	TRUE,

	TRUE,
	FALSE,

	// Audio enable
#if !defined(__MODEL_slt__)
	TRUE,
#else	
	FALSE,
#endif
	// OSD configuration
	FALSE,

	
	TRUE       
    };

    if(i4Argc < 2)
    {
        rOpt.u4PanelId = 15;
    }
    else
    {
        rOpt.u4PanelId = StrToInt(szArgv[1]);
    }
    NAV_Init(&rOpt);

    return 0;
}

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _TestPll(CAL_SRC_T eSrc, UINT32 u4Target, UINT32 u4Bound)
{
#if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
    // MT5365_TODO
    return 0;

#else // !CC_MT5365

    INT32 i4Fail = 0;
    UINT32 u4CalNs = 0, u4Clock;
    UINT8 ulBand;
    UINT16 u2MS, u2NS, u2Counter;

    switch (eSrc)
    {
    case CAL_SRC_DMPLL:
        Printf("test DMPLL: ");
        break;
    case CAL_SRC_SYSPLL:
        Printf("test SYSPLL: ");
        break;
    //case CAL_SRC_CPUPLL:
        //Printf("test CPUPLL: ");
        //break;
    #if !defined(CC_MT5396)
    case CAL_SRC_VOPLL:
        Printf("test VOPLL: ");
        break;
    #endif        
    #if !defined(CC_MT8223)
    case CAL_SRC_TVDPLL:
        Printf("test TVDPLL: ");
        break;
    case CAL_SRC_DTDPLL:
        Printf("test DTDPLL: ");
        break;  
    #endif
    case CAL_SRC_APLL1:
        Printf("test APLL1: ");
        break;
    case CAL_SRC_APLL2:
        Printf("test APLL2: ");
        break;
    default:
        return -1;
    }

    if (eSrc == CAL_SRC_APLL1 || eSrc == CAL_SRC_APLL2)
    {
        BSP_Calibrate(eSrc, u4Target);        
    }
    
    do
    {
        x_thread_delay(5);
        u4Clock = BSP_GetClock(eSrc, &ulBand, &u2MS, &u2NS, &u2Counter);

        if ((u4Clock > u4Target + u4Bound) ||
            (u4Clock < u4Target - u4Bound))
        {
            Printf("FAIL: u4CalNs=%d, u4Target=%d, u4Clock=%d, ulBand=%d, "
                   "u2MS=%d, u2NS=%d, u2Counter=%d)\n",
                   u4CalNs, u4Target, u4Clock, ulBand, u2MS, u2NS, u2Counter);
            i4Fail++;
        }

#if 0
        if (eSrc == CAL_SRC_CPUPLL || eSrc == CAL_SRC_APLL0 ||
            eSrc == CAL_SRC_APLL1)
#else
        if (eSrc == CAL_SRC_APLL1 || eSrc == CAL_SRC_APLL2)
#endif
        {
            BSP_Calibrate(eSrc, u4Target);
            ++u4CalNs;
        }
        else
        {
            u4CalNs = MAX_CAL_NS;
        }
    } while (u4CalNs < MAX_CAL_NS);

    if (i4Fail == 0)
    {
        Printf("PASS\n");
    }

    return i4Fail;

#endif // CC_MT5365
}

#define TESTPLL(a, b, c)        \
{                               \
    if(_TestPll(a, b, c)){      \
        CLI_Parser("pmx.p 1");  \
        ASSERT(0);              \
    }                           \
}         

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _i4CmdPll(INT32 i4Argc, const CHAR ** szArgv)
{
#if !defined(CC_MT5368) && !defined(CC_MT5396) && !defined(CC_MT5389)

    INT32 dramclk = (i4Argc)>1?StrToInt(szArgv[1]):391500000;
    TESTPLL(CAL_SRC_DMPLL, dramclk, ((dramclk / 1000) * 7));   
    
    #ifdef CC_MT5395
    TESTPLL(SRC_SYSPLL_CLK, 459000000, 1000000);
    #else
    TESTPLL(CAL_SRC_SYSPLL, 324000000, CLK_BOUND);
    #endif
    #if (defined(CC_MT5365) || defined(CC_MT5395))
    TESTPLL(CAL_SRC_VOPLL, PANEL_GetPixelClk60Hz(), CLK_BOUND);
    #else
    TESTPLL(CAL_SRC_LVDS_DPIX_CK, dwSI_DISPLAY_DCLK_TYPICAL, CLK_BOUND);
    #endif
    #if (!defined(CC_MT8223) && !defined(CC_MT5365) && !defined(CC_MT5395))
    // MT5365_TODO
    TESTPLL(CAL_SRC_TVDPLL, 540000000, CLK_BOUND);
    TESTPLL(CAL_SRC_DTDPLL, 600000000, CLK_BOUND);
    #endif

#endif // CC_MT5368
    
    TESTPLL(CAL_SRC_APLL1, CLK_APLL294MHZ, CLK_BOUND);
    TESTPLL(CAL_SRC_APLL1, CLK_APLL270MHZ, CLK_BOUND);
    TESTPLL(CAL_SRC_APLL2, CLK_APLL270MHZ, CLK_BOUND);
    TESTPLL(CAL_SRC_APLL2, CLK_APLL294MHZ, CLK_BOUND);
    return 0;  
}

//=====================================================================
// CLI command table
//=====================================================================

static CLI_EXEC_T _arNavCmdTbl[] =
{
    // LOG macro support    
	CLIMOD_DEBUG_CLIENTRY(NAV),
	{
		"init", "i", _i4CmdInit, NULL, "", CLI_GUEST
	},
	{
		"play", "play", _i4CmdPlay, NULL, "nav.play [source] [v-pid] [a-pid] [pcr-pid] [atsc/cable] [physical channel number]", CLI_GUEST
	},	
	{
		"setscanregion", "ssr", _i4CmdSetDTVScanRegion, NULL, "nav.ssr [start channel] [end channel]", CLI_GUEST
	},
	{
		"scandtv", "sdtv", _i4CmdScanDTVChannel, NULL, "nav.sdtv [0|1]", CLI_GUEST
	},
	{
		"setdtvsys", "sdsys", _i4CmdSetDTVSystem, NULL, "nav.sdsys [atsc|dvbt|cable|cable256|dvbtw]", CLI_GUEST
	},	
#ifdef CC_NAV_DEMOD_DVBT
	{
		"scanAtvEUregion", "satveu", _i4CmdScanATVEURange, NULL, "nav.satveu [0|1][start freq] [end freq]", CLI_GUEST
	},
#else
	{
		"scanAtvUSregion", "satvus", _i4CmdScanATVUSRange, NULL, "nav.satvus [0|1][start channel] [end channel]", CLI_GUEST
	},
#endif
	{
		"scanatv", "satv", _i4CmdScanATVChannel, NULL, "nav.satv [0|1]", CLI_GUEST
	},	
	{
		"stopscan", "stopsc", _i4CmdStopScan, NULL, "nav.stopscan", CLI_GUEST
	},		
	{
		"mountusb", "mu", _i4CmdMountUsb, NULL, "nav.mu", CLI_GUEST
	},
	{
		"setmode", "sm", _i4CmdSetMode, NULL, "nav.sm [pic mode]", CLI_GUEST
	},		
	{
		"dir", "dir", _i4CmdDir, NULL, "nav.dir", CLI_GUEST
	},
	{
		"cat", "cat", _i4CmdCat, NULL, "nav.cat", CLI_GUEST
	},	
	{
		"viewer", "viewer", _i4CmdViewer, NULL, "nav.viewer", CLI_GUEST
	},		 
	{
		"pll", "pll", _i4CmdPll, NULL, "nav.pll [dramclk]", CLI_GUEST
	},			 	
	{
		NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR
	}   
};
  

CLI_MAIN_COMMAND_ITEM(Nav)
{

    "nav", "nav", NULL, _arNavCmdTbl, "Navigator commands", CLI_GUEST

};

