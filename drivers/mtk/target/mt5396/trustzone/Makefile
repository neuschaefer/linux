###############################################################################
# Copyright Statement:                                                        #
#                                                                             #
#   This software/firmware and related documentation ("MediaTek Software")    #
# are protected under international and related jurisdictions'copyright laws  #
# as unpublished works. The information contained herein is confidential and  #
# proprietary to MediaTek Inc. Without the prior written permission of        #
# MediaTek Inc., any reproduction, modification, use or disclosure of         #
# MediaTek Software, and information contained herein, in whole or in part,   #
# shall be strictly prohibited.                                               #
# MediaTek Inc. Copyright (C) 2010. All rights reserved.                      #
#                                                                             #
#   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND      #
# AGREES TO THE FOLLOWING:                                                    #
#                                                                             #
#   1)Any and all intellectual property rights (including without             #
# limitation, patent, copyright, and trade secrets) in and to this            #
# Software/firmware and related documentation ("MediaTek Software") shall     #
# remain the exclusive property of MediaTek Inc. Any and all intellectual     #
# property rights (including without limitation, patent, copyright, and       #
# trade secrets) in and to any modifications and derivatives to MediaTek      #
# Software, whoever made, shall also remain the exclusive property of         #
# MediaTek Inc.  Nothing herein shall be construed as any transfer of any     #
# title to any intellectual property right in MediaTek Software to Receiver.  #
#                                                                             #
#   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its  #
# representatives is provided to Receiver on an "AS IS" basis only.           #
# MediaTek Inc. expressly disclaims all warranties, expressed or implied,     #
# including but not limited to any implied warranties of merchantability,     #
# non-infringement and fitness for a particular purpose and any warranties    #
# arising out of course of performance, course of dealing or usage of trade.  #
# MediaTek Inc. does not provide any warranty whatsoever with respect to the  #
# software of any third party which may be used by, incorporated in, or       #
# supplied with the MediaTek Software, and Receiver agrees to look only to    #
# such third parties for any warranty claim relating thereto.  Receiver       #
# expressly acknowledges that it is Receiver's sole responsibility to obtain  #
# from any third party all proper licenses contained in or delivered with     #
# MediaTek Software.  MediaTek is not responsible for any MediaTek Software   #
# releases made to Receiver's specifications or to conform to a particular    #
# standard or open forum.                                                     #
#                                                                             #
#   3)Receiver further acknowledge that Receiver may, either presently        #
# and/or in the future, instruct MediaTek Inc. to assist it in the            #
# development and the implementation, in accordance with Receiver's designs,  #
# of certain softwares relating to Receiver's product(s) (the "Services").    #
# Except as may be otherwise agreed to in writing, no warranties of any       #
# kind, whether express or implied, are given by MediaTek Inc. with respect   #
# to the Services provided, and the Services are provided on an "AS IS"       #
# basis. Receiver further acknowledges that the Services may contain errors   #
# that testing is important and it is solely responsible for fully testing    #
# the Services and/or derivatives thereof before they are used, sublicensed   #
# or distributed. Should there be any third party action brought against      #
# MediaTek Inc. arising out of or relating to the Services, Receiver agree    #
# to fully indemnify and hold MediaTek Inc. harmless.  If the parties         #
# mutually agree to enter into or continue a business relationship or other   #
# arrangement, the terms and conditions set forth herein shall remain         #
# effective and, unless explicitly stated otherwise, shall prevail in the     #
# event of a conflict in the terms in any agreements entered into between     #
# the parties.                                                                #
#                                                                             #
#   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and     #
# cumulative liability with respect to MediaTek Software released hereunder   #
# will be, at MediaTek Inc.'s sole discretion, to replace or revise the       #
# MediaTek Software at issue.                                                 #
#                                                                             #
#   5)The transaction contemplated hereunder shall be construed in            #
# accordance with the laws of Singapore, excluding its conflict of laws       #
# principles.  Any disputes, controversies or claims arising thereof and      #
# related thereto shall be settled via arbitration in Singapore, under the    #
# then current rules of the International Chamber of Commerce (ICC).  The     #
# arbitration shall be conducted in English. The awards of the arbitration    #
# shall be final and binding upon both parties and shall be entered and       #
# enforceable in any court of competent jurisdiction.                         #
###############################################################################
ifndef DRV_ROOT
DRV_ROOT = ..
endif
OBJDIR = $(DRV_ROOT)/trustzone/obj

IC_NAME=5396


# toolchain
#RVCT_ROOT = /mtktools/ARM/RVDS40/RVCT
#ARM_BIN=$(RVCT_ROOT)/Programs/4.0/400/linux-pentium/
#ARM_INC=$(RVCT_ROOT)/Data/4.0/400/include/unix/
#ARM_LIB_PATH=$(RVCT_ROOT)/Data/4.0/400/lib
#export ARMLMD_LICENSE_FILE=$ARMLMD_LICENSE_FILE:8224@10.10.5.206

RVCT_ROOT = /mtktools/ARM/RVDS3.1/RVCT
ARM_BIN=$(RVCT_ROOT)/Programs/3.1/761/linux-pentium/
ARM_INC=$(RVCT_ROOT)/Data/3.1/761/include/unix/
ARM_LIB_PATH=$(RVCT_ROOT)/Data/3.1/761/lib

ASM = ${ARM_BIN}armasm
CC = ${ARM_BIN}armcc
LD = ${ARM_BIN}armlink
AR = ${ARM_BIN}armar
FROMELF = ${ARM_BIN}fromelf

CC_FLAG = --littleend -g -O2 -W --dwarf2 --no_unaligned_access -DCC_MT$(IC_NAME) -DNDEBUG -D_CPU_LITTLE_ENDIAN_ -DCC_SECURE_WORLD
CC_FLAG += -DCC_ENABLE_L2C
CC_FLAG += -DCC_CA9
CC_FLAG += -DLTC_SOURCE -DLTC_NO_FILE -DNPOPT_ENABLE_EXPORT_ACTION --enum_is_int
ASM_FLAG = -g --dwarf2 --pd "CC_MT$(IC_NAME) SETL {TRUE}" --diag_suppress A1786W
# cpu option
#CC_FLAG += --cpu Cortex-A9.no_neon
#ASM_FLAG += --cpu=Cortex-A9.no_neon
CC_FLAG += --cpu ARM1176JZF-S
ASM_FLAG += --cpu=ARM1176JZF-S
ifeq "$(TOM_MATH)" "fast"
CC_FLAG += -DTFM_DESC
else
CC_FLAG += -DLTM_DESC
endif

CC_INC = -I. -I./inc -I$(ARM_INC) \
-I$(DRV_ROOT)/$(IC_NAME)_driver/inc \
-I$(DRV_ROOT)/$(IC_NAME)_driver/drv_cust \
-I$(DRV_ROOT)/$(IC_NAME)_driver/drv_inc \
-I$(DRV_ROOT)/$(IC_NAME)_driver/nptv6896/inc/hw \
-I$(DRV_ROOT)/$(IC_NAME)_driver/nptv6896/inc/sys \
-I$(DRV_ROOT)/$(IC_NAME)_driver/bsp \
-I$(DRV_ROOT)/$(IC_NAME)_driver/bsp/dram \
-I$(DRV_ROOT)/$(IC_NAME)_driver/tz \
-I$(DRV_ROOT)/$(IC_NAME)_driver/dmx6896/aes \
-I$(DRV_ROOT)/$(IC_NAME)_driver/dmx6896/drm/secure_world_inc \
-I$(DRV_ROOT)/$(IC_NAME)_driver/dmx6896/drm/mtk_skb_driver \
-I$(DRV_ROOT)/$(IC_NAME)_driver/dmx6896/drm/libtomcrypt-1.16/src/headers \
-I$(DRV_ROOT)/$(IC_NAME)_driver/dmx6896/drm/libtommath-0.41 \
-I$(DRV_ROOT)/$(IC_NAME)_driver/dmx6896/drm/mtk_hci \
-I$(DRV_ROOT)/$(IC_NAME)_driver/dmx6896/drm/mtk_hci/nautilus_3.5_RC \
-I$(DRV_ROOT)/$(IC_NAME)_driver/dmx6896/drm/mtk_hci/nautilus_3.5_RC/include \
-I$(DRV_ROOT)/$(IC_NAME)_driver/dmx6896/drm/mtk_hci/nautilus_3.5_RC/include/local \
-I$(DRV_ROOT)/$(IC_NAME)_driver/dmx6896/drm/mtk_hci/nautilus_3.5_RC/include/src/cmn/include \
-I$(DRV_ROOT)/../../middleware \
-I$(DRV_ROOT)/../../middleware/os/inc/ \
-I$(INC_ROOT)/drv_inc 

TARGET = mt$(IC_NAME)_tz.axf

LD_FLAG = --entry INT_Reset --scatter ./scatter.sc --libpath $(ARM_LIB_PATH) \
		--xref --map --info sizes --verbose --list=$(subst axf,lst,$(TARGET)) \
		--locals --symdefs $(subst axf,sym,$(TARGET)) --diag_suppress=L6312W --no_unaligned_access

C_SRC = \
trustzone_main.c \
board/tgt_platform_init.c \
board/cpu_cache.c \
board/tgt_boot_pic.c \
manager/tzst_service_manager.c \
common/common.c \
common/printf.c \
services/aes.c \
services/svc_registry.c \
services/dmx_descrambler.c \
services/svc_dmx.c \
services/svc_example.c \
services/svc_drm.c \
services/svc_tzctl.c \
bsp/arm/hal_ca9.c \
bsp/arm/bootp.c \
dmx6896/drm/mtk_util.c \
dmx6896/drm/mtk_hci/aes_api.c \
dmx6896/drm/mtk_hci/rsa_api.c \
dmx6896/drm/mtk_hci/sha1_api.c \
dmx6896/drm/mtk_hci/sha256_api.c \
dmx6896/drm/mtk_hci/hmac_api.c \
dmx6896/drm/mtk_hci/crypto_api.c \
dmx6896/drm/mtk_hci/pkcs8.c \
dmx6896/drm/mtk_hci/asn.c \
dmx6896/drm/mtk_hci/custom_entry.c \
dmx6896/drm/mtk_hci/hci_binstr.c \
dmx6896/drm/mtk_hci/hci_mem.c \
dmx6896/drm/mtk_hci/hci.c \
dmx6896/drm/mtk_hci/hci_obfuscate.c \
dmx6896/drm/mtk_hci/hci_pers.c \
dmx6896/drm/mtk_skb_driver/SkbGenericCipher.c \
dmx6896/drm/mtk_skb_driver/SkbGenericEngine.c \
dmx6896/drm/mtk_skb_driver/SkbGenericSecureData.c \
dmx6896/drm/mtk_skb_driver/SkbGenericTransform.c \
dmx6896/drm/mtk_skb_driver/tomcrypt_asn1_missing.c \
dmx6896/drm/marlin3/DecoderDRM_TZ.c \
dmx6896/drm/marlin3/MediaDRM_TZ.c \
dmx6896/drm/libtomcrypt-1.16/src/ciphers/aes/aes.c \
dmx6896/drm/libtomcrypt-1.16/src/hashes/rmd128.c \
dmx6896/drm/libtomcrypt-1.16/src/hashes/rmd256.c \
dmx6896/drm/libtomcrypt-1.16/src/hashes/md2.c \
dmx6896/drm/libtomcrypt-1.16/src/hashes/md4.c \
dmx6896/drm/libtomcrypt-1.16/src/hashes/rmd160.c \
dmx6896/drm/libtomcrypt-1.16/src/hashes/md5.c \
dmx6896/drm/libtomcrypt-1.16/src/hashes/rmd320.c \
dmx6896/drm/libtomcrypt-1.16/src/hashes/sha1.c \
dmx6896/drm/libtomcrypt-1.16/src/hashes/tiger.c \
dmx6896/drm/libtomcrypt-1.16/src/hashes/sha2/sha512.c \
dmx6896/drm/libtomcrypt-1.16/src/hashes/sha2/sha256.c \
dmx6896/drm/libtomcrypt-1.16/src/mac/hmac/hmac_process.c \
dmx6896/drm/libtomcrypt-1.16/src/mac/hmac/hmac_done.c \
dmx6896/drm/libtomcrypt-1.16/src/mac/hmac/hmac_test.c \
dmx6896/drm/libtomcrypt-1.16/src/mac/hmac/hmac_memory.c \
dmx6896/drm/libtomcrypt-1.16/src/mac/hmac/hmac_file.c \
dmx6896/drm/libtomcrypt-1.16/src/mac/hmac/hmac_memory_multi.c \
dmx6896/drm/libtomcrypt-1.16/src/mac/hmac/hmac_init.c \
dmx6896/drm/libtomcrypt-1.16/src/math/multi.c \
dmx6896/drm/libtomcrypt-1.16/src/math/rand_prime.c \
dmx6896/drm/libtomcrypt-1.16/src/math/ltm_desc.c \
dmx6896/drm/libtomcrypt-1.16/src/math/gmp_desc.c \
dmx6896/drm/libtomcrypt-1.16/src/math/tfm_desc.c \
dmx6896/drm/libtomcrypt-1.16/src/modes/cbc/cbc_encrypt.c \
dmx6896/drm/libtomcrypt-1.16/src/modes/cbc/cbc_getiv.c \
dmx6896/drm/libtomcrypt-1.16/src/modes/cbc/cbc_start.c \
dmx6896/drm/libtomcrypt-1.16/src/modes/cbc/cbc_decrypt.c \
dmx6896/drm/libtomcrypt-1.16/src/modes/cbc/cbc_setiv.c \
dmx6896/drm/libtomcrypt-1.16/src/modes/cbc/cbc_done.c \
dmx6896/drm/libtomcrypt-1.16/src/modes/ctr/ctr_setiv.c \
dmx6896/drm/libtomcrypt-1.16/src/modes/ctr/ctr_encrypt.c \
dmx6896/drm/libtomcrypt-1.16/src/modes/ctr/ctr_done.c \
dmx6896/drm/libtomcrypt-1.16/src/modes/ctr/ctr_decrypt.c \
dmx6896/drm/libtomcrypt-1.16/src/modes/ctr/ctr_start.c \
dmx6896/drm/libtomcrypt-1.16/src/modes/ctr/ctr_getiv.c \
dmx6896/drm/libtomcrypt-1.16/src/modes/ctr/ctr_test.c \
dmx6896/drm/libtomcrypt-1.16/src/modes/ecb/ecb_done.c \
dmx6896/drm/libtomcrypt-1.16/src/modes/ecb/ecb_encrypt.c \
dmx6896/drm/libtomcrypt-1.16/src/modes/ecb/ecb_start.c \
dmx6896/drm/libtomcrypt-1.16/src/modes/ecb/ecb_decrypt.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/bit/der_length_bit_string.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/bit/der_encode_bit_string.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/bit/der_decode_bit_string.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/choice/der_decode_choice.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/ia5/der_decode_ia5_string.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/ia5/der_length_ia5_string.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/ia5/der_encode_ia5_string.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/integer/der_encode_integer.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/integer/der_decode_integer.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/integer/der_length_integer.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/object_identifier/der_decode_object_identifier.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/object_identifier/der_encode_object_identifier.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/object_identifier/der_length_object_identifier.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/octet/der_length_octet_string.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/octet/der_decode_octet_string.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/octet/der_encode_octet_string.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/printable_string/der_length_printable_string.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/printable_string/der_decode_printable_string.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/printable_string/der_encode_printable_string.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/sequence/der_decode_sequence_ex.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/sequence/der_sequence_free.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/sequence/der_length_sequence.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/sequence/der_encode_sequence_multi.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/sequence/der_decode_sequence_flexi.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/sequence/der_decode_sequence_multi.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/sequence/der_sequence_sibling_count.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/sequence/der_encode_sequence_ex.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/set/der_encode_setof.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/set/der_encode_set.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/short_integer/der_decode_short_integer.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/short_integer/der_length_short_integer.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/short_integer/der_encode_short_integer.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/utf8/der_encode_utf8_string.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/utf8/der_decode_utf8_string.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/utf8/der_length_utf8_string.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/boolean/der_encode_boolean.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/boolean/der_length_boolean.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/boolean/der_decode_boolean.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/t61/der_length_t61_string.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/t61/der_encode_t61_string.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/t61/der_decode_t61_string.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/utctime/der_decode_utctime.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/utctime/der_length_utctime.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/utctime/der_encode_utctime.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/generalized_time/der_decode_generalized_time.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/asn1/der/generalized_time/der_length_generalized_time.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/rsa/rsa_make_key.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/rsa/rsa_encrypt_key.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/rsa/rsa_exptmod.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/rsa/rsa_verify_hash.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/rsa/rsa_export.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/rsa/rsa_sign_hash.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/rsa/rsa_free.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/rsa/rsa_import.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/rsa/rsa_decrypt_key.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/pkcs1/pkcs_1_mgf1.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/pkcs1/pkcs_1_os2ip.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/pkcs1/pkcs_1_pss_encode.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/pkcs1/pkcs_1_oaep_encode.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/pkcs1/pkcs_1_i2osp.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/pkcs1/pkcs_1_v1_5_encode.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/pkcs1/pkcs_1_v1_5_decode.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/pkcs1/pkcs_1_oaep_decode.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/pkcs1/pkcs_1_pss_decode.c \
dmx6896/drm/libtomcrypt-1.16/src/prngs/sober128.c \
dmx6896/drm/libtomcrypt-1.16/src/prngs/rng_make_prng.c \
dmx6896/drm/libtomcrypt-1.16/src/prngs/rng_get_bytes.c \
dmx6896/drm/libtomcrypt-1.16/src/prngs/sprng.c \
dmx6896/drm/libtomcrypt-1.16/src/prngs/rc4.c \
dmx6896/drm/libtomcrypt-1.16/src/prngs/yarrow.c \
dmx6896/drm/libtomcrypt-1.16/src/prngs/fortuna.c \
dmx6896/drm/libtomcrypt-1.16/src/misc/zeromem.c \
dmx6896/drm/libtomcrypt-1.16/src/misc/error_to_string.c \
dmx6896/drm/libtomcrypt-1.16/src/misc/burn_stack.c \
dmx6896/drm/libtomcrypt-1.16/src/misc/crypt/crypt_register_prng.c \
dmx6896/drm/libtomcrypt-1.16/src/misc/crypt/crypt_find_hash.c \
dmx6896/drm/libtomcrypt-1.16/src/misc/crypt/crypt_find_hash_any.c \
dmx6896/drm/libtomcrypt-1.16/src/misc/crypt/crypt_hash_is_valid.c \
dmx6896/drm/libtomcrypt-1.16/src/misc/crypt/crypt_find_hash_oid.c \
dmx6896/drm/libtomcrypt-1.16/src/misc/crypt/crypt_find_cipher_id.c \
dmx6896/drm/libtomcrypt-1.16/src/misc/crypt/crypt.c \
dmx6896/drm/libtomcrypt-1.16/src/misc/crypt/crypt_register_hash.c \
dmx6896/drm/libtomcrypt-1.16/src/misc/crypt/crypt_fsa.c \
dmx6896/drm/libtomcrypt-1.16/src/misc/crypt/crypt_find_cipher.c \
dmx6896/drm/libtomcrypt-1.16/src/misc/crypt/crypt_cipher_is_valid.c \
dmx6896/drm/libtomcrypt-1.16/src/misc/crypt/crypt_register_cipher.c \
dmx6896/drm/libtomcrypt-1.16/src/misc/crypt/crypt_unregister_prng.c \
dmx6896/drm/libtomcrypt-1.16/src/misc/crypt/crypt_unregister_cipher.c \
dmx6896/drm/libtomcrypt-1.16/src/misc/crypt/crypt_prng_descriptor.c \
dmx6896/drm/libtomcrypt-1.16/src/misc/crypt/crypt_cipher_descriptor.c \
dmx6896/drm/libtomcrypt-1.16/src/misc/crypt/crypt_find_hash_id.c \
dmx6896/drm/libtomcrypt-1.16/src/misc/crypt/crypt_find_prng.c \
dmx6896/drm/libtomcrypt-1.16/src/misc/crypt/crypt_argchk.c \
dmx6896/drm/libtomcrypt-1.16/src/misc/crypt/crypt_hash_descriptor.c \
dmx6896/drm/libtomcrypt-1.16/src/misc/crypt/crypt_prng_is_valid.c \
dmx6896/drm/libtomcrypt-1.16/src/misc/crypt/crypt_unregister_hash.c \
dmx6896/drm/libtomcrypt-1.16/src/misc/crypt/crypt_find_cipher_any.c \
dmx6896/drm/libtomcrypt-1.16/src/misc/base64/base64_decode.c \
dmx6896/drm/libtomcrypt-1.16/src/misc/base64/base64_encode.c \
dmx6896/drm/libtomcrypt-1.16/src/hashes/helper/hash_file.c \
dmx6896/drm/libtomcrypt-1.16/src/hashes/helper/hash_filehandle.c \
dmx6896/drm/libtomcrypt-1.16/src/hashes/helper/hash_memory_multi.c \
dmx6896/drm/libtomcrypt-1.16/src/hashes/helper/hash_memory.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/ecc/ltc_ecc_mul2add.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/ecc/ecc_sizes.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/ecc/ltc_ecc_points.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/ecc/ecc_test.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/ecc/ecc_decrypt_key.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/ecc/ecc_make_key.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/ecc/ltc_ecc_projective_dbl_point.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/ecc/ecc_get_size.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/ecc/ecc_ansi_x963_import.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/ecc/ecc_encrypt_key.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/ecc/ecc_sign_hash.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/ecc/ecc_shared_secret.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/ecc/ltc_ecc_map.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/ecc/ecc_ansi_x963_export.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/ecc/ltc_ecc_mulmod_timing.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/ecc/ltc_ecc_projective_add_point.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/ecc/ecc_verify_hash.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/ecc/ecc_import.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/ecc/ecc.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/ecc/ecc_free.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/ecc/ltc_ecc_mulmod.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/ecc/ecc_export.c \
dmx6896/drm/libtomcrypt-1.16/src/pk/ecc/ltc_ecc_is_valid_idx.c \
dmx6896/sha/sha1.c \
dmx6896/sha/sha256.c \


ifeq "$(TOM_MATH)" "fast"
C_SRC += \
dmx6896/drm/tomsfastmath-0.14/src/sqr/fp_sqrmod.c \
dmx6896/drm/tomsfastmath-0.14/src/sqr/fp_sqr_comba_48.c \
dmx6896/drm/tomsfastmath-0.14/src/sqr/fp_sqr_comba_3.c \
dmx6896/drm/tomsfastmath-0.14/src/sqr/fp_sqr_comba_small_set.c \
dmx6896/drm/tomsfastmath-0.14/src/sqr/fp_sqr_comba_32.c \
dmx6896/drm/tomsfastmath-0.14/src/sqr/fp_sqr_comba_6.c \
dmx6896/drm/tomsfastmath-0.14/src/sqr/fp_sqr_comba_7.c \
dmx6896/drm/tomsfastmath-0.14/src/sqr/fp_sqr_comba_generic.c \
dmx6896/drm/tomsfastmath-0.14/src/sqr/fp_sqr_comba_17.c \
dmx6896/drm/tomsfastmath-0.14/src/sqr/fp_sqr_comba_4.c \
dmx6896/drm/tomsfastmath-0.14/src/sqr/fp_sqr_comba_24.c \
dmx6896/drm/tomsfastmath-0.14/src/sqr/fp_sqr_comba_8.c \
dmx6896/drm/tomsfastmath-0.14/src/sqr/fp_sqr_comba.c \
dmx6896/drm/tomsfastmath-0.14/src/sqr/fp_sqr_comba_20.c \
dmx6896/drm/tomsfastmath-0.14/src/sqr/fp_sqr_comba_28.c \
dmx6896/drm/tomsfastmath-0.14/src/sqr/fp_sqr_comba_12.c \
dmx6896/drm/tomsfastmath-0.14/src/sqr/fp_sqr_comba_64.c \
dmx6896/drm/tomsfastmath-0.14/src/sqr/fp_sqr_comba_9.c \
dmx6896/drm/tomsfastmath-0.14/src/sqr/fp_sqr.c \
dmx6896/drm/tomsfastmath-0.14/src/numtheory/fp_lcm.c \
dmx6896/drm/tomsfastmath-0.14/src/numtheory/fp_gcd.c \
dmx6896/drm/tomsfastmath-0.14/src/numtheory/fp_isprime.c \
dmx6896/drm/tomsfastmath-0.14/src/numtheory/fp_prime_random_ex.c \
dmx6896/drm/tomsfastmath-0.14/src/numtheory/fp_prime_miller_rabin.c \
dmx6896/drm/tomsfastmath-0.14/src/numtheory/fp_invmod.c \
dmx6896/drm/tomsfastmath-0.14/src/mul/fp_mul_comba.c \
dmx6896/drm/tomsfastmath-0.14/src/mul/fp_mul_comba_4.c \
dmx6896/drm/tomsfastmath-0.14/src/mul/fp_mul_comba_24.c \
dmx6896/drm/tomsfastmath-0.14/src/mul/fp_mul_comba_7.c \
dmx6896/drm/tomsfastmath-0.14/src/mul/fp_mul.c \
dmx6896/drm/tomsfastmath-0.14/src/mul/fp_mul_comba_48.c \
dmx6896/drm/tomsfastmath-0.14/src/mul/fp_mul_comba_28.c \
dmx6896/drm/tomsfastmath-0.14/src/mul/fp_mul_comba_small_set.c \
dmx6896/drm/tomsfastmath-0.14/src/mul/fp_mulmod.c \
dmx6896/drm/tomsfastmath-0.14/src/mul/fp_mul_comba_9.c \
dmx6896/drm/tomsfastmath-0.14/src/mul/fp_mul_comba_64.c \
dmx6896/drm/tomsfastmath-0.14/src/mul/fp_mul_2.c \
dmx6896/drm/tomsfastmath-0.14/src/mul/fp_mul_2d.c \
dmx6896/drm/tomsfastmath-0.14/src/mul/fp_mul_comba_8.c \
dmx6896/drm/tomsfastmath-0.14/src/mul/fp_mul_comba_32.c \
dmx6896/drm/tomsfastmath-0.14/src/mul/fp_mul_d.c \
dmx6896/drm/tomsfastmath-0.14/src/mul/fp_mul_comba_3.c \
dmx6896/drm/tomsfastmath-0.14/src/mul/fp_mul_comba_12.c \
dmx6896/drm/tomsfastmath-0.14/src/mul/fp_mul_comba_20.c \ 
dmx6896/drm/tomsfastmath-0.14/src/mul/fp_mul_comba_17.c \
dmx6896/drm/tomsfastmath-0.14/src/mul/fp_mul_comba_6.c \
dmx6896/drm/tomsfastmath-0.14/src/mont/fp_montgomery_reduce.c \
dmx6896/drm/tomsfastmath-0.14/src/mont/fp_montgomery_calc_normalization.c \
dmx6896/drm/tomsfastmath-0.14/src/mont/fp_montgomery_setup.c \
dmx6896/drm/tomsfastmath-0.14/src/misc/fp_ident.c \
dmx6896/drm/tomsfastmath-0.14/src/misc/fp_set.c \
dmx6896/drm/tomsfastmath-0.14/src/misc/fp_get_string.c \
#dmx6896/drm/tomsfastmath-0.14/src/generators/comba_sqr_gen.c \ 
#dmx6896/drm/tomsfastmath-0.14/src/generators/comba_mont_gen.c \
#dmx6896/drm/tomsfastmath-0.14/src/generators/comba_mult_gen.c \
#dmx6896/drm/tomsfastmath-0.14/src/generators/comba_mult_smallgen.c \
#dmx6896/drm/tomsfastmath-0.14/src/generators/comba_sqr_smallgen.c \
dmx6896/drm/tomsfastmath-0.14/src/exptmod/fp_2expt.c \
dmx6896/drm/tomsfastmath-0.14/src/exptmod/fp_exptmod.c \
dmx6896/drm/tomsfastmath-0.14/src/divide/fp_mod_d.c \
dmx6896/drm/tomsfastmath-0.14/src/divide/fp_div.c \
dmx6896/drm/tomsfastmath-0.14/src/divide/fp_div_d.c \
dmx6896/drm/tomsfastmath-0.14/src/divide/fp_mod.c \
dmx6896/drm/tomsfastmath-0.14/src/bit/fp_rshd.c \
dmx6896/drm/tomsfastmath-0.14/src/bit/fp_cnt_lsb.c \
dmx6896/drm/tomsfastmath-0.14/src/bit/fp_lshd.c \
dmx6896/drm/tomsfastmath-0.14/src/bit/fp_mod_2d.c \
dmx6896/drm/tomsfastmath-0.14/src/bit/fp_div_2.c \
dmx6896/drm/tomsfastmath-0.14/src/bit/fp_div_2d.c \
dmx6896/drm/tomsfastmath-0.14/src/bit/fp_count_bits.c \
dmx6896/drm/tomsfastmath-0.14/src/bin/fp_reverse.c \
dmx6896/drm/tomsfastmath-0.14/src/bin/fp_read_radix.c \
dmx6896/drm/tomsfastmath-0.14/src/bin/fp_to_signed_bin.c \
dmx6896/drm/tomsfastmath-0.14/src/bin/fp_signed_bin_size.c \
dmx6896/drm/tomsfastmath-0.14/src/bin/fp_read_unsigned_bin.c \
dmx6896/drm/tomsfastmath-0.14/src/bin/fp_read_signed_bin.c \
dmx6896/drm/tomsfastmath-0.14/src/bin/fp_radix_size.c \
dmx6896/drm/tomsfastmath-0.14/src/bin/fp_unsigned_bin_size.c \
dmx6896/drm/tomsfastmath-0.14/src/bin/fp_s_rmap.c \
dmx6896/drm/tomsfastmath-0.14/src/bin/fp_to_unsigned_bin.c \
dmx6896/drm/tomsfastmath-0.14/src/bin/fp_toradix.c \
dmx6896/drm/tomsfastmath-0.14/src/addsub/fp_cmp_mag.c \
dmx6896/drm/tomsfastmath-0.14/src/addsub/fp_cmp.c \
dmx6896/drm/tomsfastmath-0.14/src/addsub/s_fp_add.c \
dmx6896/drm/tomsfastmath-0.14/src/addsub/fp_add_d.c \
dmx6896/drm/tomsfastmath-0.14/src/addsub/s_fp_sub.c \
dmx6896/drm/tomsfastmath-0.14/src/addsub/fp_sub.c \
dmx6896/drm/tomsfastmath-0.14/src/addsub/fp_sub_d.c \
dmx6896/drm/tomsfastmath-0.14/src/addsub/fp_cmp_d.c \
dmx6896/drm/tomsfastmath-0.14/src/addsub/fp_addmod.c \
dmx6896/drm/tomsfastmath-0.14/src/addsub/fp_add.c \
dmx6896/drm/tomsfastmath-0.14/src/addsub/fp_submod.c
else
C_SRC += \
dmx6896/drm/libtommath-0.41/bn_mp_init_copy.c \
dmx6896/drm/libtommath-0.41/bn_fast_s_mp_sqr.c \
dmx6896/drm/libtommath-0.41/bn_mp_mul_d.c \
dmx6896/drm/libtommath-0.41/bn_mp_copy.c \
dmx6896/drm/libtommath-0.41/bn_mp_set_int.c \
dmx6896/drm/libtommath-0.41/bn_mp_reduce_2k.c \
dmx6896/drm/libtommath-0.41/bn_s_mp_sqr.c \
dmx6896/drm/libtommath-0.41/bn_mp_prime_rabin_miller_trials.c \
dmx6896/drm/libtommath-0.41/bn_mp_exch.c \
dmx6896/drm/libtommath-0.41/bn_mp_zero.c \
dmx6896/drm/libtommath-0.41/bn_mp_toradix.c \
dmx6896/drm/libtommath-0.41/bn_mp_radix_smap.c \
dmx6896/drm/libtommath-0.41/bn_mp_init_multi.c \
dmx6896/drm/libtommath-0.41/bn_mp_toradix_n.c \
dmx6896/drm/libtommath-0.41/bncore.c \
dmx6896/drm/libtommath-0.41/bn_mp_mod_2d.c \
dmx6896/drm/libtommath-0.41/bn_mp_cmp_mag.c \
dmx6896/drm/libtommath-0.41/bn_mp_to_signed_bin_n.c \
dmx6896/drm/libtommath-0.41/bn_mp_cmp_d.c \
dmx6896/drm/libtommath-0.41/bn_mp_fwrite.c \
dmx6896/drm/libtommath-0.41/bn_mp_gcd.c \
dmx6896/drm/libtommath-0.41/bn_mp_get_int.c \
dmx6896/drm/libtommath-0.41/bn_mp_neg.c \
dmx6896/drm/libtommath-0.41/bn_mp_xor.c \
dmx6896/drm/libtommath-0.41/bn_mp_rshd.c \
dmx6896/drm/libtommath-0.41/bn_mp_mod_d.c \
dmx6896/drm/libtommath-0.41/bn_fast_s_mp_mul_digs.c \
dmx6896/drm/libtommath-0.41/bn_mp_init_size.c \
dmx6896/drm/libtommath-0.41/bn_mp_dr_setup.c \
dmx6896/drm/libtommath-0.41/bn_mp_read_signed_bin.c \
dmx6896/drm/libtommath-0.41/bn_error.c \
dmx6896/drm/libtommath-0.41/bn_mp_set.c \
dmx6896/drm/libtommath-0.41/bn_mp_div_2d.c \
dmx6896/drm/libtommath-0.41/bn_mp_sub.c \
dmx6896/drm/libtommath-0.41/bn_mp_to_unsigned_bin_n.c \
dmx6896/drm/libtommath-0.41/bn_mp_cmp.c \
dmx6896/drm/libtommath-0.41/bn_mp_to_unsigned_bin.c \
dmx6896/drm/libtommath-0.41/bn_mp_reduce_2k_setup_l.c \
dmx6896/drm/libtommath-0.41/bn_mp_montgomery_setup.c \
dmx6896/drm/libtommath-0.41/bn_mp_exteuclid.c \
dmx6896/drm/libtommath-0.41/bn_mp_div.c \
dmx6896/drm/libtommath-0.41/bn_mp_reduce_setup.c \
dmx6896/drm/libtommath-0.41/bn_s_mp_exptmod.c \
dmx6896/drm/libtommath-0.41/bn_mp_init_set.c \
dmx6896/drm/libtommath-0.41/bn_prime_tab.c \
dmx6896/drm/libtommath-0.41/bn_mp_n_root.c \
dmx6896/drm/libtommath-0.41/bn_mp_init.c \
dmx6896/drm/libtommath-0.41/bn_mp_read_unsigned_bin.c \
dmx6896/drm/libtommath-0.41/bn_fast_s_mp_mul_high_digs.c \
dmx6896/drm/libtommath-0.41/bn_mp_mul.c \
dmx6896/drm/libtommath-0.41/bn_mp_prime_fermat.c \
dmx6896/drm/libtommath-0.41/bn_mp_mulmod.c \
dmx6896/drm/libtommath-0.41/bn_mp_expt_d.c \
dmx6896/drm/libtommath-0.41/bn_s_mp_mul_digs.c \
dmx6896/drm/libtommath-0.41/bn_mp_exptmod.c \
dmx6896/drm/libtommath-0.41/bn_mp_sqr.c \
dmx6896/drm/libtommath-0.41/bn_mp_karatsuba_sqr.c \
dmx6896/drm/libtommath-0.41/bn_mp_init_set_int.c \
dmx6896/drm/libtommath-0.41/bn_mp_get_string.c \
dmx6896/drm/libtommath-0.41/bn_mp_signed_bin_size.c \
dmx6896/drm/libtommath-0.41/bn_mp_reduce.c \
dmx6896/drm/libtommath-0.41/bn_mp_prime_next_prime.c \
dmx6896/drm/libtommath-0.41/bn_mp_div_3.c \
dmx6896/drm/libtommath-0.41/bn_mp_sqrmod.c \
dmx6896/drm/libtommath-0.41/bn_mp_invmod.c \
dmx6896/drm/libtommath-0.41/bn_mp_add.c \
dmx6896/drm/libtommath-0.41/bn_mp_grow.c \
dmx6896/drm/libtommath-0.41/bn_mp_sub_d.c \
dmx6896/drm/libtommath-0.41/bn_mp_dr_is_modulus.c \
dmx6896/drm/libtommath-0.41/bn_mp_prime_is_prime.c \
dmx6896/drm/libtommath-0.41/bn_mp_is_square.c \
dmx6896/drm/libtommath-0.41/bn_mp_prime_miller_rabin.c \
dmx6896/drm/libtommath-0.41/bn_mp_addmod.c \
dmx6896/drm/libtommath-0.41/bn_mp_clamp.c \
dmx6896/drm/libtommath-0.41/bn_fast_mp_invmod.c \
dmx6896/drm/libtommath-0.41/bn_mp_to_signed_bin.c \
dmx6896/drm/libtommath-0.41/bn_mp_montgomery_reduce.c \
dmx6896/drm/libtommath-0.41/bn_mp_lshd.c \
dmx6896/drm/libtommath-0.41/bn_mp_fread.c \
dmx6896/drm/libtommath-0.41/bn_mp_lcm.c \
dmx6896/drm/libtommath-0.41/bn_s_mp_mul_high_digs.c \
dmx6896/drm/libtommath-0.41/bn_mp_mul_2d.c \
dmx6896/drm/libtommath-0.41/bn_fast_mp_montgomery_reduce.c \
dmx6896/drm/libtommath-0.41/bn_mp_toom_mul.c \
dmx6896/drm/libtommath-0.41/bn_mp_and.c \
dmx6896/drm/libtommath-0.41/bn_mp_prime_is_divisible.c \
dmx6896/drm/libtommath-0.41/bn_mp_abs.c \
dmx6896/drm/libtommath-0.41/bn_mp_reduce_2k_setup.c \
dmx6896/drm/libtommath-0.41/bn_mp_unsigned_bin_size.c \
dmx6896/drm/libtommath-0.41/bn_mp_reduce_2k_l.c \
dmx6896/drm/libtommath-0.41/bn_mp_exptmod_fast.c \
dmx6896/drm/libtommath-0.41/bn_mp_add_d.c \
dmx6896/drm/libtommath-0.41/bn_mp_or.c \
dmx6896/drm/libtommath-0.41/bn_mp_dr_reduce.c \
dmx6896/drm/libtommath-0.41/bn_reverse.c \
dmx6896/drm/libtommath-0.41/bn_mp_sqrt.c \
dmx6896/drm/libtommath-0.41/bn_mp_mod.c \
dmx6896/drm/libtommath-0.41/bn_mp_montgomery_calc_normalization.c \
dmx6896/drm/libtommath-0.41/bn_mp_cnt_lsb.c \
dmx6896/drm/libtommath-0.41/bn_mp_radix_size.c \
dmx6896/drm/libtommath-0.41/bn_mp_reduce_is_2k.c \
dmx6896/drm/libtommath-0.41/bn_mp_shrink.c \
dmx6896/drm/libtommath-0.41/bn_mp_invmod_slow.c \
dmx6896/drm/libtommath-0.41/bn_mp_mul_2.c \
dmx6896/drm/libtommath-0.41/bn_mp_submod.c \
dmx6896/drm/libtommath-0.41/bn_mp_count_bits.c \
dmx6896/drm/libtommath-0.41/bn_mp_toom_sqr.c \
dmx6896/drm/libtommath-0.41/bn_mp_reduce_is_2k_l.c \
dmx6896/drm/libtommath-0.41/bn_mp_2expt.c \
dmx6896/drm/libtommath-0.41/bn_mp_clear.c \
dmx6896/drm/libtommath-0.41/bn_mp_div_2.c \
dmx6896/drm/libtommath-0.41/bn_mp_read_radix.c \
dmx6896/drm/libtommath-0.41/bn_s_mp_add.c \
dmx6896/drm/libtommath-0.41/bn_mp_karatsuba_mul.c \
dmx6896/drm/libtommath-0.41/bn_mp_div_d.c \
dmx6896/drm/libtommath-0.41/bn_s_mp_sub.c \
dmx6896/drm/libtommath-0.41/bn_mp_clear_multi.c \
dmx6896/drm/libtommath-0.41/bn_mp_prime_random_ex.c \
dmx6896/drm/libtommath-0.41/bn_mp_rand.c \
dmx6896/drm/libtommath-0.41/bn_mp_jacobi.c
endif

A_SRC = \
board/tgt_vectors.s \
board/tgt_boot_init.s \
manager/tzst_service_api.s \
monitor/monitor.s \
monitor/mon_switch.s \
monitor/mon_vectors.s \
monitor/mon_macros.s \
monitor/mon_init.s


OBJ = $(A_SRC:.s=.o) $(C_SRC:.c=.o)

.PHONY : all bin symlink clean

all: symlink bin
	@echo "build OK"

symlink:
	@if [ ! -e bsp ]; then \
		(ln -s ../../mt$(IC_NAME)/$(IC_NAME)_driver/bsp .; exit 0); \
	fi
	@if [ ! -e dmx6896 ]; then \
		(ln -s ../../mt$(IC_NAME)/$(IC_NAME)_driver/dmx6896 .; exit 0); \
	fi
	
bin: $(TARGET)
	@$(FROMELF) --bin $(TARGET) --output $(subst axf,bin,$(TARGET))

$(TARGET): $(OBJ)
	@echo "Linking $(TARGET)..."
	$(CC) -o trustzone.scatx $(CC_FLAG) $(CC_INC) $(DEFINES) -E trustzone.scat
	@awk '/ROM_LOAD/,p' trustzone.scatx > scatter.sc
	$(LD) $(LD_FLAG) $(addprefix $(OBJDIR)/,$(OBJ)) -o $(TARGET)

%.o :: %.c
	@echo "--- Building `basename $<` ---"
	@if [ ! -d $(OBJDIR)/`dirname $@` ] ; then \
		mkdir -p $(OBJDIR)/`dirname $@`; \
	fi
	$(CC) -c $(CC_FLAG) $(CC_INC) $(DEFINES) -o $(OBJDIR)/$@ $<

%.o :: %.s
	@echo "--- Building `basename $<` ---"
	@if [ ! -d $(OBJDIR)/`dirname $@` ] ; then \
		mkdir -p $(OBJDIR)/`dirname $@`; \
	fi
	$(ASM) $(ASM_FLAG) $(ASMDEFINES) -o $(OBJDIR)/$@ $<
	@if [ -e $@ ] ; then \
		@mv $@ .; \
	fi


clean:
	rm -rf $(TARGET) *.axf *.bin $(OBJDIR) *.bak *.lst *.sym trustzone.scatx scatter.sc
	rm -f `find -type l`

