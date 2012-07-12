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
	// ��Ʈ���� �ε��Ѵ�.
	void					LoadBitmaps(UINT nIDBack, UINT nIDNormal, UINT nIDSelect = 0, UINT nIDFocus = 0, int nMargin = 5);

	void					SetSTType(int type);

	// Generated message map functions
protected:
	int						m_nMargin;										// ��Ʈ���� ����

	CSize					m_czbitThumb;									// ����(Thumb)�� ��Ʈ�� ������ (���̿� ����)
	CSize					m_czbitBack;									// ��� ��Ʈ�� ������

	CBitmap					m_btThumbFocus;									// ��Ŀ���� ���� �� ����(Thumb) ��Ʈ��
	CBitmap					m_btThumbSelect;								// ���õ��� �� ����(Thumb) ��Ʈ��
	CBitmap					m_btThumbNormal;								// ���� ����(Thumb) ��Ʈ��
	CBitmap					m_btBackground;									// ��� ��Ʈ��

	CPoint					m_ptThumb;										// ����(Thumb)�� �׸� ��ġ

	bool					m_bDragChanged;									// �巡�� ����.
	bool					m_bDragging;									// �巡�� ����.

	CDC						MemDC;
	CBitmap					*oldBitmap;

	int						nPos;
	int						nRangeGap;	// Range�� Gap
	int						nPixelGap;	// Pixel�� Gap

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
