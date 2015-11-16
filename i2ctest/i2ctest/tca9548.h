#ifndef _LINUX_TCA9548_H
#define _LINUX_TCA9548_H

#define ZML_DEBUG  0



#define BIT(nr)			(1UL << (nr))
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#define I2C_CONTROLLER_NAME "/dev/i2c-1"

#define DISABLE_TCA9548_ALLCH   0x00
#define ENABLE_TCA9548_CH4      0x10
#define ENABLE_TCA9548_CH3      0x08  //connect to PCF8563
#define ENABLE_TCA9548_CH2      0x04  //connect to BQ27621
#define ENABLE_TCA9548_CH0      0x01  //connect to ADV7513 hdmi
#define ENABLE_TCA9548_CH5      0x20  //connect to CH7026  image-fcouser
#define ENABLE_INFRARED_CH6     0x40  //connect to infrared sensor
#define ENABLE_ADT7410_CH3      0x08  //connect to ADT7410


#define SLAVE_BQ27621_ADDR    0x55
#define SLAVE_PCF8563_ADDR    0x51
#define SLAVE_HDMI_ADDR       0x39
#define SLAVE_IMAGER_ADDR     0x76
#define SLAVE_INFRARED_ADDR   0x2F
#define SLAVE_ADT7410_ADDR    0x48


#define CHEM_ID_1202     1
#define CHEM_ID_1210     2
#define CHEM_ID_354      3
#define INVALID_CHEM_ID  0


#define REGOFFSET_CHECKSUM                0x60
#define REGOFFSET_DESIGNCAPCITY           0x43
#define REGOFFSET_DESIGNENERGY            0x45
#define REGOFFSET_TERMINATEVOL            0x49
#define REGOFFSET_TAPERATE                0x54

#define REGOFFSET_VOLTAGE                 0x04
#define REGOFFSET_GETFLAGS                0x06
#define REGOFFSET_GETREMAININGCAPCITY     0x0C
#define REGOFFSET_GETRFULLCHARGECAPCITY   0x0E
#define REGOFFSET_CURRENT                 0x10
#define REGOFFSET_AVGPOWER                0x18
#define REGOFFSET_STATEOFCHARGE           0x1C

//------------ADT7410-----------------------------
#define INVALID_ADT7410_VALUE             -1000.0
//#define TEMP_ALARM_FALGE                  1
#define TEMP_ALARM_THRESHOLD              55
#define TEMP_CANCELALARM_THRESHOLD        50
#define REGOFFSET_ID                      0x0B
#define REGOFFSET_CONFIG                  0x03
#define REGOFFSET_TEMP_HIGHT              0x0


typedef struct tag_i2c_bq27621
{
   unsigned char cmd;
   unsigned short param;
}i2c_bq27621;


int open_master(const char *pname);
void close_master(const int *pfd);

int enable_switcher_chan(const int fd,const int en_chx);
int disable_switcher_chan(const int fd);

int select_slave(const int fd,const unsigned int addr);
int write_device(const int fd,const unsigned char *pdata,const unsigned int len);
int bq27621_readword(const int fd, const unsigned char reg_offset, unsigned char* buf);


int bq27621_unseal(const int fd);
int bq27621_exit_configmode(const int fd);
int bq27621_chg_chemID(const int fd, const unsigned short chemID);
int bq27621_update_blcokram(const int fd, const int type, unsigned char chksum);

int init_bq27621_step1(const int fd);
int init_bq27621_step2(const int fd);
int bq27621_ischarging(const int fd);
int bq27621_stateofcharg(const int fd);
int bq27621_getvoltage(const int fd);
int bq27621_getcurrent(const int fd);
int bq27621_getavgpower(const int fd);

short int irno_getbootprogress(const int fd);

short int adt7410_getTemperature(const int fd,short int *temp_alarm,short int *cancle_alarm);

#endif