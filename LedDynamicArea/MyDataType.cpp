#include "stdafx.h"
#include "MyDataType.h"

#include "resource.h"

extern Json::Value g_ControllerList_Obj;
extern Json::Value g_Controller_Supary;


static UINT g_argProFrameSingleColorBmpCount = 0;
static UINT g_argProFrameMuliColorBmpCount = 0;

SysParameter Syssetup;
BYTE g_aryCRC16Byte[512];
string g_szCRC16Str;
static UINT *g_aryProFrameSingleColorBmp = NULL;
static UINT *g_aryProFrameMuliColorBmp = NULL;
tagCommStatusRecord g_arytagCommStatusRecord;
string g_szAppPath;

bool StrIsInt(string Source)
{
	if (Source.size() == 0)
	{
		return false;
	}
	else
	{
		for (size_t i = 0; i < Source.size(); ++i)
		{
			if ((Source[i] < '0') || (Source[i] > '9'))
			{
				return false;
			}
		}

		return true;
	}
}

//------------------------------------------------------------------------------
// 2012-12-30程序优化

void CreateFrameArray(UINT singleColorCount, UINT multiColorCount)
{
	g_aryProFrameSingleColorBmp = new UINT[singleColorCount];
	for (UINT i = 0; i < singleColorCount; i++)
	{
		g_aryProFrameSingleColorBmp[i] = IDB_SINGLECOLOR_1 + i;
	}
	g_argProFrameSingleColorBmpCount = singleColorCount;

	g_aryProFrameMuliColorBmp = new UINT[multiColorCount];
	for (UINT i = 0; i < multiColorCount; i++)
	{
		g_aryProFrameMuliColorBmp[i] = IDB_MULTICOLOR_1 + i;
	}
	g_argProFrameMuliColorBmpCount = multiColorCount;
}

void GetControllerList(Json::Value &ControllerList_Obj, Json::Value &Controller_Supary)
{
	HRSRC hResource = FindResource(GetModuleHandle(L"LedDynamicArea.dll"), MAKEINTRESOURCE(IDR_CONTROLLERLIST), TEXT("FILE"));
	if (hResource)
	{
		HGLOBAL hg = LoadResource(GetModuleHandle(L"LedDynamicArea.dll"), hResource);
		if (hg)
		{
			LPVOID pData = LockResource(hg);
			if (pData)
			{
				UINT dwSize = SizeofResource(GetModuleHandle(L"LedDynamicArea.dll"), hResource);
				Json::Reader reader;
				reader.parse((char*)pData, (char*)pData + dwSize, ControllerList_Obj, false);
				Controller_Supary = ControllerList_Obj["ProtocolVer_List"];
			}
		}
	}
}

int GetSelScreenArrayOrd(int nScreenNo, const Json::Value &Screen_Ja)
{
	for (UINT i = 0; i < Screen_Ja.size(); i++)
	{
		if (Screen_Ja[i]["Com_address"].asInt() == nScreenNo)
		{
			return (int)i;
		}
	}

	return -1;
}

void DoEvents()
{
	MSG Msg;

	while (PeekMessage(&Msg, NULL, 0, 0, 1))
	{
		if (Msg.message != 18)
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
	}
}

Json::Value GetControllerObject(const Json::Value &Controller_Supary, UINT nControllerType, unsigned char &nProtocolVer)
{
	Json::Value Result;
	unsigned char nTypeValue1, nTypeValue2;
	UINT nDefControllerType;

	GetController_DecomposeType(nControllerType, nTypeValue1, nTypeValue2);
	for (UINT i = 0; i < Controller_Supary.size(); ++i)
	{
		UINT nControllerCount = Controller_Supary[i]["Controller_Type_List"].size();
		for (UINT j = 0; j < nControllerCount; j++)
		{
			int ntmpTypeValue1 = Controller_Supary[i]["Controller_Type_List"][j]["Type_Value_1"].asInt();
			int ntmpTypeValue2 = Controller_Supary[i]["Controller_Type_List"][j]["Type_Value_2"].asInt();

			if ((nTypeValue1 == ntmpTypeValue1) && (nTypeValue2 == ntmpTypeValue2))
			{
				if (nControllerType < CONTROLLER_TYPE_FOURTH)
				{
					nProtocolVer = PROTOCOLVER_THIRD_CONTROL;
				}
				else
				{
					nProtocolVer = (unsigned char)Controller_Supary[i]["ProtocolVer"].asInt();
				}
				return Controller_Supary[i]["Controller_Type_List"][j];
			}
		}
	}

	return GetDefaultController(nProtocolVer, nDefControllerType);
}

void GetController_DecomposeType(int nControllerType, unsigned char &nTypeValue1, unsigned char &nTypeValue2)
{
	nTypeValue1 = nControllerType & 0xFF;
	nTypeValue2 = (nControllerType >> 8) & 0xFF;
}

Json::Value GetDefaultController(unsigned char &nProtocolVer, UINT &nControllerType)
{
	int nDefProtocolVerIndex = g_ControllerList_Obj["DefProtocolVerIndex"].asInt();
	UINT nDefControllerIndex = g_Controller_Supary[nDefProtocolVerIndex]["DefControllerIndex"].asUInt();
	UINT nControllerCount = g_Controller_Supary[nDefProtocolVerIndex]["Controller_Type_List"].size();
	Json::Value Controller_obj;

	if (nControllerCount <= nDefControllerIndex)
	{
		nDefControllerIndex = 0;
	}
	nProtocolVer = (unsigned char)g_Controller_Supary[nDefProtocolVerIndex]["ProtocolVer"].asInt();
	Controller_obj = g_Controller_Supary[nDefProtocolVerIndex]["Controller_Type_List"][nDefControllerIndex];
	nControllerType = GetControllerType(Controller_obj);
	return Controller_obj;
}

UINT GetControllerType(Json::Value Controller_obj)
{
	return ((Controller_obj["Type_Value_2"].asInt() << 8) & 0xFF00) +
		(Controller_obj["Type_Value_1"].asInt() & 0xFF);
}

UINT GetSelFrameWidth(UINT nDY_AreaFMode, UINT nDY_AreaFLine)
{
	UNREFERENCED_PARAMETER(nDY_AreaFMode);

	if (nDY_AreaFLine < g_argProFrameSingleColorBmpCount)
	{
		HBITMAP hBmp = LoadBitmap(GetModuleHandle(L"LedDynamicArea.dll"), MAKEINTRESOURCE(g_aryProFrameSingleColorBmp[nDY_AreaFLine]));
		BITMAP bmp;
		GetObject(hBmp, sizeof(BITMAP), &bmp);
		DeleteObject(hBmp);
		return bmp.bmHeight;
	}
	else
	{
		return 1;
	}
}

int GetSelScreenArrayOrd(int nScreenNo, Json::Value &Screen_Ja)
{
	int Result = -1;

	for (UINT I = 1; I < Screen_Ja.size(); ++I)
	{
		if (Screen_Ja[I - 1]["Com_address"].asInt() == nScreenNo)
		{
			Result = I - 1;
			break;
		}
	}

	return Result;
}

int GetSelScreenDYAreaOrd(int nDYAreaID, Json::Value &DYArea_Ja) //根据动态区域编号得到动态区区域列表中的序号。
{
	int Result = -1;
	for (UINT I = 1; I < DYArea_Ja.size(); ++I)
	{
		if (DYArea_Ja[I - 1]["DY_AreaID"].asInt() == nDYAreaID)
		{
			Result = I - 1;
			break;
		}
	}

	return Result;
}

list<string> SplitString(string Source, string Deli)
{
	list<string> result;
	char *pBuffer = new char[Source.size() + 1];
	char *pToken = NULL;

	strcpy(pBuffer, Source.c_str());
	pToken = strtok(pBuffer, Deli.c_str());

	while (pToken)
	{
		result.push_back(pToken);
		pToken = strtok(NULL, Deli.c_str());
	}

	delete[] pBuffer;

	return result;
}

BYTE GetBmpPixelFormat(BYTE nPx)
{
	switch (nPx)
	{
	case SCREEN_COLOR_SINGLE:
	case SCREEN_COLOR_DOUBLE:
	case SCREEN_COLOR_THREE:
		return 4;
	case SCREEN_COLOR_FULLCOLOR:
		return 24;
	default:
		return 4;
	}
}