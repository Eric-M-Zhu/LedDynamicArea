#pragma once

#include "MyDataType.h"

DWORD CreateMySocket(DWORD nSocketMode, string szDestAddr, DWORD nPort, SOCKET skt);
bool CloseMySocket(SOCKET &skt);
DWORD SendSocketData(tagNetwork tag_NetWork, tagstruct_PHY1Header m_srt_PHY1Header,
	const char *pSendBuf, DWORD nSendLength,
	tagstruct_PHY1Header &m_srt_ReadPHY1Header, char *pReadBuf, DWORD &nReadLength);
string GetSocketReadInfo(string szServerMode_NetworkID, BYTE nServerMode,
	DWORD nSocketMode, SOCKET skt, DWORD nSleepValue, DWORD nSleepUnit,
	DWORD &nReadLength);
string MyRecv(DWORD nSocketMode, SOCKET Skt, int vTimeout);

//string GetPCharString(const char *pBuf, int nLength);