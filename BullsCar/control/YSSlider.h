#if !defined(AFX_YSSLIDER_H__A041F14D_4E95_401B_9870_D461C5CE3288__INCLUDED_)
#define AFX_YSSLIDER_H__A041F14D_4E95_401B_9870_D461C5CE3288__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// YSSlider.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CYSSlider window

class CYSSlider : public CSliderCtrl
{
// Construction
public:
	CYSSlider();
	virtual ~CYSSlider();

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CYSSlider)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	// 비트맵을 로드한다.
	void					LoadBitmaps(UINT nIDBack, UINT nIDNormal, UINT nIDSelect = 0, UINT nIDFocus = 0, int nMargin = 5);

	void					SetSTType(int type);

	// Generated message map functions
protected:
	int						m_nMargin;										// 컨트롤의 여백

	CSize					m_czbitThumb;									// 엄지(Thumb)의 비트맵 싸이즈 (높이와 넓이)
	CSize					m_czbitBack;									// 배경 비트맵 싸이즈

	CBitmap					m_btThumbFocus;									// 포커스가 있을 때 엄지(Thumb) 비트맵
	CBitmap					m_btThumbSelect;								// 선택됐을 때 엄지(Thumb) 비트맵
	CBitmap					m_btThumbNormal;								// 평상시 엄지(Thumb) 비트맵
	CBitmap					m_btBackground;									// 배경 비트맵

	CPoint					m_ptThumb;										// 엄지(Thumb)를 그릴 위치

	bool					m_bDragChanged;									// 드래그 됐음.
	bool					m_bDragging;									// 드래그 중임.

	CDC						MemDC;
	CBitmap					*oldBitmap;

	int						nPos;
	int						nRangeGap;	// Range의 Gap
	int						nPixelGap;	// Pixel의 Gap

	double					biyul;

	int						m_nSTType;

protected:
	void					PostMessageToParent(const int nTBCode) const;
	bool					SetPointToPos(const CPoint& pt);

protected:
	//{{AFX_MSG(CYSSlider)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YSSLIDER_H__A041F14D_4E95_401B_9870_D461C5CE3288__INCLUDED_)
