#include "StdAfx.h"
#include ".\analyseur.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAnalyseur::CAnalyseur(void)
{
	if(! m_DlgAnalyseur.Create(CDlgAnalyseur::IDD))   //Create failed.
		AfxMessageBox("Error creating Analyseur");
}

CAnalyseur::~CAnalyseur(void)
{
	m_DlgAnalyseur.DestroyWindow();
}

void CAnalyseur::Show(CWnd *pWnd)
{
	m_DlgAnalyseur.ShowWindow(SW_RESTORE);
	m_DlgAnalyseur.ShowWindow(SW_SHOW);
	m_DlgAnalyseur.BringWindowToTop();
}

void CAnalyseur::Hide()
{
	m_DlgAnalyseur.ShowWindow(SW_HIDE);
}

void CAnalyseur::Open(CString path)
{
	m_DlgAnalyseur.OpenFile(path);
}

BOOL CAnalyseur::GetState()
{
	return (m_DlgAnalyseur.IsWindowVisible() && !m_DlgAnalyseur.IsIconic());
}

BOOL CAnalyseur::CanClose()
{
	return (m_DlgAnalyseur.CanClose());
}

void CAnalyseur::Close()
{
	m_DlgAnalyseur.Close();
}

//Permet de transmettre un message provenant d'un module
BOOL CAnalyseur::ForwardMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	return m_DlgAnalyseur.PostMessage(message, wParam , lParam);
}