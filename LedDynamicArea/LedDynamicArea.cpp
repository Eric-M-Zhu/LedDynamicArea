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
	// TODO: Free send threads in list
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
	// TODO: new thread
	return RETURN_NOERROR;
}