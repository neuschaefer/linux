/*
 * mt53xx partition operation functions
 */

#ifndef __MTK_MSDCPART_H__
#define __MTK_MSDCPART_H__

#include <linux/types.h>

int msdc_partread(int partno, uint32_t offset, uint32_t size, void *pvmem);
int msdc_partwrite(int partno, uint32_t offset, uint32_t size, void *pvmem);
int msdc_partread_byname(const char *name, uint32_t offset, uint32_t size, void *pvmem);
int msdc_partwrite_byname(const char *name, uint32_t offset, uint32_t size, void *pvmem);

#endif /* __MTK_MSDCPART_H__ */
