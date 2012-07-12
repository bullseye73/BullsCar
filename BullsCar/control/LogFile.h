#ifndef _LOGFILE_H_
#define _LOGFILE_H_

#include <stdio.h>

class CLogFile
{
public:
	CLogFile();
	virtual ~CLogFile();

public:
	bool					Open(char *String);
	void					AddString(char *String);
	void					Close();
	bool					FindFile(char *String);
	void					write_date_time(void);

	wchar_t					*ansiTouni(const char *szANSI);
	char					*SuniToansi(CString Suni);
	char					*uniToansi(const wchar_t* pwstrSrc);

public:
	FILE					*fp;
	char					strLogFile[255];
	SYSTEMTIME				st;
};

#endif //_LOGFILE_H_

