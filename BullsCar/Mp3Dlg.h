#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CMp3Dlg ��ȭ �����Դϴ�.

class CMp3Dlg : public CDialog
{
	DECLARE_DYNAMIC(CMp3Dlg)

public:
	CMp3Dlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMp3Dlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_MP3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
