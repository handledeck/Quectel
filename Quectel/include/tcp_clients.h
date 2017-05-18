#ifndef __TCP_CLIENT__
#define __TCPC_LIENT__

#include <qlobal.h>

#define MAX_CLIENT_COUNT	20

typedef enum {
	TP_NONE		=0,
	THROUGH		=1,
	WEB			=2,
	DBG			=3,
	EST			=4,
	UPGRADE		=5
}enum_socket_type;

typedef struct
{
	enum_socket_type en_sock_type;
	s8    socketId;
	bool  IsGreeting;  // Make a mark, whether to send a greeting, when the client connected to the server.
	u64   nSentLen;    // Bytes of number sent data through current socket    
	char*  send_buf;//[SEND_BUFFER_LEN];
	char*  recv_buf;//[RECV_BUFFER_LEN];
	s32   sendRemain_len;
	s32   recvRemain_len;
	char  *pSendCurrentPos;
	char  *pRecvCurrentPos;
	s32(*send_handle)(s32 sock, char *PData);
	s32(*send_ack)(s8 sock);
	s32(*read_handle_callback)(s32 socket, s32 errCode, void* customParam, s32 index);
}QlClient;

static QlClient m_client[MAX_CLIENT_COUNT];
#define SEND_BUF_LEN	10240
#define RECV_BUF_LEN	1024

extern char __s_buf[];
extern char __r_buf[];

s32 func_send_ack(s8 sock);
s32 func_read_handle_callback(s32 socket, s32 errCode, void* customParam, s32 index);
s32 func_send_handle(s8 sock, char *PDtata, void* params);
s32 func_send_ack(s8 sock);
s32 func_read_handle_callback(s32 socket, s32 errCode, void* customParam, s32 index);
s32 find_client_by_sockid(s8 sockid);
void client_socket_id_init();
void client_init(s32 index, s8 sockid, enum_socket_type tp);
void client_uninit(s32 index);
bool check_is_allsoc_close();
s16 insert_socket(u16 accept_socket_id, enum_socket_type tp);
QlClient* get_client_by_socketid(s32 socketId);

#endif
