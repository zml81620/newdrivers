# Makefile for fbdev applications

CROSS_COMPILE = /opt/arm-2009q1/bin/arm-none-linux-gnueabi-
CC = $(CROSS_COMPILE)gcc
HEADER_DIR = /root/dvsdk_2_00_00_22/linux-davinci/include

all: boot-app

boot-app: main.o
	$(CC) -o boot-app $^

%.o:%.c
	$(CC) $(CFLAGS) -c $^ -I $(HEADER_DIR) -I .

clean:
	rm -f *.o *~ core boot-app