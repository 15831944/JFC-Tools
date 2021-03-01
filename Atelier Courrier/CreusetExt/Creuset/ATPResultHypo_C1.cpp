// on inclut les d�finitions n�cessaires
#include "stdafx.h"
#include "ATPResultHypo_Metriques.h"
#include "ATPResult_Grandeurs.h"
#include "ATPResultHypo_C1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPResultHypo_C1::CATPResultHypo_C1()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour d�finir le document

JVoid CATPResultHypo_C1::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on m�morise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met � jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre � jour le composant

JVoid CATPResultHypo_C1::Update()
{
	// on met � jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPResultHypo_C1::~CATPResultHypo_C1()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPResultHypo_C1, JFCGrid)
	//{{AFX_MSG_MAP(CATPResultHypo_C1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////
// la fonction pour dessiner l'int�rieur des cellules

JVoid CATPResultHypo_C1::OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD)
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

	if (m_pATPDocCreuset->m_AFFContribution_New.IsVisible())
	{
		TxtHeaderCol.FormatMessage(IDS_CR_GRANDEUR); 
	}	

	// on cr�e le pinceau pour les insertions en arri�re plan
	CBrush BrushG;
	BrushG.CreateSolidBrush(CATPColors::GetColorMedium(CATPColors::COLORCREUSET));
	// dc.FillRect(&Rect, &BrushB);

	static COLORREF clr_degrad[CATPColors::m_NbGrad];
	CATPColors::GetColorDarkGraduation(CATPColors::COLORCREUSET, clr_degrad, CATPColors::m_NbGrad);

	CATPColors::FillRectGraduateWithSurround(dc, Rect, clr_degrad, CATPColors::m_NbGrad,CATPColors::STYLE_DEGRAD::FROM_TOP_DEGRAD, CATPColors::COLORCREUSET);
	
	LONG OldCol = dc.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCREUSET));

	dc.DrawText(_T(TxtHeaderCol), &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
	dc.SetTextColor(OldCol);

}

//////////////////////////////////////////////
// la fonction pour appliquer les mises � jour

JVoid CATPResultHypo_C1::OnUpdate()
{
	// on initialise le nombre d'�l�ments
	LONG xHyp		= 1;
	LONG yGrandeur	= 1;

	// on met � jour le composant
	this->SetHorzCount(xHyp, FALSE);
	this->SetVertCount(yGrandeur, TRUE);
}

///////////////////////////////////////
// CATPResultHypo_C1 message handlers

void CATPResultHypo_C1::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCGrid::PreSubclassWindow();

	// on initialise la taille des cellules
	this->SetHorzItemSize(CATPResultHypo_Metriques::CX_Bx(), TRUE, FALSE);
	this->SetVertItemSize(2*CATPResultHypo_Metriques::CY_x1(), TRUE, FALSE);
}
