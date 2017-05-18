#include <qlobal.h>



s32		__socket_th_id				=-1;
s32		__socket_web_id				=-1;
s32		__socket_dbg_id				=-1;
s32		__socket_est_id				=-1;
s32		__socket_upgrade_id			=-1;
u32		__tmr_array[7] = {1000, 5000,15000,30000,60000,150000 };
u8		__tmr_index = 0;
bool	__remote_dbg=FALSE;
s32		__rem_socket_dbg_id = -1;
s32	  __rem_socket_th_id = -1;

updater __updater__ = U_NONE;

ST_SOC_Callback callback_socket_th =
{
	socket_th_connect,
	socket_th_close,
	socket_th_accept,
	socket_th_read,
	socket_th_write
};


bool regiser_socket_th() {	
	s32 ret;
	ret = Ql_SOC_Register(callback_socket_th, "through");
	if (SOC_SUCCESS == ret)
	{
		OUTD(">Register socket through callback function successfully.");
		__socket_th_id = Ql_SOC_Create(0, SOC_TYPE_TCP);
		__socket_web_id= Ql_SOC_Create(0, SOC_TYPE_TCP);
		__socket_dbg_id = Ql_SOC_Create(0, SOC_TYPE_TCP);
		__socket_upgrade_id = Ql_SOC_Create(0, SOC_TYPE_TCP);
		if (__socket_th_id >= 0)
		{
			OUTD(">Create through socket id successfully,socketid=%d.", __socket_th_id);
			ret = Ql_SOC_Bind(__socket_th_id, __SCOCET_TH_PORT__);
			ret = Ql_SOC_Listen(__socket_th_id, __SOCKET_CLIENT_COUNT__);
		}
		if (__socket_web_id >= 0) {
			OUTD(">Create web socket id successfully,socketid=%d.", __socket_web_id);
			ret = Ql_SOC_Bind(__socket_web_id, __SCOCET_WEB_PORT__);
			ret = Ql_SOC_Listen(__socket_web_id, __SOCKET_CLIENT_COUNT__);
		}
		if (__socket_dbg_id >= 0) {
			OUTD(">Create dbg socket id successfully,socketid=%d.", __socket_dbg_id);
			ret = Ql_SOC_Bind(__socket_dbg_id, __SCOCET_DBG_PORT__);
			ret = Ql_SOC_Listen(__socket_dbg_id, __SOCKET_CLIENT_COUNT__);
		}
		if (__socket_upgrade_id >= 0) {
			OUTD(">Create upgrade socket id successfully,socketid=%d.", __socket_upgrade_id);
			ret = Ql_SOC_Bind(__socket_upgrade_id, __SCOCET_UPGRADE_PORT__);
			ret = Ql_SOC_Listen(__socket_upgrade_id, 1);
		}
	}
	else
	{
		OUTD("!Register Socket callback function failure,ret=%d.", ret);
		return FALSE;
	}
	return TRUE;
}

void est_socket_connect(u32 timerId, void* param) {
	__socket_est_id = Ql_SOC_Create(0, SOC_TYPE_TCP);
	if (__socket_est_id >= 0) {
		Ql_SOC_Connect(__socket_est_id, __mdm_settings.ip, __mdm_settings.tcp);
	}
}

void est_wait_confirm(u32 timerId, void* param) {
	if (__countRepeat > 2)
	{
		OUTD("!Timeout wait confirm message. count:%d", __countRepeat);
		Ql_Reset(0);
	}
	else
	{
		OUTD("!Continue wait confirm message. count:%d", __countRepeat);
	}
	__countRepeat++;
}

void est_empty_msg(u32 timerId, void* param) {
	TrigerEvent(None, 0, &__currentEvData);
	__toSend = BuildEventMessage(__power_state, &__currentEvData, 1);
	s32 nsend = Ql_SOC_Send(__socket_est_id, &__SendBuffer[0], __toSend);
	OUTD(">No events. Send empty packet num:%d.",__numPacket);
	Ql_Timer_Start(EST_CONFIRM_TIMER, __mdm_settings.t_send * 1000, TRUE);
	_n_state = STATE_WAITING;
}

void socket_th_connect(s32 socketId, s32 errCode, void* customParam) {
	
	if (errCode < 0) {
		Ql_SOC_Close(__socket_est_id);
		__socket_est_id = -1;
		if (__tmr_index>3) {
			__tmr_index = 0;
			Ql_Reset(0);
		}
		else
			__tmr_index++;
		Ql_Timer_Start(EST_CONNECTOR_TIMER,__tmr_array[__tmr_index],FALSE);
		OUTD("!Connect to est failure. waiting:%d %s", __tmr_array[__tmr_index]/1000," sec.");
		__EST_CONNECT__ = FALSE;
	}
	else {
		Ql_Timer_Stop(EST_CONNECTOR_TIMER);
		//__tmr_index = 0;
		OUTD(">Connect to est is OK.");
		__toSend = BuildInputStateMessage();
		s32 nsend= Ql_SOC_Send(__socket_est_id, &__SendBuffer[0], __toSend);
		OUTD(">Send pins state packet num:%d. Lenght:%d",__numPacket, __toSend);
		__EST_CONNECT__ = TRUE;
		est_empty_msg(0, NULL);
	}
}

void socket_th_close(s32 socketId, s32 errCode, void* customParam) {
	s32 index;
	if (socketId == __socket_est_id) {
		if (__mdm_settings.use_esttools == TRUE) {
			OUTD("Close socket EST");
			//_n_state = STATE_EST_CONNECT;
			Ql_Timer_Start(EST_CONNECTOR_TIMER, 10000, FALSE);
		}
		Ql_Timer_Stop(EST_CONFIRM_TIMER);
		Ql_Timer_Stop(EST_EMPTY_TIMER);
	}
	else
	{
		QlClient* client = get_client_by_socketid(socketId);
		if (socketId == __rem_socket_dbg_id) {
			__remote_dbg = FALSE;
			__rem_socket_dbg_id = -1;
		}
		else if (socketId == __rem_socket_th_id) {
			__rem_socket_th_id = -1;
		}
		index = find_client_by_sockid(socketId);
		client_uninit(index);
	}
	if (errCode == SOC_SUCCESS)
	{
		OUTD("CallBack: close socket successfully.");
	}
	else if (errCode == SOC_BEARER_FAIL)
	{
		//m_tcp_state = STATE_GPRS_DEACTIVATE;
		OUTD("!CallBack: close socket failure,(socketId=%d,error_cause=%d)", socketId, errCode);
	}
	else
	{
		//m_tcp_state = STATE_SOC_LISTENING;
		OUTD("!CallBack: close socket failure,(socketId=%d,error_cause=%d)", socketId, errCode);
	}
	OUTD("!Close client socket:%d", socketId);
} 


void socket_th_accept(s32 listenSocketId, s32 errCode, void* customParam) {
	enum_socket_type tp;
	u32 remote_ip;
	u16 remote_port;
	if (listenSocketId == __socket_th_id)
		tp = THROUGH;
	else if (listenSocketId == __socket_web_id)
		tp = WEB;
	else if (listenSocketId == __socket_dbg_id) {
		tp = DBG;
	}
	else if (listenSocketId == __socket_est_id)
		tp = EST;
	else if (listenSocketId == __socket_upgrade_id)
		tp = UPGRADE;
	s32 accept_socket=Ql_SOC_Accept(listenSocketId, &remote_ip, &remote_port);
	s32 index=insert_socket(accept_socket, tp);
	QlClient* client = get_client_by_socketid(accept_socket);
	if(client!=NULL)
		OUTD("accept listen:%d accept:%d type:%d index in:%d", listenSocketId, client->socketId, client->en_sock_type,index);
	if (tp==DBG) {
		__remote_dbg = TRUE;
		__rem_socket_dbg_id = accept_socket;
		//Ql_SOC_Send(accept_socket,"Terminal is open.\r\n",19);
	}
	if (tp == THROUGH) {
		__rem_socket_th_id = accept_socket;
		
	}
	if (tp == UPGRADE) {
	
	}
}



//u32 isize = 0;
void socket_th_read(s32 socketId, s32 errCode, void* customParam) {
	s32 index;
	s32 ret;
	if (socketId == __socket_est_id) {
		
		u8 dat[100];
		s32 len=Ql_SOC_Recv(socketId, dat, 100);
		//OUTD("recieve data EST len:%d",len);
		if (len>0) {
			unreplace(dat, len);
			__tmr_index = 0;
		}
	}
	else
	{
		index = find_client_by_sockid(socketId);
		QlClient* client = get_client_by_socketid(socketId);
		if (client != NULL)
		{
			ret = func_read_handle_callback(socketId, errCode, client, index);
			//OUTD("read len:%d type:%d index:%d", ret, client->en_sock_type, index);
			switch (client->en_sock_type)
			{
			case WEB: {
				HttpHeader header;
				header.authorization = FALSE;
				if (parse_header(client->recv_buf, &header))
				{
					OUTD("end parce web");
					if (header.method == GET) {
						if (!header.authorization) {

							Ql_strcpy(header.action, "autorize");

						}
						OUTD("autorize:%d action:%s", header.authorization, header.action);
						page_response(&header);
						OUTD("page response");
						func_send_handle(socketId, __s_buf, client);

					}
					else if (header.method == POST) {
						OUTD("len:%d", header.length);
					}
				}
				break;
			}
			case DBG: {
					u8* pData = client->recv_buf;
					if (ret > 0) {

						if (ret > 2 &&  pData[ret - 1] == 13) {
							fill_command_buffer(pData);
							__DEBUG__ = FALSE;
							parce_command();
							if (__mdm_settings.debug == TRUE)
								__DEBUG__ = TRUE;
						}
						else
						{
							if (*pData == 13 || *pData == 10) {
								if (__MENUS__) {
									get_menu_console();
								}
								else {
									parce_command();
								}
							}
							else {
								fill_command_buffer(pData);
							}

						}
					}
				break;
			}
			case THROUGH:{
				//OUTD("THROUGH");
				//data_to_hex(client->recv_buf, ret);
				u8 d[50] = { 2 };
				Ql_memcpy(&d[1], client->recv_buf, ret);
				mdm_msg_send(&d[0], ret+1);
				break;
			}
			case UPGRADE: {
				
				char* pst = Ql_strstr(client->recv_buf, "_SAM_");
				if (pst) {
					s32 handle;
					u32 writed;
					Ql_FS_Delete(__FILE_SAM_UPDATE__);
					handle = Ql_FS_Open(__FILE_SAM_UPDATE__, QL_FS_CREATE);
					if (handle>0) {
						//OUTD("Create sam file update is OK");
						Ql_FS_Close(handle);
						Ql_SOC_Send(socketId, &ret, 4);
					}
					else {
						//OUTD("!Cand create sam file update:%d", handle);
						Ql_SOC_Send(socketId, -1, 4);
					}
					__updater__ = U_SAM;
					break;
				}
				pst = Ql_strstr(client->recv_buf, "_MDM_");
				if(pst){
					s32 handle;
					u32 writed;
					Ql_FS_Delete(__FILE_MDM_UPDATE__);
					handle = Ql_FS_Open(__FILE_MDM_UPDATE__, QL_FS_CREATE);
					if (handle>0) {
						//OUTD("Create mdm file update is OK");
						Ql_FS_Close(handle);
						Ql_SOC_Send(socketId, &ret, 4);
					}
					else {
						//OUTD("!Cand create mdm file update:%d", handle);
						Ql_SOC_Send(socketId, -1, 4);
					}
					__updater__ = U_MDM;
					break;
				}
				if(Ql_strstr(client->recv_buf, "_E_N_D"))
				{
					//OUTD("_E_N_D");
						__mdm_settings.update = TRUE;
						write_mdm_settings();
						u8 d[2] = { 254,1 };
						mdm_msg_send(&d[0], 2);
						s32 fsam = Ql_FS_GetSize(__FILE_SAM_UPDATE__);
						s32 fmdm = Ql_FS_GetSize(__FILE_MDM_UPDATE__);
						OUTD("Recive update files mdm:%d sam:%d", fmdm, fsam);
						Ql_Sleep(3000);
						Ql_Reset(0);
				}
				else {
					u8 dd[1024];
					Ql_memset(dd, 0, 1024);
					Ql_memcpy(dd, client->recv_buf, ret);
					switch (__updater__)
					{
					case U_SAM: {
						u32 writed;
						s32 fz = Ql_FS_GetSize(__FILE_SAM_UPDATE__);
						s32 handle = Ql_FS_Open(__FILE_SAM_UPDATE__, QL_FS_READ_WRITE);
						if (handle > 0) {
							
							Ql_FS_Seek(handle, fz, QL_FS_FILE_BEGIN);
							Ql_FS_Write(handle, &dd[0], ret, &writed);
							Ql_FS_Flush(handle);
							Ql_FS_Close(handle);
							Ql_SOC_Send(socketId, &writed, 4);
							//OUTD("write update sam OK:%d",writed);
						}
						else {
							//OUTD("!Write update sam error:%d", handle);
							Ql_SOC_Send(socketId, -1, 4);
						}
						break;
					}
					case U_MDM: {
						u32 writed;
						s32 fz = Ql_FS_GetSize(__FILE_MDM_UPDATE__);
						s32 handle = Ql_FS_Open(__FILE_MDM_UPDATE__, QL_FS_READ_WRITE);
						if (handle > 0) {
							Ql_FS_Seek(handle, fz, QL_FS_FILE_BEGIN);
							Ql_FS_Write(handle, &dd[0], ret, &writed);
							Ql_FS_Flush(handle);
							Ql_FS_Close(handle);
							Ql_SOC_Send(socketId, &writed, 4);
							//OUTD("write update mdm OK:%d",writed);
						}
						else {
							//OUTD("!Write update mdm error:%d", handle);
							Ql_SOC_Send(socketId, -1, 4);
						}
						break;
					}
					default:
						break;
					}
				}
				break;
			}
			default:
				break;
			}
		}
	}
}

void socket_th_write(s32 socketId, s32 errCode, void* customParam){
	OUTD("!!!!!!!");
	/*s32 ret;
	s32 index;

	index = find_client_by_sockid(socketId);

	if (errCode)
	{
		OUTD("CallBack: socket write failure,(sock=%d,error=%d)", socketId, errCode);
		OUTD("Close socket[%d].", m_client[index].socketId);
		Ql_SOC_Close(m_client[index].socketId);//error , Ql_SOC_Close

		client_uninit(index);

		if (ret == SOC_BEARER_FAIL)
		{
			if (check_is_allsoc_close())// no socket connection
			{
				//m_tcp_state = STATE_GPRS_DEACTIVATE;
			}
			else
			{
				//m_tcp_state = STATE_SOC_LISTENING;
			}
		}
		else
		{
			//m_tcp_state = STATE_SOC_LISTENING;
		}
		return;
	}

	m_client[index].send_handle(m_client[index].socketId, m_client[index].pSendCurrentPos);
	OUTD("socket write id:%d", socketId);*/
}
