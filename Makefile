# SPDX-License-Identifier: GPL-2.0

# Device modules
obj-m += mikrobus_spi_device.o
obj-m += mikrobus_i2c_device.o
obj-m += mikrobus.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
install:
	sudo cp *.ko /lib/modules/$(shell uname -r)
	sudo depmod -a
