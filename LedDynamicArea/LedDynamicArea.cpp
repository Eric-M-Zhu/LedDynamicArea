#include "LedDynamicArea.h"

#include <windows.h>

#include "MyDataType.h"
#include "../jsoncpp/json.h"

#pragma comment(lib, "../Debug/jsoncpp.lib")
#pragma comment(lib, "ws2_32.lib")

static CRITICAL_SECTION g_cs;
static WSADATA wsaData;
static Json::Value g_ControllerList_Obj;
static Json::Value g_Controller_Supary;
static Json::Value devicelist_root;
static Json::Value devicelist_ja;

int __stdcall Initialize()
{
	Json::Reader reader;

	InitializeCriticalSection(&g_cs);
	CreateFrameArray(PROGRAM_FRAME_SINGLE_COLOR_COUNT, PROGRAM_FRAME_MULI_COLOR_COUNT);
	// TODO: GetCRC16ByteArray
	// TODO: Create send threads list
	WSAStartup(MAKEWORD(1, 1), &wsaData);
	GetControllerList(g_ControllerList_Obj, g_Controller_Supary);

	reader.parse("{\"ScreenList\":[]}", devicelist_root, false);
	devicelist_ja = devicelist_root["ScreenList"];

	return RETURN_NOERROR;
}

int __stdcall Uninitialize()
{
	WSACleanup();
	// TODO: Free send threads in list
	DeleteCriticalSection(&g_cs);
	return RETURN_NOERROR;                                                                        
}                                                                                                                    