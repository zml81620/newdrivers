# Makefile for v4l2 application
CROSS_COMPILE = /opt/arm-2009q1/bin/arm-none-linux-gnueabi-
INCLUDE_DIR = /root/dvsdk_2_00_00_22/linux-davinci/include
objs=v4l2_mmap_loopback_zml.o 

libobjs= ASC16.o OsdChar.o

CC = $(CROSS_COMPILE)gcc
AR = $(CROSS_COMPILE)ar

lib:$(libobjs)
	$(CC) -shared -fPIC  -o libosd.so $^		

osdtest: $(objs)
	$(CC) -o osdtest $^ -losd -L.

%.o:%.c
	$(CC) $(CFLAGS) -c $^  -I $(INCLUDE_DIR)/ -I.

clean:
	rm -f *.o *~ core osdtest
