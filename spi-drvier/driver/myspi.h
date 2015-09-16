#ifndef _MYSPI_H
#define _MYSPI_H

///////////////////////////
// PROTOTYPES
//////////////////////////
/* Declaration of dv_spi.c functions */
enum  spi_commands{Cmd_reset=1,Cmd_enspi=3,Cmd_dataformat=4,Cmd_Cshold=5,Cmd_Selchip=6};
enum  spi_selchip{Sel_chip0=1,Sel_chip1=2};
enum  spi_cshold{Cs_active_unhold=0,Cs_active_hold=1};
enum  spi_dataformat{format_8bit=1,format_16bit=2};



#endif
