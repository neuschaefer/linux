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
 * $RCSfile: swdmx_cmd.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file swdmx_cmd.c
 *  This file contains implementation of CLI CMD for software PS demux
 *
 */


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

/*lint -save -e534 -e715 -e786 -e818 -e826 -e830 -e950 -e957 -e960 -e641
             -e613 -e836 */

#include "x_drv_cli.h"
#include "x_stl_lib.h"
#include "x_printf.h"
#include "x_assert.h"
#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "x_os.h"

#include "swdmx_if.h"
#include "swdmx_drvif.h"
#include "vdec_if.h"
#include "vdec_drvif.h"

#define DEFINE_IS_LOG	SWDMX_IsLog
#include "x_debug.h"

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
// t32 host command
enum
{
	E_T32_NOP,
	E_T32_EXEC,
	E_T32_RESTART,
	E_T32_EXIT,
	E_T32_EXEC_RELOAD_SYM,
	E_T32_DETECT_HOST,
	E_T32_END
};


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------
//#define SWDMX_SIMULATION_CODE
#define INDEX_FILE_SIZE 5*100*1024
#define SWDMX_CMD_SWDMX_0 (0)
//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

CLIMOD_DEBUG_FUNCTIONS(SWDMX)

//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

#ifdef SWDMX_SIMULATION_CODE
static DEMUX_RANGE_INFO_T 		_rDmxRange;
static MIDXBULD_INDEX_TBL_T 		_rBuldIdxTbl;
static MIDXBULD_ELM_INDEX_TBL_T	_rBuldElmIdxTbl;

static DEMUX_RANGE_INFO_T 		_rDmxRange1;
static MIDXBULD_INDEX_TBL_T 		_rBuldIdxTbl1;
static MIDXBULD_ELM_INDEX_TBL_T	_rBuldElmIdxTbl1;
static char *_pcFileInfo = NULL;
#endif

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

static INT32 _SwdmxCmdInit          (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SwdmxCmdOpen          (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SwdmxCmdPlay          (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SwdmxCmdStop          (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SwdmxCmdQuery         (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SwdmxCmdTest          (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SwdmxCmdTestFeeder    (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SwdmxCmdTestMPG       (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SwdmxCmdSetSpeed      (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SwdmxCmdSetPosition   (INT32 i4Argc, const CHAR ** szArgv);

#ifdef SWDMX_STATISTICS
static INT32 _SwdmxResetTime        (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SwdmxPrintTime        (INT32 i4Argc, const CHAR ** szArgv);
#endif
//static INT32 _SwdmxCmdEnForceStop   (INT32 i4Argc, const CHAR ** szArgv);

#ifdef CC_SWDMX_RENDER_PTS
static INT32 _SwdmxSetRenderPts (INT32 i4Argc, const CHAR ** szArgv);
#endif

#ifdef CC_SWDMX_RENDER_POSITION_TEST
static INT32 _SwdmxGetRenderPosition (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SwdmxSetRenderPosition (INT32 i4Argc, const CHAR ** szArgv);
#endif

#ifdef SWDMX_DBG_USB
static VOID _SwdmxCmdPutDbgConfig( UINT32 u4SrcId, MM_DBG_INFO_T *prDbgInfo );
static UINT32 _SwdmxCmdFindSwitch(INT32 i4Argc, const CHAR **szArgv, const CHAR *szSwitch);
static INT32 _SwdmxCmdDebugConfig (INT32 i4Argc, const CHAR ** szArgv);
#endif // SWDMX_DBG_USB

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

// cmd table
static CLI_EXEC_T _arSwdmxCmdTbl[] =
{
    {
        "i", NULL, _SwdmxCmdInit, NULL,
        "Init software demuxer", CLI_SUPERVISOR
    },
    {
        "o", NULL, _SwdmxCmdOpen, NULL,
        "Open software demuxer", CLI_SUPERVISOR
    },
    {
        "p", NULL, _SwdmxCmdPlay, NULL,
        "Run the software demuxer to play AV", CLI_SUPERVISOR
    },
    {
        "s", NULL, _SwdmxCmdStop, NULL,
        "Stop the software demuxer to play AV", CLI_SUPERVISOR
    },
    {
        "q", NULL, _SwdmxCmdQuery, NULL,
        "Query the software demuxer information", CLI_SUPERVISOR
    },
    {
        "t", NULL, _SwdmxCmdTest, NULL,
        "Test the software demuxer to play AV", CLI_SUPERVISOR
    },

    {
        "tfeeder", NULL, _SwdmxCmdTestFeeder, NULL,
        "Test the software feeder to swdmx", CLI_SUPERVISOR
    },

    {
        "tmpg", NULL, _SwdmxCmdTestMPG, NULL,
        "Test the software demuxer to play MPG PS", CLI_SUPERVISOR
    },

    #ifdef SWDMX_STATISTICS
    {
        "rt", NULL, _SwdmxResetTime, NULL,
        "Reset time", CLI_SUPERVISOR
    },
    {
        "pt", NULL, _SwdmxPrintTime, NULL,
        "Print time", CLI_SUPERVISOR
    },
    #endif

    #ifdef CC_SWDMX_RENDER_PTS
    {
        "srpts", NULL, _SwdmxSetRenderPts, NULL,
        "Set Render Pts", CLI_SUPERVISOR
    },
    #endif

    #ifdef CC_SWDMX_RENDER_POSITION_TEST
    {
        "grpos", NULL, _SwdmxGetRenderPosition, NULL,
        "Get Render Position", CLI_SUPERVISOR
    },

    {
        "srpos", NULL, _SwdmxSetRenderPosition, NULL,
        "Set Render Position", CLI_SUPERVISOR
    },
    #endif

    {
        "sp", NULL, _SwdmxCmdSetSpeed, NULL,
        "Set Speed (1000, 2000, -1000)", CLI_SUPERVISOR
    },

    {
        "spos", NULL, _SwdmxCmdSetPosition, NULL,
        "Set Position (0 ~ 1000)", CLI_SUPERVISOR
    }, 
#ifdef SWDMX_DBG_USB
    {
        "d", NULL, _SwdmxCmdDebugConfig, NULL,
        "Set debug state: swdmx.d [instance_num [-s code][-f/-f1 fileName][-m/-m1 openMode]]", CLI_SUPERVISOR
    },
#endif // SWDMX_DBG_USB
    /*{
        "fstop", NULL, _SwdmxCmdEnForceStop, NULL,
        "Enable force stop condition", CLI_SUPERVISOR
    },*/

    CLIMOD_DEBUG_CLIENTRY(SWDMX),     // for LOG use

    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};


//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------
#ifdef SWDMX_DBG_USB
/*
function:   print the debug infomation of a instance
*/
static VOID _SwdmxCmdPutDbgConfig( UINT32 u4SrcId, MM_DBG_INFO_T *prDbgInfo )
{
    UINT32 u4Mode = prDbgInfo->u4Mode;

    Printf( "Instance:%d LOGCRC:%-3s SAVECRC:%-3s AUTOCHECK:%-3s SAVEDATA:%-3s SAVEBLOCK:%-3s\n",
            u4SrcId,
            (0 != MM_DBG_MODE_VALUE(u4Mode, MM_DBG_MODE_LOGCRC) )  ? 
            (1 == MM_DBG_MODE_VALUE(u4Mode, MM_DBG_MODE_LOGCRC) ? "01" 
            : (2 == MM_DBG_MODE_VALUE(u4Mode, MM_DBG_MODE_LOGCRC) ? "10" : "11") )
            : "00",
            
            (0 != MM_DBG_MODE_VALUE(u4Mode, MM_DBG_MODE_SAVECRC))  ? 
            (1 == MM_DBG_MODE_VALUE(u4Mode, MM_DBG_MODE_SAVECRC) ? "01" 
            : (2 == MM_DBG_MODE_VALUE(u4Mode, MM_DBG_MODE_SAVECRC) ? "10" : "11") )
            : "00",
            
            (0 != MM_DBG_MODE_VALUE(u4Mode, MM_DBG_MODE_AUTOCHECK) )  ? 
            (1 == MM_DBG_MODE_VALUE(u4Mode, MM_DBG_MODE_AUTOCHECK) ? "01" 
            : (2 == MM_DBG_MODE_VALUE(u4Mode, MM_DBG_MODE_AUTOCHECK) ? "10" : "11") )
            : "00",
            
            (0 != MM_DBG_MODE_VALUE(u4Mode, MM_DBG_MODE_SAVEDATA) )  ? 
            (1 == MM_DBG_MODE_VALUE(u4Mode, MM_DBG_MODE_SAVEDATA) ? "01" 
            : (2 == MM_DBG_MODE_VALUE(u4Mode, MM_DBG_MODE_SAVEDATA) ? "10" : "11") )
            : "00" ,

            (0 != MM_DBG_MODE_VALUE(u4Mode, MM_DBG_MODE_SAVEBLOCK))   ? "11" : "00" );
        
    return ;
}
        
#define DBG_GET_FILENAMEPOS( NamePos, cmdSwitch )                   \
do {                                                                                               \
    u4Pos = _SwdmxCmdFindSwitch( i4Argc, szArgv, cmdSwitch );         \
    if ( 0 != u4Pos && u4Pos + 1 < i4Argc )                                             \
    {                                                                                                \
        NamePos = u4Pos + 1;                                                              \
    }                                                                                                \
} while ( FALSE )
        
#define DBG_GET_FILEOPENMODE( OpenMode, cmdSwitch, fgChange )  \
do {                                                                                                 \
    u4Pos = _SwdmxCmdFindSwitch( i4Argc, szArgv, cmdSwitch );           \
    if ( 0 != u4Pos && u4Pos + 1 < i4Argc )                                               \
    {                                                                                                  \
        OpenMode = szArgv[u4Pos + 1][0] == '1' ? TRUE : FALSE;                 \
        fgChange = TRUE;                                                                      \
    }                                                                                                  \
} while ( FALSE )

#define DBG_GET_DEFAULT_FILENAME( szFileName, srcId, name )         \
do {                                                                                                   \
    x_memset(szFileName, 0, 256);                                                       \
    UNUSED( x_sprintf(szFileName, "%s/%d_%s", FILE_LOCAL, srcId,  name ) );  \
} while (FALSE)

#define DBG_GET_USER_FILENAME( szFileName, pos )                            \
do {                                                                                               \
    x_memset(szFileName, 0, 256);                                                                   \
    UNUSED( x_sprintf(szFileName, "%s/%s", FILE_LOCAL, szArgv[ pos ] ) );     \
} while ( FALSE )

static UINT32 _SwdmxCmdFindSwitch(INT32 i4Argc, const CHAR **szArgv, const CHAR *szSwitch)
{
    UINT32 u4Pos = 1;

    if ( NULL != szSwitch )
    {
        for ( ; u4Pos < i4Argc; u4Pos++ )
        {
            if ( '-' == szArgv[ u4Pos ][ 0 ] )
            {
                if( 0 == x_strcmp( &szArgv[ u4Pos ][ 1 ], szSwitch ) )
                {
                    return u4Pos;
                }
            }
        }
    }
    
    return 0;
}

static INT32 _SwdmxCmdDebugConfig (INT32 i4Argc, const CHAR ** szArgv)
{
   MM_DBG_INFO_T *prDbgInfo = NULL;
   UINT32 u4SrcId = 0;
   UINT32 u4Pos = 0;
   
   if ( 1 == i4Argc ) // for old branch || 0 != i4Argc%2 )
   {
        // print all instance state
        for ( u4SrcId=0; SWDMX_SOURCE_MAX>u4SrcId; u4SrcId++ )
        {
            SWDMX_SetInfo(u4SrcId, eSWDMX_SET_DBGINFO, (UINT32)&prDbgInfo, 0, 0);
            _SwdmxCmdPutDbgConfig( u4SrcId, prDbgInfo );
        }
   }
   else if ( 2 == i4Argc )
   {
       u4Pos = _SwdmxCmdFindSwitch( i4Argc, szArgv, "q" );
       if (0 < u4Pos)          // Quit Debug ?
       {
       #ifdef __KERNEL__
            Dbg_Util_Quit();
       #endif
           Dbg_ResetSrc();
           Dbg_SetLogOnOff(FALSE);

           return 0;
       }
       else
       {    
            u4Pos = _SwdmxCmdFindSwitch(i4Argc, szArgv, "l");
            if (0 < u4Pos) 
            {
                UINT32 u4Cnt = 0;
                Printf("SWDMX Data Dump Log:    %s\n", Dbg_GetLogOnOff()?"on":"off");
                Printf("0:off    1:usb on    2:semihost on\n");
                for (u4Cnt=0; SWDMX_DBG_SWITCH_NUM>u4Cnt; u4Cnt++)
                {
                    Printf("%u %-35s:%u\n", u4Cnt, Dbg_GetInfo(u4Cnt), Dbg_GetDbgSrc(u4Cnt));
                }
                return 0;
            }
       }

        // other 2 para Cmd
       {
           u4SrcId = (UINT32)StrToInt( szArgv[1] );
           if ( SWDMX_SOURCE_MAX > u4SrcId )    // Print on src info
           {
               SWDMX_SetInfo(u4SrcId, eSWDMX_SET_DBGINFO, (UINT32)&prDbgInfo, 0, 0);
               _SwdmxCmdPutDbgConfig( u4SrcId, prDbgInfo );
           }
           else     // print all instance state
           {
                for ( u4SrcId=0; SWDMX_SOURCE_MAX>u4SrcId; u4SrcId++ )
                {
                    SWDMX_SetInfo(u4SrcId, eSWDMX_SET_DBGINFO, (UINT32)&prDbgInfo, 0, 0);
                    _SwdmxCmdPutDbgConfig( u4SrcId, prDbgInfo );
                }
           }
       }
   }
   else
   {
        u4Pos = _SwdmxCmdFindSwitch( i4Argc, szArgv, "w" );
        if (0 < u4Pos) 
        {
            UINT32 u4Src = (UINT32)StrToInt( szArgv[u4Pos+1] );
            UINT32 u4Value = (UINT32)StrToInt( szArgv[u4Pos+2] );
            u4Value = (DUMP_DATA_USB_ON==u4Value) ? DUMP_DATA_USB_ON :
                          ((DUMP_DATA_SEMI_ON==u4Value)? DUMP_DATA_SEMI_ON : DUMP_DATA_OFF);

            if  (SWDMX_DBG_SWITCH_NUM > u4SrcId)
            {
                Dbg_SetDbgSrc(u4Src, u4Value);
            }
            
            return 0;
        }

        u4Pos = _SwdmxCmdFindSwitch( i4Argc, szArgv, "a" );
        if (0 < u4Pos) 
        {
            UINT32 u4Value = (UINT32)StrToInt( szArgv[u4Pos+1] );

            Dbg_SetLogOnOff(0==u4Value ? FALSE : TRUE);
            
            return 0;
        }
        
        // Set Debug state
        u4SrcId = (UINT32)StrToInt( szArgv[1] );
               
        if ( SWDMX_SOURCE_MAX > u4SrcId )
        {    
            UINT32 u4Pos = 0;                 // pos of the switch(s, f, m, f1, m1)
            UINT32 u4Mode = 0;               // LOGCRC/SAVECRC/AUTODETEC/SAVEDATA
            UINT32 u4PerMode = 0;          // ... ...
            UINT32 u4FileMode = 0;          // Append or TRUNC
            UINT32 u4Mask = 0;                // USE for set or get Mode state
            
            UINT32 u4PosFileName = 0;   
            UINT32 u4PosFileName1 = 0;
            BOOL   fgFileOpenMode = FALSE;
            BOOL   fgFileOpenMode1 = FALSE;
            BOOL   fgOpenMode = FALSE;          // Set in cmd?
            BOOL   fgOpenMode1 = FALSE;         // Set in cmd?
            
            CHAR szFileName[ 256 ] = { 0 };
            CHAR szFileName1[ 256 ] = { 0 };
            CHAR szDefNameLast[ 16 ] = { 0 };       // Default File Last name(srcId_lastName)
            CHAR szDefNameLast1[ 16 ] = { 0 };

            HANDLE_T *phHandle = NULL;              // Handle to deal with
            HANDLE_T *phHandle1 = NULL;             // Handle to deal with

            BOOL fgOldUse = FALSE;                      // Per state 
            BOOL fgNewUse = FALSE;                     // mow state
            
            SWDMX_SetInfo(u4SrcId, eSWDMX_SET_DBGINFO, (UINT32)&prDbgInfo, 0, 0);
            u4Pos = _SwdmxCmdFindSwitch( i4Argc, szArgv, "s" );
            if ( 1 < u4Pos )
            {
                if ( u4Pos + 1 < i4Argc )
                {
                    u4Mode = szArgv[u4Pos + 1][0] < '0' ? 
                                  4 : szArgv[u4Pos + 1][0] - '0';
                             
                    u4FileMode = szArgv[u4Pos + 1][1] < '0' ?
                                      4 : szArgv[u4Pos + 1][1] - '0';

                    #ifdef __KERNEL__
                    if (MM_DBG_MODE_SAVEDATA == u4Mode)
                    {
                        Printf("In linux, SaveData not allowed, cmd ignored.\n");
                        return -1;
                    }
                    #endif
                    
                    if ( u4Mode < MM_DBG_MODE_MAX && u4FileMode < 4 )
                    {
                        if ( MM_DBG_MODE_LOGCRC == u4Mode )
                        {      
                            prDbgInfo->u4Mode = ( u4FileMode | ( prDbgInfo->u4Mode & 0XFFFFFFC) );
                            return 0;
                        }
                        else if ( MM_DBG_MODE_SAVEBLOCK == u4Mode )
                        {
                            // Note that source 1 and source 2 are the same source in SAVEBLOCK mode
                            u4Mask = 3 << (u4Mode * 2);
                            prDbgInfo->u4Mode = ( ((0 == u4FileMode) ? 0 : u4Mask) | ( prDbgInfo->u4Mode & ~u4Mask) );
                            return 0;
                        }
                        else
                        {
                            u4Mask = 3 << (u4Mode * 2);
                            u4PerMode = prDbgInfo->u4Mode;
                            prDbgInfo->u4Mode = (~u4Mask & u4PerMode) | (u4FileMode << (u4Mode *2));
                            
                            DBG_GET_FILENAMEPOS(u4PosFileName, "f");
                            DBG_GET_FILENAMEPOS(u4PosFileName1, "f1");

                            DBG_GET_FILEOPENMODE(fgFileOpenMode, "m", fgOpenMode);
                            DBG_GET_FILEOPENMODE(fgFileOpenMode1, "m1", fgOpenMode1);

                            if (1 == u4Mode)
                            {
                                x_strcpy( szDefNameLast, "savecrc.txt" );
                                x_strcpy( szDefNameLast1, "savecrc1.txt" );

                                phHandle = &prDbgInfo->hFileSaveCrc;
                                phHandle1 = &prDbgInfo->hFileSaveCrc1; 
                            }
                            else if (2 == u4Mode)
                            {
                                x_strcpy( szDefNameLast, "loadcrc.txt" );
                                x_strcpy( szDefNameLast1, "loadcrc1.txt" );

                                // for read, can not set to trunc
                                if ( (TRUE == fgOpenMode || TRUE == fgOpenMode1) &&
                                     (FALSE == fgFileOpenMode || FALSE == fgFileOpenMode1 ))
                                {
                                    Printf("Autocheck mode can not set OpenMode for Trunc. parameter ignored!\n");
                                }
                                
                                fgFileOpenMode = TRUE;
                                fgFileOpenMode1 = TRUE;

                                phHandle = &prDbgInfo->hFileGetCrc;
                                phHandle1 = &prDbgInfo->hFileGetCrc1; 
                            }
                            else if (3 == u4Mode)
                            {
                                x_strcpy( szDefNameLast, "savedata.dat" );
                                x_strcpy( szDefNameLast1, "savedata1.dat" );

                                phHandle = &prDbgInfo->hFileSaveData;
                                phHandle1 = &prDbgInfo->hFileSaveData1; 
                            }
                            else
                            {
                                // not possible
                            }
                            
                            if ( 0 == u4PosFileName )
                            {
                                DBG_GET_DEFAULT_FILENAME( szFileName, u4SrcId, szDefNameLast);
                            }
                            else
                            {
                                DBG_GET_USER_FILENAME( szFileName, u4PosFileName );
                            }

                            if ( 0 == u4PosFileName1 )
                            {
                                DBG_GET_DEFAULT_FILENAME( szFileName1, u4SrcId, szDefNameLast1 );
                            }
                            else
                            {
                                DBG_GET_USER_FILENAME( szFileName1, u4PosFileName1 );
                            }

                            if (  (u4PerMode & u4Mask) !=  ( (UINT32)(prDbgInfo->u4Mode) & u4Mask)  )
                            {
                                fgOldUse = (0 != (((u4PerMode & u4Mask) >> (u4Mode * 2)) & 0X1)
                                           ? TRUE : FALSE);
                                fgNewUse = (0 != ((((UINT32)(prDbgInfo->u4Mode) & u4Mask) >> (u4Mode * 2)) & 0X1)
                                           ? TRUE : FALSE);
                                                
                                if ( FALSE==fgOldUse && TRUE==fgNewUse ) 
                                {
                                    Dbg_OpenUsbFile(szFileName, phHandle, fgFileOpenMode);
                                } 
                                else if( TRUE==fgOldUse && FALSE==fgNewUse )
                                {
                                    Dbg_CloseUsbFile( *phHandle, phHandle );
                                    *phHandle = 0; 
                                }
                                else if ( 0!=u4PosFileName || TRUE == fgOpenMode )
                                {
                                    Printf( "Source %s, Can not use switch %s%s now. ignored!\n",
                                             TRUE == fgNewUse ? "busy" : "not open",
                                             0 != u4PosFileName ? "\"-f\" " : "",
                                             TRUE == fgOpenMode ? "\"-m\"" : "" );
                                }
                                else
                                {
                                    // nothing to do
                                }

                                fgOldUse = (0 != (((u4PerMode & u4Mask) >> (u4Mode * 2)) & 0X2)
                                                ? TRUE : FALSE);
                                fgNewUse = (0 != ((((UINT32)(prDbgInfo->u4Mode) & u4Mask) >> (u4Mode * 2)) & 0X2)
                                                ? TRUE : FALSE);

                                if ( FALSE==fgOldUse && TRUE==fgNewUse ) 
                                {
                                    Dbg_OpenUsbFile(szFileName1, phHandle1, fgFileOpenMode1);
                                } 
                                else if( TRUE==fgOldUse && FALSE==fgNewUse )
                                {
                                    Dbg_CloseUsbFile( *phHandle1, phHandle1 );
                                    *phHandle1 = 0; 
                                }
                                else if ( 0!=u4PosFileName1 || TRUE == fgOpenMode1)
                                {
                                    Printf( "Source %s, Can not use switch %s%s now. ignored!\n",
                                             TRUE == fgNewUse ? "busy" : "not open",
                                             0 != u4PosFileName1 ? "\"-f1\" " : "",
                                             TRUE == fgOpenMode1 ? "\"-m1\"" : "" );
                                }
                                else
                                {
                                    // nothing to do
                                }
                            }
                            else
                            {
                                Printf("Mode unchanged. Command ignored!\n");
                            }
                        }
                    }
                    else
                    {
                        Printf("Command of \"-s\" is wrong.(Range [0...3][0...3])\n");
                    }
                }
                else
                {
                    Printf("Command wrong. Ignored!\n");
                }
            }
            else
            {
                UINT32 u4Pos = _SwdmxCmdFindSwitch( i4Argc, szArgv, "q" );

                if (1 < u4Pos)
                {
                #ifdef __KERNEL__
                    Printf("In Kernel Mode, Can not Quit Only one Src. (Use \"swdmx.d -q\")\n");
                #else
                    Printf("In necleus, Can not use quit cmd.\n");
                    #if 0
                    UNUSED( Dbg_CloseUsbFile( prDbgInfo->hFileGetCrc, &prDbgInfo->hFileGetCrc) );
                    UNUSED( Dbg_CloseUsbFile( prDbgInfo->hFileGetCrc1, &prDbgInfo->hFileGetCrc1) );
                    UNUSED( Dbg_CloseUsbFile( prDbgInfo->hFileSaveCrc, &prDbgInfo->hFileSaveCrc )  );
                    UNUSED( Dbg_CloseUsbFile( prDbgInfo->hFileSaveCrc1, &prDbgInfo->hFileSaveCrc1 ) );
                    UNUSED( Dbg_CloseUsbFile( prDbgInfo->hFileSaveData, &prDbgInfo->hFileSaveData ) );
                    UNUSED( Dbg_CloseUsbFile( prDbgInfo->hFileSaveData1, &prDbgInfo->hFileSaveData1 ) );
                    x_memset( (VOID *)&prDbgInfo, 0, sizeof( MM_DBG_INFO_T ) );
                    #endif
                 #endif
                }
            }
       }
       else
       {
            Printf("Instance %d is not exists.\n", u4SrcId );
       }
   }
   
   return 0;
}
#endif // SWDMX_DBG_USB

static INT32 _SwdmxCmdInit(INT32 i4Argc, const CHAR ** szArgv)
{
    if (szArgv == NULL)
    {
        return -1;
    }

    UNUSED(i4Argc);
    UNUSED(szArgv);

#ifdef SWDMX_SIMULATION_CODE
    _pcFileInfo = BSP_AllocDmaMemory(INDEX_FILE_SIZE);
    //Printf("d.load.binary c:\\aviidx.txt 0x%08x /ny\n", _pcFileInfo);
    //T32_HostExec(E_T32_EXEC, "d.load.binary c:\\aviidx.txt 0x%08x /ny",
    //				_pcFileInfo);
#endif

    SWDMX_Init();

    return 0;
}


static INT32 _SwdmxCmdOpen(INT32 i4Argc, const CHAR ** szArgv)
{
    if (szArgv == NULL)
    {
        return -1;
    }

    UNUSED(i4Argc);
    UNUSED(szArgv);

    //SWDMX_Open(SWDMX_FMT_MPEG2_TS, 2*1024*1024);

    return 0;
}

static INT32 _SwdmxCmdPlay(INT32 i4Argc, const CHAR ** szArgv)
{
    if (szArgv == NULL)
    {
        return -1;
    }

    UNUSED(i4Argc);
    UNUSED(szArgv);
    
#ifdef CC_53XX_SWDMX_V2
    SWDMX_Play(SWDMX_CMD_SWDMX_0);
#else
    SWDMX_Play();
#endif

    return 0;
}


static INT32 _SwdmxCmdStop(INT32 i4Argc, const CHAR ** szArgv)
{
    if (szArgv == NULL)
    {
        return -1;
    }

    UNUSED(i4Argc);
    UNUSED(szArgv);

#ifdef SWDMX_SIMULATION_CODE
    BSP_FreeDmaMemory((UINT32)_pcFileInfo);
    BSP_FreeDmaMemory((UINT32)_rBuldElmIdxTbl.u.pt_time2byte_tbl);
#endif

#ifdef CC_53XX_SWDMX_V2
    SWDMX_Stop(SWDMX_CMD_SWDMX_0);
#else
    SWDMX_Stop();
#endif

    return 0;
}

static INT32 _SwdmxCmdQuery(INT32 i4Argc, const CHAR ** szArgv)
{
    BOOL fgRet;
    UINT8 u1SrcId;
    UINT32 u4Tmp;
    ENUM_SWDMX_FMT_T eFmt;
    SWDMX_INFO_T *prSwdmxInfo;
    if (szArgv == NULL)
    {
        return -1;
    }

    UNUSED(i4Argc);
    UNUSED(szArgv);

    u1SrcId = 0;
#ifdef CC_53XX_SWDMX_V2
    for(;u1SrcId<SWDMX_SOURCE_MAX;u1SrcId++)
#endif
    {
#ifdef CC_53XX_SWDMX_V2
        fgRet = SWDMX_GetInfo(u1SrcId, (UINT8)eSWDMX_CONTAINER_TYPE, &u4Tmp, 0, 0);
#else
        fgRet = SWDMX_GetInfo((UINT8)eSWDMX_CONTAINER_TYPE, &u4Tmp, 0, 0);
#endif
        if(!fgRet)
        {
            return 0;
        }
        eFmt = (ENUM_SWDMX_FMT_T)u4Tmp;

        LOG(0, "Container: ");
        switch(eFmt)
        {
            case SWDMX_FMT_AVI:
                LOG(0, "AVI\n");
                break;
            case SWDMX_FMT_MPEG1_DAT:
                LOG(0, "DAT\n");
                break;
            case SWDMX_FMT_MPEG2_PS:
                LOG(0, "PS\n");
                break;
            case SWDMX_FMT_MPEG2_TS:
                LOG(0, "TS\n");
                break;
            case SWDMX_FMT_MPEG2_TS_192:
            case SWDMX_FMT_MPEG2_TS_ZERO_192:
                LOG(0, "TS 192\n");
                break;
            case SWDMX_FMT_MPEG2_TS_TIME_SHIFT:
                LOG(0, "TS TIME SHIFT\n");
                break;
            case SWDMX_FMT_VC1_ES:
                LOG(0, "VC1 ES\n");
                break;
            case SWDMX_FMT_MPEG2_VIDEO_ES:
                LOG(0, "MPEG12 ES\n");
                break;
            case SWDMX_FMT_MPEG4_VIDEO_ES:
                LOG(0, "MPEG4 ES\n");
                break;
            case SWDMX_FMT_H264_VIDEO_ES:
                LOG(0, "H264 ES\n");
                break;
            case SWDMX_FMT_RAW_AUDIO_ES:
                LOG(0, "RAW AUDIO ES\n");
                break;
            case SWDMX_FMT_MP3_AUDIO_ES:
                LOG(0, "MP3 AUDIO ES\n");
                break;
            case SWDMX_FMT_VC1_WMV:
                LOG(0, "VC1 WMV\n");
                break;
                /*
            case SWDMX_FMT_WMA_AUDIO_ES:
                LOG(0, "WMA AUDIO ES\n");
                break;
                */
            case SWDMX_FMT_MP4:
                LOG(0, "MP4\n");
                break;
            case SWDMX_FMT_MKV:
                LOG(0, "MKV\n");
                break;
                /*
            case SWDMX_FMT_RM:
                LOG(0, "RM\n");
                break;
                */
            default:
                LOG(0, "UnKnown(%d)\n", u4Tmp);
                break;
        }

#ifdef CC_53XX_SWDMX_V2
        u4Tmp = (UINT32)SWDMX_QueryStatus(u1SrcId);
#else
        u4Tmp = (UINT32)SWDMX_QueryStatus();
#endif
        LOG(0, "Swdmx: ");
        switch(u4Tmp)
        {
            case eSWDMX_STATE_STOP:
                LOG(0, "Stop ");
                break;
            case eSWDMX_STATE_PLAY:
                LOG(0, "Play ");
                break;
            case eSWDMX_STATE_PAUSE:
                LOG(0, "Pause ");
                break;
            default:
                LOG(0, "UnKnown ");
                break;
        }

        if(u1SrcId < VDEC_MAX_ES)
        {
            // We should have ES id
            VDEC_HDR_INFO_T rHdrInfo;
            u4Tmp = (UINT32)VDEC_QueryInfo(u1SrcId, &rHdrInfo);
            LOG(0, "Vdec: ");
            switch(rHdrInfo.u1CurState)
            {
                case VDEC_ST_STOP:
                    LOG(0, "Stop\n");
                    break;
                case VDEC_ST_PLAY:
                case VDEC_ST_PLAY_I_FRAME:
                case VDEC_ST_HEADER_PARSE:
                case VDEC_ST_DEC_TRIGGER:
                case VDEC_ST_WAIT_DEC_FINISH:
                case VDEC_ST_DROP:
                    LOG(0, "Play\n");
                    break;
                case VDEC_ST_IDLE:
                    LOG(0, "Pause\n");
                    break;
                default:
                    LOG(0, "UnKnown\n");
                    break;
            }
        }

        prSwdmxInfo = NULL;
#ifdef CC_53XX_SWDMX_V2
        fgRet = SWDMX_GetInfo(u1SrcId, (UINT8)eSWDMX_GET_INTERNAL_INFO, (UINT32*)&prSwdmxInfo, 0, 0);
#else
        fgRet = SWDMX_GetInfo((UINT8)eSWDMX_GET_INTERNAL_INFO, (UINT32*)&prSwdmxInfo, 0, 0);
#endif
        if(fgRet && prSwdmxInfo)
        {
            LOG(0, "Duration: %d (90K), Size: %lld\n",
                prSwdmxInfo->u4TotalTime,
                prSwdmxInfo->u8FileSize);
            LOG(0, "(En, Id) V(%d, %d) A(%d, %d, %d) S(%d, %d) PCR(%d, %d)\n",
                prSwdmxInfo->fgEnVideoDmx,
                prSwdmxInfo->u4VidStrmID,
                prSwdmxInfo->fgEnAudioDmx,
                prSwdmxInfo->u4AudStrmID,
                prSwdmxInfo->u4AudSubStrmID,
                prSwdmxInfo->fgEnSubTitleDmx,
                prSwdmxInfo->u4SubTitleStrmID,
                prSwdmxInfo->fgEnPcrDmx,
                prSwdmxInfo->u4PcrStrmID);
        }

#ifdef CC_53XX_SWDMX_V2
        fgRet = SWDMX_SetInfo(u1SrcId, (UINT8)eSWDMX_PRINT_INTERNAL_STATUS, 0, 0, 0);
#else
        fgRet = SWDMX_SetInfo((UINT8)eSWDMX_PRINT_INTERNAL_STATUS, 0, 0, 0);
#endif
        UNUSED(fgRet);

        if((eFmt >= SWDMX_FMT_MPEG2_TS) &&
           (eFmt <= SWDMX_FMT_MPEG2_TS_TIME_SHIFT))
        {
#ifdef CC_53XX_SWDMX_V2
            fgRet = SWDMX_GetInfo(u1SrcId, (UINT8)eSWDMX_SET_CHK_PSI_BUF_SIZE, &u4Tmp, 0, 0);
#else
            fgRet = SWDMX_GetInfo((UINT8)eSWDMX_SET_CHK_PSI_BUF_SIZE, &u4Tmp, 0, 0);
#endif
            if(fgRet)
            {
                LOG(0, "Block PSI(%d)\n", u4Tmp);
            }
#ifdef TIME_SHIFT_SUPPORT
            if(eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
            {
#ifdef CC_53XX_SWDMX_V2
                fgRet = SWDMX_GetInfo(u1SrcId, (UINT8)eSWDMX_GET_CURRENT_TICK, &u4Tmp, 0, 0);
#else
                fgRet = SWDMX_GetInfo((UINT8)eSWDMX_GET_CURRENT_TICK, &u4Tmp, 0, 0);
#endif
                if(fgRet)
                {
                    LOG(0, "Current Tick(%d)\n", u4Tmp);
                }
#ifdef CC_53XX_SWDMX_V2
                fgRet = SWDMX_SetInfo(u1SrcId, (UINT8)eSWDMX_PRINT_REC_BUF_STATUS, 0, 0, 0);
#else
                fgRet = SWDMX_SetInfo((UINT8)eSWDMX_PRINT_REC_BUF_STATUS, 0, 0, 0);
#endif
                if(fgRet)
                {
                    UNUSED(fgRet);
                }
            }
#endif
        }
    }

    return 0;
}

static INT32 _SwdmxCmdSetSpeed(INT32 i4Argc, const CHAR ** szArgv)
{
    #if 0
    INT32 i4Speed;
    if (szArgv == NULL || i4Argc < 2)
    {
        Printf("Arg: i4Speed (1x:1000, -1x:-1000)");
        return 0;
    }

    UNUSED(i4Argc);
    UNUSED(szArgv);

    i4Speed = (INT32)StrToInt(szArgv[1]);
    if(i4Speed == 1)
    {
        i4Speed = -(INT32)StrToInt(szArgv[2]);
    }
    else
    {
        i4Speed = (INT32)StrToInt(szArgv[2]);
    }

    SWDMX_Set_Speed(i4Speed);

    STC_SetSyncMode(AV_SYNC_MODE_VIDEO_MASTER);
    #endif

    return 0;
}

static INT32 _SwdmxCmdSetPosition(INT32 i4Argc, const CHAR ** szArgv)
{
    #if 0
    UINT32 u4Time;
    UINT64 u8Pos;
    if (szArgv == NULL)
    {
        Printf("Arg: u4Pos (0~1000 /1000)");
        return -1;
    }

    UNUSED(u4Time);
    UNUSED(szArgv);

    u4Time = (INT32)StrToInt(szArgv[1]);

    SWDMX_SeekTime(u4Time, &u8Pos);
    #endif

    return 0;
}

static INT32 _SwdmxCmdTest(INT32 i4Argc, const CHAR ** szArgv)
{

	if (szArgv == NULL)
	{
	    return -1;
	}

	UNUSED(i4Argc);
	UNUSED(szArgv);

#ifdef SWDMX_SIMULATION_CODE
	char		*pcToken;
	INT32 	i, i4FrameCount = 0;

	pcToken = strtok(_pcFileInfo, ",");
	if(pcToken != NULL)
	{
		i4FrameCount = StrToInt(pcToken);
		LOG(0, "Total Frame Count = %ld\n", i4FrameCount);
	}

	// Test Codes for avi_mp1_mp3.avi
	_rDmxRange.ui4_fileoffset = 0;
	_rDmxRange.z_range_sz = 4447162 ;
	_rDmxRange.ui4_vid_start_pts = 0;
	_rDmxRange.ui4_vid_render_counts = i4FrameCount;
	_rDmxRange.pv_idxtbl = (VOID*)&_rBuldIdxTbl;

	_rBuldIdxTbl.ui4_elm_tbl_num = 1;
	_rBuldIdxTbl.pt_tbl_list = &_rBuldElmIdxTbl;

	_rBuldElmIdxTbl.t_header.e_tbl_type = MIDXBULD_TBL_TYPE_VIDEO;
	_rBuldElmIdxTbl.t_header.ui4_stm_id = 0x30306463;
	_rBuldElmIdxTbl.ui4_nums_entry = i4FrameCount;
	_rBuldElmIdxTbl.ui4_start_frame_num = 0;
	_rBuldElmIdxTbl.ui4_end_frame_num = i4FrameCount;
	_rBuldElmIdxTbl.u.pt_time2byte_tbl = BSP_AllocDmaMemory(_rBuldElmIdxTbl.ui4_nums_entry * sizeof(MIDXBULD_TIME2BYTE_ENTRY_T));

	for (i = 0; i < _rBuldElmIdxTbl.ui4_nums_entry; i++)
	{
		pcToken = strtok(NULL, ",");
		_rBuldElmIdxTbl.u.pt_time2byte_tbl[i].ui8_offset = StrToInt(pcToken);
		pcToken = strtok(NULL, ",");
		_rBuldElmIdxTbl.u.pt_time2byte_tbl[i].ui4_size = StrToInt(pcToken);
		pcToken = strtok(NULL, ",");
		_rBuldElmIdxTbl.u.pt_time2byte_tbl[i].ui4_pts = StrToInt(pcToken);
	}

	_SWDMX_Add_or_Del_Range(&_rDmxRange, SWDMX_ADD_RANGE, 0);

#endif

	return 0;
}


static INT32 _SwdmxCmdTestFeeder(INT32 i4Argc, const CHAR ** szArgv)
{
    if (szArgv == NULL)
    {
        return -1;
    }

    UNUSED(i4Argc);
    UNUSED(szArgv);

#ifdef CC_53XX_SWDMX_V2
    UNUSED(SWDMX_SetInfo(SWDMX_CMD_SWDMX_0, eSWDMX_SET_FEEDER_TEST_FUNCTION, 0, 0, 0));
#else
    UNUSED(SWDMX_SetInfo(eSWDMX_SET_FEEDER_TEST_FUNCTION, 0, 0, 0));
#endif
	return 0;
}

static INT32 _SwdmxCmdTestMPG(INT32 i4Argc, const CHAR ** szArgv)
{
	return 0;
}

#ifdef SWDMX_STATISTICS
static INT32 _SwdmxResetTime (INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

#ifdef CC_53XX_SWDMX_V2
    UNUSED(_SWDMX_ResetTime(SWDMX_CMD_SWDMX_0));
#else
    UNUSED(_SWDMX_ResetTime());
#endif
    return 0;
}


static INT32 _SwdmxPrintTime (INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

#ifdef CC_53XX_SWDMX_V2
    UNUSED(_SWDMX_PrintTime(SWDMX_CMD_SWDMX_0));
#else
    UNUSED(_SWDMX_PrintTime());
#endif
    return 0;
}
#endif


#ifdef CC_SWDMX_RENDER_PTS
static INT32 _SwdmxSetRenderPts (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Time;

    if (i4Argc != 2)
    {
        Printf("Arg: render pts (measured in 90k)");
        return -1;
    }

    u4Time = (INT32)StrToInt(szArgv[1]);
#ifdef CC_53XX_SWDMX_V2
    UNUSED(SWDMX_SetRenderPts(SWDMX_CMD_SWDMX_0, u4Time));
#else
    UNUSED(SWDMX_SetRenderPts(u4Time));
#endif

    return 0;
}
#endif


#ifdef CC_SWDMX_RENDER_POSITION_TEST
#include "x_mm_common.h"
static INT32 _SwdmxGetRenderPosition (INT32 i4Argc, const CHAR ** szArgv)
{
    START_POS_INFO_T rPosInfo;

    SWDMX_GetRenderPosition((VOID*)&rPosInfo);

    Printf("Position:\n");
    Printf("ui8_aud_pts_info = %d\n", (UINT32)rPosInfo.ui8_aud_pts_info);
    Printf("ui8_aud_frame_position = %d\n", (UINT32)rPosInfo.ui8_aud_frame_position);
    Printf("ui8_i_pts_info = %d\n", (UINT32)rPosInfo.ui8_i_pts_info);
    Printf("ui8_pts_info = %d\n", (UINT32)rPosInfo.ui8_pts_info);
    Printf("ui8_i_frame_position = %d\n", (UINT32)rPosInfo.ui8_i_frame_position);
    Printf("ui8_frame_position = %d\n", (UINT32)rPosInfo.ui8_frame_position);
    Printf("ui8_frame_position_disp = %d\n", (UINT32)rPosInfo.ui8_frame_position_disp);
    Printf("i4_temporal_reference = %d\n", (UINT32)rPosInfo.i4_temporal_reference);
    Printf("ui2_decoding_order = %d\n", (UINT32)rPosInfo.ui2_decoding_order);

    return 0;
}
static INT32 _SwdmxSetRenderPosition (INT32 i4Argc, const CHAR ** szArgv)
{
    START_POS_INFO_T rPosInfo;

    if (i4Argc != 10)
    {
        Printf("Arg: by START_POS_INFO_T sequence\n");
        return -1;
    }
    
    rPosInfo.ui8_aud_pts_info = (UINT64)StrToInt(szArgv[1]);
    rPosInfo.ui8_aud_frame_position = (UINT64)StrToInt(szArgv[2]);
    rPosInfo.ui8_i_pts_info = (UINT64)StrToInt(szArgv[3]);
    rPosInfo.ui8_pts_info = (UINT64)StrToInt(szArgv[4]);
    rPosInfo.ui8_i_frame_position = (UINT64)StrToInt(szArgv[5]);
    rPosInfo.ui8_frame_position = (UINT64)StrToInt(szArgv[6]);
    rPosInfo.ui8_frame_position_disp = (UINT64)StrToInt(szArgv[7]);
    rPosInfo.i4_temporal_reference = (INT32)StrToInt(szArgv[8]);
    rPosInfo.ui2_decoding_order = (UINT16)StrToInt(szArgv[9]);

    SWDMX_SetRenderPosition((VOID*)&rPosInfo);
    return 0;
}
#endif


//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

LINT_SAVE_AND_DISABLE
CLI_MAIN_COMMAND_ITEM(Swdmx)
{
    "swdmx",
    NULL,
    NULL,
    _arSwdmxCmdTbl,
    "SWDMX command",
    CLI_SUPERVISOR
};
LINT_RESTORE

/*lint -restore */



