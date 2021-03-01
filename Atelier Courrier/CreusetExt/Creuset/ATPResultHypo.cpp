// on inclut les définitions nécessaires
#include "stdafx.h"
#include "ATPResultHypo_Metriques.h"
#include "ATPResultHypo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur
IMPLEMENT_DYNCREATE(CATPResultHypo, JFCSplitterPane)

CATPResultHypo::CATPResultHypo()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour définir le document

JVoid CATPResultHypo::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on définit le document des composants
	m_A1.SetDocument(pATPDocCreuset);
	m_A2.SetDocument(pATPDocCreuset);
	m_B1.SetDocument(pATPDocCreuset);
	m_B2.SetDocument(pATPDocCreuset);
	m_C1.SetDocument(pATPDocCreuset);
	m_C2.SetDocument(pATPDocCreuset);
	m_T1.SetDocument(pATPDocCreuset);

	// on met à jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

JVoid CATPResultHypo::Update()
{
	// on met à jour les composants
	m_A1.Update();
	m_A2.Update();
	m_B1.Update();
	m_B2.Update();
	m_C1.Update();
	m_C2.Update();
	m_T1.Update();

	// on met à jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPResultHypo::~CATPResultHypo()
{
	// on ne fait rien
}

///////////////////////////////////////////////////////////////////
// les fonctions pour récupérer les dimensions minimales du panneau

LONG CATPResultHypo::GetCXMin()
{
	// on calcule la largeur des composants
	LONG CX1 = CATPResultHypo_Metriques::CX_Ax() + 2 * CATPResultHypo_Metriques::CX_Bx() + CATPResultHypo_Metriques::CX_VS();

	// on calcule la largeur des bordures et des interstices
	LONG CX2 = (2 * CATPResultHypo_Metriques::CX_Border()) + (3 * CATPResultHypo_Metriques::CX_Interstice());

	// on renvoie la largeur
	return (CX1 + CX2);
}

LONG CATPResultHypo::GetCYMin()
{
	// on calcule la hauteur des composants
	LONG CY1 = CATPResultHypo_Metriques::CY_T1() + 2*CATPResultHypo_Metriques::CY_x1() + CATPResultHypo_Metriques::CY_x2();

	// on calcule la largeur des bordures et des interstices
	LONG CY2 = (2 * CATPResultHypo_Metriques::CY_Border()) + (2 * CATPResultHypo_Metriques::CY_Interstice());

	// on renvoie la hauteur
	return (CY1 + CY2);
}

BEGIN_MESSAGE_MAP(CATPResultHypo, JFCSplitterPane)
	//{{AFX_MSG_MAP(CATPResultHypo)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////
// la fonction pour appliquer les mises à jour

JVoid CATPResultHypo::OnUpdate()
{
	// si échelle active , on le cache sinon on le rend visible
	if (m_pATPDocCreuset != 0)
		this->ShowWindow((m_pATPDocCreuset->m_AFFCreuset.GetEchelleResult()) ? SW_HIDE : SW_SHOW );
}

////////////////////////////////////
// CATPResultHypo message handlers

int CATPResultHypo::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	// on appelle le gestionnaire de base
	if (this->JFCSplitterPane::OnCreate(lpCreateStruct) == -1) return (-1);

	// on calcule la position des composants

	// Titre Barre Générale Résultats
	LONG x1_T1 = CATPResultHypo_Metriques::CX_Border();             LONG x2_T1 = x1_T1 + CATPResultHypo_Metriques::CX_Ax() + 2 * CATPResultHypo_Metriques::CX_Bx() + 2 * CATPResultHypo_Metriques::CX_Interstice();
	LONG y1_T1 = CATPResultHypo_Metriques::CY_Border();             LONG y2_T1 = y1_T1 + CATPResultHypo_Metriques::CY_T1();

	// Titre Performance
	LONG x1_A1 = x1_T1;                                             LONG x2_A1 = x1_A1 + CATPResultHypo_Metriques::CX_Ax();
	LONG y1_A1 = y2_T1 + CATPResultHypo_Metriques::CY_Interstice(); LONG y2_A1 = y1_A1 + 2*CATPResultHypo_Metriques::CY_x1();

	// Bloc libellé grandeurs résultats 
	LONG x1_A2 = x1_A1;                                             LONG x2_A2 = x2_A1;
	LONG y1_A2 = y2_A1 + CATPResultHypo_Metriques::CY_Interstice(); LONG y2_A2 = y1_A2 + CATPResultHypo_Metriques::CY_x2();

	// Les colonnes hypothèses
	LONG x1_C1 = x2_A1 + CATPResultHypo_Metriques::CX_Interstice(); LONG x2_C1 = x1_C1 + CATPResultHypo_Metriques::CX_Bx();
	LONG y1_C1 = y1_A1;                                             LONG y2_C1 = y2_A1;

	// Colonnes résultats hypothèses
	LONG x1_C2 = x1_C1;                                             LONG x2_C2 = x2_C1;
	LONG y1_C2 = y1_A2;                                             LONG y2_C2 = y2_A2;

	// Les colonnes hypothèses
	LONG x1_B1 = x2_C1 + CATPResultHypo_Metriques::CX_Interstice(); LONG x2_B1 = x1_B1 + CATPResultHypo_Metriques::CX_Bx();
	LONG y1_B1 = y1_C1;                                             LONG y2_B1 = y2_C1;

	// Colonnes résultats hypothèses
	LONG x1_B2 = x1_B1;                                             LONG x2_B2 = x2_B1;
	LONG y1_B2 = y1_C2;                                             LONG y2_B2 = y2_C2;

	// L'ascenseur
	LONG x1_VS = x2_B2 + CATPResultHypo_Metriques::CX_Interstice(); LONG x2_VS = x1_VS + CATPResultHypo_Metriques::CX_VS();
	LONG y1_VS = y1_B2;                                             LONG y2_VS = y2_B2;

	// on crée les composants A1 à T1
	if (m_A1.Create(NULL, WS_VISIBLE, CRect(x1_A1, y1_A1, x2_A1, y2_A1), this, 0) == FALSE) return (-1);
	if (m_A2.Create(NULL, WS_VISIBLE, CRect(x1_A2, y1_A2, x2_A2, y2_A2), this, 0) == FALSE) return (-1);
	if (m_C1.Create(NULL, WS_VISIBLE, CRect(x1_C1, y1_C1, x2_C1, y2_C1), this, 0) == FALSE) return (-1);
	if (m_C2.Create(NULL, WS_VISIBLE, CRect(x1_C2, y1_C2, x2_C2, y2_C2), this, 0) == FALSE) return (-1);
	if (m_B1.Create(NULL, WS_VISIBLE, CRect(x1_B1, y1_B1, x2_B1, y2_B1), this, 0) == FALSE) return (-1);
	if (m_B2.Create(NULL, WS_VISIBLE, CRect(x1_B2, y1_B2, x2_B2, y2_B2), this, 0) == FALSE) return (-1);
	if (m_T1.Create(NULL, WS_VISIBLE, CRect(x1_T1, y1_T1, x2_T1, y2_T1), this, 0) == FALSE) return (-1);

	if (m_progress. Create( NULL, CRect(x1_T1, y2_A2+1, x2_T1, y2_A2+25),  this, 0) == FALSE) return (-1);
	m_progress.SetRange(0,100);
	m_progress.SetShowText(FALSE);
	m_progress.SetBarColour(CATPColors::GetColorSelect(CATPColors::COLORCREUSET));
	// on crée les composants HS et VS
	if (m_VS.Create(WS_VISIBLE | SBS_VERT, CRect(x1_VS, y1_VS, x2_VS, y2_VS), this, 0) == FALSE) return (-1);

	// on configure les déplacements
	this->SetMode(&m_T1, MGER_SIZE, MGER_NONE);
	this->SetMode(&m_progress,  MGER_SIZE, MGER_MOVE);
	this->SetMode(&m_A1, MGER_NONE, MGER_NONE);
	this->SetMode(&m_A2, MGER_NONE, MGER_SIZE);
	this->SetMode(&m_C1, MGER_NONE, MGER_NONE);
	this->SetMode(&m_C2, MGER_NONE, MGER_SIZE);
	this->SetMode(&m_B1, MGER_SIZE, MGER_NONE);
	this->SetMode(&m_B2, MGER_SIZE, MGER_SIZE);
	this->SetMode(&m_VS, MGER_MOVE, MGER_SIZE);

	// on configure les ascenseurs
	this->SetScrollBar(&m_A2,  NULL, &m_VS);
	this->SetScrollBar(&m_C2,  NULL, &m_VS);
	this->SetScrollBar(&m_B2,  NULL, &m_VS);

	// on lie les ascenseurs
	m_A2.SetVertScroll(&m_VS, TRUE);
	/*
	m_C2.SetVertScroll(&m_VS, FALSE);
	m_B2.SetVertScroll(&m_VS, FALSE);
	*/
	m_C2.SetVertScroll(&m_VS, TRUE);
	m_B2.SetVertScroll(&m_VS, TRUE);

	SetColors();
	// on quitte
	return (0);
}

void CATPResultHypo::SetColors()
{
	// m_T1.SetBorderColor(CATPColors::GetColorLight(CATPColors::COLORCREUSET));
	Invalidate();
}

// Set Horizontal Scroll
JFControl & CATPResultHypo::SetHorzScrollBar1(JFCScroll & HScroll)
{
	this->SetScrollBar(&m_B1, &HScroll,  NULL);
	m_B1.SetHorzScroll(&HScroll, TRUE);
	return m_B1;
}

JFControl & CATPResultHypo::SetHorzScrollBar2(JFCScroll & HScroll)
{
	this->SetScrollBar(&m_B2, &HScroll, &m_VS);
	m_B2.SetHorzScroll(&HScroll, TRUE);
	return m_B2;
}
