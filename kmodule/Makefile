obj-m += kmod.o
kmod-y += kmod-main.o kmod-ioctl.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

install: 
	sudo rmmod kmod || true
	sudo insmod kmod.ko

remove: 
	sudo rmmod kmod || true