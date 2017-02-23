#include <qlobal.h>

u16 __confirmRecive = 0;
EventData __bufEventData[__COUNT_EVENTS__];



type_settings __mdm_settings = {
	//apn	
	"vpn2.mts.by",
	//apn_user
	"vpn",
	//apn_password
	"gsd9drekj5",
	//debug
	TRUE,
	//t_send
	50,
	//ip address connect to est
	{172,23,0,9},
	//tcp port connect to est
	10245,
	//deboung
	500,
	//log
	"FALSE",
	//w_user
	"admin",
	//w_password
	"admin",
	//num
	1,
	//using est tools for check
	FALSE,
	//latitude
	55.1910967,
	//longitude
	30.1253259,
	//time zone
	3,
	//lights state
	FALSE,
	//active door channels default first 5
	0xFFE0,
	//active channels discret defilt 16
	0xFFFF,
	//active out discret channels 8
	0xFF,
	//active analog chanels 4
	0x3,
	//deadband
	1,
	//schedulers
	"AA==",
	FALSE,
	"9600,8,0,1"
};

void save_all_settings() {
	write_mdm_settings();
	write_sam_settings();
	read_mdm_settings();
}

void write_sam_settings() {
	u8 buf[27];
	Ql_memset(buf, 0, 27);
	buf[0] = 7;
	Ql_memcpy(&buf[1], &__mdm_settings.deboung, sizeof(u16));
	Ql_memcpy(&buf[3], &__mdm_settings.act_door_chanals, sizeof(u16));
	Ql_memcpy(&buf[5], &__mdm_settings.dinn_active, sizeof(u16));
	Ql_memcpy(&buf[7], &__mdm_settings.dout_active, sizeof(u8));
	Ql_memcpy(&buf[8], &__mdm_settings.ainn_active, sizeof(u8));
	Ql_memcpy(&buf[9], &__mdm_settings.deadband, sizeof(u8));
	Ql_memcpy(&buf[10], &__mdm_settings.latidute, sizeof(float));
	Ql_memcpy(&buf[14], &__mdm_settings.longitude, sizeof(float));
	Ql_memcpy(&buf[18], &__mdm_settings.timezone, sizeof(u8));
	if (__mdm_settings.use_sched == FALSE)
		buf[19] = 0;
	else
		buf[19] = 1;
		u8 ind = 0;
		u8* pchar = strtok(&__mdm_settings.sport, ",");
		while (pchar != NULL)
		{
			if (ind == 0)
			{
				u32 boud = (u32)Ql_atoi(pchar);
				Ql_memcpy(&buf[20], &boud, sizeof(u32));
			}
			if (ind == 1) {
				u8 bits = (u8)Ql_atoi(pchar);
				buf[24] = bits;
			}
			if (ind == 2) {
				u8 parity = (u8)Ql_atoi(pchar);
				buf[25] = parity;
			}
			if (ind == 3) {
				u8 sbit = (u8)Ql_atoi(pchar);
				buf[26] = sbit;
			}
			pchar = strtok(NULL, ",");
			ind++;
		}
	mdm_msg_send(buf, 27);
}

void write_mdm_settings() {
	s32 handle;
	u32 writed;

	Ql_FS_Delete(__FILE_SETTINGS__);
	handle = Ql_FS_Open(__FILE_SETTINGS__, QL_FS_CREATE);
	if (handle>0) {
		//OUTD("open file for write settings OK\r\n",NULL);
		Ql_FS_Write(handle, (u8*)&__mdm_settings, sizeof(type_settings), &writed);
		//OUTD("result write settings:%d\r\n",result);
		Ql_FS_Close(handle);
		//OUTD("Try APN:%s Login:%s Pass:%s\r\n",sf->APN,sf->User,sf->Pass);
		/*Ql_DebugTrace("APN=%s IP=%d.%d.%d.%d TCP=%d USER=%s PASS=%s DEBUG=%d NUM=%d PWD=%s\r\n",
		sf->APN,sf->IP[0],sf->IP[1],sf->IP[2],sf->IP[3],
		sf->TCP,sf->User,sf->Pass,sf->Debug,sf->Num,sf->Pwd);*/

	}
	else OUTD("!Write settings error:%d", handle);
}
void read_mdm_settings() {
	s32 handle;
	u32 readed;
	//Ql_FileGetSize(FILE_SETTINGS,&readed);
	//OUTD("try open file %s.File size:%d\r\n",FILE_SETTINGS,readed);
	handle = Ql_FS_Open(__FILE_SETTINGS__, QL_FS_READ_ONLY);
	if (handle>0) {
		//OUTD("open file for read settings OK\r\n",NULL);
		Ql_memset(&__mdm_settings, 0, sizeof(type_settings));
		Ql_FS_Read(handle, (u8*)&__mdm_settings, readed, &readed);
		//OUTD("result read settings:%d\r\n",result);
		Ql_FS_Close(handle);
		/*Ql_DebugTrace("APN=%s IP=%d.%d.%d.%d TCP=%d USER=%s PASS=%s DEBUG=%d NUM=%d PWD=%s\r\n",
		sf->APN,sf->IP[0],sf->IP[1],sf->IP[2],sf->IP[3],
		sf->TCP,sf->User,sf->Pass,sf->Debug,sf->Num,sf->Pwd);*/
	}
	else {
		//OUTD("!Setting file not found. Create new default file",NULL);
		write_mdm_settings();
	}
}

void check_event_file() {
	if (Ql_FS_Check(__FILE_EVENTS__))
	{
		s32 tr;
		//OUTD(">event file not found.create new");
		s32 handle = Ql_FS_Open(__FILE_EVENTS__, QL_FS_CREATE);
		if (handle <= 0)
			OUTD("error create file:%d", handle);
		Ql_FS_Flush(handle);
		Ql_FS_Close(handle);
		s32 size=Ql_FS_GetSize(__FILE_EVENTS__);
		//OUTD("event file size:%d", size);
	}
	else
	{
		s32 size = Ql_FS_GetSize(__FILE_EVENTS__);
		OUTD("event file is present size:%d", size);
	}
	
}

s8 read_events(void) {
	u32 filesize = 0;
	u32 readed = 0;
	s32 handle;
	filesize=Ql_FS_GetSize(__FILE_EVENTS__);
	// if (error!=0) {
	//   OUTD("operation get file size:%d var size:%d", error, filesize); 
	// }
	__confirmRecive = filesize / sizeof(EventData);
	//OUTD("Count to send message is:%d\r\n",__confirmRecive);
	if (__confirmRecive>0) {
		handle = Ql_FS_Open(__FILE_EVENTS__, (QL_FS_READ_WRITE));
		if (handle>0) {
			Ql_FS_Read(handle, (u8*)&__bufEventData[0], filesize, &readed);
			Ql_FS_Close(handle);
			__toSend = BuildEventMessage(1, &__bufEventData[0], __confirmRecive);
		}
		else {
			//OUTD("File not present:%d", handle);
			__toSend = 0;
		}
	}
	else {
		__toSend = 0;
	}
	return NULL;
}

void confirm_events() {
	s32 handle;
	u32 filesize = 0;
	u32 readed = 0;
	u32 poss = 0;
	//определяем размер файла хранилища событий 
	filesize=Ql_FS_GetSize(__FILE_EVENTS__);
	// if(error!=0)
	//   OUTD("operation get size:%d filesize:%d\r\n",filesize,error);
	if (filesize == 0) {
		//OUTD("File empty.Nothing confirm",NULL);
		return;
	}
	//открыв. файл
	handle = Ql_FS_Open(__FILE_EVENTS__, (QL_FS_READ_WRITE));
	if (handle>0) {

		poss = __confirmRecive*sizeof(EventData);
		//OUTD("concat from position:%d\r\n", poss); 
		Ql_FS_Seek(handle, poss, QL_FS_FILE_BEGIN);
		//OUTD("read from position:%d\r\n",filesize-poss);
		Ql_FS_Read(handle, (u8*)&__bufEventData[0], filesize - poss, &readed);
		//OUTD("read from file:%d\r\n",readed);
		//Ql_FileTruncate(handle);
		Ql_FS_Close(handle);
		Ql_FS_Delete(__FILE_EVENTS__);
		//Ql_FileGetSize(__file_events,&filesize);
		//Ql_DebugTrace("close file.Size:%d\r\n",filesize);
		handle = Ql_FS_Open(__FILE_EVENTS__, (QL_FS_READ_WRITE | QL_FS_CREATE));
		if (handle>0) {
			//OUTD("reopen file\r\n",NULL);
			Ql_FS_Seek(handle, 0, QL_FS_FILE_BEGIN);
			Ql_FS_Write(handle, (u8*)&__bufEventData[0], filesize - poss, &readed);
			Ql_FS_Close(handle);
		}
	}
	else
		OUTD("!Can't open file error:%d", handle);
}


void write_events(void* evdata) {
	s32 handle;
	u32 filesize = 0;
	u32 readed = 0;
	EventData* edata = (EventData*)evdata;
	//определяем размер файла хранилища событий
	filesize = Ql_FS_GetSize(__FILE_EVENTS__);
	handle = Ql_FS_Open(__FILE_EVENTS__, QL_FS_READ_WRITE);
	
	//OUTD("fsize:%d", filesize);
	if (handle>0) {
		if (__confirmRecive>0) {
			__confirmRecive--;
		}
		if (filesize >= sizeof(EventData) * __COUNT_EVENTS__) {
			Ql_FS_Seek(handle, sizeof(EventData), QL_FS_FILE_BEGIN);
			Ql_FS_Read(handle, (u8*)&__bufEventData[0], (__COUNT_EVENTS__ - 1)*sizeof(EventData), &readed);
			Ql_FS_Seek(handle, 0, QL_FS_FILE_BEGIN);
			Ql_FS_Write(handle, (u8*)&__bufEventData[0], (__COUNT_EVENTS__ - 1)*sizeof(EventData), &readed);
			//OUTD("write to file:%d\r\n",readed);
			Ql_FS_Write(handle, (u8*)evdata, sizeof(EventData), &readed);
		}
		else {

			Ql_FS_Seek(handle, filesize, QL_FS_FILE_BEGIN);
			Ql_FS_Write(handle, (u8*)&edata[0], sizeof(EventData), &readed);
			//Ql_FS_Close(handle);
		}
		Ql_FS_Close(handle);
	}
	else
		OUTD("!Can't open file error:%d", handle);
}


void delete_mdm_settings() {
	s32 ret=Ql_FS_Delete(__FILE_SETTINGS__);
	ret = Ql_FS_Delete(__FILE_EVENTS__);
	if (ret == QL_RET_OK) {
		OUTD("File settings and events delete", NULL);
	}
	else
		OUTD("!Error delete file events and settings:%d", ret);
}