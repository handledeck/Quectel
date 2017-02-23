#ifndef __EVENTS_H__
#define __EVENTS_H__

#include "qlobal.h"

typedef enum {
//[Description("��� �������")]
None = 0,
//[Description("���� 1, ������� H-L")]
DIHighToLow1 = 1,
//[Description("���� 2, ������� H-L")]
DIHighToLow2 = 3,
//[Description("���� 3, ������� H-L")]
DIHighToLow3 = 5,
//[Description("���� 4, ������� H-L")]
DIHighToLow4 = 7,
//[Description("���� 5, ������� H-L")]
DIHighToLow5 = 9,
//[Description("���� 6, ������� H-L")]
DIHighToLow6 = 11,
//[Description("���� 7, ������� H-L")]
DIHighToLow7 = 13,
//[Description("���� 8, ������� H-L")]
DIHighToLow8 = 15,
//[Description("���� 9, ������� H-L")]
DIHighToLow9 = 71,
//[Description("���� 10, ������� H-L")]
DIHighToLow10 = 73,
//[Description("���� 11, ������� H-L")]
DIHighToLow11 = 75,
//[Description("���� 12, ������� H-L")]
DIHighToLow12 = 77,
//[Description("���� 13, ������� H-L")]
DIHighToLow13 = 79,
//[Description("���� 14, ������� H-L")]
DIHighToLow14 = 81,
//[Description("���� 15, ������� H-L")]
DIHighToLow15 = 83,
//[Description("���� 16, ������� H-L")]
DIHighToLow16 = 85,
//[Description("���� 1, ������� L-H")]
DILowToHigh1 = 2,
//[Description("���� 2, ������� L-H")]
DILowToHigh2 = 4,
//[Description("���� 3, ������� L-H")]
DILowToHigh3 = 6,
//[Description("���� 4, ������� L-H")]
DILowToHigh4 = 8,
//[Description("���� 5, ������� L-H")]
DILowToHigh5 = 10,
//[Description("���� 6, ������� L-H")]
DILowToHigh6 = 12,
//[Description("���� 7, ������� L-H")]
DILowToHigh7 = 14,
//[Description("���� 8, ������� L-H")]
DILowToHigh8 = 16,
//[Description("���� 9, ������� L-H")]
DILowToHigh9 = 70,
//[Description("���� 10, ������� L-H")]
DILowToHigh10 = 72,
//[Description("���� 11, ������� L-H")]
DILowToHigh11 = 74,
//[Description("���� 12, ������� L-H")]
DILowToHigh12 = 76,
//[Description("���� 13, ������� L-H")]
DILowToHigh13 = 78,
//[Description("���� 14, ������� L-H")]
DILowToHigh14 = 80,
//[Description("���� 15, ������� L-H")]
DILowToHigh15 = 82,
//[Description("���� 16, ������� L-H")]
DILowToHigh16 = 84,


//��
//[Description("��������� ������� ���������� �� ������ 1")]
DO1Exec = 62,
//[Description("��������� ������� ���������� �� ������ 2")]
DO2Exec = 63,
//[Description("��������� ������� ���������� �� ������ 3")]
DO3Exec = 64,
//[Description("��������� ������� ���������� �� ������ 4")]
DO4Exec = 65,
//[Description("��������� ������� ���������� �� ������ 5")]
DO5Exec = 66,
//[Description("��������� ������� ���������� �� ������ 6")]
DO6Exec = 67,
//[Description("��������� ������� ���������� �� ������ 7")]
DO7Exec = 68,
//[Description("��������� ������� ���������� �� ������ 8")]
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



//�������
//[Description("���������� ���� 1, �����")]
AINorm1 = 50,
//[Description("���������� ���� 2, �����")]
AINorm2 = 51,
//[Description("���������� ���� 3, �����")]
AINorm3 = 52,
//[Description("���������� ���� 4, �����")]
AINorm4 = 53,
//[Description("���������� ���� 1 ���� �����")]
AIHigh1 = 54,
//[Description("���������� ���� 2 ���� �����")]
AIHigh2 = 55,
//[Description("���������� ���� 3 ���� �����")]
AIHigh3 = 56,
//[Description("���������� ���� 4 ���� �����")]
AIHigh4 = 57,
//[Description("���������� ���� 1 ���� �����")]
AILow1 = 58,
//[Description("���������� ���� 2 ���� �����")]
AILow2 = 59,
//[Description("���������� ���� 3 ���� �����")]
AILow3 = 60,
//[Description("���������� ���� 4 ���� �����")]
AILow4 = 61,
//[Description("������� ������� ��������")]
PowerOn = 22,
//[Description("������� ������� ���������")]
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
//������� ���������� ������ ���� ������ 0 �������� ��������
extern u32 __toSend;
//����� �������� ������������� ������
extern u16 __numPacket;
//������� �������.���������� ������� ������� ������
extern s32 __timeOutEvents;
extern u32 __count_num_packets;

typedef enum {
	//����� ������� �������
	Events = 0,
	//����� ������������� ������
	ConfirmReceipt = 1,
	//��������� �����
	Text = 2,
	//����� ��������� �������
	StateInput = 3,

}TypePackage;

typedef struct {
	//��� �������
	unsigned char	TypeEvent;
	//�������� ���
	u16	EventDesc;
	//����� �������
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

//���������� �������� �������
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


