#ifndef IBLOCKSTRUCT_H
#define IBLOCKSTRUCT_H

struct foxconn_hdr {
    char serial[13];
    char emac[18];
    char wifimac[18];
    char spare[7];
    unsigned version;
    unsigned recno;
};

struct iblock_struct {
    /* 0x000 */ unsigned char signature[256];
    /* 0x100 */ unsigned version;
    /* 0x104 */ unsigned length;
    /* 0x108 */ unsigned char privdat[56];
    /* 0x140 */ unsigned char tdeskey[24];
    /* 0x158 */ unsigned char aes_key[16];
    /* 0x168 */ unsigned char uniqueid[8];
    /* 0x170 */ unsigned char kpe[16];
    /* 0x180 */ unsigned char kph[32];
    /* 0x1a0 */ unsigned char bypasstest[16];
    /* 0x1b0 */ unsigned char level2test[16];
    /* 0x1c0 */ unsigned char level3test[16];
    /* 0x1d0 */ unsigned char mmapsum[16];
    /* 0x1e0 */ unsigned char fskey[16];
    /* 0x1f0 */ unsigned char plugkey[16];
    /* 0x200 */ unsigned char exkey1[16];
    /* 0x210 */ unsigned char exkey2[16];
    /* 0x220 */ unsigned char exkey3[16];
    /* 0x230 */ unsigned char exkey4[16];
    /* 0x240 */ unsigned char hdcp_key[16];
    /* 0x250 */ unsigned char jtag_key[16];
    /* 0x260 */ unsigned char boot_key[16];
};

struct manu_record {
    struct foxconn_hdr hdr;
    struct iblock_struct ib;
};

struct manu_header {
    unsigned magic;
    unsigned version;
    unsigned record_count;
    unsigned size;
    char description[128];
};

#define MANU_HEADER_MAGIC 0x58464C4E

#endif //IBLOCKSTRUCT_H
