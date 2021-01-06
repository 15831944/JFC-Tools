// on inclut les définitions nécessaires
#include "stdafx.h"
#include "JFCViewportSelect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// le constructeur
/////////////////////////////////////////////////////////////////////////////

JFCViewportSelect::JFCViewportSelect()
{
	// on initialise l'indicateur
	m_FlagSel = FALSE;

	// on initialise la sélection
	m_HorzSel = m_VertSel = m_EtatSel = 0;
}

/////////////////////////////////////////////////////////////////////////////
// le destructeur
/////////////////////////////////////////////////////////////////////////////

JFCViewportSelect::~JFCViewportSelect()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(JFCViewportSelect, JFCViewport)
	//{{AFX_MSG_MAP(JFCViewportSelect)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// la fonction pour récupérer la sélection des éléments
/////////////////////////////////////////////////////////////////////////////

LONG JFCViewportSelect::OnGetItemSelection(LONG i, LONG j)
{
	// on renvoie la sélection de l'élément
	return (0);
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour sélectionner les éléments
/////////////////////////////////////////////////////////////////////////////

bool JFCViewportSelect::OnBeginSelecting(LONG & etat)
{
	// on arrête la sélection des éléments
	return (false);
}
 
void JFCViewportSelect::OnSelectItem(LONG i, LONG j, LONG etat)
{
	// on ne fait rien
}

void JFCViewportSelect::OnEndSelecting()
{
	// on ne fait rien
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour récupérer l'état des éléments
/////////////////////////////////////////////////////////////////////////////

LONG JFCViewportSelect::OnGetItemState(LONG i, LONG j)
{
	// on teste si on a une sélection en cours
	if ((m_FlagSel != FALSE) && (i == m_HorzSel) && (j == m_VertSel))
	{
		// on renvoie l'état de l'élément
		return (m_EtatSel);
	}
	else
	{
		// on renvoie l'état de l'élément
		return (this->OnGetItemSelection(i, j));
	}
}

/////////////////////////////////////////////////////////////////////////////
// JFCViewportSelect message handlers

void JFCViewportSelect::PreSubclassWindow() 
{
	// on initialise l'indicateur
	m_FlagSel = FALSE;

	// on initialise la sélection
	m_HorzSel = m_VertSel = m_EtatSel = 0;

	// on appelle le gestionnaire de base	
	this->JFCViewport::PreSubclassWindow();
}

void JFCViewportSelect::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// on teste si on peut lancer une sélection
	if ((m_FlagSel == FALSE) && (NULL == ::GetCapture()))
	{
		LONG i, j;

		// on récupère l'élément sous le curseur
		if (this->GetIdxAtPos(point.x, point.y, i, j))
		{
			// on récupère l'état de l'élément
			m_EtatSel = this->OnGetItemSelection(i, j);

			// on initialise la sélection des éléments
			if (this->OnBeginSelecting(m_EtatSel))
			{
				// on capture la souris
				::SetCapture(this->m_hWnd);

				// on teste si a capturé la souris
				if (this->m_hWnd == ::GetCapture())
				{
					// on modifie l'indicateur de sélection
					m_FlagSel = TRUE;

					// on mémorise le point de sélection
					m_HorzSel = i; m_VertSel = j;

					// on met à jour l'affichage
					this->InvalidateRect(NULL, FALSE);
				}
				else
				{
					// on annule la sélection
					this->OnEndSelecting();
				}
			}
		}
	}

	// on appelle le gestionnaire de base	
	this->JFCViewport::OnLButtonDown(nFlags, point);
}

void JFCViewportSelect::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// on teste si on a une sélection en cours
	if (m_FlagSel != FALSE)
	{
		// on libère la souris
		::ReleaseCapture();

		// on teste si la libération s'est bien effectuée
		if (this->m_hWnd != ::GetCapture())
		{
			// on sélectionne l'élément
			this->OnSelectItem(m_HorzSel, m_VertSel, m_EtatSel);

			// on réinitialise l'indicateur de sélection
			m_FlagSel = FALSE;

			// on termine la sélection des éléments
			this->OnEndSelecting();

			// on redessine la fenêtre
			this->InvalidateRect(NULL, FALSE);
		}
	}

	// on appelle le gestionnaire de base	
	this->JFCViewport::OnLButtonUp(nFlags, point);
}
