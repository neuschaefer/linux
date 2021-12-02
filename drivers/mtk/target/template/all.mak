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
###########################################################################
# $RCSfile: all.mak,v $
# $Revision: #1 $
# $Date: 2012/04/27 $
# $Author: dtvbm11 $
# $CCRevision: /main/DTV_X/DTV_X_HQ_int/21 $
# $SWAuthor: Clear Case Administrator $
# $MD5HEX: 92549ed85f3314045e6ee95f924029a5 $
#
# Description:
#         Rules to create libraries, object files and dependency files.
#
#############################################################################

include $(TARGET_ROOT)/files.mak


#
# Adjust SUB_COMPONENTS and OPT_SUB_COMPONENTS if required
#
ifeq ($(strip $(NO_SUB_COMP)),TRUE)
SUB_COMPONENTS =
endif

ifeq ($(strip $(NO_OPT_SUB_COMP)),TRUE)
OPT_SUB_COMPONENTS =
endif


#
# Set the actual compiler include path and define
#
ABS_INCLUDE = $(sort $(CC_INC)) -I$(X_INC_DIR) -I$(D_INC_DIR) \
              $(TARGET_INC) $(TARGET_DRV_INC)
ABS_DEFINES = $(sort $(DEFINES))


#
# Specify the various goals
#
.PHONY : all target lib depend $(SUB_COMPONENTS) $(OPTIONAL_SUB_COMPONENTS)


all target lib : $(SUB_COMPONENTS) $(OPTIONAL_SUB_COMPONENTS) $(OBJ) $(ABS_LIB) $(LIBS)


depend : $(SUB_COMPONENTS) $(OPTIONAL_SUB_COMPONENTS)


#
# Cycle through all sub- and optional sub components.
#
$(SUB_COMPONENTS) :
	@$(MAKE) $(MAKE_FLAG) -C $(call adjust_target_comp_path,$@) $(MAKECMDGOALS) ENTER_OPT_SUB_COMP=FALSE


$(OPTIONAL_SUB_COMPONENTS) :
	@$(MAKE) $(MAKE_FLAG) -C $(call adjust_target_comp_path,$@) $(MAKECMDGOALS) ENTER_OPT_SUB_COMP=TRUE


#
# Set the search path's
#
vpath
vpath %.o $(OBJ_DIR)
vpath %.d $(DEPEND_DIR)

V ?= 0

ifeq "$(V)" "0"
QUIET = @
else
QUIET = 
endif

define rule_cc_o_c
	@echo -e "  CC\t$@"
	@echo -E '$(CC) $(CC_DEP_GEN_FLAG)$(strip $(OBJ_DIR)/$(@:.o=.d)) -c $(CC_FLAG_R) $(call inc_path_cnvt,$(ABS_INCLUDE)) $(ABS_DEFINES) $< -o $(OBJ_DIR)/$@' > $(OBJ_DIR)/$(@:.o=.o.cmd)
	$(QUIET)$(CC) $(CC_DEP_GEN_FLAG)$(strip $(OBJ_DIR)/$(@:.o=.d)) -c $(CC_FLAG_R) $(call inc_path_cnvt,$(ABS_INCLUDE)) $(ABS_DEFINES) $< -o $(OBJ_DIR)/$@ 2>&1 | tee $(OBJ_DIR)/$@.log
endef


define rule_cpp_o_c
	@echo -e "  CPP\t$@"
	@echo -E '$(CPP) $(CC_DEP_GEN_FLAG)$(strip $(OBJ_DIR)/$(@:.o=.d)) -c $(CPP_FLAG) $(call inc_path_cnvt,$(ABS_INCLUDE)) $(ABS_DEFINES) $< -o $(OBJ_DIR)/$@' > $(OBJ_DIR)/$(@:.o=.o.cmd)
	$(QUIET)$(CPP) $(CC_DEP_GEN_FLAG)$(strip $(OBJ_DIR)/$(@:.o=.d)) -c $(CPP_FLAG) $(call inc_path_cnvt,$(ABS_INCLUDE)) $(ABS_DEFINES) $< -o $(OBJ_DIR)/$@ 2>&1 | tee $(OBJ_DIR)/$@.log
endef


#
# Build libraries
#
$(LIBS) : $(OBJ)
	$(call if_dir_not_exist_fct,$(call path_cnvt,$(TARGET_LIB_DIR)), \
	    $(MKDIR) $(MKDIR_FLAG) $(call path_cnvt,$(TARGET_LIB_DIR));)
	@echo ""
	@echo "---------------- Archiving `basename $@` ---------------"
	@$(LOCKFILE) $(call path_cnvt,$@).lock
	$(AR) $(AR_FLAG) $(call path_cnvt,$@) $(call path_cnvt,$(ABS_OBJ))
	@$(RM) $(RM_FLAG) $(call path_cnvt,$@).lock


#
# And here the rules begin.
#
%.o : %.c
	$(call if_dir_not_exist_fct,$(dir $@), \
	    $(MKDIR) $(MKDIR_FLAG) $(dir $@);)
	$(call if_dir_not_exist_fct,$(dir $(OBJ_DIR)/$@), \
	    $(MKDIR) $(MKDIR_FLAG) $(dir $(OBJ_DIR)/$@);)
	@echo "$(CURT_ROOT)/$<,$(OBJ_DIR)/$@.log" >> $(WARNING_LOG)
	$(rule_cc_o_c)
	@$(call mod_dep_file_01_fct,$(OBJ_DIR)/$(@:.o=.d))

%.o : %.s
	$(call if_dir_not_exist_fct,$(call path_cnvt,$(OBJ_DIR)), \
	    $(MKDIR) $(MKDIR_FLAG) $(call path_cnvt,$(OBJ_DIR));)
	@echo ""
	@echo "---------------- Building `basename $@` ----------------"
	$(ASM) -c $(ASM_FLAG) $(ASM_DEFINES) -o $(call path_cnvt,$(OBJ_DIR)/$@) $(call path_cnvt,$<)



_obj_/%.o : %.o
	$(call if_dir_not_exist_fct,$(call path_cnvt,$(OBJ_DIR)), \
	    $(MKDIR) $(MKDIR_FLAG) $(call path_cnvt,$(OBJ_DIR));)
	@echo ""
	@echo "---------------- Copying `basename $@` ----------------"
	$(CP) $(CP_FLAG) $(call path_cnvt,$<) $(call path_cnvt,$(OBJ_DIR))


$(LIB_LIB_DIR)/%.a : %.a
	$(call if_dir_not_exist_fct,$(call path_cnvt,$(LIB_LIB_DIR)), \
	    $(MKDIR) $(MKDIR_FLAG) $(call path_cnvt,$(LIB_LIB_DIR));)
	@echo ""
	@echo "---------------- Copying `basename $@` ----------------"
	$(CP) $(CP_FLAG) $(call path_cnvt,$<) $(call path_cnvt,$(LIB_LIB_DIR))


ifneq ($(strip $(ABS_DEPEND)),)
-include $(ABS_DEPEND)
endif

