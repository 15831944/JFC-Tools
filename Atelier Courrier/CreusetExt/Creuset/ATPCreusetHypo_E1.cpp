// on inclut les définitions nécesaires
#include "stdafx.h"
#include "ATPCreusetHypo_Metriques.h"
#include "ATPCreusetHypo_E1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPCreusetHypo_E1::CATPCreusetHypo_E1()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour définir le document

JVoid CATPCreusetHypo_E1::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met à jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

JVoid CATPCreusetHypo_E1::Update()
{
	// on met à jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPCreusetHypo_E1::~CATPCreusetHypo_E1()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPCreusetHypo_E1, JFCGrid)
	//{{AFX_MSG_MAP(CATPCreusetHypo_E1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////
// la fonction pour appliquer les mises à jour

JVoid CATPCreusetHypo_E1::OnUpdate()
{
	// on initialise le nombre d'éléments
	LONG cx = 0;
	LONG cy = 0;

	// on vérifie la validité du document
	if (m_pATPDocCreuset != 0)
	{
		// on récupère le nombre d'éléments
		// cx = m_pATPDocCreuset->m_MaxHypothese;
		cx = 1;
		cy = 1;
	}

	// on met à jour le composant
	this->SetHorzCount(cx, FALSE);
	this->SetVertCount(cy, TRUE);
	if(m_pATPDocCreuset && m_pATPDocCreuset->m_AFFCreuset.GetAffQuickPlan())
		this->SetHorzItemSize(CATPCreusetHypo_Metriques::CX_CxQP(), TRUE, FALSE);
	this->SetVertItemSize(2*CATPCreusetHypo_Metriques::CY_x2(), TRUE, FALSE);
}

JVoid CATPCreusetHypo_E1::OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD)
{
	// on calcule le rectangle
	RECT Rect; 
	Rect.left	= rect.left - 3; 
	Rect.right	= rect.right + 3; 
	Rect.top	= rect.top - 2; 
	Rect.bottom = rect.bottom + 1;

	// on dessine le fond de la cellule
	CString TxtHeaderCol;
	TxtHeaderCol.FormatMessage(IDS_CR_PM); 
	
	if(m_pATPDocCreuset->m_AFFCreuset.GetAffQuickPlan())
		TxtHeaderCol.FormatMessage(IDS_CR_TARIF); 

	// on crée le pinceau pour les insertions en arrière plan
	// Font spécifique (bold et taille moyenne 12)
	LOGFONT lf;
	dc.GetCurrentFont()->GetLogFont(&lf);
	lf.lfWeight = FW_BOLD;
	CFont fontBold;
	fontBold.CreateFontIndirect(&lf);

	static COLORREF clr_degrad[CATPColors::m_NbGrad];
	CATPColors::GetColorDarkGraduation(CATPColors::COLORCREUSET, clr_degrad, CATPColors::m_NbGrad);
	CATPColors::FillRectGraduateWithSurround(dc, Rect, clr_degrad, CATPColors::m_NbGrad,CATPColors::STYLE_DEGRAD::FROM_TOP_DEGRAD, CATPColors::COLORCREUSET);
	
	LONG OldCol = dc.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCREUSET));

	dc.DrawText(_T(TxtHeaderCol), &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
	dc.SetTextColor(OldCol);
}

/////////////////////////////////////
// CATPCreusetHypo_E1 message handlers

void CATPCreusetHypo_E1::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCGrid::PreSubclassWindow();

	// on initialise la taille des cellules
	this->SetHorzItemSize(CATPCreusetHypo_Metriques::CX_Cx(), TRUE, FALSE);
	this->SetVertItemSize(CATPCreusetHypo_Metriques::CY_x2(), TRUE, FALSE);
}
