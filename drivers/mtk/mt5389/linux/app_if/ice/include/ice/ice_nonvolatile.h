/**
 * Copyright 2004 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief The interface to query, erase, read and write non-volatile blocks.
 *
 * The basic model is that there may be a number of non-volatile technologies
 * available on a given device (e.g. set-top box), for example, flash, eeprom,
 * hard disk; and the non-volatile storage on a given device may be used for a
 * number of purposes, for example settings, boot images, OAD keys.
 *
 * A number of blocks of non-volatile storage are allocated (by design, fixed
 * on a per-project basis) to each purpose.  The blocks for a given purpose
 * may be of mixed technologies and mixed block sizes (unless noted
 * otherwise).  Those blocks are addressed by a logical block number that is
 * an unsigned integer, 0 for the first logical block for a given purpose, 1
 * for the next, and so on.  A given logically-numbered block for a given
 * purpose should correspond to the same physical block (and therefore have
 * the same technology, size and contents) across reboots and power cycles.
 *
 * A purpose and a logical block number together identify a physical block;
 * that is, a physical block is addressed by a (purpose, logical block number)
 * pair.  Each physical block must be assigned to at most one (purpose,
 * logical block number).
 *
 * This interface is designed to support two broad classes of client (users of
 * the interface).  Simple clients can ignore the underlying technology
 * supporting the non-volatile storage.  They should regard all blocks as if
 * they require erasing before writing, consider all bytes that have not been
 * written to since erase as being undefined, and regard all bytes as if they
 * can only be written once between block erases.  This corresponds to a
 * simple use of flash.  More advanced clients, which query the underlying
 * technology, can use the devices more powerfully.
 *
 * The implementation of this interface is responsible for thread safety.
 * That is, the implementation must deal correctly with calls to this
 * interface from multiple threads simultaneously.  Simple implementations can
 * meet this requirement by serialising all activity using a mutex, however
 * good implementations ought to allow as much parallelism as the underlying
 * device(s) permit, for efficiency.
 *
 * Implementations should not return until the required operation on the
 * device is completed (and where appropriate, verified). Clients should be
 * aware that some calls may block for significant periods in some
 * circumstances.
 *
 * Flash (technology == #ICE_NonVolatileTechnologyFlash) is assumed to be
 * erasable in blocks, and readable and writable in bytes (but see
 * Implementation note below).  A call to ICE_NonVolatileErase() MUST set ALL
 * bytes in the specified block either to 0xFF or 0x00 (not a mixture) before
 * it returns success. An implementation of ICE_NonVolatileWrite() MUST verify
 * that, after writing, the contents of the specified bytes of flash are
 * bit-for-bit identical to the specified values.
 *
 * Clients that update the values of bytes that are not in the erased state
 * are responsible for ensuring that values specified in
 * ICE_NonVolatileWrite() are achievable by the underlying device.  Clients
 * should note that #ICE_NonVolatileTechnologyFlash is used for both NAND and
 * NOR flash.  To determine which type is in use, clients may call erase then
 * read a byte; 0xFF immediately following a successful erase indicates NAND
 * flash.
 *
 * Hard-disk and EEPROM blocks are assumed not to need erasing.
 *
 * Hard-disk implementations are normally expected to access the raw hard disk
 * device at block level, although an implementation on top of a hard-disk
 * filesystem is possible.
 *
 * @note Implementation note: The only "purpose" that Eclipse
 * currently uses is #ICE_NonVolatilePurposeSettings.
 *
 * @note Implementation note: Eclipse currently (and for the foreseeable
 * future) uses this interface in a restricted way, which permits some
 * potentially significant simplifications in the implementation.
 *   -# All offsets in calls to ICE_NonVolatileRead() and
 *      ICE_NonVolatileWrite() will always be a multiple of 4.
 *   -# All counts in calls to ICE_NonVolatileRead() and
 *      ICE_NonVolatileWrite() will always be a multiple of 4.
 *   .
 * This may dramatically simplify implementations on word-addressable flash
 * devices, for example.  Implementations that make use of this assumption
 * should assert that \a offset and \a count are indeed a multiple of the word
 * size they assume, and MUST report block sizes that are multiples of the
 * word size.
 *
 * @note Implementation note: The implementation should provide access to the
 * raw device(s) without using the device(s) itself (it should not format or
 * put magic numbers into the block, for example).
 */

#ifndef CABOT_ICE_ICE_NONVOLATILE_H_INCLUDED
#define CABOT_ICE_ICE_NONVOLATILE_H_INCLUDED

#include "frost/frost_basictypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief The purpose for which non-volatile storage is used.
 *
 * This enumeration specifies the ways in which non-volatile memory may be
 * used.
 *
 * This type is always passed as a parameter to the methods of the
 * non-volatile interface.  Note that a block number uniquely identifies a
 * block ONLY in conjunction with a purpose.
 *
 * @note Currently Eclipse only uses purpose #ICE_NonVolatilePurposeSettings.
 * Other uses of non-volatile storage, such as boot images, are supported by
 * other interfaces.  The implementation of this interface needs to be done in
 * conjunction with the implementation of the other users of non-volatile
 * storage, to ensure that clients do not conflict when accessing blocks.
 */
typedef enum ICE_NonVolatilePurpose
{
    /**
     * For Eclipse's settings.
     *
     * The flash usage depends on the UI app, but 64k is typical.
     * An additional block will increase robustness, so
     * power loss during a write won't cause loss of settings.
     *
     * Blocks with this purpose may be any size and any technology.
     */
    ICE_NonVolatilePurposeSettings,

    /**
     * For storing sensitive data that must be protected against unauthorised
     * access
     *
     * Currently only required by CI Plus
     *
     * This will typically be used to store an encryption key required by
     * the crypto engine or other cryptographically sensitive data. 256 bytes
     * of data would be a typical minimum size.
     *
     * Blocks stored using this purpose must be protected against copying or
     * unauthorised viewing of the data. This must be implemented such that the
     * protection cannot be circumvented using "Commonly Available Tools" such
     * as screw drivers, jumpers, clips, soldering irons, EEPROM readers and
     * writers etc.
     *
     * The protection should only be defeated or circumvented using
     * professional tools or equipment that are only available on the basis of
     * non-disclosure agreements, such as logic analysers, chip disassembly
     * systems, in-circuit emulators or other tools, equipment, methods or
     * techniques not listed as "Commonly Available Tools"
     *
     */
    ICE_NonVolatilePurposeSecureStorage

} ICE_NonVolatilePurpose;


/**
 * @brief The non-volatile technology used to implement a block.
 *
 * This enumeration specifies the underlying technology of the non-volatile
 * storage.
 *
 * @note In the special case where an implementation uses one technology to
 * simulate another (e.g. hard disk to simulate flash), the implementation is
 * responsible for simulating the required behaviour of the technology fully,
 * including reporting the simulated technology not the actual technology
 * used.  For example a PC-based emulator that actually uses hard disk to
 * simulate flash storage should return #ICE_NonVolatileTechnologyFlash and
 * not #ICE_NonVolatileTechnologyHardDisk.
 */
typedef enum ICE_NonVolatileTechnologyType
{
    ICE_NonVolatileTechnologyNone, /**< Indicates an error  */
    ICE_NonVolatileTechnologyFlash, /**< Flash */
    ICE_NonVolatileTechnologyEEPROM, /**< EEPROM */
    ICE_NonVolatileTechnologyHardDisk /**< Hard disk */
} ICE_NonVolatileTechnologyType;


/**
 * @brief Query the number of blocks available for a given purpose.
 *
 * The implementation may perform appropriate driver initialisation when this
 * call is made, if required.
 *
 * This function may be called more than once with the same or different
 * parameters.  This must always be called before any other functions are
 * called with the same \a purpose.  The return value must be the same as
 * previous returns for a given purpose, even as a result of reboot or power
 * cycle.
 *
 * @note Purposes for which there is no client may (indeed, probably should)
 * return 0 blocks available.
 *
 * @param purpose The purpose for which the blocks are used.  Implementations
 * MUST return 0 if \a purpose is invalid or not supported.
 *
 * @return Zero indicates either no blocks allocated to \a purpose, or an
 * error:
 *   - \a purpose is invalid, or
 *   - there is no implementation matching \a purpose, or
 *   - the blocks are not available.
 *   .
 * Non-zero indicates the number of blocks provided for the specified purpose.
 */
frost_uint
ICE_NonVolatileNumberOfBlocks(ICE_NonVolatilePurpose purpose);


/**
 * @brief Query the size of a block.
 *
 * This returns the number of bytes in a block. It may be called more than
 * once with the same or different parameters.
 *
 * @note Different blocks can have different sizes, but the size and contents
 * of a specific block (as identified by its purpose and block number) must
 * not change even as a result of reboot or power cycle.
 *
 * @param purpose The purpose for which the blocks are used.  Implementations
 * MUST return 0 if \a purpose is invalid or not supported.
 *
 * @param block The index of the block, in the range 0 to
 * (ICE_NonVolatileNumberOfBlocks() - 1) inclusive.  Implementations MUST
 * return 0 if \a block is out of range.
 *
 * @return Zero indicates either a zero-sized block, or an error:
 *   - \a purpose is invalid, or
 *   - there is no implementation matching \a purpose, or
 *   - \a block is out of range.
 *   .
 * Non-zero indicates the size (in bytes) of the specified block.
 *
 * @note Implementation note: Implementations that take advantage of the
 * permitted simplifying assumption that accesses are word-aligned MUST report
 * block sizes that are a multiple of the word size.  That is, implementations
 * MUST NOT report a block size that prevents access by clients to all bytes
 * within the block.  For example, if an implementation requires 4-byte
 * aligned access, it must report block sizes that are a multiple of 4.
 */
frost_uint
ICE_NonVolatileNumberOfBytes(ICE_NonVolatilePurpose purpose,
                             frost_uint block);


/**
 * @brief Query the technology a block uses.
 *
 * This function returns the type of technology used to implement the
 * specified block.  It may be useful to clients that take advantage of, or
 * rely on, the erase and write behaviour of certain technologies.  This
 * function may be called more than once with the same or different
 * parameters.
 *
 * @note Different blocks can have different technologies, but the technology
 * of a specific block must never change even as the result of reboot or power
 * cycle.
 *
 * @param purpose The purpose for which the blocks are used.  Implementations
 * MUST return #ICE_NonVolatileTechnologyNone if \a purpose is invalid or not
 * supported.
 *
 * @param block The index of the block, in the range 0 to
 * (ICE_NonVolatileNumberOfBlocks() - 1) inclusive.  Implementations MUST
 * return #ICE_NonVolatileTechnologyNone if \a block is out of range.
 *
 * @return The type of technology used to implement the block, or
 * #ICE_NonVolatileTechnologyNone to indicate an error:
 *   - \a purpose is invalid, or
 *   - there is no implementation matching \a purpose, or
 *   - \a block is out of range.
 * For a zero-sized block, you may return any #ICE_NonVolatileTechnologyType,
 * including #ICE_NonVolatileTechnologyNone.
 */
ICE_NonVolatileTechnologyType
ICE_NonVolatileTechnology(ICE_NonVolatilePurpose purpose, frost_uint block);


/**
 * @brief Erase all bytes in a block.
 *
 * For devices that require erasing (flash) this attempts to erase all the
 * bytes in a block.  It returns either when the block has been successfully
 * erased (all bytes set to 0x00 or 0xff) or when the attempt to erase the
 * block has been abandoned as unsuccessful, in which case the contents of the
 * block are in an undefined state.  For such devices, the call to
 * ICE_NonVolatileErase() MUST set ALL bytes in the specified block either to
 * 0xFF or 0x00 (not a mixture) before returning success.
 *
 * For devices where the technology does not require a byte to be in the
 * erased state for a write of an arbitrary value to succeed (hard disk and
 * EEPROM), a call to ICE_NonVolatileErase() with valid parameters should
 * return success (#frost_true).  The implementation may modify the contents
 * of the specified block in any way, including doing nothing.  This allows
 * "simple" clients to work without needing to distinguish between the
 * different underlying technologies.

 * @param purpose The purpose for which the blocks are used.  Implementations
 * MUST return #frost_false if \a purpose is invalid or not supported.
 *
 * @param block The index of the block, in the range 0 to
 * (ICE_NonVolatileNumberOfBlocks() - 1) inclusive.  Implementations MUST
 * return #frost_false if \a block is out of range.
 *
 * @return #frost_false indicates failure and that the contents of the block
 * are undefined:
 *   - \a purpose is invalid, or
 *   - there is no implementation matching \a purpose, or
 *   - \a block is out of range, or
 *   - the erase was abandoned before all the bits were erased.
 *   .
 * #frost_true indicates success: arbitrary values may now be written to bytes
 * in the block by calling ICE_NonVolatileWrite().
 *
 * For a zero-sized block, the return value is undefined.
 */
frost_bool
ICE_NonVolatileErase(ICE_NonVolatilePurpose purpose, frost_uint block);


/**
 * @brief Read from a block of non-volatile storage.
 *
 * This attempts to read bytes from part or all of a block into a
 * client-supplied buffer.  It returns either when the data has been
 * successfully read and copied into the buffer, or when the attempt to read
 * from the block has been abandoned as unsuccessful, in which case the
 * contents of the buffer are in an undefined state.
 *
 * @note The read is not permitted to extend beyond a block.
 *
 * @note The implementation must read data from the actual device and not from
 * a cache.
 *
 * @note The order that the bytes are transferred to the device is undefined.
 *
 * @note Implementation note: Eclipse currently (and for the foreseeable
 * future) uses this interface in a restricted way.  When Eclipse calls
 * ICE_NonVolatileWrite() or ICE_NonVolatileRead() the \a offset and \a count
 * parameters will always be a multiple of 4.  This may dramatically simplify
 * implementations on word-addressable flash devices, for example.
 * Implementations that make use of this assumptions should assert that \a
 * offset and \a count are indeed a multiple of the word size they assume.
 *
 * @param purpose The purpose for which the blocks are used.  Implementations
 * MUST return #frost_false if \a purpose is invalid or not
 * supported.
 *
 * @param block The index of the block, in the range 0 to
 * (ICE_NonVolatileNumberOfBlocks() - 1) inclusive.
 *
 * @param offset The offset in the block of the byte that is transferred to
 * the first byte in the buffer, in the range 0 to
 * (ICE_NonVolatileNumberOfBytes() - 1) inclusive.
 *
 * @param count The number of bytes to be transferred from non-volatile
 * storage into \a buffer.  (\a offset + \a count - 1) must lie within
 * the block (i.e. be less than ICE_NonVolatileNumberOfBytes()).
 *
 * @param buffer A caller-supplied buffer of at least \a count bytes in size,
 * into which the contents of the specified bytes of non-volatile storage will
 * be copied.
 *
 * @return #frost_false indicates the read was unsuccessful and the contents
 * of \a buffer are undefined:
 *   - \a purpose is invalid, or
 *   - there is no implementation matching \a purpose, or
 *   - \a block is out of range, or
 *   - \a count is out of range, or
 *   - the read was abandoned before all the bits were transferred to the
 *     buffer.
 *   .
 * #frost_true indicates \a count bytes have been transferred from the
 * specified non-volatile storage at \a offset to \a buffer, bytes 0 to (\a
 * count - 1).
 *
 * If count is zero, the return value is undefined.
 */
frost_bool
ICE_NonVolatileRead(ICE_NonVolatilePurpose purpose,
                    frost_uint block,
                    frost_uint offset,
                    frost_uint count,
                    frost_uint8 * buffer);


/**
 * @brief Write to a block of non-volatile storage.
 *
 * This attempts to write bytes from a client-supplied buffer into part or all
 * of a block of non-volatile storage.  It returns either when the data has
 * been successfully written or when the attempt to write to the block has
 * been abandoned as unsuccessful, in which case the contents of the
 * corresponding area in non-volatile storage are undefined.
 *
 * @note The write is not permitted to extend beyond a block.
 *
 * @note The implementation must write data to the actual device and not to a
 * cache.
 *
 * @note The destination must not be erased prior to writing. If the data
 * cannot be written correctly because erased bits in the buffer have already
 * been programmed on the device, then the call will fail and return
 * #frost_false.
 *
 * @note The order that the bytes are written to the block is undefined.
 *
 * @note Implementation note: Eclipse currently (and for the foreseeable
 * future) uses this interface in a restricted way.  When Eclipse calls
 * ICE_NonVolatileWrite() or ICE_NonVolatileRead() the \a offset and \a count
 * parameters will always be a multiple of 4.  This may dramatically simplify
 * implementations on word-addressable flash devices, for example.
 * Implementations that make use of this assumptions should assert that
 * \a offset and \a count are indeed a multiple of the word size they assume.
 *
 * @param purpose The purpose for which the blocks are used.  Implementations
 * MUST return #frost_false if \a purpose is invalid or not supported.
 *
 * @param block The index of the block, in the range 0 to
 * (ICE_NonVolatileNumberOfBlocks() - 1) inclusive.  Implementations MUST
 * return #frost_false if \a block is out of range.
 *
 * @param offset The offset in the block of the byte that is transferred to
 * the first byte in the buffer, in the range 0 to
 * (ICE_NonVolatileNumberOfBytes() - 1) inclusive.  Implementations MUST
 * return #frost_false if \a offset is out of range.
 *
 * @param count The number of bytes to be transferred from \a buffer into
 * non-volatile storage.  (\a offset + \a count - 1) must lie within the
 * block (i.e. be less than ICE_NonVolatileNumberOfBytes()).  Implementations
 * MUST return #frost_false if \a count is out of range.
 *
 * @param buffer A caller supplied read-only buffer of at least \a count bytes
 * in size.  The contents of the specified bytes of non-volatile storage will
 * be set to the values in \a buffer.
 *
 * @return #frost_false indicates the write was unsuccessful and the contents
 * of the corresponding area of non-volatile storage are undefined:
 *   - \a purpose is invalid, or
 *   - there is no implementation matching \a purpose, or
 *   - \a block is out of range, or
 *   - \a count is out of range, or
 *   - the write was abandoned before all the bits were programmed, or
 *   - some bits in the buffer were already programmed in the block.
 *   .
 * #frost_true indicates \a count bytes in non-volatile storage at the \a
 * offset specified are now the same as \a buffer bytes 0 to (\a count - 1).
 *
 * If count is zero, the return value is undefined.
 */
frost_bool
ICE_NonVolatileWrite(ICE_NonVolatilePurpose purpose,
                     frost_uint block,
                     frost_uint offset,
                     frost_uint count,
                     frost_uint8 const * buffer);

/**
 * @brief Indicate if persistent store may be (temporarily) held in RAM.
 *
 * A flag to indicate whether risky movement of data is permitted.
 * Returning #frost_true will result in better utilisation of
 * store, but with a risk that data may be lost.
 * Returning #frost_false guarantees persistent data is stored reliably
 * regardless of power loss or system failure, where sufficient non-volatile
 * storage is available.
 *
 * @param purpose The purpose for which the blocks are used.  Implementations
 * MUST return #frost_false if \a purpose is invalid or not supported.
 *
 * @return #frost_false indicates that peristent data must not be copied to
 * RAM or any other non-persistent storage and/or that \a purpose is invalid.
 *
 * #frost_true indicates that persistent data may be temporarily held in
 * RAM e.g. while a persistent block is being erased.
 */
frost_bool
ICE_NonVolatileUnsafePermitted(ICE_NonVolatilePurpose purpose);

/**
 * @brief Create backup for unrecoverable cases
 *
 * @return #frost_true indicates to it is successful
 * */
frost_bool
ICE_NonVolatileBackup(void);

/**
 * @brief Restore backup for unrecoverable cases
 *
 * @return #frost_true indicates to it is successful
 * */
frost_bool
ICE_NonVolatileRestoreToBackup(void);

/**
 * @brief Revert backup files
 *
 * @return #frost_true indicates to it is successful
 * */
frost_bool
ICE_NonVolatileRevertBackup(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CABOT_ICE_ICE_NONVOLATILE_H_INCLUDED*/
