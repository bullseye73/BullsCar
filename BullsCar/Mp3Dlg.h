#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CMp3Dlg 대화 상자입니다.

class CMp3Dlg : public CDialog
{
	DECLARE_DYNAMIC(CMp3Dlg)

public:
	CMp3Dlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMp3Dlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MP3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonClose();
public:
	CListCtrl m_CtrlMp3List;
public:
	CEdit m_CtrlMp3Path;
public:
	afx_msg void OnBnClickedButtonMp3browser();
};
