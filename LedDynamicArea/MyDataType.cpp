#include "MyDataType.h"

#include <string>
#include <vector>

#include <Windows.h>

#include "resource.h"

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