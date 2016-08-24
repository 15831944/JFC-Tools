// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "admin.h"
#include "DlgAjoutCmplx.h"
#include "MainFrm.h"
#include "DlgModifCmplx.h"
#include "DlgSuppCmplx.h"
#include "DlgAjoutBac.h"
#include "DlgModifBac.h"
#include "DlgSuppBac.h"
#include "DlgAjoutProduit.h"
#include "DlgModifProduit.h"
#include "DlgSuppProduit.h"
#include "DlgAjoutTempo.h"
#include "DlAjoutFormat.h"
#include "DlgSuppFormat.h"
#include "DlgSuppTempo.h"
#include "DlgModifTempo.h"
#include "DlgSelectProduit.h"
#include "DlgEditModifTarif.h"
#include "DlgModifCNC.h"
#include "DlgAjoutComplexeGroupe.h"
#include "DlgAjoutBacGroupe.h"
#include "DlgSuppBacGroupe.h"
#include "DlgSuppCpmlxGroupe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_COMPLEXE_AJOUT, OnComplexeAjout)
	ON_COMMAND(ID_COMPLEXE_MODIFIER, OnComplexeModifier)
	ON_COMMAND(ID_COMPLEXE_SUPPRIMER, OnComplexeSupprimer)

	ON_COMMAND(ID_COMPLX_AJOUT_GROUPE, OnComplexeAjoutGroupe)
	ON_COMMAND(ID_COMPLX_SUPP_GROUPE, OnComplexeSupprimerGroupe)
	// ON_COMMAND(ID_COMPLX_MODIF_GROUPE, OnComplexeModifierGroupe)

	ON_COMMAND(ID_BASE_METTREJOUR, OnBaseMettrejour)

	ON_COMMAND(ID_BAC_AJOUTER, OnBacAjouter)
	ON_COMMAND(ID_BAC_MODIFIER, OnBacModifier)
	ON_COMMAND(ID_BAC_SUPPRIMER, OnBacSupprimer)

	ON_COMMAND(ID_BAC_AJOUT_GROUPE, OnBacAjoutGroupe)
	ON_COMMAND(ID_BAC_SUPP_GROUPE, OnBacSupprimerGroupe)
	// ON_COMMAND(ID_BAC_MODIF_GROUPE, OnBacModifierGroupe)

	ON_WM_CLOSE()
	ON_COMMAND(ID_PRODUIT_AJOUTER, OnProduitAjouter)
	ON_COMMAND(ID_PRODUIT_MODIFIER, OnProduitModifier)
	ON_COMMAND(ID_PRODUIT_SUPPRIMER, OnProduitSupprimer)
	ON_COMMAND(ID_TEMPO_AJOUTER, OnTempoAjouter)
	ON_COMMAND(ID_TEMPO_MODIFIER, OnTempoModifier)
	ON_COMMAND(ID_TEMPO_SUPPRIMER, OnTempoSupprimer)
	ON_COMMAND(ID_FORMAT_AJOUTER, OnFormatAjouter)
	ON_COMMAND(ID_FORMAT_SUPPRIMER, OnFormatSupprimer)
	ON_COMMAND(ID_COMPLEXE_IMPORTDUTARIF, OnComplexeImportdutarif)
	ON_COMMAND(ID_BAC_IMPORTDUTARIF, OnBacImportdutarif)
	ON_COMMAND(ID_PRODUIT_IMPORTDUTARIF, OnProduitImportdutarif)
	ON_COMMAND(ID_DONNESCNC_IMPORTER, OnDonnescncImporter)
	ON_COMMAND(ID_SAISONNALITS_IMPORTER, ImporterSaisonnalite)
	ON_COMMAND(ID_PRODUIT_AJOUTEDEPUIS, OnProduitAjoutedepuis)
	ON_COMMAND(ID_COMPLEXE_EDITMODIFIE_TARIF, OnComplexeEditmodifieTarif)
	ON_COMMAND(ID_BAC_EDITMODIFIE_TARIF, OnBacEditmodifieTarif)
	ON_COMMAND(ID_PRODUIT_EDITMODIFIE_TARIF, OnProduitEditmodifieTarif)
	ON_COMMAND(ID_DONNEESCNC_MODIF, OnDonneescncModif)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

extern CAdminApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Supression de la vue 
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}
	
	/*if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	*/

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
void CMainFrame::OnSetFocus(CWnd* pOldWnd)
{
	// forward focus to the view window
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


void CMainFrame::OnComplexeAjout() 
{
	CDlgAjoutCmplx dlgAjout;
	// copie de la table complexe
	dlgAjout.m_TableComplexe = theApp.base.GetNewInstanceTableComplexe();
	if(dlgAjout.DoModal()!=IDOK)return;
	// récupération de la table
	theApp.base.GetTableComplexe()->RemoveAll();
	theApp.base.SetTableComplexe(dlgAjout.m_TableComplexe); 
}

void CMainFrame::OnComplexeModifier() 
{
	CDlgModifCmplx dlgModif;
	// copie de la table complexe
	dlgModif.m_TableComplexe = theApp.base.GetNewInstanceTableComplexe();

	if(dlgModif.DoModal()!=IDOK)return;
	// récupération de la table
	theApp.base.GetTableComplexe()->RemoveAll();
	theApp.base.SetTableComplexe(dlgModif.m_TableComplexe); 
}


void CMainFrame::OnComplexeSupprimer() 
{
	CDlgSuppCmplx dlgSupp;
	// copie de la table complexe
	dlgSupp.m_TableComplexe.RemoveAll();  
	dlgSupp.m_TableComplexe = theApp.base.GetNewInstanceTableComplexe();
	if(dlgSupp.DoModal()!=IDOK)return;
	// récupération de la table
	theApp.base.GetTableComplexe()->RemoveAll();
	theApp.base.SetTableComplexe(dlgSupp.m_TableComplexe); 
}


//////////////////////////////// AJOUT - SUPP - MODIF GROUPE COMPLEXES ///////////////////////////
//
void CMainFrame::OnComplexeAjoutGroupe() 
{
	CDlgAjoutComplexeGroupe dlgAjout;

	// Récup table des complexes
	dlgAjout.m_TableComplexe = theApp.base.GetNewInstanceTableComplexe();

	// copie de la table groupe complexe
	dlgAjout.m_TableGroupeComplexe = theApp.base.GetNewInstanceTableGroupeComplexe();


	// Lance dialog création groupe complexe
	if(dlgAjout.DoModal()!=IDOK)return;
}

void CMainFrame::OnComplexeModifierGroupe() 
{
	
}


void CMainFrame::OnComplexeSupprimerGroupe() 
{
	CDlgSuppCpmlxGroupe dlgSupp;

	// Récup table des complexes
	dlgSupp.m_TableComplexe = theApp.base.GetNewInstanceTableComplexe();

	// copie de la table groupe complexe
	dlgSupp.m_TableGroupeComplexe = theApp.base.GetNewInstanceTableGroupeComplexe();


	// Lance dialog création groupe complexe
	if(dlgSupp.DoModal()!=IDOK)return;
}

void CMainFrame::OnBaseMettrejour() 
{
	// Mise a jour des données: écriture des tables sur le disque	
	if(theApp.IsModified()) 
	{
		theApp.SaveTable();
	}
	else
	{
		AfxMessageBox("Les données n'ont subie aucune modification");
	}
}


void CMainFrame::OnBacAjouter() 
{
	CDlgAjoutBac dlgAjout;

	// copie de la table bac
	dlgAjout.m_TableBac = theApp.base.GetNewInstanceTableBac();
	if(dlgAjout.DoModal()!=IDOK)return;

	// récupération de la table
	theApp.base.GetTableBac()->RemoveAll();
	theApp.base.SetTableBac(dlgAjout.m_TableBac); 
	
}

void CMainFrame::OnBacModifier() 
{
	CDlgModifBac dlgModif;

	// copie de la table bac
	dlgModif.m_TableBac = theApp.base.GetNewInstanceTableBac();
	if(dlgModif.DoModal()!=IDOK)return;

	// récupération de la table
	theApp.base.GetTableBac()->RemoveAll();
	theApp.base.SetTableBac(dlgModif.m_TableBac); 
}

void CMainFrame::OnBacSupprimer() 
{
	CDlgSuppBac dlgSupp;

	// copie de la table bac
	dlgSupp.m_TableBac = theApp.base.GetNewInstanceTableBac();
	if(dlgSupp.DoModal()!=IDOK)return;

	// récupération de la table
	theApp.base.GetTableBac()->RemoveAll();
	theApp.base.SetTableBac(dlgSupp.m_TableBac); 
}

//////////////////////////////// AJOUT - SUPP - MODIF GROUPE BACS ///////////////////////////
//
void CMainFrame::OnBacAjoutGroupe() 
{
	CDlgAjoutBacGroupe dlgAjout;

	// copie de la table bac
	dlgAjout.m_TableBac = theApp.base.GetNewInstanceTableBac();

	// copie de la table groupe bac
	dlgAjout.m_TableGroupeBac = theApp.base.GetNewInstanceTableGroupeBac();


	// Lance dialog création groupe bac
	if(dlgAjout.DoModal()!=IDOK)return;

}

void CMainFrame::OnBacModifierGroupe() 
{
	
}


void CMainFrame::OnBacSupprimerGroupe() 
{
	CDlgSuppBacGroupe dlgSupp;

	// copie de la table bac
	dlgSupp.m_TableBac = theApp.base.GetNewInstanceTableBac();

	// copie de la table groupe bac
	dlgSupp.m_TableGroupeBac = theApp.base.GetNewInstanceTableGroupeBac();


	// Lance dialog création groupe bac
	if(dlgSupp.DoModal()!=IDOK)return;
}


void CMainFrame::OnClose() 
{
	// Les tables ont elles été modifié?
	if(theApp.IsModified())
	{
		if(AfxMessageBox(" Les tables ont été modifiées\nSouhaitez vous les enregistrer?",MB_YESNO)==IDYES)
		{
			theApp.SaveTable();
		}
		
	}
	CFrameWnd::OnClose();
}

void CMainFrame::OnProduitAjouter() 
{
	CDlgAjoutProduit dlgAjout;

	// copie de la table produit
	dlgAjout.m_TableProduit = theApp.base.GetNewInstanceTableProduit();
	// copie de la table bac
	dlgAjout.m_TableBac = theApp.base.GetNewInstanceTableBac();
	// copie de la table complexe
	dlgAjout.m_TableComplexe = theApp.base.GetNewInstanceTableComplexe();

	if(dlgAjout.DoModal()!=IDOK)return;

	// récupération de la table produit
	theApp.base.GetTableProduit()->RemoveAll();
	theApp.base.SetTableProduit(dlgAjout.m_TableProduit); 

	
}

void CMainFrame::OnProduitModifier() 
{
	CDlgModifProduit dlgModif;

	// copie de la table produit
	dlgModif.m_TableProduit = theApp.base.GetNewInstanceTableProduit();
	if(dlgModif.DoModal()!=IDOK)return;

	
}

void CMainFrame::OnProduitSupprimer() 
{
	CDlgSuppProduit dlgSupp;

	// copie de la table produit
	dlgSupp.m_TableProduit = theApp.base.GetNewInstanceTableProduit();

	if(dlgSupp.DoModal()!=IDOK)return;

	// récupération de la table produit
	theApp.base.GetTableProduit()->RemoveAll();
	theApp.base.SetTableProduit(dlgSupp.m_TableProduit); 	
}

void CMainFrame::OnTempoAjouter() 
{
	CDlgAjoutTempo	dlgAjoutTempo;
	// récupération de la table des tempos
	dlgAjoutTempo.m_TableTempo = theApp.base.GetNewInstanceTableTempo(); 
	if(dlgAjoutTempo.DoModal()!=IDOK)return;
	theApp.base.GetTableTempo()->RemoveAll();
	theApp.base.SetTableTempo(dlgAjoutTempo.m_TableTempo);
	int size = 	theApp.base.GetTableTempo()->GetSize();

	for(int i=0;i<size;i++)
	{
		TRACE("tempos:%s\n",theApp.base.GetTableTempo()->GetAt(i).GetLibelle());
	}

}

void CMainFrame::OnTempoModifier() 
{
	CDlgModifTempo	dlgModifTempo;
	// récupération de la table
	dlgModifTempo.m_TableTempo = theApp.base.GetNewInstanceTableTempo();
	if(dlgModifTempo.DoModal()!=IDOK)return;
	theApp.base.GetTableTempo()->RemoveAll();
	theApp.base.SetTableTempo(dlgModifTempo.m_TableTempo); 
	
}

void CMainFrame::OnTempoSupprimer() 
{
	CDlgSuppTempo	dlgSuppTempo;
	// récupération de la table
	dlgSuppTempo.m_TableTempo = theApp.base.GetNewInstanceTableTempo();
	if(dlgSuppTempo.DoModal()!=IDOK)return;
	theApp.base.GetTableTempo()->RemoveAll();
	theApp.base.SetTableTempo(dlgSuppTempo.m_TableTempo); 
}

void CMainFrame::OnFormatAjouter() 
{
	CDlAjoutFormat	dlgAjoutFormat;
	// récupération de la table
	dlgAjoutFormat.m_TableFormat = theApp.base.GetNewInstanceTableFormat();

	if(dlgAjoutFormat.DoModal()!=IDOK)return;
	theApp.base.GetTableFormat()->RemoveAll();
	theApp.base.SetTableFormat(dlgAjoutFormat.m_TableFormat); 

}

void CMainFrame::OnFormatSupprimer() 
{
	CDlgSuppFormat	dlgSuppFormat;
	// récupération de la table
	dlgSuppFormat.m_TableFormat = theApp.base.GetNewInstanceTableFormat();

	if(dlgSuppFormat.DoModal()!=IDOK)return;
	theApp.base.GetTableFormat()->RemoveAll();
	theApp.base.SetTableFormat(dlgSuppFormat.m_TableFormat); 
		
}

void CMainFrame::OnComplexeImportdutarif() 
{
	theApp.ImportTarif("TARIFC");
 
}

void CMainFrame::OnBacImportdutarif() 
{

	theApp.ImportTarif("TARIFB");
	
}

void CMainFrame::OnProduitImportdutarif() 
{
	theApp.ImportTarif("TARIFP");
	
}

void CMainFrame::OnDonnescncImporter() 
{
	theApp.ImportTarif("CNC");
	
}

void CMainFrame::ImporterSaisonnalite() 
{
	theApp.ImportTarif("SAISON");
}

void CMainFrame::OnProduitAjoutedepuis() 
{
	CDlgSelectProduit DlgSelProduit;

	// copie de la table produit
	DlgSelProduit.m_TableProduit = theApp.base.GetNewInstanceTableProduit();
	if(DlgSelProduit.DoModal()!=IDOK)return;
}

//// Modification ou Edition des Tarifs des Complexes
//// ================================================
void CMainFrame::OnComplexeEditmodifieTarif() 
{
	// Edition ou modification des tarifs des complexes
	CDlgEditModifTarif dlgEditModif;

	// Ici il s'agit de modifier les tarifs des élmts Complexes
	dlgEditModif.m_TypeElmt = theApp.TypeComplexe;

	// copie de la table complexe
	dlgEditModif.m_TableComplexe = theApp.base.GetNewInstanceTableComplexe();

	// Affichage dialogue tarif complexe
	if(dlgEditModif.DoModal()!=IDOK)return ;

	
}

//// Modification ou Edition des Tarifs des Bacs
//// ===========================================
void CMainFrame::OnBacEditmodifieTarif() 
{
	// Edition ou modification des tarifs des bacs
	CDlgEditModifTarif dlgEditModif;

	// Ici il s'agit de modifier les tarifs des élmts BAC
	dlgEditModif.m_TypeElmt = theApp.TypeBAC;

	// copie de la table BAC
	dlgEditModif.m_TableBAC = theApp.base.GetNewInstanceTableBac();

	// Affichage dialogue tarif complexe
	if(dlgEditModif.DoModal()!=IDOK)return ;
	
}

//// Modification ou Edition des Tarifs des Produits
//// ===============================================
void CMainFrame::OnProduitEditmodifieTarif() 
{
	
	// Edition ou modification des tarifs des produits
	CDlgEditModifTarif dlgEditModif;

	// Ici il s'agit de modifier les tarifs des élmts Produits
	dlgEditModif.m_TypeElmt = theApp.TypeProduit;

	// copie de la table Produit
	dlgEditModif.m_TableProduit = theApp.base.GetNewInstanceTableProduit();

	// Affichage dialogue tarif produit
	if(dlgEditModif.DoModal()!=IDOK)return ;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Modification des données CNC (affichage liste de tous les complexes + CNC courant)
void CMainFrame::OnDonneescncModif() 
{
	
	// Boite Dialogue Modification CNC Complexes
	CDlgModifCNC dlgModifCNC;

	// Affichage dialogue modif CNC
	if(dlgModifCNC.DoModal()!=IDOK)return ;
}

void CMainFrame::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
}

void CMainFrame::OnProduitfilmsAjoutedepuis() 
{
	AfxMessageBox ("A FAIRE / Ajouter produit films à partir de ....");
	
}

void CMainFrame::OnProduitfilmsAjouter() 
{
	AfxMessageBox ("A FAIRE / Ajouter produit films");
	
}

void CMainFrame::OnProduitfilmsEditermodifiertarif() 
{
	AfxMessageBox ("A FAIRE / Editer/Modifier produit films");
	
}

void CMainFrame::OnProduitfilmsImportdutarif() 
{
	AfxMessageBox ("A FAIRE / Import tarif produit films");
	
}

void CMainFrame::OnProduitfilmsModifier() 
{
	AfxMessageBox ("A FAIRE / Modifier produit films");
	
}

void CMainFrame::OnProduitfilmsSupprimer() 
{
	AfxMessageBox ("A FAIRE / Supprimer produit films");
	
}
