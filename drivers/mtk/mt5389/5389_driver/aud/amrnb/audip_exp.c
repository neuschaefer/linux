#ifdef LINUX_TURNKEY_SOLUTION

#include    "typedef.h"
#include    "audip_exp.h"

#if 0
void DRVTST( int *param1, int *param2, int *param3 ){ \
   *param1 = 0;\
   *param2 = AUDIP_VALUE5;\
   *param3 = 0;\
}\
void DRVSET( int *param1, int *param2, int *param3 ){\
   *param1 = 0;\
   *param2 = AUDIP_VALUE2;\
   *param3 = 0;\
}\
void AUDIP_ENTRY( int *param1, int *param2, int *param3 ){\
   *param1 = 0;\
   *param2 = AUDIP_VALUE2;\
   *param3 = 0;\
}\
void CODEC_ENTRY( int *param1, int *param2, int *param3 ){\
   *param1 = 0;\
   *param2 = AUDIP_VALUE2;\
   *param3 = 0;\
}\
void STREAM_ENTRY( int *param1, int *param2, int *param3 ){\
   *param1 = 0;\
   *param2 = AUDIP_VALUE3;\
   *param3 = 0;\
}\
void SYSTEM_SPECIFIC( int *param1, int *param2, int *param3 ){\
   *param1 = 0;\
   *param2 = AUDIP_VALUE4;\
   *param3 = 0;	\
}\
void SYSTEM_EXCLUSIVE( int *param1, int *param2, int *param3 ){\
   *param1 = 0;\
   *param2 = AUDIP_VALUE5;\
   *param3 = 0;\
}\
void SYSTEM_SETUP( int *param1, int *param2, int *param3 ){   \
   *param1 = 0;\
   *param2 = AUDIP_VALUE1;\
   *param3 = 0;\
}\
void SYSTEM_COMMON( int *param1, int *param2, int *param3 ){\
   *param1 = 0;\
   *param2 = AUDIP_VALUE0;\
   *param3 = 0;\
}\
void SEQSTRM( int *param1, int *param2, int *param3 ){\
   *param1 = 0;\
   *param2 = AUDIP_VALUE6;\
   *param3 = 0;\
}\
void SYNSTRM( int *param1, int *param2, int *param3 ){\
   *param1 = 0;\
   *param2 = AUDIP_VALUE0;\
   *param3 = 0;\
}\
int AUDIP_REFERENCE( int *param1, int *param2, int *param3 ){\
	return 0;\
}
#endif

#endif

