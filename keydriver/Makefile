ifneq ($(KERNELRELEASE),)
#hello-objs:=hello.o
obj-m += davinci_matrix_keypad.o

else

KDIR_PC := /lib/modules/$(shell uname -r)/build
KDIR_ARM := /root/dvsdk_2_00_00_22/linux-davinci

PWD := $(shell pwd)

test:	
	make -C $(KDIR_PC) M=$(PWD) modules
arm:	
	make -C $(KDIR_ARM) M=$(PWD) modules
clean:
	rm -fr *.o *.ko *.mod.* *.order *.symvers *.o.*  *.tmp_* *~
	sync

endif
