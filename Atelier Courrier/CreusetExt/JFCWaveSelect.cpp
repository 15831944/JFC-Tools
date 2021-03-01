// on inclut les définitions nécessaires
#include "stdafx.h"
#include "JFCWaveSelect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// le constructeur
/////////////////////////////////////////////////////////////////////////////

JFCWaveSelect::JFCWaveSelect()
{
	m_bSelAllowed = true;
	ResetSelection();
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour manipuler le nombre d'éléments verticaux
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// le destructeur
/////////////////////////////////////////////////////////////////////////////

JFCWaveSelect::~JFCWaveSelect()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(JFCWaveSelect, JFCWave)
	//{{AFX_MSG_MAP(JFCWaveSelect)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// la fonction pour récupérer l'état des éléments
/////////////////////////////////////////////////////////////////////////////
void JFCWaveSelect::AllowSelection(bool allow/* = true*/)
{
	m_bSelAllowed = allow;
}

LONG JFCWaveSelect::OnGetItemState(LONG i, LONG j)
{
	// on récupère l'état de l'élément
	LONG etat = this->OnGetItemState(j, m_Debut + i);

	if(HasSelection())
	{
		LONG iStart, jStart, iStop, jStop;
		GetSelRange( iStart, jStart, iStop, jStop);
		if((iStart <= i && i <= iStop) && (jStart <= j && j <= jStop))
		{
			etat &= JFCWAVE_FOND_MASK;
			etat |= JFCWAVE_FOND_SEL;
		}
	}


	// on renvoie l'état
	return (etat);
}

bool JFCWaveSelect::HasSelection()
{
	return ! m_rectSel.IsRectNull();
}

void JFCWaveSelect::ResetSelection()
{
	m_rectSel.SetRect(0, 0, 0, 0);
}

void JFCWaveSelect::GetSelRange(LONG &iStart, LONG& jStart, LONG &iStop, LONG& jStop)
{
	GetIdxAtPos(m_rectSel.TopLeft().x, m_rectSel.TopLeft().y, iStart, jStart);
	GetIdxAtPos(m_rectSel.BottomRight().x, m_rectSel.BottomRight().y, iStop, jStop);
}


LONG JFCWaveSelect::OnGetItemState(LONG indice, JDate date)
{
	// on renvoie l'état de l'élément
	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// les fonctions pour capter les clicks de la souris
/////////////////////////////////////////////////////////////////////////////

void JFCWaveSelect::OnLButtonDown(LONG indice, JDate date)
{
	// on ne fait rien
}

void JFCWaveSelect::OnLButtonUp(LONG indice, JDate date)
{
	// on ne fait rien
}

void JFCWaveSelect::OnRButtonDown(LONG indice, JDate date)
{
	// on ne fait rien
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour capter les déplacements de la souris
/////////////////////////////////////////////////////////////////////////////

void JFCWaveSelect::OnMouseMove(LONG indice, JDate date)
{
	// on ne fait rien
}


void JFCWaveSelect::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// on teste si on a des éléments
	if (m_Count > 0)
	{

		// on teste si on a une période
		if ((m_Debut.IsValid()) && (m_Fin.IsValid()))
		{
			LONG i,j;
			
			if(m_bSelAllowed)
			{
				m_ptFirst = point;
				m_bMoved = false;
				this->SetCapture();
			}

			// on récupère l'élément sous la souris
			if (this->GetIdxAtPos(point.x, point.y, i, j))
			{
				// on effectue le traitement
				this->OnLButtonDown(j, m_Debut + i);
			}
		}
	}

	// on appelle le gestionnaire de base
	this->JFCViewport::OnLButtonDown(nFlags, point);
}

void JFCWaveSelect::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// on teste si on a des éléments
	if (m_Count > 0)
	{
		
		// on teste si on a une période
		if ((m_Debut.IsValid()) && (m_Fin.IsValid()))
		{
			if(m_bSelAllowed && this->GetCapture() == this)
			{
				if (point == m_ptFirst)
					ResetSelection();
				else
				{
					m_rectSel.SetRect(m_ptFirst.x, m_ptFirst.y, point.x, point.y);
					m_rectSel.NormalizeRect();
					m_ptFirst = point;
				}
				m_bMoved = false;
				ReleaseCapture();
			}

			LONG i,j;

			// on récupère l'élément sous la souris
			if (this->GetIdxAtPos(point.x, point.y, i, j))
			{
				// on effectue le traitement
				this->OnLButtonUp(j, m_Debut + i);
			}
		}
	}

	// on appelle le gestionnaire de base
	this->JFCViewport::OnLButtonUp(nFlags, point);
}

void JFCWaveSelect::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// on teste si on a des éléments
	if (m_Count > 0)
	{
		// on teste si on a une période
		if (m_bSelAllowed &&(m_Debut.IsValid()) && (m_Fin.IsValid()))
		{
			LONG i,j;
			if(!HasSelection())
			{
				m_ptFirst = point;
				m_rectSel.SetRect(m_ptFirst.x, m_ptFirst.y, point.x, point.y);
			}

			// on récupère l'élément sous la souris
			if (this->GetIdxAtPos(point.x, point.y, i, j))
			{
				// on effectue le traitement
				this->OnRButtonDown(j, m_Debut + i);
			}
		}
	}

	// on appelle le gestionnaire de base
	this->JFCViewport::OnRButtonDown(nFlags, point);
}

void JFCWaveSelect::OnMouseMove(UINT nFlags, CPoint point) 
{
	// on teste si on a des éléments
	if (m_Count > 0)
	{

		// on teste si on a une période
		if ((m_Debut.IsValid()) && (m_Fin.IsValid()))
		{
			if(m_bSelAllowed && this->GetCapture() == this)
			{
				if(point != m_ptFirst)
					m_rectSel.SetRect(m_ptFirst.x, m_ptFirst.y, point.x, point.y);
				m_rectSel.NormalizeRect();
				Invalidate();
				m_bMoved = true;
			}

			LONG i,j;

			// on récupère l'élément sous la souris
			if (this->GetIdxAtPos(point.x, point.y, i, j))
			{
				// on effectue le traitement
				this->OnMouseMove(j, m_Debut + i);
			}
			else
				this->OnMouseMove(-1, m_Debut);
		}
	}
	
	// on appelle le gestionnaire de base
	this->JFCViewport::OnMouseMove(nFlags, point);
}
