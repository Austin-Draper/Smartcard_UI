
// Smartcard_UI.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
//include all from smartcard_data.cpp
#include <stdio.h>
#include <winscard.h>
#include <tchar.h>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <bitset>
#include <array>
#include <ctime>
#include "UFScanner.h"
#include "UFMatcher.h"
#include "UFDatabase.h"
#pragma comment(lib, "winscard.lib")

using namespace std;


// CSmartcard_UIApp:
// See Smartcard_UI.cpp for the implementation of this class
//

class CSmartcard_UIApp : public CWinApp
{
public:
	CSmartcard_UIApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CSmartcard_UIApp theApp;