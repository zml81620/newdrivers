all:
	arm-none-linux-gnueabi-gcc imager-control.c  tca9548.c -o imagerctl   -I .

copy:
	cp test /opt/nfsboot/rtc/
clean:
	rm -fr *.o *.ko *.mod.* *.order *.symvers *.o.*  *.tmp_* *~
