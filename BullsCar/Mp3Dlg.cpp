// Mp3Dlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "BullsCar.h"
#include "Mp3Dlg.h"


// CMp3Dlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMp3Dlg, CDialog)

CMp3Dlg::CMp3Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMp3Dlg::IDD, pParent)
{
	
}

CMp3Dlg::~CMp3Dlg()
{
}

void CMp3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MP3, m_CtrlMp3List);
	DDX_Control(pDX, IDC_EDIT_MP3PATH, m_CtrlMp3Path);
}


BEGIN_MESSAGE_MAP(CMp3Dlg, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CMp3Dlg::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_MP3BROWSER, &CMp3Dlg::OnBnClickedButtonMp3browser)
END_MESSAGE_MAP()


// CMp3Dlg 메시지 처리기입니다.

void CMp3Dlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CMp3Dlg::OnBnClickedButtonClose()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	DestroyWindow();
}

void CMp3Dlg::OnBnClickedButtonMp3browser()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog* cfd = new CFileDialog(true, NULL, NULL, OFN_HIDEREADONLY, NULL, NULL, 0);
	cfd->DoModal();
	//CString strFile = cfd->GetFileName();
	CString strPath = cfd->GetPathName();
	
	m_CtrlMp3Path.SetWindowText(strPath);
}
