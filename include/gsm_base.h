
#ifndef __GSM_BASE_H__
#define __GSM_BASE_H__


#ifdef __GSM_BASE_C__
#define __GSM_BASE_EXT__
#else
#define __GSM_BASE_EXT__	extern
#endif

#include "all.h"


//#define Buf1_Max 60 					  //串口1缓存长度
#define Buf2_Max 2000 					  //串口2缓存长度


/*************  本地变量声明	**************/
//char Uart1_Buf[Buf1_Max];
__GSM_BASE_EXT__ char Uart2_Buf[Buf2_Max];

__GSM_BASE_EXT__ u32 First_Int;
__GSM_BASE_EXT__ u8 Times ,shijian;
__GSM_BASE_EXT__ u16 Heartbeat;

__GSM_BASE_EXT__ vu8 Timer0_start;	//定时器0延时启动计数器
__GSM_BASE_EXT__ vu8 Uart2_Start;	//串口2开始接收数据
__GSM_BASE_EXT__ vu8 Uart2_End;	  //串口2接收数据结束
__GSM_BASE_EXT__ vu8 Heart_beat;		//发送心跳帧标志位


void set_Heartbeat_time(u8 time);
void clean_delay_uart(void);
u16 recv_gsm_data(u8 *buf, u16 size);
void gsm_base_send(char b);


#endif/*__GSM_BASE_H__*/


