#include "stdafx.h"
#include <stdio.h>
#include "MyListCtrl.h"
#include "resource.h"

//--check
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 생성자
CMyListCtrl::CMyListCtrl()
{
	m_nHighlight = 0;
	m_nType = 0;

	m_crWindow = ::GetSysColor(COLOR_WINDOW);
	m_crWindowText = ::GetSysColor(COLOR_WINDOWTEXT);

	m_crHighLight = RGB(247, 147, 17);

	m_CurrentMark = 0;

	m_bCheckLine = false;
}

// 소멸자
CMyListCtrl::~CMyListCtrl()
{
}

BEGIN_MESSAGE_MAP(CMyListCtrl, CListCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_MEASUREITEM_REFLECT()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

void CMyListCtrl::Init()
{
    // Change the style of the control
	VERIFY(ModifyStyle(LVS_TYPEMASK       | // this styles are removed
                       LVS_SHOWSELALWAYS  |
                       LVS_EDITLABELS,
                       LVS_REPORT         | // this styles are added
                       LVS_OWNERDRAWFIXED |
                       LVS_SINGLESEL));

	SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
}

void CMyListCtrl::PreSubclassWindow() 
{
	//use our custom CHeaderCtrl as long as there
	//is a headerctrl object to subclass
	Init();

	CFont m_NewHeaderFont;

	m_NewHeaderFont.CreatePointFont(200, _T("MS Serif"));

	CHeaderCtrl* pHeader = NULL;

	pHeader = GetHeaderCtrl();

	if(pHeader == NULL)
		return;
	
	pHeader->SetFont(&m_NewHeaderFont);

	if(GetHeaderCtrl())
		m_SkinHeaderCtrl.SubclassWindow(GetHeaderCtrl()->m_hWnd);


	CListCtrl::PreSubclassWindow();
}

INT CMyListCtrl::AddListviewColumn(int iItem, int cx, LPSTR text, int arrange)
{
	LV_COLUMN				column;

	column.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	
	if(arrange == 0)
		column.fmt = LVCFMT_LEFT;
	else 
	{
		if(arrange == 1)
			column.fmt = LVCFMT_CENTER;
		else 
		{
			if(arrange == 2)
				column.fmt = LVCFMT_RIGHT;
		}
	}

	column.cx = cx;
	//column.pszText = (unsigned short *)text;
	column.pszText = ansiTouni(text);
	column.cchTextMax = (int)strlen(text) + 5;
	column.iSubItem = iItem;

	return InsertColumn(iItem, &column);
}

INT CMyListCtrl::AddListviewData(int iItem, int iSubItem, LPSTR text, int imode, int image)
{
	LV_ITEM					item;

	if(imode == 1)
	{
		item.mask = LVIF_TEXT | LVIF_IMAGE;
		item.iImage = image;
	}
	else
	{
		item.mask = LVIF_TEXT;
	}
	item.iItem = iItem;
	item.iSubItem = iSubItem;
	item.pszText = ansiTouni(text);
	item.cchTextMax = 256;

	if(iSubItem == 0)
	{
		if(imode == 1)
			return	InsertItem(&item);
		else
			return	InsertItem(iItem, ansiTouni(text), -1);
	}
	else
		return	SetItem(&item);
}

CHAR *CMyListCtrl::CutVideoName(CHAR *name)
{
/*
    CHAR					*pch, *pcr = name;

	for(pch = pcr; *pcr != '\0'; pcr++)
	{
        if(*pcr == '/') 
			pch = pcr + 1;
    }
*/
    return name;
}

void CMyListCtrl::SwapData(SInputData *pData, int first, int second)
{
	SInputData		tempData;

	tempData = pData[first];
	pData[first] = pData[second];
	pData[second] = tempData;
}

void CMyListCtrl::SwapFtpData(SFtpFileData *pData, int first, int second)
{
	SFtpFileData		tempData;

	tempData = pData[first];
	pData[first] = pData[second];
	pData[second] = tempData;
}

INT CMyListCtrl::CompareVideoName(LPCTSTR str1, LPCTSTR str2)
{
	CString					fs, ss;

	fs = str1;
	ss = str2;
	

	return fs.CompareNoCase(ss);
}

void CMyListCtrl::QuickSortData(SInputData *pIData, int type, int fieldIndex, int direction,int left, int right, int nFlag)
{
	int						i, j, middle;
	SInputData				tempData;
	CMyListCtrl				cvmv;

	if(right - left > 0)
	{
		middle = (left + right) / 2;
		tempData = pIData[middle]; 
		i = left; 
		j = right;
		
		do
		{
			switch(nFlag)
			{
			case 0:
				switch(fieldIndex)
				{
					case 0:
						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cVodTitle)), ansiTouni(cvmv.CutVideoName(tempData.cVodTitle))) < 0) i++;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cVodTitle)), ansiTouni(cvmv.CutVideoName(tempData.cVodTitle))) > 0) i++;

						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cVodTitle)), ansiTouni(cvmv.CutVideoName(tempData.cVodTitle))) > 0) j--;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cVodTitle)), ansiTouni(cvmv.CutVideoName(tempData.cVodTitle))) < 0) j--;
						break;
					case 1:
						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cMenName)), ansiTouni(cvmv.CutVideoName(tempData.cMenName))) < 0) i++;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cMenName)), ansiTouni(cvmv.CutVideoName(tempData.cMenName))) > 0) i++;

						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cMenName)), ansiTouni(cvmv.CutVideoName(tempData.cMenName))) > 0) j--;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cMenName)), ansiTouni(cvmv.CutVideoName(tempData.cMenName))) < 0) j--;
						break;
					case 2:
						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cDate)), ansiTouni(cvmv.CutVideoName(tempData.cDate))) < 0) i++;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cDate)), ansiTouni(cvmv.CutVideoName(tempData.cDate))) > 0) i++;

						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cDate)), ansiTouni(cvmv.CutVideoName(tempData.cDate))) > 0) j--;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cDate)), ansiTouni(cvmv.CutVideoName(tempData.cDate))) < 0) j--;
						break;
					case 3:
						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cRunTime)), ansiTouni(cvmv.CutVideoName(tempData.cRunTime))) < 0) i++;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cRunTime)), ansiTouni(cvmv.CutVideoName(tempData.cRunTime))) > 0) i++;

						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cRunTime)), ansiTouni(cvmv.CutVideoName(tempData.cRunTime))) > 0) j--;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cRunTime)), ansiTouni(cvmv.CutVideoName(tempData.cRunTime))) < 0) j--;
						break;
					case 4:
						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cVodNum)), ansiTouni(cvmv.CutVideoName(tempData.cVodNum))) < 0) i++;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cVodNum)), ansiTouni(cvmv.CutVideoName(tempData.cVodNum))) > 0) i++;

						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cVodNum)), ansiTouni(cvmv.CutVideoName(tempData.cVodNum))) > 0) j--;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cVodNum)), ansiTouni(cvmv.CutVideoName(tempData.cVodNum))) < 0) j--;
						break;
					case 5:
						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cVodName)), ansiTouni(cvmv.CutVideoName(tempData.cVodName))) < 0) i++;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cVodName)), ansiTouni(cvmv.CutVideoName(tempData.cVodName))) > 0) i++;

						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cVodName)), ansiTouni(cvmv.CutVideoName(tempData.cVodName))) > 0) j--;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cVodName)), ansiTouni(cvmv.CutVideoName(tempData.cVodName))) < 0) j--;
						break;
					case 6:
						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cKind)), ansiTouni(cvmv.CutVideoName(tempData.cKind))) < 0) i++;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cKind)), ansiTouni(cvmv.CutVideoName(tempData.cKind))) > 0) i++;

						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cKind)), ansiTouni(cvmv.CutVideoName(tempData.cKind))) > 0) j--;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cKind)), ansiTouni(cvmv.CutVideoName(tempData.cKind))) < 0) j--;
						break;
					case 7:
						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cNum)), ansiTouni(cvmv.CutVideoName(tempData.cNum))) < 0) i++;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cNum)), ansiTouni(cvmv.CutVideoName(tempData.cNum))) > 0) i++;

						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cNum)), ansiTouni(cvmv.CutVideoName(tempData.cNum))) > 0) j--;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cNum)), ansiTouni(cvmv.CutVideoName(tempData.cNum))) < 0) j--;
						break;
					case 8:
						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cEtc)), ansiTouni(cvmv.CutVideoName(tempData.cEtc))) < 0) i++;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cEtc)), ansiTouni(cvmv.CutVideoName(tempData.cEtc))) > 0) i++;

						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cEtc)), ansiTouni(cvmv.CutVideoName(tempData.cEtc))) > 0) j--;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cEtc)), ansiTouni(cvmv.CutVideoName(tempData.cEtc))) < 0) j--;
						break;
				}
				break;
			case 1:
				switch(fieldIndex)
				{
					case 0:
						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cKind)), ansiTouni(cvmv.CutVideoName(tempData.cKind))) < 0) i++;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cKind)), ansiTouni(cvmv.CutVideoName(tempData.cKind))) > 0) i++;

						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cKind)), ansiTouni(cvmv.CutVideoName(tempData.cKind))) > 0) j--;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cKind)), ansiTouni(cvmv.CutVideoName(tempData.cKind))) < 0) j--;
							
						break;
					case 1:
						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cVodTitle)), ansiTouni(cvmv.CutVideoName(tempData.cVodTitle))) < 0) i++;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cVodTitle)), ansiTouni(cvmv.CutVideoName(tempData.cVodTitle))) > 0) i++;

						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cVodTitle)), ansiTouni(cvmv.CutVideoName(tempData.cVodTitle))) > 0) j--;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cVodTitle)), ansiTouni(cvmv.CutVideoName(tempData.cVodTitle))) < 0) j--;
						break;
					case 2:
						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cMenName)), ansiTouni(cvmv.CutVideoName(tempData.cMenName))) < 0) i++;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cMenName)), ansiTouni(cvmv.CutVideoName(tempData.cMenName))) > 0) i++;

						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cMenName)), ansiTouni(cvmv.CutVideoName(tempData.cMenName))) > 0) j--;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cMenName)), ansiTouni(cvmv.CutVideoName(tempData.cMenName))) < 0) j--;
						break;
					case 3:
						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cDate)), ansiTouni(cvmv.CutVideoName(tempData.cDate))) < 0) i++;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cDate)), ansiTouni(cvmv.CutVideoName(tempData.cDate))) > 0) i++;

						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cDate)), ansiTouni(cvmv.CutVideoName(tempData.cDate))) > 0) j--;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cDate)), ansiTouni(cvmv.CutVideoName(tempData.cDate))) < 0) j--;
						break;
					case 4:
						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cVodNum)), ansiTouni(cvmv.CutVideoName(tempData.cVodNum))) < 0) i++;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cVodNum)), ansiTouni(cvmv.CutVideoName(tempData.cVodNum))) > 0) i++;

						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cVodNum)), ansiTouni(cvmv.CutVideoName(tempData.cVodNum))) > 0) j--;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cVodNum)), ansiTouni(cvmv.CutVideoName(tempData.cVodNum))) < 0) j--;
						break;
					case 5:
						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cEtc)), ansiTouni(cvmv.CutVideoName(tempData.cEtc))) < 0) i++;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cEtc)), ansiTouni(cvmv.CutVideoName(tempData.cEtc))) > 0) i++;

						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cEtc)), ansiTouni(cvmv.CutVideoName(tempData.cEtc))) > 0) j--;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cEtc)), ansiTouni(cvmv.CutVideoName(tempData.cEtc))) < 0) j--;
						break;
			
				} //switch(fieldindex)
				break;
			case 2:		// 제목별 검색
				switch(fieldIndex)
				{
					case 0:
						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cVodName)), ansiTouni(cvmv.CutVideoName(tempData.cVodName))) < 0) i++;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cVodName)), ansiTouni(cvmv.CutVideoName(tempData.cVodName))) > 0) i++;

						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cVodName)), ansiTouni(cvmv.CutVideoName(tempData.cVodName))) > 0) j--;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cVodName)), ansiTouni(cvmv.CutVideoName(tempData.cVodName))) < 0) j--;
							
						break;
					case 1:
						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cVodTitle)), ansiTouni(cvmv.CutVideoName(tempData.cVodTitle))) < 0) i++;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cVodTitle)), ansiTouni(cvmv.CutVideoName(tempData.cVodTitle))) > 0) i++;

						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cVodTitle)), ansiTouni(cvmv.CutVideoName(tempData.cVodTitle))) > 0) j--;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cVodTitle)), ansiTouni(cvmv.CutVideoName(tempData.cVodTitle))) < 0) j--;
						break;
					case 2:
						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cMenName)), ansiTouni(cvmv.CutVideoName(tempData.cMenName))) < 0) i++;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cMenName)), ansiTouni(cvmv.CutVideoName(tempData.cMenName))) > 0) i++;

						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cMenName)), ansiTouni(cvmv.CutVideoName(tempData.cMenName))) > 0) j--;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cMenName)), ansiTouni(cvmv.CutVideoName(tempData.cMenName))) < 0) j--;
						break;
					case 3:
						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cDate)), ansiTouni(cvmv.CutVideoName(tempData.cDate))) < 0) i++;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cDate)), ansiTouni(cvmv.CutVideoName(tempData.cDate))) > 0) i++;

						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cDate)), ansiTouni(cvmv.CutVideoName(tempData.cDate))) > 0) j--;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cDate)), ansiTouni(cvmv.CutVideoName(tempData.cDate))) < 0) j--;
						break;
					case 4:			// 숫자정렬.
						if(direction) 
							while(atoi(cvmv.CutVideoName(pIData[i].cVodNum)) < atoi(cvmv.CutVideoName(tempData.cVodNum))) i++;
						else 
							while(atoi(cvmv.CutVideoName(pIData[i].cVodNum)) > atoi(cvmv.CutVideoName(tempData.cVodNum))) i++;

						if(direction) 
							while(atoi(cvmv.CutVideoName(pIData[j].cVodNum)) > atoi(cvmv.CutVideoName(tempData.cVodNum))) j--;
						else 
							while(atoi(cvmv.CutVideoName(pIData[j].cVodNum)) < atoi(cvmv.CutVideoName(tempData.cVodNum))) j--;
						break;
					case 5:
						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cEtc)), ansiTouni(cvmv.CutVideoName(tempData.cEtc))) < 0) i++;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cEtc)), ansiTouni(cvmv.CutVideoName(tempData.cEtc))) > 0) i++;

						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cEtc)), ansiTouni(cvmv.CutVideoName(tempData.cEtc))) > 0) j--;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cEtc)), ansiTouni(cvmv.CutVideoName(tempData.cEtc))) < 0) j--;
						break;
					case 6:
						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cFileName)), ansiTouni(cvmv.CutVideoName(tempData.cFileName))) < 0) i++;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cFileName)), ansiTouni(cvmv.CutVideoName(tempData.cFileName))) > 0) i++;

						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cFileName)), ansiTouni(cvmv.CutVideoName(tempData.cFileName))) > 0) j--;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cFileName)), ansiTouni(cvmv.CutVideoName(tempData.cFileName))) < 0) j--;
						break;
			
				}
				break;
			case 3:
				switch(fieldIndex)
				{
					case 0:
						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cDate)), ansiTouni(cvmv.CutVideoName(tempData.cDate))) < 0) i++;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cDate)), ansiTouni(cvmv.CutVideoName(tempData.cDate))) > 0) i++;

						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cDate)), ansiTouni(cvmv.CutVideoName(tempData.cDate))) > 0) j--;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cDate)), ansiTouni(cvmv.CutVideoName(tempData.cDate))) < 0) j--;
						break;
					case 1:
						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cVodTitle)), ansiTouni(cvmv.CutVideoName(tempData.cVodTitle))) < 0) i++;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cVodTitle)), ansiTouni(cvmv.CutVideoName(tempData.cVodTitle))) > 0) i++;

						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cVodTitle)), ansiTouni(cvmv.CutVideoName(tempData.cVodTitle))) > 0) j--;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cVodTitle)), ansiTouni(cvmv.CutVideoName(tempData.cVodTitle))) < 0) j--;
						break;
					case 2:
						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cMenName)), ansiTouni(cvmv.CutVideoName(tempData.cMenName))) < 0) i++;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cMenName)), ansiTouni(cvmv.CutVideoName(tempData.cMenName))) > 0) i++;

						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cMenName)), ansiTouni(cvmv.CutVideoName(tempData.cMenName))) > 0) j--;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cMenName)), ansiTouni(cvmv.CutVideoName(tempData.cMenName))) < 0) j--;
						break;
					case 3:
						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cRunTime)), ansiTouni(cvmv.CutVideoName(tempData.cRunTime))) < 0) i++;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cRunTime)), ansiTouni(cvmv.CutVideoName(tempData.cRunTime))) > 0) i++;

						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cRunTime)), ansiTouni(cvmv.CutVideoName(tempData.cRunTime))) > 0) j--;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cRunTime)), ansiTouni(cvmv.CutVideoName(tempData.cRunTime))) < 0) j--;
						break;
					case 4:
						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cVodNum)), ansiTouni(cvmv.CutVideoName(tempData.cVodNum))) < 0) i++;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cVodNum)), ansiTouni(cvmv.CutVideoName(tempData.cVodNum))) > 0) i++;

						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cVodNum)), ansiTouni(cvmv.CutVideoName(tempData.cVodNum))) > 0) j--;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cVodNum)), ansiTouni(cvmv.CutVideoName(tempData.cVodNum))) < 0) j--;
						break;
					case 5:
						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cEtc)), ansiTouni(cvmv.CutVideoName(tempData.cEtc))) < 0) i++;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cEtc)), ansiTouni(cvmv.CutVideoName(tempData.cEtc))) > 0) i++;

						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cEtc)), ansiTouni(cvmv.CutVideoName(tempData.cEtc))) > 0) j--;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cEtc)), ansiTouni(cvmv.CutVideoName(tempData.cEtc))) < 0) j--;
						break;
				}
				break;
			case 4:
				switch(fieldIndex)
				{
					case 0:
						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cMenName)), ansiTouni(cvmv.CutVideoName(tempData.cMenName))) < 0) i++;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cMenName)), ansiTouni(cvmv.CutVideoName(tempData.cMenName))) > 0) i++;

						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cMenName)), ansiTouni(cvmv.CutVideoName(tempData.cMenName))) > 0) j--;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cMenName)), ansiTouni(cvmv.CutVideoName(tempData.cMenName))) < 0) j--;
						break;

					case 1:
						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cVodTitle)), ansiTouni(cvmv.CutVideoName(tempData.cVodTitle))) < 0) i++;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cVodTitle)), ansiTouni(cvmv.CutVideoName(tempData.cVodTitle))) > 0) i++;

						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cVodTitle)), ansiTouni(cvmv.CutVideoName(tempData.cVodTitle))) > 0) j--;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cVodTitle)), ansiTouni(cvmv.CutVideoName(tempData.cVodTitle))) < 0) j--;
						break;
					case 2:
						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cDate)), ansiTouni(cvmv.CutVideoName(tempData.cDate))) < 0) i++;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cDate)), ansiTouni(cvmv.CutVideoName(tempData.cDate))) > 0) i++;

						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cDate)), ansiTouni(cvmv.CutVideoName(tempData.cDate))) > 0) j--;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cDate)), ansiTouni(cvmv.CutVideoName(tempData.cDate))) < 0) j--;
						break;						
					case 3:
						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cRunTime)), ansiTouni(cvmv.CutVideoName(tempData.cRunTime))) < 0) i++;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cRunTime)), ansiTouni(cvmv.CutVideoName(tempData.cRunTime))) > 0) i++;

						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cRunTime)), ansiTouni(cvmv.CutVideoName(tempData.cRunTime))) > 0) j--;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cRunTime)), ansiTouni(cvmv.CutVideoName(tempData.cRunTime))) < 0) j--;
						break;
					case 4:
						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cVodNum)), ansiTouni(cvmv.CutVideoName(tempData.cVodNum))) < 0) i++;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cVodNum)), ansiTouni(cvmv.CutVideoName(tempData.cVodNum))) > 0) i++;

						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cVodNum)), ansiTouni(cvmv.CutVideoName(tempData.cVodNum))) > 0) j--;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cVodNum)), ansiTouni(cvmv.CutVideoName(tempData.cVodNum))) < 0) j--;
						break;
					case 5:
						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cEtc)), ansiTouni(cvmv.CutVideoName(tempData.cEtc))) < 0) i++;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cEtc)), ansiTouni(cvmv.CutVideoName(tempData.cEtc))) > 0) i++;

						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cEtc)), ansiTouni(cvmv.CutVideoName(tempData.cEtc))) > 0) j--;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cEtc)), ansiTouni(cvmv.CutVideoName(tempData.cEtc))) < 0) j--;
						break;
					case 6:
						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cFileName)), ansiTouni(cvmv.CutVideoName(tempData.cFileName))) < 0) i++;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cFileName)), ansiTouni(cvmv.CutVideoName(tempData.cFileName))) > 0) i++;

						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cFileName)), ansiTouni(cvmv.CutVideoName(tempData.cFileName))) > 0) j--;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cFileName)), ansiTouni(cvmv.CutVideoName(tempData.cFileName))) < 0) j--;
						break;

				}
				break;
			case 5:
				switch(fieldIndex)
				{
					case 0:
						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cVodName)), ansiTouni(cvmv.CutVideoName(tempData.cVodName))) < 0) i++;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cVodName)), ansiTouni(cvmv.CutVideoName(tempData.cVodName))) > 0) i++;

						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cVodName)), ansiTouni(cvmv.CutVideoName(tempData.cVodName))) > 0) j--;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cVodName)), ansiTouni(cvmv.CutVideoName(tempData.cVodName))) < 0) j--;
						break;

					case 1:
						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cVodTitle)), ansiTouni(cvmv.CutVideoName(tempData.cVodTitle))) < 0) i++;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cVodTitle)), ansiTouni(cvmv.CutVideoName(tempData.cVodTitle))) > 0) i++;

						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cVodTitle)), ansiTouni(cvmv.CutVideoName(tempData.cVodTitle))) > 0) j--;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cVodTitle)), ansiTouni(cvmv.CutVideoName(tempData.cVodTitle))) < 0) j--;
						break;
					case 2:
						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cMenName)), ansiTouni(cvmv.CutVideoName(tempData.cMenName))) < 0) i++;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cMenName)), ansiTouni(cvmv.CutVideoName(tempData.cMenName))) > 0) i++;

						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cMenName)), ansiTouni(cvmv.CutVideoName(tempData.cMenName))) > 0) j--;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cMenName)), ansiTouni(cvmv.CutVideoName(tempData.cMenName))) < 0) j--;
						break;						
					case 3:
						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cDate)), ansiTouni(cvmv.CutVideoName(tempData.cDate))) < 0) i++;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cDate)), ansiTouni(cvmv.CutVideoName(tempData.cDate))) > 0) i++;

						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cDate)), ansiTouni(cvmv.CutVideoName(tempData.cDate))) > 0) j--;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cDate)), ansiTouni(cvmv.CutVideoName(tempData.cDate))) < 0) j--;
						break;						
					case 4:
						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cVodNum)), ansiTouni(cvmv.CutVideoName(tempData.cVodNum))) < 0) i++;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cVodNum)), ansiTouni(cvmv.CutVideoName(tempData.cVodNum))) > 0) i++;

						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cVodNum)), ansiTouni(cvmv.CutVideoName(tempData.cVodNum))) > 0) j--;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cVodNum)), ansiTouni(cvmv.CutVideoName(tempData.cVodNum))) < 0) j--;
						break;
					case 5:
						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cEtc)), ansiTouni(cvmv.CutVideoName(tempData.cEtc))) < 0) i++;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cEtc)), ansiTouni(cvmv.CutVideoName(tempData.cEtc))) > 0) i++;

						if(direction) 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cEtc)), ansiTouni(cvmv.CutVideoName(tempData.cEtc))) > 0) j--;
						else 
							while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cEtc)), ansiTouni(cvmv.CutVideoName(tempData.cEtc))) < 0) j--;
						break;
				}
				break;
			}//switch

			if(i <= j)
			{
				cvmv.SwapData(pIData, i, j);
				i++;
				j--;
			}

		} while(i <= j);

		QuickSortData(pIData, type, fieldIndex, direction, left, j, nFlag);
		QuickSortData(pIData, type, fieldIndex, direction, i, right, nFlag);
	}
}



void CMyListCtrl::QuickSortFtpData(SFtpFileData *pIData, int type, int fieldIndex, int direction,int left, int right)
{
	int						i, j, middle;
	SFtpFileData				tempData;
	CMyListCtrl				cvmv;

	if(right - left > 0)
	{
		middle = (left + right) / 2;
		tempData = pIData[middle]; 
		i = left; 
		j = right;
		
		do
		{
			switch(fieldIndex)
			{
				case 0:
					if(direction) 
						while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cVodTitle)), ansiTouni(cvmv.CutVideoName(tempData.cVodTitle))) < 0) i++;
					else 
						while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cVodTitle)), ansiTouni(cvmv.CutVideoName(tempData.cVodTitle))) > 0) i++;

					if(direction) 
						while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cVodTitle)), ansiTouni(cvmv.CutVideoName(tempData.cVodTitle))) > 0) j--;
					else 
						while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cVodTitle)), ansiTouni(cvmv.CutVideoName(tempData.cVodTitle))) < 0) j--;
					break;
				case 1:
					if(direction) 
						while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cTeacher)), ansiTouni(cvmv.CutVideoName(tempData.cTeacher))) < 0) i++;
					else 
						while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cTeacher)), ansiTouni(cvmv.CutVideoName(tempData.cTeacher))) > 0) i++;

					if(direction) 
						while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cTeacher)), ansiTouni(cvmv.CutVideoName(tempData.cTeacher))) > 0) j--;
					else 
						while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cTeacher)), ansiTouni(cvmv.CutVideoName(tempData.cTeacher))) < 0) j--;
					break;
				case 2:
					if(direction) 
						while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cRunTime)), ansiTouni(cvmv.CutVideoName(tempData.cRunTime))) < 0) i++;
					else 
						while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cRunTime)), ansiTouni(cvmv.CutVideoName(tempData.cRunTime))) > 0) i++;

					if(direction) 
						while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cRunTime)), ansiTouni(cvmv.CutVideoName(tempData.cRunTime))) > 0) j--;
					else 
						while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cRunTime)), ansiTouni(cvmv.CutVideoName(tempData.cRunTime))) < 0) j--;
					break;
				case 3:
					if(direction) 
						while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cfileDate)), ansiTouni(cvmv.CutVideoName(tempData.cfileDate))) < 0) i++;
					else 
						while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[i].cfileDate)), ansiTouni(cvmv.CutVideoName(tempData.cfileDate))) > 0) i++;

					if(direction) 
						while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cfileDate)), ansiTouni(cvmv.CutVideoName(tempData.cfileDate))) > 0) j--;
					else 
						while(cvmv.CompareVideoName(ansiTouni(cvmv.CutVideoName(pIData[j].cfileDate)), ansiTouni(cvmv.CutVideoName(tempData.cfileDate))) < 0) j--;
					break;
				case 4:
					if(direction) 
						while(atoi(cvmv.CutVideoName(pIData[i].cfileSize)) < atoi(cvmv.CutVideoName(tempData.cfileSize))) i++;
					else 
						while(atoi(cvmv.CutVideoName(pIData[i].cfileSize)) > atoi(cvmv.CutVideoName(tempData.cfileSize))) i++;

					if(direction) 
						while(atoi(cvmv.CutVideoName(pIData[j].cfileSize)) > atoi(cvmv.CutVideoName(tempData.cfileSize))) j--;
					else 
						while(atoi(cvmv.CutVideoName(pIData[j].cfileSize)) < atoi(cvmv.CutVideoName(tempData.cfileSize))) j--;
					break;

			}//switch(fieldIndex)

			if(i <= j)
			{
				cvmv.SwapFtpData(pIData, i, j);
				i++;
				j--;
			}

		} while(i <= j);

		QuickSortFtpData(pIData, type, fieldIndex, direction, left, j);
		QuickSortFtpData(pIData, type, fieldIndex, direction, i, right);
	}
}


void CMyListCtrl::DrawSortHeader(int oldIndex, int newIndex, int direction)
{
	CHeaderCtrl				*pHeaderCtrl = GetHeaderCtrl();

	if(pHeaderCtrl != NULL)
	{
		HDITEM					hdi;
		int						nlen = 0;
	
		CHAR					lpBuffer[256];
		TCHAR					lpBuf[256];


		memset((char *)lpBuffer, 0x00, strlen(lpBuffer));
		memset((char *)lpBuf, 0x00, sizeof(TCHAR) * 256);

		hdi.mask = HDI_TEXT;
		hdi.pszText = lpBuf;
		hdi.cchTextMax = 256;

		pHeaderCtrl->GetItem(oldIndex, &hdi);

		strcpy(lpBuffer, uniToansi(lpBuf));
		lpBuffer[strlen(lpBuffer) - 3] = '\0';


		hdi.mask = HDI_TEXT;
		hdi.pszText = ansiTouni(lpBuffer);
		hdi.cchTextMax = 256;

		pHeaderCtrl->SetItem(oldIndex, &hdi);

		memset((char *)lpBuf, 0x00, sizeof(TCHAR) * 256);

		hdi.mask = HDI_TEXT;
		hdi.pszText = lpBuf;
		hdi.cchTextMax = 256;

		pHeaderCtrl->GetItem(newIndex, &hdi);


		strcpy(lpBuffer, uniToansi(lpBuf));

		if(direction == 1) 
			strcat(lpBuffer, " ▼");
		else 
			strcat(lpBuffer, " ▲");

		hdi.pszText = ansiTouni(lpBuffer);

		pHeaderCtrl->SetItem(newIndex, &hdi);
	
	}
}

void CMyListCtrl::GetList_col_Data_i(CString *pData, int colnum, int colcount)
{
	CString					ff;

	for(int i = 0; i < colcount; i++)
	{
		ff = GetItemText(colnum, i);

		pData[i] = ff;
	}
}

void CMyListCtrl::SetList_col_Data_i(CString *pData, int colnum, int colcount)
{
	CString					ff;

	for(int i = 0; i < colcount; i++)
	{
		ff = pData[i];
		SetItemText(colnum, i, ff);		
	}
}

//열의 숫자를 가져옴
int CMyListCtrl::GetRowCount_i()
{
    int						count = 0;

    LVCOLUMN				col = {0,};

	col.mask = LVCF_WIDTH;

    while(1)
    {
        if(!GetColumn(count, &col)) 
			break;

		count++;
    }

    return count;
}

//행의 전체 숫자를 가져옴
int CMyListCtrl::GetDataTotalCount_i()
{
	POSITION				pos = GetFirstSelectedItemPosition();
	int						numm = 0;
	
	if(pos == NULL)
		return numm;
	else
	{
		while(pos)
		{
			int						nItem = GetNextSelectedItem(pos);

			numm++;
		}
	}

	return numm;
}

void CMyListCtrl::SetType(int type, int highlight)
{
	m_nType = type;
	m_nHighlight = highlight;
}

//
void CMyListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    CDC						*pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CPen					pen;		//선그리기를 위한 펜.

    int						oldcolor = 0;

	int						widthx = 0;
	int						heighty = 0;
	RECT					arect;

	pDC->SetBkMode(TRANSPARENT);

	//선그리기.
	pen.CreatePen(PS_SOLID, 1, RGB(192, 192, 192));
	pDC->SelectObject(&pen);

	arect = lpDrawItemStruct->rcItem;

    if(lpDrawItemStruct->itemState & ODS_SELECTED)
	{//선택된 Row
		//
        FillRect(lpDrawItemStruct->hDC, &lpDrawItemStruct->rcItem, CBrush(RGB(78, 112, 27)));  //선택된 Row
		
		pDC->SetTextColor(RGB(255, 255, 255));

		widthx = 0;
		heighty = 0;
		
		for(int i = 0; i <= GetRowCount_i(); i++)
		{
			arect.left += widthx;
			widthx += GetColumnWidth(i);
			pDC->MoveTo(widthx, arect.bottom);
			pDC->LineTo(widthx, arect.top);
		}// for
	}
	else 
	{
		widthx = 0;
		heighty = 0;
		
		pDC->SetTextColor(RGB(0, 0, 0));

		//
		//dc.Rectangle(arect.left, arect.bottom , arect.right, (arect.bottom+5));

		if((lpDrawItemStruct->itemID % 2) == 0)
			FillRect(lpDrawItemStruct->hDC, &arect, CBrush(RGB(227, 254, 173)));		//짝수 row
		else
			FillRect(lpDrawItemStruct->hDC, &arect, CBrush(RGB(224, 203, 192)));		//홀수 row

		//		
		for(int i = 0; i <= GetRowCount_i(); i++)
		{
			arect.left += widthx;
			widthx += GetColumnWidth(i);
			pDC->MoveTo((widthx), arect.bottom);
			pDC->LineTo(widthx, arect.top);			
		}// for	
	}

	pDC->MoveTo(lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top);
	pDC->LineTo(lpDrawItemStruct->rcItem.right, (lpDrawItemStruct->rcItem.top));

	pDC->MoveTo(lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.bottom);
	pDC->LineTo(lpDrawItemStruct->rcItem.right, (lpDrawItemStruct->rcItem.bottom));	

	int						sx = 0, ex = 0, width = 0;

	CRect					rect = lpDrawItemStruct->rcItem;
	CString					strBuf, strItem;
	CSize					sizeStr;
	int						flag = 1;
	int						cnt = 0;
	int						len = 0;

	rect.right = rect.left;

	for(int i = 0; i <= GetRowCount_i(); i++)
	{
		width = GetColumnWidth(i);

        if(width)
		{
            rect.left = rect.right + 3;
            rect.right += width;
            oldcolor = pDC->GetTextColor();

            pDC->SetTextColor(oldcolor);

			strItem = GetItemText(lpDrawItemStruct->itemID, i);
			// 칸을 벗어나는 글씨를 (...처리하는 부분.)
			//AfxMessageBox(strItem);
			cnt = 0;
			len = 0;
			flag = 1;
			strBuf.Empty();

			while(flag)
			{
				if(cnt < 1)		
				{
					sizeStr = pDC->GetTextExtent(strItem);
					len = strItem.GetLength();
				}
				else
				{
					sizeStr = pDC->GetTextExtent(strBuf);
					len = strBuf.GetLength();
				}

				if(rect.Width() < (sizeStr.cx + 10) )		//글자의 너비 + 10
				{
					strBuf.Format(_T("%s"), strItem.Left(len - 1) );
				}
				else
				{
					strBuf.Format(_T("%s..."), strItem.Left(len - 1) );
					flag = 0;
				}
				
				cnt++;
			}

			if(1 < cnt)
				pDC->DrawText(" " + strBuf, &rect, DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER);
			else
				pDC->DrawText(" " + strItem, &rect, DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER);
			//...처리...
        }
    }

    pDC->DeleteTempMap();

	//CListCtrl::DrawItem(lpDrawItemStruct);
}


BOOL CMyListCtrl::OnEraseBkgnd(CDC *pDC)
{
	if(pDC)
	{
		CRect					rect;

		GetClientRect(rect);

		CBrush					brush0(RGB(224, 203, 192));

		SCROLLINFO				si;

		ZeroMemory(&si, sizeof(SCROLLINFO));

		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_POS;

		GetScrollInfo(SB_HORZ, &si);
		rect.left -= si.nPos;

		pDC->FillRect(&rect, &brush0);

		brush0.DeleteObject();
	}

	return FALSE;


//	return CListCtrl::OnEraseBkgnd(pDC);
}


void CMyListCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	CRect					rect;

	GetClientRect(rect);

	lpMeasureItemStruct->itemWidth = rect.Width();
	lpMeasureItemStruct->itemHeight = 35;	// 데이타 높이.

/*	TEXTMETRIC				tm;
	CClientDC				dc(this); 
	CFont					*pFont = GetFont();
	CFont					*pOldFont = dc.SelectObject(pFont); 

	dc.GetTextMetrics(&tm);

	int						nItemHeight = tm.tmHeight + tm.tmExternalLeading;

	lpMeasureItemStruct->itemHeight = nItemHeight + 4; 
	dc.SelectObject(pOldFont);
*/
}

void CMyListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	CListCtrl::OnMouseMove(nFlags, point);
}

// GetColumns
int CMyListCtrl::GetColumns()
{
	return GetHeaderCtrl()->GetItemCount();
}

void CMyListCtrl::ItemScroll(int num)
{
	//CPoint					ptItem;

	//GetItemPosition(num, &ptItem);
	//Scroll(ptItem);
	//SetScrollPos(SB_VERT, num);
	EnsureVisible(num, false);
}

void CMyListCtrl::SetItemCurrentMark(int num)
{
	m_CurrentMark = num;
}

char *CMyListCtrl::uniToansi(const wchar_t* pwstrSrc)
{
    if(NULL == pwstrSrc)
        return NULL;    

/*    int						len = 0;

    len = (wcslen(pwstrSrc) + 1) * 2;

    char					*pstr = (char *)malloc(sizeof(char) * len);

    WideCharToMultiByte(CP_ACP, 0, pwstrSrc, -1, pstr, len, NULL, NULL);
*/

    int						ires = WideCharToMultiByte(CP_ACP, 0, pwstrSrc, -1, NULL, 0, NULL, NULL);

    if(0 == ires)
        return NULL;

    char					*pstr = new char[ires + 1];

	memset((char *)pstr, 0x00, strlen(pstr));

	ires = WideCharToMultiByte(CP_ACP, 0, pwstrSrc, -1, pstr, ires, NULL, NULL);

    if(0 == ires)
	{
        delete []pstr;
        pstr = NULL;

        return NULL;
    }

    return pstr;
}

wchar_t *CMyListCtrl::ansiTouni(const char *szANSI)
{
    wchar_t					*szwUNI = NULL;
    ULONG					cChar;

    if(NULL == szANSI)
        return NULL;    

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
