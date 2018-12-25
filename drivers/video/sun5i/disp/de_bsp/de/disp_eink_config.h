#ifndef __DISP_EINK_CONFIG_H__
#define __DISP_EINK_CONFIG_H__

#define SPV     0x800000
#define CKV     0x400000
#define XSTL    0x200000
#define XOE     0x100000
#define MODE    0x008000
#define XLE     0x002000
#define DMASK   0xff030303

#define         EINK_PANEL_WIDTH        800
#define         EINK_PANEL_HEIGHT       600
#define         EINK_PANEL_SCANLINE     608

#define IMAP_WIDTH ((EINK_PANEL_WIDTH / 2) * 3)

#define         ED060SC4                0x01
#define         ED060SC7                0x02
#define         OPM060A1                0x03

#define         EINK_BLANK              0               //每行5个clock 空白区域

#define         EINK_LCD_W              (258)
#define         EINK_LCD_H              (620)

#define         EINK_LSL                10 
#define         EINK_LBL                4
#define         EINK_LDL                200             //200*4 = 800, 
#define         EINK_LEL                44
#define         EINK_HYNC               (EINK_LSL+EINK_LBL+EINK_LEL)        //50

#define         EINK_FSL                5
#define         EINK_FBL                3
#define         EINK_FDL                600             //600
#define         EINK_FEL                12

#define         EINK_VYNC               (EINK_FSL+EINK_FBL+EINK_FEL-1)      //19


#define         EINK_WF_WIDTH           258
#define         EINK_WF_HEIGHT          619

#define         EINK_PANEL_SCAN_RL        0x00000004
#define         EINK_PANEL_SCAN_UD        0x00004000
#define         EINK_PANEL_VGG_EN         0x00040000
#define         EINK_PANEL_VCOM_EN        0x00080000 


#define         COL_MAX           128
#define         ROW_MAX           256                     //16*16 = 256

#define         DU_ROW_MAX        32                      //DU模式下，每个温度有32行

#define         T_MAX             50                      //最大50个温度值

#endif
