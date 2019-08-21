# mikroBUS Device

A simple kernel module to instantiate the devices on the Mikrobus SPI and I2C, for compiling and installing the module perform the 
following commands : 

```
git clone https://github.com/vaishnav98/mikrobus_device.git
cd mikrobus_device
(next command requires the Kernel Headers to be installed : (sudo apt-get install linux-headers-`uname -r`)
make all install
```

## Instantiating a SPI Device
```
 sudo modprobe mikrobus_spi_device name=devicename busno=spibusno csno=spi_chip_select max_speed=SPI_speed_hz irq_gpio=irqgpiono

```
## Instantiating a I2C Device
```
 sudo modprobe mikrobus_i2c_device custom(bool) name=devicename busno=i2cbusno address=devicei2caddress irq_gpio=irqgpiono

```
