
#ifndef __TCP_3A_H__
#define __TCP_3A_H__

#include "all.h"

#ifdef __TCP_3A_C__
#define __TCP_3A_EXT__
#else
#define __TCP_3A_EXT__ extern
#endif

//IP登录服务器

#define SERVER_3A_ADDR "AT+CIPSTART=\"TCP\",\"106.13.62.194\",6666"	


#define PRO_3A_HEAD_1	0x3a
#define PRO_3A_HEAD_2	0x3a

#define PRO_3A_END_1	0x5D
#define PRO_3A_END_2	0x5D



//协议部分

//登录
#define CLIENT_LOGIN 		0x1
#define SERVER_LOGIN_ACK 	0x1

//心跳
#define CLIENT_HEART 		0x2
#define SERVER_HEART_ACK 	0x2

//远程控制终端
#define SERVER_SET_SWITCH			0x0a
#define CLIENT_SET_SWITCH_ACK		0x0a

//终端检测开关（K1/K2），终端上传信息到服务器，服务器不做返回
#define CLIENT_UPLOAD_SWITCH			0x0b

//远程查询状态
#define SERVER_GET_SWITCH			0xFA
//终端返回（输出口和检测口的电平状态）
#define CLIENT_GET_OUT_POINT_ACK		0xFA
#define CLIENT_GET_CHECK_POINT_ACK		0xFB


//ICCID查询
#define SERVER_GET_ICCID			0xF6
#define CLIENT_GET_ICCID_ACK		0xF6



//状态机部分
typedef enum
{
	MACHINE_STATUS_INIT,
	MACHINE_STATUS_CONNECT,
  	MACHINE_STATUS_LOGIN,
	MACHINE_STATUS_IDLE,

  
  MACHINE_STATUS_NUM,
}MACHINE_STATUS_TYPE;

typedef enum
{
	INDEX_HEAD_1 = 0,
  	INDEX_HEAD_2,
	INDEX_CMD,
	INDEX_LEN_1,
	INDEX_LEN_2,

	INDEX_DATA_1,
	INDEX_DATA_2,
	
}PRO_3A_DATA_INDEX;

__TCP_3A_EXT__ void loop_3a_machine(void);

__TCP_3A_EXT__ void machine_set_status(MACHINE_STATUS_TYPE status);

__TCP_3A_EXT__ MACHINE_STATUS_TYPE machine_get_status(void);

#endif

