#pragma once
#include "afxcmn.h"
#include "afxwin.h"



// CControlView form view

class CControlView : public CFormView
{
	DECLARE_DYNCREATE(CControlView)

protected:
	CControlView();           // protected constructor used by dynamic creation
	virtual ~CControlView();
public:
	void SetTarget(CWnd* pWnd);
	void Init();

	void SetSerialBenResult(int val);
	void SetOpenMPBenResult(int val);
	void SetPPLBenResult(int val);
	void SetAutoPBenResult(int val);
	void SetPForEachBenResult(int val);

	
protected:
	CWnd* m_pWndTarget;

public:
	enum { IDD = IDD_DLG_CONTROLS };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_sliderBloomInt;
	CSliderCtrl m_sliderBloomSat;
	CSliderCtrl m_sliderBaseInt;
	CSliderCtrl m_sliderBaseSat;
	virtual BOOL CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, LPVOID lpParam = NULL);
	virtual void OnInitialUpdate();
	afx_msg void OnSliderBloomIntReleasedCapture(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSliderBloomSatReleasedCapture(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSliderBaseIntReleasedCapture(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSliderBaseSatReleasedCapture(NMHDR *pNMHDR, LRESULT *pResult);
	CStatic m_staticBloomIntResult;
	CStatic m_staticBloomSatResult;
	CStatic m_staticBaseIntResult;
	CStatic m_staticBaseSatResult;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CSliderCtrl m_sliderPicZoomOut;
	CStatic m_staticPicZoomOutResult;
	afx_msg void OnSliderPicZoomOutReleasedCapture(NMHDR *pNMHDR, LRESULT *pResult);
	CStatic m_staticSerialBenResult;
	CStatic m_staticOpenMPBenResult;
	CStatic m_staticPPLBenResult;
	CStatic m_staticAutoPBenResult;
	int m_nLoopMethodSelected;
	afx_msg void OnRdoOpenMP();
	afx_msg void OnRdoPPL();
	afx_msg void OnRdoAutoP();
	afx_msg void OnClickedRdoSerial();
	afx_msg void OnBnClickedBtnReset();
	CStatic m_staticPForEachBenResult;
	afx_msg void OnBnClickedRdoPforeach();
};


