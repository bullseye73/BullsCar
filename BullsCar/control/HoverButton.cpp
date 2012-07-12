// HoverButton.cpp : implementation file
//

#include "stdafx.h"
#include "HoverButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHoverButton

#ifndef WM_MOUSELEAVE
#define WM_MOUSELEAVE                   0x02A3
#endif

CHoverButton::CHoverButton()
{
	m_bHover = FALSE;

	//........
	// Button will be tracked also if when the window is inactive (like Internet Explorer)
	m_bAlwaysTrack = TRUE;

	// No timer defined
	m_nTimerId = 0;

	pMemDC = NULL;
	pOldBitmap = NULL;
}

CHoverButton::~CHoverButton()
{
}

IMPLEMENT_DYNAMIC(CHoverButton, CBitmapButton)

BEGIN_MESSAGE_MAP(CHoverButton, CBitmapButton)
	//{{AFX_MSG_MAP(CHoverButton)
	ON_WM_MOUSEMOVE()
	ON_WM_ENABLE()
	ON_WM_DESTROY()
	ON_WM_KILLFOCUS()
	ON_WM_ACTIVATE()
	ON_WM_CANCELMODE()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_WM_SETCURSOR()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////
 //	CHoverButton message handlers
		
void CHoverButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	//	TODO: Add your message handler code here and/or call default
	wndUnderMouse = NULL;
	wndActive = this;

	CButton::OnMouseMove(nFlags, point);

	ClientToScreen(&point);
	wndUnderMouse = WindowFromPoint(point);

	// If the mouse enter the button with the left button pressed then do nothing
	if(nFlags & MK_LBUTTON && m_bHover == FALSE) 
		return;

	if(m_bAlwaysTrack == FALSE)	
		wndActive = GetActiveWindow();

	if(wndUnderMouse && wndUnderMouse->m_hWnd == m_hWnd && wndActive)
	{
		if(!m_bHover)
		{
			m_bHover = TRUE;

			InvalidateRect(&m_csRect, true);

			ASSERT(m_nTimerId == 0);

			KillTimer(6);
			m_nTimerId = SetTimer(6, 50, TimerProc);
		} // if
	} //else CancelHover();
}

void CHoverButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	mydc = CDC::FromHandle(lpDrawItemStruct->hDC);

	pMemDC->CreateCompatibleDC(mydc);

	pOldBitmap = pMemDC->SelectObject(&mybitmap);
	
	if(lpDrawItemStruct->itemState & ODS_SELECTED)
	{
		//마우스 오버
		mydc->BitBlt(0, 0, m_ButtonSize.cx, m_ButtonSize.cy, pMemDC, m_ButtonSize.cx * 2, 0, SRCCOPY);
	}
	else
	{
		if(m_bHover)
		{
			//마우스 클릭 
			mydc->BitBlt(0, 0, m_ButtonSize.cx, m_ButtonSize.cy, pMemDC, m_ButtonSize.cx, 0, SRCCOPY);
		}
		else
		{
			//무 상태
			mydc->BitBlt(0, 0, m_ButtonSize.cx, m_ButtonSize.cy, pMemDC, 0, 0, SRCCOPY);
		}
	}

	// clean up
	pMemDC->SelectObject(pOldBitmap);
}

// Load a bitmap from the resources in the button, the bitmap has to have 3 buttonsstates next to each other: Up/Down/Hover
BOOL CHoverButton::LoadBitmap(UINT bitmapid)
{
	mybitmap.Attach(::LoadImage(::AfxGetInstanceHandle(), MAKEINTRESOURCE(bitmapid), IMAGE_BITMAP, 0, 0, 0));

	BITMAP					bitmapbits;

	mybitmap.GetBitmap(&bitmapbits);

	m_ButtonSize.cy = bitmapbits.bmHeight;
	m_ButtonSize.cx = bitmapbits.bmWidth / 3;

	SetWindowPos(NULL, 0, 0, m_ButtonSize.cx, m_ButtonSize.cy, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOREPOSITION);

	GetClientRect(&m_csRect);

	pMemDC = new CDC;

	return TRUE;
}

//...........................................
void CHoverButton::CancelHover()
{
	// Only for flat buttons
	// Kill timer (if active)
//	if(m_nTimerId)	
//		KillTimer(m_nTimerId);

	m_nTimerId = 0;

//	if(m_bHover)
	{
		m_bHover = FALSE;

		InvalidateRect(&m_csRect, true);
	} // if
} // End of CancelHover

// Handler for WM_MOUSELEAVE
LRESULT CHoverButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	CancelHover();

	return 0;
} // End of OnMouseLeave

void CALLBACK CHoverButton::TimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	if(idEvent == 6)
	{
		POINT					csPos;
		RECT					csRect;

		::GetCursorPos(&csPos);
		::ScreenToClient(hwnd, &csPos);

		::GetClientRect(hwnd, &csRect);

		if(!::PtInRect(&csRect, csPos))
		{
			::KillTimer(hwnd, 6);

			::PostMessage(hwnd, WM_MOUSELEAVE, 0, 0);
		} // if
	}
} // End of TimerProc

void CHoverButton::OnEnable(BOOL bEnable) 
{
	CButton::OnEnable(bEnable);
	
	if(bEnable == FALSE)	
	{
		CWnd					*pWnd = GetParent()->GetNextDlgTabItem(this);

		if(pWnd)
			pWnd->SetFocus();
		else
			GetParent()->SetFocus();

		CancelHover();
	} // if
} // End of OnEnable

void CHoverButton::OnKillFocus(CWnd* pNewWnd) 
{
	CButton::OnKillFocus(pNewWnd);

	CancelHover();
} // End of OnKillFocus

void CHoverButton::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CButton::OnActivate(nState, pWndOther, bMinimized);

	if(nState == WA_INACTIVE)	
		CancelHover();
} // End of OnActivate

void CHoverButton::OnCancelMode() 
{
	CButton::OnCancelMode();

	CancelHover();
} // End of OnCancelMode

void CHoverButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetState(TRUE);

#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300)
	SHRecognizeGesture(point);
#endif // _WIN32_WCE_PSPC

	Default();
	
	//CButton::OnLButtonDown(nFlags, point);
} // End of OnLButtonDown

DWORD CHoverButton::SetAlwaysTrack(BOOL bAlwaysTrack)
{
	m_bAlwaysTrack = bAlwaysTrack;

	return BTNST_OK;
} // End of SetAlwaysTrack

void CHoverButton::OnDestroy() 
{
	// Kill timer
	if(m_nTimerId)	
		KillTimer(m_nTimerId);

	m_nTimerId = 0;

	delete pMemDC;

	CButton::OnDestroy();
} // End of OnDestroy


#undef BS_TYPEMASK
#undef WM_MOUSELEAVE

