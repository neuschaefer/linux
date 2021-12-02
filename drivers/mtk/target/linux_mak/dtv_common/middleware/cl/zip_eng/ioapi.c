/* ioapi.c -- IO base function header for compress/uncompress .zip
   files using zlib + zip or unzip API

   Version 1.01, May 8th, 2004

   Copyright (C) 1998-2004 Gilles Vollant
*/
#include "dbg/x_dbg.h"
#include "os/inc/x_os.h"
#include "inc/x_common.h"
#include "zlib.h"
#include "ioapi.h"



/* I've found an old Unix (a SunOS 4.1.3_U1) without all SEEK_* defined.... */

#ifndef SEEK_CUR
#define SEEK_CUR    1
#endif

#ifndef SEEK_END
#define SEEK_END    2
#endif

#ifndef SEEK_SET
#define SEEK_SET    0
#endif

voidpf ZCALLBACK fopen_file_func OF((
   voidpf opaque,
   const char* filename,
   int mode));

uLong ZCALLBACK fread_file_func OF((
   voidpf opaque,
   voidpf stream,
   void* buf,
   uLong size));

uLong ZCALLBACK fwrite_file_func OF((
   voidpf opaque,
   voidpf stream,
   const void* buf,
   uLong size));

long ZCALLBACK ftell_file_func OF((
   voidpf opaque,
   voidpf stream));

long ZCALLBACK fseek_file_func OF((
   voidpf opaque,
   voidpf stream,
   uLong offset,
   int origin));

int ZCALLBACK fclose_file_func OF((
   voidpf opaque,
   voidpf stream));

int ZCALLBACK ferror_file_func OF((
   voidpf opaque,
   voidpf stream));

#ifdef CL_SUPPORT_ARCHIVE
voidpf ZCALLBACK fopen_file_func (voidpf opaque, const char* filename, int mode)
{
    UINT32    ui4_fm_mode = 0;
    INT32      i4_ret;
    HANDLE_T    h_file = NULL_HANDLE;

    if ((mode & ZLIB_FILEFUNC_MODE_READWRITEFILTER)==ZLIB_FILEFUNC_MODE_READ)
    {
        ui4_fm_mode = FM_READ_ONLY|FM_OPEN_CREATE;
    }
    else
    if (mode & ZLIB_FILEFUNC_MODE_EXISTING)
    {
        ui4_fm_mode = FM_READ_WRITE|FM_OPEN_CREATE;
    }
    else
    if (mode & ZLIB_FILEFUNC_MODE_CREATE)
    {
        ui4_fm_mode = FM_WRITE_ONLY|FM_OPEN_CREATE;
    }

    if (filename != NULL)
        {
        i4_ret = x_fm_open(FM_ROOT_HANDLE,
                           filename,
                           ui4_fm_mode,
                           0777,
                           FALSE,
                           &h_file);

        if (i4_ret != FMR_OK)
        {
            return NULL_HANDLE;
        }
        }
    return (voidpf)h_file;
}


uLong ZCALLBACK fread_file_func (voidpf opaque, voidpf stream, void* buf, uLong size)
{
    INT32      i4_ret;
    UINT32    ui4_read_count;

    i4_ret = x_fm_read((HANDLE_T)stream, buf, (size_t)size, &ui4_read_count);

    if (i4_ret != FMR_OK)
    {
        return 0;
    }

    return ui4_read_count;
}


uLong ZCALLBACK fwrite_file_func (voidpf opaque, voidpf stream, const void* buf, uLong size)
{
    INT32      i4_ret;
    UINT32    ui4_write_count;

    i4_ret = x_fm_write((HANDLE_T)stream, (VOID *)buf, (size_t)size, &ui4_write_count);

    if (i4_ret != FMR_OK)
    {
        return 0;
    }

    return ui4_write_count;
}

long ZCALLBACK ftell_file_func (voidpf opaque, voidpf stream)
{
    INT32      i4_ret;
    UINT64    ui8_pos;

    i4_ret = x_fm_lseek((HANDLE_T)stream, 0, FM_SEEK_CUR, &ui8_pos);

     if (i4_ret != FMR_OK)
    {
        return 0;
    }
    if (ui8_pos&0xffff8000)
    {
        DBG_ERROR(("[CL] get file position out of range\n"));
        return 0;
    }

    return (long)ui8_pos;
}

long ZCALLBACK fseek_file_func (voidpf opaque, voidpf stream, uLong offset, int origin)

{
    INT32    i4_ret;
    UINT8     fseek_origin=0;
    UINT64    ui8_pos;

    switch (origin)
    {
    case ZLIB_FILEFUNC_SEEK_CUR :
        fseek_origin = FM_SEEK_CUR;
        break;
    case ZLIB_FILEFUNC_SEEK_END :
        fseek_origin = FM_SEEK_END;
        break;
    case ZLIB_FILEFUNC_SEEK_SET :
        fseek_origin = FM_SEEK_BGN;
        break;
    default: return -1;
    }

    i4_ret = x_fm_lseek((HANDLE_T)stream, offset, fseek_origin, &ui8_pos);
    if (i4_ret != FMR_OK)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

int ZCALLBACK fclose_file_func (voidpf opaque, voidpf stream)
{
    INT32    i4_ret;

    i4_ret = x_fm_close((HANDLE_T)stream);
    if (i4_ret == FMR_OK)
    {
        return 0;
    }
    else
    {

         DBG_ERROR(("[CL] x_fm_close FAIL\n"));
         return -1;
    }
}

int ZCALLBACK ferror_file_func (voidpf opaque, voidpf stream)
{
    DBG_ERROR(("[CL] ferror_file_func\n"));
    return 0;
}

void fill_fopen_filefunc (zlib_filefunc_def* pzlib_filefunc_def)
{
    pzlib_filefunc_def->zopen_file = fopen_file_func;
    pzlib_filefunc_def->zread_file = fread_file_func;
    pzlib_filefunc_def->zwrite_file = fwrite_file_func;
    pzlib_filefunc_def->ztell_file = ftell_file_func;
    pzlib_filefunc_def->zseek_file = fseek_file_func;
    pzlib_filefunc_def->zclose_file = fclose_file_func;
    pzlib_filefunc_def->zerror_file = ferror_file_func;
    pzlib_filefunc_def->opaque = NULL;
}
#endif

