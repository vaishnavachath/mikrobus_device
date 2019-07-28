#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/gpio.h>

#include "mikrobus.h"

static struct mikrobus_port ports[] = {
	 {
		.name = "PBP1",
        .an_gpio = PBP1_AN,
        .rst_gpio = PBP1_RST,
        .cs_gpio = PBP1_CS,
        .sck_gpio = PBP1_SCK,
        .miso_gpio = PBP1_MISO, 
        .mosi_gpio = PBP1_MOSI,
        .sda_gpio = PBP1_SDA,
        .scl_gpio = PBP1_SCL,
        .tx_gpio = PBP1_TX,
        .rx_gpio = PBP1_RX,
        .int_gpio = PBP1_INT,
        .pwm_gpio = PBP1_PWM,
        .spi_bus = PBP1_SPIBUS,
        .spi_cs = PBP1_SPICS,
        .i2c_bus = PBP1_I2CBUS,		
	},
     {
		.name = "PBP2",
        .an_gpio = PBP2_AN,
        .rst_gpio = PBP2_RST,
        .cs_gpio = PBP2_CS,
        .sck_gpio = PBP2_SCK,
        .miso_gpio = PBP2_MISO, 
        .mosi_gpio = PBP2_MOSI,
        .sda_gpio = PBP2_SDA,
        .scl_gpio = PBP2_SCL,
        .tx_gpio = PBP2_TX,
        .rx_gpio = PBP2_RX,
        .int_gpio = PBP2_INT,
        .pwm_gpio = PBP2_PWM,
        .spi_bus = PBP2_SPIBUS,
        .spi_cs = PBP2_SPICS,
        .i2c_bus = PBP2_I2CBUS,		
	},
     {
		.name = "BBP1",
        .an_gpio = BBP1_AN,
        .rst_gpio = BBP1_RST,
        .cs_gpio = BBP1_CS,
        .sck_gpio = BBP1_SCK,
        .miso_gpio = BBP1_MISO, 
        .mosi_gpio = BBP1_MOSI,
        .sda_gpio = BBP1_SDA,
        .scl_gpio = BBP1_SCL,
        .tx_gpio = BBP1_TX,
        .rx_gpio = BBP1_RX,
        .int_gpio = BBP1_INT,
        .pwm_gpio = BBP1_PWM,
        .spi_bus = BBP1_SPIBUS,
        .spi_cs = BBP1_SPICS,
        .i2c_bus = BBP1_I2CBUS,		
	},
    {
		.name = "BBP2",
        .an_gpio = BBP2_AN,
        .rst_gpio = BBP2_RST,
        .cs_gpio = BBP2_CS,
        .sck_gpio = BBP2_SCK,
        .miso_gpio = BBP2_MISO, 
        .mosi_gpio = BBP2_MOSI,
        .sda_gpio = BBP2_SDA,
        .scl_gpio = BBP2_SCL,
        .tx_gpio = BBP2_TX,
        .rx_gpio = BBP2_RX,
        .int_gpio = BBP2_INT,
        .pwm_gpio = BBP2_PWM,
        .spi_bus = BBP2_SPIBUS,
        .spi_cs = BBP2_SPICS,
        .i2c_bus = BBP2_I2CBUS,		
	},
    {
		.name = "BBP3",
        .an_gpio = BBP3_AN,
        .rst_gpio = BBP3_RST,
        .cs_gpio = BBP3_CS,
        .sck_gpio = BBP3_SCK,
        .miso_gpio = BBP3_MISO, 
        .mosi_gpio = BBP3_MOSI,
        .sda_gpio = BBP3_SDA,
        .scl_gpio = BBP3_SCL,
        .tx_gpio = BBP3_TX,
        .rx_gpio = BBP3_RX,
        .int_gpio = BBP3_INT,
        .pwm_gpio = BBP3_PWM,
        .spi_bus = BBP3_SPIBUS,
        .spi_cs = BBP3_SPICS,
        .i2c_bus = BBP3_I2CBUS,		
	},
    {
		.name = "BBP4",
        .an_gpio = BBP4_AN,
        .rst_gpio = BBP4_RST,
        .cs_gpio = BBP4_CS,
        .sck_gpio = BBP4_SCK,
        .miso_gpio = BBP4_MISO, 
        .mosi_gpio = BBP4_MOSI,
        .sda_gpio = BBP4_SDA,
        .scl_gpio = BBP4_SCL,
        .tx_gpio = BBP4_TX,
        .rx_gpio = BBP4_RX,
        .int_gpio = BBP4_INT,
        .pwm_gpio = BBP4_PWM,
        .spi_bus = BBP4_SPIBUS,
        .spi_cs = BBP4_SPICS,
        .i2c_bus = BBP4_I2CBUS,		
	},
};

struct mikrobus_port *get_mikrobus_port(char *port)
{
    int i=0;

    for (i = 0; i < ARRAY_SIZE(ports); i++) {
		if (strncmp(port, ports[i].name, sizeof(ports[i].name)) == 0) {
			return &ports[i];
		}
	}
    return NULL;
}
EXPORT_SYMBOL_GPL(get_mikrobus_port);

MODULE_DESCRIPTION("Instantiate a Mikrobus device.");
MODULE_AUTHOR("Vaishnav M A");
MODULE_LICENSE("GPL");