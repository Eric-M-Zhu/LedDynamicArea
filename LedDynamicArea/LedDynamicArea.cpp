#include "LedDynamicArea.h"

#include <ctime>
#include <windows.h>

#include "MyDataType.h"
#include "../jsoncpp/json.h"

#include <vector>
#include <algorithm>

#pragma comment(lib, "../Debug/jsoncpp.lib")
#pragma comment(lib, "ws2_32.lib")

static CRITICAL_SECTION g_cs;
static WSADATA wsaData;
Json::Value g_ControllerList_Obj;
Json::Value g_Controller_Supary;
static Json::Value devicelist_root;
static Json::Value devicelist_ja;
static std::vector<PtagSendThread> g_lstSendThread;

static void SaveScreenInfoToFile();

/*------------------------------------------------------------------------------ -
������:    Initialize
	��ʼ����̬�⣻�ú���������ʾ��ͨѶ��
���� :
����ֵ : �������״̬���붨�塣
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
������:    Uninitialize
	�ͷŶ�̬�⣻�ú���������ʾ��ͨѶ��
���� :
����ֵ : �������״̬���붨�塣
------------------------------------------------------------------------------ -*/
int __stdcall Uninitialize()
{
	WSACleanup();
	for (std::vector<PtagSendThread>::iterator iter = g_lstSendThread.begin(); iter != g_lstSendThread.end(); ++iter)
	{
		delete *iter;
	}
	g_lstSendThread.clear();
	DeleteCriticalSection(&g_cs);
	return RETURN_NOERROR;
}

/*------------------------------------------------------------------------------ -
������:    AddScreen_Dynamic
	��̬���������ʾ����Ϣ���ú���������ʾ��ͨѶ��
���� :
	nControlType : ��ʾ���Ŀ������ͺţ�Ŀǰ�ö�̬����̬��ֻ֧��BX - 5E1��BX - 5E2��BX - 5E3��BX - 5Eϵ�к�5Q + ϵ�п�������
	nScreenNo����ʾ�����ţ��ò�����LedshowTW 2013�����"��������"ģ���"����"����һ�¡�
	nSendMode��ͨѶģʽ��Ŀǰ��̬����֧��0 : ����ͨѶ��2������ͨѶ��5�����浽�ļ�������ͨѶģʽ��
	nWidth����ʾ����ȣ���λ�����ء�
	nHeight����ʾ���߶ȣ���λ�����ء�
	nScreenType����ʾ�����ͣ�1������ɫ��2��˫��ɫ; 3:˫��ɫ 4 : ȫ��ɫ 5 : ˫��ɫ�Ҷȡ�
	nPixelMode���������ͣ�ֻ����ʾ������Ϊ˫��ɫʱ��Ч��1��R + G��2��G + R��
	pCom��ͨѶ���ڣ�ֻ���ڴ���ͨѶʱ�ò�����Ч������"COM1"
	nBaud��ͨѶ���ڲ����ʣ�ֻ���ڴ���ͨѶʱ�ò�����Ч��Ŀǰֻ֧��9600��57600���ֲ����ʡ�
	pSocketIP����������IP��ַ��ֻ��������ͨѶģʽ�¸ò�����Ч������"192.168.0.199"
	nSocketPort���������Ķ˿ڵ�ַ��ֻ��������ͨѶģʽ�¸ò�����Ч������5005
	nServerMode : 0 : ������ģʽδ������1��������ģʽ������
	pBarcode : �豸������
	pNetworkID : ����ID���
	pServerIP : ��ת������IP��ַ
	nServerPort : ��ת����������˿�
	pServerAccessUser : ��ת�����������û���
	pServerAccessPassword : ��ת��������������
	pCommandDataFile�����浽�ļ���ʽʱ������������ļ����ơ�ֻ���ڱ��浽�ļ�ģʽ�¸ò�����Ч������"curCommandData.dat"
����ֵ : �������״̬���붨�塣
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
	newScreen["Screen_control"] = nControlType; //����������� ���� ���� GPRS
	newScreen["Screen_SendMode"] = nSendMode; //ͨѶģʽ
	newScreen["Screen_ProtocolVer"] = nProtocolVer;
	newScreen["Screen_Width"] = nWidth; //���
	newScreen["Screen_Height"] = nHeight; //�߶�
	newScreen["Screen_color"] = nScreenType; //��ɫ
	newScreen["Screen_pixels"] = nPixelMode; //����
	newScreen["Screen_nPixType"] = 8; //���򳤶ȵ�����λ Ĭ��Ϊ8
	newScreen["Screen_ReadWaitTimeOut"] = WAIT_TIMEOUT_SERIALPORT + 2; //��2��֤���ƿ��ĺ����¼����ڿ��ƿ�ʱ��������ͣ��ʱ��

	//���ڵĲ���
	newScreen["Com_port"] = atoi(pCom + 3);
	newScreen["Com_baudrate"] = nBaud;
	newScreen["Com_address"] = nScreenNo;

	//GPRS�Ĳ���
	newScreen["Gprs_style"] = 0;
	newScreen["Gprs_ip"] = "192.168.0.235";
	newScreen["Gprs_id"] = "BX-GP000000";
	newScreen["Gprs_sim"] = "00000000000";
	newScreen["Gprs_port"] = 8000;
	newScreen["Gprs_level"] = 0;
	newScreen["Gprs_Unitsize"] = 900;
	newScreen["Gprs_Unittime"] = 2000;
	
	//���ڵĲ���
	newScreen["Net_IP"] = pSocketIP;	newScreen["Net_ConnnectMode"] = NET_CONNECT_STATICIP;	newScreen["Net_ServerMode"] = nServerMode; /* ������ģʽ�� 1--ʹ�ܣ�0-��ֹ */	newScreen["Net_ServerIP"] = pServerIP;	newScreen["Net_ServerPort"] = nServerPort;	newScreen["Net_ServerAccessUser"] = pServerAccessUser;	newScreen["Net_ServerAccessPassword"] = pServerAccessPassword;	newScreen["Net_ServerHeartBeatInterval"] = 20; /* ����ʱ��������λ���룩*/	newScreen["Net_mark"] = "255.255.255.0";	newScreen["Net_port"] = nSocketPort;	newScreen["Net_gateway"] = "192.168.0.1";	newScreen["Transit_Barcode"] = pBarcode;	newScreen["Network_ID"] = pNetworkID;	newScreen["Net_mac"] = "";

	newScreen["Screen_Light"] = 15; //����
	newScreen["Screen_AdjustLightMode"] = 0; //���ȵ���ģʽ

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
	newScreen["Screen_PowertimeEnabled"] = false; //��ʱ���ػ��Ƿ���Ч��
	newScreen["Screen_Powertime1use"] = false;
	newScreen["Screen_Powertime1on"] = "07:00";
	newScreen["Screen_Powertime1off"] = "07:00";
	newScreen["Screen_Powertime2use"] = false;
	newScreen["Screen_Powertime2on"] = "07:00";
	newScreen["Screen_Powertime2off"] = "07:00";
	newScreen["Screen_Powertime3use"] = false;
	newScreen["Screen_Powertime3on"] = "07:00";
	newScreen["Screen_Powertime3off"] = "07:00";
	newScreen["Screen_PA"] = nEncryptionType; //���������ܷ�ʽ�����Խ׶��趨Ϊ����ģʽ������ʱ����Ϊ�̶�����ģʽ
	newScreen["Screen_PA1"] = ""; //������������
	newScreen["Screen_PA2"] = ""; //������������
	newScreen["Screen_Reserved1"] = 0; //8������������4�����ͣ�4���ַ�����
	newScreen["Screen_Reserved2"] = 0;
	newScreen["Screen_Reserved3"] = 0;
	newScreen["Screen_Reserved4"] = 0;
	newScreen["Screen_Reserved5"] = "";
	newScreen["Screen_Reserved6"] = "";
	newScreen["Screen_Reserved7"] = "";
	newScreen["Screen_Reserved8"] = "";
	newScreen["Screen_lstDYArea"] = Json::arrayValue;
	devicelist_ja.append(newScreen);

	pNewThread = new tagSendThread();
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
������:    AddScreenDynamicArea
	��̬����ָ����ʾ����Ӷ�̬���򣻸ú���������ʾ��ͨѶ��
	���� :
nScreenNo����ʾ�����ţ��ò�����AddScreen_Dynamic�����е�nScreenNo������Ӧ��
nDYAreaID����̬�����ţ�Ŀǰϵͳ�����5����̬���򣻸�ֵȡֵ��ΧΪ0~4;
nRunMode����̬��������ģʽ��
0:��̬������ѭ����ʾ��
1 : ��̬��������ʾ��ɺ�ֹ��ʾ���һҳ���ݣ�
2 : ��̬������ѭ����ʾ�������趨ʱ���������δ����ʱ������ʾ��
3 : ��̬������ѭ����ʾ�������趨ʱ���������δ����ʱ��ʾLogo��Ϣ, Logo ��Ϣ��Ϊ��̬��������һҳ��Ϣ
4 : ��̬������˳����ʾ����ʾ�����һҳ��Ͳ�����ʾ
nTimeOut����̬�����ݳ�ʱʱ�䣻��λ����
nAllProRelate����Ŀ������־��
1�����н�Ŀ����ʾ�ö�̬����
0����ָ����Ŀ����ʾ�ö�̬���������̬����Ҫ�����ڽ�Ŀ��ʾ������һ������Ϊ�ա�
pProRelateList����Ŀ�����б��ý�Ŀ��ű�ʾ����Ŀ��ż���","����, ��Ŀ��Ŷ���ΪLedshowTW 2013�����"P***"�е�"***"
nPlayPriority����̬���򲥷����ȼ���
0���ö�̬�������첽��Ŀһ�𲥷�(��̬�����й����첽��Ŀ����Ч)��
1���첽��Ŀֹͣ���ţ������Ÿö�̬����
nAreaX����̬������ʼ�����ꣻ��λ������
nAreaY����̬������ʼ�����ꣻ��λ������
nAreaWidth����̬�����ȣ���λ������
nAreaHeight����̬����߶ȣ���λ������
nAreaFMode����̬����߿���ʾ��־��0����ɫ��1����ɫ��255���ޱ߿�
nAreaFLine����̬����߿�����, ��ɫ���ȡֵΪFRAME_SINGLE_COLOR_COUNT����ɫ���ȡֵΪ��FRAME_MULI_COLOR_COUNT
nAreaFColor���߿���ʾ��ɫ��ѡ��Ϊ��ɫ�߿�����ʱ�ò�����Ч��
nAreaFStunt���߿������ؼ���
0����˸��1��˳ʱ��ת����2����ʱ��ת����3����˸��˳ʱ��ת����
4:��˸����ʱ��ת����5�����̽�����˸��6�����̽���ת����7����ֹ���
nAreaFRunSpeed���߿������ٶȣ�
nAreaFMoveStep���߿��ƶ���������ֵȡֵ��Χ��1~8��
����ֵ : �������״̬���붨�塣
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
	newDynamicArea["DY_AreaFMode"] = nAreaFMode; //0:��ɫ��1����ɫ��$FF:û�б߿�
	newDynamicArea["DY_AreaFWidth"] = 1;
	newDynamicArea["DY_AreaFLine"] = nAreaFLine;
	newDynamicArea["DY_AreaFColor"] = nAreaFColor;
	newDynamicArea["Area_lstfile"] = Json::arrayValue; //������ļ�����
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

/*------------------------------------------------------------------------------ -
������:    AddScreenDynamicAreaText
	��̬����ָ����ʾ����ָ����̬���������Ϣ�ı����ú���������ʾ��ͨѶ��
	���� :
nScreenNo����ʾ�����ţ��ò�����AddScreen_Dynamic�����е�nScreenNo������Ӧ��
nDYAreaID����̬�����ţ��ò�����AddScreenDynamicArea�����е�nDYAreaID������Ӧ
pText����ӵ���Ϣ�ļ����ƣ�Ŀǰֻ֧��txt(֧��ANSI��UTF - 8��Unicode�ȸ�ʽ����)��bmp���ļ���ʽ
nShowSingle��������Ϣ�Ƿ�����ʾ��0��������ʾ��1��������ʾ����ʾ�ò���ֻ��szFileNameΪtxt��ʽ�ĵ�ʱ����Ч��
pFontName��������Ϣ��ʾ���壻�ò���ֻ��szFileNameΪtxt��ʽ�ĵ�ʱ����Ч��
nFontSize��������Ϣ��ʾ������ֺţ��ò���ֻ��szFileNameΪtxt��ʽ�ĵ�ʱ����Ч��
nBold��������Ϣ�Ƿ������ʾ��0��������1��������ʾ���ò���ֻ��szFileNameΪtxt��ʽ�ĵ�ʱ����Ч��
nFontColor��������Ϣ��ʾ��ɫ���ò���ֻ��szFileNameΪtxt��ʽ�ĵ�ʱ����Ч��
nStunt����̬������Ϣ�����ؼ���
00�������ʾ
01����ֹ��ʾ
02�����ٴ��
03�������ƶ�
04����������
05�������ƶ�
06����������
07����˸
08��Ʈѩ
09��ð��
10���м��Ƴ�
11����������
12�����ҽ�������
13�����½�������
14������պ�
15�������
16����������
17����������
18����������
19����������
20����������
21����������
22����������
23����������
24�����ҽ�����Ļ
25�����½�����Ļ
26����ɢ����
27��ˮƽ��ҳ
28����ֱ��ҳ
29��������Ļ
30��������Ļ
31��������Ļ
32��������Ļ
33�����ұպ�
34�����ҶԿ�
35�����±պ�
36�����¶Կ�
37�������ƶ�
38����������
39�������ƶ�
40����������
nRunSpeed����̬������Ϣ�����ٶ�
nShowTime����̬������Ϣ��ʾʱ�䣻��λ��10ms
����ֵ : �������״̬���붨�塣
	------------------------------------------------------------------------------ - */

int __stdcall AddScreenDynamicAreaText(int nScreenNo, int nDYAreaID,
	const char *pText, int nShowSingle, const char *pFontName, int nFontSize,
	int nBold, int nFontColor, int nStunt, int nRunSpeed, int nShowTime)
{
	int nScreenOrd, nDYAreaOrd;
	PtagSendThread ptmptagSendThread;
	std::string szFileName;
	std::string szFontName;
	UINT ntmpFileStyle;
	std::string Ext;
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

	newAreaText["File_Name"] = szFileName; //�ı�
	newAreaText["File_Style"] = FILE_TYPE_TEXT; //�ı�
	newAreaText["File_ShowStyle"] = ntmpFileStyle; //�ı���ʾ��ʽ
	newAreaText["File_FontName"] = szFontName; // ��������
	newAreaText["File_Bold"] = nBold; //�����Ƿ����
	newAreaText["File_FontSize"] = nFontSize; //�����ֺ�
	newAreaText["File_FontColor"] = nFontColor; //������ɫ
	newAreaText["File_PageCount"] = 0; //ת���ɵ�����ҳ
	newAreaText["File_Speed"] = nRunSpeed; //�ļ��ٶ�
	newAreaText["File_Stunt"] = nStunt; //�ļ��ؼ���ʽ
	newAreaText["File_OutsStunt"] = 0; //�˳���ʽ
	newAreaText["File_Showtime"] = nShowTime; //�ļ�ͣ��ʱ��
	newAreaText["File_ShowCount"] = 1; //�ظ����Ŵ���
	newAreaText["File_Reserved1"] = 0; //��������
	newAreaText["File_Reserved2"] = 0; //��������
	newAreaText["File_Reserved3"] = 0; //��������
	newAreaText["File_Reserved4"] = 0; //��������
	newAreaText["File_Reserved5"] = 0; //��������
	newAreaText["File_Reserved6"] = 0; //��������
	newAreaText["File_Reserved7"] = 0; //��������
	devicelist_ja[nScreenOrd]["Screen_lstDYArea"][nDYAreaOrd]["Area_lstfile"].append(newAreaText);

#ifdef _DEBUG
	SaveScreenInfoToFile();
#endif
	
	return RETURN_NOERROR;
}

/*------------------------------------------------------------------------------ -
������:    AddScreenDynamicAreaFile
	��̬����ָ����ʾ����ָ����̬���������Ϣ�ļ����ú���������ʾ��ͨѶ��
	���� :
nScreenNo����ʾ�����ţ��ò�����AddScreen_Dynamic�����е�nScreenNo������Ӧ��
nDYAreaID����̬�����ţ��ò�����AddScreenDynamicArea�����е�nDYAreaID������Ӧ
pFileName����ӵ���Ϣ�ļ����ƣ�Ŀǰֻ֧��txt(֧��ANSI��UTF - 8��Unicode�ȸ�ʽ����)��bmp���ļ���ʽ
nShowSingle��������Ϣ�Ƿ�����ʾ��0��������ʾ��1��������ʾ����ʾ�ò���ֻ��szFileNameΪtxt��ʽ�ĵ�ʱ����Ч��
pFontName��������Ϣ��ʾ���壻�ò���ֻ��szFileNameΪtxt��ʽ�ĵ�ʱ����Ч��
nFontSize��������Ϣ��ʾ������ֺţ��ò���ֻ��szFileNameΪtxt��ʽ�ĵ�ʱ����Ч��
nBold��������Ϣ�Ƿ������ʾ��0��������1��������ʾ���ò���ֻ��szFileNameΪtxt��ʽ�ĵ�ʱ����Ч��
nFontColor��������Ϣ��ʾ��ɫ���ò���ֻ��szFileNameΪtxt��ʽ�ĵ�ʱ����Ч��
nStunt����̬������Ϣ�����ؼ���
00�������ʾ
01����ֹ��ʾ
02�����ٴ��
03�������ƶ�
04����������
05�������ƶ�
06����������
07����˸
08��Ʈѩ
09��ð��
10���м��Ƴ�
11����������
12�����ҽ�������
13�����½�������
14������պ�
15�������
16����������
17����������
18����������
19����������
20����������
21����������
22����������
23����������
24�����ҽ�����Ļ
25�����½�����Ļ
26����ɢ����
27��ˮƽ��ҳ
28����ֱ��ҳ
29��������Ļ
30��������Ļ
31��������Ļ
32��������Ļ
33�����ұպ�
34�����ҶԿ�
35�����±պ�
36�����¶Կ�
37�������ƶ�
38����������
39�������ƶ�
40����������
nRunSpeed����̬������Ϣ�����ٶ�
nShowTime����̬������Ϣ��ʾʱ�䣻��λ��10ms
����ֵ : �������״̬���붨�塣
	------------------------------------------------------------------------------ - */

int __stdcall AddScreenDynamicAreaFile(int nScreenNo, int nDYAreaID,
	const char *pFileName, int nShowSingle, const char *pFontName, int nFontSize, int nBold, int nFontColor,
	int nStunt, int nRunSpeed, int nShowTime)
{
	int nScreenOrd, nDYAreaOrd;
	PtagSendThread ptmptagSendThread;
	std::string szFileName;
	std::string szFontName;
	int v6;
	UINT ntmpFileStyle;
	std::string Ext;
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
	std::transform(Ext.begin(), Ext.end(), Ext.begin(), ::toupper);
	if (Ext == ".BMP") //BMP�ļ�
	{
		v6 = FILE_TYPE_BMP;
		ntmpFileStyle = FILE_SHOWSTYLE_NORMAL;
	}
	else if (Ext == ".TXT") //TXT�ļ�
	{
		v6 = FILE_TYPE_TXT;
		if (nShowSingle == 1)
			ntmpFileStyle = FILE_SHOWSTYLE_SINGLE;
		else
			ntmpFileStyle = FILE_SHOWSTYLE_NORMAL;
	}
	else if (Ext == ".RTF") //RTF�ļ�
	{
		v6 = FILE_TYPE_RTF;
		ntmpFileStyle = FILE_SHOWSTYLE_NORMAL;
	}
	else
	{
		return RETURN_ERROR_NOSUPPORT_FILETYPE;
	}

	newAreaFile["File_Name"] = szFileName; //�ļ�����
	newAreaFile["File_Style"] = v6; //�ļ�����  //�ⲿͼƬ���ı�����񣬶��� 0��ͼƬ�ı���1��ʱ���ࣻ
	newAreaFile["File_ShowStyle"] = ntmpFileStyle; //�ı���ʾ��ʽ
	newAreaFile["File_FontName"] = szFontName; //��������
	newAreaFile["File_Bold"] = nBold; //�����Ƿ����
	newAreaFile["File_FontSize"] = nFontSize; //�����ֺ�
	newAreaFile["File_FontColor"] = nFontColor; //������ɫ
	newAreaFile["File_PageCount"] = 0; //ת���ɵ�����ҳ
	newAreaFile["File_Speed"] = nRunSpeed; //�ļ��ٶ�
	newAreaFile["File_Stunt"] = nStunt; //�ļ��ؼ���ʽ
	newAreaFile["File_OutsStunt"] = 0; //�˳���ʽ
	newAreaFile["File_Showtime"] = nShowTime; //�ļ�ͣ��ʱ��
	newAreaFile["File_ShowCount"] = 1; //�ظ����Ŵ���
	newAreaFile["File_Reserved1"] = 0; //��������
	newAreaFile["File_Reserved2"] = 0; //��������
	newAreaFile["File_Reserved3"] = 0; //��������
	newAreaFile["File_Reserved4"] = 0; //��������
	newAreaFile["File_Reserved5"] = 0; //��������
	newAreaFile["File_Reserved6"] = 0; //��������
	newAreaFile["File_Reserved7"] = 0; //��������
	devicelist_ja[nScreenOrd]["Screen_lstDYArea"][nDYAreaOrd]["Area_lstfile"].append(newAreaFile);

#ifdef _DEBUG
	SaveScreenInfoToFile();
#endif

	return RETURN_NOERROR;
}

static void SaveScreenInfoToFile()
{

}