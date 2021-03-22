MODULE_NAME := rw_dev
obj-m := $(MODULE_NAME).o

OBJ_LIST := main.o
$(MODULE_NAME)-y += $(OBJ_LIST)

ccflags-y := -O2

KERNELDIR := /lib/modules/$(shell uname -r)/build

all: rw_dev

rw_dev:
	make -C $(KERNELDIR) M=$(PWD) modules
clean:
	rm *.ko
	rm *.mod
	rm *.o
	rm *.order
	rm *.symvers
	rm *.mod.c

