#include "StdAfx.h"
#include ".\choixsupport.h"
#include "ATPMessages.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constructeur
CChoixSupport::CChoixSupport(void)
{
	if(! m_DlgChoixSupport.Create(CDlgChoixSupport::IDD))   //Create failed.
		AfxMessageBox("Error creating ChoixSupport");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Destructeur
CChoixSupport::~CChoixSupport(void)
{
	m_DlgChoixSupport.DestroyWindow();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Crée ou rend visibleboite de dialogue Choix Support
void CChoixSupport::Show(CWnd *pWnd)
{
	m_DlgChoixSupport.ShowWindow(SW_RESTORE);
	m_DlgChoixSupport.ShowWindow(SW_SHOW);
	m_DlgChoixSupport.BringWindowToTop();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Cache boite de dialogue Choix Support
void CChoixSupport::Hide()
{
	m_DlgChoixSupport.ShowWindow(SW_HIDE);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Etat boite de dialogue Choix Support
BOOL CChoixSupport::GetState()
{
	return (m_DlgChoixSupport.IsWindowVisible() && !m_DlgChoixSupport.IsIconic());
}

//Permet de transmettre un message provenant d'un module
BOOL CChoixSupport::ForwardMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		// Mise à jour des tarifs dans répertoire supports
		case ATPMSG_TARIF_UPDATED:
			if (m_DlgChoixSupport.m_pActivePage->IsWindowVisible())
				m_DlgChoixSupport.m_DocChoixSupport.UpdateAllViews(UPDATE_TARIFS);
	}
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////////
// Ouverture d'un dossier analyse offre
void CChoixSupport::Open(CString path)
{
	m_DlgChoixSupport.OnLoadPlateau(path);
}

/////////////////////////////////////////////////////////////////////////////////
// Fermeture d'un dossier analyse offre
void CChoixSupport::Close()
{
	m_DlgChoixSupport.CanClose();
}

/////////////////////////////////////////////////////////////////////////////////
// Fermeture ou non de la boite de dialogue choix supports
JBool CChoixSupport::CanClose()
{
	return m_DlgChoixSupport.CanClose();
}
