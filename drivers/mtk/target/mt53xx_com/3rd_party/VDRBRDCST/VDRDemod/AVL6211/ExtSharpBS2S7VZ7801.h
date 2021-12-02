#ifndef ExtSharpQM1D1C0042_h_h
	#define ExtSharpQM1D1C0042_h_h

	#include "avl_dvbsx.h"
    #include "ITuner.h"

	#ifdef AVL_CPLUSPLUS
extern "C" {
	#endif

    #define SHARPQM1D1C0042_PLL_LOCK		0x40
    #define OSCILLATION_FREQUENCY_MHZ       16
    
	AVL_DVBSx_ErrorCode ExtSharpBS2S7VZ7801_Initialize(struct AVL_Tuner * pTuner);
	AVL_DVBSx_ErrorCode ExtSharpBS2S7VZ7801_GetLockStatus(struct AVL_Tuner * pTuner );
	AVL_DVBSx_ErrorCode ExtSharpBS2S7VZ7801_Lock(struct AVL_Tuner * pTuner);

	#ifdef AVL_CPLUSPLUS
}
	#endif
#endif

