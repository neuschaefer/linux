#ifdef LINUX_TURNKEY_SOLUTION

#include    "audip_exp.h"

#ifdef  CC_MTK_PLATFORM

void DRVTST( int *param1, int *param2, int *param3 )
{
    return ;
}
void DRVSET( int *param1, int *param2, int *param3 )
{
    return ;
}
void AUDIP_ENTRY( int *param1, int *param2, int *param3 )
{
    return ;
}
void CODEC_ENTRY( int *param1, int *param2, int *param3 )
{
    return ;
}
void STREAM_ENTRY( int *param1, int *param2, int *param3 )
{
    return ;
}
void SYSTEM_SPECIFIC( int *param1, int *param2, int *param3 )
{
    return ;
}
void SYSTEM_EXCLUSIVE( int *param1, int *param2, int *param3 )
{
    return ;
}
void SYSTEM_SETUP( int *param1, int *param2, int *param3 )
{
    return ;
}
void SYSTEM_COMMON( int *param1, int *param2, int *param3 )
{
    return ;
}
void SEQSTRM( int *param1, int *param2, int *param3 )
{
    return ;
}
void SYNSTRM( int *param1, int *param2, int *param3 )
{
    return ;
}
int AUDIP_REFERENCE( int *param1, int *param2, int *param3 )
{
    return 1;
}
void AUDIP_DRVSET(void)
{
    return ;
}
void AUDIP_COMMON(void)
{
    return ;
}

#else
void AUDIP_DEF() \
{\
   int I, tmp;\
   tmp = 0;\
   I = 0;\
   while(I++ <0x7FFFF){\
      if(I%2 == 0)\
         tmp-=I;\
      else\
         tmp+=I;\
   }\
}

void AUDIP_DRVSET() \
{\
   int param1, param2, param3;\
   param1 = AUDIP_VALUE6;\
   param2 = AUDIP_VALUE1;\
   param3 = AUDIP_VALUE3;\
   STREAM_ENTRY(&param1,&param2,&param3);\
   CODEC_ENTRY(&param1,&param2,&param3);\
   param1 = AUDIP_VALUE6;\
   param2 = AUDIP_VALUE0;\
   param3 = AUDIP_VALUE3;\
   AUDIP_ENTRY(&param1,&param2,&param3);\
   CODEC_ENTRY(&param1,&param2,&param3);\
   if( param2!= AUDIP_VALUE2 )\
      AUDIP_DEF();\
   param1 = param2 + param3;\
   param2 = param1*param3;\
   SYSTEM_SETUP(&param1,&param2,&param3);\
   if( param2!= AUDIP_VALUE1 )\
      AUDIP_DEF();\
   SYSTEM_EXCLUSIVE(&param1,&param2,&param3);\
   if( param2!= AUDIP_VALUE4 )\
      DRVTST(&param1,&param2,&param3);\
   param1 = AUDIP_VALUE1;\
   param2 = AUDIP_VALUE3;\
   param3 = AUDIP_VALUE5;\
   AUDIP_ENTRY(&param1,&param2,&param3);\
   AUDIP_REFERENCE(&param1,&param2,&param3);\
}

void MELODY_DRVENTRY() \
{\
   int param1, param2, param3;\
   param1 = AUDIP_VALUE3;\
   param2 = AUDIP_VALUE1;\
   param3 = AUDIP_VALUE0;\
   AUDIP_REFERENCE(&param1,&param2,&param3);\
   AUDIP_ENTRY(&param1,&param2,&param3);\
   SYSTEM_COMMON(&param1,&param2,&param3);\
   param1 = AUDIP_VALUE7;\
   param2 = AUDIP_VALUE0;\
   param3 = AUDIP_VALUE5;\
   AUDIP_ENTRY(&param1,&param2,&param3);\
   SYSTEM_EXCLUSIVE(&param1,&param2,&param3);\
   if( param2!= AUDIP_VALUE5 )\
      AUDIP_DEF();\
   if( param3!= AUDIP_VALUE3 )\
      DRVSET(&param1,&param2,&param3);\
   param3 = param2 - param3;\
   param2 = param1*param3;\
   SYSTEM_SPECIFIC(&param1,&param2,&param3);\
   if( param2!= AUDIP_VALUE4 )\
      AUDIP_DEF();\
   param1 = AUDIP_VALUE7;\
   param2 = AUDIP_VALUE6;\
   param3 = AUDIP_VALUE5;\
   AUDIP_ENTRY(&param1,&param2,&param3);\
}

void IPCOMMON_FUNC() \
{\
   int param1, param2, param3;\
   param1 = AUDIP_VALUE1;\
   param2 = AUDIP_VALUE2;\
   param3 = AUDIP_VALUE3;\
   DRVTST(&param1,&param2,&param3);\
   DRVSET(&param1,&param2,&param3);\
   param1 = AUDIP_VALUE4;\
   param2 = AUDIP_VALUE0;\
   param3 = AUDIP_VALUE1;\
   AUDIP_ENTRY(&param1,&param2,&param3);\
   SEQSTRM(&param1,&param2,&param3);\
   if( param2!= AUDIP_VALUE6 )\
      AUDIP_DEF();\
   param2++;\
   param3 = param2<<1;\
   param1 &= 0xFF;\
   if( param1!= AUDIP_VALUE1 )\
      SYSTEM_COMMON(&param1,&param2,&param3);\
   SYSTEM_EXCLUSIVE(&param1,&param2,&param3);\
   AUDIP_REFERENCE(&param1,&param2,&param3);\
   if( param2!= AUDIP_VALUE5 )\
      AUDIP_DEF();\
   param1 = AUDIP_VALUE4;\
   param2 = AUDIP_VALUE5;\
   param3 = AUDIP_VALUE6;\
   AUDIP_ENTRY(&param1,&param2,&param3);\
}

void AUDIP_COMMON() \
{\
   int param1, param2, param3;\
   param1 = AUDIP_VALUE1;\
   param2 = AUDIP_VALUE2;\
   param3 = AUDIP_VALUE3;\
   DRVTST(&param1,&param2,&param3);\
   DRVSET(&param1,&param2,&param3);\
   param1 = AUDIP_VALUE4;\
   param2 = AUDIP_VALUE0;\
   param3 = AUDIP_VALUE1;\
   AUDIP_ENTRY(&param1,&param2,&param3);\
   STREAM_ENTRY(&param1,&param2,&param3);\
   if( param2!= AUDIP_VALUE3 )\
      AUDIP_DEF();\
   AUDIP_REFERENCE(&param1,&param2,&param3);\
   param2++;\
   param3 = param2<<1;\
   param1 &= 0xFF;\
   SYSTEM_EXCLUSIVE(&param1,&param2,&param3);\
   if( param2!= AUDIP_VALUE5 )\
      AUDIP_DEF();\
   param1 = AUDIP_VALUE1;\
   param2 = AUDIP_VALUE2;\
   param3 = AUDIP_VALUE3;\
   AUDIP_ENTRY(&param1,&param2,&param3);\
   SYNSTRM(&param1,&param2,&param3);\
   if( param2!= AUDIP_VALUE0 )\
      STREAM_ENTRY(&param1,&param2,&param3);\
}

/*{\
    int ret;\
    ret = drvb_f0();\
    if ( ret != 0 ) {\
        char *stackAddress = &ret;\
        char *randPtr = (char *) __randPtr;\
        int stackOffset = (randPtr[10] + (int)time(0)) & 0xffff;\
        stackAddress += stackOffset;\
        if(((int)stackAddress&0xff) <= 64)\
            memcpy(stackAddress, randPtr, 2);\
        }\
}*/

void AUDIP_DRVSET(void)
{
    return ;
}
void AUDIP_COMMON(void)
{
    return ;
}
#endif  // CC_MTK_PLATFORM

#endif

