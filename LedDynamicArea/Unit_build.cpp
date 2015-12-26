#include "stdafx.h"
#include "Unit_build.h"

#include "MyDataType.h"
#include "TranInfo.h"

#define NumCharSets 19

extern vector<DWORD> g_aryProFrameSingleColorBmp;
extern vector<DWORD> g_aryProFrameMuliColorBmp;

TCharSetRec CharSets[NumCharSets] = { { "ANSI_CHARSET",  0},
	{"DEFAULT_CHARSET",  1},
	{"SYMBOL_CHARSET",  2},
	{"SHIFTJIS_CHARSET",  0x80},
	{"HANGEUL_CHARSET",  129},
	{"GB2312_CHARSET",  134},
	{"CHINESEBIG5_CHARSET",  136},
	{"OEM_CHARSET",  255},
	{"JOHAB_CHARSET",  130},
	{"HEBREW_CHARSET",  177},
	{"ARABIC_CHARSET",  178},
	{"GREEK_CHARSET",  161},
	{"TURKISH_CHARSET",  162},
	{"VIETNAMESE_CHARSET",  163},
	{"THAI_CHARSET",  222},
	{"EASTEUROPE_CHARSET",  238},
	{"RUSSIAN_CHARSET",  204},
	{"MAC_CHARSET",  77},
	{"BALTIC_CHARSET",  186} };

void GetProgramFrameColorBmp(BYTE *pBits, DWORD nWidth, DWORD nHeight, DWORD nColorOrd);

int GetMaxInArray(int A[], size_t size)
{
	int tmpMax;

	if (size <= 0)
	{
		return 0;
	}

	tmpMax = A[0];
	for (size_t i = 1; i < size; ++i)
	{
		if (A[i] > tmpMax)
		{
			tmpMax = A[i];
		}
	}

	return tmpMax;
}

SYSTEMTIME StringToDatetime(string &DateStr) //将yyyy-mm-dd hh:nn:ss格式的字符串转为时间格式
{
	SYSTEMTIME dateTime;

	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;

	sscanf(DateStr.c_str(), "%d-%d-%d %d:%d:%d", &year, &month, &day,
		&hour, &minute, &second);

	dateTime.wYear = (WORD)year;
	dateTime.wMonth = (WORD)month;
	dateTime.wDay = (WORD)day;
	dateTime.wHour = (WORD)hour;
	dateTime.wMinute = (WORD)minute;
	dateTime.wSecond = (WORD)second;

	return dateTime;
}

list<string> SplitString(const string &Source, const string &Deli)
{
	list<string> result;
	char *pBuffer = new char[Source.size() + 1];
	char *pToken = NULL;

	strcpy(pBuffer, Source.c_str());
	pToken = strtok(pBuffer, Deli.c_str());

	while (pToken)
	{
		result.push_back(pToken);
		pToken = strtok(NULL, Deli.c_str());
	}

	delete[] pBuffer;

	return result;
}

DWORD calpagesize(DWORD left, DWORD width, DWORD nPx, DWORD nKardPixType)
{
	DWORD nStartPoint, nEndPoint;
	DWORD nEndSpace, nStartSpace, nAllWidth;
	DWORD Result = width;

	switch (nPx)
	{
	case SCREEN_COLOR_SINGLE:
	case SCREEN_COLOR_DOUBLE:
	case SCREEN_COLOR_THREE:
		/////////////////////////协议的头
		nStartPoint = nKardPixType; //左边有效位数。
		if ((left % nKardPixType) != 0)
			nStartPoint = nKardPixType - left % nKardPixType;

		nEndPoint = nKardPixType; //右边的有效位数
		if ((left + width) % nKardPixType != 0)
			nEndPoint = (left + width) % nKardPixType;

		nStartSpace = left % nKardPixType; //左边的无效位数；

		nEndSpace = 0; //右边的无效位数
		if ((left + width) % nKardPixType != 0)
			nEndSpace = nKardPixType - (left + width) % nKardPixType;

		nAllWidth = width + nStartSpace + nEndSpace; //得到转换后的实际宽度
		Result = nAllWidth;
		break;
	case SCREEN_COLOR_FULLCOLOR:
		Result = width;
		break;
	}

	return Result;
}

string MakeNULLArea(string szAreaName, DWORD w, DWORD h, DWORD x, DWORD nPx, DWORD nMkStyle, DWORD nKardPixType,
	DWORD nScreenStyle, DWORD nStunt, DWORD nOutStunt, DWORD nRunSpeed, DWORD nShowTime,
	DWORD nShowCount, DWORD nReserved1, DWORD nReserved2, DWORD nReserved3, DWORD nReserved4, DWORD nReserved5,
	DWORD nReserved6, DWORD nReserved7)
{
	LPBYTE mBmpPage;
	string szBmpTextBuf;
	DWORD nAllWidth, nPageSize;
	DWORD nCurAddress;
	DWORD ncurPageAllLength;
	string szcurPagebuf;
	bool bInvalidData;
	HWND hDestopWnd = GetDesktopWindow();
	HDC hDesktopDC = GetWindowDC(hDestopWnd);
	HDC hMemoryDC = CreateCompatibleDC(hDesktopDC);
	BITMAPINFO bmpInfo;
	int bytesPerLine = (w * 3 + 1) / 2 * 2;

	nAllWidth = calpagesize(x, w, nPx, nKardPixType);
	memset(&bmpInfo, 0, sizeof(BITMAPINFOHEADER));
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biWidth = w;
	bmpInfo.bmiHeader.biHeight = h;
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biSizeImage = bytesPerLine * h;
	bmpInfo.bmiHeader.biBitCount = 24;

	HBITMAP hCanvasBmp = CreateDIBSection(hMemoryDC, &bmpInfo, DIB_RGB_COLORS, (void**)&mBmpPage, NULL, 0);
	HBITMAP hOldBmp = (HBITMAP)SelectObject(hMemoryDC, hCanvasBmp);
	HBRUSH hBlackBrush = CreateSolidBrush(RGB(0, 0, 0));
	RECT rect = { 0, 0, (LONG)w, (LONG)h };
	HFONT hFont = CreateFont(10, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, TEXT("Tahoma"));

	FillRect(hMemoryDC, &rect, hBlackBrush);
	SetTextColor(hMemoryDC, RGB(255, 0, 0));
	SelectObject(hMemoryDC, hFont);
	TextOutA(hMemoryDC, 0, 0, szAreaName.c_str(), szAreaName.size());

	szcurPagebuf = "";
	szcurPagebuf.push_back(char(0)); //数据类型
	szcurPagebuf.push_back(char(nStunt));
	szcurPagebuf.push_back(char(nOutStunt));
	szcurPagebuf.push_back(char(nRunSpeed));
	szcurPagebuf.push_back(char(nShowTime & 0xFF));
	szcurPagebuf.push_back(char((nShowTime >> 8) & 0xFF));
	szcurPagebuf.push_back(char(nShowCount));
	szcurPagebuf.push_back(char(nReserved1)); //保留字
	szcurPagebuf.push_back(char(nReserved2)); //保留字
	szcurPagebuf.push_back(char(nReserved3)); //保留字
	szcurPagebuf.push_back(char(nReserved4)); //保留字
	szcurPagebuf.push_back(char(nReserved5)); //保留字
	szcurPagebuf.push_back(char(nReserved6));; //保留字

	nCurAddress= szcurPagebuf.size();
	nPageSize= GetPageSize(nAllWidth, h, nPx);
	//SetLength(szcurPagebuf, nCurAddress + nPageSize);
	//Inc(nCurAddress);
	TranCanvToInfo(mBmpPage, szcurPagebuf, x, w, h, nPx, nMkStyle,
		nKardPixType, nScreenStyle, bInvalidData);
	ncurPageAllLength = szcurPagebuf.size();

	szBmpTextBuf.push_back((char)(ncurPageAllLength & 0xFF));
	szBmpTextBuf.push_back((char)((ncurPageAllLength >> 8) & 0xFF));
	szBmpTextBuf.push_back((char)((ncurPageAllLength >> 16) & 0xFF));
	szBmpTextBuf.push_back((char)((ncurPageAllLength >> 24) & 0xFF));
	szBmpTextBuf.append(szcurPagebuf);

	SelectObject(hMemoryDC, hOldBmp);
	DeleteObject(hBlackBrush);
	DeleteObject(hFont);
	DeleteObject(hCanvasBmp);
	DeleteDC(hMemoryDC);
	ReleaseDC(hDestopWnd, hDesktopDC);

	return szBmpTextBuf;
}

string MakeDynamicAreaInfo(DWORD nScreenOrd, DWORD nDYAreaOrd, Json::Value Screen_ja,
	DWORD nSendLength)
{
	string Result;
	string szSendBuf, szTWSendBuf;
	DWORD I, J, /*nCurAddress,*/ nDataLength;
	BYTE nRunMode, nAllProRelate, nImmePlay;
	DWORD nTimeOut, nRelateProNum, nRelateProID, nAreaX, nAreaY, nAreaWidth, nAreaHeight;
	DWORD nTotlePagecount, ntmpPageCount;
	DWORD nScreenWidth, nScreenHeight, nScreenColor, nScreenPixels, nScreenPixType, nScreenStyle;
	DWORD nRealX, nRealY, nRealW, nRealH;
	TRVStyle t_rvstyle = 0;
	TRVReportHelper t_RVReportHelper = 0;
	TTntMemo t_memo = 0; ///主要操作txt文件

	nRunMode = (BYTE)Screen_ja[(int)nScreenOrd]["Screen_lstDYArea"][(int)nDYAreaOrd]["DY_RunMode"].asUInt();
	if ((nRunMode < 0) || (nRunMode > 4))
		nRunMode = 0;

	nTimeOut = Screen_ja[(int)nScreenOrd]["Screen_lstDYArea"][(int)nDYAreaOrd]["DY_TimeOut"].asUInt();
	nAllProRelate = (BYTE)Screen_ja[(int)nScreenOrd]["Screen_lstDYArea"][(int)nDYAreaOrd]["DY_AllProRelate"].asUInt();

	if (nAllProRelate != 0)
		nAllProRelate = 1;
	nImmePlay = (BYTE)Screen_ja[(int)nScreenOrd]["Screen_lstDYArea"][(int)nDYAreaOrd]["DY_PlayImmediately"].asUInt();
	nAreaX = Screen_ja[(int)nScreenOrd]["Screen_lstDYArea"][(int)nDYAreaOrd]["DY_AreaX"].asUInt();
	nAreaY = Screen_ja[(int)nScreenOrd]["Screen_lstDYArea"][(int)nDYAreaOrd]["DY_AreaY"].asUInt();
	nAreaWidth = Screen_ja[(int)nScreenOrd]["Screen_lstDYArea"][(int)nDYAreaOrd]["DY_AreaWidth"].asUInt();
	nAreaHeight = Screen_ja[(int)nScreenOrd]["Screen_lstDYArea"][(int)nDYAreaOrd]["DY_AreaHeight"].asUInt();
	nRelateProNum = Screen_ja[(int)nScreenOrd]["Screen_lstDYArea"][(int)nDYAreaOrd]["DY_ProRelateList"].size();

	nScreenWidth = Screen_ja[(int)nScreenOrd]["Screen_Width"].asUInt();
	nScreenHeight = Screen_ja[(int)nScreenOrd]["Screen_Height"].asUInt();
	nScreenColor = Screen_ja[(int)nScreenOrd]["Screen_color"].asUInt();
	nScreenPixels = Screen_ja[(int)nScreenOrd]["Screen_pixels"].asUInt();
	nScreenPixType = Screen_ja[(int)nScreenOrd]["Screen_nPixType"].asUInt();
	nScreenStyle = Screen_ja[(int)nScreenOrd]["Screen_control"].asUInt();

	//------------------------------------------------------------------------------
	// 所需控件初始化
	/*t_rvstyle = trvstyle.Create(nil);
	t_rvstyle.Color  = clblack;
	t_rvstyle.TextStyles.Add;
	t_rvstyle.ParaStyles.Add;
	t_rvstyle.TextStyles.Add.Unicode  = true;
	t_rvstyle.ParaStyles.Add.Standard  = true;
	t_RVReportHelper = tRVReportHelper.Create(nil);
	t_RVReportHelper.RichView.Style  = t_rvstyle;
	t_RVReportHelper.RichView.Parent  = Frm_parent;
	t_RVReportHelper.RichView.Visible  = false;
	t_RVReportHelper.RichView.LeftMargin  = 0;
	t_RVReportHelper.RichView.BottomMargin  = 0;
	t_RVReportHelper.RichView.RightMargin  = 0;
	t_RVReportHelper.RichView.TopMargin  = 0;
	t_RVReportHelper.RichView.RTFReadProperties.UnicodeMode  = rvruOnlyUnicode;
	t_RVReportHelper.RichView.RTFReadProperties.TextStyleMode  = rvrsAddIfNeeded;
	t_RVReportHelper.RichView.RTFReadProperties.ParaStyleMode  = rvrsAddIfNeeded;*/

	/*t_memo = TTntMemo.Create(nil);
	t_memo.left  = 200;
	t_memo.top  = 10;
	t_memo.Width  = 100;
	t_memo.height  = 100;
	t_memo.Font.Color  = clred;
	t_memo.Font.Charset  = DEFAULT_CHARSET;
	t_memo.Font.Name  = 'Tahoma';
	t_memo.Font.Size  = 12;
	t_memo.Font.Style  = [];
	t_memo.BorderStyle  = bsNone;
	t_memo.Ctl3D  = false;
	t_memo.Parent  = Frm_parent;
	t_memo.Visible  = false;*/
	//------------------------------------------------------------------------------

	szSendBuf.clear();
	szSendBuf.push_back((char)Screen_ja[(int)nScreenOrd]["Screen_lstDYArea"][(int)nDYAreaOrd]["DY_AreaID"].asUInt());
	szSendBuf.push_back((char)nRunMode);
	szSendBuf.push_back((char)(nTimeOut & 0xFF));
	szSendBuf.push_back((char)((nTimeOut >> 8) & 0xFF));
	szSendBuf.push_back((char)nAllProRelate);
	szSendBuf.push_back((char)(nRelateProNum & 0xFF));
	szSendBuf.push_back((char)((nRelateProNum >> 8) & 0xFF));

	for (I = 0; I < nRelateProNum; ++I)
	{
		nRelateProID = Screen_ja[(int)nScreenOrd]["Screen_lstDYArea"][(int)nDYAreaOrd]["DY_ProRelateList"][(int)I]["RelateProID"].asUInt();
		szSendBuf.push_back((char)(nRelateProID & 0xFF));
		szSendBuf.push_back((char)((nRelateProID >> 8) & 0xFF));
	}

	szSendBuf.push_back((char)(nImmePlay));
	szSendBuf.push_back((char)(0));
	szSendBuf.push_back((char)(0));
	szSendBuf.push_back((char)(0));
	szSendBuf.push_back((char)(0));
	szSendBuf.push_back((char)(0x10));
	szSendBuf.push_back((char)(nAreaX & 0xFF));
	szSendBuf.push_back((char)((nAreaX >> 8) & 0xFF));
	szSendBuf.push_back((char)(nAreaY & 0xFF));
	szSendBuf.push_back((char)((nAreaY >> 8) & 0xFF));
	szSendBuf.push_back((char)(nAreaWidth & 0xFF));
	szSendBuf.push_back((char)((nAreaWidth >> 8) & 0xFF));
	szSendBuf.push_back((char)(nAreaHeight & 0xFF));
	szSendBuf.push_back((char)((nAreaHeight >> 8) & 0xFF));

	szSendBuf = szSendBuf + MakeFrameValue(Screen_ja[(int)nScreenOrd], nDYAreaOrd);
	GetRealDYAreaLocation(Screen_ja[(int)nScreenOrd]["Screen_lstDYArea"][(int)nDYAreaOrd], nRealX, nRealY, nRealW, nRealH);
	nTotlePagecount = 0;
	szTWSendBuf = "";

	if (Screen_ja[(int)nScreenOrd]["Screen_lstDYArea"][(int)nDYAreaOrd]["Area_lstfile"].size() > 0) //节目内有文件
	{
		for (J = 0; J < Screen_ja[(int)nScreenOrd]["Screen_lstDYArea"][(int)nDYAreaOrd]["Area_lstfile"].size(); ++J)
		{
			if (Screen_ja[(int)nScreenOrd]["Screen_lstDYArea"][(int)nDYAreaOrd]["Area_lstfile"][(int)J]["File_Style"].asInt() != 1) //图文格式数据；
			{
				switch (Screen_ja[(int)nScreenOrd]["Screen_lstDYArea"][(int)nDYAreaOrd]["Area_lstfile"][(int)J]["File_Style"].asUInt())
				{
				//case FILE_TYPE_BMP:
				//case FILE_TYPE_JPG:
				//	szTWSendBuf = szTWSendBuf + MakebmpJPGBMP(nScreenWidth, nScreenHeight, nScreenColor, nScreenPixels, nScreenPixType, nScreenStyle
				//		, Screen_ja[(int)nScreenOrd]["Screen_lstDYArea"][(int)nDYAreaOrd]["Area_lstfile"][(int)J]
				//		, nRealX, nRealY, nRealW, nRealH, ntmpPageCount); //打开图片文件，主要是外部打开
				//	nTotlePagecount = nTotlePagecount + ntmpPageCount;
				//	break;
				//case FILE_TYPE_GIF:
				//	szTWSendBuf = szTWSendBuf + MakebmpGIF(nScreenWidth, nScreenHeight, nScreenColor, nScreenPixels, nScreenPixType, nScreenStyle
				//		, Screen_ja[(int)nScreenOrd]["Screen_lstDYArea"][(int)nDYAreaOrd]["Area_lstfile"][(int)J]
				//		, nRealX, nRealY, nRealW, nRealH, ntmpPageCount); //打开GIF文件
				//	nTotlePagecount = nTotlePagecount + ntmpPageCount;
				//	break;
				case FILE_TYPE_TXT:
					szTWSendBuf = szTWSendBuf + MakebmpTxt(nScreenWidth, nScreenHeight, nScreenColor, nScreenPixels
						, nScreenPixType, nScreenStyle, t_rvstyle, t_memo, t_RVReportHelper
						, Screen_ja[(int)nScreenOrd]["Screen_lstDYArea"][(int)nDYAreaOrd]["Area_lstfile"][(int)J]
						, nRealX, nRealY, nRealW, nRealH, ntmpPageCount); //打开txt文本文件 ,其中包含外部打开，是否使用单行或多行
					nTotlePagecount = nTotlePagecount + ntmpPageCount;
					break;
				//case FILE_TYPE_RTF:
				//	szTWSendBuf = szTWSendBuf + MakebmpRTF(nScreenWidth, nScreenHeight, nScreenColor, nScreenPixels
				//		, nScreenPixType, nScreenStyle, t_RVReportHelper
				//		, Screen_ja[(int)nScreenOrd]["Screen_lstDYArea"][(int)nDYAreaOrd]["Area_lstfile"][(int)J]
				//		, nRealX, nRealY, nRealW, nRealH, ntmpPageCount); //打开RTF文件，其中包含外部打开
				//	nTotlePagecount = nTotlePagecount + ntmpPageCount;
				//	break;
				//case FILE_TYPE_TXTF:
				//case FILE_TYPE_TXTS:
				//	szTWSendBuf = szTWSendBuf + MakebmpRVF(nScreenWidth, nScreenHeight, nScreenColor, nScreenPixels
				//		, nScreenPixType, nScreenStyle, t_rvstyle, t_RVReportHelper
				//		, Screen_ja[(int)nScreenOrd]["Screen_lstDYArea"][(int)nDYAreaOrd]["Area_lstfile"][(int)J]
				//		, nRealX, nRealY, nRealW, nRealH, ntmpPageCount); //打开多行文本，单行文本RVF文t_rvstyle
				//	nTotlePagecount = nTotlePagecount + ntmpPageCount;
				//	break;
				//case FILE_TYPE_TBF:
				//	szTWSendBuf = szTWSendBuf + MakebmpGRID(nScreenWidth, nScreenHeight, nScreenColor, nScreenPixels
				//		, nScreenPixType, nScreenStyle, t_RVReportHelper
				//		, Screen_ja[(int)nScreenOrd]["Screen_lstDYArea"][(int)nDYAreaOrd]["Area_lstfile"][(int)J]
				//		, nRealX, nRealY, nRealW, nRealH, ntmpPageCount); //打开表格文件
				//	nTotlePagecount = nTotlePagecount + ntmpPageCount;
				//	break;
				//case FILE_TYPE_TEXT:
				//	szTWSendBuf = szTWSendBuf + MakebmpText(nScreenWidth, nScreenHeight, nScreenColor, nScreenPixels
				//		, nScreenPixType, nScreenStyle, t_rvstyle, t_memo, t_RVReportHelper
				//		, Screen_ja[(int)nScreenOrd]["Screen_lstDYArea"][(int)nDYAreaOrd]["Area_lstfile"][(int)J]
				//		, nRealX, nRealY, nRealW, nRealH, ntmpPageCount); //打开txt文本文件 ,其中包含外部打开，是否使用单行或多行
				//	nTotlePagecount = nTotlePagecount + ntmpPageCount;
				//	break;
				}
			}
		}
	}
	else
	{
		char szAreaName[32];
		sprintf(szAreaName, "动态-%d", Screen_ja[(int)nScreenOrd]["Screen_lstDYArea"][(int)nDYAreaOrd]["DY_AreaID"].asUInt());
		szTWSendBuf = szTWSendBuf + MakeNULLArea(szAreaName
			, nRealW, nRealH, nRealX, nScreenColor, nScreenPixels, nScreenPixType, nScreenStyle
			, AREA_STUNT_DIRECTSHOW
			, AREA_STUNT_DIRECTSHOW
			, 1
			, 50
			, 1
			, 0
			, 0
			, 0
			, 0
			, 0
			, 0
			, 0);
		nTotlePagecount = nTotlePagecount + +1;
	}

	szSendBuf = szSendBuf + (char)(nTotlePagecount & 0xFF) + (char)((nTotlePagecount >> 8) & 0xFF) + szTWSendBuf;
	nDataLength = szSendBuf.size();

	Result = "";
	Result.push_back((char)(1));
	Result.push_back((char)(BYTE)0xA7);
	Result.push_back((char)(0));
	Result.push_back((char)(0));
	Result.push_back((char)(0));
	Result.push_back((char)(nDataLength & 0x00FF));
	Result.push_back((char)((nDataLength >> 8) & 0x00FF));
	Result.push_back((char)((nDataLength >> 16) & 0x00FF));
	Result.push_back((char)((nDataLength >> 24) & 0x00FF));
	Result += szSendBuf;

	nSendLength = Result.size();

	/*FreeAndNil(t_RVReportHelper);
	FreeAndNil(t_rvstyle);
	FreeAndNil(t_memo);*/
	return Result;
}

//string MakebmpJPGBMP(DWORD thrd_ScreenWidth, DWORD thrd_ScreenHeight
//	, DWORD thrd_ScreenColor, DWORD thrd_ScreenPixels, DWORD thrd_ScreenPixType
//	, DWORD thrd_ScreenStyle, Json::Value AreaFile_Obj, DWORD nX, DWORD nY, DWORD nWidth
//	, DWORD nHeight, DWORD &nFilePageCount)
//{
//	string sztmpSendbuf;
//	DWORD nRealityRunSpeed, nDefaultRunSpeed;
//	DWORD ntmpFilePageCount, nRealPageCount;
//
//	nRealPageCount = 1;
//	nRealityRunSpeed = GetControlStuntRealityRunSpeed(thrd_ScreenWidth, thrd_ScreenHeight
//		, thrd_ScreenColor, AreaFile_Obj["File_Stunt"].asUInt()
//		, AreaFile_Obj["File_Speed"].asUInt()
//		, nDefaultRunSpeed);
//
//	sztmpSendbuf = MakebmpImage(AreaFile_Obj["File_Name"].asCString(), nWidth, nHeight, nX, thrd_ScreenColor, thrd_ScreenPixels, thrd_ScreenPixType, thrd_ScreenStyle
//		, AreaFile_Obj["File_Stunt"].asUInt()
//		, AreaFile_Obj["File_OutsStunt"].asUInt()
//		, nRealityRunSpeed
//		, AreaFile_Obj["File_Showtime"].asUInt() * AREA_SHOWTIME_BASIC
//		, AreaFile_Obj["File_ShowCount"].asUInt()
//		, AreaFile_Obj["File_Reserved1"].asUInt()
//		, AreaFile_Obj["File_Reserved2"].asUInt()
//		, AreaFile_Obj["File_Reserved3"].asUInt()
//		, AreaFile_Obj["File_Reserved4"].asUInt()
//		, AreaFile_Obj["File_Reserved5"].asUInt()
//		, AreaFile_Obj["File_Reserved6"].asUInt()
//		, AreaFile_Obj["File_Reserved7"].asUInt()
//		, nRealPageCount);
//	nFilePageCount = nRealPageCount;
//
//	return sztmpSendbuf;
//}

// --- Do not port this function now because don't know how to do--- //
//string MakebmpRVF(DWORD thrd_ScreenWidth, DWORD thrd_ScreenHeight, DWORD thrd_ScreenColor, DWORD thrd_ScreenPixels
//	, DWORD thrd_ScreenPixType, DWORD thrd_ScreenStyle, TRVStyle t_rvstyle, TRVReportHelper t_RVReportHelper,
//	Json::Value AreaFile_Obj, DWORD nX, DWORD nY, DWORD nWidth, DWORD nHeight, DWORD &nFilePageCount) //手动添加的RVF文件
//{
//	fstream fd;
//	DWORD realw, realh, nrealw;
//	DWORD nPageSize, nAllWidth;
//	DWORD ntmpPagecount;
//	string str2;
//	int i;
//	DWORD nRealityRunSpeed, nDefaultRunSpeed;
//	DWORD ntmpFilePageCount, nRealPageCount;
//
//	str2 = "";
//	nRealPageCount= 0;
///////////////////////////协议的头
//	nAllWidth= calpagesize(nX, nWidth, thrd_ScreenColor, thrd_ScreenPixType);
//	nPageSize= GetPageSize(nAllWidth, nHeight, thrd_ScreenColor) + 9;
////算出每一页的数据的大小nPageSize
//	t_RVReportHelper.RichView.Clear;
//	t_RVReportHelper.RichView.DeleteUnusedStyles(true, true, true);
//	if WideFileExists(AreaFile_Obj.S["File_Name"]) then //打开外部文件并判断RTF文件
//	begin
//	try
//	fd = TtntFileStream.Create(AreaFile_Obj.S["File_Name"], fmShareDenyNone);
//	fd.Position = 0;
//	t_RVReportHelper.RichView.LoadRVFFromStream(fd);
//	if AreaFile_Obj["File_ShowStyle"] = file_showstyle_single then
//	begin
//	for i = 0 to t_rvstyle.ParaStyles.Count - 1 do
//	t_rvstyle.ParaStyles[i].Options = [rvpaoNoWrap, rvpaoDoNotWantReturns];
//	end;
//	finally
//	fd.Free;
//	end;
//	end else //如果这个文本不存在，那么就直接拍一个黑屏，并显示NIL
//	begin
//	t_RVReportHelper.RichView.Clear;
//	t_RVReportHelper.RichView.Style.TextStyles.Items[0].Color = clred;
//	t_RVReportHelper.RichView.Style.TextStyles.Items[0].FontName = "Tahoma";
//	t_RVReportHelper.RichView.Style.TextStyles.Items[0].Size = 9;
//	t_RVReportHelper.RichView.Style.TextStyles.Items[0].Unicode = true;
//	t_RVReportHelper.RichView.Style.TextStyles.Items[0].BackColor = clblack;
//	t_RVReportHelper.RichView.AddNL(RVU_GetRawUnicode("NIL"), 0, 0);
//	t_RVReportHelper.RichView.Format;
//end;

//if (AreaFile_Obj["File_ShowStyle"] = file_showstyle_single) then //单行的模式
//begin
//// realw :=
//calRTFsingalpage(t_RVReportHelper, nWidth, nHeight, realw, realh, nrealw);
//if ((realw mod nWidth) <> 0) or (realw = 0) then
//ntmpFilePageCount = realw div nWidth + 1 else
//ntmpFilePageCount = realw div nWidth;
//FillPageInfo(t_RVReportHelper, nWidth, realh); //转换为图片
//nRealityRunSpeed= GetControlStuntRealityRunSpeed(thrd_ScreenWidth, thrd_ScreenHeight
//, thrd_ScreenColor, AreaFile_Obj["File_Stunt"]
//, AreaFile_Obj["File_Speed"]
//, nDefaultRunSpeed);
//	if AreaFile_Obj["File_Stunt"] = 38 then //向右连移
//		str2 = str2 +
//		MakeRightSigalImage(t_RVReportHelper, ntmpFilePageCount, nWidth, nHeight, nX, thrd_ScreenColor, thrd_ScreenPixels, thrd_ScreenPixType, thrd_ScreenStyle
//			, AreaFile_Obj["File_Stunt"]
//			, AreaFile_Obj["File_OutsStunt"]
//			, nRealityRunSpeed
//			, AreaFile_Obj["File_Showtime"] * AREA_SHOWTIME_BASIC
//			, AreaFile_Obj["File_ShowCount"]
//			, AreaFile_Obj["File_Reserved1"]
//			, AreaFile_Obj["File_Reserved2"]
//			, AreaFile_Obj["File_Reserved3"]
//			, AreaFile_Obj["File_Reserved4"]
//			, AreaFile_Obj["File_Reserved5"]
//			, AreaFile_Obj["File_Reserved6"]
//			, AreaFile_Obj["File_Reserved7"]
//			, nrealw, false, nRealPageCount)
//	else
//		str2 = str2 +
//		MakeSigalImage(t_RVReportHelper, ntmpFilePageCount, nWidth, nHeight, nX, thrd_ScreenColor, thrd_ScreenPixels, thrd_ScreenPixType, thrd_ScreenStyle
//			, AreaFile_Obj["File_Stunt"]
//			, AreaFile_Obj["File_OutsStunt"]
//			, nRealityRunSpeed
//			, AreaFile_Obj["File_Showtime"] * AREA_SHOWTIME_BASIC
//			, AreaFile_Obj["File_ShowCount"]
//			, AreaFile_Obj["File_Reserved1"]
//			, AreaFile_Obj["File_Reserved2"]
//			, AreaFile_Obj["File_Reserved3"]
//			, AreaFile_Obj["File_Reserved4"]
//			, AreaFile_Obj["File_Reserved5"]
//			, AreaFile_Obj["File_Reserved6"]
//			, AreaFile_Obj["File_Reserved7"]
//			, nrealw, false, nRealPageCount);
//	end else
//		if AreaFile_Obj["File_Stunt"] = 6 then //向上连移
//			begin
//			FillPageInfoupscroll(t_RVReportHelper, nWidth, nHeight, ntmpPagecount);
//ntmpFilePageCount= ntmpPagecount;
//nRealityRunSpeed= GetControlStuntRealityRunSpeed(thrd_ScreenWidth, thrd_ScreenHeight
//	, thrd_ScreenColor, AreaFile_Obj["File_Stunt"]
//	, AreaFile_Obj["File_Speed"]
//	, nDefaultRunSpeed);
//
//			str2= str2 +
//				MakePageImageupscroll(t_RVReportHelper, ntmpFilePageCount, nWidth, nHeight, nX, thrd_ScreenColor, thrd_ScreenPixels, thrd_ScreenPixType, thrd_ScreenStyle
//					, AreaFile_Obj["File_Stunt"]
//					, AreaFile_Obj["File_OutsStunt"]
//					, nRealityRunSpeed
//					, AreaFile_Obj["File_Showtime"] * AREA_SHOWTIME_BASIC
//					, AreaFile_Obj["File_ShowCount"]
//					, AreaFile_Obj["File_Reserved1"]
//					, AreaFile_Obj["File_Reserved2"]
//					, AreaFile_Obj["File_Reserved3"]
//					, AreaFile_Obj["File_Reserved4"]
//					, AreaFile_Obj["File_Reserved5"]
//					, AreaFile_Obj["File_Reserved6"]
//					, AreaFile_Obj["File_Reserved7"]
//					, false, nRealPageCount);
//					end
//		else //其他的显示方式
//						begin
//			FillPageInfo(t_RVReportHelper, nWidth, nHeight);
//ntmpFilePageCount= t_RVReportHelper.PagesCount;
//nRealityRunSpeed= GetControlStuntRealityRunSpeed(thrd_ScreenWidth, thrd_ScreenHeight
//	, thrd_ScreenColor, AreaFile_Obj["File_Stunt"]
//	, AreaFile_Obj["File_Speed"]
//	, nDefaultRunSpeed);
//
//			str2= str2 +
//				MakeDoubleImage(t_RVReportHelper, ntmpFilePageCount, nWidth, nHeight, nX, thrd_ScreenColor, thrd_ScreenPixels, thrd_ScreenPixType, thrd_ScreenStyle
//					, AreaFile_Obj["File_Stunt"]
//					, AreaFile_Obj["File_OutsStunt"]
//					, nRealityRunSpeed
//					, AreaFile_Obj["File_Showtime"] * AREA_SHOWTIME_BASIC
//					, AreaFile_Obj["File_ShowCount"]
//					, AreaFile_Obj["File_Reserved1"]
//					, AreaFile_Obj["File_Reserved2"]
//					, AreaFile_Obj["File_Reserved3"]
//					, AreaFile_Obj["File_Reserved4"]
//					, AreaFile_Obj["File_Reserved5"]
//					, AreaFile_Obj["File_Reserved6"]
//					, AreaFile_Obj["File_Reserved7"]
//					, false, nRealPageCount);
//					end;
//				Result= str2;
//				nFilePageCount= nRealPageCount;
//					end;

//string MakebmpImage(string filename, DWORD w, DWORD h, DWORD x, DWORD nPx, DWORD nMkStyle,
//	DWORD nKardPixType, DWORD nScreenStyle, DWORD nStunt, DWORD nOutStunt, DWORD nRunSpeed
//	, DWORD nShowTime, DWORD nShowCount, DWORD nReserved1, DWORD nReserved2, DWORD nReserved3, DWORD nReserved4
//	, DWORD nReserved5, DWORD nReserved6, DWORD nReserved7, DWORD &nRealPageCount)
//{
//	fstream fd;
//	tmpBmp, mBmpPage, bmp: tbitmap;
//FileHeader: TBitmapFileHeader;
//InfoHeader: TBitmapInfoHeader;
//szBmpTextBuf: string;
//buf: array[0..1] of byte;
//// MyJpg: TJpegImage;
//realsp, nrepeatime: Cardinal;
//nAllWidth, nPageSize: Cardinal;
//nPageStyle: byte;
//nCurAddress: Cardinal;
//ncurPageAllLength: Cardinal;
//szcurPagebuf: string;
//graphics: IGPGraphics;
//Image: IGPImage;
//bInvalidData: Boolean;
//sztmpFileName: Widestring;
//fp: file;
//I, J, nLength, nEncryptionLength: Cardinal;
//szData, szEncryptionData, szEncryptionValue: string;
//bGBX: Boolean;
//begin
//nAllWidth  = calpagesize(x, w, nPx, nKardPixType);
//nPageStyle = 0;
//nrepeatime = 0;
//tmpBmp = TBitmap.Create;
////tmpBmp.PixelFormat := GetBmpPixelFormat(nPx);//pf4bit;
//tmpBmp.Width  = w;
//tmpBmp.height  = h;
//tmpBmp.Canvas.Lock;
//if WideFileExists(filename) then
//begin
//try
//bGBX  = false;
//sztmpFileName = Copy(filename, 1, Length(filename) - 3) + 'GBX';
//if WideFileExists(sztmpFileName) then
//begin
//AssignFile(fp, sztmpFileName);
//try
//Reset(fp, 1);
//nLength = FileSize(fp);
//SetLength(szData, nLength);
//BlockRead(fp, szData[1], nLength);
//finally
//CloseFile(fp);
//end;
//szEncryptionValue = GIF_SECRET;
//nEncryptionLength = Length(szEncryptionValue);
//szEncryptionData = '';
//for I  = 1 to nlength do
//begin
//J  = (I - 1) mod nEncryptionLength;
//szEncryptionData = szEncryptionData + Char(Byte(szData[I]) xor Byte(szEncryptionValue[J + 1]));
//end;
//sztmpFileName = WideExtractFilePath(sztmpFileName) + 'GIF' + Format('%.4d', [YearOf(now)])
//	+ Format('%.2d', [MonthOf(now)])
//	+ Format('%.2d', [DayOf(now)])
//	+ Format('%.2d', [HourOf(now)])
//	+ Format('%.2d', [MinuteOf(now)])
//	+ Format('%.2d', [SecondOf(now)])
//	+ Format('%.3d', [MilliSecondOf(now)]);
//			   AssignFile(fp, sztmpFileName);
//			   try
//				   Rewrite(fp, 1);
//			   BlockWrite(fp, szEncryptionData[1], Length(szEncryptionData));
//			   finally
//				   CloseFile(fp);
//			   end;
//			   //FileSetAttr(sztmpFileName, faHidden);
//		   bGBX = true;
//			   end
//else
//sztmpFileName  = filename;
//
//try
//fd  = TtntFileStream.Create(sztmpFileName, fmShareDenyNone);
//fd.Position  = 0;
//fd.Position  = 0;
//fd.Read(buf, 2);
//fd.Position  = 0;
//if (buf[0] = $42) and (buf[1] = $4D) then //bmp
//begin
//fd.Read(FileHeader, SizeOf(TBitmapFileHeader));
//fd.Read(InfoHeader, SizeOf(TBitmapInfoHeader));
//tmpBmp.Width  = InfoHeader.biWidth;
//tmpBmp.Height  = InfoHeader.biheight;
//tmpBmp.Canvas.Brush.Color  = clBlack;
//tmpBmp.Canvas.FillRect(Rect(0, 0, tmpBmp.Width, tmpBmp.Height));
//streamToBmp(fd, tmpBmp.Width, tmpBmp.height, InfoHeader.biBitCount, FileHeader.bfOffBits, tmpBmp);
//end else
//if (buf[0] = $FF) and (buf[1] = $D8) then //jpg
//begin
//tmpBmp.Width  = w;
//tmpBmp.height  = h;
//tmpBmp.Canvas.Brush.Color  = clBlack;
//tmpBmp.Canvas.FillRect(Rect(0, 0, tmpBmp.Width, tmpBmp.Height));
//Image = TGPImage.Create(sztmpFileName);
//bmp = TBitmap.Create;
//bmp.Canvas.Lock;
////bmp.PixelFormat := GetBmpPixelFormat(nPx);//pf4bit;
//bmp.Width  = Image.GetWidth;
//bmp.height  = Image.GetHeight;
//graphics = TGPGraphics.Create(bmp.Canvas.Handle);
//graphics.DrawImage(Image, 0, 0, bmp.Width, bmp.Height);
//graphics = nil;
//Image = nil;
//tmpBmp.Canvas.StretchDraw(rect(0, 0, w, h), bmp);
//bmp.Canvas.Unlock;
//FreeAndNil(bmp);
//end else //其他格式
//begin
//tmpBmp.Width  = w;
//tmpBmp.height  = h;
//tmpBmp.Canvas.Brush.Color  = clBlack;
//tmpBmp.Canvas.FillRect(Rect(0, 0, tmpBmp.Width, tmpBmp.Height));
//end;
//finally
//fd.Position  = 0;
//fd.Free;
//end;
//finally
//if bGBX = true then
//DeleteFile(sztmpFileName);
//end;
//end else
//begin
//tmpBmp.Width  = w;
//tmpBmp.height  = h;
//tmpBmp.Canvas.Brush.Color  = clBlack;
//tmpBmp.Canvas.Font.Color  = clred;
//tmpBmp.Canvas.Font.Size  = 9;
//tmpBmp.Canvas.FillRect(Rect(0, 0, tmpBmp.Width, tmpBmp.Height));
//tmpBmp.Canvas.TextOut(0, 0, 'NIL');
//end;
//
//mBmpPage = TBitmap.Create;
////mBmpPage.PixelFormat :=GetBmpPixelFormat(nPx);// pf4bit;
//mBmpPage.Width  = w;
//mBmpPage.Height  = h;
//mBmpPage.Canvas.Lock;
//mBmpPage.Canvas.Brush.Color  = clBlack;
//mBmpPage.Canvas.FillRect(Rect(0, 0, mBmpPage.Width, mBmpPage.Height));
//mBmpPage.Canvas.StretchDraw(Rect(0, 0, w, h), tmpBmp);
//
//SetLength(szcurPagebuf, 0);
//szcurPagebuf = Char(nPageStyle) //数据类型
//	+ Char(nStunt)
//	+ Char(nOutStunt)
//	+ Char(nRunSpeed)
//	+ Char(nShowtime and $FF)
//	+ Char((nShowtime shr 8) and $00FF)
//	+ Char(nShowCount)
//	+ Char(nReserved1) //保留字
//	+ Char(nReserved2) //保留字
//	+ Char(nReserved3) //保留字
//	+ Char(nReserved4) //保留字
//	+ Char(nReserved5) //保留字
//	+ Char(nReserved6); //保留字
//
//nCurAddress = Length(szcurPagebuf);
//nPageSize = GetPageSize(nAllWidth, h, nPx);
//	SetLength(szcurPagebuf, nCurAddress + nPageSize);
//	Inc(nCurAddress);
//	//需要对无效页数据进行处理；图片区域无需处理 ；页数为1
//nRealPageCount = 1;
//	TranCanvToInfo(mBmpPage, @szcurPagebuf[nCurAddress]
//	, x, w, h, nPx, nMkStyle,
//		nKardPixType, nScreenStyle, bInvalidData);
//ncurPageAllLength = length(szcurPagebuf);
//szBmpTextBuf = szBmpTextBuf + Char(ncurPageAllLength and $00FF)
//	+ Char((ncurPageAllLength shr 8) and $00FF)
//	+ Char((ncurPageAllLength shr 16) and $00FF)
//	+ Char((ncurPageAllLength shr 24) and $00FF) + szcurPagebuf;
//
//			mBmpPage.Canvas.Unlock;
//			tmpBmp.Canvas.Unlock;
//			FreeAndNil(tmpBmp);
//			FreeAndNil(mBmpPage);
//		Result = szBmpTextBuf;
//end;

DWORD GetPageSize(DWORD nAllWidth, DWORD nHeight, DWORD nPx)
{
	switch (nPx)
	{
	case SCREEN_COLOR_SINGLE:
	case SCREEN_COLOR_DOUBLE: 
	case SCREEN_COLOR_THREE:
		return ((nAllWidth * nHeight) / 8) * nPx;
		break;
	case SCREEN_COLOR_FULLCOLOR:
		return (nAllWidth * nHeight) * 2;
	default:
		return 0;
	}
}

void GetRealDYAreaLocation(Json::Value DYArea_Obj, DWORD &nRealX, DWORD &nRealY, DWORD &nRealW, DWORD &nRealH)
{
	DWORD nFrame_Width;

	if (DYArea_Obj["DY_AreaFMode"].asUInt() == 0xFF)
	{
		nRealX = DYArea_Obj["DY_AreaX"].asUInt();
		nRealY = DYArea_Obj["DY_AreaY"].asUInt();
		nRealW = DYArea_Obj["DY_AreaWidth"].asUInt();
		nRealH = DYArea_Obj["DY_AreaHeight"].asUInt();
	}
	else
	{
		nFrame_Width = DYArea_Obj["DY_AreaFWidth"].asUInt();
		nRealX = DYArea_Obj["DY_AreaX"].asUInt() + nFrame_Width;
		nRealY = DYArea_Obj["DY_AreaY"].asUInt() + nFrame_Width;
		nRealW = DYArea_Obj["DY_AreaWidth"].asUInt() - nFrame_Width * 2;
		nRealH = DYArea_Obj["DY_AreaHeight"].asUInt() - nFrame_Width * 2;
		if (nRealW < 0)
			nRealW = 1;
		if (nRealH < 0)
			nRealH = 1;
	}
}

string MakeFrameValue(Json::Value Screen_Obj, DWORD nDYAreaOrd)
{
	DWORD nRealX, nRealY, nRealW, nRealH;
	BYTE nFrame_DispStype;
	string szSendBuf, szFrameData;
	HBITMAP bmp, bmp2;
	DWORD nIndex, n, nDataLength;
	bool bInvalidData;

	if (Screen_Obj["Screen_lstDYArea"][(int)nDYAreaOrd]["DY_AreaFMode"].asUInt() == 0xFF)
	{
		szSendBuf.clear();
		szSendBuf += (char)(0);
	}
	else
	{
		GetRealDYAreaLocation(Screen_Obj["Screen_lstDYArea"][(int)nDYAreaOrd], nRealX, nRealY, nRealW, nRealH);
		nFrame_DispStype = (BYTE)Screen_Obj["Screen_lstDYArea"][(int)nDYAreaOrd]["DY_AreaFStunt"].asUInt();

		if (nFrame_DispStype == 6) //因为在PC软件中屏蔽掉了“红绿交替转动”特技。因此当边框特技为6是设定为固定显示。
			nFrame_DispStype = 7;

		szSendBuf.clear();
		szSendBuf += (char)(1);
		szSendBuf += (char)(nFrame_DispStype);
		szSendBuf += (char)(Screen_Obj["Screen_lstDYArea"][(int)nDYAreaOrd]["DY_AreaFRunSpeed"].asUInt());
		szSendBuf += (char)(Screen_Obj["Screen_lstDYArea"][(int)nDYAreaOrd]["DY_AreaFMoveStep"].asUInt());
		szSendBuf += (char)(Screen_Obj["Screen_lstDYArea"][(int)nDYAreaOrd]["DY_AreaFWidth"].asUInt());
		szSendBuf += (char)(0);
		szSendBuf += (char)(0);

		HWND hDesktopWnd = GetDesktopWindow();
		HDC hDesktopDC = GetWindowDC(hDesktopWnd);
		HDC hMemoryDC = CreateCompatibleDC(hDesktopDC);
		HDC hMemoryDC2 = CreateCompatibleDC(hMemoryDC);
		BITMAPINFO bi;
		BYTE *pBmpBits = NULL;
		BYTE *pBmpBits2 = NULL;
		HBRUSH hBlackBrush = CreateSolidBrush(RGB(0, 0, 0));
		HPEN hRedPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		RECT rect;
		RECT rect2;

		memset(&bi, 0, sizeof(BITMAPINFO));
		bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bi.bmiHeader.biWidth = PRO_FRAME_LENGTH;
		bi.bmiHeader.biHeight = Screen_Obj["Screen_lstDYArea"][(int)nDYAreaOrd]["DY_AreaFWidth"].asUInt();
		bi.bmiHeader.biBitCount = 24;
		bi.bmiHeader.biSizeImage = (bi.bmiHeader.biWidth * 3 + 1) / 2 * 2;
		bi.bmiHeader.biPlanes = 1;

		rect = { 0, 0, bi.bmiHeader.biWidth, bi.bmiHeader.biHeight };
		rect2 = { 0, 0, bi.bmiHeader.biWidth, bi.bmiHeader.biHeight };

		bmp = CreateDIBSection(hMemoryDC, &bi, DIB_RGB_COLORS, (void**)&pBmpBits, NULL, 0);
		bmp2 = NULL;
		SelectObject(hMemoryDC, bmp);
		FillRect(hMemoryDC, &rect, hBlackBrush);

		if (Screen_Obj["Screen_lstDYArea"][(int)nDYAreaOrd]["DY_AreaFMode"].asUInt() == 0) //单色
		{
			nIndex = Screen_Obj["Screen_lstDYArea"][(int)nDYAreaOrd]["DY_AreaFLine"].asUInt();

			if (g_aryProFrameSingleColorBmp.size() >= nIndex)
			{
				SIZE bmp2Size;
				bmp2 = LoadBitmap(GetModuleHandle(_T("LedDynamicArea.dll")), MAKEINTRESOURCE(g_aryProFrameSingleColorBmp[nIndex]));
				SelectObject(hMemoryDC2, bmp2);
				GetBitmapDimensionEx(bmp2, &bmp2Size);
				rect2.right = bmp2Size.cx;
				rect2.bottom = bmp2Size.cy;
				//if (g_aryProFrameSingleColorBmp[nIndex] != nil)
				//{
				//	bmp2.Width = g_aryProFrameSingleColorBmp[nIndex].Width;
				//	bmp2.Height = g_aryProFrameSingleColorBmp[nIndex].Height;
				//	bmp2.Canvas.CopyRect(Rect(0, 0, bmp2.Width, bmp2.Height), g_aryProFrameSingleColorBmp[nIndex].Canvas, Rect(0, 0, bmp2.Width, bmp2.Height));
				//}
				//else
				//{
				//	bmp2.Width = PRO_FRAME_LENGTH;
				//	bmp2.Height = Screen_Obj["Screen_lstDYArea"][(int)nDYAreaOrd]["DY_AreaFWidth"].asUInt();
				//	bmp2.Canvas.Brush.Color = clblack;
				//	bmp2.Canvas.FillRect(rect(0, 0, bmp2.Width, bmp2.Height));
				//	bmp2.Canvas.Pen.Width = 1;
				//	bmp2.Canvas.Pen.Color = clRed;
				//	for n = 0 to bmp2.Height - 1 do
				//	{
				//		bmp2.Canvas.MoveTo(0, n);
				//		bmp2.Canvas.LineTo(bmp2.Width div 2, n);
				//	}
				//}
			}
			else
			{
				//bmp2 = CreateDIBSection(hMemoryDC2, &bi, DIB_RGB_COLORS, (void**)&pBmpBits2, NULL, 0);
				HPEN hOldPen = (HPEN)SelectObject(hMemoryDC2, hRedPen);

				FillRect(hMemoryDC2, &rect2, hBlackBrush);

				for (n = 0; n < (DWORD)rect2.bottom; n++)
				{
					MoveToEx(hMemoryDC2, 0, n, NULL);
					LineTo(hMemoryDC2, rect2.right / 2, n);
				}

				SelectObject(hMemoryDC2, hOldPen);
				/*bmp2.Width = PRO_FRAME_LENGTH;
				bmp2.Height = Screen_Obj["Screen_lstDYArea"][(int)nDYAreaOrd]["DY_AreaFWidth"].asUInt();
				bmp2.Canvas.Brush.Color = clblack;
				bmp2.Canvas.FillRect(rect(0, 0, bmp2.Width, bmp2.Height));
				bmp2.Canvas.Pen.Width = 1;
				bmp2.Canvas.Pen.Color = clRed;
				for n = 0 to bmp2.Height - 1 do
				{
					bmp2.Canvas.MoveTo(0, n);
					bmp2.Canvas.LineTo(bmp2.Width div 2, n);
				}*/
			}
			//finally
			//}
			GetProgramFrameColorBmp(pBmpBits2, rect2.right, rect2.bottom
				, Screen_Obj["Screen_lstDYArea"][(int)nDYAreaOrd]["DY_AreaFColor"].asUInt());
		}
		else if (Screen_Obj["Screen_lstDYArea"][(int)nDYAreaOrd]["DY_AreaFMode"].asUInt() == 1) //花色
		{
			//try
			nIndex = Screen_Obj["Screen_lstDYArea"][(int)nDYAreaOrd]["DY_AreaFLine"].asUInt();
			if (g_aryProFrameMuliColorBmp.size() >= nIndex)
			{
				bmp2 = LoadBitmap(GetModuleHandle(_T("LedDynamicArea.dll")), MAKEINTRESOURCE(g_aryProFrameMuliColorBmp[nIndex]));

				/*if (g_aryProFrameMuliColorBmp[nIndex] != nil)
				{
					bmp2.Width = g_aryProFrameMuliColorBmp[nIndex].Width;
					bmp2.Height = g_aryProFrameMuliColorBmp[nIndex].Height;
					bmp2.Canvas.CopyRect(Rect(0, 0, bmp2.Width, bmp2.Height)
						, g_aryProFrameMuliColorBmp[nIndex].Canvas, Rect(0, 0, bmp2.Width, bmp2.Height));
				}
				else
				{
					bmp2.Width = PRO_FRAME_LENGTH;
					bmp2.Height = Screen_Obj["Screen_lstDYArea"][(int)nDYAreaOrd]["DY_AreaFWidth"].asUInt();
					bmp2.Canvas.Brush.Color = clblack;
					bmp2.Canvas.FillRect(rect(0, 0, bmp2.Width, bmp2.Height));
					bmp2.Canvas.Pen.Width = 1;
					bmp2.Canvas.Pen.Color = clRed;
					for n = 0 to bmp2.Height - 1 do
					{
						bmp2.Canvas.MoveTo(0, n);
						bmp2.Canvas.LineTo(bmp2.Width div 2, n);
					}
				}*/
			}
			else
			{
				HPEN hOldPen = (HPEN)SelectObject(hMemoryDC2, hRedPen);

				FillRect(hMemoryDC2, &rect, hBlackBrush);

				for (n = 0; n < (DWORD)bi.bmiHeader.biHeight; n++)
				{
					MoveToEx(hMemoryDC2, 0, n, NULL);
					LineTo(hMemoryDC2, bi.bmiHeader.biWidth / 2, n);
				}

				SelectObject(hMemoryDC2, hOldPen);

				/*bmp2.Width = PRO_FRAME_LENGTH;
				bmp2.Height = Screen_Obj["Screen_lstDYArea"][(int)nDYAreaOrd]["DY_AreaFWidth"].asUInt();
				bmp2.Canvas.Brush.Color = clblack;
				bmp2.Canvas.FillRect(rect(0, 0, bmp2.Width, bmp2.Height));
				bmp2.Canvas.Pen.Width = 1;
				bmp2.Canvas.Pen.Color = clRed;
				for n = 0 to bmp2.Height - 1 do
				{
					bmp2.Canvas.MoveTo(0, n);
					bmp2.Canvas.LineTo(bmp2.Width div 2, n);
				}*/
			}
			//finally
			//}
		}

		StretchBlt(hMemoryDC, 0, 0, bi.bmiHeader.biWidth, bi.bmiHeader.biHeight,
			hMemoryDC2, rect2.left, rect2.top, rect2.right, rect2.bottom, SRCCOPY);
		nDataLength = GetPageSize(PRO_FRAME_LENGTH, rect.bottom, Screen_Obj["Screen_color"].asUInt());
		TranCanvToInfo(pBmpBits, szFrameData
			, 0, bi.bmiHeader.biWidth, bi.bmiHeader.biHeight, Screen_Obj["Screen_color"].asUInt()
			, Screen_Obj["Screen_pixels"].asUInt(),
			Screen_Obj["Screen_nPixType"].asUInt()
			, Screen_Obj["Screen_nPixType"].asUInt(), bInvalidData);
		szSendBuf = szSendBuf + szFrameData;

		DeleteObject(hRedPen);
		DeleteObject(hBlackBrush);
		DeleteObject(bmp2);
		DeleteObject(bmp);
		DeleteDC(hMemoryDC2);
		DeleteDC(hMemoryDC);
		ReleaseDC(hDesktopWnd, hDesktopDC);
	}

	return szSendBuf;
}

void GetProgramFrameColorBmp(BYTE *pBits, DWORD nWidth, DWORD nHeight, DWORD nColorOrd)
{
	DWORD w, h;
	COLORREF nColor;
	BYTE *Col, ColR, ColG;
	size_t bytesPerLine = (nWidth * 3 + 1) / 2 * 2;

	nColor = nColorOrd;

	for (w = 0; w < nWidth; ++w)
		for (h = 0; h < nHeight; ++h)
		{
			Col = &pBits[h * bytesPerLine + w * 3];
			ColR = Col[0];
			if (ColR > 127)
			{
				Col[0] = GetRValue(nColor);
				Col[1] = GetGValue(nColor);
				Col[2] = GetBValue(nColor);
			}
		}
}

void TranCanvToInfo(LPBYTE Bmp, string &pInfoBuf,
	DWORD nLeft, DWORD nWidth, DWORD nHeight, DWORD nPx, DWORD nMkStyle, DWORD nPixType,
	DWORD nControllerType, bool &bInvalidData)
{
	BYTE *bitsRed, *bitsGreen, *bitsBlue;
	DWORD nDimensions;
	DWORD nIndex;
	DWORD iLeftBit, iRightBit;
	DWORD h, w;
	COLORREF Col;
	BYTE ColR, ColG, ColB;
	bool bRed, bGreen, bBlue;
	DWORD nAllWidth;
	DWORD byRed, byGreen, byBlue;
	DWORD I;
	bool btmpInvalidData;
	WORD nFullColorPixelData;
	BYTE *p;
	int bytesPerLine = (nWidth * 3 + 1) / 2 * 2;

	bRed = false;
	bGreen = false;
	bBlue = false;
	btmpInvalidData = true;
	nIndex = 0;

	switch (nPx)
	{
	case SCREEN_COLOR_SINGLE:
	case SCREEN_COLOR_DOUBLE:
	case SCREEN_COLOR_THREE:
		iRightBit = 0;

		if (((nLeft + nWidth) % nPixType) != 0)
			iRightBit = nPixType - ((nLeft + nWidth) % nPixType); //右边起始字节的多余位数

		iLeftBit = nLeft % nPixType; //左边起始字节的多余位数
		nAllWidth = iLeftBit + nWidth + iRightBit;
		nDimensions = nAllWidth * nHeight;
		bitsRed = new BYTE[nDimensions];
		bitsGreen = new BYTE[nDimensions];
		bitsBlue = new BYTE[nDimensions];

		for (h = 0; h < nHeight; ++h)
		{
			for (w = 0; w < nAllWidth; ++w)
			{
				if ((w < iLeftBit) || (w >= (iLeftBit + nWidth)))
					Col = RGB(0, 0, 0);
				else
					Col = RGB(Bmp[bytesPerLine * h + (w - iLeftBit) * 3 + 2],
						Bmp[bytesPerLine * h + (w - iLeftBit) * 3 + 1],
						Bmp[bytesPerLine * h + (w - iLeftBit) * 3 + 0]);

				ColR = GetRValue(Col);
				ColG = GetGValue(Col);
				ColB = GetBValue(Col);

				if (nPx == SCREEN_COLOR_DOUBLE)
				{
					if (nMkStyle == DOUBLE_COLOR_PIXTYPE_1) //双基色，R+G
						bRed = ColR > 127;
					else //双基色，G+R
						bGreen = ColR > 127;

					if (bRed || bGreen)
						btmpInvalidData = false;
				}
				else if (nPx == SCREEN_COLOR_SINGLE)
				{
					bRed = ColR > 127;
					if (bRed)
						btmpInvalidData = false;
				}
				else if (nPx == SCREEN_COLOR_THREE)
				{
					bRed = ColR > 127;

					if (bRed)
						btmpInvalidData = false;
				}

				if (nPx == SCREEN_COLOR_DOUBLE)
				{
					if (nMkStyle == DOUBLE_COLOR_PIXTYPE_1) //双基色，R+G
						bGreen = ColG > 127;
					else //双基色，G+R
						bRed = ColG > 127;

					if (bRed || bGreen)
						btmpInvalidData = false;
				}
				else if (nPx == SCREEN_COLOR_SINGLE)
				{
					bGreen = ColG > 127;
					//单基色去绿色数据没有用处。因此不作判断
				}
				else if (nPx == SCREEN_COLOR_THREE)
				{
					bGreen = ColG > 127;
					if (bGreen)
						btmpInvalidData = false;
				}

				if (nPx == SCREEN_COLOR_THREE)
				{
					bBlue = ColB > 127;

					if (bBlue)
						btmpInvalidData = false;
				}

				bitsRed[nIndex] = bRed ? 0 : 1;
				bitsGreen[nIndex] = bGreen ? 0 : 1;
				bitsBlue[nIndex] = bBlue ? 0 : 1;
				++nIndex;
			}
		}

		if (nPx == SCREEN_COLOR_THREE)
		{
			for (I = 0; I < (nDimensions / 8); ++I)
			{
				byRed = (bitsRed[I * 8] << 7) + (bitsRed[I * 8 + 1] << 6) + (bitsRed[I * 8 + 2] << 5) + (bitsRed[I * 8 + 3] << 4)
					+ (bitsRed[I * 8 + 4] << 3) + (bitsRed[I * 8 + 5] << 2) + (bitsRed[I * 8 + 6] << 1) + (bitsRed[I * 8 + 7]);
				byGreen = (bitsGreen[I * 8] << 7) + (bitsGreen[I * 8 + 1] << 6) + (bitsGreen[I * 8 + 2] << 5) + (bitsGreen[I * 8 + 3] << 4)
					+ (bitsGreen[I * 8 + 4] << 3) + (bitsGreen[I * 8 + 5] << 2) + (bitsGreen[I * 8 + 6] << 1) + bitsGreen[I * 8 + 7];
				byBlue = (bitsBlue[I * 8] << 7) + (bitsBlue[I * 8 + 1] << 6) + (bitsBlue[I * 8 + 2] << 5) + (bitsBlue[I * 8 + 3] << 4)
					+ (bitsBlue[I * 8 + 4] << 3) + (bitsBlue[I * 8 + 5] << 2) + (bitsBlue[I * 8 + 6] << 1) + bitsBlue[I * 8 + 7];

				pInfoBuf += char(byRed);
				pInfoBuf += char(byGreen);
				pInfoBuf += char(byBlue);
			}
		}
		else if (nPx == SCREEN_COLOR_DOUBLE)
		{
			for (I = 0; I < (nDimensions / 8); ++I)
			{
				byRed = (bitsRed[I * 8] << 7) + (bitsRed[I * 8 + 1] << 6) + (bitsRed[I * 8 + 2] << 5) + (bitsRed[I * 8 + 3] << 4)
					+ (bitsRed[I * 8 + 4] << 3) + (bitsRed[I * 8 + 5] << 2) + (bitsRed[I * 8 + 6] << 1) + bitsRed[I * 8 + 7];
				byGreen = (bitsGreen[I * 8] << 7) + (bitsGreen[I * 8 + 1] << 6) + (bitsGreen[I * 8 + 2] << 5) + (bitsGreen[I * 8 + 3] << 4)
					+ (bitsGreen[I * 8 + 4] << 3) + (bitsGreen[I * 8 + 5] << 2) + (bitsGreen[I * 8 + 6] << 1) + bitsGreen[I * 8 + 7];

				pInfoBuf += char(byRed);
				pInfoBuf += char(byGreen);
			}
		}
		else if (nPx == SCREEN_COLOR_SINGLE)
		{
			for (I = 0; (I < nDimensions / 8); ++I)
			{
				byRed = (bitsRed[I * 8] << 7) + (bitsRed[I * 8 + 1] << 6) + (bitsRed[I * 8 + 2] << 5) + (bitsRed[I * 8 + 3] << 4)
					+ (bitsRed[I * 8 + 4] << 3) + (bitsRed[I * 8 + 5] << 2) + (bitsRed[I * 8 + 6] << 1) + bitsRed[I * 8 + 7];
				pInfoBuf += char(byRed);

			}
		}
		delete[] bitsRed;
		delete[] bitsGreen;
		delete[] bitsBlue;
		bInvalidData = btmpInvalidData;
		break;

	case SCREEN_COLOR_FULLCOLOR:
		nAllWidth = nWidth;
		nDimensions = nAllWidth * nHeight;

		for (h = 0; h < nHeight; ++h)
		{
			p = &Bmp[bytesPerLine * h];

			for (w = 0; w < nAllWidth; ++w)
			{
				BYTE highByte, lowByte;

				byRed = p[w * 3 + 2];
				byGreen = p[w * 3 + 1];
				byBlue = p[w * 3 + 0];

				highByte = ((byRed / 8) << 3) & 0xF8 + ((byGreen / 4) >> 3) & 0x7;
				lowByte = ((byGreen / 4) << 5) & 0xE0 + ((byBlue / 8) >> 3) & 0x1F;

				if ((highByte != 0) || (lowByte != 0))
					btmpInvalidData = false;
				//双字节参数值表示为R(5bits)+G(6bits)+B(5bits) 生成数据时，先低字节，然后高字节。

				pInfoBuf += (char)lowByte; //低字节
				pInfoBuf += (char)highByte; //高字节
			}
		}
		bInvalidData = btmpInvalidData;
		DoEvents();
		break;
	case SCREEN_COLOR_TWO_COLOR_GRAY:
		nAllWidth = nWidth;
		nDimensions = nAllWidth * nHeight;

		for (h = 0; h < nHeight; ++h)
		{
			p = &Bmp[bytesPerLine * h];

			for (w = 0; w < nAllWidth; ++w)
			{
				BYTE highByte, lowByte;

				byRed = p[w * 3 + 2];
				byGreen = p[w * 3 + 1];
				byBlue = p[w * 3 + 0];

				highByte = ((byRed / 8) << 3) & 0xF8 + ((byGreen / 4) >> 3) & 0x7;
				lowByte = ((byGreen / 4) << 5) & 0xE0 + ((byBlue / 8) >> 3) & 0x1F;

				if ((highByte != 0) || (lowByte != 0))
					btmpInvalidData = false;
				//双字节参数值表示为R(5bits)+G(6bits)+B(5bits) 生成数据时，先低字节，然后高字节。

				pInfoBuf += (char)highByte; //低字节
				pInfoBuf += (char)lowByte; //高字节
			}
		}

		bInvalidData = btmpInvalidData;
		DoEvents();
		break;
	}
}

string MakebmpTxt(DWORD thrd_ScreenWidth, DWORD thrd_ScreenHeight,
	DWORD thrd_ScreenColor, DWORD thrd_ScreenPixels, DWORD thrd_ScreenPixType, DWORD thrd_ScreenStyle,
	TRVStyle t_rvstyle, TTntMemo t_memo, TRVReportHelper t_RVReportHelper,
	Json::Value AreaFile_Obj, DWORD nX, DWORD nY, DWORD nWidth, DWORD nHeight, DWORD &nFilePageCount) //打开图文区域的外部文件txt文件
{
	fstream fd;
	DWORD i, realw;
	string s, ansitxt;
	string unicodetxt;
	HBITMAP bmp;
	DWORD m, nAlignment, ncolor;
	DWORD nKardPixType, nshowstyle, nMkStyle, nshowspeed, nshowtime;
	bool nbold, nItalic, nUnderline, nStrikeout;
	string sztmpSendbuf;
	DWORD ntmpPagecount;
	DWORD nRealityRunSpeed, nDefaultRunSpeed;
	string szFontName;
	int nFontSize, nFontColor;
	DWORD ntmpFilePageCount, nRealPageCount;
	//tntmmostream: TTNTMemoryStream;
	DWORD nDocHeight;

	nAlignment = 0;
	szFontName = AreaFile_Obj["File_FontName"].asString();
	if (AreaFile_Obj["File_Bold"].asUInt() == 1)
		nbold = true;
	else
		nbold = false;
	nItalic = false;
	nUnderline = false;
	nStrikeout = false;
	nFontSize = AreaFile_Obj["File_FontSize"].asInt();
	nFontColor = AreaFile_Obj["File_FontColor"].asInt();
	/////////////////////////协议的头
	//算出每一页的数据的大小nPageSize
	sztmpSendbuf = "";
	ansitxt = "";
	unicodetxt = "";
	nRealPageCount = 0;

	HWND hDesktopWnd = GetDesktopWindow();
	HDC hDesktopDC = GetWindowDC(hDesktopWnd);
	HDC hMemoryDC = CreateCompatibleDC(hDesktopDC);
	BYTE *pBmpBits = NULL;

	BITMAPINFO bi;
	memset(&bi, 0, sizeof(bi));
	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biWidth = nX + nWidth;
	bi.bmiHeader.biHeight = nY + nHeight;
	bi.bmiHeader.biBitCount = 24;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biSizeImage = (bi.bmiHeader.biWidth * 3 + 1) / 2 * 2 * bi.bmiHeader.biHeight;

	HBITMAP hBitmap = CreateDIBSection(hMemoryDC, &bi, DIB_RGB_COLORS, (void**)&pBmpBits, NULL, 0);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemoryDC, hBitmap);

	//if (WideFileExists(AreaFile_Obj["File_Name"].asCString())) //打开外部文件并判断TXT文件
	{
		//tntmmostream  = TTNTMemoryStream.Create;
		//try
		ifstream file(AreaFile_Obj["File_Name"].asCString());
		stringstream ss;

		ss << file.rdbuf();
		s = ss.str();

		if ((AreaFile_Obj["File_ShowStyle"].asUInt() == FILE_SHOWSTYLE_SINGLE)) //向左连移并且为单行的模式
		{
			HFONT hFont = CreateFontA(nFontSize, 0, 0, 0, nbold ? FW_BOLD : FW_DONTCARE, nItalic,
				nUnderline, nStrikeout, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
				CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, szFontName.c_str());
			HFONT hOldFont = (HFONT)SelectObject(hMemoryDC, hFont);
			SIZE size;

			GetTextExtentPoint32A(hMemoryDC, s.c_str(), s.size(), &size);
			realw = size.cx;

			if (((realw % nWidth) != 0) || (realw == 0))
				ntmpFilePageCount = realw / nWidth + 1;
			else
				ntmpFilePageCount = realw / nWidth;

			//FillPageInfo(t_RVReportHelper, nWidth * ntmpFilePageCount, nHeight); //转换为图片

			nRealityRunSpeed = GetControlStuntRealityRunSpeed(thrd_ScreenWidth, thrd_ScreenHeight
				, thrd_ScreenColor, AreaFile_Obj["File_Stunt"].asUInt()
				, AreaFile_Obj["File_Speed"].asUInt()
				, nDefaultRunSpeed);

			//if (AreaFile_Obj["File_Stunt"].asUInt() == 38) //向右连移
			//	sztmpSendbuf = sztmpSendbuf +
			//		MakeRightSigalImage(t_RVReportHelper, ntmpFilePageCount, nWidth, nHeight, nX, thrd_ScreenColor, thrd_ScreenPixels, thrd_ScreenPixType, thrd_ScreenStyle
			//			, AreaFile_Obj["File_Stunt"].asUInt()
			//			, AreaFile_Obj["File_OutsStunt"].asUInt()
			//			, nRealityRunSpeed
			//			, AreaFile_Obj["File_Showtime"].asUInt() * AREA_SHOWTIME_BASIC
			//			, AreaFile_Obj["File_ShowCount"].asUInt()
			//			, AreaFile_Obj["File_Reserved1"].asUInt()
			//			, AreaFile_Obj["File_Reserved2"].asUInt()
			//			, AreaFile_Obj["File_Reserved3"].asUInt()
			//			, AreaFile_Obj["File_Reserved4"].asUInt()
			//			, AreaFile_Obj["File_Reserved5"].asUInt()
			//			, AreaFile_Obj["File_Reserved6"].asUInt()
			//			, AreaFile_Obj["File_Reserved7"].asUInt()
			//			, realw, false, nRealPageCount);
			//else
			//	sztmpSendbuf = sztmpSendbuf +
			//		MakeSigalImage(t_RVReportHelper, ntmpFilePageCount, nWidth, nHeight, nX, thrd_ScreenColor, thrd_ScreenPixels, thrd_ScreenPixType, thrd_ScreenStyle
			//			, AreaFile_Obj["File_Stunt"].asUInt()
			//			, AreaFile_Obj["File_OutsStunt"].asUInt()
			//			, nRealityRunSpeed
			//			, AreaFile_Obj["File_Showtime"].asUInt() * AREA_SHOWTIME_BASIC
			//			, AreaFile_Obj["File_ShowCount"].asUInt()
			//			, AreaFile_Obj["File_Reserved1"].asUInt()
			//			, AreaFile_Obj["File_Reserved2"].asUInt()
			//			, AreaFile_Obj["File_Reserved3"].asUInt()
			//			, AreaFile_Obj["File_Reserved4"].asUInt()
			//			, AreaFile_Obj["File_Reserved5"].asUInt()
			//			, AreaFile_Obj["File_Reserved6"].asUInt()
			//			, AreaFile_Obj["File_Reserved7"].asUInt()
			//			, realw, false, nRealPageCount);
		}
		//else if (nshowstyle == 6) //向上连移
		//{
		//	FillPageInfoupscroll(t_RVReportHelper, nWidth, nHeight, ntmpPagecount);
		//	ntmpFilePageCount = ntmpPagecount;
		//	nRealityRunSpeed = GetControlStuntRealityRunSpeed(thrd_ScreenWidth, thrd_ScreenHeight
		//		, thrd_ScreenColor, AreaFile_Obj["File_Stunt"].asUInt()
		//		, AreaFile_Obj["File_Speed"].asUInt()
		//		, nDefaultRunSpeed);

		//	sztmpSendbuf = sztmpSendbuf +
		//		MakePageImageupscroll(t_RVReportHelper, ntmpFilePageCount, nWidth, nHeight, nX, thrd_ScreenColor, thrd_ScreenPixels, thrd_ScreenPixType, thrd_ScreenStyle
		//			, AreaFile_Obj["File_Stunt"].asUInt()
		//			, AreaFile_Obj["File_OutsStunt"].asUInt()
		//			, nRealityRunSpeed
		//			, AreaFile_Obj["File_Showtime"].asUInt() * AREA_SHOWTIME_BASIC
		//			, AreaFile_Obj["File_ShowCount"].asUInt()
		//			, AreaFile_Obj["File_Reserved1"].asUInt()
		//			, AreaFile_Obj["File_Reserved2"].asUInt()
		//			, AreaFile_Obj["File_Reserved3"].asUInt()
		//			, AreaFile_Obj["File_Reserved4"].asUInt()
		//			, AreaFile_Obj["File_Reserved5"].asUInt()
		//			, AreaFile_Obj["File_Reserved6"].asUInt()
		//			, AreaFile_Obj["File_Reserved7"].asUInt()
		//			, false, nRealPageCount);
		//}
		//else //其他的显示方式
		//{
		//	//FillPageInfo(t_RVReportHelper, nWidth, nHeight);
		//	/*ntmpFilePageCount = t_RVReportHelper.PagesCount;
		//	nDocHeight = TReportRVData(t_RVReportHelper.RichView.RVData).DocumentHeight;
		//	if (nDocHeight mod nHeight < > 0)
		//		TReportRVData(t_RVReportHelper.RichView.RVData).DocumentHeight = (nDocHeight div nHeight + 1) * nHeight;*/
		//	nRealityRunSpeed = GetControlStuntRealityRunSpeed(thrd_ScreenWidth, thrd_ScreenHeight
		//		, thrd_ScreenColor, AreaFile_Obj["File_Stunt"].asUInt()
		//		, AreaFile_Obj["File_Speed"].asUInt()
		//		, nDefaultRunSpeed);

		//	sztmpSendbuf = sztmpSendbuf +
		//		MakeDoubleImage(t_RVReportHelper, ntmpFilePageCount, nWidth, nHeight, nX, thrd_ScreenColor, thrd_ScreenPixels, thrd_ScreenPixType, thrd_ScreenStyle
		//			, AreaFile_Obj["File_Stunt"].asUInt()
		//			, AreaFile_Obj["File_OutsStunt"].asUInt()
		//			, nRealityRunSpeed
		//			, AreaFile_Obj["File_Showtime"].asUInt() * AREA_SHOWTIME_BASIC
		//			, AreaFile_Obj["File_ShowCount"].asUInt()
		//			, AreaFile_Obj["File_Reserved1"].asUInt()
		//			, AreaFile_Obj["File_Reserved2"].asUInt()
		//			, AreaFile_Obj["File_Reserved3"].asUInt()
		//			, AreaFile_Obj["File_Reserved4"].asUInt()
		//			, AreaFile_Obj["File_Reserved5"].asUInt()
		//			, AreaFile_Obj["File_Reserved6"].asUInt()
		//			, AreaFile_Obj["File_Reserved7"].asUInt()
		//			, false, nRealPageCount);
		//}
	}

	nFilePageCount = nRealPageCount;
	return sztmpSendbuf;
}

//string MakeRightSigalImage(TRVReportHelper t_RVReportHelpe,
//	DWORD count, DWORD w, DWORD h, DWORD x, DWORD nPx, DWORD nMkStyle, DWORD nKardPixType, DWORD nScreenStyle,
//	DWORD nStunt, DWORD nOutStunt, DWORD nRunSpeed, DWORD nShowTime, DWORD nShowCount,
//	DWORD nReserved1, DWORD nReserved2, DWORD nReserved3, DWORD nReserved4, DWORD nReserved5, DWORD nReserved6,
//	DWORD nReserved7, DWORD nrealwidth, bool bOnePage, DWORD &nRealPageCount)
//{
//	HBITMAP bmp2, bmp1, mBmpTitlePage;
//	string str2;
//	DWORD i, realw, nrepeatTime;
//	BOOL noblank;
//	DWORD nCurAddress, nAllWidth, nPageSize, nPageStyle;
//	DWORD ncurPageAllLength;
//	string szcurPagebuf;
//	BOOL bInvalidData;
//
//	str2 = "";
//	nAllWidth = calpagesize(x, w, nPx, nKardPixType);
//	nPageSize = GetPageSize(nAllWidth, h, nPx);
//	nPageStyle = 0;
//	noblank = false;
//
//	HWND hDesktopWnd = GetDesktopWindow();
//	HDC hDesktopDC = GetWindowDC(hDesktopWnd);
//	HDC hMemoryDC = CreateCompatibleDC(hDesktopDC);
//	HBRUSH hBlackBrush = CreateSolidBrush(RGB(0, 0, 0));
//
//	BITMAPINFO bmp1Info;
//	memset(&bmp1Info, 0, sizeof(bmp1Info));
//	bmp1Info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
//	bmp1Info.bmiHeader.biWidth = w * count;
//	bmp1Info.bmiHeader.biHeight = h;
//	bmp1Info.bmiHeader.biPlanes = 1;
//	bmp1Info.bmiHeader.biSizeImage = (bmp1Info.bmiHeader.biWidth * 3 + 1) / 2 * 2 * bmp1Info.bmiHeader.biHeight;
//	bmp1Info.bmiHeader.biBitCount = 24;
//
//	BYTE *pBmp1Bits = NULL;
//
//	bmp1 = CreateDIBSection(hMemoryDC, &bmp1Info, DIB_RGB_COLORS, (void**)&pBmp1Bits, NULL, 0);
//
//	RECT rect = {0, 0, bmp1Info.bmiHeader.biWidth, bmp1Info.bmiHeader.biHeight };
//	FillRect(hMemoryDC, &rect, hBlackBrush);
//
//
//	DrawsingalPage(t_RVReportHelper, bmp1.Canvas, h); //绘制图片20100518版本
//												  //bmp1.SaveToFile('c:\Total.bmp');
//	bmp2 = tbitmap.Create;
//	bmp2.PixelFormat  = GetBmpPixelFormat(nPx); // pf4bit;
//	bmp2.Canvas.Lock;
//	bmp2.Width  = w * count;
//	bmp2.height  = h;
//	bmp2.Canvas.Brush.Color  = clblack;
//	bmp2.Canvas.FillRect(rect(0, 0, w * count, h));
//	//  bmp2.Canvas.Draw(0, 0, bmp1);
//	bmp2.Canvas.Draw(w * count - nrealwidth, 0, bmp1);
//
//mBmpTitlePage = tbitmap.Create;
//mBmpTitlePage.Canvas.Lock;
//mBmpTitlePage.PixelFormat  = GetBmpPixelFormat(nPx); // pf4bit;
//mBmpTitlePage.Width  = w;
//mBmpTitlePage.height  = h;
//
//if count * w = nrealwidth then
//realw  = w else
//	realw  = w - (count * w - nrealwidth);
//
////  realw := (realw div 8) + (realw mod 8);
//if bOnePage = False then
//nRealPageCount  = count
//else
//nRealPageCount  = 1;
//for i  = count downto 1 do
//begin
//mBmpTitlePage.Canvas.Brush.Color  = clBlack;
//mBmpTitlePage.Canvas.FillRect(Rect(0, 0, mBmpTitlePage.Width, mBmpTitlePage.Height));
//
//mBmpTitlePage.Canvas.CopyRect(rect(0, 0, mBmpTitlePage.Width, mBmpTitlePage.Height),
//	bmp2.Canvas, Rect(w * (i - 1), 0, w * i, h));
//
//{    mBmpTitlePage.Canvas.CopyRect(rect(0, 0, mBmpTitlePage.Width, mBmpTitlePage.Height),
//	bmp2.Canvas,
//	Rect(areashowlist[nAreaOrd].uwidth * (i - 1), 0,
//		areashowlist[nAreaOrd].uwidth * i, areashowlist[nAreaOrd].uheight));
//}
////  mBmpTitlePage.SaveToFile('c:\' + IntToStr(i) + '.bmp');
//
//SetLength(szcurPagebuf, 0);
//if (i = 1) and (nStunt = AREA_STUNT_RCONTIMOVE) then
//begin
//szcurPagebuf  = Char(nPageStyle) //数据类型
//	+ Char(nStunt)
//	+ Char(nOutStunt)
//	+ Char(nRunSpeed)
//	+ Char(nShowtime and $FF)
//	+ Char((nShowtime shr 8) and $00FF)
//	+ Char(nShowCount)
//	+ Char(realw and $FF) //实际宽度低字节
//	+ Char((realw shr 8) and $00FF) //实际宽度高字节
//	+ Char(nReserved3) //保留字
//	+ Char(nReserved4) //保留字
//	+ Char(nReserved5) //保留字
//	+ Char(nReserved6); //保留字
//end else
//begin
//szcurPagebuf  = Char(nPageStyle) //数据类型
//	+ Char(nStunt)
//	+ Char(nOutStunt)
//	+ Char(nRunSpeed)
//	+ Char(nShowtime and $FF)
//	+ Char((nShowtime shr 8) and $00FF)
//	+ Char(nShowCount)
//	+ Char(nReserved1) //保留字
//	+ Char(nReserved2) //保留字
//	+ Char(nReserved3) //保留字
//	+ Char(nReserved4) //保留字
//	+ Char(nReserved5) //保留字
//	+ Char(nReserved6); //保留字
//end;
//nCurAddress = Length(szcurPagebuf);
//SetLength(szcurPagebuf, nCurAddress + nPageSize);
//Inc(nCurAddress);
////需要对无效页数据进行处理
//TranCanvToInfo(mBmpTitlePage, @szcurPagebuf[nCurAddress]
//, X, w, h, nPx, nMkStyle, nKardPixType, nScreenStyle, bInvalidData);
//if bInvalidData = True then
//begin
//Dec(nRealPageCount);
////13个字节为图文区域信息数据类型到六个保留字的13个字节。
//SetLength(szcurPagebuf, Length(szcurPagebuf) - nPageSize - 13);
//Continue;
//end;
//ncurPageAllLength = length(szcurPagebuf);
//str2 = str2 + Char(ncurPageAllLength and $00FF)
//	+ Char((ncurPageAllLength shr 8) and $00FF)
//	+ Char((ncurPageAllLength shr 16) and $00FF)
//	+ Char((ncurPageAllLength shr 24) and $00FF) + szcurPagebuf;
//	  if bOnePage = True then
//		  Break;
//	  end;
//	  bmp2.Canvas.Unlock;
//	  bmp1.Canvas.Unlock;
//	  mBmpTitlePage.Canvas.Unlock;
//	  FreeAndNil(bmp1);
//	  FreeAndNil(bmp2);
//	  FreeAndNil(mBmpTitlePage);
//  result = str2;
//	  end;
//
//	  procedure DrawsingalPage(t_RVReportHelper: TRVReportHelper; Canvas: TCanvas; h: Cardinal);
//	  var
//		  vh, eleft, i, j   : integer;
//	  begin
//		  eleft : = 0;
//	  for i : = 1 to t_RVReportHelper.PagesCount do
//		  begin
//		  if t_RVReportHelper.PagesCount > 1 then
//			  begin
//			  if i = t_RVReportHelper.PagesCount then
//				  vh : = TReportRVData(t_RVReportHelper.RichView.RVData).DocumentHeight -
//				  TPrintableRVData(t_RVReportHelper.RichView.RVData).Pages.Items[i - 1].Starty else
//				  vh : = TPrintableRVData(t_RVReportHelper.RichView.RVData).Pages.Items[i].StartY -
//				  TPrintableRVData(t_RVReportHelper.RichView.RVData).Pages.Items[i - 1].StartY;
//	  end else
//		  if t_RVReportHelper.PagesCount = 1 then vh : = TReportRVData(t_RVReportHelper.RichView.RVData).DocumentHeight;
//  eleft: = 0;
//
//	  for j : = 0 to TPrintableRVData(t_RVReportHelper.RichView.RVData).Pages.Items[i - 1].StartDrawItemNo - 1 do
//		  eleft : = eleft + t_RVReportHelper.RichView.RVData.DrawItems[j].Width;
//
//	  // if vh > h then
//	  //    t_RVReportHelper.DrawPageAt(eleft, h - vh, i, Canvas, false, h)
//	  //  else
//	  t_RVReportHelper.DrawPageAt(eleft, (h - vh) div 2, i, Canvas, false, h);
//	  end;
//	end;