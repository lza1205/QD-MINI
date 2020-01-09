
#include "config.h"
#include "agent_proto.h"
#include "qdy_interface.h"
#include "all.h"
#include "stdio.h"
#include "cJSON.h"

#define SERVER_PORT		8000
#define SERVER_IP		"106.13.62.194"

void *recv_server_data(void *pdata);
void qdy_upload(void);

char *device_id = "lancy";

#define sleep(x)	Delay_nS(x)

//
//

int my_send(unsigned char *buf, int ret)
{
	//sendto(sockfd, buf, ret, 0, (struct sockaddr *)&servaddr, sizeof(struct sockaddr_in));
	//dump_data((unsigned char *)buf, ret);
	esp8266_tcp_send(buf, ret);
	//4G_SEND()
	return 0;
}

int login(void);
unsigned char recvline[1024];

int client_main(void)
{
	
	set_name(device_id, strlen(device_id) + 1);
	set_customer("test", strlen("test") + 1);

	set_net_send(my_send);

	login();

	while(1)
	{
		

		int ret;
		//接受数据
		//ret = recvfrom(sockfd, recvline, 1024, 0, (struct sockaddr*)&addrRecv,(socklen_t*)&sizeRecvAddr);
		
		
		ret = esp8266_tcp_recv(recvline);
		if(ret != 0)
		{
			qdy_recv_data(recvline, ret);
		}

		//心跳包
		if(Heart_beat)
		{
			printf("Heart_beat \r\n");
			qdy_sync_server();
			Heart_beat = 0;

			sleep(1);
			//上传温度湿度数据
			qdy_upload();
		}
	}

    //close(sockfd);
    //return 1;
}



int login(void)
{
	int ret;
	struct check_head *recv_head;

	struct proto_s_login_ack *ack;
	struct check_head head;
	__compages_head(&head, _aff_client_login_, device_id, 1234, "test");
	//暂时不做 crc 随机数 时间等

	//登录
	dump_data((unsigned char *)&head, sizeof(struct check_head));
	printf("%s %d %d \r\n", __FILE__, __LINE__, sizeof(struct check_head));
	//sendto(sockfd, &head, sizeof(struct check_head), 0, (struct sockaddr *)addr, sizeof(struct sockaddr_in));
	esp8266_tcp_send( (unsigned char *)&head, sizeof(struct check_head));

	

	//等待服务器应答
	while(1)
	{

		
		//接受数据
		//ret = recvfrom(sockfd, recvline, 1024, 0, (struct sockaddr*)&addrRecv,(socklen_t*)&sizeRecvAddr);
		ret = esp8266_tcp_recv(recvline);
		if(ret != 0)
		{
			recv_head = (struct check_head *)recvline;
			printf("recv data! \r\n");
			dump_data(recvline, ret);

			if((recv_head->affairs == _aff_server_login_ack_))
				   goto login_ack;
			
			sleep(5);
			continue;
		}

		


		sleep(5);
	}
	
login_ack:
	
	ack = (struct proto_s_login_ack *)(recvline + sizeof(struct check_head));

	if(ack->ack == 0)
	{
		printf("connet is ok!\r\n");
	}else{
		printf("connet is err!\r\n");
	}
	return ack->ack;		//返回登陆情况

}


double get_temperature(void)
{
	static int flg = 0;
	float ret = flg?21.4:21.9;
	flg = !flg;
	return ret;
}

double get_humidity(void)
{
	static int flg = 0;
	float ret = flg?31.7:31.1;
	flg = !flg;
	return ret;
}



void qdy_upload(void)
{
	cJSON *root;
	char *result;
	
	root=cJSON_CreateObject();//创建一个机器人状态的JSON对象

	cJSON_AddNumberToObject(root,"temperature", get_temperature());
	cJSON_AddNumberToObject(root,"humidity", get_humidity());
	result=cJSON_PrintUnformatted(root);

	qdy_send_json_data("__server__", result, strlen(result) + 1);

	cJSON_Delete(root);//最后将root根节点删除
	free(result);//释放result的空间，必须要有，要不然内存里会失去一段空间，最后系统崩溃
}









