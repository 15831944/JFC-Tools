#include "StdAfx.h"
#include ".\advision.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Constructeur
CAdvision::CAdvision(void)
{
	if(! m_DlgAdvision.Create(CDlgAdvision::IDD))   //Create failed.
		AfxMessageBox("Error creating Advision");
}

// Destructeur
CAdvision::~CAdvision(void)
{
	m_DlgAdvision.DestroyWindow();
}

void CAdvision::Show(CWnd *pWnd)
{
	m_DlgAdvision.ShowWindow(SW_RESTORE);
	m_DlgAdvision.ShowWindow(SW_SHOW);
	m_DlgAdvision.BringWindowToTop();
}

void CAdvision::Hide()
{
	m_DlgAdvision.ShowWindow(SW_HIDE);
}

void CAdvision::Open(CString path)
{
	m_DlgAdvision.OpenFile(path);
}

BOOL CAdvision::GetState()
{
	return (m_DlgAdvision.IsWindowVisible() && !m_DlgAdvision.IsIconic());
}

BOOL CAdvision::CanClose()
{
	return (m_DlgAdvision.CanClose());
}

void CAdvision::Close()
{
	m_DlgAdvision.Close();
}

//Permet de transmettre un message provenant d'un module
BOOL CAdvision::ForwardMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	return m_DlgAdvision.PostMessage(message, wParam , lParam);
}
