// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "cinema.h"
extern CCinemaApp theApp;

#include "MainFrm.h"
#include "splash.h"
#include "DlgSelection.h"
#include "DlgSelectionProduit.h"
#include "Campagne.h"
#include "CinemaDoc.h"
#include "CinemaView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame


IMPLEMENT_DYNAMIC(CMainFrame, SECMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, SECMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_AFFICHAGE_RPERTOIREDECIBLES, OnAffichageRpertoiredecibles)
	ON_COMMAND(ID_AFFICHAGE_BARREDOUTILSSOURIS, OnAffichageBarredoutilssouris)
	ON_UPDATE_COMMAND_UI(ID_AFFICHAGE_BARREDOUTILSSOURIS, OnUpdateAffichageBarredoutilssouris)
	ON_WM_CLOSE()
	//ON_COMMAND(ID_FILE_NEWPRODUITFILMS, OnFileNewproduitfilms)
	//ON_COMMAND(ID_FILE_OPEN_PRODUITFILMS, OnFileOpenProduitfilms)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// create a menu bar
	m_pMenuBar = new SECMDIMenuBar;	// this is a base class member
	// enable bitmap menu support.
	EnableBmpMenus();
}

CMainFrame::~CMainFrame()
{
	delete m_pMenuBar;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (SECMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(CBRS_EX_COOLBORDERS|CBRS_EX_GRIPPER,this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndOutils.CreateEx(CBRS_EX_COOLBORDERS|CBRS_EX_GRIPPER,this) ||
		!m_wndOutils.LoadToolBar(IDR_OUTILS))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	EnableDocking(CBRS_ALIGN_ANY);
	// Initialize menubar support
	// create a menubar without the toolbar manager
	if (!m_pMenuBar->CreateEx(CBRS_EX_COOLBORDERS | CBRS_EX_GRIPPER, 
								this,
								WS_VISIBLE | WS_CHILD | CBRS_TOP|CBRS_SIZE_DYNAMIC))
	{
		TRACE0("Failed to create menubar\n");
		return -1;
	}

	// Indique les menus de remplacement selon fenêtre Child (Campagne Cinema ou Campagne Films)
	m_pMenuBar->SetMenuInfo(3,IDR_MAINFRAME, IDR_CINEMATYPE,IDR_FILMTYPE);

	AddBmpMenuToolBarResource(IDR_MAINFRAME);
	AddBmpMenuToolBarResource(IDR_OUTILS);
	AddBmpMenuToolBarResource(IDR_TOOLBARNOTUSED);

	m_pMenuBar->EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(m_pMenuBar);
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	m_wndOutils.SetBarStyle(m_wndOutils.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	m_wndOutils.EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndOutils);
	DockControlBarEx(&m_wndOutils,AFX_IDW_DOCKBAR_TOP,1,1/*, float fPctWidth = 0, int nHeight = 150*/);

	// CG: The following line was added by the Splash Screen component.
	CSplashWnd::ShowSplashScreen(this);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	int ytitre=GetSystemMetrics(SM_CYCAPTION);
	int bordx=GetSystemMetrics(SM_CXSIZEFRAME);
	int bordy=GetSystemMetrics(SM_CYSIZEFRAME);

	/* avant
	cs.cx = 788+4*bordx;
	cs.cy = 545+(4*bordy+ytitre+20);
	*/

	// Modif Sept 2013
	cs.cx = 870+4*bordx;
	cs.cy = 650+(4*bordy+ytitre+20);

	if(!SECMDIFrameWnd::PreCreateWindow(cs))
		return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	SECMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	SECMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


#include "dlgrepcible.h"
#include "Splash.h"
void CMainFrame::OnAffichageRpertoiredecibles() 
{
	CDlgRepCible dlg;
	dlg.DoModal();
	
}

void CMainFrame::OnAffichageBarredoutilssouris() 
{
	ShowControlBar(&m_wndOutils,m_wndOutils.IsWindowVisible()==0,0);
}

void CMainFrame::OnUpdateAffichageBarredoutilssouris(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck((m_wndOutils.IsWindowVisible()) != 0);
//	pCmdUI->ContinueRouting();
}

/*
////////////////////////////////////////////////////////////////////////////////////////
//                           GESTION FICHIERS PRODUIT FILMS
//
BOOL CMainFrame::OnFileNewproduitfilms() 
{

	// Création d'une nouvelle campagne étude produits films
	CAnalyseFilmView FilmVisu;
	if(FilmVisu.DoModal()!=IDOK) return FALSE;
	CWaitCursor wait;

	return TRUE;

}
*/

/*
void CMainFrame::OnFileOpenProduitfilms() 
{
	AfxMessageBox ("A FAIRE/ Ouverture produit films");
	
}
*/
