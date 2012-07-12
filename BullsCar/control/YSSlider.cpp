#include "stdafx.h"
#include "resource.h"
#include "YSSlider.h"

#include "MemDCx.h"
#define USE_MEM_DC 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CYSSlider

CYSSlider::CYSSlider()
{
	m_czbitThumb.cx = 0;
	m_czbitThumb.cy = 0;

	m_bDragging = false;

	oldBitmap = NULL;

	nPos = 0;
	nRangeGap = 0;	// Range의 Gap
	nPixelGap = 0;	// Pixel의 Gap

	biyul = 0.0;

	m_nSTType = 1;
}

CYSSlider::~CYSSlider()
{
}

BEGIN_MESSAGE_MAP(CYSSlider, CSliderCtrl)
	//{{AFX_MSG_MAP(CYSSlider)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CYSSlider message handlers

// 비트맵을 Load합니다.
void CYSSlider::LoadBitmaps(UINT nIDBack, UINT nIDNormal, UINT nIDSelect, UINT nIDFocus, int nMargin)
{
	BITMAP					bitmap;

	// 슬라이더 배경에 깔 비트맵을 설정합니다.
	m_btBackground.LoadBitmap(nIDBack);

	// 배경의 비트맵 정보를 읽어와서
	m_btBackground.GetBitmap(&bitmap);

	// 슬라이더의 엄지(Thumb)로 쓸 비트맵을 설정합니다.
	// 먼저, 평상시(Normal)에 보여줄 비트맵을 설정합니다.
	m_btThumbNormal.LoadBitmap(nIDNormal);

	// 그리고, 마우스로 지긋이 눌렀을때(Select) 보여줄 비트맵을 설정합니다.
	if(nIDSelect == 0) 
	{
		m_btThumbSelect.LoadBitmap(nIDNormal);
	}
	else 
	{
		m_btThumbSelect.LoadBitmap(nIDSelect);
	}

	// 마지막으로 포커스를 가지고 있을때(Focus) 보여줄 비트맵을 설정합니다.
	if(nIDFocus == 0) 
	{
		m_btThumbFocus.LoadBitmap(nIDNormal);
	}
	else 
	{
		m_btThumbFocus.LoadBitmap(nIDFocus);
	}

	// 엄지(Thumb)의 비트맵 정보를 읽어와서
	m_btThumbNormal.GetBitmap(&bitmap);

	// 높이랑 넓이를 저장해둡니다.
	m_czbitThumb.cy = 16;
	m_czbitThumb.cx = 16;

	// 이건, 엄지(Thumb)이 이동할 곳을 제한해 줍니다.
	// 양쪽에 약간의 여백을 둡니다.
	m_nMargin = nMargin;

    // 엄지(Thumb)의 초기 x값을 설정합니다. 맨 왼쪽에...
	//m_ptThumb.x = rc.left + m_nMargin;
	m_ptThumb.x = 0;

	// 엄지(Thumb)의 초기 y을 설정합니다. 가운뎁니다.
	m_ptThumb.y = 16;
}

BOOL CYSSlider::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CYSSlider::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	const int nMin = GetRangeMin();
	const int nMax = GetRangeMax();

	// 배경을 그린다.
	MemDC.CreateCompatibleDC(&dc);
	oldBitmap = MemDC.SelectObject(&m_btBackground);

  	dc.BitBlt(0, 0, 1024, 45, &MemDC, 0, 0, SRCCOPY);

	MemDC.SelectObject(oldBitmap);
	MemDC.DeleteDC();
	//...

	// 슬라이더의 rect를 알아온다.
	CRect					rc;

	GetClientRect(rc);

	// Position값을 좌표로 변환해준다.
	nPos = GetPos();
	nRangeGap = nMax - nMin;			// Range의 Gap
	nPixelGap = (rc.right - rc.left);	// Pixel의 Gap

	biyul = ((double)nPixelGap  / (double)nRangeGap);

	m_ptThumb.x = (long)((((double)nPos * biyul) - ((double)nMin * biyul)));

	if(m_ptThumb.x <= 0)
		m_ptThumb.x = 0;
	//...

	// Thumb을 그려준다.
	MemDC.CreateCompatibleDC(&dc);

	if(m_bDragging)
	{
		oldBitmap = MemDC.SelectObject(&m_btThumbSelect);		// 선택됐을 때
	}
	else
	{
		if(GetFocus() == this)
		{
			oldBitmap = MemDC.SelectObject(&m_btThumbFocus);	// 포커스를 가지고 있을 때
		}
		else 
		{
			oldBitmap = MemDC.SelectObject(&m_btThumbNormal);	// 보통 때
		}
	}

	dc.BitBlt(m_ptThumb.x, m_ptThumb.y, 16, 16, &MemDC, 0, 0, SRCCOPY);

	MemDC.SelectObject(oldBitmap);
	MemDC.DeleteDC();
	//...
}

// 마우스 눌렀을 때 마우스를 캡쳐하고, 해당위치에 엄지(Thumb)를 그린다.
// 그리고, 부모윈도우에 메시지를 날린다.
void CYSSlider::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(!m_bDragging)
	{
		m_bDragging = true;
		m_bDragChanged = false;

		SetCapture();
		SetFocus();

		if(SetPointToPos(point))
		{
			m_bDragChanged = true;

			PostMessageToParent(TB_THUMBTRACK);
		}

		RedrawWindow();			// OnPaint()를 호출한다.
	}
	else
	{
		CSliderCtrl::OnLButtonDown(nFlags, point);
	}
}

// 마우스 띄었을 때 마우스 캡쳐를 풀어주고, 해당위치에 엄지(Thumb)를 그린다.
// 그리고, 부모윈도우에 메시지를 날린다.
void CYSSlider::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(m_bDragging)
	{
		m_bDragging = false;
		::ReleaseCapture();

		if(SetPointToPos(point))
		{
			PostMessageToParent(TB_THUMBTRACK);

			m_bDragChanged = true;
		}

		if(m_bDragChanged)
		{
			PostMessageToParent(TB_THUMBPOSITION);

			m_bDragChanged = false;
		}

		RedrawWindow();
	}
	else
	{
		CSliderCtrl::OnLButtonUp(nFlags, point);
	}
}

// 키를 눌렀을때
void CYSSlider::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	const int				nMin = GetRangeMin();
	const int				nMax = GetRangeMax() + 1;

	if(nChar == 0X11)
		return;

	switch(nChar)
	{
		case 0X49 :
		case VK_LEFT :
			{
				int						nNewPos = GetPos() - 10;

				if(nNewPos < nMin) 
				{
					nNewPos = nMin;
				}

				if(nNewPos < 0)
					nNewPos = 0;

				SetPos(nNewPos);
				RedrawWindow();

				PostMessageToParent(TB_LINEUP);
			}

			break;
		
		case 0X4F :
		case VK_RIGHT :
			{
				int						nNewPos = GetPos() + 10;

				if(nNewPos > nMax) 
				{
					nNewPos = nMax;
				}

				if(nNewPos > nMax)
					nNewPos = nMax;

				SetPos(nNewPos);
				RedrawWindow();

				PostMessageToParent(TB_LINEDOWN);
			}

			break;
	}

	CSliderCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CYSSlider::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
/*	switch(nChar)
	{
		case VK_LEFT:
			printf("OnKeyUp %x \n", nChar);
			break;

		case VK_RIGHT:
		case VK_PRIOR:
		case VK_NEXT:
		case VK_HOME:
		case VK_END:
			PostMessageToParent(TB_ENDTRACK);
			break;

		default:
			//CSliderCtrl::OnKeyUp(nChar, nRepCnt, nFlags);
			break;
	}
*/
	return;
}

void CYSSlider::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_bDragging)
	{
		if(SetPointToPos(point))
		{
			m_bDragChanged = true;

			PostMessageToParent(TB_THUMBTRACK);

			RedrawWindow();
		}
	}
	else
	{
		CSliderCtrl::OnMouseMove(nFlags, point);
	}
}

void CYSSlider::PreSubclassWindow() 
{
	CSliderCtrl::PreSubclassWindow();

	SetLineSize(1);			// Line 이동은 1씩 증감.  (LEFT,RIGHT 등)
	SetPageSize(10);		// Page 이동은 10씩 증감. (PAGE UP, PAGE DOWN 등)
}

// 현재 마우스의 위치를 슬라이더의 위치(Position)으로 변환한다.
// 이전 위치와 다르면 true를, 그렇지 않으면 false를 리턴한다.
//
bool CYSSlider::SetPointToPos(const CPoint &pt)
{
	const int				nMin = GetRangeMin();			// Range의 최소값
	const int				nMax = GetRangeMax() + 1;		// Range의 최대값

	int						nRangeGap = nMax - nMin;		// Range의 Gap

	CRect					rc;

	GetClientRect(rc);

	int						nPixelGap = (rc.right - rc.left);		// Pixel의 Gap

	// 비례식으로 계산해보니까...
	m_ptThumb.x = pt.x - (m_czbitThumb.cx / 2 + m_nMargin);

	double					biyul = ((double)nRangeGap  / (double)nPixelGap);

	int						nNewPos = nMin + (long)(biyul * m_ptThumb.x);		// 해당하는 위치(Position)

	// 최소값보다 작으면 최소값으로
	if(nNewPos < nMin) 
	{			
		nNewPos = nMin;
	}
	else
	{
		// 최대값보다 크면 최대값으로
		if(nNewPos > nMax) 
		{		
			nNewPos = nMax;
		}
	}

	const bool				bChanged = (nNewPos != GetPos());

	if(bChanged) 
	{
		SetPos(nNewPos);
	}

	return bChanged;
}

// 부모 윈도우에 스크롤 메시지를 보냅니다.
//
void CYSSlider::PostMessageToParent(const int nTBCode) const
{
	CWnd					*pWnd = GetParent();

	if(GetKeyState(0x11) < 0)
		return;
	
	if(nTBCode == 0X11)
		return;

	if(pWnd) 
		pWnd->PostMessage(WM_HSCROLL, (WPARAM)((GetPos() << 16) | nTBCode), (LPARAM)GetSafeHwnd());
}

void CYSSlider::SetSTType(int type)
{
	m_nSTType = type;
}
