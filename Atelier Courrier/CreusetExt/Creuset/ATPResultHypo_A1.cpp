// on inclut les définitions nécessaires
#include "stdafx.h"
#include "ATPResultHypo_Metriques.h"
#include "ATPResultHypo_A1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPResultHypo_A1::CATPResultHypo_A1()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour définir le document

JVoid CATPResultHypo_A1::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met à jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

JVoid CATPResultHypo_A1::Update()
{
	// on met à jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPResultHypo_A1::~CATPResultHypo_A1()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPResultHypo_A1, JFCGrid)
	//{{AFX_MSG_MAP(CATPResultHypo_A1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////
// la fonction pour dessiner l'intérieur des cellules

JVoid CATPResultHypo_A1::OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD)
{
	// on calcule le rectangle
	RECT Rect; Rect.left = rect.left - 3; Rect.right = rect.right + 3; Rect.top = rect.top - 2; Rect.bottom = rect.bottom + 1;

	// on dessine le fond de la cellule
	// dc.FillRect(&Rect, &BrushB);

	// const int NbGrad = 10;
	static COLORREF clr_degrad[CATPColors::m_NbGrad];
	CATPColors::GetColorDarkGraduation(CATPColors::COLORCREUSET, clr_degrad, CATPColors::m_NbGrad);

	CATPColors::FillRectGraduate(dc, Rect, clr_degrad, CATPColors::m_NbGrad,CATPColors::STYLE_DEGRAD::FROM_TOP_DEGRAD);

	// on écrit le titre
	CString str;
	if (m_pATPDocCreuset->m_AFFDistributionC)
		str.LoadString(IDS_CR_DISTRIBCUMUL);
	if (m_pATPDocCreuset->m_AFFDistributionN)
		str.LoadString(IDS_CR_DISTRIBNETTE);
	if (m_pATPDocCreuset->m_AFFVentilation.IsVisible())
		str.LoadString(IDS_CR_VENTIL);

	if (m_pATPDocCreuset->m_AFFContribution_New.IsVisible())
		str = m_pATPDocCreuset->m_AFFContribution_New.GetTypeStringContrib();

	if(str=="")
		str.LoadString(IDS_CR_PERF);

	LONG OldCol = dc.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCREUSET));


	dc.DrawText(_T(str), &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
	dc.SetTextColor(OldCol);
}

////////////////////////////////////////////////////
// les fonctions pour capter les clicks de la souris

JVoid CATPResultHypo_A1::OnLButtonDown(LONG i, LONG j)
{
	// on ne fait rien
}

JVoid CATPResultHypo_A1::OnRButtonDown(LONG i, LONG j)
{
	// on ne fait rien
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises à jour

JVoid CATPResultHypo_A1::OnUpdate()
{
	// on redessine la fenêtre
	this->InvalidateRect(NULL, FALSE);
}

///////////////////////////////////////
// CATPResultHypo_A1 message handlers

void CATPResultHypo_A1::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCGrid::PreSubclassWindow();

	// on initialise la taille des cellules
	this->SetHorzItemSize(CATPResultHypo_Metriques::CX_Ax(), TRUE, FALSE);
	this->SetVertItemSize(2*CATPResultHypo_Metriques::CY_x1(), TRUE, FALSE);

	// on initialise le nombre de cellules
	this->SetHorzCount(1, FALSE);
	this->SetVertCount(1, FALSE);
}
