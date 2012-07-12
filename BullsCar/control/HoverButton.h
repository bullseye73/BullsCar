#if !defined(AFX_HOVERBUTTON_H__16C6D980_BD45_11D3_BDA3_00104B133581__INCLUDED_)
#define AFX_HOVERBUTTON_H__16C6D980_BD45_11D3_BDA3_00104B133581__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER > 1000
// HoverButton.h : header file
//

// Return values
#ifndef	BTNST_OK
#define	BTNST_OK						0
#endif
#ifndef	BTNST_INVALIDRESOURCE
#define	BTNST_INVALIDRESOURCE			1
#endif
#ifndef	BTNST_FAILEDMASK
#define	BTNST_FAILEDMASK				2
#endif
#ifndef	BTNST_INVALIDINDEX
#define	BTNST_INVALIDINDEX				3
#endif
#ifndef	BTNST_INVALIDALIGN
#define	BTNST_INVALIDALIGN				4
#endif

/////////////////////////////////////////////////////////////////////////////
// CHoverButton by Niek Albers
// Thanks to some people for the tooltip.
// A cool CBitmapButton derived class with 3 states,
// Up/Down/Hover.
class CHoverButton : public CBitmapButton
{
	DECLARE_DYNAMIC(CHoverButton);

	// Construction
public:
	CHoverButton();
	virtual ~CHoverButton();

public:

// Attributes
protected:

	BOOL					m_bHover;						// indicates if mouse is over the button
	CSize					m_ButtonSize;					// width and height of the button
	CBitmap					mybitmap;

	CDC						*mydc;

	CDC						*pMemDC;
	CBitmap					*pOldBitmap;

	CWnd					*wndUnderMouse;
	CWnd					*wndActive;

	BOOL					m_bAlwaysTrack;		// Always hilight button?
	UINT					m_nTimerId;			// Timer ID used for mouse-leave

	RECT					m_csRect;

protected:
	DWORD					SetAlwaysTrack(BOOL bAlwaysTrack = TRUE);

	void					OnMouseHover();

	LRESULT					OnMouseLeave(WPARAM wParam, LPARAM lParam);
	static void CALLBACK	TimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime);

	void					CancelHover();

protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHoverButton)
	protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL					LoadBitmap(UINT bitmapid);

	// Generated message map functions
protected:
	//{{AFX_MSG(CHoverButton)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnDestroy();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnCancelMode();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOVERBUTTON_H__16C6D980_BD45_11D3_BDA3_00104B133581__INCLUDED_)
