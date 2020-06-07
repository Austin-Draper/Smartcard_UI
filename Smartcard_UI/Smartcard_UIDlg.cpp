
// Smartcard_UIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Smartcard_UI.h"
#include "Smartcard_UIDlg.h"
#include "Enter_Card.h"
#include "Enter_PIN.h"
#include "dbinfo_file.h"
#include "afxdialogex.h"
#include <iostream>
#include <fstream>
#include <cstdint>
#include <experimental\filesystem>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSmartcard_UIDlg dialog



CSmartcard_UIDlg::CSmartcard_UIDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SMARTCARD_UI_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSmartcard_UIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSmartcard_UIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CSmartcard_UIDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDOK2, &CSmartcard_UIDlg::OnBnClickedOk2)
	ON_BN_CLICKED(IDCANCEL, &CSmartcard_UIDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CSmartcard_UIDlg message handlers

BOOL CSmartcard_UIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	SetDlgItemText(IDC_EDIT5, PIN);
	SetDlgItemText(IDC_EDIT4, FASCN);
	SetDlgItemText(IDC_EDIT16, _agencyCode);
	SetDlgItemText(IDC_EDIT12, _systemCode);
	SetDlgItemText(IDC_EDIT17, _credentialNumber);
	SetDlgItemText(IDC_EDIT15, _CS);
	SetDlgItemText(IDC_EDIT10, _ICI);
	SetDlgItemText(IDC_EDIT9, _PI);
	SetDlgItemText(IDC_EDIT11, _OC);
	SetDlgItemText(IDC_EDIT13, _OI);
	SetDlgItemText(IDC_EDIT14, _POA);
	SetDlgItemText(IDC_EDIT18, _GUID);
	SetDlgItemText(txt_FirstName, f_name);
	SetDlgItemText(txt_LastName, l_name);
	SetDlgItemText(txt_MiddleInitial, m_initial);
	SetDlgItemText(IDC_EDIT7, expirationDate);
	SetDlgItemText(IDC_EDIT6, currentDate);
	SetDlgItemText(IDC_EDIT8, cardType);





	ifstream infile;
	infile.open("dbinfo.txt");
	string _server, _port;
	infile >> _server;
	infile >> _port;
	infile.close();
	if (_server.size() == 0)
	{
		dbinfo_file _info;
		_info.DoModal();
	}

	/*if (std::experimental::filesystem::exists("dbinfo.txt"))
	{
		std::cout << "dbinfo exists" << endl;
		//it exists, run the program as normal
	}
	else
	{
		std::cout << "need to create dbinfo" << endl;
		//ofstream myfile;
		//myfile.open("dbinfo.txt");
		
		//ask user for database name and port number
		//make a dialog that gets that information then goes back to homepage
		//rest of program now runs as normal
		dbinfo_file _info;
		_info.DoModal();
	}*/







	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSmartcard_UIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSmartcard_UIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSmartcard_UIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSmartcard_UIDlg::OnBnClickedOk()//Save Button
{
	// TODO: Add your control notification handler code here
	SQLWork();//upload everything to database
	//popup that says database import was successful
	//go back to homepage with no data in any box (maybe just call a DoModal() without any initializations?... or re-initialize everything to null and then call the modal
	std::cout << "success!" << endl;
	return;
	//CSmartcard_UIDlg dlg;
	//int nRet = 5;
	//CSmartcard_UIDlg::EndDialog(nRet);//in future... hide dialog instead, make it re-appear if either of next 2 dialogs cancelled... THEN I can delete and remake if user goes through both other dialogs successfully
	//dlg.DoModal();

}


void CSmartcard_UIDlg::OnBnClickedOk2()//New Card Button
{
	// TODO: Add your control notification handler code here
	Enter_Card dlg;
	int nRet = 5;
	CSmartcard_UIDlg::EndDialog(nRet);//in future... hide dialog instead, make it re-appear if either of next 2 dialogs cancelled... THEN I can delete and remake if user goes through both other dialogs successfully
	dlg.DoModal();
}


void CSmartcard_UIDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//CDialogEx::OnCancel();
	exit(0);
}

string CSmartcard_UIDlg::_CStringtoString(CString txtname)
{
	std::string strStd;//convert CString to std::string
	for (int i = 0; i < txtname.GetLength(); ++i)
	{
		if (txtname[i] <= 0x7f)
			strStd.append(1, static_cast<char>(txtname[i]));
		else
			strStd.append(1, '?');
	}//end of convert CString to std::string
	return strStd;
}

void CSmartcard_UIDlg::SQLWork()
{
#define SQL_RESULT_LEN 240
#define SQL_RETURN_CODE_LEN 1000

	//this works for test database and table...
	/*int hello = 27;
	char* testinq = "hey";
	string _firstone = "INSERT INTO testtable (name, testint) VALUES ('";
	string _secondone = testinq;
	string _secondhalf = "',";
	string _thirdone = to_string(hello);
	string _fourthone = ")";
	string total_res = _firstone + _secondone + _secondhalf + _thirdone + _fourthone;
	auto execQuery = GetWCharFromString(total_res);*/

	//upload all data to database
	//extras I might need to add: CardNumber, CompanyID, RecordRequest = 72?, RecordStatus, ImportNow = 1?,
	CSmartcard_UIDlg main_dlg;
	Enter_PIN _strconvert;
	//string _lastname = "";
	string _lastname = main_dlg._CStringtoString(l_name);
	string _firstname = main_dlg._CStringtoString(f_name);
	string ICI = main_dlg._CStringtoString(_ICI);
	string _crednum = main_dlg._CStringtoString(_credentialNumber);
	string _systemcode = main_dlg._CStringtoString(_systemCode);
	string _agencycode = main_dlg._CStringtoString(_agencyCode);
	string _PIN = main_dlg._CStringtoString(PIN);
	string _activedate = main_dlg._CStringtoString(currentDate);
	string _expirationdate = main_dlg._CStringtoString(expirationDate);
	string _cardformat = main_dlg._CStringtoString(cardType);
	if (_cardformat == "CMV_U")
	{
		_cardformat = "4";
	}
	else if (_cardformat == "CMV_F")
	{
		_cardformat = "5";
	}

	string _personalidentifier = main_dlg._CStringtoString(_PI);
	string OC = main_dlg._CStringtoString(_OC);
	string OI = main_dlg._CStringtoString(_OI);
	string _OA = main_dlg._CStringtoString(_POA);
	string __CS = main_dlg._CStringtoString(_CS);
	string _CardIdentifier = main_dlg._CStringtoString(_GUID);//only for CMV-U cards (the PIV-I)
	string _midinitial = main_dlg._CStringtoString(m_initial);
	string _RecordStatus = "0";
	string _ImportNow = "1";

	if (_firstname == "" || _lastname == "")//I think it's ok for _midinitial to be empty
	{
		_firstname = "Dummy";
		_lastname = "Name";
	}//might change this to prompting the user for first and last name
	 //in this case we would also ask for middle name
	 //any name missing is blank. any name found would be shown but changeable


	string total_res;
	if (_cardformat == "4")
	{//replaced CredentialNumber with CardNumber                                                                                                    RecordStatus = 0       ImportNow = 1
		string _firstone = "INSERT INTO DataImportTable (LastName, FirstName, CredentialIssueLevel, SystemCode, AgencyCode, ActiveDate, ExpiryDate, RecordStatus, InitLet, ImportNow, CardFormat, PersonIdentifier, OrganisationCategory, OrganisationIdentifier, OrganisationAssociation, CredentialSeriesCode, CardIdentifier, CardNumber) VALUES ('";
		string _secondone = _lastname;
		string _secondhalf = "',";
		string _thirdone = _crednum;
		string _fourthone = ")";
		total_res = _firstone + _secondone + _secondhalf + "'" + _firstname + "'," + ICI + "," + _systemcode + "," + _agencycode + ",'" + _activedate + "','"
			+ _expirationdate + "'," + _RecordStatus + ",'" + _midinitial + "'," + _ImportNow + "," + _cardformat + "," + _personalidentifier + "," + OC + "," + OI + "," + _OA + "," + __CS + ",'" + _CardIdentifier + "'," + _thirdone + _fourthone;
		//total_res = _firstone + _secondone + _secondhalf + "'" + _firstname + "'," + ICI + "," + _systemcode + "," + _agencycode + ",'" + _activedate + "','"
			//+ _expirationdate + "','" + _midinitial + "'," + _cardformat + "," + _personalidentifier + "," + OC + "," + OI + "," + _OA + "," + __CS + ",'" + _CardIdentifier + "'," + _thirdone + _fourthone;

	}
	else
	{//replaced CredentialNumber with CardNumber
		string _firstone = "INSERT INTO DataImportTable (LastName, FirstName, CredentialIssueLevel, SystemCode, AgencyCode, ActiveDate, ExpiryDate, RecordStatus, InitLet, ImportNow, CardFormat, PersonIdentifier, OrganisationCategory, OrganisationIdentifier, OrganisationAssociation, CredentialSeriesCode, CardNumber) VALUES ('";
		string _secondone = _lastname;
		string _secondhalf = "',";
		string _thirdone = _crednum;
		string _fourthone = ")";
		total_res = _firstone + _secondone + _secondhalf + "'" + _firstname + "'," + ICI + "," + _systemcode + "," + _agencycode + ",'" + _activedate + "','"
			+ _expirationdate + "'," + _RecordStatus + ",'" + _midinitial + "'," + _ImportNow + "," + _cardformat + "," + _personalidentifier + "," + OC + "," + OI + "," + _OA + "," + __CS + "," + _thirdone + _fourthone;
		//total_res = _firstone + _secondone + _secondhalf + "'" + _firstname + "'," + ICI + "," + _systemcode + "," + _agencycode + ",'" + _PIN + "','" + _activedate + "','"
			//+ _expirationdate + "','" + _midinitial + "'," + _cardformat + "," + _personalidentifier + "," + OC + "," + OI + "," + _OA + "," + __CS + "," + _thirdone + _fourthone;
	}
	
	auto execQuery = _strconvert.GetWCharFromString(total_res);



	//define handles and variables
	SQLHANDLE sqlConnHandle;
	SQLHANDLE sqlStmtHandle;
	SQLHANDLE sqlEnvHandle;
	SQLWCHAR retconstring[SQL_RETURN_CODE_LEN];

	//initializations
	sqlConnHandle = NULL;
	sqlStmtHandle = NULL;


	//OPEN dbinfo.txt AND PUT DATA INTO server AND port
	ifstream infile;
	infile.open("dbinfo.txt");
	string server, port;
	infile >> server;
	infile >> port;
	infile.close();


	string serverconn1 = "DRIVER={SQL Server};SERVER=";
	string serverconn2 = ", ";
	string serverconn3 = ";DATABASE=multiMaxImport;Trusted=true;";
	//string serverconn3 = ";DATABASE=testing;Trusted=true;";
	string SQLconnstr = serverconn1 + server + serverconn2 + port + serverconn3;
	auto connstr = _strconvert.GetWCharFromString(SQLconnstr);
	//SQLWCHAR* _query = (SQLWCHAR *)L"DRIVER={SQL Server};SERVER=localhost, 1433;DATABASE=multimax;Trusted=true;";

	//allocations
	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlEnvHandle))
		goto COMPLETED;

	if (SQL_SUCCESS != SQLSetEnvAttr(sqlEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
		goto COMPLETED;

	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, sqlEnvHandle, &sqlConnHandle))
		goto COMPLETED;

	//output
	cout << "Attempting connection to SQL Server...";
	cout << "\n";


	//You have the option to use a username/password instead of a trusted connection
	//but is more secure to use a trusted connection
	switch (SQLDriverConnect(sqlConnHandle,
		NULL,
		(SQLWCHAR*)connstr,
		//(SQLWCHAR*)L"DRIVER={SQL Server};SERVER=localhost, 1433;DATABASE=testing;UID=myID;PWD=myPW;",
		SQL_NTS,
		retconstring,
		1024,
		NULL,
		SQL_DRIVER_NOPROMPT)) {

	case SQL_SUCCESS:
		cout << "Successfully connected to SQL Server";
		cout << "\n";
		break;

	case SQL_SUCCESS_WITH_INFO:
		cout << "Successfully connected to SQL Server";
		cout << "\n";
		break;

	case SQL_INVALID_HANDLE:
		cout << "Could not connect to SQL Server";
		cout << "\n";
		DisplayDatabaseFailMessageBox();
		goto COMPLETED;

	case SQL_ERROR:
		cout << "Could not connect to SQL Server";
		cout << "\n";
		DisplayDatabaseFailMessageBox();
		goto COMPLETED;

	default:
		break;
	}

	//if there is a problem connecting then exit application
	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
		goto COMPLETED;

	//output
	cout << "\n";
	cout << "Executing T-SQL query...";
	cout << "\n";

	//if there is a problem executing the query then exit application
	//else display query result
	if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)execQuery, SQL_NTS)) {
		DisplayFailMessageBox();
		goto COMPLETED;
	}

	else {
		DisplaySuccessMessageBox();
		goto COMPLETED;
		//declare output variable and pointer
		SQLCHAR sqlVersion[SQL_RESULT_LEN];
		SQLINTEGER ptrSqlVersion;

		while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {

			SQLGetData(sqlStmtHandle, 1, SQL_CHAR, sqlVersion, SQL_RESULT_LEN, &ptrSqlVersion);

			//display query result
			cout << "\nQuery Result:\n\n";
			cout << sqlVersion << endl;
		}
	}





	//close connection and free resources
COMPLETED:
	SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
	SQLDisconnect(sqlConnHandle);
	SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
	SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvHandle);

	//pause the console window - exit when key is pressed
	cout << "\nPress any key to exit...";
	getchar();



	return;
}

void CSmartcard_UIDlg::DisplaySuccessMessageBox()
{
	int msgboxID = MessageBox(
		(LPCWSTR)L"Import Successful",
		(LPCWSTR)L"Import Successful",
		MB_ICONWARNING | MB_OK | MB_DEFBUTTON1
	);

	switch (msgboxID)
	{
	case IDOK:
		// TODO: add code
		break;
	}
	return;
}

void CSmartcard_UIDlg::DisplayFailMessageBox()
{
	int msgboxID = MessageBox(
		(LPCWSTR)L"Import Failed",
		(LPCWSTR)L"Import Failed",
		MB_ICONWARNING | MB_OK | MB_DEFBUTTON1
	);

	switch (msgboxID)
	{
	case IDOK:
		// TODO: add code
		break;
	}
	return;
}

void CSmartcard_UIDlg::DisplayDatabaseFailMessageBox()
{
	int msgboxID = MessageBox(
		(LPCWSTR)L"Failed to connect to database",
		(LPCWSTR)L"Failed to connect to database",
		MB_ICONWARNING | MB_OK | MB_DEFBUTTON1
	);

	switch (msgboxID)
	{
	case IDOK:
		// TODO: add code
		break;
	}
	return;
}