
#ifndef __QDY_INTERFACE_H__
#define __QDY_INTERFACE_H__

#include "config.h"
#include "agent_proto.h"


int qdy_send_data(unsigned char *name, char *data, int len);
void __qdy_recv_data(unsigned char *buf, int len);
void qdy_recv_data(unsigned char *recvbuf, int len);
void __compages_head(struct check_head *head, unsigned int air, char *name, unsigned int passwd, char *customer);
int qdy_get_len(int len);
char *qdy_get_dest_name(char *buf);
char *qdy_get_src_name(char *buf);
char *qdy_get_data(char *buf);
void update_head(struct check_head *head, unsigned int air);
int check_head_crc(struct check_head *head);
int __crc(char *buf, int len);
void set_net_send(int (*send)(unsigned char *buf, int len));
void qdy_resolve_recv_data(unsigned char *buf, int len);
void set_name(char *name, int len);
void dump_data(unsigned char *buf, int len);
int qdy_sync_server(void);



#endif

