/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************

                  版权所有 (C), 2008-2018, 海思半导体有限公司

 ******************************************************************************
  文 件 名   : hi_cache.h
  版 本 号   : 初稿
  生成日期   : D2014_03_09

******************************************************************************/

#ifndef __HI_CACHE_H__
#define __HI_CACHE_H__

#define HI_REG_L2_CTRL              						0x0000
#define HI_REG_L2_AUCTRL            						0x0004
#define HI_REG_L2_STATUS            						0x0008
#define HI_REG_L2_INTMASK           						0x0100
#define HI_REG_L2_MINT          							0x0104
#define HI_REG_L2_RINT          							0x0108
#define HI_REG_L2_INTCLR            						0x010C
#define HI_REG_L2_INTMASK_INT_MON       					0x0110
#define HI_REG_L2_MINT_INT_MON      						0x0114
#define HI_REG_L2_RINT_INT_MON      						0x0118
#define HI_REG_L2_INTCLR_INT_MON        					0x011C
#define HI_REG_L2_INTMASK_EXT_MON       					0x0120
#define HI_REG_L2_MINT_EXT_MON      						0x0124
#define HI_REG_L2_RINT_EXT_MON      						0x0128
#define HI_REG_L2_INTCLR_EXT_MON    						0x012C
#define HI_REG_L2_SYNC              						0x0200
#define HI_REG_L2_INVALID           						0x0204
#define HI_REG_L2_CLEAN         							0x0208
#define HI_REG_L2_INVALID_SYNC								0x0210 
#define HI_REG_L2_CLEAN_SYNC								0x0214
#define HI_REG_L2_MAINT_AUTO            					0x020C
#define HI_REG_L2_DLOCKWAY          						0x0300
#define HI_REG_L2_ILOCKWAY          						0x0304
#define HI_REG_L2_TESTMODE          						0x0400
#define HI_REG_L2_INTTEST           						0x0404
#define HI_REG_L2_INTTEST_INT_MON       					0x0408
#define HI_REG_L2_INTTEST_EXT_MON       					0x040C
#define HI_REG_L2_EVENTTEST         						0x0410
#define HI_REG_L2_REGION0           						0x0500
#define HI_REG_L2_REGION1           						0x0504
#define HI_REG_L2_REGION2           						0x0508
#define HI_REG_L2_REGION3           						0x050C
#define HI_REG_L2_REGION4           						0x0510
#define HI_REG_L2_INT_COUNTER0          					0x0600
#define HI_REG_L2_INT_COUNTER1          					0x0604
#define HI_REG_L2_INT_COUNTER2          					0x0608
#define HI_REG_L2_INT_COUNTER3          					0x060C
#define HI_REG_L2_INT_COUNTER4          					0x0610
#define HI_REG_L2_INT_COUNTER5          					0x0614
#define HI_REG_L2_INT_COUNTER6          					0x0618
#define HI_REG_L2_INT_COUNTER7          					0x061C
#define HI_REG_L2_INT_COUNTER8          					0x0620
#define HI_REG_L2_INT_COUNTER9          					0x0624
#define HI_REG_L2_INT_COUNTER10     						0x0628
#define HI_REG_L2_EXT_COUNTER0          					0x0700
#define HI_REG_L2_EXT_COUNTER1          					0x0704
#define HI_REG_L2_EXT_COUNTER2          					0x0708
#define HI_REG_L2_EXT_COUNTER3          					0x070C
#define HI_REG_L2_EXT_COUNTER4          					0x0710
#define HI_REG_L2_EXT_COUNTER5          					0x0714
#define HI_REG_L2_EXT_COUNTER6          					0x0718
#define HI_REG_L2_EXT_COUNTER7          					0x071C
#define HI_REG_L2_EXT_COUNTER8          					0x0720
#define HI_REG_L2_EXT_COUNTER9          					0x0724
#define HI_REG_L2_EXT_COUNTER10     						0x0728
#define HI_REG_L2_EXT_COUNTER11     						0x072C
#define HI_REG_L2_EXT_COUNTER12     						0x0730
#define HI_REG_L2_EXT_COUNTER13     						0x0734
#define HI_REG_L2_EXT_COUNTER14     						0x0738
#define HI_REG_L2_EXT_COUNTER15     						0x073C
#define HI_REG_L2_EXT_COUNTER16     						0x0740
#define HI_REG_L2_EXT_COUNTER17     						0x0744
#define HI_REG_L2_EXT_COUNTER18     						0x0748
#define HI_REG_L2_EXT_COUNTER19     						0x074C
#define HI_REG_L2_EXT_COUNTER20     						0x0750
#define HI_REG_L2_EXT_COUNTER21     						0x0754
#define HI_REG_L2_EXT_COUNTER22     						0x0758
#define HI_REG_L2_EXT_COUNTER23     						0x075C
#define HI_REG_L2_EXT_COUNTER24     						0x0760
#define HI_REG_L2_EXT_COUNTER25     						0x0764
#define HI_REG_L2_EXT_COUNTER26     						0x0768
#define HI_REG_L2_EXT_COUNTER27     						0x076C
#define HI_REG_L2_SPECIAL_CTRL      						0x0800
#define HI_REG_L2_SPECIAL_CHECK0                    		0x0804
#define HI_REG_L2_SPECIAL_CHECK1                    		0x0808
#define HI_BIT_L2_CTRL_CACHE_ENABLE                			0
#define HI_BIT_L2_AUCTRL_NON_SECURE_INT_CON        			20
#define HI_BIT_L2_AUCTRL_NON_SECURE_LOCK_EN        			19
#define HI_BIT_L2_AUCTRL_OVERRIDE_SECRUE_CHECK     			18
#define HI_BIT_L2_AUCTRL_FORCE_WRITE_ALLOCATE      			16
#define HI_BIT_L2_AUCTRL_SHARED_ATTRIBUTE_OVER_EN  			15
#define HI_BIT_L2_AUCTRL_MONITOR_EN             			13
#define HI_BIT_L2_AUCTRL_EVENT_BUS_EN           			12
#define HI_BIT_L2_AUCTRL_EXCLUSIVE_CACHE_OPER       		11
#define HI_BIT_L2_STATUS_SPNIDEN                   			1
#define HI_BIT_L2_STATUS_IDLE                      			0
#define HI_BIT_L2_INTMASK_AUTO_END                 			14
#define HI_BIT_L2_INTMASK_SLVERR_RB                			13
#define HI_BIT_L2_INTMASK_DECERR_RA                			12
#define HI_BIT_L2_INTMASK_DECERR_WB                			11
#define HI_BIT_L2_INTMASK_SLVERR_WB                			10
#define HI_BIT_L2_INTMASK_DECERR_WA                			9
#define HI_BIT_L2_INTMASK_SLVERR_WA                			8
#define HI_BIT_L2_INTMASK_DECERR_EB                			7
#define HI_BIT_L2_INTMASK_SLVERR_EB                			6
#define HI_BIT_L2_INTMASK_ERRRD                    			5
#define HI_BIT_L2_INTMASK_ERRRT                    			4
#define HI_BIT_L2_INTMASK_ERRWD                    			3
#define HI_BIT_L2_INTMASK_ERRWT                    			2
#define HI_BIT_L2_INTMASK_PARRD                    			1
#define HI_BIT_L2_INTMASK_PARRT                    			0
#define HI_BIT_L2_MINT_AUTO_END                 			14
#define HI_BIT_L2_MINT_SLVERR_RB                			13
#define HI_BIT_L2_MINT_DECERR_RA                			12
#define HI_BIT_L2_MINT_DECERR_WB                			11
#define HI_BIT_L2_MINT_SLVERR_WB                			10
#define HI_BIT_L2_MINT_DECERR_WA                			9
#define HI_BIT_L2_MINT_SLVERR_WA                			8
#define HI_BIT_L2_MINT_DECERR_EB                			7
#define HI_BIT_L2_MINT_SLVERR_EB                			6
#define HI_BIT_L2_MINT_ERRRD                    			5
#define HI_BIT_L2_MINT_ERRRT                    			4
#define HI_BIT_L2_MINT_ERRWD                    			3
#define HI_BIT_L2_MINT_ERRWT                    			2
#define HI_BIT_L2_MINT_PARRD                    			1
#define HI_BIT_L2_MINT_PARRT                    			0
#define HI_BIT_L2_RINT_AUTO_END                 			14
#define HI_BIT_L2_RINT_SLVERR_RB                			13
#define HI_BIT_L2_RINT_DECERR_RA                			12
#define HI_BIT_L2_RINT_DECERR_WB                			11
#define HI_BIT_L2_RINT_SLVERR_WB                			10
#define HI_BIT_L2_RINT_DECERR_WA                			9
#define HI_BIT_L2_RINT_SLVERR_WA                			8
#define HI_BIT_L2_RINT_DECERR_EB                			7
#define HI_BIT_L2_RINT_SLVERR_EB                			6
#define HI_BIT_L2_RINT_ERRRD                    			5
#define HI_BIT_L2_RINT_ERRRT                    			4
#define HI_BIT_L2_RINT_ERRWD                    			3
#define HI_BIT_L2_RINT_ERRWT                    			2
#define HI_BIT_L2_RINT_PARRD                    			1
#define HI_BIT_L2_RINT_PARRT                    			0
#define HI_BIT_L2_INTCLR_AUTO_END                 			14
#define HI_BIT_L2_INTCLR_SLVERR_RB                			13
#define HI_BIT_L2_INTCLR_DECERR_RA                			12
#define HI_BIT_L2_INTCLR_DECERR_WB                			11
#define HI_BIT_L2_INTCLR_SLVERR_WB                			10
#define HI_BIT_L2_INTCLR_DECERR_WA                			9
#define HI_BIT_L2_INTCLR_SLVERR_WA                			8
#define HI_BIT_L2_INTCLR_DECERR_EB                			7
#define HI_BIT_L2_INTCLR_SLVERR_EB                			6
#define HI_BIT_L2_INTCLR_ERRRD                    			5
#define HI_BIT_L2_INTCLR_ERRRT                    			4
#define HI_BIT_L2_INTCLR_ERRWD                    			3
#define HI_BIT_L2_INTCLR_ERRWT                    			2
#define HI_BIT_L2_INTCLR_PARRD                    			1
#define HI_BIT_L2_INTCLR_PARRT                    			0
#define HI_BIT_L2_SYNC_SYNC                       			0
#define HI_BIT_L2_INVALID_WAYADDRESS              			29
#define HI_BIT_L2_INVALID_LINEADDRESS             			5
#define HI_BIT_L2_INVALID_BYADDRESS               			1
#define HI_BIT_L2_CLEAN_WAYADDRESS                			29
#define HI_BIT_L2_CLEAN_LINEADDRESS               			5
#define HI_BIT_L2_CLEAN_BYADDRESS                 			1
#define HI_BIT_L2_MAINT_AUTO_WAYADDRESS           			2
#define HI_BIT_L2_MAINT_AUTO_CLEAN                			1
#define HI_BIT_L2_MAINT_AUTO_START                			0
#define HI_BIT_L2_DLOCKWAY_DATALOCK               			0
#define HI_L2_LOCKWAY_MASK                        			0XFF
#define HI_BIT_L2_ILOCKWAY_INSTRLOCK              			0
#define HI_BIT_L2_TESTMODE_TEST_MODE              			0
#define HI_L2_INTTEST_DECERR_WB                				11
#define HI_L2_INTTEST_SLVERR_WB                				10
#define HI_L2_INTTEST_DECERR_WA                				9
#define HI_L2_INTTEST_SLVERR_WA                				8
#define HI_L2_INTTEST_DECERR_EB                				7
#define HI_L2_INTTEST_SLVERR_EB                				6
#define HI_L2_INTTEST_ERRRD                    				5
#define HI_L2_INTTEST_ERRRT                    				4
#define HI_L2_INTTEST_ERRWD                    				3
#define HI_L2_INTTEST_ERRWT                    				2
#define HI_L2_INTTEST_PARRD                    				1
#define HI_L2_INTTEST_PARRT                    				0
#define HI_BIT_L2_EVENTTEST_DATAREAD_HIT       				8
#define HI_BIT_L2_EVENTTEST_DATAREAD_REQUEST   				7
#define HI_BIT_L2_EVENTTEST_DATAWRITE_HIT      				6
#define HI_BIT_L2_EVENTTEST_DATAWRITE_REQUEST  				5
#define HI_BIT_L2_EVENTTEST_INSTRUT_HIT        				4
#define HI_BIT_L2_EVENTTEST_INSTRUT_REQUEST    				3
#define HI_BIT_L2_EVENTTEST_EVICTION           				2
#define HI_BIT_L2_EVENTTEST_BUFFEREDW_REQUEST  				1
#define HI_BIT_L2_EVENTTEST_WRITEALL_REQUEST   				0
#define HI_L2_SIZE              							0x40000
#define HI_L2_LINE_SIZE         							0x20
#define HI_L2_WAY_NUM           							0x8
#define HI_L2_WAY_SIZE          							(HI_L2_SIZE/HI_L2_WAY_NUM)
#define HI_L2_LINE_NUM          							(HI_L2_WAY_SIZE/HI_L2_LINE_SIZE)

#endif /* __HI_CACHE_H__ */

