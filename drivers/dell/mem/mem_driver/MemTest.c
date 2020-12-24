#include <asm/ioctl.h> 
#include <pthread.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <sys/ioctl.h>
#include <unistd.h>    /*Ref. exit(), syscall()*/


/* type define */
typedef unsigned char       UINT8;
typedef unsigned short int  UINT16;
typedef unsigned long int   UINT32;

/* IOCTL command */
#define AESS_MEMDRV_IOC_MAGIC    0xB4
#define AESS_MEMDRV_RW           _IOWR(AESS_MEMDRV_IOC_MAGIC, 0, int)
#define AESS_MEMDRV_READ         _IOWR(AESS_MEMDRV_IOC_MAGIC, 1, int)
#define AESS_MEMDRV_WRITE        _IOWR(AESS_MEMDRV_IOC_MAGIC, 2, int)
#define AESS_MEMDRV_REQUEST      _IOWR(AESS_MEMDRV_IOC_MAGIC, 3, int)
#define AESS_MEMDRV_RELEASE      _IOWR(AESS_MEMDRV_IOC_MAGIC, 4, int)

#define MEM_READ      0
#define MEM_WRITE     1

/* define the limit of memory driver */
#define MEM_DRV_MAX_REGION_NUM      10
#define MEM_DRV_MAX_REGION_SIZE     65535


/******************************************************************************
*   Enum      :   eMemDrvDataWidthType
******************************************************************************/
/**
 *  @brief   memory driver data width enumeration type
 *
 *****************************************************************************/
typedef enum
{
	/** Byte */
	MEM_DRV_BYTE = 0,
	
	/** Word */
	MEM_DRV_WORD,
	
	/** Double word */
	MEM_DRV_DWORD
	
} eMemDrvDataWidthType;


/******************************************************************************
*   STRUCT      :   sMemDrvInfoType
******************************************************************************/
/**
 *  @brief   Structure to memory driver related data parameter.
 *
 *****************************************************************************/
typedef struct
{
	/* physical base address of request region */
	UINT32 u32BaseAddr;
    
	/* size of request region in bytes */
	UINT16 u16RegionSize;
    
	/* offset of base address in bytes */
	UINT16 u16Offset;
    
	/* read/write data buffer pointer */
	void *pDataPtr;
    
	/* data size to read/write */
	UINT16 u16DataSize;
    
	/* read/write data width */
	UINT8 u8DataWidth;
	
	/* identifier */
	UINT8 u8ID;
	
} sMemDrvInfoType;


UINT8 G_u8ReadBuf[64];
UINT16 G_u16ReadBuf[32];
UINT32 G_u32ReadBuf[16];


static const unsigned long crc_table[256] = {
  0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L,
  0x706af48fL, 0xe963a535L, 0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L,
  0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L,
  0x90bf1d91L, 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
  0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L,
  0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L,
  0xfa0f3d63L, 0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L,
  0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
  0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L,
  0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L, 0x26d930acL, 0x51de003aL,
  0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L,
  0xb8bda50fL, 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
  0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL, 0x76dc4190L,
  0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL,
  0x9fbfe4a5L, 0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL,
  0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
  0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL,
  0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L,
  0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L,
  0xfbd44c65L, 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
  0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL,
  0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L,
  0xaa0a4c5fL, 0xdd0d7cc9L, 0x5005713cL, 0x270241aaL, 0xbe0b1010L,
  0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
  0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L,
  0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L,
  0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L,
  0x73dc1683L, 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
  0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L,
  0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL,
  0x196c3671L, 0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL,
  0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
  0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L,
  0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL, 0xd80d2bdaL, 0xaf0a1b4cL,
  0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL,
  0x4669be79L, 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
  0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL, 0xc5ba3bbeL,
  0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L,
  0x2cd99e8bL, 0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL,
  0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
  0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL,
  0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L,
  0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L,
  0x18b74777L, 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
  0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L, 0xa00ae278L,
  0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L,
  0x4969474dL, 0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L,
  0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
  0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L,
  0xcdd70693L, 0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L,
  0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL,
  0x2d02ef8dL
};


/* ========================================================================= */
#define DO1(buf) crc = crc_table[((int)crc ^ (*buf++)) & 0xff] ^ (crc >> 8);
#define DO2(buf)  DO1(buf); DO1(buf);
#define DO4(buf)  DO2(buf); DO2(buf);
#define DO8(buf)  DO4(buf); DO4(buf);


/* ========================================================================= */
static unsigned long crc32(unsigned long crc,
                           const unsigned char *buf,
                           unsigned int len)
{
    crc = crc ^ 0xffffffffL;
    while (len >= 8)
    {
      DO8(buf);
      len -= 8;
    }
    if (len) do {
      DO1(buf);
    } while (--len);
    return crc ^ 0xffffffffL;
}

int main(int argc, char* argv[])
{
    int i;
    UINT32 u32MEMDrvFD;
    char d_path[64] ="/dev/aess_memdrv";
    
    sMemDrvInfoType sMemDrvInfo;
    
    char function;
    char tmp_type;
    int type = 2;
    unsigned long data;
    unsigned long address;
    unsigned char rc = 0;
    unsigned char crcchkdata[16];
    int crc;
    
    if (argc < 2)
    {
        goto help;
    }
    
    if (argc < 3)
    {
        #define _AMEA_BOARD_ADDR    0x44000012
        #define _PLATFORM_ID_ADDR   0xC4000003
        #define _COMM_UBOOT_KRNL_ADDR 0x46000000
        if( strcmp( "IS_AMEA_BOARD_EXIST" , argv[1] )== 0 )
        {
            sMemDrvInfo.u32BaseAddr = (_AMEA_BOARD_ADDR ) ;
            sMemDrvInfo.u16RegionSize = 1;
            
            u32MEMDrvFD = open(d_path, O_RDWR, 0);

            if (ioctl(u32MEMDrvFD, AESS_MEMDRV_REQUEST, (UINT32) &sMemDrvInfo))
            {
                printf("ioctl(AESS_MEMDRV_REQUEST) error\n");
            }
            sMemDrvInfo.u16Offset = 0;
            sMemDrvInfo.pDataPtr = (void *) &rc;
            sMemDrvInfo.u16DataSize = 1;
            sMemDrvInfo.u8DataWidth = 0;
            
            if (ioctl(u32MEMDrvFD, AESS_MEMDRV_READ, (UINT32) &sMemDrvInfo))
            {
                printf("ioctl(AESS_MEMDRV_READ) error\n");
            }
            
            if (ioctl(u32MEMDrvFD, AESS_MEMDRV_RELEASE, (UINT32) &sMemDrvInfo))
            {
                printf("ioctl(AESS_MEMDRV_RELEASE) error\n");
            }
            printf("\n IS_AMEA_BOARD_EXIST = %x ",rc);
            
            if( rc & 0x02 )
                return 0 ;
            else                                
                return 1 ;
        }
        else if( strcmp( "PLATFORM_ID" , argv[1] )== 0 )
        {
            sMemDrvInfo.u32BaseAddr = (_PLATFORM_ID_ADDR ) ;
            sMemDrvInfo.u16RegionSize = 1;
            
            u32MEMDrvFD = open(d_path, O_RDWR, 0);

            if (ioctl(u32MEMDrvFD, AESS_MEMDRV_REQUEST, (UINT32) &sMemDrvInfo))
            {
                printf("ioctl(AESS_MEMDRV_REQUEST) error\n");
            }
            sMemDrvInfo.u16Offset = 0;
            sMemDrvInfo.pDataPtr = (void *) &rc;
            sMemDrvInfo.u16DataSize = 1;
            sMemDrvInfo.u8DataWidth = 0;
            
            if (ioctl(u32MEMDrvFD, AESS_MEMDRV_READ, (UINT32) &sMemDrvInfo))
            {
                printf("ioctl(AESS_MEMDRV_READ) error\n");
            }
            
            if (ioctl(u32MEMDrvFD, AESS_MEMDRV_RELEASE, (UINT32) &sMemDrvInfo))
            {
                printf("ioctl(AESS_MEMDRV_RELEASE) error\n");
            }
            printf("\n PLATFORM_ID = %x ",rc);
            return rc ;
        }
        else if( strcmp( "RdUBootKrnlCRC" , argv[1] )== 0 )
        {
            sMemDrvInfo.u32BaseAddr = (_COMM_UBOOT_KRNL_ADDR ) ;
            sMemDrvInfo.u16RegionSize = 16;
            
            u32MEMDrvFD = open(d_path, O_RDWR, 0);

            if (ioctl(u32MEMDrvFD, AESS_MEMDRV_REQUEST, (UINT32) &sMemDrvInfo))
            {
                printf("ioctl(AESS_MEMDRV_REQUEST) error\n");
            }
            sMemDrvInfo.u16Offset = 0;
            sMemDrvInfo.pDataPtr = (void *) crcchkdata;
            sMemDrvInfo.u16DataSize = 16;
            sMemDrvInfo.u8DataWidth = 0;
            
            for (i =0; i < 16; i++)
            {
                if (ioctl(u32MEMDrvFD, AESS_MEMDRV_READ, (UINT32) &sMemDrvInfo))
                {
                    printf("ioctl(AESS_MEMDRV_READ) error\n");
                }
                printf("UBootKrnlData[%02d] = 0x%02X\n", i, crcchkdata[i]);
            }
            
            if (ioctl(u32MEMDrvFD, AESS_MEMDRV_RELEASE, (UINT32) &sMemDrvInfo))
            {
                printf("ioctl(AESS_MEMDRV_RELEASE) error\n");
            }
            
            return rc ;
        }
        else if( strcmp( "WrUBootKrnlCRC" , argv[1] )== 0 )
        {
            sMemDrvInfo.u32BaseAddr = (_COMM_UBOOT_KRNL_ADDR ) ;
            sMemDrvInfo.u16RegionSize = 16;
            
            u32MEMDrvFD = open(d_path, O_RDWR, 0);
            
            if (ioctl(u32MEMDrvFD, AESS_MEMDRV_REQUEST, (UINT32) &sMemDrvInfo))
            {
                printf("ioctl(AESS_MEMDRV_REQUEST) error\n");
            }
            
            sMemDrvInfo.u16Offset = 0;
            sMemDrvInfo.pDataPtr = (void *) crcchkdata;
            sMemDrvInfo.u16DataSize = 16;
            sMemDrvInfo.u8DataWidth = 0;
            
            for (i =0; i < 16; i++)
            {
                if (ioctl(u32MEMDrvFD, AESS_MEMDRV_READ, (UINT32) &sMemDrvInfo))
                {
                    printf("ioctl(AESS_MEMDRV_READ) error\n");
                }
                printf("UBootKrnlData[%02d] = 0x%02X\n", i, crcchkdata[i]);
            }

            crc = crc32(0, (char *) &crcchkdata[4], 12);
            printf("UBootKrnlData CRC = 0x%X is written...\n", crc);

            sMemDrvInfo.u16Offset = 0;
            sMemDrvInfo.pDataPtr = (void *) &crc;
            sMemDrvInfo.u16DataSize = 1;
            sMemDrvInfo.u8DataWidth = 2;
            
            if (ioctl(u32MEMDrvFD, AESS_MEMDRV_WRITE, (UINT32) &sMemDrvInfo))
            {
                printf("ioctl(AESS_MEMDRV_WRITE) error\n");
            }
            
            if (ioctl(u32MEMDrvFD, AESS_MEMDRV_RELEASE, (UINT32) &sMemDrvInfo))
            {
                printf("ioctl(AESS_MEMDRV_RELEASE) error\n");
            }
            
            return rc ;
        }
help:
        printf("\n" \
               " Avocent Memory Test Tool                        Ver.0.2 \n" \
               "---------------------------------------------------------\n" \
               "\n" \
               " MEMORY READ\n" \
               "    %s -r[b|w|l] phy_address\n" \
               "\n" \
               " MEMORY DUMP\n" \
               "    %s -d[b|w|l] phy_address\n" \
               "\n" \
               " MEMORY WRITE\n" \
               "    %s -w[b|w|l] phy_address [data]\n" \
               "\n" \
               " MEMORY WRITE-AND-CHECK\n" \
               "    %s -c[b|w|l] phy_address [data]\n" \
               "\n" \
               "      b|w|l       - byte | word | long, default is long\n" \
               "      phy_address - physical address (hex)\n" \
               "      data        - data (hex)\n" \
               "\n" \
               " EXAMPLE \n" \
               "    %s -r b0000204\n" \
               "    %s -wb c8001001 c0\n" \
               "\n" \
               "    %s IS_AMEA_BOARD_EXIST\n" \
               "    %s PLATFORM_ID\n" \
               "    %s RdUBootKrnlCRC\n" \
               "    %s WrUBootKrnlCRC\n" \
               "\n",
               argv[0], argv[0], argv[0], argv[0], argv[0],
               argv[0], argv[0], argv[0], argv[0], argv[0]);
        
        return 0;
    }
    
    function = argv[1][0];
    address = strtoul(argv[2],0,16);
    if (function == '-')
    {
        if (argv[1][1] != 0)
        {
            function = argv[1][1];
            
            if (argv[1][2] != 0)
            {
                tmp_type = argv[1][2];
                
                switch (tmp_type)
                {
                    case 'b':
                    case 'B':
                        type = 0;
                        break;
                    case 'w':
                    case 'W':
                        if (address & 0x01)
                        {
                            printf("phy_address is not aligned!\n");
                            return 0;
                        }
                        type = 1;
                        break;
                    case 'l':
                    case 'L':
                        if (address & 0x03)
                        {
                            printf("phy_address is not aligned!\n");
                            return 0;
                        }
                        type = 2;
                        break;
                }
            }
            else
            {
                if (address & 0x03)
                {
                    printf("phy_address is not aligned!\n");
                    return 0;
                }
            }
        }
        else
        {
            goto help;
        }
    }
    else
    {
        goto help;
    }
    
    u32MEMDrvFD = open(d_path, O_RDWR, 0);
    
    switch (function)
    {
        case 'h':
        case 'H':
            goto help;
        
        case 'r':
        case 'R':
            
            sMemDrvInfo.u32BaseAddr = (address & 0xFFFFFFF0);
            sMemDrvInfo.u16RegionSize = 64;
            
            if (ioctl(u32MEMDrvFD, AESS_MEMDRV_REQUEST, (UINT32) &sMemDrvInfo))
            {
                printf("ioctl(AESS_MEMDRV_REQUEST) error\n");
                return -1;
            }
            
            sMemDrvInfo.u16Offset = (address & 0x0F);;
            
            switch (type)
            {
                case 0:
                    sMemDrvInfo.pDataPtr = (void *) &G_u8ReadBuf[0];
                    break;
                case 1:
                    sMemDrvInfo.pDataPtr = (void *) &G_u16ReadBuf[0];
                    break;
                case 2:
                    sMemDrvInfo.pDataPtr = (void *) &G_u32ReadBuf[0];
                    break;
            }
            
            sMemDrvInfo.u16DataSize = 1;
            sMemDrvInfo.u8DataWidth = type;
            
            if (ioctl(u32MEMDrvFD, AESS_MEMDRV_READ, (UINT32) &sMemDrvInfo))
            {
                printf("ioctl(AESS_MEMDRV_READ) error\n");
                return -1;
            }
            
            if (ioctl(u32MEMDrvFD, AESS_MEMDRV_RELEASE, (UINT32) &sMemDrvInfo))
            {
                printf("ioctl(AESS_MEMDRV_RELEASE) error\n");
                return -1;
            }
            
            switch (type)
            {
                case 0:
                    printf("0x%08lx -> %02x\n\n", address, G_u8ReadBuf[0]);
                    break;
                    
                case 1:
                    printf("0x%08lx -> %04x\n\n", address, G_u16ReadBuf[0]);
                    break;
                    
                case 2:
                    printf("0x%08lx -> %08lx\n\n", address, G_u32ReadBuf[0]);
                    break;
            }
            
            break;
            
        case 'd':
        case 'D':
            
            sMemDrvInfo.u32BaseAddr = (address & 0xFFFFFFF0);
            sMemDrvInfo.u16RegionSize = 64;
            
            if (ioctl(u32MEMDrvFD, AESS_MEMDRV_REQUEST, (UINT32) &sMemDrvInfo))
            {
                printf("ioctl(AESS_MEMDRV_REQUEST) error\n");
                return -1;
            }
            
            sMemDrvInfo.u16Offset = 0;
            
            switch (type)
            {
                case 0:
                    sMemDrvInfo.pDataPtr = (void *) &G_u8ReadBuf[0];
                    break;
                case 1:
                    sMemDrvInfo.pDataPtr = (void *) &G_u16ReadBuf[0];
                    break;
                case 2:
                    sMemDrvInfo.pDataPtr = (void *) &G_u32ReadBuf[0];
                    break;
            }
            
            sMemDrvInfo.u16DataSize = 64 >> type;
            sMemDrvInfo.u8DataWidth = type;
            
            if (ioctl(u32MEMDrvFD, AESS_MEMDRV_READ, (UINT32) &sMemDrvInfo))
            {
                printf("ioctl(AESS_MEMDRV_READ) error\n");
                return -1;
            }
            
            if (ioctl(u32MEMDrvFD, AESS_MEMDRV_RELEASE, (UINT32) &sMemDrvInfo))
            {
                printf("ioctl(AESS_MEMDRV_RELEASE) error\n");
                return -1;
            }
            
            switch (type)
            {
                case 0:
                    
                    printf("\n              0  1  2  3  4  5  6  7 :  8  9  A  B  C  D  E  F\n");
                    printf("--------------------------------------------------------------\n");
                    
                    for (i = 0; i < 64; i++)
                    {
                        if (i%16 == 0)
                        {
                            printf("0x%08lx = ", ((address & 0xFFFFFFF0) + i));
                        }
                        
                        printf("%02x ", G_u8ReadBuf[i]);
                        
                        
                        if (i%16 == 7)
                        {
                            printf(": ");
                        }
                        
                        if (i%16 == 15)
                        {
                            printf("\n");
                        }
                    }
                    
                    printf("\n");
                    
                    break;
                    
                case 1:
                    printf("\n                0    2    4    6 :    8    A    C    E\n");
                    printf("------------------------------------------------------\n");
                    
                    for (i = 0; i < 32; i++)
                    {
                        if (i%8 == 0)
                        {
                            printf("0x%08lx = ", ((address & 0xFFFFFFF0) + (i<<type)));
                        }
                        
                        printf("%04x ", G_u16ReadBuf[i]);
                        
                        if (i%8 == 3)
                        {
                            printf(": ");
                        }
                        
                        if (i%8 == 7)
                        {
                            printf("\n");
                        }
                    }
                    
                    printf("\n");
                    
                    break;
                    
                case 2:
                    
                    printf("\n                    0        4 :        8        C\n");
                    printf("--------------------------------------------------\n");
                    
                    for (i = 0; i < 16; i++)
                    {
                        if (i%4 == 0)
                        {
                            printf("0x%08lx = ", ((address & 0xFFFFFFF0) + (i<<type)));
                        }
                        
                        printf("%08lx ", G_u32ReadBuf[i]);
                        
                        if (i%4 == 1)
                        {
                            printf(": ");
                        }
                        
                        if (i%4 == 3)
                        {
                            printf("\n");
                        }
                    }
                    
                    printf("\n");
                    
                    break;
            }
            
            break;
            
        case 'w':
        case 'W':
        case 'c':
        case 'C':
            
            if (argc < 4)
            {
                goto help;
            }
            
            data = strtoul(argv[3],0,16);
            
            sMemDrvInfo.u32BaseAddr = (address & 0xFFFFFFF0);
            sMemDrvInfo.u16RegionSize = 16;
            
            if (ioctl(u32MEMDrvFD, AESS_MEMDRV_REQUEST, (UINT32) &sMemDrvInfo))
            {
                printf("ioctl(AESS_MEMDRV_REQUEST) error\n");
                return -1;
            }
            
            sMemDrvInfo.u16Offset = (address & 0x0F);
            sMemDrvInfo.pDataPtr = (void *) &data;
            sMemDrvInfo.u16DataSize = 1;
            sMemDrvInfo.u8DataWidth = type;
            
            if (ioctl(u32MEMDrvFD, AESS_MEMDRV_WRITE, (UINT32) &sMemDrvInfo))
            {
                printf("ioctl(AESS_MEMDRV_WRITE) error\n");
                return -1;
            }
            
            if ((function == 'w') || (function == 'W'))
            {
                switch (type)
                {
                    case 0:
                        printf("0x%08lx <- %02x\n\n", address, (UINT8) data);
                        break;
                        
                    case 1:
                        printf("0x%08lx <- %04x\n\n", address, (UINT16) data);
                        break;
                        
                    case 2:
                        printf("0x%08lx <- %08lx\n\n", address, (UINT32) data);
                        break;
                }
                
                return 0;
            }
            
            sMemDrvInfo.u16Offset = 0;
            
            switch (type)
            {
                case 0:
                    sMemDrvInfo.pDataPtr = (void *) &G_u8ReadBuf[0];
                    break;
                case 1:
                    sMemDrvInfo.pDataPtr = (void *) &G_u16ReadBuf[0];
                    break;
                case 2:
                    sMemDrvInfo.pDataPtr = (void *) &G_u32ReadBuf[0];
                    break;
            }
            
            sMemDrvInfo.u16DataSize = 16 >> type;
            sMemDrvInfo.u8DataWidth = type;
            
            if (ioctl(u32MEMDrvFD, AESS_MEMDRV_READ, (UINT32) &sMemDrvInfo))
            {
                printf("ioctl(AESS_MEMDRV_READ) error\n");
                return -1;
            }
            
            if (ioctl(u32MEMDrvFD, AESS_MEMDRV_RELEASE, (UINT32) &sMemDrvInfo))
            {
                printf("ioctl(AESS_MEMDRV_RELEASE) error\n");
                return -1;
            }
            
            switch (type)
            {
                case 0:
                    
                    printf("\n              0  1  2  3  4  5  6  7 :  8  9  A  B  C  D  E  F\n");
                    printf("--------------------------------------------------------------\n");
                    printf("0x%08lx = ", (address & 0xFFFFFFF0));
                    
                    for (i = 0; i < 16; i++)
                    {
                        printf("%02x ", G_u8ReadBuf[i]);
                        
                        if (i == 7)
                        {
                            printf(": ");
                        }
                    }
                    
                    printf("\n            ");
                    
                    for (i = 0; i < (address & 0x0F); i++)
                    {
                        printf("   ");
                        
                        if (i == 7)
                        {
                            printf("  ");
                        }
                    }
                    
                    printf(" ^\n\n");
                    
                    break;
                    
                case 1:
                    
                    printf("\n                0    2    4    6 :    8    A    C    E\n");
                    printf("------------------------------------------------------\n");
                    printf("0x%08lx = ", (address & 0xFFFFFFF0));
                    
                    for (i = 0; i < 8; i++)
                    {
                        printf("%04x ", G_u16ReadBuf[i]);
                        
                        if (i == 3)
                        {
                            printf(": ");
                        }
                    }
                    
                    printf("\n            ");
                    
                    for (i = 0; i < ((address & 0x0F) >> type); i++)
                    {
                        printf("     ");
                        
                        if (i == 3)
                        {
                            printf("  ");
                        }
                    }
                    
                    printf(" ^\n\n");
                    
                    break;
                    
                case 2:
                    
                    printf("\n                    0        4 :        8        C\n");
                    printf("--------------------------------------------------\n");
                    printf("0x%08lx = ", (address & 0xFFFFFFF0));
                    
                    for (i = 0; i < 4; i++)
                    {
                        printf("%08lx ", G_u32ReadBuf[i]);
                        
                        if (i == 1)
                        {
                            printf(": ");
                        }
                    }
                    
                    printf("\n            ");
                    
                    for (i = 0; i < ((address & 0x0F) >> type); i++)
                    {
                        printf("         ");
                        
                        if (i == 1)
                        {
                            printf("  ");
                        }
                    }
                    
                    printf(" ^\n\n");
                    
                    break;
            }
            
            break;
            
        default:
            
            goto help;
            
            break;
    }
    
    return 0; 
}



/*
void MEM_READ( unsigned long Addr , unsigned char* data , int len )
{
    UINT32 u32MEMDrvFD;
    char d_path[64] ="/dev/aess_memdrv";
    sMemDrvInfoType sMemDrvInfo;
    
    u32MEMDrvFD = open(d_path, O_RDWR, 0);

    sMemDrvInfo.u32BaseAddr = Addr ;
    sMemDrvInfo.u16RegionSize = len;
    
    if (ioctl(u32MEMDrvFD, AESS_MEMDRV_REQUEST, (UINT32) &sMemDrvInfo))
    {
        printf("ioctl(AESS_MEMDRV_REQUEST) error\n");
    }
    sMemDrvInfo.u16Offset = 0;
    sMemDrvInfo.pDataPtr = (void *) data;
    sMemDrvInfo.u16DataSize = len;
    sMemDrvInfo.u8DataWidth = 0;
    
    if (ioctl(u32MEMDrvFD, AESS_MEMDRV_READ, (UINT32) &sMemDrvInfo))
    {
        printf("ioctl(AESS_MEMDRV_READ) error\n");
    }
    
    if (ioctl(u32MEMDrvFD, AESS_MEMDRV_RELEASE, (UINT32) &sMemDrvInfo))
    {
        printf("ioctl(AESS_MEMDRV_RELEASE) error\n");
    }
} 
*/



