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
/*----------------------------------------------------------------------------*/
/*! @file u_dlna_upnp.h
*  $RCSfile: $
*  $Revision: #1 $
*  $Date: 2012/04/27 $
*  $Author: dtvbm11 $
*  
*  @par Description:
*         This header file contains DLNA UPnP exported constants, macros,
*         and types.
*/
/*----------------------------------------------------------------------------*/

/*!
* @addtogroup  groupMW_INET_DLNA
* @brief       Provide an external API about DLNA.
* %DLNA API would be used by user who wants to play media stream in local network  
* 
* @see 
*
* @{
* */


#ifndef U_DLNA_UPNP_H
#define U_DLNA_UPNP_H

/* Base Properties */
#define DLNA_PROP_AT_ID                                     "@id"
#define DLNA_PROP_AT_PARENT_ID                              "@parentID"
#define DLNA_PROP_AT_REF_ID                                 "@refID"
#define DLNA_PROP_AT_RESTRICTED                             "@restricted"
#define DLNA_PROP_AT_SEARCHABLE                             "@searchable"
#define DLNA_PROP_AT_CHILDCOUNT                             "@childCount"
#define DLNA_PROP_DC_NS_TITLE                               "dc:title"
#define DLNA_PROP_DC_NS_CREATOR                             "dc:creator"
#define DLNA_PROP_RES                                       "res"
#define DLNA_PROP_UPNP_NS_CLASS                             "upnp:class"
#define DLNA_PROP_UPNP_NS_CLASS_AT_NAME                     "upnp:class@name"
#define DLNA_PROP_UPNP_NS_SEARCHCLASS                       "upnp:searchClass"
#define DLNA_PROP_UPNP_NS_SEARCHCLASS_AT_NAME               "upnp:searchClass@name"
#define DLNA_PROP_UPNP_NS_SEARCHCLASS_AT_INCLUDEDERIVED     "upnp:searchClass@includeDerived"
#define DLNA_PROP_UPNP_NS_CREATECLASS                       "upnp:createClass"
#define DLNA_PROP_UPNP_NS_CREATECLASS_AT_NAME               "upnp:createClass@name"
#define DLNA_PROP_UPNP_NS_CREATECLASS_AT_INCLUDEDERIVED     "upnp:createClass@includeDerived"
#define DLNA_PROP_UPNP_NS_WRITESTATUS                       "upnp:writeStatus"

/* Resource Encoding Characteristics Properties */             
#define DLNA_PROP_RES_AT_PROTOCOLINFO    "res@protocolInfo"    
#define DLNA_PROP_RES_AT_IMPORTURI       "res@importUri"       
#define DLNA_PROP_RES_AT_SIZE            "res@size"            
#define DLNA_PROP_RES_AT_DURATION        "res@duration"        
#define DLNA_PROP_RES_AT_PROTECTION      "res@protection"      
#define DLNA_PROP_RES_AT_BITRATE         "res@bitrate"         
#define DLNA_PROP_RES_AT_BITSPERSAMPLE   "res@bitsPerSample"   
#define DLNA_PROP_RES_AT_SAMPLEFREQUENCY "res@sampleFrequency" 
#define DLNA_PROP_RES_AT_NRAUDIOCHANNELS "res@nrAudioChannels" 
#define DLNA_PROP_RES_AT_RESOLUTION      "res@resolution"      
#define DLNA_PROP_RES_AT_COLORDEPTH      "res@colorDepth"      
#define DLNA_PROP_RES_AT_TSPEC           "res@tspec"           
#define DLNA_PROP_RES_AT_ALLOWEDUSE      "res@allowedUse"      
#define DLNA_PROP_RES_AT_VALIDITYSTART   "res@validityStart"   
#define DLNA_PROP_RES_AT_VALIDITYEND     "res@validityEnd"     
#define DLNA_PROP_RES_AT_REMAININGTIME   "res@remainingTime"   
#define DLNA_PROP_RES_AT_USAGEINFO       "res@usageInfo"       
#define DLNA_PROP_RES_AT_RIGHTSINFOURI   "res@rightsInfoURI"   
#define DLNA_PROP_RES_AT_CONTENTINFOURI  "res@contentInfoURI"  
#define DLNA_PROP_RES_AT_RECORDQUALITY   "res@recordQuality"   

/* Contributor-related Properties */
#define DLNA_PROP_UPNP_NS_ARTIST            "upnp:artist"
#define DLNA_PROP_UPNP_NS_ARTIST_AT_ROLE    "upnp:artist@role"
#define DLNA_PROP_UPNP_NS_ACTOR             "upnp:actor"
#define DLNA_PROP_UPNP_NS_ACTOR_AT_ROLE     "upnp:actor@role"
#define DLNA_PROP_UPNP_NS_AUTHOR            "upnp:author"
#define DLNA_PROP_UPNP_NS_AUTHOR_AT_ROLE    "upnp:author@role"
#define DLNA_PROP_UPNP_NS_PRODUCER          "upnp:producer"
#define DLNA_PROP_UPNP_NS_DIRECTOR          "upnp:director"
#define DLNA_PROP_DC_NS_PUBLISHER           "dc:publisher"
#define DLNA_PROP_DC_NS_CONTRIBUTOR         "dc:contributor"

/* Affiliation-related Properties */
#define DLNA_PROP_UPNP_NS_GENRE             "upnp:genre"
#define DLNA_PROP_UPNP_NS_GENRE_AT_ID       "upnp:genre@id"
#define DLNA_PROP_UPNP_NS_GENRE_AT_EXTENDED "upnp:genre@extended"
#define DLNA_PROP_UPNP_NS_ALBUM             "upnp:album"
#define DLNA_PROP_UPNP_NS_PLAYLIST          "upnp:playlist"

/* Associated Resources Properties */
#define DLNA_PROP_UPNP_NS_ALBUMARTURI           "upnp:albumArtURI"
#define DLNA_PROP_UPNP_NS_ARTISTDISCOGRAPHYURI  "upnp:artistDiscographyURI"
#define DLNA_PROP_UPNP_NS_LYRICSURI             "upnp:lyricsURI"
#define DLNA_PROP_DC_NS_RELATION                "dc:relation"

/* Storage-Related Properties */
#define DLNA_PROP_UPNP_NS_STORAGETOTAL          "upnp:storageTotal"
#define DLNA_PROP_UPNP_NS_STORAGEUSED           "upnp:storageUsed"
#define DLNA_PROP_UPNP_NS_STORAGEFREE           "upnp:storageFree"
#define DLNA_PROP_UPNP_NS_STORAGEMAXPARTITION   "upnp:storageMaxPartition"
#define DLNA_PROP_UPNP_NS_STORAGEMEDIUM         "upnp:storageMedium"

/* General Description (mainly for UI purposes) Properties */
#define DLNA_PROP_DC_NS_DESCRIPTION                 "dc:description"
#define DLNA_PROP_UPNP_NS_LONGDESCRIPTION           "upnp:longDescription"
#define DLNA_PROP_UPNP_NS_ICON                      "upnp:icon"
#define DLNA_PROP_UPNP_NS_REGION                    "upnp:region"
#define DLNA_PROP_DC_NS_RIGHTS                      "dc:rights"
#define DLNA_PROP_DC_NS_DATE                        "dc:date"
#define DLNA_PROP_DC_NS_LANGUAGE                    "dc:language"
#define DLNA_PROP_UPNP_NS_PLAYBACKCOUNT             "upnp:playbackCount"
#define DLNA_PROP_UPNP_NS_LASTPLAYBACKTIME          "upnp:lastPlaybackTime"
#define DLNA_PROP_UPNP_NS_LASTPLAYBACKPOSITION      "upnp:lastPlaybackPosition"
#define DLNA_PROP_UPNP_NS_RECORDEDSTARTDATETIME     "upnp:recordedStartDateTime"
#define DLNA_PROP_UPNP_NS_RECORDEDDURATION          "upnp:recordedDuration"
#define DLNA_PROP_UPNP_NS_RECORDEDDAYOFWEEK         "upnp:recordedDayOfWeek"
#define DLNA_PROP_UPNP_NS_SRSRECORDSCHEDULEID       "upnp:srsRecordScheduleID"
#define DLNA_PROP_UPNP_NS_SRSRECORDTASKID           "upnp:srsRecordTaskID"
#define DLNA_PROP_UPNP_NS_RECORDABLE                "upnp:recordable"

/* Recorded Object-related Properties */
#define DLNA_PROP_UPNP_PROGRAMTITLE         "upnp:programTitle"
#define DLNA_PROP_UPNP_SERIESTITLE          "upnp:seriesTitle"
#define DLNA_PROP_UPNP_PROGRAMID            "upnp:programID"
#define DLNA_PROP_UPNP_PROGRAMID_AT_TYPE    "upnp:programID@type"
#define DLNA_PROP_UPNP_SERIESID             "upnp:seriesID"
#define DLNA_PROP_UPNP_SERIESID_AT_TYPE     "upnp:seriesID@type"
#define DLNA_PROP_UPNP_CHANNELID            "upnp:channelID"
#define DLNA_PROP_UPNP_CHANNELID_AT_TYPE    "upnp:channelID@type"
#define DLNA_PROP_UPNP_EPISODECOUNT         "upnp:episodeCount"
#define DLNA_PROP_UPNP_EPISODENUMBER        "upnp:episodeNumber"
#define DLNA_PROP_UPNP_PROGRAMCODE          "upnp:programCode"
#define DLNA_PROP_UPNP_PROGRAMCODE_AT_TYPE  "upnp:programCode@type"
#define DLNA_PROP_UPNP_RATING               "upnp:rating"
#define DLNA_PROP_UPNP_RATING_AT_TYPE       "upnp:rating@type"
#define DLNA_PROP_UPNP_EPISODETYPE          "upnp:episodeType"

/* User Channel and EPG Related Properties */
#define DLNA_PROP_UPNP_NS_CHANNELGROUPNAME          "upnp:channelGroupName"
#define DLNA_PROP_UPNP_NS_CHANNELGROUPNAME_AT_ID    "upnp:channelGroupName@id"
#define DLNA_PROP_UPNP_NS_CALLSIGN                  "upnp:callSign"
#define DLNA_PROP_UPNP_NS_NETWORKAFFILIATION        "upnp:networkAffiliation"
#define DLNA_PROP_UPNP_NS_SERVICEPROVIDER           "upnp:serviceProvider"
#define DLNA_PROP_UPNP_NS_PRICE                     "upnp:price"
#define DLNA_PROP_UPNP_NS_PRICE_AT_CURRENCY         "upnp:price@currency"
#define DLNA_PROP_UPNP_NS_PAYPERVIEW                "upnp:payPerView"
#define DLNA_PROP_UPNP_NS_EPGPROVIDERNAME           "upnp:epgProviderName"
#define DLNA_PROP_UPNP_NS_DATETIMERANGE             "upnp:dateTimeRange"

/* Radio Broadcast Properties */
#define DLNA_PROP_UPNP_NS_RADIOCALLSIGN     "upnp:radioCallSign"
#define DLNA_PROP_UPNP_NS_RADIOSTATIONID    "upnp:radioStationID"
#define DLNA_PROP_UPNP_NS_RADIOBAND         "upnp:radioBand"

/* Video Broadcast Properties */
#define DLNA_PROP_UPNP_NS_CHANNELNR             "upnp:channelNr"
#define DLNA_PROP_UPNP_NS_CHANNELNAME           "upnp:channelName"
#define DLNA_PROP_UPNP_NS_SCHEDULEDSTARTTIME    "upnp:scheduledStartTime"
#define DLNA_PROP_UPNP_NS_SCHEDULEDENDTIME      "upnp:scheduledEndTime"

/* Physical Tuner Status-related Properties */
#define DLNA_PROP_UPNP_NS_SIGNALSTRENGTH    "upnp:signalStrength"
#define DLNA_PROP_UPNP_NS_SIGNALLOCKED      "upnp:signalLocked"
#define DLNA_PROP_UPNP_NS_TUNED             "upnp:tuned"

/* Bookmark-related Properties */
#define DLNA_PROP_AT_NEVERPLAYABLE                                      "@neverPlayable"
#define DLNA_PROP_UPNP_NS_BOOKMARKID                                    "upnp:bookmarkID"
#define DLNA_PROP_UPNP_NS_BOOKMARKEDOBJECTID                            "upnp:bookmarkedObjectID"
#define DLNA_PROP_UPNP_NS_DEVICEUDN                                     "upnp:deviceUDN"
#define DLNA_PROP_UPNP_NS_DEVICEUDN_AT_SERVICETYPE                      "upnp:deviceUDN@serviceType"
#define DLNA_PROP_UPNP_NS_DEVICEUDN_AT_SERVICEID                        "upnp:deviceUDN@serviceId"
#define DLNA_PROP_UPNP_NS_STATEVARIABLECOLLECTION                       "upnp:stateVariableCollection"
#define DLNA_PROP_UPNP_NS_STATEVARIABLECOLLECTION_AT_SERVICENAME        "upnp:stateVariableCollection@serviceName"
#define DLNA_PROP_UPNP_NS_STATEVARIABLECOLLECTION_AT_RCSINSTANCETYPE    "upnp:stateVariableCollection@rcsInstanceType"

/* Miscellaneous Properties */
#define DLNA_PROP_UPNP_NS_VDREGIONCODE               "upnp:DVDRegionCode"
#define DLNA_PROP_UPNP_NS_RIGINALTRACKNUMBER         "upnp:originalTrackNumber"
#define DLNA_PROP_UPNP_NS_OC                         "upnp:toc"
#define DLNA_PROP_UPNP_NS_SERANNOTATION              "upnp:userAnnotation"



/* allowedValueList for the upnp:class Property */
#define DLNA_PROP_UPNP_CLASS_OBJECT_ITEM                                        "object.item"
#define DLNA_PROP_UPNP_CLASS_OBJECT_ITEM_IMAGEITEM                              "object.item.imageItem"
#define DLNA_PROP_UPNP_CLASS_OBJECT_ITEM_IMAGEITEM_PHOTO                        "object.item.imageItem.photo"
#define DLNA_PROP_UPNP_CLASS_OBJECT_ITEM_AUDIOITEM                              "object.item.audioItem"
#define DLNA_PROP_UPNP_CLASS_OBJECT_ITEM_AUDIOITEM_MUSICTRACK                   "object.item.audioItem.musicTrack"
#define DLNA_PROP_UPNP_CLASS_OBJECT_ITEM_AUDIOITEM_AUDIOBROADCAST               "object.item.audioItem.audioBroadcast"
#define DLNA_PROP_UPNP_CLASS_OBJECT_ITEM_AUDIOITEM_AUDIOBOOK                    "object.item.audioItem.audioBook"
#define DLNA_PROP_UPNP_CLASS_OBJECT_ITEM_VIDEOITEM                              "object.item.videoItem"
#define DLNA_PROP_UPNP_CLASS_OBJECT_ITEM_VIDEOITEM_MOVIE                        "object.item.videoItem.movie"
#define DLNA_PROP_UPNP_CLASS_OBJECT_ITEM_VIDEOITEM_VIDEOBROADCAST               "object.item.videoItem.videoBroadcast"
#define DLNA_PROP_UPNP_CLASS_OBJECT_ITEM_VIDEOITEM_MUSICVIDEOCLIP               "object.item.videoItem.musicVideoClip"
#define DLNA_PROP_UPNP_CLASS_OBJECT_ITEM_PLAYLISTITEM                           "object.item.playlistItem"
#define DLNA_PROP_UPNP_CLASS_OBJECT_ITEM_TEXTITEM                               "object.item.textItem"
#define DLNA_PROP_UPNP_CLASS_OBJECT_ITEM_BOOKMARKITEM                           "object.item.bookmarkItem"
#define DLNA_PROP_UPNP_CLASS_OBJECT_ITEM_EPGITEM                                "object.item.epgItem"
#define DLNA_PROP_UPNP_CLASS_OBJECT_ITEM_EPGITEM_AUDIOPROGRAM                   "object.item.epgItem.audioProgram"
#define DLNA_PROP_UPNP_CLASS_OBJECT_ITEM_EPGITEM_VIDEOPROGRAM                   "object.item.epgItem.videoProgram"
#define DLNA_PROP_UPNP_CLASS_OBJECT_CONTAINER_PERSON                            "object.container.person"
#define DLNA_PROP_UPNP_CLASS_OBJECT_CONTAINER_PERSON_MUSICARTIST                "object.container.person.musicArtist"
#define DLNA_PROP_UPNP_CLASS_OBJECT_CONTAINER_PLAYLISTCONTAINER                 "object.container.playlistContainer"
#define DLNA_PROP_UPNP_CLASS_OBJECT_CONTAINER_ALBUM                             "object.container.album"
#define DLNA_PROP_UPNP_CLASS_OBJECT_CONTAINER_ALBUM_MUSICALBUM                  "object.container.album.musicAlbum"
#define DLNA_PROP_UPNP_CLASS_OBJECT_CONTAINER_ALBUM_PHOTOALBUM                  "object.container.album.photoAlbum"
#define DLNA_PROP_UPNP_CLASS_OBJECT_CONTAINER_GENRE                             "object.container.genre"
#define DLNA_PROP_UPNP_CLASS_OBJECT_CONTAINER_GENRE_MUSICGENRE                  "object.container.genre.musicGenre"
#define DLNA_PROP_UPNP_CLASS_OBJECT_CONTAINER_GENRE_MOVIEGENRE                  "object.container.genre.movieGenre"
#define DLNA_PROP_UPNP_CLASS_OBJECT_CONTAINER_CHANNELGROUP                      "object.container.channelGroup"
#define DLNA_PROP_UPNP_CLASS_OBJECT_CONTAINER_CHANNELGROUP_AUDIOCHANNELGROUP    "object.container.channelGroup.audioChannelGroup"
#define DLNA_PROP_UPNP_CLASS_OBJECT_CONTAINER_CHANNELGROUP_VIDEOCHANNELGROUP    "object.container.channelGroup.videoChannelGroup"
#define DLNA_PROP_UPNP_CLASS_OBJECT_CONTAINER_EPGCONTAINER                      "object.container.epgContainer"
#define DLNA_PROP_UPNP_CLASS_OBJECT_CONTAINER_STORAGESYSTEM                     "object.container.storageSystem"
#define DLNA_PROP_UPNP_CLASS_OBJECT_CONTAINER_STORAGEVOLUME                     "object.container.storageVolume"
#define DLNA_PROP_UPNP_CLASS_OBJECT_CONTAINER_STORAGEFOLDER                     "object.container.storageFolder"
#define DLNA_PROP_UPNP_CLASS_OBJECT_CONTAINER_BOOKMARKFOLDER                    "object.container.bookmarkFolder"

/* allowedValueList for the upnp:writeStatus Property */
#define DLNA_PROP_UPNP_NS_WRITESTATUS_WRITABLE    	"WRITABLE"
#define DLNA_PROP_UPNP_NS_WRITESTATUS_PROTECTED   	"PROTECTED"
#define DLNA_PROP_UPNP_NS_WRITESTATUS_NOT_WRITABLE	"NOT_WRITABLE"
#define DLNA_PROP_UPNP_NS_WRITESTATUS_UNKNOWN     	"UNKNOWN"
#define DLNA_PROP_UPNP_NS_WRITESTATUS_MIXED       	"MIXED"

/* allowedValueList for the upnp:recordedDayOfWeek Property */
#define DLNA_PROP_UPNP_NS_RECORDEDDAYOFWEEK_SUN "SUN" 
#define DLNA_PROP_UPNP_NS_RECORDEDDAYOFWEEK_MON "MON" 
#define DLNA_PROP_UPNP_NS_RECORDEDDAYOFWEEK_TUE "TUE" 
#define DLNA_PROP_UPNP_NS_RECORDEDDAYOFWEEK_WED "WED" 
#define DLNA_PROP_UPNP_NS_RECORDEDDAYOFWEEK_THU "THU" 
#define DLNA_PROP_UPNP_NS_RECORDEDDAYOFWEEK_FRI "FRI" 
#define DLNA_PROP_UPNP_NS_RECORDEDDAYOFWEEK_SAT "SAT" 


#endif /* U_DLNA_UPNP_H */

/*! @} */

