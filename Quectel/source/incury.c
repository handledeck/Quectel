#include <qlobal.h>


u8		__rs_r_buf[RS_LEN_BUFFER];
u8		__rs_s_buf[RS_LEN_BUFFER];
u16		__count_r_buf = 0;

const u8 __sm[] = { 0x17,0x13 };
const u8 __em[] = { 0x17,0x5 };



void extract(u8* data) {
	
	enum EnFunc f = (enum EnFunc)*data;
	switch (f)
	{
	case none: {
		OUTD(">Event none");
			mdm_msg_send(0, 1);
		break;
	}
	case state: {
		//0x17, 0x13, 0x15, 0x01, 0xfb, 0xff, 0x00, 0x00, 0x16, 0x00, 0x0e, 0x00, 0xb2, 0x0b,
			//0x9e, 0x0c, 0xe0, 0xff, 0xff, 0xff, 0xff, 0x0f, 0xf4, 0x01, 0x76, 0x09, 0x71, 0x44, 0x17, 0x05
		
		Ql_memcpy(&__pins_input_value, ++data, sizeof(u16));
		data += 2;
		
		if (*data == 1)
			__power_state = 1;
		else
			__power_state = 0;
		data++;
		//OUTD("Power state:%d", __power_state);
		__pins_output_value = *data++;
		Ql_memcpy(&__pins_anl_value[0], data, (sizeof(u16)*4));
		u8 in[17];
		data_to_state(in, __pins_input_value, 16);
		u8 out[9];
		data_to_state(out, __pins_output_value, 8);
		u8 lights = 0xFF & __pins_output_value;
		if (lights > 0)
			__mdm_settings.lights = TRUE;
		else
			__mdm_settings.lights = FALSE;
		data += sizeof(u16) * 4;
		Ql_memcpy(&__mdm_settings.act_door_chanals, data, sizeof(u16));
		data += 2;
		Ql_memcpy(&__mdm_settings.dinn_active, data, sizeof(u16));
		data += 2;
		Ql_memcpy(&__mdm_settings.dout_active, data++, sizeof(u8));
		Ql_memcpy(&__mdm_settings.ainn_active, data++, sizeof(u8));
		Ql_memcpy(&__mdm_settings.deboung, data, sizeof(u16));
		data += 2;
		Ql_memcpy(&__mdm_settings.deadband, data, sizeof(u8));
		//OUTD(">Event state: in:%s out:%s an:%d %d %d %d",in,out,__pins_anl_value[0], __pins_anl_value[1], __pins_anl_value[2], __pins_anl_value[3]);
		OUTD("<SWAP OK");
		break;
	}
	case through:
	{
		u8 tlen = *--data;
		data += 2;
		if (__rem_socket_th_id != -1) {
			s32 lwrite = Ql_SOC_Send(__rem_socket_th_id, data, tlen - 1);
			//OUTD(">Send to client:%d", lwrite);
		}
		//data_to_hex(data, tlen-1);
		break;
	}
	case evti:
	{
		u8 input=*++data;
		data++;;
		//OUTD(">Event input:%d val:%d", input, *data);
		
		u8 desc=0;
		EventData ilocdata;
		GetInputDescripon(input, (char)*data, &desc);
		TrigerEvent(desc, 0, &ilocdata);
		write_events(&ilocdata);
		if (input == 16) {
			if (*data == 1)
				__power_state = 1;
			else
				__power_state=0;
		}
		else {
			if (*data == 1) {
				OUTDS("EVT:DI:%d,0", input);
				__pins_input_value ^= (1 << input);
			}
			else {
				u32 s = 0;
				s ^= (1 << input) ^ 0xFFFF;
				__pins_input_value &= s;
				OUTDS("EVT:DI:%d,1", input);
			}
		}
		
		break;
	}
	case evta: {
		
		u8 input = *++data;
		data++;
		
		u8 desc = 0;
		u16 val = 0;
		EventData ilocdata;
		GetAnalogDescripon(input, &desc);
		
		memcpy(&val, data, sizeof(u16));
		TrigerEvent(desc, val, &ilocdata);
		//OUTD(">Event analog change:%d val:%d",input,val);
		OUTDS("EVT:DA:%d,%d", input, val);
		write_events(&ilocdata);
		__pins_anl_value[input] = val;
		break;
	}
	case evto:
	{
		//Что с этим делать? 
		data++;
		u8 output=*data;
		data++;
		u8 pos =*data;
		//OUTD(">Event out:%d val:%d", output, *data);
		OUTDS("PWD:OK LIGHTS:%d", *data);
		OUTDS("EVT:DO:%d,%d", output, *data);
		u8 desc=0;
		EventData ilocdata;
		GetOutDescripon(output, pos, &desc);
		TrigerEvent(desc, 0, &ilocdata);
		write_events(&ilocdata);
		if (*data = 1) {
			__pins_output_value ^= (1 << output);
		}
		else {
			u32 s = 0;
			s ^= (1 << output) ^ 0xFF;
			__pins_output_value &= s;
		}
		
		break;
	}
	case getsettings: {
		u8 tlen = *--data;
		data += 2;
		//data_to_hex(data, tlen-1);
		Ql_memcpy(&__mdm_settings.act_door_chanals, data, sizeof(u16));
		data += 2;
		Ql_memcpy(&__mdm_settings.dinn_active, data, sizeof(u16));
		data += 2;
		Ql_memcpy(&__mdm_settings.dout_active, data++, sizeof(u8));
		Ql_memcpy(&__mdm_settings.ainn_active, data++, sizeof(u8));
		Ql_memcpy(&__mdm_settings.deboung, data, sizeof(u16));
		data += 2;
		Ql_memcpy(&__mdm_settings.deadband, data, sizeof(u16));
		break;
	}
	case setsettings: {
		
	}
	case debug: {
		char sbg[100] = "\0";
		u8 tlen = *--data;
		data += 2;
		Ql_memcpy(sbg, data, tlen-2);
		OUTD("SAMDBG>%s", sbg);
		break;
	}
	case update: {
		data++;
		u16 row;
		Ql_memcpy(&row, data, 2);
		OUTD("sam row:%d", row);
		//if (__updater == U_SAM_WAIT) {
		//}
		//else if(__updater == U_M66_WAIT)
		//{
			//__updater = U_M66;
		//}
		break;
	}
	default:
		break;
	}
}


static void check_time(u8* data) {
	u32 tm_rem;
	ST_Time t1;
	Ql_GetLocalTime(&t1);
	u32 tm_loc = rtc_calendar_time_to_register_value(&t1);
	Ql_memcpy(&tm_rem, data, sizeof(u32));
	s32 res=Ql_abs(tm_loc - tm_rem);
	if (res > 60) {
		OUTD("NEED TO SYNC DATE TIME!!!! Result:%d second",res);
		mdm_msg_send(0, 1);
	}
}

void check() {
	if (__count_r_buf > 0) {
		u16 count_now = __count_r_buf;
		for (u16 i = 0; i < __count_r_buf; i++)
		{
			u8* pbuf = &__rs_r_buf[0];
			u8* prun = &__rs_r_buf[1];
			if (*pbuf == 0x17 && *prun == 0x13)
			{
				u16 pack_len = __rs_r_buf[2];
				if (pack_len + 9 <= __count_r_buf) {

					if (__rs_r_buf[pack_len + 7] == 0x17 && __rs_r_buf[pack_len + 8] == 0x5) {
						check_time(&__rs_r_buf[pack_len + 3]);
						extract(&__rs_r_buf[3]);
						Ql_memcpy(&__rs_r_buf[0], &__rs_r_buf[pack_len + 9], __count_r_buf - (pack_len + 9));
						__count_r_buf -= (pack_len + 9);
						if (__count_r_buf == 0)
						{
							Ql_memset(__rs_r_buf, 0, RS_LEN_BUFFER);
						}
					}
					else {
						//not find end x17x5.clean 2 first byes
						Ql_memcpy(&__rs_r_buf[0], &__rs_r_buf[2], __count_r_buf - 2);
						__count_r_buf -= 2;
					}
				}
				else {
					//wait for part message;
				}
				return;
			}
			else {
				//wrong message. not start x17x15.clean one byte.
				Ql_memcpy(&__rs_r_buf[0], &__rs_r_buf[1], count_now);
				count_now -= 1;
				__count_r_buf = count_now;
			}

		}
		__count_r_buf = count_now;
	}
	
}

void add(u8* data, u16 count) {
	if (__count_r_buf + count > RS_LEN_BUFFER)
	{
		Ql_memset(__rs_r_buf, 0, RS_LEN_BUFFER);
		Ql_memcpy(__rs_r_buf, data, count);
		__count_r_buf = count;
	}
	else {
		Ql_memcpy(&__rs_r_buf[__count_r_buf], data, count);
		__count_r_buf += count;
	}
}

void mdm_msg_send(u8* data,u16 len) {
	u8* pbuf = &__rs_s_buf[0];
	Ql_memset(pbuf, 0, RS_LEN_BUFFER);
	Ql_memcpy(pbuf, __sm, sizeof(__sm));
	pbuf += 2; *pbuf++ = len;
	memcpy(pbuf, data, len);
	pbuf += len;
	ST_Time time;
	Ql_GetLocalTime(&time);
	//rtc_calendar_get_time(&__lights_rtc, &time);
	u32 tval = rtc_calendar_time_to_register_value(&time);
	Ql_memcpy(pbuf, &tval, sizeof(u32));
	pbuf += sizeof(u32);
	Ql_memcpy(pbuf, __em, sizeof(__em));
	//pbuf += 2;
	Ql_UART_Write(UART_PORT3, &__rs_s_buf[0], LEN_MARK_SERVICE + len);
	//usart_write_buffer_wait(&__mdm_uart, &__rs_s_buf[0], LEN_MARK_SERVICE + len);
	
}


