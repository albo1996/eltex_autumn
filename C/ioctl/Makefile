CURRENT = $(shell uname -r)
KDIR = /lib/modules/$(CURRENT)/build
PWD = $(shell pwd)
DEST = /lib/modules/$(CURRENT)/misc
TARGET = RW_module
obj-m := $(TARGET).o
default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	rm *.ko
	rm *.mod
	rm *.o
	rm *.order
	rm *.symvers
	rm *.mod.c
ioctl: hello.c
	gcc hello.c -o hello
