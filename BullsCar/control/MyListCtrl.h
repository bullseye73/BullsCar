#ifndef _MYLISTCTRL_H_
#define _MYLISTCTRL_H_

#include "SeeLive_DS.h"
#include "SkinHeaderCtrl.h"

class CMyListCtrl : public CListCtrl
{
public :
	// 持失切人 社瑚切
	CMyListCtrl();
	virtual ~CMyListCtrl();

public :
	INT						AddListviewData(int iItem, int iSubItem, LPSTR text, int imode, int image);
	CHAR					*CutVideoName(CHAR *name);
	void					SwapData(SInputData *pData, int first, int second);
	void					SwapFtpData(SFtpFileData *pData, int first, int second);

	INT						CompareVideoName(LPCTSTR str1, LPCTSTR str2);
	void					QuickSortData(SInputData *pIData, int type, int fieldIndex, int direction, int left, int right, int nFlag);
	INT						AddListviewColumn(int iItem, int cx, LPSTR text, int arrange);
	void					DrawSortHeader(int oldIndex, int newIndex, int direction);
	void					QuickSortFtpData(SFtpFileData *pIData, int type, int fieldIndex, int direction,int left, int right);

	void					GetList_col_Data_i(CString *pData, int colnum, int colcount);
	void					SetList_col_Data_i(CString *pData, int colnum, int colcount);
	int						GetRowCount_i();
	int						GetDataTotalCount_i();

	//
	void					SetType(int type = 0, int highlight = 0);

	//
	int						GetColumns();

	void					ItemScroll(int num);

	void					SetItemCurrentMark(int num);

	char					*uniToansi(const wchar_t* pwstrSrc);
	wchar_t					*ansiTouni(const char *szANSI);

	void					Init();

	//{{AFX_VIRTUAL(CMyListCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

public :
	int						m_nType;
	int						m_nHighlight;

	//c
	COLORREF				m_crHighLight;
	COLORREF				m_crHighLightText;
	COLORREF				m_crWindow;
	COLORREF				m_crWindowText;
	COLORREF				m_crBtnFace;

	//
	int						m_CurrentMark;

	CSkinHeaderCtrl			m_SkinHeaderCtrl;
	
//	SInputData				*m_sVodData;

	bool					m_bCheckLine;


protected:
	afx_msg BOOL OnEraseBkgnd(CDC *pDC);
	afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()

public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
};

#endif


