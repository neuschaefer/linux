
#include "x_os.h"
#include "x_printf.h"
#include "zip.h"
#include "jffs2_int.h"

#define JFFS2_DECOMP_TEMP (void*)0x1000000
#define JFFS2_DECOMP_TEMP_SIZE 0x100000
#define LOG(lvl, msg...)

#define malloc   x_mem_alloc
#define realloc  x_mem_realloc

typedef struct JFFS2_INODE
{
    UINT32 version;

    struct jffs2_raw_inode *raw;
} JFFS2_INODE_T;

typedef struct JFFS2_INODE_INFO
{
    UINT32 ino;

    UINT32 u4InodeNum, u4InodeAlloc;
    JFFS2_INODE_T *pInodesArray;     // The Array is sorted by descent version order.
} JFFS2_INODE_INFO_T;

typedef struct JFFS2_DIRENT
{
    UINT32 version;

    struct jffs2_raw_dirent *raw;
} JFFS2_DIRENT_T;

typedef struct JFFS2_DIR
{
    UINT32 ino;

    UINT32 u4DirentNum, u4DirentAlloc;
    JFFS2_DIRENT_T *pDirentsArray;     // The Array is sorted by descent version order.
} JFFS2_DIR_T;

typedef struct JFFS2_ROOT
{
    JFFS2_DIR_T *root;

    UINT32 u4InodeInfoNum, u4InodeInfoAlloc;
    JFFS2_INODE_INFO_T **ppInodeInfosArray;

    UINT32 u4DirNum, u4DirAlloc;
    JFFS2_DIR_T **ppDirsArray;
} JFFS2_ROOT_T;

struct JFFS2_RANGE
{
    UINT32 start, end;
};

struct JFFS2_RANGE_SET
{
    UINT32 u4RangeNum, u4RangeAlloc;
    struct JFFS2_RANGE *aRangesArray;
};



/// Read from JFFS2
/// The function will read from undelying device and return address of the data.
static UINT8* JFFS2_IO_Read_MMAP(void *IOHandle, UINT32 u4Offset, UINT32 u4Size)
{
    return ((UINT8*)IOHandle)+u4Offset;
}


#define ALIGN4(a)        ((a+3)&(~3))

#define JFFS2_IO_Read    JFFS2_IO_Read_MMAP

#define JFFS2_INODE_INFO_ALLOC_SIZE 64
#define JFFS2_INODE_ALLOC_SIZE 128
#define JFFS2_DIR_ALLOC_SIZE 32
#define JFFS2_DIRENT_ALLOC_SIZE 32
#define JFFS2_RANGE_ALLOC_SIZE 32

static JFFS2_INODE_INFO_T* JFFS2_FindInode(JFFS2_ROOT_T *pFsRoot, UINT32 ino)
{
    UINT32 i;

    for (i=0; i<pFsRoot->u4InodeInfoNum; i++)
    {
        if (pFsRoot->ppInodeInfosArray[i]->ino == ino)
        {
            return pFsRoot->ppInodeInfosArray[i];
        }
    }

    return 0;
}

static int JFFS2_AddInode(JFFS2_ROOT_T *pFsRoot, struct jffs2_raw_inode *raw)
{
    UINT32 ino, i, version;
    JFFS2_INODE_INFO_T *inode = 0;
    // Cache last inode info to save inode search time.
    static JFFS2_ROOT_T *pLastFsRoot = 0;
    static JFFS2_INODE_INFO_T *pLastInodeInfo = 0;

    LOG(2, "JFFS2_AddInode ino %d ver %d off %d dsize %d\n", raw->ino.v32, raw->version.v32, raw->offset.v32, raw->dsize.v32);

    // Find the inode, if not exists, create it.
    ino = raw->ino.v32;

    if (pLastFsRoot == pFsRoot && pLastInodeInfo && pLastInodeInfo->ino == ino)
    {
        // Cache hit!
        inode = pLastInodeInfo;
    }
    else
    {
        inode = JFFS2_FindInode(pFsRoot, ino);

        if (!inode)
        {
            // Not found, create a new one.
            inode = (JFFS2_INODE_INFO_T*)malloc(sizeof(JFFS2_INODE_INFO_T));
            inode->ino = ino;
            inode->u4InodeNum = inode->u4InodeAlloc = 0;
            inode->pInodesArray = 0;

            if (pFsRoot->u4InodeInfoNum == pFsRoot->u4InodeInfoAlloc)
            {
                pFsRoot->u4InodeInfoAlloc += JFFS2_INODE_INFO_ALLOC_SIZE;
                pFsRoot->ppInodeInfosArray = realloc(pFsRoot->ppInodeInfosArray, pFsRoot->u4InodeInfoAlloc*sizeof(JFFS2_INODE_T*));
            }

            pFsRoot->ppInodeInfosArray[pFsRoot->u4InodeInfoNum++] = inode;
        }

        pLastFsRoot = pFsRoot;
        pLastInodeInfo = inode;
    }

    // Insert raw inode
    if (inode->u4InodeNum == inode->u4InodeAlloc)
    {
        inode->u4InodeAlloc += JFFS2_INODE_ALLOC_SIZE;
        inode->pInodesArray = realloc(inode->pInodesArray, inode->u4InodeAlloc*sizeof(JFFS2_INODE_T));
    }

    version = raw->version.v32;
    for (i=0; i<inode->u4InodeNum; i++)
    {
        if (version > inode->pInodesArray[i].version)
        {
            // Insert befor i.
            memmove(&inode->pInodesArray[i+1], &inode->pInodesArray[i], (inode->u4InodeNum-i)*sizeof(JFFS2_INODE_T));
            inode->pInodesArray[i].raw = raw;
            inode->pInodesArray[i].version = version;
            inode->u4InodeNum++;
            break;
        }
    }

    if (i==inode->u4InodeNum)
    {
        // Add to last one.
        inode->pInodesArray[i].raw = raw;
        inode->pInodesArray[i].version = version;
        inode->u4InodeNum++;
    }

    return 0;
}

static JFFS2_DIR_T* JFFS2_FindDirent(JFFS2_ROOT_T *pFsRoot, UINT32 ino)
{
    UINT32 i;

    for (i=0; i<pFsRoot->u4DirNum; i++)
    {
        if (pFsRoot->ppDirsArray[i]->ino == ino)
        {
            return pFsRoot->ppDirsArray[i];
        }
    }

    return 0;
}

static JFFS2_DIRENT_T *JFFS2_DirFindName(JFFS2_DIR_T *dir, char *name, int len)
{
    UINT32 i;
    
    for (i=0; i<dir->u4DirentNum; i++)
    {
        if (dir->pDirentsArray[i].raw->nsize != len)
            continue;
        
        if (!strncmp((char*)dir->pDirentsArray[i].raw->name, name, len))
            return &dir->pDirentsArray[i];
    }

    return 0;
}
        
static int JFFS2_AddDirent(JFFS2_ROOT_T *pFsRoot, struct jffs2_raw_dirent *raw)
{
    UINT32 ino, i, version;
    JFFS2_DIR_T *dir;

    LOG(2, "JFFS2_AddDirent pino %d ino %d ver %d name %s\n", raw->pino.v32, raw->ino.v32, raw->version.v32, raw->name);

    // Find the parent inode, if not exists, create it.
    ino = raw->pino.v32;

    dir = JFFS2_FindDirent(pFsRoot, ino);

    if (!dir)
    {
        // Not found, create a new one.
        dir = (JFFS2_DIR_T*)malloc(sizeof(JFFS2_DIR_T));
        dir->ino = ino;
        dir->u4DirentNum = dir->u4DirentAlloc = 0;
        dir->pDirentsArray = 0;

        if (pFsRoot->u4DirNum == pFsRoot->u4DirAlloc)
        {
            pFsRoot->u4DirAlloc += JFFS2_INODE_INFO_ALLOC_SIZE;
            pFsRoot->ppDirsArray = realloc(pFsRoot->ppDirsArray, pFsRoot->u4DirAlloc*sizeof(JFFS2_DIR_T*));
        }

        pFsRoot->ppDirsArray[pFsRoot->u4DirNum++] = dir;
    }

    // Insert raw dirent
    if (dir->u4DirentNum == dir->u4DirentAlloc)
    {
        dir->u4DirentAlloc += JFFS2_DIRENT_ALLOC_SIZE;
        dir->pDirentsArray = realloc(dir->pDirentsArray, dir->u4DirentAlloc*sizeof(JFFS2_INODE_T));
    }

    version = raw->version.v32;
    for (i=0; i<dir->u4DirentNum; i++)
    {
        if (version > dir->pDirentsArray[i].version)
        {
            // Insert befor i.
            memmove(&dir->pDirentsArray[i+1], &dir->pDirentsArray[i], (dir->u4DirentNum-i)*sizeof(JFFS2_INODE_T));
            dir->pDirentsArray[i].raw = raw;
            dir->pDirentsArray[i].version = version;
            dir->u4DirentNum++;
            break;
        }
    }

    if (i==dir->u4DirentNum)
    {
        // Add to last one.
        dir->pDirentsArray[i].raw = raw;
        dir->pDirentsArray[i].version = version;
        dir->u4DirentNum++;
    }

    return 0;
}

static int JFFS2_InRage(struct JFFS2_RANGE_SET *prs, UINT32 start, UINT32 end)
{
    UINT32 i, istart, iend, rstart, rend;
    
    for (i=0; i<prs->u4RangeNum; i++)
    {
        istart = prs->aRangesArray[i].start;
        iend = prs->aRangesArray[i].end;
        rstart = start;
        rend = end;

        if (istart > rstart)
            rstart = istart;

        if (iend < rend)
            rend = iend;
            
        if (rend > rstart)
            return 1;
    }

    return 0;
}

static int JFFS2_ReadData(JFFS2_INODE_INFO_T *inode, char *buf, int offset, int size)
{
    long dlen;
    int ret;
    UINT32 i, j, ioff, dsize, zero, istart, iend, rstart, rend;
    struct jffs2_raw_inode *raw;
    struct JFFS2_RANGE_SET rs;
    char *data;

    // Init read range set.    
    rs.u4RangeAlloc = JFFS2_RANGE_ALLOC_SIZE;
    rs.aRangesArray = malloc(sizeof(rs.aRangesArray[0])*rs.u4RangeAlloc);
    rs.aRangesArray[0].start = offset;
    rs.aRangesArray[0].end = offset + size;
    rs.u4RangeNum = 1;

    for (i=0; i<inode->u4InodeNum; i++)
    {
        raw = inode->pInodesArray[i].raw;
        ioff = raw->offset.v32;
        dsize = raw->dsize.v32;

        // Check if in read range.
        if (!JFFS2_InRage(&rs, ioff, ioff+dsize))
            continue;

        // Get the data.
        data = (char*)raw->data;
        zero = (raw->compr == JFFS2_COMPR_ZERO);
        if (raw->compr == JFFS2_COMPR_ZLIB)
        {
            dlen = JFFS2_DECOMP_TEMP_SIZE;
            ret = ZIP_Decompress((UINT32)data, raw->csize.v32, (UINT32)JFFS2_DECOMP_TEMP, &dlen);
            if (ret != Z_OK && ret != Z_STREAM_END)
                LOG(0, "Uncompress error off %d ret %d\n", ioff, ret);
            data = JFFS2_DECOMP_TEMP;
        }

        for (j=0; j<rs.u4RangeNum; j++)
        {
            rstart = ioff;
            rend = ioff + dsize;
            istart = rs.aRangesArray[j].start;
            iend = rs.aRangesArray[j].end;

            if (istart > rstart)
                rstart = istart;

            if (iend < rend)
                rend = iend;

            if (rend <= rstart)
                continue;

            if (zero)
                memset(buf + rstart - offset, 0, rend - rstart);
            else
                memcpy(buf + rstart - offset, data + rstart - ioff, rend - rstart);

            // Subtrace [rstart,rend) from the set.
            if (rstart > istart)
                istart = rstart;

            if (rend < iend)
                iend = rend;

            if (istart == rs.aRangesArray[j].start)
            {
                if (iend == rs.aRangesArray[j].end)
                {
                    // Good, remove this range.
                    rs.u4RangeNum--;
                    memcpy(&rs.aRangesArray[j], &rs.aRangesArray[j+1], (rs.u4RangeNum-j)*sizeof(rs.aRangesArray[0]));
                    j--;
                }
                else
                {
                    rs.aRangesArray[j].start = iend;
                }
            }
            else
            {
                if (iend == rs.aRangesArray[j].end)
                {
                    rs.aRangesArray[j].end = istart;
                }
                else
                {
                    // Change to 2 range.
                    if (rs.u4RangeNum == rs.u4RangeAlloc)
                    {
                        rs.u4RangeAlloc+=JFFS2_RANGE_ALLOC_SIZE;
                        rs.aRangesArray = realloc(rs.aRangesArray, rs.u4RangeAlloc*sizeof(rs.aRangesArray[0]));
                    }
                    rs.aRangesArray[rs.u4RangeNum].start = iend;
                    rs.aRangesArray[rs.u4RangeNum++].end = rs.aRangesArray[j].end;
                    rs.aRangesArray[j].end = istart;
                }
            }
        }
    }
    
    // Fill all the rest with 0.
    for (i=0; i<rs.u4RangeNum; i++)
    {
        rstart = rs.aRangesArray[i].start;
        rend = rs.aRangesArray[i].end;
        memset(buf + rstart - offset, 0, rend - rstart);
    }
    return 0;
}

static int JFFS2_ScanBlock(JFFS2_ROOT_T *pFsRoot, UINT32 u4Offset, UINT32 u4BlockSize, void *IOHandle)
{
    int off = 0;
    struct jffs2_unknown_node *node;

    // Check cleanmark. Skip the block if it doesn't exist.
    node = (struct jffs2_unknown_node*)JFFS2_IO_Read(IOHandle, u4Offset + off, sizeof(struct jffs2_unknown_node));
    if (node->magic.v16 != JFFS2_MAGIC_BITMASK && node->nodetype.v16 != JFFS2_NODETYPE_CLEANMARKER)
    {
        LOG(1, "Ignore block without cleanmark at 0x%x\n", u4Offset);
        return 0;
    }

    LOG(2, "ScanBlock at 0x%x\n", u4Offset);

    off += ALIGN4(node->totlen.v32);
    while (off < u4BlockSize)
    {
        // Read header
        node = (struct jffs2_unknown_node*)JFFS2_IO_Read(IOHandle, u4Offset + off, sizeof(struct jffs2_unknown_node));
        if (node->magic.v16 != JFFS2_MAGIC_BITMASK)
        {
            LOG(2, "No valid magic at 0x%x, val 0x%x\n", u4Offset+off, node->magic.v16);
            return 0;
        }

        if (node->totlen.v32 <= 0 || node->totlen.v32 > u4BlockSize)
        {
            LOG(1, "Invalid node size at 0x%x, val 0x%x, len %u\n", u4Offset+off, node->nodetype.v16, node->totlen.v32);
            return 0;
        }

        switch (node->nodetype.v16)
        {
            case JFFS2_NODETYPE_DIRENT:
                // Add the dirent
                JFFS2_AddDirent(pFsRoot, (struct jffs2_raw_dirent*)JFFS2_IO_Read(IOHandle, u4Offset + off, node->totlen.v32));
                break;

            case JFFS2_NODETYPE_INODE:
                // Add the node.
                JFFS2_AddInode(pFsRoot, (struct jffs2_raw_inode*)JFFS2_IO_Read(IOHandle, u4Offset + off, node->totlen.v32));
                break;

            case JFFS2_NODETYPE_PADDING:
                // Padding, just skip it.
                break;

            // Unknow node. Just skip it.
            case JFFS2_NODETYPE_CLEANMARKER:
            default:
                LOG(1, "Unknown node at 0x%x, val 0x%x\n", u4Offset+off, node->nodetype.v16);
                break;
        }
        off += ALIGN4(node->totlen.v32);
    }

    return 0;
}

int JFFS2_Mount(JFFS2_ROOT_T **ppFsRoot, UINT32 u4BlockSize, UINT32 u4BlockNum, void *IOHandle)
{
    int i, ret;
    JFFS2_ROOT_T *pFsRoot;

    pFsRoot = (JFFS2_ROOT_T*)malloc(sizeof(JFFS2_ROOT_T));
    memset(pFsRoot, 0, sizeof(JFFS2_ROOT_T));

    for (i=0; i<u4BlockNum; i++)
    {
        // Scan block.
        ret = JFFS2_ScanBlock(pFsRoot, i*u4BlockSize, u4BlockSize, IOHandle);
    }

    pFsRoot->root = JFFS2_FindDirent(pFsRoot, 1);

    *ppFsRoot = pFsRoot;
    return 0;
}

int JFFS2_Read(JFFS2_ROOT_T *pFsRoot, char *path, char **pBuf, UINT32 *pSize)
{
    JFFS2_DIR_T *dir = pFsRoot->root;
    JFFS2_DIRENT_T *dirent;
    JFFS2_INODE_INFO_T *inode;
    char *name, *tmp, *buf;
    int len, type, size;

    name = path;
    while (1)
    {
        if (!dir)
            return -1;

        while (*name == '/' || (*name == '.' && *(name+1) == '/')) name++;
        tmp = strchr(name, '/');
        if (tmp)
            len = tmp - name;
        else
            len = strlen(name);

        dirent = JFFS2_DirFindName(dir, name, len);
        if (!dirent)
            return -1;

        name = name + len;
        if (!*name)
            break;

        if (!dirent->raw->ino.v32)
            return -1;

        type = dirent->raw->type;
        if (type != DT_DIR && type != DT_LNK)
            return -1;

        if (type == DT_DIR)
        {
           dir = JFFS2_FindDirent(pFsRoot, dirent->raw->ino.v32);
        }
        else
        {
           // Sym link, not support yet.
           return -1;
        }
    }

    // File found. Read content.
    if (dirent->raw->type != DT_REG)
        return -1;

    if (!dirent->raw->ino.v32)
        return -1;

    inode = JFFS2_FindInode(pFsRoot, dirent->raw->ino.v32);
    if (!inode)
        return -1;

    if (!inode->u4InodeNum)
        return -1;

    size = inode->pInodesArray[0].raw->isize.v32;
    buf = malloc(size+1);
    *pBuf = buf;
    *pSize = size;
    return JFFS2_ReadData(inode, buf, 0, size);
}

