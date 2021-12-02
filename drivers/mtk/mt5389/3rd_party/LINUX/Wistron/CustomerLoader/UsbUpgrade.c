
//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#ifdef DEFINE_IS_LOG
#undef DEFINE_IS_LOG
#endif
#ifdef CC_MTK_LOADER
#define DEFINE_IS_LOG(level, fmt...)   Printf(fmt)
#else /* CC_MTK_LOADER */
#define DEFINE_IS_LOG	EXTRA_IsLog
#endif /* CC_MTK_LOADER */
#include "x_debug.h"
#include "x_ldr_env.h"
#include "eeprom_if.h"
#include "nand_if.h"
#include "nor_if.h"
#include "pe_if.h"
#include "panel.h"
#include "c_model.h"
#include "aud_if.h"
#include "Waffle_gpio.h"
#if defined(CC_MTK_LOADER) && defined(FLASH_PANEL_NVM_PARTITION)
#include "u_dlm.h"
#endif

//Tag version eerpom offset
#define EEPROM_NVRAM_VERION_OFFSET              0x393 // last (6~13) is edid version
#define EEPROM_NVRAM_VERION_LENGTH              (7)  
#define EEPROM_EDID_VERION_OFFSET               0x370 // last (14~21) is edid version

//---------------------------------------------------------------------------
//Customer inlude file here:
//---------------------------------------------------------------------------
#include "loader_imghdr.h"

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

#define CC_COMBINE_UP_FW_NVM_UPGRADE

#define EEPROM_DLM_UPGRADE_SIZE         (64 * 1024)

#ifdef CC_Support_Sil9285
#define page9                           0xE0
#define EDID_FIFO_ADDR                  0x02
#define EDID_FIFO_DATA                  0x03
#define EDID_FIFO_SEL                   0x04
#define NVM_COMMAND                     0x05
#define NVM_COMMAND_DONE                0x07

/*Main chip I2C define*/
#define Chip_I2C_ChannelId              2       // It CANNOT be changed
#define SIL9187_I2C_TIMING	            0x100
#define I2C_AddrNum 				    1
#endif /* CC_Support_Sil9285 */

typedef struct
{
    UINT8 u1FrontLen;
    CHAR* szFront;
    UINT8 u1BackLen;
    CHAR* szBack;
}  UPGRADE_NAME_T;


//-----------------------------------------------------------------------------
// Prototypes
//-----------------------------------------------------------------------------
#ifdef CC_MTK_LOADER
void _LdrUpgradeDlmCbFunc(UINT32 u4BufSize);
void _LdrUpgradeDlmGetTagVerCbFunc(UCHAR* ucTagName, CHAR *szCurVer);
INT32 _LdrUpgradeDlmCmpTagVerCbFunc(UCHAR* ucTagName, CHAR *szCurVer, CHAR *szUpgVer);
#ifndef CC_FORCE_USB_UPGRADE
static BOOL FlashAQCompVersion(CHAR *szCurVer, CHAR *szUpgVer);
#endif
#endif /* CC_MTK_LOADER */

#ifndef CC_UBOOT
static void FlashPQGetVersion(CHAR *szCurVer);
static void FlashAQGetVersion(CHAR *szCurVer);
static void FlashSysImgGetVersion(CHAR *szCurVer);

extern UINT32 CustomPanelQueyFunc(VOID);

//-----------------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------------
   

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------

void vGetTagVersion(CHAR* ucTagName, CHAR * szVerStr)
{
    if (x_strcmp((CHAR*)ucTagName, "load") == 0) // loader by luis
    {
        // get version from dram
        LDR_ENV_T* prLdrEnv = (LDR_ENV_T*)CC_LDR_ENV_OFFSET;
        x_strncpy(szVerStr, prLdrEnv->u.rHdr.szVersion, 8);
        szVerStr[8]='\0';
    }
    else if (x_strcmp((CHAR*)ucTagName, "firm") == 0) // firmware by luis
    {
        FlashSysImgGetVersion(szVerStr);
    }
    else if (x_strcmp((CHAR*)ucTagName, "pqda") == 0) // PQ data by Norman
    {
        FlashPQGetVersion(szVerStr);
    }
    else if (x_strcmp((CHAR*)ucTagName, "pnel") == 0) // panel data by Cosh
    {
         x_strcpy(szVerStr, PANEL_GetVersion());
    }
    else if (x_strcmp((CHAR*)ucTagName, "aqda") == 0) // AQ data by Andrew
    {
        FlashAQGetVersion(szVerStr);
    }
    else if (x_strcmp((CHAR*)ucTagName, "nvrm") == 0) // eeprom by Jifeng
    {
		EEPROM_Read((UINT64)(EEPROM_NVRAM_VERION_OFFSET),
		            (UINT32)szVerStr, EEPROM_NVRAM_VERION_LENGTH);
		szVerStr[EEPROM_NVRAM_VERION_LENGTH]='\0';
    }
    else if (x_strcmp((CHAR*)ucTagName, "edid") == 0) // EDID by CI
    {
    	EEPROM_Read((UINT64)EEPROM_EDID_VERION_OFFSET, (UINT32)szVerStr, 8);
    }
    else
    {
         // default use force update version
    }

    Printf("vGetTagVersion:[%s, %s]\n", ucTagName, szVerStr);
}


#ifdef CC_MTK_LOADER
#include "loader_if.h"

static INT32 _LdrEepromUpgrade(DLM_ACCESS_T e_flag,
                               UCHAR *pauc_buffer, INT32* pi4_len)
{
    UINT32  u4i;
    UINT32  u4MemLen=0;
    UINT32  u4DataIndex, u4MaskIndex;
    UINT8   uiTmp;
    UINT32  u4BufLen;

    if (NULL == pauc_buffer)
    {
        Printf("\n<NVM Upgrade>:pauc_buffer = NULL\n");
        return DLM_OPEN_FILE_FAILED;
    }

    if (NULL == pi4_len)
    {
        Printf("\n<NVM Upgrade>:pi4_len = NULL\n");
        return DLM_OPEN_FILE_FAILED;
    }

    for (u4i = 0; u4i < 4; u4i++)
    {
        Printf("\n<NVM Upgrade>:The Byte  %d is  = %d\n", u4i, pauc_buffer[u4i]);
        u4MemLen =((pauc_buffer[u4i] << (u4i * 8)) + u4MemLen);
    }

    u4BufLen = (UINT32)*pi4_len;

    Printf("\n<NVM Upgrade>:The Data length is u4MemLen = %d\n", u4MemLen);
    Printf("\n  EEPROM Size is 0x%x \n", EEPROM_GetSize());
    Printf("\n  Buf Length is 0x%x \n", u4BufLen);

    if (u4MemLen > (UINT32)EEPROM_GetSize())
    {
        Printf("\n<NVM Upgrade>:The Buf  Size  ERROR!!!!\n");
        return DLM_OPEN_FILE_FAILED;
    }

    if (u4BufLen != ((u4MemLen * 2) + 4))
    {
        Printf("\n<NVM Upgrade>:The Buf Length=%d  ERROR!!!!\n", u4BufLen);
        return DLM_OPEN_FILE_FAILED;
    }

    u4DataIndex = 4;
    u4MaskIndex = u4MemLen + 4;

    for (u4i = 0; u4i < u4MemLen; u4i++)
    {
        if (pauc_buffer[u4i + u4MaskIndex] == 0)
        {
            continue;
        }
        
        EEPROM_Read((UINT64)(u4i), (UINT32) &uiTmp, 1);
        
        uiTmp = ((uiTmp & (~pauc_buffer[u4i+u4MaskIndex])) | 
                 (pauc_buffer[u4DataIndex + u4i]& pauc_buffer[u4i + u4MaskIndex]));
        EEPROM_Write((UINT64)(u4i), (UINT32) &uiTmp, 1);
        
        Printf("<_LdrEepromUpgrade>: Offset:  0x%2x;  Write Data: 0x%2x\n", u4i, uiTmp);
    }
    return DLM_OK;
}

static INT32 _LdrEdidUpgrade(DLM_ACCESS_T e_flag, UCHAR *pauc_buffer, INT32* ui4_len)
{
    // download raw data to edid, pauc_buffer is raw data, ui4_len is data size
    return 0;
}

void _LdrUpgradeDlmCbFunc(UINT32 u4BufSize)
{
    UNUSED(_LdrUpgradeDlmCbFunc);
    x_dlm_reg_append_callback("nvrm", EEPROM_DLM_UPGRADE_SIZE,  _LdrEepromUpgrade);
    x_dlm_reg_append_callback("edid", u4BufSize,                _LdrEdidUpgrade);
}

// return the current version of corresponding tag
void _LdrUpgradeDlmGetTagVerCbFunc(UCHAR* ucTagName, CHAR *szCurVer)
{
    UNUSED(_LdrUpgradeDlmGetTagVerCbFunc);

    Printf("Get Tag Version: TagName=%s\n", ucTagName);

    if (x_strcmp((CHAR*)ucTagName, "load") == 0)        // loader by luis
    {
        // get version from dram
        LDR_ENV_T* prLdrEnv = (LDR_ENV_T*)CC_LDR_ENV_OFFSET;
        x_strncpy(szCurVer, prLdrEnv->u.rHdr.szVersion, 8);
        szCurVer[8] = '\0';
    }
    else if (x_strcmp((CHAR*)ucTagName, "firm") == 0)   // firmware by luis
    {
        UINT8  u1SysImgPartition;
        IMAGE_UPGRADE_HEADER_T  rImgUpHdr;
        
        u1SysImgPartition = (UINT8)DRVCUST_InitGet(eNANDFlashPartIdSysImgA);
        Loader_ReadNandFlash(u1SysImgPartition, 0, 
                             (VOID*)&rImgUpHdr, sizeof(IMAGE_UPGRADE_HEADER_T));
        x_strncpy(szCurVer, (CHAR*)rImgUpHdr.au1Version, 8);
    }
    else if (x_strcmp((CHAR*)ucTagName, "pqda") == 0)   // PQ data by Norman
    {
    	FlashPQGetVersion(szCurVer);
    }
    else if (x_strcmp((CHAR*)ucTagName, "pnel") == 0)   // panel data by Cosh
    {
        x_strcpy(szCurVer, PANEL_GetVersion());
    }
    else if (x_strcmp((CHAR*)ucTagName, "aqda") == 0)   // AQ data by Andrew
    {
        FlashAQGetVersion(szCurVer);
    }
    else if (x_strcmp((CHAR*)ucTagName, "nvrm") == 0)   // eeprom by Jifeng
    {
        EEPROM_Read((UINT64)(EEPROM_NVRAM_VERION_OFFSET), (UINT32)szCurVer,
                    EEPROM_NVRAM_VERION_LENGTH);
		szCurVer[EEPROM_NVRAM_VERION_LENGTH] = '\0';
    }
    else if (x_strcmp((CHAR*)ucTagName, "edid") == 0)   // EDID by CI
    {
        EEPROM_Read((UINT64)EEPROM_EDID_VERION_OFFSET, (UINT32)szCurVer, 8);
    }
    else
    {
        // default use force update version
        Printf("default use force update version\n");
        x_strcpy(szCurVer, (char*)DRVCUST_InitGet(eLoaderUpgradeDlmForceVer));
    }
}

// compare if corresponding tag could be download or not.
INT32 _LdrUpgradeDlmCmpTagVerCbFunc(UCHAR* ucTagName, CHAR *szCurVer, CHAR *szUpgVer)
{
    static BOOL bInit = TRUE;
    static BOOL bIsSemiStandby = FALSE;
    static UINT32 u4CountryIndex = DRV_CUSTOM_COUNTRY_EU;

    if (bInit)
    {
        bIsSemiStandby = PDWNC_Check_Enter_SemiStandy();
        u4CountryIndex = CustomPCBAQueryCountry();
        bInit = FALSE;
    }

    // Not allow to update if system boots in the semi-standby mode
    if (bIsSemiStandby)
    {
        return 0;
    }
    
    UNUSED(_LdrUpgradeDlmCmpTagVerCbFunc);
    if (u4CountryIndex == DRV_CUSTOM_COUNTRY_EU)
    {
        // In EU model, loader upgrade does not need to do version check.
        return 1;
    }

    // always allow to update
    if ((x_strcmp(szCurVer, (char*)DRVCUST_InitGet(eLoaderUpgradeDlmForceVer)) == 0) ||
        (x_strcmp(szUpgVer, (char*)DRVCUST_InitGet(eLoaderUpgradeDlmForceVer)) == 0) ||
        (*(UINT32*)szCurVer == 0xffffffff))
    {
        return 1;
    }

    Printf("Tag:%s, current ver(%s), upgrade ver(%s)\n", ucTagName, szCurVer, szUpgVer);

#ifndef CC_FORCE_USB_UPGRADE
    // compare here, return 1 as allow, return 0 as NOT allow.
    if (x_strcmp((CHAR*)ucTagName, "load") == 0)        // loader by luis
    {
        return (x_strcmp(szUpgVer, szCurVer) >= 0);
    }
    else if (x_strcmp((CHAR*)ucTagName, "firm") == 0)   // firmware by luis
    {
        return (x_strcmp(szUpgVer, szCurVer) >= 0);
    }
    else if (x_strcmp((CHAR*)ucTagName, "pqda") == 0)   // PQ data by Norman
    {
        return (x_strcmp(szUpgVer, szCurVer) >= 0);
    }
    else if (x_strcmp((CHAR*)ucTagName, "pnel") == 0)   // panel data by Cosh
    {
        return (x_strcmp(szUpgVer, szCurVer) >= 0);
    }
    else if (x_strcmp((CHAR*)ucTagName, "aqda") == 0)   // AQ data by Andrew
    {
        return FlashAQCompVersion(szCurVer, szUpgVer);
    }
    else if (x_strcmp((CHAR*)ucTagName, "nvrm") == 0)   // eeprom by Jifeng
    {
        return (x_strcmp(szUpgVer, szCurVer) >= 0);
    }
    else if (x_strcmp((CHAR*)ucTagName, "edid") == 0)   // EDID by CI
    {
        return (x_strcmp(szUpgVer, szCurVer) >= 0);
    }
#endif

    // this is the default rule
    return 1;
}

#ifndef CC_FORCE_USB_UPGRADE
static BOOL FlashAQCompVersion(CHAR *szCurVer, CHAR *szUpgVer)
{
    UNUSED(FlashAQCompVersion);
    if (x_strcmp(szUpgVer,szCurVer) >= 0)
        return TRUE;
    else
        return FALSE;
}
#endif

#endif /* CC_MTK_LOADER */

static void FlashPQGetVersion(CHAR *szCurVer)
{
	UINT64 u8PqBaseAddr;
	struct tagFileHead aFlashPqFileHead;

    if (DRVCUST_InitGet(eFlagFlashPqEnable))
    {
        // Check if flash PQ address & block size definition.
        if ((((UINT64)DRVCUST_InitGet(eFlashPqBasePartition) << 32) +
            DRVCUST_InitGet(eFlashPqBaseAddress) == 0) ||
            (DRVCUST_InitGet(eFlashPqBlockSize) == 0))
        {
			szCurVer = NULL; // Address error.
        }

        // Get PQ base address.
        u8PqBaseAddr =
            ((UINT64)DRVCUST_InitGet(eFlashPqBasePartition) << 32) +
            DRVCUST_InitGet(eFlashPqBaseAddress) +
            DRVCUST_InitGet(eFlashPqOffset);

        // PQ base address must be 32-bit alignment.
        if ((u8PqBaseAddr % 4) != 0)
        {
			szCurVer = NULL; // Address error.
        }

#ifdef CC_MTK_LOADER
        // Flash api for loader.
	    if (DRVCUST_InitGet(eFlashPqUseNandFalsh))
	    {
	        Loader_ReadNandFlash(DRVCUST_InitGet(eFlashPqBasePartition),
	            u8PqBaseAddr,
	            (VOID *)&aFlashPqFileHead,
	            (UINT32)(sizeof(struct tagFileHead)));
	    }
	    else
	    {
/*	        NOR_Read(u8PqBaseAddr,
	            (UINT32)&aFlashPqFileHead,
	            (UINT32)(sizeof(struct tagFileHead)));*/
	    }
#else // #ifdef CC_MTK_LOADER
        // Flash api for image.
	    if (DRVCUST_InitGet(eFlashPqUseNandFalsh))
	    {
            // Flash api for loader.
	        STORG_SyncNandRead(DRVCUST_InitGet(eFlashPqBasePartition),
	            u8PqBaseAddr,
	            (VOID *)&aFlashPqFileHead,
	            (UINT32)(sizeof(struct tagFileHead)));
	    }
	    else
	    {
/*	        NOR_Read(u8PqBaseAddr,
	            (UINT32)&aFlashPqFileHead,
	            (UINT32)(sizeof(struct tagFileHead)));*/
	    }
#endif // #ifdef CC_MTK_LOADER

        if (x_strcmp((CHAR *) (aFlashPqFileHead.bID), (CHAR *) FLASHPQ_ID) == 0)
        {
            x_strncpy(szCurVer, (CHAR*)aFlashPqFileHead.bQtyXmlVer, 8);
        }
        else
        {
			szCurVer = NULL;
        }
    }
	else
	{
		szCurVer = NULL; // No flashPQ function.
	}
}

static void FlashAQGetVersion(CHAR *szCurVer)
{
#if 0 //temporarily disable AQ get version due to NANRPART_Read issue.
    UNUSED(FlashAQGetVersion);
	szCurVer = NULL;
	(void)szCurVer;
#else
    UINT8 u1AQPartition;
    UINT32 u2AQSize;    
    UINT64 u8Offset;

    UNUSED(FlashAQGetVersion);
    UNUSED(u2AQSize);
    UNUSED(u8Offset);    

    u1AQPartition = (UINT8)DRVCUST_InitGet(eAudioFlashAQPartition);
    if (u1AQPartition == 0xFF)
    {
        szCurVer[0] = '\0';
        LOG(3, "AudioFlashAQPartition is not defined !!\n");
        return;
    }    
    //u8Offset = ((UINT64)u1AQPartition << 32) | ((UINT64)FLASH_AQ_VER_OFFSET);
#ifdef CC_MTK_LOADER
    Loader_ReadNandFlash(u1AQPartition, FLASH_AQ_VER_OFFSET, (VOID*)szCurVer, FLASH_AQ_VER_LENGTH);
    //NANDPART_Read(u8Offset, (UINT32)szCurVer, FLASH_AQ_VER_LENGTH);
#else
    STORG_SyncNandRead(u1AQPartition,FLASH_AQ_VER_OFFSET,(VOID*)szCurVer, FLASH_AQ_VER_LENGTH);
#endif //CC_MTK_LOADER
#endif //if 1

}

static void FlashSysImgGetVersion(CHAR *szCurVer)
{
    UINT8                   u1SysImgPartition;
    UINT64                  u8Offset;
    IMAGE_UPGRADE_HEADER_T  rImgUpHdr;
    UINT32                  u4IfSecondImage = 0;
    DTVCFG_T                rDtvCfg;

    x_memset(&rDtvCfg, 0, sizeof(DTVCFG_T));
    UNUSED(EEPDTV_GetCfg(&rDtvCfg));
    u4IfSecondImage = rDtvCfg.u1Flags2 & DTVCFG_FLAG2_IMAGE_BANK;    // 0 for the first image, 1 for the second one

    if (u4IfSecondImage)
    {
        u1SysImgPartition = (UINT8)DRVCUST_InitGet(eNANDFlashPartIdSysImgB);
    }
    else
    {
        u1SysImgPartition = (UINT8)DRVCUST_InitGet(eNANDFlashPartIdSysImgA);
    }
    
    if (u1SysImgPartition == 0xFF)
    {
        szCurVer[0] ='\0';
        return;
    }

    u8Offset = ((UINT64)u1SysImgPartition << 32);
    NANDPART_Read(u8Offset, (UINT32)&rImgUpHdr, sizeof(IMAGE_UPGRADE_HEADER_T));
    x_strncpy(szCurVer, (CHAR*)rImgUpHdr.au1Version, 8);
}

BOOL CUSTOMER_USBUpgradeMatchFunc(CHAR *szUpgradeName)
{
    UINT8   u1Idx=0;
    UINT8   u1Pos;
    UINT8   u1Len;
    BOOL    bFront, bBack;
    UINT8   ui1_list_size;
        
    static const UPGRADE_NAME_T atUpgradeNameList[] =     
    {
        {19, "sony_tvupdate_2010_", 11, "_uc_dtv.pkg"}, //sony_tvupdate_2010_xxxx_ga_atv.pkg
        {9, "10DTV.pkg", 0, NULL}, //10DTV.pkg
        {0, NULL, 0, NULL}
    };

    ui1_list_size = sizeof(atUpgradeNameList) / sizeof(UPGRADE_NAME_T);
    
    u1Len = x_strlen(szUpgradeName);
    
    for (u1Idx=0; u1Idx < ui1_list_size; u1Idx++)
    {
        bFront = FALSE;
        bBack = FALSE;
        
        if ((atUpgradeNameList[u1Idx].u1FrontLen==0)&&(atUpgradeNameList[u1Idx].u1BackLen==0))
        {
            break;
        }
        
        if (atUpgradeNameList[u1Idx].u1FrontLen)
        {
            if (x_strncmp(szUpgradeName, atUpgradeNameList[u1Idx].szFront, atUpgradeNameList[u1Idx].u1FrontLen) == 0)
            {
                bFront = TRUE;
            }
        }
        else
        {
            bFront = TRUE;
        }

        if (atUpgradeNameList[u1Idx].u1BackLen)
        {
            u1Pos = u1Len - atUpgradeNameList[u1Idx].u1BackLen;
            if (x_strncmp(&szUpgradeName[u1Pos], atUpgradeNameList[u1Idx].szBack, atUpgradeNameList[u1Idx].u1BackLen) == 0)
            {
                bBack = TRUE;
            }
        }
        else
        {
            bBack = TRUE;
        }
        
        if (bFront && bBack)
        {
            return TRUE;
        }
    }
    
    return FALSE;
}

void CustomSetLoaderUsbUpgrade(BOOL bEnable)
{
    UINT32 u4UpgradeForceDisableAddr;
    if (DRVCUST_InitQuery(eUsbUpgradeForceDisable, &u4UpgradeForceDisableAddr) == 0)
    {
        UINT8 u1Disable = (bEnable) ? 0 : 1;
        UNUSED(EEPROM_Write(u4UpgradeForceDisableAddr,
                            (UINT32)((void *)&u1Disable), sizeof(UINT8)));
    }
}

BOOL CustomIsLoaderUsbUpgradeEnable(void)
{
    UINT32 u4UpgradeForceDisableAddr;
    if (DRVCUST_InitQuery(eUsbUpgradeForceDisable, &u4UpgradeForceDisableAddr) == 0)
    {
        UINT8 u1Disable = 0;
        UNUSED(EEPROM_Read(u4UpgradeForceDisableAddr,
                           (UINT32)((void *)&u1Disable), sizeof(UINT8)));
        if (u1Disable != 0)
        {
            // Force disable loader upgrade
            return FALSE;
        }
    }
    
    return TRUE;
}
#endif

