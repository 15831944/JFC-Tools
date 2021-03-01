#include "StdAfx.h"
#include ".\repitems.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Constructeur r�pertoire des items (mode r�sultat + mode visualisation)
CRepItems::CRepItems(int ShowResults, int ShowMode, int ShowSite)
{
	// D�finition mode visualistaion dans boite de dialogue items
	m_DlgItems.SetShowMode(ShowMode); 

	// pour les tests en mode Simple 
	// m_DlgItems.SetShowMode(SHOWMODE_SIMPLE);

	// D�finition mode r�sultats dans boite de dialogue items
	m_DlgItems.SetShowResults(ShowResults);

	// D�finition mode s�lection avec ou sans site
	m_DlgItems.SetShowSite(ShowSite);

	if(! m_DlgItems.Create(CDlgItems::IDD))   //Create failed.
		AfxMessageBox("Error creating RepItems");
}

CRepItems::~CRepItems(void)
{
	m_DlgItems.DestroyWindow();
}

void CRepItems::Show(CWnd *pWnd)
{
	// Visualise fen�tre r�pertoire des items
	m_DlgItems.ShowWindow(SW_RESTORE);
	m_DlgItems.ShowWindow(SW_SHOW);
	m_DlgItems.BringWindowToTop();
	m_DlgItems.InitRepItem();
}

void CRepItems::Hide()
{
	m_DlgItems.ShowWindow(SW_HIDE);
}

BOOL CRepItems::GetState()
{
	return (m_DlgItems.IsWindowVisible() && !m_DlgItems.IsIconic());
}

BOOL CRepItems::CanClose()
{
	return (m_DlgItems.CanClose());
}

void CRepItems::Close()
{
	m_DlgItems.Close();
}

//Permet de transmettre un message provenant d'un module
BOOL CRepItems::ForwardMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	return m_DlgItems.PostMessage(message, wParam , lParam);
}