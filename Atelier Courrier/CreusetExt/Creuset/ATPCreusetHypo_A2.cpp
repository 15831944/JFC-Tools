// on inclut les définitions nécesaires
#include "stdafx.h"
#include "ATPCreusetHypo_Metriques.h"
#include "ATPCreusetHypo_A2.h"
#include "ATPCreusetHypo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPCreusetHypo_A2::CATPCreusetHypo_A2()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour définir le document

JVoid CATPCreusetHypo_A2::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met à jour le composant
	this->OnUpdate();
}

JVoid CATPCreusetHypo_A2::SetParent(CATPCreusetHypo* pATPCreusetHypo)
{
	m_pATPCreusetHypo = pATPCreusetHypo;
}


//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

JVoid CATPCreusetHypo_A2::Update()
{
	// on met à jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPCreusetHypo_A2::~CATPCreusetHypo_A2()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPCreusetHypo_A2, JFCGrid)
	//{{AFX_MSG_MAP(CATPCreusetHypo_A2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////
// la fonction pour dessiner l'intérieur des cellules

JVoid CATPCreusetHypo_A2::OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD)
{
	/* Avant
	// on récupère l'identifiant du Support via le plateau
	const CKeySupport & KeySupp = m_pATPDocCreuset->m_PlateauSupports.Item(j);
	const JUnt32 & IdSupport = KeySupp.m_IdSupport; 
	*/

	JUnt32 IdSupport;
	// on récupère l'identifiant du Support via le plateau
	const CKeySupport & KeySupp = m_pATPDocCreuset->m_PlateauSupports.Item(j);
	IdSupport = KeySupp.m_IdSupport; 

	// on calcule le rectangle
	RECT RectL; 
	RectL.left = rect.left + 18; 
	RectL.right = rect.right; 
	RectL.top = rect.top; 
	RectL.bottom = rect.bottom;

	CString Text;
	Text.Format(_T("Support %09d"), IdSupport);

	// on teste si on a trouvé le Support
	if (IdSupport)
	{
		// on récupère le Support
		CRegroupements * pReg = CRegroupements::GetInstance();
		IElem * pElem= pReg->GetIElemByID(IdSupport);
		if(pElem)
		{
			if(!KeySupp.m_IdParent)
			{
				if(pElem->IsCouplage())
					// Support Enfant decalé
					Text.Format("+ %s", pElem->GetLabel().AsJCharPtr());
				else
					// on écrit le libellé du Support
					Text = pElem->GetLabel().AsJCharPtr();
			}
			else
				// Support Enfant decalé
				Text.Format("---> %s", pElem->GetLabel().AsJCharPtr());
		}
	}

	// si support actif, couleur différente
	//if (j+1 == m_pATPCreusetHypo->m_NoLigSupportActif)
	//{
	//	// on crée le pinceau pour les insertions en arrière plan
	//	CBrush BrushB;
	//	BrushB.CreateSolidBrush(CATPColors::GetColorLight(CATPColors::COLORCREUSET));

	//	// on dessine le fond de la cellule
	//	CRect RectFond = rect;
	//	RectFond.top    -= 2;
	//	RectFond.bottom += 1;
	//	RectFond.left   -= 3;
	//	RectFond.right  += 3;
	//	dc.FillRect(&RectFond, &BrushB);
	//}

	// on écrit le libellé du Support
	dc.DrawText(Text, &rect, DT_LEFT | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);

}

////////////////////////////////////////////////////
// les fonctions pour capter les clicks de la souris

JVoid CATPCreusetHypo_A2::OnLButtonDown(LONG i, LONG j)
{

	// en mode plateau
	const CKeySupport & KeySupp			= m_pATPDocCreuset->m_PlateauSupports.Item(j);
	m_pATPCreusetHypo->m_IdSupportActif	= KeySupp.m_IdSupport; 
	m_pATPDocCreuset->m_MapIdCouplageExpanded.MoveTo(KeySupp.m_IdSupport);
	if(m_pATPDocCreuset->m_MapIdCouplageExpanded.IsValid())
		m_pATPDocCreuset->m_MapIdCouplageExpanded.GetItem() = !m_pATPDocCreuset->m_MapIdCouplageExpanded.GetItem();
	
	m_pATPDocCreuset->UpdateAllViews(UPDATE_KEY_PLATEAU); 

	m_pATPCreusetHypo->m_NoLigSupportActif = m_pATPDocCreuset->OnRecupLigneSuppActif(m_pATPCreusetHypo->m_IdSupportActif); 

	// Mise à jour composant
	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_HYPO);
}

JVoid CATPCreusetHypo_A2::OnRButtonDown(LONG i, LONG j)
{
	//// Support actif
	//m_pATPCreusetHypo->m_NoLigSupportActif = j+1;

	//// Mise à jour composant
	//m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_HYPO);
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises à jour

JVoid CATPCreusetHypo_A2::OnUpdate()
{
	// on initialise le nombre d'éléments
	LONG cx = 0;
	LONG cy = 0;

	// on vérifie la validité du document
	if (m_pATPDocCreuset != 0)
	{
		// on récupère le nombre d'éléments
		cx = 1;

		// en mode plateau
		cy = m_pATPDocCreuset->m_PlateauSupports.GetCount();
	}

	// on met à jour le composant
	this->SetHorzCount(cx, FALSE);
	this->SetVertCount(cy, TRUE);
}

/////////////////////////////////////
// CATPCreusetHypo_A2 message handlers

void CATPCreusetHypo_A2::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCGrid::PreSubclassWindow();

	// on initialise la taille des cellules
	this->SetHorzItemSize(CATPCreusetHypo_Metriques::CX_Ax(), TRUE, FALSE);
	this->SetVertItemSize(CATPCreusetHypo_Metriques::CY_x2(), TRUE, FALSE);
}
