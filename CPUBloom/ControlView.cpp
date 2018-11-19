// ControlView.cpp : implementation file
//

#include "stdafx.h"
#include "CPUBloom.h"
#include "ControlView.h"
#include "CPUBloomDoc.h"

// CControlView

IMPLEMENT_DYNCREATE(CControlView, CFormView)

CControlView::CControlView()
	: CFormView(CControlView::IDD),
	m_pWndTarget(NULL)
	, m_nLoopMethodSelected(0)
{
}

CControlView::~CControlView()
{
}

void CControlView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_BLOOM_INT, m_sliderBloomInt);
	DDX_Control(pDX, IDC_SLIDER_BLOOM_SAT, m_sliderBloomSat);
	DDX_Control(pDX, IDC_SLIDER_BASE_INT, m_sliderBaseInt);
	DDX_Control(pDX, IDC_SLIDER_BASE_SAT, m_sliderBaseSat);
	DDX_Control(pDX, IDC_STATIC_BLOOM_INT_RES, m_staticBloomIntResult);
	DDX_Control(pDX, IDC_STATIC_BLOOM_SAT_RES, m_staticBloomSatResult);
	DDX_Control(pDX, IDC_STATIC_BASE_INT_RES, m_staticBaseIntResult);
	DDX_Control(pDX, IDC_STATIC_BASE_SAT_RES, m_staticBaseSatResult);
	DDX_Control(pDX, IDC_SLIDER_PIC_ZOOM_OUT, m_sliderPicZoomOut);
	DDX_Control(pDX, IDC_STATIC_PIC_ZOOM_OUT_RES, m_staticPicZoomOutResult);
	DDX_Control(pDX, IDC_STATIC_SERIAL_RES, m_staticSerialBenResult);
	DDX_Control(pDX, IDC_STATIC_OPENMP_RES, m_staticOpenMPBenResult);
	DDX_Control(pDX, IDC_STATIC_PPL_RES, m_staticPPLBenResult);
	DDX_Control(pDX, IDC_STATIC_AUTOP_RES, m_staticAutoPBenResult);
	DDX_Radio(pDX, IDC_RDO_SERIAL, m_nLoopMethodSelected);
	DDX_Control(pDX, IDC_STATIC_PFOREACH_RES, m_staticPForEachBenResult);
}

BEGIN_MESSAGE_MAP(CControlView, CFormView)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_BLOOM_INT, &CControlView::OnSliderBloomIntReleasedCapture)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_BLOOM_SAT, &CControlView::OnSliderBloomSatReleasedCapture)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_BASE_INT, &CControlView::OnSliderBaseIntReleasedCapture)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_BASE_SAT, &CControlView::OnSliderBaseSatReleasedCapture)
	ON_WM_HSCROLL()
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_PIC_ZOOM_OUT, &CControlView::OnSliderPicZoomOutReleasedCapture)
	ON_COMMAND(IDC_RDO_OPENMP, &CControlView::OnRdoOpenMP)
	ON_COMMAND(IDC_RDO_PPL, &CControlView::OnRdoPPL)
	ON_COMMAND(IDC_RDO_AUTOP, &CControlView::OnRdoAutoP)
	ON_BN_CLICKED(IDC_RDO_SERIAL, &CControlView::OnClickedRdoSerial)
	ON_BN_CLICKED(IDC_BTN_RESET, &CControlView::OnBnClickedBtnReset)
	ON_BN_CLICKED(IDC_RDO_PFOREACH, &CControlView::OnBnClickedRdoPforeach)
END_MESSAGE_MAP()


// CControlView diagnostics

#ifdef _DEBUG
void CControlView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CControlView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CControlView message handlers
void CControlView::SetTarget(CWnd* pWnd)
{
	m_pWndTarget = pWnd;
}

void CControlView::Init()
{
	m_sliderBloomInt.SetRange(0, BLOOM_INT_MAX * 100);
	m_sliderBloomInt.SetPos(100);
	m_sliderBloomInt.SetBuddy(&m_staticBloomIntResult,0);
	m_sliderBloomSat.SetRange(0, BLOOM_SAT_MAX * 100);
	m_sliderBloomSat.SetPos(100);
	m_sliderBloomSat.SetBuddy(&m_staticBloomSatResult,0);
	m_sliderBaseInt.SetRange(0, BASE_INT_MAX * 100);
	m_sliderBaseInt.SetPos(100);
	m_sliderBaseInt.SetBuddy(&m_staticBaseIntResult,0);
	m_sliderBaseSat.SetRange(0, BASE_SAT_MAX * 100);
	m_sliderBaseSat.SetPos(100);
	m_sliderBaseSat.SetBuddy(&m_staticBaseSatResult,0);
	m_sliderPicZoomOut.SetRange(0, PIC_ZOOM_OUT_MAX * 100);
	m_sliderPicZoomOut.SetPos(0);
	m_sliderPicZoomOut.SetBuddy(&m_staticPicZoomOutResult,0);

}

BOOL CControlView::CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, LPVOID lpParam)
{
	// TODO: Add your specialized code here and/or call the base class

	return CFormView::CreateEx(dwExStyle, lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, lpParam);
}


void CControlView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	Init();
}


void CControlView::OnSliderBloomIntReleasedCapture(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	CString str;
	str.Format(L"%.2f", m_sliderBloomInt.GetPos() / 100.0f);
	m_staticBloomIntResult.SetWindowTextW(str);

	CCPUBloomDoc* pDoc = (CCPUBloomDoc*)(GetDocument());
	pDoc->SetBloomIntensity(m_sliderBloomInt.GetPos()/100.0f);
}


void CControlView::OnSliderBloomSatReleasedCapture(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	CString str;
	str.Format(L"%.2f", m_sliderBloomSat.GetPos() / 100.0f);
	m_staticBloomSatResult.SetWindowTextW(str);

	CCPUBloomDoc* pDoc = (CCPUBloomDoc*)(GetDocument());
	pDoc->SetBloomSaturation(m_sliderBloomSat.GetPos()/100.0f);
}


void CControlView::OnSliderBaseIntReleasedCapture(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	CString str;
	str.Format(L"%.2f", m_sliderBaseInt.GetPos() / 100.0f);
	m_staticBaseIntResult.SetWindowTextW(str);

	CCPUBloomDoc* pDoc = (CCPUBloomDoc*)(GetDocument());
	pDoc->SetBaseIntensity(m_sliderBaseInt.GetPos()/100.0f);
}


void CControlView::OnSliderBaseSatReleasedCapture(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	CString str;
	str.Format(L"%.2f", m_sliderBaseSat.GetPos() / 100.0f);
	m_staticBaseSatResult.SetWindowTextW(str);

	CCPUBloomDoc* pDoc = (CCPUBloomDoc*)(GetDocument());
	pDoc->SetBaseSaturation(m_sliderBaseSat.GetPos()/100.0f);
}


void CControlView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	CSliderCtrl *pCtrl =  (CSliderCtrl *)(pScrollBar);
	if(pCtrl==&m_sliderBloomInt)
	{
		CString str;
		str.Format(L"%.2f", m_sliderBloomInt.GetPos() / 100.0f);
		m_staticBloomIntResult.SetWindowTextW(str);

		CCPUBloomDoc* pDoc = (CCPUBloomDoc*)(GetDocument());
		pDoc->SetBloomIntensity(m_sliderBloomInt.GetPos()/100.0f);
	}
	if(pCtrl==&m_sliderBloomSat)
	{
		CString str;
		str.Format(L"%.2f", m_sliderBloomSat.GetPos() / 100.0f);
		m_staticBloomSatResult.SetWindowTextW(str);

		CCPUBloomDoc* pDoc = (CCPUBloomDoc*)(GetDocument());
		pDoc->SetBloomSaturation(m_sliderBloomSat.GetPos()/100.0f);
	}
	if(pCtrl==&m_sliderBaseInt)
	{
		CString str;
		str.Format(L"%.2f", m_sliderBaseInt.GetPos() / 100.0f);
		m_staticBaseIntResult.SetWindowTextW(str);

		CCPUBloomDoc* pDoc = (CCPUBloomDoc*)(GetDocument());
		pDoc->SetBaseIntensity(m_sliderBaseInt.GetPos()/100.0f);
	}
	if(pCtrl==&m_sliderBaseSat)
	{
		CString str;
		str.Format(L"%.2f", m_sliderBaseSat.GetPos() / 100.0f);
		m_staticBaseSatResult.SetWindowTextW(str);

		CCPUBloomDoc* pDoc = (CCPUBloomDoc*)(GetDocument());
		pDoc->SetBaseSaturation(m_sliderBaseSat.GetPos()/100.0f);
	}
	if(pCtrl==&m_sliderPicZoomOut)
	{
		CString str;
		str.Format(L"%.2f", m_sliderPicZoomOut.GetPos() / 100.0f);
		m_staticPicZoomOutResult.SetWindowTextW(str);

		CCPUBloomDoc* pDoc = (CCPUBloomDoc*)(GetDocument());
		pDoc->SetPicZoomOut(m_sliderPicZoomOut.GetPos()/100.0f);
	}
	CFormView::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CControlView::OnSliderPicZoomOutReleasedCapture(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	CString str;
	str.Format(L"%.2f", m_sliderPicZoomOut.GetPos() / 100.0f);
	m_staticPicZoomOutResult.SetWindowTextW(str);

	CCPUBloomDoc* pDoc = (CCPUBloomDoc*)(GetDocument());
	pDoc->SetPicZoomOut(m_sliderPicZoomOut.GetPos()/100.0f);
}

void CControlView::SetSerialBenResult(int val)
{
	CString str;
	str.Format(L"%d", val);
	m_staticSerialBenResult.SetWindowTextW(str+L"ms");
}

void CControlView::SetOpenMPBenResult(int val)
{
	CString str;
	str.Format(L"%d", val);
	m_staticOpenMPBenResult.SetWindowTextW(str+L"ms");
}

void CControlView::SetPPLBenResult(int val)
{
	CString str;
	str.Format(L"%d", val);
	m_staticPPLBenResult.SetWindowTextW(str+L"ms");
}

void CControlView::SetAutoPBenResult(int val)
{
	CString str;
	str.Format(L"%d", val);
	m_staticAutoPBenResult.SetWindowTextW(str+L"ms");
}

void CControlView::SetPForEachBenResult(int val)
{
	CString str;
	str.Format(L"%d", val);
	m_staticPForEachBenResult.SetWindowTextW(str + L"ms");
}

//void CControlView::OnClickedRdoSerial()
//{
//	UpdateData();
//
//	CCPUBloomDoc* pDoc = (CCPUBloomDoc*)(GetDocument());
//	pDoc->SetLoopMethod(m_nLoopMethodSelected);
//}


void CControlView::OnRdoOpenMP()
{
	UpdateData();
	
	CCPUBloomDoc* pDoc = (CCPUBloomDoc*)(GetDocument());
	pDoc->SetLoopMethod(m_nLoopMethodSelected);
}


void CControlView::OnRdoPPL()
{
	UpdateData();

	CCPUBloomDoc* pDoc = (CCPUBloomDoc*)(GetDocument());
	pDoc->SetLoopMethod(m_nLoopMethodSelected);
}


void CControlView::OnRdoAutoP()
{
	UpdateData();

	CCPUBloomDoc* pDoc = (CCPUBloomDoc*)(GetDocument());
	pDoc->SetLoopMethod(m_nLoopMethodSelected);
}


void CControlView::OnClickedRdoSerial()
{
	UpdateData();

	CCPUBloomDoc* pDoc = (CCPUBloomDoc*)(GetDocument());
	pDoc->SetLoopMethod(m_nLoopMethodSelected);
}

void CControlView::OnBnClickedRdoPforeach()
{
	UpdateData();

	CCPUBloomDoc* pDoc = (CCPUBloomDoc*)(GetDocument());
	pDoc->SetLoopMethod(m_nLoopMethodSelected);
}

void CControlView::OnBnClickedBtnReset()
{
	CCPUBloomDoc* pDoc = (CCPUBloomDoc*)(GetDocument());
	pDoc->BeginReset();

	m_sliderBloomInt.SetPos(100);
	m_sliderBloomSat.SetPos(100);
	m_sliderBaseInt.SetPos(100);
	m_sliderBaseSat.SetPos(100);
	m_sliderPicZoomOut.SetPos(0);
	m_nLoopMethodSelected = 0;
	UpdateData(FALSE);

	m_staticSerialBenResult.SetWindowTextW(L"NA");
	m_staticOpenMPBenResult.SetWindowTextW(L"NA");
	m_staticPPLBenResult.SetWindowTextW(L"NA");
	m_staticAutoPBenResult.SetWindowTextW(L"NA");

	m_staticBaseIntResult.SetWindowTextW(L"1.00");
	m_staticBaseSatResult.SetWindowTextW(L"1.00");
	m_staticBloomIntResult.SetWindowTextW(L"1.00");
	m_staticBloomSatResult.SetWindowTextW(L"1.00");
	m_staticPicZoomOutResult.SetWindowTextW(L"1.00");

	pDoc->SetBaseIntensity(1.0f);
	pDoc->SetBaseSaturation(1.0f);
	pDoc->SetBloomIntensity(1.0f);
	pDoc->SetBloomSaturation(1.0f);
	pDoc->SetLoopMethod(0);
	pDoc->SetPicZoomOut(0.0);

	pDoc->EndReset();

}
