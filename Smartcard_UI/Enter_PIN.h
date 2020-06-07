#pragma once
#include "Smartcard_UIDlg.h"


//include the below additional libraries
#include <iostream>
#include <string>
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
#include <stdio.h>
#include <stdlib.h>
#pragma comment(lib,"odbc32.lib")

// Enter_PIN dialog

class Enter_PIN : public CDialogEx
{
	DECLARE_DYNAMIC(Enter_PIN)

public:
	Enter_PIN(CWnd* pParent = NULL);   // standard constructor
	virtual ~Enter_PIN();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL Enter_PIN::OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	int _convertPIN(string _temp);
	SCARDHANDLE hCardHandle;
	LPCSCARD_IO_REQUEST pioSendPci;
	SCARDCONTEXT hContext;



	void binaryConvert(vector<int> &hex_storage, vector<string> &fiveBinary);
	void BDC_Convert(vector<int> &hex_storage, vector<string> &fiveBinary, int BDC_Length);
	void BDC_ConvertFour(vector<int> &hex_storage, vector<string> &fiveBinary, int BDC_Length);
	void parseFASCN(vector<string>fiveBinary, string &_agencyCode, string &_systemCode, string &_credentialNumber, string &_CS, string &_ICI, string &_PI, string &_OC, string &_OI, string &_POA, string &_LRC, CSmartcard_UIDlg &main_dlg);
	string parseOI(vector<string>fiveBinary);
	string parseOIFour(vector<string>fiveBinary);
	string parseGUID(vector<string>fiveBinary);
	string parsePrinted(vector<int> &hex_storage, int);
	string switchReturn(string binaryInt);
	string switchReturnFour(string binaryInt);
	string switchReturnDecimal(string binaryInt);
	void parseExpiration(string&, string&, string&);
	void DisplayResourcePINMessageBox(int);


	wchar_t * GetWCharFromString(std::string inString);
	

	afx_msg void OnBnClickedCancel();
};
