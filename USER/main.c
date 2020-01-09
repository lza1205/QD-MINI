/**********************************************************************************
 * 工程名  :GPRS
 * 作者    :lza1205
 * 博客    :https://blog.csdn.net/aa120515692
**********************************************************************************/

#include "stm32f10x.h"
#include "usart.h"
#include "SysTick.h"
#include "timer.h"
#include "string.h"


#include "gsm_code.h"
#include "gsm_base.h"
#include "tcp_base.h"

#include "all.h"

extern int client_main(void);
char buf[2000];
u32 ret;
/*******************************************************************************
* 函数名 : main 
* 描述   : 主函数
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 串口2负责与MG323模块通信，串口1用于串口调试，可以避免在下载程序时数据
					 还发送到模块
*******************************************************************************/
int main(void)
{
	SysTick_Init_Config();

	USART1_Init_Config(115200);
	USART2_Init_Config(115200);
	Timer2_Init_Config();
	
	Delay_nMs(2000);
	UART1_SendString("设置客户端模式\r\n");
	Second_AT_Command("AT+CWMODE=1","OK",1);
	Set_ATE0();
	UART1_SendString("扫描wifi\r\n");

	ret = Second_AT_Command_Recv("AT+CWLAP","OK", buf, 2);
	UART1_SendData((u8 *)buf, ret);
/*
	if(strstr(buf,"HUAWEI")!=NULL)
	{
		UART1_SendString("连接wifi\r\n");
		//lianjie
		Second_AT_Command("AT+CWJAP_DEF=\"HUAWEI\",\"076812345678\"", "OK", 10);
	}
	*/
	Second_AT_Command("AT+CWJAP_DEF=\"HUAWEI\",\"076812345678\"", "OK", 10);
	//Second_AT_Command("AT+CWJAP_DEF=\"Tenda_29F368\",\"wx133889\"", "OK", 10);

	//单连接
	Second_AT_Command("AT+CIPMUX=0","OK",1);
	
	//tcp 连接到服务器
	Second_AT_Command("AT+CIPSTART=\"TCP\",\"106.13.62.194\",8080","OK",10);
	//Second_AT_Command("AT+CIPSTART=\"UDP\",\"106.13.62.194\",8000","OK",10);
	
	//Connect_Server(NULL);
	UART1_SendString("连接 wifi 成功\r\n");
	//进入透传模式
	Second_AT_Command("AT+CIPMODE=1","OK",1);
	Second_AT_Command("AT+CIPSEND","OK",1);
	while(1)
	{
		client_main();
	}
}

