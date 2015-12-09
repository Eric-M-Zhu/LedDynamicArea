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

/*------------------------------------------------------------------------------ -
过程名:    AddScreenDynamicArea
	向动态库中指定显示屏添加动态区域；该函数不与显示屏通讯。
	参数 :
nScreenNo：显示屏屏号；该参数与AddScreen_Dynamic函数中的nScreenNo参数对应。
nDYAreaID：动态区域编号；目前系统中最多5个动态区域；该值取值范围为0~4;
nRunMode：动态区域运行模式：
0:动态区数据循环显示；
1 : 动态区数据显示完成后静止显示最后一页数据；
2 : 动态区数据循环显示，超过设定时间后数据仍未更新时不再显示；
3 : 动态区数据循环显示，超过设定时间后数据仍未更新时显示Logo信息, Logo 信息即为动态区域的最后一页信息
4 : 动态区数据顺序显示，显示完最后一页后就不再显示
nTimeOut：动态区数据超时时间；单位：秒
nAllProRelate：节目关联标志；
1：所有节目都显示该动态区域；
0：在指定节目中显示该动态区域，如果动态区域要独立于节目显示，则下一个参数为空。
pProRelateList：节目关联列表，用节目编号表示；节目编号间用","隔开, 节目编号定义为LedshowTW 2013软件中"P***"中的"***"
nPlayPriority：动态区域播放优先级；
0：该动态区域与异步节目一起播放(动态区域有关联异步节目才有效)；
1：异步节目停止播放，仅播放该动态区域
nAreaX：动态区域起始横坐标；单位：像素
nAreaY：动态区域起始纵坐标；单位：像素
nAreaWidth：动态区域宽度；单位：像素
nAreaHeight：动态区域高度；单位：像素
nAreaFMode：动态区域边框显示标志；0：纯色；1：花色；255：无边框
nAreaFLine：动态区域边框类型, 纯色最大取值为FRAME_SINGLE_COLOR_COUNT；花色最大取值为：FRAME_MULI_COLOR_COUNT
nAreaFColor：边框显示颜色；选择为纯色边框类型时该参数有效；
nAreaFStunt：边框运行特技；
0：闪烁；1：顺时针转动；2：逆时针转动；3：闪烁加顺时针转动；
4:闪烁加逆时针转动；5：红绿交替闪烁；6：红绿交替转动；7：静止打出
nAreaFRunSpeed：边框运行速度；
nAreaFMoveStep：边框移动步长；该值取值范围：1~8；
返回值 : 详见返回状态代码定义。
------------------------------------------------------------------------------ - */

int __stdcall AddScreenDynamicArea(int nScreenNo, int nDYAreaID, int nRunMode, int nTimeOut, int nAllProRelate,
	const char *pProRelateList, int nPlayPriority, int nAreaX, int nAreaY, int nAreaWidth, int nAreaHeight,
	int nAreaFMode, int nAreaFLine, int nAreaFColor, int nAreaFStunt, int nAreaFRunSpeed, int nAreaFMoveStep)
{
	int nScreenOrd = GetSelScreenArrayOrd(nScreenNo, devicelist_ja);
	int nDYAreaOrd;
	Json::Value newDynamicArea;

	if (nScreenOrd < 0)
	{
		return RETURN_ERROR_NOFIND_SCREENNO;
	}

	// TODO: Check if is sending

	newDynamicArea["DY_AreaID"] = nDYAreaID;
	newDynamicArea["DY_RunMode"] = nRunMode;
	newDynamicArea["DY_TimeOut"] = nTimeOut;
	newDynamicArea["DY_AllProRelate"] = nAllProRelate;
	newDynamicArea["DY_ProRelateList"] = Json::arrayValue;
	newDynamicArea["DY_PlayImmediately"] = nPlayPriority;
	newDynamicArea["DY_AreaX"] = nAreaX;
	newDynamicArea["DY_AreaY"] = nAreaY;
	newDynamicArea["DY_AreaWidth"] = nAreaWidth;
	newDynamicArea["DY_AreaHeight"] = nAreaHeight;
	newDynamicArea["DY_AreaFStunt"] = nAreaFStunt;
	newDynamicArea["DY_AreaFRunSpeed"] = nAreaFRunSpeed;
	newDynamicArea["DY_AreaFMoveStep"] = nAreaFMoveStep;
	newDynamicArea["DY_AreaFMode"] = nAreaFMode; //0:纯色；1：花色；$FF:没有边框
	newDynamicArea["DY_AreaFWidth"] = 1;
	newDynamicArea["DY_AreaFLine"] = nAreaFLine;
	newDynamicArea["DY_AreaFColor"] = nAreaFColor;
	newDynamicArea["Area_lstfile"] = Json::arrayValue; //区域的文件内容
	devicelist_ja[nScreenOrd]["Screen_lstDYArea"].append(newDynamicArea);
	nDYAreaOrd = devicelist_ja[nScreenOrd]["Screen_lstDYArea"].size() - 1;

	if (devicelist_ja[nScreenOrd]["Screen_lstDYArea"][nDYAreaOrd]["DY_AreaFMode"].asInt() == 0)
	{
		if (devicelist_ja[nScreenOrd]["Screen_lstDYArea"][nDYAreaOrd]["DY_AreaFLine"].asInt() >= PROGRAM_FRAME_SINGLE_COLOR_COUNT)
		{
			devicelist_ja[nScreenOrd]["Screen_lstDYArea"][nDYAreaOrd]["DY_AreaFLine"] = 0;
		}
		/*devicelist_ja[nScreenOrd].A['Screen_lstDYArea'][nDYAreaOrd].I['DY_AreaFWidth'] : =
			GetSelFrameWidth(devicelist_ja[nScreenOrd].A['Screen_lstDYArea'][nDYAreaOrd].I['DY_AreaFMode']
				, devicelist_ja[nScreenOrd].A['Screen_lstDYArea'][nDYAreaOrd].I['DY_AreaFLine']);*/
	}
	else if (devicelist_ja[nScreenOrd]["Screen_lstDYArea"][nDYAreaOrd]["DY_AreaFMode"].asInt() == 1)
	{
		if (devicelist_ja[nScreenOrd]["Screen_lstDYArea"][nDYAreaOrd]["DY_AreaFLine"].asInt() >= PROGRAM_FRAME_MULI_COLOR_COUNT)
		{
			devicelist_ja[nScreenOrd]["Screen_lstDYArea"][nDYAreaOrd]["DY_AreaFLine"] = 0;
		}
		/*devicelist_ja[nScreenOrd].A['Screen_lstDYArea'][nDYAreaOrd].I['DY_AreaFWidth'] : =
			GetSelFrameWidth(devicelist_ja[nScreenOrd].A['Screen_lstDYArea'][nDYAreaOrd].I['DY_AreaFMode']
				, devicelist_ja[nScreenOrd].A['Screen_lstDYArea'][nDYAreaOrd].I['DY_AreaFLine']);*/
	}
	else
	{
		devicelist_ja[nScreenOrd]["Screen_lstDYArea"][nDYAreaOrd]["DY_AreaFMode"] = 0xFF;
		devicelist_ja[nScreenOrd]["Screen_lstDYArea"][nDYAreaOrd]["DY_AreaFWidth"] = 0;
	}

	if (nAllProRelate == 0)
	{
		char *pszProRelate = new char[strlen(pProRelateList)];

		if (pszProRelate)
		{
			char *pToken = NULL;

			strcpy(pszProRelate, pProRelateList);
			pToken = strtok(pszProRelate, ",");
			while (pToken)
			{
				Json::Value proRelateID;
				proRelateID["RelateProID"] = atoi(pToken);
				devicelist_ja[nScreenOrd]["Screen_lstDYArea"][nDYAreaOrd]["DY_ProRelateList"].append(proRelateID);
				pToken = strtok(NULL, ",");
			}
			delete[] pszProRelate;
		}
		else
		{
			devicelist_ja[nScreenOrd]["Screen_lstDYArea"][nDYAreaOrd]["DY_AllProRelate"] = 1;
		}
	}
	else
	{
		devicelist_ja[nScreenOrd]["Screen_lstDYArea"][nDYAreaOrd]["DY_AllProRelate"] = 1;
	}

	return RETURN_NOERROR;
}