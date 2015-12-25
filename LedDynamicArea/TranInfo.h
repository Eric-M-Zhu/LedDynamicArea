#pragma once

//string GetPingCommandInfo(DWORD nProtocolVer, BYTE nRtnReq); //Ping命令。
//string GetReviseTimeInfo(DWORD nYear, DWORD nMonth, DWORD nDay, DWORD nHour, DWORD nMinute, DWORD nSecond, DWORD nWeek,
//	DWORD nProtocolVer, BYTE nRtnReq); //校正时间命令;nWeek:周一为1，周日为7
//string GetStartFileFlagInfo(DWORD nProtocolVer, BYTE nRtnReq); //发送多节目开始标志信息。
//string GetEndFileFlagInfo(DWORD nProtocolVer, BYTE nRtnReq); //发送多节目结束标志信息
//																			 //nSendCmd：命令组；nFileType：要读取的文件类型；$FF:读取所有文件类型
//string GetReadDir(DWORD nSendCmd, DWORD nFileType,
//	DWORD nProtocolVer, DWORD nMaxReadDir, BYTE nRtnReq); //读取目录
//string GetMemoryVolume(DWORD nProtocolVer, BYTE nRtnReq); //查询文件系统容量。
//string GetDeleteProgramFile(list<DWORD> &SendProgramList, list<DWORD> QueryProgramList,
//	DWORD &nDeleteCount, DWORD &nDeleteSectors, DWORD &nValidProgramSectors,
//	DWORD nProtocolVer, BYTE nRtnReq); //得到当前要删除的文件列表
//string DeleteAllProgramFiles(DWORD nProtocolVer, BYTE nRtnReq);
//string GetFormatInfo(DWORD nProtocolVer, BYTE nRtnReq); //格式化命令
//string GetDeleteAllProgram(DWORD nProtocolVer, BYTE nRtnReq); //删除所有节目信息。
//string GetPowerONOFF(BYTE nPowerStatus, DWORD nProtocolVer, BYTE nRtnReq); //强制开关机命令信息
//string GetTimerONOFFInfo(Json::Value Screen_ja, DWORD nScreenOrd, DWORD nProtocolVer, BYTE nRtnReq); //定时开关机信息
//string GetUnTimerONOFFInfo(DWORD nProtocolVer, BYTE nRtnReq); //取消定时开关机信息
//string GetAdjustLightInfo(Json::Value Screen_ja, DWORD nScreenOrd, DWORD nProtocolVer, BYTE nRtnReq); //调亮信息
//string GetReadControlID(DWORD nProtocolVer, BYTE nRtnReq); //读取控制卡ID号码
//string GetReadCopyrightInfo(DWORD nProtocolVer, BYTE nRtnReq); //读取控制卡版权信息
//string GetDisplayOptimization(DWORD nScanTime, DWORD nProtocolVer, BYTE nRtnReq); //得到设定单CPU架构控制卡显示优化信息。
//string GetScreenLockScreenStatusInfo(BYTE nLock_Status, BYTE nNonvolatile, DWORD nProtocolVer, BYTE nRtnReq); //nLock_Status:0:解锁；1:锁定；nNonvolatile:0:掉电不保存；1:掉电保存；
//string GetScreenLockProgramStatusInfo(string szProgramName, BYTE nLock_Status, BYTE nNonvolatile,
//	DWORD nProtocolVer, BYTE nRtnReq); //szProgramName:节目名称；nLock_Status:0:解锁；1:锁定；nNonvolatile:0:掉电不保存；1:掉电保存；
//string GetReadFile(DWORD nSendCmd, string szFileName, DWORD nProtocolVer, BYTE nRtnReq); //读取文件；
//string GetReadScreenStatusInfo(DWORD nProtocolVer, BYTE nRtnReq); //读取控制器状态
//string GetSystemResetInfo(DWORD nProtocolVer, BYTE nRtnReq); //系统复位命令
//
//string GetSetPasswordInfo(string szOldPassword, string szNewPassword, DWORD nProtocolVer, BYTE nRtnReq); //设定控制器密码
//string GetDeletePasswordInfo(Json::Value Screen_ja, DWORD nScreenOrd, DWORD nProtocolVer, BYTE nRtnReq); //清除控制器密码
//
//string GetControlDelayStartSecondInfo(DWORD nSecond,
//	DWORD nProtocolVer, BYTE nRtnReq); //转换开机延时数据
//string GetControlDisplayModeInfo(DWORD nDisplayMode,
//	DWORD nProtocolVer, BYTE nRtnReq); //设定显示屏显示模式数据
//
//string GetControlButtonModeInfo(BYTE nMode,
//	DWORD nProtocolVer, BYTE nRtnReq);
//string GetScreenLOGOInfo(Json::Value Screen_ja, DWORD nScreenOrd, DWORD &nSendLength,
//	DWORD nProtocolVer, BYTE nRtnReq);
//string GetDleteScreenLOGOInfo(string szLOGOFileNamde,
//	DWORD nProtocolVer, BYTE nRtnReq);
//string GetScreenParameterInfo(Json::Value Screen_ja, int nScreenOrd, BYTE ntmpBXCEColor);
//string GetScanData(DWORD nControlType, ptagScreenFlowItem pScreenFlowItem, DWORD nIndex); //得到扫描配置的参数信息
//void GetReadFileDirList(string szReadBuf, list<DWORD> QueryProgramList);
//string GetScreenMessage(DWORD nControlType);
//
//string GetFirmwareFormatInfo(string szFirmwareName);
//string GetFirmwareActivateInf(string szFirmwareFile,
//	DWORD nProtocolVer, BYTE nRtnReq); //激活控制器程序程序；给客户只有一个控制器程序程序，因此名称固定为"F001"，原厂名称固定为"F000"
//string GetQueryFirmwareStatus(DWORD nProtocolVer, BYTE nRtnReq); //查询当前Firmware状态
//string GetFormatExternalFlash(DWORD nProtocolVer, BYTE nRtnReq);
//string SetScreenIPAddressInfo(Json::Value Screen_ja, int nScreenOrd,
//	DWORD nProtocolVer, BYTE nRtnReq,
//	string szIP, string szSubMask, string szGateWay,
//	DWORD nPort, DWORD nServerMode, DWORD nServerPort, DWORD nHeartBeatInterval,
//	string szServerIP, string szServerAccessPassword, string szNetID,
//	bool bWildCardsSend, string szMacInfo);
//string SetScreenNetMACInfo(Json::Value Screen_ja, int nScreenOrd,
//	DWORD nProtocolVer, BYTE nRtnReq);
//
//string GetReadScreenTCPIPInfo(DWORD nProtocolVer, BYTE nRtnReq);
//string GetTransStatus(DWORD nProtocolVer, BYTE nRtnReq);
//bool GetTransStatusInfo(string szReadBuf, DWORD nReadLength, BYTE nTransRW,
//	string &szTransFileName, string &szTransFileCrc, DWORD &nTransFileOffset);
SYSTEMTIME GetVersionDateTime(string szFirmWareVersion);
//SYSTEMTIME GetVersionDateTimeFifthProtocol(string szFirmWareVersion);
//
//DWORD GetAllSendProgramSectors(list<DWORD> &SendProgramList);
////void Bublerank(var a : array of string; var b : array of Cardinal; maxtomin: boolean); //冒泡算法将时段排序开；MaxtoMin=True，从高到底；False，从低到高
//string AutoGetUserMAC(SYSTEMTIME dtDateTime);
DWORD GetControlStuntRealityRunSpeed(DWORD nWidth, DWORD nHeight, DWORD nScreenColor, DWORD nStuntIndex, DWORD nRunSpeed, DWORD &nDefaultRunSpeed);
DWORD GetControlFrameStuntRealityRunSpeed(DWORD nWidth, DWORD nHeight, DWORD nScreenColor, DWORD nStuntIndex, DWORD nRunSpeed, DWORD &nDefaultRunSpeed);
//DWORD GetControlStuntDefaultRunSpeed(DWORD nControlType, DWORD nWidth, DWORD nHeight, DWORD nScreenColor); //根据控制卡型号、面积计算出默认特技的运行速度等级
//DWORD GetControlFrameStuntDefaultRunSpeed(DWORD nWidth, DWORD nHeight, DWORD nScreenColor); //根据控制卡型号、面积计算出边框默认特技的运行速度等级
//tagServerModeLookUpRecord GetServerModeLookUp(string szReadInfo, DWORD nReadLength);
////nFileType：文件类型;
//// nControlType：控制器类型；
//// szFirmwareVersion:控制器版本；4位字符串；
//// szFirmwareDatetime控制器修改日期；19位字符串；
////szFilrmware:下载控制器程序文件名；格式‘F***’
//// szFirmwareName: 下载控制器程序数据源文件名。
//
//DWORD GetMAXMovieExtractFrameCount(DWORD nLeft, DWORD nWidth, DWORD nHeight, DWORD nScreenType, DWORD nPixType);
//
string MakeDeleteDynamicAreas(string szDelAreaInfo, DWORD &nSendLength);
string MakeDeleteDynamicAreaPages(DWORD nDYAreaID, string szDelPageInfo, DWORD &nSendLength);
