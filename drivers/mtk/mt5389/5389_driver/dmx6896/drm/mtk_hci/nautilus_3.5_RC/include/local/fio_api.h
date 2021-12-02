/** @addtogroup base base
 * @{ */

/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2004,2005,2006,2007,2008,2009,2010 Sony Corporation, All Rights Reserved.
 *
 */

#ifndef FIO_API_H
#define FIO_API_H

/** @file
 * File I/O API
 *
 * @require cmn/base/mem
 * @author Norifumi Goto <n-goto@sm.sony.co.jp>
 */

NP_BEGIN_EXTERN_C

/*---------------------------------------------------------------------------*
 * type definition
 *---------------------------------------------------------------------------*/
/**
 * file-handle definition
 */
typedef void *fio_handle_t;

/*
 * file size (basic_types_.h)
 *
 */
/*
typedef int32_t fio_size_t;
 */

/**
 * @brief file offset
 */
/*
typedef int32_t fio_offset_t;
 */

/**
 * file stat definition
 */
typedef struct fio_stat_tag {
    u_int32_t  st_mode;        /**< File mode.  */
    fio_size_t st_size;        /**< Size of file, in bytes.  */
} fio_stat_t;

typedef struct fio_file_list_tag {
    struct fio_file_list_tag * next_file; /**< Pointer to the next file. */
    char *                     file_name;  /**< File Name. */
} fio_file_list_t;

/*---------------------------------------------------------------------------*
 * macro definition
 *---------------------------------------------------------------------------*/

/**
 * Null file-handle
 */
#define FIO_NULL ((fio_handle_t)0)

#define FIO_FILEPATH_MAX    256

/**
 * Open Mode
 */
/* internal use */
#define FIO_L_OPENMODEFLAG_READ        (1<<0)
#define FIO_L_OPENMODEFLAG_WRITE       (1<<1)
#define FIO_L_OPENMODEFLAG_APPEND      (1<<2)
#define FIO_L_OPENMODEFLAG_TEXT        (1<<3)
/* exported macros */
#define FIO_OPENMODEMASK_ACCESS      (FIO_L_OPENMODEFLAG_READ|FIO_L_OPENMODEFLAG_WRITE|FIO_L_OPENMODEFLAG_APPEND)
#define FIO_OPENMODEFLAG_TEXT        FIO_L_OPENMODEFLAG_TEXT
#define FIO_OPENMODE_READ            FIO_L_OPENMODEFLAG_READ
#define FIO_OPENMODE_WRITE           FIO_L_OPENMODEFLAG_WRITE
#define FIO_OPENMODE_READWRITE       (FIO_L_OPENMODEFLAG_READ|FIO_L_OPENMODEFLAG_WRITE)
#define FIO_OPENMODE_APPEND          FIO_L_OPENMODEFLAG_APPEND
#define FIO_OPENMODE_READ_TEXT       (FIO_OPENMODE_READ|FIO_OPENMODEFLAG_TEXT)
#define FIO_OPENMODE_WRITE_TEXT      (FIO_OPENMODE_WRITE|FIO_OPENMODEFLAG_TEXT)
#define FIO_OPENMODE_READWRITE_TEXT  (FIO_OPENMODE_READWRITE|FIO_OPENMODEFLAG_TEXT)
#define FIO_OPENMODE_APPEND_TEXT     (FIO_OPENMODE_APPEND|FIO_OPENMODEFLAG_TEXT)

/**
 * Base Position definition
 * FIO_SEEK_SET: beginning of file
 * FIO_SEEK_CUR: current position
 * FIO_SEEK_END: end of file
 *
 */
#define FIO_SEEK_SET 0
#define FIO_SEEK_CUR 1
#define FIO_SEEK_END 2

/**
 * File types.
 */
#define FIO_STAT_IFDIR        0    /**< directory  */
#define FIO_STAT_IFREG        1    /**< regular file  */

/**
 * Test macros for file types.
 */
#define FIO_STAT_ISDIR(fsb)    (((fsb)->st_mode) == FIO_STAT_IFDIR)
#define FIO_STAT_ISREG(fsb)    (((fsb)->st_mode) == FIO_STAT_IFREG)

/**
 * File I/O Errors
 */
#define ERR_FIO_CANNOTOPEN    (ERR_FIO_BASE|1)
#define ERR_FIO_NOTEXIST      (ERR_FIO_BASE|2)
#define ERR_FIO_CANNOTREAD    (ERR_FIO_BASE|3)
#define ERR_FIO_CANNOTWRITE   (ERR_FIO_BASE|4)
#define ERR_FIO_CANNOTSEEK    (ERR_FIO_BASE|5)
#define ERR_FIO_CANNOTTELL    (ERR_FIO_BASE|6)
#define ERR_FIO_CANNOTSTAT    (ERR_FIO_BASE|7)
#define ERR_FIO_ILLEGALMODE   (ERR_FIO_BASE|8)

/*---------------------------------------------------------------------------*
 * APIs
 *---------------------------------------------------------------------------*/

/**
 * @brief  Opens the  file
 *
 * Open the file specified by in_filename with the mode in_openmode.
 * If it is successfully opened, a new file handle is created and stored
 * in out_handle.  Otherwise the contents of out_handle is not modified.
 *
 * @param [in] in_context       Context
 * @param [in] in_filename      File name to be opened
 * @param [in] in_openmode      Open mode (FIO_OPENMODE_*)
 * @param [out] out_handle      The buffer where a new file handle is stored
 *
 * @return ::ERR_OK                     <br>Indicating success.
 * @return ::ERR_ILLEGALARGS  		<br>The input arguments are incorrect. Arguments cannot be NULL.
 * @return ::ERR_NOMEM                  <br>If failed to allocate the file handle buffer
 * @return ::ERR_FIO_ILLEGALMODE        <br>If in_openmode is illegal
 * @return ::ERR_FIO_CANNOTOPEN         <br>If failed to open the file
 * @return ::ERR_FIO_NOTEXIST           <br>If file doesn't exist.
 */
extern retcode_t
npi_fio_Open(
    npi_cmncontext_t   in_context,
    const char *       in_filename,
    int32_t            in_openmode,
    fio_handle_t *     out_handle);

/**
 * @brief Closes the  file
 *
 * Close the file handle. The buffer of the file handle is freed here.
 *
 * @param [in] in_context       Context
 * @param [in,out] io_handle    Handle to be closed
 *
 * @return ::ERR_OK                     <br>If the handle is successfully closed
 * @return ::ERR_ILLEGALHANDLE          <br>If io_handle is NULL or illegal
 * @return ::ERR_ILLEGALARGS            <br>The input arguments are incorrect. Arguments cannot be NULL.
 * @return ::ERR_INTERNALERROR          <br>An unrecoverable error has occurred in Nautilus.
 */
extern retcode_t
npi_fio_Close(
    npi_cmncontext_t   in_context,
    fio_handle_t       io_handle);

/**
 * @brief Reads from file
 *
 * @param [in] in_context       Context
 * @param [in,out] io_handle    File handle
 * @param [in] in_size          Size of read data
 * @param [out] out_size        The buffer where the real read size is stored
 * @param [out] out_buffer      The buffer where read data are stored
 *
 * @return ::ERR_OK                     <br>Indiacting  success
 * @return ::ERR_ILLEGALHANDLE          <br>If io_handle is NULL or illegal
 * @return ::ERR_ILLEGALARGS            <br>If out_size or out_buffer is NULL, or in_size <= 0
 * @return ::ERR_FIO_CANNOTREAD         <br>If read failed
 */
extern retcode_t
npi_fio_Read(
    npi_cmncontext_t   in_context,
    fio_handle_t       io_handle,
    fio_size_t         in_size,
    fio_size_t *       out_size,
    void *             out_buffer);

/**
 * @brief Reads from file
 *
 * Read max in_size bytes from file until the first occurence of the byte in_delimit.
 *
 * @param [in] in_context       Context
 * @param [in,out] io_handle    File handle
 * @param [in] in_size          Size of read data
 * @param [in] in_delmit        The byte. Until it's first occurence, data will be read.
 * @param [out] out_size        The buffer where the real read size is stored
 * @param [out] out_buffer      The buffer where read data are stored
 *
 * @return ::ERR_OK                     <br>Indicating success
 * @return ::ERR_ILLEGALHANDLE          <br>If io_handle is NULL or illegal
 * @return ::ERR_ILLEGALARGS            <br>The input arguments are incorrect. Arguments cannot be NULL.
 */
extern retcode_t
npi_fio_ReadUntil(
    npi_cmncontext_t  in_context,
    fio_handle_t      io_handle,
    int32_t           in_delimit,
    fio_size_t        in_size,
    fio_size_t *      out_size,
    void *            out_buffer);

/**
 * @brief Writes to file
 *
 * @param [in] in_context       Context
 * @param [in, out] io_handle   File handle
 * @param [in] in_size          Size of buffer to be written to file
 * @param [in] in_buffer        Buffer to be written to file
 *
 * @return ::ERR_OK                     <br>Indicating success
 * @return ::ERR_ILLEGALHANDLE          <br>If handle is NULL or in use by another protocol 
 * @return ::ERR_ILLEGALARGS            <br>The input arguments are incorrect. Arguments cannot be NULL.
 * @return ::ERR_FIO_CANNOTWRITE        <br>If write failed
 */
extern retcode_t
npi_fio_Write(
    npi_cmncontext_t    in_context,
    fio_handle_t        io_handle,
    fio_size_t          in_size,
    const void *        in_buffer);

/**
 * @brief Sets current file position
 *
 * @param [in] in_context       Context
 * @param [in, out] io_handle   File handle
 * @param [in] in_offset        Offset from base position
 * @param [in] in_whence        Base position (FIO_SEEK_SET/CUR/END)
 *
 * @return ::ERR_OK                     <br>Indicating success
 * @return ::ERR_ILLEGALHANDLE          <br>If handle is NULL or in use by another protocol
 * @return ::ERR_ILLEGALARGS            <br>The input arguments are incorrect. Arguments cannot be NULL.
 * @return ::ERR_FIO_CANONOTSEEK        <br>If seek failed
 */
extern retcode_t
npi_fio_Seek(
    npi_cmncontext_t   in_context,
    fio_handle_t       io_handle,
    fio_offset_t       in_offset,
    int32_t            in_whence);

/**
 * @brief Gets current file position
 *
 * @param [in] in_context       Context
 * @param [in,out] io_handle    File handle
 * @param [out] out_size        The current position in file
 *
 * @return ::ERR_OK                     <br>Indicating success
 * @return ::ERR_ILLEGALHANDLE          <br>If  handle is NULL or in use by another protocol
 * @return ::ERR_ILLEGALARGS            <br>The input arguments are incorrect. Arguments cannot be NULL. 
 * @return ::ERR_FIO_CANNOTTELL         <br>If tell failed
 */
extern retcode_t
npi_fio_Tell(
    npi_cmncontext_t    in_context,
    fio_handle_t        io_handle,
    fio_offset_t *      out_size);

/**
 * @brief Gets file-size
 *
 * @param [in] in_context       Context
 * @param [in,out] io_handle    File handle
 * @param [out] out_size        The size of the file
 *
 * @return ::ERR_OK                     <br>Indicating success
 * @return ::ERR_ILLEGALHANDLE          <br>If  handle is NULL or in use by another protocol
 * @return ::ERR_ILLEGALARGS            <br>The input arguments are incorrect. Arguments cannot be NULL. 
 * @return ::ERR_FIO_CANONOTSEEK        <br>If seek failed
 * @return ::ERR_FIO_CANNOTTELL         <br>If tell failed
 */
extern retcode_t
npi_fio_Length(
    npi_cmncontext_t    in_context,
    fio_handle_t        io_handle,
    fio_size_t *        out_size);

/**
 * @brief Removes a file
 *
 * @param [in] in_context       Context
 * @param [in] in_filename      File handle to be removed
 *
 * @return ::ERR_OK                     <br>Indicating success
 * @return ::ERR_FIO_CANNOTOPEN         <br>If remove failed
 * @return ::ERR_FIO_NOTEXIST           <br>If File doesnot exist
 */
extern retcode_t
npi_fio_Remove(
    npi_cmncontext_t    in_context,
    const char *        in_filename);

/**
 * @brief Makes a directory
 *
 * @param [in] in_context       Context
 * @param [in] in_dir           Direcotory name
 *
 * @return ::ERR_OK                     <br>Indicating success
 * @return ::ERR_NOMEM                  <br>A resource failure has occurred.
 * @return ::ERR_ILLEGALARGS            <br>The input arguments are incorrect. Arguments cannot be NULL.
 * @return ::ERR_FIO_CANNOTOPEN         <br>If make directory failed
 */
extern retcode_t
npi_fio_MakeDir(
    npi_cmncontext_t  in_context,
    char *            in_dir);

/**
 * @brief Gets the user's home directory
 *
 * @param [in] in_context       Context
 * @param [in,out] io_len       The length of buffer out_buf
 * @param [out] out_buf         The buffer where home directory path will be written
 *
 * @return ::ERR_OK                     <br>Indicating success
 * @return ::ERR_NOMEM                  <br>A resource failure has occurred.
 * @return ::ERR_ILLEGALARGS            <br>The input arguments are incorrect. Arguments cannot be NULL.
 * @return ::ERR_NG                     <br>If failed to get the environment variables.
 */
extern retcode_t
npi_fio_GetHomeDir(
    npi_cmncontext_t in_context,
    np_size_t *      io_len,
    char *           out_buf);

/**
 * @brief Appends the file/directory path to a directory path
 *
 * @param [in,out] io_len       The length of the buffer
 * @param [in,out] io_buf       The path buffer
 * @param [in] in_addpath       The path which is appended to the buffer
 *
 * @return ::ERR_OK                     <br>Indicating  success
 * @return ::ERR_NG                     <br>If failed
 * @return ::ERR_ILLEGALARGS            <br>The input arguments are incorrect. Arguments cannot be NULL. 
 */
extern retcode_t
npi_fio_AddPath(
    np_size_t *      io_len,
    char *           io_buf,
    char *           in_addpath);

/**
 * @brief Gets file status
 *
 * @param [in] in_context       Context
 * @param [in] in_filename      File name
 * @param [out] fio_stat_t      Structure used to represent file mode and size. 
 * @param [out] out_statbuf     The buffer where a file status is stored
 
 * @return ::ERR_OK                     <br>Indicating success
 * @return ::ERR_ILLEGALARGS            <br>The input arguments are incorrect. Arguments cannot be NULL.
 * @return ::ERR_FIO_NOTEXIST           <br>If in_filename does not exist
 * @return ::ERR_FIO_CANNOTSTAT         <br>If failed to get file status
 */
extern retcode_t
npi_fio_Stat(
    npi_cmncontext_t    in_context,
    const char *        in_filename,
    fio_stat_t *        out_statbuf);

/**
 * Get file size from file status
 * Get file size from file status by fio_Stat.
 * But the size acquisition of directory does not support.
 *
 * @param [in] in_statbuf       File status buffer
 * @return                      File size
 */
#define npi_fio_Stat_FileSize(fsb)    ((fsb)->st_size)

/** 
 * @brief Checks if a path points to a directory.
 *
 * @param [in]  in_path         The path to check.
 * @param [out] out_result      0, if path is a directory; -1, otherwise
 *
 * @return ::ERR_OK                     <br>Indicating success
 * @return ::ERR_NOMEM                  <br>A resource failure has occurred.
 * @return ::ERR_ILLEGALARGS            <br>The input arguments are incorrect. Arguments cannot be NULL.
 */
extern retcode_t
npi_fio_IsDirectory(
    char    *in_path,
    int32_t *out_result);

/**
 * Returns the file delimiter ("/" or "\") for the current platform.
 */
extern char
npi_fio_GetFileDelimiter(void);

/** 
 * @brief Removes a directory.
 *
 * @param [in]  in_path         The path of the directory to be removed.
 *
 * @return ::ERR_OK                     <br>Indicating success
 * @return ::ERR_NG                     <br>A failure has occurred.
 */
extern retcode_t
npi_fio_RemoveDirectory(const char *in_path);

/**
 * @brief Returns all the files from the directory.
 *
 *
 * @param [in]  in_context      Security context (either NPI_CMN_CONTEXT_SECURE or NPI_CMN_CONTEXT_NONSECURE).
 * @param [in]  in_path         The path of the directory to be searched.
 * @param [out] out_files       List of file names.
 *
 * @return ::ERR_OK                     <br>Indicating success
 * @return ::ERR_NOMEM                  <br>Resource failure has occurred. 
 * @return ::ERR_ILLEGALARGS            <br>Input arguments are incorrect. Arguments cannot be NULL. 
 * @return ::ERR_FIO_NOT_EXIST          <br>Directory does not exist.
 * @return ::ERR_NG                     <br>A failure has occurred.
 */
extern retcode_t 
npi_fio_GetDirectoryFiles(
    npi_cmncontext_t   in_context,
    const char *       in_path,
    fio_file_list_t ** out_files);

/**
 * @brief Frees a lists of file names allocated by npi_fio_GetDirectoryFiles().
 *
 * @param [in]  in_context     Security context (either NPI_CMN_CONTEXT_SECURE or NPI_CMN_CONTEXT_NONSECURE).
 * @param [out] in_files       NULL or List of file names.
 *
 * @return ::ERR_OK                     <br>Indicating success
 */
extern retcode_t 
npi_fio_FreeFileList(
    npi_cmncontext_t  in_context,
    fio_file_list_t * in_files);

NP_END_EXTERN_C

#endif /* FIO_API_H */
/** @} */
