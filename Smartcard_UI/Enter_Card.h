#pragma once


// Enter_Card dialog

class Enter_Card : public CDialogEx
{
	DECLARE_DYNAMIC(Enter_Card)

public:
	Enter_Card(CWnd* pParent = NULL);   // standard constructor
	virtual ~Enter_Card();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	void DisplayResourceNAMessageBox();
};
