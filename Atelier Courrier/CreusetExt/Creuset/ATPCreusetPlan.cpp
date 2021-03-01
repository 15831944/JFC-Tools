// on inclut les définitions nécessaires
#include "stdafx.h"
#include "ATPCreusetPlan_Metriques.h"
#include "ATPCreusetPlan.h"
#include ".\atpcreusetplan.h"
#include "MsgPrvCreuset.h"
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur
IMPLEMENT_DYNCREATE(CATPCreusetPlan, JFCSplitterPane)
CATPCreusetPlan::CATPCreusetPlan()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour définir le document

JVoid CATPCreusetPlan::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on définit le document des composants
	m_A0.SetDocument(pATPDocCreuset);
	m_A1.SetDocument(pATPDocCreuset);
	m_A2.SetDocument(pATPDocCreuset);
	m_A3.SetDocument(pATPDocCreuset);
	m_B1.SetDocument(pATPDocCreuset);
	m_B2.SetDocument(pATPDocCreuset);
	m_B3.SetDocument(pATPDocCreuset);
	m_C1.SetDocument(pATPDocCreuset);
	m_C2.SetDocument(pATPDocCreuset);
	m_C3.SetDocument(pATPDocCreuset);
	
	m_LstHypo.SetDocument(pATPDocCreuset);
	m_BtnPM.SetDocument(pATPDocCreuset);
	
	// on met à jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

JVoid CATPCreusetPlan::Update()
{
	// on met à jour les composants
	m_A0.Update();
	m_A1.Update();
	m_A2.Update();
	m_A3.Update();
	m_B1.Update();
	m_B2.Update();
	m_B3.Update();
	m_C1.Update();
	m_C2.Update();
	m_C3.Update();

	m_LstHypo.Update();
	m_BtnPM.Update();

	m_D1.SetCheck(m_pATPDocCreuset->m_AFFPlateauGrille);
	m_D2.SetCheck(!m_pATPDocCreuset->m_AFFPlateauGrille);

	// m_BtnPE.SetCheck(m_pATPDocCreuset->m_AFFPlanEtendu);
	m_D3.EnableWindow(!m_pATPDocCreuset->m_AFFPlanMaitre);

	// on met à jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPCreusetPlan::~CATPCreusetPlan()
{
	// on ne fait rien
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour configurer le graphe

JVoid CATPCreusetPlan::SetGraphe(JFControl & Control)
{
	// on configure les acsenseurs
	this->SetScrollBar(&Control, &m_HS, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour récupérer l'ascenseur horizontal

JFCScroll & CATPCreusetPlan::GetHorzScrollBar()
{
	// on renvoie l'ascenseur
	return (m_HS);
}

///////////////////////////////////////////////////////////////////
// les fonctions pour récupérer les dimensions minimales du panneau

LONG CATPCreusetPlan::GetCXMin()
{
	// on calcule la largeur des composants
	LONG CX1 = CATPCreusetPlan_Metriques::CX_Ax() + CATPCreusetPlan_Metriques::CX_Bx() + CATPCreusetPlan_Metriques::CX_Cx() + CATPCreusetPlan_Metriques::CY_x4();

	// on calcule la largeur des bordures et des interstices
	LONG CX2 = (2 * CATPCreusetPlan_Metriques::CX_Border()) + (3 * CATPCreusetPlan_Metriques::CX_Interstice());

	// on renvoie la largeur
	return (CX1 + CX2);
}

LONG CATPCreusetPlan::GetCYMin()
{
	// on calcule la hauteur des composants
	LONG CY1 = CATPCreusetPlan_Metriques::CY_x1() + CATPCreusetPlan_Metriques::CY_x2() + CATPCreusetPlan_Metriques::CY_x3()+ 2*CATPCreusetPlan_Metriques::CY_x4() ;

	// on calcule la largeur des bordures et des interstices
	LONG CY2 = 2 * CATPCreusetPlan_Metriques::CY_Border() + 4 * CATPCreusetPlan_Metriques::CY_Interstice();

	// on renvoie la hauteur
	return (CY1 + CY2);
}

LONG CATPCreusetPlan::GetCYIdeal()
{
	// on calcule la hauteur des composants
	LONG CY1 = CATPCreusetPlan_Metriques::CY_x1() + 5 *CATPCreusetPlan_Metriques::CY_x2() + CATPCreusetPlan_Metriques::CY_x3()+ 2*CATPCreusetPlan_Metriques::CY_x4() ;

	// on calcule la largeur des bordures et des interstices
	LONG CY2 = 2 * CATPCreusetPlan_Metriques::CY_Border() + 4 * CATPCreusetPlan_Metriques::CY_Interstice();

	// on renvoie la hauteur
	return (CY1 + CY2);
}

BEGIN_MESSAGE_MAP(CATPCreusetPlan, JFCSplitterPane)
	//{{AFX_MSG_MAP(CATPCreusetPlan)
	ON_WM_CREATE()
	ON_BN_CLICKED(MSG_CR_PLUS,		OnBtnPlus)
	ON_BN_CLICKED(MSG_CR_MOINS,		OnBtnMoins)
	ON_BN_CLICKED(MSG_CR_T,		OnBtnT)
	ON_BN_CLICKED(MSG_CR_PLATEAU,	OnBtnPlateau)
	ON_BN_CLICKED(MSG_CR_PLAN,		OnBtnPlan)
	ON_BN_CLICKED(MSG_CR_EFFACER,	OnBtnEffacer)
	ON_BN_CLICKED(MSG_CR_PE,		OnBtnPlanEtendu)
	ON_BN_CLICKED(MSG_CR_PT,		OnBtnPlanTemoin)
	ON_BN_CLICKED(MSG_CR_SELALLHYPO,	OnBtnSelHypo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////
// la fonction pour appliquer les mises à jour

JVoid CATPCreusetPlan::OnUpdate()
{
	// on ne fait rien
}

//////////////////////////////////
// CATPCreusetPlan message handlers

int CATPCreusetPlan::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	// on appelle le gestionnaire de base
	if (this->JFCSplitterPane::OnCreate(lpCreateStruct) == -1) return (-1);

	// on calcule la position des composants de la grille calendaire
	LONG x1_A0 = CATPCreusetPlan_Metriques::CX_Border();             LONG x2_A0 = x1_A0 + CATPCreusetPlan_Metriques::CX_Ax() + CATPCreusetPlan_Metriques::CX_Interstice() + CATPCreusetPlan_Metriques::CX_Bx();
	LONG y1_A0 = CATPCreusetPlan_Metriques::CY_Border();             LONG y2_A0 = y1_A0 + CATPCreusetPlan_Metriques::CY_x0();

	LONG x1_A1 = CATPCreusetPlan_Metriques::CX_Border();             LONG x2_A1 = x1_A1 + CATPCreusetPlan_Metriques::CX_Ax();
	LONG y1_A1 = y2_A0 + CATPCreusetPlan_Metriques::CY_Interstice(); LONG y2_A1 = y1_A1 + CATPCreusetPlan_Metriques::CY_x0();

	LONG x1_A2 = x1_A1;                                             LONG x2_A2 = x2_A1;
	LONG y1_A2 = y2_A1 + CATPCreusetPlan_Metriques::CY_Interstice(); LONG y2_A2 = y1_A2 + CATPCreusetPlan_Metriques::CY_x2();

	LONG x1_A3 = x1_A2;                                             LONG x2_A3 = x2_A2;
	LONG y1_A3 = y2_A2 + CATPCreusetPlan_Metriques::CY_Interstice(); LONG y2_A3 = y1_A3 + CATPCreusetPlan_Metriques::CY_x3();

	LONG x1_B1 = x2_A1 + CATPCreusetPlan_Metriques::CX_Interstice(); LONG x2_B1 = x1_B1 + CATPCreusetPlan_Metriques::CX_Bx();
	LONG y1_B1 = y1_A1;                                             LONG y2_B1 = y2_A1;

	LONG x1_B2 = x1_B1;                                             LONG x2_B2 = x2_B1;
	LONG y1_B2 = y1_A2;                                             LONG y2_B2 = y2_A2;
	LONG x1_B3 = x1_B2;                                             LONG x2_B3 = x2_B2;
	LONG y1_B3 = y1_A3;                                             LONG y2_B3 = y2_A3;
	
	LONG x1_C1 = x2_B1 + CATPCreusetPlan_Metriques::CX_Interstice(); LONG x2_C1 = x1_C1 + CATPCreusetPlan_Metriques::CX_Cx();
	LONG y1_C1 = y1_A0;                                             LONG y2_C1 = y2_B1;
	
	LONG x1_C2 = x1_C1;                                             LONG x2_C2 = x2_C1;
	LONG y1_C2 = y1_B2;                                             LONG y2_C2 = y2_B2;
	LONG x1_C3 = x1_C2;                                             LONG x2_C3 = x2_C2;
	LONG y1_C3 = y1_B3;                                             LONG y2_C3 = y2_B3;

	// Positionnement des boutons (Plateau/Plan/Effacer) sous la grille calendaire
	LONG x1_D1 = x1_A1;												LONG x2_D1 = x1_D1 + CATPCreusetPlan_Metriques::CX_Dx();
	LONG y1_D1 = y2_C3 + CATPCreusetPlan_Metriques::CY_Interstice();LONG y2_D1 = y1_D1 + CATPCreusetPlan_Metriques::CY_x4();
	LONG x1_D2 = x2_D1 + CATPCreusetPlan_Metriques::CX_Interstice();LONG x2_D2 = x1_D2 + CATPCreusetPlan_Metriques::CX_Dx();
	LONG y1_D2 = y1_D1;                                             LONG y2_D2 = y2_D1;
	LONG x1_D3 = x2_D2 + CATPCreusetPlan_Metriques::CX_Interstice();LONG x2_D3 = x2_B3;
	LONG y1_D3 = y1_D1;                                             LONG y2_D3 = y2_D1;


	// Positionnement ascenseurs grille calendaire
	LONG x1_HS = x1_C3;                                             LONG x2_HS = x2_C3;
	LONG y1_HS = y2_C3 + CATPCreusetPlan_Metriques::CY_Interstice();
//	LONG y1_HS = y2_C3 + CATPCreusetPlan_Metriques::CY_Interstice() + (CATPCreusetPlan_Metriques::CY_x4()-CATPCreusetPlan_Metriques::CY_HS())/2; //centré
	LONG y2_HS = y1_HS + CATPCreusetPlan_Metriques::CY_HS();

	LONG x1_VS = x2_C2 + CATPCreusetPlan_Metriques::CX_Interstice();LONG x2_VS = x1_VS + CATPCreusetPlan_Metriques::CX_VS();
//	LONG x1_VS = x2_C2 + CATPCreusetPlan_Metriques::CX_Interstice() + (CATPCreusetPlan_Metriques::CY_x3()-CATPCreusetPlan_Metriques::CX_VS())/2;LONG x2_VS = x1_VS + CATPCreusetPlan_Metriques::CX_VS(); // centré
	LONG y1_VS = y1_C2;                                             LONG y2_VS = y2_C2;

	LONG x1_E1 = x2_C2 + CATPCreusetPlan_Metriques::CX_Interstice();LONG x2_E1 = x1_E1 + CATPCreusetPlan_Metriques::CY_x3();
	LONG y1_E1 = y2_VS + CATPCreusetPlan_Metriques::CY_Interstice();LONG y2_E1 = y1_E1 + CATPCreusetPlan_Metriques::CY_x3();
	LONG x1_E2 = x2_C2 + CATPCreusetPlan_Metriques::CX_Interstice();LONG x2_E2 = x2_E1;
	LONG y1_E2 = y2_E1 + CATPCreusetPlan_Metriques::CY_Interstice();LONG y2_E2 = y1_E2 + CATPCreusetPlan_Metriques::CY_x3();

	LONG x1_F1 = x2_C1 + CATPCreusetPlan_Metriques::CX_Interstice();LONG x2_F1 = x2_E1;
	LONG y1_F1 = y1_A0;												LONG y2_F1 = y1_F1 + CATPCreusetPlan_Metriques::CY_x3();

	LONG x1_F2 = x2_C2 + CATPCreusetPlan_Metriques::CX_Interstice();LONG x2_F2 = x2_E1;
	LONG y1_F2 = y2_E2 + CATPCreusetPlan_Metriques::CY_Interstice();LONG y2_F2 = y1_F2 + CATPCreusetPlan_Metriques::CY_x3();

	// on crée les composants A1 à C3
	if (m_A0.Create(NULL, WS_VISIBLE, CRect(x1_A0, y1_A0, x2_A0, y2_A0), this, 0) == FALSE) return (-1);
	if (m_A1.Create(NULL, WS_VISIBLE| SS_CENTERIMAGE | SS_CENTER, CRect(x1_A1, y1_A1, x2_A1, y2_A1), this, 0) == FALSE) return (-1);

	if (m_A2.Create(NULL, WS_VISIBLE, CRect(x1_A2, y1_A2, x2_A2, y2_A2), this, 0) == FALSE) return (-1);
	if (m_A3.Create(NULL, WS_VISIBLE, CRect(x1_A3, y1_A3, x2_A3, y2_A3), this, 0) == FALSE) return (-1);
	if (m_B1.Create(NULL, WS_VISIBLE, CRect(x1_B1, y1_B1, x2_B1, y2_B1), this, 0) == FALSE) return (-1);
	if (m_B2.Create(NULL, WS_VISIBLE, CRect(x1_B2, y1_B2, x2_B2, y2_B2), this, 0) == FALSE) return (-1);
	if (m_B3.Create(NULL, WS_VISIBLE, CRect(x1_B3, y1_B3, x2_B3, y2_B3), this, 0) == FALSE) return (-1);
	if (m_C1.Create(NULL, WS_VISIBLE, CRect(x1_C1, y1_C1, x2_C1, y2_C1), this, 0) == FALSE) return (-1);
	if (m_C2.Create(NULL, WS_VISIBLE, CRect(x1_C2, y1_C2, x2_C2, y2_C2), this, 0) == FALSE) return (-1);
	if (m_C3.Create(NULL, WS_VISIBLE, CRect(x1_C3, y1_C3, x2_C3, y2_C3), this, 0) == FALSE) return (-1);
	m_C3.SetVertItemSize(22, TRUE);

	// on crée les boutons (palteau/plan/effacer) sous la grille
	CString str;
	str.LoadString(IDS_CR_PLATEAU);
	if (m_D1.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(x1_D1, y1_D1, x2_D1, y2_D1), this, MSG_CR_PLATEAU	) == FALSE) return (-1);
	str.LoadString(IDS_CR_PLAN);
	if (m_D2.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(x1_D2, y1_D2, x2_D2, y2_D2), this,    MSG_CR_PLAN ) == FALSE) return (-1);
	str.LoadString(IDS_CR_EFFACER);
	if (m_D3.Create(str, WS_CHILD|WS_VISIBLE, CRect(x1_D3, y1_D3, x2_D3, y2_D3), this, MSG_CR_EFFACER	) == FALSE) return (-1);

	// on crée les composants HS et VS
	if (m_HS.Create(WS_VISIBLE | SBS_HORZ, CRect(x1_HS, y1_HS, x2_HS, y2_HS), this, 0) == FALSE) return (-1);
	if (m_VS.Create(WS_VISIBLE | SBS_VERT, CRect(x1_VS, y1_VS, x2_VS, y2_VS), this, 0) == FALSE) return (-1);

	// on crée les boutons +/-
	str.LoadString(IDS_CR_PLUS);
	if (m_BtnPlus.Create(str, WS_CHILD|WS_VISIBLE, CRect(x1_E1, y1_E1, x2_E1, y2_E1), this, MSG_CR_PLUS) == FALSE) return (-1);
	str.LoadString(IDS_CR_MOINS);
	if (m_BtnMoins.Create(str, WS_CHILD|WS_VISIBLE, CRect(x1_E2, y1_E2, x2_E2, y2_E2), this , MSG_CR_MOINS) == FALSE) return (-1);

	str.LoadString(IDS_CR_T);
	if (m_BtnT.Create(str, WS_CHILD|WS_VISIBLE, CRect(x1_F1, y1_F1, x2_F1, y2_F1), this, MSG_CR_T) == FALSE) return (-1);

	str.Format("¤");
	if (m_BtnAff.Create(str, WS_CHILD|WS_VISIBLE, CRect(x1_F2, y1_F2, x2_F2, y2_F2), this, MSG_CR_SELALLHYPO) == FALSE) return (-1);

	LONG x1_P1 = x1_A1;												LONG x2_P1 = x1_P1 + CATPCreusetPlan_Metriques::CX_Dx();
	LONG y1_P1 = y2_D3 + CATPCreusetPlan_Metriques::CY_Interstice();LONG y2_P1 = y1_P1 + CATPCreusetPlan_Metriques::CY_x4();
	LONG x1_P2 = x2_P1 + CATPCreusetPlan_Metriques::CX_Interstice();LONG x2_P2 = x1_P2 + CATPCreusetPlan_Metriques::CX_Dx();
	LONG y1_P2 = y1_P1;                                             LONG y2_P2 = y2_P1;
	LONG x1_P3 = x2_P2 + CATPCreusetPlan_Metriques::CX_Interstice();LONG x2_P3 = x2_B3;
	LONG y1_P3 = y1_P1;                                             LONG y2_P3 = y2_P1;

	// Create Btn Plan Etendu
	/*
	str.LoadString(IDS_CR_PLAN_E);
	if (m_BtnPE.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX|WS_DISABLED, CRect(x1_P1, y1_P1, x2_P1, y2_P1), this, MSG_CR_PE) == FALSE) return (-1);
	*/

	// Create Btn Plan Témoin
	/*
	str.LoadString(IDS_CR_PLAN_T);
	if (m_BtnPT.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX|WS_DISABLED, CRect(x1_P2, y1_P2, x2_P2, y2_P2), this, MSG_CR_PT) == FALSE) return (-1);
	*/

	// Create Btn Plan Maitre
	str.LoadString(IDS_CR_PLAN_M);
	if (m_BtnPM. Create(NULL, WS_VISIBLE, CRect(x1_P3, y1_P3, x2_P3, y2_P3),  this, 0) == FALSE) return (-1);

	m_BtnPM.SetHorzItemSize(CATPCreusetPlan_Metriques::CX_Dx()-3, 0, 0,TRUE);
	m_BtnPM.SetVertItemSize(CATPCreusetPlan_Metriques::CY_x4()-1, 0, 0,TRUE);


	LONG x1_H1 = x1_C1-2;												LONG x2_H1 = x1_H1 + CATPCreusetPlan_Metriques::CX_Cx();
	LONG y1_H1 = y2_D3 + CATPCreusetPlan_Metriques::CY_Interstice();	LONG y2_H1 = y2_P1;

	if (m_LstHypo. Create(NULL, WS_VISIBLE, CRect(x1_H1,  y1_H1, x2_H1,   y2_H1),  this, 0) == FALSE) return (-1);
	m_LstHypo.SetVertItemSize(CATPCreusetPlan_Metriques::CY_x4()-1,0,0,TRUE);

	//if (m_progress. Create( NULL, CRect(x1_H1,  y1_H1, x2_H1,   y2_H1),  this, 0) == FALSE) return (-1);
	//m_progress.SetRange(0,100);

	// Mode Size des composants pupitre
	this->SetMode(&m_LstHypo,  MGER_SIZE, MGER_MOVE);

	// Mode Btn Etendu, Temoin, Maitre
	// this->SetMode(&m_BtnPE, MGER_NONE, MGER_MOVE);
	// this->SetMode(&m_BtnPT, MGER_NONE, MGER_MOVE);
	this->SetMode(&m_BtnPM, MGER_NONE, MGER_MOVE);

	// on configure les déplacements
	this->SetMode(&m_A0, MGER_NONE, MGER_NONE);
	this->SetMode(&m_A1, MGER_NONE, MGER_NONE);
	this->SetMode(&m_A2, MGER_NONE, MGER_SIZE);
	this->SetMode(&m_A3, MGER_NONE, MGER_MOVE);
	this->SetMode(&m_B1, MGER_NONE, MGER_NONE);
	this->SetMode(&m_B2, MGER_NONE, MGER_SIZE);
	this->SetMode(&m_B3, MGER_NONE, MGER_MOVE);
	this->SetMode(&m_C1, MGER_SIZE, MGER_NONE);
	this->SetMode(&m_C2, MGER_SIZE, MGER_SIZE);
	this->SetMode(&m_C3, MGER_SIZE, MGER_MOVE);
	this->SetMode(&m_D1, MGER_NONE, MGER_MOVE);
	this->SetMode(&m_D2, MGER_NONE, MGER_MOVE);
	this->SetMode(&m_D3, MGER_NONE, MGER_MOVE);
	this->SetMode(&m_HS, MGER_SIZE, MGER_MOVE);
	this->SetMode(&m_VS, MGER_MOVE, MGER_SIZE);

	this->SetMode(&m_BtnPlus, MGER_MOVE, MGER_MOVE);
	this->SetMode(&m_BtnMoins, MGER_MOVE, MGER_MOVE);

	this->SetMode(&m_BtnT, MGER_MOVE, MGER_NONE);
	this->SetMode(&m_BtnAff, MGER_MOVE, MGER_MOVE);

	// on configure les ascenseurs
	this->SetScrollBar(&m_A2,  NULL, &m_VS);
	this->SetScrollBar(&m_B2,  NULL, &m_VS);
	this->SetScrollBar(&m_C1, &m_HS,  NULL);
	this->SetScrollBar(&m_C2, &m_HS, &m_VS);
	this->SetScrollBar(&m_C3, &m_HS,  NULL);

	// on lie les ascenseurs
	m_A2.SetVertScroll(&m_VS, TRUE);
	m_B2.SetVertScroll(&m_VS, FALSE);
	m_C1.SetHorzScroll(&m_HS, TRUE);
	m_C2.SetHorzScroll(&m_HS, FALSE);
	m_C2.SetVertScroll(&m_VS, FALSE);
	m_C3.SetHorzScroll(&m_HS, FALSE);

	m_C1.SetParent(this);
	m_C2.SetParent(this);
	m_C3.SetParent(this);

	SetColors();

	// on quitte	
	return (0);
}

void CATPCreusetPlan::Invalidate(BOOL bErase)
{
	m_A0.Invalidate(bErase);
	m_A1.Invalidate(bErase);
	m_A2.Invalidate(bErase);
	m_A3.Invalidate(bErase);
	m_B1.Invalidate(bErase);
	m_B2.Invalidate(bErase);
	m_B3.Invalidate(bErase);
	m_C1.Invalidate(bErase);
	m_C2.Invalidate(bErase);
	m_C3.Invalidate(bErase);
	m_D1.Invalidate(bErase);
	m_D2.Invalidate(bErase);
	m_D3.Invalidate(bErase);
	m_LstHypo.Invalidate(bErase);
	m_BtnPM.Invalidate(bErase);
	CWnd::Invalidate(bErase);
}

void CATPCreusetPlan::SetColors()
{
	// TEST aurelien (SetBorderColor sur classes dérivées de JFCTitle)
	// m_A0.SetBorderColor(CATPColors::GetColorLight(CATPColors::COLORCREUSET));
	// m_A1.SetBorderColor(CATPColors::GetColorLight(CATPColors::COLORCREUSET));
	// m_A3.SetBorderColor(CATPColors::GetColorLight(CATPColors::COLORCREUSET));
	// m_B1.SetBorderColor(CATPColors::GetColorLight(CATPColors::COLORCREUSET));
	// m_B3.SetBorderColor(CATPColors::GetColorLight(CATPColors::COLORCREUSET));

	m_D1.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_D1.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_D1.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_D1.SetFontName(CATPColors::GetFontStaticName());
	m_D1.SetFontSize(CATPColors::GetFontStaticSize());
	m_D1.SetFontBold(0);

	m_D2.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_D2.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_D2.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_D2.SetFontName(CATPColors::GetFontStaticName());
	m_D2.SetFontSize(CATPColors::GetFontStaticSize());
	m_D2.SetFontBold(0);

	m_D3.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_D3.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_D3.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_D3.SetFontName(CATPColors::GetFontStaticName());
	m_D3.SetFontSize(CATPColors::GetFontStaticSize());
	m_D3.SetFontBold(0);

	m_BtnPlus.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_BtnPlus.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_BtnPlus.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_BtnPlus.SetFontName(CATPColors::GetFontStaticName());
	m_BtnPlus.SetFontSize(CATPColors::GetFontStaticSize());
	m_BtnPlus.SetFontBold(0);

	m_BtnMoins.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_BtnMoins.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_BtnMoins.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_BtnMoins.SetFontName(CATPColors::GetFontStaticName());
	m_BtnMoins.SetFontSize(CATPColors::GetFontStaticSize());
	m_BtnMoins.SetFontBold(0);

	m_BtnAff.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_BtnAff.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_BtnAff.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_BtnAff.SetFontName(CATPColors::GetFontStaticName());
	m_BtnAff.SetFontSize(CATPColors::GetFontStaticSize());
	m_BtnAff.SetFontBold(0);

	m_BtnT.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_BtnT.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_BtnT.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_BtnT.SetFontName(CATPColors::GetFontStaticName());
	m_BtnT.SetFontSize(CATPColors::GetFontStaticSize());
	m_BtnT.SetFontBold(0);

	// Couleur Btn Plan Etendu
	/*
	m_BtnPE.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCREUSET) );
	m_BtnPE.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCREUSET) );
	m_BtnPE.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCREUSET) );
	m_BtnPE.SetFontName(CATPColors::GetFontStaticName());
	m_BtnPE.SetFontSize(CATPColors::GetFontStaticSize());
	m_BtnPE.SetFontBold(0);
	*/

	// Couleur Btn Plan Temoin
	/*
	m_BtnPT.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCREUSET) );
	m_BtnPT.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCREUSET) );
	m_BtnPT.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCREUSET) );
	m_BtnPT.SetFontName(CATPColors::GetFontStaticName());
	m_BtnPT.SetFontSize(CATPColors::GetFontStaticSize());
	m_BtnPT.SetFontBold(0);
	*/

	Invalidate();
}

void CATPCreusetPlan::OnBtnT()
{
	if (m_pATPDocCreuset->IsValid())
		m_C1.BtnT();
}

void CATPCreusetPlan::OnBtnPlus()
{
	if (m_pATPDocCreuset->m_ZoomGrille < 25)
		m_pATPDocCreuset->m_ZoomGrille += 2;
	m_pATPDocCreuset->UpdateAllViews(UPDATE_GRILLE_ZOOM);
}

void CATPCreusetPlan::OnBtnMoins()
{
	if (m_pATPDocCreuset->m_ZoomGrille > 4)
		m_pATPDocCreuset->m_ZoomGrille -= 2;
	m_pATPDocCreuset->UpdateAllViews(UPDATE_GRILLE_ZOOM);
}

void CATPCreusetPlan::OnBtnPlateau()
{
	if(!m_pATPDocCreuset->m_AFFPlateauGrille)
		m_pATPDocCreuset->m_AFFPlateauGrille = 1;
	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_BTN_CREUSETPLAN);
}

void CATPCreusetPlan::OnBtnPlan()
{
	if(m_pATPDocCreuset->m_AFFPlateauGrille)
		m_pATPDocCreuset->m_AFFPlateauGrille = 0;
	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_BTN_CREUSETPLAN);
}

void CATPCreusetPlan::OnBtnEffacer()
{
	CString ttl, qst;
	ttl.LoadString(IDR_CR_ATPFRAME);
	qst.LoadString(IDS_CR_SUPPINSERT);
	if ( MessageBox(qst, ttl, MB_ICONQUESTION|MB_OKCANCEL) == IDOK)
	{
		m_pATPDocCreuset->OnSuppInsertion(m_pATPDocCreuset->m_IdxHypotheseCourant, true);
		// on met à jour les vues du document
		m_pATPDocCreuset->UpdateAllViews(UPDATE_CURRENT_HYPO);
	}
}

void CATPCreusetPlan::OnBtnPlanEtendu()
{
	if (m_pATPDocCreuset->IsValid())
	{
		m_pATPDocCreuset->m_AFFPlanEtendu = !m_pATPDocCreuset->m_AFFPlanEtendu;
		// on met à jour les vues du document
		m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_BAR_PLAN);
	}
}

void CATPCreusetPlan::OnBtnPlanTemoin()
{
}


void CATPCreusetPlan::OnBtnSelHypo()
{
	m_pATPDocCreuset->m_MapAffHypothese.ResetSet(m_pATPDocCreuset->m_IdxHypotheseActif);
	// Update affichage insertions
	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_CREUSET);
}

