#include "StdAfx.h"
#include ".\RepSupports.h"

CRepSupports::CRepSupports(void)
{
    if(! m_DlgSupports.Create(CRepSupportsDlg::IDD))   //Create failed.
        AfxMessageBox("Error creating RepSupport");
}

CRepSupports::~CRepSupports(void)
{

	CRegroupements::GetInstance()->Save();

	CTarifs::GetInstance()->SaveTarifsManuels();

	((JSRCPresse*)&JSRCPresse::GetInstance())->m_TBLFormats.SaveFormatSpe();

	m_DlgSupports.DestroyWindow();
}

void CRepSupports::Show(CWnd *pWnd)
{
	m_DlgSupports.ShowWindow(SW_RESTORE);
	m_DlgSupports.ShowWindow(SW_SHOW);
	m_DlgSupports.BringWindowToTop();
}

void CRepSupports::Hide()
{
	m_DlgSupports.ShowWindow(SW_HIDE);
}

BOOL CRepSupports::GetState()
{
	return (m_DlgSupports.IsWindowVisible() && !m_DlgSupports.IsIconic());
}

//Permet de transmettre un message provenant d'un module
BOOL CRepSupports::ForwardMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	return m_DlgSupports.PostMessage(message, wParam , lParam);
}

