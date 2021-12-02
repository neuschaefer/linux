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
/* (c) 2008 Koninklijke Philips Electronics N.V., All rights reserved
 * 
 * This source code and any compilation or derivative thereof is the
 * proprietary information of Koninklijke Philips Electronics N.V. and is
 * confidential in nature.
 * Under no circumstances is this software to be exposed to or placed under an
 * Open Source License of any type without the expressed written permission of
 * Koninklijke Philips Electronics N.V.
 */

#include "papi_types.h"

/**
 *   @file papi_conn.h
 */
/** @defgroup papi_conn  */

/*
 *  interface cmdmx of type IPlfApiCmDmx
 */

/** @defgroup papi_conn_cmdmx papi_conn_cmdmx: CmDmx
  * @ingroup papi_conn */

/** @defgroup papi_conn_cmdmx_Datatypes Datatypes 
  * @ingroup papi_conn_cmdmx */
/**
 *   @defgroup papi_conn_cmdmx_CmDmxError Error codes CmDmx
 *   @ingroup papi_conn_cmdmx_Datatypes
 *   Error codes for CmDmx.
 *   @{ */
#define papi_conn_cmdmx_CmDmxClientStreamNotAvailable 2049
#define papi_conn_cmdmx_CmDmxPlatformStreamTypeNotSupported 2050
/*   @} */

/** @defgroup papi_conn_cmdmx_Functions Functions 
  * @ingroup papi_conn_cmdmx */

/**  @ingroup papi_conn_cmdmx_Functions */
/**  Allocate a client stream. A client stream represents a filtered output stream from the demultiplexer that is forwarded to a client-side memory stream buffer (inside the application). */ 
/**  @param streamid Output parameter which identifies the client stream that has been allocated. */
/**  @param retval Returns Ok or Client Stream Not Available See datatype \ref papi_conn_cmdmx_CmDmxError. */
extern FResult papi_conn_cmdmx_AllocateClientStream( int * streamid, Nat32 * retval );

/**  @ingroup papi_conn_cmdmx_Functions */
/**  Returns the number of client streams that can still be allocated. */ 
/**  @param AvailClientStr Output parameter returning the number of client streams that can still be allocated. */
/**  @param retval Standard return value. */
extern FResult papi_conn_cmdmx_GetAvailableClientStreams( Nat32 * AvailClientStr, Nat32 * retval );

/**  @ingroup papi_conn_cmdmx_Functions */
/**  Associate an allocated client stream with a memory stream buffer that has been reserved by the application. The buffer must be in shared memory, not system memory! */ 
/**  @param streamid Input parameter identifying the client stream that has been allocated. */
/**  @param bufferType Input parameter indicating the buffer type. */
/**  @param bufferSize Input parameter indicating the buffer size, in bytes. */
/**  @param granularity Input parameter indicating the grain size at which notification of data is requested by the application, in bytes. */
/**  @param buffer Input parameter with the pointer to the stream buffer in memory */
/**  @param retval Standard return value. */
extern FResult papi_conn_cmdmx_SetBuffer( int streamid, tmCmDmx_BufferType_t bufferType, Nat32 bufferSize, Nat32 granularity, tmCmDmx_Buffer_t buffer, Nat32 * retval );

/**  @ingroup papi_conn_cmdmx_Functions */
/**  Release the lock on a data packet that has been notified. */ 
/**  @param streamid Input parameter with the stream id. */
/**  @param datacookie Value that has been passed with the corresponding notification function. */
/**  @param retval Standard return value. */
extern FResult papi_conn_cmdmx_Free( int streamid, Nat32 datacookie, Nat32 * retval );

/**  @ingroup papi_conn_cmdmx_Functions */
/**  Returns the types of platform streams that the demultiplexer can filter out. */ 
/**  @param SuppPlatformStreams Output parameter returning the types of platform streams that the demultiplexer can filter out. The value is a bit-mask that is constructed by OR-ing the values of enum #_tmCmDmx_PlatformStreamType_t. */
/**  @param retval Standard return value. */
extern FResult papi_conn_cmdmx_GetSupportedPlatformStreams( tmCmDmx_SuppPlatformStreamTypes_t * SuppPlatformStreams, Nat32 * retval );

/**  @ingroup papi_conn_cmdmx_Functions */
/**  Get the platform stream of specified type. A platform stream represents a filtered output stream from the demultiplexer that is forwarded inside the platform. */ 
/**  @param platformStreamType Input parameter identifying the type of platform stream that is requested. */
/**  @param streamid Output parameter which identifies the requested platform stream of specified type. */
/**  @param retval Returns Ok or Platform Stream Not Supported See datatype \ref papi_conn_cmdmx_CmDmxError. */
extern FResult papi_conn_cmdmx_GetPlatformStream( tmCmDmx_PlatformStreamType_t platformStreamType, int * streamid, Nat32 * retval );

/**  @ingroup papi_conn_cmdmx_Functions */
/**  Start filtering for the elementary stream that has been set for this stream id. */ 
/**  @param streamid Input parameter with the stream id. */
/**  @param retval Standard return value. */
extern FResult papi_conn_cmdmx_Start( int streamid, Nat32 * retval );

/**  @ingroup papi_conn_cmdmx_Functions */
/**  Stop filtering for this stream id. */ 
/**  @param streamid Input parameter with the stream id. */
/**  @param retval Standard return value. */
extern FResult papi_conn_cmdmx_Stop( int streamid, Nat32 * retval );

/**  @ingroup papi_conn_cmdmx_Functions */
/**  Selects for a particular stream id the index of the elementary stream that has to be filtered out by the demultiplexer. */ 
/**  @param streamid Input parameter with the stream id. */
/**  @param streamIndex Input parameter that selects a particular elementary stream. */
/**  @param retval Standard return value. */
extern FResult papi_conn_cmdmx_SetStreamIndex( int streamid, Nat32 streamIndex, Nat32 * retval );

/**  @ingroup papi_conn_cmdmx_Functions */
/**  Returns stream information for the individual elementary streams in the incoming multiplexed stream. */ 
/**  @param streamIndex Input parameter that selects a particular elementary stream, between [1..StreamMapSize] */
/**  @param langSize Input parameter indicating the maximum allowed size of the streamLanguage output string. */
/**  @param StreamType Output parameter returning the type of the elementary stream. */
/**  @param streamLanguage Output parameter returning the stream language in ISO 639-2 format. */
/**  @param retval Standard return value. */
extern FResult papi_conn_cmdmx_GetStreamInfo( Nat32 streamIndex, Nat16 langSize, tmCmDmx_StreamType_t * StreamType, char * streamLanguage, Nat32 * retval );

/**  @ingroup papi_conn_cmdmx_Functions */
/**  Returns the number of elementary streams that are contained in the incoming multiplexed stream. */ 
/**  @param NumberOfStreams Output parameter returning the number of elementary streams in the multiplexed stream. */
/**  @param retval Standard return value. */
extern FResult papi_conn_cmdmx_GetStreamMapSize( Nat32 * NumberOfStreams, Nat32 * retval );



/*
 *  interface imgdec of type IPlfApiImageDec
 */

/** @defgroup papi_conn_imgdec papi_conn_imgdec: ImageDec
  * The function of an Image Decoding logical component is to decode encoded images. It can decode full images to a requested format, get thumbnails in a requested format via decoding or extraction, and extract raw meta-data. It will scale the image after decoding when the original and requested resolutions differ.<br>
An Image Decoding logical component takes encoded image data as input and sends the decoded image to the client via a notification function. The decoder is based on a logical streaming model and can therefore process the provided input data only once for the input. When input data needs to be processed a second time, the input data has to be provided to the component anew.<br>
The image decoder is modeled using the following processes:
<ul>
<li> Obtainment of basic encoded image information i.e. image resolution, aspect ratio and orientation. This process can be triggered by calling the GetOrgFullResolution function and needs to be called before any requests to decode the main image can be made.</li>
<li> Detection of thumbnail presence and obtainment of basic thumbnail image information i.e. thumbnail resolution. This process can be triggered by calling the GetThumbnailPresent function and needs to be called before any requests to decode or extract a thumbnail can be made.</li>
<li> Main image decoding process; this process can be triggered by calling the DecodeImage function.</li>
<li> Thumbnail image decoding/extraction process; this process can be triggered by calling the DecodeThumbnail function.</li>
<li> Raw meta-data extraction, if this is possible depends on the image format and the policy of the platform (not all meta-data might be available for retrieval by the client as raw data). Whether raw meta-data, for retrieval by the client is present can be queried by calling the GetMetaDataPresent function. If this function returns True, the raw meta-data extraction process can be triggered by calling the GetRawMetaData function.</li>
</ul>
All the processes mentioned above when triggered wait for input data to arrive in order to start processing. How long a process will wait before indicating a failure is context/implementation dependent and not part of this specification.
  * @ingroup papi_conn */

/** @defgroup papi_conn_imgdec_Functions Functions 
  * @ingroup papi_conn_imgdec */

/**  @ingroup papi_conn_imgdec_Functions */
/**  This function returns the byte alignment required of image buffer lines. The byte alignment poses restrictions on the stride the client specifies for image buffers (see DecodeImage and DecodeThumbnail). */ 
/**  @param BufAlignment Output parameter returning the byte alignment of the lines of an image buffer. */
/**  @param retval Standard Return Value. */
extern FResult papi_conn_imgdec_GetBufAlignment( Nat32 * BufAlignment, Nat32 * retval );

/**  @ingroup papi_conn_imgdec_Functions */
/**  @param requestedHorRes The requested width: the horizontal resolution of the returned image in number of pixels per line. */
/**  @param requestedVertRes The requested height: the vertical resolution of the returned image in number of lines. */
/**  @param stride The stride (in bytes) of the image. */
/**  @param formatClass The requested pixel format class. */
/**  @param formatType The requested pixel format type. */
/**  @param useImageArea Flag indicating if the encoded image should be partially decoded (True) or not (False). The area of the encoded image to be decoded is indicated by requestedImageArea. */
/**  @param requestedImageArea The area of the encoded image to be decoded if useImageArea equals True. */
/**  @param bufferSize The size of the buffer where image is to be decoded in bytes. */
/**  @param Buffer Pointer to the first element of the client's buffer where image is to be decoded. */
/**  @param retval Standard Return Value. */
extern FResult papi_conn_imgdec_DecodeImage( Nat32 requestedHorRes, Nat32 requestedVertRes, Nat32 stride, tmPixFmtCls_t formatClass, tmPixFmtType_t formatType, Bool useImageArea, tmImageDec2_ImageArea_t requestedImageArea, Nat32 bufferSize, tmImageDec_Buffer_t Buffer, Nat32 * retval );

/**  @ingroup papi_conn_imgdec_Functions */
/**  This function requests the decoding or extraction of a thumbnail in the requested format and resolution. When the requested resolution differs from the resolution of the encoded image, the image is scaled to the requested resolution. It is the client's responsibility to assure that the resulting scale factor including the effects of applied rotation (see SetRotationMode) is within the supported range (see GetScaleRange). There is no support for pixel formats requiring multiple buffers (e.g. one for Y, one for UV). */ 
/**  @param requestedHorRes The requested witdth: the horizontal resolution of the returned image in number of pixels per line. */
/**  @param requestedVertRes The requested height: the vertical resolution of the returned image in number of lines. */
/**  @param stride The stride (in bytes) of the thumbnail. */
/**  @param formatClass The requested pixel format class. */
/**  @param formatType The requested pixel format type. */
/**  @param bufferSize The size of the buffer where thumbnail is to be decoded or extracted in bytes. */
/**  @param Buffer Pointer to the first element of the client's buffer where thumbnail is to be decoded or extracted. */
/**  @param retval Standard Return Value. */
extern FResult papi_conn_imgdec_DecodeThumbnail( Nat32 requestedHorRes, Nat32 requestedVertRes, Nat32 stride, tmPixFmtCls_t formatClass, tmPixFmtType_t formatType, Nat32 bufferSize, tmImageDec_Buffer_t Buffer, Nat32 * retval );

/**  @ingroup papi_conn_imgdec_Functions */
/**  This function returns the supported format classes to which an image can be decoded. */ 
/**  @param FormatClasses Output parameter returning the supported format classes to which an image can be decoded. The value is a bit-mask that is constructed by OR-ing the values of enum _tmPixFmtCls_t. */
/**  @param retval Standard Return Value. */
extern FResult papi_conn_imgdec_GetSuppFormatClasses( tmPixFmtClsSet_t * FormatClasses, Nat32 * retval );

/**  @ingroup papi_conn_imgdec_Functions */
/**  This function returns the supported format types of the specified pixel format class to which an image can be decoded. */ 
/**  @param formatClass The pixel class for which the supported pixel format types are requested. */
/**  @param FormatTypes Output parameter returning the supported pixel format types for the pixel format class. The value is a bit-mask that is constructed by OR-ing the values of enum _tmPixYuvFmt_t, _tmPixRgbFmt_t or _tmPixClutFmt_t depending on the format class. */
/**  @param retval Standard Return Value. */
extern FResult papi_conn_imgdec_GetSuppFormatTypes( tmPixFmtCls_t formatClass, tmPixFmtTypeSet_t * FormatTypes, Nat32 * retval );

/**  @ingroup papi_conn_imgdec_Functions */
/**  This function returns the supported image formats. */ 
/**  @param Formats Output parameter returning the supported image formats. The value is a bit-mask that is constructed by OR-ing the values of enum _tmImageDec2_ImageFormat_t. */
/**  @param retval Standard Return Value. */
extern FResult papi_conn_imgdec_GetSuppImageFormats( tmImageDec2_ImageFormats_t * Formats, Nat32 * retval );

/**  @ingroup papi_conn_imgdec_Functions */
/**  This function returns whether meta-data is present that can be retrieved by the client as raw meta-data. When meta-data is present, the raw meta-data can be retrieved by subscribing to OnMetaDataFound and OnEndMetaData and calling GetRawMetaData. */ 
/**  @param MetaDataPresent Output parameter returning whether extracted raw meta-data is present (True) or not (False). */
/**  @param retval  */
extern FResult papi_conn_imgdec_GetMetaDataPresent( Bool * MetaDataPresent, Nat32 * retval );

/**  @ingroup papi_conn_imgdec_Functions */
/**  This function triggers the retrieval of the original resolution of the full image. Before calling this function, the client needs to be subscribed to OnResolutionObtained. */ 
/**  @param retval Standard Return Value. */
extern FResult papi_conn_imgdec_GetOrgFullResolution( Nat32 * retval );

/**  @ingroup papi_conn_imgdec_Functions */
/**  This function returns the aspect ratio of the original image. */ 
/**  @param Numerator Output parameter returning the numerator of the aspect ratio of the input. */
/**  @param Denominator Output parameter returning the denominator of the aspect ratio of the input. */
/**  @param retval Standard Return Value. */
extern FResult papi_conn_imgdec_GetOrgImageAspectRatio( Nat16 * Numerator, Nat16 * Denominator, Nat32 * retval );

/**  @ingroup papi_conn_imgdec_Functions */
/**  This function returns the orientation of the image as described in the rotation bits of the image. */ 
/**  @param Orientation Output parameter providing the orientation of the image. */
/**  @param retval Standard Return Value. */
extern FResult papi_conn_imgdec_GetOrgImageOrientation( tmImageDec2_Orientation_t * Orientation, Nat32 * retval );

/**  @ingroup papi_conn_imgdec_Functions */
/**  This function returns the original resolution of the thumbnail. */ 
/**  @param HorRes Output parameter returning the original horizontal resolution of the thumbnail received via the input. The resolution is expressed in number of pixels per line. */
/**  @param VertRes Output parameter returning the original vertical resolution of the thumbnail received via the input. The resolution is expressed in number of lines. */
/**  @param retval Standard Return Value. */
extern FResult papi_conn_imgdec_GetOrgThumbnailResolution( Nat32 * HorRes, Nat32 * VertRes, Nat32 * retval );

/**  @ingroup papi_conn_imgdec_Functions */
/**  This function returns the rotation modes supported by the Image Decoder. */ 
/**  @param Modes Output parameter providing rotation modes supported by the image decoder. The value is a bit-mask that is constructed by OR-ing the values of enum _tmImageDec2_RotationMode_t. */
/**  @param retval Standard Return Value. */
extern FResult papi_conn_imgdec_GetSupportedRotationModes( tmImageDec2_RotationModes_t * Modes, Nat32 * retval );

/**  @ingroup papi_conn_imgdec_Functions */
/**  This function requests the retrieval of raw meta-data. Before calling this function, a client needs to check if meta-data is present by calling GetMetaDataPresent. To receive the meta-data, the client has to subscribe to tmImageDec2_MetaDataFound and tmImageDec2_EndMetaData. After calling this function, extracted meta-data will be notified by calling the OnMetaDataFound function of the client one or more times. When no more meta-data is present, the OnEndMetaData function of the client will be called. */ 
/**  @param ImageFormat Output parameter indicating the format of the image. */
/**  @param retval Standard Return Value. */
extern FResult papi_conn_imgdec_GetRawMetaData( tmImageDec2_ImageFormat_t * ImageFormat, Nat32 * retval );

/**  @ingroup papi_conn_imgdec_Functions */
/**  This function returns the maximal horizontal and vertical resolution that images can be decoded to. */ 
/**  @param MaxHorRes Output parameter returning the maximal horizontal resolution. */
/**  @param MaxVertRes Output parameter returning the maximal vertical resolution. */
/**  @param retval Standard Return Value. */
extern FResult papi_conn_imgdec_GetMaxResolution( Nat32 * MaxHorRes, Nat32 * MaxVertRes, Nat32 * retval );

/**  @ingroup papi_conn_imgdec_Functions */
/**  This function returns the rotation mode used for decoding the main image and the thumbnail. */ 
/**  @param Mode The rotation mode that is currently set. */
/**  @param Orientation The orientation that is currently set. This orientation is only applied if the rotation mode specifies forced rotation. */
/**  @param retval Standard Return Value. */
extern FResult papi_conn_imgdec_GetRotationMode( tmImageDec2_RotationMode_t * Mode, tmImageDec2_Orientation_t * Orientation, Nat32 * retval );

/**  @ingroup papi_conn_imgdec_Functions */
/**  This function sets the rotation mode used for decoding the main image and the thumbnail. If the mode parameter is set to forced rotation mode, the orientation parameter specifies the orientation for the rotation. For all other rotation modes the orientation parameter is ignored. */ 
/**  @param mode The rotation mode to be used. */
/**  @param orientation The orientation to be used if the rotation mode specifies forced rotation. */
/**  @param retval Standard Return Value. */
extern FResult papi_conn_imgdec_SetRotationMode( tmImageDec2_RotationMode_t mode, tmImageDec2_Orientation_t orientation, Nat32 * retval );

/**  @ingroup papi_conn_imgdec_Functions */
/**  This function returns the minimal and maximal horizontal and vertical scale factors in percentages that images can be scaled to after decoding. A number higher than 100 indicates up scaling, equal to 100 indicates no scaling, and lower than 100 indicates down scaling. */ 
/**  @param MinHorPercentage Output parameter returning the minimal horizontal scale factor in percentages. */
/**  @param MinVertPercentage Output parameter returning the minimal vertical scale factor in percentages. */
/**  @param MaxHorPercentage Output parameter returning the maximal horizontal scale factor in percentages. */
/**  @param MaxVertPercentage Output parameter returning the maximal vertical scale factor in percentages. */
/**  @param retval Standard Return Value. */
extern FResult papi_conn_imgdec_GetScaleRange( Nat32 * MinHorPercentage, Nat32 * MinVertPercentage, Nat32 * MaxHorPercentage, Nat32 * MaxVertPercentage, Nat32 * retval );

/**  @ingroup papi_conn_imgdec_Functions */
/**  This function triggers the detection of thumbnail presence. Before calling this function, the client needs to be subscribed to OnThumbnailPresence. */ 
/**  @param retval Standard Return Value. */
extern FResult papi_conn_imgdec_GetThumbnailPresent( Nat32 * retval );



/*
 *  interface urlsrcjpg of type IPlfApiUrlSrc
 */

/** @defgroup papi_conn_urlsrcjpg papi_conn_urlsrcjpg: UrlSrc
  * The function of a URL Source is to stream data identified by a URL into the platform. A URL has the following format:scheme://scheme specific format. The scheme, also called protocol, is used for accessing the data. Examples are: FILE, HTTP, and FTP. For the HTTP scheme, a URL will look like: http://hostport[/path][?search].<br>
Based on the protocol, capabilities of the server, and content type of the data special operations can be applied to the data like pause, fast forward, go to position, and so on. These special operations are often referred to as trick modes. When a new URL is requested, the available trick modes may change. Clients have to take this into account. Note that linear playback of the data is always supported.<br>
Accessing a server that provides a URL can be slow, especially when it points to a remote server. Therefore, gaining access is done in two steps. First, the URL is parsed to see if the requested scheme is available and the format of the URL is correct. This can be done locally and fast. Secondly, the server and the actual data will be accessed which might require remote access and can be slow.<br>
When a URL identifies data on a remote server, the connection to the server may be lost. This will be signaled to the client. A number of retries will be performed before a definite connection lost notification is given. Furthermore, delays in the transmission can occur. These will not be signaled to the client but gracefully handled by the platform.<br>
Data streaming from a server is subject to jitter. Therefore, an input buffer is modeled as part of URL Source. This buffer is used to compensate for this jitter, that is, to ensure a continuous playback. When the URL Source is put in play state, data streaming to the output only starts once the amount of data in the buffer has reached a level of nominal + hysteresis. Then, the fullness level of the buffer fluctuates around (below and above) the nominal level. The detailed buffering model during trick modes is scheme-specific and not described further in this document.<br>
The ratio between the nominal level and the total buffer size is URL scheme-specific. The total buffer size itself depends on the maximum possible jitter in the system and on the bit rate of the selected URL. The fullness level of the buffer is observable to the client (in % of nominal level).
  * @ingroup papi_conn */

/** @defgroup papi_conn_urlsrcjpg_Datatypes Datatypes 
  * @ingroup papi_conn_urlsrcjpg */
/**
 *   @defgroup papi_conn_urlsrcjpg_UrlSrcError Error codes UrlSrc
 *   @ingroup papi_conn_urlsrcjpg_Datatypes
 *   Error codes for UrlSrc.
 *   @{ */
#define papi_conn_urlsrcjpg_UrlSrcInvalidUrl 2049
#define papi_conn_urlsrcjpg_UrlSrcSchemeNotSupported 2050
#define papi_conn_urlsrcjpg_UrlSrcAborted 2051
#define papi_conn_urlsrcjpg_UrlSrcNotAborted 2052
#define papi_conn_urlsrcjpg_UrlSrcEndOfStream 2053
#define papi_conn_urlsrcjpg_UrlSrcConnectionLost 2054
/*   @} */

/** @defgroup papi_conn_urlsrcjpg_Functions Functions 
  * @ingroup papi_conn_urlsrcjpg */

/**  @ingroup papi_conn_urlsrcjpg_Functions */
/**  This function is intended to abort an OpenUrl or an automatic connection retry. Calling this function has no effect when there is no attempt to set a URL or to reconnect to a URL. The function is synchronous. When it aborted an OpenUrl or an automatic connection retry, it notifies the client with a synchronous OnActionAborted notification before completion and makes the accessState of the currentUrl equal to AsNoAccess. */ 
/**  @param retval Return value can be either Ok or Not Aborted. See datatype \ref papi_conn_urlsrcjpg_UrlSrcError. */
extern FResult papi_conn_urlsrcjpg_AbortAction( Nat32 * retval );

/**  @ingroup papi_conn_urlsrcjpg_Functions */
/**  Returns the available step modes. */ 
/**  @param AvailStepModes Output parameter indicating the set of trick modes that are supported. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcjpg_GetAvailStepModes( tmUrlSrc_StepModes_t * AvailStepModes, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcjpg_Functions */
/**  If the current URL is accessible, this function returns the current buffer fullness level. Otherwise it returns 0. */ 
/**  @param BufferFullness Output parameter with the percentage with which the playback buffer is filled. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcjpg_GetBufferFullness( Nat32 * BufferFullness, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcjpg_Functions */
/**  Close the connection with the current url. */ 
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcjpg_CloseUrl( Nat32 * retval );

/**  @ingroup papi_conn_urlsrcjpg_Functions */
/**  Sets the URL that identifies the data. The OpenUrl function will try to make the connection to the location. It will generate a notification OnNotAccessible or OnAccessible on failure or on success.<br>
During the attempt to access the URL, the state of the UrlSrc is AsAccessing. When the UrlSrc is in this state no other OpenUrl may be called. This state is not explicitly observable but only implicitly by deducing it from the notifications. */ 
/**  @param url Input parameter with the url as a null-terminated string. */
/**  @param timeOut Input parameter defining the maximum number of microseconds that establishing a connection may take. */
/**  @param retval Return value can be either Ok, Invalid Url, or Scheme not supported. See datatype \ref papi_conn_urlsrcjpg_UrlSrcError. */
extern FResult papi_conn_urlsrcjpg_OpenUrl( char * url, Nat32 timeOut, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcjpg_Functions */
/**  Pause playback of the current url. */ 
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcjpg_Pause( Nat32 * retval );

/**  @ingroup papi_conn_urlsrcjpg_Functions */
/**  Returns the current position in the stream that is playing. */ 
/**  @param Time Output parameter indicating the current position in the stream, in milliseconds. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcjpg_GetPosition( Nat32 * Time, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcjpg_Functions */
/**  Sets the position to the specified time. It generates the notification OnSetPositionCompleted after the current position is set to the actual rounded position. */ 
/**  @param requestedTime Input parameter that specifies the requested position, in milliseconds. */
/**  @param rounding Input parameter indicating the rounding technique that has to be used to determine the actual position in the stream. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcjpg_SetPosition( Nat32 requestedTime, tmUrlSrc_Rounding_t rounding, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcjpg_Functions */
/**  Resumes playback at the point in the stream where Pause halted it or from the position set by the last SetPosition call if SetPosition is called after Pause. */ 
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcjpg_Resume( Nat32 * retval );

/**  @ingroup papi_conn_urlsrcjpg_Functions */
/**  Returns the different schemes that are supported (ftp, http, rtsp, ..). */ 
/**  @param SupportedSchemes Output parameter indicating the set of the schemes that are supported. The value is a bit-mask that is constructed by OR-ing the values of enum #_tmUrlSrc_Scheme_t. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcjpg_GetSuppSchemes( tmUrlSrc_Schemes_t * SupportedSchemes, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcjpg_Functions */
/**  Returns the current seek mode. */ 
/**  @param seekMode The current seek mode. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcjpg_GetSeekMode( tmUrlSrc_SeekMode_t * seekMode, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcjpg_Functions */
/**  Sets the seek mode to be used. */ 
/**  @param seekMode The desired seek mode. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcjpg_SetSeekMode( tmUrlSrc_SeekMode_t seekMode, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcjpg_Functions */
/**  Returns (1) whether reverse speeds are allowed and (2) the lower and upper bounds of the speed that can be handled for the content identified by the URL. The bounds are determined as follows: minSpeed = (*pMinSpeedNum / *pMinSpeedDenom) and maxSpeed = (*pMaxSpeedNum / *pMaxSpeedDenom). */ 
/**  @param ReverseSupp Output parameter indicating whether reverse speed is supported. */
/**  @param MinSpeedNum Output parameter with the minimal playback speed, numerator part. */
/**  @param MinSpeedDenom Output parameter with the minimal playback speed, denominator part. */
/**  @param MaxSpeedNum Output parameter with the maximum playback speed, numerator part. */
/**  @param pMaxSpeedDenom Output parameter with the maximum playback speed, denominator part. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcjpg_GetSpeedRange( Bool * ReverseSupp, Nat8 * MinSpeedNum, Nat8 * MinSpeedDenom, Nat8 * MaxSpeedNum, Nat8 * pMaxSpeedDenom, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcjpg_Functions */
/**  Returns the playback speed as follows: requestedSpeed = (*pRequestedSpeedNum / *pRequestedSpeedDenom) and the speed that was actually set as follows: actualSpeed = (*pActualSpeedNum / *pActualSpeedDenom). */ 
/**  @param requestedSpeedNum Output parameter with the playback speed that was requested, numerator part. */
/**  @param requestedSpeedDenom Output parameter with the playback speed that was requested, denominator part. */
/**  @param ActualSpeedNum Output parameter with the actual playback speed, numerator part. */
/**  @param ActualSpeedDenom Output parameter with the actual playback speed, denominator part. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcjpg_GetSpeed( Int8 * requestedSpeedNum, Nat8 * requestedSpeedDenom, Int8 * ActualSpeedNum, Nat8 * ActualSpeedDenom, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcjpg_Functions */
/**  Sets the playback speed as follows: requestedSpeed = (requestedSpeedNum/requestedSpeedDenom). Returns the speed that was actually set as follows: actualSpeed = (actualSpeedNum/actualSpeedDenom). */ 
/**  @param requestedSpeedNum Input parameter with the requested playback speed, numerator part. */
/**  @param requestedSpeedDenom Input parameter with the requested playback speed, denominator part. */
/**  @param ActualSpeedNum Output parameter with the actual playback speed, numerator part. */
/**  @param ActualSpeedDenom Output parameter with the actual playback speed, denominator part. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcjpg_SetSpeed( Int8 requestedSpeedNum, Nat8 requestedSpeedDenom, Int8 * ActualSpeedNum, Nat8 * ActualSpeedDenom, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcjpg_Functions */
/**  This function starts streaming the data whenever data becomes available. A consecutive call to start does not have any effect on the URL currently being streamed. */ 
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcjpg_Start( Nat32 * retval );

/**  @ingroup papi_conn_urlsrcjpg_Functions */
/**  Performs playback of the previous access unit. If there is no previous access unit, a return value indicating this is returned. The function returns when there was a noticeable effect for the client.<br>
When the connection was lost, the function immediately returns without waiting for retries */ 
/**  @param retval Return value can be either Ok, End Of Stream, or Connection Lost. See datatype \ref papi_conn_urlsrcjpg_UrlSrcError. */
extern FResult papi_conn_urlsrcjpg_StepBackward( Nat32 * retval );

/**  @ingroup papi_conn_urlsrcjpg_Functions */
/**  Performs playback of the next access unit. If there is no next access unit, a return value indicating this is returned. The function returns when there was a noticeable effect for the client.<br>
When the connection was lost, the function immediately returns without waiting for retries. */ 
/**  @param retval Return value can be either Ok, End Of Stream, or Connection Lost. See datatype \ref papi_conn_urlsrcjpg_UrlSrcError. */
extern FResult papi_conn_urlsrcjpg_StepForward( Nat32 * retval );

/**  @ingroup papi_conn_urlsrcjpg_Functions */
/**  Stops playing the current URL (flushes the data that has not been decoded, and stops rendering as soon as possible). */ 
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcjpg_Stop( Nat32 * retval );

/**  @ingroup papi_conn_urlsrcjpg_Functions */
/**  If the current URL is accessible and the total duration of the data is known, this function returns that duration. Otherwise it returns 0. */ 
/**  @param TotalDuration Output parameter returning the total duration in milliseconds. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcjpg_GetTotalDuration( Nat32 * TotalDuration, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcjpg_Functions */
/**  If the current URL is accessible and the total size of the data is known, this function returns that size. Otherwise it returns 0. */ 
/**  @param TotalSize Output parameter returning the total size in bytes. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcjpg_GetTotalSize( Nat64 * TotalSize, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcjpg_Functions */
/**  Returns the URL that was set as a null-terminated string and the actual size of the URL. */ 
/**  @param size Input parameter indicating the maximum size of the returned string, in bytes. */
/**  @param ActualSize Output parameter indicating the actual size of the returned string, in bytes. */
/**  @param url Output parameter returning the url that was set, as a null-terminated string. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcjpg_GetUrl( Nat16 size, Nat32 * ActualSize, Nat8 * url, Nat32 * retval );



/*
 *  interface urlsrcmp3 of type IPlfApiUrlSrc
 */

/** @defgroup papi_conn_urlsrcmp3 papi_conn_urlsrcmp3: UrlSrc
  * The function of a URL Source is to stream data identified by a URL into the platform. A URL has the following format:scheme://scheme specific format. The scheme, also called protocol, is used for accessing the data. Examples are: FILE, HTTP, and FTP. For the HTTP scheme, a URL will look like: http://hostport[/path][?search].<br>
Based on the protocol, capabilities of the server, and content type of the data special operations can be applied to the data like pause, fast forward, go to position, and so on. These special operations are often referred to as trick modes. When a new URL is requested, the available trick modes may change. Clients have to take this into account. Note that linear playback of the data is always supported.<br>
Accessing a server that provides a URL can be slow, especially when it points to a remote server. Therefore, gaining access is done in two steps. First, the URL is parsed to see if the requested scheme is available and the format of the URL is correct. This can be done locally and fast. Secondly, the server and the actual data will be accessed which might require remote access and can be slow.<br>
When a URL identifies data on a remote server, the connection to the server may be lost. This will be signaled to the client. A number of retries will be performed before a definite connection lost notification is given. Furthermore, delays in the transmission can occur. These will not be signaled to the client but gracefully handled by the platform.<br>
Data streaming from a server is subject to jitter. Therefore, an input buffer is modeled as part of URL Source. This buffer is used to compensate for this jitter, that is, to ensure a continuous playback. When the URL Source is put in play state, data streaming to the output only starts once the amount of data in the buffer has reached a level of nominal + hysteresis. Then, the fullness level of the buffer fluctuates around (below and above) the nominal level. The detailed buffering model during trick modes is scheme-specific and not described further in this document.<br>
The ratio between the nominal level and the total buffer size is URL scheme-specific. The total buffer size itself depends on the maximum possible jitter in the system and on the bit rate of the selected URL. The fullness level of the buffer is observable to the client (in % of nominal level).
  * @ingroup papi_conn */

/** @defgroup papi_conn_urlsrcmp3_Datatypes Datatypes 
  * @ingroup papi_conn_urlsrcmp3 */
/**
 *   @defgroup papi_conn_urlsrcmp3_UrlSrcError Error codes UrlSrc
 *   @ingroup papi_conn_urlsrcmp3_Datatypes
 *   Error codes for UrlSrc.
 *   @{ */
#define papi_conn_urlsrcmp3_UrlSrcInvalidUrl 2049
#define papi_conn_urlsrcmp3_UrlSrcSchemeNotSupported 2050
#define papi_conn_urlsrcmp3_UrlSrcAborted 2051
#define papi_conn_urlsrcmp3_UrlSrcNotAborted 2052
#define papi_conn_urlsrcmp3_UrlSrcEndOfStream 2053
#define papi_conn_urlsrcmp3_UrlSrcConnectionLost 2054
/*   @} */

/** @defgroup papi_conn_urlsrcmp3_Functions Functions 
  * @ingroup papi_conn_urlsrcmp3 */

/**  @ingroup papi_conn_urlsrcmp3_Functions */
/**  This function is intended to abort an OpenUrl or an automatic connection retry. Calling this function has no effect when there is no attempt to set a URL or to reconnect to a URL. The function is synchronous. When it aborted an OpenUrl or an automatic connection retry, it notifies the client with a synchronous OnActionAborted notification before completion and makes the accessState of the currentUrl equal to AsNoAccess. */ 
/**  @param retval Return value can be either Ok or Not Aborted. See datatype \ref papi_conn_urlsrcmp3_UrlSrcError. */
extern FResult papi_conn_urlsrcmp3_AbortAction( Nat32 * retval );

/**  @ingroup papi_conn_urlsrcmp3_Functions */
/**  Returns the available step modes. */ 
/**  @param AvailStepModes Output parameter indicating the set of trick modes that are supported. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcmp3_GetAvailStepModes( tmUrlSrc_StepModes_t * AvailStepModes, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcmp3_Functions */
/**  If the current URL is accessible, this function returns the current buffer fullness level. Otherwise it returns 0. */ 
/**  @param BufferFullness Output parameter with the percentage with which the playback buffer is filled. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcmp3_GetBufferFullness( Nat32 * BufferFullness, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcmp3_Functions */
/**  Close the connection with the current url. */ 
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcmp3_CloseUrl( Nat32 * retval );

/**  @ingroup papi_conn_urlsrcmp3_Functions */
/**  Sets the URL that identifies the data. The OpenUrl function will try to make the connection to the location. It will generate a notification OnNotAccessible or OnAccessible on failure or on success.<br>
During the attempt to access the URL, the state of the UrlSrc is AsAccessing. When the UrlSrc is in this state no other OpenUrl may be called. This state is not explicitly observable but only implicitly by deducing it from the notifications. */ 
/**  @param url Input parameter with the url as a null-terminated string. */
/**  @param timeOut Input parameter defining the maximum number of microseconds that establishing a connection may take. */
/**  @param retval Return value can be either Ok, Invalid Url, or Scheme not supported. See datatype \ref papi_conn_urlsrcmp3_UrlSrcError. */
extern FResult papi_conn_urlsrcmp3_OpenUrl( char * url, Nat32 timeOut, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcmp3_Functions */
/**  Pause playback of the current url. */ 
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcmp3_Pause( Nat32 * retval );

/**  @ingroup papi_conn_urlsrcmp3_Functions */
/**  Returns the current position in the stream that is playing. */ 
/**  @param Time Output parameter indicating the current position in the stream, in milliseconds. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcmp3_GetPosition( Nat32 * Time, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcmp3_Functions */
/**  Sets the position to the specified time. It generates the notification OnSetPositionCompleted after the current position is set to the actual rounded position. */ 
/**  @param requestedTime Input parameter that specifies the requested position, in milliseconds. */
/**  @param rounding Input parameter indicating the rounding technique that has to be used to determine the actual position in the stream. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcmp3_SetPosition( Nat32 requestedTime, tmUrlSrc_Rounding_t rounding, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcmp3_Functions */
/**  Resumes playback at the point in the stream where Pause halted it or from the position set by the last SetPosition call if SetPosition is called after Pause. */ 
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcmp3_Resume( Nat32 * retval );

/**  @ingroup papi_conn_urlsrcmp3_Functions */
/**  Returns the different schemes that are supported (ftp, http, rtsp, ..). */ 
/**  @param SupportedSchemes Output parameter indicating the set of the schemes that are supported. The value is a bit-mask that is constructed by OR-ing the values of enum #_tmUrlSrc_Scheme_t. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcmp3_GetSuppSchemes( tmUrlSrc_Schemes_t * SupportedSchemes, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcmp3_Functions */
/**  Returns the current seek mode. */ 
/**  @param seekMode The current seek mode. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcmp3_GetSeekMode( tmUrlSrc_SeekMode_t * seekMode, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcmp3_Functions */
/**  Sets the seek mode to be used. */ 
/**  @param seekMode The desired seek mode. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcmp3_SetSeekMode( tmUrlSrc_SeekMode_t seekMode, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcmp3_Functions */
/**  Returns (1) whether reverse speeds are allowed and (2) the lower and upper bounds of the speed that can be handled for the content identified by the URL. The bounds are determined as follows: minSpeed = (*pMinSpeedNum / *pMinSpeedDenom) and maxSpeed = (*pMaxSpeedNum / *pMaxSpeedDenom). */ 
/**  @param ReverseSupp Output parameter indicating whether reverse speed is supported. */
/**  @param MinSpeedNum Output parameter with the minimal playback speed, numerator part. */
/**  @param MinSpeedDenom Output parameter with the minimal playback speed, denominator part. */
/**  @param MaxSpeedNum Output parameter with the maximum playback speed, numerator part. */
/**  @param pMaxSpeedDenom Output parameter with the maximum playback speed, denominator part. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcmp3_GetSpeedRange( Bool * ReverseSupp, Nat8 * MinSpeedNum, Nat8 * MinSpeedDenom, Nat8 * MaxSpeedNum, Nat8 * pMaxSpeedDenom, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcmp3_Functions */
/**  Returns the playback speed as follows: requestedSpeed = (*pRequestedSpeedNum / *pRequestedSpeedDenom) and the speed that was actually set as follows: actualSpeed = (*pActualSpeedNum / *pActualSpeedDenom). */ 
/**  @param requestedSpeedNum Output parameter with the playback speed that was requested, numerator part. */
/**  @param requestedSpeedDenom Output parameter with the playback speed that was requested, denominator part. */
/**  @param ActualSpeedNum Output parameter with the actual playback speed, numerator part. */
/**  @param ActualSpeedDenom Output parameter with the actual playback speed, denominator part. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcmp3_GetSpeed( Int8 * requestedSpeedNum, Nat8 * requestedSpeedDenom, Int8 * ActualSpeedNum, Nat8 * ActualSpeedDenom, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcmp3_Functions */
/**  Sets the playback speed as follows: requestedSpeed = (requestedSpeedNum/requestedSpeedDenom). Returns the speed that was actually set as follows: actualSpeed = (actualSpeedNum/actualSpeedDenom). */ 
/**  @param requestedSpeedNum Input parameter with the requested playback speed, numerator part. */
/**  @param requestedSpeedDenom Input parameter with the requested playback speed, denominator part. */
/**  @param ActualSpeedNum Output parameter with the actual playback speed, numerator part. */
/**  @param ActualSpeedDenom Output parameter with the actual playback speed, denominator part. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcmp3_SetSpeed( Int8 requestedSpeedNum, Nat8 requestedSpeedDenom, Int8 * ActualSpeedNum, Nat8 * ActualSpeedDenom, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcmp3_Functions */
/**  This function starts streaming the data whenever data becomes available. A consecutive call to start does not have any effect on the URL currently being streamed. */ 
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcmp3_Start( Nat32 * retval );

/**  @ingroup papi_conn_urlsrcmp3_Functions */
/**  Performs playback of the previous access unit. If there is no previous access unit, a return value indicating this is returned. The function returns when there was a noticeable effect for the client.<br>
When the connection was lost, the function immediately returns without waiting for retries */ 
/**  @param retval Return value can be either Ok, End Of Stream, or Connection Lost. See datatype \ref papi_conn_urlsrcmp3_UrlSrcError. */
extern FResult papi_conn_urlsrcmp3_StepBackward( Nat32 * retval );

/**  @ingroup papi_conn_urlsrcmp3_Functions */
/**  Performs playback of the next access unit. If there is no next access unit, a return value indicating this is returned. The function returns when there was a noticeable effect for the client.<br>
When the connection was lost, the function immediately returns without waiting for retries. */ 
/**  @param retval Return value can be either Ok, End Of Stream, or Connection Lost. See datatype \ref papi_conn_urlsrcmp3_UrlSrcError. */
extern FResult papi_conn_urlsrcmp3_StepForward( Nat32 * retval );

/**  @ingroup papi_conn_urlsrcmp3_Functions */
/**  Stops playing the current URL (flushes the data that has not been decoded, and stops rendering as soon as possible). */ 
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcmp3_Stop( Nat32 * retval );

/**  @ingroup papi_conn_urlsrcmp3_Functions */
/**  If the current URL is accessible and the total duration of the data is known, this function returns that duration. Otherwise it returns 0. */ 
/**  @param TotalDuration Output parameter returning the total duration in milliseconds. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcmp3_GetTotalDuration( Nat32 * TotalDuration, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcmp3_Functions */
/**  If the current URL is accessible and the total size of the data is known, this function returns that size. Otherwise it returns 0. */ 
/**  @param TotalSize Output parameter returning the total size in bytes. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcmp3_GetTotalSize( Nat64 * TotalSize, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcmp3_Functions */
/**  Returns the URL that was set as a null-terminated string and the actual size of the URL. */ 
/**  @param size Input parameter indicating the maximum size of the returned string, in bytes. */
/**  @param ActualSize Output parameter indicating the actual size of the returned string, in bytes. */
/**  @param url Output parameter returning the url that was set, as a null-terminated string. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcmp3_GetUrl( Nat16 size, Nat32 * ActualSize, Nat8 * url, Nat32 * retval );



/*
 *  interface urlsrcmpeg2 of type IPlfApiUrlSrc
 */

/** @defgroup papi_conn_urlsrcmpeg2 papi_conn_urlsrcmpeg2: UrlSrc
  * The function of a URL Source is to stream data identified by a URL into the platform. A URL has the following format:scheme://scheme specific format. The scheme, also called protocol, is used for accessing the data. Examples are: FILE, HTTP, and FTP. For the HTTP scheme, a URL will look like: http://hostport[/path][?search].<br>
Based on the protocol, capabilities of the server, and content type of the data special operations can be applied to the data like pause, fast forward, go to position, and so on. These special operations are often referred to as trick modes. When a new URL is requested, the available trick modes may change. Clients have to take this into account. Note that linear playback of the data is always supported.<br>
Accessing a server that provides a URL can be slow, especially when it points to a remote server. Therefore, gaining access is done in two steps. First, the URL is parsed to see if the requested scheme is available and the format of the URL is correct. This can be done locally and fast. Secondly, the server and the actual data will be accessed which might require remote access and can be slow.<br>
When a URL identifies data on a remote server, the connection to the server may be lost. This will be signaled to the client. A number of retries will be performed before a definite connection lost notification is given. Furthermore, delays in the transmission can occur. These will not be signaled to the client but gracefully handled by the platform.<br>
Data streaming from a server is subject to jitter. Therefore, an input buffer is modeled as part of URL Source. This buffer is used to compensate for this jitter, that is, to ensure a continuous playback. When the URL Source is put in play state, data streaming to the output only starts once the amount of data in the buffer has reached a level of nominal + hysteresis. Then, the fullness level of the buffer fluctuates around (below and above) the nominal level. The detailed buffering model during trick modes is scheme-specific and not described further in this document.<br>
The ratio between the nominal level and the total buffer size is URL scheme-specific. The total buffer size itself depends on the maximum possible jitter in the system and on the bit rate of the selected URL. The fullness level of the buffer is observable to the client (in % of nominal level).
  * @ingroup papi_conn */

/** @defgroup papi_conn_urlsrcmpeg2_Datatypes Datatypes 
  * @ingroup papi_conn_urlsrcmpeg2 */
/**
 *   @defgroup papi_conn_urlsrcmpeg2_UrlSrcError Error codes UrlSrc
 *   @ingroup papi_conn_urlsrcmpeg2_Datatypes
 *   Error codes for UrlSrc.
 *   @{ */
#define papi_conn_urlsrcmpeg2_UrlSrcInvalidUrl 2049
#define papi_conn_urlsrcmpeg2_UrlSrcSchemeNotSupported 2050
#define papi_conn_urlsrcmpeg2_UrlSrcAborted 2051
#define papi_conn_urlsrcmpeg2_UrlSrcNotAborted 2052
#define papi_conn_urlsrcmpeg2_UrlSrcEndOfStream 2053
#define papi_conn_urlsrcmpeg2_UrlSrcConnectionLost 2054
/*   @} */

/** @defgroup papi_conn_urlsrcmpeg2_Functions Functions 
  * @ingroup papi_conn_urlsrcmpeg2 */

/**  @ingroup papi_conn_urlsrcmpeg2_Functions */
/**  This function is intended to abort an OpenUrl or an automatic connection retry. Calling this function has no effect when there is no attempt to set a URL or to reconnect to a URL. The function is synchronous. When it aborted an OpenUrl or an automatic connection retry, it notifies the client with a synchronous OnActionAborted notification before completion and makes the accessState of the currentUrl equal to AsNoAccess. */ 
/**  @param retval Return value can be either Ok or Not Aborted. See datatype \ref papi_conn_urlsrcmpeg2_UrlSrcError. */
extern FResult papi_conn_urlsrcmpeg2_AbortAction( Nat32 * retval );

/**  @ingroup papi_conn_urlsrcmpeg2_Functions */
/**  Returns the available step modes. */ 
/**  @param AvailStepModes Output parameter indicating the set of trick modes that are supported. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcmpeg2_GetAvailStepModes( tmUrlSrc_StepModes_t * AvailStepModes, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcmpeg2_Functions */
/**  If the current URL is accessible, this function returns the current buffer fullness level. Otherwise it returns 0. */ 
/**  @param BufferFullness Output parameter with the percentage with which the playback buffer is filled. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcmpeg2_GetBufferFullness( Nat32 * BufferFullness, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcmpeg2_Functions */
/**  Close the connection with the current url. */ 
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcmpeg2_CloseUrl( Nat32 * retval );

/**  @ingroup papi_conn_urlsrcmpeg2_Functions */
/**  Sets the URL that identifies the data. The OpenUrl function will try to make the connection to the location. It will generate a notification OnNotAccessible or OnAccessible on failure or on success.<br>
During the attempt to access the URL, the state of the UrlSrc is AsAccessing. When the UrlSrc is in this state no other OpenUrl may be called. This state is not explicitly observable but only implicitly by deducing it from the notifications. */ 
/**  @param url Input parameter with the url as a null-terminated string. */
/**  @param timeOut Input parameter defining the maximum number of microseconds that establishing a connection may take. */
/**  @param retval Return value can be either Ok, Invalid Url, or Scheme not supported. See datatype \ref papi_conn_urlsrcmpeg2_UrlSrcError. */
extern FResult papi_conn_urlsrcmpeg2_OpenUrl( char * url, Nat32 timeOut, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcmpeg2_Functions */
/**  Pause playback of the current url. */ 
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcmpeg2_Pause( Nat32 * retval );

/**  @ingroup papi_conn_urlsrcmpeg2_Functions */
/**  Returns the current position in the stream that is playing. */ 
/**  @param Time Output parameter indicating the current position in the stream, in milliseconds. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcmpeg2_GetPosition( Nat32 * Time, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcmpeg2_Functions */
/**  Sets the position to the specified time. It generates the notification OnSetPositionCompleted after the current position is set to the actual rounded position. */ 
/**  @param requestedTime Input parameter that specifies the requested position, in milliseconds. */
/**  @param rounding Input parameter indicating the rounding technique that has to be used to determine the actual position in the stream. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcmpeg2_SetPosition( Nat32 requestedTime, tmUrlSrc_Rounding_t rounding, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcmpeg2_Functions */
/**  Resumes playback at the point in the stream where Pause halted it or from the position set by the last SetPosition call if SetPosition is called after Pause. */ 
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcmpeg2_Resume( Nat32 * retval );

/**  @ingroup papi_conn_urlsrcmpeg2_Functions */
/**  Returns the different schemes that are supported (ftp, http, rtsp, ..). */ 
/**  @param SupportedSchemes Output parameter indicating the set of the schemes that are supported. The value is a bit-mask that is constructed by OR-ing the values of enum #_tmUrlSrc_Scheme_t. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcmpeg2_GetSuppSchemes( tmUrlSrc_Schemes_t * SupportedSchemes, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcmpeg2_Functions */
/**  Returns the current seek mode. */ 
/**  @param seekMode The current seek mode. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcmpeg2_GetSeekMode( tmUrlSrc_SeekMode_t * seekMode, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcmpeg2_Functions */
/**  Sets the seek mode to be used. */ 
/**  @param seekMode The desired seek mode. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcmpeg2_SetSeekMode( tmUrlSrc_SeekMode_t seekMode, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcmpeg2_Functions */
/**  Returns (1) whether reverse speeds are allowed and (2) the lower and upper bounds of the speed that can be handled for the content identified by the URL. The bounds are determined as follows: minSpeed = (*pMinSpeedNum / *pMinSpeedDenom) and maxSpeed = (*pMaxSpeedNum / *pMaxSpeedDenom). */ 
/**  @param ReverseSupp Output parameter indicating whether reverse speed is supported. */
/**  @param MinSpeedNum Output parameter with the minimal playback speed, numerator part. */
/**  @param MinSpeedDenom Output parameter with the minimal playback speed, denominator part. */
/**  @param MaxSpeedNum Output parameter with the maximum playback speed, numerator part. */
/**  @param pMaxSpeedDenom Output parameter with the maximum playback speed, denominator part. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcmpeg2_GetSpeedRange( Bool * ReverseSupp, Nat8 * MinSpeedNum, Nat8 * MinSpeedDenom, Nat8 * MaxSpeedNum, Nat8 * pMaxSpeedDenom, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcmpeg2_Functions */
/**  Returns the playback speed as follows: requestedSpeed = (*pRequestedSpeedNum / *pRequestedSpeedDenom) and the speed that was actually set as follows: actualSpeed = (*pActualSpeedNum / *pActualSpeedDenom). */ 
/**  @param requestedSpeedNum Output parameter with the playback speed that was requested, numerator part. */
/**  @param requestedSpeedDenom Output parameter with the playback speed that was requested, denominator part. */
/**  @param ActualSpeedNum Output parameter with the actual playback speed, numerator part. */
/**  @param ActualSpeedDenom Output parameter with the actual playback speed, denominator part. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcmpeg2_GetSpeed( Int8 * requestedSpeedNum, Nat8 * requestedSpeedDenom, Int8 * ActualSpeedNum, Nat8 * ActualSpeedDenom, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcmpeg2_Functions */
/**  Sets the playback speed as follows: requestedSpeed = (requestedSpeedNum/requestedSpeedDenom). Returns the speed that was actually set as follows: actualSpeed = (actualSpeedNum/actualSpeedDenom). */ 
/**  @param requestedSpeedNum Input parameter with the requested playback speed, numerator part. */
/**  @param requestedSpeedDenom Input parameter with the requested playback speed, denominator part. */
/**  @param ActualSpeedNum Output parameter with the actual playback speed, numerator part. */
/**  @param ActualSpeedDenom Output parameter with the actual playback speed, denominator part. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcmpeg2_SetSpeed( Int8 requestedSpeedNum, Nat8 requestedSpeedDenom, Int8 * ActualSpeedNum, Nat8 * ActualSpeedDenom, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcmpeg2_Functions */
/**  This function starts streaming the data whenever data becomes available. A consecutive call to start does not have any effect on the URL currently being streamed. */ 
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcmpeg2_Start( Nat32 * retval );

/**  @ingroup papi_conn_urlsrcmpeg2_Functions */
/**  Performs playback of the previous access unit. If there is no previous access unit, a return value indicating this is returned. The function returns when there was a noticeable effect for the client.<br>
When the connection was lost, the function immediately returns without waiting for retries */ 
/**  @param retval Return value can be either Ok, End Of Stream, or Connection Lost. See datatype \ref papi_conn_urlsrcmpeg2_UrlSrcError. */
extern FResult papi_conn_urlsrcmpeg2_StepBackward( Nat32 * retval );

/**  @ingroup papi_conn_urlsrcmpeg2_Functions */
/**  Performs playback of the next access unit. If there is no next access unit, a return value indicating this is returned. The function returns when there was a noticeable effect for the client.<br>
When the connection was lost, the function immediately returns without waiting for retries. */ 
/**  @param retval Return value can be either Ok, End Of Stream, or Connection Lost. See datatype \ref papi_conn_urlsrcmpeg2_UrlSrcError. */
extern FResult papi_conn_urlsrcmpeg2_StepForward( Nat32 * retval );

/**  @ingroup papi_conn_urlsrcmpeg2_Functions */
/**  Stops playing the current URL (flushes the data that has not been decoded, and stops rendering as soon as possible). */ 
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcmpeg2_Stop( Nat32 * retval );

/**  @ingroup papi_conn_urlsrcmpeg2_Functions */
/**  If the current URL is accessible and the total duration of the data is known, this function returns that duration. Otherwise it returns 0. */ 
/**  @param TotalDuration Output parameter returning the total duration in milliseconds. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcmpeg2_GetTotalDuration( Nat32 * TotalDuration, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcmpeg2_Functions */
/**  If the current URL is accessible and the total size of the data is known, this function returns that size. Otherwise it returns 0. */ 
/**  @param TotalSize Output parameter returning the total size in bytes. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcmpeg2_GetTotalSize( Nat64 * TotalSize, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcmpeg2_Functions */
/**  Returns the URL that was set as a null-terminated string and the actual size of the URL. */ 
/**  @param size Input parameter indicating the maximum size of the returned string, in bytes. */
/**  @param ActualSize Output parameter indicating the actual size of the returned string, in bytes. */
/**  @param url Output parameter returning the url that was set, as a null-terminated string. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcmpeg2_GetUrl( Nat16 size, Nat32 * ActualSize, Nat8 * url, Nat32 * retval );



/*
 *  interface urlsrcvid of type IPlfApiUrlSrc
 */

/** @defgroup papi_conn_urlsrcvid papi_conn_urlsrcvid: UrlSrc
  * The function of a URL Source is to stream data identified by a URL into the platform. A URL has the following format:scheme://scheme specific format. The scheme, also called protocol, is used for accessing the data. Examples are: FILE, HTTP, and FTP. For the HTTP scheme, a URL will look like: http://hostport[/path][?search].<br>
Based on the protocol, capabilities of the server, and content type of the data special operations can be applied to the data like pause, fast forward, go to position, and so on. These special operations are often referred to as trick modes. When a new URL is requested, the available trick modes may change. Clients have to take this into account. Note that linear playback of the data is always supported.<br>
Accessing a server that provides a URL can be slow, especially when it points to a remote server. Therefore, gaining access is done in two steps. First, the URL is parsed to see if the requested scheme is available and the format of the URL is correct. This can be done locally and fast. Secondly, the server and the actual data will be accessed which might require remote access and can be slow.<br>
When a URL identifies data on a remote server, the connection to the server may be lost. This will be signaled to the client. A number of retries will be performed before a definite connection lost notification is given. Furthermore, delays in the transmission can occur. These will not be signaled to the client but gracefully handled by the platform.<br>
Data streaming from a server is subject to jitter. Therefore, an input buffer is modeled as part of URL Source. This buffer is used to compensate for this jitter, that is, to ensure a continuous playback. When the URL Source is put in play state, data streaming to the output only starts once the amount of data in the buffer has reached a level of nominal + hysteresis. Then, the fullness level of the buffer fluctuates around (below and above) the nominal level. The detailed buffering model during trick modes is scheme-specific and not described further in this document.<br>
The ratio between the nominal level and the total buffer size is URL scheme-specific. The total buffer size itself depends on the maximum possible jitter in the system and on the bit rate of the selected URL. The fullness level of the buffer is observable to the client (in % of nominal level).
  * @ingroup papi_conn */

/** @defgroup papi_conn_urlsrcvid_Datatypes Datatypes 
  * @ingroup papi_conn_urlsrcvid */
/**
 *   @defgroup papi_conn_urlsrcvid_UrlSrcError Error codes UrlSrc
 *   @ingroup papi_conn_urlsrcvid_Datatypes
 *   Error codes for UrlSrc.
 *   @{ */
#define papi_conn_urlsrcvid_UrlSrcInvalidUrl 2049
#define papi_conn_urlsrcvid_UrlSrcSchemeNotSupported 2050
#define papi_conn_urlsrcvid_UrlSrcAborted 2051
#define papi_conn_urlsrcvid_UrlSrcNotAborted 2052
#define papi_conn_urlsrcvid_UrlSrcEndOfStream 2053
#define papi_conn_urlsrcvid_UrlSrcConnectionLost 2054
/*   @} */

/** @defgroup papi_conn_urlsrcvid_Functions Functions 
  * @ingroup papi_conn_urlsrcvid */

/**  @ingroup papi_conn_urlsrcvid_Functions */
/**  This function is intended to abort an OpenUrl or an automatic connection retry. Calling this function has no effect when there is no attempt to set a URL or to reconnect to a URL. The function is synchronous. When it aborted an OpenUrl or an automatic connection retry, it notifies the client with a synchronous OnActionAborted notification before completion and makes the accessState of the currentUrl equal to AsNoAccess. */ 
/**  @param retval Return value can be either Ok or Not Aborted. See datatype \ref papi_conn_urlsrcvid_UrlSrcError. */
extern FResult papi_conn_urlsrcvid_AbortAction( Nat32 * retval );

/**  @ingroup papi_conn_urlsrcvid_Functions */
/**  Returns the available step modes. */ 
/**  @param AvailStepModes Output parameter indicating the set of trick modes that are supported. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcvid_GetAvailStepModes( tmUrlSrc_StepModes_t * AvailStepModes, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcvid_Functions */
/**  If the current URL is accessible, this function returns the current buffer fullness level. Otherwise it returns 0. */ 
/**  @param BufferFullness Output parameter with the percentage with which the playback buffer is filled. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcvid_GetBufferFullness( Nat32 * BufferFullness, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcvid_Functions */
/**  Close the connection with the current url. */ 
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcvid_CloseUrl( Nat32 * retval );

/**  @ingroup papi_conn_urlsrcvid_Functions */
/**  Sets the URL that identifies the data. The OpenUrl function will try to make the connection to the location. It will generate a notification OnNotAccessible or OnAccessible on failure or on success.<br>
During the attempt to access the URL, the state of the UrlSrc is AsAccessing. When the UrlSrc is in this state no other OpenUrl may be called. This state is not explicitly observable but only implicitly by deducing it from the notifications. */ 
/**  @param url Input parameter with the url as a null-terminated string. */
/**  @param timeOut Input parameter defining the maximum number of microseconds that establishing a connection may take. */
/**  @param retval Return value can be either Ok, Invalid Url, or Scheme not supported. See datatype \ref papi_conn_urlsrcvid_UrlSrcError. */
extern FResult papi_conn_urlsrcvid_OpenUrl( char * url, Nat32 timeOut, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcvid_Functions */
/**  Pause playback of the current url. */ 
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcvid_Pause( Nat32 * retval );

/**  @ingroup papi_conn_urlsrcvid_Functions */
/**  Returns the current position in the stream that is playing. */ 
/**  @param Time Output parameter indicating the current position in the stream, in milliseconds. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcvid_GetPosition( Nat32 * Time, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcvid_Functions */
/**  Sets the position to the specified time. It generates the notification OnSetPositionCompleted after the current position is set to the actual rounded position. */ 
/**  @param requestedTime Input parameter that specifies the requested position, in milliseconds. */
/**  @param rounding Input parameter indicating the rounding technique that has to be used to determine the actual position in the stream. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcvid_SetPosition( Nat32 requestedTime, tmUrlSrc_Rounding_t rounding, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcvid_Functions */
/**  Resumes playback at the point in the stream where Pause halted it or from the position set by the last SetPosition call if SetPosition is called after Pause. */ 
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcvid_Resume( Nat32 * retval );

/**  @ingroup papi_conn_urlsrcvid_Functions */
/**  Returns the different schemes that are supported (ftp, http, rtsp, ..). */ 
/**  @param SupportedSchemes Output parameter indicating the set of the schemes that are supported. The value is a bit-mask that is constructed by OR-ing the values of enum #_tmUrlSrc_Scheme_t. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcvid_GetSuppSchemes( tmUrlSrc_Schemes_t * SupportedSchemes, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcvid_Functions */
/**  Returns the current seek mode. */ 
/**  @param seekMode The current seek mode. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcvid_GetSeekMode( tmUrlSrc_SeekMode_t * seekMode, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcvid_Functions */
/**  Sets the seek mode to be used. */ 
/**  @param seekMode The desired seek mode. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcvid_SetSeekMode( tmUrlSrc_SeekMode_t seekMode, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcvid_Functions */
/**  Returns (1) whether reverse speeds are allowed and (2) the lower and upper bounds of the speed that can be handled for the content identified by the URL. The bounds are determined as follows: minSpeed = (*pMinSpeedNum / *pMinSpeedDenom) and maxSpeed = (*pMaxSpeedNum / *pMaxSpeedDenom). */ 
/**  @param ReverseSupp Output parameter indicating whether reverse speed is supported. */
/**  @param MinSpeedNum Output parameter with the minimal playback speed, numerator part. */
/**  @param MinSpeedDenom Output parameter with the minimal playback speed, denominator part. */
/**  @param MaxSpeedNum Output parameter with the maximum playback speed, numerator part. */
/**  @param pMaxSpeedDenom Output parameter with the maximum playback speed, denominator part. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcvid_GetSpeedRange( Bool * ReverseSupp, Nat8 * MinSpeedNum, Nat8 * MinSpeedDenom, Nat8 * MaxSpeedNum, Nat8 * pMaxSpeedDenom, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcvid_Functions */
/**  Returns the playback speed as follows: requestedSpeed = (*pRequestedSpeedNum / *pRequestedSpeedDenom) and the speed that was actually set as follows: actualSpeed = (*pActualSpeedNum / *pActualSpeedDenom). */ 
/**  @param requestedSpeedNum Output parameter with the playback speed that was requested, numerator part. */
/**  @param requestedSpeedDenom Output parameter with the playback speed that was requested, denominator part. */
/**  @param ActualSpeedNum Output parameter with the actual playback speed, numerator part. */
/**  @param ActualSpeedDenom Output parameter with the actual playback speed, denominator part. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcvid_GetSpeed( Int8 * requestedSpeedNum, Nat8 * requestedSpeedDenom, Int8 * ActualSpeedNum, Nat8 * ActualSpeedDenom, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcvid_Functions */
/**  Sets the playback speed as follows: requestedSpeed = (requestedSpeedNum/requestedSpeedDenom). Returns the speed that was actually set as follows: actualSpeed = (actualSpeedNum/actualSpeedDenom). */ 
/**  @param requestedSpeedNum Input parameter with the requested playback speed, numerator part. */
/**  @param requestedSpeedDenom Input parameter with the requested playback speed, denominator part. */
/**  @param ActualSpeedNum Output parameter with the actual playback speed, numerator part. */
/**  @param ActualSpeedDenom Output parameter with the actual playback speed, denominator part. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcvid_SetSpeed( Int8 requestedSpeedNum, Nat8 requestedSpeedDenom, Int8 * ActualSpeedNum, Nat8 * ActualSpeedDenom, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcvid_Functions */
/**  This function starts streaming the data whenever data becomes available. A consecutive call to start does not have any effect on the URL currently being streamed. */ 
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcvid_Start( Nat32 * retval );

/**  @ingroup papi_conn_urlsrcvid_Functions */
/**  Performs playback of the previous access unit. If there is no previous access unit, a return value indicating this is returned. The function returns when there was a noticeable effect for the client.<br>
When the connection was lost, the function immediately returns without waiting for retries */ 
/**  @param retval Return value can be either Ok, End Of Stream, or Connection Lost. See datatype \ref papi_conn_urlsrcvid_UrlSrcError. */
extern FResult papi_conn_urlsrcvid_StepBackward( Nat32 * retval );

/**  @ingroup papi_conn_urlsrcvid_Functions */
/**  Performs playback of the next access unit. If there is no next access unit, a return value indicating this is returned. The function returns when there was a noticeable effect for the client.<br>
When the connection was lost, the function immediately returns without waiting for retries. */ 
/**  @param retval Return value can be either Ok, End Of Stream, or Connection Lost. See datatype \ref papi_conn_urlsrcvid_UrlSrcError. */
extern FResult papi_conn_urlsrcvid_StepForward( Nat32 * retval );

/**  @ingroup papi_conn_urlsrcvid_Functions */
/**  Stops playing the current URL (flushes the data that has not been decoded, and stops rendering as soon as possible). */ 
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcvid_Stop( Nat32 * retval );

/**  @ingroup papi_conn_urlsrcvid_Functions */
/**  If the current URL is accessible and the total duration of the data is known, this function returns that duration. Otherwise it returns 0. */ 
/**  @param TotalDuration Output parameter returning the total duration in milliseconds. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcvid_GetTotalDuration( Nat32 * TotalDuration, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcvid_Functions */
/**  If the current URL is accessible and the total size of the data is known, this function returns that size. Otherwise it returns 0. */ 
/**  @param TotalSize Output parameter returning the total size in bytes. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcvid_GetTotalSize( Nat64 * TotalSize, Nat32 * retval );

/**  @ingroup papi_conn_urlsrcvid_Functions */
/**  Returns the URL that was set as a null-terminated string and the actual size of the URL. */ 
/**  @param size Input parameter indicating the maximum size of the returned string, in bytes. */
/**  @param ActualSize Output parameter indicating the actual size of the returned string, in bytes. */
/**  @param url Output parameter returning the url that was set, as a null-terminated string. */
/**  @param retval Standard return value. */
extern FResult papi_conn_urlsrcvid_GetUrl( Nat16 size, Nat32 * ActualSize, Nat8 * url, Nat32 * retval );



/*
 *  interface cmdmxN of type IPlfApiCmDmxNotify
 */

/** @defgroup papi_conn_cmdmx papi_conn_cmdmx: CmDmx
  * @ingroup papi_conn */

/** @defgroup papi_conn_cmdmxN_Notifications Notifications 
  * @ingroup papi_conn_cmdmx */

/**  @ingroup papi_conn_cmdmxN_Notifications */
/**  Notification function to signal that the client-side memory stream buffer is full. In case a buffer does not hold enough free space to store data, this data is discarded and the filter process is paused. Furthermore, data which arrives after the pausing of the filter process will be discarded as well, even if this data is smaller in size and might fit in the remaining free buffer space. If the filter process is paused due to lack of free buffer space, it will be resumed in case a client releases the lock on some data and frees buffer space in a way that there is enough consecutive space from the position in the buffer where the last data has been written to store new data */ 
extern void papi_conn_cmdmxN_OnBufferFull( void );

/**  @ingroup papi_conn_cmdmxN_Notifications */
/**  Notification to signal the availability of new data in the client-side memory stream buffer. The rate of this notification is determined by the granularity that is set by SetBuffer(). */ 
/**  @param dataCookie Cookie that can be used by the client to free the available data in the stream buffer. */
/**  @param primaryDataSize Size of the available data, in bytes (excluding any buffer wrap-around) */
/**  @param PrimaryDataStartAddress 4-bytes aligned memory pointer pointing at the start of the available data. */
/**  @param secondaryDataSize Size of the available data in the second part, in bytes, in case of a buffer wrap-around. Zero, otherwise. */
/**  @param SecondaryDataStartAddress 4-bytes aligned memory pointer pointing at the start of the available data in the second part, in case of a buffer wrap-around. */
extern void papi_conn_cmdmxN_OnDataArrived( Nat32 dataCookie, Nat32 primaryDataSize, tmCmDmx_Buffer_t PrimaryDataStartAddress, Nat32 secondaryDataSize, tmCmDmx_Buffer_t SecondaryDataStartAddress );

/**  @ingroup papi_conn_cmdmxN_Notifications */
/**  Notification function to signal that the bitrate of the incoming multiplexed stream has changed. */ 
/**  @param bitRate The new bit rate in KB/s. */
extern void papi_conn_cmdmxN_OnSystemBitRateChanged( Nat32 bitRate );



/*
 *  interface digadecN of type IPlfApiDigitalAudioDecoderNotify
 */

/** @defgroup papi_conn_digadec papi_conn_digadec: DigitalAudioDecoder
  * This interface is used to notify changes on the digital audio decoder
  * @ingroup papi_conn */

/** @defgroup papi_conn_digadecN_Notifications Notifications 
  * @ingroup papi_conn_digadec */

/**  @ingroup papi_conn_digadecN_Notifications */
/**  In case of Signal Loss (i.e. an under-run error) is raised through the OnError Notification.<br>
Also this error notification function is used to inform the subscribed client that an error occurred while decoding audio from the digital audio input stream.<br>
 */ 
/**  @param err Describes the type of error that occurred */
extern void papi_conn_digadecN_OnError( tmDigAdec_Error_t err );



/*
 *  interface digvdecN of type IPlfApiDigitalVideoDecoderNotify
 */

/** @defgroup papi_conn_digvdec papi_conn_digvdec: DigitalVideoDecoder
  * Interface to notify properties of the digital video decoder
  * @ingroup papi_conn */

/** @defgroup papi_conn_digvdecN_Notifications Notifications 
  * @ingroup papi_conn_digvdec */

/**  @ingroup papi_conn_digvdecN_Notifications */
/**  This notification function is used to inform the subscribed client that an error occurred while decoding video from the digital video input stream. */ 
/**  @param err Describes the type of error that occurred */
extern void papi_conn_digvdecN_OnError( tmDigVdec_Error_t err );

/**  @ingroup papi_conn_digvdecN_Notifications */
/**  This function is called by the digital video decoder whenever it detects a change in the compression standard, profile, or level of its input stream */ 
/**  @param standard The new compression standard of the input stream */
/**  @param profile The new profile of the input stream */
/**  @param level The new level of the input stream */
extern void papi_conn_digvdecN_OnStandardDataChanged( tmDigVdec_CompressionStandard_t standard, tmDigVdec_Profile_t profile, tmDigVdec_Level_t level );



/*
 *  interface imgdecN of type IPlfApiImageDecNotify
 */

/** @defgroup papi_conn_imgdec papi_conn_imgdec: ImageDec
  * @ingroup papi_conn */

/** @defgroup papi_conn_imgdecN_Notifications Notifications 
  * @ingroup papi_conn_imgdec */

/**  @ingroup papi_conn_imgdecN_Notifications */
/**  This function is called by the ImageDecoder when no more meta-data is available in the input stream. */ 
extern void papi_conn_imgdecN_OnEndMetaData( void );

/**  @ingroup papi_conn_imgdecN_Notifications */
/**  This function is called by the ImageDecoder when it has either successfully decoded a full image or failed to do so. Failure to decode is reported via the status parameter. On success, the decoded image is present in the buffer passed with DecodeImage. Otherwise, this buffer is left as it was. */ 
/**  @param status The status of the image decoding. */
extern void papi_conn_imgdecN_OnFullImageDecoded( tmImageDec2_Status_t status );

/**  @ingroup papi_conn_imgdecN_Notifications */
/**  This function is called by the ImageDecoder when it has found raw meta-data in the input stream. The format of the raw meta-data is compliant with the specification of the image type and its file format. The client is expected to copy the meta-data from the buffer pointed to by pRawData. When this function returns the buffer pointed to by pRawData will no longer be valid. */ 
/**  @param size The size of the buffer containing the meta-data. */
/**  @param RawData Pointer to the buffer of the image decoder containing the raw meta-data. */
extern void papi_conn_imgdecN_OnMetaDataFound( Nat32 size, tmImageDec_Buffer_t RawData );

/**  @ingroup papi_conn_imgdecN_Notifications */
/**  This function is called by the ImageDecoder when the process to determine the resolution, aspect ratio and orientation of the encoded image in the input stream has finished. */ 
/**  @param status The status of the resolution obtainment. */
/**  @param horRes Parameter indicating the horizontal resolution. The resolution is expressed in number of pixels per line. */
/**  @param vertRes Parameter indicating the vertical resolution. The resolution is expressed in number of lines. */
extern void papi_conn_imgdecN_OnResolutionObtained( tmImageDec2_Status_t status, Nat32 horRes, Nat32 vertRes );

/**  @ingroup papi_conn_imgdecN_Notifications */
/**  This function is called by the ImageDecoder when it has either decoded or extracted a thumbnail or failed to do so. Failure to decode or extract is reported via the status parameter. On success, the decoded or extracted thumbnail is present in the buffer passed with DecodeThumbnail. Otherwise, this buffer is left as it was. */ 
/**  @param status The status of the thumbnail decoding. */
extern void papi_conn_imgdecN_OnThumbnailDecoded( tmImageDec2_Status_t status );

/**  @ingroup papi_conn_imgdecN_Notifications */
/**  This function is called by the ImageDecoder when the process to detect the presence of a thumbnail in the input stream has finished. */ 
/**  @param status The status of the thumbnail presence detection. */
/**  @param presence Parameter indicating whether a thumbnail is present (True) or not (False). */
extern void papi_conn_imgdecN_OnThumbnailPresence( tmImageDec2_Status_t status, Bool presence );



/*
 *  interface urlsrcjpgN of type IPlfApiUrlSrcNotify
 */

/** @defgroup papi_conn_urlsrcjpg papi_conn_urlsrcjpg: UrlSrc
  * @ingroup papi_conn */

/** @defgroup papi_conn_urlsrcjpgN_Notifications Notifications 
  * @ingroup papi_conn_urlsrcjpg */

/**  @ingroup papi_conn_urlsrcjpgN_Notifications */
/**  Notification when a requested url has become accessible, for instance after an OpenUrl(). */ 
extern void papi_conn_urlsrcjpgN_OnAccessible( void );

/**  @ingroup papi_conn_urlsrcjpgN_Notifications */
/**  This function is called when the client has called AbortAction and indeed a OpenUrl or a connection retry was cancelled. */ 
extern void papi_conn_urlsrcjpgN_OnActionAborted( void );

/**  @ingroup papi_conn_urlsrcjpgN_Notifications */
/**  This function is called when the buffering status changed. */ 
extern void papi_conn_urlsrcjpgN_OnBufferingStatusChanged( tmUrlSrc_BufferingState_t bufferState );

/**  @ingroup papi_conn_urlsrcjpgN_Notifications */
/**  This function is called when the URL that was accessed is no longer accessible due to a lost connection. There are a number of autonomous retries and each retry causes such a notification. The time between retries is chosen by the platform. If not aborted (see AbortAction), the function is called nrOfRetries times before an OnNotAccessible is called */ 
extern void papi_conn_urlsrcjpgN_OnConnectionRetry( void );

/**  @ingroup papi_conn_urlsrcjpgN_Notifications */
/**  This function is called each time at least 1% of the data has been consumed. However, this function will not be called more than a predefined times per second. This function will only be called if the total size is known. */ 
/**  @param bytesConsumed Number of bytes that have been consumed thusfar. */
extern void papi_conn_urlsrcjpgN_OnDataConsumed( Nat64 bytesConsumed );

/**  @ingroup papi_conn_urlsrcjpgN_Notifications */
/**  This function is called while playing when the source of the data indicates that there is no more data. It does not mean that all data is rendered, but it indicates that a new OpenUrl can be performed without interrupting the current URL. This notification is signaled only when an end of the input was encountered during normal streaming (playing forward or reverse in arbitrary speeds). */ 
extern void papi_conn_urlsrcjpgN_OnEndOfInput( void );

/**  @ingroup papi_conn_urlsrcjpgN_Notifications */
/**  This function is called when it detects that the end of a stream has been reached while playing. It indicates that the data is rendered and has left the platform. This notification is signaled due to the following conditions (the notification will come after data has left the platform):<br>
<ul>
<li>The end of the stream was encountered while playing that stream (either forward or reverse). Note that it is irrelevant how the end was reached (for example using SetPosition) just the fact that the end is reached triggers the notification.</li>
<li>An OpenUrl is called that interrupts a playing stream.</li>
<li>An AbortAction is called that interrupts an OpenUrl or a connection retry, while a stream was being played.</li>
<li>A CloseUrl is called while a stream was being played.</li>
</ul>
In case the output of UrlSrc is not really a streaming output but for example an image, this function is only called in the latter three cases; i.e. when the client does OpenUrl, AbortAction, or CloseUrl. */ 
extern void papi_conn_urlsrcjpgN_OnEndOfRendering( void );

/**  @ingroup papi_conn_urlsrcjpgN_Notifications */
/**  Notification when the url is not accessible, for instance after a timeout of OpenUrl(). */ 
/**  @param cause Indicating the cause why the url is not accessible. */
extern void papi_conn_urlsrcjpgN_OnNotAccessible( tmUrlSrc_NotAccessibleCause_t cause );

/**  @ingroup papi_conn_urlsrcjpgN_Notifications */
/**  This function is called each time at least 1% of the total duration has passed. However, this function will not be called more than a predefined times per second. This function will only be called if the total duration is known. */ 
/**  @param timePlayed Current position in the stream, in milliseconds. */
extern void papi_conn_urlsrcjpgN_OnPlayTimeChanged( Nat32 timePlayed );

/**  @ingroup papi_conn_urlsrcjpgN_Notifications */
/**  Notification when setting the position is aborted. */ 
extern void papi_conn_urlsrcjpgN_OnSetPositionAborted( void );

/**  @ingroup papi_conn_urlsrcjpgN_Notifications */
/**  Notification when the requested position in the stream has been reached. */ 
extern void papi_conn_urlsrcjpgN_OnSetPositionCompleted( Nat32 actualTime, Bool endOfStreamReached );

/**  @ingroup papi_conn_urlsrcjpgN_Notifications */
/**  This function is called when a new OpenUrl looses the capability to do trick modes. This notification will only come when previously a GetTrickControl call was made that indeed returned an interface pointer and when this interface pointer is not released. */ 
extern void papi_conn_urlsrcjpgN_OnTrickControlLost( void );



/*
 *  interface urlsrcmp3N of type IPlfApiUrlSrcNotify
 */

/** @defgroup papi_conn_urlsrcmp3 papi_conn_urlsrcmp3: UrlSrc
  * @ingroup papi_conn */

/** @defgroup papi_conn_urlsrcmp3N_Notifications Notifications 
  * @ingroup papi_conn_urlsrcmp3 */

/**  @ingroup papi_conn_urlsrcmp3N_Notifications */
/**  Notification when a requested url has become accessible, for instance after an OpenUrl(). */ 
extern void papi_conn_urlsrcmp3N_OnAccessible( void );

/**  @ingroup papi_conn_urlsrcmp3N_Notifications */
/**  This function is called when the client has called AbortAction and indeed a OpenUrl or a connection retry was cancelled. */ 
extern void papi_conn_urlsrcmp3N_OnActionAborted( void );

/**  @ingroup papi_conn_urlsrcmp3N_Notifications */
/**  This function is called when the buffering status changed. */ 
extern void papi_conn_urlsrcmp3N_OnBufferingStatusChanged( tmUrlSrc_BufferingState_t bufferState );

/**  @ingroup papi_conn_urlsrcmp3N_Notifications */
/**  This function is called when the URL that was accessed is no longer accessible due to a lost connection. There are a number of autonomous retries and each retry causes such a notification. The time between retries is chosen by the platform. If not aborted (see AbortAction), the function is called nrOfRetries times before an OnNotAccessible is called */ 
extern void papi_conn_urlsrcmp3N_OnConnectionRetry( void );

/**  @ingroup papi_conn_urlsrcmp3N_Notifications */
/**  This function is called each time at least 1% of the data has been consumed. However, this function will not be called more than a predefined times per second. This function will only be called if the total size is known. */ 
/**  @param bytesConsumed Number of bytes that have been consumed thusfar. */
extern void papi_conn_urlsrcmp3N_OnDataConsumed( Nat64 bytesConsumed );

/**  @ingroup papi_conn_urlsrcmp3N_Notifications */
/**  This function is called while playing when the source of the data indicates that there is no more data. It does not mean that all data is rendered, but it indicates that a new OpenUrl can be performed without interrupting the current URL. This notification is signaled only when an end of the input was encountered during normal streaming (playing forward or reverse in arbitrary speeds). */ 
extern void papi_conn_urlsrcmp3N_OnEndOfInput( void );

/**  @ingroup papi_conn_urlsrcmp3N_Notifications */
/**  This function is called when it detects that the end of a stream has been reached while playing. It indicates that the data is rendered and has left the platform. This notification is signaled due to the following conditions (the notification will come after data has left the platform):<br>
<ul>
<li>The end of the stream was encountered while playing that stream (either forward or reverse). Note that it is irrelevant how the end was reached (for example using SetPosition) just the fact that the end is reached triggers the notification.</li>
<li>An OpenUrl is called that interrupts a playing stream.</li>
<li>An AbortAction is called that interrupts an OpenUrl or a connection retry, while a stream was being played.</li>
<li>A CloseUrl is called while a stream was being played.</li>
</ul>
In case the output of UrlSrc is not really a streaming output but for example an image, this function is only called in the latter three cases; i.e. when the client does OpenUrl, AbortAction, or CloseUrl. */ 
extern void papi_conn_urlsrcmp3N_OnEndOfRendering( void );

/**  @ingroup papi_conn_urlsrcmp3N_Notifications */
/**  Notification when the url is not accessible, for instance after a timeout of OpenUrl(). */ 
/**  @param cause Indicating the cause why the url is not accessible. */
extern void papi_conn_urlsrcmp3N_OnNotAccessible( tmUrlSrc_NotAccessibleCause_t cause );

/**  @ingroup papi_conn_urlsrcmp3N_Notifications */
/**  This function is called each time at least 1% of the total duration has passed. However, this function will not be called more than a predefined times per second. This function will only be called if the total duration is known. */ 
/**  @param timePlayed Current position in the stream, in milliseconds. */
extern void papi_conn_urlsrcmp3N_OnPlayTimeChanged( Nat32 timePlayed );

/**  @ingroup papi_conn_urlsrcmp3N_Notifications */
/**  Notification when setting the position is aborted. */ 
extern void papi_conn_urlsrcmp3N_OnSetPositionAborted( void );

/**  @ingroup papi_conn_urlsrcmp3N_Notifications */
/**  Notification when the requested position in the stream has been reached. */ 
extern void papi_conn_urlsrcmp3N_OnSetPositionCompleted( Nat32 actualTime, Bool endOfStreamReached );

/**  @ingroup papi_conn_urlsrcmp3N_Notifications */
/**  This function is called when a new OpenUrl looses the capability to do trick modes. This notification will only come when previously a GetTrickControl call was made that indeed returned an interface pointer and when this interface pointer is not released. */ 
extern void papi_conn_urlsrcmp3N_OnTrickControlLost( void );



/*
 *  interface urlsrcmpeg2N of type IPlfApiUrlSrcNotify
 */

/** @defgroup papi_conn_urlsrcmpeg2 papi_conn_urlsrcmpeg2: UrlSrc
  * @ingroup papi_conn */

/** @defgroup papi_conn_urlsrcmpeg2N_Notifications Notifications 
  * @ingroup papi_conn_urlsrcmpeg2 */

/**  @ingroup papi_conn_urlsrcmpeg2N_Notifications */
/**  Notification when a requested url has become accessible, for instance after an OpenUrl(). */ 
extern void papi_conn_urlsrcmpeg2N_OnAccessible( void );

/**  @ingroup papi_conn_urlsrcmpeg2N_Notifications */
/**  This function is called when the client has called AbortAction and indeed a OpenUrl or a connection retry was cancelled. */ 
extern void papi_conn_urlsrcmpeg2N_OnActionAborted( void );

/**  @ingroup papi_conn_urlsrcmpeg2N_Notifications */
/**  This function is called when the buffering status changed. */ 
extern void papi_conn_urlsrcmpeg2N_OnBufferingStatusChanged( tmUrlSrc_BufferingState_t bufferState );

/**  @ingroup papi_conn_urlsrcmpeg2N_Notifications */
/**  This function is called when the URL that was accessed is no longer accessible due to a lost connection. There are a number of autonomous retries and each retry causes such a notification. The time between retries is chosen by the platform. If not aborted (see AbortAction), the function is called nrOfRetries times before an OnNotAccessible is called */ 
extern void papi_conn_urlsrcmpeg2N_OnConnectionRetry( void );

/**  @ingroup papi_conn_urlsrcmpeg2N_Notifications */
/**  This function is called each time at least 1% of the data has been consumed. However, this function will not be called more than a predefined times per second. This function will only be called if the total size is known. */ 
/**  @param bytesConsumed Number of bytes that have been consumed thusfar. */
extern void papi_conn_urlsrcmpeg2N_OnDataConsumed( Nat64 bytesConsumed );

/**  @ingroup papi_conn_urlsrcmpeg2N_Notifications */
/**  This function is called while playing when the source of the data indicates that there is no more data. It does not mean that all data is rendered, but it indicates that a new OpenUrl can be performed without interrupting the current URL. This notification is signaled only when an end of the input was encountered during normal streaming (playing forward or reverse in arbitrary speeds). */ 
extern void papi_conn_urlsrcmpeg2N_OnEndOfInput( void );

/**  @ingroup papi_conn_urlsrcmpeg2N_Notifications */
/**  This function is called when it detects that the end of a stream has been reached while playing. It indicates that the data is rendered and has left the platform. This notification is signaled due to the following conditions (the notification will come after data has left the platform):<br>
<ul>
<li>The end of the stream was encountered while playing that stream (either forward or reverse). Note that it is irrelevant how the end was reached (for example using SetPosition) just the fact that the end is reached triggers the notification.</li>
<li>An OpenUrl is called that interrupts a playing stream.</li>
<li>An AbortAction is called that interrupts an OpenUrl or a connection retry, while a stream was being played.</li>
<li>A CloseUrl is called while a stream was being played.</li>
</ul>
In case the output of UrlSrc is not really a streaming output but for example an image, this function is only called in the latter three cases; i.e. when the client does OpenUrl, AbortAction, or CloseUrl. */ 
extern void papi_conn_urlsrcmpeg2N_OnEndOfRendering( void );

/**  @ingroup papi_conn_urlsrcmpeg2N_Notifications */
/**  Notification when the url is not accessible, for instance after a timeout of OpenUrl(). */ 
/**  @param cause Indicating the cause why the url is not accessible. */
extern void papi_conn_urlsrcmpeg2N_OnNotAccessible( tmUrlSrc_NotAccessibleCause_t cause );

/**  @ingroup papi_conn_urlsrcmpeg2N_Notifications */
/**  This function is called each time at least 1% of the total duration has passed. However, this function will not be called more than a predefined times per second. This function will only be called if the total duration is known. */ 
/**  @param timePlayed Current position in the stream, in milliseconds. */
extern void papi_conn_urlsrcmpeg2N_OnPlayTimeChanged( Nat32 timePlayed );

/**  @ingroup papi_conn_urlsrcmpeg2N_Notifications */
/**  Notification when setting the position is aborted. */ 
extern void papi_conn_urlsrcmpeg2N_OnSetPositionAborted( void );

/**  @ingroup papi_conn_urlsrcmpeg2N_Notifications */
/**  Notification when the requested position in the stream has been reached. */ 
extern void papi_conn_urlsrcmpeg2N_OnSetPositionCompleted( Nat32 actualTime, Bool endOfStreamReached );

/**  @ingroup papi_conn_urlsrcmpeg2N_Notifications */
/**  This function is called when a new OpenUrl looses the capability to do trick modes. This notification will only come when previously a GetTrickControl call was made that indeed returned an interface pointer and when this interface pointer is not released. */ 
extern void papi_conn_urlsrcmpeg2N_OnTrickControlLost( void );



/*
 *  interface urlsrcvidN of type IPlfApiUrlSrcNotify
 */

/** @defgroup papi_conn_urlsrcvid papi_conn_urlsrcvid: UrlSrc
  * @ingroup papi_conn */

/** @defgroup papi_conn_urlsrcvidN_Notifications Notifications 
  * @ingroup papi_conn_urlsrcvid */

/**  @ingroup papi_conn_urlsrcvidN_Notifications */
/**  Notification when a requested url has become accessible, for instance after an OpenUrl(). */ 
extern void papi_conn_urlsrcvidN_OnAccessible( void );

/**  @ingroup papi_conn_urlsrcvidN_Notifications */
/**  This function is called when the client has called AbortAction and indeed a OpenUrl or a connection retry was cancelled. */ 
extern void papi_conn_urlsrcvidN_OnActionAborted( void );

/**  @ingroup papi_conn_urlsrcvidN_Notifications */
/**  This function is called when the buffering status changed. */ 
extern void papi_conn_urlsrcvidN_OnBufferingStatusChanged( tmUrlSrc_BufferingState_t bufferState );

/**  @ingroup papi_conn_urlsrcvidN_Notifications */
/**  This function is called when the URL that was accessed is no longer accessible due to a lost connection. There are a number of autonomous retries and each retry causes such a notification. The time between retries is chosen by the platform. If not aborted (see AbortAction), the function is called nrOfRetries times before an OnNotAccessible is called */ 
extern void papi_conn_urlsrcvidN_OnConnectionRetry( void );

/**  @ingroup papi_conn_urlsrcvidN_Notifications */
/**  This function is called each time at least 1% of the data has been consumed. However, this function will not be called more than a predefined times per second. This function will only be called if the total size is known. */ 
/**  @param bytesConsumed Number of bytes that have been consumed thusfar. */
extern void papi_conn_urlsrcvidN_OnDataConsumed( Nat64 bytesConsumed );

/**  @ingroup papi_conn_urlsrcvidN_Notifications */
/**  This function is called while playing when the source of the data indicates that there is no more data. It does not mean that all data is rendered, but it indicates that a new OpenUrl can be performed without interrupting the current URL. This notification is signaled only when an end of the input was encountered during normal streaming (playing forward or reverse in arbitrary speeds). */ 
extern void papi_conn_urlsrcvidN_OnEndOfInput( void );

/**  @ingroup papi_conn_urlsrcvidN_Notifications */
/**  This function is called when it detects that the end of a stream has been reached while playing. It indicates that the data is rendered and has left the platform. This notification is signaled due to the following conditions (the notification will come after data has left the platform):<br>
<ul>
<li>The end of the stream was encountered while playing that stream (either forward or reverse). Note that it is irrelevant how the end was reached (for example using SetPosition) just the fact that the end is reached triggers the notification.</li>
<li>An OpenUrl is called that interrupts a playing stream.</li>
<li>An AbortAction is called that interrupts an OpenUrl or a connection retry, while a stream was being played.</li>
<li>A CloseUrl is called while a stream was being played.</li>
</ul>
In case the output of UrlSrc is not really a streaming output but for example an image, this function is only called in the latter three cases; i.e. when the client does OpenUrl, AbortAction, or CloseUrl. */ 
extern void papi_conn_urlsrcvidN_OnEndOfRendering( void );

/**  @ingroup papi_conn_urlsrcvidN_Notifications */
/**  Notification when the url is not accessible, for instance after a timeout of OpenUrl(). */ 
/**  @param cause Indicating the cause why the url is not accessible. */
extern void papi_conn_urlsrcvidN_OnNotAccessible( tmUrlSrc_NotAccessibleCause_t cause );

/**  @ingroup papi_conn_urlsrcvidN_Notifications */
/**  This function is called each time at least 1% of the total duration has passed. However, this function will not be called more than a predefined times per second. This function will only be called if the total duration is known. */ 
/**  @param timePlayed Current position in the stream, in milliseconds. */
extern void papi_conn_urlsrcvidN_OnPlayTimeChanged( Nat32 timePlayed );

/**  @ingroup papi_conn_urlsrcvidN_Notifications */
/**  Notification when setting the position is aborted. */ 
extern void papi_conn_urlsrcvidN_OnSetPositionAborted( void );

/**  @ingroup papi_conn_urlsrcvidN_Notifications */
/**  Notification when the requested position in the stream has been reached. */ 
extern void papi_conn_urlsrcvidN_OnSetPositionCompleted( Nat32 actualTime, Bool endOfStreamReached );

/**  @ingroup papi_conn_urlsrcvidN_Notifications */
/**  This function is called when a new OpenUrl looses the capability to do trick modes. This notification will only come when previously a GetTrickControl call was made that indeed returned an interface pointer and when this interface pointer is not released. */ 
extern void papi_conn_urlsrcvidN_OnTrickControlLost( void );


