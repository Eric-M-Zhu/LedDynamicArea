#pragma once

const DWORD VERYLARGEVALUE = 0xFFFFFFF;
const int START_YEAR = 1901;
const int END_YEAR = 2050;

typedef DWORD TTntMemo;
typedef DWORD TRVReportHelper;
typedef DWORD TRVStyle;
typedef DWORD TCustomRVData;
typedef DWORD IGPImage;
typedef DWORD TCanvas;
typedef DWORD TBitmap;

const string stylelist[] = { "tm_year", "tm_month", "tm_day", "tm_hour",
"tm_minute", "tm_second", "tm_apm", "tm_week", "tm_bp", "tm_js", "tm_jr",
"tm_ymd", "tm_hns", "tm_txt" };
//const string stylearr[] = { "year_list", "month_list", "day_list", "hour_list", "minute_list", "second_list",
//"ampm_list", "week_list", "bp_list", "js_list", "jr_list", "ymd_list", "hns_list" };
const string Decimal[] = { "12", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11" };
const string Roman[] = { "XII", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX", "X", "XI" };

typedef struct
{
	string Name;
	int ID;
} TCharSetRec;

string MakeTWValue(Json::Value Controller_obj, BYTE nProtocolVer,
	DWORD thrd_ScreenWidth, DWORD thrd_ScreenHeight, DWORD thrd_ScreenColor, DWORD thrd_ScreenPixels,
	DWORD thrd_ScreenPixType, DWORD thrd_ScreenStyle, SYSTEMTIME &dtFirmwareDateTimeVer, TRVStyle t_rvstyle,
	TTntMemo t_memo, TRVReportHelper t_RVReportHelper, Json::Value Screen_Supary, Json::Value project_Supary,
	DWORD nX, DWORD nY, DWORD nWidth, DWORD nHeight, DWORD nRimTop, DWORD nRimLeft, DWORD nRimRight, DWORD nRimBottom,
	DWORD nScreenOrd, DWORD nProgramOrd, DWORD nAreaOrd); //制作节目的图文区域的内容 ------新卡

string MakeNULLArea(string szAreaName, DWORD w, DWORD h, DWORD x, DWORD nPx, DWORD nMkStyle,
	DWORD nKardPixType, DWORD nScreenStyle, DWORD nStunt, DWORD nOutStunt, DWORD nRunSpeed, DWORD nShowTime, DWORD nShowCount, 
	DWORD nReserved1, DWORD nReserved2, DWORD nReserved3, DWORD nReserved4, DWORD nReserved5, DWORD nReserved6, DWORD nReserved7);
DWORD calpagesize(DWORD left, DWORD width, DWORD nPx, DWORD nKardPixType); //算出单页的宽度

void calRTFsingalpage(TRVReportHelper t_RVReportHelper, DWORD w, DWORD h,
	DWORD &realw, DWORD &realh, DWORD &nrealw); //: Cardinal; //算出RTF单行显示的时候的实际宽度

string MakeString(DWORD thrd_ScreenWidth, DWORD thrd_ScreenHeight, DWORD thrd_ScreenColor,
	DWORD thrd_ScreenPixels, DWORD thrd_ScreenPixType, DWORD thrd_ScreenStyle, TRVStyle t_rvstyle,
	TRVReportHelper t_RVReportHelper, Json::Value project_Supary, DWORD nX, DWORD nY,
	DWORD nWidth, DWORD nHeight, DWORD nProgramOrd, DWORD nAreaOrd, DWORD &nFilePageCount); //打开txt文本文件 ,其中包含外部打开，是否使用单行或多行
void RemoveParagraphBreaks(TCustomRVData RVData);
//转换每一条信息的内容，这些信息包含txt,jpg,gif,多行文本，单行文本，时间，表格
//图文区域中的文件转换
string MakebmpTxt(DWORD thrd_ScreenWidth, DWORD thrd_ScreenHeight, DWORD thrd_ScreenColor,
	DWORD thrd_ScreenPixels, DWORD thrd_ScreenPixType, DWORD thrd_ScreenStyle, TRVStyle t_rvstyle,
	TTntMemo t_memo, TRVReportHelper t_RVReportHelper, Json::Value AreaFile_Obj,
	DWORD nX, DWORD nY, DWORD nWidth, DWORD nHeight, DWORD &nFilePageCount); //打开txt文本文件 ,其中包含外部打开，是否使用单行或多行
string MakebmpText(DWORD thrd_ScreenWidth, DWORD thrd_ScreenHeight, DWORD thrd_ScreenColor,
	DWORD thrd_ScreenPixels, DWORD thrd_ScreenPixType, DWORD thrd_ScreenStyle, TRVStyle t_rvstyle,
	TTntMemo t_memo, TRVReportHelper t_RVReportHelper, Json::Value AreaFile_Obj,
	DWORD nX, DWORD nY, DWORD nWidth, DWORD nHeight, DWORD  &nFilePageCount); //文本

string MakebmpRTF(DWORD thrd_ScreenWidth, DWORD thrd_ScreenHeight, DWORD thrd_ScreenColor,
	DWORD thrd_ScreenPixels, DWORD thrd_ScreenPixType, DWORD thrd_ScreenStyle,
	TRVReportHelper t_RVReportHelper, Json::Value AreaFile_Obj,
	DWORD nX, DWORD nY, DWORD nWidth, DWORD nHeight, DWORD &nFilePageCount); //打开RTF文件，其中包含外部打开
string  MakebmpJPGBMP(DWORD thrd_ScreenWidth, DWORD thrd_ScreenHeight, DWORD thrd_ScreenColor,
	DWORD thrd_ScreenPixels, DWORD thrd_ScreenPixType, DWORD thrd_ScreenStyle,
	Json::Value AreaFile_Obj, DWORD nX, DWORD nY, DWORD nWidth, DWORD nHeight,
	DWORD &nFilePageCount); //打开图片文件，主要是外部打开
string MakebmpRVF(DWORD thrd_ScreenWidth, DWORD thrd_ScreenHeight, DWORD thrd_ScreenColor,
	DWORD thrd_ScreenPixels, DWORD thrd_ScreenPixType, DWORD thrd_ScreenStyle, TRVStyle t_rvstyle,
	TRVReportHelper t_RVReportHelper, Json::Value AreaFile_Obj,
	DWORD nX, DWORD nY, DWORD nWidth, DWORD nHeight, DWORD &nFilePageCount); //打开多行文本，单行文本RVF文件
string MakebmpGRID(DWORD thrd_ScreenWidth, DWORD thrd_ScreenHeight, DWORD thrd_ScreenColor,
	DWORD thrd_ScreenPixels, DWORD thrd_ScreenPixType, DWORD thrd_ScreenStyle,
	TRVReportHelper t_RVReportHelper, Json::Value AreaFile_Obj,
	DWORD nX, DWORD nY, DWORD nWidth, DWORD nHeight, DWORD &nFilePageCount); //打开表格文件
string MakebmpGIF(DWORD thrd_ScreenWidth, DWORD thrd_ScreenHeight, DWORD thrd_ScreenColor,
	DWORD thrd_ScreenPixels, DWORD thrd_ScreenPixType, DWORD thrd_ScreenStyle, Json::Value AreaFile_Obj,
	DWORD nX, DWORD nY, DWORD nWidth, DWORD nHeight, DWORD &nFilePageCount); //打开GIF文件

string MakegifImage(DWORD count, DWORD w, DWORD h, DWORD x, DWORD nPx, DWORD nMkStyle, DWORD nKardPixType, DWORD nScreenStyle, IGPImage t_img,
	DWORD nDataStyle, DWORD nStunt, DWORD nOutStunt, DWORD nRunSpeed, DWORD nShowTime, DWORD nShowCount
	, DWORD nReserved1, DWORD nReserved2, DWORD nReserved3, DWORD nReserved4, DWORD nReserved5
	, DWORD nReserved6, DWORD nReserved7, DWORD &nRealPageCount); //转换GIF数据

string MakeDoubleImage(TRVReportHelper t_RVReportHelper, DWORD count, DWORD w, DWORD h, DWORD x, DWORD nPx, DWORD nMkStyle, DWORD nKardPixType, DWORD nScreenStyle,
	DWORD nStunt, DWORD nOutStunt, DWORD nRunSpeed, DWORD nShowTime, DWORD nShowCount, DWORD nReserved1
	, DWORD nReserved2, DWORD nReserved3, DWORD nReserved4, DWORD nReserved5, DWORD nReserved6
	, DWORD nReserved7, bool bOnePage, DWORD &nRealPageCount); //转换多行数据

string MakeGridImage(TRVReportHelper t_RVReportHelper, DWORD count, DWORD w, DWORD h, DWORD x, DWORD nPx, DWORD nMkStyle, DWORD nKardPixType, DWORD nScreenStyle,
	DWORD nDataStyle, DWORD nStunt, DWORD nOutStunt, DWORD nRunSpeed, DWORD nShowTime, DWORD nShowCount
	, DWORD nReserved1, DWORD nReserved2, DWORD nReserved3, DWORD nReserved4, DWORD nReserved5
	, DWORD nReserved6, DWORD nReserved7, DWORD &nRealPageCount); //转换表格数据

string MakeSigalImage(TRVReportHelper t_RVReportHelper,
	DWORD count, DWORD w, DWORD h, DWORD x, DWORD nPx, DWORD nMkStyle, DWORD nKardPixType, DWORD nScreenStyle,
	DWORD nStunt, DWORD nOutStunt, DWORD nRunSpeed, DWORD nShowTime, DWORD nShowCount,
	DWORD nReserved1, DWORD nReserved2, DWORD nReserved3, DWORD nReserved4, DWORD nReserved5, DWORD nReserved6,
	DWORD nReserved7, DWORD nrealwidth, bool bOnePage, DWORD &nRealPageCount);

string MakeRightSigalImage(TRVReportHelper t_RVReportHelpe,
	DWORD count, DWORD w, DWORD h, DWORD x, DWORD nPx, DWORD nMkStyle, DWORD nKardPixType, DWORD nScreenStyle,
	DWORD nStunt, DWORD nOutStunt, DWORD nRunSpeed, DWORD nShowTime, DWORD nShowCount,
	DWORD nReserved1, DWORD nReserved2, DWORD nReserved3, DWORD nReserved4, DWORD nReserved5, DWORD nReserved6,
	DWORD nReserved7, DWORD nrealwidth, bool bOnePage, DWORD &nRealPageCount); //转换向右连移单行数据

string MakebmpImage(string filename, DWORD w, DWORD h, DWORD x, DWORD nPx, DWORD nMkStyle, DWORD nKardPixType, DWORD nScreenStyle
	, DWORD nStunt, DWORD nOutStunt, DWORD nRunSpeed, DWORD nShowTime, DWORD nShowCount
	, DWORD nReserved1, DWORD nReserved2, DWORD nReserved3, DWORD nReserved4, DWORD nReserved5, DWORD nReserved6
	, DWORD nReserved7, DWORD &nRealPageCount); //转换图片数据
string MakePageImageupscroll(TRVReportHelper t_RVReportHelper, DWORD count, DWORD w, DWORD h, DWORD x, DWORD nPx, DWORD nMkStyle, DWORD nKardPixType, DWORD nScreenStyle,
	DWORD nStunt, DWORD nOutStunt, DWORD nRunSpeed, DWORD nShowTime, DWORD nShowCount
	, DWORD nReserved1, DWORD nReserved2, DWORD nReserved3, DWORD nReserved4, DWORD nReserved5
	, DWORD nReserved6, DWORD nReserved7, bool bOnePage, DWORD &nRealPageCount);

void FillPageInfoupscroll(TRVReportHelper t_RVReportHelper, DWORD w, DWORD h, DWORD &nupscrollPageCount); //向上连移的时候处理

DWORD MMToPixels(DWORD mm, DWORD ppi);
void FillPageInfo(TRVReportHelper t_RVReportHelper, DWORD w, DWORD h);
void DrawPage(TRVReportHelper t_RVReportHelper, TCanvas Canvas, DWORD PageNo, DWORD h);
void DrawGRIDPage(TRVReportHelper t_RVReportHelper, TCanvas Canvas, DWORD PageNo, DWORD h);
void DrawsingalPage(TRVReportHelper t_RVReportHelper, TCanvas Canvas, DWORD h); //做单行处理

DWORD realspeed(DWORD w, DWORD h, DWORD sped, string c_style); //返回实际的速度值
int rgbshow(int rgb, DWORD nScreenColor, DWORD nMkStyle); //返回颜色的转换算法
DWORD ColorOrdShow(DWORD nColorOrd, DWORD nScreenColor, DWORD nMkStyle); //通过颜色序号得到颜色值。
																			   //下面的数据是转换图片为实际数据的过程
list<string> SplitString(const string &Source, string &Ch);
string IntConvertHex(DWORD A, DWORD Len);
string tohex(string s);

//文字动画
void DrawFinalImage(int File_Width, int File_Height, int Screen_color, int Screen_pix, TBitmap bmp2);

string MakeProValue(DWORD nScreenOrd, DWORD nProgramOrd,
	SYSTEMTIME dtFirmwareDateTimeVer, BYTE ntmpBXCEColor,
	Json::Value Screen_Supary, Json::Value project_Supary, DWORD &nSendLength); //制作节目的属性参数  ------新卡
string BuildProgramInfo(DWORD screenid, DWORD proid,
	SYSTEMTIME dtFirmwareDateTimeVer, Json::Value Screen_Supary, Json::Value project_Supary,
	DWORD &nSendLength, BYTE ntmpBXCEColor);

void ScreenProgramsInfo(DWORD nScreenOrd, list<DWORD> lstSendProgram, Json::Value Screen_ja, Json::Value Program_ja);

//nPlayMode:上电是否显示；0：上电不显示；1：上电显示
string MakeScreenLOGOValue(Json::Value Screen_Supary, DWORD nScreenOrd, DWORD &nSendLength); //制作LOGO画面界面。

DWORD GetPageSize(DWORD nAllWidth, DWORD nHeight, DWORD nPx);

void TranCanvToInfo(LPBYTE Bmp, string &pInfoBuf,
	DWORD nLeft, DWORD nWidth, DWORD nHeight, DWORD nPx, DWORD nMkStyle, DWORD nPixType,
	DWORD nControllerType, bool &bInvalidData);

string MakeFrameValue(Json::Value Screen_Obj, DWORD nDYAreaOrd);
void GetRealDYAreaLocation(Json::Value DYArea_Obj, DWORD &nRealX, DWORD &nRealY, DWORD &nRealW, DWORD &nRealH);


string MakeDynamicAreaInfo(DWORD nScreenOrd, DWORD nDYAreaOrd, Json::Value Screen_ja,
	DWORD &nSendLength);

string MakebmpTxtSinglePage(DWORD thrd_ScreenWidth, DWORD thrd_ScreenHeight, DWORD thrd_ScreenColor,
	DWORD thrd_ScreenPixels, DWORD thrd_ScreenPixType, DWORD thrd_ScreenStyle, TRVStyle t_rvstyle,
	TTntMemo t_memo, TRVReportHelper t_RVReportHelper, Json::Value AreaFile_Obj,
	DWORD nX, DWORD nY, DWORD nWidth, DWORD nHeight, DWORD &nFilePageCount); //打开txt文本文件 ,其中包含外部打开，是否使用单行或多行

string MakebmpTextSinglePage(DWORD thrd_ScreenWidth, DWORD thrd_ScreenHeight, DWORD thrd_ScreenColor,
	DWORD thrd_ScreenPixels, DWORD thrd_ScreenPixType, DWORD thrd_ScreenStyle, TRVStyle t_rvstyle,
	TTntMemo t_memo, TRVReportHelper t_RVReportHelper, Json::Value AreaFile_Obj,
	DWORD nX, DWORD nY, DWORD nWidth, DWORD nHeight, DWORD &nFilePageCount); //打开txt文本文件 ,其中包含外部打开，是否使用单行或多行

string MakeDynamicAreaPageInfo(DWORD nScreenOrd, DWORD nDYAreaOrd, DWORD nFileOrd, DWORD nPageOrd, Json::Value Screen_ja,
	DWORD &nSendLength);