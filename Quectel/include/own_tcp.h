#ifndef __OWN_TCP_H__
#define __OWN_TCP_H__

#include <ql_type.h> 

typedef enum {
	STATE_NONE			= 0,
	STATE_REGISTER		= 1,
	STATE_CONFIG		= 2,
	STATE_ACTIVATE		= 3,
	STATE_ACTIVATING	= 4,
	STATE_LOCALIP		= 5,
	STATE_EST_CONNECT	= 6,
	STATE_WAITING		= 7,
	STATE_CHECK_EVENTS	= 8,
	STATE_SYNC_SAM		= 9,
	STATE_EST_STOP		= 10
	
}STATE_TCP;

extern STATE_TCP _n_state;
extern STATE_TCP _n_current;


void callback_tcp_actived(u8 contexId, s32 errCode, void* customParam);
void callBack_tcp_deactived(u8 contextId, s32 errCode, void* customParam);
void tcp_main_function(void);

#endif
