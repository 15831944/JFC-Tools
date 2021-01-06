// on inclut les d�finitions n�cessaires
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

	// on initialise la s�lection
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
// la fonction pour r�cup�rer la s�lection des �l�ments
/////////////////////////////////////////////////////////////////////////////

LONG JFCViewportSelect::OnGetItemSelection(LONG i, LONG j)
{
	// on renvoie la s�lection de l'�l�ment
	return (0);
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour s�lectionner les �l�ments
/////////////////////////////////////////////////////////////////////////////

bool JFCViewportSelect::OnBeginSelecting(LONG & etat)
{
	// on arr�te la s�lection des �l�ments
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
// la fonction pour r�cup�rer l'�tat des �l�ments
/////////////////////////////////////////////////////////////////////////////

LONG JFCViewportSelect::OnGetItemState(LONG i, LONG j)
{
	// on teste si on a une s�lection en cours
	if ((m_FlagSel != FALSE) && (i == m_HorzSel) && (j == m_VertSel))
	{
		// on renvoie l'�tat de l'�l�ment
		return (m_EtatSel);
	}
	else
	{
		// on renvoie l'�tat de l'�l�ment
		return (this->OnGetItemSelection(i, j));
	}
}

/////////////////////////////////////////////////////////////////////////////
// JFCViewportSelect message handlers

void JFCViewportSelect::PreSubclassWindow() 
{
	// on initialise l'indicateur
	m_FlagSel = FALSE;

	// on initialise la s�lection
	m_HorzSel = m_VertSel = m_EtatSel = 0;

	// on appelle le gestionnaire de base	
	this->JFCViewport::PreSubclassWindow();
}

void JFCViewportSelect::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// on teste si on peut lancer une s�lection
	if ((m_FlagSel == FALSE) && (NULL == ::GetCapture()))
	{
		LONG i, j;

		// on r�cup�re l'�l�ment sous le curseur
		if (this->GetIdxAtPos(point.x, point.y, i, j))
		{
			// on r�cup�re l'�tat de l'�l�ment
			m_EtatSel = this->OnGetItemSelection(i, j);

			// on initialise la s�lection des �l�ments
			if (this->OnBeginSelecting(m_EtatSel))
			{
				// on capture la souris
				::SetCapture(this->m_hWnd);

				// on teste si a captur� la souris
				if (this->m_hWnd == ::GetCapture())
				{
					// on modifie l'indicateur de s�lection
					m_FlagSel = TRUE;

					// on m�morise le point de s�lection
					m_HorzSel = i; m_VertSel = j;

					// on met � jour l'affichage
					this->InvalidateRect(NULL, FALSE);
				}
				else
				{
					// on annule la s�lection
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
	// on teste si on a une s�lection en cours
	if (m_FlagSel != FALSE)
	{
		// on lib�re la souris
		::ReleaseCapture();

		// on teste si la lib�ration s'est bien effectu�e
		if (this->m_hWnd != ::GetCapture())
		{
			// on s�lectionne l'�l�ment
			this->OnSelectItem(m_HorzSel, m_VertSel, m_EtatSel);

			// on r�initialise l'indicateur de s�lection
			m_FlagSel = FALSE;

			// on termine la s�lection des �l�ments
			this->OnEndSelecting();

			// on redessine la fen�tre
			this->InvalidateRect(NULL, FALSE);
		}
	}

	// on appelle le gestionnaire de base	
	this->JFCViewport::OnLButtonUp(nFlags, point);
}
