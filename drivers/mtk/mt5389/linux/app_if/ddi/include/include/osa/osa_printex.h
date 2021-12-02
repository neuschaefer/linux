/******************************************************************************
 *	 DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *	 Copyright(c) 1999 by LG Electronics Inc.
 *
 *	 All rights reserved. No part of this work may be reproduced, stored in a
 *	 retrieval system, or transmitted by any means without prior written
 *	 permission of LG Electronics Inc.
 *****************************************************************************/

/** @file osa_printex.h
 *
 *	System Call debug API.
 *
 *	@author 	dhjung
 *	- @author	changwook.im
 *	@version	0.2
 *	@date		2011. 07. 12
 *	@note
 *	@see
 */

/******************************************************************************
	Header File Guarder
******************************************************************************/
#ifndef _OSA_PRINTEX_H_
#define _OSA_PRINTEX_H_

#ifndef WIN32_SIM
#include "common.h"
#else
#include "common_win32.h"
#endif

#include	"common.h"
#include	"xlibc_api.h"
#include 	"xcmd_api.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************
	상수 정의(Constant Definitions)
******************************************************************************/
#define ALL_PRINT_MASK									0xFFFFFFFF
#define NO_PRINT_MASK									0x00000000
/******************************************************************************
	타입 정의(Types Definitions)
******************************************************************************/
typedef  cmd_mask_t OSA_PRINT_MASK_T ;

/******************************************************************************
	매크로 함수 정의 (Macro Definitions)
******************************************************************************/

/*************************************************************************************************************/
/*                                          Print Format Check Attribute for OSA_DBG_PrintEx                 */
/*************************************************************************************************************/
/* Default Format Check : Disabled for windows emulator
 * [L8] 20100721 seokjoo.lee : Default : Enable FORMAT_CHECK.
 * [L8] 20100709 seokjoo.lee : strict format check for OSA print series.
 * Usage : for checking, define _USE_PRINT_FORMAT_CHECK as positive non-zero value in profer .mk file.
 */
#if ( defined (_EMUL_WIN) )
#define _USE_PRINT_FORMAT_CHECK 0
#endif
#if ( !defined(_USE_PRINT_FORMAT_CHECK) )
#define _USE_PRINT_FORMAT_CHECK 0
#endif

/* aliaas of __attribute__ (printf())
 * see __printf(a,b) in include/linux/compiler-gcc.h
 */
#if (_USE_PRINT_FORMAT_CHECK > 0)
#define __PRINTF(_f,_a)		__attribute__ (( format (printf, _f, _a) ))
#else
#define __PRINTF(_f,_a)
#endif

/*************************************************************************************************************/
/*                                          Print Alloc per bit                                              */
/*************************************************************************************************************/
#define DEFAULT_PRINT_MASK CMD_MASK_DEFAULT

#define PRNT_MOD_UI1				(0)
#define PRNT_MOD_UI2				(1)
#define PRNT_MOD_MW1				(2)
#define PRNT_MOD_MW2				(3)
#define PRNT_MOD_MW3				(4)
#define PRNT_MOD_MW4				(5)
#define PRNT_MOD_DD1				(6)
#define PRNT_MOD_DD2				(7)
#define PRINT_MOD_DBG				(8)
/*************************************************************************************************************/
/*                                          Print Definition                                                 */
/*************************************************************************************************************/
#define OSA_RawPrintf									rawprintf
#undef assert
#define OSA_ASSERT(condition)							xlibc_assert((condition), "1")
#define assert(x)										OSA_ASSERT(x)


#ifndef _EMUL_WIN
#define OSA_DBG_CriticalError(format, args...)			xlibc_critical(format, ##args)
#define OSA_DBG_WarningError(format, args...)			xlibc_warning(format, ##args)
#define OSA_DBG_GetStringInput(_pm, _ps)					(cmd_io_read_string(_pm, _ps, 127))

#define OSA_DBG_PrintUi1(number, format, args...)		cmd_mask_print(PRNT_MOD_UI1, number, format, ##args)
#define OSA_DBG_EnablePrintMaskUi1(number) 				cmd_mask_bit_enable(PRNT_MOD_UI1, number)
#define OSA_DBG_DisablePrintMaskUi1(number)				cmd_mask_bit_disable(PRNT_MOD_UI1, number)

#define OSA_DBG_PrintUi2(number, format, args...)		cmd_mask_print(PRNT_MOD_UI2, number, format, ##args)
#define OSA_DBG_EnablePrintMaskUi2(number) 				cmd_mask_bit_enable(PRNT_MOD_UI2, number)
#define OSA_DBG_DisablePrintMaskUi2(number)				cmd_mask_bit_disable(PRNT_MOD_UI2, number)

#define OSA_DBG_PrintMw1(number, format, args...)		cmd_mask_print(PRNT_MOD_MW1, number, format, ##args)
#define OSA_DBG_EnablePrintMaskMw1(number) 				cmd_mask_bit_enable(PRNT_MOD_MW1, number)
#define OSA_DBG_DisablePrintMaskMw1(number)				cmd_mask_bit_disable(PRNT_MOD_MW1, number)

#define OSA_DBG_PrintMw2(number, format, args...)		cmd_mask_print(PRNT_MOD_MW2, number, format, ##args)
#define OSA_DBG_EnablePrintMaskMw2(number) 				cmd_mask_bit_enable(PRNT_MOD_MW2, number)
#define OSA_DBG_DisablePrintMaskMw2(number)				cmd_mask_bit_disable(PRNT_MOD_MW2, number)

#define OSA_DBG_PrintMw3(number, format, args...)		cmd_mask_print(PRNT_MOD_MW3, number, format, ##args)
#define OSA_DBG_EnablePrintMaskMw3(number) 				cmd_mask_bit_enable(PRNT_MOD_MW3, number)
#define OSA_DBG_DisablePrintMaskMw3(number)				cmd_mask_bit_disable(PRNT_MOD_MW3, number)

#define OSA_DBG_PrintMw4(number, format, args...)		cmd_mask_print(PRNT_MOD_MW4, number, format, ##args)
#define OSA_DBG_EnablePrintMaskMw4(number) 				cmd_mask_bit_enable(PRNT_MOD_MW4, number)
#define OSA_DBG_DisablePrintMaskMw4(number)				cmd_mask_bit_disable(PRNT_MOD_MW4, number)

#define OSA_DBG_PrintDrv1(number, format, args...)		cmd_mask_print(PRNT_MOD_DD1, number, format, ##args)
#define OSA_DBG_EnablePrintMaskDrv1(number)				cmd_mask_bit_enable(PRNT_MOD_DD1, number)
#define OSA_DBG_DisablePrintMaskDrv1(number)			cmd_mask_bit_disable(PRNT_MOD_DD1, number)

#define OSA_DBG_PrintDrv2(number, format, args...)		cmd_mask_print(PRNT_MOD_DD2, number, format, ##args)
#define OSA_DBG_EnablePrintMaskDrv2(number)				cmd_mask_bit_enable(PRNT_MOD_DD2, number)
#define OSA_DBG_DisablePrintMaskDrv2(number)			cmd_mask_bit_disable(PRNT_MOD_DD2, number)

/*************************************************************************************************************/
/*                                        Print 할당 및 Define                                               */
/*************************************************************************************************************/
/*------------ System Call ------------*/
#define OSA_PRINT(format, args...) 						tprint0n(format, ##args)
#define OSA_RPRINT(format, args...) 					rprint0n(format, ##args)
#define OSA_PRINT_ERROR(format, args...)				OSA_DBG_CriticalError(format, ##args)
#define OSA_PRINT_WARNING(format, args...) 				OSA_DBG_WarningError(format, ##args)
#define	API_ERR_PRINT(format, args...)					cmd_mask_print(PRINT_MOD_DBG, 0, format, ##args)

/*---------------- Ui1 ----------------*/
#define UI_MAIN_PrintEx(format, args...)				OSA_DBG_PrintUi1 ( 0, format, ##args)
#define	UI_GLOBAL_PRINT(format, args...)				OSA_DBG_PrintUi1 ( 1, format, ##args)	/* UI GLOBAL TEST Mode */
#define	UI_MENU_PRINT(format, args...) 					OSA_DBG_PrintUi1 ( 2, format, ##args)	/* Setup Menu */
#define	UI_HOME_PRINT(format, args...) 					OSA_DBG_PrintUi1 ( 3, format, ##args)	/* Home Menu */
#define	UI_PCCTL_PRINT(format, args...) 				OSA_DBG_PrintUi1 ( 4, format, ##args)	/* Pc Control */
#define	UI_ACAP_KEY_PRINT(format, args...) 				OSA_DBG_PrintUi1 ( 5, format, ##args)	/* UI ACAP Key */
#define UI_KEY_PRINT(format, args...)					OSA_DBG_PrintUi1 ( 6, format, ##args)	/* UI Key */
#define	UI_GUIDE_PRINT(format, args...) 				OSA_DBG_PrintUi1 ( 7, format, ##args)	/* GUIDE */
#define	UI_FULLINFO_PRINT(format, args...) 				OSA_DBG_PrintUi1 ( 8, format, ##args)	/* FULLINFO */
#define	UI_BRINFO_PRINT(format, args...) 				OSA_DBG_PrintUi1 ( 9, format, ##args)	/* BRINFO */
#define	UI_PGBAR_PRINT(format, args...) 				OSA_DBG_PrintUi1 (10, format, ##args)	/* PGBAR */
#define	UI_USB_PRINT(format, args...) 					OSA_DBG_PrintUi1 (11, format, ##args)	/* USB */
#define	UI_PHOTO_PRINT(format, args...) 				OSA_DBG_PrintUi1 (12, format, ##args)	/* Photo List */
#define	UI_MUSIC_PRINT(format, args...) 				OSA_DBG_PrintUi1 (13, format, ##args)	/* Music List */
#define	UI_LIST_PRINT(format, args...) 					OSA_DBG_PrintUi1 (14, format, ##args)	/* List Manager */
#define	UI_RES_PRINT(format, args...) 					OSA_DBG_PrintUi1 (15, format, ##args)	/* Reserve List*/
#define	UI_RESREC_PRINT(format, args...) 				OSA_DBG_PrintUi1 (16, format, ##args)	/* Reserve Record */
#define	UI_RECLIST_PRINT(format, args...) 				OSA_DBG_PrintUi1 (17, format, ##args)	/* Record List */
#define UI_SUMODE_PRINT(format, args...)				OSA_DBG_PrintUi1 (18, format, ##args)	/* UI Special User Mode*/
#define	UI_DVRMODE_PRINT(format, args...)				OSA_DBG_PrintUi1 (19, format, ##args)	/* UI DVR Mode*/
#define	UI_PATH_PRINT(format, args...)					OSA_DBG_PrintUi1 (20, format, ##args)	/* UI GLOBAL TEST Mode*/
#define	UI_INSTALL_PRINT(format, args...)				OSA_DBG_PrintUi1 (21, format, ##args)	/* Install Guide */
#define	UI_CI_PRINT(format, args...) 					OSA_DBG_PrintUi1 (22, format, ##args)	/* CI Print */
#define	UI_PQL_PRINT(format, args...) 					OSA_DBG_PrintUi1 (23, format, ##args)	/* PQL Print */
#define	UI_SUBT_PRINT(format, args...) 					OSA_DBG_PrintUi1 (24, format, ##args)	/* Subtitle Print */
#define	UI_AUTOAV_PRINT(format, args...) 				OSA_DBG_PrintUi1 (25, format, ##args)	/* AUTO AV */
#define	UI_DIRKEY_PRINT(format, args...) 				OSA_DBG_PrintUi1 (26, format, ##args)	/* Direct Key input */
#define	UI_OAD_PRINT(format, args...) 					OSA_DBG_PrintUi1 (27, format, ##args)	/* OAD UI */
#define UI_ACAP_PRINT(format, args...)					OSA_DBG_PrintUi1 (28, format, ##args)	/* ACAP */
#define UI_HOTEL_PRINT(format, args...)					OSA_DBG_PrintUi1 (29, format, ##args)	/* Hotel Mode */
#define UI_POLLING_PRINT(format, args...)				OSA_DBG_PrintUi1 (30, format, ##args)	/* UI Polling Print */
#define UI_HCEC_PRINT(format, args...)					OSA_DBG_PrintUi1 (31, format, ##args)	/* UI HCEC Print */

/*---------------- Ui2 ----------------*/
#define MATRIX_PRINT(format, args...) 					OSA_DBG_PrintUi2 ( 0, format, ##args)	/* UI Matrix Print */
#define UI_VIDEO_PRINT(format, args...)					OSA_DBG_PrintUi2 ( 1, format, ##args)	/* UI Video Print */
#define UI_BT_PRINT(format, args...)					OSA_DBG_PrintUi2 ( 2, format, ##args)	/* UI Blue Tooth Print */	//hskim 갯수 부족으로 HCEC와 같이 사용함.
#define UI_SYS_PRINT(format, args...)					OSA_DBG_PrintUi2 ( 3, format, ##args)	/* System 관련 메시지 출력 */
#define UI_TOOLKIT_PRINT(format, args...)				OSA_DBG_PrintUi2 ( 4, format, ##args)	/* Toolkit 관련 메시지 출력 */
#define UI_EPGSEG_PRINT(format, args...)				OSA_DBG_PrintUi2 ( 5, format, ##args)	/* DVR_EPGSEG 관련 메시지 출력 */
#define UI_DASY_PRINT(format, args...)					OSA_DBG_PrintUi2 ( 6, format, ##args)	/* DVR_UIDASY 관련 메시지 출력 */
#define UI_PGBARTIME_PRINT(format, args...)				OSA_DBG_PrintUi2 ( 7, format, ##args)	/* DVR PGBar 관련 메시지 출력 */
#define UI_ATUNING_PRINT(format, args...)				OSA_DBG_PrintUi2 ( 8, format, ##args)	/* UI AutoTuning Print */
#define UI_MTUNING_PRINT(format, args...)				OSA_DBG_PrintUi2 ( 9, format, ##args)	/* UI ManualTuning Print */
#define UI_MOVIE_PRINT(format, args...)					OSA_DBG_PrintUi2 (10, format, ##args)	/* Movie List */
#define UI_ACMS_PRINT(format, args...)					OSA_DBG_PrintUi2 (11, format, ##args)	/* ACMS */
#define UI_NET_PRINT(format, args...)					OSA_DBG_PrintUi2 (12, format, ##args)	/* UI Network print */
#define UI_NETFLIX_PRINT(format, args...)				OSA_DBG_PrintUi2 (13, format, ##args)	/* UI Netflix print */
#define UI_NSU_PRINT(format, args...)					OSA_DBG_PrintUi2 (14, format, ##args)	/* UI Netflix print */
#define UI_YOUTUBE_PRINT(format, args...)				OSA_DBG_PrintUi2 (15, format, ##args) 	/* UI YouTube print */
#define UI_RATING_PRINT(format, args...)					OSA_DBG_PrintUi2 (16, format, ##args) 	/* UI Rating print */
#define UI_MHP_PRINT(format, args...)					OSA_DBG_PrintUi2 (17, format, ##args) 	/* UI MHP print */
#define UI_ANI_PRINT(format, args...)					OSA_DBG_PrintUi2 (18, format, ##args)	/* Animation Task 관련 메시지 출력 */
#define UI_MULTICHANNEL_PRINT(format, args...)			OSA_DBG_PrintUi2 (19, format, ##args)	/* Multi-Channel */
#define	UI_MHP_KEY_PRINT(format, args...) 				OSA_DBG_PrintUi2 (20, format, ##args)	/* UI MHP Key */
#define UI_IMAGE_PRINT(format, args...)					OSA_DBG_PrintUi2 (21, format, ##args)	/* UI Image Process */
#ifdef INCLUDE_VCS
#define UI_VCS_PRINT(format, args...)					OSA_DBG_PrintUi2 (22, format, ##args)	/* VCS print */
#endif //INCLUDE_VCS

#ifdef INCLUDE_BML
#define	UI_BML_KEY_PRINT(format, args...) 				OSA_DBG_PrintUi2 (23, format, ##args)	/* UI BML Key */
#define	UI_BML_PRINT(format, args...) 					OSA_DBG_PrintUi2 (24, format, ##args)	/* UI BML Print */
#endif

#define UI_MOUSE_PRINT(format, args...)					OSA_DBG_PrintUi2 (29, format, ##args)	/* UI Image Process */
#ifdef INCLUDE_UI_EFFECT
#define UI_EFFECT_PRINT(format, args...)				OSA_DBG_PrintUi2 (30, format, ##args)	/* effect */
#define UI_EFFECT_TIME_PRINT(format, args...)			OSA_DBG_PrintUi2 (31, format, ##args)	/* effect */
#endif


/*---------------- Mw1 ----------------*/
/*---------------- CM -----------------*/
#define CM_PRINT_0(format, args...)						OSA_DBG_PrintMw1 ( 0, format, ##args)
#define CM_PRINT_1(format, args...)						OSA_DBG_PrintMw1 ( 1, format, ##args)
#define CM_PRINT_2(format, args...)						OSA_DBG_PrintMw1 ( 2, format, ##args)
#define CM_PRINT_3(format, args...)						OSA_DBG_PrintMw1 ( 3, format, ##args)
#define CM_PRINT_4(format, args...)						OSA_DBG_PrintMw1 (28, format, ##args)
#define CM_PRINT_5(format, args...)						OSA_DBG_PrintMw1 (29, format, ##args)
#define CM_PRINT_6(format, args...)						OSA_DBG_PrintMw1 (30, format, ##args)
#define CM_PRINT_7(format, args...)						OSA_DBG_PrintMw1 (31, format, ##args)

/*--------------- SI ------------------*/
#define SI_PRINT_1(format, args...)						OSA_DBG_PrintMw1 ( 6, format, ##args)
#define SI_PRINT_2(format, args...)						OSA_DBG_PrintMw1 ( 7, format, ##args)
#define SI_PRINT_3(format, args...)						OSA_DBG_PrintMw1 ( 8, format, ##args)
#define SI_PRINT_ERR(format, args...)					OSA_DBG_PrintMw1 ( 9, format, ##args)

/*--------------- PSIP ----------------*/
#define DBG_Sem1(format, args...)						OSA_DBG_PrintMw1 ( 4, format, ##args)
#define DBG_Sem2(format, args...)						OSA_DBG_PrintMw1 ( 5, format, ##args)
#define DBG_Msg1(format, args...)						OSA_DBG_PrintMw1 ( 6, format, ##args)
#define DBG_Msg2(format, args...)						OSA_DBG_PrintMw1 ( 7, format, ##args)
#define DBG_Msg3(format, args...)						OSA_DBG_PrintMw1 ( 8, format, ##args)
#define DBG_Msg4(format, args...)						OSA_DBG_PrintMw1 ( 8, format, ##args)
#define ERR_Msg(format, args...)						OSA_DBG_PrintMw1 ( 9, format, ##args)

/*--------------- DTVCC ----------------*/
#define DCC_PRINTF(format, args...)						OSA_DBG_PrintMw1 (10, format, ##args)
#define DCC_WARN_PRINTF(format, args...)				OSA_DBG_PrintMw1 (11, format, ##args)
#define DCC_ERR_PRINTF(format, args...)					OSA_DBG_PrintMw1 (12, format, ##args)

/*--------------- SUBT ----------------*/
#define SUBT_PRINTF(format, args...)					OSA_DBG_PrintMw1 (10, format, ##args)
#define SUBT_WARN_PRINTF(format, args...)				OSA_DBG_PrintMw1 (11, format, ##args)
#define SUBT_ERR_PRINTF(format, args...)				OSA_DBG_PrintMw1 (12, format, ##args)

/*--------------- EMF -----------------*/
#define	EME_INFO_PRINTF(format, args... ) 				OSA_DBG_PrintMw1 (13, format, ##args)
#define	EME_JPEG_PRINTF(format, args... ) 				OSA_DBG_PrintMw1 (14, format, ##args)
#define	EME_SUBT_PRINTF(format, args... ) 				OSA_DBG_PrintMw1 (15, format, ##args)
#define	EME_POLL_PRINTF(format, args... ) 				OSA_DBG_PrintMw1 (16, format, ##args)

/*--------------- MLM -----------------*/
#define	MLM_INFO_PRINTF(format, args... ) 				OSA_DBG_PrintMw1 (17, format, ##args)
#define	MLM_WARNING_PRINTF(format, args... ) 			OSA_DBG_PrintMw1 (18, format, ##args)
#define	MLM_ERROR_PRINTF(format, args... ) 				OSA_DBG_PrintMw1 (19, format, ##args)

/*--------------- ATVCC ----------------*/
#define	ACC_PRINTF(format, args... ) 					OSA_DBG_PrintMw1 (20, format, ##args)
#define	ACC_ERR_PRINTF(format, args... ) 				OSA_DBG_PrintMw1 (21, format, ##args)

/*--------------- TTX -----------------*/
#define	TTX_PRINTF(format, args... ) 					OSA_DBG_PrintMw1 (20, format, ##args)
#define	TTX_ERR_PRINTF(format, args... ) 				OSA_DBG_PrintMw1 (21, format, ##args)

/*--------------- DASY ----------------*/
#define	DASY_PRINTF_1(format, args...)					OSA_DBG_PrintMw1 (22, format, ##args)
#define	DASY_PRINTF_2(format, args...)					OSA_DBG_PrintMw1 (23, format, ##args)
#define	DASY_PRINTF_3(format, args...)					OSA_DBG_PrintMw1 (24, format, ##args)

/*---------------- MFS ----------------*/
#define	MFS_PRINT_ERR(format, args...)					OSA_DBG_PrintMw1 (25, format, ##args)
#define	MFS_PRINT_MSG1(format, args...)					OSA_DBG_PrintMw1 (26, format, ##args)	/* fdisk, format, mount */
#define	MFS_PRINT_MSG2(format, args...)					OSA_DBG_PrintMw1 (27, format, ##args)	/* MFS_API, Main, MFS */




/*---------------- Mw2 ----------------*/
/*---------------- MRE ----------------*/
#define	MRE_INFO_PRINTF(format, args...) 				OSA_DBG_PrintMw2 ( 0, format, ##args)
#define	MRE_WARNING_PRINTF(format, args...) 			OSA_DBG_PrintMw2 ( 1, format, ##args)
#define	MRE_ERROR_PRINTF(format, args...) 				OSA_DBG_PrintMw2 ( 2, format, ##args)
#define	MRE_FATAL_PRINTF(format, args...) 				OSA_DBG_PrintMw2 ( 3, format, ##args)
#define	MRE_PATH_PRINTF(format, args...) 				OSA_DBG_PrintMw2 ( 4, format, ##args)

/*--------------- NSU ----------------*/
#define NSU_PRINT(format, args...)						OSA_DBG_PrintMw2 ( 5, format, ##args)

/*--------------- UTIL ----------------*/
#define	UTIL_PRINT(format, args...) 					OSA_DBG_PrintMw2 ( 6, format, ##args)

/*--------------- SWU -----------------*/
#define	SWU_API_PRINTF(format, args...) 				OSA_DBG_PrintMw2 ( 7, format, ##args)
#define	SWU_EPK_PRINTF(format, args...) 				OSA_DBG_PrintMw2 ( 8, format, ##args)
#define	SWU_MTD_PRINTF(format, args...) 				OSA_DBG_PrintMw2 ( 9, format, ##args)
#define	SWU_UTIL_PRINTF(format, args...) 				OSA_DBG_PrintMw2 (10, format, ##args)

/*-------------- AVBLOCK --------------*/
#define AV_BLOCK_PRINTF(format, args...)				OSA_DBG_PrintMw2 (11, format, ##args)

/*--------------- SYS -----------------*/
#define	AUDIO_API_PRINTF(format, args...) 				OSA_DBG_PrintMw2 (12, format, ##args)
#define	VIDEO_API_PRINT(format, args...) 				OSA_DBG_PrintMw2 (13, format, ##args)
#define	VIDEO_FMT_PRINT(format, args...)					OSA_DBG_PrintMw2 (14, format, ##args)

/*--------------- OTA -----------------*/
#define	OTA_PRINTF1(format, args...)					OSA_DBG_PrintMw2 (15, format, ##args)
#define	OTA_PRINTF2(format, args...)					OSA_DBG_PrintMw2 (16, format, ##args)

/*---------------- CI -----------------*/
#define CI_DBG_PRINT_MAIN(format, args...)				OSA_DBG_PrintMw2 (17, format, ##args)
#ifdef INCLUDE_CIPLUS
#define CI_DBG_PRINT_APP		CI_DBG_PRINT_APP_RESMGR
#define CI_DBG_PRINT_RESMGR		CI_DBG_PRINT_APP_RESMGR
#define CI_DBG_PRINT_APPINFO	CI_DBG_PRINT_APPINFO_HOSTCTL
#define CI_DBG_PRINT_HOSTCTL	CI_DBG_PRINT_APPINFO_HOSTCTL

#define CI_DBG_PRINT_LINK(format, args...)				OSA_DBG_PrintMw2 (18, format, ##args)
#define CI_DBG_PRINT_TRANS(format, args...)				OSA_DBG_PrintMw2 (19, format, ##args)
#define CI_DBG_PRINT_SESSION(format, args...)			OSA_DBG_PrintMw2 (20, format, ##args)
#define CI_DBG_PRINT_APP_RESMGR(format, args...)		OSA_DBG_PrintMw2 (21, format, ##args)
#define CI_DBG_PRINT_APPINFO_HOSTCTL(format, args...)	OSA_DBG_PrintMw2 (22, format, ##args)
#define CI_DBG_PRINT_CAS(format, args...)				OSA_DBG_PrintMw2 (23, format, ##args)
#define CI_DBG_PRINT_DATETIME(format, args...)			OSA_DBG_PrintMw2 (24, format, ##args)
#define CI_DBG_PRINT_MMI(format, args...)				OSA_DBG_PrintMw2 (25, format, ##args)
#define CI_DBG_PRINT_APPMMI(format, args...)			OSA_DBG_PrintMw2 (26, format, ##args)
#define CI_DBG_PRINT_LANGCOUNTRY(format, args...)		OSA_DBG_PrintMw2 (27, format, ##args)
#define CI_DBG_PRINT_UPGRADE(format, args...)			OSA_DBG_PrintMw2 (28, format, ##args)
#define CI_DBG_PRINT_CC(format, args...)				OSA_DBG_PrintMw2 (29, format, ##args)
#else
#define CI_DBG_PRINT_PHY(format, args...)				OSA_DBG_PrintMw2 (18, format, ##args)
#define CI_DBG_PRINT_CIS(format, args...)				OSA_DBG_PrintMw2 (19, format, ##args)
#define CI_DBG_PRINT_LINK(format, args...)				OSA_DBG_PrintMw2 (20, format, ##args)
#define CI_DBG_PRINT_TRANS(format, args...)				OSA_DBG_PrintMw2 (21, format, ##args)
#define CI_DBG_PRINT_SESSION(format, args...)			OSA_DBG_PrintMw2 (22, format, ##args)
#define CI_DBG_PRINT_APP(format, args...)				OSA_DBG_PrintMw2 (23, format, ##args)
#define CI_DBG_PRINT_RESMGR(format, args...)			OSA_DBG_PrintMw2 (24, format, ##args)
#define CI_DBG_PRINT_APPINFO(format, args...)			OSA_DBG_PrintMw2 (25, format, ##args)
#define CI_DBG_PRINT_CAS(format, args...)				OSA_DBG_PrintMw2 (26, format, ##args)
#define CI_DBG_PRINT_DATETIME(format, args...)			OSA_DBG_PrintMw2 (27, format, ##args)
#define CI_DBG_PRINT_HOSTCTL(format, args...)			OSA_DBG_PrintMw2 (28, format, ##args)
#define CI_DBG_PRINT_MMI(format, args...)				OSA_DBG_PrintMw2 (29, format, ##args)
#endif
#define CI_DBG_PRINT_ERR(format, args...)				OSA_DBG_PrintMw2 (30, format, ##args)

/*--------------- CECP ----------------*/
#define CECP_PRINTF(format, args...)					OSA_DBG_PrintMw2 (31, format, ##args)

/*---------------- Mw3 ----------------*/
#ifdef INCLUDE_ACAP
/*--------------- ACAP ----------------*/
#define	ACAP_MODULE_PRINT(format, args...)				OSA_DBG_PrintMw3 ( 0, format, ##args)
#define ACAP_SI_PRINT(format, args...) 					OSA_DBG_PrintMw3 ( 1, format, ##args)
#define ACAP_OSA_PRINT(format, args...) 					OSA_DBG_PrintMw3 ( 2, format, ##args)
#define ACAP_CM_PRINT(format, args...) 					OSA_DBG_PrintMw3 ( 3, format, ##args)
#define	ACAP_SYSTEM_PRINT(format, args...) 				OSA_DBG_PrintMw3 ( 4, format, ##args)
#define ACAP_AV_PRINT(format, args...) 					OSA_DBG_PrintMw3 ( 5, format, ##args)
#define	ACAP_API_PRINT(format, args...) 					OSA_DBG_PrintMw3 ( 6, format, ##args)
#define ACAP_DEVICE_PRINT(format, args...) 				OSA_DBG_PrintMw3 ( 7, format, ##args)
#define ACAP_OSD_PRINT(format, args...) 					OSA_DBG_PrintMw3 ( 8, format, ##args)

#else
/*--------------- MHEG ----------------*/
#define	MHEG_MODULE_PRINT(format, args...)				OSA_DBG_PrintMw3 ( 0, format, ##args)
#define	MHEG_DSM_PRINT(format, args...)					OSA_DBG_PrintMw3 ( 1, format, ##args)
#define	MHEG_API_PRINT(format, args...)					OSA_DBG_PrintMw3 ( 2, format, ##args)
#define	MHEG_CMSI_PRINT(format, args...)					OSA_DBG_PrintMw3 ( 3, format, ##args)
#define	MHEG_ESM_PRINT(format, args...)					OSA_DBG_PrintMw3 ( 4, format, ##args)
#define	MHEG_AV_PRINT(format, args...)					OSA_DBG_PrintMw3 ( 5, format, ##args)
#define	MHEG_DEV_PRINT(format, args...)					OSA_DBG_PrintMw3 ( 6, format, ##args)
#define	MHEG_NET_PRINT(format, args...)					OSA_DBG_PrintMw3 ( 7, format, ##args)
#define	MHEG_OSD_PRINT(format, args...)					OSA_DBG_PrintMw3 ( 8, format, ##args)
#ifdef INCLUDE_CIPLUS
#define	MHEG_CIPLUS_PRINT(format, args...)				OSA_DBG_PrintDrv2(11, format, ##args)
#endif
#endif

/*-------------- EDBMS ----------------*/
#define EDBMS_PRINTF0(format, args...)					OSA_DBG_PrintMw3 ( 9, format, ##args)
#define EDBMS_PRINTF1(format, args...)					OSA_DBG_PrintMw3 (10, format, ##args)

/*--------------- SDM -----------------*/
#define	USB_PRINT_MSG(format, args...)					OSA_DBG_PrintMw3 (11, format, ##args)

#ifdef INCLUDE_MCB
/*--------------- MCB -----------------*/
#define	MCB_PRINTF(format, args...)						OSA_DBG_PrintMw3 (10, format, ##args)
#endif

/*-------------- ACMS ---------------*/
#define ACMS_DBG_PRINTF(format, args...)				OSA_DBG_PrintMw3 (14, format,##args);
#define ACMS_VPS_PRINTF(format, args...)				OSA_DBG_PrintMw3 (15, format,##args);
#define ACMS_P830F1_PRINTF(format, args...)				OSA_DBG_PrintMw3 (16, format,##args);
#define ACMS_P830F2_PRINTF(format, args...)				OSA_DBG_PrintMw3 (17, format,##args);
#define ACMS_VBI_PRINTF(format, args...)				OSA_DBG_PrintMw3 (18, format,##args);

/*-------------- WIDGET DIARY ---------------*/
#define	DIARY_PRINTF(format, args...)					OSA_DBG_PrintMw3 (19, format, ##args);

/*--------------- Bluetooth ----------------*/
#define	BT_INTERNAL_PRINT(format, args...)				OSA_DBG_PrintMw3 ( 20, format, ##args)
#define	BT_OSADAP_PRINT(format, args...)				OSA_DBG_PrintMw3 ( 21, format, ##args)
#define	BT_API_PRINT(format, args...)				    OSA_DBG_PrintMw3 ( 22, format, ##args)
#define	BT_ENCODE_PRINT(format, args...)				OSA_DBG_PrintMw3 ( 23, format, ##args)
#define	BT_DECODE_PRINT(format, args...)				OSA_DBG_PrintMw3 ( 24, format, ##args)
#define	BT_ARBITER_PRINT(format, args...)				OSA_DBG_PrintMw3 ( 25, format, ##args)

/*---------------- CIFS ----------------*/
#define	CIFS_PRINT(format, args...)						OSA_DBG_PrintMw3 ( 26, format, ##args)

/*--------------- YWE ----------------*/
#define YWE_PRINT(format, args...)						OSA_DBG_PrintMw3 ( 27, format, ##args)

/*--------------- STREAM ----------------*/
#define STREAM_PRINT(format, args...)					OSA_DBG_PrintMw3 ( 13, format, ##args)
#define STREAM_TRACE(format, args...)					OSA_DBG_PrintMw3 ( 19, format, ##args)

/*--------------- HTTP client -----------*/
#define HTTP_DEBUG_PRINT(format, args...)				OSA_DBG_PrintMw3(12, format, ##args)

/*--------------- ADCC for BR----------------*/
#define	ADCC_INFO_PRINTF(format, args... ) 					OSA_DBG_PrintMw3 (28, format, ##args)
#define	ADCC_ERROR_PRINTF(format, args... ) 				OSA_DBG_PrintMw3 (29, format, ##args)
/*--------------- OAE ----------------*/
#define	OAE_PRINTF(format, args...)						OSA_DBG_PrintMw3 ( 30, format, ##args)
#define	OAE_ERR_PRINTF(format, args...)					OSA_DBG_PrintMw3 ( 31, format, ##args)

/*---------------- Mw4 ----------------*/
#define	DLNA_DMP_INFO_PRINTF(format, args...)			OSA_DBG_PrintMw4 ( 0, format, ##args)
#define	DLNA_DMP_ERR_PRINTF(format, args...)			OSA_DBG_PrintMw4 ( 1, format, ##args)
#define	DLNA_DMR_INFO_PRINTF(format, args...)			OSA_DBG_PrintMw4 ( 2, format, ##args)
#define	DLNA_DMR_CB_PRINTF(format, args...)				OSA_DBG_PrintMw4 ( 3, format, ##args)
#define	DLNA_STACK_PRINTF(format, args...)				OSA_DBG_PrintMw4 ( 4, format, ##args)
/*--------------- WMDRM ----------------*/
#define	WMDRM_PRINTF(format, args...)     				OSA_DBG_PrintMw4 ( 5, format, ##args)
/*--------------- MMS ----------------*/
#define	MMS_PRINTF(format, args...)     				OSA_DBG_PrintMw4 ( 6, format, ##args)
/*--------------- SDP ----------------*/
#define	SDP_PRINTF(format, args...)     				OSA_DBG_PrintMw4 ( 7, format, ##args)
#define	SDP_DNLD_PRINTF(format, args...)     			OSA_DBG_PrintMw4 ( 8, format, ##args)
/*--------------- TTX 2 -----------------*/
#define TTX_DRV_PRINTF(format, args...)					OSA_DBG_PrintMw4 ( 9, format, ##args)
#define TTX_DSP_PRINTF(format, args...)					OSA_DBG_PrintMw4 (10, format, ##args)
#define TTX_DBG_PRINTF(format, args...)					OSA_DBG_PrintMw4 (11, format, ##args)
/*--------------- SUBT 2 ----------------*/
#define SUBT_MAIN_PRINTF(format, args...)				OSA_DBG_PrintMw4 (12, format, ##args)
#define SUBT_DRV_PRINTF(format, args...)				OSA_DBG_PrintMw4 (13, format, ##args)
#define SUBT_DSP_PRINTF(format, args...)				OSA_DBG_PrintMw4 (14, format, ##args)
#define SUBT_DBG_PRINTF(format, args...)				OSA_DBG_PrintMw4 (15, format, ##args)
/*--------------- WIDEVINE ----------------*/
#define	WIDEVINE_PRINTF(format, args...)     			OSA_DBG_PrintMw4 (16, format, ##args)

/*--------------- WIN7 LLTD ----------------*/
#define	DMR_LLTD_PRINTF(format, args...)     			OSA_DBG_PrintMw4 (17, format, ##args)
/*---------------- HDCP ----------------*/
#define	HDCP_PRINTF(format, args...)					OSA_DBG_PrintMw4 (18, format, ##args)

#ifdef INCLUDE_SMARTSHARE
#define	SMH_PRINT_MSG(format, args...)					OSA_DBG_PrintMw4 (19, format, ##args)
#endif


/*---------------- BML ----------------*/
#ifdef INCLUDE_BML
#define	BML_SYS_PRINT(format, args...)					OSA_DBG_PrintMw4 (20, format, ##args)
#define BML_DSM_PRINT(format, args...)					OSA_DBG_PrintMw4 (21, format, ##args)
#define BML_EM_PRINT(format, args...)					OSA_DBG_PrintMw4 (22, format, ##args)
#define BML_GR_PRINT(format, args...)					OSA_DBG_PrintMw4 (23, format, ##args)
#define BML_AV_PRINT(format, args...)					OSA_DBG_PrintMw4 (24, format, ##args)
#endif

#ifdef INCLUDE_VCHB
/*--------------- GRACENOTE -----------*/
#define GN_PRINT_NET(format, args...)						OSA_DBG_PrintMw4 (24, format, ##args) /* For network transaction and parser*/
#define GN_PRINT_CHANNEL(format, args...)				OSA_DBG_PrintMw4 (25, format, ##args) /* about channel db and map */
#define GN_PRINT_EPGDB(format, args...)					OSA_DBG_PrintMw4 (26, format, ##args) /* about EPGDB */
#define GN_PRINT_THUMNAIL(format, args...)				OSA_DBG_PrintMw4 (27, format, ##args) /* for thumnail */
#define GN_PRINT_COMMON(format, args...)					OSA_DBG_PrintMw4 (28, format, ##args) /* MSG sequence and so on*/
#define GN_PRINT_ERR(format, args...)						OSA_DBG_PrintMw4 (29, format, ##args)
#endif
/*---------------- NETIF ---------------*/
#define NETIF_PRINT_NET(format, args...)					OSA_DBG_PrintMw4 (29, format, ##args)

/*---------------- WiFi Display----------------*/

#ifdef INCLUDE_WIFI_DISPLAY
#define WFD_PRINTF(format, args...)						OSA_DBG_PrintDrv2(30, format, ##args)
#endif


/*----------SmartShare ----------------*/
#ifdef INCLUDE_SMARTSHARE
#define	SMH_PRINT_ERR(format, args...)					OSA_DBG_PrintMw4 (31, format, ##args)
#endif

/*---------------- Drv1 ---------------*/
#if 1
#define FE_PRINT(mode, format, args...) 				OSA_DBG_PrintDrv1 (mode, format, ##args)
#else
#define FE_PRINT_0(format, args...) 					OSA_DBG_PrintDrv1( 0, format, ##args)
#define FE_PRINT_1(format, args...) 					OSA_DBG_PrintDrv1( 1, format, ##args)
#define FE_PRINT_2(format, args...) 					OSA_DBG_PrintDrv1( 2, format, ##args)
#define TU_PRINT_3(format, args...) 					OSA_DBG_PrintDrv1( 3, format, ##args)
#endif

#if (PLATFORM_TYPE == LG_PLATFORM)
#define FE_PRINT_0(format, args...) 					OSA_DBG_PrintDrv1( 0, format, ##args)
#define FE_PRINT_1(format, args...) 					OSA_DBG_PrintDrv1( 1, format, ##args)
#define FE_PRINT_2(format, args...) 					OSA_DBG_PrintDrv1( 2, format, ##args)
#define TU_PRINT_3(format, args...) 					OSA_DBG_PrintDrv1( 3, format, ##args)
#endif

#define SDEC_PRINT_PESF(format, args...) 				OSA_DBG_PrintDrv1( 4, format, ##args)
#define SDEC_PRINT_SECF(format, args...) 				OSA_DBG_PrintDrv1( 5, format, ##args)
#define VDEC_PRINT(format, args...) 					OSA_DBG_PrintDrv1( 6, format, ##args)
#define VDEC_PRINT_USRD(format, args...) 				OSA_DBG_PrintDrv1( 7, format, ##args)
#define VDEC_PRINT_ISR(format, args...)					OSA_DBG_PrintDrv1( 8, format, ##args)
#define OSD_PRINT(format, args...)						OSA_DBG_PrintDrv1( 9, format, ##args)

/* debug print 정리까지 임시 사용 by seongcheoll.kim */
#if 1
#define	VIDEO_WINDOW_PRINT(format, args...)	 			OSA_DBG_PrintDrv1(10, format, ##args)
#define	VIDEO1_PRINT(format, args...)					OSA_DBG_PrintDrv1(11, format, ##args)
#define	VIDEO2_PRINT(format, args...)			 		OSA_DBG_PrintDrv1(12, format, ##args)
#define	VIDEO_PERI_PRINT(format, args...) 				OSA_DBG_PrintDrv1(13, format, ##args)
#define	VIDEO4_PRINT(format, args...)					OSA_DBG_PrintDrv1(14, format, ##args)
#define	VIDEO_DDI_PRINT(format, args...) 				OSA_DBG_PrintDrv1(15, format, ##args)
#endif

#define	VIDEO_FMTINFO_PRINT(format, args...)			OSA_DBG_PrintDrv1(10, format, ##args)
#define	VIDEO_WIN_PRINT(format, args...)	 			OSA_DBG_PrintDrv1(11, format, ##args)
#define	VIDEO_MUTE_PRINT(format, args...)			 		OSA_DBG_PrintDrv1(12, format, ##args)
#define	VIDEO_3D_PRINT(format, args...) 				OSA_DBG_PrintDrv1(13, format, ##args)
#define	VIDEO_VBI_PRINT(format, args...)					OSA_DBG_PrintDrv1(14, format, ##args)
#define	VIDEO_DDI_PRINT(format, args...) 				OSA_DBG_PrintDrv1(15, format, ##args)


#define SWITCH_PRINT(format, args...)	 				OSA_DBG_PrintDrv1(16, format, ##args)
#define HDMI_PRINT(format, args...) 					OSA_DBG_PrintDrv1(17, format, ##args)
#define	AUDIO_DDI_PRINTF(format, args...) 				OSA_DBG_PrintDrv1(18, format, ##args)
#define	AUDIO_DD_PRINTF(format, args...) 				OSA_DBG_PrintDrv1(19, format, ##args)
#define	AUDIO_ADEC_PRINTF(format, args...) 				OSA_DBG_PrintDrv1(20, format, ##args)
#define	AUDIO_TASK_PRINTF(format, args...) 				OSA_DBG_PrintDrv1(21, format, ##args)
#define	AUDIO_ADEC_DD_PRINTF(format, args...) 			OSA_DBG_PrintDrv1(22, format, ##args)
#define ADJ_PRINT(format, args...) 						OSA_DBG_PrintDrv1(23, format, ##args)
#define FRC_PRINT(format, args...) 						OSA_DBG_PrintDrv1(24, format, ##args)
#define MICOM_PRINT(format, args...)					OSA_DBG_PrintDrv1(25, format, ##args)
#define SVC_AUTO_PRINT(format, args...)					OSA_DBG_PrintDrv1(26, format, ##args)
#define	UCOM_PRINT(format, args...) 					OSA_DBG_PrintDrv1(27, format, ##args)
#define	PQL_PRINTF(format, args...) 					OSA_DBG_PrintDrv1(28, format, ##args)
#define	GPIO_PRINT(format, args...) 					OSA_DBG_PrintDrv1(29, format, ##args)
#define	NVM_PRINT(format, args...)  					OSA_DBG_PrintDrv1(30, format, ##args)
#define DIMMING_PRINT(format, args...) 				OSA_DBG_PrintDrv1(31, format, ##args)


/*---------------- Drv2 ---------------*/
#define PVR_PRINTF(format, args...)						OSA_DBG_PrintDrv2(0, format, ##args)
#define PVR_UP_PRINTF(format, args...)					OSA_DBG_PrintDrv2(1, format, ##args)
#define PVR_DN_PRINTF(format, args...)					OSA_DBG_PrintDrv2(2, format, ##args)
#define PVR_IDX_PRINTF(format, args...)					OSA_DBG_PrintDrv2(3, format, ##args)
#define EMP_DDI_PRINTF(format, args...)					OSA_DBG_PrintDrv2(4, format, ##args)
#define EMP_MP3_PRINTF(format, args...)					OSA_DBG_PrintDrv2(5, format, ##args)
#define EMP_JPG_PRINTF(format, args...)					OSA_DBG_PrintDrv2(6, format, ##args)
#define EMP_DIVX_PRINTF(format, args...)				OSA_DBG_PrintDrv2(7, format, ##args)
#define MOUSE_PRINT(format, args...)  					OSA_DBG_PrintDrv2(8, format, ##args)
#if defined(INCLUDE_WIRELESS_READY)
#define WCM_PRINTF(format,args...)						OSA_DBG_PrintDrv2(9, format, ##args)
#define WIRELESS_PRINTF(format, args...)				OSA_DBG_PrintDrv2(10, format, ##args)
#endif

#ifdef INCLUDE_DSVC
#define DSVC_API_PRINT(format, args...)                 OSA_DBG_PrintDrv2 (18, format, ##args)
#endif

#ifdef INCLUDE_MHP
/*----------------MHP ---------------*/
#define	MHP_MODULE_PRINT(format, args...)				OSA_DBG_PrintDrv2 (12, format, ##args)
#define MHP_SI_PRINT(format, args...) 					OSA_DBG_PrintDrv2 (13, format, ##args)
#define MHP_OSA_PRINT(format, args...) 					OSA_DBG_PrintDrv2 (14, format, ##args)
#define MHP_CM_PRINT(format, args...) 					OSA_DBG_PrintDrv2 (15, format, ##args)
#define	MHP_SYSTEM_PRINT(format, args...) 				OSA_DBG_PrintDrv2 (16, format, ##args)
#define MHP_AV_PRINT(format, args...) 					OSA_DBG_PrintDrv2 (17, format, ##args)
#define	MHP_API_PRINT(format, args...) 					OSA_DBG_PrintDrv2 (18, format, ##args)
#define MHP_DEVICE_PRINT(format, args...) 				OSA_DBG_PrintDrv2 (19, format, ##args)
#define MHP_OSD_PRINT(format, args...) 					OSA_DBG_PrintDrv2 (20, format, ##args)
#define MHP_BB_PRINT(format, args...) 					OSA_DBG_PrintDrv2 (21, format, ##args)
#define MHP_EME_PRINT(format, args...) 					OSA_DBG_PrintDrv2 (22, format, ##args)
#endif

#ifdef INCLUDE_HBBTV
/*----------------HbbTV ---------------*/
#define HBBTV_MODULE_PRINT(format, args...)             OSA_DBG_PrintDrv2 (28, format, ##args)
#define HBBTV_MW_PRINT(format, args...)                 OSA_DBG_PrintDrv2 (29, format, ##args)
#define HBBTV_AV_PRINT(format, args...)                 OSA_DBG_PrintDrv2 (30, format, ##args)
#define HBBTV_HE_PRINT(format, args...)                 OSA_DBG_PrintDrv2 (31, format, ##args)
#endif

#ifdef INCLUDE_CHB
#define CHB_L1_PRINTF(format, args...)					OSA_DBG_PrintDrv2(24, format, ##args)
#define CHB_L2_PRINTF(format, args...)					OSA_DBG_PrintDrv2(25, format, ##args)
#endif
#define EMP_FLASHES_L1_PRINTF(format, args...)					OSA_DBG_PrintDrv2(26, format, ##args)
#define EMP_FLASHES_L2_PRINTF(format, args...)					OSA_DBG_PrintDrv2(27, format, ##args)

/*[L8] 2010/12/07 minjun.kim  add
  *
  */
#if (PLATFORM_TYPE == LG_PLATFORM)
#define LMF_PRINT(format, args...)					OSA_DBG_PrintDrv2(26, format, ##args)
#endif

#else /* _EMUL_WIN */

extern void xlibc_critical(const char *format, ...);
extern void xlibc_warning(const char *format, ...);


#define OSA_DBG_PrintUi1
#define OSA_DBG_EnablePrintMaskUi1
#define OSA_DBG_DisablePrintMaskUi1

#define OSA_DBG_PrintUi2
#define OSA_DBG_EnablePrintMaskUi2
#define OSA_DBG_DisablePrintMaskUi2

#define OSA_DBG_PrintMw1
#define OSA_DBG_EnablePrintMaskMw1
#define OSA_DBG_DisablePrintMaskMw1

#define OSA_DBG_PrintMw2
#define OSA_DBG_EnablePrintMaskMw2
#define OSA_DBG_DisablePrintMaskMw2

#define OSA_DBG_PrintMw3
#define OSA_DBG_EnablePrintMaskMw3
#define OSA_DBG_DisablePrintMaskMw3

#define OSA_DBG_PrintMw4
#define OSA_DBG_EnablePrintMaskMw3
#define OSA_DBG_DisablePrintMaskMw3

#define OSA_DBG_PrintDrv1
#define OSA_DBG_EnablePrintMaskDrv1
#define OSA_DBG_DisablePrintMaskDrv1

#define OSA_DBG_PrintDrv2
#define OSA_DBG_EnablePrintMaskDrv2
#define OSA_DBG_DisablePrintMaskDrv2

/*************************************************************************************************************/
/*                                        Print 할당 및 Define                                               */
/*************************************************************************************************************/
/*------------ System Call ------------*/
#define OSA_PRINT
#define OSA_RPRINT
#define OSA_PRINT_ERROR
#define OSA_PRINT_WARNING
#define	API_ERR_PRINT

/*---------------- Ui1 ----------------*/
#define UI_MAIN_PrintEx
#define	UI_GLOBAL_PRINT
#define	UI_MENU_PRINT
#define	UI_HOME_PRINT
#define	UI_PCCTL_PRINT
#define	UI_ACAP_KEY_PRINT
#define UI_KEY_PRINT
#define	UI_GUIDE_PRINT
#define	UI_FULLINFO_PRINT
#define	UI_BRINFO_PRINT
#define	UI_PGBAR_PRINT
#define	UI_USB_PRINT
#define	UI_PHOTO_PRINT
#define	UI_MUSIC_PRINT
#define	UI_LIST_PRINT
#define	UI_RES_PRINT
#define	UI_RESREC_PRINT
#define	UI_RECLIST_PRINT
#define UI_SUMODE_PRINT
#define	UI_DVRMODE_PRINT
#define	UI_PATH_PRINT
#define	UI_INSTALL_PRINT
#define	UI_CI_PRINT
#define	UI_PQL_PRINT
#define	UI_SUBT_PRINT
#define	UI_AUTOAV_PRINT
#define	UI_DIRKEY_PRINT
#define	UI_OAD_PRINT
#define UI_ACAP_PRINT
#define UI_HOTEL_PRINT
#define UI_POLLING_PRINT
#define UI_HCEC_PRINT

/*---------------- Ui2 ----------------*/
#define MATRIX_PRINT
#define UI_VIDEO_PRINT
#define UI_BT_PRINT
#define UI_SYS_PRINT
#define UI_TOOLKIT_PRINT
#define UI_EPGSEG_PRINT
#define UI_DASY_PRINT
#define UI_PGBARTIME_PRINT
#define UI_ATUNING_PRINT
#define UI_MTUNING_PRINT
#define UI_MOVIE_PRINT
#define UI_ACMS_PRINT
#define UI_NET_PRINT
#define UI_NETFLIX_PRINT
#define UI_NSU_PRINT
#define UI_YOUTUBE_PRINT
#define UI_RATING_PRINT
#define UI_MHP_PRINT
#define UI_ANI_PRINT
#define UI_MULTICHANNEL_PRINT
#define	UI_MHP_KEY_PRINT
#define UI_IMAGE_PRINT(format, args, ...)

#ifdef INCLUDE_VCS
#define UI_VCS_PRINT
#endif //INCLUDE_VCS
#define UI_MOUSE_PRINT
#ifdef INCLUDE_UI_EFFECT
#define UI_EFFECT_PRINT
#define UI_EFFECT_TIME_PRINT
#endif


/*---------------- Mw1 ----------------*/
/*---------------- CM -----------------*/
#define CM_PRINT_0
#define CM_PRINT_1
#define CM_PRINT_2
#define CM_PRINT_3
#define CM_PRINT_4
#define CM_PRINT_5
#define CM_PRINT_6
#define CM_PRINT_7

/*--------------- SI ------------------*/
#define SI_PRINT_1
#define SI_PRINT_2
#define SI_PRINT_3
#define SI_PRINT_ERR

/*--------------- PSIP ----------------*/
#define DBG_Sem1
#define DBG_Sem2
#define DBG_Msg1
#define DBG_Msg2
#define DBG_Msg3
#define DBG_Msg4
#define ERR_Msg

/*--------------- DTVCC ----------------*/
#define DCC_PRINTF
#define DCC_WARN_PRINTF
#define DCC_ERR_PRINTF

/*--------------- SUBT ----------------*/
#define SUBT_PRINTF
#define SUBT_WARN_PRINTF
#define SUBT_ERR_PRINTF

/*--------------- EMF -----------------*/
#define	EME_INFO_PRINTF
#define	EME_JPEG_PRINTF
#define	EME_SUBT_PRINTF
#define	EME_POLL_PRINTF

/*--------------- MLM -----------------*/
#define	MLM_INFO_PRINTF
#define	MLM_WARNING_PRINTF
#define	MLM_ERROR_PRINTF

/*--------------- ATVCC ----------------*/
#define	ACC_PRINTF
#define	ACC_ERR_PRINTF

/*--------------- TTX -----------------*/
#define	TTX_PRINTF
#define	TTX_ERR_PRINTF

/*--------------- DASY ----------------*/
#define	DASY_PRINTF_1
#define	DASY_PRINTF_2
#define	DASY_PRINTF_3

/*---------------- MFS ----------------*/
#define	MFS_PRINT_ERR
#define	MFS_PRINT_MSG1
#define	MFS_PRINT_MSG2

/*---------------- Mw2 ----------------*/
/*---------------- MRE ----------------*/
#define	MRE_INFO_PRINTF
#define	MRE_WARNING_PRINTF
#define	MRE_ERROR_PRINTF
#define	MRE_FATAL_PRINTF
#define	MRE_PATH_PRINTF

/*--------------- NSU ----------------*/
#define NSU_PRINT

/*--------------- OAD -----------------*/
#define	OAD_PRINTF

/*--------------- UTIL ----------------*/
#define	UTIL_PRINT

/*--------------- SWU -----------------*/
#define	SWU_API_PRINTF
#define	SWU_EPK_PRINTF
#define	SWU_MTD_PRINTF
#define	SWU_UTIL_PRINTF

/*-------------- AVBLOCK --------------*/
#define AV_BLOCK_PRINTF

/*--------------- SYS -----------------*/
#define	AUDIO_API_PRINTF
#define	VIDEO_API_PRINT
#define	VIDEO_FMT_PRINT

/*--------------- OTA -----------------*/
#define	OTA_PRINTF1
#define	OTA_PRINTF2

/*---------------- CI -----------------*/
#define CI_DBG_PRINT_MAIN
#define CI_DBG_PRINT_PHY
#define CI_DBG_PRINT_CIS
#define CI_DBG_PRINT_LINK
#define CI_DBG_PRINT_TRANS
#define CI_DBG_PRINT_SESSION
#define CI_DBG_PRINT_APP
#define CI_DBG_PRINT_RESMGR
#define CI_DBG_PRINT_APPINFO
#define CI_DBG_PRINT_CAS
#define CI_DBG_PRINT_DATETIME
#define CI_DBG_PRINT_HOSTCTL
#define CI_DBG_PRINT_MMI
#define CI_DBG_PRINT_ERR

#ifdef INCLUDE_CIPLUS
#define CI_DBG_PRINT_APPMMI
#define CI_DBG_PRINT_LANGCOUNTRY
#define CI_DBG_PRINT_UPGRADE
#define CI_DBG_PRINT_CC
#endif
/*--------------- CECP ----------------*/
#define CECP_PRINTF

/*---------------- Mw3 ----------------*/
#ifdef INCLUDE_ACAP
/*--------------- ACAP ----------------*/
#define	ACAP_MODULE_PRINT
#define ACAP_SI_PRINT
#define ACAP_OSA_PRINT
#define ACAP_CM_PRINT
#define	ACAP_SYSTEM_PRINT
#define ACAP_AV_PRINT
#define	ACAP_API_PRINT
#define ACAP_DEVICE_PRINT
#define ACAP_OSD_PRINT

#else
/*--------------- MHEG ----------------*/
#define	MHEG_MODULE_PRINT
#define	MHEG_CMSI_PRINT
#define	MHEG_API_PRINT
#define	MHEG_CMSI_PRINT
#define	MHEG_ESM_PRINT
#define	MHEG_AV_PRINT
#define	MHEG_DEV_PRINT
#define	MHEG_NET_PRINT
#define	MHEG_OSD_PRINT
#endif

/*-------------- EDBMS ----------------*/
#define EDBMS_PRINTF0
#define EDBMS_PRINTF1

/*--------------- SDM -----------------*/
#define	USB_PRINT_MSG

/*-------------- ACMS ---------------*/
#define ACMS_DBG_PRINTF
#define ACMS_VPS_PRINTF
#define ACMS_P830F1_PRINTF
#define ACMS_P830F2_PRINTF
#define ACMS_VBI_PRINTF

/*-------------- WIDGET DIARY ---------------*/
#define	DIARY_PRINTF

/*--------------- Bluetooth ----------------*/
#define	BT_INTERNAL_PRINT
#define	BT_OSADAP_PRINT
#define	BT_API_PRINT
#define	BT_ENCODE_PRINT
#define	BT_DECODE_PRINT
#define	BT_ARBITER_PRINT

/*---------------- CIFS ----------------*/
#define	CIFS_PRINT

/*--------------- YWE ----------------*/
#define YWE_PRINT

/*--------------- STREAM ----------------*/
#define STREAM_PRINT
#define STREAM_TRACE

/*--------------- HTTP client -----------*/
#define HTTP_DEBUG_PRINT

/*--------------- ADCC for BR----------------*/
#define	ADCC_INFO_PRINTF
#define	ADCC_ERROR_PRINTF

/*--------------- OAE ----------------*/
#define	OAE_PRINTF
#define	OAE_ERR_PRINTF

/*---------------- Mw4 ----------------*/
#define	DLNA_DMP_INFO_PRINTF
#define	DLNA_DMP_ERR_PRINTF
#define	DLNA_DMR_INFO_PRINTF
#define	DLNA_DMR_ERR_PRINTF
#define	DLNA_STACK_PRINTF
/*--------------- WMDRM ----------------*/
#define	WMDRM_PRINTF
/*--------------- MMS ----------------*/
#define	MMS_PRINTF
/*--------------- SDP ----------------*/
#define	SDP_PRINTF
#define	SDP_DNLD_PRINTF
/*--------------- TTX 2 ----------------*/
#define TTX_DRV_PRINTF
#define TTX_DSP_PRINTF
#define TTX_DBG_PRINTF
/*--------------- SUBT 2 ----------------*/
#define SUBT_MAIN_PRINTF
#define SUBT_DRV_PRINTF
#define SUBT_DSP_PRINTF
#define SUBT_DBG_PRINTF
/*--------------- WIDEVINE ----------------*/
#define	WIDEVINE_PRINTF
/*--------------- WIN7 LLTD----------------*/
#define DMR_LLTD_PRINTF
/*---------------- HDCP -------------------*/
#define HDCP_PRINTF

/*---------------- Drv1 ---------------*/
#if 1
#define FE_PRINT
#define TU_PRINT printf
#else
#define FE_PRINT_0
#define FE_PRINT_1
#define FE_PRINT_2
#define TU_PRINT_3
#endif
#define SDEC_PRINT_PESF
#define SDEC_PRINT_SECF
#define VDEC_PRINT
#define VDEC_PRINT_USRD
#define VDEC_PRINT_ISR
#define OSD_PRINT
#define	VIDEO_WIN_PRINT
#define	VIDEO1_PRINT
#define	VIDEO2_PRINT
#define	VIDEO_PERI_PRINT
#define	VIDEO4_PRINT
#define VIDEO_API_PRINT
#define SWITCH_PRINT
#define HDMI_PRINT
#define	AUDIO_DDI_PRINTF
#define	AUDIO_DD_PRINTF
#define	AUDIO_ADEC_PRINTF
#define	AUDIO_TASK_PRINTF
#define	AUDIO_ADEC_DD_PRINTF
#define ADJ_PRINT
#define FRC_PRINT
#define DIMMING_PRINT
#define MICOM_PRINT
#define SVC_AUTO_PRINT
#define	UCOM_PRINT
#define	PQL_PRINTF
#define	GPIO_PRINT
#define	NVM_PRINT

/*---------------- Drv2 ---------------*/
#define PVR_PRINTF
#define PVR_UP_PRINTF
#define PVR_DN_PRINTF
#define PVR_IDX_PRINTF
#define EMP_DDI_PRINTF
#define EMP_MP3_PRINTF
#define EMP_JPG_PRINTF
#define EMP_DIVX_PRINTF

#ifdef INCLUDE_MHP
/*---------------- MHP ---------------*/
#define	MHP_MODULE_PRINT
#define MHP_SI_PRINT
#define MHP_OSA_PRINT
#define MHP_CM_PRINT
#define	MHP_SYSTEM_PRINT
#define MHP_AV_PRINT
#define	MHP_API_PRINT
#define MHP_DEVICE_PRINT
#define MHP_OSD_PRINT
#define MHP_BB_PRINT
#define MHP_EME_PRINT
#endif

/*[L8] 2010/12/07 minjun.kim  add
  *
  */
#if (PLATFORM_TYPE == LG_PLATFORM)
#define LMF_PRINT
#endif

#endif /* _EMUL_WIN */


#ifdef _PRINT_EXPAND_C_
static cmd_mask_group_attr_t	_gUi1Attr =
{
	"UI1",PRNT_MOD_UI1,
	{
		/*pName | num | nm_flag | color*/
		{ "UI-DBG",        0, 1, 0 }, { "UI GLOBAL",     1, 1, 0 },
		{ "UI MENU",       2, 1, 0 }, { "UI HOME MENU",  3, 1, 0 },
		{ "UI PCCTRL",     4, 1, 0 }, { "UI ACAPKEY",    5, 1, 0 },
		{ "UI KEY",        6, 1, 0 }, { "UI GUIDE",      7, 1, 0 },
		{ "UI FULLINFO",   8, 1, 0 }, { "UI BRINFO",     9, 1, 0 },
		{ "UI PGBAR",     10, 1, 0 }, { "UI USB",       11, 1, 0 },
		{ "UI PHOTOLIST", 12, 1, 0 }, { "UI MUSICLIST", 13, 1, 0 },
		{ "UI LISTMGR",   14, 1, 0 }, { "UI RESLIST",   15, 1, 0 },
		{ "UI RESREC",    16, 1, 0 }, { "UI RECLIST",   17, 1, 0 },
		{ "UI SUMODE",    18, 1, 0 }, { "UI DVRMODE",   19, 1, 0 },
		{ "UI PATH",      20, 1, 0 }, { "UI INSTALL",   21, 1, 0 },
		{ "UI CI",        22, 1, 0 }, { "UI ED",        23, 1, 0 },
		{ "UI SUBT/CC",      24, 1, 0 }, { "UI AUTOAV",    25, 1, 0 },
		{ "UI KEYWORD",   26, 1, 0 }, { "UI OAD",       27, 1, 0 },
		{ "UI ACAP",      28, 1, 0 }, { "UI HOTEL",     29, 1, 0 },
		{ "UI POLLING",   30, 1, 0 }, { "UI HCEC",      31, 1, 0 }
	}
};
static cmd_mask_group_attr_t	_gUi2Attr =
{
	"UI2",PRNT_MOD_UI2,
	{
		{ "UI MATRUX",     0, 1, 0 }, { "UI VIDEO",      1, 1, 0 },
		{ "UI_BT_PRINT",   2, 1, 0 }, { "UI SYS",        3, 1, 0 },
		{ "UI TOOLKIT",    4, 1, 0 }, { "UI EPGSEG",     5, 0, 0 },
		{ "UI DASY",       6, 0, 0 }, { "UI PGBARTIME",  7, 0, 0 },
		{ "UI AUTOTUNING",8, 1, 0  }, { "UI MANUALTUNING",9, 1, 0},
		{ "UI MOVIELIST", 10, 1, 0 }, { "UI ACMS",      11, 0, 0 },
		{ "UI NETWORK",   12, 0, 0 }, { "UI NETFLIX",   13, 0, 0 },
		{ "UI NSU",       14, 0, 0 }, { "UI YOUTUBE",   15, 1, 0 },
		{ "UI RATING",    16, 0, 0 }, { "UI MHP",       17, 1, 0 },
		{ "UI ANI",18, 1, 0 }, { "UI MULTICHANNEL",	19, 1, 0 },
		{ "UI MHPKEY",   20, 1, 0 }, { "UI IMAGE",      21, 1, 0 },
		{ "UI BMLKEY",    22, 1, 0 }, { "UI BML",        23, 1, 0 },
		{ NULL,           24, 0, 0 }, { NULL,           25, 0, 0 },
		{ NULL,           26, 0, 0 }, { NULL,           27, 0, 0 },
		{ NULL,           28, 0, 0 }, { "UI MOUSE",     29, 1, 0 },
	#ifdef INCLUDE_UI_EFFECT
		{ "UI EFFECT",    30, 0, 0 }, { "UI EFFECT TIME", 31, 0, 0 }
	#else
		{ NULL,    30, 0, 0 }, { NULL, 31, 0, 0 }
	#endif
	}
};
static cmd_mask_group_attr_t	_gMw1Attr =
{
	"MW1",PRNT_MOD_MW1,
	{
	{ "CM-0",          0, 1, 0 }, { "CM-1",          1, 1, 0 },
	{ "CM-2",          2, 1, 0 }, { "CM-3",          3, 1, 0 },
	#if (SYS_DVB)
	{ "SI",            4, 1, 0 }, { "SI_0",          5, 1, 0 },
	{ "SI_1",          6, 1, 0 }, { "SI_2",          7, 1, 0 },
	{ "SI_3",          8, 1, 0 }, { "SI_ERR",        9, 1, 0 },
	#elif (SYS_ATSC)
	{ "PSIP",          4, 1, 0 }, { "PSIP_0",        5, 1, 0 },
	{ "PSIP_1",        6, 1, 0 }, { "PSIP_2",        7, 1, 0 },
	{ "PSIP_3",        8, 1, 0 }, { "PSIP_ERR",      9, 1, 0 },
	#endif
	{ "SUBT/DCC",         10, 1, 0 }, { "SUBT/DCC WARN",    11, 1, 0 },
	{ "SUBT/DCC ERR",     12, 1, 0 }, { "EME INFO",     13, 1, 0 },
	{ "EME JPEG",     14, 1, 0 }, { "EME SUBT",     15, 1, 0 },
	{ "EME POLL",     16, 1, 0 }, { "MLM INFO",     17, 1, 0 },
	{ "MLM WARN",     18, 1, 0 }, { "MLM ERR",      19, 1, 0 },
	{ "TTX/ACC",      	  20, 1, 0 }, { "TTX/ACC ERR",      21, 1, 0 },
	{ "DASY_GEN",     22, 1, 0 }, { "DASY_UP",      23, 1, 0 },
	{ "DASY_DN",      24, 1, 0 }, { "MFS-0",        25, 1, 0 },
	{ "MFS-1",        26, 1, 0 }, { "MFS-2",        27, 1, 0 },
	{ "CM-4",         28, 1, 0 }, { "CM-5",         29, 1, 0 },
	{ "CM-6",         30, 1, 0 }, { "CM-7",         31, 1, 0 }
	}
};
static cmd_mask_group_attr_t	_gMw2Attr =
{
	"MW2",PRNT_MOD_MW2,
	{
	{ "MRE INFO",      0, 1, 0 }, { "MRE WARN",      1, 1, 0 },
	{ "MRE ERR",       2, 1, 0 }, { "MRE FATAL",     3, 1, 0 },
	{ "MRE PATH",      4, 1, 0 }, { "NSU",           5, 1, 0 },
	{ "API_UTIL",      6, 1, 0 }, { "SWU API",       7, 1, 0 },
    { "SWU EPK",       8, 1, 0 }, { "SWU MTD",       9, 1, 0 },
	{ "SWU UTIL",     10, 1, 0 }, { "AVBLOCK",      11, 1, 0 },
	{ "AUD API",      12, 1, 0 }, { "VIDEO API",    13, 1, 0 },
	{ "VFMT",         14, 1, 0 }, { "OTA0",         15, 1, 0 },
	{ "OTA1",         16, 1, 0 }, { "CI_MAIN",      17, 1, 0 },
#ifdef INCLUDE_CIPLUS
	{ "CI_LINK",      18, 1, 0 }, { "CI_TRANS",     19, 1, 0 },
	{ "CI_SESSION",   20, 1, 0 }, { "CI_APP_RESM",	21, 1, 0 },
	{ "CI_APPIN_HST", 22, 1, 0 }, { "CI_CAS",   	23, 1, 0 },
	{ "CI_DATETIME",  24, 1, 0 }, { "CI_MMI",  		25, 1, 0 },
	{ "CI_APPMMI",    26, 1, 0 }, { "CI_LANGCONT", 	27, 1, 0 },
	{ "CI_UPGRADE",   28, 1, 0 }, { "CI_CC", 		29, 1, 0 },
#else
	{ "CI_PHY",       18, 1, 0 }, { "CI_CIS",       19, 1, 0 },
	{ "CI_LINK",      20, 1, 0 }, { "CI_TRANS",     21, 1, 0 },
	{ "CI_SESSION",   22, 1, 0 }, { "CI_APP",       23, 1, 0 },
	{ "CI_RESMGR",    24, 1, 0 }, { "CI_APPINFO",   25, 1, 0 },
	{ "CI_CAS",       26, 1, 0 }, { "CI_DATETIME",  27, 1, 0 },
	{ "CI_HOSTCTL",   28, 1, 0 }, { "CI_MMI",       29, 1, 0 },
#endif
	{ "CI_ERR",       30, 1, 0 }, { "CECP",         31, 1, 0 }
	}
};
static cmd_mask_group_attr_t	_gMw3Attr =
{
	"MW3",PRNT_MOD_MW3,
{
#ifdef INCLUDE_ACAP
	{ "ACAP DBG",      0, 1, 0 }, { "ACAP SI",       1, 1, 0 },
	{ "ACAP OSA",      2, 1, 0 }, { "ACAP CM",       3, 1, 0 },
	{ "ACAP SYSTEM",   4, 1, 0 }, { "ACAP AV",       5, 1, 0 },
	{ "ACAP API",      6, 1, 0 }, { "ACAP DEVICE",   7, 1, 0 },
	{ "ACAP OSD",      8, 1, 0 },
#else
	{ "MHEG-DBG",      0, 1, 0 }, { "MHEG DSM",       1, 1, 0 },
	{ "MHEG API",      2, 1, 0 }, { "MHEG CMSI",       3, 1, 0 },
	{ "MHEG ESM",      4, 1, 0 }, { "MHEG AV",       5, 1, 0 },
	{ "MHEG DEV",      6, 1, 0 }, { "MHEG NET",      7, 1, 0 },
	{ "MHEG OSD",      8, 1, 0 },
#endif
	{ "EDB INFO",      9, 1, 0 },
#ifdef INCLUDE_MCB
	{ "MCB DBG",       10, 0, 0 },
#else
	{ NULL, 		  10, 0, 0 },
#endif
	{ "USB_MSG",      11, 1, 0 },
	{ "HTTPClient",   12, 1, 0 }, { "STREAM",       13, 1, 0 },
	{ "ACMS DBG",     14, 0, 0 }, { "ACMS VPS",     15, 0, 0 },
	{ "ACMS P830F1",  16, 0, 0 }, { "ACMS P830F2",  17, 0, 0 },
	{ "ACMS VBI",     18, 0, 0 }, { "STREAM TRACE", 19, 1, 0 },
	{ "BT INT",       20, 1, 0 }, { "BT OSA",       21, 1, 0 },
	{ "BT API",       22, 1, 0 }, { "BT ENC",       23, 1, 0 },
	{ "BT DEC",       24, 1, 0 }, { "BT ARBIT",     25, 1, 0 },
	{ "CIFS",         26, 1, 0 }, { "YWE",          27, 0, 0 },
	{ "ADCC_INFO",    28, 0, 0 }, { "ADCC_ERR",     29, 0, 0 },
	{ "OAE",          30, 1, 0 }, { "OAE_ERR",      31, 1, 0 }
	}
};

static cmd_mask_group_attr_t	_gMw4Attr =
{
	"MW4",PRNT_MOD_MW4,
	{
		{ "DLNA_DMP_INFO", 0, 1, 0 }, { "DLNA_DMP_ERR",  1, 1, 0 },
		{ "DLNA_DMR_INFO", 2, 1, 0 }, { "DLNA_DMR_ERR",  3, 1, 0 },
		{ "DLNA_STACK",    4, 1, 0 }, { "WMDRM",         5, 1, 0 },
		{ "MMS",           6, 1, 0 }, { "SDP",           7, 0, 0 },
		{ "SDP_DNLD", 	   8, 0, 0 }, { "TTX_DRV",    	 9, 0, 0 },
		{ "TTX_DSP",      10, 0, 0 }, { "TTX_DBG",     	11, 0, 0 },
		{ "SUBT_MAIN",    12, 0, 0 }, { "SUBT_DRV",     13, 0, 0 },
		{ "SUBT_DSP",     14, 0, 0 }, { "SUBT_DBG",     15, 0, 0 },
		{ "WIDEVINE",     16, 1, 0 }, { "WIN7_LLTD",    17, 0, 0 },
		{ "HDCP",         18, 1, 0 }, { "SMH_MSG",      19, 0, 0 },
#ifdef INCLUDE_BML
		{ "BML SYSTEM",   20, 0, 0 }, { "BML DC",       21, 0, 0 },
		{ "BML EVENT",    22, 0, 0 }, { "BML GRAPHIC",  23, 0, 0 },
#endif

#ifdef INCLUDE_VCHB
		{ "GN_NETWORK",	  24, 0, 0 }, { "GN_CANNEL",    25, 0, 0 },
#else
		{ "BML AV", 	  24, 0, 0 }, { NULL,			25, 0, 0 },
#endif

#ifdef INCLUDE_VCHB
		{ "GN_EPGDB",     26, 0, 0 }, { "GN_THUMNAIL",  27, 0, 0 },
		{ "GN_CMN",       28, 0, 0 }, { "GN_ERR",       29, 0, 0 },
#else
		{ NULL,			  24, 0, 0 }, { NULL,           25, 0, 0 },
		{ NULL,           26, 0, 0 }, { NULL,           27, 0, 0 },
		{ NULL,           28, 0, 0 }, { "NETIF",           29, 0, 0 },
#endif
		{ "WFD",           30, 0, 0 }, { "SMH_ERR",     31, 0, 0 },
	}
};
//INCLUDE_SMARTSHARE
static cmd_mask_group_attr_t	_gDd1Attr =
{
	"DD1",PRNT_MOD_DD1,
	{
	{ "FE TUNER",      0, 0, 0 }, { "FE TUNER",      1, 0, 0 },
	{ "FE TUNER",      2, 0, 0 }, { "FE TUNER",      3, 0, 0 },
	{ "SDEC PES",      4, 1, 0 }, { "SDEC SEC",      5, 1, 0 },
	{ "VDEC",          6, 1, 0 }, { "VDEC USRD",     7, 1, 0 },
	{ "VDEC ISR",      8, 1, 0 }, { "OSD",           9, 1, 0 },
	{ "VIDEO FMT",   10, 1, 0 }, { "VIDEO WIN",  11, 1, 0 },
	{ "VIDEO MUTE",   12, 1, 0 }, { "VIDEO 3D",    13, 1, 0 },
	{ "VIDEO VBI",    14, 1, 0 }, { "VIDEO DDI",    15, 1, 0 },
	{ "AVSWITCH",     16, 1, 0 }, { "HDMI",         17, 1, 0 },
	{ "AUDIO DDI",    18, 1, 0 }, { "AUDIO AMP",    19, 1, 0 },
	{ "ADEC DDI",     20, 1, 0 }, { "AUDIO TASK",   21, 1, 0 },
	{ "ADEC DD",      22, 1, 0 }, { "ADC",          23, 1, 0 },
	{ "FRC",          24, 1, 0 }, { "MICOM",        25, 1, 0 },
	{ "AUTO TEST",    26, 1, 0 }, { "MICOM ucom",   27, 1, 0 },
	{ "PQL",          28, 1, 0 }, { "GPIO",         29, 1, 0 },
	{ "NVM",          30, 1, 0 }, { "DIMMING",      31, 1, 0 }
	}
};
static cmd_mask_group_attr_t	_gDd2Attr =
{
	"DD2",PRNT_MOD_DD2,
	{
	{ "PVR",           0, 1, 0 }, { "PVR UP",        1, 1, 0 },
	{ "PVR DN",        2, 1, 0 }, { "PVR IDX",       3, 1, 0 },
	{ "EMP_DDI",	   4, 1, 0 }, { "EMP_MP3",       5, 1, 0 },
	{ "EMP_JPG",       6, 1, 0 }, { "EMP_DIVX",      7, 1, 0 },
	{ "MOUSE SP",      8, 1, 0 },
#ifdef INCLUDE_WIRELESS_READY
	{ "WCM",           9, 0, 0 }, { "WIRELESS",     10, 0, 0 },
#else
	{ NULL,		   9, 0, 0 }, { NULL, 	10, 0, 0 },
#endif
#ifdef INCLUDE_CIPLUS
	{ "MH CIPLUS",    11, 1, 0 },
#else
	{ NULL,           11, 0, 0 },
#endif
#ifdef INCLUDE_MHP
	{ "MHP DBG",      12, 1, 0 }, { "MHP SI",       13, 1, 0 },
	{ "MHP OSA",      14, 1, 0 }, { "MHP CM",       15, 1, 0 },
	{ "MHP SYSTEM",   16, 1, 0 }, { "MHP AV",       17, 1, 0 },
	{ "MHP API",      18, 1, 0 }, { "MHP DEVICE",   19, 1, 0 },
	{ "MHP OSD",      20, 1, 0 }, { "MHP BB", 		21, 1, 0 },
	{ "MHP EME",      22, 1, 0 }, { NULL, 			23, 1, 0 },
#else
	{ NULL,           12, 0, 0 }, { NULL,           13, 0, 0 },
	{ NULL,           14, 0, 0 }, { NULL,           15, 0, 0 },
	{ NULL,           16, 0, 0 }, { NULL,           17, 0, 0 },
	{ NULL,           18, 0, 0 }, { NULL,           19, 0, 0 },
	{ NULL,           20, 0, 0 }, { NULL, 	    21, 0, 0 },
	{ NULL,		 22, 0, 0 }, { NULL, 	    23, 0, 0 },
#endif
#ifdef INCLUDE_CHB
	{ "CHB L1",       24, 0, 0 }, { "CHB_L2",       25, 0, 0 },
#else
	{ NULL,           24, 0, 0 }, { NULL,           25, 0, 0 },
#endif
/*[L8] 2010/12/07 minjun.kim  add
  *
  */
#if (PLATFORM_TYPE == LG_PLATFORM)
    { "LMF", 		  26, 0, 0 }, { NULL,			27, 0, 0 },
#else
	{ "FLASHES L1",   26, 1, 0 }, { "FLASHES L2",   27, 1, 0 },
#endif
#ifdef INCLUDE_HBBTV
	{ "HBBTV DBG",    28, 1, 0 }, { "HBBTV MW",     29, 1, 0 },
	{ "HBBTV AV",     30, 1, 0 }, { "HBBTV HE",     31, 0, 0 },
#else
	{ NULL,           28, 0, 0 }, { NULL,           29, 0, 0 },
	{ NULL,           30, 0, 0 }, { NULL,           31, 0, 0 }
#endif
	}
};
static cmd_mask_group_attr_t	_gDbgAttr =
{
	"DBG",PRINT_MOD_DBG,
	{
		{"API_ERR",        0, 1, 0 }, { NULL,            1, 0, 0 },
		{ NULL,            2, 0, 0 }, { NULL,            3, 0, 0 },
		{ NULL,            4, 0, 0 }, { NULL,            5, 0, 0 },
		{ NULL,            6, 0, 0 }, { NULL,            7, 0, 0 },
		{ NULL,            8, 0, 0 }, { NULL,            9, 0, 0 },
		{ NULL,           10, 0, 0 }, { NULL,           11, 0, 0 },
		{ NULL,           12, 0, 0 }, { NULL,           13, 0, 0 },
		{ NULL,           14, 0, 0 }, { NULL,           15, 0, 0 },
		{ NULL,           16, 0, 0 }, { NULL,           17, 0, 0 },
		{ NULL,           18, 0, 0 }, { NULL,           19, 0, 0 },
		{ NULL,           20, 0, 0 }, { NULL,           21, 0, 0 },
		{ NULL,           22, 0, 0 }, { NULL,           23, 0, 0 },
		{ NULL,           24, 0, 0 }, { NULL,           25, 0, 0 },
		{ NULL,           26, 0, 0 }, { NULL,           27, 0, 0 },
		{ NULL,           28, 0, 0 }, { NULL,           29, 0, 0 },
		{ NULL,           30, 0, 0 }, { NULL,           31, 0, 0 }
	}
};

#endif

#ifdef __cplusplus
}
#endif
#endif /* _OSA_PRINTEX_H_ */
