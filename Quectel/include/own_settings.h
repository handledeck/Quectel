#ifndef __OWN_SETTINGS_H__
#define __OWN_SETTINGS_H__

#include <qlobal.h>

#define __FILE_SETTINGS__		"mdm_settings.dat"
#define __FILE_EVENTS__			"mdm_events.dat"
#define __COUNT_EVENTS__			30

typedef struct {
	char	apn[30];
	char	apn_user[30];
	char	apn_password[30];
	bool	debug;
	u8		t_send;
	u8		ip[4];
	u16		tcp;
	u16		deboung;
	char	log[6];
	char	w_user[30];
	char	w_password[30];
	u8		num;
	bool	use_esttools;
	float	latidute;
	float	longitude;
	u8		timezone;
	bool	lights;
	u16		act_door_chanals;
	u16		dinn_active;
	u8		dout_active;
	u8		ainn_active;
	u8		deadband;
	u8		schedulers[300];
	bool	use_sched;
	u8		sport[13];
}type_settings;



extern type_settings __mdm_settings;

void write_mdm_settings(void);
void read_mdm_settings(void);
void check_event_file(void);
void delete_mdm_settings(void);
s8 read_events(void);
void write_events(void* evdata);
void confirm_events(void);
void write_sam_settings(void);
void save_all_settings(void);
#endif // !__OWN_SETTINGS_H__

