// on inclut les d�finitions n�cessaires
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

	// on initialise la s�lection
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
// la fonction pour r�cup�rer la s�lection des �l�ments
/////////////////////////////////////////////////////////////////////////////

LONG JFCViewportSelectEx::OnGetItemSelection(LONG i, LONG j)
{
	// on renvoie la s�lection de l'�l�ment
	return (0);
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour s�lectionner les �l�ments
/////////////////////////////////////////////////////////////////////////////

bool JFCViewportSelectEx::OnBeginSelecting(LONG & etat)
{
	// on arr�te la s�lection des �l�ments
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
// la fonction pour r�cup�rer l'�tat des �l�ments
/////////////////////////////////////////////////////////////////////////////

LONG JFCViewportSelectEx::OnGetItemState(LONG i, LONG j)
{
	// on teste si a une s�lection en cours
	if (m_FlagSel != FALSE)
	{
		// on initialise l'intervalle horizontal
		LONG hstart = (m_HorzSel1 <= m_HorzSel2) ? m_HorzSel1 : m_HorzSel2;
		LONG hend   = (m_HorzSel1 <= m_HorzSel2) ? m_HorzSel2 : m_HorzSel1;

		// on initialise l'intervalle vertical
		LONG vstart = (m_VertSel1 <= m_VertSel2) ? m_VertSel1 : m_VertSel2;
		LONG vend   = (m_VertSel1 <= m_VertSel2) ? m_VertSel2 : m_VertSel1;

		// on teste si l'�l�ment est dans l'intervalle
		if ((i >= hstart) && (i <= hend) && (j >= vstart) && (j <= vend))
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
	else
	{
		// on renvoie l'�tat de l'�l�ment
		return (this->OnGetItemSelection(i, j));
	}
}

/////////////////////////////////////////////////////////////////////////////
// JFCViewportSelectEx message handlers

void JFCViewportSelectEx::PreSubclassWindow() 
{
	// on initialise l'indicateur
	m_FlagSel = FALSE;

	// on initialise la s�lection
	m_HorzSel1 = m_VertSel1 = m_HorzSel2 = m_VertSel2 = m_EtatSel = 0;

	// on appelle le gestionnaire de base	
	this->JFCViewport::PreSubclassWindow();
}

void JFCViewportSelectEx::OnLButtonDown(UINT nFlags, CPoint point) 
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
					m_HorzSel1 = m_HorzSel2 = i;
					m_VertSel1 = m_VertSel2 = j;

					// on met � jour l'affichage
					this->InvalidateRect(NULL, FALSE);

					// on lance un timer
					this->SetTimer(48758, 50, NULL);
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

void JFCViewportSelectEx::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// on teste si a une s�lection en cours
	if (m_FlagSel != FALSE)
	{
		// on lib�re la souris
		::ReleaseCapture();

		// on teste si la lib�ration s'est bien effectu�e
		if (this->m_hWnd != ::GetCapture())
		{
			// on lib�re le timer
			this->KillTimer(48758);

			// on permute les indices si n�cessaire
			if (m_HorzSel1 > m_HorzSel2) { LONG temp = m_HorzSel1; m_HorzSel1 = m_HorzSel2; m_HorzSel2 = temp; }
			if (m_VertSel1 > m_VertSel2) { LONG temp = m_VertSel1; m_VertSel1 = m_VertSel2; m_VertSel2 = temp; }

			// on parcourt tous les �l�ments � s�lectionner
			for (LONG j = m_VertSel1; j <= m_VertSel2; j += 1)
			{
				for (LONG i = m_HorzSel1; i <= m_HorzSel2; i += 1)
				{
					// on s�lectionne l'�l�ment courant
					this->OnSelectItem(i, j, m_EtatSel);
				}
			}

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

void JFCViewportSelectEx::OnMouseMove(UINT nFlags, CPoint point) 
{
	// on teste si on a une s�lection en cours
	if (m_FlagSel != FALSE)
	{
		RECT rect;

		// on r�cup�re la zone cliente
		this->GetClientRect(&rect);

		// on teste si le curseur est dans la fen�tre
		if ((point.x + 1) > rect.left && (point.y + 1) > rect.top && point.x < rect.right && point.y < rect.bottom)
		{
			// on r�cup�re l'�l�ment sous la position
			this->GetTrackIdx(point.x, point.y, m_HorzSel2, m_VertSel2, FALSE);

			// on redessine la fen�tre
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
		// on teste si on a une s�lection en cours
		if (m_FlagSel != FALSE)
		{
			RECT  rect;
			POINT point;

			// on r�cup�re la position du curseur
			DWORD pos = ::GetMessagePos();

			// on r�cup�re les coordonn�es du curseur
			point.x = (LONG)(SHORT)LOWORD(pos);
			point.y = (LONG)(SHORT)HIWORD(pos);

			// on r�cup�re la zone cliente
			this->GetClientRect(&rect);

			// on convertit la position du curseur dans le rep�re de la fen�tre
			this->ScreenToClient(&point);

			// on teste si le curseur est hors de la fen�tre
			if (point.x < rect.left || point.y < rect.top || (point.x + 1) > rect.right || (point.y + 1) > rect.bottom)
			{
				// on r�cup�re l'�l�ment sous la position
				this->GetTrackIdx(point.x, point.y, m_HorzSel2, m_VertSel2, TRUE);

				// on redessine la fen�tre
				this->InvalidateRect(NULL, FALSE);
			}
		}
	}

	// on appelle le gestionnaire de base	
	this->JFCViewport::OnTimer(nIDEvent);
}
