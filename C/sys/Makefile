CURRENT = $(shell uname -r)
KDIR = /lib/modules/$(CURRENT)/build
PWD = $(shell pwd)
DEST = /lib/modules/$(CURRENT)/misc
TARGET = hello
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
