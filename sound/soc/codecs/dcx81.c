/*
 * dcx81.c  --  DCX81 ALSA SoC Audio driver
 *
 * Copyright 2018 Linutronix GmbH
 *
 * Author: Philipp Rosenberger <p.rosenberger@linutronix.de>
 *         Torben Hohn <torben.hohn@linutornix.de>
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/pm.h>
#include <linux/clk.h>
#include <linux/slab.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/initval.h>
#include <sound/tlv.h>


static const struct snd_soc_dai_ops dcx81_dai_ops = {
	/*
	 * the codec only supports a single
	 * audio setting, and that is ensured via
	 * the DAI constraints.
	 *
	 * no dai_ops necessary.
	 */
};

static struct snd_soc_dai_driver dcx81_dai = {
	.name = "dcx81-hifi",
	.playback = {
		.stream_name = "Playback",
		.channels_min = 1,
		.channels_max = 1,
		.rates = SNDRV_PCM_RATE_16000,
		.formats = SNDRV_PCM_FMTBIT_S16_LE,},
	.capture = {
		.stream_name = "Capture",
		.channels_min = 1,
		.channels_max = 1,
		.rates = SNDRV_PCM_RATE_16000,
		.formats = SNDRV_PCM_FMTBIT_S16_LE,},
	.ops = &dcx81_dai_ops,
};

static int dcx81_probe(struct snd_soc_codec *codec)
{
	return 0;
}

static const struct snd_soc_codec_driver soc_codec_dev_dcx81 = {
	/*
	 * the codec only supports a single
	 * audio setting, and that is ensured via
	 * the DAI constraints.
	 *
	 * provide just an empty probe function.
	 */
	.probe =	dcx81_probe,
};

static int dcx81_platform_probe(struct platform_device *pdev)
{
	return snd_soc_register_codec(&pdev->dev,
				      &soc_codec_dev_dcx81,
				      &dcx81_dai,
				      1);
}

static int dcx81_platform_remove(struct platform_device *pdev)
{
	snd_soc_unregister_codec(&pdev->dev);
	return 0;
}

static const struct of_device_id dcx81_of_match[] = {
       { .compatible = "dsp,dcx81", },
       { }
};
MODULE_DEVICE_TABLE(of, dcx81_of_match);

static struct platform_driver dcx81_platform_driver = {
	.driver = {
		.name = "dcx81",
		.of_match_table = dcx81_of_match,
	},
	.probe =    dcx81_platform_probe,
	.remove =   dcx81_platform_remove,
};

module_platform_driver(dcx81_platform_driver);

MODULE_DESCRIPTION("ASoC DCX81 driver");
MODULE_AUTHOR("Philipp Rosenberger");
MODULE_AUTHOR("Torben Hohn <torben.hohn@linutronix.de>");
MODULE_LICENSE("GPL");
