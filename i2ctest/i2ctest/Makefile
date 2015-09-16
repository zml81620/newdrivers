all:
	arm-none-linux-gnueabi-gcc test.c  tca9548.c -o i2ctest  -I .

copy:
	cp test /opt/nfsboot/rtc/
clean:
	rm -fr *.o *.ko *.mod.* *.order *.symvers *.o.*  *.tmp_* *~
