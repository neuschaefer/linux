
#ifndef __JFFS2_H__
#define __JFFS2_H__

typedef struct JFFS2_ROOT JFFS2_ROOT_T;

int JFFS2_Mount(JFFS2_ROOT_T **ppFsRoot, UINT32 u4BlockSize, UINT32 u4BlockNum, void *IOHandle);
int JFFS2_Read(JFFS2_ROOT_T *pFsRoot, char *path, char **pBuf, UINT32 *pSize);

#endif /* __JFFS2_H__ */

