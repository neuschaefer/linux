/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/******************************************************************************
* [File]			msdc_slave_hw.h
* [Version]			v1.0
* [Revision Date]	2011-05-04
* [Author]			Shunli Wang, shunli.wang@mediatek.inc, 82896, 2011-05-04
* [Description]
*	MSDC Driver Header File
* [Copyright]
*	Copyright (C) 2011 MediaTek Incorporation. All Rights Reserved.
******************************************************************************/

#ifndef _MSDC_SLAVE_HW_H_
#define _MSDC_SLAVE_HW_H_

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------
/* Card Type */
#define CARDTYPE_NOT_SD				0
#define CARDTYPE_MEM_STD			1
#define CARDTYPE_MEM_HC				2
#define CARDTYPE_MEM_MMC			3
#define CARDTYPE_IO					4
#define CARDTYPE_COMBO_STD			5
#define CARDTYPE_COMBO_HC			6

/* Voltage Type */
#define VOLTAGE_NORMAL_RANGE		0x1
#define VOLTAGE_LOW_RANGE			0x2

struct __sd_cid_s__ 
{
	unsigned long long resv0			:1;
	unsigned long long crc				:7;
	unsigned long long mdt				:12;
	unsigned long long resv1			:4;
	unsigned long long psn				:32;
	unsigned long long prv				:8;

	unsigned long long pnm				:40;

	unsigned long long oid				:16;
	unsigned long long mid				:8;
};
typedef struct __sd_cid_s__  sd_cid_t;
  
struct __csd_10_s__ 
{
	unsigned int resv0					:1;
	unsigned int crc					:7;
	unsigned int resv1					:2;
	unsigned int fileFmt				:2;
	unsigned int tmpWrProtect			:1;
	unsigned int permWrProtect			:1;
	unsigned int copy					:1;
	unsigned int fileFmtGrp				:1;

	unsigned int resv2					:5;
	unsigned int wrBlPartial			:1;
	unsigned int wrBlLen				:4;
	unsigned int r2wFact				:3;
	unsigned int resv3					:2;
	unsigned int wpGrpEnable			:1;

	unsigned long long wpGrpsize		:7;
	unsigned long long sectorSize		:7;
	unsigned long long eraseBlkEn		:1;
	unsigned long long cSizeMult		:3;
	unsigned long long vddWCurrMax		:3;
	unsigned long long vddWCurrMin		:3;
	unsigned long long vddRCurrMax		:3;
	unsigned long long vddRCurrMin		:3;
	unsigned long long cSize			:12;
	unsigned long long resv4			:2;
	unsigned long long dsrImp			:1;
	unsigned long long rdBlkMisAlign	:1;
	unsigned long long wrBlkMisAlign	:1;
	unsigned long long rdBlPartial		:1;
	unsigned long long rdBlkLen			:4;
	unsigned long long ccc				:12;
};
typedef struct __csd_10_s__  csd_10_t;

struct __csd_20_s__ 
{
	unsigned int resv0					:1;
	unsigned int crc					:7;

	unsigned int resv1					:2;
	unsigned int fileFmt				:2;
	unsigned int tmpWrProtect			:1;
	unsigned int permWrProtect			:1;
	unsigned int copy					:1;
	unsigned int fileFmtGrp				:1;
	unsigned int resv2					:5;
	unsigned int wrBlPartial			:1;
	unsigned int wrBlLen				:4;
	unsigned int r2wFactor				:3;
	unsigned int resv3					:2;
	unsigned int wpGrpEnable			:1;

	unsigned long long wpGrpSize		:7;
	unsigned long long sectorSize		:7;
	unsigned long long eraseBlkLen		:1;
	unsigned long long resv4			:1;
	unsigned long long cSize			:22;
	unsigned long long resv5			:6;
	unsigned long long dsrImp			:1;
	unsigned long long rdBlkMisAlign	:1;
	unsigned long long wrBlkMisAlign	:1;
	unsigned long long rdBlPartial		:1;
	unsigned long long rdBlLen			:4;
	unsigned long long ccc				:12;

};
typedef struct __csd_20_s__  csd_20_t;

struct __sd_csd_s__ 
{

	union {
		csd_10_t v10;
		csd_20_t v20;
	} csdSub;

	unsigned int tranSpeed				:8;
	unsigned int nsac					:8;
	unsigned int taac					:8;
	unsigned int resv0					:6;
	unsigned int csdStructure			:2;
}  __attribute ((packed));
typedef struct __sd_csd_s__  sd_csd_t;
  
struct __card_status_s__ 
{
	unsigned int resv0					:3;
	unsigned int akeSeqErr				:1;
	unsigned int resv1					:1;
	unsigned int appCmd					:1;
	unsigned int resv2					:2;
	unsigned int rdyForData				:1;
	unsigned int currStat				:4;
	unsigned int eraseReset				:1;
	unsigned int cardEccDisabled		:1;
	unsigned int wpEraseSkip			:1;
	unsigned int csdOverwrite			:1;
	unsigned int resv3					:2;
	unsigned int error					:1;
	unsigned int ccError				:1;
	unsigned int cardEccFailed			:1;
	unsigned int illegalCmd				:1;
	unsigned int comCrcErr				:1;
	unsigned int lockUnlockFail			:1;
	unsigned int cardIsLocked			:1;
	unsigned int wpViolation			:1;
	unsigned int eraseParam				:1;
	unsigned int eraseSeqErr			:1;
	unsigned int blkLenErr				:1;
	unsigned int addressErr				:1;
	unsigned int outOfRange				:1;
};
typedef struct __card_status_s__  card_status_t;

struct __mem_ocr_s__ 
{
	unsigned int ocr					:24;
	unsigned int resv0					:6;
	unsigned int hcs					:1;
	unsigned int memRdy					:1;
};
typedef struct __mem_ocr_s__  mem_ocr_t;

struct __io_ocr_s__ 
{
	unsigned int ocr					:24;
	unsigned int resv0					:3;
	unsigned int memPresent				:1;
	unsigned int nof					:3;
	unsigned int ioRdy					:1;
};
typedef struct __io_ocr_s__  io_ocr_t;

struct __io_r5_s__ 
{
	unsigned int rwData					:8;
	unsigned int respFlag				:8;
	unsigned int resv					:16;
};
typedef struct __io_r5_s__  io_r5_t;

struct __sd_scr_s__ 
{
	unsigned int manufactureResv;
	unsigned int resv0					:16;
	unsigned int busWidth				:4;
	unsigned int security				:3;
	unsigned int dsAfterErase			:1;
	unsigned int spec					:4;
	unsigned int scrStructure			:4;
};
typedef struct __sd_scr_s__  sd_scr_t;

#define CSD_LENGTH				(4)

typedef struct __sd_card_s__ 
{

	unsigned int cardType;
	unsigned int blkAddrMode;

    // Common Card Definitions
	sd_cid_t cid;

    // Flag for Memory initialized status
	unsigned int memInitDone;		

    // Card Status
	card_status_t cardStatus;

    // Memory Card Definitions
	unsigned int memRca;
	mem_ocr_t memOcr;	
} sdcard_t;

struct __sd_status_s__ 
{
	unsigned int resv0[12];				// 384 bits

	unsigned long long resv1			:16;
	unsigned long long eraseOffset		:2;
	unsigned long long eraseTimeout		:6;
	unsigned long long eraseSize		:16;
	unsigned long long resv2			:4;
	unsigned long long auSize			:4;
	unsigned long long performanceMove	:8;
	unsigned long long speedClass		:8;

	unsigned int sizeOfProtArea;

	unsigned int sdCardType				:16;
	unsigned int resv3					:13;
	unsigned int securedMode			:1;
	unsigned int dataBusWidth			:2;
};
typedef struct __sd_status_s__ sd_status_t;

#define CMD_TYPE_NORMAL				    0
#define CMD_TYPE_ACMD				    1

/* SDC_CMD */
/* Standard SD 2.0 Commands						Type	Arguments			Response	*/
/* Class 0 */
#define CMD0_GO_IDLE_STATE				0	/*	bc										*/
#define CMD1_MMC_SEND_OP_COND			1	/*	bcr		[23:0] OCR			R3			*/
#define CMD2_ALL_SEND_CID				2	/*	bcr							R2			*/
#define CMD3_SEND_RELATIVE_ADDR			3	/*	bcr							R6			*/
#define CMD4_SET_DSR					4	/*	bc		[31:16] DSR						*/
#define CMD6_MMC_SWITCH				    6	/*	ac		[1:0] Bus width		R1B			*/
#define CMD7_SELECT_CARD				7	/*	ac		[31:16] RCA			R1b			*/
#define CMD8_SEND_IF_COND				8	/*	bcr		[11:8] VHS			R7			*/
#define CMD8_MMC_SEND_EXT_CSD		    8	/*	adtc		[31:0] stuff bits			R1		*/
#define CMD9_SEND_CSD					9	/*	ac		[31:16] RCA			R2			*/
#define CMD10_SEND_CID					10	/*	ac		[31:16] RCA			R2			*/
#define CMD12_STOP_TRANSMISSION			12	/*	ac							R1b			*/
#define CMD13_SEND_STATUS				13	/*	ac		[31:16] RCA			R1b			*/
#define CMD15_GO_INACTIVE_STATE			15	/*	ac		[31:16] RCA						*/
	
/* Class 2 */
#define CMD16_SET_BLOCKLEN				16	/*	ac		[31:0] blk len		R1			*/
#define CMD17_READ_SINGLE_BLOCK			17	/*	adtc	[31:0] data addr.	R1			*/
#define CMD18_READ_MULTIPLE_BLOCK		18	/*	adtc	[31:0] data addr.	R1			*/

/* Class 4 */
#define	CMD24_WRITE_BLOCK				24	/*	adtc	[31:0] data addr.	R1			*/
#define CMD25_WRITE_MULTIPLE_BLOCK		25	/*	adtc	[31:0] data addr.	R1			*/
#define CMD27_PROGRAM_CSD				27	/*	adtc						R1			*/

/* Class 6 */
#define CMD28_SET_WRITE_PROT			28	/*	ac		[31:0] data addr.	R1b			*/
#define CMD29_CLR_WRITE_PROT			29	/*	ac		[31:0] data addr.	R1b			*/
#define CMD30_SEND_WRITE_PROT			30	/*	adtc	[31:0] prot addr.	R1			*/

/* Class 5 */
#define CMD32_ERASE_WR_BLK_START		32	/*	ac		[31:0] data addr.	R1			*/
#define CMD33_ERASE_WR_BLK_END			33	/*	ac		[31:0] data addr.	R1			*/
#define CMD35_ERASE_WR_BLK_START		35	/*	ac		[31:0] data addr.	R1			*/
#define CMD36_ERASE_WR_BLK_END			36	/*	ac		[31:0] data addr.	R1			*/
#define CMD38_ERASE						38	/*	ac							R1b			*/

/* Class 7 */
#define CMD42_LOCK_UNLOCK				42	/*	ac		[31:0] Reserved		R1			*/

/* Class 8 */
#define CMD55_APP_CMD					55	/*	ac		[31:16] RCA			R1			*/
#define CMD56_GEN_CMD					56	/*	adtc	[0] RD/WR			R1			*/

/* Application Specific Cmds */
#define ACMD6_SET_BUS_WIDTH				6	/*	ac		[1:0] Bus width		R1			*/
#define ACMD13_SD_STATUS				13	/*	adtc						R1			*/
#define ACMD22_SEND_NUM_WR_BLOCKS		22	/*	adtc						R1			*/
#define ACMD23_SET_WR_BLK_ERASE_COUNT	23	/*	ac		[22:0] Blk num.		R1			*/
#define ACMD41_SD_SEND_OP_COND			41	/*	bcr		[23:0] OCR			R3			*/
#define ACMD42_SET_CLR_CARD_DETECT		42	/*	ac		[0] Set cd			R1			*/
#define ACMD51_SEND_SCR					51	/*	adtc						R1			*/

/* IO Card Commands */
#define CMD5_IO_SEND_OP_COND			5	/*	ac		[24:0] OCR			R4			*/
#define CMD52_IO_RW_DIRECT				52	/*	ac							R5			*/
#define CMD53_IO_RW_EXTENDED			53	/*	ac							R5			*/

/* Checksum Option for fill_chksum */
#define CHKSUM_OPT_NONE			        (0x00)
#define CHKSUM_OPT_GPD			        (0x01)
#define CHKSUM_OPT_BD			        (0x02)
#define CHKSUM_OPT_ERR_IN_GPD	        (0x04)
#define CHKSUM_OPT_ERR_IN_BD	        (0x08)

#define ACMD41_BUS_WIDTH_1				(0x00)
#define ACMD41_BUS_WIDTH_4				(0x02)
#define ACMD41_BUS_WIDTH_8				(0xFF)	//TODO: UNDEFINED!!

// MMC CMD6 Argument :
// (1) Bus Width Selection :
// Access bits = 0x03 (Write byte), Index = 0xB7 = 183, Value = 0(1bit), 1(4bits), 2(8bits)
#define MMC_CMD6_ARG_1BIT_BUS            0x03B70000
#define MMC_CMD6_ARG_4BIT_BUS            0x03B70100
#define MMC_CMD6_ARG_8BIT_BUS            0x03B70200

// (2) High SPeed Mode Selection :
// Access bits = 0x03 (Write byte), Index = 0xB9 = 185, Value = 0(26 Mhz max), 1(52 Mhz max)
#define MMC_CMD6_ARG_NORM_SPEED          0x03B90000
#define MMC_CMD6_ARG_HIGH_SPEED          0x03B90100

#endif // #ifndef _MSDC_SLAVE_HW_H_
