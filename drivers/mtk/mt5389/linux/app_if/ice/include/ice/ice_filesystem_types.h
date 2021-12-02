/*
 * ice_filesystem_types.h
 *
 *  Created on: Nov 2, 2010
 *      Author: korayg
 */

#ifndef CABOT_ICE_ICE_FILESYSTEM_TYPES_H_INCLUDED
#define CABOT_ICE_ICE_FILESYSTEM_TYPES_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    VFAT,
    NTFS,
    EXT3,
    EXT2,
    UNDEFINED_FS
}fs_type_t;

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_FILESYSTEM_TYPES_H_INCLUDED */
