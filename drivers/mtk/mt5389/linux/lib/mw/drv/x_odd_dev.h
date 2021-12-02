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
/*****************************************************************************
* LEGAL DISCLAIMER
*
* (Header of MediaTek Software/Firmware Release or Documentation)
*
* BY OPENING OR USING THIS FILE, USER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND
* AGREES THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
* ARE PROVIDED TO USER ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS
* ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
* IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
* NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH
* RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED
* IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND USER AGREES TO LOOK ONLY TO
* SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL
* ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO USER'S
* SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
* USER HEREBY ACKNOWLEDGES THE CONFIDENTIALITY OF MEDIATEK SOFTWARE AND AGREES
* NOT TO DISCLOSE OR PERMIT DISCLOSURE OF ANY MEDIATEK SOFTWARE TO ANY THIRD
* PARTY OR TO ANY OTHER PERSON, EXCEPT TO DIRECTORS, OFFICERS, EMPLOYEES OF
* USER WHO ARE REQUIRED TO HAVE THE INFORMATION TO CARRY OUT THE PURPOSE OF
* OPENING OR USING THIS FILE.
*****************************************************************************/

#ifndef _X_ODD_DEV_H_
#define _X_ODD_DEV_H_

#include "inc/x_common.h"

/*! The status type used by ODD device to notify middleware.
 *! - When the tray of ODD is moving, there are five kinds of status:
 *!   - When the tray is completely opened,
 *!     notifies ODD_STATUS_TRAY_OPEN.
 *!   - When the tray is completely closed,
 *!     notifies ODD_STATUS_TRAY_CLOSE.
 *!   - When the tray is in opening process,
 *!     notifies ODD_STATUS_TRAY_OPENING.
 *!   - When the tray is in closing process,
 *!     notifies ODD_STATUS_TRAY_CLOSING.
 *!   - When the tray is blocked or out of order,
 *!     notifies ODD_STATUS_TRAY_ERROR.
 *!
 *! - Once the tray is completely closed and ODD device will detect if there
 *!   is a disc inside, and notify to middleware the disc status:
 *!   - If there is a disc inside, notifies ODD_STATUS_DISC_PRESENT
 *!   - If there is NOT a disc inside, notifies ODD_STATUS_DISC_ABSENT
 */
typedef enum
{
    ODD_STATUS_TRAY_OPEN = 0x81,  /*!< tray is completely opened */
    ODD_STATUS_TRAY_CLOSE,        /*!< tray is completely closed */
    ODD_STATUS_TRAY_OPENING,      /*!< tray is in opening process */
    ODD_STATUS_TRAY_CLOSING,      /*!< tray is in closing process */
    ODD_STATUS_TRAY_ERROR,        /*!< tray is blocked or out of order */
    ODD_STATUS_DISC_LOADING,      /*!< loader is loading disc */
    ODD_STATUS_DISC_PRESENT,      /*!< there is a disc inside */
    ODD_STATUS_DISC_ABSENT        /*!< there is NOT a disc inside */
} ODD_STATUS_T;


/******************************************************************************/
/*! ODD socket device access only property types */
/******************************************************************************/
typedef enum {
    ODD_PR_DEV_SPEED                = 0x81, /*!< returns the current ODD transfer speed in Kbytes/Sec */
    ODD_PR_DEV_SPIN_OFF_TIMER               /*!< set or returns the auto spindle off timer in seconds due to ODD idle for a long time */
} ODD_DEV_PROP_TYPE_T;


/******************************************************************************/
/*! ODD block device access only property types */
/******************************************************************************/
typedef enum {
    ODD_PR_MED_TYPE                 = 0xB1, /*!< Returns the medium type enumeration */
    ODD_PR_MED_STATUS,                      /*!< Returns the medium status enumeration */
    ODD_PR_MED_LAYER_NUM,                   /*!< Returns the medium layer number */
    ODD_PR_MED_LAYER_SIZE,                  /*!< Returns the medium layer size in sectors of specified layer number */
    ODD_PR_MED_NWA,                         /*!< Returns next writable LBA of write-once medium */
    ODD_PR_MED_TOC_HEADER,                  /*!< Returns the medium TOC Header information (ODD_PR_TOC_HEADER_T)*/
    ODD_PR_MED_TOC_ENTRY,                   /*!< Returns the medium TOC Entry information (ODD_PR_TOC_ENTRY_T)*/
    ODD_PR_MED_SESSION_NUM,                 /*!< Returns the total session number of the medium */
    ODD_PR_MED_SESSION_INFO,                /*!< Returns the session information of specified session no. */
    ODD_PR_MED_TRACK_NUM,                   /*!< Returns the total track number of the medium */
    ODD_PR_MED_TRACK_INFO                   /*!< Returns the track information of specified track no. */
} ODD_MED_PROP_TYPE_T;


/******************************************************************************/
/*! ODD socket device access only property types */
/******************************************************************************/
typedef enum {
    ODD_PR_COM_LAST_SENSE_KEY       = 0xE1, /*!< Returns the last command error codes */
} ODD_COM_PROP_TYPE_T;


typedef enum {
    ODD_SPD_CD_CLV_4X,
    ODD_SPD_CD_CLV_10X,
    ODD_SPD_DVD_CLV_2X,
    ODD_SPD_DVD_CLV_4X,
    ODD_SPD_BD_CAV_4X,
    ODD_SPD_BD_CLV_2X
} ODD_DEV_SPEED_TYPE_T;

/*! Device speed information */
typedef struct _ODD_DEV_SPEED_T {
    ODD_DEV_SPEED_TYPE_T  u4ReadSpeed;     /*!< read speed */
    ODD_DEV_SPEED_TYPE_T  u4WriteSpeed;    /*!< write speed */
} ODD_DEV_SPEED_T;


/*! medium type enumeration */
typedef enum {
    ODD_MED_UNKNOWN   =0x01,
    ODD_MED_CD_ROM    =0x03,
    ODD_MED_CD_R,
    ODD_MED_CD_RW,
    ODD_MED_DVD_ROM,
    ODD_MED_DVD_R,
    ODD_MED_DVD_RW,
    ODD_MED_DVD_PR,
    ODD_MED_DVD_PRW,
    ODD_MED_DVD_RAM,
    ODD_MED_BD_ROM,
    ODD_MED_BD_RE,
    ODD_MED_BD_RW
} ODD_PR_MED_TYPE_T;


/*! medium status enumeration */
typedef enum {
    ODD_MED_INVALID,
    ODD_MED_RDONLY,
    ODD_MED_BLANK,
    ODD_MED_FORMATTED,
    ODD_MED_APPENDALBE
} ODD_PR_MED_STATUS_T;


/*! the medium layer size in sectors of specified layer number */
typedef struct _ODD_PR_MED_LAYER_SIZE_T {
    UINT32 u4LayerNo;         /*!< [IN]  specified layer no. */
    UINT32 u4SectorNum;       /*!< [OUT] returns the sector number of the specified layer no. */
} ODD_PR_MED_LAYER_SIZE_T;


#define ODD_TOC_ENTRY_MAX_NS (15854) /* 7927 tracks + 7927 sessions */

typedef struct
{
    UINT8 u1DiscType;   /* Byte 0 */
    UINT8 u1TotalTrk;   /* Byte 1 */
    UINT8 u1TotalSen;   /* Byte 2 */
    UINT8 u1Flag;       /* Byte 3 */
    UINT8 u1Trk1St;     /* Byte 4 */
    UINT8 u1Reserve;    /* Byte 5 */
} ODD_PR_TOC_HEADER_T;

typedef struct
{
    UINT8 u1TrkNo;      /* Byte 0 */
    UINT8 u1CtrlAddr;   /* Byte 1 */
    UINT8 u1Reserve;    /* Byte 2 */
    UINT8 u1AMin;       /* Byte 3 */
    UINT8 u1ASec;       /* Byte 4 */
    UINT8 u1ABlk;       /* Byte 5 */
} ODD_PR_TOC_ENTRY_T;

/*! session information of specified session no. */
typedef struct _ODD_PR_MED_SESSION_INFO_T {
    UINT32 u4SessionNo;         /*!< [IN]  specified session no. */
    UINT32 u4StartTrackNo;      /*!< [OUT] start track no. */
    UINT32 u4EndTrackNo;        /*!< [OUT] end track no. */
} ODD_PR_MED_SESSION_INFO_T;


/*! track information of specified session no. */
typedef struct _ODD_PR_MED_TRACK_INFO_T {
    UINT32  u4TrackNo;          /*!< [IN]  specified track no. */
    BOOL    fgIsAudioTrack;     /*!< [OUT] TRUE - audio track, FALSE - data track */
    BOOL    fgIsComplete;       /*!< [OUT] TRUE - complete track, FALSE - incomplete track */
    UINT32  u4StartLBA;         /*!< [OUT] track start LBA address */
    UINT32  u4SectorNum;        /*!< [OUT] track sector number */
} ODD_PR_MED_TRACK_INFO_T;


/******************************************************************************/
/*! socket device I/O control types */
/******************************************************************************/
typedef enum {
    ODD_IO_DEV_SPIN_CTRL,         /*!< Start or Stop the ODD spindle */
    ODD_IO_DEV_TRAY_CTRL,         /*!< Open or Close the ODD tray */
    ODD_IO_DEV_SET_SPEED,         /*!< Set current read/write speed */
    ODD_IO_DEV_UPGRADE,           /*!< Upgrade ODD firmware */
    ODD_IO_DEV_SOFTWARE_RESET,    /*!< Software reset ODD */
    ODD_IO_DEV_HARDWARE_RESET,    /*!< Hardware reset ODD */
    ODD_IO_DEV_TEST_UNIT_READY,   /*!< Check ODD is ready or not */
    ODD_IO_DEV_GET_EVENT_ST,      /*!< Check medium is available or not, TRUE ¡V medium present, FALSE ¡V medium abse */
    ODD_IO_DEV_MECHANISM_ST,      /*!< Check ODD tray status */
    ODD_IO_DEV_SET_TRANS_MODE,    /*!< Set ODD transfer mode to PIO or UDMA transfer */
    ODD_IO_DEV_READ_BUF_CAP,      /*!< Read buffer capacity from ODD for burning engine buffer control */
    ODD_IO_DEV_MTK_INQUIRY        /*!< Read vendor information from ODD for manufacture mode (FA mode) */
} ODD_DEV_IO_TYPE_T;


typedef enum {
    ODD_DEV_SPIN_ON,
    ODD_DEV_SPIN_OFF
} ODD_IO_DEV_SPIN_CTRL_T;


typedef enum {
    ODD_DEV_TRAY_OPEN,
    ODD_DEV_TRAY_CLOSE
} ODD_IO_DEV_TRAY_CTRL_T;


typedef struct _ODD_IO_DEV_FIRMWARE_T {
    UINT32  u4BufLen;             /*!< firmware buffer length */
    UINT8   *pu1Buf;              /*!< firmware buffer pointer */
} ODD_IO_DEV_FIRMWARE_T;


typedef enum {
    ODD_DEV_TRANS_MODE_PIO_1,
    ODD_DEV_TRANS_MODE_PIO_2,
    ODD_DEV_TRANS_MODE_PIO_3,
    ODD_DEV_TRANS_MODE_PIO_4,
    ODD_DEV_TRANS_MODE_UDMA_1,
    ODD_DEV_TRANS_MODE_UDMA_2
} ODD_IO_DEV_TRANS_MODE_T;


typedef struct _ODD_IO_DEV_BUF_CAP_T {
    UINT32  u4TotalBuf;           /*!< total buffer length in bytes */
    UINT32  u4UnusedBuf;          /*!< unused buffer length in bytes */
} ODD_IO_DEV_BUF_CAP_T;


typedef struct _ODD_IO_DEV_MTK_INQUIRY_T {
    UINT8   pu1Buf[4];            /*!< MTK inquiry data */
} ODD_IO_DEV_MTK_INQUIRY_T;


/******************************************************************************/
/*! block device I/O control types */
/******************************************************************************/
typedef enum {
    ODD_IO_MED_FORMAT         = 0x81,   /*!< Format the medium physical information */
    ODD_IO_MED_FINALIZE,                /*!< Finalize the medium to update physical information for playback */
    ODD_IO_MED_UNFINALIZE,              /*!< Unfinalize the medium to allow further recording */
    ODD_IO_MED_GET_SECTOR_SIZE,         /*!< Returns the sector size of specified LBA */
    ODD_IO_MED_READ_CD,                 /*!< Use Read CD ATAPI command to read data from CDDA discs */
    ODD_IO_MED_READ_DVD_STRUCT,         /*!< Read DVD structure information from medium to get physical information */
    ODD_IO_MED_AUTO_TUNE,               /*!< Execute disc auto tunning command to automatically adjust write strategy for unsupported discs. */
    ODD_IO_MED_READ_KCD,                /*!< Read KCD data */
    ODD_IO_MED_READ_CD_TEXT             /*!< Read CD-TEXT data */
} ODD_MED_IO_TYPE_T;


typedef struct _ODD_IO_MED_FORMAT_T {
    ODD_PR_MED_TYPE_T   eMedType;       /*!< medium type */
    UINT32              u4FormatType;   /*!< format type */
    VOID                *pvCallback;    /*!< progress callback function */
} ODD_IO_MED_FORMAT_T;


typedef struct _ODD_IO_MED_FINALIZE_T {
    ODD_PR_MED_TYPE_T   eMedType;       /*!< medium type */
    UINT32              u4FormatType;   /*!< format type */
    VOID                *pvCallback;    /*!< progress callback function */
} ODD_IO_MED_FINALIZE_T;


typedef struct _ODD_IO_MED_UNFINALIZE_T {
    ODD_PR_MED_TYPE_T   eMedType;       /*!< medium type */
    UINT32              u4FormatType;   /*!< format type */
    VOID                *pvCallback;    /*!< progress callback function */
} ODD_IO_MED_UNFINALIZE_T;


typedef enum {
    ODD_MED_SECT_SZ_2448,               /*!< audio track */
    ODD_MED_SECT_SZ_2336,               /*!< mode 0 / mode 2 formless */
    ODD_MED_SECT_SZ_2048,               /*!< mode 1 / mode 2 form 1 */
    ODD_MED_SECT_SZ_2324,               /*!< mode 2 form 2 */
    ODD_MED_SECT_SZ_2328                /*!< mode 2 form 2 */
} ODD_MED_SECTOR_SIZE_TYPE_T;


typedef struct _ODD_IO_MED_SECTOR_SIZE_T {
    UINT32                     u4Lba;   /*!< [IN] specified LBA */
    ODD_MED_SECTOR_SIZE_TYPE_T rSectSz; /*!< [OUT] sector size type of the specified LBA */
} ODD_IO_MED_SECTOR_SIZE_T;


typedef struct _ODD_IO_MED_READ_CD_T {
    UINT32                      u4Offset;       /*!< read start address */
    UINT32                      u4Count;        /*!< read data length */
    ODD_MED_SECTOR_SIZE_TYPE_T  rSectSz;        /*!< sector size in bytes */
    void                        *pvBuf;         /*!< buffer pointer */
    UINT32                      *pu4XferCnt;    /*!< actual transfer count */
} ODD_IO_MED_READ_CD_T;


typedef struct _ODD_IO_MED_READ_DVD_STRUCT_T {
    UINT8               u1MedType;      /*!< MedType: 00b - DVD & HD DVD, 01b - BD */
    UINT32              u4Address;      /*!< Address */
    UINT8               u1Layer;        /*!< Layer */
    UINT8               u1FmtCd;        /*!< FmtCd */
    UINT8               u1AGID;         /*!< AGID */
    void                *pvBuf;         /*!< buffer pointer */
    UINT32              u4BufLen;       /*!< buffer length */
} ODD_IO_MED_READ_DVD_STRUCT_T;


typedef struct _ODD_IO_MED_READ_KCD_T {
    UINT8               u1AGID;         /*!< AGID */
    void                *pvBuf;         /*!< buffer pointer */
    UINT32              u4BufLen;       /*!< buffer length */
} ODD_IO_MED_READ_KCD_T;

typedef struct _ODD_IO_MED_READ_CD_TEXT_T {
    void                *pvBuf;         /*!< buffer pointer */
    UINT32              u4BufLen;       /*!< buffer length */
} ODD_IO_MED_READ_CD_TEXT_T;

/******************************************************************************/
/*! ODD socket & block device access I/O types */
/******************************************************************************/
typedef enum {
    ODD_IO_COM_REPORT_KEY       = 0xE1, /*!< Report key from ODD for content protection key exchange */
    ODD_IO_COM_SEND_KEY,                /*!< Send key to ODD for content protection key exchange */
    ODD_IO_COM_GET_FEATURE              /*!< Get features of ODD loader or disc */
} ODD_COM_IO_TYPE_T;


typedef struct _ODD_IO_COM_REPORT_KEY_T {
    UINT32              u4Lba;          /*!< Lba */
    UINT8               u1Func;         /*!< Func */
    UINT8               u1KeyClass;     /*!< Key class */
    UINT8               u1AGID;         /*!< AGID */
    UINT8               u1KeyFmt;       /*!< KeyFmt */
    void                *pvBuf;         /*!< buffer pointer */
    UINT32              u4BufLen;       /*!< buffer length */
} ODD_IO_COM_REPORT_KEY_T;


typedef struct _ODD_IO_COM_SEND_KEY_T {
    UINT8               u1Func;         /*!< Func */
    UINT8               u1KeyClass;     /*!< Key class */
    UINT8               u1AGID;         /*!< AGID */
    UINT8               u1KeyFmt;       /*!< KeyFmt */
    void                *pvBuf;         /*!< buffer pointer */
    UINT32              u4BufLen;       /*!< buffer length */
} ODD_IO_COM_SEND_KEY_T;


typedef struct _ODD_IO_COM_GET_FEATURE_T {
    UINT32              u4FeatureNum;   /*!< Feature Number */
    void                *pvBuf;         /*!< buffer pointer */
    UINT32              u4BufLen;       /*!< buffer length */
} ODD_IO_COM_GET_FEATURE_T;


#endif /* _X_ODD_DEV_H_ */

