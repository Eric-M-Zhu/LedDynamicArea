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

SYSTEMTIME StringToDatetime(string &DateStr) //��yyyy-mm-dd hh:nn:ss��ʽ���ַ���תΪʱ���ʽ
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
//		iRightBit  = nPixType - ((nLeft + nWidth) % nPixType); //�ұ���ʼ�ֽڵĶ���λ��
//iLeftBit = nLeft % nPixType; //�����ʼ�ֽڵĶ���λ��
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
//		if (nMkStyle = DOUBLE_COLOR_PIXTYPE_1) then //˫��ɫ��R+G
//			bRed  = ColR > 127
//		else //˫��ɫ��G+R
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
//		if (nMkStyle = DOUBLE_COLOR_PIXTYPE_1) then //˫��ɫ��R+G
//			bGreen  = ColG > 127
//		else //˫��ɫ��G+R
//			bRed  = ColG > 127;
//	if (bRed = True) or (bGreen = True) then
//		btmpInvalidData  = False;
//	end
//	else if (nPx = SCREEN_COLOR_SINGLE) then
//		begin
//		bGreen  = ColG > 127;
//	//����ɫȥ��ɫ����û���ô�����˲����ж�
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
//			 //˫�ֽڲ���ֵ��ʾΪR(5bits)+G(6bits)+B(5bits) ��������ʱ���ȵ��ֽڣ�Ȼ����ֽڡ�
//
//			 pInfoBuf^  = char(p[(W - 1) * 2]); //���ֽ�
//			 Inc(pInfoBuf);
//			 pInfoBuf^  = char(p[(W - 1) * 2 + 1] and 0xFF); //���ֽ�
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
//			 //˫�ֽڲ���ֵ��ʾΪR(5bits)+G(6bits)+B(5bits) ��������ʱ���ȵ��ֽڣ�Ȼ����ֽڡ�
//		 nFullColorPixelData = p[(W - 1) * 2] + (p[(W - 1) * 2 + 1] shl 8) and $FF00;
//		 ColR = nFullColorPixelData shr 11;
//		 ColG = (nFullColorPixelData shr 5) and $3F;
//		 colB = nFullColorPixelData and $1F;
//		 nFullColorPixelData = (colB shl 11) and $F800 + (ColG shl 5) and $07E0 + ColR;
//
//			 pInfoBuf^  = char(nFullColorPixelData and $FF); //���ֽ�
//			 Inc(pInfoBuf);
//			 pInfoBuf^  = char((nFullColorPixelData shr 8) and $FF); //���ֽ�
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
	DWORD Result = width;

	switch (nPx)
	{
	case SCREEN_COLOR_SINGLE:
	case SCREEN_COLOR_DOUBLE:
	case SCREEN_COLOR_THREE:
		/////////////////////////Э���ͷ
		nStartPoint = nKardPixType; //�����Чλ����
		if ((left % nKardPixType) != 0)
			nStartPoint = nKardPixType - left % nKardPixType;

		nEndPoint = nKardPixType; //�ұߵ���Чλ��
		if ((left + width) % nKardPixType != 0)
			nEndPoint = (left + width) % nKardPixType;

		nStartSpace = left % nKardPixType; //��ߵ���Чλ����

		nEndSpace = 0; //�ұߵ���Чλ��
		if ((left + width) % nKardPixType != 0)
			nEndSpace = nKardPixType - (left + width) % nKardPixType;

		nAllWidth = width + nStartSpace + nEndSpace; //�õ�ת�����ʵ�ʿ��
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
	szcurPagebuf.push_back(char(0)); //��������
	szcurPagebuf.push_back(char(nStunt));
	szcurPagebuf.push_back(char(nOutStunt));
	szcurPagebuf.push_back(char(nRunSpeed));
	szcurPagebuf.push_back(char(nShowTime & 0xFF));
	szcurPagebuf.push_back(char((nShowTime >> 8) & 0xFF));
	szcurPagebuf.push_back(char(nShowCount));
	szcurPagebuf.push_back(char(nReserved1)); //������
	szcurPagebuf.push_back(char(nReserved2)); //������
	szcurPagebuf.push_back(char(nReserved3)); //������
	szcurPagebuf.push_back(char(nReserved4)); //������
	szcurPagebuf.push_back(char(nReserved5)); //������
	szcurPagebuf.push_back(char(nReserved6));; //������

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
	TTntMemo t_memo = 0; ///��Ҫ����txt�ļ�

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
	// ����ؼ���ʼ��
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

	if (Screen_ja[(int)nScreenOrd]["Screen_lstDYArea"][(int)nDYAreaOrd]["Area_lstfile"].size() > 0) //��Ŀ�����ļ�
	{
		for (J = 0; J < Screen_ja[(int)nScreenOrd]["Screen_lstDYArea"][(int)nDYAreaOrd]["Area_lstfile"].size(); ++J)
		{
			if (Screen_ja[(int)nScreenOrd]["Screen_lstDYArea"][(int)nDYAreaOrd]["Area_lstfile"][(int)J]["File_Style"].asInt() != 1) //ͼ�ĸ�ʽ���ݣ�
			{
				switch (Screen_ja[(int)nScreenOrd]["Screen_lstDYArea"][(int)nDYAreaOrd]["Area_lstfile"][(int)J]["File_Style"].asUInt())
				{
				//case FILE_TYPE_BMP:
				//case FILE_TYPE_JPG:
				//	szTWSendBuf = szTWSendBuf + MakebmpJPGBMP(nScreenWidth, nScreenHeight, nScreenColor, nScreenPixels, nScreenPixType, nScreenStyle
				//		, Screen_ja[(int)nScreenOrd]["Screen_lstDYArea"][(int)nDYAreaOrd]["Area_lstfile"][(int)J]
				//		, nRealX, nRealY, nRealW, nRealH, ntmpPageCount); //��ͼƬ�ļ�����Ҫ���ⲿ��
				//	nTotlePagecount = nTotlePagecount + ntmpPageCount;
				//	break;
				//case FILE_TYPE_GIF:
				//	szTWSendBuf = szTWSendBuf + MakebmpGIF(nScreenWidth, nScreenHeight, nScreenColor, nScreenPixels, nScreenPixType, nScreenStyle
				//		, Screen_ja[(int)nScreenOrd]["Screen_lstDYArea"][(int)nDYAreaOrd]["Area_lstfile"][(int)J]
				//		, nRealX, nRealY, nRealW, nRealH, ntmpPageCount); //��GIF�ļ�
				//	nTotlePagecount = nTotlePagecount + ntmpPageCount;
				//	break;
				case FILE_TYPE_TXT:
					szTWSendBuf = szTWSendBuf + MakebmpTxt(nScreenWidth, nScreenHeight, nScreenColor, nScreenPixels
						, nScreenPixType, nScreenStyle, t_rvstyle, t_memo, t_RVReportHelper
						, Screen_ja[(int)nScreenOrd]["Screen_lstDYArea"][(int)nDYAreaOrd]["Area_lstfile"][(int)J]
						, nRealX, nRealY, nRealW, nRealH, ntmpPageCount); //��txt�ı��ļ� ,���а����ⲿ�򿪣��Ƿ�ʹ�õ��л����
					nTotlePagecount = nTotlePagecount + ntmpPageCount;
					break;
				//case FILE_TYPE_RTF:
				//	szTWSendBuf = szTWSendBuf + MakebmpRTF(nScreenWidth, nScreenHeight, nScreenColor, nScreenPixels
				//		, nScreenPixType, nScreenStyle, t_RVReportHelper
				//		, Screen_ja[(int)nScreenOrd]["Screen_lstDYArea"][(int)nDYAreaOrd]["Area_lstfile"][(int)J]
				//		, nRealX, nRealY, nRealW, nRealH, ntmpPageCount); //��RTF�ļ������а����ⲿ��
				//	nTotlePagecount = nTotlePagecount + ntmpPageCount;
				//	break;
				//case FILE_TYPE_TXTF:
				//case FILE_TYPE_TXTS:
				//	szTWSendBuf = szTWSendBuf + MakebmpRVF(nScreenWidth, nScreenHeight, nScreenColor, nScreenPixels
				//		, nScreenPixType, nScreenStyle, t_rvstyle, t_RVReportHelper
				//		, Screen_ja[(int)nScreenOrd]["Screen_lstDYArea"][(int)nDYAreaOrd]["Area_lstfile"][(int)J]
				//		, nRealX, nRealY, nRealW, nRealH, ntmpPageCount); //�򿪶����ı��������ı�RVF��t_rvstyle
				//	nTotlePagecount = nTotlePagecount + ntmpPageCount;
				//	break;
				//case FILE_TYPE_TBF:
				//	szTWSendBuf = szTWSendBuf + MakebmpGRID(nScreenWidth, nScreenHeight, nScreenColor, nScreenPixels
				//		, nScreenPixType, nScreenStyle, t_RVReportHelper
				//		, Screen_ja[(int)nScreenOrd]["Screen_lstDYArea"][(int)nDYAreaOrd]["Area_lstfile"][(int)J]
				//		, nRealX, nRealY, nRealW, nRealH, ntmpPageCount); //�򿪱���ļ�
				//	nTotlePagecount = nTotlePagecount + ntmpPageCount;
				//	break;
				case FILE_TYPE_TEXT:
					szTWSendBuf = szTWSendBuf + MakebmpText(nScreenWidth, nScreenHeight, nScreenColor, nScreenPixels
						, nScreenPixType, nScreenStyle, t_rvstyle, t_memo, t_RVReportHelper
						, Screen_ja[(int)nScreenOrd]["Screen_lstDYArea"][(int)nDYAreaOrd]["Area_lstfile"][(int)J]
						, nRealX, nRealY, nRealW, nRealH, ntmpPageCount); //��txt�ı��ļ� ,���а����ⲿ�򿪣��Ƿ�ʹ�õ��л����
					nTotlePagecount = nTotlePagecount + ntmpPageCount;
					break;
				}
			}
		}
	}
	else
	{
		char szAreaName[32];
		sprintf(szAreaName, "��̬-%d", Screen_ja[(int)nScreenOrd]["Screen_lstDYArea"][(int)nDYAreaOrd]["DY_AreaID"].asUInt());
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
//	Json::Value AreaFile_Obj, DWORD nX, DWORD nY, DWORD nWidth, DWORD nHeight, DWORD &nFilePageCount) //�ֶ���ӵ�RVF�ļ�
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
///////////////////////////Э���ͷ
//	nAllWidth= calpagesize(nX, nWidth, thrd_ScreenColor, thrd_ScreenPixType);
//	nPageSize= GetPageSize(nAllWidth, nHeight, thrd_ScreenColor) + 9;
////���ÿһҳ�����ݵĴ�СnPageSize
//	t_RVReportHelper.RichView.Clear;
//	t_RVReportHelper.RichView.DeleteUnusedStyles(true, true, true);
//	if WideFileExists(AreaFile_Obj.S["File_Name"]) then //���ⲿ�ļ����ж�RTF�ļ�
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
//	end else //�������ı������ڣ���ô��ֱ����һ������������ʾNIL
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

//if (AreaFile_Obj["File_ShowStyle"] = file_showstyle_single) then //���е�ģʽ
//begin
//// realw :=
//calRTFsingalpage(t_RVReportHelper, nWidth, nHeight, realw, realh, nrealw);
//if ((realw mod nWidth) <> 0) or (realw = 0) then
//ntmpFilePageCount = realw div nWidth + 1 else
//ntmpFilePageCount = realw div nWidth;
//FillPageInfo(t_RVReportHelper, nWidth, realh); //ת��ΪͼƬ
//nRealityRunSpeed= GetControlStuntRealityRunSpeed(thrd_ScreenWidth, thrd_ScreenHeight
//, thrd_ScreenColor, AreaFile_Obj["File_Stunt"]
//, AreaFile_Obj["File_Speed"]
//, nDefaultRunSpeed);
//	if AreaFile_Obj["File_Stunt"] = 38 then //��������
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
//		if AreaFile_Obj["File_Stunt"] = 6 then //��������
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
//		else //��������ʾ��ʽ
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
//bGBX  = False;
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
//		   bGBX = True;
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
//end else //������ʽ
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
//if bGBX = True then
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
//szcurPagebuf = Char(nPageStyle) //��������
//	+ Char(nStunt)
//	+ Char(nOutStunt)
//	+ Char(nRunSpeed)
//	+ Char(nShowtime and $FF)
//	+ Char((nShowtime shr 8) and $00FF)
//	+ Char(nShowCount)
//	+ Char(nReserved1) //������
//	+ Char(nReserved2) //������
//	+ Char(nReserved3) //������
//	+ Char(nReserved4) //������
//	+ Char(nReserved5) //������
//	+ Char(nReserved6); //������
//
//nCurAddress = Length(szcurPagebuf);
//nPageSize = GetPageSize(nAllWidth, h, nPx);
//	SetLength(szcurPagebuf, nCurAddress + nPageSize);
//	Inc(nCurAddress);
//	//��Ҫ����Чҳ���ݽ��д���ͼƬ�������账�� ��ҳ��Ϊ1
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
		nFrame_DispStype = Screen_Obj["Screen_lstDYArea"][(int)nDYAreaOrd]["DY_AreaFStunt"].asUInt();

		if (nFrame_DispStype == 6) //��Ϊ��PC��������ε��ˡ����̽���ת�����ؼ�����˵��߿��ؼ�Ϊ6���趨Ϊ�̶���ʾ��
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
		RECT rect = { 0, 0, bi.bmiHeader.biWidth, bi.bmiHeader.biHeight };
		RECT rect2 = { 0, 0, bi.bmiHeader.biWidth, bi.bmiHeader.biHeight };

		memset(&bi, 0, sizeof(BITMAPINFO));
		bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bi.bmiHeader.biWidth = PRO_FRAME_LENGTH;
		bi.bmiHeader.biHeight = Screen_Obj["Screen_lstDYArea"][(int)nDYAreaOrd]["DY_AreaFWidth"].asUInt();
		bi.bmiHeader.biBitCount = 24;
		bi.bmiHeader.biSizeImage = (bi.bmiHeader.biWidth * 3 + 1) / 2 * 2;
		bi.bmiHeader.biPlanes = 1;

		bmp = CreateDIBSection(hMemoryDC, &bi, DIB_RGB_COLORS, (void**)&pBmpBits, NULL, 0);
		bmp2 = NULL;
		SelectObject(hMemoryDC, bmp);
		FillRect(hMemoryDC, &rect, hBlackBrush);

		if (Screen_Obj["Screen_lstDYArea"][(int)nDYAreaOrd]["DY_AreaFMode"].asUInt() == 0) //��ɫ
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

				for (size_t n = 0; n < rect2.bottom; n++)
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
		else if (Screen_Obj["Screen_lstDYArea"][(int)nDYAreaOrd]["DY_AreaFMode"].asUInt() == 1) //��ɫ
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

				for (size_t n = 0; n < bi.bmiHeader.biHeight; n++)
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
		TranCanvToInfo(bmp, szFrameData
			, 0, bmp.Width, bmp.Height, Screen_Obj["Screen_color"].asUInt()
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