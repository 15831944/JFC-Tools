#include "StdAfx.h"
#include ".\jfcetslayoutdialog.h"

/////////////////////////////////////////////////////////////////////////////
// les constructeurs
/////////////////////////////////////////////////////////////////////////////
 
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

JFCETSLayoutDialog::JFCETSLayoutDialog(UINT nIDTemplate, CWnd* pParent) : ETSLayoutDialog(nIDTemplate, pParent)
{
}

JFCETSLayoutDialog::~JFCETSLayoutDialog()
{
}

// la fonction pour mettre à jour les vues
VOID JFCETSLayoutDialog::UpdateAllViews(const JFCUpdate & Message)
{
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour configurer les modes de déplacement
/////////////////////////////////////////////////////////////////////////////

void JFCETSLayoutDialog::SetMode(CWnd* pWnd, LONG HMode, LONG VMode)
{
	// on teste si on a quelque chose à faire
	if (pWnd != NULL)
	{
		Link Infos;

		// on vérifie la validité de la fenêtre
		ASSERT(::IsWindow(pWnd->m_hWnd));

		// on recherche l'élément
		BOOL trouve = m_Items.Lookup(pWnd, Infos);

		// on teste si on doit insérer un nouvel élément
		if (trouve == FALSE)
		{
			RECT rect;

			// on récupère le rectangle de la fenêtre
			pWnd->GetWindowRect(&rect);

			// on convertit le rectangle dans le repère de la boîte
			this->ScreenToClient(&rect);

			// on initialise les infos
			Infos.m_HPos     = rect.left;
			Infos.m_VPos     = rect.top;
			Infos.m_HSize    = rect.right  - rect.left;
			Infos.m_VSize    = rect.bottom - rect.top;
			Infos.m_HMode    = HMode;
			Infos.m_VMode    = VMode;
			Infos.m_pHScroll = NULL; 
			Infos.m_pVScroll = NULL;
		}
		else
		{
			// on met à jour les indicateurs
			Infos.m_HMode = HMode;
			Infos.m_VMode = VMode;
		}

		// on insère ou on modifie l'élément
		m_Items.SetAt(pWnd, Infos);
	}
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour configurer la gestion des ascenseurs
/////////////////////////////////////////////////////////////////////////////

void JFCETSLayoutDialog::SetScrollBar(CWnd* pWnd, CWnd* pHScroll, CWnd* pVScroll)
{
	// on teste si on a quelque chose à faire
	if (pWnd != NULL)
	{
		Link Infos;

		// on vérifie la validité de la fenêtre
		ASSERT(::IsWindow(pWnd->m_hWnd));

		// on recherche l'élément
		BOOL trouve = m_Items.Lookup(pWnd, Infos);

		// on teste si on doit insérer un nouvel élément
		if (trouve == FALSE)
		{
			RECT rect;

			// on récupère le rectangle de la fenêtre
			pWnd->GetWindowRect(&rect);

			// on convertit le rectangle dans le repère de la boîte
			this->ScreenToClient(&rect);

			// on initialise les infos
			Infos.m_HPos     = rect.left;
			Infos.m_VPos     = rect.top;
			Infos.m_HSize    = rect.right  - rect.left;
			Infos.m_VSize    = rect.bottom - rect.top;
			Infos.m_HMode    = MGER_NONE;
			Infos.m_VMode    = MGER_NONE;
			Infos.m_pHScroll = pHScroll;
			Infos.m_pVScroll = pVScroll;
		}
		else
		{
			// on met à jour les indicateurs
			Infos.m_pHScroll = pHScroll;
			Infos.m_pVScroll = pVScroll;
		}

		// on insère ou on modifie l'élément
		m_Items.SetAt(pWnd, Infos);
	}
}