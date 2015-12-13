#pragma once

#include "MyDataType.h"

void GetDecipheringData(tagstruct_PHY1Header_5th srt_PHY1Header, DWORD nEncryptionLength,
	string szData, DWORD nLength, string &szDecipheringData); //根据CRC16的g_aryCRC16Byte的序号指定nEncryptionLength长度密匙数据解密指定长度数据；目前其实与加密算法一致。
string GetUSBUserEncryption(string szdef_encryption, string szuser_encryption); //USB的Phy1层发送的加密信息
string GetXorPassword(string szPassword, string szDef_Password);
void GetPhy1Data(tagstruct_PHY1Header srt_PHY1Header,
	const char *pData, DWORD nLength, string &szPhy1Data); //得到物理层数据(转义前)
void GetAllPhy1Data(tagstruct_PHY1Header srt_PHY1Header, const char *pData, DWORD nLength, string &szPhy1AllData); //得到物理层数据(转义后)
string GetReadPhy1Data(string szReadOriginalData, tagstruct_PHY1Header &srt_ReadPHY1Header, DWORD &nSendError); //处理接收到的信息
DWORD GetStrInt(string Str); //从字符串中提取数字
DWORD GetRandomValue(int AFrom, int ATo); //从AFrom到ATo之间取一个随机数。