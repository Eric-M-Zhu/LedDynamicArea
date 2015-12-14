#pragma once

#include "MyDataType.h"

bool OpenSerialComm(tagSerialComm tag_SerialComm);
bool CloseSerialComm(tagSerialComm tag_SerialComm);

DWORD SendCommData(tagSerialComm tag_SerialComm, tagstruct_PHY1Header m_srt_PHY1Header, const char *pSendBuf, DWORD nSendLength,
	tagstruct_PHY1Header &m_srt_ReadPHY1Header, char *pReadBuf, DWORD &nReadLength);

//GetReadInfo:从串口读到显示屏信息。ComPort：串口控件；nSleepValue:最大等待时间，单位毫秒；nSleepUnit：单位等待时间，单位毫秒；nReadLength：读到的数据长度；返回读到的数据。
string GetReadInfo(HANDLE ComPort, DWORD nSleepValue, DWORD nSleepUnit, DWORD &nReadLength);
string GetSerialPortsInValid(const list<string> &Ports, string &szFirstComName); //判断串口是否是真正串口；而不是虚拟串口
void EnumComPorts(list<string> &Ports);