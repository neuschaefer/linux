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

/*
 * MUSBStack-S USB Software Platform descriptor analysis library API.
 * $Revision: #1 $
 */

#ifndef __MUSB_DESCRIPTORS_H__
#define __MUSB_DESCRIPTORS_H__

#include "mu_dev.h"

/**
 * Find a descriptor of a given type.
 *
 * @param pConfig pointer to a full configuration descriptor in which to search
 * 
 * @param bDescriptorType the type of descriptor desired
 * 
 * @param bIndex the 0-based index of the instance of the desired descriptor
 * (e.g. if there are 3 HID descriptors, bIndex==2 retrieves the third)
 *
 * @return descriptor pointer if found; NULL on failure
 */
extern const uint8_t* MUSB_FindDescriptor(
    const MUSB_ConfigurationDescriptor* pConfig,
    uint8_t bDescriptorType, 
    uint8_t bIndex);
/**
 * Count alternate interfaces.
 * A given USB device configuration has a specified number of interfaces,
 * given by its bNumInterfaces.
 * Each interface has at least one descriptor, with additional ones
 * for "alternate" settings.
 * The first descriptor for a given interface is called "alternate setting 0."
 * Each interface descriptor (primary or alternate) is followed by 
 * a number of endpoint descriptors, given by bNumEndpoints.
 * Therefore, the zero-based index of endpoint descriptor E
 * for interface N (>0) is:
 <pre>
 iEnd = E + bNumEndpoints[0] * NumAlternates[0] + ... + bNumEndpoints[N-1] * NumAlternates[N-1]
 </pre>
 * where bNumEndpoints[M] is interface M's bNumEndpoints field,
 * and NumAlternates[M] is the total number of alternate settings 
 * (including setting 0) for interface M.
 * The array of NumAlternates is computed by this function.
 * It is the index iEnd that should be passed to MUSB_FindDescriptor
 * to find the desired endpoint's descriptor.
 * In a similar fashion, the index of the descriptor for alternate
 * setting A for interface N (>0) is:
 <pre>
 iAlt = A + NumAlternates[0] + ... + NumAlternates[N-1]
 </pre>
 * so iAlt would be passed to MUSB_FindDescriptor to find the desired
 * alternate interface's descriptor.
 *
 * @param aCount array in which to store NumAlternates for each interface.
 * The appropriate size is given by the configuration's bNumInterfaces,
 * so the array is assumed at least this size.
 *
 * @param pConfig pointer to configuration descriptor
 *
 * @return the number of interfaces actually found (therefore the number of filled
 * elements in aCount)
 */
#if 0 
extern uint8_t MUSB_CountAlternates(uint8_t* aCount, 
				    const MUSB_ConfigurationDescriptor* pConfig);
#endif
/**
 * Find an interface descriptor
 *
 * @param pConfig pointer to configuration descriptor
 * 
 * @param bInterfaceNumber the number of the desired interface
 * 
 * @param bAlternateSetting the desired alternate setting
 *
 * @return pointer to the requested descriptor, or NULL if not found.
 * This is actually a pointer within a contiguous descriptor block,
 * so it can be used in a call to MUSB_FindDescriptorInBlock
 * to find class-specific interface descriptors.
 */
extern const MUSB_InterfaceDescriptor* MUSB_FindInterfaceDescriptor(
    const MUSB_ConfigurationDescriptor* pConfig, 
    uint8_t bInterfaceNumber, 
    uint8_t bAlternateSetting);

/**
 * Find an endpoint descriptor
 *
 * @param pConfig pointer to the configuration in which the interface resides,
 * to limit the search based on wTotalLength
 * 
 * @param pInterfaceDescriptor pointer to an interface descriptor,
 * e.g. from a successful call to MUSB_FindInterfaceDescriptor
 * 
 * @param bEndpointIndex the 0-based index of the desired endpoint descriptor
 *
 * @return pointer to the requested descriptor, or NULL if not found.
 * This is actually a pointer within a contiguous descriptor block,
 * so it can be used in a call to MUSB_FindDescriptorInBlock
 * to find class-specific endpoint descriptors.
 */
extern const MUSB_EndpointDescriptor* MUSB_FindEndpointDescriptor(
    const MUSB_ConfigurationDescriptor* pConfig, 
    const MUSB_InterfaceDescriptor* pInterfaceDescriptor,
    uint8_t bEndpointIndex);

/**
 * Find an arbitrary descriptor within a descriptor block.
 * A descriptor block could be the return value of 
 * MUSB_FindInterfaceDescriptor or MUSB_FindEndpointDescriptor.
 * This is useful for finding class-specific descriptors associated with
 * the already-found type, since they are required to follow it.
 *
 * @param pConfig pointer to the configuration in which the block resides,
 * to limit the search based on wTotalLength
 * 
 * @param pDescriptorBlock pointer to a descriptor of arbitrary type.
 * The search begins here, and is limited to either the end of the
 * configuration or when another descriptor of this same type is encountered.
 * 
 * @param bDescriptorType the type of descriptor desired
 * 
 * @param bIndex the index (counting from 0) of the instance of the desired descriptor
 * (e.g. if there are 3 HID descriptors, bIndex==2 retrieves the third)
 *
 * @return pointer to the requested descriptor, or NULL if not found
 */
extern const uint8_t* MUSB_FindDescriptorInBlock(
    const MUSB_ConfigurationDescriptor* pConfig, 
    const uint8_t* pDescriptorBlock, 
    uint8_t bDescriptorType, 
    uint8_t bIndex);

#endif	/* multiple inclusion protection */
