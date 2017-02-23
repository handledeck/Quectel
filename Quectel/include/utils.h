#ifndef __UTILS_H__
#define __UTILS_H__

//#define BUFFER_COMMAND_LEN 100

//extern char __buf_command[];
//extern u8 __buf_command_pos;


void remove_char(char *str, char garbage);
char* strtok_r(char *s, const char *delim, char **last);
char * strtok(char *s, const char *delim);
void parce_command(void);
void fill_command_buffer(char* ctxt);
void get_menu_console(void);
void text_bool(bool value, char* outtext);
void  data_to_state_invert(u8* buffer, u32 value, u8 count);
//void WriteSettings(void);
//void ReadSettings(void);
//void PrintSettings(void);
//void SetDefaultSettins(void);
void print_settings(void);
void set_command(char* command);
void  data_to_state(u8* buffer, u32 value, u8 count);
u32 is_valid_ip(const u8 *ip_str,u8 ip[4]);
u32 is_valid_datetime(const u8 *ip_str, u32 dt[6]);
void get_config_form();
void set_config_form(char* config);
void set_state_est(STATE_TCP state);
bool backspace_buffer();
s32 Ql_abs(s32 a);
bool check_pwd();
#endif 

