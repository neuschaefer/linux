/**
 * Copyright 2010 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief  Interface to the STB's on screen display (OSD).
 *
 * This file defines the ICE interfaces to the on-screen display.
 *
 * All functions in this interface are implemented in STB-specific code
 * and called by Cabot code.
 *
 * The OSD model centres around the creation of one or more graphics planes.
 * A plane represents an area of display memory upon which rendering can
 * be performed. Each plane maintains its own pixel buffer called surface.
 * A plane is always full-screen.
 *
 * Each plane has a colour model which describes the pixel format. For example,
 * a pixel value may correspond to a colour look-up table (CLUT), or may be a
 * direct-colour ARGB value. If a plane uses a CLUT, then the plane will
 * additionally maintain a palette which maps pixel values to colours.
 *
 * Rendering operations on a plane are assumed to be atomic; that is, any
 * visual updating of the OSD should appear instantaneous to the user. In
 * particular, changing a plane palette (if any) and modifying the contents
 * of the pixel buffer. Eclipse recommends, but does not assume, a double-
 * buffering implementation of bit block transfers (blit) in order to facilitate
 * this.
 *
 * <h3>Sequence of initialisation</h3>
 *
 * To aid implementation of this API (for new ports and for those upgrading from
 * previous eclipse versions) the following gives an overview of the order of
 * initialisation.
 *
 * i) Capabilities:
 * During system initialisation ICE_OsdGetCapability will be called. This allows
 * the platform to specify its graphics rendering capabilities via the
 * ICE_OsdCapability type. This type identifies an array of ICE_OsdPlaneCapability(s)
 * which specify the details of each plane (size, colour-model etc.)
 *
 * ii) Plane Creation:
 * Once eclipse knows the capabilities of the system, a request for a plane
 * will be made via ICE_OsdPlaneOpen. This plane should correspond to one
 * specified as being supported in the above stage.
 *
 * iii) Off-screen surface allocation:
 * To allow vendors to choose what memory is used for a screen pixel buffer, the
 * ICE_OsdCreateSurface function is called. This provides eclipse with
 * additional pixel-buffer memory to use for hardware-accelerated rendering and
 * blitting.
 *
 * <h3>Pixel buffer requirements</h3>
 *
 * Direct access to the OSD pixel buffer is required. Additionally, off-screen
 * surface support may be provided to allow for hardware-accelerated operations.
 *
 * See #ICE_OsdPlaneGetSurface() and #ICE_OsdSurfaceCreate() for details.
 *
 * <h3>Hardware acceleration</h3>
 *
 * Eclipse contains generic implementations of all graphic operations it uses.
 * Platfrom may provide own implementation of some or all of those operations
 * in order to make use of specialised graphics hardware, assembly language
 * routines or other optimisations. Surface API gives platform full control over
 * selection of generic or hardware-accelerated functions at run time.
 *
 */

#ifndef CABOT_ICE_ICE_OSD_H_INCLUDED
#define CABOT_ICE_ICE_OSD_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "frost/frost_basictypes.h"
#include "ice/ice_osd_types.h"

/**
 * @brief Opaque type to identify a handle to an OSD graphics plane.
 *
 * OSD plane represents a full-screen graphics layer. It can be z-ordered shown
 * or hidden. The ICE_OsdSurfaceHandle received from plane is used for
 * drawing to an OSD.
 */
typedef struct ICE_OsdPlaneHandle ICE_OsdPlaneHandle;
/**
 * @brief Opaque type to identify a handle to a surface.
 *
 * Surface represents a pixel buffer Eclipse can write to. Each OSD plane has
 * a surface associated with it. Off-screen surfaces can also be created.
 */
typedef struct ICE_OsdSurfaceHandle ICE_OsdSurfaceHandle;

/***************************************************************************/
/**
 * @}
 * @name Basic OSD functions
 * @{
 */
/***************************************************************************/

/**
 * @brief Get memory buffer for OSD use.
 *
 * Eclipse requires one or two full-screen buffers that will be used to
 * store off-screen images for later blitting to the OSD.
 * This function allows the implementation to determine where this memory
 * is located.  At its most basic, the implementation might return the result
 * of malloc(buffer_size).
 *
 * The requested size will match the size of the OSD plane used for UI or
 * the OSD plane used for MHEG, as returned by ICE_OsdGetCapability.  In the
 * simplest case of a 720x576 8-bit OSD, Eclipse requires 414,720 bytes (i.e.
 * 720 * 576 bytes) of memory.  For the largest likely OSD, a 1920x1080 HD
 * OSD using ARGB8888, Eclipse requires 1920 * 1080 * 4 == 8,294,400 bytes.
 *
 * This function will be called twice - once to request a buffer for the UI,
 * and once to request a buffer for MHEG (if included).
 *
 * If only one OSD plane is shared by UI and MHEG, then Eclipse cannot display
 * MHEG and UI at the same time.  In this case Eclipse only requires one buffer,
 * and the second call to this function may return the address of that same
 * buffer.
 *
 * If UI and MHEG are in different OSD groups, or if there are two or more OSD
 * planes in the group used for UI and MHEG,  then Eclipse requires two
 * distinct buffers - one for UI, and one for MHEG.
 *
 * @param buffer_size The size of the required buffer in bytes.
 * @return Address of block of memory for OSD use.
 */
void *ICE_OsdGetBuffer(frost_int buffer_size);


/**
 * @brief Release the memory reserved by ICE_OsdGetBuffer
 * @param buffer_to_release Pointer to memory to be released
 *
 * @note If multiple OSD buffers have been created using
 *       <code>ICE_OsdGetBuffer</code> then the pointers should be stored so
 *       that the buffer_to_release can be identified
 *
 * @note If the OSD Buffer has been released, Eclipse will will bypass
 * any drawing operations until an OSD buffer is made available again
 * using ICE_OsdGetbuffer.
 */
void ICE_OsdReleaseBuffer(void * buffer_to_release);

/**
 * @brief Get the supported capabilities of the OSD device.
 *
 * This function will be called during system initialisation and is used to
 * determine the graphics capabilities of the system.
 *
 * @param osd_capability The capabilities of the systems OSD device.
 *
 * @return #frost_true if osd_capability field has been set, otherwise
 *         #frost_false.
 */
frost_bool ICE_OsdGetCapability(ICE_OsdCapability * osd_capability);

/**
 * @brief Get the co-ordinate system for the video scaling output window.
 *
 * This function will be called during system initialisation and is used to
 * determine the video positioning & scaling capabilities of the system.
 *
 * @todo This function should be renamed and moved to ice_video.h.
 *
 * @param osd_screen_resolution The screen resolution used for video scaling.
 *
 * @return #frost_true if osd_screen_resolution field has been set, otherwise
 *         #frost_false.
 */
frost_bool ICE_OsdGetScreenResolution(ICE_OsdScreenResolution * osd_screen_resolution);


/**
 * @brief Opens a graphics plane on an OSD device.
 *
 * This function opens a graphics plane on an OSD, so that it can be drawn
 * to. Multiple graphics planes may be visible at the same time. Graphics
 * planes are always full-screen.
 *
 * Planes are initially hidden, and a call to ICE_OsdPlaneSetVisible() is
 * required to make the plane visible.
 *
 * Planes must be initially cleared to be fully-transparent by this function.
 * This applies to both truecolour and palette-based planes.
 *
 * The initial palette and contents of palette-based planes are undefined,
 * and Cabot will set the palette and blit to every pixel before showing a
 * palette-based plane.
 *
 * The OSD plane will initially be scaled to full screen.
 *
 * The width, height and colour_model will match the relavant entry returned
 * by ICE_OsdGetCapabilities().
 *
 * @param plane_position The graphics plane to open.  0 is the front plane,
 *                       higher numbers are further away from the viewer in
 *                       the Z order.  For hardware supporting N planes, this
 *                       number will be in the range 0 to (N-1) inclusive.
 * @param width          Horizontal resolution, in pixels, of the graphics
 *                       plane.  Typically 720 for SD PAL systems.
 * @param height         Vertical resolution, in pixels, of the graphics
 *                       plane.  Typically 576 for SD PAL systems.
 * @param colour_model   The colour model to use.
 *
 * @return Handle to the new plane, or NULL if it cannot be created.
 */
ICE_OsdPlaneHandle* ICE_OsdPlaneOpen(frost_uint plane_position,
                                     frost_uint width,
                                     frost_uint height,
                                     ICE_OsdColourModel colour_model);


/**
 * @brief Close an active OSD plane.
 *
 * This call should hide the plane (if it is not already hidden).
 * After this call, the ICE_OsdPlaneHandle plane_handle is invalid
 * and will not be used again. The OSD plane can be re-opened using
 * ICE_OsdPlaneOpen.
 *
 * If plane_handle is NULL, this function shall do nothing.
 *
 * @param plane_handle The handle to the plane to close, or NULL.
 */
void ICE_OsdPlaneClose(ICE_OsdPlaneHandle *plane_handle);


/**
 * @brief Show or hide an OSD plane.
 *
 * Call this function to make the OSD plane visible on the device, or to hide
 * it.
 *
 * @note ICE_OsdPlaneBlitRectangle can be called whilst a plane is not visible.
 *       The OSD should not be updated whilst the plane is not visible.
 *
 * @param plane_handle Handle to the OSD plane.
 * @param visible If #frost_true, the plane should be made visible on the
 *                device. Otherwise the plane should be hidden.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_OsdPlaneSetVisible(ICE_OsdPlaneHandle *plane_handle,
                                  frost_bool visible);

/**
 * @brief Clears an OSD plane to a specified colour.
 *
 * @param plane_handle Handle to the plane to clear.
 * @param clear_colour The colour to clear to, as a pointer to a 1x1 bitmap in the
 *               current mode of the OSD plane.  (I.e. a pointer to a
 *               frost_uint8 palette index for #ICE_OsdColourModel8BitCLUT,
 *               a pointer to a frost_uint16 RGB4444 value for
 *               #ICE_OsdColourModelARGB4444, or a pointer to a frost_uint32
 *               ARGB8888 value for #ICE_OsdColourModelARGB8888 OSD planes).
 * @param palette The palette to be applied to the plane. The changing
 *                of the palette and the blit should be atomic from
 *                the user's perspective. If the plane does not use a
 *                CLUT, and the palette is non-zero then this call
 *                return frost_false. If the plane uses a CLUT then a
 *                null pointer indicates that the current palette shall
 *                be used. Palette will be an array with the appropriate
 *                number of entries for the colour depth of the plane.
 *                For planes with type ICE_OsdColourModel8BitCLUT this will
 *                be 256 entries.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_OsdPlaneClear(ICE_OsdPlaneHandle *plane_handle,
                        const void * clear_colour,
                        const ICE_Colour * palette);

/**
 * @brief Bring the OSD plane to the front of other OSD planes.
 *
 * @param handle The handle to the required OSD plane.
 *
 * This function may be called when Eclipse is using multiple OSD planes.
 * When this function is called, the output of the OSD plane specified should
 * be brought in front of any other visible OSD planes.
 *
 * @note On platforms which do not support multiple OSD planes of the same type
 *       (e.g. multiple 8-bit planes) or multiple planes which can be reconfigured
 *       at run-time, this function may return frost_false.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_OsdPlaneToFront(ICE_OsdPlaneHandle* handle);


/**
 * @brief Send the OSD plane to the back of other OSD planes.
 *
 * @param handle The handle to the required OSD plane.
 *
 * This function may be called when Eclipse is using multiple OSD planes.
 * When this function is called, the output of the OSD plane specified should
 * be displayed behind that of any other visible OSD planes.
 *
 * @note On platforms which do not support multiple OSD planes of the same type
 *       (e.g. multiple 8-bit planes) or multiple planes which can be reconfigured
 *       at run-time, this function may return frost_false.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_OsdPlaneToBack(ICE_OsdPlaneHandle* handle);


/**
 * @brief Specify the size and location on screen at which the OSD will be
 *        shown.
 *
 * This function takes a region of the source OSD plane and scales and positions
 * it so that it appears in the given area of the display.
 *
 * The top-left co-ordinates for these windows will not be negative.
 * These windows cannot have a size of zero.
 *
 * The input window is relative to the plane size, as specified in the call
 * to ICE_OsdPlaneOpen().  The input window will not go outside the bounds of
 * the OSD plane.
 *
 * The output window is relative to the "full screen output" size returned by
 * ICE_VideoGetFullScreenOutputResolution().  The bottom-right corner of the
 * output window will not exceed the "full screen output" size.
 *
 * @param handle        The handle of the OSD plane that will be positioned.
 * @param input_window  The part of the OSD region that will be positioned.
 *                      Will not be null.
 * @param output_window The area of the display where the OSD will appear.
 *                      Will not be null.
 *
 * @note This function must always be present (at least as a stub). A full
 *       implementation is only required for single-chip IDTVs and HDMI set top
 *       boxes.
 *
 * @return #frost_true on success, #frost_false otherwise.
 */
frost_bool ICE_OsdPlanePosition(ICE_OsdPlaneHandle* handle,
                                const ICE_DisplayWindow *input_window,
                                const ICE_DisplayWindow *output_window);


/***************************************************************************/
/**
 * @}
 * @name OSD functions for Freesat extensions
 * @{
 */
/***************************************************************************/

/**
 * @brief Specify the colour of the background plane
 *
 * @param plane_colour The colour to be displayed on the background plane.
 *
 * @note The transparency component must be ignored such that plane_colour is
 *       rendered opaque.
 */
void ICE_OsdSetBackgroundColour(ICE_Colour plane_colour);


/***************************************************************************/
/**
 * @}
 * @name Surface handling functions
 * @{
 */
/***************************************************************************/


/**
 * @brief Get interface to plane's surface.
 *
 * Implementation of this function is mandatory. All drawing operations are
 * performed using returned surface handle.
 *
 * Eclipse will never destroy the returned handle.  (I.e. it will not call
 * ICE_OsdSurfaceDestroy()).  The handle should be destroyed by the ICE
 * implementation when ICE_OsdPlaneClose() is called.
 *
 * If this function is called multiple times, the ICE implementation can
 * return the same handle each time.
 *
 * @param plane_handle Handle to an open OSD plane.
 * @return handle to the surface representing plane's pixel buffer.
 */
ICE_OsdSurfaceHandle *
ICE_OsdPlaneGetSurface(ICE_OsdPlaneHandle *plane_handle);

/**
 * @brief Create an off-screen surface.
 *
 * Implementation of this function is highly recommended. Partial
 * implementations are allowed and recommended if target platform supports
 * subset of required features.
 *
 * This function should create an off-screen surface. Off-screen
 * surfaces are used by Eclipse to take advantage of hardware-accelerated blit
 * operations.
 *
 * For 8-bit surfaces, the initial palette is undefined.  Eclipse will call
 * ICE_OsdSurfaceSetPalette() to set the palette.
 *
 * @param properties Required surface properties.  This function must copy any
 *                   fields it needs; the pointer must not be kept once this
 *                   function returns.
 * @return Handle to newly created surface or NULL on error.
 * @see ICE_OsdSurfaceProperties
 */
ICE_OsdSurfaceHandle *
ICE_OsdSurfaceCreate(const ICE_OsdSurfaceProperties * properties);

/**
 * @brief Destroy an off-screen surface.
 *
 * Implementation of this function is mandatory if ICE_OsdPlaneGetSurface() is
 * implemented.
 *
 * The ICE_OsdSurfaceDestroy should destroy an off-screen surface created by
 * ICE_OsdSurfaceCreate.
 *
 * @param surface Handle to an off-screen surface to be destroyed.
 */
void
ICE_OsdSurfaceDestroy(ICE_OsdSurfaceHandle *surface);

/**
 * @brief Get direct access to surface memory.
 *
 * Implementation of this function is mandatory.
 *
 * This function should give Eclipse direct access to the surface memory.
 *
 * For double-buffered OSDs, this may give access to a back buffer.  In that
 * case, ICE_OsdUpdate() must copy the dirty region from the back buffer
 * to the front buffer.
 *
 * Eclipse will not lock an already locked surface.
 *
 * @param surface Handle to surface
 * @return ICE_OsdSurfaceMemory structure providing non-NULL pointer to
 *         surface's memory together with pitch and other properties.
 *         This pointer is valid until the corresponding call to
 *         ICE_OsdSurfaceUnlock().
 */
const ICE_OsdSurfaceMemory *
ICE_OsdSurfaceLock(ICE_OsdSurfaceHandle *surface);

/**
 * @brief Unlock surface memory locked by #ICE_OsdSurfaceLock.
 *
 * Implementation of this function is mandatory.
 *
 * Eclipse will call this function after updating some (or all) pixels of
 * the surface.
 *
 * This function can be stubbed out for off-screen surfaces if
 * #ICE_OsdSurfaceCreate is unimplemented.
 *
 * @param surface Handle to surface
 * @param dirty_rect Rectangle bounding all modified pixels or NULL no pixels
 *        were modified.
 */
void
ICE_OsdSurfaceUnlock(
        ICE_OsdSurfaceHandle *surface,
        const ICE_OsdRect * dirty_rect);

/**
 * @brief Change palette of an 8-bit CLUT surface.
 *
 * Implementation of this function is mandatory only if 8-bit CLUT surfaces
 * are supported.
 *
 * When the palette of an OSD surface is changed, the behaviour is slightly
 * implementation-dependent.  The ICE implementation may choose to apply the
 * new palette immediately, or it may choose to wait until ICE_OsdUpdate() is
 * called.
 *
 * WARNING: Currently this is called even on non-palette surfaces.  This is a
 *          bug in Eclipse.  For now, ice implementations must ignore such
 *          calls.
 *
 * WARNING: Currently this is called even on non-palette surfaces.  This is a
 *          bug in Eclipse.  For now, ice implementations must ignore such
 *          calls.
 *
 * @param surface Handle to surface with #ICE_OsdColourModel8BitCLUT type.
 * @param palette Pointer to 256-element palette to be applied to the surface.
 *
 * @todo Fix Eclipse bugs listed above
 */
void ICE_OsdSurfaceSetPalette(
        ICE_OsdSurfaceHandle *surface,
        const ICE_Colour * palette);

/**
 * @brief Fill a surface with a specified colour.
 *
 * This function can be implemented completely, partially or stubbed out.
 *
 * Implementation makes sense only if the target platform can provide
 * hardware-accelerated or otherwise optimised implementation. Partial
 * implementations are allowed and recommended if target platform
 * supports subset of required features.
 *
 * As a minimum, stub version returning #frost_false should be provided.
 *
 * If implemented, this function should fill surface with provided colour.
 * Depending on parameters passed in, either entire surface or part of it
 * should be filled. Current contents of the surface should either be replaced
 * or blended with provided colour.
 *
 * Returned value is used by Eclipse to determine whether requested operation
 * was handled by the platform. If #frost_false is returned Eclipse will
 * fall-back to internal, generic implementation.
 *
 * @param surface Handle to the surface to clear.
 * @param colour The colour to clear to, as a pointer to a 1x1 bitmap in the
 *               current mode of the OSD plane.  (I.e. a pointer to a
 *               frost_uint8 palette index for #ICE_OsdColourModel8BitCLUT,
 *               a pointer to a frost_uint16 RGB4444 value for
 *               #ICE_OsdColourModelARGB4444, or a pointer to a frost_uint32
 *               ARGB8888 value for #ICE_OsdColourModelARGB8888 OSD planes).
 * @param rect Optional parameter, specifying a rectangle to fill.
 *             If NULL, entire surface should be filled. If not NULL, the
 *             implementation can assume the rectangle is non-empty
 *             and clipped to surface boundaries.
 * @param blend If set to #frost_true, current contents of the surface should
 *        be blended with the provided colour using "SRC_OVER" Porter-Duff
 *        composition rule. If set to #frost_false, contents  of the surface
 *        should be replaced with the provided colour, which is equivalent to
 *        "SRC" rule. Other Porter-Duff operations are not required.
 * @return true on success, false if specified operation could not be performed.
 */
frost_bool ICE_OsdSurfaceFillRect(
        ICE_OsdSurfaceHandle *surface,
        const void * colour,
        const ICE_OsdRect * rect,
        frost_bool blend
        );

/**
 * @brief Copy a rectangle of pixels.
 *
 * This function can be implemented completely, partially or stubbed out.
 *
 * Implementation makes sense only if the target platform can provide
 * hardware-accelerated or otherwise optimised implementation. Partial
 * implementations are allowed and recommended if target platform
 * supports subset of required features.
 *
 * This function performs a blit operation with combination of flags
 * specified in #ICE_OsdBlitFlag enum. If requested combination is unsupported
 * this function must not blit. It should mask out combination of bits it does
 * support from \e blit_flags and return the result.
 *
 * TODO: are some of the flag combinations unused?
 *
 * As a minimum, stub version returning unmodified \e blit_flags should be
 * provided.
 *
 * Returned value is used by Eclipse to determine which of the requested blit
 * flags are unsupported. If value identical to \e blit_flags parameter is
 * returned, Eclipse will fall-back to internal, generic implementation.
 * Any other, non-zero return value will result in 2-stage blit. At first
 * Eclipse will create temporary surface. Unsupported operations will be
 * performed by Eclipse, remaining ones will be handed back to this function.
 *
 * The order of operations depends on #ICE_OsdBlitBlend flag. If
 * #ICE_OsdBlitBlend is supported by the platform, Eclipse will perform all
 * unsupported operations first and than call this function again to perform
 * the remaining ones. If ICE_OsdBlitBlend is unsupported, this function will
 * be called first, to blit into the temporary surface using supported subset
 * of flags. Than Eclipse will perform remaining operations and blend the
 * result into target surface.
 *
 * If generic implementation provided by Eclipse is better option than 2-stage
 * blit this function should return unmodified \e blit_flags.
 *
 * @param dest_surface target surface
 * @param src_surface source surface
 * @param blit_flags Bitwise \c OR of flags specified in #ICE_OsdBlitFlag enum.
 * @param dest_rect Position of the left-top corner on the \e dest_surface
 *        where the \e src_surface should be copied. The width and height
 *        of the \e dest_rect should be ignored unless #ICE_OsdBlitStretch
 *        bit is set in \e blit_flags. Implementation can assume
 *        the rectangle is clipped so it doesn't exceed source or destination
 *        surface size.
 * @param src_rect Optional rectangle within \e src_surface or NULL
 *        to blit whole source \e src_surface. Implementation can assume
 *        the rectangle is clipped so it doesn't exceed source or destination
 *        surface sizes.
 * @param alpha The constant alpha to be multiplied with the alpha of the
 *        source. 0 means to transparent, and 255 means to opaque. This
 *        parameter can be ignored unless #ICE_OsdBlitBlend bit is set.
 * @param rotation optional rotation angle to be applied to \e src_surface if
 *        #ICE_OsdBlitRotate bit is set in \e blit_flags.
 * @return 0 on success, non-zero on error. The returned value should be
 *         a subset of bits passed in \e blit_flags that are unsupported.
 */
frost_uint32 ICE_OsdSurfaceBlit(
        ICE_OsdSurfaceHandle *dest_surface,
        ICE_OsdSurfaceHandle *src_surface,
        frost_uint32 blit_flags,
        ICE_OsdRect dest_rect,
        const ICE_OsdRect * src_rect,
        frost_uint8 alpha,
        ICE_OsdRotation rotation);

/**
 * @brief Update an OSD layer.
 *
 * Eclipse will call this function after finishing a group of drawing
 * operations in order to present the result on screen.
 *
 * On platforms with double-buffered OSD this function may blend all OSD
 * layers into the back buffer and flip buffers.
 *
 * @param osd Handle to the updated OSD plane
 * @param dirty_rect Rectangle bounding all pixels modified on that plane or
 *        NULL if entire plane was modified.
 */
void ICE_OsdUpdate(
        ICE_OsdPlaneHandle *osd,
        const ICE_OsdRect * dirty_rect
        );

#if 0
/**
 * @brief Specify the transparency of OSD
 * @param transparency_level  Must be 0 to 255.
 * @return #frost_true on success, #frost_false otherwise.
 */
frost_bool ICE_OsdSetTransparency(frost_int transparency_level);
#endif

/***************************************************************************/
/**
 * @}
 * @name OSD functions for Freesat extensions
 * @{
 */
/***************************************************************************/

/**
 * @brief Specify the colour of the background plane
 *
 * @param plane_colour The colour to be displayed on the background plane.
 *
 * @note The transparency component must be ignored such that plane_colour is
 *       rendered opaque.
 */
void ICE_OsdSetBackgroundColour(ICE_Colour plane_colour);


/***************************************************************************/
/**
 * @}
 * @name OSD functions for MHP extensions
 *
 * @note These functions should be implemented as long as MHP or a similar
 * module needs the same functionality. Eclipse does not need these
 * functions. Platform draw/blit capabilites should be used for efficiency
 * while imlplementing them.
 * @{
 */
/***************************************************************************/

/**
 * @brief Opaque type to identify a handle to an OSD graphics plane.
 */
typedef struct ICE_OsdBitmapHandle ICE_OsdBitmapHandle;

/**
 * @brief Creates a bitmap structure for the given handle.
 *
 * This function is supposed to allocate enough memory calculated by given
 * width and height and the colour model supported by the plane. If there is
 * h/w support for specific blitting types then related resource allocations
 * can be made in this function.
 *
 * If the bitmap is to be created for the main display, there is no need to allocate
 * an additional buffer if there is one already created while the plane open process.
 *
 * @param plane_handle  The handle to the plane to create the bitmap.
 * @param width         width of the bitmap.
 * @param height        height of the bitmap.
 * @param main_screen   determines whether this bitmap belongs to main osd or offscreen.
 *
 * @return Handle to the created bitmap, or NULL in any case of failure.
 */
ICE_OsdBitmapHandle* ICE_OsdCreateBitmap(ICE_OsdPlaneHandle *plane_handle,
                             frost_uint width,
                             frost_uint height,
                             frost_bool main_screen,
                             frost_bool use_plane_memory);

/**
 * @brief Deallocates all the resources created for the given bitmap.
 *
 * @param bitmap_handle The handle to the bitmap.
 */
void ICE_OsdDestroyBitmap(ICE_OsdBitmapHandle *bitmap_handle);

/**
 * @brief Retrieves the buffer pointer of the given bitmap handle.
 *
 * @param bitmap_handle The handle to the bitmap.
 *
 * @return pointer to the buffer, or NULL in any case of failure.
 */
void* ICE_OsdGetBitmapBuffer(ICE_OsdBitmapHandle *bitmap_handle);

/**
 * @brief Locks the frame buffer of the given bitmap to be able to access it directly.
 *
 * @param bitmap_handle The handle to the bitmap.
 *
 * @return #frost_true on success, #frost_false on failure.
 *
 * @note If this feature is not supported just return #frost_true.
 */
frost_bool ICE_OsdLockBitmap(ICE_OsdBitmapHandle *bitmap_handle);

/**
 * @brief Unocks the frame buffer of the given bitmap after access.
 *
 * @param bitmap_handle The handle to the bitmap.
 *
 * @return #frost_true on success, #frost_false on failure.
 *
 * @note If this feature is not supported just return #frost_true.
 */
frost_bool ICE_OsdUnlockBitmap(ICE_OsdBitmapHandle *bitmap_handle);

/**
 * @brief Fills the bitmap rectangle with the given colour.
 *
 * @param bitmap_handle The handle to the bitmap.
 * @param dst_x_pos     Coordinate, in pixels, of the left-most edge of the
 *                      destination rectangle relative to the plane.
 * @param dst_y_pos     Coordinate, in pixels, of the top-most edge of the
 *                      destination rectangle relative  to the plane.
 * @param width         The width, in pixels, of the destination rectangle.
 * @param height        The height, in pixels, of the destination and source
 *                      rectangles.
 * @param colour        Colour to fill the rectangle.
 * @param strategy      Filling rule. See ICE_OsdBlitStrategy.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_OsdFillRectangle(ICE_OsdBitmapHandle *bitmap_handle,
                             frost_int dst_x_pos,
                             frost_int dst_y_pos,
                             frost_uint width,
                             frost_uint height,
                             frost_uint32 colour,
                             ICE_OsdBlitStrategy strategy);

/**
 * @brief Blit the source contents into destination bitmap.
 *
 * @param dst_bitmap    The handle to the destination bitmap.
 * @param dst_x_pos     Coordinate, in pixels, of the left-most edge of the
 *                      destination rectangle relative to the plane.
 * @param dst_y_pos     Coordinate, in pixels, of the top-most edge of the
 *                      destination rectangle relative  to the plane.
 * @param width         The width, in pixels, of the destination rectangle.
 * @param height        The height, in pixels, of the destination and source
 *                      rectangles.
 * @param src_bitmap    The handle to the source bitmap.
 * @param src_x_pos     Coordinate, in pixels, of the left-most edge of the
 *                      source rectangle relative to the plane.
 * @param src_y_pos     Coordinate, in pixels, of the top-most edge of the
 *                      source rectangle relative  to the plane.
 * @param alpha         Alpha value to multiply with source alpha.
 * @param strategy      Filling rule. See ICE_OsdBlitStrategy.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_OsdBlit(ICE_OsdBitmapHandle *dst_bitmap,
                             frost_int dst_x_pos,
                             frost_int dst_y_pos,
                             frost_uint width,
                             frost_uint height,
                             ICE_OsdBitmapHandle *src_bitmap,
                             frost_int src_x_pos,
                             frost_int src_y_pos,
                             frost_uint8 alpha,
                             ICE_OsdBlitStrategy strategy);

/**
 * @brief Blit the source contents into destination bitmap by stretching.
 *
 * Scales and flips source image to destination bitmap.
 * Scaling of the image from source to destination is performed such that
 * the first coordinate of the source rectangle is mapped to the
 * first coordinate of the destination rectangle, and the second source
 * coordinate is mapped to the second destination coordinate.
 * Drawing on destination bitmap must be performed only inside of clipping area.
 *
 * @param dst_bitmap    The handle to the destination bitmap.
 * @param dst_x_pos     Coordinate, in pixels, of the left-most edge of the
 *                      destination rectangle relative to the plane.
 * @param dst_y_pos     Coordinate, in pixels, of the top-most edge of the
 *                      destination rectangle relative  to the plane.
 * @param dst_width     The width, in pixels, of the destination rectangle.
 * @param dst_height    The height, in pixels, of the destination rectangle.
 * @param dst_x_pos     Coordinate, in pixels, of the left-most edge of the
 *                      destination clip rectangle relative to the plane.
 * @param dst_y_pos     Coordinate, in pixels, of the top-most edge of the
 *                      clip destination rectangle relative  to the plane.
 * @param clip_x_pos    Coordinate, in pixels, of the left-most edge of the
 *                      source rectangle relative to the plane.
 * @param clip_y_pos    Coordinate, in pixels, of the top-most edge of the
 *                      source rectangle relative to the plane.
 * @param clip_width    The width, in pixels, of the destination clip rectangle.
 * @param clip_height   The height, in pixels, of the destination clip rectangle.
 * @param src_bitmap    The handle to the source bitmap.
 * @param src_x_pos     Coordinate, in pixels, of the left-most edge of the
 *                      source rectangle relative to the plane.
 * @param src_y_pos     Coordinate, in pixels, of the top-most edge of the
 *                      source rectangle relative  to the plane.
 * @param src_width     The width, in pixels, of the source rectangle.
 * @param src_height    The height, in pixels, of the source rectangle.
 * @param alpha         Alpha value to multiply with source alpha.
 * @param strategy      Filling rule. See ICE_OsdBlitStrategy.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_OsdStretchBlit(ICE_OsdBitmapHandle *dst_bitmap,
                             frost_int dst_x_pos,
                             frost_int dst_y_pos,
                             frost_uint dst_width,
                             frost_uint dst_height,
                             frost_int clip_x_pos,
                             frost_int clip_y_pos,
                             frost_uint clip_width,
                             frost_uint clip_height,
                             ICE_OsdBitmapHandle *src_bitmap,
                             frost_int src_x_pos,
                             frost_int src_y_pos,
                             frost_uint src_width,
                             frost_uint src_height,
                             frost_uint8 alpha,
                             ICE_OsdBlitStrategy strategy);

/**
 * @brief Refreshes the screen by drawing the contents of the bitmap buffer.
 *
 * @param plane_handle      Handle to the required plane.
 * @param dst_x_pos         Coordinate, in pixels, of the left-most edge of the
 *                          destination rectangle relative to the plane.
 * @param dst_y_pos         Coordinate, in pixels, of the top-most edge of the
 *                          destination rectangle relative  to the plane.
 * @param width             The width, in pixels, of the destination and source
 *                          rectangles.
 * @param height            The height, in pixels, of the destination and source
 *                          rectangles.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_OsdPlaneUpdate(ICE_OsdPlaneHandle *plane_handle,
                             frost_int dst_x_pos,
                             frost_int dst_y_pos,
                             frost_uint width,
                             frost_uint height);

/**
 * @brief Return plane handle for the specified purpose.
 *
 * @param purpose Purpose for which plane handle will be returned.
 *
 * @return Plane handle if there is any for the specified purpose, NULL otherwise.
 */ 
ICE_OsdPlaneHandle* ICE_OsdGetPlaneByPurpose(frost_uint purpose);                         

void * ICE_OsdGrabScreenBMP(frost_uint plane_index, frost_uint * buf_len);

#ifdef __cplusplus
} /* end of extern "C" block */
#endif


/**
 * @brief Draws 4:2:2 image at given position.
 *
 * @param buffer        Pointer to image data.
 * @param dst_x_pos     Coordinate, in pixels, of the left-most edge of the
 *                      destination rectangle relative to the plane.
 * @param dst_y_pos     Coordinate, in pixels, of the top-most edge of the
 *                      destination rectangle relative  to the plane.
 * @param width         The width, in pixels, of the destination rectangle.
 * @param height        The height, in pixels, of the destination and source
 *                      rectangles.
 */
void ICE_SelfVideoFrameUpdate(unsigned char *buffer,
                             frost_int dst_x_pos,
                             frost_int dst_y_pos,
                             frost_uint width,
                             frost_uint height,
                             frost_uint window_width,
                             frost_uint window_height,
                             frost_bool clear_screen
                             );


#endif /* CABOT_ICE_ICE_OSD_H_INCLUDED*/
