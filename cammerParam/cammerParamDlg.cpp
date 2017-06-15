
// cammerParamDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "cammerParam.h"
#include "cammerParamDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif





HANDLE hComm; //句柄
OVERLAPPED m_ov; //用于异步输入输出的信息的结构体
COMSTAT comstat;  //结构体
DWORD m_dwCommEvents;

struct commParam
{
	int port;
	int baud;
};

struct commParam m_commParam;



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


// CcammerParamDlg 对话框

LRESULT  CcammerParamDlg::OnDisplayChange(WPARAM wParam, LPARAM lParam)
{
	//更新Edit  
	UpdateData(wParam);
	return 0;
}

CcammerParamDlg::CcammerParamDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CcammerParamDlg::IDD, pParent)
	, m_edit_fps(1)
	, m_edit_brightness(0)
	, m_edit_GAIN(0)
	, m_edit_sharpness(0)
	, m_edit_contrast(0)
	, m_edit_saturation(0)
	, m_edit_exposure_compensation(0)
	, m_radio_exposure_mode(0)
	, m_radio_metering(0)
	, m_edit_shutter(0)
	, m_edit_aver_gray(0)
	, m_check_enhance_image(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CcammerParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_FPS, m_edit_fps);
	DDV_MinMaxInt(pDX, m_edit_fps, 1, 30);
	DDX_Text(pDX, IDC_EDIT_BRIGHTNESS, m_edit_brightness);
	DDV_MinMaxInt(pDX, m_edit_brightness, 0, 100);
	DDX_Text(pDX, IDC_EDIT_GAIN, m_edit_GAIN);
	DDV_MinMaxInt(pDX, m_edit_GAIN, 0, 1000);
	DDX_Text(pDX, IDC_EDIT_SHARPNESS, m_edit_sharpness);
	DDV_MinMaxInt(pDX, m_edit_sharpness, 0, 200);
	DDX_Text(pDX, IDC_EDIT_CONTRAST, m_edit_contrast);
	DDV_MinMaxInt(pDX, m_edit_contrast, 0, 200);
	DDX_Text(pDX, IDC_EDIT_SATURATION, m_edit_saturation);
	DDV_MinMaxInt(pDX, m_edit_saturation, 0, 200);
	DDX_Text(pDX, IDC_EDIT_EXPOSURE_COMPENSATION, m_edit_exposure_compensation);
	DDV_MinMaxInt(pDX, m_edit_exposure_compensation, 0, 255);
	DDX_Control(pDX, IDC_SLIDER_FPS, m_slider_fps);
	DDX_Control(pDX, IDC_SLIDER_BRIGHTNESS, m_slider_brightness);
	DDX_Control(pDX, IDC_SLIDER_GAIN, m_slider_gain);
	DDX_Control(pDX, IDC_SLIDER_SHARPNESS, m_slider_sharpness);
	DDX_Control(pDX, IDC_SLIDER_CONTRAST, m_slider_contrast);
	DDX_Control(pDX, IDC_SLIDER_SATURATION, m_slider_saturation);
	DDX_Control(pDX, IDC_SLIDER_EXPOSURE_COMPENSATION, m_slider_exposure_compensation);
	DDX_Radio(pDX, IDC_RADIO1, m_radio_exposure_mode);
	DDX_Radio(pDX, IDC_RADIO8, m_radio_metering);
	DDX_Control(pDX, IDC_BUTTON1, m_button_adjust_param);
	DDX_Control(pDX, IDC_EDIT_STATE, m_comm_state);
	DDX_Text(pDX, IDC_EDIT_SHUTTER, m_edit_shutter);
	DDV_MinMaxInt(pDX, m_edit_shutter, 0, 330000);
	DDX_Control(pDX, IDC_SLIDER_SHUTTER, m_slider_shutter);
	DDX_Text(pDX, IDC_EDIT_AVE_GRAY, m_edit_aver_gray);
	DDX_Check(pDX, IDC_CHECK_ENHANCE, m_check_enhance_image);
}

BEGIN_MESSAGE_MAP(CcammerParamDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_EDIT_FPS, &CcammerParamDlg::OnEnChangeEditFps)
	ON_EN_CHANGE(IDC_EDIT_BRIGHTNESS, &CcammerParamDlg::OnEnChangeEditBrightness)
	ON_EN_CHANGE(IDC_EDIT_GAIN, &CcammerParamDlg::OnEnChangeEditGain)
	ON_EN_CHANGE(IDC_EDIT_SHARPNESS, &CcammerParamDlg::OnEnChangeEditSharpness)
	ON_EN_CHANGE(IDC_EDIT_CONTRAST, &CcammerParamDlg::OnEnChangeEditContrast)
	ON_EN_CHANGE(IDC_EDIT_SATURATION, &CcammerParamDlg::OnEnChangeEditSaturation)
	ON_EN_CHANGE(IDC_EDIT_EXPOSURE_COMPENSATION, &CcammerParamDlg::OnEnChangeEditExposureCompensation)
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(IDC_RADIO2, &CcammerParamDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO1, &CcammerParamDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO8, &CcammerParamDlg::OnBnClickedRadio8)
	ON_BN_CLICKED(IDC_RADIO10, &CcammerParamDlg::OnBnClickedRadio10)
	ON_BN_CLICKED(IDC_RADIO9, &CcammerParamDlg::OnBnClickedRadio9)
	ON_BN_CLICKED(IDC_RADIO11, &CcammerParamDlg::OnBnClickedRadio11)
	ON_BN_CLICKED(IDC_BUTTON1, &CcammerParamDlg::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT_SHUTTER, &CcammerParamDlg::OnEnChangeEditShutter)
	ON_MESSAGE(WM_DISPLAY_CHANGE, OnDisplayChange)

	ON_BN_CLICKED(IDC_CHECK_ENHANCE, &CcammerParamDlg::OnBnClickedCheckEnhance)
END_MESSAGE_MAP()


// CcammerParamDlg 消息处理程序

BOOL CcammerParamDlg::OnInitDialog()
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	initPhase();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CcammerParamDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CcammerParamDlg::OnPaint()
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
HCURSOR CcammerParamDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//打开串口
bool openport(char *portname)
{
	hComm = CreateFile(portname, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
	if (hComm == INVALID_HANDLE_VALUE)
		return FALSE;
	else
		return TRUE;

}


bool setupdcb(int rate_arg)
{

	DCB dcb;                             //在串口通信中都需要用到这个DCB，可以理解为开辟了一块内存块
	int rate = rate_arg;                   //设置波特率
	memset(&dcb, 0, sizeof(dcb));
	if (!GetCommState(hComm, &dcb))        //获取当前dcb配置
	{
		return FALSE;

	}
	dcb.DCBlength = sizeof(dcb);
	//串口配置
	dcb.BaudRate = rate;                  //波特率
	dcb.Parity = EVENPARITY;                //无检验
	dcb.fParity = 0;
	dcb.StopBits = ONESTOPBIT;            //1位停止位
	dcb.ByteSize = 8;
	dcb.fOutxCtsFlow = 0;
	dcb.fOutxDsrFlow = 0;
	dcb.fDtrControl = DTR_CONTROL_DISABLE;
	dcb.fDsrSensitivity = 0;
	dcb.fRtsControl = RTS_CONTROL_DISABLE;
	dcb.fOutX = 0;
	dcb.fInX = 0;

	//misc参数（我不知道这个什么意思!!=_=）
	dcb.fErrorChar = 0;
	dcb.fBinary = 1;
	dcb.fNull = 0;
	dcb.fAbortOnError = 0;
	dcb.wReserved = 0;
	dcb.XonLim = 2;
	dcb.XoffLim = 4;
	dcb.XonChar = 0x13;
	dcb.XoffChar = 0x19;
	dcb.EvtChar = 0;

	//设置DCB
	if (!SetCommState(hComm, &dcb))
	{
		return false;
	}
	else
		return true;

}



//设置前后2个数据之间的发送时间间隔，还有最长间隔时间之类的。。。。。
bool setuptimeout(DWORD ReadInterval, DWORD ReadTotalMultiplier, DWORD ReadTotalConstant, DWORD WriteTotalMultiplier, DWORD WriteTotalConstant)
{

	COMMTIMEOUTS timeouts;
	timeouts.ReadIntervalTimeout = ReadInterval;
	timeouts.ReadTotalTimeoutConstant = ReadTotalConstant;
	timeouts.ReadTotalTimeoutMultiplier = ReadTotalMultiplier;
	timeouts.WriteTotalTimeoutConstant = WriteTotalConstant;
	timeouts.WriteTotalTimeoutMultiplier = WriteTotalMultiplier;
	if (!SetCommTimeouts(hComm, &timeouts))
		return false;
	else
		return true;
}


//发送数据函数
void WriteChar(const char* m_szWriteBuffer, DWORD m_nToSend)              //参考ReceiveChar()函数
{
	BOOL bWrite = TRUE;
	BOOL bResult = TRUE;
	DWORD BytesSent = 0;
	HANDLE m_hWriteEvent = NULL;
	ResetEvent(m_hWriteEvent);
	if (bWrite)
	{
		m_ov.Offset = 0;
		m_ov.OffsetHigh = 0;
		//清空缓存区
		bResult = WriteFile(hComm, m_szWriteBuffer, m_nToSend, &BytesSent, &m_ov);
		if (!bResult)
		{
			DWORD dwError = GetLastError();
			switch (dwError)
			{
			case ERROR_IO_PENDING:
			{
				//继续
				BytesSent = 0;
				bWrite = FALSE;
				break;
			}
			default:
			{
				break;
			}
			}
		}

	}
	if (!bWrite)
	{
		bWrite = TRUE;
		bResult = GetOverlappedResult(hComm, &m_ov, &BytesSent, TRUE);
		if (!bResult)
		{
			printf("GetOverlappedResults() in WriteFile()");
		}
	}
	if (BytesSent != m_nToSend)
	{
		printf("WARNING:WriteFile() error..Bytes Sent:%d;Message Length:%d\n", BytesSent, strlen((char*)m_szWriteBuffer));
	}
	//return true;

}


static unsigned __stdcall ThreadComm(void * p)
{
	CcammerParamDlg* _this = (CcammerParamDlg*)p;
	BOOL bRead = TRUE;
	BOOL bResult = TRUE;
	DWORD dwError = 0;
	DWORD BytesRead = 0;
	char RXBuff;                                                       //接收到的数据
	DWORD dwLength;

	char strBuf[20];
	_this->m_comm_state.SetWindowTextA("在线");


	sprintf(strBuf, "COM%d", m_commParam.port);
	if (openport(strBuf))                                                          //打开串口
	{
		printf("open comport success\n");
	}
	else
		_this->m_comm_state.SetWindowTextA("离线");

	if (setupdcb(m_commParam.baud))                                                            //设置波特率为9600，其他采用默认值
		printf("setup DCB success\n");
	else
		_this->m_comm_state.SetWindowTextA("离线");

	if (setuptimeout(0, 0, 0, 0, 0))                                                   //设置延时等待时间
		printf("setup timeouts success\n");
	else
		_this->m_comm_state.SetWindowTextA("离线");

	PurgeComm(hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);     //清空缓冲区
	unsigned char dataMarkBitIdx = 0;
	int serial_data_indx = 0;

#define  MAX_BUFF 10024
	BYTE m_InPutBuff[MAX_BUFF];
#define STARTSIZE 3
	unsigned char serialBuf_start[STARTSIZE] = { 0X11, 0X22, 0X55 };  //起始标志
	unsigned char serialBuf_data[MAX_BUFF];
	while (1)
	{
		bResult = ClearCommError(hComm, &dwError, &comstat);

		dwLength = comstat.cbInQue;
		if (dwLength > MAX_BUFF)
			dwLength = MAX_BUFF;

		if (dwLength > 0)
			ReadFile(hComm, m_InPutBuff, dwLength, &BytesRead, &m_ov);
		static int lenIndex = 0;
		bool flagEnd = 0;
		static bool flagS = 1;
		static int switch_on = 0;
		static int trueNum = 0;
		static int errorNum = 0;
		static unsigned char len = 0;
		int indX = 0;
		static unsigned char verifyBit = 0;

		for (int i = 0; i < dwLength; i++)
		{
			switch (switch_on)
			{
			case 0:  //起始标志位
			{
				if (flagS)
				{
					if (m_InPutBuff[i] == serialBuf_start[0])
					{
						flagS = 0;
						lenIndex++;
						continue;   //起始标志位的第一个字节----找到
					}
					else
					{
						//	printf("\n起始标志位未找到---%d(%d)\n", trueNum, errorNum++);  //起始标志位的第一个字节------未找到
						flagEnd = 1;
						continue;
					}
				}
				else
				{

					if (m_InPutBuff[i] != serialBuf_start[lenIndex])
					{
						//printf("\n数据包出错---重新再来---%d(%d)\n", trueNum, errorNum++);
						flagS = 1;
						flagEnd = 1;
						lenIndex = 0;
						i--;

						continue;  //数据包出错---重新再来
					}
					lenIndex++;
					if (STARTSIZE == lenIndex)
					{
						lenIndex = 0;
						flagS = 1;
						switch_on = 1;  //状态转移

						//indX = i+1;

						break;
					}
				}
			}
				break;
			case 1:  //数据标识位
				//1--平均灰度值
				dataMarkBitIdx = m_InPutBuff[i];
				switch_on = 2;
					
				
				break;
			case 2:  //数据长度len
				len = m_InPutBuff[i];
				//	indX = i + 1;
				serial_data_indx = 0;
				verifyBit = 0;

				switch_on = 3;  //状态转移
				break;
			case 3:  //读取len个数据
				serialBuf_data[serial_data_indx++] = m_InPutBuff[i];
				verifyBit ^= m_InPutBuff[i];
				if (serial_data_indx == len)
				{
					switch_on = 4; //状态转移
				}

				break;
			case 4: //读取校验码
					switch_on = 5; //状态转移
			case 5: //结束标志位
				if (0x88 != m_InPutBuff[i])
				{
					//出错----丢弃数据包
				
				}
				else
				{
					if (1 == dataMarkBitIdx)
					{
						_this->m_edit_aver_gray = serialBuf_data[0];
						_this->SendMessage(WM_DISPLAY_CHANGE, FALSE);
					}

				}
				switch_on = 0; //状态转移
			default:
				switch_on = 0; //状态转移
				break;
			}
		}
		memset(m_InPutBuff, 0, MAX_BUFF);
		::Sleep(2);
	}
	_endthreadex(0);
	return 0;
}


void CcammerParamDlg::initPhase()
{
	m_use_param_flag.use_br = 0;
	m_use_param_flag.use_co = 0;
	m_use_param_flag.use_ec = 0;
	m_use_param_flag.use_em = 0;
	m_use_param_flag.use_gain = 0;
	m_use_param_flag.use_mm = 0;
	m_use_param_flag.use_sa = 0;
	m_use_param_flag.use_sh = 0;
	m_use_param_flag.use_shutter = 0;
	m_use_param_flag.use_enhance_image = 0;

	m_button_choice = -1;
	m_radio_exposure_mode = 0;
	m_radio_metering = 0;

	m_check_enhance_image = 0;

	m_slider_fps.SetRange(1, 30);
	m_slider_brightness.SetRange(0, 100);
	m_slider_sharpness.SetRange(0, 200);
	m_slider_contrast.SetRange(0, 200);
	m_slider_saturation.SetRange(0, 200);
	m_slider_exposure_compensation.SetRange(0, 255);
	m_slider_shutter.SetRange(0, 330000);

	m_slider_fps.SetLineSize(1);
	m_slider_brightness.SetLineSize(1);
	m_slider_sharpness.SetLineSize(1);
	m_slider_contrast.SetLineSize(1);
	m_slider_saturation.SetLineSize(1);
	m_slider_exposure_compensation.SetLineSize(1);
	m_slider_shutter.SetLineSize(1);

	m_slider_fps.SetPageSize(3);
	m_slider_brightness.SetPageSize(3);
	m_slider_sharpness.SetPageSize(3);
	m_slider_contrast.SetPageSize(3);
	m_slider_saturation.SetPageSize(3);
	m_slider_exposure_compensation.SetPageSize(3);
	m_slider_shutter.SetPageSize(1000);


	m_commParam.port = 8;
	m_commParam.baud = 115200;
	loadConfig();

	m_Handle_comm = (HANDLE)_beginthreadex(NULL, 0, ThreadComm, this, 0, NULL);

	UpdateData(FALSE);

}

bool CcammerParamDlg::loadConfig()
{
	TCHAR charPath[MAX_PATH] = { 0 };
	::GetModuleFileName(NULL, charPath, MAX_PATH);
	// 设置ini路径到exe同一目录下
	//_tcsrchr() 反向搜索获得最后一个'\\'的位置，并返回该位置的指针  
	TCHAR *pFind = _tcsrchr(charPath, '\\');
	if (pFind != NULL)
	{
		*pFind = '\0';
	}

	CString strPath = charPath;




	strPath += "\\CONFIG.ini";
	char str[20];

	::GetPrivateProfileString("COMMPARAM", "port", "1", str, 20, strPath);
	m_commParam.port = _ttoi(str);
	::GetPrivateProfileString("COMMPARAM", "baud", "1", str, 20, strPath);
	m_commParam.baud = _ttoi(str);

	::GetPrivateProfileString("CAMMERPARAM", "m_edit_fps", "1", str, 20, strPath);
	m_edit_fps = _ttoi(str);
	m_slider_fps.SetPos(m_edit_fps);
	::GetPrivateProfileString("CAMMERPARAM", "m_edit_brightness", "1", str, 20, strPath);
	m_edit_brightness = _ttoi(str);
	m_slider_brightness.SetPos(m_edit_brightness);
	::GetPrivateProfileString("CAMMERPARAM", "m_edit_GAIN", "1", str, 20, strPath);
	m_edit_GAIN = _ttoi(str);
	m_slider_gain.SetPos(m_edit_GAIN);
	::GetPrivateProfileString("CAMMERPARAM", "m_edit_sharpness", "1", str, 20, strPath);
	m_edit_sharpness = _ttoi(str);
	m_slider_sharpness.SetPos(m_edit_sharpness);
	::GetPrivateProfileString("CAMMERPARAM", "m_edit_contrast", "1", str, 20, strPath);
	m_edit_contrast = _ttoi(str);
	m_slider_contrast.SetPos(m_edit_contrast);
	::GetPrivateProfileString("CAMMERPARAM", "m_edit_saturation", "1", str, 20, strPath);
	m_edit_saturation = _ttoi(str);
	m_slider_saturation.SetPos(m_edit_saturation);
	::GetPrivateProfileString("CAMMERPARAM", "m_edit_exposure_compensation", "1", str, 20, strPath);
	m_edit_exposure_compensation = _ttoi(str);
	m_slider_exposure_compensation.SetPos(m_edit_exposure_compensation);
	::GetPrivateProfileString("CAMMERPARAM", "m_edit_shutter", "1", str, 20, strPath);
	m_edit_shutter = _ttoi(str);
	m_slider_shutter.SetPos(m_edit_shutter);
	::GetPrivateProfileString("CAMMERPARAM", "m_radio_exposure_mode", "1", str, 20, strPath);
	m_radio_exposure_mode = _ttoi(str);
	::GetPrivateProfileString("CAMMERPARAM", "m_radio_metering", "1", str, 20, strPath);
	m_radio_metering = _ttoi(str);
	UpdateData(FALSE);

	return false;
}


void CcammerParamDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (*pScrollBar == m_slider_fps)
	{
//		m_button_choice = RPI_FPS;
		m_edit_fps = m_slider_fps.GetPos();
		UpdateData(FALSE);
	}
	else if (*pScrollBar == m_slider_brightness)
	{
		m_use_param_flag.use_br = 1;
		m_button_choice = RPI_BR;
		m_edit_brightness = m_slider_brightness.GetPos();
		UpdateData(FALSE);
	}
	else if (*pScrollBar == m_slider_gain)
	{
		m_use_param_flag.use_gain = 1;
		m_button_choice = RPI_GAIN;
		m_edit_GAIN = m_slider_gain.GetPos();
		UpdateData(FALSE);
	}
	else if (*pScrollBar == m_slider_sharpness)
	{
		m_use_param_flag.use_sh = 1;
		m_button_choice = RPI_SH;
		m_edit_sharpness = m_slider_sharpness.GetPos();
		UpdateData(FALSE);
	}
	else if (*pScrollBar == m_slider_contrast)
	{
		m_use_param_flag.use_co = 1;
		m_button_choice = RPI_CO;
		m_edit_contrast = m_slider_contrast.GetPos();
		UpdateData(FALSE);
	}
	else if (*pScrollBar == m_slider_saturation)
	{
		m_use_param_flag.use_sa = 1;
		m_button_choice = RPI_SA;
		m_edit_saturation = m_slider_saturation.GetPos();
		UpdateData(FALSE);
	}
	else if (*pScrollBar == m_slider_exposure_compensation)
	{
		m_use_param_flag.use_ec = 1;
		m_button_choice = RPI_EC;
		m_edit_exposure_compensation = m_slider_exposure_compensation.GetPos();
		UpdateData(FALSE);
	}
	else if (*pScrollBar == m_slider_shutter)
	{
		m_use_param_flag.use_shutter = 1;
		m_button_choice = RPI_SHUTTER;
		m_edit_shutter = m_slider_shutter.GetPos();
		UpdateData(FALSE);
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CcammerParamDlg::OnEnChangeEditFps()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_slider_fps.SetPos(m_edit_fps);

//	m_button_choice = RPI_FPS;
}


void CcammerParamDlg::OnEnChangeEditBrightness()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_slider_brightness.SetPos(m_edit_brightness);
	m_button_choice = RPI_BR;
	m_use_param_flag.use_br= 1;

}


void CcammerParamDlg::OnEnChangeEditGain()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_slider_gain.SetPos(m_edit_GAIN);
	m_button_choice = RPI_GAIN;
	m_use_param_flag.use_gain = 1;
}


void CcammerParamDlg::OnEnChangeEditSharpness()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_slider_sharpness.SetPos(m_edit_sharpness);
	m_button_choice = RPI_SH;
	m_use_param_flag.use_sh = 1;
}


void CcammerParamDlg::OnEnChangeEditContrast()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_slider_contrast.SetPos(m_edit_contrast);
	m_button_choice = RPI_CO;
	m_use_param_flag.use_co = 1;
}


void CcammerParamDlg::OnEnChangeEditSaturation()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_slider_saturation.SetPos(m_edit_saturation);
	m_button_choice = RPI_SA;
	m_use_param_flag.use_sa = 1;
}


void CcammerParamDlg::OnEnChangeEditExposureCompensation()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_slider_exposure_compensation.SetPos(m_edit_exposure_compensation);
	m_button_choice = RPI_EC;
	m_use_param_flag.use_ec = 1;
}


void CcammerParamDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	TCHAR charPath[MAX_PATH] = { 0 };
	::GetModuleFileName(NULL, charPath, MAX_PATH);
	// 设置ini路径到exe同一目录下
	//_tcsrchr() 反向搜索获得最后一个'\\'的位置，并返回该位置的指针  
	TCHAR *pFind = _tcsrchr(charPath, '\\');
	if (pFind != NULL)
	{
		*pFind = '\0';
	}

	CString strPath = charPath;
	strPath += "\\CONFIG.ini";


	UpdateData(TRUE);
	char send_bug[9] = { 0x11, 0x22, 0x55, 0x00, 0x03, 0x00, 0x00, 0x00, 0x88 };
	if (m_use_param_flag.use_br)
	{
		m_use_param_flag.use_br = 0;
		send_bug[3] = RPI_BR;
		send_bug[5] = (m_edit_brightness & 0xff0000) >> 16;
		send_bug[6] = (m_edit_brightness & 0x00ff00) >> 8;
		send_bug[7] = (m_edit_brightness & 0x0000ff);
		WriteChar(send_bug, 9);

		char str[10];
		sprintf(str, "%d", m_edit_brightness);
		::WritePrivateProfileString("CAMMERPARAM", "m_edit_brightness", str, strPath);
	}
	if (m_use_param_flag.use_gain)
	{
		m_use_param_flag.use_gain = 0;
		send_bug[3] = RPI_GAIN;
		send_bug[5] = (m_edit_GAIN & 0xff0000) >> 16;
		send_bug[6] = (m_edit_GAIN & 0x00ff00) >> 8;
		send_bug[7] = (m_edit_GAIN & 0x0000ff);
		WriteChar(send_bug, 9);

		char str[10];
		sprintf(str, "%d", m_edit_GAIN);
		::WritePrivateProfileString("CAMMERPARAM", "m_edit_GAIN", str, strPath);
	}
	if (m_use_param_flag.use_sh)
	{
		m_use_param_flag.use_sh = 0;
		send_bug[3] = RPI_SH;
		send_bug[5] = (m_edit_sharpness & 0xff0000) >> 16;
		send_bug[6] = (m_edit_sharpness & 0x00ff00) >> 8;
		send_bug[7] = (m_edit_sharpness & 0x0000ff);
		WriteChar(send_bug, 9);

		char str[10];
		sprintf(str, "%d", m_edit_sharpness);
		::WritePrivateProfileString("CAMMERPARAM", "m_edit_sharpness", str, strPath);
	}
	if (m_use_param_flag.use_co)
	{
		m_use_param_flag.use_co = 0;
		send_bug[3] = RPI_CO;
		send_bug[5] = (m_edit_contrast & 0xff0000) >> 16;
		send_bug[6] = (m_edit_contrast & 0x00ff00) >> 8;
		send_bug[7] = (m_edit_contrast & 0x0000ff);
		WriteChar(send_bug, 9);

		char str[10];
		sprintf(str, "%d", m_edit_contrast);
		::WritePrivateProfileString("CAMMERPARAM", "m_edit_contrast", str, strPath);
	}
	if (m_use_param_flag.use_sa)
	{
		m_use_param_flag.use_sa = 0;
		send_bug[3] = RPI_SA;
		send_bug[5] = (m_edit_saturation & 0xff0000) >> 16;
		send_bug[6] = (m_edit_saturation & 0x00ff00) >> 8;
		send_bug[7] = (m_edit_saturation & 0x0000ff);
		WriteChar(send_bug, 9);

		char str[10];
		sprintf(str, "%d", m_edit_saturation);
		::WritePrivateProfileString("CAMMERPARAM", "m_edit_saturation", str, strPath);
	}
	if (m_use_param_flag.use_ec)
	{
		m_use_param_flag.use_ec = 0;
		send_bug[3] = RPI_EC;
		send_bug[5] = (m_edit_exposure_compensation & 0xff0000) >> 16;
		send_bug[6] = (m_edit_exposure_compensation & 0x00ff00) >> 8;
		send_bug[7] = (m_edit_exposure_compensation & 0x0000ff);
		WriteChar(send_bug, 9);

		char str[10];
		sprintf(str, "%d", m_edit_exposure_compensation);
		::WritePrivateProfileString("CAMMERPARAM", "m_edit_exposure_compensation", str, strPath);
	}
	if (m_use_param_flag.use_shutter)
	{
		m_use_param_flag.use_shutter = 0;
		send_bug[3] = RPI_SHUTTER;
		send_bug[5] = (m_edit_shutter & 0xff0000) >> 16;
		send_bug[6] = (m_edit_shutter & 0x00ff00) >> 8;
		send_bug[7] = (m_edit_shutter & 0x0000ff);
		WriteChar(send_bug, 9);

		char str[10];
		sprintf(str, "%d", m_edit_shutter);
		::WritePrivateProfileString("CAMMERPARAM", "m_edit_shutter", str, strPath);
	}
	if (m_use_param_flag.use_em)
	{
		int emode = 1;
		if (m_radio_exposure_mode == 0)
		{
			emode = 0;
		}
		m_use_param_flag.use_em = 0;
		send_bug[3] = RPI_EM;
		send_bug[5] = (emode & 0xff0000) >> 16;
		send_bug[6] = (emode & 0x00ff00) >> 8;
		send_bug[7] = (emode & 0x0000ff);
		WriteChar(send_bug, 9);

		char str[10];
		sprintf(str, "%d", m_radio_exposure_mode);
		::WritePrivateProfileString("CAMMERPARAM", "m_radio_exposure_mode", str, strPath);
	}
	if (m_use_param_flag.use_mm)
	{
		m_use_param_flag.use_mm = 0;
		send_bug[3] = RPI_MM;
		send_bug[5] = (m_radio_metering & 0xff0000) >> 16;
		send_bug[6] = (m_radio_metering & 0x00ff00) >> 8;
		send_bug[7] = (m_radio_metering & 0x0000ff);
		WriteChar(send_bug, 9);

		char str[10];
		sprintf(str, "%d", m_radio_metering);
		::WritePrivateProfileString("CAMMERPARAM", "m_radio_metering", str, strPath);
	}

	//str.Format("11 22 55 %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d 88"
	//	,RPI_PARM_SIZE * 2,
	//	/*RPI_FPS, m_edit_fps,*/ RPI_BR, m_edit_brightness, RPI_GAIN, m_edit_GAIN,
	//	RPI_SH, m_edit_sharpness, RPI_CO, m_edit_contrast, RPI_SA, m_edit_saturation,
	//	RPI_EC, m_edit_exposure_compensation, RPI_SHUTTER, m_edit_shutter,
	//	RPI_EM, m_radio_exposure_mode, RPI_MM, m_radio_metering
	//	);


	CDialogEx::OnRButtonUp(nFlags, point);
}


void CcammerParamDlg::OnBnClickedRadio2()
{
	// TODO:  在此添加控件通知处理程序代码
	m_use_param_flag.use_em = 1;
}


void CcammerParamDlg::OnBnClickedRadio1()
{
	m_use_param_flag.use_em = 1;

	// TODO:  在此添加控件通知处理程序代码
}


void CcammerParamDlg::OnBnClickedRadio8()
{
	m_use_param_flag.use_mm = 1;

	// TODO:  在此添加控件通知处理程序代码
}


void CcammerParamDlg::OnBnClickedRadio10()
{
	m_use_param_flag.use_mm = 1;


	// TODO:  在此添加控件通知处理程序代码
}


void CcammerParamDlg::OnBnClickedRadio9()
{
	m_use_param_flag.use_mm = 1;


	// TODO:  在此添加控件通知处理程序代码
}


void CcammerParamDlg::OnBnClickedRadio11()
{
	m_use_param_flag.use_mm = 1;


	// TODO:  在此添加控件通知处理程序代码
}

void CcammerParamDlg::saveCammerParam()
{
	UpdateData(TRUE);
	TCHAR charPath[MAX_PATH] = { 0 };
	::GetModuleFileName(NULL, charPath, MAX_PATH);
	// 设置ini路径到exe同一目录下
	//_tcsrchr() 反向搜索获得最后一个'\\'的位置，并返回该位置的指针  
	TCHAR *pFind = _tcsrchr(charPath, '\\');
	if (pFind != NULL)
	{
		*pFind = '\0';
	}

	CString strPath = charPath;
	strPath += "\\rpi_cammer5647_param.txt";

	std::ofstream file;
	if (file.bad())
	{
		std::cout << "cannot open file" << std::endl;;
	}
	file.open(strPath, std::ios::app);

	CString str;
	str.Format("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d"
		,m_edit_aver_gray, RPI_PARM_SIZE * 2,
		/*RPI_FPS, m_edit_fps,*/ RPI_BR, m_edit_brightness, RPI_GAIN, m_edit_GAIN,
		RPI_SH, m_edit_sharpness, RPI_CO, m_edit_contrast, RPI_SA, m_edit_saturation,
		RPI_EC, m_edit_exposure_compensation, RPI_SHUTTER, m_edit_shutter,
		RPI_EM, m_radio_exposure_mode, RPI_MM, m_radio_metering
		);
	file << str << "\n";
}

void CcammerParamDlg::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	CString strName;
	m_button_adjust_param.GetWindowTextA(strName);

	//CString str;
	if (strName == "调试...")
	{
		m_button_adjust_param.SetWindowTextA("完成");
		char send_bug[9] = { 0x11, 0x22, 0x55, 0x00, 0x03, 0x00, 0x00, 0x00, 0x88 };
		send_bug[3] = RPI_ADJUST_PARAM;
		WriteChar(send_bug, 9);
	}
	else{
		m_button_adjust_param.SetWindowTextA("调试...");
		saveCammerParam();
		//str.Format("11 22 55 1 %d 0 88", RPI_ADJUST_PARAM);
	}

	
	
}


void CcammerParamDlg::OnEnChangeEditShutter()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_slider_shutter.SetPos(m_edit_shutter);

	m_button_choice = RPI_SHUTTER;
}


void CcammerParamDlg::OnBnClickedCheckEnhance()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);

	{
		char send_bug[9] = { 0x11, 0x22, 0x55, 0x00, 0x03, 0x00, 0x00, 0x00, 0x88 };

		send_bug[3] = RPI_ENHANCE_IMAGE;
		send_bug[5] = (m_check_enhance_image & 0xff0000) >> 16;
		send_bug[6] = (m_check_enhance_image & 0x00ff00) >> 8;
		send_bug[7] = (m_check_enhance_image & 0x0000ff);
		WriteChar(send_bug, 9);

		
	}
}
