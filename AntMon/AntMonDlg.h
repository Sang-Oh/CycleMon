
// AntMonDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include <concurrent_queue.h>
#include "afxcmn.h"
#include "DashBoard.h"
using namespace Concurrency;

#define RX_BUFFER_SIZE	64
#define MAX_DEVICES		128
#define HUEBUFFER_COUNT	64
#define HUEBUFFER_SIZE	256
#define FILE_RIDER		"\\rider.ini"
#define ANT_MSG_LENGTH	14

#define MAX_INTERVALS 500
typedef struct _ANTMsg {
	USHORT deviceNo;
	USHORT deviceType;
	
	USHORT hrBpm;
	USHORT hrCount;
	USHORT hrTime;

	USHORT pwrValue;
	USHORT pwrTime;
	USHORT pwrCount;

	USHORT cadValue;
} ANTMsg ;

typedef struct _HUECommand {
	char buffer[HUEBUFFER_SIZE];
	int length;
	int hue_id;
} HUECommand;


// CAntMonDlg dialog
class CAntMonDlg : public CDialogEx
{
// Construction
public:
	CAntMonDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANTMON_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonStart();
	static void ANTCallback(UCHAR ucEvent_, char* pcBuffer_);


	CString SendHTTPMsg(LPCTSTR pszUrl, int method, LPVOID body, DWORD bodyLen, LPCTSTR pszReferer/* = NULL*/,  LPCTSTR pszAppendHeader/* = NULL*/);
	ANTMsg m_rxMsg[MAX_DEVICES];

	void StartMon(int port);
	void StopMon();
	bool m_bOpened;
	CListBox m_listLog;
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnDestroy();
	void AddLog(LPCTSTR log);
	void HandleMessage(UCHAR *pBuffer);
	void WriteLog(UCHAR *pcBuffer);
	int AddMsgBuf(USHORT deviceNo, USHORT deviceType);
	void AddNewDevice(ANTMsg* pMsg);
	void ControlHUE(RIDER *pRider);

	// HUE Control
	static UINT HueThread(LPVOID _mothod);
	bool m_bHueThreadStop;
	CString m_strHueUrl;
	concurrent_queue< HUECommand* > m_hueCommandQ;
	void FuncHueThread(bool bStart);
	HANDLE m_hHueEvent;
	bool AddHueCommand(HUECommand* pCommand);
	
	void FuncService(bool bStart);
	CListCtrl m_listRider;
	RIDER m_Riders[MAX_RIDERS];
	int m_nSizeInterval;
	INTERVAL m_Intervals[MAX_INTERVALS];
	void InitRiderList();
	int ReadRiderFile();
	CDashBoard m_Dashboard;
	void CreateDashBoard();
	afx_msg void OnBnClickedButtonDashboard();
	void ResetRider();
	int FindRider(ANTMsg* pMsg);
	void UpdateRider(ANTMsg* pMsg, int nRider);

	afx_msg void OnBnClickedOk();
protected:
	afx_msg LRESULT OnAntMsg(WPARAM wParam, LPARAM lParam);
public:
	bool ReadIntervalFile(char* szFilePath);
};
