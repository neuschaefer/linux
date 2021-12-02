#ifndef COMMON_H
#define COMMON_H


#include "x_lint.h"
LINT_EXT_HEADER_BEGIN

#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif

#ifndef __GCC__
#include "x_printf.h"
#include "x_stl_lib.h"
#define malloc x_mem_alloc
//#define free x_mem_free
#define free
#else
#include <stdio.h>
#include <stdlib.h>
#endif

#include "x_mid.h"
#include "x_hal_926.h"
#include "x_bim.h"
#include "v7_pmu.h"

LINT_EXT_HEADER_END

extern void   ConfigPerformanceMonitorControlReg(UINT32 value);
extern UINT32 ReadARMCycleCounter(void);
extern BOOL   IsARMCycleCounterOverflow(void);

#endif
