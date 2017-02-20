
// AntMonDlg.h : header file
//

#pragma once
#include "afxwin.h"

#define RX_BUFFER_SIZE	64
#define MAX_DEVICES		64

typedef struct _ANTMsg {
	USHORT deviceNo;
	USHORT deviceType;
	USHORT heartRate;
} ANTMsg ;

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
	int AddMsgBuf(USHORT deviceNo, USHORT deviceType);
	void AddNewDevice(ANTMsg* pMsg);
	void ControlHUE(ANTMsg* pMsg);
};
