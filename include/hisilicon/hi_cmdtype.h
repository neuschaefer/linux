/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************

                  版权所有 (C), 2009-2019, 海思半导体有限公司

 ******************************************************************************
  文 件 名   : hi_cmdtype.h
  版 本 号   : 初稿
  生成日期   : D2011_11_08

******************************************************************************/
#ifndef __HI_CMDTYPE_H__
#define __HI_CMDTYPE_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

extern hi_uint32 hi_ifm_regapi( hi_uchar8 *puc_funcname, hi_void *pv_callback );
extern hi_uint32 hi_ifm_regso( hi_uchar8 *puc_funcname, hi_void *pv_callback );
extern hi_uint32 hi_hlp_regko( hi_uchar8 *puc_funcname, hi_void* pv_callback );
extern hi_uint32 hi_hlp_unregko( hi_uchar8 *puc_funcname );

/******************************************************************************/
typedef struct {
    hi_void   *pv_callback;//prototype HI_FUNCCALLBACK_EXT
    hi_uchar8  *puc_funcname;//[HI_FUNCNAME_MAXLEN];
}hi_cmdtype_s;

#define HI_CMDTYPE_REGENTRY( func )    { func, (hi_uchar8 *)(#func) }


#define HI_CMDTYPE_INVALIDARG(inmaxlen) \
    if( ( HI_NULL == pv_data ) || ( ui_inlen   != (inmaxlen) ) || ( HI_NULL == pui_outlen ))\
    {\
        hi_printk("\r\n[ERROR INPUT NULL PTR %s %d]!!!!!!!!!!!!!!!!!!\r\n",__FILE__,__LINE__);\
        return HI_RET_INVALID_PARA;\
    }

#define HI_CMDTYPE_REGKO(array)  { \
    hi_uint32 ui_cnt,ui_ret; \
    for( ui_cnt=0; ui_cnt< ( sizeof(array)/sizeof(hi_cmdtype_s) ); ui_cnt++) { \
        ui_ret = hi_hlp_regko(array[ui_cnt].puc_funcname, array[ui_cnt].pv_callback ); \
        if ( HI_RET_SUCC != ui_ret )\
        {\
            hi_printk("\r\n[ERROR CODE=0x%08x %s %d]!!!!!!!!!!!!!!!!!!\r\n[funcname = %s]",ui_ret,__FILE__,__LINE__,array[ui_cnt].puc_funcname);\
            return ui_ret; \
        }\
    } \
}

#define HI_CMDTYPE_UNREGKO(array){ \
    hi_uint32 ui_cnt; \
    for( ui_cnt=0; ui_cnt< ( sizeof(array)/sizeof(hi_cmdtype_s) ); ui_cnt++) { \
        hi_hlp_unregko(array[ui_cnt].puc_funcname); \
    } \
}
    


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_CMDTYPE_H__ */
