#ifndef __LINUX_MIKROBUS_H
#define __LINUX_MIKROBUS_H

//PocketBeagle Mikrobus Port 1 GPIO Pins
#define PBP1_AN 87 //P1_02
#define PBP1_RST 89 //P1_04
#define PBP1_CS 5 //P1_06
#define PBP1_SCK 2 //P1_08
#define PBP1_MISO 3 //P1_10
#define PBP1_MOSI 4 //P1_12
#define PBP1_SDA 14 //P2_11
#define PBP1_SCL 15 //P2_09
#define PBP1_TX 31 //P2_07
#define PBP1_RX 30 //P2_05
#define PBP1_INT 23 //P2_03
#define PBP1_PWM 50 //P2_01
#define PBP1_SPIBUS 1 //spidev1.0
#define PBP1_SPICS 0
#define PBP1_I2CBUS 1
//PocketBeagle Mikrobus Port 2 GPIO Pins
#define PBP2_AN 86 //P2_35
#define PBP2_RST 45 //P2_33
#define PBP2_CS 19 //P2_31
#define PBP2_SCK 7 //P2_29
#define PBP2_MISO 40 //P2_27
#define PBP2_MOSI 41 //P2_25
#define PBP2_SDA 12 //P1_26
#define PBP2_SCL 13 //P1_28
#define PBP2_RX 43 //P1_30
#define PBP2_TX 42 //P1_32
#define PBP2_INT 26 //P1_34
#define PBP2_PWM 110 //P1_36
#define PBP2_SPIBUS 2 //spidev2.1
#define PBP2_SPICS 1
#define PBP2_I2CBUS 2
//Beaglebone Black Mikrobus Cape Port 1 GPIO Pins
#define BBP1_AN 0 //AIN3
#define BBP1_RST 60 //P9_12
#define BBP1_CS 113 //P9_28
#define BBP1_SCK 110 //P9_31
#define BBP1_MISO 111 //P9_29
#define BBP1_MOSI 112//P9_30
#define BBP1_SDA 12 //P9_20
#define BBP1_SCL 13 //P9_19
#define BBP1_TX 3 //P9_21
#define BBP1_RX 2 //P9_22
#define BBP1_INT 48 //P9_15
#define BBP1_PWM 50 //P9_14
#define BBP1_SPIBUS 1 //spidev1.0
#define BBP1_SPICS 0
#define BBP1_I2CBUS 2
//Beaglebone Black Mikrobus Cape Port 2 GPIO Pins
#define BBP2_AN 0 //AIN2
#define BBP2_RST 49 //P9_23
#define BBP2_CS 7 //P9_42
#define BBP2_SCK 110 //P9_31
#define BBP2_MISO 111 //P9_29
#define BBP2_MOSI 112 //P9_30
#define BBP2_SDA 12 //P9_20
#define BBP2_SCL 13//P9_19
#define BBP2_TX 15 //P9_24
#define BBP2_RX 14 //P9_26
#define BBP2_INT 116 //P9_41
#define BBP2_PWM 51 //P9_16
#define BBP2_SPIBUS 1 //spidev1.1
#define BBP2_SPICS 1
#define BBP2_I2CBUS 2
//Beaglebone Black Mikrobus Cape Port 3 GPIO Pins
#define BBP3_AN 0 //AIN1
#define BBP3_RST 26 //P8_14
#define BBP3_CS 5 //P9_17
#define BBP3_SCK 2 //P9_22
#define BBP3_MISO 3 //P9_21
#define BBP3_MOSI 4 //P9_18
#define BBP3_SDA 12 //P9_20
#define BBP3_SCL 13 //P9_19
#define BBP3_TX 15 //P9_24
#define BBP3_RX 14 //P9_26
#define BBP3_INT 65 //P8_18
#define BBP3_PWM 22 //P8_19
#define BBP3_SPIBUS 0 //spidev0.0
#define BBP3_SPICS 0
#define BBP3_I2CBUS 2
//Beaglebone Black Mikrobus Cape Port 4 GPIO Pins
#define BBP4_AN 0 //AIN0
#define BBP4_RST 46 //P8_16
#define BBP4_CS 68 //P8_10
#define BBP4_SCK 110 //P9_31
#define BBP4_MISO 111 //P9_29
#define BBP4_MOSI 112 //P9_30
#define BBP4_SDA 12 //P9_20
#define BBP4_SCL 13 //P9_19
#define BBP4_TX 31 //P9_13
#define BBP4_RX 30 //P9_11
#define BBP4_INT 27 //P8_17
#define BBP4_PWM 23 //P8_13
#define BBP4_SPIBUS 1 //spidev1.2
#define BBP4_SPICS 2
#define BBP4_I2CBUS 2
/**
 * struct mikrobus_port - Structure that holds the mikrobus port pin mapping
 * @name: port name (PBP1,PBP2,BBP1...etc)
 * @an_gpio: Analog GPIO number
 * @cs_gpio: CS GPIO number
 * @sck_gpio: SCK GPIO number
 * @miso_gpio: MISO GPIO number
 * @mosi_gpio: MOSI GPIO number
 * @sda_gpio: SDA GPIO number
 * @scl_gpio: SCL GPIO number
 * @rx_gpio: RX GPIO number
 * @tx_gpio: TX GPIO number
 * @int_gpio: int GPIO number
 * @pwm_gpio: PWM GPIO number
 * @spi_bus: SPI Bus number
 * @spi_cs: SPI CS number
 * @i2c_bus: I2C Bus number
 */
struct mikrobus_port {
	char name[8];
	unsigned int an_gpio;
    unsigned int rst_gpio;
    unsigned int cs_gpio;
    unsigned int sck_gpio;
    unsigned int miso_gpio;
    unsigned int mosi_gpio;
    unsigned int sda_gpio;
    unsigned int scl_gpio;
    unsigned int rx_gpio;
    unsigned int tx_gpio;
    unsigned int int_gpio;
    unsigned int pwm_gpio;
    uint8_t spi_bus;
    uint8_t spi_cs;
    uint8_t i2c_bus;
};

struct mikrobus_port *get_mikrobus_port(char *port);

#endif /* __LINUX_MIKROBUS_H */