// cinemaView.cpp : implementation of the CCinemaView class
//

#include "stdafx.h"
#include <afxadv.h>            // For CSharedFile
#include "cinema.h"
extern CCinemaApp theApp;

#include "cinemaDoc.h"
#include "cinemaView.h"
#include "dlgformat.h"
#include "DlgSelection.h"
#include "DlgExportMediavision.h"
#include "DlgExportScreenvision.h"
#include "CampagneFilm.h"
#include "MainFrm.h"

// MODIF AURELIEN : TEST UGRID
#include "TestGrid.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCinemaView

IMPLEMENT_DYNCREATE(CCinemaView, CFormView)

BEGIN_MESSAGE_MAP(CCinemaView, CFormView)
	//{{AFX_MSG_MAP(CCinemaView)
	ON_WM_PAINT()
	ON_COMMAND(ID_OUTILS_AJOUTERSUPPRIMER, OnOutilsAjoutersupprimer)
	ON_COMMAND(ID_OUTILS_COPIERCOLLER, OnOutilsCopiercoller)
	ON_COMMAND(ID_OUTILS_COUPER, OnOutilsCouper)
	ON_COMMAND(ID_OUTILS_DURE, OnOutilsDure)
	ON_COMMAND(ID_OUTILS_GLISSER, OnOutilsGlisser)
	ON_COMMAND(ID_OUTILS_GOMMER, OnOutilsGommer)
	ON_COMMAND(ID_OUTILS_TEMPO, OnOutilsTempo)
	ON_UPDATE_COMMAND_UI(ID_OUTILS_AJOUTERSUPPRIMER, OnUpdateOutilsAjoutersupprimer)
	ON_UPDATE_COMMAND_UI(ID_OUTILS_COPIERCOLLER, OnUpdateOutilsCopiercoller)
	ON_UPDATE_COMMAND_UI(ID_OUTILS_COUPER, OnUpdateOutilsCouper)
	ON_UPDATE_COMMAND_UI(ID_OUTILS_DURE, OnUpdateOutilsDure)
	ON_UPDATE_COMMAND_UI(ID_OUTILS_GLISSER, OnUpdateOutilsGlisser)
	ON_UPDATE_COMMAND_UI(ID_OUTILS_GOMMER, OnUpdateOutilsGommer)
	ON_UPDATE_COMMAND_UI(ID_OUTILS_TEMPO, OnUpdateOutilsTempo)
	ON_COMMAND(ID_PARAMTRES_EXERCICE, OnParamtresExercice)
	ON_BN_CLICKED(IDC_GRAPHE, OnGraphe)
	ON_BN_CLICKED(IDC_PERF, OnPerformances)
	ON_BN_CLICKED(IDC_EFFACER, OnEffacer)
	ON_COMMAND(ID_PARAMTRES_OPTIONDECALCUL, OnParamtresOptiondecalcul)
	ON_COMMAND(ID_PARAMTRES_BUDGETNET, OnParamtresBudgetnet)
	ON_COMMAND(ID_PARAMTRES_CIBLE1, OnParamtresCible1)
	ON_COMMAND(ID_PARAMTRES_BTA, OnParamtresBta)
	ON_BN_CLICKED(IDC_DUREE1, OnDuree1)
	ON_BN_CLICKED(IDC_DUREE2, OnDuree2)
	ON_BN_CLICKED(IDC_DUREE3, OnDuree3)
	ON_BN_CLICKED(IDC_DUREE4, OnDuree4)
	ON_BN_CLICKED(IDC_DUREE5, OnDuree5)
	ON_COMMAND(IDC_ACTION_VALIDER, OnActionValider)
	ON_BN_CLICKED(IDC_HYPOTHESE1, OnHypothese1)
	ON_BN_CLICKED(IDC_HYPOTHESE2, OnHypothese2)
	ON_BN_CLICKED(IDC_HYPOTHESE3, OnHypothese3)
	ON_BN_CLICKED(IDC_HYPOTHESE4, OnHypothese4)
	ON_BN_CLICKED(IDC_HYPOTHESE5, OnHypothese5)
	ON_COMMAND(ID_AFFICHAGE_TEST, OnAffichageTest)
	ON_COMMAND(IDC_ACTION_COPIER, OnActionCopier)
	ON_COMMAND(ID_ACTION_COLLER, OnActionColler)
	ON_UPDATE_COMMAND_UI(ID_ACTION_COLLER, OnUpdateActionColler)
	ON_COMMAND(ID_ACTION_RECOPIERDANSLHYPOTHESEACTIVE_LHYPOTHSE1, OnActionRecopierdanslhypotheseactiveLhypothse1)
	ON_COMMAND(ID_ACTION_RECOPIERDANSLHYPOTHESEACTIVE_LHYPOTHSE2, OnActionRecopierdanslhypotheseactiveLhypothse2)
	ON_COMMAND(ID_ACTION_RECOPIERDANSLHYPOTHESEACTIVE_LHYPOTHSE3, OnActionRecopierdanslhypotheseactiveLhypothse3)
	ON_COMMAND(ID_ACTION_RECOPIERDANSLHYPOTHESEACTIVE_LHYPOTHSE4, OnActionRecopierdanslhypotheseactiveLhypothse4)
	ON_COMMAND(ID_ACTION_RECOPIERDANSLHYPOTHESEACTIVE_LHYPOTHSE5, OnActionRecopierdanslhypotheseactiveLhypothse5)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_COMMAND(ID_CAMPAGNE_EXPORTER, OnCampagneExporter)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_COMMAND(ID_FILE_PRINT2, OnFilePrint2)
	ON_WM_HSCROLL()
	ON_COMMAND(ID_PARAMTRES_CIBLE2, OnParamtresCible2)
	ON_WM_VSCROLL()
	ON_COMMAND(ID_PARAMTRES_FORMAT, OnParamtresFormat)
	ON_BN_CLICKED(IDC_BUTTONCHOIX, OnButtonchoix)
	ON_BN_CLICKED(IDC_PERFAGGLO, OnPerfagglo)
	ON_COMMAND(ID_EXPORT_MEDIAVISION, OnExportMediavision)
	ON_UPDATE_COMMAND_UI(ID_EXPORT_MEDIAVISION, OnUpdateExportMediavision)
	ON_COMMAND(ID_EXPORT_SCREENVISION, OnExportScreenvision)
	ON_UPDATE_COMMAND_UI(ID_EXPORT_SCREENVISION, OnUpdateExportScreenvision)
	ON_WM_SIZE()
	ON_WM_MOVE()
	ON_WM_CLOSE()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCinemaView construction/destruction

CCinemaView::CCinemaView()
	: CFormView(CCinemaView::IDD)
{
	
	//{{AFX_DATA_INIT(CCinemaView)
	//}}AFX_DATA_INIT
	CWaitCursor wait;	

}

CCinemaView::~CCinemaView()
{
}

void CCinemaView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCinemaView)
	DDX_Control(pDX, IDC_BUTTON4, m_BoutonTete);
	DDX_Control(pDX, IDC_SCROLLBAR1, m_ScrollV);
	DDX_Control(pDX, IDC_EFFACER, m_BtnEffacer);
	DDX_Control(pDX, IDC_GRAPHE, m_BtnGraphe);
	DDX_Control(pDX, IDC_PERF, m_BtnPerf);
	//DDX_Control(pDX, IDC_PANORAMA, m_BtnPanorama);
	DDX_Control(pDX, IDC_CADRE2, m_Cadre2);
	DDX_Control(pDX, IDC_DUREE5, m_BtnD5);
	DDX_Control(pDX, IDC_DUREE4, m_BtnD4);
	DDX_Control(pDX, IDC_DUREE3, m_BtnD3);
	DDX_Control(pDX, IDC_DUREE2, m_BtnD2);
	DDX_Control(pDX, IDC_DUREE1, m_BtnD1);
	DDX_Control(pDX, IDC_CADRE1, m_Cadre1);
	DDX_Control(pDX, IDC_HYPOTHESE5, m_BtnH5);
	DDX_Control(pDX, IDC_HYPOTHESE4, m_BtnH4);
	DDX_Control(pDX, IDC_HYPOTHESE3, m_BtnH3);
	DDX_Control(pDX, IDC_HYPOTHESE2, m_BtnH2);
	DDX_Control(pDX, IDC_HYPOTHESE1, m_BtnH1);
	DDX_Control(pDX, IDC_CADRE, m_Cadre);
	DDX_Control(pDX, IDC_HSCROLL, m_ScrollH);
	DDX_Control(pDX, IDC_NBPASSAGES, m_NbPassages);
	DDX_Control(pDX, IDC_GRILLE, m_Grille);
	DDX_Control(pDX, IDC_CALENDRIER, m_Calendrier);
	DDX_Control(pDX, IDC_BINOME, m_Binome);
	//}}AFX_DATA_MAP
}

BOOL CCinemaView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CCinemaView::OnInitialUpdate()
{
	CWaitCursor wait;	
	CFormView::OnInitialUpdate();
	//ResizeParentToFit();
	//CenterWindow();
	SetStateBoutonDuree();
	SetStateBoutonHypothese();

	CCampagneAgglo * camp=&GetDocument()->m_Campagne;

	GetDocument()->m_DlgPerf.Create(IDD_PERFORMANCES,this);
	GetDocument()->m_DlgPerf.Initialise(camp);
	camp->m_Perf=&GetDocument()->m_DlgPerf;

	GetDocument()->m_DlgPerfAgglo.Create(IDD_PERFAGLO,this);
	GetDocument()->m_DlgPerfAgglo.Initialise(camp);
	camp->m_PerfAgglo=&GetDocument()->m_DlgPerfAgglo;

	GetDocument()->m_DlgGraphe.Create(IDD_GRAPHE,this);
	GetDocument()->m_DlgGraphe.Initialise(camp,0);
	camp->m_Graphe=&GetDocument()->m_DlgGraphe;

	m_Calendrier.Init(0,camp);
	m_Binome.Init(1,camp);
	m_NbPassages.Init(2,camp);
	m_Grille.Init(3,camp);

	m_BtnPerf.Initialise(STYLE_STANDARD);//,FORMAT_2STATES);
	m_BoutonTete.Initialise(STYLE_STANDARD);//,FORMAT_2STATES);
	m_BtnGraphe.Initialise(STYLE_STANDARD);//,FORMAT_2STATES);
	//m_BtnPanorama.Initialise(STYLE_STANDARD,FORMAT_2STATES);
	m_BtnEffacer.Initialise(STYLE_STANDARD);

	m_Cadre.SetFond(RGB(128,128,128));

	m_BtnH1.Initialise(STYLE_LAMPE,FORMAT_LIBRE);
	m_BtnH1.ChangeColorLampe(5);
	m_BtnH2.Initialise(STYLE_LAMPE,FORMAT_LIBRE);
	m_BtnH2.ChangeColorLampe(4);
	m_BtnH3.Initialise(STYLE_LAMPE,FORMAT_LIBRE);
	m_BtnH3.ChangeColorLampe(1);
	m_BtnH4.Initialise(STYLE_LAMPE,FORMAT_LIBRE);
	m_BtnH4.ChangeColorLampe(2);
	m_BtnH5.Initialise(STYLE_LAMPE,FORMAT_LIBRE);
	m_BtnH5.ChangeColorLampe(9);

	m_BtnD1.Initialise(STYLE_LAMPE,FORMAT_LIBRE);
	m_BtnD2.Initialise(STYLE_LAMPE,FORMAT_LIBRE);
	m_BtnD3.Initialise(STYLE_LAMPE,FORMAT_LIBRE);
	m_BtnD4.Initialise(STYLE_LAMPE,FORMAT_LIBRE);
	m_BtnD5.Initialise(STYLE_LAMPE,FORMAT_LIBRE);
	InitBoutonDuree();


	// les ascenceurs
	// acsenseur horizontal
	camp->m_PosAscH=0;
	camp->m_hAscH=m_ScrollH.m_hWnd;

	// et vertical
	camp->m_PosAscV=0;
	camp->m_hAscV=m_ScrollV.m_hWnd;

	m_Grille.InitDessin();
	m_Grille.InitAccelerateur();

	RECT R;
	m_Calendrier.GetWindowRect(&R);
	camp->m_HautLigneTitre=(R.bottom-R.top)/2;



	// on doit attendre les données
	camp->m_SetEnCours=1;
	camp->m_SetEnCours=0;

	camp->CalculPassageACCOGRIL();
	camp->ChargeCible(camp->m_NumCible[0],0);

	// préparation du vecteur
	camp->LanceCalcul(CCampagne::CIBLEUTILE);


}

/////////////////////////////////////////////////////////////////////////////
// CCinemaView printing


BOOL CCinemaView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CCinemaView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	m_Print=new CPrint;
	CCampagneAgglo * C=&GetDocument()->m_Campagne;
	m_Print->SetInfo(C);
	m_Print->OnBeginPrinting(pDC,pInfo);
}

void CCinemaView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	m_Print->OnEndPrinting(pDC,pInfo);
	delete m_Print;
	m_Print=NULL;
	UndoPrepareCalculPrint();
}

void CCinemaView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	m_Print->OnPrint(pDC,pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CCinemaView diagnostics

#ifdef _DEBUG
void CCinemaView::AssertValid() const
{
	CFormView::AssertValid();
}

void CCinemaView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CCinemaDoc* CCinemaView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCinemaDoc)));
	return (CCinemaDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCinemaView message handlers

void CCinemaView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CBrush fond(RGB_BleuJFC); //(RGB(23,114,121));

	/*
	CBitmap Bitmap;
	Bitmap.LoadBitmap(IDB_PATTERN1);
	fond.CreatePatternBrush(&Bitmap);
	*/

	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
}

void CCinemaView::OnOutilsAjoutersupprimer() 
{
	GetDocument()->m_Campagne.OutilsActif=0;
}

void CCinemaView::OnOutilsCopiercoller() 
{
	GetDocument()->m_Campagne.OutilsActif=6;
}

void CCinemaView::OnOutilsCouper() 
{
	GetDocument()->m_Campagne.OutilsActif=5;
}

void CCinemaView::OnOutilsDure() 
{
	GetDocument()->m_Campagne.OutilsActif=4;
}

void CCinemaView::OnOutilsGlisser() 
{
	GetDocument()->m_Campagne.OutilsActif=1;
}

void CCinemaView::OnOutilsGommer() 
{
	GetDocument()->m_Campagne.OutilsActif=3;
}

#include "dlgchxtempo.h"
void CCinemaView::OnOutilsTempo() 
{
	GetDocument()->m_Campagne.OutilsActif=2;
	CDlgChxTempo dlg;
	dlg.m_Tempo=GetDocument()->m_Campagne.NrTempoActif;
	if(dlg.DoModal()==IDOK)GetDocument()->m_Campagne.NrTempoActif=dlg.m_Tempo;
}

void CCinemaView::OnUpdateOutilsAjoutersupprimer(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetDocument()->m_Campagne.OutilsActif==0);
}

void CCinemaView::OnUpdateOutilsCopiercoller(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetDocument()->m_Campagne.OutilsActif==6);
}

void CCinemaView::OnUpdateOutilsCouper(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetDocument()->m_Campagne.OutilsActif==5);
}

void CCinemaView::OnUpdateOutilsDure(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetDocument()->m_Campagne.OutilsActif==4);
}

void CCinemaView::OnUpdateOutilsGlisser(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetDocument()->m_Campagne.OutilsActif==1);
	
}

void CCinemaView::OnUpdateOutilsGommer(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetDocument()->m_Campagne.OutilsActif==3);
}

void CCinemaView::OnUpdateOutilsTempo(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetDocument()->m_Campagne.OutilsActif==2);
}


#include "dlgchangerperiode.h"
void CCinemaView::OnParamtresExercice() 
{
	CDlgChangerPeriode dlg;
	dlg.m_DateDebut=GetDocument()->m_Campagne.m_DateDebut;
	dlg.m_NbSemaine=GetDocument()->m_Campagne.m_NbSemaine;
	if(dlg.DoModal()!=IDOK)return ;
	GetDocument()->m_Campagne.SetExercice(dlg.m_DateDebut,dlg.m_NbSemaine);

	GetDocument()->m_Campagne.CalculPassageACCOGRIL();
	m_Grille.InitDessin();
	Invalidate();
	GetDocument()->m_Campagne.LanceCalcul(CCampagne::HYPOTHESES);
}

void CCinemaView::OnGraphe() 
{
	CCampagne * C=&GetDocument()->m_Campagne;
	if(GetDocument()->m_DlgGraphe.IsWindowVisible()==0)
	{
		// CALCUL FRED
		// on n'a pas besoin de tout calculer, le calcul de l'hyp active et valide suffit
		C->LanceCalcul(CCampagne::TOUTHYPOTHESEVALIDE);
		C->LanceCalcul(CCampagne::TOUTHYPOTHESEACTIVEIFNOTVALIDE);
		// MODIF FRED: 20/06/2002 : suppression du bouton panorama
		// if(m_BtnPanorama.GetBtnState())m_BtnPanorama.SetBtnState(0);
		GetDocument()->m_DlgGraphe.MajAffichageACCOGRAP();
		GetDocument()->m_DlgGraphe.ShowWindow(SW_SHOWNORMAL);
	}
	else 
	{
		GetDocument()->m_DlgGraphe.ShowWindow(SW_HIDE);
	}
}



void CCinemaView::OnEffacer() 
{
	CCampagne * C;
	C=&GetDocument()->m_Campagne;

	int h=C->m_HypotheseActive;
	C->Efface(h);
	
	C->CalculPassageACCOGRIL();
	m_Grille.Invalidate();
	m_NbPassages.Invalidate();
	C->LanceCalcul(CCampagne::BUDGETPERF);
}

#include "dlgoptioncalcul.h"
void CCinemaView::OnParamtresOptiondecalcul() 
{

	CCampagne * C;
	C=&GetDocument()->m_Campagne;
	CDlgOptionCalcul dlg;
	
	
	if(C->CalculDememo)dlg.m_Dememorisation=0;
	else dlg.m_Dememorisation=1;

	if(C->fPiloteDurees)dlg.m_DureeTouteActive=0;
	else dlg.m_DureeTouteActive=1;

	if(C->CalculSaisonnalite)dlg.m_Saisonnalite=0;
	else dlg.m_Saisonnalite=1;

	if(C->RedressementCNC)dlg.m_CNCActif_Inactif=0;
	else dlg.m_CNCActif_Inactif=1;

	// Parametres strates sous cible agglomérations 
	dlg.m_RadioAgglo=C->NumeroModaliteAgglo-3;

	// CALCUL FRED
	if(!C->BaseCalcul)
	{
		dlg.m_BaseRegionale=1;
	}
	else 
	{ 
		dlg.m_BaseRegionale=0;
	}
	
	if(dlg.DoModal()!=IDOK)return;

	if(dlg.m_Saisonnalite)C->CalculSaisonnalite=0;
	else C->CalculSaisonnalite=1;

	if(dlg.m_DureeTouteActive)C->fPiloteDurees=0;
	else C->fPiloteDurees=1;

	if(dlg.m_Dememorisation)C->CalculDememo=0;
	else C->CalculDememo=1;

	if(dlg.m_CNCActif_Inactif==0)C->RedressementCNC=1;
	else C->RedressementCNC=0;
	// CALCUL FRED

	if(dlg.m_BaseRegionale)C->BaseCalcul=0;
	else C->BaseCalcul=2;

	C->NumeroModaliteAgglo=dlg.m_RadioAgglo+3;


	// changement de la base de calcul
	C->LanceCalcul(CCampagne::CIBLEUTILE);
	C->LanceCalcul(CCampagne::HYPOTHESESPOP);
}

#include "dlgbudgetnet.h"
void CCinemaView::OnParamtresBudgetnet() 
{
	CCampagne * C=&GetDocument()->m_Campagne;
	CDlgBudgetNet dlg;
	dlg.m_BudgetNet=C->m_Resultat.BudgetNet[C->m_HypotheseActive];
	if(dlg.DoModal()!=IDOK)return;
	C->m_Resultat.BudgetNet[C->m_HypotheseActive]=dlg.m_BudgetNet;
	C->LanceCalcul(CCampagne::HYPOTHESES);
}

#include "dlgfonctioncible.h"
void CCinemaView::OnParamtresCible1() 
{
	CCampagne * C=&GetDocument()->m_Campagne;

	CDlgFonctionCible dlg;
	dlg.m_Mode=0;
	if(dlg.DoModal()!=IDOK)return ;
	C->m_NumCible[0]=dlg.m_NumCible;
	C->m_LibCible[0]=dlg.m_Nom;

	C->ChargeCible(C->m_NumCible[0],0);
	C->LanceCalcul(CCampagne::CIBLEUTILE);
	C->LanceCalcul(CCampagne::HYPOTHESESPOP);
}

#include "dlgbeta.h"
void CCinemaView::OnParamtresBta() 
{
	CCampagne * C=&GetDocument()->m_Campagne;
	CDlgBeta dlg;
	dlg.m_Beta=C->m_Beta;
	if(dlg.DoModal()!=IDOK)return ;
	C->m_Beta=dlg.m_Beta;
	C->CalculCourbeReponse();
	C->LanceCalcul(CCampagne::HYPOTHESES);
}

void CCinemaView::InitBoutonDuree()
{
	CCampagne * C=&GetDocument()->m_Campagne;
	m_BtnD1.SetWindowText(C->m_LibFormat[0]);

	if(C->m_NbFormat>=2)
	{
		m_BtnD2.SetWindowText(C->m_LibFormat[1]);
		m_BtnD2.EnableWindow(1);
	}
	else 
	{
		m_BtnD2.SetWindowText("");
		m_BtnD2.EnableWindow(0);
	}

	if(C->m_NbFormat>=3)
	{
		m_BtnD3.SetWindowText(C->m_LibFormat[2]);
		m_BtnD3.EnableWindow(1);
	}
	else 
	{
		m_BtnD3.SetWindowText("");
		m_BtnD3.EnableWindow(0);
	}

	if(C->m_NbFormat>=4)
	{
		m_BtnD4.SetWindowText(C->m_LibFormat[3]);
		m_BtnD4.EnableWindow(1);
	}
	else 
	{
		m_BtnD4.SetWindowText("");
		m_BtnD4.EnableWindow(0);
	}

	if(C->m_NbFormat>=5)
	{
		m_BtnD5.SetWindowText(C->m_LibFormat[4]);
		m_BtnD5.EnableWindow(1);
	}
	else 
	{
		m_BtnD5.SetWindowText("");
		m_BtnD5.EnableWindow(0);
	}
}

void CCinemaView::SetStateBoutonDuree()
{
	CCampagne * C=&GetDocument()->m_Campagne;

	m_BtnD1.SetBtnState(C->m_FlagFormat[0]);
	if(C->m_NbFormat>=2)m_BtnD2.SetBtnState(C->m_FlagFormat[1]);
	if(C->m_NbFormat>=3)m_BtnD3.SetBtnState(C->m_FlagFormat[2]);
	if(C->m_NbFormat>=4)m_BtnD4.SetBtnState(C->m_FlagFormat[3]);
	if(C->m_NbFormat>=5)m_BtnD5.SetBtnState(C->m_FlagFormat[4]);
	m_BtnD1.UpdateWindow();
	m_BtnD2.UpdateWindow();
	m_BtnD3.UpdateWindow();
	m_BtnD4.UpdateWindow();
	m_BtnD5.UpdateWindow();

}

void CCinemaView::OnDuree1() 
{
	CCampagne * C=&GetDocument()->m_Campagne;
	if(m_BtnD1.GetBtnState()&1)
	{
		C->m_FlagFormat[0]=3;
		C->m_FlagFormat[1]=0;
		C->m_FlagFormat[2]=0;
		C->m_FlagFormat[3]=0;
		C->m_FlagFormat[4]=0;
	}
	else 
	{
		C->m_FlagFormat[0]=m_BtnD1.GetBtnState();
	}
	SetStateBoutonDuree();
	C->UpdateMap(0); 
	m_Grille.Invalidate();
	m_Grille.UpdateWindow();
	C->CalculPassageACCOGRIL();
	m_NbPassages.Invalidate();
	C->LanceCalcul(CCampagne::HYPOTHESEFORMATACTIF);
	
}

void CCinemaView::OnDuree2() 
{
	CCampagne * C=&GetDocument()->m_Campagne;
	if(m_BtnD2.GetBtnState()&1)
	{
		C->m_FlagFormat[0]=0;
		C->m_FlagFormat[1]=3;
		C->m_FlagFormat[2]=0;
		C->m_FlagFormat[3]=0;
		C->m_FlagFormat[4]=0;
	}
	else 
	{
		C->m_FlagFormat[1]=m_BtnD2.GetBtnState();
	}
	SetStateBoutonDuree();
	C->UpdateMap(0); 
	m_Grille.Invalidate();
	m_Grille.UpdateWindow();
	C->CalculPassageACCOGRIL();
	m_NbPassages.Invalidate();
	C->LanceCalcul(CCampagne::HYPOTHESEFORMATACTIF);
}

void CCinemaView::OnDuree3() 
{
	CCampagne * C=&GetDocument()->m_Campagne;
	if(m_BtnD3.GetBtnState()&1)
	{
		C->m_FlagFormat[0]=0;
		C->m_FlagFormat[1]=0;
		C->m_FlagFormat[2]=3;
		C->m_FlagFormat[3]=0;
		C->m_FlagFormat[4]=0;
	}
	else 
	{
		C->m_FlagFormat[2]=m_BtnD3.GetBtnState();
	}
	SetStateBoutonDuree();
	C->UpdateMap(0); 
	m_Grille.Invalidate();
	m_Grille.UpdateWindow();
	C->CalculPassageACCOGRIL();
	m_NbPassages.Invalidate();
	C->LanceCalcul(CCampagne::HYPOTHESEFORMATACTIF);
}

void CCinemaView::OnDuree4() 
{
	CCampagne * C=&GetDocument()->m_Campagne;
	if(m_BtnD4.GetBtnState()&1)
	{
		C->m_FlagFormat[0]=0;
		C->m_FlagFormat[1]=0;
		C->m_FlagFormat[2]=0;
		C->m_FlagFormat[3]=3;
		C->m_FlagFormat[4]=0;
	}
	else 
	{
		C->m_FlagFormat[3]=m_BtnD4.GetBtnState();
	}
	SetStateBoutonDuree();
	C->UpdateMap(0); 
	m_Grille.Invalidate();
	m_Grille.UpdateWindow();
	C->CalculPassageACCOGRIL();
	m_NbPassages.Invalidate();
	C->LanceCalcul(CCampagne::HYPOTHESEFORMATACTIF);
}

void CCinemaView::OnDuree5() 
{
	CCampagne * C=&GetDocument()->m_Campagne;
	if(m_BtnD5.GetBtnState()&1)
	{
		C->m_FlagFormat[0]=0;
		C->m_FlagFormat[1]=0;
		C->m_FlagFormat[2]=0;
		C->m_FlagFormat[3]=0;
		C->m_FlagFormat[4]=3;
	}
	else 
	{
		C->m_FlagFormat[4]=m_BtnD5.GetBtnState();
	}
	SetStateBoutonDuree();
	C->UpdateMap(0); 
	m_Grille.Invalidate();
	m_Grille.UpdateWindow();
	C->CalculPassageACCOGRIL();
	m_NbPassages.Invalidate();
	C->LanceCalcul(CCampagne::HYPOTHESEFORMATACTIF);
}


void CCinemaView::OnActionValider() 
{
	CCampagne * C=&GetDocument()->m_Campagne;

	C->m_HypotheseValide=C->m_HypotheseActive;
	SetStateBoutonHypothese();
	GetDocument()->m_DlgPerf.MajAffichagePerf();
}

void CCinemaView::SetStateBoutonHypothese()
{
	CCampagne * C=&GetDocument()->m_Campagne;
	// MODIF FRED : 20/06/2002: modification de la vue
	m_BtnH1.SetWindowText((C->m_HypotheseValide==0)?"OK":"Hypothèse1");
	m_BtnH2.SetWindowText((C->m_HypotheseValide==1)?"OK":"Hypothèse2");
	m_BtnH3.SetWindowText((C->m_HypotheseValide==2)?"OK":"Hypothèse3");
	m_BtnH4.SetWindowText((C->m_HypotheseValide==3)?"OK":"Hypothèse4");
	m_BtnH5.SetWindowText((C->m_HypotheseValide==4)?"OK":"Hypothèse5");

	m_BtnH1.SetBtnState(C->m_VisuHypothese[0]+(C->m_HypotheseActive==0?1:0));
	m_BtnH2.SetBtnState(C->m_VisuHypothese[1]+(C->m_HypotheseActive==1?1:0));
	m_BtnH3.SetBtnState(C->m_VisuHypothese[2]+(C->m_HypotheseActive==2?1:0));
	m_BtnH4.SetBtnState(C->m_VisuHypothese[3]+(C->m_HypotheseActive==3?1:0));
	m_BtnH5.SetBtnState(C->m_VisuHypothese[4]+(C->m_HypotheseActive==4?1:0));

	m_BtnH1.UpdateWindow();
	m_BtnH2.UpdateWindow();
	m_BtnH3.UpdateWindow();
	m_BtnH4.UpdateWindow();
	m_BtnH5.UpdateWindow();

}

void CCinemaView::OnHypothese1() 
{
	CCampagne * C=&GetDocument()->m_Campagne;
	if(m_BtnH1.GetBtnState()&1)
	{
		C->m_VisuHypothese[0]=2;
		C->m_VisuHypothese[1]=0;
		C->m_VisuHypothese[2]=0;
		C->m_VisuHypothese[3]=0;
		C->m_VisuHypothese[4]=0;
		C->m_HypotheseActive=0;
	}
	else 
	{
		
		C->m_VisuHypothese[0]=m_BtnH1.GetBtnState();
	}
	
	SetStateBoutonHypothese();

	C->UpdateMap(1);
	C->CalculPassageACCOGRIL();
	m_Grille.Invalidate();
	m_Grille.UpdateWindow();
	m_NbPassages.Invalidate();
	C->LanceCalcul(CCampagne::BUDGETPERF);
}

void CCinemaView::OnHypothese2() 
{
	CCampagne * C=&GetDocument()->m_Campagne;

	if(m_BtnH2.GetBtnState()&1)
	{
		C->m_VisuHypothese[0]=0;
		C->m_VisuHypothese[1]=2;
		C->m_VisuHypothese[2]=0;
		C->m_VisuHypothese[3]=0;
		C->m_VisuHypothese[4]=0;
		C->m_HypotheseActive=1;
	}
	else 
	{
		
		C->m_VisuHypothese[1]=m_BtnH2.GetBtnState();
	}
	SetStateBoutonHypothese();
	C->UpdateMap(1);
	C->CalculPassageACCOGRIL();
	m_Grille.Invalidate();
	m_Grille.UpdateWindow();
	m_NbPassages.Invalidate();
	C->LanceCalcul(CCampagne::BUDGETPERF);
}

void CCinemaView::OnHypothese3() 
{
	CCampagne * C=&GetDocument()->m_Campagne;
	if(m_BtnH3.GetBtnState()&1)
	{
		C->m_VisuHypothese[0]=0;
		C->m_VisuHypothese[1]=0;
		C->m_VisuHypothese[2]=2;
		C->m_VisuHypothese[3]=0;
		C->m_VisuHypothese[4]=0;
		C->m_HypotheseActive=2;
	}
	else 
	{
		
		C->m_VisuHypothese[2]=m_BtnH3.GetBtnState();
	}
	
	SetStateBoutonHypothese();
	C->UpdateMap(1);
	C->CalculPassageACCOGRIL();
	m_Grille.Invalidate();
	m_Grille.UpdateWindow();
	m_NbPassages.Invalidate();
	C->LanceCalcul(CCampagne::BUDGETPERF);
}

void CCinemaView::OnHypothese4() 
{
	CCampagne * C=&GetDocument()->m_Campagne;
	if(m_BtnH4.GetBtnState()&1)
	{
		C->m_VisuHypothese[0]=0;
		C->m_VisuHypothese[1]=0;
		C->m_VisuHypothese[2]=0;
		C->m_VisuHypothese[3]=2;
		C->m_VisuHypothese[4]=0;
		C->m_HypotheseActive=3;
	}
	else 
	{
		
		C->m_VisuHypothese[3]=m_BtnH4.GetBtnState();
	}
	
	SetStateBoutonHypothese();
	C->UpdateMap(1);
	C->CalculPassageACCOGRIL();
	m_Grille.Invalidate();
	m_Grille.UpdateWindow();
	m_NbPassages.Invalidate();
	C->LanceCalcul(CCampagne::BUDGETPERF);
}

void CCinemaView::OnHypothese5() 
{
	CCampagne * C=&GetDocument()->m_Campagne;
	if(m_BtnH5.GetBtnState()&1)
	{
		C->m_VisuHypothese[0]=0;
		C->m_VisuHypothese[1]=0;
		C->m_VisuHypothese[2]=0;
		C->m_VisuHypothese[3]=0;
		C->m_VisuHypothese[4]=2;
		C->m_HypotheseActive=4;
	}
	else 
	{
		
		C->m_VisuHypothese[4]=m_BtnH5.GetBtnState();
	}
	
	SetStateBoutonHypothese();
	C->UpdateMap(1);
	C->CalculPassageACCOGRIL();
	m_Grille.Invalidate();
	m_Grille.UpdateWindow();
	m_NbPassages.Invalidate();
	C->LanceCalcul(CCampagne::BUDGETPERF);
}



void CCinemaView::OnAffichageTest() 
{
	CCampagne * C=&GetDocument()->m_Campagne;
	
	CString txt;
	txt.Format("Contacts %.0f",1000*C->m_Resultat.NbContactTot[0][0]);
	MessageBox(txt);
}

void CCinemaView::OnActionCopier() 
{
	CCampagne * C=&GetDocument()->m_Campagne;
	C->CopierHypotheseActive(-1);

}

void CCinemaView::OnActionColler() 
{
	CCampagne * C=&GetDocument()->m_Campagne;
	C->CollerHypotheseActive();
	Invalidate();
}

void CCinemaView::OnUpdateActionColler(CCmdUI* pCmdUI) 
{
	CCampagne * C=&GetDocument()->m_Campagne;
	pCmdUI->Enable(C->fPressePapier);
}

void CCinemaView::OnActionRecopierdanslhypotheseactiveLhypothse1() 
{
	CCampagne * C=&GetDocument()->m_Campagne;
	C->CopierHypotheseActive(0);
	Invalidate();
}

void CCinemaView::OnActionRecopierdanslhypotheseactiveLhypothse2() 
{
	CCampagne * C=&GetDocument()->m_Campagne;
	C->CopierHypotheseActive(1);
	Invalidate();
}

void CCinemaView::OnActionRecopierdanslhypotheseactiveLhypothse3() 
{
	CCampagne * C=&GetDocument()->m_Campagne;
	C->CopierHypotheseActive(2);
	Invalidate();
}

void CCinemaView::OnActionRecopierdanslhypotheseactiveLhypothse4() 
{
	CCampagne * C=&GetDocument()->m_Campagne;
	C->CopierHypotheseActive(3);
	Invalidate();
}

void CCinemaView::OnActionRecopierdanslhypotheseactiveLhypothse5() 
{
	CCampagne * C=&GetDocument()->m_Campagne;
	C->CopierHypotheseActive(4);
	Invalidate();
}

void CCinemaView::OnButton4() 
{
	// C'est le bouton cine park (ex binôme) qui se trouve dans la grille
	CCampagne * C=&GetDocument()->m_Campagne;
	for(int b=0;b<C->m_Selection.GetSize();b++)if(!C->m_Selection.SelectBinome[b])break;

	if(b<C->m_Selection.GetSize())for(b=0;b<C->m_Selection.GetSize();b++)C->m_Selection.SelectBinome[b]=1;

	else for(b=0;b<C->m_Selection.GetSize();b++)C->m_Selection.SelectBinome[b]=0;
	m_Binome.Invalidate();

	C->LanceCalcul(CCampagne::HYPOTHESESPOP);
}

void CCinemaView::OnCampagneExporter() 
{
	// on copie dans le vrai presse papier!
	CCampagne * C=&GetDocument()->m_Campagne;
	C->LanceCalcul(CCampagne::TOUT);

	C->CopierDansPressePapier();
	MessageBox("La campagne est exportée dans le presse-papier.");
}


void CCinemaView::OnFilePrintPreview() 
{
	if(OpenDialogPrint())
	{
		PrepareCalculPrint();
		CView::OnFilePrintPreview();
	}
}

void CCinemaView::OnFilePrint2() 
{
	if(OpenDialogPrint())
	{
		PrepareCalculPrint();
		OnFilePrint();
	}
}


void CCinemaView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CCampagne * C=&GetDocument()->m_Campagne;
	if(pScrollBar==&m_ScrollH)
	{
		HWND h=m_ScrollH.m_hWnd;
		int i, nb, mini, maxi;


		i=::GetScrollPos(h,SB_CTL);
		nb=nPos;
		::GetScrollRange(h,SB_CTL,&mini,&maxi);

		switch(nSBCode) {
			case SB_BOTTOM: 	::SetScrollPos(h,SB_CTL,maxi,1); break;
			case SB_ENDSCROLL: 	break;
			case SB_LINEDOWN: 	::SetScrollPos(h,SB_CTL,min(i+1,maxi),1); break;
			case SB_LINEUP:		::SetScrollPos(h,SB_CTL,max(i-1,mini),1); break;
			case SB_PAGEDOWN:	::SetScrollPos(h,SB_CTL,min(i+(C->m_NbColone),maxi),1); break;
			case SB_PAGEUP:		::SetScrollPos(h,SB_CTL,max(i-(C->m_NbColone),mini),1); break;
			case SB_TOP:		::SetScrollPos(h,SB_CTL,mini,1); break;

			case SB_THUMBPOSITION:
			case SB_THUMBTRACK:	::SetScrollPos(h,SB_CTL,nb,1); break;
		}

		// au besoin on redessine le bouton à qui il est associé
		if((C->m_PosAscH=::GetScrollPos(h,SB_CTL))==i)return;
		if(C->fBloc){
			OffsetRect(&C->RDebut,(i-C->m_PosAscH)*C->m_LargColone,0);
			OffsetRect(&C->RBloc,(i-C->m_PosAscH)*C->m_LargColone,0);
		}
		m_Calendrier.Invalidate();
		m_Grille.Invalidate();
		m_Calendrier.UpdateWindow();
		m_Grille.UpdateWindow();
	}
	else
	{
		CFormView::OnHScroll(nSBCode, nPos, pScrollBar);
	}
	
}

void CCinemaView::OnParamtresCible2() 
{
/*	CCampagne * C=&GetDocument()->m_Campagne;

	CDlgFonctionCible dlg;
	dlg.m_Mode=4;
	if(dlg.DoModal()!=IDOK)return ;
	C->m_NumCible2=dlg.m_NumCible;
	C->m_LibCible2=dlg.m_Nom;

	if(C->m_NumCible2>=0)C->ChargeCible(C->m_NumCible2,1);
	C->LanceCalcul(-5);


	if(C->m_NumCible2>=0)
	{
		if(GetDocument()->m_DlgPerf.IsWindowVisible())
		{
			GetDocument()->m_DlgPerf.ShowWindow(SW_HIDE);
			GetDocument()->m_DlgPerf2.ShowWindow(SW_SHOWNORMAL);
		}
	}
	else
	{
		if(GetDocument()->m_DlgPerf2.IsWindowVisible())
		{
			GetDocument()->m_DlgPerf2.ShowWindow(SW_HIDE);
			GetDocument()->m_DlgPerf.ShowWindow(SW_SHOWNORMAL);
		}
	}
	*/
}
 
// gestion de l'ascenseur vertical

void CCinemaView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar)
{
	CCampagne * C=&GetDocument()->m_Campagne;

	if(pScrollBar==&m_ScrollV)
	{
		HWND h=m_ScrollV.m_hWnd;
		int i, nb, mini, maxi;


		i=::GetScrollPos(h,SB_CTL);
		nb=nPos;
		::GetScrollRange(h,SB_CTL,&mini,&maxi);
	

		switch(nSBCode) {
			case SB_BOTTOM: 	::SetScrollPos(h,SB_CTL,maxi,1); break;
			case SB_ENDSCROLL: 	break;
			case SB_LINEDOWN: 	::SetScrollPos(h,SB_CTL,min(i+1,maxi),1); break;
			case SB_LINEUP:		::SetScrollPos(h,SB_CTL,max(i-1,mini),1); break;
			case SB_PAGEDOWN:	::SetScrollPos(h,SB_CTL,min(i+(C->m_NbLigne),maxi),1); break;
			case SB_PAGEUP:		::SetScrollPos(h,SB_CTL,max(i-(C->m_NbLigne),mini),1); break;
			case SB_TOP:		::SetScrollPos(h,SB_CTL,mini,1); break;
			case SB_THUMBPOSITION: break;
			case SB_THUMBTRACK:	::SetScrollPos(h,SB_CTL,nb,1); break;
		}

		// au besoin on redessine le bouton à qui il est associé
		if((C->m_PosAscV=::GetScrollPos(h,SB_CTL))==i)return;
		if(C->fBloc){
			OffsetRect(&C->RDebut,(i-C->m_PosAscV)*C->m_HautLigne,0);
			OffsetRect(&C->RBloc,(i-C->m_PosAscV)*C->m_HautLigne,0);
		}
		m_NbPassages.Invalidate();
		m_Grille.Invalidate();
		m_Binome.Invalidate();
		m_NbPassages.UpdateWindow();
		m_Grille.UpdateWindow();
		m_Binome.UpdateWindow();
	}
	else
	{
		CFormView::OnVScroll(nSBCode, nPos, pScrollBar);
	}
}



void CCinemaView::OnParamtresFormat() 
{
	CDlgFormat dlg;
	int p,s;
	// pointeurs temporaires pour le stockage des passage.
	char  *** PassageTemp; // par [Parc][semaine][Format]
	// tableau de chaines temporaires pour stocker les libellés formats
	CStringArray LibFormatTemp;

	CCampagne * C=&GetDocument()->m_Campagne;
	dlg.m_ListFormat.Copy(C->m_LibFormat);
	if(dlg.DoModal()!=IDOK)return ;
	C->FreeCoeffFormat();
	// Récupération des libellés des formats
	LibFormatTemp.Copy(dlg.m_ListFormat);
	
	// Allocation
	PassageTemp = new char**[C->m_Selection.GetNbParcTotal()];
	for(p=0;p<C->m_Selection.GetNbParcTotal();p++)
	{
		PassageTemp[p] = new char*[C->m_NbSemaine];
		for(s=0;s<C->m_NbSemaine;s++)
		{
			PassageTemp[p][s] = new char[dlg.m_ListFormat.GetSize()];
			for(int idxf=0;idxf<dlg.m_ListFormat.GetSize();idxf++)
			{
				// Initialisation
				PassageTemp[p][s][idxf] = 0;

			}
		}

	}
	for(int idxf=0;idxf<dlg.m_ListFormat.GetSize();idxf++)
	{
		if(dlg.m_IndexFormatCampagne.GetAt(idxf)==WORD(-1))continue;
		// Changement de place des passages du format idxfc à idxf
		for(p=0;p<C->m_Selection.GetNbParcTotal();p++)
		{
			for(s=0;s<C->m_NbSemaine;s++)
			{
				if(C->m_Passage[C->m_HypotheseActive][p][s][dlg.m_IndexFormatCampagne.GetAt(idxf)])
				{
					PassageTemp[p][s][idxf] = 1;
				}
			}
		}

	}
	// Remise à zéro des passages de l'hypothese
	for(p=0;p<C->m_Selection.GetNbParcTotal();p++)
	{
		for(s=0;s<C->m_NbSemaine;s++)
		{
			for(int idxf=0;idxf<dlg.m_ListFormat.GetSize();idxf++)
			{
				if(PassageTemp[p][s][idxf])
				{
					// copie des passages
					C->m_Passage[C->m_HypotheseActive][p][s][idxf] = 1;
				}
				else C->m_Passage[C->m_HypotheseActive][p][s][idxf]=0;			
			}
		}
	}
	// Libération mémoire
	for(p=0;p<C->m_Selection.GetNbParcTotal();p++)
	{
		for(s=0;s<C->m_NbSemaine;s++)
		{
			delete[] PassageTemp[p][s];
		}
		delete[] PassageTemp[p];
	}
	delete[] PassageTemp;

	C->m_LibFormat.Copy(dlg.m_ListFormat);
	C->m_NbFormat=C->m_LibFormat.GetSize();
	C->m_FlagFormat.RemoveAll();
	for(int i=0;i<5;i++)
	{
		C->m_FlagFormat.Add(0);
	}
	C->m_FlagFormat[0]=3;
	InitBoutonDuree();
	SetStateBoutonDuree();
	C->CalculCoeffFormat();
	C->UpdateMap(1); 
	m_Grille.Invalidate();			
	m_NbPassages.Invalidate();			
	C->LanceCalcul(CCampagne::BUDGETPERF);			
}

// retourne 1 si OK
bool CCinemaView::OpenDialogPrint()
{
	CCampagne * C=&GetDocument()->m_Campagne;

	// mise à jour des libellés
	m_DlgParamPrint.m_NomCampagne  = C->m_NomCampagne;
	m_DlgParamPrint.m_NomAnnonceur = C->m_NomAnnonceur;
	m_DlgParamPrint.m_NomCentrale  = C->m_NomCentrale;
	m_DlgParamPrint.m_NomProduit   = C->m_NomProduit;

	// l'impression multicible
	m_DlgParamPrint.m_DlgMulticible.m_NumeroCible[0]=C->m_NumCible[0];
	for(int cib=1;cib<NBCIBLECALCUL;cib++)
		m_DlgParamPrint.m_DlgMulticible.m_NumeroCible[cib]=C->m_IndexCibleImprime[cib];

	if (m_DlgParamPrint.DoModal()!=IDOK)return 0;

	C->m_PrintGraph=m_DlgParamPrint.m_Graphe;
	C->m_PrintPerf=m_DlgParamPrint.m_Performances;
	C->m_PrintPerfAgglo=m_DlgParamPrint.m_PerfAgglo;
	C->m_PrintCalendrier=m_DlgParamPrint.m_Calendrier;
	C->m_NomCampagne = m_DlgParamPrint.m_NomCampagne;
	C->m_NomAnnonceur = m_DlgParamPrint.m_NomAnnonceur;
	C->m_NomCentrale = m_DlgParamPrint.m_NomCentrale;
	C->m_NomProduit = m_DlgParamPrint.m_NomProduit;

	for(cib=0;cib<NBCIBLECALCUL;cib++)
		C->m_IndexCibleImprime[cib]=m_DlgParamPrint.m_DlgMulticible.m_NumeroCible[cib];
	// chargement des cibles
//	C->LanceCalcul(CCampagne::MULTICIBLE);
	return 1;
}

bool CCinemaView::PrepareCalculPrint()
{
	CCampagne * C=&GetDocument()->m_Campagne;
	// On calcule sur l'hypothèse active toutes les cibles d'impression
	for(int cib=1;cib<NBCIBLECALCUL;cib++)
	{
		int IndexCible = C->m_IndexCibleImprime[cib];

		C->m_NumCible[cib]=C->m_IndexCibleImprime[cib];
		if(C->m_NumCible[cib]<0)continue;
		C->ChargeCible(C->m_NumCible[cib],cib);
		C->PrepareVecteurIndividus(cib);
		C->CalculPopCibleUtile(cib);
	}
	C->LanceCalcul(CCampagne::TOUTHYPOTHESEACTIVE);

	return 1;
}

bool CCinemaView::UndoPrepareCalculPrint()
{
	CCampagne * C=&GetDocument()->m_Campagne;

	/*
	for(int cib=1;cib<NBCIBLECALCUL;cib++)
	{
		C->m_NumCible[cib]=-1;
	}
	*/

	// faire du ménage sur les cibles 1-n ??

	return 1;
}

void CCinemaView::OnButtonchoix() 
{
	CCampagne * C=&GetDocument()->m_Campagne;				

	CDlgSelection	dlgselect;
	// On transmet les codes de la sélection à la boite
	// On boucle sur les codes de la sélection
	for(int s=0;s<C->m_Selection.GetSize();s++)
	{
		// MODIF FRED : 22/05/2002
		// Il faut transmettre les libellés au lieu des codes!!
		// Les codes sont récupérés dans UpdateListFromLibelle de CDlgSelection
		// dlgselect.AddCode(C->m_Selection.GetCode(s)); 
		CString libelle = C->m_Selection.GetLibelle(s); 
		dlgselect.AddLibelle(libelle);
	}
	if(dlgselect.DoModal()!=IDOK)return;

	// curseur d'attente
	CWaitCursor wait;

	RedrawWindow(NULL,NULL,RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
	// Libération des variables liées aux parcs
	// NB on se met de coté les passages pour les restituer
	char **** VieuxPassage = C->m_Passage;
	C->m_Passage = NULL;
	CStringArray VieuxCodes;
	C->m_Selection.GetAllCode(VieuxCodes);
	

	C->FreeParc();
	C->FreeBudget();
	
	// Libération de la sélection
    C->m_Selection.FreeMemory();
	C->m_Selection.RemoveAll();

	// On récupère les codes et on construit une nouvelle sélection
	for(int idcode=0;idcode<dlgselect.GetNbCode();idcode++)
	{
		CString code;
		code = dlgselect.GetCode(idcode);
		 C->m_Selection.AddCode(code); 
	}

	C->m_Selection.InitTab();
	C->m_Selection.BindAccelerateur();
	C->m_Selection.BindMap();
	C->m_Selection.ConstruitListeBacComplexeActifs();	
	C->AllocParc(); 
	C->AllocBudget();


	// boucle sur toutes les nouvelles lignes
	for(int ligne=0;ligne<C->m_Selection.GetSize();ligne++)
	{
		// On cherche si le code existait déjà dans l'ancienne sélection
		for(int oldligne=0;oldligne<VieuxCodes.GetSize();oldligne++)
		{
			if(VieuxCodes[oldligne]==C->m_Selection.GetCode(ligne))
			{
				// Copie des anciens passages sur les lignes que l'on retrouvent
				// Pour chacune des hypotheses
				for(int h=0;h<NBHYPOTHESE;h++)
				{
					// Pour chacune des semaines
					for(int sem=0;sem<C->m_NbSemaine;sem++)
					{

						// Pour chacun des formats
						for(int f=0;f<NBFORMATMAX;f++)
						{
							C->m_Passage[h][ligne*2][sem][f]     = VieuxPassage[h][oldligne*2][sem][f];
							C->m_Passage[h][(ligne*2)+1][sem][f] = VieuxPassage[h][(oldligne*2)+1][sem][f];

						}
					}
				}

			}
		}

	}
	// Maintenant on peut liberer l'ancien pointeur
	C->FreeParcPassage(VieuxPassage,VieuxCodes.GetSize()*2);
	
	C->SetInit();

	// Mise a jour du map
	C->UpdateMap(1);

	// Recalcul
	if((GetDocument()->m_DlgPerf.IsWindowVisible())||(GetDocument()->m_DlgGraphe.IsWindowVisible()))
	{
		C->LanceCalcul(CCampagne::TOUTHYPOTHESEVALIDE);
		C->LanceCalcul(CCampagne::TOUTHYPOTHESEACTIVEIFNOTVALIDE);
		GetDocument()->m_DlgPerf.MajCouleur();
		GetDocument()->m_DlgPerf.ShowWindow(SW_SHOWNORMAL);
		
	}
	// Mise à jour des ascenseurs,etc...
	C->m_PosAscH=0;
	C->m_hAscH=m_ScrollH.m_hWnd;

	// et vertical
	C->m_PosAscV=0;
	C->m_hAscV=m_ScrollV.m_hWnd;

	m_Grille.InitDessin();
	m_Grille.InitAccelerateur();

	RECT R;
	m_Calendrier.GetWindowRect(&R);
	C->m_HautLigneTitre=(R.bottom-R.top)/2;

	// on doit attendre les données
	C->m_SetEnCours=1;
	C->m_SetEnCours=0;

	C->CalculPassageACCOGRIL();
	C->ChargeCible(C->m_NumCible[0],0);

	// préparation du vecteur
	C->LanceCalcul(CCampagne::CIBLEUTILE);

	RedrawWindow(NULL,NULL,RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
}


void CCinemaView::OnPerformances() 
{
// MODIF AURELIEN : TEST UGRID
//	CTestgrid testgrid(this);;
//	testgrid.DoModal();
//	
	CCampagne * C=&GetDocument()->m_Campagne;
	if(GetDocument()->m_DlgPerf.IsWindowVisible()==0)
	{
		// CALCUL FRED
		// on n'a pas besoin de tout calculer, le calcul de l'hyp active et valide suffit
		C->LanceCalcul(CCampagne::TOUTHYPOTHESEVALIDE);
		C->LanceCalcul(CCampagne::TOUTHYPOTHESEACTIVEIFNOTVALIDE);
		GetDocument()->m_DlgPerf.MajCouleur();
		GetDocument()->m_DlgPerf.ShowWindow(SW_SHOWNORMAL);
		
	}
	else 
	{
		GetDocument()->m_DlgPerf.ShowWindow(SW_HIDE);
	}
}

void CCinemaView::OnPerfagglo() 
{
	CCampagneAgglo * C=&GetDocument()->m_Campagne;
	if(GetDocument()->m_DlgPerfAgglo.IsWindowVisible()==0)
	{
		GetDocument()->m_DlgPerfAgglo.ShowWindow(SW_SHOWNORMAL);
		C->LanceCalcul(CCampagne::TOUTHYPOTHESEACTIVE);
	}
	else 
	{
		GetDocument()->m_DlgPerfAgglo.ShowWindow(SW_HIDE);
	}	
	
}

//////////////////////////////////////////////////////////////////////////////////////////
// Export spécifique pour Médiavision
void CCinemaView::OnExportMediavision() 
{
	// Boite de dialogue paramètres export Médiavision
	CDlgExportMediavision DlgExportM;
	
	// Récupère la campagne
	DlgExportM.m_pCampagne = &GetDocument()->m_Campagne;

	// Après remplissage (vérifier titre/ ss-titre et au moins 1 cible de sélectionner)
	if(DlgExportM.DoModal() == IDOK)
	{
		// Nb Cibles sélectionnées
		int NbCibleSel = DlgExportM.m_TSelCibleExportM.GetSize();

		// Test si export possible (au moins 1 cible sélectionnée)
		// if (DlgExportM.m_Titre != "" && NbCibleSel > 0)   
		if (NbCibleSel > 0)   
		{
			// Remplit info titre et ss-titre si manquantes
			if (DlgExportM.m_Titre	 == "")  DlgExportM.m_Titre		= "Performances de la campagne";
			if (DlgExportM.m_SsTitre == "")  DlgExportM.m_SsTitre	= "Médiavision";

			// Lance l'Export Mediavision
			ExportMediavision(DlgExportM.m_pCampagne, DlgExportM.m_Titre, DlgExportM.m_SsTitre, DlgExportM.m_TSelCibleExportM);

			ExportMediavision(DlgExportM.m_pCampagne, DlgExportM.m_Titre, DlgExportM.m_SsTitre, DlgExportM.m_TSelCibleExportM);

			// Message Fin Export
			AfxMessageBox("Export Médiavision dans le presse-papier.");
		}
		else
			// Manque des informations pour effectuer l'export
			AfxMessageBox("Manque sélection de cibles",MB_ICONEXCLAMATION);
	}		
}

//////////////////////////////////////////////////////////////////////////////////////////
// Mise à jour accés Export spécifique pour Médiavision
void CCinemaView::OnUpdateExportMediavision(CCmdUI* pCmdUI) 
{

	// Alain : Test si Client Mediavision
	int ClientMediavision = atoi(theApp.GetProfileString("Configuration", "Mediavision", "0"));

	// Active ou désactive option export spécifique
	pCmdUI->Enable(ClientMediavision == 1);
}

///////////////////////////////////////////////////////////////////////////////////////////
// Recup libelle du bac associé au libellé complexe
CString CCinemaView::GetLibelleBacCodeComplexe(CString LibComplexe)
{
	// Récup Table des BACS et des Complexes
	CTableBAC *tBac				= theApp.m_TableCinema.GetTableBac();
	CTableComplexe *tComplexe	= theApp.m_TableCinema.GetTableComplexe();
		
	/*
	for (int idxcomplexe = 0; idxcomplexe < tComplexe->GetSize(); idxcomplexe++)
	{
		// CString lib  = tComplexe->GetAt(idxcomplexe).GetLibelle();
		CString codecomplexe = tComplexe->GetAt(idxcomplexe).GetCodeBAC();

		for (int idxbac = 0; idxbac < tBac->GetSize(); idxbac++)
		{ 
			CString code = tBac->GetAt(idxbac).m_CodeComposite;
			if (!strncmp(code,codecomplexe,6))
			{
				CString strbac = tBac->GetAt(idxbac).GetLibelle(); 
			}
			else
			{
				int Ok = 2;
			}
		}
	}
	*/

	// Recherche Code Complexe
	CString CodeComplexe = "";
	CString LibBacAss = "";

	if (tComplexe && tBac)
	{
		for (int idxcomplexe = 0; idxcomplexe < tComplexe->GetSize(); idxcomplexe++)
		{
			if (tComplexe->GetAt(idxcomplexe).GetLibelle() == LibComplexe)
			{
				CodeComplexe = tComplexe->GetAt(idxcomplexe).GetCodeBAC();
				break;
			}
		}

		// Recherche du bac associé
		if (CodeComplexe != "")
		{
			for (int idxbac = 0; idxbac < tBac->GetSize(); idxbac++)
			{ 
				CString code = tBac->GetAt(idxbac).m_CodeComposite;
				if (!strncmp(code,CodeComplexe,6))
				{
					LibBacAss = tBac->GetAt(idxbac).GetLibelle(); 
					break;
				}
			}
		}
	}


	return LibBacAss;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Export spécifique pour Médiavision
void CCinemaView::ExportMediavision(CCampagne *pCampagne, CString Titre, CString SsTitre, CArray <int,int&> &TabIdCible)
{
	// Tabulation
	const CString SepTab = "\t";

	// Fichier temporaire clipboard
	CSharedFile clipb (GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);

	// Entete Information
	CString ClipString		= _T(" ");
	ClipString = "EXPORT ATELIER CINEMA - MEDIAVISION\n"; 
	clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

	// Bloc Infos Générales
	ClipString = "\n@BLOC1\n";
	clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

	// le titre
	ClipString = Titre + "\n";
	clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());
	
	// le ss-titre
	ClipString = SsTitre + "\n";
	clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

	// Debut et fin date campagne
	ClipString = pCampagne->m_DateDebut.Format("%d-%m-%Y") + "\n";
	ClipString.TrimLeft(); 
	clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

	// Date fin
	COleDateTime DateFin;
	DateFin = pCampagne->m_DateDebut + COleDateTimeSpan(pCampagne->m_NbSemaine*7-1,0,0,0);
	ClipString  = DateFin.Format("%d-%m-%Y") + "\n";
	ClipString.TrimLeft(); 
	clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

	// Liste des supports avec intervalle semaine
	ClipString = "\n@BLOC4\n";
	clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());
	ClipString = "Liste Supports\n";
	clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());


	// Recupere le complexe via n° de ligne
	for (int ligne=0; ligne < pCampagne->m_Selection.GetSize(); ligne++)
	{
		// Par defaut ce sont des bacs / complexes
		CString LibelleComplexe = "";
		CString LibelleProduit = "";
		tagBAC bac;

		// Nom bac et complexe pour entete
		CString StrComplexe = "";
		CString StrBac = ""; 


		// Est ce un produit ???
		if(pCampagne->m_Selection.m_TypeParLigne[ligne] == tagSelection::PRODUITFIGE ||
		   pCampagne->m_Selection.m_TypeParLigne[ligne] == tagSelection::PRODUIT)
		{
			// Recup libellé du produit via la  ligne
			int indexProduit=pCampagne->m_Selection.GetIndex(ligne);
			LibelleProduit = theApp.m_TableCinema.GetTableProduit()->GetAt(indexProduit).GetLibelle();
		}
		else
		{
			// Recherche nom du bac via le libellé du complexe
			StrComplexe = pCampagne->m_Selection.GetLibelle(ligne);
			StrBac  = GetLibelleBacCodeComplexe(StrComplexe);

			// Recherche du bac / complexe de la ligne
			/* Je ne comprends pas
			for (int idxproba=0;idxproba<theApp.m_NbBacProbabilise;idxproba++)
			{
				if (pCampagne->m_QuotaBacUsedParLigne != NULL)
				{						
					if (pCampagne->m_QuotaBacUsedParLigne[idxproba][ligne][0]>0)
					{

						// Info bac utilisé
						bac = theApp.m_TableCinema.GetTableBac()->GetAt(idxproba);
						LPCSTR codebac = bac.m_Code;

						int b;
						if(theApp.m_MapIndexBac.Lookup(codebac,b))
						{
							if (pCampagne->m_ListeCpxParLigne[ligne].GetSize() > 0)
							{
								WORD Cpx = pCampagne->m_ListeCpxParLigne[ligne].GetAt(0);
								LibelleComplexe = theApp.m_TableCinema.GetTableComplexe()->GetAt(Cpx).GetLibelle();
							}
						}
						break;
					}
				}					
			}
			*/
		}

		/* Pourquoi une 2eme fois
		for (int idxproba=0;idxproba<theApp.m_NbBacProbabilise;idxproba++)
		{
			if (pCampagne->m_QuotaBacUsedParLigne != NULL)
			{
				if (pCampagne->m_QuotaBacUsedParLigne[idxproba][ligne][0]>0)
				{
					// Info bac utilisé
					bac = theApp.m_TableCinema.GetTableBac()->GetAt(idxproba);
					LPCSTR codebac = bac.m_Code;

					int b;
					if(theApp.m_MapIndexBac.Lookup(codebac,b))
					{
						if (pCampagne->m_ListeCpxParLigne[ligne].GetSize() > 0)
						{
							WORD Cpx = pCampagne->m_ListeCpxParLigne[ligne].GetAt(0);
							LibelleComplexe = theApp.m_TableCinema.GetTableComplexe()->GetAt(Cpx).GetLibelle();
						}
					}
					break;
				}
			}
		}
		*/

		// Initialise map des semaines utilisées
		CMap <int,int,int,int> MapSemUtil;
		MapSemUtil.RemoveAll();


		// Boucle sur les 1/2 parcs
		for (int NrParc = ligne * 2; NrParc <= (ligne * 2) + 1; NrParc++)
		{
			// Boucle sur tous les formats et semaine calendrier courant
			for(int format=0;format<pCampagne->m_NbFormat;format++)
			{
				for (int colsem=0; colsem< pCampagne->m_NbSemaine; colsem++)
				{
					// Test si 1 insertion
					if (pCampagne->m_Passage[pCampagne->m_HypotheseActive][NrParc][colsem][format]!=0)
					{
						if (!MapSemUtil.Lookup(colsem, colsem))
							MapSemUtil.SetAt(colsem, colsem);
					}
				}
			}
		}

		ClipString = "";
		if (MapSemUtil.GetCount())
		{

			if (LibelleProduit == "")
			{
				/*
				// Nom du Bac
				ClipString = bac.GetLibelle();
				// ClipString += "\t";
				ClipString += " - ";

				// Nom du complexe
				ClipString += LibelleComplexe;
				// ClipString += "\t";
				ClipString += " - ";
				*/
				
				// Nom du Bac
				ClipString = StrBac;
				ClipString += " - ";

				// Nom du complexe
				ClipString += StrComplexe;
				ClipString += " - ";
			}
			else
			{
				// Cas d'un produit
				ClipString += LibelleProduit;
				// ClipString += "\t\t";
				ClipString += " - ";
			}
			
			// Nom des semaines utilisées
			int NbBlocSemaine = 0;
			CString IntervalleSemaine = "";
			CString IntervalleNoSemaine = "";
			CString TxtSemActives = "";
			CString TxtNoSems = "";
			bool FlagTitreSemOk = false;

			
			for (int colsem=0; colsem< pCampagne->m_NbSemaine; colsem++)
			{
				if (MapSemUtil.Lookup(colsem, colsem))
				{

					int NoSemaineDebut = pCampagne->m_NrSemaine[pCampagne->m_DebutCalend+colsem];

					// CString NoSemaine;
					// NoSemaine.Format("%d\t",colsem+1);
					// ClipString += NoSemaine;
					
					// Date en clair
					COleDateTime DateDebut;
					DateDebut = pCampagne->m_DateDebut + COleDateTimeSpan(colsem * 7,0,0,0);

					// recherche des semaines actives contigues
					int SemSuiv = colsem;
					while (MapSemUtil.Lookup(SemSuiv, SemSuiv))
					{
						SemSuiv++;
					}
					SemSuiv--;

					if (SemSuiv != colsem)
					{
						// Cree la chaine intervalle date
						COleDateTime DateFin;
						DateFin = pCampagne->m_DateDebut + COleDateTimeSpan(SemSuiv*7 + 6,0,0,0);

						IntervalleSemaine += DateDebut.Format("%d/%m/%Y");
						IntervalleSemaine += "-";
						IntervalleSemaine += DateFin.Format("%d/%m/%Y"); 
						// ClipString  += IntervalleSemaine + "\t";
						// TxtSemActives += IntervalleSemaine + "\t";


						if (FlagTitreSemOk == true)
							TxtSemActives += ", ";

						TxtNoSems.Format("%d", NoSemaineDebut);

						// Forme la chaine des nos de semaines actives
						if (FlagTitreSemOk == false)
							IntervalleNoSemaine += "(semaines " + TxtNoSems;
						else
							IntervalleNoSemaine += TxtNoSems;
						FlagTitreSemOk = true;
							
						IntervalleNoSemaine += " à ";
						TxtNoSems.Format("%d", pCampagne->m_NrSemaine[pCampagne->m_DebutCalend+SemSuiv]);
						IntervalleNoSemaine += TxtNoSems;

						TxtSemActives += IntervalleNoSemaine;

						colsem = SemSuiv;
						IntervalleSemaine="";
						IntervalleNoSemaine = "";


						// 1 bloc semaine actif en +
						NbBlocSemaine++;
					}
					else
					{
						// 1 seule semaine discontinue
						// ClipString  += DateDebut.Format("%d/%m/%Y");
						// TxtSemActives += DateDebut.Format("%d/%m/%Y") + "\t";

						// Forme l'info no de semaine active
						if (FlagTitreSemOk == true)
							TxtSemActives += ", ";

						TxtNoSems.Format("%d", NoSemaineDebut);
						if (FlagTitreSemOk == false)
							IntervalleNoSemaine += "(semaines " + TxtNoSems;
						else
							IntervalleNoSemaine += TxtNoSems;
						
						TxtSemActives += IntervalleNoSemaine;

						FlagTitreSemOk = true;

						IntervalleSemaine="";
						IntervalleNoSemaine ="";

						// 1 bloc semaine actif en +
						NbBlocSemaine++;
					}
				}
			}

			// Ajoute Nb Blocs Semaines actives
			CString TxtNbBloc;
			// TxtNbBloc.Format("%d\t",NbBlocSemaine);
			// ClipString  += TxtNbBloc;

			// Ajoute infos semaines actives
			// ClipString  += TxtSemActives + "\n";
			ClipString  += TxtSemActives + ")\n";

			// Ecriture dans buffer
			clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());
		}
	}


	// Bloc Info Pied de page
	// ID_PIEDPAGE1_POPNAT
	// ID_PIEDPAGE1_BAC_COUVERTS
	// ID_PIEDPAGE2
	ClipString = "\n@BLOC5\n";
	clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

	if (pCampagne->BaseCalcul == 0)
		ClipString.LoadString(ID_PIEDPAGE1_BAC_COUVERTS);
	else
		ClipString.LoadString(ID_PIEDPAGE1_POPNAT);
	

	ClipString += "\n";
	clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

	ClipString.LoadString(ID_PIEDPAGE2);
	ClipString += "\n";
	clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

	ClipString.LoadString(ID_PIEDPAGE3);
	ClipString += "\n";
	clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

	// Bloc Infos Générales
	ClipString = "\n@BLOC2\n";
	clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

	
	// Infos bloc
	ClipString = "Cible" + SepTab + "Effectif" + SepTab + "Nb Interviews" 
				 + SepTab + "Nb entrées" + SepTab + "Couv en indivs" 
				 + SepTab + "Couv cum  en %" + SepTab + "Répétition" + SepTab + "Couv Mémo % \n";
	clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

	// Sauvegarde des cibles à imprimer (bidouille)
	int SaveIndexCibleImprime[NBCIBLECALCUL];
	for(int cib=1; cib<NBCIBLECALCUL; cib++)	
	{
		SaveIndexCibleImprime[cib]= pCampagne->m_IndexCibleImprime[cib];
	}

	// définition des cibles de la campagne et calcul
	for(cib=1; cib<NBCIBLECALCUL; cib++)
	{
		
		int Size = TabIdCible.GetSize();

		if (cib <= Size)
		{
			int Index = TabIdCible[cib-1] + 1;
			pCampagne->m_NumCible[cib] = TabIdCible[cib-1] + 1;
			pCampagne->m_IndexCibleImprime[cib] = Index;
		}
		else
		{
			pCampagne->m_NumCible[cib] = -1;
			pCampagne->m_IndexCibleImprime[cib] = -1;
		}

		if(pCampagne->m_NumCible[cib]<0)
			continue;

		
		int InxCible = pCampagne->m_NumCible[cib];

		///////////////////////////////  Correction problème de décalage 14/02/2006
		// Libelle de la cible
		int max = TabIdCible.GetSize();
		int Indice = TabIdCible[cib-1];
		CString LibCible = theApp.m_Cible.m_LibCible[TabIdCible[cib-1]];

		int size=theApp.m_Cible.m_NumCible.GetSize();  
		for(int cible=0;cible<size;cible++)
		{
			if(theApp.m_Cible.m_LibCible[cible]==LibCible)
			{
				InxCible = theApp.m_Cible.m_NumCible[cible]; 
			}
		}
		////////////////////////////////////////////////////////////////////////

		// pCampagne->Cible[cib].Vecteur = NULL;
		pCampagne->ChargeCible(InxCible,cib);
		pCampagne->PrepareVecteurIndividus(cib);
		pCampagne->CalculPopCibleUtile(cib);
	}

	// Paramètres de calcul	
	pCampagne->m_PrintGraph		 = 0;
	pCampagne->m_PrintPerf		 = 1;
	pCampagne->m_PrintPerfAgglo  = 0;
	pCampagne->m_PrintCalendrier = 0;
	pCampagne->LanceCalcul(CCampagne::TOUTHYPOTHESEACTIVE);

	// Liste des cibles
	for (int InxCib = 1; InxCib <= TabIdCible.GetSize(); InxCib++)
	{
		// Index cible dans tab cible
		int IndiceCible = InxCib;
		// int IndiceCible = TabIdCible[InxCib-1];

		int NbCible = theApp.m_Cible.m_LibCible.GetSize(); 

		// Export libellé cible
		ClipString = theApp.m_Cible.m_LibCible[TabIdCible[InxCib-1]] + SepTab;
		clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

		// Les résultats généraux par cible
		//		-- cible, pop cible, entrées, couv en individus, couv en %, répétition, mémo en %
		//
		double PopCible = pCampagne->CibleUtile[IndiceCible].PopPhy;
		ClipString.Format("%.0f\t",PopCible);

		clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

		// Nombre de cas
		int NbCas = pCampagne->CibleUtile[IndiceCible].NbCas;
		ClipString.Format("%d \t",NbCas);
		clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());
		
		// Nb Entrées ou Nb Contacts
		double NbEntree = 1000.0 * pCampagne->m_Resultat.NbContactTot[IndiceCible][pCampagne->m_HypotheseActive];
		ClipString.Format("%.0f\t",NbEntree);
		clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

		// Couverture en indivs
		double CouvIndiv = 1000 * pCampagne->m_Resultat.NbIndivTouchesTot[IndiceCible][pCampagne->m_HypotheseActive];
		ClipString.Format("%.0f \t",CouvIndiv);
		clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

		// Couverture en %
		double CouvPrct = 100000 * pCampagne->m_Resultat.NbIndivTouchesTot[IndiceCible][pCampagne->m_HypotheseActive]/pCampagne->m_Resultat.BasePopulation[IndiceCible];
		ClipString.Format("%.2f \t",CouvPrct);
		clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

		// Répétition
		double Repet = pCampagne->m_Resultat.NbContactTot[IndiceCible][pCampagne->m_HypotheseActive]/pCampagne->m_Resultat.NbIndivTouchesTot[IndiceCible][pCampagne->m_HypotheseActive];
		ClipString.Format("%.2f \t",Repet);
		clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

		// Mémorisation en %
		double MemoPrct = pCampagne->m_Resultat.MemoNrSemaineX[IndiceCible][pCampagne->m_HypotheseActive];
		ClipString.Format("%.2f \n",MemoPrct);
		clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());
				
	}


	//////////////////////////////// 2ème bloc résultats ///////////////////////////////////
	//
	// Résultats par semaine pour la cible de référence 
	// soit redéfinition générale
	//      -- titre, ss-titre, date début, date fin, cible de référence
	// Bloc Infos Générales
	ClipString = "\n@BLOC3\n";

	clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

	// le titre
	ClipString = Titre + "\n";
	clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());
	
	// le ss-titre
	ClipString = SsTitre + "\n";
	clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

	// Debut et fin date campagne
	ClipString = pCampagne->m_DateDebut.Format("%d-%m-%Y") + "\n";
	ClipString.TrimLeft(); 
	clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

	// Date fin
	DateFin = pCampagne->m_DateDebut + COleDateTimeSpan(pCampagne->m_NbSemaine*7-1,0,0,0);
	ClipString  = DateFin.Format("%d-%m-%Y") + "\n";
	ClipString.TrimLeft(); 
	clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

	// Export libellé cible
	ClipString = theApp.m_Cible.m_LibCible[TabIdCible[0]] + "\n";
	clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

	// + résultats
	//      -- n° semaine, date début semaine, semaine valide, couv cum, couv cum mémorisée
	// Couv cum et couv cum mémo par semaine
	ClipString = "N° semaine" + SepTab + "Début semaine" + SepTab + "Semaine valide" 
		         + SepTab + "GRP/sem" + SepTab + "Couv cumul/sem" + SepTab + "Répet/sem" 
				 + SepTab + "Couv mémo cum/sem \n";
	clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

	COleDateTime DateDebSem;
	for (int NoSemaine = 1; NoSemaine <= pCampagne->m_NbSemaine; NoSemaine++)
	{
		// No de la semaine
		ClipString.Format("%d \t",NoSemaine);
		clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

		// Date début semaine
		DateDebSem = pCampagne->m_DateDebut + COleDateTimeSpan((NoSemaine-1)*7,0,0,0); 
		ClipString  = DateDebSem.Format("%d-%m-%Y") + "\t";
		clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

		// Semaine valide ou pas
		ClipString  = "0\t";
		if (pCampagne->SemaineActive(NoSemaine-1))
		{
			ClipString  = "1\t";
		}
		clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

		// Le GRP cumulé
		double Grp = pCampagne->NbContactsCumulParSemaine[0][pCampagne->m_HypotheseActive][NoSemaine-1+pCampagne->m_DebutCalend]*100.0 / pCampagne->m_Resultat.BasePopulation[0];
		ClipString.Format("%.2f \t",Grp);
		clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());
		
		// Couverture cumulée -- A VOIR
		double CouvCum = pCampagne->CouvTot[0][pCampagne->m_HypotheseActive][NoSemaine-1+pCampagne->m_DebutCalend]*100.0;;
		ClipString.Format("%.2f \t",CouvCum);
		clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

		// Repetition par semaine
		double Repet = 0;
		if (CouvCum > 0) 
			Repet = Grp / CouvCum;
		ClipString.Format("%.2f \t",Repet);
		clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());


		// Couverture cumulée mémo -- A VOIR
		double CouvCumMemo = pCampagne->MemoTot[0][pCampagne->m_HypotheseActive][NoSemaine-1+pCampagne->m_DebutCalend]*100.0;
		ClipString.Format("%.2f\n",CouvCumMemo);
		clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());
	}

	// Restitution des cibles à imprimer
	for(cib=1; cib<NBCIBLECALCUL; cib++)
	{
		pCampagne->m_IndexCibleImprime[cib] = SaveIndexCibleImprime[cib];
		pCampagne->m_NumCible[cib] = pCampagne->m_IndexCibleImprime[cib];
	}

	// Test opération Clipboard
	VERIFY(OpenClipboard());
	BOOL bEmpty = EmptyClipboard();
	VERIFY(CloseClipboard());

	// Lance la copie clipboard
	HGLOBAL hData = 0;
	HANDLE hResult = 0;
	hData = clipb.Detach();

	// Now, hand over memory block to clipboard
	if (bEmpty && hData)
	{
		VERIFY(OpenClipboard());
		hResult = SetClipboardData(CF_TEXT, hData);
		VERIFY(CloseClipboard());
	}
		
}


//////////////////////////////////////////////////////////////////////////////////////////
// Export spécifique pour Screenvision
void CCinemaView::OnExportScreenvision() 
{
	// Boite de dialogue paramètres export Screenvision
	CDlgExportScreenvision DlgExportSV;
	
	// Récupère la campagne
	DlgExportSV.m_pCampagne = &GetDocument()->m_Campagne;

	// Après remplissage (vérifier titre/ ss-titre et au moins 1 cible de sélectionner)
	if(DlgExportSV.DoModal() == IDOK)
	{
		// Nb Cibles sélectionnées
		int NbCibleSel = DlgExportSV.m_TSelCibleExportSV.GetSize();

		// Test si export possible (au moins 1 cible sélectionnée)
		// if (DlgExportSV.m_Titre != "" && NbCibleSel > 0)   
		if (NbCibleSel > 0)   
		{
			if (DlgExportSV.m_Titre != "" && DlgExportSV.m_SsTitre != "")
			{
				// Lance l'Export Mediavision
				ExportScreenvision(DlgExportSV.m_pCampagne, DlgExportSV.m_Titre, DlgExportSV.m_SsTitre, DlgExportSV.m_TSelCibleExportSV, DlgExportSV.m_IndexCiblePerfSem);

				// Message Fin Export
				AfxMessageBox("Export Screenvision dans le presse-papier.");
			}
			else
			{
				// Manque des informations titre ou ss titre
				AfxMessageBox("Manque titre ou ss-titre",MB_ICONEXCLAMATION);
			}
		}
		else
			// Manque des informations pour effectuer l'export
			AfxMessageBox("Manque sélection de cibles",MB_ICONEXCLAMATION);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
// Mise à jour accés Export spécifique pour Screenvision
void CCinemaView::OnUpdateExportScreenvision(CCmdUI* pCmdUI) 
{

	// Alain : Test si Client Mediavision
	int ClientScreenvision = atoi(theApp.GetProfileString("Configuration", "Mediavision", "0"));

	// Active ou désactive option export spécifique (il faut que ce soit client autre que Mediavision)
	pCmdUI->Enable(ClientScreenvision == 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Export spécifique pour Screenvision
void CCinemaView::ExportScreenvision(CCampagne *pCampagne, CString Titre, CString SsTitre, CArray <int,int&> &TabIdCible, int InxCiblePerfSem)
{
	// Tabulation
	const CString SepTab = "\t";

	// Pied de tableau (sera mis à la fin de chaque tableau)
	const CString PiedTab = "Sources : Médiamétrie corrigée par les entrées CNC - JFC, Atelier Cinéma";

	// Fichier temporaire clipboard
	CSharedFile clipb (GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);

	// Entete Information
	CString ClipString		= _T(" ");
	ClipString = "EXPORT ATELIER CINEMA - SCREENVISION\n"; 
	clipb.Write(ClipString.GetBuffer(100), ClipString.GetLength());

	// Bloc Infos Générales
	ClipString = "\n@BLOC1\n";
	clipb.Write(ClipString.GetBuffer(100), ClipString.GetLength());

	// Agence / Agence média
	ClipString = "Agence / Agence média : " + Titre + "\n";
	clipb.Write(ClipString.GetBuffer(100), ClipString.GetLength());
	
	// Produit
	ClipString = "Produit : " + SsTitre + "\n";
	clipb.Write(ClipString.GetBuffer(100), ClipString.GetLength());

	// Rappel dispositif sélectionné
	CString Dispositif;
	Dispositif = "";


	///////////////////////////////// Les supports sélectionnés ////////////////////////////
	// Recupere les infos dispositifs et le mode de parc
	int NbParc100	= 0;
	int NbParc50	= 0;
	int NbParc33	= 0;
	for (int ligne=0; ligne < pCampagne->m_Selection.GetSize(); ligne++)
	{
		// Par defaut ce sont des bacs / complexes
		CString LibelleComplexe = "";
		CString LibelleProduit = "";
		tagBAC bac;

		// Est ce un produit ???
		if(pCampagne->m_Selection.m_TypeParLigne[ligne] == tagSelection::PRODUITFIGE ||
		   pCampagne->m_Selection.m_TypeParLigne[ligne] == tagSelection::PRODUIT)
		{
			// Recup libellé du produit via la  ligne
			int indexProduit=pCampagne->m_Selection.GetIndex(ligne);
			LibelleProduit = theApp.m_TableCinema.GetTableProduit()->GetAt(indexProduit).GetLibelle();
		}
		else
		{
			// Recherche du bac / complexe de la ligne
			for (int idxproba=0;idxproba<theApp.m_NbBacProbabilise;idxproba++)
			{

				if (pCampagne->m_QuotaBacUsedParLigne != NULL)
				{
					if (pCampagne->m_QuotaBacUsedParLigne[idxproba][ligne][0]>0)
					{
						// Info bac utilisé
						bac = theApp.m_TableCinema.GetTableBac()->GetAt(idxproba);
						LPCSTR codebac = bac.m_Code;

						int b;
						if(theApp.m_MapIndexBac.Lookup(codebac,b))
						{
							if (pCampagne->m_ListeCpxParLigne[ligne].GetSize() > 0)
							{
								WORD Cpx = pCampagne->m_ListeCpxParLigne[ligne].GetAt(0);
								LibelleComplexe = theApp.m_TableCinema.GetTableComplexe()->GetAt(Cpx).GetLibelle();
							}
						}
						break;
					}
				}
			}
		}

		for (int idxproba=0;idxproba<theApp.m_NbBacProbabilise;idxproba++)
		{
			if (pCampagne->m_QuotaBacUsedParLigne != NULL)
			{
				if (pCampagne->m_QuotaBacUsedParLigne[idxproba][ligne][0]>0)
				{
					// Info bac utilisé
					bac = theApp.m_TableCinema.GetTableBac()->GetAt(idxproba);
					LPCSTR codebac = bac.m_Code;

					int b;
					if(theApp.m_MapIndexBac.Lookup(codebac,b))
					{
						if (pCampagne->m_ListeCpxParLigne[ligne].GetSize() > 0)
						{
							WORD Cpx = pCampagne->m_ListeCpxParLigne[ligne].GetAt(0);
							LibelleComplexe = theApp.m_TableCinema.GetTableComplexe()->GetAt(Cpx).GetLibelle();
						}
					}
					break;
				}
			}
		}
		
		// Initialise map des semaines utilisées
		CMap <int,int,int,int> MapSemUtil;
		MapSemUtil.RemoveAll();

		
		// Boucle sur tous les formats et semaine calendrier courant
		for(int format=0;format<pCampagne->m_NbFormat;format++)
		{
			for (int colsem=0; colsem< pCampagne->m_NbSemaine; colsem++)
			{
				// Boucle sur les 1/2 parcs
				int PrctParc = 0;
				for (int NrParc = ligne * 2; NrParc <= (ligne * 2) + 1; NrParc++)
				{
					// Raz % parc
					if (NrParc % 2 == 0) PrctParc = 0;

					// Test si 1 insertion
					if (pCampagne->m_Passage[pCampagne->m_HypotheseActive][NrParc][colsem][format]!=0)
					{
						// Mise à jour % parc utilisé
						PrctParc += 50;	
						
						// Mise à jour des semaines utilisées
						if (!MapSemUtil.Lookup(colsem, colsem))
							MapSemUtil.SetAt(colsem, colsem);
					}

					// Incrémente les 100% parc et les 50% parc
					if (NrParc % 2 == 1)
					{
						if (PrctParc == 50)
						{
							NbParc50 ++;
						}
						else if (PrctParc == 100)
						{
							NbParc100 ++;
						}
					}
				}
			}
		}

		if (MapSemUtil.GetCount())
		{

			if (LibelleProduit == "")
			{
				// Nom du Bac
				Dispositif += bac.GetLibelle();
				Dispositif += " - ";

				// Nom du complexe
				Dispositif += LibelleComplexe;
			}
			else
			{
				// Cas d'un produit
				Dispositif += LibelleProduit;
			}
			
			// Ecriture dans buffer
			if (ligne != pCampagne->m_Selection.GetSize() - 1)
				Dispositif += " + ";
		}
	}

	// Ecriture dispositif utilisé (complexes / bacs  / produits) 
	ClipString = "Rappel du dispositif : " + Dispositif + "\n";
	ClipString.TrimLeft(); 
	clipb.Write(ClipString.GetBuffer(100), ClipString.GetLength());

	// Rappel période sélectionnée
	CString InfoPeriode;
	COleDateTime DateFin = pCampagne->m_DateDebut + COleDateTimeSpan(pCampagne->m_NbSemaine * 7 - 1,0,0,0);
	InfoPeriode.Format("Sem. %d à Sem. %d (du %s au %s)\n",
		                pCampagne->m_NrSemaine[pCampagne->m_DebutCalend], 
						pCampagne->m_NrSemaine[pCampagne->m_DebutCalend + pCampagne->m_NbSemaine - 1], 
						pCampagne->m_DateDebut.Format("%d/%m/%Y"),
						DateFin.Format("%d/%m/%Y"));
	ClipString = "Rappel de la période : " + InfoPeriode;
	ClipString.TrimLeft(); 
	clipb.Write(ClipString.GetBuffer(100), ClipString.GetLength());

	// Liste des supports avec intervalle semaine (pas demandé pour le moment par ScreenVision)
	/*
	ClipString = "\n@BLOC2\n";
	clipb.Write(ClipString.GetBuffer(100), ClipString.GetLength());
	ClipString = "Liste Supports\n";
	clipb.Write(ClipString.GetBuffer(100), ClipString.GetLength());

	// Recupere le complexe via n° de ligne
	for (int ligne=0; ligne < pCampagne->m_Selection.GetSize(); ligne++)
	{
		// Par defaut ce sont des bacs / complexes
		CString LibelleComplexe = "";
		CString LibelleProduit = "";
		tagBAC bac;

		// Est ce un produit ???
		if(pCampagne->m_Selection.m_TypeParLigne[ligne] == tagSelection::PRODUITFIGE ||
		   pCampagne->m_Selection.m_TypeParLigne[ligne] == tagSelection::PRODUIT)
		{
			// Recup libellé du produit via la  ligne
			int indexProduit=pCampagne->m_Selection.GetIndex(ligne);
			LibelleProduit = theApp.m_TableCinema.GetTableProduit()->GetAt(indexProduit).GetLibelle();
		}
		else
		{
			// Recherche du bac / complexe de la ligne
			for (int idxproba=0;idxproba<theApp.m_NbBacProbabilise;idxproba++)
			{
				if (pCampagne->m_QuotaBacUsedParLigne[idxproba][ligne][0]>0)
				{
					// Info bac utilisé
					bac = theApp.m_TableCinema.GetTableBac()->GetAt(idxproba);
					LPCSTR codebac = bac.m_Code;

					int b;
					if(theApp.m_MapIndexBac.Lookup(codebac,b))
					{
						if (pCampagne->m_ListeCpxParLigne[ligne].GetSize() > 0)
						{
							WORD Cpx = pCampagne->m_ListeCpxParLigne[ligne].GetAt(0);
							LibelleComplexe = theApp.m_TableCinema.GetTableComplexe()->GetAt(Cpx).GetLibelle();
						}
					}
					break;
				}
			}
		}

		for (int idxproba=0;idxproba<theApp.m_NbBacProbabilise;idxproba++)
		{
			if (pCampagne->m_QuotaBacUsedParLigne[idxproba][ligne][0]>0)
			{
				// Info bac utilisé
				bac = theApp.m_TableCinema.GetTableBac()->GetAt(idxproba);
				LPCSTR codebac = bac.m_Code;

				int b;
				if(theApp.m_MapIndexBac.Lookup(codebac,b))
				{
					if (pCampagne->m_ListeCpxParLigne[ligne].GetSize() > 0)
					{
						WORD Cpx = pCampagne->m_ListeCpxParLigne[ligne].GetAt(0);
						LibelleComplexe = theApp.m_TableCinema.GetTableComplexe()->GetAt(Cpx).GetLibelle();
					}
				}
				break;
			}
		}
		
		// Initialise map des semaines utilisées
		CMap <int,int,int,int> MapSemUtil;
		MapSemUtil.RemoveAll();


		// Boucle sur les 1/2 parcs
		for (int NrParc = ligne * 2; NrParc <= (ligne * 2) + 1; NrParc++)
		{
			// Boucle sur tous les formats et semaine calendrier courant
			for(int format=0;format<pCampagne->m_NbFormat;format++)
			{
				for (int colsem=0; colsem< pCampagne->m_NbSemaine; colsem++)
				{
					// Test si 1 insertion
					if (pCampagne->m_Passage[pCampagne->m_HypotheseActive][NrParc][colsem][format]!=0)
					{
						if (!MapSemUtil.Lookup(colsem, colsem))
							MapSemUtil.SetAt(colsem, colsem);
					}
				}
			}
		}

		ClipString = "";
		if (MapSemUtil.GetCount())
		{

			if (LibelleProduit == "")
			{
				// Nom du Bac
				ClipString = bac.GetLibelle();
				// ClipString += "\t";
				ClipString += " - ";

				// Nom du complexe
				ClipString += LibelleComplexe;
				// ClipString += "\t";
				ClipString += " - ";
			}
			else
			{
				// Cas d'un produit
				ClipString += LibelleProduit;
				// ClipString += "\t\t";
				ClipString += " - ";
			}
			
			// Nom des semaines utilisées
			int NbBlocSemaine = 0;
			CString IntervalleSemaine = "";
			CString IntervalleNoSemaine = "";
			CString TxtSemActives = "";
			CString TxtNoSems = "";
			bool FlagTitreSemOk = false;

			
			for (int colsem=0; colsem< pCampagne->m_NbSemaine; colsem++)
			{
				if (MapSemUtil.Lookup(colsem, colsem))
				{

					int NoSemaineDebut = pCampagne->m_NrSemaine[pCampagne->m_DebutCalend+colsem];

					// CString NoSemaine;
					// NoSemaine.Format("%d\t",colsem+1);
					// ClipString += NoSemaine;
					
					// Date en clair
					COleDateTime DateDebut;
					DateDebut = pCampagne->m_DateDebut + COleDateTimeSpan(colsem * 7,0,0,0);

					// recherche des semaines actives contigues
					int SemSuiv = colsem;
					while (MapSemUtil.Lookup(SemSuiv, SemSuiv))
					{
						SemSuiv++;
					}
					SemSuiv--;

					if (SemSuiv != colsem)
					{
						// Cree la chaine intervalle date
						COleDateTime DateFin;
						DateFin = pCampagne->m_DateDebut + COleDateTimeSpan(SemSuiv*7 + 6,0,0,0);

						IntervalleSemaine += DateDebut.Format("%d/%m/%Y");
						IntervalleSemaine += "-";
						IntervalleSemaine += DateFin.Format("%d/%m/%Y"); 
						// ClipString  += IntervalleSemaine + "\t";
						// TxtSemActives += IntervalleSemaine + "\t";


						if (FlagTitreSemOk == true)
							TxtSemActives += ", ";

						TxtNoSems.Format("%d", NoSemaineDebut);

						// Forme la chaine des nos de semaines actives
						if (FlagTitreSemOk == false)
							IntervalleNoSemaine += "(semaines " + TxtNoSems;
						else
							IntervalleNoSemaine += TxtNoSems;
						FlagTitreSemOk = true;
							
						IntervalleNoSemaine += " à ";
						TxtNoSems.Format("%d", pCampagne->m_NrSemaine[pCampagne->m_DebutCalend+SemSuiv]);
						IntervalleNoSemaine += TxtNoSems;

						TxtSemActives += IntervalleNoSemaine;

						colsem = SemSuiv;
						IntervalleSemaine="";
						IntervalleNoSemaine = "";


						// 1 bloc semaine actif en +
						NbBlocSemaine++;
					}
					else
					{
						// 1 seule semaine discontinue
						// ClipString  += DateDebut.Format("%d/%m/%Y");
						// TxtSemActives += DateDebut.Format("%d/%m/%Y") + "\t";

						// Forme l'info no de semaine active
						if (FlagTitreSemOk == true)
							TxtSemActives += ", ";

						TxtNoSems.Format("%d", NoSemaineDebut);
						if (FlagTitreSemOk == false)
							IntervalleNoSemaine += "(semaines " + TxtNoSems;
						else
							IntervalleNoSemaine += TxtNoSems;
						
						TxtSemActives += IntervalleNoSemaine;

						FlagTitreSemOk = true;

						IntervalleSemaine="";
						IntervalleNoSemaine ="";

						// 1 bloc semaine actif en +
						NbBlocSemaine++;
					}
				}
			}

			// Ajoute Nb Blocs Semaines actives
			CString TxtNbBloc;
			// TxtNbBloc.Format("%d\t",NbBlocSemaine);
			// ClipString  += TxtNbBloc;

			// Ajoute infos semaines actives
			// ClipString  += TxtSemActives + "\n";
			ClipString  += TxtSemActives + ")\n";

			// Ecriture dans buffer
			clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());
		}
	}
	*/

	// Info 100% parc, 50% parc, 33% parc, mixte parc 
    if (NbParc50 == 0 && NbParc33 == 0)
		ClipString = "Parc à 100%\n";
	else if (NbParc100 == 0 && NbParc33 == 0)
		ClipString = "Parc à 50%\n";
	else if (NbParc100 == 0 && NbParc50 == 0)
		ClipString = "Parc à 33%\n";
	else
		ClipString = "Mix de parc\n";	

	ClipString.TrimLeft(); 
	clipb.Write(ClipString.GetBuffer(100), ClipString.GetLength());

	/* pas utilisée par screenvision pour le moment
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//											Bloc Info Pied de page
	// ID_PIEDPAGE1_POPNAT
	// ID_PIEDPAGE1_BAC_COUVERTS
	// ID_PIEDPAGE2
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ClipString = "\n@BLOC2\n";
	clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

	if (pCampagne->BaseCalcul == 0)
		ClipString.LoadString(ID_PIEDPAGE1_BAC_COUVERTS);
	else
		ClipString.LoadString(ID_PIEDPAGE1_POPNAT);
	
	ClipString += "\n";
	clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

	ClipString.LoadString(ID_PIEDPAGE2);
	ClipString += "\n";
	clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

	ClipString.LoadString(ID_PIEDPAGE3);
	ClipString += "\n";
	clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());
	*/

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//											Bloc Infos Générales
	//
	// PERFORMANCES DES CIBLES SUR LES AGGLOMERATIONS CONCERNEES
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ClipString = "\n@BLOC2\n";
	clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

	// Infos bloc
	ClipString = "PERFORMANCES DES CIBLES SUR LES AGGLOMERATIONS CONCERNEES (MIGRATION COMPRISE) \n";
	clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

	// Infos bloc
	ClipString = "Cibles" + SepTab + "Pop cible" + SepTab + "Entrées" 
				 + SepTab + "Couverture en milliers" 
				 + SepTab + "Couv en %" + SepTab + "Répétition" + SepTab + "GRP" + SepTab + "Couverture mémorisée en % \n";
	clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

	// Sauvegarde des cibles à imprimer (bidouille)
	int SaveIndexCibleImprime[NBCIBLECALCUL];
	for(int cib=1; cib<NBCIBLECALCUL; cib++)	
	{
		SaveIndexCibleImprime[cib]= pCampagne->m_IndexCibleImprime[cib];
	}

	// définition des cibles de la campagne et calcul
	for(cib=1; cib<NBCIBLECALCUL; cib++)
	{
		
		int Size = TabIdCible.GetSize();

		if (cib <= Size)
		{
			int Index = TabIdCible[cib-1] + 1;
			pCampagne->m_NumCible[cib] = TabIdCible[cib-1] + 1;
			pCampagne->m_IndexCibleImprime[cib] = Index;
		}
		else
		{
			pCampagne->m_NumCible[cib] = -1;
			pCampagne->m_IndexCibleImprime[cib] = -1;
		}

		if(pCampagne->m_NumCible[cib]<0)
			continue;

		
		int InxCible = pCampagne->m_NumCible[cib];

		///////////////////////////////  Correction problème de décalage 14/02/2006
		// Libelle de la cible
		int max = TabIdCible.GetSize();
		int Indice = TabIdCible[cib-1];
		CString LibCible = theApp.m_Cible.m_LibCible[TabIdCible[cib-1]];

		int size=theApp.m_Cible.m_NumCible.GetSize();  
		for(int cible=0;cible<size;cible++)
		{
			if(theApp.m_Cible.m_LibCible[cible]==LibCible)
			{
				InxCible = theApp.m_Cible.m_NumCible[cible]; 
			}
		}
		////////////////////////////////////////////////////////////////////////

		// pCampagne->Cible[cib].Vecteur = NULL;
		pCampagne->ChargeCible(InxCible,cib);
		pCampagne->PrepareVecteurIndividus(cib);
		pCampagne->CalculPopCibleUtile(cib);
	}

	// Paramètres de calcul	
	pCampagne->m_PrintGraph		 = 0;
	pCampagne->m_PrintPerf		 = 1;
	pCampagne->m_PrintPerfAgglo  = 0;
	pCampagne->m_PrintCalendrier = 0;
	pCampagne->LanceCalcul(CCampagne::TOUTHYPOTHESEACTIVE);

	// Liste des cibles
	for (int InxCib = 1; InxCib <= TabIdCible.GetSize(); InxCib++)
	{
		// Index cible dans tab cible
		int IndiceCible = InxCib;
		// int IndiceCible = TabIdCible[InxCib-1];

		int NbCible = theApp.m_Cible.m_LibCible.GetSize(); 

		// Export libellé cible
		ClipString = theApp.m_Cible.m_LibCible[TabIdCible[InxCib-1]] + SepTab;
		clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

		// Les résultats généraux par cible
		//		-- cible, pop cible, entrées, couv en individus, couv en %, répétition, mémo en %
		//
		double PopCible = pCampagne->CibleUtile[IndiceCible].PopPhy;
		ClipString.Format("%.0f\t",PopCible);
		clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

		// Nombre de cas
		// int NbCas = pCampagne->CibleUtile[IndiceCible].NbCas;
		// ClipString.Format("%d \t",NbCas);
		// clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());
		
		// Nb Entrées ou Nb Contacts
		double NbEntree = 1000.0 * pCampagne->m_Resultat.NbContactTot[IndiceCible][pCampagne->m_HypotheseActive];
		ClipString.Format("%.0f\t",NbEntree);
		clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

		// Couverture en indivs
		double CouvIndiv = 1000 * pCampagne->m_Resultat.NbIndivTouchesTot[IndiceCible][pCampagne->m_HypotheseActive];
		ClipString.Format("%.0f \t",CouvIndiv);
		clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

		// Couverture en %
		double CouvPrct = 100000 * pCampagne->m_Resultat.NbIndivTouchesTot[IndiceCible][pCampagne->m_HypotheseActive]/pCampagne->m_Resultat.BasePopulation[IndiceCible];
		ClipString.Format("%.2f \t",CouvPrct);
		clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

		// Répétition
		double Repet = pCampagne->m_Resultat.NbContactTot[IndiceCible][pCampagne->m_HypotheseActive]/pCampagne->m_Resultat.NbIndivTouchesTot[IndiceCible][pCampagne->m_HypotheseActive];
		ClipString.Format("%.2f \t",Repet);
		clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

		// GRP Fin Periode
		double Grp = pCampagne->NbContactsCumulParSemaine[IndiceCible][pCampagne->m_HypotheseActive][pCampagne->m_NbSemaine - 1 + pCampagne->m_DebutCalend]*100.0 / pCampagne->m_Resultat.BasePopulation[IndiceCible];
		ClipString.Format("%.2f \t",Grp);
		clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

		// Mémorisation en %
		double MemoPrct = pCampagne->MemoTot[IndiceCible][pCampagne->m_HypotheseActive][pCampagne->m_NbSemaine - 1] * 100.0;
		ClipString.Format("%.2f \n",MemoPrct);
		clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());
	}

	// Info pied de tableau
	ClipString = PiedTab + "\n";
	clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//											Bloc Infos Performances semaines
	//
	// EVOLUTION SEMAINE PAR SEMAINE DES PREFORMANCES DE LA CIBLE PRINCIPALE
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ClipString = "\n@BLOC3\n";
	clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

	// Infos bloc
	ClipString = "EVOLUTION SEMAINE APRES SEMAINE DES PERFORMANCES\n";
	clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

	// Infos Cible pour les preformances par semaine
	int IndiceCible = InxCiblePerfSem -1;
	CString LibCible = theApp.m_Cible.m_LibCible[IndiceCible + 1];

	ClipString = "Cible sélectionnée :" + LibCible + "\n";
	clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

	// TODO ..............................................
	// Bloc résultats performance semaine
	// Infos bloc
	ClipString = "N° semaine" + SepTab + "Début semaine" + SepTab + "Couv cumul/sem en %" 
				 + SepTab + "Répet cumul/sem" + SepTab + "GRP cumul/sem" 
				 + SepTab + "Couv mémo cumul/sem en %\n";
	clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

    //!!!! Alain (12 Nov 2015)

	// Boucle sur toutes les semaines
	CString TxtNoSem		= "";
	CString TxtDebSem		= "";

	for (int icible = 0; icible < 5; icible++)
	{
		double CouvCum = pCampagne->CouvTot[icible][pCampagne->m_HypotheseActive][pCampagne->m_NbSemaine - 1 + pCampagne->m_DebutCalend]*100.0;
		bool Ok = true;
	}

	for (int colsem=0; colsem< pCampagne->m_NbSemaine; colsem++)
	{
		// No de la semaine
		int NoSemaineDebut = pCampagne->m_NrSemaine[pCampagne->m_DebutCalend+colsem];
		TxtNoSem.Format("%d", NoSemaineDebut);

		// Date début de semaine en clair
		COleDateTime DateDebut;
		DateDebut = pCampagne->m_DateDebut + COleDateTimeSpan(colsem * 7,0,0,0);
		TxtDebSem = DateDebut.Format("%d/%m/%Y");

		// Debut et fin date campagne
		ClipString = TxtNoSem + "\t" + TxtDebSem + "\t"; 
		ClipString.TrimLeft(); 
		clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

		// Les résultats
		//
		//
		// Cumul Couverture en %
		// double CouvCum = pCampagne->CouvTot[IndiceCible + 1][pCampagne->m_HypotheseActive][colsem + pCampagne->m_DebutCalend]*100.0;
		// double CouvCum = pCampagne->CouvTot[InxCiblePerfSem][pCampagne->m_HypotheseActive][colsem + pCampagne->m_DebutCalend]*100.0;
		double CouvCum = pCampagne->CouvTot[1][pCampagne->m_HypotheseActive][colsem + pCampagne->m_DebutCalend]*100.0;
		
		
		ClipString.Format("%.2f \t",CouvCum);
		clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());


		// Recup GRP cumulé
		// double GrpCumSem = pCampagne->NbContactsCumulParSemaine[InxCiblePerfSem][pCampagne->m_HypotheseActive][colsem + pCampagne->m_DebutCalend]*100.0 / pCampagne->m_Resultat.BasePopulation[InxCiblePerfSem];
		double GrpCumSem = pCampagne->NbContactsCumulParSemaine[1][pCampagne->m_HypotheseActive][colsem + pCampagne->m_DebutCalend]*100.0 / pCampagne->m_Resultat.BasePopulation[1];
		// 
		// double GrpCumSem = pCampagne->NbContactsCumulParSemaine[IndiceCible + 1][pCampagne->m_HypotheseActive][colsem + pCampagne->m_DebutCalend]*100.0 / pCampagne->m_Resultat.BasePopulation[IndiceCible + 1];
		// double GrpCumSem = pCampagne->NbContactsCumulParSemaine[0][pCampagne->m_HypotheseActive][colsem + pCampagne->m_DebutCalend]*100.0 / pCampagne->m_Resultat.BasePopulation[IndiceCible + 1];


		// Cumul Répétition
		double RepetCumSem = 0.0;
		if (CouvCum > 0.0)
			RepetCumSem = GrpCumSem / CouvCum;
		ClipString.Format("%.2f \t",RepetCumSem);
		clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

		// ecriture GRP cumulé
		ClipString.Format("%.2f \t",GrpCumSem);
		clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

		// Cumul Mémorisation en %
		// double CouvMemoCumSem = pCampagne->MemoTot[IndiceCible + 1][pCampagne->m_HypotheseActive][colsem] * 100.0;
		// double CouvMemoCumSem = pCampagne->MemoTot[InxCiblePerfSem][pCampagne->m_HypotheseActive][colsem] * 100.0;
		double CouvMemoCumSem = pCampagne->MemoTot[1][pCampagne->m_HypotheseActive][colsem] * 100.0;
		

		ClipString.Format("%.2f \n",CouvMemoCumSem);
		clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());
	}

	// Info pied de tableau
	ClipString = PiedTab + "\n";
	clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//											Bloc Distribution de contacts
	//
	// DISTRIBUTION DE CONTACTS POUR TOUTES LES CIBLES
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ClipString = "\n@BLOC4\n";
	clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());
	
	// Infos bloc
	ClipString = "DISTRIBUTION DES CONTACTS\n";
	clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

	// Bloc entete distribution des contacts
	ClipString = "Couv en %" + SepTab;

	// Entete colonne pour chaque cible
	for (InxCib = 1; InxCib <= TabIdCible.GetSize(); InxCib++)
	{
		// Export libellé cible
		ClipString += theApp.m_Cible.m_LibCible[TabIdCible[InxCib-1]] + SepTab;
	}
	ClipString += "\n";
	clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

	// Boucle sur le bombre de contacts
	int MaxNbContact = 5;
	for (int NbContact = 1; NbContact <= MaxNbContact; NbContact++)
	{
		// Intitule entete ligne (couv pour nb contact en cours)
		if (NbContact == 1)
			ClipString.Format("%s %d %s","Couverture à",NbContact,"contact");
		else
			ClipString.Format("%s %d %s","Couverture à",NbContact,"contacts");
		ClipString += SepTab;
		clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

		// Boucle sur les cibles sélectionnées
		for (InxCib = 1; InxCib <= TabIdCible.GetSize(); InxCib++)
		{

			// Recup couverture pour ce nombre de contact
			double basepop = pCampagne->m_Resultat.BasePopulation[InxCib];
			double couv = pCampagne->NbContactPlaque[InxCib][pCampagne->m_HypotheseActive][pCampagne->m_NbSemaine-1][NbContact-1] * 100.0 / basepop;
			bool Ok = true;

			ClipString.Format("%.2f \t",couv);
			clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());
		}

		// Passe à la ligne nb contact suivante
		ClipString = "\n";
		clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());
	}

	// Info pied de tableau
	ClipString = PiedTab + "\n";
	clipb.Write(ClipString.GetBuffer(100),ClipString.GetLength());

	// Test opération Clipboard
	VERIFY(OpenClipboard());
	BOOL bEmpty = EmptyClipboard();
	VERIFY(CloseClipboard());

	// Lance la copie clipboard
	HGLOBAL hData = 0;
	HANDLE hResult = 0;
	hData = clipb.Detach();

	// Now, hand over memory block to clipboard
	if (bEmpty && hData)
	{
		VERIFY(OpenClipboard());
		hResult = SetClipboardData(CF_TEXT, hData);
		VERIFY(CloseClipboard());
	}

}	

void CCinemaView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}


/*
void CCinemaView::OnCancelMode() 
{
	CFormView::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}

void CCinemaView::OnCaptureChanged(CWnd *pWnd) 
{
	// TODO: Add your message handler code here
	
	CFormView::OnCaptureChanged(pWnd);
}
*/

void CCinemaView::OnMove(int x, int y) 
{
	CFormView::OnMove(x, y);
	
	// TODO: Add your message handler code here
	
}


void CCinemaView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CFormView::OnMouseMove(nFlags, point);
}
