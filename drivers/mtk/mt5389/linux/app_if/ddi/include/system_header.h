#ifndef __SYS_HEADER_H__
#define __SYS_HEADER_H__

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
    #include 파일들
    (File Inclusions)
------------------------------------------------------------------------------*/
/* #include <sys/types.h> */
#include "osa/osadap.h"

/*-----------------------------------------------------------------------------
	상수 정의
	(Constant Definitions)
------------------------------------------------------------------------------*/


#define	IMAGE_MAGIC			0x00696d61		// Magic number for system header.
#define RL_MAGIC_0			0x00524C30		// Not relocated, or no need to relocate.
#define RL_MAGIC_1			0x00524C31		// Relocated from flash rom.
#define RL_MAGIC_2			0x00524C32		// Relocated from ram
#define RL_MAGIC_3			0x00524C33		// Relocated from ram, for test load.

#define	RL_BOOT_MAGIC		0x42434d44		/* "BCMD"					*/
#define	RL_BOOT_magic		0x62636d64		/* "bcmd"					*/
#define RL_BOOT_OFFSET		0x00008000		/* first 0x8000 bytes of CPU*/

#define RL_B2A_BOOT			0x626F6F74		/* "BOOT" : boot after next reset */
#define RL_B2A_DONE			0x646F6E65		/* "DONE" : already booted */

#define SYS_HDR_OFFSET		0x400

#define APPLICATION_NUM		2
#define APPAREA_SIZE		0x01000000		/* 16 MB */
#define APP_MAX_SIZE		(APPAREA_SIZE / APPLICATION_NUM)
/*-----------------------------------------------------------------------------
	매크로 함수 정의
	(Macro Definitions)
------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
    형 정의
    (Type Definitions)
------------------------------------------------------------------------------*/
typedef struct	{					/* LGE CFE 용 System Header					*/
									/* -- Will be stored at 0x000 of SRAM		*/
	unsigned int sh_reset_code[6];	/* 0x00: Power On Reset Jump Code			*/
	unsigned int sh_magic_code[1];  /* 0x18: magic code         				*/
	unsigned int sh_rsvd_0[14];		/* 0x1c: reserved area 0                    */

	unsigned int sh_mtd_magic;		/* 0x54: mtdinfo Magic						*/
	unsigned int sh_reloc_magic;	/* 0x58: Relocation Magic Number			*/

									/* Board and Image Specific Informations	*/
	unsigned int sh_system_clock;	/* 0x5c: System Clock Rate                  */
	unsigned int sh_total_memsz;	/* 0x60: Total Memory Of This system        */
	unsigned int sh_cpu_memsz;		/* 0x64: CPU(system) Memory Size            */

	char		 *sh_build_date;	/* 0x68: Date of image building				*/
	char		 *sh_image_maker;	/* 0x6c: Name of image maker				*/
	char		 *sh_compile_path;	/* 0x70: Path of compilation 				*/
	char		 *sh_ver_string;	/* 0x74: Version string. of Bootrom/App		*/
	char         *sh_full_ver;		/* 0x78: Full version string                */

	unsigned int sh_image_magic;	/* 0x7c: Code Image Header Magic No.		*/

									/* 0x80: Size of this structure				*/
} SystemHeader_t;

#define LG_KERN_SZ			0x2000
#define TASK_COMM_LEN		16
typedef struct {
	unsigned int thId;
	unsigned int osv;
	unsigned int regi_flag;
	unsigned int pc;
	unsigned int sp;
	unsigned int ra;
} thr_info_t;

typedef struct {
	  signed int index;					/*  0x00         */
	char         comm[TASK_COMM_LEN];	/*  0x04 ~  0x13 */
	unsigned int start;					/*  0x14         */
	unsigned int rsvd_1[2];				/*  0x18 ~  0x1C */
	unsigned int null[504];				/*  0x20 ~ 0x7FC */
	thr_info_t	 info[256];				/* 0x400 ~ 0xFFC */
} KernelInfo_t;

#define OTA_VALID_magic		0xbc
typedef struct {
	unsigned int magic;
	unsigned int app_cnt;
	unsigned int area_sz;
	unsigned int app_sz;
	unsigned int index;
} OTA_t;

#ifdef __cplusplus
}
#endif

#endif/*__SYS_HEADER_H__*/
