#pragma once

const UINT VERYLARGEVALUE = 0xFFFFFFF;
const int START_YEAR = 1901;
const int END_YEAR = 2050;

const string stylelist[] = { "tm_year", "tm_month", "tm_day", "tm_hour",
"tm_minute", "tm_second", "tm_apm", "tm_week", "tm_bp", "tm_js", "tm_jr",
"tm_ymd", "tm_hns", "tm_txt" };
const string stylearr[] = { "year_list", "month_list", "day_list", "hour_list", "minute_list", "second_list",
"ampm_list", "week_list", "bp_list", "js_list", "jr_list", "ymd_list", "hns_list" };
const string Decimal[] = { "12", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11" };
const string Roman[] = { "XII", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX", "X", "XI" };

typedef struct
{
	string Name;
	string ID;
} TCharSetRec;

//string MakeTWValue(Json::Value Controller_obj, BYTE nProtocolVer,
//	UINT thrd_ScreenWidth, UINT thrd_ScreenHeight, UINT thrd_ScreenColor, UINT thrd_ScreenPixels,
//	UINT thrd_ScreenPixType, UINT thrd_ScreenStyle, SYSTEMTIME &dtFirmwareDateTimeVer, TRVStyle t_rvstyle,
//	TTntMemo t_memo, TRVReportHelper t_RVReportHelper, Screen_Supary, project_Supary: TSuperArray;
//	UINT nX, UINT nY, UINT nWidth, UINT nHeight, UINT nRimTop, UINT nRimLeft, UINT nRimRight, UINT nRimBottom,
//	UINT nScreenOrd, UINT nProgramOrd, UINT nAreaOrd); //������Ŀ��ͼ����������� ------�¿�

string MakeNULLArea(string szAreaName, UINT w, UINT h, UINT x, UINT nPx, UINT nMkStyle,
	UINT nKardPixType, UINT nScreenStyle, UINT nStunt, UINT nOutStunt, UINT nRunSpeed, UINT nShowTime, UINT nShowCount, 
	UINT nReserved1, UINT nReserved2, UINT nReserved3, UINT nReserved4, UINT nReserved5, UINT nReserved6, UINT nReserved7);
UINT calpagesize(UINT left, UINT width, UINT nPx, UINT nKardPixType); //�����ҳ�Ŀ��
//void calRTFsingalpage(TRVReportHelper t_RVReportHelper, UINT w, UINT h,
//	UINT &realw, UINT &realh, UINT &nrealw); //: Cardinal; //���RTF������ʾ��ʱ���ʵ�ʿ��

//function MakeString(thrd_ScreenWidth, thrd_ScreenHeight, thrd_ScreenColor,
//	thrd_ScreenPixels, thrd_ScreenPixType, thrd_ScreenStyle: Cardinal; t_rvstyle: TRVStyle;
//t_RVReportHelper: TRVReportHelper; project_Supary: TSuperArray; nX, nY,
//	nWidth, nHeight: Cardinal; nProgramOrd, nAreaOrd: Cardinal; var nFilePageCount : Cardinal) : string; //��txt�ı��ļ� ,���а����ⲿ�򿪣��Ƿ�ʹ�õ��л����
//procedure RemoveParagraphBreaks(RVData: TCustomRVData);
////ת��ÿһ����Ϣ�����ݣ���Щ��Ϣ����txt,jpg,gif,�����ı��������ı���ʱ�䣬���
////ͼ�������е��ļ�ת��
//function MakebmpTxt(thrd_ScreenWidth, thrd_ScreenHeight, thrd_ScreenColor,
//	thrd_ScreenPixels, thrd_ScreenPixType, thrd_ScreenStyle: Cardinal; t_rvstyle: TRVStyle;
//t_memo: TTntMemo; t_RVReportHelper: TRVReportHelper; AreaFile_Obj: ISuperObject;
//nX, nY, nWidth, nHeight: Cardinal; var nFilePageCount : Cardinal) : string; //��txt�ı��ļ� ,���а����ⲿ�򿪣��Ƿ�ʹ�õ��л����
//function MakebmpText(thrd_ScreenWidth, thrd_ScreenHeight, thrd_ScreenColor,
//	thrd_ScreenPixels, thrd_ScreenPixType, thrd_ScreenStyle: Cardinal; t_rvstyle: TRVStyle;
//t_memo: TTntMemo; t_RVReportHelper: TRVReportHelper; AreaFile_Obj: ISuperObject;
//nX, nY, nWidth, nHeight: Cardinal; var nFilePageCount : Cardinal) : string; //�ı�
//
//function MakebmpRTF(thrd_ScreenWidth, thrd_ScreenHeight, thrd_ScreenColor,
//	thrd_ScreenPixels, thrd_ScreenPixType, thrd_ScreenStyle: Cardinal;
//t_RVReportHelper: TRVReportHelper; AreaFile_Obj: ISuperObject;
//nX, nY, nWidth, nHeight: Cardinal; var nFilePageCount : Cardinal) : string; //��RTF�ļ������а����ⲿ��
//function MakebmpJPGBMP(thrd_ScreenWidth, thrd_ScreenHeight, thrd_ScreenColor,
//	thrd_ScreenPixels, thrd_ScreenPixType, thrd_ScreenStyle: Cardinal;
//AreaFile_Obj: ISuperObject; nX, nY, nWidth, nHeight: Cardinal;
//var nFilePageCount : Cardinal) : string; //��ͼƬ�ļ�����Ҫ���ⲿ��
//function MakebmpRVF(thrd_ScreenWidth, thrd_ScreenHeight, thrd_ScreenColor,
//	thrd_ScreenPixels, thrd_ScreenPixType, thrd_ScreenStyle: Cardinal; t_rvstyle: TRVStyle;
//t_RVReportHelper: TRVReportHelper; AreaFile_Obj: ISuperObject;
//nX, nY, nWidth, nHeight: Cardinal; var nFilePageCount : Cardinal) : string; //�򿪶����ı��������ı�RVF�ļ�
//function MakebmpGRID(thrd_ScreenWidth, thrd_ScreenHeight, thrd_ScreenColor,
//	thrd_ScreenPixels, thrd_ScreenPixType, thrd_ScreenStyle: Cardinal;
//t_RVReportHelper: TRVReportHelper; AreaFile_Obj: ISuperObject;
//nX, nY, nWidth, nHeight: Cardinal; var nFilePageCount : Cardinal) : string; //�򿪱���ļ�
//function MakebmpGIF(thrd_ScreenWidth, thrd_ScreenHeight, thrd_ScreenColor,
//	thrd_ScreenPixels, thrd_ScreenPixType, thrd_ScreenStyle: Cardinal; AreaFile_Obj: ISuperObject;
//nX, nY, nWidth, nHeight: Cardinal; var nFilePageCount : Cardinal) : string; //��GIF�ļ�
//
//function MakegifImage(count, w, h, x, nPx, nMkStyle, nKardPixType, nScreenStyle: Cardinal; t_img: IGPImage;
//nDataStyle, nStunt, nOutStunt, nRunSpeed, nShowTime, nShowCount
//, nReserved1, nReserved2, nReserved3, nReserved4, nReserved5
//, nReserved6, nReserved7: Cardinal; var nRealPageCount : Cardinal) : string; //ת��GIF����
//
//function MakeDoubleImage(t_RVReportHelper: TRVReportHelper; count, w, h, x, nPx, nMkStyle, nKardPixType, nScreenStyle: Cardinal;
//nStunt, nOutStunt, nRunSpeed, nShowTime, nShowCount, nReserved1
//, nReserved2, nReserved3, nReserved4, nReserved5, nReserved6
//, nReserved7: Cardinal; bOnePage: Boolean; var nRealPageCount : Cardinal) : string; //ת����������
//function MakeGridImage(t_RVReportHelper: TRVReportHelper; count, w, h, x, nPx, nMkStyle, nKardPixType, nScreenStyle: Cardinal;
//nDataStyle, nStunt, nOutStunt, nRunSpeed, nShowTime, nShowCount
//, nReserved1, nReserved2, nReserved3, nReserved4, nReserved5
//, nReserved6, nReserved7: Cardinal; var nRealPageCount : Cardinal) : string; //ת���������
//
//function MakeSigalImage(t_RVReportHelper: TRVReportHelper;
//count, w, h, x, nPx, nMkStyle, nKardPixType, nScreenStyle: Cardinal;
//nStunt, nOutStunt, nRunSpeed, nShowTime, nShowCount,
//nReserved1, nReserved2, nReserved3, nReserved4, nReserved5, nReserved6,
//nReserved7: Cardinal; nrealwidth: Cardinal; bOnePage: Boolean; var nRealPageCount : Cardinal) : string;
//
//function MakeRightSigalImage(t_RVReportHelper: TRVReportHelper;
//count, w, h, x, nPx, nMkStyle, nKardPixType, nScreenStyle: Cardinal;
//nStunt, nOutStunt, nRunSpeed, nShowTime, nShowCount,
//nReserved1, nReserved2, nReserved3, nReserved4, nReserved5, nReserved6,
//nReserved7: Cardinal; nrealwidth: Cardinal; bOnePage: Boolean; var nRealPageCount : Cardinal) : string; //ת���������Ƶ�������
//
//function MakebmpImage(filename: widestring; w, h, x, nPx, nMkStyle, nKardPixType, nScreenStyle
//	, nStunt, nOutStunt, nRunSpeed, nShowTime, nShowCount
//	, nReserved1, nReserved2, nReserved3, nReserved4, nReserved5, nReserved6
//	, nReserved7: Cardinal; var nRealPageCount : Cardinal) : string; //ת��ͼƬ����
//function MakePageImageupscroll(t_RVReportHelper: TRVReportHelper; count, w, h, x, nPx, nMkStyle, nKardPixType, nScreenStyle: Cardinal;
//nStunt, nOutStunt, nRunSpeed, nShowTime, nShowCount
//, nReserved1, nReserved2, nReserved3, nReserved4, nReserved5
//, nReserved6, nReserved7: Cardinal; bOnePage: Boolean; var nRealPageCount : Cardinal) : string;
//
//procedure FillPageInfoupscroll(t_RVReportHelper: TRVReportHelper; w, h: Cardinal; var nupscrollPageCount : Cardinal); //�������Ƶ�ʱ����
//
//function MMToPixels(mm, ppi: Cardinal) : Cardinal;
//procedure FillPageInfo(t_RVReportHelper: TRVReportHelper; w, h: Cardinal);
//procedure DrawPage(t_RVReportHelper: TRVReportHelper; Canvas: TCanvas; PageNo, h: Cardinal);
//procedure DrawGRIDPage(t_RVReportHelper: TRVReportHelper; Canvas: TCanvas; PageNo, h: Cardinal);
//procedure DrawsingalPage(t_RVReportHelper: TRVReportHelper; Canvas: TCanvas; h: Cardinal); //�����д���
//
//function realspeed(w, h, sped: Cardinal; c_style: string) : Cardinal; //����ʵ�ʵ��ٶ�ֵ
//function rgbshow(rgb: integer; nScreenColor, nMkStyle: Cardinal) : integer; //������ɫ��ת���㷨
//function ColorOrdShow(nColorOrd, nScreenColor, nMkStyle: Cardinal) : Cardinal; //ͨ����ɫ��ŵõ���ɫֵ��
//																			   //�����������ת��ͼƬΪʵ�����ݵĹ���
//function SplitString(const Source, Ch: string) : TStringList;
//function IntConvertHex(A: Cardinal; Len: Cardinal) : string;
//function tohex(s: string) : string;
//
////���ֶ���
//procedure DrawFinalImage(File_Width, File_Height, Screen_color, Screen_pix: integer; bmp2: tbitmap);
//
//function MakeProValue(nScreenOrd, nProgramOrd: Cardinal;
//dtFirmwareDateTimeVer: TDateTime; ntmpBXCEColor: byte;
//Screen_Supary, project_Supary: TSuperArray; var nSendLength : Cardinal) : string; //������Ŀ�����Բ���  ------�¿�
//function BuildProgramInfo(screenid, proid: Cardinal;
//dtFirmwareDateTimeVer: TDateTime; Screen_Supary, project_Supary: TSuperArray;
//var nSendLength : Cardinal; ntmpBXCEColor: Byte) : string;
//
//procedure ScreenProgramsInfo(nScreenOrd: Cardinal; lstSendProgram: TList; Screen_ja, Program_ja: TSuperArray);
//
////nPlayMode:�ϵ��Ƿ���ʾ��0���ϵ粻��ʾ��1���ϵ���ʾ
//function MakeScreenLOGOValue(Screen_Supary: TSuperArray; nScreenOrd: Cardinal; var nSendLength : Cardinal) : string; //����LOGO������档
//
//function GetPageSize(nAllWidth, nHeight, nPx: Cardinal) : Cardinal;
//
//procedure TranCanvToInfo(Bmp: TBitmap; pInfoBuf: pChar;
//nLeft, nWidth, nHeight, nPx, nMkStyle, nPixType: Cardinal;
//nControllerType: Cardinal; var bInvalidData : Boolean);
//
//function MakeFrameValue(Screen_Obj: ISuperObject; nDYAreaOrd: Cardinal) : string;
//procedure GetRealDYAreaLocation(DYArea_Obj: ISuperObject; var nRealX, nRealY, nRealW, nRealH: Cardinal);
//
//function MakeDynamicAreaInfo(nScreenOrd, nDYAreaOrd: Cardinal; Screen_ja: TSuperArray;
//var nSendLength : Cardinal) : string;
//
//function MakebmpTxtSinglePage(thrd_ScreenWidth, thrd_ScreenHeight, thrd_ScreenColor,
//	thrd_ScreenPixels, thrd_ScreenPixType, thrd_ScreenStyle: Cardinal; t_rvstyle: TRVStyle;
//t_memo: TTntMemo; t_RVReportHelper: TRVReportHelper; AreaFile_Obj: ISuperObject;
//nX, nY, nWidth, nHeight: Cardinal; var nFilePageCount : Cardinal) : string; //��txt�ı��ļ� ,���а����ⲿ�򿪣��Ƿ�ʹ�õ��л����
//
//function MakebmpTextSinglePage(thrd_ScreenWidth, thrd_ScreenHeight, thrd_ScreenColor,
//	thrd_ScreenPixels, thrd_ScreenPixType, thrd_ScreenStyle: Cardinal; t_rvstyle: TRVStyle;
//t_memo: TTntMemo; t_RVReportHelper: TRVReportHelper; AreaFile_Obj: ISuperObject;
//nX, nY, nWidth, nHeight: Cardinal; var nFilePageCount : Cardinal) : string; //��txt�ı��ļ� ,���а����ⲿ�򿪣��Ƿ�ʹ�õ��л����
//
//function MakeDynamicAreaPageInfo(nScreenOrd, nDYAreaOrd, nFileOrd, nPageOrd: Cardinal; Screen_ja: TSuperArray;
//var nSendLength : Cardinal) : string;