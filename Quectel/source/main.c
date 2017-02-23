#include <qlobal.h>




#define TIME_GPRS_REGISTER		120000 //таймаут регистрации в gprs сети(нет регистр. перегруз.)
#define TIME_GPRS_ACTIVATE		50	   //цикл работы
#define SAM_IN_BUF_LEN			256		//приемный буфер от SAM
#define SAM_TIMEOUT_SYNC		30000



static void callback_debug_port(Enum_SerialPort port, Enum_UARTEventType msg, bool level, void* customizedPara);
static void callback_samd20_port(Enum_SerialPort port, Enum_UARTEventType msg, bool level, void* customizedPara);
static void tmr_register_gprs(u32 timerId, void* param);
static void tmr_work_cycle(u32 timerId, void* param);
static void tmr_sam_cync(u32 timerId, void* param);
static void system_reboot(u32 timerId, void* param);
static s32 get_time_synx_cmd(char* line, u32 len, void* userdata);

bool	__DEBUG__ = FALSE;
bool	__MENUS__ = FALSE;
bool	__LOG__	  = FALSE;
bool	__REBOOT_ = FALSE;
bool	__EST_CONNECT__ = FALSE;
u8		__sync_msg_count = 0;


void accept_settings() {

	if (__mdm_settings.debug){//Ql_strstr(__mdm_settings.debug, "TRUE")) {
		__DEBUG__ = TRUE;
	}
	else {
		__DEBUG__ = FALSE;
	}
}

void proc_main_task(s32 taskId)
{
	char state[30];
	Ql_memset(state, 0, 30);
	Ql_GetCoreVer((u8*)state, 30);
	
	Ql_UART_Register(UART_PORT1, &callback_debug_port, NULL);
	Ql_UART_Register(UART_PORT3, &callback_samd20_port, NULL);
	Ql_Timer_Register(GPRS_REGISTER_TIMER, tmr_register_gprs, NULL);
	Ql_Timer_Register(GPRS_ACTIVATE_TIMER, tmr_work_cycle, NULL);
	Ql_Timer_Register(EST_CONNECTOR_TIMER, est_socket_connect, NULL);
	Ql_Timer_Register(EST_CONFIRM_TIMER, est_wait_confirm, NULL);
	Ql_Timer_Register(EST_EMPTY_TIMER, est_empty_msg, NULL);
	Ql_Timer_Register(SYNC_SAM_MSG, tmr_sam_cync, NULL);
	Ql_Timer_Register(SYSTEM_REBOOT, system_reboot, NULL);
	Ql_UART_Open(UART_PORT1, 115200, FC_NONE);
	Ql_UART_Open(UART_PORT3, 115200, FC_NONE);
	OUTD("Core version:%s", state);
	read_mdm_settings();
	accept_settings();
	check_event_file();
	//
	//delete_mdm_settings();
	//
	//Ql_WTD_Init(0, PINNAME_NETLIGHT, 600);
	//Ql_WTD_Start(3000);
	client_socket_id_init();
	Ql_Timer_Start(GPRS_REGISTER_TIMER, TIME_GPRS_REGISTER, FALSE);
	ST_MSG msg;
	while (1)
	{
		Ql_OS_GetMessage(&msg);
		switch (msg.message)
		{
		case MSG_ID_RIL_READY: {
			OUTD("RIL is ready");
			Ql_RIL_Initialize();
			break;
		}
		case MSG_ID_URC_INDICATION:
		{
			Ql_memset(state, 0, 30);
			switch (msg.param1)
			{
			case URC_SYS_INIT_STATE_IND:
				parse_system_state(msg.param2, state);
				OUTD(">Sys Init Status:%s", state);
				break;
			case URC_SIM_CARD_STATE_IND:
				parse_sim_state(msg.param2, state);
				OUTD(">SIM Card Status:%s", state);
				break;
			case URC_GSM_NW_STATE_IND:
				parse_network_state(msg.param2, state);
				OUTD(">GSM Network Status:%s", state);
				break;
			case URC_GPRS_NW_STATE_IND:
				if (parse_network_state(msg.param2, state)) {
					//Ql_RIL_SendATCmd("AT+CCLK?", 8, time_handler_callback, NULL, 0);
					
					//Ql_RIL_SendATCmd("AT+CTZU=3", 9, NULL, NULL, 0);
					Ql_RIL_SendATCmd("AT+CTZU?", 8, get_time_synx_cmd, NULL, 0);
					Ql_Timer_Stop(GPRS_REGISTER_TIMER);
					//общий цикл
					Ql_Timer_Start(GPRS_ACTIVATE_TIMER,TIME_GPRS_ACTIVATE,TRUE);
					u8 d[1] = { 1 };
					mdm_msg_send(&d[0], 1);
					Ql_Timer_Start(SYNC_SAM_MSG, SAM_TIMEOUT_SYNC, TRUE);
					//__DEBUG__ = FALSE;
					//DisplayMainMenu();
					break;
				}
				OUTD(">GPRS Network Status:%s", state);
				break;
			case URC_CFUN_STATE_IND:
				parse_cfun_state(msg.param2, state);
				OUTD(">CFUN Status:%s", state);
				break;
			}
		default:
			break;
		}
		}
		//Ql_WTD_Feed(__wtdid);
	}
}

static void system_reboot(u32 timerId, void* param) {
	Ql_Reset(0);
}

bool __run = FALSE;
bool __need_reboot = TRUE;
static s32 get_time_synx_cmd(char* line, u32 len, void* userdata) {	
	s32 result=-1;
	char* pHead = Ql_RIL_FindString(line, len, "+CTZU:");
	if (pHead!=NULL) {
		Ql_sscanf(pHead, "%*[^:]: %d", &result);
		if (result == 0) {
			OUTD("Set auto sync GPRS modem: CTZU=%d", result);
			__run = TRUE;
		}
		else {
			OUTD("Time sync modem: CTZU=%d", result);
			__run = TRUE;
			__need_reboot = FALSE;
		}
	}
	else{
		char* peror = Ql_RIL_FindString(line, len, "+CME ERROR:");
		if (peror!=NULL) {
			Ql_sscanf(peror, "%*[^:]: %d[\r\n]", &result);
			OUTD("Time sync modem error: CME ERROR:%d", result);
			__run = TRUE;
		}
	}
}

static void tmr_sam_cync(u32 timerId, void* param) {
	if (__sync_msg_count > 3) {
		OUTD("!!!Timeout sync SAM. System reboot!!!", NULL);
		Ql_Sleep(100);
		Ql_Reset(0);
		__sync_msg_count = 0;
	}
	else {
		OUTD(">SYNC SAM");
		u8 d[1] = { 1 };
		mdm_msg_send(&d[0], 1);
		__sync_msg_count += 1;
	}
}


//void est_tmr_connector(u32 timerId, void* param) {
//	
//		_n_state = STATE_EST_CONNECT;
//		Ql_Timer_Stop(EST_CONNECTOR_TIMER);
//}


static void tmr_work_cycle(u32 timerId, void* param) {
	if (__run) {
		if (__need_reboot) {
			Ql_RIL_SendATCmd("AT+CTZU=3", 9, NULL, NULL, 0);
			Ql_Sleep(100);
			Ql_Reset(0);
		}
	}
	if (__count_r_buf>0) {
		check();
	}
	tcp_main_function();
}


static void tmr_register_gprs(u32 timerId, void* param)
{
		OUTD("!!!Timeout registration GPRS.System reboot!!!", NULL);
		Ql_Sleep(100);
		Ql_Reset(0);
}


void data_to_hex(u8* data, u8 count) {
	u8 cdata[256];
	Ql_memset(cdata, 0, 256);
	u8* pdata = &cdata[0];
	for (u16 i = 0; i < count; i++) {
		Ql_sprintf(pdata, "0x%02x,", *data++);
		pdata += 5;
	}
	*--pdata = '\0';
	OUTDS("%s", &cdata[0]);
}


static void checkrun(u32 itime,u32 interval,void(*callback)(u32* ltime)) {
	u32 tm_rem;
	ST_Time t1;
	Ql_GetLocalTime(&t1);
	u32 tm_loc = rtc_calendar_time_to_register_value(&t1);
	u32 res = tm_loc - itime;
	if (res < 0) res *= -1;
	if(res>interval)
		func(tm_loc);
}

static void callback_samd20_port(Enum_SerialPort port, Enum_UARTEventType msg, bool level, void* customizedPara) {
	switch (msg)
	{
	case EVENT_UART_READY_TO_READ:
	{
		char in_data[SAM_IN_BUF_LEN];
		char out_data[SAM_IN_BUF_LEN];
		char* command;
		char* pend;
		Ql_memset(in_data, 0, SAM_IN_BUF_LEN);
		Ql_memset(out_data, 0, SAM_IN_BUF_LEN);
		s32 len = Ql_UART_Read(UART_PORT3, in_data, SAM_IN_BUF_LEN);
		//data_to_hex(in_data, len);
		//OUTD("%s len:%d", in_data, len);
		if (len > 0) {
			//OUTD("count in buffer:%d", __count_r_buf);
			add(in_data, len);
			check();
			//есть связь с SAM
			Ql_Timer_Stop(SYNC_SAM_MSG);
			//перезапуск ожидания сообщений от SAM  
			Ql_Timer_Start(SYNC_SAM_MSG, SAM_TIMEOUT_SYNC, TRUE);
			__sync_msg_count = 0;
		}
		if (Ql_strlen(out_data)>0)
			Ql_UART_Write(UART_PORT3, out_data, Ql_strlen(out_data));
		break;
	}
	case EVENT_UART_DTR_IND:// DTR level changed, developer can wake up the module in here
	{
		break;

	}
	case EVENT_UART_READY_TO_WRITE:
		OUTD("write data\r\n");
		break;
	default:
		break;
	}
}

static void callback_debug_port(Enum_SerialPort port, Enum_UARTEventType msg, bool level, void* customizedPara)
{
	switch (msg)
	{
	case EVENT_UART_READY_TO_READ:
	{
		char dat[1024];
		Ql_memset(dat, 0, 1024);
		s32 len = Ql_UART_Read(UART_PORT1, dat, 1024);
		if (len > 0) {
			u8* pData = dat;
			
			if (len > 2 &&  dat[len - 1] == 13) {
				fill_command_buffer(pData);
				__DEBUG__ = FALSE;
				parce_command();
				if (__mdm_settings.debug == TRUE)
					__DEBUG__ = TRUE;
			}
			else
			{
				if (*pData == 13 || *pData == 10) {
					if (__MENUS__==TRUE) {
						get_menu_console();
					}
					else {
						parce_command();
					}
				}
				else {
					/*if (*pData == 8) {
						if(backspace_buffer())
							Ql_UART_Write(UART_PORT1,(u8*)8, 1);
					}
					else*/
					fill_command_buffer(pData);
				}

			}
		}
		break;
	}
	case EVENT_UART_DTR_IND:// DTR level changed, developer can wake up the module in here
	{
		break;

	}
	case EVENT_UART_READY_TO_WRITE:
		break;
	default:
		break;
	}
}



