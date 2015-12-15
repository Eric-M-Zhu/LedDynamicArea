#include "stdafx.h"
#include "SendCommUnit.h"
#include "Phy01.h"

void EnumComPorts(list<string> &Ports)
{
	DWORD ErrCode, Index;
	HKEY hKey;
	char ValueName[256], Data[256];
	DWORD ValueLen, DataLen, ValueType;

	ErrCode = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DEVICEMAP\\SERIALCOMM", 0, KEY_READ, &hKey);
	if (ErrCode == ERROR_SUCCESS)
	{
		Index = 0;
		ValueLen = 256;
		DataLen = 256;

		while (ERROR_SUCCESS == RegEnumValueA(hKey, Index++, ValueName, &ValueLen, NULL, &ValueType, (LPBYTE)Data, &DataLen))
		{
			Ports.push_back(Data);
		}

		RegCloseKey(hKey);
	}
}

string GetSerialPortsInValid(const list<string> &Ports, string &szFirstComName)
{
	// Don't implement it now because it's not used
	UNREFERENCED_PARAMETER(Ports);
	szFirstComName = "";
	return "";
}

/*function GetSerialPortsInValid(Ports: TStringList; var szFirstComName : string) : string;
	var
		szComName : string;
ComPort_Thread: TComPort;
	szAllSendBuf, szSendbuf: string;
szReadBuf: string;
	NumberOfWrite, Writting, NumberOfSend: Cardinal;
	Answer, Data      : string;
nMaxReadLength: Cardinal;
i: integer;
nComCount: Cardinal;
	begin
		Result : = '';
szFirstComName: = '';
	for i : = 1 to Ports.Count do
		begin
		nMaxReadLength : = 4096;
szComName: = Ports.Strings[i - 1];
szAllSendBuf: = Char($01) + Char($02) + Char($FE) + Char($FF);
	try
		ComPort_Thread : = TComPort.Create(nil);
	ComPort_Thread.Port : = szComName;
	ComPort_Thread.BaudRate : = br57600;
	ComPort_Thread.Parity.Bits : = prNone;
	ComPort_Thread.DataBits : = dbEight;
	ComPort_Thread.StopBits : = sbOneStopBit;
	ComPort_Thread.Events : = [];
	ComPort_Thread.Open;
	if ComPort_Thread.Connected = True then
		begin
		NumberOfWrite : = Length(szAllSendBuf);
Answer: = '';
Data: = '';
	ComPort_Thread.Write(szAllSendBuf[1], NumberOfWrite);
	ComPort_Thread.ReadStr(Answer, nMaxReadLength);
	end;
	if Answer = szAllSendBuf then
		Result : = Result + szComName + ';'
	else
		begin
		if szFirstComName = '' then
			szFirstComName : = szComName;
nComCount: = Length(g_arytagCommStatusRecord);
	SetLength(g_arytagCommStatusRecord, nComCount + 1);
	g_arytagCommStatusRecord[Length(g_arytagCommStatusRecord) - 1].nCom_port : = GetStrInt(szComName);
	g_arytagCommStatusRecord[Length(g_arytagCommStatusRecord) - 1].bBusyStatus : = False;
	end;
	ComPort_Thread.Close;
	ComPort_Thread.Free;
	except
		ComPort_Thread.Close;
	ComPort_Thread.Free;
Result: = Result + szComName + ';'
	end;
		end;
		end;*/

string GetReadInfo(HANDLE ComPort, DWORD nSleepValue, DWORD nSleepUnit, DWORD &nReadLength)
{
	DWORD nCirculCount;
	DWORD n, nMaxReadLength, ncurReadLength;
	char *Answer;
	string Result;
	//COMMTIMEOUTS commTimeouts;

	Result = "";
	n = 0;
	Answer = "";
	nReadLength = 0;
	ncurReadLength = 0;
	nMaxReadLength = COMPORT_MAX_READ_LENGTH;
	nCirculCount = nSleepValue / nSleepUnit;

	//GetCommTimeouts(ComPort, &commTimeouts);
	//commTimeouts.ReadIntervalTimeout = 0;
	//commTimeouts.ReadTotalTimeoutMultiplier = 0;
	//commTimeouts.ReadTotalTimeoutConstant = nSleepUnit;
	//SetCommTimeouts(ComPort, &commTimeouts);

	Answer = new char[nMaxReadLength];
	if (Answer)
	{
		while (n < nCirculCount)
		{
			if (ReadFile(ComPort, (LPVOID)Answer, nMaxReadLength, &ncurReadLength, NULL))
			{
				Result.append(Answer, ncurReadLength);
				nReadLength += ncurReadLength;

				if (-1 != Result.find_first_of(0x5A))
				{
					break;
				}
			}

			++n;
		}

		delete[] Answer;
	}

	return Result;
}

bool OpenSerialComm(tagSerialComm tag_SerialComm)
{
	bool result = false;
	string szCom = tag_SerialComm.szCommName;

	transform(szCom.begin(), szCom.end(), szCom.begin(), ::toupper);
	if (szCom != SERIAL_PORT_NONE)
	{
		tag_SerialComm.CommPort = CreateFileA(szCom.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (tag_SerialComm.CommPort != INVALID_HANDLE_VALUE)
		{
			DCB dcb;
			dcb.DCBlength = sizeof(DCB);

			GetCommState(tag_SerialComm.CommPort, &dcb);

			dcb.BaudRate = tag_SerialComm.nCommBaud;

			if ((0 <= tag_SerialComm.nCommParity) && (tag_SerialComm.nCommParity <= 4))
			{
				dcb.Parity = (BYTE)tag_SerialComm.nCommParity;
			}

			if ((5 <= tag_SerialComm.nCommDataBits) && (tag_SerialComm.nCommDataBits <= 8))
			{
				dcb.ByteSize = (BYTE)tag_SerialComm.nCommDataBits;
			}

			if ((0 <= tag_SerialComm.nCommStopBits) && (tag_SerialComm.nCommStopBits <= 2))
			{
				dcb.StopBits = (BYTE)tag_SerialComm.nCommStopBits;
			}

			SetCommState(tag_SerialComm.CommPort, &dcb);

			result = true;
		}
	}

	return result;
}

bool CloseSerialComm(tagSerialComm tag_SerialComm)
{
	if (tag_SerialComm.CommPort != INVALID_HANDLE_VALUE)
	{
		CloseHandle(tag_SerialComm.CommPort);
		return true;
	}
	else
	{
		return false;
	}
}

DWORD SendCommData(tagSerialComm tag_SerialComm, tagstruct_PHY1Header &m_srt_PHY1Header, const char *pSendBuf, DWORD nSendLength,
	tagstruct_PHY1Header &m_srt_ReadPHY1Header, char *pReadBuf, DWORD &nReadLength)
{
	DWORD NumberOfWrite, Writting, NumberOfSend;
	string Answer, Data;
	DWORD nMaxReadLength;
	string szAllSendBuf, szSendbuf;
	string szReadBuf;
	DWORD n;
	DWORD m;
	DWORD Result = 0;
	COMMTIMEOUTS commTimeouts;


	szSendbuf.append(pSendBuf, nSendLength);
	if (tag_SerialComm.CommPort != INVALID_HANDLE_VALUE)
	{
		GetAllPhy1Data(m_srt_PHY1Header, pSendBuf, nSendLength, szAllSendBuf);

		commTimeouts.ReadIntervalTimeout = 0;
		commTimeouts.ReadTotalTimeoutMultiplier = 0;
		commTimeouts.ReadTotalTimeoutConstant = tag_SerialComm.nReadWaitTimeOut;
		commTimeouts.WriteTotalTimeoutMultiplier = 0;
		commTimeouts.WriteTotalTimeoutConstant = 5000;
		SetCommTimeouts(tag_SerialComm.CommPort, &commTimeouts);

		NumberOfSend = 0;
		NumberOfWrite = szAllSendBuf.size();

		while (NumberOfWrite > 0)
		{
			DWORD writtenLength;
			NumberOfSend = min(NumberOfWrite, tag_SerialComm.nSendUnit);
			if (WriteFile(tag_SerialComm.CommPort, szAllSendBuf.c_str() + szAllSendBuf.size() - NumberOfWrite, NumberOfSend, &writtenLength, NULL))
			{
				if (NumberOfWrite > writtenLength)
				{
					NumberOfWrite -= writtenLength;
				}
				else
				{
					NumberOfWrite = 0;
				}
			}
		}

		Data = GetReadInfo(tag_SerialComm.CommPort, tag_SerialComm.nReadWaitTimeOut, COMPORT_MAX_READ_SLEEP_UNIT, nMaxReadLength);
		if (Data.size() > 0)
		{
			szReadBuf = GetReadPhy1Data(Data, m_srt_PHY1Header, n);
			if (m_srt_PHY1Header.SrcAddr != m_srt_ReadPHY1Header.DstAddr)
			{
				n = seDstAddrError;
			}
			else if (m_srt_PHY1Header.DstAddr != m_srt_ReadPHY1Header.SrcAddr)
			{
				n = seSrcAddrError;
			}
			else if (m_srt_PHY1Header.ProtocolVer != m_srt_ReadPHY1Header.ProtocolVer)
			{
				n = seProtocolVerError;
			}
			nReadLength = szReadBuf.size();
			memcpy(pReadBuf, szReadBuf.c_str(), nReadLength);

			Result = n;
		}
		else
		{
			Result = SeNoAnswer;
		}
	}
	else
	{
		Result = seNoneSerialProt;
	}

	return Result;
}
