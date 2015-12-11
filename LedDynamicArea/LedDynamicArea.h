#ifndef _LEDDYNAMICAREA_H
#define _LEDDYNAMICAREA_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifdef _USRDLL
#define LEDDYNAMICAREA_LIB_API __declspec(dllexport)
#else
#define LEDDYNAMICAREA_LIB_API __declspec(dllimport)
#endif

	// 控制器类型
	typedef enum
	{
		BX_5E1	=	0x0154,
		BX_5E2	=	0x0254,
		BX_5E3	=	0x0354,
		BX_5Q0P	=	0x1056,
		BX_5Q1P	=	0x1156,
		BX_5Q2P	=	0x1256,
	} ControllerType;

	#define	FRAME_SINGLE_COLOR_COUNT	23	// 纯色边框图片个数
	#define	FRAME_MULI_COLOR_COUNT		47	// 花色边框图片个数

	// 通讯模式
	typedef enum
	{
		SEND_MODE_SERIALPORT	=	0,
		SEND_MODE_NETWORK		=	2,
		SEND_MODE_Server_2G		=	5,
		SEND_MODE_Server_3G		=	6,
		SEND_MODE_SAVEFILE		=	7,
	} CommunicationMode;

	// 动态区域运行模式
	typedef enum
	{
		RUN_MODE_CYCLE_SHOW					=	0, //动态区数据循环显示；
		RUN_MODE_SHOW_LAST_PAGE				=	1, //动态区数据显示完成后静止显示最后一页数据；
		RUN_MODE_SHOW_CYCLE_WAITOUT_NOSHOW	=	2, //动态区数据循环显示，超过设定时间后数据仍未更新时不再显示；
		RUN_MODE_SHOW_ORDERPLAYED_NOSHOW	=	4, //动态区数据顺序显示，显示完最后一页后就不再显示
	} DynamicAreaRunMode;

	// 返回状态代码定义
	#define RETURN_NOERROR								0    // 没有错误
	#define RETURN_ERROR_NOFIND_DYNAMIC_AREA			0xE1 // 没有找到有效的动态区域
	#define RETURN_ERROR_NOFIND_DYNAMIC_AREA_FILE_ORD	0xE2 // 在指定的动态区域没有找到指定的文件序号
	#define RETURN_ERROR_NOFIND_DYNAMIC_AREA_PAGE_ORD	0xE3 // 在指定的动态区域没有找到指定的页序号
	#define RETURN_ERROR_NOSUPPORT_FILETYPE				0xE4 // 不支持该文件类型
	#define RETURN_ERROR_RA_SCREENNO					0xF8 // 已经有该显示屏信息. 如要重新设定请先DeleteScreen删除该显示屏再添加
	#define RETURN_ERROR_NOFIND_AREA					0xFA // 没有找到有效的显示区域. 可以使用AddScreenProgramBmpTextArea添加区域信息
	#define RETURN_ERROR_NOFIND_SCREENNO				0xFC // 系统内没有查找到该显示屏. 可以使用AddScreen函数添加显示屏
	#define RETURN_ERROR_NOW_SENDING					0xFD // 系统内正在向该显示屏通讯. 请稍后再通讯
	#define RETURN_ERROR_OTHER							0xFF // 其它错误

	LEDDYNAMICAREA_LIB_API int __stdcall Initialize();
	LEDDYNAMICAREA_LIB_API int __stdcall Uninitialize();
	LEDDYNAMICAREA_LIB_API int __stdcall AddScreen_Dynamic(int nControlType, int nScreenNo, int nSendMode,
		int nWidth, int nHeight, int nScreenType, int nPixelMode, const char *pCom, int nBaud, const char *pSocketIP, int nSocketPort,
		int nServerMode, const char *pBarcode, const char *pNetworkID, const char *pServerIP, int nServerPort,
		const char *pServerAccessUser, const char *pServerAccessPassword, const char *pCommandDataFile);
	LEDDYNAMICAREA_LIB_API int __stdcall AddScreenDynamicArea(int nScreenNo, int nDYAreaID, int nRunMode, int nTimeOut, int nAllProRelate,
		const char *pProRelateList, int nPlayPriority, int nAreaX, int nAreaY, int nAreaWidth, int nAreaHeight,
		int nAreaFMode, int nAreaFLine, int nAreaFColor, int nAreaFStunt, int nAreaFRunSpeed, int nAreaFMoveStep);
	LEDDYNAMICAREA_LIB_API int __stdcall AddScreenDynamicAreaText(int nScreenNo, int nDYAreaID,
		const char *pText, int nShowSingle, const char *pFontName, int nFontSize,
		int nBold, int nFontColor, int nStunt, int nRunSpeed, int nShowTime);
	LEDDYNAMICAREA_LIB_API int __stdcall AddScreenDynamicAreaFile(int nScreenNo, int nDYAreaID,
		const char *pFileName, int nShowSingle, const char *pFontName, int nFontSize, int nBold, int nFontColor,
		int nStunt, int nRunSpeed, int nShowTime);
	LEDDYNAMICAREA_LIB_API int __stdcall DeleteScreen_Dynamic(int nScreenNo);
	LEDDYNAMICAREA_LIB_API int __stdcall DeleteScreenDynamicArea(int nScreenNo, int nDYAreaID);
	LEDDYNAMICAREA_LIB_API int __stdcall DeleteScreenDynamicAreaFile(int nScreenNo, int nDYAreaID, int nFileOrd);

#undef LEDDYNAMICAREA_LIB_API

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _LEDDYNAMICAREA_H


