#pragma once

#include "MyDataType.h"

#define WM_SENDSTATUS (WM_USER + 160)
#define WM_MODIFY_PING_BAUD (WM_USER + 131)

//------------------------------------------------------------------------------
// GetAckOrNAck判断是Ack还是NAck  ;返回值：0：NAck；1：Ack；2：其它错误

//参数说明：szBuf:接收到信息串
//------------------------------------------------------------------------------
DWORD GetAckOrNAck(tagstruct_PHY1Header srt_PHY1Header, DWORD nSendCmd, string szReadBuf,
	bool &bNeedReSend);

//------------------------------------------------------------------------------
// SendScreenInfo发送显示屏信息到显示屏
//参数说明：
//nLangueID：语言ID；
//Screen_ja:显示屏列表；
//nScreenOrd:显示屏在文件列表中的序号；
//nSendMode:通讯模式；0：串口；1：gprs；2：网口。
//srt_PHY1Header:物理层结构
//nSendCmd:发送命令。
//pMessageStr：提示信息。
//pSendBuf:发送数据指针
//nSendLength：发送数据长度
//hdl:接收消息句柄
//pReadBuf：从控制器读取的信息指针；只有读取类命令该参数才有效
//nReadLength：从控制器读取的信息长度；只有读取类命令该参数才有效
//------------------------------------------------------------------------------
void FreeList(list<DWORD> lstSendInfo);

//返回不同状态下的NAck错误状态信息
string GetNAckMessageInfo(DWORD nScreenOrd, Json::Value Screen_ja, string szSendMessage, DWORD nNAckOrd);

//nSendFileType为要发送的文件类型；没有文件类型时，该参数为0xFF
DWORD CreateSendThreadList(DWORD nSingleOrMulti, DWORD nIndex, DWORD nScreenOrd, HANDLE hdlResultHandle, PtagSendThread pSendThread,
	Json::Value Screen_ja, DWORD nSendMode, tagstruct_PHY1Header srt_PHY1Header, DWORD nSendCmd, BYTE nSendFileType, string wdszMessageStr,
	string szSendbuf, DWORD nSendLength, list<DWORD> &lstSendProgram, HANDLE hdl, bool bReadBufSaveAs, string szReadFileName);