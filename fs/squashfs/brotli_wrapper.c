#include <linux/mutex.h>
#include <linux/buffer_head.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/brotli/decode.h>

#include "squashfs_fs.h"
#include "squashfs_fs_sb.h"
#include "squashfs_fs_i.h"
#include "squashfs.h"
#include "decompressor.h"
#include "page_actor.h"

static void *brotli_init(struct squashfs_sb_info *msblk, void *buff)
{
    BrotliDecoderState* state = kmalloc(BrotliDecoderStateSize(), GFP_KERNEL);
    if (state == NULL)
        return ERR_PTR(-ENOMEM);
    return state;
}

static void brotli_free(void *state)
{
    kfree(state);
}

static int brotli_decompress(struct squashfs_sb_info *msblk,  void *strm, 
	struct buffer_head **bh, int b, int offset, int length,
	struct squashfs_page_actor *output)
{
    BrotliDecoderState *state = strm; /*msblk->stream;*/
    size_t avail_in = 0;
    size_t avail_out = PAGE_CACHE_SIZE;
    size_t total_out = 0;
    int k = 0;
    const uint8_t* next_in = NULL;
    uint8_t* next_out = squashfs_first_page(output);
    BrotliDecoderResult res = BROTLI_DECODER_RESULT_SUCCESS;

    BrotliDecoderStateInit(state);

    do {
        if (avail_in == 0 && k < b) {
            int avail = min(length, msblk->devblksize - offset);
            length -= avail;
            next_in = bh[k]->b_data + offset;
            avail_in = avail;
            offset = 0;
        }
	if (avail_out == 0) {
		next_out = squashfs_next_page(output);
		if (next_out != NULL)
			avail_out = PAGE_CACHE_SIZE;
	}
        res = BrotliDecoderDecompressStream(state, 
                &avail_in, &next_in, &avail_out, &next_out, &total_out);

        if (avail_in == 0 && k < b)
            put_bh(bh[k++]);
    } while (res == BROTLI_DECODER_RESULT_NEEDS_MORE_INPUT ||
             res == BROTLI_DECODER_RESULT_NEEDS_MORE_OUTPUT);

    squashfs_finish_page(output);

    if (res != BROTLI_DECODER_RESULT_SUCCESS) {
        ERROR("brotli error %d (code %d), data probably corrupt\n", (int)res, (int)BrotliDecoderGetErrorCode(state));
        goto error;
    }
    if (k < b) {
        ERROR("brotli error, data remaining (%d < %d)\n", k, b);
        goto error;
    }
    BrotliDecoderStateCleanup(state);
    return total_out;

error:
    for (; k < b; k++)
        put_bh(bh[k]);
    return -EIO;
}

const struct squashfs_decompressor squashfs_brotli_comp_ops = {
    .init = brotli_init,
    .free = brotli_free,
    .decompress = brotli_decompress,
    .id = BROTLI_COMPRESSION,
    .name = "brotli",
    .supported = 1
};
