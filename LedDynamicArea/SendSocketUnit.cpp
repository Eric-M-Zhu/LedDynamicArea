#include "stdafx.h"
#include "SendSocketUnit.h"
#include "Phy01.h"

DWORD CreateMySocket(DWORD nSocketMode, string szDestAddr, DWORD nPort, SOCKET skt)
{
	sockaddr_in addr;
	BOOL opt;
	int re;
	DWORD Result = seInvalid_Socket;

	//Sleep(0);
//try
	//WSAStartup($0101, WSAData);
	if (nSocketMode == NET_CONNECT_STATICIP)
	{
		skt = socket(AF_INET, SOCK_STREAM, 0);
		if (skt == INVALID_SOCKET)
		{
			closesocket(skt);
			//WSACleanup();
			return seInvalid_Socket;
		}
		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_addr.S_un.S_addr = inet_addr(szDestAddr.c_str());
		addr.sin_port = htons((u_short)nPort);
		re = connect(skt, (sockaddr*)&addr, sizeof(addr));
		if (re == SOCKET_ERROR)
		{
			closesocket(skt);
			//WSACleanup();
			return seSocket_Error;
		}
		Result = seSocket_OK;
	}
	else if (nSocketMode == NET_CONNECT_DIRECT)
	{
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = 0;
		addr.sin_port = htons(9999);
		opt = TRUE; //设置该套接字为广播类型，
		skt = socket(AF_INET, SOCK_DGRAM, 0);
		if (skt == INVALID_SOCKET)
		{
			closesocket(skt);
			//WSACleanup();
			return seInvalid_Socket;
		}

		setsockopt(skt, SOL_SOCKET, SO_BROADCAST, (const char *)&opt, sizeof(opt));
		re = bind(skt, (sockaddr*)&addr, sizeof(sockaddr_in)); //把该套接字绑定在一个具体的地址上
		if (re == SOCKET_ERROR)
		{
			closesocket(skt);
			//WSACleanup();
			return seSocket_Error;
		}
		Result = seSocket_OK;
	}
	//except
		//closesocket(skt);
		//WSACleanup();
		//Result: = Ord(seInvalid_Socket);

	return Result;
}

bool CloseMySocket(SOCKET &skt)
{
	closesocket(skt);
	return true;
}

DWORD SendSocketData(tagNetwork tag_NetWork, tagstruct_PHY1Header m_srt_PHY1Header,
	const char *pSendBuf, DWORD nSendLength, tagstruct_PHY1Header m_srt_ReadPHY1Header,
	char *pReadBuf, DWORD &nReadLength)
{
	DWORD NumberOfWrite, Writting, NumberOfSend;
	string Answer, Data;
	DWORD nMaxReadLength;
	string szAllSendBuf, szSendbuf;
	string szCom;
	string szReadBuf;
	DWORD n;
	//DWORD m;
	int re = SOCKET_ERROR;
	//bool re2;
	sockaddr_in toaddr;
	DWORD Result = 0;

	szSendbuf.append(pSendBuf, nSendLength);
	GetAllPhy1Data(m_srt_PHY1Header, pSendBuf, nSendLength, szAllSendBuf);
	//try
	if (tag_NetWork.nServerMode == 0)
	{
		if (tag_NetWork.Network_Skt > 0)
		{
			NumberOfWrite = szAllSendBuf.size();
			nMaxReadLength = 2048; // NumberOfWrite * 2;
			Writting = 0;
			if (NumberOfWrite >= tag_NetWork.nSendUnit)
				NumberOfSend = tag_NetWork.nSendUnit; //须分块传送,一次一块,每块128字节
			else
				NumberOfSend = NumberOfWrite;

			do
			{
				if (tag_NetWork.Network_Mode == NET_CONNECT_STATICIP)
					re = send(tag_NetWork.Network_Skt, szAllSendBuf.c_str() + Writting, NumberOfSend, 0);
				else if (tag_NetWork.Network_Mode == NET_CONNECT_DIRECT)
				{
					toaddr.sin_family = AF_INET;
					toaddr.sin_addr.S_un.S_addr = inet_addr(tag_NetWork.Network_IP.c_str()); // INADDR_BROADCAST;
					toaddr.sin_port = htons(5007);
					re = sendto(tag_NetWork.Network_Skt, szAllSendBuf.c_str() + Writting, NumberOfSend, 0, (sockaddr*)&toaddr, sizeof(toaddr)); //从广播地址发送消息
				}
				if (re == SOCKET_ERROR)
				{
					return seSocket_Error;
				}
				NumberOfWrite = NumberOfWrite - NumberOfSend;
				Writting = Writting + NumberOfSend;

				if (NumberOfWrite < NumberOfSend)
					NumberOfSend = NumberOfWrite;

				Sleep(tag_NetWork.nUnitSleepValue);
			} while (NumberOfWrite > 0);

			Data = GetSocketReadInfo(tag_NetWork.szServerMode_ID, (BYTE)tag_NetWork.nServerMode,
				tag_NetWork.Network_Mode, tag_NetWork.Network_Skt, tag_NetWork.nReadWaitTimeOut, COMPORT_MAX_READ_SLEEP_UNIT,
				nMaxReadLength);

			if (Data.size() > 0)
			{
				szReadBuf = GetReadPhy1Data(Data, m_srt_ReadPHY1Header, n);
				if (m_srt_PHY1Header.SrcAddr != m_srt_ReadPHY1Header.DstAddr)
					n = seDstAddrError;
				else if (m_srt_PHY1Header.DstAddr != m_srt_ReadPHY1Header.SrcAddr)
					n = seSrcAddrError;
				else if (m_srt_PHY1Header.ProtocolVer != m_srt_ReadPHY1Header.ProtocolVer)
					n = seProtocolVerError;

				nReadLength = szReadBuf.size();
				memcpy(pReadBuf, szReadBuf.c_str(), nReadLength);
				if (n == SeNoAnswer)
				{
					Result = SeNoAnswer;
				}
				else if (n == seAnswerError)
				{
					Result = seAnswerError;
				}
				else if (n == seOK)
				{
					Result = seOK;
				}
				else
				{
					Result = n;
				}
			}
			else if (Data.size() == 0)
			{
				Result = SeNoAnswer;
			}
		}
	}
	//else
	//{
	//	NumberOfWrite = szAllSendBuf.size();
	//	nMaxReadLength = 2048; // NumberOfWrite * 2;
	//	Writting = 1;
	//	if (NumberOfWrite >= tag_NetWork.nSendUnit)
	//		NumberOfSend = tag_NetWork.nSendUnit; //须分块传送,一次一块,每块128字节
	//	else
	//		NumberOfSend = NumberOfWrite;
	//	do
	//	{
	//		re2 = ServerMode_Send(tag_NetWork.szServerMode_ID, szAllSendBuf.c_str() + Writting, NumbeOfSend);

	//		if (!re2)
	//		{
	//			return seSendError;
	//		}

	//		NumberOfWrite = NumberOfWrite - NumberOfSend;
	//		Writting = Writting + NumberOfSend;
	//		if (NumberOfWrite < NumberOfSend)
	//			NumberOfSend = NumberOfWrite;
	//		Sleep(tag_NetWork.nUnitSleepValue);
	//	} while (NumberOfWrite > 0);

	//	Data = GetSocketReadInfo(tag_NetWork.szServerMode_ID, tag_NetWork.nServerMode,
	//		tag_NetWork.Network_Mode, tag_NetWork.Network_Skt, tag_NetWork.nReadWaitTimeOut,
	//		COMPORT_MAX_READ_SLEEP_UNIT, nMaxReadLength);
	//	if (Data.size() > 0)
	//	{
	//		szReadBuf = GetReadPhy1Data(Data, m_srt_ReadPHY1Header, n);
	//		if (m_srt_PHY1Header.SrcAddr != m_srt_ReadPHY1Header.DstAddr)
	//			n = seDstAddrError;
	//		else if (m_srt_PHY1Header.DstAddr != m_srt_ReadPHY1Header.SrcAddr)
	//			n = seSrcAddrError;
	//		else if (m_srt_PHY1Header.ProtocolVer != m_srt_ReadPHY1Header.ProtocolVer)
	//			n = seProtocolVerError;
	//		else if (m_srt_PHY1Header.DeviceType != m_srt_ReadPHY1Header.DeviceType)
	//			n = seDeviceTypeError;

	//		nReadLength = szReadBuf.size();
	//		memcpy(pReadBuf, szReadBuf.c_str() nReadLength);

	//		if (n == SeNoAnswer)
	//		{
	//			Result = SeNoAnswer;
	//		}
	//		else if (n == seAnswerError)
	//		{
	//			Result = seAnswerError;
	//		}
	//		else if (n == seOK)
	//		{
	//			Result = seOK;
	//		}
	//		else
	//		{
	//			Result = n;
	//		}
	//	}
	//	else
	//	{
	//		Result = SeNoAnswer;
	//	}
	//}
	/*except
		Result : = ORD(seSocket_Error);
	end;*/

	return Result;
}

string GetSocketReadInfo(string szServerMode_NetworkID, BYTE nServerMode,
	DWORD nSocketMode, SOCKET skt, DWORD nSleepValue, DWORD nSleepUnit,
	DWORD &nReadLength)
{
	DWORD nMaxReadLength, ncurReadLength;
	string Answer, tmpAnswer;
	DWORD n, /*k,*/ nCirculCount;
	string Result = "";

	n = 0;
	Answer = "";
	nReadLength = 0;
	ncurReadLength = 0;
	nMaxReadLength = COMPORT_MAX_READ_LENGTH;
	nCirculCount = nSleepValue / nSleepUnit;

	//SetLength(tmpAnswer, 4096);
	// while n < nCirculCount do

	if (nServerMode == 0)
	{
		//SetLength(Answer, 2920);
		Answer = MyRecv(nSocketMode, skt, nSleepValue);
		ncurReadLength = Answer.size();
		Result = Result + Answer;
		nReadLength = nReadLength + ncurReadLength;
	}
	else
	{
		//while (n < nCirculCount)
		//{
		//	ncurReadLength = ServerMode_Recv(szServerMode_NetworkID, @tmpAnswer[1]);
		//	for (k = 0; k < ncurReadLength; ++k)
		//	{
		//		Answer = Answer + tmpAnswer[k];
		//		nReadLength = nReadLength + ncurReadLength;
		//		if pos(Char($5A), Answer) > 0 then //查到有帧尾立即退出读串口操作，否则不判断的话要等待全部时间
		//		{
		//			break;
		//		}
		//		++n;
		//		Sleep(nSleepUnit);
		//	}
		//	Result = Answer;
		//}
	}

	return Result;
}

string MyRecv(DWORD nSocketMode, SOCKET Skt, int vTimeout)
{
	fd_set FDRead;
	struct timeval LTimeout;
	char *BufRecv;
	int iResult;
	//DWORD m;
	//int nBufStart;
	string Result = "";

	UNREFERENCED_PARAMETER(nSocketMode);

	BufRecv = (char*)malloc(2920);
	memset(BufRecv, 0, 2920);

	//try
	iResult = -1;
	FD_ZERO(&FDRead);
	FD_SET(Skt, &FDRead);

	if (vTimeout <= 0)
		iResult = select(0, &FDRead, NULL, NULL, NULL);
	else
	{
		LTimeout.tv_sec = vTimeout / 1000;
		LTimeout.tv_usec = (vTimeout % 1000) * 1000;
		iResult = select(0, &FDRead, NULL, NULL, &LTimeout);
	}

	if (iResult == 1)
	{
		iResult = recv(Skt, BufRecv, 2920, 0);
		if (iResult == SOCKET_ERROR)
			iResult = WSAGetLastError();
	}
	else
		iResult = WSAGetLastError();

	if (iResult > 0)
		Result.append(BufRecv, iResult);

	free(BufRecv);

	/*except
		FreeMem(BufRecv);
	end;*/

	return Result;
}

//function GetPCharString(pBuf: PChar; nLength: Integer) : string;
//var
//	m : Cardinal;
//begin
//	for m : = 1 to nLength do
//		begin
//		Result : = Result + pBuf^;
//inc(pBuf);
//end;
//end;