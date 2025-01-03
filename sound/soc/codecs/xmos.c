/*
 * xmos.c  --  XMOS ALSA SoC Audio driver
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/pm.h>
#include <linux/clk.h>
#include <linux/i2c.h>
#include <linux/slab.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/initval.h>
#include <sound/tlv.h>
#include <sound/xmos.h>

#include "xmos.h"

struct xmos_priv {
	struct clk *mclk;
	struct regmap *regmap;
	int (*set_bias_level)(struct snd_soc_codec *,
			      enum snd_soc_bias_level level);

	int lrclk;
	int bclk;
	int spdif_mute;

	struct i2c_client *i2c;
	uint8_t i2c_cache[0x16+1];
	uint8_t i2c_unwritten[0x16+1];
	uint8_t i2c_unwritten_state[0x16+1];
};

static const DECLARE_TLV_DB_SCALE(inpga_tlv, -1725, 75, 0);
static const DECLARE_TLV_DB_MINMAX(dac_tlv, -6000, 0);

static int xmos_write_i2c (struct xmos_priv *priv, uint8_t reg, uint8_t val)
{
	int ret;
	if (priv->lrclk == 0)
	{
		printk ("xmos: param not written, lrclock == 0\n");
		priv->i2c_unwritten[reg] = val;
		priv->i2c_unwritten_state[reg] = 1;
		ret = 0;
	}
	else
	{
		uint8_t cmd[] = {0x42, 0x00, 0x0c, reg, 0x00, 0x00, 0x00, val, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00};
		ret = i2c_master_send(priv->i2c, cmd, 15);
		priv->i2c_cache[reg] = val;
	}

	if (ret >= 0)
		return 0;

	printk ("xmos: write failed reg=%d, val=%d, ret=%d\n", reg, val, ret);
	return ret;
}


static int xmos_get_volume (struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_codec *codec = snd_soc_kcontrol_codec(kcontrol);
	struct xmos_priv *priv = snd_soc_codec_get_drvdata(codec);

	ucontrol->value.integer.value[0] = priv->i2c_cache[XMOS_DSP_DAC_VOL];

	return 0;
}

static int xmos_set_volume (struct snd_kcontrol *kcontrol,
			      struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_codec *codec = snd_soc_kcontrol_codec(kcontrol);
	struct xmos_priv *priv = snd_soc_codec_get_drvdata(codec);
	unsigned int val = ucontrol->value.enumerated.item[0];
	int ret = 0;

	/*
	 * pr_debug("%s: val=%d id=%d name=%s index=%d\n",
	 *	 __func__, val, ucontrol->id.numid, ucontrol->id.name,
	 *	 ucontrol->id.index);
	 */

	if (strstr(ucontrol->id.name, "Master Playback Volume"))
	{
		ret = xmos_write_i2c (priv, XMOS_DSP_DAC_VOL, val);
		if (ret == 0)
			ret = xmos_write_i2c (priv, XMOS_DSP_SPDIF_VOL, val);
	}
	else
		printk ("xmos: unknown control: %s\n", ucontrol->id.name);

	return ret;
}

static int xmos_get_switch (struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_codec *codec = snd_soc_kcontrol_codec(kcontrol);
	struct xmos_priv *priv = snd_soc_codec_get_drvdata(codec);

	if (strstr(ucontrol->id.name, "Master Playback Switch"))
	{
		ucontrol->value.integer.value[0] = !priv->i2c_cache[XMOS_DSP_DAC_MUTE];
	}
	else if (strstr(ucontrol->id.name, "Headphone Playback Switch"))
	{
		ucontrol->value.integer.value[0] = priv->i2c_cache[XMOS_DSP_HEADP_ONOFF];
	}
	else if (strstr(ucontrol->id.name, "Speaker Playback Switch"))
	{
		ucontrol->value.integer.value[0] = priv->i2c_cache[XMOS_DSP_SPEAK_ONOFF];
	}
	else if (strstr(ucontrol->id.name, "SPDIF Playback Switch"))
	{
		ucontrol->value.integer.value[0] = !priv->spdif_mute;
	}
	else if (strstr(ucontrol->id.name, "Master Capture Switch"))
	{
		ucontrol->value.integer.value[0] = !priv->i2c_cache[XMOS_DSP_MIC_MUTE];
	}
	else
	{
		printk ("xmos: get switch unknown control: %s\n", ucontrol->id.name);
		return 0;
	}
	//~ pr_debug ("xmos_get_switch() val=%d id=%d name=%s \n", ucontrol->value.integer.value[0], ucontrol->id.numid, ucontrol->id.name);

	return 0;
}

static int xmos_set_switch (struct snd_kcontrol *kcontrol,
			      struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_codec *codec = snd_soc_kcontrol_codec(kcontrol);
	struct xmos_priv *priv = snd_soc_codec_get_drvdata(codec);
	unsigned int val = ucontrol->value.enumerated.item[0];
	int ret = 0;

	//~ printk ("xmos_set_switch() val=%d id=%d name=%s index=%d\n",
	//~	    val, ucontrol->id.numid, ucontrol->id.name, ucontrol->id.index);

	if (strstr(ucontrol->id.name, "Master Playback Switch"))
	{
		ret = xmos_write_i2c (priv, XMOS_DSP_DAC_MUTE, !val);
		if (ret == 0)
			return xmos_write_i2c (priv, XMOS_DSP_SPDIF_MUTE,
					       (!val) | priv->spdif_mute);
	}
	else if (strstr(ucontrol->id.name, "Headphone Playback Switch"))
	{
		return xmos_write_i2c (priv, XMOS_DSP_HEADP_ONOFF, val);
	}
	else if (strstr(ucontrol->id.name, "Speaker Playback Switch"))
	{
		return xmos_write_i2c (priv, XMOS_DSP_SPEAK_ONOFF, val);
	}
	else if (strstr(ucontrol->id.name, "SPDIF Playback Switch"))
	{
		priv->spdif_mute = !val;
		return xmos_write_i2c (priv, XMOS_DSP_SPDIF_MUTE,
				       priv->i2c_cache[XMOS_DSP_DAC_MUTE] |
				       priv->spdif_mute);
	}
	else if (strstr(ucontrol->id.name, "Master Capture Switch"))
	{
		return xmos_write_i2c (priv, XMOS_DSP_MIC_MUTE, !val);
	}
	else
		printk ("xmos: unknown control\n");

	return ret;
}

static int xmos_get_dspmode (struct snd_kcontrol *kcontrol,
		struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_codec *codec = snd_soc_kcontrol_codec(kcontrol);
	struct xmos_priv *priv = snd_soc_codec_get_drvdata(codec);

	ucontrol->value.integer.value[0] = priv->i2c_cache[XMOS_DSP_MODE];

	/*
	 * pr_debug("%s: val=%ld id=%d name=%s index=%d\n", __func__,
	 *	 ucontrol->value.integer.value[0], ucontrol->id.numid,
	 *	 ucontrol->id.name, ucontrol->id.index);
	 */

	return 0;
}

static int xmos_set_dspmode (struct snd_kcontrol *kcontrol,
		struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_codec *codec = snd_soc_kcontrol_codec(kcontrol);
	struct xmos_priv *priv = snd_soc_codec_get_drvdata(codec);
	//unsigned int val = ucontrol->value.enumerated.item[0];
	int ret = 0;
	uint8_t mode = ucontrol->value.integer.value[0];

	/*
	 * pr_debug("%s: val=%ld id=%d name=%s index=%d\n", __func__,
	 *	 ucontrol->value.integer.value[0], ucontrol->id.numid,
	 *	 ucontrol->id.name, ucontrol->id.index);
	 */

	ret = xmos_write_i2c (priv, XMOS_DSP_MODE, mode);
	if (ret == 0)
		priv->i2c_cache[XMOS_DSP_MODE] = ucontrol->value.integer.value[0];

	return ret;
}

static const char *xmos_dspmode_text[] = {
	"Mute", "Speaker_Music", "Speaker_Telephony", "Speaker_TTS", "Headphone", "Unused_5", "Unused_6", "Unused_7"
};

static SOC_ENUM_SINGLE_DECL(xmos_dspmode_enum,
			    XMOS_DSP_MODE, 0,
			    xmos_dspmode_text);

static const struct snd_kcontrol_new xmos_snd_controls[] = {
	SOC_SINGLE_EXT_TLV("Master Playback Volume", XMOS_DAC,0, 255, 0, xmos_get_volume, xmos_set_volume, dac_tlv),
	SOC_SINGLE_EXT("Master Playback Switch", XMOS_DACCTL1, 0, 1, 0, xmos_get_switch, xmos_set_switch),
	SOC_SINGLE_EXT("Speaker Playback Switch", XMOS_DACCTL1, 0, 1, 0, xmos_get_switch, xmos_set_switch),
	SOC_SINGLE_EXT("Headphone Playback Switch", XMOS_DACCTL1, 0, 1, 0, xmos_get_switch, xmos_set_switch),
	SOC_SINGLE_EXT("SPDIF Playback Switch", XMOS_DACCTL1, 0, 1, 0, xmos_get_switch, xmos_set_switch),
	SOC_SINGLE_EXT("Master Capture Switch", XMOS_DACCTL1, 0, 1, 0, xmos_get_switch, xmos_set_switch),
	SOC_ENUM_EXT("DSP Modes", xmos_dspmode_enum, xmos_get_dspmode, xmos_set_dspmode),
 };

static int xmos_set_dai_fmt(struct snd_soc_dai *codec_dai,
		unsigned int fmt)
{
	return 0;
}

static int xmos_hw_params(struct snd_pcm_substream *substream,
			    struct snd_pcm_hw_params *params,
			    struct snd_soc_dai *dai)
{
	struct snd_soc_codec *codec = dai->codec;
	struct xmos_priv *xmos = snd_soc_codec_get_drvdata(codec);
	// bool tx = substream->stream == SNDRV_PCM_STREAM_PLAYBACK;
	// int i;

	xmos->bclk = snd_soc_params_to_bclk(params);
	if (params_channels(params) == 1)
		xmos->bclk *= 2;

	/* bit size */
	switch (params_width(params)) {
	case 32:
		break;
	default:
		dev_err(codec->dev, "unsupported width %d\n",
			params_width(params));
		return -EINVAL;
	}

	xmos->lrclk = params_rate(params);

	// geht ja noch nicht .. keine Clock ohne playback
	//~ for (i=0; i<sizeof(xmos->i2c_cache); i++)
	//~ {
		//~ if (xmos->i2c_unwritten_state[i])
		//~ {
			//~ if (xmos_write_i2c(xmos, i, xmos->i2c_unwritten[i]) < 0 )
				//~ printk ("xmos: failed to rewrite param %d\n", i);
			//~ else
				//~ printk ("xmos: rewrote param %d\n", i);
		//~ }
	//~ }

	printk("xmos lrclk: %d\n", xmos->lrclk);

	return 0;
}

static int xmos_hw_free(struct snd_pcm_substream *substream,
		struct snd_soc_dai *dai)
{
	struct snd_soc_codec *codec = dai->codec;
	struct xmos_priv *xmos = snd_soc_codec_get_drvdata(codec);

	xmos->lrclk = 0;

	return 0;
}

static int xmos_mute(struct snd_soc_dai *dai, int mute)
{
	//~ struct snd_soc_codec *codec = dai->codec;
	//~ struct xmos_priv *priv = snd_soc_codec_get_drvdata(codec);
	//~ int ret;

	//~ ret = xmos_write_i2c (priv, XMOS_DSP_DAC_MUTE, mute);
	//~ if (ret == 0)
		//~ return xmos_write_i2c (priv, XMOS_DSP_SPDIF_MUTE, mute);
	//~ return ret;
	return 0;
}

static int xmos_set_bias_level(struct snd_soc_codec *codec,
				 enum snd_soc_bias_level level)
{
	//Not implemented
	return 0;
}

#define XMOS_RATES SNDRV_PCM_RATE_48000

#define XMOS_FORMATS \
	(SNDRV_PCM_FMTBIT_S32_LE)

static const struct snd_soc_dai_ops xmos_dai_ops = {
	.hw_params = xmos_hw_params,
	.hw_free = xmos_hw_free,
	.digital_mute = xmos_mute,

	.set_fmt = xmos_set_dai_fmt,
};

static struct snd_soc_dai_driver xmos_dai = {
	.name = "xmos-hifi",
	.playback = {
		.stream_name = "Playback",
		.channels_min = 2,
		.channels_max = 2,
		.rates = XMOS_RATES,
		.formats = XMOS_FORMATS,},
	.capture = {
		.stream_name = "Capture",
		.channels_min = 2,
		.channels_max = 2,
		.rates = XMOS_RATES,
		.formats = XMOS_FORMATS,},
	.ops = &xmos_dai_ops,
	.symmetric_rates = 1,
};

static int xmos_probe(struct snd_soc_codec *codec)
{
	snd_soc_add_codec_controls(codec, xmos_snd_controls,
				   ARRAY_SIZE(xmos_snd_controls));

	return 0;
}

static struct snd_soc_codec_driver soc_codec_dev_xmos = {
	.probe =	xmos_probe,
	.set_bias_level = xmos_set_bias_level,
	.suspend_bias_off = true,
};

static int xmos_i2c_probe(struct i2c_client *i2c,
			    const struct i2c_device_id *id)
{
	//struct xmos_data *pdata = dev_get_platdata(&i2c->dev);
	struct xmos_priv *xmos;
	int ret, i;

	xmos = devm_kzalloc(&i2c->dev, sizeof(struct xmos_priv),
			      GFP_KERNEL);
	if (xmos == NULL)
		return -ENOMEM;

	xmos->mclk = devm_clk_get(&i2c->dev, "mclk");
	if (IS_ERR(xmos->mclk)) {
		if (PTR_ERR(xmos->mclk) == -EPROBE_DEFER)
			return -EPROBE_DEFER;
	}

	i2c_set_clientdata(i2c, xmos);

	xmos->i2c = i2c;

	for (i=0; i<sizeof(xmos->i2c_cache); i++)
	{
		xmos->i2c_cache[i] = 0;
		xmos->i2c_unwritten[i] = 0;
		xmos->i2c_unwritten_state[i] = 0;
	}

	xmos->i2c_cache[XMOS_DSP_DAC_MUTE] = 0;
	xmos->i2c_cache[XMOS_DSP_DAC_VOL] = 255;
	xmos->i2c_cache[XMOS_DSP_SPEAK_ONOFF] = 1;
	xmos->i2c_cache[XMOS_DSP_HEADP_ONOFF] = 1;
	xmos->i2c_cache[XMOS_DSP_MODE] = 1;
	xmos->spdif_mute = 0;

	ret = snd_soc_register_codec(&i2c->dev,
			&soc_codec_dev_xmos, &xmos_dai, 1);

	printk ("i2c probe %d", ret);

	return ret;
}

static int xmos_i2c_remove(struct i2c_client *client)
{
	snd_soc_unregister_codec(&client->dev);
	return 0;
}

static const struct i2c_device_id xmos_i2c_id[] = {
	{ "xmos", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, xmos_i2c_id);

static const struct of_device_id xmos_of_match[] = {
       { .compatible = "ser,xmos", },
       { }
};
MODULE_DEVICE_TABLE(of, xmos_of_match);

static struct i2c_driver xmos_i2c_driver = {
	.driver = {
		.name = "xmos",
		.owner = THIS_MODULE,
		.of_match_table = xmos_of_match,
	},
	.probe =    xmos_i2c_probe,
	.remove =   xmos_i2c_remove,
	.id_table = xmos_i2c_id,
};

module_i2c_driver(xmos_i2c_driver);

MODULE_DESCRIPTION("Sercomm XMOS3000 driver");
MODULE_AUTHOR("Colin Gu");
MODULE_LICENSE("GPL");
