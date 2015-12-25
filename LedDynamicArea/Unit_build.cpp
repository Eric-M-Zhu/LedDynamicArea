#include "stdafx.h"
#include "Unit_build.h"

#include "MyDataType.h"

#define NumCharSets 19

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

//void TranCanvToInfo(Bmp: TBitmap; pInfoBuf: pChar;
//nLeft, nWidth, nHeight, nPx, nMkStyle, nPixType: Cardinal;
//nControllerType: Cardinal; var bInvalidData : Boolean);
//var
//bitsRed, bitsGreen, bitsBlue: TBits;
//nDimensions: Cardinal;
//nIndex: Cardinal;
//iLeftBit, iRightBit: Cardinal;
//h, w              : Cardinal;
//Col: TColor;
//ColR, ColG, ColB  : Byte;
//bRed, bGreen, bBlue: Boolean;
//nAllWidth: Cardinal;
//byRed, byGreen, byBlue: Cardinal;
//I: Cardinal;
//btmpInvalidData: Boolean;
//nFullColorPixelData: Word;
//p: PByteArray;
//begin
//bRed  = False;
//bGreen = False;
//bBlue = False;
//btmpInvalidData = True;
//nIndex = 0;
//case nPx of
//SCREEN_COLOR_SINGLE, SCREEN_COLOR_DOUBLE, SCREEN_COLOR_THREE:
//	begin
//		iRightBit  = 0;
//	if ((nLeft + nWidth) % nPixType != 0) then
//		iRightBit  = nPixType - ((nLeft + nWidth) % nPixType); //右边起始字节的多余位数
//iLeftBit = nLeft % nPixType; //左边起始字节的多余位数
//nAllWidth = iLeftBit + nWidth + iRightBit;
//nDimensions = nAllWidth * nHeight;
//bitsRed = TBits.Create;
//	bitsRed.Size  = nDimensions;
//bitsGreen = TBits.Create;
//	bitsGreen.Size  = nDimensions;
//bitsBlue = TBits.Create;
//	bitsBlue.Size  = nDimensions;
//	for h  = 1 to nHeight do
//		begin
//		for w  = 1 to nAllWidth do
//			begin
//			if ((w - 1 < iLeftBit) or (w - 1 >= (iLeftBit + nWidth))) then
//				Col  = RGB(0, 0, 0)
//			else
//				Col  = Bmp.Canvas.Pixels[w - 1 - iLeftBit, h - 1];
//ColR = GetRValue(Col);
//ColG = GetGValue(Col);
//ColB = GetBValue(Col);
//	if (nPx = SCREEN_COLOR_DOUBLE) then
//		begin
//		if (nMkStyle = DOUBLE_COLOR_PIXTYPE_1) then //双基色，R+G
//			bRed  = ColR > 127
//		else //双基色，G+R
//			bGreen  = ColR > 127;
//	if (bRed = True) or (bGreen = True) then
//		btmpInvalidData  = False;
//	end
//	else if (nPx = SCREEN_COLOR_SINGLE) then
//		begin
//		bRed  = ColR > 127;
//	if (bRed = True) then
//		btmpInvalidData  = False;
//	end
//	else if (nPx = SCREEN_COLOR_THREE) then
//		begin
//		bRed  = ColR > 127;
//	if (bRed = True) then
//		btmpInvalidData  = False;
//	}
//	if (nPx = SCREEN_COLOR_DOUBLE) then
//		begin
//		if (nMkStyle = DOUBLE_COLOR_PIXTYPE_1) then //双基色，R+G
//			bGreen  = ColG > 127
//		else //双基色，G+R
//			bRed  = ColG > 127;
//	if (bRed = True) or (bGreen = True) then
//		btmpInvalidData  = False;
//	end
//	else if (nPx = SCREEN_COLOR_SINGLE) then
//		begin
//		bGreen  = ColG > 127;
//	//单基色去绿色数据没有用处。因此不作判断
//	end
//	else if (nPx = SCREEN_COLOR_THREE) then
//		begin
//		bGreen  = ColG > 127;
//	if (bGreen = True) then
//		btmpInvalidData  = False;
//	}
//	if (nPx = SCREEN_COLOR_THREE) then
//		begin
//		bBlue  = ColB > 127;
//	if (bBlue = True) then
//		btmpInvalidData  = False;
//	}
//	bitsRed[nIndex]  = not bRed;
//	bitsGreen[nIndex]  = not bGreen;
//	bitsBlue[nIndex]  = not bBlue;
//	Inc(nIndex);
//	}
//	}
//	if (nPx = SCREEN_COLOR_THREE) then
//		begin
//		for I  = 1 to nDimensions div 8 do
//			begin
//			byRed  = Ord(bitsRed[(I - 1) * 8]) shl 7 + Ord(bitsRed[(I - 1) * 8 + 1]) shl 6 + Ord(bitsRed[(I - 1) * 8 + 2]) shl 5 + Ord(bitsRed[(I - 1) * 8 + 3]) shl 4
//			+ Ord(bitsRed[(I - 1) * 8 + 4]) shl 3 + Ord(bitsRed[(I - 1) * 8 + 5]) shl 2 + Ord(bitsRed[(I - 1) * 8 + 6]) shl 1 + Ord(bitsRed[(I - 1) * 8 + 7]);
//byGreen = Ord(bitsGreen[(I - 1) * 8]) shl 7 + Ord(bitsGreen[(I - 1) * 8 + 1]) shl 6 + Ord(bitsGreen[(I - 1) * 8 + 2]) shl 5 + Ord(bitsGreen[(I - 1) * 8 + 3]) shl 4
//	+ Ord(bitsGreen[(I - 1) * 8 + 4]) shl 3 + Ord(bitsGreen[(I - 1) * 8 + 5]) shl 2 + Ord(bitsGreen[(I - 1) * 8 + 6]) shl 1 + Ord(bitsGreen[(I - 1) * 8 + 7]);
//	 byBlue = Ord(bitsBlue[(I - 1) * 8]) shl 7 + Ord(bitsBlue[(I - 1) * 8 + 1]) shl 6 + Ord(bitsBlue[(I - 1) * 8 + 2]) shl 5 + Ord(bitsBlue[(I - 1) * 8 + 3]) shl 4
//		 + Ord(bitsBlue[(I - 1) * 8 + 4]) shl 3 + Ord(bitsBlue[(I - 1) * 8 + 5]) shl 2 + Ord(bitsBlue[(I - 1) * 8 + 6]) shl 1 + Ord(bitsBlue[(I - 1) * 8 + 7]);
//			 pInfoBuf^  = char(byRed);
//			 Inc(pInfoBuf);
//			 pInfoBuf^  = char(byGreen);
//			 Inc(pInfoBuf);
//			 pInfoBuf^  = char(byBlue);
//			 Inc(pInfoBuf);
//			 }
//			 end
//	else if (nPx = SCREEN_COLOR_DOUBLE) then
//				 begin
//		for I  = 1 to nDimensions div 8 do
//			begin
//			byRed  = Ord(bitsRed[(I - 1) * 8]) shl 7 + Ord(bitsRed[(I - 1) * 8 + 1]) shl 6 + Ord(bitsRed[(I - 1) * 8 + 2]) shl 5 + Ord(bitsRed[(I - 1) * 8 + 3]) shl 4
//			+ Ord(bitsRed[(I - 1) * 8 + 4]) shl 3 + Ord(bitsRed[(I - 1) * 8 + 5]) shl 2 + Ord(bitsRed[(I - 1) * 8 + 6]) shl 1 + Ord(bitsRed[(I - 1) * 8 + 7]);
//byGreen = Ord(bitsGreen[(I - 1) * 8]) shl 7 + Ord(bitsGreen[(I - 1) * 8 + 1]) shl 6 + Ord(bitsGreen[(I - 1) * 8 + 2]) shl 5 + Ord(bitsGreen[(I - 1) * 8 + 3]) shl 4
//	+ Ord(bitsGreen[(I - 1) * 8 + 4]) shl 3 + Ord(bitsGreen[(I - 1) * 8 + 5]) shl 2 + Ord(bitsGreen[(I - 1) * 8 + 6]) shl 1 + Ord(bitsGreen[(I - 1) * 8 + 7]);
//		 pInfoBuf^  = char(byRed);
//		 Inc(pInfoBuf);
//		 pInfoBuf^  = char(byGreen);
//		 Inc(pInfoBuf);
//		 }
//		 end
//	else if (nPx = SCREEN_COLOR_SINGLE) then
//			 begin
//				 for I  = 1 to nDimensions div 8 do
//					 begin
//					 byRed  = Ord(bitsRed[(I - 1) * 8]) shl 7 + Ord(bitsRed[(I - 1) * 8 + 1]) shl 6 + Ord(bitsRed[(I - 1) * 8 + 2]) shl 5 + Ord(bitsRed[(I - 1) * 8 + 3]) shl 4
//					 + Ord(bitsRed[(I - 1) * 8 + 4]) shl 3 + Ord(bitsRed[(I - 1) * 8 + 5]) shl 2 + Ord(bitsRed[(I - 1) * 8 + 6]) shl 1 + Ord(bitsRed[(I - 1) * 8 + 7]);
//			 pInfoBuf^  = char(byRed);
//			 Inc(pInfoBuf);
//			 }
//			 }
//			 FreeAndNil(bitsRed);
//			 FreeAndNil(bitsGreen);
//			 FreeAndNil(bitsBlue);
//		 bInvalidData = btmpInvalidData;
//			 }
//		 SCREEN_COLOR_FULLCOLOR:
//			 begin
//				 nAllWidth  = nWidth;
//		 nDimensions = nAllWidth * nHeight;
//			 bmp.PixelFormat  = pf16bit;
//			 for h  = 1 to nHeight do
//				 begin
//				 p  = bmp.ScanLine[h - 1];
//			 for W  = 1 to nAllWidth do
//				 begin
//				 if (p[(W - 1) * 2 + 1] != 0) or (p[(W - 1) * 2] != 0) then
//					 btmpInvalidData  = False;
//			 //双字节参数值表示为R(5bits)+G(6bits)+B(5bits) 生成数据时，先低字节，然后高字节。
//
//			 pInfoBuf^  = char(p[(W - 1) * 2]); //低字节
//			 Inc(pInfoBuf);
//			 pInfoBuf^  = char(p[(W - 1) * 2 + 1] and 0xFF); //高字节
//			 Inc(pInfoBuf);
//
//			 }
//			 }
//		 bInvalidData = btmpInvalidData;
//			 DoEvents;
//			 }
//		 SCREEN_COLOR_TWO_COLOR_GRAY:
//			 begin
//				 nAllWidth  = nWidth;
//		 nDimensions = nAllWidth * nHeight;
//			 bmp.PixelFormat  = pf16bit;
//			 for h  = 1 to nHeight do
//				 begin
//				 p  = bmp.ScanLine[h - 1];
//			 for W  = 1 to nAllWidth do
//				 begin
//				 if (p[(W - 1) * 2 + 1] != 0) or (p[(W - 1) * 2] != 0) then
//					 btmpInvalidData  = False;
//			 //双字节参数值表示为R(5bits)+G(6bits)+B(5bits) 生成数据时，先低字节，然后高字节。
//		 nFullColorPixelData = p[(W - 1) * 2] + (p[(W - 1) * 2 + 1] shl 8) and $FF00;
//		 ColR = nFullColorPixelData shr 11;
//		 ColG = (nFullColorPixelData shr 5) and $3F;
//		 colB = nFullColorPixelData and $1F;
//		 nFullColorPixelData = (colB shl 11) and $F800 + (ColG shl 5) and $07E0 + ColR;
//
//			 pInfoBuf^  = char(nFullColorPixelData and $FF); //低字节
//			 Inc(pInfoBuf);
//			 pInfoBuf^  = char((nFullColorPixelData shr 8) and $FF); //高字节
//			 Inc(pInfoBuf);
//			 }
//			 }
//		 bInvalidData = btmpInvalidData;
//			 DoEvents;
//			 }
//			 }
//			 }

DWORD calpagesize(DWORD left, DWORD width, DWORD nPx, DWORD nKardPixType)
{
	DWORD nStartPoint, nEndPoint;
	DWORD nEndSpace, nStartSpace, nAllWidth;
	DWORD Result;

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
	RECT rect = { 0, 0, w, h };
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
				case FILE_TYPE_BMP:
				case FILE_TYPE_JPG:
					szTWSendBuf = szTWSendBuf + MakebmpJPGBMP(nScreenWidth, nScreenHeight, nScreenColor, nScreenPixels, nScreenPixType, nScreenStyle
						, Screen_ja[(int)nScreenOrd]["Screen_lstDYArea"][(int)nDYAreaOrd]["Area_lstfile"][(int)J]
						, nRealX, nRealY, nRealW, nRealH, ntmpPageCount); //打开图片文件，主要是外部打开
					nTotlePagecount = nTotlePagecount + ntmpPageCount;
					break;
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
				case FILE_TYPE_TEXT:
					szTWSendBuf = szTWSendBuf + MakebmpText(nScreenWidth, nScreenHeight, nScreenColor, nScreenPixels
						, nScreenPixType, nScreenStyle, t_rvstyle, t_memo, t_RVReportHelper
						, Screen_ja[(int)nScreenOrd]["Screen_lstDYArea"][(int)nDYAreaOrd]["Area_lstfile"][(int)J]
						, nRealX, nRealY, nRealW, nRealH, ntmpPageCount); //打开txt文本文件 ,其中包含外部打开，是否使用单行或多行
					nTotlePagecount = nTotlePagecount + ntmpPageCount;
					break;
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
	Result.push_back((char)0xA7);
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

string MakebmpJPGBMP(DWORD thrd_ScreenWidth, DWORD thrd_ScreenHeight
	, DWORD thrd_ScreenColor, DWORD thrd_ScreenPixels, DWORD thrd_ScreenPixType
	, DWORD thrd_ScreenStyle, Json::Value AreaFile_Obj, DWORD nX, DWORD nY, DWORD nWidth
	, DWORD nHeight, DWORD &nFilePageCount)
{
	string sztmpSendbuf;
	DWORD nRealityRunSpeed, nDefaultRunSpeed;
	DWORD ntmpFilePageCount, nRealPageCount;

	nRealPageCount = 1;
	nRealityRunSpeed = GetControlStuntRealityRunSpeed(thrd_ScreenWidth, thrd_ScreenHeight
		, thrd_ScreenColor, AreaFile_Obj["File_Stunt"].asUInt()
		, AreaFile_Obj["File_Speed"].asUInt()
		, nDefaultRunSpeed);

	sztmpSendbuf = MakebmpImage(AreaFile_Obj["File_Name"].asCString(), nWidth, nHeight, nX, thrd_ScreenColor, thrd_ScreenPixels, thrd_ScreenPixType, thrd_ScreenStyle
		, AreaFile_Obj["File_Stunt"].asUInt()
		, AreaFile_Obj["File_OutsStunt"].asUInt()
		, nRealityRunSpeed
		, AreaFile_Obj["File_Showtime"].asUInt() * AREA_SHOWTIME_BASIC
		, AreaFile_Obj["File_ShowCount"].asUInt()
		, AreaFile_Obj["File_Reserved1"].asUInt()
		, AreaFile_Obj["File_Reserved2"].asUInt()
		, AreaFile_Obj["File_Reserved3"].asUInt()
		, AreaFile_Obj["File_Reserved4"].asUInt()
		, AreaFile_Obj["File_Reserved5"].asUInt()
		, AreaFile_Obj["File_Reserved6"].asUInt()
		, AreaFile_Obj["File_Reserved7"].asUInt()
		, nRealPageCount);
	nFilePageCount = nRealPageCount;

	return sztmpSendbuf;
}

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
//	t_RVReportHelper.RichView.Style.TextStyles.Items[0].Unicode = True;
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
//			, nrealw, False, nRealPageCount)
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
//			, nrealw, False, nRealPageCount);
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
//					, False, nRealPageCount);
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
//					, False, nRealPageCount);
//					end;
//				Result= str2;
//				nFilePageCount= nRealPageCount;
//					end;