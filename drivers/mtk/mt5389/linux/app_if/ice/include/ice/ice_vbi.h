/**
 * Copyright 2002-3 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief ICE Interface for insertion of data into the
 *        vertical blanking interval.
 *
 * This includes EBU teletext data and copy protection data
 *
 * If Eclipse is ported to hardware that supports the insertion of EBU
 * teletext data into the vertical blanking interval, then the teletext functions
 * defined by this API should be implemented.If insertion of EBU teletext
 * data into the vertical blanking interval is not supported/required, then
 * these functions should return frost_false.
 *
 * The teletext API herein is not to be used to drive a resident teletext decoder if that
 * decoder intends to use the same OSD that Eclipse uses via the ice_osd.h API.
 *
 * In this documentation, "EBU teletext data" includes teletext subtitles,
 * since these are carried on a particular teletext magazine/page.
 *
 * The copy protection API herein covers insertion of APS(Macrovison),CGMS and RCT(Redistribution control) data
 *
 */

#ifndef CABOT_ICE_ICE_VBI_H_INCLUDED
#define CABOT_ICE_ICE_VBI_H_INCLUDED

#include "frost/frost_basictypes.h"
#include "ice/ice_device_number.h"
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief TeletextVBI device open parameters.
 */
typedef struct ICE_VBITeletextOpenParams
{
    /**
     * Identifies the instance of the device as the source of teletext.
     */
    ICE_DeviceSource m_device_source;
} ICE_VBITeletextOpenParams;

/**
 * @brief Opaque type used to identify an open connection.
 */
typedef struct ICE_VBITeletextHandle ICE_VBITeletextHandle;

/**
 * @brief Start insertion of EBU teletext data into the vertical blanking interval.
 *
 * @param params        A pointer to 'c' structure for configuring ice layer VBI teletext.
 *
 * @return A handle for future use by the ice layer or NULL (if the handle is already in use).
 */
ICE_VBITeletextHandle * ICE_VBITeletextOpen( const ICE_VBITeletextOpenParams * params );

/**
 * @brief Closes the connection to the VBI insertion device and releases any resources.
 *
 * @param handle The handle to the VBI insertion device to be closed. NULL handles are to be ignored.
 */
void ICE_VBITeletextClose(ICE_VBITeletextHandle* handle);

/**
 * @brief Start insertion of EBU teletext data into the vertical blanking interval.
 *
 * @param handle            A handle to the ice layer teletext handler.
 * @param pid               The PID which carries the teletext data
 * @param magazine_number   The magazine number of the initial page (1-8)
 * @param page_number       The page number carrying initial page in original hex format (0x00-0xFF)
 *
 * @note magazine_number and page_number indicate the initial page which
 *       should be shown if the "teletext" mode is entered by the viewer.
 *       These parameters are only of interest if the ICE implementation is
 *       able to pass this information to the television's teletext decoder.
 *       If this is not possible, then these parameters may be ignored.
 *
 * @return frost_true if VBI insertion was started, otherwise frost_false.
 */
frost_bool ICE_VBITeletextInsert(ICE_VBITeletextHandle * handle, frost_uint pid, frost_uint magazine_number, frost_uint page_number);


/**
 * @brief Stop any current VBI insertion of EBU teletext data
 *
 * @param handle            A handle to the ice layer teletext handler.
 * Stop any current VBI insertion of EBU teletext data.  Any data that may be
 * cached in the television's teletext decoder should be flushed if possible.
 *
 * @return frost_true if VBI insertion was stopped, otherwise frost_false.
 */
frost_bool ICE_VBITeletextClear(ICE_VBITeletextHandle * handle);


/**
 * @brief Commence showing of a given teletext page if possible.
 *
 * This function allows Eclipse to instruct a teletext decoder to enter "teletext"
 * mode and show a specified magazine and page number.  Eclipse will use this
 * function to cause teletext subtitles to be displayed when a teletext subtitle
 * source is selected by the UI application.
 *
 * The implementation must not use Eclipse's OSD to show the teletext page as
 * Eclipse may continue to render graphics to its OSD using ice_osd.h
 * functions while a teletext page is being shown.
 *
 * @param handle            A handle to the ice layer teletext handler.
 * @param pid               The PID which carries the teletext data
 * @param magazine_number   The magazine number carrying the page to show (1-8)
 * @param page_number       The page number carrying the page to show in original hex format (0x00-0xFF)
 *
 * @note magazine_number and page_number are only of interest if the ICE
 *       implementation is able to instruct a teletext decoder to
 *       begin decoding and showing this teletext page.
 *
 * @return frost_true if the specified teletext page is showing, otherwise frost_false.
 */
frost_bool ICE_VBITeletextShowPage(ICE_VBITeletextHandle * handle, frost_uint pid, frost_uint magazine_number, frost_uint page_number);


/**
 * @brief Cease the showing of any teletext page.
 *
 * @param handle            A handle to the ice layer teletext handler.
 * Stop displaying any teletext page which is showing as a result of a previous
 * call to ICE_VBITeletextShowPage().
 */
frost_bool ICE_VBITeletextStopShowingPage(ICE_VBITeletextHandle * handle);


/**
 * @brief Teletext data bytes
 *
 * @param handle            A handle to the ice layer teletext handler.
 * @param bytes             An array of 44 bytes containing the data
 * When Teletext data is decoded in PES packets this function is called for each 44 bytes of data.
 */
void ICE_VBITeletextData(ICE_VBITeletextHandle * handle, frost_uint8 const *bytes);


/**
 * @brief Opaque type used to identify an open connection to the copy 
 * protection system.
 */
typedef struct ICE_VBICopyProtectionHandle ICE_VBICopyProtectionHandle;


/**
 * @brief Values for the Analogue Protection System (APS) using Automatic Gain 
 * Control (AGC) and Colorstripe copy control systems.
 *
 * See the document entitled "Specification of the Macrovision Copy Protection 
 * Process for STB/IRD Products" Revision 7.1S1, October 1, 1999 and other
 * applicable specifications licensed by Macrovision.
 */
typedef enum ICE_VBICopyProtectionAPS
{
    /**
     * No Analogue Protection System (APS) is required. 
     *
     * This is the equivilant to APS value (0, 0).
     *
     * Automatic Gain Control (AGC) copy control system off. 
     */
    vbi_aps_off,

    /**
     * Automatic Gain Control (AGC) copy control system on, split burst off.
     *
     * This is the equivilant to APS value (0, 1).
     */
    vbi_aps_agc_on_split_burst_off,

     /**
     * Automatic Gain Control (AGC) copy control system on, 2 line split burst.
     *
     * This is the equivilant to APS value (1, 0).
     */
    vbi_aps_agc_on_2_line_split_burst,

    /**
     * Automatic Gain Control (AGC) copy control system on, 4 line split burst.
     *
     * This is the equivilant to APS value (1, 1).
     */
    vbi_aps_agc_on_4_line_split_burst

} ICE_VBICopyProtectionAPS;


/**
 * @brief Values for Copy Generation Management System - Analog (CGMS-A) .
 *
 * Copy protection for analog signals as defined in IEC 61880, IEC 61880-2, 
 * IEC 62375, JEITA EIA-J, CEA-608-B, CEA-805, CPR1204-1, ETSI EN 300 294 and
 * other relevant specificaitons depending out the analog output format.
 */
typedef enum ICE_VBICopyProtectionCGMSA
{
    /**
     * Unlimited copies may be made of the content. 
     *
     * This is the equivilant to CGMS Value (0, 0)
     */
    vbi_cgms_copy_freely,

    /**
     * One generation of copies has already been made; no further copying is 
     * allowed.
     *
     * This is the equivilant to CGMS Value (0, 1)
     */
    vbi_cgms_copy_no_more,
    
    /** 
     * One generation of copies may be made
     *
     * This is the equivilant to CGMS Value (1, 0)
     */
    vbi_cgms_copy_once,

    /**
     * No copies may be made of the content
     *
     * This is the equivilant to CGMS Value (1, 1)
     */
    vbi_cgms_copy_never

} ICE_VBICopyProtectionCGMSA;


/**
 * @brief Open a handle for copy protection functions which use VBI
 *
 * @return A handle for future use by the ice layer or NULL (if the handle is
 * already in use).
 */
ICE_VBICopyProtectionHandle * ICE_VBICopyProtectionOpen(void);


/**
 * @brief Closes the connection to the copy protection functionality
 *
 * @param handle The handle to the copy protection functionality
 */
void ICE_VBICopyProtectionClose(ICE_VBICopyProtectionHandle * handle);


/**
 * @brief Set the mode for the Analogue Protection System (APS).
 *
 * Uses the Automatic Gain Control (AGC) and Colorstripe copy control systems.
 *
 * See the document entitled "Specification of the Macrovision Copy Protection 
 * Process for STB/IRD Products" Revision 7.1S1, October 1, 1999 and other
 * applicable specifications licensed by Macrovision.
 *
 * @remarks \a aps_mode and \a aps_bits are both specified because it is 
 * possible to set the APS bits on output without enabling the Macrovision
 * Automatic Gain Control (AGC) itself. For example, if the CGMS-A is copy once
 * and no copy has been made yet then Macrovision AGC would be disabled but the
 * APS bits may still be set so that a recording device would copy and apply 
 * them for its own output.
 *
 * @param handle    The handle to the copy protection functionality.
 * @param aps_mode  The APS mode to apply.
 * @param aps_bits  The APS bits to set in the VBI.
 *
 * @return #frost_true if mode and bits are set successfully.
 */
frost_bool ICE_VBICopyProtectionAPSSet(
    ICE_VBICopyProtectionHandle * handle, 
    ICE_VBICopyProtectionAPS      aps_mode,
    ICE_VBICopyProtectionAPS      aps_bits);


/**
 * @brief Set the bits for the Copy Generation Management System - Analog 
 * (CGMS-A) system in the VBI.
 *
 * Copy protection for analog signals as defined in IEC 61880, IEC 61880-2, 
 * IEC 62375, JEITA EIA-J, CEA-608-B, CEA-805, CPR1204-1, ETSI EN 300 294 and
 * other relevant specificaitons depending out the analog output format.
 *
 * @param handle    The handle to the copy protection functionality.
 * @param cgmsa     The CGMS-A bits to set.
 *
 * @return #frost_true if bits are set successfully.
 */
frost_bool ICE_VBICopyProtectionCGMSASet(
    ICE_VBICopyProtectionHandle * handle, 
    ICE_VBICopyProtectionCGMSA    cgmsa);


ICE_VBICopyProtectionCGMSA ICE_VBICopyProtectionCGMSAGet(ICE_VBICopyProtectionHandle * handle);

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_VBI_H_INCLUDED*/
