/**
 * Copyright 2005 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief Cabot's portable interface to a file system.
 *
 * The ICE File System API provides an abstraction of basic file system operations
 * that should be portable across a variety of underlying file systems.
 * To maximize portability the following conventions are adopted :-
 *  - There is no concept of a current directory or of relative paths. All references
 *    are done with full absolute file specifications (filespecs).
 *  - There are no '.' and '..' directory references, and they must not be used in
 *    file specifications.
 *  - There is no concept of file permissions, ownership, or read-only files.
 *    (However, USB disks may be completely read-only).
 *  - Each physical drive may be partitioned into logical disks designated
 *    with a partition identifier. Theoretically each drive may be partitioned into
 *    26 partitions. In practice specific products will determine the maximum required,
 *    for example Cabot DVR will only use a maximum of two per drive.
 *  - File specifications must be nul-terminated UTF-8 encoded strings.
 *  - Eclipse will use a restricted set of characters in a file specification.
 *    This set consists of 'A'-'Z', 'a'-'z', '0'-'9', and the symbols '-', '.', '~', '_'
 *    and '%' (the latter can used to percent-encode characters not in this set).
 *    However, with external devices, all characters supported by its filesystem should be
 *    allowed in file specifications.
 *    ICE_Fs functions should fail if a caller uses an unsupported file specification.
 *  - File specifications follow the format
 *    "[drive_number],[partition]:/[dirname]/[dirname]/.../[filename]".
 *    Empty [dirname]s and [filename]s (i.e. '//') are not permitted.
 *  - For ICE_FsStatFile() only, the root directory may be specified using
 *    "[drive_number],[partition]:/"
 *  - Implementations of the ICE_Fs functions may enforce restrictions on
 *    file specifications.
 *  - File specifications are case sensitive.
 *  - [drive_number]s run from zero through to the number of drives in the system minus one.
 *  - [partition] identifiers run consecutively from 'a' to 'z'.
 *  - File specification example: "0,b:/streaming/dvr_0001-0002.rec"
 *  - If the Receiver itself (below the ICE API) requires disk space for its own use
 *    then the functions for formatting, available disk space, listing files, etc.
 *    should be implemented in such a way to make this storage invisible &
 *    inaccessable to Eclipse.
 *
 * @note The filesystem must support the opening of two file handles
 * on the same file (typically one would be used for reading and
 * one for writing).
 *
 *<br>
 *<b> Disk Partitions </b>
 *<br>
 * The API supports the partitioning of a physical drive into logical
 * partitions.  Typically this is used to support different block size
 * formatting for streaming large volumes of data. The Partition Drive
 * function provides a 'standard' or 'large' block size hint.
 *
 * - A drive must be partitioned at formatting time. A drive containing a
 *   formatted partition cannot be subsequently re-partitioned without
 *   reformatting of the whole hard disk.
 * - A single logical partition cannot extend across physical drives.
 *   I.e. there is no linkage between partitions with the same identifier on
 *   different drives.
 * - If the ICE implementation does not support partitioning then this should
 *   be emulated, e.g. by creating "/partition_a", "/partition_b", etc. base
 *   directories.
 */

#ifndef CABOT_ICE_ICE_FILESYSTEM_H_INCLUDED
#define CABOT_ICE_ICE_FILESYSTEM_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "config/include_dlna_support.h"
#include "config/include_nflc_21.h"
#include "config/include_nflc_23.h"
#include "frost/frost_basictypes.h"

/**
 * Enum of possible file system error codes
 */
typedef enum ICE_FsError
{
    ICE_NoError = 0,        /**< Operation completed successfully. */
    ICE_FileNotFound,       /**< Filespec does not exist. */
    ICE_OpenError,          /**< Error attempting to open or create a file. */
    ICE_EndOfData,          /**< End of data was reached.  This is end of file
                                 if reading in a forward direction, or start
                                 of file if reading in a reverse direction */
    ICE_ReadError,          /**< Error occurred reading data from the disk. */
    ICE_WriteError,         /**< Error occurred writing data to the disk. */
    ICE_MemoryAllocFail,    /**< Unable to allocate memory within the implementation of a function. */
    ICE_BadParameter,       /**< An invalid parameter was passed to the function. */
    ICE_DiskFull,           /**< The relevant disk is full. */
    ICE_DiskMissing,        /**< The drive's removeable disk is not present -- for future support. */
    ICE_DirNotEmpty,        /**< Attempt was made to delete a directory that is not empty. */
    ICE_DirPathNotFound,    /**< Attempt was made to create a directory with a filespec that
                            contained other directories that do not exist. */
    ICE_DirAlreadyExists,   /**< Attempt was made to create a directory that already exists. */
    ICE_OperationFail       /**< An unspecified error occurred. */

} ICE_FsError;

/**
 * @brief Initialise the ICE file system.
 *
 * This function must be called prior to any calls to any other ICE Filesystem functions.
 * If called again the call is silently ignored.
 *
 * @param max_partitions_per_drive Maximum number of permitted partitions on any drive.
 * @note This API supports 26 but particular product requirements may be much less.
 *       For example Cabot's DVR product only requires two.
 * @return frost_true if initialisation is successful or if the Filesystem is already initialised,
 * else frost_false if initialisation failed.
 */
frost_bool ICE_FileSystemInit(frost_uint8 max_partitions_per_drive);

/**
 * @brief Tests whether the ICE file system has been successfully initialised.
 *
 * @return frost_true if file system has been successfully initialisation,
 * else frost_false if initialisation failed or if ICE_FileSystemInit() has not yet been called.
 */
frost_bool ICE_FsIsInitialised(void);

/**
 * @brief Checks and repairs a physical drive file system.
 *
 * This function verifies the consistency of a file system and tries to
 * repair it if needed.
 *
 * @param driver_number Specifies the physical drive to check and repair.
 *
 * @return true if file system is successfully repaired, false otherwise.
 */
frost_bool ICE_FsCheckRepair(frost_uint8 driver_number);

/**
* === DISK DRIVE OPERATIONS ===
*/
/**
 * Maximum length of a disk information string, excluding the trailing NUL.
 * @see ICE_FsPartitionStatBuffer::display_info
 */
#define ICE_FS_MAX_DISKINFO_LENGTH 31

/**
 * Enum of the possible block size hints
 *
 * This is a hint only, the ICE implementation is allowed to ignore it.
 */
typedef enum ICE_FsDiskBlockSize
{
    /** Used for 'standard' files, e.g. settings and metadata. */
    ICE_StandardBlockSize,
    /** Typically used for streaming large volume data, e.g. DVR recordings.
     */
    ICE_LargeBlockSize
} ICE_FsDiskBlockSize;

/**
 * Structure to receive a disk's partition statistics.
 */
typedef struct ICE_FsPartitionStatBuffer
{
    /** Partition identifier in the range 'a' to 'z'. */
    char partition;

    /**
     * Size in kilobytes of the partition.
     *
     * When formatting, one (and one only) partition may be specified with a
     * size of zero.  This indicates this partition will receive all the
     * remaining disk space once the other partitions have been allocated.
     */
    frost_uint32 size_kb;

    /** The block size hint passed to ICE_FsFormatDisk(). */
    ICE_FsDiskBlockSize block_size_hint;

    /** NOT USED */
    frost_bool removed;

    /**
     * A null-terminated string suitable for display to the user, with disk
     * information.
     *
     * This is platform-dependent, it may for e.g. give drive model and serial
     * number.  If no information is available, this can be a zero length
     * string.
     */
    char display_info[ICE_FS_MAX_DISKINFO_LENGTH +1];
} ICE_FsPartitionStatBuffer;


/**
 * @brief Retrieves whether a format is needed.
 *
 * @return frost_true if a format is needed,
 * else frost_false.
 */
frost_bool ICE_FsIsFormatNeeded(void);

/**
 * @brief Retrieves whether usb is inserted.
 *
 * @return frost_true if usb is available,
 * else frost_false.
 */
frost_bool ICE_FsIsUSBAvailable(void);

/**
 * @brief Get the number of non-removable physical drives.
 *
 * This returns the number of hard disks in the system, usually 1.
 *
 * The non-removable drives must have drive numbers from 0 to
 * (ICE_FsNumberOfPhysicalDrives() - 1) inclusive.
 *
 * @return Count of non-removable drives on the system.
 */
frost_uint8 ICE_FsNumberOfPhysicalDrives(void);

/**
 * @brief Retrieve the size in kilobytes of a physical drive.
 *
 * @param drive_number Specifies the physical drive to query.
 * @param drive_size_kb Receives the size of the drive in Kb.
 * @return ICE_NoError on success,
 *         else error code indicating the failure.
 *
 * @note It is not specified whether this returns a value in
 *       SI kB (=1000 bytes) or SI KiB (=1024 bytes).
 * @note Drives larger than 4Tb are not supported.
 */
ICE_FsError ICE_FsDriveSizeKb(frost_uint8 drive_number,
                              frost_uint32 *drive_size_kb);

/**
 * @brief Partitions and formats a physical drive.
 *
 * This function prepares a physical disk for use, any existing files and
 * directories are destroyed.
 *
 * See <b> Drive Partitions </b> comments for details.
 *
 * @param drive_number Specifies the physical drive to change.
 * @param number_of_partitions The number of partitions the physical drive is
 *                             to be divided into.  This will not exceed the
 *                             max_partitions_per_drive parameter passed to
 *                             ICE_FileSytemInit().
 * @param partition_stat_array Pointer to an array of length
 *                             number_of_partitions that contains the
 *                             partition identifier, size and block size hint.
 *                             The fields 'removed' & 'display_info' are
 *                             unused and undefined.  The 'partition' field
 *                             must be 'a' for the first partition, 'b' for
 *                             the second, etc.
 * @return ICE_NoError on success,
 *         else error code indicating the failure.
 */
ICE_FsError ICE_FsFormatDrive(
                  frost_uint8 drive_number, frost_uint8 number_of_partitions,
                  const ICE_FsPartitionStatBuffer *partition_stat_array);

/**
 * @brief Retrieves the number of a partitions on a physical drive.
 *
 * @param drive_number Specifies the physical drive.
 * @return Number of partitions on success,
 *         else zero if the drive is unformatted or is inaccessable.
 */
frost_uint8 ICE_FsNumberOfPartitions(frost_uint8 drive_number);

/**
 * @brief Retrieves statistics of a drive partition.
 *
 * @param drive_number          Specifies the drive.
 * @param partition             Specifies the partition on the drive.
 * @param partition_stat_buffer Pointer to a structure to receive the statistics.
 * @return ICE_NoError on success,
 *         else error code indicating the failure.
 */
ICE_FsError ICE_FsPartitionStatistics(
                  frost_uint8 drive_number, char partition,
                  ICE_FsPartitionStatBuffer *partition_stat_buffer);

/**
 * @brief Retrieves the free space remaining on a partition in kilobytes.
 *
 * @param drive_number  Specifies the drive containing the disk.
 * @param partition     Specifies the partition on the drive.
 * @param free_space_kb Pointer to a variable to receive the free space value.
 * @return ICE_NoError on success,
 *         else error code indicating the failure.
 */
ICE_FsError ICE_FsPartitionFreeSpaceKb(frost_uint8 drive_number,
                                       char partition,
                                       frost_uint32* free_space_kb);


/* === DISK DIRECTORY OPERATIONS === */

/**
 * Maximum length of an individual file or directory name.
 */
#define ICE_FS_MAX_FILENAME_LENGTH 700

/**
 * Maximum length of a full file specification including
 * drive number, partition, pathname & filename.
 */
#define ICE_FS_MAX_FILESPEC_LENGTH 1024

/**
 * Maximum length of an individual file url
 * This is equal to ICE_FS_MAX_FILESPEC_LENGTH because url is set as spec directly in some cases
 */
#define ICE_FS_MAX_URL_LENGTH 1000

/**
 * @brief Creates a directory.
 *
 * Only one new directory is created per call so all previous directories on
 * the specified directory path must already exist.
 *
 * Reasons this may fail include:
 * - ICE_DirPathNotFound if a previous directory on the path does not exist
 * - ICE_DirAlreadyExists if the directory already exists
 * - ICE_DiskFull if the disk is full
 *
 * @param dirspec An absolute drive, partition, pathname and directory
 *                specifier for the new directory.
 * @return ICE_NoError on success, else an error code indicating the failure.
 *
 * @note The new directory can only portably contain 7-bit ASCII characters
 *       (in the range 32 to 126 inclusive - that is U+0020 to U+007E
 *       inclusive) that are also supported by the filesystem.  There is no
 *       portable ICE API to create directories containing other Unicode
 *       characters.
 */
ICE_FsError ICE_FsCreateDirectory(char const* dirspec);
ICE_FsError ICE_FsCreateDirectoryWithPermissions(char const* dirspec, frost_uint32 permissions);

/**
 * @brief Deletes a directory.
 *
 * The specified directory is deleted. The directory must be empty and must
 * not be the root directory.
 *
 * Reasons this may fail include:
 * - ICE_FileNotFound if the directory does not exist
 * - ICE_DirNotEmpty if any files or sub-directories exist in the directory
 *
 * @param dirspec An absolute drive, partition, pathname and directory
 *                specifier of the directory.
 * @return ICE_NoError on success, else error code indicating the failure.
 */
ICE_FsError ICE_FsDeleteDirectory(char const* dirspec);


/* === FILE MANAGEMENT OPERATIONS === */

/**
 * Enum of possible item types referenced by a filespec
 */
typedef enum ICE_FsFilespecType
{
    ICE_Directory,           /**< Is a directory */
    ICE_File                 /**< Is a linear data file */
} ICE_FsFilespecType;

/**
 * Structure to receive a file's statistics.
 */
typedef struct ICE_FsFileStatBuffer
{
    /** Type of the referenced item. */
    ICE_FsFilespecType type;

    /** Size of the file in bytes. */
    frost_uint32       file_size;

    /** The creation time of the file - in UCT.*/
    frost_uint32    creation_time;

    /** Name of the file or directory (only, i.e. without the full path). */
    char filename[ICE_FS_MAX_FILENAME_LENGTH+1];

    /** Name encoded in UTF-8. Some platforms do not provide full Unicode
     * support but offer special methods for obtaining Unicode-encoded file
     * and directory names.  (We only support UTF-8 encoding of 16-bit BMP
     * characters, which could be up to three bytes long.)
     */
    char filename_utf8[3 * ICE_FS_MAX_FILENAME_LENGTH + 1];
    /** Time of last access to this file
    */
    frost_uint32 last_access_time;
    /** Time of last modification made to this file
    */
    frost_uint32 last_modify_time;

    /**
     * Index of object in container.
     * (E.g. file or folder index in containing directory or object index in dlna container)
     * Container may be a playlist or filter result thus it avoids duplicates and slash problems in titles.
     */
    void * object;

} ICE_FsFileStatBuffer;

/**
 * @brief Retrieve statistics about a file or directory.
 *
 * @param filespec An absolute drive, partition, pathname and filename
 *                 specifier for the file or directory.  The root directory
 *                 may be specified, e.g. "1,a:/"
 * @param stat_buffer Pointer to a buffer to receive the retrieved statistics.
 * @return ICE_NoError on success,
 *         else ICE_FileNotFound if the file does not exist,
 *         else error code indicating the failure.
 */
ICE_FsError ICE_FsStatFile(char const* filespec,
                           ICE_FsFileStatBuffer* stat_buffer);

/**
 * @brief Retrieve statistics about a file or directory.
 *
 * @param filepath An absolute pathname and filename

 * @param stat_buffer Pointer to a buffer to receive the retrieved statistics.
 * @return ICE_NoError on success,
 *         else ICE_FileNotFound if the file does not exist,
 *         else error code indicating the failure.
 */
ICE_FsError ICE_FsStatAbsoluteNameFile(char const* filepath,
                           ICE_FsFileStatBuffer* stat_buffer);

/**
 * ICE_FsFindHandle abstracts a handle to a find session.
 *
 * This is an opaque type, to be manipulated only by the functions specified
 * in this header file.  Its implementation is specific to the target
 * platform, though this interface is portable.
 */
typedef struct ICE_FsFindHandle ICE_FsFindHandle;

/**
 * @brief Find the first file matching a filespec.
 *
 * The filename will use a wild-card '*' to match any portion of the
 * name, e.g:
 * - wib* matches all files & directories commencing with "wib",
 * - *.txt matches all files & directories ending in ".txt",
 * - wib*.txt matches all files & directories commencing with "wib" & ending
 *   in ".txt",
 *
 * Wildcards are not permitted before the final '/' within filespec.
 * (However, on some systems - including Linux - '*' is a valid character in
 * a directory name.  So '*' characters before the final '/' have to be
 * interpreted as literal '*' characters in the directory name, not wildcards.
 * '*' characters after the final '/' are wildcards).
 *
 * Multiple wildcards are not allowed in the filename, there will be at most
 * one wildcard.
 *
 * If a matching file is created or deleted after the ICE_FsFindFirstFile()
 * call, then:
 * - whether a subsequent call to ICE_FsFindNextFile() returns an entry
 *   for that file is unspecified.
 * - files that existed before the ICE_FsFindFirstFile() call (and have not
 *   been deleted) must not be skipped.  (Note that MacOS readdir() does NOT
 *   provide this guarantee - see
 *   http://support.apple.com/kb/TA21420?viewlocale=en_US).
 * (One use of this function is to iterate over a directory whilst deleting
 * some of the found files; the specification above is intended to make sure
 * that works).
 *
 * @param filespec An absolute drive, partition, and file/directory specifier.
 * @param stat_buffer Pointer to a buffer to receive the retrieved statistics.
 * @param find_handle_ptr A pointer to a pointer to receive the ICE_FsFindHandle,
 *                        set to NULL on any error.
 * @return ICE_NoError on success,
 *         else ICE_FileNotFound if no matching file exists (stat_buffer is
 *         undefined in this case),
 *         else error code indicating the failure.
 */
ICE_FsError ICE_FsFindFirstFile(char const* filespec,
                                ICE_FsFileStatBuffer* stat_buffer,
                                ICE_FsFindHandle** find_handle_ptr);

/**
 * @brief Find the next file matching a filespec.
 *
 * Retrieves details of the next file matching a find previously opened with
 * ICE_FsFindFirstFile.
 *
 * @param handle A handle returned from a call to ICE_FsFindFirstFile.
 * @param stat_buffer Pointer to a buffer to receive the retrieved statistics.
 * @return ICE_NoError on success & stat_buffer is filled,
 *         else ICE_FileNotFound if there are no more matching files to
 *         retrieve (stat_buffer is undefined),
 *         else error code indicating the failure.
 */
ICE_FsError ICE_FsFindNextFile(ICE_FsFindHandle* handle,
                               ICE_FsFileStatBuffer* stat_buffer);

/**
 * @brief Close a Find File session.
 *
 * Releases all resources for the Find File session.
 * After a call to this function the ICE_FsFindHandle is no longer valid and
 * must not be used.
 *
 * @param handle A handle returned from a call to ICE_FsFindFirstFile.
 * @return ICE_NoError on success,
 *         else error code indicating the failure.
 */
ICE_FsError ICE_FsFindClose(ICE_FsFindHandle* handle);


/**
 * @brief Delete an existing file.
 *
 * @param filespec An absolute drive, partition, pathname and filename
 *                 specifier for the file.
 * @return ICE_NoError on success,
 *         else ICE_FileNotFound if the filespec does not exist,
 *         else error code indicating the failure.
 */
ICE_FsError ICE_FsDeleteFile(char const* filespec);



/**
 * @brief Renames an existing file or directory.
 *
 * This function supports moving a file to a different directory, partition,
 * or disk.
 *
 * @param old_filespec An absolute drive, partition, pathname and name of the
 *                     old file.
 * @param new_filespec An absolute drive, partition, pathname and name of the
 *                     new filename.
 * @return ICE_NoError on success,
 *         else ICE_FileNotFound if the filespec does not exist,
 *         else error code indicating the failure.
 *
 * @note The new filename can only portably contain 7-bit ASCII characters
 *       (in the range 32 to 126 inclusive - that is U+0020 to U+007E
 *       inclusive) that are also supported by the filesystem.  There is no
 *       portable ICE API to rename files where the new name contains other
 *       Unicode characters.
 */
ICE_FsError ICE_RenameFile(char const *old_filespec, char const *new_filespec);


/* === FILE DATA OPERATIONS === */

/**
 * Enum of possible open & creation modes
 */
typedef enum ICE_FsOpenMode
{
    /**
     * Open existing file for reading only
     * The position is initially set to the start of the file.
     */
    ICE_ReadOnly,

    /**
     * Open existing file for writing only.
     * The position is initially set to the end of the file.
     *
     * @todo The Win32 implementation doesn't do that!!!!
     */
    ICE_WriteOnly,

    /**
     * Open existing file for reading & writing.
     * The position is initially set to the end of the file.
     */
    ICE_ReadWrite,

    /**
     * Creates a new file for reading & writing, if file exists then destroy
     * its contents.
     */
    ICE_CreateFile,

    /**
     * Opens for reading & writing, if file doesn't exist then create it.
     * The position is initially set to the end of the file.
     */
    ICE_AppendFile
} ICE_FsOpenMode;


/**
 * ICE_FsHandle abstracts a handle to an open file
 *
 * This is an opaque type, to be manipulated only by the functions specified
 * in this header file.  Its implementation is specific to the target
 * platform, though this interface is portable.
 */
typedef struct ICE_FsHandle ICE_FsHandle;

/**
 * @brief Create a new file or open an existing file.
 *
 * @param filespec An absolute drive, partition, pathname and filename
 *                 specifier for the file.
 * @param mode The open/creation mode
 * @param file_handle_ptr A pointer to a pointer to receive the ICE_FsHandle,
 *                        set to NULL on any error.
 * @return      ICE_NoError on success,
 *              else ICE_OpenError if the filespec does not exist (for open
 *               existing file modes) or if the creation of a new file fails,
 *              else error code indicating the failure.
 *
 * @note When creating a new file, the new filename can only portably contain
 *       7-bit ASCII characters (in the range 32 to 126 inclusive - that is
 *       U+0020 to U+007E inclusive) that are also supported by the filesystem.
 *       There is no portable ICE API to create files containing other Unicode
 *       characters.
 */
ICE_FsError ICE_FsOpenFile(char const *filespec, ICE_FsOpenMode mode,
                           ICE_FsHandle** file_handle_ptr);

ICE_FsError ICE_FsOpenFileWithPermissions(char const *filespec, ICE_FsOpenMode mode,
                           frost_uint32 permissions, ICE_FsHandle** file_handle_ptr);


ICE_FsError ICE_FsCopyFileToMemory(unsigned char* buffer, frost_uint32 bytes_to_read,
                char const *filespec, frost_uint32* bytes_read);

ICE_FsError ICE_FsCopyMemoryToFile(unsigned char* buffer, frost_uint32 bytes_to_write,
                char const *filespec, frost_uint32* bytes_written);

/**
 * @brief Create a new file or open an existing file.
 *
 * @param filepath An absolute pathname and filename
 * @param mode The open/creation mode
 * @param file_handle_ptr A pointer to a pointer to receive the ICE_FsHandle,
 *                        set to NULL on any error.
 * @return      ICE_NoError on success,
 *              else ICE_OpenError if the filespec does not exist (for open
 *               existing file modes) or if the creation of a new file fails,
 *              else error code indicating the failure.
 *
 * @note When creating a new file, the new filename can only portably contain
 *       7-bit ASCII characters (in the range 32 to 126 inclusive - that is
 *       U+0020 to U+007E inclusive) that are also supported by the filesystem.
 *       There is no portable ICE API to create files containing other Unicode
 *       characters.
 */
ICE_FsError ICE_FsOpenAbsoluteNameFile(char const *filepath, ICE_FsOpenMode mode,
                           ICE_FsHandle** file_handle_ptr);
   
#ifdef INCLUDE_DLNA_SUPPORT                        
/**
 * @brief Enum of possible file type codes
 */
typedef enum ICE_FsFileType
{
    ICE_FilePicture,
    ICE_FileAudio,
    ICE_FileVideo,
    ICE_FileSubtitle,
    ICE_FileUnknown
} ICE_FsFileType;

ICE_FsFileType ICE_FsGetDlnaFileType(char const *name);
char const * ICE_FsDlnaGetDownloadedFile();
ICE_FsError ICE_FsDlnaDownloadFile(char const *spec, void * object);
ICE_FsError ICE_FSDlnaRemoveDownloadedFile(char const *spec, void * object);
frost_bool ICE_FsDlnaCompareFiles(void * object1, void * object2 );
#endif

/**
 * @brief Close an open file handle.
 *
 * Flushes any unwritten buffered data to disk, closes the file and releases
 * any system resources.
 *
 * After a call to this function the ICE_FsHandle is no longer valid and
 * must not be used.  (Even if this function fails, e.g. if the flush fails,
 * the handle is still closed).
 *
 * @note If the ICE implementation is buffering ICE_FsWrite requests, then
 *       any write errors may not be detected until the next ICE_FsFlush()
 *       or ICE_FsClose() call on that file.
 *
 * @param handle A handle returned from ICE_FsOpenFile.  May be NULL, in
 *               which case this function does nothing, successfully.
 * @return ICE_NoError on success, else error code indicating the failure.
 */
ICE_FsError ICE_FsCloseFile(ICE_FsHandle* handle);

/**
 * @brief Read data from an open file.
 *
 * If the read goes beyond the physical end-of-file then the size_read will
 * indicate the bytes read & ICE_EndOfData will be returned.
 *
 * @param handle A handle returned from ICE_FsOpenFile.
 * @param buffer A buffer to receive the bytes read, it must be at least
 *               size_to_read bytes long.
 * @param bytes_to_read Number of bytes to read.
 * @param bytes_read Pointer to a variable to receive the number of bytes
 *                   actually read.
 * @return ICE_NoError on success,
 *         ICE_EndOfData on reaching the end of the file,
 *         else error code indicating the failure.
 */
ICE_FsError ICE_FsRead(ICE_FsHandle* handle, unsigned char* buffer,
                        frost_uint32 bytes_to_read, frost_uint32* bytes_read);

/**
 * @brief Write data to an open file.
 *
 * If writing at the end of the file, then the file size will be increased as
 * the data is written.
 *
 * @note Optionally, the ICE implementation may buffer ICE_FsWrite requests.
 *       They may only get written to disk when ICE_FsFlush() or ICE_FsClose()
 *       is called on the file handle.
 *
 * @note If the ICE implementation is buffering ICE_FsWrite requests, then
 *       any write errors may not be detected until the next ICE_FsFlush()
 *       or ICE_FsClose() call on that file.  Additionally, the file size
 *       reported via ICE_FsStatFile() or ICE_FsFindFirstFile() might not be
 *       updated until the file is closed.
 *
 * @param handle A handle returned from ICE_FsOpenFile().  Will be non-NULL.
 * @param buffer A buffer containing the bytes to write.  Will be non-NULL.
 * @param bytes_to_write Number of bytes to write.  Will be nonzero.
 * @param bytes_written Pointer to a variable to receive the number of bytes
 *                      actually written.  Note that if the return value is
 *                      ICE_NoError, then this is guaranteed to be the same
 *                      as bytes_to_write.  In the case of an error, then this
 *                      will be smaller and reflects the number of bytes that
 *                      were successfully written before the error occured.
 *                      This pointer will be non-NULL.
 * @param logging The Cabot trace system can be used to log trace to a disk
 *                file, this flag indicates whether this call is logging
 *                output. If this is frost_true then the implementation code
 *                itself must NOT make calls to Cabot trace, otherwise there
 *                will be an infinite recursion.
 * @return ICE_NoError on success,
 *         ICE_DiskFull if there is insufficien disk space,
 *         else another error code indicating the failure.
 */
ICE_FsError ICE_FsWrite(ICE_FsHandle* handle, unsigned char const* buffer,
                        frost_uint32 bytes_to_write, frost_uint32* bytes_written,
                        frost_bool logging);

/**
 * @brief Flush any buffered data to an open file.
 *
 * Forces any buffered unwritten data to be committed to disk.
 *
 * @note If the ICE implementation is buffering ICE_FsWrite requests, then
 *       any write errors may not be detected until the next ICE_FsFlush()
 *       or ICE_FsClose() call on that file.
 *
 * @param handle A handle returned from ICE_FsOpenFile.
 * @param logging The Cabot trace system can be used to log trace to a disk
 *                file, this flag indicates whether this call is logging
 *                output. If this is frost_true then the implementation code
 *                itself must NOT make calls to Cabot trace, otherwise there
 *                will be an infinite recursion.
 * @return ICE_NoError on success,
 *         else error code indicating the failure.
 */
ICE_FsError ICE_FsFlush(ICE_FsHandle* handle, frost_bool logging);


/**
 * Enum of possible seek modes
 */
typedef enum ICE_FsSeekMode
{
    /** Seek relative to the beginning of the file. */
    ICE_SeekFromBegin,

    /** Seek relative to the end of the file. */
    ICE_SeekFromEnd,

    /** Seek relative to the current file pointer position. */
    ICE_SeekFromCurrent
} ICE_FsSeekMode;

/**
 * @brief Seek to a position in a file.
 *
 * Subsequent read and write operations will commence at the new file position.
 *
 * The offset is a signed quantity.  For ICE_SeekFromBegin the offset must be
 * positive or zero.  For ICE_SeekFromEnd the offset must be negative or zero.
 * For ICE_SeekFromCurrent the offset may be positive or negative.
 *
 * Eclipse should never try to seek past the end of the file or before the
 * start of the file.  The result of trying to do that is platform-dependent;
 * ideally such a seek call would fail.
 *
 * @note Offset is split into two 32-bit values to aid portability - not all
 *       platforms support 64-bit integral types.
 *
 * @param handle A handle returned from ICE_FsOpenFile().
 * @param offset_high High 32 bits of number of offset bytes to seek to.
 *                    Note that the offset is a signed 64-bit quantity.
 * @param offset_low Low 32 bits of number of offset bytes to seek to.
 * @param mode The seek origin mode.
 * @return ICE_NoError on success,
 *         else error code indicating the failure.
 */
ICE_FsError ICE_FsSeek(ICE_FsHandle* handle, frost_uint32 offset_high,
                       frost_uint32 offset_low, ICE_FsSeekMode mode);


frost_int ICE_FsGetC(ICE_FsHandle* handle);

ICE_FsError ICE_FsPutC(ICE_FsHandle* handle, unsigned char* buffer);

void ICE_FsConvertFileSpecToPlatformPath(char const* filespec, char* platform_path);

/**
 * @brief Retrieve the file pointer's current absolute position in a file.
 *
 * Subsequent read & write operations will commence at this position.
 *
 * @note Position is split into two 32-bit values to aid portability - not
 *       all platforms support 64-bit integral types.
 *
 * @param handle A handle returned from ICE_FsOpenFile().
 * @param position_high Pointer to a variable to receive the high 32-bits of
 *                      the position.
 * @param position_low Pointer to a variable to receive the low 32-bits of
 *                     the position.
 * @return ICE_NoError on success,
 *         else error code indicating the failure.
 */
ICE_FsError ICE_FsReadTell(ICE_FsHandle* handle, frost_uint32* position_high,
                           frost_uint32* position_low);

/**
 * @brief Unused function.
 *
 * @note Eclipse never calls this function.  You should not even implement it.
 *       ICE_FsReadTell() provides the same functionality.
 */
ICE_FsError ICE_FsWriteTell(ICE_FsHandle* handle, frost_uint32* position_high,
                            frost_uint32* position_low);


/**
 * @brief This function is used to convert a native filespec to platform specific
 *         path string. This is needed when passing path information
 *         to third party components which access the file system.
 *
 * @param filespec An absolute drive, partition, pathname and filename
 *                 specifier for the file or directory.  The root directory
 *                 may be specified, e.g. "1,a:/"
 *
 * @param platform_path Pointer to a buffer to receive the platform path.
 *                      Buffer length should be ICE_FS_MAX_FILESPEC_LENGTH
 *
 * @return ICE_NoError on success,
 *         else error code indicating the failure.
 *
 */
ICE_FsError ICE_FsFilespecToPlatformPath(char const *filespec, char *platform_path);

/**
 * @brief Change the size of an opened file.
 *
 *
 * @note New size is split into two 32-bit values to aid portability - not
 *       all platforms support 64-bit integral types.
 *
 * @param handle A handle returned from ICE_FsOpenFile().
 * @param new_size_high Pointer to a variable to receive the high 32-bits of
 *                      the new file size.
 * @param new_size_low Pointer to a variable to receive the low 32-bits of
 *                     the new file size.
 * @return ICE_NoError on success,
 *         else error code indicating the failure.
 */
ICE_FsError ICE_FsChangeSize(ICE_FsHandle* handle, frost_uint32 new_size_high, frost_uint32 new_size_low);

/**
 * @brief Checks users permissions for a file
 *
 * @param filespec An absolute drive, partition, pathname and filename
 *                 specifier for the file or directory.  The root directory
 *                 may be specified, e.g. "1,a:/"
 * @param access_mode is a mask contining access mode flags to check.
 * @return ICE_NoError on success,
 *         else ICE_FileNotFound if the file does not exist,
 *         else error code indicating the failure.
 */
ICE_FsError ICE_FsCheckAccess(char const* filespec, frost_uint32 access_mode);

/**
 * @brief Gets the number of bytes available for the given file for reading and
 * stores the result into the value pointed to by the second argument.
 * @param handle File descriptor.
 * @param result Pointer to a variable to store the result. Cannot be null.
 * @return ICE_NoError if the call is successful, ICE_OperationFail if an error has occurred.
 */
ICE_FsError ICE_FsAvailable(ICE_FsHandle* handle, frost_uint32 * result);

#if defined (INCLUDE_NFLC_21) || defined (INCLUDE_NFLC_23)
ICE_FsError ICE_FsFindDLNASubtitle(char const* filespec,
                                ICE_FsFileStatBuffer* stat_buffer,
                                ICE_FsFindHandle** find_handle_ptr);

ICE_FsError ICE_DlnaDownloadSubtitleFile(frost_int8* uri, frost_int8* file_name);
#endif

#ifdef INCLUDE_DLNA_SUPPORT


ICE_FsError ICE_FsDLNABufferAlbumArt(char const* filespec,
                                char const* file_name,
                                char const* download_path,
                                void * object);

ICE_FsError ICE_DlnaDownloadAlbumArt(frost_int8* uri, frost_int8* file_name);

frost_bool ICE_FsDLNAFileCheck(char const* filespec);

#endif

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_FILESYSTEM_H_INCLUDED*/
