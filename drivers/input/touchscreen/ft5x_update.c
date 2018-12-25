static irqreturn_t cyttsp_irq(int irq, void *handle)
{
	struct ft5x_ts_data *ts = (struct ft5x_ts_data *) handle;

	pr_debug("[%s]  -- == INTERRUPT == --\n", __func__);

	if (ctp_ops.judge_int_occur()) {
		pr_debug("[%s]  ... other interrupt\n", __func__);
		return IRQ_NONE;
	}

	pr_debug("==IRQ_EINT%d=\n", CTP_IRQ_NO);
	ctp_ops.clear_penirq();

	complete(&ts->do_irq);

	return IRQ_HANDLED;
}

static void __dump_buf(const char *name, const char *func, const u8 *data, size_t len)
{
	char buf[1024];
	unsigned buf_len = sizeof(buf);
	char *p = buf;
	int i;
	int l;

	l = snprintf(p, buf_len, "dumping %s:", name);
	buf_len -= l;
	p += l;
	for (i = 0; i < len && buf_len; i++, p += l, buf_len -= l) {
		l = snprintf(p, buf_len, " %02x", *((char *)data + i));
		if ((i % 16) == 0)
			l = snprintf(p, buf_len, "\n");
	}

	pr_debug("[%s] %s\n", func, buf);
}

static int get_data(struct i2c_client *client, u8 addr, u8 *data, u16 len)
{
	int r;
	struct i2c_msg msgs[] = {
		{
			.addr	= client->addr,
			.flags	= 0,
			.len	= 1,
			.buf	= &addr,
		},
		{
			.addr	= client->addr,
			.flags	= I2C_M_RD,
			.len	= len,
			.buf	= data,
		},
	};

	pr_debug("[%s] Reading regs, address %02x len %d bytes\n", __func__, addr, len);
	memset(data, '\0', len);

	r = i2c_transfer(client->adapter, msgs, ARRAY_SIZE(msgs));
	if (r < 0)
		pr_err("[%s] msg i2c read error: %d\n", __func__, r);

	__dump_buf("received data", __func__, data, len);

	return r;
}

#define FT5X_TRIES		50
static int send_data_addr(struct i2c_client *client, u8 addr, u8 *data, u16 len)
{
	int r, tries;
	u8 *buf = NULL;

	/* data + 1 byte for address */
	buf = kmalloc(len + 1, GFP_KERNEL);
	if (buf == NULL) {
		pr_err("[%s] Failed to malloc memory\n", __func__);
		return -ENOMEM;
	}

	struct i2c_msg msg[] = {
		{
			.addr	= client->addr,
			.flags	= 0,
			.len	= len + 1,
			.buf	= buf,
		},
	};

	buf[0] = addr;
	memcpy(&buf[1], data, len);

	pr_debug("[%s] Writing %d bytes at addr %d\n", __func__, len, addr);
	__dump_buf("send data", __func__, buf, len + 1);

	for (tries = 0, r = -1; tries < FT5X_TRIES && (r < 0); tries++) {
		r = i2c_transfer(client->adapter, msg, ARRAY_SIZE(msg));
		if (r < 0)
			pr_err("[%s] i2c write error: %d\n", __func__, r);
	}

	return r > 0 ? 0 : r;
}

static void _cyttsp_load_bl_regs(struct ft5x_ts_data *ts)
{
	int retval = 0;

	pr_debug("[%s] Enter\n", __func__);

	memset(&(ts->bl_data), '\0', sizeof(struct cyttsp_bootloader_data));

	retval = get_data(ts->client, CY_REG_BASE, &(ts->bl_data), sizeof(struct cyttsp_bootloader_data));
	if (retval < 0) {
		pr_err("[%s]: bus fail reading Bootloader regs r=%d\n", __func__, retval);
		/* Calling process determines state change requirement */
		goto cyttsp_load_bl_regs_exit;
	}

	if (IS_BOOTLOADERMODE(ts->bl_data.bl_status)) {
		pr_debug("[%s]: Bootloader Regs:\n"
			 "  file=%02X status=%02X error=%02X\n"
			 "  BL Version:          0x%02X%02X\n"
			 "  Build BL Version:    0x%02X%02X\n"
			 "  TTSP Version:        0x%02X%02X\n"
			 "  Application ID:      0x%02X%02X\n"
			 "  Application Version: 0x%02X%02X\n"
			 "  Custom ID:           0x%02X%02X%02X\n",
			__func__,
			ts->bl_data.bl_file, ts->bl_data.bl_status,
			ts->bl_data.bl_error,
			ts->bl_data.blver_hi, ts->bl_data.blver_lo,
			ts->bl_data.bld_blver_hi, ts->bl_data.bld_blver_lo,
			ts->bl_data.ttspver_hi, ts->bl_data.ttspver_lo,
			ts->bl_data.appid_hi, ts->bl_data.appid_lo,
			ts->bl_data.appver_hi, ts->bl_data.appver_lo,
			ts->bl_data.cid_0, ts->bl_data.cid_1,
			ts->bl_data.cid_2);
	} else {
		pr_debug("%s: Not Bootloader mode:\n"
			"  mode=%02X status=%02X error=%02X\n",
			__func__,
			ts->bl_data.bl_file, ts->bl_data.bl_status,
			ts->bl_data.bl_error);
	}

cyttsp_load_bl_regs_exit:
	return;

}

static int _cyttsp_wait_ready(struct ft5x_ts_data *ts, struct completion *complete,
	u8 *cmd, size_t cmd_size, unsigned long timeout_ms)
{
	unsigned long timeout = 0;
	unsigned long uretval = 0;
	int retval = 0;

	pr_debug("[%s] Enter. timeout %lu ms\n", __func__, timeout_ms);

	timeout = msecs_to_jiffies(timeout_ms);
	INIT_COMPLETION(*complete);
	if ((cmd != NULL) && (cmd_size != 0)) {
		retval = get_data(ts->client, CY_REG_BASE, cmd, cmd_size);
		if (retval < 0) {
			pr_err("[%s] bus write fail switch mode r=%d\n", __func__, retval);
			goto _cyttsp_wait_ready_exit;
		}
	}
	pr_info("[%s] Wairing for completeion\n", __func__);
	uretval = wait_for_completion_interruptible_timeout(complete, timeout);
	msleep(10);
	if (uretval == 0) {
		pr_err("[%s]: Switch Mode Timeout waiting for ready interrupt - try reading regs\n", __func__);
		/* continue anyway */
		retval = 0;
	}

	pr_debug("[%s] Switch Mode: ret=%d uretval=%lu timeout=%lu", __func__, retval, uretval, timeout);

_cyttsp_wait_ready_exit:
	return retval;
}

static int _cyttsp_wait_bl_ready(struct ft5x_ts_data *ts, bool (*cond)(struct ft5x_ts_data *), int timeout_ms)
{
	unsigned long timeout = 0;
	unsigned long uretval = 0;
	int retval = 0;

	pr_debug("[%s] Enter, timeout %d\n", __func__, timeout_ms);

	/* wait for interrupt to set ready completion */
	timeout = msecs_to_jiffies(timeout_ms);
	uretval = wait_for_completion_interruptible_timeout(&ts->do_irq, timeout);
	msleep(10);
	pr_debug("[%s] COmpletion %lu\n", __func__, uretval);

	if (uretval == 0) {
		/* do not return error; continue in case of missed interrupt */
		pr_err("[%s] Loader timeout waiting for loader ready\n", __func__);
	}

	/* if cond == NULL then no status check is required */
	if (cond != NULL) {
		pr_debug("[%s] Loader Start: ret=%d uretval=%lu timeout=%lu\n", __func__, retval, uretval, timeout);

		_cyttsp_load_bl_regs(ts);
		if (cond(ts) == false) {
			if (uretval == 0) {
				pr_err("[%s] BL loader timeout err ret=%d uretval=%lu timeout=%lu\n",
				__func__, retval, uretval, timeout);
				retval = -ETIME;
			} else {
				pr_err("[%s] BL loader status err r=%d bl_f=%02X bl_s=%02X bl_e=%02X\n",
					__func__, retval,
					ts->bl_data.bl_file,
					ts->bl_data.bl_status,
					ts->bl_data.bl_error);
				retval = -EIO;
			}
		}
	}

	return retval;
}

static bool _cyttsp_chk_bl_startup(struct ft5x_ts_data *ts)
{
	return !(ts->bl_data.bl_status & CY_BL_BUSY) &&
		(ts->bl_data.bl_error == CY_BL_RUNNING);
}

static bool _cyttsp_chk_bl_terminate(struct ft5x_ts_data *ts)
{
	return (ts->bl_data.bl_status == CY_BL_READY_APP) &&
	       (ts->bl_data.bl_error == CY_BL_NOERR);
}

static int _cyttsp_wr_blk_chunks(struct ft5x_ts_data *ts, u8 addr, u8 len, const u8 *values)
{
	int retval = 0;
	int block = 1;
	int length = len;
	u8 dataray[CY_BL_PAGE_SIZE + 1];

	pr_debug("[%s] Write address %02x len %d\n", __func__, addr, len);

	memset(dataray, 0, sizeof(dataray));

	/* first page already includes the bl page offset */
	memcpy(dataray, values, sizeof(dataray));
	INIT_COMPLETION(ts->do_irq);
	retval = send_data_addr(ts->client, addr, dataray, sizeof(dataray));
	if (retval < 0) {
		pr_err("%s: Write chunk err block=0 r=%d\n", __func__, retval);
		goto _cyttsp_wr_blk_chunks_exit;
	}

	values += CY_BL_PAGE_SIZE + 1;
	length -= CY_BL_PAGE_SIZE + 1;

	/* remaining pages require bl page offset stuffing */
	for (block = 1; block < CY_BL_NUM_PAGES; block++) {
		if (length < 0) {
			pr_err("%s: Write chunk length err block=%d r=%d\n", __func__, block, retval);
			goto _cyttsp_wr_blk_chunks_exit;
		}
		dataray[0] = CY_BL_PAGE_SIZE * block;

		retval = _cyttsp_wait_bl_ready(ts, NULL, 100);
		if (retval < 0) {
			pr_err("%s: wait ready timeout block=%d length=%d\n", __func__, block, length);
			goto _cyttsp_wr_blk_chunks_exit;
		}

		memcpy(&dataray[1], values, length >= CY_BL_PAGE_SIZE ? CY_BL_PAGE_SIZE : length);
		INIT_COMPLETION(ts->do_irq);
		retval = send_data_addr(ts->client, addr, dataray,
				length >= CY_BL_PAGE_SIZE ? sizeof(dataray) : length + 1);
		if (retval < 0) {
			pr_err("%s: Write chunk err block=%d r=%d\n", __func__, block, retval);
			goto _cyttsp_wr_blk_chunks_exit;
		}

		values += CY_BL_PAGE_SIZE;
		length -= CY_BL_PAGE_SIZE;
	}

	retval = _cyttsp_wait_bl_ready(ts, NULL, 200);
	if (retval < 0) {
		pr_err("%s: last wait ready timeout block=%d length=%d\n", __func__, block, length);
		goto _cyttsp_wr_blk_chunks_exit;
	}

	_cyttsp_load_bl_regs(ts);
#if 0
	if (retval < 0) {
		pr_err("%s: Load BL regs err r=%d\n", __func__, retval);
		goto _cyttsp_wr_blk_chunks_exit;
	}
#endif
	if (!(((ts->bl_data.bl_status == CY_BL_READY_NO_APP) &&
	       (ts->bl_data.bl_error  == CY_BL_RUNNING)) ||
	       (ts->bl_data.bl_status == CY_BL_READY_APP))) {
		pr_err("%s: BL status fail bl_stat=0x%02X, bl_err=0x%02X\n",
			__func__, ts->bl_data.bl_status, ts->bl_data.bl_error);
		retval = -ETIMEDOUT;
	}

_cyttsp_wr_blk_chunks_exit:
	return retval;
}

static int cyttsp_load_app(struct ft5x_ts_data *ts, const u8 *fw, int fw_size)
{
	int retval = 0;
	int loc = 0;

	pr_info("[%s] enter\n", __func__);

	/* sync up with the next bootloader heartbeat interrupt in order
	 * to remove haearbeat vs. repsonse interrupt ambiguity
	 */
	retval = _cyttsp_wait_ready(ts, &ts->do_irq, NULL, 0, CY_HALF_SEC_TMO_MS);
	if (retval < 0) {
		pr_err("[%s] fail wait ready r=%d\n", __func__, retval);
		goto _cyttsp_load_app_exit;
	}

	/* send bootload initiation command */
	pr_info("[%s] Send BL Loader Enter\n", __func__);
	INIT_COMPLETION(ts->do_irq);
	retval = send_data_addr(ts->client, CY_REG_BASE, (void *)(&fw[loc]), CY_BL_ENTER_CMD_SIZE);
	if (retval < 0) {
		pr_err("[%s] BL loader fail startup r=%d\n", __func__, retval);
		goto _cyttsp_load_app_exit;
	}

	retval = _cyttsp_wait_bl_ready(ts, _cyttsp_chk_bl_startup, CY_TEN_SEC_TMO_MS);
	if (retval < 0) {
		pr_err("[%s]: BL loader startup err r=%d\n", __func__, retval);
		goto _cyttsp_load_app_exit;
	}

	loc += CY_BL_ENTER_CMD_SIZE;

	/* send bootload firmware load blocks */
	pr_info("%s: Send BL Loader Blocks\n", __func__);
	while ((fw_size - loc) > CY_BL_WR_BLK_CMD_SIZE) {
		pr_debug("[%s] BL loader block=%d f=%02X s=%02X e=%02X loc=%d\n", __func__,
			loc / CY_BL_WR_BLK_CMD_SIZE,
			ts->bl_data.bl_file,
			ts->bl_data.bl_status,
			ts->bl_data.bl_error,
			loc);
		retval = _cyttsp_wr_blk_chunks(ts, CY_REG_BASE, CY_BL_WR_BLK_CMD_SIZE, &fw[loc]);
		if (retval < 0) {
			pr_err("%s: BL loader fail r=%d block=%d loc=%d\n", __func__, retval,
				loc / CY_BL_WR_BLK_CMD_SIZE, loc);
			goto _cyttsp_load_app_exit;
		}

		_cyttsp_load_bl_regs(ts);

#if 0
		/* Checks firmware version*/
		if ((ts->bl_data.bl_status & CY_BL_BUSY) || (ts->bl_data.bl_error != CY_BL_RUNNING)) {
			/* signal a status err */
			pr_err("%s: BL READY ERR on write block bl_file=%02X bl_status=%02X bl_error=%02X\n",
				__func__,
				ts->bl_data.bl_file,
				ts->bl_data.bl_status,
				ts->bl_data.bl_error);
			retval = -EIO;
			goto _cyttsp_load_app_exit;
		} else {
			/* point to next block */
			loc += CY_BL_WR_BLK_CMD_SIZE;
		}
#else
		loc += CY_BL_WR_BLK_CMD_SIZE;
#endif
	}

	/* send bootload terminate command */
	pr_info("%s: Send BL Loader Terminate\n", __func__);
	if (loc == (fw_size - CY_BL_EXIT_CMD_SIZE)) {
		INIT_COMPLETION(ts->do_irq);

		retval = send_data_addr(ts->client, CY_REG_BASE, (void *)(&fw[loc]), CY_BL_EXIT_CMD_SIZE);
		if (retval < 0) {
			pr_err("%s: BL fail Terminate r=%d\n", __func__, retval);
			goto _cyttsp_load_app_exit;
		}

		retval = _cyttsp_wait_bl_ready(ts, _cyttsp_chk_bl_terminate, CY_TEN_SEC_TMO_MS);
		if (retval < 0) {
			pr_err("%s: BL Loader Terminate err r=%d\n", __func__, retval);
			/*
			 * if app valid bit is set
			 * return success and let driver
			 * try a normal restart anyway
			 */
			if (IS_VALID_APP(ts->bl_data.bl_status)) {
				pr_err("%s: BL Loader Valid App indicated bl_s=%02X\n",
						__func__, ts->bl_data.bl_status);
				retval = 0;
			}
		}
	} else {
		pr_err("%s: FW size mismatch\n", __func__);
		retval = -EINVAL;
		goto _cyttsp_load_app_exit;
	}

_cyttsp_load_app_exit:
	return retval;
}

static int cyttsp_boot_loader(struct ft5x_ts_data *ts)
{
	int retval = 0;
	bool new_vers = false;
	struct cyttsp_vers *fw_v = NULL;

	pr_info("[%s] enter\n", __func__);

	if ((ts->fw->ver == NULL) || (ts->fw->img == NULL)) {
		pr_err("[%s]: empty version list or no image\n", __func__);
		goto _cyttsp_boot_loader_exit;
	}

	if (ts->fw->vsize != CY_BL_VERS_SIZE) {
		pr_err("%s: bad fw version list size=%d\n", __func__, ts->fw->vsize);
		goto _cyttsp_boot_loader_exit;
	}

	/* automatically update firmware if new version detected */
	fw_v = (struct cyttsp_vers *)ts->fw->ver;

	new_vers = (((u16)fw_v->app_verh << 8) + (u16)fw_v->app_verl) !=
		   (((u16)ts->bl_data.appver_hi << 8) + (u16)ts->bl_data.appver_lo);

	if (IS_VALID_APP(ts->bl_data.bl_status))
		pr_debug("[%s] APP IS VALID\n", __func__);
	if (new_vers)
		pr_debug("[%s] NEW VERSION\n", __func__);

	/* Force firmware update */
	retval = cyttsp_load_app(ts, ts->fw->img, ts->fw->size);
	if (retval < 0) {
		pr_err("[%s]: bus fail on load fw\n", __func__);
		goto _cyttsp_boot_loader_exit;
	}

	ctp_reset();;

_cyttsp_boot_loader_exit:
	return retval;
}

static int _cyttsp_hndshk(struct ft5x_ts_data *ts, u8 hst_mode)
{
	int retval = 0;
	u8 mode = 0;

	mode = hst_mode & CY_HNDSHK_BIT ?
		hst_mode & ~CY_HNDSHK_BIT :
		hst_mode | CY_HNDSHK_BIT;

	retval = send_data_addr(ts->client, CY_REG_BASE, &mode, sizeof(mode));

	if (retval < 0)
		pr_err("%s: bus write fail on handshake r=%d\n", __func__, retval);

	return retval;
}

/* TrueTouch Standard Product Gen3 interface definition */
struct cyttsp_status_regs {
	u8 hst_mode;
	u8 tt_mode;
	u8 tt_stat;
} __packed;

static int _cyttsp_load_status_regs(struct ft5x_ts_data *ts,
	struct cyttsp_status_regs *status_regs)
{
	int retval = 0;

	memset(status_regs, 0, sizeof(struct cyttsp_status_regs));

	retval = get_data(ts->client, CY_REG_BASE, (u8 *) status_regs, sizeof(struct cyttsp_status_regs));

	if (retval < 0)
		pr_err("%s: bus fail reading status regs r=%d\n", __func__, retval);

	return retval;
}

static int _cyttsp_exit_bl_mode(struct ft5x_ts_data *ts)
{
	struct cyttsp_status_regs status_regs;
	int retval = 0;
	u8 bl_cmd[] = {0x00, 0xFF, 0xA5,/* BL file 0, cmd seq, exit cmd */
		0, 1, 2, 3, 4, 5, 6, 7};/* Default BL keys */

	pr_info("[%s] enter\n", __func__);

	memset(&status_regs, 0, sizeof(struct cyttsp_status_regs));
	if (ts->sett[CY_IC_GRPNUM_BL_KEY] != NULL) {
		if ((ts->sett[CY_IC_GRPNUM_BL_KEY]->data != NULL) &&
		    (ts->sett[CY_IC_GRPNUM_BL_KEY]->size <= CY_BL_NUM_KEYS)) {
			memcpy(&bl_cmd[3], ts->sett[CY_IC_GRPNUM_BL_KEY]->data,
			                   ts->sett[CY_IC_GRPNUM_BL_KEY]->size);
		} else {
			pr_err("[%s]: Too many platform bootloader keys--%s.\n",
				__func__, "using defaults instead");
		}
	} else {
		pr_err("[%s]: Missing platform bootloader keys--%s.\n",
			__func__, "using defaults instead");
	}

	/* send exit command; wait for host mode interrupt */
	retval = _cyttsp_wait_ready(ts, &ts->do_irq, bl_cmd, sizeof(bl_cmd), CY_TEN_SEC_TMO_MS);
	if (retval < 0) {
		pr_err("[%s]: fail wait ready r=%d\n", __func__, retval);
		goto _cyttsp_exit_bl_mode_exit;
	}

	retval = _cyttsp_load_status_regs(ts, &status_regs);
	if (retval < 0) {
		pr_err("[%s]: Fail read status regs r=%d\n", __func__, retval);
		goto _cyttsp_exit_bl_mode_exit;
	}

	retval = _cyttsp_hndshk(ts, status_regs.hst_mode);
	if (retval < 0) {
		pr_err("[%s]: Fail write handshake r=%d\n", __func__, retval);
		/* continue anyway */
		retval = 0;
	}

	if (IS_BOOTLOADERMODE(status_regs.tt_mode)) {
		pr_err("[%s]: Fail exit bootloader mode r=%d\n", __func__, retval);
		retval = -EIO;
	} else if (GET_HSTMODE(status_regs.hst_mode) != GET_HSTMODE(CY_OPERATE_MODE)) {
		pr_err("[%s]: Fail enter op mode=0x%02X\n", __func__, status_regs.hst_mode);
		retval = -EIO;
	}

_cyttsp_exit_bl_mode_exit:
	return retval;
}

static int ft5x_do_update_fw(struct ft5x_ts_data *ts)
{
	int retval;

	ts->fw = &cyttsp3_i2c_firmware;
	ts->client = this_client;
	ts->sett[0] = cyttsp3_sett[0];
	ts->sett[1] = cyttsp3_sett[1];
	ts->sett[2] = cyttsp3_sett[2];
	ts->sett[3] = cyttsp3_sett[3];

	init_completion(&ts->do_irq);
	ctp_reset();

	/* Interrupt should already be available and no need for explicit init */
	free_irq(SW_INT_IRQNO_PIO, ts);
	retval = request_irq(SW_INT_IRQNO_PIO, cyttsp_irq, IRQF_SHARED, "ft5x_ts", ts);
	if (retval < 0)
		pr_err("[%s] request irq failed\n", __func__);
	enable_irq(SW_INT_IRQNO_PIO);

	_cyttsp_load_bl_regs(ts);
	cyttsp_boot_loader(ts);
	_cyttsp_exit_bl_mode(ts);

	return 0;
}

