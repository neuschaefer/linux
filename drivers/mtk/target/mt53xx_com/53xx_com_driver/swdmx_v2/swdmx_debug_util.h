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

#ifndef SWDMX_DEBUG_UTIL
#define SWDMX_DEBUG_UTIL

#include "x_typedef.h"
#include "swdmx_if.h"
#include "swdmx_drvif.h"

#if defined(__KERNEL__) && defined(SWDMX_DBG_USB)
#include<linux/cb_data.h>
typedef VOID (* PFN_PUT_ENVENT)(SWDMX_DBG_T *prCmd);
#endif // defined(__KERNEL__) && defined(SWDMX_DBG_USB)

#if defined(SWDMX_DBG_USB) || defined(SWDMX_DBG_SEMIHOST)
// MPG module related format, "except" can be SWDMX_FMT_UNKNOWN/0
#define IS_MPEG_MODULE_EXCEPT(format, exceptFormat) \
(                                                                                     \
   (format == SWDMX_FMT_MPEG1_DAT                    ||         \
    format == SWDMX_FMT_MPEG2_PS                      ||        \
    format == SWDMX_FMT_MPEG2_TS                      ||        \
    format == SWDMX_FMT_MPEG2_TS_192                ||       \
    format == SWDMX_FMT_MPEG2_TS_ZERO_192       ||       \
    format == SWDMX_FMT_MPEG2_TS_ENCRYPT_192  ||       \
    format == SWDMX_FMT_VC1_ES                          ||       \
    format == SWDMX_FMT_MPEG2_VIDEO_ES             ||       \
    format == SWDMX_FMT_MPEG4_VIDEO_ES             ||       \
    format == SWDMX_FMT_H264_VIDEO_ES               ||       \
    format == SWDMX_FMT_AVS_VIDEO_ES                ||       \
    format == SWDMX_FMT_RAW_AUDIO_ES               ||       \
    format == SWDMX_FMT_MP3_AUDIO_ES                ||       \
    format == SWDMX_FMT_VC1_WMV                       ||       \
    format == SWDMX_FMT_WMA_AUDIO_ES               ||       \
    format == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)    &&       \
    format != exceptFormat )

// to determine pointer a, if it is NULL, then return b 
#define POINTER_NULL(a, b)                              \
do                                                                   \
{                                                                     \
    if ( 0 == (a) )                                                 \
    {                                                                  \
        printf("%s: NULL value.\n", #a);                     \
        return (b);                                                 \
    }                                                                   \
}while( FALSE )
#endif // defined(SWDMX_DBG_USB) || defined(SWDMX_DBG_SEMIHOST)

#ifdef SWDMX_DBG_USB
/*
function:   Dbg_OpenUsbFile

parameter:  @pcFileName:
                 @fgAppend: Open mode, TRUE if append mode, FALSE if TRUNC mode

return value:   HANDLE of the opened file(NULL indicate open file failed)
*/
BOOL Dbg_OpenUsbFile( CHAR *pcFileName, UINT32 *pu4hFile, BOOL fgAppend );

/*
function:   Bbg_CloseUsbFile

parameter: 
                 @h_file: Handle of the file

return value:  TRUE: seccess
*/
BOOL Dbg_CloseUsbFile( UINT32 h_file, UINT32 *pu4hFile);

/*
function:    DBG_SaveUsbFile
                save the data to File
parameter: @h_file: 
                @u4Addr: Buffer start addr
                @u4Size: lenght should be saved

return value: TRUE: success     FALSE:failed
*/
BOOL Dbg_SaveUsbFile(HANDLE_T h_file, UINT32 u4Addr, UINT32 u4Size );


/*
function:   DBG_SaveUsbFileEx
                save the data to File
                Notes: This function take "ring buffer" into account

parameter:  @pi1_file_name: 
                 @u4_file_pos:  the data's offset of the orignal file
                 @u4_bufStart:  start of the ring buffer
                 @u4_bufEnd:  end of the ring buffer
                 @u4_readPos:  the frist byte need to be done
                 @u4Size: lenght should be saved

return value:TRUE: success  FALSE:failed
*/
BOOL Dbg_SaveUsbFileEx(HANDLE_T h_file,
                                               UINT32 u4_bufStart,         // buf start pos
                                               UINT32 u4_bufEnd,           // buf end pos
                                               UINT32 u4_readPos,          // read pointer pos
                                               UINT32 u4_size ) ;            // size will be done        

/*
function:   T32_LoadUsbFile(Get data from the files in the target)	
parameter:
            @pcFileName:    Filename
            @u4Addr:    Address of buffer where to stroe the datas
            @u4Size:    The buffer size
return value:   Byte of data the function got
*/
UINT32 Dbg_LoadUsbFile(CHAR *pcFileName, UINT32 u4Addr, UINT32 u4Size );

/*
function:       Dbg_SaveCrc
                    get the data's crc, then save this to a file
                    each element is "off_set_of_the_file datas_size crc"

parameter:      @pi1_file_name:     where to save the result(a usb file name)
                    @u4_file_pos:       The data's offset of the orignal file
                    @u4_addr:            buf start pos that we will deal with
                    @u4_size:            size will be done

return value:   TRUE: success       FALSE:failed
*/
BOOL Dbg_SaveCrc(HANDLE_T h_file,
                                  UINT32 u4_file_pos,    // The offset of the file
                                  UINT32 u4_addr,        // buf start pos
                                  UINT32 u4_size ) ;       // size will be done


/*
function:    Dbg_SaveCrcEx
                get the data's crc, then save this to a file
                each element is "off_set_of_the_file datas_size crc"
                Notes: This function take "ring buffer" into account

parameter:      @pi1_file_name:     where to save the result(a usb file name)
                    @u4_file_pos:       The data's offset of the orignal file
                    @u4_bufStart:       start of the ring buffer
                    @u4_bufEnd:         end of the ring buffer
                    @u4_readPos:       the frist byte need to be done
                    @u4_size:            size will be done

return value:   TRUE: success       FALSE:failed
*/
BOOL Dbg_SaveCrcEx(HANDLE_T h_file,
                                        UINT32 u4_file_pos,         // The offset of the file
                                        UINT32 u4_bufStart,         // buf start pos
                                        UINT32 u4_bufEnd,           // buf end pos
                                        UINT32 u4_readPos,          // read pointer pos
                                        UINT32 u4_size ) ;              // size will be done
#ifndef __KERNEL_
#define ELEMENT_SIZE            ((UINT32) 46)
#define ELEMENT_COUNT         ((UINT32) 16)
        
#define START_OFFSET            ((UINT32) 0)
#define START_BLOCK_SIZE     ((UINT32) 11)
#define START_ORIGNAL_CRC   ((UINT32) 22)
#define START_CALC_CRC        ((UINT32) 33)
#define START_CHECK_FALG    ((UINT32) 44)
#endif // __KERNEL__

UINT32 Dbg_GetOrignalCrc( HANDLE_T h_file,  UINT64 u8Offset, UINT32 *pu4Crc );
/*
function:   Dbg_GetCrc
                calculate the CRC of a buf

parameter:  @addr:  The start add of the buffer
                @len:   lenght of the buffer
                @pu4_crc: [OUT] get the result CRC

return value:   TRUE: success       FALSE:failed	
*/
BOOL  Dbg_GetCrc( UINT32 addr, UINT32 len, UINT32 *pu4_crc);

/*
function:        Dbg_GetCrcEx
                    get the data's crc
                    Notes: This function take "ring buffer" into account

parameter:      @u4_file_pos:       The data's offset of the orignal file
                     @u4_bufStart:       start of the ring buffer
                    @u4_bufEnd:         end of the ring buffer
                    @u4_readPos:       the frist byte need to be done
                    @u4_size:            size will be done

return value:   TRUE: success       FALSE:failed
*/
BOOL Dbg_GetCrcEx(UINT32 u4_bufStart, UINT32 u4_bufEnd,
                                     UINT32 u4_readPos,  UINT32 u4_size,
                                     UINT32 *pu4_crc);


/*
function:          DBG_log
                      et the data's crc, then print it to stdout
                      each element is "off_set_of_the_file datas_size crc"

parameter:       pos:   he data's offset of the orignal file
                       addr:    buf start pos that we will deal with
                       size:    size will be done

return value:   TRUE: seccess   false:failed
*/
BOOL Dbg_Log( UINT32 addr, UINT32 size, UINT64 pos ) ;

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
BOOL Dbg_LogEx( UINT32 bufStart, UINT32 bufEnd,
                                UINT32 readPos, UINT32 size, UINT64 pos );
#endif

#ifdef SWDMX_DBG_SEMIHOST
// semihosting CMD type
enum
{
    E_T32_NOP = 0,
    E_T32_EXEC,
    E_T32_RESTART,
    E_T32_EXIT,
    E_T32_EXEC_RELOAD_SYM,
    E_T32_DETECT_HOST,
    E_T32_END
};
INT32 T32_HostExec(INT32 i4TargetFlag, const CHAR *szFormat, ...);
INT32 T32_HostExec2(INT32 i4TargetFlag,CHAR *puFilePath,UINT32 u4Address,UINT32 u4Size);
INT32 T32_HostDetect(void);
INT32 T32_HostSaveData(CHAR *szFileName, void *pvMemoryStart, UINT32 u4ByteSize);
INT32 T32_HostLoadData(CHAR *szFileName, void *pvMemoryStart);
INT32 T32_HostRestart(void);
INT32 T32_HostExit(void);

#define Dbg_HostExec                 T32_HostExec    
#define Dbg_HostExec2               T32_HostExec2
#define Dbg_HostDetect              T32_HostDetect
#define Dbg_HostSaveData          T32_HostSaveData
#define Dbg_HostLoadData          T32_HostLoadData
#define Dbg_HostRestart             T32_HostRestart
#define Dbg_HostExit                  T32_HostExit

#endif // SWDMX_DBG_SEMIHOST
#endif // SWDMX_DEBUG_UTIL

