// PhotoView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "BullsCar.h"
#include "PhotoView.h"

#include <string.h>

// CPhotoView 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPhotoView, CDialog)

CPhotoView::CPhotoView(CWnd* pParent /*=NULL*/)
	: CDialog(CPhotoView::IDD, pParent)
{
	m_pImage = NULL;

}

CPhotoView::~CPhotoView()
{
}

void CPhotoView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PHOTO, m_CtrlStcPhoto);
	DDX_Control(pDX, IDC_STATIC_MESSAGE, m_CtrlStcMsg);
}


BEGIN_MESSAGE_MAP(CPhotoView, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CPhotoView::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_SHOW, &CPhotoView::OnBnClickedButtonShow)
	ON_WM_PAINT()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CPhotoView 메시지 처리기입니다.

void CPhotoView::OnBnClickedButtonClose()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	DestroyWindow();
	//SetTransparent(100);
}

void CPhotoView::OnBnClickedButtonShow()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog* cfd = new CFileDialog(true, L"JPG", L"jpg", OFN_HIDEREADONLY, NULL, NULL, 0);
	cfd->DoModal();
	//CString strFile = cfd->GetFileName();
	CString strPath = cfd->GetPathName();
	CString strExt;

	m_CtrlStcMsg.SetWindowText(strPath);
	
	if(m_pImage)
		delete m_pImage;

	strExt = cfd->GetFileExt();
	strExt.MakeUpper();
	if (strcmp((LPSTR)(LPCTSTR)strExt, (LPSTR)(LPCTSTR)L"JPG") == 0)
	{
		m_pImage=new CBaseImage;
		m_pImage->LoadJpg(m_pParentDlg->uniToansi(strPath));
		SIZE sz = m_pImage->GetSize();

		m_CtrlStcPhoto.MoveWindow(0,0,sz.cx, sz.cy, true);
		
		if(m_pImage)
			m_pImage->PaintImage(0,0, m_CtrlStcPhoto.GetWindowDC()->m_hDC);
		
		return ;
	}
	//AfxMessageBox("your choice file that's not a JPG file!");
}

void CPhotoView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.
	
}


void CPhotoView::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_pImage)
	{
		delete m_pImage;
		m_pImage = NULL;
	}
}

BOOL CPhotoView::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_CtrlStcPhoto.MoveWindow(0,0,0,0, true);
	m_CtrlStcMsg.MoveWindow(0,0,800,20, true);

	
/*	
	if(m_pParentDlg->m_pFile->Open("photolist.dat"))
	{
		m_pParentDlg->m_pFile->AddString("test");
		m_pParentDlg->m_pFile->Close();
	}
*/
/*
	CFile cf;
	if(cf.Open(L"photolist.dat", CFile::modeReadWrite, NULL))
	{
		cf.Write("test", strlen("test"));
		cf.Close();
	}
*/
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}


