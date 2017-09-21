obj-m+=RPi3_GPIO_Kernel.o
all:
 make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
clean:
 make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean