// on inclut les définitions nécessaires
#include "stdafx.h"

#include "JFCFrame.h"
#include "JFCApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// on définit le nom de la classe
static const TCHAR* g_Class = "JFCFrame";

//////////////////
// le constructeur

JFCFrame::JFCFrame()
{
	// on initialise l'indicateur
	m_bAutoDelete = TRUE;
}

//////////////////////////////////
// la fonction pour créer le frame

BOOL JFCFrame::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT & rect, CWnd* pParentWnd, LPCTSTR lpszMenuName, DWORD dwExStyle)
{
	// on appelle le gestionnaire de base
	if (this->CFrameWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, lpszMenuName, dwExStyle, NULL) == FALSE) return (FALSE);

	// on quitte
	return (TRUE);
}

////////////////////////////////////
// la fonction pour charger le frame

BOOL JFCFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd)
{
	// on appelle le gestionnaire de base
	if (this->CFrameWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, NULL) == FALSE) return (FALSE);

	// on modifie l'icône de la fenêtre
	HICON hIcon = ::AfxGetApp()->LoadIcon(nIDResource); if (NULL != hIcon) this->SetIcon(hIcon, TRUE);

	// on quitte
	return (TRUE);
}

/////////////////
// le destructeur

JFCFrame::~JFCFrame()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(JFCFrame, CFrameWnd)
	//{{AFX_MSG_MAP(JFCFrame)
	ON_WM_PAINT()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////
// la fonction pour mettre à jour les vues

VOID JFCFrame::UpdateAllViews(const JFCUpdate & Message)
{
	// on ne fait rien
}

////////////////////////////////////////////
// la fonction d'enregistrement de la classe

BOOL JFCFrame::RegisterClass()
{
	WNDCLASS infos;

	// on initialise les infos de la classe de fenêtre
    infos.style         = CS_OWNDC | CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
    infos.lpfnWndProc   = ::DefWindowProc;
    infos.cbClsExtra    = 0;
    infos.cbWndExtra    = 0;
    infos.hInstance     = ::AfxGetInstanceHandle();
    infos.hIcon         = NULL;
    infos.hCursor       = ::LoadCursor(NULL, IDC_ARROW);
    infos.hbrBackground = NULL;
    infos.lpszMenuName  = NULL;
    infos.lpszClassName = _T(g_Class);

	// on enregistre la classe de fenêtre
	BOOL succes = ::AfxRegisterClass(&infos);

	// on quitte
	return (succes);
}

////////////////////////////
// JFCFrame message handlers

BOOL JFCFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	WNDCLASS wndClass;

	// on modifie la classe de fenêtre
	cs.lpszClass = _T(g_Class);

	// on teste si la classe de fenêtre existe
	if (::GetClassInfo(NULL, _T(g_Class), &wndClass) == FALSE)
	{
		// on enregistre la classe de fenêtre
		if (this->RegisterClass() == FALSE) return (FALSE);
	}

	// on appelle le gestionnaire de base
	if (this->CFrameWnd::PreCreateWindow(cs) == FALSE) return (FALSE);

	// on annule tous les styles de bordure
	cs.dwExStyle &= ~(WS_EX_CLIENTEDGE | WS_EX_STATICEDGE | WS_EX_DLGMODALFRAME);

	// on positionne celui qu'il nous faut
	cs.dwExStyle |= WS_EX_WINDOWEDGE;

	// on quitte
	return (TRUE);
}

void JFCFrame::OnPaint() 
{
	// on initialise le DC
	CPaintDC Dc(this);

	// on récupère les dimensions à dessiner
	LONG Larg = Dc.m_ps.rcPaint.right  - Dc.m_ps.rcPaint.left;
	LONG Haut = Dc.m_ps.rcPaint.bottom - Dc.m_ps.rcPaint.top;

	// on vérifie les dimensions
	if (Larg > 0 && Haut > 0)
	{
		// on crée le pinceau
		CBrush BrushF(::GetSysColor(COLOR_3DFACE));

		// on dessine le fond
		Dc.FillRect(&Dc.m_ps.rcPaint, &BrushF);
	}
}

void JFCFrame::PostNcDestroy() 
{
	// on teste l'indicateur de libération
	if (m_bAutoDelete != FALSE)
	{
		// on appelle le gestionnaire de base
		this->CFrameWnd::PostNcDestroy();
	}
	else
	{
		// on appelle le gestionnaire de base
		this->CWnd::PostNcDestroy();
	}
}

void JFCFrame::OnClose() 
{
	// on supprime notre référence de l'application
	((JFCApp*)::AfxGetApp())->RemFrame(this);

	// on appelle le gestionnaire de base
	this->CFrameWnd::OnClose();
}
