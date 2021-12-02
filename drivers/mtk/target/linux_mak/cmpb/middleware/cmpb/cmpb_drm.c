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


/**
    IMtkPb_DRM defines secure storage operations.  Data in the secure storage is encrypted and signature protected.
    Note that no cache mechanism is inside MtkPbLib.
*/


/*----------------------------------------------------------------------------*/
/*! @addtogroup IMtkPb_DRM
 *  @{
 */
/*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "IMtkPb_ErrorCode.h"
//#include "SecureStorage.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/


/*!
 * @name General
 * @{
 */


/*! @brief          Initializes IMtkPb_DRM
 *  @return         indicate success
 *  @retval         IMTK_PB_ERROR_CODE_OK                       Success.
 *  @retval         IMTK_PB_ERROR_CODE_SYS_LIB_ERROR            unexpected error in system call or standard library
 *  @retval         IMTK_PB_ERROR_CODE_FILE_NOT_FOUND           target kernel module does not exist at system.
 *  @retval         IMTK_PB_ERROR_CODE_INSUFFICIENT_MEMORY      this size of memory does not be allocated.
 *  @retval         IMTK_PB_ERROR_CODE_FILE_READ_ERROR          target device can not be loaded.
 *  @retval         IMTK_PB_ERROR_CODE_DRV_ERROR                target device has internal error when target device open.
 *  @note           Before a process starts using IMtkPb_DRM, it should first call this function.
 *  @see            IMtkPb_DRM_Terminate()
 */
IMTK_PB_ERROR_CODE_T IMtkPb_DRM_Init(void)
{
    return IMTK_PB_ERROR_CODE_OK;
}


/*! @brief          Terminates IMtkPb_DRM
 *  @return         indicate success
 *  @retval         IMTK_PB_ERROR_CODE_OK                       Success.
 *  @retval         IMTK_PB_ERROR_CODE_SYS_LIB_ERROR            unexpected error in system call or standard library
 *  @retval         IMTK_PB_ERROR_CODE_DRV_ERROR                target device has internal error after target device load.
 *  @note           When a process finishes using IMtkPb_DRM, it should call this function.
 *  @see            IMtkPb_DRM_Init()
 */
IMTK_PB_ERROR_CODE_T IMtkPb_DRM_Terminate(void)
{
    return IMTK_PB_ERROR_CODE_OK;
}


/*!
 * @}
 * @name Secure Storage
 * @{
 */


/*! @brief          Load (decrypt and integrity check) data from secure storage
 *  @param [in]     pcSrcPath       path of the securely stored file
 *  @param [in]     pcDstPath       path of the plaintext destination file
 *  @retval         IMTK_PB_ERROR_CODE_OK                       Success.
 *  @retval         IMTK_PB_ERROR_CODE_INV_ARG                  incorrect input parameter.
 *  @retval         IMTK_PB_ERROR_CODE_FILE_NOT_FOUND           encrypted file does not exist.
 *  @retval         IMTK_PB_ERROR_CODE_INSUFFICIENT_MEMORY      this size of memory does not be allocated.
 *  @retval         IMTK_PB_ERROR_CODE_FILE_READ_ERROR          encrypted file can not be read.
 *  @retval         IMTK_PB_ERROR_CODE_FILE_CRYPTO_ERROR        process of decryption or verification occurs fault.
 *  @retval         IMTK_PB_ERROR_CODE_FILE_NOT_CREATE          plain text file can not be created.
 *  @retval         IMTK_PB_ERROR_CODE_FILE_WRITE_ERROR         plain text file can not be written.
 *  @note
 *  @see            IMtkPb_DRM_SecureStorage_Save()
 */
#if 0
IMTK_PB_ERROR_CODE_T IMtkPb_DRM_SecureStorage_Load(const char*   pcSrcPath,
                                                          const char*   pcDstPath)
{
    int32_t i4_ret=0;

#ifdef SECURE_STORAGE_SUPPORT
    i4_ret = x_secure_storage_file_decryption(pcSrcPath, pcDstPath);
#endif

    return ((i4_ret == 0) ? IMTK_PB_ERROR_CODE_OK : IMTK_PB_ERROR_CODE_INV_ARG);
}


/*! @brief          Store (encrypt and add signature) data into secure storage
 *  @param [in]     pcSrcPath       path of the plaintext file to be stored
 *  @param [in]     pcDstPath       path of the file to contain the securely stored data
 *  @retval         IMTK_PB_ERROR_CODE_OK                       Success.
 *  @retval         IMTK_PB_ERROR_CODE_INV_ARG                  incorrect input parameter.
 *  @retval         IMTK_PB_ERROR_CODE_FILE_NOT_FOUND           plain text file does not exist.
 *  @retval         IMTK_PB_ERROR_CODE_INSUFFICIENT_MEMORY      this size of memory does not be allocated.
 *  @retval         IMTK_PB_ERROR_CODE_FILE_READ_ERROR          plain text file can not be read.
 *  @retval         IMTK_PB_ERROR_CODE_FILE_CRYPTO_ERROR        process of encryption or signature occurs fault.
 *  @retval         IMTK_PB_ERROR_CODE_FILE_NOT_CREATE          encrypted file can not be created.
 *  @retval         IMTK_PB_ERROR_CODE_FILE_WRITE_ERROR         encrypted file can not be written.
 *  @note
 *  @see            IMtkPb_DRM_SecureStorage_Load()
 */
IMTK_PB_ERROR_CODE_T IMtkPb_DRM_SecureStorage_Save(const char*   pcSrcPath,
                                                          const char*   pcDstPath)
{
    int32_t i4_ret=0;

#ifdef SECURE_STORAGE_SUPPORT
    i4_ret = x_secure_storage_file_encryption(pcSrcPath, pcDstPath);
#endif

    return ((i4_ret == 0) ? IMTK_PB_ERROR_CODE_OK : IMTK_PB_ERROR_CODE_INV_ARG);    
}


/*! @brief          Securely clean-up a plaintext file
 *  @param [in]     pcFilePath      path of the file to be securely cleaned
 *  @retval         IMTK_PB_ERROR_CODE_OK                       Success.
 *  @retval         IMTK_PB_ERROR_CODE_INV_ARG                  incorrect input parameter.
 *  @retval         IMTK_PB_ERROR_CODE_FILE_NOT_FOUND           target file does not exist.
 *  @retval         IMTK_PB_ERROR_CODE_INSUFFICIENT_MEMORY      this size of memory does not be allocated.
 *  @retval         IMTK_PB_ERROR_CODE_FILE_WRITE_ERROR         target file can not be reset.
 *  @retval         IMTK_PB_ERROR_CODE_FILE_NOT_DELETE          target file can not be deleted.
 *  @note           The contents of the file will be erased before the file being deleted
 *  @see
 */
IMTK_PB_ERROR_CODE_T IMtkPb_DRM_SecureStorage_Cleanup(const char*    pcFilePath)
{
    int32_t i4_ret=0;
    
#ifdef SECURE_STORAGE_SUPPORT
    i4_ret = x_secure_storage_file_cleanup(pcFilePath);
#endif

    return ((i4_ret == 0) ? IMTK_PB_ERROR_CODE_OK : IMTK_PB_ERROR_CODE_INV_ARG);     
}
#endif


/*! @brief          Read (decrypt and integrity check) data from secure storage as plain text
 *  @param [in]     pcSrcPath       path of the securely stored file
 *  @param [out]    pcData          the buffer to hold the data
 *  @param [in]     u4Length        length of the data
 *  @retval         IMTK_PB_ERROR_CODE_OK                       Success.
 *  @retval         IMTK_PB_ERROR_CODE_INV_ARG                  incorrect input parameter.
 *  @retval         IMTK_PB_ERROR_CODE_FILE_NOT_FOUND           encrypted file does not exist.
 *  @retval         IMTK_PB_ERROR_CODE_INSUFFICIENT_MEMORY      this size of memory does not be allocated.
 *  @retval         IMTK_PB_ERROR_CODE_FILE_READ_ERROR          encrypted file can not be read.
 *  @retval         IMTK_PB_ERROR_CODE_FILE_CRYPTO_ERROR        process of decryption or verification occurs fault.
 *  @note
 *  @see            IMtkPb_DRM_SecureStorage_Write()
 */
IMTK_PB_ERROR_CODE_T IMtkPb_DRM_SecureStorage_Read(const char*   pcSrcPath,
                                                          char*         pcData,
                                                          uint32_t      u4Length)
{
    return IMTK_PB_ERROR_CODE_INV_ARG;
}


/*! @brief          Write (encrypt and add signature) data into secure storage
 *  @param [in]     pcDstPath       path of the file to contain the securely stored data
 *  @param [in]     pcData          the buffer to hold the source data
 *  @param [in]     u4Length        length of the data
 *  @retval         IMTK_PB_ERROR_CODE_OK                       Success.
 *  @retval         IMTK_PB_ERROR_CODE_INV_ARG                  incorrect input parameter.
 *  @retval         IMTK_PB_ERROR_CODE_INSUFFICIENT_MEMORY      this size of memory does not be allocated.
 *  @retval         IMTK_PB_ERROR_CODE_FILE_CRYPTO_ERROR        process of encryption or signature occurs fault.
 *  @retval         IMTK_PB_ERROR_CODE_FILE_NOT_CREATE          encrypted file can not be created.
 *  @retval         IMTK_PB_ERROR_CODE_FILE_WRITE_ERROR         encrypted file can not be written.
 *  @note
 *  @see            IMtkPb_DRM_SecureStorage_Read()
 */
IMTK_PB_ERROR_CODE_T IMtkPb_DRM_SecureStorage_Write(const char*  pcDstPath,
                                                           const char*  pcData,
                                                           uint32_t     u4Length)
{
    return IMTK_PB_ERROR_CODE_INV_ARG;
}


/*! @brief          Process (encrypt or decrypt) data using AES counter mode
 *  @param [in]     pcKey           the buffer to hold the aes key.
 *  @param [in]     u4KeyLength     length of the key, unit is bit, ex. 128, 192, or 256.
 *  @param [in]     pcNonce         nonce, size is 16 bytes.
 *  @param [in]     pcSrcBuf        the buffer to hold the source data.
 *  @param [out]    pcDstBuf        the buffer to hold the destination data, and it could be the same as pcSrcBuf.
 *  @param [in]     u4BufLength     length of the data; must be multiple of 16.
 *  @retval         IMTK_PB_ERROR_CODE_OK                       Success.
 *  @retval         IMTK_PB_ERROR_CODE_INV_ARG                  incorrect input parameter.
 *  @retval         IMTK_PB_ERROR_CODE_DATA_CRYPTO_ERROR        encryption or decryption error.
 *  @note
 *  @see
 */
IMTK_PB_ERROR_CODE_T IMtkPb_DRM_AES_CTR(const char* pcKey,
                                               uint32_t    u4KeyLength,
                                               const char* pcNonce,
                                               const char* pcSrcBuf,
                                               char*       pcDstBuf,
                                               uint32_t    u4BufLength)
{
    return IMTK_PB_ERROR_CODE_INV_ARG;
}

/*!
 * @}
 */


/*! @} */


