
#include "config.h"
#include "agent_proto.h"
#include "qdy_interface.h"
#include "string.h"

void qdy_resolve_recv_data(unsigned char *buf, int len)
{
	char *recv_data;
	int recv_len;
	struct check_head *head;
	struct proto_c_send_data *recv_proto;
	char *msg = "I fine!";
	
	//dump_data(buf, len);

	head = (struct check_head *)buf;
	recv_proto = (struct proto_c_send_data *)(buf + sizeof(struct check_head));
	recv_data = (char *)(buf + sizeof(struct check_head) + sizeof(struct proto_c_send_data));

	
	printf("recv from %s 's %s data : [%s] \r\n", recv_proto->src_name, 
										((head->affairs == _aff_client_p2p_data_)?"stun":"turn"),
										recv_data);	
	
	//·¢ËÍÊý¾Ý
	
	qdy_send_data(recv_proto->src_name, (unsigned char *)msg, strlen(msg) + 1);
}


