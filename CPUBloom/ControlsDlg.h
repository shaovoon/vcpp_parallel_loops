#pragma once


// CControlsDlg dialog

class CControlsDlg : public CFormView
{
	DECLARE_DYNAMIC(CControlsDlg)

public:
	CControlsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CControlsDlg();

	void SetTarget(CWnd* pWnd);
private:
	CWnd* m_pWndTarget;
public:

// Dialog Data
	enum { IDD = IDD_DLG_CONTROLS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};
