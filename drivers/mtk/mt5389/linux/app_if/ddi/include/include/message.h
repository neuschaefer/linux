/******************************************************************************

	LGE. DTV RESEARCH LABORATORY
	COPYRIGHT(c) LGE CO.,LTD. 2000. SEOUL, KOREA.
	All rights are reserved.
	No part of this work covered by the copyright hereon may be
	reproduced, stored in a retrieval system, in any form or
	by any means, electronic, mechanical, photocopying, recording
	or otherwise, without the prior permission of LG Electronics.

	FILE NAME	: message.h
	VERSION 	:
	AUTHOR		: Moon Kang yun / Changseob Kim / Soyoung Kim
	DATE		: 2000. 1. 5 / 2004. 8.28 / 2006.05.17
	DESCRIPTION : Message 정의 위한 헤더 파일.
	             - 이 화일에는 TV Common한 MSG 만을 정의 한다.(ex: CM, PSIP, FE ...)
	             - DVR, EMF 등 advanced 한 feature를 위한 것은 Base만 정의 하며
	               각 모듈 별로 알아서 Base 기준으로 해당 모듈에 MSG를 정의 함.
	             - Base 만 정의 됨으로 #ifdef같은 define 필요 없음.
	             - 기본적으로 MSG는 Sender 입장에서 정의 한다.

*******************************************************************************/

#ifndef _MESSAGE_H_
#define _MESSAGE_H_
#include "branches.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
       Message Indentify
*/
#define MSG_SENDER_UI								0x0000
#define MSG_SENDER_CM								0x0001
#define MSG_SENDER_SI								0x0002 //#if (SYS_DVB)
#define MSG_SENDER_PSIP								0x0003 //#elif (SYS_ATSC) //#endif
#define MSG_SENDER_CI								0x0004
#define MSG_SENDER_MICOM							0x0005
#define MSG_SENDER_SYS								0x0006
#define MSG_SENDER_DRV								0x0007
#define MSG_SENDER_MFS								0x0008
#define MSG_SENDER_DASY								0x0009
#define MSG_SENDER_MLM								0x000A
#define MSG_SENDER_CECP								0x000B
#define MSG_SENDER_SDM								0x000C
#define MSG_SENDER_SWU								0x000D
#define MSG_SENDER_OC								0x000F
#define MSG_SENDER_MHEG								0x0010
#define MSG_SENDER_SSU								0x0011
#define MSG_SENDER_BT								0x0012 // INCLUDE_BT
#define MSG_SENDER_ACAP								0x0013
#define MSG_SENDER_TLL								0x0014 // INCLUDE_HOTEL_MODE
#define MSG_SENDER_EME								0x0015 // kjoh LGE; 20080814
#define MSG_SENDER_TTX								0x0016 // neospace LGE 081008
#define MSG_SENDER_CIFS								0x0017 // INCLUDE_CIFS
#define MSG_SENDER_IPM								0x0018 // INCLUDE_IPM
#define MSG_SENDER_NSU								0x0019 // INCLUDE_NSU
#define MSG_SENDER_NETFLIX							0x0020 // INCLUDE_NETFLIX_BUILTIN
#define MSG_SENDER_DLNA								0x0022 // INCLUDE_DLNA
#define MSG_SENDER_WCM								0x0023 // INCLUDE_WIRELESS_READY
#define MSG_SENDER_ADDONHOST						0x0024 // INCLUDE_ADDON
#define MSG_SENDER_NM								0x0025 // INCLUDE_NM
#define MSG_SENDER_MHP								0x0026 // INCLUDE_MHP
#define MSG_SENDER_VUDU								0x0027 // INCLUDE_VUDU_NATIVE
#define MSG_SENDER_MCB								0x0028 // INCLUDE_MCB
#define MSG_SENDER_GAME								0x0029 // INCLUDE_MOUSE
#define MSG_SENDER_SDP								0x002A // INCLUDE_SDP
#define MSG_SENDER_WIFIRC							0x002B // INCLUDE_HDSP; msjin; 20100719
#define MSG_SENDER_PQ								0x002C
#define MSG_SENDER_MDNS								0x002D // INCLUDE_MDNS
#define MSG_SENDER_RS								0x002E // INCLUDE_REMOTESVC
#define MSG_SENDER_BML								0x002F // INCLUDE_BML
#define MSG_SENDER_BCAS								0x0030 // INCLUDE_BCAS
#define MSG_SENDER_HBBTV                            0x0031 // INCLUDE_HBBTV
#define MSG_SENDER_WFD								0x0032 // INCLUDE_WIFI_DISPLAY
#define MSG_SENDER_SMH								0x0033 // INCLUDE_SMARTSHARE


/**
       Message Base Define
*/
/* General TV */
#define MSG_BASE_UI( X )							((MSG_SENDER_UI  	<<16) + X)
#define MSG_BASE_CM( X )							((MSG_SENDER_CM  	<<16) + X)
#define MSG_BASE_SI( X )							((MSG_SENDER_SI  	<<16) + X) //#if (SYS_DVB)
#define MSG_BASE_PSIP( X )							((MSG_SENDER_PSIP	<<16) + X) //#elif (SYS_ATSC) //#endif
#define MSG_BASE_CI( X )							((MSG_SENDER_CI   	<<16) + X)
#define MSG_BASE_ACAP( X )							((MSG_SENDER_ACAP 	<<16) + X)
#define MSG_BASE_MICOM( X )							((MSG_SENDER_MICOM	<<16) + X)
#define MSG_BASE_SYS( X )							((MSG_SENDER_SYS  	<<16) + X)
#define MSG_BASE_DRV( X )							((MSG_SENDER_DRV  	<<16) + X)
/* For DVR, EMF etc. */
#define MSG_BASE_MFS( X )							((MSG_SENDER_MFS  	<<16) + X)
#define MSG_BASE_DASY( X )							((MSG_SENDER_DASY 	<<16) + X)
#define MSG_BASE_EME( X )							((MSG_SENDER_EME  	<<16) + X)
#define MSG_BASE_MLM( X )							((MSG_SENDER_MLM  	<<16) + X)
#define MSG_BASE_CECP( X )							((MSG_SENDER_CECP 	<<16) + X)
#define MSG_BASE_SDM( X )							((MSG_SENDER_SDM  	<<16) + X)
#define MSG_BASE_SWU( X )							((MSG_SENDER_SWU 	<<16) + X)
/* For Object carousel */
#define MSG_BASE_OC( X )							((MSG_SENDER_OC 	<<16) + X)
/* For MHEG */
#define MSG_BASE_MHEG( X )							((MSG_SENDER_MHEG 	<<16) + X)
/* For SSU */
#define MSG_BASE_SSU( X )							((MSG_SENDER_SSU 	<<16) + X)
/* For Teletext */
#define MSG_BASE_TTX( X )							((MSG_SENDER_TTX 	<<16) + X)
/* For BT */
#define MSG_BASE_BT( X )							((MSG_SENDER_BT <<16) + X) // #ifdef INCLUDE_BT
#define MSG_BASE_TLL( X )							((MSG_SENDER_TLL <<16) + X) // INCLUDE_HOTEL_MODE
#define MSG_BASE_CIFS( X )							((MSG_SENDER_CIFS <<16) + X) // INCLUDE_CIFS
#define MSG_BASE_IPM( X )							((MSG_SENDER_IPM 	<<16) + X) // INCLUDE_IPM
#define MSG_BASE_NM( X )							((MSG_SENDER_NM 	<<16) + X) // INCLUDE_NM
#define MSG_BASE_NSU( X )							((MSG_SENDER_NSU 	<<16) + X) // INCLUDE_NSU
#define MSG_BASE_NETFLIX( X )						((MSG_SENDER_NETFLIX 	<<16) + X) // INCLUDE_NETFLIX_BUILTIN
#define MSG_BASE_DLNA( X )							((MSG_SENDER_DLNA 	<<16) + X) // INCLUDE_DLNA
#define MSG_BASE_WCM(X)								((MSG_SENDER_WCM <<16) + X) // INCLUDE_WIRELESS_READY
#define MSG_BASE_ADDONHOST(X)						((MSG_SENDER_ADDONHOST <<16) + X) // INCLUDE_ADDON
#define MSG_BASE_MHP(X)								((MSG_SENDER_MHP<<16) + X) // INCLUDE_MHP
#define MSG_BASE_VUDU( X )							((MSG_SENDER_VUDU 	<<17) + X) //INCLUDE_VUDU_NATIVE
#define MSG_BASE_MCB( X )							((MSG_SENDER_MCB 	<<16) + X) // INCLUDE_MCB
#define MSG_BASE_GAME( X )							((MSG_SENDER_GAME	<<16) + X) // INCLUDE_MOUSE
#define MSG_BASE_SDP( X )							((MSG_SENDER_SDP	<<16) + X) // INCLUDE_SDP
#define MSG_BASE_WIFIRC( X )						((MSG_SENDER_WIFIRC <<16) + X) // #ifdef INCLUDE_WIFIRC msjin; 20100719
#define MSG_BASE_PQ( X )							((MSG_SENDER_PQ 	<<16) + X)
#define MSG_BASE_MDNS( X )							((MSG_SENDER_MDNS 	<<16) + X)
#define MSG_BASE_RS(X)								((MSG_SENDER_RS	<<16) + X) // INCLUDE_REMOTESVC
#define MSG_BASE_BML(X)								((MSG_SENDER_BML <<16) + X) // INCLUDE_BML
#define MSG_BASE_BCAS(X)							((MSG_SENDER_BCAS <<16) + X) // INCLUDE_BCAS
#define MSG_BASE_HBBTV(X)                           ((MSG_SENDER_HBBTV <<16) + X)   // INCLUDE_HBBTV
#define MSG_BASE_WFD(X)								((MSG_SENDER_WFD <<16) + X)		//INCLUDE_WIFI_DISPLAY
#define MSG_BASE_SMH(X)								((MSG_SENDER_SMH 	<<16) + X)	// INCLUDE_SMARTSHARE

/**
       U S E R   I N T E R F A C E
*/
/* to UI */
#define MSG_UI2UI_SHOW_HOST_DIAGNOSTICS				MSG_BASE_UI(0x0101)
#define UI_EVENT_HDD_AUTOFORMAT						MSG_BASE_UI(0x0102)
#define UI_EVENT_1394_STOP_TO_HDD					MSG_BASE_UI(0x0103)
#define UI_EVENT_1394_STOP_TO_DVHS					MSG_BASE_UI(0x0104)
#define UI_EVENT_REFRESH_TNLIST						MSG_BASE_UI(0x0105)
#define UI_EVENT_PVR_POWEROFF						MSG_BASE_UI(0x0106)
#define UI_EVENT_STOP_RECORD						MSG_BASE_UI(0x0107)
#define UI_EVENT_PVR_DEFAULTTS						MSG_BASE_UI(0x0108)
#define UI_EVENT_PVR_CREATE_STATUSDISPLAY			MSG_BASE_UI(0x0109)

#define MSG_UI2UI_MUSIC_STOPPED						MSG_BASE_UI(0x010C)
#define MSG_UI2UI_FILE_COPY							MSG_BASE_UI(0x010D)
#define MSG_UI2UI_FILE_MOVE							MSG_BASE_UI(0x010E)
#define MSG_UI2UI_FILE_DELETE						MSG_BASE_UI(0x010F)
#define MSG_UI2UI_FILE_COPY_SUCCESS					MSG_BASE_UI(0x0110)
#define MSG_UI2UI_FILE_MOVE_SUCCESS					MSG_BASE_UI(0x0111)
#define MSG_UI2UI_FILE_DELETE_SUCCESS				MSG_BASE_UI(0x0112)
#define MSG_UI2UI_FILE_FAILED						MSG_BASE_UI(0x0113)
#define MSG_UI2UI_PHOTO_SLIDE						MSG_BASE_UI(0x0114)
#define MSG_UI2UI_TIMERLIST_UPDATE					MSG_BASE_UI(0x0116)

/* For PDP ISM method */
#define MSG_UI2UI_ISMMETHOD 						MSG_BASE_UI(0x0117)

/* For POST Start APP */
#define MSG_UI2UI_START_POSTSTARTAPP				MSG_BASE_UI(0x0118)

/* to OAD */ //by injepost
#define MSG_UI2SI_OAD_START_REQUEST					MSG_BASE_UI(0x0120)
#define MSG_UI2SI_OAD_STOP_REQUEST    				MSG_BASE_UI(0x0121)
#define MSG_UI2SI_OAD_USER_NOTWANT					MSG_BASE_UI(0x0122)

/*For Divx DRM */
#define MSG_UI2UI_DRM_FILE_END						MSG_BASE_UI(0x0124)
/*For NETMGR*/
#define MSG_UI2UI_NETMGR_STATE_CHANGED				MSG_BASE_UI(0x0125)

/*For USB Update */
#define MSG_UI2UI_UPDATE_INIT						MSG_BASE_UI(0x0125)
#define MSG_UI2UI_UPDATE_NEW						MSG_BASE_UI(0x0126)
#ifdef INCLUDE_HOTEL_MODE
#define MSG_UI2UI_AUTOCONFIG						MSG_BASE_UI(0x0127)
#endif
/**
       C H A N N E L      M A N A G E R
*/
/* to CM for UI Request */
#define MSG_UI2CM_START_CHANNEL_CHANGE  			MSG_BASE_CM(0x1101)   	/* Type B, CM_ChannelChange_T* */
#define MSG_UI2CM_STOP_CHANNEL_CHANGE   			MSG_BASE_CM(0x1102)   	/* Type A, No data */
#define MSG_UI2CM_START_AUTOSCAN        			MSG_BASE_CM(0x1103)   	/* Type A, channel source */
#define MSG_UI2CM_STOP_AUTOSCAN         			MSG_BASE_CM(0x1104)   	/* Type A, No data */
#define MSG_UI2CM_START_MANUALSCAN      			MSG_BASE_CM(0x1105)   	/* Type A, channel source, rf number */
#define MSG_UI2CM_CANCEL_MANUALSCAN       			MSG_BASE_CM(0x1106)   	/* Type A, No data */
#define MSG_UI2CM_START_BAND_SEARCH     			MSG_BASE_CM(0x1107)   	/* Type A */
#define MSG_UI2CM_STOP_BAND_SEARCH      			MSG_BASE_CM(0x1108)   	/* Type A */
#define MSG_UI2CM_SAVE_MANUALSCAN       			MSG_BASE_CM(0x1109)   	/* Type A, No data */  // sanha
#define MSG_UI2CM_START_EXT_INPUT_CHANGE 			MSG_BASE_CM(0x110c)   	/* Type A, No data */
#define MSG_UI2CM_NOTIFY_EANBLE_INPUT  				MSG_BASE_CM(0x110d)   	/* Type A, No data */
#define MSG_UI2CM_NOTIFY_DISABLE_INPUT  			MSG_BASE_CM(0x110e)   	/* Type A, No data */
#define MSG_UI2CM_START_SKIP_OVERLAPPING_CHNUM  	MSG_BASE_CM(0x110f)   	/* Type A, No data */

/* to CM */
#define MSG_CM2CM_INTERRUPT             			MSG_BASE_CM(0x1201)   	/* Type A, No data */
#define MSG_CM2CM_START_SCAN						MSG_BASE_CM(0x1202)   	/* Type A, Start Scan job */
#define MSG_CM2CM_START_CHANGE						MSG_BASE_CM(0x1203)   	/* Type A, Start Channel Change job */
#define MSG_CM2CM_STATUS							MSG_BASE_CM(0x1204)   	/* Type A, Inform Channel status */
#define MSG_CM2CM_START_PWORK						MSG_BASE_CM(0x1205)   	/* Type A, start periodic work */
#define MSG_CM2CM_STOP_PWORK						MSG_BASE_CM(0x1206)   	/* Type A, stop periodic work */
#define MSG_CM2CM_STATUS_AUTOAV						MSG_BASE_CM(0x1208)   	/* Type A, Inform Auto AV status */
#define MSG_AV2CM_STATUS							MSG_BASE_CM(0x1220)

//#if (SYS_DVB)
/* to SI MAIN */
#define MSG_CM2SM_RESET_CHANNEL     	    		MSG_BASE_CM(0x1301)   	/* reset SI old channel */
#define MSG_CM2SM_REQ_CH_JOB		   		 		MSG_BASE_CM(0x1302)	  	/* request channel job (ch-ch or auto-scan) */
#define MSG_CM2SM_NOTIFY_VCH						MSG_BASE_CM(0x1305)	  	/* confirm current vch */
#define MSG_CM2SM_RUNNING_STATUS					MSG_BASE_CM(0x1306)	  	/* request monitoring running-status */
#define MSG_CM2SM_NOTIFY_PAYCH_STARTCOLLECTION		MSG_BASE_CM(0x1307)		/* request to start satellite pay channel collection */
#define MSG_CM2SM_NOTIFY_OPERATOR_INFO				MSG_BASE_CM(0x1308)		/* to set satellite pay channel operator info */
//#elif (SYS_ATSC)
/* to PSIP MAIN */
#define MSG_CM2PM_RESET_CHANNEL         			MSG_BASE_CM(0x1301)   	/* reset PSIP in old channel */
#define MSG_CM2PM_REQ_CH_JOB		    			MSG_BASE_CM(0x1302)	  	/* request channel job (ch-ch or auto-scan) */
#define MSG_CM2PM_NOTIFY_VCH						MSG_BASE_CM(0x1305)	  	/* confirm current vch */
#define MSG_CM2PM_RESET_ACAP						MSG_BASE_CM(0x1306)	  	/* reset acap for vch */
//#endif

/* to UI */
#define MSG_CM2UI_GOOD_VIDEO						MSG_BASE_CM(0x1401)
#define MSG_CM2UI_BAD_VIDEO							MSG_BASE_CM(0x1402)
#define MSG_CM2UI_VCH_INFO_CHECKED					MSG_BASE_CM(0x1403)
#define MSG_CM2UI_AUTOSCAN_STARTED					MSG_BASE_CM(0x1404)
#define MSG_CM2UI_AUTOSCAN_COMPLETED				MSG_BASE_CM(0x1405)
#define MSG_CM2UI_AUDIO_ONLY						MSG_BASE_CM(0x1406)
#define MSG_CM2UI_AUTOSCAN_CHANNUM					MSG_BASE_CM(0x1407)
#define MSG_CM2UI_GUIDE_SCAN_STARTED				MSG_BASE_CM(0x1408)
#define MSG_CM2UI_GUIDE_SCAN_CHANNUM				MSG_BASE_CM(0x1409)
#define MSG_CM2UI_GUIDE_SCAN_COMPLETED 				MSG_BASE_CM(0x140A)
#define MSG_CM2UI_DATA_TUNE_RESULT					MSG_BASE_CM(0x140B)
#define MSG_CM2UI_WEAK_SIGNAL						MSG_BASE_CM(0x140C)
#define MSG_CM2UI_DATA_ONLY							MSG_BASE_CM(0x140D)
#define MSG_CM2UI_CP_DETECTED						MSG_BASE_CM(0x140E)
#define MSG_CM2UI_CP_CLEARED						MSG_BASE_CM(0x140F)
#define MSG_CM2UI_AUTOSCAN_PROGRESS					MSG_BASE_CM(0x1410)		/* Analog scan progress bar */
#define MSG_CM2UI_SCART_DETECT						MSG_BASE_CM(0x1411)		/* Scart Detect : dhjung */
#define MSG_CM2UI_RECOVER_TV_INPUT					MSG_BASE_CM(0x1412)		/* Return to Tv Input : dhjung */
#define MSG_CM2UI_INVALID_CHANNEL					MSG_BASE_CM(0x1413)
#define MSG_CM2UI_NOTIFY_FE_LOCK					MSG_BASE_CM(0x1414)
#if 0
#define MSG_CM2UI_GOOD_VIDEO_AUTOAV					MSG_BASE_CM(0x1415)
#define MSG_CM2UI_BAD_VIDEO_AUTOAV					MSG_BASE_CM(0x1416)
#endif
#define MSG_CM2UI_NOTIFY_FE_NOT_LOCKED				MSG_BASE_CM(0x1417)		//	2008/03/13
#define MSG_CM2UI_RATING_TABLE						MSG_BASE_CM(0x1418)		/* For DRRT : yong 20080508 */
#define MSG_CM2UI_SSU_DATA_RECEIVE					MSG_BASE_CM(0x1419)
#define MSG_CM2UI_STREAM_CHANGEDED					MSG_BASE_CM(0x1420)
#define MSG_CM2UI_NETWORK_CHANGEDED					MSG_BASE_CM(0x1421)
#define MSG_CM2UI_REPLACE_CHANNEL					MSG_BASE_CM(0x1422)
#define MSG_CM2UI_RETURN_BACK						MSG_BASE_CM(0x1423)
#define MSG_CM2UI_DTCP_DETECTED					MSG_BASE_CM(0x1424)
#define MSG_CM2UI_DTCP_CLEARED					MSG_BASE_CM(0x1425)
#define MSG_CM2UI_NOTIFY_FE_LOCKED				MSG_BASE_CM(0x1426)		//	2010/01/18
#define MSG_CM2UI_NOTIFY_SSU_FE_LOCKINFO			MSG_BASE_CM(0x1426)



#define MSG_CM2UI_ANALOG_MANUALSCAN_STARTED			MSG_BASE_CM(0x1450)
#define MSG_CM2UI_ANALOG_MANUALSCAN_COMPLETED		MSG_BASE_CM(0x1451)
#define MSG_CM2UI_ANALOG_MANUALSCAN_CHANNUM			MSG_BASE_CM(0x1452)
#define MSG_CM2UI_ANALOG_MANUALSCAN_SEARCHFLUSH		MSG_BASE_CM(0x1453)
#define MSG_CM2UI_ANALOG_MANUALSCAN_FINETUNE    	MSG_BASE_CM(0x1454)
#define MSG_CM2UI_ANALOG_MANUALSCAN_CHANNELTUNE		MSG_BASE_CM(0x1455)
#define MSG_CM2UI_ANALOG_MANUALSCAN_SYSTEMTUNE		MSG_BASE_CM(0x1456)
#define MSG_CM2UI_DIGITAL_MANUALSCAN_STARTED		MSG_BASE_CM(0x1457)
#define MSG_CM2UI_DIGITAL_MANUALSCAN_TUNE_OK    	MSG_BASE_CM(0x1458)
#define MSG_CM2UI_DIGITAL_MANUALSCAN_TUNE_NOT_OK	MSG_BASE_CM(0x1459)
#define MSG_CM2UI_DIGITAL_MANUAL_SAVE_COMPLETED		MSG_BASE_CM(0x145A)
#define MSG_CM2UI_MANUALSCAN_COMPLETED				MSG_BASE_CM(0x145B)
#define MSG_CM2UI_TRIGGER_GUIDE						MSG_BASE_CM(0x145C)// 채널 변경시 Guide 정보 변경을 위해. 2008-06-12 Jaehong Kim
#define MSG_CM2UI_ASPECTRAIO_CHANGED				MSG_BASE_CM(0x145D)// Addon event 전달을 위해. 2009-08-13 Jaehong Kim
#define MSG_CM2UI_NOTIFY_SIMULCAST_CHANNEL			MSG_BASE_CM(0x145E)// Addon event 전달을 위해. 2009-08-13 Jaehong Kim
#define MSG_CM2UI_MOTOR_ROTATION_STARTED			MSG_BASE_CM(0x145F)
#define MSG_CM2UI_MOTOR_ROTATION_STOPED				MSG_BASE_CM(0x1460)
#define MSG_CM2UI_TIVU_SERVICES_UPDATED             MSG_BASE_CM(0x1461)
#define MSG_CM2UI_HOMESYNC_ERROR                    MSG_BASE_CM(0x1462)
#define MSG_CM2UI_HOMESYNC_DONE                    	MSG_BASE_CM(0x1463)
#define MSG_CM2UI_SAT_DB_IS_FULL                    MSG_BASE_CM(0x1464)
#define MSG_CM2UI_SERVICE_MOVED						MSG_BASE_CM(0x1465)
#define	MSG_CM2UI_SAT_INVALID_CD_NETWORK_ID			MSG_BASE_CM(0x1466)
#define MSG_CM2UI_ICS_START							MSG_BASE_CM(0x1467)
#define MSG_CM2UI_ICS_END							MSG_BASE_CM(0x1468)

#define	MSG_CM2UI_SAT_INVALID_NETWORK_ID			MSG_BASE_CM(0x1469)
#define MSG_CM2UI_SAT_TP_TSID_DIFFERENT				MSG_BASE_CM(0x146A)
#define MSG_CM2UI_SAT_TUNE_TO_BACKUP_TP				MSG_BASE_CM(0x146B)
#define MSG_CM2UI_SAT_TUNE_TO_LINK_0x04				MSG_BASE_CM(0x146C)
#define MSG_CM2UI_SAT_TUNE_TO_LINK_0x90_0x91		MSG_BASE_CM(0x146D)
#define MSG_CM2UI_SAT_COLLECTING_SGT_SERVICES		MSG_BASE_CM(0x146E)
#define MSG_CM2UI_SAT_LNB_SHORTED					MSG_BASE_CM(0x146F)
#define MSG_CM2UI_TIVU_HOTBIRD_NOT_SCANNED			MSG_BASE_CM(0x1470)
#define MSG_CM2UI_TIVU_SLU_STOPPED                  MSG_BASE_CM(0x1471)
#define MSG_CM2UI_CP_CHANNEL_CHANGE					MSG_BASE_CM(0x1472)
/* to MRE */
#define MSG_MRE2CM_STOP_GUIDESCAN					MSG_BASE_CM(0x1501)
#define MSG_CM2UI_READY_ACAP						MSG_BASE_CM(0x1502)  /* to ACAP */

//#if (SYS_DVB)
/**
       S I (DVB)
*/
/* to SI MAIN */
#define MSG_SF2SM_PAT_DONE        					MSG_BASE_SI(0x2101)
#define MSG_SF2SM_PMT_DONE      					MSG_BASE_SI(0x2102)
#define MSG_SF2SM_NIT_DONE      					MSG_BASE_SI(0x2103)
#define MSG_SF2SM_NITO_DONE							MSG_BASE_SI(0x210E)
#define MSG_SF2SM_SDTA_DONE      					MSG_BASE_SI(0x2104)
#define MSG_SF2SM_EIT_DONE      					MSG_BASE_SI(0x2105)
#define MSG_SF2SM_CAT_DONE      					MSG_BASE_SI(0x2106)
#define MSG_SF2SM_TDT_DONE      					MSG_BASE_SI(0x2107)
#define MSG_SF2SM_TOT_DONE      					MSG_BASE_SI(0x2108)
#define MSG_SF2SM_SCH_EVENT_FULL					MSG_BASE_SI(0x2109)
#define MSG_SF2SM_SDTO_DONE      					MSG_BASE_SI(0x210A)
#ifdef INCLUDE_MHP
#define MSG_SF2SM_AIT_DONE						MSG_BASE_SI(0x210B)
#endif
#define MSG_SF2SM_BAT_DONE      					MSG_BASE_SI(0x210C)	/*For BAT parser: liurunfa */
#ifdef INCLUDE_MHP
#define MSG_SF2SM_TSDT_DONE      					MSG_BASE_SI(0x210D)
#endif
#ifdef SI_ISDB
#define MSG_SF2SM_BIT_DONE							MSG_BASE_SI(0x210E)
#define MSG_SF2SM_CDT_DONE							MSG_BASE_SI(0x210F)
#endif /* SI_ISDB */

#define MSG_TM2SM_PAT_TIMEOUT          				MSG_BASE_SI(0x2201)
#define MSG_TM2SM_GET_TIME         					MSG_BASE_SI(0x2202)
#define MSG_TM2SM_EIT_SCHEDULE     					MSG_BASE_SI(0x2203)

#define	MSG_UI2SM_TIME_ZONE 						MSG_BASE_SI(0x2204)
#define MSG_UI2SM_DST_MODE							MSG_BASE_SI(0x2205)

// by icystar : 2008-07-02 when changing the menulanguage, Discard and Get EPG Data!
#define MSG_UI2SM_EIT_CLEAR_GET						MSG_BASE_SI(0x2206)
#define MSG_UI2SM_UPDATE_CH_INFO					MSG_BASE_SI(0x2207)
#define MSG_UI2SM_CHANGE_TTX_LANG						MSG_BASE_SI(0x2208)
#ifdef INCLUDE_MHP
#define MSG_MHP2SI_REQ_AIT						MSG_BASE_SI(0x2209)
#endif

#define MSG_SF2SM_SGT_DONE							MSG_BASE_SI(0x220A)	/* APS ORS */
#define MSG_SF2SM_BATSPS_DONE						MSG_BASE_SI(0x220B) /* CANALPLUS */
#define MSG_SF2SM_FNT_DONE							MSG_BASE_SI(0x220C)	/* BENELUX */
#define MSG_SF2SM_FST_DONE							MSG_BASE_SI(0x220D)	/* BENELUX */
#define MSG_SF2SM_RPDT_DONE							MSG_BASE_SI(0x220E) /* CANALPLUS */

#define	MSG_SSU2SM_OTA_STATUS 						MSG_BASE_SI(0x2302)
#define MSG_SSU2SI_EIT_CLEAR						MSG_BASE_SI(0x2303)	/* Discard EPG Data! */
#define MSG_UI2SI_EPG_CREATE						MSG_BASE_SI(0x2304)
#define MSG_UI2SI_EPG_EXIT							MSG_BASE_SI(0x2305)

#define	MSG_CI2SM_CAM_INSERTED					MSG_BASE_SI(0x230A)
#define	MSG_CI2SM_CAM_REMOVED						MSG_BASE_SI(0x230B)

/* to CM */
#define MSG_SM2CM_NOTIFY_CH_INFO					MSG_BASE_SI(0x2401)
#define MSG_SM2CM_NOTIFY_CH_BANNER					MSG_BASE_SI(0x2402)
#define MSG_SM2CM_NOTIFY_RETURN_BACK				MSG_BASE_SI(0x240A)
#define MSG_SM2CM_NOTIFY_OTHER_SVCINFO				MSG_BASE_CM(0x240B)
#define MSG_SM2CM_NOTIFY_PMT_INFO					MSG_BASE_CM(0x240C)
#define MSG_SM2CM_NOTIFY_CH_INFO_FAIL				MSG_BASE_CM(0x240D)
/* CANALPLUS */
#define MSG_SM2CM_NOTIFY_REG_SVC_INFO				MSG_BASE_CM(0x240E)

/* to UI */
#define MSG_SI2UI_NEW_EVENT							MSG_BASE_SI(0x2403)	/* notify new event to ui */
#define MSG_SI2UI_CHANGE_TOT						MSG_BASE_SI(0x2404) /* notify tot to ui */
#define MSG_SI2UI_EPG_SCH_EVENT						MSG_BASE_SI(0x2406) /* notify EPG SCH to ui */

//#elif (SYS_ATSC)

/**
       P S I P
*/
/* to PSIP MAIN */
#define MSG_PP2PM_PARSING_DONE          			MSG_BASE_PSIP(0x2101)   /* notify, all sections are parsed */
#define MSG_PP2PM_PARSING_AGAIN        				MSG_BASE_PSIP(0x2102)   /* request, one more parsing */
#define MSG_PP2PM_PARSING_TIMEOUT      				MSG_BASE_PSIP(0x2103)   /* timeout, one more parsing */


#define MSG_TM2PM_NO_PSIP_CH         				MSG_BASE_PSIP(0x2201)	/* timeout PSIP-ON tables, determine PSIP-Off ch */
#define MSG_TM2PM_TIME_UPDATE						MSG_BASE_PSIP(0x2202)	/* alarm for STT Parsing interval */
#define	MSG_TM2PM_EVENT_TIMEOUT						MSG_BASE_PSIP(0x2203)	/* alarm for starting the next event */
#define	MSG_TM2PM_ABNORMAL_PMTS						MSG_BASE_PSIP(0x2204)	/* inconsistent number of PMT with PAT */

#define MSG_UI2PM_REQ_EPG_JOB        				MSG_BASE_PSIP(0x2303)
#define MSG_SSU2PSIP_OTA_STATUS						MSG_BASE_PSIP(0x2304)	/* Inform PSIP from SSU */
#define MSG_SSU2PSIP_EPG_CLEAR						MSG_BASE_PSIP(0x2305)	/* Inform PSIP from SSU in order to Clear EPG */

/* to CM */
#define MSG_PM2CM_NOTIFY_CH_INFO					MSG_BASE_PSIP(0x2401)
#define MSG_PM2CM_NOTIFY_CH_BANNER					MSG_BASE_PSIP(0x2402)
#define MSG_PM2CM_NOTIFY_EPG_SCAN					MSG_BASE_PSIP(0x2403)
#define MSG_PM2CM_NOTIFY_VCH_TUNE					MSG_BASE_PSIP(0x2404)
#define MSG_PM2CM_NOTIFY_ACAP_INFO					MSG_BASE_PSIP(0x2405)
#define MSG_PM2CM_NOTIFY_OTA_INFO					MSG_BASE_PSIP(0x2406)
#define MSG_PM2CM_NOTIFY_RRT						MSG_BASE_PSIP(0x2410)	/* For DRRT : yong 20080508 */

/* to PSIP Parser */
#define MSG_PS2PP_NORMAL              				MSG_BASE_PSIP(0x2501)	/* toss section for parsing */
#define MSG_PS2PP_NEW_VERSION         				MSG_BASE_PSIP(0x2502)	/* toss section and notify version */
#define MSG_PS2PP_TIME_OUT							MSG_BASE_PSIP(0x2503)	/* toss section for time out */

#define	MSG_PM2PP_DELETE_PCH						MSG_BASE_PSIP(0x2601)	/* delete given pch */
#define	MSG_PM2PP_INIT_PCH							MSG_BASE_PSIP(0x2602)	/* delete given pch */

/* to UI */
#define MSG_PSIP2UI_OTA_START						MSG_BASE_PSIP(0x2701)
#define MSG_PSIP2UI_NEW_EVENT						MSG_BASE_PSIP(0x2702)	/* notify new event to ui for Guide */

//#endif
/**
       C I (Europe)
*/
#define CI_MSG_CA_SYSTEM_ID							MSG_BASE_CI(0x3101)		/* Send CA system ID from SI to CI */
#define CI_MSG_CA_PMT								MSG_BASE_CI(0x3102)		/* Send PMT from SI to CI */
#define CI_MSG_CC_TIMEOUT							MSG_BASE_CI(0x3103)		/* Send Channel change timeout msg to CAS */
#define CI_MSG_VIEWED_SERVICE_REQ					MSG_BASE_CI(0x3104)		/* Request Viewed service to SI */
#define CI_MSG_VIEWED_SERVICE_ANSW					MSG_BASE_CI(0x3105)		/* Send Service information from SI */
#define CI_MSG_SELECTION_INFO_REQ					MSG_BASE_CI(0x3106)		/* Request Selction Information */
#define CI_MSG_SELECTION_INFO_ANSW					MSG_BASE_CI(0x3107)		/* Send Selction Inforamtion */
#define CI_MSG_DISPLAY_MESSAGE						MSG_BASE_CI(0x3108)		/* Send Display Information to UI */
#define CI_EVT_NOTIFY								MSG_BASE_CI(0x3109)
#ifdef INCLUDE_CIPLUS
#define CI_MSG_CA_CI_ENTRY_ID						MSG_BASE_CI(0x310A)		/* Send SDT CI Entry Id  from SI to CI */ //boae 20090508
#endif
#define CI_MSG_CAINFO								MSG_BASE_CI(0x3111)		/* Send CA system ID from CI to UI */
#define CI_MSG_AUDIO_CHANGE							MSG_BASE_CI(0x3112)


#define  HOSTCTR_TO_UI_MSG							MSG_BASE_CI(0x3120)
#define  UI_TO_HOSTCTR_MSG							MSG_BASE_CI(0x3121)
#define  UI_KEY_MMI_OPEN							MSG_BASE_CI(0x3122)
#define  UI_KEY_MMI_CLOSE							MSG_BASE_CI(0x3123)
#define  UI_KEY_MMI_KEYPAD_CONTROL					MSG_BASE_CI(0x3124)
#define  UI_KEY_MMI_ENQ_ANSW						MSG_BASE_CI(0x3125)
#define  UI_KEY_MMI_MENU							MSG_BASE_CI(0x3126)
#define  UI_KEY_MMI_LIST							MSG_BASE_CI(0x3127)
#define  UI_MENU_CI_INFO							MSG_BASE_CI(0x3128)
#define  UI_MENU_CI_ENTER_MENU						MSG_BASE_CI(0x3129)
#define  UI_MENU_CI_EXIT_MENU						MSG_BASE_CI(0x312A)
#define  UI_KEY_MMI_CONFIRM							MSG_BASE_CI(0x312B)
#ifdef INCLUDE_CIPLUS
#define  UI_KEY_APPMMI_ENTER						MSG_BASE_CI(0x312C)		// CI info banner
#define  UI_KEY_APPMMI_READY						MSG_BASE_CI(0x312D)		// StartReq
#define  UI_KEY_APPMMI_EXIT							MSG_BASE_CI(0x312E)		// AppAbort
#define  CAMUPGRADE_TO_UI_MSG						MSG_BASE_CI(0x312F)
#define  URI_INFO_TO_UI_MSG							MSG_BASE_CI(0x3130)
#endif

#ifdef INCLUDE_VCHB
#define  MSG_VCHB_READY_OPERATION					MSG_BASE_CI(0x4001)
#define  MSG_VCHB_END_OPERATION						MSG_BASE_CI(0x4002)
#define  MSG_VCHB_REQUEST_LOCAL_CH_INFO				MSG_BASE_CI(0x4003)
#define  MSG_VCHB_REQUEST_NET_CH_INFO				MSG_BASE_CI(0x4004)
#define  MSG_VCHB_REQUEST_NET_EPG_INFO					MSG_BASE_CI(0x4005)
#define  MSG_VCHB_REQUEST_NET_ICON_INFO					MSG_BASE_CI(0x4006)

#define  MSG_VCHB_RECEIVE_LOCAL_CH_INFO				MSG_BASE_CI(0x4020)
#define  MSG_VCHB_RECEIVE_NET_CH_INFO				MSG_BASE_CI(0x4021)
#define  MSG_VCHB_RECEIVE_NET_EPG_INFO			MSG_BASE_CI(0x4022)
#define  MSG_VCHB_RECEIVE_NET_ICON_INFO				MSG_BASE_CI(0x4024)
#define  MSG_VCHB_RECEIVE_NET_URL_INIT				MSG_BASE_CI(0x4025)
#define  MSG_VCHB_RECEIVE_NET_JPG_INFO				MSG_BASE_CI(0x4026)
#define  MSG_VCHB_DONE_EPG_GATHERING					MSG_BASE_CI(0x4030)



#define  MSG_GRACENOTE_INIT_URL_LIST                MSG_BASE_CI(0x4100)
#define  MSG_GRACENOTE_REQUEST_CHANNEL_LIST         MSG_BASE_CI(0x4101)
#define  MSG_GRACENOTE_REQUEST_EPG_INFO             MSG_BASE_CI(0x4102)
#define  MSG_GRACENOTE_REQUEST_THUMBNAIL            MSG_BASE_CI(0x4103)
#define  MSG_GRACENOTE_DESTROY						MSG_BASE_CI(0x4110)

/*to vchb net*/
#define MSG_VCHB_PAR2NET_REQUSET_EPGINFO				MSG_BASE_CI(0x4201)


/*to vchb parser*/
#define  MSG_VCHB_MON2PAR_REQUEST_EPGINFO				MSG_BASE_CI(0x4211)
#define  MSG_VCHB_NET2PAR_RESPOND_EPGINFO				MSG_BASE_CI(0x4212)


//to vchb monitor



#endif
/**
       A C A P
*/

#define	MSG_ACAP2ACAP_RETRIEVE						MSG_BASE_ACAP(0x5101)
#define MSG_ACAP_SECTION_ACQUIRED					MSG_BASE_ACAP(0x5201)	/* toss Acap section handler from SDEC */
/* to UI */
/* 이 message를 받으면 OCAP module을 위한 channel change를 해야 함. 모든 경우에  API_CM_OCAP_ChannelChange를 호출해야 함.
   param[0] : Channel Change Mode
   param[1] : 상위 16bit : Default Componet가 정의되어 있는지의 여부
             하위 16bit : network id (CM에 전달해줘야 함)
   param[2] : First Argument
   parma[3] : Second Argument */
#define	MSG_ACAP2UI_CHANNEL_CHANGE					MSG_BASE_ACAP(0x5301) /* UI_MSG_ACAP_CHANNEL_CHANGED : 0x5301*/
/* 이 message를 받으면 OCAP module을 위한 Component 변경을 시도함.
   param[0] : Component 변경 모드 -> CM에 전달필요.
   Component 변경 모드가 STOP_PLAY_COMPONENT 모드일 경우에는 Block을 걸어야 함.
   Component 변경 모드가 START_PLAY_COMPONENT일 경우 Block 해제
   모든 경우에 한해 API_CM_OCAP_CompChange를 호출함.
   param[1] : Video Mute 여부
   param[2] : Audio Mute 여부
   parma[3] : NULL */
#define	MSG_ACAP2UI_COMPONENT_CHANGE				MSG_BASE_ACAP(0x5302)	 /* UI_MSG_ACAP_COMPONENT_CHANGED : 0x5302*/
/* param[0] : MOA의 status
   			MOA_ENABLE : 데이터 방송이 가능한 상태임.
   			MOA_DISABLE : 데이터 방송이 불가능한 상태임.
   			MOA_LOAD : 데이터 방송(Bound Application)을 로딩 중임.
   			MOA_ON : 데이터 방송(Bound Application)이 실행 중임.
   			MOA_INIT : 데이터 방송이 초기화됨.
   			MOA_HIDE : 데이터 방송이 숨겨진 상태임.
   			MOA_ABNORMAL : 데이터 방송이 비정상적인 상태임.
   param[1] : shared Flag
   			0 : 채널 전환 시 비디오 Resize를 Full로 설정
   			1 : 채널 전환 시 비디오 Resize를 Full로 설정하지 않고 그대로 둠.
   param[2] : abnormal reason
   parma[3] : NULL */
#define MSG_ACAP2UI_MOA_STATUS_CHANGED				MSG_BASE_ACAP(0x5303) 	/* MSG_ACAP2UI_MOA_STATUS_CHANGED : 0x5303*/
#define MSG_ACAP2UI_TV_NOTIFY_KEY					MSG_BASE_ACAP(0x5304)	/* MSG_ACAP2UI_TV_NOTIFY_KEY : 0x5304*/
#define MSG_ACAP2UI_IPM_COMPLETE						MSG_BASE_ACAP(0x5305)	/* MSG_ACAP2UI_IPM_COMPLETE : 0x5307 */
#define MSG_ACAP2UI_IPM_ERROR						MSG_BASE_ACAP(0x5306)	/* MSG_ACAP2UI_IPM_ERROR : 0x5308 */
//aeroshin 2006.11.23 ACAP chch시 notify select severvice 위해 수정함.
#define	MSG_ACAP2UI_SERVICE_CHANGE					MSG_BASE_ACAP(0x5307)	/* MSG_ACAP2UI_SERVICE_CHANGE : 0x5309*/
#define MSG_ACAP2UI_RESIZE_VIDEO						MSG_BASE_ACAP(0x5308)	/* MSG_ACAP2UI_VIDEO_RESIZE : 0x530A */

#ifdef INCLUDE_IPM
#define MSG_IPM2UI_IPM_COMPLETE						MSG_BASE_IPM(0x5318)	/* MSG_IPM2UI_IPM_COMPLETE : 0x0x5318 */
#define MSG_IPM2UI_IPM_ERROR							MSG_BASE_IPM(0x5319)	/* MSG_IPMP2UI_IPM_ERROR : 0x5319 */
#endif

#ifdef INCLUDE_NM
#define MSG_IPM2UI_IPM_COMPLETE						MSG_BASE_NM(0x5318)	/* MSG_IPM2UI_IPM_COMPLETE : 0x0x5318 */
#define MSG_IPM2UI_IPM_ERROR							MSG_BASE_NM(0x5319)	/* MSG_IPMP2UI_IPM_ERROR : 0x5319 */
#endif

/**
       M I C O M
*/
/* to UI */
#define	MSG_MICOM2UI_OFF_TIMER						MSG_BASE_MICOM(0x6101)
#define	MSG_MICOM2UI_ON_TIMER						MSG_BASE_MICOM(0x6102)
#define	MSG_MICOM2UI_POWER							MSG_BASE_MICOM(0x6103)
#define	MSG_MICOM2UI_KEYLOCK						MSG_BASE_MICOM(0x6104)
#define	MSG_MICOM2UI_DIGITAL_EYE_CHANGED			MSG_BASE_MICOM(0x6105)
#define MSG_MICOM2UI_CLEAR_AUTOOFF					MSG_BASE_MICOM(0x6106)
#define MSG_MICOM2UI_BYPASS_AUTO_GUIDE_ON			MSG_BASE_MICOM(0x6107)
#define MSG_MICOM2UI_TOUCH_KEY_RECEIVED				MSG_BASE_MICOM(0x6108)
#define MSG_MICOM2UI_TVLINK_IRCOMMAND				MSG_BASE_MICOM(0x6109)
#define MSG_MICOM2UI_OTA_START						MSG_BASE_MICOM(0x610A)
#define MSG_MICOM2UI_WB_IRCOMMAND   				MSG_BASE_MICOM(0x610B)
#define MSG_MICOM2UI_IRTYPE_CHANGE   				MSG_BASE_MICOM(0x610C)
#define MSG_MICOM2UI_SELF_DIAGNOSIS_IR_CMD		MSG_BASE_MICOM(0x610D)
#define MSG_MICOM2UI_LOCAL_DIMMING				MSG_BASE_MICOM(0x610E)		/* 091222 매장전용 -Local Dimming */

/**
       D A S Y
*/
#define MSG_DASY2UI_INITIALIZED						MSG_BASE_DASY(0x7001)
#define MSG_DASY2UI_UNINITIALIZED					MSG_BASE_DASY(0x7002)
#define MSG_DASY2UI_DOWNLOAD_FAILURE				MSG_BASE_DASY(0x7003)
#define MSG_DASY2UI_UPLOAD_FAILURE					MSG_BASE_DASY(0x7004)
#define MSG_DASY2UI_RECORDFAIL_DISKFULL				MSG_BASE_DASY(0x7005)
#define MSG_DASY2UI_RECORDFAIL_TOOSHORT				MSG_BASE_DASY(0x7006)
#define MSG_DASY2UI_HDDPLAYBACK_SHOWPGBAR  			MSG_BASE_DASY(0x7007)
#define MSG_DASY2UI_HDDPLAYBACK_STOP				MSG_BASE_DASY(0x7008)
#define MSG_DASY2UI_HDDPLAYBACK_PAUSE				MSG_BASE_DASY(0x7009)
#define MSG_DASY2UI_HDDPLAYBACK_END					MSG_BASE_DASY(0x700A)
#define MSG_DASY2UI_HDDPLAYBACK_RESUME				MSG_BASE_DASY(0x700B)
#define	MSG_DASY2UI_VIDEO_MUTE_BLACK				MSG_BASE_DASY(0x700C)
#define	MSG_DASY2UI_VIDEO_MUTE_FREEZE				MSG_BASE_DASY(0x700D)
#define	MSG_DASY2UI_VIDEO_MUTE_OFF					MSG_BASE_DASY(0x700E)
#define	MSG_DASY2UI_AUDIO_MUTE_ON					MSG_BASE_DASY(0x700F)
#define	MSG_DASY2UI_AUDIO_MUTE_OFF					MSG_BASE_DASY(0x7010)
#define	MSG_DASY2UI_TOO_MANY_FILES					MSG_BASE_DASY(0x7011)
#define MSG_DASY2UI_COMPLETED_REC					MSG_BASE_DASY(0x7012)	/* 녹화정지후 파일 정리가 완전히 종료한경우 */
#define MSG_DASY2UI_COMPLETED_EDIT					MSG_BASE_DASY(0x7013)	/* 녹화프로그램 재생 종료시 편집 설정후 파일 정리가 */
																		 	/* 완전히 종료한경우 */
//#define MSG_DASY2UI_COMPLETED_REC					MSG_BASE_DASY(0x7011)	/* 녹화정지후 파일 정리가 완전히 종료한경우 */
#define MSG_DASY2UI_COMPLETED_TS					MSG_BASE_DASY(0x7014)	/* TS 종료시 */
#define MSG_DASY2UI_COMPLETED_PLAY					MSG_BASE_DASY(0x7015)	/* 녹화프로그램 재생 종료시 */
#define MSG_DASY2UI_COMPLETED_DRAGANDPLAY			MSG_BASE_DASY(0x7016)	/*  Drag&Play 종료시 */
#define MSG_DASY2UI_LIVETV_STATE_ON					MSG_BASE_DASY(0x7017)	/*  */
#define MSG_DASY2UI_LIVETV_STATE_OFF				MSG_BASE_DASY(0x7018)	/*  */
#define MSG_DASY2UI_MFSREAD_FAIL					MSG_BASE_DASY(0x7019)	/*  play 중 fail. (hdd & usb hdd) */
#define MSG_DASY2UI_MFSWRITE_FAIL					MSG_BASE_DASY(0x7020)	/*  rec or backup 중 fail. (hdd & usb hdd) */
#define MSG_DASY2UI_BACKUPFAIL_DISKFULL 			MSG_BASE_DASY(0x7021)
#define	MSG_DASY2UI_RATING_CHANGE					MSG_BASE_DASY(0x7022)
#define	MSG_DASY2UI_WSS_CHANGE						MSG_BASE_DASY(0x7023)
#define MSG_DASY2UI_SESSION_CHANGE					MSG_BASE_DASY(0x7024) // Session Change

#define  MSG_DASY2MLM_BACKUP_COMPLETE				MSG_BASE_DASY(0x7031)
#define  MSG_DASY2MLM_BACKUP_FAIL					MSG_BASE_DASY(0x7032)
#define  MSG_DASY2MLM_BACKUP_BROKEN					MSG_BASE_DASY(0x7033)
#define  MSG_DASY2MLM_BACKUP_SUSPEND				MSG_BASE_DASY(0x7034)
#define  MSG_DASY2MLM_BACKUPSTOP_DISKFULL 			MSG_BASE_DASY(0x7035)	/* USB HDD disk full --> tobe deleted.. */
#define	 MSG_DASY2MLM_BACKUP_STOP					MSG_BASE_DASY(0x7036)

/**
       S Y S T E M
*/
/* Analog Caption */
#define	MSG_FIELD1_CC 								MSG_BASE_SYS(0x7101)
#define	MSG_FIELD2_CC 								MSG_BASE_SYS(0x7102)
#define MSG_RATING_EXT_CHECK						MSG_BASE_SYS(0x7103)
#define MSG_RATING_D1_CHECK 						MSG_BASE_SYS(0x7104)
#define MSG_FLUSH_CC								MSG_BASE_SYS(0x7105)
#define	MSG_FIELD1_DTVCC 							MSG_BASE_SYS(0x7106)
#define	MSG_FIELD2_DTVCC 							MSG_BASE_SYS(0x7107)

#if (SYS_ATSC)
/* to PSIP Cleaner */
#define MSG_TM2CN_CLEANING_TIMEOUT					MSG_BASE_SYS(0x7201)	/* alarm for next cleaning time  */

/* from Memory Manager */
#define MSG_MM2CN_CLEANING_REQ        				MSG_BASE_SYS(0x0051)	/* TBD:request, cleaning PSIP DB */
#endif

/* from SDEC to SI PSIP Parser */
#define MSG_SDEC2SIPSIP_SECTION_ACQUIRED			MSG_BASE_SYS(0x7301)	/* toss section from SDEC */
#define MSG_SDEC2TTXSUBT_PES_ACQUIRED				MSG_BASE_SYS(0x7302)	/* toss PES from SDEC */

/* from util to UI */
#define MSG_UTIL2UI_CUR_TIME_SET					MSG_BASE_SYS(0x7303)	/* notify to ui current time set completed */

/* from Service Mode to UI */
#define MSG_SVC2UI_AUTO_WB_PROCESS					MSG_BASE_SYS(0x7304)	/* notify to ui to request src change into Analog */

//#if (SYS_ATSC)
/**
       C A P T I O N
*/
/* to CM */
#define MSG_CC2CM_RATING                			MSG_BASE_SYS(0xA101)	/* Type A, rating byte 2 */
#define MSG_CC2CM_NETWORKNAME						MSG_BASE_SYS(0xA102)	/* CC Network Name byte 2 */
#define MSG_SDEC2CM_CA_INFO							MSG_BASE_SYS(0xA103)
//#endif

/**
       F R O N T E N D
*/
/* to CM */
#define MSG_FE2CM_DIGITAL_LOCKED       				MSG_BASE_DRV(0x8101)	/* Type A, Ch Num, Mode */
#define MSG_FE2CM_DIGITAL_NOT_LOCKED   				MSG_BASE_DRV(0x8102)	/* Type A, Ch Num, Mode */
#define MSG_FE2CM_ANALOG_LOCKED       				MSG_BASE_DRV(0x8103)	/* Type A, Ch Num, Mode */
#define MSG_FE2CM_ANALOG_NOT_LOCKED   				MSG_BASE_DRV(0x8104)	/* Type A, Ch Num, Mode */
#define MSG_FE2CM_WEAK_SIGNAL   					MSG_BASE_DRV(0x8105)	/* Type A, Ch Num, Mode */
#define MSG_FE2CM_ANALOG_PROGRESS  					MSG_BASE_DRV(0x8106)	/* Type A, Ch Num, Mode */
#define MSG_FE2CM_MOTOR_ROTATE_STARTED 				MSG_BASE_DRV(0x8107)	/* Type A, Ch Num, Mode */
#define MSG_FE2CM_MOTOR_ROTATE_STOPED 				MSG_BASE_DRV(0x8108)	/* Type A, Ch Num, Mode */
#define MSG_FE2CM_LNB_SHORTED						MSG_BASE_DRV(0x8109)
#define MSG_FE2CM_EMERGENCY_STATUS					MSG_BASE_DRV(0x810A)


/**
       V D E C
*/
/* to CM */
#define MSG_VIDEO2CM_RUNNING            			MSG_BASE_DRV(0x9101)	/* Type A, No Data */
#define MSG_VIDEO2CM_STOPPED            			MSG_BASE_DRV(0x9102)	/* TYpe A, No Data */
#define MSG_VIDEO2CM_FORMAT_CHANGE         			MSG_BASE_DRV(0x9103)	/* TYpe A, No Data */


#define MSG_VDEC_NOTIFY_SEQ            				MSG_BASE_DRV(0x9110) 	/* otify vdec seq. header interrupt */
#define MSG_VDEC_NOTIFY_I_PIC            			MSG_BASE_DRV(0x9111) 	/* otify vdec picture interrupt(I) */
#define MSG_VDEC_NOTIFY_P_PIC            			MSG_BASE_DRV(0x9112) 	/* otify vdec picture interrupt(I) */
#define MSG_VDEC_NOTIFY_B_PIC            			MSG_BASE_DRV(0x9113)	/* notify vdec picture interrupt(I) */


/**
	   HDMI
*/
/* to UI */
#define MSG_VIDEO2UI_COSA_CHANGE            		MSG_BASE_DRV(0x9201) /* Type A, No Data */
// [vivakjh] 2008/12/03	DVB Analog Color System 판단 후 화질 적용을 위한 VIDEO to UI Message Ready
#define MSG_VIDEO2UI_ANA_COLSYS_CHANGE            	MSG_BASE_DRV(0x9202) /* Type A, No Data */
// [vivakjh] 2009/06/10 MediaDirector Spec Out!
//#define MSG_VIDEO2UI_THX_MD_CHANGE            		MSG_BASE_DRV(0x9203) /* Type A, No Data */

#define MSG_VIDEO2UI_HDMIAUTOSTATUS	            	MSG_BASE_DRV(0x9203) /* Type A, No Data */
#define MSG_VIDEO2UI_HDMIAUTOCHANGECOMPLETE            	MSG_BASE_DRV(0x9204) /* Type A, No Data */

/**
       USER DATA
*/
/* to UI */
#define MSG_VDEC2UI_CC_CHANGE						MSG_BASE_DRV(0xC101)	/* Type A, No Data */
#define MSG_VDEC2UI_FORMAT_CHANGE          			MSG_BASE_DRV(0xC102)	/* TYpe A, No Data */

/**
	   HDMI-CEC
*/
#ifdef	INCLUDE_HDMICEC //GP : 2008520 bsg
#define MSG_CECP2UI_RESPONSE_FAILED					MSG_BASE_CECP(0xD100)
#define MSG_CECP2UI_CONNECTION_UPDATED				MSG_BASE_CECP(0xD101)
#define MSG_CECP2UI_REQUEST_HDMICEC					MSG_BASE_CECP(0xD102)
#define MSG_CECP2UI_HPD_OK							MSG_BASE_CECP(0xD103)
#define MSG_CECP2UI_AUDIOMODE_CHANGED				MSG_BASE_CECP(0xD104)
#define MSG_CECP2UI_REQUEST_HPD						MSG_BASE_CECP(0xD105)
#define MSG_CECP2UI_UPDATE_VOLUME					MSG_BASE_CECP(0xD106)
#endif

/**
       M F S
*/
/* to MFS for DASY Request */
#define	MSG_DASY2MFS_READ							MSG_BASE_MFS(0xE101)
#define	MSG_DASY2MFS_WRITE							MSG_BASE_MFS(0xE102)

/* to DASY */
#define MSG_MFS2DASY_READ_DONE						MSG_BASE_MFS(0xE201)
#define MSG_MFS2DASY_WRITE_DONE						MSG_BASE_MFS(0xE202)

#ifdef INCLUDE_MFS_NEW
/* to MFS for UI Request */
#define	MSG_UI2MFS_FORMAT_ALL						MSG_BASE_MFS(0xE303)
#define	MSG_UI2MFS_CLEAR							MSG_BASE_MFS(0xE304)
#define	MSG_UI2MFS_CHECKDISK						MSG_BASE_MFS(0xE305)
#define MSG_UI2MFS_GET_SPACE						MSG_BASE_MFS(0xE306)
#define MSG_UI2MFS_USB_DEV_INIT						MSG_BASE_MFS(0xE309)

#ifdef INCLUDE_IPTV
#define MSG_UI2MFS_FORMAT_VOD						MSG_BASE_MFS(0xE310)
#define MSG_UI2MFS_CHANGE_DEV_LAYOUT				MSG_BASE_MFS(0xE311)
#endif

/* to UI */
#define	MSG_MFS2UI_DEV_INIT_DONE					MSG_BASE_MFS(0xE401)
#define	MSG_MFS2UI_DUPLICATE_DEV					MSG_BASE_MFS(0xE402)
#define	MSG_MFS2UI_FORMAT_ALL_DONE					MSG_BASE_MFS(0xE403)
#define	MSG_MFS2UI_CLEAR_DONE						MSG_BASE_MFS(0xE404)
#define	MSG_MFS2UI_CHECKDISK_DONE					MSG_BASE_MFS(0xE405)
#define	MSG_MFS2UI_CHECKSPACE_DONE					MSG_BASE_MFS(0xE406)
#define	MSG_MFS2UI_GET_SPACE_DONE					MSG_BASE_MFS(0xE407)

#define MSG_MFS2UI_DEV_ATTACHED						MSG_BASE_MFS(0xE411)	/*  */
#define MSG_MFS2UI_DEV_DETACHED						MSG_BASE_MFS(0xE412)	/*  */
#define MSG_MFS2UI_DEV_DAMAGED						MSG_BASE_MFS(0xE413)	/*  */
#define MSG_MFS2UI_DEV_FIRMWARE_UPGRADE				MSG_BASE_MFS(0xE414)	/*  */
#define MSG_MFS2UI_DEV_FAST_CONNECT					MSG_BASE_MFS(0xE415)	/*  */
#define MSG_MFS2UI_ACCESS_USB_DEV					MSG_BASE_MFS(0xE416)	/*  */

#ifdef INCLUDE_IPTV
#define MSG_MFS2UI_FORMAT_VOD_DONE					MSG_BASE_MFS(0xE419)
#define MSG_MFS2UI_CHANGE_DEV_LAYOUT_DONE			MSG_BASE_MFS(0xE420)
#endif

#define MSG_MFS2UI_GPHOTO2_FULLIMG					MSG_BASE_MFS(0xE421)
#define MSG_MFS2MFS_GPHOTO							MSG_BASE_MFS(0xE422)

#define MSG_MFS2UI_REQ_REBOOT						MSG_BASE_MFS(0xE501)	/*  */
#define	MSG_MFS2MFS_INIT							MSG_BASE_MFS(0xE502)

/* to MFS for Performance Test */
#define MSG_MFS_TEST_READ							MSG_BASE_MFS(0xE531)	/*  */
#define MSG_MFS_TEST_WRITE							MSG_BASE_MFS(0xE532)	/*  */
#define MSG_MFS_TEST_COPY							MSG_BASE_MFS(0xE533)	/*  */
#else	// INCLUDE_MFS_NEW

/* to MFS for UI Request */
#define	MSG_UI2MFS_SATA_INIT						MSG_BASE_MFS(0xE301)	/* Load SATA Module */
#define	MSG_UI2MFS_FORMAT_DRIVE						MSG_BASE_MFS(0xE302)
#define	MSG_UI2MFS_FORMAT_ALL						MSG_BASE_MFS(0xE303)
#define	MSG_UI2MFS_CLEAR							MSG_BASE_MFS(0xE304)
#define	MSG_UI2MFS_CHECKDISK						MSG_BASE_MFS(0xE305)
#define MSG_UI2MFS_GET_SPACE						MSG_BASE_MFS(0xE306)
#ifdef _USE_HDD_SLEEP_ /* hdd sleep/active control 안함. jihyeon@20080724 */
#define MSG_UI2MFS_ACTIVE_HDD						MSG_BASE_MFS(0xE307)
#endif
#define MSG_UI2MFS_USB_BACKUP_HDD_INIT				MSG_BASE_MFS(0xE308)
#define MSG_UI2MFS_USB_HDD_INIT						MSG_BASE_MFS(0xE309)

#ifdef INCLUDE_IPTV
#define MSG_UI2MFS_FORMAT_VOD						MSG_BASE_MFS(0xE310)
#define MSG_UI2MFS_CHANGE_DEV_LAYOUT				MSG_BASE_MFS(0xE311)
#endif
#define MSG_UI2MFS_APPDEV_INIT						MSG_BASE_MFS(0xE312)

/* to UI */
#define	MSG_MFS2UI_DVR_INIT_DONE					MSG_BASE_MFS(0xE401)
#define	MSG_MFS2UI_FORMAT_DRIVE_DONE				MSG_BASE_MFS(0xE402)
#define	MSG_MFS2UI_FORMAT_ALL_DONE					MSG_BASE_MFS(0xE403)
#define	MSG_MFS2UI_CLEAR_DONE						MSG_BASE_MFS(0xE404)
#define	MSG_MFS2UI_CHECKDISK_DONE					MSG_BASE_MFS(0xE405)
#define	MSG_MFS2UI_CHECKSPACE_DONE					MSG_BASE_MFS(0xE406)
#define	MSG_MFS2UI_GET_SPACE_DONE					MSG_BASE_MFS(0xE406)
#ifdef _USE_HDD_SLEEP_ /* hdd sleep/active control 안함. jihyeon@20080724 */
#define MSG_UI2MFS_ACTIVE_HDD_DONE					MSG_BASE_MFS(0xE408)
#endif
#define MSG_MFS2UI_INSTOP_FORMAT_DONE				MSG_BASE_MFS(0xE409)

#define MSG_MFS2UI_DEV_ATTACHED						MSG_BASE_MFS(0xE411)	/*  */
#define MSG_MFS2UI_DEV_DETACHED						MSG_BASE_MFS(0xE412)	/*  */
#define MSG_MFS2UI_DEV_LOADED						MSG_BASE_MFS(0xE413)	/*  */
#define MSG_MFS2UI_DEV_UNLOADED						MSG_BASE_MFS(0xE414)	/*  */
#define MSG_MFS2UI_DEV_DAMAGED						MSG_BASE_MFS(0xE415)	/*  */
#define MSG_MFS2UI_DEV_EJECT						MSG_BASE_MFS(0xE416)	/*  */
#define MSG_MFS2UI_DEV_FIRMWARE_UPGRADE				MSG_BASE_MFS(0xE417)	/*  */
#define MSG_MFS2UI_DEV_FULL_SPEED					MSG_BASE_MFS(0xE418)	/*  */

#ifdef INCLUDE_IPTV
#define MSG_MFS2UI_FORMAT_VOD_DONE					MSG_BASE_MFS(0xE419)
#define MSG_MFS2UI_CHANGE_DEV_LAYOUT_DONE			MSG_BASE_MFS(0xE420)
#endif

#define	MSG_MFS2UI_APP_INIT_DONE					MSG_BASE_MFS(0xE421)
#define MSG_MFS2UI_DEV_FAST_CONNECT					MSG_BASE_MFS(0xE422)	/*  */

#define MSG_MFS2UI_GPHOTO2_FULLIMG					MSG_BASE_MFS(0xE424)

#define MSG_MFS2UI_REQ_REBOOT						MSG_BASE_MFS(0xE501)	/*  */

/* to MFS for Performance Test */
#define MSG_MFS_TEST_READ							MSG_BASE_MFS(0xE531)	/*  */
#define MSG_MFS_TEST_WRITE							MSG_BASE_MFS(0xE532)	/*  */
#define MSG_MFS_TEST_COPY							MSG_BASE_MFS(0xE533)	/*  */

#define	MSG_MFS2MFS_INIT							MSG_BASE_MFS(0xE502)

#define MSG_MFS2MFS_GPHOTO							MSG_BASE_MFS(0xE530)
#endif

/**
		E M E
*/
#define MSG_EME2UI_SUBT_FOUND						MSG_BASE_EME(0x1001)	/* EME divx subtitle text found event */
#define MSG_EME2UI_STOPPED							MSG_BASE_EME(0x1002)	/* MP3 STOPPED event */
#define MSG_EME2UI_PHOTO_DECODINGDONE				MSG_BASE_EME(0x1003)	/* Image file의 decoding 완료 */
#define MSG_EME2UI_PHOTO_FULL_DISPLAYDONE			MSG_BASE_EME(0x1004)	/* Image file의 Display 완료 */
#define MSG_EMF2UI_PHOTO_PREVIEW_DISPLAYDONE		MSG_BASE_EME(0x1005)	/* Image Preview의 Display 완료 */
#define MSG_EMF2UI_PHOTO_ROTATE_DISPLAYDONE			MSG_BASE_EME(0x1006)	/* Image Rotate의 Display 완료 */
#define MSG_EME2UI_PHOTO_DECODING_ERROR				MSG_BASE_EME(0x1007)	/* Image file의 decoding 실패 */
#define MSG_EME2UI_MOVIE_CHAPTER_THUMBNAIL			MSG_BASE_EME(0x1008)	/* DivX PLUS HD Chapter 정보 */
#define MSG_EME2UI_MUSIC_ELAPSEDTIME				MSG_BASE_EME(0x1009)	/* music file의 재생시, 경과시간 */
#define MSG_EMF2UI_PHOTO_ENLARGE_DISPLAYDONE		MSG_BASE_EME(0x1023)	/* Image Enlarge의 Display 완료 */
#ifdef INCLUDE_YOUTUBE
#define MSG_EMF2UI_YOUTUBE_TN_DISPLAYDONE			MSG_BASE_EME(0x100a)	/* Thumbnail Image 의 Display 완료 */
#define MSG_EME2UI_YOUTUBE_DECODING_ERROR			MSG_BASE_EME(0x100b)	/* Image file의 decoding 실패 */
#define MSG_EMF2UI_YOUTUBE_PLAYDONE					MSG_BASE_EME(0x100c)	/* Thumbnail Image 의 Display 완료 */
#define MSG_EME2UI_YOUTUBE_PLAY_ERROR				MSG_BASE_EME(0x100d)	/* Image file의 decoding 실패 */
#define MSG_EME2UI_YOUTUBE_PLAY_1X					MSG_BASE_EME(0x100e)	/* Image file의 decoding 실패 */
#define MSG_EME2UI_YOUTUBE_START_DONE				MSG_BASE_EME(0x100f)	/* Play start가 성공여부  */
#define MSG_EME2UI_YOUTUBE_PAUSE_DONE				MSG_BASE_EME(0x1010)	/* Pause start가 성공여부  */
#define MSG_EME2UI_YOUTUBE_SEEK_DONE				MSG_BASE_EME(0x1011)	/* Pause start가 성공여부  */
#endif

#define MSG_EME2UI_3DON								MSG_BASE_EME(0x1112)	/* EME 3D ON */
#define MSG_EME2UI_3DOFF							MSG_BASE_EME(0x1113)    /* EME 3D OFF*/


#ifdef INCLUDE_NEW_WMDRMPD

/*
WMDRM Error 처리 코드
*/
#define MSG_EME2UI_WMDRM_ERR_FAIL					MSG_BASE_EME(0x1100)	/* WMDRM Common error 처리 */
#define MSG_EME2UI_WMDRM_LIC_FAILLOCAL				MSG_BASE_EME(0x1101)	/* License store에 저장된 License 가 없음 */
#define MSG_EME2UI_WMDRM_LIC_FAILTRANSFER			MSG_BASE_EME(0x1102)	/* License 전송시 error & 전송된 License error*/
#define MSG_EME2UI_WMDRM_LIC_EXPIRED				MSG_BASE_EME(0x1103)	/* 저장된 License 가 만료 됨  */

#endif


#if defined (INCLUDE_YOUTUBE) || defined (INCLUDE_BROADBAND) || defined (INCLUDE_STREAM)
#define MSG_EME2UI_STREAMING_BUFF_EVENT				MSG_BASE_EME(0x1011)
#define MSG_EME2UI_STREAMING_HTTP_EVENT				MSG_BASE_EME(0x1012)	/* Image file의 decoding 실패 */
#define MSG_EME2UI_STREAMING_PAUSE					MSG_BASE_EME(0x1013)
#define MSG_EME2UI_STREAMING_RESUME					MSG_BASE_EME(0x1014)
#endif

#ifdef INCLUDE_STREAM
#define MSG_EME2UI_START_DONE						MSG_BASE_EME(0x1015)	/* EME Play Done event */
#define MSG_EME2UI_PAUSE_DONE						MSG_BASE_EME(0x1016)	/* EME Play Done event */
#define MSG_EME2UI_RESUME_DONE						MSG_BASE_EME(0x1017)	/* EME Play Done event */
#define MSG_EME2UI_PLAY_DONE						MSG_BASE_EME(0x1018)	/* EME Play Done event */
#define MSG_EME2UI_PLAY_ERROR						MSG_BASE_EME(0x1019)	/* EME Play Done event */
#define MSG_EME2UI_PLAY_EOF							MSG_BASE_EME(0x1020)	/* EME Play EOF event */
#define MSG_EME2UI_SEEK_DONE						MSG_BASE_EME(0x1021)	/* EME Seek Done event */
#define MSG_EME2UI_SEEK_FAIL						MSG_BASE_EME(0x1022)	/* EME Seek Fail event */

#ifdef INCLUDE_FLASH_BR
#define MSG_FLASHES2UI_BUFFER_LOW					MSG_BASE_EME(0x1120)	/* Flash ES EME Low buffer level */
#define MSG_FLASHES2UI_BUFFER_FULL					MSG_BASE_EME(0x1121)	/* Flash ES EME High buffer level */
#define MSG_FLASHES2UI_H264_EOF						MSG_BASE_EME(0x1122)	/* Flash ES EME H264 End of File (decoding) */
#endif
#define MSG_EME2UI_DRAGPLAY_ERROR					MSG_BASE_EME(0x1123)	/* EME Drag&Play error event */
#define MSG_EME2UI_STOP_ERROR						MSG_BASE_EME(0x1124)	/* DMR stop error */
#define MSG_EME2UI_STOP_CLIENT_ERROR				MSG_BASE_EME(0x1125)	/* DMR stop error */

#endif
#define MSG_EME2UI_PHOTO_EFFECTDONE					MSG_BASE_EME(0x1126)	/* Photo Effect Done Event*/
#define MSG_EME2UI_DLNA_ERROR_EVENT					MSG_BASE_EME(0x1127)	/* Photo Effect Done Event*/
/**
		M L M
*/
#define MSG_MLM2UI_FILE_COPY						MSG_BASE_MLM(0x1004)	/* MLM file copy event */
#define MSG_MLM2UI_FILE_MOVE						MSG_BASE_MLM(0x1005)	/* MLM file move event */
#define MSG_MLM2UI_FILE_DELETE						MSG_BASE_MLM(0x1006)	/* MLM file delete event */
#define MSG_MLM2UI_FILE_COPY_FAILED					MSG_BASE_MLM(0x1007)	/* MLM file copy failed event */
#define MSG_MLM2UI_FILE_MOVE_FAILED					MSG_BASE_MLM(0x1008)	/* MLM file move failed event */
#define MSG_MLM2UI_FILE_DELETE_FAILED				MSG_BASE_MLM(0x1009)	/* MLM file delete failed event */
#define MSG_MLM2UI_FILE_COPY_SUCCESS				MSG_BASE_MLM(0x100a)	/* MLM file copy succeeded event */
#define MSG_MLM2UI_FILE_MOVE_SUCCESS				MSG_BASE_MLM(0x100b)	/* MLM file move succeeded event */
#define MSG_MLM2UI_FILE_DELETE_SUCCESS				MSG_BASE_MLM(0x100c)	/* MLM file delete succeeded event */
#define MSG_MLM2UI_FILE_COPY_STOPPED				MSG_BASE_MLM(0x100d)	/* MLM file copy stopped event */
#define MSG_MLM2UI_FILE_MOVE_STOPPED				MSG_BASE_MLM(0x100e)	/* MLM file move stopped event */
#define MSG_MLM2UI_FILE_DELETE_STOPPED				MSG_BASE_MLM(0x100f)	/* MLM file delete stopped event */
#define MSG_MLM2UI_NOT_ENOUGH_USB_SPACE				MSG_BASE_MLM(0x1010)	/* MLM file copy not enough MC space event */
#define MSG_MLM2UI_NOT_ENOUGH_HDD_SPACE				MSG_BASE_MLM(0x1011)	/* MLM file copy not enough HDD space event */
#define MSG_EME2UI_JPEG_DECODING_SUCCESS			MSG_BASE_MLM(0x1012)	/* JPEG decoding success event */
#define MSG_MLM2UI_MC_INIT_FAILED					MSG_BASE_MLM(0x1013)	/* Memory card initializing failed */
#define MSG_MLM2UI_COPY_FAILED_BEYOND_MAX_NUM		MSG_BASE_MLM(0x1014)	/* Copy to MC failed because the # of
																		   	   copy items exceed 255*/
#define MSG_MLM2UI_CARD1_DETACHED					MSG_BASE_MLM(0x1015)	/* Memory card1 detached */
#define MSG_MLM2UI_CARD2_DETACHED					MSG_BASE_MLM(0x1016)	/* Memory card2 detached */
#define MSG_MLM2UI_PHOTO_FORMAT						MSG_BASE_MLM(0x1017)	/* Format photo drive */
#define MSG_MLM2UI_MUSIC_FORMAT						MSG_BASE_MLM(0x1018)	/* Format music drive */
#define MSG_MLM2UI_HDD_FORMAT						MSG_BASE_MLM(0x1019)	/* Format hdd */
#define MSG_MLM2UI_CARD_DETACHED					MSG_BASE_MLM(0x101a)	/* Memory card2 detached */
#define MSG_MLM2UI_FILE_NUMBER_FULL                 MSG_BASE_MLM(0x101b)    /* File Number Full */  //kjoh

// Back-up
#define MSG_MLM2UI_BACKUP_COMPLETED					MSG_BASE_MLM(0x1020)
#define MSG_MLM2UI_BACKUP_STOPPED					MSG_BASE_MLM(0x1021)
#define MSG_MLM2UI_BACKUP_SUSPENDED					MSG_BASE_MLM(0x1022)
#define MSG_MLM2UI_BACKUP_FAILED					MSG_BASE_MLM(0x1023)
#define MSG_MLM2UI_BACKUP_ALLCOMPLETED				MSG_BASE_MLM(0x1024)
#define MSG_MLM2UI_BACKUP_NOW						MSG_BASE_MLM(0x1025)
#define MSG_MLM2UI_BACKUPSTOP_DISKFULL				MSG_BASE_MLM(0x1026)
#define MSG_MLM2UI_BACKUPSTOP_MAXRECORDINGS 		MSG_BASE_MLM(0x1027)
// USB Memory
#define MSG_MLM2UI_DELETE_FAIL_WRITEPROTECT 		MSG_BASE_MLM(0x1030)
#define MSG_MLM2UI_DELETE_FAIL_NOFILE 				MSG_BASE_MLM(0x1031)
#define MSG_MLM2UI_DELETE_FAIL_DEVICEBROKEN 		MSG_BASE_MLM(0x1032)


// UI2MLM Message Driven                    // Added by minsik.kim at 20110126
// ASYNC_MLM
#define MSG_UI2MLM_GETFOLDERLIST                    MSG_BASE_MLM(0x1040)
#define MSG_MLM2UI_GETFOLDERLIST                    MSG_BASE_MLM(0x1041)
#define MSG_UI2MLM_GETMEDIALIST                     MSG_BASE_MLM(0x1042)
#define MSG_MLM2UI_GETMEDIALIST                     MSG_BASE_MLM(0x1043)
#define MSG_UI2MLM_GETFIRSTMEDIA                    MSG_BASE_MLM(0x1044)
#define MSG_MLM2UI_GETFIRSTMEDIA                    MSG_BASE_MLM(0x1045)

#define MSG_UI2MLM_GETLASTMEDIA                     MSG_BASE_MLM(0x1046)
#define MSG_MLM2UI_GETLASTMEDIA                     MSG_BASE_MLM(0x1047)
#define MSG_UI2MLM_GETFOLDERLISTLASTPATH            MSG_BASE_MLM(0x1048)
#define MSG_MLM2UI_GETFOLDERLISTLASTPATH            MSG_BASE_MLM(0x1049)
#define MSG_UI2MLM_GETFOLDERINFO                    MSG_BASE_MLM(0x104a)
#define MSG_MLM2UI_GETFOLDERINFO                    MSG_BASE_MLM(0x104b)
#define MSG_UI2MLM_CANCELCMD                        MSG_BASE_MLM(0x104c)
#define MSG_MLM2UI_CANCELCMD                        MSG_BASE_MLM(0x104d)

/**
		O C
 */
	/* to OC */
#define MSG_OC2OC_TIMER_ON		 					MSG_BASE_OC(0x1001)

/* (dcache) to OC */
#define MSG_SI2OC_DSC_ARRIVE						MSG_BASE_OC(0x1002)
#define MSG_SI2OC_DSM_MSG_ARRIVE					MSG_BASE_OC(0x1003)
#define MSG_SI2OC_MODULE_COMPLET					MSG_BASE_OC(0x1004)
#define MSG_SI2OC_STR_DSC_ARRIVE					MSG_BASE_OC(0x1005)
#define MSG_SI2OC_NPT_DSC_ARRIVE					MSG_BASE_OC(0x1006)

/* to SI*/
#define MSG_OC2SI_REGISTER_MODULE					MSG_BASE_OC(0x1007)
#define MSG_OC2SI_REGISTER_PID						MSG_BASE_OC(0x1008)

/* MHEG Related Messages */
#define MSG_OC2ME_STREAM_EVENT_NOTIFY				MSG_BASE_OC(0x1009)
#define MSG_OC2ME_CHANNEL_CHANGE					MSG_BASE_OC(0x100A)
#define MSG_OC2ME_SERVICE_READY						MSG_BASE_OC(0x100B)
#define MSG_OC2ME_FILE_OPEN_ACK						MSG_BASE_OC(0x100C)
#define MSG_ME2OC_ME_IDLE							MSG_BASE_OC(0x100D)
#define MSG_ME2OC_FILE_OPEN_REQ						MSG_BASE_OC(0x100E)
#define MSG_ME2OC_FILE_CLOSE_REQ					MSG_BASE_OC(0x100F)

/**
 * For Satellite SkyD
*/
/* to SI Main Task */
#define MSG_OC2SM_NOTIFY_SKYSL_READY				MSG_BASE_OC(0x1010)
#define MSG_OC2SM_SKYSL_DONE						MSG_BASE_OC(0x1011) 		/* SKYD */

/**
		M H E G
 */
/* to UI */
#define MSG_ME2UI_CHANNEL_CHANGE					MSG_BASE_MHEG(0x1001)
#define MSG_ME2UI_STATE_ON       					MSG_BASE_MHEG(0x1002)
#define MSG_ME2UI_STATE_OFF       					MSG_BASE_MHEG(0x1003)
#define MSG_CM2ME_GOOD_VIDEO       					MSG_BASE_MHEG(0x1004)
#define MSG_ME2UI_ACLIP_PLAY       					MSG_BASE_MHEG(0x1005)
#define MSG_ME2UI_ACLIP_DONE       					MSG_BASE_MHEG(0x1006)
#ifdef INCLUDE_CIPLUS
#define MSG_ME2UI_SETDEF_VIDEO						MSG_BASE_MHEG(0x1007)
#endif

/* to CM */
#define MSG_ME2CM_ICS_START							MSG_BASE_MHEG(0x1008)
#define MSG_ME2CM_ICS_END							MSG_BASE_MHEG(0x1009)

/**
		T E L E T E X T
 */
/* to UI */
#define MSG_TTX2UI_OSD0_OFF							MSG_BASE_TTX(0x1001)
#define MSG_TTX2UI_DETECT_WRAPAROUND				MSG_BASE_TTX(0x1002)

/**
		O A D
 		1. 이런식으로 정의한 것은 OAD와 관련 되었지만 SI가 메세지를 보낸다는 뜻이다.
 		2. MSG의 BASE가 OAD인 것은 UI에서 OAD관련 메세지를 통합적으로 처리하기 위해서 있다. by injepost
 */
/* to UI (OAD) */
#define MSG_SI2UI_OAD_DSI_ARRIVE					MSG_BASE_OAD(0x1000)
#define MSG_SI2UI_OAD_LINKAGE_EXIST					MSG_BASE_OAD(0x1001)
#define MSG_OC2UI_OAD_SERVICE_READY					MSG_BASE_OAD(0x1200)
#define MSG_OC2UI_OAD_FILE_OPEN_ACK					MSG_BASE_OAD(0x1201)
#define MSG_UI2OC_OAD_FILE_OPEN_REQ					MSG_BASE_OAD(0x1202)
#define MSG_UI2UI_OAD_CHECK_CHUCKSUM				MSG_BASE_OAD(0x1203)

#define MSG_SWU2UI_OAD_UPDATE_START					MSG_BASE_OAD(0x1204)
#define MSG_SWU2UI_OAD_UPDATE_DONE					MSG_BASE_OAD(0x1205)
#define MSG_SWU2UI_OAD_UPDATE_ERROR 				MSG_BASE_OAD(0x1206)

/**
		S S U
*/
#define MSG_SSU2UI_SIGNAL_DETECTED					MSG_BASE_SSU(0x0000)
#define MSG_SSU2UI_PROGRESS_INFO						MSG_BASE_SSU(0x0001)
#define MSG_SSU2UI_ERROR_INFO 				        MSG_BASE_SSU(0x0002)
#define MSG_SSU2UI_MUX_CHANGE_REQUEST		        MSG_BASE_SSU(0x0003)
#define MSG_SSU2UI_MUX_SCAN_REQUEST		            MSG_BASE_SSU(0x0004)
#define MSG_SSU2UI_MUX_SCAN_RESULT		            MSG_BASE_SSU(0x0005)
#define MSG_SSU2UI_MEM_FREE		            		MSG_BASE_SSU(0x0006)
#define MSG_SSU2UI_APPLY		            			MSG_BASE_SSU(0x0007)
#define MSG_SSU2UI_REBOOT		            			MSG_BASE_SSU(0x0008)
#define MSG_SSU2UI_ACK_USB	            			MSG_BASE_SSU(0x0009)
#define MSG_SSU2UI_SCAN_START            			MSG_BASE_SSU(0x000A)
#define MSG_SSU2UI_SCAN_NEXT            				MSG_BASE_SSU(0x000B)
#define MSG_SSU2UI_SCAN_END            				MSG_BASE_SSU(0x000C)
#define MSG_SSU2UI_SCAN_PROGRESS            			MSG_BASE_SSU(0x000D)

//#if (SYS_ATSC)
/* psip에서 ui로 sender 했지만 MSG를 ui->ssu처리하므로 ssu base로 설정함 */
#define MSG_PSIP2UI_SSU_EIT_CLEAR_DONE				MSG_BASE_SSU(0x000a) /* notify "EPG DB clear done" to ui */
//#else
/* si에서 ui로 sender 했지만 MSG를 ui->ssu처리하므로 ssu base로 설정함  */
#define MSG_SI2UI_SSU_EIT_CLEAR_DONE				MSG_BASE_SSU(0x000b) /* notify "EPG DB clear done" to ui */
//#endif

/* For ACAP moa enabled */
/* ui에서 ui로 sender 했지만 MSG를 ui->ssu처리하므로 ssu base로 설정함  */
#define MSG_UI2UI_SSU_ACAP_MOA_ENABLED				MSG_BASE_SSU(0x000c)

#define MSG_SSU2UI_STOP								MSG_BASE_SSU(0x000d)
#define MSG_SSU2UI_RESUME							MSG_BASE_SSU(0x000e)
#define MSG_SSU2UI_CHECK_DVBOUI						MSG_BASE_SSU(0x000f)
#define MSG_SSU2UI_FOUND_DVBOUI						MSG_BASE_SSU(0x0010)


/**
		S W U
*/
/* to UI for USB Upgrade */
#define MSG_SWU2UI_USB_UPDATE_START					MSG_BASE_SWU(0x1001)
#define MSG_SWU2UI_USB_UPDATE_DONE					MSG_BASE_SWU(0x1002)
#define MSG_SWU2UI_USB_UPDATE_ERROR					MSG_BASE_SWU(0x1003)

/**
		S D M
*/
/* to UI for SDM */
#ifdef INCLUDE_SDM_NEW
#define MSG_SDM2UI_USB_HUB_LEV_EXCESS				MSG_BASE_SDM(0x1001)
#define MSG_SDM2UI_NOT_MASS_STORAGE					MSG_BASE_SDM(0x1002)
#define MSG_SDM2UI_NEW_USB_DETECT					MSG_BASE_SDM(0x1003)
#define MSG_SDM2UI_USB_REMOVED						MSG_BASE_SDM(0x1004)
#define MSG_SDM2UI_USB_NOT_REGISTERED				MSG_BASE_SDM(0x1005)
#define MSG_SDM2UI_OVER_MAX_STORAGE					MSG_BASE_SDM(0x1006)
#define MSG_SDM2UI_OVER_MAX_DEVICE					MSG_BASE_SDM(0x1007)
#define	MSG_SDM2UI_USB_INFO_COMPLETED				MSG_BASE_SDM(0x1008)
#define	MSG_SDM2UI_ACCESS_USB_DETECT				MSG_BASE_SDM(0x1009)
#else  // INCLUDE_SDM_NEW
#define MSG_SDM2UI_USB_HUB_DETECT					MSG_BASE_SDM(0x1001)
#define MSG_SDM2UI_NOT_MASS_STORAGE					MSG_BASE_SDM(0x1002)
#define MSG_SDM2UI_NEW_USB_DETECT					MSG_BASE_SDM(0x1003)
#define MSG_SDM2UI_USB_REMOVED						MSG_BASE_SDM(0x1004)
#define MSG_SDM2UI_USB_NOT_REGISTERED				MSG_BASE_SDM(0x1005)
#define MSG_SDM2UI_USB_WIRELESS_DONGLE				MSG_BASE_SDM(0x1006)
#define MSG_SDM2UI_USB_CAM_MICROPHONE				MSG_BASE_SDM(0x1007)
#define	MSG_SDM2UI_USB_CAMERA						MSG_BASE_SDM(0x1008)
#define	MSG_SDM2UI_INIT_DONE						MSG_BASE_SDM(0x1009)
#define	MSG_SDM2UI_USB_INFO_COMPLETED				MSG_BASE_SDM(0x100A)
#define	MSG_SDM2UI_ACCESS_USB_DETECT				MSG_BASE_SDM(0x100B)

#ifdef INCLUDE_MOUSE_DONGLE
#define MSG_SDM2UI_USB_MOUSE_DONGLE					MSG_BASE_SDM(0x1011)
#endif

/* to UI for 3G*/
#ifdef INCLUDE_3G
#define MSG_NC2UI_USB_NETCARD_DONGLE				MSG_BASE_NM(0x1020)
#endif
#endif

#ifdef INCLUDE_MCB
#define MSG_MCB2UI_THUMBNAIL_UPDATE				MSG_BASE_MCB(0x1001)
#endif

#ifdef INCLUDE_HOTEL_MODE
#define MSG_TLL2UI_DOWNLOAD_START				MSG_BASE_TLL(0x1001)
#define MSG_TLL2UI_DOWNLOAD_PROGRESS			MSG_BASE_TLL(0x1002)
#define MSG_TLL2UI_DOWNLOAD_COMPLETE			MSG_BASE_TLL(0x1003)
#define MSG_TLL2UI_DOWNLOAD_FAIL				MSG_BASE_TLL(0x1004)
#define MSG_TLL2UI_UPLOAD_START					MSG_BASE_TLL(0x2001)
#define MSG_TLL2UI_UPLOAD_PROGRESS				MSG_BASE_TLL(0x2002)
#define MSG_TLL2UI_UPLOAD_COMPLETE				MSG_BASE_TLL(0x2003)
#define MSG_TLL2UI_UPLOAD_FAIL					MSG_BASE_TLL(0x2004)
#define MSG_TLL2UI_PROCESS_CHANNELDATA			MSG_BASE_TLL(0x3001)
#endif

#ifdef INCLUDE_BT
/* to UI for BT */
#define MSG_BT2UI_GAP_ENABLED					MSG_BASE_BT(0x2001)
#define MSG_BT2UI_GAP_DISABLED					MSG_BASE_BT(0x2002)
#define MSG_BT2UI_GAP_SET_PINCODE				MSG_BASE_BT(0x2003)
#define MSG_BT2UI_GAP_SEARCH_RESULT				MSG_BASE_BT(0x2004)
#define MSG_BT2UI_GAP_SEARCH_COMPLETED			MSG_BASE_BT(0x2005)
#define MSG_BT2UI_GAP_SEARCH_CANCELED			MSG_BASE_BT(0x2006)
#define MSG_BT2UI_GAP_AUTH_COMPLETED			MSG_BASE_BT(0x2007)
#define MSG_BT2UI_GAP_REGISTER_COMPLETED		MSG_BASE_BT(0x2008)
#define MSG_BT2UI_GAP_SEARCH_CFM				MSG_BASE_BT(0x2009)
#define MSG_BT2UI_GAP_CONNECTED					MSG_BASE_BT(0x200A)
#define MSG_BT2UI_GAP_DISCONNECTED				MSG_BASE_BT(0x200B)
#define MSG_BT2UI_GAP_DEVICE_DELETED			MSG_BASE_BT(0x200C)
#define MSG_BT2UI_GAP_SET_NAME_COMPLETED		MSG_BASE_BT(0x200D)
#define MSG_BT2UI_GAP_REJ_INC_CONNECTION		MSG_BASE_BT(0x200E)
#define MSG_BT2UI_GAP_SERVICE_DISCOVERED		MSG_BASE_BT(0x200F)
#define MSG_BT2UI_GAP_INCOMING_CONNECTED		MSG_BASE_BT(0x2010)
#define MSG_BT2UI_GAP_SERVER_ENABLED			MSG_BASE_BT(0x2011)
#define MSG_BT2UI_GAP_SERVER_DISABLED			MSG_BASE_BT(0x2012)
#define MSG_BT2UI_GAP_DB_FULL					MSG_BASE_BT(0x2013)

#define MSG_BT2UI_OPS_PUT_AUTH					MSG_BASE_BT(0x2030)
#define MSG_BT2UI_OPS_PUT_METHOD				MSG_BASE_BT(0x2031)
#define MSG_BT2UI_OPS_PROGRESS					MSG_BASE_BT(0x2032)
#define MSG_BT2UI_OPS_PUT_COMPLETED				MSG_BASE_BT(0x2033)
#define MSG_BT2UI_OPS_PUT_ABORTED				MSG_BASE_BT(0x2034)
#define MSG_BT2UI_OPS_DISCONNECTED				MSG_BASE_BT(0x2035)
#define MSG_BT2UI_OPS_CANCELLED					MSG_BASE_BT(0x2036)
#define MSG_BT2UI_OPS_SPACE_FULL				MSG_BASE_BT(0x2037)
#define MSG_BT2UI_OPS_VCARD_MSG					MSG_BASE_BT(0x2038)

#define MSG_BT2UI_A2SRC_CONNECT_REQUESTED		MSG_BASE_BT(0x2050)
#define MSG_BT2UI_A2SRC_START_STREAMING			MSG_BASE_BT(0x2051)
#define MSG_BT2UI_A2SRC_PAUSE_STREAMING			MSG_BASE_BT(0x2052)
#define MSG_BT2UI_A2SRC_STOP_STREAMING			MSG_BASE_BT(0x2053)
#define MSG_BT2UI_A2SRC_RESUME_STREAMING		MSG_BASE_BT(0x2054)

#define MSG_BT2UI_A2SNK_CONNECT_REQUESTED		MSG_BASE_BT(0x2070)
#define MSG_BT2UI_A2SNK_START_STREAMING			MSG_BASE_BT(0x2071)
#define MSG_BT2UI_A2SNK_PAUSE_STREAMING			MSG_BASE_BT(0x2072)
#define MSG_BT2UI_A2SNK_STOP_STREAMING			MSG_BASE_BT(0x2073)
#define MSG_BT2UI_A2SNK_RESUME_STREAMING		MSG_BASE_BT(0x2074)

#define MSG_BT2UI_HSP_SCO_OPENED				MSG_BASE_BT(0x2060)
#define MSG_BT2UI_HSP_SCO_CLOSED				MSG_BASE_BT(0x2061)

#define MSG_BT2UI_TEST_ENTER_DUT				MSG_BASE_BT(0x2100)
#define MSG_BT2UI_TEST_EXIT_DUT					MSG_BASE_BT(0x2101)
#define MSG_BT2UI_TEST_CHANGE_BD_ADDR			MSG_BASE_BT(0x2102)
#endif

#ifdef INCLUDE_CIFS
#define MSG_CIFS2UI_FOUND_SHARE					MSG_BASE_CIFS(0x1001)
#define MSG_CIFS2UI_SEARCH_DONE					MSG_BASE_CIFS(0x1002)
#define MSG_CIFS2UI_SEARCH_FAIL					MSG_BASE_CIFS(0x1003)
#define MSG_CIFS2UI_MOUNT_DONE					MSG_BASE_CIFS(0x1004)
#define MSG_CIFS2UI_UNMOUNT_DONE				MSG_BASE_CIFS(0x1005)
#define MSG_CIFS2UI_MOUNT_FAIL					MSG_BASE_CIFS(0x1006)
#define MSG_CIFS2UI_UNMOUNT_FAIL				MSG_BASE_CIFS(0x1007)
#endif

#ifdef INCLUDE_NSU
#define MSG_NSU2UI_FOUND_NEW_IMAGE				MSG_BASE_NSU(0x1001)
#define MSG_NSU2UI_NOT_FOUND_NEW_IMAGE			MSG_BASE_NSU(0x1002)
#define MSG_NSU2UI_ERROR_INFO 				    MSG_BASE_NSU(0x1003)
#define MSG_NSU2UI_PROGRESS_INFO					MSG_BASE_NSU(0x1004)
#define MSG_NSU2UI_APPLY							MSG_BASE_NSU(0x1005)
#define MSG_NSU2UI_REBOOT							MSG_BASE_NSU(0x1006)
#define MSG_NSU2UI_ACK_USB						MSG_BASE_NSU(0x1007)
#endif

#ifdef INCLUDE_NETFLIX_BUILTIN
#define MSG_NETFLIX2UI_END_APP					MSG_BASE_NETFLIX(0x1001)
#define MSG_NETFLIX2UI_OPEN_NETWORK_MENU		MSG_BASE_NETFLIX(0x1002)
#define MSG_NETFLIX2UI_OPEN_NSU_MENU			MSG_BASE_NETFLIX(0x1003)
#define MSG_NETFLIX2UI_START_APP_ERROR			MSG_BASE_NETFLIX(0x1004)
#define MSG_NETFLIX2UI_ASF_GOOD_VIDEO			MSG_BASE_NETFLIX(0X1010)
#endif

#ifdef INCLUDE_DLNA
#define MSG_DLNA2UI_DEVICE_ADD					MSG_BASE_DLNA(0x1001)
#define MSG_DLNA2UI_DEVICE_DEL					MSG_BASE_DLNA(0x1002)
#define MSG_DLNA_2UI_CMD						MSG_BASE_DLNA(0x1003)
#define MSG_DMR_2UI_CMD							MSG_BASE_DLNA(0x1004)
#endif

#ifdef INCLUDE_WIRELESS_READY
#define MSG_WCM2UI_DONGLE_DETECTED				MSG_BASE_WCM(0x1001)
#define MSG_WCM2UI_DONGLE_REMOVED				MSG_BASE_WCM(0x1002)
#define MSG_WCM2UI_CONNECTION_SUCCESS			MSG_BASE_WCM(0x1003)
#define MSG_WCM2UI_CONNECTION_FAIL				MSG_BASE_WCM(0x1004)
#define MSG_WCM2UI_CONNECTION_DISCONNECTED		MSG_BASE_WCM(0x1005)
#define	MSG_WCM2UI_PC_ADJUST_INFO				MSG_BASE_WCM(0x1006)
#define	MSG_WCM2UI_FRONT_KEY_INPUT				MSG_BASE_WCM(0x1007)
#define MSG_WCM2UI_PC_RESOLUTION				MSG_BASE_WCM(0x1008)
#define MSG_WCM2UI_AUTODETECT_RESET				MSG_BASE_WCM(0x1009)
#define MSG_WCM2UI_MEDIABOX_AUTO_OFF			MSG_BASE_WCM(0x100a)
#define MSG_WCM2UI_INPUT_CAPABILITY				MSG_BASE_WCM(0x100b)
#define MSG_WCM2UI_TTX_MSG						MSG_BASE_WCM(0x100c)
#define MSG_WCM2UI_MANUFACTURE_MSG				MSG_BASE_WCM(0x100d)
#define MSG_WCM2UI_WCM_RESYNC					MSG_BASE_WCM(0x100e)
#define MSG_WCM2UI_WCM_MEDIABOX_SWU				MSG_BASE_WCM(0x100f)
#define MSG_WCM2UI_SET_AVCONFIG					MSG_BASE_WCM(0x1010)
#define MSG_WCM2UI_NG_MEDIABOX_TYPE				MSG_BASE_WCM(0x1011)
#define MSG_WCM2UI_VIDEO_CONNECTION_STATUS		MSG_BASE_WCM(0x1012)
#define MSG_WCM2UI_RATING_INFO_CHANGE			MSG_BASE_WCM(0x1013)
#define MSG_WCM2UI_DONGLE_REMOVED_CHECK			MSG_BASE_WCM(0x1014)
#endif

#ifdef INCLUDE_ADDON
#define MSG_ADDONHOST2UI_SHOW_BROWSER			MSG_BASE_ADDONHOST(0x1001)
#define MSG_ADDONHOST2UI_SHOW_NETCONFIG			MSG_BASE_ADDONHOST(0x1002)
#define MSG_ADDONHOST2UI_UPDATE_SW				MSG_BASE_ADDONHOST(0x1003)
#define MSG_ADDONHOST2UI_SHOW_GAMEMENU			MSG_BASE_ADDONHOST(0x1004)
#define MSG_ADDONHOST2UI_SHOW_QUICKMENU			MSG_BASE_ADDONHOST(0x1005)
#define MSG_ADDONHOST2UI_RUN_PRECACHE			MSG_BASE_ADDONHOST(0x1006)
#define MSG_ADDONHOST2UI_RUN_FLUSHCACHE			MSG_BASE_ADDONHOST(0x1007)
#define MSG_ADDONHOST2UI_SHOW_ASPECTRATIO		MSG_BASE_ADDONHOST(0x1008)
#define MSG_ADDONHOST2UI_SHOW_NETCONFIG_NETCAST		MSG_BASE_ADDONHOST(0x1009)
#define MSG_ADDONHOST2UI_SHOW_NETWORKSETTING	MSG_BASE_ADDONHOST(0x100A)
#define MSG_ADDONHOST2UI_APPSTORE_FLASH_FORMAT_ALL_DONE	MSG_BASE_ADDONHOST(0x100B)//ieeum.test
#define MSG_ADDONHOST2UI_SHOW_MOTIONRC_CONTROLPAD	MSG_BASE_ADDONHOST(0x100C)
#define MSG_ADDONHOST2UI_CREATE_MUTEOSD			MSG_BASE_ADDONHOST(0x100D)
#define MSG_ADDONHOST2UI_DESTROY_MSGBOX			MSG_BASE_ADDONHOST(0x100E)
#define MSG_ADDONHOST2UI_CREATE_MSGBOX			MSG_BASE_ADDONHOST(0x100F)


#ifdef INCLUDE_1Q
#define MSG_ADDONHOST2UI_SHOW_3DMENU            MSG_BASE_ADDONHOST(0x100E)
#endif
#define MSG_ADDONHOST2UI_RUNMODE                MSG_BASE_ADDONHOST(0x100F)
#define MSG_ADDONHOST2UI_BROWSER_MSG			MSG_BASE_ADDONHOST(0x1012)
#endif

#ifdef INCLUDE_MHP
#define MSG_MHP2UI_SELECT_SERVICE				MSG_BASE_MHP(0x0001)
#define MSG_MHP2UI_PLAY_AV						MSG_BASE_MHP(0x0002)
#define MSG_MHP2UI_CHANNEL_TUNE					MSG_BASE_MHP(0x0003)
#define MSG_MHP2UI_COMP_CHANGE					MSG_BASE_MHP(0x0004)
#define MSG_MHP2UI_COMP_STOP					MSG_BASE_MHP(0x0005)
#define MSG_MHP2MHP_RETRIEVE						MSG_BASE_MHP(0x0006)
#define MSG_MHP_SECTION_ACQUIRED					MSG_BASE_MHP(0x0007)
#define MSG_MHP2UI_MOA_STATUS_CHANGED			MSG_BASE_MHP(0x0008)
#define MSG_MHP2UI_TV_NOTIFY_KEY					MSG_BASE_MHP(0x0009)
#define MSG_MHP2UI_IPM_COMPLETE					MSG_BASE_MHP(0x000A)
#define MSG_MHP2UI_IPM_ERROR						MSG_BASE_MHP(0x000B)
#define MSG_MHP2UI_SERVICE_CHANGE				MSG_BASE_MHP(0x000C)
#define MSG_MHP2UI_RESIZE_VIDEO					MSG_BASE_MHP(0x000D)
#define MSG_MHP2UI_UPDATE_MHPAPPLIST			MSG_BASE_MHP(0x000E)
#define MSG_MHP2UI_APP_STATE_CHANGED			MSG_BASE_MHP(0x000F)
#ifdef ICS_SUBTITLE	//_Subtitle_//_kwangmin1108.lee_//
#define MSG_MHP2UI_ICS_SET_SUBT					MSG_BASE_MHP(0x0010)
#define MSG_MHP2UI_ICS_SET_SUBT_LANG			MSG_BASE_MHP(0x0011)
#define MSG_MHP2UI_ICS_SET_SUBT_ONOFF			MSG_BASE_MHP(0x0012)
#endif
#define MSG_MHP2UI_SET_AUDIO_LANG			MSG_BASE_MHP(0x0020)
//_Subtitle_//_~kwangmin1108.lee_//
#define MSG_MHP2UI_BB_PLAY_START				MSG_BASE_MHP(0x0030)
#define MSG_MHP2UI_BB_PLAY_STOP					MSG_BASE_MHP(0x0031)
#define MSG_MHP2UI_BB_NETWORK_DISABLE			MSG_BASE_MHP(0x0032)

#endif

#ifdef INCLUDE_BML
/*
	Message from BML Peer to BML EM
*/
#define MSG_BML_START_BROWSER					MSG_BASE_BML(0x0001)
#define MSG_BML_STOP_BROWSER					MSG_BASE_BML(0x0002)
#define MSG_BML_PMTRECEIVED						MSG_BASE_BML(0x0003)
#define MSG_BML_PMTCHANGED						MSG_BASE_BML(0x0004)
#define MSG_BML_CCSTATUSCHANGED					MSG_BASE_BML(0x0005)			//Caption On/Off/change language
#define MSG_BML_COMPONENTCHANGED				MSG_BASE_BML(0x0006)
#define MSG_BML_AUDIOCHANGED					MSG_BASE_BML(0x0007)
#define MSG_BML_AUDIOSTOP						MSG_BASE_BML(0x0008)
#define MSG_BML_HALTBMLBROWSER					MSG_BASE_BML(0x0009)
/*
	Message from BML to UI
*/
#define MSG_BML2UI_EPGSTATUS					MSG_BASE_BML(0x0020)
#define MSG_BML2UI_VCOMPSTOP					MSG_BASE_BML(0x0021)		//VideoStream_UpdatePeer status = DTV_NO
#define MSG_BML2UI_VCOMPCHANGE					MSG_BASE_BML(0x0022)		//VideoStream_UpdatePeer status = DTV_YES, Component has been changed
#define MSG_BML2UI_RESIZEVIDEO					MSG_BASE_BML(0x0023)		//VideoStream_UpdatePeer status = DTV_YES, Video parameters has been changed
#define MSG_BML2UI_ACOMPCHANGE					MSG_BASE_BML(0x0024)		//VideoStream_UpdatePeer status = DTV_YES, Component has been changed
#define MSG_BML2UI_ACOMPSTOP					MSG_BASE_BML(0x0025)		//AudioStream_UpdatePeer status = DTV_NO
#define MSG_BML2UI_EPGRESERVE					MSG_BASE_BML(0x0026)		// to send msg from BML to UI to Reserve REMIND or RECORD & update status to BML
#define MSG_BML2UI_EPGCANCELRESV				MSG_BASE_BML(0x0027)		// to sned msg from BML to UI to Cancel REMIND or RECORD & Update status to BML
#endif

#ifdef INCLUDE_BCAS

#define MSG_BCAS2CM_CARD_INSERTED				MSG_BASE_BCAS(0x0001)
#define MSG_BCAS2CM_CARD_EXTRACTED				MSG_BASE_BCAS(0x0002)
#define MSG_BCAS2CM_CARD_INVALID				MSG_BASE_BCAS(0x0003)
#define MSG_BCAS2CM_AV_CONFIGURE				MSG_BASE_BCAS(0x0004)
#define MSG_BCAS2CM_AV_RESET					MSG_BASE_BCAS(0x0005)
#define MSG_BCAS2CM_AV_CANNOT_VIEW_PAY_PGM		MSG_BASE_BCAS(0x0006)
#define MSG_BCAS2CM_AV_CAN_VIEW_PAY_PGM			MSG_BASE_BCAS(0x0007)
#define MSG_BCAS2CM_EMM_AUTOMATIC_MESSAGE		MSG_BASE_BCAS(0x0008)
#define MSG_BCAS2CM_POC_INFO_AVAILABLE			MSG_BASE_BCAS(0x0009)


#endif /* INCLUDE_BCAS */
#ifdef INCLUDE_VUDU_NATIVE
#define MSG_VUDU2UI_END_APP						MSG_BASE_VUDU(0x1001)
#define MSG_VUDU2UI_GOOD_VIDEO					MSG_BASE_VUDU(0X1002)
#define	MSG_VUDU2UI_GO_TO_NETWORK_CFG			MSG_BASE_VUDU(0x1003)
#endif

#ifdef INCLUDE_MOUSE
#define MSG_GAME2UI_READY_TO_SWING				MSG_BASE_GAME(0x1001)
#define MSG_GAME2UI_SET_SPACE_SENSING			MSG_BASE_GAME(0x1002)
#define MSG_GAME2UI_ENTER_TITLE					MSG_BASE_GAME(0x1003)
#define MSG_GAME2UI_READY_TO_JUMP				MSG_BASE_GAME(0x1004)
#define MSG_GAME2UI_READY_TO_THROW				MSG_BASE_GAME(0x1005)
#define MSG_GAME2UI_READY_TO_GESTURE			MSG_BASE_GAME(0x1006)
#define MSG_GAME2UI_END_SWING					MSG_BASE_GAME(0x1007)
#endif

#ifdef INCLUDE_SDP
#define	MSG_SDP2SDP_NETWORK_READY				MSG_BASE_SDP(0x0000)
#define MSG_SDP2SDP_CHECK_NETCAST_VERSION		MSG_BASE_SDP(0x0001)
#define MSG_SDP2SDP_CHECK_MAINPAGE				MSG_BASE_SDP(0x0002)
#define MSG_SDP2SDP_CHECK_HOME_CARDS			MSG_BASE_SDP(0x0003)
#define MSG_SDP2SDP_CHANGE_SERVER				MSG_BASE_SDP(0x0004)
#define	MSG_SDP2SDP_NETWORK_DISCONNECT			MSG_BASE_SDP(0x0005)
#define MSG_SDP2SDP_REQUEST_DNLD				MSG_BASE_SDP(0x1001)
#define MSG_SDP2SDP_REQUEST_NETCAST_PKGS		MSG_BASE_SDP(0x1002)
#define MSG_SDP2SDP_REQUEST_MAINPAGE			MSG_BASE_SDP(0x1003)
#define MSG_SDP2SDP_REQUEST_HOME_CARDS			MSG_BASE_SDP(0x1004)
#define MSG_SDP2SDP_REQUEST_CHECK_USERID		MSG_BASE_SDP(0x1005)
#define MSG_SDP2SDP_REQUEST_REGISTER_USER		MSG_BASE_SDP(0x1006)
#define MSG_SDP2SDP_REQUEST_LOGIN				MSG_BASE_SDP(0x1007)
#define MSG_SDP2SDP_REQUEST_LOGOUT				MSG_BASE_SDP(0x1008)
#define MSG_SDP2SDP_REQUEST_USER_LIST			MSG_BASE_SDP(0x1009)
#define MSG_SDP2SDP_REQUEST_GET_USER_INFO		MSG_BASE_SDP(0x100A)
#define MSG_SDP2SDP_REQUEST_DELETE_DEVICE		MSG_BASE_SDP(0x100B)
#define MSG_SDP2SDP_REQUEST_DEACTIVATE_USER		MSG_BASE_SDP(0x100C)
#define MSG_SDP2SDP_REQUEST_DEVICE_AUTH			MSG_BASE_SDP(0x100D)
#define MSG_SDP2SDP_REQUEST_CANCEL_DEVICE_AUTH	MSG_BASE_SDP(0x100E)
#define MSG_SDP2SDP_REQUEST_AUTO_LOGIN			MSG_BASE_SDP(0x100F)
#define MSG_SDP2SDP_REQUEST_CHECK_USER			MSG_BASE_SDP(0x1010)
#define MSG_SDP2SDP_REQUEST_USER_SESSION_EXT	MSG_BASE_SDP(0x1011)
#define MSG_SDP2SDP_REQUEST_PURCHASE			MSG_BASE_SDP(0x1012)
#define MSG_SDP2SDP_REQUEST_CHECK_TERMS			MSG_BASE_SDP(0x1013)
#define MSG_SDP2SDP_REQUEST_AGREE_TERMS			MSG_BASE_SDP(0x1014)
#define MSG_SDP2SDP_REQUEST_REGISTER_USER_EX	MSG_BASE_SDP(0x1015)
#define MSG_SDP2SDP_REQUEST_LOGIN_EX			MSG_BASE_SDP(0x1016)
#define MSG_SDP2SDP_REQUEST_AUTO_LOGIN_EX		MSG_BASE_SDP(0x1017)
#define MSG_SDP2SDP_REQUEST_NETCAST_RESET		MSG_BASE_SDP(0x1018)
#define MSG_SDP2SDP_REQUEST_RECOMM				MSG_BASE_SDP(0x1019)
#define MSG_SDP2SDP_REQUEST_REFRESH_APP_ICONS	MSG_BASE_SDP(0x101A)
// SNS MESSAGE
#define MSG_SDP2SDP_REQUEST_LGID_SNSID_INFO		MSG_BASE_SDP(0x1100)
#define MSG_SDP2SDP_REQUEST_SNSID_AUTO_LOGIN_STATUS	MSG_BASE_SDP(0x1101)
#define MSG_SDP2SDP_REQUEST_CHECK_SNSID_EXIST		MSG_BASE_SDP(0x1102)
#define MSG_SDP2SDP_REQUEST_REGISTER_USER_SNSID		MSG_BASE_SDP(0x1103)
#define MSG_SDP2SDP_REQUEST_DEACTIVATE_USER_SNSID	MSG_BASE_SDP(0x1104)

#define MSG_SDP2SDP_DNLD_DONE					MSG_BASE_SDP(0x2001)
#define MSG_SDP2SDP_DNLD_ERR					MSG_BASE_SDP(0x2002)
#define	MSG_SDP2SDP_SYNC_SERVER					MSG_BASE_SDP(0x3000)
#define	MSG_SDP2SDP_NOTIFY_CHANGE_COUNTRY		MSG_BASE_SDP(0x3001)
#define	MSG_SDP2SDP_NOTIFY_CHANGE_LANG			MSG_BASE_SDP(0x3002)
#define	MSG_SDP2SDP_NOTIFY_SELECT_RECOMM_ITEM	MSG_BASE_SDP(0x3003)
#define	MSG_SDP2SDP_REQUEST_CANDIDATE_COUNTRY	MSG_BASE_SDP(0x4000)
#define	MSG_SDP2UI_RESPONSE_COUNTRY				MSG_BASE_SDP(0x5000)
#define MSG_SDP2UI_UPDATE_HOME_CARDS			MSG_BASE_SDP(0x5001)
#define MSG_SDP2UI_UPDATE_PROGRESS				MSG_BASE_SDP(0x5002)
#define MSG_SDP2UI_UPDATE_DONE					MSG_BASE_SDP(0x5003)
#define MSG_SDP2UI_UPDATE_ERR					MSG_BASE_SDP(0x5004)
#define MSG_SDP2UI_NOTIFY_COUNTRY				MSG_BASE_SDP(0x5005)
#define MSG_SDP2UI_UPDATE_OPT					MSG_BASE_SDP(0x5006)
#define MSG_SDP2UI_RESP_DEVICE_AUTH				MSG_BASE_SDP(0x5007)
#define MSG_SDP2UI_NOTIFY_READY					MSG_BASE_SDP(0x5008)
#define	MSG_SDP2UI_NOTIFY_LOGIN					MSG_BASE_SDP(0x5009)
#define	MSG_SDP2UI_NOTIFY_CHECK_USER			MSG_BASE_SDP(0x500A)
#define	MSG_SDP2UI_NOTIFY_PURCHASE				MSG_BASE_SDP(0x500B)
#define	MSG_SDP2UI_UPDATE_RECOMM				MSG_BASE_SDP(0x500C)
#endif

//msjin; 20100719
#ifdef INCLUDE_WIFIRC
/* to UI for WIFIRC */
#define MSG_HDCP2UI_DIPLAY_REQUEST              MSG_BASE_WIFIRC(0x0001)
#endif

#define MSG_PQ2PQ_PS_RATIO_CONTROL				MSG_BASE_PQ(0x0001)
#define MSG_PQ2UI_UPDATE_ENERGY_SAVING			MSG_BASE_PQ(0x1000)

#ifdef INCLUDE_MDNS
#define MSG_UI2MDNS_START						MSG_BASE_MDNS(0x0001)
#define MSG_UI2MDNS_STOP						MSG_BASE_MDNS(0x0002)
#endif

#ifdef INCLUDE_REMOTESVC
#define MSG_RS2UI_NOTIFY_SERVICE_REGISTERED		MSG_BASE_RS(0x0000)
#define MSG_RS2UI_NOTIFY_SERVICE_LAUNCHED		MSG_BASE_RS(0x0001)
#define MSG_RS2UI_NOTIFY_SERVICE_CLOSED			MSG_BASE_RS(0x0002)
#define MSG_RS2UI_NOTIFY_ERROR					MSG_BASE_RS(0x0003)
#define MSG_RS2UI_REQUEST_GET_INFORMATION		MSG_BASE_RS(0x0010)
#define MSG_RS2UI_REQUEST_SET_INFORMATION		MSG_BASE_RS(0x0020)
#define MSG_RS2UI_REQUEST_PRESS_IRKEY			MSG_BASE_RS(0x0030)
#ifdef INCLUDE_1Q
#define MSG_RS2UI_REQUEST_CAPTURE_SCREEN		MSG_BASE_RS(0x0040)
#endif
#endif /* INCLUDE_REMOTESVC */

#ifdef INCLUDE_SMARTSHARE
#define MSG_UI2SMH_USB_LOADED					MSG_BASE_SMH(0x0000)
#define MSG_UI2SMH_USB_UNLOADED					MSG_BASE_SMH(0x0001)
#define MSG_DLNA2SMH_DLNA_LOADED				MSG_BASE_SMH(0x0002)
#define MSG_DLNA2SMH_DLNA_UNLOADED				MSG_BASE_SMH(0x0003)
#define MSG_UI2SMH_DVR_LOADED					MSG_BASE_SMH(0x0004)
#define MSG_UI2SMH_DVR_UNLOADED					MSG_BASE_SMH(0x0005)
#define MSG_MDNS2SMH_PLEX_LOADED				MSG_BASE_SMH(0x0006)
#define MSG_MDNS2SMH_PLEX_UNLOADED				MSG_BASE_SMH(0x0007)
#define MSG_DLNA2SMH_DLNA_UPDATED				MSG_BASE_SMH(0x0008)

#define MSG_API2SMH_IC_PREFETCH					MSG_BASE_SMH(0x0010)
#define MSG_API2SMH_IC_STARTTASK				MSG_BASE_SMH(0x0020)
#define MSG_API2SMH_IC_STOPTASK					MSG_BASE_SMH(0x0030)

#define MSG_UI2SMH_IMAGE_REQUEST				MSG_BASE_SMH(0x1000)
#define MSG_SMH2UI_IMAGE_DECODING_DONE			MSG_BASE_SMH(0x1001)
#define MSG_SMH2UI_METADATA_GATHERING_DONE		MSG_BASE_SMH(0x1002)

#define MSG_SMH_USBSCAN_REQUEST					MSG_BASE_SMH(0x2000)
#define MSG_SMH_USBSCAN_STOP					MSG_BASE_SMH(0x2001)
#define MSG_SMH_USBSCAN_LIST					MSG_BASE_SMH(0x2002)
#define MSG_SMH_USBSCAN_COMPLETE				MSG_BASE_SMH(0x2003)

#define MSG_SMH_DLNASCAN_REQUEST				MSG_BASE_SMH(0x2010) // scan request to dlna
#define MSG_SMH_DLNASCAN_STOP					MSG_BASE_SMH(0x2011) // scan stop to dlna
#define MSG_SMH_DLNASCAN_LIST					MSG_BASE_SMH(0x2012) // scan list to SMH
#define MSG_SMH_DLNASCAN_COMPLETE				MSG_BASE_SMH(0x2013) // scan complete to SMH
#define MSG_SMH_DLNASCAN_NOUPDATE				MSG_BASE_SMH(0x2014) // no update to SMH
#define MSG_SMH_DLNASCAN_PAUSE					MSG_BASE_SMH(0x2015) 
#define MSG_SMH_DLNASCAN_RESUME					MSG_BASE_SMH(0x2016) 

#define MSG_SMH_PLEXSCAN_REQUEST				MSG_BASE_SMH(0x2100)
#define MSG_SMH_PLEXSCAN_STOP					MSG_BASE_SMH(0x2101)
#define MSG_SMH_PLEXSCAN_VIDEO_LIST				MSG_BASE_SMH(0x2102)
#define MSG_SMH_PLEXSCAN_TRACK_LIST				MSG_BASE_SMH(0x2103)
#define MSG_SMH_PLEXSCAN_PHOTO_LIST				MSG_BASE_SMH(0x2104)
#define MSG_SMH_PLEXSCAN_COMPLETE				MSG_BASE_SMH(0x2105)
#define MSG_SMH_PLEXSCAN_NOUPDATE				MSG_BASE_SMH(0x2106)
#define MSG_SMH_PLEXSCAN_CHECK_TIMELINE			MSG_BASE_SMH(0x2107)
#define MSG_SMH_PLEX_START_SCAN					MSG_BASE_SMH(0x2108)

#define MSG_SMH_DVRSCAN_REQUEST					MSG_BASE_SMH(0x2200)
#define MSG_SMH_DVRSCAN_STOP					MSG_BASE_SMH(0x2201)
#define MSG_SMH_DVRSCAN_LIST					MSG_BASE_SMH(0x2202)
#define MSG_SMH_DVRSCAN_COMPLETE				MSG_BASE_SMH(0x2203)
#define MSG_UI2SMH_DVR_FILE_ADD					MSG_BASE_SMH(0x2204)
#define MSG_UI2SMH_DVR_FILE_DEL					MSG_BASE_SMH(0x2205)
#define MSG_UI2SMH_DVR_FILE_EDIT				MSG_BASE_SMH(0x2206)

#define MSG_SMH2SMH_METADATA_REQUEST			MSG_BASE_SMH(0x3001)
#define MSG_SMH2SMH_METADATA_COMPLETE			MSG_BASE_SMH(0x3002)

#define MSG_SMH2UI_GATHERING_START				MSG_BASE_SMH(0x4001)
#define MSG_SMH2UI_GATHERING_COMPLETE			MSG_BASE_SMH(0x4002)

#define MSG_UI2SMH_MLMDLNA_START				MSG_BASE_SMH(0x5000)
#define MSG_UI2SMH_MLMDLNA_STOP					MSG_BASE_SMH(0x5001)
#define MSG_UI2SMH_DLNAPLAY_START				MSG_BASE_SMH(0x5002)
#define MSG_UI2SMH_DLNAPLAY_STOP				MSG_BASE_SMH(0x5003)
#endif //INCLUDE_SMARTSHARE

#ifdef INCLUDE_HBBTV
#define MSG_HB2UI_CHANNEL_CHANGE                MSG_BASE_HBBTV(0x0001)
#define MSG_HB2UI_VIDEO_SCALE_ON                MSG_BASE_HBBTV(0x0002)
#define MSG_HB2UI_VIDEO_SCALE_OFF               MSG_BASE_HBBTV(0x0003)
#define MSG_HB2UI_APP_READY                     MSG_BASE_HBBTV(0x0004)
#define MSG_HB2UI_APP_NONE                      MSG_BASE_HBBTV(0x0005)
#define MSG_HB2UI_STREAMING_START               MSG_BASE_HBBTV(0x0006)
#define MSG_HB2UI_HBBTV_OFF                     MSG_BASE_HBBTV(0x0007)
#endif


/*		WI-FI Dispaly	-donghee0109.lee 20110620 	*/

#ifdef INCLUDE_WIFI_DISPLAY
#define MSG_WFD2UI_INIT_READY					MSG_BASE_WFD(0x1001)
#define MSG_WFD2UI_PLAY_START					MSG_BASE_WFD(0x1002)
#define MSG_WFD2UI_PAUSE						MSG_BASE_WFD(0x1003)
#define MSG_WFD2UI_SESSION_END					MSG_BASE_WFD(0x1004)
#define MSG_WFD2UI_ERROR						MSG_BASE_WFD(0x1005)
#define MSG_WFD2UI_TIME_OUT						MSG_BASE_WFD(0x1006)
#define MSG_WFD2UI_FINALIZE_COMPLETE			MSG_BASE_WFD(0x1007)
#endif


#ifdef __cplusplus
}
#endif

#endif /* end of _MESSAGE_H_ */
