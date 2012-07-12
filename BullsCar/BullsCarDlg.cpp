// BullsCarDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "BullsCar.h"
#include "BullsCarDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBullsCarDlg 대화 상자


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


// CBullsCarDlg 메시지 처리기

BOOL CBullsCarDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	
	MoveWindow(0,0,800,480, true);
	m_pFile = new CLogFile();
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CBullsCarDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CBullsCarDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CBullsCarDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CBullsCarDlg::OnBnClickedButtonClose()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
		AfxMessageBox(_T("CSchDateDlg : uniToansi error! => 죄송합니다 다원미디어 개발자에게 버그 메세지를 알려 주세요!"));

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