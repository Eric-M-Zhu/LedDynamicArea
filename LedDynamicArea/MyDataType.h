#pragma once

#include "../jsoncpp/json.h"

const int FIND_SERIAL_PING_WAITOUT = 500; //ping命令的串口等待时间。
const int FIND_NETWORK_PING_WAITOUT = 500; //ping命令的网口等待时间。

//------------------------------------------------------------------------------
// 物理层协议版本号
const unsigned char PROTOCOLVER_THIRD_CONTROL = 0x00;
const unsigned char PROTOCOLVER_FOURTH_CONTROL = 0x04;
const unsigned char PROTOCOLVER_FIFTH_CONTROL = 0x05;
const unsigned char PROTOCOLVER_FIFTH_DYNAMIC = 0xF0;
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// 通讯是否返回状态
const int COMMUNICATION_RtnReq = 1; //返回状态。
const int COMMUNICATION_NORtnReq = 1; //不返回状态
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// 加密算法随机数取值范围
const int RANDOM_FROM = 2;
const int RANDOM_TO = 255;
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// 物理层最大传输字节
const int COM_FRAMELENMAX = 132; //串口模式最大传输字节除以16
const int NET_FRAMELENMAX = 132; //网口模式最大传输字节除以16
const int GPRS_FRAMELENMAX = 8; //GPRS模式最大传输字节除以16
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// 加密方式
const unsigned char ENCRYPTION_NONE = 0x96; //明码模式
const unsigned char ENCRYPTION_STATIC = 1; //默认密码加密模式
const unsigned char ENCRYPTION_USER = 2; //用户密码加密模式
const int ENCRYPTION_LENGTH = 112; //密钥加密长度
//------------------------------------------------------------------------------

const int USB_FIFTH_PARAMETERS_FILECOUNT = 11; //当前5代USB控制卡有11个参数文件

//------------------------------------------------------------------------------
// 控制器类型
const unsigned char CONTROLLER_TYPE_FOURTH = 0x40; //判断3代卡、4代卡的起始值

const unsigned char CONTROLLER_TYPE_4VERSION2 = 0xF0;
const unsigned char CONTROLLER_TYPE_4VERSION3 = 0xEF;
const unsigned short CONTROLLER_TYPE_WILDCARD = 0xFFFE; //控制器类型通配符
const unsigned short CONTROLLER_TYPE_3T = 0x10;
const unsigned short CONTROLLER_TYPE_3A = 0x20;
const unsigned short CONTROLLER_TYPE_3A1 = 0x21;
const unsigned short CONTROLLER_TYPE_3A2 = 0x22;
const unsigned short CONTROLLER_TYPE_3M = 0x30;
const unsigned short CONTROLLER_TYPE_4A = 0x0041;

const unsigned short CONTROLLER_TYPE_4C = 0x0043;
const unsigned short CONTROLLER_TYPE_4E1 = 0x0144;
const unsigned short CONTROLLER_TYPE_4E = 0x0044;
const unsigned short CONTROLLER_TYPE_4EL = 0x0844;

const unsigned short CONTROLLER_TYPE_4U0 = 0x0045;
const unsigned short CONTROLLER_TYPE_4U1 = 0x0145;
const unsigned short CONTROLLER_TYPE_4U2 = 0x0245;
const unsigned short CONTROLLER_TYPE_4U3 = 0x0345;
const unsigned short CONTROLLER_TYPE_4UT = 0x0445;
const unsigned short CONTROLLER_TYPE_4U2X = 0x0545;

const unsigned short CONTROLLER_TYPE_5E1 = 0x0154;
const unsigned short CONTROLLER_TYPE_5E2 = 0x0254;
const unsigned short CONTROLLER_TYPE_5E3 = 0x0354;
const unsigned short CONTROLLER_TYPE_5EC = 0x0454;
//------------------------------------------------------------------------------

const int BX5E_SERIAL_FIRWAREVERSION = 0; //BX-5E串口版本
const int BX5E_NETWORK_FIRWAREVERSION = 1; //BX-5E网口口版本

const int CONTROLLER_TYPE_OEFRONT[3] = { 0, 2, 4 };

//------------------------------------------------------------------------------
// 特技列表宏定义
const int STUNT_RANDOM = 0; //随机显示
const int STUNT_STATIC = 1; //静态
const int STUNT_DIRECT_SHOW = 2; //快速打出
const int STUNT_MOVE_LEFT = 3; //向左移动
const int STUNT_CONTINUOUS_MOVE_LEFT = 4; //向左连移
const int STUNT_MOVE_UP = 5; //向上移动
const int STUNT_CONTINUOUS_MOVE_UP = 6; //向上连移
const int STUNT_FLICKER = 7; //闪烁
const int STUNT_SNOWING = 8; //飘雪
const int STUNT_BUBBLING = 9; //冒泡
const int STUNT_MIDDLE_OUT = 10; //中间移出
const int STUNT_MOVED_AROUND = 11; //左右移入
const int STUNT_HORIZONTAL_CROSS_MOVE = 12; //左右交叉移入
const int STUNT_VERTICAL_CROSS_MOVE = 13; //上下交叉移入
const int STUNT_SCROLL_CLOSED = 14; //画卷闭合
const int STUNT_SCROLL_OPENED = 15; //画卷打开
const int STUNT_LEFT_STRETCH = 16; //向左拉伸
const int STUNT_RIGHT_STRETCH = 17; //向右拉伸
const int STUNT_UP_STRETCH = 18; //向上拉伸
const int STUNT_DOWN_STRETCH = 19; //向下拉伸
const int STUNT_LEFT_LASER = 20; //向左镭射
const int STUNT_RIGHT_LASER = 21; //向右镭射
const int STUNT_UP_LASER = 22; //向上镭射
const int STUNT_DOWN_LASER = 23; //向下镭射
const int STUNT_CROSS_CURTAIN_LEFT_AND_RIGHT = 24; //左右交叉拉幕
const int STUNT_CROSS_CURTAIN_UP_AND_DOWN = 25; //上下交叉拉幕
const int STUNT_CURTAIN_SCATTERED_TO_THE_LEFT = 26; //分散左拉
const int STUNT_HORIZONTAL_BLINDS = 27; //水平百页
const int STUNT_VERTICAL_BLINDS = 28; //垂直百页
const int STUNT_CURTAIN_LEFT = 29; //向左拉幕
const int STUNT_CURTAIN_RIGHT = 30; //向右拉幕
const int STUNT_CURTAIN_UP = 31; //向上拉幕
const int STUNT_CURTAIN_DOWN = 32; //向下拉幕
const int STUNT_MOVE_TO_CENTER_FROM_LEFT_AND_RIGHT = 33; //左右闭合
const int STUNT_SPLIT_TO_LEFT_AND_RIGHT = 34; //左右对开
const int STUNT_MOVE_TO_CENTER_FROM_UP_AND_DOWN = 35; //上下闭合
const int STUNT_SPLIT_TO_UP_AND_DOWN = 36; //上下对开
const int STUNT_MOVE_RIGHT = 37; //向右移动
const int STUNT_CONTINUOUS_MOVE_RIGHT = 38; //向右连移
const int STUNT_MOVE_DOWN = 39; //向下移动
const int STUNT_CONTINUOUS_MOVE_DOWN = 40; //向下连移
//------------------------------------------------------------------------------

const int AreaMovieRunSpeed[16] = { 30, 28, 26, 24, 22, 20, 18, 16, 14, 12, 10, 8, 6, 4, 2, 1 }; // 用于每一动画的运行帧速率

//------------------------------------------------界面上所有的combobox进行参数设置
const std::string COMBOBOX_TYPE_Pro_FrameStunt[7] = { "Flicker", "Clockwise rotation", "Counterclockwise rotation",
	"Flicker and clockwise rotation", "Flicker and counterclockwise rotation", "Colors alternating", "Static" }; //边框的运行方式
const std::string COMBOBOX_TYPE_Pro_FrameSpeed[16] = {
	"1(Fastest)", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16(Slowest)" }; // 边框的运行速度
const std::string COMBOBOX_TYPE_Device_SendMode[3] = { "Serial communication", "Network Communication", "GPRS Communication" }; //发送模式
const std::string COMBOBOX_TYPE_Device_Color[3] = { "Single color", "Two colors", "Three colors" }; //颜色
const std::string COMBOBOX_TYPE_Device_DA[2] = { "Negative", "Positive" }; //DA极性
const std::string COMBOBOX_TYPE_Device_OE[2] = { "Low", "High" }; //OE极性
const std::string COMBOBOX_TYPE_Device_RowOrder[3] = { "Normal", "+1", "-1" }; //行顺序
const std::string COMBOBOX_TYPE_Device_MirrorMode[2] = { "Normal", "Mirror" }; //数据流向

const int BaudRate[2] = { 9600, 57600 };
const int SCREEN_ADDRESS_LISTCOUNT = 5;
const int PRO_FRAME_LENGTH = 32; //节目流水边框图案属性为的定长为32.
const int PRO_FRAME_MAX_HEIGHT = 8; //节目流水边框图案最大高度.
const std::string USER_PASSWORD_ADMIN = "888";
const std::string USER_PASSWORD_GUEST = "168";
const std::string USER_PASSWORD_SUPER = "onbonbx";
const int GROUP_MAX_LEVEL = 5; //分组的最大级别数。从0开始。
const int AREA_SHOWTIME_BASIC = 50; //目前控制器的停留时间为10毫秒；但图文、字幕区域的基本单位为0.5秒；因此此处为500/10=50；
const std::string SERIAL_PORT_NONE = "COM0"; //无串口标志；当设定为该串口是提示用户选择串口
const int CONTROLLER_ADDRESS_WILDCARD = 0xFFFE; //控制器地址通配符
const std::string NET_UDP_BROADCAST = "255.255.255.255"; //用于计算机网络直连情况下UDP的直连IP地址。
const std::string NET_AUTO_MAC = "00-FF-FE"; //自动生成MAC地址的前三段；
const int PROGRAM_FRAME_MAX_WIDTH = 8; //流水边框最大值
const int COMPORT_MAX_READ_LENGTH = 4096; //从串口最大读取一包数据长度。
const int COMPORT_MAX_READ_SLEEP_UNIT = 5; //从串口最大读取一包数据长度等待时间单位；毫秒。
const int BXGPRS_MAX_READ_SLEEP_UNIT = 100; //从BXGPRS最大读取一包数据长度等待时间单位；毫秒。
const int DEFAULT_BX_GPRS_SERVER_PORT = 8000; //BX-GPRS服务器的端口
const int BX_GPRS_ERROR_SLEEP = 2000; //GPRS模式错误重发前的等待时间；单位：毫秒
const int MULTI_COMMUNICATION_COUNT = 20; //多机通讯同时通讯最大个数。
const int SEND_SINGLE_COMMUNICATION = 0; //单机通讯模式
const int SEND_MULTI_COMMUNICATION = 1; //多机通讯模式

//------------------------------------------------------------------------------
// 控制器网络链接模式
const int NET_CONNECT_DIRECT = 0; //单击直连模式；UDP
const int NET_CONNECT_DHCP = 1; // 自动获取IP（DHCP）；
const int NET_CONNECT_STATICIP = 2; //手动设置IP（Static IP）；
//------------------------------------------------------------------------------

const int AREA_MOVIE_FRAME_UNIT = 1000; //动画区域的帧播放单位为1秒；1000毫秒
const int AREA_MOVIE_FRAME_STUNT = 0xFE; //动画区域的播放特技为1；快速打出；
const int AREA_MOVIE_FRAME_OUTSTUNT = 1; //动画区域的退出特技默认为无效；
const int AREA_SHOWTIME_UNIT = 10; //图文区域的停留时间实际单位为10毫秒

//------------------------------------------------------------------------------
// 不同通讯模式下的通讯超时设置
const int WAIT_TIMEOUT_SERIALPORT = 15;
const int WAIT_TIMEOUT_NET = 6;
const int WAIT_TIMEOUT_GPRS = 40;
const int WAIT_TIMEOUT_PCVALUE = 1000; //计算机得到的返回值的超时时间设置要比控制器多50%,并且将其转化为毫秒
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// 不同通讯模式下的通讯超时设置，实际等待的参数，单位为毫秒
const int WAIT_TIMEOUT_REALITY_SERIALPORT = 2500;
const int WAIT_TIMEOUT_REALITY_NET = 15000;
const int WAIT_TIMEOUT_REALITY_GPRS = 40000;
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// 发送信息结果
const int SEND_SENDING = 0; //:正在发送中，且没有完成；
const int SEND_FAILED = 1; //：发送完成，但没有发送成功；
const int SEND_SUCESSED = 2; //：发送完成且成功。 发送完成后要删除该结构指针的；
const int SEND_PARAMETER_DISACCORD = 3; //：发送ping命令返回发现参数不对应。
const int SEND_UNCOMPATIBLE = 4; //Firmware版本不兼容；
const int SEND_NOSTART_SEND = 5; //还未开始发送
const int SEND_FIRMWARE_FIRST_INCOMP = 6; //与控制卡的第一版本不兼容。
const int SEND_FIRMWARE_SECOND_INCOMP = 7; //与控制卡的第二版本不兼容。
const int SEND_FIRMWARE_THIRD_INCOMP = 8; //与控制卡的第三本不兼容。
const int SEND_BX_5E_FIRMWARE_INCOMP_0 = 9; //与BX-5E控制卡的子版本不兼容,需载入0段子版本。
const int SEND_BX_5E_FIRMWARE_INCOMP_1 = 10; //与BX-5E控制卡的子版本不兼容,需载入1段子版本。
const int SEND_BX_5E_FIRMWARE_INCOMP = 11; //与BX-5E控制卡的版本不兼容，提示用户升级程序。
//------------------------------------------------------------------------------

//==============================================================================
// 发送数据的文件类型    此值要与Screen_Control的序号对应。
const int SEND_FILE_P = 0x00; //节目文件
const int SEND_FILE_C = 0x01; // 控制器参数配置文件
const int SEND_FILE_S = 0x02; // 扫描配置文件
const int SEND_FILE_L = 0x03; // 播放列表文件
const int SEND_FILE_F = 0x04; // 控制器固件
const int SEND_FILE_O = 0x05; //字库文件
//==============================================================================

//------------------------------------------------------------------------------
// 默认边框类型、序号
const int PROGRAM_FRAME_16_DEFAULT_LINE_SINGLE_COLOR = 0; //单色小于等于16点时默认的边框类型
const int PROGRAM_FRAME_16_DEFAULT_WIDTH_SINGLE_COLOR = 1; //单色小于等于16点时默认的边框宽度
const int PROGRAM_FRAME_32_DEFAULT_LINE_SINGLE_COLOR = 2; //单色大于16小于等于32点时默认的边框类型
const int PROGRAM_FRAME_32_DEFAULT_WIDTH_SINGLE_COLOR = 2; //单色大于16小于等于32点时默认的边框宽度
const int PROGRAM_FRAME_MORE_32_DEFAULT_LINE_SINGLE_COLOR = 11; //单色大于32点式默认的边框类型;修改了边框图形后该参数值需要重新定义。
const int PROGRAM_FRAME_MORE_32_DEFAULT_WIDTH_SINGLE_COLOR = 4; //单色大于等于32点时默认的边框宽度

const int PROGRAM_FRAME_16_DEFAULT_LINE_MULI_COLOR = 0; //双基色小于等于16点时默认的边框类型
const int PROGRAM_FRAME_16_DEFAULT_WIDTH_MULI_COLOR = 1; //双基色小于等于16点时默认的边框宽度
const int PROGRAM_FRAME_32_DEFAULT_LINE_MULI_COLOR = 2; //双基色大于16小于等于32点时默认的边框类型
const int PROGRAM_FRAME_32_DEFAULT_WIDTH_MULI_COLOR = 2; //双基色大于16小于等于32点时默认的边框宽度
const int PROGRAM_FRAME_MORE_32_DEFAULT_LINE_MULI_COLOR = 12; //双基色大于32点式默认的边框类型;修改了边框图形后该参数值需要重新定义。
const int PROGRAM_FRAME_MORE_32_DEFAULT_WIDTH_MULI_COLOR = 4; //双基色大于等于32点时默认的边框宽度
const int PROGRAM_FRAME_SINGLE_COLOR_COUNT = 23; //纯色边框图片个数
const int PROGRAM_FRAME_MULI_COLOR_COUNT = 47; //花色边框图片个数
const int SCREEN_MESSAGE_BMP_MAXWIDTH = 128;
const int SCREEN_MESSAGE_BMP_MAXHEIGHT = 16;
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// 控制器存储空间  单位:Kbytes
const int MEMORY_BX_3T = 1024;
const int MEMORY_BX_3A1 = 1024;
const int MEMORY_BX_3A2 = 1024;
const int MEMORY_BX_3A = 2048;
const int MEMORY_BX_3M = 2048;
const int MEMORY_BX_4A1 = 1024;
const int MEMORY_BX_4A2 = 1024;
const int MEMORY_BX_4A3 = 1024;
const int MEMORY_BX_4AQ = 1024;
const int MEMORY_BX_4A = 2048;

const int MEMORY_BX_4M1 = 2048;
const int MEMORY_BX_4M = 2048;
const int MEMORY_BX_4MC = 2048;
const int MEMORY_BX_4E = 2048;
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// 控制器实际有效存储空间  单位:Kbytes
const int MEMORY_REALITY_BX_3T = 756;
const int MEMORY_REALITY_BX_3A1 = 756;
const int MEMORY_REALITY_BX_3A2 = 756;
const int MEMORY_REALITY_BX_3A = 1500;
const int MEMORY_REALITY_BX_3M = 1552;
const int MEMORY_REALITY_BX_4A1 = 716;
const int MEMORY_REALITY_BX_4A2 = 716;
const int MEMORY_REALITY_BX_4A3 = 716;
const int MEMORY_REALITY_BX_4AQ = 716;
const int MEMORY_REALITY_BX_4A = 1484;
const int MEMORY_REALITY_BX_4M1 = 1496;
const int MEMORY_REALITY_BX_4M = 1496;
const int MEMORY_REALITY_BX_4MC = 1496;
const int MEMORY_REALITY_BX_4E = 1496;
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// 激活新固件所需要的事件；单位毫秒
const int FIRMWARE_UPDATE_3A = 10000;
const int FIRMWARE_UPDATE_4A = 20000;
const int FIRMWARE_UPDATE_3M = 30000;
const int FIRMWARE_UPDATE_4M = 50000;
const int FIRMWARE_UPDATE_OTHER = 50000;
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// USB资源存放目录
const std::string USB_DISK_DIRCECTORY = "onbonbx";
const std::string USB_DISK_OPTION_FILE = "bxconfig.bin";
const std::string USB_DISK_PROGRAM_FILE = "bxdata.dat";
const std::string USB_DISK_BXSET_FILE = "bxset.bin";
const std::string USB_DISK_SET0_FILE = "SET0";
//------------------------------------------------------------------------------

const int MOVIE_AREA_SINGLE_FILE_MAX_LENGTH = 150 * 1024; //单个动画文件的最大数据量为150Kbytes
const int MOVIE_AREA_WIDTH = 768; //动画区域最宽为768点。
const int MOVIE_AREA_HEIGHT = 256; //动画区域最高为256点。
const int MOVIE_AREA_AREA = 768 * 64; //单个文件的最大区域为单色768*64；双色为768*64/2;

//------------------------------------------------------------------------------
// 最大控制节目个数
const int PROGRAM_MAX_3T = 8;
const int PROGRAM_MAX_3A = 16;
const int PROGRAM_MAX_3M = 192;
const int PROGRAM_MAX_4A1 = 16;
const int PROGRAM_MAX_4A2 = 16;
const int PROGRAM_MAX_4A3 = 16;
const int PROGRAM_MAX_4AQ = 128;
const int PROGRAM_MAX_4A = 16;
const int PROGRAM_MAX_4M1 = 192;
const int PROGRAM_MAX_4M = 192;
const int PROGRAM_MAX_4MC = 192;
const int PROGRAM_MAX_4E = 192;
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// 单节目最大控制区域数
const int AREA_MAX_3T = 4;
const int AREA_MAX_3A = 8;
const int AREA_MAX_3M = 32;
const int AREA_MAX_4A1 = 8;
const int AREA_MAX_4A2 = 8;
const int AREA_MAX_4A3 = 8;
const int AREA_MAX_4AQ = 8;
const int AREA_MAX_4A = 8;

const int AREA_MAX_4M1 = 32;
const int AREA_MAX_4M = 32;
const int AREA_MAX_4MC = 32;
const int AREA_MAX_4E = 32;
const int TIMER_AREA_MAX_SINGLE = 1; //单CPU类控制卡最大的时间类区域个数。
const int TIMER_AREA_MAX_BX_4M = 2;
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// 系统最大支持的显示屏数量
const int SYSTEM_MAXSCREEN_COUNT = 255;
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// 显示屏类型
const int SCREEN_COLOR_SINGLE = 1; //单基色
const int SCREEN_COLOR_DOUBLE = 2; //双基色
const int SCREEN_COLOR_THREE = 3; //双基色
const int SCREEN_COLOR_FULLCOLOR = 4; //全彩色
const int SCREEN_COLOR_TWO_COLOR_GRAY = 5; //双基色灰度

const int DOUBLE_COLOR_PIXTYPE_1 = 1; //双基色1：R+G
const int DOUBLE_COLOR_PIXTYPE_2 = 2; //双基色2：G+R

const int FULL_COLOR_NOGRAY = 0; //全彩无灰度；
const int FULL_COLOR_GRAY = 1; //全彩有灰度
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// 文本文件类的显示方式
//------------------------------------------------------------------------------
const int FILE_SHOWSTYLE_NORMAL = 0; //正常显示方式
const int FILE_SHOWSTYLE_SINGLE = 1; //单行显示方式
const int FILE_SHOWSTYLE_AUTO = 2; //自适应显示方式

//------------------------------------------------------------------------------
// 默认显示区域坐标、大小
//------------------------------------------------------------------------------
const int DEFAULT_AREA_TW_WIDTH = 64;
const int DEFAULT_AREA_TW_HEIGHT = 64;
const int DEFAULT_AREA_ZM_WIDTH = 64;
const int DEFAULT_AREA_ZM_HEIGHT = 16;
const int DEFAULT_AREA_MOV_WIDTH = 64;
const int DEFAULT_AREA_MOV_HEIGHT = 64;
const int DEFAULT_AREA_TMDEFAULT_WIDTH = 64;
const int DEFAULT_AREA_TMDEFAULT_HEIGHT = 16;
const int DEFAULT_AREA_TM_WIDTH = 81;
const int DEFAULT_AREA_TM_HEIGHT = 13;
const int DEFAULT_AREA_CHONO_WIDTH = 41;
const int DEFAULT_AREA_CHONO_HEIGHT = 13;
const int DEFAULT_AREA_CLOCK_WIDTH = 48;
const int DEFAULT_AREA_CLOCK_HEIGHT = 48;
const int DEFAULT_AREA_LUNAR_WIDTH = 52;
const int DEFAULT_AREA_LUNAR_HEIGHT = 13;
const int DEFAULT_AREA_FONT_WIDTH = 64;
const int DEFAULT_AREA_FONT_HEIGHT = 32;
const int DEFAULT_AREA_TMP_WIDTH = 64;
const int DEFAULT_AREA_TMP_HEIGHT = 16;
const int DEFAULT_AREA_HUM_WIDTH = 64;
const int DEFAULT_AREA_HUM_HEIGHT = 16;
const int DEFAULT_AREA_NOS_WIDTH = 64;
const int DEFAULT_AREA_NOS_HEIGHT = 16;

//------------------------------------------------------------------------------
// 区域类型
const int AREA_TYPE_BMPTEXT = 0;
const int AREA_TYPE_FONT = 1;
const int AREA_TYPE_TIME = 2;
const int AREA_TYPE_TEMPERATURE = 3;
const int AREA_TYPE_HUMIDITY = 4;
const int AREA_TYPE_NOISE = 5;
const int AREA_TYPE_SINGLETEXT = 0x80;
const int AREA_TYPE_MOVIE = 6;
const int AREA_TYPE_TIMEDEFAULT = 7;
const int AREA_TYPE_CHRONOGRAPY = 8;
const int AREA_TYPE_TIMELUNAR = 9;
const int AREA_TYPE_TIMECLOCK = 10;
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// 特技列表
const unsigned char AREA_STUNT_RANDOM = 0x00; //0x00 – 随机显示
const unsigned char AREA_STUNT_STATIC = 0x01; //0x01 – 静止显示
const unsigned char AREA_STUNT_DIRECTSHOW = 0x02; //0x02 – 快速打出
const unsigned char AREA_STUNT_LMOVE = 0x03; //0x03 – 向左移动
const unsigned char AREA_STUNT_LCONTIMOVE = 0x04; //0x04 – 向左连移
const unsigned char AREA_STUNT_UMOVE = 0x05; //0x05 – 向上移动
const unsigned char AREA_STUNT_UCONTIMOVE = 0x06; //0x06 – 向上连移
const unsigned char AREA_STUNT_FICKER = 0x07; //0x07 – 闪烁
const unsigned char AREA_STUNT_SNOWING = 0x08; //0x08 – 飘雪
const unsigned char AREA_STUNT_BUBBING = 0x09; //0x09 – 冒泡
const unsigned char AREA_STUNT_MIDMOVE = 0x0A; //0x0a – 中间移出
const unsigned char AREA_STUNT_LRMOVE = 0x0B; //0x0b – 左右移入
const unsigned char AREA_STUNT_LRCROSSMOVE = 0x0C; //0x0c – 左右交叉移入
const unsigned char AREA_STUNT_UDCROSSMOVE = 0x0D; //0x0d – 上下交叉移入
const unsigned char AREA_STUNT_SCROLLCLOSE = 0x0E; //0x0e – 画卷闭合
const unsigned char AREA_STUNT_SCROLLOPEN = 0x0F; //0x0f – 画卷打开
const unsigned char AREA_STUNT_LSTRETCH = 0x10; //0x10 – 向左拉伸
const unsigned char AREA_STUNT_RSTRETCH = 0x11; //0x11 – 向右拉伸
const unsigned char AREA_STUNT_USTRETCH = 0x12; //0x12 – 向上拉伸
const unsigned char AREA_STUNT_DSTRETCH = 0x13; //0x13 – 向下拉伸
const unsigned char AREA_STUNT_LLASER = 0x14; //0x14 – 向左镭射
const unsigned char AREA_STUNT_RLASER = 0x15; //0x15 – 向右镭射
const unsigned char AREA_STUNT_ULASER = 0x16; //0x16 – 向上镭射
const unsigned char AREA_STUNT_DLASER = 0x17; //0x17 – 向下镭射
const unsigned char AREA_STUNT_LRCROSSCURTAIN = 0x18; //0x18 – 左右交叉拉幕
const unsigned char AREA_STUNT_UDCROSSCURTAIN = 0x19; //0x19 – 上下交叉拉幕
const unsigned char AREA_STUNT_DISPERSELCURTAIN = 0x1A; //0x1a – 分散左拉
const unsigned char AREA_STUNT_HORLAYER = 0x1B; //0x1b – 水平百页
const unsigned char AREA_STUNT_VERLAYER = 0x1C; //0x1c – 垂直百页
const unsigned char AREA_STUNT_LCURTAIN = 0x1D; //0x1d – 向左拉幕
const unsigned char AREA_STUNT_RCURTAIN = 0x1E; //0x1e – 向右拉幕
const unsigned char AREA_STUNT_UCURTAIN = 0x1F; //0x1f – 向上拉幕
const unsigned char AREA_STUNT_DCURTAIN = 0x20; //0x20 – 向下拉幕
const unsigned char AREA_STUNT_LRCLOSE = 0x21; //0x21 – 左右闭合
const unsigned char AREA_STUNT_LROPEN = 0x22; //0x22 – 左右对开
const unsigned char AREA_STUNT_UDCLOSE = 0x23; //0x23 – 上下闭合
const unsigned char AREA_STUNT_UPOPEN = 0x24; //0x24 – 上下对开
const unsigned char AREA_STUNT_RMOVE = 0x25; //0x25 – 向右移动
const unsigned char AREA_STUNT_RCONTIMOVE = 0x26; //0x26 - 向右连移
const unsigned char AREA_STUNT_DMOVE = 0x27; //0x27 – 向下移动
const unsigned char AREA_STUNT_DCONTIMOVE = 0x28; //0x28 - 向下连移
const unsigned char AREA_STUNT_MOVIE = 0xFE; //动画模式
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// 分组、选中状态、显示屏、节目、区域等图片索引值
const int IMAGEINDEX_GROUP = 0;
const int IMAGEINDEX_SUBGROUP = 1;
const int IMAGEINDEX_DELETE = 2;
const int IMAGEINDEX_SCREEN = 3;
const int IMAGEINDEX_UNCHECK = 4;
const int IMAGEINDEX_CHECK = 5;
const int IMAGEINDEX_PROGRAM_OVERALL = 6;
const int IMAGEINDEX_PROGRAM_NORMAL = 7;
const int IMAGEINDEX_PROGRAM_SHARE = 8;
const int IMAGEINDEX_PROGRAM_AUTO = 9;
const int IMAGEINDEX_AREA_BMPTEXT = 10;
const int IMAGEINDEX_AREA_TITLE = 11;
const int IMAGEINDEX_AREA_TIME = 12;
const int IMAGEINDEX_AREA_FONT = 13;
const int IMAGEINDEX_AREA_TEMPERATURE = 14;
const int IMAGEINDEX_AREA_HUMIDITY = 15;
const int IMAGEINDEX_AREA_NOISE = 16;
const int IMAGEINDEX_AREA_MOVIE = 18;
const int IMAGEINDEX_AREA_TIMEDEFAULT = 19; //老的时间区域
const int IMAGEINDEX_AREA_CHRONOGRAPY = 20; //老的记时区域
const int IMAGEINDEX_AREA_TIMELUNAR = 21;
const int IMAGEINDEX_AREA_TIMECLOCK = 22;
const int IMAGEINDEX_GPRS_OUTLINE_SCREEN = 23; //不在线的GPRS终端。
//IMAGEINDEX_
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// 文件类型
const int FILE_TYPE_TMF = 1; //时间文件
const int FILE_TYPE_TXTF = 2; //TXTF文件
const int FILE_TYPE_TXTS = 3; //TXTS文件
const int FILE_TYPE_TBF = 4; //表格文件
const int FILE_TYPE_TXT = 5; //TXT文件
const int FILE_TYPE_RTF = 6; //RTF文件
const int FILE_TYPE_BMP = 7; //BMP文件
const int FILE_TYPE_JPG = 8; //JPG文件
const int FILE_TYPE_GIF = 9; //GIF文件
const int FILE_TYPE_JPEG = 10; //JPEG文件
const int FILE_TYPE_AVI = 11; //AVI文件
const int FILE_TYPE_SWF = 12; //SWF文件
const int FILE_TYPE_TXTA = 13; //TXTA文件,自适应文件
const int FILE_TYPE_TXTSWF = 14; //TXTA文件,自适应文件
const int FILE_TYPE_TEXT = 15; //TEXT文本
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// 固定文件名称
const std::string SEND_FIXEDFILE_PARAMETER = "C000";
const std::string SEND_FIXEDFILE_SCREENSCAN = "S000";
const std::string SEND_FIXEDFILE_FIRMWARE = "F001";
const std::string SEND_FIXEDFILE_FIRMWARE2 = "F003";
const std::string SEND_FIXEDFILE_FACTORYFIRMWARE = "F000";
const std::string SEND_FIXEDFILE_FACTORYFIRMWARE2 = "F002";
const std::string SEND_FIXEDFILE_INFORMATION = "I000";
const std::string SEND_FIXEDFILE_LOGO = "A001"; //LOGO文件名称
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// 发送数据类型

//OFS维护命令组
const unsigned short SEND_ACK = 0xA000; //Ack
const unsigned short SEND_NACK = 0xA001; //NAck
const unsigned short SEND_FORMAT = 0xA100; //格式化
const unsigned short SEND_DELETE_FILE = 0xA101; //文件删除
const unsigned short SEND_ADD_FILE = 0xA102; //文件添加
const unsigned short SEND_READ_FILE = 0xA104; //文件读取
const unsigned short SEND_READ_DIR = 0xA106; //读取目录
const unsigned short SEND_MULIADD_STARTFLAG = 0xA108; //发送多节目开始标志；
const unsigned short SEND_MULIADD_ENDFLAG = 0xA109; //发送多节目结束标志；

const unsigned short SEND_MULIADD_FILE = 0xA110; //多文件添加；用于发送多节目时，有发送节目帧头时，发送节目信息改命令字。
const unsigned short SEND_ADDALL_FILE = 0xA1FF; //添加所有节目
//控制器维护命令组
const unsigned short SEND_PING = 0xA200; //Ping命令
const unsigned short SEND_SYSTEMRESET = 0xA201; //系统复位
const unsigned short SEND_RESIVETIME = 0xA202; //校正时间
//FIRMWARE文件维护
const unsigned short SEND_DELETE_FIXEDFILE = 0xA10A; //固定文件删除
const unsigned short SEND_ADD_FIXEDFILE = 0xA10B; //固定文件添加
const unsigned short SEND_ADD_FACTORYFIRMWARE = 0xA1FB; //升级原厂固件程序
const unsigned short SEND_READ_FIXEDFILE = 0xA10D; //固定文件读取

const unsigned short SEND_FIRMWARE_ACTIVATE = 0xA203; //firmware激活
const unsigned short SEND_FIRMWARE_FACTORYRESET_ACTIVATE = 0xA2E3; //firmware恢复并且读出状态

const unsigned short SEND_FIRMWARE_CURRENTSTATUE = 0xA204; //查询当前firmware状态。
const unsigned short SEND_READ_CONTROLID = 0xA20A; //查询控制卡ID编号。
const unsigned short SEND_READ_COPYRIGHT = 0xA20B; //查询控制卡版权信息。
const unsigned short SEND_POWERONOFF = 0xA205; //强制开关机
const unsigned short SEND_TIMERPOWERONOFF = 0xA206; //定时开关机
const unsigned short SEND_UNTIMERPOWERONOFF = 0xA208; //取消定时开关机
const unsigned short SEND_ADJUST_LIGHT = 0xA207; //亮度调整
const unsigned short SEND_QUERY_MEMORY = 0xA112; //查询存储空间容量
const unsigned short SEND_SCREEN_LOGO = 0xA1FE; //发送显示屏LOGO信息
const unsigned short SEND_SCREENSCAN_SWITCHMODE = 0xA2E4; //BX-3A控制器切换两种Firmware模式。
const unsigned short SEND_ETHERNETSET_IP = 0xA401; //设置控制卡的IP地址信息。
const unsigned short SEND_READ_IPINFO = 0xA402; //读取显示屏的IP信息。
const unsigned short SEND_ETHERNETSET_MAC = 0xA400; //设置控制卡的mac地址信息。
const unsigned short SEND_ETHERNETSET_IP_EX = 0xA4FE; //设置控制卡的IP地址信息,带MAC地址过滤。
const unsigned short SEND_ETHERNETSET_IP_CASCADE = 0xA4FF; //设置级联控制卡的IP地址信息、级联列表信息。
const unsigned short SEND_FORMAT_EXTERNALFLASH = 0xA209; //擦除片外FLASH空间。
const unsigned short SEND_SCAN_TIME = 0xA20C; //设定显示优化。
const unsigned short SEND_LOCK_SCREEN = 0xA20D; //屏幕锁定、解锁
const unsigned short SEND_LOCK_PROGRAM = 0xA20E; //锁定节目，解锁。
const unsigned short SEND_MULI_TIMERPOWERON = 0xA2FE; //群组开机
const unsigned short SEND_MULI_TIMERPOWEROFF = 0xA2FF; //群组关机

const unsigned short SEND_SET_ENCRYPTION = 0xA210; //设置控制器加密密码
const unsigned short SEND_ABOLISH_ENCRYPTION = 0xA211; //取消控制器加密密码
const unsigned short SEND_DELAY_START_SECOND = 0xA212; //设置开机延时
const unsigned short SEND_BUTTON_MODE = 0xA213; //设置按钮功能。
const unsigned short SEND_DISPLAY_MODE = 0xA215; //设置控制器显示模式

const unsigned short SEND_READ_SCREEN_STATUS = 0xA20F; //查询控制器状态。

const unsigned short SEND_DYNAMIC_AREA_INFO = 0xA700; //更新动态区域信息
const unsigned short SEND_DELETE_DYNAMIC_AREA = 0xA701; //删除动态区域信息
const unsigned short SEND_DYNAMIC_AREA_PAGE = 0xA702; //更新动态区域页信息
const unsigned short SEND_DELETE_DYNAMIC_AREA_PAGE = 0xA703; //删除动态区域页信息
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// 下载文件类型
const int SEND_FILE_TYPE_PROGRAM = 0; //节目文件类型
const int SEND_FILE_TYPE_PARAMETER = 1; //参数文件类型
const int SEND_FILE_TYPE_SCREENSCAN = 2; //扫描文件类型
const int SEND_FILE_TYPE_PROGRAM_LIST = 3; //播放列表文件类型
const int SEND_FILE_TYPE_FIRMWARE = 4; //Firmware文件类型
const int SEND_FILE_TYPE_FONT = 5; //字库文件类型
const int SEND_FILE_TYPE_ALL = 0xFF; //未指定文件类型
const int SEND_FILE_TYPE_INFORMATION = 7; //提示信息文件类型
const int SEND_FILE_TYPE_LOGO = 8; //显示屏LOGO文件类型
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// 控制器通讯模式
//const int SEND_MODE_COMM = 0;
//const int SEND_MODE_GPRS = 1;
//const int SEND_MODE_NET = 2;
//const int SEND_MODE_USB = 3;
//const int SEND_MODE_WiFi = 4;
//const int SEND_MODE_Server_2G = 5;
//const int SEND_MODE_Server_3G = 6;
//const int SEND_MODE_SAVEFILE = 7; //保存到文件模式。
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// 多机通讯发送状态标志
const int SEND_STATUS_NONEED = 0xFF; //无需发送
const int SEND_STATUS_NOSTART = 0; //未发送。
const int SEND_STATUS_SENDING = 1; //正在发送过程中。
const int SEND_STATUS_COMPLETE = 2; //发送完成。
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// 断点续传读写标志
const int TRANS_STATUS_READ = 0; //读取数据状态
const int TRANS_STATUS_WRITE = 1; //写数据状态
const int TRANS_STATUS_NONE = 2; //无读写状态
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// NOACK返回错误代码
const int ERR_NO = 0; //No error
const int ERR_OUTOFGROUP = 1; //Command Group Error
const int ERR_NOCMD = 2; //Command Not Found
const int ERR_BUSY = 3; //The Controller is busy now
const int ERR_MEMORYVOLUME = 4; // Out of the Memory Volume
const int ERR_CHECKSUM = 5; //CRC16 Checksum Error
const int ERR_FILENOTEXIST = 6; // File Not Exist
const int ERR_FLASH = 7; //Flash Access Error
const int ERR_FILE_DOWNLOAD = 8; // File Download Error
const int ERR_FILE_NAME = 9; //Filename Error
const int ERR_FILE_TYPE = 10; // File type Error
const int ERR_FILE_CRC16 = 11; //File CRC16 Error
const int ERR_FONT_NOT_EXIST = 12; // Font Library Not Exist
const int ERR_FIRMWARE_TYPE = 13; //Firmware Type Error (Check the controller type)
const int ERR_DATE_TIME_FORMAT = 14; // Date Time format error
const int ERR_FILE_EXIST = 15; //File Exist for File overwrite
const int ERR_FILE_BLOCK_NUM = 16; //File block number error
const int ERR_CONTROLLER_TYPE = 17; //Controller Type Error；
const int Err_SCREEN_PARA = 18; //控制器参数越界或错误
const int ERR_CONTROLLER_ID = 19; //读取控制器 ID错误
const int ERR_USER_SECRET = 20; //通讯时用户密码错误
const int ERR_OLD_SECRET = 21; //修改密码时输入的老密码错误
const int ERR_PHY1_NO_SECRET = 22; //通讯时以加密2方式运行提示物理层 PHY1没有设定过密码
const int ERR_PHY1_USE_SECRET = 23; //通讯时以固定加密1方式运行提示物理层 PHY1有设定过密码
const int ERR_OTHER = 0xFE; //Other Error;
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// 不同区域、不同控制卡的特技列表
const std::string SINGLE_TEXT_STUNT[1] = { "Continuous move left" };
const std::string BX3T_BMPTEXT_STUNT[] = { "Random" //随机显示
	, "Static" //静态
	, "Direct show" //快速打出
	, "Move Left" //向左移动
	, "Continuous move left" //向左连移
							 //    , "Move Up" //向上移动
							 //    , "Continuous move up" //向上连移
							 //    , "Flicker" //闪烁
	, "Snowing" //飘雪
	, "Bubbling" //冒泡
	, "Middle Out" //中间移出
	, "Moved Around" //左右移入
	, "Horizontal cross move" //左右交叉移入
	, "Vertical cross move" //上下交叉移入
	, "Scroll Closed" //画卷闭合
	, "Scroll Opened" //画卷打开
	, "Left Stretch" //向左拉伸
	, "Right Stretch" //向右拉伸
	, "Up Stretch" //向上拉伸
				   //    , "Down Stretch" //向下拉伸
	, "Left Laser" //向左镭射
	, "Right Laser" //向右镭射
	, "Up Laser" //向上镭射
	, "Down Laser" //向下镭射
	, "Cross Curtain Left And Right" //左右交叉拉幕
	, "Cross Curtain Up And Down" //上下交叉拉幕
	, "Curtain Scattered To The Left" //分散左拉
									  //    , "Horizontal Blinds" //水平百页
									  //    , "Vertical Blinds" //垂直百页
									  //    , "Curtain Left" //向左拉幕
									  //    , "Curtain Right" //向右拉幕
									  //    , "Curtain Up" //向上拉幕
									  //    , "Curtain Down" //向下拉幕
									  //    , "Move To Center From Left And Right" //左右闭合
									  //    , "Split To Left And Right" //左右对开
									  //    , "Move To Center From Up And Down" //上下闭合
									  //    , "Split To Up And Down" //上下对开
	, "Move Right" //向右移动
	, "Continuum Move Right" //向右连移
//    , "Move Down" //向下移动
//    , "Continuum Move Down" //向下连移
};

const std::string BX3A_BMPTEXT_STUNT[] = { "Random" //随机显示
	, "Static" //静态
	, "Direct show" //快速打出
	, "Move Left" //向左移动
	, "Continuous move left" //向左连移
	, "Move Up" //向上移动
	, "Continuous move up" //向上连移
	, "Flicker" //闪烁
	, "Snowing" //飘雪
	, "Bubbling" //冒泡
	, "Middle Out" //中间移出
	, "Moved Around" //左右移入
	, "Horizontal cross move" //左右交叉移入
	, "Vertical cross move" //上下交叉移入
	, "Scroll Closed" //画卷闭合
	, "Scroll Opened" //画卷打开
	, "Left Stretch" //向左拉伸
	, "Right Stretch" //向右拉伸
	, "Up Stretch" //向上拉伸
	, "Down Stretch" //向下拉伸
	, "Left Laser" //向左镭射
	, "Right Laser" //向右镭射
	, "Up Laser" //向上镭射
	, "Down Laser" //向下镭射
	, "Cross Curtain Left And Right" //左右交叉拉幕
	, "Cross Curtain Up And Down" //上下交叉拉幕
	, "Curtain Scattered To The Left" //分散左拉
									  //    , "Horizontal Blinds" //水平百页
									  //    , "Vertical Blinds" //垂直百页
									  //    , "Curtain Left" //向左拉幕
									  //    , "Curtain Right" //向右拉幕
									  //    , "Curtain Up" //向上拉幕
									  //    , "Curtain Down" //向下拉幕
	, "Move To Center From Left And Right" //左右闭合
	, "Split To Left And Right" //左右对开
	, "Move To Center From Up And Down" //上下闭合
	, "Split To Up And Down" //上下对开
	, "Move Right" //向右连移
	, "Continuum Move Right" //向右连移
	, "Move Down" //向下移动
	, "Continuum Move Down" }; //向下连移

const std::string BXOTHER_A_BMPTEXT_STUNT[] = { "Random" //随机显示
					, "Static" //静态
					, "Direct show" //快速打出
					, "Move Left" //向左移动
					, "Continuous move left" //向左连移
					, "Move Up" //向上移动
					, "Continuous move up" //向上连移
					, "Flicker" //闪烁
					, "Snowing" //飘雪
					, "Bubbling" //冒泡
					, "Middle Out" //中间移出
					, "Moved Around" //左右移入
					, "Horizontal cross move" //左右交叉移入
					, "Vertical cross move" //上下交叉移入
					, "Scroll Closed" //画卷闭合
					, "Scroll Opened" //画卷打开
					, "Left Stretch" //向左拉伸
					, "Right Stretch" //向右拉伸
					, "Up Stretch" //向上拉伸
					, "Down Stretch" //向下拉伸
					, "Left Laser" //向左镭射
					, "Right Laser" //向右镭射
					, "Up Laser" //向上镭射
					, "Down Laser" //向下镭射
					, "Cross Curtain Left And Right" //左右交叉拉幕
					, "Cross Curtain Up And Down" //上下交叉拉幕
					, "Curtain Scattered To The Left" //分散左拉
													  //    , "Horizontal Blinds" //水平百页
													  //    , "Vertical Blinds" //垂直百页
					, "Curtain Left" //向左拉幕
					, "Curtain Right" //向右拉幕
					, "Curtain Up" //向上拉幕
					, "Curtain Down" //向下拉幕
					, "Move To Center From Left And Right" //左右闭合
					, "Split To Left And Right" //左右对开
					, "Move To Center From Up And Down" //上下闭合
					, "Split To Up And Down" //上下对开
					, "Move Right" //向右连移
					, "Continuum Move Right" //向右连移
					, "Move Down" //向下移动
					, "Continuum Move Down" }; //向下连移

const std::string BXOTHER_BMPTEXT_STUNT[] = { "Random" //随机显示
										 , "Static" //静态
										 , "Direct show" //快速打出
										 , "Move Left" //向左移动
										 , "Continuous move left" //向左连移
										 , "Move Up" //向上移动
										 , "Continuous move up" //向上连移
										 , "Flicker" //闪烁
										 , "Snowing" //飘雪
										 , "Bubbling" //冒泡
										 , "Middle Out" //中间移出
										 , "Moved Around" //左右移入
										 , "Horizontal cross move" //左右交叉移入
										 , "Vertical cross move" //上下交叉移入
										 , "Scroll Closed" //画卷闭合
										 , "Scroll Opened" //画卷打开
										 , "Left Stretch" //向左拉伸
										 , "Right Stretch" //向右拉伸
										 , "Up Stretch" //向上拉伸
										 , "Down Stretch" //向下拉伸
										 , "Left Laser" //向左镭射
										 , "Right Laser" //向右镭射
										 , "Up Laser" //向上镭射
										 , "Down Laser" //向下镭射
										 , "Cross Curtain Left And Right" //左右交叉拉幕
										 , "Cross Curtain Up And Down" //上下交叉拉幕
										 , "Curtain Scattered To The Left" //分散左拉
										 , "Horizontal Blinds" //水平百页
															   //    , "Vertical Blinds" //垂直百页
										 , "Curtain Left" //向左拉幕
										 , "Curtain Right" //向右拉幕
										 , "Curtain Up" //向上拉幕
										 , "Curtain Down" //向下拉幕
										 , "Move To Center From Left And Right" //左右闭合
										 , "Split To Left And Right" //左右对开
										 , "Move To Center From Up And Down" //上下闭合
										 , "Split To Up And Down" //上下对开
										 , "Move Right" //向右连移
										 , "Continuum Move Right" //向右连移
										 , "Move Down" //向下移动
										 , "Continuum Move Down" }; //向下连移
//------------------------------------------------------------------------------

//==============================================================================
// 函数返回代码值
//const int RETURN_ERROR_NOFIND_DYNAMIC_AREA = 0xE1; //没有找到有效的动态区域。
//const int RETURN_ERROR_NOFIND_DYNAMIC_AREA_FILE_ORD = 0xE2; //在指定的动态区域没有找到指定的文件序号。
//const int RETURN_ERROR_NOFIND_DYNAMIC_AREA_PAGE_ORD = 0xE3; //在指定的动态区域没有找到指定的页序号。
//const int RETURN_ERROR_NOSUPPORT_FILETYPE = 0xE4; //不支持该文件类型。
//const int RETURN_ERROR_RA_SCREENNO = 0xF8; //已经有该显示屏信息。如要重新设定请先DeleteScreen删除该显示屏再添加；
//const int RETURN_ERROR_NOFIND_AREA = 0xFA; //没有找到有效的显示区域；可以使用AddScreenProgramBmpTextArea添加区域信息。
//const int RETURN_ERROR_NOFIND_SCREENNO = 0xFC; //系统内没有查找到该显示屏；可以使用AddScreen函数添加显示屏
//const int RETURN_ERROR_NOW_SENDING = 0xFD; //系统内正在向该显示屏通讯，请稍后再通讯；
//const int RETURN_ERROR_OTHER = 0xFF; //其它错误；
//const int RETURN_NOERROR = 0; //没有错误
//==============================================================================

//--------------------------------------------------
//自适应用到的常量
const int clred = 0;
const int cllime = 1;
const int clYellow = 2;
const int Colors[3] = { clred, cllime, clYellow }; //
const std::string ymdlist[] = {
	"2003年12月30日", "00年12月30日", "12/30/2000", "2000/12/30", "00-12-30", "00.12.30",
	"12月30日", "12.30.2000", "2000-12-30", "30 12 2000", "30:12:2000", "30:00:00",
	"2000", "12", "30" };
const std::string ymdreallist[] = {
"[Y1]年[M1]月[D1]日", "[Y2]年[M1]月[D1]日", "[M1]/[D1]/[Y1]", "[Y1]/[M1]/[D1]",
"[Y2]-[M1]-[D1]", "[Y2].[M1].[D1]", "[M1]月[D1]日", "[M1].[D1].[Y1]",
"[Y1]-[M1]-[D1]", "[D1] [M1] [Y1]", "[D1]:[M1]:[Y1]", "[D1]:[M1]:[Y2]",
"[Y1]", "[M1]", "[D1]" };
const std::string weeklist[] = { "星期一", "Wednesday", "Tues" };
const std::string timelist[] = {
	"20时59分59秒", "20時59分59秒", "20:59:59", "20 59 59", "8 59 59", "20时59分",
	"20時59分", "20:59", "上午", "AM", "20(时)", "59(分)", "59(秒)", "上午 8:59",
	"AM 8:59", "8:59 AM", "AM 8 59", "8 59 AM", "8:59 上午" };
const std::string timereallist[] = {
	"[H1]时[N1]分[S1]秒", "[H1]時[N1]分[S1]秒", "[H1]:[N1]:[S1]", "[H1] [N1] [S1]", "[H2] [N1] [S1]",
	"[H1]时[N1]分", "[H1]時[N1]分", "[H1]:[N1]", "[P1]", "[P2]", "[H1]", "[N1]", "[S1]", "[P1] [H2]:[N1]",
	"[P2] [H2]:[N1]", "[H2]:[N1] [P2]", "[P2] [H2] [N1]", "[H2] [N1] [P2]", "[H2]:[N1] [P1]" };
const std::string stylearr[] = {
"year_list", "month_list", "day_list", "hour_list", "minute_list", "second_list",
"ampm_list", "week_list", "bp_list", "js_list", "jr_list", "ymd_list", "hns_list" };
//-----------------------------------------------------------------------

void CreateFrameArray(int singleColorCount, int multiColorCount);
void GetControllerList(Json::Value &ControllerList_Obj, Json::Value &Controller_Supary);

int GetSelScreenArrayOrd(int nScreenNo, const Json::Value &Screen_Ja);
Json::Value GetControllerObject(const Json::Value &Controller_Supary, int nControllerType, unsigned char &nProtocolVer);
void GetController_DecomposeType(int nControllerType, unsigned char &nTypeValue1, unsigned char &nTypeValue2);
Json::Value GetDefaultController(unsigned char &nProtocolVer, int &nControllerType);
int GetControllerType(Json::Value Controller_obj);