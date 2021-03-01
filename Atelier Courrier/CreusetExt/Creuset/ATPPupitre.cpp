// on inclut les définitions nécessaires
#include "stdafx.h"
#include "ATPPupitre_Metriques.h"
#include "ATPPupitre.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur
IMPLEMENT_DYNCREATE(CATPPupitre, JFCSplitterPane)

CATPPupitre::CATPPupitre()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour définir le document

JVoid CATPPupitre::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on définit le document des composants
	// Composant sources
	m_A1.SetDocument(pATPDocCreuset);
	// m_A2.SetDocument(pATPDocCreuset);
	m_ListTerrains.SetDocument(pATPDocCreuset); 
		
	// Composant cibles
	m_A3.SetDocument(pATPDocCreuset);
	// m_A4.SetDocument(pATPDocCreuset);
	m_ListCibles.SetDocument(pATPDocCreuset); 
	
	// Composant formats
	m_A5.SetDocument(pATPDocCreuset);
	m_A6.SetDocument(pATPDocCreuset);

	// ..... Tous + ascenseurs verticaux
	m_A7.SetDocument(pATPDocCreuset);
	m_A8.SetDocument(pATPDocCreuset);
	m_A9.SetDocument(pATPDocCreuset);

	// Composants Ref affinité
	//m_A10.SetDocument(pATPDocCreuset);
	//m_A11.SetDocument(pATPDocCreuset);

	// on met à jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

JVoid CATPPupitre::Update()
{
	// on met à jour les composants
	// Composant sources
	m_A1.Update();
	// m_A2.Update();
	m_ListTerrains.Update(); 

	// Composant cibles
	m_A3.Update();
	// m_A4.Update();
	m_ListCibles.Update(); 

	// Composant formats
	m_A5.Update();
	m_A6.Update();

	// ..... Tous + ascenseurs verticaux
	m_A7.Update();
	m_A8.Update();
	m_A9.Update();

	// Composants réf affinité
	//m_A10.Update();
	//m_A11.Update();

	// on met à jour le composant
	this->OnUpdate();
}

//////////////////
// le destructeur

CATPPupitre::~CATPPupitre()
{
	// on ne fait rien
}

///////////////////////////////////////////////////////////////////
// les fonctions pour récupérer les dimensions minimales du panneau

LONG CATPPupitre::GetCXMin()
{
	// on calcule la largeur des composants
	LONG CX1 = CATPPupitre_Metriques::CX_Ax();

	// on calcule la largeur des bordures et des interstices
	LONG CX2 = (2 * CATPPupitre_Metriques::CX_Border());

	// on renvoie la largeur
	return (CX1 + CX2);
}

LONG CATPPupitre::GetCYMin()
{
	// on calcule la hauteur des composants
	LONG CY1 = 4 * CATPPupitre_Metriques::CY_y1();
	CY1 += CATPPupitre_Metriques::CY_y2() + CATPPupitre_Metriques::CY_y3() + CATPPupitre_Metriques::CY_y4() + 2*CATPPupitre_Metriques::CY_y11();

	// on calcule la largeur des bordures et des interstices
	LONG CY2 = (2 * CATPPupitre_Metriques::CY_Border()) + (8 * CATPPupitre_Metriques::CY_Interstice());

	// on renvoie la hauteur
	return (CY1 + CY2);
}

BEGIN_MESSAGE_MAP(CATPPupitre, JFCSplitterPane)
	//{{AFX_MSG_MAP(CATPPupitre)
	ON_WM_CREATE()
	ON_LBN_SELCHANGE(IDC_CR_LIST_TERRAINPUPITRE, OnSelChangeListTerrain)
	ON_LBN_SELCHANGE(IDC_CR_LIST_CIBLEPUPITRE, OnSelChangeListCible)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////
// la fonction pour appliquer les mises à jour

JVoid CATPPupitre::OnUpdate()
{
	if(m_pATPDocCreuset && m_pATPDocCreuset->m_MapIdFormatsSel.GetCount() > 1)
	{
		// Bouton Tous et ascenseurs formats visibles
		m_A7.ShowWindow(SW_SHOW);
		m_A8.ShowWindow(SW_SHOW);
		m_A9.ShowWindow(SW_SHOW);
	}
	else
	{
		// Bouton Tous et ascenseurs formats non visibles
		m_A7.ShowWindow(SW_HIDE);
		m_A8.ShowWindow(SW_HIDE);
		m_A9.ShowWindow(SW_HIDE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CATPPupitre message handlers

int CATPPupitre::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	// on appelle le gestionnaire de base
	if (this->JFCPane::OnCreate(lpCreateStruct) == -1) return (-1);

	// on calcule la position des composants
	// Boite pupitre terrains
	LONG x1_A1 = CATPPupitre_Metriques::CX_Border();					LONG x2_A1 = x1_A1 + CATPPupitre_Metriques::CX_Ax();
	LONG y1_A1 = CATPPupitre_Metriques::CY_Border();					LONG y2_A1 = y1_A1 + CATPPupitre_Metriques::CY_y1();

	// Liste terrains
	LONG x1_A2 = x1_A1;													LONG x2_A2 = x2_A1;
	LONG y1_A2 = y2_A1 + CATPPupitre_Metriques::CY_Interstice();		LONG y2_A2 = y1_A2 + CATPPupitre_Metriques::CY_y2();

	// Boite pupitre cibles
	LONG x1_A3 = x1_A2;													LONG x2_A3 = x2_A2;
	LONG y1_A3 = y2_A2 + CATPPupitre_Metriques::CY_Interstice();		LONG y2_A3 = y1_A3 + CATPPupitre_Metriques::CY_y1();

	// Liste cibles
	LONG x1_A4 = x1_A3;													LONG x2_A4 = x2_A3;
	LONG y1_A4 = y2_A3 + CATPPupitre_Metriques::CY_Interstice();		LONG y2_A4 = y1_A4 + CATPPupitre_Metriques::CY_y3();

	LONG x1_A10 = x1_A4;												LONG x2_A10 = x2_A4;
	LONG y1_A10 = y2_A4 + CATPPupitre_Metriques::CY_Interstice();		LONG y2_A10 = y1_A10 + CATPPupitre_Metriques::CY_y1();
	LONG x1_A11 = x1_A10;												LONG x2_A11 = x2_A10;
	LONG y1_A11 = y2_A10 + CATPPupitre_Metriques::CY_Interstice();		LONG y2_A11 = y1_A11 + CATPPupitre_Metriques::CY_y11();

	// Boite pupitre formats
	LONG x1_A5 = x1_A11;												LONG x2_A5 = x2_A11;
	LONG y1_A5 = y2_A11 + CATPPupitre_Metriques::CY_Interstice();		LONG y2_A5 = y1_A5 + CATPPupitre_Metriques::CY_y1();
	LONG x1_A6 = x1_A5;													LONG x2_A6 = x2_A5;
	LONG y1_A6 = y2_A5 + CATPPupitre_Metriques::CY_Interstice();		LONG y2_A6 = y1_A6 + CATPPupitre_Metriques::CY_y4();
	
	// Coordonnés barre Tous et Ascenseurs formats
	LONG x1_A7 = x1_A5;													LONG x2_A7 = x1_A7 + CATPPupitre_Metriques::CX_x1();
	LONG y1_A7 = y2_A6 + CATPPupitre_Metriques::CY_Interstice();		LONG y2_A7 = y1_A7 + CATPPupitre_Metriques::CY_y11();
	LONG x1_A8 = x2_A7 ;												LONG x2_A8 = x1_A8 + CATPPupitre_Metriques::CX_x2();
	LONG y1_A8 = y1_A7;													LONG y2_A8 = y2_A7;
	LONG x1_A9 = x2_A8;													LONG x2_A9 = x1_A9 + CATPPupitre_Metriques::CX_x2();
	LONG y1_A9 = y1_A7;													LONG y2_A9 = y2_A7;

	// Composants Terrains
	if (m_A1. Create(NULL, WS_VISIBLE, CRect(x1_A1,  y1_A1, x2_A1,   y2_A1),  this, 0) == FALSE) return (-1);
	// if (m_A2. Create(NULL, WS_VISIBLE, CRect(x1_A2,  y1_A2, x2_A2,   y2_A2),  this, 0) == FALSE) return (-1);
	// if (m_ListTerrains.Create(WS_VISIBLE|LBS_DISABLENOSCROLL|LBS_HASSTRINGS|LBS_NOTIFY|LBS_NOINTEGRALHEIGHT|LBS_USETABSTOPS|LBS_OWNERDRAWVARIABLE|WS_TABSTOP|WS_VSCROLL,CRect(x1_A2,  y1_A2, x2_A2,   y2_A2), this, IDC_CR_LIST_TERRAINPUPITRE) == FALSE) return (-1);
	if (m_ListTerrains.Create(WS_VISIBLE|LBS_HASSTRINGS|LBS_NOTIFY|LBS_NOINTEGRALHEIGHT|LBS_USETABSTOPS|LBS_OWNERDRAWVARIABLE|WS_TABSTOP|WS_VSCROLL,CRect(x1_A2,  y1_A2, x2_A2,   y2_A2), this, IDC_CR_LIST_TERRAINPUPITRE) == FALSE) return (-1);
	m_ListTerrains.SetSelectColor(CATPColors::GetColorSelect(CATPColors::COLORCREUSET));
	
	// Modifie la font liste terrains
	LOGFONT lf;
	m_FontCreuset.CreatePointFont(CATPColors::GetFontSizeM(),CATPColors::GetFontLabelName());
	m_FontCreuset.GetLogFont(&lf);
	lf.lfWeight = FW_NORMAL;
	m_ListTerrains.SetFont(&m_FontCreuset,TRUE);
			
	// Composants cibles
	if (m_A3. Create(NULL, WS_VISIBLE, CRect(x1_A3,  y1_A3, x2_A3,   y2_A3),  this, 0) == FALSE) return (-1);
	//if (m_A4. Create(NULL, WS_VISIBLE, CRect(x1_A4,  y1_A4, x2_A4,   y2_A4),  this, 0) == FALSE) return (-1);
	//if (m_ListCibles.Create(WS_VISIBLE|LBS_DISABLENOSCROLL|LBS_HASSTRINGS|LBS_NOTIFY|LBS_NOINTEGRALHEIGHT|LBS_USETABSTOPS|LBS_OWNERDRAWVARIABLE|WS_TABSTOP|WS_VSCROLL ,CRect(x1_A4,  y1_A4, x2_A4,   y2_A4), this, IDC_CR_LIST_CIBLEPUPITRE) == FALSE) return (-1);
	//if (m_ListCibles.Create(WS_VISIBLE|LBS_HASSTRINGS|LBS_NOTIFY|LBS_NOINTEGRALHEIGHT|LBS_USETABSTOPS|LBS_OWNERDRAWVARIABLE|WS_TABSTOP|WS_VSCROLL ,CRect(x1_A4,  y1_A4, x2_A4,   y2_A4), this, IDC_CR_LIST_CIBLEPUPITRE) == FALSE) return (-1);
	if (m_ListCibles.Create(WS_VISIBLE|LBS_HASSTRINGS|LBS_NOTIFY|LBS_NOINTEGRALHEIGHT|LBS_USETABSTOPS|LBS_OWNERDRAWVARIABLE|WS_TABSTOP|WS_VSCROLL ,CRect(x1_A4,  y1_A4, x2_A4,   y2_A11), this, IDC_CR_LIST_CIBLEPUPITRE) == FALSE) return (-1);
	m_ListCibles.SetSelectColor(CATPColors::GetColorSelect(CATPColors::COLORCREUSET));

	// Modifie la font liste cibles
	m_ListCibles.SetFont(&m_FontCreuset,TRUE);

	if (m_A5. Create(NULL, WS_VISIBLE, CRect(x1_A5,  y1_A5, x2_A5,   y2_A5),  this, 0) == FALSE) return (-1);
	if (m_A6. Create(NULL, WS_VISIBLE, CRect(x1_A6,  y1_A6, x2_A6,   y2_A6),  this, 0) == FALSE) return (-1);

	// Composants Ref. Affinité
	//if (m_A10. Create(NULL, WS_VISIBLE, CRect(x1_A10,  y1_A10, x2_A10,   y2_A10),  this, 0) == FALSE) return (-1);
	//if (m_A11. Create(NULL, WS_VISIBLE, CRect(x1_A11,  y1_A11, x2_A11,   y2_A11),  this, 0) == FALSE) return (-1);

	// composants tous + ascenseurs formats
	if (m_A7. Create(NULL, WS_VISIBLE, CRect(x1_A7,  y1_A7, x2_A7,   y2_A7),  this, 0) == FALSE) return (-1);
	if (m_A8. Create(NULL, WS_VISIBLE, CRect(x1_A8,  y1_A8, x2_A8,   y2_A8),  this, 0) == FALSE) return (-1);
	if (m_A9. Create(NULL, WS_VISIBLE, CRect(x1_A9,  y1_A9, x2_A9,   y2_A9),  this, 0) == FALSE) return (-1);

	// Mode Size des composants pupitre
	// Composants terrains
	this->SetMode(&m_A1,  MGER_NONE, MGER_NONE);
	// this->SetMode(&m_A2,  MGER_NONE, MGER_NONE);
	this->SetMode(&m_ListTerrains,  MGER_NONE, MGER_NONE);

	// Composants cibles
	this->SetMode(&m_A3,  MGER_NONE, MGER_NONE);
	//this->SetMode(&m_A4,  MGER_NONE, MGER_SIZE);
	this->SetMode(&m_ListCibles,  MGER_NONE, MGER_SIZE);

	// Composants réf affinité
	//this->SetMode(&m_A10,  MGER_NONE, MGER_MOVE);
	//this->SetMode(&m_A11,  MGER_NONE, MGER_MOVE);

	// Panel liste formats resizable
	this->SetMode(&m_A5,  MGER_NONE, MGER_MOVE);
	this->SetMode(&m_A6,  MGER_NONE, MGER_MOVE);
 
	// Boutons tous et ascenseur format fixe
	this->SetMode(&m_A7,  MGER_NONE, MGER_MOVE);
	this->SetMode(&m_A8,  MGER_NONE, MGER_MOVE);
	this->SetMode(&m_A9,  MGER_NONE, MGER_MOVE);

	// on appelle la methode de definition des couleurs
	SetColors();

	// on quitte	
	return (0);
}

void CATPPupitre::SetColors()
{
	// m_A1.SetBorderColor(CATPColors::GetColorLight(CATPColors::COLORCREUSET));
	// m_A3.SetBorderColor(CATPColors::GetColorLight(CATPColors::COLORCREUSET));
	// m_A5.SetBorderColor(CATPColors::GetColorLight(CATPColors::COLORCREUSET));
	m_ListTerrains.SetSelectColor(CATPColors::GetColorSelect(CATPColors::COLORCREUSET));
	m_ListCibles.SetSelectColor(CATPColors::GetColorSelect(CATPColors::COLORCREUSET));
	Invalidate();
}

// Change sélection terrain
void CATPPupitre::OnSelChangeListTerrain()
{
	// Changement de terrain
	// m_ListTerrains.ChangeSelection();
	if (m_ListTerrains.ChangeSelection() == true)
	{
		// Update via la nouvelle sélection cible
		m_pATPDocCreuset->UpdateAllViews(UPDATE_SEL_TERRAIN);
	}
}

// Change sélection cible
void CATPPupitre::OnSelChangeListCible()
{
	// Changement de cible
	if (m_ListCibles.ChangeSelection() == true)
	{
		// Update via la nouvelle sélection cible
		m_pATPDocCreuset->UpdateAllViews(UPDATE_SEL_CIBLE);
	}	
}

