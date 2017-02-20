
// AntMonDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AntMon.h"
#include "AntMonDlg.h"
#include "afxdialogex.h"

#include "config.h"
#include "types.h"
#include "ant_control.h"
#include "ant_dll.h"
#include "antmessage.h"

#include <afxinet.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static CAntMonDlg* pDlgMon;


// CAboutDlg dialog used for App About
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CAntMonDlg dialog



CAntMonDlg::CAntMonDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ANTMON_DIALOG, pParent)
	, m_bOpened(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	for (int i = 0;i < MAX_DEVICES;i++)
	{
		memset(&m_rxMsg[i], 0, sizeof(ANTMsg));
		m_rxMsg[i].deviceNo = 0xFFFF;
	}
}

void CAntMonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LOG, m_listLog);
}

BEGIN_MESSAGE_MAP(CAntMonDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CAntMonDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CAntMonDlg::OnBnClickedButtonStop)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CAntMonDlg message handlers

BOOL CAntMonDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	pDlgMon = this;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAntMonDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAntMonDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAntMonDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CAntMonDlg::OnBnClickedButtonStart()
{
	StartMon(0);
}


void CAntMonDlg::StartMon(int port)
{
	if (m_bOpened == false)
	{
		UCHAR ucUSB;
		USHORT usBaud = 57600;  // Scanning mode is not supported on AP1 

		ucUSB = port;
		if (ucUSB > 4)
			ucUSB = 0;

		if (ANTControl_Connect((ANT_RESPONSE_FUNC)ANTCallback, ucUSB, usBaud))
		{

			m_bOpened = true;
		}
	}
	
}


void CAntMonDlg::StopMon()
{
	if (m_bOpened == true) 
	{
		ANTControl_CloseScanningMode();
		ANTControl_Disconnect();
		m_bOpened = false;
	}
	
}

void CAntMonDlg::ANTCallback(UCHAR ucEvent_, char* pcBuffer_)
{
	USHORT usDeviceNo = 0xFFFFFFFF;
	UCHAR ucDeviceType = 0;
	UCHAR ucTransType = 0;

	BOOL bRxOnly = TRUE;

	switch (ucEvent_)
	{
		// Handle events generated at ant_control
	case ANT_EVENT_DEBUG:
	{
		if (pcBuffer_)
			pDlgMon->AddLog((LPCTSTR)pcBuffer_);
		break;
	}
	case ANT_EVENT_CONNECT:
	{
		pDlgMon->AddLog(_T("Connected"));



		// Config scanning mode
		if (TRUE == ANTControl_ConfigScanningMode(0, 0, 0, 57, TRUE))
		{
			ANTControl_OpenScanningMode();
		}
		break;
	}
	case ANT_EVENT_DISCONNECT:
	{

		pDlgMon->AddLog(_T("DisConnected"));
		break;
	}
	case ANT_EVENT_OPEN:
	{
		pDlgMon->AddLog(_T("Started scanning mode..."));


		break;
	}
	case ANT_EVENT_CLOSE:
	{
		pDlgMon->AddLog(_T("Close scanning"));
		break;
	}
	
	// Handle ANT messages (Rx data)
	case EVENT_RX_FLAG_ACKNOWLEDGED: // Handle flagged extended messages
	case EVENT_RX_FLAG_BURST_PACKET:
	case EVENT_RX_FLAG_BROADCAST:
	{
		if (pcBuffer_)
		{
			UCHAR ucFlag = (UCHAR)pcBuffer_[8]; // Flag byte

			if (ucFlag & 0x80)    // extended message including channel ID
			{
				// Get Channel ID
				usDeviceNo = ((UCHAR)pcBuffer_[10]) << 8 | (UCHAR)pcBuffer_[9];
				ucDeviceType = (UCHAR)pcBuffer_[11];
				ucTransType = (UCHAR)pcBuffer_[12];
			}
		}

	}
	
	case EVENT_RX_ACKNOWLEDGED:
	case EVENT_RX_BURST_PACKET:
	case EVENT_RX_BROADCAST:
	{
		if (pcBuffer_)
		{
			pDlgMon->HandleMessage((UCHAR*)pcBuffer_);
		}
		break;
	}
	/*
	case EVENT_RX_EXT_ACKNOWLEDGED:  // Legacy extended messages
	case EVENT_RX_EXT_BURST_PACKET:
	case EVENT_RX_EXT_BROADCAST:
	{
		if (pcBuffer_)
		{
			// Get Channel ID
			USHORT usDeviceNo = ((UCHAR)pcBuffer_[1]) << 8 | (UCHAR)pcBuffer_[0];
			UCHAR ucDeviceType = (UCHAR)pcBuffer_[2];
			UCHAR ucTransType = (UCHAR)pcBuffer_[3];

			// Display received data
			sprintf_s((char*)aucTxExtBuf, TX_BUFFER_SIZE, "<ChID:%d,%d,%d> ", usDeviceNo, ucDeviceType, ucTransType);
			tbDisplay->AppendText(gcnew String(aucTxExtBuf));

			if (ucEvent_ == EVENT_RX_EXT_ACKNOWLEDGED)
				tbDisplay->AppendText("Acked Rx: ");
			else if (ucEvent_ == EVENT_RX_EXT_BURST_PACKET)
				tbDisplay->AppendText("Burst Rx: ");
			else
				tbDisplay->AppendText("Rx: ");

			sprintf_s((char*)aucTxExtBuf, TX_BUFFER_SIZE, "[%02X][%02X][%02X][%02X][%02X][%02X][%02X][%02X]\r\n", (UCHAR)pcBuffer_[4], (UCHAR)pcBuffer_[5], (UCHAR)pcBuffer_[6], (UCHAR)pcBuffer_[7], (UCHAR)pcBuffer_[8], (UCHAR)pcBuffer_[9], (UCHAR)pcBuffer_[10], (UCHAR)pcBuffer_[11]);
			tbDisplay->AppendText(gcnew String(aucTxExtBuf));
		}
		break;
	}
	case EVENT_TX:
	{
		tbDisplay->AppendText("Transmitted data\r\n"); // Pending broadcast was transmitted
		break;
	}
	case EVENT_TRANSFER_TX_COMPLETED:
	{
		tbDisplay->AppendText("Tx success!\r\n");   // Pending burst/ack was transmitted successfully
		break;
	}
	default:
		break;
		*/
	}
}

void CAntMonDlg::OnBnClickedButtonStop()
{
	StopMon();
}


void CAntMonDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	StopMon();
}


void CAntMonDlg::AddLog(LPCTSTR log)
{
	m_listLog.InsertString(0, log);
}


void CAntMonDlg::HandleMessage(UCHAR *pcBuffer_)
{
	CHAR ucFlag = (UCHAR)pcBuffer_[8]; // Flag byte

	if (!(ucFlag & 0x80)) return;

	USHORT usDeviceNo = ((UCHAR)pcBuffer_[10]) << 8 | (UCHAR)pcBuffer_[9];
	USHORT ucDeviceType = (UCHAR)pcBuffer_[11];

	int nMsgIndex = AddMsgBuf(usDeviceNo, ucDeviceType);
	ANTMsg *pMsg = NULL;
	if (nMsgIndex < 0) return;// error
	pMsg = &m_rxMsg[nMsgIndex];
	
	if (pMsg->deviceNo == 0xFFFF)
	{
		//new device
		pMsg->deviceNo = usDeviceNo;
		pMsg->deviceType = ucDeviceType;

		AddNewDevice(pMsg);
	}

	if (pMsg->deviceType == 120) {
		UCHAR hr = pcBuffer_[7];
		if (pMsg->heartRate != hr)
		{
			pMsg->heartRate = hr;
			TRACE(_T("Heart Rate %d\n"), pMsg->heartRate);
			ControlHUE(pMsg);
		}
	}
	
}

int CAntMonDlg::AddMsgBuf(USHORT deviceNo, USHORT deviceType)
{
	int nMsgIndex = -1;
	for (int i = 0;i < MAX_DEVICES;i++)
	{
		if (m_rxMsg[i].deviceNo == deviceNo || m_rxMsg[i].deviceNo == 0xFFFF)
		{
			nMsgIndex = i;
		}
	}
	return nMsgIndex;
}


void CAntMonDlg::AddNewDevice(ANTMsg* pMsg)
{
	CString msg;
	msg.Format(_T("Added %d:%d"), pMsg->deviceNo, pMsg->deviceType);
	m_listLog.InsertString(0, msg);
}


void CAntMonDlg::ControlHUE(ANTMsg* pMsg)
{
	char szBody[256];
	sprintf_s(szBody, "{\"bri\":%d}", (pMsg->heartRate-40)*30);
	SendHTTPMsg(
		_T("http://192.168.0.36/api/q2HQvhloDSN5MQHa3zDGyfpgR34CDWzTOh394zDx/lights/9/state"), 
		CHttpConnection::HTTP_VERB_PUT, 
		szBody,
		strlen(szBody),
		NULL, 
		NULL
	);
}

CString CAntMonDlg::SendHTTPMsg(LPCTSTR pszUrl, int method, LPVOID body, DWORD bodyLen, LPCTSTR pszReferer/* = NULL*/, LPCTSTR pszAppendHeader/* = NULL*/)
{
	// UTF-8�� �����ؾ��ϴ°�� (���������� �ѱ��� ���ԵǼ� �����ؾ� �Ҷ�)
	CString sParameters = _T("{\"bri\":30}");

	// ��ǥURL�� �ƴ� asp�� jsp�������� ������ ��ü�ּ�
	CString sURL = pszUrl;

	//http ���� / Ŀ�ؼ�
	CInternetSession*    pSession = NULL;
	CHttpConnection* pHttpConnect = NULL;
	CHttpFile*      pHttpFile = NULL;

	// url�Ľ� ����
	CString strServerName, strObject, strHeaders;
	INTERNET_PORT nPort;
	DWORD dwServiceType = 0;
	DWORD m_dwStatusCode = 0;

	// url�Ľ�
	AfxParseURL(sURL, dwServiceType, strServerName, strObject, nPort);

	// http�����û
	pSession = new CInternetSession;
	pHttpConnect = pSession->GetHttpConnection(strServerName);

	pHttpFile = pHttpConnect->OpenRequest(method, strObject, pszReferer);

	// http���

	if (!pszAppendHeader)
	{
		strHeaders = _T("Content-Type: application/x-www-form-urlencoded");
		pHttpFile->AddRequestHeaders(strHeaders);
		strHeaders = _T("User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; rv:11.0)  like Gecko");
		pHttpFile->AddRequestHeaders(strHeaders);
	}
	else
	{
		pHttpFile->AddRequestHeaders(pszAppendHeader);
	}

	// ������ ������ ����
	//const int dwLength = _tcslen(sParameters) + 1;
	const int dwLength = _tcslen(sParameters);

	// ������ ����
	//pHttpFile->SendRequest(strHeaders, sParameters.GetBuffer(), dwLength);
	pHttpFile->SendRequest(strHeaders, body, bodyLen);

	pHttpFile->QueryInfoStatusCode(m_dwStatusCode);

	CString szHeaders;
	pHttpFile->QueryInfo(HTTP_QUERY_RAW_HEADERS_CRLF, szHeaders);

	BOOL bUTF8mode = szHeaders.Find(_T("charset=UTF-8")) >= 0 ||
		szHeaders.Find(_T("charset=utf-8")) >= 0 ||
		szHeaders.Find(_T("CHARSET=UTF-8")) >= 0 ||
		szHeaders.Find(_T("CHARSET=utf-8")) >= 0;

	CString strCompleteString;
	if (m_dwStatusCode == 200)
	{
		CString strResult;
		while (pHttpFile->ReadString(strResult))
		{
			if (bUTF8mode)
				strCompleteString += CW2T(CA2W((char*)strResult.GetBuffer(), CP_UTF8));
			else
				strCompleteString += CW2T(CA2W((char*)strResult.GetBuffer()));

			strResult.ReleaseBuffer();
		}
	}

	// ��������
	if (pHttpFile)
	{
		delete pHttpFile;
		pHttpFile = NULL;
	}
	if (pHttpConnect)
	{
		pHttpConnect->Close();
		delete pHttpConnect;
		pHttpConnect = NULL;
	}
	if (pSession)
	{
		pSession->Close();
		delete pSession;
		pSession = NULL;
	}

	return strCompleteString;
}