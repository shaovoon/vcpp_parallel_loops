// ControlsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CPUBloom.h"
#include "ControlsDlg.h"
#include "afxdialogex.h"


// CControlsDlg dialog

IMPLEMENT_DYNAMIC(CControlsDlg, CDialogEx)

CControlsDlg::CControlsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CControlsDlg::IDD, pParent),
	m_pWndTarget(NULL)
{

}

CControlsDlg::~CControlsDlg()
{
}

void CControlsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CControlsDlg, CDialogEx)
END_MESSAGE_MAP()


// CControlsDlg message handlers

void CControlsDlg::SetTarget(CWnd* pWnd)
{
	m_pWndTarget = pWnd;
}


BOOL CControlsDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if(m_pWndTarget)
	{           
		return m_pWndTarget->SendMessage(WM_COMMAND, wParam, lParam);
	}
	else
	{
		return CDialogEx::OnCommand(wParam, lParam);
	}

	//return CDialogEx::OnCommand(wParam, lParam);
}
