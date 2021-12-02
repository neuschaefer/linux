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

#ifndef PAPI_TYPES_H
#define PAPI_TYPES_H

#if 0

#define __STDC_HOSTED__ 1
#define __GNUC__ 3
#define __GNUC_MINOR__ 4
#define __GNUC_PATCHLEVEL__ 4
#define __SIZE_TYPE__ unsigned int
#define __PTRDIFF_TYPE__ int
#define __WCHAR_TYPE__ short unsigned int
#define __WINT_TYPE__ unsigned int
#define __GXX_ABI_VERSION 1002
#define __USING_SJLJ_EXCEPTIONS__ 1
#define __SCHAR_MAX__ 127
#define __SHRT_MAX__ 32767
#define __INT_MAX__ 2147483647
#define __LONG_MAX__ 2147483647L
#define __LONG_LONG_MAX__ 9223372036854775807LL
#define __WCHAR_MAX__ 65535U
#define __CHAR_BIT__ 8
#define __FLT_EVAL_METHOD__ 2
#define __FLT_RADIX__ 2
#define __FLT_MANT_DIG__ 24
#define __FLT_DIG__ 6
#define __FLT_MIN_EXP__ (-125)
#define __FLT_MIN_10_EXP__ (-37)
#define __FLT_MAX_EXP__ 128
#define __FLT_MAX_10_EXP__ 38
#define __FLT_MAX__ 3.40282347e+38F
#define __FLT_MIN__ 1.17549435e-38F
#define __FLT_EPSILON__ 1.19209290e-7F
#define __FLT_DENORM_MIN__ 1.40129846e-45F
#define __FLT_HAS_INFINITY__ 1
#define __FLT_HAS_QUIET_NAN__ 1
#define __DBL_MANT_DIG__ 53
#define __DBL_DIG__ 15
#define __DBL_MIN_EXP__ (-1021)
#define __DBL_MIN_10_EXP__ (-307)
#define __DBL_MAX_EXP__ 1024
#define __DBL_MAX_10_EXP__ 308
#define __DBL_MAX__ 1.7976931348623157e+308
#define __DBL_MIN__ 2.2250738585072014e-308
#define __DBL_EPSILON__ 2.2204460492503131e-16
#define __DBL_DENORM_MIN__ 4.9406564584124654e-324
#define __DBL_HAS_INFINITY__ 1
#define __DBL_HAS_QUIET_NAN__ 1
#define __LDBL_MANT_DIG__ 64
#define __LDBL_DIG__ 18
#define __LDBL_MIN_EXP__ (-16381)
#define __LDBL_MIN_10_EXP__ (-4931)
#define __LDBL_MAX_EXP__ 16384
#define __LDBL_MAX_10_EXP__ 4932
#define __DECIMAL_DIG__ 21
#define __LDBL_MAX__ 1.18973149535723176502e+4932L
#define __LDBL_MIN__ 3.36210314311209350626e-4932L
#define __LDBL_EPSILON__ 1.08420217248550443401e-19L
#define __LDBL_DENORM_MIN__ 3.64519953188247460253e-4951L
#define __LDBL_HAS_INFINITY__ 1
#define __LDBL_HAS_QUIET_NAN__ 1
#define __REGISTER_PREFIX__ 
#define __USER_LABEL_PREFIX__ _
#define __VERSION__ "3.4.4 (cygming special) (gdc 0.12, using dmd 0.125)"
#define __NO_INLINE__ 1
#define __FINITE_MATH_ONLY__ 0


#define __i386 1
#define __i386__ 1
#define i386 1
#define __tune_i686__ 1
#define __tune_pentiumpro__ 1
#define _X86_ 1

#define __stdcall __attribute__((__stdcall__))
#define __fastcall __attribute__((__fastcall__))
#define __cdecl __attribute__((__cdecl__))
#define __declspec(x) __attribute__((x))
#define _stdcall __attribute__((__stdcall__))
#define _fastcall __attribute__((__fastcall__))
#define _cdecl __attribute__((__cdecl__))
#define __CYGWIN32__ 1
#define __CYGWIN__ 1
#define unix 1
#define __unix__ 1
#define __unix 1
#define __linux__ 1
#define NDEBUG 1
#define PAPI_TYPES_H 
#define _INFRAGLOBALS_DD_ 
#define _PHSTDTYPES_H_ 
#define _PHSTDTYPESGCCX86LINUX_H_ 

#endif


/* Standard error codes */
/**
 *   @defgroup StandardErrorCodes Standard error codes
 *   Standard error codes.
 *   @{ */
/** @brief No error */
#define TM_OK                           0x00000000U
/** @brief SW interface compatibility error  */
#define TM_ERR_COMPATIBILITY            0x001U
/** @brief SW major version error       */
#define TM_ERR_MAJOR_VERSION            0x002U
/** @brief SW component version error   */
#define TM_ERR_COMP_VERSION             0x003U
/** @brief SW - HW module ID error      */
#define TM_ERR_BAD_MODULE_ID            0x004U
/** @brief Invalid device unit number   */
#define TM_ERR_BAD_UNIT_NUMBER          0x005U
/** @brief Bad input instance value     */
#define TM_ERR_BAD_INSTANCE             0x006U
/** @brief Bad input handle             */
#define TM_ERR_BAD_HANDLE               0x007U
/** @brief Bad input index              */
#define TM_ERR_BAD_INDEX                0x008U
/** @brief Invalid input parameter      */
#define TM_ERR_BAD_PARAMETER            0x009U
/** @brief No instances available       */
#define TM_ERR_NO_INSTANCES             0x00AU
/** @brief Component is not present     */
#define TM_ERR_NO_COMPONENT             0x00BU
/** @brief Resource is not available    */
#define TM_ERR_NO_RESOURCES             0x00CU
/** @brief Instance is already in use   */
#define TM_ERR_INSTANCE_IN_USE          0x00DU
/** @brief Resource is already in use   */
#define TM_ERR_RESOURCE_OWNED           0x00EU
/** @brief Caller does not own resource */
#define TM_ERR_RESOURCE_NOT_OWNED       0x00FU
/** @brief Inconsistent input parameters */
#define TM_ERR_INCONSISTENT_PARAMS      0x010U
/** @brief Component is not initialized */
#define TM_ERR_NOT_INITIALIZED          0x011U
/** @brief Component is not enabled     */
#define TM_ERR_NOT_ENABLED              0x012U
/** @brief Function is not supported    */
#define TM_ERR_NOT_SUPPORTED            0x013U
/** @brief Initialization failed        */
#define TM_ERR_INIT_FAILED              0x014U
/** @brief Component is busy            */
#define TM_ERR_BUSY                     0x015U
/** @brief Component is not busy        */
#define TM_ERR_NOT_BUSY                 0x016U
/** @brief Read error                   */
#define TM_ERR_READ                     0x017U
/** @brief Write error                  */
#define TM_ERR_WRITE                    0x018U
/** @brief Erase error                  */
#define TM_ERR_ERASE                    0x019U
/** @brief Lock error                   */
#define TM_ERR_LOCK                     0x01AU
/** @brief Unlock error                 */
#define TM_ERR_UNLOCK                   0x01BU
/** @brief Memory allocation failed     */
#define TM_ERR_OUT_OF_MEMORY            0x01CU
/** @brief Bad virtual address          */
#define TM_ERR_BAD_VIRT_ADDRESS         0x01DU
/** @brief Bad physical address         */
#define TM_ERR_BAD_PHYS_ADDRESS         0x01EU
/** @brief Timeout error                */
#define TM_ERR_TIMEOUT                  0x01FU
/** @brief Data overflow/overrun error  */
#define TM_ERR_OVERFLOW                 0x020U
/** @brief Queue full         */
#define TM_ERR_FULL                     0x021U
/** @brief Queue empty        */
#define TM_ERR_EMPTY                    0x022U
/** @brief Streaming function failed    */
#define TM_ERR_NOT_STARTED              0x023U
/** @brief Start function failed        */
#define TM_ERR_ALREADY_STARTED          0x024U
/** @brief Non-streaming function failed */
#define TM_ERR_NOT_STOPPED              0x025U
/** @brief Stop function failed         */
#define TM_ERR_ALREADY_STOPPED          0x026U
/** @brief Setup function failed        */
#define TM_ERR_ALREADY_SETUP            0x027U
/** @brief Null input parameter         */
#define TM_ERR_NULL_PARAMETER           0x028U
/** @brief Null data input function     */
#define TM_ERR_NULL_DATAINFUNC          0x029U
/** @brief Null data output function    */
#define TM_ERR_NULL_DATAOUTFUNC         0x02AU
/** @brief Null control function        */
#define TM_ERR_NULL_CONTROLFUNC         0x02BU
/** @brief Null completion function     */
#define TM_ERR_NULL_COMPLETIONFUNC      0x02CU
/** @brief Null progress function       */
#define TM_ERR_NULL_PROGRESSFUNC        0x02DU
/** @brief Null error handler function  */
#define TM_ERR_NULL_ERRORFUNC           0x02EU
/** @brief Null memory alloc function   */
#define TM_ERR_NULL_MEMALLOCFUNC        0x02FU
/** @brief Null memory free function    */
#define TM_ERR_NULL_MEMFREEFUNC         0x030U
/** @brief Null configuration function  */
#define TM_ERR_NULL_CONFIGFUNC          0x031U
/** @brief Null parent data             */
#define TM_ERR_NULL_PARENT              0x032U
/** @brief Null in/out descriptor       */
#define TM_ERR_NULL_IODESC              0x033U
/** @brief Null control descriptor      */
#define TM_ERR_NULL_CTRLDESC            0x034U
/** @brief Unsupported data class       */
#define TM_ERR_UNSUPPORTED_DATACLASS    0x035U
/** @brief Unsupported data type        */
#define TM_ERR_UNSUPPORTED_DATATYPE     0x036U
/** @brief Unsupported data subtype     */
#define TM_ERR_UNSUPPORTED_DATASUBTYPE  0x037U
/** @brief Invalid/unsupported format   */
#define TM_ERR_FORMAT                   0x038U
/** @brief Bad input descriptor flags  */
#define TM_ERR_INPUT_DESC_FLAGS         0x039U
/** @brief Bad output descriptor flags  */
#define TM_ERR_OUTPUT_DESC_FLAGS        0x03AU
/** @brief Capability                   */
#define TM_ERR_CAP_REQUIRED             0x03BU
/** @brief Invalid channel identifier   */
#define TM_ERR_INVALID_CHANNEL_ID       0x03DU
/** @brief Invalid command/request      */
#define TM_ERR_INVALID_COMMAND          0x03EU
/** @brief Stream mode config conflict  */
#define TM_ERR_STREAM_MODE_CONFUSION    0x03FU
/** @brief Data underflow/underrun      */
#define TM_ERR_UNDERRUN                 0x040U
/** @brief Empty data packet received   */
#define TM_ERR_EMPTY_PACKET_RECVD       0x041U
/** @brief Other data input/output error */
#define TM_ERR_OTHER_DATAINOUT_ERR      0x042U
/** @brief Stop in progress             */
#define TM_ERR_STOP_REQUESTED           0x043U
/** @brief Assertion failure            */
#define TM_ERR_ASSERTION                0x049U
/** @brief Highway bandwidth bus error  */
#define TM_ERR_HIGHWAY_BANDWIDTH        0x04AU
/** @brief Hardware reset failed        */
#define TM_ERR_HW_RESET_FAILED          0x04BU
/** @brief Bad flags                    */
#define TM_ERR_BAD_FLAGS                0x04DU
/** @brief Bad priority                 */
#define TM_ERR_BAD_PRIORITY             0x04EU
/** @brief Bad reference count          */
#define TM_ERR_BAD_REFERENCE_COUNT      0x04FU
/** @brief Bad setup                    */
#define TM_ERR_BAD_SETUP                0x050U
/** @brief Bad stack size               */
#define TM_ERR_BAD_STACK_SIZE           0x051U
/** @brief In place                     */
#define TM_ERR_IN_PLACE                 0x053U
/** @brief Not cache aligned            */
#define TM_ERR_NOT_CACHE_ALIGNED        0x054U
/** @brief Null packet                  */
#define TM_ERR_NULL_PACKET              0x059U
/** @brief Format freed                 */
#define TM_ERR_FORMAT_FREED             0x05AU
/** @brief Format internal              */
#define TM_ERR_FORMAT_INTERNAL          0x05BU
/** @brief Bad format                   */
#define TM_ERR_BAD_FORMAT               0x05CU
/** @brief Null format                  */
#define TM_ERR_NULL_FORMAT              0x061U
/** @brief Format reference count       */
#define TM_ERR_FORMAT_REFERENCE_COUNT   0x062U
/** @brief Format not unique            */
#define TM_ERR_FORMAT_NOT_UNIQUE        0x063U
/** @brief New format (not an error)    */
#define TM_NEW_FORMAT                   0x064U
/** @brief Invalid state for function   */
#define TM_ERR_INVALID_STATE            0x066U
/** @brief No connection to this pin    */
#define TM_ERR_NULL_CONNECTION          0x067U
/** @brief Operation not permitted   */
#define TM_ERR_OPERATION_NOT_PERMITTED  0x068U
/** @brief Power down - clocked off     */
#define TM_ERR_NOT_CLOCKED              0x069U
/*   @} */



typedef char *String;
typedef unsigned int Bool;
typedef char Char;

typedef signed char Int8;
typedef short Int16;
typedef long Int32;
typedef long long Int64;

typedef unsigned char UInt8;
typedef unsigned short UInt16;
typedef unsigned long UInt32;
typedef unsigned long long UInt64;
typedef UInt32 tmErrorCode_t;

typedef Int8 * pInt8;
typedef Int16 * pInt16;
typedef Int32 * pInt32;
typedef Int64 * pInt64;

typedef UInt8 * pUInt8;
typedef UInt16 * pUInt16;
typedef UInt32 * pUInt32;
typedef UInt64 * pUInt64;

typedef Bool * pBool;
typedef Char * pChar;
typedef String * pString;



typedef int intptr_t;
typedef unsigned int uintptr_t;


#define INT8_MIN (-127-1)
#define INT8_MAX 127

#define INT16_MIN (-32767-1)
#define INT16_MAX 32767

#define INT32_MIN (-2147483647-1)
#define INT32_MAX 2147483647

#define INT64_MIN (-9223372036854775807-1)
#define INT64_MAX 9223372036854775807

#define UINT8_MAX 255U
#define UINT16_MAX 65535U
#define UINT32_MAX 4294967295U
#define UINT64_MAX 18446744073709551615U





#define TRUE 1




#define FALSE 0

#undef NULL
#define NULL (0)

#define TM_OK 0
#define _PHEXTRATYPES_H_ 
#define TMOPSTDTMTYPEDEFS 

#define False 0
#define Null 0
#define True 1

typedef void Void;
typedef char * Address;
typedef char const * ConstAddress;
typedef void * Pointer;
typedef void const * ConstPointer;
typedef char const * ConstString;

typedef unsigned char Byte;
typedef int Int;
typedef unsigned int UInt;
typedef float Float;
typedef float Float32;

typedef double Float64;







typedef UInt32 Flags;
typedef enum { TM32 = 0, TM3260, TM5250, TM2270, TM3270,
        TM64=100 } TMArch;

extern char* TMArch_names[];


extern TMArch TMArch_values[];

typedef struct {
  UInt8 majorVersion;
  UInt8 minorVersion;
  UInt16 buildVersion;
} tmVersion_t, *ptmVersion_t;
#define LL_CONST(c) (c ##LL)
#define ULL_CONST(c) (c ##ULL)
#define LL_MOD "ll"







typedef Int Endian;
#define BigEndian 0
#define LittleEndian 1







extern char *get_TMArch_name(TMArch arch) ;
#define STRINGIFY(s) #s







#define TRACEPRINT(x,...) ((void)0)




#define CONCAT(a,b) a ##b
typedef unsigned int Nat;

typedef UInt8 Nat8;
typedef UInt16 Nat16;
typedef UInt32 Nat32;

typedef UInt64 Nat64;
typedef Int32 DateTime;
typedef void (*Function)(void);
typedef void (*FunctionInt)(int);
typedef void (*FunctionIntInt)(int,int);
typedef Nat FResult;





#define CHAR_BIT 8
#define SCHAR_MIN (-128)
#define SCHAR_MAX 127
#define UCHAR_MIN 0
#define UCHAR_MAX 0xff

#define CHAR_MIN SCHAR_MIN
#define CHAR_MAX SCHAR_MAX

#define NAT8_MIN 0
#define NAT8_MAX 255U
#define NAT16_MIN 0
#define NAT16_MAX 65535U
#define NAT32_MIN 0
#define NAT32_MAX 4294967295UL

#define NAT_MIN NAT32_MIN
#define NAT_MAX NAT32_MAX

#define INT_MIN INT32_MIN
#define INT_MAX INT32_MAX
#define NEAR 
#define ROM const


#define ROMADDR ROM*


#define HUGE 
#define _RPT0(rptno,msg) 
#define _RPT1(rptno,msg,arg1) 
#define _RPT2(rptno,msg,arg1,arg2) 
#define _RPT3(rptno,msg,arg1,arg2,arg3) 
#define _RPT4(rptno,msg,arg1,arg2,arg3,arg4) 
#define _RPTF0(rptno,msg) 
#define _RPTF1(rptno,msg,arg1) 
#define _RPTF2(rptno,msg,arg1,arg2) 
#define _RPTF3(rptno,msg,arg1,arg2,arg3) 
#define _RPTF4(rptno,msg,arg1,arg2,arg3,arg4) 
#define DBGPRINT(txt) 
#define DBGPRINT1(fmt,arg1) 
#define DBGPRINT2(fmt,arg1,arg2) 
#define DBGPRINT3(fmt,arg1,arg2,arg3) 
#define DBGPRINT4(fmt,arg1,arg2,arg3,arg4) 
#define UNUSED(p) ( (void) ( (void)(p),0) )
#define ASSERT(e) ((void)0)
#define ASSERT_NO_NOTIFY(e) ((void)0)
#define assert(e) ((void)0)
#define _INFRACEC_DD 




typedef enum
{
  Version1_1 = 0
, Version1_2 = 1
, Version1_2a = 2
, Version1_3 = 3
, Version1_3a = 4
} CecVersion;


typedef enum
{
  cecLogicalAddressTv = 0
, cecLogicalAddressRecDev1 = 1
, cecLogicalAddressRecDev2 = 2
, cecLogicalAddressStb1 = 3
, cecLogicalAddressDvd1 = 4
, cecLogicalAddressAudioSys = 5
, cecLogicalAddressStb2 = 6
, cecLogicalAddressStb3 = 7
, cecLogicalAddressDvd2 = 8
, cecLogicalAddressRecDev3 = 9
, cecLogicalAddressFreeUse = 14
, cecLogicalAddressBroadcast = 15
, cecLogicalAddressUnreg = 15
} CecLogicalAddress;



typedef enum
{
  cecTransmissionStatusSuccess
, cecTransmissionStatusBusBlocked
, cecTransmissionStatusDeviceNotPresent
, cecTransmissionStatusDeviceBusy
, cecTransmissionStatusMessageRejected
} CecTransmissionStatus;


typedef enum
{
  cecFeatureAbort = 0x00
, cecSetAnaloguePreset = 0x01
, cecStartPresetTransfer = 0x02
, cecEndPresetTransfer = 0x03
, cecImageViewOn = 0x04
, cecTunerStepIncrement = 0x05
, cecTunerStepDecrement = 0x06
, cecTunerDeviceStatus = 0x07
, cecGiveTunerDeviceStatus = 0x08
, cecRecordOn = 0x09
, cecRecordStatus = 0x0A
, cecRecordOff = 0x0B
, cecSetPresetName = 0x0C
, cecTextViewOn = 0x0D

, cecRecordTvScreen = 0x0F
, cecStopPresetTransfer = 0x10


, cecSelectPreset = 0x13

, cecGivePresetListVersionNumber = 0x15
, cecActualPresetListVersionNumber = 0x16


, cecTimerRecordListTrigger = 0x19
, cecGiveDeckStatus = 0x1A
, cecDeckStatus = 0x1B
, cecSetCountry = 0x31
, cecSetLanguage = 0x32
, cecSetMenuLanguage = 0x32
, cecClearTimer = 0x33
, cecClearAnalogueTimer = 0x33
, cecSetTimer = 0x34
, cecSetAnalogueTimer = 0x34
, cecTimerStatus = 0x35
, cecStandby = 0x36
, cecStartTimerRecordBlockTransfer = 0x37
, cecTimerRecordBlockTransfer = 0x38
, cecEndTimerRecordBlockTransfer = 0x39




, cecGiveTimerOptions = 0x3E
, cecTimerOptions = 0x3F

, cecPlay = 0x41
, cecDeckControl = 0x42
, cecTimerClearedStatus = 0x43
, cecUserControlPressed = 0x44
, cecUserControlReleased = 0x45
, cecGiveOsdName = 0x46
, cecSetOsdName = 0x47
, cecGiveSystemInfo = 0x52
, cecSetSystemInfoVersionNumber = 0x53
, cecSetOsdString = 0x64


, cecSetTimerProgramTitle = 0x67
, cecSystemAudioModeRequest = 0x70
, cecGiveAudioStatus = 0x71
, cecSetSystemAudioMode = 0x72







, cecReportAudioStatus = 0x7A


, cecGiveSystemAudioModeStatus = 0x7D
, cecSystemAudioModeStatus = 0x7E

, cecRoutingChange = 0x80
, cecRoutingInformation = 0x81
, cecActiveSource = 0x82
, cecGivePhysicalAddress = 0x83
, cecReportPhysicalAddress = 0x84
, cecRequestActiveSource = 0x85
, cecSetStreamPath = 0x86
, cecDeviceVendorId = 0x87

, cecVendorCommand = 0x89
, cecVendorRemoteButtonDown = 0x8A
, cecVendorRemoteButtonUp = 0x8B
, cecGiveDeviceVendorId = 0x8C
, cecMenuRequest = 0x8D
, cecMenuStatus = 0x8E
, cecGiveDevicePowerStatus = 0x8F
, cecGetDevPowerStatus = 0x8F
, cecReportPowerStatus = 0x90
, cecGetMenuLanguage = 0x91
, cecSelectAnalogueService = 0x92
, cecSelectDigitalService = 0x93



, cecSetDigitalTimer = 0x97

, cecClearDigitalTimer = 0x99
, cecSetAudioRate = 0x9A


, cecInactiveSource = 0x9D
, cecCECVersion = 0x9E
, cecGetCECVersion = 0x9F
, cecVendorCommandWithID = 0xA0
, cecClearExternalTimer = 0xA1
, cecSetExternalTimer = 0xA2

, cecPollMessage = 0xAA
, cecAbort = 0xFF
, cecAbortMessage = 0xFF

} CecOpcode;
#define TMDIGADEC_H 
#define tmDigAdec_Version ( "2.4.1" )
#define tmDigAdec_OutBase ( 1 )
#define tmDigAdec_AllNotifications ( 0x000003FF )
#define tmDigAdec_OutputAllNotifications ( 0x00000003 )
#define tmDigAdec_ClipAllNotifications ( 0x0000003F )


typedef enum _tmDigAdec_Notification_t
{
    tmDigAdec_SubscriptionChanged = 0x00000001,
    tmDigAdec_StandardChanged = 0x00000002,
    tmDigAdec_SamplingFreqChanged = 0x00000004,
    tmDigAdec_BitRateChanged = 0x00000008,
    tmDigAdec_NumberBitsPerSampleChanged = 0x00000010,
    tmDigAdec_ChannelsChanged = 0x00000020,
    tmDigAdec_DualChannelChanged = 0x00000040,
    tmDigAdec_BitStreamModeChanged = 0x00000080,
    tmDigAdec_DolbySurroundChanged = 0x00000100,
    tmDigAdec_Error = 0x00000200
} tmDigAdec_Notification_t;

typedef tmDigAdec_Notification_t* ptmDigAdec_Notification_t;

typedef Int32 tmDigAdec_Notifications_t;

typedef tmDigAdec_Notifications_t* ptmDigAdec_Notifications_t;

typedef enum _tmDigAdec_OutputNotification_t
{
    tmDigAdec_OutputSubscriptionChanged = 0x00000001,
    tmDigAdec_OutputConfigChanged = 0x00000002
} tmDigAdec_OutputNotification_t;

typedef tmDigAdec_OutputNotification_t* ptmDigAdec_OutputNotification_t;

typedef Int32 tmDigAdec_OutputNotifications_t;

typedef tmDigAdec_OutputNotifications_t* ptmDigAdec_OutputNotifications_t;

typedef enum _tmDigAdec_CompressionStandard_t
{
    tmDigAdec_NoCompressionStandard = 0x00000000,
    tmDigAdec_Mpeg1Layer1 = 0x00000001,
    tmDigAdec_Mpeg1Layer2 = 0x00000002,
    tmDigAdec_Mpeg1Layer3 = 0x00000004,
    tmDigAdec_Mp3Pro = 0x00000008,
    tmDigAdec_Mpeg2Layer1 = 0x00000010,
    tmDigAdec_Mpeg2Layer2 = 0x00000020,
    tmDigAdec_Mpeg2Layer3 = 0x00000040,
    tmDigAdec_Mpeg2_5Layer3 = 0x00000080,
    tmDigAdec_Mpeg2Aac = 0x00000100,
    tmDigAdec_Mpeg4Aac = 0x00000200,
    tmDigAdec_Mpeg4Celp = 0x00000400,
    tmDigAdec_AacPlus = 0x00000800,
    tmDigAdec_Ac3 = 0x00001000,
    tmDigAdec_Dts = 0x00002000,
    tmDigAdec_Dv = 0x00004000,
    tmDigAdec_Pcm = 0x00008000,
    tmDigAdec_InvalidCompressionStandard = 0x00010000
} tmDigAdec_CompressionStandard_t;

typedef tmDigAdec_CompressionStandard_t* ptmDigAdec_CompressionStandard_t;

typedef Int32 tmDigAdec_CompressionStandardSet_t;

typedef tmDigAdec_CompressionStandardSet_t* ptmDigAdec_CompressionStandardSet_t;

typedef enum _tmDigAdec_SamplingFrequency_t
{
    tmDigAdec_SamplingFrequencyUnknown = 0x00000000,
    tmDigAdec_8k = 0x00000001,
    tmDigAdec_11k025 = 0x00000002,
    tmDigAdec_12k = 0x00000004,
    tmDigAdec_16k = 0x00000008,
    tmDigAdec_22k05 = 0x00000010,
    tmDigAdec_24k = 0x00000020,
    tmDigAdec_32k = 0x00000040,
    tmDigAdec_44k1 = 0x00000080,
    tmDigAdec_48k = 0x00000100,
    tmDigAdec_64k = 0x00000200,
    tmDigAdec_88k2 = 0x00000400,
    tmDigAdec_96k = 0x00000800,
    tmDigAdec_128k = 0x00001000,
    tmDigAdec_176k4 = 0x00002000,
    tmDigAdec_192k = 0x00004000
} tmDigAdec_SamplingFrequency_t;

typedef tmDigAdec_SamplingFrequency_t* ptmDigAdec_SamplingFrequency_t;

typedef Int32 tmDigAdec_SamplingFrequencySet_t;

typedef tmDigAdec_SamplingFrequencySet_t* ptmDigAdec_SamplingFrequencySet_t;

typedef enum _tmDigAdec_StereoInfo_t
{
    tmDigAdec_NotStereo = 0x00000000,
    tmDigAdec_Stereo = 0x00000001,
    tmDigAdec_JointStereo = 0x00000002,
    tmDigAdec_LcStereo = 0x00000003
} tmDigAdec_StereoInfo_t;

typedef tmDigAdec_StereoInfo_t* ptmDigAdec_StereoInfo_t;

typedef enum _tmDigAdec_DualChannelInfo_t
{
    tmDigAdec_NoDualChannel = 0x00000000,
    tmDigAdec_DualMonoChannel = 0x00000001,
    tmDigAdec_DualStereoChannel = 0x00000002
} tmDigAdec_DualChannelInfo_t;

typedef tmDigAdec_DualChannelInfo_t* ptmDigAdec_DualChannelInfo_t;

typedef enum _tmDigAdec_DolbySurroundInfo_t
{
    tmDigAdec_DolbyNotIndicated = 0x00000001,
    tmDigAdec_NotDolbySurround = 0x00000002,
    tmDigAdec_DolbySurround = 0x00000003
} tmDigAdec_DolbySurroundInfo_t;

typedef tmDigAdec_DolbySurroundInfo_t* ptmDigAdec_DolbySurroundInfo_t;

typedef enum _tmDigAdec_Error_t
{
    tmDigAdec_SyncNotFound = 0x00000001,
    tmDigAdec_InvalidHeader = 0x00000002,
    tmDigAdec_DecodeFrame = 0x00000004,
    tmDigAdec_CrcFailed = 0x00000008,
    tmDigAdec_Underrun = 0x00000010
} tmDigAdec_Error_t;

typedef tmDigAdec_Error_t* ptmDigAdec_Error_t;

typedef enum _tmDigAdec_OutputConfiguration_t
{
    tmDigAdec_ConfigNotAvailable = 0x00000000,
    tmDigAdec_Audio1x0 = 0x00000001,
    tmDigAdec_Audio2x0 = 0x00000002,
    tmDigAdec_AudioDolbySurround = 0x00000004,
    tmDigAdec_Audio3x0 = 0x00000008,
    tmDigAdec_Audio2x1 = 0x00000010,
    tmDigAdec_Audio3x1 = 0x00000020,
    tmDigAdec_Audio2x2 = 0x00000040,
    tmDigAdec_Audio3x2 = 0x00000080,
    tmDigAdec_Audio5x2 = 0x00000100
} tmDigAdec_OutputConfiguration_t;

typedef tmDigAdec_OutputConfiguration_t* ptmDigAdec_OutputConfiguration_t;

typedef Int32 tmDigAdec_OutputConfigurationSet_t;

typedef tmDigAdec_OutputConfigurationSet_t* ptmDigAdec_OutputConfigurationSet_t;

typedef enum _tmDigAdec_DualMonoMode_t
{
    tmDigAdec_DualMonoModeNotAvailable = 0x00000001,
    tmDigAdec_ChannelsStereo = 0x00000002,
    tmDigAdec_Channel1Mono = 0x00000004,
    tmDigAdec_Channel2Mono = 0x00000008,
    tmDigAdec_ChannelsMixedMono = 0x00000010
} tmDigAdec_DualMonoMode_t;

typedef tmDigAdec_DualMonoMode_t* ptmDigAdec_DualMonoMode_t;

typedef Int32 tmDigAdec_DualMonoModeSet_t;

typedef tmDigAdec_DualMonoModeSet_t* ptmDigAdec_DualMonoModeSet_t;

typedef enum _tmDigAdec_DualStereoMode_t
{
    tmDigAdec_DualStereoModeNotAvailable = 0x00000001,
    tmDigAdec_DualStereoSetSeparated = 0x00000002,
    tmDigAdec_DualStereoSet1 = 0x00000004,
    tmDigAdec_DualStereoSet2 = 0x00000008,
    tmDigAdec_DualStereoSetMixed = 0x00000010
} tmDigAdec_DualStereoMode_t;

typedef tmDigAdec_DualStereoMode_t* ptmDigAdec_DualStereoMode_t;

typedef Int32 tmDigAdec_DualStereoModeSet_t;

typedef tmDigAdec_DualStereoModeSet_t* ptmDigAdec_DualStereoModeSet_t;

typedef enum _tmDigAdec_DynRangeCompMode_t
{
    tmDigAdec_NoDrcMode = 0x00000001,
    tmDigAdec_CustomMode0 = 0x00000002,
    tmDigAdec_CustomMode1 = 0x00000004,
    tmDigAdec_LineMode = 0x00000008,
    tmDigAdec_RfMode = 0x00000010
} tmDigAdec_DynRangeCompMode_t;

typedef tmDigAdec_DynRangeCompMode_t* ptmDigAdec_DynRangeCompMode_t;

typedef Int32 tmDigAdec_DynRangeCompModeSet_t;

typedef tmDigAdec_DynRangeCompModeSet_t* ptmDigAdec_DynRangeCompModeSet_t;

typedef enum _tmDigAdec_DynRangeCompSetting_t
{
    tmDigAdec_DynamicRangeCompOff = 0x00000001,
    tmDigAdec_StandardDynamicRange = 0x00000002,
    tmDigAdec_MinimumDynamicRange = 0x00000004,
    tmDigAdec_TvStyleDynamicRange = 0x00000008
} tmDigAdec_DynRangeCompSetting_t;

typedef tmDigAdec_DynRangeCompSetting_t* ptmDigAdec_DynRangeCompSetting_t;

typedef Int32 tmDigAdec_DynRangeCompSettingSet_t;

typedef tmDigAdec_DynRangeCompSettingSet_t* ptmDigAdec_DynRangeCompSettingSet_t;

typedef enum _tmDigAdec_ClipNotification_t
{
    tmDigAdec_ClipSubscriptionChanged = 0x00000001,
    tmDigAdec_ClipBufferAvailable = 0x00000002,
    tmDigAdec_ClipEndOfData = 0x00000004,
    tmDigAdec_ClipStarted = 0x00000008,
    tmDigAdec_ClipStopped = 0x00000010,
    tmDigAdec_ClipPaused = 0x00000020
} tmDigAdec_ClipNotification_t;

typedef tmDigAdec_ClipNotification_t* ptmDigAdec_ClipNotification_t;

typedef Int32 tmDigAdec_ClipNotifications_t;

typedef tmDigAdec_ClipNotifications_t* ptmDigAdec_ClipNotifications_t;

typedef enum _tmDigAdec_ClipDecodingState_t
{
    tmDigAdec_ClipDecodingStarting = 0,
    tmDigAdec_ClipDecodingStarted = 1,
    tmDigAdec_ClipDecodingStopping = 2,
    tmDigAdec_ClipDecodingStopped = 3,
    tmDigAdec_ClipDecodingPausing = 4,
    tmDigAdec_ClipDecodingPaused = 5
} tmDigAdec_ClipDecodingState_t;

typedef tmDigAdec_ClipDecodingState_t* ptmDigAdec_ClipDecodingState_t;
#define TMPLFINSTPRESET_H 
#define tmPlfInstPreset_Version ( 1.1 )
#define tmPlfInstPreset_AttrUnknown ( 0x00000000 )
#define tmPlfInstPreset_AttrInvalid ( 0x00000001 )
#define tmPlfInstPreset_AttrPal ( 0x00000002 )
#define tmPlfInstPreset_AttrSecam ( 0x00000003 )
#define tmPlfInstPreset_AttrNtsc ( 0x00000004 )
#define tmPlfInstPreset_AttrNone ( 0x00000001 )
#define tmPlfInstPreset_AttrBg ( 0x00000002 )
#define tmPlfInstPreset_AttrD ( 0x00000004 )
#define tmPlfInstPreset_AttrDk ( 0x00000008 )
#define tmPlfInstPreset_AttrDk1 ( 0x00000010 )
#define tmPlfInstPreset_AttrDk2 ( 0x00000020 )
#define tmPlfInstPreset_AttrI ( 0x00000040 )
#define tmPlfInstPreset_AttrH ( 0x00000080 )
#define tmPlfInstPreset_AttrK1 ( 0x00000100 )
#define tmPlfInstPreset_AttrL ( 0x00000200 )
#define tmPlfInstPreset_AttrL1 ( 0x00000400 )
#define tmPlfInstPreset_AttrM ( 0x00000800 )
#define tmPlfInstPreset_AttrN ( 0x00001000 )
#define tmPlfInstPreset_AttrC64Qam ( 0x00002000 )
#define tmPlfInstPreset_AttrC256Qam ( 0x00004000 )
#define tmPlfInstPreset_AttrT8Vsb ( 0x00008000 )
#define tmPlfInstPreset_AttrT16Vsb ( 0x00010000 )
#define tmPlfInstPreset_IoAllNotifications ( 0x00000007 )
#define tmPlfInstPreset_tvSourceBits ( 0x0000FFFF )
#define tmPlfInstPreset_tvAvSourceBits ( 0x000083FF )
#define tmPlfInstPreset_tvTunerSourceBits ( 0x00004000 )
#define tmPlfInstPreset_tvTunerSourceDesignatorBits ( 0x00030000 )
#define tmPlfInstPreset_tvVideoSourceDesignatorBits ( 0xFF000000 )
#define tmPlfInstPreset_tvSoundSourceDesignatorBits ( 0x007C0000 )
#define tmPlfInstPreset_tvsrcIsDeviceMask ( 0x000F )
#define tmPlfInstPreset_tvsrcP50InfoMask ( 0x0FF0 )
#define tmPlfInstPreset_tvsrcP50VideoMask ( 0x00F0 )
#define tmPlfInstPreset_tvsrcP50AudioMask ( 0x0F00 )
#define tmPlfInstPreset_tvsrcOverlayMask ( 0x7000 )


typedef enum _tmPlfInstPreset_IoNotification_t
{
    tmPlfInstPreset_IoSubscriptionChanged = 0x00000001,
    tmPlfInstPreset_IoExtensionStatuschanged = 0x00000002,
    tmPlfInstPreset_IoDestinationStatusChanged = 0x00000004
} tmPlfInstPreset_IoNotification_t;

typedef tmPlfInstPreset_IoNotification_t* ptmPlfInstPreset_IoNotification_t;

typedef Int32 tmPlfInstPreset_IoNotifications_t;

typedef tmPlfInstPreset_IoNotifications_t* ptmPlfInstPreset_IoNotifications_t;

typedef enum _tmPlfInstPreset_Source_t
{
    tmPlfInstPreset_tvsrcNoSource = 0x00000000,
    tmPlfInstPreset_tvsrcAvIn0 = 0x00000004,
    tmPlfInstPreset_tvsrcAvIn1 = 0x00000008,
    tmPlfInstPreset_tvsrcAvIn2 = 0x00000010,
    tmPlfInstPreset_tvsrcAvIn3 = 0x00000020,
    tmPlfInstPreset_tvsrcAvIn4 = 0x00000040,
    tmPlfInstPreset_tvsrcAvIn5 = 0x00000080,
    tmPlfInstPreset_tvsrcAvIn6 = 0x00000100,
    tmPlfInstPreset_tvsrcAvIn7 = 0x00000200,
    tmPlfInstPreset_tvsrcSpdif = 0x00000400,
    tmPlfInstPreset_tvsrcCenter = 0x00000800,
    tmPlfInstPreset_tvsrcDvd = 0x00001000,
    tmPlfInstPreset_tvsrcTxt = 0x00002000,
    tmPlfInstPreset_tvsrcSpdif2 = 0x00002001,
    tmPlfInstPreset_tvsrcHdd = 0x00002002,
    tmPlfInstPreset_tvsrcRadio = 0x00002003,
    tmPlfInstPreset_tvsrcAvMediaPlayer = 0x00002004,
    tmPlfInstPreset_tvsrcMainTuner = 0x00004001,
    tmPlfInstPreset_tvsrcAuxTuner = 0x00004002,
    tmPlfInstPreset_tvsrcAvIn8 = 0x00008000,
    tmPlfInstPreset_tvSourceCVBS = 0x80000000,
    tmPlfInstPreset_tvSourceYC = 0x40000000,
    tmPlfInstPreset_tvSourceYUV = 0x20000000,
    tmPlfInstPreset_tvSourceRGB = 0x10000000,
    tmPlfInstPreset_tvSourceVGA = 0x08000000,
    tmPlfInstPreset_tvSourceYUV2FH = 0x04000000,
    tmPlfInstPreset_tvSourceRGB2FH = 0x02000000,
    tmPlfInstPreset_tvSourceVideoDigital = 0x01000000,
    tmPlfInstPreset_tvSourceLeftRight = 0x00400000,
    tmPlfInstPreset_tvSourceAudioDigital = 0x00200000,
    tmPlfInstPreset_tvSourceAudioSpdif = 0x00100000,
    tmPlfInstPreset_tvSourceAudioCenter = 0x00080000,
    tmPlfInstPreset_tvSourceText = 0x00040000,
    tmPlfInstPreset_tvSourceTerrestrial = 0x00020000,
    tmPlfInstPreset_tvSourceCable = 0x00010000
} tmPlfInstPreset_Source_t;

typedef tmPlfInstPreset_Source_t* ptmPlfInstPreset_Source_t;

typedef enum _tmPlfInstPreset_SourceType_t
{
    tmPlfInstPreset_tvsrcIsUnknown = 0x0000,
    tmPlfInstPreset_tvsrcIsVcr = 0x0001,
    tmPlfInstPreset_tvsrcIsDvd = 0x0002,
    tmPlfInstPreset_tvsrcIsHomeCinema = 0x0003,
    tmPlfInstPreset_tvsrcIsTxt = 0x0004,
    tmPlfInstPreset_tvsrcIsEpg = 0x0005,
    tmPlfInstPreset_tvsrcIsGemstar = 0x0006,
    tmPlfInstPreset_tvsrcIsOsd = 0x0007,
    tmPlfInstPreset_tvsrcIsDigStb = 0x0008,
    tmPlfInstPreset_tvsrcIsDecoder = 0x0009,
    tmPlfInstPreset_tvsrcIsNone = 0x000F,
    tmPlfInstPreset_tvsrcP50Ar4by3FullFormat = 0x0010,
    tmPlfInstPreset_tvsrcP50Ar14by9FullFormat = 0x0020,
    tmPlfInstPreset_tvsrcP50Ar16by9FullFormat = 0x0030,
    tmPlfInstPreset_tvsrcP50Ar14by9Top = 0x0040,
    tmPlfInstPreset_tvsrcP50Ar14by9Center = 0x0050,
    tmPlfInstPreset_tvsrcP50Ar16by9Top = 0x0060,
    tmPlfInstPreset_tvsrcP50Ar16by9Center = 0x0070,
    tmPlfInstPreset_tvsrcP50ArMoreThan16by9Center = 0x0080,
    tmPlfInstPreset_tvsrcP50Mono = 0x0100,
    tmPlfInstPreset_tvsrcP50Stereo = 0x0200,
    tmPlfInstPreset_tvsrcP50Dpl = 0x0300,
    tmPlfInstPreset_tvsrcP50Unknown = 0x0000,
    tmPlfInstPreset_tvsrcOverlayHalfWindow = 0x1000,
    tmPlfInstPreset_tvsrcOverlayFullWindow = 0x2000,
    tmPlfInstPreset_tvsrcOverlayTransparent = 0x4000,

} tmPlfInstPreset_SourceType_t;

typedef tmPlfInstPreset_SourceType_t* ptmPlfInstPreset_SourceType_t;

typedef enum _tmPlfInstPreset_Destination_t
{
    tmPlfInstPreset_tvdestNoDestination = 0x00000000,
    tmPlfInstPreset_tvdestMainWindow = 0x00000001,
    tmPlfInstPreset_tvdestAuxWindow = 0x00000002,
    tmPlfInstPreset_tvdestSpeakers = 0x00000003,
    tmPlfInstPreset_tvdestHeadphones = 0x00000004,
    tmPlfInstPreset_tvdestVbiData = 0x00000005,
    tmPlfInstPreset_tvdestAvOut1 = 0x00000006,
    tmPlfInstPreset_tvdestAvOut2 = 0x00000007,
    tmPlfInstPreset_tvdestLrCinch = 0x00000008,
    tmPlfInstPreset_tvdestWireless = 0x00000009,
    tmPlfInstPreset_tvdestSpdif = 0x0000000A,
    tmPlfInstPreset_tvdestHdd = 0x0000000B,
    tmPlfInstPreset_tvdest1394 = 0x0000000C,
    tmPlfInstPreset_tvDestinationCVBS = 0x80000000,
    tmPlfInstPreset_tvDestinationYC = 0x40000000,
    tmPlfInstPreset_tvDestinationLeftRightTotal = 0x04000000,
    tmPlfInstPreset_tvDestinationCentre = 0x02000000,
    tmPlfInstPreset_tvDestinationSubWoofer = 0x01000000,
    tmPlfInstPreset_tvDestinationLeftRight = 0x00800000,
    tmPlfInstPreset_tvDestinationForcedMono = 0x00400000,
    tmPlfInstPreset_tvDestinationSpdif = 0x00200000,
    tmPlfInstPreset_tvDestinationSurroundLeft = 0x00100000,
    tmPlfInstPreset_tvDestinationSurroundRight = 0x00010000,
    tmPlfInstPreset_tvDestinationBits = 0x000000FF,
    tmPlfInstPreset_tvVideoDestinationDesignatorBits = 0xC0000000,
    tmPlfInstPreset_tvSoundDestinationDesignatorBits = 0x07F10000,
    tmPlfInstPreset_tvDesignatorBits = 0xFFFF0000,
    tmPlfInstPreset_tvTunerDesignatorBits = 0x00040000,
    tmPlfInstPreset_tvVideoDesignatorBits = 0xFF800000,
    tmPlfInstPreset_tvSoundDesignatorBits = 0x07FF0000
} tmPlfInstPreset_Destination_t;

typedef tmPlfInstPreset_Destination_t* ptmPlfInstPreset_Destination_t;

typedef enum _tmPlfInstPreset_DestinationType_t
{
    tmPlfInstPreset_tvdestIsNotSupported = 0,
    tmPlfInstPreset_tvdestIsConstantLevel = 1,
    tmPlfInstPreset_tvdestIsVariableLevel = 2
} tmPlfInstPreset_DestinationType_t;

typedef tmPlfInstPreset_DestinationType_t* ptmPlfInstPreset_DestinationType_t;

typedef enum _tmPlfInstPreset_PresetAttribute_t
{
    tmPlfInstPreset_OnStationFound = 0,
    tmPlfInstPreset_TunerFrequency = 1,
    tmPlfInstPreset_Afc = 3,
    tmPlfInstPreset_TvSystem = 4,
    tmPlfInstPreset_SuggestedColorSystem = 5,
    tmPlfInstPreset_RfAmplification = 9,
} tmPlfInstPreset_PresetAttribute_t;

typedef tmPlfInstPreset_PresetAttribute_t* ptmPlfInstPreset_PresetAttribute_t;
#define TMFE_H 
#define tmFe_Version ( "1.1.2" )
#define TM_ERR_FE_NO_CHAN ( 0x00000801 )
#define TM_ERR_FE_NO_FREQ ( 0x00000802 )
#define tmFe_NoOffset ( 0x00000000 )
#define tmFe_AutoOffset ( 0xFFFFFFFF )
#define tmFe_NoRfAmp ( 0x00000000 )
#define tmFe_TuningAllNotifications ( 0x000001FF )
#define tmFe_SigStrengthMeasAllNotifications ( 0x00000007 )
#define tmFe_DigDemodDecodeAllNotifications ( 0x00000007 )
#define tmFe_ChanDecAllNotifications ( 0x000003FF )
#define tmFe_ASigStrengthMeasAllNotifications ( 0x00000007 )
#define tmFe_QSigStrengthMeasAllNotifications ( 0x00000007 )


typedef enum _tmFe_TuningNotification_t
{
    tmFe_TuningSubscriptionChanged = 0x00000001,
    tmFe_TuningSearchStateChanged = 0x00000002,
    tmFe_TuningSearchInProgress = 0x00000004,
    tmFe_TuningStationFound = 0x00000008,
    tmFe_TuningStationNotFound = 0x00000010,
    tmFe_TuningTvSystemDetected = 0x00000020,
    tmFe_TuningAfcFreqChanged = 0x00000040,
    tmFe_TuningAfcLimitReached = 0x00000080,
    tmFe_TuningAfcModeChanged = 0x00000100
} tmFe_TuningNotification_t;

typedef tmFe_TuningNotification_t* ptmFe_TuningNotification_t;

typedef Int32 tmFe_TuningNotifications_t;

typedef tmFe_TuningNotifications_t* ptmFe_TuningNotifications_t;

typedef enum _tmFe_SigStrengthMeasNotification_t
{
    tmFe_SigStrengthMeasSubscriptionChanged = 0x00000001,
    tmFe_SigStrengthMeasReady = 0x00000002,
    tmFe_SigStrengthMeasAborted = 0x00000004
} tmFe_SigStrengthMeasNotification_t;

typedef tmFe_SigStrengthMeasNotification_t* ptmFe_SigStrengthMeasNotification_t;

typedef Int32 tmFe_SigStrengthMeasNotifications_t;

typedef tmFe_SigStrengthMeasNotifications_t* ptmFe_SigStrengthMeasNotifications_t;

typedef enum _tmFe_DigDemodDecodeNotification_t
{
    tmFe_DigDemodDecodeSubscriptionChanged = 0x00000001,
    tmFe_DigDemodDecodeBerBelowThreshold = 0x00000002,
    tmFe_DigDemodDecodeBerAboveThreshold = 0x00000004
} tmFe_DigDemodDecodeNotification_t;

typedef tmFe_DigDemodDecodeNotification_t* ptmFe_DigDemodDecodeNotification_t;

typedef Int32 tmFe_DigDemodDecodeNotifications_t;

typedef tmFe_DigDemodDecodeNotifications_t* ptmFe_DigDemodDecodeNotifications_t;

typedef enum _tmFe_TvSystem_t
{
    tmFe_TvSystemNone = 0x00000001,
    tmFe_TvSystemBg = 0x00000002,
    tmFe_TvSystemD = 0x00000004,
    tmFe_TvSystemDk = 0x00000008,
    tmFe_TvSystemDk1 = 0x00000010,
    tmFe_TvSystemDk2 = 0x00000020,
    tmFe_TvSystemI = 0x00000040,
    tmFe_TvSystemH = 0x00000080,
    tmFe_TvSystemK1 = 0x00000100,
    tmFe_TvSystemL = 0x00000200,
    tmFe_TvSystemL1 = 0x00000400,
    tmFe_TvSystemM = 0x00000800,
    tmFe_TvSystemN = 0x00001000,
    tmFe_TvSystemC64Qam = 0x00002000,
    tmFe_TvSystemC256Qam = 0x00004000,
    tmFe_TvSystemT8Vsb = 0x00008000,
    tmFe_TvSystemT16Vsb = 0x00010000
} tmFe_TvSystem_t;

typedef tmFe_TvSystem_t* ptmFe_TvSystem_t;

typedef Nat32 tmFe_TvSystemSet_t;

typedef tmFe_TvSystemSet_t* ptmFe_TvSystemSet_t;

typedef enum _tmFe_SearchMode_t
{
    tmFe_SmDefault = 0x00000001,
    tmFe_SmChanTable = 0x00000002,
    tmFe_SmPositiveModulation = 0x00000004,
    tmFe_SmCurrentIncluded = 0x00000008,
    tmFe_SmDirectionDown = 0x00000010,
    tmFe_SmFinetuneIncluded = 0x00000020,
    tmFe_SmTvSystemIncluded = 0x00000040,
    tmFe_SmColorSystemIncluded = 0x00000080,
    tmFe_SmSoundSystemIncluded = 0x00000100
} tmFe_SearchMode_t;

typedef tmFe_SearchMode_t* ptmFe_SearchMode_t;

typedef Nat32 tmFe_SearchModeSet_t;

typedef tmFe_SearchModeSet_t* ptmFe_SearchModeSet_t;

typedef enum _tmFe_SearchState_t
{
    tmFe_SsNone = 0x00000001,
    tmFe_SsSearching = 0x00000002,
    tmFe_SsFinetune = 0x00000004,
    tmFe_SsTvSystemDecoding = 0x00000008,
    tmFe_SsColorSystemDecoding = 0x00000010,
    tmFe_SsSoundSystemDecoding = 0x00000020
} tmFe_SearchState_t;

typedef tmFe_SearchState_t* ptmFe_SearchState_t;

typedef Nat32 tmFe_SearchStateSet_t;

typedef tmFe_SearchStateSet_t* ptmFe_SearchStateSet_t;

typedef enum _tmFe_AfcValue_t
{
    tmFe_FreqUnknown = 0x00000001,
    tmFe_FreqFarBelowIf = 0x00000002,
    tmFe_FreqBelowIf = 0x00000004,
    tmFe_FreqAboveIf = 0x00000008,
    tmFe_FreqFarAboveIf = 0x00000010
} tmFe_AfcValue_t;

typedef tmFe_AfcValue_t* ptmFe_AfcValue_t;

typedef enum _tmFe_DataRate_t
{
    tmFe_UnknownDataRate = 0x00000001,
    tmFe_Dvs167_1544 = 0x00000002,
    tmFe_Dvs178_2048 = 0x00000004,
    tmFe_Dvs167_3088 = 0x00000008,
    tmFe_Dvs167_1544Inv = 0x00000010,
    tmFe_Dvs178_2048Inv = 0x00000020,
    tmFe_Dvs167_3088Inv = 0x00000040
} tmFe_DataRate_t;

typedef tmFe_DataRate_t* ptmFe_DataRate_t;

typedef Nat32 tmFe_DataRateSet_t;

typedef tmFe_DataRateSet_t* ptmFe_DataRateSet_t;

typedef enum _tmFe_ChanTable_t
{
    tmFe_UnknownTable = 0x00000001,
    tmFe_TableApAntenna = 0x00000002,
    tmFe_TableApCable = 0x00000004,
    tmFe_TableEuropeS = 0x00000008,
    tmFe_TableEuropeC = 0x00000010,
    tmFe_TableUsCable = 0x00000020,
    tmFe_TableUsAntenna = 0x00000040,
    tmFe_TableUsAfn = 0x00000080,
    tmFe_TableEuropeDkS = 0x00000100,
    tmFe_TableEuropeDkC = 0x00000200,
    tmFe_TableEuropeIS = 0x00000400,
    tmFe_TableEuropeIC = 0x00000800,
    tmFe_TableEuropeLS = 0x00001000,
    tmFe_TableEuropeLC = 0x00002000,
    tmFe_TableBrasilAntenna = 0x00004000
} tmFe_ChanTable_t;

typedef tmFe_ChanTable_t* ptmFe_ChanTable_t;

typedef Nat32 tmFe_ChanTableSet_t;

typedef tmFe_ChanTableSet_t* ptmFe_ChanTableSet_t;

typedef enum _tmFe_Constellation_t
{
    tmFe_UnknownConstellation = 0x00000001,
    tmFe_Bpsk = 0x00000002,
    tmFe_Qpsk = 0x00000004,
    tmFe_8psk = 0x00000008,
    tmFe_Qam4 = 0x00000010,
    tmFe_Qam16 = 0x00000020,
    tmFe_Qam64 = 0x00000040,
    tmFe_Qam128 = 0x00000080,
    tmFe_Qam256 = 0x00000100,
    tmFe_Qam512 = 0x00000200,
    tmFe_Qam1024 = 0x00000400,
    tmFe_Vsb8 = 0x00000800,
    tmFe_Vsb16 = 0x00001000,
    tmFe_AutoConstellation = 0x00002000,
    tmFe_InvalidConstellation = 0x00004000,
    tmFe_Qam32 = 0x00008000,
    tmFe_DQpsk = 0x00010000
} tmFe_Constellation_t;

typedef tmFe_Constellation_t* ptmFe_Constellation_t;

typedef Nat32 tmFe_ConstellationSet_t;

typedef tmFe_ConstellationSet_t* ptmFe_ConstellationSet_t;

typedef enum _tmFe_IqMode_t
{
    tmFe_UnknownIqMode = 0x00000001,
    tmFe_IqModeNormal = 0x00000002,
    tmFe_IqModeInverse = 0x00000004,
    tmFe_IqModeAuto = 0x00000008,
    tmFe_InvalidIqMode = 0x00000010
} tmFe_IqMode_t;

typedef tmFe_IqMode_t* ptmFe_IqMode_t;

typedef Nat32 tmFe_IqModeSet_t;

typedef tmFe_IqModeSet_t* ptmFe_IqModeSet_t;

typedef enum _tmFe_CodeRate_t
{
    tmFe_CodeRateInvalid = 0x80000000,
    tmFe_CodeRateUnknown = 0x40000000,
    tmFe_CodeRateAuto = 0x00000001,
    tmFe_CodeRateNone = 0x00000002,
    tmFe_CodeRate1_2 = 0x00000004,
    tmFe_CodeRate2_3 = 0x00000008,
    tmFe_CodeRate3_4 = 0x00000010,
    tmFe_CodeRate5_6 = 0x00000020,
    tmFe_CodeRate6_7 = 0x00000040,
    tmFe_CodeRate7_8 = 0x00000080,
    tmFe_CodeRate8_9 = 0x00000100
} tmFe_CodeRate_t;

typedef tmFe_CodeRate_t* ptmFe_CodeRate_t;

typedef Int32 tmFe_CodeRateSet_t;

typedef tmFe_CodeRateSet_t* ptmFe_CodeRateSet_t;

typedef enum _tmFe_HierarchyMode_t
{
    tmFe_HierarchyModeInvalid = 0x80000000,
    tmFe_HierarchyModeUnknown = 0x40000000,
    tmFe_HierarchyModeAuto = 0x00000001,
    tmFe_HierarchyAlpha1 = 0x00000002,
    tmFe_HierarchyAlpha2 = 0x00000004,
    tmFe_HierarchyAlpha4 = 0x00000008,
    tmFe_HierarchyNone = 0x00000010
} tmFe_HierarchyMode_t;

typedef tmFe_HierarchyMode_t* ptmFe_HierarchyMode_t;

typedef Int32 tmFe_HierarchyModeSet_t;

typedef tmFe_HierarchyModeSet_t* ptmFe_HierarchyModeSet_t;

typedef enum _tmFe_GuardInterval_t
{
    tmFe_GuardIntervalInvalid = 0x80000000,
    tmFe_GuardIntervalUnknown = 0x40000000,
    tmFe_GuardIntervalAuto = 0x00000001,
    tmFe_GuardInterval1_32 = 0x00000002,
    tmFe_GuardInterval1_16 = 0x00000004,
    tmFe_GuardInterval1_8 = 0x00000008,
    tmFe_GuardInterval1_4 = 0x00000010
} tmFe_GuardInterval_t;

typedef tmFe_GuardInterval_t* ptmFe_GuardInterval_t;

typedef Int32 tmFe_GuardIntervalSet_t;

typedef tmFe_GuardIntervalSet_t* ptmFe_GuardIntervalSet_t;

typedef enum _tmFe_TxMode_t
{
    tmFe_TxModeInvalid = 0x80000000,
    tmFe_TxModeUnknown = 0x40000000,
    tmFe_TxModeAuto = 0x00000001,
    tmFe_TxMode2k = 0x00000002,
    tmFe_TxMode8k = 0x00000004
} tmFe_TxMode_t;

typedef tmFe_TxMode_t* ptmFe_TxMode_t;

typedef Int32 tmFe_TxModeSet_t;

typedef tmFe_TxModeSet_t* ptmFe_TxModeSet_t;

typedef enum _tmFe_ChanBandwidth_t
{
    tmFe_ChanBandwidthInvalid = 0x80000000,
    tmFe_ChanBandwidthUnknown = 0x40000000,
    tmFe_ChanBandwidthAuto = 0x00000001,
    tmFe_ChanBandwidth6MHz = 0x00000002,
    tmFe_ChanBandwidth7MHz = 0x00000004,
    tmFe_ChanBandwidth8MHz = 0x00000008
} tmFe_ChanBandwidth_t;

typedef tmFe_ChanBandwidth_t* ptmFe_ChanBandwidth_t;

typedef Int32 tmFe_ChanBandwidthSet_t;

typedef tmFe_ChanBandwidthSet_t* ptmFe_ChanBandwidthSet_t;

typedef struct _tmFe_SearchSymbolRate_t
{
    tmFe_Constellation_t constellation;
    UInt32 rate;
} tmFe_SearchSymbolRate_t;

typedef tmFe_SearchSymbolRate_t* ptmFe_SearchSymbolRate_t;

typedef enum _tmFe_ChanDecNotification_t
{
    tmFe_ChanDecSubscriptionChanged = 0x00000001,
    tmFe_ChanDecConstellationDetected = 0x00000002,
    tmFe_ChanDecIqModeDetected = 0x00000004,
    tmFe_ChanDecCarrierPresentChanged = 0x00000008,
    tmFe_ChanDecBerThresholdCrossed = 0x00000010,
    tmFe_ChanDecCodeRateDetected = 0x00000020,
    tmFe_ChanDecHierarchyModeDetected = 0x00000040,
    tmFe_ChanDecGuardIntervalDetected = 0x00000080,
    tmFe_ChanDecTxModeDetected = 0x00000100,
    tmFe_ChanDecChannelBandwidthDetected = 0x00000200
} tmFe_ChanDecNotification_t;

typedef tmFe_ChanDecNotification_t* ptmFe_ChanDecNotification_t;

typedef Int32 tmFe_ChanDecNotifications_t;

typedef tmFe_ChanDecNotifications_t* ptmFe_ChanDecNotifications_t;

typedef enum _tmFe_ASigStrengthMeasNotification_t
{
    tmFe_ASigStrengthMeasSubscriptionChanged = 0x00000001,
    tmFe_ASigStrengthMeasReady = 0x00000002,
    tmFe_ASigStrengthMeasAborted = 0x00000004
} tmFe_ASigStrengthMeasNotification_t;

typedef tmFe_ASigStrengthMeasNotification_t* ptmFe_ASigStrengthMeasNotification_t;

typedef Int32 tmFe_ASigStrengthMeasNotifications_t;

typedef tmFe_ASigStrengthMeasNotifications_t* ptmFe_ASigStrengthMeasNotifications_t;

typedef enum _tmFe_QSigStrengthMeasNotification_t
{
    tmFe_QSigStrengthMeasSubscriptionChanged = 0x00000001,
    tmFe_QSigStrengthMeasReady = 0x00000002,
    tmFe_QSigStrengthMeasAborted = 0x00000004
} tmFe_QSigStrengthMeasNotification_t;

typedef tmFe_QSigStrengthMeasNotification_t* ptmFe_QSigStrengthMeasNotification_t;

typedef Int32 tmFe_QSigStrengthMeasNotifications_t;

typedef tmFe_QSigStrengthMeasNotifications_t* ptmFe_QSigStrengthMeasNotifications_t;

typedef enum _tmFe_StreamPriority_t
{
    tmFe_StreamPriorityHigh = 0x00000001,
    tmFe_StreamPriorityLow = 0x00000002
} tmFe_StreamPriority_t;

typedef tmFe_StreamPriority_t* ptmFe_StreamPriority_t;

typedef enum _tmFe_FrequencyOffset_t
{
    tmFe_OffsetNone = 0x00000001,
    tmFe_OffsetInvalid = 0x00000002,
    tmFe_OffsetUnknown = 0x00000004,
    tmFe_OffsetAuto = 0x00000008,
    tmFe_OffsetP1d6MHz = 0x00000010,
    tmFe_OffsetP2d6MHz = 0x00000020,
    tmFe_OffsetP3d6MHz = 0x00000040,
    tmFe_OffsetM1d6MHz = 0x00000080,
    tmFe_OffsetM2d6MHz = 0x00000100,
    tmFe_OffsetM3d6MHz = 0x00000200
} tmFe_FrequencyOffset_t;

typedef tmFe_FrequencyOffset_t* ptmFe_FrequencyOffset_t;

typedef Int32 tmFe_FrequencyOffsetSet_t;

typedef tmFe_FrequencyOffsetSet_t* ptmFe_FrequencyOffsetSet_t;
#define DPLFAPICMDMX_DD 






typedef Nat8* tmCmDmx_Buffer_t;
#define TMCMDMX_H 
#define tmCmDmx_Version ( "1.0" )
#define TM_ERR_CMDMX_CLIENTSTR_NOT_AVAILABLE ( 0x00000801 )
#define TM_ERR_CMDMX_PLATFSTRTYPE_NOT_SUPP ( 0x00000802 )
#define tmCmDmx_AllNotifications ( 0x00000003 )
#define tmCmDmx_StreamBufferAllNotifications ( 0x00000007 )


typedef enum _tmCmDmx_Notification_t
{
    tmCmDmx_SubscriptionChanged = 0x00000001,
    tmCmDmx_SystemBitRateChanged = 0x00000002
} tmCmDmx_Notification_t;

typedef tmCmDmx_Notification_t* ptmCmDmx_Notification_t;

typedef Int32 tmCmDmx_Notifications_t;

typedef tmCmDmx_Notifications_t* ptmCmDmx_Notifications_t;

typedef enum _tmCmDmx_PlatformStreamType_t
{
    tmCmDmx_PlatformStreamAudio = 0x00000001,
    tmCmDmx_PlatformStreamVideo = 0x00000002,
    tmCmDmx_PlatformStreamSubtitle = 0x00000004
} tmCmDmx_PlatformStreamType_t;

typedef tmCmDmx_PlatformStreamType_t* ptmCmDmx_PlatformStreamType_t;

typedef Int32 tmCmDmx_SuppPlatformStreamTypes_t;

typedef tmCmDmx_SuppPlatformStreamTypes_t* ptmCmDmx_SuppPlatformStreamTypes_t;

typedef enum _tmCmDmx_StreamType_t
{
    tmCmDmx_StreamTypeUnknown = 0x00000001,
    tmCmDmx_StreamTypeAudio = 0x00000002,
    tmCmDmx_StreamTypeVideo = 0x00000003,
    tmCmDmx_StreamTypeSubtitle = 0x00000004
} tmCmDmx_StreamType_t;

typedef tmCmDmx_StreamType_t* ptmCmDmx_StreamType_t;

typedef enum _tmCmDmx_StreamBufferNotification_t
{
    tmCmDmx_StreamBufferSubscriptionChanged = 0x00000001,
    tmCmDmx_StreamBufferFull = 0x00000002,
    tmCmDmx_StreamBufferDataArrived = 0x00000004
} tmCmDmx_StreamBufferNotification_t;

typedef tmCmDmx_StreamBufferNotification_t* ptmCmDmx_StreamBufferNotification_t;

typedef Int32 tmCmDmx_StreamBufferNotifications_t;

typedef tmCmDmx_StreamBufferNotifications_t* ptmCmDmx_StreamBufferNotifications_t;

typedef enum _tmCmDmx_BufferType_t
{
    tmCmDmx_Linear = 0x00000001,
    tmCmDmx_Circular = 0x00000002
} tmCmDmx_BufferType_t;

typedef tmCmDmx_BufferType_t* ptmCmDmx_BufferType_t;
#define TMDIGVDEC_H 
#define tmDigVdec_Version ( "1.5.1" )
#define tmDigVdec_AllNotifications ( 0x0000003F )
#define tmDigVdec_UserDataAllNotifications ( 0x000000FF )
#define tmDigVdec_ClipAllNotifications ( 0x0000001F )


typedef enum _tmDigVdec_Notification_t
{
    tmDigVdec_SubscriptionChanged = 0x00000001,
    tmDigVdec_PictureSkipped = 0x00000002,
    tmDigVdec_StandardDataChanged = 0x00000004,
    tmDigVdec_PictureDataChanged = 0x00000008,
    tmDigVdec_Error = 0x00000010,
    tmDigVdec_Afd = 0x00000020
} tmDigVdec_Notification_t;

typedef tmDigVdec_Notification_t* ptmDigVdec_Notification_t;

typedef Int32 tmDigVdec_Notifications_t;

typedef tmDigVdec_Notifications_t* ptmDigVdec_Notifications_t;

typedef enum _tmDigVdec_UserDataNotification_t
{
    tmDigVdec_UserDataSubscriptionChanged = 0x00000001,
    tmDigVdec_UserDataSequence = 0x00000002,
    tmDigVdec_UserDataGop = 0x00000004,
    tmDigVdec_UserDataPicture = 0x00000008,
    tmDigVdec_UserDataVisualObjectSequence = 0x00000010,
    tmDigVdec_UserDataVisualObject = 0x00000020,
    tmDigVdec_UserDataVideoObjectLayer = 0x00000040,
    tmDigVdec_UserDataGroupOfVideoObjectPlane = 0x00000080
} tmDigVdec_UserDataNotification_t;

typedef tmDigVdec_UserDataNotification_t* ptmDigVdec_UserDataNotification_t;

typedef Int32 tmDigVdec_UserDataNotifications_t;

typedef tmDigVdec_UserDataNotifications_t* ptmDigVdec_UserDataNotifications_t;

typedef enum _tmDigVdec_CompressionStandard_t
{
    tmDigVdec_NoCompressionStandard = 0x00000000,
    tmDigVdec_Mpeg1 = 0x00000001,
    tmDigVdec_Mpeg2 = 0x00000002,
    tmDigVdec_Dv = 0x00000004,
    tmDigVdec_Mpeg4 = 0x00000008,
    tmDigVdec_DivX3 = 0x00000010,
    tmDigVdec_DivX4 = 0x00000020,
    tmDigVdec_DivX5 = 0x00000040,
    tmDigVdec_InvalidCompressionStandard = 0x00000080
} tmDigVdec_CompressionStandard_t;

typedef tmDigVdec_CompressionStandard_t* ptmDigVdec_CompressionStandard_t;

typedef Int32 tmDigVdec_CompressionStandardSet_t;

typedef tmDigVdec_CompressionStandardSet_t* ptmDigVdec_CompressionStandardSet_t;

typedef enum _tmDigVdec_Profile_t
{
    tmDigVdec_UnknownProfile = 0x00000000,
    tmDigVdec_SimpleProfile = 0x00000001,
    tmDigVdec_MainProfile = 0x00000002,
    tmDigVdec_SnrScalableProfile = 0x00000004,
    tmDigVdec_SpatiallyScalableProfile = 0x00000008,
    tmDigVdec_HighProfile = 0x00000010,
    tmDigVdec_AdvancedSimpleProfile = 0x00000020
} tmDigVdec_Profile_t;

typedef tmDigVdec_Profile_t* ptmDigVdec_Profile_t;

typedef Int32 tmDigVdec_ProfileSet_t;

typedef tmDigVdec_ProfileSet_t* ptmDigVdec_ProfileSet_t;

typedef enum _tmDigVdec_Level_t
{
    tmDigVdec_UnknownLevel = 0x00000000,
    tmDigVdec_Mpeg1ConstrainedParameter = 0x00000001,
    tmDigVdec_Mpeg2LowLevel = 0x00000002,
    tmDigVdec_Mpeg2MainLevel = 0x00000004,
    tmDigVdec_Mpeg2High1440Level = 0x00000008,
    tmDigVdec_Mpeg2HighLevel = 0x00000010,
    tmDigVdec_Dv25Level = 0x00000020,
    tmDigVdec_Dv50Level = 0x00000040,
    tmDigVdec_Mpeg4Level1 = 0x00000080,
    tmDigVdec_Mpeg4Level2 = 0x00000100,
    tmDigVdec_Mpeg4Level3 = 0x00000200,
    tmDigVdec_Mpeg4Level4 = 0x00000400,
    tmDigVdec_Mpeg4Level5 = 0x00000800,
    tmDigVdec_Mpeg4Level0 = 0x00001000,
    tmDigVdec_Mpeg4Level0b = 0x00002000,
    tmDigVdec_Mpeg4Level3b = 0x00004000
} tmDigVdec_Level_t;

typedef tmDigVdec_Level_t* ptmDigVdec_Level_t;

typedef Int32 tmDigVdec_LevelSet_t;

typedef tmDigVdec_LevelSet_t* ptmDigVdec_LevelSet_t;

typedef struct _tmDigVdec_StandardVersion_t
{
    Nat32 major;
    Nat32 minor;
    Nat32 revision;
} tmDigVdec_StandardVersion_t;

typedef tmDigVdec_StandardVersion_t* ptmDigVdec_StandardVersion_t;

typedef enum _tmDigVdec_FrameRate_t
{
    tmDigVdec_UnknownFrameRate = 0,
    tmDigVdec_23_976Hz = 1,
    tmDigVdec_24Hz = 2,
    tmDigVdec_25Hz = 3,
    tmDigVdec_29_97Hz = 4,
    tmDigVdec_30Hz = 5,
    tmDigVdec_50Hz = 6,
    tmDigVdec_59_94Hz = 7,
    tmDigVdec_60Hz = 8,
    tmDigVdec_Variable = 9
} tmDigVdec_FrameRate_t;

typedef tmDigVdec_FrameRate_t* ptmDigVdec_FrameRate_t;

typedef enum _tmDigVdec_ScanType_t
{
    tmDigVdec_UnknownScanType = 0,
    tmDigVdec_Interlaced = 1,
    tmDigVdec_Progressive = 2
} tmDigVdec_ScanType_t;

typedef tmDigVdec_ScanType_t* ptmDigVdec_ScanType_t;

typedef enum _tmDigVdec_Error_t
{
    tmDigVdec_DecodePicture = 0x00000001,
    tmDigVdec_Underrun = 0x00000002
} tmDigVdec_Error_t;

typedef tmDigVdec_Error_t* ptmDigVdec_Error_t;

typedef enum _tmDigVdec_ClipNotification_t
{
    tmDigVdec_ClipSubscriptionChanged = 0x00000001,
    tmDigVdec_ClipBufferAvailable = 0x00000002,
    tmDigVdec_ClipEndOfData = 0x00000004,
    tmDigVdec_ClipStarted = 0x00000008,
    tmDigVdec_ClipStopped = 0x00000010
} tmDigVdec_ClipNotification_t;

typedef tmDigVdec_ClipNotification_t* ptmDigVdec_ClipNotification_t;

typedef Int32 tmDigVdec_ClipNotifications_t;

typedef tmDigVdec_ClipNotifications_t* ptmDigVdec_ClipNotifications_t;

typedef enum _tmDigVdec_ClipDecodingState_t
{
    tmDigVdec_ClipDecodingStarting = 0,
    tmDigVdec_ClipDecodingStarted = 1,
    tmDigVdec_ClipDecodingStopping = 2,
    tmDigVdec_ClipDecodingStopped = 3
} tmDigVdec_ClipDecodingState_t;

typedef tmDigVdec_ClipDecodingState_t* ptmDigVdec_ClipDecodingState_t;
#define DPLFAPIIMAGEDEC_DD 






typedef Nat8* tmImageDec_Buffer_t;
#define TMIMAGEDEC2_H 






#define TMGLOBALNHAPITYPES_H 


#define TMAPITYPES_H 
#define tmGlobalNhapiTypes_Version ( 1.2 )

typedef struct _tmXY_t
{
    Int32 x;
    Int32 y;
} tmXY_t;

typedef tmXY_t* ptmXY_t;

typedef struct _tmRect_t
{
    tmXY_t ul;
    tmXY_t lr;
} tmRect_t;

typedef tmRect_t* ptmRect_t;

typedef enum _tmPixFmtCls_t
{
    tmPixFmtCls_Yuv = 0x00000010,
    tmPixFmtCls_Rgb = 0x00000020,
    tmPixFmtCls_Clut = 0x00000008
} tmPixFmtCls_t;

typedef tmPixFmtCls_t* ptmPixFmtCls_t;


typedef Int32 tmPixFmtClsSet_t;

typedef tmPixFmtClsSet_t* ptmPixFmtClsSet_t;

typedef Nat32 tmPixFmtType_t;

typedef tmPixFmtType_t* ptmPixFmtType_t;

typedef Int32 tmPixFmtTypeSet_t;

typedef tmPixFmtTypeSet_t* ptmPixFmtTypeSet_t;

typedef enum _tmPixYuvFmt_t
{
    tmPixYuvFmt_Uyvy = 0x00000001,
    tmPixYuvFmt_Yuy2 = 0x00000002,
    tmPixYuvFmt_Vyuy = 0x00000004
} tmPixYuvFmt_t;

typedef tmPixYuvFmt_t* ptmPixYuvFmt_t;

typedef Int32 tmPixYuvFmtSet_t;

typedef tmPixYuvFmtSet_t* ptmPixYuvFmtSet_t;

typedef enum _tmPixRgbFmt_t
{
    tmPixRgbFmt_Xrgb1555 = 0x00000040,
    tmPixRgbFmt_Argb1555 = 0x00000004,
    tmPixRgbFmt_Xrgb4444 = 0x00000080,
    tmPixRgbFmt_Argb4444 = 0x00000100,
    tmPixRgbFmt_Rgb565 = 0x00000008,
    tmPixRgbFmt_Xrgb8888 = 0x00000010,
    tmPixRgbFmt_Argb8888 = 0x00000020
} tmPixRgbFmt_t;

typedef tmPixRgbFmt_t* ptmPixRgbFmt_t;

typedef Int32 tmPixRgbFmtSet_t;

typedef tmPixRgbFmtSet_t* ptmPixRgbFmtSet_t;

typedef enum _tmPixClutFmt_t
{
    tmPixClutFmt_RgbClut1bpp = 0x00000001,
    tmPixClutFmt_RgbClut2bpp = 0x00000002,
    tmPixClutFmt_RgbClut4bpp = 0x00000004,
    tmPixClutFmt_RgbClut8bpp = 0x00000008,
    tmPixClutFmt_ArgbClut1bpp = 0x00000010,
    tmPixClutFmt_ArgbClut2bpp = 0x00000020,
    tmPixClutFmt_ArgbClut4bpp = 0x00000040,
    tmPixClutFmt_ArgbClut8bpp = 0x00000080,
    tmPixClutFmt_YuvClut1bpp = 0x00000100,
    tmPixClutFmt_YuvClut2bpp = 0x00000200,
    tmPixClutFmt_YuvClut4bpp = 0x00000400,
    tmPixClutFmt_YuvClut8bpp = 0x00000800,
    tmPixClutFmt_AyuvClut1bpp = 0x00001000,
    tmPixClutFmt_AyuvClut2bpp = 0x00002000,
    tmPixClutFmt_AyuvClut4bpp = 0x00004000,
    tmPixClutFmt_AyuvClut8bpp = 0x00008000
} tmPixClutFmt_t;

typedef tmPixClutFmt_t* ptmPixClutFmt_t;

typedef Int32 tmPixClutFmtSet_t;

typedef tmPixClutFmtSet_t* ptmPixClutFmtSet_t;

typedef struct _tmColor_t
{
    Nat8 redOrY;
    Nat8 greenOrU;
    Nat8 blueOrV;
} tmColor_t;

typedef tmColor_t* ptmColor_t;

typedef struct _tmColorAlpha_t
{
    Nat8 alpha;
    Nat8 redOrY;
    Nat8 greenOrU;
    Nat8 blueOrV;
} tmColorAlpha_t;

typedef tmColorAlpha_t* ptmColorAlpha_t;
typedef struct _tmTimeStamp2_t
{
    Nat32 timeStampHigh;
    Nat32 timeStampLow;
} tmTimeStamp2_t;

typedef tmTimeStamp2_t* ptmTimeStamp2_t;
#define tmImageDec2_Version ( "1.0" )
#define tmImageDec2_AllNotifications ( 0x0000007F )


typedef enum _tmImageDec2_Notification_t
{
    tmImageDec2_SubscriptionChanged = 0x00000001,
    tmImageDec2_FullImageDecoded = 0x00000002,
    tmImageDec2_ThumbnailDecoded = 0x00000004,
    tmImageDec2_MetaDataFound = 0x00000008,
    tmImageDec2_EndMetaData = 0x00000010,
    tmImageDec2_ResolutionObtained = 0x00000020,
    tmImageDec2_ThumbnailPresence = 0x00000040
} tmImageDec2_Notification_t;

typedef tmImageDec2_Notification_t* ptmImageDec2_Notification_t;

typedef Int32 tmImageDec2_Notifications_t;

typedef tmImageDec2_Notifications_t* ptmImageDec2_Notifications_t;

typedef enum _tmImageDec2_ImageFormat_t
{
    tmImageDec2_JpegExif = 0x00000001,
    tmImageDec2_ProgJpegExif = 0x00000002,
    tmImageDec2_JpegJfif = 0x00000004,
    tmImageDec2_ProgJpegJfif = 0x00000008,
    tmImageDec2_JpegSpiff = 0x00000010,
    tmImageDec2_ProgJpegSpiff = 0x00000020,
    tmImageDec2_Jpeg2k = 0x00000040,
    tmImageDec2_Gif = 0x00000080,
    tmImageDec2_InterlacedGif = 0x00000100,
    tmImageDec2_AnimatedGif = 0x00000200,
    tmImageDec2_Bmp = 0x00000400,
    tmImageDec2_Png = 0x00000800,
    tmImageDec2_Tiff = 0x00001000,
    tmImageDec2_Targa = 0x00002000,
    tmImageDec2_Pcd = 0x00004000,
    tmImageDec2_Jpeg = 0x00008000,
    tmImageDec2_ProgJpeg = 0x00010000
} tmImageDec2_ImageFormat_t;

typedef tmImageDec2_ImageFormat_t* ptmImageDec2_ImageFormat_t;

typedef Int32 tmImageDec2_ImageFormats_t;

typedef tmImageDec2_ImageFormats_t* ptmImageDec2_ImageFormats_t;

typedef enum _tmImageDec2_Status_t
{
    tmImageDec2_Successful = 1,
    tmImageDec2_Failed = 2,
    tmImageDec2_UnsupportedFormat = 3,
    tmImageDec2_CorruptData = 4
} tmImageDec2_Status_t;

typedef tmImageDec2_Status_t* ptmImageDec2_Status_t;

typedef enum _tmImageDec2_Orientation_t
{
    tmImageDec2_Default = 1,
    tmImageDec2_90ClockWise = 2,
    tmImageDec2_180ClockWise = 3,
    tmImageDec2_270ClockWise = 4,
    tmImageDec2_FlipHorizontal = 5,
    tmImageDec2_FlipVertical = 6,
    tmImageDec2_FlipHorizontal_90ClockWise = 7,
    tmImageDec2_FlipVertical_90ClockWise = 8
} tmImageDec2_Orientation_t;

typedef tmImageDec2_Orientation_t* ptmImageDec2_Orientation_t;

typedef enum _tmImageDec2_RotationMode_t
{
    tmImageDec2_NoRotation = 0x00000001,
    tmImageDec2_UseRotationBits = 0x00000002,
    tmImageDec2_ForcedRotation = 0x00000004
} tmImageDec2_RotationMode_t;

typedef tmImageDec2_RotationMode_t* ptmImageDec2_RotationMode_t;

typedef Int32 tmImageDec2_RotationModes_t;

typedef tmImageDec2_RotationModes_t* ptmImageDec2_RotationModes_t;

typedef struct _tmImageDec2_ImageArea_t
{
    Nat32 topLeftX;
    Nat32 topLeftY;
    Nat32 bottomRightX;
    Nat32 bottomRightY;
} tmImageDec2_ImageArea_t;

typedef tmImageDec2_ImageArea_t* ptmImageDec2_ImageArea_t;
#define TMSTCDEC_H 
#define TMNXTYPES_H 
#define _TMtypes_h 
#define _TMFLAGS_H_SEEN_ 



#define TMFL_CPU_TYPE_MASK 0xffff0000
#define TMFL_CPU_TYPE_X86 0x00010000
#define TMFL_CPU_TYPE_MIPS 0x00020000
#define TMFL_CPU_TYPE_TM 0x00030000
#define TMFL_CPU_TYPE_HP 0x00040000
#define TMFL_CPU_TYPE_ARM 0x00050000
#define TMFL_CPU_TYPE_REAL 0x00060000
#define TMFL_CPU_TYPE_8051 0x00070000
#define TMFL_CPU_MODEL_MASK 0x0000ffff
#define TMFL_CPU_MODEL_I486 0x00000001
#define TMFL_CPU_MODEL_R3940 0x00000002
#define TMFL_CPU_MODEL_R4300 0x00000003
#define TMFL_CPU_MODEL_TM1100 0x00000004
#define TMFL_CPU_MODEL_TM1300 0x00000005
#define TMFL_CPU_MODEL_TM32 0x00000006
#define TMFL_CPU_MODEL_HP 0x00000007
#define TMFL_CPU_MODEL_R4640 0x00000008
#define TMFL_CPU_MODEL_ARM7 0x00000009
#define TMFL_CPU_MODEL_ARM920T 0x0000000a
#define TMFL_CPU_MODEL_ARM940T 0x0000000b
#define TMFL_CPU_MODEL_ARM10 0x0000000c
#define TMFL_CPU_MODEL_STRONGARM 0x0000000d
#define TMFL_CPU_MODEL_RD24120 0x0000000e
#define TMFL_CPU_MODEL_ARM926EJS 0x0000000f
#define TMFL_CPU_MODEL_ARM946 0x00000010
#define TMFL_CPU_MODEL_R1910 0x00000011
#define TMFL_CPU_MODEL_R4450 0x00000012
#define TMFL_CPU_MODEL_TM3260 0x00000013
#define TMFL_CPU_MODEL_TM8051 0x00000014
#define TMFL_CPU_MODEL_TM2270 0x00000015
#define TMFL_CPU_MODEL_TM5250 0x00000016
#define TMFL_CPU_MODEL_TM3270 0x00000017
#define TMFL_CPU_MODEL_TM3270R128 0x00000018
#define TMFL_ENDIAN_BIG 1
#define TMFL_ENDIAN_LITTLE 0
#define TMFL_OS_MASK 0xff000000
#define TMFL_OS_VERSION_MASK 0x00ffffff
#define TMFL_OS_BTM 0x00000000
#define TMFL_OS_CE 0x01000000
#define TMFL_OS_CE212 0x01020102
#define TMFL_OS_CE300 0x01030000
#define TMFL_OS_NT 0x02000000
#define TMFL_OS_NT4 0x02040000
#define TMFL_OS_PSOS 0x03000000
#define TMFL_OS_PSOS250 0x03020500
#define TMFL_OS_PSOS200 0x03020000
#define TMFL_OS_NULLOS 0x04000000
#define TMFL_OS_ECOS 0x05000000
#define TMFL_OS_VXWORKS 0x06000000
#define TMFL_OS_MTOS 0x07000000
#define TMFL_OS_CEXEC 0x08000000
#define TMFL_OS_NUCLEUS 0x09000000
#define TMFL_SCOPE_SP 0
#define TMFL_SCOPE_MP 1
#define TMFL_REL_TRACE 0x00000003
#define TMFL_REL_ASSERT 0x00000002
#define TMFL_REL_DEBUG 0x00000001
#define TMFL_REL_RETAIL 0x00000000
#define TMFL_CPU_I486 0x00010001
#define TMFL_CPU_R3940 0x00020002
#define TMFL_CPU_R4300 0x00020003
#define TMFL_CPU_TM1100 0x00030004
#define TMFL_CPU_TM1300 0x00030005
#define TMFL_CPU_TM32 0x00030006
#define TMFL_CPU_HP 0x00040007
#define TMFL_CPU_R4640 0x00020008
#define TMFL_CPU_ARM7 0x00050009
#define TMFL_CPU_ARM920T 0x0005000a
#define TMFL_CPU_ARM940T 0x0005000b
#define TMFL_CPU_ARM10 0x0005000c
#define TMFL_CPU_STRONGARM 0x0005000d
#define TMFL_CPU_RD24120 0x0006000e
#define TMFL_CPU_ARM926EJS 0x0005000f
#define TMFL_CPU_ARM946 0x00050010
#define TMFL_CPU_R1910 0x00020011
#define TMFL_CPU_R4450 0x00020012
#define TMFL_CPU_TM3260 0x00030013
#define TMFL_CPU_TM8051 0x00070014
#define TMFL_CPU_TM2270 0x00030015
#define TMFL_CPU_TM5250 0x00030016
#define TMFL_CPU_TM3270 0x00030017
#define TMFL_CPU_TM3270R128 0x00030018
#define TMFL_MODE_KERNEL 1
#define TMFL_MODE_USER 0
#define _TMENDIAN_H_ 
#define TMFL_ENDIAN TMFL_ENDIAN_LITTLE
#define TMNXTYPES_STDC_VERSION_C89 198912L
#define TMNXTYPES_STDC_VERSION_C95 199409L
#define TMNXTYPES_STDC_VERSION_C99 199901L



#define TMFL_STDC_VERSION_C89 TMNXTYPES_STDC_VERSION_C89
#define TMFL_STDC_VERSION_C95 TMNXTYPES_STDC_VERSION_C95
#define TMFL_STDC_VERSION_C99 TMNXTYPES_STDC_VERSION_C99
#define TM_FALSE 0
#define TM_TRUE 1
#define TM_ENDIAN_BIG 0
#define TM_ENDIAN_LITTLE 1
#define _TMtypes_h 
typedef signed int IBits32;
typedef unsigned int UBits32;





typedef IBits32 *pIBits32;
typedef UBits32 *pUBits32;




typedef Void *pVoid;
typedef Float *pFloat;
typedef double Double, *pDouble;


typedef Int *pInt;
typedef UInt *pUInt;


typedef Int tmEndian_t, *ptmEndian_t;


#define HAL_DEVICE_NAME_LENGTH 16


typedef UInt32 tmProgressCode_t;


typedef UInt64 tmTimeStamp_t, *ptmTimeStamp_t;


#define ticks lo
#define hiTicks hi
typedef union tmColor3
{
    UBits32 u32;
    struct {
        UBits32 blue : 8;
        UBits32 green : 8;
        UBits32 red : 8;
        UBits32 : 8;
    } rgb;
    struct {
        UBits32 v : 8;
        UBits32 u : 8;
        UBits32 y : 8;
        UBits32 : 8;
    } yuv;
    struct {
        UBits32 l : 8;
        UBits32 m : 8;
        UBits32 u : 8;
        UBits32 : 8;
    } uml;

} tmColor3_t, *ptmColor3_t;

typedef union tmColor4
{
    UBits32 u32;
    struct {
        UBits32 blue : 8;
        UBits32 green : 8;
        UBits32 red : 8;
        UBits32 alpha : 8;
    } argb;
    struct {
        UBits32 v : 8;
        UBits32 u : 8;
        UBits32 y : 8;
        UBits32 alpha : 8;
    } ayuv;
    struct {
        UBits32 l : 8;
        UBits32 m : 8;
        UBits32 u : 8;
        UBits32 alpha : 8;
    } auml;

} tmColor4_t, *ptmColor4_t;




typedef enum tmPowerState
{
    tmPowerOn,
    tmPowerStandby,
    tmPowerSuspend,
    tmPowerOff

} tmPowerState_t, *ptmPowerState_t;




typedef struct tmSWVersion
{
    UInt32 compatibilityNr;
    UInt32 majorVersionNr;
    UInt32 minorVersionNr;

} tmSWVersion_t, *ptmSWVersion_t;






#define TM_CHARS_TO_UINT32(a,b,c,d) ((UInt32) ((UInt8) (a) << 24 | (UInt8) (b) << 16 | (UInt8) (c) << 8 | (UInt8) (d)))
#define _TMBOARDDEF_H_ 




typedef Int tmUnitSelect_t, *ptmUnitSelect_t;

#define tmUnitNone (-1)
#define tmUnit0 0
#define tmUnit1 1
#define tmUnit2 2
#define tmUnit3 3
#define tmUnit4 4


#define unitSelect_t tmUnitSelect_t
#define unit0 tmUnit0
#define unit1 tmUnit1
#define unit2 tmUnit2
#define unit3 tmUnit3
#define unit4 tmUnit4
#define DEVICE_NAME_LENGTH HAL_DEVICE_NAME_LENGTH





#define tmUnit5 5
#define tmUnit6 6
#define tmUnit7 7




typedef Int tmInstance_t, *ptmInstance_t;


typedef Void (*ptmCallback_t) (UInt32 events, Void *pData, UInt32 userData);
#define tmCallback_t ptmCallback_t
#define __STDC_VERSION__ (TMNXTYPES_STDC_VERSION_C89)


#define STDC_VERSION_LOCALLY_DEFINED 
#define TMNATIVEINT64_H 
#define TMFL_NATIVE_INT64 1

#define I64LIT(i) i ##LL
#define U64LIT(u) u ##ULL
#define inline __inline__


#define restrict 
#undef STDC_VERSION_LOCALLY_DEFINED
#define tmStcDec_Version ( "1.3" )
#define TM_ERR_STCDEC_ALARM_NOT_AVAILABLE ( 0x00000001 )
#define TM_ERR_STCDEC_ALARM_IN_PAST ( 0x00000002 )
#define TM_ERR_STCDEC_ALARM_EXPIRED ( 0x00000003 )
#define TM_ERR_STCDEC_NOT_LOCKED ( 0x00000004 )
#define tmStcDec_AllNotifications ( 0x0000000F )
#define tmStcDec_AlarmAllNotifications ( 0x00000007 )


typedef enum _tmStcDec_Notification_t
{
    tmStcDec_SubscriptionChanged = 0x00000001,
    tmStcDec_Locked = 0x00000002,
    tmStcDec_Unlocked = 0x00000004,
    tmStcDec_Discontinuity = 0x00000008
} tmStcDec_Notification_t;

typedef tmStcDec_Notification_t* ptmStcDec_Notification_t;

typedef Int32 tmStcDec_Notifications_t;

typedef tmStcDec_Notifications_t* ptmStcDec_Notifications_t;

typedef enum _tmStcDec_AlarmNotification_t
{
    tmStcDec_AlarmSubscriptionChanged = 0x00000001,
    tmStcDec_AlarmExpired = 0x00000002,
    tmStcDec_AlarmCancelled = 0x00000004
} tmStcDec_AlarmNotification_t;

typedef tmStcDec_AlarmNotification_t* ptmStcDec_AlarmNotification_t;

typedef Int32 tmStcDec_AlarmNotifications_t;

typedef tmStcDec_AlarmNotifications_t* ptmStcDec_AlarmNotifications_t;
#define TSDMXABSTRACT 





typedef enum SectionCrcMode
{
    CrcModeSkipBadSections = 1,
    CrcModeAcceptAllSections = 2,
    CrcModeDontCheckCrc = 3
}SectionCrcMode;

typedef struct SectionFilterConfig
{
    int pid;
    int BufferSize;
    SectionCrcMode CrcMode;
    Bool ContinuousMode;
    int cookie;
}SectionFilterConfig;




#define TMTSDMX_H 
#define tmTsDmx_Version ( "1.5" )
#define TM_ERR_TSDMX_PCR_FILTER_NOT_AVAILABLE ( 0x00000801 )
#define TM_ERR_TSDMX_DESCR_NOT_AVAILABLE ( 0x00000802 )
#define TM_ERR_TSDMX_PID_FILTER_NOT_AVAILABLE ( 0x00000803 )
#define TM_ERR_TSDMX_TPH_FILTER_NOT_AVAILABLE ( 0x00000804 )
#define TM_ERR_TSDMX_SCT_FILTER_NOT_AVAILABLE ( 0x00000805 )
#define TM_ERR_TSDMX_PES_FILTER_NOT_AVAILABLE ( 0x00000806 )
#define TM_ERR_TSDMX_FILTER_NOT_STARTED ( 0x00000807 )
#define tmTsDmx_OutBase ( 1 )
#define tmTsDmx_AllNotifications ( 0x0000001F )
#define tmTsDmx_StreamBufferAllNotifications ( 0x00000003 )
#define tmTsDmx_PcrAllNotifications ( 0x00000003 )
#define tmTsDmx_PidAllNotifications ( 0x00000003 )
#define tmTsDmx_DescramblerAssociation2AllNotifications ( 0x00000003 )
#define tmTsDmx_SectionAllNotifications ( 0x00000007 )
#define tmTsDmx_PesAllNotifications ( 0x0000003F )
#define tmTsDmx_SectionListAllNotifications ( 0x00000007 )

typedef Nat8* tmTsDmx_Buffer_t;

typedef enum _tmTsDmx_Notification_t
{
    tmTsDmx_SubscriptionChanged = 0x00000001,
    tmTsDmx_SyncLost = 0x00000002,
    tmTsDmx_SyncLock = 0x00000004,
    tmTsDmx_InputOverflow = 0x00000008,
    tmTsDmx_PlatformPidStopped = 0x00000010
} tmTsDmx_Notification_t;

typedef tmTsDmx_Notification_t* ptmTsDmx_Notification_t;

typedef Int32 tmTsDmx_Notifications_t;

typedef tmTsDmx_Notifications_t* ptmTsDmx_Notifications_t;

typedef enum _tmTsDmx_StreamBufferNotification_t
{
    tmTsDmx_StreamBufferSubscriptionChanged = 0x00000001,
    tmTsDmx_StreamBufferFull = 0x00000002
} tmTsDmx_StreamBufferNotification_t;

typedef tmTsDmx_StreamBufferNotification_t* ptmTsDmx_StreamBufferNotification_t;

typedef Int32 tmTsDmx_StreamBufferNotifications_t;

typedef tmTsDmx_StreamBufferNotifications_t* ptmTsDmx_StreamBufferNotifications_t;

typedef enum _tmTsDmx_PcrNotification_t
{
    tmTsDmx_PcrSubscriptionChanged = 0x00000001,
    tmTsDmx_PcrFilterStopped = 0x00000002
} tmTsDmx_PcrNotification_t;

typedef tmTsDmx_PcrNotification_t* ptmTsDmx_PcrNotification_t;

typedef Int32 tmTsDmx_PcrNotifications_t;

typedef tmTsDmx_PcrNotifications_t* ptmTsDmx_PcrNotifications_t;

typedef enum _tmTsDmx_PidNotification_t
{
    tmTsDmx_PidSubscriptionChanged = 0x00000001,
    tmTsDmx_PidFilterStopped = 0x00000002
} tmTsDmx_PidNotification_t;

typedef tmTsDmx_PidNotification_t* ptmTsDmx_PidNotification_t;

typedef Int32 tmTsDmx_PidNotifications_t;

typedef tmTsDmx_PidNotifications_t* ptmTsDmx_PidNotifications_t;

typedef enum _tmTsDmx_DescramblerAssociation2Notification_t
{
    tmTsDmx_DescramblerAssociation2SubscriptionChanged = 0x00000001,
    tmTsDmx_DescramblerAssociation2ScramblingStatusChanged = 0x00000002
} tmTsDmx_DescramblerAssociation2Notification_t;

typedef tmTsDmx_DescramblerAssociation2Notification_t* ptmTsDmx_DescramblerAssociation2Notification_t;

typedef Int32 tmTsDmx_DescramblerAssociation2Notifications_t;

typedef tmTsDmx_DescramblerAssociation2Notifications_t* ptmTsDmx_DescramblerAssociation2Notifications_t;

typedef enum _tmTsDmx_SectionNotification_t
{
    tmTsDmx_SectionSubscriptionChanged = 0x00000001,
    tmTsDmx_SectionFilterStopped = 0x00000002,
    tmTsDmx_SectionArrived = 0x00000004
} tmTsDmx_SectionNotification_t;

typedef tmTsDmx_SectionNotification_t* ptmTsDmx_SectionNotification_t;

typedef Int32 tmTsDmx_SectionNotifications_t;

typedef tmTsDmx_SectionNotifications_t* ptmTsDmx_SectionNotifications_t;

typedef enum _tmTsDmx_PesNotification_t
{
    tmTsDmx_PesSubscriptionChanged = 0x00000001,
    tmTsDmx_PesNotifierStopped = 0x00000002,
    tmTsDmx_PesCompletePacketArrived = 0x00000004,
    tmTsDmx_PesPartialPacketArrived = 0x00000008,
    tmTsDmx_PesPacketCorrupt = 0x00000010,
    tmTsDmx_PesStreamIdChanged = 0x00000020
} tmTsDmx_PesNotification_t;

typedef tmTsDmx_PesNotification_t* ptmTsDmx_PesNotification_t;

typedef Int32 tmTsDmx_PesNotifications_t;

typedef tmTsDmx_PesNotifications_t* ptmTsDmx_PesNotifications_t;

typedef enum _tmTsDmx_DescramblerType_t
{
    tmTsDmx_TdesFbEcbRbNoneSbNone = 0x00000001,
    tmTsDmx_TdesFbEcbRbOfbIvSbNone = 0x00000002,
    tmTsDmx_TdesFbEcbRbOfbPbSbNone = 0x00000004,
    tmTsDmx_TdesFbEcbRbOfbIvSbOfbIv = 0x00000008,
    tmTsDmx_TdesFbEcbRbOfbPbSbOfbIv = 0x00000010,
    tmTsDmx_TdesFbCbcRbNoneSbNone = 0x00000020,
    tmTsDmx_TdesFbCbcRbOfbIvSbNone = 0x00000040,
    tmTsDmx_TdesFbCbcRbOfbPbSbNone = 0x00000080,
    tmTsDmx_TdesFbCbcRbOfbIvSbOfbIv = 0x00000100,
    tmTsDmx_TdesFbCbcRbOfbPbSbOfbIv = 0x00000200,
    tmTsDmx_SdesFbEcbRbNoneSbNone = 0x00000400,
    tmTsDmx_SdesFbEcbRbOfbIvSbNone = 0x00000800,
    tmTsDmx_SdesFbEcbRbOfbPbSbNone = 0x00001000,
    tmTsDmx_SdesFbEcbRbOfbIvSbOfbIv = 0x00002000,
    tmTsDmx_SdesFbEcbRbOfbPbSbOfbIv = 0x00004000,
    tmTsDmx_SdesFbCbcRbNoneSbNone = 0x00008000,
    tmTsDmx_SdesFbCbcRbOfbIvSbNone = 0x00010000,
    tmTsDmx_SdesFbCbcRbOfbPbSbNone = 0x00020000,
    tmTsDmx_SdesFbCbcRbOfbIvSbOfbIv = 0x00040000,
    tmTsDmx_SdesFbCbcRbOfbPbSbOfbIv = 0x00080000,
    tmTsDmx_Dvb = 0x00100000,
    tmTsDmx_Multi2 = 0x00200000
} tmTsDmx_DescramblerType_t;

typedef tmTsDmx_DescramblerType_t* ptmTsDmx_DescramblerType_t;

typedef Int32 tmTsDmx_DescramblerTypes_t;

typedef tmTsDmx_DescramblerTypes_t* ptmTsDmx_DescramblerTypes_t;

typedef enum _tmTsDmx_StreamDataType_t
{
    tmTsDmx_InternalFormat = 0x00000001,
    tmTsDmx_Pes = 0x00000002,
    tmTsDmx_Tp = 0x00000003,
    tmTsDmx_MpegSection = 0x00000004
} tmTsDmx_StreamDataType_t;

typedef tmTsDmx_StreamDataType_t* ptmTsDmx_StreamDataType_t;

typedef enum _tmTsDmx_BufferType_t
{
    tmTsDmx_Linear = 0x00000001,
    tmTsDmx_Circular = 0x00000002
} tmTsDmx_BufferType_t;

typedef tmTsDmx_BufferType_t* ptmTsDmx_BufferType_t;

typedef Int32 tmTsDmx_BufferTypes_t;

typedef tmTsDmx_BufferTypes_t* ptmTsDmx_BufferTypes_t;

typedef enum _tmTsDmx_CrcMode_t
{
    tmTsDmx_SkipBadSections = 0x00000001,
    tmTsDmx_AcceptAllSections = 0x00000002,
    tmTsDmx_DontCheckCrc = 0x00000003
} tmTsDmx_CrcMode_t;

typedef tmTsDmx_CrcMode_t* ptmTsDmx_CrcMode_t;

typedef enum _tmTsDmx_CrcStatus_t
{
    tmTsDmx_CrcOk = 0x00000001,
    tmTsDmx_CrcNotOk = 0x00000002,
    tmTsDmx_CrcNotChecked = 0x00000003,
    tmTsDmx_NoCrc = 0x00000004
} tmTsDmx_CrcStatus_t;

typedef tmTsDmx_CrcStatus_t* ptmTsDmx_CrcStatus_t;

typedef enum _tmTsDmx_FilterMode_t
{
    tmTsDmx_OneShot = 0x00000001,
    tmTsDmx_Continuous = 0x00000002
} tmTsDmx_FilterMode_t;

typedef tmTsDmx_FilterMode_t* ptmTsDmx_FilterMode_t;

typedef enum _tmTsDmx_ScramblingStatus_t
{
    tmTsDmx_ScramblingUnknown = 0x00000001,
    tmTsDmx_NotScrambled = 0x00000002,
    tmTsDmx_Scrambled = 0x00000003
} tmTsDmx_ScramblingStatus_t;

typedef tmTsDmx_ScramblingStatus_t* ptmTsDmx_ScramblingStatus_t;

typedef struct _tmTsDmx_TimeStamp_t
{
    Nat32 timeStampHigh;
    Nat32 timeStampLow;
} tmTsDmx_TimeStamp_t;

typedef tmTsDmx_TimeStamp_t* ptmTsDmx_TimeStamp_t;

typedef enum _tmTsDmx_CwType_t
{
    tmTsDmx_OddKey = 0x00000001,
    tmTsDmx_EvenKey = 0x00000002,
    tmTsDmx_SystemKey = 0x00000003,
    tmTsDmx_InitVectorCbc = 0x00000004,
    tmTsDmx_InitVectorOfb = 0x00000005
} tmTsDmx_CwType_t;

typedef tmTsDmx_CwType_t* ptmTsDmx_CwType_t;

typedef enum _tmTsDmx_PlatformPin_t
{
    tmTsDmx_VideoPin = 1,
    tmTsDmx_AudioPin = 2,
    tmTsDmx_SptsPin = 3,
    tmTsDmx_AudioDescriptionPin = 4
} tmTsDmx_PlatformPin_t;

typedef tmTsDmx_PlatformPin_t* ptmTsDmx_PlatformPin_t;


typedef struct _tmTsDmx_Section_t {
    Nat32 primarySectionSize;
    Nat8* pPrimarySectionData;
    Nat32 secondarySectionSize;
    Nat8* pSecondarySectionData;
    tmTsDmx_CrcStatus_t crcStatus;
} tmTsDmx_Section_t;

typedef enum _tmTsDmx_SectionListNotification_t {
    tmTsDmx_SectionListSubscriptionChanged = 0x00000001,
    tmTsDmx_SectionListStopped = 0x00000002,
    tmTsDmx_SectionListArrived = 0x00000004
} tmTsDmx_SectionListNotification_t ;

typedef tmTsDmx_SectionListNotification_t *ptmTsDmx_SectionListNotification_t;
typedef Int32 tmTsDmx_SectionListNotifications_t;
typedef tmTsDmx_SectionListNotifications_t *ptmTsDmx_SectionListNotifications_t;
#define TMURLSRC_H 
#define tmUrlSrc_Version ( "3.1" )
#define TM_ERR_URLSRC_INVALID_URL ( 0x00000801 )
#define TM_ERR_URLSRC_SCHEME_NOT_SUPP ( 0x00000802 )
#define TM_ERR_URLSRC_ABORTED ( 0x00000803 )
#define TM_ERR_URLSRC_NOT_ABORTED ( 0x00000804 )
#define TM_ERR_URLSRC_END_OF_STREAM ( 0x00000805 )
#define TM_ERR_URLSRC_CONNECTION_LOST ( 0x00000806 )
#define tmUrlSrc_AllNotificationsEx ( 0x000007FF )
#define tmUrlSrc_AllTrickNotifications ( 0x00000007 )


typedef enum _tmUrlSrc_NotificationEx_t
{
    tmUrlSrc_SubscriptionChanged = 0x00000001,
    tmUrlSrc_Accessible = 0x00000002,
    tmUrlSrc_NotAccessible = 0x00000004,
    tmUrlSrc_ConnectionRetry = 0x00000008,
    tmUrlSrc_EndOfRendering = 0x00000010,
    tmUrlSrc_EndOfInput = 0x00000020,
    tmUrlSrc_DataConsumed = 0x00000040,
    tmUrlSrc_PlayTimeChanged = 0x00000080,
    tmUrlSrc_ActionAborted = 0x00000100,
    tmUrlSrc_TrickControlLost = 0x00000200,
    tmUrlSrc_BufferingStateChanged = 0x00000400
} tmUrlSrc_NotificationEx_t;

typedef tmUrlSrc_NotificationEx_t* ptmUrlSrc_NotificationEx_t;

typedef Int32 tmUrlSrc_NotificationsEx_t;

typedef tmUrlSrc_NotificationsEx_t* ptmUrlSrc_NotificationsEx_t;

typedef enum _tmUrlSrc_Scheme_t
{
    tmUrlSrc_SchemeFtp = 0x00000001,
    tmUrlSrc_SchemeHttp = 0x00000002,
    tmUrlSrc_SchemeHttps = 0x00000004,
    tmUrlSrc_SchemeGopher = 0x00000008,
    tmUrlSrc_SchemeNews = 0x00000010,
    tmUrlSrc_SchemeNntp = 0x00000020,
    tmUrlSrc_SchemeFile = 0x00000040,
    tmUrlSrc_SchemeProspero = 0x00000080,
    tmUrlSrc_SchemeRtsp = 0x00000100,
    tmUrlSrc_SchemeRtspu = 0x00000200
} tmUrlSrc_Scheme_t;

typedef tmUrlSrc_Scheme_t* ptmUrlSrc_Scheme_t;

typedef Int32 tmUrlSrc_Schemes_t;

typedef tmUrlSrc_Schemes_t* ptmUrlSrc_Schemes_t;

typedef enum _tmUrlSrc_NotAccessibleCause_t
{
    tmUrlSrc_NacNotFound = 0,
    tmUrlSrc_NacTimeOut = 1,
    tmUrlSrc_NacConnectionLost = 2,
    tmUrlSrc_NacConnectionTerminated = 3
} tmUrlSrc_NotAccessibleCause_t;

typedef tmUrlSrc_NotAccessibleCause_t* ptmUrlSrc_NotAccessibleCause_t;

typedef enum _tmUrlSrc_Rounding_t
{
    tmUrlSrc_RoundUp = 0,
    tmUrlSrc_RoundDown = 1,
    tmUrlSrc_RoundNearest = 2
} tmUrlSrc_Rounding_t;

typedef tmUrlSrc_Rounding_t* ptmUrlSrc_Rounding_t;

typedef enum _tmUrlSrc_StepMode_t
{
    tmUrlSrc_StepForward = 0x00000001,
    tmUrlSrc_StepBackward = 0x00000002
} tmUrlSrc_StepMode_t;

typedef tmUrlSrc_StepMode_t* ptmUrlSrc_StepMode_t;

typedef Int32 tmUrlSrc_StepModes_t;

typedef tmUrlSrc_StepModes_t* ptmUrlSrc_StepModes_t;

typedef enum _tmUrlSrc_BufferingState_t
{
    tmUrlSrc_BufferStateStreaming = 0,
    tmUrlSrc_BufferStateBuffering = 1
} tmUrlSrc_BufferingState_t;

typedef tmUrlSrc_BufferingState_t* ptmUrlSrc_BufferingState_t;

typedef enum _tmUrlSrc_TrickNotification_t
{
    tmUrlSrc_TrickSubscriptionChanged = 0x00000001,
    tmUrlSrc_TrickSetPositionAborted = 0x00000002,
    tmUrlSrc_TrickSetPositionCompleted = 0x00000004
} tmUrlSrc_TrickNotification_t;

typedef tmUrlSrc_TrickNotification_t* ptmUrlSrc_TrickNotification_t;

typedef Int32 tmUrlSrc_TrickNotifications_t;

typedef tmUrlSrc_TrickNotifications_t* ptmUrlSrc_TrickNotifications_t;

typedef enum _tmUrlSrc_SeekMode_t {
    tmUrlSrc_SeekNone = 0,
    tmUrlSrc_SeekByBytes = 1,
    tmUrlSrc_SeekByTime = 2
} tmUrlSrc_SeekMode_t;

typedef tmUrlSrc_SeekMode_t* ptmUrlSrc_SeekMode_t;
#define TMVBISLICE2_H 
#define tmVbiSlice2_Version ( "2.4.2" )
#define TM_ERR_VBISLICE2_LINES_NOT_SET ( 0x00000801 )
#define tmVbiSlice2_AllNotifications ( 0x00000007 )


typedef enum _tmVbiSlice2_Notification_t
{
    tmVbiSlice2_SubscriptionChanged = 0x00000001,
    tmVbiSlice2_FrameData = 0x00000002,
    tmVbiSlice2_FrameNoData = 0x00000004
} tmVbiSlice2_Notification_t;

typedef tmVbiSlice2_Notification_t* ptmVbiSlice2_Notification_t;

typedef Int32 tmVbiSlice2_Notifications_t;

typedef tmVbiSlice2_Notifications_t* ptmVbiSlice2_Notifications_t;

typedef enum _tmVbiSlice2_Wst_t
{
    tmVbiSlice2_EuroTeletext = 0x00000000,
    tmVbiSlice2_UsTeletext = 0x00000001,
    tmVbiSlice2_Teletext = 0x00000002,
    tmVbiSlice2_Nabts = 0x00000003,
    tmVbiSlice2_Moji = 0x00000004
} tmVbiSlice2_Wst_t;

typedef tmVbiSlice2_Wst_t* ptmVbiSlice2_Wst_t;

typedef enum _tmVbiSlice2_Cc_t
{
    tmVbiSlice2_EuroClosedCaption = 0x00000000,
    tmVbiSlice2_UsClosedCaption = 0x00000001
} tmVbiSlice2_Cc_t;

typedef tmVbiSlice2_Cc_t* ptmVbiSlice2_Cc_t;

typedef enum _tmVbiSlice2_Wss_t
{
    tmVbiSlice2_EuroWss = 0x00000000,
    tmVbiSlice2_UsWss = 0x00000001
} tmVbiSlice2_Wss_t;

typedef tmVbiSlice2_Wss_t* ptmVbiSlice2_Wss_t;

typedef enum _tmVbiSlice2_Gemstar_t
{
    tmVbiSlice2_Gemstar1x = 0x00000000,
    tmVbiSlice2_Gemstar2x = 0x00000001
} tmVbiSlice2_Gemstar_t;

typedef tmVbiSlice2_Gemstar_t* ptmVbiSlice2_Gemstar_t;

typedef enum _tmVbiSlice2_VbiData_t
{
    tmVbiSlice2_VbiNone = 0x00000000,
    tmVbiSlice2_VbiEuroTeletext = 0x00000001,
    tmVbiSlice2_VbiUsTeletext = 0x00000002,
    tmVbiSlice2_VbiTeletext = 0x00000004,
    tmVbiSlice2_VbiNabts = 0x00000008,
    tmVbiSlice2_VbiMoji = 0x00000010,
    tmVbiSlice2_VbiEuroClosedCaption = 0x00000020,
    tmVbiSlice2_VbiUsClosedCaption = 0x00000040,
    tmVbiSlice2_VbiEuroWss = 0x00000080,
    tmVbiSlice2_VbiUsWss = 0x00000100,
    tmVbiSlice2_VbiVps = 0x00000200,
    tmVbiSlice2_VbiGemstar1x = 0x00000400,
    tmVbiSlice2_VbiGemstar2x = 0x00000800,
    tmVbiSlice2_VbiCgms = 0x00001000
} tmVbiSlice2_VbiData_t;

typedef tmVbiSlice2_VbiData_t* ptmVbiSlice2_VbiData_t;

typedef Int32 tmVbiSlice2_VbiDataSet_t;

typedef tmVbiSlice2_VbiDataSet_t* ptmVbiSlice2_VbiDataSet_t;
typedef struct _VMixXY
{
    Int32 x;
    Int32 y;
} VMixXY;

typedef struct _VMixRect
{
    VMixXY ul;
    VMixXY lr;
} VMixRect;




typedef struct _VMixColor
{
    Nat8 redOrY;
    Nat8 greenOrU;
    Nat8 blueOrV;
} VMixColor;

typedef enum _VMixValStat
{
    VMix_ValStatInvalid = -2,
    VMix_ValStatUnknown = -1,
    VMix_ValStatKnown = 1
} VMixValStat;

typedef struct _VMixNewVidLayerProp {
    int changes;
    VMixValStat status;
    Nat32 horResolution;
    Nat32 vertResolution;
    Nat16 aspectRatioNumerator;
    Nat16 aspectRatioDenominator;
    VMixRect activeImageArea;
} VMixNewVidLayerProp;

typedef enum _VMixNewVidLayerPropChanged
{
    VMix_ResolutionChanged = 0x00000001,
    VMix_AspectRatioChanged = 0x00000002,
    VMix_ActiveImageAreaChanged = 0x00000004
} VMixNewVidLayerPropChanged;


// MTK
#define MTK_CUST_KEYBOARD_BUTTON_MAX_COUNT 10
#define MTK_CUST_RC_MAX_COUNT 8

typedef struct 
{
    unsigned int u4Top;           ///< ADC top value of the button
    unsigned int u4Btm;           ///< ADC button value of the button
    unsigned int u4Key;           ///< For System internal key command.
} MTK_CUST_KEYBOARD_BUTTON_T;

typedef struct 
{
    int i4Src;
    int i4Sys;
    int i4Cmd;
} MTK_CUST_WAKEUP_KEY_T;

//! lvds mode
#define LVDS_MODE_8BIT_JEIDA  0
#define LVDS_MODE_8BIT_VESA   1
#define LVDS_MODE_10BIT_VESA  2
#define LVDS_MODE_10BIT_JEIDA 3
#define LVDS_MODE_10BIT_KOREA 4

//! lvds color domain
#define LVDS_COLOR_DOMAIN_YUV 0
#define LVDS_COLOR_DOMAIN_RGB 1


typedef struct
{
    Nat32   HActive;
    Nat32   VActive;
    Nat32   HTotal_50Hz;
    Nat32   VTotal_50Hz;
    Nat32   HBlankS_50Hz;
    Nat32   HBlankE_50Hz;
    Nat32   VBlankS_50Hz;
    Nat32   VBlankE_50Hz;


    Nat32   HTotal_60Hz;
    Nat32   VTotal_60Hz;
    Nat32   HBlankS_60Hz;
    Nat32   HBlankE_60Hz;
    Nat32   VBlankS_60Hz;
    Nat32   VBlankE_60Hz;

    Nat32   HSyncS;
    Nat32   HSyncE;
    Nat32   VSyncS;
    Nat32   VSyncE;

    Bool     HSyncCtlEn;
    Bool     VSyncCtlEn;
    Bool     BlankCtl;
    Bool     HSyncNeg;
    Bool     VSyncNeg;
    Bool     BlankNeg;

    Nat8    LvdsMode;

    Nat8    FieldRate;
    Nat8    ColorDomain;

    /* pwm */
    Nat16    PwmFrequency; //Hz
    Nat8    PwmMin;   // duty cycle, 255 = 100%
    Nat8    PwmMax;   // duty cycle, 255 = 100%
    Bool     PwmInverted;
    Bool     PwmFrameSynced;

    /* power sequence */
    Nat8    LvdsOnDelay;
    Nat8    BacklightOnDelay;

    Nat8    LvdsOffDelay;
    Nat8    PanelOffDelay;

    /* spread spectrum */
    Nat16   SpreadSpectrumRange; //  o/oo
    Nat8    SpreadSpectrumPeriod;    // us
    
} MTK_CUST_PANEL_ATTR_T;

//! frontend customization 
typedef enum _MTK_CUST_FE_ID_T 
{
    MTK_CUST_FE_SHARP_VA1G5BF   = 0,
//    MTK_CUST_FE_VENDOR_TUNER_A  = 1,
//    MTK_CUST_FE_VENDOR_TUNER_B  = 2,
    MTK_CUST_FE_NUM
} MTK_CUST_FE_ID_T;

typedef struct 
{
    MTK_CUST_FE_ID_T    FeId;
    UInt32              maxFreq;    // KHz
    UInt32              minFreq;    // KHz
} MTK_CUST_FE_DIV_T;


#endif /* PAPI_TYPES_H */

