
// Inject_x64.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CInject_x64App:
// See Inject_x64.cpp for the implementation of this class
//

class CInject_x64App : public CWinApp
{
public:
	CInject_x64App();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CInject_x64App theApp;