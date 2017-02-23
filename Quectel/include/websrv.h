#ifndef __WEBSRV_H__
#define __WEBSRV_H__

#include <qlobal.h>

extern char* __error_page;
extern char* __ok_page;
extern char* __main_page;
extern char* __download_pagmkae;




typedef enum {
	GET,
	POST
}Method;

typedef struct {
	Method method;
	u32  length;
	char action[100];
	//char boundary[200];
	char cookie[200];
	//unsigned char data[10 * 512];
	bool authorization;
	bool set_cookie;
}HttpHeader;

#define HTTP_DATA_LEN	4096


void get_main_page(const char* message,char* buffer);
void get_error_page(void);
void get_rebot_page(char* buffer);
void page_response(HttpHeader *http_header);
bool SetLocDate(char* line);
bool parse_header(u8* header, HttpHeader *http_header);
bool vilidation(HttpHeader *http_header);
s32 decode_b64(u8 *buftodec, u8 *decbuf);
s32 decode_b64_bin(const unsigned char *buftodec, u8 size, unsigned char *decbuf);
#endif 

