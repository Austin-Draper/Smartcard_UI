// Enter_Card.cpp : implementation file
//

#include "stdafx.h"
#include "Smartcard_UI.h"
#include "Enter_Card.h"
#include "Enter_PIN.h"
#include "RFID_Information.h"
#include "afxdialogex.h"


// Enter_Card dialog

IMPLEMENT_DYNAMIC(Enter_Card, CDialogEx)

Enter_Card::Enter_Card(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

Enter_Card::~Enter_Card()
{
}

void Enter_Card::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Enter_Card, CDialogEx)
	ON_BN_CLICKED(IDOK, &Enter_Card::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &Enter_Card::OnBnClickedCancel)
END_MESSAGE_MAP()


// Enter_Card message handlers


void Enter_Card::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//CDialogEx::OnOK();
	//make sure card has connected to reader.
	//if true, move to next page
	//of false, popup message says "it failed, try again" close it with ok button
	//Declare variables
	SCARDHANDLE hCardHandle;
	DWORD dwReaders;
	LPSTR szReaders = NULL;
	LPCSCARD_IO_REQUEST pioSendPci;
	SCARDCONTEXT hContext;
	bool bRunning = true;
	LONG lReturn;
	LPSTR reader_name;
	//LPTSTR card_name;
	//std::vector<const char*> cards;
	vector<const char*> cards;
	//SCARD_READERSTATE rgscState[1];//1 is maximum smartcard readers. increase this if more necessary
	DWORD dwAP;//Later... change to a loop to try ALL readers stored in cards vector.
			   //Done Declaring variables

	LONG status = SCardEstablishContext(SCARD_SCOPE_SYSTEM, NULL, NULL, &hContext);//swapped USER with SYSTEM
	if (status != SCARD_S_SUCCESS) {
		//popup error message "could not connect to card reader"
		return;
	}
	//std::cout << "Established Context!" << std::endl;
	//Done establishing context with card reader

	//Displaying names of all registered card readers
	dwReaders = SCARD_AUTOALLOCATE;
	if (SCardListReadersA(hContext, NULL, (LPSTR)&szReaders, &dwReaders) == SCARD_S_SUCCESS) {
		LPSTR reader = szReaders;
		reader_name = reader;
		while (reader != NULL && *reader != '\0') {
			//std::cout << "Reader name: '" << reader << "'" << std::endl;
			cards.push_back(reader);//vector of ALL card readers saved for later use
			reader += strlen(reader) + 1;
		}
		//std::cout << "Reader name outside: '" << reader_name << "'" << std::endl;//delete after testing
		//Done displaying names of all registered card readers

		/*
		*Listing all card types the reader will recognize

		LPTSTR pmszCards = NULL;
		LPTSTR pCard;
		DWORD cch = SCARD_AUTOALLOCATE;

		lReturn = SCardListCards(NULL,
			NULL,
			NULL,
			NULL,
			(LPTSTR)&pmszCards,
			&cch);
		if (SCARD_S_SUCCESS != lReturn)
		{
			std::cout << "Failed SCardListCards\n";
			system("pause");
			exit(1);
		}

		pCard = pmszCards;
		while ('\0' != *pCard)
		{
			printf("%S\n", pCard);
			pCard = pCard + wcslen(pCard) + 1;
		}
		SCardFreeMemory(hContext, pmszCards);

		*Done Listing all card types the reader will recognize
		*/

		LPSCARD_READERSTATEA lpState = new SCARD_READERSTATEA[cards.size()];
		for (size_t n = 0; n < cards.size(); ++n) {
			memset(lpState + n, 0, sizeof(SCARD_READERSTATEA));
			lpState[n].szReader = cards[n];
		}

		do {
			status = SCardGetStatusChangeA(hContext, INFINITE, lpState, cards.size());
			//std::cout << "Status is: " + status << std::endl;
			switch (status)
			{
			case SCARD_S_SUCCESS:
			case SCARD_E_TIMEOUT:
				for (size_t n = 0; n < cards.size(); ++n) {
					if (lpState[n].dwEventState & SCARD_STATE_PRESENT) {
						//std::cout << "'" << lpState[n].szReader << "' present" << std::endl;
						//SCARDHANDLE hCardHandle;
						//DWORD dwAP;//change HID Global.... to variable name reader_name. Later... change to a loop to try ALL readers stored is cards vector.
						lReturn = (SCardConnectA(hContext, lpState[n].szReader,
							SCARD_SHARE_SHARED, SCARD_PROTOCOL_T1 | SCARD_PROTOCOL_T0, &hCardHandle, &dwAP));//if multiple readers... need to check each reader if one is connected... do this later
						if (SCARD_S_SUCCESS != lReturn)
						{
							//popup message: "failed to connect to card, try again"
							//std::cout << "Failed SCardConnect\n";
							//std::cout << "Error code is: " << lReturn << "\n";
							//system("pause");
							return;
						}
						//std::cout << "SCardConnect Successful!\n";
						bRunning = false;
						break;
					}
					else {
						//std::cout << "'" << lpState[n].szReader << "' not present" << std::endl;
					}
				}
				//popup window "No Card Present In Reader"
				if (bRunning == false)
				{
					break;
				}
				DisplayResourceNAMessageBox();
				SCardReleaseContext(hContext);
				return;
			default:
				//std::cout << "Other result: " << status << std::endl;
				break;
			}
		} while (bRunning);
		bRunning = true;

		switch (dwAP)
		{
		case SCARD_PROTOCOL_T0:
			//printf("Active protocol T0\n");
			pioSendPci = SCARD_PCI_T0;
			break;

		case SCARD_PROTOCOL_T1:
			//printf("Active protocol T1\n");
			pioSendPci = SCARD_PCI_T1;
			break;

		case SCARD_PROTOCOL_UNDEFINED:
		default:
			//printf("Active protocol unnegotiated or unknown\n");
			pioSendPci = SCARD_PCI_RAW;
			break;
		}
		//DO THE FF TRANSMIT TO FIGURE OUT WHETHER WE'RE CONTACT OR CONTACTLESS
		//IF FF SUCCESSFUL, RUN NEW MODAL
		//IF FF FAILS, RUN BELOW CODE
		
		/*
		int lReturn;
		int cbSendLength = 5;
		byte pbRecvBuffer[255];
		DWORD dwSize = 255;
		byte pbSendBuffer[] = { 0xFF, 0xCA, 0x00, 0x00, 0x00 };
		//SCARD_PCI_T1
		lReturn = SCardTransmit(hCardHandle, pioSendPci, pbSendBuffer, cbSendLength, NULL, pbRecvBuffer, &dwSize);//girls face needs T0... rest use T1
		if (lReturn == SCARD_S_SUCCESS) {
			std::cout << "Transmit Success!\n";
		}
		else
		{
			//standard error message popup that something failed. put user back on same page
			std::cout << "Transmit Failed!\n";
			std::cout << lReturn << std::endl;
			system("pause");
			exit(1);
		}

		int _pinCodeSize1 = dwSize - 2;
		int _pinCodeSize2 = dwSize - 1;
		int _pinCode1 = (int)pbRecvBuffer[_pinCodeSize1];
		int _pinCode2 = (int)pbRecvBuffer[_pinCodeSize2];
		if (_pinCode1 == 144)// RFID/Contactless card reader
		{
			//go to new contactless dialogs
			RFID_Information dlg;
			int nRet = 5;
			Enter_Card::EndDialog(nRet);//in future... hide dialog instead, make it re-appear if either of next 2 dialogs cancelled... THEN I can delete and remake if user goes through both other dialogs successfully
			dlg.hCardHandle = hCardHandle;
			dlg.pioSendPci = pioSendPci;
			dlg.DoModal();
		}
		*/

		//now run dlg.DoModal() and all that other stuff from before
		
		SCardFreeMemory(hContext, lpState);

		Enter_PIN dlg;
		int nRet = 6;
		Enter_Card::EndDialog(nRet);//in future... hide dialog instead, make it re-appear if either of next 2 dialogs cancelled... THEN I can delete and remake if user goes through both other dialogs successfully
		dlg.hCardHandle = hCardHandle;
		dlg.pioSendPci = pioSendPci;
		dlg.hContext = hContext;
		dlg.DoModal();
	}
}


void Enter_Card::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//CDialogEx::OnCancel();
	CSmartcard_UIDlg dlg;
	int nRet = 5;
	Enter_Card::EndDialog(nRet);
	dlg.DoModal();
}

void Enter_Card::DisplayResourceNAMessageBox()
{
	int msgboxID = MessageBox(
		(LPCWSTR)L"No Card Present In Reader",
		(LPCWSTR)L"Enter Card",
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

