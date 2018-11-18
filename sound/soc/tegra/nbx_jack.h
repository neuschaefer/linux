/*
 * Copyright (C) 2011-2012 Sony Corporation
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2, as
 * published by the Free Software Foundation.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef __NBX_JACK_H__
#define __NBX_JACK_H__

#include <sound/soc.h>

struct nbx_jack_device_t {
    struct snd_soc_codec *codec;
    int plug_det_gpio;
    int codec_hp_det_gpio;
    int codec_short_gpio;
};

extern int nbx_jack_dev_register(struct nbx_jack_device_t *);
extern void nbx_jack_dev_unregister(void);
extern int nbx_jack_resume(void);
extern int nbx_jack_suspend(void);

#endif /* __NBX_JACK_H__ */
