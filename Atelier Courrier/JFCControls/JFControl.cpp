// on inclut les d�finitions n�cessaires
#include "stdafx.h"
#include "JFControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// on d�finit le nom de la classe
static const TCHAR* g_Class = "JFControl";

/////////////////////////////////////////////////////////////////////////////
// le constructeur
/////////////////////////////////////////////////////////////////////////////

JFControl::JFControl()
{
	// on ne fait rien
	m_CATPColorsApp = CATPColors::COLORCREUSET;
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour cr�er un contr�le
/////////////////////////////////////////////////////////////////////////////

BOOL JFControl::Create(LPCTSTR lpszText, DWORD dwStyle, const RECT & rect, CWnd* pParentWnd, UINT nID)
{
	// on cr�e la fen�tre
	BOOL succes = ((CWnd*)this)->Create(_T(g_Class), lpszText, dwStyle, rect, pParentWnd, nID);

	// on renvoie l'indicateur de r�ussite
	return (succes);
}

/////////////////////////////////////////////////////////////////////////////
// le destructeur
/////////////////////////////////////////////////////////////////////////////

JFControl::~JFControl()
{
	// on ne fait rien
}

/////////////////////////////////////////////////////////////////////////////
// la fonction d'enregistrement de la classe de fen�tre
/////////////////////////////////////////////////////////////////////////////

BOOL JFControl::RegisterClass()
{
	return (JFControl::RegisterClass(::AfxGetInstanceHandle()));
}

BOOL JFControl::RegisterClass(HINSTANCE hInstance)
{
	WNDCLASS infos;

	// on initialise les infos de la classe de fen�tre
    infos.style         = CS_OWNDC | CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_GLOBALCLASS;
    infos.lpfnWndProc   = ::DefWindowProc;
    infos.cbClsExtra    = 0;
    infos.cbWndExtra    = 0;
    infos.hInstance     = hInstance;
    infos.hIcon         = NULL;
    infos.hCursor       = ::LoadCursor(NULL, IDC_ARROW);
    infos.hbrBackground = NULL;
    infos.lpszMenuName  = NULL;
    infos.lpszClassName = _T(g_Class);

	// on enregistre la classe de fen�tre
	BOOL succes = ::AfxRegisterClass(&infos);

	// on quitte
	return (succes);
}

BEGIN_MESSAGE_MAP(JFControl, CWnd)
	//{{AFX_MSG_MAP(JFControl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// JFControl message handlers
