#ifndef __MENU__
#define __MENU__

#include "qlobal.h"

#define MENU(FORMAT,...) {\
    Ql_memset(DBG_BUFFER, 0, DBG_BUF_LEN);\
	Ql_sprintf(&DBG_BUFFER[0],FORMAT,##__VA_ARGS__);\
	if(__remote_dbg){ \
		Ql_SOC_Send(__rem_socket_dbg_id, DBG_BUFFER, Ql_strlen(DBG_BUFFER));} \
	else{ \
	Ql_UART_Write(UART_PORT1, DBG_BUFFER, Ql_strlen(DBG_BUFFER));}}

extern char choice;
extern char choice1;
extern char insert[];

void menu(char* inp);
void DisplayMainMenu(void);

#endif // !__MENU__

