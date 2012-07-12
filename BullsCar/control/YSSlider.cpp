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
	nRangeGap = 0;	// Range�� Gap
	nPixelGap = 0;	// Pixel�� Gap

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

// ��Ʈ���� Load�մϴ�.
void CYSSlider::LoadBitmaps(UINT nIDBack, UINT nIDNormal, UINT nIDSelect, UINT nIDFocus, int nMargin)
{
	BITMAP					bitmap;

	// �����̴� ��濡 �� ��Ʈ���� �����մϴ�.
	m_btBackground.LoadBitmap(nIDBack);

	// ����� ��Ʈ�� ������ �о�ͼ�
	m_btBackground.GetBitmap(&bitmap);

	// �����̴��� ����(Thumb)�� �� ��Ʈ���� �����մϴ�.
	// ����, ����(Normal)�� ������ ��Ʈ���� �����մϴ�.
	m_btThumbNormal.LoadBitmap(nIDNormal);

	// �׸���, ���콺�� ������ ��������(Select) ������ ��Ʈ���� �����մϴ�.
	if(nIDSelect == 0) 
	{
		m_btThumbSelect.LoadBitmap(nIDNormal);
	}
	else 
	{
		m_btThumbSelect.LoadBitmap(nIDSelect);
	}

	// ���������� ��Ŀ���� ������ ������(Focus) ������ ��Ʈ���� �����մϴ�.
	if(nIDFocus == 0) 
	{
		m_btThumbFocus.LoadBitmap(nIDNormal);
	}
	else 
	{
		m_btThumbFocus.LoadBitmap(nIDFocus);
	}

	// ����(Thumb)�� ��Ʈ�� ������ �о�ͼ�
	m_btThumbNormal.GetBitmap(&bitmap);

	// ���̶� ���̸� �����صӴϴ�.
	m_czbitThumb.cy = 16;
	m_czbitThumb.cx = 16;

	// �̰�, ����(Thumb)�� �̵��� ���� ������ �ݴϴ�.
	// ���ʿ� �ణ�� ������ �Ӵϴ�.
	m_nMargin = nMargin;

    // ����(Thumb)�� �ʱ� x���� �����մϴ�. �� ���ʿ�...
	//m_ptThumb.x = rc.left + m_nMargin;
	m_ptThumb.x = 0;

	// ����(Thumb)�� �ʱ� y�� �����մϴ�. ����ϴ�.
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

	// ����� �׸���.
	MemDC.CreateCompatibleDC(&dc);
	oldBitmap = MemDC.SelectObject(&m_btBackground);

  	dc.BitBlt(0, 0, 1024, 45, &MemDC, 0, 0, SRCCOPY);

	MemDC.SelectObject(oldBitmap);
	MemDC.DeleteDC();
	//...

	// �����̴��� rect�� �˾ƿ´�.
	CRect					rc;

	GetClientRect(rc);

	// Position���� ��ǥ�� ��ȯ���ش�.
	nPos = GetPos();
	nRangeGap = nMax - nMin;			// Range�� Gap
	nPixelGap = (rc.right - rc.left);	// Pixel�� Gap

	biyul = ((double)nPixelGap  / (double)nRangeGap);

	m_ptThumb.x = (long)((((double)nPos * biyul) - ((double)nMin * biyul)));

	if(m_ptThumb.x <= 0)
		m_ptThumb.x = 0;
	//...

	// Thumb�� �׷��ش�.
	MemDC.CreateCompatibleDC(&dc);

	if(m_bDragging)
	{
		oldBitmap = MemDC.SelectObject(&m_btThumbSelect);		// ���õ��� ��
	}
	else
	{
		if(GetFocus() == this)
		{
			oldBitmap = MemDC.SelectObject(&m_btThumbFocus);	// ��Ŀ���� ������ ���� ��
		}
		else 
		{
			oldBitmap = MemDC.SelectObject(&m_btThumbNormal);	// ���� ��
		}
	}

	dc.BitBlt(m_ptThumb.x, m_ptThumb.y, 16, 16, &MemDC, 0, 0, SRCCOPY);

	MemDC.SelectObject(oldBitmap);
	MemDC.DeleteDC();
	//...
}

// ���콺 ������ �� ���콺�� ĸ���ϰ�, �ش���ġ�� ����(Thumb)�� �׸���.
// �׸���, �θ������쿡 �޽����� ������.
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

		RedrawWindow();			// OnPaint()�� ȣ���Ѵ�.
	}
	else
	{
		CSliderCtrl::OnLButtonDown(nFlags, point);
	}
}

// ���콺 ����� �� ���콺 ĸ�ĸ� Ǯ���ְ�, �ش���ġ�� ����(Thumb)�� �׸���.
// �׸���, �θ������쿡 �޽����� ������.
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

// Ű�� ��������
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

	SetLineSize(1);			// Line �̵��� 1�� ����.  (LEFT,RIGHT ��)
	SetPageSize(10);		// Page �̵��� 10�� ����. (PAGE UP, PAGE DOWN ��)
}

// ���� ���콺�� ��ġ�� �����̴��� ��ġ(Position)���� ��ȯ�Ѵ�.
// ���� ��ġ�� �ٸ��� true��, �׷��� ������ false�� �����Ѵ�.
//
bool CYSSlider::SetPointToPos(const CPoint &pt)
{
	const int				nMin = GetRangeMin();			// Range�� �ּҰ�
	const int				nMax = GetRangeMax() + 1;		// Range�� �ִ밪

	int						nRangeGap = nMax - nMin;		// Range�� Gap

	CRect					rc;

	GetClientRect(rc);

	int						nPixelGap = (rc.right - rc.left);		// Pixel�� Gap

	// ��ʽ����� ����غ��ϱ�...
	m_ptThumb.x = pt.x - (m_czbitThumb.cx / 2 + m_nMargin);

	double					biyul = ((double)nRangeGap  / (double)nPixelGap);

	int						nNewPos = nMin + (long)(biyul * m_ptThumb.x);		// �ش��ϴ� ��ġ(Position)

	// �ּҰ����� ������ �ּҰ�����
	if(nNewPos < nMin) 
	{			
		nNewPos = nMin;
	}
	else
	{
		// �ִ밪���� ũ�� �ִ밪����
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

// �θ� �����쿡 ��ũ�� �޽����� �����ϴ�.
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
