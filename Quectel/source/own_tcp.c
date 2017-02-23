#include <qlobal.h>



STATE_TCP _n_state = STATE_REGISTER;
STATE_TCP _n_current = STATE_NONE;
static s32 ret;
static ST_GprsConfig  m_gprsCfg;
char __STR_LOC_IP_ADDR[16];

ST_PDPContxt_Callback  callback_tcp_functions =
{
	callback_tcp_actived,
	callBack_tcp_deactived
};

void tcp_main_function(void) {
	switch (_n_state)
	{
	case STATE_REGISTER: {
		ret = Ql_GPRS_Register(0, &callback_tcp_functions, NULL);
		if (GPRS_PDP_SUCCESS == ret)
		{
			OUTNET(">Register GPRS callback function successfully.", NULL);
			_n_state = STATE_CONFIG;
		}
		else
			OUTD("!Register GPRS callback function failure,ret=%d.", ret);
		break;
	}
	case STATE_CONFIG: {
		Ql_strcpy(m_gprsCfg.apnName, __mdm_settings.apn);
		Ql_strcpy(m_gprsCfg.apnUserId, __mdm_settings.apn_user);
		Ql_strcpy(m_gprsCfg.apnPasswd, __mdm_settings.apn_password);
		m_gprsCfg.authtype = 0;
		ret = Ql_GPRS_Config(0, &m_gprsCfg);
		if (GPRS_PDP_SUCCESS == ret)
		{
			OUTNET(">Configure GPRS param successfully",NULL);
		}
		else
		{
			OUTNET("!Configure GPRS param failure,return=%d.", ret);
		}
		_n_state = STATE_ACTIVATE;
		break;
	}
	case STATE_ACTIVATE:
	{
		_n_state = STATE_ACTIVATING;
		ret = Ql_GPRS_Activate(0);
		if (ret == GPRS_PDP_SUCCESS)
		{
			OUTNET(">Activate GPRS successfully.",NULL);
			_n_state = STATE_LOCALIP;
		}
		else if (ret == GPRS_PDP_WOULDBLOCK)
		{
			OUTNET(">Waiting for the result of GPRS activated.,ret=%d.", ret);
			//waiting Callback_GPRS_Actived
		}
		else if (ret == GPRS_PDP_ALREADY)
		{
			OUTNET("!GPRS has already been activated,ret=%d.", ret);
			_n_state = STATE_LOCALIP;
		}
		else//error
		{
			OUTNET("!Activate GPRS failure,ret=%d.", ret);
			_n_state = STATE_ACTIVATE;
		}
		break;
	}
	case STATE_LOCALIP:
	{
		u8 ip_addr[5];
		Ql_memset(ip_addr, 0, 5);
		Ql_memset(__STR_LOC_IP_ADDR, 0, 16);
		ret = Ql_GPRS_GetLocalIPAddress(0, (u32 *)ip_addr);
		if (ret == GPRS_PDP_SUCCESS)
		{
			Ql_sprintf(__STR_LOC_IP_ADDR, "%d.%d.%d.%d", ip_addr[0], ip_addr[1], ip_addr[2], ip_addr[3]);
			OUTNET(">Get Local Ip successfully,Local Ip=%s",__STR_LOC_IP_ADDR);
			if (regiser_socket_th()) {
				if (__mdm_settings.use_esttools==TRUE) {
					_n_state = STATE_EST_CONNECT;
				}
				else {
					_n_state = STATE_NONE;
				}
			}
			else
			{
				Ql_Reset(0);
			}
		}
		else
		{
			OUTNET("!Get Local Ip failure,ret=%d.", ret);
		}
		break;
	}
	case STATE_EST_CONNECT: {
		if (__mdm_settings.use_esttools==TRUE) {
			est_socket_connect(-1, NULL);
		}
		_n_state = STATE_WAITING;
		break;
	}
	case STATE_CHECK_EVENTS: {
		if(__mdm_settings.use_esttools==TRUE){
			read_events();
			if (__toSend > 0) {
				Ql_Timer_Stop(EST_EMPTY_TIMER);
				s32 nsend = Ql_SOC_Send(__socket_est_id, &__SendBuffer[0], __toSend);
				OUTD(">Send events packet num %d.Lenght:%d", __numPacket, __toSend);
				Ql_Timer_Start(EST_CONFIRM_TIMER, __mdm_settings.t_send * 1000, TRUE);
				_n_state = STATE_WAITING;
			}
		}
		break;
	}
	case STATE_SYNC_SAM: {
		OUTD(">Wait for sync SAM20");
			mdm_msg_send(0, 1);
		_n_state = STATE_WAITING;
		break;
	}
	}
}

void callback_tcp_actived(u8 contexId, s32 errCode, void* customParam) {
	OUTNET(">Callback activate tcp code:%d", errCode);
	_n_state = STATE_LOCALIP;

}
void callBack_tcp_deactived(u8 contextId, s32 errCode, void* customParam) {
	OUTNET("!Callback deactivate tcp code:%d", errCode);
	Ql_Reset(0);
}
