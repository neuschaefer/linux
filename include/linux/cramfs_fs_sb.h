#ifndef _CRAMFS_FS_SB
#define _CRAMFS_FS_SB

/*
 * cramfs super-block data in memory
 */
struct cramfs_sb_info {
			unsigned long magic;
			unsigned long size;
			unsigned long blocks;
			unsigned long files;
			unsigned long flags;
			unsigned long uid;
			unsigned long gid;
};

static inline struct cramfs_sb_info *CRAMFS_SB(struct super_block *sb)
{
	return sb->s_fs_info;
}

#define CRAMFS_SUPER_SIZE  512
#define CRAMFS_AUTH_SCRATCH_SIZE  (2 * CRAMFS_SUPER_SIZE)

#endif
