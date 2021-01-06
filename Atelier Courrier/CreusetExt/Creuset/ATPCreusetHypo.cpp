// on inclut les définitions nécessaires
#include "stdafx.h"
#include "ATPCreusetHypo_Metriques.h"
#include "ATPCreusetHypo.h"
#include "MsgPrvCreuset.h"
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur
IMPLEMENT_DYNCREATE(CATPCreusetHypo, JFCSplitterPane)

CATPCreusetHypo::CATPCreusetHypo()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
	m_IdSupportActif = 0;
	m_NoLigSupportActif = 0;
	m_C2 = new CATPCreusetHypo_C2();
	m_E2 = new CATPCreusetHypo_E2();
}

///////////////////////////////////////
// la fonction pour définir le document

JVoid CATPCreusetHypo::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on définit le document des composants
	m_T1.SetDocument(pATPDocCreuset);
	m_A1.SetDocument(pATPDocCreuset);
	m_A2.SetDocument(pATPDocCreuset);
	m_A2.SetParent(this);
	m_A3.SetDocument(pATPDocCreuset);
	//m_B1.SetDocument(pATPDocCreuset);
	//m_B2.SetDocument(pATPDocCreuset);
	//m_B2.SetParent(this);
	//m_B3.SetDocument(pATPDocCreuset);
	m_C1.SetDocument(pATPDocCreuset);
	m_C2->SetDocument(pATPDocCreuset);
	m_C2->SetParent(this);
	m_C3.SetDocument(pATPDocCreuset);

	m_E1.SetDocument(pATPDocCreuset);
	m_E2->SetDocument(pATPDocCreuset);
	m_E2->SetParent(this);
	m_E3.SetDocument(pATPDocCreuset);
	
	// on met à jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

JVoid CATPCreusetHypo::Update()
{
	// on met à jour les composants
	m_T1.Update();
	m_A1.Update();
	m_A2.Update();
	m_A3.Update();
	//m_B1.Update();
	//m_B2.Update();
	//m_B3.Update();
	m_C1.Update();
	m_C2->Update();
	m_C3.Update();

	m_E1.Update();
	m_E2->Update();
	m_E3.Update();
	// on met à jour le composant
	this->OnUpdate();

	LONG x1_T1 = CATPCreusetHypo_Metriques::CX_Border();             LONG x2_T1 = x1_T1 + CATPCreusetHypo_Metriques::CX_Ax();
	LONG y1_T1 = CATPCreusetHypo_Metriques::CY_Border();             LONG y2_T1 = y1_T1 + CATPCreusetHypo_Metriques::CY_T1();
	if (0==1)
		m_T1.SetWindowPos(this,x1_T1,y1_T1, x2_T1-x1_T1, y2_T1-y1_T1, SWP_NOZORDER);

}


/////////////////
// le destructeur

CATPCreusetHypo::~CATPCreusetHypo()
{
	// on ne fait rien
	if(m_C2)
	{
		delete m_C2;
		m_C2 = NULL;
	}
	if(m_E2)
	{
		delete m_E2;
		m_E2 = NULL;
	}
}


/////////////////////////////////////////////////////////////////////////////
// la fonction pour configurer le graphe

JVoid CATPCreusetHypo::SetGraphe(JFControl & Control)
{
	// on configure les acsenseurs
	this->SetScrollBar(&Control, &m_HS, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour récupérer l'ascenseur horizontal

JFCScroll & CATPCreusetHypo::GetHorzScrollBar()
{
	// on renvoie l'ascenseur
	return (m_HS);
}

///////////////////////////////////////////////////////////////////
// les fonctions pour récupérer les dimensions minimales du panneau

LONG CATPCreusetHypo::GetCXMin()
{
	// on calcule la largeur des composants
	LONG CX1 = CATPCreusetHypo_Metriques::CX_Ax() /*+ CATPCreusetHypo_Metriques::CX_Bx()*/ + 2 * CATPCreusetHypo_Metriques::CX_Cx() + CATPCreusetHypo_Metriques::CX_VS();

	// on calcule la largeur des bordures et des interstices
	LONG CX2 = (2 * CATPCreusetHypo_Metriques::CX_Border()) + (4 * CATPCreusetHypo_Metriques::CX_Interstice());

	// on renvoie la largeur
	return (CX1 + CX2);
}

LONG CATPCreusetHypo::GetCYMin()
{
	// on calcule la hauteur des composants
	LONG CY1 = CATPCreusetHypo_Metriques::CY_T1() + CATPCreusetHypo_Metriques::CY_x1() + CATPCreusetHypo_Metriques::CY_x2() + CATPCreusetHypo_Metriques::CY_x3() + CATPCreusetHypo_Metriques::CY_x4();

	// on calcule la largeur des bordures et des interstices
	LONG CY2 = (2 * CATPCreusetHypo_Metriques::CY_Border()) + (4 * CATPCreusetHypo_Metriques::CY_Interstice());

	// on renvoie la hauteur
	return (CY1 + CY2);
}

LONG CATPCreusetHypo::GetCYIdeal()
{
	// on calcule la hauteur des composants
	LONG CY1 = CATPCreusetHypo_Metriques::CY_T1() + CATPCreusetHypo_Metriques::CY_x1() + 5*CATPCreusetHypo_Metriques::CY_x2() + CATPCreusetHypo_Metriques::CY_x3() + CATPCreusetHypo_Metriques::CY_x4();

	// on calcule la largeur des bordures et des interstices
	LONG CY2 = (2 * CATPCreusetHypo_Metriques::CY_Border()) + (4 * CATPCreusetHypo_Metriques::CY_Interstice());

	// on renvoie la hauteur
	return (CY1 + CY2);
}

BEGIN_MESSAGE_MAP(CATPCreusetHypo, JFCSplitterPane)
	//{{AFX_MSG_MAP(CATPCreusetHypo)
	ON_WM_CREATE()
	ON_BN_CLICKED(MSG_CR_EFFACER,	OnBtnEffacer)
	ON_BN_CLICKED(MSG_CR_EFFACERTT,	OnBtnEffacerToute)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////
// la fonction pour appliquer les mises à jour

JVoid CATPCreusetHypo::OnUpdate()
{
	// on ne fait rien
}

//////////////////////////////////
// CATPCreusetHypo message handlers

int CATPCreusetHypo::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	// on appelle le gestionnaire de base
	if (this->JFCSplitterPane::OnCreate(lpCreateStruct) == -1) return (-1);

	// on calcule la position des composants

	// Titre général bloc définition hypothèses
	LONG x1_T1 = CATPCreusetHypo_Metriques::CX_Border();             LONG x2_T1 = x1_T1 + CATPCreusetHypo_Metriques::CX_Ax() /*+ CATPCreusetHypo_Metriques::CX_Bx()*/ + 2 * CATPCreusetHypo_Metriques::CX_Cx() + 3 * CATPCreusetHypo_Metriques::CX_Interstice();
	LONG y1_T1 = CATPCreusetHypo_Metriques::CY_Border();             LONG y2_T1 = y1_T1 + CATPCreusetHypo_Metriques::CY_T1();

	// Titre Bloc Support
	LONG x1_A1 = CATPCreusetHypo_Metriques::CX_Border();             LONG x2_A1 = x1_A1 + CATPCreusetHypo_Metriques::CX_Ax();
	LONG y1_A1 = y2_T1 + CATPCreusetHypo_Metriques::CY_Interstice(); LONG y2_A1 = y1_A1 + CATPCreusetHypo_Metriques::CY_x1();

	// Bloc Supports
	LONG x1_A2 = x1_A1;                                              LONG x2_A2 = x2_A1;
	LONG y1_A2 = y2_A1 + CATPCreusetHypo_Metriques::CY_Interstice(); LONG y2_A2 = y1_A2 + CATPCreusetHypo_Metriques::CY_x2();

	// Titre Total insertions
	LONG x1_A3 = x1_A2;                                              LONG x2_A3 = x2_A2;
	LONG y1_A3 = y2_A2 + CATPCreusetHypo_Metriques::CY_Interstice(); LONG y2_A3 = y1_A3 + CATPCreusetHypo_Metriques::CY_x3();

	// Titre colonne µ
	//LONG x1_B1 = x2_A1 + CATPCreusetHypo_Metriques::CX_Interstice(); LONG x2_B1 = x1_B1 + CATPCreusetHypo_Metriques::CX_Bx();
	//LONG y1_B1 = y1_A1;                                              LONG y2_B1 = y2_A1;

	//// Bloc des µ
	//LONG x1_B2 = x1_B1;                                              LONG x2_B2 = x2_B1;
	//LONG y1_B2 = y1_A2;                                              LONG y2_B2 = y2_A2;

	//// Bloc vide
	//LONG x1_B3 = x1_B2;                                              LONG x2_B3 = x2_B2;
	//LONG y1_B3 = y1_A3;                                              LONG y2_B3 = y2_A3;

	// 
	LONG x1_E1 = x2_A1 + CATPCreusetHypo_Metriques::CX_Interstice(); LONG x2_E1 = x1_E1 + CATPCreusetHypo_Metriques::CX_Cx();
	LONG y1_E1 = y1_A1;                                              LONG y2_E1 = y2_A1;

	// 
	LONG x1_E2 = x1_E1;                                              LONG x2_E2 = x2_E1;
	LONG y1_E2 = y1_A2;                                              LONG y2_E2 = y2_A2;

	// 
	LONG x1_E3 = x1_E2;                                              LONG x2_E3 = x2_E2;
	LONG y1_E3 = y1_A3;                                              LONG y2_E3 = y2_A3;

	// Titre des n° hypothèses
	LONG x1_C1 = x2_E1 + CATPCreusetHypo_Metriques::CX_Interstice(); LONG x2_C1 = x1_C1 + CATPCreusetHypo_Metriques::CX_Cx();
	LONG y1_C1 = y1_E1;                                              LONG y2_C1 = y2_E1;

	// Bloc Insertions par hypothèse
	LONG x1_C2 = x1_C1;                                              LONG x2_C2 = x2_C1;
	LONG y1_C2 = y1_E2;                                              LONG y2_C2 = y2_E2;

	// Bloc Tot insertions par hypothèse
	LONG x1_C3 = x1_C2;                                              LONG x2_C3 = x2_C2;
	LONG y1_C3 = y1_E3;                                              LONG y2_C3 = y2_E3;

	// Positionnement des boutons (Offre/Plateau/Effacer) 
	LONG x1_D3 = x1_A1;												LONG x2_D3 = (x2_E3-x1_A1)/2 - CATPCreusetHypo_Metriques::CX_Interstice()/2;
	LONG y1_D3 = y2_C3 + CATPCreusetHypo_Metriques::CY_Interstice();LONG y2_D3 = y1_D3 + CATPCreusetHypo_Metriques::CY_x4();
	LONG x1_D4 = x2_D3 + CATPCreusetHypo_Metriques::CX_Interstice();LONG x2_D4 = x2_E3;
	LONG y1_D4 = y1_D3;                                             LONG y2_D4 = y2_D3;

	// Ascenseur horizontal
	LONG x1_HS = x1_C3;                                              LONG x2_HS = x2_C3;
	LONG y1_HS = y2_C3 + CATPCreusetHypo_Metriques::CY_Interstice(); LONG y2_HS = y1_HS + CATPCreusetHypo_Metriques::CY_HS();
//	LONG y1_HS = y2_C3 + CATPCreusetHypo_Metriques::CY_Interstice()+ (CATPCreusetHypo_Metriques::CY_x4()-CATPCreusetHypo_Metriques::CY_HS())/2; LONG y2_HS = y1_HS + CATPCreusetHypo_Metriques::CY_HS(); // centré
	// Ascenseur vertical
	LONG x1_VS = x2_C2 + CATPCreusetHypo_Metriques::CX_Interstice(); LONG x2_VS = x1_VS + CATPCreusetHypo_Metriques::CX_VS();
	LONG y1_VS = y1_C2;                                              LONG y2_VS = y2_C2;

	// on crée les composants A1 à C3
	if (m_T1.Create(NULL, WS_VISIBLE, CRect(x1_T1, y1_T1, x2_T1, y2_T1), this, 0) == FALSE) return (-1);
	if (m_A1.Create(NULL, WS_VISIBLE, CRect(x1_A1, y1_A1, x2_A1, y2_A1), this, 0) == FALSE) return (-1);
	if (m_A2.Create(NULL, WS_VISIBLE, CRect(x1_A2, y1_A2, x2_A2, y2_A2), this, 0) == FALSE) return (-1);
	if (m_A3.Create(NULL, WS_VISIBLE, CRect(x1_A3, y1_A3, x2_A3, y2_A3), this, 0) == FALSE) return (-1);
	//if (m_B1.Create(NULL, WS_VISIBLE, CRect(x1_B1, y1_B1, x2_B1, y2_B1), this, 0) == FALSE) return (-1);
	//if (m_B2.Create(NULL, WS_VISIBLE, CRect(x1_B2, y1_B2, x2_B2, y2_B2), this, 0) == FALSE) return (-1);
	//if (m_B3.Create(NULL, WS_VISIBLE, CRect(x1_B3, y1_B3, x2_B3, y2_B3), this, 0) == FALSE) return (-1);
	
	if (m_E1.Create(NULL, WS_VISIBLE, CRect(x1_E1, y1_E1, x2_E1, y2_E1), this, 0) == FALSE) return (-1);
	if (m_E2->Create(NULL, WS_VISIBLE, CRect(x1_E2, y1_E2, x2_E2, y2_E2), this, 0) == FALSE) return (-1);
	if (m_E3.Create(NULL, WS_VISIBLE, CRect(x1_E3, y1_E3, x2_E3, y2_E3), this, 0) == FALSE) return (-1);

	
	if (m_C1.Create(NULL, WS_VISIBLE, CRect(x1_C1, y1_C1, x2_C1, y2_C1), this, 0) == FALSE) return (-1);
	if (m_C2->Create(NULL, WS_VISIBLE, CRect(x1_C2, y1_C2, x2_C2, y2_C2), this, 0) == FALSE) return (-1);
	if (m_C3.Create(NULL, WS_VISIBLE, CRect(x1_C3, y1_C3, x2_C3, y2_C3), this, 0) == FALSE) return (-1);
	

	// on crée les boutons (offre/plateau/effacer)

	CString str;
	str.LoadString(IDS_CR_EFFHYPOTT);
	if (m_D3.Create(str, WS_CHILD|WS_VISIBLE, CRect(x1_D3, y1_D3, x2_D3, y2_D3), this, MSG_CR_EFFACERTT) == FALSE) return (-1);
	str.LoadString(IDS_CR_EFFHYPOACT);
	if (m_D4.Create(str, WS_CHILD|WS_VISIBLE, CRect(x1_D4, y1_D4, x2_D4, y2_D4), this, MSG_CR_EFFACER) == FALSE) return (-1);

	// on crée les composants HS et VS
	if (m_HS.Create(WS_VISIBLE | SBS_HORZ, CRect(x1_HS, y1_HS, x2_HS, y2_HS), this, 0) == FALSE) return (-1);
	if (m_VS.Create(WS_VISIBLE | SBS_VERT, CRect(x1_VS, y1_VS, x2_VS, y2_VS), this, 0) == FALSE) return (-1);

	// on configure les déplacements
	this->SetMode(&m_T1, MGER_SIZE, MGER_NONE);
	this->SetMode(&m_A1, MGER_NONE, MGER_NONE);
	this->SetMode(&m_A2, MGER_NONE, MGER_SIZE);
	this->SetMode(&m_A3, MGER_NONE, MGER_MOVE);
	//this->SetMode(&m_B1, MGER_NONE, MGER_NONE);
	//this->SetMode(&m_B2, MGER_NONE, MGER_SIZE);
	//this->SetMode(&m_B3, MGER_NONE, MGER_MOVE);

	this->SetMode(&m_E1, MGER_NONE, MGER_NONE);
	this->SetMode(m_E2, MGER_NONE, MGER_SIZE);
	this->SetMode(&m_E3, MGER_NONE, MGER_MOVE);

	this->SetMode(&m_C1, MGER_SIZE, MGER_NONE);
	this->SetMode(m_C2, MGER_SIZE, MGER_SIZE);
	this->SetMode(&m_C3, MGER_SIZE, MGER_MOVE);

	// this->SetMode(&m_D1, MGER_NONE, MGER_MOVE);
	// this->SetMode(&m_D2, MGER_NONE, MGER_MOVE);
	this->SetMode(&m_D3, MGER_NONE, MGER_MOVE);
	this->SetMode(&m_D4, MGER_NONE, MGER_MOVE);

	this->SetMode(&m_HS, MGER_SIZE, MGER_MOVE);
	this->SetMode(&m_VS, MGER_MOVE, MGER_SIZE);

	// on configure les ascenseurs
	this->SetScrollBar(&m_A2,  NULL, &m_VS);
	//this->SetScrollBar(&m_B2,  NULL, &m_VS);
	this->SetScrollBar(m_E2,  NULL, &m_VS);
	this->SetScrollBar(&m_C1, &m_HS,  NULL);
	this->SetScrollBar(m_C2, &m_HS, &m_VS);
	this->SetScrollBar(&m_C3, &m_HS,  NULL);

	// on lie les ascenseurs
	//m_B2.SetVertScroll(&m_VS, FALSE);
	m_C1.SetHorzScroll(&m_HS, TRUE);
	m_C2->SetHorzScroll(&m_HS, TRUE);
	m_C3.SetHorzScroll(&m_HS, TRUE);

	m_A2.SetVertScroll(&m_VS, TRUE);

	/*
	m_E2->SetVertScroll(&m_VS, FALSE);
	m_C2->SetVertScroll(&m_VS, FALSE);
	*/
	m_E2->SetVertScroll(&m_VS, TRUE);
	m_C2->SetVertScroll(&m_VS, TRUE);

	SetColors();
	// on quitte	
	return (0);
}

void CATPCreusetHypo::SetColors()
{
	// m_T1.SetBorderColor(CATPColors::GetColorLight(CATPColors::COLORCREUSET));
	// m_A1.SetBorderColor(CATPColors::GetColorLight(CATPColors::COLORCREUSET));
	// m_A3.SetBorderColor(CATPColors::GetColorLight(CATPColors::COLORCREUSET));
	m_D4.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_D4.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_D4.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_D4.SetFontName(CATPColors::GetFontStaticName());
	m_D4.SetFontSize(CATPColors::GetFontStaticSize());
	m_D4.SetFontBold(0);

	m_D3.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_D3.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_D3.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_D3.SetFontName(CATPColors::GetFontStaticName());
	m_D3.SetFontSize(CATPColors::GetFontStaticSize());
	m_D3.SetFontBold(0);
	Invalidate();
}

void CATPCreusetHypo::OnBtnEffacer()
{
	if(m_pATPDocCreuset->m_IdxHypotheseCourant == 0)
		return;

	CString ttl, qst;
	ttl.LoadString(IDR_CR_ATPFRAME);

	if(m_pATPDocCreuset->m_AFFCreuset.GetAffQuickPlan())
		qst = "Voulez-vous supprimer les insertions de l'hypothèse active?";
	else
		qst.LoadString(IDS_CR_SUPPINSERT);

	if ( MessageBox(qst, ttl, MB_ICONQUESTION|MB_OKCANCEL) == IDOK)
	{
		m_pATPDocCreuset->OnSuppInsertion(m_pATPDocCreuset->m_IdxHypotheseCourant, false);
		// on met à jour les vues du document
		m_pATPDocCreuset->UpdateAllViews(UPDATE_CURRENT_HYPO);
	}
}

void CATPCreusetHypo::OnBtnEffacerToute()
{
	CString ttl, qst;
	ttl.LoadString(IDR_CR_ATPFRAME);

	if(m_pATPDocCreuset->m_AFFCreuset.GetAffQuickPlan())
		qst = "Voulez-vous supprimer les insertions dans toutes les hypothèses?";
	else
		qst.LoadString(IDS_CR_SUPPINSERT);

	if ( MessageBox(qst, ttl, MB_ICONQUESTION|MB_OKCANCEL) == IDOK)
	{
		m_pATPDocCreuset->OnSuppInsertionAllHypo();
		// on met à jour les vues du document
		m_pATPDocCreuset->UpdateAllViews(UPDATE_ETUDE);
	}
}
