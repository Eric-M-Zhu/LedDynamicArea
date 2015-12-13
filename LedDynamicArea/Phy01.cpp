#include "stdafx.h"
#include "Phy01.h"
#include "MyDataType.h"
#include "CRC16.h"

extern BYTE g_aryCRC16Byte[];
extern string g_szCRC16Str;

void GetDecipheringData(tagstruct_PHY1Header_5th srt_PHY1Header, DWORD nEncryptionLength,
	string szData, DWORD nLength, string &szDecipheringData)
{
	DWORD I, J, nCRC16Ord;
	string szEncryptionValue;
	string sztmpData;

	if ((srt_PHY1Header.ProtocolVer == PROTOCOLVER_FOURTH_CONTROL) ||
		(srt_PHY1Header.ProtocolVer == PROTOCOLVER_FIFTH_CONTROL))
	{
		if (srt_PHY1Header.EncryptionType != ENCRYPTION_NONE)
		{
			nCRC16Ord = srt_PHY1Header.RandomNumber;
			szEncryptionValue = "";
			szEncryptionValue = g_szCRC16Str;// Copy(g_szCRC16Str, nCRC16Ord + 1, nEncryptionLength);
			sztmpData = szData;
			szDecipheringData = "";
			for (I = 1; I < nLength; ++I)
			{
				J = (I - 1) % nEncryptionLength;
				szDecipheringData = szDecipheringData + (char)((BYTE)(sztmpData[I]) ^ (BYTE)(szEncryptionValue[J + 1]));
			}
		}
		else
		{
			szDecipheringData = szData;
		}
	}
	else
	{
		szDecipheringData = szData;
	}
}

//function GetUSBUserEncryption(szdef_encryption, szuser_encryption: string) : string; //USB的Phy1层发送的加密信息
//var
//I : Cardinal;
//tmpdef_encryption, tmpuser_encryption: string;
//nEncryptionLength: Cardinal;
//szPhy1UserEncryption: string;
//begin
//tmpdef_encryption : = szdef_encryption;
//tmpuser_encryption: = szuser_encryption;
//nEncryptionLength: = Length(szdef_encryption);
//
//SetLength(tmpdef_encryption, nEncryptionLength);
//SetLength(tmpuser_encryption, nEncryptionLength);
//for I : = 1 to nEncryptionLength do
//begin
//szPhy1UserEncryption : = szPhy1UserEncryption + Char(((Byte(tmpdef_encryption[I]) + Byte(tmpuser_encryption[I])) and $FF) xor Byte(tmpdef_encryption[I]))
//	end;
//Result: = szPhy1UserEncryption;
//end;

//function GetXorPassword(szPassword, szDef_Password: string) : string;
//var
//I : Cardinal;
//tmpPassword: string;
//begin
//tmpPassword : = szPassword;
//SetLength(tmpPassword, Length(szDef_Password));
//Result: = '';
//for I : = 1 to Length(szDef_Password) do
//begin
//Result : = Result + Char(Byte(tmpPassword[I]) xor Byte(szDef_Password[I]));
//end;
//end;

void GetPhy1Data(tagstruct_PHY1Header srt_PHY1Header, const char *pData, DWORD nLength, string &szPhy1Data)
{
	string szData, szEncryptionedData;
	string szPhy1Header;
	DWORD i;
	WORD wPhy1Check;

	szData.resize(nLength);
	for (i = 0; i < nLength; ++i)
	{
		szData[i] = *pData;
		++pData;
	}

	szEncryptionedData = szData;

	szPhy1Header.resize(16);
	szPhy1Header[0] = (char)(srt_PHY1Header.DstAddr & 0xFF);
	szPhy1Header[1] = (char)((srt_PHY1Header.DstAddr >> 8) & 0xFF);
	szPhy1Header[2] = (char)(srt_PHY1Header.SrcAddr & 0xFF);
	szPhy1Header[3] = (char)((srt_PHY1Header.SrcAddr >> 8) & 0xFF);
	szPhy1Header[4] = (char)(srt_PHY1Header.ProtocolVer);
	szPhy1Header[5] = (char)(srt_PHY1Header.Reserved1);
	szPhy1Header[6] = (char)(srt_PHY1Header.DeviceType & 0xFF);
	szPhy1Header[7] = (char)((srt_PHY1Header.DeviceType >> 8) & 0xFF);
	szPhy1Header[8] = (char)(srt_PHY1Header.Reserved2);
	szPhy1Header[9] = (char)(srt_PHY1Header.Reserved2);
	szPhy1Header[10] = (char)(srt_PHY1Header.Reserved2);
	szPhy1Header[11] = (char)(srt_PHY1Header.Reserved2);
	szPhy1Header[12] = (char)(nLength & 0xFF);
	szPhy1Header[13] = (char)((nLength >> 8) & 0xFF);
	szPhy1Header[14] = (char)((nLength >> 16) & 0xFF);
	szPhy1Header[15] = (char)((nLength >> 24) & 0xFF);

	nLength = nLength + szPhy1Header.size();
	szPhy1Data = szPhy1Header + szEncryptionedData;
	wPhy1Check = 0;
	GetCRCCheck(szPhy1Data.c_str(), nLength, wPhy1Check);

	szPhy1Data = szPhy1Data + (char)(wPhy1Check & 0xFF) + (char)((wPhy1Check >> 8) & 0xFF);
}

void GetAllPhy1Data(tagstruct_PHY1Header srt_PHY1Header, const char *pData, DWORD nLength, string &szPhy1AllData)
{
	string szPhy1Data;
	string sztmpPhyData;
	DWORD i;
	DWORD ncurAddress;

	ncurAddress = 0;
	GetPhy1Data(srt_PHY1Header, pData, nLength, szPhy1Data);
	for (i = 0; i < szPhy1Data.size(); ++i)
	{
		if (szPhy1Data[i] == 0xA5) //转义$A5为$A6,$02
		{
			ncurAddress += 2;
			sztmpPhyData.resize(ncurAddress);
			sztmpPhyData[ncurAddress - 2] = (BYTE)0xA6;
			sztmpPhyData[ncurAddress - 1] = (BYTE)0x02;
		}
		else if (szPhy1Data[i] == 0xA6) //转义$A6为$A6,$01
		{
			ncurAddress += 2;
			sztmpPhyData.resize(ncurAddress);
			sztmpPhyData[ncurAddress - 2] = (BYTE)0xA6;
			sztmpPhyData[ncurAddress - 1] = (BYTE)0x01;
		}
		else if (szPhy1Data[i] == 0x5A) //转义$5A为$5B,$02
		{
			ncurAddress += 2;
			sztmpPhyData.resize(ncurAddress);
			sztmpPhyData[ncurAddress - 2] = (BYTE)0x5B;
			sztmpPhyData[ncurAddress - 1] = (BYTE)0x02;
		}
		else if (szPhy1Data[i] == 0x5B) //转义$5B为$5B,$01
		{
			ncurAddress += 2;
			sztmpPhyData.resize(ncurAddress);
			sztmpPhyData[ncurAddress - 2] = (BYTE)0x5B;
			sztmpPhyData[ncurAddress - 1] = (BYTE)0x01;
		}
		else
		{
			++ncurAddress;
			sztmpPhyData.resize(ncurAddress);
			sztmpPhyData[ncurAddress - 1] = szPhy1Data[i];
		}
	}

	szPhy1AllData.push_back((BYTE)0xA5);
	szPhy1AllData.push_back((BYTE)0xA5);
	szPhy1AllData.push_back((BYTE)0xA5);
	szPhy1AllData.push_back((BYTE)0xA5);
	szPhy1AllData.push_back((BYTE)0xA5);
	szPhy1AllData.push_back((BYTE)0xA5);
	szPhy1AllData.push_back((BYTE)0xA5);
	szPhy1AllData.push_back((BYTE)0xA5);
	szPhy1AllData += sztmpPhyData;
	szPhy1AllData.push_back((BYTE)0xA5);
}

int Pos(char Needle, string HayStack)
{
	for (size_t i = 0; i < HayStack.size(); i++)
	{
		if (HayStack[i] == Needle)
		{
			return i + 1; // Because delphi counts array from 1
		}
	}

	return 0;
}

string GetReadPhy1Data(string szReadOriginalData, tagstruct_PHY1Header &srt_ReadPHY1Header, DWORD &nSendError) //处理接收到的信息
{
	string sztmpReadOriginalData, sztmpStr;
	DWORD ntmpReadOriginalDataLength;
	DWORD I;
	DWORD ncurAddress;
	string szDecipheringData;
	tagstruct_PHY1Header_5th srt_ReadPHY1Header_5th;

	ncurAddress = 0;

	if (szReadOriginalData.size() == 0)
	{
		nSendError = 2; //seNoAnswer
	}
	else if ((Pos((BYTE)0xA5, szReadOriginalData) <= 0) || (Pos((BYTE)0x5A, szReadOriginalData) <= 0))
	{
		nSendError = 3; //seAnswerError
	}
	else
	{
		sztmpStr = szReadOriginalData;
		sztmpStr.erase(0, sztmpStr.find_first_not_of((BYTE)0xA5));
		sztmpStr.erase(sztmpStr.find_last_not_of((BYTE)0x5A) + 1);

		I = 0;
		while (I < sztmpStr.size())//for i := 1 to length(sztmpStr) do
		{
			if (sztmpStr[I] == 0xA6)
			{
				if (sztmpStr.size() >(I + 1))
				{
					if (sztmpStr[I + 1] == 0x02)
					{
						++ncurAddress;
						sztmpReadOriginalData.resize(ncurAddress);
						sztmpReadOriginalData[ncurAddress - 1] = (BYTE)0xA5;
						I = I + 2;
					}
					else if (sztmpStr[I + 1] == 0x01)
					{
						++ncurAddress;
						sztmpReadOriginalData.resize(ncurAddress);
						sztmpReadOriginalData[ncurAddress - 1] = (BYTE)0xA6;
						I = I + 2;
					}
				}
				else //实际上该数据已经出错，因为$A6后边一定有一个字节的$01或$02
				{
					++ncurAddress;
					sztmpReadOriginalData.resize(ncurAddress);
					sztmpReadOriginalData[ncurAddress - 1] = (BYTE)0xA6;
					++I;
				}
			}
			else if (sztmpStr[I] == 0x5B)
			{
				if (sztmpStr.size() > (I + 1))
				{
					if (sztmpStr[I + 1] == 0x02)
					{
						++ncurAddress;
						sztmpReadOriginalData.resize(ncurAddress);
						sztmpReadOriginalData[ncurAddress - 1] = 0x5A;
						I = I + 2;
					}
					else if (sztmpStr[I + 1] == 0x01)
					{
						++ncurAddress;
						sztmpReadOriginalData.resize(ncurAddress);
						sztmpReadOriginalData[ncurAddress - 1] = 0x5B;
						I = I + 2;
					}
				}
				else //实际上该数据已经出错，因为$5B后边一定有一个字节的$01或$02
				{
					++ncurAddress;
					sztmpReadOriginalData.resize(ncurAddress);
					sztmpReadOriginalData[ncurAddress - 1] = 0x5B;
					++I;
				}
			}
			else //其它数据正常接收
			{
				++ncurAddress;
				sztmpReadOriginalData.resize(ncurAddress);
				sztmpReadOriginalData[ncurAddress - 1] = sztmpStr[I];
				++I;
			}
		}

		if (sztmpReadOriginalData.size() >= 2)
			srt_ReadPHY1Header.DstAddr = ((BYTE)(sztmpReadOriginalData[2]) << 8) & 0xFF00 + (BYTE)(sztmpReadOriginalData[1]);

		if (sztmpReadOriginalData.size() >= 4)
			srt_ReadPHY1Header.SrcAddr = ((BYTE)(sztmpReadOriginalData[4]) << 8) & 0xFF00 + (BYTE)(sztmpReadOriginalData[3]);

		if (sztmpReadOriginalData.size() >= 5)
			srt_ReadPHY1Header.ProtocolVer = (BYTE)(sztmpReadOriginalData[5]);

		if (srt_ReadPHY1Header.ProtocolVer == PROTOCOLVER_FIFTH_CONTROL)
		{
			memset(&srt_ReadPHY1Header_5th, 0, sizeof(tagstruct_PHY1Header_5th));

			if (sztmpReadOriginalData.size() >= 2)
				srt_ReadPHY1Header_5th.DstAddr = ((BYTE)(sztmpReadOriginalData[2]) << 8) & 0xFF00 + (BYTE)(sztmpReadOriginalData[1]);
			if (sztmpReadOriginalData.size() >= 4)
				srt_ReadPHY1Header_5th.SrcAddr = ((BYTE)(sztmpReadOriginalData[4]) << 8) & 0xFF00 + (BYTE)(sztmpReadOriginalData[3]);
			if (sztmpReadOriginalData.size() >= 5)
				srt_ReadPHY1Header_5th.ProtocolVer = (BYTE)(sztmpReadOriginalData[5]);

			if (sztmpReadOriginalData.size() >= 6)
				srt_ReadPHY1Header_5th.ProtecolType = (BYTE)(sztmpReadOriginalData[6]);
			if (sztmpReadOriginalData.size() >= 8)
				srt_ReadPHY1Header_5th.DeviceType = (BYTE)(sztmpReadOriginalData[7]) + ((BYTE)(sztmpReadOriginalData[8]) << 8) & 0xFF00;
			if (sztmpReadOriginalData.size() >= 9)
				srt_ReadPHY1Header_5th.PHYType = (BYTE)(sztmpReadOriginalData[9]);
			if (sztmpReadOriginalData.size() >= 10)
				srt_ReadPHY1Header_5th.Reserved1 = (BYTE)(sztmpReadOriginalData[10]);
			if (sztmpReadOriginalData.size() >= 12)
				srt_ReadPHY1Header_5th.MsgSeq = ((BYTE)(sztmpReadOriginalData[12]) << 8) & 0xFF00 + (BYTE)(sztmpReadOriginalData[11]);
			if (sztmpReadOriginalData.size() >= 13)
				srt_ReadPHY1Header_5th.Reserved2 = (BYTE)(sztmpReadOriginalData[13]);
			if (sztmpReadOriginalData.size() >= 14)
				srt_ReadPHY1Header_5th.FrameLenMax = (BYTE)(sztmpReadOriginalData[14]);
			if (sztmpReadOriginalData.size() >= 15)
				srt_ReadPHY1Header_5th.EncryptionType = (BYTE)(sztmpReadOriginalData[15]);
			if (sztmpReadOriginalData.size() >= 16)
				srt_ReadPHY1Header_5th.RandomNumber = (BYTE)(sztmpReadOriginalData[16]);

			if (sztmpReadOriginalData.size() >= 22)
			{
				srt_ReadPHY1Header_5th.Secret[0] = sztmpReadOriginalData[17];
				srt_ReadPHY1Header_5th.Secret[1] = sztmpReadOriginalData[18];
				srt_ReadPHY1Header_5th.Secret[2] = sztmpReadOriginalData[19];
				srt_ReadPHY1Header_5th.Secret[3] = sztmpReadOriginalData[20];
				srt_ReadPHY1Header_5th.Secret[4] = sztmpReadOriginalData[21];
				srt_ReadPHY1Header_5th.Secret[5] = sztmpReadOriginalData[22];
			}

			if (sztmpReadOriginalData.size() >= 24)
				srt_ReadPHY1Header_5th.Datalen = ((BYTE)(sztmpReadOriginalData[24]) << 8) & 0xFF00 + (BYTE)(sztmpReadOriginalData[23]);
			if (sztmpReadOriginalData.size() >= 26)
				srt_ReadPHY1Header_5th.DataChecksum = ((BYTE)(sztmpReadOriginalData[26]) << 8) & 0xFF00 + (BYTE)(sztmpReadOriginalData[25]);
			if (sztmpReadOriginalData.size() >= 27)
				srt_ReadPHY1Header_5th.HeaderChecksum = (BYTE)(sztmpReadOriginalData[27]);
			if (sztmpReadOriginalData.size() > 27)
				sztmpReadOriginalData.erase(0, 27);

			ntmpReadOriginalDataLength = sztmpReadOriginalData.size();
			GetDecipheringData(srt_ReadPHY1Header_5th, ENCRYPTION_LENGTH, sztmpReadOriginalData, ntmpReadOriginalDataLength, szDecipheringData);
			nSendError = seOK;
			return szDecipheringData;
		}
		else if (srt_ReadPHY1Header.ProtocolVer == PROTOCOLVER_FIFTH_DYNAMIC)
		{
			if (sztmpReadOriginalData.size() >= 6)
				srt_ReadPHY1Header.Reserved1 = (BYTE)(sztmpReadOriginalData[6]);
			if (sztmpReadOriginalData.size() >= 8)
				srt_ReadPHY1Header.DeviceType = (BYTE)(sztmpReadOriginalData[7]) + (BYTE)(sztmpReadOriginalData[8] << 8) & 0xFF00;
			if (sztmpReadOriginalData.size() >= 9)
				srt_ReadPHY1Header.Reserved2 = (BYTE)(sztmpReadOriginalData[9]);
			if (sztmpReadOriginalData.size() >= 10)
				srt_ReadPHY1Header.Reserved3 = (BYTE)(sztmpReadOriginalData[10]);
			if (sztmpReadOriginalData.size() >= 11)
				srt_ReadPHY1Header.Reserved4 = (BYTE)(sztmpReadOriginalData[11]);
			if (sztmpReadOriginalData.size() >= 12)
				srt_ReadPHY1Header.Reserved5 = (BYTE)(sztmpReadOriginalData[12]);

			if (sztmpReadOriginalData.size() >= 16)
				srt_ReadPHY1Header.Datalen = (BYTE)(sztmpReadOriginalData[13])
				+ ((BYTE)(sztmpReadOriginalData[14]) << 8) & 0xFF00
				+ ((BYTE)(sztmpReadOriginalData[15]) << 16) & 0xFF0000
				+ ((BYTE)(sztmpReadOriginalData[16]) << 24) & 0xFF000000;

			if (sztmpReadOriginalData.size() > 16)
				sztmpReadOriginalData.erase(0, 16);

			ntmpReadOriginalDataLength = sztmpReadOriginalData.size() - 2; //去掉最后两个字节的校验字节。
			nSendError = seOK;
			return sztmpReadOriginalData.substr(0, ntmpReadOriginalDataLength);
		}
	}

	return "";
}

DWORD GetStrInt(string Str) //从字符串中提取数字
{
	DWORD xh;
	string B_Str, B_Edit;

	B_Str = "";
	B_Edit = Str;
	for (xh = 1; xh < B_Edit.size(); ++xh)
	{
		if (('0' <= B_Edit[xh]) && (B_Edit[xh] <= '9')) //取消小数点, '.'] then
			B_Str = B_Str + B_Edit[xh];
	} //循环结束，B_Str就是你所要提取的数字

	return atoi(B_Str.c_str());
}

DWORD GetRandomValue(int AFrom, int ATo)
{
	srand((int)time(0));
	return rand() % (ATo - AFrom) + AFrom;
}