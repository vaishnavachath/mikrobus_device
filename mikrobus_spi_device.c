/*
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#define pr_fmt(fmt) "mikrobus_spi_device: " fmt
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/spi/spi.h>

#include "mikrobus.h"
#include "mikrobus_spi.h"

static struct spi_device *spi_device;

static char *name;
module_param(name, charp, 0000);
MODULE_PARM_DESC(name, "Device name (required).");

static char *port;
module_param(port, charp, 0000);
MODULE_PARM_DESC(name, "Mikrobus port (required).");


struct mikrobus_spi_device {
	char *name;
	struct spi_board_info *spi;
};

static struct mikrobus_spi_device devices[] = {
	 {
		.name = "oledc",
		.spi = &(struct spi_board_info) {
			.modalias = "fb_ssd1351",
			.max_speed_hz = 6000000,
			.mode = SPI_MODE_0,
			.chip_select = 0,
			.bus_num = 0,
			.irq=-1,
			.platform_data = &(struct fbtft_platform_data) {
				.bgr = false,
				.rotate = 0,
				.fps = 40,
				.display = {
					.buswidth = 8,
					.backlight = 2,
					.width = 128,
					.height = 128,
				},
				.gpios = (const struct fbtft_gpio []) {
					{ "reset", 89 },
					{ "dc", 50 },
				},
			}
		}
	},
	{
		.name = "oledb",
		.spi = &(struct spi_board_info) {
			.modalias = "fb_ssd1306",
			.max_speed_hz = 6000000,
			.mode = SPI_MODE_0,
			.chip_select = 0,
			.bus_num = 0,			
			.irq=-1,
			.platform_data = &(struct fbtft_platform_data) {
				.bgr = false,
				.rotate = 0,
				.fps = 40,
				.display = {
					.buswidth = 8,
					.backlight = 2,
					.width = 96,
					.height = 39,
				},
				.gpios = (const struct fbtft_gpio []) {
					{ "reset", 89 },
					{ "dc", 50 },
				},
			}
		}
	},
	{
		.name = "microsd",
		.spi = &(struct spi_board_info) {
			.modalias = "mmc_spi",
			.max_speed_hz = 6000000,
			.mode = SPI_MODE_0,
			.chip_select = 0,
			.bus_num = 0,
			.irq=-1,
			.platform_data = &(struct mmc_spi_platform_data) {
				.cd_gpio = 59,
				.ocr_mask= 0x00100000|0x00200000,
			}
		}
	},
	{
		.name = "eth",
		.spi = &(struct spi_board_info) {
			.modalias = "enc28j60",
			.max_speed_hz = 16000000,
			.mode = SPI_MODE_0,
			.chip_select = 0,
			.bus_num = 0,
			.irq=1,
		}
	},
};
#ifdef MODULE
static void mikrobus_spi_device_spi_delete(struct spi_master *master, unsigned int cs)
{
	struct device *dev;
	char str[32];

	snprintf(str, sizeof(str), "%s.%u", dev_name(&master->dev), cs);

	dev = bus_find_device_by_name(&spi_bus_type, NULL, str);
	if (dev) {
		device_del(dev);
	}
}

static int mikrobus_spi_device_spi_device_register(struct spi_board_info *spi)
{
	struct spi_master *master;

	master = spi_busnum_to_master(spi->bus_num);
	if (!master) {
		pr_err("spi_busnum_to_master(%d) returned NULL\n",
		       spi->bus_num);
		return -EINVAL;
	}
	/* make sure it's available */
	mikrobus_spi_device_spi_delete(master, spi->chip_select);
	spi_device = spi_new_device(master, spi);
	put_device(&master->dev);
	if (!spi_device) {
		dev_err(&master->dev, "spi_new_device() returned NULL\n");
		return -EPERM;
	}
	return 0;
}
#else
static int mikrobus_spi_device_spi_device_register(struct spi_board_info *spi)
{
	return spi_register_board_info(spi, 1);
}
#endif

static int __init mikrobus_spi_device_init(void)
{
	struct spi_board_info *spi = NULL;
	struct mikrobus_port *m_port = NULL;
	bool found = false;
	int i = 0;
	int ret = 0;

	if (!name) {
#ifdef MODULE
		pr_err("missing module parameter: 'name'\n");
		return -EINVAL;
#else
		return 0;
#endif
	}

	if (!port) {
#ifdef MODULE
		pr_err("missing module parameter: 'port'\n");
		return -EINVAL;
#else
		return 0;
#endif
	}

	m_port=get_mikrobus_port(port);

	for (i = 0; i < ARRAY_SIZE(devices); i++) {
		if (strncmp(name, devices[i].name, SPI_NAME_SIZE) == 0) {
			if (devices[i].spi) {
				spi = devices[i].spi;
				spi->bus_num= m_port->spi_bus;
				spi->chip_select= m_port->spi_cs;
				if(spi->irq)
					spi->irq=gpio_to_irq(m_port->int_gpio);
				ret = mikrobus_spi_device_spi_device_register(spi);
				if (ret) {
					pr_err("failed to register SPI device\n");
					return ret;
				}
			}
			found = true;
			break;
		}
	}

	if (!found) {
		pr_err("device not supported: '%s'\n", name);
		return -EINVAL;
	}
	return 0;
}

static void __exit mikrobus_spi_device_exit(void)
{
	if (spi_device) {
		device_del(&spi_device->dev);
		kfree(spi_device);
	}
}

arch_initcall(mikrobus_spi_device_init);
module_exit(mikrobus_spi_device_exit);

MODULE_DESCRIPTION("Instantiate a Mikrobus device.");
MODULE_AUTHOR("Vaishnav M A");
MODULE_LICENSE("GPL");
