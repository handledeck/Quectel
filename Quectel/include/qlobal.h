#ifndef __GLOBAL__
#define __GLOBAL__

#include "ql_stdlib.h"
#include "ql_error.h"
#include "ql_trace.h"
#include "ql_uart.h"
#include <ql_type.h>
#include "ql_system.h"
#include <ql_timer.h>
#include <ql_time.h>
#include <ql_fs.h>
#include <ril.h>
#include <ril_sim.h>
#include <ril_network.h>
#include <ril_util.h>
#include <ril_system.h>
#include <ql_gprs.h>
#include <ql_socket.h>
#include <own_tcp.h>
#include <own_settings.h>
#include <utils.h>
#include <socket_th.h>
#include <tcp_clients.h>
#include <websrv.h>
#include <events.h>
#include "incury.h"
#include <calendar.h>
#include <rtc.h>
#include "sunrise.h"
#include "menu.h"
#include <ql_wtd.h>
#include <ql_fota.h>

//I			input	16
//O			output	8
//A			analog	4
//L			lights
//D			doors
//C			counter
//FOTA		Remote update support

#define __VERSION__	"I16O2A2-LDC-3-FOTA"

#define MSG_TCP_REGISTER   (MSG_ID_USER_START + 3)
#define DEBUG_ENABLE 1
#if DEBUG_ENABLE > 0
#define DEBUG_PORT  UART_PORT2
#define DBG_BUF_LEN   512

char DBG_BUFFER[DBG_BUF_LEN];
#endif

#define	GPRS_REGISTER_TIMER		TIMER_ID_USER_START+1
#define	GPRS_ACTIVATE_TIMER		TIMER_ID_USER_START+2
#define	EST_CONNECTOR_TIMER		TIMER_ID_USER_START+3
#define EST_CONFIRM_TIMER		TIMER_ID_USER_START+4
#define EST_EMPTY_TIMER			TIMER_ID_USER_START+5
#define SYNC_SAM_MSG			TIMER_ID_USER_START+6
#define SYSTEM_REBOOT			TIMER_ID_USER_START+7
#define FILE_UPDATE_TRANSMIT	TIMER_ID_USER_START+8

typedef enum
{
	U_NONE,
	U_SAM,
	U_MDM,
}updater;


static ST_Time __tm;
extern bool __DEBUG__;
extern bool __MENUS__;
extern bool __LOG__;
extern bool __EST_CONNECT__;
extern char __STR_LOC_IP_ADDR[];

extern updater	__updater__;
extern u16		__pins_input_value;
extern u8		__pins_output_value;
extern u16		__pins_anl_value[4];
extern	bool	     __remote_dbg;
extern s32		__rem_socket_dbg_id;
extern s32		__rem_socket_th_id;
extern u8		__power_state;


#define OUTP(FORMAT,...)if(__DEBUG__) {\
    Ql_memset(DBG_BUFFER, 0, DBG_BUF_LEN);\
	Ql_sprintf(&DBG_BUFFER[0],FORMAT,##__VA_ARGS__);\
	if(__remote_dbg){ \
		Ql_SOC_Send(__rem_socket_dbg_id, DBG_BUFFER, Ql_strlen(DBG_BUFFER));} \
	else{ \
	 Ql_UART_Write(UART_PORT1, DBG_BUFFER, Ql_strlen(DBG_BUFFER));}}



#define OUTD(FORMAT,...)if(__DEBUG__) {\
    Ql_memset(DBG_BUFFER, 0, DBG_BUF_LEN);\
	Ql_GetLocalTime(&__tm);\
	Ql_sprintf(&DBG_BUFFER[0], " %c%.2d:%.2d:%.2d %s",8, __tm.hour, __tm.minute, __tm.second, "  ");\
	Ql_sprintf(&DBG_BUFFER[12],FORMAT,##__VA_ARGS__);\
	Ql_memcpy(&DBG_BUFFER[Ql_strlen(DBG_BUFFER)],"\r\n",2);\
	if(__remote_dbg){ \
		Ql_SOC_Send(__rem_socket_dbg_id, DBG_BUFFER, Ql_strlen(DBG_BUFFER));} \
	else{ \
	 Ql_UART_Write(UART_PORT1, DBG_BUFFER, Ql_strlen(DBG_BUFFER));}}


//	Ql_Debug_Trace(DBG_BUFFER,NULL);}}

#define OUTDS(FORMAT,...) {\
    Ql_memset(DBG_BUFFER, 0, DBG_BUF_LEN);\
	Ql_sprintf(&DBG_BUFFER[0],FORMAT,##__VA_ARGS__);\
	Ql_memcpy(&DBG_BUFFER[Ql_strlen(DBG_BUFFER)],"\r\n",2);\
	if(__remote_dbg){ \
		Ql_SOC_Send(__rem_socket_dbg_id, DBG_BUFFER, Ql_strlen(DBG_BUFFER));} \
	else{ \
	 Ql_UART_Write(UART_PORT1, DBG_BUFFER, Ql_strlen(DBG_BUFFER));}}	
//Ql_Debug_Trace(DBG_BUFFER,NULL);}}


#define OUTNET(x,...)\
    if(_n_current!=_n_state){\
     _n_current=_n_state;\
     OUTD((x),__VA_ARGS__);\
     /*if(__log){LOG((x), __VA_ARGS__);}*/}\

void parse_sim_state(u8 state, char* out_state);
bool parse_network_state(u8 state, char* out_state);
void parse_cfun_state(u8 state, char* out_state);
void parse_system_state(u8 state, char* out_state);
void est_tmr_connector(u32 timerId, void* param);
void est_wait_confirm(u32 timerId, void* param);


#endif
