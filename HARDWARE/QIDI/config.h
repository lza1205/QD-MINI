
#ifndef __CONFIG_H_
#define __CONFIG_H_

#define P2P_EN_UDP_HLOP		0	//是否启动打洞技术
#define P2P_DEF_AES256		0	//是否开启加密

/* 服务器默认起始端口号 */
#define SERVER_SYNC_PORT			8000

/* TCP 服务器端口号 */
#define SERVER_PORT_TCP				8080


/* TCP 服务器升级端口号 */
#define SERVER_PORT_TCP_UPDATE				8081


/* 版本号 */
#define CLIENT_VERSION		"client-15.07.27.1"	
#define SERVER_VERSION		"server-15.05.15"

/* 调试打印信息 */
#define app_printf(fmt, ...)	if(printf_app_flg)	\
									printf(fmt, ##__VA_ARGS__)

#define dbg_printf(fmt, ...)	if(printf_dbg_flg)	\
									printf("__debug :");	\
									printf(fmt, ##__VA_ARGS__)

#define sync_printf(fmt, ...)	if(printf_sync_flg)			\
									printf(fmt, ##__VA_ARGS__)


/* 同步间隔 */
#define SYNC_TIME				30		
#define LOGIN_CNT_INIT			60		/* 用户活跃状态初始值 */
#define LOGIN_CNT_DIF			30		/* 用户活跃值递减值 */


/* 一次UDP sendto recvfrom 的最大字节数 */
#define UDP_PACK_MAX_SIZE				(10*1024)

/* ID 名长度 */
#define USER_NAME_LEN	20

/* 密码长度 */
#define USER_PASSWD_LEN	10


/* IP 最大长度 */
#define IP_STRING_MAX	20

/* 最多可以存放多少个好友客户端 */
#define P2P_CLIENT_MAX	10




/* 用于校验、做包头 */
/* 4 + 4 + 20 + 20 + 20 + 4 = 72 */
struct check_head{
	unsigned int affairs;	/* 处理事务 */
//	unsigned int type;		/* 类型 0 主机 1 APP*/
	char name[USER_NAME_LEN];
	unsigned int passwd;
	unsigned char customer[USER_NAME_LEN];	//厂家ID
//	char version[30];
//	int timecnt[6];			/* 时间戳: 年月日时分秒 */
	unsigned int key;		/* 数据包唯一编号 */

	int crc;			/* 数据校验 */
};

/* 服务器应答登陆情况 */
struct server_ack_login{
	int ack;				/* 登录是否成功 */
	int master_sta;			/* 主机是否在线 */
};





#define __max(a,b)        (((a) > (b)) ? (a) : (b))
#define __min(a,b)        (((a) < (b)) ? (a) : (b))


#endif

