
// Win32Test.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CWin32TestApp:
// See Win32Test.cpp for the implementation of this class
//

class CWin32TestApp : public CWinApp
{
public:
	CWin32TestApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CWin32TestApp theApp;