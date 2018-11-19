
// CPUBloomView.cpp : implementation of the CCPUBloomView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "CPUBloom.h"
#endif

#include "CPUBloomDoc.h"
#include "CPUBloomView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCPUBloomView

IMPLEMENT_DYNCREATE(CCPUBloomView, CView)

BEGIN_MESSAGE_MAP(CCPUBloomView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CCPUBloomView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CCPUBloomView construction/destruction

CCPUBloomView::CCPUBloomView()
{
	// TODO: add construction code here

}

CCPUBloomView::~CCPUBloomView()
{
}

BOOL CCPUBloomView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CCPUBloomView drawing

void CCPUBloomView::OnDraw(CDC* /*pDC*/)
{
	CCPUBloomDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CCPUBloomView printing


void CCPUBloomView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CCPUBloomView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CCPUBloomView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CCPUBloomView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CCPUBloomView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CCPUBloomView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CCPUBloomView diagnostics

#ifdef _DEBUG
void CCPUBloomView::AssertValid() const
{
	CView::AssertValid();
}

void CCPUBloomView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCPUBloomDoc* CCPUBloomView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCPUBloomDoc)));
	return (CCPUBloomDoc*)m_pDocument;
}
#endif //_DEBUG


// CCPUBloomView message handlers
