#ifndef __EVENTS_H__
#define __EVENTS_H__

#include "qlobal.h"

typedef enum {
//[Description("Нет событий")]
None = 0,
//[Description("Вход 1, переход H-L")]
DIHighToLow1 = 1,
//[Description("Вход 2, переход H-L")]
DIHighToLow2 = 3,
//[Description("Вход 3, переход H-L")]
DIHighToLow3 = 5,
//[Description("Вход 4, переход H-L")]
DIHighToLow4 = 7,
//[Description("Вход 5, переход H-L")]
DIHighToLow5 = 9,
//[Description("Вход 6, переход H-L")]
DIHighToLow6 = 11,
//[Description("Вход 7, переход H-L")]
DIHighToLow7 = 13,
//[Description("Вход 8, переход H-L")]
DIHighToLow8 = 15,
//[Description("Вход 9, переход H-L")]
DIHighToLow9 = 71,
//[Description("Вход 10, переход H-L")]
DIHighToLow10 = 73,
//[Description("Вход 11, переход H-L")]
DIHighToLow11 = 75,
//[Description("Вход 12, переход H-L")]
DIHighToLow12 = 77,
//[Description("Вход 13, переход H-L")]
DIHighToLow13 = 79,
//[Description("Вход 14, переход H-L")]
DIHighToLow14 = 81,
//[Description("Вход 15, переход H-L")]
DIHighToLow15 = 83,
//[Description("Вход 16, переход H-L")]
DIHighToLow16 = 85,
//[Description("Вход 1, переход L-H")]
DILowToHigh1 = 2,
//[Description("Вход 2, переход L-H")]
DILowToHigh2 = 4,
//[Description("Вход 3, переход L-H")]
DILowToHigh3 = 6,
//[Description("Вход 4, переход L-H")]
DILowToHigh4 = 8,
//[Description("Вход 5, переход L-H")]
DILowToHigh5 = 10,
//[Description("Вход 6, переход L-H")]
DILowToHigh6 = 12,
//[Description("Вход 7, переход L-H")]
DILowToHigh7 = 14,
//[Description("Вход 8, переход L-H")]
DILowToHigh8 = 16,
//[Description("Вход 9, переход L-H")]
DILowToHigh9 = 70,
//[Description("Вход 10, переход L-H")]
DILowToHigh10 = 72,
//[Description("Вход 11, переход L-H")]
DILowToHigh11 = 74,
//[Description("Вход 12, переход L-H")]
DILowToHigh12 = 76,
//[Description("Вход 13, переход L-H")]
DILowToHigh13 = 78,
//[Description("Вход 14, переход L-H")]
DILowToHigh14 = 80,
//[Description("Вход 15, переход L-H")]
DILowToHigh15 = 82,
//[Description("Вход 16, переход L-H")]
DILowToHigh16 = 84,


//ТУ
//[Description("Выполнена команда управления по выходу 1")]
DO1Exec = 62,
//[Description("Выполнена команда управления по выходу 2")]
DO2Exec = 63,
//[Description("Выполнена команда управления по выходу 3")]
DO3Exec = 64,
//[Description("Выполнена команда управления по выходу 4")]
DO4Exec = 65,
//[Description("Выполнена команда управления по выходу 5")]
DO5Exec = 66,
//[Description("Выполнена команда управления по выходу 6")]
DO6Exec = 67,
//[Description("Выполнена команда управления по выходу 7")]
DO7Exec = 68,
//[Description("Выполнена команда управления по выходу 8")]
DO8Exec = 69,

DO1ExecOn = 86,
DO1ExecOFF = 87,
DO2ExecOn = 88,
DO2ExecOFF = 89,
DO3ExecOn = 90,
DO3ExecOFF = 91,
DO4ExecOn = 92,
DO4ExecOFF = 93,
DO5ExecOn = 94,
DO5ExecOFF = 95,
DO6ExecOn = 96,
DO6ExecOFF = 97,
DO7ExecOn = 99,
DO7ExecOFF = 100,
DO8ExecOn = 101,
DO8ExecOFF = 102,



//Аналоги
//[Description("Аналоговый вход 1, норма")]
AINorm1 = 50,
//[Description("Аналоговый вход 2, норма")]
AINorm2 = 51,
//[Description("Аналоговый вход 3, норма")]
AINorm3 = 52,
//[Description("Аналоговый вход 4, норма")]
AINorm4 = 53,
//[Description("Аналоговый вход 1 выше нормы")]
AIHigh1 = 54,
//[Description("Аналоговый вход 2 выше нормы")]
AIHigh2 = 55,
//[Description("Аналоговый вход 3 выше нормы")]
AIHigh3 = 56,
//[Description("Аналоговый вход 4 выше нормы")]
AIHigh4 = 57,
//[Description("Аналоговый вход 1 выше нормы")]
AILow1 = 58,
//[Description("Аналоговый вход 2 выше нормы")]
AILow2 = 59,
//[Description("Аналоговый вход 3 выше нормы")]
AILow3 = 60,
//[Description("Аналоговый вход 4 выше нормы")]
AILow4 = 61,
//[Description("Сетевое питание включено")]
PowerOn = 22,
//[Description("Сетевое питание выключено")]
PowerOff = 23,

}TypeEvent;

#define LEN_EVENT 3
#define LEN_EVENT_DATA 9
#define LEN_WITHOUT_DATA 8
#define LEN_BUFFER_SEND 8192
#define EVENT_SEND_TIMEOUT 60000;
#define LEN_INPUT_STATE 11;
#define LEN_TEMP_BUFFER 18;
#define LEN_INPUT_STATE_PACKET LEN_WITHOUT_DATA + LEN_INPUT_STATE

extern unsigned char __SendBuffer[];
//признак наполнения буфера если больше 0 начинаем передачу
extern u32 __toSend;
//номер текущего передаваемого пакета
extern u16 __numPacket;
//таймаут событий.необходима посылка пустого пакета
extern s32 __timeOutEvents;
extern u32 __count_num_packets;

typedef enum {
	//Пакет событий прибора
	Events = 0,
	//Пакет подтверждения приема
	ConfirmReceipt = 1,
	//Текстовый пакет
	Text = 2,
	//Пакет состояния выходов
	StateInput = 3,

}TypePackage;

typedef struct {
	//тип события
	unsigned char	TypeEvent;
	//описание его
	u16	EventDesc;
	//время события
	unsigned char	TimeEvent[6];
}EventData;

typedef struct {
	unsigned char din;
	unsigned short dout_1;
	unsigned short dout_2;
	unsigned short dout_3;
	unsigned short dout_4;
	unsigned char guard_tape;
}StateInputData;

//переменная текущего события
extern EventData __currentEvData;
extern StateInputData __inputState;
extern u16 __countMsgSend;
extern u8 __countRepeat;

unsigned short ReverseUSort(unsigned short* value);
u32 ReverseUInt(u32* value);
void WriteShort(unsigned char* data, unsigned short* value);
void WriteAddress(unsigned char* data, unsigned int* value);
int BuildEventMessage(unsigned char PowerState, EventData* events, unsigned short len_evt);
void BuildEvent(unsigned char* data, unsigned short* pos, EventData* events, unsigned short count);
//void BuildInput(unsigned char* data, unsigned short* pos);
int BuildInputStateMessage(void);
void TrigerEvent(TypeEvent typeEvent, unsigned short description, EventData* eventData);
void SetEventTime(EventData* evdata);
int replace(unsigned char* data, int datalen, unsigned char*buffer);
int unreplace(unsigned char* data, int datalen);
void ExtractPakets(unsigned char* pakets, u32 dlen);
void CheckTypePacket(unsigned char* packet, u16 len);
void ParseTextCommand(char* command, unsigned char len);
int myAtoi(char *str);
u16 calc_crc(u8 *buf, u16 len);
void GetInputDescripon(unsigned char pin, char level, unsigned char* state);
void GetOutDescripon(unsigned char pin, char level, unsigned char* state);
void GetAnalogDescripon(unsigned char pin, unsigned char* state);

#endif


