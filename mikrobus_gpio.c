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

#define pr_fmt(fmt) "mikrobus_gpio: " fmt
#include <linux/module.h>   
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>

#include "mikrobus.h"
#include "mikrobus_gpio.h"

static int __init mikrobus_gpio_device_init(void)
{
    int i;
	for(i=MIKROBUS_START_GPIO;i<(MIKROBUS_START_GPIO+MIKROBUS_NUM_GPIOS);i++){
           gpio_request(i, "sysfs");
           gpio_direction_input(i);
           gpio_export(i,true);
    }
}

static void __exit mikrobus_gpio_device_exit(void)
{   
    int i;
	for(i=MIKROBUS_START_GPIO;i<(MIKROBUS_START_GPIO+MIKROBUS_NUM_GPIOS);i++)
        gpio_free(i);
}

arch_initcall(mikrobus_gpio_device_init);
module_exit(mikrobus_gpio_device_exit);

MODULE_DESCRIPTION("Module to Export Mikrobus Port GPIOS");
MODULE_AUTHOR("Vaishnav M A");
MODULE_LICENSE("GPL");