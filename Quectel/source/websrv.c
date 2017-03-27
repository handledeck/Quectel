#include <qlobal.h>

const char Base64Table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
char* __error_page = "HTTP/1.1 404 Not Found\nContent-Type: text/html\nContent-Length: 0\n\n";
char* __autorize = "HTTP 401 Not Authorized\r\nWWW-Authenticate: Basic realm=\"insert realm\"\r\nContent-Length: %d\r\n\r\n";
char* __ok_page = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %d\r\n\r\n";
char* __download_page = "HTTP/1.1 200 OK\r\ncontent-description: File Transfer\r\ncontent-type: application/octet-stream\r\ncontent-disposition: attachment; filename=log.txt\r\nContent-Length: %d\r\n\r\n";
char* __main_page = "<html><head><style>td{border:none;font-family:Tahoma;}.al{text-align:right;}</style></head>"
"<body>"
"<div style='padding-top:100px' align='center'>"
"<h1>Controller IP address %s</h1>"
"<form action='settings'>"
"<table style='width:800px;border-collapse:collapse'>"
"<tr><td>Signal strength, unit in dBm</td><td class='al'>%d</td></tr>"
"<tr><td>Input state</td><td class='al'>%s</td></tr>"
"<tr><td>Output state</td><td class='al'>%s</td></tr>"
"<tr><td>Date Time</></td><td class='al'>%.2d.%.2d.%.2d %.2d:%.2d:%.2d</td></tr>"
"<tr><td>Apn</td><td class='al'>%s</td></tr>"
"<tr><td>Address to connect</td><td class='al'><input style='text-align:right;font-size:15' type='text' name='IP' pattern=\"((^|\\.)((25[0-5])|(2[0-4]\\d)|(1\\d\\d)|([1-9]?\\d))){4}$\" value='%d.%d.%d.%d' required/></td></tr>"
"<tr><td>Port to connect</td><td class='al'><input style='text-align:right;;font-size:15' type='number' name='TCP' min='1025' max='65535' value='%d' required/></td></tr>"
"<tr><td>Time to send empty packet</td><td class='al'><input style='text-align:right;font-size:15' type='number'  min='30' max='55' step='5' value='%d' name='TSEND' required/></td></tr>"
"<tr><td>Debounce</td><td class='al'><input style='text-align:right;font-size:15' type='number' min='100' max='1000' step='50' value='%d' name='DEBOUNCE' required/></td></tr>"
//"<tr><td>Debug</td><td class='al'><select name='DEBUG'>%s</select></td></tr>"
"<tr><td>State Est</td><td class='al'><span style='color:%s;font-size:25'>&#9679</span></td></tr>"
"<tr><td>Lights</td><td class='al'><span style='color:%s;font-size:25'>&#9679</span></td></tr>"
"<tr><td>Use Schedulers</td><td class='al'><select name='RAS'>%s</select></td></tr>"
"<tr><td style='border-bottom:double'>&nbsp</td><td style='border-bottom:double'>&nbsp</td></tr>"
"<tr><td>&nbsp</td><td>&nbsp</td></tr>"
"<tr><td>&nbsp</td><td class='al'><code>%s</code></td></tr>"
"<tr><td>&nbsp</td><td>&nbsp</td></tr>"
"<tr><td><input type='submit'  value='Save settings' />\r\n"
"<input type='button' value='Set Date time' onclick=\"javascript:var d=new Date();location.href='date?'+(d.getFullYear()-2000)+'-'+(d.getMonth()+1)+'-'+d.getDate()+'T'+d.getHours()+':'+d.getMinutes()+':'+d.getSeconds()\" />\r\n"
"<input type='button' value='%s' onclick=\"javascript:location.href='%s'\" />\r\n"
"<input type='button' value='%s' onclick=\"javascript:location.href='%s'\" />\r\n"
"<input type='button' value='Refresh' onclick=\"javascript:location.href='/'\" />\r\n"
"</td>"
"<td align='right'>"
"<input type='button' value='Reboot' onclick=\"javascript: location.href = 'reboot'\" />"
"</td>"
"</tr>"
"</table>"
"</form>"
"</div>"
"</body></html>\0";
char* __reboot_page = "<html><body><div style='padding-top:100px' align='center'><h1>Controller ip:%s reboot</h1></div></body></html>";


bool SetLocDate(char* line) {
	char* pch;
	char* last;
	int inx = 0;
	ST_Time ql;
	Ql_GetLocalTime(&ql);
	//OUTD("line:%s",line);
	pch = strtok_r(line, "T", &last);
	pch = strtok(pch, "-");
	while (pch != NULL)
	{
		//OUTD("ph:%s", pch);
		if (inx == 0)
			ql.year =(2000+Ql_atoi(pch));
		if (inx == 1)
			ql.month = Ql_atoi(pch);
		if (inx == 2)
			ql.day = Ql_atoi(pch);
		pch = strtok(NULL, "-");
		inx++;
	}
	
	inx = 0;
	pch = strtok(last, ":");
	while (pch != NULL)
	{
		if (inx == 0)
			ql.hour = Ql_atoi(pch);
		if (inx == 1)
			ql.minute = Ql_atoi(pch);
		if (inx == 2)
			ql.second = Ql_atoi(pch);
		pch = strtok(NULL, ":");
		inx++;
	}
	//OUTD("date:%d.%d.%d %d:%d:%d",ql.day,ql.month,ql.year,ql.hour, ql.minute, ql.second);
	if (QL_RET_OK == Ql_SetLocalTime(&ql))
		return TRUE;
	else
	return FALSE;
}

void get_error_page(void) {

}

void get_rebot_page(char* buffer) {
	Ql_sprintf(buffer, __reboot_page, __STR_LOC_IP_ADDR);
}

void get_main_page(const char* message,char* buffer) {
	u8 in[17];
	u8 out[9];
	u16 rev = ~__pins_input_value;
	data_to_state(in, rev, 16);
	data_to_state(out, __pins_output_value, 8);
	ST_Time loct;
	u32 rssi;
	u32 ber;
	Ql_memset(buffer, "\0", HTTP_DATA_LEN);
	//RIL_SIM_GetSimState(&simcard);
	
	//RIL_NW_GetGSMState(&creg);
	//RIL_NW_GetGPRSState(&cgreg);
	RIL_NW_GetSignalQuality(&rssi, &ber);
	Ql_GetLocalTime(&loct);
	//GetTextStateGpio(&pp[0]);
	//OUTD("pass through:%s %d %d %d %d %d %d", __STR_LOC_IP_ADDR, loct.day, loct.month, loct.year, loct.hour, loct.minute, rssi);
	Ql_sprintf(&buffer[0], __main_page,
		__STR_LOC_IP_ADDR,
		rssi,
		&in[0],
		&out[0],
		loct.day, loct.month, loct.year, loct.hour, loct.minute, loct.second,
		__mdm_settings.apn,
		__mdm_settings.ip[0], __mdm_settings.ip[1], __mdm_settings.ip[2], __mdm_settings.ip[3],
		__mdm_settings.tcp,
		__mdm_settings.t_send,
		__mdm_settings.deboung,
		//__DEBUG__ ? "<option selected>ON</option><option>OFF</option>" : "<option>TRUE</option><option selected>OFF</option>",
		__EST_CONNECT__ ? "00FF00" : "FF0000",
		__pins_output_value & 0x3 ? "00FF00" : "FF0000",
		__mdm_settings.use_sched ? "<option selected>ON</option><option>OFF</option>" : "<option selected>OFF</option><option>ON</option>",
		message,
		__EST_CONNECT__ ? "Stop Est" : "Start Est",
		__EST_CONNECT__ ? "stop" : "run",
		__pins_output_value & 0x3 ? "Lights off" : "Lights on",
		__pins_output_value & 0x3 ? "lioff" : "lion"
		
		);
}

bool vilidation(HttpHeader *http_header) {
	OUTD("cookie value is:%s", http_header->cookie);
	if (Ql_strstr(http_header->cookie, "NONE"))
	{
		return FALSE;
	}
	else return TRUE;
}

void page_response(HttpHeader *header) {
	bool reset = FALSE;
	bool prepare = TRUE;
	char* action = header->action;
	OUTD("action=%s", action);
	char* buffer = (char*)Ql_MEM_Alloc(SEND_BUF_LEN);
	
	Ql_memset(__s_buf, "\0", SEND_BUF_LEN);
	
	if (Ql_strstr(action, "/")) {
		get_main_page(__EST_CONNECT__ ? "System state is start" : "System state stop", buffer);
		//OUTD("main page:%d %d", Ql_strlen(__main_page), Ql_strlen(buffer));
	}
	else if (Ql_strstr(action, "reboot")) {
		get_rebot_page(buffer);
		reset = TRUE;
	}
	else if (Ql_strstr(action, "settings?")) {
		char* pch = NULL;
		char* last = NULL;
		//OUTD("parse setting action:%s",pch);
		pch = strtok_r(action + 9, "&", &last);
		//OUTD("parse command:%s",pch);
		while (pch != NULL) {
			set_command(pch);
			pch = strtok_r(NULL, "&", &last);
			//OUTD("parse command:%s",pch);
		}

		//write_mdm_settings();
		save_all_settings();
		//read_mdm_settings();
		get_main_page("Save settings controller is OK", buffer);
	}
	else if (Ql_strstr(action, "date?")) {
		bool sdt = SetLocDate(action + 5);
		get_main_page(sdt ? "Set date and time is OK" : "Error set date and time", buffer);
	}
	else if (Ql_strstr(action, "stop")) {
		//TODO   !!---------------------------EST--------------------------------!!  
		//StopEstConnection();
		OUTD("WEB:recieve command est stop");
		Ql_SOC_Close(__socket_est_id);
		Ql_Timer_Stop(EST_CONFIRM_TIMER);
		Ql_Timer_Stop(EST_EMPTY_TIMER);
		Ql_Timer_Stop(EST_CONNECTOR_TIMER);
		__EST_CONNECT__ = FALSE;
		__mdm_settings.use_esttools = FALSE;
		write_mdm_settings();
		get_main_page("Recive command est stop", buffer);
	}
	else if (Ql_strstr(action, "run")) {
		//nSTATE = STATE_EST_SOCKET_CREATE;
		OUTD("WEB:recieve command est start");
		__mdm_settings.use_esttools = TRUE;
		write_mdm_settings();
		_n_state = STATE_EST_CONNECT;
		get_main_page("Recive command est start", buffer);
	}
	else if (Ql_strstr(action, "lion")) {
		__mdm_settings.use_sched = FALSE;
		save_all_settings();
		u8 d[2] = { 5,1 };
		mdm_msg_send(&d[0], 2);
		get_main_page("Action command lights on", buffer);
	}
	else if (Ql_strstr(action, "lioff")) {
		__mdm_settings.use_sched = FALSE;
		save_all_settings();
		u8 d[2] = { 5,0 };
		mdm_msg_send(&d[0], 2);
		get_main_page("Action command lights off", buffer);
	}
	else if (Ql_strstr(action, "autorize")) {
		Ql_sprintf(__s_buf, __autorize, 0);
		prepare = FALSE;
		//OUTD("set autorize", NULL);
	}
	else {
		get_error_page();
		Ql_sprintf(__s_buf, __error_page, 0);
		prepare = FALSE;
	}
	if (prepare) {
		//if (header->set_cookie) {
		//   Ql_sprintf(__s_buf, __ok_page, Ql_strlen(__data_buf),"\r\nSet-Cookie: __ztp=987654321\r\n"); 
		//}
		Ql_sprintf(&__s_buf[0], __ok_page, Ql_strlen(buffer));
		Ql_strcpy(&__s_buf[Ql_strlen(__s_buf)], buffer);
		//OUTD("sbuf:%d dbuf:%d", Ql_strlen(__s_buf), Ql_strlen(buffer));

	}
	if (reset) {
		Ql_Sleep(1000);
		Ql_Reset(0);
	}
	if (buffer != NULL)
		Ql_MEM_Free(buffer);
}

bool parse_header(u8* header, HttpHeader *http_header) {
	u8 *param;
	u8 *value;
	u8 *last;
	u8 *ph;
	u8 *lens = NULL;
	char* head = Ql_strstr(header, "\r\n\r\n");
	if (head) {
		////header = (char*)Ql_MEM_Alloc(1024);
		//http_header->set_cookie = FALSE;
		//http_header->authorization = FALSE;
		//Ql_memset(http_header->action, 0, 100);
		////Ql_memset(http_header->boundary, 0, 200);
		//Ql_memset(http_header->cookie, 0, 200);
		////Ql_memset(http_header->data, 0, 10 * 1024);
		//http_header->length = 0;
		//Ql_strcpy(http_header->cookie, "NONE");
		////int head_len = (char*)head - http_data);
		////int body_len = (int)(client->pRecvCurrentPos) - head_len - 4;
		////Ql_memcpy(header, http_data, head_len);
		
		ph = strtok_r(header, "\r\n", &last);
		char* lst;
		if (Ql_strstr(ph, "GET") != NULL)
		{
			char* act = strtok_r(ph + 4, " ", &lst);
			if (Ql_strlen(act)>1)
				Ql_strcpy(http_header->action, act + 1);
			else
				Ql_strcpy(http_header->action, act);
			http_header->method = GET;
		}
		if (Ql_strstr(ph, "POST") != NULL) {
			http_header->method = POST;
			char* act = strtok_r(ph + 5, " ", &lst);
			if (Ql_strlen(act)>1)
				Ql_strcpy(http_header->action, act + 1);
			else
				Ql_strcpy(http_header->action, act);
		}
		while (ph != NULL)
		{
			ph = strtok_r(NULL, "\r\n", &last);
			if (ph != NULL) {
				param = strtok_r(ph, ":", &value);
				if (param != NULL && value != NULL)
				{
					//OUTD("param:%s", param);
					///*if (Ql_strstr(param, "Content-Type")) {
					//	char* bon = Ql_strstr(value, "boundary");
					//	bon_val = bon + 9;
					//	Ql_strcpy(http_header->boundary, bon_val);
					//}*/
					if ((lens = Ql_strstr(param, "Cookie")) != NULL)
					{
						u8* cookie;
						if ((cookie = Ql_strstr(value, "__ztp")) != NULL) {
							Ql_strcpy(http_header->cookie, cookie + 6);
							http_header->authorization = TRUE;
							http_header->set_cookie = FALSE;
						}
					}
					///*else if ((lens = Ql_strstr(param, "Content-Length")) != NULL) {
					//	char* slen = lens + 16;
					//	int len_head = Ql_atoi(slen);
					//	http_header->length = len_head;
					//	if (len_head == body_len) {
					//		OUTD("data full", NULL);
					//	}
					//	else
					//	{
					//		OUTD("not full data.wait:%d have:%d", len_head, body_len);
					//		return FALSE;
					//	}
					//}*/
					if ((lens = Ql_strstr(param, "Authorization")) != NULL) {
						s8* slen = lens + 21;
						u8 dbuf[50];
						Ql_memset(dbuf, 0, 50);
						decode_b64((u8*)slen, dbuf);
						//OUTD("decode buf:%s", dbuf);
						param = strtok_r((char*)dbuf, ":", &value);
						if (Ql_strstr(param, &__mdm_settings.w_user) && Ql_strstr(value, &__mdm_settings.w_password)) {
							//OUTD("Autorization user:%s pswd:%s", param, value);
							http_header->authorization = TRUE;
							http_header->set_cookie = TRUE;
						}
					}

				}
			}
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//bool get_http_data(QlClient *client, HttpHeader *header) {
//	char* fota = Ql_strstr((char*)client->recv_buf, "QUECTEL_FOTA_UPGRADE");
//	u16 pos = client->pRecvCurrentPos;
//	if (fota) {
//		unsigned char* end = &client->recv_buf[pos - (Ql_strlen(header->boundary) + 8)];
//		if (Ql_strstr((char*)end, header->boundary)) {
//			int lens = (char*)end - fota;
//			Ql_memcpy(header->data, fota, lens);
//		}
//		return TRUE;
//	}
//	else
//		return FALSE;
//
//}

s32 encode_b64(const unsigned char *buftoenc, unsigned char *encbuf)
{
	// выделяем память под временный буфер
	//unsigned char *buftemp = (unsigned char*)Ql_GetMemory(bufsize + 3);
	u8 bufsize = 50;
	unsigned char buftemp[50];
	Ql_memset(buftemp, '\0', bufsize + 3);
	Ql_memcpy(buftemp, buftoenc, bufsize);

	s32 i = 0;
	s32 b64byte[5];

	while (i < bufsize)
	{
		b64byte[0] = buftemp[i] >> 2;
		b64byte[1] = ((buftemp[i] & 3) << 4) | (buftemp[i + 1] >> 4);
		b64byte[2] = ((buftemp[i + 1] & 0x0F) << 2) | (buftemp[i + 2] >> 6);
		b64byte[3] = buftemp[i + 2] & 0x3F;
		if (b64byte[0] == 0)
			encbuf[i + (i / 3)] = '=';
		else
			encbuf[i + (i / 3)] = Base64Table[b64byte[0]];

		if (b64byte[1] == 0)
			encbuf[i + (i / 3) + 1] = '=';
		else
			encbuf[i + (i / 3) + 1] = Base64Table[b64byte[1]];

		if (b64byte[2] == 0)
			encbuf[i + (i / 3) + 2] = '=';
		else
			encbuf[i + (i / 3) + 2] = Base64Table[b64byte[2]];

		if (b64byte[3] == 0)
			encbuf[i + (i / 3) + 3] = '=';
		else
			encbuf[i + (i / 3) + 3] = Base64Table[b64byte[3]];
		i += 3;
	}

	//Ql_FreeMemory(buftemp);
	return Ql_strlen((char*)encbuf);
}

s32 decode_b64(u8 *buftodec, u8 *decbuf)
{
	// выделяем память под временный буфер
	//unsigned char *buftemp =(unsigned char*)Ql_GetMemory(bufsize);
	u8 bufsize = 50;
	unsigned char buftemp[50];
	Ql_memset(buftemp, '\0', bufsize);
	Ql_memcpy(buftemp, buftodec, bufsize);

	int i = 0;
	int cpos[5];
	unsigned char binbyte[4];

	while (i < bufsize)
	{
		if (buftemp[i] == '=')
			cpos[0] = 0;
		else
			cpos[0] = Ql_strchr(Base64Table, buftemp[i]) - Base64Table;

		if (buftemp[i + 1] == '=')
			cpos[1] = 0;
		else
			cpos[1] = Ql_strchr(Base64Table, buftemp[i + 1]) - Base64Table;

		if (buftemp[i + 2] == '=')
			cpos[2] = 0;
		else
			cpos[2] = Ql_strchr(Base64Table, buftemp[i + 2]) - Base64Table;

		if (buftemp[i + 3] == '=')
			cpos[3] = 0;
		else
			cpos[3] = Ql_strchr(Base64Table, buftemp[i + 3]) - Base64Table;

		binbyte[0] = ((cpos[0] << 2) | (cpos[1] >> 4));
		binbyte[1] = ((cpos[1] << 4) | (cpos[2] >> 2));
		binbyte[2] = (((cpos[2] & 0x03) << 6) | (cpos[3] & 0x3f));
		decbuf[i - (i / 4)] = binbyte[0];
		decbuf[i - (i / 4) + 1] = binbyte[1];
		decbuf[i - (i / 4) + 2] = binbyte[2];
		i += 4;
	}

	//Ql_FreeMemory(buftemp);
	return Ql_strlen((char*)decbuf);
}


s32 decode_b64_bin(const unsigned char *buftodec, u8 size, unsigned char *decbuf)
{
	// выделяем память под временный буфер
	//unsigned char *buftemp =(unsigned char*)Ql_GetMemory(bufsize);
	u8 bufsize = 1024;
	unsigned char buftemp[1024];
	Ql_memset(buftemp, '\0', bufsize);
	Ql_memcpy(buftemp, buftodec, bufsize);

	int i = 0;
	int cpos[5];
	unsigned char binbyte[4];
	int pos = 0;
	while (i < size)
	{
		if (buftemp[i] == '=')
			cpos[0] = 0;
		else
			cpos[0] = Ql_strchr(Base64Table, buftemp[i]) - Base64Table;

		if (buftemp[i + 1] == '=')
			cpos[1] = 0;
		else
			cpos[1] = Ql_strchr(Base64Table, buftemp[i + 1]) - Base64Table;

		if (buftemp[i + 2] == '=')
			cpos[2] = 0;
		else
			cpos[2] = Ql_strchr(Base64Table, buftemp[i + 2]) - Base64Table;

		if (buftemp[i + 3] == '=')
			cpos[3] = 0;
		else
			cpos[3] = Ql_strchr(Base64Table, buftemp[i + 3]) - Base64Table;

		binbyte[0] = ((cpos[0] << 2) | (cpos[1] >> 4));
		binbyte[1] = ((cpos[1] << 4) | (cpos[2] >> 2));
		binbyte[2] = (((cpos[2] & 0x03) << 6) | (cpos[3] & 0x3f));
		decbuf[i - (i / 4)] = binbyte[0];
		decbuf[i - (i / 4) + 1] = binbyte[1];
		decbuf[i - (i / 4) + 2] = binbyte[2];
		i += 4;
		pos += 3;
	}

	//Ql_FreeMemory(buftemp);
	return pos;// Ql_strlen((char*)decbuf);
}










