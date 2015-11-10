all:
	arm-none-linux-gnueabi-gcc main.c  ../i2ctest/i2ctest/tca9548.c -o progress-boot  -I .  -I ../i2ctest/i2ctest

copy:
	cp test /opt/nfsboot/rtc/
clean:
	rm -fr *.o *.ko *.mod.* *.order *.symvers *.o.*  *.tmp_* *~
