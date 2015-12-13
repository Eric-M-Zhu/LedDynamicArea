#pragma once

//void LanguageUse(TComponent AnObject; int lan_i);
void SaveBinFile(const char *pdata, UINT nDataLen, string szFileName);
//void ClearTransitList(TList &lstTransit);
//判断中转服务器类型控制器是否在线。True：在线；否则不在线。
//bool GetTransitScreenStatus(TList lstTransit; UINT ScreenOrd; TSuperArray Screen_Ary);
bool Judge_Scan_File_Detail_Exist(Json::Value ScanObj, string szSmartScanPath, string &szSameScanName);
bool SaveScanData(Json::Value SmartScanFile_root, Json::Value ScanObj,
	string szSmartScanPath, string szcurScanName, string szScanFile);
string Transit_Message(BYTE errid);