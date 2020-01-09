

#define __GSM_BASE_C__

#include "all.h"


 vu32 __delay_uart = 0;
 vu8 _gsm_data_flg = 0;

/*******************************************************************************
* 函数名  : USART2_IRQHandler
* 描述    : 串口1中断服务程序
* 输入    : 无
* 返回    : 无 
* 说明    : 
*******************************************************************************/
void USART2_IRQHandler(void)                	
{
//	    if (USART_GetFlagStatus(USART2, USART_FLAG_PE) != RESET)
//	{
//			USART_ReceiveData(USART2);
//			USART_ClearFlag(USART2, USART_FLAG_PE);
//		}
//		if (USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET)
//		{
//			USART_ReceiveData(USART2);
//			USART_ClearFlag(USART2, USART_FLAG_ORE);
//		}
//		if (USART_GetFlagStatus(USART2, USART_FLAG_FE) != RESET)
//		{
//			USART_ReceiveData(USART2);
//			USART_ClearFlag(USART2, USART_FLAG_FE);
//		}
    
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {   
			
			u8 Res=0;
			Res =USART_ReceiveData(USART2);
			Uart2_Buf[First_Int] = Res;  	  //将接收到的字符串存到缓存中
			First_Int++;                			//缓存指针向后移动
			if(First_Int > Buf2_Max)       		//如果缓存满,将缓存指针指向缓存的首地址
			{
				First_Int = 0;
			}

			__delay_uart = 2;
			_gsm_data_flg = 1;
        USART_ClearFlag(USART2, USART_FLAG_RXNE);
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
        
    }
}


void clean_delay_uart(void)
{
	__delay_uart = 0;
	_gsm_data_flg = 0;
}

#define min(x,y) ( (x)>(y)?(y):(x) )

u16 recv_gsm_data(u8 *buf, u16 size)
{
	u32 len = min(First_Int, size);
	
	if(__delay_uart > 0)
	{

		return 0;
	}
	else
	{
		//printf("recv data \r\n");
		if(_gsm_data_flg)
		{
			printf("recv data \r\n");
			_gsm_data_flg = 0;

			memcpy(buf , Uart2_Buf, len);

			return len;
		}
		return 0;
	}
}

vu16 Heartbeat_time = 10;

void set_Heartbeat_time(u8 time)
{
	Heartbeat_time = time;
}

/*******************************************************************************
* 函数名  : TIM2_IRQHandler
* 描述    : 定时器2中断断服务函数
* 输入    : 无
* 输出    : 无
* 返回    : 无 
* 说明    : 无
*******************************************************************************/
void TIM2_IRQHandler(void)   //TIM3中断
{
	static u8 flag =1;

	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx更新中断标志 
		
		if(Timer0_start)
		{
			Times++;
		}
		if(Times > shijian)
		{
			Timer0_start = 0;
			Times = 0;
		}

		if(__delay_uart > 0)
		{
			__delay_uart -- ;
		}

		Heartbeat++;
		if(Heartbeat > Heartbeat_time)//每90秒发送心跳帧
		{
			Heartbeat=0;
			Heart_beat=1;
		}
		if(flag)
		{ 
			flag=0;
		}
		else
		{
			flag=1;
		}
	}	
}



void gsm_base_send(char b)
{
	USART_SendData(USART2, b);//UART2_SendData(*b);
}


