/********************************************************************************************
 *     LEGAL DISCLAIMER
 *
 *     (Header of MediaTek Software/Firmware Release or Documentation)
 *
 *     BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 *     THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED
 *     FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS
 *     ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED,
 *     INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR
 *     A PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY
 *     WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 *     INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK
 *     ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *     NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION
 *     OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
 *
 *     BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE LIABILITY WITH
 *     RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION,
 *     TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE
 *     FEES OR SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.
 ************************************************************************************************/

#include "x_lint.h"
#include "x_drv_cli.h"
#include "x_printf.h"
//#include "x_debug.h"
#include "x_assert.h"
#include "x_hal_arm.h"
#include "x_hal_5381.h"
#include "x_os.h"

#include "x_util.h"
#include "x_stl_lib.h"

#include "gcpu_if.h"

#define DEFINE_IS_LOG   GCPU_IsLog
#include "x_debug.h"
#include "x_rand.h"

#if 1
LINT_SAVE_AND_DISABLE
CLIMOD_DEBUG_FUNCTIONS(GCPU)
LINT_RESTORE
#endif

#define CLI_AR CLI_SUPERVISOR

#ifdef CC_EMULATION

//CSS
INT32 GCPU_EMU_CSSAV(void);
INT32 GCPU_EMU_CSSAUTHDRV(void);
INT32 GCPU_EMU_CSSAUTHDEC(void);
INT32 GCPU_EMU_CSSAUTHBK(void);

//AES
INT32 GCPU_EMU_AESCTRD(void);
INT32 GCPU_EMU_AESOFBD(void);
INT32 GCPU_EMU_AESD(void);
INT32 GCPU_EMU_AESE(void);
INT32 GCPU_EMU_AESG(void);
INT32 GCPU_EMU_AESDPAK(void);
INT32 GCPU_EMU_AESEPAK(void);
INT32 GCPU_EMU_AESDCMP(void);
INT32 GCPU_EMU_AESCMAC(void);
INT32 GCPU_EMU_AESDCBC(void);
INT32 GCPU_EMU_AESECBC(void);

//VCPS
INT32 GCPU_EMU_VCPSHash(void);
INT32 GCPU_EMU_VCPSDPAK(void);
INT32 GCPU_EMU_VCPSEPAK(void);
INT32 GCPU_EMU_VCPSDKBH(void);
INT32 GCPU_EMU_VCPSDHDK(void);
INT32 GCPU_EMU_VCPSDCBC(void);
INT32 GCPU_EMU_VCPSECBC(void);

//AACS
INT32 GCPU_EMU_AACSBDAV(void);
INT32 GCPU_EMU_AACSBEAV(void);
INT32 GCPU_EMU_AACSDTN(void);
INT32 GCPU_EMU_AACSETN(void);
INT32 GCPU_EMU_AACSDHD(void);
INT32 GCPU_EMU_AACSEHD(void);
INT32 GCPU_EMU_AACSDVCALC(void);

//DES
INT32 GCPU_EMU_DESD(void);
INT32 GCPU_EMU_DESE(void);
INT32 GCPU_EMU_DESDMAD(void);
INT32 GCPU_EMU_DESDMAE(void);
INT32 GCPU_EMU_DESCBCD(void);
INT32 GCPU_EMU_DESCBCE(void);

//BDRE
INT32 GCPU_EMU_BDREDBD(void);
INT32 GCPU_EMU_BDREEBD(void);
INT32 GCPU_EMU_BDREDTN(void);
INT32 GCPU_EMU_BDREETN(void);
INT32 GCPU_EMU_BDREBE(void);
INT32 GCPU_EMU_SHA1(void);
INT32 GCPU_EMU_SHA256(void);
INT32 GCPU_EMU_MD5(void);
INT32 GCPU_EMU_SHA224(void);
INT32 GCPU_EMU_MEMCPY(void);
INT32 GCPU_EMU_DMA(void);
INT32 GCPU_EMU_XOR(void);
INT32 GCPU_EMU_ROMBIST(void);
INT32 GCPU_EMU_RNG(void);
INT32 GCPU_EMU_RC4(void);
INT32 GCPU_EMU_PTX(void);
INT32 GCPU_EMU_IOMMU(void);
INT32 GCPU_EMU_IOMMU2(void);
INT32 GCPU_EMU_IOMMU3(void);
INT32 GCPU_EMU_IOMMU4(void);
INT32 GCPU_EMU_IOMMU5(void);

//CPRM
INT32 GCPU_EMU_C2D(void);
INT32 GCPU_EMU_C2E(void);
INT32 GCPU_EMU_C2G(void);
INT32 GCPU_EMU_C2H(void);
INT32 GCPU_EMU_CPPMDPAK(void);
INT32 GCPU_EMU_CPRMDPAK(void);
INT32 GCPU_EMU_CPRMEPAK(void);
INT32 GCPU_EMU_CCIVERIFY(void);

//E-Fuse
INT32 GCPU_EMU_EFUSE(void);

typedef struct
{
    char *pcCmd;
} CMD_T;

static CMD_T au1Cmd[] =
{
    //CSS
    {"g.css.av"},
    {"g.css.drv"},
    {"g.css.dec"},
    {"g.css.bk"},
    //CPPM
    {"g.cppm.c2d"},
    {"g.cppm.c2e"},
    {"g.cppm.c2g"},
    {"g.cppm.c2h"},
    {"g.cppm.cppmdpak"},
    {"g.cppm.dpak"},
    {"g.cppm.epak"},
    {"g.cppm.vfy"},
    //AES
    {"g.aes.c"},
    {"g.aes.o"},
    {"g.aes.d"},
    {"g.aes.e"},
    {"g.aes.g"},
    {"g.aes.dpak"},
    {"g.aes.epak"},
    {"g.aes.dcmp"},
    {"g.aes.cmac"},
    {"g.aes.dcbc"},
    {"g.aes.ecbc"},
    //key protection
/*
    {"g.aes.ekd"},
    {"g.aes.eke"},
    {"g.aes.pkekd"},
    {"g.aes.pkeke"},
    {"g.aes.pkd"},
    {"g.aes.pke"},
    {"g.aes.pkdpak"},
    {"g.aes.pkepak"},
    {"g.aes.pkdcbc"},
    {"g.aes.pkecbc"},
    {"g.aes.pkekdcbc"},
*/
    //VCPS
    {"g.v.h"},
    {"g.v.dpak"},
    {"g.v.epak"},
    {"g.v.dkbh"},
    {"g.v.dhdk"},
    {"g.v.dcbc"},
    {"g.v.ecbc"},
    //AACS

    {"g.aacs.av"},
    {"g.aacs.ave"},
    {"g.aacs.dtn"},
    {"g.aacs.etn"},
    {"g.aacs.dhd"},
    {"g.aacs.ehd"},
    {"g.aacs.dvc"},

    //DES
    {"g.des.d"},
    {"g.des.e"},
    {"g.des.dmad"},
    {"g.des.dmae"},
    {"g.des.cbcd"},
    {"g.des.cbce"},

    //BD
    {"g.b.d"},
    {"g.b.e"},
    {"g.b.dt"},
    {"g.b.et"},
    {"g.b.b"},

    //MISC
    {"g.misc.s1"},
    {"g.misc.s256"},
    {"g.misc.m5"},
    {"g.misc.s224"},
    {"g.misc.xor"},
    {"g.misc.mcp"},
    {"g.misc.r"},
    {"g.misc.d"},
    {"g.misc.rc4"},
    {"g.misc.i"},
    {"g.misc.i2"},
    {"g.misc.i3"},
    {"g.misc.i4"},
//    {"g.misc.i5"},
    {"g.misc.p"},
/*
    {"g.misc.rb"},
*/
//    {"g.ef.eftst"},

//    NULL,
};

static INT32 _Cmd_GCPUTestInit(INT32 i4Argc, const CHAR** aszArgv)
{
    GCPU_Init(0);
    return 0;
}

static INT32 _Cmd_GCPUDramInstTestInit(INT32 i4Argc, const CHAR** aszArgv)
{
    GCPU_Init(1);
    return 0;
}

static INT32 _Cmd_GCPURandomTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 i = (rand() % (sizeof(au1Cmd)/sizeof(CMD_T)));

    Printf("Test Case: 0x%02x - %s\n", i, au1Cmd[i].pcCmd);
    CLI_Parser(au1Cmd[i].pcCmd);

    return 0;
}

//CSS
static INT32 _Cmd_GCPUCSSAVTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_CSSAV();
}

static INT32 _Cmd_GCPUCSSAUTHDRVTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_CSSAUTHDRV();
}

static INT32 _Cmd_GCPUCSSAUTHDECTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_CSSAUTHDEC();
}

static INT32 _Cmd_GCPUCSSAUTHBKPTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_CSSAUTHBK();
}

//AES
static INT32 _Cmd_GCPUAESCTRTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_AESCTRD();
}

static INT32 _Cmd_GCPUAESHDCPTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_AESHDCP();
}

static INT32 _Cmd_GCPUAESOFBTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_AESOFBD();
}

static INT32 _Cmd_GCPUAESDTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_AESD();
}

static INT32 _Cmd_GCPUAESETest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_AESE();
}

static INT32 _Cmd_GCPUAESGTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_AESG();
}


static INT32 _Cmd_GCPUAESDPAKTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_AESDPAK();
}


static INT32 _Cmd_GCPUAESEPAKTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_AESEPAK();
}

static INT32 _Cmd_GCPUAESDCMPTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_AESDCMP();
}

static INT32 _Cmd_GCPUAESCMACTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_AESCMAC();
}

/*
static INT32 _Cmd_GCPUAESCMAC2Test(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_AESCMAC2();
}
*/

static INT32 _Cmd_GCPUAESDCBCTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_AESDCBC();
}

static INT32 _Cmd_GCPUAESECBCTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_AESECBC();
}

/*
static INT32 _Cmd_GCPUAESPKDPAKTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_AESPKDPAK();
}

static INT32 _Cmd_GCPUAESPKEPAKTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_AESPKEPAK();
}

static INT32 _Cmd_GCPUAESPKDCBCTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_AESPKDCBC();
}

static INT32 _Cmd_GCPUAESPKECBCTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_AESPKECBC();
}

static INT32 _Cmd_GCPUAESPKDTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_AESPKD();
}

static INT32 _Cmd_GCPUAESPKETest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_AESPKE();
}


static INT32 _Cmd_GCPUAESEKDTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_AESEKD();
}

static INT32 _Cmd_GCPUAESEKETest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_AESEKE();
}

static INT32 _Cmd_GCPUAESPKEKDTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_AESPKEKD();
}

static INT32 _Cmd_GCPUAESPKEKETest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_AESPKEKE();
}

static INT32 _Cmd_GCPUAESPKEKDCBCTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_AESPKEKDCBC();
}
*/

//VCPS
static INT32 _Cmd_GCPUVCPSHashTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_VCPSHash();
}

static INT32 _Cmd_GCPUVCPSDPAK(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_VCPSDPAK();
}

static INT32 _Cmd_GCPUVCPSEPAK(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_VCPSEPAK();
}

static INT32 _Cmd_GCPUVCPSDKBH(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_VCPSDKBH();
}

static INT32 _Cmd_GCPUVCPSDHDK(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_VCPSDHDK();
}

static INT32 _Cmd_GCPUVCPSDCBC(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_VCPSDCBC();
}

static INT32 _Cmd_GCPUVCPSECBC(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_VCPSECBC();
}

//AACS
static INT32 _Cmd_GCPUAACSBDAVTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_AACSBDAV();
}


static INT32 _Cmd_GCPUAACSBEAVTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_AACSBEAV();
}

static INT32 _Cmd_GCPUAACSDTNTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_AACSDTN();
}

static INT32 _Cmd_GCPUAACSETNTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_AACSETN();
}


static INT32 _Cmd_GCPUAACSDHDTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_AACSDHD();
}


static INT32 _Cmd_GCPUAACSEHDTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_AACSEHD();
}

static INT32 _Cmd_GCPUAACSDVCALCTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_AACSDVCALC();
}


//DES
static INT32 _Cmd_GCPUDESDTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_DESD();
}

static INT32 _Cmd_GCPUDESETest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_DESE();
}

static INT32 _Cmd_GCPUDESDMADTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_DESDMAD();
}

static INT32 _Cmd_GCPUDESDMAETest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_DESDMAE();
}

static INT32 _Cmd_GCPUDESCBCDTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_DESCBCD();
}

static INT32 _Cmd_GCPUDESCBCETest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_DESCBCE();
}

//BDRE
static INT32 _Cmd_GCPUBDREDBDTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_BDREDBD();
}

static INT32 _Cmd_GCPUBDREEDBTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_BDREEBD();
}

static INT32 _Cmd_GCPUBDREDTNTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_BDREDTN();
}

static INT32 _Cmd_GCPUBDREETNTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_BDREETN();
}

static INT32 _Cmd_GCPUBDREBETest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_BDREBE();
}

static INT32 _Cmd_GCPUSHA1Test(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_SHA1();
}

static INT32 _Cmd_GCPUSHA256Test(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_SHA256();
}

static INT32 _Cmd_GCPUMD5Test(INT32 i4Argc, const CHAR** aszArgv)
{
    return GCPU_EMU_MD5();
}

static INT32 _Cmd_GCPUSHA224Test(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_SHA224();
}

static INT32 _Cmd_GCPUMEMCPYTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_MEMCPY();
}

static INT32 _Cmd_GCPUDMATest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_DMA();
}

static INT32 _Cmd_GCPUXORTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_XOR();
}

static INT32 _Cmd_GCPUROMBISTTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_ROMBIST();
}

static INT32 _Cmd_GCPURNGTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_RNG();
}

static INT32 _Cmd_GCPURC4Test(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_RC4();
}

static INT32 _Cmd_GCPUPTXTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_PTX();
}

static INT32 _Cmd_GCPUIOMMUTest(INT32 i4Argc, const CHAR ** szArgv)
{
    GCPU_EMU_IOMMU();
    return 0;
}

static INT32 _Cmd_GCPUIOMMUTest2(INT32 i4Argc, const CHAR ** szArgv)
{
    GCPU_EMU_IOMMU2();
    return 0;
}

static INT32 _Cmd_GCPUIOMMUTest3(INT32 i4Argc, const CHAR ** szArgv)
{
    GCPU_EMU_IOMMU3();
    return 0;
}

static INT32 _Cmd_GCPUIOMMUTest4(INT32 i4Argc, const CHAR ** szArgv)
{
    GCPU_EMU_IOMMU4();
    return 0;
}

static INT32 _Cmd_GCPUIOMMUTest5(INT32 i4Argc, const CHAR ** szArgv)
{
    GCPU_EMU_IOMMU5();
    return 0;
}

static INT32 _Cmd_GCPUTSDesc(INT32 i4Argc, const CHAR ** szArgv)
{
    GCPU_EMU_TSDesc();
    return 0;
}

//CPRM
static INT32 _Cmd_GCPUC2DTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_C2D();
}

static INT32 _Cmd_GCPUC2ETest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_C2E();
}

static INT32 _Cmd_GCPUC2GTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_C2G();
}

static INT32 _Cmd_GCPUC2HTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_C2H();
}

static INT32 _Cmd_GCPUCPPMDPAKTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_CPPMDPAK();
}

static INT32 _Cmd_GCPUCPRMDPAKTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_CPRMDPAK();
}

static INT32 _Cmd_GCPUCPRMEPAKTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_CPRMEPAK();
}

static INT32 _Cmd_GCPUDCICCIVERIFYTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_CCIVERIFY();
}


//E-Fuse
static INT32 _Cmd_GCPUEFTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return GCPU_EMU_EFUSE();
}


static CLI_EXEC_T _arDrvCSSCmdTbl[] =
{
    {"AV",  "av",   _Cmd_GCPUCSSAVTest,             NULL, "AV Test",        CLI_AR},
    {"DRV", "drv",  _Cmd_GCPUCSSAUTHDRVTest,        NULL, "AuthDRV Test",   CLI_AR},
    {"DEC", "dec",  _Cmd_GCPUCSSAUTHDECTest,        NULL, "AuthDEC Test",   CLI_AR},
    {"BK",  "bk",   _Cmd_GCPUCSSAUTHBKPTest,        NULL, "AuthBK Test",    CLI_AR},

	// last cli command record, NULL
 	{NULL, NULL, NULL, NULL, NULL, CLI_AR}
};

static CLI_EXEC_T _arDrvCppmCmdTbl[] =
{
    {"C2D",  "c2d", _Cmd_GCPUC2DTest,               NULL, "C2D Test",           CLI_AR},
    {"C2E",  "c2e", _Cmd_GCPUC2ETest,               NULL, "C2E Test",           CLI_AR},
    {"C2g",  "c2g", _Cmd_GCPUC2GTest,               NULL, "C2G Test",           CLI_AR},
    {"C2h",  "c2h", _Cmd_GCPUC2HTest,               NULL, "C2H Test",           CLI_AR},
    {"CPPMDPAK",  "cppmdpak", _Cmd_GCPUCPPMDPAKTest,NULL, "CPPM DPAK Test",     CLI_AR},
    {"DPAK",  "dpak", _Cmd_GCPUCPRMDPAKTest,        NULL, "CPRM DPAK Test",     CLI_AR},
    {"EPAK",  "epak", _Cmd_GCPUCPRMEPAKTest,        NULL, "CPRM EPAK Test",     CLI_AR},
    {"CCI VFY",  "vfy", _Cmd_GCPUDCICCIVERIFYTest,  NULL, "CCI VFY Test",       CLI_AR},

	// last cli command record, NULL
 	{NULL, NULL, NULL, NULL, NULL, CLI_AR}
};

static CLI_EXEC_T _arDrvAESCmdTbl[] =
{
    {"AESCTR",  "c", _Cmd_GCPUAESCTRTest,           NULL, "AESCTR Test",        CLI_AR},
    {"AESOFB",  "o", _Cmd_GCPUAESOFBTest,           NULL, "AESOFB Test",        CLI_AR},
    {"AESD",  "d", _Cmd_GCPUAESDTest,               NULL, "AESD Test",          CLI_AR},
    {"AESE",  "e", _Cmd_GCPUAESETest,               NULL, "AESE Test",          CLI_AR},
    {"AESG",  "g", _Cmd_GCPUAESGTest,               NULL, "AESG Test",          CLI_AR},
    {"AESDPAK",  "dpak", _Cmd_GCPUAESDPAKTest,      NULL, "AESDPAK Test",       CLI_AR},
    {"AESEPAK",  "epak", _Cmd_GCPUAESEPAKTest,      NULL, "AESEPAK Test",       CLI_AR},
    {"AESDCMP",  "dcmp", _Cmd_GCPUAESDCMPTest,      NULL, "AESDCMP Test",       CLI_AR},
    {"AESCMAC",  "cmac", _Cmd_GCPUAESCMACTest,      NULL, "AESCMAC Test",       CLI_AR},
//    {"AESCMACII",  "cmac2", _Cmd_GCPUAESCMAC2Test,        NULL, "AESCMAC 20 Bytes Test",       CLI_AR},
    {"AESDCBC",  "dcbc", _Cmd_GCPUAESDCBCTest,      NULL, "AESDCBC Test",       CLI_AR},
    {"AESECBC",  "ecbc", _Cmd_GCPUAESECBCTest,      NULL, "AESECBC Test",       CLI_AR},

    {"AESHDCP",  "hdcp", _Cmd_GCPUAESHDCPTest,      NULL, "AESHDCP Test",       CLI_AR},

    //AESPK Group
/*
    {"AESEKD",  "ekd", _Cmd_GCPUAESEKDTest,        NULL, "AESEKD Test",       CLI_AR},
    {"AESEKE",  "eke", _Cmd_GCPUAESEKETest,        NULL, "AESEKE Test",       CLI_AR},
    {"AESPKEKD",  "pkekd", _Cmd_GCPUAESPKEKDTest,        NULL, "AESPKEKD Test",       CLI_AR},
    {"AESPKEKE",  "pkeke", _Cmd_GCPUAESPKEKETest,        NULL, "AESPKEKE Test",       CLI_AR},
    {"AESPKD",  "pkd", _Cmd_GCPUAESPKDTest,        NULL, "AESPKD Test",       CLI_AR},
    {"AESPKE",  "pke", _Cmd_GCPUAESPKETest,        NULL, "AESPKE Test",       CLI_AR},
    {"AESPKDPAK",  "pkdpak", _Cmd_GCPUAESPKDPAKTest,        NULL, "AESPKDPAK Test",       CLI_AR},
    {"AESPKEPAK",  "pkepak", _Cmd_GCPUAESPKEPAKTest,        NULL, "AESPKEPAK Test",       CLI_AR},
    {"AESPKDCBC",  "pkdcbc", _Cmd_GCPUAESPKDCBCTest,        NULL, "AESPKDCBC Test",       CLI_AR},
    {"AESPKECBC",  "pkecbc", _Cmd_GCPUAESPKECBCTest,        NULL, "AESPKECBC Test",       CLI_AR},
    {"AESPKEKDCBC",  "pkekdcbc", _Cmd_GCPUAESPKEKDCBCTest,        NULL, "AESPKECBC Test",       CLI_AR},
*/
	// last cli command record, NULL
 	{NULL, NULL, NULL, NULL, NULL, CLI_AR}
};

static CLI_EXEC_T _arDrvVCPSCmdTbl[] =
{
    {"VCPSH",     "h",    _Cmd_GCPUVCPSHashTest,    NULL, "VCPSH Test",     CLI_AR},
    {"VCPSDPAK",  "dpak", _Cmd_GCPUVCPSDPAK,        NULL, "VCPSDPAK Test",  CLI_AR},
    {"VCPSEPAK",  "epak", _Cmd_GCPUVCPSEPAK,        NULL, "VCPSEPAK Test",  CLI_AR},
    {"VCPSDKBH",  "dkbh", _Cmd_GCPUVCPSDKBH,        NULL, "VCPSDKBH Test",  CLI_AR},
    {"VCPSDHDK",  "dhdk", _Cmd_GCPUVCPSDHDK,        NULL, "VCPSDHDK Test",  CLI_AR},
    {"VCPSDCBC",  "dcbc", _Cmd_GCPUVCPSDCBC,        NULL, "VCPSDCBC Test",  CLI_AR},
    {"VCPSECBC",  "ecbc", _Cmd_GCPUVCPSECBC,        NULL, "VCPSECBC Test",  CLI_AR},

	// last cli command record, NULL
 	{NULL, NULL, NULL, NULL, NULL, CLI_AR}
};

static CLI_EXEC_T _arDrvAACSCmdTbl[] =
{
 	{"BDAV",  "av", _Cmd_GCPUAACSBDAVTest,          NULL, "AACSBDAV Test",  CLI_AR},
 	{"BDAVE",  "ave", _Cmd_GCPUAACSBEAVTest,        NULL, "AACSBDAVE Test", CLI_AR},
 	{"DTN",  "dtn", _Cmd_GCPUAACSDTNTest,           NULL, "AACSDTN Test",   CLI_AR},
 	{"ETN",  "etn", _Cmd_GCPUAACSETNTest,           NULL, "AACSETN Test",   CLI_AR},
 	{"DHD",  "dhd", _Cmd_GCPUAACSDHDTest,           NULL, "AACSDHD Test",   CLI_AR},
 	{"EHD",  "ehd", _Cmd_GCPUAACSEHDTest,           NULL, "AACSEHD Test",   CLI_AR},
 	{"DVCALC",  "dvc", _Cmd_GCPUAACSDVCALCTest,     NULL, "AACSDVCALC Test",CLI_AR},

	// last cli command record, NULL
 	{NULL, NULL, NULL, NULL, NULL, CLI_AR}
};

static CLI_EXEC_T _arDrvDESCmdTbl[] =
{
    {"d",  "d", _Cmd_GCPUDESDTest,                  NULL, "DESD Test",      CLI_AR},
    {"e",  "e", _Cmd_GCPUDESETest,                  NULL, "DESE Test",      CLI_AR},
    {"dmad",  "dmad", _Cmd_GCPUDESDMADTest,         NULL, "DESDMAD Test",   CLI_AR},
    {"dmae",  "dmae", _Cmd_GCPUDESDMAETest,         NULL, "DESDMAE Test",   CLI_AR},
    {"cbcd",  "cbcd", _Cmd_GCPUDESCBCDTest,         NULL, "DESCBCD Test",   CLI_AR},
    {"cbce",  "cbce", _Cmd_GCPUDESCBCETest,         NULL, "DESCBCE Test",   CLI_AR},

	// last cli command record, NULL
 	{NULL, NULL, NULL, NULL, NULL, CLI_AR}
};

static CLI_EXEC_T _arDrvBDRECmdTbl[] =
{
    {"dbd",  "d",  _Cmd_GCPUBDREDBDTest,            NULL, "DBD Test",       CLI_AR},
    {"ebd",  "e",  _Cmd_GCPUBDREEDBTest,            NULL, "EDB Test",       CLI_AR},
    {"dtn",  "dt", _Cmd_GCPUBDREDTNTest,            NULL, "DTN Test",       CLI_AR},
    {"etn",  "et", _Cmd_GCPUBDREETNTest,            NULL, "ETN Test",       CLI_AR},
    {"be",   "b",  _Cmd_GCPUBDREBETest,             NULL, "BE Test",        CLI_AR},

	// last cli command record, NULL
 	{NULL, NULL, NULL, NULL, NULL, CLI_AR}
};

static CLI_EXEC_T _arDrvMiscCmdTbl[] =
{
    {"sha1",  "s1",  _Cmd_GCPUSHA1Test,             NULL, "SHA1 Test",      CLI_AR},
    {"sha256",  "s256",  _Cmd_GCPUSHA256Test,       NULL, "SHA256 Test",    CLI_AR},
    {"md5",  "m5", _Cmd_GCPUMD5Test,                NULL, "MD5 Test",       CLI_AR},
    {"sha224",  "s224", _Cmd_GCPUSHA224Test,        NULL, "SHA224 Test",    CLI_AR},
    {"xor",   "xor",  _Cmd_GCPUXORTest,             NULL, "XOR Test",       CLI_AR},
    {"mcp",   "mcp",  _Cmd_GCPUMEMCPYTest,          NULL, "MEMCPY Test",    CLI_AR},
    {"dma",   "d",  _Cmd_GCPUDMATest,               NULL, "DMA Test",       CLI_AR},
    {"rombist",   "rb",  _Cmd_GCPUROMBISTTest,      NULL, "ROM Test",       CLI_AR},
    {"rng",   "r",  _Cmd_GCPURNGTest,               NULL, "RNG Test",       CLI_AR},
    {"rc4",   "rc4",  _Cmd_GCPURC4Test,             NULL, "RC4 Test",       CLI_AR},
    {"ptx",   "p",  _Cmd_GCPUPTXTest,               NULL, "PTX Test",       CLI_AR},
    {"iommu",   "i",  _Cmd_GCPUIOMMUTest,           NULL, "IOMMU Test",     CLI_AR},
    {"iommu2",   "i2",  _Cmd_GCPUIOMMUTest2,        NULL, "IOMMU Test",     CLI_AR},
    {"iommu3",   "i3",  _Cmd_GCPUIOMMUTest3,        NULL, "IOMMU Test",     CLI_AR},
    {"iommu4",   "i4",  _Cmd_GCPUIOMMUTest4,        NULL, "IOMMU Test",     CLI_AR},
    {"iommu5",   "i5",  _Cmd_GCPUIOMMUTest5,        NULL, "IOMMU Test",     CLI_AR},
    {"tsdesc",   "td",  _Cmd_GCPUTSDesc,            NULL, "TS DESC Test",     CLI_AR},
	// last cli command record, NULL
 	{NULL, NULL, NULL, NULL, NULL, CLI_AR}
};

static CLI_EXEC_T _arDrvEFCmdTbl[] =
{
 	{"EFTest",  "eftst", _Cmd_GCPUEFTest,        NULL, "EF Test",       CLI_AR},
	// last cli command record, NULL
 	{NULL, NULL, NULL, NULL, NULL, CLI_AR}
};
#endif //CC_EMULATION


CLI_EXEC_T _arGCPUCmdTbl[] =
{
    #ifdef CC_EMULATION
    // ========================
    //Driver Verify CLI Cmd
    // ========================
    {"init",    "i", 	_Cmd_GCPUTestInit,  NULL, "", CLI_AR},
    {"draminit",    "di", 	_Cmd_GCPUDramInstTestInit,  NULL, "", CLI_AR},
    {"randomt",    "rt", 	_Cmd_GCPURandomTest,      NULL, "", CLI_AR},
    {"CSS",		"css",	NULL, 	_arDrvCSSCmdTbl,		"css cmd", 	CLI_AR},
    {"Cppm",	"cppm",	NULL, 	_arDrvCppmCmdTbl,		"cppm cmd", CLI_AR},
    {"AES",		"aes",	NULL, 	_arDrvAESCmdTbl,		"aes cmd", 	CLI_AR},
    {"VCPS",	"v",	NULL, 	_arDrvVCPSCmdTbl,		"vcps cmd", CLI_AR},
    {"AACS",	"aacs",	NULL, 	_arDrvAACSCmdTbl,		"aacs cmd", CLI_AR},
    {"DES",		"des",	NULL, 	_arDrvDESCmdTbl,		"des cmd", 	CLI_AR},
    {"BDRE",    "b",	NULL, 	_arDrvBDRECmdTbl,		"bdre cmd",	CLI_AR},
    {"Misc",	"misc",	NULL, 	_arDrvMiscCmdTbl,		"misc cmd", CLI_AR},
    {"EF",		"ef",	NULL, 	_arDrvEFCmdTbl,		"ef cmd", 	CLI_AR},
    #endif //CC_EMULATION

    CLIMOD_DEBUG_CLIENTRY(GCPU),
    // last cli command record, NULL
    {NULL, NULL, NULL, NULL, NULL, CLI_GUEST}
};


LINT_SAVE_AND_DISABLE
CLI_MAIN_COMMAND_ITEM(Gcpu)
{
        "gcpu", "g", NULL, _arGCPUCmdTbl, "GCPU command", CLI_GUEST

};
LINT_RESTORE

