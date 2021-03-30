CURRENT=$(shell uname -r)
KDIR=/lib/modules/$(CURRENT)/build
TARGET=block_port_module

obj-m += $(TARGET).o

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
