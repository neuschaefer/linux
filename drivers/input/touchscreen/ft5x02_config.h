#ifndef __FT5X02_CONFIG_H__ 
#define __FT5X02_CONFIG_H__ 
/*FT5X02 config*/


#define FT5X02_FIRMWARE_ID			14
#define FT5X02_OTP_PARAM_ID			0
#define FT5X02_CUSTOMER_ID			1
#define FT5X02_CHIPER_ID			2
#define FT5X02_RESOLUTION_X			600
#define FT5X02_RESOLUTION_Y			800
#define FT5X02_LEMDA_X			57
#define FT5X02_LEMDA_Y			50
#define FT5X02_KX			242
#define FT5X02_KY			214
#define FT5X02_DIRECTION			1
#define FT5X02_KX_LR			458
#define FT5X02_KY_UD			406
#define FT5X02_POINTS_SUPPORTED			5

//pb624 touch sensitivity correction; default is 100
#define FT5X02_THGROUP			64

#define FT5X02_THPEAK			60
#define FT5X02_THDIFF			2560
#define FT5X02_MAX_TOUCH_VALUE			1200
#define FT5X02_DRAW_LINE_TH			250
#define FT5X02_PWMODE_CTRL			1
#define FT5X02_PERIOD_ACTIVE			16//报点率 值越大 报点高 值小报点低
#define FT5X02_TIME_ENTER_MONITOR			10
#define FT5X02_PERIOD_MONITOR			40
#define FT5X02_FILTER_FRAME_NOISE			2
#define FT5X02_POWERNOISE_FILTER_TH			0
#define FT5X02_DIFFDATA_HADDLE_VALUE			-100
#define FT5X02_FACE_DETECT_MODE			0
#define FT5X02_FACE_DETECT_STATISTICS_TX_NUM			3
#define FT5X02_FACE_DETECT_PRE_VALUE			20
#define FT5X02_FACE_DETECT_NUM			10
#define FT5X02_FACE_DETECT_LAST_TIME			1000
#define FT5X02_BIGAREA_PEAK_VALUE_MIN			255
#define FT5X02_BIGAREA_DIFF_VALUE_OVER_NUM			30
#define FT5X02_BIGAREA_POINT_AUTO_CLEAR_TIME			3000
#define FT5X02_ABNORMAL_DIFF_VALUE			60
#define FT5X02_ABNORMAL_DIFF_NUM			10
#define FT5X02_ABNORMAL_DIFF_LAST_FRAME			30
#define FT5X02_START_RX			0
#define FT5X02_ADC_TARGET			8500
#define FT5X02_ESD_FILTER_FRAME			2
#define FT5X02_MOVSTH_I			3
#define FT5X02_MOVSTH_N			2
#define FT5X02_MODE			1
#define FT5X02_PMODE			0
#define FT5X02_ERR			0
#define FT5X02_AUTO_CLB_MODE			255
#define FT5X02_STATE			1
#define FT5X02_HIGH_SPEED_TH			221
#define FT5X02_MID_SPEED_TH			85
#define FT5X02_STATIC_TH			156
#define FT5X02_THFALSE_TOUCH_PEAK			5


unsigned char g_ft5x02_tx_num = 15;
unsigned char g_ft5x02_rx_num = 10;
unsigned char g_ft5x02_gain = 10;
unsigned char g_ft5x02_voltage = 3;
unsigned char g_ft5x02_scanselect = 4;//3~15//解决屏跟TP之间的干扰的参数
unsigned char g_ft5x02_tx_order[] = {14,13,12,11,10,9,8,7,6,5,4,3,2,1,0};
unsigned char g_ft5x02_tx_offset = 1;
unsigned char g_ft5x02_tx_cap[] = {23,23,24,25,25,25,25,25,25,25,25,25,25,26,31};
unsigned char g_ft5x02_rx_order[] = {0,1,2,3,4,5,6,7,8,9};
unsigned char g_ft5x02_rx_offset[] = {101,102,85,85,85};
unsigned char g_ft5x02_rx_cap[] = {50,50,50,50,50,53,52,52,50,54};

//qiu tian wei can shu
#define FT5X02_FIRMWARE_ID_QTW			14
#define FT5X02_OTP_PARAM_ID_QTW			0
#define FT5X02_CUSTOMER_ID_QTW			1
#define FT5X02_CHIPER_ID_QTW			2
#define FT5X02_RESOLUTION_X_QTW			600
#define FT5X02_RESOLUTION_Y_QTW				800
#define FT5X02_LEMDA_X_QTW				58
#define FT5X02_LEMDA_Y_QTW				53
#define FT5X02_KX_QTW				242
#define FT5X02_KY_QTW				213
#define FT5X02_DIRECTION_QTW				1
#define FT5X02_KX_LR_QTW				465
#define FT5X02_KY_UD_QTW				470
#define FT5X02_POINTS_SUPPORTED_QTW				5
#define FT5X02_THGROUP_QTW				100
#define FT5X02_THPEAK_QTW				60
#define FT5X02_THDIFF_QTW				2560
#define FT5X02_MAX_TOUCH_VALUE_QTW				1200
#define FT5X02_DRAW_LINE_TH_QTW				250
#define FT5X02_PWMODE_CTRL_QTW				1
#define FT5X02_PERIOD_ACTIVE_QTW				16
#define FT5X02_TIME_ENTER_MONITOR_QTW				10
#define FT5X02_PERIOD_MONITOR_QTW				40
#define FT5X02_FILTER_FRAME_NOISE_QTW				2
#define FT5X02_POWERNOISE_FILTER_TH_QTW			60
#define FT5X02_DIFFDATA_HADDLE_VALUE_QTW				100
#define FT5X02_FACE_DETECT_MODE_QTW				0
#define FT5X02_FACE_DETECT_STATISTICS_TX_NUM_QTW				3
#define FT5X02_FACE_DETECT_PRE_VALUE_QTW				20
#define FT5X02_FACE_DETECT_NUM_QTW				10
#define FT5X02_FACE_DETECT_LAST_TIME_QTW				1000
#define FT5X02_BIGAREA_PEAK_VALUE_MIN_QTW				255
#define FT5X02_BIGAREA_DIFF_VALUE_OVER_NUM_QTW				30
#define FT5X02_BIGAREA_POINT_AUTO_CLEAR_TIME_QTW				3000
#define FT5X02_ABNORMAL_DIFF_VALUE_QTW				60
#define FT5X02_ABNORMAL_DIFF_NUM_QTW				15
#define FT5X02_ABNORMAL_DIFF_LAST_FRAME_QTW				30
#define FT5X02_START_RX_QTW				0
#define FT5X02_ADC_TARGET_QTW				8500
#define FT5X02_ESD_FILTER_FRAME_QTW				2
#define FT5X02_MOVSTH_I_QTW				3
#define FT5X02_MOVSTH_N_QTW				2
#define FT5X02_MODE_QTW				1
#define FT5X02_PMODE_QTW				1
#define FT5X02_ERR_QTW				0
#define FT5X02_AUTO_CLB_MODE_QTW				255
#define FT5X02_STATE_QTW				1
#define FT5X02_HIGH_SPEED_TH_QTW				72
#define FT5X02_MID_SPEED_TH_QTW				240
#define FT5X02_STATIC_TH_QTW				136
#define FT5X02_THFALSE_TOUCH_PEAK_QTW				16


unsigned char g_ft5x02_tx_num_qtw = 15;
unsigned char g_ft5x02_rx_num_qtw = 10;
unsigned char g_ft5x02_gain_qtw = 15;
unsigned char g_ft5x02_voltage_qtw = 3; 
unsigned char g_ft5x02_scanselect_qtw = 7;
unsigned char g_ft5x02_tx_order_qtw[] = {14,13,12,11,10,9,8,7,6,5,4,3,2,1,0};
unsigned char g_ft5x02_tx_offset_qtw = 1;
unsigned char g_ft5x02_tx_cap_qtw[] = {30,30,30,30,30,30,30,30,30,30,30,30,30,30,29};
unsigned char g_ft5x02_rx_order_qtw[] = {0,1,2,3,4,5,6,7,8,9};
unsigned char g_ft5x02_rx_offset_qtw[] = {154,155,187,170,187};
unsigned char g_ft5x02_rx_cap_qtw[] = {54,55,55,56,55,55,55,55,55,54};

#endif