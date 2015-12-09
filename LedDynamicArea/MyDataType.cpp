#include "MyDataType.h"

#include <string>
#include <vector>

#include <Windows.h>

#include "resource.h"

extern Json::Value g_ControllerList_Obj;
extern Json::Value g_Controller_Supary;

static DWORD *g_pSingleColorFrameBitmapIDs = NULL;
static DWORD *g_pMultiColorFrameBitmapIDs = NULL;

void CreateFrameArray(int singleColorCount, int multiColorCount)
{
	g_pSingleColorFrameBitmapIDs = new DWORD[singleColorCount];
	for (int i = 0; i < singleColorCount; i++)
	{
		g_pSingleColorFrameBitmapIDs[i] = IDB_SINGLECOLOR_1 + i;
	}

	g_pMultiColorFrameBitmapIDs = new DWORD[multiColorCount];
	for (int i = 0; i < multiColorCount; i++)
	{
		g_pMultiColorFrameBitmapIDs[i] = IDB_MULTICOLOR_1 + i;
	}
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
				DWORD dwSize = SizeofResource(GetModuleHandle(L"LedDynamicArea.dll"), hResource);
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

Json::Value GetControllerObject(const Json::Value &Controller_Supary, int nControllerType, unsigned char &nProtocolVer)
{
	Json::Value Result;
	unsigned char nTypeValue1, nTypeValue2;
	int nDefControllerType;

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

Json::Value GetDefaultController(unsigned char &nProtocolVer, int &nControllerType)
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

int GetControllerType(Json::Value Controller_obj)
{
	return ((Controller_obj["Type_Value_2"].asInt() << 8) & 0xFF00) +
		(Controller_obj["Type_Value_1"].asInt() & 0xFF);
}