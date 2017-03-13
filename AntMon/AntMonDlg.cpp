
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

#define WM_ANT_MSG (WM_USER + 1)

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
	, m_bHueThreadStop(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_strHueUrl = _T("http://192.168.1.200/api/q2HQvhloDSN5MQHa3zDGyfpgR34CDWzTOh394zDx/lights/");
	//m_strHueUrl = _T("http://192.168.1.4/api/q2HQvhloDSN5MQHa3zDGyfpgR34CDWzTOh394zDx/lights/5/state");
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
	DDX_Control(pDX, IDC_LIST_RIDER, m_listRider);
}

BEGIN_MESSAGE_MAP(CAntMonDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CAntMonDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CAntMonDlg::OnBnClickedButtonStop)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_DASHBOARD, &CAntMonDlg::OnBnClickedButtonDashboard)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDOK, &CAntMonDlg::OnBnClickedOk)
	ON_MESSAGE(WM_ANT_MSG, &CAntMonDlg::OnAntMsg)
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

	
	InitRiderList();
	ResetRider();
	ReadRiderFile();

	CreateDashBoard();
	FuncService(true);
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
	FuncService(true);
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
	USHORT usDeviceNo = 0xFFFF;
	UCHAR ucDeviceType = 0;
	UCHAR ucTransType = 0;

	BOOL bRxOnly = TRUE;

	switch (ucEvent_)
	{
		// Handle events generated at ant_control
		case ANT_EVENT_DEBUG:
		{
			if (pcBuffer_)
				pDlgMon->AddLog(CString(pcBuffer_));
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
				UCHAR *lParam = new UCHAR[ANT_MSG_LENGTH];
				memcpy(lParam, &pcBuffer_[0], ANT_MSG_LENGTH);
				pDlgMon->PostMessageA(WM_ANT_MSG, 0, (LPARAM)lParam);
				//pDlgMon->SendMessage(WM_ANT_MSG, 0, (LPARAM)lParam);
			}
			break;
		}
	}
}

void CAntMonDlg::OnBnClickedButtonStop()
{
	StopMon();
}


void CAntMonDlg::OnDestroy()
{

	CDialogEx::OnDestroy();
	
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
	
	int riderIndex = -1;
	bool bUpdateDashboard = false;
	if (pMsg->deviceNo == 0xFFFF)
	{
		//new device
		pMsg->deviceNo = usDeviceNo;
		pMsg->deviceType = ucDeviceType;

		AddNewDevice(pMsg);
	}
	/* Heart rates*/
	if (pMsg->deviceType == ANT_TYPE_HEART) {
		UCHAR bpm = pcBuffer_[7];
		USHORT time = (0xFF00 & (USHORT)pcBuffer_[5] << 4) |pcBuffer_[4];
		USHORT count = pcBuffer_[6];

		//TRACE("Heart Time:%d\tCount:%d\tBPM:%d\n", time, count, bpm);

		pMsg->hrBpm = bpm;
		pMsg->hrCount = count;
		pMsg->hrTime = time;

		riderIndex = FindRider(pMsg);
		if (riderIndex != -1) {
			if (m_Riders[riderIndex].heart != pMsg->hrBpm ||
				m_Riders[riderIndex].heart_time != pMsg->hrTime) {
				bUpdateDashboard = true;
			}
		}
		m_Riders[riderIndex].heart = pMsg->hrBpm;
		m_Riders[riderIndex].heart_time = pMsg->hrTime;

		m_Riders[riderIndex].power = pMsg->hrBpm;
		m_Riders[riderIndex].power_time = pMsg->hrTime;

		m_Riders[riderIndex].cadence = pMsg->hrBpm;
		//	ControlHUE(pMsg);
	}
	else if ((pMsg->deviceType == ANT_TYPE_PWR || pMsg->deviceType == ANT_TYPE_PWR2) && pcBuffer_[0]==16) {	// bike power
		pMsg->pwrValue = pcBuffer_[6] | ( ((USHORT)pcBuffer_[7]<<8 & 0x1FF));
		pMsg->cadValue = pcBuffer_[3];
		
		riderIndex = FindRider(pMsg);
		if (riderIndex != -1) {
			if (m_Riders[riderIndex].power != pMsg->pwrValue || m_Riders[riderIndex].cadence != pMsg->cadValue) {
				m_Riders[riderIndex].power = pMsg->pwrValue;
				m_Riders[riderIndex].cadence = pMsg->cadValue;
				bUpdateDashboard = true;
				//ControlHUE(&m_Riders[riderIndex]);
			}

		}
	}
	else if (pMsg->deviceType == 121) {	// bike candence and speed
	}

	/*
	if ((pMsg->deviceType==121 || pMsg->deviceType == 11) && pcBuffer_[0]==16 && pMsg->deviceNo==4118)
		//if (pMsg->deviceType == 17)
			TRACE(_T("Power Rate %d: %d %d %d %d %d %d %d %d\n"), pMsg->deviceType, pcBuffer_[0], pcBuffer_[1], pcBuffer_[2], pcBuffer_[3], pcBuffer_[4], pcBuffer_[5], pcBuffer_[6], pcBuffer_[7]);
	*/
	if (bUpdateDashboard == true) {
		UpdateRider(pMsg, riderIndex);
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
	char msg[128];
	int nRiderIndex = FindRider(pMsg);
	if (nRiderIndex == -1) {
		sprintf_s(msg, "New Device %d:%d", pMsg->deviceNo, pMsg->deviceType);
		m_listLog.InsertString(0, msg);
	}
	else {
		sprintf_s(msg, "Found Device %d:%d", pMsg->deviceNo, pMsg->deviceType);
		m_listLog.InsertString(0, msg);

		if ((pMsg->deviceType == ANT_TYPE_PWR || pMsg->deviceType == ANT_TYPE_PWR2)) {
			m_listRider.SetItem(nRiderIndex, 3, LVIF_TEXT, "OK", -1, -1, -1, NULL);
		}
		if (pMsg->deviceType == ANT_TYPE_HEART) {
			m_listRider.SetItem(nRiderIndex, 5, LVIF_TEXT, "OK", -1, -1, -1, NULL);
		}
	}



	TRACE("%s\n", msg);
	/*
	CString list = "26404:17 6404:11 25796:17 5796:11 26416:17 6416:121 26322:17 6322:11 26284:121 6284:11";
	CString msg;
	msg.Format(_T("%d:%d"), pMsg->deviceNo, pMsg->deviceType);
	if (list.Find(msg, 0) == -1)
	{
		m_listLog.InsertString(0, msg);
		TRACE("%s\n", msg);
	}
	*/
}
void CAntMonDlg::ControlHUE(RIDER *pRider)
{
	if (pRider->cadence == 0) return;

	HUECommand *pCommand = new HUECommand();
	int minCad = 50;
	int maxCad = 120;
	int bri = 254 * (pRider->cadence - minCad) / (maxCad - minCad);
	bri = bri < 0 ? 0 : (bri > 254? 254 : bri);
	sprintf_s(pCommand->buffer, "{\"bri\":%d,\"sat\":%d, \"hue\":%d}", bri,bri, bri);
	pCommand->length = strlen(pCommand->buffer);
	pCommand->hue_id = pRider->hue_id;
	AddHueCommand(pCommand);
}

CString CAntMonDlg::SendHTTPMsg(LPCTSTR pszUrl, int method, LPVOID body, DWORD bodyLen, LPCTSTR pszReferer/* = NULL*/, LPCTSTR pszAppendHeader/* = NULL*/)
{
	// UTF-8로 전송해야하는경우 (웹페이지에 한글이 포함되서 전달해야 할때)
	CString sParameters = _T("{\"bri\":30}");

	// 대표URL이 아닌 asp나 jsp페이지를 포함한 전체주소
	CString sURL = pszUrl;

	//http 세션 / 커넥션
	CInternetSession*    pSession = NULL;
	CHttpConnection* pHttpConnect = NULL;
	CHttpFile*      pHttpFile = NULL;

	// url파싱 변수
	CString strServerName, strObject, strHeaders;
	INTERNET_PORT nPort;
	DWORD dwServiceType = 0;
	DWORD m_dwStatusCode = 0;

	// url파싱
	AfxParseURL(sURL, dwServiceType, strServerName, strObject, nPort);

	// http연결요청
	pSession = new CInternetSession;
	pHttpConnect = pSession->GetHttpConnection(strServerName);

	pHttpFile = pHttpConnect->OpenRequest(method, strObject, pszReferer);

	// http헤더

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

	// 세션해제
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

UINT CAntMonDlg::HueThread(LPVOID pParam)
{
	CAntMonDlg *pDlg = (CAntMonDlg*)pParam;

	// 대표URL이 아닌 asp나 jsp페이지를 포함한 전체주소
	CString sURL = pDlg->m_strHueUrl;
	HANDLE event = pDlg->m_hHueEvent;
	concurrent_queue< HUECommand* >*pCommandQ = &pDlg->m_hueCommandQ;

	//http 세션 / 커넥션
	CInternetSession*    pSession = NULL;
	CHttpConnection* pHttpConnect = NULL;
	CHttpFile*      pHttpFile = NULL;

	// url파싱 변수
	CString strServerName, strObject, strHeaders;
	INTERNET_PORT nPort;
	DWORD dwServiceType = 0;
	DWORD dwStatusCode = 0;

	// url파싱
	AfxParseURL(sURL, dwServiceType, strServerName, strObject, nPort);

	// http연결요청
	pSession = new CInternetSession;
	pHttpConnect = pSession->GetHttpConnection(strServerName);

	DWORD dwRet;
	CString hueStrObject;
	while (1)
	{

		BOOL bRet = true;
		HUECommand *pCommand = NULL;
		dwRet = WaitForSingleObject(event, INFINITE);
		if (pDlg->m_bHueThreadStop) {
			CloseHandle(event);
			// clean up
			while (pCommandQ->try_pop(pCommand))
			{
				delete pCommand;
			}
			break;
		}

		while (pCommandQ->try_pop(pCommand)) {

			hueStrObject.Format("%s%d/state", strObject, pCommand->hue_id);
			TRACE("Q Size : %d, Hue : %s\n", pCommandQ->unsafe_size(), pCommand->buffer);
			try
			{
				pHttpFile = pHttpConnect->OpenRequest(CHttpConnection::HTTP_VERB_PUT, hueStrObject, NULL);

				strHeaders = _T("Content-Type: application/x-www-form-urlencoded");
				pHttpFile->AddRequestHeaders(strHeaders);
				strHeaders = _T("User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; rv:11.0)  like Gecko");
				pHttpFile->AddRequestHeaders(strHeaders);

				bRet = pHttpFile->SendRequest(strHeaders, pCommand->buffer, pCommand->length);
				// 세션해제
				if (pHttpFile)
				{
					pHttpFile->Close();
					delete pHttpFile;
					pHttpFile = NULL;
				}
			}
			catch (CInternetException* pEx) {
				CString msg;
				msg.Format("CInternetException %x", pEx->m_dwError);
				TRACE(msg);
			}
			delete pCommand;
		}		
	}

	// clean up HUE Command
	HUECommand *pCommand = NULL;

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
	return 0;
}


void CAntMonDlg::FuncHueThread(bool bStart)
{
	if (bStart)
	{
		m_bHueThreadStop = false;
		m_hHueEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		AfxBeginThread(HueThread, this);
	}
	else {
		m_bHueThreadStop = true;
		SetEvent(m_hHueEvent);
	}
}

bool CAntMonDlg::AddHueCommand(HUECommand *pCommand)
{
	m_hueCommandQ.push(pCommand);
	SetEvent(m_hHueEvent);
	return true;
}


void CAntMonDlg::FuncService(bool bStart)
{
	if (bStart)
	{
		FuncHueThread(true);
		StartMon(0);
	} else {
		FuncHueThread(false);
		StopMon();
	}
}


void CAntMonDlg::InitRiderList()
{
	m_listRider.InsertColumn(0, "No", LVCFMT_LEFT, 30, -1);
	m_listRider.InsertColumn(1, "Name", LVCFMT_LEFT, 100, -1);
	m_listRider.InsertColumn(2, "Weight", LVCFMT_LEFT, 60, -1);
	m_listRider.InsertColumn(3, "PWR", LVCFMT_LEFT, 60, -1);
	m_listRider.InsertColumn(4, "PWR_ID", LVCFMT_LEFT, 60, -1);
	m_listRider.InsertColumn(5, "HRT", LVCFMT_LEFT, 60, -1);
	m_listRider.InsertColumn(6, "HRT_ID", LVCFMT_LEFT, 60, -1);
	m_listRider.InsertColumn(7, "Cadence", LVCFMT_LEFT, 60, -1);
	m_listRider.InsertColumn(8, "WPK", LVCFMT_LEFT, 60, -1);
	m_listRider.InsertColumn(9, "HUE", LVCFMT_LEFT, 60, -1);

}


int CAntMonDlg::ReadRiderFile()
{
	//CString iniFile = FILE_RIDER;
	char szBuffer[MAX_PATH];
	char iniFile[MAX_PATH];
	CString section;

	::GetCurrentDirectory(MAX_PATH, iniFile);
	strcat(iniFile, FILE_RIDER);

	for (int i = 0;i < MAX_RIDERS;i++) {
		RIDER *pRider = &m_Riders[i];
		section.Format("%02d", i + 1);
		m_listRider.InsertItem(i, section);
		
		GetPrivateProfileString(section, "name", "rider", szBuffer, sizeof(szBuffer), iniFile);
		m_listRider.SetItem(i, 1, LVIF_TEXT, szBuffer, -1, -1, -1, NULL);
		
		GetPrivateProfileString(section, "weight", "0", szBuffer, sizeof(szBuffer), iniFile);
		m_listRider.SetItem(i, 2, LVIF_TEXT, szBuffer, -1, -1, -1, NULL);

		GetPrivateProfileString(section, "ant_power_id", "0", szBuffer, sizeof(szBuffer), iniFile);
		m_listRider.SetItem(i, 4, LVIF_TEXT, szBuffer, -1, -1, -1, NULL);
		pRider->ant_power_id = atoi(szBuffer);

		GetPrivateProfileString(section, "ant_heart_id", "0", szBuffer, sizeof(szBuffer), iniFile);
		m_listRider.SetItem(i, 6, LVIF_TEXT, szBuffer, -1, -1, -1, NULL);
		pRider->ant_heart_id = atoi(szBuffer);

		GetPrivateProfileString(section, "weight", "1", szBuffer, sizeof(szBuffer), iniFile);
		m_listRider.SetItem(i, 2, LVIF_TEXT, szBuffer, -1, -1, -1, NULL);
		pRider->weight = atof(szBuffer);

		GetPrivateProfileString(section, "hue_id", "0", szBuffer, sizeof(szBuffer), iniFile);
		m_listRider.SetItem(i, 9, LVIF_TEXT, szBuffer, -1, -1, -1, NULL);
		pRider->hue_id = atoi(szBuffer);
	}
	return 0;
}


void CAntMonDlg::CreateDashBoard()
{
	CString strClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW, 0, (HBRUSH)(COLOR_WINDOW + 1));
	m_Dashboard.SetRiders(&m_Riders[0], MAX_RIDERS);
	CRect rect;
	GetDesktopWindow()->GetWindowRect(&rect);
	m_Dashboard.CreateEx(WS_EX_TOOLWINDOW,
		strClass, _T("CyleMon"), WS_POPUP, rect.left, rect.top, rect.Width(), rect.Height(), m_hWnd, NULL, NULL);
}


void CAntMonDlg::OnBnClickedButtonDashboard()
{
	m_Dashboard.ShowWindow(SW_SHOW);
}


void CAntMonDlg::ResetRider()
{
	memset(&m_rxMsg[0], sizeof(ANTMsg), MAX_DEVICES);
	for (int i = 0;i < MAX_RIDERS;i++) {
		RIDER *pRider = &m_Riders[i];
		pRider->heart = 0;
		pRider->power = 0;
		pRider->speed = 0;
		pRider->distance = 0;
		pRider->cadence = 0;
	}
}


int CAntMonDlg::FindRider(ANTMsg* pMsg)
{
	RIDER *pRider = NULL;
	for (int i = 0;i < MAX_RIDERS;i++) {
		pRider = &m_Riders[i];
		if (pRider->ant_heart_id == pMsg->deviceNo && pMsg->deviceType == ANT_TYPE_HEART) {
			return i;
		}
		if (pRider->ant_power_id == pMsg->deviceNo && (pMsg->deviceType == ANT_TYPE_PWR || pMsg->deviceType == ANT_TYPE_PWR2)) {
			return i;
		}
	}
	return -1;
}


void CAntMonDlg::UpdateRider(ANTMsg* pMsg, int nRider)
{
	char szBuffer[128];
	m_Dashboard.UpdateRider(nRider, pMsg->deviceType);
	
	if (pMsg->deviceType == ANT_TYPE_PWR || pMsg->deviceType == ANT_TYPE_PWR2) {
		sprintf_s(szBuffer, "%d", pMsg->pwrValue);
		m_listRider.SetItem(nRider, 3, LVIF_TEXT, szBuffer, -1, -1, -1, NULL);
		sprintf_s(szBuffer, "%d", pMsg->cadValue);
		m_listRider.SetItem(nRider, 7, LVIF_TEXT, szBuffer, -1, -1, -1, NULL);
	}

	if (pMsg->deviceType == ANT_TYPE_HEART) {
		sprintf_s(szBuffer, "%d", pMsg->hrBpm);
		m_listRider.SetItem(nRider, 5, LVIF_TEXT, szBuffer, -1, -1, -1, NULL);
	}
	
}


void CAntMonDlg::OnBnClickedOk()
{
	FuncService(false);
	CDialogEx::OnOK();
}


afx_msg LRESULT CAntMonDlg::OnAntMsg(WPARAM wParam, LPARAM lParam)
{
	UCHAR *pCommand = (UCHAR*)lParam;
	HandleMessage(pCommand);
	delete pCommand;
	return 0;
}
