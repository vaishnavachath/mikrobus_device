#ifndef __LINUX_MIKROBUS_H
#define __LINUX_MIKROBUS_H

/**
 * struct mikrobus_port - Structure that holds the mikrobus port pin mapping
 * @name: pinname (reset, dc, etc.)
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
 */
struct mikrobus_port {
	char name[8];
	unsigned int an_gpio;
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
};


#endif /* __LINUX_MIKROBUS_H */