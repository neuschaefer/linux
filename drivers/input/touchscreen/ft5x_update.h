#include "cyttsp_fw.h"

enum cyttsp_ic_grpnum {
	CY_IC_GRPNUM_RESERVED = 0,
	CY_IC_GRPNUM_OP_TAG,	/* Platform Data Operational tagged registers */
	CY_IC_GRPNUM_SI_TAG,	/* Platform Data Sysinfo tagged registers */
	CY_IC_GRPNUM_BL_KEY,	/* Platform Data Bootloader Keys */
	CY_IC_GRPNUM_OP_REG,	/* general Operational registers read/write */
	CY_IC_GRPNUM_SI_REG,	/* general Sysinfo registers read/write */
	CY_IC_GRPNUM_BL_REG,	/* general Bootloader registers read access */
	CY_IC_GRPNUM_NUM	/* always last */
};

#define CY_I2C_NAME FT5X_NAME

#define GET_NUM_TOUCHES(x)          ((x) & 0x0F)
#define IS_LARGE_AREA(x)            (((x) & 0x10) >> 4)
#define IS_BAD_PKT(x)               ((x) & 0x20)
#define IS_VALID_APP(x)             ((x) & 0x01)
#define IS_OPERATIONAL_ERR(x)       ((x) & 0x3F)
#define GET_HSTMODE(reg)            ((reg & 0x70) >> 4)
#define IS_BOOTLOADERMODE(reg)      ((reg & 0x10) >> 4)
#define BL_WATCHDOG_DETECT(reg)     (reg & 0x02)

#define CY_NTCH                     0 /* lift off */
#define CY_TCH                      1 /* touch down */
#define CY_SMALL_TOOL_WIDTH         10
#define CY_REG_BASE                 0x00
#define CY_REG_OP_START             0x1B
#define CY_REG_OP_END               0x1E
#define CY_REG_OP_FEATURE           0x1F
#define CY_REG_SI_START             0x17
#define CY_REG_SI_END               0x1F
#define CY_DELAY_DFLT               20 /* ms */
#define CY_DELAY_MAX                (500/CY_DELAY_DFLT) /* half second */
#define CY_HALF_SEC_TMO_MS          500
#define CY_TEN_SEC_TMO_MS           10000
#define CY_HNDSHK_BIT               0x80
#define CY_HST_MODE_CHANGE_BIT      0x08
#define CY_CA_BIT                   0x01 /* Charger Armor(tm) bit */
#define CY_WAKE_DFLT                99 /* causes wake strobe on INT line
					* in sample board configuration
					* platform_data->hw_recov() function
					*/
/* device mode bits */
#define CY_OPERATE_MODE             0x00
#define CY_SYSINFO_MODE             0x10
/* power mode select bits */
#define CY_SOFT_RESET_MODE          0x01 /* return to Bootloader mode */
#define CY_DEEP_SLEEP_MODE          0x02
/* abs settings */
/* abs value offsets */
#define CY_NUM_ABS_VAL              5 /* number of abs values per setting */
#define CY_SIGNAL_OST               0
#define CY_MIN_OST                  1
#define CY_MAX_OST                  2
#define CY_FUZZ_OST                 3
#define CY_FLAT_OST                 4
/* axis signal offsets */
#define CY_NUM_ABS_SET              5 /* number of abs signal sets */
#define CY_ABS_X_OST                0
#define CY_ABS_Y_OST                1
#define CY_ABS_P_OST                2
#define CY_ABS_W_OST                3
#define CY_ABS_ID_OST               4
#define CY_IGNORE_VALUE             0xFFFF /* mark unused signals as ignore */

#define HI_TRACKID(reg)        ((reg & 0xF0) >> 4)
#define LO_TRACKID(reg)        ((reg & 0x0F) >> 0)

#define CY_BL_NUM_KEYS		8
#define CY_BL_PAGE_SIZE		16
#define CY_BL_NUM_PAGES		5
#define CY_BL_BUSY		0x80
#define CY_BL_READY_NO_APP	0x10
#define CY_BL_READY_APP		0x11
#define CY_BL_RUNNING		0x20
#define CY_BL_NOERR		0x00
#define CY_BL_ENTER_CMD_SIZE	11
#define CY_BL_EXIT_CMD_SIZE	11
#define CY_BL_WR_BLK_CMD_SIZE	79
#define CY_BL_VERS_SIZE		9
#define CY_BL_FW_NAME_SIZE	NAME_MAX
#define CY_MAX_PRBUF_SIZE	PIPE_BUF
#define CY_IRQ_DEASSERT		1
#define CY_IRQ_ASSERT		0

struct cyttsp_vers {
	u8 tts_verh;
	u8 tts_verl;
	u8 app_idh;
	u8 app_idl;
	u8 app_verh;
	u8 app_verl;
	u8 cid[3];
};

struct touch_firmware {
	const uint8_t   *img;
	uint32_t        size;
	const uint8_t   *ver;
	uint8_t         vsize;
} __attribute__ ((packed));

static struct touch_firmware cyttsp3_i2c_firmware = {
	.img = cyttsp3_img,
	.size = sizeof(cyttsp3_img),
	.ver = cyttsp3_ver,
	.vsize = sizeof(cyttsp3_ver),
};

struct touch_settings {
	const uint8_t   *data;
	uint8_t         size;
	uint8_t         tag;
} __attribute__ ((packed));

static const uint8_t cyttsp_op_regs[] = {0, 0, 0, 0x08};
static const uint8_t cyttsp_si_regs[] = {0, 0, 0, 0, 0, 0, 0x00, 0xFF, 0x0A};
static const uint8_t cyttsp_bl_keys[] = {0, 1, 2, 3, 4, 5, 6, 7};
static const char cyttsp_use_name[] = CY_I2C_NAME;

static struct touch_settings cyttsp_sett_op_regs = {
	.data = (uint8_t *)&cyttsp_op_regs[0],
	.size = sizeof(cyttsp_op_regs),
//	.tag = 1,
	.tag = 3,
};

static struct touch_settings cyttsp_sett_si_regs = {
	.data = (uint8_t *)&cyttsp_si_regs[0],
	.size = sizeof(cyttsp_si_regs),
//	.tag = 3,
	.tag = 6,
};

static struct touch_settings cyttsp_sett_bl_keys = {
	.data = (uint8_t *)&cyttsp_bl_keys[0],
	.size = sizeof(cyttsp_bl_keys),
	.tag = 0,
};

struct touch_settings *cyttsp3_sett[4] ={
	NULL,
	&cyttsp_sett_op_regs,
	&cyttsp_sett_si_regs,
	&cyttsp_sett_bl_keys,
};

#define CY_BL_CHKSUM_OK 0x01
struct cyttsp_bootloader_data {
	u8 bl_file;
	u8 bl_status;
	u8 bl_error;
	u8 blver_hi;
	u8 blver_lo;
	u8 bld_blver_hi;
	u8 bld_blver_lo;
	u8 ttspver_hi;
	u8 ttspver_lo;
	u8 appid_hi;
	u8 appid_lo;
	u8 appver_hi;
	u8 appver_lo;
	u8 cid_0;
	u8 cid_1;
	u8 cid_2;
};


