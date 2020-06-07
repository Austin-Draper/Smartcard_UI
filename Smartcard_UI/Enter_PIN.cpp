// Enter_PIN.cpp : implementation file
//

#include "stdafx.h"
#include "Smartcard_UI.h"
#include "Smartcard_UIDlg.h"
#include "Enter_PIN.h"
#include "afxdialogex.h"
#include <time.h>


// Enter_PIN dialog

IMPLEMENT_DYNAMIC(Enter_PIN, CDialogEx)

Enter_PIN::Enter_PIN(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{
}

Enter_PIN::~Enter_PIN()
{
}

void Enter_PIN::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Enter_PIN, CDialogEx)
	ON_BN_CLICKED(IDOK, &Enter_PIN::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &Enter_PIN::OnBnClickedCancel)
END_MESSAGE_MAP()



BOOL Enter_PIN::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: Add extra initialization here
	//SetDlgItemInt(window2_num, PIN);
	//SetDlgItemInt(window2_num, 123);
	Enter_PIN a;
	a.hCardHandle = hCardHandle;
	a.pioSendPci = pioSendPci;
	a.hContext = hContext;
	GotoDlgCtrl(GetDlgItem(enterPINEdit));

	return FALSE;
	//return TRUE;  // return TRUE  unless you set the focus to a control
}
// Enter_PIN message handlers


void Enter_PIN::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//CDialogEx::OnOK();

	//take the PIN user inserted, look at it 1 decimal at a time and convert into proper hex.
	//run the transmit functions up to PIN transmit. run PIN transmit (looping back for another attempt if user inputs wrong PIN)
	//popup how many uses the user has left OR (if i have to be static) have a popup for when the user is out of attempts
	CString txtname;
	//cout << hCardHandle << endl;
	Enter_PIN a;
	//cout << a.hCardHandle << endl;
	string _temp = "";
	int _counter = 0, _answer;
	CSmartcard_UIDlg main_dlg;
	GetDlgItemText(enterPINEdit, txtname);//use txtname later to send to main dialog
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

	//0x00, 0x20, 0x00, 0x80 is for normal cards... replacing 0x80 for 0x00 is only for CaC
	byte pbSendBuffer3[] = { 0x00, 0x20, 0x00, 0x00, 0x08, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
	for (int i = 5; i < _strlen + 5; i++)
	{
		_temp = strStd[_counter];
		//call function to convert single string to int
		_answer = _convertPIN(_temp);
		pbSendBuffer3[i] = _answer;
		_counter++;
		_temp = "";
	}
	_counter = 0;

	int lReturn;
	int cbSendLength = 14;
	//int cbSendLength = 16;

	//byte pbRecvBuffer[255];
	//DWORD dwSize = 255;
	byte pbRecvBuffer[258];
	DWORD dwSize = 258;

	//byte pbSendBuffer2[] = { 0x00, 0xA4, 0x04, 0x00, 0x0B, 0xA0, 0x00, 0x00, 0x03, 0x08, 0x00, 0x00, 0x10, 0x00, 0x01, 0x00 };

	//byte pbSendBuffer2[] = { 0x00, 0xA4, 0x04, 0x00, 0x09, 0xA0, 0x00, 0x00, 0x03, 0x08, 0x00, 0x00, 0x10, 0x00 };
	byte pbSendBuffer2[] = { 0x00, 0xA4, 0x04, 0x00, 0x07, 0xA0, 0x00, 0x00, 0x01, 0x16, 0xDB, 0x00 };
	cbSendLength = 12;

	//run the CCC first, then the RID second
	//byte	CAC_RID[7] = { 0xA0, 0x00, 0x00, 0x00, 0x79, 0x01, 0x00 };		//CAC RID = A0 00 00 00 79 01 00 
	//byte	CAC_CCC[7] = { 0xA0,0x00,0x00,0x01,0x16,0xDB,0x00 };			//JC_CAC2_CCC_AID

	//SCARD_PCI_T1
	lReturn = SCardTransmit(hCardHandle, pioSendPci, pbSendBuffer2, cbSendLength, NULL, pbRecvBuffer, &dwSize);//girls face needs T0... rest use T1
	if (lReturn == SCARD_S_SUCCESS) {
		//	std::cout << "Transmit Success!\n";
	}
	else
	{
		//standard error message popup that something failed. put user back on same page
	//	std::cout << "Transmit Failed!\n";
	//	std::cout << lReturn << std::endl;
		system("pause");
		exit(1);
	}
	byte pbSendBuffer2t[] = { 0x00, 0xA4, 0x04, 0x00, 0x07, 0xA0, 0x00, 0x00, 0x00, 0x79, 0x01, 0x00 };
	cbSendLength = 12;

	//SCARD_PCI_T1
	lReturn = SCardTransmit(hCardHandle, pioSendPci, pbSendBuffer2t, cbSendLength, NULL, pbRecvBuffer, &dwSize);//girls face needs T0... rest use T1
	if (lReturn == SCARD_S_SUCCESS) {
		std::cout << "Transmit Success!\n";
	}
	//if ((int)pbRecvBuffer[0] == 144)
	//{
		//THIRD TRANSMIT
	cbSendLength = 13;
	//might need SCARD_PCI_T1 to be dynamic in deciding to use T1 or T0
	lReturn = SCardTransmit(hCardHandle, pioSendPci, pbSendBuffer3, cbSendLength, NULL, pbRecvBuffer, &dwSize);
	int _pinCode1 = (int)pbRecvBuffer[0];
	int _pinCode2 = (int)pbRecvBuffer[1];
	if (lReturn == SCARD_S_SUCCESS) {
		//	std::cout << "Transmit Success!\n";
	}
	else
	{
		//error message PIN was invalid. In future tell how many are left as well as when 0 attempts remain
		//return user to same page
		//	std::cout << "Transmit Failed!\n";
		//	std::cout << lReturn << std::endl;
		system("pause");
		return;
	}
	//popup message invalid PIN (if returned hex message states its invalid
	if (_pinCode1 != 144)
	{
		DisplayResourcePINMessageBox(_pinCode2);
		return;
	}
	//if successful up to this point... need to go to visual studio CaC.cpp lines 318+
	//to select the person object 0200


	byte pbSendBufferperson[] = { 0x00, 0xA4, 0x02, 0x00, 0x02, 0x02, 0x00 };//success
																			 //byte pbSendBufferperson[] = { 0x00, 0xA4, 0x02, 0x00, 0x03, 0xA0, 0x02, 0x00 };//fail
	cbSendLength = 7;

	//SCARD_PCI_T1
	lReturn = SCardTransmit(hCardHandle, pioSendPci, pbSendBufferperson, cbSendLength, NULL, pbRecvBuffer, &dwSize);//girls face needs T0... rest use T1
	if (lReturn == SCARD_S_SUCCESS) {
		std::cout << "Transmit Success!\n";
	}
	else
	{
		//standard error message popup that something failed. put user back on same page
		std::cout << "Transmit Failed!\n";
		//	std::cout << lReturn << std::endl;
		system("pause");
		exit(1);
	}
	//i have connected to the person file 0200
	//now i need to read the data off the file
	//0x02
	byte pbSendBufferRead[] = { 0x80, 0x52, 0x00, 0x00, 0x02, 0x01, 0x02 };//7 bytes total... byte 6-> 0x01 = T-buffer, 0x02 = V-buffer																 
																		   //read the first 2 bytes to get tag length, then call again with that length to get entire tag
	cbSendLength = 7;

	//SCARD_PCI_T1
	lReturn = SCardTransmit(hCardHandle, pioSendPci, pbSendBufferRead, cbSendLength, NULL, pbRecvBuffer, &dwSize);//girls face needs T0... rest use T1
	if (lReturn == SCARD_S_SUCCESS) {
		std::cout << "Transmit Success!\n";
	}
	else
	{
		//standard error message popup that something failed. put user back on same page
		std::cout << "Transmit Failed!\n";
		//	std::cout << lReturn << std::endl;
		system("pause");
		exit(1);
	}

	byte pbSendBuffer5t[] = { 0x00, 0xC0, 0x00, 0x00, 0x04 };
	cbSendLength = 5;
	//byte pbRecvBuffer2[258];
	//dwSize = 258;
	dwSize = 4;

	lReturn = SCardTransmit(hCardHandle, pioSendPci, pbSendBuffer5t, cbSendLength, NULL, pbRecvBuffer, &dwSize);//girls face needs T0... rest use T1
	if (lReturn == SCARD_S_SUCCESS) {
		//std::cout << "Transmit Success!\n";
		for (int i = 0; i < dwSize; i++)
		{
			std::cout << (int)pbRecvBuffer[i] << " ";
		}//first transmit for T-buffer is returning: 108, 2
		 //the 2 is either the current tag, next tag, or leftover from my last call... I think it's the last option
	}
	else
	{
		std::cout << "Transmit Failed!\n";
		//std::cout << lReturn << std::endl;
		system("pause");
		//exit(1);
	}

	//0x02//0x6C
	byte pbSendBufferRead2[] = { 0x80, 0x52, 0x00, 0x00, 0x02, 0x01, 64 };//7 bytes total... byte 6-> 0x01 = T-buffer, 0x02 = V-buffer																 
																		  //read the first 2 bytes to get tag length, then call again with that length to get entire tag
	cbSendLength = 7;
	dwSize = 110;
	//SCARD_PCI_T1
	lReturn = SCardTransmit(hCardHandle, pioSendPci, pbSendBufferRead2, cbSendLength, NULL, pbRecvBuffer, &dwSize);//girls face needs T0... rest use T1
	if (lReturn == SCARD_S_SUCCESS) {
		std::cout << "Transmit Success!\n";
	}
	else
	{
		//standard error message popup that something failed. put user back on same page
		std::cout << "Transmit Failed!\n";
		//	std::cout << lReturn << std::endl;
		system("pause");
		exit(1);
	}


	//0x6E
	byte pbSendBuffer5t2[] = { 0x00, 0xC0, 0x00, 0x00, 64 };
	cbSendLength = 5;
	//byte pbRecvBuffer2[258];
	//dwSize = 258;
	dwSize = 110;//test with lower number if i run into issues

	lReturn = SCardTransmit(hCardHandle, pioSendPci, pbSendBuffer5t2, cbSendLength, NULL, pbRecvBuffer, &dwSize);//girls face needs T0... rest use T1
	if (lReturn == SCARD_S_SUCCESS) {
		//std::cout << "Transmit Success!\n";
		for (int i = 0; i < dwSize; i++)
		{
			std::cout << (int)pbRecvBuffer[i] << " ";
		}//first transmit for T-buffer is returning: 108, 2
		 //the 2 is either the current tag, next tag, or leftover from my last call... I think it's the last option
	}
	else
	{
		std::cout << "Transmit Failed!\n";
		//std::cout << lReturn << std::endl;
		system("pause");
		//exit(1);
	}
	//this currently fills receive buffer with all info but no tags... need to use T-buffer more I think... maybe send tag values to V-buffer? such as 0x01 for firstname, 0x02 middlename, etc... 

	string CaC_fname = "";//0x01
	int CaC_fname_len = 0;
	string CaC_mname = "";//0x02
	int CaC_mname_len = 0;
	string CaC_lname = "";//0x03
	int CaC_lname_len = 0;
	int CaC_count = 0;
	bool CaC_runtag = true;
	while (CaC_count < dwSize)
	{
		if (pbRecvBuffer[CaC_count] == 0x01)
		{
			CaC_count++;
			CaC_fname_len = pbRecvBuffer[CaC_count];
			CaC_count++;
			break;
		}
		CaC_count++;
	}
	while (CaC_count < dwSize)
	{
		if (pbRecvBuffer[CaC_count] == 0x02)
		{
			CaC_count++;
			CaC_mname_len = pbRecvBuffer[CaC_count];
			CaC_count++;
			break;
		}
		CaC_count++;
	}
	while (CaC_count < dwSize)
	{
		if (pbRecvBuffer[CaC_count] == 0x03)
		{
			CaC_count++;
			CaC_lname_len = pbRecvBuffer[CaC_count];
			CaC_count++;
			break;
		}
		CaC_count++;
	}

	//now get actual string names out of V_Buffer
	byte pbSendBufferRead3[] = { 0x80, 0x52, 0x00, 0x00, 0x02, 0x02, 108 };//7 bytes total... byte 6-> 0x01 = T-buffer, 0x02 = V-buffer																 
																		   //read the first 2 bytes to get tag length, then call again with that length to get entire tag
	cbSendLength = 7;
	dwSize = 110;
	//SCARD_PCI_T1
	lReturn = SCardTransmit(hCardHandle, pioSendPci, pbSendBufferRead3, cbSendLength, NULL, pbRecvBuffer, &dwSize);//girls face needs T0... rest use T1
	if (lReturn == SCARD_S_SUCCESS) {
		std::cout << "Transmit Success!\n";
	}
	else
	{
		//standard error message popup that something failed. put user back on same page
		std::cout << "Transmit Failed!\n";
		//	std::cout << lReturn << std::endl;
		system("pause");
		exit(1);
	}


	//0x6E
	byte pbSendBuffer5t3[] = { 0x00, 0xC0, 0x00, 0x00, 108 };
	cbSendLength = 5;
	//byte pbRecvBuffer2[258];
	//dwSize = 258;
	dwSize = 110;//test with lower number if i run into issues

	lReturn = SCardTransmit(hCardHandle, pioSendPci, pbSendBuffer5t3, cbSendLength, NULL, pbRecvBuffer, &dwSize);//girls face needs T0... rest use T1
	if (lReturn == SCARD_S_SUCCESS) {
		//std::cout << "Transmit Success!\n";
		for (int i = 0; i < dwSize; i++)
		{
			std::cout << (int)pbRecvBuffer[i] << " ";
		}//first transmit for T-buffer is returning: 108, 2
		 //the 2 is either the current tag, next tag, or leftover from my last call... I think it's the last option
	}
	else
	{
		std::cout << "Transmit Failed!\n";
		//std::cout << lReturn << std::endl;
		system("pause");
		//exit(1);
	}

	//go until '/0', then use my 3 lengths i saved
	CaC_count = 0;
	while (CaC_count < dwSize)
	{
		if (pbRecvBuffer[CaC_count] == NULL)
		{
			CaC_count++;
			for (int i = 0; i < CaC_fname_len; i++)
			{
				CaC_fname = CaC_fname + (char)pbRecvBuffer[CaC_count];
				CaC_count++;
			}

			if (CaC_fname_len > 0 && CaC_fname != "")
			{
				break;
			}
		}
		CaC_count++;
	}
	for (int i = 0; i < CaC_mname_len; i++)
	{
		CaC_mname = CaC_mname + (char)pbRecvBuffer[CaC_count];
		CaC_count++;
	}
	for (int i = 0; i < CaC_lname_len; i++)
	{
		CaC_lname = CaC_lname + (char)pbRecvBuffer[CaC_count];
		CaC_count++;
	}

	//send it to database cpp file just like the printed information names
	main_dlg.f_name = CaC_fname.c_str();
	main_dlg.l_name = CaC_lname.c_str();
	main_dlg.m_initial = CaC_mname.c_str();
	//transfer to main program at very end
	//int nRett = 5;
	//Enter_PIN::EndDialog(nRett);
	//main_dlg.DoModal();

	//}




	



    











	//FOURTH TRANSMIT
	int hex_sw[2];
	//byte pbSendBuffer4[] = { 0x00, 0xCB, 0x3F, 0xFF, 0x05, 0x5C, 0x03, 0x5F, 0xC1, 0x02 };
	byte pbSendBuffer4[] = { 0x00, 0xCB, 0x3F, 0xFF, 0x05, 0x5C, 0x03, 0x5F, 0xC1, 0x02, 0x00 };
	//83 130........ government starts at 200.... 255 and 256 are 97 0
	//cbSendLength = 10;
	cbSendLength = 11;
	dwSize = 258;
	vector<int> hex_storage;
	int sw_counter = 0;
	lReturn = SCardTransmit(hCardHandle, pioSendPci, pbSendBuffer4, cbSendLength, NULL, pbRecvBuffer, &dwSize);//girls face needs T0... rest use T1
	if (lReturn == SCARD_S_SUCCESS) {
		//hex_sw[0] = (int)pbRecvBuffer[0];
		//hex_sw[1] = (int)pbRecvBuffer[1];
		for (int i = 0; i < dwSize - 2; ++i)
		{
			hex_storage.push_back((int)pbRecvBuffer[i]);//1792
			sw_counter++;
		}
		hex_sw[0] = (int)pbRecvBuffer[sw_counter];
		hex_sw[1] = (int)pbRecvBuffer[sw_counter + 1];
		sw_counter = 0;
	}
	else
	{
	//	std::cout << "Transmit Failed!\n";
	//	std::cout << lReturn << std::endl;
		system("pause");
		exit(1);
	}



	//FIFTH TRANSMIT
	//Run until pbRecvBuffer ends with 90 00

	//int sw_counter = 0;
	//vector<int> hex_storage;//just dont append when its the final two numbers (those are sw1 and sw2)... maybe have a second loop that runs exactly 2 times and have main loop run 2 less times
							//byte pbSendBuffer5[] = { 0x00, 0xC0, 0x00, 0x00, 0x00 };//repeat read response until all is read
	byte pbSendBuffer5[] = { 0x00, 0xC0, 0x00, 0x00, 0x00 };
	cbSendLength = 5;
	byte pbRecvBuffer2[258];
	//dwSize = 258;
	dwSize = hex_sw[1] + 2;
	bool first_run = true;
	do
	{
		if (hex_sw[1] == 0)
		{
			dwSize = 258;
		}
		if (first_run == false && hex_sw[1] != 0)
		{
			dwSize = hex_sw[1] +2;

			pbSendBuffer5[4] = (int)dwSize-2;
			//pbSendBuffer5[4] = 0x86;//134

		}
		lReturn = SCardTransmit(hCardHandle, pioSendPci, pbSendBuffer5, cbSendLength, NULL, pbRecvBuffer2, &dwSize);//girls face needs T0... rest use T1
		if (lReturn == SCARD_S_SUCCESS) {
			//std::cout << "Transmit Success!\n";
			//std::cout << lReturn << std::endl;//this just shows lReturn = 0 which means success
			for (int i = 0; i < dwSize; i++)
			{
				//std::cout << pbRecvBuffer2[i] << " ";
			}
			//std::cout << std::endl;
			for (int i = 0; i < dwSize - 2; ++i)
			{
				hex_storage.push_back((int)pbRecvBuffer2[i]);//1792
				sw_counter++;
			}
			hex_sw[0] = (int)pbRecvBuffer2[sw_counter];
			hex_sw[1] = (int)pbRecvBuffer2[sw_counter + 1];
			int _temptest = (int)pbRecvBuffer2[sw_counter + 1];
			sw_counter = 0;
			if (first_run == true)
				first_run = false;
		}
		else
		{
			//std::cout << "Transmit Failed!\n";
			//std::cout << lReturn << std::endl;
			system("pause");
			//exit(1);
		}

	} while (hex_sw[0] != 144);//144 is 90 in hex (90 00)







	//CSmartcard_UIDlg main_dlg;
	vector<string> fiveBinary;
	string _agencyCode, _systemCode, _credentialNumber, _CS, _ICI, _PI, _OC, _OI, _POA, _LRC;
	binaryConvert(hex_storage, fiveBinary);//pass in hex_storage holding data to be manipulated... and fiveBinary to collect result data
										   //now pass fiveBinary to a function along with a variable (passed by reference) for every bit of FASC-N information to be filled in the function
	parseFASCN(fiveBinary, _agencyCode, _systemCode, _credentialNumber, _CS, _ICI, _PI, _OC, _OI, _POA, _LRC, main_dlg);
	//cout << _agencyCode << endl;
	//cout << _systemCode << endl;
	//cout << _credentialNumber << endl;
	//cout << _CS << endl;
	//cout << _ICI << endl;
	//cout << _PI << endl;
	//cout << _OC << endl;
	//cout << _OI << endl;
	//cout << _POA << endl;
	//cout << _LRC << endl;

	//take out leading zeros before sending to dialog
	int _holder = _PI.size();
	for (int i = 0; i < _holder; i++)
	{
		if (_PI[0] == '0')
		{
			_PI.erase(0, 1);
		}
		else
			break;
	}
	_holder = _credentialNumber.size();
	for (int i = 0; i < _holder; i++)
	{
		if (_credentialNumber[0] == '0')
		{
			_credentialNumber.erase(0, 1);
		}
		else
			break;
	}

	main_dlg._agencyCode = _agencyCode.c_str();
	main_dlg._systemCode = _systemCode.c_str();
	main_dlg._credentialNumber = _credentialNumber.c_str();
	main_dlg._CS = _CS.c_str();
	main_dlg._ICI = _ICI.c_str();
	main_dlg._PI = _PI.c_str();
	main_dlg._OC = _OC.c_str();
	main_dlg._OI = _OI.c_str();
	main_dlg._POA = _POA.c_str();
	main_dlg._LRC = _LRC.c_str();
	main_dlg.PIN = txtname;
	//from here until return we will check for 0x32, 0x33, 0x34, etc...
	//when we find one, run the function for it followed by the parse function for it
	int BDC_Length = 0;
	if (hex_storage[0] == 49)//0x31, Unknown???? (optional)
	{
		fiveBinary.clear();
		hex_storage.erase(hex_storage.begin());
		BDC_Length = (int)hex_storage[0];
		hex_storage.erase(hex_storage.begin());
		if (BDC_Length == 0)
		{

		}
		else
		{
			BDC_Convert(hex_storage, fiveBinary, BDC_Length);//test: might need to read binary as 4 bits at a time instead of 5 (might only need parity bit for FASCN)
			string _Unknown = parseOI(fiveBinary);//OI now holds Organizational Identifier
			cout << "UNKNOWN IS: " << _Unknown << endl;
		}
	}
	if (hex_storage[0] == 50)//0x32, Organizational Identifier (optional)
	{
		fiveBinary.clear();
		hex_storage.erase(hex_storage.begin());
		BDC_Length = (int)hex_storage[0];
		hex_storage.erase(hex_storage.begin());
		if (BDC_Length == 0)
		{

		}
		else
		{
			BDC_ConvertFour(hex_storage, fiveBinary, BDC_Length);//test: might need to read binary as 4 bits at a time instead of 5 (might only need parity bit for FASCN)
			string _OrgId = parseOIFour(fiveBinary);//OI now holds Organizational Identifier
			cout << "ORGANIZATIONAL IDENTIFIER IS: " << _OI << endl;
			//can ignore last 2 lines... only need to run BDC_ConvertFour
		}

	}
	if (hex_storage[0] == 51)//0x33 DUNS (optional)
	{
		fiveBinary.clear();
		hex_storage.erase(hex_storage.begin());
		BDC_Length = (int)hex_storage[0];
		hex_storage.erase(hex_storage.begin());
		if (BDC_Length == 0)
		{

		}
		else
		{
			BDC_ConvertFour(hex_storage, fiveBinary, BDC_Length);
			string _DUNS = parseOIFour(fiveBinary);
			cout << "DUNS IS: " << _DUNS << endl;
		}

	}
	if (hex_storage[0] == 52)//0x34 GUID
	{
		fiveBinary.clear();
		hex_storage.erase(hex_storage.begin());
		BDC_Length = (int)hex_storage[0];
		hex_storage.erase(hex_storage.begin());
		if (BDC_Length == 0)
		{

		}
		else
		{
			//call a function to parse data for PIV-I (only call if Agency Code, System Code, and Credential Number are only 9's)
			if (_agencyCode == "9999" && _systemCode == "9999" && _credentialNumber == "999999")
			{
				string cardType = "CMV_U";
				main_dlg.cardType = cardType.c_str();
				BDC_ConvertFour(hex_storage, fiveBinary, BDC_Length);
				string _GUID = parseGUID(fiveBinary);
				cout << "GUID IS: " << _GUID << endl;
				main_dlg._GUID = _GUID.c_str();
				//now parse acquired UUID approptiately for PIV-I card
			}
			else
			{
				string cardType = "CMV_F";
				main_dlg.cardType = cardType.c_str();
				BDC_ConvertFour(hex_storage, fiveBinary, BDC_Length);
				string _GUID = parseGUID(fiveBinary);
				cout << "GUID IS: " << _GUID << endl;
				main_dlg._GUID = _GUID.c_str();
			}

		}

	}

	string expirationDate = "";
	int printedLength = 0;
	if (hex_storage[0] == 53)//0x35 Expiration Date
	{
		hex_storage.erase(hex_storage.begin());
		printedLength = (int)hex_storage[0];
		hex_storage.erase(hex_storage.begin());
		if (printedLength == 0)
		{

		}
		else
		{// 0/20/EC02             20321202
			expirationDate = parsePrinted(hex_storage, printedLength);
			cout << "Expiration Date Is: " << expirationDate << endl;

			string exp_day, exp_month, exp_year;
			exp_day = expirationDate[6];
			exp_day += expirationDate[7];
			exp_month = expirationDate[4];
			exp_month += expirationDate[5];
			exp_year = expirationDate[0];
			exp_year += expirationDate[1];
			exp_year += expirationDate[2];
			exp_year += expirationDate[3];
			expirationDate = exp_month + '/' + exp_day + '/' + exp_year;
			cout << expirationDate << endl;
			main_dlg.expirationDate = expirationDate.c_str();
		}
	}
	if (hex_storage[0] == 54)//0x36 CardHolder UUID (Optional)
	{

	}

	//example date:    12/16/2020    month,day,year
	/*work with current and expiration date*/
	time_t now = time(0);
	tm ltm;
	localtime_s(&ltm, &now);
	int _day, _month, _year;
	cout << ltm.tm_mday << endl;
	_day = ltm.tm_mday;
	cout << ltm.tm_mon << endl;
	_month = ltm.tm_mon;
	cout << ltm.tm_year+1900 << endl;
	_year = ltm.tm_year + 1900;
	string current_time = to_string(_month + 1) + "/" + to_string(_day) + "/" + to_string(_year);
	cout << current_time << endl;
	main_dlg.currentDate = current_time.c_str();
	//current done.... expiration is done in printedinfo below
	/*end of working with current and expiration date*/
	












	//NOW I NEED TO GET PRINTED INFORMATION (full name and expiration date)
	//run the transmit to connect to that file as well as any get response transmits I need (copy above)
	//FOURTH TRANSMIT
	pbSendBuffer4[9] = { 0x09 };
	//cbSendLength = 10;
	cbSendLength = 11;
	dwSize = 258;//might need to remove
	sw_counter = 0;
	hex_storage.clear();
	lReturn = SCardTransmit(hCardHandle, pioSendPci, pbSendBuffer4, cbSendLength, NULL, pbRecvBuffer, &dwSize);//girls face needs T0... rest use T1
	if (lReturn == SCARD_S_SUCCESS) {
		//hex_sw[0] = (int)pbRecvBuffer[0];
		//hex_sw[1] = (int)pbRecvBuffer[1];
		//if (hex_sw[0] == 106 && hex_sw[1] == 130)
		//{
		//	int nRet = 5;
		//	Enter_PIN::EndDialog(nRet);
		//	main_dlg.DoModal();
		//}
		hex_sw[0] = (int)pbRecvBuffer[dwSize];
		hex_sw[1] = (int)pbRecvBuffer[dwSize + 1];
		if (pbRecvBuffer[0] == 106 && pbRecvBuffer[1] == 130)
		{
			int nRet = 5;
			Enter_PIN::EndDialog(nRet);
			main_dlg.DoModal();
		}
		else
		{
			for (int i = 0; i < dwSize - 2; ++i)
			{
				hex_storage.push_back((int)pbRecvBuffer[i]);//1792
				sw_counter++;
			}
			hex_sw[0] = (int)pbRecvBuffer[sw_counter];
			hex_sw[1] = (int)pbRecvBuffer[sw_counter + 1];
			sw_counter = 0;
		}

	}
	else
	{
		//std::cout << "Transmit Failed!\n";
		//std::cout << lReturn << std::endl;
		system("pause");
		exit(1);
	}

	//FIFTH TRANSMIT
	//Run until pbRecvBuffer ends with 90 00
	/*
	sw_counter = 0;
	//hex_storage.clear();//just dont append when its the final two numbers (those are sw1 and sw2)... maybe have a second loop that runs exactly 2 times and have main loop run 2 less times
	cbSendLength = 5;
	//dwSize = 258;
	first_run = true;
	//dwSize = hex_sw[1] + 2;
	do
	{
		if (first_run == true && hex_sw[1] != 0)
		{
			pbSendBuffer5[4] = (int)dwSize - 2;
		}
		if (hex_sw[1] == 0)
		{
			dwSize = 258;
		}
		if (first_run == false && hex_sw[1] != 0)
		{
			dwSize = hex_sw[1] + 2;

			pbSendBuffer5[4] = (int)dwSize - 2;
			//pbSendBuffer5[4] = 0x86;//134

		}
		lReturn = SCardTransmit(hCardHandle, pioSendPci, pbSendBuffer5, cbSendLength, NULL, pbRecvBuffer2, &dwSize);//girls face needs T0... rest use T1
		if (lReturn == SCARD_S_SUCCESS) {
			//std::cout << "Transmit Success!\n";
			//std::cout << lReturn << std::endl;//this just shows lReturn = 0 which means success
			for (int i = 0; i < dwSize; i++)
			{
				//std::cout << pbRecvBuffer2[i] << " ";
			}
			//std::cout << std::endl;
			for (int i = 0; i < dwSize - 2; ++i)
			{
				//cout << hex << (int)pbRecvBuffer2[i] << " ";
				hex_storage.push_back((int)pbRecvBuffer2[i]);
				sw_counter++;
			}
			//cout << endl;
			hex_sw[0] = (int)pbRecvBuffer2[sw_counter];
			hex_sw[1] = (int)pbRecvBuffer2[sw_counter + 1];
			//cout << hex_sw[0] << " " << hex_sw[1] << "\n";
			sw_counter = 0;
			if (first_run == true)
				first_run = false;
		}
		else
		{
			std::cout << "Transmit Failed!\n";
			std::cout << lReturn << std::endl;
			system("pause");
			//exit(1);
		}

	} while (hex_sw[0] != 144);//144 is 90 in hex (90 00)
	//cout << "out of loop" << endl;
	*/
	//while reading the hex (out of hex_storage), do the coding looking for the int value of the hex tags + lengths
	//then read that much data into variables like before
	//unlike before... not using binary... just putting characters into string variables
	string full_name = "", f_name = "", m_initial = "", l_name = "";//tag 0x01
	string employeeAffiliation = "";//tag 0x02
	string tag_0x03 = "";//tag 0x03 can exist but will either not be used or be 0 bytes
	expirationDate = "";//tag 0x04
	string agencyCardSerialNumber = "";//tag 0x05
	string issuerIdentification = "";//tag 0x06
									 //not looking for tag 0x07 or 0x08... add in future if necessary

	if (hex_storage.size() == 0)
	{
		//no printed info... just send all previously gathered info to main dialog and delete this dialog
		int nRet = 5;
		Enter_PIN::EndDialog(nRet);
		main_dlg.DoModal();
	}
	printedLength = 0;
	//sift through hex_storage until we reach tag 0x01
	for (int i = 0; i < hex_storage.size(); i++)
	{
		if (hex_storage[0] == 1)
		{
			hex_storage.erase(hex_storage.begin());
			printedLength = (int)hex_storage[0];
			hex_storage.erase(hex_storage.begin());
			break;
		}
		else
		{
			hex_storage.erase(hex_storage.begin());
		}
	}
	full_name = parsePrinted(hex_storage, printedLength);
	cout << "Full Name Is: " << full_name << endl;
	//if comma, do the one below, else... do the fname mname lname one
	char c = ',';
	size_t found = full_name.find(c);
	//if (found != string::npos)//if there is a comma, do this if statement
	if(full_name.find(c) != string::npos)
	{
		//need to parse full_name into 3 pieces
		int _step = 0;
		while (full_name[_step] != ',' && full_name[_step] != ' ')
		{
			l_name = l_name + full_name[_step];
			_step = _step + 1;
		}
		//full_name.erase(full_name.begin());
		_step = _step + 1;
		while (full_name[_step] == ' ')
		{
			_step = _step + 1;
		}
		while (full_name[_step] != ' ')
		{
			f_name = f_name + full_name[_step];
			_step = _step + 1;
		}
		while (full_name[_step] == ' ')
		{
			_step = _step + 1;
		}
		for (_step; _step <= full_name.size(); _step++)
		{
			if (full_name[_step] == ' ')
			{
				break;
			}
			m_initial = m_initial + full_name[_step];
		}
	}
	
	else//if there is no comma, run this else statement instead
	{
		//need to parse full_name into 3 pieces
		int _step = 0;
		while (full_name[_step] != ' ')
		{
			f_name = f_name + full_name[_step];
			_step = _step + 1;
		}
		//full_name.erase(full_name.begin());
		_step = _step + 1;
		while (full_name[_step] == ' ')
		{
			_step = _step + 1;
		}
		while (full_name[_step] != ' ')
		{
			m_initial = m_initial + full_name[_step];
			_step = _step + 1;
		}
		while (full_name[_step] == ' ')
		{
			_step = _step + 1;
		}
		for (_step; _step <= full_name.size(); _step++)
		{
			if (full_name[_step] == ' ')
			{
				break;
			}
			l_name = l_name + full_name[_step];
		}


	}
	


	
	/*
	for (int i = 0; i < full_name.size(); i++)//will run too far. have a stopper after middle initial
	{
		if (full_name[i] == ',')
		{
			i = i + 2;
			for (i; i < full_name.size(); i++)
			{
				if (full_name[i] == ' ')
				{
					i = i + 1;
					m_initial = full_name[i];
					break;
				}
				l_name = l_name + full_name[i];
			}
			break;
		}
		else
		{
			f_name = f_name + full_name[i];
		}
	}
	*/
	main_dlg.f_name = f_name.c_str();
	main_dlg.l_name = l_name.c_str();
	main_dlg.m_initial = m_initial.c_str();


	if (hex_storage[0] == 2)//0x02 employee affiliation
	{
		hex_storage.erase(hex_storage.begin());
		printedLength = (int)hex_storage[0];
		hex_storage.erase(hex_storage.begin());
		if (printedLength == 0)
		{

		}
		else
		{
			employeeAffiliation = parsePrinted(hex_storage, printedLength);
			cout << "Employee Affiliation Is: " << employeeAffiliation << endl;
		}
	}

	if (hex_storage[0] == 3)//0x03 unknown tag
	{
		hex_storage.erase(hex_storage.begin());
		printedLength = (int)hex_storage[0];
		hex_storage.erase(hex_storage.begin());
		if (printedLength == 0)
		{

		}
		else
		{
			tag_0x03 = parsePrinted(hex_storage, printedLength);
			cout << "tag_0x03 Is: " << tag_0x03 << endl;
		}
	}

	if (hex_storage[0] == 4)//0x04 expirationDate
	{
		hex_storage.erase(hex_storage.begin());
		printedLength = (int)hex_storage[0];
		hex_storage.erase(hex_storage.begin());
		if (printedLength == 0)
		{

		}
		else
		{
			//ex:     01DEC2012 (convert this to  12/1/2012)
			/*
			expirationDate = parsePrinted(hex_storage, printedLength);
			cout << "Expiration Date Is: " << expirationDate << endl;

			string exp_day, exp_month, exp_year;
			exp_day = expirationDate[0];
			exp_day += expirationDate[1];
			exp_month = expirationDate[2];
			exp_month += expirationDate[3];
			exp_month += expirationDate[4];
			exp_year = expirationDate[5];
			exp_year += expirationDate[6];
			exp_year += expirationDate[7];
			exp_year += expirationDate[8];
			parseExpiration(exp_day, exp_month, exp_year);//make the function
			expirationDate = exp_month + '/' + exp_day + '/' + exp_year;
			main_dlg.expirationDate = expirationDate.c_str();
			*/
		}
	}

	if (hex_storage[0] == 5)//0x05 agencyCardSerialNumber
	{
		hex_storage.erase(hex_storage.begin());
		printedLength = (int)hex_storage[0];
		hex_storage.erase(hex_storage.begin());
		if (printedLength == 0)
		{

		}
		else
		{
			agencyCardSerialNumber = parsePrinted(hex_storage, printedLength);
			cout << "Agency Card Serial Number Is: " << agencyCardSerialNumber << endl;
		}
	}

	if (hex_storage[0] == 6)//0x06 issuerIdentification
	{
		hex_storage.erase(hex_storage.begin());
		printedLength = (int)hex_storage[0];
		hex_storage.erase(hex_storage.begin());
		if (printedLength == 0)
		{

		}
		else
		{
			issuerIdentification = parsePrinted(hex_storage, printedLength);
			cout << "Issuer Identification Is: " << issuerIdentification << endl;
		}
	}







	


	//transfer to main program at very end
	int nRet = 5;
	Enter_PIN::EndDialog(nRet);
	main_dlg.DoModal();
}

//ex:     01DEC2012 (convert this to  12/1/2012)
void Enter_PIN::parseExpiration(string &_day, string &_month, string &_year)
{
	//if day has leading zero, remove.
	//change month to a number
	//year is perfect as is
	if (_day[0] == '0')
	{
		_day.erase(0, 1);//remove just the first character of the 2 character string
	}
	if (_month == "JAN")
	{
		_month = "1";
	}
	else if (_month == "FEB")
	{
		_month = "2";
	}
	else if (_month == "MAR")
	{
		_month = "3";
	}
	else if (_month == "APR")
	{
		_month = "4";
	}
	else if (_month == "MAY")
	{
		_month = "5";
	}
	else if (_month == "JUN")
	{
		_month = "6";
	}
	else if (_month == "JUL")
	{
		_month = "7";
	}
	else if (_month == "AUG")
	{
		_month = "8";
	}
	else if (_month == "SEP")
	{
		_month = "9";
	}
	else if (_month == "OCT")
	{
		_month = "10";
	}
	else if (_month == "NOV")
	{
		_month = "11";
	}
	else if (_month == "DEC")
	{
		_month = "12";
	}
	else
	{
		_month = "0";
	}
	return;

}

int Enter_PIN::_convertPIN(string _temp)
{
	int _result;
	_result = std::stoi(_temp);
	if (_result == 0)
	{
		_result = 48;
		return _result;
	}
	else if (_result == 1)
	{
		_result = 49;
		return _result;
	}
	else if (_result == 2)
	{
		_result = 50;
		return _result;
	}
	else if (_result == 3)
	{
		_result = 51;
		return _result;
	}
	else if (_result == 4)
	{
		_result = 52;
		return _result;
	}
	else if (_result == 5)
	{
		_result = 53;
		return _result;
	}
	else if (_result == 6)
	{
		_result = 54;
		return _result;
	}
	else if (_result == 7)
	{
		_result = 55;
		return _result;
	}
	else if (_result == 8)
	{
		_result = 56;
		return _result;
	}
	else if (_result == 9)
	{
		_result = 57;
		return _result;
	}
	else
	{
		_result = 0;
		return _result;
	}
}












//START OF HELPER FUNCTIONS
void Enter_PIN::binaryConvert(vector<int> &hex_storage, vector<string> &fiveBinary)
{
	string binary, holder, _test;
	
	int BDC_Length = 0;
	for (int i = 0; i < hex_storage.size();)
	{
		if (hex_storage[i] == 48)//hex 0x30 tag
		{
			BDC_Length = hex_storage[i + 1];
			hex_storage.erase(hex_storage.begin());
			hex_storage.erase(hex_storage.begin());
			break;
		}
		hex_storage.erase(hex_storage.begin());
	}



	//make everything below a helper function? send it 1. binary by reference and 2. BDC_Length by reference and 3. hex_storage by reference and 4. fiveBinary
	for (int counter = 0; counter < BDC_Length; counter++)//replace 25 with hex length variable which is the value after tag
	{
		binary = binary + bitset<8>(hex_storage[0]).to_string();
		hex_storage.erase(hex_storage.begin());
	}
	cout << "spitting out binary \n";
	cout << binary << endl;
	//2. sift through the new string, taking 5 bytes at a time and storing them in fiveBinary
	int counter = 0;
	for (int i = 0; counter < binary.length(); i++)
	{
		holder = holder + binary[counter];
		counter++;
		holder = holder + binary[counter];
		counter++;
		holder = holder + binary[counter];
		counter++;
		holder = holder + binary[counter];
		counter++;
		holder = holder + binary[counter];
		counter++;
		//fiveBinary[i] = holder;
		fiveBinary.push_back(holder);
		holder = "";
	}
	cout << endl;
	for (int i = 0; i < fiveBinary.size(); i++)
	{
		cout << fiveBinary[i] << endl;
	}
	return;

}

void Enter_PIN::BDC_Convert(vector<int> &hex_storage, vector<string> &fiveBinary, int BDC_Length)
{
	string binary = "";
	string holder = "";
	for (int counter = 0; counter < BDC_Length; counter++)
	{
		binary = binary + bitset<8>(hex_storage[0]).to_string();
		hex_storage.erase(hex_storage.begin());
	}
	cout << "spitting out binary \n";
	cout << binary << endl;
	//2. sift through the new string, taking 5 bytes at a time and storing them in fiveBinary
	int counter = 0;
	for (int i = 0; counter < binary.length(); i++)
	{
		holder = holder + binary[counter];
		counter++;
		holder = holder + binary[counter];
		counter++;
		holder = holder + binary[counter];
		counter++;
		holder = holder + binary[counter];
		counter++;
		holder = holder + binary[counter];
		counter++;
		//fiveBinary[i] = holder;
		fiveBinary.push_back(holder);
		holder = "";
	}
	cout << endl;
	for (int i = 0; i < fiveBinary.size(); i++)
	{
		cout << fiveBinary[i] << endl;
	}
	return;
}

void Enter_PIN::BDC_ConvertFour(vector<int> &hex_storage, vector<string> &fiveBinary, int BDC_Length)
{
	string binary = "";
	string holder = "";
	for (int counter = 0; counter < BDC_Length; counter++)
	{
		binary = binary + bitset<8>(hex_storage[0]).to_string();
		hex_storage.erase(hex_storage.begin());
	}
	cout << "spitting out binary \n";
	cout << binary << endl;
	//2. sift through the new string, taking 5 bytes at a time and storing them in fiveBinary
	int counter = 0;
	for (int i = 0; counter < binary.length(); i++)
	{
		holder = holder + binary[counter];
		counter++;
		holder = holder + binary[counter];
		counter++;
		holder = holder + binary[counter];
		counter++;
		holder = holder + binary[counter];
		counter++;
		//fiveBinary[i] = holder;
		fiveBinary.push_back(holder);
		holder = "";
	}
	cout << endl;
	for (int i = 0; i < fiveBinary.size(); i++)
	{
		cout << fiveBinary[i] << endl;
	}
	return;
}

void Enter_PIN::parseFASCN(vector<string>fiveBinary, string &_agencyCode, string &_systemCode, string &_credentialNumber, string &_CS, string &_ICI, string &_PI, string &_OC, string &_OI, string &_POA, string &_LRC, CSmartcard_UIDlg &main_dlg)
{
	//parse the FASCN 
	//1. take fiveBinary and run all 40 numbers through a switch statement that converts the number to something else
	//2. during the switch statement, take the converted numbers and store them all in a different string array
	//3. parse appropriately to fill each variable with the right numbers
	//4. this should now be 100% done. Print each one neatly to be sure, then work on second card (and email Mark)
	//5. if I don't know what cards are PIV and which are PIV-I after a little research, ask Mark in the same email
	string FASCN = "";//this will hold entire integer FASCN when done calling switchReturn 40 times
	string _holder;
	for (int i = 0; i < fiveBinary.size(); i++)//fill FASCN with full FASCN int value
	{
		_holder = fiveBinary[i];
		FASCN = FASCN + switchReturn(_holder);
	}
	cout << "FASCN: " << FASCN << endl;
	//time to parse FASCN
	_agencyCode += FASCN[1];
	_agencyCode += FASCN[2];
	_agencyCode += FASCN[3];
	_agencyCode += FASCN[4];
	_systemCode += FASCN[6];
	_systemCode += FASCN[7];
	_systemCode += FASCN[8];
	_systemCode += FASCN[9];
	_credentialNumber += FASCN[11];
	_credentialNumber += FASCN[12];
	_credentialNumber += FASCN[13];
	_credentialNumber += FASCN[14];
	_credentialNumber += FASCN[15];
	_credentialNumber += FASCN[16];
	_CS = FASCN[18];
	_ICI = FASCN[20];
	_PI += FASCN[22];
	_PI += FASCN[23];
	_PI += FASCN[24];
	_PI += FASCN[25];
	_PI += FASCN[26];
	_PI += FASCN[27];
	_PI += FASCN[28];
	_PI += FASCN[29];
	_PI += FASCN[30];
	_PI += FASCN[31];
	_OC = FASCN[32];
	_OI += FASCN[33];
	_OI += FASCN[34];
	_OI += FASCN[35];
	_OI += FASCN[36];
	_POA = FASCN[37];
	_LRC = FASCN[39];

	//removing from fascn: '[' ']' and num after ']'
	size_t l_brace, r_brace, backNum;//remove num AFTER back brace (if exists)... then back brace
	l_brace = FASCN.find('[');
	if (l_brace != std::string::npos)
		FASCN.erase(l_brace, 1);

	if (FASCN.at((FASCN.size() - 1) == ']'))
		FASCN.pop_back();

	r_brace = FASCN.find(']');
	if (r_brace != std::string::npos)
		FASCN.erase(r_brace, 1);
	
	
	main_dlg.FASCN = FASCN.c_str();
	return;

}

string Enter_PIN::parseOI(vector<string>fiveBinary)
{
	string OI = "";
	string _holder = "";
	for (int i = 0; i < fiveBinary.size(); i++)//fill FASCN with full FASCN int value
	{
		_holder = fiveBinary[i];
		OI = OI + switchReturn(_holder);
	}
	return OI;
}

string Enter_PIN::parseOIFour(vector<string>fiveBinary)
{
	string OI = "";
	string _holder = "";
	for (int i = 0; i < fiveBinary.size(); i++)//fill FASCN with full FASCN int value
	{
		_holder = fiveBinary[i];
		OI = OI + switchReturnFour(_holder);
	}
	return OI;
}

string Enter_PIN::parseGUID(vector<string>fiveBinary)
{
	string OI = "";
	string _holder = "";
	for (int i = 0; i < fiveBinary.size(); i++)//fill FASCN with full FASCN int value
	{
		_holder = fiveBinary[i];
		OI = OI + switchReturnDecimal(_holder);
	}
	return OI;

}

string Enter_PIN::parsePrinted(vector<int> &hex_storage, int printedLength)
{
	string _result = "";
	for (int i = 0; i < printedLength; i++)
	{
		_result = _result + (char)hex_storage[0];
		hex_storage.erase(hex_storage.begin());
	}
	return _result;
}

string Enter_PIN::switchReturn(string binaryInt)
{
	//parseFASCN sends 1 string from array to this function
	//this function turns it into a number and sends it back
	//switch needs to be enumerated, so for now just use nested if
	if (binaryInt == "00001")
	{
		return "0";
	}
	else if (binaryInt == "10000")
	{
		return "1";
	}
	else if (binaryInt == "01000")
	{
		return "2";
	}
	else if (binaryInt == "11001")
	{
		return "3";
	}
	else if (binaryInt == "00100")
	{
		return "4";
	}
	else if (binaryInt == "10101")
	{
		return "5";
	}
	else if (binaryInt == "01101")
	{
		return "6";
	}
	else if (binaryInt == "11100")
	{
		return "7";
	}
	else if (binaryInt == "00010")
	{
		return "8";
	}
	else if (binaryInt == "10011")
	{
		return "9";
	}
	else if (binaryInt == "11010")
	{
		return "[";
	}
	else if (binaryInt == "10110")
	{
		return "-";
	}
	else if (binaryInt == "11111")
	{
		return "]";
	}
	else//default case if it fits none of these
	{
		cout << "something went wrong in switchReturn" << endl;
		return "";
	}
	return "";
}

string Enter_PIN::switchReturnFour(string binaryInt)
{
	//parseFASCN sends 1 string from array to this function
	//this function turns it into a number and sends it back
	//switch needs to be enumerated, so for now just use nested if
	if (binaryInt == "0000")
	{
		return "0";
	}
	else if (binaryInt == "1000")
	{
		return "1";
	}
	else if (binaryInt == "0100")
	{
		return "2";
	}
	else if (binaryInt == "1100")
	{
		return "3";
	}
	else if (binaryInt == "0010")
	{
		return "4";
	}
	else if (binaryInt == "1010")
	{
		return "5";
	}
	else if (binaryInt == "0110")
	{
		return "6";
	}
	else if (binaryInt == "1110")
	{
		return "7";
	}
	else if (binaryInt == "0001")
	{
		return "8";
	}
	else if (binaryInt == "1001")
	{
		return "9";
	}
	else if (binaryInt == "1101")
	{
		return "[";
	}
	else if (binaryInt == "1011")
	{
		return "-";
	}
	else if (binaryInt == "1111")
	{
		return "]";
	}
	else//default case if it fits none of these
	{
		cout << "something went wrong in switchReturn" << endl;
		return "";
	}
	return "";
}

string Enter_PIN::switchReturnDecimal(string binaryInt)
{
	//convert 4 bit binary values sent to hex and return the hex value
	if (binaryInt == "0000")
	{
		return "0";
	}
	else if (binaryInt == "0001")
	{
		return "1";
	}
	else if (binaryInt == "0010")
	{
		return "2";
	}
	else if (binaryInt == "0011")
	{
		return "3";
	}
	else if (binaryInt == "0100")
	{
		return "4";
	}
	else if (binaryInt == "0101")
	{
		return "5";
	}
	else if (binaryInt == "0110")
	{
		return "6";
	}
	else if (binaryInt == "0111")
	{
		return "7";
	}
	else if (binaryInt == "1000")
	{
		return "8";
	}
	else if (binaryInt == "1001")
	{
		return "9";
	}
	else if (binaryInt == "1010")
	{
		return "A";
	}
	else if (binaryInt == "1011")
	{
		return "B";
	}
	else if (binaryInt == "1100")
	{
		return "C";
	}
	else if (binaryInt == "1101")
	{
		return "D";
	}
	else if (binaryInt == "1110")
	{
		return "E";
	}
	else if (binaryInt == "1111")
	{
		return "F";
	}
	else//default case if it fits none of these
	{
		cout << "something went wrong in switchReturn" << endl;
		return "";
	}
	return "";
}

void Enter_PIN::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//CDialogEx::OnCancel();
	SCardReleaseContext(hContext);
	CSmartcard_UIDlg dlg;
	int nRet = 5;
	Enter_PIN::EndDialog(nRet);
	dlg.DoModal();
}

void Enter_PIN::DisplayResourcePINMessageBox(int _pinCode2)
{
	if (_pinCode2 == 201)
	{
		_pinCode2 = 9;
	}
	else if (_pinCode2 == 200)
	{
		_pinCode2 = 8;
	}
	else if (_pinCode2 == 199)
	{
		_pinCode2 = 7;
	}
	else if (_pinCode2 == 198)
	{
		_pinCode2 = 6;
	}
	else if (_pinCode2 == 197)
	{
		_pinCode2 = 5;
	}
	else if (_pinCode2 == 196)
	{
		_pinCode2 = 4;
	}
	else if (_pinCode2 == 195)
	{
		_pinCode2 = 3;
	}
	else if (_pinCode2 == 194)
	{
		_pinCode2 = 2;
	}
	else if (_pinCode2 == 193)
	{
		_pinCode2 = 1;
	}
	else if (_pinCode2 == 192)
	{
		_pinCode2 = 0;
	}
	else
	{
		_pinCode2 = 10;
	}
	CString msg;
	msg.Format(_T("Invalid PIN\n%d Attempts Remain"), _pinCode2);


	int msgboxID = MessageBox(
		msg,
		(LPCWSTR)L"Try Again",
		MB_ICONWARNING | MB_OK | MB_DEFBUTTON1
	);

	switch (msgboxID)
	{
	case IDOK:
		// TODO: add code
		break;
	}
	GotoDlgCtrl(GetDlgItem(enterPINEdit));
	return;
}

wchar_t * Enter_PIN::GetWCharFromString(std::string inString)
{
	std::wstring wsinString(inString.begin(), inString.end());
	unsigned int maxlength = inString.length() + 1;
	int len = wcsnlen_s(wsinString.c_str(), maxlength);
	wchar_t* outString = new wchar_t[len + 1];

	wcscpy_s(outString, len + 1, wsinString.c_str());

	return outString;
}