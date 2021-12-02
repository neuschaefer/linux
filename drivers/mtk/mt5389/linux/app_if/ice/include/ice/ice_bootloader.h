/**
 * Copyright 2004 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief Cabot portable interface for implementing a Boot Loader.
 *
 * The Boot Loader performs the basic role of validating an image, then copying
 * the image from Flash to RAM (if necessary) and then running it. The Flash
 * memory will contain two placeholders (slots) for Cabot Eclipse code images,
 * both of a given size. Only the software from one of them can be running at
 * any given time. See #ICE_BootLoader_RunImage for more details.
 *
 * During the Over-Air-Download (OAD) update (or update via any other means
 * e.g. serial), the new software image is placed in the non-currently running
 * slot, and picked up as operational after the next restart.
 * In the case of problems the Set Top Box (STB) can use a previous version of
 * the software reverting to a known working state.
 *
 * The Boot Loader can be extended to support one or more of the following
 * features:
 *  - debug output via serial port showing load status and error reports
 *  - supporting compressed images using the ZLIB package
 *  - downloading via serial port
 *  - selecting an old image via serial port or remote control magic key codes
 *
 * The Boot Loader interface requires knowledge of the Flash Memory
 * configuration via the following external variables:
 *
 * @li #ice_flash_base_address
 * @li #ice_flash_size
 * @li #ice_flash_slota_base
 * @li #ice_flash_slota_size
 * @li #ice_flash_slotb_base
 * @li #ice_flash_slotb_size
 *
 * Please consult the documentation to specify the correct values for the
 * target hardware. The preferred method is to set these globals to actual
 * linker symbols to minimise the chance of errors.
 *
 * @htmlonly
 * The diagram below shows a typical Flash memory map. The Boot Loader
 * will determine the address of each of these locations.
 * @endhtmlonly
 *
 * @image   html flash_memory_map.png "Flash Memory Map"
 *
 * @note The code responsible for driving the OAD is itself contained in the
 * OAD image and not in the Boot Loader (this allows the OAD mechanism to be
 * updated).
 *
 * @note  This header file, as with frost_basictypes.h, is currently only
 * portable to platforms where CHAR_BIT==8, sizeof(short)==2, sizeof(int)==4
 * (and there are no pad bits in a short or int).
 */

#ifndef CABOT_ICE_ICE_BOOTLOADER_H_INCLUDED
#define CABOT_ICE_ICE_BOOTLOADER_H_INCLUDED

#include "frost/frost_basictypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * A version number that should be updated when any of the structures are expanded.
 * @note This version number should be stored in the version field of
 * #ICE_BootLoader_Info. See #ice_bootloader_global_version
 */
#define BOOTLOADER_VERSION  0x0001

/**
 * @brief BootLoader errors
 */
typedef enum ICE_BootLoader_Error {
    ICE_bootloader_error_noerror = 0,   /**< - No error */
    ICE_bootloader_error_wait,          /**< - Please wait */
    ICE_bootloader_error_critical,      /**< - Signalled when control flow reaches never-here place */
    ICE_bootloader_error_no_code,       /**< - No suitable software present */
    ICE_bootloader_error_image,         /**< - Image error */
    ICE_bootloader_error_decompression, /**< - Image decompression error */
    ICE_bootloader_error_flashwrite     /**< - Flash write error*/
} ICE_BootLoader_Error;

/**
 * @brief BootLoader table entry
 */
typedef struct ICE_BootLoader_TableEntry
{
    frost_ulong size;                     /**< - Size of uncompressed data. ZERO signals end of table */
    frost_ulong destination_address;      /**< - Address of destination in RAM */
    frost_ulong compressed_size;          /**< - Size of compressed data. ZERO means not compressed */
    frost_ulong data_offset;              /**< - Offset to the data segment within image */
} ICE_BootLoader_TableEntry;

/**
 * @brief BootLoader image header
 */
typedef struct ICE_BootLoader_OADHeader
{
#ifdef SERIAL_DOWNLOAD_SUPPORT
    frost_ulong   burn_address;   /**< - Serial port loader: destination address */
    frost_ulong   disable_other;  /**< - Serial port loader: disable other image */
    frost_ulong   raw_data;       /**< - Serial port loader: Serial port loader: not an image - raw write starting from 1st byte after this header */
#else
    frost_ulong   reserved[3];    /**< - Reserved */
#endif
    frost_ulong   status;         /**< - Status of this code image. The least significant bit is set/cleared to mark an image as valid or invalid respectively */
#ifdef SERIAL_DOWNLOAD_SUPPORT
    frost_ulong   burn_noerase;   /**< - Serial port loader: do not erase flash content prior to writing */
#else
    frost_ulong   reserved2;      /**< - Reserved */
#endif
    frost_ulong   spare[7];       /**< - Spare placeholder */
    frost_ulong   crc32;          /**< - CRC checksum (includes all following this field) */
    char          name[24];       /**< - Code name string */
    frost_ulong   OUI;            /**< - Organisationally Unique Identifier (OUI) as value. See http://standards.ieee.org/regauth/oui/index.shtml for more details */
    frost_ulong   model;          /**< - Model number - MUST be 0x0001 to 0xFFFF for System Software Update(SSU) compatibility */
    char          oui[7];         /**< - OUID of manufacturer as string (space for 6 hex characters + nul terminator) */
    char          filename[33];   /**< - Download Control File (DCF) filename of OAD image (relative to OUID base directory) without slashes */
    frost_ulong   length;         /**< - Code image length (entire header plus all data) */
    frost_ulong   version;        /**< - Code image version no. - MUST be 0x0001 to 0xFFFF for SSU compatibility */
    frost_ulong   reserved3[2];   /**< - Reserved */
    frost_ulong   entry;          /**< - Code entry function address */
    frost_ulong   code_table;     /**< - Offset to list of code table entries (ICE_BootLoader_TableEntry) */
    frost_ulong   bss_table;      /**< - Offset to list of bss table entries (ICE_BootLoader_TableEntry) */
    frost_uint8  *boot_data;      /**< - Copy of the original boot data */

} ICE_BootLoader_OADHeader;

/**
 * @brief BootLoader information structure
 * @note Total structure size MUST be a multiple of 4 bytes due to the common
 * limitation when reading/writing to Flash Memory.
 */
typedef struct ICE_BootLoader_Info
{
    frost_ulong spare[7];                       /**<- Other information may be placed here */
    frost_ulong version;                        /**<- This field contains the Boot Loader Version. See #BOOTLOADER_VERSION */
    frost_ulong slot_A_crc_ok;                  /**<- Slot A - CRC is valid if non-zero */
    ICE_BootLoader_OADHeader *slot_A_ptr;       /**<- Pointer to slot A image descriptor */
    frost_ulong slot_B_crc_ok;                  /**<- Slot B - CRC is valid if non-zero */
    ICE_BootLoader_OADHeader *slot_B_ptr;       /**<- Pointer to slot B image descriptor */
    ICE_BootLoader_OADHeader *current_slot_ptr; /**<- Pointer to currently selected image descriptor */
} ICE_BootLoader_Info;

/**
 * Load and run an image from one of the two slots.
 *
 * @par
 * This function should perform the following steps:
 * @li Check if either or both of the slots' are marked as valid. A slot is
 * marked valid if the least significant bit of the status field is set.
 * @li If both slots are marked valid then the Boot Loader will choose the
 * slot which has the latest image version (i.e. the highest value in version field)
 * @li If only one slot is marked valid then the image in this slot will be chosen
 * @li If no slots are marked valid #ICE_bootloader_error_no_code should be reported
 * @li CRC check the chosen image
 * @li Load the image into RAM (if necessary)
 * @li Run the image
 */
void ICE_BootLoader_RunImage(void);

/**
 * Check if the current image should be run.
 *
 * @par
 * This function could be called from within #ICE_BootLoader_RunImage to
 * override the logic which chooses which image to use when both slots are
 * marked as valid.
 * @return #frost_true if current image should be run, otherwise #frost_false
 * to run the other image
 */
frost_bool ICE_BootLoader_UseCurrentImage(void);

/**
 * Displays error code, and takes appropriate action if it is of terminal
 * type.
 * @param error The boot loader error that occurred.
 */
void ICE_BootLoader_ShowError(ICE_BootLoader_Error error);

/**
 * Resets STB
 */
void ICE_BootLoader_Restart(void);

/**
 * Global to access the Boot Loader's information structure
 */
extern ICE_BootLoader_Info* ice_bootloader_global_info;

/**
 * Global to access the Boot Loader's version
 */
extern frost_uint16 ice_bootloader_global_version;

/**
 * Global to access the Flash memory's base address
 */
extern frost_uint ice_flash_base_address;

/**
 * Global to access the Flash memory's size
 */
extern frost_uint ice_flash_size;

/**
 * Global to access the base address of Slot A within the Flash Memory
 */
extern frost_uint ice_flash_slota_base;

/**
 * Global to access the size of Slot A within the Flash Memory
 */
extern frost_uint ice_flash_slota_size;

/**
 * Global to access the base address of Slot B within the Flash Memory
 */
extern frost_uint ice_flash_slotb_base;

/**
 * Global to access the size of Slot B within the Flash Memory
 *
 * @note The size of slot B may be different from slot A when memory is
 *       limited.
 */
extern frost_uint ice_flash_slotb_size;

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_BOOTLOADER_H_INCLUDED*/

/* EOF */

