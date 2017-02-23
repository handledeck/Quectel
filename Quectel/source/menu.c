#include <menu.h>

char choice = 0;
char choice1 = 0;
char insert[50];

#define MEXIT MENU("? - Return to main menu\r\n")

static bool get_dbg_state() {
	if (__mdm_settings.debug){//Ql_strstr(__mdm_settings.debug, "TRUE")) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

void set_act_chnll(char* text, u16* channals,u8 count) {
	for (u8 i = 0; i < count; i++)
	{
		char a = text[i];
		if (atoi(&a) == 0) {
			*channals = (1 << i) | *channals;
		}
	}
}


void DisplayMainMenu()
{
	ST_Time loct;
	Ql_GetLocalTime(&loct);
	loct.timezone = __mdm_settings.timezone;
	s32 simcard;
	s32 creg;
	s32 cgreg;
	u32 rssi;
	u32 ber;
	u8 ind[17];
	u8 outd[9];
	u8 indoor[17];
	u8 in_actdin[17];
	u8 in_actain[7];
	u32 osunrise;
	u32 osunset;
	u8 ss[30];
	u8 sr[30];

	sun_rise_set(&loct, __mdm_settings.latidute, __mdm_settings.longitude, &osunrise, &osunset);
	time_to_text(&osunrise, sr,TRUE);
	time_to_text(&osunset, ss,TRUE);
	data_to_state(ind, __pins_input_value, 16);
	data_to_state(outd, __pins_output_value, 8);
	data_to_state_invert(indoor, __mdm_settings.act_door_chanals, 16);
	data_to_state(in_actdin, __mdm_settings.dinn_active, 16);
	data_to_state(in_actain, __mdm_settings.ainn_active, 4);
	RIL_SIM_GetSimState(&simcard);
	RIL_NW_GetGSMState(&creg);
	RIL_NW_GetGPRSState(&cgreg);
	RIL_NW_GetSignalQuality(&rssi, &ber);

	MENU("\r\n*****************************%s***********************\r\n", __STR_LOC_IP_ADDR);
	MENU("DIN:%s\r\nDOUT:%s\r\nAIN:%d %d %d %d\r\n", ind, outd, __pins_anl_value[0], __pins_anl_value[1], __pins_anl_value[2], __pins_anl_value[3]);
	MENU("SUNRISE:%s SUNSET:%s\r\n", ss, sr);
	MENU("SIM:%d GSM : %d GPRS : %d SIGNAL : %ddBm\r\n", simcard, creg, cgreg, rssi);
	MENU("*****************************************************************\r\n");
	MENU("Please make your selection\r\n");
	MENU("0 - EXIT FROM MENU\r\n");
	MENU("1 - APN (%s)\r\n", __mdm_settings.apn);
	MENU("2 - APN USER (%s)\r\n", __mdm_settings.apn_user);
	MENU("3 - APN PASSWORD (%s)\r\n", __mdm_settings.apn_password);
	MENU("4 - EST TCP (%d)\r\n", __mdm_settings.tcp);
	MENU("5 - EST IP (%d.%d.%d.%d)\r\n", __mdm_settings.ip[0], __mdm_settings.ip[1], __mdm_settings.ip[2], __mdm_settings.ip[3]);
	MENU("6 - EST STATE (%s)\r\n", __mdm_settings.use_esttools == TRUE ? "ON" : "OFF");
	MENU("7 - DEBUG (%s)\r\n", __mdm_settings.debug ==TRUE ? "ON":"OFF");// __mdm_settings.debug);
	MENU("8 - DATE (%.2d.%.2d.%.2d)\r\n", loct.day, loct.month, loct.year);
	MENU("9 - TIME (%.2d:%.2d:%.2d)\r\n", loct.hour, loct.minute, loct.second);
	MENU("10 - TSEND (%d)\r\n", __mdm_settings.t_send);
	MENU("11 - DEBOUNCE (%d)\r\n", __mdm_settings.deboung);
	MENU("12 - LIGHTS (%s)\r\n", (__pins_output_value & 0x3) == 0x3 ? "ON" : "OFF");
	MENU("13 - LATITUDE (%.3f)\r\n", __mdm_settings.latidute);
	MENU("14 - LONGITUDE (%.3f)\r\n", __mdm_settings.longitude);
	MENU("15 - SAVE SETTINGS\r\n");
	MENU("16 - REBOOT\r\n");
	MENU("17 - LIGHT DOORS CONTROL(%s)\r\n", indoor);
	MENU("18 - ACTIVE INPUT CHANNELS(%s)\r\n", in_actdin);
	MENU("19 - ACTIVE ANALOG CHANNELS(%s)\r\n", in_actain);
	MENU("20 - DEADBAND ANALOG CHANNELS(%d%%)\r\n", __mdm_settings.deadband);
	MENU("21 - USING SCHEDULERS(%s)\r\n", __mdm_settings.use_sched == TRUE ? "ON" : "OFF");
	MENU("Selection: ");
}



void apn() {
	if (choice == 0) {
		MENU("\r\nInsert apn name limit of 30 characters \r\n");
		MEXIT;
		MENU("APN: ");
		choice = 1;
	}
	else if (choice == 1 && (*insert == '?')) {
		DisplayMainMenu();
		choice = 0;
	}
	else {
		memset(&__mdm_settings.apn[0], 0, 30);
		memcpy(&__mdm_settings.apn[0], insert, strlen(insert));
		choice = 0;
		DisplayMainMenu();
	}
}

void ip() {
	if (choice == 0) {
		MENU("Insert ip addres connect to EST Tools. Format: 172.22.32.19\r\n");
		MEXIT;
		MENU("IP: ");
		choice = 5;
	}
	else if (choice == 5 && (*insert == '?')) {
		DisplayMainMenu();
		choice = 0;
	}
	else {
		char idx = 0;
		char* pch = strtok(insert, ".");
		while (pch != NULL)
		{
			__mdm_settings.ip[idx] = atoi(pch);
			pch = strtok(NULL, ".");
			idx++;
		}
		choice = 0;
		DisplayMainMenu();
	}
}

void tcp() {
	if (choice == 0) {
		MENU("\r\nInsert tcp port connect between from 1024 to 65535\r\n");
		MEXIT;
		MENU("TCP: ");
		choice = 4;
	}
	else if (choice == 4 && (*insert == '?')) {
		DisplayMainMenu();
		choice = 0;
	}
	else {
		__mdm_settings.tcp = atoi(insert);
		choice = 0;
		DisplayMainMenu();
	}
}

void apn_u_p(bool user) {
	if (choice == 0) {
		if (user) {
			MENU("\r\nInsert apn user limit of 30 characters\r\n");
			MEXIT
			MENU("APN USER: ");
			choice = 2;
		}
		else
		{
			MENU("Insert apn password limit of 30 characters \r\n");
			MEXIT;
			MENU("APN PASSWORD: ");
			choice = 3;
		}
	}
	else if ((choice == 2 || choice==3) && (*insert == '?')) {
		DisplayMainMenu();
		choice = 0;
	}
	else {
		if (user) {
			memset(&__mdm_settings.apn_user[0], 0, 30);
			memcpy(__mdm_settings.apn_user, insert, strlen(insert));
		}
		else {
			memset(&__mdm_settings.apn_password[0], 0, 30);
			memcpy(__mdm_settings.apn_password, insert, strlen(insert));
		}
		choice = 0;
		DisplayMainMenu();
	}
}

void Debug() {
	if (choice == 0) {
		MENU("0 - OFF 1 - ON\r\n");
		MEXIT;
		MENU("DEBUG: ");
		choice = 7;
	}
	else if (choice == 7 && (*insert == '?')) {
		DisplayMainMenu();
		choice = 0;
	}
	else {
		s32 dbg= atoi(insert);
		if (dbg < 0 && dbg>1) {
			MENU("\r\n!!!Error. 0 - OFF 1 - ON\r\n");
		}
		else
		{
			if (dbg == 0)
			{
				//__DEBUG__ = FALSE;
				//Ql_strcpy(&__mdm_settings.debug[0], "FALSE"); 
				__mdm_settings.debug = FALSE;
				MENU("\r\nDebuger is OFF\r\n");
			}
			else {
				//__DEBUG__ = TRUE;
				//Ql_strcpy(&__mdm_settings.debug[0], "TRUE");
				__mdm_settings.debug = TRUE;
				MENU("\r\nDebuger is ON\r\n");
			}
		}
		choice = 0;
	}
}

void use_scedulers() {
	if (choice == 0) {
		MENU("0 - OFF 1 - ON\r\n");
		MEXIT;
		MENU("SEDULERS: ");
		choice = 21;
	}
	else if (choice == 21 && (*insert == '?')) {
		DisplayMainMenu();
		choice = 0;
	}
	else {
		s32 dbg = atoi(insert);
		if (dbg < 0 && dbg>1) {
			MENU("\r\n!!!Error. 0 - OFF 1 - ON\r\n");
		}
		else
		{
			if (dbg == 0)
			{
				__mdm_settings.use_sched = FALSE;
				MENU("\r\nSchedulers is OFF\r\n");
			}
			else {
				__mdm_settings.use_sched = TRUE;
				MENU("\r\nSchedulers is ON\r\n");
			}
		}
		choice = 0;
	}
}

void date() {
	if (choice == 0) {
		MENU("Insert date.\r\nFormat: 31.12.2016\r\n");
		MEXIT;
		MENU("DATE: ");
		choice = 8;
	}
	else if (choice == 8 && (*insert == '?')) {
		DisplayMainMenu();
		choice = 0;
	}
	else {
		ST_Time tm;
		char* last;
		Ql_GetLocalTime(&tm);
		if (Ql_strlen(insert) == 10) {
			tm.day = Ql_atoi(strtok_r(insert, ".", &last));
			tm.month = Ql_atoi(strtok_r(last, ".", &last));
			tm.year = Ql_atoi(strtok_r(last, ".", &last));
			if (Ql_SetLocalTime(&tm) != QL_RET_OK)
			{
				MENU("!!!!Error set date");
			}
			else {
				MENU("\r\nSet date is OK");
			}
		}
		choice = 0;
		DisplayMainMenu();
	}
}

void time() {
	if (choice == 0) {
		MENU("\r\nInsert time.\r\nFormat: 23:59:59\r\n");
		MEXIT;
		MENU("TIME: ");
		choice = 9;
	}
	else if (choice == 9 && (*insert == '?')) {
		DisplayMainMenu();
		choice = 0;
	}
	else {
		ST_Time tm;
		char* last;
		Ql_GetLocalTime(&tm);
		if (Ql_strlen(insert) == 8) {
			tm.hour = Ql_atoi(strtok_r(insert, ":", &last));
			tm.minute = Ql_atoi(strtok_r(last, ":", &last));
			tm.second = Ql_atoi(strtok_r(last, ":", &last));
			if (Ql_SetLocalTime(&tm) != QL_RET_OK)
			{
				MENU("\r\n!!!!Error set time");
			}
			else {
				MENU("\r\nSet time is OK");
			}
		}
		choice = 0;
		DisplayMainMenu();
	}
}

void tsend() {
	if (choice == 0) {
		MENU("\r\nInsert interval send empty packet from 30 to 55 second\r\n");
		MEXIT;
		MENU("TSEND: ");
		choice = 10;
	}
	else if (choice == 9 && (*insert == '?')) {
		DisplayMainMenu();
		choice = 10;
	}
	else {
		u32 ts = atoi(insert);
		if (ts < 30 || ts>55) {
			MENU("\r\n!Tsend must be in interval from 30 to 55\r\n");
		}
		else {
			__mdm_settings.t_send = ts;
		}
		choice = 0;
		DisplayMainMenu();
	}
}

void debounce() {
	if (choice == 0) {
		MENU("\r\nInsert debounce value from 100 to 1000\r\n");
		MEXIT;
		MENU("DEBOUNCE: ");
		choice = 11;
	}
	else if (choice == 11 && (*insert == '?')) {
		DisplayMainMenu();
		choice = 0;
	}
	else {
		u32 db = atoi(insert);
		if (db < 100 || db>1000) {
			MENU("\r\n!Debounce must be in interval from 100 to 1000\r\n");
		}
		else {
			__mdm_settings.deboung = db;
		}
		choice = 0;
		DisplayMainMenu();
	}
}

void number() {
	if (choice == 0) {
		MENU("\r\nNumber controller from 1 to 255\r\n");
		MEXIT;
		MENU("NUM: ");
		choice = 11;
	}
	else if (choice == 11 && (*insert == '?')) {
		DisplayMainMenu();
		choice = 0;
	}
	else {
		s32 db = Ql_atoi(insert);
		if (db < 1 || db>255) {
			MENU("\r\n!Number must be in interval from 1 to 255\r\n");
		}
		else {
			__mdm_settings.num= db;
		}
		choice = 0;
		DisplayMainMenu();
	}
}

void latidute() {
	if (choice == 0) {
		MENU("Insert latitude.\r\nFormat 55.1910967\r\n");
		MEXIT;
		MENU("LATITUDE: ");
		choice = 13;
	}
	else if (choice == 13 && (*insert == '?')) {
		DisplayMainMenu();
		choice = 0;
	}
	else {
		float db = Ql_atof(insert);
		__mdm_settings.latidute = db;
		choice = 0;
		DisplayMainMenu();
	}
}

void longitude() {
	if (choice == 0) {
		MENU("Insert longitude.\r\nFormat 30.1253259\r\n");
		MEXIT;
		MENU("LONGITUDE: ");
		choice = 14;
	}
	else if (choice == 14 && (*insert == '?')) {
		DisplayMainMenu();
		choice = 0;
	}
	else {
		float db = Ql_atof(insert);
		__mdm_settings.longitude = db;
		choice = 0;
		DisplayMainMenu();
	}
}

void use_est() {
	if (choice == 0) {
		MENU("0 - OFF 1 - ON\r\n");
		MEXIT;
		MENU("STATE: ");
		choice = 6;
	}
	else if (choice == 6 && (*insert == '?')) {
		DisplayMainMenu();
		choice = 0;
	}
	else {
		s32 state = atoi(insert);
		if (state < 0 && state>1) {
			MENU("!!!Error. 0 - OFF 1 - ON\r\n");
		}
		else
		{
			if (state == 0)
			{
				Ql_Timer_Stop(EST_CONFIRM_TIMER);
				Ql_Timer_Stop(EST_EMPTY_TIMER);
				Ql_Timer_Stop(EST_CONNECTOR_TIMER);
				__EST_CONNECT__ = FALSE;
				__mdm_settings.use_esttools = FALSE;
				write_mdm_settings();
				MENU("Set Est OFF\r\n");
			}
			else {

				__mdm_settings.use_esttools = TRUE;
				write_mdm_settings();
				_n_state = STATE_EST_CONNECT;
				MENU("Set Est ON\r\n");
			}
			DisplayMainMenu();
		}
		choice = 0;
	}
}

void lights() {
	if (choice == 0) {
		MENU("0 - OFF 1 - ON\r\n");
		MEXIT;
		MENU("LIGHTS STATE: ");
		choice = 12;
	}
	else if (choice == 12 && (*insert == '?')) {
		DisplayMainMenu();
		choice = 0;
	}
	else {
		s32 state = atoi(insert);
		if (state < 0 && state>1) {
			MENU("!!!Error. 0 - OFF 1 - ON\r\n");
		}
		else
		{
			if (state == 0)
			{
				//__mdm_settings.lights = FALSE;
				MENU("Send command lights OFF\r\n");
				u8 d[2] = { 5,0 };
				__mdm_settings.use_sched = FALSE;
				save_all_settings();
				mdm_msg_send(&d[0], 2);
			}
			else {
				
				//__mdm_settings.lights = TRUE;
				MENU("Send command lights ON\r\n");
				__mdm_settings.use_sched = FALSE;
				save_all_settings();
				u8 d[2] = { 5,1 };
				mdm_msg_send(&d[0], 2);
			}
		}
		choice = 0;
	}
}

void act_door() {
	if (choice == 0) {
		MENU("Lights doors control.\r\nFormat: 10101010110101\r\n");
		MEXIT;
		MENU("DOORS: ");
		choice = 17;
	}
	else if (choice == 17 && (*insert == '?')) {
		DisplayMainMenu();
		choice = 0;
	}
	else {
		u16 ch=0;
		set_act_chnll(insert, &ch,16);
		__mdm_settings.act_door_chanals = ch;
		save_all_settings();
		choice = 0;
		DisplayMainMenu();
	}
}

void act_in() {
	if (choice == 0) {
		MENU("Active input channels.\r\nFormat: 10101010110101\r\n");
		MEXIT;
		MENU("INPUT: ");
		choice = 18;
	}
	else if (choice == 18 && (*insert == '?')) {
		DisplayMainMenu();
		choice = 0;
	}
	else {
		u16 ch = 0;
		set_act_chnll(insert, &ch,16);
		
		__mdm_settings.dinn_active = ~ch;
		save_all_settings();
		choice = 0;
		DisplayMainMenu();
	}
}

void act_anin() {
	if (choice == 0) {
		MENU("Active analog channels.\r\nFormat: 10101\r\n");
		MEXIT;
		MENU("ANALOG: ");
		choice = 19;
	}
	else if (choice == 19 && (*insert == '?')) {
		DisplayMainMenu();
		choice = 0;
	}
	else {
		u16 ch = 0;
		set_act_chnll(insert, &ch,4);

		__mdm_settings.ainn_active = ~ch;
		save_all_settings();
		choice = 0;
		DisplayMainMenu();
	}
}

void deadband() {
	if (choice == 0) {
		MENU("\r\nInsert deadband value from 1 to 5\r\n");
		MEXIT;
		MENU("DEADBAND: ");
		choice = 20;
	}
	else if (choice == 20 && (*insert == '?')) {
		DisplayMainMenu();
		choice = 0;
	}
	else {
		u32 db = atoi(insert);
		if (db < 1 || db>5) {
			MENU("\r\n!Deadband must be in interval from 1 to 5\r\n");
		}
		else {
			__mdm_settings.deadband = db;
			save_all_settings();
		}
		choice = 0;
		DisplayMainMenu();
	}
}

void menu(char* inp) {
	Ql_memset(insert, 0, 50);
	Ql_memcpy(insert, inp, strlen(inp));
	s32 num = -1;
	if (choice == 0) {
		if(Ql_isdigit(*insert)>0)
			num= Ql_atoi(insert);
		
		if (*insert == '?') {
			if (__DEBUG__==TRUE) {
				__DEBUG__ = FALSE;
				MENU("\r\nDebuger stop\r\n");
			}
			DisplayMainMenu();
		}
		else if (num == 1) {
			apn();
		}
		else if (num== 2) {
			apn_u_p(TRUE);
		}
		else if (num == 3) {
			apn_u_p(FALSE);
		}
		else if (num == 4) {
			tcp();
		}
		else if (num == 5) {
			ip();
		}
		else if (num == 6) {
			use_est();
		}
		else if (num == 7) {
			//MENU("Debuger start...\r\n");
			//__DEBUG__ = TRUE;
			Debug();
		}
		else if (num == 8) {
			date();
		}
		else if (num == 9) {
			time();
		}
		else if (num == 10) {
			tsend();
		}
		else if (num == 11) {
			debounce();
		}
		else if (num == 12) {
			lights();
		}
		else if (num == 13) {
			latidute();
		}
		else if (num == 14) {
			longitude();
		}
		else if (num == 15) {
			save_all_settings();
			MENU("\r\nSave settings is OK\r\n");
			if (__mdm_settings.use_esttools == TRUE) {
				set_state_est(STATE_EST_CONNECT);
			}
			else {
				set_state_est(STATE_EST_STOP);
			}
				
			MENU("Close est connection\r\n");
			DisplayMainMenu();
		}
		else if (num == 16) {
			Ql_Reset(0);
		}
		else if (num == 17) {
			act_door();
		}
		else if (num == 18) {
			act_in();
		}
		else if (num == 19) {
			act_anin();
		}
		else if (num == 20) {
			deadband();
		}
		else if (num == 21) {
			use_scedulers();
		}
		else if (num == 0) {
			
			//Ql_Reset(0);
			__MENUS__ = FALSE;
			//if (Ql_strstr(__mdm_settings.debug, "TRUE")) {
			if(__mdm_settings.debug==TRUE){
				__DEBUG__ = TRUE;
			}
			else {
				__DEBUG__ = FALSE;
			}
			MENU("\r\nExit for edit settings.\r\nDebugger is:%s\r\n",__DEBUG__==TRUE ? "ON":"OFF");
		}

		else {
			MENU("\r\n!!!Wrong insert number!!!\r\n");
			MENU("Selection: ");
		}
	}

	else if (choice == 1) {
		apn();
	}
	else if (choice == 2) {
		apn_u_p(TRUE);
	}
	else if (choice == 3) {
		apn_u_p(FALSE);
	}
	else if (choice == 4) {
		tcp();
	}
	else if (choice == 5) {
		ip();
	}
	else if (choice == 6) {
		use_est();
	}
	else if (choice == 7) {
		Debug();
	}
	else if (choice == 8) {
		date();
	}
	else if (choice == 9) {
		time();
	}
	else if (choice == 10) {
		tsend();
	}
	else if (choice == 11) {
		debounce();
	}
	else if (choice == 12) {
		lights();
		
	}
	else if (choice == 13) {
		latidute();
	}
	else if (choice == 14) {
		longitude();
	}
	else if (choice == 17) {
		act_door();
	}
	else if (choice == 18) {
		act_in();
	}
	else if (choice == 19) {
		act_anin();
	}
	else if (choice == 20) {
		deadband();
	}
	else if (choice == 21) {
		use_scedulers();
	}
}
