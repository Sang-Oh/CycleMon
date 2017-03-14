#pragma once
#include "afxwin.h"
#include "atltypes.h"
#define ANT_TYPE_HEART	120
#define ANT_TYPE_PWR	11
#define ANT_TYPE_PWR2	121

#define MAX_RIDERS		10

#define MAX_HISTORY_LENGTH 120

typedef struct _RIDER {
	char name[256];
	double weight;
	USHORT ant_power_id;
	long power_time;
	USHORT power;
	USHORT ant_heart_id;
	long heart_time;
	USHORT heart;
	USHORT hue_id;
	float speed;
	float distance;
	USHORT cadence;
} RIDER;

// CDashBoard
class CDashBoard : public CWnd
{
	DECLARE_DYNAMIC(CDashBoard)

public:
	CDashBoard();
	virtual ~CDashBoard();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	
	CFont m_fontTitle;
	CFont m_fontNo;
	CFont m_fontPwr;
	CFont m_fontHrt;
	CFont m_fontSpd;
	CFont m_fontDis;
	CFont m_fontCad;

	CFont m_fontValue;
	CFont m_fontUnit;

	CFont m_fontMaxValue;
	CFont m_fontAvgValue;

	CBrush m_brushBGRider1;
	CBrush m_brushBGRider2;
	CBrush m_brushBG;
	CBrush m_brushNo;
	CBrush m_brushPwr;
	CBrush m_brushPwrChart;
	CBrush m_brushHrt;
	CBrush m_brushHrtChart;
	CBrush m_brushSpd;
	CBrush m_brushDis;
	CBrush m_brushCad;
	CBrush m_brushCadChart;


	CPen m_penEmpty;
	CPen m_penHrt;
	CPen m_penHrtChart;
	CPen m_penPwr;
	CPen m_penPwrChart;
	CPen m_penCad;
	CPen m_penCadChart;
	CPen m_penBG;
	CPen m_penBGRider;

	COLORREF m_colorTitle;
	COLORREF m_colorBG;
	COLORREF m_colorValue;
	COLORREF m_colorUnit;
	COLORREF m_colorHeart;
	COLORREF m_colorPwr;
	COLORREF m_colorCad;

	char m_szBuf[64];

	int m_nRow;
	int m_nCol;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	int m_nCellHeight;
	int m_nCellWidth;
	int SetRiders(RIDER* pRiders,int nRiders);
	RIDER* m_pRiders;
	int m_nRiders;
	CRect m_rectRiders[MAX_RIDERS];
	void UpdateRider(int nRider, int nSensor);
	void DrawRider(int nRider, CDC& dc);
	void DrawTitle(CDC& dc);
	int m_cyTitle;
	CRect m_rectTitle;

	int m_nPH;
	int m_nPW;
	int m_nTH;
	int m_rowH;
	int m_cyValue;
	int m_cxValue;
	int m_cyUnit;
	int m_cxUnit;

	USHORT	m_hisValHeart[MAX_RIDERS][MAX_HISTORY_LENGTH];
	int	m_nTopHeartHistoryIndex[MAX_HISTORY_LENGTH];
	__time64_t m_timeLastHeartHistory[MAX_RIDERS];
	USHORT	m_hisMaxValHeart[MAX_RIDERS];
	USHORT	m_hisMinValHeart[MAX_RIDERS];
	USHORT	m_hisAvgValHeart[MAX_RIDERS];

	USHORT	m_hisValPower[MAX_RIDERS][MAX_HISTORY_LENGTH];
	int	m_nTopPowerHistoryIndex[MAX_HISTORY_LENGTH];
	__time64_t m_timeLastPowerHistory[MAX_RIDERS];
	USHORT	m_hisMaxValPower[MAX_RIDERS];
	USHORT	m_hisMinValPower[MAX_RIDERS];
	USHORT	m_hisAvgValPower[MAX_RIDERS];

	USHORT	m_hisValCadence[MAX_RIDERS][MAX_HISTORY_LENGTH];
	int	m_nTopCadenceHistoryIndex[MAX_HISTORY_LENGTH];
	__time64_t m_timeLastCadenceHistory[MAX_RIDERS];
	USHORT	m_hisMaxValCadence[MAX_RIDERS];
	USHORT	m_hisMinValCadence[MAX_RIDERS];
	USHORT	m_hisAvgValCadence[MAX_RIDERS];

	POINT m_pointHistory[MAX_HISTORY_LENGTH+2];

	void AddHeartHistory(int rider);
	void AddPowerHistory(int rider);
	void AddCadenceHistory(int rider);

	int GetPrevHistoryIndex(int nIndex);
	int GetNextHistoryIndex(int nIndex);
	void DrawHeartChart(int nRider, CDC& dc);
	void DrawPowerChart(int nRider, CDC& dc);
	void DrawCadenceChart(int nRider, CDC& dc);

	int GetChartPosValue(int value, int max, int min, int height);
	void DrawRiderNo(int nRider, CDC& dc);
};


