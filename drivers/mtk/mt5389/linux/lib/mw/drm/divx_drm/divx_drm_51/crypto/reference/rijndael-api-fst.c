/**
 * rijndael-api-fst.c
 *
 * @version 2.9 (December 2000)
 *
 * Optimised ANSI C code for the Rijndael cipher (now AES)
 *
 * @author Vincent Rijmen <vincent.rijmen@esat.kuleuven.ac.be>
 * @author Antoon Bosselaers <antoon.bosselaers@esat.kuleuven.ac.be>
 * @author Paulo Barreto <paulo.barreto@terra.com.br>
 *
 * This code is hereby placed in the public domain.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ''AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Acknowledgements:
 *
 * We are deeply indebted to the following people for their bug reports,
 * fixes, and improvement suggestions to this implementation. Though we
 * tried to list all contributions, we apologise in advance for any
 * missing reference.
 *
 * Andrew Bales <Andrew.Bales@Honeywell.com>
 * Markus Friedl <markus.friedl@informatik.uni-erlangen.de>
 * John Skodon <skodonj@webquill.com>
 */

#ifndef __KERNEL__

#ifdef _MTK_BUILD_
#include "libc/memory.h"
#include "libc/stdlib.h"
#include "libc/string.h"
#else
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#endif


#include "rijndael-api-fst.h"

#include "rijndael-alg-fst.h"

#else

#include "rijndael-api-fst.h"

#include "rijndael-alg-fst.h"

#include "../../../divx_drm_adp_common.h" /* NULL */

#endif

int aesMakeKey(aesKeyInstance *key, BYTE direction, int keyLen, char *keyMaterial) {

//	int i;
//	char *keyMat;
	u8 cipherKey[MAXKB];
	
	if (key == NULL) {
		return BAD_KEY_INSTANCE;
	}

	if ((direction == DIR_ENCRYPT) || (direction == DIR_DECRYPT)) {
		key->direction = direction;
	} else {
		return BAD_KEY_DIR;
	}

	if ((keyLen == 128) || (keyLen == 192) || (keyLen == 256)) {
		key->keyLen = keyLen;
	} else {
		return BAD_KEY_MAT;
	}

	if (keyMaterial != NULL) {
		memcpy(key->keyMaterial, keyMaterial, keyLen/8);
	}

	/* initialize key schedule: */
    memcpy ( cipherKey,
             key->keyMaterial,
             key->keyLen/8 );
   key->Nr_enc = divxRijndaelKeySetupEnc(key->rk_enc, cipherKey, keyLen);
    memcpy ( cipherKey,
             key->keyMaterial,
             key->keyLen/8 );
   key->Nr_dec = divxRijndaelKeySetupDec(key->rk_dec, cipherKey, keyLen);
	return TRUE;
}

int aesUnmakeKey( aesKeyInstance *key )
{
    return 0;
}

int aesCipherInit(aesCipherInstance *cipher, BYTE mode, char *IV) {
	if ((mode == MODE_ECB) || (mode == MODE_CBC) || (mode == MODE_CFB1)) {
		cipher->mode = mode;
	} else {
		return BAD_CIPHER_MODE;
	}
	memset(cipher->IV, 0, MAX_IV_SIZE);
	return TRUE;
}

int aesCipherFinalize(aesCipherInstance *cipher)
{
    memset(cipher->IV, 0, MAX_IV_SIZE);
    return TRUE;
}

int aesBlockEncrypt(aesCipherInstance *cipher, aesKeyInstance *key,
		const BYTE *input, const int inputLen, BYTE *outBuffer) {
	int i, numBlocks;

	if (cipher == NULL ||
		key == NULL ||
		key->direction == DIR_DECRYPT) {
		return BAD_CIPHER_STATE;
	}
	if (input == NULL || inputLen <= 0) {
		return 0; /* nothing to do */
	}

	numBlocks = inputLen/128;
	
	switch (cipher->mode) {
	case MODE_ECB:
		for (i = numBlocks; i > 0; i--) {
			divxRijndaelEncrypt(key->rk_enc, key->Nr_enc, input, outBuffer);
			input += 16;
			outBuffer += 16;
		}
		break;
	default:
		return BAD_CIPHER_STATE;
	}
	
	return 128*numBlocks;
}

int aesBlockDecrypt(aesCipherInstance *cipher, aesKeyInstance *key,
		BYTE *input, int inputLen, BYTE *outBuffer) {
	int i, numBlocks;

	if (cipher == NULL ||
		key == NULL ||
		cipher->mode != MODE_CFB1 && key->direction == DIR_ENCRYPT) {
		return BAD_CIPHER_STATE;
	}
	if (input == NULL || inputLen <= 0) {
		return 0; /* nothing to do */
	}

	numBlocks = inputLen/128;

	switch (cipher->mode) {
	case MODE_ECB:
		for (i = numBlocks; i > 0; i--) {
			divxRijndaelDecrypt(key->rk_dec, key->Nr_dec, input, outBuffer);
			input += 16;
			outBuffer += 16;
		}
		break;		
	default:
		return BAD_CIPHER_STATE;
	}
	
	return 128*numBlocks;
}


