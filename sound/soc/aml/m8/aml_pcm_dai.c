/*
 * sound/soc/aml/m8/aml_pcm_dai.c
 *
 * Copyright (C) 2015 Amlogic, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
*/
#define pr_fmt(fmt) "aml_pcm_dai: " fmt

#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/gpio/consumer.h>

#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/initval.h>
#include <sound/soc.h>

#include "aml_pcm_dai.h"
#include "aml_pcm.h"
#include "aml_i2s.h"
#include "aml_audio_hw_pcm2bt.h"

#include <linux/of.h>

#define DEV_NAME "aml-pcm-dai"

static int aml_dai_pcm_startup(struct snd_pcm_substream *substream,
			       struct snd_soc_dai *dai)
{
	pr_debug("***Entered %s\n", __func__);
	return 0;
}

static void aml_dai_pcm_shutdown(struct snd_pcm_substream *substream,
				 struct snd_soc_dai *dai)
{
	pr_debug("***Entered %s\n", __func__);
}

static int aml_dai_pcm_prepare(struct snd_pcm_substream *substream,
			       struct snd_soc_dai *dai)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct aml_pcm_runtime_data *prtd = runtime->private_data;

	pr_debug("***Entered %s\n", __func__);

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
		pr_info(
		       "%s playback stream buffer start: %ld size: 0x%x\n",
		       __func__, (long)prtd->buffer_start, prtd->buffer_size);
		pcm_out_set_buf(prtd->buffer_start, prtd->buffer_size);
	} else {
		pr_info(
		       "%s capture stream buffer start: %ld size: 0x%x\n",
		       __func__, (long)prtd->buffer_start, prtd->buffer_size);
		pcm_in_set_buf(prtd->buffer_start, prtd->buffer_size);
	}

	memset(runtime->dma_area, 0, runtime->dma_bytes);
	prtd->buffer_offset = 0;
	prtd->data_size = 0;
	prtd->peroid_elapsed = 0;
/* #endif */
	return 0;
}

static int aml_dai_pcm_trigger(struct snd_pcm_substream *substream, int cmd,
			       struct snd_soc_dai *dai)
{
	switch (cmd) {
	case SNDRV_PCM_TRIGGER_START:
	case SNDRV_PCM_TRIGGER_RESUME:
	case SNDRV_PCM_TRIGGER_PAUSE_RELEASE:
		/* TODO */
		if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
			pr_info("aiu pcm playback enable\n\n");
			pcm_out_enable(1);
		} else {
			pcm_in_enable(1);
		}
		break;
	case SNDRV_PCM_TRIGGER_STOP:
	case SNDRV_PCM_TRIGGER_SUSPEND:
	case SNDRV_PCM_TRIGGER_PAUSE_PUSH:
		if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
			pr_info("aiu pcm playback disable\n\n");
			pcm_out_enable(0);
		} else {
			pcm_in_enable(0);

		}
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static int aml_dai_pcm_hw_params(struct snd_pcm_substream *substream,
				 struct snd_pcm_hw_params *params,
				 struct snd_soc_dai *dai)
{
	pr_debug("***Entered %s:%s\n", __FILE__, __func__);
	return 0;
}

static int aml_dai_set_pcm_fmt(struct snd_soc_dai *dai, unsigned int fmt)
{
	pr_debug("***Entered %s\n", __func__);
	if (fmt & SND_SOC_DAIFMT_CBS_CFS)
		snd_soc_dai_get_drvdata(dai);
	return 0;
}

static int aml_dai_set_pcm_sysclk(struct snd_soc_dai *dai,
				  int clk_id, unsigned int freq, int dir)
{
	pr_debug("***Entered %s\n", __func__);
	return 0;
}

#ifdef CONFIG_PM
static int aml_dai_pcm_suspend(struct snd_soc_dai *dai)
{

	pr_debug("***Entered %s\n", __func__);
	return 0;
}

static int aml_dai_pcm_resume(struct snd_soc_dai *dai)
{
	pr_debug("***Entered %s\n", __func__);
	return 0;
}

#else				/* CONFIG_PM */
#define aml_dai_pcm_suspend	NULL
#define aml_dai_pcm_resume	NULL
#endif				/* CONFIG_PM */

#define AML_DAI_PCM_RATES		(SNDRV_PCM_RATE_8000)
#define AML_DAI_PCM_FORMATS		(SNDRV_PCM_FMTBIT_S16_LE |\
	SNDRV_PCM_FMTBIT_S24_LE | SNDRV_PCM_FMTBIT_S32_LE)

static struct snd_soc_dai_ops aml_dai_pcm_ops = {
	.startup = aml_dai_pcm_startup,
	.shutdown = aml_dai_pcm_shutdown,
	.prepare = aml_dai_pcm_prepare,
	.trigger = aml_dai_pcm_trigger,
	.hw_params = aml_dai_pcm_hw_params,
	.set_fmt = aml_dai_set_pcm_fmt,
	.set_sysclk = aml_dai_set_pcm_sysclk,
};

struct snd_soc_dai_driver aml_pcm_dai[] = {
	{
	 .suspend = aml_dai_pcm_suspend,
	 .resume = aml_dai_pcm_resume,
	 .playback = {
		      .channels_min = 1,
		      .channels_max = 1,
		      .rates = AML_DAI_PCM_RATES,
		      .formats = AML_DAI_PCM_FORMATS,},
	 .capture = {
		     .channels_min = 1,
		     .channels_max = 1,
		     .rates = AML_DAI_PCM_RATES,
		     .formats = AML_DAI_PCM_FORMATS,},
	 .ops = &aml_dai_pcm_ops,
	 },

};
EXPORT_SYMBOL_GPL(aml_pcm_dai);

static const struct snd_soc_component_driver aml_component = {
	.name = DEV_NAME,
};

static int aml_pcm_dai_probe(struct platform_device *pdev)
{
	struct pinctrl *pin_ctl;

	pr_debug("enter %s\n", __func__);

	pin_ctl = devm_pinctrl_get_select(&pdev->dev, "aml_audio_btpcm");
	if (IS_ERR(pin_ctl))
		pr_err("aml audio pcm dai pinmux set error!\n");

	return snd_soc_register_component(&pdev->dev, &aml_component,
					  aml_pcm_dai, ARRAY_SIZE(aml_pcm_dai));
}

static int aml_pcm_dai_remove(struct platform_device *pdev)
{
	snd_soc_unregister_component(&pdev->dev);
	return 0;
}

#ifdef CONFIG_OF
static const struct of_device_id amlogic_dai_dt_match[] = {
	{.compatible = "amlogic, aml-pcm-dai",
	 },
	{},
};
#else
#define amlogic_dai_dt_match NULL
#endif

static struct platform_driver aml_pcm_dai_driver = {
	.driver = {
		   .name = DEV_NAME,
		   .owner = THIS_MODULE,
		   .of_match_table = amlogic_dai_dt_match,
		   },

	.probe = aml_pcm_dai_probe,
	.remove = aml_pcm_dai_remove,
};

static int __init aml_dai_modinit(void)
{
	return platform_driver_register(&aml_pcm_dai_driver);
}

module_init(aml_dai_modinit);

static void __exit aml_dai_modexit(void)
{
	platform_driver_unregister(&aml_pcm_dai_driver);
}

module_exit(aml_dai_modexit);

/* Module information */
MODULE_AUTHOR("AMLogic, Inc.");
MODULE_DESCRIPTION("AML DAI driver for ALSA");
MODULE_LICENSE("GPL");
