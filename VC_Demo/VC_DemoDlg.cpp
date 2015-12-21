
// VC_DemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VC_Demo.h"
#include "VC_DemoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//------------------------------------------------------------------------------
			//控制器类型
		#define BX_5E1  0x0154
        #define BX_5E2  0x0254
        #define BX_5E3  0x0354
        #define BX_5Q0P  0x1056
        #define BX_5Q1P  0x1156
        #define BX_5Q2P  0x1256

		int controlType[6]  =  {BX_5E1,BX_5E2,BX_5E3,BX_5Q0P,BX_5Q1P,BX_5Q2P};

		//----------------------------------------------------------------------

		#define FRAME_SINGLE_COLOR_COUNT  23 //纯色边框图片个数
        #define FRAME_MULI_COLOR_COUNT  47 //花色边框图片个数

//------------------------------------------------------------------------------
        // 通讯模式
        #define SEND_MODE_SERIALPORT  0
        #define SEND_MODE_NETWORK  2
        #define SEND_MODE_Server_2G  5
        #define SEND_MODE_Server_3G  6
        #define SEND_MODE_SAVEFILE  7
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        // 动态区域运行模式
        #define RUN_MODE_CYCLE_SHOW  0 //动态区数据循环显示；
        #define RUN_MODE_SHOW_LAST_PAGE  1 //动态区数据显示完成后静止显示最后一页数据；
        #define RUN_MODE_SHOW_CYCLE_WAITOUT_NOSHOW  2 //动态区数据循环显示，超过设定时间后数据仍未更新时不再显示；
        #define RUN_MODE_SHOW_ORDERPLAYED_NOSHOW  4 //动态区数据顺序显示，显示完最后一页后就不再显示
        //------------------------------------------------------------------------------
        //==============================================================================
        // 返回状态代码定义
        #define RETURN_ERROR_NOFIND_DYNAMIC_AREA  0xE1 //没有找到有效的动态区域。
        #define RETURN_ERROR_NOFIND_DYNAMIC_AREA_FILE_ORD  0xE2 //在指定的动态区域没有找到指定的文件序号。
        #define RETURN_ERROR_NOFIND_DYNAMIC_AREA_PAGE_ORD  0xE3 //在指定的动态区域没有找到指定的页序号。
        #define RETURN_ERROR_NOSUPPORT_FILETYPE  0xE4 //不支持该文件类型。
        #define RETURN_ERROR_RA_SCREENNO  0xF8 //已经有该显示屏信息。如要重新设定请先DeleteScreen删除该显示屏再添加；
        #define RETURN_ERROR_NOFIND_AREA  0xFA //没有找到有效的显示区域；可以使用AddScreenProgramBmpTextArea添加区域信息。
        #define RETURN_ERROR_NOFIND_SCREENNO  0xFC //系统内没有查找到该显示屏；可以使用AddScreen函数添加显示屏
        #define RETURN_ERROR_NOW_SENDING  0xFD //系统内正在向该显示屏通讯，请稍后再通讯；
        #define RETURN_ERROR_OTHER  0xFF //其它错误；
        #define RETURN_NOERROR  0 //没有错误
        //==============================================================================

		#define SCREEN_NO  1
        #define SCREEN_WIDTH  192
        #define SCREEN_HEIGHT  96
        #define SCREEN_TYPE  2
        #define SCREEN_DATADA  0
        #define SCREEN_DATAOE  0
        #define SCREEN_COMM  "COM1"
        #define SCREEN_BAUD  57600
        #define SCREEN_ROWORDER  0
        #define SCREEN_FREQPAR  0
        #define SCREEN_SOCKETIP  "192.168.2.199"
        #define SCREEN_SOCKETPORT  5005
        //#define m_bSendBusy  false	此变量在数据更新中非常重要，请务必保留。
        #define FAddDynamicArea  "第二步-----添加动态区域"
        #define FAddDYAreaFile  "第三步-----动态区域文件属性"


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CVC_DemoDlg 对话框




CVC_DemoDlg::CVC_DemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVC_DemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	hInst = NULL;
	hInst = LoadLibrary(L"LedDynamicArea.dll");
	Initialize = (pInitialize)GetProcAddress(hInst,"Initialize");
	Uninitialize = (pUninitialize)GetProcAddress(hInst,"Uninitialize");
	AddScreen_Dynamic = (pAddScreen_Dynamic)GetProcAddress(hInst,"AddScreen_Dynamic");
	QuerryServerDeviceList = (pQuerryServerDeviceList)GetProcAddress(hInst,"QuerryServerDeviceList");
	BindServerDevice = (pBindServerDevice)GetProcAddress(hInst,"BindServerDevice");
	AddScreenDynamicArea = (pAddScreenDynamicArea)GetProcAddress(hInst,"AddScreenDynamicArea");
	AddScreenDynamicAreaFile = (pAddScreenDynamicAreaFile)GetProcAddress(hInst,"AddScreenDynamicAreaFile");
	AddScreenDynamicAreaText = (pAddScreenDynamicAreaText)GetProcAddress(hInst,"AddScreenDynamicAreaText");
	DeleteScreen_Dynamic = (pDeleteScreen_Dynamic)GetProcAddress(hInst,"DeleteScreen_Dynamic");
	DeleteScreenDynamicAreaFile = (pDeleteScreenDynamicAreaFile)GetProcAddress(hInst,"DeleteScreenDynamicAreaFile");
	SendDynamicAreaInfoCommand = (pSendDynamicAreaInfoCommand)GetProcAddress(hInst,"SendDynamicAreaInfoCommand");
	SendDeleteDynamicAreasCommand = (pSendDeleteDynamicAreasCommand)GetProcAddress(hInst,"SendDeleteDynamicAreasCommand");
}

void CVC_DemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO1, m_ctrlRadio1);
	DDX_Control(pDX, IDC_EDIT4, m_Edit4);
	DDX_Control(pDX, IDC_EDIT5, m_Edit5);
	DDX_Control(pDX, IDC_SPIN4, m_Spin4);
	DDX_Control(pDX, IDC_EDIT6, m_Edit6);
	DDX_Control(pDX, IDC_COMBO5, m_Combo5);
	DDX_Control(pDX, IDC_COMBO6, m_Combo6);
	DDX_Control(pDX, IDC_EDIT7, m_Edit7);
	DDX_Control(pDX, IDC_EDIT8, m_Edit8);
	DDX_Control(pDX, IDC_SPIN5, m_Spin5);
	DDX_Control(pDX, IDC_EDIT9, m_Edit9);
	DDX_Control(pDX, IDC_EDIT10, m_Edit10);
	DDX_Control(pDX, IDC_EDIT11, m_Edit11);
	DDX_Control(pDX, IDC_SPIN1, m_Spin1);
	DDX_Control(pDX, IDC_COMBO1, m_Combo1);
	DDX_Control(pDX, IDC_COMBO2, m_Combo2);
	DDX_Control(pDX, IDC_SPIN2, m_Spin2);
	DDX_Control(pDX, IDC_SPIN3, m_Spin3);
	DDX_Control(pDX, IDC_COMBO3, m_Combo3);
	DDX_Control(pDX, IDC_COMBO4, m_Combo4);
	DDX_Control(pDX, IDC_RICHEDIT21, m_RichEdit);
	DDX_Control(pDX, IDC_SPIN6, m_Spin6);
	DDX_Control(pDX, IDC_SPIN7, m_Spin7);
	DDX_Control(pDX, IDC_SPIN8, m_Spin8);
	DDX_Control(pDX, IDC_SPIN9, m_Spin9);
	DDX_Control(pDX, IDC_SPIN10, m_Spin10);
	DDX_Control(pDX, IDC_COMBO7, m_Combo7);
	DDX_Control(pDX, IDC_SPIN11, m_Spin11);
	DDX_Control(pDX, IDC_COMBO8, m_Combo8);
	DDX_Control(pDX, IDC_RADIO3, m_Radio3);
	DDX_Control(pDX, IDC_RADIO4, m_Radio4);
	DDX_Control(pDX, IDC_SPIN12, m_Spin12);
	DDX_Control(pDX, IDC_SPIN13, m_Spin13);
	DDX_Control(pDX, IDC_SPIN14, m_Spin14);
	DDX_Control(pDX, IDC_COMBO9, m_Combo9);
	DDX_Control(pDX, IDC_SPIN15, m_Spin15);
	DDX_Control(pDX, IDC_SPIN16, m_Spin16);
	DDX_Control(pDX, IDC_RADIO6, m_Radio6);
	DDX_Control(pDX, IDC_RADIO5, m_Radio5);
	DDX_Control(pDX, IDC_EDIT23, m_Edit23);
	DDX_Control(pDX, IDC_CHECK2, m_Check2);
	DDX_Control(pDX, IDC_COMBO10, m_Combo10);
	DDX_Control(pDX, IDC_SPIN17, m_Spin17);
	DDX_Control(pDX, IDC_SPIN18, m_Spin18);
	DDX_Control(pDX, IDC_CHECK3, m_Check3);
	DDX_Control(pDX, IDC_LIST1, m_ListBox1);
	DDX_Control(pDX, IDC_EDIT1, m_Edit1);
	DDX_Control(pDX, IDC_EDIT12, m_Edit12);
}

BEGIN_MESSAGE_MAP(CVC_DemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_COMBO2, &CVC_DemoDlg::OnCbnSelchangeCombo2)
	ON_BN_CLICKED(IDC_RADIO1, &CVC_DemoDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CVC_DemoDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_BUTTON4, &CVC_DemoDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON3, &CVC_DemoDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CVC_DemoDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CVC_DemoDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_RADIO3, &CVC_DemoDlg::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &CVC_DemoDlg::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_BUTTON5, &CVC_DemoDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CVC_DemoDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CVC_DemoDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON10, &CVC_DemoDlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON9, &CVC_DemoDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON8, &CVC_DemoDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON11, &CVC_DemoDlg::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &CVC_DemoDlg::OnBnClickedButton12)
	ON_BN_CLICKED(IDC_BUTTON13, &CVC_DemoDlg::OnBnClickedButton13)
END_MESSAGE_MAP()


// CVC_DemoDlg 消息处理程序

BOOL CVC_DemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	//初始化显示屏初始化GroupBox
	m_Spin1.SetRange(1,255);
	m_Spin1.SetPos(1);
	m_Combo1.InsertString(0,L"BX-5E1");
	m_Combo1.InsertString(1,L"BX-5E2");
	m_Combo1.InsertString(2,L"BX-5E3");
	m_Combo1.InsertString(3,L"BX-5Q1+");
	m_Combo1.InsertString(4,L"BX-5Q2+");
	m_Combo1.InsertString(5,L"BX-5Q3+");
	m_Combo1.SetCurSel(0);
	m_Combo2.InsertString(0,L"串口通讯");
	m_Combo2.InsertString(1,L"网络通讯");
	m_Combo2.InsertString(2,L"ONBON服务器-GPRS");
	m_Combo2.InsertString(3,L"ONBON服务器-3G");
	m_Combo2.InsertString(4,L"保存到文件");
	m_Combo2.SetCurSel(1);
	m_Spin2.SetRange(0,4096);
	m_Spin2.SetPos(80);
	m_Spin3.SetRange(0,512);
	m_Spin3.SetPos(16);
	m_Combo3.InsertString(0,L"单基色");
	m_Combo3.InsertString(1,L"双基色");
	m_Combo3.InsertString(2,L"全彩色");
	m_Combo3.SetCurSel(0);
	m_Combo4.InsertString(0,L"I  (R+G)");
	m_Combo4.InsertString(0,L"II (G+R)");
	m_Combo4.SetCurSel(0);

	//设置通讯模式下组件的显示或隐藏
	GetDlgItem(IDC_GrpNet)->ShowWindow(TRUE);
	m_ctrlRadio1.ShowWindow(TRUE);
	GetDlgItem(IDC_RADIO2)->ShowWindow(TRUE);
	GetDlgItem(IDC_textNetIP)->ShowWindow(TRUE);
	m_Edit4.ShowWindow(TRUE);
	GetDlgItem(IDC_textNetPort)->ShowWindow(TRUE);
	m_Edit5.ShowWindow(TRUE);
	m_Spin4.ShowWindow(TRUE);
	GetDlgItem(IDC_textNetID)->ShowWindow(FALSE);
	m_Edit6.ShowWindow(FALSE);

	GetDlgItem(IDC_GrpSp)->ShowWindow(FALSE);
	GetDlgItem(IDC_textSp)->ShowWindow(FALSE);
	m_Combo5.ShowWindow(FALSE);
	GetDlgItem(IDC_textBaud)->ShowWindow(FALSE);
	m_Combo6.ShowWindow(FALSE);

	GetDlgItem(IDC_GrpServer)->ShowWindow(FALSE);
	GetDlgItem(IDC_textServerIP)->ShowWindow(FALSE);
	m_Edit7.ShowWindow(FALSE);
	GetDlgItem(IDC_textServerPort)->ShowWindow(FALSE);
	m_Edit8.ShowWindow(FALSE);
	m_Spin5.ShowWindow(FALSE);
	GetDlgItem(IDC_textServerUser)->ShowWindow(FALSE);
	m_Edit9.ShowWindow(FALSE);
	GetDlgItem(IDC_textServerPwd)->ShowWindow(FALSE);
	m_Edit10.ShowWindow(FALSE);

	GetDlgItem(IDC_GrpSave)->ShowWindow(FALSE);
	GetDlgItem(IDC_textSave)->ShowWindow(FALSE);
	m_Edit11.ShowWindow(FALSE);

	GetDlgItem(IDC_BUTTON1)->ShowWindow(FALSE);
	GetDlgItem(IDC_BUTTON2)->ShowWindow(FALSE);
	GetDlgItem(IDC_BUTTON3)->ShowWindow(TRUE);
	GetDlgItem(IDC_BUTTON4)->ShowWindow(TRUE);

	//通讯模式关联GroupBox相关控件初始化
	m_Combo5.InsertString(0,L"COM1");
	m_Combo5.InsertString(1,L"COM2");
	m_Combo5.InsertString(2,L"COM3");
	m_Combo5.InsertString(3,L"COM4");
	m_Combo5.InsertString(4,L"COM5");
	m_Combo5.InsertString(5,L"COM6");
	m_Combo5.InsertString(6,L"COM7");
	m_Combo5.InsertString(7,L"COM8");
	m_Combo5.InsertString(8,L"COM9");
	m_Combo5.InsertString(9,L"COM10");
	m_Combo5.SetCurSel(0);
	m_Combo6.InsertString(0,L"9600");
	m_Combo6.InsertString(1,L"57600");
	m_Combo6.SetCurSel(1);
	m_ctrlRadio1.SetCheck(TRUE);
	m_Edit4.SetWindowText(L"192.168.0.15");
	m_Spin4.SetRange(1000,9999);
	m_Spin4.SetPos(5005);
	m_Spin5.SetRange(1000,9999);
	m_Spin5.SetPos(5005);
	m_Edit11.SetWindowText(L"curCommandData.dat");

	//第二步添加动态区域相关控件初始化
	m_Spin6.SetRange(0,3);
	m_Spin6.SetPos(0);
	m_Spin7.SetRange(0,8192);
	m_Spin7.SetPos(0);
	m_Spin8.SetRange(0,512);
	m_Spin8.SetPos(0);
	m_Spin9.SetRange(8,8192);
	m_Spin9.SetPos(8);
	m_Spin10.SetRange(8,512);
	m_Spin10.SetPos(8);
	m_Combo7.InsertString(0,L"动态区数据循环显示");
	m_Combo7.InsertString(1,L"显示完成后静止显示最后一页");
	m_Combo7.InsertString(2,L"超时后未更新删除动态区");
	m_Combo7.InsertString(3,L"顺序显示完成最后页后就不再显示 ");
	m_Combo7.SetCurSel(0);
	m_Spin11.SetRange32(1,65535);
	m_Spin11.SetPos(1);
	m_Combo8.InsertString(0,L"与节目一起播放(动态区关联异步节目才有效)");
	m_Combo8.InsertString(1,L"节目停止播放，仅播放该动态区域");
	m_Combo8.SetCurSel(0);
	m_Radio3.SetCheck(TRUE);
	m_Spin12.SetRange(0,23);
	m_Spin12.SetPos(0);
	m_Spin13.SetRange32(0,65535);
	m_Spin13.SetPos(255);
	m_Spin14.SetRange(0,46);
	m_Spin14.SetPos(0);
	m_Combo9.InsertString(0,L"0-闪烁 ");
	m_Combo9.InsertString(1,L"1 –顺时针转动 ");
	m_Combo9.InsertString(2,L"2 –逆时针转动 ");
	m_Combo9.InsertString(3,L"3 –闪烁加顺时针转动");
	m_Combo9.InsertString(4,L"4 –闪烁加逆时针转动");
	m_Combo9.InsertString(5,L"5 –红绿交替闪烁");
	m_Combo9.InsertString(6,L"6 –红绿交替转动");
	m_Combo9.InsertString(7,L"7 –静止打出");
	m_Combo9.SetCurSel(1);
	m_Spin15.SetRange32(0,65535);
	m_Spin15.SetPos(0);
	m_Spin16.SetRange(1,32);
	m_Spin16.SetPos(1);
	m_Radio6.SetCheck(TRUE);
	GetCurGrpCaption();

	//第三步动态区域文件属性相关控件初始化
	m_Combo10.InsertString(0,L"0 –随机显示");
	m_Combo10.InsertString(1,L"1 –静止显示");
	m_Combo10.InsertString(2,L"2 –快速打出");
	m_Combo10.InsertString(3,L"3 –向左移动");
	m_Combo10.InsertString(4,L"4 –向左连移");
	m_Combo10.InsertString(5,L"5 –向上移动");
	m_Combo10.InsertString(6,L"6 –向上连移");
	m_Combo10.InsertString(7,L"7 –闪烁");
	m_Combo10.InsertString(8,L"8 –飘雪");
	m_Combo10.InsertString(9,L"9 –冒泡");
	m_Combo10.InsertString(10,L"10 –中间移出");
	m_Combo10.InsertString(11,L"11 –左右移入");
	m_Combo10.InsertString(12,L"12 –左右交叉移入");
	m_Combo10.InsertString(13,L"13 –上下交叉移入");
	m_Combo10.InsertString(14,L"14 –画卷闭合");
	m_Combo10.InsertString(15,L"15 –画卷打开");
	m_Combo10.InsertString(16,L"16 –向左拉伸");
	m_Combo10.InsertString(17,L"17 –向右拉伸");
	m_Combo10.InsertString(18,L"18 –向上拉伸");
	m_Combo10.InsertString(19,L"19 –向下拉伸");
	m_Combo10.InsertString(20,L"20 –向左镭射");
	m_Combo10.InsertString(21,L"21 –向右镭射");
	m_Combo10.InsertString(22,L"22 –向上镭射");
	m_Combo10.InsertString(23,L"23 –向下镭射");
	m_Combo10.InsertString(24,L"24 –左右交叉拉幕");
	m_Combo10.InsertString(25,L"25 –上下交叉拉幕");
	m_Combo10.InsertString(26,L"26 –分散左拉");
	m_Combo10.InsertString(27,L"27 –水平百页");
	m_Combo10.InsertString(28,L"28 –垂直百页");
	m_Combo10.InsertString(29,L"29 –向左拉幕");
	m_Combo10.InsertString(30,L"30 –向右拉幕");
	m_Combo10.InsertString(31,L"31 –向上拉幕");
	m_Combo10.InsertString(32,L"32 –向下拉幕");
	m_Combo10.InsertString(33,L"33 –左右闭合");
	m_Combo10.InsertString(34,L"34 –左右对开");
	m_Combo10.InsertString(35,L"35 –上下闭合");
	m_Combo10.InsertString(36,L"36 –上下对开");
	m_Combo10.InsertString(37,L"37 –向右移动");
	m_Combo10.InsertString(38,L"38 –向右连移");
	m_Combo10.InsertString(39,L"39 –向下移动");
	m_Combo10.InsertString(40,L"40 –向下连移");
	m_Combo10.SetCurSel(2);
	m_Spin17.SetRange(0,63);
	m_Spin17.SetPos(8);
	m_Spin18.SetRange32(0,65535);
	m_Spin18.SetPos(5);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CVC_DemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CVC_DemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CVC_DemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CVC_DemoDlg::OnCbnSelchangeCombo2()	//通讯模式combobox消息处理函数
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_Combo2.GetCurSel() == 1)
	{
		g_nSendMode = SEND_MODE_NETWORK;	//网络通讯

		GetDlgItem(IDC_GrpNet)->ShowWindow(TRUE);
		m_ctrlRadio1.ShowWindow(TRUE);
		GetDlgItem(IDC_RADIO2)->ShowWindow(TRUE);
		GetDlgItem(IDC_textNetIP)->ShowWindow(TRUE);
		m_Edit4.ShowWindow(TRUE);
		GetDlgItem(IDC_textNetPort)->ShowWindow(TRUE);
		m_Edit5.ShowWindow(TRUE);
		m_Spin4.ShowWindow(TRUE);
		GetDlgItem(IDC_textNetID)->ShowWindow(FALSE);
		m_Edit6.ShowWindow(FALSE);

		GetDlgItem(IDC_GrpSp)->ShowWindow(FALSE);
		GetDlgItem(IDC_textSp)->ShowWindow(FALSE);
		m_Combo5.ShowWindow(FALSE);
		GetDlgItem(IDC_textBaud)->ShowWindow(FALSE);
		m_Combo6.ShowWindow(FALSE);

		GetDlgItem(IDC_GrpServer)->ShowWindow(FALSE);
		GetDlgItem(IDC_textServerIP)->ShowWindow(FALSE);
		m_Edit7.ShowWindow(FALSE);
		GetDlgItem(IDC_textServerPort)->ShowWindow(FALSE);
		m_Edit8.ShowWindow(FALSE);
		m_Spin5.ShowWindow(FALSE);
		GetDlgItem(IDC_textServerUser)->ShowWindow(FALSE);
		m_Edit9.ShowWindow(FALSE);
		GetDlgItem(IDC_textServerPwd)->ShowWindow(FALSE);
		m_Edit10.ShowWindow(FALSE);

		GetDlgItem(IDC_GrpSave)->ShowWindow(FALSE);
		GetDlgItem(IDC_textSave)->ShowWindow(FALSE);
		m_Edit11.ShowWindow(FALSE);

		GetDlgItem(IDC_BUTTON1)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON2)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON3)->ShowWindow(TRUE);
		GetDlgItem(IDC_BUTTON4)->ShowWindow(TRUE);
	}
	else if(m_Combo2.GetCurSel() == 2)
	{
		g_nSendMode = SEND_MODE_Server_2G;	//ONBON服务器-GPRS

		GetDlgItem(IDC_GrpNet)->ShowWindow(FALSE);
		m_ctrlRadio1.ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO2)->ShowWindow(FALSE);
		GetDlgItem(IDC_textNetIP)->ShowWindow(FALSE);
		m_Edit4.ShowWindow(FALSE);
		GetDlgItem(IDC_textNetPort)->ShowWindow(FALSE);
		m_Edit5.ShowWindow(FALSE);
		m_Spin4.ShowWindow(FALSE);
		GetDlgItem(IDC_textNetID)->ShowWindow(FALSE);
		m_Edit6.ShowWindow(FALSE);

		GetDlgItem(IDC_GrpSp)->ShowWindow(FALSE);
		GetDlgItem(IDC_textSp)->ShowWindow(FALSE);
		m_Combo5.ShowWindow(FALSE);
		GetDlgItem(IDC_textBaud)->ShowWindow(FALSE);
		m_Combo6.ShowWindow(FALSE);

		GetDlgItem(IDC_GrpServer)->ShowWindow(TRUE);
		GetDlgItem(IDC_textServerIP)->ShowWindow(TRUE);
		m_Edit7.ShowWindow(TRUE);
		GetDlgItem(IDC_textServerPort)->ShowWindow(TRUE);
		m_Edit8.ShowWindow(TRUE);
		m_Spin5.ShowWindow(TRUE);
		GetDlgItem(IDC_textServerUser)->ShowWindow(TRUE);
		m_Edit9.ShowWindow(TRUE);
		GetDlgItem(IDC_textServerPwd)->ShowWindow(TRUE);
		m_Edit10.ShowWindow(TRUE);

		GetDlgItem(IDC_GrpSave)->ShowWindow(FALSE);
		GetDlgItem(IDC_textSave)->ShowWindow(FALSE);
		m_Edit11.ShowWindow(FALSE);

		GetDlgItem(IDC_BUTTON1)->ShowWindow(TRUE);
		GetDlgItem(IDC_BUTTON2)->ShowWindow(TRUE);
		GetDlgItem(IDC_BUTTON3)->ShowWindow(TRUE);
		GetDlgItem(IDC_BUTTON4)->ShowWindow(TRUE);
	}
	else if(m_Combo2.GetCurSel() == 3)
	{
		g_nSendMode = SEND_MODE_Server_3G;	//ONBON服务器-3G

		GetDlgItem(IDC_GrpNet)->ShowWindow(FALSE);
		m_ctrlRadio1.ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO2)->ShowWindow(FALSE);
		GetDlgItem(IDC_textNetIP)->ShowWindow(FALSE);
		m_Edit4.ShowWindow(FALSE);
		GetDlgItem(IDC_textNetPort)->ShowWindow(FALSE);
		m_Edit5.ShowWindow(FALSE);
		m_Spin4.ShowWindow(FALSE);
		GetDlgItem(IDC_textNetID)->ShowWindow(FALSE);
		m_Edit6.ShowWindow(FALSE);

		GetDlgItem(IDC_GrpSp)->ShowWindow(FALSE);
		GetDlgItem(IDC_textSp)->ShowWindow(FALSE);
		m_Combo5.ShowWindow(FALSE);
		GetDlgItem(IDC_textBaud)->ShowWindow(FALSE);
		m_Combo6.ShowWindow(FALSE);

		GetDlgItem(IDC_GrpServer)->ShowWindow(TRUE);
		GetDlgItem(IDC_textServerIP)->ShowWindow(TRUE);
		m_Edit7.ShowWindow(TRUE);
		GetDlgItem(IDC_textServerPort)->ShowWindow(TRUE);
		m_Edit8.ShowWindow(TRUE);
		m_Spin5.ShowWindow(TRUE);
		GetDlgItem(IDC_textServerUser)->ShowWindow(TRUE);
		m_Edit9.ShowWindow(TRUE);
		GetDlgItem(IDC_textServerPwd)->ShowWindow(TRUE);
		m_Edit10.ShowWindow(TRUE);

		GetDlgItem(IDC_GrpSave)->ShowWindow(FALSE);
		GetDlgItem(IDC_textSave)->ShowWindow(FALSE);
		m_Edit11.ShowWindow(FALSE);

		GetDlgItem(IDC_BUTTON1)->ShowWindow(TRUE);
		GetDlgItem(IDC_BUTTON2)->ShowWindow(TRUE);
		GetDlgItem(IDC_BUTTON3)->ShowWindow(TRUE);
		GetDlgItem(IDC_BUTTON4)->ShowWindow(TRUE);
	}
	else if(m_Combo2.GetCurSel() == 4)
	{
		g_nSendMode = SEND_MODE_SAVEFILE;	//保存到文件

		GetDlgItem(IDC_GrpNet)->ShowWindow(FALSE);
		m_ctrlRadio1.ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO2)->ShowWindow(FALSE);
		GetDlgItem(IDC_textNetIP)->ShowWindow(FALSE);
		m_Edit4.ShowWindow(FALSE);
		GetDlgItem(IDC_textNetPort)->ShowWindow(FALSE);
		m_Edit5.ShowWindow(FALSE);
		m_Spin4.ShowWindow(FALSE);
		GetDlgItem(IDC_textNetID)->ShowWindow(FALSE);
		m_Edit6.ShowWindow(FALSE);

		GetDlgItem(IDC_GrpSp)->ShowWindow(FALSE);
		GetDlgItem(IDC_textSp)->ShowWindow(FALSE);
		m_Combo5.ShowWindow(FALSE);
		GetDlgItem(IDC_textBaud)->ShowWindow(FALSE);
		m_Combo6.ShowWindow(FALSE);

		GetDlgItem(IDC_GrpServer)->ShowWindow(FALSE);
		GetDlgItem(IDC_textServerIP)->ShowWindow(FALSE);
		m_Edit7.ShowWindow(FALSE);
		GetDlgItem(IDC_textServerPort)->ShowWindow(FALSE);
		m_Edit8.ShowWindow(FALSE);
		m_Spin5.ShowWindow(FALSE);
		GetDlgItem(IDC_textServerUser)->ShowWindow(FALSE);
		m_Edit9.ShowWindow(FALSE);
		GetDlgItem(IDC_textServerPwd)->ShowWindow(FALSE);
		m_Edit10.ShowWindow(FALSE);

		GetDlgItem(IDC_GrpSave)->ShowWindow(TRUE);
		GetDlgItem(IDC_textSave)->ShowWindow(TRUE);
		m_Edit11.ShowWindow(TRUE);

		GetDlgItem(IDC_BUTTON1)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON2)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON3)->ShowWindow(TRUE);
		GetDlgItem(IDC_BUTTON4)->ShowWindow(TRUE);
	}
	else
	{
		g_nSendMode = SEND_MODE_SERIALPORT;	//串口通讯

		GetDlgItem(IDC_GrpNet)->ShowWindow(FALSE);
		m_ctrlRadio1.ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO2)->ShowWindow(FALSE);
		GetDlgItem(IDC_textNetIP)->ShowWindow(FALSE);
		m_Edit4.ShowWindow(FALSE);
		GetDlgItem(IDC_textNetPort)->ShowWindow(FALSE);
		m_Edit5.ShowWindow(FALSE);
		m_Spin4.ShowWindow(FALSE);
		GetDlgItem(IDC_textNetID)->ShowWindow(FALSE);
		m_Edit6.ShowWindow(FALSE);

		GetDlgItem(IDC_GrpSp)->ShowWindow(TRUE);
		GetDlgItem(IDC_textSp)->ShowWindow(TRUE);
		m_Combo5.ShowWindow(TRUE);
		GetDlgItem(IDC_textBaud)->ShowWindow(TRUE);
		m_Combo6.ShowWindow(TRUE);

		GetDlgItem(IDC_GrpServer)->ShowWindow(FALSE);
		GetDlgItem(IDC_textServerIP)->ShowWindow(FALSE);
		m_Edit7.ShowWindow(FALSE);
		GetDlgItem(IDC_textServerPort)->ShowWindow(FALSE);
		m_Edit8.ShowWindow(FALSE);
		m_Spin5.ShowWindow(FALSE);
		GetDlgItem(IDC_textServerUser)->ShowWindow(FALSE);
		m_Edit9.ShowWindow(FALSE);
		GetDlgItem(IDC_textServerPwd)->ShowWindow(FALSE);
		m_Edit10.ShowWindow(FALSE);

		GetDlgItem(IDC_GrpSave)->ShowWindow(FALSE);
		GetDlgItem(IDC_textSave)->ShowWindow(FALSE);
		m_Edit11.ShowWindow(FALSE);

		GetDlgItem(IDC_BUTTON1)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON2)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON3)->ShowWindow(TRUE);
		GetDlgItem(IDC_BUTTON4)->ShowWindow(TRUE);
	}
}


void CVC_DemoDlg::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_textNetID)->ShowWindow(FALSE);
	m_Edit6.ShowWindow(FALSE);
}


void CVC_DemoDlg::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_textNetID)->ShowWindow(TRUE);
	m_Edit6.ShowWindow(TRUE);
}

void CVC_DemoDlg::OnBnClickedRadio3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Radio4.SetCheck(FALSE);
}


void CVC_DemoDlg::OnBnClickedRadio4()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Radio3.SetCheck(FALSE);
}

void CVC_DemoDlg::GetCurGrpCaption()
{
	CString tmp1(FAddDynamicArea);
	CString tmp2(_T("(屏号："));
	CString tmp3;
	m_Edit1.GetWindowText(tmp3);
	CString tmp4(_T("）"));
	SetDlgItemText(grp_AddDynamicArea,tmp1+tmp2+tmp3+tmp4);

	CString Tmp1(FAddDYAreaFile);
	CString Tmp2(_T("；动态区域编号："));
	CString Tmp3;
	m_Edit12.GetWindowText(Tmp3);
	SetDlgItemText(grp_AddDYAreaFile,Tmp1+tmp2+tmp3+Tmp2+Tmp3+tmp4);
}

void CVC_DemoDlg::GetErrorMessage(char* szfunctionName, int nResult)	//函数执行结果汉化
{
	CString szResult;
	CTime time = CTime::GetCurrentTime(); ///构造CTime对象
	CString m_strTime = time.Format("%Y-%m-%d %H:%M:%S");
	m_RichEdit.ReplaceSel(m_strTime);
	m_RichEdit.ReplaceSel(L"---执行函数：");
	CString functionName(szfunctionName);
	m_RichEdit.ReplaceSel(functionName);
	m_RichEdit.ReplaceSel(L"---返回结果：");

	CString returnResult;
	switch (nResult)
            {
                case RETURN_ERROR_NOFIND_DYNAMIC_AREA:
                    returnResult = "没有找到有效的动态区域。\r\n";
                    break;
                case RETURN_ERROR_NOFIND_DYNAMIC_AREA_FILE_ORD:
                    returnResult = "在指定的动态区域没有找到指定的文件序号。\r\n";
                    break;
                case RETURN_ERROR_NOFIND_DYNAMIC_AREA_PAGE_ORD:
                    returnResult = "在指定的动态区域没有找到指定的页序号。\r\n";
                    break;
                case RETURN_ERROR_NOSUPPORT_FILETYPE:
                    returnResult = "动态库不支持该文件类型。\r\n";
                    break;
                case RETURN_ERROR_RA_SCREENNO:
                    returnResult = "系统中已经有该显示屏信息。如要重新设定请先执行DeleteScreen函数删除该显示屏后再添加。\r\n";
                    break;
                case RETURN_ERROR_NOFIND_AREA:
                    returnResult = "系统中没有找到有效的动态区域；可以先执行AddScreenDynamicArea函数添加动态区域信息后再添加。\r\n";
                    break;
                case RETURN_ERROR_NOFIND_SCREENNO:
                    returnResult = "系统内没有查找到该显示屏；可以使用AddScreen函数添加该显示屏。\r\n";
                    break;
                case RETURN_ERROR_NOW_SENDING:
                    returnResult = "系统内正在向该显示屏通讯，请稍后再通讯。\r\n";
                    break;
                case RETURN_ERROR_OTHER:
                    returnResult = "其它错误。\r\n";
                    break;
                case RETURN_NOERROR:
                    returnResult = "函数执行成功。\r\n";
                    break;
            }
	m_RichEdit.ReplaceSel(returnResult);
}


void CVC_DemoDlg::OnBnClickedButton4()	//添加显示屏
{
	// TODO: 在此添加控件通知处理程序代码
	int nSendMode;
	int nScreenType;
	CString strCom;
	CString strBaud;
	CString strSocketIP;
	CString strNetWorkID;
	CString strServerIP;
	CString strUserName;
	CString strUserPassword;
	CString strSaveFile;
	int nServerMode;
	int nResult;

	switch(m_Combo2.GetCurSel())
	{
		case 0:
			nSendMode = SEND_MODE_SERIALPORT;
			break;
		case 1:
			nSendMode = SEND_MODE_NETWORK;
			break;
		case 2:
			nSendMode = SEND_MODE_Server_2G;
			break;
		case 3:
			nSendMode = SEND_MODE_Server_3G;
			break;
		case 4:
			nSendMode = SEND_MODE_SAVEFILE;
			break;
		default:
			nSendMode = SEND_MODE_NETWORK;
			break;
	}

	switch(m_Combo3.GetCurSel())
	{
		case 0:
			nScreenType = 1;
            break;
        case 1:
            nScreenType = 2;
            break;
        case 2:
            nScreenType = 4;
            break;
        default:
            nScreenType = 2;
            break;
	}

	m_Combo5.GetWindowText(strCom);
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP,NULL,strCom,-1,NULL,NULL,0,NULL);
	char *Com = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP,NULL,strCom,-1,Com,dwNum,0,NULL);

	m_Combo6.GetWindowText(strBaud);

	m_Edit4.GetWindowText(strSocketIP);
	dwNum = WideCharToMultiByte(CP_OEMCP,NULL,strSocketIP,-1,NULL,NULL,0,NULL);
	char *SocketIP = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP,NULL,strSocketIP,-1,SocketIP,dwNum,0,NULL);

	char *Barcode = "";

	m_Edit6.GetWindowText(strNetWorkID);
	dwNum = WideCharToMultiByte(CP_OEMCP,NULL,strSocketIP,-1,NULL,NULL,0,NULL);
	char *NetWorkID = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP,NULL,strNetWorkID,-1,NetWorkID,dwNum,0,NULL);

	m_Edit7.GetWindowText(strServerIP);
	dwNum = WideCharToMultiByte(CP_OEMCP,NULL,strSocketIP,-1,NULL,NULL,0,NULL);
	char *ServerIP = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP,NULL,strServerIP,-1,ServerIP,dwNum,0,NULL);

	m_Edit9.GetWindowText(strUserName);
	dwNum = WideCharToMultiByte(CP_OEMCP,NULL,strSocketIP,-1,NULL,NULL,0,NULL);
	char *UserName = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP,NULL,strUserName,-1,UserName,dwNum,0,NULL);

	m_Edit10.GetWindowText(strUserPassword);
	dwNum = WideCharToMultiByte(CP_OEMCP,NULL,strSocketIP,-1,NULL,NULL,0,NULL);
	char *UserPassword = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP,NULL,strUserPassword,-1,UserPassword,dwNum,0,NULL);

	m_Edit10.GetWindowText(strSaveFile);
	dwNum = WideCharToMultiByte(CP_OEMCP,NULL,strSocketIP,-1,NULL,NULL,0,NULL);
	char *SaveFile = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP,NULL,strSaveFile,-1,SaveFile,dwNum,0,NULL);

	if(m_ctrlRadio1.GetCheck() == 1)
	{
		nServerMode = 0;
	}
	else
	{
		nServerMode = 1;
	}

	nResult = AddScreen_Dynamic(controlType[m_Combo1.GetCurSel()], m_Spin1.GetPos(), nSendMode, m_Spin2.GetPos()
                , m_Spin3.GetPos(), nScreenType, m_Combo4.GetCurSel() + 1
                , Com, _ttoi(strBaud)
                , SocketIP, m_Spin4.GetPos(), nServerMode, Barcode, NetWorkID
                , ServerIP, m_Spin5.GetPos(), UserName, UserPassword
                , SaveFile);

	GetErrorMessage("执行AddScreen_Dynamic函数,", nResult);
	GetCurGrpCaption();
}


void CVC_DemoDlg::OnBnClickedButton3()	//删除显示屏
{
	// TODO: 在此添加控件通知处理程序代码
	int nResult = DeleteScreen_Dynamic(m_Spin1.GetPos());
    GetErrorMessage("执行DeleteScreen函数, ", nResult);
}


void CVC_DemoDlg::OnBnClickedButton2()	//查询设备列表
{
	// TODO: 在此添加控件通知处理程序代码
	int nResult;
    int DeviceCount = 0;
    byte DeviceList[100];
    nResult = QuerryServerDeviceList(m_Spin1.GetPos(), DeviceList, DeviceCount);
	GetErrorMessage("执行QuerryServerDeviceList函数, ", nResult);
}


void CVC_DemoDlg::OnBnClickedButton1()	//绑定设备
{
	// TODO: 在此添加控件通知处理程序代码
	int nResult;
    char* sBarcode = "";
    char* sNetworkID = "BX-NET000001";
    nResult = BindServerDevice(m_Spin1.GetPos(), sBarcode, sNetworkID);
	GetErrorMessage("执行BindServerDevice函数, ", nResult);
}

void CVC_DemoDlg::OnBnClickedButton5()	//添加屏幕动态区域
{
	// TODO: 在此添加控件通知处理程序代码
	int nRunMode;
	CString strTmp;
	char* szProRelateList = "";
    int nAllProRelate = 1;
	int nPlayPriority;
	int nAreaFMode;
	int nAreaFLine;
	int nResult;

	switch(m_Combo7.GetCurSel())
	{
		case 1:
			nRunMode = RUN_MODE_SHOW_LAST_PAGE;
			break;
		case 2:
			nRunMode = RUN_MODE_SHOW_CYCLE_WAITOUT_NOSHOW;
			break;
		case 3:
			nRunMode = RUN_MODE_SHOW_ORDERPLAYED_NOSHOW;
			break;
		default:
			nRunMode = RUN_MODE_CYCLE_SHOW;
			break;
	}

	if (m_Radio6.GetCheck() == 1)
    {
		nAllProRelate = 1;
		szProRelateList = "";
    }
	else
	{
		nAllProRelate = 0;
		if(m_Radio5.GetCheck() == 1)
		{
			szProRelateList = "";
		}
		else
		{
			m_Edit23.GetWindowTextW(strTmp);
			DWORD dwNum = WideCharToMultiByte(CP_OEMCP,NULL,strTmp,-1,NULL,NULL,0,NULL);
			//char *Com = new char[dwNum];
			WideCharToMultiByte(CP_OEMCP,NULL,strTmp,-1,szProRelateList,dwNum,0,NULL);
		}
	}

	nPlayPriority = m_Combo8.GetCurSel();

	if(m_Check2.GetCheck() == 1)
	{
		if(m_Radio3.GetCheck() == 1)
		{
			nAreaFMode = 0;
			nAreaFLine = m_Spin12.GetPos();
		}
		else
		{
			nAreaFMode = 1;
			nAreaFLine = m_Spin14.GetPos();
		}
	}
	else
	{
		nAreaFMode = 255;
        nAreaFLine = 0;
	}

	nResult = AddScreenDynamicArea(m_Spin1.GetPos(), m_Spin6.GetPos(), nRunMode
            , m_Spin11.GetPos(), nAllProRelate, szProRelateList, nPlayPriority
            , m_Spin7.GetPos(), m_Spin8.GetPos(), m_Spin9.GetPos(), m_Spin10.GetPos()
            , nAreaFMode, nAreaFLine, m_Spin13.GetPos()
			, m_Combo9.GetCurSel(), m_Spin15.GetPos(), m_Spin16.GetPos());
    GetErrorMessage("执行AddScreenDynamicArea函数,", nResult);
	GetCurGrpCaption();
}


void CVC_DemoDlg::OnBnClickedButton6()	//添加文件
{
	// TODO: 在此添加控件通知处理程序代码
	int nSingleLine;
    int nResult;

	if(m_Check3.GetCheck() == 1)
	{
		nSingleLine = 1;
	}
	else
	{
		nSingleLine = 0;
	}

	CString strFilename;//保存路径		
	CFileDialog opendlg (TRUE,_T("*"),_T(""),OFN_OVERWRITEPROMPT, _T("所有文件(*.*;)|*.*||"),NULL);   	
	if (opendlg.DoModal()==IDOK)	
	{		
		strFilename=opendlg.GetPathName();
		m_ListBox1.AddString(strFilename);
		DWORD dwNum = WideCharToMultiByte(CP_OEMCP,NULL,strFilename,-1,NULL,NULL,0,NULL);
		char *filename = new char[dwNum];
		WideCharToMultiByte(CP_OEMCP,NULL,strFilename,-1,filename,dwNum,0,NULL);
		nResult = AddScreenDynamicAreaFile(m_Spin1.GetPos(), m_Spin6.GetPos()
                , filename, nSingleLine, "宋体", 12, 0, 255
				, m_Combo10.GetCurSel(), m_Spin17.GetPos(), m_Spin18.GetPos());
		GetErrorMessage("执行AddScreenDynamicAreaFile函数, ", nResult);
	} 
}


void CVC_DemoDlg::OnBnClickedButton7()	//删除文件
{
	// TODO: 在此添加控件通知处理程序代码
	int nFileOrd;
    int nIndex;
    int nResult;

	if(m_ListBox1.GetCount() >= 0)
	{
		nFileOrd = m_ListBox1.GetCurSel();
		nIndex = nFileOrd;
		nResult = DeleteScreenDynamicAreaFile(m_Spin1.GetPos(), m_Spin6.GetPos()
                , nFileOrd);
		nIndex = nIndex - 1;
		m_ListBox1.DeleteString(nFileOrd);
		GetErrorMessage("执行DeleteScreenDynamicAreaFile函数, ", nResult);
	}
}


void CVC_DemoDlg::OnBnClickedButton10()	//更新动态区域函数
{
	// TODO: 在此添加控件通知处理程序代码
	int nResult;
	nResult = SendDynamicAreaInfoCommand(m_Spin1.GetPos(), m_Spin6.GetPos());
    GetErrorMessage("执行SendDynamicAreaInfoCommand函数, ", nResult);
}


void CVC_DemoDlg::OnBnClickedButton9()	//删除当前动态区域
{
	// TODO: 在此添加控件通知处理程序代码
	int nResult;
    nResult = SendDeleteDynamicAreasCommand(m_Spin1.GetPos(), 1, "");
    GetErrorMessage("执行SendDeleteDynamicAreasCommand函数, ", nResult);
}


void CVC_DemoDlg::OnBnClickedButton8()	//删除所有动态区域
{
	// TODO: 在此添加控件通知处理程序代码
    int nResult;
	CString strDYAreaIDList;
	GetDlgItem(IDC_EDIT12)->GetWindowText(strDYAreaIDList);
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP,NULL,strDYAreaIDList,-1,NULL,NULL,0,NULL);
	char *szDYAreaIDList = new char[dwNum];	//如果删除多个动态区域，动态区域编号间用","隔开。
	WideCharToMultiByte(CP_OEMCP,NULL,strDYAreaIDList,-1,szDYAreaIDList,dwNum,0,NULL);
    nResult = SendDeleteDynamicAreasCommand(m_Spin1.GetPos(), 0
            , szDYAreaIDList);
    GetErrorMessage("执行SendDeleteDynamicAreasCommand函数, ", nResult);
}



void CVC_DemoDlg::OnBnClickedButton11()	//初始化动态库
{
	// TODO: 在此添加控件通知处理程序代码
	int nResult = Initialize();
    GetErrorMessage("执行Initialize函数, ", nResult);
}


void CVC_DemoDlg::OnBnClickedButton12()	//释放动态库
{
	// TODO: 在此添加控件通知处理程序代码
	int nResult = Uninitialize();
    GetErrorMessage("执行Uninitialize函数, ", nResult);
}


void CVC_DemoDlg::OnBnClickedButton13()	//添加文本
{
	// TODO: 在此添加控件通知处理程序代码
	int nSingleLine;
    int nResult;

	if(m_Check3.GetCheck() == 1)
	{
		nSingleLine = 1;
	}
	else
	{
		nSingleLine = 0;
	}

	nResult = AddScreenDynamicAreaText(m_Spin1.GetPos(), m_Spin6.GetPos()
                , "测试", nSingleLine, "宋体", 12, 0, 255
				, m_Combo10.GetCurSel(), m_Spin17.GetPos(), m_Spin18.GetPos());
	GetErrorMessage("执行AddScreenDynamicAreaText函数, ", nResult);
}
