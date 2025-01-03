/*
 * Copyright (C) 2015-2016 Freescale Semiconductor, Inc.
 *
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */

#include <linux/module.h>
#include <linux/of_platform.h>
#include <linux/i2c.h>
#include <linux/of_gpio.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/clk.h>
#include <sound/soc.h>
#include <sound/jack.h>
#include <sound/control.h>
#include <sound/pcm_params.h>
#include <sound/soc-dapm.h>
#include <linux/pinctrl/consumer.h>
#include <linux/mfd/syscon.h>
#include "../codecs/xmos.h"
#include "fsl_sai.h"
#include <linux/delay.h>

/* SoC machine driver */
struct imx_xmos_data {
	struct snd_soc_card card;
	struct clk *codec_clk;
	unsigned int clk_frequency;
	bool is_stream_in_use[2];
	bool is_stream_opened[2];
	struct regmap *gpr;
	unsigned int hp_det[2];
	u32 asrc_rate;
	u32 asrc_format;
};

struct imx_priv {
	struct platform_device *pdev;
	struct platform_device *asrc_pdev;
	struct snd_card *snd_card;
};

static struct imx_priv card_priv;

static void sc_rst_xmos(void)
{
	int err;
	int sc_rs_gpio = 60;

	gpio_free(sc_rs_gpio);
	if (!gpio_is_valid(sc_rs_gpio))
	{
		printk("Must sc_rs_gpio  GPIO to use regulator support\n");
	}
	err = gpio_request_one(sc_rs_gpio, GPIOF_OUT_INIT_HIGH, "rst_xmos");
	if (err<0)
		printk("unable to request gpio [%d]\n",sc_rs_gpio);
	gpio_direction_output(sc_rs_gpio, 1);
	gpio_direction_output(sc_rs_gpio, 0);
	mdelay(500);
	gpio_direction_output(sc_rs_gpio, 1);
	mdelay(50);
	gpio_free(sc_rs_gpio);
	}

static void sc_set_input(void)
{
	int err;
	int sc_cs_gpio = 46;
	int sc_ck_gpio = 45;

	gpio_free(sc_cs_gpio);
	gpio_free(sc_ck_gpio);

	if (!gpio_is_valid(sc_cs_gpio))
	{
		printk("Must sc_cs_gpio  GPIO to use regulator support\n");
	}
	err = gpio_request_one(sc_cs_gpio, GPIOF_IN, "cs_enable");
	if (err<0)
		printk("unable to request gpio [%d]\n",sc_cs_gpio);
	if(gpio_direction_input(sc_cs_gpio))
		printk("unable to request gpio input [%d]\n",sc_cs_gpio);
	if (!gpio_is_valid(sc_ck_gpio))
	{
		printk("Must sc_ck_gpio  GPIO to use regulator support\n");
	}
	err = gpio_request_one(sc_ck_gpio, GPIOF_IN, "ck_enable");
	if (err<0)
		printk("unable to request gpio [%d]\n",sc_ck_gpio);
	if(gpio_direction_input(sc_ck_gpio))
		printk("unable to request gpio input [%d]\n",sc_ck_gpio);
}

static const struct snd_soc_dapm_widget imx_xmos_dapm_widgets[] = {
	SND_SOC_DAPM_HP("Headphone Jack", NULL),
	SND_SOC_DAPM_SPK("Ext Spk", NULL),
};

static int imx_hifi_hw_params(struct snd_pcm_substream *substream,
				     struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	struct snd_soc_card *card = rtd->card;
	struct imx_xmos_data *data = snd_soc_card_get_drvdata(card);
	bool tx = substream->stream == SNDRV_PCM_STREAM_PLAYBACK;
	struct device *dev = card->dev;
	//unsigned int sample_rate = params_rate(params);
	//unsigned int pll_out;
	unsigned int fmt;
	int ret = 0;

	data->is_stream_in_use[tx] = true;

	if (data->is_stream_in_use[!tx])
		return 0;

	fmt = SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_NB_NF | SND_SOC_DAIFMT_CBS_CFS;

	/* set cpu DAI configuration */
	ret = snd_soc_dai_set_fmt(cpu_dai, fmt);
	if (ret) {
		dev_err(dev, "failed to set cpu dai fmt: %d\n", ret);
		return ret;
	}
	/* set codec DAI configuration */
	ret = snd_soc_dai_set_fmt(codec_dai, fmt);
	if (ret) {
		dev_err(dev, "failed to set codec dai fmt: %d\n", ret);
		return ret;
	}

	ret = snd_soc_dai_set_tdm_slot(cpu_dai, 0x3, 0x3, 2, params_width(params));
	if (ret) {
		dev_err(dev, "failed to set cpu dai tdm slot: %d\n", ret);
		return ret;
	}

	ret = snd_soc_dai_set_sysclk(cpu_dai, 0, 0, SND_SOC_CLOCK_OUT);
	if (ret) {
		dev_err(dev, "failed to set cpu sysclk: %d\n", ret);
		return ret;
	}
	return 0;
}

static int imx_hifi_hw_free(struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	//struct snd_soc_dai *codec_dai = rtd->codec_dai;
	struct snd_soc_card *card = rtd->card;
	struct imx_xmos_data *data = snd_soc_card_get_drvdata(card);
	bool tx = substream->stream == SNDRV_PCM_STREAM_PLAYBACK;
	//struct device *dev = card->dev;
	//int ret;

	data->is_stream_in_use[tx] = false;

	return 0;
}

static int imx_hifi_startup(struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	struct snd_soc_card *card = rtd->card;
	struct imx_xmos_data *data = snd_soc_card_get_drvdata(card);
	bool tx = substream->stream == SNDRV_PCM_STREAM_PLAYBACK;
	struct fsl_sai *sai = dev_get_drvdata(cpu_dai->dev);
	int ret = 0;

	data->is_stream_opened[tx] = true;
	if (data->is_stream_opened[tx] != sai->is_stream_opened[tx] ||
	    data->is_stream_opened[!tx] != sai->is_stream_opened[!tx]) {
		data->is_stream_opened[tx] = false;
		return -EBUSY;
	}

	ret = clk_prepare_enable(data->codec_clk);
	if (ret) {
		dev_err(card->dev, "Failed to enable MCLK: %d\n", ret);
		return ret;
	}
	return ret;
}

static void imx_hifi_shutdown(struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_card *card = rtd->card;
	struct imx_xmos_data *data = snd_soc_card_get_drvdata(card);
	bool tx = substream->stream == SNDRV_PCM_STREAM_PLAYBACK;

//	clk_disable_unprepare(data->codec_clk);

	data->is_stream_opened[tx] = false;
}

/* SoC audio ops */
static struct snd_soc_ops imx_hifi_ops = {
	.hw_params = imx_hifi_hw_params,
	.hw_free = imx_hifi_hw_free,
	.startup   = imx_hifi_startup,
	.shutdown  = imx_hifi_shutdown,
};

static int imx_xmos_late_probe(struct snd_soc_card *card)
{
	return 0;
}

static int be_hw_params_fixup(struct snd_soc_pcm_runtime *rtd,
			struct snd_pcm_hw_params *params)
{
	struct snd_soc_card *card = rtd->card;
	struct imx_xmos_data *data = snd_soc_card_get_drvdata(card);
	struct imx_priv *priv = &card_priv;
	struct snd_interval *rate;
	struct snd_mask *mask;

	if (!priv->asrc_pdev)
		return -EINVAL;

	rate = hw_param_interval(params, SNDRV_PCM_HW_PARAM_RATE);
	rate->max = rate->min = data->asrc_rate;

	mask = hw_param_mask(params, SNDRV_PCM_HW_PARAM_FORMAT);
	snd_mask_none(mask);
	snd_mask_set(mask, data->asrc_format);

	return 0;
}

/* digital audio interface glue - connects codec <--> CPU */
static struct snd_soc_dai_link imx_xmos_dai[] = {
	{
		.name = "HiFi",
		.stream_name = "HiFi",
		.codec_dai_name = "xmos-hifi",
		.ops = &imx_hifi_ops,
	},
	{
		.name = "HiFi-ASRC-FE",
		.stream_name = "HiFi-ASRC-FE",
		.codec_name = "snd-soc-dummy",
		.codec_dai_name = "snd-soc-dummy-dai",
		.dynamic = 1,
		.ignore_pmdown_time = 1,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
	},
	{
		.name = "HiFi-ASRC-BE",
		.stream_name = "HiFi-ASRC-BE",
		.codec_dai_name = "xmos-hifi",
		.platform_name = "snd-soc-dummy",
		.no_pcm = 1,
		.ignore_pmdown_time = 1,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		.ops = &imx_hifi_ops,
		.be_hw_params_fixup = be_hw_params_fixup,
	},
};

static int imx_xmos_probe(struct platform_device *pdev)
{
	struct device_node *cpu_np, *codec_np = NULL;
	struct device_node *gpr_np;
	struct platform_device *cpu_pdev;
	struct imx_priv *priv = &card_priv;
	struct i2c_client *codec_dev;
	struct imx_xmos_data *data;
	struct platform_device *asrc_pdev = NULL;
	struct device_node *asrc_np;
	u32 width;
	int ret;

	priv->pdev = pdev;

	cpu_np = of_parse_phandle(pdev->dev.of_node, "cpu-dai", 0);
	if (!cpu_np) {
		dev_err(&pdev->dev, "cpu dai phandle missing or invalid\n");
		ret = -EINVAL;
		goto fail;
	}

	codec_np = of_parse_phandle(pdev->dev.of_node, "audio-codec", 0);
	if (!codec_np) {
		dev_err(&pdev->dev, "phandle missing or invalid\n");
		ret = -EINVAL;
		goto fail;
	}

	cpu_pdev = of_find_device_by_node(cpu_np);
	if (!cpu_pdev) {
		dev_err(&pdev->dev, "failed to find SAI platform device\n");
		ret = -EINVAL;
		goto fail;
	}

	codec_dev = of_find_i2c_device_by_node(codec_np);
	if (!codec_dev || !codec_dev->dev.driver) {
		dev_err(&pdev->dev, "failed to find codec platform device\n");
		ret = -EINVAL;
		goto fail;
	}

	data = devm_kzalloc(&pdev->dev, sizeof(*data), GFP_KERNEL);
	if (!data) {
		ret = -ENOMEM;
		goto fail;
	}

	data->codec_clk = devm_clk_get(&codec_dev->dev, "mclk");
	if (IS_ERR(data->codec_clk)) {
		ret = PTR_ERR(data->codec_clk);
		dev_err(&pdev->dev, "failed to get codec clk: %d\n", ret);
		goto fail;
	}
	/* add by colin */
	sc_set_input();
	mdelay(500);
	clk_prepare_enable(data->codec_clk);
	sc_rst_xmos();

	gpr_np = of_parse_phandle(pdev->dev.of_node, "gpr", 0);
        if (gpr_np) {
		data->gpr = syscon_node_to_regmap(gpr_np);
		if (IS_ERR(data->gpr)) {
			ret = PTR_ERR(data->gpr);
			dev_err(&pdev->dev, "failed to get gpr regmap\n");
			goto fail;
		}

		/* set SAI2_MCLK_DIR to enable codec MCLK for imx7d */
		regmap_update_bits(data->gpr, 4, 1<<20, 1<<20);
	}

	of_property_read_u32_array(pdev->dev.of_node, "hp-det", data->hp_det, 2);

	asrc_np = of_parse_phandle(pdev->dev.of_node, "asrc-controller", 0);
	if (asrc_np) {
		asrc_pdev = of_find_device_by_node(asrc_np);
		priv->asrc_pdev = asrc_pdev;
	}

	data->card.dai_link = imx_xmos_dai;

	imx_xmos_dai[0].codec_of_node	= codec_np;
	imx_xmos_dai[0].cpu_dai_name = dev_name(&cpu_pdev->dev);
	imx_xmos_dai[0].platform_of_node = cpu_np;

	if (!asrc_pdev) {
		data->card.num_links = 1;
	} else {
		imx_xmos_dai[1].cpu_of_node = asrc_np;
		imx_xmos_dai[1].platform_of_node = asrc_np;
		imx_xmos_dai[2].codec_of_node	= codec_np;
		imx_xmos_dai[2].cpu_dai_name = dev_name(&cpu_pdev->dev);
		data->card.num_links = 3;

		ret = of_property_read_u32(asrc_np, "fsl,asrc-rate",
				&data->asrc_rate);
		if (ret) {
			dev_err(&pdev->dev, "failed to get output rate\n");
			ret = -EINVAL;
			goto fail;
		}

		ret = of_property_read_u32(asrc_np, "fsl,asrc-width", &width);
		if (ret) {
			dev_err(&pdev->dev, "failed to get output rate\n");
			ret = -EINVAL;
			goto fail;
		}

		if (width == 24)
			data->asrc_format = SNDRV_PCM_FORMAT_S24_LE;
		else
			data->asrc_format = SNDRV_PCM_FORMAT_S16_LE;
	}

	data->card.dev = &pdev->dev;
	data->card.owner = THIS_MODULE;
	ret = snd_soc_of_parse_card_name(&data->card, "model");
	if (ret)
		goto fail;
	data->card.dapm_widgets = imx_xmos_dapm_widgets;
	data->card.num_dapm_widgets = ARRAY_SIZE(imx_xmos_dapm_widgets);

	ret = snd_soc_of_parse_audio_routing(&data->card, "audio-routing");
	if (ret)
		goto fail;

	data->card.late_probe = imx_xmos_late_probe;

	platform_set_drvdata(pdev, &data->card);
	snd_soc_card_set_drvdata(&data->card, data);
	ret = devm_snd_soc_register_card(&pdev->dev, &data->card);
	if (ret) {
		dev_err(&pdev->dev, "snd_soc_register_card failed (%d)\n", ret);
		goto fail;
	}

	priv->snd_card = data->card.snd_card;

	ret = 0;
fail:
	if (cpu_np)
		of_node_put(cpu_np);
	if (codec_np)
		of_node_put(codec_np);

	return ret;
}

static int imx_xmos_remove(struct platform_device *pdev)
{

	return 0;
}

static const struct of_device_id imx_xmos_dt_ids[] = {
	{ .compatible = "fsl,imx-audio-xmos", },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, imx_xmos_dt_ids);

static struct platform_driver imx_xmos_driver = {
	.driver = {
		.name = "imx-xmos",
		.pm = &snd_soc_pm_ops,
		.of_match_table = imx_xmos_dt_ids,
	},
	.probe = imx_xmos_probe,
	.remove = imx_xmos_remove,
};
module_platform_driver(imx_xmos_driver);

MODULE_AUTHOR("Sercomm Colin");
MODULE_DESCRIPTION("Freescale i.MX XMOS ASoC machine driver");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:imx-xmos");
