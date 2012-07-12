#pragma once

// ��ǻ�Ϳ��� Microsoft Visual C++�� ����Ͽ� ������ IDispatch ���� Ŭ�����Դϴ�.

// ����: �� ������ ������ �������� ���ʽÿ�. Microsoft Visual C++����
//  �� Ŭ������ �ٽ� ������ �� ������ ������ ����ϴ�.

/////////////////////////////////////////////////////////////////////////////
// CThumbctl1 ���� Ŭ�����Դϴ�.

class CThumbctl1 : public CWnd
{
protected:
	DECLARE_DYNCREATE(CThumbctl1)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x71650000, 0xE8A8, 0x11D2, { 0x96, 0x52, 0x0, 0xC0, 0x4F, 0xC3, 0x8, 0x71 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID, 
						CCreateContext* pContext = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); 
	}

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
				UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
				BSTR bstrLicKey = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); 
	}

// Ư���Դϴ�.
public:


// �۾��Դϴ�.
public:

// IThumbCtl

// Functions
//

	BOOL displayFile(LPCTSTR bsFileName)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, bsFileName);
		return result;
	}
	BOOL haveThumbnail()
	{
		BOOL result;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	CString get_freeSpace()
	{
		CString result;
		InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_usedSpace()
	{
		CString result;
		InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_totalSpace()
	{
		CString result;
		InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}

// Properties
//



};
