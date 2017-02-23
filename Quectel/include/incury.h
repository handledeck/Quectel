#ifndef __INCURY_H__
#define __INCURY_H__

#include <ql_type.h>
#include <ql_memory.h>

#define RS_LEN_BUFFER 512

#define RS_LEN_BUFFER		512
#define LEN_START_END		4
#define LEN_MARK_TIME		4
#define LEN_MARK_MSG		1
#define LEN_MARK_SERVICE	LEN_START_END+LEN_MARK_TIME+LEN_MARK_MSG

extern u8	__rs_r_buf[RS_LEN_BUFFER];
extern u8	__rs_s_buf[RS_LEN_BUFFER];
extern u16 __count_r_buf;



enum EnFunc {
	none=0,
	state=1,
	through=2,
	evti=3,
	evto=4,
	evta=5,
	getsettings=6,
	setsettings=7,
	debug=100,
	
};

void data_to_hex(u8* data, u8 count);
void check();
void add(u8* data, u16 count);
void extract(u8* data);
void mdm_msg_send(u8* data,u16 len);

#endif