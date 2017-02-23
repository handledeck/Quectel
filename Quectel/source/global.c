#include <ql_type.h>
#include <ql_time.h>
#include <qlobal.h>
#include <ql_stdlib.h>



void parse_cfun_state(u8 state, char* out_state) {
	switch (state)
	{
	case 0:
		Ql_strcpy(out_state, "CFUN_STATE_0");
		break;
	case 1:
		Ql_strcpy(out_state, "CFUN_STATE_1");
		break;
	case 2:
		Ql_strcpy(out_state, "CFUN_STATE_4");
		break;
	}
}

void parse_system_state(u8 state, char* out_state) {
	switch (state)
	{
	case 0:
		Ql_strcpy(out_state, "SYS_STATE_START");
		break;
	case 1:
		Ql_strcpy(out_state, "SYS_STATE_ATOK");
		break;
	case 2:
		Ql_strcpy(out_state, "SYS_STATE_PHBOK");
		break;
	case 3:
		Ql_strcpy(out_state, "SYS_STATE_SMSOK");
		break;
	}
}

void parse_sim_state(u8 state, char* out_state) {
	switch (state)
	{
	case 0:
		Ql_strcpy(out_state, "SIM_STAT_NOT_INSERTED");
		break;
	case 1:
		Ql_strcpy(out_state, "SIM_STAT_READY");
		break;
	case 2:
		Ql_strcpy(out_state, "SIM_STAT_PIN_REQ");
		break;
	case 3:
		Ql_strcpy(out_state, "SIM_STAT_PUK_REQ");
		break;
	case 4:
		Ql_strcpy(out_state, "SIM_STAT_PH_PIN_REQ");
		break;
	case 5:
		Ql_strcpy(out_state, "SIM_STAT_PH_PUK_REQ");
		break;
	case 6:
		Ql_strcpy(out_state, "SIM_STAT_PIN2_REQ");
		break;
	case 7:
		Ql_strcpy(out_state, "SIM_STAT_PUK2_REQ");
		break;
	case 8:
		Ql_strcpy(out_state, "SIM_STAT_BUSY");
		break;
	case 9:
		Ql_strcpy(out_state, "SIM_STAT_NOT_READY");
		break;
	case 10:
		Ql_strcpy(out_state, "SIM_STAT_NOT_INSERTED");
		break;
	default:
		Ql_strcpy(out_state, "SIM_STAT_UNSPECIFIED");
		break;
	}
}


bool parse_network_state(u8 state, char* out_state) {
	bool result = FALSE;
	switch (state)
	{
	case 0:
		Ql_strcpy(out_state, "NW_STAT_NOT_REGISTERED");
		break;
	case 1:
		Ql_strcpy(out_state, "NW_STAT_REGISTERED");
		result = TRUE;
		break;
	case 2:
		Ql_strcpy(out_state, "NW_STAT_SEARCHING");
		break;
	case 3:
		Ql_strcpy(out_state, "NW_STAT_REG_DENIED");
		break;
	case 4:
		Ql_strcpy(out_state, "NW_STAT_UNKNOWN");
		break;
	case 5:
		Ql_strcpy(out_state, "NW_STAT_REGISTERED_ROAMING");
		break;
	}
	return result;
}