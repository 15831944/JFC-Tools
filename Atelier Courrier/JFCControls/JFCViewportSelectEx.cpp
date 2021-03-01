// on inclut les définitions nécessaires
#include "stdafx.h"
#include "JFCViewportSelectEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// le constructeur
/////////////////////////////////////////////////////////////////////////////

JFCViewportSelectEx::JFCViewportSelectEx()
{
	// on initialise l'indicateur
	m_FlagSel = FALSE;

	// on initialise la sélection
	m_HorzSel1 = m_VertSel1 = m_HorzSel2 = m_VertSel2 = m_EtatSel = 0;
}

/////////////////////////////////////////////////////////////////////////////
// le destructeur
/////////////////////////////////////////////////////////////////////////////

JFCViewportSelectEx::~JFCViewportSelectEx()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(JFCViewportSelectEx, JFCViewport)
	//{{AFX_MSG_MAP(JFCViewportSelectEx)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// la fonction pour récupérer la sélection des éléments
/////////////////////////////////////////////////////////////////////////////

LONG JFCViewportSelectEx::OnGetItemSelection(LONG i, LONG j)
{
	// on renvoie la sélection de l'élément
	return (0);
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour sélectionner les éléments
/////////////////////////////////////////////////////////////////////////////

bool JFCViewportSelectEx::OnBeginSelecting(LONG & etat)
{
	// on arrête la sélection des éléments
	return (false);
}

void JFCViewportSelectEx::OnSelectItem(LONG i, LONG j, LONG etat)
{
	// on ne fait rien
}

void JFCViewportSelectEx::OnEndSelecting()
{
	// on ne fait rien
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour récupérer l'état des éléments
/////////////////////////////////////////////////////////////////////////////

LONG JFCViewportSelectEx::OnGetItemState(LONG i, LONG j)
{
	// on teste si a une sélection en cours
	if (m_FlagSel != FALSE)
	{
		// on initialise l'intervalle horizontal
		LONG hstart = (m_HorzSel1 <= m_HorzSel2) ? m_HorzSel1 : m_HorzSel2;
		LONG hend   = (m_HorzSel1 <= m_HorzSel2) ? m_HorzSel2 : m_HorzSel1;

		// on initialise l'intervalle vertical
		LONG vstart = (m_VertSel1 <= m_VertSel2) ? m_VertSel1 : m_VertSel2;
		LONG vend   = (m_VertSel1 <= m_VertSel2) ? m_VertSel2 : m_VertSel1;

		// on teste si l'élément est dans l'intervalle
		if ((i >= hstart) && (i <= hend) && (j >= vstart) && (j <= vend))
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
	else
	{
		// on renvoie l'état de l'élément
		return (this->OnGetItemSelection(i, j));
	}
}

/////////////////////////////////////////////////////////////////////////////
// JFCViewportSelectEx message handlers

void JFCViewportSelectEx::PreSubclassWindow() 
{
	// on initialise l'indicateur
	m_FlagSel = FALSE;

	// on initialise la sélection
	m_HorzSel1 = m_VertSel1 = m_HorzSel2 = m_VertSel2 = m_EtatSel = 0;

	// on appelle le gestionnaire de base	
	this->JFCViewport::PreSubclassWindow();
}

void JFCViewportSelectEx::OnLButtonDown(UINT nFlags, CPoint point) 
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
					m_HorzSel1 = m_HorzSel2 = i;
					m_VertSel1 = m_VertSel2 = j;

					// on met à jour l'affichage
					this->InvalidateRect(NULL, FALSE);

					// on lance un timer
					this->SetTimer(48758, 50, NULL);
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

void JFCViewportSelectEx::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// on teste si a une sélection en cours
	if (m_FlagSel != FALSE)
	{
		// on libère la souris
		::ReleaseCapture();

		// on teste si la libération s'est bien effectuée
		if (this->m_hWnd != ::GetCapture())
		{
			// on libère le timer
			this->KillTimer(48758);

			// on permute les indices si nécessaire
			if (m_HorzSel1 > m_HorzSel2) { LONG temp = m_HorzSel1; m_HorzSel1 = m_HorzSel2; m_HorzSel2 = temp; }
			if (m_VertSel1 > m_VertSel2) { LONG temp = m_VertSel1; m_VertSel1 = m_VertSel2; m_VertSel2 = temp; }

			// on parcourt tous les éléments à sélectionner
			for (LONG j = m_VertSel1; j <= m_VertSel2; j += 1)
			{
				for (LONG i = m_HorzSel1; i <= m_HorzSel2; i += 1)
				{
					// on sélectionne l'élément courant
					this->OnSelectItem(i, j, m_EtatSel);
				}
			}

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

void JFCViewportSelectEx::OnMouseMove(UINT nFlags, CPoint point) 
{
	// on teste si on a une sélection en cours
	if (m_FlagSel != FALSE)
	{
		RECT rect;

		// on récupère la zone cliente
		this->GetClientRect(&rect);

		// on teste si le curseur est dans la fenêtre
		if ((point.x + 1) > rect.left && (point.y + 1) > rect.top && point.x < rect.right && point.y < rect.bottom)
		{
			// on récupère l'élément sous la position
			this->GetTrackIdx(point.x, point.y, m_HorzSel2, m_VertSel2, FALSE);

			// on redessine la fenêtre
			this->InvalidateRect(NULL, FALSE);
		}
	}

	// on appelle le gestionnaire de base	
	this->JFCViewport::OnMouseMove(nFlags, point);
}

void JFCViewportSelectEx::OnTimer(UINT nIDEvent) 
{
	// on teste si c'est le bon timer
	if (nIDEvent == 48758)
	{
		// on teste si on a une sélection en cours
		if (m_FlagSel != FALSE)
		{
			RECT  rect;
			POINT point;

			// on récupère la position du curseur
			DWORD pos = ::GetMessagePos();

			// on récupère les coordonnées du curseur
			point.x = (LONG)(SHORT)LOWORD(pos);
			point.y = (LONG)(SHORT)HIWORD(pos);

			// on récupère la zone cliente
			this->GetClientRect(&rect);

			// on convertit la position du curseur dans le repère de la fenêtre
			this->ScreenToClient(&point);

			// on teste si le curseur est hors de la fenêtre
			if (point.x < rect.left || point.y < rect.top || (point.x + 1) > rect.right || (point.y + 1) > rect.bottom)
			{
				// on récupère l'élément sous la position
				this->GetTrackIdx(point.x, point.y, m_HorzSel2, m_VertSel2, TRUE);

				// on redessine la fenêtre
				this->InvalidateRect(NULL, FALSE);
			}
		}
	}

	// on appelle le gestionnaire de base	
	this->JFCViewport::OnTimer(nIDEvent);
}
