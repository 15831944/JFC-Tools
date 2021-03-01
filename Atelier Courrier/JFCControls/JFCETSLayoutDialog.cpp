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

// la fonction pour mettre � jour les vues
VOID JFCETSLayoutDialog::UpdateAllViews(const JFCUpdate & Message)
{
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour configurer les modes de d�placement
/////////////////////////////////////////////////////////////////////////////

void JFCETSLayoutDialog::SetMode(CWnd* pWnd, LONG HMode, LONG VMode)
{
	// on teste si on a quelque chose � faire
	if (pWnd != NULL)
	{
		Link Infos;

		// on v�rifie la validit� de la fen�tre
		ASSERT(::IsWindow(pWnd->m_hWnd));

		// on recherche l'�l�ment
		BOOL trouve = m_Items.Lookup(pWnd, Infos);

		// on teste si on doit ins�rer un nouvel �l�ment
		if (trouve == FALSE)
		{
			RECT rect;

			// on r�cup�re le rectangle de la fen�tre
			pWnd->GetWindowRect(&rect);

			// on convertit le rectangle dans le rep�re de la bo�te
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
			// on met � jour les indicateurs
			Infos.m_HMode = HMode;
			Infos.m_VMode = VMode;
		}

		// on ins�re ou on modifie l'�l�ment
		m_Items.SetAt(pWnd, Infos);
	}
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour configurer la gestion des ascenseurs
/////////////////////////////////////////////////////////////////////////////

void JFCETSLayoutDialog::SetScrollBar(CWnd* pWnd, CWnd* pHScroll, CWnd* pVScroll)
{
	// on teste si on a quelque chose � faire
	if (pWnd != NULL)
	{
		Link Infos;

		// on v�rifie la validit� de la fen�tre
		ASSERT(::IsWindow(pWnd->m_hWnd));

		// on recherche l'�l�ment
		BOOL trouve = m_Items.Lookup(pWnd, Infos);

		// on teste si on doit ins�rer un nouvel �l�ment
		if (trouve == FALSE)
		{
			RECT rect;

			// on r�cup�re le rectangle de la fen�tre
			pWnd->GetWindowRect(&rect);

			// on convertit le rectangle dans le rep�re de la bo�te
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
			// on met � jour les indicateurs
			Infos.m_pHScroll = pHScroll;
			Infos.m_pVScroll = pVScroll;
		}

		// on ins�re ou on modifie l'�l�ment
		m_Items.SetAt(pWnd, Infos);
	}
}