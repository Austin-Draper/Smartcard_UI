// dbinfo_file.cpp : implementation file
//

#include "stdafx.h"
#include "Smartcard_UI.h"
#include "dbinfo_file.h"
#include "afxdialogex.h"
#include "Smartcard_UIDlg.h"

// dbinfo_file dialog

IMPLEMENT_DYNAMIC(dbinfo_file, CDialogEx)

dbinfo_file::dbinfo_file(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG4, pParent)
{

}

dbinfo_file::~dbinfo_file()
{
}

void dbinfo_file::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(dbinfo_file, CDialogEx)
	ON_BN_CLICKED(IDOK, &dbinfo_file::OnBnClickedOk)
END_MESSAGE_MAP()


// dbinfo_file message handlers


void dbinfo_file::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString txtname;//
	GetDlgItemText(enterServerEdit, txtname);//use txtname later to send to main dialog
	int _strlen = txtname.GetLength();
	//if (_strlen == 0)//un-comment this block if PIN isn't allowed to be empty
	//{
	//error popup window
	//return;
	//}
	std::string strStd;//convert CString to std::string
	for (int i = 0; i < txtname.GetLength(); ++i)
	{
		if (txtname[i] <= 0x7f)
			strStd.append(1, static_cast<char>(txtname[i]));
		else
			strStd.append(1, '?');
	}//end of convert CString to std::string
	
	string _ServerName = strStd;

	GetDlgItemText(enterPortEdit, txtname);//use txtname later to send to main dialog
	_strlen = txtname.GetLength();
	//if (_strlen == 0)//un-comment this block if PIN isn't allowed to be empty
	//{
	//error popup window
	//return;
	//}
	strStd = "";
	for (int i = 0; i < txtname.GetLength(); ++i)
	{
		if (txtname[i] <= 0x7f)
			strStd.append(1, static_cast<char>(txtname[i]));
		else
			strStd.append(1, '?');
	}//end of convert CString to std::string

	string _PortNumber = strStd;

	std::cout << "testing if server name and port come through correctly\n";
	ofstream myfile;
	myfile.open("dbinfo.txt");
	//put server and port into dbinfo
	myfile << _ServerName << "\n";
	myfile << _PortNumber;
	myfile.close();

	CSmartcard_UIDlg main_dlg;
	int nRet = 5;
	dbinfo_file::EndDialog(nRet);
	main_dlg.DoModal();







	CDialogEx::OnOK();
}