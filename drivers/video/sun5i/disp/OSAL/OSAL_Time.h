/*
*************************************************************************************
*                         			eBsp
*					   Operation System Adapter Layer
*
*				(c) Copyright 2006-2010, All winners Co,Ld.
*							All	Rights Reserved
*
* File Name 	: OSAL_time.h
*
* Author 		: javen
*
* Description 	: Time操作
*
* History 		:
*      <author>    		<time>       	<version >    		<desc>
*       javen     	   2010-09-07          1.0         create this word
*
*************************************************************************************
*/
#ifndef  __OSAL_TIME_H__
#define  __OSAL_TIME_H__

/* 定时器 */
#define  OSAL_TIMER_EVENT_TYPE_ONCE       0   /* 一次触发     */
#define  OSAL_TIMER_EVENT_TYPE_PERIOD     1   /* 周期性触发   */

typedef void (* TIMECALLBACK)(void *pArg);

/*
*******************************************************************************
*                     OSAL_CreateTimer
*
* Description:
*    初始化一个timer
*
* Parameters:
*    Period     :  input. 周期时间
*    EventType  :  input. 事件触发的类型，一次还是多次。
*    CallBack   :  input. 回调函数
*    pArg       :  input. 回调函数的参数
* 
* Return value:
*    返回timer句柄
*
* note:
*    void
*
*******************************************************************************
*/
__hdle OSAL_CreateTimer(__u32 Period, __u32 EventType, TIMECALLBACK CallBack, void *pArg);

/*
*******************************************************************************
*                     OSAL_DelTimer
*
* Description:
*    删除timer
*
* Parameters:
*    HTimer  :  input. OSAL_InitTimer申请timer句柄
* 
* Return value:
*    返回成功或者失败
*
* note:
*    void
*
*******************************************************************************
*/
__s32 OSAL_DelTimer(__hdle HTimer);

/*
*******************************************************************************
*                     OSAL_StartTimer
*
* Description:
*    开始timer计时
*
* Parameters:
*    HTimer  :  input. OSAL_InitTimer申请timer句柄
* 
* Return value:
*    返回成功或者失败
*
* note:
*    void
*
*******************************************************************************
*/
__s32 OSAL_StartTimer(__hdle HTimer);

/* 睡眠 */
void OSAL_Sleep(__u32 Milliseconds);	/* 单位：毫秒 */

#endif   //__OSAL_TIME_H__

