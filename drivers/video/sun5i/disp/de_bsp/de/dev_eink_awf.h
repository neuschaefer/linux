
#ifndef  _DRV_EINK_AWF_H_
#define  _DRV_EINK_AWF_H_

#define    C_HEADER_INFO_OFFSET               0
#define    C_HEADER_TYPE_ID_OFFSET            0                                             //第一个字节为EINK_ID,标示OED or PVI,or size
#define    C_HEADER_VERSION_STR_OFFSET        1
#define    C_HEADER_INFO_SIZE                 128
#define    C_TEMP_TBL_OFFSET                  (C_HEADER_INFO_OFFSET+C_HEADER_INFO_SIZE)      //温度表起始128,长度32
#define    C_TEMP_TBL_SIZE                    32        //预留32个温度区间，每个模块也是32

#define    C_MODE_ADDR_TBL_OFFSET             (C_TEMP_TBL_OFFSET+C_TEMP_TBL_SIZE)             //起始64+32,长度64
#define    C_MODE_ADDR_TBL_SIZE               64

#define    C_INIT_MODE_ADDR_OFFSET            C_MODE_ADDR_TBL_OFFSET
#define    C_GC16_MODE_ADDR_OFFSET            (C_MODE_ADDR_TBL_OFFSET+4)
#define    C_GC4_MODE_ADDR_OFFSET             (C_MODE_ADDR_TBL_OFFSET+8)
#define    C_DU_MODE_ADDR_OFFSET              (C_MODE_ADDR_TBL_OFFSET+12)
#define    C_A2_MODE_ADDR_OFFSET              (C_MODE_ADDR_TBL_OFFSET+16)
#define    C_GC16_LOCAL_MODE_ADDR_OFFSET      (C_MODE_ADDR_TBL_OFFSET+20)
#define    C_GC4_LOCAL_MODE_ADDR_OFFSET       (C_MODE_ADDR_TBL_OFFSET+24)

#define    C_INIT_MODE_OFFSET                 (C_MODE_ADDR_TBL_OFFSET+C_MODE_ADDR_TBL_SIZE)

#define    C_REAL_TEMP_AREA_NUM               15              //实际最大只有15个温度区间

#define    WF_MAX_COL                         256   //  GC16, 16*16 = 256



#endif
