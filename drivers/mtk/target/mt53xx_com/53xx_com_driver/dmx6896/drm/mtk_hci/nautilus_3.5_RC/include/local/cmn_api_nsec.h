/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2006,2007,2008,2009,2010,2011 Sony Corporation, All Rights Reserved.
 *
 */
#ifndef CMN_API_NSEC_H
#define CMN_API_NSEC_H

/** @file
 *
 *
 * @require none
 *
 * @author Yoshizumi Tanaka <y-tanaka@sm.sony.co.jp>
 */

NP_BEGIN_EXTERN_C

/**
 * base_api.h
 */
#define base_Init(in_mem, in_memSize) \
    npi_base_Init(NPI_CMN_CONTEXT_NONSECURE, in_mem, in_memSize)

#define base_Fin() \
    npi_base_Fin(NPI_CMN_CONTEXT_NONSECURE)

#define base_InitForSample() \
    npi_base_InitForSample()

#define base_FinForSample() \
    npi_base_FinForSample()

/**
 * soapfault_api.h
 */
#define soapfault_FreeDetail(io_detail) \
    npi_soapfault_FreeDetail(NPI_CMN_CONTEXT_NONSECURE, io_detail)

#define soapfault_FreeInfo(io_info) \
    npi_soapfault_FreeInfo(NPI_CMN_CONTEXT_NONSECURE, io_info)

/**
 * binstr_api.h
 */
#define BINSTR(a,b) NPI_BINSTR(a,b)

#define binstr_Len(in_str) npi_binstr_Len(in_str)
#define binstr_Buf(in_str) npi_binstr_Buf(in_str)
#define binstr_Check(in_str) npi_binstr_Check(in_str)

#define binstr_Alloc(in_size, in_buf) \
    npi_binstr_Alloc(NPI_CMN_CONTEXT_NONSECURE, in_size, in_buf)

#define binstr_AllocFill(in_size, in_byte) \
    npi_binstr_AllocFill(NPI_CMN_CONTEXT_NONSECURE, in_size, in_byte)

#define binstr_Free(io_str) \
    npi_binstr_Free(NPI_CMN_CONTEXT_NONSECURE, io_str)

#define binstr_Dup(in_str) \
    npi_binstr_Dup(NPI_CMN_CONTEXT_NONSECURE, in_str)

#define binstr_Concat(in_str1, in_str2) \
    npi_binstr_Concat(NPI_CMN_CONTEXT_NONSECURE, in_str1, in_str2)

#define binstr_CmpBuf(in_len1, in_buf1, in_len2, in_buf2) \
    npi_binstr_CmpBuf(NPI_CMN_CONTEXT_NONSECURE, in_len1, in_buf1, in_len2, in_buf2)

#define binstr_CmpFun(in_str1, in_str2) \
    npi_binstr_CmpFun(NPI_CMN_CONTEXT_NONSECURE, in_str1, in_str2)

#define binstr_Cmp(in_str1, in_str2) \
    npi_binstr_Cmp(NPI_CMN_CONTEXT_NONSECURE, in_str1, in_str2)

#define binstr_Dump(in_str)   npi_binstr_Dump(in_str)
#define binstr_Dump0(in_str)  npi_binstr_Dump0(in_str)
#define binstr_Dump1(in_str)  npi_binstr_Dump1(in_str)
#define binstr_Dump2(in_str)  npi_binstr_Dump2(in_str)
#define binstr_Dump3(in_str)  npi_binstr_Dump3(in_str)

#define binstr_Print(in_str)  npi_binstr_Print(in_str)
#define binstr_Print0(in_str) npi_binstr_Print0(in_str)
#define binstr_Print1(in_str) npi_binstr_Print1(in_str)
#define binstr_Print2(in_str) npi_binstr_Print2(in_str)
#define binstr_Print3(in_str) npi_binstr_Print3(in_str)

#define binstr_SetClear(io_str) \
    npi_binstr_SetClear(NPI_CMN_CONTEXT_NONSECURE, io_str)

#define binstr_SetReference(io_str, in_size, in_buf) \
    npi_binstr_SetReference(NPI_CMN_CONTEXT_NONSECURE, io_str, in_size, in_buf)

#define binstr_SetClone(io_str, in_size, in_buf) \
    npi_binstr_SetClone(NPI_CMN_CONTEXT_NONSECURE, io_str, in_size, in_buf)

#define binstr_FreeClone(io_str) \
    npi_binstr_FreeClone(NPI_CMN_CONTEXT_NONSECURE, io_str)

#define binstr_ToStr(in_str, out_dst) \
    npi_binstr_ToStr(NPI_CMN_CONTEXT_NONSECURE, in_str, out_dst)

#define mbinstr_Alloc(in_num, in_strs) \
    npi_mbinstr_Alloc(NPI_CMN_CONTEXT_NONSECURE, in_num, in_strs)

#define mbinstr_Free(io_strs) \
    npi_mbinstr_Free(NPI_CMN_CONTEXT_NONSECURE, io_strs)

/**
 * byte_api.h
 */
#define byte_bswap16(src) npi_byte_bswap16(src)
#define byte_bswap32(src) npi_byte_bswap32(src)

#define byte_BE1R(p)      npi_byte_BE1R(p)
#define byte_BE2R(p)      npi_byte_BE2R(p)
#define byte_BE4R(p)      npi_byte_BE4R(p)
#define byte_LE1R(p)      npi_byte_LE1R(p)
#define byte_LE2R(p)      npi_byte_LE2R(p)
#define byte_LE4R(p)      npi_byte_LE4R(p)
#define byte_BE1W(p,v)    npi_byte_BE1W(p,v)
#define byte_BE2W(p,v)    npi_byte_BE2W(p,v)
#define byte_BE4W(p,v)    npi_byte_BE4W(p,v)
#define byte_LE1W(p,v)    npi_byte_LE1W(p,v)
#define byte_LE2W(p,v)    npi_byte_LE2W(p,v)
#define byte_LE4W(p,v)    npi_byte_LE4W(p,v)

#define byte_4CHARCODE(a,b,c,d)    npi_byte_4CHARCODE(a,b,c,d)

/**
 * b64_api.h
 */
#define b64_Encode(in_str, in_chars_inline, in_eolchars, out_str) \
    npi_b64_Encode(NPI_CMN_CONTEXT_NONSECURE, in_str, in_chars_inline, in_eolchars, out_str)

#define b64_Decode(in_str, out_str) \
    npi_b64_Decode(NPI_CMN_CONTEXT_NONSECURE, in_str, out_str)


/**
 * device_api.h
 */
#define device_GetUid() \
    npi_device_GetUid(NPI_CMN_CONTEXT_NONSECURE)

#define device_SetUid(in_len, in_buf) \
    npi_device_SetUid(NPI_CMN_CONTEXT_NONSECURE, in_len, in_buf)


/**
 * fio_api.h
 */
#define fio_Open(in_filename, in_openmode, out_handle) \
    npi_fio_Open(NPI_CMN_CONTEXT_NONSECURE, in_filename, in_openmode, out_handle)

#define fio_Close(io_handle) \
    npi_fio_Close(NPI_CMN_CONTEXT_NONSECURE, io_handle)

#define fio_Read(io_handle, in_size, out_size, out_buffer) \
    npi_fio_Read(NPI_CMN_CONTEXT_NONSECURE, io_handle, in_size, out_size, out_buffer)

#define fio_ReadUntil(io_handle, in_delimit, in_size, out_size, out_buffer) \
    npi_fio_ReadUntil(NPI_CMN_CONTEXT_NONSECURE, io_handle, in_delimit, in_size, out_size, out_buffer)

#define fio_Write(io_handle, in_size, in_buffer) \
    npi_fio_Write(NPI_CMN_CONTEXT_NONSECURE, io_handle, in_size, in_buffer)

#define fio_Seek(io_handle, in_offset, in_whence) \
    npi_fio_Seek(NPI_CMN_CONTEXT_NONSECURE, io_handle, in_offset, in_whence)

#define fio_Tell(io_handle, out_size) \
    npi_fio_Tell(NPI_CMN_CONTEXT_NONSECURE, io_handle, out_size)

#define fio_Length(io_handle, out_size) \
    npi_fio_Length(NPI_CMN_CONTEXT_NONSECURE, io_handle, out_size)

#define fio_Remove(in_filename) \
    npi_fio_Remove(NPI_CMN_CONTEXT_NONSECURE, in_filename)

#define fio_MakeDir(in_dir) \
    npi_fio_MakeDir(NPI_CMN_CONTEXT_NONSECURE, in_dir)

#define fio_GetHomeDir(io_len, out_buf) \
    npi_fio_GetHomeDir(NPI_CMN_CONTEXT_NONSECURE, io_len, out_buf)

#define fio_AddPath(io_len, io_buf, in_addpath) \
    npi_fio_AddPath(io_len, io_buf, in_addpath)

#define fio_Stat(in_filename, out_statbuf) \
    npi_fio_Stat(NPI_CMN_CONTEXT_NONSECURE, in_filename, out_statbuf)

#define fio_Stat_FileSize(fsb) npi_fio_Stat_FileSize(fsb)

#define fio_IsDirectory(in_path, out_result) \
    npi_fio_IsDirectory(in_path, out_result)

#define fio_GetFileDelimiter  npi_fio_GetFileDelimiter

#define  fio_RemoveDirectory(in_path) \
    npi_fio_RemoveDirectory(in_path)

#define fio_GetDirectoryFiles(io_len, out_buf) \
    npi_fio_GetDirectoryFiles(NPI_CMN_CONTEXT_NONSECURE, io_len, out_buf)

#define fio_FreeFileList(in_files) \
    npi_fio_FreeFileList(NPI_CMN_CONTEXT_NONSECURE, in_files)

/**
 * mem_api.h
 */
#define mem_Init(in_start, in_size) \
    npi_mem_Init(NPI_CMN_CONTEXT_NONSECURE, in_start, in_size)

#define mem_Fin() \
    npi_mem_Fin(NPI_CMN_CONTEXT_NONSECURE)

#define mem_Alloc(in_size) \
    npi_mem_Alloc(NPI_CMN_CONTEXT_NONSECURE, in_size)

#define mem_Realloc(in_buf, in_size) \
    npi_mem_Realloc(NPI_CMN_CONTEXT_NONSECURE, in_buf, in_size)

#define mem_Free(io_buf) \
    npi_mem_Free(NPI_CMN_CONTEXT_NONSECURE, io_buf)

#define mem_StrDup(in_str) \
    npi_mem_StrDup(NPI_CMN_CONTEXT_NONSECURE, in_str)

#define mem_Move(out_dst, in_src, in_size) \
    npi_mem_Move(out_dst, in_src, in_size)

#define mem_Copy(out_dst, in_src, in_size) \
    npi_mem_Copy(out_dst, in_src, in_size)

#define mem_Cmp(in_buf1, in_buf2, in_size) \
    npi_mem_Cmp(in_buf1, in_buf2, in_size)

#define mem_Set(in_buf, in_c, in_size) \
    npi_mem_Set(in_buf, in_c, in_size)

/**
 * mutex_api.h
 */
#define mutex_Init(out_mutex)  npi_mutex_Init(out_mutex)
#define mutex_Lock(io_mutex)   npi_mutex_Lock(io_mutex)
#define mutex_Unlock(in_mutex) npi_mutex_Unlock(in_mutex)
#define mutex_Fin(io_mutex)    npi_mutex_Fin(io_mutex)

#define mutex_ProcLock(in_id, in_timeout, out_mutex) \
    npi_mutex_ProcLock(NPI_CMN_CONTEXT_NONSECURE, in_id, in_timeout, out_mutex)

#define mutex_ProcUnlock(in_mutex) \
    npi_mutex_ProcUnlock(NPI_CMN_CONTEXT_NONSECURE, in_mutex)


/**
 * time_api.h
 */
#define time_GetTime()                                         npi_time_GetTime()
#define time_GetLocalTimeOffset()                              npi_time_GetLocalTimeOffset()
#define time_GetSeedValue(out_value)                           npi_time_GetSeedValue(out_value)
#define time_Date2Unix(in_date, out_time)                      npi_time_Date2Unix(in_date, out_time)
#define time_Unix2Date(in_time, in_offset, in_isdst, out_date) npi_time_Unix2Date(in_time, in_offset, in_isdst, out_date)
#define time_EPOCMin2Date(in_time, out_date)                   npi_time_EPOCMin2Date(in_time, out_date)
#define time_Date2EPOCMin(in_date, out_time)                   npi_time_Date2EPOCMin(in_date, out_time)
#define time_Cmp(in_t1, in_t2) npi_time_Cmp(in_t1, in_t2)


/**
 * vlbinstr_api.h
 */
#define vlbinstr_Dup(in_str) \
    npi_vlbinstr_Dup(NPI_CMN_CONTEXT_NONSECURE, in_str)

#define vlbinstr_AppendBuf(io_str, in_len, in_buf) \
    npi_vlbinstr_AppendBuf(NPI_CMN_CONTEXT_NONSECURE, io_str, in_len, in_buf)

#define vlbinstr_Append(io_str1, io_str2) \
    npi_vlbinstr_Append(NPI_CMN_CONTEXT_NONSECURE, io_str1, io_str2)

#define vlbinstr_Trunc(io_str, in_newlen) \
    npi_vlbinstr_Trunc(NPI_CMN_CONTEXT_NONSECURE, io_str, in_newlen)

#define vlbinstr_CmpFun(in_str1, in_str2) \
    npi_vlbinstr_CmpFun(NPI_CMN_CONTEXT_NONSECURE, in_str1, in_str2)

#define vlbinstr_Cmp(in_str1, in_str2) \
    npi_vlbinstr_Cmp(NPI_CMN_CONTEXT_NONSECURE, in_str1, in_str2)

#define vlbinstr_Len(in_str) npi_vlbinstr_Len(in_str)
#define vlbinstr_Buf(in_str) npi_vlbinstr_Buf(in_str)

#define vlbinstr_Alloc(in_size, in_buf) \
    npi_vlbinstr_Alloc(NPI_CMN_CONTEXT_NONSECURE, in_size, in_buf)

#define vlbinstr_AllocFill(in_size, in_byte) \
    npi_vlbinstr_AllocFill(NPI_CMN_CONTEXT_NONSECURE, in_size, in_byte)

#define vlbinstr_Free(io_str) \
    npi_vlbinstr_Free(NPI_CMN_CONTEXT_NONSECURE, io_str)

#define vlbinstr_Dump(in_str)   npi_vlbinstr_Dump(in_str)
#define vlbinstr_Dump0(in_str)  npi_vlbinstr_Dump0(in_str)
#define vlbinstr_Dump1(in_str)  npi_vlbinstr_Dump1(in_str)
#define vlbinstr_Dump2(in_str)  npi_vlbinstr_Dump2(in_str)
#define vlbinstr_Dump3(in_str)  npi_vlbinstr_Dump3(in_str)

#define vlbinstr_Print(in_str)  npi_vlbinstr_Print(in_str)
#define vlbinstr_Print0(in_str) npi_vlbinstr_Print0(in_str)
#define vlbinstr_Print1(in_str) npi_vlbinstr_Print1(in_str)
#define vlbinstr_Print2(in_str) npi_vlbinstr_Print2(in_str)
#define vlbinstr_Print3(in_str) npi_vlbinstr_Print3(in_str)


/**
 * xml_api.h
 */
#define xml_ParseBuf(in_xmltextlen, in_xmltext, out_xmlnode) \
    npi_xml_ParseBuf(NPI_CMN_CONTEXT_NONSECURE, in_xmltextlen, in_xmltext, out_xmlnode)

#define xml_Parse(t, n) \
    npi_xml_Parse(NPI_CMN_CONTEXT_NONSECURE, t, n)

#define xml_ParseAndInsert(in_xmltextlen, in_xmltext, io_parent) \
    npi_xml_ParseAndInsert(NPI_CMN_CONTEXT_NONSECURE, in_xmltextlen, in_xmltext, io_parent)

#define xml_ParseAndReplace(in_xmltextlen, in_xmltext, io_victim, io_parent) \
    npi_xml_ParseAndReplace(NPI_CMN_CONTEXT_NONSECURE, in_xmltextlen, in_xmltext, io_victim, io_parent)

#define xml_ParseInteger(in_xmltextlen, in_xmltext, out_i) \
    npi_xml_ParseInteger(NPI_CMN_CONTEXT_NONSECURE, in_xmltextlen, in_xmltext, out_i)

#define xml_ParseDateTime(in_xmltextlen, in_xmltext, out_time) \
    npi_xml_ParseDateTime(NPI_CMN_CONTEXT_NONSECURE, in_xmltextlen, in_xmltext, out_time)

#define xml_Find(in_xmlnode, in_xpath, in_namespace, out_xmlnodes) \
    npi_xml_Find(NPI_CMN_CONTEXT_NONSECURE, in_xmlnode, in_xpath, in_namespace, out_xmlnodes)

#define xml_FindOne(in_xmlnode, in_xpath, in_namespace, out_xmlnode) \
    npi_xml_FindOne(NPI_CMN_CONTEXT_NONSECURE, in_xmlnode, in_xpath, in_namespace, out_xmlnode)

#define xml_GetTopElement(in_xmlnode, out_topnode) \
    npi_xml_GetTopElement(NPI_CMN_CONTEXT_NONSECURE, in_xmlnode, out_topnode)

#define xml_GetAttr(in_xmlnode, in_namespace, in_attrname, out_attrvalue) \
    npi_xml_GetAttr(NPI_CMN_CONTEXT_NONSECURE, in_xmlnode, in_namespace, in_attrname, out_attrvalue)

#define xml_GetContent(in_xmlnode, out_content) \
    npi_xml_GetContent(NPI_CMN_CONTEXT_NONSECURE, in_xmlnode, out_content)

#define xml_GetName(in_xmlnode, out_content) \
    npi_xml_GetName(NPI_CMN_CONTEXT_NONSECURE, in_xmlnode, out_content)

#define xml_ExclC14N(in_xmlnode, out_xmltext) \
    npi_xml_ExclC14N(NPI_CMN_CONTEXT_NONSECURE, in_xmlnode, out_xmltext)

#define xml_ToString(in_xmlnode, out_xmltext) \
    npi_xml_ToString(NPI_CMN_CONTEXT_NONSECURE, in_xmlnode, out_xmltext)

#define xml_CopyNode(io_node) \
    npi_xml_CopyNode(NPI_CMN_CONTEXT_NONSECURE, io_node)

#define xml_FreeNode(io_node) \
    npi_xml_FreeNode(NPI_CMN_CONTEXT_NONSECURE, io_node)

#define xml_FreeNodeVec(io_nodevec) \
    npi_xml_FreeNodeVec(NPI_CMN_CONTEXT_NONSECURE, io_nodevec)

#define xml_NodeVecSize(nodevec)     npi_xml_NodeVecSize(nodevec)
#define xml_NodeVecItem(nodevec,idx) npi_xml_NodeVecItem(nodevec,idx)

#define xml_ComposeElement(in_name, in_namespacetag, in_namespaceuri, in_nattrs, in_attrs, in_nconts, in_conts, in_freef, out_node) \
npi_xml_ComposeElement(NPI_CMN_CONTEXT_NONSECURE, in_name, in_namespacetag, in_namespaceuri, in_nattrs, in_attrs, in_nconts, in_conts, in_freef, out_node)

#define xml_AddAttribute(io_node, in_name, in_value) \
    npi_xml_AddAttribute(NPI_CMN_CONTEXT_NONSECURE, io_node, in_name, in_value)

#define xml_AddText(io_node, in_text) \
    npi_xml_AddText(NPI_CMN_CONTEXT_NONSECURE, io_node, in_text)

#define xml_ReplaceText(io_node, in_text) \
    npi_xml_ReplaceText(NPI_CMN_CONTEXT_NONSECURE, io_node, in_text)

#define xml_AddElement(io_node, io_child, in_freef) \
        npi_xml_AddElement(NPI_CMN_CONTEXT_NONSECURE, io_node, io_child, in_freef)

#define xml_OutputComposedNode(in_node, in_version, in_encoding, out_str) \
        npi_xml_OutputComposedNode(NPI_CMN_CONTEXT_NONSECURE, in_node, in_version, in_encoding, out_str)

#define xml_OutputComposedNodes(in_count, in_nodes, out_str) \
        npi_xml_OutputComposedNodes(NPI_CMN_CONTEXT_NONSECURE, in_count, in_nodes, out_str)

#define xml_RemoveContent(io_parent, io_child) \
        npi_xml_RemoveContent(NPI_CMN_CONTEXT_NONSECURE, io_parent, io_child)

#define xml_GetElementFromRoot(in_root_node, out_element_node) \
        npi_xml_GetElementFromRoot(NPI_CMN_CONTEXT_NONSECURE, in_root_node, out_element_node )


/**
 *  * str_api.h
 *   */
#define str_NCopy(io_buf1, in_buf2, in_n)    npi_str_NCopy(io_buf1, in_buf2, in_n)
#define str_Len(in_buf)                      npi_str_Len(in_buf)
#define str_NCat(io_buf1, in_buf2, in_n)     npi_str_NCat(io_buf1, in_buf2, in_n)
#define str_Cmp(in_buf1, in_buf2)            npi_str_Cmp(in_buf1, in_buf2)
#define str_NCmp(in_buf1, in_buf2, in_n)     npi_str_NCmp(in_buf1, in_buf2, in_n)
#define str_Str(in_str1, in_str2)            npi_str_Str(in_str1, in_str2)
#define str_ToL(in_str, out_endptr, in_base) npi_str_ToL(in_str, out_endptr, in_base)
#define str_Chr(in_s, in_c)                  npi_str_Chr(in_s, in_c)
#define str_RChr(in_s, in_c)                 npi_str_RChr(in_s, in_c)
#define str_NCaseCmp(in_str1, in_str2, in_len) \
    npi_str_NCaseCmp(NPI_CMN_CONTEXT_NONSECURE, in_str1, in_str2, in_len)
#define str_Spn(in_s1, in_s2)                npi_str_Spn(in_s1, in_s2)


/**
 * smi_api.h
 */
#define smi_setMsgId(smi, val) \
        npi_smi_setMsgId(smi, val)

#define smi_setMsgSize(smi, val) \
        npi_smi_setMsgSize(smi, val)

#define smi_setMsgRet(smi, val) \
        npi_smi_setMsgRet(smi, val)

#define smi_setMsgResvd(smi, val) \
        npi_smi_setMsgResvd(smi, val)

#define smi_setMsgParamType(smi, val) \
        npi_smi_setMsgParamType(smi, val)

#define smi_setMsgParamSize(smi, val) \
        npi_smi_setMsgParamSize(smi, val)

#define smi_getMsgId(smi) \
        npi_smi_getMsgId(smi)

#define smi_getMsgSize(smi) \
        npi_smi_getMsgSize(smi)

#define smi_getMsgRet(smi) \
        npi_smi_getMsgRet(smi)

#define smi_getMsgResvd(smi) \
        npi_smi_getMsgResvd(smi)

#define smi_getMsgParamType(smi) \
        npi_smi_getMsgParamType(smi)

#define smi_getMsgParamSize(smi) \
        npi_smi_getMsgParamSize(smi)

#define smi_AssertParamsSize(in_params, in_sz) \
        npi_smi_AssertParamsSize(in_params, in_sz)

#define smi_PrepareMsg(in_msg_id, in_msg_ret, in_params_list, out_msg) \
        npi_smi_PrepareMsg(NPI_CMN_CONTEXT_NONSECURE, in_msg_id, in_msg_ret, in_params_list, out_msg)

#define smi_ExtractParamVals(in_msg, out_params_list) \
        npi_smi_ExtractParamVals(NPI_CMN_CONTEXT_NONSECURE, in_msg, out_params_list)

#define smi_params_Alloc(in_nb, out_params_list) \
        npi_smi_params_Alloc(NPI_CMN_CONTEXT_NONSECURE, in_nb, out_params_list)

#define smi_params_Free(in_params_list) \
        npi_smi_params_Free(NPI_CMN_CONTEXT_NONSECURE, in_params_list)

#define smi_ReadParamAsFaultInfo(in_param, out_info) \
        npi_smi_ReadParamAsFaultInfo(NPI_CMN_CONTEXT_NONSECURE, in_param, out_info)

#define smi_ReadParamAsStr(in_param, out_buf) \
        npi_smi_ReadParamAsStr(NPI_CMN_CONTEXT_NONSECURE, in_param, out_buf)

#define smi_ReadParamAsCrlNumber(in_param, out_crl) \
        npi_smi_ReadParamAsCrlNumber(NPI_CMN_CONTEXT_NONSECURE, in_param, out_crl)

#define smi_ReadParamAsInt32(in_param, out_int32) \
        npi_smi_ReadParamAsInt32(NPI_CMN_CONTEXT_NONSECURE, in_param, out_int32)

#define smi_ReadParamAsUInt32(in_param, out_uint32) \
        npi_smi_ReadParamAsUInt32(NPI_CMN_CONTEXT_NONSECURE, in_param, out_uint32)

#define smi_ReadParamAsHandle(in_param, out_hndl) \
        npi_smi_ReadParamAsHandle(NPI_CMN_CONTEXT_NONSECURE, in_param, out_hndl)

#define smi_ReadParamAsUInt8Ptr(in_param, out_uint8_ptr) \
	    npi_smi_ReadParamAsUInt8Ptr(NPI_CMN_CONTEXT_NONSECURE, in_param, out_uint8_ptr)

#define smi_ReadParamAsBool(in_param, out_bool) \
        npi_smi_ReadParamAsBool(NPI_CMN_CONTEXT_NONSECURE, in_param, out_bool)

#define smi_ReadParamAsSize(in_param, out_size) \
        npi_smi_ReadParamAsSize(NPI_CMN_CONTEXT_NONSECURE, in_param, out_size)

#define smi_ReadParamAsDate(in_param, out_date) \
        npi_smi_ReadParamAsDate(NPI_CMN_CONTEXT_NONSECURE, in_param, out_date)

#define smi_ReadParamAsCallback(in_param, out_callback) \
        npi_smi_ReadParamAsCallback(NPI_CMN_CONTEXT_NONSECURE, in_param, out_callback)

#define smi_ReadParamAsCrtStdList(in_param, out_crtstdlist) \
        npi_smi_ReadParamAsCrtStdList(NPI_CMN_CONTEXT_NONSECURE, in_param, out_crtstdlist)

#define smi_ReadParamAsCertStdList(in_param, out_certstdlist) \
        npi_smi_ReadParamAsCertStdList(NPI_CMN_CONTEXT_NONSECURE, in_param, out_certstdlist)

#define smi_ReadParamAsTrackInfo(in_param, out_trackinfo) \
        npi_smi_ReadParamAsTrackInfo(NPI_CMN_CONTEXT_NONSECURE, in_param, out_trackinfo)\

#define smi_ReadParamAsSASInfo(in_param, out_sasinfo) \
        npi_smi_ReadParamAsSASInfo(NPI_CMN_CONTEXT_NONSECURE, in_param, out_sasinfo)

/**
 * esbman_api.h
 */
#define esbman_Parse(in_esbsize,in_esbbuf,out_info)  \
        npi_esbman_Parse(NPI_CMN_CONTEXT_NONSECURE,in_esbsize,in_esbbuf,out_info)

#define esbman_Free(io_info) \
        npi_esbman_Free( NPI_CMN_CONTEXT_NONSECURE,io_info )

#define esbman_VerifyCritical(in_info) \
        npi_esbman_VerifyCritical( NPI_CMN_CONTEXT_NONSECURE,in_info )


/**
 * uri_api.h
 */
#define uri_IsUriTemplatePresent(in_buf, out_bool)                      \
        npi_uri_IsUriTemplatePresent(NPI_CMN_CONTEXT_NONSECURE, in_buf, out_bool)

#define uri_TransformTemplate(in_buf, in_nvars, in_vars, out_str)       \
        npi_uri_TransformTemplate(NPI_CMN_CONTEXT_NONSECURE, in_buf, in_nvars, in_vars, out_str)

#define uri_IsCompoundURI(in_buf, out_bool)                             \
        npi_uri_IsCompoundURI(NPI_CMN_CONTEXT_NONSECURE, in_buf, out_bool)

#define uri_ParseCompoundURI(in_buf, out_left, out_right)               \
        npi_uri_ParseCompoundURI(NPI_CMN_CONTEXT_NONSECURE, in_buf, out_left, out_right)

/**
 * asn_api.h
 */
#define asn_Encode(in_asn, out_buf, out_sz) \
        npi_asn_Encode(NPI_CMN_CONTEXT_NONSECURE, in_asn, out_buf, out_sz)

#define asn_EncodeWoMalloc(in_asn, out_buf, out_sz) \
        npi_asn_EncodeWoMalloc(NPI_CMN_CONTEXT_NONSECURE, in_asn, out_buf, out_sz)

#define asn_Decode(in_buf, in_bufsize, out_next, out_asn) \
        npi_asn_Decode(NPI_CMN_CONTEXT_NONSECURE, in_buf, in_bufsize, out_next, out_asn)

#define asn_DecodeWoCopy(in_buf, in_bufsize, out_next, out_asn) \
        npi_asn_DecodeWoCopy(NPI_CMN_CONTEXT_NONSECURE, in_buf, in_bufsize, out_next, out_asn)

#define asn_DecodeCheck(in_buf, in_bufsize, out_next) \
        npi_asn_DecodeCheck(NPI_CMN_CONTEXT_NONSECURE, in_buf, in_bufsize, out_next)

#define asn_ElemInfoGet(in_elem, in_elem_sz, out_next_elem, out_content_len, out_elem_len) \
        npi_asn_ElemInfoGet(NPI_CMN_CONTEXT_NONSECURE, in_elem, in_elem_sz, out_next_elem, out_content_len, out_elem_len)

#define asn_SetDefinite(io_asn, in_bool) \
        npi_asn_SetDefinite(NPI_CMN_CONTEXT_NONSECURE, io_asn, in_bool)

#define asn_GetDefinite(in_asn) \
        npi_asn_GetDefinite(NPI_CMN_CONTEXT_NONSECURE, in_asn)

#define asn_Clone(in_asn, out_asn) \
        npi_asn_Clone(NPI_CMN_CONTEXT_NONSECURE, in_asn, out_asn)

#define asn_NewEncoded(in_src_size, in_src_addr, out_asn) \
        npi_asn_NewEncoded(NPI_CMN_CONTEXT_NONSECURE, in_src_size, in_src_addr, out_asn)

#define asn_NewRaw(in_class, in_stflag, in_tag, in_length, in_body, out_aval) \
        npi_asn_NewRaw(NPI_CMN_CONTEXT_NONSECURE, in_class, in_stflag, in_tag, in_length, in_body, out_aval)

#define asn_NewStructured(in_class, in_tag, out_aval) \
        npi_asn_NewStructured(NPI_CMN_CONTEXT_NONSECURE, in_class, in_tag, out_aval)

#define asn_NewUnstructured(in_class, in_tag, in_length, in_body, out_aval) \
        npi_asn_NewUnstructured(NPI_CMN_CONTEXT_NONSECURE, in_class, in_tag, in_length, in_body, out_aval)

#define asn_Free(in_asn) \
        npi_asn_Free(NPI_CMN_CONTEXT_NONSECURE, in_asn)

#define asn_EncodeFree(in_buf) \
        npi_asn_EncodeFree(NPI_CMN_CONTEXT_NONSECURE, in_buf)

#define asn_Insert(io_asn, in_index, in_asn) \
        npi_asn_Insert(NPI_CMN_CONTEXT_NONSECURE, io_asn, in_index, in_asn)

#define asn_Append(io_asn, in_asn) \
        npi_asn_Append(NPI_CMN_CONTEXT_NONSECURE, io_asn, in_asn)

#define asn_Remove(io_asn, in_index) \
        npi_asn_Remove(NPI_CMN_CONTEXT_NONSECURE, io_asn, in_index)

#define asn_Attach(io_asn, in_index, in_asn, out_asn) \
        npi_asn_Attach(NPI_CMN_CONTEXT_NONSECURE, io_asn, in_index, in_asn, out_asn)

#define asn_Detach(io_asn, in_index, out_asn) \
        npi_asn_Detach(NPI_CMN_CONTEXT_NONSECURE, io_asn, in_index, out_asn)

#define asn_FindFirst(in_asn, in_class, in_tag) \
        npi_asn_FindFirst(NPI_CMN_CONTEXT_NONSECURE, in_asn, in_class, in_tag)

#define asn_NewInteger(in_int, out_aval) \
        npi_asn_NewInteger(NPI_CMN_CONTEXT_NONSECURE, in_int, out_aval)

#define asn_NewString(in_length, in_body, out_aval) \
        npi_asn_NewString(NPI_CMN_CONTEXT_NONSECURE, in_length, in_body, out_aval)

#define asn_NewEnumerated(in_enum, out_aval) \
        npi_asn_NewEnumerated(NPI_CMN_CONTEXT_NONSECURE, in_enum, out_aval)

#define asn_NewSequence(out_aval) \
        npi_asn_NewSequence(NPI_CMN_CONTEXT_NONSECURE, out_aval)

#define asn_ParseBoolean(in_asn, out_bool) \
        npi_asn_ParseBoolean(NPI_CMN_CONTEXT_NONSECURE, in_asn, out_bool)

#define asn_ParseInteger(in_asn, out_int) \
        npi_asn_ParseInteger(NPI_CMN_CONTEXT_NONSECURE, in_asn, out_int)

#define asn_ParseUnsignedInteger(in__asn, in_bufsize, out_buf) \
        npi_asn_ParseUnsignedInteger(NPI_CMN_CONTEXT_NONSECURE, in__asn, in_bufsize, out_buf)

#define asn_ParseBitstring(in_asn, in_buf_sz, out_buf, out_bitsz) \
        npi_asn_ParseBitstring(NPI_CMN_CONTEXT_NONSECURE, in_asn, in_buf_sz, out_buf, out_bitsz)

#define asn_ParseString(in_asn, out_string, out_string_size) \
        npi_asn_ParseString(NPI_CMN_CONTEXT_NONSECURE, in_asn, out_string, out_string_size)

#define asn_ParseEnumerated(in_asn, out_enum) \
        npi_asn_ParseEnumerated(NPI_CMN_CONTEXT_NONSECURE, in_asn, out_enum)

#define asn_ParseUTCTime(in_asn, in_year19xx, out_time) \
        npi_asn_ParseUTCTime(NPI_CMN_CONTEXT_NONSECURE, in_asn, in_year19xx, out_time)

#define asn_ParseGeneralizedTime(in_asn, out_time) \
        npi_asn_ParseGeneralizedTime(NPI_CMN_CONTEXT_NONSECURE, in_asn, out_time)

#define asn_ParseUTCTime_Ex(in_asn, in_year19xx, out_date) \
        npi_asn_ParseUTCTime_Ex(NPI_CMN_CONTEXT_NONSECURE, in_asn, in_year19xx, out_date)

#define asn_ParseGeneralizedTime_Ex(in_asn, out_date) \
        npi_asn_ParseGeneralizedTime_Ex(NPI_CMN_CONTEXT_NONSECURE, in_asn, out_date)

#define asn_SetImplicit(in_tag, io_aval) \
        npi_asn_SetImplicit(NPI_CMN_CONTEXT_NONSECURE, in_tag, io_aval)

#define asn_NewExplicit(in_tag, in_aval, out_aval) \
        npi_asn_NewExplicit(NPI_CMN_CONTEXT_NONSECURE, in_tag, in_aval, out_aval)

#define asn_ParseStructured(in_buf, in_size, io_asn, in_num, out_num) \
        npi_asn_ParseStructured(NPI_CMN_CONTEXT_NONSECURE, in_buf, in_size, io_asn, in_num, out_num)

#define asn_Dump(in_asn) \
        npi_asn_Dump(NPI_CMN_CONTEXT_NONSECURE, in_asn)


NP_END_EXTERN_C


#endif /* CMN_API_NSEC_H */
