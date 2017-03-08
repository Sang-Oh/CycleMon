#pragma once
#include "afxwin.h"
#define ANT_TYPE_HEART	120
#define ANT_TYPE_PWR	17
typedef struct _RIDER {
	char name[256];
	float weight;
	USHORT ant_power_id;
	USHORT power;
	USHORT ant_heart_id;
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
	CFont m_fontNo;
	CFont m_fontPwr;
	CFont m_fontHrt;
	CFont m_fontSpd;
	CFont m_fontDis;
	CFont m_fontCad;
	int m_nRow;
	int m_nCol;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	int m_nCellHeight;
	int m_nCellWidth;
	int SetRiders(RIDER* pRiders,int nRiders);
	RIDER* m_pRiders;
	int m_nRiders;
	void UpdateRider(int nRider, int nSensor);
};


