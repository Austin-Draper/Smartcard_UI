// RFID_Information.cpp : implementation file
//

#include "stdafx.h"
#include "Smartcard_UI.h"
#include "Smartcard_UIDlg.h"
#include "RFID_Information.h"
#include "Enter_Card.h"
#include "afxdialogex.h"

// RFID_Information dialog

IMPLEMENT_DYNAMIC(RFID_Information, CDialogEx)

RFID_Information::RFID_Information(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG3, pParent)
{

}

RFID_Information::~RFID_Information()
{
}

void RFID_Information::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(RFID_Information, CDialogEx)
	ON_BN_CLICKED(IDOK, &RFID_Information::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &RFID_Information::OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL RFID_Information::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: Add extra initialization here
	RFID_Information a;
	a.hCardHandle = hCardHandle;
	a.pioSendPci = pioSendPci;
	GotoDlgCtrl(GetDlgItem(RFID_Fname));//First Name

	return FALSE;
	//return TRUE;  // return TRUE  unless you set the focus to a control
}
// RFID_Information message handlers


void RFID_Information::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CSmartcard_UIDlg a;
	CString _txtname;
	GetDlgItemText(RFID_Fname, _txtname);
	string _fName = a._CStringtoString(_txtname);
	GetDlgItemText(RFID_Minit, _txtname);
	string _mInit = a._CStringtoString(_txtname);
	GetDlgItemText(RFID_Lname, _txtname);
	string _lName = a._CStringtoString(_txtname);
	GetDlgItemText(RFID_PIN, _txtname);
	string _RFID_PIN = a._CStringtoString(_txtname);
	GetDlgItemText(RFID_ExpDate, _txtname);
	string _RFID_ExpDate = a._CStringtoString(_txtname);//modify to however it's supposed to be stored in the card

	//If PIN length != 4, stop here (aka return)
	if (_RFID_PIN.size() != 4)
	{
		AfxMessageBox(_T("PIN length must be 4"));
		return;
	}
	//Start of modify _RFID_ExpDate to proper card format
	//TODO (also if time selected = current date... increment day by 1
	//End of modify _RFID_ExpDate to proper card format

	std::string _fascnTest = "";
	_fascnTest = pseudoFascn();//holds the pseudo fascn
	//do scanner work
	unsigned char aTemplate[TEMPLATE_SIZE];
	thumbPrint(aTemplate);//aTemplate holds the template of the thumb print image

	//do all transmits and work with all data gathered now

	AfxMessageBox(_T("Card Creation Successful."));

	//transfer to main program at very end
	CSmartcard_UIDlg main_dlg;
	int nRet = 5;
	RFID_Information::EndDialog(nRet);
	main_dlg.DoModal();
	CDialogEx::OnOK();
}

void RFID_Information::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CSmartcard_UIDlg main_dlg;
	//transfer to main program at very end
	int nRet = 5;
	RFID_Information::EndDialog(nRet);
	main_dlg.DoModal();

	CDialogEx::OnCancel();
}

std::string RFID_Information::pseudoFascn()
{
	//32 numbers + 5 dashes = 37 in length
	//4-4-6-1-1-16
	std::string _result = "";
	int _randNum = 0;
	srand(time(NULL));
	for (int i = 0; i < 32; i++)
	{
		if (i == 4 || i == 8 || i == 14 || i == 15 || i == 16)
			_result += "-";
		_randNum = rand() % 9 + 0;
		_result += std::to_string(_randNum);
	}

	return _result;
}

//void RFID_Information::thumbPrint(unsigned char aTemplate[])
void RFID_Information::thumbPrint(unsigned char aTemplate[TEMPLATE_SIZE])
{
	//1. Initialize scanner
	UFS_STATUS ufs_res;
	int nScannerNumber = 0;
	ufs_res = UFS_Init();// Initialize scanner module
	if (ufs_res != UFS_OK)
		exit(0);

	//2. Get number of scanners
	ufs_res = UFS_GetScannerNumber(&nScannerNumber);
	if (nScannerNumber != NULL)
	{
		AfxMessageBox(_T("Device is initialized successfully."));
	}
	//3. Get first scanner
	int nScannerIndex = 0;
	HUFScanner hScanner;
	ufs_res = UFS_GetScannerHandle(nScannerIndex, &hScanner);
	//4. Set parameters
	int nValue = 5000;// Set timeout for capturing images to 5 seconds
	ufs_res = UFS_SetParameter(hScanner, UFS_PARAM_TIMEOUT, &nValue);

	// Set template size to 1024 bytes
	nValue = TEMPLATE_SIZE;
	ufs_res = UFS_SetParameter(hScanner, UFS_PARAM_TEMPLATE_SIZE, &nValue);

	// Set not to detect core when extracting template
	nValue = FALSE;
	ufs_res = UFS_SetParameter(hScanner, UFS_PARAM_DETECT_CORE, &nValue);
	//5. Capture image and extract template
	//unsigned char aTemplate[TEMPLATE_SIZE];
	int nTemplateSize;
	int nEnrollQuality = 0;

	while (nEnrollQuality < 70)//if program is closed during this segment after nEnrollQuality was too low, it will stay broken until reader is unplugged and re-plugged in
	{
		// Clear capture buffer
		ufs_res = UFS_ClearCaptureImageBuffer(hScanner);
		// Capture single image
		ufs_res = UFS_CaptureSingleImage(hScanner);
		// If capturing images fails, iterate above capture routine or show error message

		//Extract template from captured image
		ufs_res = UFS_Extract(hScanner, aTemplate, &nTemplateSize, &nEnrollQuality);
		// If extraction is successful, check nEnrollQuality is above the
		// predefined quality threshold
		//Austin: I believe aTemplate now holds the image in char* format
		if (nEnrollQuality < 70)
		{
			AfxMessageBox(_T("Picture quality too low \n" "          Try Again!"));
		}
	}
}