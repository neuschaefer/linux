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
#define DEFINE_IS_LOG   GDMA_IsLog       // for LOG use
#include "x_debug.h"
#include "x_util.h"
#include "x_hal_5381.h"
#include "x_os.h"
#include "x_assert.h"
#include "x_typedef.h"
#include "x_stl_lib.h"
#include "x_bim.h"
#include "x_rand.h"
#include "x_timer.h"
#include "x_hal_926.h"
#ifdef __MW_CLI_DEF__
 #include "x_mid_cli.h"
#else
 #include "x_drv_cli.h"
#endif



//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------
#define GDMA_DEBUG
#define GDMA_INTERRUPT_ON     1  

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
//GDMA BASE
#define GDMA_BASE	            0x20001000  //0x20020000   //0x20000000  //0x20004000
//#define VECTOR_GDMA           12    //13      //move to x_bim.h

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------
#define CONST   const
#define STATIC  static
typedef struct {
    UINT32 u4TestLoop;
    UINT32 u1TestType;
    char arThreadName[80];
} T_ThreadArg;

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------
#define CACHE(addr)                                   ((addr) & 0x0fffffff)
#define NONCACHE(addr)                            (((addr) & 0x0fffffff) | 0x30000000)
#define CHECK_IS_CACHED(addr)               ( (((addr) & 0xf0000000) | 0x30000000)? TRUE:FALSE)
#ifdef NDEBUG
  #define LOG_LOG_WARNING_LVL(fmt...)
  #define LOG_LOG_ERROR_LVL(fmt...)
#endif /* NDEBUG */
CLIMOD_DEBUG_FUNCTIONS(GDMA);     // for LOG use
//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------
STATIC VOID _GDMA_SetSrcAddrReg( UINT32 Val );
STATIC VOID _GDMA_SetDstAddrReg( UINT32 Val );
STATIC VOID _GDMA_SetDataLenReg( UINT32 Val );
STATIC VOID _GDMA_SetFire( VOID );

//---------------------------------------------------------------------------
// GDMA Reg Map
//---------------------------------------------------------------------------
//GDMA REG File
//for fillmode
#define REG_GDMA_FILLVALUE    0x002c
#define FILL_MODE_MSK   (1<<12)
#define FILL_MODE_FILL  (1<<12)
#define FILL_MODE_DMA  (0<<12)

#define REG_GDMA_CONFIG		0x0004
#define REG_GDMA_STATUS		0x0008
#define REG_GDMA_FIRE		    0x000c
#define REG_GDMA_DATA_LEN	    0x0010
#define REG_GDMA_SRC_ADDR	    0x0014
#define REG_GDMA_DST_ADDR     0x0018
#define _GDMA_READ32(OFFSET)		IO_READ32(GDMA_BASE,OFFSET)
#define _GDMA_WRITE32(OFFSET,VALUE)	IO_WRITE32(GDMA_BASE,OFFSET,VALUE)
#define _GDMA_WRITE32MSK(OFFSET,VALUE,MASK)	IO_WRITE32MSK(GDMA_BASE,OFFSET,VALUE,MASK)


//---------------------------------------------------------------------------
// GDMA Emulation
//---------------------------------------------------------------------------
//STATIC HANDLE_T h_gdam_stress_test_thread;
STATIC HAL_RAW_TIME_T H_RawTime;
#define GDMA_MAX_STRESS_TEST_THREAD 30
#define GDMA_MAX_STRESS_TEST_LOOP     100
#if 0
#define GDMA_TEST_AREA_LOW_BOUND        0x3579c000  //(0x0579c000)
#define GDMA_TEST_AREA_SIZE             0x3121b000//(0x0121b000)                 
#define GDMA_TEST_AREA_UPPER_BOUND        (GDMA_TEST_AREA_LOW_BOUND+GDMA_TEST_AREA_SIZE)
#else
#define GDMA_TEST_AREA_LOW_BOUND        (0x0579c000)
#define GDMA_TEST_AREA_SIZE                     0xffffff//(0x0121b000)  //(4096)                 
#define GDMA_TEST_AREA_UPPER_BOUND       (GDMA_TEST_AREA_LOW_BOUND+GDMA_TEST_AREA_SIZE)
#endif
#define UNIT_LOCKTIMEOUT    (10*1000)           //10sec

//---------------------------------------------------------------------------
// Static Variables
//---------------------------------------------------------------------------
static BOOL _fgGdmaInit = FALSE;
static HANDLE_T sema_memcp_func_exit;
static HANDLE_T sema_memcp_func_entry;
static HANDLE_T sema_emu_MemcpyAndCompare_entry;


STATIC VOID _GDMA_SetDataLenReg( UINT32 Val )
{
    _GDMA_WRITE32MSK( REG_GDMA_DATA_LEN, Val, 0x00FFFFFF );
    LOG(LOG_NOTE, "[_GDMA_SetDataLenReg] REG_GDMA_DATA_LEN: 0x%08X\n", _GDMA_READ32(REG_GDMA_DATA_LEN) );        
}

STATIC VOID _GDMA_SetSrcAddrReg( UINT32 Val )
{
    _GDMA_WRITE32MSK( REG_GDMA_SRC_ADDR, Val , 0x1FFFFFFF );
    LOG(LOG_NOTE, "[_GDMA_SetSrcAddrReg] REG_GDMA_SRC_ADDR: 0x%08X\n", _GDMA_READ32(REG_GDMA_SRC_ADDR) );        
}

STATIC VOID _GDMA_SetDstAddrReg( UINT32 Val )
{
    _GDMA_WRITE32MSK( REG_GDMA_DST_ADDR, Val , 0x1FFFFFFF );
    LOG(LOG_NOTE, "[_GDMA_SetDstAddrReg] REG_GDMA_DST_ADDR: 0x%08X\n", _GDMA_READ32(REG_GDMA_DST_ADDR) );        
}

STATIC VOID _GDMA_SetFire( VOID )
{
    _GDMA_WRITE32MSK( REG_GDMA_FIRE, 0x00000001, 0x00000001 );
    LOG(LOG_NOTE, "[_GDMA_SetFire] REG_GDMA_FIRE: 0x%08X\n", _GDMA_READ32(REG_GDMA_FIRE) );        
}

STATIC VOID _GDMA_EnableIntBit( VOID )
{
    _GDMA_WRITE32MSK( REG_GDMA_CONFIG, 0x00000001, 0x00000001 );
    LOG(LOG_NOTE, "[_GDMA_EnableIntBit] REG_GDMA_CONFIG: 0x%08X\n", _GDMA_READ32(REG_GDMA_CONFIG) );        
}

static VOID _GDMA_DisableIntBit( VOID )
{
    _GDMA_WRITE32MSK( REG_GDMA_CONFIG, 0x00000000, 0x00000001 );
    LOG(LOG_NOTE, "[_GDMA_DisableIntBit] REG_GDMA_CONFIG: 0x%08X\n", _GDMA_READ32(REG_GDMA_CONFIG) );        
}

STATIC VOID _GDMA_EnableClkGen( VOID )
{
    _GDMA_WRITE32MSK( REG_GDMA_CONFIG, 0x00000080, 0x00000080 );
    LOG(LOG_NOTE, "[_GDMA_EnableClkGen] REG_GDMA_CONFIG: 0x%08X\n", _GDMA_READ32(REG_GDMA_CONFIG) );        
}

static VOID _GDMA_DisableClkGen( VOID )
{
    _GDMA_WRITE32MSK( REG_GDMA_CONFIG, 0x00000000, 0x00000080 );
    LOG(LOG_NOTE, "[_GDMA_DisableClkGen] REG_GDMA_CONFIG: 0x%08X\n", _GDMA_READ32(REG_GDMA_CONFIG) );        
}

STATIC VOID _GDMA_Isr( UINT16 u2Vector_ID)
{
    ASSERT( u2Vector_ID == VECTOR_GDMA );
/*
#ifdef GDMA_DEBUG
    Printf("[_GDMA_Isr] u2Vector_ID=%d\n", u2Vector_ID );
#endif 
*/
    LOG(LOG_NOTE, "[_GDMA_Isr] u2Vector_ID=%d\n", u2Vector_ID );
    VERIFY(BIM_ClearIrq(VECTOR_GDMA));

    //release semaphore
    VERIFY( x_sema_unlock(sema_memcp_func_exit)==OSR_OK);
}

#if (GDMA_INTERRUPT_ON==1)
STATIC VOID _GDMA_WaitTillDmaAvaliable(VOID)
{
    //lock entry semaphore
#if 0
    VERIFY(x_sema_lock_timeout(sema_memcp_func_entry, UNIT_LOCKTIMEOUT)==OSR_OK);
#else
    if (x_sema_lock_timeout(sema_memcp_func_entry, UNIT_LOCKTIMEOUT)!=OSR_OK)
    {
        Printf("[_GDMA_WaitTillDmaAvaliable] Lock sema_memcp_func_entry failed in %d milisec time\n",UNIT_LOCKTIMEOUT);
    }
#endif
}
STATIC VOID _GDMA_WaitTillDmaDone(VOID)
{
   //lock semaphore
#if 0
    VERIFY(x_sema_lock_timeout(sema_memcp_func_exit, UNIT_LOCKTIMEOUT)==OSR_OK);
#else
    if((x_sema_lock_timeout(sema_memcp_func_exit, UNIT_LOCKTIMEOUT)!=OSR_OK))
    {
        Printf("[_GDMA_WaitTillDmaDone] Lock sema_memcp_func_exit failed in %d milisec time\n",UNIT_LOCKTIMEOUT);
    }
#endif
    //---->wake up by Gdma Isr

    //make dma avaliable again
    VERIFY(x_sema_unlock(sema_memcp_func_entry)==OSR_OK);    
}
#endif
void GDMA_EmuInit(void)
{
    VERIFY( x_sema_create(&sema_emu_MemcpyAndCompare_entry, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK)==OSR_OK);       
}

VOID GDMA_Init(VOID)
{
//    static BOOL _fgGdmaInit = FALSE;
    x_os_isr_fct  pfOldIsr ;
    if(  _fgGdmaInit != TRUE )
    {   //first time use
        _GDMA_WRITE32MSK(REG_GDMA_CONFIG, ((UINT32)0x3)<<30 , 0xC0000000 ); //soft reset
        LOG(LOG_NOTE, "[GDMA_Init] Reset: 0x%08X\n", _GDMA_READ32(REG_GDMA_CONFIG) );        
        
        _fgGdmaInit = TRUE;
        
        _GDMA_WRITE32MSK(REG_GDMA_CONFIG, (0x0)<<30 , 0xC0000000 ); //un-reset
        LOG(LOG_NOTE, "[GDMA_Init] Un-Reset: 0x%08X\n", _GDMA_READ32(REG_GDMA_CONFIG) ); 
        
        _GDMA_EnableIntBit();
        _GDMA_EnableClkGen();

        VERIFY( x_sema_create(&sema_memcp_func_entry, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK)==OSR_OK);       
        VERIFY(x_sema_create(&sema_memcp_func_exit, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK)==OSR_OK);       
        
        VERIFY( x_reg_isr( VECTOR_GDMA, _GDMA_Isr, &pfOldIsr )==OSR_OK );

        GDMA_EmuInit();
    } 
    else
    {
        LOG(LOG_NOTE, "was initialized before, will not init .\n");
    }
}

VOID GDMA_Stop(VOID)
{
    //clean flgas
    x_os_isr_fct  pfOldIsr ;
    _fgGdmaInit = FALSE;

    //stop isr
    VERIFY( x_reg_isr( VECTOR_GDMA, NULL, &pfOldIsr )==OSR_OK );
    VERIFY( BIM_DisableIrq(VECTOR_GDMA)==TRUE );

    //delete semaphores
    //x_sema_delete(sema_memcp_func_entry);
    //x_sema_delete(sema_memcp_func_exit);

    _GDMA_DisableIntBit();
    _GDMA_DisableClkGen();
}

VOID * GDMA_MemSet( VOID *s, UINT32 c , UINT32 n)
{
	_GDMA_WRITE32MSK(REG_GDMA_CONFIG, FILL_MODE_FILL , FILL_MODE_MSK ); 
	_GDMA_WRITE32(REG_GDMA_FILLVALUE, (UINT32)c ); //constand to fill
	_GDMA_SetDstAddrReg( (UINT32)s);
	_GDMA_SetDataLenReg( (UINT32)n );
	_GDMA_SetFire();

	return s;
}

VOID GDMA_Query(VOID)
{
    Printf("GDMA_BASE 0x%08X\n", GDMA_BASE );        
    Printf("REG_GDMA_CONFIG %05X: 0x%08X\n", REG_GDMA_CONFIG , _GDMA_READ32(REG_GDMA_CONFIG));        
    Printf("REG_GDMA_STATUS %05X: 0x%08X\n", REG_GDMA_STATUS , _GDMA_READ32(REG_GDMA_STATUS));        
    Printf("REG_GDMA_FIRE %05X: 0x%08X\n", REG_GDMA_FIRE, _GDMA_READ32(REG_GDMA_FIRE));        
    Printf("REG_GDMA_DATA_LEN %05X: 0x%08X\n", REG_GDMA_DATA_LEN, _GDMA_READ32(REG_GDMA_DATA_LEN));        
    Printf("REG_GDMA_SRC_ADDR %05X: 0x%08X\n", REG_GDMA_SRC_ADDR, _GDMA_READ32(REG_GDMA_SRC_ADDR));        
    Printf("REG_GDMA_DST_ADDR %05X: 0x%08X\n", REG_GDMA_DST_ADDR ,_GDMA_READ32(REG_GDMA_DST_ADDR));        
}


VOID _GDMA_DumpMem( UINT32 u4Addr, UINT32 u4Len )   //testing tool
{
    UINT32 u4Idx;

    if( u4Len > 0x1000 )
    {
        u4Len = 0x1000;
    }

    u4Addr &= 0xFFFFFFFC;   //double word alignment
    
    for( u4Idx = 0 ; u4Idx < u4Len ; u4Idx+=16 )
    {
        LOG(LOG_INFO, "0x%08X: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\n"
            ,(u4Addr+u4Idx)
            ,IO_READ8(u4Addr+u4Idx, 0),IO_READ8(u4Addr+u4Idx,1),IO_READ8(u4Addr+u4Idx,2),IO_READ8(u4Addr+u4Idx,3)
            ,IO_READ8(u4Addr+u4Idx, 4),IO_READ8(u4Addr+u4Idx,5),IO_READ8(u4Addr+u4Idx,6),IO_READ8(u4Addr+u4Idx,7)
            ,IO_READ8(u4Addr+u4Idx, 8),IO_READ8(u4Addr+u4Idx,9),IO_READ8(u4Addr+u4Idx,10),IO_READ8(u4Addr+u4Idx,11)
            ,IO_READ8(u4Addr+u4Idx, 12),IO_READ8(u4Addr+u4Idx,13),IO_READ8(u4Addr+u4Idx,14),IO_READ8(u4Addr+u4Idx,15)
        );
    }
}

UINT32 GDMA__NonOverlapMemcpy( VOID *dst, CONST VOID *src, UINT32 n )
{
    UINT32 u4Diff=0;
    BOOL fgIsSrcCached=CHECK_IS_CACHED( (UINT32)src);
    BOOL fgIsDstCached=CHECK_IS_CACHED( (UINT32)dst);

    //----check parameters-----------------------------------
    if( n==0 )  
    {
        Printf("[_GDMA__NonOverlapMemcpy] Invalid, Size equals to 0!\n");
        return 0;
    }

    u4Diff = (dst > src)? (UINT32)dst - (UINT32)src : (UINT32)src - (UINT32)dst ; 
    
    if ( u4Diff < n )
    {
        Printf("[_GDMA__NonOverlapMemcpy] Overlap not allowed!\n");
        return 0;	//no overlap!
    }
    //------------------------------------------------------

    GDMA_Init(); 
	_GDMA_WRITE32MSK(REG_GDMA_CONFIG, FILL_MODE_DMA , FILL_MODE_MSK );  //dma memory copy
   //-----check if gdma engine is avaliable; if not, wait for it--------------
#if (GDMA_INTERRUPT_ON==1)
    _GDMA_WaitTillDmaAvaliable();   //aovid other thread re-enter
#else
  #error polling mode not ready yet
#endif
    //------------------------------------------------------------
    

    if( fgIsSrcCached || fgIsDstCached )
    {
        HalFlushDCache();
    }

    //---------set gdma engine-----------------------------------
    _GDMA_SetSrcAddrReg( (UINT32)src );
    _GDMA_SetDstAddrReg( (UINT32)dst );
    _GDMA_SetDataLenReg( (UINT32)n );
    _GDMA_SetFire();
    //-----------------------------------------------------------

    //-----wait for DMA finish its job--------------
#if (GDMA_INTERRUPT_ON==1)
    _GDMA_WaitTillDmaDone();        //block I/O
#else
  #error polling mode not ready yet
#endif
    //-------------------------------------------

    if( fgIsSrcCached || fgIsDstCached )
    {
        HalFlushInvalidateDCache();
    }

    return n;		//link this to real status later
}

INT32 _GDMA_MemcpyCmd( INT32 i4Argc, CONST CHAR ** szArgv )
{
    UINT32 u4Dst, u4Src, u4Len;
    INT32 i4Ret;
    if( i4Argc < 4 )
    {
        Printf("Arg: u4Dst, u4Src, u4Len" );
        return 0;
    }

    u4Dst = StrToInt( szArgv[1] );
    u4Src = StrToInt( szArgv[2] );
    u4Len = StrToInt( szArgv[3] );
    
    i4Ret= GDMA__NonOverlapMemcpy ( (void *)u4Dst , (void *)u4Src , u4Len );
    Printf("u4Dst=0x%08X, u4Src=0x%08X, u4Len=%d, i4Ret=%d \n", u4Dst, u4Src, u4Len, i4Ret );

    return 0;
} ;

INT32 _GDMA_FillMemCmd( INT32 i4Argc, CONST CHAR ** szArgv )
{
//fim $address $len [$pattern=0xc3]
//fim $address $len $pattern
    UINT32 u4Addr, u4Len, u4Pattern;
    if( i4Argc <3 )
    {
        Printf("Arg: address len [pattern=0xc3]" );
        return 0;
    }

    u4Addr = StrToInt( szArgv[1] );
    u4Len = StrToInt( szArgv[2] );
    if( i4Argc == 3)
    {
        u4Pattern = 0xc3;
    }else
    {
        u4Pattern = StrToInt( szArgv[3] );
    }

    if(u4Len >0x1000)
    { 
        u4Len = 0x1000;
    }
    
    //x_memset( (void *)u4Addr ,  u4Pattern , u4Len );
    GDMA_MemSet( (void *)u4Addr ,  u4Pattern , u4Len );
    LOG(LOG_NOTE, "[_GDMA_FillMemCmd] u4Addr=%08X, u4Pattern=%08X, u4Len=%08X\n", u4Addr ,  u4Pattern , u4Len);        
    _GDMA_DumpMem( u4Addr, u4Len );

    return 0;
}

STATIC INT32 _GDMA_StopCmd( INT32 i4Argc, CONST CHAR ** szArgv )
{
    GDMA_Stop();
    return 0;
}
STATIC INT32 _GDMA_InitCmd( INT32 i4Argc, CONST CHAR ** szArgv )
{
    GDMA_Init();
    return 0;
}
STATIC INT32 _GDMA_QueryCmd( INT32 i4Argc, CONST CHAR ** szArgv )
{
    GDMA_Query();
    return 0;
}
STATIC INT32 _GDMA_DumpMemCmd( INT32 i4Argc, CONST CHAR ** szArgv )
{
    UINT32 u4Addr, u4Len;
    if( i4Argc < 3)
    {
        Printf("Arg: u4Addr, u4Len\n");
    }
    u4Addr = StrToInt( szArgv[1]);
    u4Len =  StrToInt( szArgv[2]);   
    _GDMA_DumpMem(u4Addr,u4Len);
    return 0;
}
#if 0
STATIC INT32 my_memcmp(const VOID *pv_s1, const VOID *pv_s2, SIZE_T z_l)
{
    INT32 i4_d;
    CHAR *ps_s1, *ps_s2;

    if (z_l == 0)
    {
        return(0);
    }

    if (pv_s1 == NULL)
    {
        if (pv_s2 != NULL)
        {
            return(-1);
        }
        else
        {
            return(-2);
        }
    }
    else if (pv_s2 == NULL)
    {
        return(-3);
    }

    ps_s1 = (CHAR *) pv_s1;
    ps_s2 = (CHAR *) pv_s2;
    do
    {
        i4_d = (*ps_s1)  - (*ps_s2);
        if (i4_d != 0)
        {
            Printf("[my_memcmp] compare mismatch at 0x%08X and 0x%08X\n", ps_s1 ,ps_s2);
            return (UINT32)ps_s1;
        }
        ps_s1++;    ps_s2++;
    } while (--z_l > 0);

    return(0);
}
#endif
static UINT32 MemcpyAndCompare( UINT32 dst, UINT32 src, UINT32 size)
{
        UINT32 u4Result;
            //-----ATOMIC Transaction-----
        VERIFY( x_sema_lock(sema_emu_MemcpyAndCompare_entry, X_SEMA_OPTION_WAIT)==OSR_OK );
        //memory copy
            //Printf("GDMA__NonOverlapMemcpy(u4Dst=0x%08X, u4Src=0x%08X, u4Size=0x%08X)\n",u4Dst,u4Src,u4Size);
        VERIFY(GDMA__NonOverlapMemcpy( (VOID *)dst, (VOID *)src, size ));

        //memory compare
#if 0
        u4Result = (UINT32)my_memcmp( (const void *)dst ,(const void *)src ,size );
#else
        u4Result = (UINT32)x_memcmp( (const void *)dst ,(const void *)src ,size );
#endif
        VERIFY( x_sema_unlock(sema_emu_MemcpyAndCompare_entry)==OSR_OK );
        //----------------------------
        return u4Result;
}

STATIC VOID gdam_stress_test_thread_main( VOID * pvArg)
{
    UINT32 u4StressLoop;
    UINT8 u1TestType;
    UINT32 u4Dst, u4Src, u4Size;
    UINT32 u4Result=FALSE;
    INT64 A,B,C;
//    BOOL bTestFail=FALSE;
    char * pchThreadName;
    UINT32 i,j,k;    UNUSED(i);      UNUSED(j);      UNUSED(k);

    T_ThreadArg * pArg= (T_ThreadArg *)pvArg;
    
    u4StressLoop= pArg->u4TestLoop;     //u4StressLoop = *((UINT32 *)pvArg);
    u1TestType = pArg->u1TestType; 
    pchThreadName = pArg->arThreadName;

    //-----parameter check-----
    if( u1TestType>1 ||  u4StressLoop==0 || pchThreadName==NULL )
    {
        return ;
    }

    //Printf( "[gdam_stress_test_thread_main] u4StressLoop=%d\n",u4StressLoop );
    Printf( "[%s] TestType=%d, u4StressLoop=%d\n", pchThreadName, u1TestType, u4StressLoop );

    if(u1TestType==1)   //random
    {
        x_thread_delay( random(5000) );      //before start, random delay up to 5 sec
    }

    while( u4StressLoop-- >0 )
    {
        if(u1TestType ==1 ) //random
        {
            //generate parameters
            u4Dst = random( GDMA_TEST_AREA_UPPER_BOUND - GDMA_TEST_AREA_LOW_BOUND ) +  GDMA_TEST_AREA_LOW_BOUND;
            u4Src = random( GDMA_TEST_AREA_UPPER_BOUND - GDMA_TEST_AREA_LOW_BOUND ) +  GDMA_TEST_AREA_LOW_BOUND;
            A= ABS( (INT64)u4Dst-(INT64)u4Src);
            B= ABS((INT64)GDMA_TEST_AREA_UPPER_BOUND - (INT64)u4Dst);
            C= ABS((INT64)GDMA_TEST_AREA_UPPER_BOUND - (INT64)u4Src);
            u4Size = random(MIN(A, MIN(B,C)));
            u4Size = u4Size?u4Size:1;       //at least 1 byte

            //randomly set src memory area
#if 0
        for(i=0;i<u4Size;i++)
        {
            j=random(0x100);
            x_memset( (void *)(u4Src+i), j , 1 );
            if( (i%16) == 0 )
            {
                LOG(9, "0x%08X:",u4Src+i);
            }
            LOG(9, "%08X ",j);
        }
#endif
            LOG(LOG_NOTE, "[%s] GDMA__NonOverlapMemcpy(u4Dst=0x%08X, u4Src=0x%08X, u4Size=0x%08X)\n",
                pchThreadName ,u4Dst,u4Src,u4Size);
            u4Result = MemcpyAndCompare(u4Dst, u4Src, u4Size);

            if( u4Result!=0 )
                    break;

            x_thread_delay( random(100) );      //random delay
        }
        else
        {
            for( u4Size=1 ; u4Size<=GDMA_TEST_AREA_SIZE ; u4Size++)
            {
                u4Src =  (UINT32)x_mem_alloc(u4Size);
                u4Dst =  (UINT32)x_mem_alloc(u4Size);
                VERIFY( u4Src !=NULL);
                VERIFY( u4Dst !=NULL);
                LOG(LOG_NOTE, "[%s] GDMA__NonOverlapMemcpy(u4Dst=0x%08X, u4Src=0x%08X, u4Size=0x%08X)\n",
                        pchThreadName ,u4Dst,u4Src,u4Size);
                LOG(LOG_INFO, "copy %0d bytes from 0x%08X to 0x%08X \n", u4Size, u4Src, u4Dst );
                u4Result = MemcpyAndCompare(u4Dst, u4Src, u4Size);
                x_mem_free((void*)u4Src);
                x_mem_free((void*)u4Dst);

                if( u4Result!=0 )
                    break;
            }                       
            if( u4Result!=0 )
                break;
        }
    }
    if(u4Result)        //u4Result !=0 <--> compare not ok
    {
        Printf("[%s]!!!!Test Fail!!!!\n", pchThreadName);
    }
    else            //u4Result==0 <--> compare ok
    {
        Printf("[%s]====Test OK====\n", pchThreadName);
    }
}

STATIC INT32 _GDMA_Emul_Cmd( INT32 i4Argc, CONST CHAR ** szArgv )    //single thread, random size
{
    static HANDLE_T * ph_gdam_stress_test_thread;
    int i;     
    UINT32 u4ThreadNum=0 , u4StressLoop=0  ;
    UINT8 u1TestType;
    UINT32 u4RandomSeed;
    char arThreadName[80];
    T_ThreadArg * pThreadArg;

    //-----check parameters------
    if(i4Argc<4)
    {
        Printf("Arg: [TestType=0(sequencial)|1(random)] [u4ThreadNum] [u4StressLoop]\n");
        return 0;
    }
    else
    {
        u1TestType =  StrToInt( szArgv[1] ); u1TestType= (u1TestType>1)?1:u1TestType ;
        u4ThreadNum = StrToInt( szArgv[2] );
        u4StressLoop = StrToInt( szArgv[3] );
        Printf("[_GDMA_Emul_Cmd] TestType=%d, Create %d threads, each of them run %d times\n", u1TestType, u4ThreadNum, u4StressLoop );
    }
    
    //GENERATE RANDOM SEED    
    HAL_GetRawTime(&H_RawTime) ;
    u4RandomSeed = H_RawTime.u4Cycles ;
    srand(u4RandomSeed );
    Printf("u4RandomSeed=0x%08X\n", u4RandomSeed );
    
    //set random algorithm
    //rand_set_algorithm( RAND_KNUTH );     //Slower
    rand_set_algorithm( RAND_SIMPLEST );    
    
    //-----handles of thread-----
    VERIFY( (ph_gdam_stress_test_thread = x_mem_calloc(u4ThreadNum, sizeof(HANDLE_T)))!=NULL );

    for( i=0; i<u4ThreadNum ; i++)
    {
        x_sprintf(arThreadName , "#%dGdmaTest" , i );

        pThreadArg=x_mem_alloc(sizeof(T_ThreadArg));
        pThreadArg->u4TestLoop=u4StressLoop;
        pThreadArg->u1TestType = u1TestType;   
        x_strcpy( pThreadArg->arThreadName , arThreadName );

        VERIFY( x_thread_create(ph_gdam_stress_test_thread+i, 
                    arThreadName,
                    1024,   //stack size
                    128,    //priority
                    gdam_stress_test_thread_main,
                    sizeof(T_ThreadArg),
                    pThreadArg) == OSR_OK);
    }
    return 0;
}
STATIC INT32 _GDMA_Emul_SS_Cmd( INT32 i4Argc, CONST CHAR ** szArgv )    //single thread, random size
{
    //-----parameter check-----
    if( i4Argc<2 )
    {
        Printf("_GDMA_Emul_SS_Cmd u4Loop\n");
        return 0;
    }
    else
    {
        const char * argv[] = 
        {
            "_GDMA_Emul_Cmd",
            "0",    //sequencail
            "1",    //one thread
            szArgv[1],
        };

        _GDMA_Emul_Cmd(4,(CONST CHAR **)argv);
        return 0;
    };
}

STATIC INT32 _GDMA_Emul_SR_Cmd( INT32 i4Argc, CONST CHAR ** szArgv )    //single thread, random size
{
#if 1
    //-----parameter check-----
    if( i4Argc<2 )
    {
        Printf("_GDMA_Emul_SR_Cmd u4Loop\n");
        return 0;
    }
    else
    {
        const char * argv[] = 
        {
            "_GDMA_Emul_Cmd",
            "1",    //random
            "1",    //one thread
            szArgv[1],
        };

        _GDMA_Emul_Cmd(4,(CONST CHAR **)argv);
        return 0;
    };

#else
    static HANDLE_T * ph_gdam_stress_test_thread;
    int i;     
    UINT32 u4ThreadNum = GDMA_MAX_STRESS_TEST_THREAD, u4StressLoop = GDMA_MAX_STRESS_TEST_LOOP;
    UINT32 u4RandomSeed;
    char arThreadName[80];

    //-----check parameters------
    if(i4Argc==1)
    {
        Printf("Arg: [u4ThreadNum] [u4StressLoop]\n");
    }

    if( i4Argc > 1 )
    {
        u4ThreadNum = StrToInt( szArgv[1] );
    }

    if( i4Argc > 2 )
    {
        u4StressLoop = StrToInt( szArgv[2] );
    }
    
    Printf("[_GDMA_Emul_SR_Cmd] Create %d threads, each of them run %d times\n", u4ThreadNum, u4StressLoop );

    //GENERATE RANDOM SEED    
    HAL_GetRawTime(&H_RawTime) ;
    u4RandomSeed = H_RawTime.u4Cycles ;
    srand(u4RandomSeed );
    Printf("u4RandomSeed=0x%08X\n", u4RandomSeed );
    
    //set random algorithm
    //rand_set_algorithm( RAND_KNUTH );     //Slower
    rand_set_algorithm( RAND_SIMPLEST );    
    
    //create n (n>20) threads
    VERIFY( (ph_gdam_stress_test_thread = x_mem_calloc(u4ThreadNum, sizeof(HANDLE_T)))!=NULL );

    for( i=0; i<u4ThreadNum ; i++)
    {
        x_sprintf(arThreadName , "#%dGdmaTest" , i );
        VERIFY( x_thread_create(ph_gdam_stress_test_thread+i, 
                    arThreadName,
                    1024,
                    128,
                    gdam_stress_test_thread_main,
                    sizeof(ThreadArg),
                    pArg) == OSR_OK);
    }
#endif
}
STATIC INT32 _GDMA_Emul_MS_Cmd( INT32 i4Argc, CONST CHAR ** szArgv )    //multiple thread, sequencial size
{
    //-----parameter check-----
    if( i4Argc<2 )
    {
        Printf("_GDMA_Emul_SS_Cmd u4Loop\n");
        return 0;
    }
    else
    {
        const char * argv[] = 
        {
            "_GDMA_Emul_Cmd",
            "0",    //sequencail
            "100",    //multiple thread
            szArgv[1],
        };

        _GDMA_Emul_Cmd(4,(CONST CHAR **)argv);
        return 0;
    };
}
STATIC INT32 _GDMA_Emul_MR_Cmd( INT32 i4Argc, CONST CHAR ** szArgv )    //multiple thread, random size
{
    //-----parameter check-----
    if( i4Argc<2 )
    {
        Printf("_GDMA_Emul_SS_Cmd u4Loop\n");
        return 0;
    }
    else
    {
        const char * argv[] = 
        {
            "_GDMA_Emul_Cmd",
            "1",    //sequencail
            "100",    //multiple thread
            szArgv[1],
        };

        _GDMA_Emul_Cmd(4,(CONST CHAR **)argv);
        return 0;
    };
}

/************************ END OF EMULATION & STRESS TEST **********************************/
STATIC CLI_EXEC_T _arGdmaEmulationCmdTbl[]=
{
    { "ss", "1", _GDMA_Emul_SS_Cmd, NULL, "Single thread, Sequencail size ", CLI_SUPERVISOR },
    { "sr", "2", _GDMA_Emul_SR_Cmd, NULL, "Single thread, Random size ", CLI_SUPERVISOR },
    { "ms", "3", _GDMA_Emul_MS_Cmd, NULL, "Multiple thread, Sequencial size ", CLI_SUPERVISOR },
    { "mr", "4", _GDMA_Emul_MR_Cmd, NULL, "Multiple thread, Random Size ", CLI_SUPERVISOR },
    { "stress", "s", _GDMA_Emul_Cmd, NULL, "Stress test (assign parameters freely) ", CLI_SUPERVISOR },
    { NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR }
};



STATIC CLI_EXEC_T _arGdmaCmdTbl[]=
{
    { "emu", "e", NULL, _arGdmaEmulationCmdTbl, "Emulation", CLI_SUPERVISOR },
    { "init", "i", _GDMA_InitCmd, NULL, "Gdma initialize", CLI_GUEST },
    { "memcpy", "mcp", _GDMA_MemcpyCmd, NULL, "Gdma memory copy", CLI_GUEST },
    { "fillmem", "fim", _GDMA_FillMemCmd, NULL, "Fill a chunk of memory", CLI_GUEST },
    { "dumpmem", "dm", _GDMA_DumpMemCmd, NULL, "Dump a chunk of memory", CLI_GUEST },
    { "stop", "p", _GDMA_StopCmd, NULL, "Stop (destruct) GDMA module", CLI_GUEST },
    { "query", "q", _GDMA_QueryCmd, NULL, "Query for GDMA related registers", CLI_GUEST },
    CLIMOD_DEBUG_CLIENTRY(GDMA),
    { NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR }
};

CLI_MAIN_COMMAND_ITEM(Gdma)
{
    "gdma", 
    NULL, 
    NULL, 
    _arGdmaCmdTbl, 
    "Gdma command", 
    CLI_GUEST
};
