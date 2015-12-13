#pragma once

//------------------------------------------------------------------------------
// 物理层协议栈版本号
const BYTE ProtocolVer_Third = 0x00; //3代控制卡的协议版本号位00；
const BYTE ProtocolVer_Fourth = 0x04; //4代控制卡的协议版本号位04；
const BYTE ProtocolVer_Fifth = 0x05; //5代控制卡的协议版本号位05；
const BYTE ProtocolVer_Sixth = 0x06;
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// 像素转换模式
const UINT Tran_Pixel_Mode0 = 0; //0：8bit红+8bit绿；
const UINT Tran_Pixel_Mode1 = 1; //1：从高到低位5bit红+6bit绿+5bit蓝；现用于BX-5Q+系列，BX-5QL除外
const UINT Tran_Pixel_Mode2 = 2; //2：从高到低位5bit红+5bit绿+5bit蓝；现用于BX-5QL
const UINT Tran_Pixel_Mode3 = 3; //3：从高到低位5bit蓝+6bit绿+5bit红；现用与BX-5QS系列
const UINT Tran_Pixel_Mode4 = 4; //4：红绿蓝分别整屏分开，基本单位为32bit。从高位到低位31位对应0,0点。发送先低字节后高字节
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// 中转服务器相关定义
const BYTE Transit_Error_Other = 0xFF; //操作失败
const BYTE Transit_Success = 0x00; //操作成功
const BYTE Transit_Error = 0x01; //	协议解析失败
const BYTE Transit_User_Error = 0x02; //	用户密码错误
const BYTE Transit_User_NotExist = 0x03; //	用户不存在
const BYTE Transit_User_Logged = 0x04; //	用户已经登录
const BYTE Transit_User_Unavailable = 0x05; //	用户不可以使用，表示已经冻结
const BYTE Transit_User_NotSend = 0x06; //	用户不允许发送（用户处于已退出状态）
const BYTE Transit_User_Mismatching = 0x07; // 登陆用户和管理设备用户不匹配
const BYTE Transit_User_Deleted = 0x08; //用户已经删除

const BYTE Transit_Device_Unknown = 0x10; //	设备未知
const BYTE Transit_Device_Online = 0x11; //	设备在线
const BYTE Transit_Device_Offline = 0x12; //	设备不在线
const BYTE Transit_Device_Overdue = 0x13; //	设备过期，欠费
const BYTE Transit_Device_Deleted = 0x14; //	设备已经删除
const BYTE Transit_Device_NotExist = 0x15; //	设备不存在
const BYTE Transit_Device_2G = 0x16; //	设备为2G
const BYTE Transit_Device_3G = 0x17; //	设备为3G
const BYTE Transit_Device_Sending = 0x18; //	设备处于发送中
const BYTE Transit_Device_Idle = 0x19; //	设备空闲中

const BYTE Transit_Control_NotExist = 0x20; //	控制器网络ID不存在
const BYTE Transit_Control_Offline = 0x21; //	控制器不在线
const BYTE Transit_Control_OutTime = 0x22; //	控制器通信超时
const BYTE Transit_Control_Online = 0x23; //	控制器在线
const BYTE Transit_Device_Invalidation = 0x27; //	设备失效
const BYTE Transit_Device_Type_Error = 0x28; //  设备类型错误

const int NetID_Len = 12;
const int Transit_Def_TimeoutSec = 5; //中转服务器通讯默认延时。

const int Logo_Width_Default = 64;
const int Logo_Height_Default = 64;

typedef enum
{
	TCP_StaticIPMode = 0,
	UDP_StaticIPMode = 1
} TNetwork_StaticIPMode;

typedef enum
{
	Manual_Adjust = 0,
	Timer_Adjust = 1,
	Auto_Adjust = 2
} TAdjustBright;

typedef enum
{
	LogoAlign_Center = 0,
	LogoAlign_TopRight = 1,
	LogoAlign_TopLeft = 2
} TLogoAlign;

typedef enum
{
	LogoMode_Pic = 0,
	LogoMode_Txt = 1
} TLogoMode;

typedef struct
{
	string Name; //设备名称
	string BarCode; //设备条形码
	UINT Status; //设备在线状态
	HANDLE Transit_Type; //设备类型；2G ? 3G
	bool Matched; // 设备是否已经匹配
	int Matche_ScreenOrd; //设备匹配软件中屏序号
	string NetworkID; //网络ID,只有3G类型时该属性才有效
} tag_Transit, Ttag_Transit, *Ptag_Transit;
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// 全局变量集
typedef void (__stdcall *TProcessMessageEvent)(string szMessagge, int nProcess);

string g_szDefFont;
//中转服务器的ip地址和端口，从SysSetup.ini中读取
string g_Transit_Server_IP;
int g_Transit_ServerCon_Port, g_Transit_ServerUser_Port;
string g_Transit_User;
string g_Transit_Pwd;
bool g_bTransit_Login, g_bHaveTransit;
HANDLE g_Transit_CheckThread;
TProcessMessageEvent g_OnProcessMessage;

//g_lstTransit: TList;