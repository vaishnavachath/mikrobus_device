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

#include "mikrobus_spi.h"

static struct spi_device *spi_device;

//if custom == true , then name should correspond to the device driver name
static char *name;
module_param(name, charp, 0000);
MODULE_PARM_DESC(name, "Device name (required).");

static unsigned int busno;
module_param(busno, uint, 0000);
MODULE_PARM_DESC(busno,"SPI Bus No to instantiate device");

static unsigned short csno;
module_param(csno, ushort, 0000);
MODULE_PARM_DESC(csno,"SPI Bus Chip Select No.");

static unsigned int irq_gpio;
module_param(irq_gpio, uint, 0000);
MODULE_PARM_DESC(irq_gpio,"Interrupt GPIO Number");

static unsigned int max_speed;
module_param(max_speed, uint, 0000);
MODULE_PARM_DESC(max_speed,"Interrupt GPIO Number");

static bool custom;
module_param(custom, bool, 0000);
MODULE_PARM_DESC(custom, "Add a custom device");



struct mikrobus_spi_device {
	char *name;
	struct spi_board_info *spi;
};

static struct mikrobus_spi_device devices[] = {
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
	{
		.name = "custom",
		.spi = &(struct spi_board_info) {
			.modalias = "",
			.max_speed_hz = 0,
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

	if (!busno) {
#ifdef MODULE
		pr_err("missing module parameter: 'busno'\n");
		return -EINVAL;
#else
		return 0;
#endif
	}

	if (!csno) {
#ifdef MODULE
		csno=0;
#else
		return 0;
#endif
	}

	for (i = 0; i < ARRAY_SIZE(devices); i++) {
		if (strncmp(name, devices[i].name, SPI_NAME_SIZE) == 0) {
			if (devices[i].spi) {
				spi = devices[i].spi;
				spi->bus_num= busno;
				spi->chip_select= csno;
				if(spi->irq) {
					spi->irq=gpio_to_irq(irq_gpio);
					if (spi->irq < 0) {
						pr_err("Could not get irq for gpio pin: %d",0);
						return -EINVAL;
					}
				}
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
		if(!custom) {
			pr_err("device not supported: '%s'\n", name);
			return -EINVAL;
		}
		else {
			spi = devices[ARRAY_SIZE(devices)-1].spi;
			strlcpy(spi->modalias, name,SPI_NAME_SIZE);
			spi->bus_num= busno;
			spi->chip_select= csno;
			spi->max_speed_hz= max_speed;
			spi->irq= gpio_to_irq(irq_gpio);;
			ret = mikrobus_spi_device_spi_device_register(spi);
				if (ret) {
					pr_err("failed to register Custom SPI device\n");
					return ret;
				}
		}
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
