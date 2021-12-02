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
 * @file SdUtil.h
 * @brief SDAL Utility header file
 * @author S/W Platform Lab
 * @date 2007/07/02
 */
 
#ifndef	_SDUTIL_H_
#define	_SDUTIL_H_

/********************************************************************
	INCLUDE FILES
********************************************************************/
#include "SdType.h"

/********************************************************************
	MACRO CONSTANT DEFINITIONS
********************************************************************/

/********************************************************************
	MACRO FUNCTION DEFINITIONS
********************************************************************/

/********************************************************************
	TYPE DEFINITION
********************************************************************/

/**
 * @brief 16Áø¼ö¸¦ KeyÀÔ·ÂÀ¸·Î ¹Þ´Â ÇÔ¼ö.  Hexadecimal Number Input Function.
 * @remarks 16Áø¼ö·Î º¸¿©Áö´Â ¹®ÀÚ¿­À» Á¤¼ö·Î º¯È¯ÇÑ´Ù.
 * @code
 
	char		cmd[1024];

#if DD_DEBUG
	printf("%s: 0x", comment);

 	scanf("%s",cmd);
#else
	if (fd != NULL)
	{
		printf("%s: 0x", comment);
	 	fscanf(fd, "%s",cmd);
	}
	else
	{
		scanf("%s",cmd);
	}
#endif	
	return( strtoul( cmd, (char **) NULL, 16 ) );
 
 * @endcode
 * @param comment KeyÀÔ·Â¿¡ ´ëÇÑ ¼³¸í.
 * @return uint32_t º¯È¯µÈ Á¤¼ö°ª.
 * @see
 */
uint32_t SdReadKeyInput(const char* comment);

/**
 * @brief 10Áø¼ö¸¦ KeyÀÔ·ÂÀ¸·Î ¹Þ´Â ÇÔ¼ö. Decimal Number Input Function.
 * @remarks 10Áø¼ö·Î º¸¿©Áö´ÂÔ ¹®ÀÚ¿­À» Á¤¼ö·Î º¯È¯ÇÑ´Ù.
 * @param comment KeyÀÔ·Â¿¡ ´ëÇÑ ¼³¸í.
 * @return uint32_t º¯È¯µÈ Á¤¼ö°ª
 * @see
 */
uint32_t SdReadDecimalKeyInput(const char* comment);

/**
 * @brief ½Ç¼ö¸¦ KeyÀÔ·ÂÀ¸·Î ¹Þ´Â ÇÔ¼ö. Float Input Function.
 * @remarks ½Ç¼ö·Î º¸¿©Áö´Â ¹®ÀÚ¿­À» ½Ç¼ö·Î º¯È¯ÇÑ´Ù.
 * @param comment KeyÀÔ·Â¿¡ ´ëÇÑ ¼³¸í.
 * @return SdFloat32_t º¯È¯µÈ ½Ç¼ö°ª.
 * @see
 */
SdFloat32_t SdReadFloatKeyInput(const char* comment);

/**
 * @brief Time Check ½ÃÀÛÁ¡À» ¼³Á¤ÇÏ´Â ÇÔ¼ö.
 It sets the start time point to mesure the elaped time.
 * @remarks 
 * @code
 
 	gettimeofday(&tChkStart, NULL);
 	
 * @endcode
 * @return 
 * @see
 */
void SdTimeCheckStart(void);

/**
 * @brief Time CheckÀÇ Á¾·áÁ¡À» ¼³Á¤ÇÏ°í °æ°ú½Ã°£À» ¸®ÅÏÇÑ´Ù.
 It sets the end time point and returns the elapsed time.
 * @remarks 
 * @code
 
 	SdFloat64_t rStart, rEnd, rDiff;
 	
 	gettimeofday(&tChkEnd, NULL);

	rStart = (SdFloat64_t)tChkStart.tv_sec + (SdFloat64_t)tChkStart.tv_usec/(1000*1000);
	rEnd = (SdFloat64_t)tChkEnd.tv_sec + (SdFloat64_t)tChkEnd.tv_usec/(1000*1000);
	rDiff = rEnd - rStart;
	rInitTime += rDiff;
	if(rDiff == 0.0) 
		rDiff = .0001;	
	return rDiff;
 	
 * @endcode 
 * @return SdFloat64_t Start¿Í End »çÀÌÀÇ °æ°ú ½Ã°£.
 * @see
 */
SdFloat64_t SdTimeCheckEnd(void);

/**
 * @brief ÇöÀç ³¯Â¥¿Í ½Ã°£¿¡ ´ëÇÑ ¹®ÀÚ¿­À» ¾ò´Â´Ù. 
 It gets the current time written in string.
 * @remarks 
 * @code
 
 	time_t  curtime;
	struct  tm  *loctime;

	curtime  =  time(NULL);	// ÇöÀçÀÇ ½Ã°£À» ¾ò¾î¶ó
	loctime  =  localtime  (&curtime);	// Áö¿ª½Ã°£ Ç¥ÇöÀ¸·Î º¯È¯ÇÏ¶ó

	return asctime(loctime);
	
 * @endcode 
 * @return uint8_t* ½Ã°£Á¤º¸ ¹®ÀÚ¿­
 * @see
 */
uint8_t* SdGetsCurrentTime(void);

/**
 * @brief UserDataÁ¤º¸ÀÇ EndianÀ» º¯°æÇÏ´Â ÇÔ¼ö.
 * @remarks Application¿¡¼­ ¿ä±¸ÇÏ´Â UserData Æ÷¸ËÀÌ Big EndianÀÌ¹Ç·Î Little Endian Data´Â Big EndianÀ¸·Î 
 ¹Ù²ãÁà¾ß ÇÑ´Ù.
 * @param user_data	º¯°æÇØ¾ßÇÒ userdata ¹®ÀÚ¿­ÀÇ ½ÃÀÛ ÁÖ¼Ò.
 * @param data_length º¯°æÇØ¾ßÇÒ userdata ¹®ÀÚ¿­ÀÇ ±æÀÌ.
 * @return 
 * @code 
 	#define MAX_USRDATA_PKT_BUFSIZE		128
	void SdUserdata_ChangeEndian(uint8_t *user_data, uint32_t data_length)
	{
		uint8_t		TmpData;
		uint32_t	byte;
		uint32_t	Count;			// change endian from little endian to big endian
		
		if (data_length == 0)
		{
			return;
		}
		
		if (data_length > MAX_USRDATA_PKT_BUFSIZE)
		{
			data_length = MAX_USRDATA_PKT_BUFSIZE;
		}
		
		// From Little to Big endian
		for(byte = 0; byte < data_length; byte += 2)
		{
			Count = ( 3 - ((byte%4)<<1) ) + byte; 
			TmpData = user_data[Count];
	
			// swap data..(4,3,2,1)=>(1,2,3,4)
			user_data[Count] = user_data[byte];
			user_data[byte] = TmpData;
		}
	}
 * @endcode
 */
void SdChangeEndian(uint8_t *user_data, uint32_t data_length);

#endif
