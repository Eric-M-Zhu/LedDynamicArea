#include "stdafx.h"
#include "Unit_build.h"

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
	int I;
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

	sscanf(DateStr.c_str(), "%d-%d-%d %d:%d:%d", &dateTime.wYear, &dateTime.wMonth, &dateTime.wDay, 
		&dateTime.wHour, &dateTime.wMinute, &dateTime.wSecond);
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
//bRed : = False;
//bGreen: = False;
//bBlue: = False;
//btmpInvalidData: = True;
//nIndex: = 0;
//case nPx of
//SCREEN_COLOR_SINGLE, SCREEN_COLOR_DOUBLE, SCREEN_COLOR_THREE:
//	begin
//		iRightBit : = 0;
//	if ((nLeft + nWidth) mod nPixType <> 0) then
//		iRightBit : = nPixType - ((nLeft + nWidth) mod nPixType); //右边起始字节的多余位数
//iLeftBit: = nLeft mod nPixType; //左边起始字节的多余位数
//nAllWidth: = iLeftBit + nWidth + iRightBit;
//nDimensions: = nAllWidth * nHeight;
//bitsRed: = TBits.Create;
//	bitsRed.Size : = nDimensions;
//bitsGreen: = TBits.Create;
//	bitsGreen.Size : = nDimensions;
//bitsBlue: = TBits.Create;
//	bitsBlue.Size : = nDimensions;
//	for h : = 1 to nHeight do
//		begin
//		for w : = 1 to nAllWidth do
//			begin
//			if ((w - 1 < iLeftBit) or (w - 1 >= (iLeftBit + nWidth))) then
//				Col : = RGB(0, 0, 0)
//			else
//				Col : = Bmp.Canvas.Pixels[w - 1 - iLeftBit, h - 1];
//ColR: = GetRValue(Col);
//ColG: = GetGValue(Col);
//ColB: = GetBValue(Col);
//	if (nPx = SCREEN_COLOR_DOUBLE) then
//		begin
//		if (nMkStyle = DOUBLE_COLOR_PIXTYPE_1) then //双基色，R+G
//			bRed : = ColR > 127
//		else //双基色，G+R
//			bGreen : = ColR > 127;
//	if (bRed = True) or (bGreen = True) then
//		btmpInvalidData : = False;
//	end
//	else if (nPx = SCREEN_COLOR_SINGLE) then
//		begin
//		bRed : = ColR > 127;
//	if (bRed = True) then
//		btmpInvalidData : = False;
//	end
//	else if (nPx = SCREEN_COLOR_THREE) then
//		begin
//		bRed : = ColR > 127;
//	if (bRed = True) then
//		btmpInvalidData : = False;
//	end;
//	if (nPx = SCREEN_COLOR_DOUBLE) then
//		begin
//		if (nMkStyle = DOUBLE_COLOR_PIXTYPE_1) then //双基色，R+G
//			bGreen : = ColG > 127
//		else //双基色，G+R
//			bRed : = ColG > 127;
//	if (bRed = True) or (bGreen = True) then
//		btmpInvalidData : = False;
//	end
//	else if (nPx = SCREEN_COLOR_SINGLE) then
//		begin
//		bGreen : = ColG > 127;
//	//单基色去绿色数据没有用处。因此不作判断
//	end
//	else if (nPx = SCREEN_COLOR_THREE) then
//		begin
//		bGreen : = ColG > 127;
//	if (bGreen = True) then
//		btmpInvalidData : = False;
//	end;
//	if (nPx = SCREEN_COLOR_THREE) then
//		begin
//		bBlue : = ColB > 127;
//	if (bBlue = True) then
//		btmpInvalidData : = False;
//	end;
//	bitsRed[nIndex] : = not bRed;
//	bitsGreen[nIndex] : = not bGreen;
//	bitsBlue[nIndex] : = not bBlue;
//	Inc(nIndex);
//	end;
//	end;
//	if (nPx = SCREEN_COLOR_THREE) then
//		begin
//		for I : = 1 to nDimensions div 8 do
//			begin
//			byRed : = Ord(bitsRed[(I - 1) * 8]) shl 7 + Ord(bitsRed[(I - 1) * 8 + 1]) shl 6 + Ord(bitsRed[(I - 1) * 8 + 2]) shl 5 + Ord(bitsRed[(I - 1) * 8 + 3]) shl 4
//			+ Ord(bitsRed[(I - 1) * 8 + 4]) shl 3 + Ord(bitsRed[(I - 1) * 8 + 5]) shl 2 + Ord(bitsRed[(I - 1) * 8 + 6]) shl 1 + Ord(bitsRed[(I - 1) * 8 + 7]);
//byGreen: = Ord(bitsGreen[(I - 1) * 8]) shl 7 + Ord(bitsGreen[(I - 1) * 8 + 1]) shl 6 + Ord(bitsGreen[(I - 1) * 8 + 2]) shl 5 + Ord(bitsGreen[(I - 1) * 8 + 3]) shl 4
//	+ Ord(bitsGreen[(I - 1) * 8 + 4]) shl 3 + Ord(bitsGreen[(I - 1) * 8 + 5]) shl 2 + Ord(bitsGreen[(I - 1) * 8 + 6]) shl 1 + Ord(bitsGreen[(I - 1) * 8 + 7]);
//	 byBlue: = Ord(bitsBlue[(I - 1) * 8]) shl 7 + Ord(bitsBlue[(I - 1) * 8 + 1]) shl 6 + Ord(bitsBlue[(I - 1) * 8 + 2]) shl 5 + Ord(bitsBlue[(I - 1) * 8 + 3]) shl 4
//		 + Ord(bitsBlue[(I - 1) * 8 + 4]) shl 3 + Ord(bitsBlue[(I - 1) * 8 + 5]) shl 2 + Ord(bitsBlue[(I - 1) * 8 + 6]) shl 1 + Ord(bitsBlue[(I - 1) * 8 + 7]);
//			 pInfoBuf^ : = char(byRed);
//			 Inc(pInfoBuf);
//			 pInfoBuf^ : = char(byGreen);
//			 Inc(pInfoBuf);
//			 pInfoBuf^ : = char(byBlue);
//			 Inc(pInfoBuf);
//			 end;
//			 end
//	else if (nPx = SCREEN_COLOR_DOUBLE) then
//				 begin
//		for I : = 1 to nDimensions div 8 do
//			begin
//			byRed : = Ord(bitsRed[(I - 1) * 8]) shl 7 + Ord(bitsRed[(I - 1) * 8 + 1]) shl 6 + Ord(bitsRed[(I - 1) * 8 + 2]) shl 5 + Ord(bitsRed[(I - 1) * 8 + 3]) shl 4
//			+ Ord(bitsRed[(I - 1) * 8 + 4]) shl 3 + Ord(bitsRed[(I - 1) * 8 + 5]) shl 2 + Ord(bitsRed[(I - 1) * 8 + 6]) shl 1 + Ord(bitsRed[(I - 1) * 8 + 7]);
//byGreen: = Ord(bitsGreen[(I - 1) * 8]) shl 7 + Ord(bitsGreen[(I - 1) * 8 + 1]) shl 6 + Ord(bitsGreen[(I - 1) * 8 + 2]) shl 5 + Ord(bitsGreen[(I - 1) * 8 + 3]) shl 4
//	+ Ord(bitsGreen[(I - 1) * 8 + 4]) shl 3 + Ord(bitsGreen[(I - 1) * 8 + 5]) shl 2 + Ord(bitsGreen[(I - 1) * 8 + 6]) shl 1 + Ord(bitsGreen[(I - 1) * 8 + 7]);
//		 pInfoBuf^ : = char(byRed);
//		 Inc(pInfoBuf);
//		 pInfoBuf^ : = char(byGreen);
//		 Inc(pInfoBuf);
//		 end;
//		 end
//	else if (nPx = SCREEN_COLOR_SINGLE) then
//			 begin
//				 for I : = 1 to nDimensions div 8 do
//					 begin
//					 byRed : = Ord(bitsRed[(I - 1) * 8]) shl 7 + Ord(bitsRed[(I - 1) * 8 + 1]) shl 6 + Ord(bitsRed[(I - 1) * 8 + 2]) shl 5 + Ord(bitsRed[(I - 1) * 8 + 3]) shl 4
//					 + Ord(bitsRed[(I - 1) * 8 + 4]) shl 3 + Ord(bitsRed[(I - 1) * 8 + 5]) shl 2 + Ord(bitsRed[(I - 1) * 8 + 6]) shl 1 + Ord(bitsRed[(I - 1) * 8 + 7]);
//			 pInfoBuf^ : = char(byRed);
//			 Inc(pInfoBuf);
//			 end;
//			 end;
//			 FreeAndNil(bitsRed);
//			 FreeAndNil(bitsGreen);
//			 FreeAndNil(bitsBlue);
//		 bInvalidData: = btmpInvalidData;
//			 end;
//		 SCREEN_COLOR_FULLCOLOR:
//			 begin
//				 nAllWidth : = nWidth;
//		 nDimensions: = nAllWidth * nHeight;
//			 bmp.PixelFormat : = pf16bit;
//			 for h : = 1 to nHeight do
//				 begin
//				 p : = bmp.ScanLine[h - 1];
//			 for W : = 1 to nAllWidth do
//				 begin
//				 if (p[(W - 1) * 2 + 1] <> 0) or (p[(W - 1) * 2] <> 0) then
//					 btmpInvalidData : = False;
//			 //双字节参数值表示为R(5bits)+G(6bits)+B(5bits) 生成数据时，先低字节，然后高字节。
//
//			 pInfoBuf^ : = char(p[(W - 1) * 2]); //低字节
//			 Inc(pInfoBuf);
//			 pInfoBuf^ : = char(p[(W - 1) * 2 + 1] and $FF); //高字节
//			 Inc(pInfoBuf);
//
//			 end;
//			 end;
//		 bInvalidData: = btmpInvalidData;
//			 DoEvents;
//			 end;
//		 SCREEN_COLOR_TWO_COLOR_GRAY:
//			 begin
//				 nAllWidth : = nWidth;
//		 nDimensions: = nAllWidth * nHeight;
//			 bmp.PixelFormat : = pf16bit;
//			 for h : = 1 to nHeight do
//				 begin
//				 p : = bmp.ScanLine[h - 1];
//			 for W : = 1 to nAllWidth do
//				 begin
//				 if (p[(W - 1) * 2 + 1] <> 0) or (p[(W - 1) * 2] <> 0) then
//					 btmpInvalidData : = False;
//			 //双字节参数值表示为R(5bits)+G(6bits)+B(5bits) 生成数据时，先低字节，然后高字节。
//		 nFullColorPixelData: = p[(W - 1) * 2] + (p[(W - 1) * 2 + 1] shl 8) and $FF00;
//		 ColR: = nFullColorPixelData shr 11;
//		 ColG: = (nFullColorPixelData shr 5) and $3F;
//		 colB: = nFullColorPixelData and $1F;
//		 nFullColorPixelData: = (colB shl 11) and $F800 + (ColG shl 5) and $07E0 + ColR;
//
//			 pInfoBuf^ : = char(nFullColorPixelData and $FF); //低字节
//			 Inc(pInfoBuf);
//			 pInfoBuf^ : = char((nFullColorPixelData shr 8) and $FF); //高字节
//			 Inc(pInfoBuf);
//			 end;
//			 end;
//		 bInvalidData: = btmpInvalidData;
//			 DoEvents;
//			 end;
//			 end;
//			 end;