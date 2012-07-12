// BullsCarDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "BullsCar.h"
#include "BullsCarDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBullsCarDlg ��ȭ ����


CBullsCarDlg::CBullsCarDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBullsCarDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBullsCarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBullsCarDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CBullsCarDlg::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_MP3, &CBullsCarDlg::OnBnClickedButtonMp3)
	ON_BN_CLICKED(IDC_BUTTON_MOVIE, &CBullsCarDlg::OnBnClickedButtonMovie)
	ON_BN_CLICKED(IDC_BUTTON_IMAGE, &CBullsCarDlg::OnBnClickedButtonImage)
	ON_BN_CLICKED(IDC_BUTTON_NAVI, &CBullsCarDlg::OnBnClickedButtonNavi)
	ON_BN_CLICKED(IDC_BUTTON_INTERNET, &CBullsCarDlg::OnBnClickedButtonInternet)
END_MESSAGE_MAP()


// CBullsCarDlg �޽��� ó����

BOOL CBullsCarDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	
	MoveWindow(0,0,800,480, true);
	m_pFile = new CLogFile();
	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CBullsCarDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CBullsCarDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CBullsCarDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

void CBullsCarDlg::OnBnClickedButtonClose()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	this->DestroyWindow();
}

void CBullsCarDlg::OnBnClickedButtonMp3()
{
	// mp3
	m_pCMp3 = new CMp3Dlg;
	if(m_pCMp3->Create(IDD_DIALOG_MP3, this)){
		m_pCMp3->MoveWindow(0,0,800,480, true);
		m_pCMp3->ShowWindow(SW_SHOW);
	}	
}

void CBullsCarDlg::OnBnClickedButtonMovie()
{
	// Movie
}

void CBullsCarDlg::OnBnClickedButtonImage()
{
	// Image
	m_pCPhotoDlg = new CPhotoView;
	if(m_pCPhotoDlg->Create(IDD_DIALOG_PHOTO, this)){
		m_pCPhotoDlg->MoveWindow(0,0,800,480, true);
		m_pCPhotoDlg->ShowWindow(SW_SHOW);
		//m_pCPhotoDlg->m_pParentDlg = this;
	}	

}

void CBullsCarDlg::OnBnClickedButtonNavi()
{
	// Navi
}

void CBullsCarDlg::OnBnClickedButtonInternet()
{
	// Internet
}


//---------------------------------------------------------------------------
wchar_t *CBullsCarDlg::ansiTouni(const char *szANSI)
{
	wchar_t					*szwUNI = NULL;
	ULONG					cChar;

	if(NULL == szANSI)
	{
		szwUNI = NULL;

		return NULL;
	}

	cChar =  (ULONG)strlen(szANSI) + 1;

	szwUNI = new wchar_t[cChar * 2];

	if(NULL == szwUNI)
		return NULL;

	if(0 == MultiByteToWideChar(CP_ACP, 0, szANSI, cChar, szwUNI, cChar))
	{
		delete []szwUNI;
		szwUNI = NULL;

		return NULL;
	}

	return szwUNI;
}
//---------------------------------------------------------------------------
char *CBullsCarDlg::SuniToansi(CString Suni)
{
	char					*szData = NULL;
	int						len = 0;

	if(Suni.GetLength() == 0)
		Suni.Format(_T(" "));

	len = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, Suni, Suni.GetLength(), szData, 0, NULL, NULL);

	if(len == 0)
		return NULL;

	szData = new char[len + 1];

	memset(szData, 0x00, len + 1);

	if(NULL == szData)
		return NULL;

	if(0 == WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, Suni, Suni.GetLength(), szData, len + 1, NULL, NULL))
	{
		delete []szData;
		szData = NULL;

		return NULL;
	}

	return szData;
}
//---------------------------------------------------------------------------
char *CBullsCarDlg::uniToansi(const wchar_t* pwstrSrc)
{
	//    ASSERT(pwstrSrc);

	if(NULL == pwstrSrc)
		AfxMessageBox(_T("CSchDateDlg : uniToansi error! => �˼��մϴ� �ٿ��̵�� �����ڿ��� ���� �޼����� �˷� �ּ���!"));

	int						len = 0;

	len = ((int)wcslen(pwstrSrc) + 1) * 2;

	char					*pstr = (char *)malloc(sizeof(char) * len);

	WideCharToMultiByte(CP_ACP, 0, pwstrSrc, -1, pstr, len, NULL, NULL);

	return pstr;
}

void CBullsCarDlg::SetTransparent(int percent)
{
	SLWA pSetLayeredWindowAttributes = NULL;

	HINSTANCE hmodUSER32 = LoadLibrary(L"USER32.DLL");
	pSetLayeredWindowAttributes =
		(SLWA)GetProcAddress(hmodUSER32,"SetLayeredWindowAttributes");

	HWND hwnd = this->m_hWnd;
	SetWindowLong(hwnd,GWL_EXSTYLE,GetWindowLong(hwnd,GWL_EXSTYLE) | WS_EX_LAYERED);
	pSetLayeredWindowAttributes(hwnd, 0, percent, LWA_ALPHA);
}