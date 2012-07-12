// BullsCarDlg.h : 헤더 파일
//

#pragma once
//////////////////////////////////////////////////////////////////////////
// dialog include file
#include "Mp3Dlg.h"
#include "PhotoView.h"

//////////////////////////////////////////////////////////////////////////
#include "control/LogFile.h"
//////////////////////////////////////////////////////////////////////////
class CMp3Dlg;
class CPhotoView;

class CLogFile;
//////////////////////////////////////////////////////////////////////////

// CBullsCarDlg 대화 상자
class CBullsCarDlg : public CDialog
{
// 생성입니다.
public:
	CBullsCarDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_BULLSCAR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
//////////////////////////////////////////////////////////////////////////
	CMp3Dlg* m_pCMp3;
	CPhotoView* m_pCPhotoDlg;
//////////////////////////////////////////////////////////////////////////
	CLogFile* m_pFile;
//////////////////////////////////////////////////////////////////////////

public:
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnBnClickedButtonMp3();
	afx_msg void OnBnClickedButtonMovie();
	afx_msg void OnBnClickedButtonImage();
	afx_msg void OnBnClickedButtonNavi();
	afx_msg void OnBnClickedButtonInternet();

	char* uniToansi(const wchar_t* pwstrSrc);
	char* SuniToansi(CString Suni);
	wchar_t* ansiTouni(const char *szANSI);

	typedef BOOL(WINAPI *SLWA)(HWND,COLORREF,BYTE,DWORD);
	void SetTransparent(int percent);

};
