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

#define pr_fmt(fmt) "mikrobus_i2c_device: " fmt
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/i2c.h>

#include "mikrobus.h"
#include "mikrobus_i2c.h"

static struct i2c_client *i2c_device;

static char *name;
module_param(name, charp, 0000);
MODULE_PARM_DESC(name, "Device name (required).");

static char *port;
module_param(port, charp, 0000);
MODULE_PARM_DESC(name, "Mikrobus port (required).");


struct mikrobus_i2c_device {
	char *name;
	struct i2c_board_info *i2c;
};

static struct mikrobus_i2c_device devices[] = {
	 {
		.name = "rtc6",
		.i2c = &(struct i2c_board_info) {
			I2C_BOARD_INFO("mcp7941x", 0x6f),
			.irq=-1,
		}
	},
	{
		.name = "weather",
		.i2c = &(struct i2c_board_info) {
			I2C_BOARD_INFO("bme280", 0x76),
			.irq=-1,
		}
	},
	{
		.name = "mpu9dof",
		.i2c = &(struct i2c_board_info) {
			I2C_BOARD_INFO("mpu9150", 0x68),
			.irq=1,
		}
	},
	{
		.name = "techlab_accel",
		.i2c = &(struct i2c_board_info) {
			I2C_BOARD_INFO("mma8453", 0x1c),
			.irq=1,
		}
	},
};

#ifdef MODULE
static int mikrobus_i2c_device_register(struct i2c_board_info *i2c, uint8_t bus)
{
	struct i2c_adapter *i2c_adap;

    i2c_adap = i2c_get_adapter(1);

	if (!i2c_adap) {
		pr_err("i2c_busnum_to_adapter(%d) returned NULL\n",
		       1);
		return -EINVAL;
	}
	i2c_device = i2c_new_device(i2c_adap, i2c);
	put_device(&i2c_adap->dev);
	if (!i2c_device) {
		dev_err(&i2c_adap->dev, "i2c_new_device() returned NULL\n");
		return -EPERM;
	}
	return 0;
}
#else
static int mikrobus_i2c_device_register(struct i2c_board_info *i2c, uint8_t bus)
{
	return i2c_register_board_info(1,i2c, 1);
}
#endif

static int __init mikrobus_i2c_device_init(void)
{
	struct i2c_board_info *i2c = NULL;
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
		if (strncmp(name, devices[i].name, I2C_NAME_SIZE) == 0) {
			if (devices[i].i2c) {
				i2c = devices[i].i2c;
				if(i2c->irq)
					i2c->irq=gpio_to_irq(m_port->int_gpio);
				ret = mikrobus_i2c_device_register(i2c,m_port->i2c_bus);
				if (ret) {
					pr_err("failed to register I2C device\n");
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

static void __exit mikrobus_i2c_device_exit(void)
{
	if (i2c_device) {
		device_del(&i2c_device->dev);
		kfree(i2c_device);
	}
}

arch_initcall(mikrobus_i2c_device_init);
module_exit(mikrobus_i2c_device_exit);

MODULE_DESCRIPTION("Instantiate a Mikrobus device.");
MODULE_AUTHOR("Vaishnav M A");
MODULE_LICENSE("GPL");
