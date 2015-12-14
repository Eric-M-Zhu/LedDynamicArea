#include "stdafx.h"
#include "SendCommUnit.h"

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
	COMMTIMEOUTS commTimeouts;

	Result = "";
	n = 0;
	Answer = "";
	nReadLength = 0;
	ncurReadLength = 0;
	nMaxReadLength = COMPORT_MAX_READ_LENGTH;
	nCirculCount = nSleepValue / nSleepUnit;

	GetCommTimeouts(ComPort, &commTimeouts);
	commTimeouts.ReadIntervalTimeout = 0;
	commTimeouts.ReadTotalTimeoutMultiplier = 0;
	commTimeouts.ReadTotalTimeoutConstant = nSleepUnit;
	SetCommTimeouts(ComPort, &commTimeouts);

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


				 // function SendCommData(tag_SerialComm: tagSerialComm; m_srt_PHY1Header: tagstruct_PHY1Header;
			  //pSendBuf: pChar; nSendLength: Cardinal;
				 // var m_srt_ReadPHY1Header : tagstruct_PHY1Header; pReadBuf: PChar; var nReadLength : Cardinal) : Cardinal;
				 // var
					//  NumberOfWrite, Writting, NumberOfSend: Cardinal;
				 // Answer, Data      : string;
			  //nMaxReadLength: Cardinal;
				 // szAllSendBuf, szSendbuf: string;
			  //szReadBuf: string;
			  //n: Cardinal;
			  //m: Cardinal;
				 // begin
					//  Result : = 0;
				 // for m : = 1 to nSendLength do
					//  begin
					//  szSendbuf : = szSendbuf + pSendBuf^;
				 // inc(pSendBuf);
				 // end;
				 // if tag_SerialComm.CommPort <> nil then
					//  begin
					//  GetAllPhy1Data(m_srt_PHY1Header, @szSendBuf[1], Length(szSendBuf), szAllSendBuf);

				 // //串口设置////////////////////////
				 // try
					//  NumberOfWrite : = Length(szAllSendBuf);
			  //nMaxReadLength: = 2048; // NumberOfWrite * 2;
			  //Writting: = 1;
				 // if NumberOfWrite >= tag_SerialComm.nSendUnit then
					//  NumberOfSend : = tag_SerialComm.nSendUnit //须分块传送,一次一块,每块128字节
				 // else NumberOfSend : = NumberOfWrite;
				 // repeat
					//  tag_SerialComm.CommPort.Write(szAllSendBuf[Writting], NumberofSend);
			  //NumberOfWrite: = NumberOfWrite - NumberofSend;
			  //Writting: = Writting + NumberofSend;
				 // if NumberOfWrite < NumberOfSend then
					//  NumberOfSend : = numberOfWrite;
				 // Sleep(tag_SerialComm.nUnitSleepValue);
				 // until(NumberOfWrite <= 0);

			  //Data: = GetReadInfo(tag_SerialComm.CommPort, tag_SerialComm.nReadWaitTimeOut, COMPORT_MAX_READ_SLEEP_UNIT, nMaxReadLength);
				 // if Length(Data) > 0 then
					//  begin
					//  szReadBuf : = GetReadPhy1Data(Data, m_srt_ReadPHY1Header, n);
				 // if m_srt_PHY1Header.SrcAddr <> m_srt_ReadPHY1Header.DstAddr then
					//  n : = ORD(seDstAddrError)
				 // else if m_srt_PHY1Header.DstAddr <> m_srt_ReadPHY1Header.SrcAddr then
					//  n : = ORD(seSrcAddrError)
				 // else if m_srt_PHY1Header.ProtocolVer <> m_srt_ReadPHY1Header.ProtocolVer then
					//  n : = ORD(seProtocolVerError);
			  //nReadLength: = length(szReadBuf);
				 // for m : = 1 to nReadLength do
					//  begin
					//  pReadBuf^ : = szReadBuf[m];
				 // inc(pReadBuf);
				 // end;
				 // if n = Ord(SeNoAnswer) then
					//  begin
					//  Result : = ORD(SeNoAnswer);
				 // end
				 // else if n = Ord(seAnswerError) then
					//  begin
					//  Result : = ORD(seAnswerError);
				 // end
				 // else if n = Ord(seOK) then
					//  begin
					//  Result : = ORD(seOK);
				 // end
				 // else
					//  begin
					//  Result : = n;
				 // end;
				 // end
				 // else if Length(Data) = 0 then
					//  begin
					//  Result : = ORD(SeNoAnswer);
				 // end;
				 // except
					//  Result : = ORD(seOpenCommError);
				 // end;
				 // end
				 // else
					//  begin
					//  Result : = ORD(seNoneSerialProt);
				 // end;
				 // end;