#pragma once

#include "./control/BASEIMAGE.h"
#include "afxwin.h"

#include "BullsCarDlg.h"
#include "thumbctl1.h"
// CPhotoView ��ȭ �����Դϴ�.

class CBullsCarDlg;



class CPhotoView : public CDialog
{
	DECLARE_DYNAMIC(CPhotoView)

public:
	CPhotoView(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPhotoView();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_PHOTO };

public:
	CBaseImage *m_pImage;
	CBullsCarDlg *m_pParentDlg;

	CFile m_File;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonClose();
	virtual BOOL OnInitDialog();

public:
	afx_msg void OnBnClickedButtonShow();
public:
	afx_msg void OnPaint();

public:
	afx_msg void OnDestroy();
public:
	CStatic m_CtrlStcPhoto;
public:
	CStatic m_CtrlStcMsg;
};
