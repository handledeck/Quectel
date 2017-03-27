
#include <qlobal.h>

u8 __SendBuffer[LEN_BUFFER_SEND];
u16 __countMsgSend = 0;
u8 __countRepeat = 0;
u32 __count_num_packets = 90;
//u16 __pins = 0;
s8 __waitConfirm = 0;
u16 __numPacket=0;
u32 __toSend=0;
EventData __currentEvData;

u16		__pins_input_value;
u8		__pins_output_value=0;
u16		__pins_anl_value[4];
u8		__power_state;

static const unsigned short Crc16Table[256] = {
	0x000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
	0x810, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
	0x123, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
	0x933, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
	0x246, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
	0xA56, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
	0x365, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
	0xB75, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
	0x48C, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
	0xC9C, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
	0x5AF, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
	0xDBF, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
	0x6CA, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
	0xEDA, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
	0x7E9, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
	0xFF9, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
	0x918, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
	0x108, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
	0x83B, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
	0x02B, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
	0xB5E, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
	0x34E, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
	0xA7D, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
	0x26D, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
	0xD94, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
	0x584, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
	0xCB7, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
	0x4A7, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
	0xFD2, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
	0x7C2, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
	0xEF1, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
	0x6E1, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};

u16 calc_crc(u8 *buf, u16 len) {
	//   Name  : CRC-16 CCITT
	//   Poly  : 0x1021    x^16 + x^12 + x^5 + 1
	//   Init  : 0xFFFF
	//   Revert: false
	//   XorOut: 0x0000
	//   Check : 0x29B1 ("123456789")
	//   MaxLen: 4095 байт (32767 бит) - обнаружение одинарных, двойных, тройных и всех нечетных
	unsigned short crc = 0xFFFF;
	while (len--)
		crc = (crc << 8) ^ Crc16Table[(crc >> 8) ^ *buf++];
	return crc;
}



unsigned short ReverseUSort(unsigned short* value) {
	unsigned short val = 0;
	val = (*value << 8 & 0xFF00) ^ (*value >> 8 & 0x00FF);
	//val=value[1]<<8^value[0];
	return val;
	//return val;
}


u32 ReverseUInt(u32* value) {
	return *value << 24 ^ (*value >> 8 & 0xFF) << 16 ^ (*value >> 16 & 0xFF) << 8 ^ (*value >> 24 & 0xFF);
}

void BuildEvent(unsigned char* data, unsigned short* pos, EventData* events, unsigned short count) {
	for (u32 i = 0; i < count; i++) {
		unsigned short evdesc;
		EventData* evdata = &events[i];
		data[*pos] = evdata->TypeEvent;
		*pos += 1;
		evdesc = evdata->EventDesc;
		WriteShort(&data[*pos], &evdesc);
		*pos += 2;
		Ql_memcpy(&data[*pos], &evdata->TimeEvent[0], 6);

		*pos += 6;
	}
}


//void BuildInput(unsigned char* data, unsigned short* pos) {
//	//состояние входов
//	data[*pos] = __pins;
//	*pos += 1;
//	WriteShort(&data[*pos], 0);
//	*pos += 2;
//	WriteShort(&data[*pos], 0);
//	*pos += 2;
//	WriteShort(&data[*pos], 0);
//	*pos += 2;
//	WriteShort(&data[*pos], 0);
//	*pos += 1;
//	data[*pos] = 0;
//	*pos++;
//}


void TrigerEvent(TypeEvent typeEvent, unsigned short description, EventData* eventData) {
	eventData->TypeEvent = typeEvent;
	eventData->EventDesc = description;
	SetEventTime(eventData);
}


void WriteShort(unsigned char* data, unsigned short* value) {
	unsigned short val = ReverseUSort(value);
	Ql_memcpy(data, &val, sizeof(unsigned short));
}

void WriteAddress(unsigned char* data, unsigned int* value) {
	int val = *value << 16 ^ (0x0000FF00 & *value) ^ (*value >> 16 & 0x000000FF);
	Ql_memcpy(data, &val, 3);
}

int BuildEventMessage(unsigned char PowerState, EventData* events, unsigned short len_evt) {
	unsigned short len = LEN_WITHOUT_DATA + (LEN_EVENT + (LEN_EVENT_DATA*len_evt));
	//unsigned char* tempBuf = (unsigned char*)Ql_GetMemory(len+LEN_WITHOUT_DATA);
	unsigned char tempBuf[512];
	unsigned short pos = 0;
	unsigned int address = __mdm_settings.num;
	unsigned short lensend = 0;
	u16 crc;
	//write lenght message
	//Ql_memset(&tempBuf[0],0,LEN_EVENT_DATA*(len_evt)+LEN_WITHOUT_DATA);
	Ql_memset(&tempBuf[0], 0, 512);
	WriteShort(&tempBuf[pos], &len);
	pos += 2;
	//write address device
	WriteAddress(&tempBuf[pos], &address);
	pos += 3;
	//тип пакаета
	tempBuf[pos] = 0;
	//номер пакета
	WriteShort(&tempBuf[++pos], &__numPacket);
	pos += 2;
	//источник питания
	tempBuf[pos] = __power_state;
	pos++;
	//упаковка всех сообщений
	BuildEvent(tempBuf, &pos, events, len_evt);
	//расчет изапись crc
	crc = calc_crc(&tempBuf[0], len - 2);
	WriteShort(&tempBuf[pos], &crc);
	pos += 2;
	Ql_memset(&__SendBuffer[0], 0, LEN_BUFFER_SEND);
	//формирование основного пакета для передачи		
	__SendBuffer[0] = 0x17; __SendBuffer[1] = 0x13;
	//замена символов
	lensend = replace(&tempBuf[0], pos, &__SendBuffer[2]);
	lensend += 2;
	//Ql_FreeMemory(tempBuf);
	//окончание формирования сообщения
	__SendBuffer[lensend] = 0x17; __SendBuffer[++lensend] = 0x5;
	//OUTD("Build message event.Number packet:%d",__numPacket);
	__waitConfirm = 1;
	return ++lensend;
}


int BuildInputStateMessage() {
	unsigned short pos = 0;
	unsigned int address = __mdm_settings.num;
	unsigned short lensend = 0;
	u16 crc;
	unsigned short len = LEN_INPUT_STATE_PACKET;
	unsigned char tempBuf[19];

	//len = LEN_WITHOUT_DATA + LEN_INPUT_STATE;

	//tempBuf = (u8*)Ql_GetMemory(len);

	Ql_memset(&tempBuf[0], 0, len);

	WriteShort(&tempBuf[pos], &len);
	pos += 2;
	//write address device
	WriteAddress(&tempBuf[pos], &address);
	pos += 3;
	//тип пакаета
	tempBuf[pos] = 4;
	pos += 1;
	//упаковка сообщения
	//BuildInput(tempBuf,&pos);
	Ql_memcpy(&tempBuf[pos], &__pins_input_value, sizeof(u16));
	//tempBuf[pos] = __pins;
	pos += 2;
	Ql_memcpy(&tempBuf[pos], &__pins_anl_value, sizeof(u16)*4);
	pos += 8;
	Ql_memcpy(&tempBuf[pos], &__pins_output_value, sizeof(u8));
	pos++;
	//pos += 10;
	//расчет изапись crc
	crc = calc_crc(&tempBuf[0], len - 2);
	WriteShort(&tempBuf[pos], &crc);
	pos += 2;
	//формирование основного пакета для передачи
	//Ql_memset(&__SendBuffer[0],0,LEN_BUFFER_SEND);		
	__SendBuffer[0] = 0x17; __SendBuffer[1] = 0x13;
	//замена символов
	lensend = replace(&tempBuf[0], pos, &__SendBuffer[2]);
	lensend += 2;
	//Ql_FreeMemory(tempBuf);
	//окончание формирования сообщения
	__SendBuffer[lensend] = 0x17; __SendBuffer[++lensend] = 0x5;

	return ++lensend;

}


void SetEventTime(EventData* evdata) {
	ST_Time cmn;
	Ql_GetLocalTime(&cmn);
	evdata->TimeEvent[5] = cmn.second;
	evdata->TimeEvent[4] = cmn.minute;
	evdata->TimeEvent[3] = cmn.hour;
	evdata->TimeEvent[2] = (cmn.year-2000);
	evdata->TimeEvent[1] = cmn.day;
	evdata->TimeEvent[0] = cmn.month;
	//OUTD("s:%d mi:%d h:%d d:%d mo:%d y:%d", evdata->TimeEvent[5], evdata->TimeEvent[4], evdata->TimeEvent[3], evdata->TimeEvent[1], evdata->TimeEvent[0], evdata->TimeEvent[2]);
}

int replace(unsigned char* data, int datalen, unsigned char*buffer) {
	unsigned char find = 0x17;
	u32 locpos = 0;
	u32 i;
	for (i = 0; i < datalen; i++)
	{
		if (data[i] == find)
		{
			buffer[locpos] = data[i];
			buffer[++locpos] = 0x12;
		}
		else buffer[locpos] = data[i];
		locpos++;
	}
	return locpos;
}

int unreplace(unsigned char* data, int datalen) {
	unsigned char find = 0x17;
	unsigned char find_1 = 0x12;
	u32 pos = 0;
	unsigned char buffer[512];
	for (u32 i = 0; i < datalen; i++)
	{
		if (data[i] == find && data[i + 1] == find_1)
		{
			buffer[pos] = data[i];
			i++;
		}
		else buffer[pos] = data[i];
		pos++;
	}
	ExtractPakets(&buffer[0], pos);
	return pos;
}


void ExtractPakets(unsigned char* pakets, u32 dlen) {
	u8 ch_begin = 0x17;
	u8 ch_start = 0x13;
	u8 ch_end = 0x5;
	u16 start = 0;
	u16 end = 0;
	//u16 crc,
	u16 len;
	u32 address;
	for (u32 i = 0; i < dlen; i++)
	{
		if (pakets[i] == ch_begin && pakets[i + 1] == ch_start) {
			start = i + 2;
			i++;
			continue;
		}
		if (pakets[i] == ch_begin && pakets[i + 1] == ch_end) {
			end = i - 1;
			if (calc_crc(&pakets[start], (end - start) - 1) == (pakets[end - 1] <<= 8) | pakets[end]) {//(clc== crc){
																									   //длина пакета
				len = ReverseUSort((unsigned short*)&pakets[start]);
				//адрес устройства
				address = (int)((((pakets[start + 2] << 0x10) | (pakets[start + 3] << 8)) | pakets[start + 4]));
				//OUTD("Buffer lenght:%d. Packet length:%d. Address device:%d.",dlen,len,address);
				//тип принятого пакета
				CheckTypePacket(&pakets[start + 5], len - 8);
				continue;
			}
		}
	}
}

void CheckTypePacket(unsigned char* packet, u16 len) {
	TypePackage tpack = (TypePackage)*packet;

	switch (tpack)
	{
	case Events:
		break;
	case ConfirmReceipt:
	{
		packet++;
		//OUTD("char packet NUMBER",packet[1]<<8^packet[0]);

		unsigned short pnum = ReverseUSort((unsigned short*)packet);
		if (pnum == __numPacket) {
			__countRepeat = 0;
			OUTD(">Recive confirm num:%d.", pnum);
			//LOG(">Recive confirm number:%d.", pnum);
			confirm_events();
			if (__numPacket >= __count_num_packets)
				__numPacket = 0;
			__numPacket++;
			__waitConfirm = 0;
			Ql_Timer_Stop(EST_CONFIRM_TIMER);
			Ql_Timer_Start(EST_EMPTY_TIMER,(__mdm_settings.t_send*1000),FALSE);
			_n_state = STATE_CHECK_EVENTS;
		}
		else {
			OUTD("!Diff number pack send:%d recive:%d ", __numPacket, pnum);
			//TODO Перегрузка или закрытие сокета
			__countRepeat++;

			//nSTATE=STATE_SOCKET_RECIVE;
			if (__countRepeat>2) {

				OUTD("!Diff packet more than 3.System reboot", NULL);
				Ql_Sleep(200);
				Ql_Reset(0);
			}
		}
		break;
	}
	case Text: {
		packet[len + 1] = '\0';
		OUTD(">Recive:%s", ++packet);
		ParseTextCommand((char*)packet, len);
		break;
	}
	case StateInput:
		break;
	default:
		break;
	}
}



void ParseTextCommand(char* command, unsigned char len) {
	command[len] = '\0';
	char* pc;
	char sym[3];
	int day = 0; int month = 0; int year = 0;
	int hour = 0; int minute = 0; int second = 0;
	unsigned char loctime;
	char* confing = Ql_strstr(command, "CONFIG");
	if (confing) {
		pc = Ql_strstr(confing++, "DATE");
		if (pc) {
			Ql_memset(&sym[0], 0, 3);
			pc += 5;
			Ql_memcpy(&sym[0], pc, 2);
			day = myAtoi(&sym[0]);
			pc += 3;
			Ql_memcpy(&sym[0], pc, 2);
			month = myAtoi(&sym[0]);
			pc += 3;
			Ql_memcpy(&sym[0], pc, 2);
			year = myAtoi(&sym[0]);
			pc++;

		}
		pc = Ql_strstr(confing++, "TIME");
		if (pc) {

			Ql_memset(&sym[0], 0, 3);
			pc += 5;
			Ql_memcpy(&sym[0], pc, 2);
			hour = myAtoi(&sym[0]);
			pc += 3;
			Ql_memcpy(&sym[0], pc, 2);
			minute = myAtoi(&sym[0]);
			pc += 3;
			Ql_memcpy(&sym[0], pc, 2);
			second = myAtoi(&sym[0]);
			pc++;
		}
		ST_Time tm;
		Ql_GetLocalTime(&tm);
		tm.year = (2000+year);
		tm.month = month;
		tm.day = day;
		tm.hour = hour;
		tm.minute = minute;
		tm.second = second;
		loctime = Ql_SetLocalTime(&tm);
		OUTD(">Set local time:%d-%d-%d %d:%d:%d result:%d", year, month, day, hour, minute, second, loctime);
		//LOG(">Set local time:%d-%d-%d %d:%d:%d result:%d", year, month, day, hour, minute, second, loctime);
	}
	pc = Ql_strstr(command, "OUT");
	if (pc) {
		pc += 5;
		if (Ql_isdigit(*pc) != 0) {
			u8 val = (u8)Ql_atoi(pc);
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
	
}

int myAtoi(char *str)
{
	int res = 0; // Initialize result
				 // Iterate through all characters of input string and update result
	for (int i = 0; str[i] != '\0'; ++i)
		res = res * 10 + str[i] - '0';
	return res;
}


void GetOutDescripon(unsigned char pin, char level, unsigned char* state) {
	switch (pin) {
	case 0:
		*state = level == 1 ? DO1ExecOn : DO1ExecOFF;
		break;
	case 1:
		*state = level == 1 ? DO2ExecOn : DO2ExecOFF;
		break;
	case 2:
		*state = level == 1 ? DO3ExecOn : DO3ExecOFF;
		break;
	case 3:
		*state = level == 1 ? DO4ExecOn : DO4ExecOFF;
		break;
	case 4:
		*state = level == 1 ? DO5ExecOn : DO5ExecOFF;
		break;
	case 5:
		*state = level == 1 ? DO6ExecOn : DO6ExecOFF;
		break;
	case 6:
		*state = level == 1 ? DO7ExecOn : DO7ExecOFF;
		break;
	case 7:
		*state = level == 1 ? DO8ExecOn : DO8ExecOFF;
		break;
	}
}

void GetAnalogDescripon(unsigned char pin, unsigned char* state) {
	switch (pin) {
	case 0:
		*state = AIHigh1;
		break;
	case 1:
		*state = AIHigh2;
		break;
	case 2:
		*state = AIHigh3;
		break;
	case 3:
		*state = AIHigh4;
		break;
	}
}

void GetInputDescripon(unsigned char pin, char level, unsigned char* state) {
	switch (pin) {
	case 0:
		*state = level == 1 ? DILowToHigh1 : DIHighToLow1;
		break;
	case 1:
		*state = level == 1 ? DILowToHigh2 : DIHighToLow2;
		break;
	case 2:
		*state = level == 1 ? DILowToHigh3 : DIHighToLow3;
		break;
	case 3:
		*state = level == 1 ? DILowToHigh4 : DIHighToLow4;
		break;
	case 4:
		*state = level == 1 ? DILowToHigh5 : DIHighToLow5;
		break;
	case 5:
		*state = level == 1 ? DILowToHigh6 : DIHighToLow6;
		break;
	case 6:
		*state = level == 1 ? DILowToHigh7 : DIHighToLow7;
		break;
	case 7:
		*state = level == 1 ? DILowToHigh8 : DIHighToLow8;
		break;
	case 8:
		*state = level == 1 ? DILowToHigh9 : DIHighToLow9;
		break;
	case 9:
		*state = level == 1 ? DILowToHigh10 : DIHighToLow10;
		break;
	case 10:
		*state = level == 1 ? DILowToHigh11 : DIHighToLow11;
		break;
	case 11:
		*state = level == 1 ? DILowToHigh12 : DIHighToLow12;
		break;
	case 12:
		*state = level == 1 ? DILowToHigh13 : DIHighToLow13;
		break;
	case 13:
		*state = level == 1 ? DILowToHigh14 : DIHighToLow14;
		break;
	case 14:
		*state = level == 1 ? DILowToHigh15 : DIHighToLow15;
		break;
	case 15:
		*state = level == 1 ? DILowToHigh16 : DIHighToLow16;
		break;
	case 16:
		*state = level == 1 ? PowerOff : PowerOn;
	}
}





