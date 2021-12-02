#ifndef FLASH_LAYOUT_H
#define FLASH_LAYOUT_H

#define NAND_FLASH_TOTAL_SIZE               (64*1024*1024)
#ifdef LOADER_MAX_SIZE
    #define NAND_FLASH_LOADER_SIZE          (LOADER_MAX_SIZE)   /*(640*1024) 640 KB */
#else /* LOADER_MAX_SIZE */
    #define NAND_FLASH_LOADER_SIZE          (640*1024)          /*(640*1024) 640 KB */
#endif /* LOADER_MAX_SIZE */
#define NAND_FLASH_APPIMAGE_SIZE            (11*1024*1024)      /* 11 MB */
#define NAND_FLASH_SINGLE_CHANNELLIST_SIZE  (256*1024)          /* 256 KB to support 3000 channels*/
#define NAND_FLASH_PANEL_SIZE               (128*1024)          /* 128 KB */
#define NAND_FLASH_PQ_SIZE                  (17*1024*1024)      /* 17 MB */
#define NAND_FLASH_AQ_SIZE                  (512*1024)          /* 512 KB */
#define NAND_FLASH_FS_SIZE                  (22*1024*1024)      /* 22 MB KB */ 
#define NAND_FLASH_CIPLUS_SIZE              (128*1024)          /* 128 KB */
#define NAND_FLASH_NTFS_SIZE                (192*1024)          /* 192 KB */
#define NAND_FLASH_EDID_SIZE                (128*1024)          /* 128 KB */
#define NAND_FLASH_GAMMA_SIZE               (128*1024)          /* 128 KB */
#define NAND_PART_MAX_IMG_SIZE              (9.5*1024*1024)     /* 9.5 MB */

#define NAND_PART_SIZE_0        (NAND_FLASH_LOADER_SIZE)                /* loader */
#define NAND_PART_SIZE_1        (NAND_FLASH_APPIMAGE_SIZE)              /* appimg */
#define NAND_PART_SIZE_2        (0)                                     /* up image */
#define NAND_PART_SIZE_3        (NAND_FLASH_APPIMAGE_SIZE)              /* appimg */
#define NAND_PART_SIZE_4        (0)                                     /* up image */
#define NAND_PART_SIZE_5        (NAND_FLASH_SINGLE_CHANNELLIST_SIZE)    /* channel list */
#define NAND_PART_SIZE_6        (NAND_FLASH_SINGLE_CHANNELLIST_SIZE)    /* channel list */
#define NAND_PART_SIZE_7        (NAND_FLASH_PANEL_SIZE)                 /* flash Panel */
#define NAND_PART_SIZE_8        (NAND_FLASH_PQ_SIZE)                    /* flash PQ */
#define NAND_PART_SIZE_9        (NAND_FLASH_AQ_SIZE)                    /* flash AQ */
#define NAND_PART_SIZE_10       (NAND_FLASH_FS_SIZE)                    /* flash fs */
#define NAND_PART_SIZE_11       (NAND_FLASH_CIPLUS_SIZE)                /* CI plus */
#define NAND_PART_SIZE_12       (NAND_FLASH_NTFS_SIZE)                  /* NTFS 1*/
#define NAND_PART_SIZE_13       (NAND_FLASH_NTFS_SIZE)                  /* NTFS 2*/
#define NAND_PART_SIZE_14       (NAND_FLASH_EDID_SIZE)                  /* EDID */
#define NAND_PART_SIZE_15       (NAND_FLASH_GAMMA_SIZE)                 /* Gamma */

#define NAND_FLASH_SYS_IMG_0_PART_ID    (1)
#define NAND_FLASH_SYS_IMG_1_PART_ID    (3)

#define NAND_PART_SIZE_MAX_0            (0x0040000)                 /*256 KB*/
#define NAND_PART_SIZE_MAX_1            (NAND_PART_MAX_IMG_SIZE)
#define NAND_PART_SIZE_MAX_2            (0x0000000)
#define NAND_PART_SIZE_MAX_3            (NAND_PART_MAX_IMG_SIZE)
#define NAND_PART_SIZE_MAX_4            (0x0000000)

#define NAND_PART_NAME_5                "nand_writer/ff.bin"
#define NAND_PART_NAME_6                "nand_writer/ff.bin"
#define NAND_PART_NAME_7                "data/panel.bin"
#define NAND_PART_NAME_8                "data/pq.bin"
#define NAND_PART_NAME_9                "data/aq.bin"
#define NAND_PART_NAME_10               "data/flash_fs.bin"
#define NAND_PART_NAME_11               ""
#define NAND_PART_NAME_12               "data/ntfs.bin"
#define NAND_PART_NAME_13               "data/ntfs.bin"
#define NAND_PART_NAME_14               "nand_writer/ff.bin"
#define NAND_PART_NAME_15               "nand_writer/ff.bin"

#define NAND_FLASH_STATIC_LAST_PART_ID   (4)

#define NAND_PART_END                    (16)    /* total 16 Partitions */

#define NAND_FLASH_STATIC_PART_SIZE \
    (   (NAND_PART_SIZE_0) + (NAND_PART_SIZE_1) + \
        (NAND_PART_SIZE_2) + (NAND_PART_SIZE_3) + (NAND_PART_SIZE_4))

#define NAND_FLASH_DYNAMIC_PART_USED_SIZE \
    (   (NAND_PART_SIZE_5) + (NAND_PART_SIZE_6) + (NAND_PART_SIZE_7) + \
        (NAND_PART_SIZE_8) + (NAND_PART_SIZE_9) + (NAND_PART_SIZE_10) + \
        (NAND_PART_SIZE_11) + (NAND_PART_SIZE_12) + (NAND_PART_SIZE_13) + \
        (NAND_PART_SIZE_14) + (NAND_PART_SIZE_15))

/*Flash PQ customization*/
#ifdef SUPPORT_FLASH_PQ
#define FLASH_PQ_BASE_PARTITION             (8)
#define FLASH_PQ_BASE_ADDRESS               (0)
#define FLASH_PQ_BASE_OFFSET				(0)
#define FLASH_PQ_BLOCK_SIZE					(1)
#endif

#define FLASH_AQ_BASE_PARTITION             (9)
#define FLASH_AQ_BASE_ADDRESS               (0)
#define LOADER_DUAL_BOOT_OFFSET             (NAND_PART_SIZE_0) + (NAND_PART_SIZE_1) + (NAND_PART_SIZE_2)


#ifdef CC_PANEL_FROM_FLASH

/* Flash Panel Table */
#define PANEL_FLASH_HEADER_ADDRESS          (0)
#define PANEL_FLASH_HEADER_LENGTH           (32)
#define PANEL_FLASH_TABLE_SIZE	            (176)
#define PANEL_FLASH_TABLE_BASE_PARTITION    (7)
#define PANEL_FLASH_TABLE_ADDRESS           (32)
#endif  /* CC_PANEL_FROM_FLASH */

#endif /*FLASH_LAYOUT_H*/
