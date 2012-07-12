#pragma once

#include "./control/BASEIMAGE.h"
#include "afxwin.h"

#include "BullsCarDlg.h"
#include "thumbctl1.h"
// CPhotoView 대화 상자입니다.

class CBullsCarDlg;



class CPhotoView : public CDialog
{
	DECLARE_DYNAMIC(CPhotoView)

public:
	CPhotoView(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPhotoView();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_PHOTO };

public:
	CBaseImage *m_pImage;
	CBullsCarDlg *m_pParentDlg;

	CFile m_File;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.


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
