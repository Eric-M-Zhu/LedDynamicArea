#include "LedDynamicArea.h"

#include <ctime>
#include <windows.h>

#include "MyDataType.h"
#include "../jsoncpp/json.h"

#pragma comment(lib, "../Debug/jsoncpp.lib")
#pragma comment(lib, "ws2_32.lib")

static CRITICAL_SECTION g_cs;
static WSADATA wsaData;
Json::Value g_ControllerList_Obj;
Json::Value g_Controller_Supary;
static Json::Value devicelist_root;
static Json::Value devicelist_ja;

/*------------------------------------------------------------------------------ -
过程名:    Initialize
	初始化动态库；该函数不与显示屏通讯。
参数 :
返回值 : 详见返回状态代码定义。
------------------------------------------------------------------------------ -*/
int __stdcall Initialize()
{
	Json::Reader reader;

	InitializeCriticalSection(&g_cs);
	CreateFrameArray(PROGRAM_FRAME_SINGLE_COLOR_COUNT, PROGRAM_FRAME_MULI_COLOR_COUNT);
	// TODO: GetCRC16ByteArray
	// TODO: Create send threads list
	WSAStartup(MAKEWORD(1, 1), &wsaData);
	GetControllerList(g_ControllerList_Obj, g_Controller_Supary);

	reader.parse("{\"ScreenList\"] = []}", devicelist_root, false);
	devicelist_ja = devicelist_root["ScreenList"];

	return RETURN_NOERROR;
}

/*------------------------------------------------------------------------------ -
过程名:    Uninitialize
	释放动态库；该函数不与显示屏通讯。
参数 :
返回值 : 详见返回状态代码定义。
------------------------------------------------------------------------------ -*/
int __stdcall Uninitialize()
{
	WSACleanup();
	// TODO: Free send threads in list
	DeleteCriticalSection(&g_cs);
	return RETURN_NOERROR;
}

/*------------------------------------------------------------------------------ -
过程名:    AddScreen_Dynamic
	向动态库中添加显示屏信息；该函数不与显示屏通讯。
参数 :
	nControlType : 显示屏的控制器型号，目前该动态区域动态库只支持BX - 5E1、BX - 5E2、BX - 5E3等BX - 5E系列和5Q + 系列控制器。
	nScreenNo：显示屏屏号；该参数与LedshowTW 2013软件中"设置屏参"模块的"屏号"参数一致。
	nSendMode：通讯模式；目前动态库中支持0 : 串口通讯；2：网络通讯；5：保存到文件等三种通讯模式。
	nWidth：显示屏宽度；单位：像素。
	nHeight：显示屏高度；单位：像素。
	nScreenType：显示屏类型；1：单基色；2：双基色; 3:双基色 4 : 全彩色 5 : 双基色灰度。
	nPixelMode：点阵类型，只有显示屏类型为双基色时有效；1：R + G；2：G + R。
	pCom：通讯串口，只有在串口通讯时该参数有效；例："COM1"
	nBaud：通讯串口波特率，只有在串口通讯时该参数有效；目前只支持9600、57600两种波特率。
	pSocketIP：控制器的IP地址；只有在网络通讯模式下该参数有效，例："192.168.0.199"
	nSocketPort：控制器的端口地址；只有在网络通讯模式下该参数有效，例：5005
	nServerMode : 0 : 服务器模式未启动；1：服务器模式启动。
	pBarcode : 设备条形码
	pNetworkID : 网络ID编号
	pServerIP : 中转服务器IP地址
	nServerPort : 中转服务器网络端口
	pServerAccessUser : 中转服务器访问用户名
	pServerAccessPassword : 中转服务器访问密码
	pCommandDataFile：保存到文件方式时，命令保存命令文件名称。只有在保存到文件模式下该参数有效，例："curCommandData.dat"
返回值 : 详见返回状态代码定义。
------------------------------------------------------------------------------ - */
int __stdcall AddScreen_Dynamic(int nControlType, int nScreenNo, int nSendMode,
	int nWidth, int nHeight, int nScreenType, int nPixelMode, const char *pCom, int nBaud, const char *pSocketIP, int nSocketPort,
	int nServerMode, const char *pBarcode, const char *pNetworkID, const char *pServerIP, int nServerPort,
	const char *pServerAccessUser, const char *pServerAccessPassword, const char *pCommandDataFile)
{
	int nScreenOrd = GetSelScreenArrayOrd(nScreenNo, devicelist_ja);
	int nEncryptionType;
	SYSTEMTIME now;
	char tmpSrc[16];
	Json::Value Controller_obj;
	Json::Value newScreen;
	unsigned char nProtocolVer;
	
	if (nScreenOrd >= 0)
	{
		return RETURN_ERROR_RA_SCREENNO;
	}

#ifdef _DEBUG
	nEncryptionType = ENCRYPTION_NONE;
#else // _DEBUG
	nEncryptionType = ENCRYPTION_STATIC;
#endif // _DEBUG

	GetLocalTime(&now);
	sprintf(tmpSrc, "SC%02d%02d%02d%02d%02d%03d", now.wMonth,
		now.wDay,
		now.wHour,
		now.wMinute,
		now.wSecond,
		now.wMilliseconds);
	Controller_obj = GetControllerObject(g_Controller_Supary, nControlType, nProtocolVer);

	newScreen["Screen_src"] = tmpSrc;
	newScreen["Screen_CommandDataFile"] = pCommandDataFile;
	newScreen["Screen_Reality_Memory"] = MEMORY_REALITY_BX_4A;
	newScreen["Screen_control"] = nControlType; //控制组件类型 串口 网口 GPRS
	newScreen["Screen_SendMode"] = nSendMode; //通讯模式
	newScreen["Screen_ProtocolVer"] = nProtocolVer;
	newScreen["Screen_Width"] = nWidth; //宽度
	newScreen["Screen_Height"] = nHeight; //高度
	newScreen["Screen_color"] = nScreenType; //颜色
	newScreen["Screen_pixels"] = nPixelMode; //点阵
	newScreen["Screen_nPixType"] = 8; //横向长度递增单位 默认为8
	newScreen["Screen_ReadWaitTimeOut"] = WAIT_TIMEOUT_SERIALPORT + 2; //加2保证控制卡的黑屏事件大于控制卡时间溢出最大停留时间

	//串口的参数
	newScreen["Com_port"] = atoi(pCom + 3);
	newScreen["Com_baudrate"] = nBaud;
	newScreen["Com_address"] = nScreenNo;

	//GPRS的参数
	newScreen["Gprs_style"] = 0;
	newScreen["Gprs_ip"] = "192.168.0.235";
	newScreen["Gprs_id"] = "BX-GP000000";
	newScreen["Gprs_sim"] = "00000000000";
	newScreen["Gprs_port"] = 8000;
	newScreen["Gprs_level"] = 0;
	newScreen["Gprs_Unitsize"] = 900;
	newScreen["Gprs_Unittime"] = 2000;
	
	//网口的参数
	newScreen["Net_IP"] = pSocketIP;	newScreen["Net_ConnnectMode"] = NET_CONNECT_STATICIP;	newScreen["Net_ServerMode"] = nServerMode; /* 服务器模式， 1--使能，0-禁止 */	newScreen["Net_ServerIP"] = pServerIP;	newScreen["Net_ServerPort"] = nServerPort;	newScreen["Net_ServerAccessUser"] = pServerAccessUser;	newScreen["Net_ServerAccessPassword"] = pServerAccessPassword;	newScreen["Net_ServerHeartBeatInterval"] = 20; /* 心跳时间间隔（单位：秒）*/	newScreen["Net_mark"] = "255.255.255.0";	newScreen["Net_port"] = nSocketPort;	newScreen["Net_gateway"] = "192.168.0.1";	newScreen["Transit_Barcode"] = pBarcode;	newScreen["Network_ID"] = pNetworkID;	newScreen["Net_mac"] = "";

	newScreen["Screen_Light"] = 15; //亮度
	newScreen["Screen_AdjustLightMode"] = 0; //亮度调整模式

	newScreen["Screen_LightTime1Use"] = false;
	newScreen["Screen_LightTime1"] = "06:30";
	newScreen["Screen_LightValue1"] = 15;
	newScreen["Screen_LightTime2Use"] = false;
	newScreen["Screen_LightTime2"] = "10:30";
	newScreen["Screen_LightValue2"] = 15;
	newScreen["Screen_LightTime3Use"] = false;
	newScreen["Screen_LightTime3"] = "12:30";
	newScreen["Screen_LightValue3"] = 15;
	newScreen["Screen_LightTime4Use"] = false;
	newScreen["Screen_LightTime4"] = "17:30";
	newScreen["Screen_LightValue4"] = 15;

	newScreen["Screen_Power"] = 0;
	newScreen["Screen_Powerstate"] = 0;
	newScreen["Screen_PowertimeEnabled"] = false; //定时开关机是否有效。
	newScreen["Screen_Powertime1use"] = false;
	newScreen["Screen_Powertime1on"] = "07:00";
	newScreen["Screen_Powertime1off"] = "07:00";
	newScreen["Screen_Powertime2use"] = false;
	newScreen["Screen_Powertime2on"] = "07:00";
	newScreen["Screen_Powertime2off"] = "07:00";
	newScreen["Screen_Powertime3use"] = false;
	newScreen["Screen_Powertime3on"] = "07:00";
	newScreen["Screen_Powertime3off"] = "07:00";
	newScreen["Screen_PA"] = nEncryptionType; //控制器加密方式；调试阶段设定为明码模式；发布时调整为固定密码模式
	newScreen["Screen_PA1"] = ""; //控制器旧密码
	newScreen["Screen_PA2"] = ""; //控制器新密码
	newScreen["Screen_Reserved1"] = 0; //8个保留参数，4个整型，4个字符串型
	newScreen["Screen_Reserved2"] = 0;
	newScreen["Screen_Reserved3"] = 0;
	newScreen["Screen_Reserved4"] = 0;
	newScreen["Screen_Reserved5"] = "";
	newScreen["Screen_Reserved6"] = "";
	newScreen["Screen_Reserved7"] = "";
	newScreen["Screen_Reserved8"] = "";
	newScreen["Screen_lstDYArea"] = Json::arrayValue;
	devicelist_ja.append(newScreen);
	// TODO: new thread
	return RETURN_NOERROR;
}