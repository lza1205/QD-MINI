
#ifndef __LOCK_H__
#define __LOCK_H__

#include "all.h"

void lock_switch(u8 swh, u8 of);
u8 get_output_port(void);
u8 get_check_port(void);
void get_local_iccid(u8 *buf, u8 len);
void __sys_id_init(u8 *buf);

void lock_init(void);



#endif	/*__LOCK_H__ */

