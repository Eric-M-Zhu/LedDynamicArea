#pragma once

#include "MyDataType.h"
#include "SendInfo.h"

class CommunicateClass
{
public:
	CommunicateClass(Json::Value Screen_ja, DWORD nScreenOrd, DWORD nSendMode,
		DWORD nSendCmd, tagstruct_PHY1Header srt_PHY1Header,
		string szMessageStr, string szSendbuf,
		DWORD nSendLength, list<DWORD> lstSendProgram, HANDLE hdl, PtagSendThread ptagSendThread);
	~CommunicateClass();

	DWORD m_nSendResult;

private:
	Json::Value thrd_Screen_ja;
	DWORD thrd_nScreenOrd, thrd_nSendMode;
	tagstruct_PHY1Header thrd_srt_PHY1Header;
	DWORD thrd_nSendCmd;
	string thrd_szMessageStr;
	string thrd_szSendBuf;
	DWORD thrd_nSendLength;
	HANDLE thrd_hdl;
	PtagSendThread thrd_ptagSendThread;
	//thrd_SendProgramList: TList;
	//thrd_dtFirmwareVersion: TDate;
	BYTE thrd_ntmpBXCEColor; //第一版本的4C、4E控控制卡不管单双基色都采用双基色来转换数据。

	DWORD SendSingleCommand(string szSendBuf, DWORD nSendLength, DWORD nScreenOrd, DWORD nSendMode,
		string szSingleCommandInfo, tagCommunication &tag_Communication, tagstruct_PHY1Header &srt_PHY1Header,
		HANDLE hdl, Json::Value Screen_Supary, BYTE nRtnReq, DWORD &nStatus,
		DWORD &nErrorNumber, tagSendStatus &tmptagSendStatus, string &szReadBuf, DWORD &nReadLength);

	bool InitCommunication(DWORD nSendMode, DWORD nSendCmd, DWORD nBaud, tagCommunication &tag_Communication,
		Json::Value Screen_Obj, string szFileName, tagstruct_PHY1Header &srt_PHY1Header, tagSendStatus &tmptagSendStatus);
	bool CloseCommunication(DWORD nSendMode, tagCommunication &tag_Communication);

	DWORD SendScreenInfo(Json::Value Screen_Supary, DWORD nScreenOrd, DWORD nSendMode, BYTE nRtnReq,
		DWORD nSendCmd, string wdszMessageStr, list<DWORD> lstSendInfo, string szSendbuf, DWORD nSendLength, HANDLE hdl,
		PtagSendThread ptmptagSendThread, tagstruct_PHY1Header &srt_PHY1Header);

	void ReSetstruct_PHY1Header(tagstruct_PHY1Header &srt_PHY1Header, tagstruct_PHY1Header Source_srt_PHY1Header);
};

