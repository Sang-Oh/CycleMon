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

	m_fontAvgValue.CreateFont(int(0.3*m_nCellHeight*0.3*0.7), 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "궁서체");
	m_fontMaxValue.CreateFont(int(0.3*m_nCellHeight*0.3*0.7), 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "궁서체");

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
	m_brushPwr.CreateSolidBrush(RGB(100, 0, 0));
	m_brushHrt.CreateSolidBrush(m_colorHeart);
	m_brushSpd.CreateSolidBrush(RGB(97, 179, 228));
	m_brushDis.CreateSolidBrush(RGB(255, 255, 0));
	m_brushCad.CreateSolidBrush(m_colorCad);

	m_penBG.CreatePen(PS_SOLID, 1, m_colorBG);
	m_penBGRider.CreatePen(PS_SOLID, 1, m_colorBG);
	m_penHeart.CreatePen(PS_SOLID, 3, m_colorHeart);
	m_penPwr.CreatePen(PS_SOLID, 3, m_colorPwr);
	m_penCad.CreatePen(PS_SOLID, 3, m_colorCad);

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
	}
		
	InvalidateRect(m_rectRiders[nRider]);
}


void CDashBoard::DrawRider(int nRider, CDC& dc)
{
	CRect *pRect = &m_rectRiders[nRider];
	RIDER *pRider = &m_pRiders[nRider];
	POINT roundPt = { 20, 20};
	char szBuf[64];
	CRect rect;

	if (nRider > 3) {
		dc.SelectObject(nRider%2?m_brushBGRider1: m_brushBGRider2);
	}
	else {
		dc.SelectObject(nRider % 2 ? m_brushBGRider2 : m_brushBGRider1);
	}
	
	dc.SelectObject(m_penEmpty);
	dc.Rectangle(pRect);

	// heart under line
	int nPH = 5, nPW = 5;
	int nTH = 4;
	int x = pRect->left,
		y = pRect->top, 
		rowH = int(pRect->Height() / 3), 
		cyValue = int(0.7*(pRect->Height() / 3)),
		cxValue = int(pRect->Width()/2),
		cyUnit = int(0.3*(pRect->Height() / 3)),
		cxUnit = int(pRect->Width() / 2);

	dc.SelectObject(m_penHeart);

	/////////////////////////////
	// heart
	// under line 
	dc.MoveTo(x + nPW, y + rowH - nPH);
	dc.LineTo(x + cxValue - nPW, y + rowH - nPH);

	dc.MoveTo(x + cxValue, y + rowH - nPH);
	dc.LineTo(pRect->right, y + rowH - nPH);

	// draw heart line
	rect.SetRect(
		pRect->right - MAX_HISTORY_LENGTH,
		y,
		pRect->right,
		y + rowH - nPH
	);
	
	USHORT *pHistory = &m_hisValHeart[nRider][0];
	int nTopIndex = m_nTopHeartHistoryIndex[nRider];
	CString log="";
	if (nTopIndex != -1) {
		//int nPrevIndex = GetPrevHistoryIndex(nTopIndex);
		int chartMin = 20, chartMax = 100, chartHeight = rect.Height();
		int chartValue = pHistory[nTopIndex];
		int posValue = 0;

		dc.SelectObject(m_brushPwr);
		//dc.SetROP2(R2_MERGEPEN);
		m_pointHistory[MAX_HISTORY_LENGTH].x = rect.left;
		m_pointHistory[MAX_HISTORY_LENGTH].y = rect.bottom;

		m_pointHistory[MAX_HISTORY_LENGTH + 1].x = rect.right;
		m_pointHistory[MAX_HISTORY_LENGTH + 1].y = rect.bottom;
		int nPrevIndex = GetNextHistoryIndex(nTopIndex);
		for (int i = 0;i < MAX_HISTORY_LENGTH ;i++) {
			chartValue = pHistory[nPrevIndex];
			posValue = int(chartHeight * (chartValue - chartMin) / (chartMax - chartMin));
			posValue = posValue < 0 ? 0 : posValue;

			m_pointHistory[i].x = rect.right - i;
			m_pointHistory[i].y = rect.bottom - posValue;
			
			nPrevIndex = GetPrevHistoryIndex(nPrevIndex);
		}
		dc.Polygon(m_pointHistory, sizeof(m_pointHistory) / sizeof(m_pointHistory[0]));

		// min , max		
		dc.SetTextColor(m_colorValue);
		dc.SelectObject(m_fontMaxValue);
		
		posValue = int(chartHeight * (m_hisMaxValHeart[nRider] - chartMin) / (chartMax - chartMin));
		posValue = posValue < 0 ? 0 : posValue;
		rect.SetRect(
			x + cxValue,
			pRect->top,
			pRect->right,
			pRect->bottom
		);
		sprintf_s(szBuf, "%d", m_hisMaxValHeart[nRider]);
		dc.DrawText(szBuf, rect, DT_SINGLELINE | DT_LEFT | DT_TOP);
		
		posValue = int(chartHeight * (m_hisAvgValHeart[nRider] - chartMin) / (chartMax - chartMin));
		posValue = posValue < 0 ? 0 : posValue;
		rect.SetRect(
			x + cxValue,
			pRect->top,
			pRect->right,
			y + rowH - nPH
		);
		sprintf_s(szBuf, "%d", m_hisAvgValHeart[nRider]);
		dc.DrawText(szBuf, rect, DT_SINGLELINE | DT_LEFT | DT_BOTTOM);
		
	}


	
	// value
	sprintf_s(szBuf, "%d", pRider->heart);
	rect.SetRect(
		x,
		y,
		x + cxValue,
		y + cyValue
	);
	dc.SetTextColor(m_colorValue);
	dc.SelectObject(m_fontValue);
	dc.DrawText(szBuf, rect, DT_SINGLELINE | DT_CENTER | DT_BOTTOM);
	// unit
	rect.SetRect(
		x,
		y+cyValue,
		x + cxUnit,
		y + cyValue+ cyUnit
	);
	dc.SetTextColor(m_colorUnit);
	dc.SelectObject(m_fontUnit);
	dc.DrawText("bpm", rect, DT_SINGLELINE | DT_CENTER | DT_TOP);




	////////////////////
	// power
	y += rowH;
	dc.SelectObject(m_penPwr);
	dc.MoveTo(x + nPW, y + rowH - nPH);
	dc.LineTo(x + cxValue - nPW, y + rowH - nPH);

	dc.MoveTo(x + cxValue, y + rowH - nPH);
	dc.LineTo(pRect->right, y + rowH - nPH);
	/*
	dc.SelectObject(m_brushPwr);
	rect.SetRect(
		x + nPW,
		y + rowH - nTH - nPH,
		x + cxValue - nPW,
		y + rowH - nPH
	);
	dc.Rectangle(rect);

	rect.SetRect(
		x + nPW + cxValue,
		y + rowH - nTH - nPH,
		pRect->right,
		y + rowH - nPH
	);
	dc.Rectangle(rect);
	*/
	// value
	sprintf_s(szBuf, "%d", pRider->power);
	rect.SetRect(
		x,
		y,
		x + cxValue,
		y + cyValue
	);
	dc.SetTextColor(m_colorValue);
	dc.SelectObject(m_fontValue);
	dc.DrawText(szBuf, rect, DT_SINGLELINE | DT_CENTER | DT_BOTTOM);
	// unit
	rect.SetRect(
		x,
		y + cyValue,
		x + cxUnit,
		y + cyValue + cyUnit
	);
	dc.SetTextColor(m_colorUnit);
	dc.SelectObject(m_fontUnit);
	dc.DrawText("watt", rect, DT_SINGLELINE | DT_CENTER | DT_TOP);


	// cad
	y += rowH;
	dc.SelectObject(m_penCad);
	dc.MoveTo(x + nPW, y + rowH - nPH);
	dc.LineTo(x + cxValue - nPW, y + rowH - nPH);

	dc.MoveTo(x + cxValue, y + rowH - nPH);
	dc.LineTo(pRect->right, y + rowH - nPH);

	/*
	dc.SelectObject(m_brushCad);
	rect.SetRect(
		x + nPW,
		y + rowH - nTH - nPH,
		x + cxValue - nPW,
		y + rowH - nPH
	);
	dc.Rectangle(rect);

	rect.SetRect(
		x + nPW + cxValue,
		y + rowH - nTH - nPH,
		pRect->right,
		y + rowH - nPH
	);
	dc.Rectangle(rect);
	*/

	// value
	sprintf_s(szBuf, "%d", pRider->cadence);
	rect.SetRect(
		x,
		y,
		x + cxValue,
		y + cyValue
	);
	dc.SetTextColor(m_colorValue);
	dc.SelectObject(m_fontValue);
	dc.DrawText(szBuf, rect, DT_SINGLELINE | DT_CENTER | DT_BOTTOM);
	// unit
	rect.SetRect(
		x,
		y + cyValue,
		x + cxUnit,
		y + cyValue + cyUnit
	);
	dc.SetTextColor(m_colorUnit);
	dc.SelectObject(m_fontUnit);
	dc.DrawText("rpm", rect, DT_SINGLELINE | DT_CENTER | DT_TOP);

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
