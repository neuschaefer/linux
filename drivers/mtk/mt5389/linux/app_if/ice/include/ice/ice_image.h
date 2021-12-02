/**
 * Copyright 2008 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief ICE Interface for decoding, displaying and manipulating JPEG images
 *        using hardware capabilities.
 *
 * @note If decoding of JPEGs by hardware is not supported, the functions
 *       in this interface do not need to be implemented.
 */

#ifndef CABOT_ICE_ICE_IMAGE_H_INCLUDED
#define CABOT_ICE_ICE_IMAGE_H_INCLUDED

#include "frost/frost_basictypes.h"

#include "ice/ice_exif.h"

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * Types of image zoom modes.
     */
    typedef enum ICE_ImageZoomMode
    {
       /**
        * 50 % zoom.
        */
        ice_image_zoom_50_percent,

       /**
        * no zoom.
        */
        ice_image_zoom_100_percent,

       /**
        * 150 % zoom.
        */
        ice_image_zoom_150_percent,

       /**
        * 200 % zoom.
        */
        ice_image_zoom_200_percent,

       /**
        * 300 % zoom.
        */
        ice_image_zoom_300_percent,

        /**
         * 400 % zoom.
         */
        ice_image_zoom_400_percent,

    } ICE_ImageZoomMode;


    /**
     * Types of image zoom movements.
     */
    typedef enum ICE_ImageZoomMovement
    {
       /**
        * no move.
        */
        ice_image_no_move,

       /**
        * move up.
        */
        ice_image_zoom_move_up,

       /**
        * move down.
        */
        ice_image_zoom_move_down,

       /**
        * move left.
        */
        ice_image_zoom_move_left,

       /**
        * move right.
        */
        ice_image_zoom_move_right,

    } ICE_ImageZoomMovement;

/**
 * Types of image rotations.
 */
typedef enum ICE_ImageRotation
{
   /**
    * 90 degrees clockwise rotation.
    */
    ice_image_rotate_90_degrees_clockwise,

   /**
    * 90 degrees anticlockwise rotation.
    */
    ice_image_rotate_90_degrees_anticlockwise

} ICE_ImageRotation;

/**
 * Possible decoding results.
 */
typedef enum
{
    /**
     * Decoding successful.
     */
    ice_image_decode_successful,

    /**
     * Decoding successful.
     */
    ice_image_decode_failed,

    /**
     * Decoding successful.
     */
    ice_image_decode_aborted

} ICE_ImageDecodeResult;

/**
* @brief Read the contents of a JPEG file and decode it.
*
* Two passed buffers are used for double buffering; one keeps
* the decompressed image while the other is used as temporary storage.
*
* The passed width and height hints are used to compromise
* quality for speed. When possible, decoding will create a smaller output
* image, which is done more quickly. This is usesul when displaying thumbnails.
*
* @param filename path to JPEG file
* @param target_size_width  width of area where the decoded image will be displayed.
* @param target_size_height height of area where the decoded image will be displayed.
* @param is_thumbnail defines if image is thumbnail or not.
* @param is_exact_path defines if image is on exact path or not.
*
* @warning All parameters should be non-zero.
*
* @return ice_image_decode_successful if decode succeeds, ice_image_decode_failed if decode fails, ice_image_decode_aborted if decode is aborted
*/

ICE_ImageDecodeResult ICE_ImageReadAndDecode(char const *filename,
                                                        frost_int target_size_width,
                                                        frost_int target_size_height,
                                                        frost_bool is_thumbnail,
                                                        frost_bool is_exact_path);

/**
 * @brief Display image in the given positon.
 *
 * @param x  top x position of area where the decoded image will be displayed.
 * @param y  top y position of area where the decoded image will be displayed.
 * @param width  width of area where the decoded image will be displayed.
 * @param height  height of area where the decoded image will be displayed.
 *
 */
void ICE_ImageDisplay(frost_int x, frost_int y, frost_int width, frost_int height);


/**
 * @brief Hide current displayed image.
 *
 * @return true if the requested operation completes successfully,
 *         false otherwise
 */
frost_bool ICE_ImageHide(void);

/**
 * @brief Stop reading or decoding.
 *
 * @return frost_true if aborted successfully, otherwise frost_false.
 */
frost_bool ICE_ImageAbortDecompress(frost_bool clear);

/**
 * @brief Get the width of the image.
 *
 * @note The returned value must be updated after opertaions
 *       such as rotation and resizing.
 *
 * @return The width in pixels.
 */
frost_int ICE_ImageGetWidth(void);

/**
 * @brief Get the height of the image.
 *
 * @note The returned value must be updated after opertaions
 *       such as rotation and resizing.
 *
 * @return The height in pixels.
 */
frost_int ICE_ImageGetHeight(void);

/**
 * @brief Rotate the image.
 *
 * @param rotation Type of rotation.
 */
void ICE_ImageRotate(ICE_ImageRotation rotation);

/**
* @brief Resize the image.
*
* Resize the master image and store result in display buffer.
*
* @param dest_w width of destination area
* @param dest_h height of destination area
* @param filter smooth resizing if true, nearest-neighbour otherwise.
*
* @return frost_true if the requested resize operation completes successfully,
*         frost_false otherwise
*/
frost_bool ICE_ImageResize(frost_int dest_w, frost_int dest_h, frost_bool filter);

/**
* @brief Resize the image to fit the area.
*
* Resize the master image and store result in display buffer.
*
* @param dest_w width of destination area
* @param dest_h height of destination area
* @param filter smooth resizing if true, nearest-neighbour otherwise.
*
* @warning  dest_w and dest_h should all be non-zero.
*
* @return true if the requested resize operation completes successfully,
*         false otherwise
*/
frost_bool ICE_ImageResizeToFit(frost_int dest_w, frost_int dest_h, frost_bool filter);


frost_bool ICE_ImageReadEXIF(char const *filename, ICE_MediaPlayerEXIFDetails * ice_exif_details);
/**
 * @brief Zoom the image.
 *
 * @param zoom_mode Type of zoom.
 */
frost_bool ICE_ImageZoom(ICE_ImageZoomMode zoom_mode);

/**
 * @brief Move on zoomed image.
 *
 * @param move Type of move.
 */
void ICE_ImageZoomMove(ICE_ImageZoomMovement move);

/**
* @brief Show/Hide boot up logo .
*
* Resize the master image and store result in display buffer.
*
* @param enable showing or hiding the logo
* @param timeout indicate how many milliseconds to keep logo on screen
*
* @warning  timeout is not in use when enable parameter is false.
*
* @return true if showing or hiding the logo is succesfull, false otherwise
*         
*/
frost_bool ICE_ImageShowBootupImage(frost_bool enable, frost_uint32 timeout);


#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_IMAGE_H_INCLUDED*/
