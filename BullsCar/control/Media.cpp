#include "stdafx.h"
#include "Media.h"

// {8AB4D3D0-C812-11d5-BE17-00A0C90AA8A1}
DEFINE_GUID (CLSID_SDMP4FileSource, 0x8ab4d3d0, 0xc812, 0x11d5, 0xbe, 0x17, 0x0, 0xa0, 0xc9, 0xa, 0xa8, 0xa1);

// {67133BD0-EB71-11d5-A968-0080AD91BD94}
DEFINE_GUID(IID_IDSEM84xx, 0x67133bd0, 0xeb71, 0x11d5, 0xa9, 0x68, 0x0, 0x80, 0xad, 0x91, 0xbd, 0x94);


void CMediaCtrl::HideDisplayAction()
{
	HWND					hOverlay = FindWindow(_T("OverlayWindow"), NULL);

	SendMessage(hOverlay, WM_COMMAND, 51002, 0);
}

BOOL CMediaCtrl::ShowDisplayActionStart()
{
	//..
	HWND					hOverlay = FindWindow(_T("OVERLAYWINDOW"), NULL);
	FILE					*fp = fopen("\\windows\\windowpos", "w") ;

	if(fp != NULL)
	{
		fprintf(fp, "%d %d %d %d %d", 0, 0, 0, 10, 10); //* 해상도가 들어가야 할 자리
		fclose(fp);

		SendMessage(hOverlay, WM_COMMAND, 51001, 0);

		return true;
	}
	//..

	return false;
}

BOOL CMediaCtrl::ShowDisplayAction()
{
	//..
	HWND					hOverlay = FindWindow(_T("OVERLAYWINDOW"), NULL);
	FILE					*fp = fopen("\\windows\\windowpos", "w") ;

	if(fp != NULL)
	{
		fprintf(fp, "%d %d %d %d %d", 0, 0, 0, 1024, 768); //* 해상도가 들어가야 할 자리
		fclose(fp);

		SendMessage(hOverlay, WM_COMMAND, 51001, 0);

		return true;
	}
	//..

	return false;
}

CMediaCtrl::CMediaCtrl()
{
	pGB = NULL;
	pMC = NULL;
	//pME = NULL;
	pVW = NULL;
	pBA = NULL;
	pBV = NULL;
	pMS = NULL;
	pMP = NULL;

	pEM = NULL;
	pRF = NULL;

	//pSF = NULL;
	pIP = NULL;
	pISF = NULL;

	//g_szFileName[0] = L'\0';
	g_bAudioOnly = FALSE;
	g_lVolume = VOLUME_FULL;
	g_psCurrent = Stopped;

	m_pParent = NULL;
}

CMediaCtrl::~CMediaCtrl()
{
}

BOOL CMediaCtrl::IsMPEG4File(WCHAR *str)
{
	int						ch = '.';
	WCHAR					*p = wcsrchr(str, ch);

	if(p && p[1] == 'm' && p[2] == 'p' && p[3] == '4' && p[4] == L'\0')
		return TRUE;

	return FALSE;
}

IPin *CMediaCtrl::GetPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir)
{
	BOOL						bFound = FALSE;
	IEnumPins					*pEnum;
	IPin						*pPin;

	pFilter->EnumPins(&pEnum);

	while(pEnum->Next(1, &pPin, NULL) == S_OK)
	{
		PIN_DIRECTION				PinDirThis;

		pPin->QueryDirection(&PinDirThis);

		if(bFound = (PinDir == PinDirThis))
			break;

		pPin->Release();
	}

	pEnum->Release();

	return (bFound ? pPin : NULL);
}

HRESULT CMediaCtrl::PlayMovieInWindow(LPTSTR szFile)
{
    WCHAR					wFile[MAX_PATH];
    HRESULT					hr;
    //BOOL					b_mp4;

    // Clear open dialog remnants before calling RenderFile()
    //UpdateWindow(ghApp);

    lstrcpy(wFile, szFile);

    // Get the interface for DirectShow's GraphBuilder
    hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, reinterpret_cast<void **>(&pGB));

	if(FAILED(hr))
	{
		Msg(TEXT("CoCreateInstance Error : [%s]"), wFile);
		//printf("PlayMovieInWindow RenderFile Error : [%s] \n", uniToansi(wFile));

		return -1;
	}

    if(SUCCEEDED(hr))
    {
		// Have the graph construct its the appropriate graph automatically
		//JIF(pGB->RenderFile(wFile, NULL));

		hr = pGB->RenderFile(wFile, NULL);

		if(FAILED(hr))
		{
			Msg(TEXT("RenderFile Error : [%s]"), wFile);
			//printf("PlayMovieInWindow RenderFile Error : [%s] \n", uniToansi(wFile));

			return -1;
		}

        // QueryInterface for DirectShow interfaces
        hr = pGB->QueryInterface(IID_IMediaControl, reinterpret_cast<void **>(&pMC));

		if(FAILED(hr))
		{
			Msg(TEXT("QueryInterface IID_IMediaControl Error : [%s]"), wFile);
			//printf("PlayMovieInWindow QueryInterface IID_IMediaControl Error : [%s] \n", uniToansi(wFile));

			return -1;
		}

        //JIF(pGB->QueryInterface(IID_IMediaEventEx, (void **)&pME));
        hr = pGB->QueryInterface(IID_IMediaSeeking, reinterpret_cast<void **>(&pMS));

		if(FAILED(hr))
		{
			Msg(TEXT("QueryInterface IID_IMediaSeeking Error : [%s]"), wFile);
			//printf("PlayMovieInWindow QueryInterface IID_IMediaSeeking Error : [%s] \n", uniToansi(wFile));

			return -1;
		}

		hr = pGB->QueryInterface(IID_IMediaPosition, reinterpret_cast<void **>(&pMP));

		if(FAILED(hr))
		{
			Msg(TEXT("QueryInterface IID_IMediaPosition Error : [%s]"), wFile);
			//printf("PlayMovieInWindow QueryInterface IID_IMediaPosition : [%s] \n", uniToansi(wFile));

			return -1;
		}

        // Query for video interfaces, which may not be relevant for audio files
        hr = pGB->QueryInterface(IID_IVideoWindow, reinterpret_cast<void **>(&pVW));

		if(FAILED(hr))
		{
			Msg(TEXT("QueryInterface IID_IVideoWindow Error : [%s]"), wFile);
			//printf("PlayMovieInWindow QueryInterface IID_IVideoWindow : [%s] \n", uniToansi(wFile));

			return -1;
		}

        hr = pGB->QueryInterface(IID_IBasicVideo, reinterpret_cast<void **>(&pBV));

		if(FAILED(hr))
		{
			Msg(TEXT("QueryInterface IID_IBasicVideo Error : [%s]"), wFile);
			//printf("PlayMovieInWindow QueryInterface IID_IBasicVideo : [%s] \n", uniToansi(wFile));

			return -1;
		}

        // Query for audio interfaces, which may not be relevant for video-only files
        hr = pGB->QueryInterface(IID_IBasicAudio, reinterpret_cast<void **>(&pBA));

		if(FAILED(hr))
		{
			Msg(TEXT("QueryInterface IID_IBasicAudio Error : [%s]"), wFile);
			//printf("PlayMovieInWindow QueryInterface IID_IBasicAudio : [%s] \n", uniToansi(wFile));

			return -1;
		}

        pGB->FindFilterByName(TEXT("Sigma Designs Filter"), reinterpret_cast<IBaseFilter **>(&pRF));

        if(pRF == NULL)
		{
			hr = pGB->FindFilterByName(TEXT("Sigma Designs Renderer Filter"), reinterpret_cast<IBaseFilter **>(&pRF));

			if(FAILED(hr))
			{
				Msg(TEXT("PlayMovieInWindow FindFilterByName Error : [%s]"), wFile);
				//printf("PlayMovieInWindow FindFilterByName : [%s] \n", uniToansi(wFile));

				return -1;
			}
		}

        if(pRF)
		{
			hr = pRF->QueryInterface(IID_IDSEM84xx, reinterpret_cast<void **>(&pEM));

			if(FAILED(hr))
			{
				Msg(TEXT("PlayMovieInWindow QueryInterface IID_IDSEM84xx Error : [%s]"), wFile);
				//printf("PlayMovieInWindow QueryInterface IID_IDSEM84xx Error : [%s] \n", uniToansi(wFile));

				return -1;
			}
		}

        if(!pEM || !pRF)
		{
			Msg(TEXT("!pEM || !pRF : [%s]"), wFile);
			//printf("!pEM || !pRF : [%s] \n", uniToansi(wFile));

			return -1;
		}

        // Run the graph to play the media file
        hr = pMC->Run();

		if(FAILED(hr))
		{
			Msg(TEXT("Run Error : [%s]"), wFile);
			//printf("PlayMovieInWindow Run Error : [%s] \n", uniToansi(wFile));

			return -1;
		}

		//m_pParent->SendMessage(WM_SLIDERCONTROL_SET, 0, 0);
		m_pParent->SliderControlSet();	

        g_psCurrent = Running;
    }

    return hr;
}

HRESULT CMediaCtrl::InitVideoWindow(int nMultiplier, int nDivider)
{
//    LONG					lHeight, lWidth;
    HRESULT					hr = S_OK;
//    RECT					rect;

/*    if(!pBV)
        return S_OK;

    // Read the default video size
    JIF(pBV->GetVideoSize(&lWidth, &lHeight));

    // Account for requests of normal, half, or double size
    lWidth = lWidth * nMultiplier / nDivider;
    lHeight = lHeight * nMultiplier / nDivider;

    //SetWindowPos(NULL, 0, 0, lWidth, lHeight, SWP_NOMOVE | SWP_NOOWNERZORDER);

    int						nTitleHeight = GetSystemMetrics(SM_CYCAPTION);
    int						nBorderWidth = GetSystemMetrics(SM_CXBORDER);
    int						nBorderHeight = GetSystemMetrics(SM_CYBORDER);
*/
/*
    // Account for size of title bar and borders for exact match
    // of window client area to default video size
    SetWindowPos(ghApp, NULL, 0, 0, lWidth + 2 * nBorderWidth, lHeight + nTitleHeight + 2 * nBorderHeight, SWP_NOMOVE | SWP_NOOWNERZORDER);
*/
	// just default to 352 x 240
    //SetWindowPos(NULL, 0, 0, 352, 240, SWP_NOMOVE | SWP_NOOWNERZORDER);

//    GetClientRect(ghApp, &rect); 

//    JIF(pVW->SetWindowPosition(rect.left, rect.top + GetSystemMetrics(SM_CYMENU), rect.right, rect.bottom));

    return hr;
}

HRESULT CMediaCtrl::InitPlayerWindow(void)
{
    // Reset to a default size for audio and after closing a clip
//    SetWindowPos(NULL, 0, 0, DEFAULT_AUDIO_WIDTH, DEFAULT_AUDIO_HEIGHT, SWP_NOMOVE | SWP_NOOWNERZORDER);

    return S_OK;
}

void CMediaCtrl::MoveVideoWindow(void)
{
    HRESULT					hr;
    
    // Track the movement of the container window and resize as needed
    if(pVW)
    {
        RECT					client;

        GetClientRect(ghApp, &client);

        hr = pVW->SetWindowPosition(client.left, client.top + GetSystemMetrics(SM_CYMENU), client.right, client.bottom);
    }

}

void CMediaCtrl::CheckVisibility(void)
{
    long					lVisible;
    HRESULT					hr;

    g_bAudioOnly = FALSE;

    if((!pVW) || (!pBV))
    {
        g_bAudioOnly = TRUE;

        //Msg(TEXT("No video interface.  Assuming audio/MIDI file or unsupported video codec.\r\n"));
        return;
    }

    hr = pVW->get_Visible(&lVisible);

    if(FAILED(hr))
    {
        // If this is an audio-only clip, get_Visible() won't work.
        //
        // Also, if this video is encoded with an unsupported codec,
        // we won't see any video, although the audio will work if it is
        // of a supported format.
        //
        if(hr == E_NOINTERFACE)
        {
            g_bAudioOnly = TRUE;
        }
        else
        {
            Msg(TEXT("Failed(%08lx) in pVW->get_Visible()!\r\n"), hr);
        }
    }

}

void CMediaCtrl::PauseClip(void)
{
    HRESULT					hr;
    
    if(!pMC)
        return;

    // Toggle play/pause behavior
    if((g_psCurrent == Paused) || (g_psCurrent == Stopped))
    {
        g_psCurrent = Running;

        hr = pMC->Run();
    }
    else
    {
        g_psCurrent = Paused;

        hr = pMC->Pause();
    }

//    UpdateMainTitle();
}

void CMediaCtrl::StopClip(void)
{
    HRESULT					hr;
	LONGLONG				pos = 0;
    
    if((!pMC) || (!pMS))
        return;

    hr = pMC->Stop();
    hr = pMS->SetPositions(&pos, AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning);

    // Stop and reset postion to beginning
    if((g_psCurrent == Paused) || (g_psCurrent == Running))
    {
        //LONGLONG				pos = 0;

        //hr = pMC->Stop();

        g_psCurrent = Stopped;

        //hr = pMS->SetPositions(&pos, AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning);

        // Display the first frame to indicate the reset condition
        hr = pMC->Pause();
    }

    //UpdateMainTitle();
}

HRESULT CMediaCtrl::OpenClip(TCHAR *szFileName)
{
    HRESULT					hr;

    if(g_psCurrent != Init)
		CloseClip();

    // Reset status variables
    g_psCurrent = Stopped;
    g_lVolume = VOLUME_FULL;

    hr = PlayMovieInWindow(szFileName);

    // If we couldn't play the clip, clean up
    //if(FAILED(hr))
    //    CloseClip();

	if(hr == -1)
	{
		CloseClip();
		
		hr = 0;
	}
	else
	{
		hr = 1;
	}

	return hr;
}

void CMediaCtrl::ChangeAudioOutput(EAudioOutputMode eMode)
{
  if(pEM)
    pEM->SetAudioOutput(eMode);
}

BOOL CMediaCtrl::GetClipFileName(LPTSTR szName)
{
    static OPENFILENAME		ofn = {0};
    static BOOL				bSetInitialDir = FALSE;

    *szName = 0;

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = NULL;
    ofn.lpstrFilter = FILE_FILTER_TEXT;
    ofn.lpstrCustomFilter = NULL;
    ofn.nFilterIndex = 1;
    ofn.lpstrFile = szName;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrTitle = TEXT("Open Media File...\0");
    ofn.lpstrFileTitle = NULL;
    ofn.lpstrDefExt = TEXT("*\0");
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_READONLY | OFN_PATHMUSTEXIST;
    
    // Remember the path of the first selected file
    if(bSetInitialDir == FALSE)
    {
        ofn.lpstrInitialDir = DEFAULT_MEDIA_PATH;
        bSetInitialDir = TRUE;
    }
    else 
        ofn.lpstrInitialDir = NULL;

    return GetOpenFileName((LPOPENFILENAME)&ofn);
}

void CMediaCtrl::CloseClip()
{
    HRESULT					hr;

    if(pMC)
        hr = pMC->Stop();

    g_psCurrent = Stopped;
    g_bAudioOnly = TRUE;

    CloseInterfaces();

    // Clear file name to allow selection of new file with open dialog
    //g_szFileName[0] = L'\0';

    // No current media state
    g_psCurrent = Init;
/*
    RECT					rect;

//    GetClientRect(ghApp, &rect);
//    InvalidateRect(ghApp, &rect, TRUE);

    UpdateMainTitle();
    InitPlayerWindow();
*/
}

void CMediaCtrl::CloseInterfaces(void)
{
    HRESULT					hr;
    
    // Relinquish ownership (IMPORTANT!) after hiding video window
    if(pVW)
    {
        hr = pVW->put_Visible(OAFALSE);
        hr = pVW->put_Owner(NULL);
    }

//	if(pME)
//		hr = pME->SetNotifyWindow((OAHWND)NULL, 0, 0);

#ifdef REGISTER_FILTERGRAPH
    if(g_dwGraphRegister)
    {
        RemoveGraphFromRot(g_dwGraphRegister);

        g_dwGraphRegister = 0;
    }
#endif

    SAFE_RELEASE(pIP);
    SAFE_RELEASE(pISF);

    SAFE_RELEASE(pEM);
    SAFE_RELEASE(pRF);

    //SAFE_RELEASE(pME);
    SAFE_RELEASE(pMS);
	SAFE_RELEASE(pMP);
    SAFE_RELEASE(pMC);
    SAFE_RELEASE(pBA);
    SAFE_RELEASE(pBV);
    SAFE_RELEASE(pVW);
    //SAFE_RELEASE(pSF);
    SAFE_RELEASE(pGB);
}

#ifdef REGISTER_FILTERGRAPH

HRESULT CMediaCtrl::AddGraphToRot(IUnknown *pUnkGraph, DWORD *pdwRegister) 
{
    IMoniker				*pMoniker;
    IRunningObjectTable		*pROT;

    if(FAILED(GetRunningObjectTable(0, &pROT))) 
	{
        return E_FAIL;
    }

    WCHAR					wsz[128];

    wsprintfW(wsz, L"FilterGraph %08x pid %08x", (DWORD_PTR)pUnkGraph, GetCurrentProcessId());

    HRESULT					hr = CreateItemMoniker(L"!", wsz, &pMoniker);

    if(SUCCEEDED(hr)) 
	{
        hr = pROT->Register(0, pUnkGraph, pMoniker, pdwRegister);
        pMoniker->Release();
    }

    pROT->Release();

    return hr;
}

void CMediaCtrl::RemoveGraphFromRot(DWORD pdwRegister)
{
    IRunningObjectTable		*pROT;

    if(SUCCEEDED(GetRunningObjectTable(0, &pROT))) 
	{
        pROT->Revoke(pdwRegister);
        pROT->Release();
    }
}

#endif

void Msg(TCHAR *szFormat, ...)
{
    TCHAR					szBuffer[512];  // Large buffer for very long filenames (like with HTTP)
    va_list					pArgs;

    va_start(pArgs, szFormat);

    vswprintf(szBuffer, szFormat, pArgs);

    va_end(pArgs);

    MessageBox(NULL, szBuffer, TEXT("PlayWnd Sample"), MB_OK);
}

HRESULT CMediaCtrl::ToggleMute(void)
{
    HRESULT					hr = S_OK;

	LONG					Volume = 0;

    if((!pGB) || (!pBA))
        return S_OK;

    // Read current volume
    hr = pBA->get_Volume(&Volume);

    if(hr == E_NOTIMPL)
    {
        // Fail quietly if this is a video-only media file
        return S_OK;
    }
    else if(FAILED(hr))
    {
        Msg(TEXT("Failed to read audio volume!  hr=0x%x\r\n"), hr);

        return hr;
    }

    // Switch volume levels
    if(Volume != VOLUME_SILENCE)
        Volume = VOLUME_SILENCE;
    else
        Volume = g_lVolume;

    // Set new volume
    JIF(pBA->put_Volume(Volume));

//    UpdateMainTitle();

    return hr;
}

void CMediaCtrl::UpdateMainTitle(void)
{
    TCHAR					szTitle[MAX_PATH], szFile[MAX_PATH];

    // If no file is loaded, just show the application title
    if(g_szFileName[0] == L'\0')
    {
        wsprintf(szTitle, TEXT("%s"), APPLICATIONNAME);
    }
    // Otherwise, show useful information, including filename and play state
    else
    {
        // Get file name without full path		
        GetFilename(g_szFileName, szFile);

        // Update the window title to show muted/normal status
        wsprintf(szTitle, TEXT("%s [%s] %s%s"),
                szFile,
                g_bAudioOnly ? TEXT("Audio") : TEXT("Video"),
                (g_lVolume == VOLUME_SILENCE) ? TEXT("(Muted)") : TEXT(""),
                (g_psCurrent == Paused) ? TEXT("(Paused)") : TEXT(""));
    }

//    SetWindowText(ghApp, szTitle);
}

void CMediaCtrl::GetFilename(TCHAR *pszFull, TCHAR *pszFile)
{
    int						nLength;
    TCHAR					szPath[MAX_PATH] = {0};
    BOOL					bSetFilename = FALSE;

    _tcscpy(szPath, pszFull);

    nLength = _tcslen(szPath);

    for(int i = nLength - 1; i >= 0; i--)
    {
        if((szPath[i] == '\\') || (szPath[i] == '/'))
        {
            szPath[i] = '\0';
            lstrcpy(pszFile, &szPath[i + 1]);
            bSetFilename = TRUE;
            break;
        }
    }

    // If there was no path given (just a file name), then
    // just copy the full path to the target path.
    if(!bSetFilename)
        _tcscpy(pszFile, pszFull);
}

HRESULT CMediaCtrl::ToggleFullScreen(void)
{
    HRESULT					hr = S_OK;
    LONG					lMode;
    static HWND				hDrain = 0;

    // Don't bother with full-screen for audio-only files
    if((g_bAudioOnly) || (!pVW))
        return S_OK;

    if(!pVW)
        return S_OK;

    // Read current state
    JIF(pVW->get_FullScreenMode(&lMode));

    if(lMode == OAFALSE)
    {
        // Save current message drain
        LIF(pVW->get_MessageDrain((OAHWND *)&hDrain));

        // Set message drain to application main window
        LIF(pVW->put_MessageDrain((OAHWND)ghApp));

        // Switch to full-screen mode
        lMode = OATRUE;
        JIF(pVW->put_FullScreenMode(lMode));

		Msg(TEXT("Failed(0x%08lx) to stop media clip!\r\n"), hr);
    }
    else
    {
        // Switch back to windowed mode
        lMode = OAFALSE;
        JIF(pVW->put_FullScreenMode(lMode));

        // Undo change of message drain
        LIF(pVW->put_MessageDrain((OAHWND)hDrain));

        // Reset video window
        LIF(pVW->SetWindowForeground(-1));

        // Reclaim keyboard focus for player application
//        UpdateWindow(ghApp);
//        SetForegroundWindow(ghApp);
//        SetFocus(ghApp);
    }

    return hr;
}

HRESULT CMediaCtrl::HandleGraphEvent(void)
{
    HRESULT					hr = S_OK;

//    while(SUCCEEDED(pME->GetEvent(&evCode, &evParam1, &evParam2, 0)))
    {
        // Spin through the events
//        hr = pME->FreeEventParams(evCode, evParam1, evParam2);

//        if(EC_COMPLETE == evCode)
        {
            LONGLONG				pos = 0;

            // Reset to first frame of movie
            hr = pMS->SetPositions(&pos, AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning);

            if(FAILED(hr))
            {
                // Some custom filters (like the Windows CE MIDI filter) 
                // may not implement seeking interfaces (IMediaSeeking)
                // to allow seeking to the start.  In that case, just stop 
                // and restart for the same effect.  This should not be
                // necessary in most cases.
                if(FAILED(hr = pMC->Stop()))
                {
                    Msg(TEXT("Failed(0x%08lx) to stop media clip!\r\n"), hr);
                    return false;
                }

                if(FAILED(hr = pMC->Run()))
                {
                    Msg(TEXT("Failed(0x%08lx) to reset media clip!\r\n"), hr);
                    return false;
                }
            }
        }
    }

    return hr;
}

void CMediaCtrl::SetWnd(CDMXPlayerDlg *pParent)
{
	if(pParent == NULL)
		return;

	m_pParent = pParent;

	ghApp = m_pParent->m_hWnd;
}

void CMediaCtrl::GetCurrentPosition(double *lCur)
{
	pMP->get_CurrentPosition(lCur);
}

void CMediaCtrl::SetCurrentPosition(REFTIME lCur)
{
	pMP->put_CurrentPosition(lCur);
}

void CMediaCtrl::GetDuration(double *lTotal)
{
	pMP->get_Duration(lTotal);
}

void CMediaCtrl::GetRate(double *lCur)
{
	pMP->get_Rate(lCur);
}

bool CMediaCtrl::SetRate(double lCur)
{
	HRESULT result = pMP->put_Rate(lCur);

	if(S_OK == result)
		return true;
	else
		return false;
}

bool CMediaCtrl::GetVolume(LONG *Volume)
{
    HRESULT					hr = S_OK;

    if((!pGB) || (!pBA))
        return false;

    // Read current volume
    hr = pBA->get_Volume(Volume);

    if(hr == E_NOTIMPL)
    {
        // Fail quietly if this is a video-only media file
        return true;
    }
    else if(FAILED(hr))
    {
        Msg(TEXT("Failed to read audio volume!  hr=0x%x\r\n"), hr);

        return false;
    }

	return true;
}

bool CMediaCtrl::SetVolume(LONG Volume)
{
	HRESULT					hr = S_OK;

    if((!pGB) || (!pBA))
        return false;

	g_lVolume = Volume;

    // Set new volume
    JIF(pBA->put_Volume(g_lVolume));

	return true;
}

bool CMediaCtrl::SetBrightness()
{
	if(!pEM)
		return false;

	LONG					currVal;

	pEM->GetBrightness(&currVal);

	if(!GetKeyState(VK_CONTROL))
	{
		currVal += 100;

		if(currVal > 1000) 
			currVal -= 1000;
	}
	else
	{
		currVal -= 100;

		if(currVal < 0) 
			currVal += 1000;
	}

	pEM->SetBrightness(currVal);

	return true;
}

bool CMediaCtrl::SetContrast()
{
	if(!pEM)
		return false;

	LONG				currVal;

	pEM->GetContrast(&currVal);

	if(!GetKeyState(VK_CONTROL))
	{
		currVal += 100;

		if(currVal > 1000) 
			currVal -= 1000;
	}
	else
	{
		currVal -= 100;

		if(currVal < 0) 
			currVal += 1000;
	}

	pEM->SetContrast(currVal);

	return true;
}

bool CMediaCtrl::SetSaturation()
{
	if(!pEM)
		return false;

	LONG				currVal;

	pEM->GetSaturation(&currVal);

	if(!GetKeyState(VK_CONTROL))
	{
		currVal += 100;

		if(currVal > 1000) 
			currVal -= 1000;
	}
	else
	{
		currVal -= 100;

		if(currVal < 0) 
			currVal += 1000;
	}

	pEM->SetSaturation(currVal);

	return true;
}

char *CMediaCtrl::uniToansi(const wchar_t* pwstrSrc)
{
//    ASSERT(pwstrSrc);

    if(NULL == pwstrSrc)
	{
		AfxMessageBox(_T("CMediaCtrl : uniToansi error! => 죄송합니다 다원미디어 개발자에게 버그 메세지를 알려 주세요!"));

        return NULL;
	}

    int						len = 0;

    len = (wcslen(pwstrSrc) + 1) * 2;

    char					*pstr = (char *)malloc(sizeof(char) * len);

    WideCharToMultiByte(CP_ACP, 0, pwstrSrc, -1, pstr, len, NULL, NULL);

    return pstr;
}

wchar_t *CMediaCtrl::ansiTouni(const char *szANSI)
{
    wchar_t					*szwUNI = NULL;
    ULONG					cChar;

    if(NULL == szANSI)
	{
        szwUNI = NULL;

        return NULL;
    }

    cChar =  strlen(szANSI) + 1;

    szwUNI = new wchar_t[cChar * 2];

    if(NULL == szwUNI)
		return NULL;

    if(0 == MultiByteToWideChar(CP_ACP, 0, szANSI, cChar, szwUNI, cChar))
	{
        delete [] szwUNI;
        szwUNI = NULL;

        return NULL;
    }

    return szwUNI;
}
//---------------------------------------------------------------------------
char *CMediaCtrl::SuniToansi(CString Suni)
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

int CMediaCtrl::GetFrameRate()
{
	IQualProp				*m_pIQP = NULL;
	int						nFPS = 0;

	pRF->QueryInterface(IID_IQualProp, reinterpret_cast<void **>(&m_pIQP));	
	m_pIQP->get_AvgFrameRate(&nFPS);

	return nFPS;
}
