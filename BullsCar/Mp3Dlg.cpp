// Mp3Dlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "BullsCar.h"
#include "Mp3Dlg.h"


// CMp3Dlg ��ȭ �����Դϴ�.

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


// CMp3Dlg �޽��� ó�����Դϴ�.

void CMp3Dlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

void CMp3Dlg::OnBnClickedButtonClose()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	DestroyWindow();
}

void CMp3Dlg::OnBnClickedButtonMp3browser()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog* cfd = new CFileDialog(true, NULL, NULL, OFN_HIDEREADONLY, NULL, NULL, 0);
	cfd->DoModal();
	//CString strFile = cfd->GetFileName();
	CString strPath = cfd->GetPathName();
	
	m_CtrlMp3Path.SetWindowText(strPath);
}
