#include "stdafx.h"
#include "LedDynamicArea.h"

#include "MyDataType.h"

#pragma comment(lib, "../Debug/jsoncpp.lib")
#pragma comment(lib, "ws2_32.lib")

static CRITICAL_SECTION g_cs;
static WSADATA wsaData;
static SOCKET g_socket;
Json::Value g_ControllerList_Obj;
Json::Value g_Controller_Supary;
static Json::Value devicelist_root;
static Json::Value devicelist_ja;
static vector<PtagSendThread> g_lstSendThread;

static void SaveScreenInfoToFile();

//extern "C" {

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
		for (vector<PtagSendThread>::iterator iter = g_lstSendThread.begin(); iter != g_lstSendThread.end(); ++iter)
		{
			delete *iter;
		}
		g_lstSendThread.clear();
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
		PtagSendThread pNewThread;

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
		newScreen["Net_IP"] = pSocketIP;		newScreen["Net_ConnnectMode"] = NET_CONNECT_STATICIP;		newScreen["Net_ServerMode"] = nServerMode; /* 服务器模式， 1--使能，0-禁止 */		newScreen["Net_ServerIP"] = pServerIP;		newScreen["Net_ServerPort"] = nServerPort;		newScreen["Net_ServerAccessUser"] = pServerAccessUser;		newScreen["Net_ServerAccessPassword"] = pServerAccessPassword;		newScreen["Net_ServerHeartBeatInterval"] = 20; /* 心跳时间间隔（单位：秒）*/		newScreen["Net_mark"] = "255.255.255.0";		newScreen["Net_port"] = nSocketPort;		newScreen["Net_gateway"] = "192.168.0.1";		newScreen["Transit_Barcode"] = pBarcode;		newScreen["Network_ID"] = pNetworkID;		newScreen["Net_mac"] = "";

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

		pNewThread = new tagSendThread;
		if (pNewThread)
		{
			memset(pNewThread, 0, sizeof(tagSendThread));
			pNewThread->szScreenSrc = tmpSrc;
		}
		else
		{
			return RETURN_ERROR_OTHER;
		}

		g_lstSendThread.push_back(pNewThread);

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
		string szProRelate;
		list<string> S;

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
			devicelist_ja[nScreenOrd]["Screen_lstDYArea"][nDYAreaOrd]["DY_AreaFWidth"] =
				GetSelFrameWidth(devicelist_ja[nScreenOrd]["Screen_lstDYArea"][nDYAreaOrd]["DY_AreaFMode"].asUInt(),
					devicelist_ja[nScreenOrd]["Screen_lstDYArea"][nDYAreaOrd]["DY_AreaFLine"].asUInt());
		}
		else if (devicelist_ja[nScreenOrd]["Screen_lstDYArea"][nDYAreaOrd]["DY_AreaFMode"].asInt() == 1)
		{
			if (devicelist_ja[nScreenOrd]["Screen_lstDYArea"][nDYAreaOrd]["DY_AreaFLine"].asInt() >= PROGRAM_FRAME_MULI_COLOR_COUNT)
			{
				devicelist_ja[nScreenOrd]["Screen_lstDYArea"][nDYAreaOrd]["DY_AreaFLine"] = 0;
			}
			devicelist_ja[nScreenOrd]["Screen_lstDYArea"][nDYAreaOrd]["DY_AreaFWidth"] =
				GetSelFrameWidth(devicelist_ja[nScreenOrd]["Screen_lstDYArea"][nDYAreaOrd]["DY_AreaFMode"].asUInt(),
					devicelist_ja[nScreenOrd]["Screen_lstDYArea"][nDYAreaOrd]["DY_AreaFLine"].asUInt());
		}
		else
		{
			devicelist_ja[nScreenOrd]["Screen_lstDYArea"][nDYAreaOrd]["DY_AreaFMode"] = 0xFF;
			devicelist_ja[nScreenOrd]["Screen_lstDYArea"][nDYAreaOrd]["DY_AreaFWidth"] = 0;
		}

		if (nAllProRelate == 0)
		{
			szProRelate = pProRelateList;
			S = SplitString(szProRelate, ",");

			for (list<string>::iterator iter = S.begin(); iter != S.end(); ++iter)
			{
				if (StrIsInt(*iter))
				{
					Json::Value proRelateID;
					proRelateID["RelateProID"] = *iter;
					devicelist_ja[nScreenOrd]["Screen_lstDYArea"][nDYAreaOrd]["DY_ProRelateList"].append(proRelateID);
				}
			}
		}
		else
		{
			devicelist_ja[nScreenOrd]["Screen_lstDYArea"][nDYAreaOrd]["DY_AllProRelate"] = 1;
		}

		return RETURN_NOERROR;
	}

	/*------------------------------------------------------------------------------ -
	过程名:    AddScreenDynamicAreaText
		向动态库中指定显示屏的指定动态区域添加信息文本；该函数不与显示屏通讯。
		参数 :
	nScreenNo：显示屏屏号；该参数与AddScreen_Dynamic函数中的nScreenNo参数对应。
	nDYAreaID：动态区域编号；该参数与AddScreenDynamicArea函数中的nDYAreaID参数对应
	pText：添加的信息文件名称；目前只支持txt(支持ANSI、UTF - 8、Unicode等格式编码)、bmp的文件格式
	nShowSingle：文字信息是否单行显示；0：多行显示；1：单行显示；显示该参数只有szFileName为txt格式文档时才有效；
	pFontName：文字信息显示字体；该参数只有szFileName为txt格式文档时才有效；
	nFontSize：文字信息显示字体的字号；该参数只有szFileName为txt格式文档时才有效；
	nBold：文字信息是否粗体显示；0：正常；1：粗体显示；该参数只有szFileName为txt格式文档时才有效；
	nFontColor：文字信息显示颜色；该参数只有szFileName为txt格式文档时才有效；
	nStunt：动态区域信息运行特技；
	00：随机显示
	01：静止显示
	02：快速打出
	03：向左移动
	04：向左连移
	05：向上移动
	06：向上连移
	07：闪烁
	08：飘雪
	09：冒泡
	10：中间移出
	11：左右移入
	12：左右交叉移入
	13：上下交叉移入
	14：画卷闭合
	15：画卷打开
	16：向左拉伸
	17：向右拉伸
	18：向上拉伸
	19：向下拉伸
	20：向左镭射
	21：向右镭射
	22：向上镭射
	23：向下镭射
	24：左右交叉拉幕
	25：上下交叉拉幕
	26：分散左拉
	27：水平百页
	28：垂直百页
	29：向左拉幕
	30：向右拉幕
	31：向上拉幕
	32：向下拉幕
	33：左右闭合
	34：左右对开
	35：上下闭合
	36：上下对开
	37：向右移动
	38：向右连移
	39：向下移动
	40：向下连移
	nRunSpeed：动态区域信息运行速度
	nShowTime：动态区域信息显示时间；单位：10ms
	返回值 : 详见返回状态代码定义。
		------------------------------------------------------------------------------ - */

	int __stdcall AddScreenDynamicAreaText(int nScreenNo, int nDYAreaID,
		const char *pText, int nShowSingle, const char *pFontName, int nFontSize,
		int nBold, int nFontColor, int nStunt, int nRunSpeed, int nShowTime)
	{
		int nScreenOrd, nDYAreaOrd;
		PtagSendThread ptmptagSendThread;
		string szFileName;
		string szFontName;
		UINT ntmpFileStyle;
		string Ext;
		Json::Value newAreaText;

		nScreenOrd = GetSelScreenArrayOrd(nScreenNo, devicelist_ja);
		if (nScreenOrd == -1)
		{
			return RETURN_ERROR_NOFIND_SCREENNO;
		}

		if ((g_lstSendThread.size() > (size_t)nScreenOrd) && (nScreenOrd >= 0))
		{
			ptmptagSendThread = g_lstSendThread[nScreenOrd];
			if (ptmptagSendThread->bSending)
			{
				return RETURN_ERROR_NOW_SENDING;
			}
		}

		nDYAreaOrd = GetSelScreenDYAreaOrd(nDYAreaID, devicelist_ja[nScreenOrd]["Screen_lstDYArea"]);
		if (nDYAreaOrd == -1)
		{
			return RETURN_ERROR_NOFIND_DYNAMIC_AREA;
		}
		szFileName = pText;
		szFontName = pFontName;
		if (nShowSingle == 1)
			ntmpFileStyle = FILE_SHOWSTYLE_SINGLE;
		else
			ntmpFileStyle = FILE_SHOWSTYLE_NORMAL;

		newAreaText["File_Name"] = szFileName; //文本
		newAreaText["File_Style"] = FILE_TYPE_TEXT; //文本
		newAreaText["File_ShowStyle"] = ntmpFileStyle; //文本显示方式
		newAreaText["File_FontName"] = szFontName; // 字体名称
		newAreaText["File_Bold"] = nBold; //字体是否粗体
		newAreaText["File_FontSize"] = nFontSize; //字体字号
		newAreaText["File_FontColor"] = nFontColor; //字体颜色
		newAreaText["File_PageCount"] = 0; //转换成的数据页
		newAreaText["File_Speed"] = nRunSpeed; //文件速度
		newAreaText["File_Stunt"] = nStunt; //文件特技方式
		newAreaText["File_OutsStunt"] = 0; //退出方式
		newAreaText["File_Showtime"] = nShowTime; //文件停留时间
		newAreaText["File_ShowCount"] = 1; //重复播放次数
		newAreaText["File_Reserved1"] = 0; //保留参数
		newAreaText["File_Reserved2"] = 0; //保留参数
		newAreaText["File_Reserved3"] = 0; //保留参数
		newAreaText["File_Reserved4"] = 0; //保留参数
		newAreaText["File_Reserved5"] = 0; //保留参数
		newAreaText["File_Reserved6"] = 0; //保留参数
		newAreaText["File_Reserved7"] = 0; //保留参数
		devicelist_ja[nScreenOrd]["Screen_lstDYArea"][nDYAreaOrd]["Area_lstfile"].append(newAreaText);

#ifdef _DEBUG
		SaveScreenInfoToFile();
#endif

		return RETURN_NOERROR;
	}

	/*------------------------------------------------------------------------------ -
	过程名:    AddScreenDynamicAreaFile
		向动态库中指定显示屏的指定动态区域添加信息文件；该函数不与显示屏通讯。
		参数 :
	nScreenNo：显示屏屏号；该参数与AddScreen_Dynamic函数中的nScreenNo参数对应。
	nDYAreaID：动态区域编号；该参数与AddScreenDynamicArea函数中的nDYAreaID参数对应
	pFileName：添加的信息文件名称；目前只支持txt(支持ANSI、UTF - 8、Unicode等格式编码)、bmp的文件格式
	nShowSingle：文字信息是否单行显示；0：多行显示；1：单行显示；显示该参数只有szFileName为txt格式文档时才有效；
	pFontName：文字信息显示字体；该参数只有szFileName为txt格式文档时才有效；
	nFontSize：文字信息显示字体的字号；该参数只有szFileName为txt格式文档时才有效；
	nBold：文字信息是否粗体显示；0：正常；1：粗体显示；该参数只有szFileName为txt格式文档时才有效；
	nFontColor：文字信息显示颜色；该参数只有szFileName为txt格式文档时才有效；
	nStunt：动态区域信息运行特技；
	00：随机显示
	01：静止显示
	02：快速打出
	03：向左移动
	04：向左连移
	05：向上移动
	06：向上连移
	07：闪烁
	08：飘雪
	09：冒泡
	10：中间移出
	11：左右移入
	12：左右交叉移入
	13：上下交叉移入
	14：画卷闭合
	15：画卷打开
	16：向左拉伸
	17：向右拉伸
	18：向上拉伸
	19：向下拉伸
	20：向左镭射
	21：向右镭射
	22：向上镭射
	23：向下镭射
	24：左右交叉拉幕
	25：上下交叉拉幕
	26：分散左拉
	27：水平百页
	28：垂直百页
	29：向左拉幕
	30：向右拉幕
	31：向上拉幕
	32：向下拉幕
	33：左右闭合
	34：左右对开
	35：上下闭合
	36：上下对开
	37：向右移动
	38：向右连移
	39：向下移动
	40：向下连移
	nRunSpeed：动态区域信息运行速度
	nShowTime：动态区域信息显示时间；单位：10ms
	返回值 : 详见返回状态代码定义。
		------------------------------------------------------------------------------ - */

	int __stdcall AddScreenDynamicAreaFile(int nScreenNo, int nDYAreaID,
		const char *pFileName, int nShowSingle, const char *pFontName, int nFontSize, int nBold, int nFontColor,
		int nStunt, int nRunSpeed, int nShowTime)
	{
		int nScreenOrd, nDYAreaOrd;
		PtagSendThread ptmptagSendThread;
		string szFileName;
		string szFontName;
		int v6;
		UINT ntmpFileStyle;
		string Ext;
		Json::Value newAreaFile;

		nScreenOrd = GetSelScreenArrayOrd(nScreenNo, devicelist_ja);
		if (nScreenOrd == -1)
		{
			return RETURN_ERROR_NOFIND_SCREENNO;
		}

		if ((g_lstSendThread.size() > (size_t)nScreenOrd) && (nScreenOrd >= 0))
		{
			ptmptagSendThread = g_lstSendThread[nScreenOrd];
			if (ptmptagSendThread->bSending)
			{
				return RETURN_ERROR_NOW_SENDING;
			}
		}

		nDYAreaOrd = GetSelScreenDYAreaOrd(nDYAreaID, devicelist_ja[nScreenOrd]["Screen_lstDYArea"]);
		if (nDYAreaOrd == -1)
		{
			return RETURN_ERROR_NOFIND_DYNAMIC_AREA;
		}
		szFileName = pFileName;
		szFontName = pFontName;

		Ext = szFileName.substr(szFileName.find_last_of('.'));
		transform(Ext.begin(), Ext.end(), Ext.begin(), ::toupper);
		if (Ext == ".BMP") //BMP文件
		{
			v6 = FILE_TYPE_BMP;
			ntmpFileStyle = FILE_SHOWSTYLE_NORMAL;
		}
		else if (Ext == ".TXT") //TXT文件
		{
			v6 = FILE_TYPE_TXT;
			if (nShowSingle == 1)
				ntmpFileStyle = FILE_SHOWSTYLE_SINGLE;
			else
				ntmpFileStyle = FILE_SHOWSTYLE_NORMAL;
		}
		else if (Ext == ".RTF") //RTF文件
		{
			v6 = FILE_TYPE_RTF;
			ntmpFileStyle = FILE_SHOWSTYLE_NORMAL;
		}
		else
		{
			return RETURN_ERROR_NOSUPPORT_FILETYPE;
		}

		newAreaFile["File_Name"] = szFileName; //文件名称
		newAreaFile["File_Style"] = v6; //文件类型  //外部图片，文本，表格，动画 0：图片文本；1：时间类；
		newAreaFile["File_ShowStyle"] = ntmpFileStyle; //文本显示方式
		newAreaFile["File_FontName"] = szFontName; //字体名称
		newAreaFile["File_Bold"] = nBold; //字体是否粗体
		newAreaFile["File_FontSize"] = nFontSize; //字体字号
		newAreaFile["File_FontColor"] = nFontColor; //字体颜色
		newAreaFile["File_PageCount"] = 0; //转换成的数据页
		newAreaFile["File_Speed"] = nRunSpeed; //文件速度
		newAreaFile["File_Stunt"] = nStunt; //文件特技方式
		newAreaFile["File_OutsStunt"] = 0; //退出方式
		newAreaFile["File_Showtime"] = nShowTime; //文件停留时间
		newAreaFile["File_ShowCount"] = 1; //重复播放次数
		newAreaFile["File_Reserved1"] = 0; //保留参数
		newAreaFile["File_Reserved2"] = 0; //保留参数
		newAreaFile["File_Reserved3"] = 0; //保留参数
		newAreaFile["File_Reserved4"] = 0; //保留参数
		newAreaFile["File_Reserved5"] = 0; //保留参数
		newAreaFile["File_Reserved6"] = 0; //保留参数
		newAreaFile["File_Reserved7"] = 0; //保留参数
		devicelist_ja[nScreenOrd]["Screen_lstDYArea"][nDYAreaOrd]["Area_lstfile"].append(newAreaFile);

#ifdef _DEBUG
		SaveScreenInfoToFile();
#endif

		return RETURN_NOERROR;
	}

	/*------------------------------------------------------------------------------ -
	过程名:    DeleteScreen_Dynamic
		删除动态库中指定显示屏的所有信息；该函数不与显示屏通讯。
		参数 :
	nScreenNo：显示屏屏号；该参数与AddScreen_Dynamic函数中的nScreenNo参数对应。
	返回值 : 详见返回状态代码定义
		------------------------------------------------------------------------------ - */

	int __stdcall DeleteScreen_Dynamic(int nScreenNo)
	{
		int nScreenOrd;
		PtagSendThread ptmptagSendThread;
		string v;
		char screenOrdText[8];
		Json::Value removedScreen;

		nScreenOrd = GetSelScreenArrayOrd(nScreenNo, devicelist_ja);
		v = _itoa(nScreenOrd, screenOrdText, 10);
		v += "-";
		if (nScreenOrd == -1)
		{
			return RETURN_ERROR_NOFIND_SCREENNO;
		}

		if ((g_lstSendThread.size() > (size_t)nScreenOrd) && (nScreenOrd >= 0))
		{
			ptmptagSendThread = g_lstSendThread[nScreenOrd];
			if (ptmptagSendThread->bSending)
			{
				return RETURN_ERROR_NOW_SENDING;
			}
		}

		devicelist_ja.removeIndex(nScreenOrd, &removedScreen);
		return RETURN_NOERROR;
	}

	/*------------------------------------------------------------------------------ -
	过程名:    DeleteScreenDynamicArea
		删除动态库中指定显示屏指定的动态区域信息；该函数不与显示屏通讯
		参数 :
	nScreenNo：显示屏屏号；该参数与AddScreen_Dynamic函数中的nScreenNo参数对应。
	nDYAreaID：动态区域编号；该参数与AddScreenDynamicArea函数中的nDYAreaID参数对应
	返回值 : 详见返回状态代码定义
		------------------------------------------------------------------------------ - */

	int __stdcall DeleteScreenDynamicArea(int nScreenNo, int nDYAreaID)
	{
		int nScreenOrd, nDYAreaOrd;
		PtagSendThread ptmptagSendThread;
		Json::Value removedScreenDynamicArea;

		nScreenOrd = GetSelScreenArrayOrd(nScreenNo, devicelist_ja);
		if (nScreenOrd == -1)
		{
			return RETURN_ERROR_NOFIND_SCREENNO;
		}

		if ((g_lstSendThread.size() > (size_t)nScreenOrd) && (nScreenOrd >= 0))
		{
			ptmptagSendThread = g_lstSendThread[nScreenOrd];
			if (ptmptagSendThread->bSending)
			{
				return RETURN_ERROR_NOW_SENDING;
			}
		}

		nDYAreaOrd = GetSelScreenDYAreaOrd(nDYAreaID, devicelist_ja[nScreenOrd]["Screen_lstDYArea"]);
		if (nDYAreaOrd == -1)
		{
			return RETURN_ERROR_NOFIND_DYNAMIC_AREA;
		}

		devicelist_ja[nScreenOrd]["Screen_lstDYArea"].removeIndex(nDYAreaOrd, &removedScreenDynamicArea);
		return RETURN_NOERROR;
	}

	/*------------------------------------------------------------------------------ -
	过程名:    DeleteScreenDynamicAreaFile
		删除动态库中指定显示屏指定的动态区域指定文件信息；该函数不与显示屏通讯。
		参数 :
	nScreenNo：显示屏屏号；该参数与AddScreen_Dynamic函数中的nScreenNo参数对应。
	nDYAreaID：动态区域编号；该参数与AddScreenDynamicArea函数中的nDYAreaID参数对应
	nFileOrd：动态区域的指定文件的文件序号；该序号按照文件添加顺序，从0顺序递增，如删除中间的文件，后面的文件序号自动填充。
	返回值 : 详见返回状态代码定义
		------------------------------------------------------------------------------ - */

	int __stdcall DeleteScreenDynamicAreaFile(int nScreenNo, int nDYAreaID, int nFileOrd)
	{
		int nScreenOrd, nDYAreaOrd;
		PtagSendThread ptmptagSendThread;
		Json::Value removedScreenDynamicAreaFile;

		nScreenOrd = GetSelScreenArrayOrd(nScreenNo, devicelist_ja);
		if (nScreenOrd == -1)
		{
			return RETURN_ERROR_NOFIND_SCREENNO;
		}

		if ((g_lstSendThread.size() > (size_t)nScreenOrd) && (nScreenOrd >= 0))
		{
			ptmptagSendThread = g_lstSendThread[nScreenOrd];
			if (ptmptagSendThread->bSending)
			{
				return RETURN_ERROR_NOW_SENDING;
			}
		}

		nDYAreaOrd = GetSelScreenDYAreaOrd(nDYAreaID, devicelist_ja[nScreenOrd]["Screen_lstDYArea"]);
		if (nDYAreaOrd == -1)
		{
			return RETURN_ERROR_NOFIND_DYNAMIC_AREA;
		}
		if ((size_t)nFileOrd >= devicelist_ja[nScreenOrd]["Screen_lstDYArea"][nDYAreaOrd]["Area_lstfile"].size())
		{
			return RETURN_ERROR_NOFIND_AREA;
		}
		devicelist_ja[nScreenOrd]["Screen_lstDYArea"][nDYAreaOrd]["Area_lstfile"].removeIndex(nFileOrd, &removedScreenDynamicAreaFile);
		return RETURN_NOERROR;
	}

	/*------------------------------------------------------------------------------ -
	过程名:    SendDynamicAreaInfoCommand
		发送动态库中指定显示屏指定的动态区域信息到显示屏；该函数与显示屏通讯。
		参数 :
	nScreenNo：显示屏屏号；该参数与AddScreen_Dynamic函数中的nScreenNo参数对应。
	nDYAreaID：动态区域编号；该参数与AddScreenDynamicArea函数中的nDYAreaID参数对应
	返回值 : 详见返回状态代码定义
		------------------------------------------------------------------------------ - */

	int __stdcall SendDynamicAreaInfoCommand(int nScreenNo, int nDYAreaID)
	{
		return RETURN_NOERROR;
	}
	//	int nScreenOrd, nDYAreaOrd;
	//	PtagSendThread ptmptagSendThread;
	//	string szSendBuf;
	//	UINT nSendLength, ntmpSendCmd, ntmpSendFileType;
	//	tagstruct_PHY1Header srt_PHY1Header;
	//	//lstSendPrograms: TList;
	//	ofstream fp;
	//	int Result;
	//
	//	__try
	//	{
	//		Result = RETURN_ERROR_OTHER; //其它错误
	//		EnterCriticalSection(&g_cs);
	//		__try
	//		{
	//			nScreenOrd = GetSelScreenArrayOrd(nScreenNo, devicelist_ja);
	//			if (nScreenOrd == -1)
	//			{
	//				return RETURN_ERROR_NOFIND_SCREENNO;
	//			}
	//
	//			if (g_lstSendThread.size() <= nScreenOrd)
	//			{
	//				return RETURN_ERROR_OTHER;
	//			}
	//			
	//		ptmptagSendThread = g_lstSendThread[nScreenOrd];
	//		if (ptmptagSendThread->bSending)
	//		{
	//			return RETURN_ERROR_NOW_SENDING;
	//		}
	//
	//		nDYAreaOrd = GetSelScreenDYAreaOrd(nDYAreaID, devicelist_ja[nScreenOrd]["Screen_lstDYArea"]);
	//		if (nDYAreaOrd == -1)
	//		{
	//			return RETURN_ERROR_NOFIND_DYNAMIC_AREA;
	//		}
	//		
	//		szSendBuf = MakeDynamicAreaInfo(nScreenOrd, nDYAreaOrd, devicelist_ja, nSendLength);
	//
	//		ntmpSendCmd = SEND_DYNAMIC_AREA_INFO;
	//		ntmpSendFileType = 0xFF;
	//
	//		memset(&srt_PHY1Header, 0, sizeof(srt_PHY1Header));
	//		srt_PHY1Header.DstAddr = 1;
	//		srt_PHY1Header.SrcAddr = 32768;
	//		srt_PHY1Header.ProtocolVer = PROTOCOLVER_FIFTH_DYNAMIC;
	//		srt_PHY1Header.Datalen = 1024;
	//
	//		switch (devicelist_ja[nScreenOrd]["Screen_SendMode"].asUInt())
	//		{
	//		case SEND_MODE_NETWORK:
	//		case SEND_MODE_Server_2G:
	//		case SEND_MODE_Server_3G:
	//			srt_PHY1Header.DstAddr = CONTROLLER_ADDRESS_WILDCARD;
	//			break;
	//		case SEND_MODE_SAVEFILE:
	//			{
	//				ofstream ofs;
	//				ofs.open(devicelist_ja[nScreenOrd]["Screen_CommandDataFile"].asString());
	//				ofs.close();
	//			}
	//			break;
	//		default:
	//			break;
	//		}
	//
	//		if devicelist_ja[nScreenOrd]["Screen_SendMode"]
	//in[SEND_MODE_NET, SEND_MODE_GPRS, SEND_MODE_WIFI, SEND_MODE_Server_2G, SEND_MODE_Server_3G] then
	//srt_PHY1Header.DstAddr : = CONTROLLER_ADDRESS_WILDCARD
	//else if devicelist_ja[nScreenOrd].I['Screen_SendMode'] = SEND_MODE_SAVEFILE then
	//begin
	//try
	//AssignFile(fp, devicelist_ja[nScreenOrd].S['Screen_CommandDataFile']);
	//Rewrite(fp, 1);
	//BlockWrite(fp, szSendBuf[1], Length(szSendBuf));
	//finally
	//CloseFile(fp);
	//Result: = RETURN_NOERROR;
	//end;
	//Exit;
	//end
	//else
	//srt_PHY1Header.DstAddr : = devicelist_ja[nScreenOrd].I['Com_address'];
	//srt_PHY1Header.DeviceType : = devicelist_ja[nScreenOrd].I['Screen_control'];
	//finally
	//LeaveCriticalSection(CS);
	//end;
	//ptmptagSendThread.bSending : = true;
	//try
	//Result : = CreateSendThreadList(SEND_SINGLE_COMMUNICATION, $FF, nScreenOrd, Application.Handle
	//	, ptmptagSendThread, devicelist_ja, devicelist_ja[nScreenOrd].I['Screen_SendMode'],
	//	srt_PHY1Header, ntmpSendCmd, ntmpSendFileType, '', szSendBuf, nSendLength, lstSendPrograms, Application.Handle, False, '');
	//finally
	//ptmptagSendThread.bSending : = False;
	//end;
	//except
	//end
	//end;


	/*------------------------------------------------------------------------------ -
	过程名:    SendDeleteDynamicAreasCommand
		删除动态库中指定显示屏指定的动态区域信息；同时向显示屏通讯删除指定的动态区域信息。该函数与显示屏通讯
		参数 :
	nScreenNo：显示屏屏号；该参数与AddScreen_Dynamic函数中的nScreenNo参数对应。
	nDYAreaID：动态区域编号；该参数与AddScreenDynamicArea函数中的nDYAreaID参数对应
	pDYAreaIDList：动态区域编号列表；如果同时删除多个动态区域，动态区域编号间用","隔开。如"0,1"
	返回值 : 详见返回状态代码定义
		------------------------------------------------------------------------------ - */

	int __stdcall SendDeleteDynamicAreasCommand(int nScreenNo, int nDelAllDYArea, char *pDYAreaIDList)
	{
		return RETURN_NOERROR;
	}

	/*------------------------------------------------------------------------------ -
	过程名:    SendUpdateDynamicAreaPageInfoCommand
		向动态库中指定显示屏指定的动态区域单独更新指定的数据页信息；该函数与显示屏通讯
		该函数使用时，必须能明确好当前预更新信息在显示屏中的显示页码，否则可能更新出错。
		参数 :
	nScreenNo：显示屏屏号；该参数与AddScreen_Dynamic函数中的nScreenNo参数对应。
	nDYAreaID：动态区域编号；该参数与AddScreenDynamicArea函数中的nDYAreaID参数对应
	nFileOrd：动态区域的指定文件的文件序号；该序号按照文件添加顺序，从0顺序递增，如删除中间的文件，后面的文件序号自动填充。
	nPageOrd：预更新数据页的页序号。从0开始顺序递增。
	返回值 : 详见返回状态代码定义
		------------------------------------------------------------------------------ - */
	int __stdcall SendUpdateDynamicAreaPageInfoCommand(int nScreenNo, int nDYAreaID, int nFileOrd, int nPageOrd)
	{
		return RETURN_NOERROR;
	}

	/*------------------------------------------------------------------------------ -
	过程名:    SendDeleteDynamicAreaPageCommand
		删除动态库中指定显示屏的指定动态区域指定的数据页信息；该函数与显示屏通讯
		该函数使用时，必须能明确好当前预删除数据页信息在显示屏中的显示页码，否则可能删除出错。
		参数 :
	nScreenNo：显示屏屏号；该参数与AddScreen_Dynamic函数中的nScreenNo参数对应。
	nDYAreaID：动态区域编号；该参数与AddScreenDynamicArea函数中的nDYAreaID参数对应
	pDYAreaPageOrdList：预删除动态区域数据页序号列表；如果同时删除多个数据页信息，数据页序号间用","隔开。如"0,1"；
	删除数据页信息执行成功后，后面的数据页信息的数据页序号自动向前对齐。
	返回值 : 详见返回状态代码定义
		------------------------------------------------------------------------------ - */

	int __stdcall SendDeleteDynamicAreaPageCommand(int nScreenNo, int nDYAreaID, char *pDYAreaPageOrdList)
	{
		return RETURN_NOERROR;
	}

	/*------------------------------------------------------------------------------ -
	过程名:    StartServer
		启动服务器, 用于网络模式下的服务器模式和GPRS通讯模式。
		参数 :
	nSendMode : 与显示屏的通讯模式；
		0 : 串口通讯；
		2：网络通讯(只支持固定IP模式)；
		5：ONBON服务器 - GPRS
		6：ONBON服务器 - 3G
		7：保存到文件等三种通讯模式。
		pServerIP : 服务器IP地址
		nServerPort : 服务器网络端口
		返回值 : 详见返回状态代码定义。
		------------------------------------------------------------------------------ - */
	int __stdcall StartServer(int nSendMode, char *pServerIP, int nServerPort)
	{
		return RETURN_NOERROR;
	}

	/*------------------------------------------------------------------------------ -
	过程名:    StopServer
		关闭服务器, 用于网络模式下的服务器模式和GPRS通讯模式。
		参数 :
	nSendMode : 与显示屏的通讯模式；
		0 : 串口通讯；
		2：网络通讯(只支持固定IP模式)；
		5：ONBON服务器 - GPRS
		6：ONBON服务器 - 3G
		7：保存到文件等三种通讯模式。
		返回值 : 详见返回状态代码定义。
		------------------------------------------------------------------------------ - */
	int __stdcall StopServer(int nSendMode)
	{
		return RETURN_NOERROR;
	}

//}

static void SaveScreenInfoToFile()
{
	Json::FastWriter writer;
	string jsonFileContent = writer.write(devicelist_root);
	ofstream ofs;
	ofs.open("screenlist.json");
	ofs << jsonFileContent;
	ofs.close();
}