static int ft5x_test_write(unsigned char *buf, size_t len)
{
	int ret, i;

	printk(KERN_DEBUG "WRITING: ");
	for (i = 0; i < len; i++)
		printk(" 0x%02x", buf[i]);
	printk("\n");

	ret = ft5x_i2c_txdata(buf, len);
	if (ret < 0) {
		pr_err("write reg failed! %#x ret: %d", buf[0], ret);
		return -1;
	}

	return 0;
}

#if 0
static int ft5x_test_read(u8 addr)
{
	int ret;
	u8 cmd[2] = {0};

	cmd[0] = addr;
	printk("<0> READING\n");

	ret = ft5x_i2c_rxdata(cmd, sizeof(cmd));
	if (ret < 0) {
		pr_err("        read TS err %d\n", ret);
		return ret;
	}
	printk("<0> -- %02x %02x\n", cmd[0], cmd[1]);

	return cmd[0];
}
#endif

#define SEND_DATA(d...) \
{ \
	unsigned char c[] = {d}; \
	ft5x_test_write(c, sizeof(c) / sizeof(c[0])); \
} while (0);

#define READ_DATA(addr) ft5x_test_read(addr)
#define PB_CMD_DELAY 50

//static void dump_buf(const u8 *buf, int l);

static void read_regs(void *buf, size_t len)
{
	int r;
	//unsigned char *__buf = buf;

	pr_debug("[%s] Enter\n", __func__);

//	while (1) {

	memset(buf, '\0', len);
	r = ft5x_i2c_rxdata(buf, len);
	if (r < 0)
		pr_err("%s:%d error reading %d\n", __func__, __LINE__, r);



//		if (!(__buf[1] & (1 << 5)))
//			break;

#if 0
		if (__buf[1] & (1 << 5)) {
			pr_warn("[%s] BUFFER INVALID BIT IS SET, RE-READ DATA\n", __func__);
		} else {
			pr_info("[%s] Buffer valid, no need to re-read data\n", __func__);
			break;
		}
#endif
//	}
}

static int read_regs_addr(void *buf, size_t len, char read_addr)
{
	int ret;
	struct i2c_msg msgs[] = {
		{
			.addr	= this_client->addr,
			.flags	= 0,
			.len	= 1,
			.buf	= &read_addr,
		},
		{
			.addr	= this_client->addr,
			.flags	= I2C_M_RD,
			.len	= len,
			.buf	= buf,
		},
	};

	pr_debug("[%s] Reading regs, address %02x len %d\n", __func__, read_addr, len);
	memset(buf, '\0', len);
	ret = i2c_transfer(this_client->adapter, msgs, 2);
	if (ret < 0)
		pr_err("[%s] msg i2c read error: %d\n", __func__, ret);

	dump_buf(buf, len);

	return ret;


//		if (!(__buf[1] & (1 << 5)))
//			break;

#if 0
		if (__buf[1] & (1 << 5)) {
			pr_warn("[%s] BUFFER INVALID BIT IS SET, RE-READ DATA\n", __func__);
		} else {
			pr_info("[%s] Buffer valid, no need to re-read data\n", __func__);
			break;
		}
#endif
//	}
}

static void read_regs_dump(void *buf, size_t len)
{
	pr_debug("[%s] Enter\n", __func__);
	read_regs(buf, len);
	dump_buf(buf, len);
}

static u8 toggle_bit(u8 *cmd)
{
	int r;
	u8 buf[2] = {0};
	u8 hst_mode;

	pr_debug("[%s] Toggling handshake bit %d\n", __func__, cmd ? *cmd : 0);

	/* No cmd given, then read its value from device */
	if (cmd == NULL) {
		r = read_regs_addr(buf, 1, 0x0);
		if (r < 0)
			pr_err("[%s] Can read i2c data %d\n", __func__, r);
		hst_mode = buf[0];
		pr_debug("[%s] HST_MODE %02x\n", __func__, hst_mode);
	} else {
		hst_mode = *cmd;
	}

	hst_mode ^= CY_HNDSHK_BIT;
	SEND_DATA(0x00, hst_mode);

	if (cmd)
		*cmd = hst_mode;

	return hst_mode;
}

static void dump_mfg_stat(const u8 stat)
{
	pr_debug("[%s] Regs values set %s %s %s %s %s %s\n", __func__,
		       stat & (1 << 7) ? "PASS/FAIL" : "",
		       stat & (1 << 6) ? "INV_CMD" : "",
		       stat & (1 << 5) ? "BUFF_INV" : "",
		       stat & (1 << 2) ? "SCAN_ACTIVE" : "",
		       stat & (1 << 1) ? "CMD_COMPLT" : "",
		       stat & (1 << 0) ? "BUSY" : "");
}

static void clear_flags_cmd_bit(u8 *__hst_mode)
{
#if 0
	unsigned char buf[3];
	unsigned mfg_stat = 0;
	int repeat = 25;
	u8 hst_mode = 0;

	pr_debug("[%s] Enter\n", __func__);

	do_handshake_return(&hst_mode);
	msleep(PB_CMD_DELAY);
	/* sending clear flag command (0x2f) */
	SEND_DATA(0x00, hst_mode ^ CY_HNDSHK_BIT, 0xE3, 0x2F);

	//while ((mfg_stat != 0xE3) && repeat--) {
	while (((mfg_stat & 0xe3) != 0) && repeat--) {
		pr_debug("[%s] Looping %d\n", __func__, repeat);
		do_handshake();
		msleep(5);
		read_regs_addr(buf, ARRAY_SIZE(buf), 0x0);
		mfg_stat = buf[1]; //READ_DATA(REG_MFG_STAT); // offset 1
		pr_debug("[%s] hst_mode 0x%02x mfg_stat 0x%02x\n", __func__, buf[0], buf[1]);
		pr_debug("[%s] mfg_cmd 0x%02x\n", __func__, buf[2]);
	}
#else
	unsigned char buf[3];
	unsigned mfg_stat = 0;
	int repeat = 0;
	u8 hst_mode = 0;

	pr_debug("[%s] Enter\n", __func__);

	hst_mode = toggle_bit(NULL);
	msleep(PB_CMD_DELAY);
	hst_mode ^= CY_HNDSHK_BIT;
	SEND_DATA(0x00, hst_mode, 0xE3, 0x2F);

//	while (((mfg_stat & 0xe3) != 0) && repeat--) {
	while (((mfg_stat & 0xe3) != 0)) {
		pr_debug("[%s] Looping %d\n", __func__, repeat++);
		toggle_bit(&hst_mode);
		//do_handshake();
		msleep(5);
		read_regs_addr(buf, ARRAY_SIZE(buf), 0x0);
		mfg_stat = buf[1]; //READ_DATA(REG_MFG_STAT); // offset 1
		pr_debug("[%s] hst_mode 0x%02x mfg_stat 0x%02x\n", __func__, buf[0], buf[1]);
		pr_debug("[%s] mfg_cmd 0x%02x\n", __func__, buf[2]);
		dump_mfg_stat(mfg_stat);
	}

	if (__hst_mode)
		*__hst_mode = hst_mode;
#endif
}

static void clear_flags_cmd(void)
{
	clear_flags_cmd_bit(NULL);
}


#define PB_TEST_PASSED 0
#define PB_TEST_FAILED 1

struct ft5x_id {
	unsigned int id;
	unsigned int ver;
};

struct ft5x_id supported_ids[] = {
	{0x2054, 0x0206},
	{0x2054, 0x0207},
	{0x2054, 0x020a},
};

struct ft5x_id unsupported_ids[] = {
	{0xaa1d, 0xaaaa},
};

static int is_in_list(struct ft5x_id *id, struct ft5x_id *ids, size_t ids_len)
{
	int i;

	for (i = 0; i < ids_len; i++)
		if (id->id == ids[i].id && id->ver == ids[i].ver)
			return 1;

	return 0;
}

static int is_id_supported(unsigned int id, unsigned int ver)
{
	struct ft5x_id curr_id = {id, ver};
	return is_in_list(&curr_id, supported_ids, ARRAY_SIZE(supported_ids));
}

static int is_id_unsupported(unsigned int id, unsigned int ver)
{
	struct ft5x_id curr_id = {id, ver};
	return is_in_list(&curr_id, unsupported_ids, ARRAY_SIZE(unsupported_ids));
}

static int ft5x_do_firmware_tests(struct ft5x_ts_data *ft5x_ts)
{
	int r = PB_TEST_FAILED;
	struct cyttsp_sysinfo_data si;
	unsigned int app_id, app_ver;

	clear_flags_cmd();

	do_handshake();
	SEND_DATA(0x0, 0x90, 0x0, 0xF2);
	do_handshake();
	read_regs(&si, sizeof(si));
	do_handshake();
	msleep(5);

	read_regs(&si, sizeof(si));
	if (si.app_idh == 0  && si.app_idl == 0)
		pr_err("[%s] APP ID is zero\n", __func__);
	if (si.app_verh == 0  && si.app_verl == 0)
		pr_err("[%s] APP VER is zero\n", __func__);

	app_id = (si.app_idh << 8) | si.app_idl;
	app_ver = (si.app_verh << 8) | si.app_verl;

	pr_info("[%s] App ID 0x%04X APP Ver 0x%04X\n", __func__, app_id, app_ver);

	if (is_id_supported(app_id, app_ver)) {
		pr_info("[%s] ID and VER are supported\n", __func__);
		r = PB_TEST_PASSED;
	} else if(is_id_unsupported(app_id, app_ver)) {
		pr_err("[%s] ID and VER are __NOT__ supported\n", __func__);
	} else {
		pr_crit("[%s] ID and VER are unknown\n", __func__);
	}

	return r;
}

struct cyttsp_mfgtest_data {
	u8 hst_mode;
	u8 mfg_stat;
	u8 mfg_cmd;

	u8 reg_data[5];
	u8 reg_drive0[5];
	u8 reg_drive1[5];
};

struct mf_test_write {
	u8 offset;
	u8 mfg_cmd;

	u8 mfg_reg0_4[5];
	u8 mfg_dm0[5];
	u8 mfg_dm1[5];
	u8 mfg_data[5];
};

static void wait_for_cmd_complete_bit(u8 *__hst_mode)
{
	u8 buf[16];
	int repeat = 25;
	u8 hst_mode = 0;
	u8 mfg_stat = 0;

	if (__hst_mode)
		hst_mode = *__hst_mode;

	memset(buf, '\0', sizeof(buf));
//	while (! (mfg_stat & (1 << 1)) && repeat--) {
	repeat = 0;
	while (! (mfg_stat & (1 << 1))) {
		pr_debug("[%s] looping %02d\n", __func__, repeat++);
		// Step D
		toggle_bit(__hst_mode ? &hst_mode : NULL);
		// Step E
		msleep(100);
		// Step F
		read_regs_addr(buf, 4, 0x0);
		mfg_stat = buf[1];
		dump_mfg_stat(mfg_stat);
	}

	if (__hst_mode)
		*__hst_mode = hst_mode;
}

static void wait_for_cmd_complete(void)
{
#if 1
	u8 buf[16];
	int cnt = 10;

	memset(buf, '\0', sizeof(buf));
	while (! (buf[1] & (1 << 1)) && cnt--) {
		pr_debug("[%s] looping %02d\n", __func__, cnt);
		do_handshake();
		msleep(100);
		read_regs_addr(buf, 4, 0x0);
	}
#else
	wait_for_cmd_complete_bit(NULL);

#endif
}

#if 0
static void write_shorts(struct cyttsp_mfgtest_data *s)
{
	struct mf_test_write mf;
	//u8 mfg_cmd;

	//do_handshake();
	clear_flags_cmd();
	//do_handshake();

	mf.offset = 0x02;
	mf.mfg_cmd = 0x12;
	mf.mfg_reg0_4[0] = 0x00;
	mf.mfg_reg0_4[1] = 0x92;
	mf.mfg_reg0_4[2] = 0x01;
	mf.mfg_reg0_4[3] = 0x01;
	mf.mfg_reg0_4[4] = 0x05;

	memcpy(mf.mfg_dm0, s->reg_drive0, 5);
	memcpy(mf.mfg_dm1, s->reg_drive1, 5);
	memcpy(mf.mfg_data, s->reg_data, 5);

	do_handshake();
	ft5x_test_write(&mf, sizeof(struct mf_test_write));
	do_handshake();
	wait_for_cmd_complete();
	do_handshake();
}
#endif

#define WRITE_SHORTS_LEN 24

static void write_shorts_bit(struct cyttsp_mfgtest_data *s, u8 *hst_mode)
{
	//struct mf_test_write mf;
	unsigned char *buf;
	//u8 mfg_cmd;

	*hst_mode ^= CY_HNDSHK_BIT;

	buf = kmalloc(WRITE_SHORTS_LEN * sizeof(unsigned char), GFP_KERNEL);
	if (buf == NULL) {
		pr_err("[%s] Failed to allocate buffer to collect statistics\n", __func__);
		goto out;
	}


	buf[0] = 0x00;

	buf[1] = *hst_mode;
	buf[2] = 0x00;
	buf[3] = 0x12;

	buf[4] = 0x00;
	buf[5] = 0x92;
	buf[6] = 0x01;
	buf[7] = 0x01;
	buf[8] = 0x05;

	memcpy(&buf[ 9], s->reg_drive0, 5);
	memcpy(&buf[14], s->reg_drive1, 5);
	memcpy(&buf[19], s->reg_data, 5);

#if 0

	mf.offset = 0x02;
	mf.mfg_cmd = 0x12;
	mf.mfg_reg0_4[0] = 0x00;
	mf.mfg_reg0_4[1] = 0x92;
	mf.mfg_reg0_4[2] = 0x01;
	mf.mfg_reg0_4[3] = 0x01;
	mf.mfg_reg0_4[4] = 0x05;

	memcpy(mf.mfg_dm0, s->reg_drive0, 5);
	memcpy(mf.mfg_dm1, s->reg_drive1, 5);
	memcpy(mf.mfg_data, s->reg_data, 5);
#endif

	ft5x_test_write(buf, WRITE_SHORTS_LEN);
	toggle_bit(hst_mode);

out:
	kfree(buf);
}

struct cyttsp_mfgtest_data mfgtest_data;

static void dump_5regs(u8 *buf, char *reg_name)
{
	int i;

	printk(KERN_DEBUG "%s:", reg_name);
	for (i = 0; i < 5; i++)
		printk(" %02x", buf[i]);
	printk("\n");
}

#if 0
static void dump_5regs_and(u8 *buf, u8 *mask, char *reg_name)
{
	int i;

	printk("<0> %s:", reg_name);
	for (i = 0; i < 5; i++)
		printk(" %02x", buf[i] & mask[i]);
	printk("\n");
}

static void dump_5regs_or(u8 *buf, u8 *mask, char *reg_name)
{
	int i;

	printk("<0> %s:", reg_name);
	for (i = 0; i < 5; i++)
		printk(" %02x", buf[i] | mask[i]);
	printk("\n");
}
#endif

#if 0
#define START_TEST(d...) 				\
do { 							\
	unsigned char c[] = {d}; 			\
	do_handshake(); 				\
	clear_flags_cmd(); 				\
	do_handshake(); 				\
	ft5x_test_write(c, ARRAY_SIZE(c)); 		\
	wait_for_cmd_complete(); 			\
	do_handshake();					\
} while (0);
#else
#define START_TEST(d...) 				\
do { 							\
	unsigned char c[] = {d}; 			\
	clear_flags_cmd(); 				\
	ft5x_test_write(c, ARRAY_SIZE(c)); 		\
	wait_for_cmd_complete(); 			\
	do_handshake();					\
} while (0);
#endif

#define READ_BIG_DATA_2()				\
do {							\
	msleep(PB_CMD_DELAY);				\
	do_handshake();					\
	msleep(PB_CMD_DELAY);				\
	read_regs_dump(buf, 2);				\
	do_handshake();					\
	msleep(PB_CMD_DELAY);				\
	read_regs_dump(buf, 2);				\
	SEND_DATA(0x07);				\
	read_regs(buf, TOUCH_ROWS * TOUCH_COLS + 3);	\
	dump_regs_matrix(buf, 3);			\
} while (0);

#define READ_BIG_DATA()					\
do {							\
	msleep(PB_CMD_DELAY);				\
	do_handshake();					\
	msleep(PB_CMD_DELAY);				\
	read_regs_dump(buf, 2);				\
	SEND_DATA(0x07);				\
	read_regs(buf, TOUCH_ROWS * TOUCH_COLS + 3);	\
	dump_regs_matrix(buf, 3);			\
} while (0);

#define READ_BIG_DATA_BUF(__d)				\
do {							\
	msleep(PB_CMD_DELAY);				\
	do_handshake();					\
	msleep(PB_CMD_DELAY);				\
	read_regs_dump(&__d, 2);			\
	SEND_DATA(0x07);				\
	do_handshake();					\
	read_regs(&__d, TOUCH_ROWS * TOUCH_COLS + 3);	\
	dump_regs_matrix((unsigned char *)&__d, 3);	\
} while (0);

static void dump_shadow(struct cyttsp_mfgtest_data *sh, const char *step)
{
	pr_debug("[%s] Dumping shadow registers - %s\n", __func__, step);

	dump_5regs(sh->reg_data, "DATA reg");
	dump_5regs(sh->reg_drive0, " DM0 reg");
	dump_5regs(sh->reg_drive1, " DM1 reg");
}


/*
 * Key mask:
 * 0 - 11111011 - 0xFD
 * 1 - 01000100 - 0x44
 * 2 - 11111011 - 0xFD
 * 3 - 11111111 - 0xFF
 * 4 - 11111111 - 0xFF
 *
 */
const u8 mask_tt[] = {0xfd, 0x44, 0xfd, 0xff, 0xff};

/* pin-2-pin shorts mask */
const u8 iter_mask[][5] = {
	{0xfb, 0x44, 0xfb, 0x00, 0x00},
	{0xfb, 0x40, 0x00, 0xff, 0x00},
	{0xf0, 0x00, 0xf0, 0xf0, 0xf0},
	{0xcc, 0x44, 0xcc, 0xcc, 0xcc},
	{0xaa, 0x00, 0xaa, 0xaa, 0xaa},
};

#define DO_CHECK(in, out, par, cond) 		\
if (cond) {					\
	out->par[i] = in->par[i] | mask_tt[i];	\
} else {					\
	out->par[i] = in->par[i] & ~mask_tt[i];	\
}

static void prepare_test_mask(const struct cyttsp_mfgtest_data *shadow,
		struct cyttsp_mfgtest_data *send, int dm0, int dm1, int data,
		const u8 *iter)
{
	int i;

	for (i = 0; i < 5; i++) {
		DO_CHECK(shadow, send, reg_drive0, dm0);
		DO_CHECK(shadow, send, reg_drive1, dm1);
		DO_CHECK(shadow, send, reg_data, data);
	}

	for (i = 0; iter && i < 5; i++)
		send->reg_data[i] = iter[i];
//		send->reg_data[i] &= iter[i];
}

static void prepare_test(const struct cyttsp_mfgtest_data *shadow,
		struct cyttsp_mfgtest_data *send, int dm0, int dm1, int data)
{
	prepare_test_mask(shadow, send, dm0, dm1, data, NULL);
}

static int compare_data_regs(const struct cyttsp_mfgtest_data *rcv,
		const struct cyttsp_mfgtest_data *tst)
{
	int ret = PB_TEST_PASSED;
	int i;

	for (i = 0; i < 5; i++) {
		if (rcv->reg_data[i] != tst->reg_data[i]) {
			pr_err("[%s] data[%d] failed %02x != %02x\n", __func__, i,
					rcv->reg_data[i], tst->reg_data[i]);
			ret = PB_TEST_FAILED;
		}
	}

	return ret;
}

static void dump_reg_bin(const u8 *buf, const char *name)
{
	int i, j;
	char outb[8*5 + 4 + 1]; // 5 bytes plus 4 spaces and one zero in the end

	memset(outb, ' ', ARRAY_SIZE(outb));
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 5; j++)
			outb[7 - i + j*8 + j] = buf[j] & (1 << i) ? '1' : '0';
	}

	outb[ARRAY_SIZE(outb) - 1] = '\0';

	pr_warn("[%s] %s : %s\n", __func__, name, outb);
}

#if 0
static int do_handshake_return(u8 *ret_cmd)
{
	int r;
	u8 cmd[2] = {0};

	pr_debug("[%s] Enter\n", __func__);
	r = ft5x_i2c_rxdata(cmd, sizeof(cmd));
	dump_buf(cmd, 2);
	if (r < 0)
		pr_err("        read TS err %d\n", r);

	cmd[1] = cmd[0] ^ CY_HNDSHK_BIT;
	cmd[0] = 0;
	if (ret_cmd != NULL)
		*ret_cmd = cmd[1];
	dump_buf(cmd, 2);
	r = ft5x_i2c_txdata(cmd, sizeof(cmd));
	if (r < 0)
		pr_err("        wrote into TS result %d\n", r);

	return 0;
}
#endif


static int switch_mode(u8 mode)
{
	int r;
//	u8 val;
	u8 hst_mode = 0;

	pr_debug("[%s] Switching to mode %02x\n", __func__, mode);

	r = read_regs_addr(&hst_mode, 1, 0x0);
	if (r < 0)
		pr_err("[%s] Can read i2c data %d\n", __func__, r);

	hst_mode = ((hst_mode & CY_HNDSHK_BIT) + mode);
	hst_mode ^= CY_HNDSHK_BIT;
	SEND_DATA(0x00, hst_mode);

	msleep(50);

	toggle_bit(&hst_mode);

	msleep(50);

	return r;
}

static int ft5x_do_test_tests(struct ft5x_ts_data *ft5x_ts)
{
	unsigned char buf[3];
	unsigned mfg_stat = 0;
	int repeat = 10;
	u8 hst_mode = 0;

	pr_debug("[%s] Enter\n", __func__);

	hst_mode = toggle_bit(NULL);
	msleep(PB_CMD_DELAY);
	hst_mode ^= CY_HNDSHK_BIT;
	SEND_DATA(0x00, hst_mode, 0xE3, 0x2F);

	while (((mfg_stat & 0xe3) != 0) && repeat--) {
	//while ((mfg_stat != 0xE3) && repeat--) {
		pr_debug("[%s] Looping %d\n", __func__, repeat);
		toggle_bit(&hst_mode);
		//do_handshake();
		msleep(5);
		read_regs_addr(buf, ARRAY_SIZE(buf), 0x0);
		mfg_stat = buf[1]; //READ_DATA(REG_MFG_STAT); // offset 1
		pr_debug("[%s] hst_mode 0x%02x mfg_stat 0x%02x\n", __func__, buf[0], buf[1]);
		pr_debug("[%s] mfg_cmd 0x%02x\n", __func__, buf[2]);
	}

	return PB_TEST_FAILED;
}

static int ft5x_do_shorts_low_tests(struct ft5x_ts_data *ft5x_ts)
{
	struct cyttsp_mfgtest_data shadow;
	struct cyttsp_mfgtest_data testdata;
	int i, errors;
	u8 hst_mode;

	pr_info("[%s] Test started\n", __func__);

//	dump_reg_bin(mask_tt, "MASK : ");

	clear_flags_cmd_bit(&hst_mode);

	hst_mode ^= CY_HNDSHK_BIT;
	SEND_DATA(0x0, hst_mode, 0x0, 0x04);
	toggle_bit(&hst_mode);
	wait_for_cmd_complete_bit(&hst_mode);

	read_regs_dump(&shadow, sizeof(struct cyttsp_mfgtest_data));
	dump_shadow(&shadow, "Step 2");

	prepare_test(&shadow, &testdata, 0, 0, 1);
	dump_shadow(&testdata, "Step 2 data for shorts low test (DM0-0, DM1-0, DATA-1)");

	clear_flags_cmd_bit(&hst_mode);

	write_shorts_bit(&testdata, &hst_mode);
	toggle_bit(&hst_mode);

	wait_for_cmd_complete_bit(&hst_mode);

	read_regs_dump(&shadow, sizeof(struct cyttsp_mfgtest_data));
	dump_shadow(&shadow, "Step 2 (value after test)");

	dump_reg_bin(testdata.reg_data, "WRIT : ");
	dump_reg_bin(shadow.reg_data, "READ : ");
	dump_reg_bin(mask_tt, "MASK : ");

	errors = 0;
	for (i = 0; i < 5; i++) {
		if ((shadow.reg_data[i] & mask_tt[i]) != mask_tt[i]) {
			pr_err("[%s] data[%d] Short low detected %02x != %02x\n", __func__,
					i, shadow.reg_data[i], mask_tt[i]);
			errors++;
		}
	}
	if (errors)
		pr_err("[%s] Errors detected %d\n", __func__, errors);
	else
		pr_info("[%s] Shorts low test PASSED\n", __func__);

	return errors ? PB_TEST_FAILED : PB_TEST_PASSED;
}

static int ft5x_do_shorts_high_tests(struct ft5x_ts_data *ft5x_ts)
{
	struct cyttsp_mfgtest_data shadow;
	struct cyttsp_mfgtest_data testdata;
	int i, errors;
	u8 hst_mode;

	pr_info("[%s] Test started\n", __func__);

//	dump_reg_bin(mask_tt, "MASK : ");

	clear_flags_cmd_bit(&hst_mode);

	hst_mode ^= CY_HNDSHK_BIT;
	SEND_DATA(0x0, hst_mode, 0x0, 0x04);
	toggle_bit(&hst_mode);
	wait_for_cmd_complete_bit(&hst_mode);

	read_regs_dump(&shadow, sizeof(struct cyttsp_mfgtest_data));
	dump_shadow(&shadow, "Step 2");

	prepare_test(&shadow, &testdata, 0, 0, 0);
	dump_shadow(&testdata, "Step 2 data for shorts low test (DM0-0, DM1-0, DATA-1)");

	clear_flags_cmd_bit(&hst_mode);

	write_shorts_bit(&testdata, &hst_mode);
	toggle_bit(&hst_mode);

	wait_for_cmd_complete_bit(&hst_mode);

	read_regs_dump(&shadow, sizeof(struct cyttsp_mfgtest_data));
	dump_shadow(&shadow, "Step 2 (value after test)");

	dump_reg_bin(testdata.reg_data, "WRIT : ");
	dump_reg_bin(shadow.reg_data, "READ : ");
	dump_reg_bin(mask_tt, "MASK : ");

	errors = 0;
	for (i = 0; i < 5; i++) {
		if ((~shadow.reg_data[i] & mask_tt[i]) != mask_tt[i]) {
			pr_err("[%s] data[%d] Short high detected %02x != %02x\n", __func__,
					i, (unsigned char)~shadow.reg_data[i], mask_tt[i]);
			errors++;
		}
	}
	if (errors) {
		pr_err("[%s] Errors detected %d\n", __func__, errors);
	} else {
		pr_info("[%s] Shorts high test PASSED\n", __func__);
	}

	return errors ? PB_TEST_FAILED : PB_TEST_PASSED;
}

static int ft5x_do_shorts_p2p_tests(struct ft5x_ts_data *ft5x_ts)
{
	struct cyttsp_mfgtest_data shadow;
	struct cyttsp_mfgtest_data testdata;
	int i, errors;
	u8 hst_mode = 0;

	errors = 0;
	for (i = 0; i < 5; i++) {

		pr_info("\n[%s] Iteration %d\n\n", __func__, i);

		// Step A

		// Step B
		clear_flags_cmd_bit(&hst_mode);

		// Step C
		hst_mode ^= CY_HNDSHK_BIT;
		SEND_DATA(0x0, hst_mode, 0x0, 0x04);
		//// Step D
		//// toggle_bit(&hst_mode);
		// Step G - {D, E, F}
		wait_for_cmd_complete_bit(&hst_mode);

		// Step H
		read_regs_dump(&shadow, sizeof(struct cyttsp_mfgtest_data));
		dump_shadow(&shadow, "Step 2");


		prepare_test_mask(&shadow, &testdata, 0, 0, 1, iter_mask[i]);
		dump_shadow(&testdata, "Test data that is written to device");

		// Step I
		clear_flags_cmd_bit(&hst_mode);

		// Step J
		write_shorts_bit(&testdata, &hst_mode);

		// Step N - {K, L, M}
		wait_for_cmd_complete_bit(&hst_mode);

		// Step O
		read_regs_dump(&shadow, sizeof(struct cyttsp_mfgtest_data));
		dump_shadow(&shadow, "Step 2 (value after test)");

		// Step P
		if (compare_data_regs(&shadow, &testdata) == PB_TEST_FAILED)
			errors++;

		dump_reg_bin(testdata.reg_data, "WRIT : ");
		dump_reg_bin(shadow.reg_data, "READ : ");
		dump_reg_bin(mask_tt, "MASK : ");

	}

	if (errors)
		pr_err("[%s] Errors detected %d\n", __func__, errors);
	else
		pr_info("[%s] Shorts high test PASSED\n", __func__);


	return errors ? PB_TEST_FAILED : PB_TEST_PASSED;
}

#define TOUCH_ROWS 18
#define TOUCH_COLS 14

static void dump_regs_matrix(unsigned char *buf, size_t offset)
{
	size_t i, j;

	for (i = 0; i < TOUCH_COLS; i++) {
		printk("buf[%2d]: ", i);
		for (j = 0; j < TOUCH_ROWS; j++) {
			printk(" %02x", buf[i * TOUCH_ROWS + j + offset]);
		}
		printk("\n");
	}
	printk("\n");
}

static void dump_regs_matrix_int(int *buf)
{
	size_t i, j;

	for (i = 0; i < TOUCH_COLS; i++) {
		printk("buf[%2d]: ", i);
		for (j = 0; j < TOUCH_ROWS; j++) {
			printk(" %i", buf[i * TOUCH_ROWS + j]);
		}
		printk("\n");
	}
	printk("\n");
}

static int ft5x_do_open_tests(struct ft5x_ts_data *ft5x_ts)
{
	int r, i, ret = PB_TEST_PASSED;
	u8 buf[512];
	const char threshold = 12;

	memset(buf, '\0', sizeof(buf));

	START_TEST(0x00, 0x10, 0x00, 0x20, 0xC3, 0x00);

	r = switch_to_mode(CY_TEST_MODE_IDAC);
	if (r < 0) {
		pr_err("%s:%d error switching into sysinfo, response %d, exiting\n", __func__, __LINE__, r);
		return PB_TEST_FAILED;
	}

	READ_BIG_DATA_2();

	for (i = 3; i < 18 * 14 + 3; i++) {
		if (buf[i] < threshold) {
			pr_err("value for buf[%d] less then threshold %d\n", i, buf[i]);
			ret = PB_TEST_FAILED;
		}
	}

	return ret;
}

/* TODO */
static int ft5x_do_idac_tests(struct ft5x_ts_data *ft5x_ts)
{
	int r, ret = PB_TEST_FAILED;
	u8 buf[512];

	memset(buf, '\0', sizeof(buf));

	START_TEST(0x00, 0x90, 0x00, 0x20, 0x00, 0x00);

	r = switch_to_mode(CY_TEST_MODE_IDAC);
	if (r < 0) {
		pr_err("%s:%d error switching into sysinfo, response %d, exiting\n", __func__, __LINE__, r);
		return -EFAULT;
	}

	read_regs_addr(buf, 22, 0x7);
	printk("GLOBAL IDAC VALUED\n");
	dump_buf(buf, 22);

	SEND_DATA(0x0);
	do_handshake();

	read_regs_addr(buf, 22, 0x7);
	printk("LOCAL IDAC values\n");
	dump_buf(buf, 22);

	pr_info("[%s] Reading global IDAC and gain values\n", __func__);
	READ_BIG_DATA();
	pr_info("[%s] Reading local IDAC value\n", __func__);
	READ_BIG_DATA();

	msleep(PB_CMD_DELAY);

	return ret;
}

/* TODO */

struct ft5x_baseline {
	u8 tt_mode;
	u8 mfg_stat;
	u8 mfg_cmd;
	u8 data[18 * 14];
};

/* Number of iteration that should be accomplished to collect statistics data */
#define NUM_OF_STATS 10

#define MATRIX_SIZE (TOUCH_ROWS * TOUCH_COLS)

/* Do not touch device while this test is running */
static int ft5x_do_noise_tests(struct ft5x_ts_data *ft5x_ts)
{
	int r, ret = PB_TEST_FAILED;
	u8 buf[512];
	struct ft5x_baseline *bs;
//	int bs_mean[MATRIX_SIZE], bs_final[MATRIX_SIZE];
	int *bs_mean, *bs_final;
	size_t i, j, errors;
	int noise_thr = 6; /* Let is be 3 */

	bs = kmalloc(NUM_OF_STATS * sizeof(struct ft5x_baseline), GFP_KERNEL);
	if (bs == NULL) {
		pr_err("[%s] Failed to allocate buffer to collect statistics\n", __func__);
		ret = -ENOMEM;
		goto out;
	}

	bs_mean = kmalloc(MATRIX_SIZE * sizeof(int), GFP_KERNEL);
	if (bs_mean == NULL) {
		pr_err("[%s] Failed to allocate bs_mean buffer\n", __func__);
		ret = -ENOMEM;
		goto out;
	}

	bs_final = kmalloc(MATRIX_SIZE * sizeof(int), GFP_KERNEL);
	if (bs_final == NULL) {
		pr_err("[%s] Failed to allocate bs_final buffer\n", __func__);
		ret = -ENOMEM;
		goto out;
	}

	memset(bs, '\0', NUM_OF_STATS * sizeof(struct ft5x_baseline));
	memset(bs_final, '\0', sizeof(int) * MATRIX_SIZE);
	memset(bs_mean, '\0', sizeof(int) * MATRIX_SIZE);
	memset(buf, '\0', sizeof(buf));

	START_TEST(0x00, 0x90, 0x00, 0x21);

	r = switch_to_mode(CY_TEST_MODE_BASELINES);
	if (r < 0) {
		pr_err("%s:%d error switching into sysinfo, response %d, exiting\n", __func__, __LINE__, r);
		ret = PB_TEST_FAILED;
		goto out;
	}

	pr_info("[%s] Read raw data (discard stale data)\n", __func__);
	READ_BIG_DATA();

	// Read register values for 10 times and calculate standart deviation

	for (i = 0; i < NUM_OF_STATS; i++) {
		READ_BIG_DATA_BUF(bs[i]);
		msleep(100);
	}

	/* At first, mean value should be calculated */
	for (i = 0; i < NUM_OF_STATS; i++) {
		for (j = 0; j < MATRIX_SIZE; j++) {
			bs_mean[j] += bs[i].data[j];
		}
	}
	for (j = 0; j < MATRIX_SIZE; j++) {
		bs_mean[j] /= NUM_OF_STATS;
	}

	/* Now evaluate stddev value */
	for (j = 0; j < MATRIX_SIZE; j++) {
		for (i = 0; i < NUM_OF_STATS; i++) {
			bs_final[j] += abs(bs_mean[j] - bs[i].data[j]);
		}
	}
	for (j = 0; j < MATRIX_SIZE; j++) {
		bs_final[j] /= NUM_OF_STATS;
	}

	/* Check for errors */
	errors = 0;
	for (i = 0; i < MATRIX_SIZE; i++) {
		/* TODO: Maybe threshold value should be reconsidered */
		if (bs_final[i] > noise_thr) {
			pr_err("[%s] Found values that exceeds threshold buf[%d]: %d > %d\n",
					__func__, i, bs_final[i], noise_thr);
			errors++;
		}
	}

	if (errors) {
		ret = PB_TEST_FAILED;
		dump_regs_matrix_int(bs_final);
	} else {
		ret = PB_TEST_PASSED;
	}

	msleep(PB_CMD_DELAY);

out:
	kfree(bs);
	return ret;
}

/* TODO */
static int ft5x_do_signal_tests(struct ft5x_ts_data *ft5x_ts)
{
	int r, ret = PB_TEST_FAILED;
	u8 buf[512];

	memset(buf, '\0', sizeof(buf));

	START_TEST(0x00, 0x90, 0x00, 0x21);

	r = switch_to_mode(CY_TEST_MODE_BASELINES);
	if (r < 0) {
		pr_err("%s:%d error switching into sysinfo, response %d, exiting\n", __func__, __LINE__, r);
		return -EFAULT;
	}

	pr_info("[%s] Read difference counts (discard stale data)\n", __func__);
	READ_BIG_DATA();
	pr_info("[%s] Read difference counts\n", __func__);
	READ_BIG_DATA();

	msleep(PB_CMD_DELAY);

	return ret;
}
