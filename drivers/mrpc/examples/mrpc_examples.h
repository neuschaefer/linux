/*
 *
 * mrpc_examples.h
 * Description:
 * MRPC examples header file
 *
 *
 * GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2016 Intel Corporation.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of version 2 of the GNU General Public License as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *  The full GNU General Public License is included in this distribution
 *  in the file called LICENSE.GPL.
 *
 *  Contact Information:
 *  Intel Corporation
 *  2200 Mission College Blvd.
 *  Santa Clara, CA  97052
 */

#ifndef __HW_MRPC_EXAMPLES_H__
#define __HW_MRPC_EXAMPLES_H__

/**
 * example macro to define a procedure
 */
#define PROC(proc, func)                              \
[proc] = {                                            \
        .procid   = proc,                             \
		.p_func	  = func,   						  \
        .p_arglen = proc##_ARG_LEN,                   \
        .p_replen = proc##_REP_LEN,                   \
        .p_name   = #func,                            \
}

/**
 * demo procedures enumeration
 */
enum {
    DEMO_PROC_0 = 0,
    DEMO_PROC_1,
    DEMO_PROC_2,
    DEMO_PROC_3,
    DEMO_PROC_4
};

/**
 * demo2 procedure argument structure
 */
struct demo2_arg_struct {
    char str1[30];
    int a;
    struct {
        unsigned long x;
        char str2[10];
    };
};

/**
 * demo2 procedure reply structure
 */
struct demo2_rep_struct {
    unsigned int b;
    char str1[30];
    struct {
        short y;
        char str2[26];
    };
};

/**
 * demo procedures arguments and reply length definitions
 */
#define DEMO_PROC_0_ARG_LEN 0
#define DEMO_PROC_0_REP_LEN 0
#define DEMO_PROC_1_ARG_LEN sizeof(unsigned int)
#define DEMO_PROC_1_REP_LEN sizeof(unsigned int)
#define DEMO_PROC_2_ARG_LEN sizeof(struct demo2_arg_struct)
#define DEMO_PROC_2_REP_LEN sizeof(struct demo2_rep_struct)
#define DEMO_PROC_3_ARG_LEN 0
#define DEMO_PROC_3_REP_LEN 0
#define DEMO_PROC_4_ARG_LEN sizeof(unsigned int)
#define DEMO_PROC_4_REP_LEN -1

#endif
