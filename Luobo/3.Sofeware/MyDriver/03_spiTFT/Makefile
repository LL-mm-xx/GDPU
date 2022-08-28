KERNELDIR := /home/lmx/nanopi/linux
CURRENT_PATH := $(shell pwd)

obj-m := spiTFT.o

build: kernel_modules

kernel_modules:
	make ARCH=arm CROSS_COMPILE=arm-linux- -C $(KERNELDIR) M=$(CURRENT_PATH) modules

clean:
	make ARCH=arm CROSS_COMPILE=arm-linux- -C $(KERNELDIR) M=$(CURRENT_PATH) clean

