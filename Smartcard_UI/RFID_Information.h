#pragma once
#include "Smartcard_UI.h"

#define TEMPLATE_SIZE 1024
// RFID_Information dialog

class RFID_Information : public CDialogEx
{
	DECLARE_DYNAMIC(RFID_Information)

public:
	RFID_Information(CWnd* pParent = NULL);   // standard constructor
	virtual ~RFID_Information();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL RFID_Information::OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	SCARDHANDLE hCardHandle;
	LPCSCARD_IO_REQUEST pioSendPci;
	std::string RFID_Information::pseudoFascn();
	void RFID_Information::thumbPrint(unsigned char aTemplate[TEMPLATE_SIZE]);
};
