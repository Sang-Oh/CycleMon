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
	, m_cyTitle(0)
{
	for (int i = 0;i < MAX_RIDERS;i++) {
		m_nTopHeartHistoryIndex[i] = -1;
		m_timeLastHeartHistory[i] = 0;
		memset(&m_hisValHeart[i][0], 0, sizeof(USHORT)*MAX_HISTORY_LENGTH);
		m_hisMaxValHeart[i] = 0;
		m_hisMinValHeart[i] = 0xFFFF;
		m_hisAvgValHeart[i] = 0;

		m_nTopCadenceHistoryIndex[i] = -1;
		m_timeLastCadenceHistory[i] = 0;
		memset(&m_hisValCadence[i][0], 0, sizeof(USHORT)*MAX_HISTORY_LENGTH);
		m_hisMaxValCadence[i] = 0;
		m_hisMinValCadence[i] = 0xFFFF;
		m_hisAvgValCadence[i] = 0;

		m_nTopPowerHistoryIndex[i] = -1;
		m_timeLastPowerHistory[i] = 0;
		memset(&m_hisValPower[i][0], 0, sizeof(USHORT)*MAX_HISTORY_LENGTH);
		m_hisMaxValPower[i] = 0;
		m_hisMinValPower[i] = 0xFFFF;
		m_hisAvgValPower[i] = 0;
	}
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
	
	CPen *pOldPen = (CPen*)MemDC.GetCurrentPen();
	CBrush *pOldBrush = (CBrush*)MemDC.GetCurrentBrush();
	CFont *pOldFont = (CFont*)MemDC.GetCurrentFont();
	int oldBkMode = MemDC.GetBkMode();
	COLORREF oldTextColor = MemDC.GetTextColor();

	// background
	MemDC.SelectObject(m_brushBG);
	MemDC.SelectObject(m_penEmpty);
	MemDC.Rectangle(clientRect);

	MemDC.SetBkMode(TRANSPARENT);

	// title
	DrawTitle(MemDC);

	for (int i = 0; i < m_nRiders;i++) {
		DrawRider(i,MemDC);
	}
/*
	CRect rect;

	


	// draw white frame
	MemDC.SetBkMode(TRANSPARENT);
	MemDC.SelectStockObject(WHITE_PEN);
	for (int i = 0;i < m_nCol;i++) {
		for (int j = 0;j < m_nRow;j++) {
			int x = m_nCellWidth*i, y = m_nCellHeight*(m_nRow / 2 - j);
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
			int x = m_nCellWidth*i, y = m_nCellHeight*(m_nRow/2-j);

			rect.SetRect(x + 10, y + 10, x + m_nCellWidth, y + int(m_nCellHeight*0.25));
			sprintf_s(szBuf, "%02d", rider + 1);
			MemDC.DrawText(szBuf, rect, DT_SINGLELINE | DT_LEFT | DT_TOP);
		}
	}

	// draw power
	MemDC.SelectObject(m_fontPwr);
	MemDC.SetTextColor(RGB(255, 255, 0));
	for (int i = 0;i < m_nCol;i++) {
		for (int j = 0;j < m_nRow;j++) {
			int rider = m_nCol*j + i;
			int x = m_nCellWidth*i, y = m_nCellHeight*(m_nRow / 2 - j);
			rect.SetRect(x + 10, y + 10, x + m_nCellWidth, y + int(m_nCellHeight*0.5));

			sprintf_s(szBuf, "%d", m_pRiders[rider].power);
			MemDC.DrawText(szBuf, rect, DT_SINGLELINE | DT_CENTER | DT_BOTTOM);
		}
	}

	// draw hbt
	MemDC.SelectObject(m_fontHrt);
	MemDC.SetTextColor(RGB(255, 0, 0));
	for (int i = 0;i < m_nCol;i++) {
		for (int j = 0;j < m_nRow;j++) {
			int rider = m_nCol*j + i;
			int x = m_nCellWidth*i, y = m_nCellHeight*(m_nRow / 2 - j);
			rect.SetRect(x, y +  int(m_nCellHeight*0.5), x + int(m_nCellWidth*0.5), y + int(m_nCellHeight*0.75));

			sprintf_s(szBuf, "%d", m_pRiders[rider].heart);
			MemDC.DrawText(szBuf, rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		}
	}

	// draw cad
	MemDC.SelectObject(m_fontCad);
	MemDC.SetTextColor(RGB(0, 0, 255));
	for (int i = 0;i < m_nCol;i++) {
		for (int j = 0;j < m_nRow;j++) {
			int rider = m_nCol*j + i;
			int x = m_nCellWidth*i, y = m_nCellHeight*(m_nRow / 2 - j);
			rect.SetRect(x + int(m_nCellWidth*0.5), y + int(m_nCellHeight*0.5), x + m_nCellWidth, y + int(m_nCellHeight*0.75));

			sprintf_s(szBuf, "%d", m_pRiders[rider].cadence);
			MemDC.DrawText(szBuf, rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		}
	}
	
	// draw spd
	MemDC.SelectObject(m_fontSpd);
	MemDC.SetTextColor(RGB(255, 255, 255));
	for (int i = 0;i < m_nCol;i++) {
		for (int j = 0;j < m_nRow;j++) {
			int rider = m_nCol*j + i;
			int x = m_nCellWidth*i, y = m_nCellHeight*(m_nRow/2-j);
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
			int x = m_nCellWidth*i, y = m_nCellHeight*(m_nRow/2-j);
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
	
	m_cyTitle = int(lpCreateStruct->cy / 9);
	m_rectTitle.SetRect(10, 10, lpCreateStruct->cx-10, m_cyTitle-10);
	
	int nPW = 1;

	// color
	m_colorValue = RGB(255, 255, 255);
	m_colorUnit = RGB(150, 150, 150);
	m_colorBG = RGB(90, 179, 228);
	m_colorTitle = RGB(255, 255, 255);
	m_colorHeart = RGB(255, 28, 35);
	m_colorPwr = RGB(43, 175, 47);
	m_colorCad = RGB(233, 30, 99);

	m_nCellWidth = (lpCreateStruct->cx- nPW *2)/m_nCol;
	m_nCellHeight = (lpCreateStruct->cy- m_cyTitle) /m_nRow;

	m_fontTitle.CreateFont(int(m_cyTitle*0.8), 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "궁서체");
	m_fontValue.CreateFont(int(0.3*m_nCellHeight*0.7*0.8), 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "궁서체");
	m_fontUnit.CreateFont(int(0.3*m_nCellHeight*0.3*0.8), 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "궁서체");

	m_fontAvgValue.CreateFont(int(0.3*m_nCellHeight*0.3*0.5), 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "궁서체");
	m_fontMaxValue.CreateFont(int(0.3*m_nCellHeight*0.3*0.5), 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "궁서체");

	m_fontNo.CreateFont(int(0.5*m_nCellHeight*0.25), 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "궁서체");
	m_fontPwr.CreateFont(int(0.6*m_nCellHeight*0.5), 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "궁서체");
	m_fontHrt.CreateFont(int(0.3*m_nCellHeight*0.5), 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "궁서체");
	m_fontSpd.CreateFont(int(0.6*m_nCellHeight*0.25), 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "궁서체");
	m_fontDis.CreateFont(int(0.6*m_nCellHeight*0.25), 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "궁서체");
	m_fontCad.CreateFont(int(0.6*m_nCellHeight*0.25), 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "궁서체");

	m_penEmpty.CreatePen(PS_SOLID, 0, RGB(0, 0, 0));

	m_brushBG.CreateSolidBrush(m_colorBG);

	m_brushBGRider1.CreateSolidBrush(RGB(0, 0, 0));	//odd
	m_brushBGRider2.CreateSolidBrush(RGB(50, 50, 50)); //even

	m_brushNo.CreateSolidBrush(RGB(255, 255, 255));
	m_brushPwr.CreateSolidBrush(RGB(43, 175, 47));
	m_brushPwrChart.CreateSolidBrush(RGB(23, 85, 23));
	m_brushHrt.CreateSolidBrush(m_colorHeart);
	m_brushHrtChart.CreateSolidBrush(RGB(100, 0, 0));
	m_brushSpd.CreateSolidBrush(RGB(97, 179, 228));
	m_brushDis.CreateSolidBrush(RGB(255, 255, 0));
	m_brushCad.CreateSolidBrush(m_colorCad);
	m_brushCadChart.CreateSolidBrush(RGB(115, 15, 50));

	m_penBG.CreatePen(PS_SOLID, 1, m_colorBG);
	m_penBGRider.CreatePen(PS_SOLID, 1, m_colorBG);
	m_penHrt.CreatePen(PS_SOLID, 3, m_colorHeart);
	m_penHrtChart.CreatePen(PS_SOLID, 1, m_colorHeart);
	m_penPwr.CreatePen(PS_SOLID, 3, m_colorPwr);
	m_penPwrChart.CreatePen(PS_SOLID, 1, m_colorPwr);
	m_penCad.CreatePen(PS_SOLID, 3, m_colorCad);
	m_penCadChart.CreatePen(PS_SOLID, 1, m_colorCad);

	// alloc rider rectangle
	for (int i = 0;i < m_nCol;i++) {
		for (int j = 0;j < m_nRow;j++) {
			int rider = m_nCol*j + i;
			int l = nPW + m_nCellWidth*i,
				t = m_cyTitle + m_nCellHeight*(m_nRow / 2 - j);
			int r = l + m_nCellWidth, b = t + m_nCellHeight;
			
			if (i == (m_nCol - 1)) r = lpCreateStruct->cx - 1;
			//if (j == (m_nRow - 1)) b = lpCreateStruct->cy - 1;

			m_rectRiders[rider].SetRect(l, t, r, b);
		}
	}

	CRect *pRect = &m_rectRiders[0];
	m_nPH = 5;
	m_nPW = 5;
	m_nTH = 4;
	m_rowH = int(pRect->Height() / 3),
		m_cyValue = int(0.7*(pRect->Height() / 3)),
		m_cxValue = int(pRect->Width() / 2),
		m_cyUnit = int(0.3*(pRect->Height() / 3)),
		m_cxUnit = int(pRect->Width() / 2);

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
	if (nSensor == ANT_TYPE_HEART) {
		AddHeartHistory(nRider);
		AddPowerHistory(nRider);
		AddCadenceHistory(nRider);
	}
		
	InvalidateRect(m_rectRiders[nRider]);
}


void CDashBoard::DrawRider(int nRider, CDC& dc)
{
	CRect *pRect = &m_rectRiders[nRider];
	RIDER *pRider = &m_pRiders[nRider];
	POINT roundPt = { 20, 20};

	CRect rect;

	if (nRider > 3) {
		dc.SelectObject(nRider%2?m_brushBGRider1: m_brushBGRider2);
	}
	else {
		dc.SelectObject(nRider % 2 ? m_brushBGRider2 : m_brushBGRider1);
	}
	
	dc.SelectObject(m_penEmpty);
	dc.Rectangle(pRect);

	DrawHeartChart(nRider, dc);

	DrawPowerChart(nRider, dc);

	DrawCadenceChart(nRider, dc);

}


void CDashBoard::DrawTitle(CDC& dc)
{
	dc.SelectObject(m_fontTitle);
	dc.SetTextColor(m_colorTitle);
	dc.DrawText("Cyclepark", m_rectTitle, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
}


void CDashBoard::AddHeartHistory(int rider)
{
	__time64_t now = CTime::GetCurrentTime().GetTime();
	__time64_t last = m_timeLastHeartHistory[rider];
	long diff = now - last;
	int topIndex = m_nTopHeartHistoryIndex[rider];
	int nNextIndex = GetNextHistoryIndex(topIndex);
	USHORT *pValues = &m_hisValHeart[rider][0];
	if (topIndex != -1) {
		for (long i = 0;i < diff;i++) { // if empty value by second
			pValues[nNextIndex] = m_pRiders[rider].heart;
			nNextIndex = GetNextHistoryIndex(nNextIndex);
		}
	}
	pValues[nNextIndex] = m_pRiders[rider].heart;
	m_nTopHeartHistoryIndex[rider] = nNextIndex;
	m_timeLastHeartHistory[rider] = now;

	// min max
	m_hisMinValHeart[rider] = m_hisMinValHeart[rider] > m_pRiders[rider].heart ? m_pRiders[rider].heart : m_hisMinValHeart[rider];
	m_hisMaxValHeart[rider] = m_hisMaxValHeart[rider] < m_pRiders[rider].heart ? m_pRiders[rider].heart : m_hisMaxValHeart[rider];
	ULONG sum = 0, count=0, value;
	for (int i = 0;i<MAX_HISTORY_LENGTH;i++) {
		value = pValues[nNextIndex];
		if (value != 0) {
			count++;
			sum += value;
		}
		nNextIndex = GetNextHistoryIndex(nNextIndex);
	}
	m_hisAvgValHeart[rider] = (USHORT)(sum / count);

}

void CDashBoard::AddPowerHistory(int rider)
{
	__time64_t now = CTime::GetCurrentTime().GetTime();
	__time64_t last = m_timeLastPowerHistory[rider];
	long diff = now - last;
	int topIndex = m_nTopPowerHistoryIndex[rider];
	int nNextIndex = GetNextHistoryIndex(topIndex);
	USHORT *pValues = &m_hisValPower[rider][0];
	if (topIndex != -1) {
		for (long i = 0;i < diff;i++) { // if empty value by second
			pValues[nNextIndex] = m_pRiders[rider].power;
			nNextIndex = GetNextHistoryIndex(nNextIndex);
		}
	}
	pValues[nNextIndex] = m_pRiders[rider].power;
	m_nTopPowerHistoryIndex[rider] = nNextIndex;
	m_timeLastPowerHistory[rider] = now;

	// min max
	m_hisMinValPower[rider] = m_hisMinValPower[rider] > m_pRiders[rider].power ? m_pRiders[rider].power : m_hisMinValPower[rider];
	m_hisMaxValPower[rider] = m_hisMaxValPower[rider] < m_pRiders[rider].power ? m_pRiders[rider].power : m_hisMaxValPower[rider];
	ULONG sum = 0, count = 0, value;
	for (int i = 0;i<MAX_HISTORY_LENGTH;i++) {
		value = pValues[nNextIndex];
		if (value != 0) {
			count++;
			sum += value;
		}
		nNextIndex = GetNextHistoryIndex(nNextIndex);
	}
	m_hisAvgValPower[rider] = (USHORT)(sum / count);

}

void CDashBoard::AddCadenceHistory(int rider)
{
	__time64_t now = CTime::GetCurrentTime().GetTime();
	__time64_t last = m_timeLastCadenceHistory[rider];
	long diff = now - last;
	int topIndex = m_nTopCadenceHistoryIndex[rider];
	int nNextIndex = GetNextHistoryIndex(topIndex);
	USHORT *pValues = &m_hisValCadence[rider][0];
	if (topIndex != -1) {
		for (long i = 0;i < diff;i++) { // if empty value by second
			pValues[nNextIndex] = m_pRiders[rider].cadence;
			nNextIndex = GetNextHistoryIndex(nNextIndex);
		}
	}
	pValues[nNextIndex] = m_pRiders[rider].cadence;
	m_nTopCadenceHistoryIndex[rider] = nNextIndex;
	m_timeLastCadenceHistory[rider] = now;

	// min max
	m_hisMinValCadence[rider] = m_hisMinValCadence[rider] > m_pRiders[rider].cadence ? m_pRiders[rider].cadence : m_hisMinValCadence[rider];
	m_hisMaxValCadence[rider] = m_hisMaxValCadence[rider] < m_pRiders[rider].cadence ? m_pRiders[rider].cadence : m_hisMaxValCadence[rider];
	ULONG sum = 0, count = 0, value;
	for (int i = 0;i<MAX_HISTORY_LENGTH;i++) {
		value = pValues[nNextIndex];
		if (value != 0) {
			count++;
			sum += value;
		}
		nNextIndex = GetNextHistoryIndex(nNextIndex);
	}
	m_hisAvgValCadence[rider] = (USHORT)(sum / count);

}

int CDashBoard::GetPrevHistoryIndex(int nIndex)
{
	int nPrevIndex = nIndex-1;
	return nPrevIndex<0?MAX_HISTORY_LENGTH-1:nPrevIndex;
}


int CDashBoard::GetNextHistoryIndex(int nIndex)
{
	int nNextIndex = nIndex + 1;
	return nNextIndex>MAX_HISTORY_LENGTH - 1?0: nNextIndex;
}

void CDashBoard::DrawHeartChart(int nRider, CDC& dc)
{
	CRect *pRect = &m_rectRiders[nRider];
	RIDER *pRider = &m_pRiders[nRider];

	// heart under line
	int x = pRect->left, y = pRect->top;

	dc.SelectObject(m_penHrt);

	/////////////////////////////
	// heart
	// under line 
	dc.MoveTo(x + m_nPW, y + m_rowH - m_nPH);
	dc.LineTo(x + m_cxValue - m_nPW, y + m_rowH - m_nPH);

	dc.MoveTo(x + m_cxValue, y + m_rowH - m_nPH);
	dc.LineTo(pRect->right, y + m_rowH - m_nPH);

	// draw heart line
	CRect rect(
		pRect->right - MAX_HISTORY_LENGTH,
		y,
		pRect->right,
		y + m_rowH - m_nPH
	);

	USHORT *pHistory = &m_hisValHeart[nRider][0];
	int nTopIndex = m_nTopHeartHistoryIndex[nRider];
	CString log = "";
	if (nTopIndex != -1) {
		int chartValue = pHistory[nTopIndex];
		int posValue = 0;

		
		m_pointHistory[MAX_HISTORY_LENGTH].x = rect.left;
		m_pointHistory[MAX_HISTORY_LENGTH].y = rect.bottom-1;

		m_pointHistory[MAX_HISTORY_LENGTH + 1].x = rect.right-1;
		m_pointHistory[MAX_HISTORY_LENGTH + 1].y = rect.bottom-1;

		int nPrevIndex = GetNextHistoryIndex(nTopIndex);
		for (int i = 0;i < MAX_HISTORY_LENGTH;i++) {
			chartValue = pHistory[nPrevIndex];

			posValue = GetChartPosValue(chartValue, m_hisMaxValHeart[nRider], m_hisMinValHeart[nRider], rect.Height());
			m_pointHistory[i].x = rect.right - i;
			m_pointHistory[i].y = rect.bottom - posValue;

			nPrevIndex = GetPrevHistoryIndex(nPrevIndex);
		}

		dc.SelectObject(m_brushHrtChart);
		dc.SelectObject(m_penHrtChart);
		dc.Polygon(m_pointHistory, sizeof(m_pointHistory) / sizeof(m_pointHistory[0]));

		// min , max		
		dc.SetTextColor(m_colorValue);
		dc.SelectObject(m_fontMaxValue);

		rect.SetRect(
			pRect->left + m_cxValue,
			y,
			pRect->right,
			y + m_rowH - m_nPH
		);
		sprintf_s(m_szBuf, "%d", m_hisMaxValHeart[nRider]);
		dc.DrawText(m_szBuf, rect, DT_SINGLELINE | DT_LEFT | DT_TOP);

		sprintf_s(m_szBuf, "%d", m_hisAvgValHeart[nRider]);
		dc.DrawText(m_szBuf, rect, DT_SINGLELINE | DT_LEFT | DT_VCENTER);

		sprintf_s(m_szBuf, "%d", m_hisMinValHeart[nRider]);
		dc.DrawText(m_szBuf, rect, DT_SINGLELINE | DT_LEFT | DT_BOTTOM);
	}
	// value
	sprintf_s(m_szBuf, "%d", pRider->heart);
	rect.SetRect(
		x,
		y,
		x + m_cxValue,
		y + m_cyValue
	);
	dc.SetTextColor(m_colorValue);
	dc.SelectObject(m_fontValue);
	dc.DrawText(m_szBuf, rect, DT_SINGLELINE | DT_CENTER | DT_BOTTOM);
	// unit
	rect.SetRect(
		x,
		y + m_cyValue,
		x + m_cxUnit,
		y + m_cyValue + m_cyUnit
	);
	dc.SetTextColor(m_colorUnit);
	dc.SelectObject(m_fontUnit);
	dc.DrawText("bpm", rect, DT_SINGLELINE | DT_CENTER | DT_TOP);
}

void CDashBoard::DrawPowerChart(int nRider, CDC& dc)
{
	CRect *pRect = &m_rectRiders[nRider];
	RIDER *pRider = &m_pRiders[nRider];

	// Power under line
	int x = pRect->left, y = pRect->top + m_rowH;

	dc.SelectObject(m_penPwr);

	/////////////////////////////
	// Power
	// under line 
	dc.MoveTo(x + m_nPW, y + m_rowH - m_nPH);
	dc.LineTo(x + m_cxValue - m_nPW, y + m_rowH - m_nPH);

	dc.MoveTo(x + m_cxValue, y + m_rowH - m_nPH);
	dc.LineTo(pRect->right, y + m_rowH - m_nPH);

	// draw Power line
	CRect rect(
		pRect->right - MAX_HISTORY_LENGTH,
		y,
		pRect->right,
		y + m_rowH - m_nPH
	);

	USHORT *pHistory = &m_hisValPower[nRider][0];
	int nTopIndex = m_nTopPowerHistoryIndex[nRider];
	CString log = "";
	if (nTopIndex != -1) {
		int chartValue = pHistory[nTopIndex];
		int posValue = 0;

		m_pointHistory[MAX_HISTORY_LENGTH].x = rect.left;
		m_pointHistory[MAX_HISTORY_LENGTH].y = rect.bottom - 1;

		m_pointHistory[MAX_HISTORY_LENGTH + 1].x = rect.right - 1;
		m_pointHistory[MAX_HISTORY_LENGTH + 1].y = rect.bottom - 1;

		int nPrevIndex = GetNextHistoryIndex(nTopIndex);
		for (int i = 0;i < MAX_HISTORY_LENGTH;i++) {
			chartValue = pHistory[nPrevIndex];

			posValue = GetChartPosValue(chartValue, m_hisMaxValPower[nRider], m_hisMinValPower[nRider], rect.Height());
			m_pointHistory[i].x = rect.right - i;
			m_pointHistory[i].y = rect.bottom - posValue;

			nPrevIndex = GetPrevHistoryIndex(nPrevIndex);
		}

		dc.SelectObject(m_brushPwrChart);
		dc.SelectObject(m_penPwrChart);
		dc.Polygon(m_pointHistory, sizeof(m_pointHistory) / sizeof(m_pointHistory[0]));

		// min , max		
		dc.SetTextColor(m_colorValue);
		dc.SelectObject(m_fontMaxValue);

		rect.SetRect(
			pRect->left + m_cxValue,
			y,
			pRect->right,
			y + m_rowH - m_nPH
		);
		sprintf_s(m_szBuf, "%d", m_hisMaxValPower[nRider]);
		dc.DrawText(m_szBuf, rect, DT_SINGLELINE | DT_LEFT | DT_TOP);

		sprintf_s(m_szBuf, "%d", m_hisAvgValPower[nRider]);
		dc.DrawText(m_szBuf, rect, DT_SINGLELINE | DT_LEFT | DT_VCENTER);

		sprintf_s(m_szBuf, "%d", m_hisMinValPower[nRider]);
		dc.DrawText(m_szBuf, rect, DT_SINGLELINE | DT_LEFT | DT_BOTTOM);
	}
	// value
	sprintf_s(m_szBuf, "%d", pRider->power);
	rect.SetRect(
		x,
		y,
		x + m_cxValue,
		y + m_cyValue
	);
	dc.SetTextColor(m_colorValue);
	dc.SelectObject(m_fontValue);
	dc.DrawText(m_szBuf, rect, DT_SINGLELINE | DT_CENTER | DT_BOTTOM);
	// unit
	rect.SetRect(
		x,
		y + m_cyValue,
		x + m_cxUnit,
		y + m_cyValue + m_cyUnit
	);
	dc.SetTextColor(m_colorUnit);
	dc.SelectObject(m_fontUnit);
	dc.DrawText("watt", rect, DT_SINGLELINE | DT_CENTER | DT_TOP);
}

void CDashBoard::DrawCadenceChart(int nRider, CDC& dc)
{
	CRect *pRect = &m_rectRiders[nRider];
	RIDER *pRider = &m_pRiders[nRider];

	// Cadence under line
	int x = pRect->left, y = pRect->top + m_rowH*2;

	dc.SelectObject(m_penCad);

	/////////////////////////////
	// Cadence
	// under line 
	dc.MoveTo(x + m_nPW, y + m_rowH - m_nPH);
	dc.LineTo(x + m_cxValue - m_nPW, y + m_rowH - m_nPH);

	dc.MoveTo(x + m_cxValue, y + m_rowH - m_nPH);
	dc.LineTo(pRect->right, y + m_rowH - m_nPH);

	// draw Cadence line
	CRect rect(
		pRect->right - MAX_HISTORY_LENGTH,
		y,
		pRect->right,
		y + m_rowH - m_nPH
	);

	USHORT *pHistory = &m_hisValCadence[nRider][0];
	int nTopIndex = m_nTopCadenceHistoryIndex[nRider];
	CString log = "";
	if (nTopIndex != -1) {
		int chartValue = pHistory[nTopIndex];
		int posValue = 0;


		m_pointHistory[MAX_HISTORY_LENGTH].x = rect.left;
		m_pointHistory[MAX_HISTORY_LENGTH].y = rect.bottom - 1;

		m_pointHistory[MAX_HISTORY_LENGTH + 1].x = rect.right - 1;
		m_pointHistory[MAX_HISTORY_LENGTH + 1].y = rect.bottom - 1;

		int nPrevIndex = GetNextHistoryIndex(nTopIndex);
		for (int i = 0;i < MAX_HISTORY_LENGTH;i++) {
			chartValue = pHistory[nPrevIndex];

			posValue = GetChartPosValue(chartValue, m_hisMaxValCadence[nRider], m_hisMinValCadence[nRider], rect.Height());
			m_pointHistory[i].x = rect.right - i;
			m_pointHistory[i].y = rect.bottom - posValue;

			nPrevIndex = GetPrevHistoryIndex(nPrevIndex);
		}

		dc.SelectObject(m_brushCadChart);
		dc.SelectObject(m_penCadChart);
		dc.Polygon(m_pointHistory, sizeof(m_pointHistory) / sizeof(m_pointHistory[0]));

		// min , max		
		dc.SetTextColor(m_colorValue);
		dc.SelectObject(m_fontMaxValue);

		rect.SetRect(
			pRect->left + m_cxValue,
			y,
			pRect->right,
			y + m_rowH - m_nPH
		);
		sprintf_s(m_szBuf, "%d", m_hisMaxValCadence[nRider]);
		dc.DrawText(m_szBuf, rect, DT_SINGLELINE | DT_LEFT | DT_TOP);

		sprintf_s(m_szBuf, "%d", m_hisAvgValCadence[nRider]);
		dc.DrawText(m_szBuf, rect, DT_SINGLELINE | DT_LEFT | DT_VCENTER);

		sprintf_s(m_szBuf, "%d", m_hisMinValCadence[nRider]);
		dc.DrawText(m_szBuf, rect, DT_SINGLELINE | DT_LEFT | DT_BOTTOM);
	}
	// value
	sprintf_s(m_szBuf, "%d", pRider->cadence);
	rect.SetRect(
		x,
		y,
		x + m_cxValue,
		y + m_cyValue
	);
	dc.SetTextColor(m_colorValue);
	dc.SelectObject(m_fontValue);
	dc.DrawText(m_szBuf, rect, DT_SINGLELINE | DT_CENTER | DT_BOTTOM);
	// unit
	rect.SetRect(
		x,
		y + m_cyValue,
		x + m_cxUnit,
		y + m_cyValue + m_cyUnit
	);
	dc.SetTextColor(m_colorUnit);
	dc.SelectObject(m_fontUnit);
	dc.DrawText("rpm", rect, DT_SINGLELINE | DT_CENTER | DT_TOP);
}

int CDashBoard::GetChartPosValue(int value, int max, int min, int chartHeight)
{
	int chartMin = int(min*0.9), chartMax = int(max*1.1);
	int posValue = int(chartHeight * (value - chartMin) / (chartMax - chartMin));
	posValue = posValue < 0 ? 0 : posValue;

	return posValue;
}
