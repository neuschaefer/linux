/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (C) 2018 BayLibre, SAS
 * Author: Maxime Jourdan <mjourdan@baylibre.com>
 */

#ifndef __MESON_VDEC_VDEC_1_H_
#define __MESON_VDEC_VDEC_1_H_

#include "vdec.h"

int vdec_1_start(struct amvdec_core *core);
int vdec_1_stop(struct amvdec_core *core);
int vdec_1_load_program(struct amvdec_core *core, void __user *program, size_t size);

extern struct amvdec_ops vdec_1_ops;

#endif
