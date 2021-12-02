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

//-----------------------------------------------------------------------------
// Include headers
//-----------------------------------------------------------------------------
#include "x_typedef.h"
#include "x_assert.h"
#include "x_debug.h"
#include "swdmx_debug_util.h"

#if defined(SWDMX_DBG_USB)
#ifndef __KERNEL__
#include "x_fm.h"
#else
PFN_PUT_ENVENT _gfnPutEvent = NULL;

BOOL Dbg_Register_DBG_CB(UINT8 u1SrcId, VOID *pvFunc)
{
        _gfnPutEvent = (PFN_PUT_ENVENT)pvFunc;
        
        UNUSED(u1SrcId);
        return TRUE;
}
#endif //  __KERNEL__
#endif //   defined(SWDMX_DBG_USB)

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
#ifdef SWDMX_DBG_SEMIHOST
/*
function:   _t32_HostBreakPT( used for SWI, this flag should be set in CVD or T32 .cmm files )
parameter:
        i4TargetFlag:       be stored in R0, used for command flag
        szHostCommand:  he command that T32 or CVD will execute
return value:   ignored
*/
static INT32 _t32_HostBreakPT(INT32 i4TargetFlag,
                                                      CHAR *szHostCommand)
{
    UNUSED(szHostCommand);
    return i4TargetFlag;
}

/*
function:         _t32_HostBreakPT2( used for SWI, this flag should be set in CVD or T32 .cmm files )
parameter:      not used
return value:   Ignored
*/
static INT32 _t32_HostBreakPT2(INT32 i4TargetFlag,
                                                        CHAR *szHostCommand,
                                                        UINT32 u4Address,
                                                        UINT32 u4Size)
{
    UNUSED(szHostCommand);
    UNUSED(u4Address);
    UNUSED(u4Size);
    
    return i4TargetFlag;
}
#endif // DBG_SWDMX_SEMIHOST

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

// The usb related public functions
#ifdef SWDMX_DBG_USB
BOOL Dbg_Util_Quit(VOID)
{
#ifndef __KERNEL__
#else
     SWDMX_DBG_T rCmd = {0};

    rCmd.eCmd = SWDMX_DBG_DO_DEBUG_EXIT;
    
    if (NULL != _gfnPutEvent)
    {
        _gfnPutEvent( &rCmd );
    }
#endif
    return TRUE;
}

/*
function:       T32_LoadUsbFile(Get data from the files in the target)	
parameter:
        pcFileName:     Filename
        u4Addr:          Address of buffer where to stroe the datas
        u4Size:           The buffer size
return value:           Byte of data the function got
*/
static UINT32 _gu4Switch[SWDMX_DBG_SWITCH_NUM] = {0};
static BOOL _gfgLogOn = FALSE;
static CHAR _gacInfo[DUMP_DATA_SIZE_MAX][40] = 
{
    "SWDMX Get data from FEEDER",
    "SWDMX Send Video Data to DMX",
    "SWDMX Send Audio Data to DMX",
    "SWDMX Send VES to VDEC",
    "SWDMX Send AES to AUD",
    "VDEC decode a frame",
    "FBM set frame to display state",
    "USER define feature"
};

UINT32 Dbg_GetDbgSrc(UINT32 u4SrcId)
{
    if (SWDMX_DBG_SWITCH_NUM < u4SrcId)
    {
        return 0;
    }
    else
    {
        return _gu4Switch[u4SrcId];
    }
}

VOID Dbg_SetDbgSrc(UINT32 u4SrcId, UINT32 u4Value)
{
    if (SWDMX_DBG_SWITCH_NUM > u4SrcId)
    {
        _gu4Switch[u4SrcId] = u4Value;
    }
}

VOID Dbg_ResetSrc(VOID)
{
    int u4Cnt = 0;

    for (u4Cnt=0; SWDMX_DBG_SWITCH_NUM>u4Cnt; u4Cnt++)
    {
        _gu4Switch[u4Cnt] = 0;
    }

    return ;
}

BOOL Dbg_GetLogOnOff(VOID)
{
    return _gfgLogOn;
}

VOID Dbg_SetLogOnOff(BOOL fgOn)
{
    _gfgLogOn = fgOn;
    
    return ;
}

CHAR *Dbg_GetInfo(UINT32 u4Src)
{
    if (SWDMX_DBG_SWITCH_NUM > u4Src)
    {
        return _gacInfo[u4Src] ;
    }

    return NULL;
}

UINT32 Dbg_LoadUsbFile(CHAR *pcFileName, UINT32 u4Addr, UINT32 u4Size )
{
#ifndef __KERNEL__
    UINT64 u8FileSize, u8Tmp;
    UINT32 u4Request, u4Read;
    HANDLE_T  h_file;

    VERIFY(FMR_OK == x_fm_open(FM_ROOT_HANDLE, pcFileName,
                                                  FM_READ_ONLY, 0777, FALSE, &h_file));
    VERIFY(FMR_OK == x_fm_lseek(h_file, 0, FM_SEEK_END, &u8FileSize));
    VERIFY(FMR_OK == x_fm_lseek(h_file, 0, FM_SEEK_BGN, &u8Tmp));

    // the u4Size may large than the file's lenght, so we need to decide the read size
    u4Request = (UINT32)u8FileSize;
    u4Request = u4Size>u4Request ? u4Request : u4Size;

    x_fm_read(h_file, (VOID *)u4Addr, u4Request, &u4Read);

    ASSERT(u4Request == u4Read);

    VERIFY(FMR_OK == x_fm_close(h_file));

    return u4Read;          // return the size that the function get
#else
    SWDMX_DBG_T rCmd = {0};

    rCmd.eCmd = SWDMX_DBG_DO_LOAD_DATA;
    rCmd.u.rBufInfo.u4Src = 0;
    rCmd.u.rBufInfo.pvBufStart = (VOID *)u4Addr;
    rCmd.u.rBufInfo.u4BlockSize = u4Size;
    x_memcpy(rCmd.u.rBufInfo.au1FilePath, pcFileName, sizeof(rCmd.u.rBufInfo.au1FilePath));

    if (NULL != _gfnPutEvent)
    {
        _gfnPutEvent( &rCmd );
    }
    return 0;
#endif
}

/*
function:   Dbg_OpenUsbFile

parameter:  @pcFileName: 
                 @fgAppend:    open mode(TRUE:append  FALSE:trunc)

return value:   TRUE:success FALSE:failed
*/
BOOL  Dbg_OpenUsbFile( CHAR *pcFileName, UINT32 *phFile, BOOL fgAppend )
{
#ifndef __KERNEL__
    UINT32 u4OpenMode = FM_READ_WRITE | FM_OPEN_CREATE;
    HANDLE_T h_file = NULL;
    
    POINTER_NULL( pcFileName, FALSE );
    POINTER_NULL( phFile, FALSE );

    *phFile = NULL;
    
    if ( TRUE == fgAppend )
    {
        u4OpenMode |= FM_OPEN_APPEND;
    }
    else
    {
        u4OpenMode |= FM_OPEN_TRUNC;
    }

    if (FMR_OK != x_fm_open(FM_ROOT_HANDLE,
                                          pcFileName,
                                          u4OpenMode,
                                          0777, FALSE, &h_file) )
    {
        if (DUMP_LOG)
        {
            LOG(1, "Open file fail.\n");
        }
        return FALSE;
    }
    
    *phFile = h_file;
#else
    SWDMX_DBG_T rCmd = {0};

    rCmd.eCmd = SWDMX_DBG_DO_OPEN_FILE;
    rCmd.u.rFileInfo.u4Src = 0;
    rCmd.u.rFileInfo.u4File = 0;
    rCmd.u.rFileInfo.u4FileAddr = (UINT32)phFile;
    rCmd.u.rFileInfo.fgAppend = fgAppend;
    x_memcpy(rCmd.u.rFileInfo.au1FilePath, pcFileName, sizeof(rCmd.u.rFileInfo.au1FilePath));

    if (NULL != _gfnPutEvent)
    {
        _gfnPutEvent( &rCmd );
    }
#endif    
    return TRUE;
}


BOOL Dbg_CloseUsbFile( UINT32 h_file, UINT32 *ph_File)
{
#ifndef __KERNEL__
    if ( NULL != h_file )
    {
        UNUSED( x_fm_close(h_file) );  
        h_file = NULL;
    } 
    *ph_File = NULL;
#else
    SWDMX_DBG_T rCmd = {0};

    rCmd.eCmd = SWDMX_DBG_DO_CLOSE_FILE;
    rCmd.u.rFileInfo.u4Src = 0;
    rCmd.u.rFileInfo.u4File = h_file;
    rCmd.u.rFileInfo.u4FileAddr = (UINT32)ph_File;

    if (NULL != _gfnPutEvent)
    {
        _gfnPutEvent( &rCmd );
    }
#endif     
    return TRUE;
}

BOOL Dbg_SaveBlock(CHAR *pcFileName, UINT32 u4Addr, UINT32 u4Size)
{
#ifndef __KERNEL__
    HANDLE_T h_file = NULL;
    Dbg_OpenUsbFile( pcFileName, &h_file, FALSE);
    Dbg_SaveUsbFile( h_file, u4Addr, u4Size);
    Dbg_CloseUsbFile(h_file, &h_file);
    return TRUE;
#else
    SWDMX_DBG_T rCmd = {0};

    rCmd.eCmd = SWDMX_DBG_DO_SAVE_BLOCK;
    rCmd.u.rBufInfo.u4Src = 0;
    rCmd.u.rBufInfo.pvBufStart = (VOID *)u4Addr;
    rCmd.u.rBufInfo.u4BlockSize = u4Size;
    x_memcpy(rCmd.u.rBufInfo.au1FilePath, pcFileName, sizeof(rCmd.u.rBufInfo.au1FilePath));

    if (NULL != _gfnPutEvent)
    {
        _gfnPutEvent( &rCmd );
    }

    return TRUE;
#endif
}

/*
function:       DBG_SaveUsbFile
                   Save the data to File
parameter:     pcFileName: 
                    u4Addr: Buffer start addr
                    u4Size: lenght should be saved
                    fgAppend: Open mode, TRUE if append mode, FALSE if TRUNC mode
return value:   TRUE: success       FALSE:failed	
*/
 BOOL Dbg_SaveUsbFile(HANDLE_T h_file, UINT32 u4Addr, UINT32 u4Size )
{
#ifndef __KERNEL__
    UINT32        u4WriteByte;

    POINTER_NULL( u4Addr, FALSE );
    POINTER_NULL( h_file, FALSE );

    if ( FMR_OK != x_fm_write(h_file, (VOID*)u4Addr, u4Size, &u4WriteByte) )
    {   
        if (DUMP_LOG)
        {
            LOG( 1, "%s:%u Write file failed.\n", __FUNCTION__, __LINE__ );
        }
        
        return FALSE;
    }
#else
    SWDMX_DBG_T rCmd = {0};

    rCmd.eCmd = SWDMX_DBG_DO_SAVE_DATA;
    rCmd.u.rBufInfo.u4Src = 0;
    rCmd.u.rBufInfo.u4File = h_file;
    rCmd.u.rBufInfo.pvBufStart = (VOID *)u4Addr;
    rCmd.u.rBufInfo.u4BlockSize = u4Size;

    if (NULL != _gfnPutEvent)
    {
        _gfnPutEvent( &rCmd );
    }
#endif
    return TRUE;
}

/*
function:         DBG_SaveUsbFileEx
                    save the data to File
                    Notes: This function take "ring buffer" into account

parameter:      @pi1_file_name: 
                    @u4_file_pos:       The data's offset of the orignal file
                    @u4_bufStart:       start of the ring buffer
                    @u4_bufEnd:         end of the ring buffer
                    @u4_readPos:       the frist byte need to be done
                    @u4Size:              lenght should be saved
                    @fgAppend: Open mode, TRUE if append mode, FALSE if TRUNC mode
return value:   TRUE: success       FALSE:failed	
*/
BOOL Dbg_SaveUsbFileEx(HANDLE_T h_file,
                                               UINT32 u4_bufStart,         // buf start pos
                                               UINT32 u4_bufEnd,           // buf end pos
                                               UINT32 u4_readPos,          // read pointer pos
                                               UINT32 u4_size )             // size will be done                                       
{
    UINT32  u4_size1 = 0;       // size of block 1
    UINT32  u4_size2 = 0;       // size of block 2

    POINTER_NULL( u4_readPos,  FALSE );
    POINTER_NULL( u4_bufStart,  FALSE );
    POINTER_NULL( h_file,           FALSE );
    POINTER_NULL( u4_bufEnd,    FALSE );

    // take the "ring buffer" into account, get the two data_block size
    if ( u4_size + u4_readPos < u4_bufEnd )
    {
        u4_size1 = u4_size;
    }
    else
    {
        u4_size1 = u4_bufEnd - u4_readPos;
        u4_size2 = u4_size - u4_size1;
    }

    if ( FALSE == Dbg_SaveUsbFile( h_file, u4_readPos, u4_size1 ) )
    {
        return FALSE;
    }
        
    if ( 0 != u4_size2 )
    {
        if ( FALSE == Dbg_SaveUsbFile( h_file, u4_bufStart, u4_size2 ) )
        {
            return FALSE;
        }
    }

    return TRUE;
}

/*
function:       Dbg_SaveCrc
                    get the data's crc, then save this to a file
                    each element is "off_set_of_the_file datas_size crc"

parameter:      pi1_file_name:  where to save the result(a usb file name)
                    u4_file_pos:      The data's offset of the orignal file
                    u4_addr:            buf start pos that we will deal with
                    u4_size:            size will be done

return value:   TRUE: success       FALSE:failed
*/
BOOL Dbg_SaveCrc(HANDLE_T h_file,
                                  UINT32 u4_file_pos,    // The offset of the file
                                  UINT32 u4_addr,        // buf start pos
                                  UINT32 u4_size )        // size will be done
{
    UINT32  u4_crc = 0;
    CHAR    au1_save[ 35 ];

    if ( FALSE == Dbg_GetCrc(u4_addr, u4_size, &u4_crc) )
    {
        return FALSE;
    }
#ifndef __KERNEL__
    UNUSED( /*x_vsprintf*/x_sprintf(au1_save, "%-10u %-10u %-10u\n",
                                   u4_file_pos, u4_size, u4_crc) );

    return Dbg_SaveUsbFile( h_file, (UINT32)(VOID *)au1_save, x_strlen( au1_save ) );
#else
{
    SWDMX_DBG_T rCmd = {0};

    rCmd.eCmd = SWDMX_DBG_DO_SAVE_CRC;
    rCmd.u.rCrcInfo.u4Src = 0;
    rCmd.u.rCrcInfo.u4File = h_file;
    rCmd.u.rCrcInfo.u4FileOffset = u4_file_pos;
    rCmd.u.rCrcInfo.u4BlockSize = u4_size;
    rCmd.u.rCrcInfo.u4Crc = u4_crc;
    
    if (NULL != _gfnPutEvent)
    {
        _gfnPutEvent( &rCmd );
    }
    
    UNUSED(au1_save);
    return TRUE;
}
#endif
}

/*
function:       Dbg_SaveCrcEx
                   get the data's crc, then save this to a file
                    each element is "off_set_of_the_file datas_size crc"
                    Notes: This function take "ring buffer" into account

parameter:      pi1_file_name:      where to save the result(a usb file name)
                    u4_file_pos:       The data's offset of the orignal file
                    u4_bufStart:       start of the ring buffer
                    u4_bufEnd:         end of the ring buffer
                    u4_readPos:       the frist byte need to be done
                    u4_size:            size will be done

return value:   TRUE: success       FALSE:failed
*/
BOOL Dbg_SaveCrcEx(HANDLE_T h_file,
                                        UINT32 u4_file_pos,         // The offset of the file
                                        UINT32 u4_bufStart,         // buf start pos
                                        UINT32 u4_bufEnd,           // buf end pos
                                        UINT32 u4_readPos,          // read pointer pos
                                        UINT32 u4_size )               // size will be done
{
    UINT32  u4_crc = 0;
    CHAR    au1_save[ 35 ];

    if ( FALSE == Dbg_GetCrcEx(u4_bufStart, u4_bufEnd,
                                             u4_readPos, u4_size, &u4_crc) )
    {
        return FALSE;
    }
#ifndef __KERNEL__
    UNUSED( /*x_vsprintf*/x_sprintf(au1_save, "%-10u %-10u %-10u\n",
                                  u4_file_pos, u4_size, u4_crc) );
                                  
    return Dbg_SaveUsbFile( h_file, (UINT32)(VOID *)au1_save, x_strlen( au1_save ) );
#else
{
    SWDMX_DBG_T rCmd = {0};

    rCmd.eCmd = SWDMX_DBG_DO_SAVE_CRC;
    rCmd.u.rCrcInfo.u4Src = 0;
    rCmd.u.rCrcInfo.u4File = h_file;
    rCmd.u.rCrcInfo.u4FileOffset = u4_file_pos;
    rCmd.u.rCrcInfo.u4BlockSize = u4_size;
    rCmd.u.rCrcInfo.u4Crc = u4_crc;
    
    if (NULL != _gfnPutEvent)
    {
        _gfnPutEvent( &rCmd );
    }
    
    UNUSED(au1_save);
    return TRUE;
}
#endif
}

#ifndef __KERNEL__
static UINT32 StrToInt( CHAR *pi1_str )
{
    UINT32 u4_len = 0; 
    UINT32 u4_value = 0;
    UINT32 i = 0;

    if ( NULL == pi1_str )
    {
         return u4_value;
    }

    u4_len = x_strlen( pi1_str );

    for( i=0; i<u4_len; i++ )
    {
        if ( pi1_str[i] - '0' >= 0 && pi1_str[i] - '0' < 10 )
        {
             u4_value *= 10;
             u4_value += pi1_str[i] - '0';
        }
        else
        {
            break;
        }
    }

    return u4_value;
}
#endif // __KERNEL__

UINT32 Dbg_GetOrignalCrc( HANDLE_T h_file,  UINT64 u8Offset, UINT32 *pu4Crc )
{
#ifndef __KERNEL__
    UINT64 u8FileSize, u8Pos;
    UINT32 u4Read, u4UnDeal;
    UINT64 u8FileRead = 0;
    UINT32 u4Count = 0;
    CHAR szBuf[ELEMENT_SIZE * ELEMENT_COUNT] = { 0 };
    BOOL fgEnd = FALSE;
    UINT32 u4Retvl = 0;

    POINTER_NULL( h_file, 3 );
    POINTER_NULL( pu4Crc, 3 );
    
    *pu4Crc = 0;

    do
    {
        VERIFY(FMR_OK == x_fm_lseek( h_file, 0, FM_SEEK_CUR, &u8Pos) );
        VERIFY(FMR_OK == x_fm_lseek( h_file, 0, FM_SEEK_END, &u8FileSize) );
        // Notes: &u8Pos is not modified before use u8Pos in x_fm_lseek
        VERIFY(FMR_OK == x_fm_lseek(h_file, u8Pos, FM_SEEK_BGN, &u8Pos) );
        
        while ( u8FileSize > u8FileRead )
        {
            UNUSED( x_fm_feof( h_file, &fgEnd ) );
            if ( TRUE == fgEnd )
            {
                VERIFY(FMR_OK == x_fm_lseek(h_file, 0, FM_SEEK_BGN, &u8Pos));
            }

            x_fm_read(h_file, (VOID *)szBuf, ELEMENT_SIZE * ELEMENT_COUNT, &u4Read);
            u4UnDeal = u4Read;
            u4Count = 0;

            while ( u4UnDeal >= ELEMENT_SIZE )
            {
                szBuf[ u4Count * ELEMENT_SIZE + START_BLOCK_SIZE - 1 ] = '\0';
                 if ( u8Offset == StrToInt( &szBuf[ u4Count * ELEMENT_SIZE + START_OFFSET ] ) )
                {
                    szBuf[ u4Count * ELEMENT_SIZE + START_CALC_CRC - 1] = '\0';
                    *pu4Crc = StrToInt( &szBuf[ u4Count * ELEMENT_SIZE + START_ORIGNAL_CRC ] );
                    VERIFY(FMR_OK == x_fm_lseek( h_file, u8Pos + ELEMENT_SIZE * (u4Count + 1), 
                                                                   FM_SEEK_BGN, &u8Pos ) );
                    
                    u4Retvl = 2;
                    break;
                }

                    u4Count ++;
                    u4UnDeal -= ELEMENT_SIZE;
            }

            if ( 2 == u4Retvl )
            {
                break;
            }

            u8Pos += u4Read;
            u8FileRead = u8FileRead + u4Read;
        }
    }while ( FALSE );
    
    if ( 0 == u4Retvl )
    {
        u4Retvl = 1;
        if (DUMP_LOG)
        {
            LOG(1, "%s:%u Can not find the offset:%u\n", __FUNCTION__, __LINE__, u8Offset );
        }
    }
    
    return u4Retvl;
#else
{
    SWDMX_DBG_T rCmd = {0};

    rCmd.eCmd = SWDMX_DBG_DO_LOAD_CRC;
    rCmd.u.rCrcInfo.u4Src = 0;
    rCmd.u.rCrcInfo.u4File = h_file;
    rCmd.u.rCrcInfo.u4FileOffset = u8Offset;
    
    if (NULL != _gfnPutEvent)
    {
        _gfnPutEvent( &rCmd );
    }
    return 0;
}
#endif
}

/*
function:       Dbg_GetCrc
                    calculate the CRC of a buf

parameter:  addr:   The start add of the buffer
                 len:   lenght of the buffer
                pu4_crc: [OUT] get the result CRC

return value:   TRUE: success   FALSE:failed	
*/
BOOL  Dbg_GetCrc( UINT32 addr, UINT32 len, UINT32 *pu4_crc)
{
    UINT32 u4_crc        = 0;
    UINT32 u4_count    = len;               // number of UINT32
    BYTE *pu1_pos      = NULL;
    BYTE u1_data1;
    BYTE u1_data2;
    BYTE u1_data3;

    POINTER_NULL( addr, FALSE );
    POINTER_NULL( pu4_crc, FALSE );

    u4_crc = *pu4_crc;
    pu1_pos = (BYTE *)addr;

    while ( 3 <= u4_count )
    {
        u1_data1 = *pu1_pos++;
        u1_data2 = *pu1_pos++;
        u1_data3 = *pu1_pos++;

        u4_crc += u1_data1;
        u4_crc += u1_data2;
        u4_crc += u1_data3;

        u4_count -= 3;
    }

    while ( 0 != u4_count )
    {
        u4_crc += *pu1_pos++;
        u4_count --;
    }
    
    *pu4_crc = u4_crc;

    return TRUE;
}

/*
function:       Dbg_GetCrcEx
                    get the data's crc
                    notes: This function take "ring buffer" into account

parameter:      u4_file_pos:        he data's offset of the orignal file
                     u4_bufStart:       start of the ring buffer
                     u4_bufEnd:         end of the ring buffer
                     u4_readPos:       the frist byte need to be done
                     u4_size:            size will be done

return value:   TRUE: success    FALSE:failed
*/
BOOL Dbg_GetCrcEx(UINT32 u4_bufStart, UINT32 u4_bufEnd,
                                     UINT32 u4_readPos,  UINT32 u4_size,
                                     UINT32 *pu4_crc )
{
    UINT32  u4_size1 = 0;       // size of block 1
    UINT32  u4_size2 = 0;       // size of block 2

    POINTER_NULL( u4_readPos,  FALSE );
    POINTER_NULL( u4_bufStart,  FALSE );
    POINTER_NULL( pu4_crc,       FALSE );
    POINTER_NULL( u4_bufEnd,   FALSE );
    ASSERT( u4_bufEnd - u4_bufStart >= u4_size );
    ASSERT( (u4_readPos >= u4_bufStart) && (u4_readPos < u4_bufEnd));
      
    // take the "ring buffer" into account, get the two data_block size
    if ( u4_size + u4_readPos < u4_bufEnd )
    {
        u4_size1 = u4_size;
    }
    else
    {
        u4_size1 = u4_bufEnd - u4_readPos;
        u4_size2 = u4_size - u4_size1;
    }

    if ( FALSE == Dbg_GetCrc(u4_readPos, u4_size1, pu4_crc) )
    {
        return FALSE;
    }

    if ( 0 != u4_size2 )
    {
        if ( FALSE == Dbg_GetCrc(u4_bufStart, u4_size2, pu4_crc) )
        {
            return FALSE;
        }
    }

    return TRUE;
}

/*
function:          DBG_log
                      et the data's crc, then print it to stdout
                      each element is "off_set_of_the_file datas_size crc"

parameter:       pos:   he data's offset of the orignal file
                       addr:    buf start pos that we will deal with
                       size:    size will be done

return value:   TRUE: seccess   false:failed
*/
BOOL Dbg_Log( UINT32 addr, UINT32 size, UINT64 pos ) 
{
    UINT32 u4_crc = 0;
    if ( FALSE == Dbg_GetCrc( addr, size, &u4_crc) )
    {
        return FALSE;
    }

    if (DUMP_LOG)
    {
        LOG(0, "%s:%s [%u %u %u]\n", __FUNCTION__, __LINE__,
                (UINT32)pos, size, u4_crc );
    }
    
    return TRUE;
}

/*
function:       DBG_log_ex
                   get the data's crc, then print it to stdout
                   each element is "off_set_of_the_file datas_size crc"
                   Notes: This function take "ring buffer" into account
            
parameter:  @pos:   The data's offset of the orignal file
                 @readPos:    read start pos that we will deal with
                 @size:    size will be done
            
return value:   TRUE:seccess    false:failed
*/
BOOL Dbg_LogEx( UINT32 bufStart, UINT32 bufEnd, UINT32 readPos, UINT32 size, UINT64 pos )
{
    UINT32 u4_crc = 0;

    if ( FALSE == Dbg_GetCrcEx( bufStart, bufEnd,
                                              readPos, size, &u4_crc) )
    {
        return FALSE;
    }

    if (DUMP_LOG)
    {
        LOG(1, "%s:%u [%u %u %u]\n", __FUNCTION__, __LINE__,
            (UINT32)pos, size, u4_crc );
    }
    
   return TRUE;
}

#endif // SWDMX_DBG_USB


// The semihosting related public functions
#ifdef SWDMX_DBG_SEMIHOST
/*
function:       T32_HostExec
                    Transfer command to T32 or CVD
parameter:      @i4TargetFlag: command type, a E_T32_XX (enum)
                    @szFormat: the command
return value:   ignored
*/
INT32 T32_HostExec(INT32 i4TargetFlag, const CHAR *szFormat, ...)
{
    CHAR szBuffer[256];
    VA_LIST t_ap;

    VA_START(t_ap, szFormat);
    UNUSED(x_vsprintf(szBuffer, szFormat, t_ap));
    VA_END(t_ap);

    UNUSED(_t32_HostBreakPT( i4TargetFlag, szBuffer ));

    return 0;
}

INT32 T32_HostExec2(INT32 i4TargetFlag,CHAR *puFilePath,UINT32 u4Address,UINT32 u4Size)
{
        UNUSED(_t32_HostBreakPT2(i4TargetFlag, puFilePath,u4Address,u4Size));
        return 0;
}

INT32 T32_HostDetect(void)
{
    INT32 i4Detect;

    i4Detect = _t32_HostBreakPT(E_T32_DETECT_HOST, NULL);
    if (i4Detect == E_T32_DETECT_HOST)
    {
        return -1;
    }

    return 0;
}

INT32 T32_HostSaveData(CHAR *szFileName, void *pvMemoryStart, UINT32 u4ByteSize)
{
    return T32_HostExec(E_T32_EXEC,
                                    "d.save.binary %s 0x%x--0x%x",
                                    szFileName,
                                    (UINT32)pvMemoryStart,
                                    ((UINT32)pvMemoryStart + u4ByteSize) - 1);
}

INT32 T32_HostLoadData(CHAR *szFileName, void *pvMemoryStart)
{
    return T32_HostExec(E_T32_EXEC_RELOAD_SYM,
                                    "d.load.binary %s 0x%x /ny",
                                    szFileName,
                                    (UINT32)pvMemoryStart);
}

INT32 T32_HostRestart(void)
{
    return _t32_HostBreakPT( E_T32_RESTART, "screen" );
}

INT32 T32_HostExit(void)
{
    return _t32_HostBreakPT( E_T32_EXIT, "screen" );
}

#endif // SWDMX_DBG_SEMIHOST

