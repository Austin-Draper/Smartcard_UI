#pragma once


// dbinfo_file dialog

class dbinfo_file : public CDialogEx
{
	DECLARE_DYNAMIC(dbinfo_file)

public:
	dbinfo_file(CWnd* pParent = NULL);   // standard constructor
	virtual ~dbinfo_file();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
