// on inclut les d�finitions n�cessaires
#include "stdafx.h"
#include "JFCScroll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// le constructeur
/////////////////////////////////////////////////////////////////////////////

JFCScroll::JFCScroll()
{
	// on ne fait rien
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour cr�er un contr�le
/////////////////////////////////////////////////////////////////////////////

BOOL JFCScroll::Create(DWORD dwStyle, const RECT & Rect, CWnd* pParentWnd, UINT nID)
{
	// on cr�e la fen�tre
	BOOL succes = ((CWnd*)this)->Create(_T("SCROLLBAR"), NULL, dwStyle, Rect, pParentWnd, nID);

	// on renvoie l'indicateur de r�ussite
	return (succes);
}

/////////////////////////////////////////////////////////////////////////////
// le destructeur
/////////////////////////////////////////////////////////////////////////////

JFCScroll::~JFCScroll()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(JFCScroll, CWnd)
	//{{AFX_MSG_MAP(JFCScroll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// la fonction pour manipuler la configuration de l'ascenseur
/////////////////////////////////////////////////////////////////////////////

void JFCScroll::SetRange(LONG minimum, LONG maximum, LONG page)
{
	SCROLLINFO infos;

	// on initialise les informations
	infos.cbSize = sizeof(infos);
    infos.fMask  = SIF_RANGE | SIF_PAGE;
    infos.nMin   = minimum;
    infos.nMax   = maximum;
	infos.nPage  = page;

	// on met � jour l'ascenseur
	this->SendMessage(SBM_SETSCROLLINFO, (WPARAM)FALSE, (LPARAM)&infos);

	// on teste le nouvel �tat de l'ascenseur
	if ((infos.nMax - infos.nMin) >= (LONG)infos.nPage)
	{
		// on active l'ascenseur
		this->EnableWindow(TRUE);

		// on redessine l'ascenseur
		this->InvalidateRect(NULL, FALSE);
	}
	else
	{
		// on d�sactive l'ascenseur
		this->EnableWindow(FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour manipuler la position de l'ascenseur
/////////////////////////////////////////////////////////////////////////////

void JFCScroll::SetPosition(LONG position)
{
	SCROLLINFO infos;

	// on v�rifie l'�tat de la fen�tre
	if (FALSE == this->IsWindowEnabled()) return;

	// on initialise les informations
	infos.cbSize = sizeof(infos);
    infos.fMask  = SIF_POS;
	infos.nPos   = position;

	// on r�cup�re les infos
	this->SendMessage(SBM_SETSCROLLINFO, (WPARAM)TRUE, (LPARAM)&infos);
}

LONG JFCScroll::GetPosition()
{
	SCROLLINFO infos;

	// on v�rifie l'�tat de la fen�tre
	if (FALSE == this->IsWindowEnabled()) return (0);

	// on initialise les informations
	infos.cbSize = sizeof(infos);
    infos.fMask  = SIF_POS;

	// on r�cup�re les infos
	this->SendMessage(SBM_GETSCROLLINFO, (WPARAM)0, (LPARAM)&infos);

	// on renvoie la position
	return (infos.nPos);
}

/////////////////////////////////////////////////////////////////////////////
// JFCScroll message handlers

void JFCScroll::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->CWnd::PreSubclassWindow();

	// on initialise l'ascenseur
	this->SetRange(0, 0, 1);
}
