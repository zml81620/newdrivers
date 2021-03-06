#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <linux/types.h>
#include <string.h>

#include <linux/i2c-dev.h>
#include <linux/i2c.h>

#include "tca9548.h"
#include "cmd.h"


static void pabort(const char *s)
{
	perror(s);
	abort();
}

int last_cmd = 0;

struct tagFiledata
{
	unsigned char reg_offset;
	unsigned char  data;
};

char *format[4] =
{
	[0] = "reg_dc=0x%2x,L=0x%2x,H=0x%2x\n",
	[1] = "reg_de=0x%2x,L=0x%2x,H=0x%2x\n",
	[2] = "reg_tv=0x%2x,L=0x%2x,H=0x%2x\n",
	[3] = "reg_tr=0x%2x,L=0x%2x,H=0x%2x\n"
};


const char *command[] =
{
   "0:quite",\
   "1:Enable chan2",\
   "2:Disable chan2",\
   "3:Enable chan3",\
   "4:Disable chan3",\
   "5:Init the bq27621",
   "6:Enable chan4",\
   "7:Disable chan4",\
   "8:HDMI test",\
   "9:Imager test",\
   "10:infrared-sensor test",\
   "11:ADT7410 test"
};



void show_command(void)
{
   int i;
   for(i=0;i<ARRAY_SIZE(command);i++)
   {
	  if(command[i])
	  {
		printf("\n%s\n",command[i]);
      }
   }
}

void send_cmd(const int fd,const unsigned char cmd[][2],int cnt)
{
	int i,ret;

	for (i = 0; i < cnt; i++)
	{
		ret = write(fd,cmd[i],sizeof(cmd[i]));
		printf("\nSend cmd[%d][0]=0x%0x,cmd[%d][1]=0x%0x %s,\n",i,\
			(unsigned char)(cmd[i][0]), i, (unsigned char)(cmd[i][1]), (ret>0) ? "successfully" : "failed");
	}
}

#define SLAVE_INFRARED_ADDR 0x2F

int main(int argc,char *argv[])
{
    int fd = -1 ;

    unsigned char val = DISABLE_TCA9548_ALLCH; // CMD_ENABLE_TCA9548_CHX(TCA9548_CH3_PCF8561);
    int len = 0 ;
    int ret = -1;
    int in_val = 1;
    int temp;
    int i;
    unsigned char nchksum = 0;
    unsigned char newchksum = 0;
    unsigned char status = 0;
    unsigned char ino_rdbuf[3] = {0,0,0};
    unsigned char ino_wrbuf[4] = {0,0,0};
    struct i2c_rdwr_ioctl_data ioctl_data;
    struct i2c_msg msgs[2];
    unsigned long arg_func;
    unsigned char val_buf[8] = {0} ;
    unsigned short value = 0;


    unsigned char old_chksum = 0x0;
    unsigned char old_dc[2] = {0x0};
    unsigned char old_de[2] = {0x0};
    unsigned char old_tv[2] = {0x0};
    unsigned char old_tr[2] = {0x0};
    unsigned char bufcmd[2] = {0x0};

    short adt_temp = 0.0;
    short temp_alarm = 0;
    short cancel_alarm = 0;

   fd = open_master(I2C_CONTROLLER_NAME);
   if (fd < 0)
   {
	pabort("can't open device");
   }
  
   while(in_val)
   {

	  show_command();
	  printf("\ninput your choice\n");
	  scanf("%d",&temp);
	  switch(temp)
	  {
		 case 0:
		 {
           in_val = 0;
		 }
		 break;
		 case 1:
		 {
		   len = enable_switcher_chan(fd,ENABLE_TCA9548_CH2);
		   printf("\nlen=%d,val=%x\n",len,ENABLE_TCA9548_CH2);		   
		 }
		 break;
		 case 3:
		 {
		   len = enable_switcher_chan(fd,ENABLE_TCA9548_CH3);
		   printf("\nlen=%d,val=%x\n",len,ENABLE_TCA9548_CH3);
		 }
         break;
		 case 2:
		 case 4:
		 case 7:
		 {
		   disable_switcher_chan(fd);
		 }
		 break;
		 case 5:
		 {
#if 0
		   enable_switcher_chan(fd, ENABLE_TCA9548_CH2);
		   select_slave(fd, SLAVE_BQ27621_ADDR);
		   if (last_cmd == 0)
		   {
		      ret = init_bq27621(fd);
		      printf("\n init_bq27621,return value=%d \n", ret);
			  last_cmd = 1;
		   }
		   ret = bq27621_ischarging(fd);
		   printf("\n bq27621_ischarging,return value=0x%x \n",ret);		   
		   ret = bq27621_stateofcharg(fd);
		   printf("\n bq27621_stateofcharg,return value=0x%x \n",ret);
		   ret = bq27621_getremainingcapcity(fd);
		   printf("\n bq27621_getremainingcapcity,return value=0x%x \n",ret);
		   ret = bq27621_getfullchargecapcity(fd);
		   printf("\n bq27621_getfullchargecapcity,return value=0x%x \n",ret);
		   ret = bq27621_getcurrent(fd);
		   printf("\n bq27621_getcurrent,return value=0x%x \n",ret);		     
		   ret = bq27621_getvoltage(fd);
		   printf("\n bq27621_getvoltage,return value=0x%x \n", ret);
		   disable_switcher_chan(fd);
#endif
		 }
		 break;
		 case 6:
		 {
		   unsigned char cmd[] = {0x0f,0x37};
		   unsigned char cmd_cfg[] = {0x1f,0x3,0x0b};

		   len = enable_switcher_chan(fd,ENABLE_TCA9548_CH4);
		   printf("\nlen=%d,val=%x\n",len,ENABLE_TCA9548_CH4);
		   len = select_slave(fd,0x2c);
		   printf("\n select_slave,len=%d \n",len);
		   len = write_device(fd,cmd,sizeof(cmd));
		   printf("\n write data to OV5640,len=%d \n",len);

		   len = write_device(fd,cmd,sizeof(cmd_cfg));
		   printf("\n write data to OV5640,len=%d \n",len);
		 }
		 break;
		 case 8:
		 {
		   len = enable_switcher_chan(fd, ENABLE_TCA9548_CH0);
		   printf("\n enable ch0 %s\n", (len>0)?"successfully":"failed");
		   len = select_slave(fd, SLAVE_HDMI_ADDR);
		   printf("\n select_slave to 0x%x %s\n", SLAVE_HDMI_ADDR,(len == 0) ? "successfully" : "failed");
		   send_cmd(fd,hdmi_cmd, ARRAY_SIZE(hdmi_cmd));
		   disable_switcher_chan(fd);
		 }
		 break;
		 case 9:
		 {
		   len = enable_switcher_chan(fd, ENABLE_TCA9548_CH5);
		   printf("\n enable ch5 %s\n", (len>0) ? "successfully" : "failed");
		   len = select_slave(fd, SLAVE_IMAGER_ADDR);
		   printf("\n select_slave to 0x%x %s\n", SLAVE_IMAGER_ADDR,(len == 0) ? "successfully" : "failed");
		   send_cmd(fd,imager_cmd, ARRAY_SIZE(imager_cmd));	
		   disable_switcher_chan(fd);
		 }
		 break;
		 case 10:
		 {
#if 0
                   cmd_infrared[0] = 0x10;
                   cmd_infrared[1] = 0x2;
                   cmd_infrared[2] = 0x0;

		   len = enable_switcher_chan(fd, ENABLE_INFRARED_CH6);
		   printf("\n enable ch6 %s\n", (len>0) ? "successfully" : "failed");
		   len = select_slave(fd, SLAVE_INFRARED_ADDR);
		   printf("\n select_slave to 0x%x %s\n", SLAVE_INFRARED_ADDR,(len == 0) ? "successfully" : "failed");
		   len = write(fd,cmd_infrared,1);
                   printf("\n write cmd=0x%x %s\n",cmd_infrared[0],(len>0) ? "successfully" : "failed");
                   (len>0) ? "successfully" : "failed",len,cmd_infrared[4],cmd_infrared[5]);
                   len = -1;
		   len = write(fd,&cmd_infrared[0],3);
                   printf("\n write cmd %s\n",(len>0) ? "successfully" : "failed");
                   len = -1;
		   len = write(fd,&cmd_infrared[2],1);
                   printf("\n write cmd=0x%x %s\n",cmd_infrared[2],(len>0) ? "successfully" : "failed");
                   len = -1;
                   len = read(fd,&cmd_infrared[6],2);
                   printf("\n read cmd %s,len=%d,cmd_infrared[6]=0x%x,cmd_infrared[7]=0x%x\n",\
                    (len>0) ? "successfully" : "failed",len,cmd_infrared[6],cmd_infrared[7]);
#else
		   len = enable_switcher_chan(fd, ENABLE_INFRARED_CH6);
		   printf("\n enable ch6 %s\n", (len>0) ? "successfully" : "failed");
		   len = select_slave(fd, SLAVE_INFRARED_ADDR);
		   printf("\n select_slave to 0x%x %s\n", SLAVE_INFRARED_ADDR,(len == 0) ? "successfully" : "failed");

                   len = ioctl(fd,I2C_FUNCS,&arg_func);
                   printf("\n write cmd %s,len = %d,func=0x%x,\n",(len==0) ? "successfully" : "failed",len,arg_func);

                   printf("\n please input the value to write\n");
                   //scanf("%d",&value);
                   memset(val_buf,0,sizeof(val_buf));
                   read(STDIN_FILENO,&val_buf,sizeof(val_buf));
                   value = atoi(val_buf);
                   printf("\n input value=0x%x\n",value);

                   msgs[0].addr = SLAVE_INFRARED_ADDR;
                   msgs[0].len = 4;
                   msgs[0].flags = 0;
                   ino_wrbuf[0] = 0x10;
                   ino_wrbuf[1] = 0x02;
                   ino_wrbuf[2] = (unsigned char)(value>>8);
                   ino_wrbuf[3] = (unsigned char)(value);
                   msgs[0].buf = ino_wrbuf;
                   ioctl_data.nmsgs = 1;
                   ioctl_data.msgs = &msgs[0];
                   len = ioctl(fd,I2C_RDWR,&ioctl_data);
                   printf("\n write cmd %s,len = %d\n",(len>0) ? "successfully" : "failed",len);

#if 0
                   ino_wrbuf[0] = 0x10;
                   //ino_wrbuf[0] = 0x10;
                   ino_wrbuf[1] = 0x14;
                   msgs[0].addr = SLAVE_INFRARED_ADDR;
                   msgs[0].len = 2;
                   msgs[0].flags = 0;
                   msgs[0].buf = ino_wrbuf;

                   //ino_rdbuf[0] = SLAVE_INFRARED_ADDR;
                   ino_rdbuf[0] = 0x0;
                   ino_rdbuf[0] = 0x0;

                   msgs[1].addr = SLAVE_INFRARED_ADDR;
                   msgs[1].len = 2;
                   msgs[1].flags |= I2C_M_RD;
                   msgs[1].buf = ino_rdbuf;

                   ioctl_data.nmsgs = 2;
                   ioctl_data.msgs = msgs;
                   len = ioctl(fd,I2C_RDWR,&ioctl_data);
                   printf("\n read %s,len = %d,ino_rdbuf[0]=0x%x,ino_rdbuf[1]=0x%x,ino_rdbuf[2]=0x%x\n",\
                          (len>0) ? "successfully" : "failed",len,ino_rdbuf[0],ino_rdbuf[1],ino_rdbuf[2]);
                   sleep(3);
#endif
#endif
                   disable_switcher_chan(fd);
		 }
		 break;
                 case 11:
                 {
#if 1
                   cancel_alarm = 0;
                   temp_alarm = 0;
                   adt_temp = adt7410_getTemperature(fd,&temp_alarm,&cancel_alarm);
                   printf("read temp value,value=%d,alarm_flage=%d,cancel_flage=%d\n",adt_temp,temp_alarm,cancel_alarm);
#else
                   enable_switcher_chan(fd, ENABLE_ADT7410_CH3);
                   select_slave(fd, SLAVE_ADT7410_ADDR);

                   ino_wrbuf[0] = REGOFFSET_ID;
                   ino_rdbuf[0] = 0;
                   len = write(fd,ino_wrbuf,1);
                   printf("1 read ID,value=0x%x\n",len);
                   len = read(fd,ino_rdbuf,1);
                   printf("2 read ID,value=0x%x\n",ino_rdbuf[0]);
                   disable_switcher_chan(fd);
#endif
                 }
                 break;
	  }//switch
   }//while()   
   close_master(&fd);
}