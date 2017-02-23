#include <qlobal.h>


static s32 m_Index_Ack;
char __s_buf[SEND_BUF_LEN];
char __r_buf[RECV_BUF_LEN];


s32 func_send_handle(s8 sock, char *PDtata,void* params) {
	s32 ret;
	s32 index;
	QlClient* client = (QlClient*)params;
	//if (!Ql_strlen(PDtata)) {
	//	OUTD("no data need to send");
	//	return -1;
	//}
	////m_tcp_state = STATE_SOC_SENDING;

	//index = find_client_by_sockid(sock);

	/*Ql_memset(client->send_buf, '\0', SEND_BUF_LEN);
	if (Ql_strlen(PDtata) > SEND_BUF_LEN)
	{
		Ql_strncpy(client->send_buf, PDtata, SEND_BUF_LEN);
	}
	else
	{
		Ql_strncpy(client->send_buf, PDtata, Ql_strlen(PDtata));
	}*/
	client->sendRemain_len = Ql_strlen(client->send_buf);
	client->pSendCurrentPos = client->send_buf;


	do
	{
		ret = Ql_SOC_Send(client->socketId, client->pSendCurrentPos, client->sendRemain_len);
		//OUTD("Send data to socket[%d],number of bytes=%d", client->socketId, ret);
		if (ret == client->sendRemain_len)//send compelete
		{
			client->sendRemain_len = 0;
			client->pSendCurrentPos = NULL;
			client->nSentLen += ret;
			m_Index_Ack = index;

			//m_tcp_state = STATE_SOC_ACK;

			break;
		}
		else if ((ret <= 0) && (ret == SOC_WOULDBLOCK))
		{
			//waiting CallBack_socket_write, then send data; 
			m_Index_Ack = index;
			break;
		}
		else if (ret <= 0)
		{
			//OUTD("Send data failure,ret=%d.", ret);
			//OUTD("<-- Close socket[%d].", client->socketId);
			Ql_SOC_Close(client->socketId);//error , Ql_SOC_Close

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
			break;
		}
		else if (ret < client->sendRemain_len)//continue send, do not send all data
		{
			client->sendRemain_len -= ret;
			client->pSendCurrentPos += ret;
			client->nSentLen += ret;
		}
	} while (1);


}

s32 func_send_ack(s8 sock) {
	s32 index;
	u64 ackedNumCurr;

	index = find_client_by_sockid(sock);

	s32 ret = Ql_SOC_GetAckNumber(sock, &ackedNumCurr);
	if (ret < 0)
	{
		OUTD("!Error socket for ask:%d", ret);
	}
	if (m_client[index].nSentLen == ackedNumCurr)
	{
		OUTD("ACK Number:%llu/%llu from socket[%d].Client has received all data.", m_client[index].nSentLen, ackedNumCurr, sock);

		Ql_memset(m_client[index].send_buf, '\0', SEND_BUF_LEN);
		//m_tcp_state = STATE_SOC_SEND;
	}
	else
	{
		OUTD("ACK Number:%llu/%llu from socket[%d]", ackedNumCurr, m_client[index].nSentLen, sock);
	}

}

s32 func_read_handle_callback(s32 socket, s32 errCode, void* customParam, s32 index) {
	s32 ret;
	u32 pos=0;
	QlClient* client = (QlClient*)customParam;
	
	Ql_memset(client->recv_buf, '\0', RECV_BUF_LEN);
	if (errCode)
	{

		//OUTD("CallBack: socket read failure,(sock=%d,error=%d)", socket, errCode);
		//OUTD("Close socket[%d].", m_client[index].socketId);
		Ql_SOC_Close(client->socketId);//error , Ql_SOC_Close

		client_uninit(index);

		if (errCode == SOC_BEARER_FAIL)
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
		return -1;
	}
	do
	{
		ret = Ql_SOC_Recv(client->socketId,&client->recv_buf[pos], RECV_BUF_LEN);

		if ((ret < 0) && (ret != SOC_WOULDBLOCK))
		{
			//OUTD("Receive data failure,ret=%d.", ret);
			//OUTD("Close socket[%d].", client->socketId);
			Ql_SOC_Close(client->socketId);//error , Ql_SOC_Close

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
			break;
		}
		else if (ret == SOC_WOULDBLOCK)
		{
			//OUTD("wait next CallBack_socket_read");
			//Ql_memcpy(client->recv_buf[pos], "\0", 1);
			return pos;
		}
		else if (ret <= RECV_BUF_LEN)
		{
			//OUTD("Receive data from sock(%d),len(%d):%s <<<<", socket, ret, client->recv_buf);
			pos += ret;
			//m_tcp_state = STATE_SOC_SENDING;
			//m_client[index].send_handle(m_client[index].socketId, m_client[index].recv_buf);//send back to client.
			//break;

		}
		

	} while (1);
}


s32 find_client_by_sockid(s8 sockid)
{
	s32 i;
	s32 index = -1;
	for (i = 0; i < MAX_CLIENT_COUNT; i++)
	{
		if (sockid == m_client[i].socketId)
		{
			index = i;
			break;
		}
	}
	return index;
}

void client_socket_id_init()
{
	s32 i;
	for (i = 0; i < MAX_CLIENT_COUNT; i++)
	{
		m_client[i].en_sock_type = TP_NONE;
		m_client[i].socketId = 0x7F;
		m_client[i].recv_buf = NULL;
		m_client[i].send_buf = NULL;
	}
}

s16 insert_socket(u16 accept_socket_id, enum_socket_type tp) {
	s16 index = -1;
	for (s32 i = 0; i < MAX_CLIENT_COUNT; i++)
	{
		if (m_client[i].socketId == 0x7F)
		{
			client_init(i, accept_socket_id, tp);
			index = i;
			break;
		}
	}
	return index;
}


QlClient* get_client_by_socketid(s32 socketId) {
	s32 index = find_client_by_sockid(socketId);
	if (index != -1)
		return &m_client[index];
	else
		return NULL;
}


void client_init(s32 index, s8 sockid, enum_socket_type tp)
{
	
	m_client[index].en_sock_type = tp;
	m_client[index].socketId = sockid;
	m_client[index].IsGreeting = FALSE;
	m_client[index].nSentLen = 0;

	m_client[index].recv_buf = __r_buf;
	m_client[index].send_buf = __s_buf;

	Ql_memset(m_client[index].send_buf, '\0', SEND_BUF_LEN);
	Ql_memset(m_client[index].recv_buf, '\0', RECV_BUF_LEN);

	m_client[index].sendRemain_len = 0;
	m_client[index].recvRemain_len = RECV_BUF_LEN;

	m_client[index].pSendCurrentPos = m_client[index].send_buf;
	m_client[index].pRecvCurrentPos = m_client[index].recv_buf;

	m_client[index].send_handle = func_send_handle;
	m_client[index].send_ack = func_send_ack;
	m_client[index].read_handle_callback = func_read_handle_callback;
}

void client_uninit(s32 index)
{
	m_client[index].en_sock_type = TP_NONE;
	m_client[index].socketId = 0x7F;
	m_client[index].IsGreeting = FALSE;
	m_client[index].nSentLen = 0;

	m_client[index].recv_buf = NULL;
	m_client[index].send_buf = NULL;

	//Ql_memset(m_client[index].send_buf, '\0', SEND_BUFFER_LEN);
	//Ql_memset(m_client[index].recv_buf, '\0', RECV_BUFFER_LEN);

	m_client[index].sendRemain_len = 0;
	m_client[index].recvRemain_len = 0;

	m_client[index].pSendCurrentPos = NULL;
	m_client[index].pRecvCurrentPos = NULL;

	m_client[index].send_handle = NULL;
	m_client[index].send_ack = NULL;
	m_client[index].read_handle_callback = NULL;
}

bool check_is_allsoc_close()
{
	s32 i;

	for (i = 0; i < MAX_CLIENT_COUNT; i++)
	{
		if (m_client[i].socketId != 0x7F)
		{
			break;
		}
	}

	if (i == MAX_CLIENT_COUNT)
	{
		return TRUE;
	}

	return FALSE;
}

