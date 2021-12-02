/*************************************************************************************
                  Silicon Laboratories Broadcast Si2156 Layer 1 API
   API types used by commands and properties
   FILE: Si2156_typedefs.h
   Supported IC : Si2156
   Compiled for ROM 12 firmware 1_E_build_6
   Revision: REVISION_NAME
   Date: May 31 2011
  (C) Copyright 2011, Silicon Laboratories, Inc. All rights reserved.
**************************************************************************************/
#ifndef   Si2156_TYPEDEFS_H
#define   Si2156_TYPEDEFS_H


typedef struct L1_Si2156_Context {
  L0_Context                 *i2c;
  L0_Context                  i2cObj;
  Si2156_CmdObj              *cmd;
  Si2156_CmdObj               cmdObj;
  Si2156_CmdReplyObj         *rsp;
  Si2156_CmdReplyObj          rspObj;
  Si2156_PropObj             *prop;
  Si2156_PropObj              propObj;
  Si2156_COMMON_REPLY_struct *status;
  Si2156_COMMON_REPLY_struct  statusObj;
} L1_Si2156_Context;
#endif /* Si2156_TYPEDEFS_H */





