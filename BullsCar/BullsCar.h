// BullsCar.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CBullsCarApp:
// �� Ŭ������ ������ ���ؼ��� BullsCar.cpp�� �����Ͻʽÿ�.
//

class CBullsCarApp : public CWinApp
{
public:
	CBullsCarApp();

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CBullsCarApp theApp;