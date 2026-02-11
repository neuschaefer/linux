/*
 * uncompress.c
 *
 * (C) Copyright 1999 Linus Torvalds
 *
 * cramfs interfaces to the uncompression library. There's really just
 * three entrypoints:
 *
 *  - cramfs_uncompress_init() - called to initialize the thing.
 *  - cramfs_uncompress_exit() - tell me when you're done
 *  - cramfs_uncompress_block() - uncompress a block.
 *
 * NOTE NOTE NOTE! The uncompression is entirely single-threaded. We
 * only have one stream, and we'll initialize it only once even if it
 * then is used by multiple filesystems.
 */

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/vmalloc.h>
#include <linux/zlib.h>
#include <linux/cramfs_fs.h>

static z_stream stream;
static int initialized;

/* Returns length of decompressed data. */
int cramfs_uncompress_block(void *dst, int dstlen, void **src, int *srclen)
{
	int err;
	int i;

	stream.next_out = dst;
	stream.avail_out = dstlen;

	err = zlib_inflateReset(&stream);
	if (err != Z_OK) {
		printk("zlib_inflateReset error %d\n", err);
		zlib_inflateEnd(&stream);
		zlib_inflateInit(&stream);
	}

	for (i=0;srclen[i];i++) {
		stream.next_in = src[i];
		stream.avail_in = srclen[i];
		err = zlib_inflate(&stream, Z_NO_FLUSH);
		if (err == Z_STREAM_END) {
			if (srclen[i+1])
				goto err;
			break;
		}
		if (err != Z_OK)
			goto err;
	}

	if (err != Z_STREAM_END)
		goto err;
	return stream.total_out;

err:
	printk("Error %d while decompressing!\n", err);
	printk("->%p(%d)\n", dst, dstlen);
	return -EIO;
}

int cramfs_uncompress_init(void)
{
	if (!initialized++) {
		stream.workspace = vmalloc(zlib_inflate_workspacesize());
		if ( !stream.workspace ) {
			initialized = 0;
			return -ENOMEM;
		}
		stream.next_in = NULL;
		stream.avail_in = 0;
		zlib_inflateInit(&stream);
	}
	return 0;
}

void cramfs_uncompress_exit(void)
{
	if (!--initialized) {
		zlib_inflateEnd(&stream);
		vfree(stream.workspace);
	}
}
