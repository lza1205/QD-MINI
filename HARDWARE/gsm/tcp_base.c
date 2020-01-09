

#define __TCP_BASE_C__

#include "all.h"
#include "gsm_base.h"
#include "stdio.h"
//TCP 连接
const char *__string = "AT+CIPSTART=\"UDP\",\"106.13.62.194\",8000";	//IP登录服务器



/*******************************************************************************
* 函数名 : Connect_Server
* 描述   : GPRS连接服务器函数
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void Connect_Server(char *string)
{
	UART2_SendString("AT+CIPCLOSE=1");	//关闭连接
  Delay_nMs(100);
	Second_AT_Command("AT+CIPSHUT","SHUT OK",2);		//关闭移动场景
	Second_AT_Command("AT+CGCLASS=\"B\"","OK",2);//设置GPRS移动台类别为B,支持包交换和数据交换 
	Second_AT_Command("AT+CGDCONT=1,\"IP\",\"CMNET\"","OK",2);//设置PDP上下文,互联网接协议,接入点等信息
	Second_AT_Command("AT+CGATT=1","OK",2);//附着GPRS业务
	Second_AT_Command("AT+CIPCSGP=1,\"CMNET\"","OK",2);//设置为GPRS连接模式
	Second_AT_Command("AT+CIPHEAD=1","OK",2);//设置接收数据显示IP头(方便判断数据来源,仅在单路连接有效)
	if(string != NULL)
	{
		Second_AT_Command((char*)string,"OK",5);
	}
	else
	{
		Second_AT_Command((char*)__string,"OK",5);
	}
	Delay_nMs(100);
	CLR_Buf2();
}
/*******************************************************************************
* 函数名 : Rec_Server_Data
* 描述   : 接收服务器数据函数
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void Rec_Server_Data(void)
{
	#if 0
	if(strstr(Uart2_Buf,"+IPD")!=NULL)   		//若缓存字符串中含有^SISR
	{	
		Heartbeat=0;	//清除心跳帧计数器
		Heart_beat=0;
		Delay_nMs(100);
		if(strstr(Uart2_Buf,"onled")!=NULL)
		{
			LED1_ON();
		}
		else if(strstr(Uart2_Buf,"offled")!=NULL)
		{
			LED1_OFF();
		}
		UART1_SendString("收到新信息：\r\n");
		UART1_SendString(Uart2_Buf);
		CLR_Buf2();
		Heart_beat=1;//发送应答数据，告诉服务器收到数据		
	}
	#endif
}
/*******************************************************************************
* 函数名 : tcp_heart_beat
* 描述   : 发送数据应答服务器的指令，该函数在有两功能
					1：接收到服务器的数据后，应答服务器
					2：服务器无下发数据时，每隔10秒发送一帧心跳，保持与服务器连接
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/

static void Send_OK(void)
{
	Second_AT_Command("AT+CIPSEND",">",2);
	Second_AT_Command("OK\32\0","SEND OK",8);;			//回复OK 
}

void tcp_heart_beat(void)
{
	if(Heart_beat)
	{
		Send_OK();
		Heart_beat=0;
	}
}



static u8 send_buf[1024];
void tcp_send(u8 *buf, int len)
{
	u8 ret;

	memset(send_buf, 0, sizeof(send_buf));
	sprintf(send_buf, "AT+CIPSEND=%d", len);

	printf("%s\r\n", send_buf);
	ret = Second_AT_Command_Try(send_buf,">",4);

	if(ret == 1)
	{
		//printf("%s %d %d \r\n", __FILE__, __LINE__, len);
		Second_AT_Data((char *)buf,"SEND OK",len, len);;			//回复OK 
	}else{
		gsm_base_send(0x1A);
	}

	/*
	测试用的函数
	*/
}


void esp8266_tcp_send(u8 *buf, int len)
{
	uart_send_data(buf,len);
}

int esp8266_tcp_recv(u8 *buf)
{
	int ret = recv_gsm_data(buf, 2048);
	if(ret != 0)
	{
		CLR_Buf2();
	}
	return ret;
}


u32 tcp_recv(u8 * buf)
{
	u32 ret = 0;
	u8 offset = 0;
	char *p;
	
	if(strstr(Uart2_Buf,"+IPD")!=NULL)   		//若缓存字符串中含有^SISR
	{
		Delay_nMs(100);

		/*
		printf_s("收到新信息：\r\n");

		printf_s(Uart2_Buf);
		printf_s("\r\n");
		*/
		p = Uart2_Buf;
		while(1)
		{
			offset ++;
			if(*p == ':')
			{
				break;
			}
			p ++;
			
		}
		
		memcpy(buf, Uart2_Buf + offset, First_Int);
		ret = First_Int - offset;
/*
		printf_hex((u8 *)Uart2_Buf, First_Int);
		printf_s("\r\n");
		*/
		
		CLR_Buf2();
	}

	return ret;
	/*
	测试用的函数
	*/
}


