#ifndef _MEDIACTRL_H_
#define _MEDIACTRL_H_

#include "DMXPlayer.h"
#include "DMXPlayerDlg.h"

#include <streams.h>
#include <commctrl.h>
#include <commdlg.h>
#include <stdio.h>
#include <tchar.h>

#include <initguid.h>
#include "idsem84xx.h"
#include "rm84cmn.h"

#include "playwnd.h"


class CMediaCtrl
{
public :
	// 持失切人 社瑚切
	CMediaCtrl();
	virtual ~CMediaCtrl();

public :
	HRESULT					InitPlayerWindow(void);
	HRESULT					InitVideoWindow(int nMultiplier, int nDivider);
	HRESULT					HandleGraphEvent(void);

	BOOL					GetClipFileName(LPTSTR szName);

	void					PaintAudioWindow(void);
	void					MoveVideoWindow(void);
	void					CheckVisibility(void);
	void					CloseInterfaces(void);

	HRESULT					OpenClip(TCHAR *szFileName);
	void					PauseClip(void);
	void					StopClip(void);
	void					CloseClip(void);

	void					UpdateMainTitle(void);
	void					GetFilename(TCHAR *pszFull, TCHAR *pszFile);

	HRESULT					AddGraphToRot(IUnknown *pUnkGraph, DWORD *pdwRegister);
	void					RemoveGraphFromRot(DWORD pdwRegister);

	//.
	BOOL					IsMPEG4File(WCHAR *str);
	IPin					*GetPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir);
	HRESULT					PlayMovieInWindow(LPTSTR szFile);
	void					ChangeAudioOutput(EAudioOutputMode eMode);
	HRESULT					ToggleMute(void);
	HRESULT					ToggleFullScreen(void);

	void					SetWnd(CDMXPlayerDlg *pParent = NULL);

	void					GetCurrentPosition(double *lCur);
	void					SetCurrentPosition(REFTIME lCur);
	void					GetDuration(double *lTotal);
	void					GetRate(double *lCur);
	bool					SetRate(double lCur);
	bool					GetVolume(LONG *Volume);
	bool					SetVolume(LONG Volume);
	bool					SetBrightness();
	bool					SetContrast();
	bool					SetSaturation();

	BOOL					ShowDisplayAction();
	void					HideDisplayAction();
	BOOL					ShowDisplayActionStart();

	char					*uniToansi(const wchar_t* pwstrSrc);
	wchar_t					*ansiTouni(const char *szANSI);
	char					*SuniToansi(CString Suni);

	int						GetFrameRate();

public :
	// DirectShow interfaces
	IGraphBuilder			*pGB;
	IMediaControl			*pMC;
	//IMediaEventEx			*pME;
	IVideoWindow			*pVW;
	IBasicAudio				*pBA;
	IBasicVideo				*pBV;
	IMediaSeeking			*pMS;
	IMediaPosition			*pMP;

	IDSEM84xx				*pEM;
	IBaseFilter				*pRF;

	IBaseFilter				*pSF;
	IPin					*pIP;
	IFileSourceFilter		*pISF;

	HWND					ghApp;

	LONG					g_lVolume;

	CDMXPlayerDlg			*m_pParent;

protected:
	TCHAR					g_szFileName[MAX_PATH];
	BOOL					g_bAudioOnly;	
	PLAYSTATE				g_psCurrent;	
};

#endif
