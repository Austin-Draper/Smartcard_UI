
// Smartcard_UIDlg.h : header file
//

#pragma once


// CSmartcard_UIDlg dialog
class CSmartcard_UIDlg : public CDialogEx
{
// Construction
public:
	CSmartcard_UIDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SMARTCARD_UI_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedOk2();
	CString PIN;
	CString FASCN;
	CString _agencyCode;
	CString _systemCode;
	CString _credentialNumber;
	CString _CS;//Credential Series/Series Code
	CString _ICI;//Individual Credential Issue/Credential Issue Level
	CString _PI;//Person Identifier
	CString _OC;//Organizational Category (1,2,3, or 4)
	CString _OI;//Organizational Identifier 
	CString _POA;//Person/Organization Association Category 
	CString _LRC;//Longitudinal Redundancy Character
	CString _GUID;//Generated User ID
	CString f_name;
	CString l_name;
	CString m_initial;
	CString expirationDate;
	CString currentDate;
	CString cardType;
	string _CStringtoString(CString txtname);
	void SQLWork();
	afx_msg void OnBnClickedCancel();
	void DisplaySuccessMessageBox();
	void DisplayFailMessageBox();
	void DisplayDatabaseFailMessageBox();
};
