/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************

                  ��Ȩ���� (C), 2008-2018, ��˼�뵼�����޹�˾

 ******************************************************************************
  �� �� ��   : smp.h
  �� �� ��   : ����
  ��������   : D2014_03_10

******************************************************************************/

#ifndef __SMP_H__
#define __SMP_H__


#define hard_smp_processor_id()			\
	({						\
		unsigned int cpunum;			\
		__asm__("mrc p15, 0, %0, c0, c0, 5"	\
			: "=r" (cpunum));		\
		cpunum &= 0x03;				\
	})


static inline void smp_cross_call(const struct cpumask *mask)
{
	gic_raise_softirq(mask, 1);
}


#endif /* __SMP_H__ */
