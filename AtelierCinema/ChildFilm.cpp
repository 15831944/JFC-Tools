// ChildFilm.cpp : implementation file
//

#include "stdafx.h"
#include "cinema.h"
#include "ChildFilm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFilm

IMPLEMENT_DYNCREATE(CChildFilm, SECMDIChildWnd)

CChildFilm::CChildFilm()
{
}

CChildFilm::~CChildFilm()
{
}


BEGIN_MESSAGE_MAP(CChildFilm, SECMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFilm)
	ON_WM_GETMINMAXINFO()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFilm message handlers

void CChildFilm::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// MODIF FRED
	// Modification des dimensions de la fenetre de la vue pour qu'elle s'ajuste au cadre de la fenetre MDI.

	CPoint pt(785,/*486*/505);

	// A VIRER CPoint pt(900,700);

	int ytitre=GetSystemMetrics(SM_CYCAPTION);
	int bordx=GetSystemMetrics(SM_CXSIZEFRAME);
	int bordy=GetSystemMetrics(SM_CYSIZEFRAME);
	pt.x+=2*bordx;
	pt.y+=2*bordy;
	lpMMI->ptMaxSize=lpMMI->ptMaxPosition=lpMMI->ptMinTrackSize=lpMMI->ptMaxTrackSize=pt;
	
	SECMDIChildWnd::OnGetMinMaxInfo(lpMMI);


}

void CChildFilm::OnClose() 
{
	SECMDIChildWnd::OnClose();
}

BOOL CChildFilm::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.x = 0;
	cs.y = 0;
	return SECMDIChildWnd::PreCreateWindow(cs);
}
