##############################################################################
# No Warranty                                                                #
# Except as may be otherwise agreed to in writing, no warranties of any      #
# kind, whether express or implied, are given by MTK with respect to any MTK #
# Deliverables or any use thereof, and MTK Deliverables are provided on an   #
# "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        #
# including any implied warranties of merchantability, non-infringement and  #
# fitness for a particular purpose and any warranties arising out of course  #
# of performance, course of dealing or usage of trade.  Parties further      #
# acknowledge that Company may, either presently and/or in the future,       #
# instruct MTK to assist it in the development and the implementation, in    #
# accordance with Company's designs, of certain softwares relating to        #
# Company's product(s) (the "Services").  Except as may be otherwise agreed  #
# to in writing, no warranties of any kind, whether express or implied, are  #
# given by MTK with respect to the Services provided, and the Services are   #
# provided on an "AS IS" basis.  Company further acknowledges that the       #
# Services may contain errors, that testing is important and Company is      #
# solely responsible for fully testing the Services and/or derivatives       #
# thereof before they are used, sublicensed or distributed.  Should there be #
# any third party action brought against MTK, arising out of or relating to  #
# the Services, Company agree to fully indemnify and hold MTK harmless.      #
# If the parties mutually agree to enter into or continue a business         #
# relationship or other arrangement, the terms and conditions set forth      #
# hereunder shall remain effective and, unless explicitly stated otherwise,  #
# shall prevail in the event of a conflict in the terms in any agreements    #
# entered into between the parties.                                          #
##############################################################################
###########################################################################
#
#  Copyright (c) 2004, MediaTek, Inc.
#  All rights reserved
#
#  Unauthorized use, practice, perform, copy, distribution, reproduction,
#  or disclosure of this information in whole or in part is prohibited.
###########################################################################
# $RCSfile: Makefile,v $
# $Revision: #1 $
# $Date: 2009/02/23 $
# $Author: eddiehuang $
#
# Description:
#         Leave-level makefile to build the subcomponent of driver library.
#
#         Specify the source files to be compile in SRC.
#############################################################################

THIS_COMPONENT = mtk_skb

ifdef LINUX_DRV_ROOT
export DRV_ROOT = $(LINUX_DRV_ROOT)
else
export DRV_ROOT = ../../..
endif

SRC = SkbGenericCipher.c SkbGenericEngine.c SkbGenericSecureData.c SkbGenericTransform.c tomcrypt_asn1_missing.c mtk_util.c
SRC += ./libtomcrypt-1.16/src/ciphers/aes/aes.c
SRC += ./libtomcrypt-1.16/src/hashes/rmd128.c
SRC += ./libtomcrypt-1.16/src/hashes/rmd256.c
SRC += ./libtomcrypt-1.16/src/hashes/md2.c
SRC += ./libtomcrypt-1.16/src/hashes/md4.c
SRC += ./libtomcrypt-1.16/src/hashes/rmd160.c
SRC += ./libtomcrypt-1.16/src/hashes/md5.c
SRC += ./libtomcrypt-1.16/src/hashes/rmd320.c
SRC += ./libtomcrypt-1.16/src/hashes/sha1.c
SRC += ./libtomcrypt-1.16/src/hashes/tiger.c
SRC += ./libtomcrypt-1.16/src/hashes/sha2/sha512.c
SRC += ./libtomcrypt-1.16/src/hashes/sha2/sha256.c
SRC += ./libtomcrypt-1.16/src/mac/hmac/hmac_process.c
SRC += ./libtomcrypt-1.16/src/mac/hmac/hmac_done.c
SRC += ./libtomcrypt-1.16/src/mac/hmac/hmac_test.c
SRC += ./libtomcrypt-1.16/src/mac/hmac/hmac_memory.c
SRC += ./libtomcrypt-1.16/src/mac/hmac/hmac_file.c
SRC += ./libtomcrypt-1.16/src/mac/hmac/hmac_memory_multi.c
SRC += ./libtomcrypt-1.16/src/mac/hmac/hmac_init.c
SRC += ./libtomcrypt-1.16/src/math/multi.c
SRC += ./libtomcrypt-1.16/src/math/rand_prime.c
SRC += ./libtomcrypt-1.16/src/math/ltm_desc.c
SRC += ./libtomcrypt-1.16/src/math/gmp_desc.c
SRC += ./libtomcrypt-1.16/src/math/tfm_desc.c
SRC += ./libtomcrypt-1.16/src/modes/cbc/cbc_encrypt.c
SRC += ./libtomcrypt-1.16/src/modes/cbc/cbc_getiv.c
SRC += ./libtomcrypt-1.16/src/modes/cbc/cbc_start.c
SRC += ./libtomcrypt-1.16/src/modes/cbc/cbc_decrypt.c
SRC += ./libtomcrypt-1.16/src/modes/cbc/cbc_setiv.c
SRC += ./libtomcrypt-1.16/src/modes/cbc/cbc_done.c
SRC += ./libtomcrypt-1.16/src/modes/ctr/ctr_setiv.c
SRC += ./libtomcrypt-1.16/src/modes/ctr/ctr_encrypt.c
SRC += ./libtomcrypt-1.16/src/modes/ctr/ctr_done.c
SRC += ./libtomcrypt-1.16/src/modes/ctr/ctr_decrypt.c
SRC += ./libtomcrypt-1.16/src/modes/ctr/ctr_start.c
SRC += ./libtomcrypt-1.16/src/modes/ctr/ctr_getiv.c
SRC += ./libtomcrypt-1.16/src/modes/ctr/ctr_test.c
SRC += ./libtomcrypt-1.16/src/modes/ecb/ecb_done.c
SRC += ./libtomcrypt-1.16/src/modes/ecb/ecb_encrypt.c
SRC += ./libtomcrypt-1.16/src/modes/ecb/ecb_start.c
SRC += ./libtomcrypt-1.16/src/modes/ecb/ecb_decrypt.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/bit/der_length_bit_string.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/bit/der_encode_bit_string.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/bit/der_decode_bit_string.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/choice/der_decode_choice.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/ia5/der_decode_ia5_string.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/ia5/der_length_ia5_string.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/ia5/der_encode_ia5_string.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/integer/der_encode_integer.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/integer/der_decode_integer.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/integer/der_length_integer.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/object_identifier/der_decode_object_identifier.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/object_identifier/der_encode_object_identifier.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/object_identifier/der_length_object_identifier.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/octet/der_length_octet_string.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/octet/der_decode_octet_string.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/octet/der_encode_octet_string.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/printable_string/der_length_printable_string.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/printable_string/der_decode_printable_string.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/printable_string/der_encode_printable_string.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/sequence/der_decode_sequence_ex.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/sequence/der_sequence_free.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/sequence/der_length_sequence.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/sequence/der_encode_sequence_multi.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/sequence/der_decode_sequence_flexi.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/sequence/der_decode_sequence_multi.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/sequence/der_sequence_sibling_count.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/sequence/der_encode_sequence_ex.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/set/der_encode_setof.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/set/der_encode_set.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/short_integer/der_decode_short_integer.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/short_integer/der_length_short_integer.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/short_integer/der_encode_short_integer.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/utf8/der_encode_utf8_string.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/utf8/der_decode_utf8_string.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/utf8/der_length_utf8_string.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/boolean/der_encode_boolean.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/boolean/der_length_boolean.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/boolean/der_decode_boolean.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/t61/der_length_t61_string.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/t61/der_encode_t61_string.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/t61/der_decode_t61_string.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/utctime/der_decode_utctime.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/utctime/der_length_utctime.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/utctime/der_encode_utctime.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/generalized_time/der_decode_generalized_time.c
SRC += ./libtomcrypt-1.16/src/pk/asn1/der/generalized_time/der_length_generalized_time.c
SRC += ./libtomcrypt-1.16/src/pk/rsa/rsa_make_key.c
SRC += ./libtomcrypt-1.16/src/pk/rsa/rsa_encrypt_key.c
SRC += ./libtomcrypt-1.16/src/pk/rsa/rsa_exptmod.c
SRC += ./libtomcrypt-1.16/src/pk/rsa/rsa_verify_hash.c
SRC += ./libtomcrypt-1.16/src/pk/rsa/rsa_export.c
SRC += ./libtomcrypt-1.16/src/pk/rsa/rsa_sign_hash.c
SRC += ./libtomcrypt-1.16/src/pk/rsa/rsa_free.c
SRC += ./libtomcrypt-1.16/src/pk/rsa/rsa_import.c
SRC += ./libtomcrypt-1.16/src/pk/rsa/rsa_decrypt_key.c
SRC += ./libtomcrypt-1.16/src/pk/pkcs1/pkcs_1_mgf1.c
SRC += ./libtomcrypt-1.16/src/pk/pkcs1/pkcs_1_os2ip.c
SRC += ./libtomcrypt-1.16/src/pk/pkcs1/pkcs_1_pss_encode.c
SRC += ./libtomcrypt-1.16/src/pk/pkcs1/pkcs_1_oaep_encode.c
SRC += ./libtomcrypt-1.16/src/pk/pkcs1/pkcs_1_i2osp.c
SRC += ./libtomcrypt-1.16/src/pk/pkcs1/pkcs_1_v1_5_encode.c
SRC += ./libtomcrypt-1.16/src/pk/pkcs1/pkcs_1_v1_5_decode.c
SRC += ./libtomcrypt-1.16/src/pk/pkcs1/pkcs_1_oaep_decode.c
SRC += ./libtomcrypt-1.16/src/pk/pkcs1/pkcs_1_pss_decode.c
SRC += ./libtomcrypt-1.16/src/prngs/sober128.c
SRC += ./libtomcrypt-1.16/src/prngs/rng_make_prng.c
SRC += ./libtomcrypt-1.16/src/prngs/rng_get_bytes.c
SRC += ./libtomcrypt-1.16/src/prngs/sprng.c
SRC += ./libtomcrypt-1.16/src/prngs/rc4.c
SRC += ./libtomcrypt-1.16/src/prngs/yarrow.c
SRC += ./libtomcrypt-1.16/src/prngs/fortuna.c
SRC += ./libtomcrypt-1.16/src/misc/zeromem.c
SRC += ./libtomcrypt-1.16/src/misc/error_to_string.c
SRC += ./libtomcrypt-1.16/src/misc/burn_stack.c
SRC += ./libtomcrypt-1.16/src/misc/crypt/crypt_register_prng.c
SRC += ./libtomcrypt-1.16/src/misc/crypt/crypt_find_hash.c
SRC += ./libtomcrypt-1.16/src/misc/crypt/crypt_find_hash_any.c
SRC += ./libtomcrypt-1.16/src/misc/crypt/crypt_hash_is_valid.c
SRC += ./libtomcrypt-1.16/src/misc/crypt/crypt_find_hash_oid.c
SRC += ./libtomcrypt-1.16/src/misc/crypt/crypt_find_cipher_id.c
SRC += ./libtomcrypt-1.16/src/misc/crypt/crypt.c
SRC += ./libtomcrypt-1.16/src/misc/crypt/crypt_register_hash.c
SRC += ./libtomcrypt-1.16/src/misc/crypt/crypt_fsa.c
SRC += ./libtomcrypt-1.16/src/misc/crypt/crypt_find_cipher.c
SRC += ./libtomcrypt-1.16/src/misc/crypt/crypt_cipher_is_valid.c
SRC += ./libtomcrypt-1.16/src/misc/crypt/crypt_register_cipher.c
SRC += ./libtomcrypt-1.16/src/misc/crypt/crypt_unregister_prng.c
SRC += ./libtomcrypt-1.16/src/misc/crypt/crypt_unregister_cipher.c
SRC += ./libtomcrypt-1.16/src/misc/crypt/crypt_prng_descriptor.c
SRC += ./libtomcrypt-1.16/src/misc/crypt/crypt_cipher_descriptor.c
SRC += ./libtomcrypt-1.16/src/misc/crypt/crypt_find_hash_id.c
SRC += ./libtomcrypt-1.16/src/misc/crypt/crypt_find_prng.c
SRC += ./libtomcrypt-1.16/src/misc/crypt/crypt_argchk.c
SRC += ./libtomcrypt-1.16/src/misc/crypt/crypt_hash_descriptor.c
SRC += ./libtomcrypt-1.16/src/misc/crypt/crypt_prng_is_valid.c
SRC += ./libtomcrypt-1.16/src/misc/crypt/crypt_unregister_hash.c
SRC += ./libtomcrypt-1.16/src/misc/crypt/crypt_find_cipher_any.c
SRC += ./libtomcrypt-1.16/src/misc/base64/base64_decode.c
SRC += ./libtomcrypt-1.16/src/misc/base64/base64_encode.c
SRC += ./libtomcrypt-1.16/src/hashes/helper/hash_file.c
SRC += ./libtomcrypt-1.16/src/hashes/helper/hash_filehandle.c
SRC += ./libtomcrypt-1.16/src/hashes/helper/hash_memory_multi.c
SRC += ./libtomcrypt-1.16/src/hashes/helper/hash_memory.c
SRC += ./libtomcrypt-1.16/src/pk/ecc/ltc_ecc_mul2add.c
SRC += ./libtomcrypt-1.16/src/pk/ecc/ecc_sizes.c
SRC += ./libtomcrypt-1.16/src/pk/ecc/ltc_ecc_points.c
SRC += ./libtomcrypt-1.16/src/pk/ecc/ecc_test.c
SRC += ./libtomcrypt-1.16/src/pk/ecc/ecc_decrypt_key.c
SRC += ./libtomcrypt-1.16/src/pk/ecc/ecc_make_key.c
SRC += ./libtomcrypt-1.16/src/pk/ecc/ltc_ecc_projective_dbl_point.c
SRC += ./libtomcrypt-1.16/src/pk/ecc/ecc_get_size.c
SRC += ./libtomcrypt-1.16/src/pk/ecc/ecc_ansi_x963_import.c
SRC += ./libtomcrypt-1.16/src/pk/ecc/ecc_encrypt_key.c
SRC += ./libtomcrypt-1.16/src/pk/ecc/ecc_sign_hash.c
SRC += ./libtomcrypt-1.16/src/pk/ecc/ecc_shared_secret.c
SRC += ./libtomcrypt-1.16/src/pk/ecc/ltc_ecc_map.c
SRC += ./libtomcrypt-1.16/src/pk/ecc/ecc_ansi_x963_export.c
SRC += ./libtomcrypt-1.16/src/pk/ecc/ltc_ecc_mulmod_timing.c
SRC += ./libtomcrypt-1.16/src/pk/ecc/ltc_ecc_projective_add_point.c
SRC += ./libtomcrypt-1.16/src/pk/ecc/ecc_verify_hash.c
SRC += ./libtomcrypt-1.16/src/pk/ecc/ecc_import.c
SRC += ./libtomcrypt-1.16/src/pk/ecc/ecc.c
SRC += ./libtomcrypt-1.16/src/pk/ecc/ecc_free.c
SRC += ./libtomcrypt-1.16/src/pk/ecc/ltc_ecc_mulmod.c
SRC += ./libtomcrypt-1.16/src/pk/ecc/ecc_export.c
SRC += ./libtomcrypt-1.16/src/pk/ecc/ltc_ecc_is_valid_idx.c


ifeq "$(TOM_MATH)" "fast"
SRC += ./tomsfastmath-0.14/src/sqr/fp_sqrmod.c
SRC += ./tomsfastmath-0.14/src/sqr/fp_sqr_comba_48.c
SRC += ./tomsfastmath-0.14/src/sqr/fp_sqr_comba_3.c
SRC += ./tomsfastmath-0.14/src/sqr/fp_sqr_comba_small_set.c
SRC += ./tomsfastmath-0.14/src/sqr/fp_sqr_comba_32.c
SRC += ./tomsfastmath-0.14/src/sqr/fp_sqr_comba_6.c
SRC += ./tomsfastmath-0.14/src/sqr/fp_sqr_comba_7.c
SRC += ./tomsfastmath-0.14/src/sqr/fp_sqr_comba_generic.c
SRC += ./tomsfastmath-0.14/src/sqr/fp_sqr_comba_17.c
SRC += ./tomsfastmath-0.14/src/sqr/fp_sqr_comba_4.c
SRC += ./tomsfastmath-0.14/src/sqr/fp_sqr_comba_24.c
SRC += ./tomsfastmath-0.14/src/sqr/fp_sqr_comba_8.c
SRC += ./tomsfastmath-0.14/src/sqr/fp_sqr_comba.c
SRC += ./tomsfastmath-0.14/src/sqr/fp_sqr_comba_20.c
SRC += ./tomsfastmath-0.14/src/sqr/fp_sqr_comba_28.c
SRC += ./tomsfastmath-0.14/src/sqr/fp_sqr_comba_12.c
SRC += ./tomsfastmath-0.14/src/sqr/fp_sqr_comba_64.c
SRC += ./tomsfastmath-0.14/src/sqr/fp_sqr_comba_9.c
SRC += ./tomsfastmath-0.14/src/sqr/fp_sqr.c
SRC += ./tomsfastmath-0.14/src/numtheory/fp_lcm.c
SRC += ./tomsfastmath-0.14/src/numtheory/fp_gcd.c
SRC += ./tomsfastmath-0.14/src/numtheory/fp_isprime.c
SRC += ./tomsfastmath-0.14/src/numtheory/fp_prime_random_ex.c
SRC += ./tomsfastmath-0.14/src/numtheory/fp_prime_miller_rabin.c
SRC += ./tomsfastmath-0.14/src/numtheory/fp_invmod.c
SRC += ./tomsfastmath-0.14/src/mul/fp_mul_comba.c
SRC += ./tomsfastmath-0.14/src/mul/fp_mul_comba_4.c
SRC += ./tomsfastmath-0.14/src/mul/fp_mul_comba_24.c
SRC += ./tomsfastmath-0.14/src/mul/fp_mul_comba_7.c
SRC += ./tomsfastmath-0.14/src/mul/fp_mul.c
SRC += ./tomsfastmath-0.14/src/mul/fp_mul_comba_48.c
SRC += ./tomsfastmath-0.14/src/mul/fp_mul_comba_28.c
SRC += ./tomsfastmath-0.14/src/mul/fp_mul_comba_small_set.c
SRC += ./tomsfastmath-0.14/src/mul/fp_mulmod.c
SRC += ./tomsfastmath-0.14/src/mul/fp_mul_comba_9.c
SRC += ./tomsfastmath-0.14/src/mul/fp_mul_comba_64.c
SRC += ./tomsfastmath-0.14/src/mul/fp_mul_2.c
SRC += ./tomsfastmath-0.14/src/mul/fp_mul_2d.c
SRC += ./tomsfastmath-0.14/src/mul/fp_mul_comba_8.c
SRC += ./tomsfastmath-0.14/src/mul/fp_mul_comba_32.c
SRC += ./tomsfastmath-0.14/src/mul/fp_mul_d.c
SRC += ./tomsfastmath-0.14/src/mul/fp_mul_comba_3.c
SRC += ./tomsfastmath-0.14/src/mul/fp_mul_comba_12.c
SRC += ./tomsfastmath-0.14/src/mul/fp_mul_comba_20.c
SRC += ./tomsfastmath-0.14/src/mul/fp_mul_comba_17.c
SRC += ./tomsfastmath-0.14/src/mul/fp_mul_comba_6.c
SRC += ./tomsfastmath-0.14/src/mont/fp_montgomery_reduce.c
SRC += ./tomsfastmath-0.14/src/mont/fp_montgomery_calc_normalization.c
SRC += ./tomsfastmath-0.14/src/mont/fp_montgomery_setup.c
SRC += ./tomsfastmath-0.14/src/misc/fp_ident.c
SRC += ./tomsfastmath-0.14/src/misc/fp_set.c
SRC += ./tomsfastmath-0.14/src/misc/fp_get_string.c
#SRC += ./tomsfastmath-0.14/src/generators/comba_sqr_gen.c
#SRC += ./tomsfastmath-0.14/src/generators/comba_mont_gen.c
#SRC += ./tomsfastmath-0.14/src/generators/comba_mult_gen.c
#SRC += ./tomsfastmath-0.14/src/generators/comba_mult_smallgen.c
#SRC += ./tomsfastmath-0.14/src/generators/comba_sqr_smallgen.c
SRC += ./tomsfastmath-0.14/src/exptmod/fp_2expt.c
SRC += ./tomsfastmath-0.14/src/exptmod/fp_exptmod.c
SRC += ./tomsfastmath-0.14/src/divide/fp_mod_d.c
SRC += ./tomsfastmath-0.14/src/divide/fp_div.c
SRC += ./tomsfastmath-0.14/src/divide/fp_div_d.c
SRC += ./tomsfastmath-0.14/src/divide/fp_mod.c
SRC += ./tomsfastmath-0.14/src/bit/fp_rshd.c
SRC += ./tomsfastmath-0.14/src/bit/fp_cnt_lsb.c
SRC += ./tomsfastmath-0.14/src/bit/fp_lshd.c
SRC += ./tomsfastmath-0.14/src/bit/fp_mod_2d.c
SRC += ./tomsfastmath-0.14/src/bit/fp_div_2.c
SRC += ./tomsfastmath-0.14/src/bit/fp_div_2d.c
SRC += ./tomsfastmath-0.14/src/bit/fp_count_bits.c
SRC += ./tomsfastmath-0.14/src/bin/fp_reverse.c
SRC += ./tomsfastmath-0.14/src/bin/fp_read_radix.c
SRC += ./tomsfastmath-0.14/src/bin/fp_to_signed_bin.c
SRC += ./tomsfastmath-0.14/src/bin/fp_signed_bin_size.c
SRC += ./tomsfastmath-0.14/src/bin/fp_read_unsigned_bin.c
SRC += ./tomsfastmath-0.14/src/bin/fp_read_signed_bin.c
SRC += ./tomsfastmath-0.14/src/bin/fp_radix_size.c
SRC += ./tomsfastmath-0.14/src/bin/fp_unsigned_bin_size.c
SRC += ./tomsfastmath-0.14/src/bin/fp_s_rmap.c
SRC += ./tomsfastmath-0.14/src/bin/fp_to_unsigned_bin.c
SRC += ./tomsfastmath-0.14/src/bin/fp_toradix.c
SRC += ./tomsfastmath-0.14/src/addsub/fp_cmp_mag.c
SRC += ./tomsfastmath-0.14/src/addsub/fp_cmp.c
SRC += ./tomsfastmath-0.14/src/addsub/s_fp_add.c
SRC += ./tomsfastmath-0.14/src/addsub/fp_add_d.c
SRC += ./tomsfastmath-0.14/src/addsub/s_fp_sub.c
SRC += ./tomsfastmath-0.14/src/addsub/fp_sub.c
SRC += ./tomsfastmath-0.14/src/addsub/fp_sub_d.c
SRC += ./tomsfastmath-0.14/src/addsub/fp_cmp_d.c
SRC += ./tomsfastmath-0.14/src/addsub/fp_addmod.c
SRC += ./tomsfastmath-0.14/src/addsub/fp_add.c
SRC += ./tomsfastmath-0.14/src/addsub/fp_submod.c
else
SRC += ./libtommath-0.41/bn_mp_init_copy.c
SRC += ./libtommath-0.41/bn_fast_s_mp_sqr.c
SRC += ./libtommath-0.41/bn_mp_mul_d.c
SRC += ./libtommath-0.41/bn_mp_copy.c
SRC += ./libtommath-0.41/bn_mp_set_int.c
SRC += ./libtommath-0.41/bn_mp_reduce_2k.c
SRC += ./libtommath-0.41/bn_s_mp_sqr.c
SRC += ./libtommath-0.41/bn_mp_prime_rabin_miller_trials.c
SRC += ./libtommath-0.41/bn_mp_exch.c
SRC += ./libtommath-0.41/bn_mp_zero.c
SRC += ./libtommath-0.41/bn_mp_toradix.c
SRC += ./libtommath-0.41/bn_mp_radix_smap.c
SRC += ./libtommath-0.41/bn_mp_init_multi.c
SRC += ./libtommath-0.41/bn_mp_toradix_n.c
SRC += ./libtommath-0.41/bncore.c
SRC += ./libtommath-0.41/bn_mp_mod_2d.c
SRC += ./libtommath-0.41/bn_mp_cmp_mag.c
SRC += ./libtommath-0.41/bn_mp_to_signed_bin_n.c
SRC += ./libtommath-0.41/bn_mp_cmp_d.c
SRC += ./libtommath-0.41/bn_mp_fwrite.c
SRC += ./libtommath-0.41/bn_mp_gcd.c
SRC += ./libtommath-0.41/bn_mp_get_int.c
SRC += ./libtommath-0.41/bn_mp_neg.c
SRC += ./libtommath-0.41/bn_mp_xor.c
SRC += ./libtommath-0.41/bn_mp_rshd.c
SRC += ./libtommath-0.41/bn_mp_mod_d.c
SRC += ./libtommath-0.41/bn_fast_s_mp_mul_digs.c
SRC += ./libtommath-0.41/bn_mp_init_size.c
SRC += ./libtommath-0.41/bn_mp_dr_setup.c
SRC += ./libtommath-0.41/bn_mp_read_signed_bin.c
SRC += ./libtommath-0.41/bn_error.c
SRC += ./libtommath-0.41/bn_mp_set.c
SRC += ./libtommath-0.41/bn_mp_div_2d.c
SRC += ./libtommath-0.41/bn_mp_sub.c
SRC += ./libtommath-0.41/bn_mp_to_unsigned_bin_n.c
SRC += ./libtommath-0.41/bn_mp_cmp.c
SRC += ./libtommath-0.41/bn_mp_to_unsigned_bin.c
SRC += ./libtommath-0.41/bn_mp_reduce_2k_setup_l.c
SRC += ./libtommath-0.41/bn_mp_montgomery_setup.c
SRC += ./libtommath-0.41/bn_mp_exteuclid.c
SRC += ./libtommath-0.41/bn_mp_div.c
SRC += ./libtommath-0.41/bn_mp_reduce_setup.c
SRC += ./libtommath-0.41/bn_s_mp_exptmod.c
SRC += ./libtommath-0.41/bn_mp_init_set.c
SRC += ./libtommath-0.41/bn_prime_tab.c
SRC += ./libtommath-0.41/bn_mp_n_root.c
SRC += ./libtommath-0.41/bn_mp_init.c
SRC += ./libtommath-0.41/bn_mp_read_unsigned_bin.c
SRC += ./libtommath-0.41/bn_fast_s_mp_mul_high_digs.c
SRC += ./libtommath-0.41/bn_mp_mul.c
SRC += ./libtommath-0.41/bn_mp_prime_fermat.c
SRC += ./libtommath-0.41/bn_mp_mulmod.c
SRC += ./libtommath-0.41/bn_mp_expt_d.c
SRC += ./libtommath-0.41/bn_s_mp_mul_digs.c
SRC += ./libtommath-0.41/bn_mp_exptmod.c
SRC += ./libtommath-0.41/bn_mp_sqr.c
SRC += ./libtommath-0.41/bn_mp_karatsuba_sqr.c
SRC += ./libtommath-0.41/bn_mp_init_set_int.c
SRC += ./libtommath-0.41/bn_mp_get_string.c
SRC += ./libtommath-0.41/bn_mp_signed_bin_size.c
SRC += ./libtommath-0.41/bn_mp_reduce.c
SRC += ./libtommath-0.41/bn_mp_prime_next_prime.c
SRC += ./libtommath-0.41/bn_mp_div_3.c
SRC += ./libtommath-0.41/bn_mp_sqrmod.c
SRC += ./libtommath-0.41/bn_mp_invmod.c
SRC += ./libtommath-0.41/bn_mp_add.c
SRC += ./libtommath-0.41/bn_mp_grow.c
SRC += ./libtommath-0.41/bn_mp_sub_d.c
SRC += ./libtommath-0.41/bn_mp_dr_is_modulus.c
SRC += ./libtommath-0.41/bn_mp_prime_is_prime.c
SRC += ./libtommath-0.41/bn_mp_is_square.c
SRC += ./libtommath-0.41/bn_mp_prime_miller_rabin.c
SRC += ./libtommath-0.41/bn_mp_addmod.c
SRC += ./libtommath-0.41/bn_mp_clamp.c
SRC += ./libtommath-0.41/bn_fast_mp_invmod.c
SRC += ./libtommath-0.41/bn_mp_to_signed_bin.c
SRC += ./libtommath-0.41/bn_mp_montgomery_reduce.c
SRC += ./libtommath-0.41/bn_mp_lshd.c
SRC += ./libtommath-0.41/bn_mp_fread.c
SRC += ./libtommath-0.41/bn_mp_lcm.c
SRC += ./libtommath-0.41/bn_s_mp_mul_high_digs.c
SRC += ./libtommath-0.41/bn_mp_mul_2d.c
SRC += ./libtommath-0.41/bn_fast_mp_montgomery_reduce.c
SRC += ./libtommath-0.41/bn_mp_toom_mul.c
SRC += ./libtommath-0.41/bn_mp_and.c
SRC += ./libtommath-0.41/bn_mp_prime_is_divisible.c
SRC += ./libtommath-0.41/bn_mp_abs.c
SRC += ./libtommath-0.41/bn_mp_reduce_2k_setup.c
SRC += ./libtommath-0.41/bn_mp_unsigned_bin_size.c
SRC += ./libtommath-0.41/bn_mp_reduce_2k_l.c
SRC += ./libtommath-0.41/bn_mp_exptmod_fast.c
SRC += ./libtommath-0.41/bn_mp_add_d.c
SRC += ./libtommath-0.41/bn_mp_or.c
SRC += ./libtommath-0.41/bn_mp_dr_reduce.c
SRC += ./libtommath-0.41/bn_reverse.c
SRC += ./libtommath-0.41/bn_mp_sqrt.c
SRC += ./libtommath-0.41/bn_mp_mod.c
SRC += ./libtommath-0.41/bn_mp_montgomery_calc_normalization.c
SRC += ./libtommath-0.41/bn_mp_cnt_lsb.c
SRC += ./libtommath-0.41/bn_mp_radix_size.c
SRC += ./libtommath-0.41/bn_mp_reduce_is_2k.c
SRC += ./libtommath-0.41/bn_mp_shrink.c
SRC += ./libtommath-0.41/bn_mp_invmod_slow.c
SRC += ./libtommath-0.41/bn_mp_mul_2.c
SRC += ./libtommath-0.41/bn_mp_submod.c
SRC += ./libtommath-0.41/bn_mp_count_bits.c
SRC += ./libtommath-0.41/bn_mp_toom_sqr.c
SRC += ./libtommath-0.41/bn_mp_reduce_is_2k_l.c
SRC += ./libtommath-0.41/bn_mp_2expt.c
SRC += ./libtommath-0.41/bn_mp_clear.c
SRC += ./libtommath-0.41/bn_mp_div_2.c
SRC += ./libtommath-0.41/bn_mp_read_radix.c
SRC += ./libtommath-0.41/bn_s_mp_add.c
SRC += ./libtommath-0.41/bn_mp_karatsuba_mul.c
SRC += ./libtommath-0.41/bn_mp_div_d.c
SRC += ./libtommath-0.41/bn_s_mp_sub.c
SRC += ./libtommath-0.41/bn_mp_clear_multi.c
SRC += ./libtommath-0.41/bn_mp_prime_random_ex.c
SRC += ./libtommath-0.41/bn_mp_rand.c
SRC += ./libtommath-0.41/bn_mp_jacobi.c
endif


SUB_COMPONENTS =

DEFINES += -DLTC_SOURCE -DLTC_NO_FILE
ifeq "$(TOM_MATH)" "fast"
DEFINES += -DTFM_DESC
else
DEFINES += -DLTM_DESC
endif

CC_INC +=   -I$(DRV_ROOT)/$(DRIVER_DIR)/dmx/mtk_skb/libtomcrypt-1.16/src/headers
ifeq "$(TOM_MATH)" "fast"
CC_INC +=   -I$(DRV_ROOT)/$(DRIVER_DIR)/dmx/mtk_skb/tomsfastmath-0.14/src/headers
else
CC_INC +=   -I$(DRV_ROOT)/$(DRIVER_DIR)/dmx/mtk_skb/libtommath-0.41
endif

#############################################################################
#
# Include the makefile common to all components
#

include $(DRV_ROOT)/driver.mak
