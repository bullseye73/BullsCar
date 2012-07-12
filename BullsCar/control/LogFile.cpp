#include "stdafx.h"

#include "LogFile.h"
#include <string.h>
#include <Windows.h>
#include <time.h>

void CLogFile::write_date_time(void) 
{ 
	GetLocalTime(&st);

	fprintf(fp, "[%d년, %d월, %d일, %d시, %d분, %d초] \r\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
} 

CLogFile::CLogFile()
{
	fp = NULL;
}

CLogFile::~CLogFile()
{
	if(fp)
	{
		fclose(fp);
		fp = NULL;
	}
}

bool CLogFile::Open(char *String)
{
	strcpy(strLogFile, String);	

	fp = fopen(String, "w+");

	if(fp)
	{
		write_date_time();

		if(fp)
		{
			fclose(fp);
			fp = NULL;
		}

		fp = fopen(strLogFile, "a");

		if(fp)
			return true;
		else
			return false;
	}
	else
	{
		return false;
	}
}

void CLogFile::AddString(char *String)
{
	if(fp)
	{
		;
	}
	else
	{
		if(fp)
		{
			fclose(fp);
			fp = NULL;
		}

		fp = fopen(strLogFile, "a");
	}	

	//시간 출력...
	GetLocalTime(&st);

	fprintf(fp, "[%d-%d, %d:%d:%d] %s \r\n", st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, String);

	//fprintf(fp, "%s \r\n", String);
	fflush(fp);
}

void CLogFile::Close()
{
	fflush(fp);

	if(fp)
	{
		fclose(fp);
		fp = NULL;
	}
}

bool CLogFile::FindFile(char *String)
{
    HANDLE					hSrch;
    WIN32_FIND_DATA			wfd;
    BOOL					bResult = TRUE;

    strcat(String, "\\*.*");

	//printf("%s \n", String);

    hSrch = FindFirstFile(ansiTouni(String), &wfd);

    while(bResult)
	{
		//디렉토리냐? 
        if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
		{			            
        }

		//strcpy(wfd.cFileName, wfd.cFileName);

        bResult = FindNextFile(hSrch, &wfd);

		break;
    }

    FindClose(hSrch);

	return bResult;
}

wchar_t *CLogFile::ansiTouni(const char *szANSI)
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
        delete []szwUNI;
        szwUNI = NULL;

        return NULL;
    }

    return szwUNI;
}

char *CLogFile::SuniToansi(CString Suni)
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

char *CLogFile::uniToansi(const wchar_t* pwstrSrc)
{
//    ASSERT(pwstrSrc);

    if(NULL == pwstrSrc)
		AfxMessageBox(_T("CLogFile : uniToansi error! => 죄송합니다 다원미디어 개발자에게 버그 메세지를 알려 주세요!"));

    int						len = 0;

    len = (wcslen(pwstrSrc) + 1) * 2;

    char					*pstr = (char *)malloc(sizeof(char) * len);

    WideCharToMultiByte(CP_ACP, 0, pwstrSrc, -1, pstr, len, NULL, NULL);

    return pstr;
}

