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

#include "mikrobus_i2c.h"

static struct i2c_client *i2c_device;

//if custom == true , then name should correspond to the device driver name
static char *name;
module_param(name, charp, 0000);
MODULE_PARM_DESC(name, "Device name (required).");

static unsigned int busno;
module_param(busno, uint, 0000);
MODULE_PARM_DESC(busno,"I2C Bus No to instantiate device");

static unsigned short address;
module_param(address, ushort, 0000);
MODULE_PARM_DESC(address,"I2C Device Address");

static unsigned int irq_gpio;
module_param(irq_gpio, uint, 0000);
MODULE_PARM_DESC(irq_gpio,"Interrupt GPIO Number");

static bool custom;
module_param(custom, bool, 0000);
MODULE_PARM_DESC(custom, "Add a custom device");

struct mikrobus_i2c_device {
	char *name;
	struct i2c_board_info *i2c;
};

static struct mikrobus_i2c_device devices[] = {
	{
		.name = "mpu9dof",
		.i2c = &(struct i2c_board_info) {
			I2C_BOARD_INFO("mpu9150", 0x68),
			.irq=1,
		}
	},
	{
		.name = "custom",
		.i2c = &(struct i2c_board_info) {
			.type = "",
			.addr = 0x00,
			.irq = 1,
		}
	},
};

#ifdef MODULE
static int mikrobus_i2c_device_register(struct i2c_board_info *i2c)
{
	struct i2c_adapter *i2c_adap;

    i2c_adap = i2c_get_adapter(busno);

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
	return i2c_register_board_info(busno,i2c, 1);
}
#endif

static int __init mikrobus_i2c_device_init(void)
{
	struct i2c_board_info *i2c = NULL;
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

	for (i = 0; i < ARRAY_SIZE(devices); i++) {
		if (strncmp(name, devices[i].name, I2C_NAME_SIZE) == 0) {
			if (devices[i].i2c) {
				i2c = devices[i].i2c;
				if(i2c->irq) {
					i2c->irq=gpio_to_irq(irq_gpio);
					if (i2c->irq < 0) {
						pr_err("Could not get irq for gpio pin: %d",irq_gpio);
						return -EINVAL;
					}
				}
				ret = mikrobus_i2c_device_register(i2c);
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
		if(!custom) {
			pr_err("device not supported: '%s'\n", name);
			return -EINVAL;
		}
		else {
			i2c = devices[ARRAY_SIZE(devices)-1].i2c;
			strlcpy(i2c->type, name,I2C_NAME_SIZE);
			i2c->addr = address;
			i2c->irq = gpio_to_irq(irq_gpio);
			ret = mikrobus_i2c_device_register(i2c);
				if (ret) {
					pr_err("failed to register Custom I2C device\n");
					return ret;
				}
		}
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

MODULE_DESCRIPTION("Instantiate a Mikrobus I2C device.");
MODULE_AUTHOR("Vaishnav M A");
MODULE_LICENSE("GPL");
