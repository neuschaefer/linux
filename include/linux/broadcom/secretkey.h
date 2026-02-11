#ifndef _SECRET_KEY_H
#define _SECRET_KEY_H

#include <linux/ioctl.h>
#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <stdint.h>
#endif


#define SECRETKEY_MAGIC_1    'k'

struct secretkey_data {
	/* operation to perform */
	unsigned int mode;

	/* input/output data */
	uint32_t data[4];
};

/* ioctl() command numbers */
#define SECRETKEY_CMD_OP   0x80

#define SECRETKEY_DEV_NAME "secretkey"

/*
 * Perform a secretkey operation. Result is stored in the 'data' field
 */
#define SECRETKEY_IOCTL_OP   _IOWR(SECRETKEY_MAGIC_1, SECRETKEY_CMD_OP, struct secretkey_data)

#endif  /* _SECRETKEY_H */
