
// Serial151.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CSerial151App:
// �� Ŭ������ ������ ���ؼ��� Serial151.cpp�� �����Ͻʽÿ�.
//

class CSerial151App : public CWinAppEx
{
public:
	CSerial151App();

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CSerial151App theApp;