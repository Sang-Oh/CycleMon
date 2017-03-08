// DashBoard.cpp : implementation file
//

#include "stdafx.h"
#include "AntMon.h"
#include "DashBoard.h"

// CDashBoard

IMPLEMENT_DYNAMIC(CDashBoard, CWnd)

CDashBoard::CDashBoard()
	: m_nRow(2)
	, m_nCol(4)
	, m_nCellHeight(0)
	, m_nCellWidth(0)
	, m_pRiders(NULL)
	, m_nRiders(0)
{

}

CDashBoard::~CDashBoard()
{
}


BEGIN_MESSAGE_MAP(CDashBoard, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_KEYUP()
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CDashBoard message handlers




void CDashBoard::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect clientRect;
	GetClientRect(clientRect);

	CDC MemDC;  //메모리 DC   
	CBitmap* pOldBmp; //기존의 오브젝트를 저장할 변수   
	CBitmap Bitmap; //메모리 DC로 사용할 새 오브젝트  


	MemDC.CreateCompatibleDC(&dc); // 메모리 DC를 원본 DC와 같게 설정한다.( 생성한다. )  
	Bitmap.CreateCompatibleBitmap(&dc, clientRect.Width(), clientRect.Height());  // 메모리 DC로 사용
	pOldBmp = (CBitmap*)MemDC.SelectObject(&Bitmap);
	
	char szBuf[64];

	

	CPen *pOldPen = (CPen*)MemDC.GetCurrentPen();
	CBrush *pOldBrush = (CBrush*)MemDC.GetCurrentBrush();
	CFont *pOldFont = (CFont*)MemDC.GetCurrentFont();
	int oldBkMode = MemDC.GetBkMode();
	COLORREF oldTextColor = MemDC.GetTextColor();
	CRect rect;

	
	// background black
	MemDC.SelectStockObject(BLACK_BRUSH);
	MemDC.Rectangle(clientRect);

	// draw white frame
	MemDC.SetBkMode(TRANSPARENT);
	MemDC.SelectStockObject(WHITE_PEN);
	for (int i = 0;i < m_nCol;i++) {
		for (int j = 0;j < m_nRow;j++) {
			int x = m_nCellWidth*i, y = m_nCellHeight*j;
			rect.SetRect(x, y, x + m_nCellWidth, y + m_nCellHeight);
			MemDC.Rectangle(rect);
		}
	}

	// draw rider no
	MemDC.SelectObject(m_fontNo);
	MemDC.SetTextColor(RGB(255, 255, 255));
	for (int i = 0;i < m_nCol;i++) {
		for (int j = 0;j < m_nRow;j++) {
			int rider = m_nCol*j + i;
			int x = m_nCellWidth*i, y = m_nCellHeight*j;

			rect.SetRect(x + 10, y + 10, x + m_nCellWidth, y + m_nCellHeight*0.25);
			sprintf(szBuf, "%02d", rider + 1);
			MemDC.DrawText(szBuf, rect, DT_SINGLELINE | DT_LEFT | DT_TOP);
		}
	}

	// draw power
	MemDC.SelectObject(m_fontPwr);
	MemDC.SetTextColor(RGB(255, 255, 0));
	for (int i = 0;i < m_nCol;i++) {
		for (int j = 0;j < m_nRow;j++) {
			int rider = m_nCol*j + i;
			int x = m_nCellWidth*i, y = m_nCellHeight*j;
			rect.SetRect(x + 10, y + 10, x + m_nCellWidth, y + m_nCellHeight*0.5);

			sprintf(szBuf, "%d", m_pRiders[rider].power);
			MemDC.DrawText(szBuf, rect, DT_SINGLELINE | DT_CENTER | DT_BOTTOM);
		}
	}

	// draw hbt
	MemDC.SelectObject(m_fontHrt);
	MemDC.SetTextColor(RGB(255, 0, 0));
	for (int i = 0;i < m_nCol;i++) {
		for (int j = 0;j < m_nRow;j++) {
			int rider = m_nCol*j + i;
			int x = m_nCellWidth*i, y = m_nCellHeight*j;
			rect.SetRect(x, y +  m_nCellHeight*0.5, x + m_nCellWidth*0.5, y + m_nCellHeight*0.75);

			sprintf(szBuf, "%d", m_pRiders[rider].heart);
			MemDC.DrawText(szBuf, rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		}
	}

	// draw cad
	MemDC.SelectObject(m_fontCad);
	MemDC.SetTextColor(RGB(0, 0, 255));
	for (int i = 0;i < m_nCol;i++) {
		for (int j = 0;j < m_nRow;j++) {
			int rider = m_nCol*j + i;
			int x = m_nCellWidth*i, y = m_nCellHeight*j;
			rect.SetRect(x + m_nCellWidth*0.5, y + m_nCellHeight*0.5, x + m_nCellWidth, y + m_nCellHeight*0.75);

			sprintf(szBuf, "%d", m_pRiders[rider].cadence);
			MemDC.DrawText(szBuf, rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		}
	}
	/*
	// draw spd
	MemDC.SelectObject(m_fontSpd);
	MemDC.SetTextColor(RGB(255, 255, 255));
	for (int i = 0;i < m_nCol;i++) {
		for (int j = 0;j < m_nRow;j++) {
			int rider = m_nCol*j + i;
			int x = m_nCellWidth*i, y = m_nCellHeight*j;
			rect.SetRect(x, y + m_nCellHeight*0.75, x + m_nCellWidth*0.5, y + m_nCellHeight);

			sprintf(szBuf, "%.1f", m_pRiders[rider].speed);
			MemDC.DrawText(szBuf, rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		}
	}

	// draw dis
	MemDC.SelectObject(m_fontDis);
	MemDC.SetTextColor(RGB(255, 255, 255));
	for (int i = 0;i < m_nCol;i++) {
		for (int j = 0;j < m_nRow;j++) {
			int rider = m_nCol*j + i;
			int x = m_nCellWidth*i, y = m_nCellHeight*j;

			rect.SetRect(x + m_nCellWidth*0.5, y + m_nCellHeight*0.75, x + m_nCellWidth, y + m_nCellHeight);

			sprintf(szBuf, "%.1f", m_pRiders[rider].distance);
			MemDC.DrawText(szBuf, rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		}
	}
	*/
	dc.BitBlt(0, 0, clientRect.Width(), clientRect.Height(), &MemDC, 0, 0, SRCCOPY);
	
	MemDC.SelectObject(pOldPen);
	MemDC.SelectObject(pOldBrush);
	MemDC.SelectObject(pOldFont);
	MemDC.SetBkMode(oldBkMode);
	MemDC.SetTextColor(oldTextColor);
	
	MemDC.SelectObject(pOldBmp);

	MemDC.DeleteDC();
	Bitmap.DeleteObject();
}


BOOL CDashBoard::OnEraseBkgnd(CDC* pDC)
{
	return 0;	//return CWnd::OnEraseBkgnd(pDC);
}


void CDashBoard::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_ESCAPE) this->ShowWindow(SW_HIDE);

	CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}


int CDashBoard::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_nCellWidth = lpCreateStruct->cx/m_nCol;
	m_nCellHeight = lpCreateStruct->cy/m_nRow;

	m_fontNo.CreateFont(0.5*m_nCellHeight*0.25, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "궁서체");
	m_fontPwr.CreateFont(0.6*m_nCellHeight*0.5, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "궁서체");
	m_fontHrt.CreateFont(0.6*m_nCellHeight*0.25, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "궁서체");
	m_fontSpd.CreateFont(0.6*m_nCellHeight*0.25, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "궁서체");
	m_fontDis.CreateFont(0.6*m_nCellHeight*0.25, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "궁서체");
	m_fontCad.CreateFont(0.6*m_nCellHeight*0.25, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "궁서체");

	return 0;
}


int CDashBoard::SetRiders(RIDER* pRiders, int nRiders)
{
	m_pRiders = pRiders;
	m_nRiders = nRiders;
	return 0;
}


void CDashBoard::UpdateRider(int nRider, int nSensor)
{
	this->Invalidate();
	/*
	CRect rect(m_nCellWidth*(nRider / m_nCol),
		m_nCellHeight*(nRider%m_nRow),
		m_nCellWidth*((nRider / m_nCol) + 1),
		m_nCellHeight*((nRider%m_nRow) + 1));

	this->InvalidateRect(rect);
	*/
}
