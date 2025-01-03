/*
 * imx-dcx81.c  --  SoC audio for imx based boards with dcx81 codec
 *
 * Copyright 2018 Torben Hohn <torben.hohn@linutronix.de>
 *
 * Based on imx-mx13783:
 * Copyright 2012 Philippe Retornaz, <philippe.retornaz@epfl.ch>
 *
 * Heavly based on phycore-mc13783:
 * Copyright 2009 Sascha Hauer, Pengutronix <s.hauer@pengutronix.de>
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/device.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/soc.h>
#include <sound/soc-dapm.h>
#include <asm/mach-types.h>

#include "imx-ssi.h"
#include "imx-audmux.h"

#define FMT_SSI (SND_SOC_DAIFMT_DSP_A | SND_SOC_DAIFMT_NB_NF | \
		SND_SOC_DAIFMT_CBS_CFS)

static int imx_dcx81_hifi_hw_params(struct snd_pcm_substream *substream,
	struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	int ret;

	/* setting the clk freq to 1 enables the half_rate_hack
	 */
	ret = snd_soc_dai_set_sysclk(cpu_dai, 0, 1, SND_SOC_CLOCK_OUT);
	if (ret) {
		pr_err("failed to set cpu sysclk: %d\n", ret);
		return ret;
	}

	/*
	 * for unknown reasons, the codec wants 16 slots.
	 *
	 * the datastream has 2 frames in the first 2 slots.
	 */
	ret = snd_soc_dai_set_tdm_slot(cpu_dai, 0x3, 0x3, 16, 16);
	if (ret)
		return ret;

	return 0;
}

static struct snd_soc_ops imx_dcx81_hifi_ops = {
	.hw_params = imx_dcx81_hifi_hw_params,
};

static struct snd_soc_dai_link imx_dcx81_dai_dcx81[] = {
	{
		.name = "Dect-Voice",
		.stream_name	 = "HiFi",
		.codec_dai_name	 = "dcx81-hifi",
		.codec_name	 = "dcx81",
		.cpu_dai_name	 = "308a0000.sai",
		.platform_name	 = "308a0000.sai",
		.ops		 = &imx_dcx81_hifi_ops,
		.symmetric_rates = 1,
		.dai_fmt	 = FMT_SSI,
	},
};

static struct snd_soc_card imx_dcx81 = {
	.name		= "DECT-Voice",
	.owner		= THIS_MODULE,
	.dai_link	= imx_dcx81_dai_dcx81,
	.num_links	= ARRAY_SIZE(imx_dcx81_dai_dcx81),
};

static int imx_dcx81_probe(struct platform_device *pdev)
{
	int ret;

	imx_dcx81.dev = &pdev->dev;

	ret = snd_soc_register_card(&imx_dcx81);
	if (ret) {
		dev_err(&pdev->dev, "snd_soc_register_card failed (%d)\n",
			ret);
		return ret;
	}


	return ret;
}

static int imx_dcx81_remove(struct platform_device *pdev)
{
	snd_soc_unregister_card(&imx_dcx81);

	return 0;
}

static const struct of_device_id imx_dcx81_dt_ids[] = {
	{ .compatible = "fsl,imx-audio-dcx81", },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, imx_dcx81_dt_ids);

static struct platform_driver imx_dcx81_audio_driver = {
	.driver = {
		.name = "imx_dcx81",
		.of_match_table = imx_dcx81_dt_ids,
	},
	.probe = imx_dcx81_probe,
	.remove = imx_dcx81_remove
};

module_platform_driver(imx_dcx81_audio_driver);

MODULE_AUTHOR("Sascha Hauer <s.hauer@pengutronix.de>");
MODULE_AUTHOR("Philippe Retornaz <philippe.retornaz@epfl.ch");
MODULE_AUTHOR("Torben Hohn <torben.hohn@linutronix.de>");
MODULE_DESCRIPTION("imx with dcx81 codec ALSA SoC driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:imx_dcx81");
