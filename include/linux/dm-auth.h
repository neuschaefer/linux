#ifndef _ROKU_MERKLE_H_
#define _ROKU_MERKLE_H_ 1

#include <linux/ioctl.h>

#ifndef __BIT_TYPES_DEFINED__
typedef unsigned int uint32_t;
#endif

enum { MH_VERSION = 1, MH_MAX_LEVELS = 8, MH_MAGIC = 0xD3CFA6B3 };

/*
 * Definition of a Merkle tree stored in a file.
 */
struct MerkleHeader {
    uint32_t         mh_magic;         /* must be MH_MAGIC */
    uint32_t         mh_version;       /* version of this header */
    uint32_t         mh_min_version;   /* min reader version */
    uint32_t         mh_header_size;   /* size of this header */
    uint32_t         mh_page_size;     /* size of hashed pages (usually 4096) */
    uint32_t         mh_hash_size;     /* size of hash value (16 for md5) */
    uint32_t         mh_udata_size;    /* size of user data */
    uint32_t         mh_num_levels;    /* num Merkle levels, including root */
    char             mh_hash_type[16]; /* name of hash, eg. "md5" */
    uint32_t         mh_pad1[8];
    unsigned char    mh_root_hash[32]; /* hash of root level (one page) */
    uint32_t         mh_pad2[8];
    uint32_t         mh_level_pos[MH_MAX_LEVELS];  /* pos of start of n-th level */
    uint32_t         mh_pad3[8];
    uint32_t         mh_level_size[MH_MAX_LEVELS]; /* size of n-th level */
    uint32_t         mh_pad4[8];
};

#define DMAUTH_FILENAME_MAX_LEN         64
struct MerkleFile {
    char             name[DMAUTH_FILENAME_MAX_LEN];
};

struct AuthDataHeader {
    char                 ah_filesystem[32]; /* Name of filesystem (usually "cramfs") */
    uint32_t             ah_wtype;          /* aimage type of authenticated data */
};

struct AuthData {
    struct AuthDataHeader ad_header;
    struct MerkleHeader   ad_mheader;
    unsigned char         ad_scratch[512];   /* grotesque kludge */
};

/* ioctl to write a MerkleHeader to a dm-auth device. */
#define DMAUTH_IOCTL_SET_MERKLE         _IOW(0xD8, 1, struct MerkleHeader)
#define DMAUTH_IOCTL_SET_MERKLE_STICKY  _IOW(0xD8, 2, struct MerkleHeader)
#define DMAUTH_IOCTL_SET_FILENAME       _IOW(0xD8, 3, struct MerkleFile)

/* ioctl to read authdata (MerkleHeader) from a block device.
 * Returns the offset within the partition of the start of the data
 * authenticated by the returned MerkleHeader. */
#define BLKGETAUTHDATA                  _IOWR(0x12, 159, struct AuthData)

#endif /* _ROKU_MERKLE_H_ */
