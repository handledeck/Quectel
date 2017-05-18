
#ifndef __SOCKET_TH_H__
#define __SOCKET_TH_H__



#define __SOCKET_CLIENT_COUNT__		5
#define __SCOCET_TH_PORT__			10250
#define __SCOCET_WEB_PORT__			80
#define __SCOCET_DBG_PORT__			10251
#define __SCOCET_UPGRADE_PORT__		10255

extern s32 __socket_th_id;
extern s32 __socket_web_id;
extern s32 __socket_dbg_id;
extern s32 __socket_est_id;
extern u32 __tmr_array[7];
extern u8  __tmr_index;


void socket_th_connect(s32 socketId, s32 errCode, void* customParam);
void socket_th_close(s32 socketId, s32 errCode, void* customParam);
void socket_th_accept(s32 listenSocketId, s32 errCode, void* customParam);
void socket_th_read(s32 socketId, s32 errCode, void* customParam);
void socket_th_write(s32 socketId, s32 errCode, void* customParam);
bool regiser_socket_th();
void est_socket_connect(u32 timerId, void* param);
void est_wait_confirm(u32 timerId, void* param);
void est_empty_msg(u32 timerId, void* param);
bool check_update();

#endif
