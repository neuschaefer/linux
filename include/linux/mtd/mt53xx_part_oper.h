/*
 * mt53xx partition operation functions
 */

#ifndef __MT53XX_PART_OPER_H__
#define __MT53XX_PART_OPER_H__

#include <linux/types.h>

int i4NFBPartitionFastboot(struct mtd_info *mtd);
int i4NFBPartitionRead(uint32_t u4DevId, uint32_t u4Offset, uint32_t u4MemPtr, uint32_t u4MemLen);
int i4NFBPartitionWrite(uint32_t u4DevId, uint32_t u4Offset, uint32_t u4MemPtr, uint32_t u4MemLen);

#endif /* !__MT53XX_PART_OPER_H__ */
