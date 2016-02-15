// ATCloseWnd.cpp : implementation file
//

// on inclut les d�finitions n�cessaires
#include "stdafx.h"
#include "ATCloseWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CATCloseWnd

CATCloseWnd::CATCloseWnd()
{
	// on ne fait rien
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour cr�er la fen�tre
/////////////////////////////////////////////////////////////////////////////

BOOL CATCloseWnd::Create(UINT nID)
{
	// on cr�e la fen�tre
	BOOL succes = ((CWnd*)this)->Create(_T("arzaprntClass32"), NULL, WS_OVERLAPPEDWINDOW, CRect(0,0,0,0), this->GetDesktopWindow(), nID);

	// on renvoie l'indicateur de r�ussite
	return (succes);
}

CATCloseWnd::~CATCloseWnd()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATCloseWnd, CWnd)
	//{{AFX_MSG_MAP(CATCloseWnd)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// la fonction d'enregistrement de la classe de fen�tre
/////////////////////////////////////////////////////////////////////////////

BOOL CATCloseWnd::RegisterClass()
{
	WNDCLASS infos;

	// on initialise les infos de la classe de fen�tre
    infos.style         = CS_HREDRAW | CS_VREDRAW;
    infos.lpfnWndProc   = ::DefWindowProc;
    infos.cbClsExtra    = 0;
    infos.cbWndExtra    = 0;
    infos.hInstance     = ::AfxGetInstanceHandle();
    infos.hIcon         = NULL;
    infos.hCursor       = ::LoadCursor(NULL, IDC_ARROW);
    infos.hbrBackground = NULL;
    infos.lpszMenuName  = NULL;
    infos.lpszClassName = _T("arzaprntClass32");

	// on enregistre la classe de fen�tre
	BOOL succes = AfxRegisterClass(&infos);

	// on renvoie l'indicateur de r�ussite
	return (succes);
}

/////////////////////////////////////////////////////////////////////////////
// CATCloseWnd message handlers

void CATCloseWnd::OnDestroy() 
{
	// on poste un message de sortie
	this->PostMessage(WM_QUIT, 0, 0);

	// on appelle le gestionnaire de base
	CWnd::OnDestroy();
}
