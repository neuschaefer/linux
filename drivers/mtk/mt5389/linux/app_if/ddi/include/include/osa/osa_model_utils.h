/******************************************************************************
 *	 DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *	 Copyright(c) 1999 by LG Electronics Inc.
 *
 *	 All rights reserved. No part of this work may be reproduced, stored in a
 *	 retrieval system, or transmitted by any means without prior written
 *	 permission of LG Electronics Inc.
 *****************************************************************************/

/** @file osa_model_utils.h
 * @author		bk1472
 * @version		1.0
 * @date		2008/09/10
 */

#ifndef _OSA_MODEL_UTILS_H_
#define _OSA_MODEL_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*MODEL_CFG_FUNC_T)(char *pEnv, int Len);

typedef struct
{
	char	*pmatStr;
	int		matVal;
} CONF_MAT_T;

typedef struct
{
	char 		*pItemName;
	CONF_MAT_T	*pmatItem;
} CONF_ITEM_T;

typedef struct
{
	char	*pName;
	UINT32	*pFeature;
} MODEL_FEAT_T;

/*-------------------------------------------------------------
 * macro function definitions
 *------------------------------------------------------------*/
/* Configuration Idivisual Value Matching Table */
#define DECLARE_CONF_MAT(_mat)			static CONF_MAT_T _mat[] = {
#define CONF_MAT_ITEM(_value, _mat)		{_value, _mat}
#define END_DECLARE_CONF_MAT()			,{NULL}};

/* Configuration Item assembly */
#define DECLARE_CONF_ITEM(_item)		static CONF_ITEM_T _item[] = {
#define CONF_ITEM_LIST(_item, _list)	{_item, &_list[0]}
#define END_DECLARE_CONF_ITEM()			,{NULL}};

/* Model Feature Item assembly */
#define DECLARE_MODEL_FEAT(_feat)		static MODEL_FEAT_T _feat[] = {
#define MODEL_FEAT_LIST(_feat, _list)	{_feat, (UINT32 *)&_gModelConfig._list}
#define END_DECLARE_MODEL_FEAT()		,{NULL}};

#define _GET_CFG_ITEM_NUM(_ev, _n, _l, i)				OSA_CfgGetModelCfg(_n, NULL, 0, _ev, _l, i)
#define _GET_CFG_ITEM_STR(_ev, _n, _v, _l, i)			OSA_CfgGetModelCfg(_n, (_v), 0, _ev, _l, i)
#define _CHK_DEV_I2C(_ev, _l, i)						OSA_CfgGetModelCfg("[i2c]", NULL, 1, _ev, _l, i)
#define _GET_CFG_AUTORUN(_ev, _l, i)					OSA_CfgGetModelCfg("auto", NULL, 0, _ev, _l, i)

#define SHFT_CNT	12

extern MODEL_FEAT_T		*gpModelFeatTbl;
extern CONF_ITEM_T 		*gpItemList;
extern MODEL_CFG_FUNC_T	pfnModelCfgFunc;
extern char				*gszPath;

extern int				OSA_CfgGetModelCfg(char *pConfName, char *pStr, int devCheck, char *pEnvFile, int fLen, int idx);
extern void				OSA_InitConfiguration(char appName[], int argc, char **argv);

#ifdef __cplusplus
}
#endif

#endif/*_OSA_MODEL_UTILS_H_*/
