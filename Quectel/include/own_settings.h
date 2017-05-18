#ifndef __OWN_SETTINGS_H__
#define __OWN_SETTINGS_H__

#include <qlobal.h>

#define __FILE_SETTINGS__		"mdm_settings.dat"
#define __FILE_EVENTS__			"mdm_events.dat"
#define __FILE_SAM_UPDATE__			"mdm_sam_update.dat"
#define __FILE_MDM_UPDATE__			"mdm_mdm_update.dat"
#define __COUNT_EVENTS__			30

typedef struct {
	char	apn[30];
	char	apn_user[30];
	char	apn_password[30];
	bool	debug;
	u8		t_send;
	u8		ip[4];
	u16		tcp;
	u16		deboung;			//100
	char	log[6];				//106
	char	w_user[30];			//136
	char	w_password[30];		//166
	u8		num;				//167
	bool	use_esttools;		//168
	float	latidute;			//172
	float	longitude;			//176
	u8		timezone;			//177
	bool	lights;				//178
	u16		act_door_chanals;	//180
	u16		dinn_active;		//182
	u8		dout_active;		
	u8		ainn_active;
	u8		deadband;			//185
	u8		schedulers[300];	//485
	bool	use_sched;			//486
	u8		sport[13];			//499
	bool	update;				//500
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

