#include <qlobal.h>

#define BUFFER_COMMAND_LEN 512
static char __buf_command[BUFFER_COMMAND_LEN];
static u16 __buf_command_pos=0;

void text_bool(bool value, char* outtext) {
	Ql_memset(outtext, 0, 5);
	if (value)
		Ql_memcpy(outtext, "ON", 2);
	else
		Ql_memcpy(outtext, "OFF", 3);
}

void set_state_est(STATE_TCP state) {
	switch (state)
	{
	case STATE_EST_CONNECT:
		if(!__EST_CONNECT__)
			_n_state = STATE_EST_CONNECT;
		break;
	case STATE_EST_STOP:
		Ql_SOC_Close(__socket_est_id);
		Ql_Timer_Stop(EST_CONFIRM_TIMER);
		Ql_Timer_Stop(EST_EMPTY_TIMER);
		__EST_CONNECT__ = FALSE;
		_n_state = STATE_NONE;
		break;
	default:
		break;
	}
}

void set_config_form(char* conf) {

	char* pchar;
	char ips[16] = "\0";
	if (conf != NULL) {
		if (Ql_strstr(conf, "CONFIG:"))
			conf += 7;
	
		pchar = strtok(conf, " ");
		
		while (pchar != NULL) {
			if (Ql_strstr(pchar, "DT")) {
				pchar += 3;
				if (Ql_isdigit(*pchar) != 0) {
					ST_Time tm;
					Ql_GetLocalTime(&tm);
					rtc_calendar_register_value_to_time(Ql_atoi(pchar), &tm);
					Ql_SetLocalTime(&tm);
				}
			}
			else if (Ql_strstr(pchar, "APN")) {
				pchar += 4;
				Ql_memset(&__mdm_settings.apn, 0, 30);
				Ql_memcpy(&__mdm_settings.apn, pchar, 30);
			}
			else if (Ql_strstr(pchar, "USER")) {
				pchar += 5;
				Ql_memset(&__mdm_settings.apn_user, 0, 30);
				Ql_memcpy(&__mdm_settings.apn_user, pchar, 30);
			}
			else if (Ql_strstr(pchar, "PASS")) {
				pchar += 5;
				Ql_memset(&__mdm_settings.apn_password, 0, 30);
				Ql_memcpy(&__mdm_settings.apn_password, pchar, 30);
			}
			else if (Ql_strstr(pchar, "SCHED")) {
				pchar += 6;
				Ql_memset(&__mdm_settings.schedulers, 0, 300);
				Ql_memcpy(&__mdm_settings.schedulers, pchar, Ql_strlen(pchar));
				write_mdm_settings();
				u8 dat[300];
				Ql_memset(dat, 0, 300);
				u16 len_schd = 0;
				dat[0] = 8;
				len_schd = decode_b64_bin((u8*)pchar, Ql_strlen(pchar), &dat[1]);
				mdm_msg_send(dat, len_schd +1);
			}
			else if (Ql_strstr(pchar, "DEBOUNCE")) {
				pchar += 9;
				if (Ql_isdigit(*pchar) != 0)
				{
					u16 val = (u16)Ql_atoi(pchar);
					__mdm_settings.deboung = val;
				}
			}
			else if (Ql_strstr(pchar, "RAS")) {
				pchar += 4;
				if (Ql_isdigit(*pchar) != 0) {
					u8 val = (u8)Ql_atoi(pchar);
					__mdm_settings.use_sched = val == 1 ? TRUE : FALSE;
				}
			}
			else if (Ql_strstr(pchar, "DEBUG")) {
				pchar += 6;
				if (Ql_isdigit(*pchar) != 0) {
					u8 val = (u8)Ql_atoi(pchar);
					__mdm_settings.debug = val == 1 ? TRUE : FALSE;
				}
			}
			else if (Ql_strstr(pchar, "EST")) {
				pchar += 4;
				if (Ql_isdigit(*pchar) != 0) {
					u8 val = (u8)Ql_atoi(pchar);
					__mdm_settings.use_esttools = val == 1 ? TRUE : FALSE;
					if(__mdm_settings.use_esttools==TRUE && __EST_CONNECT__!=TRUE)
						_n_state = STATE_EST_CONNECT;
				}
			}
			else if (Ql_strstr(pchar, "TCP")) {
				pchar += 4;
				if (Ql_isdigit(*pchar) != 0)
				{
					u16 val = (u16)Ql_atoi(pchar);
					__mdm_settings.tcp = val;
				}
			}
			else if (Ql_strstr(pchar, "TSEND")) {
				pchar += 6;
				if (Ql_isdigit(*pchar) != 0) {
					u8 val = (u8)Ql_atoi(pchar);
					__mdm_settings.t_send = val;
				}
			}
			else if (Ql_strstr(pchar, "NUM")) {
				pchar += 4;
				if (Ql_isdigit(*pchar) != 0) {
					u8 val = (u8)Ql_atoi(pchar);
					__mdm_settings.num = val;
				}
			}
			else if (Ql_strstr(pchar, "AIN")) {
				pchar += 4;
				if (Ql_isdigit(*pchar) != 0) {
					u8 val = (u8)Ql_atoi(pchar);
					__mdm_settings.ainn_active = val;
				}
			}
			else if (Ql_strstr(pchar, "DIN")) {
				pchar += 4;
				if (Ql_isdigit(*pchar) != 0) {
					u16 val = (u16)Ql_atoi(pchar);
					__mdm_settings.dinn_active = val;
				}
			}
			else if (Ql_strstr(pchar, "DOUT")) {
				pchar += 5;
				if (Ql_isdigit(*pchar) != 0) {
					u8 val = (u8)Ql_atoi(pchar);
						__mdm_settings.dout_active = val;
				}
			}
			else if (Ql_strstr(pchar, "DBZ")) {
				pchar += 4;
				if (Ql_isdigit(*pchar) != 0) {
					u8 val = (u8)Ql_atoi(pchar);
					__mdm_settings.deadband = val;
				}
			}
			else if (Ql_strstr(pchar, "DOOR")) {
				pchar += 5;
				if (Ql_isdigit(*pchar) != 0)
				{
					u16 val = (u16)Ql_atoi(pchar);
					__mdm_settings.act_door_chanals = ~val;
				}
			}
			else if (Ql_strstr(pchar, "TZ")) {
				pchar += 3;
				if (Ql_isdigit(*pchar) != 0) {
					u8 val = (u8)Ql_atoi(pchar);
					__mdm_settings.timezone = val;
				}
			}
			else if (Ql_strstr(pchar, "LATIT")) {
				pchar += 6;
				if (Ql_isdigit(*pchar) != 0) {
					float val = (float)Ql_atof(pchar);
					__mdm_settings.latidute = val;
				}
					
			}
			else if (Ql_strstr(pchar, "LONGIT")) {
				pchar += 7;
				if (Ql_isdigit(*pchar) != 0) {
					float val = (float)Ql_atof(pchar);
					__mdm_settings.longitude = val;
				}
			}
			else if (Ql_strstr(pchar, "IP")) {
				pchar += 3;
				Ql_memset(ips, 0, 16);
				Ql_memcpy(ips, pchar, Ql_strlen(pchar));
			}
			else if (Ql_strstr(pchar, "SERIAL")) {
				pchar += 7;
				Ql_memset(&__mdm_settings.sport, 0, 13);
				Ql_memcpy(&__mdm_settings.sport, pchar, 13);
			}

			else if (Ql_strstr(pchar, "REBOOT")) {
				char reboot = 255;
				mdm_msg_send(&reboot, 1);
				OUTD("Recive command for rebbot");
				Ql_Sleep(2000);
				Ql_Reset(0);
			}
			pchar = strtok(NULL, " ");
		}
		if (Ql_strlen(ips)>0) {
			u8 ind = 0;
			pchar = strtok(ips, ".");
			while (pchar != NULL)
			{
				if (Ql_isdigit(*pchar) != 0) {
					u8 val = (u8)Ql_atoi(pchar);
					__mdm_settings.ip[ind] = val;
				}
				else
					break;
				pchar = strtok(NULL, ".");
				ind++;
			}
		}
		save_all_settings();
	}
}


void get_config_form() {
	char __locbuf[1024];
	ST_Time loct;
	read_mdm_settings();
	Ql_GetLocalTime(&loct);
	s32 time = rtc_calendar_time_to_register_value(&loct);
	loct.timezone = __mdm_settings.timezone;
	s32 simcard;
	s32 creg;
	s32 cgreg;
	u32 rssi;
	u32 ber;
	u32 osunrise;
	u32 osunset;
	sun_rise_set(&loct, __mdm_settings.latidute, __mdm_settings.longitude, &osunrise, &osunset);
	RIL_SIM_GetSimState(&simcard);
	RIL_NW_GetGSMState(&creg);
	RIL_NW_GetGPRSState(&cgreg);
	RIL_NW_GetSignalQuality(&rssi, &ber);
	char ipa[16];
	u16 cdir = ~__pins_input_value;
	Ql_memset(ipa, 0, 16);
	Ql_memset(__locbuf, 0, 1024);
	Ql_sprintf(ipa, "%d.%d.%d.%d", __mdm_settings.ip[0], __mdm_settings.ip[1], __mdm_settings.ip[2], __mdm_settings.ip[3]);
	//Ql_memset(buffer, 0, size);
	Ql_sprintf(__locbuf,"CONFIG:APN:%s USER:%s PASS:%s DT:%d DEBOUNCE:%d DEBUG:%d EST:%d "
		"IP:%s TCP:%d TSEND:%d AIN:%d DIN:%d DOUT:%d DOOR:%d LATIT:%.3f LONGIT:%.3f TZ:%d "
		"CDIN:%d CDOUT:%d CAIN:%d;%d;%d;%d SRISE:%d SSET:%d SIM:%d GSM:%d GPRS:%d SIGNAL:%d DBZ:%d "
		"IPOWN:%s SCHED:%s RAS:%d VER:%s SERIAL:%s NUM:%d\r\n",
		__mdm_settings.apn, __mdm_settings.apn_user, __mdm_settings.apn_password, time, __mdm_settings.deboung, __mdm_settings.debug,__mdm_settings.use_esttools,
		ipa, __mdm_settings.tcp, __mdm_settings.t_send, __mdm_settings.ainn_active, __mdm_settings.dinn_active,
		__mdm_settings.dout_active, (u16)~__mdm_settings.act_door_chanals, __mdm_settings.latidute, __mdm_settings.longitude, __mdm_settings.timezone,
		cdir,__pins_output_value,__pins_anl_value[0], __pins_anl_value[1], __pins_anl_value[2], __pins_anl_value[3],
		osunset,osunrise,simcard,creg,cgreg,rssi,__mdm_settings.deadband, __STR_LOC_IP_ADDR,__mdm_settings.schedulers,__mdm_settings.use_sched,
		__VERSION__,__mdm_settings.sport,__mdm_settings.num);
	//Ql_UART_Write(UART_PORT1, __locbuf, Ql_strlen(__locbuf));
	OUTDS(__locbuf);
}

void print_settings() {
	ST_Time loct;
	read_mdm_settings();
	Ql_GetLocalTime(&loct);
	loct.timezone = __mdm_settings.timezone;
	s32 simcard;
	s32 creg;
	s32 cgreg;
	u32 rssi;
	u32 ber;
	u8 in[17];
	u8 out[9];
	u32 osunrise;
	u32 osunset;
	u8 ss[30];
	u8 sr[30];
	sun_rise_set(&loct, __mdm_settings.latidute, __mdm_settings.longitude, &osunrise, &osunset);
	time_to_text(&osunrise, sr,TRUE);
	time_to_text(&osunset, ss,TRUE);
	data_to_state(in, __pins_input_value, 16);
	data_to_state(out, __pins_output_value, 8);
	RIL_SIM_GetSimState(&simcard);
	RIL_NW_GetGSMState(&creg);
	RIL_NW_GetGPRSState(&cgreg);
	RIL_NW_GetSignalQuality(&rssi,&ber);
	u8 btext[5];
	OUTDS("\r\nAPN\t\t\t%s\r\nIP\t\t\t%d.%d.%d.%d\r\nTCP\t\t\t%d\r\nUSER\t\t\t%s\r\n"
		"PASS\t\t\t%s\r\nDEBUG\t\t\t%s\r\nLOG\t\t\t%s\r\nNUM\t\t\t%d\r\nTSEND\t\t\t%d\r\n"
		"DATE\t\t\t%.2d.%.2d.%.2d\r\nTIME\t\t\t%.2d:%.2d:%.2d\r\nDEBOUNCE\t\t%d\r\n"
		"IPLocal\t\t\t%s\r\nWUSER\t\t\t%s\r\nWPASS\t\t\t%s\r\nLATITUDE\t\t%f\r\nLONGITUDE\t\t%f\r\n"
		"SUNRISE\t\t\t%s\r\nSUNSET\t\t\t%s\r\n"
		"\r\n *****  SIM:%d GSM:%d GPRS:%d SIGNAL:%ddBm DI:%s DO:%s  ******\r\n\r\n",
		__mdm_settings.apn, __mdm_settings.ip[0], __mdm_settings.ip[1], __mdm_settings.ip[2], __mdm_settings.ip[3],
		__mdm_settings.tcp, __mdm_settings.apn_user, __mdm_settings.apn_password, __mdm_settings.debug==TRUE ? "ON":"OFF", __mdm_settings.log,
		__mdm_settings.num, __mdm_settings.t_send,
		loct.day, loct.month, loct.year, loct.hour, loct.minute, loct.second, __mdm_settings.deboung,
		__STR_LOC_IP_ADDR, __mdm_settings.w_user, __mdm_settings.w_password,__mdm_settings.latidute,__mdm_settings.longitude,
		ss,sr,
		simcard, creg, cgreg, rssi,in,out
	);
}

void  data_to_state(u8* buffer, u32 value, u8 count) {
	for (u8 i = 0; i<count; i++) {
		if (value & (1 << i)) {
			buffer[i] = '1';
		}
		else buffer[i] = '0';
	}
	buffer[count] = '\0';
}

void  data_to_state_invert(u8* buffer, u32 value, u8 count) {
	for (u8 i = 0; i<count; i++) {
		if (value & (1 << i)) {
			buffer[i] = '0';
		}
		else buffer[i] = '1';
	}
	buffer[count] = '\0';
}


void set_command(char* command) {
	char* cmd = NULL;
	char idx = 0;
	char* pch = NULL;

	cmd = Ql_strstr(command, "RAS=");
	if (cmd) {
		cmd += 4;
		if (Ql_strstr(cmd, "OFF")) {
			__mdm_settings.use_sched = FALSE;
		}
		else {
			__mdm_settings.use_sched = TRUE;
		}
	}
	cmd = Ql_strstr(command, "DEBUG=");
	if (cmd) {
		cmd += 6;
		if (Ql_atoi(cmd) == 0)
			__mdm_settings.debug = FALSE;
		else
			__mdm_settings.debug = TRUE;
		/*Ql_memset(__mdm_settings.debug, 0, 5);
		if (Ql_strstr(cmd, "TRUE")) {
			__DEBUG__ = TRUE;
			Ql_strcpy(&__mdm_settings.debug[0], "TRUE");
		}
		else {
			__DEBUG__ = FALSE;
			Ql_strcpy(&__mdm_settings.debug[0], "FALSE");
		}*/
		
		//OUTD("Set tcp:%s", __mdm_settings.debug);
	}
	cmd = Ql_strstr(command, "LOG=");
	if (cmd) {
		cmd += 4;
		if (Ql_strstr(cmd, "TRUE")) {
			__LOG__ = TRUE;
		}
		else {
			__LOG__ = FALSE;
		}
		Ql_strcpy(&__mdm_settings.log[0], cmd);
	}
	cmd = Ql_strstr(command, "TCP=");
	if (cmd) {
		cmd += 4;
		__mdm_settings.tcp = Ql_atoi(cmd);
		//OUTD("Set tcp:%d", __mdm_settings.tcp);
	}
	cmd = Ql_strstr(command, "IP=");
	if (cmd) {
		cmd += 3;

		pch = strtok(cmd, ".");
		while (pch != NULL)
		{
			__mdm_settings.ip[idx] = Ql_atoi(pch);
			pch = strtok(NULL, ".");
			idx++;
		}
		//OUTD("Set IP:%d.%d.%d.%d\r\n", __mdm_settings.ip[0], __mdm_settings.ip[1], __mdm_settings.ip[2], __mdm_settings.ip[3]);

	}
	cmd = Ql_strstr(command, "APN=");
	if (cmd) {
		cmd += 4;
		Ql_strcpy(&__mdm_settings.apn[0], cmd);
		
	}
	cmd = Ql_strstr(command, "PASS=");
	if (cmd) {
		cmd += 5;
		Ql_strcpy(&__mdm_settings.apn_password[0], cmd);

	}
	cmd = Ql_strstr(command, "NUM=");
	if (cmd) {
		cmd += 4;
		__mdm_settings.num = Ql_atoi(cmd);
	}
	/*cmd = Ql_strstr(command, "PWD=");
	if (cmd){
	cmd += 4;
	Ql_strcpy(&__settings.Pwd[0],cmd);

	}*/
	cmd = Ql_strstr(command, "USER=");
	if (cmd) {
		cmd += 5;
		Ql_strcpy(&__mdm_settings.apn_user[0], cmd);

	}
	cmd = Ql_strstr(command, "TIME=");
	if (cmd) {
		char* last;
		ST_Time tm;
		Ql_GetLocalTime(&tm);
		cmd += 5;
		if (Ql_strlen(cmd) == 8) {
			tm.hour = Ql_atoi(strtok_r(cmd, ":", &last));
			tm.minute = Ql_atoi(strtok_r(last, ":", &last));
			tm.second = Ql_atoi(strtok_r(last, ":", &last));
			Ql_SetLocalTime(&tm);
		}
	}
	cmd = Ql_strstr(command, "DATE=");
	if (cmd) {
		char* last;
		ST_Time tm;
		Ql_GetLocalTime(&tm);
		cmd += 5;
		if (Ql_strlen(cmd) == 10) {
			tm.day = Ql_atoi(strtok_r(cmd, ".", &last));
			tm.month = Ql_atoi(strtok_r(last, ".", &last));
			tm.year = Ql_atoi(strtok_r(last, ".", &last));
			Ql_SetLocalTime(&tm);
		}
	}
	cmd = Ql_strstr(command, "REBOOT");
	if (cmd) {
		OUTD("!-----Get command to reboot..-----", NULL);
		Ql_Sleep(100);
		Ql_Reset(0);
	}
	cmd = Ql_strstr(command, "DELETE");
	if (cmd) {
		delete_mdm_settings();
		Ql_Reset(0);
	}
	cmd = Ql_strstr(command, "TSEND=");
	if (cmd) {
		s32 tsend;
		cmd += 6;
		tsend = Ql_atoi(cmd);
		if (tsend > 10 && tsend < 60) {
			__mdm_settings.t_send = tsend;
		}
		//OUTD("Set tcp:%d", __mdm_settings.t_send);
	}

	cmd = Ql_strstr(command, "DEBOUNCE=");
	if (cmd) {
		s32 deboung;
		cmd += 9;
		deboung = Ql_atoi(cmd);
		//OUTD("set debounce ok:%d",deboung);
		if (deboung >= 100 && deboung <= 1000) {
			//OUTD("set debounce ok:%d",deboung);
			__mdm_settings.deboung = deboung;
		}
		//OUTD("Set tcp:%d", __mdm_settings.deboung);
	}
	cmd = Ql_strstr(command, "WUSER=");
	if (cmd) {
		cmd += 6;
		Ql_strcpy(&__mdm_settings.w_user[0], cmd);
	}
	cmd = Ql_strstr(command, "WPASS=");
	if (cmd) {
		cmd += 6;
		Ql_strcpy(&__mdm_settings.w_password[0], cmd);
	}
	cmd = Ql_strstr(command, "?");
	if (cmd) {
		//print_settings();
		if (!__MENUS__) {
			__DEBUG__ = FALSE;
			__MENUS__ = TRUE;
			char q = "?";
			menu("?");
		}
		//get_menu_console();
	}
	
}

u32 is_valid_ip(const u8 *ip_str,u8 ip[4])
{
	u32 n1, n2, n3, n4;
	
		if (Ql_sscanf(ip_str, "%u.%u.%u.%u", &n1, &n2, &n3, &n4) != 4) return 0;
		if ((n1 != 0) && (n1 <= 255) && (n2 <= 255) && (n3 <= 255) && (n4 <= 255)) {
			
			ip[0] = n1;
			ip[1] = n2;
			ip[2] = n3;
			ip[3] = n4;
			return 1;
		}
	
	return 0;
}

u32 is_valid_datetime(const u8 *ip_str, u32 ip[6])
{
	u32 n1, n2, n3, n4,n5,n6;
	if (Ql_sscanf(ip_str, "%u.%u.%u %u:%u:%u", &n1, &n2, &n3, &n4,&n5,&n6) != 6) return 0;
	if ((n3>=0)&&(n2 <= 12) & (n2 > 0) && (n1 >= 0) && (n1 <= 31) && (n4 >=0) && (n4 <=23) && (n5 >=0) && (n5 <=59) && (n6 >=0) && (n6 <= 59)) {

		ip[0] = n1;
		ip[1] = n2;
		ip[2] = n3;
		ip[3] = n4;
		ip[4] = n5;
		ip[5] = n6;
		return 1;
	}

	return 0;
}


void get_menu_console(){
	menu(&__buf_command[0]);
	__buf_command_pos = 0;
	Ql_memset(__buf_command, 0, BUFFER_COMMAND_LEN);
}

bool check_pwd(){
	
	char password[100];
	u8 index = 0;
	u8 cpass[30];
	char* tt = Ql_strstr(__buf_command, "PWD");
	if (tt != NULL) {
		tt += 4;
		while (*tt != ' ' || *tt == 0)
		{
			if (*tt == ' ')
				break;
			password[index] = *tt++;
			index++;
		}
		decode_b64(&password[0], &cpass[0]);
		if (Ql_strstr(cpass, &__mdm_settings.w_password)) {
			return TRUE;
		}
		else {
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
}

void set_new_pwd() {
	char password[100];
	u8 index = 0;
	char cpass[30];

	char* tt = Ql_strstr(__buf_command, "NEW");
	if (tt != NULL) {
		tt += 4;
		u8 len = Ql_strlen(tt);
		for (u8 i = 0; i < len; i++)
		{
			password[i] = *tt++;
		}
		Ql_memset(&cpass[0], 0, 30);
		s32 le= decode_b64_bin((u8*)&password[0],len,(u8*)&cpass[0]);
		Ql_memset(&__mdm_settings.w_password[0], 0, 30);
		Ql_memcpy(&__mdm_settings.w_password, &cpass[0], le);
		write_mdm_settings();
	}
}

void parce_command(void) {
	char* pch = NULL;
	char* last = NULL;
	__buf_command[__buf_command_pos] = '\0';
	remove_char(__buf_command, '\r');

	if (Ql_strstr(__buf_command, "CONFIG?")) {
		get_config_form();
	}
	else if (Ql_strstr(__buf_command, "SPASS")) {
		if (check_pwd()) {
			set_new_pwd();
			OUTDS("PWD:OK");
		}
		else {
			OUTDS("PWD:ERROR");
		}
	}
	else if (Ql_strstr(__buf_command, "REBOOT"))
	{
		if (check_pwd()) {
			OUTDS("PWD:OK");
			Ql_Timer_Start(SYSTEM_REBOOT,5000,FALSE);
		}
		else
			OUTDS("PWD:ERROR");
	}
	else if (Ql_strstr(__buf_command, "CONFIG:"))
	{
		if (check_pwd()) {
			set_config_form(__buf_command);
			OUTDS("PWD:OK");
		}
		else
			OUTDS("PWD:ERROR");
	}
	else if (Ql_strstr(__buf_command, "LIGHTS:"))
	{
		if (check_pwd()) {
			char* pp = Ql_strstr(__buf_command, "LIGHTS:");
			pp += 7;
			if (Ql_isdigit(*pp) != 0) {
				u8 val = (u8)Ql_atoi(pp);
				u8 d[2] = { 5 };
				if (val == 0) {
					d[1] = 0;
				}
				else {
					d[1] = 1;
				}
				__mdm_settings.use_sched = FALSE;
				save_all_settings();
				mdm_msg_send(&d[0], 2);
			}
		}
		else
		{
			OUTDS("PWD:ERROR");
		}
	}
	else if (Ql_strstr(__buf_command, "MENU$$$")){
		if (!__MENUS__) {
			__DEBUG__ = FALSE;
			__MENUS__ = TRUE;
			char q = "?";
			menu("?");		
		}
		//pch = strtok_r(__buf_command, " ", &last);
		//while (pch != NULL) {
		//	//set_command(pch);

		//	pch = strtok_r(NULL, " ", &last);
		//}
	}
	__buf_command_pos = 0;
	Ql_memset(__buf_command, 0, BUFFER_COMMAND_LEN);
	
		
}

bool backspace_buffer() {
	if (__buf_command_pos != 0) {
		__buf_command[__buf_command_pos] = '\0';
		__buf_command_pos -= 1;
		return TRUE;
	}
	else
		return FALSE;
}

void fill_command_buffer(char* ctxt) {
	
	if ((__buf_command_pos + Ql_strlen(ctxt))>BUFFER_COMMAND_LEN)
		__buf_command_pos = 0;
	//Ql_sprintf(&__buf_command[__buf_command_pos], "%s", ctxt);
	Ql_memcpy(&__buf_command[__buf_command_pos], ctxt, Ql_strlen(ctxt));
	__buf_command_pos += Ql_strlen(ctxt);
	
}

void remove_char(char *str, char garbage) {

	char *src, *dst;
	for (src = dst = str; *src != '\0'; src++) {
		*dst = *src;
		if (*dst != garbage) dst++;
	}
	*dst = '\0';
}

char* strtok_r(char *s, const char *delim, char **last)
{
	char *spanp;
	int c, sc;
	char *tok;

	if (s == NULL && (s = *last) == NULL)
	{
		return NULL;
	}
cont:
	c = *s++;
	for (spanp = (char *)delim; (sc = *spanp++) != 0;)
	{
		if (c == sc)
		{
			goto cont;
		}
	}

	if (c == 0)
	{
		*last = NULL;
		return NULL;
	}
	tok = s - 1;
	for (;;)
	{
		c = *s++;
		spanp = (char *)delim;
		do
		{
			if ((sc = *spanp++) == c)
			{
				if (c == 0)
				{
					s = NULL;
				}
				else
				{
					char *w = s - 1;
					*w = '\0';
				}
				*last = s;
				return tok;
			}
		} while (sc != 0);
	}
}

char * strtok(char *s, const char *delim)
{
	static char *last;

	return strtok_r(s, delim, &last);
}

s32 Ql_abs(s32 a)
{
	if (a < 0)
		return -a;
	return a;
}
